/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MainMenuScene.h"
#include "GameScene.h"
#include "ui/UIButton.h"



Scene* MainMenuScene::createScene()
{
    return MainMenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
        return false;

    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    
    // Background image
    auto backgroundSprite = Sprite::createWithSpriteFrameName("CityTopDownViewWithLayer.png");
    if (backgroundSprite == nullptr || backgroundSprite->getContentSize().width <= 0 || backgroundSprite->getContentSize().height <= 0)
        problemLoading("'Sprites/CityTopDownViewMediumWithLayer.png'");
    else
    {
        SetScale(backgroundSprite, 1);

        backgroundSprite->setPosition(Point(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)));
    }
    this->addChild(backgroundSprite, 0);

    // Title
    auto label = Label::createWithTTF("Bussiness City", "fonts/BROADW.TTF", 56);
    label->setTextColor(Color4B::ORANGE);
    label->enableGlow(Color4B::BLACK);
    label->enableShadow(Color4B::BLACK);
    label->setPosition(Point(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);
    
    // Buttons

    auto buttonNormal = Sprite::createWithSpriteFrameName("ButtonBlueNormal.png");
    auto buttonSelected = Sprite::createWithSpriteFrameName("ButtonBlueLit.png");
    auto buttonDisabled = Sprite::createWithSpriteFrameName("ButtonBlueDisabled.png");
    auto closeItem = MouseOverMenuItem::create(buttonNormal, buttonSelected, buttonDisabled, CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
    auto start = Label::createWithTTF("Start", "fonts/BROADW.TTF", 25);
    start->setTextColor(Color4B(125, 100, 100, 255));
    start->setPosition(Vec2(closeItem->getContentSize().width/2, closeItem->getContentSize().height/2));
    closeItem->addChild(start, 2);

    closeItem->onMouseOver = CC_CALLBACK_2(MainMenuScene::onMouseOver, this);

    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
        problemLoading("'Sprites/ButtonBlueNormal.png' and 'Sprites/ButtonBlueLit.png'");
    else
    {
        float x = label->getPositionX();
        float y = label->getPositionY() -200.f;
        closeItem->setPosition(Vec2(x, y));
        closeItem->SetItemRect(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(MainMenuScene::onKeyPressed, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    audio = SimpleAudioEngine::getInstance();

    audio->playBackgroundMusic("Sounds/CuteBunnyHopLoopl.mp3", true);

    return true;
}


void MainMenuScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
   //Director::getInstance()->end();
    StopAudio();
    auto scene = GameScene::createScene();

    // run
    Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    /*EventCustom customEndEvent("game_scene_close_event");
    _eventDispatcher->dispatchEvent(&customEndEvent);*/
}


void MainMenuScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        StopAudio(true);
        Director::getInstance()->end();
    }
}


void MainMenuScene::SetScale(Sprite* sprite, UINT8 scale)
{
    sprite->setScaleX((visibleSize.width / sprite->getContentSize().width) * scale);
    sprite->setScaleY((visibleSize.height / sprite->getContentSize().height) * scale);
}

void MainMenuScene::onMouseOver(MouseOverMenuItem* overItem, Event* event)
{
    audio->playEffect("Sounds/SelectedSound.mp3", false, 1.f, 1.f, 1.f);
}

void MainMenuScene::StopAudio(bool deleteAudio)
{
    audio->stopBackgroundMusic(true);
    if (deleteAudio)
    {
        audio->end();
        audio = nullptr;
    }
}
