/*
* Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/
#include "RSkInputEventManager.h"
#include "ReactSkia/components/RSkComponent.h"

namespace facebook{
namespace react {
static unsigned int eventId_;
RSkInputEventManager* RSkInputEventManager::sharedInputEventManager_{nullptr};
RSkInputEventManager::RSkInputEventManager(){
    std::function<void(rnsKey, rnsKeyAction)> handler = std::bind(&RSkInputEventManager::keyHandler, this,
                                                                std::placeholders::_1, // rnsKey
                                                                std::placeholders::_2);
    eventId_ = NotificationCenter::defaultCenter().addListener("onHWKeyEvent", handler);
    spatialNavigator_ =  SpatialNavigator::RSkSpatialNavigator::sharedSpatialNavigator();
}

void RSkInputEventManager::keyHandler(rnsKey eventKeyType, rnsKeyAction eventKeyAction){
  /*when key pressed key press and key release events will be send.
   * in RNS only keypress is handled
   */
  if(eventKeyAction != RNS_KEY_Press)
    return;
  auto currentFocused = spatialNavigator_->getCurrentFocusElement();
  if(currentFocused){// if currentFocused is null, Key will is not handled.
    if(currentFocused->onHandleKey(eventKeyType)){ 
      /*onHandlekey return stopPropagating flag.
       *stopProgation is true, key is handled 
       *don't propagate. else propagate. similary 
       *like bubling.
       */
      return;
     }
  }
 sendNotificationWithEventType(
      RNSKeyMap[eventKeyType],
      currentFocused ? currentFocused->getComponentData().tag : -1,
      eventKeyAction);
  spatialNavigator_->handleKeyEvent(eventKeyType,eventKeyAction);
}

RSkInputEventManager* RSkInputEventManager::getInputKeyEventManager(){
    if (sharedInputEventManager_ == nullptr) {
        sharedInputEventManager_ = new RSkInputEventManager();
    }
    return sharedInputEventManager_;
}
void RSkInputEventManager::sendNotificationWithEventType(std::string eventType, int tag, rnsKeyAction keyAction) {
    if(eventType.c_str() == nullptr)
        return;
    RNS_LOG_DEBUG("Send : " << eventType  << " To ComponentTag : " << tag );
    NotificationCenter::defaultCenter().emit("RCTTVNavigationEventNotification",
                                                folly::dynamic(folly::dynamic::object("eventType", eventType.c_str())
                                                                              ("eventKeyAction", (int)keyAction)
                                                                              ("tag", tag)
                                                                              ("target", tag)
                                                                              ));
}

}//react
}//facebook
