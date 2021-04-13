#pragma once

#include "UIPanel.h"

NS_CC_BEGIN
namespace ui
{
	class CheckBox;
	class Widget;
}
NS_CC_END

class MouseOverMenuItem;
class MyShopSettingPanel;
class Shop;
class GameTime;
class ShopProductAdmin;

class ShopAdmin : public UIPanel
{
public:
	virtual ~ShopAdmin();

	std::vector<std::pair<cocos2d::ui::CheckBox*, cocos2d::ui::Widget*>> m_AdminTabs;
	void createAdmin(Shop* shop, cocos2d::ui::Widget* adminWidget, cocos2d::Vec2 panelMidPoint);

	void update();
	std::function<void(unsigned weekday)> onWorkDayChanges;
	std::function<void(unsigned workhour)> onWorkHourChanges;

	ShopProductAdmin* m_ShopProductAdmin = nullptr;
protected:
	void onOpenTabCallback(cocos2d::Ref* pSender, unsigned tabIndex);
	void checkBoxClickCallback(cocos2d::Ref* pSender, unsigned week);
	void reduceTimeCallback(cocos2d::Ref* pSender, unsigned weekday, bool fromHourButton);
	void increaseTimeCallback(cocos2d::Ref* pSender, unsigned weekday, bool fromHourButton);

	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);

private:
	Shop* m_MyShop = nullptr;
	GameTime* m_GameTime = nullptr;

	std::vector<cocos2d::ui::CheckBox*> m_Weekdays;
	std::vector<std::pair<cocos2d::Label*, cocos2d::Label*>> m_FromToTexts;
	const unsigned m_FromHourIncreaseStartIndex = 1;
	const unsigned m_ToHourDecreaseStartIndex = 14;
	const unsigned m_ToHourIncreaseStartIndex = 15;

	unsigned m_FromHour = 9;
	unsigned m_ToHour = 17;

	void enableHoursButtons(unsigned weekday, bool enable);

};
