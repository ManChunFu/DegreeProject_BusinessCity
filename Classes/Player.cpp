#include "Player.h"
#include "Shop.h"

Player::~Player()
{
	reset();
}

const std::string& Player::getName()
{
	return m_PlayerName;
}

cocos2d::itemTypes Player::getCharacter()
{
	return m_PlayerCharacter;
}


void Player::updateCurrentCashAmout(int cashAmout)
{
	m_CurrentCashAmout += cashAmout;

	if (onCashAmoutChange)
		onCashAmoutChange(this, m_CurrentCashAmout);
}

void Player::reset()
{
	m_MyShopIds.clear();
}
