/*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "ReactSkia/components/RSkComponentView.h"

namespace facebook {
namespace react {

class RSkComponentTextInput final : public RSkComponentView {
 public:
  RSkComponentTextInput(const ShadowView &shadowView);
  bool onHandleKey(rnsKey  eventKeyType, rnsKeyAction eventKeyAction)override;
 protected:
  void OnPaint(SkCanvas *canvas) override;
 private:
  //bool stopPropagation;
  bool isInEditingMode;
};

} // namespace react
} // namespace facebook
