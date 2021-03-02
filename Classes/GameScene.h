#pragma once

#ifndef __GAME_SCENE__H
#define __GAME_SCENE__H

#include "cocos2d.h"
#include <MouseOverMenuItem.h>
#include "cocostudio/SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;

class GameScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

private:
	Vec2 scaleFactor = Vec2::ONE;
	CREATE_FUNC(GameScene);
};

#endif
