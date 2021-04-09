#include "Bank.h"
#include "GameScene.h"
#include "GameData.h"
#include "GameFunctions.h"
#include <ui/UITextField.h>
#include "MouseOverMenuItem.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "Player.h"
#include "Shop.h"
#include "GlobalTime.h"
#include "GameTime.h"
#include "TotalExpense.h"
#include "BasicExpenseData.h"

USING_NS_CC;

Bank::Bank() 
{ 
	m_Player = GameData::getInstance().m_Player; 
	m_TotalShopExpense = new TotalExpense();
}

Bank::~Bank()
{
	delete m_TotalShopExpense;
	m_TotalShopExpense = nullptr;

	m_ShopButtons.clear();
	m_ShopWidgets.clear();

	m_Weeks = nullptr;
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
}

void Bank::openPanel(GameScene* scene, Vec2 sceneMidPoint)
{
	m_IsPanelOpen = true;
	GameData::getInstance().setTempOpenPanel(this);

	m_CurrentWeek = GameData::getInstance().m_GlobalTime->m_Gametime->week;

	if (!m_ThisPanel)
	{
		m_GameScene = scene;
		if (m_GameScene)
			createPanel(sceneMidPoint);

		return;
	}

	updatePlayerCurrentShopInfo();
	GameFunctions::updatLabelText_TimeFormat(m_Weeks, m_CurrentWeek);
	m_ThisPanel->setVisible(true);

	if (m_HasDebt)
	{
		setMenuItemsVisible(true, false, itemTypes::DEFAULT);
		m_DisabledPanel->setVisible(true);
	}
	else
	{
		setMenuItemsVisible(true, true);
		m_DisabledPanel->setVisible(false);
		m_LoanWidget->setVisible(true);
	}
}

void Bank::closePanel()
{
	m_ThisPanel->setVisible(false);
	setMenuItemsVisible(false, true);
	m_LoanWidget->setVisible(false);
	m_DisabledPanel->setVisible(false);
	
	m_IsPanelOpen = false;
	GameData::getInstance().m_TempOpenPanel = nullptr;
}

void Bank::update()
{
	// player pays weekly expense
	if (!m_Player->m_MyShopIds.size() < 0)
		return;

	updatePlayerCurrentShopInfo();

	auto amout = calculateTotalAmoutWeekly(true);
	GameData::getInstance().m_Player->updateCurrentCashAmout(amout);

	if (m_Player->getCurrentCash() < 0)
	{
		m_GameScene->gameOver();
		return;
	}

	if (m_HasDebt)
	{
		updateDebtDisplay(-m_Principle, -1);

		if (m_Debt == 0)
			resetTakeLoan();
	}
}

void Bank::createPanel(cocos2d::Vec2 sceneMidPoint, unsigned shopId)
{
	// create Bank panel
	m_ThisPanel = Sprite::createWithSpriteFrameName("Bank_Panel_420_SquareCorner.png");
	if (!m_ThisPanel)
		return;

	GameFunctions::displaySprite(m_ThisPanel, Vec2(sceneMidPoint), m_GameScene, 1);
	m_Elements.pushBack(m_ThisPanel);

	auto panelMidPoint = Vec2(m_ThisPanel->getContentSize().width * 0.5f, m_ThisPanel->getContentSize().height * 0.5f);

	// create close button
	auto closePanelButton = MouseOverMenuItem::creatMouseOverMenuButton("CloseButton_Normal.png", "CloseButton_Lit.png", "CloseButton_Disable.png",
		CC_CALLBACK_1(Bank::closeCallback, this));
	if (closePanelButton)
	{
		m_MenuItems.pushBack(displayMenuButton(closePanelButton, CC_CALLBACK_2(Bank::onMouseOver, this), 
			Vec2(sceneMidPoint.x + 272.f, sceneMidPoint.y + 185.f), itemTypes::DEFAULT, 0.7f));
	}

#pragma region create account balance label, week label and week count
	auto weeklyOverviewLabel = Label::createWithTTF("WEEKLY OVERVIEW", "fonts/NirmalaB.ttf", 20);
	if (weeklyOverviewLabel)
	{
		GameFunctions::displayLabel(weeklyOverviewLabel, GameData::getInstance().m_ColorType.Taro,
			Vec2(panelMidPoint.x - 180.f, panelMidPoint.y + 160.f), m_ThisPanel, 1);
		weeklyOverviewLabel->enableOutline(Color4B::WHITE);
		weeklyOverviewLabel->enableShadow(Color4B::BLACK);
	}

	auto weekPos = Vec2(panelMidPoint.x - 20.f, panelMidPoint.y + 160.f);

	auto weekLabel = Label::createWithTTF("WEEK", "fonts/NirmalaB.ttf", 25);
	if (weekLabel)
	{
		GameFunctions::displayLabel(weekLabel, Color4B::WHITE, weekPos, m_ThisPanel, 1);
		weekLabel->enableShadow(Color4B::BLACK);
	}

	m_Weeks = Label::createWithTTF("", "fonts/NirmalaB.ttf", 25);
	if (m_Weeks)
	{
		GameFunctions::updatLabelText_TimeFormat(m_Weeks, m_CurrentWeek);
		m_Weeks->enableOutline(Color4B::WHITE);
		GameFunctions::displayLabel(m_Weeks, Color4B::BLACK, Vec2(weekPos.x + 70.f, weekPos.y), m_ThisPanel, 1);
	}
#pragma endregion

#pragma region GetDataFromPlayerFirstTime
	auto shops = GameData::getInstance().m_Shops;
	auto shopsCount = m_Player->m_MyShopIds.size();
	auto shopIds = m_Player->m_MyShopIds;
	for (unsigned index = 0; index < shopsCount; index++)
	{
		auto shop = shops[shopIds[index]];
		m_TotalShopExpense->m_ShopsBasicExpense.push_back(new BasicExpenseData(shop->m_ShopId, shop->m_Electricity, shop->m_Water,
			shop->getTotalSalaryExpense(), shop->m_CommercialCost, shop->m_SalesIncome));
		shop->onSalesIncomeChanges = CC_CALLBACK_2(Bank::onSalesIncomeChanges, this);
	}
#pragma endregion

#pragma region Create weekly expense
	auto totalLabel = Label::createWithTTF("Total", "fonts/NirmalaB.ttf", 15);

	if (totalLabel)
	{
		totalLabel->enableGlow(Color4B::WHITE);
		GameFunctions::displayLabel(totalLabel, Color4B::BLACK, Vec2(panelMidPoint.x, panelMidPoint.y + 120.f),
			m_ThisPanel, 1);

		// electricity
		auto electricityLabel = Label::createWithTTF("Electricity", "fonts/Nirmala.ttf", 20);
		if (electricityLabel)
			GameFunctions::displayLabel(electricityLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 230.f, panelMidPoint.y + 100.f),
				m_ThisPanel, 1);

		m_Electricity = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_Electricity)
		{
			GameFunctions::updateLabelText_MoneyFormat(m_Electricity, m_TotalShopExpense->getTotalElCosts(), true);
			GameFunctions::displayLabel(m_Electricity, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y + 110.f),
				m_ThisPanel, 1, true, TextHAlignment::RIGHT);
		}

		// water
		auto waterLabel = Label::createWithTTF("Water", "fonts/Nirmala.ttf", 20);
		if (waterLabel)
			GameFunctions::displayLabel(waterLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 245.f, panelMidPoint.y + 70.f),
				m_ThisPanel, 1);

		m_Water = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_Water)
		{
			GameFunctions::updateLabelText_MoneyFormat(m_Water, m_TotalShopExpense->getTotalWaterCosts(), true);
			GameFunctions::displayLabel(m_Water, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y + 80.f),
				m_ThisPanel, 1, true, TextHAlignment::RIGHT);
		}

		// salary
		auto salaryLabel = Label::createWithTTF("Employee Salary", "fonts/Nirmala.ttf", 20);
		if (salaryLabel)
			GameFunctions::displayLabel(salaryLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 200.f, panelMidPoint.y + 40.f),
				m_ThisPanel, 1);

		m_Salary = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_Salary)
		{
			GameFunctions::updateLabelText_MoneyFormat(m_Salary, m_TotalShopExpense->getTotalSalaryCosts(), true);
			GameFunctions::displayLabel(m_Salary, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y + 50.f),
				m_ThisPanel, 1, true, TextHAlignment::RIGHT);
		}

		// commerical
		auto commericalLabel = Label::createWithTTF("Commercial", "fonts/Nirmala.ttf", 20);
		if (commericalLabel)
			GameFunctions::displayLabel(commericalLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 220.f, panelMidPoint.y + 10.f),
				m_ThisPanel, 1);

		m_Commercial = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_Commercial)
		{
			GameFunctions::updateLabelText_MoneyFormat(m_Commercial, m_TotalShopExpense->getCommercialCosts(), true);
			GameFunctions::displayLabel(m_Commercial, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y + 20.f),
				m_ThisPanel, 1, true, TextHAlignment::RIGHT);
		}

		// sales
		auto salesLabel = Label::createWithTTF("Sales Income", "fonts/Nirmala.ttf", 20);
		if (salesLabel)
			GameFunctions::displayLabel(salesLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 215.f, panelMidPoint.y - 20.f),
				m_ThisPanel, 1);

		m_Sales = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_Sales)
		{
			GameFunctions::updateLabelText_MoneyFormat(m_Sales, m_TotalShopExpense->getTotalSalesIncome());
			GameFunctions::displayLabel(m_Sales, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y - 10.f),
				m_ThisPanel, 1, true, TextHAlignment::RIGHT);
		}

		// loan
		auto loanLabel = Label::createWithTTF("Loan", "fonts/Nirmala.ttf", 20);
		if (loanLabel)
			GameFunctions::displayLabel(loanLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 250.f, panelMidPoint.y - 50.f),
				m_ThisPanel, 1);

		auto loanDetailLabel = Label::createWithTTF("(weekly repayments)", "fonts/Nirmala.ttf", 15);
		if (loanDetailLabel)
			GameFunctions::displayLabel(loanDetailLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 160.f, panelMidPoint.y -52.f),
				m_ThisPanel, 1);

		m_PaybackWeekly = Label::createWithTTF("--", "fonts/Nirmala.ttf", 20);
		if (m_PaybackWeekly)
			GameFunctions::displayLabel(m_PaybackWeekly, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y - 40.f),
				m_ThisPanel, 1, true, TextHAlignment::RIGHT);

		auto totalLabel = Label::createWithTTF("TOTAL", "fonts/NirmalaB.ttf", 20);
		if (totalLabel)
			GameFunctions::displayLabel(totalLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 240.f, panelMidPoint.y - 105.f),
				m_ThisPanel, 1);

		m_Total = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
		if (m_Total)
		{
			updateOverviewAmout(m_Total, calculateTotalAmoutWeekly());
			GameFunctions::displayLabel(m_Total, Color4B::BLACK, Vec2(panelMidPoint.x + 20.f, panelMidPoint.y - 95.f),
				m_ThisPanel, 1, true, TextHAlignment::RIGHT);
		}
	}

	// create new shop balance
	m_NewShopPos = Vec2(panelMidPoint.x + 245.f, panelMidPoint.y + 118.f);
	for (unsigned index = 0; index < m_TotalShopExpense->m_ShopsBasicExpense.size(); index++)
	{
		createNewShopBalance(index, panelMidPoint);
	}

#pragma endregion

#pragma region Create Loan Button
	m_LoanWidget = ui::Widget::create();
	m_LoanWidget->setPosition(Vec2(panelMidPoint.x - 340.f, panelMidPoint.y - 175.f));
	m_ThisPanel->addChild(m_LoanWidget, 1);

	// loan amout
	auto loanAmoutText = Label::createWithTTF("Loan Amout", "fonts/Nirmala.ttf", 14);
	if (loanAmoutText)
		GameFunctions::displayLabel(loanAmoutText, Color4B::WHITE, Vec2(panelMidPoint.x - 175.f, panelMidPoint.y - 175.f),
			m_LoanWidget, 1);

	auto loanAmoutSprite = Sprite::createWithSpriteFrameName("Border_Brown.png");
	if (loanAmoutSprite)
	{
		GameFunctions::displaySprite(loanAmoutSprite, Vec2(panelMidPoint.x - 175.f, panelMidPoint.y - 205.f), m_LoanWidget, 1);

		m_LoanAmoutText = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_LoanAmoutText)
		{
			GameFunctions::updateLabelText_MoneyFormat(m_LoanAmoutText, m_LoanAmout);
			GameFunctions::displayLabel(m_LoanAmoutText, Color4B::WHITE, Vec2(loanAmoutSprite->getContentSize().width - 15.f, loanAmoutSprite->getContentSize().height - 5.f),
				loanAmoutSprite, 1, true, TextHAlignment::RIGHT);
		}
	}

	auto reduceAmoutButton = MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(Bank::reduceAmoutCallback, this));
	if (reduceAmoutButton)
	{
		m_MenuItems.pushBack(displayMenuButton(reduceAmoutButton, CC_CALLBACK_2(Bank::onMouseOver, this),
			Vec2(sceneMidPoint.x - 175.f, sceneMidPoint.y - 180.f), itemTypes::BUTTON, 0.5f));
	}

	auto addAmoutButton = MouseOverMenuItem::createUpperButton(CC_CALLBACK_1(Bank::addAmoutCallback, this));
	if (addAmoutButton)
	{
		m_MenuItems.pushBack(displayMenuButton(addAmoutButton, CC_CALLBACK_2(Bank::onMouseOver, this),
			Vec2(sceneMidPoint.x - 255.f, sceneMidPoint.y - 160.f), itemTypes::BUTTON, 0.5f));
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

	auto reduceWeekButton = MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(Bank::reduceWeekCallback, this));
	if (reduceWeekButton)
	{
		m_MenuItems.pushBack(displayMenuButton(reduceWeekButton, CC_CALLBACK_2(Bank::onMouseOver, this),
			Vec2(sceneMidPoint.x - 85.f, sceneMidPoint.y - 180.f), itemTypes::BUTTON, 0.5f));
	}

	auto addWeekButton = MouseOverMenuItem::createUpperButton(CC_CALLBACK_1(Bank::addWeekCallback, this));
	if (addWeekButton)
	{
		m_MenuItems.pushBack(displayMenuButton(addWeekButton, CC_CALLBACK_2(Bank::onMouseOver, this),
			Vec2(sceneMidPoint.x - 115.f, sceneMidPoint.y - 160.f), itemTypes::BUTTON, 0.5f));
	}

	auto totalWeeksLabel = Label::createWithTTF("Weeks", "fonts/Nirmala.ttf", 20);
	if (totalWeeksLabel)
		GameFunctions::displayLabel(totalWeeksLabel, Color4B::WHITE, Vec2(panelMidPoint.x - 40.f, panelMidPoint.y - 170.f),
			m_ThisPanel, 1);

	// weekly repayments
	auto repaymentLabel = Label::createWithTTF("Weekly Repayments", "fonts/Nirmala.ttf", 14);
	if (repaymentLabel)
		GameFunctions::displayLabel(repaymentLabel, Color4B::WHITE, Vec2(panelMidPoint.x + 90.f, panelMidPoint.y - 140.f),
			m_ThisPanel, 1);

	auto cashSymbol = Label::createWithTTF("$", "fonts/Nirmala.ttf", 20);
	if (cashSymbol)
	{
		GameFunctions::displayLabel(cashSymbol, GameData::getInstance().m_ColorType.Taro, Vec2(panelMidPoint.x + 40.f, 
			panelMidPoint.y - 170.f), m_ThisPanel, 1);
		cashSymbol->enableShadow(Color4B::BLACK);
	}

	m_RepaymentText = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
	if (m_RepaymentText)
	{
		GameFunctions::updateLabelText_MoneyFormat(m_RepaymentText, m_Repayments);
		GameFunctions::displayLabel(m_RepaymentText, GameData::getInstance().m_ColorType.Taro, Vec2(panelMidPoint.x + 90.f, 
			panelMidPoint.y - 170.f), m_ThisPanel, 1);
		m_RepaymentText->enableShadow(Color4B::BLACK);
	}

	// apply loan button
	auto applyLoanButton = MouseOverMenuItem::creatMouseOverMenuButton("Button100.png", "Button100_Lit.png", "Button100_Disabled.png",
		CC_CALLBACK_1(Bank::takeLoan, this));
	if (applyLoanButton)
	{
		m_MenuItems.pushBack(displayMenuButton(applyLoanButton, CC_CALLBACK_2(Bank::onMouseOver, this),
			Vec2(sceneMidPoint.x + 220.f, sceneMidPoint.y - 170.f), itemTypes::BUTTON));

		auto takeLoanText = Label::createWithTTF("TAKE LOAN", "fonts/NirmalaB.ttf", 14);
		if (takeLoanText)
		{
			GameFunctions::displayLabel(takeLoanText, GameData::getInstance().m_ColorType.Taro,
				Vec2(applyLoanButton->getContentSize().width * 0.5f, applyLoanButton->getContentSize().height * 0.5f),
				applyLoanButton, 1);
			takeLoanText->enableOutline(Color4B::BLACK);
		}
	}

#pragma endregion

#pragma region Create debt panel
	// create disabled panel
	m_DisabledPanel = Sprite::createWithSpriteFrameName("LongPanelBlack80.png");
	auto disableMidPoint = Vec2(m_DisabledPanel->getContentSize().width * 0.5f, m_DisabledPanel->getContentSize().height * 0.5f);

	if (m_DisabledPanel)
	{
		GameFunctions::displaySprite(m_DisabledPanel, Vec2(Vec2(panelMidPoint.x, panelMidPoint.y - 167.f)), m_ThisPanel,
			1, 0.98f, 1.f);
		m_DisabledPanel->setVisible(false);
	}

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

	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(menu, 3);
	m_Elements.pushBack(menu);
}

void Bank::createNewShopBalance(unsigned shopIndex, cocos2d::Vec2 panelMidPoint)
{
	auto shopName = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[shopIndex]]->m_Name;

	// create shop balance button
	auto newShopButton = ui::CheckBox::create("Sprites/Checkboxes/Button_Red.png", "Sprites/Checkboxes/Button_Selected.png");
	if (newShopButton)
	{
		newShopButton->setPosition(m_NewShopPos);
		newShopButton->addClickEventListener(CC_CALLBACK_1(Bank::onOpenBalanceCallback, this, shopIndex));
		m_ThisPanel->addChild(newShopButton, 2);
		m_ShopButtons.push_back(newShopButton);

		auto shopNameText = Label::createWithTTF(shopName, "fonts/NirmalaB.ttf", 11);
		if (shopNameText)
			GameFunctions::displayLabel(shopNameText, Color4B::WHITE, newShopButton->getContentSize() * 0.5f, newShopButton, 1);
	}

	// for next shop pos
	m_NewShopPos.y -= 35.f;

	// create shop widget
	auto newShopBalanceWidget = ui::Widget::create();
	newShopBalanceWidget->setPosition(Vec2::ZERO);
	m_ThisPanel->addChild(newShopBalanceWidget, 1);
	m_ShopWidgets.pushBack(newShopBalanceWidget);
	newShopBalanceWidget->setVisible(false);

	// create widget background
	auto backgroundWidget = Sprite::createWithSpriteFrameName("WidgetBG_Brown.png");
	if (backgroundWidget)
		GameFunctions::displaySprite(backgroundWidget, Vec2(panelMidPoint.x +170.f, panelMidPoint.y + 6.f), newShopBalanceWidget, 0);

#pragma region Create weekly expense
	auto shopBasicExpense = m_TotalShopExpense->m_ShopsBasicExpense[shopIndex];
	auto shopExpensePos = Vec2(panelMidPoint.x + 130.f, panelMidPoint.y + 120.f);
	auto shop = Label::createWithTTF("", "fonts/NirmalaB.ttf", 15);
	if (shop)
	{
		shop->setString(shopName);
		shop->enableGlow(Color4B::BLACK);
		GameFunctions::displayLabel(shop, Color4B::WHITE, Vec2(shopExpensePos.x -10.f, shopExpensePos.y), newShopBalanceWidget, 1);

		// electricity
		auto electricityLabel = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (electricityLabel)
		{
			GameFunctions::updateLabelText_MoneyFormat(electricityLabel, shopBasicExpense->m_EletricityCost, true);
			GameFunctions::displayLabel(electricityLabel, Color4B::WHITE, Vec2(shopExpensePos.x, shopExpensePos.y - 10.f),
				newShopBalanceWidget, 1, true, TextHAlignment::RIGHT);
			m_TotalShopExpense->m_ShopsBasicExpense[shopIndex]->m_ElLabel = electricityLabel;
		}

		// water
		auto waterLabel = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (waterLabel)
		{
			GameFunctions::updateLabelText_MoneyFormat(waterLabel, shopBasicExpense->m_WaterCost, true);
			GameFunctions::displayLabel(waterLabel, Color4B::WHITE, Vec2(shopExpensePos.x, shopExpensePos.y -40.f),
				newShopBalanceWidget, 1, true, TextHAlignment::RIGHT);

			m_TotalShopExpense->m_ShopsBasicExpense[shopIndex]->m_WaterLabel = waterLabel;
		}

		// salary
		auto salaryLabel = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (salaryLabel)
		{
			GameFunctions::updateLabelText_MoneyFormat(salaryLabel, shopBasicExpense->m_SalaryCost, true);
			GameFunctions::displayLabel(salaryLabel, Color4B::WHITE, Vec2(shopExpensePos.x, shopExpensePos.y - 70.f),
				newShopBalanceWidget, 1, true, TextHAlignment::RIGHT);

			m_TotalShopExpense->m_ShopsBasicExpense[shopIndex]->m_SalaryLabel = salaryLabel;
		}

		// commerical
		auto commercialLabel = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (commercialLabel)
		{
			GameFunctions::updateLabelText_MoneyFormat(commercialLabel, shopBasicExpense->m_ADCost, true);
			GameFunctions::displayLabel(commercialLabel, Color4B::WHITE, Vec2(shopExpensePos.x, shopExpensePos.y - 100.f),
				newShopBalanceWidget, 1, true, TextHAlignment::RIGHT);

			m_TotalShopExpense->m_ShopsBasicExpense[shopIndex]->m_ADLabel = commercialLabel;
		}

		// sales
		auto salesLabel = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (salesLabel)
		{
			GameFunctions::updateLabelText_MoneyFormat(salesLabel, shopBasicExpense->m_SalesIncome);
			GameFunctions::displayLabel(salesLabel, Color4B::WHITE, Vec2(shopExpensePos.x, shopExpensePos.y - 130.f),
				newShopBalanceWidget, 1, true, TextHAlignment::RIGHT);

			m_TotalShopExpense->m_ShopsBasicExpense[shopIndex]->m_SalesLabel = salesLabel;
		}

		// total
		auto totalLabel = Label::createWithTTF("", "fonts/NirmalaB.ttf", 20);
		if (totalLabel)
		{
			updateOverviewAmout(totalLabel, shopBasicExpense->getBalance());
			GameFunctions::displayLabel(totalLabel, Color4B::WHITE, Vec2(shopExpensePos.x, shopExpensePos.y - 215.f),
				newShopBalanceWidget, 1, true, TextHAlignment::RIGHT);

			m_TotalShopExpense->m_ShopsBasicExpense[shopIndex]->m_TotalLabel = totalLabel;
		}
	}
#pragma endregion
}

void Bank::closeCallback(cocos2d::Ref* pSedner)
{
	closePanel();
}

void Bank::onOpenBalanceCallback(cocos2d::Ref* pSender, unsigned shopWidgetIndex)
{
	for (unsigned index = 0; index < m_ShopWidgets.size(); index++)
	{
		if (index == shopWidgetIndex)
		{
			(m_ShopWidgets.at(index)->isVisible()) ? m_ShopWidgets.at(index)->setVisible(false) :
				m_ShopWidgets.at(index)->setVisible(true);

			continue;
		}
		m_ShopWidgets.at(index)->setVisible(false);
		m_ShopButtons.at(index)->setSelected(false);
	}
}

int Bank::calculateTotalAmoutWeekly(bool removeSalesIncome)
{
	auto amout = removeSalesIncome ? m_TotalShopExpense->getTotalCosts() : m_TotalShopExpense->getTotalBalance();

	if (m_HasDebt)
		amout -= m_Repayments;

	return amout;
}

void Bank::updateOverviewAmout(Label* total, int amout)
{
	bool isMinus = false;
	if (amout < 0)
	{
		isMinus = true;
		amout *= -1;
	}
	GameFunctions::updateLabelText_MoneyFormat(total, amout, isMinus);
}

void Bank::addAmoutCallback(cocos2d::Ref* pSender)
{
	m_LoanAmout = GameFunctions::displayLabelText_ClampValue(m_LoanAmoutText, m_LoanAmout, 10000, 10000, 100000);
	calculateWeeklyRepayments();
}

void Bank::reduceAmoutCallback(cocos2d::Ref* pSender)
{
	m_LoanAmout = GameFunctions::displayLabelText_ClampValue(m_LoanAmoutText, m_LoanAmout, -10000, 10000, 100000);
	calculateWeeklyRepayments();
}

void Bank::addWeekCallback(cocos2d::Ref* pSender)
{
	m_PaybackWeeks = GameFunctions::displayLabelText_ClampValue(m_WeeklyPayText, m_PaybackWeeks, 1, 5, 30);
	calculateWeeklyRepayments();
}

void Bank::reduceWeekCallback(cocos2d::Ref* pSender)
{
	m_PaybackWeeks = GameFunctions::displayLabelText_ClampValue(m_WeeklyPayText, m_PaybackWeeks, -1, 5, 30);
	calculateWeeklyRepayments();
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
	setMenuItemsVisible(true, true);
	GameFunctions::updateLabelText_MoneyFormat(m_PaybackWeekly, 0, true);
	updateOverviewAmout(m_Total, calculateTotalAmoutWeekly());
	m_DisabledPanel->setVisible(false);
}

void Bank::takeLoan(cocos2d::Ref* pSender)
{
	GameData::getInstance().m_Player->updateCurrentCashAmout(m_LoanAmout);
	setMenuItemsVisible(false, false, itemTypes::BUTTON);
	m_LoanWidget->setVisible(false);

	// showing debt panel
	m_DisabledPanel->setVisible(true);
	m_HasDebt = true;
	updateDebtDisplay(m_LoanAmout, m_PaybackWeeks);
}

void Bank::onSalesIncomeChanges(unsigned shopId, unsigned totalSales)
{
	for (auto shop : m_TotalShopExpense->m_ShopsBasicExpense)
	{
		if (shop->m_ShopId == shopId)
		{
			shop->m_SalesIncome = totalSales;
			GameFunctions::updateLabelText_MoneyFormat(shop->m_SalesLabel, totalSales);
			updateOverviewAmout(shop->m_TotalLabel, shop->getBalance());
		}
	}
	GameFunctions::updateLabelText_MoneyFormat(m_Sales, m_TotalShopExpense->getTotalSalesIncome());
	updateOverviewAmout(m_Total, m_TotalShopExpense->getTotalBalance());
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

	updateOverviewAmout(m_Total, calculateTotalAmoutWeekly());
}

void Bank::updatePlayerCurrentShopInfo()
{
	auto myShopsSize = m_Player->m_MyShopIds.size();
	if (myShopsSize < 0)
		return;

	auto myShopsIds = m_Player->m_MyShopIds;
	auto shops = GameData::getInstance().m_Shops;
	auto shopsExpenseSize = m_TotalShopExpense->m_ShopsBasicExpense.size();
	auto shopIndex = 0;
	if (myShopsSize > shopsExpenseSize)
		shopIndex = shopsExpenseSize - 1;
	else
		shopIndex = myShopsSize;

	for (unsigned index = 0; index < myShopsSize; index++)
	{
		auto shop = shops[myShopsIds[index]];
		if (index <= shopIndex)
		{
			auto shopBasicExpense = m_TotalShopExpense->m_ShopsBasicExpense[index];
			// update value
			shopBasicExpense->m_EletricityCost = shop->m_Electricity;
			shopBasicExpense->m_WaterCost = shop->m_Water;
			shopBasicExpense->m_SalaryCost = shop->getTotalSalaryExpense();
			shopBasicExpense->m_ADCost = shop->m_CommercialCost;
			shopBasicExpense->m_SalesIncome = shop->m_SalesIncome;

			// update text
			GameFunctions::updateLabelText_MoneyFormat(shopBasicExpense->m_ElLabel, shopBasicExpense->m_EletricityCost, true);
			GameFunctions::updateLabelText_MoneyFormat(shopBasicExpense->m_WaterLabel, shopBasicExpense->m_WaterCost, true);
			GameFunctions::updateLabelText_MoneyFormat(shopBasicExpense->m_SalaryLabel, shopBasicExpense->m_SalaryCost, true);
			GameFunctions::updateLabelText_MoneyFormat(shopBasicExpense->m_ADLabel, shopBasicExpense->m_ADCost, true);
			GameFunctions::updateLabelText_MoneyFormat(shopBasicExpense->m_SalesLabel, shopBasicExpense->m_SalesIncome);
			updateOverviewAmout(shopBasicExpense->m_TotalLabel, shopBasicExpense->getBalance());
			continue;
		}

		// add new
		m_TotalShopExpense->m_ShopsBasicExpense.push_back(new BasicExpenseData(shop->m_ShopId, shop->m_Electricity, shop->m_Water, 
			shop->getTotalSalaryExpense(), shop->m_CommercialCost, shop->m_SalesIncome));

		shop->onSalesIncomeChanges = CC_CALLBACK_2(Bank::onSalesIncomeChanges, this);

		createNewShopBalance(index, m_ThisPanel->getContentSize() * 0.5f);
	}
	
	// update total
	GameFunctions::updateLabelText_MoneyFormat(m_Electricity, m_TotalShopExpense->getTotalElCosts(), true);
	GameFunctions::updateLabelText_MoneyFormat(m_Water, m_TotalShopExpense->getTotalWaterCosts(), true);
	GameFunctions::updateLabelText_MoneyFormat(m_Salary, m_TotalShopExpense->getTotalSalaryCosts(), true);
	GameFunctions::updateLabelText_MoneyFormat(m_Commercial, m_TotalShopExpense->getCommercialCosts(), true);
	GameFunctions::updateLabelText_MoneyFormat(m_Sales, m_TotalShopExpense->getTotalSalesIncome());
	updateOverviewAmout(m_Total, m_TotalShopExpense->getTotalBalance());
}

void Bank::onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event)
{}

void Bank::setMenuItemsVisible(bool visible, bool runAll, itemTypes Type)
{
	if (runAll)
	{
		for (auto item : m_MenuItems)
		{
			item->setEnabled(visible);
			item->setVisible(visible);
		}
		return;
	}

	for (auto item : m_MenuItems)
	{
		if (item->itemSelectedData.type == Type)
		{
			item->setEnabled(visible);
			item->setVisible(visible);
		}
	}
}

