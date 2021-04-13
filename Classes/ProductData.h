#pragma once

#include "cocos2d.h"

struct ProductData
{
	ProductData(unsigned id, cocos2d::Label* priceLabel, unsigned price, cocos2d::Label* currentQTYLabel, unsigned currentQTY, 
		cocos2d::Label* qauntityLabel, unsigned count) : m_ProductId(id), m_ProductPriceText(priceLabel), m_ProductSalePrice(price), 
		m_CurrentQTYLabel(currentQTYLabel), m_CurrentQTY(currentQTY), m_ProductPurchaseCountText(qauntityLabel), m_PurchaseCount(count) {};
	
	virtual~ProductData() { m_ProductPriceText = nullptr; m_CurrentQTYLabel = nullptr; m_ProductPurchaseCountText = nullptr; }

	unsigned m_ProductId = 0;
	unsigned m_ProductSalePrice = 0;
	unsigned m_CurrentQTY = 0;
	unsigned m_PurchaseCount = 0;

	cocos2d::Label* m_ProductPriceText = nullptr;
	cocos2d::Label* m_CurrentQTYLabel = nullptr;
	cocos2d::Label* m_ProductPurchaseCountText = nullptr;
};

