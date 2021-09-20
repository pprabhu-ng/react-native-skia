#include "ReactSkia/components/RSkComponent.h"
#include "ReactSkia/components/RSkComponentImage.h"
#include "ReactSkia/Fabric/RSKConversion.h"

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
    , component_(shadowView) {}

RSkComponent::~RSkComponent() {}

void RSkComponent::onPaint(SkCanvas* canvas) {
  if(canvas) {
    RNS_PROFILE_API_OFF(component_.componentName << " Paint:", OnPaint(canvas));
  } else {
    RNS_LOG_ERROR("Invalid canvas ??");
  }
}

sk_sp<SkPicture> RSkComponent::getPicture() {

  SkPictureRecorder recorder;
  auto frame = getAbsoluteFrame();

  auto *canvas = recorder.beginRecording(SkRect::MakeXYWH(0, 0, frame.size.width, frame.size.height));

  if(canvas) {
    RNS_PROFILE_API_OFF("Recording " << component_.componentName << " Paint:", OnPaint(canvas));
  } else {
    RNS_LOG_ERROR("Invalid canvas ??");
    return nullptr;
  }

  return recorder.finishRecordingAsPicture();
}

void RSkComponent::requiresLayer(const ShadowView &shadowView) {
    RNS_LOG_TODO("Need to come up with rules to decide wheather we need to create picture layer, texture layer etc");
    // Text components paragraph builder is not compatabile with Picture layer,so use default layer
    if(strcmp(component_.componentName,"Paragraph") == 0)
        layer_ = this->shared_from_this();
    else
        layer_ = RnsShell::Layer::Create(RnsShell::LAYER_TYPE_PICTURE);
}
/*
void layerProps(const facebook::react::ShadowView &newShadowView,const facebook::react::ShadowView &oldShadowView){
      auto const &newviewProps = *std::static_pointer_cast<ViewProps const>(newShadowView.props);
      auto const &oldviewProps = *std::static_pointer_cast<ViewProps const>(oldShadowView.props);

 if(oldviewProps.opacity!=newviewProps.opacity){
	layer_.opacity=newviewProps.opacity;
   }
}
*/
void RSkComponent::updateProps(const ShadowView &newShadowView,const ShadowView &oldShadowView) {
      updateComponatProps(newShadowView,oldShadowView);
      auto const &newviewProps = *std::static_pointer_cast<ViewProps const>(newShadowView.props);
      auto const &oldviewProps = *std::static_pointer_cast<ViewProps const>(oldShadowView.props);
      auto const &viewProps = *std::static_pointer_cast<ViewProps const>(component_.props);
      float ratio=255.9999;
  //opacity
/*   if(oldviewProps.opacity!=newviewProps.opacity){
    layer_->opacity=newviewProps.opacity;
   }*/
     layerProps(newShadowView,oldShadowView);
  //ShadowOpacity
   if (oldviewProps.shadowOpacity!=newviewProps.shadowOpacity) {
      layer_->shadowOpacity=newviewProps.shadowOpacity;
   }
  //shadowRadius
   if (oldviewProps.shadowRadius!=newviewProps.shadowRadius) {
      layer_->shadowRadius=newviewProps.shadowRadius;
   }
  //shadowoffset
   if (oldviewProps.shadowOffset!=newviewProps.shadowOffset) {
      layer_->shadowOffset=RSkSkSizeFromSize(newviewProps.shadowOffset);
   }
  //shadowcolor
   if (oldviewProps.shadowColor!=newviewProps.shadowColor) {
      layer_->shadowColor =RSkColorConversion(newviewProps.shadowColor);
   }
  //backfaceVisibility
   if (oldviewProps.backfaceVisibility!=newviewProps.backfaceVisibility) {
      layer_->backfaceVisibility=(int)newviewProps.backfaceVisibility;
   }
  //backgroundColor
   if (oldviewProps.backgroundColor!=newviewProps.backgroundColor) {
      component_.props_.backgroundColor=RSkColorConversion(newviewProps.backgroundColor);
 }
  //foregroundColor
   if (oldviewProps.foregroundColor!=newviewProps.foregroundColor) {
      RNS_LOG_NOT_IMPL;
      component_.props_.foregroundColor=RSkColorConversion(newviewProps.foregroundColor);
  }
  /* TODO To be verified when implemented*/
  //pointerEvents
   if (oldviewProps.pointerEvents!=newviewProps.pointerEvents) {
      RNS_LOG_NOT_IMPL;
      component_.props_.pointerEvents=(int)newviewProps.pointerEvents;
   }
  //hitslop
   if (oldviewProps.hitSlop!=newviewProps.hitSlop) {
      RNS_LOG_NOT_IMPL;
      component_.props_.hitSlop=newviewProps.hitSlop;
   }
  //overflow
   if (oldviewProps.getClipsContentToBounds()!=newviewProps.getClipsContentToBounds()) {
      RNS_LOG_NOT_IMPL;//ViewProps.yogastyle.overflow
      component_.props_.masksToBounds_=newviewProps.getClipsContentToBounds();
   }
  //zIndex
   if (oldviewProps.zIndex!=newviewProps.zIndex) {
      component_.props_.zIndex = viewProps.zIndex.value_or(0);
   }
  //transform
   if (oldviewProps.transform != newviewProps.transform ) {
      layer_->transform = RSkTransform3DFromTransformMatrix(newviewProps.transform);
   }
}

void RSkComponent::updateComponentData(const ShadowView &newShadowView , const uint32_t updateMask) {
   if(updateMask & ComponentUpdateMaskProps) {
      component_.props = newShadowView.props;
      auto const &viewProps = *std::static_pointer_cast<ViewProps const>(component_.props);
      component_.props_.zIndex = viewProps.zIndex.value_or(0);
   }
   if(updateMask & ComponentUpdateMaskState)
      component_.state = newShadowView.state;
   if(updateMask & ComponentUpdateMaskEventEmitter)
      component_.eventEmitter = newShadowView.eventEmitter;
   if(updateMask & ComponentUpdateMaskLayoutMetrics) {
      component_.layoutMetrics = newShadowView.layoutMetrics;
      /* TODO : Analyze if this computation can be handled in RNS shell Layer */
      absOrigin_ =  parent_ ? (parent_->absOrigin_ + component_.layoutMetrics.frame.origin) : component_.layoutMetrics.frame.origin;

      Rect frame = component_.layoutMetrics.frame;
      SkIRect frameIRect = SkIRect::MakeXYWH(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
      if(layer() && layer().get())
        layer().get()->setFrame(frameIRect);
   }

   if(layer_ && layer_->type() == RnsShell::LAYER_TYPE_PICTURE) {
     RNS_PROFILE_API_OFF(component_.componentName << " getPicture :", static_cast<RnsShell::PictureLayer*>(layer_.get())->setPicture(getPicture()));
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
