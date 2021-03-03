#include "MouseOverMenuItem.h"

MouseOverMenuItem::~MouseOverMenuItem()
{
	isHovering = false;
	_eventDispatcher->removeEventListener(mouseListener);
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

void MouseOverMenuItem::setItemRect(Vec2& pos)
{
	float x = pos.x - getContentSize().width / 2;
	float y = pos.y - getContentSize().height / 2;
	itemRect = Rect(x, y, getContentSize().width, getContentSize().height);
}


void MouseOverMenuItem::onMouseMove(Event* event)
{
	if (!this->isEnabled())
		return;

	EventMouse* eventMouse = (EventMouse*)event;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 locationInNode = Point(Vec2(eventMouse->getCursorX(), eventMouse->getCursorY()));

	// Show the selected image if mouse over and emit the event
	if (itemRect.containsPoint(locationInNode))
	{
		selected();
		if (onMouseOver && !isHovering)
		{
			isHovering = true;
			onMouseOver(this, event);
		}
	}
	else
	{
		isHovering = false;
		unselected();
	}
}

void MouseOverMenuItem::setMouseListener()
{
	// Create a mouse listener
	mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(MouseOverMenuItem::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

}
