#include "MyShopSettingPanel.h"
#include "GameScene.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "Player.h"
#include "Shop.h"
#include "ShopProduct.h"
#include "MouseOverMenuItem.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

MyShopSettingPanel::~MyShopSettingPanel()
{
	m_EmployeeCountText = nullptr;
	m_WorkHere = nullptr;
	m_FromHourText = nullptr;
	m_ToHourText = nullptr;
	m_Weekdays.clear();
}

void MyShopSettingPanel::openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint)
{
	if (!m_ThisPanel)
	{
		m_GameScene = scene;
		m_Player = GameData::getInstance().m_Player;

		if (m_GameScene && m_Player)
			createPanel(sceneMidPoint);

		return;
	}

	m_ThisPanel->setVisible(true);
}

void MyShopSettingPanel::closePanel()
{
}

void MyShopSettingPanel::createPanel(cocos2d::Vec2 sceneMidPoint)
{
	auto myShop = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]];

	// create this panel
	m_ThisPanel = Sprite::createWithSpriteFrameName("Brown_Panel_500_BlueLine.png");
	if (!m_ThisPanel)
		return;

	m_ThisPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(m_ThisPanel, 1);
	m_Elements.push_back(m_ThisPanel);

	auto panelMidPoint = Vec2(m_ThisPanel->getContentSize().width * 0.5f, m_ThisPanel->getContentSize().height * 0.5f);

	// shop picture, name
	auto shopPic = Sprite::createWithSpriteFrameName(myShop->m_ShopLook_Normal);
	if (shopPic)
	{
		GameFunctions::displaySprite(shopPic, Vec2(panelMidPoint.x + 170.f, panelMidPoint.y + 160.f), m_ThisPanel, 1, 0.6f, 0.6f);

		auto shopType = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (shopType)
		{
			shopType->setString(GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_ShopType);
			GameFunctions::displayLabel(shopType, Color4B::WHITE, Vec2(shopPic->getContentSize().width * 0.5f, -10.f), shopPic, 1);
		}
	}

	auto textAligmentLeft = panelMidPoint.x - 260.f;
	auto shopName = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
	if (shopName)
	{
		shopName->setString(myShop->m_Name);
		shopName->enableShadow(Color4B::BLACK);
		GameFunctions::displayLabel(shopName, Color4B::WHITE, Vec2(textAligmentLeft, panelMidPoint.y + 190.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	}

#pragma region Hire Employee button
	auto employeeText = Label::createWithTTF("Employees", "fonts/NirmalaB.ttf", 20);
	if (employeeText)
		GameFunctions::displayLabel(employeeText, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y + 150.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);

	auto boxSprite = Sprite::createWithSpriteFrameName("Border_Black_Square.png");

	if (boxSprite)
	{
		GameFunctions::displaySprite(boxSprite, Vec2(panelMidPoint.x - 120.f, panelMidPoint.y + 160.f), m_ThisPanel, 1);

		m_EmployeeCountText = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_EmployeeCountText)
		{
			m_EmployeeCountText->enableShadow(Color4B::BLACK);
			m_EmployeeCountText->setString(std::to_string(myShop->m_Employees));
			GameFunctions::displayLabel(m_EmployeeCountText, Color4B::WHITE, Vec2(boxSprite->getContentSize().width * 0.5f,
				boxSprite->getContentSize().height * 0.5f), boxSprite, 1);
		}
	}

	for (unsigned index = 0; index < 2; index++)
	{
		auto employeeButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(MyShopSettingPanel::reduceCallback, this))
			: MouseOverMenuItem::createUpperButton(CC_CALLBACK_1(MyShopSettingPanel::addCallback, this));
		if (employeeButton)
		{
			displayButtons(employeeButton, (index % 2 == 0) ? Vec2(sceneMidPoint.x - 105.f, sceneMidPoint.y + 150.f) :
				Vec2(sceneMidPoint.x - 135.f, sceneMidPoint.y + 170.f));
		}
	}

	auto hireButton = MouseOverMenuItem::creatMouseOverMenuButton("ButtonBlueNormal.png", "ButtonBlueLit.png", "ButtonBlueDisabled.png",
		CC_CALLBACK_1(MyShopSettingPanel::actionCallback, this));
	if (hireButton)
	{
		auto buttonPos = Vec2(sceneMidPoint.x - 50.f, sceneMidPoint.y + 160.f);
		hireButton->setScale(0.7f);
		hireButton->setPosition(buttonPos);
		hireButton->setItemRect(buttonPos, 0.7f);

		auto hireText = Label::createWithTTF("HIRE", "fonts/NirmalaB.ttf", 25);
		if (hireText)
			GameFunctions::displayLabel(hireText, GameData::getInstance().m_ColorType.Taro, Vec2(hireButton->getContentSize().width * 0.5f,
				hireButton->getContentSize().height * 0.5f), hireButton, 1);

		auto salaryText = Label::createWithTTF("$500/Person", "fonts/Nirmala.ttf", 15);
		if (salaryText)
			GameFunctions::displayLabel(salaryText, Color4B::BLACK, Vec2(panelMidPoint.x - 10.f, panelMidPoint.y + 150.f),
				m_ThisPanel, 1, true, TextHAlignment::LEFT);

		m_MenuItems.pushBack(hireButton);
	}
#pragma endregion

#pragma region Work here checkbox
	auto workHereText = Label::createWithTTF("I Work Here", "fonts/NirmalaB.ttf", 20);
	if (workHereText)
		GameFunctions::displayLabel(workHereText, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y + 100.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);

	m_WorkHere = ui::CheckBox::create("X/Checkbox_Normal.png", "X/Checkbox_Checked.png");
	m_WorkHere->setPosition(Vec2(panelMidPoint.x - 120.f, panelMidPoint.y + 110.f));
	m_WorkHere->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {});
	m_WorkHere->setSelected(true);

	m_ThisPanel->addChild(m_WorkHere, 1);
#pragma endregion

#pragma region work schedule
	auto workScheduleText = Label::createWithTTF("Work Schedule", "fonts/NirmalaB.ttf", 20);
	if (workScheduleText)
		GameFunctions::displayLabel(workScheduleText, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y + 40.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);

	// from
	auto fromText = Label::createWithTTF("From", "fonts/Nirmala.ttf", 18);
	if (fromText)
	{
		fromText->enableItalics();
		GameFunctions::displayLabel(fromText, Color4B::BLACK, Vec2(panelMidPoint.x - 100.f, panelMidPoint.y + 40.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	}

	auto fromBoxSprite = Sprite::createWithSpriteFrameName("Border_Black.png");
	auto fromBoxMidPoint = Vec2(fromBoxSprite->getContentSize().width * 0.5f, fromBoxSprite->getContentSize().height * 0.5f);
	if (fromBoxSprite)
	{
		GameFunctions::displaySprite(fromBoxSprite, Vec2(panelMidPoint.x, panelMidPoint.y + 50.f), m_ThisPanel, 1);

		auto minuteText = Label::createWithTTF(": 00", "fonts/Nirmala.ttf", 20);
		if (minuteText)
		{
			minuteText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(minuteText, Color4B::WHITE, Vec2(fromBoxMidPoint.x + 30.f, fromBoxMidPoint.y + 15.f),
				fromBoxSprite, 1, true, TextHAlignment::RIGHT);
		}

		m_FromHourText = Label::createWithTTF("08", "fonts/Nirmala.ttf", 20);
		if (m_FromHourText)
		{
			m_FromHourText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(m_FromHourText, Color4B::WHITE, Vec2(fromBoxMidPoint.x - 10.f, fromBoxMidPoint.y + 15.f),
				fromBoxSprite, 1, true, TextHAlignment::RIGHT);
		}
	}

	for (unsigned index = 0; index < 2; index++)
	{
		auto fromButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(MyShopSettingPanel::reduceTimeCallback, this, true))
			: MouseOverMenuItem::createUpperButton(CC_CALLBACK_1(MyShopSettingPanel::increaseTimeCallback, this, true));
		if (fromButton)
		{
			displayButtons(fromButton, (index % 2 == 0) ? Vec2(sceneMidPoint.x + 33.f, sceneMidPoint.y + 40.f) :
				Vec2(sceneMidPoint.x - 33.f, sceneMidPoint.y + 60.f));
		}
	}

	// to
	auto toText = Label::createWithTTF("To", "fonts/Nirmala.ttf", 18);
	if (toText)
	{
		toText->enableItalics();
		GameFunctions::displayLabel(toText, Color4B::BLACK, Vec2(panelMidPoint.x + 70.f, panelMidPoint.y + 40.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	}

	auto toBoxSprite = Sprite::createWithSpriteFrameName("Border_Black.png");
	if (toBoxSprite)
	{
		GameFunctions::displaySprite(toBoxSprite, Vec2(panelMidPoint.x + 150.f, panelMidPoint.y + 50.f), m_ThisPanel, 1);

		auto toMinuteText = Label::createWithTTF(": 00", "fonts/Nirmala.ttf", 20);
		if (toMinuteText)
		{
			toMinuteText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(toMinuteText, Color4B::WHITE, Vec2(fromBoxMidPoint.x + 30.f, fromBoxMidPoint.y + 15.f),
				toBoxSprite, 1, true, TextHAlignment::RIGHT);
		}

		m_ToHourText = Label::createWithTTF("17", "fonts/Nirmala.ttf", 20);
		if (m_ToHourText)
		{
			m_ToHourText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(m_ToHourText, Color4B::WHITE, Vec2(fromBoxMidPoint.x - 10.f, fromBoxMidPoint.y + 15.f),
				toBoxSprite, 1, true, TextHAlignment::RIGHT);
		}
	}

	for (unsigned index = 0; index < 2; index++)
	{
		auto toButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(MyShopSettingPanel::reduceTimeCallback, this, false))
			: MouseOverMenuItem::createUpperButton(CC_CALLBACK_1(MyShopSettingPanel::increaseTimeCallback, this, false));
		if (toButton)
		{
			displayButtons(toButton, (index % 2 == 0) ? Vec2(sceneMidPoint.x + 183.f, sceneMidPoint.y + 40.f) :
				Vec2(sceneMidPoint.x + 117.f, sceneMidPoint.y + 60.f));
		}
	}

	// weeks
	std::string weekdays[7] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
	auto checkboxPos = Vec2(panelMidPoint.x - 80.f, panelMidPoint.y - 20.f);
	for (unsigned index = 0; index < 7; index++)
	{
		auto checkbox = ui::CheckBox::create("X/Checkbox_Normal.png", "X/Checkbox_Checked.png");
		checkbox->setPosition(checkboxPos);
		checkbox->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {});
		(index > 4) ? checkbox->setSelected(false) : checkbox->setSelected(true);

		m_Weekdays.push_back(checkbox);
		m_ThisPanel->addChild(checkbox, 1);
		checkboxPos.x += 50.f;

		auto text = Label::createWithTTF(weekdays[index], "fonts/Nirmala.ttf", 15);
		if (text)
			GameFunctions::displayLabel(text, Color4B::BLACK, Vec2(checkbox->getContentSize().width * 0.5f,
				checkbox->getContentSize().height + 10.f), checkbox, 1);
	}
#pragma endregion

#pragma region Shop Products
	auto productText = Label::createWithTTF("Products", "fonts/NirmalaB.ttf", 20);
	if (productText)
		GameFunctions::displayLabel(productText, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y - 60.f), m_ThisPanel, 1,
			true, TextHAlignment::LEFT);

	auto myShopProducts = myShop->m_Products;
	auto productLength = myShopProducts.size();
	auto productSpritePos = Vec2(textAligmentLeft + 30.f, panelMidPoint.y - 80.f);
	auto productButtonPos = Vec2(sceneMidPoint.x + 15.f, sceneMidPoint.y - 90.f);
	for (unsigned productIndex = 0; productIndex < productLength; productIndex++)
	{
		// product pic
		auto productSprite = Sprite::createWithSpriteFrameName(myShopProducts[productIndex]->m_ProductSpritePath);
		if (!productSprite)
			continue;

		GameFunctions::displaySprite(productSprite, productSpritePos, m_ThisPanel, 1, 0.5f, 0.5f);

		// product name
		auto productName = Label::createWithTTF(myShopProducts[productIndex]->m_Name, "fonts/Nirmala.ttf", 15);
		if (!productName)
			continue;

		GameFunctions::displayLabel(productName, Color4B::BLACK, Vec2(productSpritePos.x + 50.f, productSpritePos.y - 10.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);

		// product quantity with add/reduce buttons
		auto boxSprite = Sprite::createWithSpriteFrameName("Border_Black_Square.png");
		if (!boxSprite)
			continue;

		GameFunctions::displaySprite(boxSprite, Vec2(productSpritePos.x + 230.f, productSpritePos.y), m_ThisPanel, 1);

		auto productCountText = Label::createWithTTF(std::to_string(myShopProducts[productIndex]->m_Quantity), "fonts/Nirmala.ttf", 20);
		if (!productCountText)
			continue;

		GameFunctions::displayLabel(productCountText, Color4B::WHITE, Vec2(boxSprite->getContentSize().width * 0.5f,
			boxSprite->getContentSize().height * 0.5f), boxSprite, 1);

		m_ProductCountText.push_back(productCountText);

		for (unsigned index = 0; index < 2; index++)
		{
			auto productButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(
				MyShopSettingPanel::reduceProductAmoutCallback, this, productIndex)) : MouseOverMenuItem::createUpperButton(
					CC_CALLBACK_1(MyShopSettingPanel::increaseProductAmoutCallback, this, productIndex));
			if (productButton)
			{
				displayButtons(productButton, (index % 2 == 0) ? productButtonPos :
					Vec2(productButtonPos.x -30.f, productButtonPos.y + 40.f));
			}
			productButtonPos.y -= 20.f;
		}
		productSpritePos.y -= 40.f;
	}
#pragma endregion

	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(menu, 1);
	m_Elements.push_back(menu);
}

void MyShopSettingPanel::reduceCallback(cocos2d::Ref* pSender)
{
	m_EmployeeCount = GameFunctions::displayLabelText_ClampValue(m_EmployeeCountText, m_EmployeeCount, -1, 0, 5);
	m_EmployeeCountText->setString(std::to_string(m_EmployeeCount));
}

void MyShopSettingPanel::addCallback(cocos2d::Ref* pSender)
{
	m_EmployeeCount = GameFunctions::displayLabelText_ClampValue(m_EmployeeCountText, m_EmployeeCount, 1, 0, 5);
	m_EmployeeCountText->setString(std::to_string(m_EmployeeCount));
}

void MyShopSettingPanel::reduceTimeCallback(cocos2d::Ref* pSender, bool fromHourButton)
{
	if (fromHourButton)
	{
		m_FromHour = GameFunctions::displayLabelText_ClampValue(m_FromHourText, m_FromHour, -1, 0, 24);
		GameFunctions::updatLabelText_TimeFormat(m_FromHourText, m_FromHour);
		return;
	}
	m_ToHour = GameFunctions::displayLabelText_ClampValue(m_ToHourText, m_ToHour, -1, 0, 24);
	GameFunctions::updatLabelText_TimeFormat(m_ToHourText, m_ToHour);
}

void MyShopSettingPanel::increaseTimeCallback(cocos2d::Ref* pSender, bool fromHourButton)
{
	if (fromHourButton)
	{
		m_FromHour = GameFunctions::displayLabelText_ClampValue(m_FromHourText, m_FromHour, 1, 0, 24);
		GameFunctions::updatLabelText_TimeFormat(m_FromHourText, m_FromHour);
		return;
	}
	m_ToHour = GameFunctions::displayLabelText_ClampValue(m_ToHourText, m_ToHour, 1, 0, 24);
	GameFunctions::updatLabelText_TimeFormat(m_ToHourText, m_ToHour);

}

void MyShopSettingPanel::reduceProductAmoutCallback(cocos2d::Ref* pSender, unsigned productIndex)
{
	auto originalQuantity = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Products[productIndex]->m_Quantity;
	originalQuantity = GameFunctions::displayLabelText_ClampValue(m_ProductCountText[productIndex], originalQuantity, -20, 0, 100);
	m_ProductCountText[productIndex]->setString(std::to_string(originalQuantity));
	GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Products[productIndex]->m_Quantity = originalQuantity;
}

void MyShopSettingPanel::increaseProductAmoutCallback(cocos2d::Ref* pSender, unsigned productIndex)
{
	auto originalQuantity = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Products[productIndex]->m_Quantity;
	originalQuantity = GameFunctions::displayLabelText_ClampValue(m_ProductCountText[productIndex], originalQuantity, 20, 0, 100);
	m_ProductCountText[productIndex]->setString(std::to_string(originalQuantity));
	GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Products[productIndex]->m_Quantity = originalQuantity;
}

void MyShopSettingPanel::actionCallback(cocos2d::Ref* pSender)
{
}

void MyShopSettingPanel::onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
}

void MyShopSettingPanel::displayButtons(MouseOverMenuItem* button, Vec2 pos)
{
	button->onMouseOver = CC_CALLBACK_2(MyShopSettingPanel::onMouseOver, this);
	button->setScale(0.5f);
	button->setPosition(pos);
	button->setItemRect(pos, 0.5f);

	m_MenuItems.pushBack(button);
}
