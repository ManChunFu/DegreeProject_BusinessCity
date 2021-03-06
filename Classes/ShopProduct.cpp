#include "ShopProduct.h"


constexpr unsigned int stringToInt(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (stringToInt(str, h + 1) * 33) ^ str[h];
}

ShopProduct::ShopProduct(rapidjson::Value& json)
{
	for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		std::string propertyName = itr->name.GetString();

		switch (stringToInt(propertyName.c_str()))
		{
		case stringToInt("ProductId"):
			m_ProductId = itr->value.GetInt();
			break;
		case stringToInt("Name"):
			m_Name = itr->value.GetString();
			break;
		case stringToInt("ProductLook"):
			m_ProductSpritePath = itr->value.GetString();
			break;
		case stringToInt("Price"):
			m_SalePrice = itr->value.GetInt();
			m_CurrentPrice = m_SalePrice;
			break;
		case stringToInt("PurchasePrice"):
			m_PurchasePrice = itr->value.GetInt();
			break;
		case stringToInt("Quantity"):
			m_Quantity = itr->value.GetInt();
			break;
		}
	}
}

void ShopProduct::setProductQuantity(unsigned quantity)
{
	m_Quantity = quantity;
}

void ShopProduct::increaseProductQuantity(unsigned amout)
{
	m_Quantity += amout;
}

void ShopProduct::setCurrentSalePrice(unsigned price)
{
	m_CurrentPrice = price;
}

