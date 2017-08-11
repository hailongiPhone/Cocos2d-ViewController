//
//  ButtonMenuView.cpp
//  TKPoker
//
//  Created by hailong on 13-10-29.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "ButtonMenuView.h"
#include "TabBarItem.h"


#pragma mark -
#pragma mark -- lifecycle --

ButtonMenuView::ButtonMenuView(void)
{
}

ButtonMenuView::~ButtonMenuView(void)
{
    m_buttons.clear();
}

ButtonMenuView * ButtonMenuView::create()
{
    ButtonMenuView * pRet = new ButtonMenuView();

    if (pRet && pRet->init()) {
        pRet->autorelease();
    }else{
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool ButtonMenuView::init()
{
    if (!super::init()) {
        return false;
    }
    
    return true;
}

#pragma mark -
#pragma mark -- SetupView --
void ButtonMenuView::resetView(const ButtonList& buttons)
{
    this->removeAllChildren();
    
    TabBarItem * aButton = dynamic_cast<TabBarItem *>(buttons.front());
    CCASSERT(aButton, "类型不对");
    int itemCount = buttons.size();
    
    int itemW = aButton->getSize().width;
    int itemH = aButton->getSize().height;
    
    int newW = 620;
    int spaceVertical = 30;
    
    int spaceEdge = 6;
    int itemsInLine = 5;
    
    
    int spaceHorizon = ((newW - 2 * spaceEdge) -
                        itemW * itemsInLine) / (itemsInLine + 1);
    int lineNum = ceil(itemCount / (float)itemsInLine);
    
    int newH = (lineNum )  * itemH +
                (lineNum + 1) * spaceVertical + 2 * spaceEdge;
    
    this->setSize(Size(newW,newH));
    
//    spriteFrame("public_box_bg.png", "tps/Public.plist");
    this->setBackGroundImage("public_box_bg.png", ui::Widget::TextureResType::PLIST);
    this->setBackGroundImageScale9Enabled(true);
    
    for (int i = 0; i < itemCount; i++) {
        aButton =  dynamic_cast<TabBarItem *>(buttons.at(i));
        aButton
        ->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType evnet)
                                {
                                    this->onButtonTap(dynamic_cast<ui::Widget*>(sender), evnet);
                                });
        int lineNum = ceil(i / itemsInLine);
        int lineIndex = i % itemsInLine;
        
        int pX = spaceEdge + (lineIndex + 1) * spaceHorizon + itemW * (lineIndex + 0.5);
        int pY = spaceEdge + (lineNum + 1) * spaceVertical + (lineNum + 0.5) * itemH;
        aButton->setPosition(Point(pX, newH - pY));
        aButton->setAnchorPoint(Point(0.5, 0.5));
       
        this->addChild(aButton);
    }
    
}

#pragma mark -
#pragma mark -- Action --
void ButtonMenuView::setupButtons(const ButtonList& buttons)
{
    m_buttons = buttons;
    resetView(m_buttons);
}


#pragma mark -
#pragma mark -- Event --
void ButtonMenuView::onButtonTap(Widget * sender, ui::Widget::TouchEventType type)
{
    if (type != ui::Widget::TouchEventType::ENDED) {
        return;
    }
    
    if (_touchEventCallback)
    {
        _touchEventCallback(sender, type);
    }
    
//    if (_touchEventListener && _touchEventSelector)
//    {
//        (_touchEventListener->*_touchEventSelector)(sender, (ui::TouchEventType)(type));
//    }
}
