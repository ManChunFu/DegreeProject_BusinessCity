#pragma once
#include "2d/CCNode.h"

class GameScene;
class MouseOverMenuItem;
class Player;
class Shop;

class GameStartPanel : public cocos2d::Node
{
public:
	virtual ~GameStartPanel();
	void createPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint);

	std::function<void(cocos2d::Ref* pSender)> onDestroyCall;
private:
	std::vector<cocos2d::Node*> m_Elements;
	GameScene* m_GameScene = nullptr;
	cocos2d::Vector<cocos2d::MenuItem*> m_StartupItems;
	Player* m_Player;
	void selectedItemCallback(cocos2d::Ref* pSender, unsigned shopID);
	void goButtonCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
};
