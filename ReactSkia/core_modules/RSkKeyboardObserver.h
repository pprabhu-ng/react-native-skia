/*
* Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/

#include "core_modules/RSkEventEmitter.h"
#include "utils/RnsLog.h"

namespace facebook {
namespace react {

class RSkKeyboardObserver final: public RSkEventEmitter {
 public:
  RSkKeyboardObserver(
      const std::string &name,
      std::shared_ptr<CallInvoker> jsInvoker,
      Instance *bridgeInstance);

 private:
  unsigned int OSKEventId_ = UINT_MAX; // Some undefined ID
  void startObserving(); 
  void stopObserving(); 
  void KeyBoardEventHandler(std::string eventName);
  std::vector<std::string> events_ = {"keyboardWillShow","keyboardWillHide"};/*List of Events TBD*/

};

} //namespace react
} //namespace facebook