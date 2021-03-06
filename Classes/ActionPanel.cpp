#include "ActionPanel.h"
#include "GameData.h"
#include "GameFunctions.h"
#include "Player.h"
#include "MouseOverMenuItem.h"
#include "GameScene.h"
#include "Shop.h"
#include "MyShopSettingPanel.h"
#include "BuyShopChoicePanel.h"
#include "SwitchSceneView.h"
#include "cocostudio/SimpleAudioEngine.h"
using namespace CocosDenshion;


USING_NS_CC;

ActionPanel::~ActionPanel()
{
	m_MainScene = nullptr;
	m_ShopButton = nullptr;
	m_ShopChoicePanel = nullptr;
	m_MyShopMap.clear();
	m_TempEmployeeIds.clear();
}

void ActionPanel::openPanel(GameScene* scene, cocos2d::Vec2 sceneMidPoint)
{
	m_GameScene = scene;
	m_SceneMidPoint = sceneMidPoint;
	m_Player = GameData::getInstance().m_Player;

	m_ThisPanel = Sprite::createWithSpriteFrameName("InGamePanel_Black_80.png");
	if (!m_ThisPanel)
		return;

	m_ThisPanel->setPosition(Vec2(m_SceneMidPoint.x - 160.f, m_SceneMidPoint.y - 320.f));
	m_GameScene->addChild(m_ThisPanel, 1);
	m_Elements.pushBack(m_ThisPanel);

	m_DisplayShopPos = Vec2((m_ThisPanel->getContentSize().width * 0.5f) - 240.f, m_ThisPanel->getContentSize().height * 0.5f);

	if (m_MainScene)
		m_MainScene->m_OpenShopChoiceNotify = CC_CALLBACK_2(ActionPanel::openShopChoiceNotify, this);
}

void ActionPanel::displayShop(unsigned shopId, Vec2 shopPosition)
{
	auto myShop = GameData::getInstance().m_Shops[shopId];
	auto myShopButton = MouseOverMenuItem::creatMouseOverMenuButton(myShop->m_ShopLook_Normal, myShop->m_ShopLook_Lit, myShop->m_ShopLook_Disabled,
		CC_CALLBACK_1(ActionPanel::openShopCallback, this, shopId));
	
	if (myShopButton)
		displayMenuButton(myShopButton, CC_CALLBACK_2(ActionPanel::onMouseOver, this), (shopPosition == Vec2::ZERO)? 
			m_DisplayShopPos : shopPosition, itemTypes::DEFAULT, 0.3f, true, Vec2(-240.f, 0.f));

	auto menu = Menu::create(myShopButton, NULL);
	menu->setPosition(Vec2::ZERO);
	m_ThisPanel->addChild(menu, 2);

	auto shopButton = new MyShopSettingPanel();
	shopButton->autorelease();
	m_ThisPanel->addChild(shopButton, 1);
	m_MyShopMap[shopId] = shopButton;
	shopButton->onShopChanges = CC_CALLBACK_1(ActionPanel::onShopChanges, this, menu, myShopButton->getPosition());

	// create shop and display in scene
	m_MainScene->displayShopInMainScene(shopId);
}

void ActionPanel::openShopCallback(cocos2d::Ref* pSender, unsigned shopId)
{
	if (GameData::getInstance().isPopupOpen())
		return;

	if (m_MyShopMap.empty())
		return;

	auto foundShop = m_MyShopMap[shopId];
	if (foundShop)
	{
		if (foundShop->isPanelOpen())
			foundShop->closePanel();
		else
		{
			foundShop->openPanel(m_GameScene, m_SceneMidPoint, shopId);
			m_CurrentOpenShopId = shopId;
		}
	}
}

void ActionPanel::onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event)
{
	GameData::getInstance().m_Audio->playEffect("Sounds/SelectedSound.mp3", false, 1.f, 1.f, 1.f);
}

void ActionPanel::onShopChanges(unsigned shopId, Node* menu, Vec2 shopPos)
{
	// create a temp shop pic
	auto shopUpgradeSprite = Sprite::createWithSpriteFrameName(GameData::getInstance().m_Shops[shopId]->m_ShopLook_Normal);
	if (shopUpgradeSprite)
		GameFunctions::displaySprite(shopUpgradeSprite, shopPos, m_GameScene, 2, 0.3f, 0.3f);

	// close the current one
	auto foundShop = m_MyShopMap[m_CurrentOpenShopId];
	if (foundShop)
		foundShop->closePanel();

	// remove from the scene
	menu->removeFromParent();
	
	// delete old shop
	removeShop(m_CurrentOpenShopId);

	// add and create the upgrade shop
	m_Player->m_MyShopIds.push_back(shopId);
	if (!m_TempEmployeeIds.empty())
		GameData::getInstance().m_Shops.at(shopId)->m_EmployeesIds = m_TempEmployeeIds;
	displayShop(shopId, shopPos);

	// kill the temp shop pic
	m_GameScene->removeChild(shopUpgradeSprite);
	delete shopUpgradeSprite;

	//ToDo:: fix shop upgrade bank display
	//if (m_OnUpgradeShopCalls)
	//	m_OnUpgradeShopCalls(m_CurrentOpenShopId, shopId);
}

void ActionPanel::openShopChoiceNotify(unsigned sceneId, unsigned startupId)
{
	auto parent = m_MainScene->getSceneView(sceneId);
	if (m_ShopChoicePanel != nullptr && m_ShopChoicePanel->getParent() == parent)
	{
		if (!m_ShopChoicePanel->isPanelOpen())
			m_ShopChoicePanel->openPanel(m_MainScene, m_SceneMidPoint, sceneId, startupId);
		else
			m_ShopChoicePanel->closePanel();
	}
	else
	{
		m_ShopChoicePanel = new BuyShopChoicePanel();
		m_ShopChoicePanel->autorelease();
		m_ThisPanel->addChild(m_ShopChoicePanel, 2);
		m_ShopChoicePanel->m_OnPlayerPurchase = CC_CALLBACK_2(ActionPanel::onPlayerPurchase, this);
		m_ShopChoicePanel->openPanel(m_MainScene, m_SceneMidPoint, sceneId, startupId);
	}
}

void ActionPanel::onPlayerPurchase(unsigned shopId, unsigned sceneId)
{
	// remove question and choice
	m_MainScene->removeQuestionAndChildPanel(sceneId);
	m_ShopChoicePanel->removeFromParent();
	
	displayShop(shopId, Vec2(m_DisplayShopPos.x += 120, m_DisplayShopPos.y));
}

void ActionPanel::removeShop(unsigned shopId)
{
	m_MyShopMap[shopId]->removeFromParent();
	m_MyShopMap.erase(shopId);

	m_TempEmployeeIds = GameData::getInstance().m_Shops.at(shopId)->m_EmployeesIds;

	m_Player->removeShopId(shopId);
	auto sceneId = GameData::getInstance().m_Shops.at(shopId)->m_ShopInSceneId;
	m_MainScene->removeShopFromScene(shopId, sceneId);
}





