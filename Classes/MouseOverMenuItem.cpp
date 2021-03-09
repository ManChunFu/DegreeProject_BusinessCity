#include "MouseOverMenuItem.h"

MouseOverMenuItem::~MouseOverMenuItem()
{
	m_IsHovering = false;
	_eventDispatcher->removeEventListener(m_MouseListener);
}

MouseOverMenuItem* MouseOverMenuItem::create(const std::string& normalImage, const std::string& overImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
	MouseOverMenuItem* mouseOver = new(std::nothrow) MouseOverMenuItem();

	if (mouseOver && mouseOver->initWithNormalImage(normalImage, overImage, disabledImage, callback));
	{
		mouseOver->setMouseListener();
		mouseOver->autorelease();

		return mouseOver;
	}
	CC_SAFE_DELETE(mouseOver);
	return nullptr;
}

MouseOverMenuItem* MouseOverMenuItem::create(Node* normalSprite, Node* overSprite, Node* disabledSprite, const ccMenuCallback& callback)
{
	MouseOverMenuItem* mouseOver = new(std::nothrow) MouseOverMenuItem();

	if (mouseOver && mouseOver->initWithNormalSprite(normalSprite, overSprite, disabledSprite, callback));
	{
		mouseOver->setMouseListener();
		mouseOver->autorelease();

		return mouseOver;
	}
	CC_SAFE_DELETE(mouseOver);
	return nullptr;
}

void MouseOverMenuItem::setItemRect(Vec2& pos, float scale)
{
	float x = pos.x - getContentSize().width * 0.5f * scale;
	float y = pos.y - getContentSize().height * 0.5f * scale;
	m_ItemRect = Rect(x, y, getContentSize().width * scale, getContentSize().height * scale);
}

void MouseOverMenuItem::onMouseMove(Event* event)
{
	if (!this->isEnabled())
		return;

	EventMouse* eventMouse = (EventMouse*)event;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 locationInNode = Point(Vec2(eventMouse->getCursorX(), eventMouse->getCursorY()));

	// Show the selected image if mouse over and emit the event
	if (m_ItemRect.containsPoint(locationInNode))
	{
		selected();
		if (onMouseOver && !m_IsHovering)
		{
			m_IsHovering = true;
			onMouseOver(this, event);
		}
	}
	else
	{
		m_IsHovering = false;
		unselected();
	}
}

void MouseOverMenuItem::setMouseListener()
{
	// Create a mouse listener
	m_MouseListener = EventListenerMouse::create();
	m_MouseListener->onMouseMove = CC_CALLBACK_1(MouseOverMenuItem::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_MouseListener, this);

}
