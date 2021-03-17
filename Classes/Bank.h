#pragma once

#include "cocos2d.h"

class GameScene;
class MouseOverMenuItem;
NS_CC_BEGIN;
namespace ui
{
	class Widget;
}
NS_CC_END;

class Bank
{
public:
	virtual ~Bank();

	void openBankPanel(GameScene* scene, unsigned currentWeek);
	void closeBankPanel();

private:
	GameScene* m_GameScene = nullptr;
	cocos2d::Sprite* m_BankPanel = nullptr;
	cocos2d::Label* m_Weeks = nullptr;
	cocos2d::Label* m_Shop = nullptr;
	cocos2d::Label* m_Electricity = nullptr;
	cocos2d::Label* m_Water = nullptr;
	cocos2d::Label* m_Salary = nullptr;
	cocos2d::Label* m_PaybackWeekly = nullptr;
	cocos2d::Label* m_Commercial = nullptr;
	cocos2d::Label* m_Sales = nullptr;
	cocos2d::Label* m_Total = nullptr;
	cocos2d::Label* m_LoanAmoutText = nullptr;
	cocos2d::Label* m_WeeklyPayText = nullptr;
	cocos2d::Label* m_RepaymentText = nullptr;
	cocos2d::Sprite* m_DisabledPanel = nullptr;
	cocos2d::Label* m_DebtAmoutText = nullptr;
	cocos2d::Label* m_RemainWeeksText = nullptr;
	cocos2d::ui::Widget* m_LoanWidget = nullptr;

	std::string m_ShopName = "";
	unsigned m_CurrentWeek = 1;
	unsigned m_ElectricityFee = 0;
	unsigned m_WaterFee = 0;
	unsigned m_SalaryExpense = 0;
	unsigned m_commercialFee = 0;
	unsigned m_SalesIncome = 0;

	unsigned m_LoanAmout = 10000;
	unsigned m_PaybackWeeks = 5;
	unsigned m_Repayments = 2200;
	unsigned m_Debt = 0;
	unsigned m_RepaymentRemainWeeks = 0;

	cocos2d::Vector<cocos2d::MenuItem*> m_BankButtons;

	bool m_HasDebt = false;

	void createBankPanel();
	void updateOverviewAmout();
	void addAmoutCallback(cocos2d::Ref* pSender);
	void reduceAmoutCallback(cocos2d::Ref* pSender);
	void addWeekCallback(cocos2d::Ref* pSender);
	void reduceWeekCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
	unsigned updateLabelText(cocos2d::Label* label, unsigned originValue, int newValue, unsigned minValue, unsigned maxValue);
	void calculateWeeklyRepayments();
	void takeLoan(cocos2d::Ref* pSender);
	void setMenuItemsVisible(bool visible);
	void updateDebtDisplay(int repayment);
	void updatePlayerCurrentShopInfo();
};