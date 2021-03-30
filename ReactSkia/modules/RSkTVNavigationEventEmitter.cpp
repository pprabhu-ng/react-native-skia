#include "RSkTVNavigationEventEmitter.h"
#include <glog/logging.h>

namespace facebook {
namespace react {

RSkTVNavigationEventEmitter::RSkTVNavigationEventEmitter(
    const std::string &name, 
    std::shared_ptr<CallInvoker> jsInvoker,
    Instance *bridgeInstance)
    : RSkEventEmitter(name, jsInvoker, bridgeInstance)
{

}

void RSkTVNavigationEventEmitter::onTVKeyEvent(const char *eventType/*, const unsigned int tag*/) {
    folly::dynamic parameters = folly::dynamic::object();
    parameters["eventType"] = eventType;
    parameters["tag"] = 0xFFFF;
    // if (tag != 0xFFFF) {
    //     sendEventWithName(events_[0], folly::dynamic::array(eventType, tag));
    // } else {
        // 0xFFFF: invalid tag, ignore
        sendEventWithName(events_[0], folly::dynamic(parameters));
    //}
}

void RSkTVNavigationEventEmitter::startObserving() {
    // use the NotificationManager to register for TV Keys
    std::function<void(const char *)> handler = std::bind(&RSkTVNavigationEventEmitter::onTVKeyEvent, this, std::placeholders::_1);
    eventId_ = keyEventNotification.addListener(tvEventName_, handler);
}

void RSkTVNavigationEventEmitter::stopObserving() {
    keyEventNotification.removeListener(eventId_);
}

} //namespace react
} //namespace facebook