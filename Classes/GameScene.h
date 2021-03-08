#pragma once

#ifndef __GAME_SCENE__H
#define __GAME_SCENE__H

#include "cocos2d.h"
#include <MouseOverMenuItem.h>
#include "cocostudio/SimpleAudioEngine.h"
#include "2d/CCLayer.h"

using namespace CocosDenshion;
USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;
	
	void update(float delta);
private:
	unsigned elapsedTime = 0;
	Size visibleSize;
	Sprite* topPanel = nullptr;
	Sprite* bottomPanel = nullptr;
	Label* weekCount = nullptr;
	Label* dayOfWeek = nullptr;
	Label* timeDisplay = nullptr;
	void setSpriteScale(Sprite* sprite, Vec2 scale);
	

	CREATE_FUNC(GameScene);
};

#endif
