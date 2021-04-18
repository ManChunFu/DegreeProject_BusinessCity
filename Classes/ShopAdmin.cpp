#include "ShopAdmin.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "Shop.h"
#include "MouseOverMenuItem.h"
#include "GlobalTime.h"
#include "GameTime.h"
#include "ShopProductAdmin.h"
#include "ShopEmployeeAdmin.h"


USING_NS_CC;

ShopAdmin::~ShopAdmin()
{
	m_AdminTabs.clear();
	m_Weekdays.clear();
	m_FromToTexts.clear();
	m_MyShop = nullptr;
	m_GameTime = nullptr;
}

void ShopAdmin::createAdmin(Shop* shop, ui::Widget* adminWidget, Vec2 panelMidPoint)
{
	m_GameTime = GameData::getInstance().m_GlobalTime->m_Gametime;

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

#pragma region Work Schedule Admin
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
		m_AdminTabs.at(EWidgetTabs::E_ScheduleWidget).second->addChild(checkbox, 1);
		checkboxPos.x += 55.f;

		auto text = Label::createWithTTF(weekdays[index], "fonts/NirmalaB.ttf", 16);
		if (text)
		{
			text->enableGlow(Color4B::BLACK);
			GameFunctions::displayLabel(text, Color4B::WHITE, Vec2(checkbox->getContentSize().width * 0.5f,
				checkbox->getContentSize().height + 10.f), checkbox, 1);
		}
	}

#pragma region work hours
	std::array<std::string, 2> fromToTexts = { "From", "To" };
	auto textPos = Vec2(panelMidPoint.x - 230.f, panelMidPoint.y - 70.f);
	auto boxPos = Vec2(panelMidPoint.x - 140.f, panelMidPoint.y - 60.f);
	auto darkCyanColor = GameData::getInstance().m_ColorType.DarkCyan;

	for (unsigned index = 0; index < 2; index++)
	{
		auto text = Label::createWithTTF(fromToTexts[index], "fonts/NirmalaB.ttf", 16);
		if (text)
		{
			text->enableGlow(Color4B::BLACK);
			GameFunctions::displayLabel(text, Color4B::WHITE, textPos, m_AdminTabs.at(EWidgetTabs::E_ScheduleWidget).second, 1, 
				true, TextHAlignment::LEFT);
		}

		for (unsigned weekdayIndex = 0; weekdayIndex < 7; weekdayIndex++)
		{
			auto boxSprite = Sprite::createWithSpriteFrameName("WhiteBox.png");
			auto boxSpriteMidPoint = Vec2(boxSprite->getContentSize() * 0.5f);

			if (boxSprite)
				GameFunctions::displaySprite(boxSprite, boxPos, m_AdminTabs.at(0).second, 1);

			auto hourText = Label::createWithTTF("", "fonts/Nirmala.ttf", 14);
			if (hourText)
			{
				if (index == 0)
				{
					GameFunctions::updatLabelText_TimeFormat(hourText, m_MyShop->m_ShopOpenHour[weekdayIndex].first, true);
					m_FromToTexts.push_back(std::make_pair(hourText, nullptr));
				}
				else
				{
					GameFunctions::updatLabelText_TimeFormat(hourText, m_MyShop->m_ShopOpenHour[weekdayIndex].second, true);
					m_FromToTexts[weekdayIndex].second = hourText;
				}

				hourText->enableShadow(darkCyanColor);
				GameFunctions::displayLabel(hourText, Color4B::WHITE, boxSpriteMidPoint, boxSprite, 1);
			}

			for (unsigned buttonIndex = 0; buttonIndex < 2; buttonIndex++)
			{
				auto button = (buttonIndex % 2 == 0) ? MouseOverMenuItem::createDecreaseButton(CC_CALLBACK_1(ShopAdmin::reduceTimeCallback,
					this, weekdayIndex, (index == 0) ? true : false)) : MouseOverMenuItem::createIncreaseButton(CC_CALLBACK_1(
						ShopAdmin::increaseTimeCallback, this, weekdayIndex, (index == 0) ? true : false));
				if (button)
				{
					m_MenuItems.pushBack(displayMenuButton(button, CC_CALLBACK_2(ShopAdmin::onMouseOver, this), ((buttonIndex % 2 == 0) ?
						Vec2(boxPos.x, boxPos.y - 20.f) : Vec2(boxPos.x, boxPos.y + 20.f)), itemTypes::BUTTON, 0.5f, true));
				}

				if (!m_Weekdays[weekdayIndex]->isSelected())
					button->setEnabled(false);
			}
			boxPos.x += 55.f;
		}

		textPos.y -= 70.f;
		boxPos.x = panelMidPoint.x - 140.f;
		boxPos.y -= 70.f;
	}

#pragma endregion work hours

#pragma endregion
	enableMenuItmes(false);
	auto menu = Menu::createWithArray(m_MenuItems);
	menu->setPosition(Vec2::ZERO);
	m_AdminTabs.at(EWidgetTabs::E_ScheduleWidget).second->addChild(menu, 1);

#pragma region create Product Admin 
	m_ShopProductAdmin = new ShopProductAdmin();
	m_ShopProductAdmin->createShopProductAdmin(m_MyShop, m_AdminTabs.at(EWidgetTabs::E_ProductWidget).second, panelMidPoint);
#pragma endregion

#pragma region create Employee Admin
	m_ShopEmployeeAdmin = new ShopEmployeeAdmin();
	m_ShopEmployeeAdmin->createShopEmployeeAdmin(m_MyShop, m_AdminTabs.at(EWidgetTabs::E_EmployeeWidget).second, panelMidPoint);
#pragma endregion

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
	enableHoursButtons(weekday, m_MyShop->m_ShopOpenDay[weekday]);
	if (onWorkDayChanges)
		onWorkDayChanges(weekday);
}

void ShopAdmin::reduceTimeCallback(cocos2d::Ref* pSender, unsigned weekday, bool fromHourButton)
{
	if (fromHourButton)
	{
		auto openHour = m_MyShop->m_ShopOpenHour[weekday].first;
		openHour = GameFunctions::displayLabelText_ClampValueTime(m_FromToTexts[weekday].first, openHour, -1, 0, 24);

		m_MyShop->setShopOpenHour(weekday, 0, openHour);

		if (onWorkHourChanges && m_GameTime->day == weekday)
			onWorkHourChanges(openHour);
		return;
	}

	auto openHour = m_MyShop->m_ShopOpenHour[weekday].second;
	openHour = GameFunctions::displayLabelText_ClampValueTime(m_FromToTexts[weekday].second, openHour, -1, 0, 24);
	m_MyShop->setShopOpenHour(weekday, 1, openHour);

	if (onWorkHourChanges && m_GameTime->day == weekday)
		onWorkHourChanges(openHour);

}

void ShopAdmin::increaseTimeCallback(cocos2d::Ref* pSender, unsigned weekday, bool fromHourButton)
{
	if (fromHourButton)
	{
		auto openHour = m_MyShop->m_ShopOpenHour[weekday].first;
		openHour = GameFunctions::displayLabelText_ClampValueTime(m_FromToTexts[weekday].first, openHour, 1, 0, 24);

		m_MyShop->setShopOpenHour(weekday, 0, openHour);

		if (onWorkHourChanges && m_GameTime->day == weekday)
			onWorkHourChanges(openHour);
		return;
	}

	auto openHour = m_MyShop->m_ShopOpenHour[weekday].second;
	openHour = GameFunctions::displayLabelText_ClampValueTime(m_FromToTexts[weekday].second, openHour, 1, 0, 24);
	m_MyShop->setShopOpenHour(weekday, 1, openHour);

	if (onWorkHourChanges && m_GameTime->day == weekday)
		onWorkHourChanges(openHour);
}

void ShopAdmin::onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
}

void ShopAdmin::enableHoursButtons(unsigned weekday, bool enable)
{
	auto index = weekday * 2;
	//from button
	m_MenuItems.at(index)->setEnabled(enable);
	m_MenuItems.at(index + m_FromHourIncreaseStartIndex)->setEnabled(enable);

	// to button
	m_MenuItems.at(index + m_ToHourDecreaseStartIndex)->setEnabled(enable);
	m_MenuItems.at(index + m_ToHourIncreaseStartIndex)->setEnabled(enable);
}




