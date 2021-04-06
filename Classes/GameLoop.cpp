#include "GameLoop.h"
#include "GameData.h"
#include "GlobalTime.h"
#include "GameTime.h"
#include "Shop.h"
#include "ShopProduct.h"
#include "Player.h"


USING_NS_CC;


GameLoop::GameLoop()
{
	GameData::getInstance().m_GlobalTime->addMinuteEventListener(CC_CALLBACK_2(GameLoop::onEveryMinuteChanges, this));
	GameData::getInstance().m_GlobalTime->addHourEventListener(CC_CALLBACK_2(GameLoop::onEveryHourChanges, this));
	m_Player = GameData::getInstance().m_Player;
}

GameLoop::~GameLoop()
{
	m_Player = nullptr;
}

void GameLoop::onEveryMinuteChanges(GlobalTime* globalTime, unsigned minute)
{
	if (m_Player->m_MyShopIds.size() < 1)
		return;

	auto gameTime = GameData::getInstance().m_GlobalTime->m_Gametime;
	for (unsigned shopIndex = 0; shopIndex < m_Player->m_MyShopIds.size(); shopIndex++)
	{
		auto myShop = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[shopIndex]];
		if (myShop->isShopOpen(gameTime->day, gameTime->hour))
		{
			auto salesIncome = myShop->runTrade(gameTime->day, myShop);
			m_Player->updateCurrentCashAmout(salesIncome);
		}
	}
}

void GameLoop::onEveryHourChanges(GlobalTime* globalTime, unsigned hour)
{
	
}


