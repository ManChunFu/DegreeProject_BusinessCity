#pragma once

#include "cocos2d.h"
#include "ui/UITextField.h"

USING_NS_CC;

class MouseOverMenuItem;
class MainMenuScene;

class MainMenuPlayerSetting 
{
public:
	~MainMenuPlayerSetting();
	
	void OpenSettingWindow(Scene* scene);
	void closeSettingWindow();
	void destroy();
private:
	MainMenuScene* mainMenu;
	Vector<MenuItem*> menuItems;
	Sprite* playerSettingPanel = nullptr;
	ui::TextField* textField = nullptr;
	bool hasSelected = false;
	void createPlayerSettingWindow();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void characterSelectedCallback(Ref* pSender);
	void playButtonSelectedCallback(Ref* pSender);
	void cancelButtonSelectedCallback(Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, Event* event);
	bool validation();
	void showInvalid();
};