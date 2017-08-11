//
//  ViewController.h
//  TKPoker
//
//  Created by hailong on 13-10-11.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//


/** ViewController
 *
 *  view控制器，主要用于和NavigationController维护views堆栈。
 *  有该viewController所在的navigationController的引用--方便view controller push，pop
 *  维护View加载，将要显示，隐藏 的事件
 */

#ifndef __ViewController_H__
#define __ViewController_H__

#include "cocos2d.h"

#include "cocos-ext.h"
USING_NS_CC_EXT;
#include "NVConfig.h"

#include "CocosGUI.h"
#include "HLTableView.h"

using namespace cocos2d;

class NavigationController;
class ViewController;

typedef std::function<void (ViewController * transformedVC)> TransformCallBack;

class ViewController : public Ref
{
    
public:
    
    enum VCZOrder {
        VCZOrderDefault = 0,
        VCZOrderMiddle  = 100,
        VCZOrderPesent  = 500,
        VCZOrderTop     = 1000,
    };
    
    enum HLVCState {
        HLVCStateDefault        = 0,
        
        HLVCStateDidLoad        = 1<<0,
        
        HLVCStateWillAppear                 = 1<<1,
        HLVCStateAppearTransitionStart      = 1<<2,
        HLVCStateAppearTransitionEnd        = 1<<3,
        HLVCStateDidAppear                  = 1<<4,
        
        HLVCStateWillDisappear                 = 1<<5,
        HLVCStateDisappearTransitionStart      = 1<<6,
        HLVCStateDisappearTransitionEnd        = 1<<7,
        HLVCStateDidDisappear                  = 1<<8,
        
    };
    
#pragma mark -- LifeCycle --
    static ViewController * create();
    static ViewController * createWithCCS(const char * jsonName);
    ViewController(void);
	virtual ~ViewController(void);
    virtual bool init();
        
#pragma mark -- SetupView --
    cocos2d::ui::Widget* loadFromCCS(const char * jsonName);
    
#pragma mark -- Event --
    virtual void viewDidLoad();
    
    virtual void viewWillAppear();
    virtual void viewAppearTransitionStart();
    virtual void viewAppearTransitionEnd();
    virtual void viewDidAppear();
    
    virtual void viewWillDisappear();
    virtual void viewDisappearTransitionStart();
    virtual void viewDisappearTransitionEnd();
    virtual void viewDidDisappear();
    
#pragma mark -- Transition --
    virtual float getTransitionDuration();
    virtual FiniteTimeAction * getTransitionActionDisplay(float duration);
    virtual FiniteTimeAction * getTransitionActionDismiss(float duration);
    
#pragma mark -- Action --
    virtual void presentViewController(ViewController * nextView);
    virtual void dismissPresentViewController();
   
    ViewController * getNavigationController() const;
    void setNavigationController(ViewController * navCon);
    
    static float  g_designHeight;
    static float  g_headerHeight;
    
    virtual void invisible(bool hasAnimation = true,
                           TransformCallBack callback = nullptr);
    virtual void dismiss(bool hasAnimation = true,
                         TransformCallBack callback = nullptr);
    
    virtual void display(Node * parent ,
                         ViewController * navigationController,
                         bool hasAnimation = true,
                         TransformCallBack callback = nullptr);
    
#pragma mark -- convenient method --
    virtual void push(ViewController * nextView,bool hasAnimation = true);
    virtual void pop();
    virtual void pop(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
    virtual ui::Widget *seekWidgetByName(const char *name)
    {
        return ui::Helper::seekWidgetByName(this->getView(), name);
    }
    
private:
    ViewController * m_navigationController;
    
    CC_SYNTHESIZE(ViewController *,
                  m_presentingViewController,
                  PresentingViewController);
    
    CC_SYNTHESIZE_RETAIN(ViewController *,
                         m_presentedViewController,
                         PresentedViewController);
    
    CC_SYNTHESIZE_RETAIN(cocos2d::ui::Layout *, m_view, View);
    
    //状态
    bool getIsRuning();
    bool isInvisible();
    bool canDisplayViewController();
    
    bool isState(HLVCState state);
    void setState(HLVCState state);    //状态
    int m_state;
    
    //加载ccs
    CC_SYNTHESIZE(bool, m_isRunAction, IsRunAction);
    CC_SYNTHESIZE(std::string, m_ccsName, CCSName);
    CC_SYNTHESIZE_RETAIN(cocos2d::ui::Widget *, m_csWidget, CsWidget);
    
    //方便tabelView动画
    CC_SYNTHESIZE(HLTableView *, m_defaultTableView, DefaultTableView);
};

class TabBarItem;
typedef Vector<ViewController*> ViewControllerList;
typedef Vector<TabBarItem *> TabBarItemList;

#endif //__ViewController_H__
