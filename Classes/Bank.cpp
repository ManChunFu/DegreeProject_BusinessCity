#include "Bank.h"
#include "GameScene.h"
#include "GameData.h"
#include "GameFunctions.h"
#include <ui/UITextField.h>
#include "MouseOverMenuItem.h"
#include "ui/UIWidget.h"
#include "Player.h"
#include "Shop.h"


USING_NS_CC;

Bank::~Bank()
{
	for (auto element : m_Elements)
	{
		m_GameScene->removeChild(element);
	}
	m_Elements.clear();
	m_GameScene = nullptr;
	m_Player = nullptr;
	m_BankPanel = nullptr;
	m_Weeks = nullptr;
	m_Shop = nullptr;
	m_Electricity = nullptr;
	m_Water = nullptr;
	m_Salary = nullptr;
	m_PaybackWeekly = nullptr;
	m_Commercial = nullptr;
	m_Sales = nullptr;
	m_Total = nullptr;

	m_LoanWidget = nullptr;
	m_LoanAmoutText = nullptr;
	m_WeeklyPayText = nullptr;
	m_RepaymentText = nullptr;

	m_DisabledPanel = nullptr;
	m_DebtAmoutText = nullptr;
	m_RemainWeeksText = nullptr;

	m_BankButtons.clear();
}

void Bank::openBankPanel(GameScene* scene, unsigned currentWeek)
{
	m_CurrentWeek = currentWeek;

	if (!m_BankPanel)
	{
		m_Player = GameData::getInstance().m_Player;
		updatePlayerCurrentShopInfo();
		m_GameScene = scene;
		if (m_GameScene)
			createBankPanel();

		return;
	}

	m_BankPanel->setVisible(true);
	GameFunctions::updatLabelText_TimeFormat(m_Weeks, m_CurrentWeek);

	if (m_HasDebt)
	{
		setMenuItemsVisible(false);
		m_DisabledPanel->setVisible(true);
	}
	else
	{
		setMenuItemsVisible(true);
		m_DisabledPanel->setVisible(false);
		m_LoanWidget->setVisible(true);
	}
}

void Bank::closeBankPanel()
{
	m_BankPanel->setVisible(false);
	setMenuItemsVisible(false);
	m_LoanWidget->setVisible(false);
	m_DisabledPanel->setVisible(false);
}

void Bank::update()
{
	// player pays weekly expense
	auto amout = calculateTotalAmoutWeekly();
	if (!m_Player)
		return;

	GameData::getInstance().m_Player->updateCurrentCashAmout(amout);

	if (m_Player->getCurrentCash() < 0)
	{
		m_GameScene->gameOver();
	}
	else
	{
		if (m_HasDebt)
		{
			updateDebtDisplay(-m_Principle, -1);

			if (m_Debt == 0)
				resetTakeLoan();
		}
	}
}

int Bank::calculateTotalAmoutWeekly()
{
	auto amout = m_SalesIncome - m_ElectricityFee - m_WaterFee - m_SalaryExpense - m_commercialFee;
	if (m_HasDebt)
		amout -= m_Repayments;

	return amout;
}

void Bank::updateOverviewAmout(int amout)
{
	bool isMinus = false;
	if (amout < 0)
	{
		isMinus = true;
		amout *= -1;
	}
	GameFunctions::updateLabelText_MoneyFormat(m_Total, amout, isMinus);
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

unsigned Bank::updateLabelText(cocos2d::Label* label, unsigned originValue, int newValue, unsigned minValue, unsigned maxValue)
{
	originValue += newValue;
	originValue = clampf(originValue, minValue, maxValue);
	GameFunctions::updateLabelText_MoneyFormat(label, originValue);
	return originValue;
}

void Bank::resetTakeLoan()
{
	m_HasDebt = false;
	m_Debt = 0;
	m_LoanAmout = 10000;
	GameFunctions::updateLabelText_MoneyFormat(m_LoanAmoutText, m_LoanAmout);
	m_PaybackWeeks = 5;
	calculateWeeklyRepayments();
	m_LoanWidget->setVisible(true);
	setMenuItemsVisible(true);
	GameFunctions::updateLabelText_MoneyFormat(m_PaybackWeekly, 0, true);
	updateOverviewAmout(calculateTotalAmoutWeekly());
	m_DisabledPanel->setVisible(false);
}

void Bank::takeLoan(cocos2d::Ref* pSender)
{
	GameData::getInstance().m_Player->updateCurrentCashAmout(m_LoanAmout);
	setMenuItemsVisible(false);
	m_LoanWidget->setVisible(false);

	// showing debt panel
	m_DisabledPanel->setVisible(true);
	m_HasDebt = true;
	updateDebtDisplay(m_LoanAmout, m_PaybackWeeks);
}

void Bank::calculateWeeklyRepayments()
{
	auto rate = ((float)(m_PaybackWeeks) * 2 / 100);
	m_Principle = roundf((float)(m_LoanAmout) / m_PaybackWeeks);
	m_InterestsWeekly = m_Principle * rate;
	m_Repayments = m_Principle + m_InterestsWeekly; // principle + interests

	GameFunctions::updateLabelText_MoneyFormat(m_RepaymentText, m_Repayments);
}

void Bank::updateDebtDisplay(int amout, unsigned remainWeeks)
{
	m_Debt += amout;
	if (m_Debt < 10)
		m_Debt = 0;

	GameFunctions::updateLabelText_MoneyFormat(m_DebtAmoutText, m_Debt);
	m_RepaymentRemainWeeks += remainWeeks;
	m_RemainWeeksText->setString(std::to_string(m_RepaymentRemainWeeks));

	// update weekly overview account
	GameFunctions::updateLabelText_MoneyFormat(m_PaybackWeekly, m_Repayments, true);
	
	updateOverviewAmout(calculateTotalAmoutWeekly());
}

void Bank::updatePlayerCurrentShopInfo()
{
	// to do: update to multiple shops 
	if (m_Player->m_MyShopIds.size() < 0)
		return;

	m_ShopName = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Name;
	m_ElectricityFee= GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Electricity;
	m_WaterFee = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_Water;
	m_SalaryExpense = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_TotalSalaryExpense;
	m_commercialFee = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]]->m_CommercialCost;
}

void Bank::onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event)
{}

void Bank::createBankPanel()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sceneMidPoint = Point(origin.x + (visibleSize.width * 0.5f), origin.y + (visibleSize.height * 0.5f));

	// create Bank panel
	m_BankPanel = Sprite::createWithSpriteFrameName("Bank_Panel_400.png");
	if (!m_BankPanel)
		return;

	GameFunctions::displaySprite(m_BankPanel, Vec2(sceneMidPoint), m_GameScene, 1);
	m_Elements.push_back(m_BankPanel);

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
	m_Shop = Label::createWithTTF("", "fonts/NirmalaB.ttf", 15);

	if (m_Shop)
	{
		m_Shop->setString(m_ShopName);
		m_Shop->enableGlow(Color4B::WHITE);
		GameFunctions::displayLabel(m_Shop, Color4B::BLACK, Vec2(panelMidPoint.x, panelMidPoint.y + 120.f),
			m_BankPanel, 1);

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
				m_BankPanel, 1, true, TextHAlignment::RIGHT);
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
			GameFunctions::displayLabel(m_Water, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y + 80.f), 
				m_BankPanel, 1, true, TextHAlignment::RIGHT);
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
			GameFunctions::displayLabel(m_Salary, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y + 50.f), 
				m_BankPanel, 1, true, TextHAlignment::RIGHT);
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

		m_PaybackWeekly = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_PaybackWeekly)
			GameFunctions::displayLabel(m_PaybackWeekly, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y + 20.f), 
				m_BankPanel, 1, true, TextHAlignment::RIGHT);

		// commerical
		auto commericalLabel = Label::createWithTTF("Commercial", "fonts/Nirmala.ttf", 20);
		if (commericalLabel)
			GameFunctions::displayLabel(commericalLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 220.f, panelMidPoint.y - 20.f),
				m_BankPanel, 1);

		m_Commercial = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_Commercial)
		{
			GameFunctions::updateLabelText_MoneyFormat(m_Commercial, m_commercialFee, true);
			GameFunctions::displayLabel(m_Commercial, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y - 10.f), 
				m_BankPanel, 1, true, TextHAlignment::RIGHT);
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
			GameFunctions::displayLabel(m_Sales, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y - 40.f), 
				m_BankPanel, 1, true, TextHAlignment::RIGHT);
		}

		auto totalLabel = Label::createWithTTF("TOTAL", "fonts/NirmalaB.ttf", 20);
		if (totalLabel)
			GameFunctions::displayLabel(totalLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 240.f, panelMidPoint.y - 105.f),
				m_BankPanel, 1);

		m_Total = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
		if (m_Total)
		{
			updateOverviewAmout(calculateTotalAmoutWeekly());
			GameFunctions::displayLabel(m_Total, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y - 95.f), 
				m_BankPanel, 1, true, TextHAlignment::RIGHT);
		}
	}
#pragma endregion

#pragma region Create Loan Button
	m_LoanWidget = ui::Widget::create();
	m_LoanWidget->setPosition(panelMidPoint);
	m_GameScene->addChild(m_LoanWidget, 2);
	m_Elements.push_back(m_LoanWidget);

	// loan amout
	auto loanAmoutText = Label::createWithTTF("Loan Amout", "fonts/Nirmala.ttf", 14);
	if (loanAmoutText)
		GameFunctions::displayLabel(loanAmoutText, Color4B::WHITE, Vec2(panelMidPoint.x - 175.f, panelMidPoint.y - 175.f),
			m_LoanWidget, 1);

	auto loanAmoutSprite = Sprite::createWithSpriteFrameName("Border_Brown.png");
	if (loanAmoutSprite)
	{
		GameFunctions::displaySprite(loanAmoutSprite, Vec2(panelMidPoint.x - 175.f, panelMidPoint.y - 205.f), m_LoanWidget, 1);

		//,000 label
		m_LoanAmoutText = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_LoanAmoutText)
		{
			GameFunctions::updateLabelText_MoneyFormat(m_LoanAmoutText, m_LoanAmout);
			GameFunctions::displayLabel(m_LoanAmoutText, Color4B::WHITE, Vec2(loanAmoutSprite->getContentSize().width - 15.f, loanAmoutSprite->getContentSize().height - 5.f),
				loanAmoutSprite, 1, true, TextHAlignment::RIGHT);
		}
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
		GameFunctions::displayLabel(paybackLabel, Color4B::WHITE, Vec2(panelMidPoint.x - 30.f, panelMidPoint.y - 175.f),
			m_LoanWidget, 1);

	auto paybackSprite = Sprite::createWithSpriteFrameName("Border_Brown_Square.png");
	if (paybackSprite)
	{
		GameFunctions::displaySprite(paybackSprite, Vec2(panelMidPoint.x - 60.f, panelMidPoint.y - 205.f), m_LoanWidget, 1);

		m_WeeklyPayText = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_WeeklyPayText)
		{
			m_WeeklyPayText->setString(std::to_string(m_PaybackWeeks));
			GameFunctions::displayLabel(m_WeeklyPayText, Color4B::WHITE, Vec2(paybackSprite->getContentSize().width - 15.f, paybackSprite->getContentSize().height - 5.f),
				paybackSprite, 1, true, TextHAlignment::RIGHT);
		}
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

	m_RepaymentText = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
	if (m_RepaymentText)
	{
		GameFunctions::updateLabelText_MoneyFormat(m_RepaymentText, m_Repayments);
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

#pragma region Create debt panel
	// create disabled panel
	m_DisabledPanel = Sprite::createWithSpriteFrameName("LongPanelBlack50.png");
	if (m_DisabledPanel)
	{
		GameFunctions::displaySprite(m_DisabledPanel, Vec2(sceneMidPoint.x, sceneMidPoint.y - 162.f), m_GameScene, 1,
			0.98f, 0.96f);
		m_DisabledPanel->setVisible(false);
		m_Elements.push_back(m_DisabledPanel);
	}

	auto disableMidPoint = Vec2(m_DisabledPanel->getContentSize().width * 0.5f, m_DisabledPanel->getContentSize().height * 0.5f);

	auto debtLabel = Label::createWithTTF("Total Debt", "fonts/Nirmala.ttf", 14);
	if (debtLabel)
		GameFunctions::displayLabel(debtLabel, Color4B::WHITE, Vec2(disableMidPoint.x - 215.f, disableMidPoint.y + 28.f),
			m_DisabledPanel, 1);

	auto debtAmoutSprite = Sprite::createWithSpriteFrameName("Border_Brown.png");
	if (debtAmoutSprite)
	{
		GameFunctions::displaySprite(debtAmoutSprite, Vec2(disableMidPoint.x - 215.f, disableMidPoint.y - 5.f), m_DisabledPanel, 1);

		m_DebtAmoutText = Label::createWithTTF("10,000", "fonts/NirmalaB.ttf", 20);
		if (m_DebtAmoutText)
		{
			GameFunctions::displayLabel(m_DebtAmoutText, GameData::getInstance().m_ColorType.Taro, Vec2(debtAmoutSprite->getContentSize().width - 15.f, debtAmoutSprite->getContentSize().height - 5.f),
				debtAmoutSprite, 1, true, TextHAlignment::RIGHT);
			m_DebtAmoutText->enableShadow(Color4B::BLACK);
		}
	}

	auto remainWeeksLabel = Label::createWithTTF("Remain Weeks", "fonts/Nirmala.ttf", 14);
	if (remainWeeksLabel)
		GameFunctions::displayLabel(remainWeeksLabel, Color4B::WHITE, Vec2(disableMidPoint.x - 70.f, disableMidPoint.y + 28.f),
			m_DisabledPanel, 1);

	auto remainWeeksSprite = Sprite::createWithSpriteFrameName("Border_Brown_Square.png");
	if (remainWeeksSprite)
	{
		GameFunctions::displaySprite(remainWeeksSprite, Vec2(disableMidPoint.x - 100.f, disableMidPoint.y - 5.f), m_DisabledPanel, 1);

		//,000 label
		m_RemainWeeksText = Label::createWithTTF("5", "fonts/NirmalaB.ttf", 20);
		if (m_RemainWeeksText)
		{
			GameFunctions::displayLabel(m_RemainWeeksText, GameData::getInstance().m_ColorType.Taro, Vec2(remainWeeksSprite->getContentSize().width - 15.f, remainWeeksSprite->getContentSize().height - 5.f),
				remainWeeksSprite, 1, true, TextHAlignment::RIGHT);
			m_RemainWeeksText->enableShadow(Color4B::BLACK);
		}
	}
#pragma endregion

	auto menu = Menu::createWithArray(m_BankButtons);
	menu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(menu, 3);
	m_Elements.push_back(menu);

}

void Bank::setMenuItemsVisible(bool visible)
{
	for (auto item : m_BankButtons)
	{
		item->setEnabled(visible);
		item->setVisible(visible);
	}
}