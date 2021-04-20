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
	void openEmployeeProfileWidget(cocos2d::Ref* pSender, unsigned widgetIndex);

	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);

private:
	enum EButtons
	{
		E_Hire = 0,
		E_Fire = 1
	};
	enum EPages
	{
		E_ProfilePage1 = 0,
		E_ProfilePage2 = 1
	};
	Shop* m_Shop = nullptr;
	cocos2d::ui::Widget* m_SalaryWidget = nullptr;
	std::vector<cocos2d::ui::Widget*> m_ProfileWidgets;
	cocos2d::Label* m_EmployeeCountText = nullptr;
	cocos2d::Label* m_TotalSalaryText = nullptr;
	std::vector<std::pair<unsigned, cocos2d::Sprite*>>m_HiredEmployees;
	
	std::vector<EmployeeData*> m_CandidateDatas;
	
	cocos2d::Vec2 m_DisplayPicPos = cocos2d::Vec2::ZERO;
	unsigned m_WidgetNextPage = 6;
	float m_PicDistance = 100.f;
	

	unsigned int getRandomEmployee();
	void displayEmployeeProfilePic(unsigned id, unsigned count);
	bool hasCreatedNextPage(unsigned count);
	void switchButtonStates(EButtons button, bool enable);
	void removeEmployee();

};
