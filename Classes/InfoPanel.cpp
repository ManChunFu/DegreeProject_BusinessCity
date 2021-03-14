#include "InfoPanel.h"
#include "Player.h"
#include "MouseOverMenuItem.h"

USING_NS_CC;


void InfoPanel::createPanel(cocos2d::Scene* scene, Player* player)
{
//	m_InfoPanel = Sprite::createWithSpriteFrameName("InGamePanel_Black_80.png");
//	if (!m_InfoPanel)
//		return;
//
//	GameFunctions::displaySprite(m_InfoPanel, Vec2(m_VisibleSize.width * 0.5f, 680.f), scene, 1);
//	auto topPanelMidPoint = Vec2(m_InfoPanel->getContentSize().width * 0.5f, m_InfoPanel->getContentSize().height * 0.5f);
//
//	auto playerSprite = Sprite::createWithSpriteFrameName(GameData::getInstance().getPlayerCharacter(player->getCharacter()));
//	if (!playerSprite)
//		return;
//
//	GameFunctions::displaySprite(playerSprite, Vec2(topPanelMidPoint.x - 250.f, topPanelMidPoint.y - 5.f), m_InfoPanel, 1, 0.4f, 0.4f);
//
//	auto nameLabel = Label::createWithTTF(player->getName(), "fonts/NirmalaB.ttf", 22);
//	if (nameLabel)
//	{
//		nameLabel->setMaxLineWidth(125);
//		nameLabel->enableWrap(true);
//		nameLabel->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
//		GameFunctions::displayLabel(nameLabel, Color4B::WHITE, Vec2(playerSprite->getPositionX() + 100.f, playerSprite->getPositionY()),
//			m_InfoPanel, 1);
//	}
//
//#pragma region CreateSavingLabels and Bank 
//	auto cashSymbol = Label::createWithTTF("$", "fonts/NirmalaB.ttf", 20);
//	if (cashSymbol)
//	{
//		cashSymbol->enableGlow(Color4B::WHITE);
//		GameFunctions::displayLabel(cashSymbol, GameData::getInstance().m_ColorType.Goldenrdo, Vec2(topPanelMidPoint.x + 350.f, topPanelMidPoint.y - 30.f),
//			m_InfoPanel, 1);
//	}
//
//	m_Saving = Label::createWithTTF("", "fonts/NirmalaB.ttf", 25);
//	if (m_Saving)
//	{
//		GameFunctions::updateLabelText_MoneyFormat(m_Saving, m_CurrentCash);
//		m_Saving->enableGlow(Color4B::WHITE);
//		GameFunctions::displayLabel(m_Saving, GameData::getInstance().m_ColorType.Gold,
//			Vec2(topPanelMidPoint.x + 420.f, topPanelMidPoint.y - 30.f), m_TopPanel, 1);
//	}
//
//	auto creditCard = Sprite::createWithSpriteFrameName("CreditCardBank_100.png");
//	if (creditCard)
//	{
//		creditCard->setPosition(topPanelMidPoint.x + 370.f, topPanelMidPoint.y + 10.f);
//		creditCard->setScale(0.6f);
//		m_InfoPanel->addChild(creditCard, 1);
//	}
//
//	// create bank buttons
//	auto bankButton = MouseOverMenuItem::creatMouseOverMenuButton("ButtonBlueNormal.png", "ButtonBlueLit.png", "ButtonBlueDisabled.png",
//		CC_CALLBACK_1(GameScene::checkBalanceCallback, this));
//	if (bankButton)
//	{
//		bankButton->onMouseOver = CC_CALLBACK_2(GameScene::onMouseOver, this);
//		auto bankPos = Vec2(m_InfoPanel->getPosition().x + 445.f, m_InfoPanel->getPosition().y);
//		bankButton->setPosition(bankPos);
//		bankButton->setScale(0.7f);
//		bankButton->setItemRect(bankPos, 0.7f);
//
//		auto myBankLabel = Label::createWithTTF("CHIBANK", "fonts/NirmalaB.ttf", 20);
//		if (myBankLabel)
//			GameFunctions::displayLabel(myBankLabel, GameData::getInstance().m_ColorType.Taro,
//				Vec2(bankButton->getContentSize().width * 0.5f, bankButton->getContentSize().height * 0.5f), bankButton, 1);
//
//		m_MenuItems.pushBack(bankButton);
//
//		m_Bank = new Bank();
//	}
//#pragma endregion
//
//#pragma region CreateTimeLabels
//	auto weekLabel = Label::createWithTTF("WEEK", "fonts/NirmalaB.ttf", 14);
//	if (weekLabel)
//	{
//		weekLabel->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
//		GameFunctions::displayLabel(weekLabel, Color4B::WHITE, Vec2(topPanelMidPoint.x + 540.f, topPanelMidPoint.y + 15.f),
//			m_InfoPanel, 1);
//	}
//
//	m_WeekCount = Label::createWithTTF("", "fonts/NirmalaB.ttf", 16);
//	if (m_WeekCount)
//	{
//		GameFunctions::updatLabelText_TimeFormat(m_WeekCount, m_Weeks);
//		m_WeekCount->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
//		GameFunctions::displayLabel(m_WeekCount, Color4B::WHITE, Vec2(topPanelMidPoint.x + 580.f, topPanelMidPoint.y + 15.f),
//			m_InfoPanel, 1);
//	}
//
//	m_WeekDay = Label::createWithTTF(m_WeekDays[m_Today], "fonts/NirmalaB.ttf", 14);
//	if (m_WeekDay)
//	{
//		m_WeekDay->enableGlow(GameData::getInstance().m_ColorType.DeepPink);
//		GameFunctions::displayLabel(m_WeekDay, Color4B::WHITE, Vec2(topPanelMidPoint.x + 555.f, topPanelMidPoint.y - 5.f),
//			m_InfoPanel, 1);
//	}
//
//	m_TimeHourDisplay = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
//	if (m_TimeHourDisplay)
//	{
//		GameFunctions::updatLabelText_TimeFormat(m_TimeHourDisplay, m_CurrentHour);
//		m_TimeHourDisplay->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
//		GameFunctions::displayLabel(m_TimeHourDisplay, Color4B::WHITE, Vec2(topPanelMidPoint.x + 530.f, topPanelMidPoint.y - 30.f),
//			m_InfoPanel, 1);
//	}
//
//	auto timeMark = Label::createWithTTF(":", "fonts/NirmalaB.ttf", 30);
//	if (timeMark)
//	{
//		timeMark->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
//		GameFunctions::displayLabel(timeMark, Color4B::WHITE, Vec2(topPanelMidPoint.x + 555.f, topPanelMidPoint.y - 28.f),
//			m_InfoPanel, 1);
//	}
//
//	m_TimeMinDisplay = Label::createWithTTF("", "fonts/NirmalaB.ttf", 30);
//	if (m_TimeHourDisplay)
//	{
//		GameFunctions::updatLabelText_TimeFormat(m_TimeMinDisplay, m_CurrentMinute);
//		m_TimeMinDisplay->enableGlow(GameData::getInstance().m_ColorType.PowderBlue);
//		GameFunctions::displayLabel(m_TimeMinDisplay, Color4B::WHITE, Vec2(topPanelMidPoint.x + 580.f, topPanelMidPoint.y - 30.f),
//			m_InfoPanel, 1);
//	}
//#pragma endregion

}
