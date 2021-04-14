#include "UIPanel.h"
#include "GameScene.h"
#include "MouseOverMenuItem.h"

USING_NS_CC;


UIPanel::~UIPanel()
{
	if (m_GameScene)
	{
		if (m_Elements.size() > 0)
		{
			for (auto element : m_Elements)
			{
				m_GameScene->removeChild(element);
			}
		}
	}
	m_Elements.clear();
	m_ThisPanel = nullptr;
	m_GameScene = nullptr;
	m_Player = nullptr;
	if (m_MenuItems.size() > 0)
		m_MenuItems.clear();
}


MouseOverMenuItem* UIPanel::displayMenuButton(MouseOverMenuItem* button, std::function<void(MouseOverMenuItem* overItem, Event* event)> onMouseOver, 
	Vec2 pos, itemTypes type, float scale, bool addParentPos, cocos2d::Vec2 parentPos)
{
	button->m_OnMouseOver = onMouseOver;
	button->itemSelectedData.type = type;
	button->setScale(scale);
	button->setPosition(pos);
	if (addParentPos)
		button->setItemRect(pos, scale, (parentPos == Vec2::ZERO? m_ParentPosition : parentPos));

	return button;
}


