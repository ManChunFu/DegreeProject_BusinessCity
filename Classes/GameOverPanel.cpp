#include "GameOverPanel.h"
#include "GameScene.h"

USING_NS_CC;

GameOverPanel::~GameOverPanel()
{
	m_GameScene->removeChild(m_GameoverPanel);
}

void GameOverPanel::init(GameScene* scene, cocos2d::Vec2 sceneMidPoint)
{
	m_GameScene = scene;

	m_GameoverPanel = Sprite::createWithSpriteFrameName("UIPanelRecBlack80.png");
	if (!m_GameoverPanel)
		return;

	m_GameoverPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(m_GameoverPanel, 3);
}
