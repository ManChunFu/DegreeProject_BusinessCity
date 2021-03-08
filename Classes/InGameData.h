#pragma once
#include "cocos2d.h"

USING_NS_CC;

extern std::string playerName;

extern itemTypes playerCharacter;

extern std::map<itemTypes, std::string> characterSpriteMap;

struct colorTypes
{
	Color4B MediumOrchild = Color4B(186, 85, 211, 255);
	Color4B Orchid = Color4B(218, 112, 214, 255);
	Color4B DarkMagenta = Color4B(139, 0, 139, 255);
	Color4B MediumVioletRed = Color4B(199, 21, 133, 255);
	Color4B DeepPink = Color4B(255, 20, 147, 255);
	Color4B HotPink = Color4B(255, 105, 180, 255);
	Color4B PaleVioletRed = Color4B(219, 112, 147, 255);
	Color4B FireBrick = Color4B(178, 34, 34, 255);
	Color4B Crimson = Color4B(220, 20, 60, 255);
	Color4B LightCoral = Color4B(240, 128, 128, 255);
	Color4B LimeGreen = Color4B(50, 205, 50, 255);
	Color4B MediumSeaGreen = Color4B(0, 250, 154, 255);
	Color4B SeaGreen = Color4B(46, 139, 87, 255);
	Color4B YellowGreen = Color4B(154, 205, 50, 255);
	Color4B MediumAquamarine = Color4B(102, 205, 170, 255);
	Color4B LightSeaGreen = Color4B(32, 178, 170, 255);
	Color4B DarkCyan = Color4B(0, 139, 139, 255);
	Color4B LightSteelBlue = Color4B(176, 192, 222, 255);
	Color4B PowderBlue = Color4B(176, 224, 230, 255);
};
extern colorTypes colorType;


