#include "MouseOverButtonHeader.h"

MouseOverButton::~MouseOverButton()
{
}

MouseOverButton* MouseOverButton::create(Node* normalSprite, Node* overSprite, Node* disabledSprite, const ccMenuCallback& callback)
{
    MouseOverButton* mouseOver = new(std::nothrow) MouseOverButton();

    if (mouseOver && mouseOver->create(normalSprite, overSprite, disabledSprite, callback));
    {
        //mouseOver->setMouseListener();
        //mouseOver->autorelease();

        return mouseOver;
    }
    CC_SAFE_DELETE(mouseOver);
    return nullptr;
}
