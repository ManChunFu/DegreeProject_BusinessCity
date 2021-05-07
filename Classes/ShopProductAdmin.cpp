#include "ShopProductAdmin.h"
#include "ui/UIWidget.h"
#include "Shop.h"
#include "ShopProduct.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "MouseOverMenuItem.h"
#include "ProductData.h"
#include "Player.h"

USING_NS_CC;

ShopProductAdmin::~ShopProductAdmin()
{
	m_Shop = nullptr;
	m_WidgetButtons.clear();
	for (auto item : m_ProductDatas)
	{
		delete item;
	}
	m_ProductDatas.clear();
}

void ShopProductAdmin::createShopProductAdmin(Shop* shop, cocos2d::ui::Widget* productWidget, cocos2d::Vec2 panelMidPoint)
{
	// only overview widget is visible when the panel is open
	productWidget->setVisible(false);
	m_Player = GameData::getInstance().m_Player;
	m_Shop = shop;
	m_Shop->onCountdownChanges = CC_CALLBACK_1(ShopProductAdmin::onCountDownChanges, this);

	auto textPos = Vec2(panelMidPoint.x - 180.f, panelMidPoint.y + 5.f);
	std::array<unsigned int, 3> textPosIncreaseX = { 110.f, 65.f, 50.f };
	std::array<std::string, 4> productData = { "Name", "Price", "QTY", "Purchase QTY" };
	for (unsigned index = 0; index < 4; index++)
	{
		auto text = Label::createWithTTF(productData[index], "fonts/NirmalaB.ttf", 16);
		if (text)
		{
			text->enableGlow(Color4B::BLACK);
			GameFunctions::displayLabel(text, Color4B::WHITE, textPos, productWidget, 1, true, TextHAlignment::LEFT);
		}

		if (index == 3)
			continue;

		textPos.x += textPosIncreaseX[index];
	}

	m_ProductWidget1 = ui::Widget::create();
	m_ProductWidget1->setPosition(Vec2::ZERO);
	productWidget->addChild(m_ProductWidget1, 1);
	createProductWidget(productWidget, m_ProductWidget1, 0, m_PanelLimit, m_MenuItems, panelMidPoint);
}

void ShopProductAdmin::openWidget2Callback(cocos2d::Ref* pSender)
{
	if (m_ProductWidget1->isVisible())
	{
		m_ProductWidget1->setVisible(false);
		m_ProductWidget2->setVisible(true);
		return;
	}

	m_ProductWidget1->setVisible(true);
	m_ProductWidget2->setVisible(false);
}

void ShopProductAdmin::reducePriceCallback(cocos2d::Ref* pSender, unsigned productId)
{
	m_ProductDatas[productId]->m_ProductSalePrice = GameFunctions::displayLabelText_ClampValue(
		m_ProductDatas[productId]->m_ProductPriceText, m_ProductDatas[productId]->m_ProductSalePrice,
		-m_MinPriceChangesEachTime, 0, m_MAxPrice);
	m_ProductDatas[productId]->m_ProductPriceText->setString(std::to_string(m_ProductDatas[productId]->m_ProductSalePrice));

	// update shop data
	m_Shop->setCurrentSalePrice(productId, m_ProductDatas[productId]->m_ProductSalePrice);

	if (onProductPriceChanges)
		onProductPriceChanges(productId, m_Shop->getCurrentSalePrice(productId));
}

void ShopProductAdmin::increasePriceCallback(cocos2d::Ref* pSender, unsigned productId)
{
	m_ProductDatas[productId]->m_ProductSalePrice = GameFunctions::displayLabelText_ClampValue(
		m_ProductDatas[productId]->m_ProductPriceText, m_ProductDatas[productId]->m_ProductSalePrice,
		m_MinPriceChangesEachTime, 0, m_MAxPrice);
	m_ProductDatas[productId]->m_ProductPriceText->setString(std::to_string(m_ProductDatas[productId]->m_ProductSalePrice));

	// update shop data
	m_Shop->setCurrentSalePrice(productId, m_ProductDatas[productId]->m_ProductSalePrice);

	if (onProductPriceChanges)
		onProductPriceChanges(productId, m_Shop->getCurrentSalePrice(productId));
}

void ShopProductAdmin::reduceProductAmoutCallback(cocos2d::Ref* pSender, unsigned productIndex)
{
	m_ProductDatas[productIndex]->m_ProductId = productIndex;
	m_ProductDatas[productIndex]->m_PurchaseCount = GameFunctions::displayLabelText_ClampValue(
		m_ProductDatas[productIndex]->m_ProductPurchaseCountText,
		m_ProductDatas[productIndex]->m_PurchaseCount, -m_MinPurchaseQTY, m_MinPurchaseQTY, m_MaxPurchaseQTY);
	m_ProductDatas[productIndex]->m_ProductPurchaseCountText->setString(std::to_string(m_ProductDatas[productIndex]->m_PurchaseCount));
}

void ShopProductAdmin::increaseProductAmoutCallback(cocos2d::Ref* pSender, unsigned productIndex)
{
	m_ProductDatas[productIndex]->m_ProductId = productIndex;
	m_ProductDatas[productIndex]->m_PurchaseCount = GameFunctions::displayLabelText_ClampValue(
		m_ProductDatas[productIndex]->m_ProductPurchaseCountText,
		m_ProductDatas[productIndex]->m_PurchaseCount, m_MinPurchaseQTY, m_MinPurchaseQTY, m_MaxPurchaseQTY);
	m_ProductDatas[productIndex]->m_ProductPurchaseCountText->setString(std::to_string(m_ProductDatas[productIndex]->m_PurchaseCount));
}

void ShopProductAdmin::buyProductCallback(cocos2d::Ref* pSender, unsigned productId)
{
	m_Shop->setRunForErrand();

	//update shop data
	m_Shop->increaseProductQuantity(productId, m_ProductDatas[productId]->m_PurchaseCount);

	//update currentQTY
	m_ProductDatas[productId]->m_CurrentQTY += m_ProductDatas[productId]->m_PurchaseCount;
	m_ProductDatas[productId]->m_CurrentQTYLabel->setString(std::to_string(m_ProductDatas[productId]->m_CurrentQTY));

	// update player current cash amout
	auto totalPurchasePrice = m_Shop->getProductPurchasePrice(productId) * m_ProductDatas[productId]->m_PurchaseCount;
	m_Player->updateCurrentCashAmout(-totalPurchasePrice);

	if (onProductAmoutChanges)
		onProductAmoutChanges(productId, m_Shop->getProductQuantity(productId));

	// buy button stats
	if (m_Shop->isReplenishing())
	{
		//enableBuyButtons(false);
		//updateShopWorkingState();
	}
}

void ShopProductAdmin::onCountDownChanges(unsigned countdown)
{
}

void ShopProductAdmin::onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
}

void ShopProductAdmin::createProductWidget(cocos2d::ui::Widget* parentWidget, cocos2d::ui::Widget* productWidget, unsigned startIndex, unsigned panelLimit, cocos2d::Vector<cocos2d::MenuItem*> buttonList, cocos2d::Vec2 panelMidPoint, bool moreButton)
{
	auto productLength = m_Shop->getProductsSize();
	auto productSpritePos = Vec2(panelMidPoint.x - 230.f, panelMidPoint.y - 30.f);
	auto darkCyanColor = GameData::getInstance().m_ColorType.DarkCyan;

	std::array<std::string, 2> purchaseText = { "$", "/PCS" };
	auto purchaseTextPosX = panelMidPoint.x + 170.f;

	if (!moreButton)
	{
		auto button = createMoreLessButton(false);
		if (button)
		{
			button->itemSelectedData.type = itemTypes::WIDGET_BUTTON;
			buttonList.pushBack(displayMenuButton(button, CC_CALLBACK_2(ShopProductAdmin::onMouseOver, this),
				Vec2(panelMidPoint.x, panelMidPoint.y - 150.f), itemTypes::WIDGET_BUTTON, 0.5f, true));
		}
	}
	for (unsigned productIndex = startIndex; productIndex < productLength; productIndex++)
	{
		if (productIndex > panelLimit)
		{
			auto button = createMoreLessButton(moreButton);
			if (button)
			{
				button->itemSelectedData.type = itemTypes::BUTTON;
				buttonList.pushBack(displayMenuButton(button, CC_CALLBACK_2(ShopProductAdmin::onMouseOver, this),
					Vec2(panelMidPoint.x, panelMidPoint.y - 150.f), itemTypes::BUTTON, 0.5f, true));
				if (moreButton)
				{
					m_ProductWidget2 = ui::Widget::create();
					m_ProductWidget2->setPosition(Vec2::ZERO);
					parentWidget->addChild(m_ProductWidget2, 1);
					createProductWidget(parentWidget, m_ProductWidget2, m_PanelLimit +1, m_PanelLimit+3, m_WidgetButtons, panelMidPoint, false);
					m_ProductWidget2->setVisible(false);
				}
			}
			break;
		}

		// product name
		auto productName = Label::createWithTTF(m_Shop->getProductName(productIndex), "fonts/Nirmala.ttf", 15);
		if (productName)
		{
			productName->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(productName, Color4B::WHITE, Vec2(productSpritePos.x, productSpritePos.y - 10.f),
				productWidget, 1, true, TextHAlignment::LEFT);
		}

		// product sale price box
		auto boxSprite = Sprite::createWithSpriteFrameName("WhiteBox50_30.png");
		auto boxSpriteMidPoint = Vec2(boxSprite->getContentSize() * 0.5f);

		if (boxSprite)
			GameFunctions::displaySprite(boxSprite, Vec2(productSpritePos.x + 180.f, productSpritePos.y), productWidget, 1);

		// product sale price text
		auto productSalePrice = Label::createWithTTF("", "fonts/Nirmala.ttf", 15);
		if (productSalePrice)
		{
			productSalePrice->enableShadow(darkCyanColor);
			productSalePrice->setString(std::to_string(m_Shop->getProductSalePrice(productIndex)));
			GameFunctions::displayLabel(productSalePrice, Color4B::WHITE, boxSpriteMidPoint, boxSprite, 1);
		}

		// product increaase decrease price
		for (unsigned buttonIndex = 0; buttonIndex < 2; buttonIndex++)
		{
			auto button = (buttonIndex % 2 == 0) ? MouseOverMenuItem::creatMouseOverMenuButton("UIButtonLess50_PointLeft.png",
				"UIButtonLess50_PointLeft_Lit.png", "UIButtonLess50_PointLeft_Disable.png",
				CC_CALLBACK_1(ShopProductAdmin::reducePriceCallback, this, productIndex)) : MouseOverMenuItem::creatMouseOverMenuButton(
					"UIButtonMore50_PointRight.png", "UIButtonMore50_PointRight_Lit.png", "UIButtonMore50_PointRight_Disable.png",
					CC_CALLBACK_1(ShopProductAdmin::increasePriceCallback, this, productIndex));
			if (button)
			{
				buttonList.pushBack(displayMenuButton(button, CC_CALLBACK_2(ShopProductAdmin::onMouseOver, this), (buttonIndex % 2 == 0) ?
					Vec2(productSpritePos.x + 155.f, productSpritePos.y) : Vec2(productSpritePos.x + 205.f, productSpritePos.y),
					itemTypes::WIDGET_BUTTON, 0.5f, true));
			}
		}

		// product current QTY
		auto currentQTY = Label::createWithTTF("", "fonts/Nirmala.ttf", 16);
		if (currentQTY)
		{
			currentQTY->setString(std::to_string(m_Shop->getProductQuantity(productIndex)));
			currentQTY->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(currentQTY, Color4B::WHITE, Vec2(productSpritePos.x + 240.f, productSpritePos.y), productWidget, 1);
		}

		// product qty box
		auto qtyBoxSprite = Sprite::createWithSpriteFrameName("WhiteBox50_30.png");
		auto qtyBoxSpriteMidPoint = Vec2(qtyBoxSprite->getContentSize() * 0.5f);

		if (qtyBoxSprite)
			GameFunctions::displaySprite(qtyBoxSprite, Vec2(productSpritePos.x + 300.f, productSpritePos.y), productWidget, 1);

		auto qtyText = Label::createWithTTF(std::to_string(m_MinPurchaseQTY), "fonts/Nirmala.ttf", 16);
		if (qtyText)
		{
			qtyText->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(qtyText, Color4B::WHITE, qtyBoxSpriteMidPoint, qtyBoxSprite, 1);
		}

		// increase / decrease purchase qty
		for (unsigned buttonIndex = 0; buttonIndex < 2; buttonIndex++)
		{
			auto button = (buttonIndex % 2 == 0) ? MouseOverMenuItem::creatMouseOverMenuButton("UIButtonLess50_PointLeft.png",
				"UIButtonLess50_PointLeft_Lit.png", "UIButtonLess50_PointLeft_Disable.png",
				CC_CALLBACK_1(ShopProductAdmin::reduceProductAmoutCallback, this, productIndex)) : MouseOverMenuItem::creatMouseOverMenuButton(
					"UIButtonMore50_PointRight.png", "UIButtonMore50_PointRight_Lit.png", "UIButtonMore50_PointRight_Disable.png",
					CC_CALLBACK_1(ShopProductAdmin::increaseProductAmoutCallback, this, productIndex));
			if (button)
			{
				buttonList.pushBack(displayMenuButton(button, CC_CALLBACK_2(ShopProductAdmin::onMouseOver, this), (buttonIndex % 2 == 0) ?
					Vec2(productSpritePos.x + 275.f, productSpritePos.y) : Vec2(productSpritePos.x + 325.f, productSpritePos.y),
					itemTypes::BUTTON, 0.5f, true));
			}
		}

		// add to product data
		m_ProductDatas.push_back(new ProductData(m_Shop->getProductId(m_Shop->m_ShopId), productSalePrice,
			m_Shop->getProductSalePrice(productIndex), currentQTY, m_Shop->getProductQuantity(productIndex),
			qtyText, m_MinPurchaseQTY));

		//purchase qty button
		auto buyButton = MouseOverMenuItem::creatMouseOverMenuButton("ButtonBlueNormal50_30.png", "ButtonBlueLit50_30.png",
			"ButtonBlueDisable50_30.png", CC_CALLBACK_1(ShopProductAdmin::buyProductCallback, this, productIndex));

		if (buyButton)
		{
			buttonList.pushBack(displayMenuButton(buyButton, CC_CALLBACK_2(ShopProductAdmin::onMouseOver, this), Vec2(productSpritePos.x + 365.f,
				productSpritePos.y), itemTypes::BUTTON, 1.f, true));

			auto buyText = Label::createWithTTF("Buy", "fonts/NirmalaB.ttf", 16);
			if (buyText)
			{
				buyText->enableGlow(Color4B::BLACK);
				GameFunctions::displayLabel(buyText, Color4B::WHITE, buyButton->getContentSize() * 0.5f, buyButton, 1);
			}
		}

		// purchase qty text
		for (unsigned textIndex = 0; textIndex < 2; textIndex++)
		{
			auto text = Label::createWithTTF(purchaseText[textIndex], "fonts/Nirmala.ttf", 15);
			if (text)
			{
				text->enableShadow(darkCyanColor);
				GameFunctions::displayLabel(text, Color4B::WHITE, Vec2(purchaseTextPosX, productSpritePos.y - 10.f), productWidget, 1,
					true, TextHAlignment::LEFT);
			}
			purchaseTextPosX += 30.f;
		}

		purchaseTextPosX = panelMidPoint.x + 170.f;

		auto purchasePriceText = Label::createWithTTF("", "fonts/Nirmala.ttf", 15);
		if (purchasePriceText)
		{
			purchasePriceText->setString(std::to_string(m_Shop->getProductPurchasePrice(productIndex)));
			purchasePriceText->enableShadow(darkCyanColor);
			GameFunctions::displayLabel(purchasePriceText, Color4B::WHITE, Vec2(productSpritePos.x + 430.f, productSpritePos.y + 10.f), 
				productWidget, 1, true, TextHAlignment::RIGHT);
		}
#pragma endregion
		productSpritePos.y -= 40.f;
	}

	auto menu = Menu::createWithArray(buttonList);
	menu->setPosition(Vec2::ZERO);
	productWidget->addChild(menu, 1);
}

MouseOverMenuItem* ShopProductAdmin::createMoreLessButton(bool more)
{
	if (more)
	{
		auto moreButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonMore50.png", "UIButtonMore50_Lit.png",
			"UIButtonMore50_Disable.png", CC_CALLBACK_1(ShopProductAdmin::openWidget2Callback, this));

		return moreButton;
	}

	auto lessButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonLess50.png", "UIButtonLess50_Lit.png",
		"UIButtonLess50_Disable.png", CC_CALLBACK_1(ShopProductAdmin::openWidget2Callback, this));

	return lessButton;
}
