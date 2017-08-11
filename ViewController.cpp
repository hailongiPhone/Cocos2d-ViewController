//
//  ViewController.cpp
//  TKPoker
//
//  Created by hailong on 13-10-11.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "ViewController.h"
#include "NavigationController.h"
#include "cocostudio/CocoStudio.h"
#include "ZXSound.h"
using namespace ui;

#include "ZXSizeAdapter.h"

#pragma mark -
#pragma mark -- Lifecycle --

float ViewController::g_designHeight = 960;
float ViewController::g_headerHeight = 160;

ViewController::ViewController(void)
:m_navigationController(nullptr)
,m_view(nullptr)
,m_presentedViewController(nullptr)
,m_presentingViewController(nullptr)
,m_ccsName("")
,m_csWidget(nullptr)
,m_state(HLVCStateDefault)
,m_isRunAction(false)
,m_defaultTableView(nullptr)
{
    NVLog();
}

ViewController::~ViewController(void)
{
    cocostudio::ActionManagerEx::getInstance()->removeActionByJsonName(getCCSName().c_str());
    
    NVLog();
    setView(nullptr);
    setNavigationController(nullptr);
    setPresentedViewController(nullptr);
    setPresentingViewController(nullptr);
    setCsWidget(nullptr);
    setDefaultTableView(nullptr);
}

bool ViewController::init()
{
    setView(cocos2d::ui::Layout::create());
    getView()->setSize(Director::getInstance()->getWinSize());
    getView()->setContentSize(Director::getInstance()->getWinSize());
    return true;
}

ViewController * ViewController::create()
{
    ViewController * pRet = new ViewController();
    
    if (pRet && pRet->init()) {
        pRet->autorelease();
    }else{
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

ViewController * ViewController::createWithCCS(const char * jsonName)
{
    ViewController * pRet = new ViewController();
    
    if (pRet && pRet->init()) {
        pRet->loadFromCCS(jsonName);
        pRet->autorelease();
    }else{
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

#pragma mark -- SetupView --
Widget* ViewController::loadFromCCS(const char * jsonName)
{
    Widget * tmp = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(jsonName);
    
    getView()->addChild(tmp);
    
    Size oSize = tmp->getSize();
    Size winSize = Director::getInstance()->getWinSize();
    float h = winSize.height - (g_designHeight - oSize.height);
    h = MIN(h, winSize.height);
    tmp->setSize(Size(oSize.width, h));
    
    float y = MAX(winSize.height - h - g_headerHeight,0);
    
    tmp->setAnchorPoint(Point(0,0));
    tmp->setPosition(Point(0, y));
    
    setCCSName(jsonName);
    setCsWidget(tmp);
    return tmp;
}

#pragma mark -
#pragma mark -- Action --
void ViewController::presentViewController(ViewController * nextView)
{
    if (!nextView) {
        return;
    }
    if (m_presentedViewController) {
        dismissPresentViewController();
    }
    
    nextView->getView()->setZOrder(VCZOrderPesent);
    getView()->getParent()->addChild(nextView->getView());
    setPresentedViewController(nextView);
    nextView->setPresentingViewController(this);
    nextView->setNavigationController(this->getNavigationController());
}

void ViewController::dismissPresentViewController()
{
    if (!m_presentedViewController) {
        return;
    }
    
    getView()->getParent()->removeChild(m_presentedViewController->getView());
    setPresentedViewController(nullptr);
}

#pragma mark -- display  dismiss invisible --
//隐藏是通过 setEnable 与 手动调用 onEnter onExit 实现

void ViewController::invisible(bool hasAnimation /*= true*/,
               TransformCallBack callback /*= nullptr*/)
{
    bool isRuning = getIsRuning();
    assert(isRuning);
    
    if (isRuning) {
        viewWillDisappear();
    }
    
    if (!hasAnimation) {
        getView()->onExit();
        getView()->setEnabled(false);
        if (isRuning) {
            viewDidDisappear();
        }
        if (callback) {
            callback(this);
        }
        this->setNavigationController(nullptr);
        return;
    }
    
    float duration = getTransitionDuration();
    
    auto transition = getTransitionActionDismiss(duration);
    
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    
    viewDisappearTransitionStart();
    setIsRunAction(true);
    getView()->runAction(Sequence::create(transition,CallFunc::create([&,isRuning,this,callback](){
        Director::getInstance()->getEventDispatcher()->setEnabled(true);
        this->viewDisappearTransitionEnd();
        
        this->getView()->onExit();

        getView()->setEnabled(false);
        
        if (isRuning) {
            this->viewDidDisappear();
        }
        this->setIsRunAction(false);
        if (callback) {
            callback(this);
        }
        this->setNavigationController(nullptr);
    }), nullptr));
}

void ViewController::dismiss(bool hasAnimation /*= true*/,
             TransformCallBack callback /*= nullptr*/)
{
    dismissPresentViewController();
    
    bool isRuning = getIsRuning();
    
    if (!isRuning) {
        getView()->removeFromParent();
        if (callback) {
            callback(this);
        }
        return;
    }
    
    if (isRuning) {
        viewWillDisappear();
    }
    
    if (!hasAnimation) {
        getView()->removeFromParent();
        if (isRuning) {
            viewDidDisappear();
        }
        if (callback) {
            callback(this);
        }
        setNavigationController(nullptr);
        return;
    }
    
    float duration = getTransitionDuration();
    auto transition = getTransitionActionDismiss(duration);
    
    Director::getInstance()->getEventDispatcher()->setEnabled(false);
    
    viewDisappearTransitionStart();
    setIsRunAction(true);
    getView()->runAction(Sequence::create(transition,CallFunc::create([&,isRuning, this,callback](){
        Director::getInstance()->getEventDispatcher()->setEnabled(true);
        this->viewDisappearTransitionEnd();
        
        this->getView()->removeFromParent();
        if (isRuning) {
            this->viewDidDisappear();
        }
        
        this->setIsRunAction(false);
        
        if (callback) {
            callback(this);
        }
        
        this->setNavigationController(nullptr);
    }), nullptr));
    
}

/*
 *  display
 *  显示Vc的前提是navigationController为空 或 
 *  navigationController 已经未显示，或者已显示
 *
 */
void ViewController::display(Node * parent ,
                             ViewController * navigationController,
                             bool hasAnimation /*= true*/,
                             TransformCallBack callback /*= nullptr*/)
{
    assert((navigationController == nullptr) ||
           navigationController->canDisplayViewController());
    
    setNavigationController(navigationController);
    if(navigationController == nullptr){
        viewDidLoad();
        viewWillAppear();
        parent->addChild(getView());
    }else if (isState(HLVCStateDidLoad)) {
        getView()->setEnabled(true);
        viewWillAppear();
        getView()->onEnter();
    }else{
        viewDidLoad();
        viewWillAppear();
        parent->addChild(getView());
    }
    
    if( hasAnimation ) {
        float duration  = getTransitionDuration();
        auto transition = getTransitionActionDisplay(duration);
        
        Director::getInstance()->getEventDispatcher()->setEnabled(false);
        viewAppearTransitionStart();
        
        setIsRunAction(true);
        
        getView()->runAction(Sequence::create(transition,CallFunc::create([&,this,callback](){
            
            Director::getInstance()->getEventDispatcher()->setEnabled(true);
            
            this->viewAppearTransitionEnd();
            this->viewDidAppear();
            
            this->setIsRunAction(false);
            
            if (callback) {
                callback(this);
            }
        }), nullptr));
    } else {
        getView()->setOpacity(255);
        this->viewDidAppear();
        
        if (callback) {
            callback(this);
        }
    }
    getView()->setEnabled(true);
}

#pragma mark -- convenient method --

ViewController * ViewController::getNavigationController() const
{
    return m_navigationController;
}

void ViewController::setNavigationController(ViewController * navCon)
{
    m_navigationController = navCon;
}

void ViewController::push(ViewController * nextView,bool hasAnimation /*true*/)
{
    if (m_navigationController) {
        m_navigationController->push(nextView,hasAnimation);
    }
}
void ViewController::pop()
{
    if (m_navigationController) {
        m_navigationController->pop();
        
    }
}

void ViewController::pop(Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
    if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
        SND->playSndTap();
        this->pop();
    }
}

#pragma mark -
#pragma mark -- Transitioning --
float ViewController::getTransitionDuration()
{
    return 0.33;
}

FiniteTimeAction * ViewController::getTransitionActionDisplay(float duration)
{
    getView()->setOpacity(0);
    return Sequence::create(FadeIn::create(duration), nullptr);
}

FiniteTimeAction * ViewController::getTransitionActionDismiss(float duration)
{
    getView()->setOpacity(255);
    return FadeOut::create(duration);
}

#pragma mark -
#pragma mark -- Event --
void ViewController::viewDidLoad()
{
    setState(HLVCStateDidLoad);
    NVLog();
}

void ViewController::viewWillAppear()
{
    assert(isState(HLVCStateDidLoad));
    setState(HLVCStateWillAppear);
    getView()->setVisible(true);
    NVLog();
}

void ViewController::viewDidAppear()
{
    assert(isState(HLVCStateWillAppear) || isState(HLVCStateAppearTransitionEnd));
    setState(HLVCStateDidAppear);
    getView()->setEnabled(true);
    
    NVLog();
}

void ViewController::viewWillDisappear()
{
    assert(isState(HLVCStateDidAppear));
    setState(HLVCStateWillDisappear);
    NVLog();
}

void ViewController::viewDidDisappear()
{
    assert(isState(HLVCStateWillDisappear) || isState(HLVCStateDisappearTransitionEnd));
    setState(HLVCStateDidDisappear);
    getView()->setEnabled(false);
    getView()->setVisible(false);
    NVLog();
}

#pragma mark -- Transition --
void ViewController::viewAppearTransitionStart()
{
    assert(isState(HLVCStateWillAppear));
    setState(HLVCStateAppearTransitionStart);
    
    if (getCCSName().length() < 1) {
        NVLog();
        return;
    }
    cocostudio::ActionManagerEx::getInstance()->playActionByName(getCCSName().c_str(), "Animation_enter");
    
    if(m_defaultTableView){
        m_defaultTableView->showCellAnimation(::getCreaterBlock(true));
    }
}

void ViewController::viewAppearTransitionEnd()
{
    assert(isState(HLVCStateAppearTransitionStart));
    setState(HLVCStateAppearTransitionEnd);
}

void ViewController::viewDisappearTransitionStart()
{
    assert(isState(HLVCStateWillDisappear));
    setState(HLVCStateDisappearTransitionStart);
    
    if (getCCSName().length() < 1) {
        NVLog();
        return;
    }
    cocostudio::ActionManagerEx::getInstance()->playActionByName(getCCSName().c_str(), "Animation_out");
    
    if(m_defaultTableView){
        m_defaultTableView->showCellAnimation(::getCreaterBlock(false));
    }

}

void ViewController::viewDisappearTransitionEnd()
{
    assert(isState(HLVCStateDisappearTransitionStart));
    setState(HLVCStateDisappearTransitionEnd);
}

#pragma mark -
#pragma mark -- State --
//状态
bool ViewController::getIsRuning(){
    return isState(HLVCStateDidLoad) && isState(HLVCStateDidAppear);
}

bool ViewController::isInvisible(){
    return (m_state == HLVCStateDidLoad) ||
    (m_state == (HLVCStateDidLoad | HLVCStateDidDisappear));
}

bool ViewController::canDisplayViewController(){
    return (m_state == HLVCStateDidLoad) ||
    (m_state == (HLVCStateDidLoad | HLVCStateDidAppear));
}

bool ViewController::isState(HLVCState state){
    return m_state & state;
}

void ViewController::setState(HLVCState state){
    m_state = (m_state & HLVCStateDidLoad) | state;
}