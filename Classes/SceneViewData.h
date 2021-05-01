#pragma once

#include "cocos2d.h"

struct SceneLocationData;
struct SceneViewData
{
	SceneViewData();
	virtual ~SceneViewData();

	std::string m_MoneyIcon = "$.png";

	std::map<unsigned int, SceneLocationData*> m_SceneLocations;

};
