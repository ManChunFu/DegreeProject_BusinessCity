#pragma once

#include "cocos2d.h"

class GameLoop
{
public:
	virtual ~GameLoop() {};

	void runGameLoop(bool isRunning);
	
private:
	bool m_IsGameRunning = false;
};
