/*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/core/SkPaint.h"

#include "ReactSkia/components/RSkComponentTextInput.h"
#include "react/renderer/components/textinput/iostextinput/TextInputShadowNode.h"
#include "ReactSkia/views/common/RSkDrawUtils.h"
#include "ReactSkia/sdk/RNSKeyCodeMapping.h"
#include <string.h>
namespace facebook {
namespace react {

using namespace RSkDrawUtils;

RSkComponentTextInput::RSkComponentTextInput(const ShadowView &shadowView)
    : RSkComponentView(shadowView) {
      isInEditingMode=false;
      

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
bool RSkComponentTextInput::onHandleKey(rnsKey  eventKeyType, rnsKeyAction eventKeyAction){
    bool stopPropagation=true;
    KeyPressMetrics keyPressMetrics_;
    keyPressMetrics_.text=eventKeyType;
    keyPressMetrics_.eventCount=eventKeyAction;
  
    TextInputMetrics textInputMetrics_;
    auto component = getComponentData();
    auto textInputEventEmitter = std::static_pointer_cast<TextInputEventEmitter const>(component.eventEmitter);
    //textInputEventEmitter->onKeyPress(keyPressMetrics_);
    if (eventKeyType == RNS_KEY_Select){
        if(isInEditingMode == false){
          //TO DO
          //component.focus(textInputMetrics_);
          //textInputEventEmitter->onfocus();
          isInEditingMode=true;
          stopPropagation = false;
        }
       else{
          //TODO
          //textInputEventEmitter->onKeyPress(keyPressMetrics_);
          textInputEventEmitter->onSubmitEditing(textInputMetrics_);
          textInputEventEmitter->onBlur(textInputMetrics_);
          textInputEventEmitter->onEndEditing(textInputMetrics_);
          isInEditingMode=false;
       }
    }
    else{
      if(isInEditingMode){//isInEditingMode
        /* TODO
        if (key == "Alphanumatric")
        mostly remapping function required to check;
        */
        if ((eventKeyType >= RNS_KEY_a && eventKeyType <= RNS_KEY_z) ||
			(eventKeyType >= RNS_KEY_A && eventKeyType <= RNS_KEY_Z) ||
			(eventKeyType >= RNS_KEY_1 && eventKeyType <= RNS_KEY_0)){
        textInputEventEmitter->onKeyPress(keyPressMetrics_);
        //TODO 
        // send onchange and onchangetext here.
        //textInputEventEmitter->onChange(textInputMetrics_);//remove the for now.
        //textInputEventEmitter->onChangetext(textInputMetrics_);//remove the for now 
           stopPropagation = true; // 
        }
        else if(eventKeyType >= RNS_KEY_Caps_Lock && eventKeyType <= RNS_KEY_Control_R){
          stopPropagation = true;
        }
      }
      else
        stopPropagation = false;
      
    }

    return stopPropagation; 
}

} // namespace react
} // namespace facebook
