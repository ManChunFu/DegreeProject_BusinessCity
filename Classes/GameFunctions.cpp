#include "GameFunctions.h"

void GameFunctions::displayLabel(cocos2d::Label* label, cocos2d::Color4B color, cocos2d::Vec2 pos, cocos2d::Node* parent, unsigned z_order)
{
	label->setTextColor(color);
	label->setPosition(pos);
	parent->addChild(label, z_order);
}
