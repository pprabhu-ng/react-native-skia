/*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#define MAX_8BIT 255.9999
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
inline SkMatrix RSkTransformTo2DMatrix(const facebook::react::Transform &transformMatrix) {

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

inline SkColor RSkColorFromSharedColor(facebook::react::SharedColor sharedColor,SkColor defaultColor) {

    if (sharedColor) {
        auto colorValue = colorComponentsFromColor(sharedColor);
        return SkColorSetARGB(colorValue.alpha * MAX_8BIT,colorValue.red * MAX_8BIT,colorValue.green * MAX_8BIT,colorValue.blue * MAX_8BIT);
    }
    return defaultColor;
}

inline facebook::react::Point RCTPointFromSkPoint(const SkPoint &point) {
  return {point.x(), point.y()};
}

inline facebook::react::Size RCTSizeFromSkSize(const SkSize &size) {
  return {size.width(), size.height()};
}
inline bool isOpaque(float opacity) {
  return (opacity == MAX_8BIT) ? true:false;
}

#ifdef NEED_OSK_CONVERSION_SUPPORT

inline rns::sdk::OSKTypes RSkToOSKKeyboardType(facebook::react::KeyboardType keyBoardType) {
  switch(keyBoardType) {
    case facebook::react::KeyboardType::Numeric:
      return rns::sdk::OSKTypes::OSK_NUMERIC_KB;
    default:
      return rns::sdk::OSKTypes::OSK_ALPHA_NUMERIC_KB;
  }
}

inline rns::sdk::OSKThemes RSkToOSKKeyboardTheme(facebook::react::KeyboardAppearance keyBoardTheme) {
  switch(keyBoardTheme) {
    case facebook::react::KeyboardAppearance::Light:
      return rns::sdk::OSKThemes::OSK_LIGHT_THEME;
    default:
      return rns::sdk::OSKThemes::OSK_DARK_THEME;
  }
}

inline rns::sdk::OSKReturnKeyType RSkToOSKReturnKeyType(facebook::react::ReturnKeyType returnKeyType) {
  switch(returnKeyType) {
    case facebook::react::ReturnKeyType::Search:
      return rns::sdk::OSKReturnKeyType::OSK_RETURN_KEY_SEARCH;
    default:
      return rns::sdk::OSKReturnKeyType::OSK_RETURN_KEY_DEFAULT;
  }
}

#endif /*NEED_OSK_CONVERSION_SUPPORT*/

