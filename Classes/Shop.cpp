#include "Shop.h"
#include "ShopProduct.h"

constexpr unsigned int StringToInt(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (StringToInt(str, h + 1) * 33) ^ str[h];
}


Shop::Shop(rapidjson::Value& json)
{
	for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		std::string propertyName = itr->name.GetString();

		switch (StringToInt(propertyName.c_str()))
		{
		case StringToInt("Type"):
			m_ShopType = itr->value.GetString();
			break;
		case StringToInt("Name"):
			m_Name = itr->value.GetString();
			break;
		case StringToInt("ShopLook"):
			m_ShopLook = itr->value.GetString();
			break;
		case StringToInt("ShopPrice"):
			m_ShopPrice = itr->value.GetInt();
			break;
		case StringToInt("ElectricityCost"):
			m_Electricity = itr->value.GetInt();
			break;
		case StringToInt("WaterCost"):
			m_Water = itr->value.GetInt();
			break;
		case StringToInt("Employees"):
			m_Employees = itr->value.GetInt();
			break;
		case StringToInt("TotalSalary"):
			m_TotalSalaryExpense = itr->value.GetInt();
			break;
		case StringToInt("CommericalCost"):
			m_CommericalCost = itr->value.GetInt();
			break;
		case StringToInt("Products"):
			if (itr->value.IsArray())
			{
				auto products = itr->value.GetArray();

				for (rapidjson::Value::ConstValueIterator productItr = products.Begin(); productItr != products.End(); ++productItr)
				{
					auto productName = (*productItr)["Name"].GetString();
					auto productPrice = (*productItr)["Price"].GetInt();
					m_Products.push_back(new ShopProduct(productName, productPrice));
				}
			}
			break;
		
		}


	}

}

