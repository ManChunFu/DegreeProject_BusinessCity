#pragma once
#include "cocos2d.h"
#include "json/Document.h"


class ShopProduct
{
public:
	ShopProduct(rapidjson::Value& json);
	~ShopProduct() {};

	unsigned getProductQuantity() { return m_Quantity; }
	unsigned getProductPurchasePrice() { return m_PurchasePrice; }
	void setProductQuantity(unsigned quantity);
	void increaseProductQuantity(unsigned amout);

	unsigned getSalePrice() { return m_SalePrice; }
	unsigned getCurrentSalePrice() { return m_CurrentPrice; }
	void setCurrentSalePrice(unsigned price);

	unsigned getProductId() { return m_ProductId; }
	std::string getProductName() { return m_Name; }
	std::string getProductSprite() { return m_ProductSpritePath; }

protected:
	unsigned m_ProductId = 0;
	std::string m_Name = "";
	std::string m_ProductSpritePath = "";
	unsigned m_PurchasePrice = 0;
	unsigned m_SalePrice = 0;
	unsigned m_CurrentPrice = 0;
	unsigned m_Quantity = 0;

};
