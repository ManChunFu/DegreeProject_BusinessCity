#pragma once

#include "UIPanel.h"

class GameScene;
class Shop;
class People;
struct SceneViewData;
namespace CocosDenshion
{
	class SimpleAudioEngine;
}

class SwitchSceneView : public UIPanel
{
public:
	virtual ~SwitchSceneView();

	void runInit(GameScene* gameScene, cocos2d::Size visibleSize, cocos2d::Vec2 origin, cocos2d::Vec2 sceneMidPoint);
	void switchView(unsigned id);
	void displayShopInMainScene(unsigned shopId);
	cocos2d::Sprite* getSceneView(unsigned viewId);
	void removeShopFromScene();

	std::function<void(unsigned shopId, unsigned productId, unsigned saleQuantity)> m_SaleHappensNotify;

protected:
	void clickIconCallback(cocos2d::Ref* pSender, unsigned viewId);
	void onBackMainCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);
	void onMouseLeave(MouseOverMenuItem* menuItem, cocos2d::Event* event);
	void onSaleHappens(unsigned sceneId, unsigned shopId, unsigned productId);

private:
	CocosDenshion::SimpleAudioEngine* m_Audio;
	SceneViewData* m_SceneViewData;
	cocos2d::Map<unsigned int, cocos2d::Sprite*> m_SceneViewMaps;
	cocos2d::Vector<cocos2d::MenuItem*> m_BackMainButtons;
	cocos2d::Sprite* m_CurrentView = nullptr;
	cocos2d::Sprite* m_MoneyIcon = nullptr;
	Shop* m_PlayerShop = nullptr;
	People* m_People = nullptr;

	unsigned m_ShopSceneId = 0;

	cocos2d::Size m_VisibleSize = cocos2d::Size::ZERO;
	cocos2d::Vec2 m_Origin = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 m_SceneMidPoint = cocos2d::Vec2::ZERO;

	std::array<std::string, 3> m_MapIcons = { "IconMap_Small.png", "IconMap_Small_Lit.png", "IconMap_Small.png" };
	float m_MovingDuration = 1.f;
	float m_MoveUp = 3.f;

	void createOrderedView(unsigned id);
	void fadeEffect(cocos2d::Sprite* viewSprite, bool fadeIn);
	void createMapIcon(const std::string& normal, const std::string& mouseOver, const std::string& disable, cocos2d::Vec2 displayPos, 
		unsigned viewId, float iconScale = 1.f, cocos2d::Vec2 parentPos = cocos2d::Vec2(10.f, -100.f));
	void createSceneViewMaps();
	void createShopInCloseSceneView(unsigned sceneId);
	void createBackMainButton();
	void enableBackMainButtons(bool enable);
	void setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale);
};