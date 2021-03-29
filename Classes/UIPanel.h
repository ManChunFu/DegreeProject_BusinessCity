#pragma once

#include "cocos2d.h"
#include "2d/CCNode.h"

class GameScene;
class Player;
class MouseOverMenuItem;

class UIPanel : public cocos2d::Node 
{
public:
	virtual ~UIPanel();

	virtual void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) {};
	virtual void closePanel() {};

	bool isPanelOpen() { return m_IsPanelOpen; }

protected:
	std::vector<cocos2d::Node*> m_Elements;
	cocos2d::Vector<cocos2d::MenuItem*> m_MenuItems;
	GameScene* m_GameScene = nullptr;
	cocos2d::Sprite* m_ThisPanel = nullptr;
	Player* m_Player = nullptr;

	bool m_IsPanelOpen = false;

	virtual void createPanel(cocos2d::Vec2 sceneMidPoint) {};

	MouseOverMenuItem* displayMenuButton(MouseOverMenuItem* button, std::function<void(MouseOverMenuItem* overItem, cocos2d::Event* event)> onMouseOver, cocos2d::Vec2 pos, cocos2d::itemTypes type = cocos2d::itemTypes::DEFAULT, float scale = 1.f);
};
