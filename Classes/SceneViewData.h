#pragma once

#include "cocos2d.h"

struct SceneLocationData;
struct SceneViewData
{
	SceneViewData();
	virtual ~SceneViewData();

	std::array<std::string, 3> m_SceneViewPaths = { "GameSceneCityView_Main1300.png",  "Hotel_View.png", "Playground_View1.png" };
	std::array<std::string, 3> m_MapIconPaths = { "IconMap_Small.png", "IconMap_Small_Lit.png", "IconMap_Small.png" };
	std::string m_MoneyIcon = "$.png";

	std::map<unsigned int, SceneLocationData*> m_SceneLocations;

};
