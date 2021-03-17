#pragma once

#include "cocos2d.h"
#include "2d/CCNode.h"

class Player;
class Bank;
class GameScene;
class MouseOverMenuItem;

class InfoPanel : public cocos2d::Node
{
public:
	virtual ~InfoPanel();

	cocos2d::Sprite* m_InfoPanel = nullptr;
	void createPanel(GameScene* scene, Player* player, cocos2d::Vec2 sceneMidPoint);
	void update(float delta);

	void enableBankButton();
private:
	GameScene* m_GameScene = nullptr;
	cocos2d::Label* m_Saving = nullptr;
	cocos2d::Sprite* m_TopPanel = nullptr;
	cocos2d::Sprite* m_BottomPanel = nullptr;
	cocos2d::Label* m_WeekCount = nullptr;
	cocos2d::Label* m_WeekDay = nullptr;
	cocos2d::Label* m_TimeHourDisplay = nullptr;
	cocos2d::Label* m_TimeMinDisplay = nullptr;
	cocos2d::Vector<cocos2d::MenuItem*> m_MenuItems;
	MouseOverMenuItem* m_BankButton = nullptr;

	Bank* m_Bank = nullptr;

	float m_ElapsedTime = 0.f;
	unsigned m_CurrentMinute = 0;
	unsigned m_CurrentHour = 8;
	unsigned m_Today = 0;
	std::string m_WeekDays[7] = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY" };
	unsigned m_Weeks = 1;
	int m_CurrentCash = 50000;

	bool m_IsOpeningSubWindow = false;
	void checkBalanceCallback(cocos2d::Ref* pSender, GameScene* scene);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onCurrentCashChange(Player* player, int currentCashAmout);

};
