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
//Configuration to be specified while OSK Launch
struct OSKConfig {
    OSKTypes  oskType;
    OSKThemes oskTheme;
    const char *    placeHolderName;
    const char *    returnKeyLabel;
    bool      enablesReturnKeyAutomatically;
};

enum keyTypes {
    ALPHA_LOWERCASE_KEYS = 1<<0,
    ALPHA_UPPERCASE_KEYS = 1<<1,
    SYMBOL_KEYS = 1<<2,
    NUMERIC_KEYS = 1<<3,
    GROUP_KEYS =  1<<4,
    ACTION_KEYS =  1<<5,
    KEY_TYPE_End
};

typedef struct keyPosition {
    SkPoint        textXY{}; // text X,Y to draw
    SkPoint        textHLXY{}; // font's X,Y to draw
    SkPoint        textCapsHLXY{}; // font's X,Y to draw
    SkRect         highlightTile; // Area to highlight on Key Focus
}keyPosition_t;

typedef struct KeyLayoutInfo {
    const char *   keyName;
    keyTypes       keyType;
    SkPoint        keySpacing; // X,Y spacing for teh key
    SkPoint        indexes;   // Row & Column index of the key
    rnsKey         keyValue;  // RNS key value
    unsigned int   indexLeft; // Index of the key on immediate Left
    unsigned int   indexRight; // Index of the key on immediate Right
    unsigned int   indexUp;  // Index of the key on immediate Up
    unsigned int   indexDown; // Index of the key on immediate down
    const char *   keyCaps;    // info about associated key, if any ex: Alpha lower & upper
    uint32_t       unicharValue;  //unichar vale of fonticon for non KB font's
}KeyLayoutInfo_t;

struct OSKLayout {
    KeyLayoutInfo_t*  keyInfo;
    keyPosition_t*    keyPos;
    unsigned int      keysCount;
    keyTypes          KBKeyType;
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
        void highlightFocussedKey(unsigned int index);
        void handleSelection();
        void drawOSK(OSKTypes oskType);
        void drawPlaceHolder();
        void drawFont(unsigned int index,SkColor color,bool onHLTile=false);
        void drawOSKPartition();
        void pushToDisplay();
        void configureScreenSize();

// State Maintainence
        const char *  placeHolderName{nullptr};
        const char*   returnKeyLabel_{nullptr};
        unsigned int  focussedIndex_{0};
        unsigned int  lastFocussedIndex_{0};
        OSKLayout     oskLayout_;
        OSKThemes     oskTheme_;
        SkColor       bgColor_{SK_ColorWHITE};
        SkColor       fontColor_{SK_ColorWHITE};
        bool          ctrlReturnKey_{false};
};

#endif //OSK_H
