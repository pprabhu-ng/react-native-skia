 /*
 * Copyright (C) 1994-2021 OpenTV, Inc. and Nagravision S.A.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef RNS_OSK_MAPPING_H
#define RNS_OSK_MAPPING_H

#include <vector>

#include "RNSKeyCodeMapping.h"
struct OSKLayout{
enum OSKTypes {
    OSK_DEFAULT,
    OSK_ALPHA_NUMERIC_KB=OSK_DEFAULT,
    OSK_CAP_ALPHA_NUMERIC,
    OSK_SYMBOLS,
};

enum OSKGroupKeys {
    OSK_GROUP_None = RNS_KEY_End,
    OSK_GROUP_Symbols,
    OSK_GROUP_Unknown,
    OSK_GROUP_End
};

struct KeyLayoutInfo {
    SkPoint        startPt{}; // Key TopLeft point{x,y}
    SkPoint        EndPt{};   // Key BottomRight point{x,y}
    rnsKey         keyValue;  // single keys
    OSKGroupKeys   groupKey;  // group keys
    unsigned int   indexSelf;
    unsigned int   indexLeft;
    unsigned int   indexRight;
    unsigned int   indexUp;
    unsigned int   indexDown;
};

/* KeyBoard Layout for Default OSK */
//typedef std::vector<std::vector<KeyLayoutInfo>> KBContainer;
#define DEFAULT_OSK_KEYS 46
KeyLayoutInfo DefaultOSK[DEFAULT_OSK_KEYS] = { 
	  //row 1
	   //KeyLayoutInfo
        { { 18,12 },{ 68,62},RNS_KEY_1 ,OSK_GROUP_None,0,9,1,40,10 }, 
        { { 76,12 },{ 125,62},RNS_KEY_2 ,OSK_GROUP_None,1,0,2,40,11 },
        { { 133,12 },{ 182,62},RNS_KEY_3 ,OSK_GROUP_None,2,1,3,41,12 },
        { { 190,12 },{ 239,62},RNS_KEY_4 ,OSK_GROUP_None,3,2,4,42,13 },
        { { 247,12 },{ 296,62},RNS_KEY_5 ,OSK_GROUP_None,4,3,5,43,14 },
        { { 302,12 },{ 353,62},RNS_KEY_6 ,OSK_GROUP_None,5,4,6,43,15 }, 
        { { 361,12 },{ 410,62},RNS_KEY_7 ,OSK_GROUP_None,6,5,7,44,16 },
        { { 418,12 },{ 467,62},RNS_KEY_8 ,OSK_GROUP_None,7,6,8,44,17 },
        { { 475,12 },{ 524,62},RNS_KEY_9 ,OSK_GROUP_None,8,7,9,45,18 },
        { { 532,12 },{ 581,62},RNS_KEY_0 ,OSK_GROUP_None,9,8,0,45,19 },
	  //row 2

        { { 18,69 },{ 68,119},RNS_KEY_a ,OSK_GROUP_None,10,19,11,0,20 },
        { { 76,69 },{ 125,119},RNS_KEY_b ,OSK_GROUP_None,11,10,12,1,21 },
        { { 133,69 },{ 182,119},RNS_KEY_c ,OSK_GROUP_None,12,11,13,2,22 },
        { { 190,69 },{ 239,119},RNS_KEY_d ,OSK_GROUP_None,13,12,14,3,23 },
        { { 247,69 },{ 296,119},RNS_KEY_e ,OSK_GROUP_None,14,13,15,4,24 },
        { { 304,69 },{ 353,119},RNS_KEY_f ,OSK_GROUP_None,15,14,16,5,25 }, 
        { { 361,69 },{ 410,119},RNS_KEY_g ,OSK_GROUP_None,16,15,17,6,26 },
        { { 418,69 },{ 467,119},RNS_KEY_h ,OSK_GROUP_None,17,16,18,7,27 },
        { { 475,69 },{ 524,119},RNS_KEY_i ,OSK_GROUP_None,18,17,19,8,28 },
        { { 532,69 },{ 581,119},RNS_KEY_j ,OSK_GROUP_None,19,18,10,9,29 },
	  //row 3
        { { 18,126 },{ 68,176},RNS_KEY_k ,OSK_GROUP_None,20,29,21,10,30 }, 
        { { 75,126 },{ 125,176},RNS_KEY_l ,OSK_GROUP_None,21,20,22,11,31 },
        { { 132,126 },{ 182,176},RNS_KEY_m ,OSK_GROUP_None,22,21,23,12,32 },
        { { 189,126 },{ 239,176},RNS_KEY_n ,OSK_GROUP_None,23,22,24,13,33 },
        { { 246,126 },{ 296,176},RNS_KEY_o ,OSK_GROUP_None,24,23,25,14,34 },
        { { 303,126 },{ 353,176},RNS_KEY_p ,OSK_GROUP_None,25,24,26,15,35 }, 
        { { 360,126 },{ 410,176},RNS_KEY_q ,OSK_GROUP_None,26,25,27,16,36},
        { { 417,126 },{ 467,176},RNS_KEY_r ,OSK_GROUP_None,27,26,28,17,37 },
        { { 474,126 },{ 524,176},RNS_KEY_s ,OSK_GROUP_None,28,27,29,18,38},
        { { 531,126 },{ 581,176},RNS_KEY_t ,OSK_GROUP_None,29,28,20,19,39 },
	  //row 4
        { { 18,183 },{ 68,233},RNS_KEY_u ,OSK_GROUP_None,30,39,31,20, 40}, 
        { { 75,183 },{ 125,233},RNS_KEY_v ,OSK_GROUP_None,31,30,32,21,40 },
        { { 132,183 },{ 182,233},RNS_KEY_w ,OSK_GROUP_None,32,31,33,22,41},
        { { 189,183 },{ 239,233},RNS_KEY_x ,OSK_GROUP_None,33,32,34,23,42 },
        { { 246,183 },{ 296,233},RNS_KEY_y ,OSK_GROUP_None,34,33,35,24,43 },
        { { 303,183 },{ 353,233},RNS_KEY_z ,OSK_GROUP_None,35,34,36,25, 43}, 
        { { 360,183 },{ 410,233},RNS_KEY_Exclam ,OSK_GROUP_None,36,35,37,26,44},
        { { 417,183 },{ 467,233},RNS_KEY_Comma ,OSK_GROUP_None,37,36,38,27,44 },
        { { 474,183 },{ 524,233},RNS_KEY_Period ,OSK_GROUP_None,38,37,39,28,45 },
        { { 531,183 },{ 581,233},RNS_KEY_At ,OSK_GROUP_None,39,38,30,29,45 },
	  //row 5
        { { 18,240 },{ 125,300},RNS_KEY_Shift_L ,OSK_GROUP_None,40,45,41,30, 0}, 
        { { 132,240 },{ 182,290},RNS_KEY_UnKnown ,OSK_GROUP_Symbols,41,40,42,32, 2},
        { { 189,240 },{ 239,290},RNS_KEY_UnKnown ,OSK_GROUP_Unknown,42,41,43,33, 3},
        { { 246,240 },{ 353,290},RNS_KEY_Space ,OSK_GROUP_None,43,42,44,34, 4},
        { { 360,240 },{ 467,290},RNS_KEY_Delete ,OSK_GROUP_None,44,43,45,36, 6},
        { { 474,240 },{ 581,290},RNS_KEY_Select ,OSK_GROUP_None,45,44,40,38, 8}
};

};
#endif// __RNSOSKMapping_h__
