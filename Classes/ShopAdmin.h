#pragma once

#include "UIPanel.h"

NS_CC_BEGIN
namespace ui
{
	class CheckBox;
	class Widget;
}
NS_CC_END

class MyShopSettingPanel;
class Shop;

class ShopAdmin : public UIPanel
{
public:
	virtual ~ShopAdmin();

	std::vector<std::pair<cocos2d::ui::CheckBox*, cocos2d::ui::Widget*>> m_AdminTabs;
	void createAdmin(Shop* shop, cocos2d::ui::Widget* adminWidget, cocos2d::Vec2 panelMidPoint);

	void update();

	std::function<void(unsigned weekday)> onWorkDayChanges;
	std::function<void(unsigned workhour)> onWorkHourChanges;
protected:
	void onOpenTabCallback(cocos2d::Ref* pSender, unsigned tabIndex);
	void checkBoxClickCallback(cocos2d::Ref* pSender, unsigned week);
	void reduceTimeCallback(cocos2d::Ref* pSender, bool fromHourButton);
	void increaseTimeCallback(cocos2d::Ref* pSender, bool fromHourButton);

private:
	Shop* m_MyShop = nullptr;
	cocos2d::Label* m_FromHourText = nullptr;
	cocos2d::Label* m_ToHourText = nullptr;

	std::vector<cocos2d::ui::CheckBox*> m_Weekdays;


	unsigned m_FromHour = 9;
	unsigned m_ToHour = 17;


};
