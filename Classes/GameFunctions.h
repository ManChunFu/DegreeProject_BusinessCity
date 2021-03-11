#pragma once
#include "cocos2d.h"

class MouseOverMenuItem;
struct GameFunctions
{
	static void displayLabel(cocos2d::Label* label, cocos2d::Color4B color, cocos2d::Vec2 pos, cocos2d::Node* parent, unsigned z_order);

	static void updateLabelText_MoneyFormat(cocos2d::Label* label, unsigned value, bool minus = false);
};
