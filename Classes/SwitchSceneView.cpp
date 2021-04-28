#include "SwitchSceneView.h"
#include "GameFunctions.h"
#include "GameData.h"
#include "GameScene.h"
#include "MouseOverMenuItem.h"
#include "Player.h"
#include "Shop.h"
#include "EViews.h"
#include "People.h"

USING_NS_CC;

SwitchSceneView::~SwitchSceneView()
{
	m_SceneViewMaps.clear();
	m_BackMainButtons.clear();
	m_CurrentView = nullptr;
	m_PlayerShop = nullptr;
	m_People = nullptr;
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
	createMapIcon(m_MapIcons[0], m_MapIcons[1], m_MapIcons[2], Vec2(m_SceneMidPoint.x - 100.f, m_SceneMidPoint.y + 250.f),
		EViews::E_Hotel, 1.f, Vec2(0.f, -120.f));

	createBackMainButton();

	// TODO : move to its own class
	auto mainSceneMidPoint = Vec2(m_SceneViewMaps.at(EViews::E_Main)->getContentSize() * 0.5f);
	auto car_Left = Sprite::createWithSpriteFrameName("Car_Left.png");
	if (car_Left)
	{
		GameFunctions::displaySprite(car_Left, Vec2(mainSceneMidPoint.x, mainSceneMidPoint.y - 255.f), m_SceneViewMaps.at(EViews::E_Main), 1.f,
			0.7f, 0.7f);
		auto moveLeft = MoveTo::create(7, Vec2(car_Left->getPosition().x - 600.f, car_Left->getPosition().y));
		auto sequence1 = Sequence::create(moveLeft, nullptr);
		car_Left->runAction(sequence1);
	}

	auto car_Right = Sprite::createWithSpriteFrameName("Car_Right.png");
	if (car_Right)
	{
		GameFunctions::displaySprite(car_Right, Vec2(mainSceneMidPoint.x - 600.f, mainSceneMidPoint.y - 285.f), m_SceneViewMaps.at(EViews::E_Main),
			1.f, 0.7f, 0.7f);
		auto moveRight = MoveTo::create(7, Vec2(car_Right->getPosition().x + 400.f, car_Right->getPosition().y));
		auto sequence2 = Sequence::create(moveRight, nullptr);
		car_Right->runAction(sequence2);
	}
}

void SwitchSceneView::switchView(unsigned id)
{
	if (!m_SceneViewMaps.at(id)->getParent())
		createOrderedView(id);

	m_CurrentView = m_SceneViewMaps.at(id);
	fadeEffect(m_CurrentView, true);
	enableBackMainButtons(true);
}

void SwitchSceneView::displayShopInMainScene(unsigned shopId)
{
	m_PlayerShop = GameData::getInstance().m_Shops[shopId];
	m_PlayerShop->onSaleHappens = CC_CALLBACK_3(SwitchSceneView::onSaleHappens, this);

	auto shopSprite = Sprite::createWithSpriteFrameName(m_PlayerShop->m_ShopInSceneSmall);
	if (shopSprite)
		GameFunctions::displaySprite(shopSprite, Vec2(m_SceneMidPoint.x - 200.f, m_SceneMidPoint.y - 80.f),
			m_SceneViewMaps.at(EViews::E_Main), 1, 0.3f, 0.3f);

	createMapIcon(m_MapIcons[0], m_MapIcons[1], m_MapIcons[2], Vec2(m_SceneMidPoint.x - 200.f, m_SceneMidPoint.y - 40.f),
		EViews::E_Playground, 1.f, Vec2(50.f, -50.f));
}

Sprite* SwitchSceneView::getSceneView(unsigned viewId)
{
	return m_SceneViewMaps.at(viewId);
}

void SwitchSceneView::clickIconCallback(cocos2d::Ref* pSender, unsigned viewId)
{
	m_MenuItems.at(EViews::E_Main)->pause();
	m_MenuItems.at(EViews::E_Main)->setEnabled(false);
	switchView(viewId);
}

void SwitchSceneView::onBackMainCallback(cocos2d::Ref* pSender)
{
	fadeEffect(m_CurrentView, false); 
	m_MenuItems.at(EViews::E_Main)->resume();
	m_MenuItems.at(EViews::E_Main)->setEnabled(true);
	enableBackMainButtons(false);
	m_CurrentView = m_SceneViewMaps.at(EViews::E_Main);
	
	m_People->detachFromParent();
}

void SwitchSceneView::onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
	if (menuItem->itemSelectedData.type == itemTypes::WIDGET_BUTTON)
		menuItem->setScale(1.f);
}

void SwitchSceneView::onMouseLeave(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
	menuItem->setScale(0.8f);
}

void SwitchSceneView::onSaleHappens(unsigned shopId, unsigned productId, unsigned saleQuantity)
{
	if (m_CurrentView == m_SceneViewMaps.at(EViews::E_Playground))
	{
		if (m_SaleHappensNotify)
			m_SaleHappensNotify(shopId, productId, saleQuantity);
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
	if (id == EViews::E_Playground)
	{
		auto playerIcon = Sprite::createWithSpriteFrameName(GameData::getInstance().getPlayerCharacter(m_Player->getCharacter()));
		if (playerIcon)
			GameFunctions::displaySprite(playerIcon, Vec2(m_SceneMidPoint.x - 140.f, m_SceneMidPoint.y + 20.f), 
				m_SceneViewMaps.at(EViews::E_Playground), 1.f, 0.4f, 0.4f);

		auto shopSprite = Sprite::createWithSpriteFrameName(m_PlayerShop->m_ShopInSceneBig);
		if (shopSprite)
			GameFunctions::displaySprite(shopSprite, Vec2(m_SceneMidPoint.x - 200.f, m_SceneMidPoint.y - 60.f),
				m_SceneViewMaps.at(EViews::E_Playground), 1);
	}
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

void SwitchSceneView::createMapIcon(const std::string& normal, const std::string& mouseOver, const std::string& disable, Vec2 displayPos, unsigned viewId, float iconScale, Vec2 parentPos)
{
	auto icon = MouseOverMenuItem::creatMouseOverMenuButton(normal, mouseOver, disable, CC_CALLBACK_1(
		SwitchSceneView::clickIconCallback, this, viewId));

	if (icon)
	{
		m_MenuItems.pushBack(displayMenuButton(icon, CC_CALLBACK_2(SwitchSceneView::onMouseOver, this), displayPos, itemTypes::BUTTON, iconScale,
			true, parentPos));

		auto hoverPosition = icon->getPosition();
		auto moveUp = MoveTo::create(m_MovingDuration, Vec2(hoverPosition.x, hoverPosition.y + m_MoveUp));
		auto moveDown = MoveTo::create(m_MovingDuration, Vec2(hoverPosition.x, hoverPosition.y - m_MoveUp));
		auto sequence = Sequence::create(moveUp, moveDown, nullptr);
		icon->runAction(RepeatForever::create(sequence));
	}

	auto iconMenu = Menu::create(icon, NULL);
	iconMenu->setPosition(Vec2::ZERO);
	m_SceneViewMaps.at(EViews::E_Main)->addChild(iconMenu, 1);
}

void SwitchSceneView::createSceneViewMaps()
{
	std::array<std::string, 3> viewSource = { "GameSceneCityView_Main1300.png",  "Hotel_View.png", "Playground_View.png" };
	std::array<unsigned int, 3> viewKeys = { 0, 1, 2 };
	auto viewsCount = viewSource.size();

	for (unsigned index = 0; index < viewsCount; ++index)
	{
		auto viewSprite = Sprite::createWithSpriteFrameName(viewSource[index]);
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



