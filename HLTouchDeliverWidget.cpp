//
//  HLTouchDeliverWidget.cpp
//  KnightAndDragon
//
//  Created by hailong on 14-5-6.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//

#include "HLTouchDeliverWidget.h"


#pragma mark -
#pragma mark -- lifecycle --

#define kStateDefault (-1)

HLTouchDeliverWidget::HLTouchDeliverWidget(void)
:m_callback(nullptr)
,m_state(kStateDefault)
{
}

HLTouchDeliverWidget::~HLTouchDeliverWidget(void)
{
}

HLTouchDeliverWidget * HLTouchDeliverWidget::create(TouchDeliverCallback callback)
{
    HLTouchDeliverWidget * pRet = new HLTouchDeliverWidget();

    if (pRet && pRet->init()) {
        pRet->setCallback(callback);
        pRet->autorelease();
    }else{
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool HLTouchDeliverWidget::init()
{
    if (!super::init()) {
        return false;
    }
    
    return true;
}

void HLTouchDeliverWidget::checkChildInfo(int handleState,cocos2d::ui::Widget* sender,const Point &touchPoint)
{
    Node * parent = getParent();
    cocos2d::ui::Widget * widgetParent = dynamic_cast<cocos2d::ui::Widget *>(parent);
    
    if (widgetParent) {
        widgetParent->checkChildInfo(handleState, sender, touchPoint);
    }else if(m_callback){
        m_state = handleState;
        m_lastPoint = touchPoint;
        m_callback(handleState,sender,touchPoint);
        if (handleState >=2) {
            m_state = kStateDefault;
        }
    }
}

void HLTouchDeliverWidget::setOpacity(GLubyte opacity)
{
    super::setOpacity(opacity);
    CCLOG("HLTouchDeliverWidget::setOpacity == %hhu",opacity);
}


void HLTouchDeliverWidget::onExit()
{
    if (m_state != kStateDefault) {
        m_callback(3,nullptr,m_lastPoint);
    }
    super::onExit();
}