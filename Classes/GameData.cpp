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

unsigned GameData::getProductQuantity(unsigned shopId, unsigned productId)
{
	return m_Shops[shopId]->m_Products[productId]->m_Quantity;
}

void GameData::setShopProductQuantity(unsigned shopId, unsigned productId, unsigned quantity)
{
	m_Shops[shopId]->m_Products[productId]->m_Quantity = quantity;
}

unsigned GameData::getProductPurchasePrice(unsigned shopId, unsigned productId)
{
	return m_Shops[shopId]->m_Products[productId]->m_PurchasePrice;
}

void GameData::setShopOpenDay(unsigned shopId, unsigned weekday)
{
	auto originSetting = m_Shops[shopId]->m_ShopOpenDay[weekday];
	m_Shops[shopId]->m_ShopOpenDay[weekday] = !originSetting;
}

void GameData::setPlayerWorkHere(unsigned shopId)
{
	auto originSetting = m_Shops[shopId]->m_PlayerWorkHere;
	m_Shops[shopId]->m_PlayerWorkHere = !originSetting;
}

void GameData::setShopOpenHour(unsigned shopId, unsigned fromOrTo, unsigned workingHour)
{
	m_Shops[shopId]->m_ShopOpenHour[fromOrTo] = workingHour;
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

	delete m_TempOpenPanel;
	m_TempOpenPanel = nullptr;
}

