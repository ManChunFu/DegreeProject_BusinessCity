#pragma once
#include "Canvas.h"

class GameScene;
class MouseOverMenuItem;
class Shop;

class GameStartPanel : public Canvas
{
public:
	virtual ~GameStartPanel();
	void createPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint);

private:
	std::vector<cocos2d::Node*> m_Elements;
	GameScene* m_GameScene = nullptr;
	cocos2d::Vector<cocos2d::MenuItem*> m_StartupItems;
	void selectedItemCallback(cocos2d::Ref* pSender, unsigned shopID);
	void goButtonCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
	void registerStartupChoice();
};
