#pragma once

#include "cocos2d.h"

struct ProductData
{
	ProductData(unsigned id, cocos2d::Label* priceLabel, cocos2d::Label* currentQTYLabel, cocos2d::Label* qauntityLabel, 
		unsigned price, unsigned count) : productId(id), productPriceText(priceLabel), productCurrentQTYText(currentQTYLabel),
		productPurchaseCountText(qauntityLabel), productSalePrice(price), purchaseCount(count) {};
	
	virtual~ProductData() { productPriceText = nullptr;  productCurrentQTYText = nullptr; productPurchaseCountText = nullptr; }

	unsigned productId = 0;
	unsigned productSalePrice = 0;
	cocos2d::Label* productPriceText = nullptr;
	cocos2d::Label* productCurrentQTYText = nullptr;
	cocos2d::Label* productPurchaseCountText = nullptr;
	unsigned purchaseCount = 0;
};

