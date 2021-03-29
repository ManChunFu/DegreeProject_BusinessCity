#pragma once

#include "cocos2d.h"

class GlobalTime;

class GameLoop
{
public:
	GameLoop();
	virtual ~GameLoop(){};

protected:
	void onEveryMinuteChanges(GlobalTime* globalTime, unsigned minute);

private:
	bool m_IsGameRunning = false;
};
