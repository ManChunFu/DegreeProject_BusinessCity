#pragma once

#include "cocos2d.h"
#include <cocos/base/ccTypes.h>

class GlobalTime;
class Player;

class GameLoop
{
public:
	GameLoop();
	virtual ~GameLoop();

protected:
	void onEveryMinuteChanges(GlobalTime* globalTime, unsigned minute);

private:
	Player* m_Player = nullptr;
	int m_MaxTradePerPerson = 4;
	unsigned m_SuccessProbabilityDaily[7] = { 30, 30, 30, 30, 40, 60, 70 };

	bool isShopOpen(unsigned shopId, unsigned day, unsigned currentHour);
	unsigned getSucessProbability(unsigned day);
	unsigned runTradeQuantity(unsigned day);
	void executeTrade(unsigned qauntity);
};
