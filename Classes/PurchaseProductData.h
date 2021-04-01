#pragma once

#include "cocos2d.h"

struct PurchaseProductData
{
	PurchaseProductData(unsigned id, cocos2d::Label* label, unsigned price, unsigned count) : 
		productId(id), productCountText(label), productSalePrice(price), purchaseCount(count) {};
	virtual~PurchaseProductData() { productCountText = nullptr; }

	unsigned productId = 0;
	unsigned productSalePrice = 0;
	cocos2d::Label* productCountText = nullptr;
	unsigned purchaseCount = 0;
};

