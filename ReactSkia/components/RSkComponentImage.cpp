#include "include/core/SkPaint.h"
#include "include/core/SkClipOp.h"
#include "include/core/SkImageFilter.h"
#include "include/effects/SkImageFilters.h"

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
  const auto source = imageProps.sources[0];

  /*Draw image not needed, if fully transparent*/
  if(imageProps.opacity <= 0.0) {
    RNS_LOG_DEBUG("Image is fully transparent,So skipping draw");
    return;
  }

  if (source.type == ImageSource::Type::Local && !source.uri.empty()) {
    assert(source.uri.substr(0, 14) == "file://assets/");
    std::string path = "./" + source.uri.substr(7);

    Rect frame = getAbsoluteFrame();
    SkRect frameRect = SkRect::MakeXYWH(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    auto const &imageBorderMetrics=imageProps.resolveBorderMetrics(component.layoutMetrics);

    SkPaint paint;
    paint.setAlphaf((imageProps.opacity >1.0 ? 1.0:imageProps.opacity));
    /*TO DO: Handle filter quality based of configuration. Setting Low Filter Quality as default for now*/
    paint.setFilterQuality(DEFAULT_IMAGE_FILTER_QUALITY);

    RNS_PROFILE_START(drawImage)
    sk_sp<SkImage> imageData=getImageData(path.c_str());
    if(imageData) {
      canvas->save();

      SkRect targetRect = computeTargetRect({imageData->width(),imageData->height()},frameRect,imageProps.resizeMode);
      /* clipping logic to be applied if computed Frame is greater than the target.*/
      if(( frameRect.width() < targetRect.width()) || ( frameRect.height() < targetRect.height())) {
        canvas->clipRect(frameRect,SkClipOp::kIntersect);
      }
      if(imageProps.resizeMode == ImageResizeMode::Repeat) {
        sk_sp<SkImageFilter> imageFilter(SkImageFilters::Tile(targetRect,frameRect ,nullptr));
        paint.setImageFilter(std::move(imageFilter));
      }
      /* Draw order 1. Background 2. Image 3. Border*/
      drawBackground(canvas,frame,imageBorderMetrics,imageProps.backgroundColor,imageProps.opacity);
      canvas->drawImageRect(imageData,targetRect,&paint);
      drawBorder(canvas,frame,imageBorderMetrics,imageProps.backgroundColor,imageProps.opacity);
      canvas->restore();
    } else {
      RNS_LOG_ERROR("Draw Image Failed for:" << path);
    }
    RNS_PROFILE_END(path.c_str(),drawImage)
#ifdef RNS_IMAGE_CACHE_USAGE_DEBUG
      printCacheUsage();
#endif //RNS_IMAGECACHING_DEBUG
  }
}

} // namespace react
} // namespace facebook
