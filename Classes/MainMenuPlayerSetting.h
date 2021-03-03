#pragma once

#include "cocos2d.h"

USING_NS_CC;

class MainMenuPlayerSetting 
{
public:
	void OpenSettingWindow(Scene* scene);
	void CloseSettingWindow(Scene* scene);
	
private:
	Sprite* playerSettingPanel = nullptr;
	bool hasCreated = false;
};
