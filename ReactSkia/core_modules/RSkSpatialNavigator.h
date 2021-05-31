/*
* Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <array>
#include <memory>
#include <set>
#include <string>
#include <vector>

////#include "ReactSkia/utils/RnsLog.h"
////#include "ReactSkia/utils/RnsUtils.h"

#include "ReactSkia/components/RSkComponent.h"
#include "sdk/NotificationCenter.h"

namespace SpatialNavigator {

typedef std::vector<std::shared_ptr<facebook::react::Component>> CandidateList;

/*
 * This Class manages Spatial Navigation 
 * Manages a list of current set of navigatable components on Screen: navList_
 * Keeps track of the currently selected component: reference_
 * Keeps track of mutations on the Components to manage change in the navigatable set: add, remove, update
 * Listens to H/W Keyboard / RCU Keys (LURD Keys) to select a candidate to trigger focus / blur events
 * 
*/

class RSkSpatialNavigator {
    private:
    static RSkSpatialNavigator *instance_;

    // Currently focused component
    facebook::react::Component *reference_ = NULL;

    // List of Navigatable Components
    CandidateList navList_;

    // Private constructor: Singleton class
    RSkSpatialNavigator();

    unsigned int eventId_ = 0xFFF; // Set to undefined ID
    NotificationCenter keyEventNotification;
    void onTVKeyEvent(int keyEvent);
    void navigateInDirection(int keyEvent);

    public:
    static RSkSpatialNavigator *getInstance();

    void addToNavList(std::shared_ptr<facebook::react::Component> candidate);

    void removeFromNavList(std::shared_ptr<facebook::react::Component> candidate);

    void updateInNavList(std::shared_ptr<facebook::react::Component> candidate);
};

} // namespace SpatialNavigator