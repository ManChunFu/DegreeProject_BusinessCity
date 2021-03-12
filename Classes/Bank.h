#pragma once

#include "cocos2d.h"

class GameScene;
class MouseOverMenuItem;

class Bank
{
public:
	~Bank();

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
	cocos2d::Label* m_Loan = nullptr;
	cocos2d::Label* m_Commerical = nullptr;
	cocos2d::Label* m_Sales = nullptr;
	cocos2d::Label* m_Total = nullptr;
	cocos2d::Label* m_LoanAmoutText = nullptr;

	std::string m_ShopName = "";
	unsigned m_CurrentWeek = 1;
	unsigned m_ElectricityFee = 0;
	unsigned m_WaterFee = 0;
	unsigned m_SalaryExpense = 0;
	unsigned m_Debt = 0;
	unsigned m_commericalFee = 0;
	unsigned m_SalesIncome = 0;
	unsigned m_LoanAmout = 10000;

	cocos2d::Vector<cocos2d::MenuItem*> m_BankButtons;

	void createBankPanel();
	int getOverviewAmout();
	void addAmoutCallback(cocos2d::Ref* pSender);
	void reduceAmoutCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);

};