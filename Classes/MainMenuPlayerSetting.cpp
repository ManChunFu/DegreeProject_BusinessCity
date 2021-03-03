#include "MainMenuPlayerSetting.h"
#include "2d/CCSprite.h"
#include "MainMenuScene.h"


void MainMenuPlayerSetting::OpenSettingWindow(Scene* scene)
{
	if (hasCreated)
	{
		scene->reorderChild(playerSettingPanel, 2);
		return;
	}

	hasCreated = true;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	playerSettingPanel = Sprite::create("X/UIPanel.png");
	playerSettingPanel->setPosition(Point(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)));
	scene->addChild(playerSettingPanel, 2);
}

void MainMenuPlayerSetting::CloseSettingWindow(Scene* scene)
{
	scene->reorderChild(playerSettingPanel, -1);
	MainMenuScene* menu = dynamic_cast<MainMenuScene*>(scene);
	if (menu)
	{
		for (auto item : menu->menuItems)
		{
			item->setEnabled(true);
		}
	}
}

