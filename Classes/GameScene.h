#pragma once

#ifndef __GAME_SCENE__H
#define __GAME_SCENE__H

#include "cocos2d.h"

namespace CocosDenshion
{
	class SimpleAudioEngine;
}
class MouseOverMenuItem;
class Bank;
class Player;
class GameStartPanel;

class GameScene : public cocos2d::Layer
{
public:
	~GameScene();

	static cocos2d::Scene* createScene();

	virtual bool init() override;
	
	void update(float delta);
	CREATE_FUNC(GameScene);

	Player* m_Player = nullptr;
	GameStartPanel* m_StartupPanel = nullptr;
	void updateCurrentCash(int amout);
	void deleteStartupPanel();
private:
	cocos2d::Size m_VisibleSize;
	cocos2d::Label* m_Saving = nullptr;
	cocos2d::Sprite* m_TopPanel = nullptr;
	cocos2d::Sprite* m_BottomPanel = nullptr;
	cocos2d::Label* m_WeekCount = nullptr;
	cocos2d::Label* m_WeekDay = nullptr;
	cocos2d::Label* m_TimeHourDisplay = nullptr;
	cocos2d::Label* m_TimeMinDisplay = nullptr;
	MouseOverMenuItem* m_BankButton = nullptr;
	cocos2d::Vector<cocos2d::MenuItem*> m_MenuItems;
	
	Bank* m_Bank = nullptr;

	float m_ElapsedTime = 0.f;
	unsigned m_CurrentMinute = 0;
	unsigned m_CurrentHour = 8;
	unsigned m_Today = 0;
	std::string m_WeekDays[7] = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY" };
	unsigned m_Weeks = 1;
	int m_CurrentCash = 50000;

	bool m_IsOpeningSubWindow = false;

	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);
	void updateGameTime(float delta);
	
	void checkBalanceCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onCurrentCashChange(Player* player, int currentCashAmout);

};
#endif
