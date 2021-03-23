#include "GameFunctions.h"
#include "ui/UITextField.h"

USING_NS_CC;

void GameFunctions::displaySprite(cocos2d::Sprite* sprite, cocos2d::Vec2 pos, cocos2d::Node* parent, unsigned z_order, float scaleX, float scaleY)
{
	sprite->setScale(scaleX, scaleY);
	sprite->setPosition(pos);
	parent->addChild(sprite, z_order);
}

void GameFunctions::displayTextField(ui::TextField* textField, Color3B color, Vec2 pos, Node* parent, unsigned z_order, bool anchor, unsigned maxLength)
{
	textField->setColor(color);
	if (maxLength > 0)
	{
		textField->setMaxLength(maxLength);
		textField->setMaxLengthEnabled(true);
	}
	if (anchor)
		textField->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	textField->setPosition(pos);
	parent->addChild(textField, z_order);
}

void GameFunctions::displayLabel(Label* label, Color4B color, Vec2 pos, Node* parent, unsigned z_order, bool anchor, TextHAlignment alignment)
{
	label->setTextColor(color);
	label->setPosition(pos);
	if (anchor)
	{
		switch (alignment)
		{
		case cocos2d::TextHAlignment::LEFT:
			label->setAnchorPoint(Vec2::ZERO);
			break;
		case cocos2d::TextHAlignment::RIGHT:
			label->setAnchorPoint(Vec2::ONE);
			break;
		}
	}
	parent->addChild(label, z_order);
}

void GameFunctions::updateLabelText_MoneyFormat(Label* label, int value, bool minus)
{
	std::string cashStr = "--";
	if (value != 0)
	{
		cashStr = std::to_string(value);
		auto cashStrLength =  (value < 0)? cashStr.length() -1 : cashStr.length();
		if (cashStrLength > 3)
			cashStr = cashStr.insert(cashStr.length() - 3, 1, ',');
		if (cashStrLength > 6)
			cashStr = cashStr.insert(cashStr.length() - 7, 1, ',');
		if (minus)
			cashStr.insert(0, "- ");
	}
	label->setString(cashStr);
}

void GameFunctions::updatLabelText_TimeFormat(cocos2d::Label* label, unsigned value)
{
	if (value == 24)
		value = 0;
	std::string timeStr = std::to_string(value);
	label->setString(std::string(2 - timeStr.length(), '0').append(timeStr));
}

unsigned GameFunctions::displayLabelText_ClampValue(cocos2d::Label* label, int originValue, int newValue, unsigned minValue, unsigned maxValue)
{
	originValue += newValue;
	originValue = clampf(originValue, minValue, maxValue);
	GameFunctions::updateLabelText_MoneyFormat(label, originValue);
	return originValue;
}



