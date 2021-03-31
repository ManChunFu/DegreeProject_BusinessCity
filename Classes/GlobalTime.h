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
	void addMinuteEventListener(const onEveryMinuteChanges& changes);

	onEveryMinuteChanges onEveryHourChanges;
	onEveryMinuteChanges onEveryDayChanges;
	onEveryMinuteChanges onEveryWeekChanges;

	GameTime* m_Gametime = nullptr;
protected:
	float m_ElapsedTime = 0.f;
	std::vector<onEveryMinuteChanges> Listeners;
};

