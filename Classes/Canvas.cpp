#include "Canvas.h"
#include "Player.h"

USING_NS_CC;

Size m_VisibleSize = Director::getInstance()->getVisibleSize();
Vec2 m_origin = Director::getInstance()->getVisibleOrigin();

void Canvas::setSpriteScale(cocos2d::Sprite* sprite, cocos2d::Vec2 scale)
{
	sprite->setScaleX((m_VisibleSize.width / sprite->getContentSize().width) * scale.x);
	sprite->setScaleY((m_VisibleSize.height / sprite->getContentSize().height) * scale.y);
}
