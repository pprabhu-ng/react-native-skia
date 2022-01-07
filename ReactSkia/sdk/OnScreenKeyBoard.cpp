 /*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <glog/logging.h>

#include "jsi/JSIDynamic.h"

#include "include/core/SkImage.h"

#include "OnScreenKeyBoard.h"

static std::unique_ptr<OnScreenKeyboard> OSKHandle_;
static unsigned int OSKeventId_;
 OnScreenKeyboard::OnScreenKeyboard() :
/*Step 1: Create Window*/ 
  OSKwindow_(RnsShell::Window::createNativeWindow(&RnsShell::PlatformDisplay::sharedDisplayForCompositing(),SkSize::Make(600,400),RnsShell::SubWindow)) {
  if(OSKwindow_) {
/*Step 1a: Set Window Title*/
    OSKwindow_->setTitle("OSK Window");
/*Step 2: Create Window Context for the OSKWindow*/
    OSKwindowContext_ = RnsShell::WCF::createContextForWindow(reinterpret_cast<GLNativeWindowType>(OSKwindow_->nativeWindowHandle()),
                   &RnsShell::PlatformDisplay::sharedDisplayForCompositing(), RnsShell::DisplayParams());
/*Step 3: Get BackBuffer from WindowContext*/
    if(OSKwindowContext_) {
      backBuffer_ = OSKwindowContext_->getBackbufferSurface();
    } else {
      RNS_LOG_ERROR("Invalid windowContext for nativeWindowHandle : " << OSKwindowContext_);
      return;
    }
    RNS_LOG_INFO("Native Window Handle : " << OSKwindow_->nativeWindowHandle() << " Window Context : " << OSKwindowContext_.get() << "Back Buffer : " << backBuffer_.get());
/*Step 4: Get canvas from BackBuffer*/
    OSKcanvas = backBuffer_->getCanvas();
/*Step 5: Listen for "OnHwKeyEvent" from OSK Notification centre*/
    std::function<void(rnsKey, rnsKeyAction)> handler = std::bind(&OnScreenKeyboard::keyHandler, this,
                                                              std::placeholders::_1, // rnsKey
                                                              std::placeholders::_2);
    OSKeventId_ = NotificationCenter::OSKCenter().addListener("onHWKeyEvent", handler);
  }
}
void OnScreenKeyboard::drawOSK() {
/*Temp Handling for POC: Dispalying static image for OSK*/
  if(OSKcanvas) {
    sk_sp<SkImage>   fImage;
    sk_sp<SkData> encoded = SkData::MakeFromFileName("keyboard.png");
    if(encoded) {
      fImage = SkImage::MakeFromEncoded(encoded);
    } else {
      RNS_LOG_ERROR("FAILED TO READ FILE");
    }
    if(fImage) {
      OSKcanvas->translate(50, 50);
      OSKcanvas->drawImage(fImage, 0, 0);
    } else {
    	RNS_LOG_ERROR("FAILED TO Make Image Data");
    }
  }
  else {
  	RNS_LOG_ERROR("INvalid Canvas , can't present OSK");
  }
   backBuffer_->flushAndSubmit();
   std::vector<SkIRect> emptyRect;
   OSKwindowContext_->swapBuffers(emptyRect);
}

void OnScreenKeyboard::init(){
  RNS_LOG_INFO("Creating OSK class ");
	OSKHandle_=std::make_unique<OnScreenKeyboard>();
}

void OnScreenKeyboard::launch() {
	OnScreenKeyboard::init();
  OSKHandle_->OSKwindow_->show();
  OSKHandle_->drawOSK();
#if 0 /*Keyboard Event handling to be done*/
  folly::dynamic parameters = folly::dynamic::object();
  NotificationCenter::OSKCenter().emit("keyboardWillShow",parameters);
#endif
}

void OnScreenKeyboard::exit() {
#if 0 /*Keyboard Event handling to be done*/
  folly::dynamic parameters = folly::dynamic::object();
  NotificationCenter::OSKCenter().emit("keyboardWillHide",parameters);
#endif
  NotificationCenter::OSKCenter().removeListener(OSKeventId_);
  OSKHandle_->OSKwindow_->closeWindow();
  OnScreenKeyboard* fp = OSKHandle_.release();
  delete fp;
}

void OnScreenKeyboard::keyHandler(rnsKey eventKeyType, rnsKeyAction eventKeyAction){
  if(eventKeyAction != RNS_KEY_Press)
    return;
  RNS_LOG_INFO("KEY RECEIVED : "<<RNSKeyMap[eventKeyType]);
/*Step 1: Emit back special keys & Alpha numberic symbol keys*/
  if(eventKeyType > RNS_KEY_Down) {
    RNS_LOG_INFO("!!!! EMITTING received key to OSKCenter!!!!!");
    NotificationCenter::OSKCenter().emit("onOSKKeyEvent", eventKeyType, eventKeyAction);
  }
  else {
/* Step 2: Process Navigation Keys*/
   RNS_LOG_TODO(" Need to come up with Navigation algorithm ");
  }
}
