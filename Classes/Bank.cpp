#include "Bank.h"
#include "GameScene.h"
#include "GameData.h"
#include "GameFunctions.h"

USING_NS_CC;

Bank::~Bank()
{
	m_Weeks = nullptr;
	m_CurrentWeek = 1;
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
	m_GameScene->updateTimeLabel(m_Weeks, m_CurrentWeek);
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
	auto accountBalanceLabel = Label::createWithTTF("ACCOUNT BALANCE", "fonts/NirmalaB.ttf", 20);
	if (accountBalanceLabel)
	{
		GameFunctions::displayLabel(accountBalanceLabel, GameData::getInstance().m_ColorType.Taro, 
			Vec2(panelMidPoint.x - 180.f, panelMidPoint.y + 160.f),m_BankPanel, 1);
		accountBalanceLabel->enableOutline(Color4B::WHITE);
		accountBalanceLabel->enableShadow(Color4B::BLACK);
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
		m_GameScene->updateTimeLabel(m_Weeks, m_CurrentWeek);
		m_Weeks->enableOutline(Color4B::WHITE);
		GameFunctions::displayLabel(m_Weeks, Color4B::BLACK, Vec2(weekPos.x + 70.f, weekPos.y), m_BankPanel, 1);
	}
#pragma endregion

#pragma region Create weekly expense
	m_Shop = Label::createWithTTF("Hotdog Stand", "fonts/NirmalaB.ttf", 15);
	if (m_Shop)
	{
		m_Shop->enableGlow(Color4B::WHITE);
		GameFunctions::displayLabel(m_Shop, Color4B::BLACK, Vec2(panelMidPoint.x, panelMidPoint.y + 120.f),
			m_BankPanel, 1);
	}

	auto electricityLabel = Label::createWithTTF("Electricity", "fonts/Nirmala.ttf", 20);
	if (electricityLabel)
		GameFunctions::displayLabel(electricityLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 230.f, panelMidPoint.y + 100.f), 
			m_BankPanel, 1);

	auto waterLabel = Label::createWithTTF("Water", "fonts/Nirmala.ttf", 20);
	if (waterLabel)
		GameFunctions::displayLabel(waterLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 245.f, panelMidPoint.y + 70.f),
			m_BankPanel, 1);

	auto salaryLabel = Label::createWithTTF("Employee Salary", "fonts/Nirmala.ttf", 20);
	if (salaryLabel)
		GameFunctions::displayLabel(salaryLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 200.f, panelMidPoint.y + 40.f), 
			m_BankPanel, 1);

	auto loanLabel = Label::createWithTTF("Loan", "fonts/Nirmala.ttf", 20);
	if (loanLabel)
		GameFunctions::displayLabel(loanLabel, Color4B::BLACK, Vec2(panelMidPoint.x - 250.f, panelMidPoint.y + 10.f),
			m_BankPanel, 1);

	auto loanDetailLabel = Label::createWithTTF("(principle + interest)", "fonts/Nirmala.ttf", 15);
	if (loanDetailLabel)
		GameFunctions::displayLabel(loanDetailLabel, Color4B::BLACK, Vec2(panelMidPoint.x -160.f, panelMidPoint.y + 8.f),
			m_BankPanel, 1);
#pragma endregion



}

