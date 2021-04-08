#pragma once

#include "cocos2d.h"

struct ShopUI
{
	ShopUI() {};
	virtual ~ShopUI() {
		m_ElLabel = nullptr; m_WaterLabel = nullptr; m_SalaryLabel = nullptr; m_ADLabel = nullptr; m_SalesLabel = nullptr; 
	}
	
	cocos2d::Label* m_ElLabel = nullptr;
	cocos2d::Label* m_WaterLabel = nullptr;
	cocos2d::Label* m_SalaryLabel = nullptr;
	cocos2d::Label* m_ADLabel = nullptr;
	cocos2d::Label* m_SalesLabel = nullptr;
};
