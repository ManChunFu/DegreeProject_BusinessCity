#include "GameFunctions.h"
#include "MouseOverMenuItem.h"

USING_NS_CC;
void GameFunctions::displayLabel(Label* label, Color4B color, Vec2 pos, Node* parent, unsigned z_order, bool anchor)
{
	label->setTextColor(color);
	label->setPosition(pos);
	if (anchor)
		label->setAnchorPoint(Vec2::ONE);
	parent->addChild(label, z_order);
}

void GameFunctions::updateLabelText_MoneyFormat(Label* label, int value, bool minus)
{
	std::string cashStr = "--";
	if (value != 0)
	{
		cashStr = std::to_string(value);
		auto cashStrLength =  cashStr.length();
		if (cashStrLength > 3)
			cashStr = cashStr.insert(cashStr.length() - 3, 1, ',');
		if (cashStrLength > 6)
			cashStr = cashStr.insert(cashStr.length() - 7, 1, ',');
		if (minus)
			cashStr.insert(0, "- ");
	}
	label->setString(cashStr);
}

