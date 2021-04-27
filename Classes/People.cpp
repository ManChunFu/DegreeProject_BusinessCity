#include "People.h"
#include "SwitchSceneView.h"
#include "GameFunctions.h"
#include "EViews.h"

USING_NS_CC;

People::People(SwitchSceneView* sceneViews, Vec2 sceneMidPoint)
{
	m_SceneViews = sceneViews;
	m_SceneViews->m_SaleHappensNotify = CC_CALLBACK_3(People::onSaleHappens, this);

	m_SceneMidPoint = sceneMidPoint;
	createPeopleList();
}

People::~People()
{
	m_SceneViews = nullptr;
	m_PeopleList.clear();
}

void People::onSaleHappens(unsigned shopId, unsigned productId, unsigned saleQuantity)
{
	displayPeopleInScene(0);
}

void People::displayPeopleInScene(unsigned listIndex)
{
	if (!m_PeopleList.at(0)->getParent())
	{
		GameFunctions::displaySprite(m_PeopleList.at(0), Vec2(m_SceneMidPoint.x - 70.f, m_SceneMidPoint.y - 120.f),
			m_SceneViews->getSceneView(EViews::E_Playground), 1, 0.8f, 0.8f);
	}

	auto fadeIn = FadeIn::create(0.5f);
	auto delay = DelayTime::create(3.0f);
	auto fadeOut = FadeOut::create(0.5f);
	auto sequence = Sequence::create(fadeIn, delay, fadeOut, nullptr);
	m_PeopleList.at(0)->runAction(sequence);
}

void People::createPeopleList()
{
	std::array<std::string, 2> peoplePaths = { "Girl1_Shadow.png", "Girl1_Shadow_Far.png" };

	for (unsigned index = 0; index < peoplePaths.size(); ++index)
	{
		auto peopleSprite = Sprite::createWithSpriteFrameName(peoplePaths[index]);
		if (peopleSprite)
			m_PeopleList.pushBack(peopleSprite);
	}
}

