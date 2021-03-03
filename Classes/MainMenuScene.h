/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

class MouseOverMenuItem;
class MainMenuPlayerSetting;

class MainMenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    Vector<MenuItem*> menuItems;

private:
    Size visibleSize = Size::ZERO;
    Vec2 scaleFactor = Vec2::ONE;
    void SetScale(Sprite* sprite, UINT8 scale);

    // a selector callback
    bool isOpeningSubWindow = false;
    
    void onMouseOver(MouseOverMenuItem* overItem, Event* event);
    void menuStartCallback(Ref* pSender);
    void menuCloseCallback(Ref* pSender);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    MainMenuPlayerSetting* playerSetting = nullptr;

    SimpleAudioEngine* audio;
    void StopAudio(bool deleteAudio = false);

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);
};

#endif // __HELLOWORLD_SCENE_H__
