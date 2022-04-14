#pragma once

#include "include/core/SkRect.h"

#include "react/renderer/components/image/ImageShadowNode.h"

#include "ReactSkia/components/RSkComponent.h"
#include "ReactSkia/views/common/RSkImageCacheManager.h"

#define DEFAULT_IMAGE_FILTER_QUALITY kLow_SkFilterQuality /*Skia's Defualt is kNone_SkFilterQuality*/

namespace facebook {
namespace react {

struct ImgProps{
    ImageResizeMode resizeMode;
    SkColor tintColor;
};

class RSkComponentImage final : public RSkComponent {
 public:
  RSkComponentImage(const ShadowView &shadowView);
  RnsShell::LayerInvalidateMask updateComponentProps(const ShadowView &newShadowView,bool forceUpdate) override;

 private :
  RSkImageCacheManager imageCacheManagerInstance_;
  ImgProps imageProps;
  std::function<void(const char*,char*,int)> callback;

  sk_sp<SkImage> getLocalImageData(ImageSource source);
  //sk_sp<SkImage> getImageData(const char *path);

  void drawAndSubmit();
  void remoteImageDataCallback(const char* path, char* response, int size);
  void requestNetworkImageData(ImageSource source);

 protected:
  void OnPaint(SkCanvas *canvas) override;
};

} // namespace react
} // namespace facebook
