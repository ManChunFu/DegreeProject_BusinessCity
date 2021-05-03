#pragma once

#include "cocos2d.h"

NS_CC_BEGIN;
namespace ui
{
	class Widget;
}
NS_CC_END;

namespace CocosDenshion
{
	class SimpleAudioEngine;
}


class SwitchSceneView;
struct PeopleData;

class People
{
public:
	People(SwitchSceneView* sceneViews, cocos2d::Vec2 sceneMidPoint);
	virtual ~People();

	void detachFromParent(unsigned currentSceneId, bool cleanUp);
	void createPeopleList(unsigned sceneId, unsigned shopId);
protected:
	void onSaleHappens(unsigned sceneId, unsigned shopId, unsigned productId);

private:
	CocosDenshion::SimpleAudioEngine* m_Audio;

	std::map<unsigned, PeopleData*> m_PeopleData;
	SwitchSceneView* m_SceneViews = nullptr;
	std::unordered_map<unsigned int, cocos2d::Vector<cocos2d::Sprite*>> m_PeopleShoppingList;
	cocos2d::Map<unsigned int, cocos2d::Sprite*> m_ProductList;
	
	cocos2d::Vec2 m_SceneMidPoint = cocos2d::Vec2::ZERO;
	bool m_SequenceIsDone = true;
	bool m_ProductDisplayIsDone = true;
	int m_RandomMax = 5;

	void displayPeopleInScene(cocos2d::Vector<cocos2d::Sprite*> peopleList, unsigned sceneId, unsigned listIndex, unsigned shopId, unsigned productId);
	void displaySaleProductInScene(unsigned sceneId, unsigned shopId, unsigned productId);

};