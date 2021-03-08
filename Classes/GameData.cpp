#include "GameData.h"

USING_NS_CC;

void GameData::registerCharacter(cocos2d::itemTypes type, std::string path)
{
	m_CharacterSpriteMap.insert(std::pair<itemTypes, std::string>(type, path));
}

std::string GameData::getPlayerCharacter()
{
	return std::string(m_CharacterSpriteMap[m_PlayerCharacter]);
}
