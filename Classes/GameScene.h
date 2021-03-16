#pragma once

#ifndef __GAME_SCENE__H
#define __GAME_SCENE__H

#include "cocos2d.h"

namespace CocosDenshion
{
	class SimpleAudioEngine;
}
class MouseOverMenuItem;
class Bank;
class Player;
class GameStartPanel;
class InfoPanel;

class GameScene : public cocos2d::Layer
{
public:
	~GameScene();

	static cocos2d::Scene* createScene();

	virtual bool init() override;
	
	void update(float delta);
	CREATE_FUNC(GameScene);

	Player* m_Player = nullptr;
	void deleteStartupPanel();
private:
	InfoPanel* m_InfoPanel = nullptr;
	GameStartPanel* m_StartupPanel = nullptr;
	cocos2d::Size m_VisibleSize;

	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};
#endif
