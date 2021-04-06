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
	m_MyShop = nullptr;
	m_ShopButton = nullptr;
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
}

void ActionPanel::displayShop(unsigned shopId)
{
	m_MyShop = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[shopId]];
	auto myFirstShopButton = MouseOverMenuItem::creatMouseOverMenuButton(m_MyShop->m_ShopLook_Normal, m_MyShop->m_ShopLook_Lit, m_MyShop->m_ShopLook_Disabled,
		CC_CALLBACK_1(ActionPanel::openShopCallback, this));
	
	if (myFirstShopButton)
		m_MenuItems.pushBack(displayMenuButton(myFirstShopButton, CC_CALLBACK_2(ActionPanel::onMouseOver, this),
			Vec2(m_ThisPanel->getPosition().x * 0.5f, m_ThisPanel->getContentSize().height * 0.5f - 5.f), itemTypes::DEFAULT, 0.3f));

	m_ShopButton = new MyShopSettingPanel();
	m_ShopButton->autorelease();
	m_ThisPanel->addChild(m_ShopButton, 1);

	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(menu, 2);
	m_Elements.pushBack(menu);
}

void ActionPanel::openShopCallback(cocos2d::Ref* pSender)
{
	if (!m_ShopButton)
		return;

	(m_ShopButton->isPanelOpen()) ? m_ShopButton->closePanel() : m_ShopButton->openPanel(m_GameScene, m_SceneMidPoint);
}

void ActionPanel::onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event)
{
}

