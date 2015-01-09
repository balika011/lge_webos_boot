#ifndef __U_CUST_DEF_H__
#define __U_CUST_DEF_H__

#include "u_common.h"

#include "u_device_type.h"

/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
typedef enum
{
    VID_QA_TYPE_UNKNOWN = -1,
    VID_QA_TYPE_BRIGHTNESS,
    VID_QA_TYPE_CONTRAST,
    VID_QA_TYPE_HUE,
    VID_QA_TYPE_SATURATION,
    VID_QA_TYPE_CTI,
    VID_QA_TYPE_SHARPNESS,
    VID_QA_TYPE_LTI,
    VID_QA_TYPE_COLOR_GAIN,
    VID_QA_TYPE_COLOR_OFFSET,
    VID_QA_TYPE_COLOR_SUPPRESS,
    VID_QA_TYPE_NR,
    VID_QA_TYPE_GAMMA,
    VID_QA_TYPE_WHITE_PEAK_LMT,
    VID_QA_TYPE_FLESH_TONE,
  
    VID_QA_TYPE_LUMA,
    VID_QA_TYPE_3D_NR,
    VID_QA_TYPE_DEMO,
    VID_QA_TYPE_DI_FILM_MODE,
    VID_QA_TYPE_DI_MA,
    VID_QA_TYPE_DI_EDGE_PRESERVE,
    VID_QA_TYPE_MJC_MODE,
    VID_QA_TYPE_MJC_EFFECT,
    VID_QA_TYPE_MJC_DEMO,
    VID_QA_TYPE_XVYCC,
    VID_QA_TYPE_WCG,
    VID_QA_TYPE_GAME_MODE,
    VID_QA_TYPE_BLUE_STRETCH, 
    VID_QA_TYPE_MPEG_NR,           
    VID_QA_TYPE_BLACK_STRETCH,       
    VID_QA_TYPE_WHITE_STRETCH,

    VID_QA_TYPE_ETI,
    VID_QA_TYPE_BLACK_LVL_EXT,

    /* do not remove this */
    VID_QA_TYPE_MAX
} VID_QA_TYPE_T;

typedef struct _VID_QA_MIN_MAX_DFT_T
{
    VID_QA_TYPE_T e_vid_qa_type;
    INT32         i4_min;
    INT32         i4_max;
    INT32         i4_default;
} VID_QA_MIN_MAX_DFT_T;


#define VID_QA_MIN_MAX_DFT_END   { VID_QA_TYPE_UNKNOWN, 0, 0, 0 }

#endif /* _U_CUST_DEF_H__ */

