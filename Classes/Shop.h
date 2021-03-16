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
	bool m_Startup = false;
	unsigned m_ShopId = 0;
	unsigned m_ShopPrice = 0;
	unsigned m_Electricity = 0;
	unsigned m_Water = 0;
	unsigned m_Employees = 0;
	unsigned m_TotalSalaryExpense = 0;
	unsigned m_CommercialCost = 0;

	std::vector<ShopProduct*> m_Products;

};