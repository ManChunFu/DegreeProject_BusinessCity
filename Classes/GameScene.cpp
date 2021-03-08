#include "GameScene.h"
#include "InGameData.h"
#include "2d/CCLayer.h"

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

	visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create("Sprites/CityView.png");
	if (!backgroundSprite)
		return false;

	setSpriteScale(backgroundSprite, Vec2::ONE);
	backgroundSprite->setPosition(Point(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)));

	this->addChild(backgroundSprite, 0);

	topPanel = Sprite::create("X/InGamePanel_Black_80.png");
	if (!topPanel)
		return false;

	topPanel->setPosition(Vec2(visibleSize.width * 0.5f, 680.f));
	this->addChild(topPanel, 1);

	auto topPanelMidPoint = Vec2(topPanel->getContentSize().width * 0.5f, topPanel->getContentSize().height * 0.5f);

	auto playerSprite = Sprite::createWithSpriteFrameName(characterSpriteMap[playerCharacter]);
	if (!playerSprite)
		return false;

	playerSprite->setPosition(topPanelMidPoint.x - 250.f, topPanelMidPoint.y - 5.f);
	playerSprite->setScale(0.4f);
	topPanel->addChild(playerSprite, 1);

	auto playerSpriteMidPoint = Vec2(playerSprite->getContentSize().width * 0.5f, playerSprite->getContentSize().height * 0.5f);

	auto nameLabel = Label::createWithTTF(playerName, "fonts/NirmalaB.ttf", 22);
	if (nameLabel)
	{
		nameLabel->setMaxLineWidth(125);
		nameLabel->enableWrap(true);
		nameLabel->setTextColor(Color4B::WHITE);
		nameLabel->enableGlow(colorType.DeepPink);
		nameLabel->setPosition(playerSprite->getPositionX() + 100.f, playerSprite->getPositionY());
		topPanel->addChild(nameLabel, 1);
	}

	auto weekLabel = Label::createWithTTF("WEEK", "fonts/NirmalaB.ttf", 14);
	if (weekLabel)
	{
		weekLabel->setTextColor(Color4B::WHITE);
		weekLabel->enableGlow(colorType.DeepPink);
		weekLabel->setPosition(topPanelMidPoint.x + 540.f, topPanelMidPoint.y + 15.f);
		topPanel->addChild(weekLabel, 1);
	}

	weekCount = Label::createWithTTF("1", "fonts/NirmalaB.ttf", 16);
	if (weekCount)
	{
		weekCount->setTextColor(Color4B::WHITE);
		weekCount->enableGlow(colorType.PowderBlue);
		weekCount->setPosition(topPanelMidPoint.x +580.f, topPanelMidPoint.y + 15.f);
		topPanel->addChild(weekCount, 1);
	}

	dayOfWeek = Label::createWithTTF("MONDAY", "fonts/NirmalaB.ttf", 14);
	if (dayOfWeek)
	{
		dayOfWeek->setTextColor(Color4B::WHITE);
		dayOfWeek->enableGlow(colorType.DeepPink);
		dayOfWeek->setPosition(topPanelMidPoint.x +550.f, topPanelMidPoint.y -5.f);
		topPanel->addChild(dayOfWeek, 1);
	}

	timeDisplay = Label::createWithTTF("08:00", "fonts/NirmalaB.ttf", 30);
	if (timeDisplay)
	{
		timeDisplay->setTextColor(Color4B::WHITE);
		timeDisplay->enableGlow(colorType.PowderBlue);
		timeDisplay->setPosition(topPanelMidPoint.x +550.f, topPanelMidPoint.y -30.f);
		topPanel->addChild(timeDisplay, 1);
	}

	bottomPanel = Sprite::create("X/InGamePanel_Black_80.png");
	bottomPanel->setPosition(Vec2(visibleSize.width * 0.5f, 50.f));
	this->addChild(bottomPanel, 1);

	elapsedTime = 0;
	this->scheduleUpdate();

	return true;
}

void GameScene::setSpriteScale(Sprite* sprite, Vec2 scale)
{
	sprite->setScaleX((visibleSize.width / sprite->getContentSize().width) * scale.x);
	sprite->setScaleY((visibleSize.height / sprite->getContentSize().height) * scale.y);
}

void GameScene::update(float delta)
{



}
