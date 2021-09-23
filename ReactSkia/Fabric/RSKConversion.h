inline SkSize RSkSkSizeFromSize(const facebook::react::Size &size) {
  return {size.width, size.height};
}

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

inline SkColor RSkColorConversion(facebook::react::SharedColor sharedColor) {

  float ratio = 255.9999;
  auto colorValue = colorComponentsFromColor(sharedColor);
  return SkColorSetARGB(colorValue.alpha * ratio,colorValue.red * ratio,colorValue.green * ratio,colorValue.blue * ratio);
}
