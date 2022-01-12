 /*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <iterator>
#include <map>

#include <glog/logging.h>

#include "jsi/JSIDynamic.h"

#include "include/core/SkImage.h"

#include "OnScreenKeyBoard.h"

static std::unique_ptr<OnScreenKeyboard> OSKHandle_;
static unsigned int OSKeventId_;
SkPoint invalidIndex{-1,-1};

OnScreenKeyboard::OnScreenKeyboard() :

/*Step 1: Create Window*/ 
  OSKwindow_(RnsShell::Window::createNativeWindow(&RnsShell::PlatformDisplay::sharedDisplayForCompositing(),SkSize::Make(650,400),RnsShell::SubWindow)) {
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
    sk_sp<SkData> encoded = SkData::MakeFromFileName("keyboard_remote.png");
    if(encoded) {
      fImage = SkImage::MakeFromEncoded(encoded);
    } else {
      RNS_LOG_ERROR("FAILED TO READ FILE");
    }
    if(fImage) {
      OSKcanvas->translate(10, 50);
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
  RNS_LOG_TODO("Need to do Event emit to APP");
#if 0 /*Keyboard Event handling to be done*/
  folly::dynamic parameters = folly::dynamic::object();
  NotificationCenter::OSKCenter().emit("keyboardWillShow",parameters);
#endif
}

void OnScreenKeyboard::exit() {
  RNS_LOG_TODO("Need to do Event emit to APP");
#if 0 /*Keyboard Event handling to be done*/
  folly::dynamic parameters = folly::dynamic::object();
  NotificationCenter::OSKCenter().emit("keyboardWillHide",parameters);
#endif
  NotificationCenter::OSKCenter().removeListener(OSKeventId_);
  OSKHandle_->OSKwindow_->closeWindow();
  OnScreenKeyboard* fp = OSKHandle_.release();
  delete fp;
}

OSKLayout::KeyLayoutInfo OnScreenKeyboard::getNextKeyInfo(rnsKey keyValue ) {
  std::map<OSKLayout::OSKTypes, OSKLayout::KBContainer> :: iterator KBLayout = (OSKLayout_.OSKContainer).find(currentOSKType_);
  if(KBLayout != OSKLayout_.OSKContainer.end()) {
    auto &OSKLayout = KBLayout->second;
    unsigned int rowIndex =currentIndex_.x();
    unsigned int keyIndex =currentIndex_.y();
    unsigned int itemsInRow = OSKLayout[currentIndex_.x()].size();
    unsigned int rowsInKB = OSKLayout.size();

    RNS_LOG_INFO("!!!! current index : Row :" << currentIndex_.x() << " item: " << currentIndex_.y());
    RNS_LOG_INFO("!!!! No of items : Row :" << rowsInKB << " item: " << itemsInRow);
    
    switch( keyValue ) {
      case RNS_KEY_Right:
      case RNS_KEY_Left:
      {
        unsigned int nextIndex=0;
        if(keyValue == RNS_KEY_Right)
          nextIndex=( keyIndex < itemsInRow -1)? keyIndex +1 : 0 ;
        else
          nextIndex =(keyIndex) ? keyIndex-1: itemsInRow-1;

        currentIndex_ = {rowIndex,nextIndex};
        return OSKLayout[rowIndex][nextIndex];
      }
      case RNS_KEY_Up:
      case RNS_KEY_Down:
      {
          unsigned int nextIndex,itemIndex=0;
          if(keyValue == RNS_KEY_Up)
            nextIndex = (rowIndex) ? rowIndex-1 :rowsInKB-1;
          else
            nextIndex = (rowIndex == rowsInKB-1 )? 0 :rowIndex+1;

          OSKLayout::KeyLayoutInfo keyInfo=OSKLayout[nextIndex][0];
          for ( auto &item : OSKLayout[nextIndex]) {
              if((item.startPt.x() >= OSKLayout[currentIndex_.x()][currentIndex_.y()].startPt.x() ) || 
                  ( item.EndPt.x() >= OSKLayout[currentIndex_.x()][currentIndex_.y()].startPt.x())){
                  currentIndex_ = {nextIndex,itemIndex};
                  return OSKLayout[nextIndex][itemIndex];
            }
            itemIndex++;
          }
          for(unsigned int i = OSKLayout[nextIndex].size()-1; i >= 0; i--) {
            if(OSKLayout[nextIndex][i].EndPt.x() <= OSKLayout[currentIndex_.x()][currentIndex_.y()].startPt.x() ) {
              currentIndex_ = {nextIndex,itemIndex};
              return OSKLayout[nextIndex][itemIndex];
            }
          }
      }
      default:
      break;
    }
  }
  OSKLayout::KeyLayoutInfo invalideInfo{invalidIndex,invalidIndex};
  return invalideInfo;
}

OSKLayout::KeyLayoutInfo OnScreenKeyboard ::getFocussedKeyInfo(rnsKey keyValue) {
  
  std::map<OSKLayout::OSKTypes, OSKLayout::KBContainer> :: iterator KBLayout = (OSKLayout_.OSKContainer).find(currentOSKType_);
  if(KBLayout != OSKLayout_.OSKContainer.end()) {
    auto &OSKLayout = KBLayout->second;
     for(unsigned int i = 0; i < OSKLayout.size(); i++){
      for(unsigned int j = 0; j < OSKLayout[i].size(); j++){
       if(OSKLayout[i][j].keyValue == keyValue) {
          RNS_LOG_INFO(" TYPE ELEMENT : "<< RNSKeyMap[OSKLayout[i][j].keyValue]);
          currentIndex_ = {i,j};
          return OSKLayout[i][j];
      }
     }
    }
  }
  OSKLayout::KeyLayoutInfo invalideInfo{invalidIndex,invalidIndex};
  return invalideInfo;
}
void OnScreenKeyboard ::highlightKey(OSKLayout::KeyLayoutInfo keyInfo) {
  if(keyInfo.startPt != invalidIndex) {
    SkPaint paintObj;
    paintObj.setAntiAlias(true);
    paintObj.setColor(SK_ColorBLUE);
    paintObj.setStyle(SkPaint::kStroke_Style);
    paintObj.setStrokeWidth(3);
    SkRect rect=SkRect::MakeLTRB(keyInfo.startPt.x(),
                                 keyInfo.startPt.y(),
                                 keyInfo.EndPt.x(),
                                 keyInfo.EndPt.y()  );
    OSKcanvas->drawRect(rect,paintObj);
    backBuffer_->flushAndSubmit();
    std::vector<SkIRect> emptyRect;
    OSKwindowContext_->swapBuffers(emptyRect); 
  }
  return;
}

void OnScreenKeyboard::keyHandler(rnsKey keyValue, rnsKeyAction eventKeyAction){
  if(eventKeyAction != RNS_KEY_Press)
    return;
  RNS_LOG_INFO("KEY RECEIVED : "<<RNSKeyMap[keyValue]);

 /*Case 1: Emit back special keys & Alpha numberic symbol keys*/
  if(keyValue >= RNS_KEY_Select) {
    RNS_LOG_INFO("!!!! EMITTING received key to OSKCenter!!!!!");
    if(keyValue == RNS_KEY_Select)
       NotificationCenter::OSKCenter().emit("onOSKKeyEvent", focussedKey, eventKeyAction);
    else
       NotificationCenter::OSKCenter().emit("onOSKKeyEvent", keyValue, eventKeyAction);
    if( (keyValue != RNS_KEY_UnKnown) && (keyValue != RNS_KEY_Select)) {
      OSKLayout::KeyLayoutInfo keyInfo= getFocussedKeyInfo(keyValue);
      if(keyInfo.startPt != invalidIndex) {
         highlightKey(keyInfo);
      }
    }
  }
  else 
  {
/* Case  2: Process Navigation Keys*/
/* Case  2a: if the processed key is select, send current focuused key */
    OSKLayout::KeyLayoutInfo keyInfo=getNextKeyInfo(keyValue);
    RNS_LOG_INFO("!!!! Received index for Nav : Row :" << currentIndex_.x() << " item: " << currentIndex_.y());
    if(currentIndex_ != invalidIndex) {
      highlightKey(keyInfo);
      focussedKey=keyInfo.keyValue;
      if(keyInfo.keyValue == RNS_KEY_UnKnown)
        RNS_LOG_TODO(" Handling for group key");
    }
  }
}
