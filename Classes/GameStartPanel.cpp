#include "GameStartPanel.h"
#include "GameScene.h"
#include "SwitchSceneView.h"
#include "GameData.h"
#include "MouseOverMenuItem.h"
#include "Shop.h"
#include "Player.h"
#include "GameFunctions.h"
#include "EPanel.h"

USING_NS_CC;

GameStartPanel::GameStartPanel(SwitchSceneView* mainScene, GameScene* scene, Vec2 sceneMidPoint)
{
	m_MainSceneView = mainScene;
	m_GameScene = scene;

	m_MainSceneView->m_StartupNotify = CC_CALLBACK_0(GameStartPanel::openPanel, this, m_GameScene, sceneMidPoint);
}

GameStartPanel::~GameStartPanel()
{
	m_MainSceneView->m_StartupNotify = nullptr;
	m_MainSceneView = nullptr;
}

void GameStartPanel::openPanel(GameScene* scene, Vec2 sceneMidPoint)
{
	m_IsPanelOpen = true;
	GameData::getInstance().setTempOpenPanel(this);

	if (!m_ThisPanel)
	{
		m_Player = GameData::getInstance().m_Player;
		createPanel(sceneMidPoint);
		return;
	}

	m_ThisPanel->setVisible(true);
}

void GameStartPanel::createPanel(cocos2d::Vec2 sceneMidPoint, unsigned shopId)
{
	m_ThisPanel = Sprite::createWithSpriteFrameName("Brown_Panel_400.png");
	m_ThisPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(m_ThisPanel, 2);
	m_Elements.pushBack(m_ThisPanel);

	auto panelMidPoint = Vec2(m_ThisPanel->getContentSize() * 0.5f);
	auto closePanelButton = MouseOverMenuItem::creatMouseOverMenuButton("CloseButton_Normal.png", "CloseButton_Lit.png", "CloseButton_Disable.png",
		CC_CALLBACK_1(GameStartPanel::closeCallback, this));
	if (closePanelButton)
		m_MenuItems.pushBack(displayMenuButton(closePanelButton, CC_CALLBACK_2(GameStartPanel::onMouseOver, this),
			Vec2(panelMidPoint.x + 270.f, panelMidPoint.y + 175.f), itemTypes::DEFAULT, 0.7f, true, Vec2(340.f, 165.f)));


	auto startupLabel = Label::createWithTTF("CHOOSE YOUR STARTUP", "fonts/NirmalaB.ttf", 20);
	if (startupLabel)
	{
		startupLabel->enableShadow(Color4B::BLACK);
		GameFunctions::displayLabel(startupLabel, Color4B::WHITE, Vec2(m_ThisPanel->getContentSize().width * 0.5f,
			m_ThisPanel->getContentSize().height - 25.f), m_ThisPanel, 1);
	}

	auto goButton = MouseOverMenuItem::creatMouseOverMenuButton("UIToggleButton.png", "UIToggleButton_Lit.png", "UIToggleButton_Disabled.png",
		CC_CALLBACK_1(GameStartPanel::goButtonCallback, this));
	if (goButton)
	{
		m_MenuItems.pushBack(displayMenuButton(goButton, CC_CALLBACK_2(GameStartPanel::onMouseOver, this),
			panelMidPoint, itemTypes::BUTTON, 1.5f, true, Vec2(5.f, 0.f)));

		goButton->setVisible(false);
		goButton->setEnabled(false);

		auto goLabel = Label::createWithTTF("GO", "fonts/NirmalaB.ttf", 16);
		if (goLabel)
			GameFunctions::displayLabel(goLabel, Color4B::WHITE, Vec2(goButton->getContentSize().width * 0.5f,
				goButton->getContentSize().height * 0.5f), goButton, 1);
	}

#pragma region create startup items

	for (auto item : GameData::getInstance().m_Shops)
	{
		if (!item.second->m_Startup)
			continue;

		auto buttonItem = MouseOverMenuItem::creatMouseOverMenuButton(item.second->m_ShopLook_Normal, item.second->m_ShopLook_Lit, item.second->m_ShopLook_Disabled,
			CC_CALLBACK_1(GameStartPanel::selectedItemCallback, this, item.first));

		if (!buttonItem)
			continue;

		m_MenuItems.pushBack(displayMenuButton(buttonItem, CC_CALLBACK_2(GameStartPanel::onMouseOver, this),
			Vec2(panelMidPoint.x + (item.first % 2 == 0 ? -145.f : 145.f), panelMidPoint.y + (item.first > 1 ? -90.f : 80.f)),
			itemTypes::DEFAULT, 0.8f, true));
		auto buttonMidPoint = Vec2(buttonItem->getContentSize().width * 0.5f, buttonItem->getContentSize().height * 0.5f);

		auto buttonLabel = Label::createWithTTF(item.second->m_ShopType, "fonts/NirmalaB.ttf", 20);
		if (buttonLabel)
			GameFunctions::displayLabel(buttonLabel, Color4B::BLACK, Vec2(buttonMidPoint.x - 130.f, buttonMidPoint.y + 60.f),
				buttonItem, 1, true, TextHAlignment::LEFT);

		auto cashSymbol = Label::createWithTTF("$", "fonts/Nirmala.ttf", 20);
		if (cashSymbol)
			GameFunctions::displayLabel(cashSymbol, Color4B::BLACK, Vec2(buttonMidPoint.x - 130.f, buttonMidPoint.y + 40.f),
				buttonItem, 1, true, TextHAlignment::LEFT);

		auto buttonPriceLabel = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
		if (buttonPriceLabel)
		{
			GameFunctions::updateLabelText_MoneyFormat(buttonPriceLabel, item.second->m_ShopPrice);
			GameFunctions::displayLabel(buttonPriceLabel, Color4B::BLACK, Vec2(buttonMidPoint.x - 100.f, buttonMidPoint.y + 40.f),
				buttonItem, 1, true, TextHAlignment::LEFT);
		}
	}

#pragma endregion

	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	m_ThisPanel->addChild(menu, 1);
}

void GameStartPanel::closePanel()
{
	m_ThisPanel->setVisible(false);
	m_IsPanelOpen = false;

	GameData::getInstance().m_TempOpenPanel = nullptr;
}

void GameStartPanel::goButtonCallback(cocos2d::Ref* pSender)
{
	m_Player->m_MyShopIds.push_back(m_SelectedShopId);
	m_Player->updateCurrentCashAmout(-GameData::getInstance().m_Shops[m_SelectedShopId]->m_ShopPrice);

	closePanel();
	
	for (auto element : m_Elements)
	{
		m_GameScene->removeChild(element);
	}
	if (onDestroyCall)
		onDestroyCall(this, m_SelectedShopId, EPanels::ACTION_PANEL);
}

void GameStartPanel::selectedItemCallback(cocos2d::Ref* pSender, unsigned shopId)
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

void GameStartPanel::closeCallback(cocos2d::Ref* pSender)
{
	if (GameData::getInstance().isPopupOpen())
		return;

	closePanel();
}

void GameStartPanel::onMouseOver(MouseOverMenuItem* item, cocos2d::Event* event)
{
}

