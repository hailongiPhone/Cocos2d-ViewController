//
//  NavigationController.h
//  TKPoker
//
//  Created by hailong on 13-10-14.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//


/** NavigationController
 *
 *  <#Discription#>
 *  <#Discription#>
 *
 */

#ifndef __NavigationController_H__
#define __NavigationController_H__

#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "ViewController.h"
class TabBarItem;
//class NavigationController;

class NavigationController : public ViewController
{
    typedef ViewController super;
    
public:
    
#pragma mark -- Lifecycle --
    static NavigationController * create();
    virtual bool init();
    
    NavigationController(void);
	virtual ~NavigationController(void);

#pragma mark -- Action --
    virtual void push(ViewController * newVC,bool hasAnimation = true);
    virtual void pop(int count = 1,bool hasAnimation = true);
    virtual void pop();
    virtual void pop(bool hasAnimation);
    virtual void popAll(bool hasAnimation = true);
    
    void popToRootView(bool hasAnimation = true);
    
    void displayTopViewController(bool hasAnimation = true);
    void invisibleTopViewController(bool hasAnimation = true);
    
    ViewController * getRootViewController();
    
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
    
private:
    
#pragma mark -- Dismiss/Display --
    void invisibleViewController(ViewController * vc,
                                 bool hasAnimation = true,
                                 TransformCallBack callback = nullptr);
    void dismissViewController(ViewController * vc,
                               bool hasAnimation = true,
                               TransformCallBack callback = nullptr);
    
    void displayViewController(ViewController * vc,
                               bool hasAnimation = true);
    
    CC_SYNTHESIZE_PASS_BY_REF(ViewControllerList, m_viewControllers, ViewControllers);
    //CC_SYNTHESIZE_RETAIN(Array *, m_viewControllers, ViewControllers);
};

#endif //__NavigationController_H__
