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
#include "include/core/SkFont.h"

#include "OnScreenKeyBoard.h"
#undef MAC_FLATLIST_MODEL
static std::unique_ptr<OnScreenKeyboard> OSKHandle_;
static unsigned int OSKeventId_;
SkPoint invalidIndex{-1,-1};
constexpr SkColor4f TeelGreen = {0, .43f, .35f, .5};
//#define HIGHTLIGHTER '#006d5b7f' //teal green
#define FONT_SIZE  24
#define ListCount 40
#define CapsListCount 26
#define FlastListCount 14
char KBtextList[ListCount]={'1','2','3','4','5','6','7','8','9','0',
                  'a','b','c','d','e','f','g','h','i','j',
                  'k','l','m','n','o','p','q','r','s','t',
                  'u','v','w','x','y','z','!',',','.','@'};
char KBFlattextList[FlastListCount]={'1','2','3','4','5','6','7','8','9','0','!',',','.','@'};
char KBCapstextList[CapsListCount]={'A','B','C','D','E','F','G','H','I','J',
                  'K','L','M','N','0','P','Q','R','S','T','U','V','W','X','Y','Z'};

char KBsmalltextList[CapsListCount]={'a','b','c','d','e','f','g','h','i','j',
                  'k','l','m','n','o','p','q','r','s','t',
                  'u','v','w','x','y','z'};
SkPaint textPaint;
SkFont font;
#ifdef MAC_FLATLIST_MODEL
unsigned int width=1920;
unsigned int height=1720;
#else
unsigned int width=650;
unsigned int height=400;
#endif
OnScreenKeyboard::OnScreenKeyboard() :

/*Step 1: Create Window*/ 
  OSKwindow_(RnsShell::Window::createNativeWindow(&RnsShell::PlatformDisplay::sharedDisplayForCompositing(),SkSize::Make(width,height),RnsShell::SubWindow)) {
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
  #if 0
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
  #else
  if(OSKcanvas) {
    OSKcanvas->clear(SK_ColorDKGRAY);
    OSKcanvas->translate(10, 50);
    textPaint.setColor(SK_ColorWHITE);
    font.setSize(FONT_SIZE);
#ifndef MAC_FLATLIST_MODEL
    for(unsigned int i=0;i<ListCount;i++) {
      OSKcanvas->drawSimpleText(&KBtextList[i], 1, SkTextEncoding::kUTF8, (OSKLayout_.DefaultOSK[i].startPt.x())+25,( OSKLayout_.DefaultOSK[i].startPt.y())+25, font, textPaint);
    }
#else
    unsigned int startX=OSKLayout_.DefaultOSK[0].startPt.x();
    unsigned int startY=OSKLayout_.DefaultOSK[0].startPt.y();
    for(unsigned int i=0;i<FlastListCount;i++) {
      OSKcanvas->drawSimpleText(&KBFlattextList[i], 1, SkTextEncoding::kUTF8, startX+25+(i*(40)), startY+25, font, textPaint);
    }
    startX=OSKLayout_.DefaultOSK[10].startPt.x();
    startY=OSKLayout_.DefaultOSK[10].startPt.y();
    for(unsigned int i=0;i<CapsListCount;i++) {
      OSKcanvas->drawSimpleText(&KBsmalltextList[i], 1, SkTextEncoding::kUTF8, startX+25+(i*(40)), startY+25, font, textPaint);
    }
#endif
  }
  #endif/* Draw with text*/
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
//  highlightKey(OSKLayout_.DefaultOSK[currentIndex_]);
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


OSKLayout::KeyLayoutInfo OnScreenKeyboard ::getFocussedKeyInfo(rnsKey keyValue) {
  
  for (unsigned int i=0; i<DEFAULT_OSK_KEYS;i++) {
  	if(OSKLayout_.DefaultOSK[i].keyValue == keyValue)
  		return OSKLayout_.DefaultOSK[i];
  }
  OSKLayout::KeyLayoutInfo Temp{invalidIndex,invalidIndex};
  return Temp;
}
void OnScreenKeyboard::redrawOSK(char* keyList, unsigned int keyCount,unsigned int startIndex) {
      SkPaint paintObj;
    
    //reset old
    OSKLayout::KeyLayoutInfo oldKeyInfo;
    paintObj.setColor(SK_ColorDKGRAY);
    paintObj.setAntiAlias(true);
#ifndef MAC_FLATLIST_MODEL
    for(unsigned int i=0;i<keyCount ;i++) {
      oldKeyInfo=OSKLayout_.DefaultOSK[startIndex+i];;
      SkRect oldRect=SkRect::MakeLTRB(oldKeyInfo.startPt.x(),
                                 oldKeyInfo.startPt.y(),
                                 oldKeyInfo.EndPt.x(),
                                 oldKeyInfo.EndPt.y()  );
      OSKcanvas->drawRect(oldRect,paintObj);
      OSKcanvas->drawSimpleText(&keyList[i], 1, SkTextEncoding::kUTF8, (oldKeyInfo.startPt.x())+25,( oldKeyInfo.startPt.y())+25, font, textPaint);
    }
#else
    oldKeyInfo=OSKLayout_.DefaultOSK[10];
    SkRect oldRect=SkRect::MakeXYWH(oldKeyInfo.startPt.x(),
                                 oldKeyInfo.startPt.y(),
                                 width,                                 
                                 height );
      OSKcanvas->drawRect(oldRect,paintObj);
    for(unsigned int i=0;i<keyCount ;i++) {

      OSKcanvas->drawSimpleText(&keyList[i], 1, SkTextEncoding::kUTF8, (oldKeyInfo.startPt.x())+25+(i*40),( oldKeyInfo.startPt.y())+25, font, textPaint);
    }
#endif
    backBuffer_->flushAndSubmit();
    std::vector<SkIRect> emptyRect;
    OSKwindowContext_->swapBuffers(emptyRect); 

}
void OnScreenKeyboard ::highlightKey(OSKLayout::KeyLayoutInfo keyInfo) {
  if(keyInfo.startPt != invalidIndex) {
    SkPaint paintObj;
    #ifndef MAC_FLATLIST_MODEL
   
    
    //reset old
    OSKLayout::KeyLayoutInfo oldKeyInfo=OSKLayout_.DefaultOSK[previousIndex_];;
    paintObj.setColor(SK_ColorDKGRAY);
    paintObj.setAntiAlias(true);

    SkRect oldRect=SkRect::MakeLTRB(oldKeyInfo.startPt.x(),
                                 oldKeyInfo.startPt.y(),
                                 oldKeyInfo.EndPt.x(),
                                 oldKeyInfo.EndPt.y()  );
    OSKcanvas->drawRect(oldRect,paintObj);
    OSKcanvas->drawSimpleText(&KBtextList[previousIndex_], 1, SkTextEncoding::kUTF8, (oldKeyInfo.startPt.x())+25,( oldKeyInfo.startPt.y())+25, font, textPaint);
    //Hight new
    paintObj.setColor(TeelGreen);
  //  paintObj.setStyle(SkPaint::kStroke_Style);
  //  paintObj.setStrokeWidth(3);
    SkRect rect=SkRect::MakeLTRB(keyInfo.startPt.x(),
                                 keyInfo.startPt.y(),
                                 keyInfo.EndPt.x(),
                                 keyInfo.EndPt.y()  );
    OSKcanvas->drawRect(rect,paintObj);
    #endif
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
  previousIndex_=currentIndex_;
  FocussedKey = RNS_KEY_Shift_L;
 /*Case 1: Emit back special keys & Alpha numberic symbol keys*/
  if(keyValue >= RNS_KEY_Select) {
    RNS_LOG_INFO("!!!! EMITTING received key to OSKCenter!!!!!");

    if((keyValue == RNS_KEY_Select) && (FocussedKey == RNS_KEY_Shift_L) ) {
      if(isCurrentCAPS) {
        redrawOSK(KBsmalltextList,CapsListCount,10);
        isCurrentCAPS=false;
      } else {
        redrawOSK(KBCapstextList,CapsListCount,10);
        isCurrentCAPS=true;
      }
    } else if(keyValue == RNS_KEY_Select  )
       NotificationCenter::OSKCenter().emit("onOSKKeyEvent", OSKLayout_.DefaultOSK[currentIndex_].keyValue, eventKeyAction);
    else
       NotificationCenter::OSKCenter().emit("onOSKKeyEvent", keyValue, eventKeyAction);
    if( (keyValue != RNS_KEY_UnKnown) && (keyValue != RNS_KEY_Select)) {
      OSKLayout::KeyLayoutInfo keyInfo= getFocussedKeyInfo(keyValue);
      if(keyInfo.startPt != invalidIndex) {
         highlightKey(keyInfo);
         currentIndex_ = keyInfo.indexSelf;
         #ifndef MAC_FLATLIST_MODEL
         FocussedKey= keyInfo.keyValue;
         #endif
      }
    }
  }
  else 
  {
/* Case  2: Process Navigation Keys*/
/* Case  2a: if the processed key is select, send current focuused key */
    OSKLayout::KeyLayoutInfo keyInfo{invalidIndex,invalidIndex};
    unsigned int index=OSKLayout_.DefaultOSK[currentIndex_].indexSelf;
    switch( keyValue ) {
      case RNS_KEY_Right:
        keyInfo= OSKLayout_.DefaultOSK[OSKLayout_.DefaultOSK[currentIndex_].indexRight];
        break;
      case RNS_KEY_Left:
        keyInfo= OSKLayout_.DefaultOSK[OSKLayout_.DefaultOSK[currentIndex_].indexLeft];
        break;
      case RNS_KEY_Up:
        keyInfo= OSKLayout_.DefaultOSK[OSKLayout_.DefaultOSK[currentIndex_].indexUp];
        break;
      case RNS_KEY_Down:
        keyInfo= OSKLayout_.DefaultOSK[OSKLayout_.DefaultOSK[currentIndex_].indexDown];
        break;
      default:
      	break;
    }
    
    RNS_LOG_INFO("!!!! Received index for Nav : "<< currentIndex_);
    if(currentIndex_ != -1) {
      currentIndex_ = keyInfo.indexSelf;
      FocussedKey= keyInfo.keyValue;
      highlightKey(keyInfo);
      if(keyInfo.keyValue == RNS_KEY_UnKnown)
        RNS_LOG_TODO(" Handling for group key");
    }
  }
}
