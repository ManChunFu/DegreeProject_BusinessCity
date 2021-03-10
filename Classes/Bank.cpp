#include "Bank.h"
#include "GameScene.h"

USING_NS_CC;

Bank::~Bank()
{
}

void Bank::openBankPanel(GameScene* scene)
{
	if (!m_BankPanel)
	{
		m_GameScene = scene;
		if (m_GameScene)
			createBankPanel();

		return;
	}
	
	m_BankPanel->setVisible(true);
}

void Bank::closeBankPanel()
{
	m_BankPanel->setVisible(false);
}

void Bank::createBankPanel()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sceneMidPoint = Point(origin.x + (visibleSize.width * 0.5f), origin.y + (visibleSize.height * 0.5f));

	// background
	m_BankPanel = Sprite::createWithSpriteFrameName("UIPanelRecBlack80.png");
	m_BankPanel->setPosition(sceneMidPoint);

	m_GameScene->addChild(m_BankPanel, 2);
}
