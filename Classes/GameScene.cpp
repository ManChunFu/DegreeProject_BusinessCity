#include "GameScene.h"
#include "2d/CCLayer.h"
#include <MouseOverMenuItem.h>
#include "GameData.h"
#include "GameFunctions.h"
#include "Bank.h"
#include "Player.h"
#include "Canvas.h"
#include "GameStartPanel.h"
#include "InfoPanel.h"
#include "cocostudio/SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;


GameScene::~GameScene()
{
	m_Player = nullptr;
	delete m_Canvas;
	m_Canvas = nullptr;
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

	if (Director::getInstance()->isPaused())
		Director::getInstance()->resume();

	//init data
	GameData::getInstance().init();

#pragma region Create In-Game UI panels
	m_Canvas = new Canvas;
	m_Canvas->Init(this, GameData::getInstance().m_Player);
#pragma endregion

	// Game time setup
	this->scheduleUpdate();

	// set key event
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	
	return true;
}

void GameScene::update(float delta)
{
	m_Canvas->update(delta);
}

void GameScene::gameOver()
{
	Director::getInstance()->pause();
	m_Canvas->gameOver();
}


void GameScene::setSpriteScale(Sprite* sprite, Vec2 scale)
{
	sprite->setScaleX((m_VisibleSize.width / sprite->getContentSize().width) * scale.x);
	sprite->setScaleY((m_VisibleSize.height / sprite->getContentSize().height) * scale.y);
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Director::getInstance()->end();
	}
}



