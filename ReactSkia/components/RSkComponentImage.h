#pragma once

#include "include/core/SkRect.h"

#include "react/renderer/mounting/ShadowView.h"

#include "ReactSkia/components/RSkComponent.h"

#define DEFAULT_IMAGE_FILTER_QUALITY kLow_SkFilterQuality /*Skia's Defualt is kNone_SkFilterQuality*/

namespace facebook {
namespace react {
struct ImageProps_t{
    ImageResizeMode resizeMode;
    SkColor tintColor;
};
class RSkComponentImage final : public RSkComponent {
 public:
  RSkComponentImage(const ShadowView &shadowView);
  void updateComponatProps(const ShadowView &newShadowView,const ShadowView &oldShadowView) override;
 private :
  ImageProps_t imgProps;
 protected:
  void OnPaint(SkCanvas *canvas) override;
};

} // namespace react
} // namespace facebook
