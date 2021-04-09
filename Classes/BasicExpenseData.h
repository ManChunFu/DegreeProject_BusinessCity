#pragma once

#include "cocos2d.h"

struct BasicExpenseData
{
	BasicExpenseData(unsigned shopId, unsigned eletricityCost, unsigned waterCost, unsigned salaryCost, unsigned ADCost, unsigned salesIncome);

	virtual ~BasicExpenseData();

	unsigned m_ShopId = 0;
	unsigned m_EletricityCost = 0;
	unsigned m_WaterCost = 0;
	unsigned m_SalaryCost = 0;
	unsigned m_ADCost = 0;
	unsigned m_SalesIncome = 0;

	cocos2d::Label* m_ElLabel = nullptr;
	cocos2d::Label* m_WaterLabel = nullptr;
	cocos2d::Label* m_SalaryLabel = nullptr;
	cocos2d::Label* m_ADLabel = nullptr;
	cocos2d::Label* m_SalesLabel = nullptr;
	cocos2d::Label* m_TotalLabel = nullptr;

	unsigned getTotalCost();
	int getBalance();
};
