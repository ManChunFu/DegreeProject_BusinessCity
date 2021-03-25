#pragma once
#include "cocos2d.h"
#include "json/Document.h"


class ShopProduct
{
public:
	ShopProduct(rapidjson::Value& json);
	~ShopProduct() {};

	unsigned m_ProductId = 0;
	std::string m_Name = "";
	std::string m_ProductSpritePath = "";
	unsigned m_PurchasePrice = 0;
	unsigned m_SalePrice = 0;
	unsigned m_discountPrice = 0;
	unsigned m_Quantity = 0;

};
