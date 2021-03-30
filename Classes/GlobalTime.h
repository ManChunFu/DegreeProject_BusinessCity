#pragma once

#include "cocos2d.h"

struct GameTime;

class GlobalTime
{
public:
	GlobalTime();
	virtual ~GlobalTime();

	void update(float delta);
	

	typedef std::function<void(GlobalTime*, unsigned)> onEveryMinuteChanges;
	std::function<void(GlobalTime* globalTime, unsigned hour)> onEveryHourChanges;
	std::function<void(GlobalTime* globalTime, unsigned day)> onEveryDayChanges;
	std::function<void(GlobalTime* globalTime, unsigned hour)> onEveryWeekChanges;

	void addMinuteEventListener(const onEveryMinuteChanges& changes);

	GameTime* m_Gametime = nullptr;
protected:
	float m_ElapsedTime = 0.f;
	onEveryMinuteChanges _onEveryMinuteChanges;
	std::vector<onEveryMinuteChanges> Listeners;
};

