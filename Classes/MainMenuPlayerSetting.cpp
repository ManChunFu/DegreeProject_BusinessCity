#include "MainMenuPlayerSetting.h"
#include "MainMenuScene.h"
#include "MouseOverMenuItem.h"
#include <ui/UITextField.h>
#include "GameData.h"
#include "GameFunctions.h"
#include "GameScene.h"


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
	if (m_PlayerSettingPanel)
	{
		GameFunctions::displaySprite(m_PlayerSettingPanel, sceneMidPoint, m_MainMenu, 2);
		m_PlayerSettingPanel->setScale(0.8f);
	}

	auto panelMidPoint = Vec2(m_PlayerSettingPanel->getContentSize().width * 0.5f, m_PlayerSettingPanel->getContentSize().height * 0.5f);

	// player name
	m_TextField = ui::TextField::create("ENTER YOUR NAME", "fonts/Nirmala.ttf", 30);
	if (m_TextField)
	{
		GameFunctions::displayTextField(m_TextField, Color3B::WHITE, Vec2(panelMidPoint.x, panelMidPoint.y + 240.f),
			m_PlayerSettingPanel, 1, false, 24);
		m_TextField->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) { m_TextField->setCursorEnabled(true); });
	}

	// option label
	auto optionLabel = Label::createWithTTF("CHOOSE YOUR PLAYER", "fonts/Nirmala.ttf", 20);
	GameFunctions::displayLabel(optionLabel, GameData::getInstance().m_ColorType.HotPink, Vec2(panelMidPoint.x, panelMidPoint.y + 160.f),
		m_PlayerSettingPanel, 1);

#pragma region Create Character MenuItem
	// woman1
	auto woman1Item = MouseOverMenuItem::creatMouseOverMenuButton("Woman1_200_Tran.png", "Woman1_200_Tran_Lit.png", "Woman1_200_Tran_Disabled.png",
		CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	if (!woman1Item)
		return;

	woman1Item->itemSelectedData.type = itemTypes::WOMAN1;
	woman1Item->m_OnMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 woman1Pos = Vec2(sceneMidPoint.x - 100.f, sceneMidPoint.y + 60.f);
	woman1Item->setScale(0.4f);
	woman1Item->setPosition(woman1Pos);
	woman1Item->setItemRect(woman1Pos, 0.4f);

	m_MenuItems.pushBack(woman1Item);
	GameData::getInstance().registerCharacter(woman1Item->itemSelectedData.type, "Woman1_200_Tran.png");

	// woman2
	auto woman2Item = MouseOverMenuItem::creatMouseOverMenuButton("Woman2_200_Tran.png", "Woman2_200_Tran_Lit.png", "Woman2_200_Tran_Disabled.png",
		CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	if (!woman2Item)
		return;

	woman2Item->itemSelectedData.type = itemTypes::WOMAN2;
	woman2Item->m_OnMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 woman2Pos = Vec2(sceneMidPoint.x, sceneMidPoint.y + 60.f);
	woman2Item->setScale(0.4f);
	woman2Item->setPosition(woman2Pos);
	woman2Item->setItemRect(woman2Pos, 0.4f);

	m_MenuItems.pushBack(woman2Item);
	GameData::getInstance().registerCharacter(woman2Item->itemSelectedData.type, "Woman2_200_Tran.png");

	// woman3
	auto woman3Item = MouseOverMenuItem::creatMouseOverMenuButton("Woman3_200_Tran.png", "Woman3_200_Tran_Lit.png", "Woman3_200_Tran_Disabled.png",
		CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	if (!woman3Item)
		return;

	woman3Item->itemSelectedData.type = itemTypes::WOMAN3;
	woman3Item->m_OnMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 woman3Pos = Vec2(sceneMidPoint.x + 100.f, sceneMidPoint.y + 60.f);
	woman3Item->setScale(0.4f);
	woman3Item->setPosition(woman3Pos);
	woman3Item->setItemRect(woman3Pos, 0.4f);

	m_MenuItems.pushBack(woman3Item);
	GameData::getInstance().registerCharacter(woman3Item->itemSelectedData.type, "Woman3_200_Tran.png");

	// man1
	auto man1Item = MouseOverMenuItem::creatMouseOverMenuButton("Man1_200_Tran.png", "Man1_200_Tran_Lit.png", "Man1_200_Tran_Disabled.png",
		CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	man1Item->itemSelectedData.type = itemTypes::MAN1;
	if (!man1Item)
		return;

	man1Item->m_OnMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 man1Pos = Vec2(sceneMidPoint.x - 100.f, sceneMidPoint.y - 40.f);
	man1Item->setScale(0.4f);
	man1Item->setPosition(man1Pos);
	man1Item->setItemRect(man1Pos, 0.4f);

	m_MenuItems.pushBack(man1Item);
	GameData::getInstance().registerCharacter(man1Item->itemSelectedData.type, "Man1_200_Tran.png");

	// man2
	auto man2Item = MouseOverMenuItem::creatMouseOverMenuButton("Man2_200_Tran.png", "Man2_200_Tran_Lit.png", "Man2_200_Tran_Disabled.png",
		CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	if (!man2Item)
		return;

	man2Item->itemSelectedData.type = itemTypes::MAN2;
	man2Item->m_OnMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 man2Pos = Vec2(sceneMidPoint.x, sceneMidPoint.y - 40.f);
	man2Item->setScale(0.4f);
	man2Item->setPosition(man2Pos);
	man2Item->setItemRect(man2Pos, 0.4f);

	m_MenuItems.pushBack(man2Item);
	GameData::getInstance().registerCharacter(man2Item->itemSelectedData.type, "Man2_200_Tran.png");

	// man3
	auto man3Item = MouseOverMenuItem::creatMouseOverMenuButton("Man3_200_Tran.png", "Man3_200_Tran_Lit.png", "Man3_200_Tran_Disabled.png",
		CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));
	if (!man3Item)
		return;

	man3Item->itemSelectedData.type = itemTypes::MAN3;
	man3Item->m_OnMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
	Vec2 man3Pos = Vec2(sceneMidPoint.x + 100.f, sceneMidPoint.y - 40.f);
	man3Item->setScale(0.4f);
	man3Item->setPosition(man3Pos);
	man3Item->setItemRect(man3Pos, 0.4f);

	m_MenuItems.pushBack(man3Item);
	GameData::getInstance().registerCharacter(man3Item->itemSelectedData.type, "Man3_200_Tran.png");
#pragma endregion

#pragma region Create Buttons
	// play button
	auto playItem = MouseOverMenuItem::creatMouseOverMenuButton("Button_Purple_20_Alpha.png", "Button_Red_50_Alpha_Selected.png", "Button_Red_50_Alpha_Disabled.png",
		CC_CALLBACK_1(MainMenuPlayerSetting::playButtonSelectedCallback, this));

		auto buttonMidPoint = Vec2(playItem->getContentSize().width * 0.5f, playItem->getContentSize().height * 0.5f);
	if (playItem)
	{
		playItem->itemSelectedData.type = itemTypes::BUTTON;
		playItem->m_OnMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
		Vec2 playPos = Vec2(sceneMidPoint.x - 80.f, sceneMidPoint.y - 160.f);
		playItem->setScale(0.8f);
		playItem->setPosition(playPos);
		playItem->setItemRect(playPos, 0.8f);

		// play label
		auto playText = Label::createWithTTF("PLAY", "fonts/Nirmala.ttf", 20);
		if (playText)
			GameFunctions::displayLabel(playText, Color4B::WHITE, buttonMidPoint,playItem, 1);

		m_MenuItems.pushBack(playItem);
	}

	// cancel button
	auto cancelItem = MouseOverMenuItem::creatMouseOverMenuButton("Button_Purple_20_Alpha.png", "Button_Red_50_Alpha_Selected.png", "Button_Red_50_Alpha_Disabled.png",
		CC_CALLBACK_1(MainMenuPlayerSetting::cancelButtonSelectedCallback, this));
	if (cancelItem)
	{
		cancelItem->itemSelectedData.type = itemTypes::BUTTON;
		cancelItem->m_OnMouseOver = CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this);
		Vec2 cancelPos = Vec2(sceneMidPoint.x + 80.f, sceneMidPoint.y - 160.f);
		cancelItem->setScale(0.8f);
		cancelItem->setPosition(cancelPos);
		cancelItem->setItemRect(cancelPos, 0.8f);

		// cancel label
		auto cancelText = Label::createWithTTF("CANCEL", "fonts/Nirmala.ttf", 20);
		if (cancelText)
			GameFunctions::displayLabel(cancelText, Color4B::WHITE, buttonMidPoint, cancelItem, 1);

		m_MenuItems.pushBack(cancelItem);
	}
#pragma endregion

	auto panelMenu = Menu::createWithArray(m_MenuItems);
	panelMenu->setPosition(Vec2::ZERO);

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
		item->itemSelectedData.selectedLabel->enableShadow(Color4B::BLACK);
		item->itemSelectedData.selectedLabel->enableGlow(GameData::getInstance().m_ColorType.LightSteelBlue);
		item->itemSelectedData.selectedLabel->setRotation(-15.f);
		GameFunctions::displayLabel(item->itemSelectedData.selectedLabel, GameData::getInstance().m_ColorType.HotPink, Vec2(item->getContentSize().width * 0.5f,
			item->getContentSize().height * 0.5f - 60.f), item, 1), 
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

	if (m_MenuItems.size() < 0)
		return false;

	for (auto item : m_MenuItems)
	{
		if (item->itemSelectedData.isSelected)
		{
			GameData::getInstance().setPlayer(m_TextField->getString(), item->itemSelectedData.type);
			return true;
		}
	}

	auto randNo = random(0, 5);
	GameData::getInstance().setPlayer(m_TextField->getString(),m_MenuItems.at(randNo)->itemSelectedData.type);
	return true;
}

void MainMenuPlayerSetting::showInvalid()
{
	m_TextField->setPlaceHolderColor(GameData::getInstance().m_ColorType.Crimson);
	m_TextField->setPlaceHolder("?");
}




