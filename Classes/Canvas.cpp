#include "Canvas.h"
#include "InfoPanel.h"
#include "GameStartPanel.h"
#include "GameScene.h"

USING_NS_CC;


Canvas::~Canvas()
{
	for (auto panel : m_UIPanels)
	{
		delete panel;
	}
	m_UIPanels.clear();
	m_InfoPanel = nullptr;
	m_GameStartPanel = nullptr;
}

void Canvas::setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale)
{
	sprite->setScaleX((m_VisibleSize.width / sprite->getContentSize().width) * scale.x);
	sprite->setScaleY((m_VisibleSize.height / sprite->getContentSize().height) * scale.y);
}

void Canvas::Init(GameScene* scene, Player* player, cocos2d::Vec2 sceneMidPoint)
{
	m_VisibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();

	m_InfoPanel = new InfoPanel();
	m_InfoPanel->createPanel(scene, player, sceneMidPoint);
	m_UIPanels.push_back(m_InfoPanel);

	m_GameStartPanel = new GameStartPanel();
	m_GameStartPanel->createPanel(scene, sceneMidPoint);
	m_GameStartPanel->onDestroyCall = CC_CALLBACK_1(Canvas::destroyPanel, this);
	m_UIPanels.push_back(m_GameStartPanel);
}

void Canvas::update(float deltaTime)
{
	for (auto panel : m_UIPanels)
	{
		panel->update(deltaTime);
	}
}

void Canvas::destroyPanel(cocos2d::Ref* pSender)
{
	for (unsigned index = 0;  index < m_UIPanels.size(); index++)
	{
		if (m_UIPanels[index] == m_GameStartPanel)
		{
			m_UIPanels.erase(m_UIPanels.begin() + index);
		}
	}

	delete m_GameStartPanel;
	m_GameStartPanel = nullptr;

	m_InfoPanel->enableBankButton();
}

