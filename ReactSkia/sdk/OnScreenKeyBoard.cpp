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
  OSKwindow_(RnsShell::Window::createNativeWindow(&RnsShell::PlatformDisplay::sharedDisplayForCompositing(),600,400)) {
  if(OSKwindow_) {
  /*Step 1a: Set Window Title*/ 
    OSKwindow_->setTitle("OSK Window");
    OSKwindow_->setType("OSK");
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
 
   if(OSKHandle_.get() == nullptr){
	  RNS_LOG_INFO("OnScreenKeyboard::init()");
	  OnScreenKeyboard::init();
    OSKHandle_->OSKwindow_->show();
	  //OSKHandle_->OSKwindowContext_->makeContextCurrent();
	  OSKHandle_->drawOSK();
  }
	
  #if 0 /*hide * show fix*/
	OSKHandle_->OSKwindow_->show();
    std::vector<SkIRect> emptyRect;
    OSKHandle_->OSKwindowContext_->swapBuffers(emptyRect);
  #endif
  folly::dynamic parameters = folly::dynamic::object();
 // NotificationCenter::OSKCenter().emit("keyboardWillShow",parameters);
}

void OnScreenKeyboard::exit() {
	//OSKHandle_->OSKwindow_->hide();
  folly::dynamic parameters = folly::dynamic::object();
//  NotificationCenter::OSKCenter().emit("keyboardWillHide",parameters);
  #if 1
  NotificationCenter::OSKCenter().removeListener(OSKeventId_);
  
  OSKHandle_->OSKwindow_->closeWindow();

  OnScreenKeyboard* fp = OSKHandle_.release();
  delete fp;
  //OSKHandle_=nullptr;
  #endif

}

void OnScreenKeyboard::keyHandler(rnsKey eventKeyType, rnsKeyAction eventKeyAction){
  if(eventKeyAction != RNS_KEY_Press)
    return;
  RNS_LOG_INFO("KEY RECEIVED : "<<RNSKeyMap[eventKeyType]);
/*Step 1: Emit back special keys*/
/* Step 2: Process Navihation Keys*/
  switch(eventKeyType ) {
    case RNS_KEY_Left:
      RNS_LOG_INFO("RNS_KEY_Left");
      break;
    case RNS_KEY_Right:
      RNS_LOG_INFO("RNS_KEY_Right");
      break;
    case RNS_KEY_Up:
      RNS_LOG_INFO("RNS_KEY_Up");
      break;
    case RNS_KEY_Down:
      RNS_LOG_INFO("RNS_KEY_Down");
      break;
    default :
     break;
  }
/*Step 3: Emit Alpha numberic symbol keys*/
  RNS_LOG_INFO("RECEIVED Alpha / numberic / symbol keys");
  RNS_LOG_ERROR("!!!! EMITTING onOSKKeyEvent to OSKCenter!!!!!");
  NotificationCenter::OSKCenter().emit("onOSKKeyEvent", eventKeyType, eventKeyAction);
}