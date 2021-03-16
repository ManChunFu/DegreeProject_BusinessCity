#pragma once

#include "cocos2d.h"
#include "GameFunctions.h"
#include "GameData.h"

class Player;

class Canvas  
{
public:
	void Init(GameScene* scene, Player* player, cocos2d::Vec2 sceneMidPoint);
	cocos2d::Size m_VisibleSize;
	cocos2d::Vec2 m_origin;
	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);

	std::vector<Canvas*> m_UIPanels;
};
