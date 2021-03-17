#include "GameData.h"
#include "Player.h"
#include "DataManager.h"
#include "Shop.h"


USING_NS_CC;

GameData::~GameData()
{
	delete m_Player;
	m_Player = nullptr;
	for (auto item : m_Shops)
	{
		delete item.second;
	}
	m_Shops.clear();
}

void GameData::init()
{
	m_Shops = DataManager::getShops();
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

