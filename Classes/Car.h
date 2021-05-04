#pragma once

#include "cocos2d.h"

class SwitchSceneView;

class Car
{
public:
	Car(SwitchSceneView* sceneView, cocos2d::Vec2 sceneMidPoint);
	virtual~Car();


protected:

private:
	enum EDirections
	{
		E_Left = 0,
		E_Right = 1,
		E_Forward = 2
	};
	SwitchSceneView* m_SceneView;
	cocos2d::Vec2 m_SceneMidPoint = cocos2d::Vec2::ZERO;
	std::unordered_map<unsigned, cocos2d::Vector<cocos2d::Sprite*>> m_SpawnCarList;

	void runFrontSceneCar(cocos2d::Vector<cocos2d::Sprite*> spriteList, cocos2d::Vec2 displayPos, float distanceRange, cocos2d::Vec2 moveDirection, float fadeInSpeed, float fadeOutSpeed);
	void runForwardSceneCar(cocos2d::Vector<cocos2d::Sprite*> spriteList, cocos2d::Vec2 displayPos, float distanceRange, cocos2d::Vec2 moveDirection, float fadeInSpeed, float fadeOutSpeed);
	void createCarSpriteList();
};
