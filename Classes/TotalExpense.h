#pragma once

#include "cocos2d.h"

struct BasicExpenseData;

struct TotalExpense
{
	TotalExpense() {};
	virtual ~TotalExpense();

	std::vector<BasicExpenseData*> m_ShopsBasicExpense;

	unsigned getTotalElCosts();
	unsigned getTotalWaterCosts();
	unsigned getTotalSalaryCosts();
	unsigned getCommercialCosts();
	unsigned getTotalSalesIncome();
	unsigned getTotalCosts();
	int getTotalBalance();
};
