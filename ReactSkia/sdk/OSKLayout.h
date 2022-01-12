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
};

/* KeyBoard Layout for Default OSK */
typedef std::vector<std::vector<KeyLayoutInfo>> KBContainer;

KBContainer DefaultOSK = {
	  //row 1
{ 
	   //KeyLayoutInfo
        { { 18,12 },{ 68,62},RNS_KEY_1 ,OSK_GROUP_None }, 
        { { 76,12 },{ 125,62},RNS_KEY_2 ,OSK_GROUP_None },
        { { 133,12 },{ 182,62},RNS_KEY_3 ,OSK_GROUP_None },
        { { 190,12 },{ 239,62},RNS_KEY_4 ,OSK_GROUP_None },
        { { 247,12 },{ 296,62},RNS_KEY_5 ,OSK_GROUP_None },
        { { 302,12 },{ 353,62},RNS_KEY_6 ,OSK_GROUP_None }, 
        { { 361,12 },{ 410,62},RNS_KEY_7 ,OSK_GROUP_None },
        { { 418,12 },{ 467,62},RNS_KEY_8 ,OSK_GROUP_None },
        { { 475,12 },{ 524,62},RNS_KEY_9 ,OSK_GROUP_None },
        { { 532,12 },{ 581,62},RNS_KEY_0 ,OSK_GROUP_None }
	  },
	  //row 2
	  { 

	   //KeyLayoutInfo
        { { 18,69 },{ 68,119},RNS_KEY_a ,OSK_GROUP_None }, 
        { { 76,69 },{ 125,119},RNS_KEY_b ,OSK_GROUP_None },
        { { 133,69 },{ 182,119},RNS_KEY_c ,OSK_GROUP_None },
        { { 190,69 },{ 239,119},RNS_KEY_d ,OSK_GROUP_None },
        { { 247,69 },{ 296,119},RNS_KEY_e ,OSK_GROUP_None },
        { { 304,69 },{ 353,119},RNS_KEY_f ,OSK_GROUP_None }, 
        { { 361,69 },{ 410,119},RNS_KEY_g ,OSK_GROUP_None },
        { { 418,69 },{ 467,119},RNS_KEY_h ,OSK_GROUP_None },
        { { 475,69 },{ 524,119},RNS_KEY_i ,OSK_GROUP_None },
        { { 532,69 },{ 581,119},RNS_KEY_j ,OSK_GROUP_None }
	  },
	  //row 3
	  {
	   //KeyLayoutInfo
        { { 18,126 },{ 68,176},RNS_KEY_k ,OSK_GROUP_None }, 
        { { 75,126 },{ 125,176},RNS_KEY_l ,OSK_GROUP_None },
        { { 132,126 },{ 182,176},RNS_KEY_m ,OSK_GROUP_None },
        { { 189,126 },{ 239,176},RNS_KEY_n ,OSK_GROUP_None },
        { { 246,126 },{ 296,176},RNS_KEY_o ,OSK_GROUP_None },
        { { 303,126 },{ 353,176},RNS_KEY_p ,OSK_GROUP_None }, 
        { { 360,126 },{ 410,176},RNS_KEY_q ,OSK_GROUP_None },
        { { 417,126 },{ 467,176},RNS_KEY_r ,OSK_GROUP_None },
        { { 474,126 },{ 524,176},RNS_KEY_s ,OSK_GROUP_None },
        { { 531,126 },{ 581,176},RNS_KEY_t ,OSK_GROUP_None }
	  },
	  //row 4
	  {
	   //KeyLayoutInfo
        { { 18,183 },{ 68,233},RNS_KEY_u ,OSK_GROUP_None }, 
        { { 75,183 },{ 125,233},RNS_KEY_v ,OSK_GROUP_None },
        { { 132,183 },{ 182,233},RNS_KEY_w ,OSK_GROUP_None },
        { { 189,183 },{ 239,233},RNS_KEY_x ,OSK_GROUP_None },
        { { 246,183 },{ 296,233},RNS_KEY_y ,OSK_GROUP_None },
        { { 303,183 },{ 353,233},RNS_KEY_z ,OSK_GROUP_None }, 
        { { 360,183 },{ 410,233},RNS_KEY_Exclam ,OSK_GROUP_None },
        { { 417,183 },{ 467,233},RNS_KEY_Comma ,OSK_GROUP_None },
        { { 474,183 },{ 524,233},RNS_KEY_Period ,OSK_GROUP_None },
        { { 531,183 },{ 581,233},RNS_KEY_At ,OSK_GROUP_None }
	   },
	  //row 5
	  { 
	   //KeyLayoutInfo
        { { 18,240 },{ 125,300},RNS_KEY_Shift_L ,OSK_GROUP_None }, 
        { { 132,240 },{ 182,290},RNS_KEY_UnKnown ,OSK_GROUP_Symbols },
        { { 189,240 },{ 239,290},RNS_KEY_UnKnown ,OSK_GROUP_Unknown },
        { { 246,240 },{ 353,290},RNS_KEY_Space ,OSK_GROUP_None },
        { { 360,240 },{ 467,290},RNS_KEY_Delete ,OSK_GROUP_None },
        { { 474,240 },{ 581,290},RNS_KEY_Select ,OSK_GROUP_None } 
	  }
};
  std::map<OSKTypes, KBContainer> OSKContainer =
  {
    {OSK_ALPHA_NUMERIC_KB,DefaultOSK},
  };
};
#endif// __RNSOSKMapping_h__
