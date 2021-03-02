#pragma once

#ifndef __MOUSEOVERBUTTON__MOUSEOVERMENUITEM__
#define __MOUSEOVERBUTTON__MOUSEOVERMENUITEM__

#include "cocos2d.h"
#include "UI/UIButton.h"

USING_NS_CC;

class MouseOverButton : public ui::Button
{
public:
	~MouseOverButton();

	static MouseOverButton* create(Node* normalSprite, Node* overSprite, Node* disabledSprite, const ccMenuCallback& callback);

};

#endif
