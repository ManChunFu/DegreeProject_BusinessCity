#pragma once
#include "cocos2d.h"
#include "2d/CCNode.h"

class GameScene;

class GameOverPanel : public cocos2d::Node
{
public:
	virtual ~GameOverPanel();
	
	void init(GameScene* scene, cocos2d::Vec2 sceneMidPoint);
private:
	GameScene* m_GameScene;
	cocos2d::Sprite* m_GameoverPanel = nullptr;
};