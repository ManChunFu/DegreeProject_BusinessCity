#pragma once

#include "cocos2d.h"
#include "Canvas.h"

class Player;

class InfoPanel : public Canvas
{
public:
	~InfoPanel() {};

	cocos2d::Sprite* m_InfoPanel = nullptr;
	void createPanel(cocos2d::Scene* scene, Player* player);
};
