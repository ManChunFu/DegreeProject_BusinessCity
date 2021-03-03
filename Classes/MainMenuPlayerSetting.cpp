#include "MainMenuPlayerSetting.h"
#include "2d/CCSprite.h"
#include "MainMenuScene.h"

MainMenuPlayerSetting::~MainMenuPlayerSetting()
{
	playerSettingPanel = nullptr;
	hasCreated = false;
}

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

	playerSettingPanel = Sprite::createWithSpriteFrameName("UIPanelRecBlack80.png");
	playerSettingPanel->setPosition(Point(origin.x + (visibleSize.width / 2), origin.y + (visibleSize.height / 2)));
	playerSettingPanel->setScale(0.5);


	textField = ui::TextField::create("Enter your name", "Arial", 50);
	textField->setColor(Color3B::WHITE);
	textField->setPosition(Vec2(playerSettingPanel->getContentSize().width / 2, playerSettingPanel->getContentSize().height - 100));
	textField->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		textField->setTextColor(Color4B::MAGENTA);
		});

	playerSettingPanel->addChild(textField, 2);
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
			item->setVisible(true);
		}
	}
}

