#pragma once

#include "cocos2d.h"

class Shop;
struct PeopleData;

struct DataManager
{
	static std::map<unsigned, Shop*> getShops();
	static std::map<unsigned, PeopleData*> getPeople();

};
