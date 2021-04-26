#include "SwitchSceneView.h"
#include "GameFunctions.h"
#include "GameData.h"
#include "GameScene.h"
#include "MouseOverMenuItem.h"
#include "Player.h"
#include "Shop.h"

USING_NS_CC;

SwitchSceneView::~SwitchSceneView()
{
	m_SceneViewMaps.clear();
	m_BackMainButtons.clear();
	m_CurrentView = nullptr;
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

	// create hotel icon
	createMapIcon("IconMap_Small.png", "IconMap_Small_Lit.png", "IconMap_Small.png", Vec2(m_SceneMidPoint.x - 100.f, m_SceneMidPoint.y + 250.f),
		EViews::E_Hotel);

	createBackMainButton();

	//auto car_Left = Sprite::createWithSpriteFrameName("Car_Left.png");
	//car_Left->setScale(0.7f);
	//car_Left->setPosition(m_SceneMidPoint.x, m_SceneMidPoint.y - 200.f);
	//auto moveLeft = MoveTo::create(7, Vec2(car_Left->getPosition().x - 450.f, car_Left->getPosition().y));
	//sequence = Sequence::create(moveLeft, nullptr);
	//car_Left->runAction(sequence);
	//m_GameScene->addChild(car_Left, 3);

	//auto car_Right = Sprite::createWithSpriteFrameName("Car_Right.png");
	//car_Right->setScale(1.f);
	//car_Right->setPosition(m_SceneMidPoint.x -450.f, m_SceneMidPoint.y - 230.f);
	//auto moveRight = MoveTo::create(7, Vec2(car_Right->getPosition().x + 350.f, car_Right->getPosition().y));
	//sequence = Sequence::create(moveRight, nullptr);
	//car_Right->runAction(sequence);
	//m_GameScene->addChild(car_Right, 3);

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
	auto shop = GameData::getInstance().m_Shops[shopId];
	
	auto shopSprite = Sprite::createWithSpriteFrameName(shop->m_ShopInSceneSmall);
	if (shopSprite)
		GameFunctions::displaySprite(shopSprite, Vec2(m_SceneMidPoint.x - 200.f, m_SceneMidPoint.y - 80.f),
			m_SceneViewMaps.at(EViews::E_Main), 1, 0.3f, 0.3f);

	createMapIcon("IconMap_Small.png", "IconMap_Small_Lit.png", "IconMap_Small.png", Vec2(m_SceneMidPoint.x - 200.f, m_SceneMidPoint.y - 40.f),
		EViews::E_Playground);
}

void SwitchSceneView::clickIconCallback(cocos2d::Ref* pSender, unsigned viewId)
{
	m_MenuItems.at(EViews::E_Main)->pause();
	switchView(viewId);
}

void SwitchSceneView::onBackMainCallback(cocos2d::Ref* pSender)
{
	fadeEffect(m_CurrentView, false);
	m_MenuItems.at(EViews::E_Main)->resume();
	enableBackMainButtons(false);
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

void SwitchSceneView::createOrderedView(unsigned id)
{
	auto newView = m_SceneViewMaps.at(id);
	if (newView)
	{
		GameFunctions::displaySprite(newView, m_SceneMidPoint, m_GameScene, 0);
		setSpriteScale(newView, Vec2::ONE);
	}
}

void SwitchSceneView::fadeEffect(Sprite* viewSprite, bool fadeIn)
{
	if (fadeIn)
	{
		viewSprite->setOpacity(0);
		auto spriteFadeIn = FadeIn::create(1.0f);
		viewSprite->runAction(spriteFadeIn);
		viewSprite->setOpacity(1);
		return;
	}

	auto spriteFadeOut = FadeOut::create(1.0f);
	viewSprite->runAction(spriteFadeOut);
	viewSprite->setOpacity(0);
}

void SwitchSceneView::createMapIcon(const std::string& normal, const std::string& mouseOver, const std::string& disable, Vec2 displayPos, unsigned viewId, float iconScale)
{
	auto icon = MouseOverMenuItem::creatMouseOverMenuButton(normal, mouseOver, disable, CC_CALLBACK_1(
		SwitchSceneView::clickIconCallback, this, viewId));

	if (icon)
	{
		m_MenuItems.pushBack(displayMenuButton(icon, CC_CALLBACK_2(SwitchSceneView::onMouseOver, this), displayPos, itemTypes::BUTTON, iconScale,
			true, Vec2(10.f, -100.f)));

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

