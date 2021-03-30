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
	m_Player = GameData::getInstance().m_Player;
}

GameLoop::~GameLoop()
{
	m_Player = nullptr;
}

void GameLoop::onEveryMinuteChanges(GlobalTime* globalTime, unsigned minute)
{
	auto myShop = GameData::getInstance().m_Shops[m_Player->m_MyShopIds[0]];
	auto gameTime = GameData::getInstance().m_GlobalTime->m_Gametime;

	if (isShopOpen(myShop->m_ShopId, gameTime->day, gameTime->hour))
	{
		auto tradeQTY = runTradeQuantity(gameTime->day);
		if (tradeQTY == 0)
			return;

		executeTrade(tradeQTY);
	}
}

bool GameLoop::isShopOpen(unsigned shopId, unsigned day, unsigned currentHour)
{
	auto shop = GameData::getInstance().m_Shops[shopId];

	if (currentHour < shop->m_ShopOpenHour[0] || currentHour > shop->m_ShopOpenHour[1])
		return false;

	return shop->m_ShopOpenDay[day];
		
}

unsigned GameLoop::getSucessProbability(unsigned day)
{
	return m_SuccessProbabilityDaily[day];
}

unsigned GameLoop::runTradeQuantity(unsigned day)
{
	auto randNo = random(0, 100);
	
	if (randNo < getSucessProbability(day))
		return random(0, m_MaxTradePerPerson);

	return 0;
}

void GameLoop::executeTrade(unsigned qauntity)
{
}
