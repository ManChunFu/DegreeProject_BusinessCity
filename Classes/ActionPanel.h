#pragma once

#include "UIPanel.h"

class Shop;
class MyShopSettingPanel;

class ActionPanel : public UIPanel
{
public:
	~ActionPanel() override;

	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;
	void displayShop(unsigned shopId);
	void displayShopOptions();

protected:
	void openShopCallback(cocos2d::Ref* pSender, unsigned shopIndex, unsigned shopId);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
	void checkShopCallback(cocos2d::Ref* pSender, unsigned shopId);
private:
	MyShopSettingPanel* m_ShopButton = nullptr;
	cocos2d::Vector<MyShopSettingPanel*> m_MyShopList;
	unsigned m_ShopIndex = 0;

	cocos2d::Vec2 m_SceneMidPoint = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 m_DisplayShopPos = cocos2d::Vec2::ZERO;

};