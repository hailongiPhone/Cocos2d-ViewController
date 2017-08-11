//
//  TabBarItem.cpp
//  TKPoker
//
//  Created by hailong on 13-10-11.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "TabBarItem.h"
//#include "cc-e.h"

enum TabBarItemZOrder {
    kTabBarItemZOrderButton = 0,
    kTabBarItemZOrderBackgroundMask,
    kTabBarItemZOrderTitle,
    kTabBarItemZOrderTop,
};

#pragma mark -
#pragma mark -- Lifecycle --

TabBarItem::TabBarItem(void)
:m_callback(nullptr)
,_pBubble(nullptr)
{
    
}

TabBarItem::~TabBarItem(void)
{
    log("~TabBarItem");
}

bool TabBarItem::init()
{
    if (!super::init()) {
        return false;
    }
    return true;
}

#pragma mark -
#pragma mark -- Action --

void TabBarItem::showHint()
{
    if (!_pBubble)
    {
        setBubble(my::MyBubble::create(1));
        this->addChild(_pBubble);
        _pBubble->setAnchorPoint(Point(0, .7));
        _pBubble->setPositionPercent(Point(-.5, .5));
    }
    _pBubble->setValue(1);
}

void TabBarItem::dismissHint()
{
    if(_pBubble)_pBubble->setValue(0);
}

void TabBarItem::autoHint(bool bshow)
{
    if (bshow)
    {
        showHint();
    }
    else
    {
        dismissHint();
    }
}

void TabBarItem::setDisable(bool isDisable)
{
}

void TabBarItem::setHighlight(bool isHighlight)
{
}

bool TabBarItem::pointInSide(Point pt)
{
    Rect r = this->getBoundingBox();
    r.origin.x -= r.size.width * .5;
    r.origin.y -= r.size.height * .5;
    return r.containsPoint(pt);
}
