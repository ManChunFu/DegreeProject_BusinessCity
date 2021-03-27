#include "Canvas.h"
#include "InfoPanel.h"
#include "GameStartPanel.h"
#include "GameOverPanel.h"
#include "GameScene.h"
#include "MyShopSettingPanel.h"
#include "EPanel.h"


USING_NS_CC;


Canvas::~Canvas()
{
	for (auto panel : m_UIPanels)
	{
		delete panel;
	}
	m_UIPanels.clear();
	m_AddPanels.clear();
	m_RemovePanels.clear();
	m_InfoPanel = nullptr;
	m_GameStartPanel = nullptr;
	m_MyShopPanel = nullptr;
	m_GameoverPanel = nullptr;

	delete m_GameStartPanel;
	m_GameStartPanel = nullptr;
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
	m_InfoPanel->openPanel(m_GameScene, m_SceneMidPoint);
	m_UIPanels.push_back(m_InfoPanel);

	m_GameStartPanel = new GameStartPanel();
	m_GameStartPanel->createPanel(m_GameScene, m_SceneMidPoint);
	m_GameStartPanel->onDestroyCall = CC_CALLBACK_1(Canvas::destroyPanel, this, EPanels::MYSHOPSETTING_PANEL);
	m_UIPanels.push_back(m_GameStartPanel);
}

void Canvas::update(float deltaTime)
{
	for (auto panel : m_AddPanels)
	{
		m_UIPanels.push_back(panel);
	}
	m_AddPanels.clear();

	for (auto panel : m_UIPanels)
	{
		panel->update(deltaTime);
	}

	for (unsigned index = 0; index < m_UIPanels.size(); index++)
	{
		for (auto ref : m_RemovePanels)
		{
			if (m_UIPanels[index] == ref)
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
	m_GameoverPanel->init(m_GameScene, m_SceneMidPoint);
	m_GameoverPanel->onDestroyCall = CC_CALLBACK_1(Canvas::destroyPanel, this, EPanels::DEFAULT_PANEL);
	m_AddPanels.push_back(m_GameoverPanel);
}

void Canvas::destroyPanel(cocos2d::Ref* pSender, EPanels uiPanel)
{
	m_RemovePanels.push_back(pSender);

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
	case EPanels::INFO_PANEL:
		m_InfoPanel->enableBankButton(true);
		break;
	case EPanels::GAMEOVER_PANEL:
		break;
	case EPanels::MYSHOPSETTING_PANEL:
		createMyShopPanel();
		break;
	}
}

void Canvas::createMyShopPanel()
{
	m_MyShopPanel = new MyShopSettingPanel();
	m_MyShopPanel->openPanel(m_GameScene, m_SceneMidPoint);
	m_MyShopPanel->onActionCall = CC_CALLBACK_1(Canvas::actionCall, this, EPanels::INFO_PANEL);
	m_AddPanels.push_back(m_MyShopPanel);

	// ToDo: move this call to its own funciton 
	m_InfoPanel->enableBankButton(true);

}

