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
	virtual ~Shop();

	bool isShopOpen(unsigned day, unsigned currentHour);
	unsigned runTrade(unsigned day, Shop* shop);
	bool isReplenishing() { return m_IsReplenishing; }
	unsigned getCountDown() { return m_ReplenishingCountDown; }

	unsigned getProductQuantity(unsigned productId);
	void setShopProductQuantity(unsigned productId, unsigned quantity);
	void increaseProductQuantity(unsigned productId, unsigned amout);

	unsigned getProductPurchasePrice(unsigned productId);
	unsigned getProductSalePrice(unsigned productId);
	unsigned getCurrentSalePrice(unsigned productId);
	void setCurrentSalePrice(unsigned productId, unsigned price);

	unsigned getProductId(unsigned productId);
	std::string getProductName(unsigned productId);
	std::string getProductSprite(unsigned productId);

	unsigned getProductsSize() { return m_Products.size(); }

	void setPlayerWorkHere();

	void setShopOpenDay(unsigned weekday);
	/* from = first, to = second */
	void setShopOpenHour(unsigned fromOrTo, unsigned workingHour);


	std::function<void(Shop* shop, unsigned productId, unsigned remainQuantity)> onQuantityChanges;
	std::function<void(Shop* shop, unsigned countdown)> onCountdownChanges;

#pragma region ShopData
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

	// shop working -> monday to sunday
	std::array<unsigned int, 7> m_SuccessProbabilityDaily = { 20, 20, 20, 20, 30, 50, 60 };
	std::array<bool, 7> m_ShopOpenDay = { true, true, true, true, true, false, false };

	// shop working hour -> from and  to
	std::pair<unsigned int, unsigned int> m_ShopOpenHour = std::make_pair( 9, 17 );

	bool m_PlayerWorkHere = true;
	bool m_IsReplenishing = false;
	unsigned m_ReplenishingCountDown = 30;
#pragma endregion
protected:
	std::vector<ShopProduct*> m_Products;

private:
	int m_MaxTradePerPerson = 4;

	unsigned getSucessProbability(unsigned day);
	void ReplenishmentCountDown();
};
