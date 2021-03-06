#pragma once

#include "UIPanel.h"

enum class EPanels;

class GameOverPanel : public UIPanel
{
public:
	virtual ~GameOverPanel();
	
	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;

	std::function<void(UIPanel*)> onDestroyCall;

private:
	void restart(cocos2d::Ref* pSender);
	void backToMenu(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* item, cocos2d::Event* event);
	void destroyPanel();
};