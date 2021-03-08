#include "GameScene.h"
#include "InGameData.h"

cocos2d::Scene* GameScene::createScene()
{
	return GameScene::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool GameScene::init()
{
	if (!Scene::init())
		return false;

	visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create("Sprites/CityView.png");
	if (backgroundSprite == nullptr || backgroundSprite->getContentSize().width <= 0 || backgroundSprite->getContentSize().height <= 0)
		problemLoading("'Sprites/CityView.png'");
	else
	{
		setSpriteScale(backgroundSprite, Vec2::ONE);
		backgroundSprite->setPosition(Point(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)));
	}
	this->addChild(backgroundSprite, 0);

	topPanel = Sprite::create("X/InGamePanel_Black_80.png");
	if (!topPanel)
		problemLoading("'X/InGamePanel_Black_80.png'");

	topPanel->setPosition(Vec2(visibleSize.width * 0.5f, 680.f));
	this->addChild(topPanel, 1);

	auto topPanelMidPoint = Vec2(topPanel->getContentSize().width * 0.5f, topPanel->getContentSize().height * 0.5f);
	
	auto playerSprite = Sprite::createWithSpriteFrameName(characterSpriteMap[playerCharacter]);
	if (!playerSprite)
		problemLoading("characterSpriteMap[playerCharacter]");

	playerSprite->setPosition(topPanelMidPoint.x -250.f, topPanelMidPoint.y -5.f);
	playerSprite->setScale(0.4f);
	topPanel->addChild(playerSprite, 1);

	auto playerSpriteMidPoint = Vec2(playerSprite->getContentSize().width * 0.5f, playerSprite->getContentSize().height * 0.5f);
	
	auto optionLabel = Label::createWithTTF(playerName, "fonts/NirmalaB.ttf", 22);
	optionLabel->setMaxLineWidth(125);
	optionLabel->enableWrap(true);
	optionLabel->setTextColor(Color4B::WHITE);
	optionLabel->enableShadow(colorType.FireBrick);
	optionLabel->enableOutline(Color4B::WHITE);
	optionLabel->setPosition(playerSprite->getPositionX() + 100.f, playerSprite->getPositionY());
	topPanel->addChild(optionLabel, 2);


	bottomPanel = Sprite::create("X/InGamePanel_Black_80.png");
	bottomPanel->setPosition(Vec2(visibleSize.width * 0.5f, 50.f));
	this->addChild(bottomPanel, 1);


	return true;
}

void GameScene::setSpriteScale(Sprite* sprite, Vec2 scale)
{
	sprite->setScaleX((visibleSize.width / sprite->getContentSize().width) * scale.x);
	sprite->setScaleY((visibleSize.height / sprite->getContentSize().height) * scale.y);
}
