//
//  NavigationController.cpp
//  TKPoker
//
//  Created by hailong on 13-10-14.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "NavigationController.h"


#pragma mark -
#pragma mark -- lifecycle --

NavigationController::NavigationController(void)
{
    m_viewControllers.clear();
}

NavigationController::~NavigationController(void)
{
    popAll(false);
    m_viewControllers.clear();
}

NavigationController * NavigationController::create()
{
    NavigationController * pRet = new NavigationController();

    if (pRet && pRet->init()) {
        pRet->autorelease();
    }else{
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool NavigationController::init()
{
    if (!super::init()) {
        return false;
    }
    m_viewControllers.clear();
    
    return true;
}


#pragma mark -
#pragma mark -- Action --
void NavigationController::push(ViewController * newVC,bool hasAnimation /*= true*/)
{
    //CCASSERT(m_viewControllers, "无效的m_viewControllers");
    
    if (!getIsRuning()) {
        m_viewControllers.pushBack(newVC);
        if(isState(HLVCStateDidLoad)){
            newVC->viewDidLoad();
        }
        return;
    }
    
    ViewController * last = m_viewControllers.back();
    m_viewControllers.pushBack(newVC);
    if (last) {
        invisibleViewController(last,hasAnimation,[&,this,newVC,hasAnimation](ViewController * trasform){
            displayViewController(newVC,hasAnimation);
        });
    }else{
        displayViewController(newVC,hasAnimation);
    }
}

void NavigationController::pop(int count,bool hasAnimation /*= true*/)
{
    //CCASSERT(m_viewControllers, "无效的m_viewControllers");
    
    if (m_viewControllers.size() <= 1) {
        //移除presented viewcontroller
        ViewController * last = m_viewControllers.back();
        if (last) {
            last->dismissPresentViewController();
        }
        return;
    }
    
    ViewController * last = nullptr;
    count = MIN(count, m_viewControllers.size()-1);
    bool showTop = false;
    for (int i = 0; i<count; i++) {
        last = m_viewControllers.back();
        
        if (last) {
            if (hasAnimation) {
                showTop = true;
                dismissViewController(last,hasAnimation,[&,this](ViewController * transtormVC){
                    this->displayTopViewController();
                });
                hasAnimation = false;
            }else{
                dismissViewController(last,hasAnimation);
            }
        }
    }
    
    if (!showTop && canDisplayViewController()) {
        displayTopViewController(hasAnimation);
    }
//    else{
//        displayTopViewController();
//    }
    
//    last =
//    m_viewControllers.back();
//    if (last) {
//        displayViewController(last,hasAnimation);
//    }
}

void NavigationController::popAll(bool hasAnimation /*= true*/)
{
    ViewController * last = nullptr;
    int count = m_viewControllers.size();
    for (int i = 0; i<count; i++) {
        last = m_viewControllers.back();
        
        if (last) {
            dismissViewController(last,hasAnimation);
        }
    }
}

void NavigationController::pop()
{
    this->pop(1);
}

void NavigationController::pop(bool hasAnimation)
{
    this->pop(1,hasAnimation);
}

void NavigationController::popToRootView(bool hasAnimation)
{
    int count = m_viewControllers.size();
    pop(count - 1,hasAnimation);
}

ViewController * NavigationController::getRootViewController()
{
    if (m_viewControllers.size()<1) {
        return nullptr;
    }
    
    return dynamic_cast<ViewController *>(m_viewControllers.at(0));
}

#pragma mark -- Dismiss/Display --

void NavigationController::displayTopViewController(bool hasAnimation /*true*/)
{
    ViewController * top = m_viewControllers.back();
    if (top) {
        displayViewController(top,hasAnimation);
    }
}

void NavigationController::invisibleTopViewController(bool hasAnimation/* true*/)
{
    ViewController * top = m_viewControllers.back();
    if (top) {
        invisibleViewController(top,hasAnimation);
    }
}

void NavigationController::invisibleViewController(ViewController * vc,
                                                   bool hasAnimation,
                                                   TransformCallBack callback)
{
    vc->invisible(hasAnimation,callback);
}

void NavigationController::dismissViewController(ViewController * vc,
                                                 bool hasAnimation,
                                                 TransformCallBack callback)
{
    
    vc->retain();
    vc->dismiss(hasAnimation,[&,this,callback](ViewController * transtormVC){
        
        if (callback) {
            
            
            callback(transtormVC);
            
        }
        
        transtormVC->release();
    });
    
    this->m_viewControllers.eraseObject(vc);
}

void NavigationController::displayViewController(ViewController * vc,
                                                 bool hasAnimation)
{
    
    //    vc->setNavigationController(this);
    vc->display(this->getView(),this,hasAnimation);
}

#pragma mark -
#pragma mark -- Event --

void NavigationController::viewDidLoad()
{
    super::viewDidLoad();
    ViewController * top = m_viewControllers.back();
    if (top && !top->isState(HLVCStateDidLoad)) {
        top->viewDidLoad();
    }
    NVLog();
}

void NavigationController::viewWillAppear()
{
    super::viewWillAppear();
    NVLog();
    ViewController * top = m_viewControllers.back();
    if (top && !top->getIsRunAction()
        && !top->isState(HLVCStateWillAppear)
        && top->isInvisible()) {
        top->viewWillAppear();
        
        top->setNavigationController(this);
        if(!top->getView()->getParent()){
            getView()->addChild(top->getView());
        }
        top->getView()->setOpacity(255);
//        if (!top->getView()->isRunning()) {
//            top->getView()->onEnter();
//        }
        
    }
}

void NavigationController::viewAppearTransitionStart()
{
    super::viewAppearTransitionStart();
    
    ViewController * top =
    m_viewControllers.back();
    if (top && !top->getIsRunAction()
        && !top->isState(HLVCStateAppearTransitionStart)) {
        top->viewAppearTransitionStart();
    }
}

void NavigationController::viewAppearTransitionEnd()
{
    super::viewAppearTransitionEnd();
    
    ViewController * top =
    m_viewControllers.back();
    if (top && !top->getIsRunAction()
        && !top->isState(HLVCStateAppearTransitionEnd)) {
        top->viewAppearTransitionEnd();
    }
}

void NavigationController::viewDidAppear()
{
    super::viewDidAppear();
    NVLog();
    
    ViewController * top =
    m_viewControllers.back();
    if (top && !top->getIsRunAction()
        && !top->isState(HLVCStateDidAppear)) {
        top->viewDidAppear();
    }
}

void NavigationController::viewWillDisappear()
{
    super::viewWillDisappear();
    NVLog();

    ViewController * top =
    m_viewControllers.back();
    if (top && !top->getIsRunAction()
        && !top->isState(HLVCStateWillDisappear)) {
        top->viewWillDisappear();
    }
}

void NavigationController::viewDisappearTransitionStart()
{
    super::viewDisappearTransitionStart();
    
    ViewController * top =
    m_viewControllers.back();
    if (top && !top->getIsRunAction()
        && !top->isState(HLVCStateDisappearTransitionStart)) {
        top->viewDisappearTransitionStart();
    }
}

void NavigationController::viewDisappearTransitionEnd()
{
    super::viewDisappearTransitionEnd();
    
    ViewController * top =
    m_viewControllers.back();
    if (top && !top->getIsRunAction() && !top->isState(HLVCStateDisappearTransitionEnd)) {
        top->viewDisappearTransitionEnd();
    }
}


void NavigationController::viewDidDisappear()
{

    ViewController * top =
    m_viewControllers.back();
    if (top && !top->getIsRunAction()
        && !top->isState(HLVCStateDidDisappear)) {
        top->viewDidDisappear();
    }
    
    super::viewDidDisappear();
}