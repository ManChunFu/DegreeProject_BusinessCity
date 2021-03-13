#include "Player.h"
#include "Shop.h"

const std::string& Player::getName()
{
	return m_PlayerName;
}

cocos2d::itemTypes Player::getCharacter()
{
	return m_PlayerCharacter;
}
