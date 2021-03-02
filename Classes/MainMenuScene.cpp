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

#include "cocostudio/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

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
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    
    auto backgroundSprite = Sprite::create("Sprites/CityTopDownViewMediumWithLayer.png");
    if (backgroundSprite == nullptr || backgroundSprite->getContentSize().width <= 0 || backgroundSprite->getContentSize().height <= 0)
        problemLoading("'Sprites/CityTopDownViewMediumWithLayer.png'");
    else
        backgroundSprite->setPosition(Point(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)));
   
    this->addChild(backgroundSprite, 0);


    auto label = Label::createWithTTF("Bussiness City", "fonts/BROADW.TTF", 24);
    label->setTextColor(Color4B::ORANGE);
    label->enableGlow(Color4B::BLACK);
    label->enableShadow(Color4B::BLACK);
    label->setPosition(Point(origin.x + visibleSize.width/2, origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);
    
    auto starNormal = Sprite::createWithSpriteFrameName("YellowStarNormal.png");
    auto starSelected = Sprite::createWithSpriteFrameName("StarSelected.png");
    auto closeItem = MouseOverMenuItem::create(starSelected, starNormal, NULL, CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));

    closeItem->onMouseOver = CC_CALLBACK_2(MainMenuScene::onMouseOver, this);

    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
        problemLoading("'Sprites/YellowStarNormal.png' and 'Sprites/StarSelected.png'");
    else
    {
        float x = label->getPositionX() + 120.f;
        float y = label->getPositionY();
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
    auto scene = MainMenuScene::createScene();

    // run
    Director::getInstance()->replaceScene(TransitionFade::create(1.f, scene, Color3B(0, 0, 0)));
    
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
