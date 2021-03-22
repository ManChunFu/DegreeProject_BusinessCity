#pragma once

#include "UIPanel.h"

class Player;

class MyShopSettingPanel : public UIPanel
{
public:
	virtual ~MyShopSettingPanel();

	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;
	void closePanel() override;
	std::function<void(cocos2d::Ref* pSender)> onActionCall;

protected:	
	void createPanel(cocos2d::Vec2 sceneMidPoint) override;

};