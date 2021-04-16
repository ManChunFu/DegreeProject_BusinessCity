#pragma once

#include "cocos2d.h"

struct GameTime;

class GlobalTime
{
public:
	GlobalTime();
	virtual ~GlobalTime();

	void update(float delta);
	

	typedef std::function<void(GlobalTime*, unsigned)> onTimeChanges;
	
	void addMinuteEventListener(const onTimeChanges& changes);
	void addHourEventListener(const onTimeChanges& changes);


	onTimeChanges onEveryDayChanges;
	onTimeChanges onEveryWeekChanges;

	GameTime* m_Gametime = nullptr;
protected:
	float m_ElapsedTime = 0.f;
	std::vector<onTimeChanges> m_MinuteListeners;
	std::vector<onTimeChanges> m_HourListeners;
};

