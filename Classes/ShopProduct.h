#pragma once
#include "cocos2d.h"
#include "json/Document.h"


class ShopProduct
{
public:
	ShopProduct(const std::string& name, int price) : m_Name(name), m_SalePrice(price) {};
	ShopProduct(rapidjson::Value json);
	~ShopProduct() {};

	std::string m_Name;
	std::string m_ProductSpritePath;
	unsigned m_PurchasePrice;
	unsigned m_SalePrice;
	unsigned m_discountPrice;
	unsigned m_Quantity;

};
