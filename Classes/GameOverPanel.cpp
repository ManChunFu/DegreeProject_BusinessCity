#include "GameOverPanel.h"
#include "MouseOverMenuItem.h"
#include "GameScene.h"
#include "GameFunctions.h"
#include "GameData.h"
#include "MainMenuScene.h"

USING_NS_CC;

GameOverPanel::~GameOverPanel()
{
	m_GameOverItems.clear();
	m_Elements.clear();
	m_GameoverPanel = nullptr;
	m_GameScene = nullptr;
}

void GameOverPanel::init(GameScene* scene, cocos2d::Vec2 sceneMidPoint)
{
	m_GameScene = scene;

	m_GameoverPanel = Sprite::createWithSpriteFrameName("UIPanelRecBlack80.png");
	if (!m_GameoverPanel)
		return;

	m_GameoverPanel->setScale(0.8f);
	m_GameoverPanel->setPosition(sceneMidPoint);
	m_GameScene->addChild(m_GameoverPanel, 3);
	m_Elements.push_back(m_GameoverPanel);

	auto panelMidPoint = Vec2(m_GameoverPanel->getContentSize().width * 0.5f, m_GameoverPanel->getContentSize().height * 0.5f);
	auto gameOverText = Label::createWithTTF("GAME OVER", "fonts/BROADW.TTF", 40);
	if (gameOverText)
	{
		gameOverText->enableOutline(Color4B::WHITE);
		GameFunctions::displayLabel(gameOverText, GameData::getInstance().m_ColorType.Crimson, Vec2(panelMidPoint.x,
			panelMidPoint.y + 200.f), m_GameoverPanel, 1);
	}

	auto textToPlayer = Label::createWithTTF("Unfortunately, you went bankrupt.", "fonts/NirmalaB.ttf", 20);
	if (textToPlayer)
	{
		textToPlayer->enableOutline(GameData::getInstance().m_ColorType.PowderBlue);
		GameFunctions::displayLabel(textToPlayer, Color4B::WHITE, Vec2(panelMidPoint.x, panelMidPoint.y + 100.f), m_GameoverPanel, 1);
	}

	for (unsigned index = 0; index < 2; index++)
	{
		auto buttonItem = MouseOverMenuItem::creatMouseOverMenuButton("Button_Purple_20_Alpha.png", "Button_Red_50_Alpha_Selected.png", "Button_Red_50_Alpha_Disabled.png",
			(index % 2 == 0) ? CC_CALLBACK_1(GameOverPanel::restart, this) : CC_CALLBACK_1(GameOverPanel::backToMenu, this));

		if (!buttonItem)
			return;

		buttonItem->onMouseOver = CC_CALLBACK_2(GameOverPanel::onMouseOver, this);
		Vec2 buttonPos = Vec2(sceneMidPoint.x, sceneMidPoint.y - (index % 2 == 0 ? 50.f : 150.f));
		buttonItem->setScale(1.2f);
		buttonItem->setPosition(buttonPos);
		buttonItem->setItemRect(buttonPos, 1.2f);

		auto buttonLabel = Label::createWithTTF((index % 2 == 0)? "PLAY AGAIN" : "BACK TO MENU", "fonts/NirmalaB.ttf", 12);
		if (buttonLabel)
			GameFunctions::displayLabel(buttonLabel, Color4B::WHITE, Vec2(buttonItem->getContentSize().width * 0.5f,
				buttonItem->getContentSize().height * 0.5f), buttonItem, 1);

		m_GameOverItems.pushBack(buttonItem);
	}

	auto menu = Menu::createWithArray(m_GameOverItems);
	menu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(menu, 4);
	m_Elements.push_back(menu);
}

void GameOverPanel::restart(cocos2d::Ref* pSender)
{
	destroyPanel();
	auto scene = GameScene::createScene();
	DelayTime::create(2.f);
	Director::getInstance()->replaceScene(scene);

}

void GameOverPanel::backToMenu(cocos2d::Ref* pSender)
{
	destroyPanel();

	auto scene = MainMenuScene::createScene();
	if (scene)
		Director::getInstance()->replaceScene(scene);
}

void GameOverPanel::onMouseOver(MouseOverMenuItem* item, cocos2d::Event* event)
{
}

void GameOverPanel::destroyPanel()
{
	for (auto element : m_Elements)
	{
		m_GameScene->removeChild(element);
	}
	if (onDestroyCall)
		onDestroyCall;
}











































