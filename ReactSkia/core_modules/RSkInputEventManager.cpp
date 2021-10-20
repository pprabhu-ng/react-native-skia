/*
* Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/
#include "RSkInputEventManager.h"
#include "ReactSkia/components/RSkComponent.h"
#include "ReactSkia/sdk/RNSKeyCodeMapping.h"

namespace facebook{
namespace react {
static unsigned int eventId_;
RSkInputEventManager* RSkInputEventManager::sharedInputEventManager_{nullptr};
RSkInputEventManager::RSkInputEventManager(){
    std::function<void(rnsKey, rnsKeyAction)> handler = std::bind(&RSkInputEventManager::keyHandler, this,
                                                                std::placeholders::_1, // rnsKey
                                                                std::placeholders::_2);
    eventId_ = NotificationCenter::defaultCenter().addListener("onHWKeyEvent", handler); // move this to input Event manager
    spatialNavigator_ =  SpatialNavigator::RSkSpatialNavigator::sharedSpatialNavigator(); // crreate private member. 
}

void RSkInputEventManager::keyHandler(rnsKey eventKeyType, rnsKeyAction eventKeyAction){ // use this binding function. do it constructor 
    RNS_LOG_INFO(" calling Constructor  RSkInputEventManager::keyHandler    ");      
    if(eventKeyAction != RNS_KEY_Press) // Need to act on keyPress only
        return; 
    auto currentFocused = spatialNavigator_->getCurrentFocusElement();
    if(currentFocused){// if currentFocused is null, there no component to be focused. 
        if(currentFocused->onHandleKey(eventKeyType)){ 
            /*onHandlekey retuen stopPropagating flag
             * stopProgation is true, say key is handled 
             *no need to propagate. else key has to propagate  
             */
           return;
       }
    }
    spatialNavigator_->sendNotificationWithEventType(RNSKeyMap[eventKeyType],currentFocused ? currentFocused->getComponentData().tag : -1, eventKeyAction);
    spatialNavigator_->handleKeyEvent(eventKeyType,eventKeyAction);
}

RSkInputEventManager* RSkInputEventManager::getInputKeyEventManager(){
    if (sharedInputEventManager_ == nullptr) {
        sharedInputEventManager_ = new RSkInputEventManager();
    }
    return sharedInputEventManager_;
}

}//react
}//facebook
