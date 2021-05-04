#include "Canvas.h"
#include "SwitchSceneView.h"
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

	m_SwitchSceneView = nullptr;
	m_GameStartPanel = nullptr;
	m_InfoPanel = nullptr;
	m_ActionPanel = nullptr;

	m_GameoverPanel = nullptr;
	m_GameScene = nullptr;
}

void Canvas::Init(GameScene* scene, Player* player)
{
	m_GameScene = scene;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	m_SceneMidPoint = Point(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2));

	m_GameStartPanel = new GameStartPanel;
	m_GameStartPanel->autorelease();
	//m_GameStartPanel->openPanel(m_GameScene, m_SceneMidPoint);
	m_GameStartPanel->onDestroyCall = CC_CALLBACK_3(Canvas::destroyPanel, this);
	m_UIPanels.pushBack(m_GameStartPanel);

	m_SwitchSceneView = new SwitchSceneView();
	m_SwitchSceneView->autorelease();
	m_SwitchSceneView->runInit(m_GameScene, m_GameStartPanel, visibleSize, origin, m_SceneMidPoint);
	m_UIPanels.pushBack(m_SwitchSceneView);

	m_InfoPanel = new InfoPanel();
	m_InfoPanel->autorelease();
	m_InfoPanel->openPanel(m_GameScene, m_SceneMidPoint);
	m_UIPanels.pushBack(m_InfoPanel);

	m_ActionPanel = new ActionPanel;
	m_ActionPanel->autorelease();
	m_ActionPanel->openPanel(m_GameScene, m_SceneMidPoint);
	m_ActionPanel->setMainScenePointer(m_SwitchSceneView);
	m_UIPanels.pushBack(m_ActionPanel);

}

void Canvas::update(float deltaTime)
{
	if (m_AddPanels.empty() && m_RemovePanels.empty())
		return;

	for (auto panel : m_AddPanels)
	{
		m_UIPanels.pushBack(panel);
	}
	m_AddPanels.clear();

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
	m_GameoverPanel->onDestroyCall = CC_CALLBACK_1(Canvas::destroyPanel, this, 0, EPanels::DEFAULT_PANEL);
	m_AddPanels.pushBack(m_GameoverPanel);
}

void Canvas::destroyPanel(UIPanel* panel, unsigned shopId, EPanels uiPanel)
{
	m_RemovePanels.pushBack(panel);

	if (uiPanel == EPanels::DEFAULT_PANEL)
		return;

	activePanel(uiPanel, shopId);
}

void Canvas::activePanel(EPanels uiPanel, unsigned shopId)
{
	switch (uiPanel)
	{
	case EPanels::SWITCHSCENEVIEW_PANEL:
		break;
	case EPanels::ACTION_PANEL:
		m_ActionPanel->displayShop(shopId);
		//m_ActionPanel->displayShopOptions();
		m_InfoPanel->enableBankButton(true);
		break;
	}
}


