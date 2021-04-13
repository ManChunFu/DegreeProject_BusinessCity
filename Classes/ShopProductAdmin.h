#pragma once

#include "UIPanel.h"

NS_CC_BEGIN
namespace ui
{
	class Widget;
}
NS_CC_END

class Shop;
class MouseOverMenuItem;
struct ProductData;

class ShopProductAdmin : public UIPanel
{
public:
	virtual ~ShopProductAdmin();

	void createShopProductAdmin(Shop* shop, cocos2d::ui::Widget* productWidget, cocos2d::Vec2 panelMidPoint);

	std::function<void(unsigned productId, unsigned price)> onProductPriceChanges;
	std::function<void(unsigned productId, unsigned amout)> onProductAmoutChanges;
protected:
	void openWidget2Callback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);
	void reducePriceCallback(cocos2d::Ref* pSender, unsigned productId);
	void increasePriceCallback(cocos2d::Ref* pSender, unsigned productId);
	void reduceProductAmoutCallback(cocos2d::Ref* pSender, unsigned productIndex);
	void increaseProductAmoutCallback(cocos2d::Ref* pSender, unsigned productIndex);
	void buyProductCallback(cocos2d::Ref* pSender, unsigned productId);

	void onCountDownChanges(unsigned countdown);

private:
	Shop* m_Shop = nullptr;
	std::vector<ProductData*> m_ProductDatas;
	cocos2d::ui::Widget* m_ProductWidget1 = nullptr;
	cocos2d::ui::Widget* m_ProductWidget2 = nullptr;
	cocos2d::Vector<cocos2d::MenuItem*> m_WidgetButtons;

	unsigned m_PanelLimit = 2;
	unsigned m_MinPriceChangesEachTime = 5;
	unsigned m_MAxPrice = 500;
	unsigned m_MinPurchaseQTY = 20;
	unsigned m_MaxPurchaseQTY = 100;

	void createProductWidget(cocos2d::ui::Widget* parentWidget, cocos2d::ui::Widget* productWidget, unsigned startIndex, unsigned panelLimit,
		cocos2d::Vector<cocos2d::MenuItem*> buttonList, cocos2d::Vec2 panelMidPoint, bool moreButton = true);
	MouseOverMenuItem* createMoreLessButton(bool more = true);
};
