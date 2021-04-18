#include "ShopEmployeeAdmin.h"
#include "ui/UIWidget.h"
#include "Shop.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "MouseOverMenuItem.h"

USING_NS_CC;

ShopEmployeeAdmin::~ShopEmployeeAdmin()
{
	m_Shop = nullptr;
	m_CurrentEmployeeCountText = nullptr;
	m_EmployeeCountText = nullptr;
	m_EmployeeCountText = nullptr;
	m_EmployeeSalaryText = nullptr;
}

void ShopEmployeeAdmin::createShopEmployeeAdmin(Shop* shop, ui::Widget* employeeWidget, Vec2 panelMidPoint)
{
	m_Shop = shop;
	employeeWidget->setVisible(false);
	auto darkCyanColor = GameData::getInstance().m_ColorType.DarkCyan;

	auto employeeText = Label::createWithTTF("Employee", "fonts/NirmalaB.ttf", 16);
	if (employeeText)
	{
		employeeText->enableGlow(Color4B::BLACK);
		GameFunctions::displayLabel(employeeText, Color4B::WHITE, Vec2(panelMidPoint.x - 220.f, panelMidPoint.y), employeeWidget, 1,
			true, TextHAlignment::LEFT);
	}

	m_CurrentEmployeeCountText = Label::createWithTTF(std::to_string(m_Shop->getEmployeeCount()), "fonts/Nirmala.ttf", 16);
	if (m_CurrentEmployeeCountText)
	{
		m_CurrentEmployeeCountText->enableShadow(darkCyanColor);
		GameFunctions::displayLabel(m_CurrentEmployeeCountText, Color4B::WHITE, Vec2(panelMidPoint.x - 130.f, panelMidPoint.y), employeeWidget, 1,
			true, TextHAlignment::LEFT);
	}

	
}

void ShopEmployeeAdmin::reduceCallback(cocos2d::Ref* pSender)
{
	m_EmployeeCount = GameFunctions::displayLabelText_ClampValue(m_EmployeeCountText, m_EmployeeCount, -1, 0, 5);
	m_EmployeeCountText->setString(std::to_string(m_EmployeeCount));
}

void ShopEmployeeAdmin::addCallback(cocos2d::Ref* pSender)
{
	m_EmployeeCount = GameFunctions::displayLabelText_ClampValue(m_EmployeeCountText, m_EmployeeCount, 1, 0, 5);
	m_EmployeeCountText->setString(std::to_string(m_EmployeeCount));
}

void ShopEmployeeAdmin::hireCallback(cocos2d::Ref* pSender)
{
	//update shop data
	m_Shop->addEmployee(m_EmployeeCount);
	m_EmployeeCountText->setString(std::to_string(m_EmployeeCount = 0));
}

void ShopEmployeeAdmin::workHereCallback(cocos2d::Ref* pSender)
{
	m_Shop->setPlayerWorkHere();
}

void ShopEmployeeAdmin::onMouseOver(MouseOverMenuItem* menuItem, Event* event)
{
}
