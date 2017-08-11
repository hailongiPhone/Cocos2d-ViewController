//
//  NVConfig.h
//  TKPoker
//
//  Created by hailong on 13-10-15.
//
//

#ifndef TKPoker_NVConfig_h
#define TKPoker_NVConfig_h
#include <iostream>
#ifdef DEBUG1
#define NVLog()     \
do {                \
    std::cout<<"NVLog:::"<<__FUNCTION__<<">>>>>"<<__LINE__<<std::endl;   \
} while (0)
#else
#define NVLog(s, ...) do {} while (0)
#endif  //DEBUG


//Event Notification
#define kNVNTabBarSwitch         "switchTabBar"

#define NVNotification(__KEY__) \
do {\
    EventCustom event(__KEY__);\
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);\
}while (0)


#endif  //TKPoker_NVConfig_h