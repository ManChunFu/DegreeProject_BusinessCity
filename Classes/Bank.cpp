#include "Bank.h"
#include "GameScene.h"
#include "GameData.h"
#include "GameFunctions.h"
#include <ui/UITextField.h>
#include "MouseOverMenuItem.h"


USING_NS_CC;

Bank::~Bank()
{
	m_BankPanel = nullptr;
	m_Weeks = nullptr;
	m_Shop = nullptr;
	m_Electricity = nullptr;
	m_Water = nullptr;
	m_Salary = nullptr;
	m_Loan = nullptr;
	m_Commerical = nullptr;
	m_Sales = nullptr;
	m_Total = nullptr;
	m_CurrentWeek = 1;
	m_BankButtons.clear();
}

void Bank::openBankPanel(GameScene* scene, unsigned currentWeek)
{
	m_CurrentWeek = currentWeek;

	if (!m_BankPanel)
	{
		m_GameScene = scene;

		if (m_GameScene)
			createBankPanel();

		return;
	}

	m_BankPanel->setVisible(true);
	GameFunctions::updatLabelText_TimeFormat(m_Weeks, m_CurrentWeek);
}

void Bank::closeBankPanel()
{
	m_BankPanel->setVisible(false);
}

void Bank::createBankPanel()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sceneMidPoint = Point(origin.x + (visibleSize.width * 0.5f), origin.y + (visibleSize.height * 0.5f));

	// create Bank panel
	m_BankPanel = Sprite::createWithSpriteFrameName("Bank_Panel_400.png");
	if (!m_BankPanel)
		return;

	m_BankPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(m_BankPanel, 1);

	auto panelMidPoint = Vec2(m_BankPanel->getContentSize().width * 0.5f, m_BankPanel->getContentSize().height * 0.5f);

#pragma region create account balance label, week label and week count
	auto weeklyOverviewLabel = Label::createWithTTF("WEEKLY OVERVIEW", "fonts/NirmalaB.ttf", 20);
	if (weeklyOverviewLabel)
	{
		GameFunctions::displayLabel(weeklyOverviewLabel, GameData::getInstance().m_ColorType.Taro,
			Vec2(panelMidPoint.x - 180.f, panelMidPoint.y + 160.f), m_BankPanel, 1);
		weeklyOverviewLabel->enableOutline(Color4B::WHITE);
		weeklyOverviewLabel->enableShadow(Color4B::BLACK);
	}

	auto weekLabel = Label::createWithTTF("WEEK", "fonts/NirmalaB.ttf", 25);
	auto weekPos = Vec2(panelMidPoint.x - 20.f, panelMidPoint.y + 160.f);
	if (weekLabel)
	{
		GameFunctions::displayLabel(weekLabel, Color4B::WHITE, weekPos, m_BankPanel, 1);
		weekLabel->enableShadow(Color4B::BLACK);
	}

	m_Weeks = Label::createWithTTF("", "fonts/NirmalaB.ttf", 25);
	if (m_Weeks)
	{
		GameFunctions::updatLabelText_TimeFormat(m_Weeks, m_CurrentWeek);
		m_Weeks->enableOutline(Color4B::WHITE);
		GameFunctions::displayLabel(m_Weeks, Color4B::BLACK, Vec2(weekPos.x + 70.f, weekPos.y), m_BankPanel, 1);
	}
#pragma endregion

#pragma region Create weekly expense
	m_Shop = Label::createWithTTF("Hot Dog Stand", "fonts/NirmalaB.ttf", 15);
	if (m_Shop)
	{
		m_Shop->enableGlow(Color4B::WHITE);
		GameFunctions::displayLabel(m_Shop, Color4B::BLACK, Vec2(panelMidPoint.x, panelMidPoint.y + 120.f),
			m_BankPanel, 1);
	}

	// electricity
	auto electricityLabel = Label::createWithTTF("Electricity", "fonts/Nirmala.ttf", 20);
	if (electricityLabel)
		GameFunctions::displayLabel(electricityLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 230.f, panelMidPoint.y + 100.f),
			m_BankPanel, 1);

	m_Electricity = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
	if (m_Electricity)
	{
		GameFunctions::updateLabelText_MoneyFormat(m_Electricity, m_ElectricityFee, true);
		GameFunctions::displayLabel(m_Electricity, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y + 110.f),
			m_BankPanel, 1, true);
	}

	// water
	auto waterLabel = Label::createWithTTF("Water", "fonts/Nirmala.ttf", 20);
	if (waterLabel)
		GameFunctions::displayLabel(waterLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 245.f, panelMidPoint.y + 70.f),
			m_BankPanel, 1);

	m_Water = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
	if (m_Water)
	{
		GameFunctions::updateLabelText_MoneyFormat(m_Water, m_WaterFee, true);
		GameFunctions::displayLabel(m_Water, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y + 80.f), m_BankPanel, 1, true);
	}

	// salary
	auto salaryLabel = Label::createWithTTF("Employee Salary", "fonts/Nirmala.ttf", 20);
	if (salaryLabel)
		GameFunctions::displayLabel(salaryLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 200.f, panelMidPoint.y + 40.f),
			m_BankPanel, 1);

	m_Salary = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
	if (m_Salary)
	{
		GameFunctions::updateLabelText_MoneyFormat(m_Salary, m_SalaryExpense, true);
		GameFunctions::displayLabel(m_Salary, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y + 50.f), m_BankPanel, 1, true);
	}

	// loan
	auto loanLabel = Label::createWithTTF("Loan", "fonts/Nirmala.ttf", 20);
	if (loanLabel)
		GameFunctions::displayLabel(loanLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 250.f, panelMidPoint.y + 10.f),
			m_BankPanel, 1);

	auto loanDetailLabel = Label::createWithTTF("(weekly repayments)", "fonts/Nirmala.ttf", 15);
	if (loanDetailLabel)
		GameFunctions::displayLabel(loanDetailLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 160.f, panelMidPoint.y + 8.f),
			m_BankPanel, 1);

	m_Loan = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
	if (m_Loan)
	{
		GameFunctions::updateLabelText_MoneyFormat(m_Loan, m_Debt, true);
		GameFunctions::displayLabel(m_Loan, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y + 20.f), m_BankPanel, 1, true);
	}

	// commerical
	auto commericalLabel = Label::createWithTTF("Commercial", "fonts/Nirmala.ttf", 20);
	if (commericalLabel)
		GameFunctions::displayLabel(commericalLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 220.f, panelMidPoint.y - 20.f),
			m_BankPanel, 1);

	m_Commerical = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
	if (m_Commerical)
	{
		GameFunctions::updateLabelText_MoneyFormat(m_Commerical, m_commericalFee, true);
		GameFunctions::displayLabel(m_Commerical, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y - 10.f), m_BankPanel, 1, true);
	}

	// sales
	auto salesLabel = Label::createWithTTF("Sales Income", "fonts/Nirmala.ttf", 20);
	if (salesLabel)
		GameFunctions::displayLabel(salesLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 215.f, panelMidPoint.y - 50.f),
			m_BankPanel, 1);

	m_Sales = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
	if (m_Sales)
	{
		GameFunctions::updateLabelText_MoneyFormat(m_Sales, m_SalesIncome);
		GameFunctions::displayLabel(m_Sales, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y - 40.f), m_BankPanel, 1, true);
	}

	auto totalLabel = Label::createWithTTF("TOTAL", "fonts/NirmalaB.ttf", 20);
	if (totalLabel)
		GameFunctions::displayLabel(totalLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 240.f, panelMidPoint.y - 105.f),
			m_BankPanel, 1);

	m_Total = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
	if (m_Total)
	{
		auto amout = getOverviewAmout();
		bool isMinus = false;
		if (amout < 0)
		{
			isMinus = true;
			amout *= -1;
		}

		GameFunctions::updateLabelText_MoneyFormat(m_Total, amout, isMinus);
		GameFunctions::displayLabel(m_Total, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y - 95.f), m_BankPanel, 1, true);
	}
#pragma endregion

#pragma region Create Loan Button
	// loan amout
	auto loanAmoutText = Label::createWithTTF("Loan Amout", "fonts/Nirmala.ttf", 14);
	if (loanAmoutText)
		GameFunctions::displayLabel(loanAmoutText, Color4B::WHITE, Vec2(panelMidPoint.x - 215.f, panelMidPoint.y - 135.f),
			m_BankPanel, 1);

	auto loanAmoutSprite = Sprite::createWithSpriteFrameName("Border_Brown.png");
	if (loanAmoutSprite)
	{
		loanAmoutSprite->setPosition(panelMidPoint.x - 215.f, panelMidPoint.y - 165.f);
		m_BankPanel->addChild(loanAmoutSprite, 1);

		//,000 label
		m_LoanAmoutText = Label::createWithTTF("10,000", "fonts/Nirmala.ttf", 20);
		if (m_LoanAmoutText)
			GameFunctions::displayLabel(m_LoanAmoutText, Color4B::WHITE, Vec2(loanAmoutSprite->getContentSize().width - 15.f, loanAmoutSprite->getContentSize().height -5.f),
				loanAmoutSprite, 1, true);
	}

	auto reduceAmoutButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonCorner40.png", "UIButtonCorner40_Lit.png", "UIButtonCorner40_Disabled.png",
		CC_CALLBACK_1(Bank::reduceAmoutCallback, this));

	if (reduceAmoutButton)
	{
		reduceAmoutButton->onMouseOver = CC_CALLBACK_2(Bank::onMouseOver, this);
		Vec2 reducePos = Vec2(sceneMidPoint.x - 175.f, sceneMidPoint.y - 175.f);
		reduceAmoutButton->setScale(0.5f);
		reduceAmoutButton->setPosition(reducePos);
		reduceAmoutButton->setItemRect(reducePos, 0.5f);

		m_BankButtons.pushBack(reduceAmoutButton);
	}

	auto addAmoutButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonCorner40Left.png", "UIButtonCorner40Left_Lit.png", "UIButtonCorner40Left_Disabled.png",
		CC_CALLBACK_1(Bank::addAmoutCallback, this));

	if (addAmoutButton)
	{
		addAmoutButton->onMouseOver = CC_CALLBACK_2(Bank::onMouseOver, this);
		Vec2 addPos = Vec2(sceneMidPoint.x - 255.f, sceneMidPoint.y - 155.f);
		addAmoutButton->setScale(0.5f);
		addAmoutButton->setPosition(addPos);
		addAmoutButton->setItemRect(addPos, 0.5f);

		m_BankButtons.pushBack(addAmoutButton);
	}

	// payback schedule
	auto paybackLabel = Label::createWithTTF("Payback Schedule", "fonts/Nirmala.ttf", 14);
	if (paybackLabel)
		GameFunctions::displayLabel(paybackLabel, Color4B::WHITE, Vec2(panelMidPoint.x -70.f, panelMidPoint.y - 135.f),
			m_BankPanel, 1);

	auto paybackSprite = Sprite::createWithSpriteFrameName("Border_Brown_Square.png");
	if (paybackSprite)
	{
		paybackSprite->setPosition(panelMidPoint.x - 100.f, panelMidPoint.y - 165.f);
		m_BankPanel->addChild(paybackSprite, 1);

		//,000 label
		m_WeeklyPayText = Label::createWithTTF("5", "fonts/Nirmala.ttf", 20);
		if (m_WeeklyPayText)
			GameFunctions::displayLabel(m_WeeklyPayText, Color4B::WHITE, Vec2(paybackSprite->getContentSize().width - 15.f, paybackSprite->getContentSize().height - 5.f),
				paybackSprite, 1, true);
	}

	auto reduceWeekButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonCorner40.png", "UIButtonCorner40_Lit.png", "UIButtonCorner40_Disabled.png",
		CC_CALLBACK_1(Bank::reduceWeekCallback, this));

	if (reduceWeekButton)
	{
		reduceWeekButton->onMouseOver = CC_CALLBACK_2(Bank::onMouseOver, this);
		Vec2 reduceWeekPos = Vec2(sceneMidPoint.x - 85.f, sceneMidPoint.y - 176.f);
		reduceWeekButton->setScale(0.5f);
		reduceWeekButton->setPosition(reduceWeekPos);
		reduceWeekButton->setItemRect(reduceWeekPos);

		m_BankButtons.pushBack(reduceWeekButton);
	}

	auto addWeekButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonCorner40Left.png", "UIButtonCorner40Left_Lit.png", "UIButtonCorner40Left_Disabled.png",
		CC_CALLBACK_1(Bank::addWeekCallback, this));

	if (addWeekButton)
	{
		addWeekButton->onMouseOver = CC_CALLBACK_2(Bank::onMouseOver, this);
		Vec2 addWeekPos = Vec2(sceneMidPoint.x - 115.f, sceneMidPoint.y - 155.f);
		addWeekButton->setScale(0.5f);
		addWeekButton->setPosition(addWeekPos);
		addWeekButton->setItemRect(addWeekPos, 0.5f);

		m_BankButtons.pushBack(addWeekButton);
	}

	auto totalWeeksLabel = Label::createWithTTF("Weeks", "fonts/Nirmala.ttf", 20);
	if (totalWeeksLabel)
		GameFunctions::displayLabel(totalWeeksLabel, Color4B::WHITE, Vec2(panelMidPoint.x - 45.f, panelMidPoint.y - 165.f),
			m_BankPanel, 1);

	// weekly repayments
	auto repaymentLabel = Label::createWithTTF("Weekly Repayments", "fonts/Nirmala.ttf", 14);
	if (repaymentLabel)
		GameFunctions::displayLabel(repaymentLabel, Color4B::WHITE, Vec2(panelMidPoint.x + 90.f, panelMidPoint.y - 135.f),
			m_BankPanel, 1);

	auto cashSymbol = Label::createWithTTF("$", "fonts/Nirmala.ttf", 20);
	if (cashSymbol)
	{
		GameFunctions::displayLabel(cashSymbol, GameData::getInstance().m_ColorType.Taro, Vec2(panelMidPoint.x + 40.f, panelMidPoint.y - 165.f),
			m_BankPanel, 1);
		cashSymbol->enableShadow(Color4B::BLACK);
	}

	m_RepaymentText = Label::createWithTTF("2,200", "fonts/NirmalaB.ttf", 20);
	if (m_RepaymentText)
	{
		GameFunctions::displayLabel(m_RepaymentText, GameData::getInstance().m_ColorType.Taro, Vec2(panelMidPoint.x + 90.f, panelMidPoint.y - 165.f),
			m_BankPanel, 1);
		m_RepaymentText->enableShadow(Color4B::BLACK);
	}

	// apply loan button
	auto applyLoanButton = MouseOverMenuItem::creatMouseOverMenuButton("Button100.png", "Button100_Lit.png", "Button100_Disabled.png",
		CC_CALLBACK_1(Bank::takeLoan, this));

	if (applyLoanButton)
	{
		applyLoanButton->onMouseOver = CC_CALLBACK_2(Bank::onMouseOver, this);
		Vec2 applyLoanPos = Vec2(sceneMidPoint.x + 220.f, sceneMidPoint.y - 165.f);
		applyLoanButton->setPosition(applyLoanPos);
		applyLoanButton->setItemRect(applyLoanPos);

		auto takeLoanText = Label::createWithTTF("TAKE LOAN", "fonts/NirmalaB.ttf", 14);
		if (takeLoanText)
		{
			GameFunctions::displayLabel(takeLoanText, GameData::getInstance().m_ColorType.Taro,
				Vec2(applyLoanButton->getContentSize().width * 0.5f, applyLoanButton->getContentSize().height * 0.5f),
				applyLoanButton, 1);
			takeLoanText->enableOutline(Color4B::BLACK);
		}

		m_BankButtons.pushBack(applyLoanButton);
	}
#pragma endregion

	auto menu = Menu::createWithArray(m_BankButtons);
	menu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(menu, 2);

}

int Bank::getOverviewAmout()
{
	return m_SalesIncome - m_ElectricityFee - m_WaterFee - m_SalaryExpense - m_commericalFee - m_Debt;
}

void Bank::addAmoutCallback(cocos2d::Ref* pSender)
{
	m_LoanAmout = updateLabelText(m_LoanAmoutText, m_LoanAmout, 10000, 10000, 100000);
	calculateWeeklyRepayments();
}

void Bank::reduceAmoutCallback(cocos2d::Ref* pSender)
{
	m_LoanAmout = updateLabelText(m_LoanAmoutText, m_LoanAmout, -10000, 10000, 100000);
	calculateWeeklyRepayments();
}

void Bank::addWeekCallback(cocos2d::Ref* pSender)
{
	m_PaybackWeeks = updateLabelText(m_WeeklyPayText, m_PaybackWeeks, 1, 5, 30);
	calculateWeeklyRepayments();
}

void Bank::reduceWeekCallback(cocos2d::Ref* pSender)
{
	m_PaybackWeeks = updateLabelText(m_WeeklyPayText, m_PaybackWeeks, -1, 5, 30);
	calculateWeeklyRepayments();
}

void Bank::onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event)
{}

unsigned Bank::updateLabelText(cocos2d::Label* label, unsigned originValue, int newValue, unsigned minValue, unsigned maxValue)
{
	originValue += newValue;
	originValue = clampf(originValue, minValue, maxValue);
	GameFunctions::updateLabelText_MoneyFormat(label, originValue);
	return originValue;
}

void Bank::calculateWeeklyRepayments()
{
	auto rate = 1 + (float)(m_PaybackWeeks) * 2 / 100;
	auto repayments = (float)(m_LoanAmout / m_PaybackWeeks) * rate;
	GameFunctions::updateLabelText_MoneyFormat(m_RepaymentText, repayments);
}

void Bank::takeLoan(cocos2d::Ref* pSender)
{
	m_GameScene->updateCurrentCash(m_LoanAmout);
}


