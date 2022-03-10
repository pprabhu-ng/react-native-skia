 /*
 * Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "include/core/SkFont.h"
#include "include/core/SkFontMgr.h"

#include "ReactSkia/utils/RnsUtils.h"

#include "OnScreenKeyBoard.h"
#include "OSKConfig.h"
#include "OSKLayout.h"

OnScreenKeyboard::OnScreenKeyboard(OSKConfig oskConfig,SkSize ScreenSize) :
    exposeEventID_(NotificationCenter::defaultCenter().addListener("windowExposed", std::function<void(RnsShell::Window*)>(std::bind(&OnScreenKeyboard::onExposeHandler,this,
                                                     std::placeholders::_1)))),
    oskConfig_(oskConfig),
    OSKwindow_(RnsShell::Window::createNativeWindow(&RnsShell::PlatformDisplay::sharedDisplayForCompositing(),
               SkSize::Make(ScreenSize.width(),ScreenSize.height()),
               RnsShell::SubWindow))
{
    sem_init(&semReadyToDraw,0,0);
    if(OSKwindow_) {
        OSKwindow_->setTitle("OSK Window");
        OSKwindowContext_ = RnsShell::WCF::createContextForWindow(reinterpret_cast<GLNativeWindowType>(OSKwindow_->nativeWindowHandle()),
                   &RnsShell::PlatformDisplay::sharedDisplayForCompositing(), RnsShell::DisplayParams());
        if(OSKwindowContext_) {
            OSKwindowContext_->makeContextCurrent();
            backBuffer_ = OSKwindowContext_->getBackbufferSurface();
            OSKcanvas_ = backBuffer_->getCanvas();
            RNS_LOG_DEBUG("Native Window Handle : " << OSKwindow_->nativeWindowHandle() << " Window Context : " << OSKwindowContext_.get() << "Back Buffer : " << backBuffer_.get());
        } else {
            RNS_LOG_ERROR("Invalid windowContext for nativeWindowHandle : " << OSKwindow_->nativeWindowHandle());
        }
    }
    sem_post(&semReadyToDraw);
}

SharedOSKHandle OnScreenKeyboard::launch(OSKConfig oskConfig) {
    RNS_LOG_TODO("Need to do emit , keyboardWillShow Event to APP");
    SkSize mainScreenSize=RnsShell::PlatformDisplay::sharedDisplay().screenSize();
    SharedOSKHandle oskHandle;
    oskHandle = std::make_shared<OnScreenKeyboard>(oskConfig,mainScreenSize);
    oskHandle->oskHandle_=oskHandle;
    return oskHandle;
}

void OnScreenKeyboard::exit() {
    RNS_LOG_TODO("Need to emitkeyboardWillHide Event  to APP");
    if( OSKeventId_ )  {
        NotificationCenter::OSKCenter().removeListener(OSKeventId_);
        OSKeventId_= -1;
    }
    if(exposeEventID_) {
        NotificationCenter::OSKCenter().removeListener(exposeEventID_);
        exposeEventID_=-1;
    }
    sem_destroy(&semReadyToDraw);
    if(OSKwindow_)
        OSKwindow_->closeWindow();
    oskHandle_.reset();
}

void OnScreenKeyboard::drawOSK(OSKTypes oskType) {
    RNS_PROFILE_START(OSKLayoutCreate)
    createOSKLayout(oskType);
    RNS_PROFILE_START(OSKDraw)
//clear KeyBoard Area
    SkRect kbArea=SkRect::MakeXYWH( ScreenSize.width()*START_OFFSET_OSK_X,
                                    ScreenSize.height()*START_OFFSET_OSK_Y,
                                    ScreenSize.width(),
                                    ScreenSize.height()- ScreenSize.height()*START_OFFSET_OSK_Y
                                  );
    SkPaint paintObj;
    paintObj.setColor(bgColor_);
    OSKcanvas_->drawRect(kbArea,paintObj);
    if(oskLayout_.keyInfo && oskLayout_.keyPos) {
        /*1. Draw Keys */
        for (unsigned int rowIndex = 0; rowIndex < oskLayout_.keyInfo->size(); rowIndex++) {
            for (unsigned int keyIndex = 0; keyIndex < oskLayout_.keyInfo->at(rowIndex).size(); keyIndex++) {
                drawFont({keyIndex,rowIndex},fontColor_);
            }
        }
        /*2. Draw Kb partition*/
        drawOSKPartition();
        /*3. Highlighlight default focussed key*/
        highlightFocussedKey(oskLayout_.defaultFocussIndex);
        focussedKey_=oskLayout_.defaultFocussIndex;
    }
    RNS_PROFILE_END("OSk Draw completion : ",OSKDraw)
}

void OnScreenKeyboard::drawPlaceHolder(){
    SkPaint paint;
    if(oskConfig_.placeHolderName) {
        SkFont font;
        paint.setColor(fontColor_);
        font.setSize(PLACEHOLDER_FONT_SIZE);
        OSKcanvas_->drawSimpleText(oskConfig_.placeHolderName,strlen(oskConfig_.placeHolderName), SkTextEncoding::kUTF8,
                                   ScreenSize.width()*START_OFFSET_NAME_X,
                                   ScreenSize.height()*START_OFFSET_NAME_Y,
                                   font, paint);
    }
    paint.setColor((oskConfig_.oskTheme == OSK_LIGHT_THEME) ? LIGHT_THEME_PH_COLOR: DARK_THEME_PH_COLOR);
    SkRect rect=SkRect::MakeXYWH( ScreenSize.width()*START_OFFSET_PLACEHOLDER_X,
                                  ScreenSize.height()*START_OFFSET_PLACEHOLDER_Y,
                                  ScreenSize.width()*PLACEHOLDER_LENGTH,
                                  PLACEHOLDER_HEIGHT
                                );
    OSKcanvas_->drawRect(rect,paint);
}

inline void OnScreenKeyboard::drawFont(SkPoint index,SkColor color,bool onHLTile) {
    unsigned int rowIndex=index.y(),keyIndex=index.x();

    if(oskLayout_.keyInfo->at(rowIndex).at(keyIndex).keyName ) {
        SkPaint textPaint;
        SkFont font;
        SkString uniChar;
        char *keyName=(char*)oskLayout_.keyInfo->at(rowIndex).at(keyIndex).keyName;
        char upperCase[2];
        unsigned int textX,textY,groupID;

        textPaint.setColor(color);
        textPaint.setAntiAlias(true);
        groupID=oskLayout_.keyInfo->at(rowIndex).at(keyIndex).KBPartitionId;

        if(( oskLayout_.keyInfo->at(rowIndex).at(keyIndex).keyType == TEXT_KEY ) &&
           (oskLayout_.KBLayoutType == ALPHA_UPPERCASE_LAYOUT) && (isalpha(*keyName))) {
            upperCase[0] = *keyName-LOWER_TO_UPPER_CASE_OFFSET;
            upperCase[1]='\0';
            keyName=upperCase;
        }

        if(onHLTile) {
            font.setSize(oskLayout_.textHLFontSize* oskLayout_.KBGroupConfig[groupID].fontScaleFactor);
            if(( oskLayout_.keyInfo->at(rowIndex).at(keyIndex).keyType == TEXT_KEY ) &&
               (isalpha(*keyName)) && oskLayout_.KBLayoutType == ALPHA_UPPERCASE_LAYOUT ) {
                textX=oskLayout_.keyPos->at(rowIndex).at(keyIndex).textCapsHLXY.x();
                textY=oskLayout_.keyPos->at(rowIndex).at(keyIndex).textCapsHLXY.y();
            } else {
                textX=oskLayout_.keyPos->at(rowIndex).at(keyIndex).textHLXY.x();
                textY=oskLayout_.keyPos->at(rowIndex).at(keyIndex).textHLXY.y();
            }
        } else {
            if (oskLayout_.keyInfo->at(rowIndex).at(keyIndex).keyType == TOGGLE_KEY) {
                ToggleKeyMap :: iterator keyFunction =toggleKeyMap.find(keyName);
                if(keyFunction != toggleKeyMap.end()) {
                   if(keyFunction->second != oskLayout_.KBLayoutType)
                       textPaint.setColor((oskConfig_.oskTheme == OSK_LIGHT_THEME) ? LIGHT_THEME_INACTIVE_FONT_COLOR: DARK_THEME_INACTIVE_FONT_COLOR);
                }
            }
            font.setSize(oskLayout_.textFontSize * oskLayout_.KBGroupConfig[groupID].fontScaleFactor);
            textX=oskLayout_.keyPos->at(rowIndex).at(keyIndex).textXY.x();
            textY=oskLayout_.keyPos->at(rowIndex).at(keyIndex).textXY.y();
        }

        if(oskLayout_.keyInfo->at(rowIndex).at(keyIndex).keyType == FUNCTION_KEY) {
            FunctionKeymap :: iterator keyFunction =functionKeyMap.find(keyName);
            keyName=(char *)DRAW_FONT_FAILURE_INDICATOR;
            if(keyFunction != functionKeyMap.end()) {
                UnicharFontConfig_t uniCharConfig = keyFunction->second;
                sk_sp<SkFontMgr> mgr(SkFontMgr::RefDefault());
                sk_sp<SkTypeface> typeface(mgr->matchFamilyStyleCharacter(nullptr, SkFontStyle(), nullptr, 0, uniCharConfig.unicharValue));
                if (typeface) {
                    uniChar.appendUnichar(uniCharConfig.unicharValue);
                    font.setTypeface(typeface);
                    if(onHLTile)
                        font.setSize(oskLayout_.textHLFontSize* uniCharConfig.fontScaleFactor);
                    else
                        font.setSize(oskLayout_.textFontSize* uniCharConfig.fontScaleFactor);
                    keyName=(char *)uniChar.c_str();
                }
            }
        }
        OSKcanvas_->drawSimpleText(keyName, strlen(keyName), SkTextEncoding::kUTF8,textX,textY,font, textPaint);

#ifdef SHOW_FONT_PLACING_ON_HLTILE
        textPaint.setColor(SK_ColorRED);
        textPaint.setStrokeWidth(4);
        OSKcanvas_->drawPoint(textX,textY,textPaint);
        textPaint.setColor(SK_ColorGREEN);
        textPaint.setStrokeWidth(2);
        OSKcanvas_->drawLine(oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.fLeft,
                             oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.fTop+(oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.height()/2),
                             oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.fRight,
                             oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.fTop+(oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.height()/2),
                             textPaint);
        OSKcanvas_->drawLine(oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.fLeft+(oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.width()/2),
                             oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.fTop,
                             oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.fLeft+(oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.width()/2),
                             oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile.fBottom,
                             textPaint);

#endif/*SHOW_FONT_PLACING_ON_HLTILE*/

    }
}

void OnScreenKeyboard::drawOSKPartition() {
    if(oskConfig_.oskType != OSK_NUMERIC_KB) {
        SkPaint paint;
        paint.setColor((oskConfig_.oskTheme == OSK_LIGHT_THEME) ? LIGHT_THEME_PH_COLOR: DARK_THEME_PH_COLOR);
        paint.setStrokeWidth(2);
        unsigned int startY,endY,xpos,rowCount=oskLayout_.keyInfo->size()-1;
        unsigned int KeyCount=oskLayout_.keyInfo->at(rowCount).size()-1;
        endY=oskLayout_.keyPos->at(rowCount).at(KeyCount).highlightTile.fBottom+3;
        startY=oskLayout_.keyPos->at(0).at(0).highlightTile.fTop - oskLayout_.KBGroupConfig[oskLayout_.keyInfo->at(0).at(0).KBPartitionId].groupKeySpacing.y();
        for (unsigned int index=1;index<oskLayout_.keyInfo->at(0).size();index++) {
            if(oskLayout_.keyInfo->at(0).at(index).KBPartitionId != oskLayout_.keyInfo->at(0).at(index-1).KBPartitionId) {
                xpos=oskLayout_.keyPos->at(0).at(index).highlightTile.x()-(oskLayout_.keyPos->at(0).at(index).highlightTile.fLeft - oskLayout_.keyPos->at(0).at(index-1).highlightTile.fRight)/2;
                OSKcanvas_->drawLine(xpos,startY,xpos,endY,paint);
            }
        }
    }
}

void OnScreenKeyboard ::highlightFocussedKey(SkPoint index) {
    SkPaint paintObj;
    unsigned int rowIndex=index.y(),keyIndex=index.x();
    unsigned int lastRowIndex=lastFocussedKey_.y(),lastKeyIndex=lastFocussedKey_.x();

     RNS_PROFILE_START(HighlightOSKKey)
     //reset last focussed item
     paintObj.setColor(bgColor_);
     paintObj.setAntiAlias(true);
     OSKcanvas_->drawRect(oskLayout_.keyPos->at(lastRowIndex).at(lastKeyIndex).highlightTile,paintObj);
     drawFont({lastKeyIndex,lastRowIndex},fontColor_);

    //Hight current focussed item
    paintObj.setColor(HIGHLIGHT_BG_COLOR);
    OSKcanvas_->drawRect(oskLayout_.keyPos->at(rowIndex).at(keyIndex).highlightTile,paintObj);
    drawFont({keyIndex,rowIndex},HIGHLIGHT_FONT_COLOR,true);
    pushToDisplay();
    RNS_PROFILE_END(" Highlight Completion : ",HighlightOSKKey)
    return;
}
void OnScreenKeyboard::onExposeHandler(RnsShell::Window* window) {
    if(window  == OSKwindow_.get()) {
        sem_wait(&semReadyToDraw);
        if(exposeEventID_) {
            NotificationCenter::OSKCenter().removeListener(exposeEventID_);
            exposeEventID_=-1;
        }
        //Set up OSk configuration
        switch(oskConfig_.oskTheme) {
            case OSK_LIGHT_THEME:
                bgColor_ = LIGHT_THEME_BG_COLOR ;
                fontColor_ = LIGHT_THEME_FONT_COLOR;
            break;
            case OSK_DARK_THEME:
            default:
                bgColor_ = DARK_THEME_BG_COLOR ;
                fontColor_ =DARK_THEME_FONT_COLOR;
            break;
        }
        //Setup Screen dimension 
        SkSize mainScreenSize=RnsShell::PlatformDisplay::sharedDisplay().screenSize();
        if(ScreenSize != mainScreenSize) {
            generateOSKLayout=true;
            ScreenSize=mainScreenSize;
        } else {
            generateOSKLayout=false;
        }
        oskLayout_.textFontSize= FONT_SIZE *(ScreenSize.width()/baseScreenSize.width());
        oskLayout_.textHLFontSize= HIGHLIGHT_FONT_SIZE *(ScreenSize.width()/baseScreenSize.width());
        // Draw OSK
        OSKwindow_->show();
        OSKcanvas_->clear(bgColor_);
        drawPlaceHolder();
        drawOSK(oskConfig_.oskType);
        // Registering Key Pressed event
        std::function<void(rnsKey, rnsKeyAction)> handler = std::bind(&OnScreenKeyboard::onHWkeyHandler,this,
                                                                  std::placeholders::_1,
                                                                  std::placeholders::_2);
        OSKeventId_ = NotificationCenter::OSKCenter().addListener("onHWKeyEvent", handler);
    }
 }

void OnScreenKeyboard::onHWkeyHandler(rnsKey keyValue, rnsKeyAction eventKeyAction){
    if(eventKeyAction != RNS_KEY_Press) return;

    SkPoint hlCandidate;
    lastFocussedKey_=focussedKey_;
    unsigned int rowIndex=focussedKey_.y(),keyIndex=focussedKey_.x();
    RNS_LOG_DEBUG("KEY RECEIVED : "<<RNSKeyMap[keyValue]);

    switch( keyValue ) {
    /*Case 1: handle Enter/selection key*/
        case RNS_KEY_Select:
            handleSelection();
            lastFocussedKey_=hlCandidate=focussedKey_;
        break;
    /* Case  2: Process Navigation Keys*/
        case RNS_KEY_Right:
            hlCandidate= oskLayout_.siblingInfo->at(rowIndex).at(keyIndex).siblingRight;
        break;
        case RNS_KEY_Left:
            hlCandidate= oskLayout_.siblingInfo->at(rowIndex).at(keyIndex).siblingLeft;
        break;
        case RNS_KEY_Up:
            hlCandidate= oskLayout_.siblingInfo->at(rowIndex).at(keyIndex).siblingUp;
        break;
        case RNS_KEY_Down:
            hlCandidate= oskLayout_.siblingInfo->at(rowIndex).at(keyIndex).siblingDown;
        break;
        default:
        {
            bool keyFound=false;
            hlCandidate=focussedKey_;
            if(keyValue !=RNS_KEY_UnKnown ) {
    /*Case 3: Emit back other known keys*/
                NotificationCenter::OSKCenter().emit("onOSKKeyEvent", keyValue, eventKeyAction);
                for (unsigned int rowIndex=0; (rowIndex < oskLayout_.keyInfo->size()) && (!keyFound);rowIndex++) {
                    for (unsigned int index=0; index<oskLayout_.keyInfo->at(rowIndex).size();index++) {
                        if(oskLayout_.keyInfo->at(rowIndex).at(index).keyValue == keyValue) {
                            hlCandidate.set(index,rowIndex);
                            keyFound=true;
                            break;
                        }
                    }
                }
            } 
        }
        break;
    }
    focussedKey_ = hlCandidate;
    if(focussedKey_ != lastFocussedKey_) {
        highlightFocussedKey(focussedKey_);
    }
}

void OnScreenKeyboard::handleSelection() {
    unsigned int keyIndex=focussedKey_.x(),rowIndex=focussedKey_.y();
    if(oskLayout_.keyInfo->at(rowIndex).at(keyIndex).keyValue == RNS_KEY_Select) {
       exit();
    } else if (oskLayout_.keyInfo->at(rowIndex).at(keyIndex).keyType == TOGGLE_KEY){
        ToggleKeyMap :: iterator keyFunction =toggleKeyMap.find(oskLayout_.keyInfo->at(rowIndex).at(keyIndex).keyName);
        if((keyFunction != toggleKeyMap.end()) && (keyFunction->second != oskLayout_.KBLayoutType)) {
            oskLayout_.KBLayoutType=keyFunction->second;
            drawOSK(OSK_ALPHA_NUMERIC_KB);
        }
    }else {
        NotificationCenter::OSKCenter().emit("onOSKKeyEvent", oskLayout_.keyInfo->at(rowIndex).at(keyIndex).keyValue, RNS_KEY_Press);
    }
}

void OnScreenKeyboard::createOSKLayout(OSKTypes oskType) {

    if(oskType == OSK_NUMERIC_KB) {
        RNS_LOG_DEBUG("DRAW call for Numeric KB");
        oskLayout_.keyInfo=&numericKBKeyKeyInfo;
        oskLayout_.keyPos=&numericKBKeyPos;
        oskLayout_.siblingInfo=&numericKBKeySiblingInfo;
        oskLayout_.KBGroupConfig=numericKBGroupConfig;
    } else {
        if(oskLayout_.KBLayoutType == SYMBOL_LAYOUT) {
            RNS_LOG_DEBUG("DRAW call for AlphaNumeric-symbol KB");
            oskLayout_.keyInfo=&symbolKBKBKeyInfo;
            oskLayout_.keyPos=&symbolKBKBKeyPos;
            oskLayout_.siblingInfo=&symbolKBKBKeySiblingInfo;
            oskLayout_.KBGroupConfig=symbolKBGroupConfig;
        } else {
            RNS_LOG_DEBUG("DRAW call for AlphaNumeric KB : "<<((oskLayout_.KBLayoutType == ALPHA_UPPERCASE_LAYOUT)? "UpperCase" : "LowerCase"));
            oskLayout_.keyInfo=&alphaNumericKBKeyInfo;
            oskLayout_.keyPos=&alphaNumericKBKeyPos;
            oskLayout_.siblingInfo=&alphaNumericKBKeySiblingInfo;
            oskLayout_.KBGroupConfig=alphaNumericKBGroupConfig;
        }
    }
    unsigned int XscaleFactor = ScreenSize.width()/baseScreenSize.width();
    unsigned int YscaleFactor =ScreenSize.height()/baseScreenSize.height();
    oskLayout_.textFontSize= FONT_SIZE *XscaleFactor;
    oskLayout_.textHLFontSize= HIGHLIGHT_FONT_SIZE *XscaleFactor;

    // Generate key Position info based on screen Size
    // Not needed, if screen Size is not changed & Layout already created
    if((!oskLayout_.keyPos->empty()) && (! generateOSKLayout )) return;

    generateOSKLayout=false;//reset on creation
    RNS_LOG_DEBUG("Creating new Layout info for KB type : "<<oskConfig_.oskType<< " and Key Type : "<<oskLayout_.KBLayoutType);
    RNS_LOG_DEBUG("Create Layout request on resolution change : "<<generateOSKLayout <<" on Launch : "<<oskLayout_.keyPos->empty());

    if(oskLayout_.keyPos && oskLayout_.siblingInfo ) {
        oskLayout_.keyPos->clear();
        oskLayout_.siblingInfo->clear();
    }

    unsigned int groupKeyIndex{0},hlX,hlY,groupHLTileWidth,groupHLTileHeigth,groupID;
    int fontHeightAdjustment=0;
    SkPoint groupOffset,groupKeySpacing;
    SkRect bounds,boundsCapsHL,boundsHL;
    SkFont font,fontHL;
    char * keyName;
    SkString uniChar;

    unsigned int rowSize=oskLayout_.keyInfo->size();
    oskLayout_.keyPos->resize(rowSize);
    oskLayout_.siblingInfo->resize(rowSize);
    SkPoint oskStartpt{(START_OFFSET_OSK_X*ScreenSize.width()*XscaleFactor),
                       (START_OFFSET_OSK_Y*ScreenSize.height()*YscaleFactor)};

    for (unsigned int rowIndex = 0; rowIndex < rowSize; rowIndex++) {

        unsigned int columnSize=oskLayout_.keyInfo->at(rowIndex).size();
        oskLayout_.keyPos->at(rowIndex).resize(columnSize);
        oskLayout_.siblingInfo->at(rowIndex).resize(columnSize);
        if(!rowIndex ) oskLayout_.defaultFocussIndex.set((columnSize/2),rowIndex);

        for (unsigned int columnIndex = 0; columnIndex < columnSize; columnIndex++) {
    //1.  Calculate Highlight Tile
            groupID=oskLayout_.keyInfo->at(rowIndex).at(columnIndex).KBPartitionId;
            groupKeyIndex=(columnIndex !=0) ?((groupID != oskLayout_.keyInfo->at(rowIndex).at(columnIndex-1).KBPartitionId) ? 0: (groupKeyIndex+1)): columnIndex;
            keyName=(char*)oskLayout_.keyInfo->at(rowIndex).at(columnIndex).keyName;

            RNS_LOG_DEBUG("Group Index : "<<groupKeyIndex<<"Partition Id : "<<groupID);
            RNS_LOG_DEBUG("Group Offset : x "<<oskLayout_.KBGroupConfig[groupID].groupOffset.x()<<"y : "<<oskLayout_.KBGroupConfig[groupID].groupOffset.y());
            RNS_LOG_DEBUG("Group Key Spacing : x "<<oskLayout_.KBGroupConfig[groupID].groupKeySpacing.x()<<"y : "<<oskLayout_.KBGroupConfig[groupID].groupKeySpacing.x());

            if(!groupKeyIndex) {
                groupHLTileWidth=groupHLTileHeigth =(oskLayout_.textHLFontSize * oskLayout_.KBGroupConfig[groupID].maxTextLength* oskLayout_.KBGroupConfig[groupID].fontScaleFactor) * oskLayout_.KBGroupConfig[groupID].hlTileFontSizeMultiplier;
                if(oskLayout_.KBGroupConfig[groupID].maxTextLength) {
                    groupHLTileHeigth =(oskLayout_.textHLFontSize * oskLayout_.KBGroupConfig[groupID].fontScaleFactor * oskLayout_.KBGroupConfig[groupID].hlTileFontSizeMultiplier);
                }
                groupOffset=oskLayout_.KBGroupConfig[groupID].groupOffset;
                groupKeySpacing=oskLayout_.KBGroupConfig[groupID].groupKeySpacing;
                font.setSize(oskLayout_.textFontSize * oskLayout_.KBGroupConfig[groupID].fontScaleFactor);
                font.setEdging(SkFont::Edging::kAntiAlias);
                fontHL.setSize(oskLayout_.textHLFontSize * oskLayout_.KBGroupConfig[groupID].fontScaleFactor);
                fontHL.setEdging(SkFont::Edging::kAntiAlias);
            }
            /* Enchancement Note:To Handle Variable tile width on the same group: Need to add Tile width info in the  Layout
                and here on generating tile x,y co-ordinate, previous tiles spacing to be accumulated and considered as groupKeySpacing
             */
            hlX = oskStartpt.x()+groupOffset.x()+(( groupHLTileWidth + groupKeySpacing.x()) * groupKeyIndex);
            hlY = oskStartpt.y()+groupOffset.y()+ (( groupHLTileHeigth + groupKeySpacing.y()) * rowIndex);
            oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.setXYWH(hlX,hlY,groupHLTileWidth,groupHLTileHeigth);

    //2.  Calculate text draw position
            /*Calculate Font dimenesion */
            if(oskLayout_.keyInfo->at(rowIndex).at(columnIndex).keyType == FUNCTION_KEY) {
                FunctionKeymap :: iterator keyFunction =functionKeyMap.find(keyName);
                keyName=(char*)DRAW_FONT_FAILURE_INDICATOR;
                if(keyFunction != functionKeyMap.end()) {
                    UnicharFontConfig_t uniCharConfig = keyFunction->second;
                    sk_sp<SkFontMgr> mgr(SkFontMgr::RefDefault());
                    sk_sp<SkTypeface> typeface(mgr->matchFamilyStyleCharacter(nullptr, SkFontStyle(), nullptr, 0, uniCharConfig.unicharValue));
                    if (typeface) {
                        font.setTypeface(typeface);
                        fontHL.setTypeface(typeface);
                        font.setSize(oskLayout_.textFontSize * uniCharConfig.fontScaleFactor);
                        fontHL.setSize(oskLayout_.textHLFontSize * uniCharConfig.fontScaleFactor);
                        uniChar.reset();
                        uniChar.appendUnichar(uniCharConfig.unicharValue);
                        keyName=(char*)uniChar.c_str();
#ifdef SHOW_UNICHAR_FAMILY_INFO
                        RNS_LOG_INFO("Draw Unicahr: "<<uniChar.c_str());
                        typeface->getFamilyName(&familyName);
                        RNS_LOG_INFO("UniCahr Found in FAMILY :"<<familyName.c_str());
#endif/*SHOW_UNICHAR_FAMILY_INFO*/
                    } 
                }
            } else {
                if(( oskLayout_.keyInfo->at(rowIndex).at(columnIndex).keyType == TEXT_KEY ) && (isalpha(*keyName))) {
                    char upperCase=*keyName-LOWER_TO_UPPER_CASE_OFFSET;
                    fontHL.measureText(&upperCase, 1, SkTextEncoding::kUTF8, &boundsCapsHL);
                    oskLayout_.keyPos->at(rowIndex).at(columnIndex).textCapsHLXY.set(
                                (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.x() + (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.width() - boundsCapsHL.width() )/2),
                                (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.y() + (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.height() + boundsCapsHL.height() )/2));
                }
            }
            font.measureText(keyName, strlen(keyName), SkTextEncoding::kUTF8, &bounds);
            fontHL.measureText(keyName, strlen(keyName), SkTextEncoding::kUTF8,&boundsHL);
#if 0
            /* Fix : to adjust the font inside Higlight tile. Need for symobol like (|  `), which has greater 
               high decend or ascend
            */
            fontHeightAdjustment=0;
            if(( oskLayout_.keyPos->at(rowIndex).at(columnIndex).textHLXY.y() +(boundsHL.height()/2) )> oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.fBottom) fontHeightAdjustment =-3;
            if(( oskLayout_.keyPos->at(rowIndex).at(columnIndex).textHLXY.y() -(boundsHL.height()/2) )< oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.fTop) fontHeightAdjustment =3;
#endif
            oskLayout_.keyPos->at(rowIndex).at(columnIndex).textXY.set(
                        (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.x() + (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.width() - bounds.width() )/2),
                        (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.y() + (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.height() + bounds.height() )/2));
            oskLayout_.keyPos->at(rowIndex).at(columnIndex).textHLXY.set(
                        (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.x() + (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.width() - boundsHL.width() )/2),
                        (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.y() + (oskLayout_.keyPos->at(rowIndex).at(columnIndex).highlightTile.height() + boundsHL.height() )/2)+fontHeightAdjustment);

#ifdef SHOW_FONT_BOUNDS
            SkPaint paint;
            paint.setColor(SK_ColorRED);
            paint.setStyle(SkPaint::kStroke_Style);
            OSKcanvas_->drawRect(bounds,paint);
            paint.setColor(SK_ColorBLUE);
            OSKcanvas_->drawRect(boundsHL,paint);
#endif/*SHOW_CREATED_COORDINATE*/
        }
    }

//3.  Calculation Navigation index
     for (unsigned int rowIndex = 0; rowIndex < oskLayout_.keyInfo->size(); rowIndex++) {
         for (unsigned int columnIndex = 0; columnIndex < oskLayout_.keyInfo->at(rowIndex).size(); columnIndex++) {
             groupID=oskLayout_.keyInfo->at(rowIndex).at(columnIndex).KBPartitionId;
             unsigned int keyCount=oskLayout_.keyInfo->at(rowIndex).size(),index;
             unsigned int rowCount=oskLayout_.keyInfo->size();

             //Predicting sibling on Left
            index =(columnIndex) ? (columnIndex-1): keyCount-1;
            oskLayout_.siblingInfo->at(rowIndex).at(columnIndex).siblingLeft.set( index,rowIndex);

            //Predicting sibling on Right
            index =(columnIndex < keyCount-1) ? (columnIndex+1): 0;
            oskLayout_.siblingInfo->at(rowIndex).at(columnIndex).siblingRight.set( index,rowIndex);

            //Predicting neighbour for Up Navigation 
            index =rowIndex;
            bool siblingFound{false};
           for(unsigned int i=0;(i<rowCount) && (!siblingFound);i++ ) {
                index = (index  !=0 ) ? index -1 :rowCount-1;
                for( unsigned int i=0; i< oskLayout_.keyPos->at(index).size();i++) {
                    if(groupID == oskLayout_.keyInfo->at(index).at(i).KBPartitionId) {
                        if(( oskLayout_.keyPos->at(index).at(i).highlightTile.fLeft >= oskLayout_.keyPos->at(rowIndex ).at(columnIndex).highlightTile.x() ) ||
                            (  oskLayout_.keyPos->at(index).at(i).highlightTile.fRight >= oskLayout_.keyPos->at(rowIndex ).at(columnIndex).highlightTile.x())) {
                               oskLayout_.siblingInfo->at(rowIndex).at(columnIndex).siblingUp.set( i,index);
                               siblingFound=true;
                               break;
                        }
                    }
                }
            }

            //Predicting neighbour for down Navigation
            index =rowIndex;
            siblingFound=false;
            for(unsigned int i=0;(i<rowCount) && (!siblingFound);i++ ) {
                index = (index != (rowCount-1) ) ? index +1 :0;
                 for( unsigned int i=0; i< oskLayout_.keyPos->at(index).size();i++) {
                     if(groupID == oskLayout_.keyInfo->at(index).at(i).KBPartitionId) {
                         if(( oskLayout_.keyPos->at(index).at(i).highlightTile.fLeft >= oskLayout_.keyPos->at(rowIndex ).at(columnIndex).highlightTile.x() ) ||
                             (  oskLayout_.keyPos->at(index).at(i).highlightTile.fRight >= oskLayout_.keyPos->at(rowIndex ).at(columnIndex).highlightTile.x())) {
                                oskLayout_.siblingInfo->at(rowIndex).at(columnIndex).siblingDown.set( i,index);
                                siblingFound=true;
                                break;
                         }
                     }
                 }
             }
#ifdef SHOW_SIBLING_INFO
            unsigned int indexL=oskLayout_.siblingInfo->at(rowIndex).at(columnIndex).siblingLeft.x();
            unsigned int indexR=oskLayout_.siblingInfo->at(rowIndex).at(columnIndex).siblingRight.x();
            unsigned int upRow=oskLayout_.siblingInfo->at(rowIndex).at(columnIndex).siblingUp.y();
            unsigned int upKey=oskLayout_.siblingInfo->at(rowIndex).at(columnIndex).siblingUp.x();
            unsigned int downRow=oskLayout_.siblingInfo->at(rowIndex).at(columnIndex).siblingDown.y();
            unsigned int downKey=oskLayout_.siblingInfo->at(rowIndex).at(columnIndex).siblingDown.x();

            RNS_LOG_INFO(" Sibling key for key : "<<oskLayout_.keyInfo->at(rowIndex).at(columnIndex).keyName
                <<" left Key :  "<< oskLayout_.keyInfo->at(rowIndex).at(indexL).keyName
                <<"  rigt Key :  "<< oskLayout_.keyInfo->at(rowIndex).at(indexR).keyName
                <<"  Up Key :  "<< oskLayout_.keyInfo->at(upRow).at(upKey).keyName
                <<"  down Key :  "<< oskLayout_.keyInfo->at(downRow).at(downKey).keyName);
#endif/*SHOW_SIBLING_INFO*/
        }
    }
}

inline void OnScreenKeyboard::pushToDisplay() {
    backBuffer_->flushAndSubmit();
    std::vector<SkIRect> emptyRect;
    OSKwindowContext_->swapBuffers(emptyRect);
}

