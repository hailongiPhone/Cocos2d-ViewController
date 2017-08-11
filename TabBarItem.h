//
//  TabBarItem.h
//  TKPoker
//
//  Created by hailong on 13-10-11.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//


/** TabBarItem
 *
 *
 *  TabBarItem: footer item
 *  不同游戏tabbaritem实现的效果都不一样，提成抽象类
 *
 *  维护的功能：  1：按下
 *              2：气泡提示
 *              3：特殊高亮（或背景）
 */

#ifndef __TabBarItem_H__
#define __TabBarItem_H__
//#include "TTBubble.h"
//#include "cc-e.h"
#include "cocos2d.h"
using namespace cocos2d;

#include "CocosGUI.h"
using namespace ui;
#include "MyBubble.h"

class TabBarItem;

typedef std::function<void(TabBarItem *, cocos2d::ui::Widget::TouchEventType type)> TabBarItemCallback;

class TabBarItem : public ui::Layout
{
    typedef ui::Layout super;
public:
    
    TabBarItem(void);
	virtual ~TabBarItem(void);
    virtual bool init();
    
    virtual void showHint();
    virtual void dismissHint();
    virtual void autoHint(bool bshow) final;

    virtual void setDisable(bool isDisable);
    virtual void setHighlight(bool isHighlight);
    
    virtual bool pointInSide(Point pt);
    
    CC_SYNTHESIZE(TabBarItemCallback, m_callback, Callback);
    CC_SYNTHESIZE_RETAIN(MyBubblePtr, _pBubble, Bubble);
};


#endif //__TabBarItem_H__
