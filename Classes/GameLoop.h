#pragma once

#include "cocos2d.h"
#include <cocos/base/ccTypes.h>

class GlobalTime;
class Player;
class Shop;

class GameLoop
{
public:
	GameLoop();
	virtual ~GameLoop();

protected:
	void onEveryMinuteChanges(GlobalTime* globalTime, unsigned minute);
	void onEveryHourChanges(GlobalTime* globalTime, unsigned hour);
private:
	Player* m_Player = nullptr;
	

};
