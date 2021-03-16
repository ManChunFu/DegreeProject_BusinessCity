#include "Player.h"
#include "Shop.h"

Player::~Player()
{
	m_MyShopIds.clear();
}

const std::string& Player::getName()
{
	return m_PlayerName;
}

cocos2d::itemTypes Player::getCharacter()
{
	return m_PlayerCharacter;
}

void Player::updateDebt(unsigned debtAmout)
{
	m_Debut += debtAmout;
	Player::updateCurrentCashAmout(debtAmout);
}

void Player::updateCurrentCashAmout(int cashAmout)
{
	m_CurrentCashAmout += cashAmout;

	if (onCashAmoutChange)
		onCashAmoutChange(this, m_CurrentCashAmout);
}
