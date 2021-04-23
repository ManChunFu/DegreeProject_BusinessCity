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
	std::array<std::string, 18> characterPaths = { "Woman1_200_Tran.png", "Woman1_200_Tran_Lit.png", "Woman1_200_Tran_Disabled.png",
		"Woman2_200_Tran.png", "Woman2_200_Tran_Lit.png", "Woman2_200_Tran_Disabled.png",
		"Woman3_200_Tran.png", "Woman3_200_Tran_Lit.png", "Woman3_200_Tran_Disabled.png",
		"Man1_200_Tran.png", "Man1_200_Tran_Lit.png", "Man1_200_Tran_Disabled.png",
		"Man2_200_Tran.png", "Man2_200_Tran_Lit.png", "Man2_200_Tran_Disabled.png",
		"Man3_200_Tran.png", "Man3_200_Tran_Lit.png", "Man3_200_Tran_Disabled.png" };
	
	std::array<itemTypes, 6> itemTypes = { itemTypes::WOMAN1, itemTypes::WOMAN2, itemTypes::WOMAN3, itemTypes::MAN1, itemTypes::MAN2,
		itemTypes::MAN3 };

	auto characterPos = Vec2(sceneMidPoint.x - 100.f, sceneMidPoint.y + 60.f);
	for (unsigned index = 0, picIndex = 0; index < 6; ++index, picIndex+=3)
	{
		auto item = MouseOverMenuItem::creatMouseOverMenuButton(characterPaths[picIndex], characterPaths[picIndex +1], characterPaths[picIndex +2],
			CC_CALLBACK_1(MainMenuPlayerSetting::characterSelectedCallback, this));

		if (item)
		{
			m_MenuItems.pushBack(displayMenuButton(item, CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this), characterPos,
				itemTypes[index], 0.4f, true, Vec2(5.0f, 0.f)));
			
			GameData::getInstance().registerCharacter(item->itemSelectedData.type, characterPaths[picIndex]);
		}
		characterPos.x += 100.f;
		if (index == 2)
		{
			characterPos.x = sceneMidPoint.x - 100.f;
			characterPos.y = sceneMidPoint.y - 40.f;
		}
	}

#pragma endregion

#pragma region Create Buttons
	std::array<std::string, 6> buttonSprites = { "Button_Purple_20_Alpha.png", "Button_Red_50_Alpha_Selected.png", "Button_Red_50_Alpha_Disabled.png",
		"Button_Purple_20_Alpha.png", "Button_Red_50_Alpha_Selected.png", "Button_Red_50_Alpha_Disabled.png" };
	
	std::array<std::string, 2> buttonTexts = { "PLAY", "CANCEL" };

	auto buttonPos = Vec2(sceneMidPoint.x - 80.f, sceneMidPoint.y - 160.f);
	
	for (unsigned index = 0, picIndex = 0; index < 2; ++index, picIndex += 3)
	{
		auto button = MouseOverMenuItem::creatMouseOverMenuButton(buttonSprites[picIndex], buttonSprites[picIndex +1], 
			buttonSprites[picIndex +2], (index == 0? CC_CALLBACK_1(MainMenuPlayerSetting::playButtonSelectedCallback, this) :
		CC_CALLBACK_1(MainMenuPlayerSetting::cancelButtonSelectedCallback, this)));

		auto buttonMidPoint = Vec2(button->getContentSize()* 0.5f);
		if (button)
		{
			m_MenuItems.pushBack(displayMenuButton(button, CC_CALLBACK_2(MainMenuPlayerSetting::onMouseOver, this), buttonPos,
				itemTypes::BUTTON, 0.8f, true, Vec2(5.f, 0.f)));

			auto text = Label::createWithTTF(buttonTexts[index], "fonts/Nirmala.ttf", 20);
			if (text)
				GameFunctions::displayLabel(text, Color4B::WHITE, buttonMidPoint, button, 1);
		}

		buttonPos.x += 160.f;
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




