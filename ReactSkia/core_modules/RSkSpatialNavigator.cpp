/*
* Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/
#include <string>
#include <algorithm>

// TO DO: Add newly created Key IDs and import the file

#include "RSkSpatialNavigator.h"

namespace SpatialNavigator {

RSkSpatialNavigator::RSkSpatialNavigator() {
        // Setup RCU / Keyboard events
        std::string hwKeyEventName = "HWKeyEvent";
        std::function<void(int)> handler = std::bind(&RSkSpatialNavigator::onTVKeyEvent, this, std::placeholders::_1);
        eventId_ = keyEventNotification.addListener(hwKeyEventName, handler);
}

RSkSpatialNavigator *RSkSpatialNavigator::getInstance() {
    if (instance_ == NULL) {

        instance_ = new RSkSpatialNavigator();
    }

    return instance_;
}

/*
 * Add a new candidate to the Navigatable if it meets the following criteria:
 * isTVSelectable, accessible are true
 * TO DO: Check if the above list has a priority order OR interdependency 
*/
void RSkSpatialNavigator::addToNavList(std::shared_ptr<facebook::react::Component> candidate) {
#if 0
    if (candidate->accessible == true ||
    candidate->isTVSelectable == true ) {
#else
    if(1)
#endif
        // Check if a method is more appropriate to update values of candidate?
        //candidate->centerX = candidate->layoutMetrics.frame.origin.x + (candidate->layoutMetrics.frame.size.width / 2);
        //candidate->centerY = candidate->layoutMetrics.frame.origin.y + (candidate->layoutMetrics.frame.size.height / 2);

        candidate->rightX = candidate->layoutMetrics.frame.origin.x + candidate->layoutMetrics.frame.size.width;
        candidate->bottomY = candidate->layoutMetrics.frame.origin.y + candidate->layoutMetrics.frame.size.height;

        navList_.push_back(candidate);

        // Trigger required focus / blur events if the  
        if (candidate->props.hasTVPreferredFocus == true) {
            // Check and trigger blur for the currently selected candidate
            if (reference_ != NULL) {
                // TO DO: Add code for triggering onBlur event
            }

            reference_ = candidate;
            // TO DO: Add code for triggering onFocus event

        }
    }
}

void RSkSpatialNavigator::removeFromNavList(std::shared_ptr<facebook::react::Component> candidate) {
    if (candidate->accessible == true ||
    candidate->isTVSelectable == true ) {
        CandidateList::iterator it;

        it = std::find(navList_.begin(), navList_.end(), candidate);

        if (it != navList_.end()) {
            // Reset data if candidate being removed is currently focused
            if (reference_ == candidate) {
                reference_ = NULL;
            }

            navList_.erase(it);
        }
    }
}

void RSkSpatialNavigator::updateInNavList(std::shared_ptr<facebook::react::Component> candidate) {
    CandidateList::iterator it;

    it = std::find(navList_.begin(), navList_.end(), candidate);

    if (it != navList_.end()) {
        // Candidate found in the navigatable list
        // Reset data if candidate's focusable props have changed
        // TO DO: Check the interdependency / priority of the props below
        if (candidate->accessible == false ||
        candidate->isTVSelectable == false ) {
            if (reference_ == candidate) {
                reference_ = NULL;
            }

            navList_.erase(it);
        } else {
            // Just update the position info
            // Check if a method is more appropriate to update values of candidate?
            //candidate->centerX = candidate->layoutMetrics.frame.origin.x + (candidate->layoutMetrics.frame.size.width / 2);
            //candidate->centerY = candidate->layoutMetrics.frame.origin.y + (candidate->layoutMetrics.frame.size.height / 2);

            candidate->rightX = candidate->layoutMetrics.frame.origin.x + candidate->layoutMetrics.frame.size.width;
            candidate->bottomY = candidate->layoutMetrics.frame.origin.y + candidate->layoutMetrics.frame.size.height;
        }
    } else {
        // Candidate not found in the current list
        // Call add method to check if one of important props (isTVNavigatable, accessible) might have changed
        addToNavList(candidate);
    }
}

void RSkSpatialNavigator::compareX(std::shared_ptr<facebook::react::Component> candidate1, std::shared_ptr<facebook::react::Component> candidate2) {
    return (candidate1->layoutMetrics.frame.origin.x <= candidate2->layoutMetrics.frame.origin.x)    
}

void RSkSpatialNavigator::compareY(std::shared_ptr<facebook::react::Component> candidate1, std::shared_ptr<facebook::react::Component> candidate2) {
    return (candidate1->layoutMetrics.frame.origin.y <= candidate2->layoutMetrics.frame.origin.y)    
}

void RSkSpatialNavigator::compareArea(std::shared_ptr<facebook::react::Component> candidate1, std::shared_ptr<facebook::react::Component> candidate2) {
    return ((candidate1->layoutMetrics.frame.size.width * candidate1->layoutMetrics.frame.size.height) <= 
            (candidate2->layoutMetrics.frame.size.width * candidate2->layoutMetrics.frame.size.height))    
}

void RSkSpatialNavigator::navigateInDirection(int keyEvent) {
    // TO DO: Scenario where reference_ is NULL
    NotificationCenter tvNotification;
    std::string tvNavigationEventName = "RCTTVNavigationEventNotification";
    CandidateList canList; // Filtered list of candidates in a direction
    CandidateList overlappingCanList; // List of candidates whose body has a projected overlap with reference in a direction
    CandidateList sameXOverlappingCanList; // List of candidates closest to reference in a direction on X plane
    CandidateList sameYOverlappingCanList; // List of candidates closest to reference in a direction on Y plane

    switch(keyEvent) {
        case KEY_TYPE_RIGHT:
            // Step 1: Filter out a list of candidates from navList_ whose x is greater than reference_'s x
            for (auto candidate = navList_.begin(); candidate != navList_.end(); candidate++) {
                if ( candidate != reference_ && 
                (*candidate)->layoutMetrics.frame.origin.x >= reference_->layoutMetrics.frame.origin.x) {
                    canList.push_back(candidate);
                }
            }

            if (canList.empty() == true) {
                // no more candidates to navigate to
                return;
            }

            // Step 2: Filter the candidates (canList) whose body has a projected overlap in Eastern region of the reference
            for (auto candidate = canList.begin(); candidate != canList.end(); candidate++) {
                if ( (candidate != reference_) && 
                !((*candidate)->bottomY <= reference_->layoutMetrics.frame.origin.y && 
                (*candidate)->layoutMetrics.frame.origin.y > reference_->bottomY)
                ) {
                    overlappingCanList.push_back(candidate);
                }
            }

            // Step 3: If no projected overlap in Eastern direction, no possible candidates for navigation: return 
            if (overlappingCanList.empty() == true) {
                // no more candidates to navigate to
                return;
            }

            // Step 4: Find the closest candidate (X) out of the overlappingCanList to the reference
            // there may be multiple, if many candidates with same X
            sort(overlappingCanList.begin(), overlappingCanList.end(), compareX);
            // filter components which have the same X
            static std::shared_ptr<facebook::react::Component> prevCandidate = NULL;
            
            for (auto candidate = overlappingCanList.begin(), prevCandidate = candidate; candidate != overlappingCanList.end(); candidate++) {
                // skip the first one, to start comparison from next
                if (prevCandidate == candidate) {
                    sameXOverlappingCanList.push_back(candidate);
                    continue;
                }

                if ( (*prevCandidate)->layoutMetrics.frame.origin.x == (*candidate)->layoutMetrics.frame.origin.x) {
                    sameXOverlappingCanList.push_back(candidate);
                } else {
                    // break out of the loop
                    break;
                }
                prevCandidate = candidate;
            }

            // Step 5: If only one candidate left, found the one
            if (sameXOverlappingCanList.size() == 1) {
                // Trigger blur event
                if (reference_ != NULL) {
                    reference_->onBlur();
                    // Trigger TV navigation "blur" event
                    tvNotification.emit(tvNavigationEventName, "blur", reference_->tag);
                }
                
                // Trigger focus event
                reference_ = sameXOverlappingCanList[0];
                reference_->onFocus();
                // Trigger TV navigation "focus" event
                tvNotification.emit(tvNavigationEventName, "focus", reference_->tag);
            } else {
                // Step 6: More than one candidates with same X: Select the one with the lowest Y
                // there may be multiple, if many candidates with same Y
                sort(sameXOverlappingCanList.begin(), sameXOverlappingCanList.end(), compareY);
                for (auto candidate = sameXOverlappingCanList.begin(), prevCandidate = candidate; candidate != sameXOverlappingCanList.end(); candidate++) {
                    // skip the first one, to start comparison from next
                    if (prevCandidate == candidate) {
                        sameYOverlappingCanList.push_back(candidate);
                        continue;
                    }

                    if ( (*prevCandidate)->layoutMetrics.frame.origin.x == (*candidate)->layoutMetrics.frame.origin.x) {
                        sameYOverlappingCanList.push_back(candidate);
                    } else {
                        // break out of the loop
                        break;
                    }
                    prevCandidate = candidate;
                }    
                // Step 7: If only one candidate left, found the one
                if (sameYOverlappingCanList.size() == 1) {
                    // Trigger blur event
                    if (reference_ != NULL) {
                        reference_->onBlur();
                        // Trigger TV navigation "blur" event
                        tvNotification.emit(tvNavigationEventName, "blur", reference_->tag);
                    }
                    
                    // Trigger focus event
                    reference_ = sameYOverlappingCanList[0];
                    reference_->onFocus();
                    // Trigger TV navigation "focus" event
                    tvNotification.emit(tvNavigationEventName, "focus", reference_->tag);
                } else {
                    // Step 8: More than one candidates with same Y: Select the candidate with the lowest area
                    // find the first one: TO DO: if area is same, look for zIndex???
                    // Sort the sameYOverlappingCanList based on the area (lowest to highest)
                    sort(sameYOverlappingCanList.begin(), sameYOverlappingCanList.end(), compareArea);

                    // Trigger blur event
                    if (reference_ != NULL) {
                        reference_->onBlur();
                        // Trigger TV navigation "blur" event
                        tvNotification.emit(tvNavigationEventName, "blur", reference_->tag);
                    }
                    
                    // Trigger focus event
                    reference_ = sameYOverlappingCanList[0];
                    reference_->onFocus();
                    // Trigger TV navigation "focus" event
                    tvNotification.emit(tvNavigationEventName, "focus", reference_->tag);
                }
            }
        break;
    }
}

/*
 * Only manage keys: select, up, down, left, right
*/
void RSkSpatialNavigator::onTVKeyEvent(int keyEvent) {
    // TO DO: Use Key IDs (not string) upto this point for faster operations
    std::string eventName = "RCTTVNavigationEventNotification";

    if (keyEvent == RNS_KEY_SELECT) {
        // Send select event on the currntly focused event
        keyNotification.emit(eventName, keyEvent, reference_ !== NULL ? reference_.tag : 0xFFFF);
        return;
    }

    switch(keyEvent) {
        case RNS_KEY_SELECT:
            // Send select event on the currntly focused event
            keyNotification.emit(eventName, "select", reference_ !== NULL ? reference_.tag : 0xFFFF);
        break;
        case KEY_TYPE_UP:
        case KEY_TYPE_DOWN:
        case KEY_TYPE_LEFT:
        case KEY_TYPE_RIGHT:
            navigateInDirection(keyEvent);
        break;
    }

    return;
}

} // namespace SpatialNavigator