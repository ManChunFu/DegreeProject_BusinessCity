#pragma once

#include "cocos2d.h"

class Shop;

class Player
{
public:
	Player(const std::string& name, cocos2d::itemTypes character) : m_PlayerName(name), m_PlayerCharacter(character){};
	virtual ~Player();

	const std::string& getName();
	cocos2d::itemTypes getCharacter();
	void updateDebt(unsigned debtAmout);
	void updateCurrentCashAmout(int cashAmout);
	
	std::function<void(Player* player, int cashAmout)> onCashAmoutChange;
private:
	const std::string& m_PlayerName = "";
	cocos2d::itemTypes m_PlayerCharacter;
	int m_CurrentCashAmout = 50000;
	unsigned m_Debut = 0;
	std::vector<unsigned> m_MyShopIds;
};
