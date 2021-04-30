#include "SceneLocationData.h"

USING_NS_CC;

SceneLocationData::SceneLocationData(unsigned sceneId, Vec2 shopLocation, Vec2 employeeLocation, Vec2 mapIconLocation, Vec2 moneyIconLocation)
{
	m_SceneId = sceneId;
	m_ShopLocation = shopLocation;
	m_EmployeeLocation = employeeLocation;
	m_MapIconLocation = mapIconLocation;
	m_MoneyIconLocation = moneyIconLocation;
}

SceneLocationData::~SceneLocationData()
{
}
