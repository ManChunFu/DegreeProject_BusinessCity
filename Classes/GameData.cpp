#include "GameData.h"
#include "Player.h"
#include "DataManager.h"
#include "Shop.h"
#include "ShopProduct.h"
#include "GlobalTime.h"
#include "UIPanel.h"
#include "cocostudio/SimpleAudioEngine.h"
using namespace CocosDenshion;


USING_NS_CC;

GameData::~GameData()
{
	reset(true, true);
	delete m_GlobalTime;
	m_GlobalTime = nullptr;
}

CocosDenshion::SimpleAudioEngine* GameData::GetAudio()
{
	if (m_Audio == nullptr)
		m_Audio = m_Audio = SimpleAudioEngine::getInstance();

	return m_Audio;
}

void GameData::init()
{
	m_Shops = DataManager::getShops();

	if (m_GlobalTime != nullptr)
	{
		delete m_GlobalTime;
		m_GlobalTime = nullptr;
	}

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

void GameData::reset(bool all, bool killAudio)
{
	if (all)
	{
		delete m_Player;
		m_Player = nullptr;
	}
	else
		m_Player->restart();

	for (auto item : m_Shops)
	{
		delete item.second;
	}
	m_Shops.clear();

	m_TempOpenPanel = nullptr;

	if (killAudio)
	{
		m_Audio->end();
		m_Audio = nullptr;
	}
	else
	{
		m_Audio->stopBackgroundMusic(true);
		m_Audio->stopAllEffects();
	}

}

