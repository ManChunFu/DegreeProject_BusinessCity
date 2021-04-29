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

	m_HotdogPeopleList.clear();
	m_HotdogPeopleList.clear();
	m_IcecreamPeopleList.clear();
}

void People::detachFromParent()
{
	for (auto people : m_HotdogPeopleList)
	{
		people->removeFromParentAndCleanup(false);
	}

	for (auto product : m_ProductList)
	{
		product.second->removeFromParentAndCleanup(false);
	}

	for (auto people : m_IcecreamPeopleList)
	{
		people->removeFromParentAndCleanup(false);
	}
}

void People::onSaleHappens(unsigned sceneId,unsigned shopId, unsigned productId)
{
	if (shopId == 0)
	{
		auto randNo = random(0, m_HotdogPeopleMax);
		displayPeopleInScene(m_HotdogPeopleList, sceneId, randNo, shopId, productId);
	}
	else
	{
		auto randNo = random(0, m_IcePeopleMax);
		displayPeopleInScene(m_IcecreamPeopleList, sceneId, randNo, shopId, productId);
	}
}

void People::displayPeopleInScene(Vector<Sprite*> peopleList, unsigned sceneId, unsigned listIndex, unsigned shopId, unsigned productId)
{
	if (!m_SequenceIsDone)
		return;

	if (!peopleList.at(listIndex)->getParent())
	{
		GameFunctions::displaySprite(peopleList.at(listIndex), (shopId == 0)? Vec2(m_SceneMidPoint.x - 70.f, m_SceneMidPoint.y - 90.f) :
			Vec2(m_SceneMidPoint.x - 400.f, m_SceneMidPoint.y -140.f), m_SceneViews->getSceneView(sceneId), 1, 0.8f, 0.8f);

		peopleList.at(listIndex)->setOpacity(0);
	}

	auto fadeIn = FadeIn::create(0.5f);
	auto delay = DelayTime::create(2.0f);
	auto fadeOut = FadeOut::create(0.5f);
	auto callback = CallFunc::create([=] { m_SequenceIsDone = true; });
	m_SequenceIsDone = false;

	auto sequence = Sequence::create(fadeIn, delay, fadeOut, callback, nullptr);
	peopleList.at(listIndex)->runAction(sequence);

	displaySaleProductInScene(sceneId, shopId, productId);
}

void People::displaySaleProductInScene(unsigned sceneId, unsigned shopId, unsigned productId)
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
		GameFunctions::displaySprite(m_ProductList.at(productId), (shopId == 0)?  Vec2(m_SceneMidPoint.x - 100.f, m_SceneMidPoint.y) 
			: Vec2(m_SceneMidPoint.x - 400.f, m_SceneMidPoint.y + 50.f), m_SceneViews->getSceneView(sceneId), 1, 0.4f, 0.4f);

		m_ProductList.at(productId)->setOpacity(0);
	}

	auto delay = DelayTime::create(0.5f);
	auto spritePos = m_ProductList.at(productId)->getPosition();
	auto fadeIn = FadeIn::create(0.5f);
	auto moveFadeOut = Sequence::create(MoveTo::create(1.f, Vec2(spritePos.x, spritePos.y + 20.f)), FadeOut::create(0.5f), nullptr);
	auto moveBack = MoveTo::create(0.1f, Vec2(spritePos.x, (shopId == 0)? m_SceneMidPoint.y : m_SceneMidPoint.y + 50.f));
	auto callback = CallFunc::create([=] {m_ProductDisplayIsDone = true; });

	auto sequence = Sequence::create(delay, fadeIn, moveFadeOut, moveBack, callback, nullptr);
	m_ProductList.at(productId)->runAction(sequence);
}

void People::createPeopleList()
{
	std::array<std::string, 7> peoplePaths = { "Girl1_Shadow.png", "Girl1_Shadow_Far.png", "Girl2_Shadow.png", "Girl3_Shadow.png", 
		"Boy1_Shadow.png", "Boy2_Shadow.png", "Boy3_Shadow.png" };

	m_HotdogPeopleMax = peoplePaths.size();

	for (unsigned index = 0; index < m_HotdogPeopleMax; ++index)
	{
		auto peopleSprite = Sprite::createWithSpriteFrameName(peoplePaths[index]);
		if (peopleSprite)
			m_HotdogPeopleList.pushBack(peopleSprite);
	}

	std::array<std::string, 6> icecreamPeoplePaths = { "IceGirl1_Shadow.png", "IceBoy1_Shadow.png", "IceBoy2_Shadow.png", "IceGirl2_Shadow.png",
		"IceGirl3_Shadow.png", "IceBoy3_Shadow.png" };
	m_IcePeopleMax = icecreamPeoplePaths.size();
	for (unsigned index = 0; index < m_IcePeopleMax; ++index)
	{
		auto sprite = Sprite::createWithSpriteFrameName(icecreamPeoplePaths[index]);
		if (sprite)
			m_IcecreamPeopleList.pushBack(sprite);
	}
	
	// for later use in random max
	m_HotdogPeopleMax--;
	m_IcePeopleMax--;
}


