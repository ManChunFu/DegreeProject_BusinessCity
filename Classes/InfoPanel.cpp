#include "InfoPanel.h"
#include "Player.h"
#include "MouseOverMenuItem.h"
#include "GameScene.h"
#include "Bank.h"
#include "GameFunctions.h"
#include "GameData.h"
#include "GlobalTime.h"
#include "GameTime.h"


USING_NS_CC;


InfoPanel::~InfoPanel()
{
	m_Saving = nullptr;
	m_WeekCount = nullptr;
	m_WeekDay = nullptr;
	m_TimeHourDisplay = nullptr;
	m_TimeMinDisplay = nullptr;
	m_BankButton = nullptr;
	delete m_Bank;
	m_Bank = nullptr;
}

void InfoPanel::openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint)
{
	m_GameScene = scene;
	m_SceneMidPoint = sceneMidPoint;

	m_Player = GameData::getInstance().m_Player;
	m_Player->onCashAmoutChange = CC_CALLBACK_2(InfoPanel::onCurrentCashChange, this);

	auto globalTime = GameData::getInstance().m_GlobalTime;
	globalTime->addMinuteEventListener(CC_CALLBACK_2(InfoPanel::onEveryMinuteChanges, this));
	globalTime->onEveryHourChanges = CC_CALLBACK_2(InfoPanel::onEveryHourChanges, this);
	globalTime->onEveryDayChanges = CC_CALLBACK_2(InfoPanel::onEveryDayChanges, this);
	globalTime->onEveryWeekChanges = CC_CALLBACK_2(InfoPanel::onEveryWeekChanges, this);


	m_ThisPanel = Sprite::createWithSpriteFrameName("InGamePanel_Black_80.png");
	if (!m_ThisPanel)
		return;

	GameFunctions::displaySprite(m_ThisPanel, Vec2(m_SceneMidPoint.x - 160.f, m_SceneMidPoint.y + 320.f), m_GameScene, 1);
	m_Elements.pushBack(m_ThisPanel);

	auto topPanelMidPoint = Vec2(m_ThisPanel->getContentSize().width * 0.5f, m_ThisPanel->getContentSize().height * 0.5f);

	auto playerSprite = Sprite::createWithSpriteFrameName(GameData::getInstance().getPlayerCharacter(m_Player->getCharacter()));
	if (!playerSprite)
		return;

	GameFunctions::displaySprite(playerSprite, Vec2(topPanelMidPoint.x - 250.f, topPanelMidPoint.y - 5.f), m_ThisPanel, 1, 0.4f, 0.4f);

	auto nameLabel = Label::createWithTTF(m_Player->getName(), "fonts/NirmalaB.ttf", 22);
	if (nameLabel)
	{
		nameLabel->setMaxLineWidth(125);
		nameLabel->enableWrap(true);
		nameLabel->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
		GameFunctions::displayLabel(nameLabel, Color4B::WHITE, Vec2(playerSprite->getPositionX() + 100.f, playerSprite->getPositionY()),
			m_ThisPanel, 1);
	}

#pragma region CreateSavingLabels and Bank 
	auto cashSymbol = Label::createWithTTF("$", "fonts/NirmalaB.ttf", 20);
	if (cashSymbol)
	{
		cashSymbol->enableGlow(Color4B::WHITE);
		GameFunctions::displayLabel(cashSymbol, GameData::getInstance().m_ColorType.Goldenrdo, Vec2(topPanelMidPoint.x + 350.f, topPanelMidPoint.y - 30.f),
			m_ThisPanel, 1);
	}

	m_Saving = Label::createWithTTF("", "fonts/NirmalaB.ttf", 25);
	if (m_Saving)
	{
		GameFunctions::updateLabelText_MoneyFormat(m_Saving, m_Player->getCurrentCash());
		m_Saving->enableGlow(Color4B::WHITE);
		GameFunctions::displayLabel(m_Saving, GameData::getInstance().m_ColorType.Gold,
			Vec2(topPanelMidPoint.x + 420.f, topPanelMidPoint.y - 30.f), m_ThisPanel, 1);
	}

	auto creditCard = Sprite::createWithSpriteFrameName("CreditCardBank_100.png");
	if (creditCard)
	{
		creditCard->setPosition(topPanelMidPoint.x + 370.f, topPanelMidPoint.y + 10.f);
		creditCard->setScale(0.6f);
		m_ThisPanel->addChild(creditCard, 1);
	}

	// create bank buttons
	m_BankButton = MouseOverMenuItem::creatMouseOverMenuButton("ButtonBlueNormal.png", "ButtonBlueLit.png", "ButtonBlueDisabled.png",
		CC_CALLBACK_1(InfoPanel::checkBalanceCallback, this, scene));
	if (m_BankButton)
	{
		m_BankButton->onMouseOver = CC_CALLBACK_2(InfoPanel::onMouseOver, this);
		auto bankPos = Vec2(m_ThisPanel->getPosition().x + 445.f, m_ThisPanel->getPosition().y);
		m_BankButton->setPosition(bankPos);
		m_BankButton->setScale(0.7f);
		m_BankButton->setItemRect(bankPos, 0.7f);

		auto myBankLabel = Label::createWithTTF("CHIBANK", "fonts/NirmalaB.ttf", 20);
		if (myBankLabel)
			GameFunctions::displayLabel(myBankLabel, GameData::getInstance().m_ColorType.Taro,
				Vec2(m_BankButton->getContentSize().width * 0.5f, m_BankButton->getContentSize().height * 0.5f), m_BankButton, 1);

		m_BankButton->setEnabled(false);

		m_MenuItems.pushBack(m_BankButton);

		m_Bank = new Bank();
	}
#pragma endregion

#pragma region CreateTimeLabels
	auto weekLabel = Label::createWithTTF("WEEK", "fonts/NirmalaB.ttf", 14);
	if (weekLabel)
	{
		weekLabel->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
		GameFunctions::displayLabel(weekLabel, Color4B::WHITE, Vec2(topPanelMidPoint.x + 540.f, topPanelMidPoint.y + 15.f),
			m_ThisPanel, 1);
	}

	m_WeekCount = Label::createWithTTF("", "fonts/NirmalaB.ttf", 16);
	if (m_WeekCount)
	{
		GameFunctions::updatLabelText_TimeFormat(m_WeekCount, globalTime->m_Gametime->week);
		m_WeekCount->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		GameFunctions::displayLabel(m_WeekCount, Color4B::WHITE, Vec2(topPanelMidPoint.x + 580.f, topPanelMidPoint.y + 15.f),
			m_ThisPanel, 1);
	}

	m_WeekDay = Label::createWithTTF(m_WeekDays[globalTime->m_Gametime->day], "fonts/NirmalaB.ttf", 14);
	if (m_WeekDay)
	{
		m_WeekDay->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
		GameFunctions::displayLabel(m_WeekDay, Color4B::WHITE, Vec2(topPanelMidPoint.x + 555.f, topPanelMidPoint.y - 5.f),
			m_ThisPanel, 1);
	}

	m_TimeHourDisplay = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
	if (m_TimeHourDisplay)
	{
		GameFunctions::updatLabelText_TimeFormat(m_TimeHourDisplay, globalTime->m_Gametime->hour);
		m_TimeHourDisplay->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		GameFunctions::displayLabel(m_TimeHourDisplay, Color4B::WHITE, Vec2(topPanelMidPoint.x + 530.f, topPanelMidPoint.y - 30.f),
			m_ThisPanel, 1);
	}

	auto timeMark = Label::createWithTTF(":", "fonts/NirmalaB.ttf", 30);
	if (timeMark)
	{
		timeMark->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		GameFunctions::displayLabel(timeMark, Color4B::WHITE, Vec2(topPanelMidPoint.x + 555.f, topPanelMidPoint.y - 28.f),
			m_ThisPanel, 1);
	}

	m_TimeMinDisplay = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
	if (m_TimeMinDisplay)
	{
		GameFunctions::updatLabelText_TimeFormat(m_TimeMinDisplay, globalTime->m_Gametime->minute);
		m_TimeMinDisplay->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		GameFunctions::displayLabel(m_TimeMinDisplay, Color4B::WHITE, Vec2(topPanelMidPoint.x + 580.f, topPanelMidPoint.y - 30.f),
			m_ThisPanel, 1);
	}
#pragma endregion

	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(menu, 2);
	m_Elements.pushBack(menu);

}

void InfoPanel::enableBankButton(bool value)
{
	m_BankButton->setEnabled(value);
}

void InfoPanel::onEveryMinuteChanges(GlobalTime* globalTime, unsigned minute)
{
	GameFunctions::updatLabelText_TimeFormat(m_TimeMinDisplay, minute);
}

void InfoPanel::onEveryHourChanges(GlobalTime* globalTime, unsigned hour)
{
	GameFunctions::updatLabelText_TimeFormat(m_TimeHourDisplay, hour);
}

void InfoPanel::onEveryDayChanges(GlobalTime* globalTime, unsigned day)
{
	m_WeekDay->setString(m_WeekDays[day]);
}

void InfoPanel::onEveryWeekChanges(GlobalTime* globalTime, unsigned week)
{
	GameFunctions::updatLabelText_TimeFormat(m_WeekCount, week);
	m_Bank->update();
}

void InfoPanel::checkBalanceCallback(cocos2d::Ref* pSender, GameScene* scene)
{
	(m_Bank->isPanelOpen()) ? m_Bank->closePanel() : m_Bank->openPanel(scene, m_SceneMidPoint);
}

void InfoPanel::onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event)
{
}

void InfoPanel::onCurrentCashChange(Player* player, int currentCashAmout)
{
	GameFunctions::updateLabelText_MoneyFormat(m_Saving, currentCashAmout);
}
