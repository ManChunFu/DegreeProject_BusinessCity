#pragma once

#include "UIPanel.h"

class Shop;
class MyShopSettingPanel;
class BuyShopChoicePanel;
class SwitchSceneView;

class ActionPanel : public UIPanel
{
public:
	~ActionPanel() override;

	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;
	void displayShop(unsigned shopId, cocos2d::Vec2 shopPosition = cocos2d::Vec2::ZERO);
	void setMainScenePointer(SwitchSceneView* mainScene) { m_MainScene = mainScene; };

	std::function<void(unsigned removeShopId, unsigned upgradeShopId)> m_OnUpgradeShopCalls;
protected:
	void openShopCallback(cocos2d::Ref* pSender, unsigned shopId);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
	void onShopChanges(unsigned shopId, cocos2d::Node* menu, cocos2d::Vec2 shopPos);
	void openShopChoiceNotify(unsigned sceneId, unsigned startupId);
	void onPlayerPurchase(unsigned shopId, unsigned sceneId);
private:
	SwitchSceneView* m_MainScene = nullptr;
	MyShopSettingPanel* m_ShopButton = nullptr;
	BuyShopChoicePanel* m_ShopChoicePanel = nullptr;
	cocos2d::Vector<MyShopSettingPanel*> m_MyShopList;
	std::unordered_map<unsigned, MyShopSettingPanel*> m_MyShopMap;
	std::vector<unsigned int> m_TempEmployeeIds;
	unsigned m_CurrentOpenShopId = 0;

	cocos2d::Vec2 m_SceneMidPoint = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 m_DisplayShopPos = cocos2d::Vec2::ZERO;

	void removeShop(unsigned shopIndex);

};