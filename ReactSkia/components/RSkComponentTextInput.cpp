/*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/core/SkPaint.h"
#include "ReactSkia/components/RSkComponentTextInput.h"
#include "react/renderer/components/textinput/TextInputShadowNode.h"
#include "ReactSkia/views/common/RSkDrawUtils.h"
#include "ReactSkia/sdk/RNSKeyCodeMapping.h"

#include <string.h>

namespace facebook {
namespace react {

using namespace RSkDrawUtils;

RSkComponentTextInput::RSkComponentTextInput(const ShadowView &shadowView)
    : RSkComponentView(shadowView)
    ,isInEditingMode_(false){
    }

void RSkComponentTextInput::OnPaint(SkCanvas *canvas) {

  auto component = getComponentData();
  auto const &textInputProps = *std::static_pointer_cast<TextInputProps const>(component.props);

  /*Retrieve Shadow Props*/
  ShadowMetrics shadowMetrics{};
  shadowMetrics.shadowColor=textInputProps.shadowColor;
  shadowMetrics.shadowOffset=textInputProps.shadowOffset;
  shadowMetrics.shadowOpacity=textInputProps.shadowOpacity;
  shadowMetrics.shadowRadius=textInputProps.shadowRadius;

  /* apply view style props */
  auto borderMetrics= textInputProps.resolveBorderMetrics(component.layoutMetrics);
  Rect frame = getAbsoluteFrame();

  /*Draw Order : 1. Shadow 2. BackGround 3 Border*/
  //drawShadow(canvas,frame,borderMetrics,shadowMetrics);
  //drawBackground(canvas,frame,borderMetrics,textInputProps.backgroundColor,textInputProps.opacity);
  drawBorder(canvas,frame,borderMetrics,textInputProps.backgroundColor,textInputProps.opacity);
  
}
bool RSkComponentTextInput::onHandleKey(rnsKey  eventKeyType){
  bool stopPropagation=true;
  
  //TODO  update Keymatrics Event Count.
  TextInputMetrics textInputMetrics;
  auto component = getComponentData();
  auto textInputEventEmitter = std::static_pointer_cast<TextInputEventEmitter const>(component.eventEmitter);
  //textInputEventEmitter->onKeyPress(keyPressMetrics_);
  if (eventKeyType == RNS_KEY_Select){
    if(isInEditingMode_ == false){
      /*TODO Check onfocus need to here.
       *textInputEventEmitter->onfocus();
       */
      isInEditingMode_=true;
      stopPropagation = false;
    }else{
      /* TODO Update textinputmatrix data and sent the event.
      textInputEventEmitter->onSubmitEditing(textInputMetrics_);
      textInputEventEmitter->onBlur(textInputMetrics_);
      textInputEventEmitter->onEndEditing(textInputMetrics_);
      */
      isInEditingMode_=false;
    }
  }else{
    if(isInEditingMode_){
      if ((eventKeyType >= RNS_KEY_1 && eventKeyType <= RNS_KEY_z)){
        keyPressMetrics_.text=RNSKeyMap[eventKeyType]; // creat local variable for keypress matrics.
        textInputEventEmitter->onKeyPress(keyPressMetrics_);
        /*TODO
         *send onchange and onchangetext here.
         */
      }
    }else{
      stopPropagation = false;
    }
  }
  return stopPropagation; 
}

} // namespace react
} // namespace facebook
