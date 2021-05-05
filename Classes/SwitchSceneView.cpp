#include "SwitchSceneView.h"
#include "GameFunctions.h"
#include "GameData.h"
#include "GameScene.h"
#include "MouseOverMenuItem.h"
#include "Player.h"
#include "Shop.h"
#include "EViews.h"
#include "People.h"
#include "Car.h"
#include "GameStartPanel.h"
#include "cocostudio/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

SwitchSceneView::~SwitchSceneView()
{
	m_SceneViewMaps.clear();
	m_BackMainButtons.clear();
	m_CurrentView.second = nullptr;
	m_PlayerShops.clear();

	delete m_People;
	m_People = nullptr;
	delete m_Car;
	m_Car = nullptr;

	m_Question = nullptr;
	m_MoneyIcon = nullptr;
	m_PlayerShopsInScene.clear();
	m_Audio = nullptr;
}

void SwitchSceneView::runInit(GameScene* gameScene, Size visibleSize, Vec2 origin, cocos2d::Vec2 sceneMidPoint)
{
	m_Player = GameData::getInstance().m_Player;
	createSceneViewMaps();
	m_GameScene = gameScene;

	m_VisibleSize = visibleSize;
	m_Origin = origin;
	m_SceneMidPoint = sceneMidPoint;

	GameFunctions::displaySprite(m_SceneViewMaps.at(EViews::E_Main), m_SceneMidPoint, m_GameScene, 0);
	setSpriteScale(m_SceneViewMaps.at(EViews::E_Main), Vec2::ONE);

	// instantiate
	m_People = new People(this, m_SceneMidPoint);

	// create hotel icon
	createMapIcon(m_MapIconPaths[0], m_MapIconPaths[1], m_MapIconPaths[2],
		Vec2(m_SceneMidPoint.x - 100.f, m_SceneMidPoint.y + 250.f), EViews::E_Hotel, 1.f, Vec2(0.f, -120.f));

	// create playground icon
	createMapIcon(m_MapIconPaths[0], m_MapIconPaths[1], m_MapIconPaths[1],
		Vec2(m_SceneMidPoint.x - 200.f, m_SceneMidPoint.y), EViews::E_Playground, 1.f, Vec2(50.f, -50.f));

	auto iconMenu = Menu::createWithArray(m_MenuItems);
	iconMenu->setPosition(Vec2::ZERO);
	m_SceneViewMaps.at(EViews::E_Main)->addChild(iconMenu, 1);

	createBackMainButton();

	// create car running in the main scene
	auto mainSceneMidPoint = Vec2(m_SceneViewMaps.at(EViews::E_Main)->getContentSize() * 0.5f);
	m_Car = new Car(this, mainSceneMidPoint);
	
	m_Audio = GameData::getInstance().m_Audio;
	m_Audio->playBackgroundMusic("Sounds/GameSceneBackground.mp3", true);
	m_Audio->setBackgroundMusicVolume(0.5f);

	m_CurrentView = std::make_pair(EViews::E_Main, m_SceneViewMaps.at(EViews::E_Main));

}

void SwitchSceneView::closePanel()
{
	onBackMainCallback(this);
}

void SwitchSceneView::displayShopInMainScene(unsigned shopId)
{
	m_PlayerShops[shopId] = GameData::getInstance().m_Shops[shopId];
	m_PlayerShops[shopId]->onSaleHappens = CC_CALLBACK_3(SwitchSceneView::onSaleHappens, this);

	auto playerShop = m_PlayerShops[shopId];
	auto shopSprite = Sprite::createWithSpriteFrameName(playerShop->m_ShopInSceneSmall);
	if (shopSprite)
		GameFunctions::displaySprite(shopSprite, playerShop->m_ShopMainSceneLocation, m_SceneViewMaps.at(EViews::E_Main), 1, 0.3f, 0.3f);

	m_MoneyIcon = Sprite::createWithSpriteFrameName("$.png");
	if (m_MoneyIcon)
	{
		GameFunctions::displaySprite(m_MoneyIcon, Vec2(playerShop->m_ShopMainSceneLocation.x, playerShop->m_ShopMainSceneLocation.y + 60.f),
			m_SceneViewMaps.at(EViews::E_Main), 1);
		m_MoneyIcon->setOpacity(0);
	}

	m_PlayerShopsInScene[shopId] = { shopSprite, m_MoneyIcon };
	auto shopSceneId = playerShop->m_ShopInSceneId;
	m_People->createPeopleList(shopSceneId, shopId);

	m_SceneShopMatchIds[shopSceneId] = shopId;

	// for extra shopping use
	m_StartupShopId = shopId;

	auto foundSceneId = m_SceneShopMatchIds.find(m_CurrentView.first);
	if (foundSceneId != m_SceneShopMatchIds.end())
		createShopInCloseSceneView(m_CurrentView.first);

}

void SwitchSceneView::removeShopFromScene(unsigned shopId, unsigned sceneId)
{
	//if (m_ShopSceneId == 0)
	//	return;

	// close shop scene
	if (m_CurrentView.first == sceneId)
		onBackMainCallback(this);
	
	// clean up people and product list
	m_People->detachFromParent(sceneId, true);

	// clean up shop scene
	auto children = m_SceneViewMaps.at(sceneId)->getChildren();
	for (auto item : children)
	{
		item->removeFromParent();
	}

	// remove from GameScene
	m_SceneViewMaps.at(sceneId)->removeFromParent();

	// clean main scene`
	for (auto item : m_PlayerShopsInScene.at(shopId))
	{
		item->removeFromParent();
	}

	m_PlayerShops.at(shopId) = nullptr;
}

void SwitchSceneView::removeQuestionAndChildPanel(unsigned sceneId)
{
	auto foundScene = m_SceneViewMaps.find(sceneId);
	if (foundScene != m_SceneViewMaps.end())
	{
		auto children = m_SceneViewMaps.at(sceneId)->getChildren();
		for (auto child : children)
		{
			child->removeFromParent();
		}
	}

}

void SwitchSceneView::switchView(unsigned id)
{
	if (!m_SceneViewMaps.at(id)->getParent())
		createOrderedView(id);

	m_CurrentView = std::make_pair(id, m_SceneViewMaps.at(id));

	fadeEffect(m_CurrentView.second, true);
	enableBackMainButtons(true);
}

void SwitchSceneView::clickIconCallback(cocos2d::Ref* pSender, unsigned viewId)
{
	if (m_StartupNotify)
	{
		m_StartupNotify();
		return;
	}

	for (auto item : m_MenuItems)
	{
		item->pause();
		item->setEnabled(false);
	}
	switchView(viewId);
}

void SwitchSceneView::onBackMainCallback(cocos2d::Ref* pSender)
{
	fadeEffect(m_CurrentView.second, false);
	for (auto item : m_MenuItems)
	{
		item->resume();
		item->setEnabled(true);
	}
	enableBackMainButtons(false);

	// clean up people list
	auto foundScene = m_SceneShopMatchIds.find(m_CurrentView.first);
	if (foundScene != m_SceneShopMatchIds.end())
		m_People->detachFromParent(m_CurrentView.first, false);

	m_CurrentView = std::make_pair(EViews::E_Main, m_SceneViewMaps.at(EViews::E_Main));
}

Sprite* SwitchSceneView::getSceneView(unsigned viewId)
{
	return m_SceneViewMaps.at(viewId);
}

void SwitchSceneView::onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
	if (menuItem->itemSelectedData.type == itemTypes::WIDGET_BUTTON)
		menuItem->setScale(1.f);

	if (m_Audio)
		m_Audio->playEffect("Sounds/SelectedSound.mp3", false, 1.f, 1.f, 1.f);

}

void SwitchSceneView::onMouseLeave(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
	menuItem->setScale(0.8f);
}

void SwitchSceneView::onSaleHappens(unsigned sceneId, unsigned shopId, unsigned productId)
{
	if (m_CurrentView.second == m_SceneViewMaps.at(EViews::E_Main))
	{
		auto textPos = m_MoneyIcon->getPosition();
		auto fadeIn = FadeIn::create(0.5f);
		auto moveFadeOut = Sequence::create(MoveTo::create(1.f, Vec2(textPos.x, textPos.y + 20.f)), FadeOut::create(0.5f), nullptr);
		auto moveBack = MoveTo::create(0.1f, Vec2(textPos.x, m_PlayerShops.at(shopId)->m_ShopMainSceneLocation.y + 60.f));
		auto sequence = Sequence::create(fadeIn, moveFadeOut, moveBack, nullptr);

		m_MoneyIcon->runAction(sequence);

		// play money sound
		if (m_Audio)
		{
			m_Audio->playEffect("Sounds/MoneyPop.mp3", false, 0.8f, 0.8f, 0.8f);
			m_Audio->setEffectsVolume(0.5f);
		}

		return;
	}

	if (m_CurrentView.first == sceneId)
	{
		if (m_SaleHappensNotify)
			m_SaleHappensNotify(sceneId, shopId, productId);
	}
}

void SwitchSceneView::shopOptionCallback(cocos2d::Ref* pSender, unsigned sceneId)
{
	if (m_OpenShopChoiceNotify)
	{
		m_OpenShopChoiceNotify(sceneId, m_StartupShopId);
		m_Question->setVisible(false);
	}
}

void SwitchSceneView::createOrderedView(unsigned id)
{
	auto newView = m_SceneViewMaps.at(id);
	if (newView)
	{
		GameFunctions::displaySprite(newView, m_SceneMidPoint, m_GameScene, 0);
		setSpriteScale(newView, Vec2::ONE);
	}

	auto foundSceneId = m_SceneShopMatchIds.find(id);
	if (foundSceneId != m_SceneShopMatchIds.end())
		createShopInCloseSceneView(id);
	else
		openShopChoice(id);

}

void SwitchSceneView::fadeEffect(Sprite* viewSprite, bool fadeIn)
{
	if (fadeIn)
	{
		viewSprite->setOpacity(0);
		auto spriteFadeIn = FadeIn::create(1.f);
		viewSprite->runAction(spriteFadeIn);

		if (viewSprite->getChildrenCount() > 0)
		{
			auto children = viewSprite->getChildren();
			for (auto item : children)
			{
				auto fade = FadeIn::create(1.f);
				item->runAction(fade);
			}
		}
		return;
	}

	auto spriteFadeOut = FadeOut::create(1.f);
	viewSprite->runAction(spriteFadeOut);

	if (viewSprite->getChildrenCount() > 0)
	{
		auto children = viewSprite->getChildren();
		for (auto item : children)
		{
			auto fade = FadeOut::create(1.f);
			item->runAction(fade);
		}
	}
}

void SwitchSceneView::openShopChoice(unsigned sceneId)
{
	m_Question = MouseOverMenuItem::creatMouseOverMenuButton("question100.png", "question100_Lit.png", "question100.png", 
		CC_CALLBACK_1(SwitchSceneView::shopOptionCallback, this, sceneId));
	if (m_Question)
	{
		displayMenuButton(m_Question, CC_CALLBACK_2(SwitchSceneView::onMouseOver, this), (sceneId == EViews::E_Hotel) ?
			Vec2(240, 290) : Vec2(550.f, 400.f), itemTypes::DEFAULT, 1.f, true, (sceneId == EViews::E_Hotel)? Vec2(100.f, -50.f) : 
			Vec2(25.f, -60.f));
		
		createIconSequence(m_Question, 10.f, 1.f);
	}
	
	auto questionMenu = Menu::create(m_Question, NULL);
	questionMenu->setPosition(Vec2::ZERO);
	m_SceneViewMaps.at(sceneId)->addChild(questionMenu, 1);
}

void SwitchSceneView::createMapIcon(const std::string& normal, const std::string& mouseOver, const std::string& disable, Vec2 displayPos, unsigned viewId, float iconScale, Vec2 parentPos)
{
	auto icon = MouseOverMenuItem::creatMouseOverMenuButton(normal, mouseOver, disable, CC_CALLBACK_1(
		SwitchSceneView::clickIconCallback, this, viewId));

	if (icon)
	{
		m_MenuItems.pushBack(displayMenuButton(icon, CC_CALLBACK_2(SwitchSceneView::onMouseOver, this), displayPos, itemTypes::BUTTON, iconScale,
			true, parentPos));

		createIconSequence(icon, 3.f, 1.f);
	}
}

void SwitchSceneView::createIconSequence(MouseOverMenuItem* icon, float moveDirection, float moveDuration)
{
	auto hoverPosition = icon->getPosition();
	auto moveUp = MoveTo::create(moveDuration, Vec2(hoverPosition.x, hoverPosition.y + moveDirection));
	auto moveDown = MoveTo::create(moveDuration, Vec2(hoverPosition.x, hoverPosition.y - moveDirection));
	auto sequence = Sequence::create(moveUp, moveDown, nullptr);
	icon->runAction(RepeatForever::create(sequence));
}

void SwitchSceneView::createShopInCloseSceneView(unsigned sceneId)
{
	auto playerIcon = Sprite::createWithSpriteFrameName(GameData::getInstance().getPlayerCharacter(m_Player->getCharacter()));
	auto shopIdMatchScene = m_SceneShopMatchIds.at(sceneId);
	auto shopSprite = Sprite::createWithSpriteFrameName(m_PlayerShops.at(shopIdMatchScene)->m_ShopInSceneBig);

	GameFunctions::displaySprite(playerIcon, m_PlayerShops.at(shopIdMatchScene)->m_EmployeeLocation, m_SceneViewMaps.at(sceneId), 1, 0.4f, 0.4f);
	GameFunctions::displaySprite(shopSprite, m_PlayerShops.at(shopIdMatchScene)->m_ShopSceneLocation, m_SceneViewMaps.at(sceneId), 1);
}

void SwitchSceneView::createSceneViewMaps()
{
	std::array<unsigned int, 3> viewKeys = { 0, 1, 2 };
	auto viewsCount = m_SceneViewPaths.size();

	for (unsigned index = 0; index < viewsCount; ++index)
	{
		auto viewSprite = Sprite::createWithSpriteFrameName(m_SceneViewPaths[index]);
		if (viewSprite)
			m_SceneViewMaps.insert(viewKeys[index], viewSprite);
	}
}

void SwitchSceneView::createBackMainButton()
{
	for (unsigned index = 0; index < 2; ++index)
	{
		auto backButton = (index == 0) ? MouseOverMenuItem::creatMouseOverMenuButton("BackButton_Normal.png", "BackButton_Lit.png",
			"BackButton_Disable.png", CC_CALLBACK_1(SwitchSceneView::onBackMainCallback, this)) : MouseOverMenuItem::creatMouseOverMenuButton(
				"MapMain.png", "MapMain_Lit.png", "MapMain.png", CC_CALLBACK_1(SwitchSceneView::onBackMainCallback, this));

		if (backButton)
		{
			if (index == 0)
			{
				m_BackMainButtons.pushBack(displayMenuButton(backButton, CC_CALLBACK_2(SwitchSceneView::onMouseOver, this),
					Vec2(m_SceneMidPoint.x - 420.f, m_SceneMidPoint.y + 230.f), itemTypes::BUTTON, 1, true, Vec2(5.f, 0.f)));
			}
			else
			{
				m_BackMainButtons.pushBack(displayMenuButton(backButton, CC_CALLBACK_2(SwitchSceneView::onMouseOver, this),
					Vec2(m_SceneMidPoint.x + 390.f, m_SceneMidPoint.y - 315.f), itemTypes::WIDGET_BUTTON, 0.8f, true, Vec2(5.f, 0.f)));

				backButton->m_OnMouseLeave = CC_CALLBACK_2(SwitchSceneView::onMouseLeave, this);
			}

			backButton->setVisible(false);
		}
	}

	auto backMainMenu = Menu::createWithArray(m_BackMainButtons);
	backMainMenu->setPosition(Vec2::ZERO);
	m_GameScene->addChild(backMainMenu, 2);
	m_Elements.pushBack(backMainMenu);
}

void SwitchSceneView::enableBackMainButtons(bool enable)
{
	for (auto button : m_BackMainButtons)
	{
		button->setVisible(enable);
	}
}

void SwitchSceneView::setSpriteScale(Sprite* sprite, Vec2 scale)
{
	sprite->setScaleX((m_VisibleSize.width / sprite->getContentSize().width) * scale.x);
	sprite->setScaleY((m_VisibleSize.height / sprite->getContentSize().height) * scale.y);
}



