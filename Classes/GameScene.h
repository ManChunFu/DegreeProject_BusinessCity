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
	~GameScene();

	static cocos2d::Scene* createScene();

	virtual bool init() override;
	
	void update(float delta);
	CREATE_FUNC(GameScene);

private:
	cocos2d::Size m_VisibleSize;
	cocos2d::Label* m_Saving = nullptr;
	cocos2d::Sprite* m_TopPanel = nullptr;
	cocos2d::Sprite* m_BottomPanel = nullptr;
	cocos2d::Label* m_WeekCount = nullptr;
	cocos2d::Label* m_WeekDay = nullptr;
	cocos2d::Label* m_TimeHourDisplay = nullptr;
	cocos2d::Label* m_TimeMinDisplay = nullptr;
	float m_ElapsedTime = 0.f;
	unsigned m_CurrentMinute = 0;
	unsigned m_CurrentHour = 8;
	unsigned m_Today = 0;
	std::string m_WeekDays[7] = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY" };
	unsigned m_Weeks = 1;
	int m_CurrentCash = 50000;
	cocos2d::Vector<cocos2d::MenuItem*> menuItems;

	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);
	void updateGameTime(float delta);
	void updateTimeLabel(cocos2d::Label* label, unsigned value);
	void updateSavingLabel(cocos2d::Label* label, int value);
	
	void checkBalanceCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
};
#endif
