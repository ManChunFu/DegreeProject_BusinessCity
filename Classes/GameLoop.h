#pragma once

#include "cocos2d.h"
#include "2d/CCNode.h"

class GlobalTime;
class Player;
class Shop;

class GameLoop : public cocos2d::Node
{
public:
	GameLoop();
	virtual ~GameLoop();

protected:
	void onEveryMinuteChanges(GlobalTime* globalTime, unsigned minute);
private:
	Player* m_Player = nullptr;
	

};
