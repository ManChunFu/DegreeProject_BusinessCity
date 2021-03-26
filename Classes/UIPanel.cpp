#include "UIPanel.h"
#include "GameScene.h"

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
