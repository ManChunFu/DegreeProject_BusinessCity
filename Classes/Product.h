#pragma once
#include "cocos2d.h"

class Product
{
public:
	std::string m_Name;
	const std::string& m_ProductSpritePath;
	unsigned m_PurchasePrice;
	unsigned m_SalePrice;
	unsigned m_discountPrice;
	unsigned m_Quantity;

};
