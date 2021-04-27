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
struct ShopUI;

class Shop
{
public:
	Shop(rapidjson::Value& json);
	virtual ~Shop();

	bool isShopOpen(unsigned day, unsigned currentHour);
	unsigned runTrade(unsigned day, Shop* shop);
	bool isReplenishing() { return m_IsReplenishing; }
	void setRunForErrand();
	unsigned getCountDown() { return m_ReplenishingCountDown; }

	void setPlayerWorkHere();
	unsigned int isAnyoneAtStore();
	unsigned getEmployeeCount() { return m_Employees; }
	void addEmployee(unsigned person, unsigned id);
	void fireEmployee(unsigned person, unsigned id);
	bool isMyEmployee(unsigned id);
	unsigned getTotalSalaryExpense() { return m_Employees * m_SalaryPerEmployee; }

	void setShopOpenDay(unsigned weekday);
	/* from = first, to = second */
	void setShopOpenHour(unsigned weekday, unsigned fromOrTo, unsigned workingHour);

	std::function<void(unsigned shopId, unsigned productId, unsigned saleQuantity)> onSaleHappens;
	std::function<void(unsigned productId, unsigned remainQuantity)> onQuantityChanges;
	std::function<void(unsigned countdown)> onCountdownChanges;
	std::function<void(unsigned shopId, unsigned totalSales)> onSalesIncomeChanges;
#pragma region ShopData
	std::string m_ShopType = "";
	std::string m_Name = "";
	ownerTypes m_Owner = ownerTypes::player;
	std::string m_ShopInSceneSmall = "";
	std::string m_ShopInSceneBig = "";
	std::string m_ShopLook_Normal = "";
	std::string m_ShopLook_Lit = "";
	std::string m_ShopLook_Disabled = "";
	bool m_Startup = false;
	unsigned m_ShopId = 0;
	unsigned m_ShopPrice = 0;
	unsigned m_Electricity = 0;
	unsigned m_Water = 0;
	unsigned m_Employees = 0;
	unsigned m_RequiredEmployees = 0;
	unsigned m_SalaryPerEmployee = 0;
	unsigned m_StaffRunForErrandCount = 0;
	unsigned m_CommercialCost = 0;
	unsigned m_SalesIncome = 0;
	std::vector<unsigned int> m_UpgradeableTo;

	// shop working -> monday to sunday
	std::array<unsigned int, 7> m_SuccessProbabilityDaily = { 30, 30, 30, 30, 40, 50, 60 };
	std::array<bool, 7> m_ShopOpenDay = { true, true, true, true, true, false, false };
	//std::array<bool, 7> m_PlayerWorkHere = { true, true, true, true, true, false, false };
	// shop working hour -> from and  to
	std::array<std::pair<unsigned int, unsigned int>, 7> m_ShopOpenHour = {
		std::make_pair(9, 17), std::make_pair(9, 17), std::make_pair(9, 17), std::make_pair(9, 17), std::make_pair(9, 17),
		std::make_pair(14, 20), std::make_pair(14, 19)};

	std::vector<unsigned int> m_EmployeesIds;
	bool m_PlayerWorkHere = true;
	bool m_IsReplenishing = false;
	unsigned m_ReplenishingCountDown = 30;
#pragma endregion

#pragma region ShopProducts
	unsigned getProductQuantity(unsigned productId);
	void setShopProductQuantity(unsigned productId, unsigned quantity);
	void increaseProductQuantity(unsigned productId, unsigned amout);

	unsigned getProductPurchasePrice(unsigned productId);
	unsigned getProductSalePrice(unsigned productId);
	unsigned getCurrentSalePrice(unsigned productId);
	void setCurrentSalePrice(unsigned productId, unsigned price);

	unsigned getProductId(unsigned shopId);
	std::string getProductName(unsigned productId);
	std::string getProductSprite(unsigned productId);

	unsigned getProductsSize() { return m_Products.size(); }
#pragma endregion

protected:
	std::vector<ShopProduct*> m_Products;

private:
	int m_MaxTradePerPerson = 4;

	unsigned getSucessProbability(ShopProduct* product, unsigned day);
	void ReplenishmentCountDown();
};
