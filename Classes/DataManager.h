#pragma once

#include "cocos2d.h"

class Shop;

struct DataManager
{
	static std::map<unsigned, Shop*> getShops();
};
