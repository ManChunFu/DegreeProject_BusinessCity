#include "GameStartPanel.h"
#include "GameScene.h"
#include "GameData.h"
#include "MouseOverMenuItem.h"
#include "Shop.h"

USING_NS_CC;


GameStartPanel::~GameStartPanel()
{
	m_StartupItems.clear(); 
	m_Elements.clear();
	m_GameScene = nullptr;
}

void GameStartPanel::createPanel(GameScene* scene, Vec2 sceneMidPoint)
{
	m_GameScene = scene;

	auto startupPanel = Sprite::createWithSpriteFrameName("Brown_Panel_400.png");
	startupPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(startupPanel, 2);
	m_Elements.push_back(startupPanel);

	auto startupLabel = Label::createWithTTF("CHOOSE YOUR STARTUP", "fonts/NirmalaB.ttf", 20);
	if (startupLabel)
	{
		startupLabel->enableShadow(Color4B::BLACK);
		GameFunctions::displayLabel(startupLabel, Color4B::WHITE, Vec2(startupPanel->getContentSize().width * 0.5f,
			startupPanel->getContentSize().height - 25.f), startupPanel, 1);
	}

	auto goButton = MouseOverMenuItem::creatMouseOverMenuButton("UIToggleButton.png", "UIToggleButton_Lit.png", "UIToggleButton_Disabled.png",
		CC_CALLBACK_1(GameStartPanel::goButtonCallback, this));
	if (goButton)
	{
		goButton->onMouseOver = CC_CALLBACK_2(GameStartPanel::onMouseOver, this);
		goButton->itemSelectedData.type = itemTypes::BUTTON;
		goButton->setScale(1.5f);
		goButton->setPosition(sceneMidPoint);
		goButton->setItemRect(sceneMidPoint, 1.5f);

		goButton->setVisible(false);
		goButton->setEnabled(false);

		auto goLabel = Label::createWithTTF("GO", "fonts/NirmalaB.ttf", 16);
		if (goLabel)
			GameFunctions::displayLabel(goLabel, Color4B::WHITE, Vec2(goButton->getContentSize().width * 0.5f,
				goButton->getContentSize().height * 0.5f), goButton, 1);

		m_StartupItems.pushBack(goButton);
	}

#pragma region create startup items
	auto item1 = MouseOverMenuItem::creatMouseOverMenuButton("HotdogStand.png", "HotdogStand_Lit.png", "HotdogStand_Disabled.png", 
		CC_CALLBACK_1(GameStartPanel::selectedItemCallback, this));

	if (item1)
	{
		item1->onMouseOver = CC_CALLBACK_2(GameStartPanel::onMouseOver, this);
		Vec2 item1Pos = Vec2(sceneMidPoint.x - 145.f, sceneMidPoint.y + 80.f);
		item1->setScale(0.8f);
		item1->setPosition(item1Pos);
		item1->setItemRect(item1Pos, 0.8f);

		auto item1MidPoint = Vec2(item1->getContentSize().width * 0.5f, item1->getContentSize().height * 0.5f);

		auto item1Label = Label::createWithTTF(GameData::getInstance().m_Shops[0]->m_ShopType, "fonts/NirmalaB.ttf", 20);
		if (item1Label)
			GameFunctions::displayLabel(item1Label, Color4B::BLACK, Vec2(item1MidPoint.x - 60.f,item1MidPoint.y + 80.f), item1, 1);

		auto cashSymbol = Label::createWithTTF("$", "fonts/Nirmala.ttf", 20);
		if (cashSymbol)
			GameFunctions::displayLabel(cashSymbol, Color4B::BLACK, Vec2(item1MidPoint.x - 100.f, item1MidPoint.y + 60.f),
				item1, 1);

		auto item1PriceLabel = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
		if (item1PriceLabel)
		{
			GameFunctions::updateLabelText_MoneyFormat(item1PriceLabel, GameData::getInstance().m_Shops[0]->m_ShopPrice);
			GameFunctions::displayLabel(item1PriceLabel, Color4B::BLACK, Vec2(item1MidPoint.x - 50.f, item1MidPoint.y + 60.f), item1, 1);
		}

		m_StartupItems.pushBack(item1);
	}

	auto item2 = MouseOverMenuItem::creatMouseOverMenuButton("Icecream.png", "Icecream_Lit.png", "Icecream_Disabled.png",
		CC_CALLBACK_1(GameStartPanel::selectedItemCallback, this));

	if (item2)
	{
		item2->onMouseOver = CC_CALLBACK_2(GameStartPanel::onMouseOver, this);
		Vec2 item2Pos = Vec2(sceneMidPoint.x + 145.f, sceneMidPoint.y + 80.f);
		item2->setScale(0.8f);
		item2->setPosition(item2Pos);
		item2->setItemRect(item2Pos, 0.8f);

		auto item2MidPoint = Vec2(item2->getContentSize().width * 0.5f, item2->getContentSize().height * 0.5f);

		auto item2Label = Label::createWithTTF(GameData::getInstance().m_Shops[1]->m_ShopType, "fonts/NirmalaB.ttf", 20);
		if (item2Label)
			GameFunctions::displayLabel(item2Label, Color4B::BLACK, Vec2(item2MidPoint.x - 50.f, item2MidPoint.y + 80.f), item2, 1);

		auto cashSymbol = Label::createWithTTF("$", "fonts/Nirmala.ttf", 20);
		if (cashSymbol)
			GameFunctions::displayLabel(cashSymbol, Color4B::BLACK, Vec2(item2MidPoint.x - 100.f, item2MidPoint.y + 60.f),
				item2, 1);

		auto item2PriceLabel = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
		if (item2PriceLabel)
		{
			GameFunctions::updateLabelText_MoneyFormat(item2PriceLabel, GameData::getInstance().m_Shops[1]->m_ShopPrice);
			GameFunctions::displayLabel(item2PriceLabel, Color4B::BLACK, Vec2(item2MidPoint.x - 50.f, item2MidPoint.y + 60.f), item2, 1);
		}

		m_StartupItems.pushBack(item2);
	}

	auto item3 = MouseOverMenuItem::creatMouseOverMenuButton("Handicrafts.png", "Handicrafts_Lit.png", "Handicrafts_Disabled.png",
		CC_CALLBACK_1(GameStartPanel::selectedItemCallback, this));

	if (item3)
	{
		item3->onMouseOver = CC_CALLBACK_2(GameStartPanel::onMouseOver, this);
		Vec2 item3Pos = Vec2(sceneMidPoint.x - 145.f, sceneMidPoint.y - 90.f);
		item3->setScale(0.8f);
		item3->setPosition(item3Pos);
		item3->setItemRect(item3Pos, 0.8f);

		auto item3MidPoint = Vec2(item3->getContentSize().width * 0.5f, item3->getContentSize().height * 0.5f);

		auto item3Label = Label::createWithTTF(GameData::getInstance().m_Shops[3]->m_ShopType, "fonts/NirmalaB.ttf", 20);
		if (item3Label)
			GameFunctions::displayLabel(item3Label, Color4B::BLACK, Vec2(item3MidPoint.x - 60.f, item3MidPoint.y + 80.f), item3, 1);

		auto cashSymbol = Label::createWithTTF("$", "fonts/Nirmala.ttf", 20);
		if (cashSymbol)
			GameFunctions::displayLabel(cashSymbol, Color4B::BLACK, Vec2(item3MidPoint.x - 100.f, item3MidPoint.y + 60.f),
				item3, 1);

		auto item3PriceLabel = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
		if (item3PriceLabel)
		{
			GameFunctions::updateLabelText_MoneyFormat(item3PriceLabel, GameData::getInstance().m_Shops[3]->m_ShopPrice);
			GameFunctions::displayLabel(item3PriceLabel, Color4B::BLACK, Vec2(item3MidPoint.x - 50.f, item3MidPoint.y + 60.f), item3, 1);
		}

		m_StartupItems.pushBack(item3);
	}
	
	auto item4 = MouseOverMenuItem::creatMouseOverMenuButton("FruitStand.png", "FruitStand_Lit.png", "FruitStand_Disabled.png",
		CC_CALLBACK_1(GameStartPanel::selectedItemCallback, this));

	if (item4)
	{
		item4->onMouseOver = CC_CALLBACK_2(GameStartPanel::onMouseOver, this);
		Vec2 item4Pos = Vec2(sceneMidPoint.x + 145.f, sceneMidPoint.y - 90.f);
		item4->setScale(0.8f);
		item4->setPosition(item4Pos);
		item4->setItemRect(item4Pos, 0.8f);

		auto item4MidPoint = Vec2(item4->getContentSize().width * 0.5f, item4->getContentSize().height * 0.5f);

		auto item4Label = Label::createWithTTF(GameData::getInstance().m_Shops[3]->m_ShopType, "fonts/NirmalaB.ttf", 20);
		if (item4Label)
			GameFunctions::displayLabel(item4Label, Color4B::BLACK, Vec2(item4MidPoint.x - 50.f, item4MidPoint.y + 80.f), item4, 1);

		auto cashSymbol = Label::createWithTTF("$", "fonts/Nirmala.ttf", 20);
		if (cashSymbol)
			GameFunctions::displayLabel(cashSymbol, Color4B::BLACK, Vec2(item4MidPoint.x - 100.f, item4MidPoint.y + 60.f),
				item4, 1);

		auto item4PriceLabel = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
		if (item4PriceLabel)
		{
			GameFunctions::updateLabelText_MoneyFormat(item4PriceLabel, GameData::getInstance().m_Shops[3]->m_ShopPrice);
			GameFunctions::displayLabel(item4PriceLabel, Color4B::BLACK, Vec2(item4MidPoint.x - 50.f, item4MidPoint.y + 60.f), item4, 1);
		}

		m_StartupItems.pushBack(item4);
	}
#pragma endregion

	auto menu = Menu::createWithArray(m_StartupItems);
	menu->setPosition(Vec2::ZERO);
	m_Elements.push_back(menu);

	m_GameScene->addChild(menu, 3);
}

void GameStartPanel::goButtonCallback(cocos2d::Ref* pSender)
{
	for (auto element : m_Elements)
	{
		m_GameScene->removeChild(element);
	}
	m_GameScene->deleteStartupPanel();
}

void GameStartPanel::selectedItemCallback(cocos2d::Ref* pSender)
{
	for (auto item : m_StartupItems)
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
			for (auto item : m_StartupItems)
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
		GameFunctions::displayLabel(item->itemSelectedData.selectedLabel, GameData::getInstance().m_ColorType.HotPink, Vec2(item->getContentSize().width * 0.5f,
			item->getContentSize().height * 0.5f - 60.f), item, 1),
		
		item->selected();
	}
}

void GameStartPanel::onMouseOver(MouseOverMenuItem* item, cocos2d::Event* event)
{

}
