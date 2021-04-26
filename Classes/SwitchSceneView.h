#pragma once

#include "UIPanel.h"

class GameScene;

class SwitchSceneView : public UIPanel
{
public:
	virtual ~SwitchSceneView();

	void runInit(GameScene* gameScene, cocos2d::Size visibleSize, cocos2d::Vec2 origin, cocos2d::Vec2 sceneMidPoint);
	void switchView(unsigned id);
	void displayShopInMainScene(unsigned shopId);

protected:
	void clickIconCallback(cocos2d::Ref* pSender, unsigned viewId);
	void onBackMainCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);
	void onMouseLeave(MouseOverMenuItem* menuItem, cocos2d::Event* event);

private:
	enum EViews
	{
		E_Main = 0,
		E_Hotel = 1,
		E_Playground = 2
	};
	cocos2d::Map<unsigned int, cocos2d::Sprite*> m_SceneViewMaps;
	cocos2d::Vector<cocos2d::MenuItem*> m_BackMainButtons;
	cocos2d::Sprite* m_CurrentView = nullptr;

	cocos2d::Size m_VisibleSize = cocos2d::Size::ZERO;
	cocos2d::Vec2 m_Origin = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 m_SceneMidPoint = cocos2d::Vec2::ZERO;

	float m_MovingDuration = 1.f;
	float m_MoveUp = 3.f;

	void createOrderedView(unsigned id);
	void fadeEffect(cocos2d::Sprite* viewSprite, bool fadeIn);
	void createMapIcon(const std::string& normal, const std::string& mouseOver, const std::string& disable, cocos2d::Vec2 displayPos, unsigned viewId, float iconScale = 1.f);
	void createSceneViewMaps();
	void createBackMainButton();
	void enableBackMainButtons(bool enable);
	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);
};