#pragma once

#include "UIPanel.h"

NS_CC_BEGIN;
namespace ui
{
	class TextField;
};
NS_CC_END;

namespace base
{
	class EventListenrKeyboard;
}
class MouseOverMenuItem;
class MainMenuScene;

class MainMenuPlayerSetting : public UIPanel
{
public:
	~MainMenuPlayerSetting();
	
	void OpenSettingWindow(cocos2d::Scene* scene);
	void closeSettingWindow();

private:
	MainMenuScene* m_MainMenu;
	//cocos2d::Vector<cocos2d::MenuItem*> m_MenuItems;
	cocos2d::Sprite* m_PlayerSettingPanel = nullptr;
	cocos2d::ui::TextField* m_TextField = nullptr;
	bool m_HasSelected = false;
	void createPlayerSettingWindow();
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void characterSelectedCallback(cocos2d::Ref* pSender);
	void playButtonSelectedCallback(cocos2d::Ref* pSender);
	void cancelButtonSelectedCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
	bool validation();
	void showInvalid();
};
