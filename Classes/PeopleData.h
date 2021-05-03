#pragma once

#include "cocos2d.h"
#include "json/Document.h"

struct PeopleLocationData;

struct PeopleData
{
	PeopleData(rapidjson::Value& json);
	virtual ~PeopleData();

	unsigned m_SceneId = 0;

	std::map<unsigned, PeopleLocationData*> m_PeopleLocations;
};
