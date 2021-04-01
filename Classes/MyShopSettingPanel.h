#pragma once

#include "UIPanel.h"

class GameTime;
class Player;
class MouseOverMenuItem;
class Shop;
struct PurchaseProductData;

NS_CC_BEGIN
namespace ui
{
	class CheckBox;
	class Widget;
}
NS_CC_END

class MyShopSettingPanel : public UIPanel
{
public:
	~MyShopSettingPanel() override;

	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;
	void closePanel() override;
	std::function<void(cocos2d::Ref* pSender)> onActionCall;

protected:	
	void createPanel(cocos2d::Vec2 sceneMidPoint) override;
	void createProductWidget2(cocos2d::Vec2 panelMidPoint, cocos2d::Vec2 sceneMidPoint);
	void closeCallback(cocos2d::Ref* pSender);
	void reduceCallback(cocos2d::Ref* pSender);
	void addCallback(cocos2d::Ref* pSender);
	void hireCallback(cocos2d::Ref* pSender);
	void workHereCallback(cocos2d::Ref* pSender);
	void reduceTimeCallback(cocos2d::Ref* pSender, bool fromHourButton);
	void increaseTimeCallback(cocos2d::Ref* pSender, bool fromHourButton);
	void checkBoxClickCallback(cocos2d::Ref* pSender, unsigned weekday);
	void reducePriceCallback(cocos2d::Ref* pSender, unsigned productId);
	void increasePriceCallback(cocos2d::Ref* pSender, unsigned productId);
	void reduceProductAmoutCallback(cocos2d::Ref* pSender, unsigned productIndex);
	void increaseProductAmoutCallback(cocos2d::Ref* pSender, unsigned productIndex);
	void buyProductCallback(cocos2d::Ref* pSender, unsigned productId);
	void openWidget2Callback(cocos2d::Ref* pSender);
	void onQuantitytChanges(Shop* shop, unsigned productId, unsigned remainQuantity);
	void onCountDownChanges(Shop* shop, unsigned countdown);

	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);

private:
	GameTime* m_GameTime = nullptr;
	cocos2d::ui::Widget* m_ProductWidget1 = nullptr;
	cocos2d::ui::Widget* m_ProductWidget2 = nullptr;
	cocos2d::ui::Widget* m_WorkStatesWidget = nullptr;
	cocos2d::Label* m_WorkStateText = nullptr;
	cocos2d::Label* m_ReplenishCountdownText = nullptr;
	cocos2d::Label* m_EmployeeCountText = nullptr;
	cocos2d::Label* m_FromHourText = nullptr;
	cocos2d::Label* m_ToHourText = nullptr;
	std::vector<cocos2d::ui::CheckBox*> m_Weekdays;
	std::vector<PurchaseProductData*> m_PurchaseProducts;
	std::vector<cocos2d::Label*> m_CurrentProductQuantityTexts;
	cocos2d::Vector<cocos2d::MenuItem*> m_WidgetMenu;

	Shop* m_MyShop = nullptr;

	bool m_DisplayWidget2 = false;
	bool m_OwnerWorkHere = false;
	unsigned m_EmployeeCount = 0;
	unsigned m_FromHour = 8;
	unsigned m_ToHour = 17;
	unsigned m_PanelLimit = 2;
	std::string m_WorkStates[3] = { "Open", "Close", "Replenish Stock" };

	void enableWidget(cocos2d::ui::Widget* widget, bool enable, cocos2d::Vector<cocos2d::MenuItem*>itemList, cocos2d::itemTypes type);
	void enableMenuItems(cocos2d::Vector<cocos2d::MenuItem*>itemList, bool enable);
	void displayButtons(MouseOverMenuItem* button, cocos2d::Vec2 pos, cocos2d::itemTypes type = cocos2d::itemTypes::DEFAULT, float scale = 0.5f);
	void updateShopProductData();
	void updateShopWorkingState();
	std::string getWorkState();
};