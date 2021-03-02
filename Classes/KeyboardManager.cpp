#include "KeyboardManager.h"


Engine::KeyboardManager::KeyboardManager()
{
    keyboardListener = EventListenerKeyboard::create();

    keyboardListener->onKeyPressed = CC_CALLBACK_2(KeyboardManager::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(KeyboardManager::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

Engine::KeyboardManager::~KeyboardManager()
{
    
}

bool Engine::KeyboardManager::isKeyPressed(Keys key)
{
    return (key == currentKey);
}

bool Engine::KeyboardManager::isKeyReleased(Keys key)
{
    return false;
}

void Engine::KeyboardManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    currentKey = (UINT8)keyCode;
}

void Engine::KeyboardManager::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}
