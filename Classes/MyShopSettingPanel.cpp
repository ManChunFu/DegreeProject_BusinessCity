#include "MyShopSettingPanel.h"
#include "GameScene.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "Player.h"
#include "Shop.h"
#include "ShopProduct.h"
#include "MouseOverMenuItem.h"
#include "ui/CocosGUI.h"
#include "ProductData.h"
#include "ui/UIWidget.h"
#include "GlobalTime.h"
#include "GameTime.h"
#include "ShopAdmin.h"
#include "ShopProductAdmin.h"
#include "ShopUpgrade.h"

USING_NS_CC;

MyShopSettingPanel::~MyShopSettingPanel()
{
	m_GameTime = nullptr;
	m_PanelTabs.clear();

	m_ProductWidget1 = nullptr;
	m_ProductWidget2 = nullptr;
	m_WorkStatesWidget = nullptr;
	m_ShopStateText = nullptr;
	m_PlayerWorkHereText = nullptr;
	m_ReplenishCountdownText = nullptr;
	m_EmployeeCountText = nullptr;
	m_EmployeeSalaryText = nullptr;
	m_FromHourText = nullptr;
	m_ToHourText = nullptr;
	m_MyShop = nullptr;

	m_ProductPriceTexts.clear();
	m_ProductQTYTexts.clear();
	m_WidgetMenu.clear();

}

void MyShopSettingPanel::openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint, unsigned shopId)
{
	m_IsPanelOpen = true;
	GameData::getInstance().setTempOpenPanel(this);

	if (!m_ThisPanel)
	{
		m_GameScene = scene;
		m_GameTime = GameData::getInstance().m_GlobalTime->m_Gametime;
		m_Player = GameData::getInstance().m_Player;

		if (m_GameScene && m_Player)
			createPanel(sceneMidPoint, shopId);

		return;
	}

	updateShopProductData();
	m_ThisPanel->setVisible(true);
	//(m_DisplayWidget2) ? enableMenuItems(m_WidgetMenu, true) : enableMenuItems(m_MenuItems, true);
}

void MyShopSettingPanel::closePanel()
{
	m_ThisPanel->setVisible(false);

	//(m_DisplayWidget2) ? enableMenuItems(m_WidgetMenu, false) : enableMenuItems(m_MenuItems, false);

	m_IsPanelOpen = false;

	GameData::getInstance().m_TempOpenPanel = nullptr;
}

void MyShopSettingPanel::createPanel(cocos2d::Vec2 sceneMidPoint, unsigned shopId)
{
	GameData::getInstance().m_GlobalTime->addHourEventListener(CC_CALLBACK_2(MyShopSettingPanel::onEveryHourChanges, this));
	m_MyShop = GameData::getInstance().m_Shops[shopId];
	m_MyShop->onQuantityChanges = CC_CALLBACK_2(MyShopSettingPanel::onQuantitytChanges, this);
	m_MyShop->onCountdownChanges = CC_CALLBACK_1(MyShopSettingPanel::onCountDownChanges, this);

	// create this panel
	m_ThisPanel = Sprite::createWithSpriteFrameName("Brown_Panel_600x500.png");
	if (!m_ThisPanel)
		return;

	m_ThisPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(m_ThisPanel, 1);
	m_Elements.pushBack(m_ThisPanel);

	auto panelMidPoint = Vec2(m_ThisPanel->getContentSize() * 0.5f);

	// create close button
	auto closePanelButton = MouseOverMenuItem::creatMouseOverMenuButton("CloseButton_Normal.png", "CloseButton_Lit.png", "CloseButton_Disable.png",
		CC_CALLBACK_1(MyShopSettingPanel::closeCallback, this));
	if (closePanelButton)
		displayMenuButton(closePanelButton, CC_CALLBACK_2(MyShopSettingPanel::onMouseOver, this),
			Vec2(panelMidPoint.x + 275.f, panelMidPoint.y + 222.f), itemTypes::DEFAULT, 0.7f, true);

	auto closeMenu = Menu::create(closePanelButton, NULL);
	closeMenu->setPosition(Vec2::ZERO);
	m_ThisPanel->addChild(closeMenu, 1);

	// shop picture, name
	auto shopPic = Sprite::createWithSpriteFrameName(m_MyShop->m_ShopLook_Normal);
	if (shopPic)
	{
		GameFunctions::displaySprite(shopPic, Vec2(panelMidPoint.x + 180.f, panelMidPoint.y + 170.f), m_ThisPanel, 1, 0.5f, 0.5f);

		auto shopType = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (shopType)
		{
			shopType->setString(GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_ShopType);
			GameFunctions::displayLabel(shopType, Color4B::WHITE, Vec2(shopPic->getContentSize().width * 0.5f, -10.f), shopPic, 1);
		}
	}

	auto textAligmentLeft = panelMidPoint.x - 270.f;
	auto shopName = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
	if (shopName)
	{
		shopName->setString(m_MyShop->m_Name);
		shopName->enableShadow(Color4B::BLACK);
		GameFunctions::displayLabel(shopName, Color4B::WHITE, Vec2(textAligmentLeft, panelMidPoint.y + 180.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	}

#pragma region create tabs and widgets
	auto widgetBackgroundSprite = Sprite::createWithSpriteFrameName("PanelBG_SquareLineBlue.png");
	if (widgetBackgroundSprite)
		GameFunctions::displaySprite(widgetBackgroundSprite, Vec2(panelMidPoint.x, panelMidPoint.y - 50.f), m_ThisPanel, 1);

	auto tabPos = (Vec2(panelMidPoint.x - 216.f, panelMidPoint.y + 118.f));
	std::array<std::string, 3> tabTexts = { "OVERVIEW", "ADMINISTRATION", "UPGRADE" };
	for (unsigned index = 0; index < 3; index++)
	{
		auto tabButton = ui::CheckBox::create("Sprites/Checkboxes/Button_BlueFull.png", "Sprites/Checkboxes/Button_SelectedFull.png");
		if (tabButton)
		{
			tabButton->setPosition(tabPos);
			tabButton->addClickEventListener(CC_CALLBACK_1(MyShopSettingPanel::onOpenTabCallback, this, index));
			m_ThisPanel->addChild(tabButton, 1);

			auto tabText = Label::createWithTTF(tabTexts[index], "fonts/NirmalaB.ttf", 10);
			if (tabText)
				GameFunctions::displayLabel(tabText, Color4B::WHITE, tabButton->getContentSize() * 0.5f, tabButton, 1);

			auto tabWidget = ui::Widget::create();
			tabWidget->setPosition(Vec2::ZERO);
			m_ThisPanel->addChild(tabWidget, 1);
			m_PanelTabs.push_back(std::make_pair(tabButton, tabWidget));

			tabPos.x += 105.f;
		}
	}

	m_PanelTabs.at(0).first->setSelected(true);
#pragma endregion

	auto darkCyanColor = GameData::getInstance().m_ColorType.DarkCyan;
	auto contentTextPos = Vec2(panelMidPoint.x - 240.f, panelMidPoint.y + 60.f);

#pragma region Create overview content
	std::array<std::string, 3> overviewLefTitles = { "Shop State", "Employees", "Products" };
	for (unsigned index = 0; index < 3; index++)
	{
		auto title = Label::createWithTTF(overviewLefTitles[index], "fonts/NirmalaB.ttf", 16);
		title->enableGlow(Color4B::BLACK);
		GameFunctions::displayLabel(title, Color4B::WHITE, contentTextPos, m_PanelTabs.at(0).second, 1,
			true, TextHAlignment::LEFT);

		contentTextPos.y -= 30.f;
	}
	// reset pos
	contentTextPos.y = panelMidPoint.y + 60.f;

	// shop state
	m_ShopStateText = Label::createWithTTF("", "fonts/Nirmala.ttf", 16);
	if (m_ShopStateText)
	{
		m_ShopStateText->setString(getWorkState());
		m_ShopStateText->enableShadow(darkCyanColor);
		GameFunctions::displayLabel(m_ShopStateText, Color4B::WHITE, Vec2(panelMidPoint.x - 150.f, contentTextPos.y),
			m_PanelTabs.at(0).second, 1, true, TextHAlignment::LEFT);
	}

#pragma region work states
	m_WorkStatesWidget = ui::Widget::create();
	m_WorkStatesWidget->setPosition(Vec2::ZERO);
	m_PanelTabs.at(0).second->addChild(m_WorkStatesWidget, 1);

	m_ReplenishCountdownText = Label::createWithTTF("", "fonts/NirmalaB.ttf", 16);
	if (m_ReplenishCountdownText)
	{
		if (m_MyShop->isReplenishing())
			m_ReplenishCountdownText->setString(std::to_string(m_MyShop->getCountDown()));

		m_ReplenishCountdownText->enableGlow(Color4B::BLACK);
		GameFunctions::displayLabel(m_ReplenishCountdownText, Color4B::RED, Vec2(panelMidPoint.x - 70.f, contentTextPos.y +20.f),
			m_WorkStatesWidget, 1, true, TextHAlignment::RIGHT);
	}
#pragma endregion

	// employee count
	m_EmployeeCountText = Label::createWithTTF("", "fonts/Nirmala.ttf", 16);
	if (m_EmployeeCountText)
	{
		m_EmployeeCountText->setString(std::to_string(m_MyShop->getEmployeeCount()));
		m_EmployeeCountText->enableShadow(darkCyanColor);
		GameFunctions::displayLabel(m_EmployeeCountText, Color4B::WHITE, Vec2(panelMidPoint.x - 130.f, contentTextPos.y - 10.f),
			m_PanelTabs.at(0).second, 1, true, TextHAlignment::RIGHT);
	}

	// set pos
	contentTextPos.x = panelMidPoint.x - 30.f;

	std::array<std::string, 2> overviewRightTitles = { "Today's Open Hours", " I Work Here" };
	for (unsigned index = 0; index < 2; index++)
	{
		auto title = Label::createWithTTF(overviewRightTitles[index], "fonts/NirmalaB.ttf", 16);
		title->enableGlow(Color4B::BLACK);
		GameFunctions::displayLabel(title, Color4B::WHITE, contentTextPos, m_PanelTabs.at(0).second, 1,
			true, TextHAlignment::LEFT);

		contentTextPos.y -= 30.f;
	}

	// reset pos
	contentTextPos.y = panelMidPoint.y + 60.f;

	// from
	m_FromHourText = Label::createWithTTF("", "fonts/Nirmala.ttf", 16);
	if (m_FromHourText)
	{
		GameFunctions::updatLabelText_TimeFormat(m_FromHourText, m_MyShop->m_ShopOpenHour[0].first, true);
		m_FromHourText->enableShadow(darkCyanColor);
		GameFunctions::displayLabel(m_FromHourText, Color4B::WHITE, Vec2(panelMidPoint.x + 130.f, contentTextPos.y),
			m_PanelTabs.at(0).second, 1, true, TextHAlignment::LEFT);
	}

	//-
	auto dashText = Label::createWithTTF("-", "fonts/Nirmala.ttf", 16);
	if (dashText)
	{
		dashText->enableShadow(darkCyanColor);
		GameFunctions::displayLabel(dashText, Color4B::WHITE, Vec2(panelMidPoint.x + 160.f, contentTextPos.y),
			m_PanelTabs.at(0).second, 1, true, TextHAlignment::LEFT);
	}

	// to
	m_ToHourText = Label::createWithTTF("", "fonts/Nirmala.ttf", 16);
	if (m_ToHourText)
	{
		GameFunctions::updatLabelText_TimeFormat(m_ToHourText, m_MyShop->m_ShopOpenHour[0].second, true);
		m_ToHourText->enableShadow(darkCyanColor);
		GameFunctions::displayLabel(m_ToHourText, Color4B::WHITE, Vec2(panelMidPoint.x + 180.f, contentTextPos.y),
			m_PanelTabs.at(0).second, 1, true, TextHAlignment::LEFT);
	}

	// playerWorkHere
	m_PlayerWorkHereText = Label::createWithTTF("", "fonts/Nirmala.ttf", 16);
	if (m_PlayerWorkHereText)
	{
		m_PlayerWorkHereText->setString(m_PlayerWorkHereArray[m_MyShop->m_PlayerWorkHere]);
		m_PlayerWorkHereText->enableShadow(darkCyanColor);
		GameFunctions::displayLabel(m_PlayerWorkHereText, Color4B::WHITE, Vec2(panelMidPoint.x + 130.f, contentTextPos.y - 30.f),
			m_PanelTabs.at(0).second, 1, true, TextHAlignment::LEFT);
	}

#pragma region Shop Products
	m_ProductWidget1 = ui::Widget::create();
	m_ProductWidget1->setPosition(Vec2::ZERO);
	m_PanelTabs.at(0).second->addChild(m_ProductWidget1, 1);

	auto dash = Label::createWithTTF("--------------------------------------------------------", "fonts/Nirmala.ttf", 20);
	if (dash)
	{
		dash->enableGlow(darkCyanColor);
		GameFunctions::displayLabel(dash, Color4B::WHITE, Vec2(panelMidPoint.x - 240.f, panelMidPoint.y - 15.f),
			m_PanelTabs.at(0).second, 1, true, TextHAlignment::LEFT);
	}

	auto titlePos = Vec2(panelMidPoint.x - 110.f, panelMidPoint.y - 20.f);
	std::array<std::string, 3> productContentTitles = { "Name", "Sale Price", "Quantity" };
	for (unsigned index = 0; index < 3; index++)
	{
		auto title = Label::createWithTTF(productContentTitles[index], "fonts/NirmalaB.ttf", 14);
		if (title)
		{
			title->enableGlow(Color4B::BLACK);
			GameFunctions::displayLabel(title, Color4B::WHITE, titlePos, m_PanelTabs.at(0).second, 1);
		}

		titlePos.x += index == 0 ? 180.f : 80.f;
	}

	auto productLength = m_MyShop->getProductsSize();
	auto productSpritePos = Vec2(panelMidPoint.x - 180.f, panelMidPoint.y - 50.f);

	for (unsigned productIndex = 0; productIndex < productLength; productIndex++)
	{
		if (productIndex > m_PanelLimit)
		{
			auto moreButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonMore50_PointRight.png", "UIButtonMore50_PointRight_Lit.png",
				"UIButtonMore50_PointRight_Disable.png", CC_CALLBACK_1(MyShopSettingPanel::openWidget2Callback, this));
			if (moreButton)
			{
				moreButton->itemSelectedData.type = itemTypes::BUTTON;
				m_MenuItems.pushBack(displayMenuButton(moreButton, CC_CALLBACK_2(MyShopSettingPanel::onMouseOver, this),
					Vec2(productSpritePos.x + 380.f, productSpritePos.y +100.f), itemTypes::BUTTON, 0.5f, true));
				createProductWidget2(panelMidPoint);
				//enableWidget(m_ProductWidget2, false, m_WidgetMenu, itemTypes::WIDGET_BUTTON);
			}
			break;
		}

#pragma region create product pics, name, quantity and buy button
		// product pic
		auto productSprite = Sprite::createWithSpriteFrameName(m_MyShop->getProductSprite(productIndex));
		if (productSprite)
			GameFunctions::displaySprite(productSprite, productSpritePos, m_ProductWidget1, 1, 0.5f, 0.5f);

		// product name
		auto productName = Label::createWithTTF(m_MyShop->getProductName(productIndex), "fonts/Nirmala.ttf", 15);
		if (productName)
		{
			productName->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(productName, Color4B::WHITE, Vec2(productSpritePos.x + 50.f, productSpritePos.y - 10.f),
				m_ProductWidget1, 1, true, TextHAlignment::LEFT);
		}

		// product sale price text
		auto productSalePrice = Label::createWithTTF("", "fonts/Nirmala.ttf", 15);
		if (productSalePrice)
		{
			productSalePrice->enableShadow(darkCyanColor);
			productSalePrice->setString(std::to_string(m_MyShop->getProductSalePrice(productIndex)));
			GameFunctions::displayLabel(productSalePrice, Color4B::WHITE, Vec2(productSpritePos.x + 250.f, productSpritePos.y),
				m_ProductWidget1, 1);

			m_ProductPriceTexts.pushBack(productSalePrice);
		}

		// product quantity text in shop
		auto currentProductCountText = Label::createWithTTF(std::to_string(m_MyShop->getProductQuantity(productIndex)),
			"fonts/Nirmala.ttf", 15);
		if (currentProductCountText)
		{
			currentProductCountText->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(currentProductCountText, Color4B::WHITE, Vec2(productSpritePos.x + 330.f, productSpritePos.y),
				m_ProductWidget1, 1);

			m_ProductQTYTexts.pushBack(currentProductCountText);
		}
		productSpritePos.y -= 40.f;
	}
#pragma endregion

#pragma endregion Shop Products

#pragma endregion Create overiew content

#pragma region Administration
	auto shopAdmin = new ShopAdmin();
	shopAdmin->createAdmin(m_MyShop, m_PanelTabs.at(1).second, panelMidPoint);
	shopAdmin->onWorkDayChanges = CC_CALLBACK_1(MyShopSettingPanel::onWorkDayChanges, this);
	shopAdmin->onWorkHourChanges = CC_CALLBACK_1(MyShopSettingPanel::onWorkHourChanges, this);
	shopAdmin->m_ShopProductAdmin->onProductPriceChanges = CC_CALLBACK_2(MyShopSettingPanel::onProductPriceChanges, this);
	shopAdmin->m_ShopProductAdmin->onProductAmoutChanges = CC_CALLBACK_2(MyShopSettingPanel::onProductAmoutChanges, this);
#pragma endregion

#pragma region Upgrade
	if (!m_MyShop->m_UpgradeableTo.empty())
	{
		auto shopUpgrade = new ShopUpgrade();
		shopUpgrade->createShopUpgrade(m_MyShop, m_PanelTabs.at(2).second, panelMidPoint);
	}
#pragma endregion

	//#pragma region Hire Employee button
	//	auto employeeText = Label::createWithTTF("Employees", "fonts/NirmalaB.ttf", 20);
	//	if (employeeText)
	//		GameFunctions::displayLabel(employeeText, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y + 140.f),
	//			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	//
	//	auto atStoreText = Label::createWithTTF("At Store", "fonts/NirmalaB.ttf", 15);
	//	if (atStoreText)
	//		GameFunctions::displayLabel(atStoreText, Color4B::BLACK, Vec2(panelMidPoint.x - 200.f, panelMidPoint.y + 170.f),
	//			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	//
	//	m_EmployeeAtStoreText = Label::createWithTTF(std::to_string(m_MyShop->getEmployeeCount()), "fonts/NirmalaB.ttf", 20);
	//	if (m_EmployeeAtStoreText)
	//	{
	//		m_EmployeeAtStoreText->enableShadow(Color4B::BLACK);
	//		GameFunctions::displayLabel(m_EmployeeAtStoreText, Color4B::WHITE, Vec2(panelMidPoint.x - 170.f, panelMidPoint.y + 165.f),
	//			m_ThisPanel, 1, true, TextHAlignment::RIGHT);
	//	}
	//
	//	auto boxSprite = Sprite::createWithSpriteFrameName("Border_Black_Square.png");
	//	if (boxSprite)
	//	{
	//		GameFunctions::displaySprite(boxSprite, Vec2(panelMidPoint.x - 120.f, panelMidPoint.y + 150.f), m_ThisPanel, 1);
	//
	//		m_EmployeeCountText = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
	//		if (m_EmployeeCountText)
	//		{
	//			m_EmployeeCountText->enableShadow(Color4B::BLACK);
	//			m_EmployeeCountText->setString(std::to_string(m_MyShop->m_Employees));
	//			GameFunctions::displayLabel(m_EmployeeCountText, Color4B::WHITE, Vec2(boxSprite->getContentSize().width * 0.5f,
	//				boxSprite->getContentSize().height * 0.5f), boxSprite, 1);
	//		}
	//	}
	//
	//	for (unsigned index = 0; index < 2; index++)
	//	{
	//		auto employeeButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(MyShopSettingPanel::reduceCallback, this))
	//			: MouseOverMenuItem::createUpperButton(CC_CALLBACK_1(MyShopSettingPanel::addCallback, this));
	//		if (employeeButton)
	//		{
	//			displayButtons(employeeButton, (index % 2 == 0) ? Vec2(sceneMidPoint.x - 105.f, sceneMidPoint.y + 140.f) :
	//				Vec2(sceneMidPoint.x - 135.f, sceneMidPoint.y + 160.f), itemTypes::BUTTON);
	//		}
	//	}
	//
	//	auto hireButton = MouseOverMenuItem::creatMouseOverMenuButton("ButtonBlueNormal.png", "ButtonBlueLit.png", "ButtonBlueDisabled.png",
	//		CC_CALLBACK_1(MyShopSettingPanel::hireCallback, this));
	//	if (hireButton)
	//	{
	//		displayButtons(hireButton, Vec2(sceneMidPoint.x - 50.f, sceneMidPoint.y + 150.f), itemTypes::DEFAULT, 0.7f);
	//
	//		auto hireText = Label::createWithTTF("HIRE", "fonts/NirmalaB.ttf", 20);
	//		if (hireText)
	//			GameFunctions::displayLabel(hireText, GameData::getInstance().m_ColorType.Taro, hireButton->getContentSize()* 0.5f, hireButton, 1);
	//
	//		auto cashSymbol = Label::createWithTTF("$", "fonts/Nirmala.ttf", 15);
	//		if (cashSymbol)
	//			GameFunctions::displayLabel(cashSymbol, Color4B::BLACK, Vec2(panelMidPoint.x, panelMidPoint.y + 140.f),
	//				m_ThisPanel, 1, true, TextHAlignment::LEFT);
	//
	//		m_EmployeeSalaryText = Label::createWithTTF(std::to_string(m_MyShop->m_SalaryPerEmployee), "fonts/Nirmala.ttf", 15);
	//		if (m_EmployeeSalaryText)
	//			GameFunctions::displayLabel(m_EmployeeSalaryText, Color4B::BLACK, Vec2(panelMidPoint.x + 15.f, panelMidPoint.y + 140.f),
	//				m_ThisPanel, 1, true, TextHAlignment::LEFT);
	//
	//		auto personText = Label::createWithTTF("/ Person", "fonts/Nirmala.ttf", 15);
	//		if (personText)
	//			GameFunctions::displayLabel(personText, Color4B::BLACK, Vec2(panelMidPoint.x + 45.f, panelMidPoint.y + 140.f),
	//				m_ThisPanel, 1, true, TextHAlignment::LEFT);
	//
	//	}
	//#pragma endregion
	//	
	//#pragma region Work here checkbox
	//	auto workHereText = Label::createWithTTF("I Work Here", "fonts/NirmalaB.ttf", 20);
	//	if (workHereText)
	//		GameFunctions::displayLabel(workHereText, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y + 100.f),
	//			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	//
	//	auto workHereCheckbox = ui::CheckBox::create("X/Checkbox_Normal.png", "X/Checkbox_Checked.png");
	//	workHereCheckbox->setPosition(Vec2(panelMidPoint.x - 170.f, panelMidPoint.y + 110.f));
	//	workHereCheckbox->addClickEventListener(CC_CALLBACK_1(MyShopSettingPanel::workHereCallback, this));
	//	workHereCheckbox->setSelected(m_MyShop->m_PlayerWorkHere);
	//
	//	m_ThisPanel->addChild(workHereCheckbox, 1);
	//#pragma endregion
	//


	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	m_ProductWidget1->addChild(menu, 1);
}

void MyShopSettingPanel::onOpenTabCallback(cocos2d::Ref* pSender, unsigned tabIndex)
{
	for (unsigned index = 0; index < m_PanelTabs.size(); index++)
	{
		if (index == tabIndex)
		{
			if (m_PanelTabs.at(index).second->isVisible())
			{
				m_PanelTabs.at(index).second->setVisible(false);
				return;
			}

			m_PanelTabs.at(index).second->setVisible(true);
			continue;
		}
		m_PanelTabs.at(index).second->setVisible(false);
		m_PanelTabs.at(index).first->setSelected(false);
	}
}

void MyShopSettingPanel::createProductWidget2(Vec2 panelMidPoint)
{
	m_ProductWidget2 = ui::Widget::create();
	if (m_ProductWidget2)
	{
		m_ProductWidget2->setPosition(Vec2::ZERO);
		m_PanelTabs.at(0).second->addChild(m_ProductWidget2, 1);
	}
	m_ProductWidget2->setVisible(false);

	auto productLength = m_MyShop->getProductsSize();
	auto productSpritePos = Vec2(panelMidPoint.x - 180.f, panelMidPoint.y - 50.f);
	auto darkCyanColor = GameData::getInstance().m_ColorType.DarkCyan;

	auto lessButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonLess50_PointLeft.png", "UIButtonLess50_PointLeft_Lit.png",
		"UIButtonLess50_PointLeft_Disable.png", CC_CALLBACK_1(MyShopSettingPanel::openWidget2Callback, this));
	if (lessButton)
	{
		lessButton->itemSelectedData.type = itemTypes::WIDGET_BUTTON;
		m_WidgetMenu.pushBack(displayMenuButton(lessButton, CC_CALLBACK_2(MyShopSettingPanel::onMouseOver, this),
			Vec2(productSpritePos.x - 50.f, productSpritePos.y - 60.f), itemTypes::WIDGET_BUTTON, 0.5f, true));
	}
#pragma region create product pics, name, quantity and buy button
	for (unsigned productIndex = m_PanelLimit + 1; productIndex < productLength; productIndex++)
	{
		// product pic
		auto productSprite = Sprite::createWithSpriteFrameName(m_MyShop->getProductSprite(productIndex));
		if (productSprite)
			GameFunctions::displaySprite(productSprite, productSpritePos, m_ProductWidget2, 1, 0.5f, 0.5f);

		// product name
		auto productName = Label::createWithTTF(m_MyShop->getProductName(productIndex), "fonts/Nirmala.ttf", 15);
		if (productName)
		{
			productName->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(productName, Color4B::WHITE, Vec2(productSpritePos.x + 50.f, productSpritePos.y - 10.f),
				m_ProductWidget2, 1, true, TextHAlignment::LEFT);
		}

		// product sale price text
		auto productSalePrice = Label::createWithTTF("", "fonts/Nirmala.ttf", 15);
		if (productSalePrice)
		{
			productSalePrice->enableShadow(darkCyanColor);
			productSalePrice->setString(std::to_string(m_MyShop->getProductSalePrice(productIndex)));
			GameFunctions::displayLabel(productSalePrice, Color4B::WHITE, Vec2(productSpritePos.x + 250.f, productSpritePos.y),
				m_ProductWidget2, 1);

			m_ProductPriceTexts.pushBack(productSalePrice);
		}

		// product quantity text in shop
		auto currentProductCountText = Label::createWithTTF(std::to_string(m_MyShop->getProductQuantity(productIndex)),
			"fonts/Nirmala.ttf", 15);
		if (currentProductCountText)
		{
			currentProductCountText->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(currentProductCountText, Color4B::WHITE, Vec2(productSpritePos.x + 330.f, productSpritePos.y),
				m_ProductWidget2, 1);

			m_ProductQTYTexts.pushBack(currentProductCountText);
		}

#pragma endregion
		productSpritePos.y -= 40.f;
	}

	auto widgetMenu = Menu::createWithArray(m_WidgetMenu);
	widgetMenu->setPosition(Vec2::ZERO);
	m_ProductWidget2->addChild(widgetMenu, 1);
}

void MyShopSettingPanel::closeCallback(cocos2d::Ref* pSender)
{
	if (GameData::getInstance().isPopupOpen())
		return;

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
	//update shop data
	m_MyShop->addEmployee(m_EmployeeCount);
	m_EmployeeAtStoreText->setString(std::to_string(m_MyShop->getEmployeeCount()));
	m_EmployeeCountText->setString(std::to_string(m_EmployeeCount = 0));
}

void MyShopSettingPanel::workHereCallback(cocos2d::Ref* pSender)
{
	m_MyShop->setPlayerWorkHere();
}

void MyShopSettingPanel::openWidget2Callback(cocos2d::Ref* pSender)
{
	if (m_ProductWidget1->isVisible())
	{
		m_ProductWidget1->setVisible(false);
		m_ProductWidget2->setVisible(true);
		return;
	}

	m_ProductWidget1->setVisible(true);
	m_ProductWidget2->setVisible(false);
}

void MyShopSettingPanel::onQuantitytChanges(unsigned productId, unsigned remainQuantity)
{
	m_ProductQTYTexts.at(productId)->setString(std::to_string(remainQuantity));
}

void MyShopSettingPanel::onCountDownChanges(unsigned countdown)
{
	m_ReplenishCountdownText->setString(std::to_string(countdown));

	if (countdown == 0)
		updateShopWorkingState();
}

void MyShopSettingPanel::onEveryHourChanges(GlobalTime* globalTime, unsigned hour)
{
	updateShopWorkingState();
}

void MyShopSettingPanel::onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
}

void MyShopSettingPanel::onWorkDayChanges(unsigned weekday)
{
	updateShopWorkingState();
}

void MyShopSettingPanel::onWorkHourChanges(unsigned workhour)
{
	m_FromHourText->setString(std::to_string(m_MyShop->m_ShopOpenHour[m_GameTime->day].first));
	m_ToHourText->setString(std::to_string(m_MyShop->m_ShopOpenHour[m_GameTime->day].second));
	updateShopWorkingState();
}

void MyShopSettingPanel::onProductPriceChanges(unsigned productId, unsigned price)
{
	m_ProductPriceTexts.at(productId)->setString(std::to_string(price));
}

void MyShopSettingPanel::onProductAmoutChanges(unsigned productId, unsigned amout)
{
	m_ProductQTYTexts.at(productId)->setString(std::to_string(amout));
}

void MyShopSettingPanel::updateShopProductData()
{
	auto shop = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]];
	auto listSize = m_ProductPriceTexts.size();
	for (unsigned index = 0; index < listSize; index++)
	{
		m_ProductPriceTexts.at(index)->setString(std::to_string(shop->getCurrentSalePrice(index)));
		m_ProductQTYTexts.at(index)->setString(std::to_string(shop->getProductQuantity(index)));
	}

	m_EmployeeCountText->setString(std::to_string(shop->getEmployeeCount()));

	updateShopWorkingState();
}

void MyShopSettingPanel::updateShopWorkingState()
{
	m_ShopStateText->setString(getWorkState());

	(m_MyShop->isReplenishing() ? m_ReplenishCountdownText->setVisible(true) : m_ReplenishCountdownText->setVisible(false));
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