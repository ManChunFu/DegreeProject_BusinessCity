#pragma once
#include "cocos2d.h"

NS_CC_BEGIN;
namespace ui
{
	class TextField;
}
NS_CC_END;

struct GameFunctions
{
	static void displaySprite(cocos2d::Sprite* sprite, cocos2d::Vec2 pos, cocos2d::Node* parent, unsigned z_order, float scaleX = 1.f, float scaleY = 1.f);

	static void displayTextField(cocos2d::ui::TextField* textField, cocos2d::Color3B color, cocos2d::Vec2 pos, cocos2d::Node* parent, unsigned z_order, bool anchor = false, unsigned maxLength = 0);

	static void displayLabel(cocos2d::Label* label, cocos2d::Color4B color, cocos2d::Vec2 pos, cocos2d::Node* parent, unsigned z_order, bool anchor = false, cocos2d::TextHAlignment alignment = cocos2d::TextHAlignment::CENTER);
	
	static void updateLabelText_MoneyFormat(cocos2d::Label* label, int value, bool minus = false);

	static void updatLabelText_TimeFormat(cocos2d::Label* label, unsigned value);

};
