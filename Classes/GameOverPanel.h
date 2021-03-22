#pragma once
#include "cocos2d.h"
#include "2d/CCNode.h"

class GameScene;
class MouseOverMenuItem;

class GameOverPanel : public cocos2d::Node
{
public:
	virtual ~GameOverPanel();
	
	void init(GameScene* scene, cocos2d::Vec2 sceneMidPoint);
	std::function<void(cocos2d::Ref* pSender)> onDestroyCall;
private:
	std::vector<cocos2d::Node*> m_Elements;
	GameScene* m_GameScene;
	cocos2d::Sprite* m_GameoverPanel = nullptr;
	cocos2d::Vector<cocos2d::MenuItem*> m_GameOverItems;

	void restart(cocos2d::Ref* pSender);
	void backToMenu(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* item, cocos2d::Event* event);
	void destroyPanel();
};