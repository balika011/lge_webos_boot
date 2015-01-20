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
 * $Date: 2015/01/20 $
 * $RCSfile: drv_nr.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file NR driver
 *
 * @author mf_tien@mtk.com.tw
 * @function entry point
 *  vDrvNRInit
 *  vDrvNRSetMode
 * @user case
 *  power on -> vDrvNRInit
 *  mode change done -> vDrvNRSetMode
 *  user menu -> vDrvNRSetMode
 */

/**************************************************************************
 * Inclusions
 *************************************************************************/
#include "sv_const.h"
#include "video_def.h"
#include "video_timing.h"

#include "drv_nr.h"
#include "drv_di.h"
#include "drv_nr_int.h"
#include "drv_bnr_int.h"
#include "drv_tdtv_drvif.h"
#include "fbm_drvif.h"
#include "drv_display.h"


#include "hw_vdoin.h"
#include "hw_nr.h"
#include "hw_di.h"
//#include "hw_sw.h"

#include "vdo_misc.h"

#include "nr_debug.h"

#include "pe_if.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_hal_5381.h"

#define NR_THREAD_CREATE

#define _P2D(item) P_Fld(wReadQualityTable(QUALITY_2DNR_##item), item)
#define _P3D(item) P_Fld(wReadQualityTable(QUALITY_3DNR_##item), item)
#define _CLIP(val, min, max) ((val>=max) ? max : ((val<=min) ? min : val))

#ifdef CC_SUPPORT_STR
#define  vDrvMibReset(x)         (vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG1, x , FLD_MIB_RST))
#define  vDrvMibSetClock(x)      (vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG1, x , FLD_MIB_CKEN))
#endif


////////////////////////////////////////////////////////////////////////////////
// Register table
////////////////////////////////////////////////////////////////////////////////
enum
{
    E_MCNR_SD_I         = 0,
    E_MCNR_SD_P         = 1,
    E_MCNR_HD           = 2,
    E_MCNR_FHD_I        = 3,
    E_MCNR_FHD_P        = 4,
    E_MCNR_NUM_TIMINGS  = 5
};

enum
{
    E_NXNR_SD           = 0,
    E_NXNR_HD           = 1,
    E_NXNR_FHD          = 2,
    E_NXNR_NUM_TIMINGS  = 3
};

const REGTBL_T CODE _MCNR_PQ_COMMON[] =
{
    {MCNR_00,    0x0000003C, 0x0000007C},       // MCNR_DCNR_STRENGTH = 8
#if !PQL_DB_ON
    {MCNR_09,    0xF83E0F80, 0xFFFFFF80},       // MCNR_DC_MODE_WEI_Y_BASE = 0xF, DIFF_THD_Y_BASE = 4, SLOP_Y = 0, MAX_WEI = 0x1E
#endif
    {MCNR_0B,    0x000024C0, 0x00003FFC},
    {MCNR_0C,    0xB1E59030, 0xFFFFFFFC},
    {MCNR_0D,    0x14000000, 0xFC000000},
    {REGTBL_END, 0x00000000, 0x00000000}
};

const REGTBL_T CODE _MCNR_PQ_PARAM[E_MCNR_NUM_TIMINGS][5] =
{
    {   // SD interlaced (480i/576i)
    #if PQL_DB_ON
        {MCNR_03,    0x01800060, 0xFC00007E},
    #else
        {MCNR_03,    0x01800060, 0xFF80007E},   // MCNR_MA_MOVINGTXR_REFINED_STILL_LVL = 1
    #endif    
        {MCNR_08,    0x02000000, 0x07800000},   // MCNR_TXTR_MC_THD = 4
        {MCNR_31,    0x00500000, 0x81F00000},   // MCNR_MV_REFINE_NEI_COST_NL = 5
        {MCNR_32,    0xB8800000, 0xFE800000},
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // SD progressive (480p/576p)
    #if PQL_DB_ON
        {MCNR_03,    0x00000020, 0xFC00007E},
    #else
        {MCNR_03,    0x00000020, 0xFF80007E},   // MCNR_MA_MOVINGTXR_REFINED_STILL_LVL = 0
    #endif    
        {MCNR_08,    0x04000000, 0x07800000},   // MCNR_TXTR_MC_THD = 4
        {MCNR_31,    0x00500000, 0x81F00000},   //  MCNR_MV_REFINE_NEI_COST_NL = 5
        {MCNR_32,    0x38000000, 0xFE800000},
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // HD 720p
    #if PQL_DB_ON
        {MCNR_03,    0x00000020, 0xFC00007E},
    #else
        {MCNR_03,    0x00000020, 0xFF80007E},   // MCNR_MA_MOVINGTXR_REFINED_STILL_LVL = 0
    #endif    
        {MCNR_08,    0x04000000, 0x07800000},   // MCNR_TXTR_MC_THD = 4
        {MCNR_31,    0x00000000, 0x81F00000},   // MCNR_MV_REFINE_NEI_COST_NL = 0
        {MCNR_32,    0x38000000, 0xFE800000},
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // FHD interlaced (1080i)
    #if PQL_DB_ON
        {MCNR_03,    0x01800060, 0xFC00007E},
    #else
        {MCNR_03,    0x01800060, 0xFF80007E},   // MCNR_MA_MOVINGTXR_REFINED_STILL_LVL = 1
    #endif    
        {MCNR_08,    0x04000000, 0x07800000},   // MCNR_TXTR_MC_THD = 4
        {MCNR_31,    0x00B00000, 0x81F00000},   // MCNR_MV_REFINE_NEI_COST_NL = 3, MCNR_MV_REFINE_NEI_COST_TXTR_COR_RATE = 1
        {MCNR_32,    0x38800000, 0xFE800000},
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // FHD progressive (1080p)
    #if PQL_DB_ON
        {MCNR_03,    0x00000030, 0xFC00007E},
    #else
        {MCNR_03,    0x00000030, 0xFF80007E},   // MCNR_MA_MOVINGTXR_REFINED_STILL_LVL = 0
    #endif    
        {MCNR_08,    0x04000000, 0x07800000},   // MCNR_TXTR_MC_THD = 4
        {MCNR_31,    0x00B00000, 0x81F00000},   // MCNR_MV_REFINE_NEI_COST_NL = 3, MCNR_MV_REFINE_NEI_COST_TXTR_COR_RATE = 1
        {MCNR_32,    0x38000000, 0xFE800000},
        {REGTBL_END, 0x00000000, 0x00000000}
    }
};

const REGTBL_T CODE _NXNR_PQ_COMMON[] =
{
    {NXNR_05, 0x0CF44000, 0x0FFFF000},

    {NXNR_06, 0x00007020, 0xFFFFFFFF},  // only detect size = 8, 15, 16, 17
    {NXNR_07, 0x00007020, 0xFFFFFFFF},
    {NXNR_08, 0x00000000, 0xFFFFFFFF},

    {NXNR_0A, 0x40402020, 0xFFFFFFFF},
    {NXNR_0B, 0xA2202D2D, 0xFFFFFFFF},

    {NXNR_12, 0x7F000000, 0x7F000000}, // flat th = 0x7F

    {NXNR_3E, 0x80000000, 0xFFFF0000}, // force_rnr_th_en = 1, force_rnr_th_con = 0

    {REGTBL_END, 0x00000000, 0x00000000}
};

const REGTBL_T CODE _NXNR_PQ_PARAM[E_NXNR_NUM_TIMINGS][1] =
{
    {   // SD
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // HD 720p
        {REGTBL_END, 0x00000000, 0x00000000}
    },
    {   // FHD
        {REGTBL_END, 0x00000000, 0x00000000}
    }
};

const REGTBL_T CODE _DNR_PQ_COMMON[] =
{
	{MCNR_28, 0x60000000, 0x70000000},
	{MCNR_38, 0x8A043F08, 0x9F1F7F1F},
	{MCNR_39, 0x33643360, 0xF7F777F7},
	{MCNR_3A, 0xA0191040, 0xFFFF77FF},
	#ifdef CC_MT5398
	{MCNR_3B, 0x00060A00, 0x001F3F00},
	#else
	{MCNR_3B, 0x80060A00, 0xC01F3F00},
	#endif
	{NXNR_38, 0xA0103F20, 0xBF3FFF3F},
	{NXNR_39, 0x0000C446, 0x800FF77F},
    {REGTBL_END, 0x00000000, 0x00000000}
};

//NR global parameter
NR_PARAM_T _rNrPrm;
HANDLE_T hNRPQIsrSema = (HANDLE_T)NULL;

////////////////////////////////////////////////////////////////////////////////
// external functions
////////////////////////////////////////////////////////////////////////////////
EXTERN void vDrvDISetInkColor(UINT32 u4Idx, UINT32 u4Color);
EXTERN void vDrvNRNoiseLevelProc(void);
EXTERN void vDrvNRPrintNMStatus(void);

EXTERN UINT8 bForceAllPQOff;
////////////////////////////////////////////////////////////////////////////////
// static function for internal use
////////////////////////////////////////////////////////////////////////////////

static void _vDrvNRSetBypass(BOOL fgBypass)
{
    NR_W(MCNR_00, fgBypass, MCNR_BYPASS);
    NR_W(NXNR_00, fgBypass, NX_BYPASS);
}

////////////////////////////////////////////////////////////////////////////////
//NR Process
////////////////////////////////////////////////////////////////////////////////
void vDrvNRSet3DNRMode(E_3DNR_MODE e3DMode)
{
    switch (e3DMode)
    {
        case MCNR_MODE_MC:
        case MCNR_MODE_MV:
        case MCNR_MODE_NM:
        case MCNR_MODE_MA:
            NR_WM(MCNR_00, /*P_Fld(_rNrPrm.u1MCNROnOff, MCNR_ENABLE) |*/ P_Fld(((UINT8)e3DMode-1), MCNR_MODE));
            break;
        case MCNR_MODE_OFF:
            default:
            NR_WM(MCNR_00, /*P_Fld(0, MCNR_ENABLE) |*/ P_Fld(0, MCNR_MODE));
            //NR_WM(MCNR_11, P_Fld(0, MCNR_FORCE_ALPHA_MC_EN)|P_Fld(0, MCNR_FORCE_ALPHA_MC));
            //NR_WM(MCNR_10, P_Fld(0, MCNR_FORCE_NOISE_LVL_EN)|P_Fld(0, MCNR_FORCE_NOISE_LVL));
            break;

    }

    _rNrPrm.eMCNRMode = e3DMode;
}

E_3DNR_MODE eDrvNRGet3DNRMode(void)
{
    return _rNrPrm.eMCNRMode;
}

void vDrvNRSet2DNRMode(E_2DNR_MODE e2DMode)
{
#ifdef __MODEL_slt__
    //LOG(0, "[SLT] 5398A NX_BYPASS = 0, vDrvNRSet2DNRMode\n");
    if (IC_VER_5398_AA == BSP_GetIcVersion())//5398A
    {
        e2DMode = 0;
    }
#endif

    UINT32 u4NXNROn = (e2DMode == NXNR_MODE_ON) ? 1 : 0;
    NR_WM(NXNR_00, P_Fld(u4NXNROn, NX_NR_EN)|P_Fld((!u4NXNROn), NX_BYPASS));

    _rNrPrm.eNXNRMode = e2DMode;
}

void vDrvNRSetChromaUpSample(BOOL fgOnOff)
{
    NR_W(NXNR_00, fgOnOff, NX_CHROMA_UP_EN);
}

void vDrvNRSetInk(UINT8 u1Mode, UINT8 u1Type)
{
    UINT32 u4InkLvlShf = 0;

    LOG(3, "MCNR Ink Mode: 0) Ink Off , 1) MCNR MV, 2) 64 lvl, 3) 16 lvl, 4) 8 lvl\n");

    if (u1Type > 22 && u1Type != 0xFF)
    {
        LOG(3, "NR Ink type error: u1Type = %d\n", u1Type);
        return;
    }

    switch (u1Mode)
    {
    default:
    case 0:
        NR_WM(MCNR_15, P_Fld(0, MCNR_INK_EN)|P_Fld(0, MCNR_INK_SEL)|P_Fld(u4InkLvlShf, MCNR_INK_LEVEL_SHIFT));
        return;
    case 1: // MV ink
        LOG(3, "11: raw_mv\n");
        LOG(3, "12: flt_refined_mv\n");
        LOG(3, "13: pmv\n");
        vDrvDISetInkColor(0, 0x88888888);
        vDrvDISetInkColor(1, 0x88888888);
        vDrvDISetInkColor(2, 0x40FC8408);
        vDrvDISetInkColor(3, 0xFC840FC8);
        vDrvDISetInkColor(4, 0x77CCCCC7);
        vDrvDISetInkColor(5, 0x00000777);
        break;
    case 2: // 0~64 level ink
        LOG(3, "1: weighting_y_mc_proc\n");
        LOG(3, "2: weighting_y_ma_proc\n");
        LOG(3, "3: weighting_y_blending\n");
        LOG(3, "4: weighting_c_adj\n");
        LOG(3, "10: motion_txt_lvl\n");
        vDrvDISetInkColor(0, 0x88888888);
        vDrvDISetInkColor(1, 0x88888888);
        vDrvDISetInkColor(2, 0xAA998877);
        vDrvDISetInkColor(3, 0xFEDDCCBB);
        vDrvDISetInkColor(4, 0xAA998877);
        vDrvDISetInkColor(5, 0xFEDDCCBB);
        u4InkLvlShf = 2;
        break;
    case 3: // 0~15 level ink
        LOG(3, "14: pmv_max_cnt1\n");
        LOG(3, "15: pmv_max_cnt2\n");
        LOG(3, "19: ma_noise_txtur_lvl\n");
        LOG(3, "20: ma_prtrbtn_lvl\n");
        LOG(3, "21: mc_noise_txtur_lvl\n");
        LOG(3, "22: mc_lmv_lvl\n");
        vDrvDISetInkColor(0, 0x88888888);
        vDrvDISetInkColor(1, 0x88888888);
        vDrvDISetInkColor(2, 0xAA998877);
        vDrvDISetInkColor(3, 0xFEDDCCBB);
        vDrvDISetInkColor(4, 0xAA998877);
        vDrvDISetInkColor(5, 0xFEDDCCBB);
        break;
    case 4: // 0~8 level ink
        LOG(3, "0: alpha_mc\n");
        LOG(3, "5: ts_ratio_y\n");
        LOG(3, "6: ts_ratio_c\n");
        LOG(3, "7: ts_ratio\n");
        LOG(3, "8: still_level\n");
        LOG(3, "9: non_smooth_level\n");
        LOG(3, "16: rd_still_level\n");
        LOG(3, "17: mv_still_level\n");
        LOG(3, "18: mv_confidence\n");

        vDrvDISetInkColor(0, 0x88888888);
        vDrvDISetInkColor(1, 0x88888888);
        vDrvDISetInkColor(2, 0xEDCBA987);
        vDrvDISetInkColor(3, 0xFFFFFFFF);
        vDrvDISetInkColor(4, 0xEDCBA987);
        vDrvDISetInkColor(5, 0xFFFFFFFF);
        break;
    }

    if (u1Type == 0xFF)
    {
        NR_WM(MCNR_15, P_Fld(0, MCNR_INK_EN)|P_Fld(0, MCNR_INK_SEL)|P_Fld(0, MCNR_INK_LEVEL_SHIFT));
    }
    else
    {
        NR_WM(MCNR_15, P_Fld(1, MCNR_INK_EN)|P_Fld(u1Type, MCNR_INK_SEL)|P_Fld(u4InkLvlShf, MCNR_INK_LEVEL_SHIFT));
    }
}

/**
* @brief Set chroma line average mode register.
* @param bPath VDP_1/ VDP_2
* @param bMode 	NR_CHROMA_LINE_AVERAGE_MODE_OFF/ NR_CHROMA_LINE_AVERAGE_MODE_UP/
*           	NR_CHROMA_LINE_AVERAGE_MODE_DOWN/ NR_CHROMA_LINE_AVERAGE_MODE_121
*/
void vDrvNRSetChromaLineAvgModeReg(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode)
{
    UNUSED(bPath);

    //Only turn on chroma line average under TVD source
    if(!fgIsMainTvd3d()||IS_3D_FMT_CONV(bPath))
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_OFF, PAL_C_AVG_EN);
        return;
    }

    //5365 use only 121 chroma line average.
    if(bMode == NR_CHROMA_LINE_AVERAGE_MODE_121)
    {
        //121
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_ON, PAL_C_AVG_EN);
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_2F, SV_ON, PAL_C_AVG_HALF);
    }
    else if(bMode == NR_CHROMA_LINE_AVERAGE_MODE_DYN)
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_ON, PAL_C_AVG_EN);
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_2F, SV_OFF, PAL_C_AVG_HALF);

    }
    else//NR_CHROMA_LINE_AVERAGE_MODE_OFF
    {
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_OFF, PAL_C_AVG_EN);
    }    
}

/**
* @brief config chroma line average mode.
* @param bPath VDP_1/ VDP_2
*/
void vDrvNRChromaLineAvgCfg(UINT8 bPath)
{
#if defined(CC_FPGA_SUPPORT)
    MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_OFF, PAL_C_AVG_EN);
    return;
#endif

    if (bPath == VDP_2)
    {
        return;
    }
    else if ((SV_VD_TVD3D != _arMDDiPrm[VDP_1].u1DecType) || (bDrvVideoGetSourceTypeTiming(bPath)==SOURCE_TYPE_TIMING_SCART_RGB) ||
        IS_3D_FMT_CONV(VDP_1) || MDDI_READ_FLD(VDP_1, MCVP_DRAM_09, DA_DRAM_LINE_LENGTH) > 3)
    {// Decoder type of SCART RGB is TVD, so turn off line avg when scart rgb.
        MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_OFF, PAL_C_AVG_EN);
        return;
    }

    switch((UINT8)bHwTvdMode())
    {
        case SV_CS_PAL_N:
        case SV_CS_PAL:
        case SV_CS_PAL_M:
        case SV_CS_PAL_60:
            MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_ON, PAL_C_AVG_EN);
            MDDI_WRITE_FLD(VDP_1, MCVP_KC_2F, SV_OFF, PAL_C_AVG_HALF);
            break;

        default://NTSC358, NTSC443 Secam
            MDDI_WRITE_FLD(VDP_1, MCVP_KC_00, SV_OFF, PAL_C_AVG_EN);
            break;
    }
    
}


UINT8 u1DrvNRGetNoiseMeterSupport(void)
{
    UINT32 u4VdoTiming = bDrvVideoGetTiming(SV_VP_MAIN);
    UINT8 u1Ret = 0;

    switch (u4VdoTiming)
    {
    case MODE_525I_OVERSAMPLE:
    case MODE_625I_OVERSAMPLE:
    case MODE_480P_OVERSAMPLE:
    case MODE_576P_OVERSAMPLE:
    case MODE_525I:
    case MODE_625I:
    case MODE_480P:
    case MODE_576P:
    case MODE_480P_24:
    case MODE_480P_30:
    case MODE_576P_25:
        u1Ret = NR_NM_SAD_HIST;
        break;
    case MODE_720p_24:
    case MODE_720p_25:
    case MODE_720p_30:
    case MODE_720p_50:
    case MODE_720p_60:
        u1Ret = NR_NM_SAD_HIST;
        break;
    case MODE_1080i_48:
    case MODE_1080i_50:
    case MODE_1080i:
        u1Ret = NR_NM_SAD_HIST; //NR_NM_TDF_BASE;
        break;
    case MODE_1080p_24:
    case MODE_1080p_25:
    case MODE_1080p_30:
    case MODE_1080p_50:
    case MODE_1080p_60:
        u1Ret = NR_NM_SAD_HIST; //NR_NM_TDF_BASE;
        break;
    default:
        u1Ret = 0;
    }

    return u1Ret;
}

UINT8 u1DrvNRGetBlockMeterSupport(void)
{
    return 1;
}

void vDrvNRSetBlkMeterParam(E_BNM_MODE eBnmMode)
{
#ifndef BNR_NEW_FW
    UINT32 u4MinVBlkLineSum = _rNrPrm.u2Height / 8;
    UINT32 u4MinHBlkLineSum = _rNrPrm.u2Width / 8;
    UINT32 u4ArtiRejTh = 64;
    UINT32 u4MaxRng = 3;
    switch (eBnmMode)
    {
    case E_BNM_CVBS:    // himedia CVBS, 480i
        u4MinVBlkLineSum = 8;
        u4ArtiRejTh = 0xC0;
        u4MaxRng =1;
        _SWW(NR_FWBNR_00,   9, NR_FWBNR_VGRIDACTIVETH);
        _SWW(NR_FWBNR_00,   3, NR_FWBNR_VGRIDINACTIVETH);
        _SWW(NR_FWBNR_01, 128, NR_FWBNR_VBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_01,  64, NR_FWBNR_VBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_01,  64, NR_FWBNR_VBLKLEVELLEAKAGE);
        _SWW(NR_FWBNR_02,  6, NR_FWBNR_VBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_02,  20, NR_FWBNR_VBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_02,   2, NR_FWBNR_VBLKGAUGEINCTH);
        _SWW(NR_FWBNR_02,   3, NR_FWBNR_VBLKWINDOWTRIALCOUNTTH);
        _SWW(NR_FWBNR_02,  32, NR_FWBNR_VBLKSCORETH);
        _SWW(NR_FWBNR_00,   1, NR_FWBNR_VBLK_IGNORE_NW);

        _SWW(NR_FWBNR_03,  12, NR_FWBNR_HGRIDACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HGRIDINACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_03,  20, NR_FWBNR_HBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_04, 512, NR_FWBNR_HBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_04, 256, NR_FWBNR_HBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_04,  64, NR_FWBNR_HBLKLEVELLEAKAGE);
        break;
    case E_BNM_COMP_I:  // pioneer component, 480i oversample drop
        _SWW(NR_FWBNR_00,  12, NR_FWBNR_VGRIDACTIVETH);
        _SWW(NR_FWBNR_00,   4, NR_FWBNR_VGRIDINACTIVETH);
        _SWW(NR_FWBNR_01, 512, NR_FWBNR_VBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_01, 256, NR_FWBNR_VBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_01,  64, NR_FWBNR_VBLKLEVELLEAKAGE);
        _SWW(NR_FWBNR_02,   4, NR_FWBNR_VBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_02,  20, NR_FWBNR_VBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_02,   2, NR_FWBNR_VBLKGAUGEINCTH);
        _SWW(NR_FWBNR_02,   5, NR_FWBNR_VBLKWINDOWTRIALCOUNTTH);
        _SWW(NR_FWBNR_02,  32, NR_FWBNR_VBLKSCORETH);
        _SWW(NR_FWBNR_00,   1, NR_FWBNR_VBLK_IGNORE_NW);

        _SWW(NR_FWBNR_03,  12, NR_FWBNR_HGRIDACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HGRIDINACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_03,  20, NR_FWBNR_HBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_04, 512, NR_FWBNR_HBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_04, 256, NR_FWBNR_HBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_04,  64, NR_FWBNR_HBLKLEVELLEAKAGE);
        break;
    case E_BNM_COMP_P:  // pioneer component, 480p oversample drop
        _SWW(NR_FWBNR_00,  12, NR_FWBNR_VGRIDACTIVETH);
        _SWW(NR_FWBNR_00,   4, NR_FWBNR_VGRIDINACTIVETH);
        _SWW(NR_FWBNR_01, 256, NR_FWBNR_VBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_01, 128, NR_FWBNR_VBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_01,  32, NR_FWBNR_VBLKLEVELLEAKAGE);
        _SWW(NR_FWBNR_02,   8, NR_FWBNR_VBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_02,  20, NR_FWBNR_VBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_02,   2, NR_FWBNR_VBLKGAUGEINCTH);
        _SWW(NR_FWBNR_02,   5, NR_FWBNR_VBLKWINDOWTRIALCOUNTTH);
        _SWW(NR_FWBNR_02,  32, NR_FWBNR_VBLKSCORETH);
        _SWW(NR_FWBNR_00,   1, NR_FWBNR_VBLK_IGNORE_NW);

        _SWW(NR_FWBNR_03,  12, NR_FWBNR_HGRIDACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HGRIDINACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_03,  20, NR_FWBNR_HBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_04, 512, NR_FWBNR_HBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_04, 256, NR_FWBNR_HBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_04,  64, NR_FWBNR_HBLKLEVELLEAKAGE);
        break;
    case E_BNM_HDMI_I:
        u4MinVBlkLineSum = 32;
        u4ArtiRejTh = 0xC0;
        u4MaxRng =1;
        _SWW(NR_FWBNR_00,  12, NR_FWBNR_VGRIDACTIVETH);
        _SWW(NR_FWBNR_00,   4, NR_FWBNR_VGRIDINACTIVETH);
        _SWW(NR_FWBNR_01,  64, NR_FWBNR_VBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_01,  32, NR_FWBNR_VBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_01,  64, NR_FWBNR_VBLKLEVELLEAKAGE);
        _SWW(NR_FWBNR_02,   6, NR_FWBNR_VBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_02,  20, NR_FWBNR_VBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_02,   2, NR_FWBNR_VBLKGAUGEINCTH);
        _SWW(NR_FWBNR_02,   3, NR_FWBNR_VBLKWINDOWTRIALCOUNTTH);
        _SWW(NR_FWBNR_02,  32, NR_FWBNR_VBLKSCORETH);
        _SWW(NR_FWBNR_00,   1, NR_FWBNR_VBLK_IGNORE_NW);

        _SWW(NR_FWBNR_03,  12, NR_FWBNR_HGRIDACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HGRIDINACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_03,  20, NR_FWBNR_HBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_04, 512, NR_FWBNR_HBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_04, 256, NR_FWBNR_HBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_04,  64, NR_FWBNR_HBLKLEVELLEAKAGE);
        break;
    case E_BNM_HDMI_P:
        u4MinVBlkLineSum = 8;
        u4ArtiRejTh = 0xC0;
        u4MaxRng =1;
        _SWW(NR_FWBNR_00,  12, NR_FWBNR_VGRIDACTIVETH);
        _SWW(NR_FWBNR_00,   4, NR_FWBNR_VGRIDINACTIVETH);
        _SWW(NR_FWBNR_01,  64, NR_FWBNR_VBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_01,  32, NR_FWBNR_VBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_01,  64, NR_FWBNR_VBLKLEVELLEAKAGE);
        _SWW(NR_FWBNR_02,   6, NR_FWBNR_VBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_02,  20, NR_FWBNR_VBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_02,   2, NR_FWBNR_VBLKGAUGEINCTH);
        _SWW(NR_FWBNR_02,   3, NR_FWBNR_VBLKWINDOWTRIALCOUNTTH);
        _SWW(NR_FWBNR_02,  32, NR_FWBNR_VBLKSCORETH);
        _SWW(NR_FWBNR_00,   1, NR_FWBNR_VBLK_IGNORE_NW);

        _SWW(NR_FWBNR_03,  12, NR_FWBNR_HGRIDACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HGRIDINACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_03,  20, NR_FWBNR_HBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_04, 512, NR_FWBNR_HBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_04, 256, NR_FWBNR_HBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_04,  64, NR_FWBNR_HBLKLEVELLEAKAGE);
        break;
    case E_BNM_DFT:
    default:
        _SWW(NR_FWBNR_00,  12, NR_FWBNR_VGRIDACTIVETH);
        _SWW(NR_FWBNR_00,   4, NR_FWBNR_VGRIDINACTIVETH);
        _SWW(NR_FWBNR_01, 512, NR_FWBNR_VBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_01, 256, NR_FWBNR_VBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_01,  64, NR_FWBNR_VBLKLEVELLEAKAGE);
        _SWW(NR_FWBNR_02,   4, NR_FWBNR_VBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_02,  20, NR_FWBNR_VBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_02,   2, NR_FWBNR_VBLKGAUGEINCTH);
        _SWW(NR_FWBNR_02,   3, NR_FWBNR_VBLKWINDOWTRIALCOUNTTH);
        _SWW(NR_FWBNR_02,  32, NR_FWBNR_VBLKSCORETH);
        _SWW(NR_FWBNR_00,   1, NR_FWBNR_VBLK_IGNORE_NW);

        _SWW(NR_FWBNR_03,  12, NR_FWBNR_HGRIDACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HGRIDINACTIVETH);
        _SWW(NR_FWBNR_03,   4, NR_FWBNR_HBLKINSIDEOUTSIDERATIO);
        _SWW(NR_FWBNR_03,  20, NR_FWBNR_HBLKACTIVETRIALTH);
        _SWW(NR_FWBNR_04, 512, NR_FWBNR_HBLKLEVELACTIVETH);
        _SWW(NR_FWBNR_04, 256, NR_FWBNR_HBLKLEVELCONFIRMTH);
        _SWW(NR_FWBNR_04,  64, NR_FWBNR_HBLKLEVELLEAKAGE);
        break;
    }

    if (!_SWR(NR_FWBNR_00, NR_FWBNR_USE_HW_MINLS_TH))
    {
        _SWW(NR_FWBNR_00, u4MinVBlkLineSum, NR_FWBNR_VGRIDMINLINESUMTH);
        _SWW(NR_FWBNR_03, u4MinHBlkLineSum, NR_FWBNR_HGRIDMINLINESUMTH);
    }

    NR_W(NXNR_09, u4ArtiRejTh, NX_VBLK_ARTI_REJ_TH);
    NR_W(NXNR_05, u4MaxRng, NX_VBLK_MAX_RANGE);
#endif  // BNR_NEW_FW
}

void vDrvNRSetMOANRParam(void)
{
#if 0   //move to PQ init
    _SWW(NR_MO_00,    0x02, MONR_MOTION_LEVEL_TH1);
    _SWW(NR_MO_00,    0x20, MONR_MOTION_LEVEL_TH2);
    _SWW(NR_MO_01,    0xF8, MONR_IIR_FACTOR);
    
    _SWW(NR_AUTO_02, 0x14, MOTNR_MAX_NL_TH);
    _SWW(NR_AUTO_02, 0x08, MOTNR_MIN_NL_TH);
    _SWW(NR_AUTO_00, 0x10, TNR_THR_GAIN_ST);
    _SWW(NR_AUTO_00, 0x8, TNR_THR_GAIN_MO);
    _SWW(NR_MO_04, 0x14, MOSNR_MAX_NL_TH);
    _SWW(NR_MO_04, 0x08, MOSNR_MIN_NL_TH);

    _SWW(NR_AUTO_00,  0x10, SNR_ADP_MAX);
    _SWW(NR_AUTO_00,  0x04, SNR_ADP_MIN);
    _SWW(NR_MO_00,    0x04, MOMNR_EDGE_TH_MO);
    _SWW(NR_MO_00,    0x0F, MOMNR_EDGE_TH_ST);
    _SWW(NR_MO_01,    0x20, MONR_CORING_MO);
    _SWW(NR_MO_01,    0x02, MONR_CORING_ST);
#endif    
}

void vDrvNRSetWindow(UINT16 u2Width, UINT16 u2Height)
{
    BNR_FW_Initialize(u2Width, u2Height);

    NR_WM(NXNR_3F,
        P_Fld(0, NX_RNR_FORCE_AGG_RATIO_EN)|
        P_Fld(0, NX_RNR_FORCE_AGG_RATIO));


    _rNrPrm.u1NormFact = (1920*1080*4) / MAX(u2Width * u2Height, (320*240));

    _rNrPrm.u2Width = u2Width;
    _rNrPrm.u2Height = u2Height;
    _rNrPrm.u4TolBlks = ((((u2Width>>3)-(_rNrPrm.u1SADWindowH*2)) * (u2Height-(_rNrPrm.u1SADWindowV*2))) >> (_rNrPrm.u1Interlace ? 1 : 0));
}
void vDrvNRSetMoANRParamByTiming(UINT32 u4VdoTiming,E_BNM_MODE eBnmMode)
{
	if(_EN(EN_LOG_ANR))
		LOG(0,">>>vDrvNRSetMoANRParamByTiming timing=%d,ebnmmode=%d \n",u4VdoTiming,eBnmMode);
    
	switch(u4VdoTiming)
	{
		case MODE_525I_OVERSAMPLE:
		case MODE_625I_OVERSAMPLE:
		case MODE_525I:
			if(eBnmMode==E_BNM_CVBS)//av&atv
			{
                _SWWM(NR_FWBNR_07,P_Fld(0x10,NR_FWBNR_TXPXL_CNT_TH_VAL_0)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_1)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_2)|P_Fld(0x80,NR_FWBNR_TXPXL_CNT_TH_VAL_3));                
                NR_WM(NXNR_2B,P_Fld(0x0F,NX_VBLK_BASE_FILTER_STR)|P_Fld(0x20,NX_VBLK_EDGE_SLOPE)/*|P_Fld(0x10,NX_VBLK_EDGE_TH)*/);                
                NR_WM(NXNR_2C,P_Fld(0x20,NX_VBLK_TXTR_TAP_SLOPE)|P_Fld(0x40,NX_VBLK_TXTR_BASE_STR_SLOPE)|P_Fld(0x60,NX_VBLK_TXTR_BASE_STR_TH));
      	        _SWW(NR_FWBNR_0A, 0x0C,NR_FWBNR_ADAP_VBFS_TH1);
              	_SWW(NR_FWBNR_0B, 0x0F,NR_FWBNR_ADAP_VBFS_TH2);                            
			}
			else if(eBnmMode== E_BNM_COMP_I ||eBnmMode== E_BNM_HDMI_I)//hdmi&ypbpr i
			{
                _SWWM(NR_FWBNR_07,P_Fld(0x30,NR_FWBNR_TXPXL_CNT_TH_VAL_0)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_1)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_2)|P_Fld(0x80,NR_FWBNR_TXPXL_CNT_TH_VAL_3));                
                NR_WM(NXNR_2B,P_Fld(0x0C,NX_VBLK_BASE_FILTER_STR)|P_Fld(0x10,NX_VBLK_EDGE_SLOPE)/*|P_Fld(0x10,NX_VBLK_EDGE_TH)*/);                
                NR_WM(NXNR_2C,P_Fld(0x10,NX_VBLK_TXTR_TAP_SLOPE)|P_Fld(0x40,NX_VBLK_TXTR_BASE_STR_SLOPE)|P_Fld(0x60,NX_VBLK_TXTR_BASE_STR_TH));
                _SWW(NR_FWBNR_0A, 0x08,NR_FWBNR_ADAP_VBFS_TH1);
                //_SWW(NR_FWBNR_0B, 0x0E,NR_FWBNR_ADAP_VBFS_TH2);
                _SWW(MCNR_39, 0x01,DNR0_EN);
			}
            _SWW(NR_FWBNR_0B, 0x08,NR_FWBNR_ADAP_VBFS_TH2);
            _SWW(NR_FWBNR_0B, 0x80,NR_FWBNR_ADAP_VTBSS_TH2);
            _SWW(NR_FWBNR_0B, 0x30,NR_FWBNR_ADAP_VTBSH_TH2);  
            break;           
		case MODE_625I:
			if(eBnmMode==E_BNM_CVBS)//av&atv
			{
                _SWWM(NR_FWBNR_07,P_Fld(0x10,NR_FWBNR_TXPXL_CNT_TH_VAL_0)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_1)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_2)|P_Fld(0x80,NR_FWBNR_TXPXL_CNT_TH_VAL_3));                
                NR_WM(NXNR_2B,P_Fld(0x0F,NX_VBLK_BASE_FILTER_STR)|P_Fld(0x20,NX_VBLK_EDGE_SLOPE)/*|P_Fld(0x10,NX_VBLK_EDGE_TH)*/);                
                NR_WM(NXNR_2C,P_Fld(0x20,NX_VBLK_TXTR_TAP_SLOPE)|P_Fld(0x40,NX_VBLK_TXTR_BASE_STR_SLOPE)|P_Fld(0x60,NX_VBLK_TXTR_BASE_STR_TH));
      	        _SWW(NR_FWBNR_0A, 0x0C,NR_FWBNR_ADAP_VBFS_TH1);
              	//_SWW(NR_FWBNR_0B, 0x0F,NR_FWBNR_ADAP_VBFS_TH2);                            
			}
			else if(eBnmMode== E_BNM_COMP_I ||eBnmMode== E_BNM_HDMI_I)//hdmi&ypbpr i
			{
                _SWWM(NR_FWBNR_07,P_Fld(0x30,NR_FWBNR_TXPXL_CNT_TH_VAL_0)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_1)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_2)|P_Fld(0x80,NR_FWBNR_TXPXL_CNT_TH_VAL_3));                
                NR_WM(NXNR_2B,P_Fld(0x0C,NX_VBLK_BASE_FILTER_STR)|P_Fld(0x10,NX_VBLK_EDGE_SLOPE)/*|P_Fld(0x10,NX_VBLK_EDGE_TH)*/);                
                NR_WM(NXNR_2C,P_Fld(0x10,NX_VBLK_TXTR_TAP_SLOPE)|P_Fld(0x40,NX_VBLK_TXTR_BASE_STR_SLOPE)|P_Fld(0x60,NX_VBLK_TXTR_BASE_STR_TH));
                _SWW(NR_FWBNR_0A, 0x08,NR_FWBNR_ADAP_VBFS_TH1);
                //_SWW(NR_FWBNR_0B, 0x0E,NR_FWBNR_ADAP_VBFS_TH2);            
			}
            _SWW(NR_FWBNR_0B, 0x08,NR_FWBNR_ADAP_VBFS_TH2);
            _SWW(NR_FWBNR_0B, 0x80,NR_FWBNR_ADAP_VTBSS_TH2);
            _SWW(NR_FWBNR_0B, 0x30,NR_FWBNR_ADAP_VTBSH_TH2);  
            //bnr
//            NR_WM(NXNR_2B,P_Fld(8,NX_VBLK_BASE_FILTER_STR)/*|P_Fld(0x1A,NX_VBLK_EDGE_SLOPE)|P_Fld(0x10,NX_VBLK_EDGE_TH)*/);
//            _SWWM(NR_FWBNR_07,P_Fld(0x10,NR_FWBNR_TXPXL_CNT_TH_VAL_0)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_1)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_2)|P_Fld(0x80,NR_FWBNR_TXPXL_CNT_TH_VAL_3));
			break;
			
	    case MODE_480P:
	    case MODE_576P:
	    case MODE_480P_OVERSAMPLE:
	    case MODE_576P_OVERSAMPLE:
	    case MODE_480P_24:
	    case MODE_480P_30:
	    case MODE_576P_25:
			//bnr
            _SWWM(NR_FWBNR_07,P_Fld(0x30,NR_FWBNR_TXPXL_CNT_TH_VAL_0)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_1)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_2)|P_Fld(0x80,NR_FWBNR_TXPXL_CNT_TH_VAL_3));                
            NR_WM(NXNR_2B,P_Fld(0x0C,NX_VBLK_BASE_FILTER_STR)|P_Fld(0x10,NX_VBLK_EDGE_SLOPE)/*|P_Fld(0x10,NX_VBLK_EDGE_TH)*/);                
            NR_WM(NXNR_2C,P_Fld(0x10,NX_VBLK_TXTR_TAP_SLOPE)|P_Fld(0x40,NX_VBLK_TXTR_BASE_STR_SLOPE)|P_Fld(0x60,NX_VBLK_TXTR_BASE_STR_TH));
  	        _SWW(NR_FWBNR_0A, 0x08,NR_FWBNR_ADAP_VBFS_TH1);
          	//_SWW(NR_FWBNR_0B, 0x0E,NR_FWBNR_ADAP_VBFS_TH2);  
            _SWW(NR_FWBNR_0B, 0x08,NR_FWBNR_ADAP_VBFS_TH2);
            _SWW(NR_FWBNR_0B, 0x80,NR_FWBNR_ADAP_VTBSS_TH2);
            _SWW(NR_FWBNR_0B, 0x30,NR_FWBNR_ADAP_VTBSH_TH2);  
			break;
		case MODE_720p_60:	
	    case MODE_1080i_48:
	    case MODE_1080i_50:
	    case MODE_1080i:
	    case MODE_3D_1080I_60_FP:
			//bnr
            _SWWM(NR_FWBNR_07,P_Fld(0x30,NR_FWBNR_TXPXL_CNT_TH_VAL_0)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_1)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_2)|P_Fld(0x80,NR_FWBNR_TXPXL_CNT_TH_VAL_3));                
            NR_WM(NXNR_2B,P_Fld(0x0C,NX_VBLK_BASE_FILTER_STR)|P_Fld(0x10,NX_VBLK_EDGE_SLOPE)/*|P_Fld(0x10,NX_VBLK_EDGE_TH)*/);                
            NR_WM(NXNR_2C,P_Fld(0x8,NX_VBLK_TXTR_TAP_SLOPE)|P_Fld(0x40,NX_VBLK_TXTR_BASE_STR_SLOPE)|P_Fld(0x60,NX_VBLK_TXTR_BASE_STR_TH));                       
  	        _SWW(NR_FWBNR_0A, 0x08,NR_FWBNR_ADAP_VBFS_TH1);
          	//_SWW(NR_FWBNR_0B, 0x0E,NR_FWBNR_ADAP_VBFS_TH2);    
            _SWW(NR_FWBNR_0B, 0x0F,NR_FWBNR_ADAP_VBFS_TH2);
            _SWW(NR_FWBNR_0B, 0x60,NR_FWBNR_ADAP_VTBSS_TH2);
            _SWW(NR_FWBNR_0B, 0x10,NR_FWBNR_ADAP_VTBSH_TH2); 
			break;
			
		case MODE_1080p_24:
	    case MODE_1080p_25:
	    case MODE_1080p_30:
	    case MODE_1080p_50:
	    case MODE_1080p_60:
	    case MODE_3D_1080p_24_FP:
			//bnr
            _SWWM(NR_FWBNR_07,P_Fld(0x30,NR_FWBNR_TXPXL_CNT_TH_VAL_0)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_1)|P_Fld(0x40,NR_FWBNR_TXPXL_CNT_TH_VAL_2)|P_Fld(0x80,NR_FWBNR_TXPXL_CNT_TH_VAL_3));                
            NR_WM(NXNR_2B,P_Fld(0x0C,NX_VBLK_BASE_FILTER_STR)|P_Fld(0x10,NX_VBLK_EDGE_SLOPE)/*|P_Fld(0x10,NX_VBLK_EDGE_TH)*/);                
            NR_WM(NXNR_2C,P_Fld(0x8,NX_VBLK_TXTR_TAP_SLOPE)|P_Fld(0x40,NX_VBLK_TXTR_BASE_STR_SLOPE)|P_Fld(0x60,NX_VBLK_TXTR_BASE_STR_TH));          
  	        _SWW(NR_FWBNR_0A, 0x08,NR_FWBNR_ADAP_VBFS_TH1);
          	//_SWW(NR_FWBNR_0B, 0x0E,NR_FWBNR_ADAP_VBFS_TH2);            
            _SWW(NR_FWBNR_0B, 0x0F,NR_FWBNR_ADAP_VBFS_TH2);
            _SWW(NR_FWBNR_0B, 0x60,NR_FWBNR_ADAP_VTBSS_TH2);
            _SWW(NR_FWBNR_0B, 0x10,NR_FWBNR_ADAP_VTBSH_TH2); 
			break;
			
		default:
			break;
			
	}
    
	_SWW(NR_FWBNR_08, 10,NR_FWBNR_HOMOSUM_TH);
	_SWW(NR_FWBNR_08, 2,NR_NM_STA_FATTAIL_RTO_TH);
	_SWW(NR_FWBNR_08, 0xB0,NR_NM_STA_TAIL_SUM_TH);
	_SWW(NR_FWBNR_08, 2,NR_FWBNR_ASL_TH);    

  	_SWW(NR_FWBNR_09, SV_OFF,NR_FWBNR_ADAP_EN);
  	_SWW(NR_FWBNR_09, 20,NR_FWBNR_ADAP_TH1);
  	_SWW(NR_FWBNR_09, 28,NR_FWBNR_ADAP_TH2);
  	_SWW(NR_FWBNR_0A, 0x08,NR_FWBNR_ADAP_VBFS_TH1);
  	//_SWW(NR_FWBNR_0B, 0x0C,NR_FWBNR_ADAP_VBFS_TH2);
  	_SWW(NR_FWBNR_0A, 0xA0,NR_FWBNR_ADAP_VTBSS_TH1);
  	//_SWW(NR_FWBNR_0B, 0x40,NR_FWBNR_ADAP_VTBSS_TH2);
  	_SWW(NR_FWBNR_0A, 0x20,NR_FWBNR_ADAP_VTBSH_TH1);
  	//_SWW(NR_FWBNR_0B, 0x60,NR_FWBNR_ADAP_VTBSH_TH2); 
    
	if(_EN(EN_LOG_ANR))
	{
		//LOG(0,">>>nr_mo_00=%x,snr_adap_max=%x \n",_SWR(NR_MO_00,MONR_MOTION_LEVEL_TH2),_SWR(NR_MO_00,SNR_ADP_MAX));
		//LOG(0,">>>nr_mo_00=%x,snr_adap_max=%x \n",_SWR(NR_PQ_17,RNR_AGG_THM_GAIN),_SWR(NR_PQ_17,RNR_AGG_THM_OFST));
	}
}

void vDrvNRSetQuality(UINT8 u1Interlace, UINT16 u2Width, UINT16 u2Height)
{
    UINT32 u4DecType = bGetVideoDecType(VDP_1);
    UINT32 u4VdoTiming = bDrvVideoGetTiming(SV_VP_MAIN);
    UINT32 u4HistTmpIIRWei = 0; //0xA;

    UINT32 u4SupportHBlkMeter = (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF) ? 1 : 0;
    UINT32 u4Thd8 = 0x30;
    UINT32 u4SADHistWinH = 4;
    UINT32 u4SADHistWinV = 16;
    UINT32 u4BKNLMaxStr = 0;
    UINT32 u4BkNLGain = 0;
	UINT8  u1MeFhdp = 0;
    //UINT8  u1UseMoAnr = 0;
    UINT32 u4DCR_En;

    E_BNM_MODE eBnmMode = E_BNM_DFT;

    NR_FW_SetState(NR_FW_STOP);

    vDrvLoadRegTbl(_MCNR_PQ_COMMON);
    vDrvLoadRegTbl(_NXNR_PQ_COMMON);
	vDrvLoadRegTbl(_DNR_PQ_COMMON);

    switch (u4VdoTiming)
    {
        case MODE_480P:
        case MODE_480P_OVERSAMPLE:
            u4DCR_En =SV_ON;
            _SWW(NR_NM_0C, 0x3E00, CLEAN_DF_LVL_PDSUM_TH);
            _SWW(NR_NM_13, 0x6A0, CLEAN_DF_LVL_PDSUM_MIN_TH);
            _SWW(NR_NM_04, 0x6FF,CLEAN_DF_LVL_PDSUM_DIFF_TH);
            _SWW(NR_NM_13, 0x5D0, CLEAN_DF_LVL_PDSUM_DIFF_MIN_TH);
            _SWW(NR_PQ_07, 0x28, NR_TBL_MA_7_CDL);
            _SWW(NR_PQ_07, 0x28, NR_TBL_MA_6_CDL);
            _SWW(NR_PQ_07, 0x28, NR_TBL_MA_5_CDL);
            _SWW(NR_PQ_07, 0x28, NR_TBL_MA_4_CDL);
            break;
        case MODE_625I_OVERSAMPLE:
        case MODE_625I:
            u4DCR_En =SV_ON;
            _SWW(NR_NM_0C, 0xA00, CLEAN_DF_LVL_PDSUM_TH);
            _SWW(NR_NM_13, 0x60, CLEAN_DF_LVL_PDSUM_MIN_TH);
            _SWW(NR_NM_04, 0x40,CLEAN_DF_LVL_PDSUM_DIFF_TH);
            _SWW(NR_NM_13, 0x25, CLEAN_DF_LVL_PDSUM_DIFF_MIN_TH);
            _SWW(NR_PQ_07, 0x22, NR_TBL_MA_7_CDL);
            _SWW(NR_PQ_07, 0x22, NR_TBL_MA_6_CDL);
            _SWW(NR_PQ_07, 0x22, NR_TBL_MA_5_CDL);
            _SWW(NR_PQ_07, 0x22, NR_TBL_MA_4_CDL);
            break;
        case MODE_576P:
        case MODE_576P_OVERSAMPLE:
            u4DCR_En =SV_ON;
            _SWW(NR_NM_0C, 0x500, CLEAN_DF_LVL_PDSUM_TH);
            _SWW(NR_NM_13, 0x80, CLEAN_DF_LVL_PDSUM_MIN_TH);
            _SWW(NR_NM_04, 0x2D0,CLEAN_DF_LVL_PDSUM_DIFF_TH);
            _SWW(NR_NM_13, 0x80, CLEAN_DF_LVL_PDSUM_DIFF_MIN_TH);
            _SWW(NR_PQ_07, 0x22, NR_TBL_MA_7_CDL);
            _SWW(NR_PQ_07, 0x22, NR_TBL_MA_6_CDL);
            _SWW(NR_PQ_07, 0x22, NR_TBL_MA_5_CDL);
            _SWW(NR_PQ_07, 0x22, NR_TBL_MA_4_CDL);
            break;
        default:
            u4DCR_En =SV_OFF;
            break;  
    }

    _SWW(NR_CTRL_00,u4DCR_En, EN_CLEAN_DF_DCR);

    switch (u4VdoTiming)
    {
    case MODE_525I_OVERSAMPLE:
    case MODE_625I_OVERSAMPLE:

    case MODE_525I:
    case MODE_625I:
        vDrvLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_SD_I]);
        vDrvLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_SD]);

        u4SADHistWinH = 4;
        u4SADHistWinV = 16;

        if (u4DecType == SV_VD_TVD3D)
        {
            NR_WMSK(NXNR_0A, 0x20201010, 0xFFFFFFFF);
            NR_WMSK(NXNR_0B, 0x00001010, 0x0000FFFF);
            NR_W(NXNR_10, 7, NX_MNR_SYM_LINE_REDUCTION);
            u4HistTmpIIRWei = 0; //4;

            eBnmMode = E_BNM_CVBS;  // for pioneer CVBS, use default
        }
        else if (u4DecType == SV_VD_YPBPR)
        {
            eBnmMode = E_BNM_COMP_I;
        }
        else
        {
            eBnmMode = E_BNM_HDMI_I;
        }

        break;
    case MODE_480P:
    case MODE_576P:
    case MODE_480P_OVERSAMPLE:
    case MODE_576P_OVERSAMPLE:
    case MODE_480P_24:
    case MODE_480P_30:
    case MODE_576P_25:
        vDrvLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_SD_P]);
        vDrvLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_SD]);
        u4SADHistWinH = 4;
        u4SADHistWinV = 16;

        if (u4DecType == SV_VD_YPBPR)
        {
            eBnmMode = E_BNM_COMP_P;
        }
        else
        {
            eBnmMode = E_BNM_HDMI_P;
        }

        break;
    case MODE_720p_24:
    case MODE_720p_25:
    case MODE_720p_30:
    case MODE_720p_50:
    case MODE_720p_60:
    case MODE_3D_720p_50_FP:
    case MODE_3D_720p_60_FP:
        vDrvLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_HD]);
        vDrvLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_HD]);
        u4Thd8 = 0x38;
        u4SADHistWinH = 7;
        u4SADHistWinV = 24;
        break;
    case MODE_1080i_48:
    case MODE_1080i_50:
    case MODE_1080i:
    case MODE_3D_1080I_60_FP:
        vDrvLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_FHD_I]);
        vDrvLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_FHD]);
        u4Thd8 = 0x38;

        u4SADHistWinH = 10;
        u4SADHistWinV = 40;
        u4BkNLGain = 3;
        u4BKNLMaxStr = 1000;
        //u1UseMoAnr = 1;
        break;
    case MODE_1080p_24:
    case MODE_1080p_25:
    case MODE_1080p_30:
    case MODE_1080p_50:
    case MODE_1080p_60:
    case MODE_3D_1080p_24_FP:
        vDrvLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_FHD_P]);
        vDrvLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_FHD]);
        u4Thd8 = 0x38;

        u4SADHistWinH = 10;
        u4SADHistWinV = 40;
        u1MeFhdp = 1;
        //u1UseMoAnr = 1;
        break;
    default:
        vDrvLoadRegTbl(_MCNR_PQ_PARAM[E_MCNR_SD_P]);
        vDrvLoadRegTbl(_NXNR_PQ_PARAM[E_NXNR_SD]);
        u4SupportHBlkMeter = 0;
        u4SADHistWinH = 4;
        u4SADHistWinV = 16;
        break;
    }

    vDrvNRSet3DNRParam();
    vDrvNRSetSNRParam();
    vDrvNRSetMpegNRParam();
    vDrvNRSetMOANRParam();
    
    //set new BNR setting by timing
	vDrvNRSetMoANRParamByTiming(u4VdoTiming,eBnmMode);
    
    _SWW(NR_NM_00, u4HistTmpIIRWei, HIST_TEMP_IIR_WEI);

    _SWW(NR_NM_06, (u4DecType == SV_VD_TVD3D ? 1 : 0), NM_EXT_NL_MODE);
    _SWW(NR_NM_0F, (u1Interlace ? 0x24 : 0x32), NM_ANTI_NL_GAIN_THD_HI);
    _SWW(NR_PQ_09, u4Thd8, NR_TBL_THD8);
    _SWW(NR_PQ_09, u4Thd8, NR_TBL_THD8_HI);
    _SWWM(NR_BK_METER_01, P_Fld(u4BKNLMaxStr, NR_BK_NL_MAX_STR)|P_Fld(u4BkNLGain, NR_BK_NL_GAIN));

    _SWW(NR_NM_06, (NR_NM_TDF_BASE == u1DrvNRGetNoiseMeterSupport() ? 3 : 0), NM_SC_Q_NL_MODE);
    _SWW(NR_NM_03, (NR_NM_TDF_BASE == u1DrvNRGetNoiseMeterSupport() ? 1 : 0), UPDATE_NL_SC);
    //_SWW(NR_NM_03, (NR_NM_TDF_BASE == u1DrvNRGetNoiseMeterSupport() || u1UseMoAnr ? 1 : 0), UPDATE_NL_SC);
    //_SWW(NR_NM_10,   u1UseMoAnr ? 0 : 1, NM_ADJ_BY_MOFBK);
    //_SWW(NR_CTRL_01, u1UseMoAnr, ANR_SEL);

    _SWW(NR_CTRL_00, u4SupportHBlkMeter, EN_SUPPORT_HBLK);

    //NR_W(MCNR_00, (u1Interlace?6:5), MCNR_R2C_INTERVAL);
    NR_WM(MCNR_28, P_Fld(u1MeFhdp, MCNR_ME_FHD_P)|P_Fld(u1MeFhdp, MCNR_RAW_MV_SEL));
    _SWW(NR_CTRL_01, u1MeFhdp, NR_ME_IS_FHDP);
	_SWW(NR_CTRL_00, 1 /*!IS_PROG_DISPMODE(VDP_1)*/, EN_BNM_ISR_MOADJ);
    _SWW(NR_FWBNR_00, 0 /*((u4DecType >= SV_VD_DVI) && (u2Width <= 720) ? 1 : 0)*/, NR_FWBNR_CG_PATCH0);

    NR_WM(MCNR_2C, P_Fld(u4SADHistWinH, MCNR_SAD_HIST_HS)|P_Fld(u4SADHistWinH, MCNR_SAD_HIST_HE));
    NR_WM(MCNR_2D,
        P_Fld((u1Interlace ? (u4SADHistWinV/2) : u4SADHistWinV), MCNR_SAD_HIST_VS)|
        P_Fld((u1Interlace ? (u4SADHistWinV/2) : u4SADHistWinV), MCNR_SAD_HIST_VE));

    _rNrPrm.u1SADWindowH = u4SADHistWinH;
    _rNrPrm.u1SADWindowV = u4SADHistWinV;

    _rNrPrm.u1Timing = u4VdoTiming;
    _rNrPrm.u1NoiseMeterEn = u1DrvNRGetNoiseMeterSupport();
    _rNrPrm.u1Interlace = u1Interlace;
    _rNrPrm.u1IsFhdP = u1MeFhdp;

    vDrvNRSetWindow(u2Width, u2Height);

    vDrvNRSetBlkMeterParam(eBnmMode);

    NR_FW_SetState(NR_FW_VSYNC);
}

void vDrvNRSetBNRFWMode(UINT8 u1FWMode)
{
    if (u1FWMode)
    {
        _SWW(NR_CTRL_00, 0, EN_COPY_LINE_SUM);
        _SWW(NR_FWBNR_00, 1, NR_FWBNR_LOC_WRITE);
    }
    else
    {
        _SWW(NR_CTRL_00, 1, EN_COPY_LINE_SUM);
        _SWW(NR_FWBNR_00, 0, NR_FWBNR_LOC_WRITE);
    }
}

void vDrvNRSet3DNRParam(void)
{
    // Strength
    NR_W(MCNR_00, wReadQualityTable(QUALITY_3DNR_TNR_ACNR_STRENGTH), MCNR_ACNR_STRENGTH);
    //NR_W(MCNR_01, wReadQualityTable(QUALITY_3DNR_TNR_DCNR_ALPHA), MCNR_DCNR_ALPHA);
    vDrvMCNRSetDftDcnrAlpha();

    // LUT
    _SWWM(NR_PQ_12, _P3D(NR_NL_THD_MIN)|_P3D(NR_NL_THD_MAX));
    _SWWM(NR_PQ_0E, _P3D(NR_TBL_THD_1)|_P3D(NR_TBL_THD_2)|_P3D(NR_TBL_THD_3)|_P3D(NR_TBL_THD_4));
    _SWWM(NR_PQ_0F, _P3D(NR_TBL_THD_5)|_P3D(NR_TBL_THD_6)|_P3D(NR_TBL_THD_7)|_P3D(NR_TBL_THD_8));
    _SWWM(NR_PQ_10, _P3D(NR_TBL_THD_E_1)|_P3D(NR_TBL_THD_E_2)|_P3D(NR_TBL_THD_E_3)|_P3D(NR_TBL_THD_E_4));
    _SWWM(NR_PQ_11, _P3D(NR_TBL_THD_E_5)|_P3D(NR_TBL_THD_E_6)|_P3D(NR_TBL_THD_E_7)|_P3D(NR_TBL_THD_E_8));

    _SWWM(NR_PQ_00, _P3D(NR_TBL_MA_0)|_P3D(NR_TBL_MA_1)|_P3D(NR_TBL_MA_2)|_P3D(NR_TBL_MA_3)|_P3D(NR_TBL_MA_4));
    _SWWM(NR_PQ_01, _P3D(NR_TBL_MA_5)|_P3D(NR_TBL_MA_6)|_P3D(NR_TBL_MA_7));
    _SWWM(NR_PQ_0C, _P3D(NR_TBL_C_0)|_P3D(NR_TBL_C_1)|_P3D(NR_TBL_C_2)|_P3D(NR_TBL_C_3)|_P3D(NR_TBL_C_4));
    _SWWM(NR_PQ_0D, _P3D(NR_TBL_C_5)|_P3D(NR_TBL_C_6)|_P3D(NR_TBL_C_7));
    _SWWM(NR_PQ_15, _P3D(NR_TBL_C_E_0)|_P3D(NR_TBL_C_E_1)|_P3D(NR_TBL_C_E_2)|_P3D(NR_TBL_C_E_3)|_P3D(NR_TBL_C_E_4));
    _SWWM(NR_PQ_16, _P3D(NR_TBL_C_E_5)|_P3D(NR_TBL_C_E_6)|_P3D(NR_TBL_C_E_7));
    _SWWM(NR_PQ_03, _P3D(NR_TBL_MA_E_0)|_P3D(NR_TBL_MA_E_1)|_P3D(NR_TBL_MA_E_2)|_P3D(NR_TBL_MA_E_3));
    _SWWM(NR_PQ_04, _P3D(NR_TBL_MA_E_4)|_P3D(NR_TBL_MA_E_5)|_P3D(NR_TBL_MA_E_6)|_P3D(NR_TBL_MA_E_7));
    _SWWM(NR_PQ_05, _P3D(NR_IIR_MAX_WEI)|_P3D(NR_IIR_MAX_WEI_E));
    // Pixel Adaptive
    _SWWM(NR_PQ_08, _P3D(NR_MOVINGTXR_SUB_WEI_BASE_DFT)|_P3D(NR_MOVINGTXR_SUB_WEI_NL_NAG_GAIN_DFT));
    NR_WM(MCNR_04,
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_MA_NOISE_ADD_WEI_BASE), MCNR_MA_NOISE_ADD_WEI_BASE)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_MA_NOISE_ADD_WEI_NL_GAIN), MCNR_MA_NOISE_ADD_WEI_NL_GAIN));
    NR_WM(MCNR_05,
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_MA_PRTRBTN_ADD_WEI_BASE), MCNR_MA_PRTRBTN_ADD_WEI_BASE)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_MA_PRTRBTN_ADD_WEI_STILL_GAIN), MCNR_MA_PRTRBTN_ADD_WEI_STILL_GAIN)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_STILL_GMV_ASSIST_DE_PRTRBTN), MCNR_STILL_GMV_ASSIST_DE_PRTRBTN));
    NR_WM(MCNR_06,
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_CS_NOISE_ADD_WEI_BASE), MCNR_MC_NOISE_ADD_WEI_BASE)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_CS_NOISE_ADD_WEI_NL_GAIN), MCNR_MC_NOISE_ADD_WEI_NL_GAIN));
    NR_WM(MCNR_07,
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_CHROMA_MOVINGTXR_SUB_WEI_MAX), MCNR_CHROMA_MOVINGTXR_SUB_WEI_MAX)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_CHROMA_DC_ADD_WEI_MAX), MCNR_CHROMA_DC_ADD_WEI_MAX)|
        P_Fld(wReadQualityTable(QUALITY_3DNR_TNR_CHROMA_DC_DIFF_BND), MCNR_CHROMA_DC_DIFF_BND));
}

void vDrvNRSetSNRParam(void)
{
    // SNR gain
    NR_WM(NXNR_04, _P2D(NX_SNR_GAIN_C));
    _SWW(NR_PQ_09, wReadQualityTable(QUALITY_2DNR_NX_SNR_GAIN_Y), NR_SNR_GAIN);
    // SNR threshold setting
    NR_WM(NXNR_11, _P2D(NX_SNR_COST_TH_DIR_ADAP)|_P2D(NX_SNR_NOISE_COST_MODE)|_P2D(NX_SNR_NOISE_REMOVAL_AUTO_TH_EDGE_STR));
    _SWWM(NR_PQ_08,
        P_Fld(wReadQualityTable(QUALITY_2DNR_NX_SNR_NOISE_REMOVAL_AUTO_TH_GENERAL_STR), NR_SNR_AUTO_TH_GENERAL_STR)|
        P_Fld(wReadQualityTable(QUALITY_2DNR_NR_SNR_MANUAL_AUTO_RATIO), NR_SNR_MANUAL_AUTO_RATIO));
    _SWWM(NR_NM_07, _P2D(SNR_THM_SEL));
    _SWWM(NR_NM_03, _P2D(SNR_THM_GAIN));
    _SWWM(NR_NM_04, P_Fld(wReadQualityTable(QUALITY_2DNR_SNR_THM_OFST), SNR_THM_OFST_TOOL));
    // SNR max ratio---remove SNR_MAX_RTO_LVL_0~9
    //_SWWM(NR_PQ_06, _P2D(SNR_MAX_RTO_LVL_0)|_P2D(SNR_MAX_RTO_LVL_1)|_P2D(SNR_MAX_RTO_LVL_2)|_P2D(SNR_MAX_RTO_LVL_3)|_P2D(SNR_MAX_RTO_LVL_4));
    //_SWWM(NR_PQ_0B, _P2D(SNR_MAX_RTO_LVL_5)|_P2D(SNR_MAX_RTO_LVL_6)|_P2D(SNR_MAX_RTO_LVL_7)|_P2D(SNR_MAX_RTO_LVL_8)|_P2D(SNR_MAX_RTO_LVL_9));
    // SNR C
    NR_WM(NXNR_12, _P2D(NX_SNR_CHROMA_PRESERVATION)|_P2D(NX_SNR_DEAD_ZONE_TRANSITION)|_P2D(NX_SNR_DEAD_ZONE_DARK)|_P2D(NX_SNR_DEAD_ZONE_BRIGHT));

    // RNR Thd
    NR_WM(NXNR_3F, _P2D(NX_RNR_SNR_THD_RATIO));
    _SWWM(NR_PQ_13, _P2D(RNR_MAX_TH)|_P2D(BNR_RNR_TH));
    // SNR Gain
    _SWWM(NR_PQ_12, _P2D(SNR_NL_THD_MIN)|_P2D(SNR_NL_THD_MAX));
    _SWWM(NR_PQ_14, _P2D(SNR_NL_GAIN_MIN)|_P2D(SNR_NL_GAIN_MAX));
}

void vDrvNRSetMpegNRParam(void)
{
    NR_WM(NXNR_04, _P2D(NX_BNR_GAIN)|_P2D(NX_MNR_GAIN));
    // BNR
    #ifndef BNR_NEW_FW
    NR_WM(NXNR_09, _P2D(NX_HBLK_MIN_LINE_SUM)|_P2D(NX_VBLK_MIN_LINE_SUM));
    #endif
    // MNR
    NR_WM(NXNR_0E, _P2D(NX_MNR_EDGE_TH)|_P2D(NX_MNR_GROUPING_STR));
    NR_WM(NXNR_0F, _P2D(NX_MNR_LERE_GAIN)|_P2D(NX_MNR_LERF_GAIN)|_P2D(NX_MNR_LFRE_GAIN)|_P2D(NX_MNR_UEDE_GAIN)|_P2D(NX_MNR_UEDF_GAIN)|_P2D(NX_MNR_UFDE_GAIN));
    NR_WM(NXNR_10, _P2D(NX_MNR_STR_GAIN));
}

void vDrvNRSet2DNRBNROnOff(UINT8 u1OnOff)
{
    UINT8 bModuleOnOff = u1OnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_BNR);    
    NR_W(NXNR_00, (bModuleOnOff ? 1 : 0), NX_BNR);
    //_SWW(NR_CTRL_00, u1OnOff&&u1DrvNRGetBlockMeterSupport(), EN_ADAP_SNR_BLK);
    _SWW(NR_CTRL_00, (bModuleOnOff ? 1 : 0), EN_BNR_RNR_ADAP);

    if (bModuleOnOff&&u1DrvNRGetBlockMeterSupport())
        vDrvNRSet2DNRSNROnOff(SV_ON);
    else if(_rNrPrm.u1BNRSNROnOff == 0)
        vDrvNRSet2DNRSNROnOff(SV_OFF);
}

void vDrvNRSet2DNRMNROnOff(UINT8 u1OnOff)
{
    UINT8 bModuleOnOff = u1OnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_MNR);    
    NR_W(NXNR_00, (bModuleOnOff ? 1 : 0), NX_MNR);
}

void vDrvNRSet2DNRSNROnOff(UINT8 u1OnOff)
{
    UINT8 bModuleOnOff = u1OnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_SNR);    
    //NR_W(NXNR_00, (u1OnOff ? 1 : (_SWR(NR_CTRL_00, EN_ADAP_SNR_BLK) ? 1 : 0)), NX_SNR_Y);
    _rNrPrm.u1BNRSNROnOff = bModuleOnOff ? 1 : 0;
    NR_W(NXNR_00, (bModuleOnOff ? 1 : (_SWR(NR_CTRL_00, EN_BNR_RNR_ADAP) ? 1 : 0)), NX_SNR_Y);
}

void vDrvNRSet3DNROnOff(UINT8 u1OnOff)
{
    UINT8 bModuleOnOff = u1OnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_TNR);        
    NR_W(MCNR_00, (bModuleOnOff ? 1 : 0), MCNR_ENABLE);
    _rNrPrm.u1MCNROnOff = bModuleOnOff;
}

void vDrvNRSetDotCrawlNROnOff(UINT8 u1OnOff)
{
	UINT32 u4VdoTiming = bDrvVideoGetTiming(SV_VP_MAIN);

	if((u4VdoTiming != MODE_525I_OVERSAMPLE) && (u4VdoTiming != MODE_525I))
	{
		u1OnOff = SV_OFF;
	}
	
    NR_W(MCNR_39, u1OnOff, DNR0_EN);
	NR_W(NXNR_39, u1OnOff, DNR1_FILTER_EN);

}

void vDrvNRSet3DNRStrength(UINT8 u1Strength)
{
    _SWW(NR_PQ_05, MIN(u1Strength, 31), NR_IIR_MAX_WEI_E);
}

void vDrvNRSet3DNRMaxNoiseLvl(UINT8 u1MaxLvl)
{
    u1MaxLvl = MIN(u1MaxLvl, 60);
    _SWW(NR_NM_03, u1MaxLvl, MAX_NOISE_LVL);
}

void vDrvNRSet2DNRAuto(UINT8 u1OnOff)
{
    u1OnOff = (u1OnOff && (NR_NM_SAD_HIST == u1DrvNRGetNoiseMeterSupport()) ? 1 : 0);
    _SWW(NR_CTRL_00, u1OnOff, EN_ADAP_SNR_THM);
    _SWW(NR_CTRL_00, u1OnOff, EN_RNR_TH_ADAP);
    //_SWW(NR_CTRL_00, u1OnOff && (!_SWR(NR_CTRL_01, ANR_SEL)), EN_RNR_TH_ADAP);
}

void vDrvNRSet3DNRAuto(UINT8 u1OnOff)
{
    u1OnOff = (u1OnOff && (NR_NM_SAD_HIST == u1DrvNRGetNoiseMeterSupport()) ? 1 : 0);
    _SWWM(NR_CTRL_00,
        P_Fld(u1OnOff, EN_AUTO_XNR)|
        P_Fld(u1OnOff || (NR_NM_TDF_BASE == u1DrvNRGetNoiseMeterSupport()), EN_ADAP_WEIGHT)|
        P_Fld(u1OnOff, EN_MOTION_ANR));
}

void vDrvNRSet3DNRAutoStrength(INT8 bStrength)
{
    _SWW(NR_AUTO_CONTROL, _CLIP(bStrength, (-5), 5), NR_AUTO_STRENGTH);
}

void vDrvNRSetBitRes(UINT8 u18bit)
{
    NR_W(MCNR_01, (u18bit ? 10 : 8), MCNR_RND_PRECISION);
    _SWW(NR_CTRL_00, (u18bit ? 1 : 0), EN_PATCH_8BIT);
}

void vDrvNRSetForceBypass(UINT8 u1OnOff)
{
    _vDrvNRSetBypass(u1OnOff);
}

void vDrvNRSet3DNRChromaPath(UINT8 u1OnOff)
{
    if (u1OnOff)
    {
        NR_W(MCNR_00, 0, MCNR_C_TEX_OFF);
        NR_WM(MCNR_11, P_Fld(0, MCNR_FORCE_C_WEI_EN)|P_Fld(0, MCNR_FORCE_C_WEI));
    }
    else
    {
        NR_W(MCNR_00, 1, MCNR_C_TEX_OFF);
        NR_WM(MCNR_11, P_Fld(1, MCNR_FORCE_C_WEI_EN)|P_Fld(0, MCNR_FORCE_C_WEI));
    }
}

void vDrvNRGetNRStatus(void)
{
    LOG(0, "(3DNR Mode, 2DNR Mode) = (%d, %d)\n", _rNrPrm.eMCNRMode, _rNrPrm.eNXNRMode);
    LOG(0, "Timing: %s\n", strDrvVideoGetTimingString(_rNrPrm.u1Timing));
    LOG(3, "ConflictCnt = %d\n", _rNrPrm.u4ConflictCnt);

    // noise meter query
    vDrvNRPrintNMStatus();
}

void vDrvNRMainloop(void* pvArgv)
{
    UINT32 u4Start;
    UINT32 u4End;

    while (1)
    {
        //CRIT_STATE_T csState;
        
        VERIFY (x_sema_lock(hNRPQIsrSema, X_SEMA_OPTION_WAIT) == OSR_OK);

        u4Start = NR_R(MCVP_KC_2D, OUT_VDE_CNT);

        //csState = x_crit_start();
        #ifndef BNR_NEW_FW
        if (_SWR(NR_FWBNR_00, NR_FWBNR_USE_HW_MINLS_TH))
        {
            UINT32 u4MinVBlkLineSum, u4MinHBlkLineSum;
            u4MinVBlkLineSum = NR_R(NXNR_09, NX_VBLK_MIN_LINE_SUM);
            u4MinHBlkLineSum = NR_R(NXNR_09, NX_HBLK_MIN_LINE_SUM);
            u4MinVBlkLineSum = u4MinVBlkLineSum == 0 ? 0 : (1 << (MIN(11,u4MinVBlkLineSum)-1));
            u4MinHBlkLineSum = u4MinHBlkLineSum == 0 ? 0 : (1 << (MIN(11,u4MinHBlkLineSum)-1));
            _SWW(NR_FWBNR_00, u4MinVBlkLineSum, NR_FWBNR_VGRIDMINLINESUMTH);
            _SWW(NR_FWBNR_03, u4MinHBlkLineSum, NR_FWBNR_HGRIDMINLINESUMTH);
        }
        #endif

        SYS_MEASURE_TIME_START(SV_BNR_ACTIVE_TIMER);
        BNR_FW_ActiveProcess();
        SYS_MEASURE_TIME_END(SV_BNR_ACTIVE_TIMER);
        SYS_STORE_TIME_INFO_TO_REG(SV_BNR_ACTIVE_TIMER);

        //vDrvNRNoiseLevelProc();
        //x_crit_end(csState);

        u4End = NR_R(MCVP_KC_2D, OUT_VDE_CNT);

        _SWWM(NR_BLK_STA_10, P_Fld(u4Start, NR_BNR_PROC_START)|P_Fld(u4End, NR_BNR_PROC_END));
    }
}

UINT8 u1DrvNRBNMISR(void)
{
    return _SWR(NR_CTRL_00, EN_BNM_ISR_MOADJ);
}

void vDrvNRSemaUnlock(void)
{
#ifdef NR_THREAD_CREATE
    VERIFY(x_sema_unlock(hNRPQIsrSema) == OSR_OK);
#endif
}

#ifdef NR_THREAD_CREATE
static void _vDrvNRCreateThread(void)
{
    static HANDLE_T _hThread;
    INT32 i4Ret;

    VERIFY(x_sema_create(&hNRPQIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

    i4Ret = x_thread_create(&_hThread, "NR main loop", 4096, 100,
        vDrvNRMainloop, 0, NULL);

    if (i4Ret != OSR_OK)
    {
        Printf("NR main loop Thread cannot be created\n");
    }
}
#endif

/**
 * @brief Initialize NR
 */
void vDrvNRInit(void)
{
    _rNrPrm.eMCNRMode = MCNR_MODE_OFF;
    _rNrPrm.eNXNRMode = NXNR_MODE_OFF;
    _rNrPrm.u4ConflictCnt = 0;
    _rNrPrm.u1Interlace = 1;
    _rNrPrm.u1MCNROnOff = 0;
    _rNrPrm.u1BNRSNROnOff = 0;
    _rNrPrm.u2Width = 720;
    _rNrPrm.u2Height = 480;
    _rNrPrm.u1SADWindowH = 4;
    _rNrPrm.u1SADWindowV = 16;
    _rNrPrm.u4TolBlks = 21600;
    _rNrPrm.u1Ver = ((IS_IC_5398()&&(BSP_GetIcVersion() > IC_VER_5398_AA))||IS_IC_5880()||IS_IC_5860());

#ifdef NR_THREAD_CREATE
    _vDrvNRCreateThread();
#endif

    vDrvNRPQInit();

    _SWW(NR_FWBNR_00, (DRVCUST_OptGet(eShareBnrHwCfg)?1:0), NR_FWBNR_USE_HW_MINLS_TH);
	_SWW(NR_FWBNR_0B, 6, NR_FWBNR_CONF_OFST);  
}

void vDrvNRProc(void)
{
    vDrvNRNoiseLevelProc();
}

/**
 * @brief Set split line for demo mode
 * @param bOnOff SV_ON/SV_OFF
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param wXCenter Split line position
 * @warning bPath does not have any effect for now
 */
void vDrvNRSet2DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter)
{
#ifdef CC_FLIP_MIRROR_SUPPORT
	if((u4QueryFlipModule(SV_VP_MAIN) & (FLIP_BY_PSCAN | FLIP_BY_B2R) )&&(u1GetFlipMirrorConfig()& SYS_MIRROR_CONFIG_ON))
	{bRegion = !bRegion;}
#endif

    // set demo side window left
    NR_WM(NXNR_01, P_Fld(0, NX_DEMO_XS)|P_Fld(wXCenter, NX_DEMO_XE));
    NR_WM(NXNR_02, P_Fld(0, NX_DEMO_YS)|P_Fld(_rNrPrm.u2Height, NX_DEMO_YE));
    NR_WM(NXNR_00, P_Fld(bOnOff, NX_DEMO_EN)|P_Fld(((bRegion==SV_LEFT)?0:1), NX_DEMO_INSIDE));
}

void vDrvNRSet3DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter)
{
#ifdef CC_FLIP_MIRROR_SUPPORT
	if(((u4QueryFlipModule(SV_VP_MAIN) & (FLIP_BY_PSCAN | FLIP_BY_B2R))&&(u1GetFlipMirrorConfig()& SYS_MIRROR_CONFIG_ON))&&
        (!((u4QueryFlipModule(SV_VP_MAIN) & FLIP_BY_PSCAN) && bDrvVideoIsSrcInterlace(VDP_1) == 0)))
	{bRegion = !bRegion;}
#endif

    // NR workaround for 420 SD progressive bottom line chroma NR issue
    if(IS_420TO422(VDP_1) && (!IS_MIB_FRC(VDP_1)) && (IS_PROGRESSIVE(VDP_1)))
    {
        if (bOnOff)
        {
            if (bRegion == SV_LEFT)
            {
                NR_WM(MCNR_13, P_Fld(0, MCNR_DEMO_HS)|P_Fld(wXCenter, MCNR_DEMO_HE));
            }
            else
            {
                NR_WM(MCNR_13, P_Fld(wXCenter, MCNR_DEMO_HS)|P_Fld(_rNrPrm.u2Width, MCNR_DEMO_HE));
            }
        }
        else
        {
            NR_WM(MCNR_13, P_Fld(0, MCNR_DEMO_HS)|P_Fld(_rNrPrm.u2Width, MCNR_DEMO_HE));
        }
        NR_WM(MCNR_14, P_Fld(0, MCNR_DEMO_VS)|P_Fld(_rNrPrm.u2Height-NR_DEMO_MASK, MCNR_DEMO_VE));
        NR_WM(MCNR_00, P_Fld(1, MCNR_DEMO_MODE)|P_Fld(1, MCNR_DEMO_INSIDE));
    }
    else
    {
        // set demo side window left
        NR_WM(MCNR_13, P_Fld(0, MCNR_DEMO_HS)|P_Fld(wXCenter, MCNR_DEMO_HE));
        NR_WM(MCNR_14, P_Fld(0, MCNR_DEMO_VS)|P_Fld(_rNrPrm.u2Height, MCNR_DEMO_VE));
        NR_WM(MCNR_00, P_Fld(bOnOff, MCNR_DEMO_MODE)|P_Fld(((bRegion==SV_LEFT)?1:0), MCNR_DEMO_INSIDE));
    }
}

void vDrvNRSet2DNRDemoRgn(UINT8 bOnOff, Region bRegion)
{
    NR_WM(NXNR_01,
        P_Fld(bRegion.wHStart, NX_DEMO_XS)|
        P_Fld(bRegion.wHEnd-1, NX_DEMO_XE));
    NR_WM(NXNR_02,
        P_Fld(bRegion.wVStart, NX_DEMO_YS)|
        P_Fld(bRegion.wVEnd, NX_DEMO_YE));
    NR_WM(NXNR_00, P_Fld((bOnOff?1:0), NX_DEMO_EN)|P_Fld(0, NX_DEMO_INSIDE));
}

void vDrvNRSet3DNRDemoRgn(UINT8 bOnOff, Region bRegion)
{
    UINT8 bSft = (_rNrPrm.u1Interlace)?1:0;

    NR_WM(MCNR_13,
        P_Fld(bRegion.wHStart, MCNR_DEMO_HS)|
        P_Fld(bRegion.wHEnd-1, MCNR_DEMO_HE));
    NR_WM(MCNR_14,
        P_Fld((bRegion.wVStart>>bSft)+1, MCNR_DEMO_VS)|
        P_Fld((bRegion.wVEnd>>bSft)+1, MCNR_DEMO_VE));
    NR_WM(MCNR_00, P_Fld((bOnOff?1:0), MCNR_DEMO_MODE)|P_Fld(1, MCNR_DEMO_INSIDE));
}

