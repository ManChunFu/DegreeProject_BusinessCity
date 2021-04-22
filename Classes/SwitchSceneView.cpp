#include "SwitchSceneView.h"
#include "GameFunctions.h"
#include "GameScene.h"
#include "MouseOverMenuItem.h"

USING_NS_CC;

SwitchSceneView::~SwitchSceneView()
{
	m_SceneViewMaps.clear();
}

void SwitchSceneView::setSpriteScale(Sprite* sprite, Vec2 scale)
{
	sprite->setScaleX((m_VisibleSize.width / sprite->getContentSize().width) * scale.x);
	sprite->setScaleY((m_VisibleSize.height / sprite->getContentSize().height) * scale.y);
}

void SwitchSceneView::runInit(GameScene* gameScene, Size visibleSize, Vec2 origin, cocos2d::Vec2 sceneMidPoint)
{
	createSceneViewMaps();
	m_GameScene = gameScene;

	m_VisibleSize = visibleSize;
	m_Origin = origin;
	m_SceneMidPoint = sceneMidPoint;

	GameFunctions::displaySprite(m_SceneViewMaps.at(EViews::E_Main), m_SceneMidPoint, m_GameScene, 0);
	setSpriteScale(m_SceneViewMaps.at(EViews::E_Main), Vec2::ONE);

	auto viewHover = MouseOverMenuItem::creatMouseOverMenuButton("IconMap_Small.png", "IconMap_Small.png", "IconMap_Big.png", CC_CALLBACK_1(
		SwitchSceneView::clickIconCallback, this, EViews::E_Hotel));
	if (viewHover)
		m_MenuItems.pushBack(displayMenuButton(viewHover, CC_CALLBACK_2(SwitchSceneView::onMouseOver, this), Vec2(m_SceneMidPoint.x + 170.f,
			m_SceneMidPoint.y + 300.f), itemTypes::BUTTON, 1, true));

	auto iconButtons = Menu::createWithArray(m_MenuItems);
	iconButtons->setPosition(Vec2::ZERO);
	m_SceneViewMaps.at(EViews::E_Main)->addChild(iconButtons, 1);
}

void SwitchSceneView::switchView(unsigned id)
{
	auto newView = m_SceneViewMaps.at(id);
	if (newView)
	{
		GameFunctions::displaySprite(newView, m_SceneMidPoint, m_GameScene, 0);

		newView->setOpacity(0);
		auto spriteFadeIn = FadeIn::create(1.0f);
		newView->runAction(spriteFadeIn);
	}

}

void SwitchSceneView::clickIconCallback(cocos2d::Ref* pSender, unsigned viewId)
{
	switchView(viewId);
}

void SwitchSceneView::onMouseOver(MouseOverMenuItem* menuItem, cocos2d::Event* event)
{
}

void SwitchSceneView::createSceneViewMaps()
{
	std::array<std::string, 2> viewSource = { "GameSceneCityView_Main.png",  "Hotel_View.png" };
	std::array<unsigned int, 2> viewKeys = { 0, 1 };
	auto viewsCount = viewSource.size();

	for (unsigned index = 0; index < viewsCount; ++index)
	{
		auto viewSprite = Sprite::createWithSpriteFrameName(viewSource[index]);
		if (viewSprite)
			m_SceneViewMaps.insert(viewKeys[index], viewSprite);
	}
}
