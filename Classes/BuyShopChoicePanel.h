#pragma once

#include "UIPanel.h"

class SwitchSceneView;
class MouseOverMenuItem;
class BuyShopChoicePanel : public UIPanel
{
public:
	~BuyShopChoicePanel() override;

	void openPanel(SwitchSceneView* sceneView, cocos2d::Vec2 sceneMidPoint, unsigned sceneId, unsigned startupId);
	void closePanel() override;

	std::function<void(unsigned shopId, unsigned sceneId)> m_OnPlayerPurchase;
protected:
	void createPanel(cocos2d::Vec2 sceneMidPoint, unsigned sceneId, unsigned startupId);
	void closeCallback(cocos2d::Ref* pSender);
	void selectedItemCallback(cocos2d::Ref* pSender, unsigned shopId);
	void BuyButtonCallback(cocos2d::Ref* pSender);

	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);

private:
	SwitchSceneView* m_SceneView;

	unsigned m_SceneIdForPanel = 0;
	unsigned m_SelectedShopId = 0;
};
