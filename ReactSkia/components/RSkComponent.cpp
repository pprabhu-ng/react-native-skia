#include "ReactSkia/components/RSkComponent.h"

#include "include/core/SkPaint.h"
#include "include/core/SkPictureRecorder.h"
#include "include/core/SkSurface.h"

#include "rns_shell/compositor/layers/PictureLayer.h"

namespace facebook {
namespace react {

RSkComponent::RSkComponent(const ShadowView &shadowView)
    : INHERITED(RnsShell::LAYER_TYPE_DEFAULT)
    , parent_(nullptr)
    , absOrigin_(shadowView.layoutMetrics.frame.origin)
    , component_(shadowView)
#ifdef RNS_ENABLE_API_PERF
    , componentName_(shadowView.componentName ? shadowView.componentName : "Rootview")
#endif
{
#ifdef RNS_ENABLE_API_PERF
    RNS_UNUSED(componentName_);
#endif
}

RSkComponent::~RSkComponent() {}

void RSkComponent::onPaint(SkSurface *surface) {
  if(surface) {
    auto canvas = surface->getCanvas();
    if(canvas)
        RNS_PROFILE_API_OFF(componentName_ << " Paint:", OnPaint(canvas));
  } else {
      RNS_LOG_ERROR("Invalid Surface ??");
  }
}

sk_sp<SkPicture> RSkComponent::getPicture() {

  SkPictureRecorder recorder;
  auto frameSize = getFrameSize();

  auto *canvas = recorder.beginRecording(SkRect::MakeXYWH(0, 0, frameSize.width, frameSize.height));

  if(canvas) {
    RNS_PROFILE_API_OFF("Recording " << componentName_ << " Paint:", OnPaint(canvas));
  } else {
    RNS_LOG_ERROR("Invalid canvas ??");
    return nullptr;
  }

  return recorder.finishRecordingAsPicture();
}

void RSkComponent::requiresLayer(const ShadowView &shadowView) {
    RNS_LOG_TODO("Need to come up with rules to decide wheather we need to create picture layer, texture layer etc");
    layer_ = RnsShell::Layer::Create(RnsShell::LAYER_TYPE_PICTURE);
}

void RSkComponent::updateComponentData(const ShadowView &newShadowView , const uint32_t updateMask) {
   if(updateMask & ComponentUpdateMaskProps)
      component_.props = newShadowView.props;
   if(updateMask & ComponentUpdateMaskState)
      component_.state = newShadowView.state;
   if(updateMask & ComponentUpdateMaskEventEmitter)
      component_.eventEmitter = newShadowView.eventEmitter;
   if(updateMask & ComponentUpdateMaskLayoutMetrics)
      component_.layoutMetrics = newShadowView.layoutMetrics;

   if(layer_ && layer_->type() == RnsShell::LAYER_TYPE_PICTURE) {
     RNS_PROFILE_API_OFF(componentName_ << " getPicture :", static_cast<RnsShell::PictureLayer*>(layer_.get())->setPicture(getPicture()));
   }
}

void RSkComponent::mountChildComponent(
    std::shared_ptr<RSkComponent> newChildComponent,
    const int index) {

    if(newChildComponent) {
        newChildComponent->parent_ = this;
        newChildComponent->absOrigin_ =  absOrigin_ + newChildComponent->component_.layoutMetrics.frame.origin;
    }

    RNS_LOG_ASSERT((this->layer_ && newChildComponent->layer_), "Layer Object cannot be null");
    if(this->layer_)
        this->layer_->insertChild(newChildComponent->layer_, index);
}

void RSkComponent::unmountChildComponent(
    std::shared_ptr<RSkComponent> oldChildComponent,
    const int index) {

    if(oldChildComponent) {
        oldChildComponent->parent_ = nullptr ;
        oldChildComponent->absOrigin_ = oldChildComponent->component_.layoutMetrics.frame.origin;
    }

    RNS_LOG_ASSERT((this->layer_ && oldChildComponent->layer_), "Layer Object cannot be null");
    if(this->layer_)
        this->layer_->removeChild(oldChildComponent->layer_, index);
}

} // namespace react
} // namespace facebook
