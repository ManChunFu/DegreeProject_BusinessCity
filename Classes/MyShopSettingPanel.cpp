#include "MyShopSettingPanel.h"
#include "GameScene.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "Player.h"
#include "Shop.h"
#include "MouseOverMenuItem.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

MyShopSettingPanel::~MyShopSettingPanel()
{
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
	m_ThisPanel = Sprite::createWithSpriteFrameName("Brown_Panel_500_BlueLine.png");
	if (!m_ThisPanel)
		return;

	m_ThisPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(m_ThisPanel, 1);
	m_Elements.push_back(m_ThisPanel);

	auto panelMidPoint = Vec2(m_ThisPanel->getContentSize().width * 0.5f, m_ThisPanel->getContentSize().height * 0.5f);

	auto shopPic = Sprite::createWithSpriteFrameName(GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_ShopLook_Normal);
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
		shopName->setString(GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Name);
		shopName->enableShadow(Color4B::BLACK);
		GameFunctions::displayLabel(shopName, Color4B::WHITE, Vec2(textAligmentLeft, panelMidPoint.y + 190.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	}

	auto employeeText = Label::createWithTTF("Employees", "fonts/NirmalaB.ttf", 20);
	if (employeeText)
		GameFunctions::displayLabel(employeeText, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y + 150.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);

#pragma region Hire Employee button
	auto boxSprite = Sprite::createWithSpriteFrameName("Border_White_Square.png");

	if (boxSprite)
	{
		GameFunctions::displaySprite(boxSprite, Vec2(panelMidPoint.x - 120.f, panelMidPoint.y + 160.f), m_ThisPanel, 1);

		m_EmployeeCountText = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_EmployeeCountText)
		{
			m_EmployeeCountText->setString(std::to_string(GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Employees));
			GameFunctions::displayLabel(m_EmployeeCountText, Color4B::WHITE, Vec2(boxSprite->getContentSize().width * 0.5f,
				boxSprite->getContentSize().height * 0.5f), boxSprite, 1);
		}
	}

	for (unsigned index = 0; index < 2; index++)
	{
		auto button = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(MyShopSettingPanel::reduceCallback, this))
			: MouseOverMenuItem::createUpperButton(CC_CALLBACK_1(MyShopSettingPanel::addCallback, this));
		if (button)
		{
			button->onMouseOver = CC_CALLBACK_2(MyShopSettingPanel::onMouseOver, this);
			Vec2 buttonPos = (index % 2 == 0) ? Vec2(sceneMidPoint.x - 105.f, sceneMidPoint.y + 150.f) :
				Vec2(sceneMidPoint.x - 135.f, sceneMidPoint.y + 170.f);
			button->setScale(0.5f);
			button->setPosition(buttonPos);
			button->setItemRect(buttonPos, 0.5f);

			m_MenuItems.pushBack(button);
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

	m_WorkHere = ui::CheckBox::create("X/Checkbox_Normal.png", "X/Checkbox_Normal_Press.png", "X/Checkbox_Active.png", "", "");
	m_WorkHere->setPosition(Vec2(panelMidPoint.x - 120.f, panelMidPoint.y + 110.f));
	m_WorkHere->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {});

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

	auto fromBoxSprite = Sprite::createWithSpriteFrameName("Border_Blue.png");
	auto fromBoxMidPoint = Vec2(fromBoxSprite->getContentSize().width * 0.5f, fromBoxSprite->getContentSize().height * 0.5f);

	if (fromBoxSprite)
	{
		GameFunctions::displaySprite(fromBoxSprite, Vec2(panelMidPoint.x, panelMidPoint.y + 50.f), m_ThisPanel, 1);

		auto minuteText = Label::createWithTTF(": 00", "fonts/Nirmala.ttf", 20);
		if (minuteText)
			GameFunctions::displayLabel(minuteText, Color4B::BLACK, Vec2(fromBoxMidPoint.x + 30.f, fromBoxMidPoint.y + 15.f), 
				fromBoxSprite, 1, true, TextHAlignment::RIGHT);

		m_FromHourText = Label::createWithTTF("08", "fonts/Nirmala.ttf", 20);
		if (m_FromHourText)
			GameFunctions::displayLabel(m_FromHourText, Color4B::BLACK, Vec2(fromBoxMidPoint.x -10.f, fromBoxMidPoint.y + 15.f),
				fromBoxSprite, 1, true, TextHAlignment::RIGHT);
	}

	for (unsigned index = 0; index < 2; index++)
	{
		auto button = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(MyShopSettingPanel::reduceTimeCallback, this))
			: MouseOverMenuItem::createUpperButton(CC_CALLBACK_1(MyShopSettingPanel::increaseTimeCallback, this));
		if (button)
		{
			button->onMouseOver = CC_CALLBACK_2(MyShopSettingPanel::onMouseOver, this);
			Vec2 buttonPos = (index % 2 == 0) ? Vec2(sceneMidPoint.x + 40.f, sceneMidPoint.y + 40.f) :
				Vec2(sceneMidPoint.x - 40.f, sceneMidPoint.y + 60.f);
			button->setScale(0.5f);
			button->setPosition(buttonPos);
			button->setItemRect(buttonPos, 0.5f);

			m_MenuItems.pushBack(button);
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

	auto toBoxSprite = Sprite::createWithSpriteFrameName("Border_Blue.png");
	if (toBoxSprite)
	{
		GameFunctions::displaySprite(toBoxSprite, Vec2(panelMidPoint.x + 150.f, panelMidPoint.y + 50.f), m_ThisPanel, 1);

		auto toMinuteText = Label::createWithTTF(": 00", "fonts/Nirmala.ttf", 20);
		if (toMinuteText)
			GameFunctions::displayLabel(toMinuteText, Color4B::BLACK, Vec2(fromBoxMidPoint.x + 30.f, fromBoxMidPoint.y + 15.f),
				toBoxSprite, 1, true, TextHAlignment::RIGHT);

		m_ToHourText = Label::createWithTTF("17", "fonts/Nirmala.ttf", 20);
		if (m_ToHourText)
			GameFunctions::displayLabel(m_ToHourText, Color4B::BLACK, Vec2(fromBoxMidPoint.x - 10.f, fromBoxMidPoint.y + 15.f),
				toBoxSprite, 1, true, TextHAlignment::RIGHT);
	}

	for (unsigned index = 0; index < 2; index++)
	{
		auto toButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(MyShopSettingPanel::reduceTimeCallback, this))
			: MouseOverMenuItem::createUpperButton(CC_CALLBACK_1(MyShopSettingPanel::increaseTimeCallback, this));
		if (toButton)
		{
			toButton->_ID = 15;
			toButton->onMouseOver = CC_CALLBACK_2(MyShopSettingPanel::onMouseOver, this);
			Vec2 buttonPos = (index % 2 == 0) ? Vec2(sceneMidPoint.x + 190.f, sceneMidPoint.y + 40.f) :
				Vec2(sceneMidPoint.x + 110.f , sceneMidPoint.y + 60.f);
			toButton->setScale(0.5f);
			toButton->setPosition(buttonPos);
			toButton->setItemRect(buttonPos, 0.5f);

			m_MenuItems.pushBack(toButton);
		}
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

void MyShopSettingPanel::reduceTimeCallback(cocos2d::Ref* pSender)
{
	if (pSender->_ID == 15)
	{
		m_ToHour = GameFunctions::displayLabelText_ClampValue(m_ToHourText, m_ToHour, -1, 0, 24);
		GameFunctions::updatLabelText_TimeFormat(m_ToHourText, m_ToHour);
		return;
	}
	m_FromHour = GameFunctions::displayLabelText_ClampValue(m_FromHourText, m_FromHour, -1, 0, 24);
	GameFunctions::updatLabelText_TimeFormat(m_FromHourText, m_FromHour);
}

void MyShopSettingPanel::increaseTimeCallback(cocos2d::Ref* pSender)
{
	if (pSender->_ID == 15)
	{
		m_ToHour = GameFunctions::displayLabelText_ClampValue(m_ToHourText, m_ToHour, 1, 0, 24);
		GameFunctions::updatLabelText_TimeFormat(m_ToHourText, m_ToHour);
		return;
	}

	m_FromHour = GameFunctions::displayLabelText_ClampValue(m_FromHourText, m_FromHour, 1, 0, 24);
	GameFunctions::updatLabelText_TimeFormat(m_FromHourText, m_FromHour);
}

void MyShopSettingPanel::actionCallback(cocos2d::Ref* pSender)
{
	m_OwnerWorkHere = m_WorkHere->isSelected();

}

void MyShopSettingPanel::onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
}
