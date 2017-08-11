//
//  TabBarController.cpp
//  TKPoker
//
//  Created by hailong on 13-10-11.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "TabBarController.h"
#include "TabBarItem.h"
#include "NavigationController.h"

#pragma mark -
#pragma mark -- define --

#define kTabBarIndexInvalid -1

enum TabBarControllerZOrder {
    kTabBarControllerZOrderBarBackground = ViewController::VCZOrderTop,
    kTabBarControllerZOrderItems,
};

#pragma mark -- Lifecycle --

TabBarController::TabBarController(void)
:m_curIndex(kTabBarIndexInvalid)
,m_resetOtherGroups(0)
,m_resetSelfGroups(0)
,m_barBackground(nullptr)
,m_groupEnableState(0)
{
    m_items.clear();
    m_viewControllers.clear();
}

TabBarController::~TabBarController(void)
{
    resetAllGroups(false);
    m_items.clear();
    m_viewControllers.clear();
    setBarBackground(nullptr);
}

TabBarController * TabBarController::create()
{
    TabBarController * pRet = new TabBarController();
    
    if (pRet && pRet->init()) {
        pRet->autorelease();
    }else{
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

bool TabBarController::init()
{
    if(!super::init()){
        return false;
    }
    
    return true;
}


#pragma mark -
#pragma mark -- SetupView --

void TabBarController::initItems(const TabBarItemList& items)
{
    setItems(items);
    lazyUpdateNavigations(items.size());
    
    TabBarItem * aItem = dynamic_cast<TabBarItem *>(items.at(0));
    
    CCASSERT(aItem != nullptr, "TabBarController::initItems");
    
    int count = items.size();
    
    float w = aItem->getSize().width;
    float space = (640- w * count)/(count + 1);
    float y = aItem->getSize().height * 0.5 + 5;
    float x = 0;
    
    for (int i = 0; i< count; i++) {
        aItem = dynamic_cast<TabBarItem *>(items.at(i));
        x = (i + 0.5) * w + (i + 1) * space;
        
        if (aItem) {
            aItem->setTag(i);
            aItem->setCallback([&,this](TabBarItem * aItem, cocos2d::ui::Widget::TouchEventType type){
                this->onTabBarItemTouch(aItem, type);
            });
            aItem->setPosition(Point(x, y));
            getView()->addChild(aItem);
            aItem->setZOrder(100 - i + kTabBarControllerZOrderItems);
        }
    }
}

void TabBarController::lazyUpdateNavigations(int count)
{
    if (!getViewControllers().empty()) {
        resetAllGroups();
    }
    
    m_viewControllers.clear();
    
    for (int i = 0; i<count; i++) {
        NavigationController * aTmp = NavigationController::create();
        m_viewControllers.pushBack(aTmp);
    }
    
}

void TabBarController::setupBarBackground(const char * fileName)
{
    if (!m_barBackground) {
        setBarBackground(ImageView::create());
        if(m_barBackground)
        {
            m_barBackground->setZOrder(kTabBarControllerZOrderBarBackground);
            getView()->addChild(m_barBackground);
        }
    }
    
    if (m_barBackground)
    {
        m_barBackground->loadTexture(fileName, ui::Widget::TextureResType::PLIST);
        m_barBackground->setAnchorPoint(Point(0.5,0));
        m_barBackground->setPositionPercent(Point(0.5, 0));
    }
    
    
}

#pragma mark -
#pragma mark -- Action --
int TabBarController::getCurGroupIndex()
{
    return m_curIndex;
}

void TabBarController::switchGroup(int newIndex,
                                   bool hasAnimation)
{
//    hasAnimation = false;
    if (m_curIndex == newIndex) {
        //点击返回到该组的前一个viewcontroller
        onTapEventPopToRoot(newIndex);
        return;
    }
    
    if (newIndex == kTabBarIndexInvalid ||
        newIndex >= m_viewControllers.size())
    {
        log("无效 index");
        return;
    }
    
    //移除NavigationController中的views（保留或者不保留rootview）
//    if (isResetSelfGroup(m_curIndex)) {
//        getCurrentGroup()->popAll(hasAnimation);
//    }
//    if (isResetOtherGroup(newIndex)) {
//        resetAllGroups(hasAnimation,[&,newIndex,hasAnimation,this](NavigationController * nc){
//            this->displayGroup(newIndex,hasAnimation);
//        });
//    }else
    bool isResetSelf = isResetSelfGroup(m_curIndex);
    bool isResetOther = isResetOtherGroup(newIndex);
    if (m_curIndex != kTabBarIndexInvalid){
        dismissGroup(m_curIndex,hasAnimation,[&,newIndex,hasAnimation,this,isResetSelf,isResetOther](NavigationController * nc){
            
            if (isResetSelf) {
                this->getCurrentGroup()->popAll(false);
            }
            if (isResetOther) {
                this->resetAllGroups(false);
            }
            this->displayGroup(newIndex,hasAnimation);
        });
    }else{
        this->displayGroup(newIndex,hasAnimation);
    }
    
    NVNotification(kNVNTabBarSwitch);
}

NavigationController * TabBarController::getCurrentGroup()
{
    return getGroup(m_curIndex);
}

NavigationController * TabBarController::getGroup(int groupIndex)
{
    assert(groupIndex < m_viewControllers.size() && groupIndex >= 0);
    
    NavigationController * nc =
    dynamic_cast<NavigationController *>(m_viewControllers.at(groupIndex));
    return nc;
}

void TabBarController::setResetSelfWhenDismiss(int displayGroup)
{
    CCASSERT((displayGroup >= 0 && displayGroup <= 10), "无效 displayGroup");
    int mask = 1<<displayGroup;
    m_resetSelfGroups |= mask;
}

void TabBarController::setResetOtherWhenDisplay(int displayGroup)
{
    CCASSERT((displayGroup >= 0 && displayGroup <= 10), "无效 displayGroup");
    int mask = 1<<displayGroup;
    m_resetOtherGroups |= mask;
}

void TabBarController::resetAllGroups(bool hasAnimation,
                                      TransformCallBackNC callback)
{
    if (m_viewControllers.empty()) {
        return;
    }
    
    int count = m_viewControllers.size();
    for (int i = 0; i<count; i++) {
        NavigationController * nc =
        dynamic_cast<NavigationController *>(m_viewControllers.at(i));
        nc->popAll(hasAnimation);
    }
    
    if (callback) {
        callback(nullptr);
    }
    //清零加载的图片资源
//    Director::getInstance()->purgeCachedData();
}

void TabBarController::setDelegate(TabBarControllerDelegate * delegate)
{
    m_delegate = delegate;
}

void TabBarController::setTabBarItemVisible(bool visible)
{
    if (m_items.empty()) {
        return;
    }
    
    int count = m_items.size();
    
    for (int i = 0; i< count; i++) {
        TabBarItem * aItem = dynamic_cast<TabBarItem *>(m_items.at(i));
        aItem->setEnabled(visible);
        aItem->setVisible(visible);
    }
}

void TabBarController::setDisableGroup(int groupIndex)
{
    int groupCount = getItems().size();
    if (groupIndex < groupCount) {
        m_groupEnableState |= (1<<groupIndex);
    }else{
        m_groupEnableState = 0xffff;
    }
    
    setTabBarItemEnable(groupIndex, false);
}

void TabBarController::setEnableGroup(int groupIndex)
{
    int groupCount = getItems().size();
    if (groupIndex < groupCount) {
        m_groupEnableState &= (~(1<<groupIndex));
    }else{
        m_groupEnableState = 0x0000;
    }
    
    setTabBarItemEnable(groupIndex, true);
}

bool TabBarController::isEnableGroup(int groupIndex)
{
    return m_groupEnableState & (1<<groupIndex);
}

#pragma mark -
#pragma mark -- Event --

void TabBarController::onTabBarItemTouch(TabBarItem * aItem, cocos2d::ui::Widget::TouchEventType type)
{
//    if (type !=ui::TOUCH_EVENT_ENDED) {
//        return;
//    }
    
    CCASSERT(m_viewControllers.size() > 0,
             "viewControllers未初始化");
    
    int tapIndex = aItem->getTag();
    CCASSERT((tapIndex >= 0 && tapIndex < m_viewControllers.size()),
             "无效的tapIndex");
    
    if (!m_delegate) {
        switchGroup(tapIndex);
    }else if(m_delegate &&
        m_delegate->TBCItemTap(this, tapIndex)){
        switchGroup(tapIndex);
    }
}

void TabBarController::onTapEventBack(int index,
                                      bool hasAnimation)
{
    CCASSERT((index>=0 && index<m_viewControllers.size()), "越界");
    NavigationController * nc =
    dynamic_cast<NavigationController *>(m_viewControllers.at(index));
   
    nc->pop(1);
//    nc->popToRootView();
}

void TabBarController::onTapEventPopToRoot(int index,
                         bool hasAnimation)
{
    CCASSERT((index>=0 && index<m_viewControllers.size()), "越界");
    NavigationController * nc =
    dynamic_cast<NavigationController *>(m_viewControllers.at(index));
    
    nc->popToRootView();
    
}



#pragma mark -
#pragma mark -- Private --

void TabBarController::setTabBarItemEnable(int index,bool enable)
{
    int count = getItems().size();
    TabBarItem *aItem = nullptr;
    
    if (index < count) {
        aItem = dynamic_cast<TabBarItem *>(m_items.at(index));
        if(aItem) aItem->setDisable(!enable);
        return;
    }
    
    for (int i = 0; i< count; i++) {
        aItem = dynamic_cast<TabBarItem *>(m_items.at(i));
        if(aItem) aItem->setDisable(!enable);
    }

}


#pragma mark SwitchGroup
void TabBarController::dismissGroup(int index,
                                      bool hasAnimation,
                                      TransformCallBackNC callback)
{
    CCASSERT((index>=0 && index<m_viewControllers.size()), "越界");
    NavigationController * nc =
    dynamic_cast<NavigationController *>(m_viewControllers.at(index));
    
    nc->invisible(hasAnimation,[&,this,callback](ViewController * vc){
        NavigationController * nc1 = dynamic_cast<NavigationController *>(vc);
        if (nc1) {
            nc1->popToRootView(false);
        }
        if (nc1 && callback) {
            callback(nc1);
        }
    });
}


void TabBarController::displayGroup(int index,bool hasAnimation)
{
    
    CCASSERT((index>=0 && index<m_viewControllers.size()), "越界");
    NavigationController * nc =
    dynamic_cast<NavigationController *>(m_viewControllers.at(index));
    
    nc->setNavigationController(this);
    
    if (!nc->getRootViewController() && m_delegate) {
        m_delegate->TBCLoadRootView(index, nc);
    }
    
    nc->display(getView(), this,hasAnimation);
    
    m_curIndex = index;
    
}

#pragma mark ResetGroup
bool TabBarController::isResetSelfGroup(int groupIndex)
{
    int mask = 1<<groupIndex;
    return m_resetSelfGroups & mask;
}
bool TabBarController::isResetOtherGroup(int groupIndex)
{
    CCASSERT((groupIndex >= 0 && groupIndex <= 10), "无效 displayGroup");
    int mask = 1<<groupIndex;
    return m_resetOtherGroups & mask;
}


