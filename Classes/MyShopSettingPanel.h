#pragma once

#include "UIPanel.h"

class GameTime;
class GlobalTime;
class Player;
class MouseOverMenuItem;
class Shop;

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

	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint, unsigned shopId = 0) override;
	void closePanel() override;
	std::function<void(cocos2d::Ref* pSender)> onActionCall;
	std::function<void(unsigned shopId)> onShopChanges;
	void removeMySelfFromParent();
protected:	
	void createPanel(cocos2d::Vec2 sceneMidPoint, unsigned shopId = 0) override;
	void onOpenTabCallback(cocos2d::Ref* pSender, unsigned tabIndex);

	void closeCallback(cocos2d::Ref* pSender);
	void reduceCallback(cocos2d::Ref* pSender);
	void addCallback(cocos2d::Ref* pSender);
	void hireCallback(cocos2d::Ref* pSender);
	void workHereCallback(cocos2d::Ref* pSender);
	void openProductWidgetCallback(cocos2d::Ref* pSender, unsigned widgetIndex);
	void onQuantitytChanges(unsigned productId, unsigned remainQuantity);
	void onCountDownChanges(unsigned countdown);
	void onEveryHourChanges(GlobalTime* globalTime, unsigned hour);

	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);

	void onWorkDayChanges(unsigned weekday);
	void onWorkHourChanges(unsigned workhour);
	void onProductPriceChanges(unsigned productId, unsigned price);
	void onProductAmoutChanges(unsigned productId, unsigned amout);
	void onUpgradeChanges(unsigned productId);
private:
	enum EPanelTabs
	{
		E_Overview = 0,
		E_Admin = 1,
		E_Upgrade = 2
	};
	GameTime* m_GameTime = nullptr;
	Shop* m_MyShop = nullptr;
	std::vector<std::pair<cocos2d::ui::CheckBox*, cocos2d::ui::Widget*>> m_PanelTabs;
	std::vector<cocos2d::ui::Widget*> m_ProductWidgets;

	cocos2d::ui::Widget* m_WorkStatesWidget = nullptr;
	cocos2d::Label* m_ShopStateText = nullptr;
	cocos2d::Label* m_PlayerWorkHereText = nullptr;
	std::array<std::string, 2> m_PlayerWorkHereArray = { "No", "Yes" };
	cocos2d::Label* m_FromHourText = nullptr;
	cocos2d::Label* m_ToHourText = nullptr;
	cocos2d::Vector<cocos2d::Label*> m_ProductPriceTexts;
	cocos2d::Vector<cocos2d::Label*> m_ProductQTYTexts;

	cocos2d::Label* m_ReplenishCountdownText = nullptr;
	cocos2d::Label* m_EmployeeAtStoreText = nullptr;
	cocos2d::Label* m_EmployeeCountText = nullptr;
	cocos2d::Label* m_EmployeeSalaryText = nullptr;


	bool m_OwnerWorkHere = false;
	unsigned m_EmployeeCount = 0;
	unsigned m_PanelLimit = 3;
	std::array<std::string, 3> m_WorkStates = { "Open", "Close", "Replenish Stock" };
	
	void createProductOverview(cocos2d::ui::Widget* productWidget, unsigned startIndex, unsigned panelLimit, cocos2d::Vec2 panelMidPoint);
	void updateShopProductData();
	void updateShopWorkingState();
	std::string getWorkState();
};