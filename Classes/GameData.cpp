#include "GameData.h"
#include "Player.h"
#include "DataManager.h"
#include "Shop.h"
#include "ShopProduct.h"
#include "GlobalTime.h"
#include "UIPanel.h"

USING_NS_CC;

GameData::~GameData()
{
	reset(true);
}

void GameData::init()
{
	m_Shops = DataManager::getShops();

	m_GlobalTime = new GlobalTime();
}

void GameData::registerCharacter(cocos2d::itemTypes type, std::string path)
{
	m_CharacterSpriteMap.insert(std::pair<itemTypes, std::string>(type, path));
}

void GameData::setPlayer(const std::string& name, cocos2d::itemTypes type)
{
	m_Player = new Player(name, type);
}

std::string GameData::getPlayerCharacter(cocos2d::itemTypes playerCharacterType)
{
	return std::string(m_CharacterSpriteMap[playerCharacterType]);
}

void GameData::setTempOpenPanel(UIPanel* panel)
{
	if (m_TempOpenPanel == nullptr)
	{
		m_TempOpenPanel = panel;
		return;
	}

	m_TempOpenPanel->closePanel();
	m_TempOpenPanel = panel;
}

void GameData::reset(bool all)
{
	if (all)
	{
		delete m_Player;
		m_Player = nullptr;
	}

	for (auto item : m_Shops)
	{
		delete item.second;
	}
	m_Shops.clear();

	delete m_GlobalTime;
	m_GlobalTime = nullptr;

	m_TempOpenPanel = nullptr;
}

