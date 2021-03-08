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

namespace CocosDenshion
{
    class SimpleAudioEngine;
}
class MouseOverMenuItem;
class MainMenuPlayerSetting;

class MainMenuScene : public cocos2d::Scene
{
public:
    virtual ~MainMenuScene();

    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void setSpriteScale(cocos2d::Sprite* sprite, float scale);
    bool isOpeningSubWindow = false;
    void setMenuItemVisible(bool value);
    void StopAudio(bool deleteAudio = false);

private:
    cocos2d::Size m_VisibleSize = cocos2d::Size::ZERO;
    cocos2d::Vector<cocos2d::MenuItem*> m_MenuItems;
    CocosDenshion::SimpleAudioEngine* m_Audio;

    // a selector callback
    void onMouseOver(MouseOverMenuItem* overItem, cocos2d::Event* event);
    void menuStartCallback(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    MainMenuPlayerSetting* m_PlayerSetting = nullptr;

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);
};

#endif // __HELLOWORLD_SCENE_H__
