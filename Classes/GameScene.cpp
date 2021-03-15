#include "GameScene.h"
#include "2d/CCLayer.h"
#include <MouseOverMenuItem.h>
#include "GameData.h"
#include "GameFunctions.h"
#include "Bank.h"
#include "Player.h"
#include "GameStartPanel.h"
#include "cocostudio/SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;


GameScene::~GameScene()
{
	delete m_StartupPanel;
	m_StartupPanel = nullptr;
	m_MenuItems.clear();
	delete m_Bank;
	m_Bank = nullptr;
	m_Player = nullptr;
}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
		return false;

	//init data
	GameData::getInstance().init();
	//

	m_Player = GameData::getInstance().m_Player;
	m_Player->onCashAmoutChange = CC_CALLBACK_2(GameScene::onCurrentCashChange, this);

#pragma region Setup InGame UI
	m_VisibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::createWithSpriteFrameName("GameSceneCityView.png");
	if (!backgroundSprite)
		return false;
	
	auto sceneMidPoint = Point(origin.x + (m_VisibleSize.width / 2), origin.y + (m_VisibleSize.height / 2));
	GameFunctions::displaySprite(backgroundSprite, sceneMidPoint, this, 0);
	setSpriteScale(backgroundSprite, Vec2::ONE);
	
	m_StartupPanel = new GameStartPanel();
	m_StartupPanel->createPanel(this, sceneMidPoint);

	m_TopPanel = Sprite::createWithSpriteFrameName("InGamePanel_Black_80.png");
	if (!m_TopPanel)
		return false;

	GameFunctions::displaySprite(m_TopPanel, Vec2(m_VisibleSize.width * 0.5f, 680.f), this, 1);
	auto topPanelMidPoint = Vec2(m_TopPanel->getContentSize().width * 0.5f, m_TopPanel->getContentSize().height * 0.5f);

	auto playerSprite = Sprite::createWithSpriteFrameName(GameData::getInstance().getPlayerCharacter(m_Player->getCharacter()));
	if (!playerSprite)
		return false;

	GameFunctions::displaySprite(playerSprite, Vec2(topPanelMidPoint.x - 250.f, topPanelMidPoint.y - 5.f), m_TopPanel, 1, 0.4f, 0.4f);

	auto nameLabel = Label::createWithTTF(m_Player->getName(), "fonts/NirmalaB.ttf", 22);
	if (nameLabel)
	{
		nameLabel->setMaxLineWidth(125);
		nameLabel->enableWrap(true);
		nameLabel->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
		GameFunctions::displayLabel(nameLabel, Color4B::WHITE, Vec2(playerSprite->getPositionX() + 100.f, playerSprite->getPositionY()),
			m_TopPanel, 1);
	}

#pragma region CreateSavingLabels and Bank 
	auto cashSymbol = Label::createWithTTF("$", "fonts/NirmalaB.ttf", 20);
	if (cashSymbol)
	{
		cashSymbol->enableGlow(Color4B::WHITE);
		GameFunctions::displayLabel(cashSymbol, GameData::getInstance().m_ColorType.Goldenrdo, Vec2(topPanelMidPoint.x + 350.f, topPanelMidPoint.y - 30.f),
			m_TopPanel, 1);
	}

	m_Saving = Label::createWithTTF("", "fonts/NirmalaB.ttf", 25);
	if (m_Saving)
	{
		GameFunctions::updateLabelText_MoneyFormat(m_Saving, m_CurrentCash);
		m_Saving->enableGlow(Color4B::WHITE);
		GameFunctions::displayLabel(m_Saving, GameData::getInstance().m_ColorType.Gold,
			Vec2(topPanelMidPoint.x + 420.f, topPanelMidPoint.y - 30.f), m_TopPanel, 1);
	}

	auto creditCard = Sprite::createWithSpriteFrameName("CreditCardBank_100.png");
	if (creditCard)
	{
		creditCard->setPosition(topPanelMidPoint.x + 370.f, topPanelMidPoint.y + 10.f);
		creditCard->setScale(0.6f);
		m_TopPanel->addChild(creditCard, 1);
	}

	// create bank buttons
	auto bankButton = MouseOverMenuItem::creatMouseOverMenuButton("ButtonBlueNormal.png", "ButtonBlueLit.png", "ButtonBlueDisabled.png",
		CC_CALLBACK_1(GameScene::checkBalanceCallback, this));
	if (bankButton)
	{
		bankButton->onMouseOver = CC_CALLBACK_2(GameScene::onMouseOver, this);
		auto bankPos = Vec2(m_TopPanel->getPosition().x + 445.f, m_TopPanel->getPosition().y);
		bankButton->setPosition(bankPos);
		bankButton->setScale(0.7f);
		bankButton->setItemRect(bankPos, 0.7f);

		auto myBankLabel = Label::createWithTTF("CHIBANK", "fonts/NirmalaB.ttf", 20);
		if (myBankLabel)
			GameFunctions::displayLabel(myBankLabel, GameData::getInstance().m_ColorType.Taro,
				Vec2(bankButton->getContentSize().width * 0.5f, bankButton->getContentSize().height * 0.5f), bankButton, 1);

		m_MenuItems.pushBack(bankButton);

		m_Bank = new Bank();
	}
#pragma endregion

#pragma region CreateTimeLabels
	auto weekLabel = Label::createWithTTF("WEEK", "fonts/NirmalaB.ttf", 14);
	if (weekLabel)
	{
		weekLabel->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
		GameFunctions::displayLabel(weekLabel, Color4B::WHITE, Vec2(topPanelMidPoint.x + 540.f, topPanelMidPoint.y + 15.f),
			m_TopPanel, 1);
	}

	m_WeekCount = Label::createWithTTF("", "fonts/NirmalaB.ttf", 16);
	if (m_WeekCount)
	{
		GameFunctions::updatLabelText_TimeFormat(m_WeekCount, m_Weeks);
		m_WeekCount->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		GameFunctions::displayLabel(m_WeekCount, Color4B::WHITE, Vec2(topPanelMidPoint.x + 580.f, topPanelMidPoint.y + 15.f),
			m_TopPanel, 1);
	}

	m_WeekDay = Label::createWithTTF(m_WeekDays[m_Today], "fonts/NirmalaB.ttf", 14);
	if (m_WeekDay)
	{
		m_WeekDay->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
		GameFunctions::displayLabel(m_WeekDay, Color4B::WHITE, Vec2(topPanelMidPoint.x + 555.f, topPanelMidPoint.y - 5.f),
			m_TopPanel, 1);
	}

	m_TimeHourDisplay = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
	if (m_TimeHourDisplay)
	{
		GameFunctions::updatLabelText_TimeFormat(m_TimeHourDisplay, m_CurrentHour);
		m_TimeHourDisplay->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		GameFunctions::displayLabel(m_TimeHourDisplay, Color4B::WHITE, Vec2(topPanelMidPoint.x + 530.f, topPanelMidPoint.y - 30.f),
			m_TopPanel, 1);
	}

	auto timeMark = Label::createWithTTF(":", "fonts/NirmalaB.ttf", 30);
	if (timeMark)
	{
		timeMark->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		GameFunctions::displayLabel(timeMark, Color4B::WHITE, Vec2(topPanelMidPoint.x + 555.f, topPanelMidPoint.y - 28.f),
			m_TopPanel, 1);
	}

	m_TimeMinDisplay = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
	if (m_TimeHourDisplay)
	{
		GameFunctions::updatLabelText_TimeFormat(m_TimeMinDisplay, m_CurrentMinute);
		m_TimeMinDisplay->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		GameFunctions::displayLabel(m_TimeMinDisplay, Color4B::WHITE, Vec2(topPanelMidPoint.x + 580.f, topPanelMidPoint.y - 30.f),
			m_TopPanel, 1);
	}
#pragma endregion

	m_BottomPanel = Sprite::createWithSpriteFrameName("InGamePanel_Black_80.png");
	m_BottomPanel->setPosition(Vec2(m_VisibleSize.width * 0.5f, 50.f));
	this->addChild(m_BottomPanel, 1);

	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);
#pragma endregion

	// Game time setup
	m_ElapsedTime = 0;
	this->scheduleUpdate();

	// set key event
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GameScene::update(float delta)
{
	updateGameTime(delta);
}

void GameScene::updateCurrentCash(int amout)
{
	m_CurrentCash += amout;
	GameFunctions::updateLabelText_MoneyFormat(m_Saving, m_CurrentCash);
}

void GameScene::setSpriteScale(Sprite* sprite, Vec2 scale)
{
	sprite->setScaleX((m_VisibleSize.width / sprite->getContentSize().width) * scale.x);
	sprite->setScaleY((m_VisibleSize.height / sprite->getContentSize().height) * scale.y);
}

void GameScene::updateGameTime(float delta)
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
		m_Weeks++;
		GameFunctions::updatLabelText_TimeFormat(m_WeekCount, m_Weeks);
	}
}


void GameScene::checkBalanceCallback(cocos2d::Ref* pSender)
{
	m_IsOpeningSubWindow = !m_IsOpeningSubWindow;

	(m_IsOpeningSubWindow) ? m_Bank->openBankPanel(this, m_Weeks) : m_Bank->closeBankPanel();
}

void GameScene::onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event)
{
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (m_IsOpeningSubWindow)
		{
			m_Bank->closeBankPanel();
			m_IsOpeningSubWindow = false;
		}
		else
			Director::getInstance()->end();
	}
}

void GameScene::onCurrentCashChange(Player* player, int currentCashAmout)
{
	GameFunctions::updateLabelText_MoneyFormat(m_Saving, currentCashAmout);
}


