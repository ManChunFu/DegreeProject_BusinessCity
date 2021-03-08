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
	CREATE_FUNC(GameScene);
private:
	Size visibleSize;
	Sprite* topPanel = nullptr;
	Sprite* bottomPanel = nullptr;
	Label* weekCount = nullptr;
	Label* dayOfWeek = nullptr;
	Label* timeHourDisplay = nullptr;
	Label* timeMinDisplay = nullptr;
	float elapsedTime = 0.f;
	unsigned currentMinute = 0;
	unsigned currentHour = 8;
	void setSpriteScale(Sprite* sprite, Vec2 scale);

};

#endif
