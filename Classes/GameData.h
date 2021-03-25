#pragma once
#include "cocos2d.h"

class Player;
class Shop;

class GameData
{
public:
	 static GameData& getInstance()
	 {
		 // The only instance
		 // Guaranteed to be lazy initialized
		 // Guaranteed that it will be destroyed correctly
		 static GameData instance;
		 return instance;
	 }
private:
	GameData() {};
	virtual ~GameData();

public:
	//GameData(GameData const&) = delete;
	//void operator=(GameData const&) = delete;
	GameData(const GameData& arg) = delete; // Copy constructor
	GameData(const GameData&& arg) = delete;  // Move constructor
	GameData& operator=(const GameData& arg) = delete; // Assignment operator
	GameData& operator=(const GameData&& arg) = delete; // Move operator

	void init();
	void registerCharacter(cocos2d::itemTypes type, std::string path);
	void setPlayer(const std::string& name, cocos2d::itemTypes type);
	std::string getPlayerCharacter(cocos2d::itemTypes playerCharacterType);

	void setShopProductQuantity(unsigned shopId, unsigned productId, unsigned quantity);

	Player* m_Player = nullptr;
	std::map<unsigned, Shop*> m_Shops;
	struct colorTypes
	{
		cocos2d::Color4B MediumOrchild = cocos2d::Color4B(186, 85, 211, 255);
		cocos2d::Color4B Orchid = cocos2d::Color4B(218, 112, 214, 255);
		cocos2d::Color4B DarkMagenta = cocos2d::Color4B(139, 0, 139, 255);
		cocos2d::Color4B MediumVioletRed = cocos2d::Color4B(199, 21, 133, 255);
		cocos2d::Color4B DeepPink = cocos2d::Color4B(255, 20, 147, 255);
		cocos2d::Color4B HotPink = cocos2d::Color4B(255, 105, 180, 255);
		cocos2d::Color4B PaleVioletRed = cocos2d::Color4B(219, 112, 147, 255);
		cocos2d::Color4B FireBrick = cocos2d::Color4B(178, 34, 34, 255);
		cocos2d::Color4B Crimson = cocos2d::Color4B(220, 20, 60, 255);
		cocos2d::Color4B LightCoral = cocos2d::Color4B(240, 128, 128, 255);
		cocos2d::Color4B LimeGreen = cocos2d::Color4B(50, 205, 50, 255);
		cocos2d::Color4B MediumSeaGreen = cocos2d::Color4B(0, 250, 154, 255);
		cocos2d::Color4B SeaGreen = cocos2d::Color4B(46, 139, 87, 255);
		cocos2d::Color4B YellowGreen = cocos2d::Color4B(154, 205, 50, 255);
		cocos2d::Color4B MediumAquamarine = cocos2d::Color4B(102, 205, 170, 255);
		cocos2d::Color4B LightSeaGreen = cocos2d::Color4B(32, 178, 170, 255);
		cocos2d::Color4B DarkCyan = cocos2d::Color4B(0, 139, 139, 255);
		cocos2d::Color4B LightSteelBlue = cocos2d::Color4B(176, 192, 222, 255);
		cocos2d::Color4B PowderBlue = cocos2d::Color4B(176, 224, 230, 255);
		cocos2d::Color4B Gold = cocos2d::Color4B(255, 215, 0, 255);
		cocos2d::Color4B Goldenrdo = cocos2d::Color4B(218, 165, 32, 255);
		cocos2d::Color4B DarkGoldenrod = cocos2d::Color4B(184, 134, 11, 255);
		cocos2d::Color4B Khaki = cocos2d::Color4B(240, 230, 140, 255);
		cocos2d::Color4B Taro = cocos2d::Color4B(125, 100, 100, 255);
	};
	colorTypes m_ColorType;
private:
	std::map<cocos2d::itemTypes, const std::string> m_CharacterSpriteMap;

};
