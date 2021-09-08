#include<third_party/skia/include/core/SkSize.h>
#include<third_party/skia/include/core/SkM44.h>
inline SkSize RCTSkSizeFromSize(const facebook::react::Size &size){
  return {size.width, size.height};
}

inline SkM44 RCTCATransform3DFromTransformMatrix(const facebook::react::Transform &transformMatrix){
  return {(float)transformMatrix.matrix[0],
          (float)transformMatrix.matrix[1],
          (float)transformMatrix.matrix[2],
          (float)transformMatrix.matrix[3],
          (float)transformMatrix.matrix[4],
          (float)transformMatrix.matrix[5],
          (float)transformMatrix.matrix[6],
          (float)transformMatrix.matrix[7],
          (float)transformMatrix.matrix[8],
          (float)transformMatrix.matrix[9],
          (float)transformMatrix.matrix[10],
          (float)transformMatrix.matrix[11],
          (float)transformMatrix.matrix[12],
          (float)transformMatrix.matrix[13],
          (float)transformMatrix.matrix[14],
          (float)transformMatrix.matrix[15]};
}

