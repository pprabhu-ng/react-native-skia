/*
* Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/
#include "RSkInputEventManager.h"
#include "ReactSkia/components/RSkComponent.h"
#include "ReactSkia/sdk/RNSKeyCodeMapping.h"
namespace facebook {
namespace react {
//namespace RSkInputEventManager {
static unsigned int eventId_;
RSkInputEventManager* RSkInputEventManager::sharedInputEventManager_{nullptr};
RSkInputEventManager::RSkInputEventManager(){
    std::function<void(rnsKey, rnsKeyAction)> handler = std::bind(&RSkInputEventManager::keyHandler, this,
                                                                std::placeholders::_1, // rnsKey
                                                                std::placeholders::_2);
    eventId_ = NotificationCenter::defaultCenter().addListener("onHWKeyEvent", handler); // move this to input Event manager
    RNS_LOG_INFO("***** calling Constructor RSkInputEventManager    ");
}

void RSkInputEventManager::keyHandler(rnsKey eventKeyType, rnsKeyAction eventKeyAction){ // use this binding function. do it constructor
   
   RNS_LOG_INFO("***** calling Constructor  RSkInputEventManager::keyHandler    "); 
    auto spatialNavigator_ =  SpatialNavigator::RSkSpatialNavigator::sharedSpatialNavigator();
    auto currentFocused    = spatialNavigator_->getCurrentFocusElement();
    RNS_LOG_INFO("***** calling Constructor RSkInputEventManager    "<<currentFocused);    
    if(eventKeyAction != RNS_KEY_Press) // Need to act on keyPress only
       return;
	if(currentFocused){// if return  null component is not present. 
        if(currentFocused->onHandleKey(eventKeyType,eventKeyAction)){ // stopPropagating 
            return; 
        }   
    }
    spatialNavigator_->sendNotificationWithEventType(RNSKeyMap[eventKeyType],currentFocused ? currentFocused->getComponentData().tag : -1, eventKeyAction);
    spatialNavigator_->handleKeyEvent(eventKeyType,eventKeyAction);
}


RSkInputEventManager* RSkInputEventManager::getInputKeyEventManager(){
      //std::lock_guard<std::mutex> lock(mutex_);
    if (sharedInputEventManager_ == nullptr) {
        sharedInputEventManager_ = new RSkInputEventManager();
    }
     return sharedInputEventManager_;
}
//}//RSkInputEventManager
}//react
}//facebook
