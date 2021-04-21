#pragma once

#include "UIPanel.h"

NS_CC_BEGIN
namespace ui
{
	class Widget;
}
NS_CC_END

class Shop;
class MouseOverMenuItem;
struct EmployeeData;

class ShopEmployeeAdmin : public UIPanel
{
public:
	virtual ~ShopEmployeeAdmin();
	void createShopEmployeeAdmin(Shop* shop, cocos2d::ui::Widget* employeeWidget, cocos2d::Vec2 panelMidPoint);

	std::function<void(unsigned employeeCount)> onEmployeeCountChanges;
protected:
	void hireCallback(cocos2d::Ref* pSender);
	void fireCallback(cocos2d::Ref* pSender);
	void shiftForwardProfile(cocos2d::Ref* pSender);
	void shiftBackwardProfile(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);

private:
	enum EButtons
	{
		E_Hire = 0,
		E_Fire = 1
	};
	enum EShiftButtons
	{
		E_Forward = 0,
		E_Backward = 1
	};
	Shop* m_Shop = nullptr;
	cocos2d::ui::Widget* m_SalaryWidget = nullptr;
	cocos2d::Label* m_EmployeeCountText = nullptr;
	cocos2d::Label* m_TotalSalaryText = nullptr;
	std::vector<EmployeeData*>m_HiredEmployees;
	std::vector<EmployeeData*> m_CandidateDatas;
	std::vector<MouseOverMenuItem*> m_NextBackButtons;

	cocos2d::Vec2 m_DisplayPicPos = cocos2d::Vec2::ZERO;
	unsigned m_WidgetLimit = 5;
	float m_PicDistance = 102.f;
	unsigned m_LastDisplayPicIndex = 3;

	unsigned int getRandomEmployee();
	void displayEmployeeProfilePic(unsigned id, unsigned count);
	void createdShiftButtons();
	void switchButtonStates(EButtons button, bool enable);
	void removeEmployee();
	void updateShiftButtonsVisibility();

};
