#include "BasicExpenseData.h"

USING_NS_CC;

BasicExpenseData::BasicExpenseData(unsigned eletricityCost, unsigned waterCost, unsigned salaryCost, unsigned ADCost, unsigned salesIncome)
{
	m_EletricityCost = eletricityCost;
	m_WaterCost = waterCost;
	m_SalaryCost = salaryCost;
	m_ADCost = ADCost;
	m_SalesIncome = salesIncome;
}

BasicExpenseData::~BasicExpenseData()
{
	m_ElLabel = nullptr;
	m_WaterLabel = nullptr;
	m_SalaryLabel = nullptr;
	m_ADLabel = nullptr;
	m_SalesLabel = nullptr;
	m_TotalLabel = nullptr;
}

int BasicExpenseData::getBalance()
{
	return m_SalesIncome - getTotalCost();
}

unsigned BasicExpenseData::getTotalCost()
{
	return m_EletricityCost + m_WaterCost + m_SalaryCost + m_ADCost;
}
