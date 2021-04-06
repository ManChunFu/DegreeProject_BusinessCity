#include "ActionPanel.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "Player.h"
#include "MouseOverMenuItem.h"
#include "GameScene.h"
#include "Shop.h"
#include "MyShopSettingPanel.h"

USING_NS_CC;

ActionPanel::~ActionPanel()
{
	m_ShopButton = nullptr;
	m_MyShopList.clear();
}

void ActionPanel::openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint)
{
	m_GameScene = scene;
	m_SceneMidPoint = sceneMidPoint;
	m_Player = GameData::getInstance().m_Player;

	m_ThisPanel = Sprite::createWithSpriteFrameName("InGamePanel_Black_80.png");
	if (!m_ThisPanel)
		return;

	m_ThisPanel->setPosition(Vec2(m_SceneMidPoint.x - 160.f, m_SceneMidPoint.y - 320.f));
	m_GameScene->addChild(m_ThisPanel, 1);
	m_Elements.pushBack(m_ThisPanel);

	m_DisplayShopPos = Vec2(m_ThisPanel->getPosition().x * 0.5f, m_ThisPanel->getContentSize().height * 0.5f - 5.f);
}

void ActionPanel::displayShop(unsigned shopId)
{
	auto myShop = GameData::getInstance().m_Shops[shopId];
	auto myShopButton = MouseOverMenuItem::creatMouseOverMenuButton(myShop->m_ShopLook_Normal, myShop->m_ShopLook_Lit, myShop->m_ShopLook_Disabled,
		CC_CALLBACK_1(ActionPanel::openShopCallback, this, m_ShopIndex, shopId));
	
	if (myShopButton)
		m_MenuItems.pushBack(displayMenuButton(myShopButton, CC_CALLBACK_2(ActionPanel::onMouseOver, this), m_DisplayShopPos, 
			itemTypes::DEFAULT, 0.3f));

	auto menu = Menu::create(myShopButton, NULL);
	menu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(menu, 2);
	m_Elements.pushBack(menu);

	auto shopButton = new MyShopSettingPanel();
	shopButton->autorelease();
	m_ThisPanel->addChild(shopButton, 1);
	m_MyShopList.pushBack(shopButton);

	m_DisplayShopPos.x += 120.f;
}

void ActionPanel::openShopCallback(cocos2d::Ref* pSender, unsigned shopIndex, unsigned shopId)
{
	if (m_MyShopList.size() < 0)
		return;

	if (m_MyShopList.at(shopIndex)->isPanelOpen())
		m_MyShopList.at(shopIndex)->closePanel();
	else
		m_MyShopList.at(shopIndex)->openPanel(m_GameScene, m_SceneMidPoint, shopId);
}

void ActionPanel::onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event)
{
}

void ActionPanel::checkShopCallback(cocos2d::Ref* pSender, unsigned shopId)
{
	m_Player->m_MyShopIds.push_back(shopId);
	m_ShopIndex++;
	displayShop(shopId);
}

void ActionPanel::displayShopOptions()
{
	auto shopHotDogButton = MouseOverMenuItem::creatMouseOverMenuButton("Checkbox_Normal.png", GameData::getInstance().m_Shops[0]->m_ShopLook_Normal,
		"Checkbox_Normal.png", CC_CALLBACK_1(ActionPanel::checkShopCallback, this, 0));
	if (shopHotDogButton)
	{
		m_MenuItems.pushBack(displayMenuButton(shopHotDogButton, CC_CALLBACK_2(ActionPanel::onMouseOver, this),
			Vec2(m_SceneMidPoint.x + 150.f, m_SceneMidPoint.y - 250.f), itemTypes::DEFAULT, 0.5f));
	}

	auto shopButton = Menu::create(shopHotDogButton, NULL);
	shopButton->setPosition(Vec2::ZERO);
	m_GameScene->addChild(shopButton, 2);
}

