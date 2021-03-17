#pragma once

#include "cocos2d.h"
#include "GameFunctions.h"
#include "GameData.h"

class GameScene;
class InfoPanel;
class GameStartPanel;
class GameOverPanel;

class Canvas  
{
public:
	virtual ~Canvas();

	void Init(GameScene* scene, Player* player);
	
	void update(float deltaTime);
	void gameOver(GameScene* scene);
private:
	std::vector<cocos2d::Node*> m_UIPanels;
	InfoPanel* m_InfoPanel;
	GameStartPanel* m_GameStartPanel;
	GameOverPanel* m_GameoverPanel;
	cocos2d::Size m_VisibleSize;
	cocos2d::Vec2 m_Origin;
	cocos2d::Vec2 m_SceneMidPoint;
	
	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);
	void destroyPanel(cocos2d::Ref* pSender);
	
};
