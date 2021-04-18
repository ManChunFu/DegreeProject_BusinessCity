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

class ShopEmployeeAdmin : public UIPanel
{
public:
	virtual ~ShopEmployeeAdmin();
	void createShopEmployeeAdmin(Shop* shop, cocos2d::ui::Widget* employeeWidget, cocos2d::Vec2 panelMidPoint);

protected:
	void reduceCallback(cocos2d::Ref* pSender);
	void addCallback(cocos2d::Ref* pSender);
	void hireCallback(cocos2d::Ref* pSender);
	void workHereCallback(cocos2d::Ref* pSender);

	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);

private:
	Shop* m_Shop = nullptr;
	cocos2d::Label* m_EmployeeCountText = nullptr;
	cocos2d::Label* m_CurrentEmployeeCountText = nullptr;
	cocos2d::Label* m_EmployeeSalaryText = nullptr;

	unsigned m_EmployeeCount = 0;


};
