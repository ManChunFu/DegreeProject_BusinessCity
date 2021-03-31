#include "GameStartPanel.h"
#include "GameScene.h"
#include "GameData.h"
#include "MouseOverMenuItem.h"
#include "Shop.h"
#include "Player.h"
#include "GameFunctions.h"
#include "EPanel.h"

USING_NS_CC;

GameStartPanel::~GameStartPanel()
{}

void GameStartPanel::openPanel(GameScene* scene, Vec2 sceneMidPoint)
{
	m_GameScene = scene;
	m_Player = GameData::getInstance().m_Player;

	m_ThisPanel = Sprite::createWithSpriteFrameName("Brown_Panel_400.png");
	m_ThisPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(m_ThisPanel, 2);
	m_Elements.pushBack(m_ThisPanel);

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
			sceneMidPoint, itemTypes::BUTTON, 1.5f));

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

		auto buttonItem = MouseOverMenuItem::creatMouseOverMenuButton(item.second->m_ShopLook_Normal, item.second ->m_ShopLook_Lit, item.second ->m_ShopLook_Disabled,
			CC_CALLBACK_1(GameStartPanel::selectedItemCallback, this, item.first));

		if (!buttonItem)
			continue;
		
		m_MenuItems.pushBack(displayMenuButton(buttonItem, CC_CALLBACK_2(GameStartPanel::onMouseOver, this),
			Vec2(sceneMidPoint.x + (item.first % 2 == 0 ? -145.f : 145.f), sceneMidPoint.y + (item.first > 1 ? -90.f : 80.f)),
			itemTypes::DEFAULT, 0.8f));
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
	m_Elements.pushBack(menu);

	m_GameScene->addChild(menu, 3);
}

void GameStartPanel::goButtonCallback(cocos2d::Ref* pSender)
{
	m_Player->updateCurrentCashAmout(-GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_ShopPrice);
	
	for (auto element : m_Elements)
	{
		m_GameScene->removeChild(element);
	}
	if (onDestroyCall)
		onDestroyCall(this, EPanels::ACTION_PANEL);
}

void GameStartPanel::selectedItemCallback(cocos2d::Ref* pSender, unsigned shopID)
{
	m_Player->m_MyShopIds.push_back(shopID);

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

void GameStartPanel::onMouseOver(MouseOverMenuItem* item, cocos2d::Event* event)
{
}

