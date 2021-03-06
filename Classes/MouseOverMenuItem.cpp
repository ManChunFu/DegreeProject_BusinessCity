#include "MouseOverMenuItem.h"
#include "base/CCEventMouse.h"
USING_NS_CC;
MouseOverMenuItem::~MouseOverMenuItem()
{
	m_IsHovering = false;
	_eventDispatcher->removeEventListener(m_MouseListener);
}

MouseOverMenuItem* MouseOverMenuItem::createUpperButton(const cocos2d::ccMenuCallback& callback)
{
	auto spriteNormal = Sprite::createWithSpriteFrameName("UIButtonCorner40Left.png");
	auto spriteSelected = Sprite::createWithSpriteFrameName("UIButtonCorner40Left_Lit.png");
	auto spriteDisabled = Sprite::createWithSpriteFrameName("UIButtonCorner40Left_Disabled.png");

	if (spriteNormal && spriteSelected && spriteDisabled)
		return create(spriteNormal, spriteSelected, spriteDisabled, callback);
	return nullptr;
}

MouseOverMenuItem* MouseOverMenuItem::createLowerButton(const cocos2d::ccMenuCallback& callback)
{
	auto spriteNormal = Sprite::createWithSpriteFrameName("UIButtonCorner40.png");
	auto spriteSelected = Sprite::createWithSpriteFrameName("UIButtonCorner40_Lit.png");
	auto spriteDisabled = Sprite::createWithSpriteFrameName("UIButtonCorner40_Disabled.png");

	if (spriteNormal && spriteSelected && spriteDisabled)
		return create(spriteNormal, spriteSelected, spriteDisabled, callback);
	return nullptr;
}

MouseOverMenuItem* MouseOverMenuItem::createIncreaseButton(const cocos2d::ccMenuCallback& callback)
{
	auto spriteNormal = Sprite::createWithSpriteFrameName("UIButtonLess50.png");
	auto spriteSelected = Sprite::createWithSpriteFrameName("UIButtonLess50_Lit.png");
	auto spriteDisabled = Sprite::createWithSpriteFrameName("UIButtonLess50_Disable.png");

	if (spriteNormal && spriteSelected && spriteDisabled)
		return create(spriteNormal, spriteSelected, spriteDisabled, callback);
	return nullptr;
}

MouseOverMenuItem* MouseOverMenuItem::createDecreaseButton(const cocos2d::ccMenuCallback& callback)
{
	auto spriteNormal = Sprite::createWithSpriteFrameName("UIButtonMore50.png");
	auto spriteSelected = Sprite::createWithSpriteFrameName("UIButtonMore50_Lit.png");
	auto spriteDisabled = Sprite::createWithSpriteFrameName("UIButtonMore50_Disable.png");

	if (spriteNormal && spriteSelected && spriteDisabled)
		return create(spriteNormal, spriteSelected, spriteDisabled, callback);
	return nullptr;
}

MouseOverMenuItem* MouseOverMenuItem::creatMouseOverMenuButton(const std::string& normalPath, const std::string& selectedPath, const std::string& disabledPath, const ccMenuCallback& callback)
{
	auto spriteNormal = Sprite::createWithSpriteFrameName(normalPath);
	auto spriteSelected = Sprite::createWithSpriteFrameName(selectedPath);
	auto spriteDisabled = Sprite::createWithSpriteFrameName(disabledPath);

	if (spriteNormal && spriteSelected && spriteDisabled)
		return create(spriteNormal, spriteSelected, spriteDisabled, callback);

	return nullptr;
}

MouseOverMenuItem* MouseOverMenuItem::creatMouseOverMenuButton(const cocos2d::ccMenuCallback& callback)
{
	auto spriteNormal = Sprite::createWithSpriteFrameName("ButtonBlueNormal.png");
	auto spriteSelected = Sprite::createWithSpriteFrameName("ButtonBlueLit.png");
	auto spriteDisabled = Sprite::createWithSpriteFrameName("ButtonBlueDisabled.png");

	if (spriteNormal && spriteSelected && spriteDisabled)
		return create(spriteNormal, spriteSelected, spriteDisabled, callback);

	return nullptr;
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

void MouseOverMenuItem::setItemRect(Vec2& pos, float scale, Vec2 parentPos)
{
	float x = pos.x - getContentSize().width * 0.5f * scale;
	float y = pos.y - getContentSize().height * 0.5f * scale;
	if (parentPos != Vec2::ZERO)
	{
		x += parentPos.x;
		y += parentPos.y;
	}
	m_ItemRect = Rect(x, y, getContentSize().width * scale, getContentSize().height * scale);

}

void MouseOverMenuItem::onMouseMove(Event* event)
{
	if (!this->isEnabled())
		return;

	EventMouse* eventMouse = (EventMouse*)event;
	Vec2 locationInNode = Point(Vec2(eventMouse->getCursorX(), eventMouse->getCursorY()));

	// Show the selected image if mouse over and emit the event
	if (m_ItemRect.containsPoint(locationInNode))
	{
		selected();
		if (m_OnMouseOver && !m_IsHovering)
		{
			m_IsHovering = true;
			m_OnMouseOver(this, event);
		}
		return;
	}

	unselected();
	if (m_IsHovering && m_OnMouseLeave)
		m_OnMouseLeave(this, event);

	m_IsHovering = false;
}

void MouseOverMenuItem::setMouseListener()
{
	// Create a mouse listener
	m_MouseListener = EventListenerMouse::create();
	m_MouseListener->onMouseMove = CC_CALLBACK_1(MouseOverMenuItem::onMouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_MouseListener, this);

}
