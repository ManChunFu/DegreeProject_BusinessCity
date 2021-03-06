#include "Player.h"
#include "Shop.h"

Player::~Player()
{
	reset();
}

void Player::removeShopId(unsigned shopId)
{
	for (unsigned index = 0; index < m_MyShopIds.size(); ++index)
	{
		if (m_MyShopIds[index] == shopId)
			m_MyShopIds.erase(m_MyShopIds.begin() + index);
	}
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

void Player::restart()
{
	reset();
	m_CurrentCashAmout = 50000;
}

void Player::reset()
{
	m_MyShopIds.clear();
}
