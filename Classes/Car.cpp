#include "Car.h"
#include "SwitchSceneView.h"
#include "GameFunctions.h"
#include "EViews.h"

USING_NS_CC;

Car::Car(SwitchSceneView* sceneView, cocos2d::Vec2 sceneMidPoint)
{
	m_SceneView = sceneView;
	m_SceneMidPoint = sceneMidPoint;
	createCarSpriteList();
	// car goes left
	runFrontSceneCar(m_SpawnCarList.at(EDirections::E_Left), Vec2(m_SceneMidPoint.x, m_SceneMidPoint.y - 260.f), 18.f, 
		Vec2(-700.f, 0.f), 1.f, 0.1f);
	// car goes right
	runFrontSceneCar(m_SpawnCarList.at(EDirections::E_Right), Vec2(m_SceneMidPoint.x - 700.f, m_SceneMidPoint.y - 291.f), 16.f, 
		Vec2(550.f, 0.f), 1.f, 0.1f);
	// car goes up
	runForwardSceneCar(m_SpawnCarList.at(EDirections::E_Forward), Vec2(m_SceneMidPoint.x - 68.f, m_SceneMidPoint.y - 190.f),  
		Vec2(7.5f, 50.f), 0.6f, 0.85f, 1.f, 1.f);
	// car goes down
	runForwardSceneCar(m_SpawnCarList.at(EDirections::E_Backward), Vec2(m_SceneMidPoint.x -5.f, m_SceneMidPoint.y + 180.f),
		Vec2(-3.8f, -45.f), 0.1f, 1.3f, 1.f, 1.f);

}

Car::~Car()
{
	m_SceneView = nullptr;
	m_SpawnCarList.clear();
}

void Car::runFrontSceneCar(Vector<Sprite*> spriteList, Vec2 displayPos, float distanceRange, Vec2 moveDirection, float fadeInSpeed, float fadeOutSpeed)
{
	float delayTime = 2.f;
	Vec2 originalPos = displayPos;
	std::array<float,2> randomDistance = { 0.f, distanceRange };
	std::array<float, 2> randomDelay = { 2.f, 3.f };
	auto size = spriteList.size();
	for (unsigned index = 0; index < size; ++index)
	{
		GameFunctions::displaySprite(spriteList.at(index), displayPos, m_SceneView->getSceneView(EViews::E_Main), 1.f, 0.7f, 0.7f);
		spriteList.at(index)->setOpacity(0);

		auto delay = DelayTime::create(delayTime);
		auto fadeIn = FadeIn::create(fadeInSpeed);
		auto move = MoveTo::create(7, Vec2(spriteList.at(index)->getPosition().x + moveDirection.x, spriteList.at(index)->getPosition().y));
		auto fadeOut = FadeOut::create(fadeOutSpeed);

		displayPos.x = originalPos.x;
		auto callFunc = CallFunc::create([=] { spriteList.at(index)->setPosition(displayPos); });
		auto sequence = Sequence::create(delay, Spawn::create(fadeIn, move, nullptr), fadeOut, callFunc, nullptr);
		spriteList.at(index)->runAction(RepeatForever::create(sequence));

		displayPos.y = originalPos.y;

		displayPos.x -= 40.f;
		displayPos.y += randomDistance[random(0, 1)];
		delayTime += randomDelay[random(0, 1)];
	}
}

void Car::runForwardSceneCar(Vector<Sprite*> spriteList, Vec2 displayPos, Vec2 moveDirection, float originScale, float scaleby,  float fadeInSpeed, float fadeOutSpeed)
{
	float delayRate = 1.f;
	auto size = spriteList.size();
	for (unsigned index = 0; index < size; ++index)
	{
		GameFunctions::displaySprite(spriteList.at(index), displayPos, m_SceneView->getSceneView(EViews::E_Main), 1.f, originScale, originScale);
		spriteList.at(index)->setOpacity(0);

		auto delay = DelayTime::create(delayRate);
		auto fadeIn = FadeIn::create(fadeInSpeed);
		auto move = MoveBy::create(1.f, moveDirection);
		auto scaleTo = ScaleBy::create(1.f, scaleby);
		auto spawn = Spawn::create(fadeIn, move, scaleTo, nullptr);
		auto repeat = Repeat::create(spawn, 7);
		auto fadeOut = FadeOut::create(fadeOutSpeed);
		auto repeat1 = Repeat::create(Spawn::create(move, scaleTo, fadeOut, nullptr), 1);
		auto callFunc = CallFunc::create([=] {spriteList.at(index)->setPosition(displayPos); spriteList.at(index)->setScale(originScale); });
		auto sequence = Sequence::create(delay, repeat, repeat1, callFunc, nullptr);
		spriteList.at(index)->runAction(RepeatForever::create(sequence));

		delayRate += random(2, 3);
	}
}

void Car::createCarSpriteList()
{
	// 0 = front car left, 1 = front car right, 2 = forward car
	std::array<std::string, 5> frontSceneCarLeft = { "RedCar_Left.png", "BlueCar_Left.png", "Car_Left.png", "RedCar_Left.png", "Car_Left.png" };
	auto leftSize = frontSceneCarLeft.size();
	Vector<Sprite*> tempSprites;
	for (unsigned index = 0; index < leftSize; ++index)
	{
		auto sprite = Sprite::createWithSpriteFrameName(frontSceneCarLeft[index]);
		if (sprite)
			tempSprites.pushBack(sprite);
	}
	m_SpawnCarList[EDirections::E_Left] = tempSprites;
	tempSprites.clear();

	std::array<std::string, 4> frontSceneCarRight = { "Car_Right.png", "GreenCar_Right.png", "PurpleCar_Right.png", "Car_Right.png" };
	auto rightSize = frontSceneCarRight.size();
	for (unsigned index = 0; index < rightSize; ++index)
	{
		auto sprite = Sprite::createWithSpriteFrameName(frontSceneCarRight[index]);
		if (sprite)
			tempSprites.pushBack(sprite);
	}
	m_SpawnCarList[EDirections::E_Right] = tempSprites;
	tempSprites.clear();

	std::array<std::string, 3> middleSceneCarForward = {"WhiteCar_Forward.png", "YellowCar_Forward.png", "YellowCar_Forward.png" };
	auto upSize = middleSceneCarForward.size();
	for (unsigned index = 0; index < upSize; ++index)
	{
		auto sprite = Sprite::createWithSpriteFrameName(middleSceneCarForward[index]);
		if (sprite)
			tempSprites.pushBack(sprite);
	}
	m_SpawnCarList[EDirections::E_Forward] = tempSprites;
	tempSprites.clear();

	std::array<std::string, 3> middleSceneCarBackward = { "WhiteCar_Backward.png", "GreenCar_Backward.png", "WhiteCar_Backward.png" };
	auto downSize = middleSceneCarBackward.size();
	for (unsigned index = 0; index < downSize; ++index)
	{
		auto sprite = Sprite::createWithSpriteFrameName(middleSceneCarBackward[index]);
		if (sprite)
			tempSprites.pushBack(sprite);
	}
	m_SpawnCarList[EDirections::E_Backward] = tempSprites;



}