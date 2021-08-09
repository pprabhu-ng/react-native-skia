#include "include/core/SkPaint.h"
#include "include/core/SkClipOp.h"
#include "include/core/SkImageFilter.h"
#include "include/effects/SkImageFilters.h"

#include "react/renderer/components/image/ImageEventEmitter.h"

#include "ReactSkia/components/RSkComponentImage.h"
#include "ReactSkia/views/common/RSkDrawUtils.h"
#include "ReactSkia/views/common/RSkImageUtils.h"
#include "ReactSkia/views/common/RSkImageCacheManager.h"

#include "ReactSkia/utils/RnsLog.h"
#include "ReactSkia/utils/RnsUtils.h"

namespace facebook {
namespace react {

using namespace RSkDrawUtils;
using namespace RSkImageUtils;
using namespace RSkImageCacheManager;

RSkComponentImage::RSkComponentImage(const ShadowView &shadowView)
    : RSkComponent(shadowView) {}

void RSkComponentImage::OnPaint(
    SkCanvas *canvas) {
  auto component = getComponentData();
  auto const &imageProps =
      *std::static_pointer_cast<ImageProps const>(component.props);
  if (imageProps.sources.empty()) {
    RNS_LOG_ERROR("Empty Source ....");
    return;
  }
  auto imageEventEmitter = std::static_pointer_cast<ImageEventEmitter const>(component.eventEmitter);
  const auto source = imageProps.sources[0];

  if (source.type == ImageSource::Type::Local && !source.uri.empty()) {
    assert(source.uri.substr(0, 14) == "file://assets/");
    std::string path = "./" + source.uri.substr(7);

    Rect frame = getAbsoluteFrame();
    SkRect frameRect = SkRect::MakeXYWH(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    auto const &imageBorderMetrics=imageProps.resolveBorderMetrics(component.layoutMetrics);
	
    /* Draw order 1. Background 2. Image 3. Border*/
    drawBackground(canvas,frame,imageBorderMetrics,imageProps.backgroundColor,imageProps.opacity);
    RNS_PROFILE_START(drawImage)
    sk_sp<SkImage> imageData=getImageData(path.c_str());
    if(imageData) {
      imageEventEmitter->onLoad(); /*Emitting Image Load completed Event*/
      canvas->save();

      SkRect targetRect = computeTargetRect({imageData->width(),imageData->height()},frameRect,imageProps.resizeMode);
      /* clipping logic to be applied if computed Frame is greater than the target.*/
      if(( frameRect.width() < targetRect.width()) || ( frameRect.height() < targetRect.height())) {
        canvas->clipRect(frameRect,SkClipOp::kIntersect);
      }
      SkPaint paint;
      if(imageProps.resizeMode == ImageResizeMode::Repeat){
        sk_sp<SkImageFilter> imageFilter(SkImageFilters::Tile(targetRect,frameRect ,nullptr));
        paint.setImageFilter(std::move(imageFilter));
      }
      /*TO DO: Handle filter quality based of configuration. Setting Low Filter Quality as default for now*/
      paint.setFilterQuality(DEFAULT_IMAGE_FILTER_QUALITY);
      canvas->drawImageRect(imageData,targetRect,&paint);
      canvas->restore();
    } else {
      imageEventEmitter->onError(); /*Emitting Image Load failed Event*/
      RNS_LOG_ERROR("Draw Image Failed for:" << path);
    }
    RNS_PROFILE_END(path.c_str(),drawImage)
#ifdef RNS_IMAGE_CACHE_USAGE_DEBUG
      printCacheUsage();
#endif //RNS_IMAGECACHING_DEBUG
    drawBorder(canvas,frame,imageBorderMetrics,imageProps.backgroundColor,imageProps.opacity);
  }
}

} // namespace react
} // namespace facebook
