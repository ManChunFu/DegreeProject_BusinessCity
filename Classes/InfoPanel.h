#pragma once

#include "UIPanel.h"

class GlobalTime;
class Bank;
class GameScene;
class MouseOverMenuItem;

class InfoPanel : public UIPanel
{
public:
	 ~InfoPanel() override;

	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;

	void enableBankButton(bool value);

protected:
	void onEveryMinuteChanges(GlobalTime* globalTime, unsigned minute);
	void onEveryHourChanges(GlobalTime* globalTime, unsigned hour);
	void onEveryDayChanges(GlobalTime* globalTime, unsigned day);
	void onEveryWeekChanges(GlobalTime* globalTime, unsigned week);

private:
	cocos2d::Label* m_Saving = nullptr;
	cocos2d::Label* m_WeekCount = nullptr;
	cocos2d::Label* m_WeekDay = nullptr;
	cocos2d::Label* m_TimeHourDisplay = nullptr;
	cocos2d::Label* m_TimeMinDisplay = nullptr;
	Bank* m_Bank = nullptr;
	MouseOverMenuItem* m_BankButton = nullptr;

	cocos2d::Vec2 m_SceneMidPoint = cocos2d::Vec2::ZERO;
	std::array<std::string, 7> m_WeekDays = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY" };

	void checkBalanceCallback(cocos2d::Ref* pSender, GameScene* scene);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
	void onCurrentCashChange(Player* player, int currentCashAmout);

};
