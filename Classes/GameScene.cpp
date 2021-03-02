#include "GameScene.h"

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

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance() -> getVisibleOrigin();

    auto backgroundSprite = Sprite::create("Sprites/CityView.png");
    if (backgroundSprite == nullptr || backgroundSprite->getContentSize().width <= 0 || backgroundSprite->getContentSize().height <= 0)
        problemLoading("'Sprites/CityView.png'");
    else
    {
        backgroundSprite->setScaleX((visibleSize.width / backgroundSprite->getContentSize().width) * scaleFactor.x);
        backgroundSprite->setScaleY((visibleSize.height / backgroundSprite->getContentSize().height) * scaleFactor.y);

        backgroundSprite->setPosition(Point(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)));
    }

    this->addChild(backgroundSprite, 0);

    return true;
}
