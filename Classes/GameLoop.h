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
	unsigned m_SuccessProbabilityDaily[7] = { 30, 30, 30, 30, 40, 60, 70 };

	bool isShopOPen();
};
