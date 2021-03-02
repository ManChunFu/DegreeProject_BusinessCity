#pragma once

#include "cocos2d.h"

USING_NS_CC;

namespace Engine
{
	enum Keys
	{
		W = 115, 
		E = 97,
		R = 110,
		A = 93,
		S = 111,
		D = 96,
		w = 146,
		e = 128,
		r = 141,
		a = 124,
		s = 142,
		d = 127,
		ESCAPE = 6,
		SPACE = 59,
		LEFT = 26,
		RIGHT = 27,
		UP = 28,
		DOWN = 29
	};
	class KeyboardManager : public cocos2d::Layer
	{
	public:
		KeyboardManager();
		~KeyboardManager();

		bool isKeyPressed(Keys key);
		bool isKeyReleased(Keys key);
	
	 private:
		EventListenerKeyboard* keyboardListener;
		void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
		void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
		UINT8 currentKey = 0;
	};
}
