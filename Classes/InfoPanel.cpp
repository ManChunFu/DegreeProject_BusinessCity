#include "InfoPanel.h"
#include "Player.h"
#include "MouseOverMenuItem.h"
#include "GameScene.h"
#include "Bank.h"
#include "GameFunctions.h"
#include "GameData.h"


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

	m_ThisPanel = Sprite::createWithSpriteFrameName("InGamePanel_Black_80.png");
	if (!m_ThisPanel)
		return;

	GameFunctions::displaySprite(m_ThisPanel, Vec2(m_SceneMidPoint.x - 160.f, m_SceneMidPoint.y + 320.f), m_GameScene, 1);
	m_Elements.push_back(m_ThisPanel);

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
		m_Bank->setMyParent(this);
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
		GameFunctions::updatLabelText_TimeFormat(m_WeekCount, m_Weeks);
		m_WeekCount->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		GameFunctions::displayLabel(m_WeekCount, Color4B::WHITE, Vec2(topPanelMidPoint.x + 580.f, topPanelMidPoint.y + 15.f),
			m_ThisPanel, 1);
	}

	m_WeekDay = Label::createWithTTF(m_WeekDays[m_Today], "fonts/NirmalaB.ttf", 14);
	if (m_WeekDay)
	{
		m_WeekDay->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
		GameFunctions::displayLabel(m_WeekDay, Color4B::WHITE, Vec2(topPanelMidPoint.x + 555.f, topPanelMidPoint.y - 5.f),
			m_ThisPanel, 1);
	}

	m_TimeHourDisplay = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
	if (m_TimeHourDisplay)
	{
		GameFunctions::updatLabelText_TimeFormat(m_TimeHourDisplay, m_CurrentHour);
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
		GameFunctions::updatLabelText_TimeFormat(m_TimeMinDisplay, m_CurrentMinute);
		m_TimeMinDisplay->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		GameFunctions::displayLabel(m_TimeMinDisplay, Color4B::WHITE, Vec2(topPanelMidPoint.x + 580.f, topPanelMidPoint.y - 30.f),
			m_ThisPanel, 1);
	}
#pragma endregion

	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(menu, 2);
	m_Elements.push_back(menu);

	//setup startup time;
	m_ElapsedTime = 0;

	// set key event
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(InfoPanel::onKeyPressed, this);
	m_GameScene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_GameScene);
}

void InfoPanel::update(float delta)
{
	m_ElapsedTime += delta;

	// update minute
	if (m_ElapsedTime < 4)
		return;

	m_CurrentMinute++;
	m_ElapsedTime = 0;
	GameFunctions::updatLabelText_TimeFormat(m_TimeMinDisplay, m_CurrentMinute % 60);

	// update hour
	if (m_CurrentMinute > 59)
	{
		m_CurrentMinute = 0;
		m_CurrentHour++;
		GameFunctions::updatLabelText_TimeFormat(m_TimeHourDisplay, m_CurrentHour % 24);
	}

	// update weekday
	if (m_CurrentHour > 23)
	{
		m_CurrentHour = 0;
		m_Today++;
		m_WeekDay->setString(m_WeekDays[m_Today % 7]);
	}

	// update week
	if (m_Today > 6)
	{
		m_Today = 0;
		m_Bank->update();
		m_Weeks++;
		GameFunctions::updatLabelText_TimeFormat(m_WeekCount, m_Weeks);
	}

}

void InfoPanel::enableBankButton(bool value)
{
	m_BankButton->setEnabled(value);
}

void InfoPanel::checkBalanceCallback(cocos2d::Ref* pSender, GameScene* scene)
{
	m_IsOpeningSubWindow = !m_IsOpeningSubWindow;

	(m_IsOpeningSubWindow) ? m_Bank->openBankPanel(scene, m_Weeks, m_SceneMidPoint) : m_Bank->closePanel();
}

void InfoPanel::onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event)
{
}

void InfoPanel::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (m_IsOpeningSubWindow)
		{
			m_Bank->closePanel();
			m_IsOpeningSubWindow = false;
		}
	}
}

void InfoPanel::onCurrentCashChange(Player* player, int currentCashAmout)
{
	GameFunctions::updateLabelText_MoneyFormat(m_Saving, currentCashAmout);
}
