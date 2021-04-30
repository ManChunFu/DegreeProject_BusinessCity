#pragma once

#include "cocos2d.h"

struct SceneLocationData
{
	SceneLocationData(unsigned sceneId, cocos2d::Vec2 shopLocation, cocos2d::Vec2 employeeLocation = cocos2d::Vec2::ZERO, 
		cocos2d::Vec2 mapIconLocation = cocos2d::Vec2::ZERO, cocos2d::Vec2 moneyIconLocation = cocos2d::Vec2::ZERO);;
	virtual ~SceneLocationData();

	unsigned m_SceneId = 0;
	cocos2d::Vec2 m_ShopLocation = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 m_MapIconLocation = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 m_MoneyIconLocation = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 m_EmployeeLocation = cocos2d::Vec2::ZERO;

};
