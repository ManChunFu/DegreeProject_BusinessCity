#pragma once
#include "UIPanel.h"

class Shop;
enum class EPanels;

class GameStartPanel : public UIPanel
{
public:
	~GameStartPanel() override;
	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;

	std::function<void(UIPanel*, EPanels)> onDestroyCall;
private:
	void selectedItemCallback(cocos2d::Ref* pSender, unsigned shopID);
	void goButtonCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
};
