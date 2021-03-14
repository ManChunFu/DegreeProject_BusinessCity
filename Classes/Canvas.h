#pragma once

#include "cocos2d.h"
#include "GameFunctions.h"
#include "GameData.h"


class Canvas  
{
public:
	cocos2d::Size m_VisibleSize;
	cocos2d::Vec2 m_origin;
	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);
};
