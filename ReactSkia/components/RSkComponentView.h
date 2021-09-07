#pragma once

#include "ReactSkia/components/RSkComponent.h"

namespace facebook {
namespace react {

class RSkComponentView final : public RSkComponent {
 public:
  RSkComponentView(const ShadowView &shadowView);
  void updateComponatProps(const ShadowView &newShadowView,const ShadowView &oldShadowView) override;
 protected:
  void OnPaint(SkCanvas *canvas) override;
};

} // namespace react
} // namespace facebook
