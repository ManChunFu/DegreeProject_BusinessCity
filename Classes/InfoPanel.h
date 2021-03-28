#pragma once

#include "UIPanel.h"

class Bank;
class GameScene;
class MouseOverMenuItem;

class InfoPanel : public UIPanel
{
public:
	 ~InfoPanel() override;

	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;
	void update(float delta);

	void enableBankButton(bool value);
private:
	cocos2d::Label* m_Saving = nullptr;
	cocos2d::Label* m_WeekCount = nullptr;
	cocos2d::Label* m_WeekDay = nullptr;
	cocos2d::Label* m_TimeHourDisplay = nullptr;
	cocos2d::Label* m_TimeMinDisplay = nullptr;

	Bank* m_Bank = nullptr;
	MouseOverMenuItem* m_BankButton = nullptr;

	cocos2d::Vec2 m_SceneMidPoint = cocos2d::Vec2::ZERO;

	float m_ElapsedTime = 0.f;
	unsigned m_CurrentMinute = 0;
	unsigned m_CurrentHour = 8;
	unsigned m_Today = 1;
	std::string m_WeekDays[7] = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY" };
	unsigned m_Weeks = 1;

	bool m_IsOpeningSubWindow = false;
	void checkBalanceCallback(cocos2d::Ref* pSender, GameScene* scene);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onCurrentCashChange(Player* player, int currentCashAmout);

};
