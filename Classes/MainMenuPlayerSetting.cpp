#include "MainMenuPlayerSetting.h"
#include "MainMenuScene.h"
#include "2d/CCSprite.h"
#include "ui/UICheckBox.h"
#include "MouseOverMenuItem.h"
#include "GameScene.h"
#include "InGameData.h"
#include "2d/CCNode.h"


USING_NS_CC;

MainMenuPlayerSetting::~MainMenuPlayerSetting()
{
	m_MenuItems.clear();

	m_TextField = nullptr;

	m_HasSelected = false;

}

void MainMenuPlayerSetting::OpenSettingWindow(Scene* scene)
{
	if (!m_PlayerSettingPanel)
	{
		m_MainMenu = dynamic_cast<MainMenuScene*>(scene);

		if (m_MainMenu)
			createPlayerSettingWindow();
		return;
	}

	m_PlayerSettingPanel->setVisible(true);
	m_TextField->setPlaceHolder("ENTER YOUR NAME");
	m_TextField->setPlaceHolderColor(Color4B::GRAY);

	if (m_HasSelected)
	{
		for (auto item : m_MenuItems)
		{
			item->setVisible(true);
			if (item->itemSelectedData.isSelected || item->itemSelectedData.type == itemTypes::BUTTON)
				item->setEnabled(true);
		}
		return;
	}

	for (auto item : m_MenuItems)
	{
		item->setEnabled(true);
		item->setVisible(true);
	}	
}

void MainMenuPlayerSetting::closeSettingWindow()
{
	m_PlayerSettingPanel->setVisible(false);
	for (auto item : m_MenuItems)
	{
		item->setEnabled(false);
		item->setVisible(false);
	}
}

void MainMenuPlayerSetting::createPlayerSettingWindow()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sceneMidPoint = Point(origin.x + (visibleSize.width * 0.5f), origin.y + (visibleSize.height * 0.5f));

	// background
	m_PlayerSettingPanel = Sprite::createWithSpriteFrameName("UIPanelRecBlack80.png");
	m_PlayerSettingPanel->setPosition(sceneMidPoint);
	m_PlayerSettingPanel->setScale(0.8f);

	auto panelMidPoint = Vec2(m_PlayerSettingPanel->getContentSize().width * 0.5f, m_PlayerSettingPanel->getContentSize().height * 0.5f);

	// player name
	m_TextField = ui::TextField::create("ENTER YOUR NAME", "fonts/Nirmala.ttf", 30);
	m_TextField->setColor(Color3B::WHITE);
	m_TextField->setMaxLengthEnabled(true);
	m_TextField->setMaxLength(24);
	m_TextField->setPosition(Vec2(panelMidPoint.x, panelMidPoint.y + 240.f));
	m_TextField->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) { m_TextField->setCursorEnabled(true); });
	m_PlayerSettingPanel->addChild(m_TextField, 1);

	// option label
	auto optionLabel = Label::createWithTTF("CHOOSE YOUR PLAYER", "fonts/Nirmala.ttf", 20);
	optionLabel->setTextColor(colorType.HotPink);
	optionLabel->setPosition(panelMidPoint.x, panelMidPoint.y + 160.f);
	m_PlayerSettingPanel->addChild(optionLabel, 1);

#pragma region Create Character MenuItem
	// woman1
	auto woman1NormalSprite = Sprite::createWithSpriteFrameName("Woman1_200_Tran.png");
	auto woman1SelectedSprite = Sprite::createWithSpriteFrameName("Woman1_200_Tran_Lit.png");
	auto woman1DisabledSprite = Sprite::createWithSpriteFrameName("Woman1_200_Tran_Disabled.png");
	auto woman1Item = MouseOverMenuItem::create(woman1NormalSprite, woman1SelectedSprite, woman1DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	woman1Item->itemSelectedData.type = itemTypes::WOMAN1;
	woman1Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 woman1Pos = Vec2(sceneMidPoint.x - 100.f, sceneMidPoint.y + 60.f);
	woman1Item->setScale(0.4f);
	woman1Item->setPosition(woman1Pos);
	woman1Item->setItemRect(woman1Pos, 0.4f);

	m_MenuItems.pushBack(woman1Item);
	characterSpriteMap.insert(std::pair<itemTypes, std::string>(woman1Item->itemSelectedData.type, "Woman1_200_Tran.png"));

	// woman2
	auto woman2NormalSprite = Sprite::createWithSpriteFrameName("Woman2_200_Tran.png");
	auto woman2SelectedSprite = Sprite::createWithSpriteFrameName("Woman2_200_Tran_Lit.png");
	auto woman2DisabledSprite = Sprite::createWithSpriteFrameName("Woman2_200_Tran_Disabled.png");
	auto woman2Item = MouseOverMenuItem::create(woman2NormalSprite, woman2SelectedSprite, woman2DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	woman2Item->itemSelectedData.type = itemTypes::WOMAN2;
	woman2Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 woman2Pos = Vec2(sceneMidPoint.x, sceneMidPoint.y + 60.f);
	woman2Item->setScale(0.4f);
	woman2Item->setPosition(woman2Pos);
	woman2Item->setItemRect(woman2Pos, 0.4f);

	m_MenuItems.pushBack(woman2Item);
	characterSpriteMap.insert(std::pair<itemTypes, std::string>(woman2Item->itemSelectedData.type, "Woman2_200_Tran.png"));

	// woman3
	auto woman3NormalSprite = Sprite::createWithSpriteFrameName("Woman3_200_Tran.png");
	auto woman3SelectedSprite = Sprite::createWithSpriteFrameName("Woman3_200_Tran_Lit.png");
	auto woman3DisabledSprite = Sprite::createWithSpriteFrameName("Woman3_200_Tran_Disabled.png");
	auto woman3Item = MouseOverMenuItem::create(woman3NormalSprite, woman3SelectedSprite, woman3DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	woman3Item->itemSelectedData.type = itemTypes::WOMAN3;
	woman3Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 woman3Pos = Vec2(sceneMidPoint.x + 100.f, sceneMidPoint.y + 60.f);
	woman3Item->setScale(0.4f);
	woman3Item->setPosition(woman3Pos);
	woman3Item->setItemRect(woman3Pos, 0.4f);

	m_MenuItems.pushBack(woman3Item);
	characterSpriteMap.insert(std::pair<itemTypes, std::string>(woman3Item->itemSelectedData.type, "Woman3_200_Tran.png"));

	// man1
	auto man1NormalSprite = Sprite::createWithSpriteFrameName("Man1_200_Tran.png");
	auto man1_SelectedSprite = Sprite::createWithSpriteFrameName("Man1_200_Tran_Lit.png");
	auto man1DisabledSprite = Sprite::createWithSpriteFrameName("Man1_200_Tran_Disabled.png");
	auto man1Item = MouseOverMenuItem::create(man1NormalSprite, man1_SelectedSprite, man1DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	man1Item->itemSelectedData.type = itemTypes::MAN1;
	man1Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 man1Pos = Vec2(sceneMidPoint.x - 100.f, sceneMidPoint.y - 40.f);
	man1Item->setScale(0.4f);
	man1Item->setPosition(man1Pos);
	man1Item->setItemRect(man1Pos, 0.4f);

	m_MenuItems.pushBack(man1Item);
	characterSpriteMap.insert(std::pair<itemTypes, std::string>(man1Item->itemSelectedData.type, "Man1_200_Tran.png"));

	// man2
	auto man2NormalSprite = Sprite::createWithSpriteFrameName("Man2_200_Tran.png");
	auto man2SelectedSprite = Sprite::createWithSpriteFrameName("Man2_200_Tran_Lit.png");
	auto man2DisabledSprite = Sprite::createWithSpriteFrameName("Man2_200_Tran_Disabled.png");
	auto man2Item = MouseOverMenuItem::create(man2NormalSprite, man2SelectedSprite, man2DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	man2Item->itemSelectedData.type = itemTypes::MAN2;
	man2Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 man2Pos = Vec2(sceneMidPoint.x, sceneMidPoint.y - 40.f);
	man2Item->setScale(0.4f);
	man2Item->setPosition(man2Pos);
	man2Item->setItemRect(man2Pos, 0.4f);

	m_MenuItems.pushBack(man2Item);
	characterSpriteMap.insert(std::pair<itemTypes, std::string>(man2Item->itemSelectedData.type, "Man2_200_Tran.png"));

	// man3
	auto man3NormalSprite = Sprite::createWithSpriteFrameName("Man3_200_Tran.png");
	auto man3SelectedSprite = Sprite::createWithSpriteFrameName("Man3_200_Tran_Lit.png");
	auto man3DisabledSprite = Sprite::createWithSpriteFrameName("Man3_200_Tran_Disabled.png");
	auto man3Item = MouseOverMenuItem::create(man3NormalSprite, man3SelectedSprite, man3DisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	man3Item->itemSelectedData.type = itemTypes::MAN3;
	man3Item->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 man3Pos = Vec2(sceneMidPoint.x + 100.f, sceneMidPoint.y - 40.f);
	man3Item->setScale(0.4f);
	man3Item->setPosition(man3Pos);
	man3Item->setItemRect(man3Pos, 0.4f);

	m_MenuItems.pushBack(man3Item);
	characterSpriteMap.insert(std::pair<itemTypes, std::string>(man3Item->itemSelectedData.type, "Man3_200_Tran.png"));
#pragma endregion

#pragma region Create Buttons
	// play button
	auto playNormalSprite = Sprite::createWithSpriteFrameName("Button_Purple_20_Alpha.png");
	auto playSelectedSprite = Sprite::createWithSpriteFrameName("Button_Red_50_Alpha_Selected.png");
	auto playDisabledSprite = Sprite::createWithSpriteFrameName("Button_Red_50_Alpha_Disabled.png");
	auto playItem = MouseOverMenuItem::create(playNormalSprite, playSelectedSprite, playDisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::playButtonSelectedCallback, this));
	playItem->itemSelectedData.type = itemTypes::BUTTON;
	playItem->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 playPos = Vec2(sceneMidPoint.x - 80.f, sceneMidPoint.y - 160.f);
	playItem->setScale(0.8f);
	playItem->setPosition(playPos);
	playItem->setItemRect(playPos, 0.8f);

	auto buttonMidPoint = Vec2(playItem->getContentSize().width * 0.5f, playItem->getContentSize().height * 0.5f);

	// play label
	auto playText = Label::createWithTTF("PLAY", "fonts/Nirmala.ttf", 20);
	playText->setTextColor(Color4B::WHITE);
	playText->setPosition(buttonMidPoint);
	playItem->addChild(playText, 1);

	m_MenuItems.pushBack(playItem);

	// cancel button
	auto cancelNormalSprite = Sprite::createWithSpriteFrameName("Button_Purple_20_Alpha.png");
	auto cancelSelectedSprite = Sprite::createWithSpriteFrameName("Button_Red_50_Alpha_Selected.png");
	auto cancelDisabledSprite = Sprite::createWithSpriteFrameName("Button_Red_50_Alpha_Disabled.png");
	auto cancelItem = MouseOverMenuItem::create(cancelNormalSprite, cancelSelectedSprite, cancelDisabledSprite, CC_CALLBACK_1(MainMenuPlayerSetting::cancelButtonSelectedCallback, this));
	cancelItem->itemSelectedData.type = itemTypes::BUTTON;
	cancelItem->onMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 cancelPos = Vec2(sceneMidPoint.x + 80.f, sceneMidPoint.y - 160.f);
	cancelItem->setScale(0.8f);
	cancelItem->setPosition(cancelPos);
	cancelItem->setItemRect(cancelPos, 0.8f);

	// cancel label
	auto cancelText = Label::createWithTTF("CANCEL", "fonts/Nirmala.ttf", 20);
	cancelText->setTextColor(Color4B::WHITE);
	cancelText->setPosition(buttonMidPoint);
	cancelItem->addChild(cancelText, 1);

	m_MenuItems.pushBack(cancelItem);
#pragma endregion

	auto panelMenu = Menu::createWithArray(m_MenuItems);
	panelMenu->setPosition(Vec2::ZERO);

	m_MainMenu->addChild(m_PlayerSettingPanel, 2);
	m_MainMenu->addChild(panelMenu, 3);

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(MainMenuPlayerSetting::onKeyPressed, this);
	auto event = m_MainMenu->getEventDispatcher(); 
	event->addEventListenerWithSceneGraphPriority(listener, m_MainMenu);

}

void MainMenuPlayerSetting::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER)
		m_TextField->didNotSelectSelf();
}

void MainMenuPlayerSetting::characterSelectedCallback(Ref* pSender)
{
	m_TextField->didNotSelectSelf();
	m_HasSelected = true;

	for (auto item : m_MenuItems)
	{
		if (item != pSender)
		{
			if (item->itemSelectedData.type == itemTypes::BUTTON)
				continue;

			item->setEnabled(false);
			continue;
		}

		if (item->itemSelectedData.isSelected)
		{
			item->itemSelectedData.isSelected = false;
			item->itemSelectedData.selectedLabel->setVisible(false);
			item->unselected();
			for (auto item : m_MenuItems)
			{
				item->setEnabled(true);
			}
			m_HasSelected = false;
			return;
		}
		
		item->itemSelectedData.isSelected = true;

		if (item->itemSelectedData.selectedLabel)
		{
			item->itemSelectedData.selectedLabel->setVisible(true);
			continue;
		}
		
		// create label for first time
		item->itemSelectedData.selectedLabel = Label::createWithTTF("SELECTED", "fonts/NirmalaB.ttf", 40.f);
		item->itemSelectedData.selectedLabel->setTextColor(colorType.HotPink);
		item->itemSelectedData.selectedLabel->enableShadow(Color4B::BLACK);
		item->itemSelectedData.selectedLabel->enableGlow(colorType.LightSteelBlue);
		item->itemSelectedData.selectedLabel->setPosition(item->getContentSize().width * 0.5f, item->getContentSize().height * 0.5f - 60.f);
		item->itemSelectedData.selectedLabel->setRotation(-15.f);
		item->addChild(item->itemSelectedData.selectedLabel, 1);
		item->selected();
	}
}

void MainMenuPlayerSetting::playButtonSelectedCallback(Ref* pSender)
{
	if (!validation())
	{
		showInvalid();
		return;
	}
	if (!m_MainMenu)
		return;

	m_MainMenu->StopAudio();
	auto scene = GameScene::createScene();
	if (scene)
		Director::getInstance()->replaceScene(TransitionFade::create(2.f, scene));
}

void MainMenuPlayerSetting::cancelButtonSelectedCallback(Ref* pSender)
{
	m_MainMenu->isOpeningSubWindow = false;
	m_MainMenu->setMenuItemVisible(true);
	closeSettingWindow();
}

void MainMenuPlayerSetting::onMouseOver(MouseOverMenuItem* overItem, Event* event)
{
}

bool MainMenuPlayerSetting::validation()
{
	if (!m_TextField || m_TextField->getString() == "")
		return false;

	playerName = m_TextField->getString();

	if (m_MenuItems.size() < 0)
		return false;

	for (auto item : m_MenuItems)
	{
		if (item->itemSelectedData.isSelected)
		{
			playerCharacter = item->itemSelectedData.type;
			return true;
		}
	}

	auto randNo = random(0, 5);
	playerCharacter = m_MenuItems.at(randNo)->itemSelectedData.type;
	return true;
}

void MainMenuPlayerSetting::showInvalid()
{
	m_TextField->setPlaceHolderColor(colorType.Crimson);
	m_TextField->setPlaceHolder("?");
}




