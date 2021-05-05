#pragma once
#include "UIPanel.h"

class Shop;
class SwitchSceneView;
enum class EPanels;

class GameStartPanel : public UIPanel
{
public:
	GameStartPanel(SwitchSceneView* mainScene, GameScene* scene, cocos2d::Vec2 sceneMidPoint);
	~GameStartPanel() override;
	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;
	void closePanel() override;

	std::function<void(UIPanel*, unsigned, EPanels)> onDestroyCall;

protected:
	void createPanel(cocos2d::Vec2 sceneMidPoint, unsigned shopId = 0) override;

	void closeCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);

private:
	SwitchSceneView* m_MainSceneView = nullptr;
	unsigned m_SelectedShopId = 0;
	void selectedItemCallback(cocos2d::Ref* pSender, unsigned shopId);
	void goButtonCallback(cocos2d::Ref* pSender);
};
