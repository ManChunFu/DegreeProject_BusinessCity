#pragma once

#include "cocos2d.h"
#include "json/Document.h"


struct PeopleLocationData
{
	PeopleLocationData(rapidjson::Value& json);
	virtual ~PeopleLocationData();

	unsigned m_ShopId = 0;
	cocos2d::Vec2 m_PeopleLocation = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 m_ProductLocation = cocos2d::Vec2::ZERO;

	std::vector<std::string> m_PeopleSpritePaths;
};