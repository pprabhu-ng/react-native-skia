#include "include/core/SkPaint.h"
#include "include/core/SkClipOp.h"
#include "include/core/SkImageFilter.h"
#include "include/effects/SkImageFilters.h"

#include "rns_shell/compositor/layers/PictureLayer.h"

#include "react/renderer/components/image/ImageEventEmitter.h"

#include "ReactSkia/components/RSkComponentImage.h"
#include "ReactSkia/views/common/RSkDrawUtils.h"
#include "ReactSkia/views/common/RSkImageUtils.h"

#include "ReactSkia/utils/RnsLog.h"
#include "ReactSkia/utils/RnsUtils.h"
#include "ReactSkia/views/common/RSkConversion.h"

namespace facebook {
namespace react {

using namespace RSkDrawUtils;
using namespace RSkImageUtils;
using namespace ImageCacheManager;

sk_sp<SkImage> getImage(ImageSource source,struct RemoteImageData* remoteImageData) {

  std::string path;
  if ( source.uri.empty() ) {
    return nullptr;
  }
  if(source.uri.substr(0, 14) == "file://assets/") {
    path = "./" + source.uri.substr(7);
  }else {
    path = source.uri.c_str();
  }
  RNS_PROFILE_START(getImageData)
  //RSkImageCacheManager &cacheInstance = RSkImageCacheManager::getInstance();
  sk_sp<SkImage> imageData = RSkImageCacheManager::getInstance().getImageData(path.c_str(),std::move(remoteImageData));
  RNS_PROFILE_END(path.c_str(),getImageData)
  if(!imageData) {
    RNS_LOG_ERROR("Draw Image Failed :" << path);
  }
  #ifdef RNS_IMAGE_CACHE_USAGE_DEBUG
    printCacheUsage();
  #endif //RNS_IMAGECACHING_DEBUG
  return imageData;
}

RSkComponentImage::RSkComponentImage(const ShadowView &shadowView)
    : RSkComponent(shadowView) {}

void RSkComponentImage::OnPaint(
    SkCanvas *canvas) {
  auto component = getComponentData();
  auto const &imageProps =
      *std::static_pointer_cast<ImageProps const>(component.props);
  RNS_LOG_DEBUG(__func__<<" files instance :"<<this);
  struct RemoteImageData* remoteImageData(new struct RemoteImageData);
  remoteImageData->callback = std::bind(&RSkComponentImage::remoteImageDataCallback, this,
                                                              std::placeholders::_1,
                                                              std::placeholders::_2,
                                                              std::placeholders::_3,
                                                              std::placeholders::_4);

  if (!imageProps.sources.empty() && (imageProps.sources[0].type == ImageSource::Type::Local ||
        imageProps.sources[0].type == ImageSource::Type::Remote)) {
    if(!imageData_)
      imageData_=getImage(imageProps.sources[0],std::move(remoteImageData));
  }

  auto imageEventEmitter = std::static_pointer_cast<ImageEventEmitter const>(component.eventEmitter);
  /* Emitting Load completed Event*/
  imageEventEmitter->onLoad();
  Rect frame = component.layoutMetrics.frame;
  SkRect frameRect = SkRect::MakeXYWH(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
  auto const &imageBorderMetrics=imageProps.resolveBorderMetrics(component.layoutMetrics);
  /* Draw order 1.Shadow 2. Background 3.Image Shadow 4. Image 5.Border*/
  bool contentShadow = false;
  bool needClipAndRestore =false;
  if(layer()->shadowOpacity && layer()->shadowFilter){
    contentShadow=drawShadow(canvas,frame,imageBorderMetrics,imageProps.backgroundColor,layer()->shadowOpacity,layer()->shadowFilter);
  }
  drawBackground(canvas,frame,imageBorderMetrics,imageProps.backgroundColor);

  if(imageData_) {
    SkRect targetRect = computeTargetRect({imageData_->width(),imageData_->height()},frameRect,imageProps.resizeMode);
    SkPaint paint,shadowPaint;
    /*Draw Image Shadow*/
    if(contentShadow) {
      if(imageProps.resizeMode == ImageResizeMode::Repeat) {
      /*TODO Not applied any shadow for solid Image when resizeMode is "Repeat"*/
        sk_sp<SkImageFilter> imageFilter(SkImageFilters::Tile(targetRect,frameRect,layer()->shadowFilter));
        shadowPaint.setImageFilter(std::move(imageFilter));
      } else {
          shadowPaint.setImageFilter(layer()->shadowFilter);
      }
      if(!(isOpaque(layer()->shadowOpacity)))
        canvas->saveLayerAlpha(&frameRect,layer()->shadowOpacity);
      canvas->drawImageRect(imageData_, targetRect, &shadowPaint);
      if(!(isOpaque(layer()->shadowOpacity)))
        canvas->restore();
    }
    /*Draw Image */
    if(( frameRect.width() < targetRect.width()) || ( frameRect.height() < targetRect.height()))
      needClipAndRestore= true;
    /* clipping logic to be applied if computed Frame is greater than the target.*/
    if(needClipAndRestore) {
        canvas->save();
        canvas->clipRect(frameRect,SkClipOp::kIntersect);
    }
    /* TODO: Handle filter quality based of configuration. Setting Low Filter Quality as default for now*/
    paint.setFilterQuality(DEFAULT_IMAGE_FILTER_QUALITY);
    if(imageProps.resizeMode == ImageResizeMode::Repeat) {
       sk_sp<SkImageFilter> imageFilter(SkImageFilters::Tile(targetRect,frameRect,nullptr));
       paint.setImageFilter(std::move(imageFilter));
    }
    canvas->drawImageRect(imageData_,targetRect,&paint);
    if(needClipAndRestore)
        canvas->restore();
    drawBorder(canvas,frame,imageBorderMetrics,imageProps.backgroundColor);
  } else {
  /* Emitting Image Load failed Event*/
    drawBackground(canvas,frame,imageBorderMetrics,imageProps.backgroundColor);
    RNS_LOG_INFO("image not loaded");
    imageEventEmitter->onError();
  }
}

RnsShell::LayerInvalidateMask RSkComponentImage::updateComponentProps(const ShadowView &newShadowView,bool forceUpdate) {

    auto const &newimageProps = *std::static_pointer_cast<ImageProps const>(newShadowView.props);
    auto component = getComponentData();
    auto const &oldimageProps = *std::static_pointer_cast<ImageProps const>(component.props);
    RnsShell::LayerInvalidateMask updateMask=RnsShell::LayerInvalidateNone;

    if((forceUpdate) || (oldimageProps.resizeMode != newimageProps.resizeMode)) {
      imageProps.resizeMode = newimageProps.resizeMode;
      updateMask =static_cast<RnsShell::LayerInvalidateMask>(updateMask | RnsShell::LayerInvalidateAll);
    }
    if((forceUpdate) || (oldimageProps.tintColor != newimageProps.tintColor )) {
      RNS_LOG_NOT_IMPL;
      imageProps.tintColor = RSkColorFromSharedColor(newimageProps.tintColor,SK_ColorTRANSPARENT);
    }
    return updateMask;
}
void RSkComponentImage::drawAndSubmit() {
  layer()->client().notifyFlushBegin();
  layer()->invalidate( RnsShell::LayerPaintInvalidate);
  if (layer()->type() == RnsShell::LAYER_TYPE_PICTURE) {
    RNS_PROFILE_API_OFF(getComponentData().componentName << " getPicture :", static_cast<RnsShell::PictureLayer*>(layer().get())->setPicture(getPicture()));
  }
  layer()->client().notifyFlushRequired();
}

void RSkComponentImage::remoteImageDataCallback(const char* path, char* response, int size, ImageCacheManager::ImageCacheMap& imageCache) {

  sk_sp<SkImage> imageDatacheck;
  sk_sp<SkData> data = SkData::MakeWithCopy(response,size);
  struct imagesDataTime imageDataTimer;
  sk_sp<SkImage> imageData = SkImage::MakeFromEncoded(data);
  imageDataTimer.imageData = imageData;
  imageDataTimer.cureentTime = (SkTime::GetSecs()+1800);// current time + 30 min expiry time
  if(imageData) {
    imageData_ = imageData;
    imageCache.insert(std::pair<std::string, imagesDataTime>(path,imageDataTimer));
    RNS_LOG_DEBUG("New Entry in Map..."<<" file :"<<path);
    drawAndSubmit();
  } else {
      RNS_LOG_DEBUG("Cache Memory limit reached or Couldn't create imageData, So file not cached ...");
  }
}

} // namespace react
} // namespace facebook
