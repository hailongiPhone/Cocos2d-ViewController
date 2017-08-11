//
//  HLTouchDeliverWidget.h
//  KnightAndDragon
//
//  Created by hailong on 14-5-6.
//  Copyright (c) 2014年 __MyCompanyName__. All rights reserved.
//


/** HLTouchDeliverWidget
 *
 *  <#Discription#>
 *  <#Discription#>
 *
 */

#ifndef __HLTouchDeliverWidget_H__
#define __HLTouchDeliverWidget_H__

#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "CocosGUI.h"

typedef std::function<void(int handleState,
                           cocos2d::ui::Widget* sender,
                           const Point &touchPoint)> TouchDeliverCallback;

class HLTouchDeliverWidget : public cocos2d::ui::Widget
{
    typedef cocos2d::ui::Widget super;
    
public:
    
#pragma mark -- Lifecycle --
    static HLTouchDeliverWidget * create(TouchDeliverCallback callback = nullptr);
    virtual bool init();
    
    HLTouchDeliverWidget(void);
	virtual ~HLTouchDeliverWidget(void);
    
    virtual void checkChildInfo(int handleState,cocos2d::ui::Widget* sender,const Point &touchPoint) override;
    
    CC_SYNTHESIZE(TouchDeliverCallback, m_callback, Callback);
    
    virtual void setOpacity(GLubyte opacity) override;
    
    
    CC_SYNTHESIZE(int, m_state, State);
    CC_SYNTHESIZE(Point, m_lastPoint, LastPoint);
    
    virtual void onExit();
};

#endif //__HLTouchDeliverWidget_H__
