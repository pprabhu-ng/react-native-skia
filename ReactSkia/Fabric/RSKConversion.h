#include<third_party/skia/include/core/SkSize.h>
#include<third_party/skia/include/core/SkM44.h>
#include<react/renderer/graphics/ColorComponents.h>
#include "include/core/SkCanvas.h"
#include "include/core/SkPictureRecorder.h"
#include "include/core/SkPaint.h"
#include "include/core/SkSurface.h"
#include "ReactSkia/components/RSkComponent.h"
#include "react/renderer/mounting/ShadowView.h"
#include "react/renderer/components/view/ViewProps.h"



inline SkSize RSkSkSizeFromSize(const facebook::react::Size &size){
  return {size.width, size.height};
}

inline SkMatrix RSkTransformTO2DMatrix(const facebook::react::Transform &transformMatrix){
  return {SkMatrix::MakeAll((float)transformMatrix.matrix[0],
          (float)transformMatrix.matrix[4],
          (float)transformMatrix.matrix[12],
          (float)transformMatrix.matrix[1],
          (float)transformMatrix.matrix[5],
          (float)transformMatrix.matrix[13],
          (float)transformMatrix.matrix[3],
          (float)transformMatrix.matrix[7],
          (float)transformMatrix.matrix[15])};
}

inline SkColor RSkColorConversion(facebook::react::SharedColor sharedColor){ 
    float ratio=255.9999;
    auto colorValue=colorComponentsFromColor(sharedColor);
    return SkColorSetARGB(colorValue.alpha * ratio,colorValue.red * ratio,colorValue.green * ratio,colorValue.blue * ratio);
}
/*
void layerProps(const facebook::react::ShadowView &newShadowView,const facebook::react::ShadowView &oldShadowView){
      auto const &newviewProps = *std::static_pointer_cast<ViewProps const>(newShadowView.props);
      auto const &oldviewProps = *std::static_pointer_cast<ViewProps const>(oldShadowView.props);
 if(oldviewProps.opacity!=newviewProps.opacity){
        this->opacity=newviewProps.opacity;
   }
}*/
