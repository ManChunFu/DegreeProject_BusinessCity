#pragma once

#include "cocos2d.h"

enum ownerTypes
{
	player,
	opponent,
	other
};


class Shop
{
public:
	~Shop() {};

	std::string m_ShopType;
	std::string m_Name;
	ownerTypes m_Owner;
	const std::string& m_ProductSpritePath;
	std::string m_ProductName;
	unsigned m_ProductSalePrice;
	unsigned m_PurchasePrice;
	unsigned m_Electricity;
	unsigned m_Water;
	unsigned m_Employees;
	unsigned m_TotalSalaryExpense;
	unsigned m_CommericalCost;

};
