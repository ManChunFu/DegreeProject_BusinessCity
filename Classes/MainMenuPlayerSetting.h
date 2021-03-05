#pragma once

#include "cocos2d.h"
#include "ui/UITextField.h"

USING_NS_CC;
class MouseOverMenuItem;

class MainMenuPlayerSetting 
{
public:
	~MainMenuPlayerSetting();
	
	void OpenSettingWindow(Scene* scene);
	void closeSettingWindow(Scene* scene);
	
private:
	Vector<MenuItem*> menuItems;
	Sprite* playerSettingPanel = nullptr;
	ui::TextField* textField = nullptr;
	bool hasCreated = false;
	void createPlayerSettingWindow(Scene* scene);
	void selectedCallback(Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, Event* event);
	
};
