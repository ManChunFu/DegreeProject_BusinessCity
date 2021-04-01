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
#include "GlobalTime.h"
#include "GameTime.h"

USING_NS_CC;

MyShopSettingPanel::~MyShopSettingPanel()
{
	m_EmployeeCountText = nullptr;
	m_FromHourText = nullptr;
	m_ToHourText = nullptr;
	m_Weekdays.clear();
	m_MyShop = nullptr;
	m_WorkStatesWidget = nullptr;
	m_WorkStateText = nullptr;
	m_ReplenishCountdownText = nullptr;
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
		m_GameTime = GameData::getInstance().m_GlobalTime->m_Gametime;
		m_Player = GameData::getInstance().m_Player;

		if (m_GameScene && m_Player)
			createPanel(sceneMidPoint);

		return;
	}

	updateShopProductData();
	m_ThisPanel->setVisible(true);
	(m_DisplayWidget2) ? enableMenuItems(m_WidgetMenu, true) : enableMenuItems(m_MenuItems, true);

}

void MyShopSettingPanel::closePanel()
{
	m_ThisPanel->setVisible(false);

	(m_DisplayWidget2) ? enableMenuItems(m_WidgetMenu, false) : enableMenuItems(m_MenuItems, false);

	m_IsPanelOpen = false;

	GameData::getInstance().m_TempOpenPanel = nullptr;
}

void MyShopSettingPanel::createPanel(cocos2d::Vec2 sceneMidPoint)
{
	m_MyShop = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]];
	m_MyShop->onQuantityChanges = CC_CALLBACK_3(MyShopSettingPanel::onQuantitytChanges, this);
	m_MyShop->onCountdownChanges = CC_CALLBACK_2(MyShopSettingPanel::onCountDownChanges, this);

	// create this panel
	m_ThisPanel = Sprite::createWithSpriteFrameName("Brown_Panel_700x500_BlueLine.png");
	if (!m_ThisPanel)
		return;

	m_ThisPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(m_ThisPanel, 1);
	m_Elements.pushBack(m_ThisPanel);

	auto panelMidPoint = Vec2(m_ThisPanel->getContentSize().width * 0.5f, m_ThisPanel->getContentSize().height * 0.5f);

	// create close button
	auto closePanelButton = MouseOverMenuItem::creatMouseOverMenuButton("CloseButton_Normal.png", "CloseButton_Lit.png", "CloseButton_Disable.png",
		CC_CALLBACK_1(MyShopSettingPanel::closeCallback, this));
	if (closePanelButton)
		displayButtons(closePanelButton, Vec2(sceneMidPoint.x + 320.f, sceneMidPoint.y + 220.f), itemTypes::DEFAULT, 0.7f);

	// shop picture, name
	auto shopPic = Sprite::createWithSpriteFrameName(m_MyShop->m_ShopLook_Normal);
	if (shopPic)
	{
		GameFunctions::displaySprite(shopPic, Vec2(panelMidPoint.x + 210.f, panelMidPoint.y + 160.f), m_ThisPanel, 1, 0.6f, 0.6f);

		auto shopType = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (shopType)
		{
			shopType->setString(GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_ShopType);
			GameFunctions::displayLabel(shopType, Color4B::WHITE, Vec2(shopPic->getContentSize().width * 0.5f, -10.f), shopPic, 1);
		}
	}

	auto textAligmentLeft = panelMidPoint.x - 320.f;
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
		GameFunctions::displaySprite(boxSprite, Vec2(panelMidPoint.x - 170.f, panelMidPoint.y + 160.f), m_ThisPanel, 1);

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
			displayButtons(employeeButton, (index % 2 == 0) ? Vec2(sceneMidPoint.x - 155.f, sceneMidPoint.y + 150.f) :
				Vec2(sceneMidPoint.x - 185.f, sceneMidPoint.y + 170.f), itemTypes::BUTTON);
		}
	}

	auto hireButton = MouseOverMenuItem::creatMouseOverMenuButton("ButtonBlueNormal.png", "ButtonBlueLit.png", "ButtonBlueDisabled.png",
		CC_CALLBACK_1(MyShopSettingPanel::hireCallback, this));
	if (hireButton)
	{
		displayButtons(hireButton, Vec2(sceneMidPoint.x - 95.f, sceneMidPoint.y + 160.f), itemTypes::DEFAULT, 0.7f);

		auto hireText = Label::createWithTTF("HIRE", "fonts/NirmalaB.ttf", 20);
		if (hireText)
			GameFunctions::displayLabel(hireText, GameData::getInstance().m_ColorType.Taro, Vec2(hireButton->getContentSize().width * 0.5f,
				hireButton->getContentSize().height * 0.5f), hireButton, 1);

		auto salaryText = Label::createWithTTF("$500/Person", "fonts/Nirmala.ttf", 15);
		if (salaryText)
			GameFunctions::displayLabel(salaryText, Color4B::BLACK, Vec2(panelMidPoint.x - 45.f, panelMidPoint.y + 150.f),
				m_ThisPanel, 1, true, TextHAlignment::LEFT);

	}
#pragma endregion

#pragma region Work here checkbox
	auto workHereText = Label::createWithTTF("I Work Here", "fonts/NirmalaB.ttf", 20);
	if (workHereText)
		GameFunctions::displayLabel(workHereText, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y + 100.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);

	auto workHereCheckbox = ui::CheckBox::create("X/Checkbox_Normal.png", "X/Checkbox_Checked.png");
	workHereCheckbox->setPosition(Vec2(panelMidPoint.x - 170.f, panelMidPoint.y + 110.f));
	workHereCheckbox->addClickEventListener(CC_CALLBACK_1(MyShopSettingPanel::workHereCallback, this));
	workHereCheckbox->setSelected(m_MyShop->m_PlayerWorkHere);

	m_ThisPanel->addChild(workHereCheckbox, 1);
#pragma endregion

#pragma region work states
	m_WorkStatesWidget = ui::Widget::create();
	m_WorkStatesWidget->setPosition(Vec2::ZERO);
	m_ThisPanel->addChild(m_WorkStatesWidget, 1);
	m_WorkStatesWidget->setVisible(m_MyShop->m_PlayerWorkHere);

	m_WorkStateText = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
	if (m_WorkStateText)
	{
		m_WorkStateText->setString(getWorkState());
		GameFunctions::displayLabel(m_WorkStateText, Color4B::GREEN, Vec2(panelMidPoint.x - 130, panelMidPoint.y + 100.f),
			m_WorkStatesWidget, 1, true, TextHAlignment::LEFT);
	}

	m_ReplenishCountdownText = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
	if (m_ReplenishCountdownText)
	{
		if (m_MyShop->isReplenishing())
			m_ReplenishCountdownText->setString(std::to_string(m_MyShop->getCountDown()));

		GameFunctions::displayLabel(m_ReplenishCountdownText, Color4B::RED, Vec2(panelMidPoint.x + 50.f, panelMidPoint.y + 125.f),
			m_WorkStatesWidget, 1, true, TextHAlignment::RIGHT);
	}
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

		m_FromHourText = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_FromHourText)
		{
			GameFunctions::updatLabelText_TimeFormat(m_FromHourText, m_MyShop->m_ShopOpenHour.first, true);
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

		m_ToHourText = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_ToHourText)
		{
			GameFunctions::updatLabelText_TimeFormat(m_ToHourText, m_MyShop->m_ShopOpenHour.second, true);
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
		checkbox->setSelected(m_MyShop->m_ShopOpenDay[index]);
		checkbox->addClickEventListener(CC_CALLBACK_1(MyShopSettingPanel::checkBoxClickCallback, this,
			index));

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
		m_ProductWidget1->setPosition(Vec2::ZERO);
		m_ThisPanel->addChild(m_ProductWidget1, 1);
	}

	auto productText = Label::createWithTTF("Products", "fonts/NirmalaB.ttf", 20);
	if (productText)
		GameFunctions::displayLabel(productText, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y - 85.f), m_ThisPanel, 1,
			true, TextHAlignment::LEFT);

	auto titlePos = Vec2(panelMidPoint.x - 40.f, panelMidPoint.y - 75.f);

	// product sale price
	auto salePriceText = Label::createWithTTF("Sale Price", "fonts/NirmalaB.ttf", 15);
	if (salePriceText)
		GameFunctions::displayLabel(salePriceText, Color4B::BLACK, titlePos, m_ThisPanel, 1);

	// current QTY in shop
	auto qtyText = Label::createWithTTF("QTY", "fonts/NirmalaB.ttf", 15);
	if (qtyText)
		GameFunctions::displayLabel(qtyText, Color4B::BLACK, Vec2(titlePos.x + 82.f, titlePos.y), m_ThisPanel, 1);

	// purchase QTY
	auto purchaseQTYText = Label::createWithTTF("Purchase QTY", "fonts/NirmalaB.ttf", 15);
	if (purchaseQTYText)
		GameFunctions::displayLabel(purchaseQTYText, Color4B::BLACK, Vec2(titlePos.x + 190.f, titlePos.y), m_ThisPanel, 1);

	auto productLength = m_MyShop->m_Products.size();
	auto productSpritePos = Vec2(textAligmentLeft + 30.f, panelMidPoint.y - 115.f);
	auto productButtonPos = Vec2(sceneMidPoint.x - 25.f, sceneMidPoint.y - 125.f);

	for (unsigned productIndex = 0; productIndex < productLength; productIndex++)
	{
		if (productIndex > m_PanelLimit)
		{
			auto moreButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonMore50.png", "UIButtonMore50_Lit.png",
				"UIButtonMore50_Disable.png", CC_CALLBACK_1(MyShopSettingPanel::openWidget2Callback, this));
			if (moreButton)
			{
				moreButton->itemSelectedData.type = itemTypes::BUTTON;
				displayButtons(moreButton, Vec2(sceneMidPoint.x - 160.f, 135.f), itemTypes::BUTTON);
				createProductWidget2(panelMidPoint, sceneMidPoint);
				enableWidget(m_ProductWidget2, false, m_WidgetMenu, itemTypes::WIDGET_BUTTON);
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

		// product sale price text
		auto priceBoxSprite = Sprite::createWithSpriteFrameName("Border_Black_Square.png");
		if (priceBoxSprite)
		{
			GameFunctions::displaySprite(priceBoxSprite, Vec2(productSpritePos.x + 250.f, productSpritePos.y), m_ProductWidget1, 1);

			auto productSalePrice = Label::createWithTTF("", "fonts/Nirmala.ttf", 15);
			if (productSalePrice)
			{
				productSalePrice->setString(std::to_string(m_MyShop->m_Products[productIndex]->m_SalePrice));
				GameFunctions::displayLabel(productSalePrice, Color4B::WHITE, (priceBoxSprite->getContentSize() * 0.5),
					priceBoxSprite, 1);
			}
		}

		// price reduce increase buttons
		for (unsigned index = 0; index < 2; index++)
		{
			auto priceButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(
				MyShopSettingPanel::reducePriceCallback, this, productIndex)) : MouseOverMenuItem::createUpperButton(
					CC_CALLBACK_1(MyShopSettingPanel::increasePriceCallback, this, productIndex));
			if (priceButton)
			{
				priceButton->itemSelectedData.type = itemTypes::BUTTON;
				displayButtons(priceButton, (index % 2 == 0) ? productButtonPos :
					Vec2(productButtonPos.x - 30.f, productButtonPos.y + 20.f), itemTypes::BUTTON);
			}
		}

		// product quantity text in shop
		auto currentProductCountText = Label::createWithTTF(std::to_string(m_MyShop->m_Products[productIndex]->m_Quantity),
			"fonts/Nirmala.ttf", 15);
		if (currentProductCountText)
		{
			currentProductCountText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(currentProductCountText, Color4B::WHITE, Vec2(productSpritePos.x + 330.f, productSpritePos.y),
				m_ProductWidget1, 1);
			m_CurrentProductQuantityTexts.push_back(currentProductCountText);
		}

		// purchase product quantity text
		auto boxSprite = Sprite::createWithSpriteFrameName("Border_Black_Square.png");
		if (boxSprite)
		{
			GameFunctions::displaySprite(boxSprite, Vec2(productSpritePos.x + 410.f, productSpritePos.y), m_ProductWidget1, 1);

			auto productCountText = Label::createWithTTF("20", "fonts/Nirmala.ttf", 15);
			if (productCountText)
			{
				productCountText->enableShadow(Color4B::BLACK);
				GameFunctions::displayLabel(productCountText, Color4B::WHITE, Vec2(boxSprite->getContentSize().width * 0.5f,
					boxSprite->getContentSize().height * 0.5f), boxSprite, 1);

				m_PurchaseProducts.push_back(new PurchaseProductData(m_MyShop->m_Products[productIndex]->m_ProductId, productCountText, 20));
			}
		}

		// product QTY reduce increase buttons
		for (unsigned index = 0; index < 2; index++)
		{
			auto productButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(
				MyShopSettingPanel::reduceProductAmoutCallback, this, productIndex)) : MouseOverMenuItem::createUpperButton(
					CC_CALLBACK_1(MyShopSettingPanel::increaseProductAmoutCallback, this, productIndex));
			if (productButton)
			{
				productButton->itemSelectedData.type = itemTypes::BUTTON;
				displayButtons(productButton, (index % 2 == 0) ? Vec2(productButtonPos.x +160.f, productButtonPos.y) :
					Vec2(productButtonPos.x +130.f, productButtonPos.y + 20.f), itemTypes::BUTTON);
			}
		}
		
		// set for next item pos;
		productButtonPos.y -= 40.f;

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
			GameFunctions::displayLabel(cashSymbol, Color4B::BLACK, Vec2(panelMidPoint.x + 240.f, productSpritePos.y - 10.f),
				m_ProductWidget1, 1, true, TextHAlignment::LEFT);

		auto priceText = Label::createWithTTF(std::to_string(m_MyShop->m_Products[productIndex]->m_PurchasePrice),
			"fonts/NirmalaB.ttf", 18);
		if (priceText)
			GameFunctions::displayLabel(priceText, Color4B::BLACK, Vec2(panelMidPoint.x + 270.f, productSpritePos.y + 15.f),
				m_ProductWidget1, 1, true, TextHAlignment::RIGHT);

		auto pieceText = Label::createWithTTF("/PCS", "fonts/NirmalaB.ttf", 16);
		if (pieceText)
			GameFunctions::displayLabel(pieceText, Color4B::BLACK, Vec2(panelMidPoint.x + 275.f, productSpritePos.y - 6.f),
				m_ProductWidget1, 1, true, TextHAlignment::LEFT);

#pragma endregion
		productSpritePos.y -= 40.f;
	}
#pragma endregion

	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(menu, 1);
	m_Elements.pushBack(menu);
}

void MyShopSettingPanel::createProductWidget2(Vec2 panelMidPoint, Vec2 sceneMidPoint)
{
	m_ProductWidget2 = ui::Widget::create();
	if (m_ProductWidget2)
	{
		m_ProductWidget2->setPosition(Vec2::ZERO);
		m_ThisPanel->addChild(m_ProductWidget2, 1);
	}

	auto lessButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonLess50.png", "UIButtonLess50_Lit.png",
		"UIButtonLess50_Disable.png", CC_CALLBACK_1(MyShopSettingPanel::openWidget2Callback, this));
	if (lessButton)
	{
		lessButton->itemSelectedData.type = itemTypes::WIDGET_BUTTON;
		displayButtons(lessButton, Vec2(sceneMidPoint.x - 160.f, 280.f), itemTypes::WIDGET_BUTTON);
	}

	auto productLength = m_MyShop->m_Products.size();
	auto productSpritePos = Vec2(panelMidPoint.x - 290.f, panelMidPoint.y - 115.f);
	auto productButtonPos = Vec2(sceneMidPoint.x - 25.f, sceneMidPoint.y - 125.f);

#pragma region create product pics, name, quantity and buy button
	for (unsigned productIndex = m_PanelLimit + 1; productIndex < productLength; productIndex++)
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

		// product sale price text
		auto priceBoxSprite = Sprite::createWithSpriteFrameName("Border_Black_Square.png");
		if (priceBoxSprite)
		{
			GameFunctions::displaySprite(priceBoxSprite, Vec2(productSpritePos.x + 250.f, productSpritePos.y), m_ProductWidget2, 1);

			auto productSalePrice = Label::createWithTTF("", "fonts/Nirmala.ttf", 15);
			if (productSalePrice)
			{
				productSalePrice->setString(std::to_string(m_MyShop->m_Products[productIndex]->m_SalePrice));
				GameFunctions::displayLabel(productSalePrice, Color4B::WHITE, (priceBoxSprite->getContentSize() * 0.5),
					priceBoxSprite, 1);
			}
		}

		// price reduce increase buttons
		for (unsigned index = 0; index < 2; index++)
		{
			auto priceButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(
				MyShopSettingPanel::reducePriceCallback, this, productIndex)) : MouseOverMenuItem::createUpperButton(
					CC_CALLBACK_1(MyShopSettingPanel::increasePriceCallback, this, productIndex));
			if (priceButton)
			{
				priceButton->itemSelectedData.type = itemTypes::WIDGET_BUTTON;
				displayButtons(priceButton, (index % 2 == 0) ? productButtonPos :
					Vec2(productButtonPos.x - 30.f, productButtonPos.y + 20.f), itemTypes::WIDGET_BUTTON);
			}
		}

		// product quantity text in shop
		auto currentProductCountText = Label::createWithTTF(std::to_string(m_MyShop->m_Products[productIndex]->m_Quantity),
			"fonts/Nirmala.ttf", 15);
		if (currentProductCountText)
		{
			currentProductCountText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(currentProductCountText, Color4B::WHITE, Vec2(productSpritePos.x + 330.f, productSpritePos.y),
				m_ProductWidget2, 1);
			m_CurrentProductQuantityTexts.push_back(currentProductCountText);
		}

		// purchase product quantity text
		auto boxSprite = Sprite::createWithSpriteFrameName("Border_Black_Square.png");
		if (boxSprite)
		{
			GameFunctions::displaySprite(boxSprite, Vec2(productSpritePos.x + 410.f, productSpritePos.y), m_ProductWidget2, 1);

			auto productCountText = Label::createWithTTF("20", "fonts/Nirmala.ttf", 15);
			if (productCountText)
			{
				productCountText->enableShadow(Color4B::BLACK);
				GameFunctions::displayLabel(productCountText, Color4B::WHITE, Vec2(boxSprite->getContentSize().width * 0.5f,
					boxSprite->getContentSize().height * 0.5f), boxSprite, 1);

				m_PurchaseProducts.push_back(new PurchaseProductData(m_MyShop->m_Products[productIndex]->m_ProductId, productCountText, 20));
			}
		}

		// product QTY reduce increase buttons
		for (unsigned index = 0; index < 2; index++)
		{
			auto productButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(
				MyShopSettingPanel::reduceProductAmoutCallback, this, productIndex)) : MouseOverMenuItem::createUpperButton(
					CC_CALLBACK_1(MyShopSettingPanel::increaseProductAmoutCallback, this, productIndex));
			if (productButton)
			{
				productButton->itemSelectedData.type = itemTypes::WIDGET_BUTTON;
				displayButtons(productButton, (index % 2 == 0) ? Vec2(productButtonPos.x + 160.f, productButtonPos.y) :
					Vec2(productButtonPos.x + 130.f, productButtonPos.y + 20.f), itemTypes::WIDGET_BUTTON);
			}
		}

		// set for next item pos;
		productButtonPos.y -= 40.f;


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
			GameFunctions::displayLabel(cashSymbol, Color4B::BLACK, Vec2(panelMidPoint.x + 240.f, productSpritePos.y - 10.f),
				m_ProductWidget2, 1, true, TextHAlignment::LEFT);

		auto priceText = Label::createWithTTF(std::to_string(m_MyShop->m_Products[productIndex]->m_PurchasePrice),
			"fonts/NirmalaB.ttf", 18);
		if (priceText)
			GameFunctions::displayLabel(priceText, Color4B::BLACK, Vec2(panelMidPoint.x + 270.f, productSpritePos.y + 15.f),
				m_ProductWidget2, 1, true, TextHAlignment::RIGHT);

		auto pieceText = Label::createWithTTF("/ PCS", "fonts/NirmalaB.ttf", 16);
		if (pieceText)
			GameFunctions::displayLabel(pieceText, Color4B::BLACK, Vec2(panelMidPoint.x + 275.f, productSpritePos.y - 6.f),
				m_ProductWidget2, 1, true, TextHAlignment::LEFT);

#pragma endregion
		productSpritePos.y -= 40.f;
	}

	auto widgetMenu = Menu::createWithArray(m_WidgetMenu);
	widgetMenu->setPosition(Vec2::ZERO);
	m_Elements.pushBack(widgetMenu);
	m_GameScene->addChild(widgetMenu, 1);
}

void MyShopSettingPanel::closeCallback(cocos2d::Ref* pSender)
{
	closePanel();
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

void MyShopSettingPanel::hireCallback(cocos2d::Ref* pSender)
{
	// TODO: implement hire
}

void MyShopSettingPanel::workHereCallback(cocos2d::Ref* pSender)
{
	GameData::getInstance().setPlayerWorkHere(m_MyShop->m_ShopId);

	auto originSetting = m_WorkStatesWidget->isVisible();
	m_WorkStatesWidget->setVisible(!originSetting);

	if (m_WorkStatesWidget->isVisible())
		m_WorkStateText->setString(getWorkState());
}

void MyShopSettingPanel::reduceTimeCallback(cocos2d::Ref* pSender, bool fromHourButton)
{
	if (fromHourButton)
	{
		m_FromHour = GameFunctions::displayLabelText_ClampValue(m_FromHourText, m_FromHour, -1, 0, 24);
		GameFunctions::updatLabelText_TimeFormat(m_FromHourText, m_FromHour, true);

		GameData::getInstance().setShopOpenHour(m_MyShop->m_ShopId, 0, m_FromHour);
		return;
	}
	m_ToHour = GameFunctions::displayLabelText_ClampValue(m_ToHourText, m_ToHour, -1, 0, 24);
	GameFunctions::updatLabelText_TimeFormat(m_ToHourText, m_ToHour, true);
	GameData::getInstance().setShopOpenHour(m_MyShop->m_ShopId, 1, m_ToHour);
}

void MyShopSettingPanel::increaseTimeCallback(cocos2d::Ref* pSender, bool fromHourButton)
{
	if (fromHourButton)
	{
		m_FromHour = GameFunctions::displayLabelText_ClampValue(m_FromHourText, m_FromHour, 1, 0, 24);
		GameFunctions::updatLabelText_TimeFormat(m_FromHourText, m_FromHour, true);

		GameData::getInstance().setShopOpenHour(m_MyShop->m_ShopId, 0, m_FromHour);
		return;
	}
	m_ToHour = GameFunctions::displayLabelText_ClampValue(m_ToHourText, m_ToHour, 1, 0, 24);
	GameFunctions::updatLabelText_TimeFormat(m_ToHourText, m_ToHour, true);
	GameData::getInstance().setShopOpenHour(m_MyShop->m_ShopId, 1, m_ToHour);
}

void MyShopSettingPanel::checkBoxClickCallback(cocos2d::Ref* pSender, unsigned weekday)
{
	GameData::getInstance().setShopOpenDay(m_MyShop->m_ShopId, weekday);
}

void MyShopSettingPanel::reducePriceCallback(cocos2d::Ref* pSender, unsigned productId)
{
}

void MyShopSettingPanel::increasePriceCallback(cocos2d::Ref* pSender, unsigned productId)
{
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

void MyShopSettingPanel::buyProductCallback(cocos2d::Ref* pSender, unsigned productId)
{
	// setup work state
	m_MyShop->m_IsReplenishing = true;
	updateShopWorkingState();

	m_MyShop->m_Products[productId]->m_Quantity += m_PurchaseProducts[productId]->purchaseCount;
	m_CurrentProductQuantityTexts[productId]->setString(std::to_string(m_MyShop->m_Products[productId]->m_Quantity));

	// update shops data
	GameData::getInstance().setShopProductQuantity(m_MyShop->m_ShopId, productId, m_MyShop->m_Products[productId]->m_Quantity);
	// update player current cash amout
	auto totalPurchasePrice = GameData::getInstance().getProductPurchasePrice(m_MyShop->m_ShopId, productId) *
		m_PurchaseProducts[productId]->purchaseCount;
	m_Player->updateCurrentCashAmout(-totalPurchasePrice);
}

void MyShopSettingPanel::openWidget2Callback(cocos2d::Ref* pSender)
{
	m_DisplayWidget2 = !m_DisplayWidget2;
	if (m_DisplayWidget2)
	{
		enableWidget(m_ProductWidget1, false, m_MenuItems, itemTypes::BUTTON);
		enableWidget(m_ProductWidget2, true, m_WidgetMenu, itemTypes::WIDGET_BUTTON);
		return;
	}
	enableWidget(m_ProductWidget2, false, m_WidgetMenu, itemTypes::WIDGET_BUTTON);
	enableWidget(m_ProductWidget1, true, m_MenuItems, itemTypes::BUTTON);
}

void MyShopSettingPanel::onQuantitytChanges(Shop* shop, unsigned productId, unsigned remainQuantity)
{
	updateShopWorkingState();

	m_CurrentProductQuantityTexts[productId]->setString(std::to_string(remainQuantity));
}

void MyShopSettingPanel::onCountDownChanges(Shop* shop, unsigned countdown)
{
	m_ReplenishCountdownText->setString(std::to_string(countdown));

	if (countdown == 0)
	{
		m_WorkStateText->setString(m_WorkStates[0]);
		m_ReplenishCountdownText->setVisible(false);
	}
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

void MyShopSettingPanel::updateShopProductData()
{
	auto shop = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]];
	for (unsigned index = 0; index < m_CurrentProductQuantityTexts.size(); index++)
	{
		m_CurrentProductQuantityTexts[index]->setString(std::to_string(shop->m_Products[index]->m_Quantity));
	}

	updateShopWorkingState();
}

void MyShopSettingPanel::updateShopWorkingState()
{
	if (m_MyShop->m_PlayerWorkHere)
	{
		m_WorkStateText->setString(getWorkState());

		(m_MyShop->isReplenishing() ? m_ReplenishCountdownText->setVisible(true) : m_ReplenishCountdownText->setVisible(false));

		if (!m_WorkStatesWidget->isVisible())
			m_WorkStatesWidget->setVisible(true);
	}
}

std::string MyShopSettingPanel::getWorkState()
{
	// 0 = open, 1 = close, 2 = coming soon
	if (!m_MyShop->isShopOpen(m_GameTime->day, m_GameTime->hour))
	{
		if (m_MyShop->isReplenishing())
			return m_WorkStates[2];
		return m_WorkStates[1];
	}

	return m_WorkStates[0];
}
