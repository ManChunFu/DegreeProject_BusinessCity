#include "Canvas.h"
#include "InfoPanel.h"
#include "ActionPanel.h"
#include "GameStartPanel.h"
#include "GameOverPanel.h"
#include "GameScene.h"
#include "MyShopSettingPanel.h"
#include "EPanel.h"

USING_NS_CC;

Canvas::~Canvas()
{
	m_UIPanels.clear();

	m_AddPanels.clear();

	m_RemovePanels.clear();

	m_GameStartPanel = nullptr;
	m_InfoPanel = nullptr;
	m_ActionPanel = nullptr;

	m_GameoverPanel = nullptr;
	m_GameScene = nullptr;
}

void Canvas::setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale)
{
	sprite->setScaleX((m_VisibleSize.width / sprite->getContentSize().width) * scale.x);
	sprite->setScaleY((m_VisibleSize.height / sprite->getContentSize().height) * scale.y);
}

void Canvas::Init(GameScene* scene, Player* player)
{
	m_GameScene = scene;
	m_VisibleSize = Director::getInstance()->getVisibleSize();
	m_Origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::createWithSpriteFrameName("GameSceneCityView.png");
	if (!backgroundSprite)
		return;

	m_SceneMidPoint = Point(m_Origin.x + (m_VisibleSize.width / 2), m_Origin.y + (m_VisibleSize.height / 2));
	GameFunctions::displaySprite(backgroundSprite, m_SceneMidPoint, m_GameScene, 0);
	setSpriteScale(backgroundSprite, Vec2::ONE);

	m_InfoPanel = new InfoPanel();
	m_InfoPanel->autorelease();
	m_InfoPanel->openPanel(m_GameScene, m_SceneMidPoint);
	m_UIPanels.pushBack(m_InfoPanel);

	m_ActionPanel = new ActionPanel;
	m_ActionPanel->autorelease();
	m_ActionPanel->openPanel(m_GameScene, m_SceneMidPoint);
	m_UIPanels.pushBack(m_ActionPanel);

	m_GameStartPanel = new GameStartPanel;
	m_GameStartPanel->autorelease();
	m_GameStartPanel->openPanel(m_GameScene, m_SceneMidPoint);
	m_GameStartPanel->onDestroyCall = CC_CALLBACK_2(Canvas::destroyPanel, this);
	m_UIPanels.pushBack(m_GameStartPanel);
}

void Canvas::update(float deltaTime)
{
	for (auto panel : m_AddPanels)
	{
		m_UIPanels.pushBack(panel);
	}
	m_AddPanels.clear();

	//for (auto panel : m_UIPanels)
	//{
	//	panel->update(deltaTime);
	//}

	for (int index = 0; index < m_UIPanels.size(); index++)
	{
		for (auto panel : m_RemovePanels)
		{
			if (m_UIPanels.at(index) == panel)
			{
				m_UIPanels.erase(m_UIPanels.begin() + index);
				index--;
			}
		}
	}

	m_RemovePanels.clear();
}

void Canvas::gameOver()
{
	m_InfoPanel->enableBankButton(false);
	m_GameoverPanel = new GameOverPanel;
	m_GameoverPanel->autorelease();
	m_GameoverPanel->openPanel(m_GameScene, m_SceneMidPoint);
	m_GameoverPanel->onDestroyCall = CC_CALLBACK_2(Canvas::destroyPanel, this);
	m_AddPanels.pushBack(m_GameoverPanel);
}

void Canvas::destroyPanel(UIPanel* panel, EPanels uiPanel)
{
	m_RemovePanels.pushBack(panel);

	if (uiPanel == EPanels::DEFAULT_PANEL)
		return;

	activePanel(uiPanel);
}

void Canvas::actionCall(cocos2d::Ref* pSender, EPanels uiPanel)
{
	if (uiPanel == EPanels::DEFAULT_PANEL)
		return;

	activePanel(uiPanel);
}

void Canvas::activePanel(EPanels uiPanel)
{
	switch (uiPanel)
	{
	case EPanels::ACTION_PANEL:
		m_ActionPanel->displayShop();
		m_InfoPanel->enableBankButton(true);
		break;
	case EPanels::GAMEOVER_PANEL:
		break;
	case EPanels::MYSHOPSETTING_PANEL:
		break;
	}
}


