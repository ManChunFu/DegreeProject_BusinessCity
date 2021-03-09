#pragma once

#ifndef __MOUSEOVER__MOUSEOVERMENUITEM__
#define __MOUSEOVER__MOUSEOVERMENUITEM__

#include "cocos2d.h"

USING_NS_CC;


class MouseOverMenuItem : public cocos2d::MenuItemImage
{
public:
	~MouseOverMenuItem();

	static MouseOverMenuItem* create(const std::string& normalImage, const std::string& overImage, const std::string& disabledImage, const ccMenuCallback& callback);
	static MouseOverMenuItem* create(Node* normalSprite, Node* overSprite, Node* disabledSprite, const ccMenuCallback& callback);

	std::function<void(MouseOverMenuItem* overItem,Event* event)> onMouseOver;
	void setItemRect(cocos2d::Vec2& pos, float scale = 1.f);
protected:
	EventListenerMouse* m_MouseListener;

private :
	Rect m_ItemRect;
	bool m_IsHovering = false;
	bool m_IsSelected = false;
	void onMouseMove(Event* event);
	void setMouseListener();
};
#endif
