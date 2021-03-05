#include "MainMenuPlayerSetting.h"
#include "2d/CCSprite.h"
#include "ui/UICheckBox.h"
#include "MouseOverMenuItem.h"

MainMenuPlayerSetting::~MainMenuPlayerSetting()
{
	delete playerSettingPanel;
	playerSettingPanel = nullptr;

	for (auto item : menuItems)
	{
		delete item;
	}
	menuItems.clear();

	delete textField;
	textField = nullptr;

	hasCreated = false;
}

void MainMenuPlayerSetting::OpenSettingWindow(Scene* scene)
{
	if (hasCreated)
	{
		scene->reorderChild(playerSettingPanel, 2);
		for (auto item : menuItems)
		{
			item->setEnabled(true);
			item->setVisible(true);
		}
		return;
	}

	createPlayerSettingWindow(scene);
}

void MainMenuPlayerSetting::closeSettingWindow(Scene* scene)
{
	scene->reorderChild(playerSettingPanel, -1);
	for (auto item : menuItems)
	{
		item->setEnabled(false);
		item->setVisible(false);
	}
}

void MainMenuPlayerSetting::createPlayerSettingWindow(Scene* scene)
{
	hasCreated = true;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sceneMidPoint = Point(origin.x + (visibleSize.width * 0.5f), origin.y + (visibleSize.height * 0.5f));
	// background
	playerSettingPanel = Sprite::createWithSpriteFrameName("UIPanelRecBlack80.png");
	playerSettingPanel->setPosition(sceneMidPoint);
	playerSettingPanel->setScale(0.8f);

	auto panelMidPoint = Vec2(playerSettingPanel->getContentSize().width * 0.5f, playerSettingPanel->getContentSize().height * 0.5f);

	// player name
	textField = ui::TextField::create("Enter your name", "Arial", 30.f);
	textField->setColor(Color3B::WHITE);
	textField->setMaxLengthEnabled(true);
	textField->setMaxLength(24);
	textField->setPosition(Vec2(panelMidPoint.x, panelMidPoint.y + 250.f));
	textField->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) { textField->setCursorEnabled(true); });
	playerSettingPanel->addChild(textField, 1);

	// option label
	auto optionLabel = Label::createWithTTF("CHOOSE YOUR PLAYER", "fonts/BROADW.TTF", 20.f);
	optionLabel->setTextColor(Color4B(255, 0, 150, 255));
	optionLabel->setPosition(panelMidPoint.x, panelMidPoint.y + 200.f);
	playerSettingPanel->addChild(optionLabel, 1);

	// woman1
	auto woman1NormalSprite = Sprite::createWithSpriteFrameName("Woman1_200_Tran.png");
	auto woman1SelectedSprite = Sprite::createWithSpriteFrameName("Woman1_200_Tran_Lit.png");
	auto woman1DisabledSprite = Sprite::createWithSpriteFrameName("Woman1_200_Tran_Disabled.png");
	auto woman1Item = MouseOverMenuItem::create(woman1NormalSprite, woman1SelectedSprite, woman1DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::selectedCallback, this));
	woman1Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 woman1Pos = Vec2(sceneMidPoint.x - 100.f, sceneMidPoint.y + 100.f);
	woman1Item->setScale(0.4f);
	woman1Item->setPosition(woman1Pos);
	woman1Item->setItemRect(woman1Pos, 0.4f);

	menuItems.pushBack(woman1Item);

	// woman2
	auto woman2NormalSprite = Sprite::createWithSpriteFrameName("Woman2_200_Tran.png");
	auto woman2SelectedSprite = Sprite::createWithSpriteFrameName("Woman2_200_Tran_Lit.png");
	auto woman2DisabledSprite = Sprite::createWithSpriteFrameName("Woman2_200_Tran_Disabled.png");
	auto woman2Item = MouseOverMenuItem::create(woman2NormalSprite, woman2SelectedSprite, woman2DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::selectedCallback, this));
	woman2Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 woman2Pos = Vec2(sceneMidPoint.x, sceneMidPoint.y + 100.f);
	woman2Item->setScale(0.4f);
	woman2Item->setPosition(woman2Pos);
	woman2Item->setItemRect(woman2Pos, 0.4f);

	menuItems.pushBack(woman2Item);

	// woman3
	auto woman3NormalSprite = Sprite::createWithSpriteFrameName("Woman3_200_Tran.png");
	auto woman3SelectedSprite = Sprite::createWithSpriteFrameName("Woman3_200_Tran_Lit.png");
	auto woman3DisabledSprite = Sprite::createWithSpriteFrameName("Woman3_200_Tran_Disabled.png");
	auto woman3Item = MouseOverMenuItem::create(woman3NormalSprite, woman3SelectedSprite, woman3DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::selectedCallback, this));
	woman3Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 woman3Pos = Vec2(sceneMidPoint.x + 100.f, sceneMidPoint.y + 100.f);
	woman3Item->setScale(0.4f);
	woman3Item->setPosition(woman3Pos);
	woman3Item->setItemRect(woman3Pos, 0.4f);

	menuItems.pushBack(woman3Item);

	// man1
	auto man1NormalSprite = Sprite::createWithSpriteFrameName("Man1_200_Tran.png");
	auto man1_SelectedSprite = Sprite::createWithSpriteFrameName("Man1_200_Tran_Lit.png");
	auto man1DisabledSprite = Sprite::createWithSpriteFrameName("Man1_200_Tran_Disabled.png");
	auto man1Item = MouseOverMenuItem::create(man1NormalSprite, man1_SelectedSprite, man1DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::selectedCallback, this));
	man1Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 man1Pos = Vec2(sceneMidPoint.x - 100.f, sceneMidPoint.y);
	man1Item->setScale(0.4f);
	man1Item->setPosition(man1Pos);
	man1Item->setItemRect(man1Pos, 0.4f);

	menuItems.pushBack(man1Item);

	// man2
	auto man2NormalSprite = Sprite::createWithSpriteFrameName("Man2_200_Tran.png");
	auto man2SelectedSprite = Sprite::createWithSpriteFrameName("Man2_200_Tran_Lit.png");
	auto man2DisabledSprite = Sprite::createWithSpriteFrameName("Man2_200_Tran_Disabled.png");
	auto man2Item = MouseOverMenuItem::create(man2NormalSprite, man2SelectedSprite, man2DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::selectedCallback, this));
	man2Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 man2Pos = Vec2(sceneMidPoint.x, sceneMidPoint.y);
	man2Item->setScale(0.4f);
	man2Item->setPosition(man2Pos);
	man2Item->setItemRect(man2Pos, 0.4f);

	menuItems.pushBack(man2Item);

	// man3
	auto man3NormalSprite = Sprite::createWithSpriteFrameName("Man3_200_Tran.png");
	auto man3SelectedSprite = Sprite::createWithSpriteFrameName("Man3_200_Tran_Lit.png");
	auto man3DisabledSprite = Sprite::createWithSpriteFrameName("Man3_200_Tran_Disabled.png");
	auto man3Item = MouseOverMenuItem::create(man3NormalSprite, man3SelectedSprite, man3DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::selectedCallback, this));
	man3Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 man3Pos = Vec2(sceneMidPoint.x + 100.f, sceneMidPoint.y);
	man3Item->setScale(0.4f);
	man3Item->setPosition(man3Pos);
	man3Item->setItemRect(man3Pos, 0.4f);

	menuItems.pushBack(man3Item);

	auto panelMenu = Menu::createWithArray(menuItems);
	panelMenu->setPosition(Vec2::ZERO);

	scene->addChild(playerSettingPanel, 2);
	scene->addChild(panelMenu, 3);
}

void MainMenuPlayerSetting::selectedCallback(Ref* pSender)
{
	for (auto item : menuItems)
	{
		if (item == pSender)
		{
			if (item->itemSelectedData.isSelected)
			{
				item->itemSelectedData.isSelected = false;
				item->itemSelectedData.selectedLabel->setVisible(false);
				item->unselected();
				for (auto item : menuItems)
				{
					item->setEnabled(true);
				}
				return;
			}
			else
			{
				// create label for first time
				item->itemSelectedData.isSelected = true;
				item->itemSelectedData.selectedLabel = Label::createWithTTF("SELECTED", "fonts/BROADW.TTF", 30.f);
				item->itemSelectedData.selectedLabel->setTextColor(Color4B(255, 0, 0, 255));
				item->itemSelectedData.selectedLabel->enableShadow(Color4B::WHITE);
				item->itemSelectedData.selectedLabel->setPosition(item->getContentSize().width * 0.5f, item->getContentSize().height * 0.5f - 60.f);
				item->itemSelectedData.selectedLabel->setRotation(-18.f);
				item->addChild(item->itemSelectedData.selectedLabel, 1);
				item->selected();
			}
		}
		else
			item->setEnabled(false);
	}
}

void MainMenuPlayerSetting::onMouseOver(MouseOverMenuItem* overItem, Event* event)
{

}

