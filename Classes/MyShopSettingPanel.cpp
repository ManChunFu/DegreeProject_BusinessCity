#include "MyShopSettingPanel.h"
#include "GameScene.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "Player.h"
#include "Shop.h"
#include "ShopProduct.h"
#include "MouseOverMenuItem.h"
#include "ui/CocosGUI.h"
#include "PurchaseProductData.h"
#include "ui/UIWidget.h"

USING_NS_CC;

MyShopSettingPanel::~MyShopSettingPanel()
{
	m_EmployeeCountText = nullptr;
	m_WorkHere = nullptr;
	m_FromHourText = nullptr;
	m_ToHourText = nullptr;
	m_Weekdays.clear();
	m_MyShop = nullptr;
	m_ProductWidget1 = nullptr;
	m_ProductWidget2 = nullptr;
	m_MyShop = nullptr;

	m_CurrentProductQuantityTexts.clear();

	for (auto product : m_PurchaseProducts)
	{
		delete product;
	}
	m_PurchaseProducts.clear();

	m_WidgetMenu.clear();
}

void MyShopSettingPanel::openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint)
{
	m_IsPanelOpen = true;
	GameData::getInstance().setTempOpenPanel(this);

	if (!m_ThisPanel)
	{
		m_GameScene = scene;
		m_Player = GameData::getInstance().m_Player;

		if (m_GameScene && m_Player)
			createPanel(sceneMidPoint);

		return;
	}

	m_ThisPanel->setVisible(true);
	(m_DisplayWidget2) ? enableMenuItems(m_WidgetMenu, true) : enableMenuItems(m_MenuItems, true);

}

void MyShopSettingPanel::closePanel()
{
	m_ThisPanel->setVisible(false);

	(m_DisplayWidget2)? enableMenuItems(m_WidgetMenu, false) : enableMenuItems(m_MenuItems, false);

	m_IsPanelOpen = false;

	GameData::getInstance().m_TempOpenPanel = nullptr;
}

void MyShopSettingPanel::createPanel(cocos2d::Vec2 sceneMidPoint)
{
	m_MyShop = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]];

	// create this panel
	m_ThisPanel = Sprite::createWithSpriteFrameName("Brown_Panel_500_BlueLine_SquareCorner.png");
	if (!m_ThisPanel)
		return;

	m_ThisPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(m_ThisPanel, 1);
	m_Elements.push_back(m_ThisPanel);

	auto panelMidPoint = Vec2(m_ThisPanel->getContentSize().width * 0.5f, m_ThisPanel->getContentSize().height * 0.5f);

	// create close button
	auto closePanelButton = MouseOverMenuItem::creatMouseOverMenuButton("CloseButton_Normal.png", "CloseButton_Lit.png", "CloseButton_Disable.png",
		CC_CALLBACK_1(MyShopSettingPanel::closeCallback, this));
	if (closePanelButton)
		displayButtons(closePanelButton, Vec2(sceneMidPoint.x + 275.f, sceneMidPoint.y + 225.f), itemTypes::DEFAULT, 0.7f);

	// shop picture, name
	auto shopPic = Sprite::createWithSpriteFrameName(m_MyShop->m_ShopLook_Normal);
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
		shopName->setString(m_MyShop->m_Name);
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
			m_EmployeeCountText->setString(std::to_string(m_MyShop->m_Employees));
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
				Vec2(sceneMidPoint.x - 135.f, sceneMidPoint.y + 170.f), itemTypes::BUTTON);
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

		auto hireText = Label::createWithTTF("HIRE", "fonts/NirmalaB.ttf", 20);
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
				Vec2(sceneMidPoint.x - 33.f, sceneMidPoint.y + 60.f), itemTypes::BUTTON);
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
				Vec2(sceneMidPoint.x + 117.f, sceneMidPoint.y + 60.f), itemTypes::BUTTON);
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
	m_ProductWidget1 = ui::Widget::create();
	if (m_ProductWidget1)
	{
		m_ProductWidget1->setPosition(Vec2(panelMidPoint.x - 300.f, panelMidPoint.y - 250.f));
		m_ThisPanel->addChild(m_ProductWidget1, 1);
	}

	auto productText = Label::createWithTTF("Products", "fonts/NirmalaB.ttf", 20);
	if (productText)
		GameFunctions::displayLabel(productText, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y - 85.f), m_ThisPanel, 1,
			true, TextHAlignment::LEFT);

	// current QTY in shop
	auto qtyText = Label::createWithTTF("QTY", "fonts/NirmalaB.ttf", 15);
	if (qtyText)
		GameFunctions::displayLabel(qtyText, Color4B::BLACK, Vec2(panelMidPoint.x - 5.f, panelMidPoint.y - 75.f), m_ThisPanel, 1);

	// purchase QTY
	auto purchaseQTYText = Label::createWithTTF("Purchase QTY", "fonts/NirmalaB.ttf", 15);
	if (purchaseQTYText)
		GameFunctions::displayLabel(purchaseQTYText, Color4B::BLACK, Vec2(panelMidPoint.x + 110.f, panelMidPoint.y - 75.f),
			m_ThisPanel, 1);

	auto productLength = m_MyShop->m_Products.size();
	auto productSpritePos = Vec2(textAligmentLeft + 30.f, panelMidPoint.y - 115.f);
	auto productButtonPos = Vec2(sceneMidPoint.x + 85.f, sceneMidPoint.y - 125.f);

	for (unsigned productIndex = 0; productIndex < productLength; productIndex++)
	{
		if (productIndex > m_PanelLimit)
		{
			auto moreButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonMore50.png", "UIButtonMore50_Lit.png", 
				"UIButtonMore50_Disable.png", CC_CALLBACK_1(MyShopSettingPanel::actionCallback, this));
			if (moreButton)
			{
				moreButton->itemSelectedData.type = itemTypes::BUTTON;
				displayButtons(moreButton, Vec2(sceneMidPoint.x - 130.f, 135.f), itemTypes::BUTTON);
			}
			break;
		}

#pragma region create product pics, name, quantity and buy button
		// product pic
		auto productSprite = Sprite::createWithSpriteFrameName(m_MyShop->m_Products[productIndex]->m_ProductSpritePath);
		if (productSprite)
			GameFunctions::displaySprite(productSprite, productSpritePos, m_ProductWidget1, 1, 0.5f, 0.5f);

		// product name
		auto productName = Label::createWithTTF(m_MyShop->m_Products[productIndex]->m_Name, "fonts/Nirmala.ttf", 15);
		if (productName)
			GameFunctions::displayLabel(productName, Color4B::BLACK, Vec2(productSpritePos.x + 50.f, productSpritePos.y - 10.f),
				m_ProductWidget1, 1, true, TextHAlignment::LEFT);

		// product quantity in shop
		auto currentProductCountText = Label::createWithTTF(std::to_string(m_MyShop->m_Products[productIndex]->m_Quantity), 
			"fonts/Nirmala.ttf", 15);
		if (currentProductCountText)
		{
			currentProductCountText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(currentProductCountText, Color4B::WHITE, Vec2(productSpritePos.x + 225.f, productSpritePos.y),
				m_ProductWidget1, 1);
			m_CurrentProductQuantityTexts.push_back(currentProductCountText);
		}

		// product quantity with add/reduce buttons
		auto boxSprite = Sprite::createWithSpriteFrameName("Border_Black_Square.png");
		if (boxSprite)
			GameFunctions::displaySprite(boxSprite, Vec2(productSpritePos.x + 300.f, productSpritePos.y), m_ProductWidget1, 1);

		auto productCountText = Label::createWithTTF(std::to_string(m_MyShop->m_Products[productIndex]->m_Quantity), 
			"fonts/Nirmala.ttf", 15);
		if (productCountText)
		{
			productCountText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(productCountText, Color4B::WHITE, Vec2(boxSprite->getContentSize().width * 0.5f,
				boxSprite->getContentSize().height * 0.5f), boxSprite, 1);

			m_PurchaseProducts.push_back(new PurchaseProductData(m_MyShop->m_Products[productIndex]->m_ProductId, productCountText, 20));
		}

		for (unsigned index = 0; index < 2; index++)
		{
			auto productButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(
				MyShopSettingPanel::reduceProductAmoutCallback, this, productIndex)) : MouseOverMenuItem::createUpperButton(
					CC_CALLBACK_1(MyShopSettingPanel::increaseProductAmoutCallback, this, productIndex));
			if (productButton)
			{
				productButton->itemSelectedData.type = itemTypes::BUTTON;
				displayButtons(productButton, (index % 2 == 0) ? productButtonPos :
					Vec2(productButtonPos.x -30.f, productButtonPos.y + 40.f), itemTypes::BUTTON);
			}
			productButtonPos.y -= 20.f;
		}

		// buy button
		auto buyButton = MouseOverMenuItem::creatMouseOverMenuButton(CC_CALLBACK_1(MyShopSettingPanel::buyProductCallback, this,
			productIndex));
		if (buyButton)
		{
			displayButtons(buyButton, Vec2(panelMidPoint.x + 480.f, productSpritePos.y + 110.f), itemTypes::BUTTON, 0.7f);
			buyButton->itemSelectedData.type = itemTypes::BUTTON;

			auto buyText = Label::createWithTTF("BUY", "fonts/NirmalaB.ttf", 20);
			if (buyText)
				GameFunctions::displayLabel(buyText, GameData::getInstance().m_ColorType.Taro, Vec2(buyButton->getContentSize().width * 0.5f,
					buyButton->getContentSize().height * 0.5f), buyButton, 1);
		}

		// productPurchasePrice
		auto cashSymbol = Label::createWithTTF("$", "fonts/NirmalaB.ttf", 18);
		if (cashSymbol)
			GameFunctions::displayLabel(cashSymbol, Color4B::BLACK, Vec2(panelMidPoint.x + 190.f, productSpritePos.y - 10.f),
				m_ProductWidget1, 1, true, TextHAlignment::LEFT);
		
		auto priceText = Label::createWithTTF(std::to_string(m_MyShop->m_Products[productIndex]->m_PurchasePrice), 
			"fonts/NirmalaB.ttf", 18);
		if (priceText)
			GameFunctions::displayLabel(priceText, Color4B::BLACK, Vec2(panelMidPoint.x + 220.f, productSpritePos.y + 15.f),
				m_ProductWidget1, 1, true, TextHAlignment::RIGHT);

		auto pieceText = Label::createWithTTF("/PCS", "fonts/NirmalaB.ttf", 16);
		if (pieceText)
			GameFunctions::displayLabel(pieceText, Color4B::BLACK, Vec2(panelMidPoint.x + 225.f, productSpritePos.y - 6.f),
				m_ProductWidget1, 1, true, TextHAlignment::LEFT);

#pragma endregion
		productSpritePos.y -= 40.f;
	}
#pragma endregion

	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(menu, 1);
	m_Elements.push_back(menu);
}

void MyShopSettingPanel::openProductWidget2()
{
	if (!m_ProductWidget2)
	{
		createProductWidget2();
		return;
	}

	enableWidget(m_ProductWidget2, true, m_WidgetMenu, itemTypes::WIDGET_BUTTON);
}

void MyShopSettingPanel::createProductWidget2()
{
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto panelMidPoint = Vec2(m_ThisPanel->getContentSize().width * 0.5f, m_ThisPanel->getContentSize().height * 0.5f);
	auto sceneMidPoint = Point((origin.x + Director::getInstance()->getVisibleSize().width / 2), 
		( origin.y + Director::getInstance()->getVisibleSize().height / 2));

	m_ProductWidget2 = ui::Widget::create();
	if (m_ProductWidget2)
	{
		m_ProductWidget2->setPosition(Vec2(panelMidPoint.x - 300.f, panelMidPoint.y - 250.f));
		m_ThisPanel->addChild(m_ProductWidget2, 1);
	}

	auto lessButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonLess50.png", "UIButtonLess50_Lit.png",
		"UIButtonLess50_Disable.png", CC_CALLBACK_1(MyShopSettingPanel::actionCallback, this));
	if (lessButton)
	{
		lessButton->itemSelectedData.type = itemTypes::WIDGET_BUTTON;
		displayButtons(lessButton, Vec2(sceneMidPoint.x - 130.f, 280.f), itemTypes::WIDGET_BUTTON);
	}

	auto productLength = m_MyShop->m_Products.size();
	auto productSpritePos = Vec2(panelMidPoint.x - 230.f, panelMidPoint.y - 115.f);
	auto productButtonPos = Vec2(sceneMidPoint.x + 85.f, sceneMidPoint.y - 125.f);

#pragma region create product pics, name, quantity and buy button
	for (unsigned productIndex = m_PanelLimit +1; productIndex < productLength; productIndex++)
	{
		// product pic
		auto productSprite = Sprite::createWithSpriteFrameName(m_MyShop->m_Products[productIndex]->m_ProductSpritePath);
		if (productSprite)
			GameFunctions::displaySprite(productSprite, productSpritePos, m_ProductWidget2, 1, 0.5f, 0.5f);

		// product name
		auto productName = Label::createWithTTF(m_MyShop->m_Products[productIndex]->m_Name, "fonts/Nirmala.ttf", 15);
		if (productName)
			GameFunctions::displayLabel(productName, Color4B::BLACK, Vec2(productSpritePos.x + 50.f, productSpritePos.y - 10.f),
				m_ProductWidget2, 1, true, TextHAlignment::LEFT);

		// product quantity in shop
		auto currentProductCountText = Label::createWithTTF(std::to_string(
			GameData::getInstance().getProductQuantity(m_MyShop->m_ShopId, productIndex)),
			"fonts/Nirmala.ttf", 15);
		if (currentProductCountText)
		{
			currentProductCountText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(currentProductCountText, Color4B::WHITE, Vec2(productSpritePos.x + 225.f, productSpritePos.y),
				m_ProductWidget2, 1);
			m_CurrentProductQuantityTexts.push_back(currentProductCountText);
		}

		// product quantity with add/reduce buttons
		auto boxSprite = Sprite::createWithSpriteFrameName("Border_Black_Square.png");
		if (boxSprite)
			GameFunctions::displaySprite(boxSprite, Vec2(productSpritePos.x + 300.f, productSpritePos.y), m_ProductWidget2, 1);

		auto productCountText = Label::createWithTTF(std::to_string(m_MyShop->m_Products[productIndex]->m_Quantity),
			"fonts/Nirmala.ttf", 15);
		if (productCountText)
		{
			productCountText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(productCountText, Color4B::WHITE, Vec2(boxSprite->getContentSize().width * 0.5f,
				boxSprite->getContentSize().height * 0.5f), boxSprite, 1);
			
			m_PurchaseProducts.push_back(new PurchaseProductData(m_MyShop->m_Products[productIndex]->m_ProductId, 
				productCountText, 20));
		}


		for (unsigned index = 0; index < 2; index++)
		{
			auto productButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(
				MyShopSettingPanel::reduceProductAmoutCallback, this, productIndex)) : MouseOverMenuItem::createUpperButton(
					CC_CALLBACK_1(MyShopSettingPanel::increaseProductAmoutCallback, this, productIndex));
			if (productButton)
			{
				productButton->itemSelectedData.type = itemTypes::WIDGET_BUTTON;
				displayButtons(productButton, (index % 2 == 0) ? productButtonPos :
					Vec2(productButtonPos.x - 30.f, productButtonPos.y + 40.f), itemTypes::WIDGET_BUTTON);
			}
			productButtonPos.y -= 20.f;
		}

		// buy button
		auto buyButton = MouseOverMenuItem::creatMouseOverMenuButton(CC_CALLBACK_1(MyShopSettingPanel::buyProductCallback, this,
			productIndex));
		if (buyButton)
		{
			displayButtons(buyButton, Vec2(panelMidPoint.x + 480.f, productSpritePos.y + 110.f), itemTypes::WIDGET_BUTTON, 0.7f);
			buyButton->itemSelectedData.type = itemTypes::WIDGET_BUTTON;

			auto buyText = Label::createWithTTF("BUY", "fonts/NirmalaB.ttf", 20);
			if (buyText)
				GameFunctions::displayLabel(buyText, GameData::getInstance().m_ColorType.Taro, Vec2(buyButton->getContentSize().width * 0.5f,
					buyButton->getContentSize().height * 0.5f), buyButton, 1);
		}

		// productPurchasePrice
		auto cashSymbol = Label::createWithTTF("$", "fonts/NirmalaB.ttf", 18);
		if (cashSymbol)
			GameFunctions::displayLabel(cashSymbol, Color4B::BLACK, Vec2(panelMidPoint.x + 190.f, productSpritePos.y - 10.f),
				m_ProductWidget2, 1, true, TextHAlignment::LEFT);

		auto priceText = Label::createWithTTF(std::to_string(m_MyShop->m_Products[productIndex]->m_PurchasePrice),
			"fonts/NirmalaB.ttf", 18);
		if (priceText)
			GameFunctions::displayLabel(priceText, Color4B::BLACK, Vec2(panelMidPoint.x + 220.f, productSpritePos.y + 15.f),
				m_ProductWidget2, 1, true, TextHAlignment::RIGHT);

		auto pieceText = Label::createWithTTF("/PCS", "fonts/NirmalaB.ttf", 16);
		if (pieceText)
			GameFunctions::displayLabel(pieceText, Color4B::BLACK, Vec2(panelMidPoint.x + 225.f, productSpritePos.y - 6.f),
				m_ProductWidget2, 1, true, TextHAlignment::LEFT);

#pragma endregion
		productSpritePos.y -= 40.f;
	}

	auto widgetMenu = Menu::createWithArray(m_WidgetMenu);
	widgetMenu->setPosition(Vec2::ZERO);
	m_Elements.push_back(widgetMenu);
	m_GameScene->addChild(widgetMenu, 1);
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
	m_PurchaseProducts[productIndex]->productId = productIndex;
	m_PurchaseProducts[productIndex]->purchaseCount = GameFunctions::displayLabelText_ClampValue(
		m_PurchaseProducts[productIndex]->productCountText,
		m_PurchaseProducts[productIndex]->purchaseCount, -20, 0, 100);
	m_PurchaseProducts[productIndex]->productCountText->setString(std::to_string(m_PurchaseProducts[productIndex]->purchaseCount));
}

void MyShopSettingPanel::increaseProductAmoutCallback(cocos2d::Ref* pSender, unsigned productIndex)
{
	m_PurchaseProducts[productIndex]->productId = productIndex;
	m_PurchaseProducts[productIndex]->purchaseCount = GameFunctions::displayLabelText_ClampValue(
		m_PurchaseProducts[productIndex]->productCountText,
		m_PurchaseProducts[productIndex]->purchaseCount, 20, 0, 100);
	m_PurchaseProducts[productIndex]->productCountText->setString(std::to_string(m_PurchaseProducts[productIndex]->purchaseCount));
}

void MyShopSettingPanel::actionCallback(cocos2d::Ref* pSender)
{
	m_DisplayWidget2 = !m_DisplayWidget2;
	if (m_DisplayWidget2)
	{
		enableWidget(m_ProductWidget1, false, m_MenuItems, itemTypes::BUTTON);
		openProductWidget2();
		return;
	}
	enableWidget(m_ProductWidget2, false, m_WidgetMenu, itemTypes::WIDGET_BUTTON);
	enableWidget(m_ProductWidget1, true, m_MenuItems, itemTypes::BUTTON);
}

void MyShopSettingPanel::buyProductCallback(cocos2d::Ref* pSender, unsigned productId)
{
	m_MyShop->m_Products[productId]->m_Quantity += m_PurchaseProducts[productId]->purchaseCount;
	m_CurrentProductQuantityTexts[productId]->setString(std::to_string(m_MyShop->m_Products[productId]->m_Quantity));

	// update shops data
	GameData::getInstance().setShopProductQuantity(m_MyShop->m_ShopId, productId, m_MyShop->m_Products[productId]->m_Quantity);
	// update player current cash amout
	auto totalPurchasePrice = GameData::getInstance().getProductPurchasePrice(m_MyShop->m_ShopId, productId) *
		m_PurchaseProducts[productId]->purchaseCount;
	m_Player->updateCurrentCashAmout(-totalPurchasePrice);
}

void MyShopSettingPanel::closeCallback(cocos2d::Ref* pSender)
{
	closePanel();
}

void MyShopSettingPanel::onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
}

void MyShopSettingPanel::displayButtons(MouseOverMenuItem* button, Vec2 pos, itemTypes type, float scale)
{
	button->onMouseOver = CC_CALLBACK_2(MyShopSettingPanel::onMouseOver, this);
	button->setScale(scale);
	button->setPosition(pos);
	button->setItemRect(pos, scale);

	if (type == itemTypes::WIDGET_BUTTON)
	{
		m_WidgetMenu.pushBack(button);
		return;
	}
	m_MenuItems.pushBack(button);
}

void MyShopSettingPanel::enableWidget(cocos2d::ui::Widget* widget, bool enable, Vector<MenuItem*>itemList, cocos2d::itemTypes type)
{
	widget->setVisible(enable);
	widget->setEnabled(enable);

	for (auto item : itemList)
	{
		if (item->itemSelectedData.type == type)
		{
			item->setVisible(enable);
			item->setEnabled(enable);
		}
	}
}
void MyShopSettingPanel::enableMenuItems(Vector<MenuItem*>itemList, bool enable)
{
	for (auto item : itemList)
	{
		item->setVisible(enable);
		item->setVisible(enable);
	}
}

