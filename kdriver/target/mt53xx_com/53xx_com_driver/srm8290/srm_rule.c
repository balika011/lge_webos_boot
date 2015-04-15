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
 * $Date: 2015/04/15 $
 * $RCSfile: srm_rule.c,v $
 * $Revision: #6 $
 *
 *---------------------------------------------------------------------------*/

/** @file srm_rule.c
 *  Brief of file srm_rule.c
 *  Details of file srm_rule.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "srm_drvif.h"
#include "fbm_drvif.h"
#include "vdp_drvif.h"
#include "srm_debug.h"
#include "panel.h"
#include "drvcust_if.h"
#include "video_def.h"
#include "x_assert.h"
//#include "x_chip_id.h"

#ifdef CC_SCPOS_EN
#include "source_table.h"  // nptv
#endif

#include "x_hal_5381.h"
#include "b2r_if.h"
#include "vdo_misc.h"
#include "drv_tdtv.h"
#include "drv_tdtv_drvif.h"
#include "source_select.h"

#if defined(CC_MT5399) || defined(CC_MT5882)||defined(CC_MT5890)
#include "drv_mjc.h"
#endif

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define SRM_DISP_MIN_FRAME_RATE 48

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern UINT32 IS_Support60MJC(VOID);
//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define SRM_CHECK_INFO                                                         \
    do                                                                         \
    {                                                                          \
        if ((prSrmVdpInfo == NULL) || (prSrmVdpCtrl == NULL))                  \
        {                                                                      \
            return 0;                                                          \
        }                                                                      \
    }                                                                          \
    while (0)


#define SRM_UPDATE_BOB(vdp, mode, arg1, arg2, arg3)                            \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_BOB_CTRL] < mode))                   \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_BOB_CTRL] = mode;                     \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_BOB_CTRL].u4Arg1 = arg1;                 \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_BOB_CTRL].u4Arg2 = arg2;                 \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_BOB_CTRL].u4Arg3 = arg3;                 \
        LOG(6, "[SRM] BOB DB vdp%d mode%d 0x%x %d %d\n"                        \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         \

#define SRM_UPDATE_DISP(vdp, mode, arg1, arg2, arg3)                           \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_DISP_CTRL] < mode))                  \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_DISP_CTRL] = mode;                    \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_DISP_CTRL].u4Arg1 = arg1;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_DISP_CTRL].u4Arg2 = arg2;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_DISP_CTRL].u4Arg3 = arg3;                \
        LOG(6, "[SRM] DISP DB vdp%d mode%d 0x%x %d %d\n"                       \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         \

#define SRM_UPDATE_TDC(vdp, mode, arg1, arg2, arg3)                           \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_TDC_CTRL] < mode))                  \
    {                                                                         \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_TDC_CTRL] = mode;                    \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_TDC_CTRL].u4Arg1 = arg1;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_TDC_CTRL].u4Arg2 = arg2;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_TDC_CTRL].u4Arg3 = arg3;                \
        LOG(6, "[SRM] TDC DB vdp%d mode%d 0x%x %d %d\n"                       \
            , vdp, mode, arg1, arg2, arg3);                                   \
     }    

#if 0
#define SRM_UPDATE_MPEG_PIP(vdp, mode, arg1, arg2, arg3)                       \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MPEG_PIP_CTRL] < mode))              \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MPEG_PIP_CTRL] = mode;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MPEG_PIP_CTRL].u4Arg1 = arg1;            \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MPEG_PIP_CTRL].u4Arg2 = arg2;            \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MPEG_PIP_CTRL].u4Arg3 = arg3;            \
        LOG(6, "[SRM] MPEG DB vdp%d mode%d 0x%x %d %d\n"                       \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         
#endif

#if defined(CC_MT5399)||defined(CC_MT5882)||defined(CC_MT5890)
#define SRM_UPDATE_MJC(vdp, mode, arg1, arg2, arg3)                            \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MJC_CTRL] < mode))                   \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MJC_CTRL] = mode;                     \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MJC_CTRL].u4Arg1 = arg1;                 \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MJC_CTRL].u4Arg2 = arg2;                 \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MJC_CTRL].u4Arg3 = arg3;                 \
        LOG(6, "[SRM] MJC DB vdp%d mode%d 0x%x %d %d\n"                        \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }                                                                         
#endif

#define SRM_UPDATE_MPEG_SIZE(vdp, mode, arg1, arg2, arg3)                      \
    if ((prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MMBUFCHG_CTRL] < mode))              \
    {                                                                          \
        prSrmVdpCtrl[vdp].u4CtrlMode[SRM_MMBUFCHG_CTRL] = mode;                \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MMBUFCHG_CTRL].u4Arg1 = arg1;            \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MMBUFCHG_CTRL].u4Arg2 = arg2;            \
        prSrmVdpCtrl[vdp].rNewPrm[SRM_MMBUFCHG_CTRL].u4Arg3 = arg3;            \
        LOG(6, "[SRM] MPEG_SIZE DB vdp%d mode%d 0x%x %d %d\n"                  \
            , vdp, mode, arg1, arg2, arg3);                                    \
     }

#if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
#define DONOT_USE_DISPMODE()\
            do {\
                if ((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) && (prSrmVdpInfo[VDP_1].u4OutputHeight < 5000) && (prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_720HD_RESOLUTION))\
                {\
                    u4Mode &= ~FBM_POOL_MODE_10BIT;\
                    u4Mode |= FBM_POOL_MODE_8BIT;\
                    u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;\
                    u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;\
                    u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;\
                    u4Mode &= ~FBM_POOL_MODE_SCPOS_LINESYNC_DISP;\
                    u4Mode |= FBM_POOL_MODE_SCPOS_FULL;\
                }\
            } while (0)
            
#define GIVEUP_PSCANNR_DRAM()\
            do {\
                if ((prSrmVdpInfo[VDP_1].u4Source == (UINT32)VSS_DTV) && (prSrmVdpInfo[VDP_1].u4OutputHeight < 5000) && (prSrmVdpInfo[VDP_1].u4Resolution >= SRM_VDP_720HD_RESOLUTION))\
                {\
                    u4Mode &= ~FBM_POOL_MODE_10BIT;\
                    u4Mode |= FBM_POOL_MODE_8BIT;\
                    u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;\
                    u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;\
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;\
                    u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;\
                    u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;\
                    u4Mode |= FBM_POOL_MODE_MDDI_BOB;\
                }\
            } while (0)
#endif
        
        
#if defined(CC_DOWN_BW_WITH_MENU)
#define JUDEG_TO_DOWN_SCALER_PQ()\
            do {\
                if (SRM_IsDownBWMode())\
                {\
                    u4Mode &= ~FBM_POOL_MODE_10BIT;\
                    u4Mode |= FBM_POOL_MODE_8BIT;\
                }\
            } while (0)
#define JUDGE_TO_DOWN_PSCANNR_PQ()\
            do {\
                if (SRM_IsDownBWMode())\
                {\
                    u4Mode &= ~FBM_POOL_MODE_10BIT;\
                    u4Mode |= FBM_POOL_MODE_8BIT;\
                    u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;\
                    u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;\
                    u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;\
                    u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;\
                    u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;\
                    u4Mode |= FBM_POOL_MODE_MDDI_BOB;\
                }\
            } while (0)
#endif

// Used to notify SRM/FBM changes. Used internally.
#define SET_RESOLUTION(RES, MODE)                                              \
    do                                                                         \
    {                                                                          \
        if ((RES) >= SRM_VDP_1080HD_RESOLUTION)                                \
        {                                                                      \
            (MODE) |= FBM_POOL_MODE_1080HD;                                    \
        }                                                                      \
        else if ((RES) >= SRM_VDP_720HD_RESOLUTION)                            \
        {                                                                      \
            (MODE) |= FBM_POOL_MODE_720HD;                                     \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            (MODE) |= FBM_POOL_MODE_SD;                                        \
        }                                                                      \
        if (SRM_IsEpgMode())                                                   \
        {                                                                      \
            (MODE) |= FBM_POOL_MODE_EPG;                                       \
        }                                                                      \
    }                                                                          \
    while (0)

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#if defined(CC_MT5890)||defined(CC_MT5882)
static UINT32 _SrmGetScposMode(UINT8 u1VdpId, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    return prSrmVdpCtrl[u1VdpId].rNewPrm[SRM_DISP_CTRL].u4Arg1;
}
#else
static UINT32 _SrmGetScposMode(SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    return prSrmVdpCtrl[VDP_1].rNewPrm[SRM_DISP_CTRL].u4Arg1;
}
#endif
#if defined(CC_MT5882)
static UINT32 _SrmScposRule5882_DDRXx2(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();
    UINT32 u4ScalerModeEx=0;
    UINT32 u4MIBCfg = u4DrvTDTVDIModeQuery();
    u4ScalerModeEx=u4DrvTDTVScalerModeQueryEx(VDP_1);

    SRM_CHECK_INFO;

    //========================
    //VDP_1 Path Confiugration
    //========================
    u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
    
    if( //3D Fmt Convert by MIB , MIB always need output sync mode            
      (E_TD_OUT_NATIVE != TD_MIB_OUT(u4MIBCfg))
        //3D overscan only support by output sync mode
       ||((E_TD_IN_2D_I != TD_MIB_IN(u4MIBCfg))&&(E_TD_IN_2D_P != TD_MIB_IN(u4MIBCfg)))
       )
    {
        u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
        u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;  
        u4Mode |= FBM_POOL_MODE_10BIT;      
    }
    else if (SRM_IsScanMode(VDP_1))
    {
        // applied to all ATV src except 3D Scenario
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_10BIT| FBM_POOL_MODE_422_FB;
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_1])) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if((prSrmVdpInfo[VDP_1].u4VgaTiming == 1) || (u4DrvTDTVForceDIDramModeQuery() == E_TDTV_DI_DRAM_0FB) 
            || (fgIs4k2kNativeTimingOut(VDP_1)))
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        else if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION)
        {
            if((bGetSignalType(VDP_1) == SV_ST_MPEG) &&(prSrmVdpInfo[VDP_1].u4InputFrameRate <= 24))
            {//For seamless low frame rate/input height  frame tear issue
                u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }

        // use 444 in vga input source
        if ((prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
            || fgIs4k2kNativeTimingOut(VDP_1))
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        // 8/10bit
        if ((prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
            || fgIs4k2kNativeTimingOut(VDP_1))
        {
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }
        if (DRVCUST_OptGet(eDdrClock) < 800000000)
        {
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_1080HD_RESOLUTION)
            {
                u4Mode &= ~FBM_POOL_MODE_8BIT;
                u4Mode &= ~FBM_POOL_MODE_10BIT;

                u4Mode |= FBM_POOL_MODE_8BIT;
            }
        }
        
#ifdef CC_GAME_MODE_BETTER_Q
        if (SRM_IsGameMode(VDP_1)&&(prSrmVdpInfo[VDP_1].u4Interlace == 1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
            u4Mode &= ~FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
            u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        }
#endif

#if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
        DONOT_USE_DISPMODE();
#endif
        
#if defined(__MODEL_slt__) // for SLT, always DRAM mode
        u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
#endif
        
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
    }

#if defined(CC_DOWN_BW_WITH_MENU)
    JUDEG_TO_DOWN_SCALER_PQ();
#endif
    
    SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);

    //========================
    //VDP_2 Path Confiugration
    //========================

    u4Mode = 0;
    u4ScalerModeEx=u4DrvTDTVScalerModeQueryEx(VDP_2);
    if (SRM_IsScanMode(VDP_2))
    {
        LOG(3, "SRM RULE: SCALER: VDP2: SCAN mode\n");
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2])  &&
    	(SRM_GetMmModeByPath(VDP_2) != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

        if (prSrmVdpInfo[VDP_2].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
        }
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
    }

#if defined(CC_DOWN_BW_WITH_MENU)
    JUDEG_TO_DOWN_SCALER_PQ();
#endif




    //SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);

    
    return u4Mode;
}
#endif

#if defined(CC_MT5399)||defined(CC_MT5890)
static UINT32 _SrmScposRule5890_DDRXx2(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4CtrlMode = SRM_CTRL_MODE_NONE;
    UINT32 u4PDSize = PANEL_GetPanelWidth();
    UINT32 u4ScalerModeEx=0;
    UINT32 u4MIBCfg = u4DrvTDTVDIModeQuery();
    u4ScalerModeEx=u4DrvTDTVScalerModeQueryEx(VDP_1);

    SRM_CHECK_INFO;

    //========================
    //VDP_1 Path Confiugration
    //========================
    u4Mode |= FBM_POOL_MODE_SCPOS_3FB;

    if( //3D Fmt Convert by MIB , MIB always need output sync mode            
      (E_TD_OUT_NATIVE != TD_MIB_OUT(u4MIBCfg))
        //3D overscan only support by output sync mode
       ||((E_TD_IN_2D_I != TD_MIB_IN(u4MIBCfg))&&(E_TD_IN_2D_P != TD_MIB_IN(u4MIBCfg)))
       )
    {
        u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
        u4Mode |= FBM_POOL_MODE_10BIT; 
        
        if (IS_IC_5861() && fgIs4k2kNativeTimingOut(VDP_1) )
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_HYBRID_DISP;  
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;  
        }
    }
    else if (SRM_IsScanMode(VDP_1))
    {
        // applied to all ATV src except 3D Scenario
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_10BIT| FBM_POOL_MODE_422_FB;
		
    }
    else if ((SRM_CheckStable(&prSrmVdpInfo[VDP_1])) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
             (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        VDP_SEAMLESS_INFO_T b2rVrmInfo;
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        if (bDrvVideoIsDecDualMode())
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_DUAL_HYBRID_DISP;
        }
        else if ( fgIs4k2kNativeTimingOut(VDP_1) )
        {
            if (IS_IC_5861() && prSrmVdpInfo[VDP_1].u4VgaTiming == 0)
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_HYBRID_DISP;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
            }
        }
        else if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1 || u4DrvTDTVForceDIDramModeQuery() == E_TDTV_DI_DRAM_0FB)
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }		
        else if(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_1080HD_RESOLUTION &&
                   VDP_GetSeamlessInfo(VDP_1, &b2rVrmInfo)!=VDP_SET_ERROR)
        {  // temp netflix seamless
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }
        else if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_SD_RESOLUTION)
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        }

        // use 444 in vga input source
        if ((prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
            || fgIs4k2kNativeTimingOut(VDP_1))
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        // 8/10bit
        if ((prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
            || fgIs4k2kNativeTimingOut(VDP_1))
        {
            if((u4Mode&FBM_POOL_MODE_SCPOS_HYBRID_DISP)||(u4Mode&FBM_POOL_MODE_SCPOS_DUAL_HYBRID_DISP))
            {
                if(bGetSignalType(VDP_1) == SV_ST_MPEG)
                {//MM 4K force 8 bit to save bandwidth
                    u4Mode |= FBM_POOL_MODE_8BIT;
                }
                else
                {
                    u4Mode |= FBM_POOL_MODE_10BIT;
                }
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_8BIT;
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_10BIT;
        }

        if (DRVCUST_OptGet(eDdrClock) < 800000000)
        {
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_1080HD_RESOLUTION)
            {
                u4Mode &= ~FBM_POOL_MODE_8BIT;
                u4Mode &= ~FBM_POOL_MODE_10BIT;

                u4Mode |= FBM_POOL_MODE_8BIT;
            }
        }

        if (SRM_DualMode8Bit())
        {   
            u4Mode &= ~FBM_POOL_MODE_10BIT;
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        
#ifdef CC_GAME_MODE_BETTER_Q
        if (SRM_IsGameMode(VDP_1)&&(prSrmVdpInfo[VDP_1].u4Interlace == 1))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
            u4Mode &= ~FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
            u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
            u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        }
#endif

#if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
        DONOT_USE_DISPMODE();
#endif
        
#if defined(__MODEL_slt__) // for SLT, always DRAM mode
        u4Mode &= ~FBM_POOL_MODE_SCPOS_FULL;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_PSCAN_DISP;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_DISP;
        u4Mode &= ~FBM_POOL_MODE_SCPOS_LINESYNC_DISP;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
#endif
        
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
    }

#if defined(CC_DOWN_BW_WITH_MENU)
    JUDEG_TO_DOWN_SCALER_PQ();
#endif
    
    SRM_UPDATE_DISP(VDP_1, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);

    //========================
    //VDP_2 Path Confiugration
    //========================

    u4Mode = 0;
    u4ScalerModeEx=u4DrvTDTVScalerModeQueryEx(VDP_2);
    if (SRM_IsScanMode(VDP_2))
    {
        LOG(3, "SRM RULE: SCALER: VDP2: SCAN mode\n");
        u4Mode = FBM_POOL_MODE_SCPOS_FULL | FBM_POOL_MODE_422_FB | FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2])  &&
    	(SRM_GetMmModeByPath(VDP_2) != SRM_MM_MODE_TYPE_THUMBNAIL))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);

    #if defined(CC_FBM_SUB_SUPPORT_FHD)
        #if 0//sub path always config scaler dram mode
	    if ((prSrmVdpInfo[VDP_2].u4Resolution > SRM_VDP_SD_RESOLUTION) && (prSrmVdpInfo[VDP_2].u4Interlace == 1))
		{
    	    u4Mode |= FBM_POOL_MODE_SCPOS_PSCAN_DISP;
		}
	    else
        #endif
		{
            u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
            u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
		}
		u4Mode |= FBM_POOL_MODE_8BIT;
    #else
        u4Mode |= FBM_POOL_MODE_8BIT;
        u4Mode |= FBM_POOL_MODE_SCPOS_FULL;
        u4Mode |= FBM_POOL_MODE_SCPOS_3FB;
    #endif
        if (prSrmVdpInfo[VDP_2].u4VgaTiming == 1)
        {
            u4Mode |= FBM_POOL_MODE_444_FB;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_422_FB;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_SCPOS_3FB;
            u4Mode |= FBM_POOL_MODE_SCPOS_2FB;
        }
    }
    else
    {
        u4Mode = FBM_POOL_MODE_UNSTABLE;
    }

#if defined(CC_DOWN_BW_WITH_MENU)
    JUDEG_TO_DOWN_SCALER_PQ();
#endif




    //SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, 0);
    SRM_UPDATE_DISP(VDP_2, u4CtrlMode, u4Mode, u4PDSize, u4ScalerModeEx);

    
    return u4Mode;
}
#endif

#if defined(CC_MT5882)
static UINT32 _SrmMddiRule5882_DDRXx2(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4PscanModeEx=0;

    u4PscanModeEx=u4DrvTDTVDIModeQuery();

    SRM_CHECK_INFO;

    //========================
    //VDP_1 Path Confiugration
    //========================

    if (_SrmGetScposMode(VDP_1,prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
    {
        u4Mode |= FBM_POOL_MODE_MDDI_DISP;
    }

    if (SRM_IsScanMode(VDP_1))
    {
        // Applied for all atv source
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;        
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL) 
        )
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
       
        if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            u4Mode &= ~FBM_POOL_MODE_10BIT;
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        //FS Interlace Only 8 Bit due to Dual DI Mode
        else if((TD_MIB_IN(u4PscanModeEx)==E_TD_IN_FS_I)&&
            (TD_MIB_OUT(u4PscanModeEx)==E_TD_OUT_NATIVE))
        {
            u4Mode &= ~FBM_POOL_MODE_10BIT;
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        //3D PR Mode Only support CBOB Mode
        else if((TD_MIB_OUT(u4PscanModeEx)==E_TD_OUT_3D_LI)&&
            (prSrmVdpInfo[VDP_1].u4Interlace == 1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_BOB;            
            u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
        }
#if !defined(CC_GAME_MODE_BETTER_Q)
        else if (SRM_IsGameMode(VDP_1)&&            
            (prSrmVdpInfo[VDP_1].u4Interlace == 1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            
            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;                
        }
#endif

        if (DRVCUST_OptGet(eDdrClock) < 800000000)
        {
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_1080HD_RESOLUTION)
            {
                u4Mode &= ~FBM_POOL_MODE_10BIT;
                u4Mode |= FBM_POOL_MODE_8BIT;
            }
        }

        // hw limitation: frame sequential interlaced timing, pscan use bob mode
        if ((u4DrvTDTVForceDIDramModeQuery() == E_TDTV_DI_DRAM_0FB)||(fgIs4k2kNativeTimingOut(VDP_1)))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        }
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
    }

#if defined(CC_DOWN_BW_WITH_MENU)
    JUDGE_TO_DOWN_PSCANNR_PQ();
#endif

#if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
    GIVEUP_PSCANNR_DRAM();
#endif

    SRM_UPDATE_BOB(VDP_1, SRM_CTRL_MODE_NONE, u4Mode, 0, u4PscanModeEx);

    //========================
    //VDP_2 Path Confiugration
    //========================

    u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_MDDI_NR_OFF;
    u4PscanModeEx = 0;
    
    if (SRM_IsScanMode(VDP_2))
    {
        if (SRM_IsPopVideo())
        {
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
        }
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]) &&
    	(SRM_GetMmModeByPath(VDP_2) != SRM_MM_MODE_TYPE_THUMBNAIL) &&
        (prSrmVdpInfo[VDP_2].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);

        if (prSrmVdpInfo[VDP_2].u4Interlace == 1)
        {   
            if (SRM_IsPopVideo())
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_CBOB; // in sub pip, pscan DONT support full mode

                if (DRVCUST_OptGet(eDdrClock) < 800000000)
                {
                    u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
                    u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
                    u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                }
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_BOB;
    }
    
#if defined(CC_DOWN_BW_WITH_MENU)
    JUDGE_TO_DOWN_PSCANNR_PQ();
#endif
    
    //SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    SRM_UPDATE_BOB(VDP_2, SRM_CTRL_MODE_NONE, u4Mode, 0, u4PscanModeEx);
    
    return u4Mode;
}


#endif

#if defined(CC_MT5399)||defined(CC_MT5890)
static UINT32 _SrmMddiRule5890_DDRXx2(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4PscanModeEx=0;
    UINT32 u4ScalerMode = 0;

    u4PscanModeEx=u4DrvTDTVDIModeQuery();

    SRM_CHECK_INFO;

    //========================
    //VDP_1 Path Confiugration
    //========================

    u4ScalerMode = _SrmGetScposMode(VDP_1,prSrmVdpCtrl); 
    if ( u4ScalerMode & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
    {
        u4Mode |= FBM_POOL_MODE_MDDI_DISP;
    }
    else if (u4ScalerMode & FBM_POOL_MODE_SCPOS_HYBRID_DISP)
    {
        u4Mode |= FBM_POOL_MODE_MDDI_HYBRID;
    }
    else if (u4ScalerMode & FBM_POOL_MODE_SCPOS_DUAL_HYBRID_DISP)
    {
        u4Mode |= FBM_POOL_MODE_MDDI_DUAL_HYBRID;
    }

    if (SRM_IsScanMode(VDP_1))
    {
        // Applied for all atv source
        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
        u4Mode |= FBM_POOL_MODE_MDDI_FULL;        
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_JPEG) &&
        (SRM_GetMmMode() != SRM_MM_MODE_TYPE_THUMBNAIL) 
        )
    {
        VDP_SEAMLESS_INFO_T b2rVrmInfo;    
        SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

        u4Mode |= FBM_POOL_MODE_10BIT;
        u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_C;
       
        if (prSrmVdpInfo[VDP_1].u4Interlace == 1)
        {
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        if (prSrmVdpInfo[VDP_1].u4VgaTiming == 1)
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            u4Mode &= ~FBM_POOL_MODE_10BIT;
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        else if (u4Mode & FBM_POOL_MODE_MDDI_DUAL_HYBRID)
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            if ( SRM_DualMode8Bit() )
            {
                u4Mode &= ~FBM_POOL_MODE_10BIT;
                u4Mode |= FBM_POOL_MODE_8BIT;            
            }
        }
        else if ( (fgIs4k2kNativeTimingOut(VDP_1)) &&
                     (E_TD_IN_2D_P == TD_MIB_IN(u4PscanModeEx)) &&
                     IS_IC_5890()
                    )
        {//  4k2k  2D  DI bypass
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        }
        else if(prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_1080HD_RESOLUTION &&
                   VDP_GetSeamlessInfo(VDP_1, &b2rVrmInfo)!=VDP_SET_ERROR)
        {  // temp netflix seamless
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        }
        //FS Interlace Only 8 Bit due to Dual DI Mode
        else if((TD_MIB_IN(u4PscanModeEx)==E_TD_IN_FS_I)&&
            (TD_MIB_OUT(u4PscanModeEx)==E_TD_OUT_NATIVE))
        {
            u4Mode &= ~FBM_POOL_MODE_10BIT;
            u4Mode |= FBM_POOL_MODE_8BIT;
        }
        //3D PR Mode Only support CBOB Mode
        else if((TD_MIB_OUT(u4PscanModeEx)==E_TD_OUT_3D_LI)&&
            (prSrmVdpInfo[VDP_1].u4Interlace == 1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_BOB;            
            u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
        }//Pscan Progressive 420 LI input  chroma TNR NG
        else if(TD_MIB_IN(u4PscanModeEx) == E_TD_IN_LI_P)
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_OFF;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_Y_ONLY;
        }
#if !defined(CC_GAME_MODE_BETTER_Q)
        else if (SRM_IsGameMode(VDP_1)&&            
            (prSrmVdpInfo[VDP_1].u4Interlace == 1))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
            
            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;                
        }
#endif

        if (DRVCUST_OptGet(eDdrClock) < 800000000)
        {
            if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_1080HD_RESOLUTION)
            {
                u4Mode &= ~FBM_POOL_MODE_10BIT;
                u4Mode |= FBM_POOL_MODE_8BIT;
            }
        }

        // hw limitation: frame sequential interlaced timing, pscan use bob mode
        if (u4DrvTDTVForceDIDramModeQuery() == E_TDTV_DI_DRAM_0FB)
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_C;
            u4Mode &= ~FBM_POOL_MODE_MDDI_NR_Y_ONLY;
            u4Mode |= FBM_POOL_MODE_MDDI_NR_OFF;
        }

        //hybrid mode or dual hybrid mode , keep same bit resolution with scaler.
        if((u4Mode & FBM_POOL_MODE_MDDI_HYBRID) || (u4Mode & FBM_POOL_MODE_MDDI_DUAL_HYBRID))
        {
            if(u4ScalerMode & FBM_POOL_MODE_8BIT)
            {
                u4Mode &= ~FBM_POOL_MODE_10BIT;
                u4Mode |= FBM_POOL_MODE_8BIT;
            }
            else
            {
                u4Mode &= ~FBM_POOL_MODE_8BIT;
                u4Mode |= FBM_POOL_MODE_10BIT;
            }
        }
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_NR_OFF|FBM_POOL_MODE_MDDI_BOB;
    }

#if defined(CC_DOWN_BW_WITH_MENU)
    JUDGE_TO_DOWN_PSCANNR_PQ();
#endif

#if defined(CC_MHEG5_DISP_WA)||defined(CC_SMALL_WINDOW_USE_DRAM_MODE)
    GIVEUP_PSCANNR_DRAM();
#endif

    SRM_UPDATE_BOB(VDP_1, SRM_CTRL_MODE_NONE, u4Mode, 0, u4PscanModeEx);

    //========================
    //VDP_2 Path Confiugration
    //========================
    u4Mode = 0;
    u4Mode = FBM_POOL_MODE_8BIT | FBM_POOL_MODE_MDDI_NR_OFF;
    u4PscanModeEx = 0;
	
    if (_SrmGetScposMode(VDP_2,prSrmVdpCtrl) & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
    {
        u4Mode |= FBM_POOL_MODE_MDDI_DISP;
    }
    if (SRM_IsScanMode(VDP_2))
    {
        if (SRM_IsPopVideo())
        {
            u4Mode |= FBM_POOL_MODE_MDDI_FULL;
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_CBOB;
        }
    }
    else if (SRM_CheckStable(&prSrmVdpInfo[VDP_2]) &&
    	(SRM_GetMmModeByPath(VDP_2) != SRM_MM_MODE_TYPE_THUMBNAIL) &&
        (prSrmVdpInfo[VDP_2].u4VgaTiming == 0))
    {
        SET_RESOLUTION(prSrmVdpInfo[VDP_2].u4Resolution, u4Mode);

        if (prSrmVdpInfo[VDP_2].u4Interlace == 1)
        {   
            if (SRM_IsPopVideo())
            {
                u4Mode |= FBM_POOL_MODE_MDDI_FULL;
            }
            else
            {
                u4Mode |= FBM_POOL_MODE_MDDI_CBOB; // in sub pip, pscan DONT support full mode

                if (DRVCUST_OptGet(eDdrClock) < 800000000)
                {
                    u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
                    u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
                    u4Mode |= FBM_POOL_MODE_MDDI_BOB;
                }
            }
        }
        else
        {
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }

        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode &= ~FBM_POOL_MODE_MDDI_FULL;
            u4Mode &= ~FBM_POOL_MODE_MDDI_CBOB;
            u4Mode |= FBM_POOL_MODE_MDDI_BOB;
        }
    }
    else
    {
        u4Mode = FBM_POOL_MODE_MDDI_BOB;
    }
    
#if defined(CC_DOWN_BW_WITH_MENU)
    JUDGE_TO_DOWN_PSCANNR_PQ();
#endif
    
    //SRM_UPDATE_BOB(VDP_2, u4CtrlMode, u4Mode, 0, 0);
    SRM_UPDATE_BOB(VDP_2, SRM_CTRL_MODE_NONE, u4Mode, 0, u4PscanModeEx);
    
    return u4Mode;
}
#endif

#if defined(CC_MT5399)||defined(CC_MT5890)
extern SRM_TV_MODE_T _eApplicationMode;
static UINT32 _SrmMJCRule5395(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    UINT32 u4Mode = 0;
    UINT32 u4MJCModeEx = u4DrvTDTVMJCModeQuery();

    SRM_CHECK_INFO;

    if (IS_Support60MJC() == FALSE)
    {
        u4Mode = FBM_POOL_MODE_FRC_Y10_C10_444;
    }
	else
    {
    	SET_RESOLUTION(prSrmVdpInfo[VDP_1].u4Resolution, u4Mode);

    	if (SRM_IsGameMode(VDP_1)
			#ifndef MJC_MASK_FUCTION_FOR_2K15_SONY
			||(SRM_IsPipVideo() && (_eApplicationMode == SRM_TV_MODE_TYPE_PIP)) || SRM_IsPopVideo() 
			#endif 
			|| (prSrmVdpInfo[VDP_1].u4VgaTiming == 1))
    	{
    	    u4Mode = FBM_POOL_MODE_FRC_Y10_C10_444;
    	}
        else if (fgDrvMJCIsMEMCSupported() == FALSE)
        {
            u4Mode = FBM_POOL_MODE_FRC_Y10_C10_444;
        }
    	else if (SRM_IsScanMode(VDP_1))
    	{
    	    u4Mode = FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
    	}
    	else
    	{
    	    if (SRM_CheckStable(&prSrmVdpInfo[VDP_1]))
    	    {
                if (DRVCUST_OptGet(eDdrClock) > 1100000000)
                {
                    //use 422 mode to avoid chroma missing when odd vertical oversan.
                    u4Mode = FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM;
                }
        	    else
        	    {
                    if (prSrmVdpInfo[VDP_1].u4Source != (UINT32)VSS_DTV)
                    {
                        u4Mode = FBM_POOL_MODE_MJC_Y08_C08_422_1RDC_3FRM;
                    }
                    else
                    {
                        u4Mode = FBM_POOL_MODE_MJC_Y10_C08_420_0RDC_3FRM;
                    }
                }
            }
            else
            {
                u4Mode = FBM_POOL_MODE_UNSTABLE;
            }
        }
    }

	//force configure y8c8-422 repeat mode  if output 4K2K, and ignore any other conditions [Orxy]
#if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
	//4k2k in , 4k2k out
	if(fgIs4k2kNativeTimingOut(VDP_1))
	{
		u4Mode = FBM_POOL_MODE_FRC_Y08_C08_422;
	}
	//4k2k in, not 4k2k out
	else if (prSrmVdpInfo[VDP_1].u4Resolution > SRM_VDP_1080HD_RESOLUTION)
    {//	4k2k  2D
 	   u4Mode |= FBM_POOL_MODE_MJC_Y08_C08_422_1RDC_3FRM;
    }	
	#endif
    SRM_UPDATE_MJC(VDP_1, SRM_CTRL_MODE_NONE, u4Mode, 0, u4MJCModeEx);
    return u4Mode;
}
#endif

// add for MPEG buffer change case
static UINT32 _SrmStableRule(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    static UINT32 cnt = 0; // force to call the callback
    UINT32 u4Mode=0;
    if (SRM_ISMMBufChg(VDP_1))
    {
        cnt++;
        SRM_CLRMMBufChg(VDP_1);
        SRM_UPDATE_MPEG_SIZE(VDP_1, SRM_CTRL_MODE_NONE, 0, 0, cnt);
    }
    if (SRM_ISMMBufChg(VDP_2))
    {
        cnt++;
        SRM_CLRMMBufChg(VDP_2);
        SRM_UPDATE_MPEG_SIZE(VDP_2, SRM_CTRL_MODE_NONE, 0, 0, cnt);
    }
    if(SRM_CheckStable(&prSrmVdpInfo[VDP_1]))
    {
        u4Mode=FBM_POOL_MODE_10BIT;
        if (SRM_IsGameMode(VDP_1))
        {
            u4Mode |= FBM_POOL_MODE_TOGGLE_TRIGGER;
        }
        else
        {
            u4Mode &= ~FBM_POOL_MODE_TOGGLE_TRIGGER;
        }
                
        SRM_UPDATE_TDC(VDP_1, 1, u4Mode, 0, 0);
    }
    else
    {
        u4Mode=FBM_POOL_MODE_UNSTABLE;
        SRM_UPDATE_TDC(VDP_1, 1, u4Mode, 0, 0);
    }
    if(SRM_CheckStable(&prSrmVdpInfo[VDP_2]))
    {
        u4Mode=FBM_POOL_MODE_10BIT;
        if (SRM_IsGameMode(VDP_2))
        {
            u4Mode |= FBM_POOL_MODE_TOGGLE_TRIGGER;
        }
        else
        {
            u4Mode &= ~FBM_POOL_MODE_TOGGLE_TRIGGER;
        }
        
        SRM_UPDATE_TDC(VDP_2, 1, u4Mode, 0, 0);
    }
    else
    {
        u4Mode=FBM_POOL_MODE_UNSTABLE;
        SRM_UPDATE_TDC(VDP_2, 1, u4Mode, 0, 0);
    }
    return 0;
}

static UINT32 _SrmDummyRule(SRM_VDP_INFO_T* prSrmVdpInfo, SRM_VDP_CRTL_T* prSrmVdpCtrl)
{
    return 0;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

VOID SRM_LoadRule(FN_SRN_DB_RULE **papfStableRule, UINT32 *pu4StableRuleNs,
                  FN_SRN_DB_RULE **papfNotStableRule, UINT32 *pu4NotStableRuleNs)
{
    if (!papfStableRule || !pu4StableRuleNs || !papfNotStableRule || !pu4NotStableRuleNs)
    {
        return;
    }    
#if (defined(CC_MT5399)&&!defined(CC_MT5882))||defined(CC_MT5890)
     if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV)
	 	||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_4K)
	 	||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_FHD)
	 	||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5890_3DTV_2K80)
	 	||(SRMFBM_GetConf() == FBM_MEM_CFG_MT5861_3DTV))
    {
        UINT8 idx = 0;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmScposRule5890_DDRXx2;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMddiRule5890_DDRXx2;
	    papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMJCRule5395;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = idx;
    }
#endif
#if defined(CC_MT5882)
     if ((SRMFBM_GetConf() == FBM_MEM_CFG_MT5882_3DTV)||
	 	(SRMFBM_GetConf() == FBM_MEM_CFG_MT5882_3DTV_768))
    {
        UINT8 idx = 0;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmScposRule5882_DDRXx2;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmMddiRule5882_DDRXx2;
        papfStableRule[idx++] = (FN_SRN_DB_RULE*)_SrmStableRule;
        *pu4StableRuleNs = idx;
    }
#endif
    else
    {
        *pu4StableRuleNs = 0;
        *pu4NotStableRuleNs = 0;

        // to solve warning
        papfNotStableRule[0] = (FN_SRN_DB_RULE*)_SrmDummyRule;

        VERIFY(0);
    }
}

UINT32 SRM_CheckDisplayMode(UINT32 u4Source, UINT32 u4Resolution,
                            UINT32 u4InputFrameRate, UINT32 u4VgaTiming, UINT32 u4Interlace)
{
    return SRM_GetVdpCtrl(VDP_1)->rPrm[SRM_DISP_CTRL].u4Arg1;
}

UINT32 SRM_CheckMddiMode(UINT32 u4Source, UINT32 u4Resolution,
                         UINT32 u4InputFrameRate, UINT32 u4Interlace)
{
    // this function is only for SS, and SS will enable MDDi for all interlace input source
    // so don't need to process SRM rules
    if (u4Interlace == TRUE)
    {
        // use mddi
        return TRUE;
    }

    UNUSED(u4Source);
    UNUSED(u4Resolution);
    UNUSED(u4InputFrameRate);

    return FALSE;
}
