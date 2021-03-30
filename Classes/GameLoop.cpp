#include "GameLoop.h"
#include "GameData.h"
#include "GlobalTime.h"


GameLoop::GameLoop()
{
	GameData::getInstance().m_GlobalTime->addMinuteEventListener(CC_CALLBACK_2(GameLoop::onEveryMinuteChanges, this));
}

void GameLoop::onEveryMinuteChanges(GlobalTime* globalTime, unsigned minute)
{
}


bool GameLoop::isShopOPen()
{
	return false;
}
