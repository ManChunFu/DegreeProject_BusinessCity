#pragma once

#ifndef __GAME_SCENE__H
#define __GAME_SCENE__H

#include "cocos2d.h"

namespace CocosDenshion
{
	class SimpleAudioEngine;
}
class GlobalTime;
class Player;
class Canvas;
class GameLoop;
enum struct EGameStates;

class GameScene : public cocos2d::Layer
{
public:
	~GameScene();

	static cocos2d::Scene* createScene();
	virtual bool init() override;
	CREATE_FUNC(GameScene);
	
	void update(float delta);
	void gameOver();
private:
	GlobalTime* m_GlobalTime = nullptr;
	GameLoop* m_GameLoop = nullptr;
	Canvas* m_Canvas = nullptr;
	Player* m_Player = nullptr;
	EGameStates m_EGameState;
	float m_BackgroundMusicVolume = 0.5f;

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};
#endif
