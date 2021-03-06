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
		case stringToInt("ShopInSceneId"):
			m_ShopInSceneId = itr->value.GetInt();
			break;
		case stringToInt("ShopSceneLocation"):
			if (itr->value.IsArray())
			{
				auto location = itr->value.GetArray();
				m_ShopSceneLocation.x = location[0].GetFloat();
				m_ShopSceneLocation.y = location[1].GetFloat();
			}
			break;
		case stringToInt("ShopMainSceneLocation"):
			if (itr->value.IsArray())
			{
				auto location = itr->value.GetArray();
				m_ShopMainSceneLocation.x = location[0].GetFloat();
				m_ShopMainSceneLocation.y = location[1].GetFloat();
			}
			break;
		case stringToInt("ShopInSceneSmall"):
			m_ShopInSceneSmall = itr->value.GetString();
			break;
		case stringToInt("ShopInSceneBig"):
			m_ShopInSceneBig = itr->value.GetString();
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
		case stringToInt("EmployeeLocation"):
			if (itr->value.IsArray())
			{
				auto location = itr->value.GetArray();
				m_EmployeeLocation.x = location[0].GetFloat();
				m_EmployeeLocation.y = location[1].GetFloat();
			}
			break;
		case stringToInt("RequiredEmployees"):
			m_RequiredEmployees = itr->value.GetInt();
			break;
		case stringToInt("SalaryPerEmployee"):
			m_SalaryPerEmployee = itr->value.GetInt();
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
		case stringToInt("UpgradableTo"):
			if (itr->value.IsArray())
			{
				auto upgradable = itr->value.GetArray();
				for (int index = 0; index < itr->value.Size(); index++)
				{
					m_UpgradeableTo.push_back(upgradable[index].GetInt());
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
	m_UpgradeableTo.clear();
	m_EmployeesIds.clear();
}

bool Shop::isShopOpen(unsigned day, unsigned currentHour)
{
	if (m_IsReplenishing)
	{
		ReplenishmentCountDown();
		return false;
	}

	if (m_Employees == 0 && !m_PlayerWorkHere)
		return false;

	if (m_ShopOpenDay[day])
	{
		if (currentHour < m_ShopOpenHour[day].first || currentHour > m_ShopOpenHour[day].second)
			return false;
		
		return true;
	}

	return false;
}

unsigned Shop::runTrade(unsigned day, Shop* shop)
{
	auto randNo = random(0, 100);

	// random a product
	auto productTypes = (int)m_Products.size() - 1;
	auto tradeProduct = random(0, productTypes);

	// check if it is succeed to sale
	if (getSucessProbability(m_Products[tradeProduct], day) < randNo)
		return 0;

	// run random trade quantity
	auto tradeQuantity = random(1, 4);
	
	// make sure the shop still have enough stock
	if (tradeQuantity < m_Products[tradeProduct]->getProductQuantity())
	{
		m_SalesIncome = 0;

		if (onSaleHappens)
			onSaleHappens(m_ShopInSceneId, m_ShopId, m_Products[tradeProduct]->getProductId());

		// update shop product qty
		m_Products[tradeProduct]->increaseProductQuantity(-tradeQuantity);
		if (onQuantityChanges)
			onQuantityChanges(m_Products[tradeProduct]->getProductId(), m_Products[tradeProduct]->getProductQuantity());

		// sales income
		m_SalesIncome += m_Products[tradeProduct]->getCurrentSalePrice() * tradeQuantity;
		if (onSalesIncomeChanges)
			onSalesIncomeChanges(m_ShopId, m_SalesIncome);

		return m_SalesIncome;
	}

	return 0;
}

void Shop::setRunForErrand()
{
	if (m_IsReplenishing)
		return;

	m_StaffRunForErrandCount++;
	if (isAnyoneAtStore() == m_StaffRunForErrandCount)
		m_IsReplenishing = true;
}

unsigned Shop::getSucessProbability(ShopProduct* product, unsigned day)
{
	auto difference = product->getCurrentSalePrice() / product->getSalePrice();

	return m_SuccessProbabilityDaily[day] * difference;
}

unsigned Shop::getProductQuantity(unsigned productId)
{
	return m_Products[productId]->getProductQuantity();
}

void Shop::setShopProductQuantity(unsigned productId, unsigned quantity)
{
	m_Products[productId]->setProductQuantity(quantity);
}

void Shop::increaseProductQuantity(unsigned productId, unsigned amout)
{
	m_Products[productId]->increaseProductQuantity(amout);
}

unsigned Shop::getProductPurchasePrice(unsigned productId)
{
	return m_Products[productId]->getProductPurchasePrice();
}

unsigned Shop::getProductSalePrice(unsigned productId)
{
	return m_Products[productId]->getSalePrice();
}

unsigned Shop::getCurrentSalePrice(unsigned productId)
{
	return m_Products[productId]->getCurrentSalePrice();
}

void Shop::setCurrentSalePrice(unsigned productId, unsigned price)
{
	m_Products[productId]->setCurrentSalePrice(price);
}

unsigned Shop::getProductId(unsigned shopId)
{
	return m_Products[shopId]->getProductId();
}

std::string Shop::getProductName(unsigned productId)
{
	return m_Products[productId]->getProductName();
}

std::string Shop::getProductSprite(unsigned productId)
{
	return m_Products[productId]->getProductSprite();
}

void Shop::setShopOpenDay(unsigned weekday)
{
	auto originSetting = m_ShopOpenDay[weekday];
	m_ShopOpenDay[weekday] = !originSetting;
}

void Shop::setPlayerWorkHere()
{
	auto originSetting = m_PlayerWorkHere;
	m_PlayerWorkHere = !originSetting;
}

unsigned int Shop::isAnyoneAtStore()
{
	auto staffs = m_Employees;
	if (m_PlayerWorkHere)
		staffs += 1;

	return staffs;
}

void Shop::addEmployee(unsigned person, unsigned id)
{
	m_Employees += person;
	m_EmployeesIds.push_back(id);
}

void Shop::fireEmployee(unsigned person, unsigned id)
{
	m_Employees -= person;
	for (unsigned index = 0; index < m_EmployeesIds.size(); ++index)
	{
		if (m_EmployeesIds[index] == id)
		{
			m_EmployeesIds.erase(m_EmployeesIds.begin() + index);
			return;
		}
	}
}

bool Shop::isMyEmployee(unsigned id)
{
	for (auto ID : m_EmployeesIds)
	{
		if (ID == id)
			return true;
	}
	return false;
}

void Shop::setShopOpenHour(unsigned weekday, unsigned fromOrTo, unsigned workingHour)
{
	(fromOrTo == 0) ? m_ShopOpenHour[weekday].first = workingHour : m_ShopOpenHour[weekday].second = workingHour;
}

void Shop::ReplenishmentCountDown()
{
	m_ReplenishingCountDown--;

	if (m_ReplenishingCountDown == 0)
	{
		m_IsReplenishing = false;
		if (onCountdownChanges)
			onCountdownChanges(m_ReplenishingCountDown);

		m_ReplenishingCountDown = 30;
		m_StaffRunForErrandCount = 0;

		return;
	}
	
	if (onCountdownChanges)
		onCountdownChanges(m_ReplenishingCountDown);
}


