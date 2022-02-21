 /*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <ctype.h>

#include "include/core/SkFont.h"
#include "include/core/SkFontMgr.h"

#include "ReactSkia/utils/RnsUtils.h"

#include "OnScreenKeyBoard.h"
#include "OSKConfig.h"
#include "OSKLayout.h"

static std::unique_ptr<OnScreenKeyboard> OSKHandle_;
static unsigned int OSKeventId_;
static SkSize ScreenSize{0,0};
static SkSize baseScreenSize{1280,720};
static bool   generateOSKLayout{true};

OnScreenKeyboard::OnScreenKeyboard() :
    OSKwindow_(RnsShell::Window::createNativeWindow(&RnsShell::PlatformDisplay::sharedDisplayForCompositing(),
             SkSize::Make(ScreenSize.width(),ScreenSize.height()),
             RnsShell::SubWindow)) {

    if(OSKwindow_) {
        OSKwindow_->setTitle("OSK Window");
        OSKwindowContext_ = RnsShell::WCF::createContextForWindow(reinterpret_cast<GLNativeWindowType>(OSKwindow_->nativeWindowHandle()),
                   &RnsShell::PlatformDisplay::sharedDisplayForCompositing(), RnsShell::DisplayParams());
        if(OSKwindowContext_) {
            backBuffer_ = OSKwindowContext_->getBackbufferSurface();
        } else {
            RNS_LOG_ERROR("Invalid windowContext for nativeWindowHandle : " << OSKwindowContext_);
            return;
        }
    RNS_LOG_DEBUG("Native Window Handle : " << OSKwindow_->nativeWindowHandle() << " Window Context : " << OSKwindowContext_.get() << "Back Buffer : " << backBuffer_.get());
    OSKcanvas_ = backBuffer_->getCanvas();
    std::function<void(rnsKey, rnsKeyAction)> handler = std::bind(&OnScreenKeyboard::onHWkeyHandler, this,
                                                              std::placeholders::_1,
                                                              std::placeholders::_2);
    OSKeventId_ = NotificationCenter::OSKCenter().addListener("onHWKeyEvent", handler);
  }
}

void OnScreenKeyboard::launch(OSKConfig oskConfig) {
/* Setup Screen dimension */
    OSKHandle_->configureScreenSize();
/* Create OSK instance */
    OSKHandle_=std::make_unique<OnScreenKeyboard>();
// Set up OSk configuration
    OSKHandle_->oskTheme_=oskConfig.oskTheme;
    if(oskConfig.oskTheme == OSK_LIGHT_THEME) {
        OSKHandle_->bgColor_ = LIGHT_THEME_BG_COLOR ;
        OSKHandle_->fontColor_ = LIGHT_THEME_FONT_COLOR;
    } else {
        OSKHandle_->bgColor_ = DARK_THEME_BG_COLOR ;
        OSKHandle_->fontColor_ =DARK_THEME_FONT_COLOR;
    }
    OSKHandle_->returnKeyLabel_=oskConfig.returnKeyLabel;
    OSKHandle_->placeHolderName=oskConfig.placeHolderName;
    OSKHandle_->ctrlReturnKey_=oskConfig.enablesReturnKeyAutomatically;
/* Display OSK */
    OSKHandle_->OSKwindow_->show();
    OSKHandle_->OSKcanvas_->clear(OSKHandle_->bgColor_);
    OSKHandle_->drawPlaceHolder();
    OSKHandle_->drawOSK(oskConfig.oskType);
    RNS_LOG_TODO("Need to do emit , keyboardWillHide Event to APP");
}

void OnScreenKeyboard::exit() {
    RNS_LOG_TODO("Need to do emit,keyboardWillHide Event  to APP");
    NotificationCenter::OSKCenter().removeListener(OSKeventId_);
    OSKHandle_->OSKwindow_->closeWindow();
    OnScreenKeyboard* fp = OSKHandle_.release();
    delete fp;
}

void OnScreenKeyboard::drawOSK(OSKTypes oskType) {
    configureScreenSize();
    RNS_PROFILE_START(OSKLayoutCreateTime)
    createOSKLayout(oskType);
    RNS_PROFILE_END("OSK Layout Create",OSKLayoutCreateTime)
//clear KeyBoard Area
    SkRect kbArea=SkRect::MakeXYWH( ScreenSize.width()*START_OFFSET_OSK_X,
                                    ScreenSize.height()*START_OFFSET_OSK_Y,
                                    ScreenSize.width(),
                                    ScreenSize.height()- ScreenSize.height()*START_OFFSET_OSK_Y
                                );
    SkPaint paintObj;
    paintObj.setColor(bgColor_);
    //OSKcanvas_->drawRect(kbArea,paintObj);
    if(oskLayout_.keyInfo && oskLayout_.keyPos) {
        RNS_PROFILE_START(OSKDrawTimeStamp)
        /*1. Draw Keys */
        for(unsigned int index=0;index<oskLayout_.keysCount;index++) {
            drawFont(index,fontColor_);
        }
        /*2. Draw Kb partition*/
        drawOSKPartition();
        RNS_PROFILE_END("OSk Draw Done : ",OSKDrawTimeStamp)
        /*3. Highlighlight default focussed key*/
        highlightFocussedKey(1);
        focussedIndex_= 1;
        pushToDisplay();
    }
}

void OnScreenKeyboard::drawPlaceHolder(){
    SkPaint paint;
    if(placeHolderName) {
        SkFont font;
        paint.setColor(fontColor_);
        font.setSize(PLACEHOLDER_FONT_SIZE);
        OSKcanvas_->drawSimpleText(placeHolderName,strlen(placeHolderName), SkTextEncoding::kUTF8,
                                   ScreenSize.width()*START_OFFSET_NAME_X,
                                   ScreenSize.height()*START_OFFSET_NAME_Y,
                                   font, paint);
    }
    paint.setColor((oskTheme_ == OSK_LIGHT_THEME) ? LIGHT_THEME_PH_COLOR: DARK_THEME_PH_COLOR);
    SkRect rect=SkRect::MakeXYWH( ScreenSize.width()*START_OFFSET_PLACEHOLDER_X,
                                  ScreenSize.height()*START_OFFSET_PLACEHOLDER_Y,
                                  ScreenSize.width()*PLACEHOLDER_LENGTH,
                                  PLACEHOLDER_HEIGHT
                                );
    OSKcanvas_->drawRect(rect,paint);
}

inline void OnScreenKeyboard::drawFont(unsigned int index,SkColor color,bool onHLTile) {
    if((index >=0) && (index < oskLayout_.keysCount) && (oskLayout_.keyInfo[index].keyName) ) {
        SkPaint textPaint;
        SkFont font;
        char *keyName=(char*)oskLayout_.keyInfo[index].keyName;
        unsigned int textX,textY;
        textPaint.setColor(color);
        textPaint.setAntiAlias(true);
        if((oskLayout_.KBKeyType == ALPHA_UPPERCASE_KEYS) && (oskLayout_.keyInfo[index].keyType == ALPHA_LOWERCASE_KEYS) ) {
            keyName=(char*)oskLayout_.keyInfo[index].keyCaps;
        }
        if(onHLTile) {
            font.setSize(oskLayout_.textHLFontSize);
            //Fix to match font icon size uniform
            if(!(strcmp(keyName , "space"))) font.setSize(oskLayout_.textHLFontSize*2);
            if(!(strcmp(keyName , "done"))) font.setSize(oskLayout_.textHLFontSize*1.5);

            if((oskLayout_.KBKeyType == ALPHA_UPPERCASE_KEYS) && (oskLayout_.keyInfo[index].keyType == ALPHA_LOWERCASE_KEYS) ) {
                textX=oskLayout_.keyPos[index].textCapsHLXY.x();
                textY=oskLayout_.keyPos[index].textCapsHLXY.y();
            } else {
                textX=oskLayout_.keyPos[index].textHLXY.x();
                textY=oskLayout_.keyPos[index].textHLXY.y();
            }
        } else {
            font.setSize(oskLayout_.textFontSize);
            //Fix to match font icon size uniform
            if(!(strcmp(keyName , "space"))) font.setSize(oskLayout_.textFontSize*2);
            if(!(strcmp(keyName , "done"))) font.setSize(oskLayout_.textFontSize*1.5);
            textX=oskLayout_.keyPos[index].textXY.x();
            textY=oskLayout_.keyPos[index].textXY.y();
        }

        if(oskLayout_.keyInfo[index].keyType == ACTION_KEYS) {
            sk_sp<SkFontMgr> mgr(SkFontMgr::RefDefault());
            sk_sp<SkTypeface> typeface(mgr->matchFamilyStyleCharacter(
                    nullptr, SkFontStyle(), nullptr, 0, oskLayout_.keyInfo[index].unicharValue));
            if (typeface) {
                SkString uniChar;
                uniChar.appendUnichar(oskLayout_.keyInfo[index].unicharValue);
                font.setTypeface(typeface);
                OSKcanvas_->drawString(uniChar,textX,textY,font,textPaint);
            }
        } else {
            OSKcanvas_->drawSimpleText(keyName, strlen(keyName), SkTextEncoding::kUTF8,
                                       textX,textY,font, textPaint
                                      );
        }
    }
}

void OnScreenKeyboard::drawOSKPartition() {
    if(oskLayout_.KBKeyType != NUMERIC_KEYS) {
        unsigned int index=1;
        SkPaint paint;
        paint.setColor((oskTheme_ == OSK_LIGHT_THEME) ? LIGHT_THEME_PH_COLOR: DARK_THEME_PH_COLOR);
        paint.setStrokeWidth(2);
        unsigned int xpos{0};
        unsigned int startY= oskLayout_.keyPos[index].highlightTile.y()-3;
        unsigned int endY=oskLayout_.keyPos[oskLayout_.keysCount-1].highlightTile.fBottom + 3;
        while(oskLayout_.keyInfo[index].indexes.y() == 1) {
        /*row 1*/
            if(oskLayout_.keyInfo[index].keyType != oskLayout_.keyInfo[index-1].keyType) {
                xpos=oskLayout_.keyPos[index].highlightTile.x()-(oskLayout_.keyPos[index].highlightTile.fLeft - oskLayout_.keyPos[index-1].highlightTile.fRight)/2;
                OSKcanvas_->drawLine(xpos,startY,xpos,endY,paint);
                RNS_LOG_DEBUG("X: "<<xpos<<" y: "<<startY<<" endY: "<<endY);
            }
            index ++;
        }
    }
}

void OnScreenKeyboard ::highlightFocussedKey(unsigned int index) {
    SkPaint paintObj;
    if((index <0) || (index >oskLayout_.keysCount))
        return;
     //reset old
    if((lastFocussedIndex_ >= 0) && (lastFocussedIndex_ <= oskLayout_.keysCount)) {
        paintObj.setColor(bgColor_);
        paintObj.setAntiAlias(true);
        OSKcanvas_->drawRect(oskLayout_.keyPos[lastFocussedIndex_].highlightTile,paintObj);
        drawFont(lastFocussedIndex_,fontColor_);
    }
    //Hight new
    if((index >= 0) && (index <= oskLayout_.keysCount)) {
        paintObj.setColor(HIGHLIGHT_BG_COLOR);
        OSKcanvas_->drawRect(oskLayout_.keyPos[index].highlightTile,paintObj);
        drawFont(index,HIGHLIGHT_FONT_COLOR,true);
    }
    pushToDisplay();
    return;
}

void OnScreenKeyboard::onHWkeyHandler(rnsKey keyValue, rnsKeyAction eventKeyAction){
    if(eventKeyAction != RNS_KEY_Press)
        return;
    unsigned int index=focussedIndex_;
    lastFocussedIndex_=focussedIndex_;
    RNS_LOG_DEBUG("KEY RECEIVED : "<<RNSKeyMap[keyValue]);
    switch( keyValue ) {
    /*Case 1: handle Enter/selection key*/
        case RNS_KEY_Select:
            handleSelection();
        break;
    /* Case  2: Process Navigation Keys*/
        case RNS_KEY_Right:
            index= oskLayout_.keyInfo[focussedIndex_].indexRight;
        break;
        case RNS_KEY_Left:
            index= oskLayout_.keyInfo[focussedIndex_].indexLeft;
        break;
        case RNS_KEY_Up:
            index= oskLayout_.keyInfo[focussedIndex_].indexUp;
        break;
        case RNS_KEY_Down:
            index= oskLayout_.keyInfo[focussedIndex_].indexDown;
        break;
        default:
        {
            if(keyValue !=RNS_KEY_UnKnown ) {
    /*Case 3: Emit back other known keys*/
                NotificationCenter::OSKCenter().emit("onOSKKeyEvent", keyValue, eventKeyAction);
                for (unsigned int count=0; count<oskLayout_.keysCount;count++) {
                    if(oskLayout_.keyInfo[count].keyValue == keyValue) {
                        index = count;
                        break;
                    }
                }
            }
        }
        break;
    }
    focussedIndex_ = index;
    if(index != lastFocussedIndex_) {
        highlightFocussedKey(index);
    }
}

void OnScreenKeyboard::handleSelection() {
    if((focussedIndex_>=0) && (focussedIndex_ <= oskLayout_.keysCount)) {
        if(oskLayout_.keyInfo[focussedIndex_].keyValue == RNS_KEY_Select) {
            exit();
        } else if(oskLayout_.keyInfo[focussedIndex_].keyValue != RNS_KEY_UnKnown) {
            NotificationCenter::OSKCenter().emit("onOSKKeyEvent", oskLayout_.keyInfo[focussedIndex_].keyValue, RNS_KEY_Press);
        } else if( oskLayout_.keyInfo[focussedIndex_].keyType & GROUP_KEYS ) {
            if((oskLayout_.keyInfo[focussedIndex_].keyType & ALPHA_UPPERCASE_KEYS) &&
               (oskLayout_.KBKeyType != ALPHA_UPPERCASE_KEYS)) {
                oskLayout_.KBKeyType= ALPHA_UPPERCASE_KEYS;
                drawOSK(OSK_ALPHA_NUMERIC_KB);
            } else if((oskLayout_.keyInfo[focussedIndex_].keyType & SYMBOL_KEYS) &&
                      (oskLayout_.KBKeyType != SYMBOL_KEYS)) {
                oskLayout_.KBKeyType=SYMBOL_KEYS;
                drawOSK(OSK_ALPHA_NUMERIC_KB);
            } else if(oskLayout_.KBKeyType != ALPHA_LOWERCASE_KEYS) {
                oskLayout_.KBKeyType= ALPHA_LOWERCASE_KEYS;
                drawOSK(OSK_ALPHA_NUMERIC_KB);
            }
        }
    }
}

void OnScreenKeyboard::createOSKLayout(OSKTypes KBtype) {
    if(KBtype == OSK_NUMERIC_KB) {
        oskLayout_.keysCount = NUMERIC_KEY_COUNT;
        oskLayout_.keyInfo=numericKBLayout;
        oskLayout_.keyPos=numericKBPos;
    } else {
        if(oskLayout_.KBKeyType == SYMBOL_KEYS) {
            oskLayout_.keysCount = SYMBOL_KEY_COUNT;
            oskLayout_.keyInfo=symbolKBLayout;
            oskLayout_.keyPos=symbolKBPos;
        } else {
            oskLayout_.keysCount = ALPHA_NUMERIC_KEY_COUNT;
            oskLayout_.keyInfo=alphaNumericKBLayout;
            oskLayout_.keyPos=alphaNumericKBPos;
        }
    }
    // Generate key Position info based on screen Size
    // Not needed, if screen Size is not changed & Layout already created
    if((oskLayout_.keyPos[0].textXY.x() == -1) || generateOSKLayout ) {
        unsigned int      XscaleFactor,YscaleFactor,verticalKeySpacing;
        XscaleFactor = ScreenSize.width()/baseScreenSize.width();
        YscaleFactor =ScreenSize.height()/baseScreenSize.height();
        oskLayout_.textFontSize= FONT_SIZE *XscaleFactor;
        oskLayout_.textHLFontSize= HIGHLIGHT_FONT_SIZE *XscaleFactor;
        RNS_LOG_INFO(" Scaled Params: "<< " ScaleX: "<< XscaleFactor<<" Scale Y:"<<YscaleFactor);

        for ( unsigned int i=0;i<oskLayout_.keysCount;i++ ) {
            /*Calculate Font dimenesion */
            unsigned int fontWidth{0},fontCapsWidth{0},fontHLWidth{0};
            SkRect bounds,boundsCaps,boundsHL;
            SkFont font,fontHL;
            font.setSize(oskLayout_.textFontSize);
            font.setEdging(SkFont::Edging::kAntiAlias);
            fontHL.setSize(oskLayout_.textHLFontSize);
            fontHL.setEdging(SkFont::Edging::kAntiAlias);
            //Fix to match font icon size uniform
            if(!(strcmp(oskLayout_.keyInfo[i].keyName , "space"))) font.setSize(oskLayout_.textFontSize*2);
            if(!(strcmp(oskLayout_.keyInfo[i].keyName , "done"))) font.setSize(oskLayout_.textFontSize*1.5);

            if(!(strcmp(oskLayout_.keyInfo[i].keyName , "space"))) fontHL.setSize(oskLayout_.textHLFontSize*2);
            if(!(strcmp(oskLayout_.keyInfo[i].keyName , "done"))) fontHL.setSize(oskLayout_.textHLFontSize*1.5);

            verticalKeySpacing=oskLayout_.keyInfo[i].keySpacing.y()*oskLayout_.keyInfo[i].indexes.y();
            if(oskLayout_.keyInfo[i].keyType != ACTION_KEYS) {
                fontWidth=font.measureText(oskLayout_.keyInfo[i].keyName, strlen(oskLayout_.keyInfo[i].keyName), SkTextEncoding::kUTF8, &bounds);
                fontHLWidth=fontHL.measureText(oskLayout_.keyInfo[i].keyName, strlen(oskLayout_.keyInfo[i].keyName), SkTextEncoding::kUTF8,&boundsHL);
                if(oskLayout_.keyInfo[i].keyType == ALPHA_LOWERCASE_KEYS) {
                    fontCapsWidth=font.measureText(oskLayout_.keyInfo[i].keyName, strlen(oskLayout_.keyInfo[i].keyName), SkTextEncoding::kUTF8, &boundsCaps);
                }
            } else {
                sk_sp<SkFontMgr> mgr(SkFontMgr::RefDefault());
                sk_sp<SkTypeface> typeface(mgr->matchFamilyStyleCharacter(
                         nullptr, SkFontStyle(), nullptr, 0, oskLayout_.keyInfo[i].unicharValue));
                if (typeface) {
                    SkString uniChar;
                    font.setTypeface(typeface);
                    uniChar.appendUnichar(oskLayout_.keyInfo[i].unicharValue);
                    fontWidth=font.measureText(uniChar.c_str(), uniChar.size(), SkTextEncoding::kUTF8,&bounds);
                    fontHLWidth=fontHL.measureText(uniChar.c_str(), uniChar.size(), SkTextEncoding::kUTF8,&boundsHL);
                }
            }

            unsigned int HLTileX,HLTileY,HLTilewidth,HLTileheight;
            unsigned int textX,textY,textHLX,textHLY,textCapsX,textCapsY;
            unsigned int YstartOffset,XstartOffset;

            HLTilewidth=HLTileheight=HIGHLIGHT_AREA_WIDTH*XscaleFactor;
            XstartOffset = (START_OFFSET_OSK_X*ScreenSize.width()*XscaleFactor)+oskLayout_.keyInfo[i].keySpacing.x();
            YstartOffset = (START_OFFSET_OSK_Y*ScreenSize.height()*YscaleFactor)+ verticalKeySpacing;
            HLTileY = YstartOffset - (oskLayout_.keyInfo[i].keySpacing.y()/3);
            HLTileX = XstartOffset+ (KEY_HORIZONTAL_GAP/3);

            if(oskLayout_.keyInfo[i].indexes.x() == 0) {
                if(oskLayout_.keyInfo[i].keyType & GROUP_KEYS) {
                    HLTileX=boundsHL.fLeft+XstartOffset;
                    HLTileY=boundsHL.fTop+YstartOffset;
                    HLTilewidth=(boundsHL.fRight-boundsHL.fLeft);
                    HLTileheight=(boundsHL.fBottom-boundsHL.fTop);
                    textHLX=textX=HLTileX;
                    textHLY=textY=HLTileY+HLTileheight;
                }
            }
/*Set up Highlight Tile Area*/
            if(oskLayout_.keyInfo[i].indexes.x()) {
                oskLayout_.keyPos[i].highlightTile.setXYWH((oskLayout_.keyPos[oskLayout_.keyInfo[i].indexLeft].highlightTile.fLeft + oskLayout_.keyInfo[i].keySpacing.x() + (KEY_HORIZONTAL_GAP/3)),
                                                             HLTileY,HLTilewidth,HLTileheight
                                                          );
            } else {
                oskLayout_.keyPos[i].highlightTile.setXYWH(HLTileX,HLTileY,HLTilewidth,HLTileheight);
            }
            if(oskLayout_.keyInfo[i].keyType & GROUP_KEYS) {
                oskLayout_.keyPos[i].highlightTile.outset(10,10);
            }
            RNS_LOG_INFO("HL X : "<<oskLayout_.keyPos[i].highlightTile.x()<<"HL Y: "<<oskLayout_.keyPos[i].highlightTile.y());
            RNS_LOG_INFO("HL W : "<<oskLayout_.keyPos[i].highlightTile.width()<<"HL H: "<<oskLayout_.keyPos[i].highlightTile.height());

/*Set up Text draw Pt*/
            if(!(oskLayout_.keyInfo[i].keyType & GROUP_KEYS)) {
                textX=oskLayout_.keyPos[i].highlightTile.x()+(oskLayout_.keyPos[i].highlightTile.width()/2)-(fontWidth/2);
                textY=oskLayout_.keyPos[i].highlightTile.y()+(oskLayout_.keyPos[i].highlightTile.height()/2)+(bounds.height()/2);
                textHLX=oskLayout_.keyPos[i].highlightTile.x()+(oskLayout_.keyPos[i].highlightTile.width()/2)-(fontHLWidth/2);
                textHLY=oskLayout_.keyPos[i].highlightTile.y()+(oskLayout_.keyPos[i].highlightTile.height()/2)+(boundsHL.height()/2);
                textCapsX=oskLayout_.keyPos[i].highlightTile.x()+((oskLayout_.keyPos[i].highlightTile.width()/2)-(fontCapsWidth/2));
                textCapsY=oskLayout_.keyPos[i].highlightTile.y()+(oskLayout_.keyPos[i].highlightTile.height()/2)+(boundsCaps.height()/2);
            }
            oskLayout_.keyPos[i].textXY.set(textX,textY);
            oskLayout_.keyPos[i].textHLXY.set(textHLX,textHLY);
            if(oskLayout_.keyInfo[i].keyType == ALPHA_LOWERCASE_KEYS) {
                oskLayout_.keyPos[i].textCapsHLXY.set(textCapsX,textCapsY);
            }
            #ifdef SHOW_CREATED_COORDINATE
            SkPaint paint;
            paint.setColor(SK_ColorRED);
            paint.setStyle(SkPaint::kStroke_Style);
            OSKcanvas_->drawRect(oskLayout_.keyPos[i].highlightTile,paint);
            paint.setColor(SK_ColorGREEN);
            OSKcanvas_->drawRect(bounds,paint);
            paint.setColor(SK_ColorBLUE);
            paint.setStrokeWidth(5);
            OSKcanvas_->drawPoint(oskLayout_.keyPos[i].textXY.x(),oskLayout_.keyPos[i].textXY.y(),paint);
            RNS_LOG_INFO("Key Name : "<<oskLayout_.keyInfo[i].keyName);
            RNS_LOG_INFO("text X : "<<oskLayout_.keyPos[i].textXY.x()<<" Y: "<<oskLayout_.keyPos[i].textXY.y());
            RNS_LOG_INFO("HL X : "<<oskLayout_.keyPos[i].highlightTile.x()<<" Y: "<<oskLayout_.keyPos[i].highlightTile.y());
            #endif
        }
    }
}

inline void OnScreenKeyboard::pushToDisplay() {
    backBuffer_->flushAndSubmit();
    std::vector<SkIRect> emptyRect;
    OSKwindowContext_->swapBuffers(emptyRect);
}
inline void OnScreenKeyboard::configureScreenSize() {
    SkSize mainScreenSize=RnsShell::PlatformDisplay::sharedDisplay().screenSize();
    if(ScreenSize != mainScreenSize) {
      generateOSKLayout=true;
      ScreenSize=mainScreenSize;
    } else {
      generateOSKLayout=false;
    }
}

