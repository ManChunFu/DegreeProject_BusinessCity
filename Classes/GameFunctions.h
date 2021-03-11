#pragma once
#include "cocos2d.h"

struct GameFunctions
{
	static void displayLabel(cocos2d::Label* label, cocos2d::Color4B color, cocos2d::Vec2 pos, cocos2d::Node* parent, unsigned z_order, bool anchor = false);

	static void updateLabelText_MoneyFormat(cocos2d::Label* label, int value, bool minus = false);

	static void updatLabelText_TimeFormat(cocos2d::Label* label, unsigned value);

};
