 /*
 * Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef OSK_H
#define OSK_H

#include <semaphore.h>

#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"

#include "rns_shell/compositor/Compositor.h"
#include "rns_shell/common/Window.h"
#include "rns_shell/platform/graphics/PlatformDisplay.h"
#include "rns_shell/platform/graphics/WindowContextFactory.h"

#include "NotificationCenter.h"
#include "RNSKeyCodeMapping.h"

//Suported KeyBoards
enum OSKTypes {
    OSK_ALPHA_NUMERIC_KB,
    OSK_NUMERIC_KB,
    OSK_TYPE_END
};
//Suported Themes
enum OSKThemes {
    OSK_DARK_THEME,
    OSK_LIGHT_THEME
};
// OSK Error Code
enum OSKErrorCode {
    OSK_LAUNCH_SUCCESS=0,
    OSK_ERROR_ANOTHER_INSTANCE_ACTIVE=-1,
    OSK_ERROR_LAUNCH_FAILED=-2
};
enum KBLayoutType {
    ALPHA_LOWERCASE_LAYOUT,
    ALPHA_UPPERCASE_LAYOUT,
    SYMBOL_LAYOUT,
    NUMERIC_LAYOUT
};

enum keyType {
    KEY_TYPE_TOGGLE,
    KEY_TYPE_TEXT,
    KEY_TYPE_FUNCTION,
    KEY_TYPE_COUNT
};

//Configuration to be specified while OSK Launch
struct OSKConfig {
    OSKTypes        type;
    OSKThemes       theme;
    const char *    placeHolderName;
    const char *    returnKeyLabel;
    bool      enablesReturnKeyAutomatically;
};

// Default OSK Configuration
static OSKConfig defaultOSKConfig={ OSK_ALPHA_NUMERIC_KB, OSK_DARK_THEME, nullptr, "done", false };

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

typedef struct keyPlacementConfig {
    SkPoint groupOffset;
    SkPoint groupKeySpacing;
    float   hlTileFontSizeMultiplier;
    float   fontScaleFactor;
    unsigned int maxTextLength;
}keyPlacementConfig_t;

typedef std::vector<std::vector<KeyInfo_t>> KBLayoutKeyInfoContainer;
typedef std::vector<std::vector<keyPosition_t>> KBLayoutKeyPosContainer;
typedef std::vector<std::vector<keySiblingInfo_t>> KBLayoutSibblingInfoContainer;


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
};

class OnScreenKeyboard {

    public:
        static OnScreenKeyboard& getInstance(); // Interface to get OSK singleton object
        static OSKErrorCode launch(OSKConfig oskConfig=defaultOSKConfig);// Interface to launch OSK
        static void exit(); //Interface to quit OSK
        static bool IsKBActive() {
             OnScreenKeyboard &oskHandle=OnScreenKeyboard::getInstance();
             return oskHandle.isOSKActive_;
        } // Interface to get whether is in launch or exit state

    private:
        OnScreenKeyboard(){};

        void cleanUpOSKInstance();
        bool prepareToLaunch();
        bool createOSkWindow();
        void showOSKWindow();
        void pushToDisplay();

        void onHWkeyHandler(rnsKey key, rnsKeyAction eventKeyAction);
        void onExposeHandler(RnsShell::Window* window);

        void createOSKLayout(OSKTypes KBtype );
        void highlightFocussedKey(SkPoint index);
        void handleSelection();
        void drawOSK(OSKTypes oskType);
        void drawPlaceHolder();
        void drawFont(SkPoint index,SkColor color,bool onHLTile=false);
        void drawOSKPartition();

// Maintainables
        RnsShell::PlatformDisplay::Type platformType_;
        RnsShell::Window* oskWindow_;
        std::unique_ptr<RnsShell::WindowContext> oskWindowContext_;
        sk_sp<SkSurface> backBuffer_;
        SkCanvas *oskCanvas_{nullptr};
        int exposeEventID_{-1};
        int oskEventId_{-1};
        sem_t semReadyToDraw;/* To sync expose event & window creation for x11 backend*/
        bool  isOSKActive_{false};

        OSKConfig     oskConfig_;
        OSKLayout     oskLayout_;
        bool          generateOSKLayout_{true};
        SkSize        screenSize_{0,0};
        SkPoint       focussedKey_{0};
        SkPoint       lastFocussedKey_{0};
        SkColor       bgColor_{SK_ColorWHITE};
        SkColor       fontColor_{SK_ColorWHITE};
};
#endif //OSK_H

