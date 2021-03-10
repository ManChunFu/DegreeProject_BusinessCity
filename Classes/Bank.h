#pragma once

#include "cocos2d.h"

class GameScene;

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
	cocos2d::Label* m_Sales = nullptr;

	std::string m_ShopName = "";
	unsigned m_CurrentWeek = 1;
	unsigned m_ElectricityFee = 200;
	unsigned m_WaterFee = 50;
	unsigned m_SalaryExpense = 5000;
	unsigned m_SalesIncome = 2000;

	void createBankPanel();

};