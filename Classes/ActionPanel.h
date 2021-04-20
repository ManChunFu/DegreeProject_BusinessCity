#pragma once

#include "UIPanel.h"

class Shop;
class MyShopSettingPanel;

class ActionPanel : public UIPanel
{
public:
	~ActionPanel() override;

	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;
	void displayShop(unsigned shopId, cocos2d::Vec2 shopPosition = cocos2d::Vec2::ZERO);
	void displayShopOptions();

protected:
	void openShopCallback(cocos2d::Ref* pSender, unsigned shopId);

	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
	void checkShopCallback(cocos2d::Ref* pSender, unsigned shopId);
	//void onShopChanges(unsigned shopId, unsigned shopListIndex, cocos2d::Vec2 shopPos);
	void onShopChanges(unsigned shopId, cocos2d::Node* menu, cocos2d::Vec2 shopPos);

private:
	MyShopSettingPanel* m_ShopButton = nullptr;
	cocos2d::Vector<MyShopSettingPanel*> m_MyShopList;
	std::unordered_map<unsigned, MyShopSettingPanel*> m_MyShopMap;
	unsigned m_CurrentOpenShopId = 0;

	cocos2d::Vec2 m_SceneMidPoint = cocos2d::Vec2::ZERO;
	cocos2d::Vec2 m_DisplayShopPos = cocos2d::Vec2::ZERO;

	void removeShop(unsigned shopIndex);

};