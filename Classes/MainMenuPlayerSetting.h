#pragma once

#include "cocos2d.h"
#include "ui/UITextField.h"

USING_NS_CC;

class MainMenuPlayerSetting 
{
public:
	~MainMenuPlayerSetting();
	void OpenSettingWindow(Scene* scene);
	void CloseSettingWindow(Scene* scene);
	
private:
	Sprite* playerSettingPanel = nullptr;
	ui::TextField* textField;
	bool hasCreated = false;
};
