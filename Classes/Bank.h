#pragma once

#include "UIPanel.h"

NS_CC_BEGIN;
namespace ui
{
	class Widget;
	class CheckBox;
}
NS_CC_END;

struct TotalExpense;
class ActionPanel;

class Bank : public UIPanel
{
public:
	Bank(ActionPanel* actionPanel);
	~Bank() override;

	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;
	void closePanel() override;

	void update();
protected:
	void createPanel(cocos2d::Vec2 sceneMidPoint, unsigned shopId = 0) override;
	void closeCallback(cocos2d::Ref* pSedner);
	void onOpenBalanceCallback(cocos2d::Ref* pSender, unsigned shopWidgetIndex);
	void addAmoutCallback(cocos2d::Ref* pSender);
	void reduceAmoutCallback(cocos2d::Ref* pSender);
	void addWeekCallback(cocos2d::Ref* pSender);
	void reduceWeekCallback(cocos2d::Ref* pSender);
	void takeLoan(cocos2d::Ref* pSender);
	void onSalesIncomeChanges(unsigned shopId, unsigned totalSales);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);

	//TODO:: fix shop upgrade display
	//void onUpgradeShopCalls(unsigned removeShopId, unsigned upgradeShopId);

private:
	struct TotalExpense* m_TotalShopExpense = nullptr;
	std::vector<cocos2d::ui::CheckBox*> m_ShopButtons;
	cocos2d::Vector<cocos2d::ui::Widget*> m_ShopWidgets;
	cocos2d::Vec2 m_NewShopPos = cocos2d::Vec2::ZERO;

	cocos2d::Label* m_Weeks = nullptr;
	cocos2d::Label* m_Electricity = nullptr;
	cocos2d::Label* m_Water = nullptr;
	cocos2d::Label* m_Salary = nullptr;
	cocos2d::Label* m_PaybackWeekly = nullptr;
	cocos2d::Label* m_Commercial = nullptr;
	cocos2d::Label* m_Sales = nullptr;
	cocos2d::Label* m_Total = nullptr;

	cocos2d::ui::Widget* m_LoanWidget = nullptr;
	cocos2d::Label* m_LoanAmoutText = nullptr;
	cocos2d::Label* m_WeeklyPayText = nullptr;
	cocos2d::Label* m_RepaymentText = nullptr;
	
	cocos2d::Sprite* m_DisabledPanel = nullptr;
	cocos2d::Label* m_DebtAmoutText = nullptr;
	cocos2d::Label* m_RemainWeeksText = nullptr;

	unsigned m_CurrentWeek = 1;

	unsigned m_LoanAmout = 10000;
	unsigned m_PaybackWeeks = 5;
	unsigned m_Repayments = 2200;
	unsigned m_InterestsWeekly = 200;
	unsigned m_Principle = 2000;
	unsigned m_RepaymentRemainWeeks = 0;
	int m_Debt = 0;

	bool m_HasDebt = false;

	int calculateTotalAmoutWeekly(bool removeSalesIncome = false);
	void updateOverviewAmout(cocos2d::Label* total,int amout);
	void calculateWeeklyRepayments();
	void updateDebtDisplay(int amout, unsigned remainWeeks);
	void resetTakeLoan();
	void updatePlayerCurrentShopInfo();
	void setMenuItemsVisible(bool visible, bool runAll, cocos2d::itemTypes excludeType = cocos2d::itemTypes::DEFAULT);
	void createNewShopBalance(unsigned shopIndex, cocos2d::Vec2 panelMidPoint);
};