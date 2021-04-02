#pragma once

#include "cocos2d.h"

struct PurchaseProductData
{
	PurchaseProductData(unsigned id, cocos2d::Label* priceLabel, cocos2d::Label* qauntityLabel, unsigned price, unsigned count) : 
		productId(id), productPriceText(priceLabel), productCountText(qauntityLabel), productSalePrice(price), purchaseCount(count) {};
	
	virtual~PurchaseProductData() { productPriceText = nullptr; productCountText = nullptr; }

	unsigned productId = 0;
	unsigned productSalePrice = 0;
	cocos2d::Label* productPriceText = nullptr;
	cocos2d::Label* productCountText = nullptr;
	unsigned purchaseCount = 0;
};

