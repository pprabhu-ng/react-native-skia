#include "ReactSkia/components/RSkComponentView.h"
#include "include/core/SkPaint.h"
#include "react/renderer/components/view/ViewShadowNode.h"
#include "ReactSkia/views/common/RSkDrawUtils.h"
#include <glog/logging.h>

namespace facebook {
namespace react {

using namespace RSkDrawUtils;

RSkComponentView::RSkComponentView(const ShadowView &shadowView)
    : RSkComponent(shadowView) {}

void RSkComponentView::OnPaint(SkCanvas *canvas) {
  auto component = getComponentData();
  auto const &viewProps = *std::static_pointer_cast<ViewProps const>(component.props);
  /* apply view style props */
  auto borderMetrics=viewProps.resolveBorderMetrics(component.layoutMetrics);
  Rect frame = getAbsoluteFrame();
  /*Retrieve Shadow Props*/
  ShadowMetrics shadowMetrics{};
  shadowMetrics.shadowColor=viewProps.shadowColor;
  shadowMetrics.shadowOffset=viewProps.shadowOffset;
  shadowMetrics.shadowOpacity=viewProps.shadowOpacity;
  shadowMetrics.shadowRadius=viewProps.shadowRadius;

  if(viewProps.opacity >= 0.0) {
    Float opacity =  ( viewProps.opacity > 1.0 ) ? 255 : viewProps.opacity *255;
/*Draw Order : 1. Shadow 2. BackGround 3 Border*/
    canvas->saveLayerAlpha(nullptr, opacity);
    drawShadow(canvas,frame,borderMetrics,shadowMetrics);
    drawBackground(canvas,frame,borderMetrics,viewProps.backgroundColor);
    drawBorder(canvas,frame,borderMetrics,viewProps.backgroundColor);
    canvas->restore();
  }
}

} // namespace react
} // namespace facebook
