#pragma once

#include "cocos2d.h"

class Shop;

struct DataManager
{
	static std::vector<Shop*> getShops();
};
