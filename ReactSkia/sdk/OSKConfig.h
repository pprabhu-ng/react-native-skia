 /*
 * Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef OSK_CONFIG_H
#define OSK_CONFIG_H

// OSK styling configuration 
#define FONT_SIZE                    24
// Dark Theme configuration
#define DARK_THEME_BG_COLOR          SkColorSetARGB(0xFF, 0x34, 0x34, 0x34)
#define DARK_THEME_FONT_COLOR        SK_ColorWHITE
#define DARK_THEME_PH_COLOR          SK_ColorDKGRAY
// LIGHT Theme configuration
#define LIGHT_THEME_BG_COLOR         SK_ColorGRAY
#define LIGHT_THEME_FONT_COLOR       SK_ColorBLACK
#define LIGHT_THEME_PH_COLOR         SK_ColorLTGRAY
//Highlight configuration
#define HIGHLIGHT_AREA_WIDTH          40
#define HIGHLIGHT_AREA_HEIGTH         40
#define HIGHLIGHT_FONT_SIZE           28
#define HIGHLIGHT_BG_COLOR           SK_ColorWHITE
#define HIGHLIGHT_FONT_COLOR         SK_ColorBLACK
//OSK Screen & component placement confiuration
#define OSK_WINDOW_WIDTH              1 /*100% of Screen Size*/
#define OSK_WINDOW_HEIGTH             1

#define START_OFFSET_NAME_X          0.1 /*10% offset from origin*/
#define START_OFFSET_NAME_Y          0.1
#define START_OFFSET_PLACEHOLDER_X   0.1
#define START_OFFSET_PLACEHOLDER_Y   0.2
#define PLACEHOLDER_LENGTH           0.7 /*Total display view will be x% of the screen size*/
#define PLACEHOLDER_HEIGHT           50
#define PLACEHOLDER_FONT_COLOR    SK_ColorBLACK
#define PLACEHOLDER_FONT_SIZE         20
#define START_OFFSET_OSK_X            0.1
#define START_OFFSET_OSK_Y            0.3
#define KEY_HORIZONTAL_GAP            45
#define KEY_VERTICAL_GAP              55
//Numeric KB COnfiguration
#define NUM_KB_HORIZONTAL_GAP         (KEY_HORIZONTAL_GAP*2)
#define NUM_KB_VERTICAL_GAP           (KEY_VERTICAL_GAP*2)
// Defaults 
#define DEFAULT_THEME_BG_COLOR       DARK_THEME_BG_COLOR // default as Dark Theme
#define DEFAULT_THEME_FONT_COLOR     DARK_THEME_FONT_COLOR  // default as Dark Theme
#define DEFAULT_RETURN_KEY_LABEL     "done"
#endif //OSK_CONFIG_H