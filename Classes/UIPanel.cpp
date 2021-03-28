#include "UIPanel.h"
#include "GameScene.h"
#include "MouseOverMenuItem.h"

USING_NS_CC;

UIPanel::~UIPanel()
{
	if (m_GameScene)
	{
		for (auto element : m_Elements)
		{
			m_GameScene->removeChild(element);
		}
		m_Elements.clear();
		m_ThisPanel = nullptr;
		m_GameScene = nullptr;
		m_Player = nullptr;

		for (auto item : m_MenuItems)
		{
			delete item;
		}
		m_MenuItems.clear();
	}
}

MouseOverMenuItem* UIPanel::displayMenuButton(MouseOverMenuItem* button, std::function<void(MouseOverMenuItem* overItem, Event* event)> onMouseOver, Vec2 pos, itemTypes type, float scale)
{
	button->onMouseOver = onMouseOver;
	button->itemSelectedData.type = type;
	button->setScale(scale);
	button->setPosition(pos);
	button->setItemRect(pos, scale);

	return button;
}


