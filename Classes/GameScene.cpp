#include "GameScene.h"
#include "2d/CCLayer.h"
#include <MouseOverMenuItem.h>
#include "cocostudio/SimpleAudioEngine.h"
#include "GameData.h"

using namespace CocosDenshion;
USING_NS_CC;


GameScene::~GameScene()
{
	//menuItems
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

	m_VisibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create("Sprites/CityView.png");
	if (!backgroundSprite)
		return false;

	setSpriteScale(backgroundSprite, Vec2::ONE);
	backgroundSprite->setPosition(Point(origin.x + (m_VisibleSize.width / 2), origin.y + (m_VisibleSize.height / 2)));

	this->addChild(backgroundSprite, 0);

	m_TopPanel = Sprite::create("X/InGamePanel_Black_80.png");
	if (!m_TopPanel)
		return false;

	m_TopPanel->setPosition(Vec2(m_VisibleSize.width * 0.5f, 680.f));
	this->addChild(m_TopPanel, 1);

	auto topPanelMidPoint = Vec2(m_TopPanel->getContentSize().width * 0.5f, m_TopPanel->getContentSize().height * 0.5f);

	auto playerSprite = Sprite::createWithSpriteFrameName(GameData::getInstance().getPlayerCharacter());

	if (!playerSprite)
		return false;

	playerSprite->setPosition(topPanelMidPoint.x - 250.f, topPanelMidPoint.y - 5.f);
	playerSprite->setScale(0.4f);
	m_TopPanel->addChild(playerSprite, 1);

	auto playerSpriteMidPoint = Vec2(playerSprite->getContentSize().width * 0.5f, playerSprite->getContentSize().height * 0.5f);

	auto nameLabel = Label::createWithTTF(GameData::getInstance().getPlayerName(), "fonts/NirmalaB.ttf", 22);

	if (nameLabel)
	{
		nameLabel->setMaxLineWidth(125);
		nameLabel->enableWrap(true);
		nameLabel->setTextColor(Color4B::WHITE);
		nameLabel->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
		nameLabel->setPosition(playerSprite->getPositionX() + 100.f, playerSprite->getPositionY());
		m_TopPanel->addChild(nameLabel, 1);
	}

#pragma region CreateSavingLabels and Bank buttons
	auto cashSymbol = Label::createWithTTF("$", "fonts/NirmalaB.ttf", 20);
	if (cashSymbol)
	{
		cashSymbol->setTextColor(GameData::getInstance().m_ColorType.Goldenrdo);
		cashSymbol->enableGlow(Color4B::WHITE);
		cashSymbol->setPosition(topPanelMidPoint.x + 350.f, topPanelMidPoint.y - 30.f);
		m_TopPanel->addChild(cashSymbol, 1);
	}

	m_Saving = Label::createWithTTF("", "fonts/NirmalaB.ttf", 25);
	if (m_Saving)
	{
		updateSavingLabel(m_Saving, m_CurrentCash);
		m_Saving->setTextColor(GameData::getInstance().m_ColorType.Gold);
		m_Saving->enableGlow(Color4B::WHITE);
		m_Saving->setPosition(topPanelMidPoint.x + 420.f, topPanelMidPoint.y - 30.f);
		m_TopPanel->addChild(m_Saving, 1);
	}

	auto creditCard = Sprite::create("X/CreditCardBank_100.png");
	if (creditCard)
	{
		creditCard->setPosition(topPanelMidPoint.x + 370.f, topPanelMidPoint.y + 10.f);
		creditCard->setScale(0.6f);
		m_TopPanel->addChild(creditCard, 1);
	}

	// create bank buttons
	auto buttonSpriteNormal = Sprite::createWithSpriteFrameName("ButtonBlueNormal.png");
	auto buttonSpriteSelected = Sprite::createWithSpriteFrameName("ButtonBlueLit.png");
	auto buttonSpriteDisabled = Sprite::createWithSpriteFrameName("ButtonBlueDisabled.png");

	if (buttonSpriteNormal && buttonSpriteSelected && buttonSpriteDisabled)
	{
		auto bankButton = MouseOverMenuItem::create(buttonSpriteNormal, buttonSpriteSelected, buttonSpriteDisabled, CC_CALLBACK_1(GameScene::checkBalanceCallback, this));
		bankButton->onMouseOver = CC_CALLBACK_2(GameScene::onMouseOver, this);
		auto bankPos = Vec2(m_TopPanel->getPosition().x + 445.f, m_TopPanel->getPosition().y);
		bankButton->setPosition(bankPos);
		bankButton->setScale(0.7f);
		bankButton->setItemRect(bankPos, 0.7f);

		auto myBankLabel = Label::createWithTTF("CHIBANK", "fonts/NirmalaB.ttf", 20);
		if (myBankLabel)
		{
			myBankLabel->setTextColor(GameData::getInstance().m_ColorType.Taro);
			myBankLabel->setPosition(bankButton->getContentSize().width * 0.5f, bankButton->getContentSize().height * 0.5f);
			bankButton->addChild(myBankLabel, 1);
		}
		menuItems.pushBack(bankButton);
	}
#pragma endregion

#pragma region CreateTimeLabels
	auto weekLabel = Label::createWithTTF("WEEK", "fonts/NirmalaB.ttf", 14);
	if (weekLabel)
	{
		weekLabel->setTextColor(Color4B::WHITE);
		weekLabel->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
		weekLabel->setPosition(topPanelMidPoint.x + 540.f, topPanelMidPoint.y + 15.f);
		m_TopPanel->addChild(weekLabel, 1);
	}

	m_WeekCount = Label::createWithTTF("", "fonts/NirmalaB.ttf", 16);
	if (m_WeekCount)
	{
		updateTimeLabel(m_WeekCount, m_Weeks);
		m_WeekCount->setTextColor(Color4B::WHITE);
		m_WeekCount->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		m_WeekCount->setPosition(topPanelMidPoint.x + 580.f, topPanelMidPoint.y + 15.f);
		m_TopPanel->addChild(m_WeekCount, 1);
	}

	m_WeekDay = Label::createWithTTF(m_WeekDays[m_Today], "fonts/NirmalaB.ttf", 14);
	if (m_WeekDay)
	{
		m_WeekDay->setTextColor(Color4B::WHITE);
		m_WeekDay->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
		m_WeekDay->setPosition(topPanelMidPoint.x + 555.f, topPanelMidPoint.y - 5.f);
		m_TopPanel->addChild(m_WeekDay, 1);
	}

	m_TimeHourDisplay = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
	if (m_TimeHourDisplay)
	{
		updateTimeLabel(m_TimeHourDisplay, m_CurrentHour);
		m_TimeHourDisplay->setTextColor(Color4B::WHITE);
		m_TimeHourDisplay->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		m_TimeHourDisplay->setPosition(topPanelMidPoint.x + 530.f, topPanelMidPoint.y - 30.f);
		m_TopPanel->addChild(m_TimeHourDisplay, 1);
	}
	auto timeMark = Label::createWithTTF(":", "fonts/NirmalaB.ttf", 30);
	if (timeMark)
	{
		timeMark->setTextColor(Color4B::WHITE);
		timeMark->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		timeMark->setPosition(topPanelMidPoint.x + 555.f, topPanelMidPoint.y - 28.f);
		m_TopPanel->addChild(timeMark, 1);
	}

	m_TimeMinDisplay = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
	if (m_TimeHourDisplay)
	{
		updateTimeLabel(m_TimeMinDisplay, m_CurrentMinute);
		m_TimeMinDisplay->setTextColor(Color4B::WHITE);
		m_TimeMinDisplay->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
		m_TimeMinDisplay->setPosition(topPanelMidPoint.x + 580.f, topPanelMidPoint.y - 30.f);
		m_TopPanel->addChild(m_TimeMinDisplay, 1);
	}
#pragma endregion

	m_BottomPanel = Sprite::create("X/InGamePanel_Black_80.png");
	m_BottomPanel->setPosition(Vec2(m_VisibleSize.width * 0.5f, 50.f));
	this->addChild(m_BottomPanel, 1);

	m_ElapsedTime = 0;
	this->scheduleUpdate();

	auto menu = Menu::createWithArray(menuItems);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);
	return true;
}

void GameScene::update(float delta)
{
	updateGameTime(delta);
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
	updateTimeLabel(m_TimeMinDisplay, m_CurrentMinute % 60);


	// update hour
	if (m_CurrentMinute > 59)
	{
		m_CurrentMinute = 0;
		m_CurrentHour++;
		updateTimeLabel(m_TimeHourDisplay, m_CurrentHour % 24);
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
		updateTimeLabel(m_WeekCount, m_Weeks);
	}
}

void GameScene::updateTimeLabel(cocos2d::Label* label, unsigned value)
{
	std::string timeStr = std::to_string(value);
	label->setString(std::string(2 - timeStr.length(), '0').append(timeStr));
}

void GameScene::updateSavingLabel(cocos2d::Label* label, int value)
{
	std::string cashStr = std::to_string(value);
	auto cashStrLength = cashStr.length();
	if (cashStrLength > 3)
		cashStr = cashStr.insert(cashStr.length() - 3, 1, ',');
	if (cashStrLength > 6)
		cashStr = cashStr.insert(cashStr.length() - 7, 1, ',');
	label->setString(cashStr);
}

void GameScene::checkBalanceCallback(cocos2d::Ref* pSender)
{
	log("Open bank account balacnce");
}

void GameScene::onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event)
{
}


