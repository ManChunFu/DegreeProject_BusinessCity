#pragma once

#include "UIPanel.h"

class Shop;
class MyShopSettingPanel;

class ActionPanel : public UIPanel
{
public:
	~ActionPanel() override;

	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;
	void displayShop();

	void setShopOpen(bool isOpen) { m_IsShopOpen = isOpen; }
protected:
	void openShopCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
private:
	Shop* m_MyShop = nullptr;
	MyShopSettingPanel* m_ShopButton = nullptr;

	bool m_IsShopOpen = false;
	cocos2d::Vec2 m_SceneMidPoint;
};