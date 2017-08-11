//
//  ButtonMenuView.h
//  TKPoker
//
//  Created by hailong on 13-10-29.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//


/** ButtonMenuView
 *
 *  按钮菜单- 依据设置的按钮多少，自动调整view的大小。
 *
 */

#ifndef __ButtonMenuView_H__
#define __ButtonMenuView_H__

#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
USING_NS_CC_EXT;
#include "CocosGUI.h"

typedef Vector<cocos2d::ui::Button*> ButtonList;

class ButtonMenuView : public ui::Layout
{
    typedef ui::Layout super;
    
public:
    
#pragma mark -- Lifecycle --
    static ButtonMenuView * create();
    virtual bool init();
    
    ButtonMenuView(void);
	virtual ~ButtonMenuView(void);
    
#pragma mark -- Action --
    void setupButtons(const ButtonList& buttons);
private:
#pragma mark -- SetupView --
    void resetView(const ButtonList& buttons);
#pragma mark -- Event --
    void onButtonTap(ui::Widget * sender, ui::Widget::TouchEventType type);
    ButtonList m_buttons;
//    CC_SYNTHESIZE_RETAIN(ButtonList *, m_buttons, Buttons);
};

#endif //__ButtonMenuView_H__
