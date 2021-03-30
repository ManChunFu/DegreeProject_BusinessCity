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
	virtual ~Shop();

	bool isShopOpen(unsigned day, unsigned currentHour);
	unsigned runTrade(unsigned day, Shop* shop);
	unsigned getSucessProbability(unsigned day);
	
	std::string m_ShopType = "";
	std::string m_Name = "";
	ownerTypes m_Owner = ownerTypes::player;
	std::string m_ShopLook_Normal = "";
	std::string m_ShopLook_Lit = "";
	std::string m_ShopLook_Disabled = "";
	bool m_Startup = false;
	unsigned m_ShopId = 0;
	unsigned m_ShopPrice = 0;
	unsigned m_Electricity = 0;
	unsigned m_Water = 0;
	unsigned m_Employees = 0;
	unsigned m_TotalSalaryExpense = 0;
	unsigned m_CommercialCost = 0;

	std::vector<ShopProduct*> m_Products;
	
	// shop working -> monday to sunday
	unsigned m_SuccessProbabilityDaily[7] = { 30, 30, 30, 30, 40, 60, 70 };
	bool m_ShopOpenDay[7] = { true, true, true, true, true, false, false };

	// shop working hour -> from and  to
	unsigned int m_ShopOpenHour[2] = { 9, 17 };

private:
	int m_MaxTradePerPerson = 4;
};
