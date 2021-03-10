#pragma once

#include "cocos2d.h"

class GameScene;

class Bank
{
public:
	~Bank();

	void openBankPanel(GameScene* scene);
	void closeBankPanel();

private:
	GameScene* m_GameScene = nullptr;
	cocos2d::Sprite* m_BankPanel = nullptr;
	void createBankPanel();


};