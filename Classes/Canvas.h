#pragma once

#include "cocos2d.h"
#include "GameFunctions.h"
#include "GameData.h"


class GameScene;
class SwitchSceneView;
class UIPanel;
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
	cocos2d::Vector<UIPanel*> m_UIPanels;
	cocos2d::Vector<UIPanel*> m_AddPanels;
	cocos2d::Vector<UIPanel*> m_RemovePanels;
	GameScene* m_GameScene = nullptr;
	SwitchSceneView* m_SwitchSceneView = nullptr;
	InfoPanel* m_InfoPanel = nullptr;
	ActionPanel* m_ActionPanel = nullptr;
	GameStartPanel* m_GameStartPanel = nullptr;
	GameOverPanel* m_GameoverPanel = nullptr;

	cocos2d::Vec2 m_SceneMidPoint;
	
	void destroyPanel(UIPanel* panel, unsigned shopId, EPanels uiPanel);
	void activePanel(EPanels uiPanel, unsigned shopId);
	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);
	
};
