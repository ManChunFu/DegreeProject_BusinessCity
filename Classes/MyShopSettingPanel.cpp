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
#include "ShopEmployeeAdmin.h"

USING_NS_CC;

MyShopSettingPanel::~MyShopSettingPanel()
{
	GameData::getInstance().m_GlobalTime->removeHourEventListener(this);
	m_GameTime = nullptr;
	m_PanelTabs.clear();

	m_WorkStatesWidget = nullptr;
	m_ShopStateText = nullptr;
	m_PlayerWorkHereText = nullptr;
	m_ReplenishCountdownText = nullptr;
	m_FromHourText = nullptr;
	m_ToHourText = nullptr;
	m_EmployeeCountText = nullptr;
	m_MyShop = nullptr;

	m_ProductPriceTexts.clear();
	m_ProductQTYTexts.clear();
	m_ProductWidgets.clear();
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
}

void MyShopSettingPanel::closePanel()
{
	m_ThisPanel->setVisible(false);
	m_IsPanelOpen = false;

	GameData::getInstance().m_TempOpenPanel = nullptr;
}

void MyShopSettingPanel::createPanel(cocos2d::Vec2 sceneMidPoint, unsigned shopId)
{
	GameData::getInstance().m_GlobalTime->addHourEventListener(CC_CALLBACK_2(MyShopSettingPanel::onEveryHourChanges, this), this);

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
	auto productWidget = ui::Widget::create();
	productWidget->setPosition(Vec2::ZERO);
	m_PanelTabs.at(EPanelTabs::E_Overview).second->addChild(productWidget, 1);
	m_ProductWidgets.push_back(productWidget);

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

	createProductOverview(productWidget, 0, m_PanelLimit, panelMidPoint);


#pragma endregion Create overiew content

#pragma region Administration
	auto shopAdmin = new ShopAdmin();
	shopAdmin->createAdmin(m_MyShop, m_PanelTabs.at(1).second, panelMidPoint);
	shopAdmin->onWorkDayChanges = CC_CALLBACK_1(MyShopSettingPanel::onWorkDayChanges, this);
	shopAdmin->onWorkHourChanges = CC_CALLBACK_1(MyShopSettingPanel::onWorkHourChanges, this);
	shopAdmin->m_ShopProductAdmin->onProductPriceChanges = CC_CALLBACK_2(MyShopSettingPanel::onProductPriceChanges, this);
	shopAdmin->m_ShopProductAdmin->onProductAmoutChanges = CC_CALLBACK_2(MyShopSettingPanel::onProductAmoutChanges, this);
	shopAdmin->m_ShopEmployeeAdmin->onEmployeeCountChanges = CC_CALLBACK_1(MyShopSettingPanel::onEmployeeCountChanges, this);
#pragma endregion

#pragma region Upgrade
	if (!m_MyShop->m_UpgradeableTo.empty())
	{
		auto shopUpgrade = new ShopUpgrade();
		shopUpgrade->createShopUpgrade(m_GameScene, m_MyShop, m_PanelTabs.at(2).second, panelMidPoint);
		shopUpgrade->onUpgradeChanges = CC_CALLBACK_1(MyShopSettingPanel::onUpgradeChanges, this);
	}
#pragma endregion


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

void MyShopSettingPanel::closeCallback(cocos2d::Ref* pSender)
{
	if (GameData::getInstance().isPopupOpen())
		return;

	closePanel();
}

void MyShopSettingPanel::openProductWidgetCallback(cocos2d::Ref* pSender, unsigned widgetIndex)
{
	for (unsigned index = 0; index < m_ProductWidgets.size(); ++index)
	{
		if (index == widgetIndex)
		{
			if (m_ProductWidgets.at(index)->isVisible())
			{
				m_ProductWidgets.at(index)->setVisible(false);
				continue;
			}
			m_ProductWidgets.at(index)->setVisible(true);
			continue;
		}
		m_ProductWidgets.at(index)->setVisible(false);
	}
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

void MyShopSettingPanel::onEmployeeCountChanges(unsigned employeeCount)
{
	m_EmployeeCountText->setString(std::to_string(employeeCount));
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

void MyShopSettingPanel::onUpgradeChanges(unsigned productId)
{
	if (onShopChanges)
		onShopChanges(productId);
}

void MyShopSettingPanel::updateShopProductData()
{
	auto listSize = m_ProductPriceTexts.size();
	for (unsigned index = 0; index < listSize; index++)
	{
		m_ProductPriceTexts.at(index)->setString(std::to_string(m_MyShop->getCurrentSalePrice(index)));
		m_ProductQTYTexts.at(index)->setString(std::to_string(m_MyShop->getProductQuantity(index)));
	}

	m_EmployeeCountText->setString(std::to_string(m_MyShop->getEmployeeCount()));

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

void MyShopSettingPanel::createProductOverview(ui::Widget* productWidget, unsigned startIndex, unsigned panelLimit, Vec2 panelMidPoint)
{
	auto productLength = m_MyShop->getProductsSize();
	auto productSpritePos = Vec2(panelMidPoint.x - 180.f, panelMidPoint.y - 50.f);
	auto darkCyanColor = GameData::getInstance().m_ColorType.DarkCyan;

	if (startIndex > 0)
	{
		auto lessButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonLess50_PointLeft.png", "UIButtonLess50_PointLeft_Lit.png",
			"UIButtonLess50_PointLeft_Disable.png", CC_CALLBACK_1(MyShopSettingPanel::openProductWidgetCallback, this, m_ProductWidgets.size() -2));
		if (lessButton)
		{
			lessButton->itemSelectedData.type = itemTypes::WIDGET_BUTTON;
			displayMenuButton(lessButton, CC_CALLBACK_2(MyShopSettingPanel::onMouseOver, this), Vec2(productSpritePos.x - 50.f, 
				productSpritePos.y - 60.f), itemTypes::WIDGET_BUTTON, 0.5f, true);

			auto lessMenu = Menu::create(lessButton, NULL);
			lessMenu->setPosition(Vec2::ZERO);
			productWidget->addChild(lessMenu, 1);
		}
	}
	
	for (unsigned productIndex = startIndex; productIndex < productLength; productIndex++)
	{
		if (productIndex > panelLimit)
		{
			auto moreButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonMore50_PointRight.png", "UIButtonMore50_PointRight_Lit.png",
				"UIButtonMore50_PointRight_Disable.png", CC_CALLBACK_1(MyShopSettingPanel::openProductWidgetCallback, this, m_ProductWidgets.size()));
			if (moreButton)
			{
				moreButton->itemSelectedData.type = itemTypes::BUTTON;
				displayMenuButton(moreButton, CC_CALLBACK_2(MyShopSettingPanel::onMouseOver, this),
					Vec2(productSpritePos.x + 380.f, productSpritePos.y + 100.f), itemTypes::BUTTON, 0.5f, true);

				auto buttonMenu = Menu::create(moreButton, NULL);
				buttonMenu->setPosition(Vec2::ZERO);
				productWidget->addChild(buttonMenu, 1);

				auto nextWidget = ui::Widget::create();
				nextWidget->setPosition(Vec2::ZERO);
				m_PanelTabs.at(EPanelTabs::E_Overview).second->addChild(nextWidget, 1);
				m_ProductWidgets.push_back(nextWidget);
				createProductOverview(nextWidget, panelLimit + 1, (m_PanelLimit *= 2) + 1 , panelMidPoint);
				nextWidget->setVisible(false);
			}
			break;
		}

		// product pic
		auto productSprite = Sprite::createWithSpriteFrameName(m_MyShop->getProductSprite(productIndex));
		if (productSprite)
			GameFunctions::displaySprite(productSprite, productSpritePos, productWidget, 1, 0.5f, 0.5f);

		// product name
		auto productName = Label::createWithTTF(m_MyShop->getProductName(productIndex), "fonts/Nirmala.ttf", 15);
		if (productName)
		{
			productName->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(productName, Color4B::WHITE, Vec2(productSpritePos.x + 50.f, productSpritePos.y - 10.f),
				productWidget, 1, true, TextHAlignment::LEFT);
		}

		// product sale price text
		auto productSalePrice = Label::createWithTTF("", "fonts/Nirmala.ttf", 15);
		if (productSalePrice)
		{
			productSalePrice->enableShadow(darkCyanColor);
			productSalePrice->setString(std::to_string(m_MyShop->getProductSalePrice(productIndex)));
			GameFunctions::displayLabel(productSalePrice, Color4B::WHITE, Vec2(productSpritePos.x + 250.f, productSpritePos.y),
				productWidget, 1);

			m_ProductPriceTexts.pushBack(productSalePrice);
		}

		// product quantity text in shop
		auto currentProductCountText = Label::createWithTTF(std::to_string(m_MyShop->getProductQuantity(productIndex)),
			"fonts/Nirmala.ttf", 15);
		if (currentProductCountText)
		{
			currentProductCountText->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(currentProductCountText, Color4B::WHITE, Vec2(productSpritePos.x + 330.f, productSpritePos.y),
				productWidget, 1);

			m_ProductQTYTexts.pushBack(currentProductCountText);
		}
		productSpritePos.y -= 40.f;
	}
}