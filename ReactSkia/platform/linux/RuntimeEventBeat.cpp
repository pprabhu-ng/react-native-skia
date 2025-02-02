/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "RuntimeEventBeat.h"
#include "ReactSkia/utils/RnsLog.h"

#define BEAT_INTERVAL  10 /*unit ms. Beat interval Set to:10 milli sec as temp fix*/

namespace facebook {
namespace react {


/*Activity is dummy param to fit in React common implementation, It is not needed as
  RNS implementation is not based on runloop*/
RunLoopObserver::Activity activities{RunLoopObserver::Activity::BeforeWaiting};
RuntimeEventBeat::RuntimeEventBeat(RunLoopObserver::WeakOwner const &owner)
  : RunLoopObserver(activities, owner),beatThread_("BeatThread"),activities_{activities} {
    beatThread_.getEventBase()->runInEventBaseThread(std::bind(&RuntimeEventBeat::beat,this));
}

RuntimeEventBeat::~RuntimeEventBeat() {
  stopObserving();
  beatThread_.getEventBase()->terminateLoopSoon();
}

void RuntimeEventBeat::startObserving() const noexcept
{
  RNS_LOG_NOT_IMPL;
/* Start Beating once the Beat thread created*/
}

void RuntimeEventBeat::beat(){
  this->activityDidChange(activities_);
  RNS_LOG_DEBUG("!!!!!! [RunLoopObserver][Activity]see Timestamp !!!!! ");
  beatThread_.getEventBase()->scheduleAt(std::bind(&RuntimeEventBeat::beat,this), \
           std::chrono::steady_clock::now() + std::chrono::milliseconds(BEAT_INTERVAL));
}

void RuntimeEventBeat::stopObserving() const noexcept {
  this->disable();
}

bool RuntimeEventBeat::isOnRunLoopThread() const noexcept {
  return (beatThread_.getThreadId() == std::thread::id());
}

} // namespace react
} // namespace facebook
