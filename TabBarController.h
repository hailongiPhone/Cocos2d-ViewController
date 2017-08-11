//
//  TabBarController.h
//  TKPoker
//
//  Created by hailong on 13-10-11.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//


/** TabBarController
 *
 *  footer使用的tabBar，维护一个数组，保存每个tabBaritem对应的NavigationConteroller
 *  
 *  主要维护当前显示的view，以及设置tabBarItem切换时是否要移除其他所有界面
 *  还要维护TabBarItem上的提示气泡，item特殊高亮
 */

#ifndef __TabBarController_H__
#define __TabBarController_H__

#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
USING_NS_CC_EXT;
#include "CocosGUI.h"

#include "ViewController.h"
#include "TabBarItem.h"

class NavigationController;
class TabBarController;



typedef std::function<void (NavigationController * transformedNC)> TransformCallBackNC;

class TabBarControllerDelegate {
public:
    //由于某些TabBar切换时，需移除其他所有rootview界面，
    //用代理回调的方式初始化rootview,可以自然合并初次加载和移除后重新加载rootview的代码
    virtual void TBCLoadRootView(int tabBarIndex,
                         NavigationController * navigationController){};
    
    //返回值 为true 表示使用默认的处理--切换组，返回false-- 表示暂不切换组
    virtual bool TBCItemTap(TabBarController * tabBarController,
                            int tabBarIndex){return true;};
                            
};


class TabBarController : public ViewController
{
    typedef ViewController super;
    
public:
    
#pragma mark -- Lifecycle --
    static TabBarController * create();
    virtual bool init();
    
    TabBarController(void);
	virtual ~TabBarController(void);
    
#pragma mark -- SetupView --
    void initItems(const TabBarItemList& items);
    void lazyUpdateNavigations(int count);
    void setupBarBackground(const char * fileName);
    
#pragma mark -- Action --
    void switchGroup(int newIndex,
                     bool hasAnimation = true);
    void setResetSelfWhenDismiss(int displayGroup);
    void setResetOtherWhenDisplay(int displayGroup);
    void resetAllGroups(bool hasAnimation = true,
                        TransformCallBackNC callback = nullptr);
    
    NavigationController * getCurrentGroup();
    NavigationController * getGroup(int groupIndex);
    int getCurGroupIndex();
    
    void setTabBarItemVisible(bool visible);
    
    void setDelegate(TabBarControllerDelegate* delegate);
    
    
    void setDisableGroup(int groupIndex);
    void setEnableGroup(int groupIndex);
    bool isEnableGroup(int groupIndex);
    
#pragma mark -- Event --
    void onTabBarItemTouch(TabBarItem * aItem, cocos2d::ui::Widget::TouchEventType type);
    void onTapEventBack(int index,
                        bool hasAnimation = false);
    void onTapEventPopToRoot(int index,
                        bool hasAnimation = false);
    
private:
    int m_curIndex;             //当前选中item序列
    int m_resetOtherGroups;     //加载这些group时其它界面需要移除  1<<index
    int m_resetSelfGroups;      //从这些group切换走时，移出自己
    bool isResetOtherGroup(int groupIndex);
    bool isResetSelfGroup(int groupIndex);
    
    int m_groupEnableState;     
    
    TabBarControllerDelegate * m_delegate;
    
#pragma mark -- SwitchGroup --
    void dismissGroup(int index,
                        bool hasAnimation = false,
                        TransformCallBackNC callback = nullptr);

    void displayGroup(int index,
                        bool hasAnimation = false);
    
    void setTabBarItemEnable(int index,bool enable);
public:
    CC_SYNTHESIZE_PASS_BY_REF(TabBarItemList, m_items, Items);
    CC_SYNTHESIZE_PASS_BY_REF(ViewControllerList, m_viewControllers, ViewControllers);
//    CC_SYNTHESIZE_RETAIN(Array *, m_items, Items);
//    CC_SYNTHESIZE_RETAIN(Array *, m_viewControllers, ViewControllers);
    CC_SYNTHESIZE_RETAIN(ui::ImageView *, m_barBackground, BarBackground);
};

#endif //__TabBarController_H__
