#pragma once

#include "UIPanel.h"

class SwitchSceneView;

class People
{
public:
	People(SwitchSceneView* sceneViews, cocos2d::Vec2 sceneMidPoint);
	virtual ~People();

protected:
	void onSaleHappens(unsigned shopId, unsigned productId, unsigned saleQuantity);

private:
	SwitchSceneView* m_SceneViews = nullptr;
	cocos2d::Vector<cocos2d::Sprite*> m_PeopleList;

	cocos2d::Vec2 m_SceneMidPoint = cocos2d::Vec2::ZERO;

	void displayPeopleInScene(unsigned listIndex);
	void createPeopleList();

};