#pragma once

#include "ReactSkia/components/RSkComponent.h"

namespace facebook {
namespace react {

class RSkComponentRootView final : public RSkComponent {
 public:
  RSkComponentRootView(const ShadowView &shadowView);
  void updateComponatProps(const ShadowView &newShadowView,const ShadowView &oldShadowView) override;
 protected:
  void OnPaint(SkCanvas *canvas) override;
};

} // namespace react
} // namespace facebook
