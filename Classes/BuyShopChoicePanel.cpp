#include "BuyShopChoicePanel.h"
#include "SwitchSceneView.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "MouseOverMenuItem.h"
#include "Player.h"
#include "Shop.h"

USING_NS_CC;

BuyShopChoicePanel::~BuyShopChoicePanel()
{
	m_SceneView = nullptr;
}

void BuyShopChoicePanel::openPanel(SwitchSceneView* sceneView, cocos2d::Vec2 sceneMidPoint, unsigned sceneId, unsigned startupId)
{
	m_IsPanelOpen = true;
	GameData::getInstance().setTempOpenPanel(this);

	if (!m_ThisPanel)
	{
		m_SceneView = sceneView;
		m_Player = GameData::getInstance().m_Player;
		createPanel(sceneMidPoint, sceneId, startupId);
		return;
	}

	m_ThisPanel->setVisible(true);

}

void BuyShopChoicePanel::closePanel()
{
	m_ThisPanel->setVisible(false);
	m_IsPanelOpen = false;

	GameData::getInstance().m_TempOpenPanel = nullptr;
	
	m_SceneView->m_Question->setVisible(true);
}

void BuyShopChoicePanel::createPanel(cocos2d::Vec2 sceneMidPoint, unsigned sceneId, unsigned startupId)
{
	m_ThisPanel = Sprite::createWithSpriteFrameName("YellowPanel80.png");
	m_ThisPanel->setPosition(Vec2(sceneMidPoint.x , sceneMidPoint.y + 100.f));
	auto view = m_SceneView->getSceneView(sceneId);
	view->addChild(m_ThisPanel, 2);

	m_SceneIdForPanel = sceneId;

	auto panelMidPoint = Vec2(m_ThisPanel->getContentSize() * 0.5f);
	auto closePanelButton = MouseOverMenuItem::creatMouseOverMenuButton("CloseButton_Normal.png", "CloseButton_Lit.png", "CloseButton_Disable.png",
		CC_CALLBACK_1(BuyShopChoicePanel::closeCallback, this));
	if (closePanelButton)
		m_MenuItems.pushBack(displayMenuButton(closePanelButton, CC_CALLBACK_2(BuyShopChoicePanel::onMouseOver, this),
			Vec2(panelMidPoint.x + 260.f, panelMidPoint.y + 165.f), itemTypes::DEFAULT, 1.f, true, Vec2(250.f, 130.f)));

	auto openShopLabel = Label::createWithTTF("SHOP OPTIONS", "fonts/NirmalaB.ttf", 20);
	if (openShopLabel)
	{
		openShopLabel->enableShadow(Color4B::BLACK);
		GameFunctions::displayLabel(openShopLabel, Color4B::WHITE, Vec2(m_ThisPanel->getContentSize().width * 0.5f,
			m_ThisPanel->getContentSize().height - 25.f), m_ThisPanel, 1);
	}

	auto buyButton = MouseOverMenuItem::creatMouseOverMenuButton(CC_CALLBACK_1(BuyShopChoicePanel::BuyButtonCallback, this));
	if (buyButton)
	{
		m_MenuItems.pushBack(displayMenuButton(buyButton, CC_CALLBACK_2(BuyShopChoicePanel::onMouseOver, this),
			Vec2(panelMidPoint.x + 100.f, panelMidPoint.y -50.f), itemTypes::BUTTON, 1.f, true, Vec2(5.f, 0.f)));
		
		buyButton->setVisible(false);
		buyButton->setEnabled(false);

		auto buyLabel = Label::createWithTTF("BUY", "fonts/NirmalaB.ttf", 16);
		if (buyLabel)
		{
			buyLabel->enableGlow(Color4B::BLACK);
			GameFunctions::displayLabel(buyLabel, Color4B::WHITE, Vec2(buyButton->getContentSize().width * 0.5f,
				buyButton->getContentSize().height * 0.5f), buyButton, 1);
		}
	}


	// create shop choices
	std::vector<Shop*> tempShops;
	for (unsigned index = 0; index < 4; ++index)
	{
		if (!GameData::getInstance().m_Shops.at(index)->m_Startup)
			continue;

		if (GameData::getInstance().m_Shops.at(index)->m_ShopId == startupId)
			continue;


		tempShops.push_back(GameData::getInstance().m_Shops.at(index));
	}

	auto shopPos = Vec2(panelMidPoint.x - 130.f, panelMidPoint.y + 80.f);
	for (unsigned index = 0; index < tempShops.size(); ++index)
	{
		if (!tempShops[index]->m_Startup)
			continue;

		if (tempShops[index]->m_ShopId == startupId)
			continue;

		auto buttonItem = MouseOverMenuItem::creatMouseOverMenuButton(tempShops[index]->m_ShopLook_Normal, tempShops[index]->m_ShopLook_Lit,
			tempShops[index]->m_ShopLook_Disabled, CC_CALLBACK_1(BuyShopChoicePanel::selectedItemCallback, this, tempShops[index]->m_ShopId));

		if (!buttonItem)
			continue;

		m_MenuItems.pushBack(displayMenuButton(buttonItem, CC_CALLBACK_2(BuyShopChoicePanel::onMouseOver, this), shopPos,
			itemTypes::DEFAULT, 0.7f, true, Vec2(340.f, 150.f)));
		auto buttonMidPoint = Vec2(buttonItem->getContentSize().width * 0.5f, buttonItem->getContentSize().height * 0.5f);

		auto buttonLabel = Label::createWithTTF(tempShops[index]->m_ShopType, "fonts/NirmalaB.ttf", 20);
		if (buttonLabel)
		{
			buttonLabel->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(buttonLabel, Color4B::WHITE, Vec2(buttonMidPoint.x - 130.f, buttonMidPoint.y + 60.f),
				buttonItem, 1, true, TextHAlignment::LEFT);
		}

		auto cashSymbol = Label::createWithTTF("$", "fonts/NirmalaB.ttf", 20);
		if (cashSymbol)
		{
			cashSymbol->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(cashSymbol, Color4B::WHITE, Vec2(buttonMidPoint.x - 130.f, buttonMidPoint.y + 40.f),
				buttonItem, 1, true, TextHAlignment::LEFT);
		}

		auto buttonPriceLabel = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
		if (buttonPriceLabel)
		{
			buttonPriceLabel->enableShadow(Color4B::BLACK);
			GameFunctions::updateLabelText_MoneyFormat(buttonPriceLabel, tempShops[index]->m_ShopPrice);
			GameFunctions::displayLabel(buttonPriceLabel, Color4B::WHITE, Vec2(buttonMidPoint.x - 100.f, buttonMidPoint.y + 40.f),
				buttonItem, 1, true, TextHAlignment::LEFT);
		}

		if (index == 0)
			shopPos.x += 265.f;
		if (index == 1)
		{
			shopPos.x = panelMidPoint.x - 130.f;
			shopPos.y -= 170.f;
		}
	}


	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	m_ThisPanel->addChild(menu, 1);
}

void BuyShopChoicePanel::closeCallback(cocos2d::Ref* pSender)
{
	closePanel();
}

void BuyShopChoicePanel::selectedItemCallback(cocos2d::Ref* pSender, unsigned shopId)
{
	m_SelectedShopId = shopId;

	for (auto item : m_MenuItems)
	{
		if (item != pSender)
		{
			if (item->itemSelectedData.type == itemTypes::BUTTON)
			{
				item->setVisible(true);
				item->setEnabled(true);
				continue;
			}

			item->setEnabled(false);
			continue;
		}

		if (item->itemSelectedData.isSelected)
		{
			item->itemSelectedData.isSelected = false;
			item->itemSelectedData.selectedLabel->setVisible(false);
			item->unselected();
			for (auto item : m_MenuItems)
			{
				if (item->itemSelectedData.type == itemTypes::BUTTON)
				{
					item->setVisible(false);
					item->setEnabled(false);
				}
				item->setEnabled(true);
			}
			return;
		}

		item->itemSelectedData.isSelected = true;

		if (item->itemSelectedData.selectedLabel)
		{
			item->itemSelectedData.selectedLabel->setVisible(true);
			continue;
		}

		// create label for first time
		item->itemSelectedData.selectedLabel = Label::createWithTTF("SELECTED", "fonts/NirmalaB.ttf", 40.f);
		item->itemSelectedData.selectedLabel->enableShadow(Color4B::BLACK);
		item->itemSelectedData.selectedLabel->enableGlow(GameData::getInstance().m_ColorType.LightSteelBlue);
		item->itemSelectedData.selectedLabel->setRotation(-15.f);
		GameFunctions::displayLabel(item->itemSelectedData.selectedLabel, GameData::getInstance().m_ColorType.HotPink,
			Vec2(item->getContentSize().width * 0.5f, item->getContentSize().height * 0.5f - 60.f), item, 1),

			item->selected();
	}
}

void BuyShopChoicePanel::BuyButtonCallback(cocos2d::Ref* pSender)
{
	m_Player->m_MyShopIds.push_back(m_SelectedShopId);
	m_Player->updateCurrentCashAmout(-GameData::getInstance().m_Shops[m_SelectedShopId]->m_ShopPrice);

	closePanel();

	if (m_OnPlayerPurchase && m_SceneIdForPanel != 0)
		m_OnPlayerPurchase(m_SelectedShopId, m_SceneIdForPanel);
}

void BuyShopChoicePanel::onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
}
