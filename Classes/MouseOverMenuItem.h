#pragma once

#ifndef __MOUSEOVER__MOUSEOVERMENUITEM__
#define __MOUSEOVER__MOUSEOVERMENUITEM__

#include "cocos2d.h"

USING_NS_CC;

class MouseOverMenuItem : public MenuItemImage
{
public:
	~MouseOverMenuItem();

	static MouseOverMenuItem* create(const std::string& normalImage, const std::string& overImage, const std::string& disabledImage, const ccMenuCallback& callback);
	static MouseOverMenuItem* create(Node* normalSprite, Node* overSprite, Node* disabledSprite, const ccMenuCallback& callback);

	std::function<void(MouseOverMenuItem* overItem, Event* event)> onMouseOver;
	void setItemRect(Vec2& pos, float scale = 1.f);
	void setSelected(bool value);
protected:
	EventListenerMouse* mouseListener;

private :
	Rect itemRect;
	bool isHovering = false;
	bool isSelected = false;
	void onMouseMove(Event* event);
	void setMouseListener();
};

#endif
