#include "Shop.h"
#include "ShopProduct.h"

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

