/*
* Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/
#include "sdk/NotificationCenter.h"
#include "RSkKeyboardObserver.h"
#include "ReactSkia/utils/RnsLog.h"

using namespace std;

namespace facebook {
namespace react {

RSkKeyboardObserver::RSkKeyboardObserver(
    const std::string &name, 
    std::shared_ptr<CallInvoker> jsInvoker,
    Instance *bridgeInstance)
    : RSkEventEmitter(name, jsInvoker, bridgeInstance){}

void RSkKeyboardObserver::KeyBoardEventHandler(std::string eventName) {
RNS_LOG_INFO("OSKkeyHandler Received event: " << eventName);
#if !defined(GOOGLE_STRIP_LOG) || (GOOGLE_STRIP_LOG <= INFO)
   // for (auto& pair : paramObject.items()) {
   //   RNS_LOG_INFO("Notify RCTOSKEventNotification : { " << pair.first << " : " << pair.second << " }");
   // }
#endif
     sendEventWithName(eventName, nullptr);
}

void RSkKeyboardObserver::startObserving() {
    // Start observing RCTTVNavigationEventNotification using default NotificationManager
    RNS_LOG_INFO("Start observing RCTOSKEventNotification using default notification center");
    std::function<void(std::string)> KeyBoardEventHandler = std::bind(&RSkKeyboardObserver::KeyBoardEventHandler, this,
                                                                         std::placeholders::_1);  // folly::dynamic
    for(vector<string>::iterator events=events_.begin(); events!=events_.end(); ++events) {
       OSKEventId_ = NotificationCenter::OSKCenter().addListener(*events, KeyBoardEventHandler);
}   }

void RSkKeyboardObserver::stopObserving() {
    NotificationCenter::OSKCenter().removeListener(OSKEventId_);
}

} //namespace react
} //namespace facebook
