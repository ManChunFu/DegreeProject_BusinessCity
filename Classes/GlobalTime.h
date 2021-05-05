#pragma once

#include "cocos2d.h"

struct GameTime;
class UIPanel;

class GlobalTime
{
public:
	GlobalTime();
	virtual ~GlobalTime();

	void update(float delta);
	
	typedef std::function<void(GlobalTime*, unsigned)> onTimeChanges;
	
	void addMinuteEventListener(const onTimeChanges& changes, cocos2d::Node* node);
	void addHourEventListener(const onTimeChanges& changes, cocos2d::Node* node);
	void removdMinuteEventListener(cocos2d::Node* node);
	void removeHourEventListener(cocos2d::Node* node);

	onTimeChanges onEveryDayChanges;
	onTimeChanges onEveryWeekChanges;

	GameTime* m_Gametime = nullptr;

	void speedUp();
protected:
	float m_ElapsedTime = 0.f;
	float m_DeltaCount = 3.f;
	std::unordered_map<cocos2d::Node*, onTimeChanges> m_MinuteListeners;
	std::unordered_map<cocos2d::Node*, onTimeChanges> m_HourListeners;
};

