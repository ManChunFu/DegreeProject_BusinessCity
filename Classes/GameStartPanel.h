#pragma once
#include "UIPanel.h"

class Shop;

class GameStartPanel : public UIPanel
{
public:
	~GameStartPanel() override;
	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;

	std::function<void(cocos2d::Ref* pSender)> onDestroyCall;
private:
	void selectedItemCallback(cocos2d::Ref* pSender, unsigned shopID);
	void goButtonCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
};
