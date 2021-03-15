#pragma once

#include "cocos2d.h"
#include "json/Document.h"

enum ownerTypes
{
	player,
	opponent,
	other
};

class ShopProduct;

class Shop
{
public:
	Shop(rapidjson::Value& json);
	Shop() {};
	virtual ~Shop() {};

	std::string m_ShopType;
	std::string m_Name;
	ownerTypes m_Owner;
	std::string m_ShopLook_Normal;
	std::string m_ShopLook_Lit;
	std::string m_ShopLook_Disabled;
	bool m_Startup;
	unsigned m_ShopId;
	unsigned m_ShopPrice;
	unsigned m_Electricity;
	unsigned m_Water;
	unsigned m_Employees;
	unsigned m_TotalSalaryExpense;
	unsigned m_CommericalCost;

	std::vector<ShopProduct*> m_Products;

};
