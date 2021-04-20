#include "ShopEmployeeAdmin.h"
#include "ui/UIWidget.h"
#include "Shop.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "MouseOverMenuItem.h"
#include "EmployeeData.h"

USING_NS_CC;

ShopEmployeeAdmin::~ShopEmployeeAdmin()
{
	m_Shop = nullptr;
	m_EmployeeCountText = nullptr;
	m_EmployeeCountText = nullptr;
	m_TotalSalaryText = nullptr;
	for (auto candidate : m_CandidateDatas)
	{
		delete candidate;
	}
	m_CandidateDatas.clear();
	m_ProfileWidgets.clear();
	m_HiredEmployees.clear();
}

void ShopEmployeeAdmin::createShopEmployeeAdmin(Shop* shop, ui::Widget* employeeWidget, Vec2 panelMidPoint)
{
	m_Shop = shop;
	employeeWidget->setVisible(false);

	auto darkCyanColor = GameData::getInstance().m_ColorType.DarkCyan;
	auto textPos = Vec2(panelMidPoint.x - 220.f, panelMidPoint.y - 10.f);

	auto employeeText = Label::createWithTTF("Employees", "fonts/NirmalaB.ttf", 16);
	if (employeeText)
	{
		employeeText->enableGlow(Color4B::BLACK);
		GameFunctions::displayLabel(employeeText, Color4B::WHITE, textPos, employeeWidget, 1, true, TextHAlignment::LEFT);
	}

	m_EmployeeCountText = Label::createWithTTF(std::to_string(m_Shop->getEmployeeCount()), "fonts/Nirmala.ttf", 16);
	if (m_EmployeeCountText)
	{
		m_EmployeeCountText->enableShadow(darkCyanColor);
		GameFunctions::displayLabel(m_EmployeeCountText, Color4B::WHITE, Vec2(panelMidPoint.x - 130.f, textPos.y), employeeWidget, 1,
			true, TextHAlignment::LEFT);
	}

	for (unsigned index = 0; index < 2; ++index)
	{
		auto button = (index == 0)? MouseOverMenuItem::creatMouseOverMenuButton(CC_CALLBACK_1(ShopEmployeeAdmin::hireCallback, this)) :
			MouseOverMenuItem::creatMouseOverMenuButton(CC_CALLBACK_1(ShopEmployeeAdmin::fireCallback, this));
		if (button)
		{
			m_MenuItems.pushBack(displayMenuButton(button, CC_CALLBACK_2(ShopEmployeeAdmin::onMouseOver, this), Vec2((index == 0)? 
				panelMidPoint.x - 50.f : panelMidPoint.x +150.f, panelMidPoint.y), itemTypes::BUTTON, 0.7f, true));

			auto buttonText = Label::createWithTTF((index == 0) ? "Hire" : "Fire", "fonts/NirmalaB.ttf", 20);
			if (buttonText)
			{
				buttonText->enableGlow(Color4B::BLACK);
				GameFunctions::displayLabel(buttonText, Color4B::WHITE, button->getContentSize() * 0.5f, button, 1);
			}
		}
	}
	m_MenuItems.at(EButtons::E_Fire)->setEnabled(false);

	auto buttonMenu = Menu::createWithArray(m_MenuItems);
	buttonMenu->setPosition(Vec2::ZERO);
	employeeWidget->addChild(buttonMenu, 1);

	// create candidate list
	std::array<std::string, 10> candidatesLinks = { "Employee_Man1.png", "Employee_Man2.png", "Employee_Man3.png", "Employee_Man4.png", 
		"Employee_Man5.png", "Employee_Woman1.png", "Employee_Woman2.png", "Employee_Woman3.png", "Employee_Woman4.png", "Employee_Woman5.png" };
	std::array<std::string, 10> candidatesName = { "Mike Stand", "Jack Muhammad", "Erik Andersson", "stefan King", "Joseph A. Dahl",
		"Erinne Berglund", "Karishma Gujarati", "Samatha Miler", "Jannifer Perez", "Catherine Nguyen" };
	for (unsigned index = 0; index < 10; ++index)
	{
		m_CandidateDatas.push_back(new EmployeeData(index, candidatesName[index], candidatesLinks[index]));
	}

	m_SalaryWidget = ui::Widget::create();
	m_SalaryWidget->setPosition(Vec2::ZERO);
	m_SalaryWidget->setVisible(false);
	employeeWidget->addChild(m_SalaryWidget, 1);
	
	// create firt profile widget
	auto widget = ui::Widget::create();
	widget->setPosition(Vec2::ZERO);
	m_SalaryWidget->addChild(widget, 1);
	m_ProfileWidgets.push_back(widget);

	// create employee salary
	auto employeeSalaryText = Label::createWithTTF("Total Salary Expense", "fonts/NirmalaB.ttf", 16);
	if (employeeSalaryText)
	{
		employeeSalaryText->enableGlow(Color4B::BLACK);
		GameFunctions::displayLabel(employeeSalaryText, Color4B::WHITE, Vec2(textPos.x, textPos.y - 50.f), m_SalaryWidget, 1, 
			true, TextHAlignment::LEFT);
	}

	m_TotalSalaryText = Label::createWithTTF("", "fonts/Nirmala.ttf", 16);
	if (m_TotalSalaryText)
	{
		m_TotalSalaryText->enableShadow(darkCyanColor);
		GameFunctions::displayLabel(m_TotalSalaryText, Color4B::WHITE, Vec2(textPos.x + 150.f, textPos.y - 50.f), m_SalaryWidget, 1, 
			true, TextHAlignment::LEFT);
	}

	m_DisplayPicPos = Vec2(panelMidPoint.x - 200.f, panelMidPoint.y - 100.f);
}

void ShopEmployeeAdmin::hireCallback(cocos2d::Ref* pSender)
{
	auto newEmployeeId = getRandomEmployee();
	while (m_Shop->isMyEmployee(newEmployeeId))
	{
		newEmployeeId = getRandomEmployee();
	}

	//update shop data
	m_Shop->addEmployee(1, newEmployeeId);
	
	auto employeeCount = m_Shop->getEmployeeCount();
	if (employeeCount == 10)
		switchButtonStates(EButtons::E_Hire, false);

	m_EmployeeCountText->setString(std::to_string(employeeCount));
	displayEmployeeProfilePic(newEmployeeId, employeeCount);

	if (onEmployeeCountChanges)
		onEmployeeCountChanges(employeeCount);

	if (!m_MenuItems.at(EButtons::E_Fire)->isEnabled())
		switchButtonStates(EButtons::E_Fire, true);
}

void ShopEmployeeAdmin::fireCallback(cocos2d::Ref* pSender)
{
	removeEmployee();

	if (!m_MenuItems.at(EButtons::E_Hire)->isEnabled())
		switchButtonStates(EButtons::E_Hire, true);
	
	if (m_Shop->getEmployeeCount() == 0)
		switchButtonStates(EButtons::E_Fire, false);
}

void ShopEmployeeAdmin::openEmployeeProfileWidget(cocos2d::Ref* pSender, unsigned widgetIndex)
{
	for (unsigned index = 0; index < m_ProfileWidgets.size(); ++index)
	{
		if (index == widgetIndex)
		{
			if (m_ProfileWidgets.at(index)->isVisible())
			{
				m_ProfileWidgets.at(index)->setVisible(false);
				continue;
			}
			m_ProfileWidgets.at(index)->setVisible(true);
			continue;
		}
		m_ProfileWidgets.at(index)->setVisible(false);
	}
}

void ShopEmployeeAdmin::onMouseOver(MouseOverMenuItem* menuItem, Event* event)
{
}

unsigned int ShopEmployeeAdmin::getRandomEmployee()
{
	auto candidateCount = m_CandidateDatas.size() - 1;
	auto randNo = random<unsigned int>(0, candidateCount);
	return m_CandidateDatas.at(randNo)->m_Id;
}

void ShopEmployeeAdmin::displayEmployeeProfilePic(unsigned id,unsigned count)
{
	if (!m_SalaryWidget->isVisible())
		m_SalaryWidget->setVisible(true);

	auto page = 0;
	if (hasCreatedNextPage(count))
		page = 1;

	// set pic
	auto profilePic = Sprite::createWithSpriteFrameName(m_CandidateDatas.at(id)->m_ProfileLink);
	if (profilePic)
	{
		m_HiredEmployees.push_back(std::make_pair(id, profilePic));
		GameFunctions::displaySprite(profilePic, m_DisplayPicPos, m_ProfileWidgets.at(page), 1, 0.4f, 0.4f);

		auto name = Label::createWithTTF(m_CandidateDatas.at(id)->m_Name, "fonts/Nirmala.ttf", 30);
		if (name)
		{
			name->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(name, Color4B::WHITE, Vec2(profilePic->getContentSize().width * 0.5f, -20.f), profilePic, 1);
		}
	}

	// set salary
	m_TotalSalaryText->setString(std::to_string(m_Shop->getTotalSalaryExpense()));

	m_DisplayPicPos.x += m_PicDistance;
}

bool ShopEmployeeAdmin::hasCreatedNextPage(unsigned count)
{
	if (m_ProfileWidgets.size() > 1)
		return true;

	if (count == m_WidgetNextPage)
	{
		auto moreButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonMore50.png", "UIButtonMore50_Lit.png",
			"UIButtonMore50_Disable.png", CC_CALLBACK_1(ShopEmployeeAdmin::openEmployeeProfileWidget, this, m_ProfileWidgets.size()));
		if (moreButton)
		{
			moreButton->itemSelectedData.type = itemTypes::BUTTON;
			displayMenuButton(moreButton, CC_CALLBACK_2(ShopEmployeeAdmin::onMouseOver, this),
				Vec2(m_DisplayPicPos.x - 305.f, m_DisplayPicPos.y - 70.f), itemTypes::BUTTON, 0.5f, true);

			auto buttonMenu = Menu::create(moreButton, NULL);
			buttonMenu->setPosition(Vec2::ZERO);
			m_ProfileWidgets.at(EPages::E_ProfilePage1)->addChild(buttonMenu, 1);

			auto widget = ui::Widget::create();
			widget->setPosition(Vec2::ZERO);
			widget->setVisible(false);
			m_SalaryWidget->addChild(widget, 1);
			m_ProfileWidgets.push_back(widget);
			

			auto lessButton = MouseOverMenuItem::creatMouseOverMenuButton("UIButtonLess50.png", "UIButtonLess50_Lit.png",
				"UIButtonLess50_Disable.png", CC_CALLBACK_1(ShopEmployeeAdmin::openEmployeeProfileWidget, this, m_ProfileWidgets.size() - 2));
			if (lessButton)
			{
				lessButton->itemSelectedData.type = itemTypes::WIDGET_BUTTON;
				displayMenuButton(lessButton, CC_CALLBACK_2(ShopEmployeeAdmin::onMouseOver, this), Vec2(m_DisplayPicPos.x - 305.f,
					m_DisplayPicPos.y - 70.f), itemTypes::WIDGET_BUTTON, 0.5f, true);

				auto lessMenu = Menu::create(lessButton, NULL);
				lessMenu->setPosition(Vec2::ZERO);
				m_ProfileWidgets.at(EPages::E_ProfilePage2)->addChild(lessMenu, 1);
			}

			m_DisplayPicPos.x -= m_PicDistance * 5;
		}

		return true;
	}
	return false;
}

void ShopEmployeeAdmin::switchButtonStates(EButtons button, bool enable)
{
	m_MenuItems.at(button)->setEnabled(enable);
}

void ShopEmployeeAdmin::removeEmployee()
{
	if (m_HiredEmployees.empty())
		return;

	// update shop data
	auto lastIndex = m_HiredEmployees.size() -1;
	m_Shop->fireEmployee(1, m_HiredEmployees.at(lastIndex).first);

	// remove form scene
	m_HiredEmployees.at(lastIndex).second->removeFromParent();
	m_HiredEmployees.erase(m_HiredEmployees.end() - 1);

	// correct next pic position
	m_DisplayPicPos.x -= m_PicDistance;

	// update display data
	auto employeeCount = m_Shop->getEmployeeCount();
	m_EmployeeCountText->setString(std::to_string(employeeCount));
	m_TotalSalaryText->setString(std::to_string(m_Shop->getTotalSalaryExpense()));

	if (onEmployeeCountChanges)
		onEmployeeCountChanges(employeeCount);
}
