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
#include "ReactSkia/sdk/CurlNetworking.h"
namespace facebook {
namespace react {

using namespace RSkDrawUtils;
using namespace RSkImageUtils;
using namespace ImageCacheManager;

RSkComponentImage::RSkComponentImage(const ShadowView &shadowView)
    : RSkComponent(shadowView) {
      imageCacheManagerInstance_ = ImageCacheManager::RSkImageCacheManager::getImageCacheManagerInstance();
    }

void RSkComponentImage::OnPaint(SkCanvas *canvas) {
  sk_sp<SkImage> imageData{nullptr};
  auto component = getComponentData();
  auto const &imageProps = *std::static_pointer_cast<ImageProps const>(component.props);
  /*First to check file entry presence . If not exist, generate imageData*/
  imageData= imageCacheManagerInstance_.findImageDataInCache(imageProps.sources[0].uri.c_str());
  if(!imageData) {
    if (!imageProps.sources.empty() && (imageProps.sources[0].type == ImageSource::Type::Local)){
      imageData=getLocalImage(imageProps.sources[0]);
    } else if(( imageProps.sources[0].type == ImageSource::Type::Remote)) {
      getUriImage(imageProps.sources[0]);
  }
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

  if(imageData) {
    SkRect targetRect = computeTargetRect({imageData->width(),imageData->height()},frameRect,imageProps.resizeMode);
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
      canvas->drawImageRect(imageData, targetRect, &shadowPaint);
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
    canvas->drawImageRect(imageData,targetRect,&paint);
    if(needClipAndRestore)
        canvas->restore();
    drawBorder(canvas,frame,imageBorderMetrics,imageProps.backgroundColor);
  } else {
  /* Emitting Image Load failed Event*/
    RNS_LOG_INFO("image not loaded");
    imageEventEmitter->onError();
  }
}

sk_sp<SkImage> RSkComponentImage::getLocalImage(ImageSource source) {
  std::string path;
  if ( source.uri.empty() ) {
    return nullptr;
  }
  if(source.uri.substr(0, 14) == "file://assets/") {
    path = "./" + source.uri.substr(7);
  }
  RNS_PROFILE_START(getImageData)
  sk_sp<SkImage> imageData = getImageData(path.c_str());
  if(imageData) {
    imageCacheManagerInstance_.imageDataInsertInCche(source.uri.c_str(), imageData);
    RNS_LOG_INFO("New Entry in Map..."<<" file :"<<source.uri.c_str());
  } else {
    RNS_LOG_ERROR("Draw Image Failed :" << path);
  }
  RNS_PROFILE_END(path.c_str(),getImageData)
  #ifdef RNS_IMAGE_CACHE_USAGE_DEBUG
    printCacheUsage();
  #endif //RNS_IMAGECACHING_DEBUG
  return imageData;
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

sk_sp<SkImage> RSkComponentImage::getImageData(const char *path) {
  sk_sp<SkImage> imageData{nullptr};
  sk_sp<SkData> data;
  if(!path) {
    RNS_LOG_ERROR("Invalid File");
    return nullptr;
  }
  data = SkData::MakeFromFileName(path);
   if (!data) {
    RNS_LOG_ERROR("Unable to make SkData for path : " << path);
    return nullptr;
  }
  imageData = SkImage::MakeFromEncoded(data);
  return imageData;
}

void RSkComponentImage::drawAndSubmit() {
  layer()->client().notifyFlushBegin();
  layer()->invalidate( RnsShell::LayerPaintInvalidate);
  if (layer()->type() == RnsShell::LAYER_TYPE_PICTURE) {
    RNS_PROFILE_API_OFF(getComponentData().componentName << " getPicture :", static_cast<RnsShell::PictureLayer*>(layer().get())->setPicture(getPicture()));
  }
  layer()->client().notifyFlushRequired();
}

void RSkComponentImage::getUriImage(ImageSource source) {
  struct RemoteImageData* remoteImageData(new struct RemoteImageData);
  auto sharedCurlNetworking = CurlNetworking::sharedCurlNetworking();
  CurlRequest *curlRequest = new CurlRequest(nullptr,source.uri.c_str(),0,"GET");

  // callback for remoteImageData
  remoteImageData->callback = [&](const char* path, char* response, int size) {
    // Responce callback from network. Get image data, insert in Cache and call Onpaint
    sk_sp<SkData> data = SkData::MakeWithCopy(response,size);
    sk_sp<SkImage> imageData = SkImage::MakeFromEncoded(data);
    if(imageData) {
    //Add in cache if image data is valid
      imageCacheManagerInstance_.imageDataInsertInCche(path, imageData);
      RNS_LOG_INFO("New Entry in Map..."<<" file :"<<path);
      drawAndSubmit();
    } else {
      RNS_LOG_DEBUG("No Image Data Downloaded ...");
    }
  };

  folly::dynamic query = folly::dynamic::object();
  curlRequest->curldelegator.delegatorData = (void *) remoteImageData;
  curlRequest->curlResponse.responseBuffer=(char *)malloc(1);

  // completioncallback lambda fuction
  auto completionCallback =  [&](void* curlRequestData,void *userdata)->bool{
  CurlResponse *responseData =  (CurlResponse *)curlRequestData;
  RemoteImageData *remoteImageData = (RemoteImageData *)userdata;
  remoteImageData->callback(responseData->responseurl,responseData->responseBuffer,responseData->contentSize);
  return 0;
  };

  curlRequest->curldelegator.CURLNetworkingCompletionCallback=completionCallback;
  sharedCurlNetworking->sendRequest(curlRequest,query);
}

} // namespace react
} // namespace facebook
