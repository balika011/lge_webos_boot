/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/

// === INCLUDE =============================================================================
#include "x_os.h"	
#include "sv_const.h"
#include "hw_vdoin.h"
#include "vdo_misc.h"
#include "nptv_debug.h"
#include "video_def.h"
#include "drv_tdtv_drvif.h"
#include "panel.h"
#include "vdo_rm.h"


#include "drv_video.h"
#include "drv_tvd.h"
#include "drv_dvi.h"
#include "drv_mpeg.h"
#include "drv_ycproc.h"
#include "source_select.h"

#ifdef CC_SUPPORT_TVE
#include "tve_if.h"
#endif

// === HW INCLUDE =============================================================================
#include "hw_ycproc.h"
#include "hw_ospe.h"
#include "hw_sw.h"

// === DEFINE =============================================================================
#define MATRIX3x3_OFFSET	    0x100
#define CLIP(val, min, max) ((val>=max) ? max : ((val<=min) ? min : val)) 

// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================
extern UINT8 bGetSignalType(UINT8 bPath) ;
EXTERN UINT8 bForceAllPQOff;
// === GLOBAL VARIABLE ====================================================================
UINT16 u2PQItemMatrix[SV_VP_NA][PQ_MAX];
UINT16 u2ExtAdjMatrix[SV_VP_NA][PQ_MAX];
UINT16 u2Ext2AdjMatrix[SV_VP_NA][PQ_MAX];  //--For fade in/fade out function
UINT8 _bMAIN_COLOR_SYS;
UINT8 _bPIP_COLOR_SYS;
INT32 COLOR_TRANSFORM_ADJ[15];
UINT8 u1HdmiColorMode = SV_HDMI_MODE_AUTO;
UINT8 u1HdmiColorModeEx = SV_HDMI_MODE_AUTO;
UINT8 u1VgaColorMode = SV_VGA_MODE_GRAPHIC;
UINT16 pPedestalMatrix[15];
UINT16 pPedestalMatrixISR[15];
UINT16 InCTMatrix[15];

UINT16 const COLOR_TRANSFORM[SV_COLORTRANS_MAX][15] =
{
    {   // 0    RGB(Full) -> RGB (Full)
        0x0000, 0x0000, 0x0000,
        0x0200, 0x0000, 0x0000,
        0x0000, 0x0200, 0x0000,
        0x0000, 0x0000, 0x0200,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 1    RGB(Limit) -> RGB (Full)
        0x07C0, 0x07C0, 0x07C0,
        0x0254, 0x0000, 0x0000,
        0x0000, 0x0254, 0x0000,
        0x0000, 0x0000, 0x0254,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 2    RGB(Full) -> YCbCr601(Limit)
        0x0000, 0x0000, 0x0000,
        0x0102, 0x0032, 0x0084,
        0xFF6B, 0x00E1, 0xFFB4,
        0xFF44, 0xFFDC, 0x00E0,
        0x0040, 0x1800, 0x1800
    }
    ,
    {   // 3    RGB(Limit) -> YCbCr601(Limit)
        0x0000, 0x0000, 0x0000,
        0x012D, 0x003A, 0x0099,
        0xFF52, 0x0106, 0xFFA8,
        0xFF25, 0xFFD6, 0x0105,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 4    RGB(Full) -> YCbCr709(Limit)
        0x0000, 0x0000, 0x0000,
        0x013B, 0x001F, 0x005E,
        0xFF53, 0x00E1, 0xFFCC,
        0xFF34, 0xFFEC, 0x00E0,
        0x0040, 0x1800, 0x1800
    }
    ,
    {   // 5    RGB(Limit) -> YCbCr709(Limit)
        0x0000, 0x0000, 0x0000,
        0x016F, 0x0024, 0x006D,
        0xFF36, 0x0106, 0xFFC4,
        0xFF13, 0xFFE9, 0x0104,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 6    YCbCr601(Limit) -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x0200, 0x0000, 0x0000,
        0x0000, 0x0200, 0x0000,
        0x0000, 0x0000, 0x0200,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 7    YCbCr601(Full) -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x01B8, 0x0000, 0x0000,
        0x0000, 0x01C2, 0x0000,
        0x0000, 0x0000, 0x01C2,
        0x0040, 0x1800, 0x1800
    }
    ,
    {   // 8    YCbCr601(Limit) -> YCbCr709(Limit)
        0x0000, 0x0600, 0x0600,
        0x0200, 0xFFC5, 0xFF96,
        0x0000, 0x0209, 0x003A,
        0x0000, 0x0026, 0x020D,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 9    YCbCr601(Full) -> YCbCr709(Limit)
        0x0000, 0x0600, 0x0600,
        0x01B6, 0xFFCD, 0xFFA5,
        0x0000, 0x01C8, 0x0033,
        0x0000, 0x0022, 0x01CB,
        0x0040, 0x1800, 0x1800
    }
    ,
    {   // 10   YCbCr601(Limit) -> RGB(Full)
        0x07C0, 0x0600, 0x0600,
        0x0254, 0xFF38, 0xFE60,
        0x0254, 0x0409, 0x0000,
        0x0254, 0x0000, 0x0331,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 11   YCbCr601(Full) -> RGB(Full)
        0x0000, 0x0600, 0x0600,
        0x0200, 0xFF54, 0xFE9B,
        0x0200, 0x0377, 0x0000,
        0x0200, 0x0000, 0x02BE,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 12    YCbCr709(Limit) -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x0200, 0x0033, 0x0062,
        0x0000, 0x01FB, 0x0FC8,
        0x0000, 0x0FDB, 0x01F8,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 13    YCbCr709(Full) -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x01B8, 0x002C, 0x0054,
        0x0000, 0x01BE, 0xFFCF,
        0x0000, 0xFFDF, 0x01BB,
        0x0040, 0x1800, 0x1800
    }
    ,
    {   // 14   YCbCr709(Limit) -> RGB(Full)
        0x07C0, 0x0600, 0x0600,
        0x0254, 0xFF93, 0xFEEF,
        0x0254, 0x0396, 0x0000,
        0x0254, 0x0000, 0x043B,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 15   YCbCr709(Full) -> RGB(Full)
        0x0000, 0x0600, 0x0600,
        0x0200, 0xFFA2, 0xFF15,
        0x0200, 0x03A2, 0x0000,
        0x0200, 0x0000, 0x0314,
        0x0000, 0x0000, 0x0000
    }
    ,
    {   // 16   TVD NTSC YUV -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x0362, 0x0000, 0x0000,
        0x0000, 0x03F8, 0x0000,
        0x0000, 0x0000, 0x02D0,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 17   TVD NTSC_J YUV -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x0321, 0x0000, 0x0000,
        0x0000, 0x03AC, 0x0000,
        0x0000, 0x0000, 0x029A,
        0x0000, 0x1800, 0x1800
    }
    ,
    {   // 18   TVD PAL YUV -> YCbCr601(Limit)
        0x0000, 0x0600, 0x0600,
        0x0333, 0x0000, 0x0000,
        0x0000, 0x03C1, 0x0000,
        0x0000, 0x0000, 0x02A9,
        0x0000, 0x1800, 0x1800
    }
};

INT32 const YCBCR2RGBOS[3][15] =
{
    {
    	0, 0, 0,
    	1196, 0, 1640,      // 1.168, 0, 1.601
    	1196, -403, -835,   // 1.168, -0.394, -0.813
    	1196, 2072, 0,      // 1.168, 2.023, 0
    	-75, -75, -75
    }
    ,
    {
    	0, 0, 0,
    	1126, 0, 1549,      // 1.1, 0, 1.513
    	1126, -378, -785,   // 1.1, -0.37, -0.77
    	1126, 1959, 0,      // 1.1, 1.913, 0
    	-70, -70, -70
    }
    ,
    {
    	0, 0, 0,
    	1024, 0, 1404,     // 1, 0, 1.371
    	1024, -345, -715,  // 1, -0.336, -0.698
    	1024, 1774, 0,     // 1, 1.732, 0
    	0, 0, 0
    }
};

// === EXTERN VARIABLE ====================================================================

// === STATIC VARIABLE ====================================================================
//sin(0 deg ~ 90 deg) = 0 ~ (2 ^ 14 = 16384)
static INT16 const COLOR_TRANSFORM_HUE[129] =
{
    0,201,402,603,803,1004,1205,1405,               
    1605,1805,2005,2204,2404,2602,2801,2998,        
    3196,3393,3589,3785,3980,4175,4369,4563,        
    4756,4948,5139,5329,5519,5708,5896,6083,        
    6269,6455,6639,6822,7005,7186,7366,7545,        
    7723,7900,8075,8249,8423,8594,8765,8934,        
    9102,9268,9434,9597,9759,9920,10079,10237,      
    10393,10548,10701,10853,11002,11150,11297,11442,
    11585,11726,11866,12003,12139,12273,12406,12536,
    12665,12791,12916,13038,13159,13278,13395,13510,
    13622,13733,13842,13948,14053,14155,14255,14353,
    14449,14543,14634,14723,14810,14895,14978,15058,
    15136,15212,15286,15357,15426,15492,15557,15618,
    15678,15735,15790,15842,15892,15940,15985,16028,
    16069,16107,16142,16175,16206,16234,16260,16284,
    16305,16323,16339,16353,16364,16372,16379,16382,
    16384
};


// VGA Color Process
// Inorder to multiply another 16bit variable and plus 1 sign bit,
// All floating point variable need to shift 15 bit = 2^15 = 32768
static INT32 const RGB2YUV[15] =
{
	0, 512, 512,
	4810, 933, 2449,       // 0.5872, 0.1139, 0.2989
	-2777, 4191, -1414,    // -0.3390, 0.5116, -0.1726
	-3508, -681, 4189,     // -0.4283, -0.0831, 0.5114
	-75, -75, -75
};

static INT32 const SRC_VGA_RGB_YUV2RGB[15] =
{
    -8, 504, 504,
    8323, 0, 11410,       // 1.0159, 0, 1.3928
    8323, -2796, -5809,   // 1, -0.3414, -0.7091
    8323, 14415, 0,       // 1.0159, 1.7596, 0
    0, 0, 0
};

static INT32 const SRC_RGB_YUV2RGB[15] =
{
	0, 512, 512,
	8192, 0, 11231,        // 1, 0, 1.371
	8192, -2753, -5718,    // 1, -0.336, -0.698
	8192, 14189, 0,        // 1, 1.732, 0
	0, 0, 0
};

static INT32 const SRC_YUV_YUV2RGB[15] =
{
    0, 0, 0,
    8192, 0, 11231,        // 1, 0, 1.371
    8192, -2753, -5718,    // 1, -0.336, -0.698
    8192, 14189, 0,        // 1, 1.732, 0
    0, 0, 0
};

static INT32 const SRC_YUV_YUV2RGBEXTLVL[15] =
{
    0, 0, 0,
    9568, 0, 13117,       // 1.168, 0, 1.601
    9568, -3215, -6678,   // 1.168, -0.394, -0.813
    9568, 16572, 0,       // 1.168, 2.023, 0
    -75, -75, -75
};

static INT32 const SRC_VGA_YUV_YUV2RGBEXTLVL[15] =
{
    0, 0, 0,
    9720, 0, 13326,
    9720, -3266, -6784,
    9720, 16836, 0,
    -77, -77, -77
};

static INT32 const SRC_YUV_YUV2RGB_HD[15] =
{
    0, 0, 0,
	8192,	0,	12615,     	// 1	0	1.54
	8192,	-1500,	-3761,	// 1	-0.183	-0.459
	8192,	14876,	0,		// 1	1.816	0
	0, 0, 0
};

static INT32 const SRC_YUV_YUV2RGBEXTLVL_HD[15] =
{
	0, 0, 0,
	9568,	0,	14734,
	9568,	-1752,	-4393,
	9568,	17375,	0,
	-75, -75, -75
};

static INT32 const SRC_VGA_YUV_YUV2RGBEXTLVL_HD[15] =
{
	0, 0, 0,
	9720,	0,   14969,
	9720,	-1780,	-4463,
	9720,	17652,	0,
	-77, -77, -77
};

static INT32 const UNITRANSFROM[15] =
{
	0x0000,    0x0000,    0x0000,
	0x2000,    0x0000,    0x0000,
	0x0000,    0x2000,    0x0000,
	0x0000,    0x0000,    0x2000,
	0x0000,    0x0000,    0x0000
};

// === BODY ===============================================================================

void vVdoSetColorTransform(UINT8 bPath)
{
    UINT8 u1DecType, u1MpegColorSpace;
    UINT32 u4ColorSystem = SV_COLOR_RGB_SD;
    UINT32 u4HDTV = wDrvVideoInputHeight(bPath) >= 720;

    u1DecType = (bPath == SV_VP_MAIN) ? _rMChannel.bDecType : _rPChannel.bDecType;

    // Check color space from decoder type
    switch (u1DecType)
    {
        case SV_VD_TVD3D:
            switch (bTvd3dGetColorSystem())
            {
                case SV_CS_SECAM:
                case SV_CS_PAL:
                case SV_CS_PAL_60:
                case SV_CS_PAL_N:
                    u4ColorSystem = SV_COLOR_YUV_PAL;
                    break;
                case SV_CS_PAL_M:
                case SV_CS_NTSC358:
                case SV_CS_NTSC443:
                    u4ColorSystem = _bAgcPedEn ? SV_COLOR_YUV_NTSC : SV_COLOR_YUV_NTSCJ;
                    break;
                default:
                    u4ColorSystem = SV_COLOR_YCBCR_SD;
                    break;
            }
            break;
        case SV_VD_YPBPR:
            u4ColorSystem = u4HDTV ? SV_COLOR_YCBCR_HD : SV_COLOR_YCBCR_SD;
            break;
        case SV_VD_VGA:
            if (bGetICInputType(bPath) == P_VGACOMP)
            {
                u4ColorSystem = u4HDTV ? SV_COLOR_YCBCR_HD : SV_COLOR_YCBCR_SD;
            }
            else
            {
                u4ColorSystem = u4HDTV ? SV_COLOR_RGB_HD : SV_COLOR_RGB_SD;
            }
            break;
#if SUPPORT_DVI
        case SV_VD_DVI:
            if (!fgIsExtSrcCCIR())
            {
                if (_bHDMIColorSpace) // RGB
                {
                    u4ColorSystem = u4HDTV ? SV_COLOR_RGB_HD : SV_COLOR_RGB_SD;
                }
                else // YCbCr
                {
                    u4ColorSystem = u4HDTV ? SV_COLOR_YCBCR_HD : SV_COLOR_YCBCR_SD;
                }
#ifdef CC_OSD_ENCODE// always RGB 
				if((bPath==SV_VP_PIP)&&(bGetICInputType(SV_VP_PIP)==P_OSDENCODE))
				{
					u4ColorSystem = u4HDTV ? SV_COLOR_RGB_HD : SV_COLOR_RGB_SD;
				}
#endif
            }
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
        case SV_VD_MPEGSD:            
            u1MpegColorSpace = (u1DecType == SV_VD_MPEGHD) ? u1MpegHdColourPrimary() : u1MpegSdColourPrimary();
            if ((u1MpegColorSpace == 4) ||   //ITU-R Recommendation BT.470-6 System M
                (u1MpegColorSpace == 5) ||   //ITU-R Recommendation BT.470-6 System B, G
                (u1MpegColorSpace == 6) ||   //Society of Motion Picture and Television Engineers 170M (1999)
                (u1MpegColorSpace == 8))     //Generic film (colour filters using Illuminant C)
            {
                u4ColorSystem = SV_COLOR_YCBCR_SD;
            }
            else if (u1MpegColorSpace == 1)  //ITU-R Recommendation BT.709
            {
                u4ColorSystem = SV_COLOR_YCBCR_SD;
            }
            else    //Unknown type, check input resolution directly
            {
                u4ColorSystem = u4HDTV ? SV_COLOR_YCBCR_HD : SV_COLOR_YCBCR_SD;
            }
            break;
#endif
        default:
            u4ColorSystem = SV_COLOR_RGB_SD;
            break;
    }

    if (bPath == SV_VP_MAIN)
    {
        _bMAIN_COLOR_SYS = u4ColorSystem;
    }
    else
    {
        _bPIP_COLOR_SYS = u4ColorSystem;
    }
    
    vDrvColorTransform(bPath);
}
 

/*****************************************************************************************/
/*********************************   3x3 Matrix   ****************************************/
/*****************************************************************************************/
UINT8 vDrvGetColorSystem(UINT8 bPath)
{
    return (bPath == SV_VP_MAIN) ? _bMAIN_COLOR_SYS : _bPIP_COLOR_SYS;
}

/**
 * @brief load 3x3 color transform matrix
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param pwTable every 3x3 matrix has 15 TRANS_INDEX entries, see COLOR_TRANSFORM[]
 *        00h~02h		input sign and offset
 *        03h~0Bh		3x3 matrix coefficients
 *        0Ch~0Eh		clamp sign, output sign, and output offset
 */
void vLoadColorMatrix(UINT8 bPath, const UINT16 * pwTable)
{
    if (pwTable == NULL)
    {
        return;
    }

    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldMulti(TRANS_MAIN_00,
                          P_Fld((pwTable[14] & 0x800) >> 11, OUT_S2P)
                          | P_Fld((pwTable[13] & 0x800) >> 11, OUT_S1P)
                          | P_Fld((pwTable[12] & 0x800) >> 11, OUT_S0P)
                          | P_Fld((pwTable[14] & 0x1000) >> 12, CLAMP_S2P)
                          | P_Fld((pwTable[13] & 0x1000) >> 12, CLAMP_S1P)
                          | P_Fld((pwTable[12] & 0x1000) >> 12, CLAMP_S0P)
                          | P_Fld((pwTable[2] & 0x800) >> 11, IN_S2P)
                          | P_Fld((pwTable[1] & 0x800) >> 11, IN_S1P)
                          | P_Fld((pwTable[0] & 0x800) >> 11, IN_S0P)
                          | P_Fld(pwTable[1] & 0x7FF, IN_OFFSET_1P)
                          | P_Fld(pwTable[0] & 0x7FF, IN_OFFSET_0P));
        vIO32WriteFldMulti(TRANS_MAIN_01,
                          P_Fld(pwTable[12] & 0x7FF, OUT_OFFSET_0P)
                          | P_Fld(pwTable[2] & 0x7FF, IN_OFFSET_2P));
        vIO32WriteFldMulti(TRANS_MAIN_02,
                          P_Fld(pwTable[14] & 0x7FF, OUT_OFFSET_2P)
                          | P_Fld(pwTable[13] & 0x7FF, OUT_OFFSET_1P));
        vIO32WriteFldMulti(TRANS_MAIN_03,
                          P_Fld(pwTable[4], COEFF_01P)
                          | P_Fld(pwTable[3], COEFF_00P));
        vIO32WriteFldMulti(TRANS_MAIN_04,
                          P_Fld(pwTable[6], COEFF_10P)
                          | P_Fld(pwTable[5], COEFF_02P));
        vIO32WriteFldMulti(TRANS_MAIN_05,
                          P_Fld(pwTable[8], COEFF_12P)
                          | P_Fld(pwTable[7], COEFF_11P));
        vIO32WriteFldMulti(TRANS_MAIN_06,
                          P_Fld(pwTable[10], COEFF_21P)
                          | P_Fld(pwTable[9], COEFF_20P));
        vIO32WriteFldAlign(TRANS_MAIN_07, pwTable[11], COEFF_22P);
    }
    else
    {
        vIO32WriteFldMulti(TRANS_PIP_00,
                          P_Fld((pwTable[14] & 0x800) >> 11, OUT_S2P)
                          | P_Fld((pwTable[13] & 0x800) >> 11, OUT_S1P)
                          | P_Fld((pwTable[12] & 0x800) >> 11, OUT_S0P)
                          | P_Fld((pwTable[14] & 0x1000) >> 12, CLAMP_S2P)
                          | P_Fld((pwTable[13] & 0x1000) >> 12, CLAMP_S1P)
                          | P_Fld((pwTable[12] & 0x1000) >> 12, CLAMP_S0P)
                          | P_Fld((pwTable[2] & 0x800) >> 11, IN_S2P)
                          | P_Fld((pwTable[1] & 0x800) >> 11, IN_S1P)
                          | P_Fld((pwTable[0] & 0x800) >> 11, IN_S0P)
                          | P_Fld((pwTable[1] & 0x7FF) >> 2, IN_OFFSET_1P_PIP)
                          | P_Fld((pwTable[0] & 0x7FF) >> 2, IN_OFFSET_0P_PIP));
        vIO32WriteFldMulti(TRANS_PIP_01,
                          P_Fld((pwTable[12] & 0x7FF) >> 2, OUT_OFFSET_0P_PIP)
                          | P_Fld((pwTable[2] & 0x7FF) >> 2, IN_OFFSET_2P_PIP));
        vIO32WriteFldMulti(TRANS_PIP_02,
                          P_Fld((pwTable[14] & 0x7FF) >> 2, OUT_OFFSET_2P_PIP)
                          | P_Fld((pwTable[13] & 0x7FF) >> 2, OUT_OFFSET_1P_PIP));
        vIO32WriteFldMulti(TRANS_PIP_03,
                          P_Fld(pwTable[4], COEFF_01P)
                          | P_Fld(pwTable[3], COEFF_00P));
        vIO32WriteFldMulti(TRANS_PIP_04,
                          P_Fld(pwTable[6], COEFF_10P)
                          | P_Fld(pwTable[5], COEFF_02P));
        vIO32WriteFldMulti(TRANS_PIP_05,
                          P_Fld(pwTable[8], COEFF_12P)
                          | P_Fld(pwTable[7], COEFF_11P));
        vIO32WriteFldMulti(TRANS_PIP_06,
                          P_Fld(pwTable[10], COEFF_21P)
                          | P_Fld(pwTable[9], COEFF_20P));
        vIO32WriteFldAlign(TRANS_PIP_07, pwTable[11], COEFF_22P);

    }
}

/*****************************************************************************************/
/*********************************  Output Stage 3x3 Matrix   **********************************/
/*****************************************************************************************/
/**
 * @brief load output stage 3x3 color transform matrix
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param pwTable every 3x3 matrix has 15 TRANS_INDEX entries, see vDrvVideoColorMatrix()
 */
void vLoadOSColorMatrix(UINT8 bPath, const INT32 * pwTable)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(YCV_OFFSET_00, pwTable[0], YIN_OFFSET_MAIN);
        vIO32WriteFldAlign(YCV_OFFSET_00, pwTable[1], CIN_OFFSET_MAIN);
        vIO32WriteFldAlign(YCV_OFFSET_01, pwTable[2], VIN_OFFSET_MAIN);

        vIO32WriteFldAlign(YCBCR2RGB_00, pwTable[3], Y2R_MATRIX00);
        vIO32WriteFldAlign(YCBCR2RGB_00, pwTable[4], Y2R_MATRIX01);
        vIO32WriteFldAlign(YCBCR2RGB_01, pwTable[5], Y2R_MATRIX02);
        vIO32WriteFldAlign(YCBCR2RGB_01, pwTable[6], Y2R_MATRIX10);
        vIO32WriteFldAlign(YCBCR2RGB_02, pwTable[7], Y2R_MATRIX11);
        vIO32WriteFldAlign(YCBCR2RGB_02, pwTable[8], Y2R_MATRIX12);
        vIO32WriteFldAlign(YCBCR2RGB_03, pwTable[9], Y2R_MATRIX20);
        vIO32WriteFldAlign(YCBCR2RGB_03, pwTable[10], Y2R_MATRIX21);
        vIO32WriteFldAlign(YCBCR2RGB_04, pwTable[11], Y2R_MATRIX22);

        vIO32WriteFldAlign(RGBOFFSET_00, pwTable[12], R_OFFSET_MAIN);
        vIO32WriteFldAlign(RGBOFFSET_01, pwTable[13], G_OFFSET_MAIN);
        vIO32WriteFldAlign(RGBOFFSET_02, pwTable[14], B_OFFSET_MAIN);
    }
    else
    {
        vIO32WriteFldAlign(YCV_OFFSET_01, pwTable[0], YIN_OFFSET_PIP);
        vIO32WriteFldAlign(YCV_OFFSET_02, pwTable[1], CIN_OFFSET_PIP);
        vIO32WriteFldAlign(YCV_OFFSET_02, pwTable[2], VIN_OFFSET_PIP);

        vIO32WriteFldAlign(YCBCR2RGB_04, pwTable[3], Y2R_MATRIX00_PIP);
        vIO32WriteFldAlign(YCBCR2RGB_05, pwTable[4], Y2R_MATRIX01_PIP);
        vIO32WriteFldAlign(YCBCR2RGB_05, pwTable[5], Y2R_MATRIX02_PIP);
        vIO32WriteFldAlign(YCBCR2RGB_06, pwTable[6], Y2R_MATRIX10_PIP);
        vIO32WriteFldAlign(YCBCR2RGB_06, pwTable[7], Y2R_MATRIX11_PIP);
        vIO32WriteFldAlign(YCBCR2RGB_07, pwTable[8], Y2R_MATRIX12_PIP);
        vIO32WriteFldAlign(YCBCR2RGB_07, pwTable[9], Y2R_MATRIX20_PIP);
        vIO32WriteFldAlign(YCBCR2RGB_08, pwTable[10], Y2R_MATRIX21_PIP);
        vIO32WriteFldAlign(YCBCR2RGB_08, pwTable[11], Y2R_MATRIX22_PIP);

        vIO32WriteFldAlign(RGBOFFSET_00, pwTable[12], R_OFFSET_PIP);
        vIO32WriteFldAlign(RGBOFFSET_01, pwTable[13], G_OFFSET_PIP);
        vIO32WriteFldAlign(RGBOFFSET_02, pwTable[14], B_OFFSET_PIP);
    }
}


void vDrvSetBlackLevel(INT8 bPath, UINT8 bTransIndex)
{
	UINT8 bBlackLvl = (bPath == SV_VP_MAIN) 
		? IO32ReadFldAlign(MATRIX_00, IN_CSC_BLACK_LVL_M)
		: IO32ReadFldAlign(MATRIX_00, IN_CSC_BLACK_LVL_P);

    x_memcpy(InCTMatrix, COLOR_TRANSFORM[bTransIndex], sizeof(InCTMatrix));

	if ((bBlackLvl+InCTMatrix[12]) >= 0x80)
    {
        InCTMatrix[12] = InCTMatrix[12]+bBlackLvl-0x80;
    }
    else
    {
        InCTMatrix[12] = 0x800+bBlackLvl-0x80-InCTMatrix[12];
    }
}

void vDrvSetMatrixLvlAdj(void)
{
    UINT8 i;
    INT32 i4Temp;

    for (i = 3; i < 12; i++)    // Set Y and C gain
    {
        INT32 u4Gain = (i <= 5) ? IO32ReadFldAlign(MATRIX_01, IN_Y_GAIN) : IO32ReadFldAlign(MATRIX_01, IN_C_GAIN);

        i4Temp = pPedestalMatrix[i] & 0xFFF;
        i4Temp = i4Temp & 0x800 ? (i4Temp - 0x1000) : i4Temp;
        i4Temp =(i4Temp * u4Gain + 0x100) >> 9;
        i4Temp = i4Temp > 0x7FF ? 0x7FF: i4Temp;
        i4Temp = i4Temp < -0x800 ? -0x800: i4Temp;
        i4Temp = i4Temp & 0xFFF;
        pPedestalMatrix[i] = (pPedestalMatrix[i]&0xF000) | i4Temp;
    }

    // Set Y output offset
    i4Temp = pPedestalMatrix[12]&0x7FF;
    i4Temp = i4Temp&0x400 ? (i4Temp-0x800) : i4Temp;
    i4Temp = i4Temp + IO32ReadFldAlign(MATRIX_01, IN_Y_OFST) - 0x200;
    i4Temp = i4Temp>0x3FF ? 0x3FF: i4Temp;
    i4Temp = i4Temp<-0x400 ? -0x400: i4Temp;
    i4Temp = i4Temp&0x7FF;
    pPedestalMatrix[12] = (pPedestalMatrix[12] & 0xF800) | i4Temp;
}

void vDrvSetPedestalMatrix(UINT8 bTransIndex)
{
    UINT8 i;
    INT32 i4Temp;

    x_memcpy(pPedestalMatrix, COLOR_TRANSFORM[bTransIndex], sizeof(pPedestalMatrix));

    if(!GET_MATRIX_PED)
    {
        return;
    }

    switch (bTransIndex)
    {
        case SV_COLORTRANS_YCBCR601L_2_YCBCR601L:
        case SV_COLORTRANS_YCBCR601F_2_YCBCR601L:
        case SV_COLORTRANS_YCBCR709L_2_YCBCR601L:
        case SV_COLORTRANS_YCBCR709F_2_YCBCR601L:
        case SV_COLORTRANS_YUVNTSC_2_YCBCR601L:
        case SV_COLORTRANS_YUVNTSCJ_2_YCBCR601L:
        case SV_COLORTRANS_YUVPAL_2_YCBCR601L:
            // Original Pre3x3: Yout = (Yin +In_offset[0])*Coef[3] +Out_offset[0] + (Uin+In_offset[1])*Coef[4] + (Vin +In_offset[2])*Coef[5]
            // 7.5 pedestal function: Yfinal = ((Yout -64) - (940-64)*7.5%)*100/(92.5) +64 = (Yout -129.7)*100/92.5 + 64
            // Yfinal =(Yin + In_offset[0] - 129.7/Coef[3]) * (Coef[3] *100/92.5) + Out_offset[0]*100/92.5 + 64 + (Uin+In_offset[1])*Coef[4]*100/92.5 + (Vin +In_offset[2])*Coef[5]*100/92.5
            // In_offset[0]' = In_offset[0] - 129.7/Coef[3]
            // Coef[3~5]' = Coef[3~5] *100/92.5
            // Out_offset[0]' = Out_offset[0]*100/92.5 + 64            
            for(i=3;i<6;i++)
            {
                i4Temp = pPedestalMatrix[i]&0xFFF;
                i4Temp = i4Temp&0x800 ? (i4Temp-0x1000) : i4Temp;
            
                i4Temp =((i4Temp * 0x229)+0x100) >> 9;
                i4Temp = i4Temp>0x7FF ? 0x7FF: i4Temp;
                i4Temp = i4Temp<-0x800 ? -0x800: i4Temp;
                i4Temp = i4Temp&0xFFF;
                 
                pPedestalMatrix[i] = (pPedestalMatrix[i]&0xF000) | i4Temp;
            } 
            
            i4Temp = pPedestalMatrix[12]&0x7FF;
            i4Temp = i4Temp&0x400 ? (i4Temp-0x800) : i4Temp;
            i4Temp = i4Temp - 76;   // (940-64)x7.5%+64=129.7, 129.7x100/92.5-64=75.7
            i4Temp = i4Temp>0x3FF ? 0x3FF: i4Temp;
            i4Temp = i4Temp<-0x400 ? -0x400: i4Temp;
            i4Temp = i4Temp&0x7FF;
            
            pPedestalMatrix[12] = (pPedestalMatrix[12]&0xF800) | i4Temp;
            break;
        case SV_COLORTRANS_RGBF_2_RGBF:
        case SV_COLORTRANS_RGBL_2_RGBF:
        case SV_COLORTRANS_YCBCR601L_2_RGBF:
        case SV_COLORTRANS_YCBCR601F_2_RGBF:
        case SV_COLORTRANS_YCBCR709L_2_RGBF:
        case SV_COLORTRANS_YCBCR709F_2_RGBF:
            // In_offset[0~2]' = In_offset[0~2]
            // Coef[3~11]' = Coef[3~5] *100/92.5
            // Out_offset[0~2]' = Out_offset[0~2]*100/92.5 -129.7*100/92.5 + 64
            for(i=3;i<12;i++)
            {
                i4Temp = pPedestalMatrix[i]&0xFFF;
                i4Temp = i4Temp&0x800 ? (i4Temp-0x1000) : i4Temp;

                i4Temp = ((i4Temp * 0x229)+ 0x100) >> 9;
                i4Temp = i4Temp>0x7FF ? 0x7FF: i4Temp;
                i4Temp = i4Temp<-0x800 ? -0x800: i4Temp;
                i4Temp = i4Temp&0xFFF;
                 
                pPedestalMatrix[i] = (pPedestalMatrix[i]&0xF000) | i4Temp;
            }
            pPedestalMatrix[12] = 0x7AD;
            pPedestalMatrix[13] = 0x7AD;
            pPedestalMatrix[14] = 0x7AD;
            break;

        case SV_COLORTRANS_RGBF_2_YCBCR601L:
        case SV_COLORTRANS_RGBL_2_YCBCR601L:
        case SV_COLORTRANS_RGBF_2_YCBCR709L:
        case SV_COLORTRANS_RGBL_2_YCBCR709L:
            // In_offset[0]' = In_offset[0]
            // Coef[3~5]' = Coef[3~5] *100/92.5
            // Out_offset[0]' = Out_offset[0]*100/92.5 -129.7*100/92.5 + 64
            for(i=3;i<6;i++)
            {
                i4Temp = pPedestalMatrix[i]&0xFFF;
                i4Temp = i4Temp&0x800 ? (i4Temp-0x1000) : i4Temp;

                i4Temp = ((i4Temp * 0x229) + 0x100) >> 9;
                i4Temp = i4Temp>0x7FF ? 0x7FF: i4Temp;
                i4Temp = i4Temp<-0x800 ? -0x800: i4Temp;
                i4Temp = i4Temp&0xFFF;
                 
                pPedestalMatrix[i] = (pPedestalMatrix[i]&0xF000) | i4Temp;
            }
            
            i4Temp = pPedestalMatrix[12]&0x7FF;
            i4Temp = i4Temp&0x400 ? (i4Temp-0x800) : i4Temp;
            i4Temp = (((i4Temp * 0x229) + 0x100) >> 9) - 76;
            i4Temp = i4Temp>0x3FF ? 0x3FF: i4Temp;
            i4Temp = i4Temp<-0x400 ? -0x400: i4Temp;
            i4Temp = i4Temp&0x7FF;
            pPedestalMatrix[12] = (pPedestalMatrix[12]&0xF800) | i4Temp;
            break;
        default:
            break;
    }
}


/**
 * @brief set color transform matrix
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
void vDrvColorTransform(UINT8 bPath)
{
    UINT32 u4ColorSys, u4VideoRange;
    UINT32 u4IsScalerRGB = bIsScalerInputRGB(bPath);
    UINT32 u4ColorTransIdx = SV_COLORTRANS_MAX;
#ifdef CC_SUPPORT_TVE
    UINT8 ucEnable;
#endif

    u4ColorSys = (bPath == SV_VP_MAIN) ? _bMAIN_COLOR_SYS : _bPIP_COLOR_SYS;
    DRVCUST_SetBlackLvlCtrl(bPath);

    // Decide input video signal range
    switch (bGetSignalType(bPath))
    {
        case SV_ST_DVI:
		case SV_ST_MPEG:
            u4VideoRange = bHDMIRgbRange();
            u4VideoRange = (u4VideoRange == SV_HDMI_RANGE_DEFAULT) ? 
                                    DRVCUST_HDMIRange(bPath) : u4VideoRange;
            break;
        case SV_ST_VGA:
            u4VideoRange = SV_HDMI_RANGE_FULL;
            break;
        default:
            u4VideoRange = SV_HDMI_RANGE_LIMIT;
            break;
    }

    // Decide color transform
    switch (u4ColorSys)
    {
        case SV_COLOR_RGB_SD:
            if (u4IsScalerRGB)
            {
                if (u4VideoRange == SV_HDMI_RANGE_LIMIT)    // limit range
                {
                    u4ColorTransIdx = SV_COLORTRANS_RGBL_2_RGBF;
                }
                else if (u4VideoRange == SV_HDMI_RANGE_FULL)    // full range
                {
                    u4ColorTransIdx = SV_COLORTRANS_RGBF_2_RGBF;
                }
            }
            else
            {
                if (u4VideoRange == SV_HDMI_RANGE_LIMIT) // limit range
                {
                    u4ColorTransIdx = SV_COLORTRANS_RGBL_2_YCBCR601L;
                }
                else if (u4VideoRange == SV_HDMI_RANGE_FULL)    // full range
                {
                    u4ColorTransIdx = SV_COLORTRANS_RGBF_2_YCBCR601L;
                }
            }
            break;

        case SV_COLOR_RGB_HD:
            if (u4IsScalerRGB)
            {
                if (u4VideoRange == SV_HDMI_RANGE_LIMIT)    // limit range
                {
                    u4ColorTransIdx = SV_COLORTRANS_RGBL_2_RGBF;
                }
                else if (u4VideoRange == SV_HDMI_RANGE_FULL)    // full range
                {
                    u4ColorTransIdx = SV_COLORTRANS_RGBF_2_RGBF;
                }
            }
            else
            {
                if (u4VideoRange == SV_HDMI_RANGE_LIMIT) // limit range
                {
                    u4ColorTransIdx = GET_MATRIX_HD_709 ? 
                        SV_COLORTRANS_RGBL_2_YCBCR709L : SV_COLORTRANS_RGBL_2_YCBCR601L;
                }
                else if (u4VideoRange == SV_HDMI_RANGE_FULL)    // full range
                {
                    u4ColorTransIdx = GET_MATRIX_HD_709 ? 
                        SV_COLORTRANS_RGBF_2_YCBCR709L : SV_COLORTRANS_RGBF_2_YCBCR601L;
                }
            }
            break;

        case SV_COLOR_YCBCR_SD:
            if (u4IsScalerRGB)
            {
                if (u4VideoRange == SV_HDMI_RANGE_FULL)   // full range
                {
                    u4ColorTransIdx = SV_COLORTRANS_YCBCR601F_2_RGBF;
                }
                else if (u4VideoRange == SV_HDMI_RANGE_LIMIT)    // limit range
                {
                    u4ColorTransIdx = SV_COLORTRANS_YCBCR601L_2_RGBF;
                }
            }
            else
            {
                if (u4VideoRange == SV_HDMI_RANGE_FULL)    // full range
                {
                    u4ColorTransIdx = SV_COLORTRANS_YCBCR601F_2_YCBCR601L;
                }
                else if (u4VideoRange == SV_HDMI_RANGE_LIMIT)     // limit range
                {
                    u4ColorTransIdx = SV_COLORTRANS_YCBCR601L_2_YCBCR601L;
                }
            }
            break;

        case SV_COLOR_YCBCR_HD:
            if (u4IsScalerRGB)
            {
                if (u4VideoRange == SV_HDMI_RANGE_FULL)    // full range
                {
                    u4ColorTransIdx = SV_COLORTRANS_YCBCR709F_2_RGBF;
                }
                else if (u4VideoRange == SV_HDMI_RANGE_LIMIT)     // limit range
                {
                    u4ColorTransIdx = SV_COLORTRANS_YCBCR709L_2_RGBF;
                }
            }
            else
            {
                if (u4VideoRange == SV_HDMI_RANGE_FULL)    // full range
                {
                    u4ColorTransIdx = (GET_MATRIX_HD_709) ? 
                        SV_COLORTRANS_YCBCR601F_2_YCBCR601L : SV_COLORTRANS_YCBCR709F_2_YCBCR601L;
                }
                else if (u4VideoRange == SV_HDMI_RANGE_LIMIT)     // limit range
                {
                    u4ColorTransIdx = (GET_MATRIX_HD_709) ? 
                        SV_COLORTRANS_YCBCR601L_2_YCBCR601L : SV_COLORTRANS_YCBCR709L_2_YCBCR601L;
                }
            }
            break;

        case SV_COLOR_YUV_NTSC:
            u4ColorTransIdx = SV_COLORTRANS_YUVNTSC_2_YCBCR601L;
            break;

        case SV_COLOR_YUV_NTSCJ:
            u4ColorTransIdx = SV_COLORTRANS_YUVNTSCJ_2_YCBCR601L;
            break;

        case SV_COLOR_YUV_PAL:
            u4ColorTransIdx = SV_COLORTRANS_YUVPAL_2_YCBCR601L;
            break;

        default:
            u4ColorTransIdx = SV_COLORTRANS_RGBF_2_RGBF;
            break;
    }

#ifdef CC_SUPPORT_TVE
    if (bPath == SV_VP_PIP)
    {
        TVE_GetEnable(TVE_1, &ucEnable);
        if (ucEnable && u4IsScalerRGB
			#if defined(CC_MT5882)
			&&(!(bDrvVrmIsAppFlagSet(VRM_APP_VIDEO_ENC) || bDrvVrmIsAppFlagSet(VRM_APP_GRAPHIC_DUMP)))
			#endif
			)
        {
            if ((bGetVideoDecType(SV_VP_PIP) == SV_VD_DVI) && (bHDMIInputType() == 0))
            {
                u4ColorTransIdx = SV_COLORTRANS_YCBCR601L_2_YCBCR601L;
            }
            else
            {
                u4ColorTransIdx = SV_COLORTRANS_RGBF_2_YCBCR601L;
            }
        }
    }
#endif

    if (u4ColorTransIdx >= SV_COLORTRANS_MAX)
    {
        LOG(0, "vDrvColorTransform matrix index error");
        return;
    }

    if(!GET_FORCE_LOAD_MATRIX)
    {
        vDrvSetPedestalMatrix(u4ColorTransIdx);
        vDrvSetMatrixLvlAdj();
        if (bPath == SV_VP_MAIN)
        {
            x_memcpy(pPedestalMatrixISR, pPedestalMatrix, sizeof(pPedestalMatrix));
            vApiRegisterVideoEvent(PE_EVENT_COLOR_TRANSFORM, bPath, SV_ON);        
        }
        else
        {
            vLoadColorMatrix(bPath, pPedestalMatrix);
        }
    }

    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(MATRIX_00, u4ColorTransIdx, MATRIXIDX_MAIN);
        vIO32WriteFldAlign(MATRIX_00, bIsScalerInput444(bPath), SCALER444_MAIN);
        vIO32WriteFldAlign(MATRIX_00, bIsScalerInputRGB(bPath), SCALERRGB_MAIN);
    }
    else
    {
        vIO32WriteFldAlign(MATRIX_00, u4ColorTransIdx, MATRIXIDX_PIP);
        vIO32WriteFldAlign(MATRIX_00, bIsScalerInput444(bPath), SCALER444_PIP);
        vIO32WriteFldAlign(MATRIX_00, bIsScalerInputRGB(bPath), SCALERRGB_PIP);
    }
}

void vDrvMatrixOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(TRANS_MAIN_00, ~bOnOff, MAIN_EASY_BP3X3);
    }
    else
    {
        vIO32WriteFldAlign(TRANS_PIP_00, ~bOnOff, MAIN_EASY_BP3X3);
    }
}

void vDrvOSMatrixOnOff(UINT8 bPath, UINT8 bOnOff)
{
	if (bPath == SV_VP_MAIN)
	{
		vIO32WriteFldAlign(YCBCR2RGB_00, ~bOnOff, YUV2RGB_BYPASS_MAIN);
	}
	else
	{
		vIO32WriteFldAlign(YCBCR2RGB_00, ~bOnOff, YUV2RGB_BYPASS_PIP);
	}
}

/**
* @brief enable/disable main/pip path 422 mode
* @param bPath SV_VP_MAIN/SV_VP_PIP
* @param bOnOff SV_ON/SV_OFF
*/
void vDrvSet422(UINT8 bPath, UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(YCBCR2YC_MAIN_00, bOnOff, C_MAIN_ENA);
        vIO32WriteFldAlign(YCBCR2YC_MAIN_00, bOnOff, C_MAIN_FILTER_ON);
        vIO32WriteFldAlign(YCBCR2YC_MAIN_00, bOnOff, C_MAIN_EVEN_EXTEND);
        vIO32WriteFldAlign(YCBCR2YC_MAIN_00, (~bOnOff), C_MAIN_NOSYNC_BLOCK);
        
        // OS 3x3matrix use standard Y2R for 422, and will be be filled by vDrvCalVideoColorMatrix() for RGB444
        if (bOnOff == SV_ON)
        {
            if ((DRVCUST_OptGet(eFlagUseMatrixPQAdj))&&
                (!(IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE) && IO32ReadFldAlign(DEMO_02, DEMO_COLOR_EN))))
            {
                vDrvVideoColorMatrix(SV_VP_MAIN);
            }
            else if(!GET_FORCE_LOAD_MATRIX)
            {
                vDrvCalVideoColorMatrix(SV_VP_MAIN, 0x80, 0x80, 0x80, 0x80);
            }
        }
        else
        {
            vDrvVideoColorMatrix(SV_VP_MAIN);
        }
    }
    else
    {
        vIO32WriteFldAlign(YCBCR2YC_PIP_00, bOnOff, C_MAIN_ENA);
        vIO32WriteFldAlign(YCBCR2YC_PIP_00, bOnOff, C_MAIN_EVEN_EXTEND);
        vIO32WriteFldAlign(YCBCR2YC_PIP_00, bOnOff, C_MAIN_FILTER_ON);
        vIO32WriteFldAlign(YCBCR2YC_PIP_00, (~bOnOff), C_MAIN_NOSYNC_BLOCK);
        vDrvVideoColorMatrix(SV_VP_PIP);
    }
}

/**OS Matrix Color Process
 * @brief set split line for demo mode
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
void vDrvVideoColorMatrix(UINT8 bPath)
{
    UINT8 u1Offset = (bPath == SV_VP_MAIN) ? 0 : 0x4;
    
    if (bPath >= SV_VP_NA)  //fix klockwork warning
    {
        return;
    }

    u2PQItemMatrix[bPath][PQ_BRIGHTNESS] = IO32ReadFldAlign(GLOBAL_ADJ_00 + u1Offset, BRIGHT_DUMMY);
    u2PQItemMatrix[bPath][PQ_CONTRAST] = IO32ReadFldAlign(GLOBAL_ADJ_02 + u1Offset, CONT_DUMMY);
    u2PQItemMatrix[bPath][PQ_SATURATION] = IO32ReadFldAlign(GLOBAL_ADJ_02 + u1Offset, SAT_DUMMY);
    u2PQItemMatrix[bPath][PQ_HUE] = IO32ReadFldAlign(GLOBAL_ADJ_00 + u1Offset, HUE_DUMMY);
    
    u2ExtAdjMatrix[bPath][PQ_BRIGHTNESS] = IO32ReadFldAlign(GLOBAL_ADJ_04 + u1Offset, EXT_ADJ_BRI);
    u2ExtAdjMatrix[bPath][PQ_CONTRAST] = IO32ReadFldAlign(GLOBAL_ADJ_04 + u1Offset, EXT_ADJ_CONT);
    u2ExtAdjMatrix[bPath][PQ_SATURATION] = IO32ReadFldAlign(GLOBAL_ADJ_04 + u1Offset, EXT_ADJ_SAT);
    u2ExtAdjMatrix[bPath][PQ_HUE] = IO32ReadFldAlign(GLOBAL_ADJ_04 + u1Offset, EXT_ADJ_HUE);
    
    //--For fade in/fade out function
    u2Ext2AdjMatrix[bPath][PQ_BRIGHTNESS] = IO32ReadFldAlign(GLOBAL_ADJ_06 + u1Offset, EXT2_ADJ_BRI);
    u2Ext2AdjMatrix[bPath][PQ_CONTRAST] = IO32ReadFldAlign(GLOBAL_ADJ_06 + u1Offset, EXT2_ADJ_CONT);
    u2Ext2AdjMatrix[bPath][PQ_SATURATION] = IO32ReadFldAlign(GLOBAL_ADJ_08 + u1Offset, EXT2_ADJ_SAT);

    vDrvCalVideoColorMatrix(bPath,
        u2PQItemMatrix[bPath][PQ_BRIGHTNESS],
        u2PQItemMatrix[bPath][PQ_CONTRAST],
        u2PQItemMatrix[bPath][PQ_SATURATION],
        u2PQItemMatrix[bPath][PQ_HUE]);
}

void vDrvCalVideoColorMatrix(UINT8 bPath, UINT16 wBriValue, UINT16 wContValue,
                          UINT16 wSatValue, UINT16 wHueValue)
{
    INT16 wCos_v, wSin_v;
    INT32  dwTable[15], dwTableY2R[15];
    UINT16 u2Rgain, u2Ggain, u2Bgain;
    UINT32 u4ExtLvl, u4Index;
    UINT32 u4Matrix709 = DRVCUST_GetOSMatrix709(bPath);
    UINT8 bModuleOnOff = (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_MATRIX);     
    UINT32 u4ExtCont, u4ExtBri, u4ExtSat, u4ExtHue;
    UINT32 u4Ext2Cont, u4Ext2Bri, u4Ext2Sat;  //--For fade in/fade out function
    UINT8 u1Offset = (bPath == SV_VP_MAIN) ? 0 : 0x4;

    if(GET_FORCE_LOAD_MATRIX)
        return;

    if(bModuleOnOff == SV_OFF)
    {
    	wBriValue = 0x80;
    	wContValue = 0x80;
    	wSatValue = 0x80;
    	wHueValue = 0x80;
    }

    u4ExtLvl = !((bPath == SV_VP_MAIN) ? 
        (IO32ReadFldAlign(PEUI_00, PEUI_XVYCC_UI_M) && bDrvGetxvYCCOnOff()) : 
        (IO32ReadFldAlign(PEUI_00, PEUI_XVYCC_UI_P) && bDrvGetxvYCCOnOff()));
   
    u2Rgain = IO32ReadFldAlign(MATRIX_02, COLOR_FILTER_R_GAIN);
    u2Ggain = IO32ReadFldAlign(MATRIX_02, COLOR_FILTER_G_GAIN);
    u2Bgain = IO32ReadFldAlign(MATRIX_02, COLOR_FILTER_B_GAIN);
    
    u4ExtCont = IO32ReadFldAlign(GLOBAL_ADJ_04+u1Offset, EXT_ADJ_CONT);
    u4ExtBri = IO32ReadFldAlign(GLOBAL_ADJ_04+u1Offset, EXT_ADJ_BRI); 
    u4ExtSat = IO32ReadFldAlign(GLOBAL_ADJ_04+u1Offset, EXT_ADJ_SAT);
    u4ExtHue = IO32ReadFldAlign(GLOBAL_ADJ_04+u1Offset, EXT_ADJ_HUE);

    //--For fade in/fade out function
    u4Ext2Cont = IO32ReadFldAlign(GLOBAL_ADJ_06+u1Offset, EXT2_ADJ_CONT);
    u4Ext2Bri = IO32ReadFldAlign(GLOBAL_ADJ_06+u1Offset, EXT2_ADJ_BRI); 
    u4Ext2Sat = IO32ReadFldAlign(GLOBAL_ADJ_08+u1Offset, EXT2_ADJ_SAT);

    LOG(5, "Global Adjust(%d) B %d  C %d  S %d  H %d\n", bPath, wBriValue, wContValue, wSatValue, wHueValue);
    LOG(5, "Extern Adjust(%d) B %d  C %d  S %d  H %d\n", bPath, u4ExtBri, u4ExtCont, u4ExtSat, u4ExtHue);

    x_memcpy(dwTable, (bIsScalerInputRGB(bPath)) ? RGB2YUV : UNITRANSFROM, sizeof(dwTable));
    x_memcpy(COLOR_TRANSFORM_ADJ, dwTable, sizeof(COLOR_TRANSFORM_ADJ));

    wHueValue += u4ExtHue - 0x80;
    if (wHueValue > 0x80)
    {
        wHueValue = (wHueValue > 0x100) ? 0x80 : (wHueValue - 0x80);
        wCos_v = COLOR_TRANSFORM_HUE[0x80 - wHueValue];                                                                                       
        wSin_v = COLOR_TRANSFORM_HUE[wHueValue];                                                                                        
    }
    else
    {
        wHueValue = (0x80 - wHueValue);
        wCos_v = COLOR_TRANSFORM_HUE[0x80 - wHueValue];                                                                                               
        wSin_v = -COLOR_TRANSFORM_HUE[wHueValue];
    }    

    // Contrast Adjustment
    //wContValue = (wContValue << 3) + u4ExtCont - 0x80;
    wContValue = (wContValue << 3) + u4ExtCont - 0x80 - u4Ext2Cont;
    for (u4Index = 3; u4Index < 12; u4Index++)
    {
        COLOR_TRANSFORM_ADJ[u4Index] = dwTable[u4Index] * wContValue >> 10;
    }

    // Hue Adjustment
    COLOR_TRANSFORM_ADJ[6] = (dwTable[6] * wCos_v + dwTable[9] * (-wSin_v) + (1<<13)) >> 14;
    COLOR_TRANSFORM_ADJ[7] = (dwTable[7] * wCos_v + dwTable[10] * (-wSin_v) + (1<<13)) >> 14;
    COLOR_TRANSFORM_ADJ[8] = (dwTable[8] * wCos_v + dwTable[11] * (-wSin_v) + (1<<13)) >> 14;
    COLOR_TRANSFORM_ADJ[9] = (dwTable[6] * wSin_v + dwTable[9] * wCos_v + (1<<13)) >> 14;
    COLOR_TRANSFORM_ADJ[10] = (dwTable[7] * wSin_v + dwTable[10] * wCos_v + (1<<13)) >> 14;
    COLOR_TRANSFORM_ADJ[11] = (dwTable[8] * wSin_v + dwTable[11] * wCos_v + (1<<13)) >> 14;

    // Saturation Adjustment
    //wSatValue = (wSatValue << 3) + (INT32)u4ExtSat - 0x80;
    wSatValue = (wSatValue << 3) + (INT32)u4ExtSat - 0x80 - u4Ext2Sat;
    for (u4Index = 6; u4Index < 12; u4Index++)
    {
        COLOR_TRANSFORM_ADJ[u4Index] = (COLOR_TRANSFORM_ADJ[u4Index] * wSatValue) >> 10;
    }

    x_memcpy(dwTable, COLOR_TRANSFORM_ADJ, sizeof(dwTable));

    if (bGetSignalType(bPath) == SV_ST_VGA)
    {
        x_memcpy(dwTableY2R, (bIsScalerInputRGB(bPath))? SRC_VGA_RGB_YUV2RGB : 
                                (u4Matrix709 ? SRC_VGA_YUV_YUV2RGBEXTLVL_HD : SRC_VGA_YUV_YUV2RGBEXTLVL),
                                sizeof(dwTableY2R));
    }
    else
    {
        x_memcpy(dwTableY2R, (bIsScalerInputRGB(bPath))? SRC_RGB_YUV2RGB : 
                                (u4ExtLvl ? 
                                    (u4Matrix709 ? SRC_YUV_YUV2RGBEXTLVL_HD : SRC_YUV_YUV2RGBEXTLVL) : 
                                    (u4Matrix709 ? SRC_YUV_YUV2RGB_HD : SRC_YUV_YUV2RGB)), 
                                sizeof(dwTableY2R));
    }

    // YUV -> RGB
    COLOR_TRANSFORM_ADJ[3] =
        (((dwTableY2R[3]) * (dwTable[3])) +
        ((dwTableY2R[4]) * (dwTable[6])) +
        ((dwTableY2R[5]) * (dwTable[9])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[4] =
        (((dwTableY2R[3]) * (dwTable[4])) +
        ((dwTableY2R[4]) * (dwTable[7])) +
        ((dwTableY2R[5]) * (dwTable[10])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[5] =
        (((dwTableY2R[3]) * (dwTable[5])) +
        ((dwTableY2R[4]) * (dwTable[8])) +
        ((dwTableY2R[5]) * (dwTable[11])) + (1<<15)) >> 16;

    COLOR_TRANSFORM_ADJ[6] =
        (((dwTableY2R[6]) * (dwTable[3])) +
        ((dwTableY2R[7]) * (dwTable[6])) +
        ((dwTableY2R[8]) * (dwTable[9])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[7] =
        (((dwTableY2R[6]) * (dwTable[4])) +
        ((dwTableY2R[7]) * (dwTable[7])) +
        ((dwTableY2R[8]) * (dwTable[10])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[8] =
        (((dwTableY2R[6]) * (dwTable[5])) +
        ((dwTableY2R[7]) * (dwTable[8])) +
        ((dwTableY2R[8]) * (dwTable[11])) + (1<<15)) >> 16;

    COLOR_TRANSFORM_ADJ[9] =
        (((dwTableY2R[9]) * (dwTable[3])) +
        ((dwTableY2R[10]) * (dwTable[6])) +
        ((dwTableY2R[11]) * (dwTable[9])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[10] =
        (((dwTableY2R[9]) * (dwTable[4])) +
        ((dwTableY2R[10]) * (dwTable[7])) +
        ((dwTableY2R[11]) * (dwTable[10])) + (1<<15)) >> 16;
    COLOR_TRANSFORM_ADJ[11] =
        (((dwTableY2R[9]) * (dwTable[5])) +
        ((dwTableY2R[10]) * (dwTable[8])) +
        ((dwTableY2R[11]) * (dwTable[11])) + (1<<15)) >> 16;

    COLOR_TRANSFORM_ADJ[0] = dwTableY2R[0];
    COLOR_TRANSFORM_ADJ[1] = dwTableY2R[1];
    COLOR_TRANSFORM_ADJ[2] = dwTableY2R[2];

    // Color Filter for RGB channel
    COLOR_TRANSFORM_ADJ[3] = (COLOR_TRANSFORM_ADJ[3] * u2Rgain) >> 9;
    COLOR_TRANSFORM_ADJ[4] = (COLOR_TRANSFORM_ADJ[4] * u2Rgain) >> 9;
    COLOR_TRANSFORM_ADJ[5] = (COLOR_TRANSFORM_ADJ[5] * u2Rgain) >> 9;

    COLOR_TRANSFORM_ADJ[6] = (COLOR_TRANSFORM_ADJ[6] * u2Ggain) >> 9;
    COLOR_TRANSFORM_ADJ[7] = (COLOR_TRANSFORM_ADJ[7] * u2Ggain) >> 9;
    COLOR_TRANSFORM_ADJ[8] = (COLOR_TRANSFORM_ADJ[8] * u2Ggain) >> 9;

    COLOR_TRANSFORM_ADJ[9] = (COLOR_TRANSFORM_ADJ[9] * u2Bgain) >> 9;
    COLOR_TRANSFORM_ADJ[10] = (COLOR_TRANSFORM_ADJ[10] * u2Bgain) >> 9;
    COLOR_TRANSFORM_ADJ[11] = (COLOR_TRANSFORM_ADJ[11] * u2Bgain) >> 9;

    // Brightness Adjustment
    //COLOR_TRANSFORM_ADJ[12] = ((dwTableY2R[12] * (INT32)wContValue) >> 10) + 
    //                            ((wBriValue - 0x80) << DRVCUST_OptGet(eBrightInterval)) + (INT32)u4ExtBri - 0x80;
    COLOR_TRANSFORM_ADJ[12] = ((dwTableY2R[12] * (INT32)wContValue) >> 10) + 
                                ((wBriValue - 0x80) << DRVCUST_OptGet(eBrightInterval)) + 
                                 (INT32)u4ExtBri - 0x80 + (u4Ext2Bri-0x200);
    COLOR_TRANSFORM_ADJ[13] = COLOR_TRANSFORM_ADJ[12];
    COLOR_TRANSFORM_ADJ[14] = COLOR_TRANSFORM_ADJ[12];

    // Overflow protection: gain
    for (u4Index = 3; u4Index < 12; u4Index++)
    {
        COLOR_TRANSFORM_ADJ[u4Index] = MIN(COLOR_TRANSFORM_ADJ[u4Index], 0x1FFF);
    }

    // Overflow protection: offset
    for (u4Index = 12; u4Index < 15; u4Index++)
    {
        COLOR_TRANSFORM_ADJ[u4Index] = CLIP(COLOR_TRANSFORM_ADJ[u4Index], -1024, 1023);
    }

	DRVCUST_SetGamutOnOSMatrix();

    vLoadOSColorMatrix(bPath, COLOR_TRANSFORM_ADJ);
}

void vDrvSetUniColorDomain(UINT8 bColorDomain, UINT8 bLvdsMode)
{
#ifdef DRV_SUPPORT_EXTMJC
    if (!IS_COMPANION_CHIP_ON())
        return;

    if ((bLvdsMode == eLvdsVidNormal) || (bLvdsMode == eLvdsVidRgb444OnePort) || (bLvdsMode == eLvdsVidRgb444TwoPort))
    {
        vIO32WriteFldAlign(OSTG_01, 1, REG_RGB2YUV_BYPASS);
    }
    else
    {
        vIO32WriteFldAlign(OSTG_01, 0, REG_RGB2YUV_BYPASS);
    }
#endif
}

void vDrvSetDefaultMatrix(UINT8 bPath, BOOL OnOff)
{
    UINT8 bModuleOnOff = OnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_MATRIX);     
    
	if (!bModuleOnOff)
	{
		vDrvCalVideoColorMatrix(bPath,0x80, 0x80, 0x80, 0x80);
	}
	else
	{
        vDrvVideoColorMatrix(bPath);
	}
	return;
}

/*=====================================
* @brief set Fade in/ Fade out interface
* @param bPath  - SV_VP_MAIN/SV_VP_PIP
*                 bValue - 0~255  [ Normal  --> Black/White ]
*                 bBg_Color - 0/1  [ Black/White ]
=====================================*/
void vDrvSetFade(UINT8 bPath, UINT8 bValue, UINT8 bBg_Color)
{
    UINT8 u1Offset = (bPath == SV_VP_MAIN) ? 0 : 0x4;
    INT32 Ori_Bri,Ori_Contrast,Ori_Sat;
    INT32 Bri,Contrast,Sat;
    //EXTERN UINT16 u2PQItemMatrix[SV_VP_NA][PQ_MAX];
    //EXTERN UINT16 u2ExtAdjMatrix[SV_VP_NA][PQ_MAX];

    if((bPath > 1) || (bValue > 255) || (bBg_Color > 1))
    {
        LOG(0, "\nvDRVCUST_SetFade, Parameter incorrect ! -bPath[%d],bValue[%d],bBg_Color[%d]-\n",
            bPath,bValue,bBg_Color);
        return;
    }

    Ori_Contrast = (u2PQItemMatrix[bPath][PQ_CONTRAST] << 3)
                    + u2ExtAdjMatrix[bPath][PQ_CONTRAST] - 0x80;
    Ori_Bri = ((u2PQItemMatrix[bPath][PQ_BRIGHTNESS] - 0x80) << DRVCUST_OptGet(eBrightInterval)) 
                  + (INT32)u2ExtAdjMatrix[bPath][PQ_BRIGHTNESS] - 0x80;
    Ori_Sat = (u2PQItemMatrix[bPath][PQ_SATURATION] << 3) + (INT32)u2ExtAdjMatrix[bPath][PQ_SATURATION] 
               - 0x80;

    if(bBg_Color == 0)    // bBg_Color[ black ]   (0->255 :  normal -> black)
    {
        Bri = (Ori_Bri >= 0) ? ((bValue == 255) ? Ori_Bri :((bValue*Ori_Bri + (255-bValue))>>8)) : 0;
        Contrast = (bValue == 255) ? Ori_Contrast : ((bValue*Ori_Contrast + (255-bValue))>>8);
        Sat = (bValue == 255) ? Ori_Sat : ((bValue*Ori_Sat + (255-bValue))>>8);

        vIO32WriteFldMulti(GLOBAL_ADJ_06+u1Offset, P_Fld(Contrast, EXT2_ADJ_CONT) 
                                    | P_Fld((0x200-MIN(0x200,Bri)), EXT2_ADJ_BRI)); 
        vIO32WriteFldAlign(GLOBAL_ADJ_08+u1Offset,Sat,EXT2_ADJ_SAT);
    }
    else if (bBg_Color == 1)    // bBg_Color[ white ]    (0->255 :  normal -> white)
    {
        Bri = (bValue == 255) ? (1023-(Ori_Bri+(-75*Ori_Contrast>>10))) :
                               ((bValue*(1023-(Ori_Bri+(-75*Ori_Contrast>>10))) + (255-bValue))>>8);
        Sat = (bValue == 255) ? Ori_Sat : ((bValue*Ori_Sat + (255-bValue))>>8);

        vIO32WriteFldMulti(GLOBAL_ADJ_06+u1Offset, P_Fld(0x0, EXT2_ADJ_CONT) 
                                   | P_Fld((Bri+0x200), EXT2_ADJ_BRI));
        vIO32WriteFldAlign(GLOBAL_ADJ_08+u1Offset,Sat,EXT2_ADJ_SAT);
    }
}
