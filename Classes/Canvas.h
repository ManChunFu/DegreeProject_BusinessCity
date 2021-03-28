#pragma once

#include "cocos2d.h"
#include "GameFunctions.h"
#include "GameData.h"


class GameScene;
class InfoPanel;
class ActionPanel;
class GameStartPanel;
class GameOverPanel;
class MyShopSettingPanel;
enum class EPanels;

class Canvas  
{
public:
	virtual ~Canvas();

	void Init(GameScene* scene, Player* player);
	
	void update(float deltaTime);
	void gameOver();

private:
	std::vector<cocos2d::Node*> m_UIPanels;
	std::vector<cocos2d::Node*> m_AddPanels;
	std::vector<cocos2d::Ref*> m_RemovePanels;
	GameScene* m_GameScene = nullptr;
	InfoPanel* m_InfoPanel = nullptr;
	ActionPanel* m_ActionPanel = nullptr;
	GameStartPanel* m_GameStartPanel = nullptr;
	GameOverPanel* m_GameoverPanel = nullptr;

	cocos2d::Size m_VisibleSize;
	cocos2d::Vec2 m_Origin;
	cocos2d::Vec2 m_SceneMidPoint;
	
	void destroyPanel(cocos2d::Ref* pSender, EPanels uiPanel);
	void actionCall(cocos2d::Ref* pSender, EPanels uiPanel);
	void activePanel(EPanels uiPanel);
	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);
	
};
