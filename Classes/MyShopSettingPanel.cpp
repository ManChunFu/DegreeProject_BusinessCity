#include "MyShopSettingPanel.h"
#include "GameScene.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "Player.h"
#include "Shop.h"
#include "MouseOverMenuItem.h"

USING_NS_CC;

MyShopSettingPanel::~MyShopSettingPanel()
{
	printf("");
}

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

	auto textAligmentLeft = panelMidPoint.x - 250.f;
	auto shopName = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
	if (shopName)
	{
		shopName->setString(GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Name);
		GameFunctions::displayLabel(shopName, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y + 190.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	}

	auto employeeText = Label::createWithTTF("Employees", "fonts/NirmalaB.ttf", 20);
	if (employeeText)
		GameFunctions::displayLabel(employeeText, Color4B::BLACK, Vec2(textAligmentLeft, panelMidPoint.y + 150.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);

	auto boxSprite = Sprite::create("X/Border_Blue.png");
	if (boxSprite)
	{
		GameFunctions::displaySprite(boxSprite, Vec2(panelMidPoint.x - 80.f, panelMidPoint.y + 160.f), m_ThisPanel, 1);

		auto employeeNo = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (employeeNo)
		{
			employeeNo->setString(std::to_string(GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Employees));
			GameFunctions::displayLabel(employeeNo, Color4B::WHITE, Vec2(60.f, 35.f), boxSprite, 1, true, TextHAlignment::RIGHT);
		}
	}

	/*auto reduceAmoutButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonCorner40.png", "UIButtonCorner40_Lit.png", "UIButtonCorner40_Disabled.png",
		CC_CALLBACK_1(Bank::reduceAmoutCallback, this));

	if (reduceAmoutButton)
	{
		reduceAmoutButton->onMouseOver = CC_CALLBACK_2(Bank::onMouseOver, this);
		Vec2 reducePos = Vec2(sceneMidPoint.x - 175.f, sceneMidPoint.y - 175.f);
		reduceAmoutButton->setScale(0.5f);
		reduceAmoutButton->setPosition(reducePos);
		reduceAmoutButton->setItemRect(reducePos, 0.5f);

		m_BankButtons.pushBack(reduceAmoutButton);
	}*/

}

void MyShopSettingPanel::reduceCallback(cocos2d::Ref* pSender)
{
}
