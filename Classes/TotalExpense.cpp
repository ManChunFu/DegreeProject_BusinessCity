#include "TotalExpense.h"
#include "BasicExpenseData.h"

TotalExpense::~TotalExpense()
{
	for (auto shopExpenseData : m_ShopsBasicExpense)
	{
		delete shopExpenseData;
	}
	m_ShopsBasicExpense.clear();
}

unsigned TotalExpense::getTotalElCosts()
{
	auto amout = 0;
	for (auto shopExpenseData : m_ShopsBasicExpense)
	{
		amout += shopExpenseData->m_EletricityCost;
	}

	return amout;
}

unsigned TotalExpense::getTotalWaterCosts()
{
	auto amout = 0;
	for (auto shopExpenseData : m_ShopsBasicExpense)
	{
		amout += shopExpenseData->m_WaterCost;
	}

	return amout;
}

unsigned TotalExpense::getTotalSalaryCosts()
{
	auto amout = 0;
	for (auto shopExpenseData : m_ShopsBasicExpense)
	{
		amout += shopExpenseData->m_SalaryCost;
	}

	return amout;
}

unsigned TotalExpense::getCommercialCosts()
{
	auto amout = 0;
	for (auto shopExpenseData : m_ShopsBasicExpense)
	{
		amout += shopExpenseData->m_ADCost;
	}

	return amout;
}

unsigned TotalExpense::getTotalSalesIncome()
{
	auto amout = 0;
	for (auto shopExpenseData : m_ShopsBasicExpense)
	{
		amout += shopExpenseData->m_SalesIncome;
	}

	return amout;
}

unsigned TotalExpense::getTotalCosts()
{
	auto amout = 0;
	for (auto shopExpenseData : m_ShopsBasicExpense)
	{
		amout += shopExpenseData->getTotalCost();
	}

	return amout;
}

int TotalExpense::getTotalBalance()
{
	auto amout = 0;
	for (auto shopExpenseData : m_ShopsBasicExpense)
	{
		amout += shopExpenseData->getBalance();
	}

	return amout;
}
