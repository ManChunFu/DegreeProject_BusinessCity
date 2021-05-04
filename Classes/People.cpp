#include "People.h"
#include "DataManager.h"
#include "PeopleData.h"
#include "PeopleLocationData.h"
#include "SwitchSceneView.h"
#include "GameFunctions.h"
#include "EViews.h"
#include "GameData.h"
#include "Shop.h"
#include "ui/UIWidget.h"
#include "cocostudio/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

People::People(SwitchSceneView* sceneViews, Vec2 sceneMidPoint)
{
	m_PeopleData = DataManager::getPeople();
	m_SceneViews = sceneViews;
	m_SceneViews->m_SaleHappensNotify = CC_CALLBACK_3(People::onSaleHappens, this);

	m_SceneMidPoint = sceneMidPoint;

	m_Audio = GameData::getInstance().m_Audio;
}

People::~People()
{
	for (auto item : m_PeopleData)
	{
		delete item.second;
	}
	m_PeopleData.clear();

	m_SceneViews = nullptr;
	m_ProductList.clear();
	m_PeopleShoppingList.clear();
}

void People::detachFromParent(unsigned currentSceneId, bool cleanUp)
{
	auto foundList = m_PeopleShoppingList.find(currentSceneId);
	if (foundList != m_PeopleShoppingList.end())
	{
		for (auto people : m_PeopleShoppingList.at(currentSceneId))
		{
			people->setOpacity(0);
			people->stopAllActions();
			people->removeFromParentAndCleanup(cleanUp);
		}
	}

	for (auto product : m_ProductList)
	{
		for (auto sprite : product.second)
		{
			sprite.second->setOpacity(0);
			sprite.second->stopAllActions();
			sprite.second->removeFromParentAndCleanup(cleanUp);
		}
	}

	m_SequenceIsDone = true;
	m_ProductDisplayIsDone = true;

	if (cleanUp)
	{
		m_PeopleShoppingList.at(currentSceneId).clear();
		m_ProductList.at(currentSceneId).clear();
	}
}

void People::onSaleHappens(unsigned sceneId, unsigned shopId, unsigned productId)
{
	auto randNo = random(0, m_RandomMax);

	displayPeopleInScene(m_PeopleShoppingList.at(sceneId), sceneId, randNo, shopId, productId);
}

void People::displayPeopleInScene(Vector<Sprite*> peopleList, unsigned sceneId, unsigned listIndex, unsigned shopId, unsigned productId)
{
	if (!m_SequenceIsDone)
		return;

	if (!peopleList.at(listIndex)->getParent())
	{
		GameFunctions::displaySprite(peopleList.at(listIndex), m_PeopleData.at(sceneId)->m_PeopleLocations.at(shopId)->m_PeopleLocation,
			m_SceneViews->getSceneView(sceneId), 1, 0.8f, 0.8f);

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

	auto foundScene = m_ProductList.find(sceneId);
	Sprite* product = nullptr;

	if (foundScene != m_ProductList.end())
		product = m_ProductList.at(sceneId).at(productId);
	
	if (!product)
	{
		auto shop = GameData::getInstance().m_Shops[shopId];
		auto productSprite = Sprite::createWithSpriteFrameName(shop->getProductSprite(productId));
		m_ProductList[sceneId].insert(productId, productSprite);
		auto  map= m_ProductList.at(sceneId);
		product = map.at(productId);
		//m_ProductList.insert(sceneId, productId, productSprite));
	}

	if (!product->getParent())
	{
		GameFunctions::displaySprite(product, m_PeopleData.at(sceneId)->m_PeopleLocations.at(shopId)->m_ProductLocation,
			m_SceneViews->getSceneView(sceneId), 1, 0.4f, 0.4f);

		product->setOpacity(0);
	}

	auto delay = DelayTime::create(0.5f);
	auto spritePos = product->getPosition();
	auto fadeIn = FadeIn::create(0.5f);
	auto moveFadeOut = Sequence::create(MoveTo::create(1.f, Vec2(spritePos.x, spritePos.y + 20.f)), FadeOut::create(0.5f), nullptr);
	auto moveBack = MoveTo::create(0.1f, Vec2(spritePos.x, m_PeopleData.at(sceneId)->m_PeopleLocations.at(shopId)->m_ProductLocation.y));
	auto callback = CallFunc::create([=] {m_ProductDisplayIsDone = true; });

	auto sequence = Sequence::create(delay, fadeIn, moveFadeOut, moveBack, callback, nullptr);
	product->runAction(sequence);

	// play money sound
	m_Audio->playEffect("Sounds/MoneyPop.mp3", false, 0.8f, 0.8f, 0.8f);
	m_Audio->setEffectsVolume(0.5f);
}

void People::createPeopleList(unsigned sceneId, unsigned shopId)
{
	auto tempPeopleData = m_PeopleData.at(sceneId)->m_PeopleLocations.at(shopId)->m_PeopleSpritePaths;
	auto pepoleSize = tempPeopleData.size();
	Vector<Sprite*> tempPeopleList;

	for (unsigned index = 0; index < pepoleSize; ++index)
	{
		auto peopleSprite = Sprite::createWithSpriteFrameName(tempPeopleData[index]);
		if (peopleSprite)
			tempPeopleList.pushBack(peopleSprite);
	}

	m_PeopleShoppingList[sceneId] = tempPeopleList;
	m_RandomMax = pepoleSize - 1;
}


