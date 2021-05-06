#include "GameScene.h"
#include "2d/CCLayer.h"
#include "GameData.h"
#include "GlobalTime.h"
#include "GameTime.h"
#include "GameLoop.h"
#include "Player.h"
#include "Canvas.h"
#include "UIPanel.h"
#include "EGameStates.h"
#include "cocostudio/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

GameScene::~GameScene()
{
	m_Player = nullptr;

	delete m_Canvas;
	m_Canvas = nullptr;

	delete m_GameLoop;
	m_GameLoop = nullptr;
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
	m_GlobalTime = GameData::getInstance().m_GlobalTime;

	m_Player = GameData::getInstance().m_Player;

#pragma region Create In-Game UI panels
	m_Canvas = new Canvas;
	m_Canvas->Init(this, GameData::getInstance().m_Player);
#pragma endregion

	// init GameLoop
	m_GameLoop = new GameLoop();

	// Game time setup
	this->scheduleUpdate();
	m_EGameState = EGameStates::RUNNING;

	// set key event
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GameScene::update(float delta)
{
	if (m_EGameState != EGameStates::RUNNING)
		return;

	m_GlobalTime->update(delta);

	if (m_Canvas)
		m_Canvas->update(delta);

}

void GameScene::gameOver()
{
	Director::getInstance()->pause();
	m_EGameState = EGameStates::PAUSE;
	m_Canvas->gameOver();
}


void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (GameData::getInstance().isAnyPanelOpen())
			GameData::getInstance().m_TempOpenPanel->closePanel();
		else
			Director::getInstance()->end();
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_0)
		m_Player->updateCurrentCashAmout(50000);

	if (keyCode == EventKeyboard::KeyCode::KEY_9)
		m_Player->updateCurrentCashAmout(-50000);

	if (keyCode == EventKeyboard::KeyCode::KEY_T)
		m_GlobalTime->speedUp();

	if (keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		m_BackgroundMusicVolume -= 0.05f;
		GameData::getInstance().m_Audio->setBackgroundMusicVolume(m_BackgroundMusicVolume);
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		m_BackgroundMusicVolume += 0.05f;
		GameData::getInstance().m_Audio->setBackgroundMusicVolume(m_BackgroundMusicVolume);
	}


}



