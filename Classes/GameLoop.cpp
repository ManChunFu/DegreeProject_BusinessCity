#include "GameLoop.h"
#include "GameData.h"
#include "GlobalTime.h"


GameLoop::GameLoop()
{
	GameData::getInstance().m_GlobalTime->onEveryMinuteChanges = CC_CALLBACK_2(GameLoop::onEveryMinuteChanges, this);
}

void GameLoop::onEveryMinuteChanges(GlobalTime* globalTime, unsigned minute)
{
}
