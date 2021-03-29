#pragma once

#include "cocos2d.h"

struct GameTime;

class GlobalTime
{
public:
	GlobalTime();
	virtual ~GlobalTime();

	void update(float delta);
	
	std::function<void(GlobalTime* globalTime, unsigned minute)> onEveryMinuteChanges;
	std::function<void(GlobalTime* globalTime, unsigned hour)> onEveryHourChanges;
	std::function<void(GlobalTime* globalTime, unsigned day)> onEveryDayChanges;
	std::function<void(GlobalTime* globalTime, unsigned hour)> onEveryWeekChanges;

	GameTime* m_Gametime = nullptr;
protected:
	float m_ElapsedTime = 0.f;
	unsigned m_CurrentMinute = 0;
	unsigned m_CurrentHour = 8;
	unsigned m_Today = 0;
	std::string m_WeekDays[7] = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY" };
	unsigned m_Weeks = 1;
};

