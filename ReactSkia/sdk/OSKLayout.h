 /*
 * Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef OSK_LAYOUT_H
#define OSK_LAYOUT_H

#include "RNSKeyCodeMapping.h"

/* KeyBoard Layout for Alpha Numeric KeyBoard */

enum partitionID {
    GROUP1,
    GROUP2,
    GROUP3,
    GROUP4,
    MAX_GROUP=GROUP4
};
#define LOWER_TO_UPPER_CASE_OFFSET 32
#define ALPHA_NUMERIC_KB_PARTITION_COUNT   4
KBLayoutKeyPosContainer alphaNumericKBKeyPos;
KBLayoutSibblingInfoContainer alphaNumericKBKeySiblingInfo;
KBLayoutKeyInfoContainer alphaNumericKBKeyInfo= {
    //row 1
    {
        {"123ABC",RNS_KEY_UnKnown,TOGGLE_KEY,GROUP1},
        {"1",RNS_KEY_1,TEXT_KEY,GROUP2},
        {"2",RNS_KEY_2,TEXT_KEY,GROUP2},
        {"3",RNS_KEY_3,TEXT_KEY,GROUP2},
        {"a",RNS_KEY_a,TEXT_KEY,GROUP3},
        {"b",RNS_KEY_b,TEXT_KEY,GROUP3},
        {"c",RNS_KEY_c,TEXT_KEY,GROUP3},
        {"d",RNS_KEY_d,TEXT_KEY,GROUP3},
        {"e",RNS_KEY_e,TEXT_KEY,GROUP3},
        {"f",RNS_KEY_f,TEXT_KEY,GROUP3},
        {"g",RNS_KEY_g,TEXT_KEY,GROUP3},
        {"h",RNS_KEY_h,TEXT_KEY,GROUP3},
        {"i",RNS_KEY_i,TEXT_KEY,GROUP3},
        {"space",RNS_KEY_Space,FUNCTION_KEY,GROUP4}
    },
     //row 2
    {
        {"123abc",RNS_KEY_UnKnown,TOGGLE_KEY,GROUP1},
        {"4",RNS_KEY_4,TEXT_KEY,GROUP2},
        {"5",RNS_KEY_5,TEXT_KEY,GROUP2},
        {"6",RNS_KEY_6,TEXT_KEY,GROUP2},
        {"j",RNS_KEY_j,TEXT_KEY,GROUP3},
        {"k",RNS_KEY_k,TEXT_KEY,GROUP3},
        {"l",RNS_KEY_l,TEXT_KEY,GROUP3},
        {"m",RNS_KEY_m,TEXT_KEY,GROUP3},
        {"n",RNS_KEY_n,TEXT_KEY,GROUP3},
        {"o",RNS_KEY_o,TEXT_KEY,GROUP3},
        {"p",RNS_KEY_p,TEXT_KEY,GROUP3},
        {"q",RNS_KEY_q,TEXT_KEY,GROUP3},
        {"r",RNS_KEY_r,TEXT_KEY,GROUP3},
        {"delete",RNS_KEY_Delete,FUNCTION_KEY,GROUP4}
    },
    {
    //row 3
        {"@?#",RNS_KEY_UnKnown,TOGGLE_KEY,GROUP1},
        {"7",RNS_KEY_7,TEXT_KEY,GROUP2},
        {"8",RNS_KEY_8,TEXT_KEY,GROUP2},
        {"9",RNS_KEY_9,TEXT_KEY,GROUP2},
        {"s",RNS_KEY_s,TEXT_KEY,GROUP3},
        {"t",RNS_KEY_t,TEXT_KEY,GROUP3},
        {"u",RNS_KEY_u,TEXT_KEY,GROUP3},
        {"v",RNS_KEY_v,TEXT_KEY,GROUP3},
        {"w",RNS_KEY_w,TEXT_KEY,GROUP3},
        {"x",RNS_KEY_x,TEXT_KEY,GROUP3},
        {"y",RNS_KEY_y,TEXT_KEY,GROUP3},
        {"z",RNS_KEY_z,TEXT_KEY,GROUP3},
        {".",RNS_KEY_Period,TEXT_KEY,GROUP3},
        {"done",RNS_KEY_Select,FUNCTION_KEY,GROUP4}
    },
    {
      //row 4
        {"-",RNS_KEY_Minus,TEXT_KEY,GROUP2},
        {"0",RNS_KEY_0,TEXT_KEY,GROUP2},
        {"_",RNS_KEY_Underscore,TEXT_KEY,GROUP2 },
        {"!",RNS_KEY_Exclam,TEXT_KEY,GROUP3},
        {"@",RNS_KEY_At,TEXT_KEY,GROUP3 },
        {"#",RNS_KEY_Numbersign,TEXT_KEY,GROUP3},
        {"?",RNS_KEY_Question,TEXT_KEY,GROUP3},
        {"*",RNS_KEY_Asterisk,TEXT_KEY,GROUP3},
        {",",RNS_KEY_Comma,TEXT_KEY,GROUP3},
        {";",RNS_KEY_Semicolon,TEXT_KEY,GROUP3},
        {"right", RNS_KEY_Right,FUNCTION_KEY,GROUP3},
        {"left", RNS_KEY_Left,FUNCTION_KEY,GROUP3}
    }
};

keyPlacementConfig_t alphaNumericKBGroupConfig[ALPHA_NUMERIC_KB_PARTITION_COUNT]={
    //GROUP1-Toggle key
    {{20,60},{15,30},1.3,0.7,4},
    //GROUP2-Numeric key
    {{150,40},{15,15},1.25,1,1},
    //GROUP3-Alpha & SYmbol key
    {{320,40},{15,15},1.25,1,1},
    //GROUP4-Function key
    {{820,50},{15,15},1.5,1,1}
};


/* KeyBoard Layout for Symbol Key Board */

KBLayoutSibblingInfoContainer symbolKBKBKeySiblingInfo;
KBLayoutKeyPosContainer symbolKBKBKeyPos;
#define SYMBOL_KB_PARTITION_COUNT 3
KBLayoutKeyInfoContainer symbolKBKBKeyInfo={
    {
        {"123ABC",RNS_KEY_UnKnown,TOGGLE_KEY,GROUP1},
        {"!",RNS_KEY_Exclam,TEXT_KEY,GROUP2},
        {"@",RNS_KEY_At,TEXT_KEY,GROUP2},
        {"#",RNS_KEY_Numbersign,TEXT_KEY,GROUP2},
        {"$",RNS_KEY_Dollar,TEXT_KEY,GROUP2},
        {"^",RNS_KEY_Asciicircum,TEXT_KEY,GROUP2},
        {"&",RNS_KEY_Ampersand,TEXT_KEY,GROUP2},
        {"*",RNS_KEY_Asterisk,TEXT_KEY,GROUP2},
        {"~",RNS_KEY_Asciitilde,TEXT_KEY,GROUP2},
        {"space",RNS_KEY_Space,FUNCTION_KEY,GROUP3}
    },
    {
     //row 2
        {"123abc",RNS_KEY_UnKnown,TOGGLE_KEY,GROUP1},
        {"(",RNS_KEY_ParenLeft,TEXT_KEY,GROUP2},
        {")",RNS_KEY_ParenRight,TEXT_KEY,GROUP2},
        {",",RNS_KEY_Comma,TEXT_KEY,GROUP2},
        {".",RNS_KEY_Period,TEXT_KEY,GROUP2},
        {"?",RNS_KEY_Question,TEXT_KEY,GROUP2},
        {"\"",RNS_KEY_Quotedbl,TEXT_KEY,GROUP2},
        {";",RNS_KEY_Semicolon,TEXT_KEY,GROUP2},
        {":",RNS_KEY_Colon,TEXT_KEY,GROUP2},
        {"delete",RNS_KEY_Delete,FUNCTION_KEY,GROUP3}
    },
    {
    //row 3
        {"@?#",RNS_KEY_UnKnown ,TOGGLE_KEY,GROUP1},
        {"|",RNS_KEY_Bar,TEXT_KEY,GROUP2},
        {"\\",RNS_KEY_BackSlash,TEXT_KEY,GROUP2 },
        {"/",RNS_KEY_Shash,TEXT_KEY,GROUP2},
        {"{",RNS_KEY_BraceLeft,TEXT_KEY,GROUP2},
        {"}",RNS_KEY_BraceRight,TEXT_KEY,GROUP2},
        {"[",RNS_KEY_BracketLeft,TEXT_KEY,GROUP2},
        {"]",RNS_KEY_BracketRight,TEXT_KEY,GROUP2},
        {"`",RNS_KEY_Grave,TEXT_KEY,GROUP2},
        {"done",RNS_KEY_Select,FUNCTION_KEY,GROUP3}
    },
    {
      //row 4
        {"'",RNS_KEY_Apostrophe,TEXT_KEY,GROUP2},
        {"=",RNS_KEY_Equal,TEXT_KEY,GROUP2},
        {"+",RNS_KEY_Plus,TEXT_KEY,GROUP2},
        {"-",RNS_KEY_Minus,TEXT_KEY,GROUP2},
        {"_",RNS_KEY_Underscore,TEXT_KEY,GROUP2},
        {"%",RNS_KEY_Percent,TEXT_KEY,GROUP2},
        {">",RNS_KEY_Greater,TEXT_KEY,GROUP2},
        {"<",RNS_KEY_Lesser,TEXT_KEY,GROUP2}
    }
};

keyPlacementConfig_t symbolKBGroupConfig[SYMBOL_KB_PARTITION_COUNT]={
    //GROUP1-Toggle key
    {{20,60},{15,30},1.3,0.7,4},
    //GROUP2-Symbol key
    {{150,40},{15,15},1.25,1,1},
    //GROUP3-Function key
    {{590,50},{15,15},1.5,1,1}
};


/* KeyBoard Layout for Numeric Key Board */

KBLayoutSibblingInfoContainer numericKBKeySiblingInfo;
KBLayoutKeyPosContainer numericKBKeyPos;
#define NUMERIC_KB_PARTITION_COUNT 1
KBLayoutKeyInfoContainer numericKBKeyKeyInfo={
    {
        {"1",RNS_KEY_1,TEXT_KEY,GROUP1},
        {"2",RNS_KEY_2,TEXT_KEY,GROUP1},
        {"3",RNS_KEY_3,TEXT_KEY,GROUP1},
        {"-",RNS_KEY_Underscore,TEXT_KEY,GROUP1}
    },
    {
        {"4",RNS_KEY_4,TEXT_KEY,GROUP1},
        {"5",RNS_KEY_5,TEXT_KEY,GROUP1},
        {"6",RNS_KEY_6,TEXT_KEY,GROUP1},
        {"space",RNS_KEY_Space,FUNCTION_KEY,GROUP1}
    },
    {
        //row 3
        {"7",RNS_KEY_7,TEXT_KEY,GROUP1},
        {"8",RNS_KEY_8,TEXT_KEY,GROUP1},
        {"9",RNS_KEY_9,TEXT_KEY,GROUP1},
        {"delete",RNS_KEY_Delete,FUNCTION_KEY,GROUP1}
    },
    {
          //row 4
        {",",RNS_KEY_Comma,TEXT_KEY,GROUP1},
        {"0",RNS_KEY_0,TEXT_KEY,GROUP1},
        {".",RNS_KEY_Period,TEXT_KEY,GROUP1 },
        {"done",RNS_KEY_Select,FUNCTION_KEY,GROUP1}
    }
};

keyPlacementConfig_t numericKBGroupConfig[NUMERIC_KB_PARTITION_COUNT] = {
    {{250,30},{40,40},1.3,1.5,1}
};


typedef std::map<std::string,KBLayoutType> ToggleKeyMap;

ToggleKeyMap toggleKeyMap = {
    {"123ABC",ALPHA_UPPERCASE_LAYOUT},
    {"123abc",ALPHA_LOWERCASE_LAYOUT},
    {"@?#",SYMBOL_LAYOUT}
};

typedef struct unicharFontConfig {
    unsigned int unicharValue;
    float fontScaleFactor;
}UnicharFontConfig_t;

typedef std::map<std::string,UnicharFontConfig_t> FunctionKeymap;

FunctionKeymap functionKeyMap = {
    {"delete",{0x232B,1}},
    {"space",{0x23B5,2}},
    {"done",{0x23CE,1.5}},
    {"right",{0x25BA,1}},
    {"left",{0x25c4,1}}
};

#endif// OSK_LAYOUT_H
