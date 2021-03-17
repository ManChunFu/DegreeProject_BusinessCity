#pragma once

#include "cocos2d.h"
#include "GameFunctions.h"
#include "GameData.h"

class GameScene;
class InfoPanel;
class GameStartPanel;

class Canvas  
{
public:
	virtual ~Canvas();

	void Init(GameScene* scene, Player* player, cocos2d::Vec2 sceneMidPoint);
	
	void update(float deltaTime);
private:
	std::vector<cocos2d::Node*> m_UIPanels;
	InfoPanel* m_InfoPanel;
	GameStartPanel* m_GameStartPanel;
	cocos2d::Size m_VisibleSize;
	cocos2d::Vec2 m_origin;
	
	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);
	void destroyPanel(cocos2d::Ref* pSender);
	
};
