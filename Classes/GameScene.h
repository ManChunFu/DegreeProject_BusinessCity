#pragma once

#ifndef __GAME_SCENE__H
#define __GAME_SCENE__H

#include "cocos2d.h"

namespace CocosDenshion
{
	class SimpleAudioEngine;
}
class MouseOverMenuItem;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init() override;
	
	void update(float delta);
	CREATE_FUNC(GameScene);

private:
	cocos2d::Size m_VisibleSize;
	cocos2d::Sprite* m_TopPanel = nullptr;
	cocos2d::Sprite* m_BottomPanel = nullptr;
	cocos2d::Label* m_WeekCount = nullptr;
	cocos2d::Label* m_DayOfWeek = nullptr;
	cocos2d::Label* m_TimeHourDisplay = nullptr;
	cocos2d::Label* m_TimeMinDisplay = nullptr;
	float m_ElapsedTime = 0.f;
	unsigned m_CurrentMinute = 0;
	unsigned m_CurrentHour = 8;
	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);

};
#endif
