#include "MyShopSettingPanel.h"
#include "GameScene.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "Player.h"
#include "Shop.h"

USING_NS_CC;

MyShopSettingPanel::~MyShopSettingPanel()
{}

void MyShopSettingPanel::openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint)
{
	if (!m_ThisPanel)
	{
		m_GameScene = scene;
		m_Player = GameData::getInstance().m_Player;

		if (m_GameScene && m_Player)
			createPanel(sceneMidPoint);
		
		return;
	}

	m_ThisPanel->setVisible(true);
}

void MyShopSettingPanel::closePanel()
{
}

void MyShopSettingPanel::createPanel(cocos2d::Vec2 sceneMidPoint)
{
	m_ThisPanel = Sprite::create("X/Brown_Panel_500.png");
	if (!m_ThisPanel)
		return;

	m_ThisPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(m_ThisPanel, 1);

	auto panelMidPoint = Vec2(m_ThisPanel->getContentSize().width * 0.5f, m_ThisPanel->getContentSize().height * 0.5f);
	
	auto shopPic = Sprite::createWithSpriteFrameName(GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_ShopLook_Normal);
	if (shopPic)
	{
		GameFunctions::displaySprite(shopPic, Vec2(panelMidPoint.x + 150.f, panelMidPoint.y + 160.f), m_ThisPanel, 1, 0.6f, 0.6f);
		
		auto shopType = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (shopType)
		{
			shopType->setString(GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_ShopType);
			GameFunctions::displayLabel(shopType, Color4B::WHITE, Vec2(shopPic->getContentSize().width * 0.5f, -10.f), shopPic, 1);
		}
	}

	auto shopName = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
	if (shopName)
	{
		shopName->setString(GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Name);
		GameFunctions::displayLabel(shopName, Color4B::BLACK, Vec2(panelMidPoint.x - 250.f, panelMidPoint.y + 190.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	}



}
