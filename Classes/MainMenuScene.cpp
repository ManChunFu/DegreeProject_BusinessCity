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
#include <MouseOverMenuItem.h>
#include "ui/UIWidget.h"
#include <MainMenuPlayerSetting.h>
 #include "cocostudio/SimpleAudioEngine.h"
 using namespace CocosDenshion;
 USING_NS_CC;


MainMenuScene::~MainMenuScene()
{
	delete m_PlayerSetting;
	m_PlayerSetting = nullptr;
}

Scene* MainMenuScene::createScene()
{
	return MainMenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename, ui::Widget::TextureResType textureType = ui::Widget::TextureResType::LOCAL)
{
	if (textureType == ui::Widget::TextureResType::PLIST)
		printf("Error while loading: %s in plst", filename);
	else
	{
		printf("Error while loading: %s\n", filename);
		printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
	}
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
		return false;

	m_VisibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////

	// background image
	auto backgroundSprite = Sprite::createWithSpriteFrameName("CityTopDownViewWithLayer.png");
	if (backgroundSprite == nullptr || backgroundSprite->getContentSize().width <= 0 || backgroundSprite->getContentSize().height <= 0)
	{
		problemLoading("'CityTopDownViewMediumWithLayer.png'", ui::Widget::TextureResType::PLIST);
		return false;
	}

	setSpriteScale(backgroundSprite, 1);
	backgroundSprite->setPosition(Point(origin.x + (m_VisibleSize.width / 2), origin.y + (m_VisibleSize.height / 2)));
	this->addChild(backgroundSprite, 0);

	// title
	auto label = Label::createWithTTF("Bussiness City", "fonts/BROADW.TTF", 56);
	label->setTextColor(Color4B::ORANGE);
	label->enableGlow(Color4B::BLACK);
	label->enableShadow(Color4B::BLACK);
	label->setPosition(Point(origin.x + m_VisibleSize.width / 2, origin.y + m_VisibleSize.height - label->getContentSize().height));
	this->addChild(label, 1);

	// start button
	auto startNormalSprite = Sprite::createWithSpriteFrameName("ButtonBlueNormal.png");
	auto startSelectedSprite = Sprite::createWithSpriteFrameName("ButtonBlueLit.png");
	auto startDisabledSprite = Sprite::createWithSpriteFrameName("ButtonBlueDisabled.png");

	auto startButtonItem = MouseOverMenuItem::create(startNormalSprite, startSelectedSprite, startDisabledSprite, CC_CALLBACK_1(MainMenuScene::menuStartCallback, this));
	if (startButtonItem == nullptr || startButtonItem->getContentSize().width <= 0 || startButtonItem->getContentSize().height <= 0)
	{
		problemLoading("'ButtonBlueNormal.png' and 'ButtonBlueLit.png' and 'ButtonBlueDisabled.png'", ui::Widget::TextureResType::PLIST);
		return false;
	}
	startButtonItem->onMouseOver = CC_CALLBACK_2(MainMenuScene::onMouseOver, this);

	// start button text
	Vec2 startButtonPos = Vec2(label->getPositionX(), label->getPositionY() - 200.f);
	startButtonItem->setPosition(startButtonPos);
	startButtonItem->setItemRect(startButtonPos);

	Vec2 buttonMidPoint = Vec2(startButtonItem->getContentSize().width * 0.5f, startButtonItem->getContentSize().height * 0.5f);
	auto startText = Label::createWithTTF("Start", "fonts/BROADW.TTF", 25);
	startText->setTextColor(Color4B(125, 100, 100, 255));
	startText->setPosition(buttonMidPoint);
	startButtonItem->addChild(startText, 2);

	m_MenuItems.pushBack(startButtonItem);

	// quit button
	auto quitNormalSprite = Sprite::createWithSpriteFrameName("ButtonBlueNormal.png");
	auto quitSelectedSprite = Sprite::createWithSpriteFrameName("ButtonBlueLit.png");
	auto quitDisabledSprite = Sprite::createWithSpriteFrameName("ButtonBlueDisabled.png");
	auto quitButtonItem = MouseOverMenuItem::create(quitNormalSprite, quitSelectedSprite, quitDisabledSprite, CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
	quitButtonItem->onMouseOver = CC_CALLBACK_2(MainMenuScene::onMouseOver, this);
	Vec2 quitButtonPos = Vec2(startButtonPos.x, startButtonPos.y - 100.f);
	quitButtonItem->setPosition(quitButtonPos);
	quitButtonItem->setItemRect(quitButtonPos);

	// quit button text
	auto quitText = Label::createWithTTF("Quit", "fonts/BROADW.TTF", 25);
	quitText->setTextColor(Color4B(125, 100, 100, 255));
	quitText->setPosition(buttonMidPoint);
	quitButtonItem->addChild(quitText, 2);

	m_MenuItems.pushBack(quitButtonItem);

	// create menu, it's an autorelease object
	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	// set key event
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(MainMenuScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// set music
	m_Audio = SimpleAudioEngine::getInstance();
	m_Audio->playBackgroundMusic("Sounds/market-song.mp3", true);

	m_PlayerSetting = new MainMenuPlayerSetting();

	return true;
}

void MainMenuScene::menuStartCallback(Ref* pSender)
{
	isOpeningSubWindow = true;
	setMenuItemVisible(false);
	m_PlayerSetting->OpenSettingWindow(this);
}

void MainMenuScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	StopAudio(true);
	m_PlayerSetting = nullptr;
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	/*EventCustom customEndEvent("game_scene_close_event");
	_eventDispatcher->dispatchEvent(&customEndEvent);*/
}

void MainMenuScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (!isOpeningSubWindow)
			menuCloseCallback(this);
		else
		{
			isOpeningSubWindow = false;
			m_PlayerSetting->closeSettingWindow();
			setMenuItemVisible(false);
		}
	}
}

void MainMenuScene::setSpriteScale(Sprite* sprite, float scale)
{
	sprite->setScaleX((m_VisibleSize.width / sprite->getContentSize().width) * scale);
	sprite->setScaleY((m_VisibleSize.height / sprite->getContentSize().height) * scale);
}

void MainMenuScene::setMenuItemVisible(bool value)
{
	for (auto item : m_MenuItems)
	{
		item->setEnabled(value);
		item->setVisible(value);
	}
}

void MainMenuScene::onMouseOver(MouseOverMenuItem* overItem, Event* event)
{
	m_Audio->playEffect("Sounds/SelectedSound.mp3", false, 1.f, 1.f, 1.f);
}

void MainMenuScene::StopAudio(bool deleteAudio)
{
	m_Audio->stopBackgroundMusic(true);
	if (deleteAudio)
	{
		m_Audio->end();
		m_Audio = nullptr;
	}
}
