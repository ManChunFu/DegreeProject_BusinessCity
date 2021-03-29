#pragma once

#include "cocos2d.h"

struct GlobalTime
{
	virtual ~GlobalTime() {};

	//static void update(float delta);
	//static unsigned getCurrentMinute() { return m_CurrentMinute; }
	//static unsigned getCurrentHour() { return m_CurrentHour; }
	//static unsigned getCurrentDay() { return m_Today; }
	//static unsigned getCurrentWeek() { return m_Weeks; }

protected:
	float m_ElapsedTime = 0.f;
	unsigned m_CurrentMinute = 0;
	unsigned m_CurrentHour = 8;
	unsigned m_Today = 0;
	std::string m_WeekDays[7] = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY" };
	unsigned m_Weeks = 1;
};

