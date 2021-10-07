/*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once
#include "ReactSkia/core_modules/RSkSpatialNavigator.h"
#include "react/renderer/components/textinput/iostextinput/TextInputEventEmitter.h"

namespace facebook {
namespace react {
 class RSkInputEventManager{
 private:
     static RSkInputEventManager *sharedInputEventManager_;
     RSkInputEventManager();
 public:
  static RSkInputEventManager* getInputKeyEventManager();
  void keyHandler(rnsKey key, rnsKeyAction eventKeyAction);
};

} // namespace react
} // namespace facebook
