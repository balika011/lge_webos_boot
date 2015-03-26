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
/*-----------------------------------------------------------------------------
 *
 * $Author: p4admin $
 * $Date: 2015/03/26 $
 * $RCSfile: drv_di_int.c,v $
 * $Revision: #14 $
 *
 *---------------------------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////
#include "general.h"
#include "typedef.h"
#include "sv_const.h"
#include "vdo_misc.h"
#include "x_bim.h"
#include "di_debug.h"
#include "x_assert.h"
#include "drv_nr.h"
#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_tdtv_drvif.h"
#include "hw_di_int.h"
#include "hw_di.h"
#include "drv_tvd.h"
#ifdef CC_MT5399
#include "hw_mjcfd.h"
#endif
#include "hw_ycproc.h"
#include "video_timing.h"
#include "source_select.h"
#include "drv_meter.h"
#include "x_hal_5381.h"
#include "video_def.h"
#include "panel.h"
#include "fbm_drvif.h"
#include "b2r_drvif.h"

#define RA_ADAPTIVE_MIN_WIDTH     208

//Disable CS when signal level lower than this threshold
extern UINT8 bTvdSignalStrengthLevel(UINT8 bMode);
#define CS_DISABLE_WEAK_SIGNAL            50       
#define SUPPORT_MULTI_CADENCE             1

#define MDDI_PD_SW_FW_FILM_PATCH            1       //Patch for Some Scenario HW Can not detect
#define MDDI_CS_FW_FILM_DISABLE_CS          1

#define DI_DEBUG_EVENT_NUM      6
#define FW_FILM_FI_SEQ_32       0x210   //b1000010000
#define FW_FILM_MASK            0x1F    //b11111
#define FW_FILM_CB              5

#define FW_FILM_FR_SEQ_32       0x1EF   //b0111101111
#define FW_FILM_FR_SEQ_55       0x063   //b0001100011
#define FW_FILM_FR_SEQ_2224     0x0FF   //b0011111111
#define FW_FILM_FR_SEQ_2332     0x3F6   //b1111110110
#define FW_FILM_FR_SEQ_64       0x198   //b0110011000
#define FW_FILM_FR_SEQ_87       0x303   //b000001100000011         

#define FW_FILM_FR_SEQ_32322     0x7BF   //b0111 1011 1111
#define FW_FILM_FR_SEQ_44        0x033   //b0011 0011
#define FW_FILM_FR_SEQ_66        0x0C3   //b0000 1100 0011

#define MDDI_FILM_ACC_MAX_SEQ_CNT    0x60
#define MDDI_FILM_MAX_SEQ_CNT        0xFF

#define FILM_MASK(bit)      ((1<<bit)-1)
#define MASK_NUM             5
UINT32 u4MaskBit[MASK_NUM] = {15, 12, 10, 8, 4};

//Status Filter
#define DILOOPNUM               8   
#define DILOOPMASK              (DILOOPNUM-1) 

UINT8 fgDIStatusPolling;
UINT8 u1YFixMotion[VDP_NS] = {0, 0};
UINT8 u1CFixMotion[VDP_NS] = {0, 0};
UINT8 u1YMotionLevel[VDP_NS] = {0, 0};
UINT8 u1CMotionLevel[VDP_NS] = {0, 0};
UINT8 u1IF_MAX_MOTION_C_Value;
extern UINT8 _fgCornPattern;

UINT8 u1sawtooth_still;
UINT8 u1smooth_still;
static HANDLE_T _hDIAdaptivePDIsrSema;

static UINT8 _u1DIState =0;
// Chroma Jaggy & DTV Tearing Setting
#define SCPQ_TH_TOTAL 5
const UINT8 u1CJaggyTh[SCPQ_TH_TOTAL] = {4, 7, 10, 13, 16};
const UINT8 u1DTVTearTh[SCPQ_TH_TOTAL] = {4, 7, 10, 13, 16};

typedef struct
{
    UINT8 bMovingCzp;    
    UINT8 bFldInv;  
    UINT8 bCzpState;
    UINT8 bFroce3DDealy;
    UINT8 bMaxMoC;
} DI_IF_PARAM;

typedef struct
{   
    UINT8 bCsOnOff;
    UINT8 bFilmCnt;    
    UINT8 bFavorCS;        
} DI_CS_PARAM;

typedef struct
{      
    UINT32 u4DarCfg;
} DI_DAR_PARAM;

typedef struct
{  
    UINT8 bFWPatch32FrSeq;  //Sequence to change 32 PD Detection
    UINT8 bFWPatchCB32;     //Current Combination of 32 PD Detection
    UINT8 bFWPatchFi32Cnt;  //Sequence to change 32 PD Detection
    UINT8 bFWPatchFr32Cnt;  //Sequence to change 32 PD Detection
    UINT8 bFldCnt;
    UINT8 bFldCB;
    UINT8 bFwAccCnt;    
    UINT8 bFwBadEdit;
    UINT8 bFwWeave;
    UINT8 bUnknowCnt;
    UINT8 bFrmCnt;
    UINT8 bFrmCB;
    UINT8 bFilm24To50;
    UINT8 bSmallMo;    
    UINT8 bFilmPatch;
    UINT32 u4FrmSeq;
    UINT32 u4FrmThld;
    UINT32 u4FrmMask;
    UINT32 u4FrmBit;
    UINT32 u4FldSeq;    
    UINT32 u4FldThld;
    UINT32 u4FldMask;
    UINT32 u4FldBit;
    UINT32 u4AdaptiveFrMoTh;
    UINT32 u4AdaptiveEgMo32Th;    
    UINT32 u4AdaptiveEgMo22Th;  
    UINT32 u4AdaptivePxlMotionTh;  
    UINT32 u4AdaptiveEgMotionTh;          
    UINT16 u2FilmMode;    
} DI_PD_PARAM;

typedef struct
{
    E_DI_QUALITY_MODE eIfQty;
    E_DI_QUALITY_MODE ePeQty;
    E_DI_QUALITY_MODE eFilmQty;
    UINT8 bCfgFilmOnOff; //Film Mode ON/OFF of UI Setting
    UINT8 bFilmOnOff_Y; //Luma Film Blending Enable
    UINT8 bFilmOnOff_C; //Chroma Film Blending Enable    
    UINT8 bFilmOnOff_MultiCadence; //MultiCadence Enable
    UINT8 b32Film; //32 Film Mode Detection Engine Enable
    UINT8 b22Film; //22 Film Mode Detection Engine Enable
    UINT8 bCJaggy; //Chroma Jaggy Detection Enable
    UINT8 bDTVTear; //DTV Tearing Detection Enable
} DI_PQ_MODE;

typedef struct
{
    DI_IF_PARAM IfPar;
    DI_CS_PARAM CsPar;
    DI_DAR_PARAM DarPar;
    DI_PD_PARAM PdPar;
    UINT8 bForceSceneChange;
} DI_PQ_PARAM;

DI_PQ_MODE  DiPQMode;
DI_PQ_STA   DiSta;
DI_PQ_PARAM DiPar;
UINT8 u1DI_Is50HZ = SV_FALSE;

////////////////////////////////////////////////////////////////////////////////
// Register table
////////////////////////////////////////////////////////////////////////////////
const REGTBL_T CODE MDDI_QTY_INIT_MAIN[] =
{
    //iFution
    {MCVP_FUSION_01     , 0x04840A08, 0xFFFFFFFF},  //Increase 2D Flicker cost 
    {MCVP_FUSION_02     , 0x0000000F, 0x0000000F},  
    {MCVP_FUSION_04     , 0x88442021, 0xFFFFFFFF},  //Increase 3D Sawtooth cost
    {MCVP_FUSION_05     , 0x01010101, 0xFFFFFFFF},  //Decrease Blending ratio      
    {MCVP_FUSION_0C     , 0x00011010, 0xFFFFFFFF},  //HOMO DIFF NOISE CORING
    {MCVP_FUSION_0D     , 0x031C1004, 0xFFFFFFFF},  //Decrease the flicker cost clip setting  
    {MCVP_FUSION_22     , 0x00000F00, 0x00000F00},  //Disable Bounadary Saw
    {MCVP_FUSION_24     , 0xFFFF0000, 0xFFFFFFFF},  //Disable Film Active Window
    {MCVP_FUSION_26     , 0x00200164, 0x00FFFFFF},
    {MCVP_FUSION_28     , 0x0DC32262, 0xFFFFFFFF},
    {MCVP_FUSION_2A     , 0x15000010, 0xFF0000FF},
    {MCVP_FUSION_2C     , 0x00000004, 0x00000004},
    //CS
    {MCVP_CS_03         , 0x41822902, 0xFFFFFFFF},  //more exact valid MV condition
    {MCVP_CS_1F         , 0x00040000, 0x00078000},  // more pre_flt_coef (8)
    {MCVP_CS_25         , 0x00000000, 0x00000080},  //Disable border detection
    //DAR
    {MCVP_DARE_06       , 0x80001020, 0xFFFFFFFF},  //CUE depend on C motion level
    {MCVP_DARE_10       , 0x3410104B, 0xFFFFFFFF},  //Content Adaptive Filter      
    //Film
    {MCVP_FILM_00       , 0xA3020013, 0xFB3A70F3},
    {MCVP_FILM_01       , 0x80000400, 0xF8000C00},  //32 Film Mask 
    {MCVP_FILM_15       , 0x00140004, 0x03FF03FF},
    {MCVP_FILM_16       , 0x00000010, 0x000003FF},
    {MCVP_FILM_24       , 0x20300010, 0x73FF83FF},    
    {MCVP_FILM_25       , 0x70000000, 0xF01FF1FF},
    {MCVP_FILM_26       , 0x00000000, 0x000001FF},    
    {MCVP_FILM_27       , 0xE8050001, 0xFF1F0001},    
    {MCVP_FILM_28       , 0x00000000, 0x000000FF},
    {MCVP_FILM_3B       , 0xFFFFFFFF, 0xFFFFFFFF},  //FI_MO_32 MIN MAX
    {MCVP_FILM_3C       , 0xFFFFFFFF, 0xFFFFFFFF},    
    {MCVP_FILM_3D       , 0xFFFFFFFF, 0xFFFFFFFF},  //FI_MO_22 MIN MAX    
    {MCVP_FILM_3E       , 0x00000000, 0xFFFFFFFF},  //ED_MO always zero
    
    {REGTBL_END    , 0x00000000, 0x00000000}
};

const REGTBL_T CODE MDDI_QTY_INIT_SUB[] =
{   
    {REGTBL_END    , 0x00000000, 0x00000000}
};

const REGTBL_T CODE MDDI_PDMA_PARAM[] =
{
    {MA4F_R_FDIFF_TH2   , 0x00000000, 0x01000000},  //Fix Last Row Region Motion Problem  
    {REGTBL_END    , 0x00000000, 0x00000000}
};

const REGTBL_T CODE MDDI_FUSION_PARAM[3][3] =
{
    {   //RF
        {MCVP_FUSION_0D, 0x00001000, 0x00003F00},
        {MCVP_FUSION_23, 0x00000000, 0x000000FF},
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //SD
        {MCVP_FUSION_0D, 0x00001000, 0x00003F00},            
        {MCVP_FUSION_23, 0x00000000, 0x000000FF},
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //1080i
        {MCVP_FUSION_0D, 0x00002000, 0x00003F00},            
        {MCVP_FUSION_23, 0x00000000, 0x000000FF},
        {REGTBL_END, 0x00000000, 0x00000000}
    }
};

const REGTBL_T CODE MDDI_FUSION_UI_PARAM[E_DI_QUALITY_NUM_MODE][1] = 
{
    {   //AGGRESIVE
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //DEFENSIVE
        {REGTBL_END, 0x00000000, 0x00000000}
    }
};

const REGTBL_T CODE MDDI_DAR_PARAM[3][4] =
{    
    {   //RF
        {MCVP_DARE_00       , 0x87000130, 0xFEFFFFFF},  //   
        {MCVP_DARE_11       , 0x21084000, 0xFFFFFFFF},  //Vertical Filter V-Variation Table
        {MCVP_DARE_12       , 0x22084000, 0xFFFFFFFF},  //Vertical Filter H-Variation Table    
        {REGTBL_END           , 0x00000000, 0x00000000}
    },    
    {   //SD
        {MCVP_DARE_00       , 0x87000130, 0xFEFFFFFF},  //   
        {MCVP_DARE_11       , 0x21084000, 0xFFFFFFFF},  //Vertical Filter V-Variation Table
        {MCVP_DARE_12       , 0x21084000, 0xFFFFFFFF},  //Vertical Filter H-Variation Table       
        {REGTBL_END           , 0x00000000, 0x00000000}
    },    
    {   //HD
        {MCVP_DARE_00       , 0x87000130, 0xFEFFFFFF},  //
        {MCVP_DARE_11       , 0x21084000, 0xFFFFFFFF},  //Vertical Filter V-Variation Table
        {MCVP_DARE_12       , 0x22084000, 0xFFFFFFFF},  //Vertical Filter H-Variation Table       
        {REGTBL_END           , 0x00000000, 0x00000000}
    }
};

const REGTBL_T CODE MDDI_PE_PARAM[3][1] =
{    
    {   //RF
        {REGTBL_END, 0x00000000, 0x00000000}
    },    
    {   //SD
        {REGTBL_END, 0x00000000, 0x00000000}
    },    
    {   //HD
        {REGTBL_END, 0x00000000, 0x00000000}
    }
};

const REGTBL_T CODE MDDI_PE_UI_PARAM[E_DI_QUALITY_NUM_MODE][1] =
{
    {   //AGGRESIVE
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //DEFENSIVE
        {REGTBL_END, 0x00000000, 0x00000000}
    }
};

const REGTBL_T CODE MDDI_PD_PARAM[3][15] =
{    
    {   //480i
        {MCVP_FILM_02, 0x10003020, 0xFF0F3F3F},
        {MCVP_FILM_07, 0x00100200, 0xFFFFFFFF},
        {MCVP_FILM_08, 0x00140280, 0xFFFFFFFF},
        {MCVP_FILM_09, 0x00100FA0, 0xFFFFFFFF},
        {MCVP_FILM_0A, 0x000A0FA0, 0xFFFFFFFF},
        {MCVP_FILM_0B, 0x00050300, 0xFFFFFFFF},
        {MCVP_FILM_0C, 0x000600E0, 0xFFFFFFFF},
        {MCVP_FILM_0D, 0x00050280, 0xFFFFFFFF},
        {MCVP_FILM_0E, 0x00000040, 0xFFFFFFFF},
        {MCVP_FILM_13, 0x00003010, 0x0000FFFF},
        {MCVP_FILM_14, 0x00400000, 0x3FFFFF00},
        {MCVP_FILM_18, 0x00000200, 0x003FFFFF},
        {MCVP_FILM_23, 0x00080008, 0x08FF00FF},            
        {MCVP_FILM_27, 0xC0000000, 0xC0000000},
        {REGTBL_END, 0x00000000, 0x00000000},
    },
    {   //576i
        {MCVP_FILM_02, 0x06003010, 0xFF0F3F3F},
        {MCVP_FILM_07, 0x00100200, 0xFFFFFFFF},
        {MCVP_FILM_08, 0x00140280, 0xFFFFFFFF},
        {MCVP_FILM_09, 0x00100FA0, 0xFFFFFFFF},
        {MCVP_FILM_0A, 0x000A0FA0, 0xFFFFFFFF},
        {MCVP_FILM_0B, 0x00050300, 0xFFFFFFFF},
        {MCVP_FILM_0C, 0x000600E0, 0xFFFFFFFF},
        {MCVP_FILM_0D, 0x00050280, 0xFFFFFFFF},
        {MCVP_FILM_0E, 0x00000040, 0xFFFFFFFF},
        {MCVP_FILM_13, 0x00003010, 0x0000FFFF},
        {MCVP_FILM_14, 0x00400000, 0x3FFFFF00},
        {MCVP_FILM_18, 0x00000200, 0x003FFFFF},
        {MCVP_FILM_23, 0x00080008, 0x08FF00FF},            
        {MCVP_FILM_27, 0xC0000000, 0xC0000000},
        {REGTBL_END, 0x00000000, 0x00000000},
    },
    {   //1080i
        {MCVP_FILM_02, 0x20003010, 0xFFFFFFFF},//Rate
        {MCVP_FILM_07, 0x00200200, 0xFFFFFFFF},//FR_ST MIN MAX
        {MCVP_FILM_08, 0x00200200, 0xFFFFFFFF},//FR_MO MIN MAX
        {MCVP_FILM_09, 0x04008000, 0xFFFFFFFF},//FR_SI MIN MAX
        {MCVP_FILM_0A, 0x04008000, 0xFFFFFFFF},//FI_SIM MIN MAX
        {MCVP_FILM_0B, 0x00802000, 0xFFFFFFFF},//FI_ST_32 MIN MAX
        {MCVP_FILM_0C, 0x00000100, 0xFFFFFFFF},//ED_ST_32 MIN MAX
        {MCVP_FILM_0D, 0x00801000, 0xFFFFFFFF},//FI_ST_22 MIN MAX
        {MCVP_FILM_0E, 0x00200100, 0xFFFFFFFF},//ED_ST_22 MIN MAX
        {MCVP_FILM_13, 0x00003010, 0x0000FFFF},//Saw Quant Thresh
        {MCVP_FILM_14, 0x02000000, 0x3FFFFF00},//SCENE_CHANGE_FI
        {MCVP_FILM_18, 0x00000800, 0x003FFFFF},//SCENE_CHANGE_EDGE
        {MCVP_FILM_23, 0x00080008, 0x08FF00FF},//Mixsaw ,FR_TH,Quantization Enable        
        {MCVP_FILM_27, 0xC0000000, 0xC0000000},//Enable film hardware detection 
        {REGTBL_END, 0x00000000, 0x00000000}
    }
};

const REGTBL_T CODE MDDI_PD_UI_PARAM[E_DI_QUALITY_NUM_MODE][1] =
{
    {   //AGGRESIVE
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //DEFENSIVE
        {REGTBL_END, 0x00000000, 0x00000000}
    }
};

const REGTBL_T CODE MDDI_CS_PARAM[3][8] =
{
    {   //RF (ATV,DTV)
		{MCVP_CS_04, 0x081D1C04, 0x00FFFF00},
		{MCVP_CS_05, 0x02080802, 0x00FFFF00},
		{MCVP_CS_06, 0x270F0F02, 0x00FFFF00},
		{MCVP_CS_07, 0x02140C04, 0x00FFFF00},
		{MCVP_CS_09, 0x20120E08, 0x00FFFF00},
        {MCVP_CS_20, 0x01D04200, 0x01F00000},       
        {MCVP_CS_22, 0x00000000, 0xC00FFFFF},  // reset histogram clip setting
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //PAL
		{MCVP_CS_04, 0x08100804, 0x00FFFF00},
		{MCVP_CS_05, 0x02040402, 0x00FFFF00},
		{MCVP_CS_06, 0x27040402, 0x00FFFF00},
		{MCVP_CS_07, 0x02100804, 0x00FFFF00},
		{MCVP_CS_09, 0x20100C08, 0x00FFFF00},
        {MCVP_CS_20, 0x00D04200, 0x01F00000},
        {MCVP_CS_22, 0x00000000, 0xC00FFFFF}, 
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   //NTSC
		{MCVP_CS_04, 0x08100804, 0x00FFFF00},
		{MCVP_CS_05, 0x02040402, 0x00FFFF00},
		{MCVP_CS_06, 0x27040402, 0x00FFFF00},
		{MCVP_CS_07, 0x02100804, 0x00FFFF00},
		{MCVP_CS_09, 0x20100C08, 0x00FFFF00},
        {MCVP_CS_20, 0x01E04200, 0x01F00000},
        {MCVP_CS_22, 0x00000000, 0xC00FFFFF}, 
        {REGTBL_END, 0x00000000, 0x00000000}
    }
};

static UINT8 _u1DrvDIQualityOnOff(UINT8 bPath) 
{
    if ((bPath != VDP_1) || IS_DI_BOB(VDP_1))
    {
        return SV_FALSE;
    }
    else
    {
        return SV_TRUE;
    }
}

static UINT8 _u1DrvDIFullQualityOnOff(UINT8 bPath) 
{
    if ((bPath == VDP_1) && (IS_DI_Y4C4(VDP_1) || IS_DI_Y4C2(VDP_1) || IS_DI_Y4C1(VDP_1)))
    {
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Basic On/Off Function
////////////////////////////////////////////////////////////////////////////////
static void _vDrvFilmDefaultOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    if (bPath == VDP_1)
    {
        DiPQMode.bFilmOnOff_Y = (u1OnOff & DiPQMode.bCfgFilmOnOff & _u1DrvDIFullQualityOnOff(bPath));
        DiPQMode.bFilmOnOff_MultiCadence = (DiPQMode.bFilmOnOff_Y & SUPPORT_MULTI_CADENCE);
 
        if (IS_DI_Y4C4(VDP_1))
        {
            DiPQMode.bFilmOnOff_C = DiPQMode.bFilmOnOff_Y;
        }
        else
        {
            DiPQMode.bFilmOnOff_C = SV_OFF;
        }

        // default film setting for different source or timing
        switch (bDrvVideoGetTiming(bPath))
        {
            case MODE_625I:
            case MODE_625I_OVERSAMPLE:
            case MODE_1080i_48:
            case MODE_1080i_50:
                DiPQMode.b22Film = u1OnOff;
                DiPQMode.b32Film = SV_OFF;
                u1DI_Is50HZ = SV_TRUE;
                break;
            default:
                DiPQMode.b22Film = u1OnOff;
                DiPQMode.b32Film = u1OnOff;
                u1DI_Is50HZ = SV_FALSE;
                break;
        }
        
        if ((VDP_GetPlayMode(bPath) == FBM_FBG_DTV_MODE)
			&&(bGetSignalType(VDP_1) == SV_ST_MPEG))
        {
        	// only disable multicadence detect in DTV
            DiPQMode.bFilmOnOff_MultiCadence = SV_OFF;
        }
    }    
}

static void _vDrvSetIfOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    vIO32WriteFldAlign(MCVP_FUSION_00, u1OnOff, IFUSION_EN);
}

static void _vDrvSetDarOnOff(UINT8 bPath, UINT8 u1OnOff)
{
#ifdef __MODEL_slt__
    vIO32WriteFldAlign(MCVP_DARE_00, SV_OFF, DARE_EN);
#else
    vIO32WriteFldAlign(MCVP_DARE_00, u1OnOff, DARE_EN);
#endif    
}

static void _vDrvSetCsOnOff(UINT8 bPath, UINT8 u1OnOff)
{
    LOG(6, "[%d]  CS turn %d\n", bPath, u1OnOff);
    vIO32WriteFldAlign(MCVP_FUSION_00, u1OnOff, IF_CS_EN);
}


////////////////////////////////////////////////////////////////////////////////
// Quality mainloop function
////////////////////////////////////////////////////////////////////////////////
static void _vDrvDISetPDMAQuality(UINT8 bPath)
{
    vDrvLoadRegTbl(MDDI_PDMA_PARAM);
    
    switch (bDrvVideoGetTiming(VDP_2))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            vIO32WriteFldAlign(VDO_MODE, SV_ON, HD_EN);           
            break;
        default:
            vIO32WriteFldAlign(VDO_MODE, SV_OFF, HD_EN);                       
            break;
    } 

    vIO32WriteFldAlign(MCVP_SB_01, (SV_VD_TVD3D == _arMDDiPrm[bPath].u1DecType), SB_NR_PAL_C_FILTER);                          
    if (_arMDDiPrm[VDP_1].u1IsDualFPI)
    {
        vIO32WriteFldAlign(MA4F_FCHECK_YW, 0xA, TH_NORM_FCHECK_YW);
        vIO32WriteFldAlign(MA4F_TH_YW_NORM, 0x6, TH_NORM_YW);
    }
    else
    {
        vIO32WriteFldAlign(MA4F_FCHECK_YW, 0x36, TH_NORM_FCHECK_YW);
        vIO32WriteFldAlign(MA4F_TH_YW_NORM, 0x1c, TH_NORM_YW);
    } 
}

static void _vDrvDISetIFQuality(UINT8 bPath)
{
    switch (bDrvVideoGetTiming(bPath))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            vDrvLoadRegTbl(MDDI_FUSION_PARAM[2]);

            if (bPath == VDP_1)
            {
                // Enable DTV tearing detection in DTV 1080i
                DiPQMode.bDTVTear = DI_DTV_TEAR_EN ? 
                    ((VDP_GetPlayMode(bPath) == FBM_FBG_DTV_MODE) && (bGetSignalType(VDP_1) == SV_ST_MPEG)) : SV_OFF;
            }
            break;
        default:            
            if ((bGetSignalType(VDP_1) == SV_ST_TV) 
                || (bGetSignalType(VDP_1) == SV_ST_MPEG))
            {
                vDrvLoadRegTbl(MDDI_FUSION_PARAM[0]);
            }
            else
            {
                vDrvLoadRegTbl(MDDI_FUSION_PARAM[1]);
            }            
            DiPQMode.bDTVTear = SV_OFF;
            break;
    }

    if (bGetSignalTypeNew(VDP_1) == SV_ST_AV)
    {
        vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_01, 0x620, CZP_3D_TH);
    }
    else
    {
        vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_01, 0x800, CZP_3D_TH);
    }
    
    if (DiPQMode.eIfQty < E_DI_QUALITY_NUM_MODE)
    {
        vDrvLoadRegTbl(MDDI_FUSION_UI_PARAM[DiPQMode.eIfQty]);   
    }    

    if (bPath == VDP_1)
    {
        // Enable chroma jaggy detection in SD progressive timing
        DiPQMode.bCJaggy = DI_CHROMA_JAGGY_EN ? 
                (IS_SD_TIMING(VDP_1) && IS_PROGRESSIVE(VDP_1)) : SV_OFF;
    }

    vIO32WriteFldAlign(VDP_SCPQ_04, DiPQMode.bDTVTear, SCPQ_DTV_TEAR_EN);
    vIO32WriteFldAlign(VDP_SCPQ_04, DiPQMode.bCJaggy, SCPQ_C_JAGGY_EN);
    vIO32WriteFldAlign(VDP_SCPQ_03, 0, SCPQ_CUST_VY_OFST);
    vIO32WriteFldAlign(VDP_SCPQ_03, 0, SCPQ_CUST_VC_OFST);    
    vApiRegisterVideoEvent(PE_EVENT_SCALER_V, SV_VP_MAIN, SV_ON);

    DiPar.IfPar.bMaxMoC = 8;

    //Not C4 mode, Turn off chroma motion reference 
    vIO32WriteFldAlign(MCVP_FUSION_08, IS_DI_Y4C4(VDP_1) ? 1 : 0, IF_YC_C_TO_Y_W);

	//reset settings in H.264 rectangle flash
	vIO32WriteFldAlign(PSCAN_FWCS_03,0x30,HIST_DIFF_RATIO);
	MDDI_WRITE_FLD(VDP_1,MCVP_FUSION_0D, 0x10,IF_RA_CLIP_FLICKER_TH);
}

static void _vDrvDISetPEQuality(UINT8 bPath)
{
    switch (bDrvVideoGetTiming(bPath))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            vDrvLoadRegTbl(MDDI_PE_PARAM[2]);
            break;
        default:
            if ((bGetSignalType(VDP_1) == SV_ST_TV) 
                || (bGetSignalType(VDP_1) == SV_ST_MPEG))
            {
                vDrvLoadRegTbl(MDDI_PE_PARAM[0]);
            }
            else
            {
                vDrvLoadRegTbl(MDDI_PE_PARAM[1]);
            }            
            break;
    }
    
    if (DiPQMode.ePeQty < E_DI_QUALITY_NUM_MODE)
    {
        vDrvLoadRegTbl(MDDI_PE_UI_PARAM[DiPQMode.ePeQty]);    
    }

    vIO32WriteFldAlign(MCVP_PE_08, 0x10, PE_DIR_HIST_START_X);    
    vIO32WriteFldAlign(MCVP_PE_09, 0x10, PE_DIR_HIST_START_Y);    
    vIO32WriteFldAlign(MCVP_PE_08, _arMDDiPrm[bPath].u2Width-0x10, PE_DIR_HIST_END_X);
    vIO32WriteFldAlign(MCVP_PE_09, _arMDDiPrm[bPath].u2Height-0x10, PE_DIR_HIST_END_Y);     
    vIO32WriteFldAlign(MCVP_PE_08, IS_SD_TIMING(bPath) ? 1 : 2, PE_DIR_HIST_QUANT);    
    
    vIO32WriteFldAlign(MCVP_PE_10, 0x10, PE_ALIAS_CNT_START_X);    
    vIO32WriteFldAlign(MCVP_PE_11, 0x10, PE_ALIAS_CNT_START_Y);    
    vIO32WriteFldAlign(MCVP_PE_10, _arMDDiPrm[bPath].u2Width-0x10, PE_ALIAS_CNT_END_X);
    vIO32WriteFldAlign(MCVP_PE_11, _arMDDiPrm[bPath].u2Height-0x10, PE_ALIAS_CNT_END_Y);     
    vIO32WriteFldAlign(MCVP_PE_10, IS_SD_TIMING(bPath) ? 1 : 2, PE_ALIAS_CNT_QUANT);    
}

static void _vDrvDISetPDQuality(UINT8 bPath)
{
    switch (bDrvVideoGetTiming(bPath))
    {
        //HD
        case MODE_1080i_48:
        case MODE_1080i_50:       
        case MODE_1080i:                    
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x60, FR_MIN);  
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x40, EG_MIN);            
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x02, EG_RATE_22);    
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x600, EG_MAX_END_32);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x600, EG_MAX_END_22);              
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x16, EG_MO_FILTER_L);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x08, FR_MO_FILTER_L);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x28, FR_MO_FILTER_H);
			if(IS_PANEL_2D_N_3D_L12R12 && PANEL_GetPanelWidth()<1200)
			{
				vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x28, EG_MO_FILTER_H); 
			}
			else
			{
                vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x34, EG_MO_FILTER_H);               
			}
            vIO32WriteFldAlign(PSCAN_FWFILM_06, 0x1, FILED_WEAVE_EN);            
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x40, FLD_MAX);            
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x68, FLD_MIN);            
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x50, FRM_MAX);
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x50, FRM_MIN);            
            vDrvLoadRegTbl(MDDI_PD_PARAM[2]);
            break;
        //PAL
        case MODE_625I:
        case MODE_625I_OVERSAMPLE:
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, FR_MIN);  
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, EG_MIN);            
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x03, EG_RATE_22);    
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x100, EG_MAX_END_32);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x80, EG_MAX_END_22);             
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, (bGetSignalType(VDP_1) == SV_ST_MPEG) ? 0x15 : 0x06, EG_MO_FILTER_L);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x24, EG_MO_FILTER_H);                
            vIO32WriteFldAlign(PSCAN_FWFILM_06, 0x1, FILED_WEAVE_EN);            
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x10, FLD_MAX);
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x20, FLD_MIN);
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MAX);
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MIN);    
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x10, FR_MO_FILTER_L);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x30, FR_MO_FILTER_H);

            if (IS_PANEL_L12R12)
            {
                vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x0C, EG_MO_FILTER_L);
                vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x22, EG_MIN);            
            }
   
            vDrvLoadRegTbl(MDDI_PD_PARAM[1]);
            break;
        //NTSC            
        default:
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, FR_MIN);  
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, EG_MIN);            
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x03, EG_RATE_22);    
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x100, EG_MAX_END_32);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x80, EG_MAX_END_22);               
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x06, EG_MO_FILTER_L);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x24, EG_MO_FILTER_H);                   
            vIO32WriteFldAlign(PSCAN_FWFILM_06, 0x1, FILED_WEAVE_EN);            
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x10, FLD_MAX);
            vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x20, FLD_MIN);
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MAX);
            vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MIN);     
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x08, FR_MO_FILTER_L);
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x28, FR_MO_FILTER_H);

            if (IS_PANEL_L12R12)
            {
                vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x0C, EG_MO_FILTER_L);
                vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x22, EG_MIN);            
            }       
            vDrvLoadRegTbl(MDDI_PD_PARAM[0]);
            break;
    }

    if (bGetSignalType(VDP_1) == SV_ST_MPEG)
    {
        //vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x10, FR_MO_FILTER_L);
        //vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x30, FR_MO_FILTER_H);
        vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x04, EG_RATE_22); 
		if((VDP_GetPlayMode(bPath) != FBM_FBG_DTV_MODE)
			&&(bDrvVideoGetTiming(VDP_1) == MODE_1080i))
		{	
			// only for MM 1080i use loose threshold for 22 film detect
			vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x02, EG_RATE_22);
		}
    }
    else
    {
        //vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x08, FR_MO_FILTER_L);
        //vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x28, FR_MO_FILTER_H);
}
    //for mp3 patch
    if ((bGetSignalType(VDP_1) == SV_ST_YP) || (bGetSignalType(VDP_1) == SV_ST_DVI))
    {
        vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 1, DI_FILM_APL_EN);
    }
    else
    {
        vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0, DI_FILM_APL_EN);
    } 
    if (DiPQMode.eFilmQty < E_DI_QUALITY_NUM_MODE)
    {
        vDrvLoadRegTbl(MDDI_PD_UI_PARAM[DiPQMode.eFilmQty]);   
    }     
}

static void _vDrvDISetCSQuality(UINT8 bPath)
{
    UINT8 bTimingType;

    bTimingType = bDrvVideoGetTiming(bPath);
    
    _u1DIState =0;
    switch (bGetSignalType(bPath))
    {
        case SV_ST_TV:
        case SV_ST_MPEG:
            vDrvLoadRegTbl(MDDI_CS_PARAM[0]);
            break;
        default:
            if((bTimingType == MODE_625I) || (bTimingType == MODE_625I_OVERSAMPLE))
            {
                vDrvLoadRegTbl(MDDI_CS_PARAM[1]);
            }
            else
            {
                vDrvLoadRegTbl(MDDI_CS_PARAM[2]);
            }
            break;
    }

    if((_arMDDiPrm[bPath].u2Width <=720) || IS_OVERSAMPLE(bPath)) //SD Timing
    {
        vIO32WriteFldAlign(MCVP_CS_00, 0, ME_MODE);
        vIO32WriteFldAlign(MCVP_CS_03, 0x1C, SAD_GRA_RATIO1);
        vIO32WriteFldAlign(MCVP_CS_03, 0x1E, SAD_GRA_RATIO2);
        vIO32WriteFldAlign(MCVP_CS_14, 0x78, SC_SAD_DIFF_THD);
       if ((bGetVideoDecTypeNew(bPath) == SV_VD_YPBPR) &&(bPath == SV_VP_MAIN && VSS_MAJOR(_bSrcMainNew) == VSS_SCART))
        {
            vIO32WriteFldAlign(PSCAN_FWCS_00, 0x70, GMV_EDGE_RATIO);
            vIO32WriteFldAlign(PSCAN_FWCS_01,0x2 ,NON_ZERO_CNT);
        }
        else
        {
        vIO32WriteFldAlign(PSCAN_FWCS_00, 0x30, GMV_EDGE_RATIO); 
            vIO32WriteFldAlign(PSCAN_FWCS_01,0x3 ,NON_ZERO_CNT);
        } 
    }
    else if(_arMDDiPrm[bPath].u2Width <=1280)//720P
    {
        vIO32WriteFldAlign(MCVP_CS_00, 1, ME_MODE);
        vIO32WriteFldAlign(MCVP_CS_03, 0xA, SAD_GRA_RATIO1);
        vIO32WriteFldAlign(MCVP_CS_03, 0x10, SAD_GRA_RATIO2);
        vIO32WriteFldAlign(PSCAN_FWCS_00, 0x1C, GMV_EDGE_RATIO);     
        vIO32WriteFldAlign(PSCAN_FWCS_01,0x3 ,NON_ZERO_CNT);
        vIO32WriteFldAlign(MCVP_CS_14, 0x78, SC_SAD_DIFF_THD);    
    }
    else if(IS_INTERLACE(bPath))//1080I
    {
        vIO32WriteFldAlign(MCVP_CS_00, 2, ME_MODE);
        vIO32WriteFldAlign(MCVP_CS_03, 0xA, SAD_GRA_RATIO1);
        vIO32WriteFldAlign(MCVP_CS_03, 0x10, SAD_GRA_RATIO2);
        vIO32WriteFldAlign(PSCAN_FWCS_00, 0x1C, GMV_EDGE_RATIO);     
        vIO32WriteFldAlign(PSCAN_FWCS_01,0x3 ,NON_ZERO_CNT);
        vIO32WriteFldAlign(MCVP_CS_14, 0xA0, SC_SAD_DIFF_THD);
    }
    else//1080P
    {
        vIO32WriteFldAlign(MCVP_CS_00, 3, ME_MODE);
        vIO32WriteFldAlign(MCVP_CS_03, 0xA, SAD_GRA_RATIO1);
        vIO32WriteFldAlign(MCVP_CS_03, 0x10, SAD_GRA_RATIO2);
        vIO32WriteFldAlign(PSCAN_FWCS_00, 0x1C, GMV_EDGE_RATIO);     
       vIO32WriteFldAlign(PSCAN_FWCS_01,0x3 ,NON_ZERO_CNT);
        vIO32WriteFldAlign(MCVP_CS_14, 0xA0, SC_SAD_DIFF_THD);
    }    
}

static void _vDrvDISetDARQuality(UINT8 bPath)
{
    UINT8 u1Index;
    
    switch (bDrvVideoGetTiming(bPath))
    {
        case MODE_1080i_48:
        case MODE_1080i_50:
        case MODE_1080i:
            u1Index = 2;
            break;
        default:
            if ((bGetSignalType(VDP_1) == SV_ST_TV) 
                || (bGetSignalType(VDP_1) == SV_ST_MPEG))
            {
                u1Index = 0;
            }
            else
            {
                u1Index = 1;
            }            
            break;
    }
    
    vDrvLoadRegTbl(MDDI_DAR_PARAM[u1Index]);

    if(IS_PR_MODE(bPath))
    {
        vIO32WriteFldAlign(MCVP_DARE_00, SV_OFF, DARE_LINE_FLICKER_EN);        
        vIO32WriteFldAlign(MCVP_DARE_00, SV_OFF, DARE_VD_SMOOTH_EN);        
        vIO32WriteFldAlign(MCVP_DARE_00, SV_OFF, DARE_VS_SMOOTH_EN);        
        vIO32WriteFldAlign(MCVP_DARE_00, SV_OFF, DARE_LUMA_JAGGY_EN);        
        vIO32WriteFldAlign(MCVP_DARE_00, SV_OFF, DARE_SINGLE_SAW_EN);
        vIO32WriteFldAlign(MCVP_DARE_00, SV_OFF, DARE_CONT_SAW_EN);                
    }
    
    DiPar.DarPar.u4DarCfg = u4IO32Read4B(MCVP_DARE_00);
}

static void _vDrvDISetAdatpive(UINT8 bPath)
{
    UINT32 u4TotalBlk = ((_arMDDiPrm[VDP_1].u2Width>>4)-2)*((_arMDDiPrm[VDP_1].u2Height>>1)-4);

    u4TotalBlk = IS_INTERLACE(VDP_1) ? u4TotalBlk : u4TotalBlk*2;
    //u4TotalBlk = IO32ReadFldAlign(MCVP_MULTI_00, DET_SUB) ? (u4TotalBlk>>1) : u4TotalBlk;

    vIO32WriteFldAlign(PSCAN_FWCS_00, u4TotalBlk, TOTAL_BLK); 
}

/**
 * @Description: Following functions are for software film detection when FS output. 
 * @vDrvSetSWFilmInfoForMJC() is called in output vsync falling.
 * @u1DrvGetSWFilmInfoFromDI() is a interface for MJC to query film status
 * @MJC should query it only in condition of DI FS output at interlace timing.
 */
static UINT8 u1DiFWFilmInfo = 0;
UINT8 u1DrvGetSWFilmInfoFromDI(void)
{
    return u1DiFWFilmInfo;
}

void vDrvSetSWFilmInfoForMJC(UINT16 u2DiFilmMode)
{
    UINT8 u1Film22FldMotion = DiPar.PdPar.u4FldSeq & 0x1;
    UINT8 u1Film32FrmMotion = DiPar.PdPar.u4FrmSeq & 0x1F;
    UINT8 u1Loop = 0;
    switch(u2DiFilmMode)
    {
        case E_DI_FILM_22:
            if(u1Film22FldMotion)
            {
                u1DiFWFilmInfo = 7;
            }
            else
            {
                u1DiFWFilmInfo = 6;
            }
            break;
        case E_DI_FILM_32:
            for(u1Loop = 0; u1Loop <= 4;u1Loop ++)
            {
                if(((u1Film32FrmMotion>>u1Loop)&0x1) == 0)
                break;
            }
            u1DiFWFilmInfo = (u1Loop + 2)%5 + 1;
            break;
        default:
            u1DiFWFilmInfo = 0;
            break;
    }
    vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG,u1DiFWFilmInfo,PDINFO_MSK_AT_FS_OUTPUT);
}

/**
 * @Description: Following functions will get PDinfo delay by DRAM R/W pointer
 * @u1SetPDInfoDelayAtProgDispMode() is called in input vsync falling.
 * @vSetPDInfoDelayAtProgDispMode() is a interface for MJC to query film status
 * @MJC should query it only in condition of prog pscan display mode.
 */
static UINT8 u1PDInfoDelay = 0;
UINT8 u1GetPDInfoDelayAtProgDispMode(void)
{
    return u1PDInfoDelay;
}

void vSetPDInfoDelayAtProgDispMode(void)
{
    UINT8 u1NRWPointerY;
    UINT8 u1SCRPointer;

    u1SCRPointer = vDrvDIGetDADebugStatus(3) & 0x7;
    u1NRWPointerY = (vDrvDIGetDADebugStatus(2)>>6) & 0x7;
    if(u1NRWPointerY < u1SCRPointer)
    {
        u1PDInfoDelay = u1NRWPointerY + MDDI_READ_FLD(VDP_1, MCVP_DRAM_08 ,DA_FRAME_NUM_Y) + 1 - u1SCRPointer;
    }
    else
    {
        u1PDInfoDelay = u1NRWPointerY - u1SCRPointer;
    }
    vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG,u1PDInfoDelay,PDINFO_DLY_AT_PRO_DISP);
}

////////////////////////////////////////////////////////////////////////////////
// Quality ISR 
////////////////////////////////////////////////////////////////////////////////
static void _vDrvDIMutliCadence(void)
{
    UINT16 i, wTest; 
    
    DiPar.PdPar.u2FilmMode = E_DI_FILM_NONE;

    if ((DiPar.PdPar.u4FldBit == 4) && (DiPar.PdPar.u4FrmBit == 0)) 
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_22;
    }
    else if ((DiPar.PdPar.u4FldBit == 4) && (DiPar.PdPar.u4FrmBit == 4)) 
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_NONE;
        DiPar.PdPar.u4FldBit = 0;
        DiPar.PdPar.u4FrmBit = 0;
    }
    //else if ((DiPar.PdPar.u4FldBit == 6) && (DiPar.PdPar.u4FrmBit == 6))
    //{
    //    DiPar.PdPar.u2FilmMode = E_DI_FILM_NONE;
    //    DiPar.PdPar.u4FldBit = 0;
    //    DiPar.PdPar.u4FrmBit = 0;
    //}
    else if ((DiPar.PdPar.u4FldBit == 8) && (DiPar.PdPar.u4FrmBit == 8))
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_44;
    }
    else if ((DiPar.PdPar.u4FldBit == 10) && (DiPar.PdPar.u4FrmBit == 10))
    {
        for (i=0; i<10; i++)
        {
            wTest = (DiPar.PdPar.u4FrmMask>>i)&0x3FF;
        
            if (wTest == FW_FILM_FR_SEQ_32)
            {
                if(((DiPar.PdPar.u4FldSeq&0x1F)==0x12)||((DiPar.PdPar.u4FldSeq&0x1F)==0x05)||
                   ((DiPar.PdPar.u4FldSeq&0x1F)==0x0A)||((DiPar.PdPar.u4FldSeq&0x1F)==0x14)||
                   ((DiPar.PdPar.u4FldSeq&0x1F)==0x09))
                {
                    DiPar.PdPar.u2FilmMode = E_DI_FILM_32;
                }
                else
                {
                    DiPar.PdPar.u2FilmMode = E_DI_FILM_NONE;
                    DiPar.PdPar.u4FldBit = 0;    // fix 32<->87  identify error issue
                    DiPar.PdPar.u4FrmBit = 0;
                    DiPar.PdPar.bFwBadEdit = 0;
                }
                break;
            }
            
            if (wTest == FW_FILM_FR_SEQ_55)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_55;
                break;
            } 
            
            if (wTest == FW_FILM_FR_SEQ_2224)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_2224;
                break;
            }

            if (wTest == FW_FILM_FR_SEQ_2332)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_2332;
                break;
            }

            if (wTest == FW_FILM_FR_SEQ_64)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_64;
                break;
            }
        }
    }
    else if ((DiPar.PdPar.u4FldBit == 12) && (DiPar.PdPar.u4FrmBit == 12))
    {
        for (i=0; i<8; i++)
        {
            wTest = (DiPar.PdPar.u4FrmMask>>i)&0x0FF;
        
            if (wTest == FW_FILM_FR_SEQ_44)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_44;
                break;
            }
        }

        if(DiPar.PdPar.u2FilmMode == E_DI_FILM_NONE)
        {
            for (i=0; i<12; i++)
            {
                wTest = (DiPar.PdPar.u4FrmMask>>i)&0x0FFF;
            
                if (wTest == FW_FILM_FR_SEQ_32322)
                {
                    DiPar.PdPar.u2FilmMode = E_DI_FILM_32322;
                    break;
                }
                
                if (wTest == FW_FILM_FR_SEQ_66)
                {
                    DiPar.PdPar.u2FilmMode = E_DI_FILM_66;
                    break;
                }
            }
        }
    }
    else if ((DiPar.PdPar.u4FldBit == 15) && (DiPar.PdPar.u4FrmBit == 15))
    {        
        for (i=0; i<10; i++)
        {
            wTest = (DiPar.PdPar.u4FrmMask>>i)&0x3FF;
        
            if (wTest == FW_FILM_FR_SEQ_32)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_32;
                break;
            }

            if (wTest == FW_FILM_FR_SEQ_55)
            {
                DiPar.PdPar.u2FilmMode = E_DI_FILM_55;
                break;
            }
        }
        
        if(DiPar.PdPar.u2FilmMode == E_DI_FILM_NONE)
        {
            for (i=0; i<15; i++)
            {
                wTest = (DiPar.PdPar.u4FrmMask>>i)&0x7FFF;
            
                if (wTest == FW_FILM_FR_SEQ_87)
                {
                    DiPar.PdPar.u2FilmMode = E_DI_FILM_87;
                    break;
                }            
            }
        }
    }
    else
    {
        DiPar.PdPar.u2FilmMode = E_DI_FILM_NONE;
    }	

    vDrvSetSWFilmInfoForMJC(DiPar.PdPar.u2FilmMode);
}

static void _vDrvDISetOSD(void)
{
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.u4FldBit ? 1 : 0, DI_DBG_FLDBIT);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.u4FrmBit ? 1 : 0, DI_DBG_FRMBIT);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.bUnknowCnt ? 1 : 0, DI_DBG_UNKNOWN);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiSta.PeSta.u4EdgeSum ? 0 : 1, DI_DBG_NOEDGE);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, (DiPar.CsPar.bFilmCnt==8) ? 1 : 0, DI_DBG_CSFILM);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.bFwWeave ? 1 : 0, DI_DBG_FWWEAVE);    
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.IfPar.bMovingCzp ? 1 : 0, DI_DBG_MOVINGCZP);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.IfPar.bFldInv, DI_DBG_FLDINV);
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.u2FilmMode, DI_DBG_FILMMODE);    
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.CsPar.bFavorCS, DI_DBG_FAVORCS);    
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.IfPar.bCzpState, DI_DBG_CZPSTATE);     
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.bFilm24To50, DI_DBG_FILM24TO50);    
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.bSmallMo, DI_DBG_SMALL_MO);     
    vDrvDISetDebugOSDMask(E_OSD_DBG_PSCAN, DiPar.PdPar.bFilmPatch, DI_DBG_FILM_PATCH); 
}

static void _vDrvDIPDSetAdapThld(void)
{
    vIO32WriteFldAlign(MCVP_FILM_23, DiPar.PdPar.u4AdaptivePxlMotionTh>>1, FR_MO_PXL_DIFF_THL);
    vIO32WriteFldAlign(MCVP_FILM_02, DiPar.PdPar.u4AdaptivePxlMotionTh, FR_MO_CUR_THL_2);
    vIO32WriteFldAlign(MCVP_FILM_23, DiPar.PdPar.u4AdaptiveEgMotionTh>>1, R_MIXSAW);
    vIO32WriteFldAlign(MCVP_FILM_13, DiPar.PdPar.u4AdaptiveEgMotionTh, FI_MO_CUR_THL_2);    
    vIO32WriteFldAlign(MCVP_FILM_07, DiPar.PdPar.u4AdaptiveFrMoTh, FR_ST_MIN_TH);
    vIO32WriteFldAlign(MCVP_FILM_08, DiPar.PdPar.u4AdaptiveFrMoTh, FR_MO_MIN_TH); 
    vIO32WriteFldAlign(MCVP_FILM_07, DiPar.PdPar.u4AdaptiveFrMoTh, FR_ST_MAX_TH);
    vIO32WriteFldAlign(MCVP_FILM_08, DiPar.PdPar.u4AdaptiveFrMoTh, FR_MO_MAX_TH);     
    vIO32WriteFldAlign(MCVP_FILM_0C, DiPar.PdPar.u4AdaptiveEgMo32Th, NEW_FI_ST_MAX_TH);
    vIO32WriteFldAlign(MCVP_FILM_0E, DiPar.PdPar.u4AdaptiveEgMo22Th, NEW_FI_ST_MAX_TH_22);
}

static void _vDrvDIPDFwPatch(void)
{
#if MDDI_PD_SW_FW_FILM_PATCH //Patch for pulldown.trp
    
    UINT32 u4CurFilmMaskFr32;
    static UINT32 u4PeCount = 0;
    UINT32 u4ASL = u1DrvGetASL();
    UINT32 u4APL = bDrvGetAPL();

    DiPar.PdPar.bFWPatch32FrSeq = (DiPar.PdPar.bFWPatch32FrSeq<<1)|(DiSta.PdSta.u4FrmMotion==0);

    u4CurFilmMaskFr32 = (FW_FILM_FI_SEQ_32 >> ((FW_FILM_CB-1)-DiPar.PdPar.bFWPatchCB32)) & FW_FILM_MASK;

    if ((DiPar.PdPar.bFWPatch32FrSeq & FW_FILM_MASK) == u4CurFilmMaskFr32)
    {           
        DiPar.PdPar.bFWPatchCB32 = (DiPar.PdPar.bFWPatchCB32 == (FW_FILM_CB-1)) ? 0 : DiPar.PdPar.bFWPatchCB32 + 1;
        DiPar.PdPar.bFWPatchFr32Cnt = MIN(DiPar.PdPar.bFWPatchFr32Cnt+1, MDDI_FILM_ACC_MAX_SEQ_CNT);  
    }
    else
    {
        DiPar.PdPar.bFWPatchCB32 = 0;
        DiPar.PdPar.bFWPatchFr32Cnt = 0;        
    }

    if (DiPar.PdPar.bFWPatchCB32 == 3)
    {
        if (DiSta.PdSta.u4EdgMotion == 0)
        {
            DiPar.PdPar.bFWPatchFi32Cnt = MIN(DiPar.PdPar.bFWPatchFi32Cnt+1, MDDI_FILM_ACC_MAX_SEQ_CNT);
        }
        else
        {
            DiPar.PdPar.bFWPatchFi32Cnt = 0;
        }
    }    

    if ((bGetSignalType(VDP_1) != SV_ST_TV) 
        && (bGetSignalType(VDP_1) != SV_ST_MPEG)
        && (u4APL > 150) && (u4APL < 190) 
        && (u4ASL > 18) && (u4ASL < 26) 
        && (DiSta.PdSta.bCurFilmMode&E_DI_FILM_32))
    {
        u4PeCount = (u4PeCount < 0x08) ? (u4PeCount+1) : 0x08;
    }
    else
    {
        u4PeCount = 0;
    }

    //LOG(3, "Count %d : APL %d ASL %d Edge0 %x  \n", u4PeCount, u4APL, u4ASL, DiSta.PeSta.u4EdgeHis[0]);
       
    if (((DiPar.PdPar.bFWPatchFi32Cnt > 0x4) 
        && (DiPar.PdPar.bFWPatchFr32Cnt > 0x10)
        && (IS_MPEG(VDP_1))
        && (u4ASL < 0x10))
        // for HQV2.0 Text vertical moving
        || (((IS_SD_TIMING(VDP_1)) && (!IS_MPEG(VDP_1)) && (u4PeCount == 8))))
    {    
        vIO32WriteFldAlign(MCVP_FILM_01, 0, FILM_DET_TYPE);
        DiPar.PdPar.bFilmPatch = 1;
    }
    else
    {
        vIO32WriteFldAlign(MCVP_FILM_01, 4, FILM_DET_TYPE);
        DiPar.PdPar.bFilmPatch = 0;
    }
#endif
}

static void _vDrvDIPDFwFilmDetetion(void)
{
    UINT16 i, j;
    UINT8 u1FldGain, u1FrmGain;
    UINT16 u2FrmMaskCur, u2FldMaskCur, u2Mask;    
    UINT32 u4FrmSaw = DiSta.PdSta.u4FrmMotion;   
    UINT32 u4FldSaw = DiSta.PdSta.u4EdgMotion; 
    UINT32 u4FrmSeq, u4FrmSeqNew;
    UINT32 u4FldSeq, u4FldSeqNew;    
    static UINT32 u4FrmMoThld = 0, u4FldMoThld = 0; 
    UINT8 u1Film24To50Det = IO32ReadFldAlign(PSCAN_FWFILM_06, DET_24TO50_EN);

    //FW FILM DETECTION 
    UINT8 u1FldGain1 = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_GAIN1);  
    UINT8 u1FldGain2 = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_GAIN2);  
    UINT32 u4FldMin = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_MIN);                                                       
    UINT32 u4FldMax = IO32ReadFldAlign(PSCAN_FWFILM_00, FLD_MAX)<<4;    
    UINT8 u1FrmGain1 = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_GAIN1);  
    UINT8 u1FrmGain2 = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_GAIN2);  
    UINT32 u4FrmMin = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_MIN);                                                       
    UINT32 u4FrmMax = IO32ReadFldAlign(PSCAN_FWFILM_02, FRM_MAX)<<8; 

    //FW weave use agressive mode to avoid enter and exit 22 film frequently(cause MJC non-smooth moving)
    if((_arMDDiPrm[VDP_1].eTDOutput == E_TD_OUT_3D_FS) && IS_INTERLACE(VDP_1))
    {
        u4FldSaw = DiSta.PdSta.u4FldMotion; 
    }

    //Force exit 
    if (DiPar.PdPar.u4FldBit && DiPar.PdPar.u4FrmBit && (DiPar.PdPar.u4FldBit != DiPar.PdPar.u4FrmBit))
    {
        DiPar.PdPar.u4FldBit = 0;
        DiPar.PdPar.u4FrmBit = 0;
        DiPar.PdPar.bFwBadEdit = 0;                                                                              
    }

    if (u4FldSaw > DiPar.PdPar.u4FldThld)                                                                                     
    {          
        DiPar.PdPar.u4FldSeq = (DiPar.PdPar.u4FldSeq<<1) | 1;   
        u4FldMoThld = ((DiPar.PdPar.u4FldBit)&&(DiPar.PdPar.u4FldSeq)) ? (u4FldSaw>>3) : 0;
        u1FldGain = u1FldGain2;
    }                                                                                                                         
    else                                                                                                                      
    {           
        DiPar.PdPar.u4FldSeq = (DiPar.PdPar.u4FldSeq<<1) | 0;   
        u1FldGain = u1FldGain1;
    }

    //Frame Sequence
    if (u4FrmSaw > DiPar.PdPar.u4FrmThld)                                                                                     
    {                    
        DiPar.PdPar.u4FrmSeq = (DiPar.PdPar.u4FrmSeq<<1) | 1;   
        u4FrmMoThld = ((DiPar.PdPar.u4FrmBit)&&(DiPar.PdPar.u4FrmSeq)) ? (u4FrmSaw>>3) : 0;
        u1FrmGain = u1FrmGain2;
    }                                                                                                                         
    else                                                                                                                      
    {                               
        DiPar.PdPar.u4FrmSeq = (DiPar.PdPar.u4FrmSeq<<1) | 0;  
        u1FrmGain = u1FrmGain1;
    } 

    //Avoid 
    u4FrmMoThld = ((DiPar.PdPar.u4FrmSeq&0xFFFF) == 0) ? 0 : u4FrmMoThld;
    u4FldMoThld = ((DiPar.PdPar.u4FldSeq&0xFFFF) == 0) ? 0 : u4FldMoThld;

    u4FldSeq = DiPar.PdPar.u4FldSeq;                                                                                        
    u4FrmSeq = DiPar.PdPar.u4FrmSeq;        
    DiPar.PdPar.u4FldThld = MIN(u4FldMax, MAX(u4FldMin, MAX((u4FldSaw*u1FldGain)>>4, u4FldMoThld))); 
    DiPar.PdPar.u4FrmThld = MIN(u4FrmMax, MAX(u4FrmMin, MAX((u4FrmSaw*u1FrmGain)>>4, u4FrmMoThld)));  

    //Field Mask matching                                                                                                           
    if (DiPar.PdPar.u4FldBit)                                                                                                     
    {                                                                                                                         
        DiPar.PdPar.bFldCB = (DiPar.PdPar.bFldCB == (DiPar.PdPar.u4FldBit-1)) ? 0 : (DiPar.PdPar.bFldCB+1);                                   
                                                                                                                              
        u2Mask = FILM_MASK(DiPar.PdPar.u4FldBit);                                                                             
        u2FldMaskCur = (DiPar.PdPar.u4FldMask>>(DiPar.PdPar.u4FldBit-DiPar.PdPar.bFldCB))&u2Mask;  
        u4FldSeq = u4FldSeq&u2Mask;
        
        if (DiPar.PdPar.bFldCnt > 0x10) 
        {
            u4FldSeqNew = u4FldSeq|u2FldMaskCur;
        }
        else
        {            
            u4FldSeqNew = u4FldSeq;   
        }
        
        if (u2FldMaskCur == u4FldSeqNew)   
        {                                 
            DiPar.PdPar.bFldCnt = MIN(DiPar.PdPar.bFldCnt+1, MDDI_FILM_MAX_SEQ_CNT);   
            
            if ((DiPar.PdPar.bFilm24To50 == SV_TRUE) && 				
                ((DiPar.PdPar.bFldCnt > 0x12) || (DiPar.PdPar.u4FldBit != 4)))          
            {               
                DiPar.PdPar.bFilm24To50 = SV_FALSE;             
                LOG(5, "Other Film sequence\n");            
            }
        }                                                                                                                     
        else                                                                                                                  
        {   
            if ((DiPar.PdPar.u4FldBit == 4) && (DiPar.PdPar.bFldCnt == 0x12))
			{
				DiPar.PdPar.bFilm24To50 = u1Film24To50Det ? SV_TRUE : SV_FALSE;
				LOG(5, "This 24 to 50 sequence\n");
			}
			else if ((DiPar.PdPar.u4FldBit != 4) && (DiPar.PdPar.bFldCnt > 0x4))
			{
				DiPar.PdPar.bFilm24To50 = SV_FALSE;
				LOG(5, "Other Film sequence jump\n");
			}
			else
			{
				LOG(5, "Not sure \n");
			}
            
            DiPar.PdPar.bFldCB = 0;                                                                                               
            DiPar.PdPar.u4FldBit = 0;   
            DiPar.PdPar.u4FldMask = 0; 
            DiPar.PdPar.bFwBadEdit = 0;  
            DiPar.PdPar.bFldCnt = 0;            

            //LOG(5, "Field Mask %4x  Seq %4x %4x \n", u2FldMaskCur, u4FldSeq, u4FldSeqNew);
        }        
    }       

    //Frame Mask matching                                                                                                           
    if (DiPar.PdPar.u4FrmBit)                                                                                                     
    {                                                                                                                         
        DiPar.PdPar.bFrmCB = (DiPar.PdPar.bFrmCB == (DiPar.PdPar.u4FrmBit-1)) ? 0 : (DiPar.PdPar.bFrmCB+1);                                   
                                                                                                                              
        u2Mask = FILM_MASK(DiPar.PdPar.u4FrmBit);                                                                             
        u2FrmMaskCur = (DiPar.PdPar.u4FrmMask>>(DiPar.PdPar.u4FrmBit-DiPar.PdPar.bFrmCB))&u2Mask;        
        u4FrmSeq = u4FrmSeq&u2Mask;
        
        if (DiPar.PdPar.bFrmCnt > 0x10) 
        {
            u4FrmSeqNew = u4FrmSeq|u2FrmMaskCur;
        }
        else
        {
            u4FrmSeqNew = u4FrmSeq;
        }
        
        if (u2FrmMaskCur == u4FrmSeqNew)   
        {    
            DiPar.PdPar.bFrmCnt = MIN(DiPar.PdPar.bFrmCnt+1, MDDI_FILM_MAX_SEQ_CNT); 
        }                                                                                                                     
        else                                                                                                                  
        {                                                                                                                     
            DiPar.PdPar.bFrmCB = 0;                                                                                               
            DiPar.PdPar.u4FrmBit = 0;  
            DiPar.PdPar.u4FrmMask = 0; 
            DiPar.PdPar.bFrmCnt = 0;            

            //LOG(5, "Frame Mask %4x  Seq %4x %4x \n", u2FrmMaskCur, u4FrmSeq, u4FrmSeqNew);   
        }                                                                                                                     
    }      

    //CS filed sequence match, now olny 22
    if ((((DiSta.CsSta.u4FieldSawRank+0x8080)&0x8888) == 0x0000) 
        || (((DiSta.CsSta.u4FieldSawRank+0x8080)&0x8888) == 0x8888))
    {
        DiPar.CsPar.bFilmCnt = (DiPar.CsPar.bFilmCnt) < 8 ? (DiPar.CsPar.bFilmCnt+1) : 8;
    }
    else
    {
        DiPar.CsPar.bFilmCnt = (DiPar.CsPar.bFilmCnt) > 0 ? (DiPar.CsPar.bFilmCnt-1) : 0;
    }

    //Special case for TG35 vertical moving pattern or random moving pattern
    //Field saw detection is wrong under this condition    
    if ((DiPar.PdPar.u4FldBit == 4 || DiPar.CsPar.bFilmCnt == 8) 
        && (DiSta.PeSta.u4EdgeSum == 0))
    {                
        DiPar.PdPar.bUnknowCnt = MIN(DiPar.PdPar.bUnknowCnt+0x8, 0x80); 
        //LOG(6, "Unknown OK:  %2x   %2x   %2x  \n", DiSta.PD.u4FldBit, DiSta.PD.bCurFilmMode, DiSta.BE[DiSta.u4IsrIndex].u4EdgeSum);   
    }
    else
    {
        DiPar.PdPar.bUnknowCnt = (DiPar.PdPar.bUnknowCnt) ? (DiPar.PdPar.bUnknowCnt-1) : 0;  
        //LOG(6, "Unknown OK:  %2x   %2x   %2x  \n", DiSta.PD.u4FldBit, DiSta.PD.bCurFilmMode, DiSta.BE[DiSta.u4IsrIndex].u4EdgeSum);   
    }

    //Search Field random mask
    if (DiPar.PdPar.u4FldBit == 0)                                                                                                
    {                                                                                                                         
        for (i=0; i < MASK_NUM; i++)                                                                                           
        {             
            u2Mask = FILM_MASK(u4MaskBit[i]);
            u4FldSeq = DiPar.PdPar.u4FldSeq;                                                                                        
            u2FldMaskCur = u4FldSeq&u2Mask;                                                                            
                                                                                                                              
            if ((u2FldMaskCur != 0) && (u2FldMaskCur != u2Mask) &&                                                      
                (u2FldMaskCur == ((u4FldSeq>>u4MaskBit[i])&u2Mask)))                                                   
            {                                                                                                                 
                for (j=0; j <= u4MaskBit[i]; j++)                                                                             
                {                                                                                                             
                    if (((u4FldSeq>>j) & 0x3) == 0x3)                                                                       
                    {                                                                                                         
                        DiPar.PdPar.bFwBadEdit = 1;                                                                              
                        break;                                                                                                
                    }                                                                                                         
                }
                
                DiPar.PdPar.u4FldMask = u4FldSeq&FILM_MASK(2*u4MaskBit[i]);
                DiPar.PdPar.u4FldBit = u4MaskBit[i];   
                break;                                                                                                        
            }                  
        }                                                                                                          
    } 

    //Search Frame random mask
    if (DiPar.PdPar.u4FrmBit == 0)                                                                                                
    {                                                                                                                         
        for (i=0; i < MASK_NUM; i++)                                                                                           
        {  
            u2Mask = FILM_MASK(u4MaskBit[i]);
            u4FrmSeq = DiPar.PdPar.u4FrmSeq;        
            u2FrmMaskCur = u4FrmSeq&u2Mask;                                                                            
                                                                                                                              
            if ((u2FrmMaskCur != 0) && (u2FrmMaskCur != u2Mask) &&                                                      
                (u2FrmMaskCur == ((u4FrmSeq>>u4MaskBit[i])&u2Mask)))                                                   
            {                                                                                                             
                DiPar.PdPar.u4FrmMask = u4FrmSeq&FILM_MASK(2*u4MaskBit[i]);
                DiPar.PdPar.u4FrmBit = u4MaskBit[i];                                                                              
                break;                                                                                                        
            }     
        }                                                                                                           
    }     

    if ((DiPar.PdPar.u4FldBit || DiPar.PdPar.u4FrmBit) && (DiPar.PdPar.bUnknowCnt == 0))
    {
        DiPar.PdPar.bFwAccCnt = MIN(DiPar.PdPar.bFwAccCnt+1, MDDI_FILM_ACC_MAX_SEQ_CNT);    
    }
    else
    {
        DiPar.PdPar.bFwAccCnt = (DiPar.PdPar.bFwAccCnt >= 0x8) ? (DiPar.PdPar.bFwAccCnt-2) : 0;                                 
    }                                  

    _vDrvDIMutliCadence();
}

static void _vDrvDIGetPQStatusISR(void)
{
    UINT32 i, u4Heter, u4Homo;

    DiSta.u4IsrIndex = (DiSta.u4IsrIndex+1)&DILOOPMASK;
    
    //============= MA Status ====================
    DiSta.IfSta.u4HEdgeCnt = IO32ReadFldAlign(MCVP_FUSION_19, IF_HOR_EDGE_CNT);
    DiSta.IfSta.u4VEdgeCnt = IO32ReadFldAlign(MCVP_FUSION_19, IF_VER_EDGE_CNT);
    DiSta.IfSta.u4EdgeSum = DiSta.IfSta.u4VEdgeCnt + DiSta.IfSta.u4HEdgeCnt;          
    DiSta.IfSta.u4AvgCurY = IO32ReadFldAlign(MCVP_FUSION_16, IF_AVG_Y);
    DiSta.IfSta.u4AvgCurCb = IO32ReadFldAlign(MCVP_FUSION_16, IF_AVG_CB);
    DiSta.IfSta.u4AvgCurCr = IO32ReadFldAlign(MCVP_FUSION_16, IF_AVG_CR);
    DiSta.IfSta.u4AvgPreY = IO32ReadFldAlign(MCVP_FUSION_17, IF_AVG_Y_PREV);
    DiSta.IfSta.u4AvgPreCb = IO32ReadFldAlign(MCVP_FUSION_17, IF_AVG_CB_PREV);
    DiSta.IfSta.u4AvgPreCr = IO32ReadFldAlign(MCVP_FUSION_17, IF_AVG_CR_PREV);
    DiSta.IfSta.u4HeterUp = IO32ReadFldAlign(MCVP_FUSION_12, IF_PD_SUM_HETER_UP);
    DiSta.IfSta.u4HomoUp = IO32ReadFldAlign(MCVP_FUSION_13, IF_PD_SUM_HOMO_UP);
    DiSta.IfSta.u4HeterDn = IO32ReadFldAlign(MCVP_FUSION_14, IF_PD_SUM_HETER_DN);
    DiSta.IfSta.u4HomoDn = IO32ReadFldAlign(MCVP_FUSION_15, IF_PD_SUM_HOMO_DN);
    DiSta.IfSta.u4SawInv = IO32ReadFldAlign(MCVP_FUSION_0B, IF_SIMPLE_SAW_INVERT);
    DiSta.IfSta.u4SawNom = IO32ReadFldAlign(MCVP_FUSION_0B, IF_SIMPLE_SAW_NORMAL);    
    
    u4Heter = DiSta.IfSta.u4HeterUp + DiSta.IfSta.u4HeterDn;
    u4Homo = DiSta.IfSta.u4HomoUp + DiSta.IfSta.u4HomoDn;
    DiSta.IfSta.u4HeterDiff = DIFF(u4Heter, DiSta.IfSta.u4Heter);
    DiSta.IfSta.u4HomoDiff = DIFF(u4Homo, DiSta.IfSta.u4Homo);
    DiSta.IfSta.u4Heter = u4Heter;
    DiSta.IfSta.u4Homo = u4Homo;    

    //============= BE Status ====================
    DiSta.PeSta.u4AliasCnt = IO32ReadFldAlign(MCVP_PE_12, ALIAS_CNT);
    DiSta.PeSta.u4EdgeSum = 0;
    DiSta.PeSta.u4EdgeHis[0] = u2IO32Read2B(MCVP_PE_0A);
    DiSta.PeSta.u4EdgeHis[1] = u2IO32Read2B(MCVP_PE_0A+2);
    
    for (i = 2; i < DI_BE_HIST_BIN; i++)
    {        
        DiSta.PeSta.u4EdgeHis[i] = u2IO32Read2B(MCVP_PE_0A+2*i);
        DiSta.PeSta.u4EdgeSum += DiSta.PeSta.u4EdgeHis[i];    
    }

    //============= FILM Status ====================
    DiSta.PdSta.u4FrmMotion = IO32ReadFldAlign(MCVP_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
    DiSta.PdSta.u4FldMotion = IO32ReadFldAlign(MCVP_STATUS_10, FD_32_SAW_FI_A_B_SAW_Q);
    DiSta.PdSta.u4EdgMotion = IO32ReadFldAlign(MCVP_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
    DiSta.PdSta.bBlkMotion = IO32ReadFldAlign(MCVP_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
    DiSta.PdSta.bCurFilmMode = IO32ReadFldAlign(MCVP_STATUS_0F, FILM_STATUS);
}

static void _vDrvDISetIFQualityISR(void)
{        
    UINT8 u1Weighting = (bGetSignalType(VDP_1) == SV_ST_TV) ? 6 : 8;
    UINT8 u1MaxMo_C;
    
    u1Weighting = DiPar.PdPar.bSmallMo ? 0 : u1Weighting;
    
    if ((((DiSta.PdSta.bCurFilmMode&E_DI_FILM_32) && (DiPQMode.b32Film)) 
        || ((DiSta.PdSta.bCurFilmMode&E_DI_FILM_22) && (DiPQMode.b22Film))) 
        && (DiPar.PdPar.bFilm24To50 == SV_FALSE)
        && (DiPar.PdPar.u2FilmMode <= E_DI_FILM_22)
        && (!(IS_PANEL_L12R12 && DiPar.IfPar.bCzpState)))
    {
        vIO32WriteFldAlign(MCVP_FUSION_23, DiPQMode.bFilmOnOff_Y ? u1Weighting : 0, IF_FILM_Y_W);
        vIO32WriteFldAlign(MCVP_FUSION_23, DiPQMode.bFilmOnOff_C ? u1Weighting : 0, IF_FILM_C_W);
    }    
    else
    {
        vIO32WriteFldAlign(MCVP_FUSION_23, 0, IF_FILM_Y_W);
        vIO32WriteFldAlign(MCVP_FUSION_23, 0, IF_FILM_C_W); 
    }    
        vIO32WriteFldAlign(MCVP_FUSION_21, 8, IF_MAX_MOTION_Y);

    u1MaxMo_C = (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_00, IF_C_AD_EN) && IS_SD_TIMING(VDP_1)) 
                    ? (8-(DiPar.CsPar.bFavorCS>>1)) : 8;

    if ((bGetSignalTypeNew(VDP_1) == SV_ST_AV) && (bHwTvdMode() == SV_CS_PAL))
    {
        u1MaxMo_C = MIN(DiPar.IfPar.bMaxMoC, u1MaxMo_C);
    }
    
    vIO32WriteFldAlign(MCVP_FUSION_21, u1MaxMo_C, IF_MAX_MOTION_C);        

    // for RF no signal
    if (((bGetSignalType(VDP_1) == SV_ST_TV) 
        && (bDrvVideoSignalStatus(VDP_1) == SV_VDO_NOSIGNAL))
        // for RF & CVBS weak signal
		|| ((bGetVideoDecTypeNew(VDP_1) == SV_VD_TVD3D) 
        && (bTvdSignalStrengthLevel(1) < CS_DISABLE_WEAK_SIGNAL))
        // for HQV 2.0 SD 
        || (IS_SD_TIMING(VDP_1) && (u1DrvGetASL() < 0x10) &&
            ((DiSta.PeSta.u4EdgeSum > 0x280) && (DiSta.PeSta.u4EdgeSum < 0x3C0)) &&
            ((DiSta.IfSta.u4HEdgeCnt>>2) > DiSta.IfSta.u4VEdgeCnt))
        // for Level change flicker issue    
        || (DiPar.IfPar.bFroce3DDealy > 0))
    {
        _vDrvSetCsOnOff(VDP_1, SV_OFF);
    }
    else
    {
        _vDrvSetCsOnOff(VDP_1, DiPar.CsPar.bCsOnOff&&(!DiPar.bForceSceneChange));
    }       

    if (DiPar.IfPar.bFroce3DDealy > 0)
    {
        DiPar.IfPar.bFroce3DDealy--;
        
        vIO32WriteFldMulti(MCVP_FUSION_20, P_Fld(1, IF_FIX_MOTION_Y) | P_Fld(0, IF_FORCE_MOTION_Y));
        
        if (IS_DI_Y4C4(VDP_1))
        {
            vIO32WriteFldMulti(MCVP_FUSION_20, P_Fld(1, IF_FIX_MOTION_C) | P_Fld(0, IF_FORCE_MOTION_C));
        }
    }
    else
    {
        //Weave field 
        if (IO32ReadFldAlign(PSCAN_FWFILM_06, FILED_WEAVE_EN)
            && (((u1DI_Is50HZ == SV_TRUE)&&((DiPar.PdPar.u2FilmMode == E_DI_FILM_44) || (DiPar.PdPar.u2FilmMode == E_DI_FILM_66)))||
                ((u1DI_Is50HZ == SV_FALSE)&&((DiPar.PdPar.u2FilmMode != E_DI_FILM_44) 
                   && (DiPar.PdPar.u2FilmMode != E_DI_FILM_66)&&(DiPar.PdPar.u2FilmMode > E_DI_FILM_22)))||
                (DiPar.PdPar.bFilm24To50) || 
                (((DiPar.PdPar.u2FilmMode == E_DI_FILM_22)||((DiPar.PdPar.u2FilmMode == E_DI_FILM_32)&&(u1DI_Is50HZ == SV_FALSE)))
                  &&(E_TD_OUT_3D_FS==_arMDDiPrm[VDP_1].eTDOutput))) 
            && (DiPar.PdPar.bFwBadEdit == 0)
        	&& (DiPQMode.bFilmOnOff_MultiCadence == SV_ON)
        	&& (_arMDDiPrm[VDP_1].u1IsDualFPI == SV_OFF))
        {
            DiPar.PdPar.bFwWeave = 1;
            vIO32WriteFldMulti(MCVP_FUSION_20, 
                        P_Fld(1, IF_FIX_MOTION_Y) | 
                        P_Fld(0, IF_FORCE_MOTION_Y) | 
                        P_Fld(1, IF_FIX_B_WEIGHT_Y) | 
                        P_Fld((DiPar.PdPar.u4FldSeq&1) ? 0 : 8, IF_FORCE_B_WEIGHT_Y)); 

            // Weave field need motion level = 0 and MV = 0 
            vIO32WriteFldAlign(MCVP_CS_17, 1, FORCE_MV_EN);   
            
            if (IS_DI_Y4C4(VDP_1))
            {
                vIO32WriteFldMulti(MCVP_FUSION_20,
                            P_Fld(1, IF_FIX_MOTION_C) | 
                            P_Fld(0, IF_FORCE_MOTION_C) | 
                            P_Fld(1, IF_FIX_B_WEIGHT_C) | 
                            P_Fld((DiPar.PdPar.u4FldSeq&1) ? 0 : 8, IF_FORCE_B_WEIGHT_C));
            }
        }
        else      
        {
            DiPar.PdPar.bFwWeave = 0;
        }
    }
    
    vIO32Write4B(PSCAN_FW_ADAPTIVE_FILM_0A, DiPar.PdPar.u4FldSeq);
    vIO32Write4B(PSCAN_FW_ADAPTIVE_FILM_0B, DiPar.PdPar.u4FrmSeq);
}

static void _vDrvDISetIFQualityISR_Ex(void)
{        
    UINT32 u4IsFilm = (DiSta.PdSta.bCurFilmMode&0x3) ? 1 : 0;

    if (DiPar.IfPar.bFroce3DDealy == 0)
    {
        // Turn off weave and use fusion result to fix multicadence top line problem. 
        vIO32WriteFldMulti(MCVP_FUSION_20, 
            P_Fld(0, IF_FIX_MOTION_Y)  | 
            P_Fld(4, IF_FORCE_B_WEIGHT_Y) | 
            P_Fld(!u4IsFilm, IF_FIX_B_WEIGHT_Y));    
        
        if (IS_DI_Y4C4(VDP_1))
        {        
            vIO32WriteFldMulti(MCVP_FUSION_20, 
                P_Fld(0, IF_FIX_MOTION_C) | 
                P_Fld(4, IF_FORCE_B_WEIGHT_C) | 
                P_Fld(!u4IsFilm, IF_FIX_B_WEIGHT_C));       
        }        
    
        vIO32WriteFldAlign(MCVP_CS_17, 0, FORCE_MV_EN); 
    }
}

static void _vDrvDISetPEQualityISR(void)
{
    
}

static void _vDrvDISetCSQualityISR(void)
{
    //FORCE MV = 0 TO PREVENT MV Remains
    vIO32WriteFldAlign(MCVP_CS_21, 
        ((DiSta.IfSta.u4Homo == 0) && (DiSta.IfSta.u4HomoDiff == 0)) ? SV_ON : SV_OFF, FORCE_ME_IN_EN);
}

static void _vDrvDISetDARQualityISR(void)
{
        //Hw Film mode turn off Dare 
    if (((DiPQMode.bFilmOnOff_Y == SV_ON)
        && (((DiSta.PdSta.bCurFilmMode&E_DI_FILM_32) && (DiPQMode.b32Film)) 
        || ((DiSta.PdSta.bCurFilmMode&E_DI_FILM_22) && (DiPQMode.b22Film))))
        //MultiCadense Film mode turn off Dare 
        || ((DiPQMode.bFilmOnOff_MultiCadence == SV_ON) 
        && ((DiPar.PdPar.u2FilmMode > E_DI_FILM_22) 
        && (DiPar.PdPar.u2FilmMode < E_DI_FILM_KALEIDOSCOPE))))  
    {       
        //Motion level should be 0 when film mode 
        vIO32WriteFldAlign(MCVP_DARE_00, 0, DARE_LINE_FLICKER_EN);                

        if (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, DAR_OFF_IN_FILM))
        {
            vIO32WriteFldMulti(MCVP_DARE_00, 
                P_Fld(0, DARE_VD_SMOOTH_EN) | 
                P_Fld(0, DARE_VS_SMOOTH_EN) | 
                P_Fld(0, DARE_CONT_SAW_EN) | 
                P_Fld(0, DARE_SINGLE_SAW_EN) | 
                P_Fld(0, DARE_LUMA_JAGGY_EN));   
        }
    }
    else
    {        
        vIO32WriteFldAlign(MCVP_DARE_00, (DiPar.DarPar.u4DarCfg&0x000100) ? 1 : 0, DARE_LINE_FLICKER_EN);  
        
        if (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, DAR_OFF_IN_FILM))
        {
            vIO32WriteFldMulti(MCVP_DARE_00, 
                P_Fld((DiPar.DarPar.u4DarCfg&0x000020) ? 1 : 0, DARE_VD_SMOOTH_EN) | 
                P_Fld((DiPar.DarPar.u4DarCfg&0x000010) ? 1 : 0, DARE_VS_SMOOTH_EN) | 
                P_Fld((DiPar.DarPar.u4DarCfg&0x100000) ? 1 : 0, DARE_CONT_SAW_EN) | 
                P_Fld((DiPar.DarPar.u4DarCfg&0x010000) ? 1 : 0, DARE_SINGLE_SAW_EN) | 
                P_Fld((DiPar.DarPar.u4DarCfg&0x001000) ? 1 : 0, DARE_LUMA_JAGGY_EN));
        }                
    }
}

static void _vDrvDISetPDQualityISR(void)
{
    _vDrvDIPDSetAdapThld();
    _vDrvDIPDFwPatch();
    _vDrvDIPDFwFilmDetetion();
}
    
void _vDrvDIFieldInverse(void)
{
    static UINT32 u4Count = 0;    
    UINT32 u4StaSawInv, u4StaSawNom;        
    UINT32 u4Max = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_01, FLD_INV_MAX_THD);
    
    if (DiPar.IfPar.bFldInv == 0)
    {        
        u4StaSawInv = DiSta.IfSta.u4SawInv;
        u4StaSawNom = DiSta.IfSta.u4SawNom;    
    }
    else
    {        
        u4StaSawInv = DiSta.IfSta.u4SawNom;  
        u4StaSawNom = DiSta.IfSta.u4SawInv;  
    }
      
    if ((u4StaSawInv < 0x4f) && (u4StaSawNom > 0x4f)
        && ((u4StaSawNom*2) > (u4StaSawInv*5)))
    {
        u4Count = (u4Count < u4Max) ? (u4Count + 1) : u4Max;
    }
    else
    {
        u4Count = (u4Count > 0) ? (u4Count - 1) : 0;
    }

    if (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_00, FLD_INV_EN))
    {
        if (u4Count > IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_01, FLD_INV_IN_THD))
        {
            DiPar.IfPar.bFldInv = 1;
        }        
        else if (u4Count < IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_01, FLD_INV_OUT_THD))
        {
            DiPar.IfPar.bFldInv = 0;
        }

        //LOG(5, "u4Count %d  =>  %d \n", u4Count, DiPar.IfPar.bFldInv);
    }
    else 
    {
        DiPar.IfPar.bFldInv = 0;
    } 
    
    vDrvDISetFieldInverse(VDP_1, DiPar.IfPar.bFldInv);    
}

static void _vDrvDICzpAdaptive(void)
{
    UINT32 u4State;    
    UINT32 u4HorSm, u4VerSm, u4Base3D;
    UINT32 u4HorCostEN, u4HorCostTH, u4Peak;    
    UINT32 u4Base3DTemp1, u4Base3DTemp2;
    static UINT32 u4Count = 0;        
    UINT32 u4Min = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_01, CZP_AD_MIN_THD);
    
    if ((DiSta.CsSta.u4MultibusrtCnt > 0x1800)
        && (DiSta.IfSta.u4SawNom < 0x4f)
        && (DiSta.IfSta.u4SawNom > 0x28)
        && (DiSta.IfSta.u4SawInv > 0x4f))
    {
        u4Count = (u4Count < 15) ? (u4Count + 1) : 15;
    }
    else
    {
        u4Count = (u4Count > 0) ? (u4Count - 1) : 0;
    }

    u4State = (u4Count < u4Min) ? 0 : ((u4Count > (u4Min + 8)) ? 8 : (u4Count - u4Min));

    // Still CZP vertical horazontal line
    if (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_00, CZP_AD_EN) == 0)
    {
        u4State = 0;
    }
    DiPar.IfPar.bCzpState = (UINT8)u4State;
    
    u4HorSm = (0x3 * ( 8 - u4State ) + 17 * u4State +4) >> 3;
    u4VerSm = (0x14 * ( 8 - u4State ) + 82 * u4State + 4) >> 3;
    u4Base3D = (0x8 * ( 8 - u4State ) + 91 * u4State + 4) >> 3;
    u4HorCostEN = (u4State > 0 ) ? 1 : 0;
    u4HorCostTH = (0 * ( 8 - u4State ) + 63 * u4State + 4) >> 3;
    u4Peak = ( 0x4 * ( 8 - u4State ) + 0 * u4State + 4) >> 3;

    if (bGetVideoDecTypeNew(VDP_1) == SV_VD_TVD3D) 
    {
        UINT32 u4NoiseLevel = IO32ReadFldAlign(NR_NMSTA_0E, NR_NM_STA_NL);

        u4Base3DTemp1 = (u4NoiseLevel >= 32) ? (2) : 
                       (u4NoiseLevel <= 8) ? (8) : 
                       (((192 - 6 * u4NoiseLevel + 12) / 24) + 2);
        u4Base3D = MIN(u4Base3D, u4Base3DTemp1);
    }

    if (IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_EN))
    {
        u4Base3DTemp2 = (u4Base3D > (DiPar.CsPar.bFavorCS>>2)) ? (u4Base3D - (DiPar.CsPar.bFavorCS>>2)) : 0;
        u4Base3D = MIN(u4Base3D, u4Base3DTemp2);
    }
    
    if ((DiPar.CsPar.bFavorCS >= 0x4) && (bGetSignalTypeNew(VDP_1) == SV_ST_AV))
    {
        if (bHwTvdMode() == SV_CS_NTSC358)
        {
            u4Base3D = 0xC - DiPar.CsPar.bFavorCS;
        }
        else if (bHwTvdMode() == SV_CS_PAL)
        {
            u4Base3D = 0x8 - DiPar.CsPar.bFavorCS;
        }
    }

    vIO32WriteFldAlign(MCVP_FUSION_09, u4HorSm, IF_HOR_SMALL_VAR_TH);
    vIO32WriteFldAlign(MCVP_FUSION_09, u4VerSm, IF_VER_SMALL_VAR_TH);
    vIO32WriteFldAlign(MCVP_FUSION_03, u4Base3D, IF_BASE_COST_3D);
    vIO32WriteFldAlign(MCVP_FUSION_01, u4HorCostEN, IF_HOR_EDGE_COST_ADD);
    vIO32WriteFldAlign(MCVP_FUSION_02, u4HorCostTH, IF_HOR_EDGE_COST_TH);
    vIO32WriteFldAlign(MCVP_FUSION_07, u4Peak, IF_RA_PEAK_MULT);

    // Moving CZP using bob mode 
    if (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_00, MOVING_CZP_BOB_EN)
        && (DiSta.PeSta.u4AliasCnt > 0x1000)
        && ((DiSta.CsSta.u1GmvX != 0) || (DiSta.CsSta.u1GmvY != 0)))
    {
        DiPar.IfPar.bMovingCzp = 1;
        vIO32WriteFldAlign(MCVP_FUSION_21, 8, IF_MIN_MOTION_C);         
        vIO32WriteFldAlign(MCVP_FUSION_21, 8, IF_MIN_MOTION_Y);  
        vIO32WriteFldAlign(MCVP_PE_00, 1, PE_FORCE_USER_DIR);  
    }
    else
    {
        DiPar.IfPar.bMovingCzp = 0;
        vIO32WriteFldAlign(MCVP_FUSION_21, 0, IF_MIN_MOTION_C);         
        vIO32WriteFldAlign(MCVP_FUSION_21, 0, IF_MIN_MOTION_Y);  
        vIO32WriteFldAlign(MCVP_PE_00, 0, PE_FORCE_USER_DIR);  
    }
}

static void _vDrvDIChromaJaggyDetection(void)
{
    UINT32 u4CJaggyConf, u4CJaggyOFST;
    UINT32 u4Update = SV_FALSE;
    static UINT32 u4PreCJaggyConf = 0;
    static UINT32 u4PreCJaggyOFST = 0;
    static UINT32 u4UpdateCnt = 0;

    if (IO32ReadFldAlign(VDP_SCPQ_04, SCPQ_C_JAGGY_EN))
    {
        u4CJaggyConf =  IO32ReadFldAlign(VDP_SCPQ_04, SCPQ_C_JAGGY_FIX_EN) ? 
                                        IO32ReadFldAlign(VDP_SCPQ_04, SCPQ_C_JAGGY_RATIO) :
                                        IO32ReadFldAlign(MCVP_FUSION_2A, IF_C_F_FOUND);

        u4PreCJaggyConf = (u4PreCJaggyConf * 63 + (u4CJaggyConf << 10) + 32) /64;
        u4CJaggyConf = (u4PreCJaggyConf + 512) >> 10;
        
        if (u4UpdateCnt % IO32ReadFldAlign(VDP_SCPQ_05, SCPQ_C_JAGGY_UPDATE_CNT) == 0)
        {
            for (u4CJaggyOFST = 0; u4CJaggyOFST < SCPQ_TH_TOTAL; u4CJaggyOFST++)
            {
                if (u4CJaggyConf < u1CJaggyTh[u4CJaggyOFST])
                    break;
            }
        
            if (u4CJaggyOFST > u4PreCJaggyOFST)
            {
                u4PreCJaggyOFST++;
                u4Update = SV_TRUE;
            }
            else if (u4CJaggyOFST < u4PreCJaggyOFST)
            {
                u4PreCJaggyOFST--;
                u4Update = SV_TRUE;
            }
        
            vIO32WriteFldAlign(VDP_SCPQ_03, u4PreCJaggyOFST, SCPQ_CUST_VC_OFST);
            vIO32WriteFldAlign(VDP_SCPQ_03, 0, SCPQ_CUST_VY_OFST);
        
            if (u4Update == SV_TRUE)
            {
                LOG(3, "u4PreCJaggyOFST = %d\n", u4PreCJaggyOFST);
                vApiRegisterVideoEvent(PE_EVENT_SCALER_V, SV_VP_MAIN, SV_ON);
            }
        }
    }

    u4UpdateCnt++;
}

static void _vDrvDIDTVTearDetection(void)
{
    UINT32 u4DTVTearConf, u4DTVTearOFST;
    UINT32 u4MoTh, u4MoThOrder, u4MoPenGain;
    UINT32 u4FakeHDHor, u4FakeHDGain;
    UINT32 u4Update = SV_FALSE;
    static UINT32 u4PreDTVTearConf = 0;
    static UINT32 u4PreDTVTearOFST = 0;
    static UINT32 u4UpdateCnt = 0;

    u4MoThOrder = IO32ReadFldAlign(VDP_SCPQ_05, SCPQ_MOTION_TH_ORDER);
    u4MoTh = 2 << u4MoThOrder;
    u4MoPenGain = 0x80;

    if (IO32ReadFldAlign(VDP_SCPQ_04, SCPQ_DTV_TEAR_EN))
    {
        u4DTVTearConf =  IO32ReadFldAlign(VDP_SCPQ_04, SCPQ_DTV_TEAR_FIX_EN) ?
                                            IO32ReadFldAlign(VDP_SCPQ_04, SCPQ_DTV_TEAR_RATIO) :
                                            IO32ReadFldAlign(MCVP_FUSION_27, DTV_TEARING_FOUND);

        if (IO32ReadFldAlign(VDP_SCPQ_04, SCPQ_DTV_TEAR_MO_PENALTY))
        {
            if (DiSta.IfSta.u4HomoAvg < u4MoTh)
            {
                u4MoPenGain = (0x80 * DiSta.IfSta.u4HomoAvg) >> u4MoThOrder;
                u4DTVTearConf = (u4DTVTearConf * u4MoPenGain + 64) / 128;
            }
        }

        if (IO32ReadFldAlign(VDP_SCPQ_04, SCPQ_DTV_TEAR_REF_FAKEHD))
        {
            u4FakeHDHor = IO32ReadFldAlign(MCVP_FUSION_2B, IF_HD_HOR_HI_F_SUM);

            if (u4FakeHDHor > 0x80)
            {
                u4FakeHDGain = 0xFF - u4FakeHDHor;
                u4DTVTearConf = (u4DTVTearConf * u4FakeHDGain + 64) / 128;
            }
        }

        u4PreDTVTearConf = (u4PreDTVTearConf * 63 + (u4DTVTearConf << 10) + 32) / 64;
        u4DTVTearConf = (u4PreDTVTearConf + 512) >> 10;
        
        if (u4UpdateCnt % IO32ReadFldAlign(VDP_SCPQ_05, SCPQ_DTV_TEAR_UPDATE_CNT) == 0)
        {
            for (u4DTVTearOFST = 0; u4DTVTearOFST < SCPQ_TH_TOTAL; u4DTVTearOFST++)
            {
                if (u4DTVTearConf < u1DTVTearTh[u4DTVTearOFST])
                    break;
            }
        
            if (u4DTVTearOFST > u4PreDTVTearOFST)
            {
                u4PreDTVTearOFST++;
                u4Update = SV_TRUE;
            }
            else if (u4DTVTearOFST < u4PreDTVTearOFST)
            {
                u4PreDTVTearOFST--;
                u4Update = SV_TRUE;
            }
        
            vIO32WriteFldAlign(VDP_SCPQ_03, u4PreDTVTearOFST, SCPQ_CUST_VC_OFST);
            vIO32WriteFldAlign(VDP_SCPQ_03, u4PreDTVTearOFST, SCPQ_CUST_VY_OFST);
        
            if (u4Update == SV_TRUE)
            {
                LOG(3, "u4PreDTVTearOFST = %d\n", u4PreDTVTearOFST);
                vApiRegisterVideoEvent(PE_EVENT_SCALER_V, SV_VP_MAIN, SV_ON);
            }
        }
    }

    u4UpdateCnt++;
}

static void _vDrvDISawToothStillPatch(void)
{

    static UINT32 u4Count = 0;
    static UINT32 u4Conf = 0;
    UINT8 u1sawtoothst;

    if ((DiSta.IfSta.u4VEdgeCnt > 0x800)
        && (DiSta.IfSta.u4HEdgeCnt  < 0x180))
    {
        u4Count = (u4Count < 160) ? (u4Count + 2) : 160;
    }
    else
    {
        u4Count = (u4Count > 0) ? (u4Count - 1) : 0;
    }
    
    vIO32WriteFldAlign(PSCAN_FWCS_04, u4Count, SAW_STILL_CNT);

    u4Conf = (u4Count > 10) ? (((u4Count - 10) > 32) ? 32 : (u4Count - 10)) : 0;

    u1sawtoothst = ((u4Conf/4) > 15)? 15 : (u4Conf/4);
    vIO32WriteFldAlign(PSCAN_FWCS_04, u1sawtoothst, SAW_STILL_UPDATE);

}

static void _vDrvDIFavorCS(void)
{
    static UINT16 _u2NonZeroCnt[5] = {0, 0, 0, 0, 0};
    static UINT32 u4Index = 0;
    static UINT32  u4Count = 0;
    static UINT8 _u1FavorState =0;
    UINT32 u4Cost, u4Stable = 0;
    UINT32 u4sawtooth_mc = 8, u4blendratio = 1, u4sawtooth_c = 2;
    UINT32 u4CostTemp, u4SmoothStill;
    UINT8 u1sawtoothst_update;

    UINT32 u4HistThd = ((UINT32)DiSta.CsSta.u2HistCnt*IO32ReadFldAlign(PSCAN_FWCS_03, HIST_DIFF_RATIO))>>8;
    UINT32 u4Max = IO32ReadFldAlign(PSCAN_FWCS_03, COUNT_MAX_BND);
    UINT32 u4ExeThd = IO32ReadFldAlign(PSCAN_FWCS_03, COUNT_EXE_THD);
    UINT32 u4NoiseLevel = IO32ReadFldAlign(NR_NMSTA_0E, NR_NM_STA_NL);    
    UINT32 u4TimingType = bDrvVideoGetTiming(VDP_1);

    _u2NonZeroCnt[u4Index] = DiSta.CsSta.u2NonZeroCnt;

    u4Stable |= ((_u2NonZeroCnt[0] == _u2NonZeroCnt[1])
                    && (_u2NonZeroCnt[0] == _u2NonZeroCnt[2])
                    && (_u2NonZeroCnt[0] == _u2NonZeroCnt[3])) ? 1 : 0;

    u4Stable |= ((_u2NonZeroCnt[1] == _u2NonZeroCnt[2])
                    && (_u2NonZeroCnt[1] == _u2NonZeroCnt[3])
                    && (_u2NonZeroCnt[1] == _u2NonZeroCnt[4])) ? 1 : 0;

    u4Stable |= ((_u2NonZeroCnt[2] == _u2NonZeroCnt[0])
                    && (_u2NonZeroCnt[2] == _u2NonZeroCnt[3])
                    && (_u2NonZeroCnt[2] == _u2NonZeroCnt[4])) ? 1 : 0;

    u4Stable |= ((_u2NonZeroCnt[3] == _u2NonZeroCnt[0])
                    && (_u2NonZeroCnt[3] == _u2NonZeroCnt[1])
                    && (_u2NonZeroCnt[3] == _u2NonZeroCnt[4])) ? 1 : 0;

    u4Stable |= ((_u2NonZeroCnt[4] == _u2NonZeroCnt[0])
                    && (_u2NonZeroCnt[4] == _u2NonZeroCnt[1])
                    && (_u2NonZeroCnt[4] == _u2NonZeroCnt[2])) ? 1 : 0;

    // LG power tower patch
    if (DiSta.CsSta.u1GmvValid && (DiSta.CsSta.u1GmvX == 0) && (DiSta.CsSta.u1GmvY != 0))
    {
        u4Stable = 0;
    }
    
    if (((u4Stable && !(DiSta.CsSta.u1GmvValid && (DiSta.CsSta.u1GmvX == 0) && (DiSta.CsSta.u1GmvY == 0)))|| (DiSta.CsSta.u1GmvValid && (DiSta.CsSta.u1GmvX != 0)))
        && (DiSta.CsSta.u4HistDiffAcc < u4HistThd) 
        && (bDrvGetFilmStatus() == E_DI_FILM_NONE)
        && ((_u2NonZeroCnt[u4Index] > 1) || DiSta.CsSta.u1GmvValid)
        && (_u2NonZeroCnt[u4Index] < 13))
    {
        u4Count = (u4Count < u4Max) ? (u4Count + 1) : u4Max;
    }
    else
    {
        u4Count = (u4Count > 0) ? (u4Count - 1) : 0;
    }
    
    DiPar.CsPar.bFavorCS = (u4Count > u4ExeThd) ? (((u4Count - u4ExeThd) > 8) ? 8 : (u4Count - u4ExeThd)) : 0;
    vIO32WriteFldAlign(PSCAN_FWCS_02, DiPar.CsPar.bFavorCS, FAVOR_CS_STATE);
    u4Cost = MAX(MIN((9-DiPar.CsPar.bFavorCS), 8), 1);
    u4SmoothStill = 4 - MIN(2, (DiPar.CsPar.bFavorCS)>>2);

    if (bGetVideoDecTypeNew(VDP_1) == SV_VD_TVD3D) 
    {
        u4CostTemp = (u4NoiseLevel >= 24) ? (0) : 
                     (u4NoiseLevel <= 8) ? (8) : 
                     (((192 - 8 * u4NoiseLevel + 8) / 16) + 0);             

        u4Cost = MIN(u4Cost, u4CostTemp);
    }

    if (bGetSignalType(VDP_1) == SV_ST_TV)
    {  
        u4sawtooth_mc  = (u4NoiseLevel >= 24) ? (0) :  (u4NoiseLevel <= 8) ? (8) : 
                     (((192 - 8 * u4NoiseLevel + 8) / 16) + 0); 
        u4blendratio = (u4NoiseLevel >= 24) ? (4) : (u4NoiseLevel <= 16) ? (1) : 
                     ( ((u4NoiseLevel-16)*3/8)+ 1); 
    }
    else
    {
        u4sawtooth_mc = 8;
        u4blendratio  = 1;
    }
	//h.264 576i rectangle flash 
	if(IS_MPEG(VDP_1)&& (VDP_GetPlayMode(VDP_1) == FBM_FBG_DTV_MODE)&& (u4TimingType== MODE_625I))
	{
		 MDDI_WRITE_FLD(VDP_1,MCVP_FUSION_0D, 0x20,IF_RA_CLIP_FLICKER_TH);
		//LOG(1," %d %d %d %d %d\n",(DiSta.CsSta.u1GmvValid),DiSta.CsSta.u1GmvX,DiSta.CsSta.u4MultibusrtCnt,DiSta.CsSta.u2NonZeroCnt,DiSta.IfSta.u4SawInv);
		 if((!DiSta.CsSta.u1GmvValid) && 
			((DiSta.CsSta.u1GmvX == 0x1E)||(DiSta.CsSta.u1GmvX == 0x1F))&&(DiSta.CsSta.u1GmvY== 0)&&
			(DiSta.CsSta.u4MultibusrtCnt<=0xFF)&&
			(DiSta.CsSta.u2NonZeroCnt<=10)
			&&(DiSta.IfSta.u4SawInv<=120))
		{
			MDDI_WRITE_FLD(VDP_1,MCVP_FUSION_01,0x10,IF_MIN_COST_2D);
			MDDI_WRITE_FLD(VDP_1,MCVP_FUSION_01,0x0D,IF_BASE_COST_2D);
			vIO32WriteFldAlign(PSCAN_FWCS_03,0xA0,HIST_DIFF_RATIO);
			u4Cost = MIN(1, u4Cost);
			u4SmoothStill =MIN(0, u4SmoothStill);
		}
	}

    //CZP more 3D
    if (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_00, CZP_3D_EN)
        &&((u4TimingType == MODE_525I) || (u4TimingType == MODE_525I_OVERSAMPLE))
        && (DiSta.PeSta.u4AliasCnt > IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_01, CZP_3D_TH))
        && (DiSta.CsSta.u1GmvX == 0) &&  (DiSta.CsSta.u1GmvY == 0))
    {
        u4Cost = MIN(1, u4Cost);
        u4SmoothStill =MIN(0, u4SmoothStill);
    }

    MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_05, u4blendratio, IF_BLEND_RATIO_Y);
    
    if((bGetSignalType(VDP_1) == SV_ST_TV) && (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_00, SAW_STILL_PATCH_EN)))
    {
        _vDrvDISawToothStillPatch();

        if(((IO32ReadFldAlign(PSCAN_FWCS_04, SAW_STILL_UPDATE)) + u4Cost)>=15)
            u1sawtoothst_update = 15;
        else
            u1sawtoothst_update = ((IO32ReadFldAlign(PSCAN_FWCS_04, SAW_STILL_UPDATE)) + u4Cost);
    }  
    else
    {
        u1sawtoothst_update = u4Cost;
    }

	if(_fgCornPattern)
	{
		u1sawtoothst_update = 0x5;
	}
    if (IO32ReadFldAlign(PSCAN_FWCS_02, FAVOR_CS_EN))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, MIN(8-DiPar.CsPar.bFavorCS,u4sawtooth_mc), IF_W_SAWTOOTH_MC);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, MIN((8-DiPar.CsPar.bFavorCS)>>2,u4sawtooth_c), IF_W_SAWTOOTH_C);
        //MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, u4Cost, IF_W_SAWTOOTH_STILL);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, u1sawtoothst_update, IF_W_SAWTOOTH_STILL);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, u4SmoothStill, IF_W_SMOOTH_STILL);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_03, (8-DiPar.CsPar.bFavorCS)/2, IF_W_MC);        
      _u1FavorState =1; 
    }
   else if (_u1FavorState ==1)
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, MIN(8,u4sawtooth_mc), IF_W_SAWTOOTH_MC);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, u4sawtooth_c, IF_W_SAWTOOTH_C);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_03, 4, IF_W_MC);
        MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, u1sawtooth_still, IF_W_SAWTOOTH_STILL);
        _u1FavorState =0;
    } 
    
    u4Index = (u4Index < 4) ? (u4Index+1) : 0;
}

static void _vDrvDIStillMove(void)
{

    static UINT32 u4Count = 0;
    static UINT32 u4Conf = 0;
    UINT8 u1sawtoothst_update;
    UINT8 u1smoothst_update;
    UINT8 u1APL = IO32ReadFldAlign(METER_INFO_00, METER_APL_VAL);
    UINT8 u1ASL = IO32ReadFldAlign(METER_INFO_00, METER_ASL_VAL);    
    if ((DiSta.CsSta.u4MultibusrtCnt > 600)
        && (DiSta.CsSta.u2EdgeCnt > 4500)
        && (DiSta.IfSta.u4SawNom > 0xB8)
        && (DiSta.IfSta.u4SawInv > 0xF8)
        && (u1APL > 0x60)
        && (u1ASL > 0x10))
    {
        u4Count = (u4Count < 160) ? (u4Count + 1) : 160;
    }
    else
    {
        u4Count = (u4Count > 0) ? (u4Count - 1) : 0;
    }
    if (IO32ReadFldAlign(PSCAN_FWCS_05, STILL_MOV_DEBUG))
    {
        u4Count = IO32ReadFldAlign(PSCAN_FWCS_05, STILL_MOVE_CNT);
    }
    else
    {
        vIO32WriteFldAlign(PSCAN_FWCS_05, u4Count, STILL_MOVE_CNT);
    }
        u4Conf = (u4Count > 20) ? (((u4Count - 20) > 64) ? 64 : (u4Count - 20)) : 0;
        vIO32WriteFldAlign(PSCAN_FWCS_05, u4Conf, STILL_MOVE_CONF);
    if (u4Count > 20)
    {
        u1sawtoothst_update = ((u4Conf/8) > 4)? 0 : (4 - u4Conf/8);
        u1smoothst_update = ((u4Conf/8) > 4)? 0 : (4 - u4Conf/8);
        if (IO32ReadFldAlign(PSCAN_FWCS_05, STILL_MOV_EN))
        {
            MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, u1sawtoothst_update, IF_W_SAWTOOTH_STILL);
            MDDI_WRITE_FLD(VDP_1, MCVP_FUSION_04, u1smoothst_update, IF_W_SMOOTH_STILL);
        }
    }
}
static void _vDrvIFAdaptive(UINT8 bPath)
{
    if (IS_SD_TIMING(bPath) && (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF))
    {
        _vDrvDIFieldInverse();
        _vDrvDICzpAdaptive();          
    }
    if(!IS_SD_HEIGHT(bPath) && (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF))
    {
        _vDrvDIStillMove();
    }
}

static void _vDrvPEAdaptive(UINT8 bPath)
{

}

static void _vDrvDARAdaptive(UINT8 bPath)
{
    INT32 i4MoPxlLevel, i4MoSumLevel;
    UINT32 u4Min, u4Max; 
    static UINT8 u1MotionIdx, u1TargetValue; 
    static UINT16 u2PreAlpha;
    static UINT32 u4MoPxl[5], u4MoSum[5];

    UINT8 u1MaxStrMin = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, DAR_V_MAX_MIN);  
    UINT8 u1MaxStrMax = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, DAR_V_MAX_MAX);  
    UINT16 u2MoPxlCoring = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, DAR_V_MOTION_PXL_CORING);  
    UINT16 u2MoSumCoring = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, DAR_V_MOTION_SUM_CORING);  
    UINT8 u1MoPxlShift = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, DAR_V_MOTION_PXL_SHIFT);  
    UINT8 u1MoSumShift = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, DAR_V_MOTION_SUM_SHIFT);  
    UINT8 u1FilterStrDelay = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, DAR_V_FILTER_STR_DELAY_SHIFT);
    UINT32 u4NoiseLevel = IO32ReadFldAlign(NR_NMSTA_0E, NR_NM_STA_NL);    

    u1MotionIdx = (u1MotionIdx < 4) ? (u1MotionIdx + 1) : 0;
    u4MoPxl[u1MotionIdx] = DiSta.PdSta.u4FrmMotion;
    u4MoSum[u1MotionIdx] = DiSta.IfSta.u4Homo;
               
    //Filter Level Decide by Motion Pixel Number
    u4Min = MIN(u4MoPxl[0], MIN(u4MoPxl[1], MIN(u4MoPxl[2], MIN(u4MoPxl[3], u4MoPxl[4]))));        
    u4Max = MAX(u4MoPxl[0], MAX(u4MoPxl[1], MAX(u4MoPxl[2], MAX(u4MoPxl[3], u4MoPxl[4]))));
    
    i4MoPxlLevel=(u4MoPxl[0]+u4MoPxl[1]+u4MoPxl[2]+u4MoPxl[3]+u4MoPxl[4]-u4Min-u4Max)/3;
         
    i4MoPxlLevel = MAX(i4MoPxlLevel-u2MoPxlCoring, 0x0);
    i4MoPxlLevel = MIN(((u1MaxStrMax-u1MaxStrMin)*i4MoPxlLevel >> u1MoPxlShift),
                    (u1MaxStrMax-u1MaxStrMin)) + u1MaxStrMin;

    //Filter Level Decide by Total Motion Diff
    u4Min = MIN(u4MoSum[0], MIN(u4MoSum[1], MIN(u4MoSum[2], MIN(u4MoSum[3], u4MoSum[4]))));        
    u4Max = MAX(u4MoSum[0], MAX(u4MoSum[1], MAX(u4MoSum[2], MAX(u4MoSum[3], u4MoSum[4]))));
    
    i4MoSumLevel = (u4MoSum[0]+u4MoSum[1]+u4MoSum[2]+u4MoSum[3]+u4MoSum[4]-u4Min-u4Max)/3;

    DiSta.IfSta.u4HomoAvg = (UINT32) i4MoSumLevel;
    DiPar.PdPar.bSmallMo = (i4MoSumLevel < IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_0C, SMALL_MO_THLD)) ? 1 : 0;

    i4MoSumLevel = MAX(i4MoSumLevel-u2MoSumCoring, 0x0);
    i4MoSumLevel = MIN(((u1MaxStrMax-u1MaxStrMin)*i4MoSumLevel >> u1MoSumShift),
                    (u1MaxStrMax-u1MaxStrMin)) + u1MaxStrMin;
   
    //Chose Maximun of Neccesary Filter
    i4MoSumLevel = MAX(i4MoSumLevel, i4MoPxlLevel)<<u1FilterStrDelay;

    //Gradually Change The Alpha Ratio
    if (i4MoSumLevel > u2PreAlpha)
    {
        u2PreAlpha++;
    }
    else if (i4MoSumLevel < u2PreAlpha)
    {
        u2PreAlpha--;
    }

    if (ABS(u2PreAlpha - (u2PreAlpha>>u1FilterStrDelay<<u1FilterStrDelay)) < 0x1)
    {
        u1TargetValue = (u2PreAlpha>>u1FilterStrDelay);
    }
    else if(ABS(u2PreAlpha - (((u2PreAlpha>>u1FilterStrDelay)+1)<<u1FilterStrDelay)) < 0x1)
    {
        u1TargetValue = (u2PreAlpha>>u1FilterStrDelay)+1;
    }

    u1TargetValue = MIN(u1TargetValue, 0x10);
    
    vIO32WriteFldAlign(MCVP_DARE_10, u1TargetValue, DAR_V_MAX_ALPHA);


	if ((bGetSignalType(VDP_1) == SV_ST_TV) && (IO32ReadFldAlign(PSCAN_FWCS_02, NOISE_REDUCE_EN)))
    {
        u4NoiseLevel = (u4NoiseLevel > 16) ? ((u4NoiseLevel-16)>>2) : 0;

        vIO32WriteFldAlign(MCVP_DARE_11, u4NoiseLevel, DAR_V_VARIATION_O0);
        vIO32WriteFldAlign(MCVP_DARE_11, u4NoiseLevel, DAR_V_VARIATION_O1);
    }
    else if ((bGetSignalTypeNew(VDP_1) == SV_ST_AV)&&(bHwTvdMode()==SV_CS_NTSC358))
    {
        if (_fgCornPattern == 1)
        {
            vIO32WriteFldAlign(MCVP_DARE_11, MAX(0x04, IO32ReadFldAlign(MCVP_DARE_11, DAR_V_VARIATION_O2)-1), DAR_V_VARIATION_O2);
        }
        else
        {
            vIO32WriteFldAlign(MCVP_DARE_11, MIN(0x10,  IO32ReadFldAlign(MCVP_DARE_11, DAR_V_VARIATION_O2)+1), DAR_V_VARIATION_O2);
        }
    }
}

static void _vDrvCSAdaptive(UINT8 bPath)
{
    UINT32 u4ZeroRatio;
    UINT32 u4GmvCnt = 0x08, u4GmvRatio = 0x14, u4FbGmvRatio = 0x13;
    UINT32 u4TotalBlk = IO32ReadFldAlign(PSCAN_FWCS_00, TOTAL_BLK);  
    UINT32 u4EdgeRatio = IO32ReadFldAlign(PSCAN_FWCS_00, GMV_EDGE_RATIO);
    UINT32 u4ReduceRatio = IO32ReadFldAlign(PSCAN_FWCS_01, REDUCE_RATIO);    
    UINT32 u4HistCnt = DiSta.CsSta.u2HistCnt;
    UINT32 u4EdgeDiff = 0x90;
    UINT32 u4LumaAvg = (DiSta.IfSta.u4AvgCurY + DiSta.IfSta.u4AvgPreY)>>3;
    static UINT8 _u1IntCost3D_set = 0;
    static UINT8 _u1IntCost3D_pre = 0;    
    static UINT8 _u1VarCost_m_pre = 0;
    static UINT8 _u1VarCost_l_pre = 0;
    static UINT8 _u1VarCost_m_set = 0;
    static UINT8 _u1VarCost_l_set = 0;
    
    if ((u4TotalBlk != 0) && (u4HistCnt != 0) && (_arMDDiPrm[bPath].u2Width <=800))
    {    
        u4GmvRatio = 0x14;
        u4FbGmvRatio = 0x13;
        u4GmvCnt = (DiSta.CsSta.u2EdgeCnt*u4EdgeRatio)/u4TotalBlk;
        u4ZeroRatio = (MV_HIST(0, 0)*256)/u4HistCnt;
                
        if ((u4ZeroRatio > IO32ReadFldAlign(PSCAN_FWCS_01, ZERO_RATIO))
            && (DiSta.CsSta.u2NonZeroCnt < IO32ReadFldAlign(PSCAN_FWCS_01, NON_ZERO_CNT)))
        {
            u4GmvCnt = (u4GmvCnt*u4ReduceRatio)>>8;
            u4GmvRatio = (u4GmvRatio*u4ReduceRatio)>>8;
            u4FbGmvRatio = (u4FbGmvRatio*u4ReduceRatio)>>8;
        }
        
        //LOG(6, "NonZero %d  ZeroRatio %d  GmvCnt %d  GmvRatio %d  FbGmvRatio %d  \n", 
        //        DiSta.CsSta.u2NonZeroCnt, u4ZeroRatio, u4GmvCnt, u4GmvRatio, u4FbGmvRatio);
    }    
    
    vIO32WriteFldAlign(MCVP_CS_16, u4GmvCnt, GMV_CNT_THD);  
    vIO32WriteFldAlign(MCVP_CS_0C, u4GmvRatio, GMV_CNT_RATIO); 
    vIO32WriteFldAlign(MCVP_CS_23, u4FbGmvRatio, NON_FB_GMV_RATIO); 

    // Level dependent threshold    
    u4LumaAvg = (u4LumaAvg<0x20) ? 0 : (u4LumaAvg-0x20);
    u4EdgeDiff = 0x8 + u4LumaAvg*2;
    u4EdgeDiff = (u4EdgeDiff > 0x90) ? 0x90 : u4EdgeDiff;
    vIO32WriteFldAlign(MCVP_CS_02, u4EdgeDiff, VER_DIFF_THD);  
    vIO32WriteFldAlign(MCVP_CS_02, u4EdgeDiff, HOR_DIFF_THD);     
    
#if MDDI_CS_FW_FILM_DISABLE_CS    
    // Special case for moving cross hatch  MVY = 2
    if (((DiSta.PeSta.u4EdgeSum == 0) && (u1DrvGetASL() < 0x10) 
        && ((DiSta.CsSta.u1GmvY == 0x2) || (DiSta.CsSta.u1GmvY == 0xE)))
        || (!IS_INTERLACE(VDP_1)))
    {
        vIO32WriteFldAlign(MCVP_CS_17, 0x2, FORCE_CS_FILMLIKE);
    }
    else if (DiPar.PdPar.bFwAccCnt > 0x10) //If current FW Film Detected, Disable CS
    {
        vIO32WriteFldAlign(MCVP_CS_17, 0x3, FORCE_CS_FILMLIKE);//FORCE CURRENT FILM FOR CS
    }
    else
    {
        vIO32WriteFldAlign(MCVP_CS_17, 0x0, FORCE_CS_FILMLIKE);
    }
#endif
    
    if (IS_SD_TIMING(VDP_1))
    {        
        // Moving towel CS broken issue
        if (_u1VarCost_m_set != IO32ReadFldAlign(MCVP_CS_07,VAR_COST_M))
        {
            _u1VarCost_m_pre = IO32ReadFldAlign(MCVP_CS_07,VAR_COST_M);
        }
        
        if (_u1VarCost_l_set != IO32ReadFldAlign(MCVP_CS_07,VAR_COST_L))
        {
            _u1VarCost_l_pre = IO32ReadFldAlign(MCVP_CS_07,VAR_COST_L);
        }        
        
        if (DiSta.CsSta.u1GmvValid && (DiSta.CsSta.u1GmvY != 0) && (DiSta.CsSta.u1GmvX == 0))
        {
            if ((bDrvVideoGetTiming(VDP_1) == MODE_525I) || (bDrvVideoGetTiming(VDP_1) == MODE_525I_OVERSAMPLE))
            {               
                _u1VarCost_m_set = 0x20;
                _u1VarCost_l_set = 0x20;
            }
            else
            {
                _u1VarCost_m_set = 0x10;
                _u1VarCost_l_set = _u1VarCost_l_pre;
            }
        }
        else
        {            
            _u1VarCost_m_set = _u1VarCost_m_pre;
            _u1VarCost_l_set = _u1VarCost_l_pre;
        }
        
        vIO32WriteFldAlign(MCVP_CS_07, _u1VarCost_m_set, VAR_COST_M);
        vIO32WriteFldAlign(MCVP_CS_07, _u1VarCost_l_set, VAR_COST_L);
        
        // Boundary flicker problem, Mustang fixed 
        if (_u1IntCost3D_set != IO32ReadFldAlign(MCVP_FUSION_03, IF_W_INTERNAL_3D))
        {
            _u1IntCost3D_pre = IO32ReadFldAlign(MCVP_FUSION_03, IF_W_INTERNAL_3D);
        }
        
        if (DiSta.IfSta.u4Homo < 10)
        {
            _u1IntCost3D_set = 2; 
        }
        else
        {
        _u1IntCost3D_set = (DiSta.IfSta.u4Homo < 10) ? 2 : _u1IntCost3D_pre; 
        }
        
        vIO32WriteFldAlign(MCVP_FUSION_03, _u1IntCost3D_set , IF_W_INTERNAL_3D);
    }
}

static void _vDrvPDAdaptive(UINT8 bPath)
{
    UINT32 u4MaxFrMo, u4MaxEgMo;
    UINT32 u4FrPreMin, u4EgMaxMinMo;
    UINT32 u4FrMoFilterTh, u4EgMoFilterTh;
    static UINT32 u4FrMo[4], u4EgMo[4];
    static UINT32 u4Index = 0;
    
    //Subtitle Detection
    static UINT32 u4EgMin[2];
    static UINT32 u4EgMinIndex = 0;
        
    UINT32 u4FrMinTh = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, FR_MIN);  
    UINT32 u4EgMinTh = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, EG_MIN);  
    UINT32 u4FrRate = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, FR_RATE);                                                       
    UINT32 u4EgRate32 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, EG_RATE_32);    
    UINT32 u4EgRate22 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, EG_RATE_22);
    UINT32 u4FrMaxTh = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_02, FR_MAX);        
    UINT32 u4EgMaxTh32 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, EG_MAX_START_32);              
    UINT32 u4EgMaxEndTh32 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, EG_MAX_END_32);  
    UINT32 u4EgMaxTh22 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, EG_MAX_START_22);              
    UINT32 u4EgMaxEndTh22 = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, EG_MAX_END_22);        
    UINT32 u4EgMaxEndBitTh = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_06, EG_MAX_END_BIT);              
    UINT32 u4SubTitleRate =  IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_03, SUBTITLE_RATE);          
    UINT32 u4FrMoFilterThLow= IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, FR_MO_FILTER_L);
    UINT32 u4FrMoFilterThHigh= IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, FR_MO_FILTER_H);
    UINT32 u4FrMoFilterBit = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, FR_MO_LARGE_BIT);
    UINT32 u4EgMoFilterThLow= IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, EG_MO_FILTER_L);
    UINT32 u4EgMoFilterThHigh= IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, EG_MO_FILTER_H);
    UINT32 u4EgMoFilterBit = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, EG_MO_LARGE_BIT);
    UINT32 u4ASL = u1DrvGetASL();
    UINT32 u4APL = bDrvGetAPL();
    UINT32 u4OnOff = SV_ON;
    
    //Adaptive Film Mode Parameter        
    u4FrMo[u4Index] = DiSta.PdSta.u4FrmMotion;
    u4EgMo[u4Index] = DiSta.PdSta.u4EdgMotion;
    
    u4MaxFrMo = MAX(u4FrMo[(u4Index)&3], MAX(u4FrMo[(u4Index-1)&3], u4FrMo[(u4Index-2)&3]));
    u4MaxEgMo = MAX(u4EgMo[(u4Index)&3], MAX(u4EgMo[(u4Index-1)&3], u4EgMo[(u4Index-2)&3]));    

    // Motion Depenent Eg Max
    if( u4MaxFrMo < (1<<u4EgMaxEndBitTh))
    {
        u4EgMaxTh32 += (u4EgMaxEndTh32 - u4EgMaxTh32) * u4MaxFrMo >> u4EgMaxEndBitTh;
        u4EgMaxTh22 += (u4EgMaxEndTh22 - u4EgMaxTh22) * u4MaxFrMo >> u4EgMaxEndBitTh;            
    }
    else
    {
        u4EgMaxTh32 = u4EgMaxEndTh32;
        u4EgMaxTh22 = u4EgMaxEndTh22;            
    }

    
    if ((u4EgMo[(u4Index-1)&3] <= (u4EgMo[(u4Index-0)&3]>>1))
        || (u4EgMo[(u4Index-1)&3] <= (u4EgMo[(u4Index-2)&3]>>1)))
    {
        u4EgMin[u4EgMinIndex] = u4EgMo[(u4Index-1)&3];
        u4EgMinIndex = (u4EgMinIndex+1)&1;
    }
    
    u4EgMaxMinMo = MAX(u4EgMin[0], u4EgMin[1]);          
    u4EgMaxTh32 = MIN(MAX((u4EgMaxMinMo*u4SubTitleRate)>>2, u4EgMinTh), u4EgMaxTh32);
    u4EgMaxTh22 = MIN(MAX((u4EgMaxMinMo*u4SubTitleRate)>>2, u4EgMinTh), u4EgMaxTh22);

    //Frame Static Max
    u4FrPreMin = MIN(u4FrMo[(u4Index)&3],MIN(u4FrMo[(u4Index-1)&3], u4FrMo[(u4Index-2)&3]));

    u4FrMaxTh = MIN(MAX((u4FrPreMin*u4SubTitleRate)>>2,u4FrMinTh), u4FrMaxTh);

    // Adaptive Motion Filter to remove Noise        
    if( u4MaxFrMo <= (1<<u4FrMoFilterBit)) 
    {
        u4FrMoFilterTh = u4FrMoFilterThLow +((u4FrMoFilterThHigh - u4FrMoFilterThLow) * u4MaxFrMo >>u4FrMoFilterBit);
    }
    else
    {
        u4FrMoFilterTh = u4FrMoFilterThHigh;
    }               

    if( DiPar.PdPar.u4AdaptivePxlMotionTh > u4FrMoFilterTh)
    {
        DiPar.PdPar.u4AdaptivePxlMotionTh--;            
    }
    else if( DiPar.PdPar.u4AdaptivePxlMotionTh < u4FrMoFilterTh)
    {
        DiPar.PdPar.u4AdaptivePxlMotionTh++;
    }            

    //for mp3 1080i patch
    if (IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_00, DI_FILM_APL_EN) &&(bPath == VDP_1) 
        && (!IS_SD_TIMING(VDP_1)) && IS_INTERLACE(VDP_1))
    {
        if ((u4APL < 0x30 ) && (u4ASL < 0x4))
        {
            u4EgMoFilterThLow = 0x50;
            u4EgMoFilterThHigh = 0x60;
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0x1, STA_DI_FILM_DARK);
        }
        else
        {
            vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0, STA_DI_FILM_DARK);
        }
    }
    if( u4MaxEgMo <= (1<<u4EgMoFilterBit)) 
    {
        u4EgMoFilterTh = u4EgMoFilterThLow +((u4EgMoFilterThHigh - u4EgMoFilterThLow) * u4MaxEgMo >>u4EgMoFilterBit);
    }
    else
    {
        u4EgMoFilterTh = u4EgMoFilterThHigh;
    }
           
    if( DiPar.PdPar.u4AdaptiveEgMotionTh > u4EgMoFilterTh)
    {
        DiPar.PdPar.u4AdaptiveEgMotionTh--;
    }
    else if ( DiPar.PdPar.u4AdaptiveEgMotionTh < u4EgMoFilterTh)
    {
        DiPar.PdPar.u4AdaptiveEgMotionTh++;
    }        

    //Sequence Deicsion Threshold
    DiPar.PdPar.u4AdaptiveFrMoTh = MIN(MAX(u4MaxFrMo>>u4FrRate, u4FrMinTh), u4FrMaxTh);
    DiPar.PdPar.u4AdaptiveEgMo32Th = MIN(MAX(u4MaxEgMo>>u4EgRate32, u4EgMinTh), u4EgMaxTh32);

    //22 Film Mode use more strict minimun threshold
    u4EgMinTh = MIN(u4EgMinTh, u4MaxEgMo>>1);
    DiPar.PdPar.u4AdaptiveEgMo22Th = MIN(MAX(u4MaxEgMo>>u4EgRate22, u4EgMinTh), u4EgMaxTh22);            

    if (IS_PANEL_L12R12 && DiPar.IfPar.bCzpState)
    {
        u4OnOff = SV_OFF;
    }
    MDDI_WRITE_FLD(VDP_1, MCVP_FILM_27, u4OnOff, R_22ENABLE);  
    u4Index= (u4Index+1)&3;        
}

void vDrvDIAdaptivePDSemaUnlock(void)
{
    VERIFY(x_sema_unlock(_hDIAdaptivePDIsrSema) == OSR_OK);
}

void vDrvDIFilmAdaptiveLoop(void* pvArgv)
{
    while(TRUE)
    {
        VERIFY (x_sema_lock(_hDIAdaptivePDIsrSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        _vDrvPDAdaptive(VDP_1);
    }
}

/**
 * @brief Create a new thread to align FILM FW setting with ISR
 * @param void
 */
void _vDrvDIQualityCreateThread(void)
{
    static UINT8 u1Initialized = SV_FALSE;
    if(!u1Initialized)
    {
        static HANDLE_T _hThread;
        INT32 i4Ret;
        VERIFY(x_sema_create(&_hDIAdaptivePDIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        i4Ret = x_thread_create(&_hThread, "DI Film Adaptive loop", 4096, 100,
            vDrvDIFilmAdaptiveLoop, 0, NULL);

        if (i4Ret != OSR_OK)
        {
            Printf("DI Film Adaptive loop Thread cannot be created\n");
        }
        u1Initialized = SV_TRUE;
    }
}

static void _vDrvDIInitSwRegister(void)
{      
    //FW Film    
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x10, FLD_MAX);
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x20, FLD_MIN);
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x30, FLD_GAIN1);
    vIO32WriteFldAlign(PSCAN_FWFILM_00, 0x08, FLD_GAIN2);    
    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MAX);
    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x20, FRM_MIN);
    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x28, FRM_GAIN1);
    vIO32WriteFldAlign(PSCAN_FWFILM_02, 0x06, FRM_GAIN2);
    vIO32WriteFldAlign(PSCAN_FWFILM_06, 0x01, FILED_WEAVE_EN);   
    vIO32WriteFldAlign(PSCAN_FWFILM_06, 0x01, DET_24TO50_EN);   

    //Adaptive IF     
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0x00, MOVING_CZP_BOB_EN);       
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0x01, FLD_INV_EN);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0x01, CZP_AD_EN);    
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0x01, IF_C_AD_EN);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0x01, CZP_3D_EN);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 0x01, DI_FILM_APL_EN);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_01, 0x08, FLD_INV_IN_THD);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_01, 0x04, FLD_INV_OUT_THD);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_01, 0x0C, FLD_INV_MAX_THD);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_01, 0x04, CZP_AD_MIN_THD);    
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_02, 0x04, DI_FORCE_MODE_DELAY);
	vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_IF_00, 1, SAW_STILL_PATCH_EN);
    vIO32WriteFldAlign(VDP_SCPQ_04, 0x0, SCPQ_DTV_TEAR_MO_PENALTY);
    vIO32WriteFldAlign(VDP_SCPQ_04, 0x1, SCPQ_DTV_TEAR_REF_FAKEHD);
    vIO32WriteFldAlign(VDP_SCPQ_05, 0xA, SCPQ_MOTION_TH_ORDER);
    vIO32WriteFldAlign(VDP_SCPQ_05, 0x1E, SCPQ_C_JAGGY_UPDATE_CNT);
    vIO32WriteFldAlign(VDP_SCPQ_05, 0x1E, SCPQ_DTV_TEAR_UPDATE_CNT);

    //Adaptive CS    
    vIO32WriteFldAlign(PSCAN_FWCS_00, 0xF0, Y4_RATIO); 
    vIO32WriteFldAlign(PSCAN_FWCS_00, 0x30, GMV_EDGE_RATIO);   
    vIO32WriteFldAlign(PSCAN_FWCS_01, 0xA0, REDUCE_RATIO);   
    vIO32WriteFldAlign(PSCAN_FWCS_01, 0x03, NON_ZERO_CNT);  
    vIO32WriteFldAlign(PSCAN_FWCS_01, 0x40, ZERO_RATIO);  
    vIO32WriteFldAlign(PSCAN_FWCS_01, 0x01, ZERO_THLD);  
    vIO32WriteFldAlign(PSCAN_FWCS_02, 0x01, FAVOR_CS_EN);
    vIO32WriteFldAlign(PSCAN_FWCS_02, 0x01, NOISE_REDUCE_EN);
    vIO32WriteFldAlign(PSCAN_FWCS_03, 0x30, HIST_DIFF_RATIO); 
    vIO32WriteFldAlign(PSCAN_FWCS_03, 0x02, COUNT_EXE_THD); 
    vIO32WriteFldAlign(PSCAN_FWCS_03, 0x0C, COUNT_MAX_BND); 
    vIO32WriteFldAlign(PSCAN_FWCS_04, 0x10, REDUCE_NOISE_TH); 
    vIO32WriteFldAlign(PSCAN_FWCS_04, 0x03, REDUCE_STEP); 
    vIO32WriteFldAlign(PSCAN_FWCS_05, 0x00, STILL_MOV_EN);

    //DYNAMIC FILM MODE DETECTION
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x10, FR_MIN);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_00, 0x18, EG_MIN);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x03, FR_RATE);                                                       
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x02, EG_RATE_32);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_01, 0x03, EG_RATE_22);    
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_02, 0x800, FR_MAX);   
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_03, 0x20, SUBTITLE_RATE);    
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x60, EG_MAX_START_32);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_05, 0x600, EG_MAX_END_32);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x60, EG_MAX_START_22);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_07, 0x600, EG_MAX_END_22);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_06, 0x11, EG_MAX_END_BIT); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x08, FR_MO_FILTER_L);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x28, FR_MO_FILTER_H);        
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_08, 0x10, FR_MO_LARGE_BIT); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x06, EG_MO_FILTER_L);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x20, EG_MO_FILTER_H);        
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_FILM_09, 0x0A, EG_MO_LARGE_BIT); 
    
    //Motion Adaptive DAR
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, 0x04, DAR_V_MAX_MIN);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, 0x10, DAR_V_MAX_MAX);     
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_00, 0x01, DAR_OFF_IN_FILM); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, 0x20, DAR_V_MOTION_PXL_CORING);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_01, 0x2000, DAR_V_MOTION_SUM_CORING);
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, 0x07, DAR_V_MOTION_PXL_SHIFT);  
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, 0x0C, DAR_V_MOTION_SUM_SHIFT); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_02, 0x05, DAR_V_FILTER_STR_DELAY_SHIFT); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_03, 0x10 ,DAR_V_TBL_IN_FOR_NOISE); 
    vIO32WriteFldAlign(PSCAN_FW_ADAPTIVE_DAR_03, 0x10 ,DAR_V_TBL_OUT_FOR_NOISE);             
}

void vDrvDIGetCSStatusISR(void)
{
    UINT32 i, u4DirtyCnt = 0;
    
    //============= CS Status ====================
    DiSta.CsSta.u1GmvValid = IO32ReadFldAlign(MCVP_CS_28, GMV_VALID);
    DiSta.CsSta.u1GmvX = IO32ReadFldAlign(MCVP_CS_28, GMV_MVX);
    DiSta.CsSta.u1GmvY = IO32ReadFldAlign(MCVP_CS_28, GMV_MVY);
    DiSta.CsSta.u2HistCnt = IO32ReadFldAlign(MCVP_CS_27, HIST_CNT);
    DiSta.CsSta.u2HistMax = IO32ReadFldAlign(MCVP_CS_27, STA_MAX_CNT1);
    DiSta.CsSta.u2EdgeCnt = IO32ReadFldAlign(MCVP_CS_18, STA_EDGE_CNT);
    DiSta.CsSta.u2NonZeroCnt = IO32ReadFldAlign(MCVP_CS_29, CS_NON_ZERO_CNT);
    DiSta.CsSta.u4HistDiffAcc = IO32ReadFldAlign(MCVP_CS_2A, HIST_DIFF_ACC);
    DiSta.CsSta.u4FieldSawRank = (DiSta.CsSta.u4FieldSawRank<<4) | IO32ReadFldAlign(MCVP_CS_0C, FIELD_SAW_RANK);
    DiSta.CsSta.u4MultibusrtCnt = IO32ReadFldAlign(MCVP_CS_18, STA_MULTIBURST_CNT);

	vIO32WriteFldAlign(MCVP_CS_11, 1, HIST_STA_AUTO_READ);
	vIO32WriteFldAlign(MCVP_CS_11, 0, HIST_STA_RADDR_INI);
	vIO32WriteFldAlign(MCVP_CS_11, SV_ON, SET_HIST_STA_RADDR);
    
    for (i = 0; i < DI_CS_MV_HIST_BIN/2; i++)
    {
        while (u4IO32Read4B(MCVP_CS_13)&0x80000000)  
        {
            if (u4DirtyCnt++ > DI_CS_MV_HIST_BIN)
            {
                DiSta.CsSta.u1MvReadValid = 0;
                LOG(4, "MV read error %d \n", u4DirtyCnt);
                return;
            }
        }
        
        DiSta.CsSta.u4MVHist[i] = u4IO32Read4B(MCVP_CS_12);
    }
    DiSta.CsSta.u1MvReadValid = 1;
    
    DiSta.CsSta.bAllBurst = IO32ReadFldAlign(MCVP_CS_11, PRE_WHOLE_MULTIBURST);
    DiSta.CsSta.u2SmtCnt = IO32ReadFldAlign(MCVP_CS_26, STA_SMT_CNT);
    DiSta.CsSta.u2SmtCntROI = IO32ReadFldAlign(MCVP_CS_26, STA_SMT_CNT_ROI);
    DiSta.CsSta.u1SmtDeconf = IO32ReadFldAlign(MCVP_CS_28, STA_SMT_DECONF);
    DiSta.CsSta.u1MvDeconf = IO32ReadFldAlign(MCVP_CS_28, STA_MV_DECONF);
    DiSta.CsSta.u4SawAB = IO32ReadFldAlign(MCVP_CS_1D, STA_SAW_AB_CNT);
    DiSta.CsSta.u4SawBC = IO32ReadFldAlign(MCVP_CS_1E, STA_SAW_BC_CNT);    
}

void vDrvDIFilmModeOnOff(UINT8 bPath, UINT8 u1OnOff)
{   
    if (bPath == VDP_1)
    {
        DiPQMode.bCfgFilmOnOff = u1OnOff;
        _vDrvFilmDefaultOnOff(VDP_1, u1OnOff);
    }    
}

UINT8 bDrvGetFilmStatus(void)
{
    return ((UINT8)DiPar.PdPar.u2FilmMode);
}

void vDrvDISetBob(UINT8 u1VdpId, UINT8 u1OnOff)
{       
    if (u1VdpId >= VDP_NS)
    {
        return;
    }

    if (SV_ON == u1OnOff)
    {
        vDrvDISetInterpMode(u1VdpId, E_MIB_DI_Y1C1);
    }
    else
    {
        vDrvDISetInterpMode(u1VdpId, MIB_DI_MODE(u1VdpId));
    }
}

void vDrvDISetFilmModeOnOff(UINT8 u1Path, UINT8 u1FilmType, UINT8 u1OnOff)
{
    if (u1Path == VDP_1)
    {
        switch (u1FilmType)
        {
    	    case 0:
                DiPQMode.b22Film = u1OnOff;
                break;
    	    case 1:
                DiPQMode.b32Film = u1OnOff;
                break;
            case 2:
                DiPQMode.bFilmOnOff_MultiCadence = u1OnOff;
                break;
            default:
                break;
        }
    }    
}

UINT8 u1DrvDIGetFilmOnOff(UINT8 bPath)
{
    return DiPQMode.bCfgFilmOnOff;
}

/**
 * @brief Turn on/off film mode Freeze Fro B2R Source
 * @param bPath VDP_1/VDP_2
 * @param u1OnOff SV_ON/SV_OFF  enable frame repeat(start freezing) => SV_OFF 
 * @warning This should be invoked by TVD only
 */
void vDrvDIB2RFilmModeFreeze(UINT8 bPath, UINT8 u1OnOff)
{
     vIO32WriteFldAlign(MCVP_FILM_27,    u1OnOff, R_22ENABLE);  
     vIO32WriteFldAlign(MCVP_FILM_27,    u1OnOff, R_32ENABLE); 
     vIO32WriteFldAlign(PSCAN_FWFILM_06, u1OnOff, FILED_WEAVE_EN);
}

void vDrvDISetMotionLevel(UINT8 u1VdpId, UINT8 u1YOnOff, UINT8 u1COnOff, UINT8 u1YLevel, UINT8 u1CLevel)
{
    u1YFixMotion[u1VdpId] = u1YOnOff;
    u1CFixMotion[u1VdpId] = u1COnOff;
    u1YMotionLevel[u1VdpId] = u1YLevel;
    u1CMotionLevel[u1VdpId] = u1CLevel;
}

void vDrvDISetRegionMotionOnOff(UINT8 u1VdpId)
{
    E_MIB_DI_DMODE eDIMode = MIB_DI_MODE(u1VdpId);
    UINT32 wSrcWidth = IO32ReadFldAlign(MCVP_KC_0A, HDEW);
    UINT32 u4TurnOff = (wSrcWidth <= RA_ADAPTIVE_MIN_WIDTH) | (eDIMode == E_MIB_DI_Y2C2) | (eDIMode == E_MIB_DI_Y2C1);

    if (u1VdpId == VDP_1)
    {
        vIO32WriteFldAlign(MCVP_FUSION_06, u4TurnOff ? 0x6 : 0x2, IF_RA_MOTION_LEVEL_INI);        
        vIO32WriteFldAlign(MCVP_FUSION_21, u4TurnOff ? 0x3 : 0x0, IF_FORCE_SCENE_CHANGE);
        DiPar.bForceSceneChange = u4TurnOff;
    }
}

void vDrvDISetInterpMode(UINT8 u1VdpId, E_MIB_DI_DMODE eDIMode)
{
    UINT32 u4IF_FIX_MOTION_Y = 0;
    UINT32 u4IF_FIX_MOTION_C = 0;
    UINT32 u4IF_FORCE_MOTION_Y = 0;
    UINT32 u4IF_FORCE_MOTION_C = 0;    
    UINT32 u4IF_FIX_BOB_Y = 0;
    UINT32 u4IF_FIX_BOB_C = 0;
    
    UINT32 u4IF_TWO_FIELD_MODE = 0x0;
    UINT32 u4IF_TWO_FIELD_C_MODE = 0x0;    
    UINT32 u4IF_THREE_FIELD_MODE = 0x0;     
    UINT32 u4IF_THREE_FIELD_C_MODE = 0x0;      // Cobra ECO & Viper
    UINT32 u4IF_FIX_B_WEIGHT_Y = 0x1;
    UINT32 u4IF_FIX_B_WEIGHT_C = 0x1;    
    UINT32 u4IF_FORCE_B_WEIGHT_Y = 0x4;
    UINT32 u4IF_FORCE_B_WEIGHT_C = 0x4;        
    UINT32 u4DARE_TWO_FIELD_C_MODE = 0x0;
    
    UINT32 u4IF_W_MC = 0x4;
    UINT32 u4IF_W_FLICKER = 0x8;
    UINT32 u4IF_W_MOTION = 0x4;
    UINT32 u4IF_W_INTERNAL_3D = 0x4;
    UINT32 u4IF_W_SMOOTH_STILL = 0x4;
    UINT32 u4IF_W_SAWTOOTH_STILL = 0x4;
    UINT32 u4IF_TWO_FIELD_LOCAL_DIFF = 0x8;
    UINT32 u4IF_2D_INTERNAL_DIFF_MODE = 0x0;
    UINT32 u4IF_HOR_EDGE_COST_ADD = 0x0;
    UINT32 u4IF_HOR_EDGE_COST_TH = 0x0;
    UINT32 u4IF_MIN_COST_2D = 0xA;
    UINT32 u4IF_BASE_COST_2D = 0x8;
    UINT8 bOnOff = _u1DrvDIQualityOnOff(VDP_1);
    UINT8 bFullOnOff = _u1DrvDIFullQualityOnOff(VDP_1);

    // Luma mode config
    switch (eDIMode)    
    {        
		case E_MIB_DI_Y3C1:
		case E_MIB_DI_Y3C2:
			u4IF_THREE_FIELD_MODE = 1;
			break;
		case E_MIB_DI_Y2C2:
		case E_MIB_DI_Y2C1:
			u4IF_TWO_FIELD_MODE = 1;
			u4IF_FIX_B_WEIGHT_Y = 1;
			u4IF_FORCE_B_WEIGHT_Y = 8;
			break;
        case E_MIB_DI_Y1C1:
        case E_MIB_DI_UNKNOWN:
            u4IF_FIX_MOTION_Y = 0x1;            
            u4IF_FORCE_MOTION_Y = 0x8;            
            u4IF_FIX_BOB_Y = 0x1;
            break;
        default:
            break;
    }

    // Chroma mode config
    switch (eDIMode)    
    {
        case E_MIB_DI_Y2C2:
        case E_MIB_DI_Y3C2:
        case E_MIB_DI_Y4C2:             
            u4IF_TWO_FIELD_C_MODE = 1;
            u4IF_FIX_B_WEIGHT_C = 1;
            u4IF_FORCE_B_WEIGHT_C = 8;            
            u4DARE_TWO_FIELD_C_MODE = 1;            
            break;        
        case E_MIB_DI_Y1C1:
        case E_MIB_DI_Y2C1:
        case E_MIB_DI_Y3C1:
        case E_MIB_DI_Y4C1:    
        case E_MIB_DI_UNKNOWN:
            u4IF_FIX_MOTION_C = 0x1;            
            u4IF_FORCE_MOTION_C = 0x8;            
            u4IF_FIX_BOB_C = 0x1;
            break;
        default:
            break;
    }

    // Fusion cost config
	switch (eDIMode)    
	{
		case E_MIB_DI_Y2C2:
		case E_MIB_DI_Y2C1:
			u4IF_W_MC                  = 0;
			u4IF_W_FLICKER             = 0;
			u4IF_W_MOTION              = 0;
			u4IF_W_INTERNAL_3D         = 0;
			u4IF_W_SMOOTH_STILL        = 0;            
			u4IF_W_SAWTOOTH_STILL      = 8;
			u4IF_HOR_EDGE_COST_ADD     = 1;
			u4IF_HOR_EDGE_COST_TH      = 0;
			u4IF_MIN_COST_2D           = 0;
			u4IF_BASE_COST_2D          = 0;             
			u4IF_TWO_FIELD_LOCAL_DIFF  = 8;
			u4IF_2D_INTERNAL_DIFF_MODE = 1;
			break;
		case E_MIB_DI_Y3C1:
		case E_MIB_DI_Y3C2:
			u4IF_W_MC                  = 0;
			u4IF_W_FLICKER             = 0;
			break;
        default:
            break;
	}

    #ifdef PSCAN_AUTO_VERIFY
    u4IF_FIX_MOTION_Y = u1YFixMotion[u1VdpId];
    u4IF_FIX_MOTION_C = u1CFixMotion[u1VdpId];
    u4IF_FORCE_MOTION_Y = u1YMotionLevel[u1VdpId];
    u4IF_FORCE_MOTION_C = u1CMotionLevel[u1VdpId];    
    #endif
    
    if (u1VdpId == VDP_1)
    {
        vIO32WriteFldMulti(MCVP_FUSION_20, 
            P_Fld(u4IF_FIX_MOTION_Y, IF_FIX_MOTION_Y) | P_Fld(u4IF_FORCE_MOTION_Y, IF_FORCE_MOTION_Y) | 
            P_Fld(u4IF_FIX_MOTION_C, IF_FIX_MOTION_C) | P_Fld(u4IF_FORCE_MOTION_C, IF_FORCE_MOTION_C) |
            P_Fld(u4IF_FIX_B_WEIGHT_Y, IF_FIX_B_WEIGHT_Y) | P_Fld(u4IF_FORCE_B_WEIGHT_Y, IF_FORCE_B_WEIGHT_Y) |
            P_Fld(u4IF_FIX_B_WEIGHT_C, IF_FIX_B_WEIGHT_C) | P_Fld(u4IF_FORCE_B_WEIGHT_C, IF_FORCE_B_WEIGHT_C) |
            P_Fld(u4IF_FIX_BOB_Y, IF_FIX_BOB_Y) | P_Fld(u4IF_FIX_BOB_C, IF_FIX_BOB_C));

        vIO32WriteFldMulti(MCVP_FUSION_11, 
            P_Fld(u4IF_TWO_FIELD_MODE, IF_TWO_FIELD_MODE) | 
            P_Fld(u4IF_TWO_FIELD_C_MODE, IF_TWO_FIELD_C_MODE) |
            P_Fld(u4IF_THREE_FIELD_MODE, IF_THREE_FIELD_MODE) | 
            P_Fld(u4IF_THREE_FIELD_C_MODE, IF_THREE_FIELD_C_MODE));        
        
        vIO32WriteFldAlign(MCVP_FUSION_01, u4IF_MIN_COST_2D           , IF_MIN_COST_2D);
        vIO32WriteFldAlign(MCVP_FUSION_01, u4IF_BASE_COST_2D          , IF_BASE_COST_2D);         
        vIO32WriteFldAlign(MCVP_FUSION_01, u4IF_W_FLICKER             , IF_W_FLICKER);
        vIO32WriteFldAlign(MCVP_FUSION_01, u4IF_W_MOTION              , IF_W_MOTION);           
        vIO32WriteFldAlign(MCVP_FUSION_01, u4IF_HOR_EDGE_COST_ADD     , IF_HOR_EDGE_COST_ADD);
        vIO32WriteFldAlign(MCVP_FUSION_02, u4IF_HOR_EDGE_COST_TH      , IF_HOR_EDGE_COST_TH);
        vIO32WriteFldAlign(MCVP_FUSION_03, u4IF_W_MC                  , IF_W_MC);
        vIO32WriteFldAlign(MCVP_FUSION_03, u4IF_W_INTERNAL_3D         , IF_W_INTERNAL_3D);
        vIO32WriteFldAlign(MCVP_FUSION_04, u4IF_W_SMOOTH_STILL        , IF_W_SMOOTH_STILL);
        vIO32WriteFldAlign(MCVP_FUSION_04, u4IF_W_SAWTOOTH_STILL      , IF_W_SAWTOOTH_STILL);        
        vIO32WriteFldAlign(MCVP_FUSION_04, (IS_3D_FMT_CONV(u1VdpId))  , IF_LM_USE_CURRENT);        
        vIO32WriteFldAlign(MCVP_FUSION_11, u4IF_TWO_FIELD_LOCAL_DIFF  , IF_TWO_FIELD_LOCAL_DIFF);
        vIO32WriteFldAlign(MCVP_FUSION_11, u4IF_2D_INTERNAL_DIFF_MODE , IF_2D_INTERNAL_DIFF_MODE);
        vIO32WriteFldAlign(MCVP_DARE_00  , u4DARE_TWO_FIELD_C_MODE    , DARE_TWO_FIELD_C_MODE);
               
        #ifdef CC_MT5398    // Cobra ECO   
        vIO32WriteFldAlign(MCVP_FUSION_20 , u4IF_THREE_FIELD_C_MODE, IF_FIX_VDWT_COEFF);
        #endif        
        DiPar.CsPar.bCsOnOff = bFullOnOff && (E_TD_IN_2D_I == _arMDDiPrm[u1VdpId].eTDInput);
        vDrvDISetRegionMotionOnOff(VDP_1);
        _vDrvSetIfOnOff(VDP_1, bOnOff);
        _vDrvSetDarOnOff(VDP_1, bOnOff); 
        _vDrvFilmDefaultOnOff(VDP_1, bFullOnOff);
        _vDrvSetCsOnOff(VDP_1, DiPar.CsPar.bCsOnOff&&(!DiPar.bForceSceneChange));     
    }
    else
    {
        vIO32WriteFldMulti(MCVP_SB_04, 
            P_Fld(u4IF_FIX_MOTION_Y, SB_FIX_MOTION_Y) | P_Fld(u4IF_FIX_MOTION_C, SB_FIX_MOTION_C) | 
            P_Fld(u4IF_FORCE_MOTION_Y, SB_FORCE_MOTION_Y) | P_Fld(u4IF_FORCE_MOTION_C, SB_FORCE_MOTION_C));
    }        
}

void vDrvDIInitQuality(UINT8 bPath)
{
    //initial PSCAN HW
    vDrvLoadRegTbl(MDDI_QTY_INIT_MAIN);
    vDrvLoadRegTbl(MDDI_QTY_INIT_SUB);
    _vDrvDIInitSwRegister();
    
    //initial FW
    DiPQMode.eIfQty = E_DI_QUALITY_AGGRESSIVE;
    DiPQMode.ePeQty = E_DI_QUALITY_AGGRESSIVE;
    DiPQMode.eFilmQty = E_DI_QUALITY_AGGRESSIVE; 
}

void vDrvDISetQuality(UINT8 bPath)
{    
    if (bPath == VDP_1)
    {            
        _vDrvDISetIFQuality(VDP_1);
        _vDrvDISetDARQuality(VDP_1);        
        _vDrvDISetPEQuality(VDP_1);
        _vDrvDISetPDQuality(VDP_1);
        _vDrvDISetCSQuality(VDP_1);
        _vDrvDISetAdatpive(VDP_1);                                 
    }
    else
    {
        _vDrvDISetPDMAQuality(VDP_2);
    }
}

void vDrvDISetQualityISR(void)
{  
#ifndef __MODEL_slt__
        
    _vDrvDIGetPQStatusISR();

    if ((IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_QTY))
        && (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_QTY_ISR)))
    {        
        if (_u1DrvDIFullQualityOnOff(VDP_1))
        {
            _vDrvDISetCSQualityISR();
            _vDrvDISetPDQualityISR();            
            _vDrvDISetIFQualityISR();       
        }
        
        if (_u1DrvDIQualityOnOff(VDP_1))
        {            
            _vDrvDISetPEQualityISR();
            _vDrvDISetDARQualityISR();                
        }
    }        
#endif
}

void vDrvDISetQualityISR_Ex(void)
{  
#ifndef __MODEL_slt__
    if ((IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_QTY))
        && (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_QTY_ISR)))
    {        
        if (_u1DrvDIFullQualityOnOff(VDP_1))
        {
            _vDrvDISetIFQualityISR_Ex(); 
        }
    }        
#endif
}

void vDrvDIQtyMainLoop(UINT8 bPath)
{
    if (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_QTY) 
        && IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_QTY_MLOOP))
    {   
        if ((eDrvNRGet3DNRMode() == MCNR_MODE_MC) || (eDrvNRGet3DNRMode() == MCNR_MODE_MV))
        {
            _vDrvCSAdaptive(VDP_1);  
            _vDrvDIFavorCS();
        }
        
        if (_u1DrvDIQualityOnOff(bPath))
        {
            _vDrvIFAdaptive(VDP_1);
            _vDrvPEAdaptive(VDP_1);            
            _vDrvDARAdaptive(VDP_1);            
        }       

        _vDrvDIChromaJaggyDetection();
        _vDrvDIDTVTearDetection();
        _vDrvDISetOSD();
    }
}

/**
* @brief Set DI Quality Mode
* @param eDIEngineType E_DI_ENGINE_MA/E_DI_ENGINE_EDGEP/E_DI_ENGINE_FILM
* @param eDIQualityMode E_DI_QUALITY_AGGRESSIVE / E_DI_QUALITY_DEFENSIVE
*/
void vDrvDISetQualityMode(E_DI_ENGINE_TYPE eDIEngineType, E_DI_QUALITY_MODE eDIQualityMode)
{
    switch (eDIEngineType)
    {
        case E_DI_ENGINE_IF:
            if (eDIQualityMode != DiPQMode.eIfQty)
            {
                DiPQMode.eIfQty = eDIQualityMode;
                _vDrvDISetIFQuality(VDP_1);
            }
            break;
        case E_DI_ENGINE_PE:
            if (eDIQualityMode != DiPQMode.ePeQty)
            {
                DiPQMode.ePeQty = eDIQualityMode;
                _vDrvDISetPEQuality(VDP_1);
            }
            break;
        case E_DI_ENGINE_FILM:
            if (eDIQualityMode != DiPQMode.eFilmQty)
            {
                DiPQMode.eFilmQty = eDIQualityMode;
                _vDrvDISetPDQuality(VDP_1);
            }
            break;
        default:
            break;
    }
}

void vDrvDIForce3DModeWithDelay(void)
{
    if ((IS_INTERLACE(VDP_1)) && (DiSta.IfSta.u4Homo == 0) && (DiSta.IfSta.u4HomoDiff == 0))
    {
		DiPar.IfPar.bFroce3DDealy = IO32ReadFldAlign(PSCAN_FW_ADAPTIVE_IF_02, DI_FORCE_MODE_DELAY);
    }
	else
	{
		DiPar.IfPar.bFroce3DDealy = 0;
	}
}

void vDrvDISetForce3DDelay(UINT8 u1Value)
{
	DiPar.IfPar.bFroce3DDealy = u1Value;
}

void vDrvDISetChromaMaxMoLevel(UINT8 u1MaxMoC)
{
    DiPar.IfPar.bMaxMoC = MIN(u1MaxMoC, 8);        
}

////////////////////////////////////////////////////////////////////////////////
//Debug Utility
////////////////////////////////////////////////////////////////////////////////
EXTERN void vDrvDIWaitDIVSyncSema(void);

void vDrvDISetDebugOSDModule(E_OSD_DEBUG_IDX u4Module)
{
    vIO32Write4B(SWRW_00, (UINT32)u4Module);
}

void vDrvDISetDebugOSD(E_OSD_DEBUG_IDX u4Module, UINT32 u4Value)
{
    //LOG(4, "Module %d = %d  Value 0x%08x\n", u4IO32Read4B(SWRW_00), u4Module, u4Value);

    if (u4IO32Read4B(SWRW_00) == u4Module)
    {
        vIO32Write4B(MCVP_KC_2A, u4Value);
    }
}

void vDrvDISetDebugOSDMask(E_OSD_DEBUG_IDX u4Module, UINT32 u4Value, UINT32 Fld)
{
    //LOG(4, "Module %d = %d  Value 0x%08x\n", u4IO32Read4B(SWRW_00), u4Module, u4Value);

    if (u4IO32Read4B(SWRW_00) == u4Module)
    {
        vIO32WriteFldAlign(MCVP_KC_2A, u4Value, Fld);
    }
}

void vDrvDISetInkColor(UINT32 u4Idx, UINT32 u4Color)
{
    switch (u4Idx)
    {
        case 0:
            vIO32WriteFldAlign(MCVP_DARE_08, u4Color, INK_COLOR_Y_0);
            break;
        case 1:
            vIO32WriteFldAlign(MCVP_DARE_09, u4Color, INK_COLOR_Y_1);
            break;
        case 2:
            vIO32WriteFldAlign(MCVP_DARE_0A, u4Color, INK_COLOR_CB_0);
            break;
        case 3:
            vIO32WriteFldAlign(MCVP_DARE_0B, u4Color, INK_COLOR_CB_1);
            break;
        case 4:
            vIO32WriteFldAlign(MCVP_DARE_0C, u4Color, INK_COLOR_CR_0);
            break;
        case 5:
            vIO32WriteFldAlign(MCVP_DARE_0D, u4Color, INK_COLOR_CR_1);
            break;
    }
}

void vDrvDISetInk(UINT8 bMode, UINT16 u2Type)
{    
    #ifndef NDEBUG
    LOG(3,"Ink Mode: 0) Ink Off , 1) CS , 2) PE , 3) iFusion, 4) DARE, 5) SB\n");

    //Reset All ink Off
    vIO32WriteFldAlign(MCVP_DARE_07, SV_OFF, DI_INK_EN);
    vIO32WriteFldAlign(MCVP_SB_06, SV_OFF, SB_INK_ENABLE);
    vIO32WriteFldAlign(MCVP_KC_21, 0x10, OSD_Y_POS);
    vIO32WriteFldAlign(MCVP_KC_21, 0x10, OSD_X_POS);
    vIO32WriteFldAlign(MCVP_DARE_07, (bMode) ? (bMode-1) : bMode, INK_SEL);

    switch (bMode)
    {
        case 0:
            vIO32WriteFldAlign(MCVP_KC_21, SV_OFF, OSD_MODE);
            vIO32WriteFldAlign(MCVP_DARE_07, SV_OFF, DI_INK_EN);
            vIO32WriteFldAlign(MCVP_CS_17, 0, CS_INK_SEL);
            vIO32WriteFldAlign(MCVP_PE_00, 0, PE_INK_SEL);
            vIO32WriteFldAlign(MCVP_FUSION_1A, 0, IF_INK_SEL);
            vIO32WriteFldAlign(MCVP_DARE_07, 0, DARE_INK_SEL);
            break;
            
        case 1: //CS ink            
            LOG(6, "0 : FullSearch MV\n");
            LOG(6, "1 : ME MV Before Write To Dram\n");
            LOG(6, "2 : MC RAW MV From Dram\n");
            LOG(6, "3 : Proc MV\n");
            LOG(6, "4 : Erosioned 2x1 MV\n");
            LOG(6, "5 : MC Input 2x1 MV\n");
            LOG(6, "6 : Edge Type\n");
            LOG(6, "7 : Gradient Type\n");
            LOG(6, "8 : Alias Found\n");
            LOG(6, "9 : Multi-Burst\n");
            LOG(6, "10 : Brailing\n");
            LOG(6, "11 : Instable Region\n");
            LOG(6, "12 : Text Line\n");
            LOG(6, "13 : Local De-Confidence\n");
            LOG(6, "14 : Confidence_Ini\n");
            LOG(6, "15 : Confidence\n");
            
            vIO32WriteFldAlign(MCVP_DARE_07, SV_ON, DI_INK_EN);
            vIO32WriteFldAlign(MCVP_CS_17, u2Type, CS_INK_SEL);

            if (u2Type == 5)
            {
                vIO32WriteFldAlign(MCVP_DARE_08, 0xdf38bdf0, INK_COLOR_Y_0);
                vIO32WriteFldAlign(MCVP_DARE_09, 0x38bdf38b, INK_COLOR_Y_1);
                vIO32WriteFldAlign(MCVP_DARE_0A, 0x50fb8508, INK_COLOR_CB_0);
                vIO32WriteFldAlign(MCVP_DARE_0B, 0xfb850fb8, INK_COLOR_CB_1);
                vIO32WriteFldAlign(MCVP_DARE_0C, 0x88fffff8, INK_COLOR_CR_0);
                vIO32WriteFldAlign(MCVP_DARE_0D, 0x888, INK_COLOR_CR_1);
            }
            else if (u2Type == 15)
            {
                vIO32WriteFldAlign(MCVP_DARE_08, 0x88888888, INK_COLOR_Y_0);
                vIO32WriteFldAlign(MCVP_DARE_09, 0x8888888f, INK_COLOR_Y_1);
                vIO32WriteFldAlign(MCVP_DARE_0A, 0xeca86428, INK_COLOR_CB_0);
                vIO32WriteFldAlign(MCVP_DARE_0B, 0xf, INK_COLOR_CB_1);
                vIO32WriteFldAlign(MCVP_DARE_0C, 0xeca86428, INK_COLOR_CR_0);
                vIO32WriteFldAlign(MCVP_DARE_0D, 0xf, INK_COLOR_CR_1);
            }            
	     else if  (u2Type == 2)
	     {
	         vIO32WriteFldAlign(MCVP_DARE_08, 0xaaaaaaaa, INK_COLOR_Y_0);
                vIO32WriteFldAlign(MCVP_DARE_09, 0xaaaaaaaa, INK_COLOR_Y_1);
                vIO32WriteFldAlign(MCVP_DARE_0A, 0xffffffff, INK_COLOR_CB_0);
                vIO32WriteFldAlign(MCVP_DARE_0B, 0x00000008, INK_COLOR_CB_1);
                vIO32WriteFldAlign(MCVP_DARE_0C, 0xeca86420, INK_COLOR_CR_0);
                vIO32WriteFldAlign(MCVP_DARE_0D, 0xeca86428, INK_COLOR_CR_1);
	     }
	     else
	     {  
                vIO32WriteFldAlign(MCVP_DARE_08, 0x88888888, INK_COLOR_Y_0);
                vIO32WriteFldAlign(MCVP_DARE_09, 0x88888888, INK_COLOR_Y_1);
                vIO32WriteFldAlign(MCVP_DARE_0A, 0xffffffff, INK_COLOR_CB_0);
                vIO32WriteFldAlign(MCVP_DARE_0B, 0x00000000, INK_COLOR_CB_1);
                vIO32WriteFldAlign(MCVP_DARE_0C, 0xff884422, INK_COLOR_CR_0);
                vIO32WriteFldAlign(MCVP_DARE_0D, 0xff884422, INK_COLOR_CR_1);
	     }
            break;
            
        case 2: //PE ink            
            LOG(6, "0 : Final_Dir\n");
            LOG(6, "1 : Final_Spec_Dir\n");
            LOG(6, "2 : Final_Confidence\n");
            LOG(6, "3 : Final_Coeff\n");
            LOG(6, "4 : Bob_Density\n");

            vIO32WriteFldAlign(MCVP_DARE_07, SV_ON, DI_INK_EN);
            vIO32WriteFldAlign(MCVP_PE_00, u2Type, PE_INK_SEL);

            vIO32WriteFldAlign(MCVP_DARE_08, 0x999999a8, INK_COLOR_Y_0);
            vIO32WriteFldAlign(MCVP_DARE_09, 0x66666688, INK_COLOR_Y_1);
            vIO32WriteFldAlign(MCVP_DARE_0A, 0x333333a8, INK_COLOR_CB_0);
            vIO32WriteFldAlign(MCVP_DARE_0B, 0xcccccc88, INK_COLOR_CB_1);
            vIO32WriteFldAlign(MCVP_DARE_0C, 0x22222218, INK_COLOR_CR_0);
            vIO32WriteFldAlign(MCVP_DARE_0D, 0xdddddd88, INK_COLOR_CR_1);
            break;

        case 3: // FUSION Ink
            LOG(6, "0 : Boundary saw type [0, 4, 8]\n");
            LOG(6, "1 : Boundary saw run [0~8]\n");
            LOG(6, "2 : Block motion level [0~15]\n");
            LOG(6, "3 : Region motion level ini [0~15]\n");
            LOG(6, "4 : Still [0~8]\n");
            LOG(6, "5 : Local motion level prev [0~15]\n");
            LOG(6, "6 : Local motion level [0~15]\n");
            LOG(6, "7 : Region motion level final [0~15]\n");
            LOG(6, "8 : Sawtooth [0~15]\n");
            LOG(6, "9 : Edge Type [0~3]\n");
            LOG(6, "10 : Y internal cost 2D [0~15]\n");
            LOG(6, "11 : C internal cost 2D [0~15]\n");
            LOG(6, "12 : Texture cost [0~15]\n");
            LOG(6, "13 : Flicker cost [0~15]\n");
            LOG(6, "14 : Y internal cost 3D [0~15]\n");
            LOG(6, "15 : C internal cost 3D [0~15]\n");
            LOG(6, "16 : MC cost [0~15]\n");
            LOG(6, "17 : Smooth cost [0~15]\n");
            LOG(6, "18 : Y sawtooth cost [0~15]\n");
            LOG(6, "19 : C sawtooth cost [0~15]\n");
            LOG(6, "20 : 2D total cost Y [0~15]\n");
            LOG(6, "21 : 2D total cost C [0~15]\n");
            LOG(6, "22 : 3D total cost Y [0~15]\n");
            LOG(6, "23 : 3D total cost C [0~15]\n");    
            LOG(6, "24 : Current Pixel Weight [2~7]\n");
            LOG(6, "25 : 2D final cost Y [0~15]\n");
            LOG(6, "26 : 2D final cost C [0~15]\n");
            LOG(6, "27 : 3D final cost Y [0~15]\n");
            LOG(6, "28 : 3D final cost C [0~15]\n");                
            LOG(6, "29 :  Y blending factor [0: still, 1-7: still blend with motion, 8: motion, 9-11: MC blend with motion, 15: MC]\n");
            LOG(6, "30 : C blending factor [0~8]\n");

            vIO32WriteFldAlign(MCVP_DARE_07, SV_ON, DI_INK_EN);
            vIO32WriteFldAlign(MCVP_FUSION_1A, u2Type, IF_INK_SEL);
            
            /*vIO32WriteFldAlign(MCVP_DARE_08, 0x999999a8, INK_COLOR_Y_0);
            vIO32WriteFldAlign(MCVP_DARE_09, 0x66666688, INK_COLOR_Y_1);
            vIO32WriteFldAlign(MCVP_DARE_0A, 0x333333a8, INK_COLOR_CB_0);
            vIO32WriteFldAlign(MCVP_DARE_0B, 0xcccccc88, INK_COLOR_CB_1);
            vIO32WriteFldAlign(MCVP_DARE_0C, 0x22222218, INK_COLOR_CR_0);
            vIO32WriteFldAlign(MCVP_DARE_0D, 0xdddddd88, INK_COLOR_CR_1);*/


            if (u2Type == 29 )
            {
                vIO32WriteFldAlign(MCVP_DARE_08, 0x88888888, INK_COLOR_Y_0);
                vIO32WriteFldAlign(MCVP_DARE_09, 0x88888888, INK_COLOR_Y_1);
                vIO32WriteFldAlign(MCVP_DARE_0A, 0xfffffff8, INK_COLOR_CB_0);
                vIO32WriteFldAlign(MCVP_DARE_0B, 0x0000000f, INK_COLOR_CB_1);
                vIO32WriteFldAlign(MCVP_DARE_0C, 0xfffffff8, INK_COLOR_CR_0);
                vIO32WriteFldAlign(MCVP_DARE_0D, 0xfaaaaaa0, INK_COLOR_CR_1);
            }
            else
            {
                vIO32WriteFldAlign(MCVP_DARE_08, 0x88888888, INK_COLOR_Y_0);
                vIO32WriteFldAlign(MCVP_DARE_09, 0x88888888, INK_COLOR_Y_1);
                vIO32WriteFldAlign(MCVP_DARE_0A, 0xffffffff, INK_COLOR_CB_0);
                vIO32WriteFldAlign(MCVP_DARE_0B, 0x00000000, INK_COLOR_CB_1);
                vIO32WriteFldAlign(MCVP_DARE_0C, 0xff884422, INK_COLOR_CR_0);
                vIO32WriteFldAlign(MCVP_DARE_0D, 0xff884422, INK_COLOR_CR_1);
            }

            break;
            
        case 4: //DARE ink            
            LOG(6, "0 : Cont_Saw_Strength\n");
            LOG(6, "1 : Luma_Jaggy_Strength\n");
            LOG(6, "2 : Line_Flicker\n");
            LOG(6, "3 : Single_Saw_Strength\n");
            LOG(6, "4 : Final_Strength\n");

            vIO32WriteFldAlign(MCVP_DARE_07, SV_ON, DI_INK_EN);
            vIO32WriteFldAlign(MCVP_DARE_07, u2Type, DARE_INK_SEL);            
            vIO32WriteFldAlign(MCVP_DARE_08, 0x88888888, INK_COLOR_Y_0);
            vIO32WriteFldAlign(MCVP_DARE_09, 0x8888888f, INK_COLOR_Y_1);
            vIO32WriteFldAlign(MCVP_DARE_0A, 0xeca86428, INK_COLOR_CB_0);
            vIO32WriteFldAlign(MCVP_DARE_0B, 0xf, INK_COLOR_CB_1);
            vIO32WriteFldAlign(MCVP_DARE_0C, 0xeca86428, INK_COLOR_CR_0);
            vIO32WriteFldAlign(MCVP_DARE_0D, 0xf, INK_COLOR_CR_1);
            break;

        case 5: //SB ink            
            LOG(6, "0: LumaMotion\n");
            LOG(6, "1: ChromaMotion\n");
            LOG(6, "2: BlkMotion\n");
            LOG(6, "3: Saw\n");
            LOG(6, "4: PDSaw\n");
            LOG(6, "5: HMagLv\n");
            LOG(6, "6: HEdgeLv\n");
            LOG(6, "7: VEdgeLv\n");
            LOG(6, "8: motion_level_y\n");
            LOG(6, "9: mema_ink_out\n");
            LOG(6, "10: be_ink_out\n");
            LOG(6, "11: cs ink\n");
            LOG(6, "12: final_motion_level_y\n");
            LOG(6, "13: final_motion_level_c\n");
            LOG(6, "14: mc_wei_y\n");
            LOG(6, "15: mc_wei_c\n");

            vIO32WriteFldAlign(MCVP_SB_06, SV_ON, SB_INK_ENABLE);
            vIO32WriteFldAlign(MCVP_SB_06, u2Type, SB_INK_SELECT);
            break;
            
        default:
            break;
    }
#else
    UNUSED(bMode);
    UNUSED(u2Type);
#endif    
}

UINT32 u4DrvDIGetMotionLevel(void)
{
    UINT32 u4Motion = 0;
    static UINT32 u4PixCntPre = 0;
    static UINT32 u4PixCntDiv = 0;

    UINT32 u4PixCnt = PANEL_GetPanelWidth()*PANEL_GetPanelHeight();

    if(u4PixCnt != u4PixCntPre)
    {
        u4PixCntPre = u4PixCnt;
        u4PixCntDiv = (1<<28) / u4PixCnt;
    }

#if defined(CC_MT5399) || defined(CC_MT5882)
    if (IS_INTERLACE(VDP_1))
    {
        u4Motion = ((((DiSta.PdSta.u4FrmMotion+(1<<3))>>4)*u4PixCntDiv + (1<<7))>>8);
    }
    else
    {
	#ifdef CC_MT5399
		u4Motion = ((((IO32ReadFldAlign(MJCFD_0B, MJCFD_FD_DET_MOV_INFO)+(1<<3))>>4)*u4PixCntDiv + (1<<7))>>8);
	#else
		u4Motion = ((((DiSta.PdSta.u4FrmMotion+(1<<3))>>4)*u4PixCntDiv + (1<<7))>>8);
	#endif
    }

    if (DiSta.u4IsrIndex == 0)
    {
        LOG(5, "u4Motion = %d\n", u4Motion);
    }
#endif

    return u4Motion;
}

UINT32 u4DrvDIGetFusionMotionLevel(void)
{    
    if (IO32ReadFldAlign(MCVP_KC_19, FRC_REPEAT_FIELD)&&IS_INTERLACE(VDP_1)&&(!IS_FRAME_SEQ(VDP_1)))
    {
        return 0;        
    }
    else
    {
        return DiSta.IfSta.u4Homo;
    }
}

/*
 * @brief MDDi Film Mode Status polling loop
 * @param pvArgv Interrupt vector
 */
void vDrvDIFilmStatusPolling(void *pvArgv)
{
    INT32 i4Cnt;
    UINT32 pu4FilmModeFlag;
    UINT32 pu4FrameStaticSeq, pu4pFrameMotionSeq, pu4FrameSimilarSeq;
    UINT32 pu4FieldStatic22Seq, pu4FieldStatic32Seq;
    UINT32 pu4FieldMotion22Seq, pu4FieldMotion32Seq;
    UINT32 pu4FieldSimilarSeq;
    UINT32 pu4EdgeStatic22Seq, pu4EdgeStatic32Seq;
    UINT32 pu4EdgeMotion22Seq, pu4EdgeMotion32Seq;
    UINT32 pu4FrameMotion, pu4FieldMotion, pu4EdgeFieldMotion ;
    UINT32 pu4BlockMotion, pu4FilmFrameCB;
    UINT32 testloop = *(UINT32*)pvArgv;
    
    Printf("22 32 2E 3E  FR_MO  FI_MO  ED_MO FR_ST FR_MO FR_SI 32_FI_ST 32_FI_MO 32_ED_ST 32_ED_MO FI_SI 22_FI_ST 22_FI_MO 22_ED_ST 22_ED_MO  BL CB\n");
    Printf("== == == ==  =====  =====  ===== ===== ===== ===== ======== ======== ======== ======== ===== ======== ======== ======== ========  ==== ==");
    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvDIWaitDIVSyncSema();

        pu4FilmModeFlag = IO32ReadFldAlign(MCVP_STATUS_0F, FD_FLAG);
        pu4FrameMotion = IO32ReadFldAlign(MCVP_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
        pu4FieldMotion = IO32ReadFldAlign(MCVP_STATUS_10, FD_32_SAW_FI_A_B_SAW_Q);
        pu4EdgeFieldMotion = IO32ReadFldAlign(MCVP_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
        pu4FrameStaticSeq = IO32ReadFldAlign(MCVP_STATUS_08, FRAME_ST_SEQ_STATUS);
        pu4pFrameMotionSeq = IO32ReadFldAlign(MCVP_STATUS_09, FRAME_ST_SEQ_STATUS);
        pu4FrameSimilarSeq = IO32ReadFldAlign(MCVP_STATUS_0A, FRAME_SIM_SEQ_STATUS);
        pu4FieldStatic22Seq = IO32ReadFldAlign(MCVP_STATUS_0C, FIELD_ST_SEQ_22_STATUS);
        pu4FieldStatic32Seq = IO32ReadFldAlign(MCVP_STATUS_0C, FIELD_ST_SEQ_STATUS);
        pu4FieldMotion22Seq = IO32ReadFldAlign(MCVP_STATUS_29, FIELD_MO_SEQ_22_STATUS);
        pu4FieldMotion32Seq = IO32ReadFldAlign(MCVP_STATUS_27, FIELD_MO_SEQ_STATUS);
        pu4EdgeStatic22Seq = IO32ReadFldAlign(MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_22_STATUS);
        pu4EdgeStatic32Seq = IO32ReadFldAlign(MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_STATUS);
        pu4EdgeMotion22Seq = IO32ReadFldAlign(MCVP_STATUS_2A, NEW_FIELD_MO_SEQ_22_STATUS);
        pu4EdgeMotion32Seq = IO32ReadFldAlign(MCVP_STATUS_28, NEW_FIELD_MO_SEQ_STATUS);
        pu4FieldSimilarSeq = IO32ReadFldAlign(MCVP_STATUS_0D, FIELD_SIM_SEQ_STATUS);
        pu4BlockMotion = IO32ReadFldAlign(MCVP_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
        pu4FilmFrameCB = IO32ReadFldAlign(MCVP_STATUS_21, FILM_CB_STATUS);

        //22 detection
        Printf("\n%d  ", (pu4FilmModeFlag>>8) & 0x1);

        //32 detection
        Printf("%d  ", (pu4FilmModeFlag>>7) & 0x1);

        //22 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>0) & 0x7);

        //32 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>3) & 0x7);

        Printf("%6x %6x %6x ", pu4FrameMotion & 0x3fffff, pu4FieldMotion & 0x3fffff, pu4EdgeFieldMotion & 0x3fffff);

        //frame static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameStaticSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4pFrameMotionSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //Field Similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }

        Printf(" %4X  %d", (pu4BlockMotion) & 0xFFFF, (pu4FilmFrameCB) & 0x7);
    }

    Printf("\nPSCAN status polling thread END\n");
}

/*
 * @brief MDDi Film Mode Status polling loop
 * @param pvArgv Interrupt vector
 */
void vDrvDIHWFilmStatusPolling(void *pvArgv)
{
    INT32 i4Cnt;
    UINT32 pu4FilmModeFlag;
    UINT32 pu4FrameStaticSeq, pu4pFrameMotionSeq, pu4FrameSimilarSeq;
    UINT32 pu4FieldStatic22Seq, pu4FieldStatic32Seq;
    UINT32 pu4FieldMotion22Seq, pu4FieldMotion32Seq;
    UINT32 pu4FieldSimilarSeq;
    UINT32 pu4EdgeStatic22Seq, pu4EdgeStatic32Seq;
    UINT32 pu4EdgeMotion22Seq, pu4EdgeMotion32Seq;
    UINT32 pu4FrameMotion, pu4FieldMotion, pu4EdgeFieldMotion ;
    UINT32 pu4BlockMotion, pu4FilmFrameCB;
    UINT32 testloop = *(UINT32*)pvArgv;
    
    Printf("22 32 2E 3E  FR_MO  EG_TH  ED_MO FR_ST FR_MO FR_SI 32_FI_ST 32_FI_MO 32_ED_ST 32_ED_MO FI_SI 22_FI_ST 22_FI_MO 22_ED_ST 22_ED_MO  BL CB\n");
    Printf("== == == ==  =====  =====  ===== ===== ===== ===== ======== ======== ======== ======== ===== ======== ======== ======== ========  ==== ==");
    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvDIWaitDIVSyncSema();

        pu4FilmModeFlag = IO32ReadFldAlign(MCVP_STATUS_0F, FD_FLAG);
        pu4FrameMotion = IO32ReadFldAlign(MCVP_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
        pu4FieldMotion = DiPar.PdPar.u4AdaptiveEgMo32Th;
        pu4EdgeFieldMotion = IO32ReadFldAlign(MCVP_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
        pu4FrameStaticSeq = IO32ReadFldAlign(MCVP_STATUS_08, FRAME_ST_SEQ_STATUS);
        pu4pFrameMotionSeq = IO32ReadFldAlign(MCVP_STATUS_09, FRAME_ST_SEQ_STATUS);
        pu4FrameSimilarSeq = IO32ReadFldAlign(MCVP_STATUS_0A, FRAME_SIM_SEQ_STATUS);
        pu4FieldStatic22Seq = IO32ReadFldAlign(MCVP_STATUS_0C, FIELD_ST_SEQ_22_STATUS);
        pu4FieldStatic32Seq = IO32ReadFldAlign(MCVP_STATUS_0C, FIELD_ST_SEQ_STATUS);
        pu4FieldMotion22Seq = IO32ReadFldAlign(MCVP_STATUS_29, FIELD_MO_SEQ_22_STATUS);
        pu4FieldMotion32Seq = IO32ReadFldAlign(MCVP_STATUS_27, FIELD_MO_SEQ_STATUS);
        pu4EdgeStatic22Seq = IO32ReadFldAlign(MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_22_STATUS);
        pu4EdgeStatic32Seq = IO32ReadFldAlign(MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_STATUS);
        pu4EdgeMotion22Seq = IO32ReadFldAlign(MCVP_STATUS_2A, NEW_FIELD_MO_SEQ_22_STATUS);
        pu4EdgeMotion32Seq = IO32ReadFldAlign(MCVP_STATUS_28, NEW_FIELD_MO_SEQ_STATUS);
        pu4FieldSimilarSeq = IO32ReadFldAlign(MCVP_STATUS_0D, FIELD_SIM_SEQ_STATUS);
        pu4BlockMotion = IO32ReadFldAlign(MCVP_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
        pu4FilmFrameCB = IO32ReadFldAlign(MCVP_STATUS_21, FILM_CB_STATUS);

        //22 detection
        Printf("\n%d  ", (pu4FilmModeFlag>>8) & 0x1);

        //32 detection
        Printf("%d  ", (pu4FilmModeFlag>>7) & 0x1);

        //22 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>0) & 0x7);

        //32 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>3) & 0x7);

        Printf("%6x %6x %6x ", pu4FrameMotion & 0x3fffff, pu4FieldMotion & 0x3fffff, pu4EdgeFieldMotion & 0x3fffff);

        //frame static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameStaticSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4pFrameMotionSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //Field Similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }

        Printf(" %4X  %d", (pu4BlockMotion) & 0xFFFF, (pu4FilmFrameCB) & 0x7);
    }

    Printf("\nPSCAN status polling thread END\n");
}


/*
 * @brief MDDi Film Mode Status polling loop
 * @param pvArgv Interrupt vector
 */
void vDrvDIFilmChk(void *pvArgv)
{
    INT32 i4Cnt;
    UINT32 pu4FilmModeFlag;
    UINT32 pu4FrameStaticSeq, pu4pFrameMotionSeq, pu4FrameSimilarSeq;
    UINT32 pu4FieldStatic22Seq, pu4FieldStatic32Seq;
    UINT32 pu4FieldMotion22Seq, pu4FieldMotion32Seq;
    UINT32 pu4FieldSimilarSeq;
    UINT32 pu4EdgeStatic22Seq, pu4EdgeStatic32Seq;
    UINT32 pu4EdgeMotion22Seq, pu4EdgeMotion32Seq;
    UINT32 pu4FrameMotion, pu4FieldMotion, pu4EdgeFieldMotion ;
    UINT32 pu4BlockMotion, pu4FilmFrameCB;

    UINT32 u4LumaValue,u4CbValue,u4CrValue,u4PreLumaValue;
    static UINT32 u4KeyCnt;
    
    UINT32 testloop = *(UINT32*)pvArgv;                                                                          
    UINT32 u4FiMaskCur, u4FrMaskCur;
    
    Printf("KEY FR FI 22 32 2E 3E  FR_MO  FI_MO  ED_MO FR_ST FR_MO FR_SI 32_FI_ST 32_FI_MO 32_ED_ST 32_ED_MO FI_SI 22_FI_ST 22_FI_MO 22_ED_ST 22_ED_MO  BL CB\n");
    Printf("=== == == == == == ==  =====  =====  ===== ===== ===== ===== ======== ======== ======== ======== ===== ======== ======== ======== ========  ==== ==");
    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvDIWaitDIVSyncSema();

        u4LumaValue = IO32ReadFldAlign(MCVP_FUSION_16, IF_AVG_Y);
        u4CbValue = IO32ReadFldAlign(MCVP_FUSION_16, IF_AVG_CB);
        u4CrValue = IO32ReadFldAlign(MCVP_FUSION_16, IF_AVG_CR);
        u4PreLumaValue = IO32ReadFldAlign(MCVP_FUSION_17, IF_AVG_Y_PREV);

        if((u4CbValue==0x100)&&(u4CrValue==0x100)&&(u4LumaValue > 0x1A0)&&(u4LumaValue==u4PreLumaValue))
        {
            u4KeyCnt=MIN(u4KeyCnt+1,0xF);
        }
        else
        {
            u4KeyCnt=0;
        }

        u4FiMaskCur = (DiPar.PdPar.u4FldMask>>(DiPar.PdPar.u4FldBit-DiPar.PdPar.bFldCB))&FILM_MASK(DiPar.PdPar.u4FldBit);  
        u4FrMaskCur = (DiPar.PdPar.u4FrmMask>>(DiPar.PdPar.u4FldBit-DiPar.PdPar.bFrmCB))&FILM_MASK(DiPar.PdPar.u4FldBit);  

        //KEY 
        Printf("\n%3d  %c  %c ", u4KeyCnt>6,DiPar.PdPar.u4FrmBit?(u4FrMaskCur&1?'M':'S'):'N',
            DiPar.PdPar.u4FldBit?(u4FiMaskCur&1 ?'M':'S'):'N');
        
        pu4FilmModeFlag = IO32ReadFldAlign(MCVP_STATUS_0F, FD_FLAG);
        pu4FrameMotion = IO32ReadFldAlign(MCVP_STATUS_0F, OLD_FRAME_MO_Q_STATUS);
        pu4FieldMotion = IO32ReadFldAlign(MCVP_STATUS_10, FD_32_SAW_FI_A_B_SAW_Q);
        pu4EdgeFieldMotion = IO32ReadFldAlign(MCVP_STATUS_11, FD_32_EDGE_FI_A_B_SAW_Q);
        pu4FrameStaticSeq = IO32ReadFldAlign(MCVP_STATUS_08, FRAME_ST_SEQ_STATUS);
        pu4pFrameMotionSeq = IO32ReadFldAlign(MCVP_STATUS_09, FRAME_ST_SEQ_STATUS);
        pu4FrameSimilarSeq = IO32ReadFldAlign(MCVP_STATUS_0A, FRAME_SIM_SEQ_STATUS);
        pu4FieldStatic22Seq = IO32ReadFldAlign(MCVP_STATUS_0C, FIELD_ST_SEQ_22_STATUS);
        pu4FieldStatic32Seq = IO32ReadFldAlign(MCVP_STATUS_0C, FIELD_ST_SEQ_STATUS);
        pu4FieldMotion22Seq = IO32ReadFldAlign(MCVP_STATUS_29, FIELD_MO_SEQ_22_STATUS);
        pu4FieldMotion32Seq = IO32ReadFldAlign(MCVP_STATUS_27, FIELD_MO_SEQ_STATUS);
        pu4EdgeStatic22Seq = IO32ReadFldAlign(MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_22_STATUS);
        pu4EdgeStatic32Seq = IO32ReadFldAlign(MCVP_STATUS_0E, NEW_FIELD_ST_SEQ_STATUS);
        pu4EdgeMotion22Seq = IO32ReadFldAlign(MCVP_STATUS_2A, NEW_FIELD_MO_SEQ_22_STATUS);
        pu4EdgeMotion32Seq = IO32ReadFldAlign(MCVP_STATUS_28, NEW_FIELD_MO_SEQ_STATUS);
        pu4FieldSimilarSeq = IO32ReadFldAlign(MCVP_STATUS_0D, FIELD_SIM_SEQ_STATUS);
        pu4BlockMotion = IO32ReadFldAlign(MCVP_STATUS_17, BLOCK_MOTION_SUM_PRE_STATUS);
        pu4FilmFrameCB = IO32ReadFldAlign(MCVP_STATUS_21, FILM_CB_STATUS);

        //22 detection
        Printf("%d  ", (pu4FilmModeFlag>>8) & 0x1);

        //32 detection
        Printf("%d  ", (pu4FilmModeFlag>>7) & 0x1);

        //22 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>0) & 0x7);

        //32 exit flag
        Printf("%d  ", (pu4FilmModeFlag>>3) & 0x7);

        Printf("%6x %6x %6x ", pu4FrameMotion & 0x3fffff, pu4FieldMotion & 0x3fffff, pu4EdgeFieldMotion & 0x3fffff);

        //frame static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameStaticSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4pFrameMotionSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //frame similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FrameSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //32 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion32Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf(" ");

        //Field Similar
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldSimilarSeq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4FieldMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field static
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeStatic22Seq>>(i4Cnt - 1)) & 0x1);
        }
        Printf("    ");

        //22 Edge field Motion
        for (i4Cnt = 5; i4Cnt > 0; i4Cnt--)
        {
            Printf("%d", (pu4EdgeMotion22Seq>>(i4Cnt - 1)) & 0x1);
        }

        Printf(" %4X  %d", (pu4BlockMotion) & 0xFFFF, (pu4FilmFrameCB) & 0x7);
    }

    Printf("\nPSCAN status polling thread END\n");
}

void vDrvDIIFStatusPolling(void *pvArgv)
{
    UINT32 testloop = *(UINT32*)pvArgv;

    Printf("\nN HETER  DIF_HE  HOMO   DIF_HO  Hedge  Vedge \n");
    Printf("= ====== ======  ====== ======  ====== ======\n");

    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvDIWaitDIVSyncSema();        
        
        Printf("\n%1x %6x %6x %6x %6x %6x %6x ",
               DiSta.u4IsrIndex, 
               DiSta.IfSta.u4Heter, DiSta.IfSta.u4HeterDiff,
               DiSta.IfSta.u4Homo, DiSta.IfSta.u4HomoDiff,               
               DiSta.IfSta.u4HEdgeCnt, DiSta.IfSta.u4VEdgeCnt);
    }
}

void vDrvDIFwFilmStatusPolling(void *pvArgv)
{
    UINT32 i, testloop = *(UINT32*)pvArgv;
    DI_PD_PARAM *pPD_Par = &DiPar.PdPar;
    DI_PD_STA *pPD_Sta = &DiSta.PdSta;

    Printf("\nIdx [ FmMo Thld    Seq    N CB Cnt ][ FdMo Thld    Seq    B CB Cnt ][Blk BE We Unknown Acc] FmMask  FldMask ", 
        DiSta.u4IsrIndex, pPD_Sta->u4FrmMotion, pPD_Par->u4FrmThld);

    while ((fgDIStatusPolling) && (testloop-- != 0))
    {
        vDrvDIWaitDIVSyncSema();

        Printf("\n%x   [%5x %4x ", DiSta.u4IsrIndex, pPD_Sta->u4FrmMotion, pPD_Par->u4FrmThld);

        for (i = 8; i > 0; i--)
        {
            Printf("%d", (pPD_Par->u4FrmSeq>>(i - 1)) & 0x1);
        } 
        Printf("  %x  %x  %2x ]", pPD_Par->u4FrmBit, pPD_Par->bFrmCB, pPD_Par->bFrmCnt);
        Printf("[%5x %4x ", pPD_Sta->u4EdgMotion, pPD_Par->u4FldThld);

        for (i = 8; i > 0; i--)
        {
            Printf("%d", (pPD_Par->u4FldSeq>>(i - 1)) & 0x1);
        } 
        Printf("  %x  %x  %2x ]", pPD_Par->u4FldBit, pPD_Par->bFldCB, pPD_Par->bFldCnt);
        
        Printf("[ %2x %x %x  %2x  %2x ]  ", pPD_Sta->bBlkMotion, pPD_Par->bFwBadEdit, 
            pPD_Par->bFwWeave, pPD_Par->bUnknowCnt, pPD_Par->bFwAccCnt);
        
        for (i = MAX(pPD_Par->u4FrmBit, pPD_Par->u4FldBit); i > 0; i--)
        {
            Printf("%d", (pPD_Par->u4FrmMask>>(i - 1)) & 0x1);
        }  
        Printf(" ");        
        for (i = MAX(pPD_Par->u4FrmBit, pPD_Par->u4FldBit); i > 0; i--)
        {
            Printf("%d", (pPD_Par->u4FldMask>>(i - 1)) & 0x1);
        }           
    }
}


void vDrvDIMVStatusPolling(void *pvArgv)
{
    UINT32 testloop = *(UINT32*)pvArgv;
    INT32 i, j;
    
    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvDIWaitDIVSyncSema();  
    
        Printf("\n ======== MV[%d] Status ========\n", DiSta.u4IsrIndex);        

        if (DiSta.CsSta.u1MvReadValid)
        {
            for (i = -DI_CS_SEARCH_RANGE_X; i <= DI_CS_SEARCH_RANGE_X; i++)
            {
                for (j = -DI_CS_SEARCH_RANGE_Y; j <= DI_CS_SEARCH_RANGE_Y; j++)
                {
                    if (MV_HIST(i, j))
                    {
                         Printf("MV[%d, %d] = %d \n", i, j, MV_HIST(i, j));
                    }
                }   
            }
        }
        else
        {
            Printf("\nMV Hist Read fail \n");
        }

        Printf("NonZero  %d / HistDiff  %d  \n", DiSta.CsSta.u2NonZeroCnt, DiSta.CsSta.u4HistDiffAcc);
        Printf("Hist max %d / Total     %d  \n", DiSta.CsSta.u2HistMax, DiSta.CsSta.u2HistCnt);
        Printf("Edge     %d   CS state  %d  \n", DiSta.CsSta.u2EdgeCnt, DiPar.CsPar.bFavorCS);
    }
}        

void vDrvDIPEPolling(void *pvArgv)
{
    UINT32 i;    
    UINT32 testloop = *(UINT32*)pvArgv;
    
    Printf("N ALIAS EdgeSum   0    V   MH   PH   M1   P1   M2   P2   M4   P4   M7   P7   \n");
    Printf("= ===== ======= ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== ");

    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvDIWaitDIVSyncSema();

        Printf("\n%1x %4x %8x ", DiSta.u4IsrIndex, 
            DiSta.PeSta.u4AliasCnt, 
            DiSta.PeSta.u4EdgeSum);

        for (i = 0; i < DI_BE_HIST_BIN; i++)
        {
            Printf("%4x ", DiSta.PeSta.u4EdgeHis[i]);
        }
    }
}

void vDrvDICSPolling(void *pvArgv)
{    
    UINT32 testloop = *(UINT32*)pvArgv;
    
    Printf("N AllBurst EdgCnt HisCnt HisMax SMT SMT_ROI  SMT_Deconf MV_Deconf\n");
    Printf("= ======== ====== ====== ====== === =======  ========== =========\n");

    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvDIWaitDIVSyncSema();        
        
        Printf("%1x   %1x    %4x  [%4x  %4x]  [%4x  %4x]  [%1x  %1x] \n", 
            DiSta.u4IsrIndex,
            DiSta.CsSta.bAllBurst,
            DiSta.CsSta.u2EdgeCnt,
            DiSta.CsSta.u2HistCnt,
            DiSta.CsSta.u2HistMax,
            DiSta.CsSta.u2SmtCnt,
            DiSta.CsSta.u2SmtCntROI,
            DiSta.CsSta.u1SmtDeconf,
            DiSta.CsSta.u1MvDeconf);
    }
}

void vDrvDIAdaptivePolling(void *pvArgv)
{    
    UINT32 testloop = *(UINT32*)pvArgv;
    
    Printf("N u4SawAB  u4SawBC SawRank SawNorm SawInv  Ratio\n");
    Printf("= ======== ======= ======= ======= ======= ======= \n");
    
    while ((fgDIStatusPolling) && (testloop-- !=0))
    {
        vDrvDIWaitDIVSyncSema();

        Printf("\n%1x %08x %08x %08x ", 
            DiSta.u4IsrIndex,
            DiSta.CsSta.u4SawAB,
            DiSta.CsSta.u4SawBC,
            DiSta.CsSta.u4FieldSawRank);
        
        Printf("%d  %d  ", DiSta.IfSta.u4SawNom, DiSta.IfSta.u4SawInv);

        if (DiSta.IfSta.u4SawNom)
        {
            Printf(" %4x", (DiSta.IfSta.u4SawNom<<8)/DiSta.IfSta.u4SawInv);
        }
    }
}


DI_PQ_THREAD DIPQThread[] =
{
    {vDrvDIIFStatusPolling,         "Fusion Status"},
    {vDrvDIFwFilmStatusPolling,     "FW FILM"},
    {vDrvDIFilmStatusPolling,       "HW FILM"},
    {vDrvDIHWFilmStatusPolling,     "NEW HW FILM"},    
    {vDrvDIFilmChk,                 "FILM Check"},    
    {vDrvDIMVStatusPolling,         "MV Hist"},
    {vDrvDIPEPolling,               "PE Status"},
    {vDrvDICSPolling,               "CS Status"},
    {vDrvDIAdaptivePolling,         "Adaptive FW Status"},
    {NULL                       ,NULL}//End Of Function Table
};

