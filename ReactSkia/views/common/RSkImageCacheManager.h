/*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#pragma once

#include "include/core/SkImage.h"
#include "include/core/SkGraphics.h"
#include "include/gpu/GrDirectContext.h"
#include <curl/curl.h>
#include <string>
#define SKIA_CPU_IMAGE_CACHE_LIMIT  50*1024*1024 // 52,428,800 bytes
#define SKIA_GPU_IMAGE_CACHE_LIMIT  50*1024*1024 // 52,428,800 bytes
using namespace std;

namespace facebook {
namespace react {
namespace ImageCacheManager{

  struct imagesDataTime {
   sk_sp<SkImage> imageData;
   double cureentTime;
  };

  typedef facebook::better::map <string,imagesDataTime> ImageCacheMap;

  struct RemoteImageData {
   std::function<void(const char*,char*,int,ImageCacheMap&)> callback;
  };

  class RSkImageCacheManager{
   public:
    ~RSkImageCacheManager();
    //static RSkImageCacheManager* defaultImageCacheManager();
    static void init();
    static RSkImageCacheManager& getInstance();
    sk_sp<SkImage> getImageData(const char *path,struct RemoteImageData* ptr);

   private:
    //static RSkImageCacheManager *defaultImageCacheManager_;
    ImageCacheMap imageCache_;
    RSkImageCacheManager();
    sk_sp<SkImage> makeImageData(const char *path,struct RemoteImageData* ptr);
    void getCacheUsage(size_t usageArr[]);
    bool evictAsNeeded();

    #ifdef RNS_IMAGE_CACHE_USAGE_DEBUG
      void printCacheUsage();
    #endif
    void geturiImage(const char* path,struct RemoteImageData* remoteImageData);
  }; //RSkImageCacheManager
} // ImageCacheManager
} // namespace react
} // namespace facebook