#pragma once

#include "cocos2d.h"

NS_CC_BEGIN;
namespace ui
{
	class Widget;
}
NS_CC_END;

class SwitchSceneView;

class People
{
public:
	People(SwitchSceneView* sceneViews, cocos2d::Vec2 sceneMidPoint);
	virtual ~People();

	void detachFromParent();
protected:
	void onSaleHappens(unsigned shopId, unsigned productId, unsigned saleQuantity);

private:
	SwitchSceneView* m_SceneViews = nullptr;
	cocos2d::Vector<cocos2d::Sprite*> m_PeopleList;
	cocos2d::Map<unsigned int, cocos2d::Sprite*> m_ProductList;
	
	cocos2d::Vec2 m_SceneMidPoint = cocos2d::Vec2::ZERO;
	bool m_SequenceIsDone = true;
	bool m_ProductDisplayIsDone = true;
	
	void displayPeopleInScene(unsigned listIndex);
	void displaySaleProductInScene(unsigned shopId, unsigned productId, unsigned saleQuantity);
	void createPeopleList();

};