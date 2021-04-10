#include "ShopAdmin.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "Shop.h"
#include "MouseOverMenuItem.h"


USING_NS_CC;

ShopAdmin::~ShopAdmin()
{
	m_AdminTabs.clear();
	m_Weekdays.clear();
	m_MyShop = nullptr;
	m_FromHourText = nullptr;
	m_ToHourText = nullptr;
}

void ShopAdmin::createAdmin(Shop* shop, ui::Widget* adminWidget, Vec2 panelMidPoint)
{
	// only overview widget is visible when the panel is open
	adminWidget->setVisible(false);
	m_MyShop = shop;

	auto widgetBackgroundSprite = Sprite::createWithSpriteFrameName("AdminBG_Blue.png");
	if (widgetBackgroundSprite)
		GameFunctions::displaySprite(widgetBackgroundSprite, Vec2(panelMidPoint.x, panelMidPoint.y - 70.f), adminWidget, 1);

#pragma region Create AdminTabs and Widgets
	auto tabPos = (Vec2(panelMidPoint.x - 205.f, panelMidPoint.y + 60.f));
	std::array<std::string, 3> tabTexts = { "SCHEDULE", "PRODUCT", "EMPLOYEE" };
	for (unsigned index = 0; index < 3; index++)
	{
		auto tabButton = ui::CheckBox::create("Sprites/Checkboxes/Button_Grey.png", "Sprites/Checkboxes/Button_BlueSmall.png");
		if (tabButton)
		{
			tabButton->setPosition(tabPos);
			tabButton->addClickEventListener(CC_CALLBACK_1(ShopAdmin::onOpenTabCallback, this, index));
			adminWidget->addChild(tabButton, 1);

			auto tabText = Label::createWithTTF(tabTexts[index], "fonts/NirmalaB.ttf", 10);
			if (tabText)
				GameFunctions::displayLabel(tabText, Color4B::WHITE, tabButton->getContentSize() * 0.5f, tabButton, 1);

			auto tabWidget = ui::Widget::create();
			tabWidget->setPosition(Vec2::ZERO);
			adminWidget->addChild(tabWidget, 1);

			m_AdminTabs.push_back(std::make_pair(tabButton, tabWidget));

			tabPos.x += 85.f;
		}
	}
	m_AdminTabs.at(0).first->setSelected(true);
#pragma endregion

#pragma region Work Schedule
	// weeks
	auto checkboxPos = Vec2(panelMidPoint.x - 140.f, panelMidPoint.y - 10.f);
	std::array<std::string, 7> weekdays = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
	for (unsigned index = 0; index < 7; index++)
	{
		auto checkbox = ui::CheckBox::create("Sprites/Checkboxes/Checkbox_White_Normal.png", "Sprites/Checkboxes/Checkbox_Checked_White.png");
		checkbox->setPosition(checkboxPos);
		checkbox->setSelected(m_MyShop->m_ShopOpenDay[index]);
		checkbox->addClickEventListener(CC_CALLBACK_1(ShopAdmin::checkBoxClickCallback, this, index));

		m_Weekdays.push_back(checkbox);
		m_AdminTabs.at(0).second->addChild(checkbox, 1);
		checkboxPos.x += 50.f;

		auto text = Label::createWithTTF(weekdays[index], "fonts/NirmalaB.ttf", 16);
		if (text)
		{
			text->enableGlow(Color4B::BLACK);
			GameFunctions::displayLabel(text, Color4B::WHITE, Vec2(checkbox->getContentSize().width * 0.5f,
				checkbox->getContentSize().height + 10.f), checkbox, 1);
		}
	}


#pragma region work hours
	// from
	auto fromText = Label::createWithTTF("From", "fonts/Nirmala.ttf", 18);
	if (fromText)
	{
		fromText->enableItalics();
		GameFunctions::displayLabel(fromText, Color4B::BLACK, Vec2(panelMidPoint.x - 100.f, panelMidPoint.y + 40.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	}

	auto fromBoxSprite = Sprite::createWithSpriteFrameName("Border_Black.png");
	auto fromBoxMidPoint = Vec2(fromBoxSprite->getContentSize().width * 0.5f, fromBoxSprite->getContentSize().height * 0.5f);
	if (fromBoxSprite)
	{
		GameFunctions::displaySprite(fromBoxSprite, Vec2(panelMidPoint.x, panelMidPoint.y + 50.f), m_ThisPanel, 1);

		auto minuteText = Label::createWithTTF(": 00", "fonts/Nirmala.ttf", 20);
		if (minuteText)
		{
			minuteText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(minuteText, Color4B::WHITE, Vec2(fromBoxMidPoint.x + 30.f, fromBoxMidPoint.y + 15.f),
				fromBoxSprite, 1, true, TextHAlignment::RIGHT);
		}

		m_FromHourText = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_FromHourText)
		{
			GameFunctions::updatLabelText_TimeFormat(m_FromHourText, m_MyShop->m_ShopOpenHour.first, true);
			m_FromHourText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(m_FromHourText, Color4B::WHITE, Vec2(fromBoxMidPoint.x - 10.f, fromBoxMidPoint.y + 15.f),
				fromBoxSprite, 1, true, TextHAlignment::RIGHT);
		}
	}

	for (unsigned index = 0; index < 2; index++)
	{
		auto fromButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(ShopAdmin::reduceTimeCallback, this, true))
			: MouseOverMenuItem::createUpperButton(CC_CALLBACK_1(ShopAdmin::increaseTimeCallback, this, true));
		if (fromButton)
		{
			//displayButtons(fromButton, (index % 2 == 0) ? Vec2(sceneMidPoint.x + 33.f, sceneMidPoint.y + 40.f) :
			//	Vec2(sceneMidPoint.x - 33.f, sceneMidPoint.y + 60.f), itemTypes::BUTTON);
		}
	}

	// to
	auto toText = Label::createWithTTF("To", "fonts/Nirmala.ttf", 18);
	if (toText)
	{
		toText->enableItalics();
		GameFunctions::displayLabel(toText, Color4B::BLACK, Vec2(panelMidPoint.x + 70.f, panelMidPoint.y + 40.f),
			m_ThisPanel, 1, true, TextHAlignment::LEFT);
	}

	auto toBoxSprite = Sprite::createWithSpriteFrameName("Border_Black.png");
	if (toBoxSprite)
	{
		GameFunctions::displaySprite(toBoxSprite, Vec2(panelMidPoint.x + 150.f, panelMidPoint.y + 50.f), m_ThisPanel, 1);

		auto toMinuteText = Label::createWithTTF(": 00", "fonts/Nirmala.ttf", 20);
		if (toMinuteText)
		{
			toMinuteText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(toMinuteText, Color4B::WHITE, Vec2(fromBoxMidPoint.x + 30.f, fromBoxMidPoint.y + 15.f),
				toBoxSprite, 1, true, TextHAlignment::RIGHT);
		}

		m_ToHourText = Label::createWithTTF("", "fonts/Nirmala.ttf", 20);
		if (m_ToHourText)
		{
			GameFunctions::updatLabelText_TimeFormat(m_ToHourText, m_MyShop->m_ShopOpenHour.second, true);
			m_ToHourText->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(m_ToHourText, Color4B::WHITE, Vec2(fromBoxMidPoint.x - 10.f, fromBoxMidPoint.y + 15.f),
				toBoxSprite, 1, true, TextHAlignment::RIGHT);
		}
	}

	for (unsigned index = 0; index < 2; index++)
	{
		auto toButton = (index % 2 == 0) ? MouseOverMenuItem::createLowerButton(CC_CALLBACK_1(ShopAdmin::reduceTimeCallback, this, false))
			: MouseOverMenuItem::createUpperButton(CC_CALLBACK_1(ShopAdmin::increaseTimeCallback, this, false));
		if (toButton)
		{
			//displayButtons(toButton, (index % 2 == 0) ? Vec2(sceneMidPoint.x + 183.f, sceneMidPoint.y + 40.f) :
			//	Vec2(sceneMidPoint.x + 117.f, sceneMidPoint.y + 60.f), itemTypes::BUTTON);
		}
	}

#pragma endregion work hours


#pragma endregion

}

void ShopAdmin::update()
{
}

void ShopAdmin::onOpenTabCallback(cocos2d::Ref* pSender, unsigned tabIndex)
{
	for (unsigned index = 0; index < m_AdminTabs.size(); index++)
	{
		if (index == tabIndex)
		{
			(m_AdminTabs.at(index).second->isVisible()) ? m_AdminTabs.at(index).second->setVisible(false) :
				m_AdminTabs.at(index).second->setVisible(true);

			continue;
		}
		m_AdminTabs.at(index).second->setVisible(false);
		m_AdminTabs.at(index).first->setSelected(false);
	}
}

void ShopAdmin::checkBoxClickCallback(cocos2d::Ref* pSender, unsigned weekday)
{
	m_MyShop->setShopOpenDay(weekday);
	if (onWorkDayChanges)
		onWorkDayChanges(weekday);
}

void ShopAdmin::reduceTimeCallback(cocos2d::Ref* pSender, bool fromHourButton)
{
	if (fromHourButton)
	{
		m_FromHour = GameFunctions::displayLabelText_ClampValue(m_FromHourText, m_FromHour, -1, 0, 24);
		GameFunctions::updatLabelText_TimeFormat(m_FromHourText, m_FromHour, true);

		m_MyShop->setShopOpenHour(0, m_FromHour);

		if (onWorkHourChanges)
			onWorkHourChanges(m_FromHour);
		return;
	}
	m_ToHour = GameFunctions::displayLabelText_ClampValue(m_ToHourText, m_ToHour, -1, 0, 24);
	GameFunctions::updatLabelText_TimeFormat(m_ToHourText, m_ToHour, true);
	m_MyShop->setShopOpenHour(1, m_ToHour);

	if (onWorkHourChanges)
		onWorkHourChanges(m_ToHour);

}

void ShopAdmin::increaseTimeCallback(cocos2d::Ref* pSender, bool fromHourButton)
{
	if (fromHourButton)
	{
		m_FromHour = GameFunctions::displayLabelText_ClampValue(m_FromHourText, m_FromHour, 1, 0, 24);
		GameFunctions::updatLabelText_TimeFormat(m_FromHourText, m_FromHour, true);

		m_MyShop->setShopOpenHour(0, m_FromHour);

		if (onWorkHourChanges)
			onWorkHourChanges(m_FromHour);

		return;
	}
	m_ToHour = GameFunctions::displayLabelText_ClampValue(m_ToHourText, m_ToHour, 1, 0, 24);
	GameFunctions::updatLabelText_TimeFormat(m_ToHourText, m_ToHour, true);
	m_MyShop->setShopOpenHour(1, m_ToHour);

	if (onWorkHourChanges)
		onWorkHourChanges(m_ToHour);
}


