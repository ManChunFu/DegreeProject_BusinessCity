#include "Shop.h"
#include "ShopProduct.h"
#include "Player.h"

USING_NS_CC;

constexpr unsigned int stringToInt(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (stringToInt(str, h + 1) * 33) ^ str[h];
}

Shop::Shop(rapidjson::Value& json)
{
	for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		std::string propertyName = itr->name.GetString();

		switch (stringToInt(propertyName.c_str()))
		{
		case stringToInt("ShopId"):
			m_ShopId = itr->value.GetInt();
			break;
		case stringToInt("Startup"):
			m_Startup = itr->value.GetBool();
			break;
		case stringToInt("Type"):
			m_ShopType = itr->value.GetString();
			break;
		case stringToInt("Name"):
			m_Name = itr->value.GetString();
			break;
		case stringToInt("ShopLookNormal"):
			m_ShopLook_Normal = itr->value.GetString();
			break;
		case stringToInt("ShopLookLit"):
			m_ShopLook_Lit = itr->value.GetString();
			break;
		case stringToInt("ShopLookDisabled"):
			m_ShopLook_Disabled = itr->value.GetString();
			break;
		case stringToInt("ShopPrice"):
			m_ShopPrice = itr->value.GetInt();
			break;
		case stringToInt("ElectricityCost"):
			m_Electricity = itr->value.GetInt();
			break;
		case stringToInt("WaterCost"):
			m_Water = itr->value.GetInt();
			break;
		case stringToInt("Employees"):
			m_Employees = itr->value.GetInt();
			break;
		case stringToInt("TotalSalary"):
			m_TotalSalaryExpense = itr->value.GetInt();
			break;
		case stringToInt("CommercialCost"):
			m_CommercialCost = itr->value.GetInt();
			break;
		case stringToInt("Products"):
			if (itr->value.IsArray())
			{
				auto products = itr->value.GetArray();
				for (int index = 0; index < itr->value.Size(); index++)
				{
					ShopProduct* shopProduct = new ShopProduct((rapidjson::Value&)(products[index]));
					m_Products.push_back(shopProduct);
				}
			}
			break;
		}
	}

}

Shop::~Shop()
{
	for (auto item : m_Products)
	{
		delete item;
	}
	m_Products.clear();
}

bool Shop::isShopOpen(unsigned day, unsigned currentHour)
{
	if (m_IsReplenishing)
	{
		ReplenishmentCountDown();
		return false;
	}

	if (currentHour < m_ShopOpenHour.first || currentHour > m_ShopOpenHour.second)
		return false;

	return m_ShopOpenDay[day];
}

unsigned Shop::runTrade(unsigned day, Shop* shop)
{
	auto randNo = random(0, 100);

	if (randNo < getSucessProbability(day))
		return 0;

	// run trade quantity
	auto tradeQuantity = random(1, 4);
	auto productTypes = (int)m_Products.size() - 1;
	auto tradeProduct = random(0, productTypes);
	if (tradeQuantity < m_Products[tradeProduct]->m_Quantity)
	{
		m_Products[tradeProduct]->m_Quantity -= tradeQuantity;
		if (onQuantityChanges)
			onQuantityChanges(this, m_Products[tradeProduct]->m_ProductId, m_Products[tradeProduct]->m_Quantity);
	}

	// sales income
	return m_Products[tradeProduct]->m_SalePrice * tradeQuantity;
}

unsigned Shop::getSucessProbability(unsigned day)
{
	return m_SuccessProbabilityDaily[day];
}

void Shop::ReplenishmentCountDown()
{
	m_ReplenishingCountDown--;
	if (onCountdownChanges)
		onCountdownChanges(this, m_ReplenishingCountDown);

	if (m_ReplenishingCountDown == 0)
	{
		m_IsReplenishing = false;
		m_ReplenishingCountDown = 30;
	}
}


