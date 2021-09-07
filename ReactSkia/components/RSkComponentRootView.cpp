#include "ReactSkia/components/RSkComponentRootView.h"

#include "include/core/SkPaint.h"
#include "react/renderer/components/root/RootShadowNode.h"

namespace facebook {
namespace react {

RSkComponentRootView::RSkComponentRootView(const ShadowView &shadowView)
    : RSkComponent(shadowView) {}
void RSkComponentRootView::updateComponatProps(const ShadowView &newShadowView,const ShadowView &oldShadowView){}
void RSkComponentRootView::OnPaint(
    SkCanvas *canvas) {
  canvas->clear(SK_ColorWHITE);
}

} // namespace react
} // namespace facebook
