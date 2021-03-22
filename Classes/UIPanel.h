#pragma once

#include "cocos2d.h"
#include "2d/CCNode.h"

class GameScene;
class Player;

class UIPanel : public cocos2d::Node 
{
public:
	virtual ~UIPanel();

	virtual void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) {};
	virtual void closePanel() {};

protected:
	std::vector<cocos2d::Node*> m_Elements;
	cocos2d::Vector<cocos2d::MenuItem*> m_MenuItems;
	GameScene* m_GameScene = nullptr;
	cocos2d::Sprite* m_ThisPanel = nullptr;
	Player* m_Player = nullptr;
	virtual void createPanel(cocos2d::Vec2 sceneMidPoint) {};
};
