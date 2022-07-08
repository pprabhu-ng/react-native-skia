 /*
 * Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef OSK_H
#define OSK_H

#include <semaphore.h>
#include <queue>

#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"

#include "NotificationCenter.h"
#include "RNSKeyCodeMapping.h"
#include "WindowDelegator.h"

namespace rns {
namespace sdk {

//Suported KeyBoards Layouts
enum OSKTypes {
  OSK_ALPHA_NUMERIC_KB,
  OSK_NUMERIC_KB,
  OSK_TYPE_END
};
//Suported KeyBoard Themes
enum OSKThemes {
  OSK_DARK_THEME,
  OSK_LIGHT_THEME
};
// OnScreenKeyBoard supported Return Key Types
enum OSKReturnKeyType {
  OSK_RETURN_KEY_DEFAULT,
  OSK_RETURN_KEY_SEARCH,
  OSK_RETURN_KEY_SUPPORT_END,
};
// OnScreenKeyBoard Error Codes
enum OSKErrorCode {
  OSK_LAUNCH_SUCCESS=0,
  OSK_ERROR_ANOTHER_INSTANCE_ACTIVE=-1,
  OSK_ERROR_LAUNCH_FAILED=-2
};
//Configuration to be specified while Launching OSK
struct OSKConfig {
  OSKTypes        type;
  OSKThemes       theme;
  OSKReturnKeyType    returnKeyLabel;
  std::string     placeHolderName;
  bool      enablesReturnKeyAutomatically;
};
// Default OSK Configuration to be used, in case client doesn't have any preference
static OSKConfig defaultOSKConfig={ OSK_ALPHA_NUMERIC_KB, OSK_DARK_THEME, OSK_RETURN_KEY_DEFAULT,"", false };

//KeyBoard Layout Type
enum KBLayoutType {
      ALPHA_LOWERCASE_LAYOUT,
      ALPHA_UPPERCASE_LAYOUT,
      SYMBOL_LAYOUT,
      NUMERIC_LAYOUT
};
// Key Type specifing it's functionality
enum keyType {
  KEY_TYPE_TOGGLE,
  KEY_TYPE_TEXT,
  KEY_TYPE_FUNCTION,
  KEY_TYPE_COUNT
};
typedef struct keyPlacementConfig {
  SkPoint groupOffset;
  SkPoint groupKeySpacing;
  float   hlTileFontSizeMultiplier;
  float   fontScaleFactor;
  unsigned int maxTextLength;
}keyPlacementConfig_t;
typedef struct keyPosition {
  SkPoint        textXY{}; // text X,Y to draw
  SkPoint        textHLXY{}; // Text X,Y to draw on Highlight Tile
  SkPoint        textCapsHLXY{}; // Text X,Y for Upper Case Alphabets
  SkRect         highlightTile; // Highlight Tile coverage
}keyPosition_t;
typedef struct KeyInfo {
  const char *   keyName;
  rnsKey         keyValue;
  keyType        keyType;
  unsigned int   kbPartitionId;
}KeyInfo_t;
typedef struct KeySiblingInfo {
  SkPoint siblingRight;
  SkPoint siblingLeft;
  SkPoint siblingUp;
  SkPoint siblingDown;
}keySiblingInfo_t;
typedef std::vector<std::vector<KeyInfo_t>> KBLayoutKeyInfoContainer;
typedef std::vector<std::vector<keyPosition_t>> KBLayoutKeyPosContainer;
typedef std::vector<std::vector<keySiblingInfo_t>> KBLayoutSibblingInfoContainer;

class OnScreenKeyboard : public WindowDelegator{

  public:
    static OnScreenKeyboard& getInstance(); // Interface to get OSK singleton object
    static OSKErrorCode launch(OSKConfig oskConfig=defaultOSKConfig);// Interface to launch OSK
    static void exit(); //Interface to quit OSK
    static void updatePlaceHolderString(std::string TIDisplayString);

  private:

    enum OSKState {
      OSK_STATE_LAUNCH_INPROGRESS,
      OSK_STATE_ACTIVE,
      OSK_STATE_EXIT_INPROGRESS,
      OSK_STATE_INACTIVE,
    };
    struct OSKLayout {
      KBLayoutKeyInfoContainer*  keyInfo;
      KBLayoutKeyPosContainer*    keyPos;
      KBLayoutSibblingInfoContainer*    siblingInfo;
      keyPlacementConfig_t*          kbGroupConfig;
      KBLayoutType      kbLayoutType;
      unsigned int      textFontSize;
      unsigned int      textHLFontSize;
      unsigned int      horizontalStartOffset;
      SkPoint           defaultFocussIndex;
      SkPoint           returnKeyIndex;
    };

    OnScreenKeyboard(){};
    ~OnScreenKeyboard(){};

    void launchOSKWindow(OSKConfig oskConfig);
    void onHWkeyHandler(rnsKey key, rnsKeyAction eventKeyAction);
    void createOSKLayout(OSKTypes KBtype );

    void emitOSKKeyEvent(rnsKey keyValue);
    void windowReadyToDrawCB();

    void drawHighLightOnKey(SkPoint index);
    void drawOSK();
    void drawKBLayout(OSKTypes oskType);
    void drawKBKeyFont(SkPoint index,SkColor color,bool onHLTile=false);

// Members for OSK Layout & sytling
    OSKConfig     oskConfig_;
    OSKLayout     oskLayout_;
    SkSize        screenSize_{0,0};
    SkColor       bgColor_{SK_ColorWHITE};
    SkColor       fontColor_{SK_ColorWHITE};

// Members for OSK operations
    int           subWindowKeyEventId_{-1};
    bool          generateOSKLayout_{true};
    SkPoint       currentFocussIndex_{};
    SkPoint       lastFocussIndex_{};
    std::string   displayString_{}; // Text to be displayed on screen
    std::string   lastDisplayedString_{};
    OSKState      oskState_{OSK_STATE_INACTIVE};
    bool          autoActivateReturnKey{false};
};

}// namespace sdk
}// namespace rns

#endif //OSK_H
