 /*
 * Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef OSK_H
#define OSK_H

#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"

#include "rns_shell/compositor/Compositor.h"
#include "rns_shell/common/Window.h"
#include "rns_shell/platform/graphics/PlatformDisplay.h"
#include "rns_shell/platform/graphics/WindowContextFactory.h"

#include "NotificationCenter.h"
#include "RNSKeyCodeMapping.h"
//Types of Suported KeyBoard
enum OSKTypes {
    OSK_DEFAULT_TYPE,
    OSK_ALPHA_NUMERIC_KB=OSK_DEFAULT_TYPE,
    OSK_NUMERIC_KB,
    OSK_TYPE_END
};
//Suported Themes
enum OSKThemes {
    OSK_DEFAULT_THEME,
    OSK_DARK_THEME=OSK_DEFAULT_THEME,
    OSK_LIGHT_THEME
};
enum KBLayoutType {
    ALPHA_LOWERCASE_LAYOUT,
    ALPHA_UPPERCASE_LAYOUT,
    SYMBOL_LAYOUT
};

enum keyType {
    TOGGLE_KEY ,
    TEXT_KEY,
    FUNCTION_KEY,
    KEY_TYPE_COUNT
};


//Configuration to be specified while OSK Launch
struct OSKConfig {
    OSKTypes  oskType;
    OSKThemes oskTheme;
    const char *    placeHolderName;
    const char *    returnKeyLabel;
    bool      enablesReturnKeyAutomatically;
};

typedef struct keyPosition {
    SkPoint        textXY{}; // text X,Y to draw
    SkPoint        textHLXY{}; // font's X,Y to draw
    SkPoint        textCapsHLXY{}; // font's X,Y to draw
    SkRect         highlightTile; // Area to highlight on Key Focus
}keyPosition_t;

typedef struct KeyInfo {
    const char *   keyName;
    rnsKey         keyValue;
    keyType        keyType;
    unsigned int   KBPartitionId;
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
    float hlTileFontSizeMultiplier;
    float fontScaleFactor;
    unsigned int maxTextLength;
}keyPlacementConfig_t;

typedef std::vector<std::vector<KeyInfo_t>> KBLayoutKeyInfoContainer;
typedef std::vector<std::vector<keyPosition_t>> KBLayoutKeyPosContainer;
typedef std::vector<std::vector<keySiblingInfo_t>> KBLayoutSibblingInfoContainer;


struct OSKLayout {
    KBLayoutKeyInfoContainer*  keyInfo;
    KBLayoutKeyPosContainer*    keyPos;
    KBLayoutSibblingInfoContainer*    siblingInfo;
    keyPlacementConfig_t*          KBGroupConfig;
    OSKTypes          KBtype;
    KBLayoutType      KBLayoutType;
    unsigned int      textFontSize;
    unsigned int      textHLFontSize;
};

class OnScreenKeyboard {
    public:
        OnScreenKeyboard();
        static void launch(OSKConfig oskConfig);// Interface to launch OSK
        static void exit(); //Interface to quit OSK
    private:
        std::unique_ptr<RnsShell::Window> OSKwindow_;
        std::unique_ptr<RnsShell::WindowContext> OSKwindowContext_;
        sk_sp<SkSurface> backBuffer_;
        SkCanvas *OSKcanvas_=nullptr;

        void createOSKLayout(OSKTypes KBtype );
        void onHWkeyHandler(rnsKey key, rnsKeyAction eventKeyAction);
        void highlightFocussedKey(SkPoint index);
        void handleSelection();
        void drawOSK(OSKTypes oskType);
        void drawPlaceHolder();
        void drawFont(SkPoint index,SkColor color,bool onHLTile=false);
        void drawOSKPartition();
        void pushToDisplay();
        void configureScreenSize();

// State Maintainence
        const char *  placeHolderName{nullptr};
        const char*   returnKeyLabel_{nullptr};
        SkPoint       focussedIndex_{0};
        SkPoint       lastFocussedIndex_{0};
        OSKLayout     oskLayout_{0};
        OSKThemes     oskTheme_{OSK_DEFAULT_THEME};
        SkColor       bgColor_{SK_ColorWHITE};
        SkColor       fontColor_{SK_ColorWHITE};
        bool          ctrlReturnKey_{false};
};

#endif //OSK_H
