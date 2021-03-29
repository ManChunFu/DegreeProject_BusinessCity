#pragma once

#include "cocos2d.h"

struct GameTime
{
	GameTime() {};
	virtual ~GameTime() {};

	unsigned minute = 0;
	unsigned hour = 8;
	unsigned day = 0;
	unsigned week = 1;
};
