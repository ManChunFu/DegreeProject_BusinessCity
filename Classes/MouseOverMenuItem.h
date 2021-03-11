#pragma once

#ifndef __MOUSEOVER__MOUSEOVERMENUITEM__
#define __MOUSEOVER__MOUSEOVERMENUITEM__

#include "cocos2d.h"

namespace base
{
	class EventListenerMouse;
}

class MouseOverMenuItem : public cocos2d::MenuItemImage
{
public:
	~MouseOverMenuItem();

	static MouseOverMenuItem* creatMouseOverMenuButton(const std::string& normalPath, const std::string& selectedPath, const std::string& disabledPath, const cocos2d::ccMenuCallback& callback);
	static MouseOverMenuItem* create(const std::string& normalImage, const std::string& overImage, const std::string& disabledImage, const cocos2d::ccMenuCallback& callback);
	static MouseOverMenuItem* create(Node* normalSprite, Node* overSprite, Node* disabledSprite, const cocos2d::ccMenuCallback& callback);

	std::function<void(MouseOverMenuItem* overItem,cocos2d::Event* event)> onMouseOver;
	void setItemRect(cocos2d::Vec2& pos, float scale = 1.f);
protected:
	cocos2d::EventListenerMouse* m_MouseListener;

private :
	cocos2d::Rect m_ItemRect;
	bool m_IsHovering = false;
	bool m_IsSelected = false;
	void onMouseMove(cocos2d::Event* event);
	void setMouseListener();
};
#endif
