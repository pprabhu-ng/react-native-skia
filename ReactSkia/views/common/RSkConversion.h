inline SkSize RSkSkSizeFromSize(const facebook::react::Size &size) {

    return {size.width, size.height};
}

/* Converting React transform matrix of form

[ ScaleX SkewY  0      0
  SkewX  ScaleY 0      0
  0      0      ScaleZ Perps
  TransX TrasnY TrasnZ 0 ]

To Skia Matrix of form

[ ScaleX SKewX  TransX
  SkewY  ScaleY TransY
  Pers0  Pers1  Pers2 ]
*/
inline SkMatrix RSkTransformTO2DMatrix(const facebook::react::Transform &transformMatrix) {

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

inline SkColor RSkColorFromSharedColor(facebook::react::SharedColor sharedColor,facebook::react::SharedColor defaultColor) {

    float ratio = 255.9999;
    if (sharedColor) {
    auto colorValue = colorComponentsFromColor(sharedColor);
    return SkColorSetARGB(colorValue.alpha * ratio,colorValue.red * ratio,colorValue.green * ratio,colorValue.blue * ratio);
    }
    else {
    auto colorValue = colorComponentsFromColor(defaultColor);
    return SkColorSetARGB(colorValue.alpha * ratio,colorValue.red * ratio,colorValue.green * ratio,colorValue.blue * ratio);
    }
}
