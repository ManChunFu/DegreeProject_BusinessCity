#pragma once
#include "Canvas.h"

class GameScene;
class MouseOverMenuItem;

class GameStartPanel : public Canvas
{
public:
	~GameStartPanel() { m_StartupItems.clear(); }
	void createPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint);

private:
	cocos2d::Vector<cocos2d::MenuItem*> m_StartupItems;
	void selectedItemCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
	
};
