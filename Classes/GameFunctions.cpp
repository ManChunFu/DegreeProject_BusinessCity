#include "GameFunctions.h"
#include "MouseOverMenuItem.h"

USING_NS_CC;
void GameFunctions::displayLabel(Label* label, Color4B color, Vec2 pos, Node* parent, unsigned z_order)
{
	label->setTextColor(color);
	label->setPosition(pos);
	parent->addChild(label, z_order);
}

