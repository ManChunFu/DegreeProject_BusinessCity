#include "ShopEmployeeAdmin.h"
#include "ui/UIWidget.h"
#include "Shop.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "MouseOverMenuItem.h"
#include "EmployeeData.h"
#include "cocostudio/SimpleAudioEngine.h"
using namespace CocosDenshion;

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
	m_HiredEmployees.clear();
	m_NextBackButtons.clear();
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
		auto button = (index == 0) ? MouseOverMenuItem::creatMouseOverMenuButton(CC_CALLBACK_1(ShopEmployeeAdmin::hireCallback, this)) :
			MouseOverMenuItem::creatMouseOverMenuButton(CC_CALLBACK_1(ShopEmployeeAdmin::fireCallback, this));
		if (button)
		{
			m_MenuItems.pushBack(displayMenuButton(button, CC_CALLBACK_2(ShopEmployeeAdmin::onMouseOver, this), Vec2((index == 0) ?
				panelMidPoint.x - 50.f : panelMidPoint.x + 150.f, panelMidPoint.y), itemTypes::BUTTON, 0.7f, true));

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

	m_DisplayPicPos = Vec2(panelMidPoint.x - 155.f, panelMidPoint.y - 110.f);

	if (m_Shop->getEmployeeCount() > 0)
	{
		for (unsigned index = 0; index <m_Shop->m_EmployeesIds.size(); ++index)
		{
			displayEmployeeProfilePic(m_Shop->m_EmployeesIds.at(index), index + 1);
		}
		switchButtonStates(EButtons::E_Fire, true);
	}
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

	if (!m_NextBackButtons.empty())
		updateShiftButtonsVisibility();
}

void ShopEmployeeAdmin::shiftForwardProfile(cocos2d::Ref* pSender)
{
	m_LastDisplayPicIndex = clampf(++m_LastDisplayPicIndex, 3, m_CandidateDatas.size() - 1);

	auto listCount = m_HiredEmployees.size();

	// shift left
	for (unsigned index = 0; index < listCount; ++index)
	{
		m_HiredEmployees.at(index)->m_ProfilePic->setPositionX(m_HiredEmployees.at(index)->m_PicPositionX -= m_PicDistance);
		if (index < m_LastDisplayPicIndex - 3 || index > m_LastDisplayPicIndex)
		{
			m_HiredEmployees.at(index)->m_ProfilePic->setVisible(false);
			continue;
		}
		m_HiredEmployees.at(index)->m_ProfilePic->setVisible(true);
	}

	updateShiftButtonsVisibility();
}

void ShopEmployeeAdmin::shiftBackwardProfile(cocos2d::Ref* pSender)
{
	m_LastDisplayPicIndex = clampf(--m_LastDisplayPicIndex, 3, m_CandidateDatas.size() -1);

	// shift right
	for (int index = m_HiredEmployees.size() -1; index > -1; --index)
	{
		m_HiredEmployees.at(index)->m_ProfilePic->setPositionX(m_HiredEmployees.at(index)->m_PicPositionX += m_PicDistance);
		if (index < m_LastDisplayPicIndex - 3 || index > m_LastDisplayPicIndex)
		{
			m_HiredEmployees.at(index)->m_ProfilePic->setVisible(false);
			continue;
		}
		m_HiredEmployees.at(index)->m_ProfilePic->setVisible(true);
	}

	updateShiftButtonsVisibility();

}

void ShopEmployeeAdmin::onMouseOver(MouseOverMenuItem* menuItem, Event* event)
{
	GameData::getInstance().m_Audio->playEffect("Sounds/SelectedSound.mp3", false, 1.f, 1.f, 1.f);
}

unsigned int ShopEmployeeAdmin::getRandomEmployee()
{
	auto candidateCount = m_CandidateDatas.size() - 1;
	auto randNo = random<unsigned int>(0, candidateCount);
	return m_CandidateDatas.at(randNo)->m_Id;
}

void ShopEmployeeAdmin::displayEmployeeProfilePic(unsigned id, unsigned count)
{
	if (!m_SalaryWidget->isVisible())
		m_SalaryWidget->setVisible(true);

	if (count == m_WidgetLimit)
		createdShiftButtons();

	// set pic
	auto profilePic = Sprite::createWithSpriteFrameName(m_CandidateDatas.at(id)->m_ProfileLink);
	if (profilePic)
	{
		if (count > 4)
			m_DisplayPicPos.x = m_HiredEmployees.at(m_HiredEmployees.size() - 1)->m_PicPositionX + m_PicDistance;
		GameFunctions::displaySprite(profilePic, m_DisplayPicPos, m_SalaryWidget, 1, 0.4f, 0.4f);

		auto name = Label::createWithTTF(m_CandidateDatas.at(id)->m_Name, "fonts/Nirmala.ttf", 30);
		if (name)
		{
			name->enableShadow(Color4B::BLACK);
			GameFunctions::displayLabel(name, Color4B::WHITE, Vec2(profilePic->getContentSize().width * 0.5f, -20.f), profilePic, 1);
		}

		m_HiredEmployees.push_back(new EmployeeData(id, profilePic, m_DisplayPicPos.x));
	}

	if (count > 4)
	{
		updateShiftButtonsVisibility();

		if (m_NextBackButtons.at(EShiftButtons::E_Forward)->isVisible())
			profilePic->setVisible(false);
	}

	// set salary
	m_TotalSalaryText->setString(std::to_string(m_Shop->getTotalSalaryExpense()));

	m_DisplayPicPos.x += m_PicDistance;
}

void ShopEmployeeAdmin::createdShiftButtons()
{
	for (unsigned index = 0; index < 2; ++index)
	{
		auto button = (index == 0) ? MouseOverMenuItem::creatMouseOverMenuButton("UIButtonMore50_PointRight.png",
			"UIButtonMore50_PointRight_Lit.png", "UIButtonMore50_PointRight_Disable.png", CC_CALLBACK_1(
				ShopEmployeeAdmin::shiftForwardProfile, this)) : MouseOverMenuItem::creatMouseOverMenuButton(
					"UIButtonLess50_PointLeft.png", "UIButtonLess50_PointLeft_Lit.png", "UIButtonLess50_PointLeft_Disable.png",
					CC_CALLBACK_1(ShopEmployeeAdmin::shiftBackwardProfile, this));
		if (button)
		{
			button->itemSelectedData.type = itemTypes::BUTTON;
			m_NextBackButtons.push_back(displayMenuButton(button, CC_CALLBACK_2(ShopEmployeeAdmin::onMouseOver, this), Vec2((index == 0) ?
				m_DisplayPicPos.x - 30.f : m_DisplayPicPos.x - (m_PicDistance * 5 - 32.f), m_DisplayPicPos.y), itemTypes::DEFAULT, 0.5f, true));

			auto buttonMenu = Menu::create(button, NULL);
			buttonMenu->setPosition(Vec2::ZERO);
			m_SalaryWidget->addChild(buttonMenu, 1);
			
			button->setVisible(false);
		}
	}
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
	auto lastIndex = m_HiredEmployees.size() - 1;
	m_Shop->fireEmployee(1, m_HiredEmployees.at(lastIndex)->m_Id);

	// remove form scene
	m_HiredEmployees.at(lastIndex)->m_ProfilePic->removeFromParent();
	m_HiredEmployees.erase(m_HiredEmployees.end() - 1);

	// correct next pic position
	m_DisplayPicPos.x -= m_PicDistance;

	// update display data
	auto employeeCount = m_Shop->getEmployeeCount();
	m_EmployeeCountText->setString(std::to_string(employeeCount));
	m_TotalSalaryText->setString(std::to_string(m_Shop->getTotalSalaryExpense()));

	if (onEmployeeCountChanges)
		onEmployeeCountChanges(employeeCount);

	if (m_LastDisplayPicIndex > m_HiredEmployees.size() - 1 && m_LastDisplayPicIndex > 3)
		shiftBackwardProfile(this);
	else
		updateShiftButtonsVisibility();

}

void ShopEmployeeAdmin::updateShiftButtonsVisibility()
{
	if (m_HiredEmployees.empty())
		return;

	auto lastIndex = m_HiredEmployees.size() -1;
	if (lastIndex > m_LastDisplayPicIndex)
	{
		m_NextBackButtons.at(EShiftButtons::E_Forward)->setVisible(true);
		
		if (m_LastDisplayPicIndex > 3)
		{
			m_NextBackButtons.at(EShiftButtons::E_Backward)->setVisible(true);
			return;
		}

		m_NextBackButtons.at(EShiftButtons::E_Backward)->setVisible(false);
		return;
	}

	if (lastIndex == m_LastDisplayPicIndex)
	{
		m_NextBackButtons.at(EShiftButtons::E_Forward)->setVisible(false);

		if (m_LastDisplayPicIndex > 3)
		{
			m_NextBackButtons.at(EShiftButtons::E_Backward)->setVisible(true);
			return;
		}

		m_NextBackButtons.at(EShiftButtons::E_Backward)->setVisible(false);
		return;
	}

	if (lastIndex < 3)
	{
		for (auto button : m_NextBackButtons)
		{
			button->setVisible(false);
		}
	}
}
