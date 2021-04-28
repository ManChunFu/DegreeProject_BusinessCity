#include "People.h"
#include "SwitchSceneView.h"
#include "GameFunctions.h"
#include "EViews.h"
#include "GameData.h"
#include "Shop.h"
#include "ui/UIWidget.h"

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
	m_ProductList.clear();
}

void People::detachFromParent()
{
	for (auto people : m_PeopleList)
	{
		people->removeFromParentAndCleanup(false);
	}

	for (auto product : m_ProductList)
	{
		product.second->removeFromParentAndCleanup(false);
	}
}

void People::onSaleHappens(unsigned shopId, unsigned productId, unsigned saleQuantity)
{
	auto randNo = random(0, 1);
	displayPeopleInScene(randNo);

	displaySaleProductInScene(shopId, productId, saleQuantity);
}

void People::displayPeopleInScene(unsigned listIndex)
{
	if (!m_SequenceIsDone)
		return;

	if (!m_PeopleList.at(listIndex)->getParent())
	{
		GameFunctions::displaySprite(m_PeopleList.at(listIndex), Vec2(m_SceneMidPoint.x - 70.f, m_SceneMidPoint.y - 120.f),
			m_SceneViews->getSceneView(EViews::E_Playground), 1, 0.8f, 0.8f);

		m_PeopleList.at(listIndex)->setOpacity(0);
	}

	auto fadeIn = FadeIn::create(0.5f);
	auto delay = DelayTime::create(2.0f);
	auto fadeOut = FadeOut::create(0.5f);
	auto callback = CallFunc::create([=] { m_SequenceIsDone = true; });
	m_SequenceIsDone = false;

	auto sequence = Sequence::create(fadeIn, delay, fadeOut, callback, nullptr);
	m_PeopleList.at(listIndex)->runAction(sequence);
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

void People::displaySaleProductInScene(unsigned shopId, unsigned productId, unsigned saleQuantity)
{
	if (!m_ProductDisplayIsDone)
		return;

	
	if (!m_ProductList.at(productId))
	{
		auto shop = GameData::getInstance().m_Shops[shopId];
		auto productSprite = Sprite::createWithSpriteFrameName(shop->getProductSprite(productId));
		m_ProductList.insert(productId, productSprite);
	}

	if (!m_ProductList.at(productId)->getParent())
	{
		GameFunctions::displaySprite(m_ProductList.at(productId), Vec2(m_SceneMidPoint.x - 100.f, m_SceneMidPoint.y),
			m_SceneViews->getSceneView(EViews::E_Playground), 1, 0.4f, 0.4f);

		m_ProductList.at(productId)->setOpacity(0);
	}

	auto delay = DelayTime::create(0.5f);
	auto spritePos = m_ProductList.at(productId)->getPosition();
	auto fadeIn = FadeIn::create(0.5f);
	auto moveFadeOut = Sequence::create(MoveTo::create(1.f, Vec2(spritePos.x, spritePos.y + 20.f)), FadeOut::create(0.5f), nullptr);
	auto moveBack = MoveTo::create(0.1f, Vec2(spritePos.x, m_SceneMidPoint.y));
	auto callback = CallFunc::create([=] {m_ProductDisplayIsDone = true; });

	auto sequence = Sequence::create(delay, fadeIn, moveFadeOut, moveBack, callback, nullptr);
	m_ProductList.at(productId)->runAction(sequence);
}

