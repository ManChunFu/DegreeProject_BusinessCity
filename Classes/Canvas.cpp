#include "Canvas.h"
#include "Player.h"
#include "InfoPanel.h"
#include "GameStartPanel.h"

USING_NS_CC;

Size m_VisibleSize = Director::getInstance()->getVisibleSize();
Vec2 m_origin = Director::getInstance()->getVisibleOrigin();

void Canvas::setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale)
{
	sprite->setScaleX((m_VisibleSize.width / sprite->getContentSize().width) * scale.x);
	sprite->setScaleY((m_VisibleSize.height / sprite->getContentSize().height) * scale.y);
}

void Canvas::Init(GameScene* scene, Player* player, cocos2d::Vec2 sceneMidPoint)
{
	auto infoPanel = new InfoPanel();
	infoPanel->createPanel(scene, player, sceneMidPoint);
	m_UIPanels.push_back(infoPanel);

	auto gameStartPanel = new GameStartPanel();
	gameStartPanel->createPanel(scene, sceneMidPoint);
	m_UIPanels.push_back(gameStartPanel);
}
