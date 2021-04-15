#pragma once

#include "UIPanel.h"

NS_CC_BEGIN
namespace ui
{
	class Widget;
}
NS_CC_END

class GameScene;
class Shop;
class MouseOverMenuItem;

class ShopUpgrade : public UIPanel
{
public:
	virtual ~ShopUpgrade();

	void createShopUpgrade(Shop* shop, cocos2d::ui::Widget* parentWidget, cocos2d::Vec2 panelMidPoint);
protected:
	void upgradeCallback(cocos2d::Ref* pSender, unsigned shopId);
	void proceedUpgradeCallback(cocos2d::Ref* pSender);
	void cancelUpgradeCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);

private:
	enum EMessages
	{
		E_Qualified = 0,
		E_DisQualified = 1
	};
	Shop* m_Shop = nullptr;
	cocos2d::ui::Widget* m_ShopLockWidget;
	std::vector<unsigned int> m_ShopUpgradePrices;
	cocos2d::Vector<cocos2d::ui::Widget*> m_MessageWidgets;
	std::array<std::string, 2> m_Messages = { "Upgrade Failed", "Upgrade Successful" };
	cocos2d::Label* m_MessageText = nullptr;
	MouseOverMenuItem* m_TempButton = nullptr;

	std::string GetShopUpgradePrice(unsigned shopId);
	void showMessage(bool success);
};
