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
	virtual ~MouseOverMenuItem();

	static MouseOverMenuItem* createUpperButton(const cocos2d::ccMenuCallback& callback);
	static MouseOverMenuItem* createLowerButton(const cocos2d::ccMenuCallback& callback);
	static MouseOverMenuItem* createIncreaseButton(const cocos2d::ccMenuCallback& callback);
	static MouseOverMenuItem* createDecreaseButton(const cocos2d::ccMenuCallback& callback);
	static MouseOverMenuItem* creatMouseOverMenuButton(const std::string& normalPath, const std::string& selectedPath, const std::string& disabledPath, const cocos2d::ccMenuCallback& callback);
	static MouseOverMenuItem* creatMouseOverMenuButton(const cocos2d::ccMenuCallback& callback);
	static MouseOverMenuItem* create(const std::string& normalImage, const std::string& overImage, const std::string& disabledImage, const cocos2d::ccMenuCallback& callback);
	static MouseOverMenuItem* create(Node* normalSprite, Node* overSprite, Node* disabledSprite, const cocos2d::ccMenuCallback& callback);

	typedef std::function<void(MouseOverMenuItem*, cocos2d::Event*)> onMouseOver;
	onMouseOver m_OnMouseOver;
	onMouseOver m_OnMouseLeave;
	void setItemRect(cocos2d::Vec2& pos, float scale = 1.f, cocos2d::Vec2 parentPos = cocos2d::Vec2::ZERO);
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
