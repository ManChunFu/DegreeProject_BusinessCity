#pragma once

#include "UIPanel.h"

class Player;
class MouseOverMenuItem;

NS_CC_BEGIN
namespace ui
{
	class CheckBox;
}
NS_CC_END

class MyShopSettingPanel : public UIPanel
{
public:
	~MyShopSettingPanel() override;

	void openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint) override;
	void closePanel() override;
	std::function<void(cocos2d::Ref* pSender)> onActionCall;

protected:	
	void createPanel(cocos2d::Vec2 sceneMidPoint) override;
	void reduceCallback(cocos2d::Ref* pSender);
	void addCallback(cocos2d::Ref* pSender);
	void reduceTimeCallback(cocos2d::Ref* pSender, bool fromHourButton);
	void increaseTimeCallback(cocos2d::Ref* pSender, bool fromHourButton);
	void actionCallback(cocos2d::Ref* pSender);
	void onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event);

private:
	cocos2d::Label* m_EmployeeCountText = nullptr;
	cocos2d::ui::CheckBox* m_WorkHere = nullptr;
	cocos2d::Label* m_FromHourText = nullptr;
	cocos2d::Label* m_ToHourText = nullptr;
	std::vector<cocos2d::ui::CheckBox*> m_Weekdays;

	bool m_OwnerWorkHere = false;
	unsigned m_EmployeeCount = 0;
	unsigned m_FromHour = 8;
	unsigned m_ToHour = 17;
};