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
 * $Date: 2015/01/23 $
 * $RCSfile: osd_base_if.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_base_if.c
 *  osd base header file, includes general osd setting
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_bim.h"
#include "osd_hw.h"
#include "osd_drvif.h"
#include "osd_exp.h"
#include "osd_debug.h"
#ifndef CC_MTK_LOADER
#include "gfx_if.h"
#include "drvcust_if.h"
//#include "ostg_if.h"
#include "x_os.h"
#include "x_timer.h"
#endif
#include "fbm_drvif.h"

LINT_EXT_HEADER_BEGIN
#include "x_hal_5381.h"
#include "panel.h"
LINT_EXT_HEADER_END

#include "eeprom_if.h"

#ifdef CC_TDTV_3D_OSD_ENABLE
#include "drv_tdtv_drvif.h"
#endif

#include "drv_display.h"
#include "sv_const.h"
#include "drv_scaler.h"
#include "drv_ttd.h"
#ifdef SMALL_OUTWINDOW_TTD_NEWSETTING
#include "hw_ttd.h"
#endif

#ifdef OSD_4K2K_SUPPORT
#include "general.h"
#include "x_pinmux.h"
#include "drvcust_if.h"
#include "source_select.h"
#include "hw_mlvds.h"
#include "drv_mjc.h"
#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_clksrc.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
#endif

#ifndef OSD_LOGO_FADING_ENABLE
#define OSD_LOGO_FADING_ENABLE (1)
#endif

#define CHECK_OSD_CPBLTY_PARA(u4_type,u4_idx)    \
{                                                   \
    if(u4_type>=E_OSD_CPBLTY_MAX)                  \
    {                                               \
        return OSD_RET_INV_ARG;                    \
    }                                               \
    if(u4_idx>=OSD_PLANE_MAX_NUM)                  \
    {                                               \
        return OSD_RET_INV_ARG;                    \
    }                                               \
}

#define OSD_CHECK_FLIP_MIRROR(u4_plane)             \
  (OSD_GetCpblty(E_OSD_CPBLTY_FLIP, 0x0)||OSD_GetCpblty(E_OSD_CPBLTY_MIRROR,0x0))



//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 _u4Clock = OSD_CK_XTAL;
static BOOL _fgYuvOutput = FALSE;
static BOOL OSD_HStart_FOR_LOGO = FALSE;
static INT32 i4Hstart = 0x0;
#ifdef CC_FASTBOOT_DRIVER_LOGO
static BOOL bFastBootDriverLogo = FALSE;
#endif
#if defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
#ifdef CC_LOADER_LOGO_LONG_TIME
static UINT32 u4OSDTopLayer = OSD_PLANE_3;
static UINT32 u4OSDMiddLayer =OSD_PLANE_2;
static UINT32 u4OSDBotLayer = OSD_PLANE_1;
#else
static UINT32 u4OSDTopLayer = OSD_PLANE_2;
static UINT32 u4OSDMiddLayer =OSD_PLANE_3;
static UINT32 u4OSDBotLayer = OSD_PLANE_1;
#endif
#endif
#ifdef OSD_TRANS_CTRL
static UINT32 _OSD_PlANE2_TransferBuf_1 = (UINT32)NULL;
static UINT32 _OSD_PlANE2_TransferBuf_2 = (UINT32)NULL;
static UINT32 _OSD_PlANE2_TransferBuf_CURRENT = (UINT32)NULL;
#endif

#if defined(CC_SUPPORT_STR)||defined(CC_LG_SNAP_SHOT)
static BOOL b_suspend = FALSE;
#endif


//#ifdef CC_MT5363
//static BOOL gfgSwap=FALSE;
//BOOL gfgSwap=FALSE;
//#endif
#ifndef CC_MTK_LOADER
static BOOL fgLogoInit = FALSE;
BOOL bLogoDisplay = FALSE;
static HANDLE_T _hOsdWaitVsyncSema;
static volatile UINT32 _u4FlipCnt = 0;
static HANDLE_T _hOsdWaitVsyncSema2;
static volatile UINT32 _u4FlipCnt2 = 0;
static BOOL _fgVsyncEn = FALSE;
static HANDLE_T _hOsdFlipToSema;
static HANDLE_T _hOsdLogoSema;
#endif
static UINT32 _u4_vsync_time = 0;

#ifdef CC_TDTV_3D_OSD_ENABLE
BOOL gfgOsd_Sbs = FALSE;
BOOL gfgOsd_Tbb = FALSE;
UINT8 _u13DOsdShift = 0;
#endif

#ifdef CC_TDTV_3D_OSD_ENABLE
static UINT8 _u1Plane2RightRegion = 0;
static UINT8 _u1Plane3RightRegion = 0;

#endif

UINT8 u1FlipMirrorReset = 0;
static UINT32 _fgOsdInit = 0;

static UINT32 _fgTimNfyCnt = 0;
static BOOL OSD2_3DPrLshift_ENABLE_FLAG = FALSE;
static OSD_CPBLTY t_osd_cpblty;

static UINT32 u4_osd_out_v_max      =0x0;
static UINT32 u4_osd_active_v_start      =0x0;

static UINT32 u4_osd_under_flow_up  =0x0;

#define OSD_DBG_INFO_MAX      0x20

UINT32 u4_osd_dbg_info[OSD_DBG_INFO_MAX]  ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#define OSD_FRONT_PORTH_S      u4_osd_out_v_max
#define OSD_ACTIVE_V_S         u4_osd_active_v_start

#define OSD_UNDER_FLOW_ISR_FG  u4_osd_under_flow_up

#ifdef OSD_4K2K_SUPPORT
static HANDLE_T _hOsdVsyncDetectTimer = NULL_HANDLE;
UINT32 _u4FrameCnt = 1;
extern UINT32 u4LinuxUiMode4K2K ;
extern OSD_4K2KOriInformation OSD2OriInformation;
extern OSD_4K2KOriInformation OSD3OriInformation;
extern OSD_4K2KJpecModeCurrentSet OSD2JpecModeCurrentSet;
extern OSD_4K2KJpecModeCurrentSet OSD3JpecModeCurrentSet;
extern OSD_4K2KMvcModeCurrentSet  OSD2MvcModeCurrentSet;
extern OSD_4K2KMvcModeCurrentSet  OSD3MvcModeCurrentSet;
#endif


// #define USE_OSD_CK_REG
#ifdef CC_UBOOT
UINT32 _fgOsdEnabelUpdate = 1;
#else
UINT32 _fgOsdEnabelUpdate = 1;
#endif

#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
static x_os_isr_fct pfnOldIsr;
static BOOL bIsCreateISR = FALSE;
static void vOsdHeaderUpdateCallbak(UINT16 u2Vector);
#endif

static UINT8 bVyncCouter = 3;

void OSD_Set_Dbg_Info(UINT32 u4_idx,UINT32 u4_val)
{
    if(u4_idx >= OSD_DBG_INFO_MAX)
    {
        return;
    }

    u4_osd_dbg_info[u4_idx] = u4_val;
}

void OSD_Set_Dbg_Info_CS(void)
{
    u4_osd_dbg_info[7] = (OSD_READ32(0x4c)&0x7FF);
    u4_osd_dbg_info[8] = (OSD_READ32(0x60)&0x7FF);
}

UINT32 OSD_Get_Dbg_Info(UINT32 u4_idx)
{
    if(u4_idx >= OSD_DBG_INFO_MAX)
    {
        return OSD_INVALID_VALUE_U32;
    }

    return u4_osd_dbg_info[u4_idx];
}

static void OSD_Base_RFIFO_UNDERFLOW_ISR(UINT16 u2Vector)
{
    if(u2Vector == VECTOR_OSD)
    {
        OSD_Base_UnderFlow_Isr_Action();
        VERIFY(BIM_ClearIrq(VECTOR_OSD));
    }

    return;
}

#ifndef CC_UBOOT

void OSD_Base_Reg_Isr(void)
{
    static UINT32 u4_init = 0x0;

    x_os_isr_fct pfnOldIsr;

    if(!u4_init)
    {
        VERIFY(x_reg_isr((UINT16) VECTOR_OSD, OSD_Base_RFIFO_UNDERFLOW_ISR, &pfnOldIsr)== OSR_OK);
        u4_init = 0x1;
        LOG(5,"OSD_Base_Reg_Isr Ok\n");
    }

    return;
}
#endif

void OSD_Reset_UnderFlow(void)
{
    OSD_WRITE32(OSD_FIFO_MONITOR_OFFSET, (OSD_READ32(OSD_FIFO_MONITOR_OFFSET)|(1<<2)));

    return;
}

UINT32 OSD_Base_Check_Abnormal_Palne(UINT32 u4_plane_base)
{
    INT32 i4_60     = 0x0;
    UINT32 u4_v_s   = 0x0;
    UINT32 u4_4c    = 0x0;
    INT32 i4_60_exp = 0x0;

    if(!(OSD_READ32(u4_plane_base)&0x1))
    {
        return 0x0;
    }

    if(OSD_READ32(0xFC)&0x1)
    {
        u4_osd_dbg_info[4]++;
        return 0x1;
    }

    if(!(OSD_READ32(u4_plane_base+0x38)&0x800))
    {
        u4_osd_dbg_info[5]++;
        return 0x1;
    }

    if(OSD_PLA3_REG_OFF == u4_plane_base)
    {
        IO_WRITE32(OSD_DE_S_REG, 0, OSD_DE_S_OSD3_VAL);
    }
    else if(OSD_PLA2_REG_OFF == u4_plane_base)
    {
        IO_WRITE32(OSD_DE_S_REG, 0, OSD_DE_S_OSD2_VAL);
    }
    else
    {
        return 0x0;
    }

    if((OSD_READ32(0x4c)&0x7FF) >= OSD_FRONT_PORTH_S)
    {
        i4_60 = (OSD_READ32(0x60)&0x7FF);
        if(OSD_PLA2_REG_OFF == u4_plane_base)
        {
            if((i4_60 + OSD_DEC_DELAY_MAX) <
                (OSD_READ32(OSD_SC2_REG_OFF+0x4)&0x7FF))
            {
                u4_osd_dbg_info[6]++;
                u4_osd_dbg_info[10]=(OSD_READ32(0x4c)&0x7FF);
                u4_osd_dbg_info[11]=i4_60;
                return 0x1;
            }
        }
        else if(OSD_PLA3_REG_OFF == u4_plane_base)
        {
            if((i4_60 + OSD_DEC_DELAY_MAX) <
                (OSD_READ32(OSD_SC3_REG_OFF+0x4)&0x7FF))
            {
                u4_osd_dbg_info[6]++;
                u4_osd_dbg_info[10]=(OSD_READ32(0x4c)&0x7FF);
                u4_osd_dbg_info[11]=i4_60;
                return 0x1;
            }
        }
        else
        {
            u4_osd_dbg_info[12]=(OSD_READ32(0x4c)&0x7FF);
            u4_osd_dbg_info[13]=i4_60;
        }
    }
    else if((OSD_READ32(0x4c)&0x7FF) > (OSD_ACTIVE_V_S+OSD_DEC_DELAY_MAX))
    {
        u4_4c   = (OSD_READ32(0x4c)&0x7FF);
        i4_60   = (OSD_READ32(0x60)&0x7FF);

        if(OSD_PLA2_REG_OFF == u4_plane_base)
        {
            u4_v_s  = (OSD_READ32(0x504)&0x7FF);
        }
        else if(OSD_PLA3_REG_OFF == u4_plane_base)
        {
            u4_v_s  = (OSD_READ32(0x604)&0x7FF);
        }

        i4_60_exp = (INT32)((u4_4c*u4_v_s)/OSD_FRONT_PORTH_S);

        if((i4_60 == 0x0)||(i4_60 < (i4_60_exp - OSD_DEC_DELAY_MAX))||(i4_60 > (i4_60_exp + OSD_DEC_DELAY_MAX)))
        {
            u4_osd_dbg_info[14] = i4_60_exp;
            u4_osd_dbg_info[15] = i4_60;
            u4_osd_dbg_info[16] = u4_4c;
            u4_osd_dbg_info[17] = u4_v_s;
            u4_osd_dbg_info[18]++;
            return 0x1;
        }

    }

    if(OSD_READ32(0xFC)&0x1)
    {
        u4_osd_dbg_info[4]++;
        return 0x1;
    }

    return 0x0;
}

UINT32 OSD_Base_Check_Abnormal_Status(void)
{
    if(OSD_PLA_Get_En(OSD_PLANE_2)&&OSD_PLA_Get_Dec(OSD_PLANE_2))
    {
        if(OSD_Base_Check_Abnormal_Palne(OSD_PLA2_REG_OFF))
        {
            return 0x1;
        }
    }

    if(OSD_PLA_Get_En(OSD_PLANE_3)&&OSD_PLA_Get_Dec(OSD_PLANE_3))
    {
        if(OSD_Base_Check_Abnormal_Palne(OSD_PLA3_REG_OFF))
        {
            return 0x1;
        }
    }

    return 0x0;
}


#if 0

{
    if((OSD_READ32(0x300)&0x1)&&(OSD_READ32(0x338)&0x800))
    {
        OSD_Reset_Wait_FP();
        if(OSD_GetUnderflowStatus())
        {
            return 0x1;
        }
    }

    return 0x0;
}

UINT32 u4_v_s       = 0x0;
UINT32 u4_rgn_w     = 0x0;

if((OSD_READ32(0x300)&0x1))
{
    IO_WRITE32(OSD_DE_S_REG, 0, OSD_DE_S_OSD3_VAL);
    u4_v_s      = (OSD_READ32(0x604)&0x7FF);
    u4_rgn_w    = (OSD_READ32(0x338)&0x7FF);

    if(u4_rgn_w != u4_v_s)
    {
        return 0x1;
    }

    if(!(OSD_READ32(0x338)&0x800))
    {
        return 0x1;
    }

    OSD_Reset_Wait_FP();

    if(OSD_GetUnderflowStatus())
    {
        return 0x1;
    }
}
else  if((OSD_READ32(0x200)&0x1))
{
    IO_WRITE32(OSD_DE_S_REG, 0, OSD_DE_S_OSD3_VAL);
    u4_v_s      = (OSD_READ32(0x504)&0x7FF);
    u4_rgn_w    = (OSD_READ32(0x238)&0x7FF);

    if(u4_rgn_w != u4_v_s)
    {
        return 0x1;
    }

    if(!(OSD_READ32(0x238)&0x800))
    {
        return 0x1;
    }

    OSD_Reset_Wait_FP();
    if(OSD_GetUnderflowStatus())
    {
        return 0x1;
    }
}
#endif


void OSD_Reset_FSM_Step1(void)
{
    OSD_WRITE32(0x0FC, (OSD_READ32(0xFC)|(1<<2)));
    OSD_WRITE32(0x294, (OSD_READ32(0x294)|((1<<6))));
    OSD_WRITE32(0x294, (OSD_READ32(0x294)&(~(1<<4))));
    OSD_WRITE32(0x394, (OSD_READ32(0x394)|((1<<6))));
    OSD_WRITE32(0x394, (OSD_READ32(0x394)&(~(1<<4))));
    OSD_WRITE32(0x4, 0x3);
    OSD_WRITE32(0x4, 0x2);

    return;
}

void OSD_Reset_FSM_Step2(void)
{
    OSD_WRITE32(0x294, (OSD_READ32(0x294)&(~(1<<6))));
    OSD_WRITE32(0x294, (OSD_READ32(0x294)|((1<<4))));
    OSD_WRITE32(0x394, (OSD_READ32(0x394)&(~(1<<6))));
    OSD_WRITE32(0x394, (OSD_READ32(0x394)|((1<<4))));
    OSD_WRITE32(0x4, 0x12);
    OSD_WRITE32(0x4, 0x13);
    OSD_WRITE32(0x0, 0x1);

    return;
}

void OSD_Base_UnderFlow_Isr_Action(void)
{
    if((OSD_READ32(0xFC)&0x1))
    {
        if(OSD_UNDER_FLOW_ISR_FG)
        {
            return;
        }

        OSD_Reset_FSM_Step1();
        OSD_UNDER_FLOW_ISR_FG=0x1;
    }
}

void OSD_Base_Check_Underflow_Isr_Action(void)
{
    if(OSD_UNDER_FLOW_ISR_FG)
    {
        if(OSD_UNDER_FLOW_ISR_FG++ > 0x3)
        {
            OSD_Reset_FSM_Step2();
            OSD_UNDER_FLOW_ISR_FG=0x0;
        }
    }
}

void OSD_Base_Check_Underflow_Action(void)
{
    static UINT32 u4_delay_cnt      = 0;
    static UINT32 u4_step           = 0x1;

    if(!(u4_osd_dbg_info[0]&0x10000000))
    {
        return ;
    }

    if(0x1 == u4_step)
    {
        if(OSD_Base_Check_Abnormal_Status())
        {
            OSD_Reset_FSM_Step1();
            u4_delay_cnt    = 0x1;
            u4_step         = 0x2;
            u4_osd_dbg_info[10]++;
        }
    }
    else
    {
        if(u4_delay_cnt)
        {
            if(u4_delay_cnt++ > 0x3)
            {
                OSD_Reset_FSM_Step2();
                u4_delay_cnt    = 0x0;
                u4_step         = 0x1;
            }
        }
    }
}

INT32 OSD_SetCpblty(UINT32 u4_type,UINT32 u4_idx,UINT32 u4_val)
{
    INT32 i4_ret = OSD_RET_OK;

    CHECK_OSD_CPBLTY_PARA(u4_type,u4_idx);

    switch(u4_type)
    {
        case E_OSD_CPBLTY_CMPRSS:
            t_osd_cpblty.u4_cmprss = u4_val;
            break;
        case E_OSD_CPBLTY_CMPRSS_PLANE:
            t_osd_cpblty.u4_cmprss_plane[u4_idx] = u4_val;
            break;
        case E_OSD_CPBLTY_SBS_PLANE:
            t_osd_cpblty.u4_sbs_plane[u4_idx] = u4_val;
            break;
        case E_OSD_CPBLTY_3D_STATUS:
            t_osd_cpblty.u4_3d_status[u4_idx] = u4_val;
            break;
        case E_OSD_CPBLTY_FLIP:
            t_osd_cpblty.u4_flip = u4_val;
            break;
        case E_OSD_CPBLTY_MIRROR:
            t_osd_cpblty.u4_mirror = u4_val;
            break;
        default:
            i4_ret = OSD_RET_INV_ARG;
            break;
    }

    return i4_ret;
}

UINT32 OSD_GetCpblty(UINT32 u4_type,UINT32 u4_idx)
{
    UINT32 u4_val = OSD_INVALID_VALUE_U32;

    CHECK_OSD_CPBLTY_PARA(u4_type,u4_idx);

    switch(u4_type)
    {
        case E_OSD_CPBLTY_CMPRSS:
            u4_val = t_osd_cpblty.u4_cmprss;
            break;
        case E_OSD_CPBLTY_CMPRSS_PLANE:
            u4_val = t_osd_cpblty.u4_cmprss_plane[u4_idx];
            break;
        case E_OSD_CPBLTY_SBS_PLANE:
            u4_val = t_osd_cpblty.u4_sbs_plane[u4_idx];
            break;
        case E_OSD_CPBLTY_3D_STATUS:
            u4_val = t_osd_cpblty.u4_3d_status[u4_idx];
            break;
        case E_OSD_CPBLTY_FLIP:
            u4_val = t_osd_cpblty.u4_flip;
            break;
        case E_OSD_CPBLTY_MIRROR:
            u4_val = t_osd_cpblty.u4_mirror;
            break;
        default:
            break;
    }

    return u4_val;
}

INT32 OSD_SetFb3DCfg(UINT32 u4_plane, UINT32 u4_3d_mode, UINT32 u4_sync)
{
    INT32  i4_ret       = OSD_RET_OK;
    #ifndef CC_MTK_LOADER
    #ifdef LINUX_TURNKEY_SOLUTION
    #ifndef CONFIG_TV_DRV_VFY
    OSD_SetCpblty(E_OSD_CPBLTY_3D_STATUS,u4_plane,u4_3d_mode);
    if(u4_sync)
    {
        i4_ret = fb_osd_reupdate_bmp_addr(u4_plane);
    }
    #endif
    #endif
    #endif
    return i4_ret;
}

void OSD_InitCpblty(void)
{
    x_memset(&t_osd_cpblty, 0, sizeof(t_osd_cpblty));

    {
        #ifdef CONFIG_OSD_PLANE1_COMPRESSION
        OSD_SetCpblty(E_OSD_CPBLTY_CMPRSS_PLANE,OSD_PLANE_1,0x1);
        #endif

        #ifdef CONFIG_OSD_PLANE2_COMPRESSION
        OSD_SetCpblty(E_OSD_CPBLTY_CMPRSS_PLANE,OSD_PLANE_2,0x1);
        #endif

        #ifdef CONFIG_OSD_PLANE3_COMPRESSION
        OSD_SetCpblty(E_OSD_CPBLTY_CMPRSS_PLANE,OSD_PLANE_3,0x1);
        #endif
    }

    {
        #ifdef CC_CONFIG_OSD_SBS_PLANE1
        OSD_SetCpblty(E_OSD_CPBLTY_SBS_PLANE,OSD_PLANE_1,OSD_3D_SBSHALF_PR);
        #endif

        #ifdef CC_CONFIG_OSD_SBS_PLANE2
        OSD_SetCpblty(E_OSD_CPBLTY_SBS_PLANE,OSD_PLANE_2,OSD_3D_SBSHALF_PR);
        #endif

        #ifdef CC_CONFIG_OSD_SBS_PLANE3
        OSD_SetCpblty(E_OSD_CPBLTY_SBS_PLANE,OSD_PLANE_3,OSD_3D_SBSHALF_PR);
        #endif

        #ifdef CC_CONFIG_OSD_TB_PLANE1
        OSD_SetCpblty(E_OSD_CPBLTY_SBS_PLANE,OSD_PLANE_1,OSD_3D_TPANDBT_PR);
        #endif

        #ifdef CC_CONFIG_OSD_TB_PLANE2
        OSD_SetCpblty(E_OSD_CPBLTY_SBS_PLANE,OSD_PLANE_2,OSD_3D_TPANDBT_PR);
        #endif

        #ifdef CC_CONFIG_OSD_TB_PLANE3
        OSD_SetCpblty(E_OSD_CPBLTY_SBS_PLANE,OSD_PLANE_3,OSD_3D_TPANDBT_PR);
        #endif

        OSD_SetCpblty(E_OSD_CPBLTY_3D_STATUS,OSD_PLANE_1,OSD_3D_NONE);
        OSD_SetCpblty(E_OSD_CPBLTY_3D_STATUS,OSD_PLANE_2,OSD_3D_NONE);
        OSD_SetCpblty(E_OSD_CPBLTY_3D_STATUS,OSD_PLANE_3,OSD_3D_NONE);
    }
    return ;
}


BOOL OSD_Base_Is_Osd_Do_Flip_Mirror(UINT32 u4_plane)
{
    if(OSD_CHECK_FLIP_MIRROR(u4_plane)&&
        !OSD_GetCpblty(E_OSD_CPBLTY_CMPRSS_PLANE,u4_plane))
    {
        return TRUE;
    }

   return FALSE;
}

BOOL OSD_Base_Is_Gfx_Do_Flip_Mirror(UINT32 u4_plane)
{
    if(OSD_CHECK_FLIP_MIRROR(u4_plane)&&
        OSD_GetCpblty(E_OSD_CPBLTY_CMPRSS_PLANE,u4_plane))
    {
        return TRUE;
    }

   return FALSE;
}


void OSD_UpdateReg(void)
{
     _OSD_UpdateReg();
}


void OSD_TimingNfy(void)
{
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    if(!IS_IC_5399_ES2())
    {
        _fgTimNfyCnt = 5;
    }
#else
    _fgTimNfyCnt = 5;
#endif
}

void OSD_DoOutISR(void)
{
    if(_fgTimNfyCnt == 3)
    {
        OSD_WRITE32(OSD_BASE_MONITOR,OSD_READ32(OSD_BASE_MONITOR)|UPDATE_TIMING_BIT);
    }
    else if(_fgTimNfyCnt == 2)
    {
        OSD_WRITE32(OSD_BASE_MONITOR,OSD_READ32(OSD_BASE_MONITOR)&(~UPDATE_TIMING_BIT));
        _OSD_UpdateReg();
    }

    if(_fgTimNfyCnt > 0)
    {
        _fgTimNfyCnt--;
    }
}



void OSD_LockFlipTo(void)
{
#ifndef CC_MTK_LOADER
    VERIFY(x_sema_lock(_hOsdFlipToSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif
}

void OSD_UnLockFlipTo(void)
{
#ifndef CC_MTK_LOADER
    VERIFY(x_sema_unlock(_hOsdFlipToSema) == OSR_OK);
#endif
}

void OSD_LockLogoDisplaying(void)
{
#ifndef CC_MTK_LOADER
    //VERIFY(x_sema_lock(_hOsdLogoSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif
}

void OSD_UnLockLogoDisplaying(void)
{
#ifndef CC_MTK_LOADER
    //VERIFY(x_sema_unlock(_hOsdLogoSema) == OSR_OK);
#endif
}

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static INT32 _OsdBaseInit(void)
{
    IGNORE_RET(_OSD_BASE_SetVsWidth(1));
    IGNORE_RET(_OSD_BASE_SetOsd1HStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd2HStart(0));
    //IGNORE_RET(_OSD_BASE_SetOsd3HStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd1VStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd2VStart(0));
    //#ifdef CC_MT5363
    IGNORE_RET(_OSD_BASE_SetOsd3HStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd3VStart(0));
    //#endif

    OSD_Base_SetHStart();

    return (INT32) OSD_RET_OK;
}

void vSetOsdFlipMirror(BOOL fgMirrorEn, BOOL fgFlipEn)
{
    OSD_SetCpblty(E_OSD_CPBLTY_MIRROR,0,fgMirrorEn);
    // printk("vSetOsdFlipMirror FLIP \n");
    OSD_SetCpblty(E_OSD_CPBLTY_FLIP,0,fgFlipEn);
    _OSD_RGN_AllFlipMirrorMode();

    return;
}

#ifndef CC_FLIP_MIRROR_FROM_DDI
#include "drv_display.h"
static void _OsdGetMirrorFlipSetting(void)
{
    UINT8 u1Config=0x0;

    #ifdef DRV_USE_EXTERNAL_3D_FRC // MT8283_FLIP_MIRROR
    {
        u1Config= ((u4GetFlipMirrorModule(SV_VP_MAIN)==FLIP_BY_EXT3DFRC)||
        (u4GetFlipMirrorModule(SV_VP_MAIN)==FLIP_NONE))?0:u1GetFlipMirrorConfig();
    }
    #else
    {
        #ifdef CC_FLIP_MIRROR_SUPPORT
     u1FlipMirrorReset = 1;
        u1Config= u1GetFlipMirrorConfig();
        #endif
    }
    #endif

    if(u1Config & SYS_FLIP_CONFIG_ON)
    {
        // printk("_OsdGetMirrorFlipSetting FLIP \n");
        OSD_SetCpblty(E_OSD_CPBLTY_FLIP,0,TRUE);
    }
    else
    {
         //printk("_OsdGetMirrorFlipSetting FLIP \n");
        OSD_SetCpblty(E_OSD_CPBLTY_FLIP,0,FALSE);
    }
    if(u1Config & SYS_MIRROR_CONFIG_ON)
    {
        OSD_SetCpblty(E_OSD_CPBLTY_MIRROR,0,TRUE);
    }
    else
    {
        OSD_SetCpblty(E_OSD_CPBLTY_MIRROR,0,FALSE);
    }
    return;
}
#endif


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

INT32 OSD_BASE_SetClock(UINT32 u4Clock)
{
#ifdef USE_OSD_CK_REG
    if (_u4Clock != u4Clock)
    {
        _u4Clock = u4Clock;
        IO_WRITE32(OSD_CK_REG, 0, u4Clock);
    }
#endif

    return (INT32) OSD_RET_OK;
}

void OSD_WaitVsync(void)
{
#ifndef CC_MTK_LOADER
#if defined(CC_SUPPORT_STR)||defined(CC_LG_SNAP_SHOT)
    if (_fgVsyncEn && b_suspend == FALSE)
#else
    if (_fgVsyncEn)
#endif  
    {
        _u4FlipCnt++;
        //x_sema_lock(_hOsdWaitVsyncSema, X_SEMA_OPTION_NOWAIT);
        VERIFY(x_sema_lock(_hOsdWaitVsyncSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        _u4FlipCnt--;
    }
    else
    {
        x_thread_delay(20);
    }

#endif
}

void OSD_WaitVsync2(void)  //for android hwc vsync thread useful
{
#ifndef CC_MTK_LOADER
#if defined(CC_SUPPORT_STR)||defined(CC_LG_SNAP_SHOT)
    if (_fgVsyncEn && b_suspend == FALSE)
#else
    if (_fgVsyncEn)
#endif      
    {
        _u4FlipCnt2++;
        VERIFY(x_sema_lock(_hOsdWaitVsyncSema2, X_SEMA_OPTION_WAIT) == OSR_OK);
        _u4FlipCnt2--;
    }
    else
    {
        x_thread_delay(20);
    }

#endif
}

void OSD_UnlockVsyncSema(void)
{

#ifndef CC_MTK_LOADER
    HAL_TIME_T t_current;
    static HAL_TIME_T t_last;
    HAL_TIME_T t_delta;
    static UINT32 _u4_vsync_cnt = 0;

    if (!_fgVsyncEn)
    {
        _fgVsyncEn = TRUE;
    }

    if (_u4_vsync_cnt < 0x20)
    {
        HAL_GetTime(&t_current);
        HAL_GetDeltaTime(&t_delta, &t_last, &t_current);
        _u4_vsync_time = ((t_delta.u4Seconds * 1000000) + t_delta.u4Micros);

        t_last = t_current;
        _u4_vsync_cnt++;
    }

    if (_u4FlipCnt > 0)
    {
        x_sema_unlock(_hOsdWaitVsyncSema);
    }

    if (_u4FlipCnt2 > 0)
    {
        x_sema_unlock(_hOsdWaitVsyncSema2);
    }

    OSD_EXP_HandleVSync();
    //if(OSD_Get_Dbg_Info(0x0)&0x2)
    //{
        //OSD_Base_Check_Underflow_Action();
    //}

    //if(OSD_Get_Dbg_Info(0x0)&0x4)
    //{
        //OSD_Base_Check_Underflow_Isr_Action();
    //}
#endif
}

UINT32 OSD_Base_GetVsyncTime(void)
{
    return (_u4_vsync_time);
}
/*just for loader_panel.c use to adjust HStar for logo show*/
void OSD_Base_LogoSetHStart(BOOL value)
{
    UNUSED(OSD_HStart_FOR_LOGO);
    OSD_HStart_FOR_LOGO = value;
}

INT32 OSD_Base_AdjustOSDPosition(INT32 i4Adjust)
{
    i4Hstart +=i4Adjust;
    #if defined(CC_MT5399)
    OSD_BASE_AdjustPlanePosition(u4OSDTopLayer,u4OSDMiddLayer,u4OSDBotLayer,i4Hstart,2);
    #else
    _OSD_BASE_SetScrnHStartOsd1(i4Hstart);
    _OSD_BASE_SetScrnHStartOsd2(i4Hstart);
    _OSD_BASE_SetScrnHStartOsd3(i4Hstart);
    #endif
    _OSD_BASE_SetScrnHStartCSR(i4Hstart-0x10);

    VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());

    return (INT32) OSD_RET_OK;
}

INT32 OSD_Base_AdjustOSDVPosition(INT32 i4Adjust)
{
    INT32 i4Vstart=0;

    i4Vstart +=i4Adjust;
    _OSD_BASE_SetOsd2VStart(i4Vstart);
    VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());

    return (INT32) OSD_RET_OK;
}

VOID OSD_BASE_FSC_AdjustOffset(INT32 *pi4PosH, INT32 *pi4PosV)
{
#if defined(CC_MT5882) || defined(CC_MT5890)
#ifndef CC_MTK_LOADER
    INT16 byp_delay_tab[BYPASS_MAX][2] =
    {
        #if defined(CC_MT5882)
        { 59, 3}, //BYPASS_SHARPNESS
        { 0, 0}, //BYPASS_CBE_CTL  //5882 have remove
        { 0, 0}, //BYPASS_BITPLUS_Y // have no effect on OSD
        { 0, 0}, //BYPASS_BITPLUS_S  //5882 have remove
        { 0,  0}, //BYPASS_3DCOLORMAP   //5882 have remove
        #else
        { 59, 3}, //BYPASS_SHARPNESS
        { 27, 1}, //BYPASS_CBE_CTL
        { 21, 0}, //BYPASS_BITPLUS_Y
        { 21, 0}, //BYPASS_BITPLUS_S
        { 4, 0}, //BYPASS_3DCOLORMAP
        #endif
    };

    ASSERT((pi4PosH != NULL));
    ASSERT((pi4PosV != NULL));

    if(u1DrvSharpnessIsBypass())
    {
        Printf("[OSD]DBG u1DrvSharpnessIsBypass\n");
        *pi4PosH -= byp_delay_tab[BYPASS_SHARPNESS][0];
        *pi4PosV -= byp_delay_tab[BYPASS_SHARPNESS][1];
    }
    if(u1DrvCBEIsBypass())
    {
        Printf("[OSD]DBG u1DrvCBEIsBypass\n");
        *pi4PosH -= byp_delay_tab[BYPASS_CBE_CTL][0];
        *pi4PosV -= byp_delay_tab[BYPASS_CBE_CTL][1];
    }
    
    if(u1DrvBitPlusYIsBypass())
    {
        Printf("[OSD]DBG u1DrvBitPlusYIsBypass\n");
        *pi4PosH -= byp_delay_tab[BYPASS_BITPLUS_Y][0];
        *pi4PosV -= byp_delay_tab[BYPASS_BITPLUS_Y][1];
    }
    if(u1DrvBitPlusSIsBypass())
    {
        Printf("[OSD]DBG u1DrvBitPlusSIsBypass\n");
        *pi4PosH -= byp_delay_tab[BYPASS_BITPLUS_S][0];
        *pi4PosV -= byp_delay_tab[BYPASS_BITPLUS_S][1];
    }
    if(u1Drv3DCMSIsBypass())
    {
        Printf("[OSD]DBG u1Drv3DCMSIsBypass\n");
        *pi4PosH -= byp_delay_tab[BYPASS_3DCOLORMAP][0];
        *pi4PosV -= byp_delay_tab[BYPASS_3DCOLORMAP][1];
    }
#endif
#endif
}

VOID OSD_BASE_UpdateAutoPosition(VOID)
{
    INT32 i4H, i4V;
    UINT32 u4HBackPorch  = 0x0;
    UINT32 u4TimingFrom;//0:from post scaler; 1:from front scaler; 2: from gfx timegen

    i4H = 0;
    i4V = 0;
    if (_fgOsdInit)
    {
        if(fgIsMJCToOSTG())
        {
            u4TimingFrom = 0;
        }
        else
        {
            u4TimingFrom = 1;
        }

        if(u4TimingFrom == 0)
        {
            Printf("[OSD]DBG Timing is Post Scaler \n");
            IGNORE_RET(_OSD_BASE_SetHsEdge(OSD_BASE_HS_EDGE));
            IGNORE_RET(_OSD_BASE_SetVsEdge(OSD_BASE_VS_EDGE));

            u4HBackPorch = wDrvGetHsyncBp();
            i4H = u4HBackPorch +  OSD_BASE_HSART_ADJ;
#if defined(CC_MT5890) || defined(CC_MT5882)
            i4V = 0x3;
#else
            i4V = 0x6;
#endif
        }
        else if(u4TimingFrom == 1)
        {
            Printf("[OSD]DBG Timing is Front Scaler \n");
            #ifdef CC_BYPASS_MJC
            if(IS_DISP_4K2K_TYPE3)
            {
              IGNORE_RET(_OSD_BASE_SetHsEdge(OSD_BASE_HS_EDGE));
              IGNORE_RET(_OSD_BASE_SetVsEdge(OSD_BASE_VS_EDGE));
            i4H = 0x179;
            i4V = 0x20;            
            }
            else
            {
              IGNORE_RET(_OSD_BASE_SetHsEdge(0));
              IGNORE_RET(_OSD_BASE_SetVsEdge(0));
              #if defined(CC_MT5882)
#ifdef SMALL_OUTWINDOW_TTD_NEWSETTING
              i4H = 0x136 - 11; /* need to subtract 11 for LG webos */
#else
              i4H = 0x1D1;
#endif
              i4V = 0x9;
              OSD_BASE_FSC_AdjustOffset(&i4H, &i4V);
              #elif defined(CC_MT5890)
              if(OSD_EXP_IS_GFX_FHD())
              {
                  i4H = 0xBB;
                  i4V = 0x6;
              }
              else if(OSD_EXP_IS_GFX_4K2K())
              {
                  i4H = 0x20B;
                  i4V = 0xC;
              }
              else
              {
                  i4H = 0xE7;
                  i4V = 0x4;
              }
              #else
              i4H = 0xDF;
              i4V = 0xA;
              #endif
            }
            #else
              IGNORE_RET(_OSD_BASE_SetHsEdge(0));
              IGNORE_RET(_OSD_BASE_SetVsEdge(0));
              #if defined(CC_MT5882)
#ifdef SMALL_OUTWINDOW_TTD_NEWSETTING
              i4H = 0x136 - 11; /* need to subtract  11 for LG webos */
#else
              i4H = 0x1D1;
#endif
              i4V = 0x9;
              OSD_BASE_FSC_AdjustOffset(&i4H, &i4V);
              #elif defined(CC_MT5890)
                if(OSD_EXP_IS_GFX_FHD())
                {
                    i4H = 0xBB;
                    i4V = 0x6;
                }
                else if(OSD_EXP_IS_GFX_4K2K())
                {
                    i4H = 0x20B;
                    i4V = 0xC;
                }
                else
                {
                    i4H = 0xE7;
                    i4V = 0x4;
                }     
              #else
              i4H = 0xDF;
              i4V = 0xA;
              #endif

            #endif          
        }

        Printf("[OSD]i4H=%d i4V=%d \n", i4H, i4V);
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        OSD_BASE_AdjustPlanePosition(u4OSDTopLayer,u4OSDMiddLayer,u4OSDBotLayer,i4H,2);
        _OSD_BASE_SetScrnHStartCSR(i4H - 6);
#else
        _OSD_BASE_SetScrnHStartOsd1(i4H);
        _OSD_BASE_SetScrnHStartOsd2(i4H);
        _OSD_BASE_SetScrnHStartOsd3(i4H);
        _OSD_BASE_SetScrnHStartCSR(i4H);
#endif
        _OSD_BASE_SetScrnVStartTop(i4V);
        i4Hstart = i4H;
    }
}

INT32 OSD_Base_SetHStart(void)
{
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    if (!_fgOsdInit)
    {
        return (INT32) OSD_RET_OK;
    }

    _OSD_BASE_SetSyncSrcSel(OSD_BASE_SYNC_SRC_SEL);
    //IGNORE_RET(_OSD_BASE_SetHsEdge(OSD_BASE_HS_EDGE));
    OSD_BASE_UpdateAutoPosition();
    OSD_WRITE32(OSD_BASE_MONITOR,OSD_READ32(OSD_BASE_MONITOR)|UPDATE_TIMING_BIT);
    OSD_WRITE32(OSD_BASE_MONITOR,OSD_READ32(OSD_BASE_MONITOR)&(~UPDATE_TIMING_BIT));

    VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());

#else
    UINT32 u4HBackPorch  = 0x0;
    INT32 i4Vstart = 0x0;
    UINT32 u4OSD1Order = 0;
    UINT32 u4OSD2Order = 0;
    UINT32 u4OSD3Order = 0;
    #if defined(CC_MT5881)
    UINT32 u4PmxOption;
    #endif
    if (!_fgOsdInit)
    {
        return (INT32) OSD_RET_OK;
    }

    _OSD_BASE_SetSyncSrcSel(OSD_BASE_SYNC_SRC_SEL);

    u4HBackPorch = wDrvGetHsyncBp();
    IGNORE_RET(_OSD_BASE_SetHsEdge(OSD_BASE_HS_EDGE));
    IGNORE_RET(_OSD_BASE_SetVsEdge(OSD_BASE_VS_EDGE));

    i4Hstart = (INT32)u4HBackPorch +  OSD_BASE_HSART_ADJ;
    i4Vstart = OSD_BASE_SCRN_VSTART_TOP;

    #ifdef SMALL_OUTWINDOW_TTD_NEWSETTING
    i4Hstart -= 65;
    #endif
    if(0 > i4Hstart)
    {
        i4Hstart = 0x0;
    }
    if(0 > i4Vstart)
    {
        i4Vstart = 0x0;
    }
    _OSD_BASE_GetOSD1Switch(&u4OSD1Order);
    _OSD_BASE_GetOSD2Switch(&u4OSD2Order);
    _OSD_BASE_GetOSD3Switch(&u4OSD3Order);
    #if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    OSD_BASE_AdjustPlanePosition(u4OSDTopLayer,u4OSDMiddLayer,u4OSDBotLayer,i4Hstart,2);
    #elif defined(CC_MT5881)
    u4PmxOption = IO_READ32(0xf0027800,0x4) & 0xC ;
    if(u4PmxOption != 4)
    {
        OSD_BASE_AdjustPlanePosition(u4OSDTopLayer,u4OSDMiddLayer,u4OSDBotLayer,i4Hstart,1);
    }
    else
    {
        OSD_BASE_AdjustPlanePosition(u4OSDTopLayer,u4OSDMiddLayer,u4OSDBotLayer,i4Hstart,11);
    }
    #else
    _OSD_BASE_SetScrnHStartOsd1(i4Hstart);
    _OSD_BASE_SetScrnHStartOsd2(i4Hstart);
    _OSD_BASE_SetScrnHStartOsd3(i4Hstart);
    #endif
    _OSD_BASE_SetScrnHStartCSR(i4Hstart-0x10);
    _OSD_BASE_SetScrnVStartTop(i4Vstart);

    OSD_WRITE32(OSD_BASE_MONITOR,OSD_READ32(OSD_BASE_MONITOR)|UPDATE_TIMING_BIT);
    LOG(5,"OSD_Base_SetHStart[h=0x%x,v=0x%x]\n",i4Hstart,i4Vstart);
    OSD_WRITE32(OSD_BASE_MONITOR,OSD_READ32(OSD_BASE_MONITOR)&(~UPDATE_TIMING_BIT));

    VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());
#endif
    return (INT32) OSD_RET_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

INT32 OSD_BASE_SetYuvOutput(BOOL fgYuvOutput)
{
    _fgYuvOutput = fgYuvOutput;
    IGNORE_RET(_OSD_BASE_SetYuvOut(fgYuvOutput));
    VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    return (INT32) OSD_RET_OK;
}

#ifndef CC_MTK_LOADER
#if defined(CC_SUPPORT_STR)||defined(CC_LG_SNAP_SHOT)

static UINT32* pm_save_fmt_regs;
static UINT32* pm_save_osd1_regs;
static UINT32* pm_save_osd2_regs;
static UINT32* pm_save_osd3_regs;
static UINT32* pm_save_osd2sc_regs;
static UINT32* pm_save_osd3sc_regs;
static UINT32* pm_save_csr_regs;
static UINT32 pm_save_pmx_regs[2];

#define OSD_FMT_REG_SIZE (4+(0xb8-0))
#if defined(OSD_IC_GROUP_TWO)
#define OSD1_REG_SIZE (4+(0x978-0x900))
#else
#define OSD1_REG_SIZE (4+(0x178-0x100))
#endif
#define PMX_RRG_BASE 0xf0027984
#define OSD2_REG_SIZE (4+(0x284-0x200))
#define OSD3_REG_SIZE (4+(0x384-0x300))
#define OSD2SC_REG_SIZE (4+(0x4d8-0x400))
#define OSD3SC_REG_SIZE (4+(0x5d8-0x500))
#define OSD_CSR_REG_SIZE (4+(0x73c-0x700))

#define OSTAG_OSDRESET_REG   (IO_ADDR(0xd1c4))
#define OSTAG_OSDRESET_BIT   (1 << 2)
#define OSTAG_OSDCLOCK_REG   (IO_ADDR(0xd1cc))
#define OSTAG_OSDCLOCK_BIT   (1 << 2)

#define SCALER_SETDATA_REG   (IO_ADDR(0x25304))
#define SCALER_SETDATA_BIT   (1 << 30)

#define IS_NULLPTR(ptr)      (NULL == (ptr))
#define FREE_IS_VALID(ptr)    if(ptr) x_mem_free((ptr))

BOOL pb_osd_enable[OSD_PLANE_MAX_NUM] = {FALSE};

void OSD_pm_suspend(void)
{
    BOOL u4Plane1Enable, u4Plane2Enable, u4Plane3Enable;
    if(b_suspend)
    {
         return;
    }

    if(pb_osd_enable[OSD_PLANE_1] == FALSE)
    {
        OSD_PLA_GetEnable(OSD_PLANE_1, &u4Plane1Enable);
    }
    else
    {
        u4Plane1Enable = pb_osd_enable[OSD_PLANE_1];
    }

    if(pb_osd_enable[OSD_PLANE_2] == FALSE)
    {
        OSD_PLA_GetEnable(OSD_PLANE_2, &u4Plane2Enable);
    }
    else
    {
        u4Plane2Enable = pb_osd_enable[OSD_PLANE_2];
    }

    if(pb_osd_enable[OSD_PLANE_3] == FALSE)
    {
        OSD_PLA_GetEnable(OSD_PLANE_3, &u4Plane3Enable);
    }
    else
    {
        u4Plane3Enable = pb_osd_enable[OSD_PLANE_3];
    }

    OSD_PLA_Enable(OSD_PLANE_1,FALSE);
    OSD_PLA_Enable(OSD_PLANE_2,FALSE);
    OSD_PLA_Enable(OSD_PLANE_3,FALSE);
    
    pb_osd_enable[OSD_PLANE_1] = u4Plane1Enable;
    pb_osd_enable[OSD_PLANE_2] = u4Plane2Enable;
    pb_osd_enable[OSD_PLANE_3] = u4Plane3Enable;

    OSD_EXP_HandleResumeSuspend(TRUE);
    pm_save_fmt_regs = x_mem_alloc(OSD_FMT_REG_SIZE);
    pm_save_osd1_regs = x_mem_alloc(OSD1_REG_SIZE);
    pm_save_osd2_regs = x_mem_alloc(OSD2_REG_SIZE);
    pm_save_osd3_regs = x_mem_alloc(OSD3_REG_SIZE);
    pm_save_osd2sc_regs = x_mem_alloc(OSD2SC_REG_SIZE);
    pm_save_osd3sc_regs = x_mem_alloc(OSD3SC_REG_SIZE);
    pm_save_csr_regs = x_mem_alloc(OSD_CSR_REG_SIZE);

    if(IS_NULLPTR(pm_save_fmt_regs) || IS_NULLPTR(pm_save_osd1_regs) || IS_NULLPTR(pm_save_osd2_regs)
        || IS_NULLPTR(pm_save_osd3_regs) || IS_NULLPTR(pm_save_osd2sc_regs) || IS_NULLPTR(pm_save_osd3sc_regs)
        || IS_NULLPTR(pm_save_csr_regs))
    {
            FREE_IS_VALID(pm_save_fmt_regs);
         FREE_IS_VALID(pm_save_osd1_regs);
         FREE_IS_VALID(pm_save_osd2_regs);
         FREE_IS_VALID(pm_save_osd3_regs);
         FREE_IS_VALID(pm_save_osd2sc_regs);
         FREE_IS_VALID(pm_save_osd3sc_regs);
         FREE_IS_VALID(pm_save_csr_regs);

         if (_u4FlipCnt > 0)
         {
             x_sema_unlock(_hOsdWaitVsyncSema);
         }
         
         if (_u4FlipCnt2 > 0)
         {
             x_sema_unlock(_hOsdWaitVsyncSema2);
         }

         return;
  }
  else
  {
        b_suspend = TRUE;

        x_memcpy((void*)pm_save_fmt_regs,(void*)(OSD_BASE+0x0),OSD_FMT_REG_SIZE);

#if defined(OSD_IC_GROUP_TWO)
        x_memcpy((void*)pm_save_osd1_regs,(void*)(OSD_BASE+0x900),OSD1_REG_SIZE);
#else
        x_memcpy((void*)pm_save_osd1_regs,(void*)(OSD_BASE+0x100),OSD1_REG_SIZE);
#endif
        x_memcpy((void*)pm_save_osd2_regs,(void*)(OSD_BASE+0x200),OSD2_REG_SIZE);
        x_memcpy((void*)pm_save_osd3_regs,(void*)(OSD_BASE+0x300),OSD3_REG_SIZE);
        x_memcpy((void*)pm_save_osd2sc_regs,(void*)(OSD_BASE+0x500),OSD2SC_REG_SIZE);
        x_memcpy((void*)pm_save_osd3sc_regs,(void*)(OSD_BASE+0x600),OSD3SC_REG_SIZE);
        x_memcpy((void*)pm_save_csr_regs,(void*)(OSD_BASE+0x700),OSD_CSR_REG_SIZE);
  }

    OSD_BASE_SetClock(OSD_CK_XTAL);

    IO_WRITE32MSK(IO_ADDR(0xd1c4), 0, 0x0, 0x00000002);
    IO_WRITE32MSK(IO_ADDR(0xd1cc), 0, 0x0, 0x00000002);
    pm_save_pmx_regs[0]=IO_READ32(PMX_RRG_BASE,0x0);
    pm_save_pmx_regs[1]=IO_READ32(PMX_RRG_BASE,0x4);

    if (_u4FlipCnt > 0)
    {
        x_sema_unlock(_hOsdWaitVsyncSema);
    }

    if (_u4FlipCnt2 > 0)
    {
        x_sema_unlock(_hOsdWaitVsyncSema2);
    }

}


void OSD_pm_resume(void)
{
    bVyncCouter = 3;  // resume for OSDIsPlaying()

    if(b_suspend == FALSE)
    {
         return;
    }

    OSD_EXP_HandleResumeSuspend(FALSE);
    b_suspend = FALSE;

    //update clock
    IO_WRITE32MSK(IO_ADDR(0xd1cc), 0, 0x2, 0x00000002);
    IO_WRITE32MSK(IO_ADDR(0xd1c4), 0, 0x2, 0x00000002);

    _u4Clock = OSD_CK_XTAL;
    OSD_BASE_SetClock(OSD_CK_OCLK); //enable OSD clock

    x_memcpy((void*)(OSD_BASE+0x0+8),(void*)(((UINT32)pm_save_fmt_regs) + 8),OSD_FMT_REG_SIZE-8);
#if defined(OSD_IC_GROUP_TWO)
    x_memcpy((void*)(OSD_BASE+0x900),(void*)(pm_save_osd1_regs),OSD1_REG_SIZE);
#else
    x_memcpy((void*)(OSD_BASE+0x100),(void*)(pm_save_osd1_regs),OSD1_REG_SIZE);
#endif
    x_memcpy((void*)(OSD_BASE+0x200),(void*)(pm_save_osd2_regs),OSD2_REG_SIZE);
    x_memcpy((void*)(OSD_BASE+0x300),(void*)(pm_save_osd3_regs),OSD3_REG_SIZE);
    x_memcpy((void*)(OSD_BASE+0x500),(void*)(pm_save_osd2sc_regs),OSD2SC_REG_SIZE);
    x_memcpy((void*)(OSD_BASE+0x600),(void*)(pm_save_osd3sc_regs),OSD3SC_REG_SIZE);
    x_memcpy((void*)(OSD_BASE+0x700),(void*)(pm_save_csr_regs),OSD_CSR_REG_SIZE);
#if defined(OSD_IC_GROUP_TWO)
    IO_WRITE32(OSD_BASE, 0x900, pm_save_osd1_regs[0]);
#else
    IO_WRITE32(OSD_BASE, 0x100, pm_save_osd1_regs[0]);
#endif
    IO_WRITE32(OSD_BASE, 0x200, pm_save_osd2_regs[0]);
    IO_WRITE32(OSD_BASE, 0x300, pm_save_osd3_regs[0]);

    IO_WRITE32MSK(IO_ADDR(0x25304), 0, 0x40000000, 0x40000000);//set data to update osd timing

#ifndef CC_MTK_FPGA
    HAL_Delay_us(50000);
#else
    x_thread_delay(50);
#endif

    IO_WRITE32(OSD_BASE, 0, pm_save_fmt_regs[0]|1); //update shadow
    IO_WRITE32(PMX_RRG_BASE,0x0,pm_save_pmx_regs[0]);
    IO_WRITE32(PMX_RRG_BASE,0x4,pm_save_pmx_regs[1]);

    x_mem_free(pm_save_fmt_regs);
    x_mem_free(pm_save_osd1_regs);
    x_mem_free(pm_save_osd2_regs);
    x_mem_free(pm_save_osd3_regs);
    x_mem_free(pm_save_osd2sc_regs);
    x_mem_free(pm_save_osd3sc_regs);
    x_mem_free(pm_save_csr_regs);

    #ifdef CC_FASTBOOT_DRIVER_LOGO
    IO_WRITE32(OSD_PLA2_REG,0x0,IO_READ32(OSD_PLA2_REG,0x0)&0xFFFFFFFE);
    IO_WRITE32(OSD_PLA3_REG,0x0,IO_READ32(OSD_PLA3_REG,0x0)&0xFFFFFFFE);
    IO_WRITE32(OSD_BASE,0x0,0x1);
    bFastBootDriverLogo = TRUE;
    #else
    //OSD_PLA_Enable(OSD_PLANE_2, pb_osd_enable[0]);
    //OSD_PLA_Enable(OSD_PLANE_3, pb_osd_enable[1]);
    #endif
    IO_WRITE32(FSC_OSD_VSYNC_SEL, 0, IO_READ32(FSC_OSD_VSYNC_SEL, 0) | 0x400U);
    IO_WRITE32(FSC_OSD_POST_SCALER_SET, 0, IO_READ32(FSC_OSD_POST_SCALER_SET, 0) & (~0x800U));
    IO_WRITE32(FSC_OSD_POST_SCALER_SET, 0, IO_READ32(FSC_OSD_POST_SCALER_SET, 0) | 0x400U);
    IO_WRITE32(OSD_POST_SCALER_SET_H_REVERSE, 0, IO_READ32(OSD_POST_SCALER_SET_H_REVERSE, 0) | 0x3000000U);
    #ifdef OSD_SCALER_DOWN_SUPPORT
    IO_WRITE32(OSD_SC_CLOCK_BASE, 0x368, IO_READ32(OSD_SC_CLOCK_BASE, 0x368) | 0x80000000U);
#ifdef CC_MT5890
    if(IS_IC_5861())
    {
        IO_WRITE32(OSD_SC_CLOCK_BASE, 0x40c, IO_READ32(OSD_SC_CLOCK_BASE, 0x40c) & 0xFFFFFFFFU);
        IO_WRITE32(OSD_SC_CLOCK_BASE, 0x40c, IO_READ32(OSD_SC_CLOCK_BASE, 0x40c) | 0x0000000FU);
    }
    else
#endif
    {
        IO_WRITE32(OSD_SC_CLOCK_BASE, 0x40c, IO_READ32(OSD_SC_CLOCK_BASE, 0x40c) & 0xFFFFFFF6U);
        IO_WRITE32(OSD_SC_CLOCK_BASE, 0x40c, IO_READ32(OSD_SC_CLOCK_BASE, 0x40c) | 0x00000006U);
    }
    #endif
    #if defined(CC_MT5881)
    #ifdef SMALL_OUTWINDOW_TTD_NEWSETTING
    vIO32WriteFldAlign(TTDPO_00  , 0x80 ,   SYNC_DELAY_CYCLES);
    vIO32WriteFldAlign(TTDIR_02  , 0x5F ,   IR_FIFO_LENGTH);
    #endif
    #endif

#ifdef CC_FLIP_MIRROR_SUPPORT
    _OsdGetMirrorFlipSetting();
    _OSD_RGN_AllFlipMirrorMode();
 #endif
}

#endif /*CC_SUPPORT_STR*/
#endif

extern void vDrvFireImportPortection(void);
//-----------------------------------------------------------------------------
/** osd global initialization function
 *  @param  void
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_Init(void)
{
    if (_fgOsdInit == 0)
    {
    #ifdef CC_MT5890
        vDrvSwitchOSDRoute();
    #endif
        // OSD_Base_Reg_Isr();
        OSD_InitCpblty();
#ifndef CC_MTK_LOADER
        // Create Logo Semphore

        if (x_sema_create(&_hOsdLogoSema, X_SEMA_TYPE_MUTEX,
                X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            ASSERT(0);
        }
#endif

        IGNORE_RET(OSD_Reset());

#ifndef CC_MTK_LOADER
        // Create OSD Flip Semphore
        if (x_sema_create(&_hOsdWaitVsyncSema, X_SEMA_TYPE_BINARY,
                X_SEMA_STATE_LOCK) != OSR_OK)
        {
            ASSERT(0);
        }

    if (x_sema_create(&_hOsdWaitVsyncSema2, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK) != OSR_OK)
    {
        ASSERT(0);
    }

        if (x_sema_create(&_hOsdFlipToSema, X_SEMA_TYPE_BINARY,
                X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            ASSERT(0);
        }

        #ifdef OSD_4K2K_SUPPORT
        if (_hOsdVsyncDetectTimer == NULL_HANDLE)
        {
            LOG(5,"%s:do create timer.\n",__FUNCTION__);
            VERIFY(x_timer_create(&_hOsdVsyncDetectTimer) == OSR_OK);
        }
        #endif

#endif

#ifndef CC_FLIP_MIRROR_FROM_DDI
        _OsdGetMirrorFlipSetting();
#endif
        #ifdef CC_MTK_LOADER
        #ifdef CC_FLIP_MIRROR_SUPPORT
        _OsdGetMirrorFlipSetting();
        #endif
        #endif
        _fgOsdInit = 1;

        OSD_Base_SetHStart();
#ifdef OSD_BASE_UPDATE_VIDEO_DATA
#if !defined(CC_MT5398) && !defined(CC_MT5880)
        vDrvFireImportPortection();
#endif
#endif


    }

#ifdef OSD_BASE_INIT_FRC_HEADER
#if LG_OSD_PATH_TEST
    OSD_BASE_InitFRCHeader();
#endif
#endif
#ifndef CC_MTK_LOADER
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)// register irq
    if(!bIsCreateISR)
    {
        VERIFY(x_reg_isr(VECTOR_OSD, vOsdHeaderUpdateCallbak, &pfnOldIsr) == OSR_OK);
        bIsCreateISR = TRUE;
    }
#endif
#endif

    return (INT32) OSD_RET_OK;
}

INT32 OSD_Reset(void)
{
    // to choose osd clock as oclk
    IGNORE_RET(OSD_BASE_SetClock(OSD_CK_OCLK));

    // reset the whole OSD
    IGNORE_RET(_OSD_BASE_SetReg(NULL));

    IGNORE_RET(_OsdBaseInit());
    IGNORE_RET(_OSD_BASE_SetYuvOut(_fgYuvOutput));
    IGNORE_RET(OSD_UpdateTiming());

    //IGNORE_RET(_OSD_BASE_SetHsEdge(OSD_BASE_HS_EDGE));
    //IGNORE_RET(_OSD_BASE_SetVsEdge(OSD_BASE_VS_EDGE));
    IGNORE_RET(_OSD_3D_SetReg(NULL));
    //For mustang eco update timing by harward
    #if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    if(IS_IC_5399_ES2())
    {
        OSD_WRITE32(OSD_BASE_MONITOR,OSD_READ32(OSD_BASE_MONITOR)|DISABLE_SW_UPDATE);
    }
    #endif
    //Select video path
    IO_WRITE32(FSC_OSD_VSYNC_SEL, 0, IO_READ32(FSC_OSD_VSYNC_SEL, 0) | 0x400U);
    IO_WRITE32(FSC_OSD_POST_SCALER_SET, 0, IO_READ32(FSC_OSD_POST_SCALER_SET, 0) & (~0x800U));
    #if defined(CC_MT5881)
    #ifdef SMALL_OUTWINDOW_TTD_NEWSETTING
    vIO32WriteFldAlign(TTDPO_00  , 0x80 ,   SYNC_DELAY_CYCLES);
    vIO32WriteFldAlign(TTDIR_02  , 0x5F ,   IR_FIFO_LENGTH);
    #endif
    #endif
    //set scaler down clock,if don't set,scaler down won't be effective
    #ifdef OSD_SCALER_DOWN_SUPPORT
    IO_WRITE32(OSD_SC_CLOCK_BASE, 0x368, IO_READ32(OSD_SC_CLOCK_BASE, 0x368) | 0x80000000U);
#ifdef CC_MT5890
    if(IS_IC_5861())
    {
        IO_WRITE32(OSD_SC_CLOCK_BASE, 0x40c, IO_READ32(OSD_SC_CLOCK_BASE, 0x40c) & 0xFFFFFFFFU);
        IO_WRITE32(OSD_SC_CLOCK_BASE, 0x40c, IO_READ32(OSD_SC_CLOCK_BASE, 0x40c) | 0x0000000FU);
    }
    else
#endif
    {
        IO_WRITE32(OSD_SC_CLOCK_BASE, 0x40c, IO_READ32(OSD_SC_CLOCK_BASE, 0x40c) & 0xFFFFFFF6U);
        IO_WRITE32(OSD_SC_CLOCK_BASE, 0x40c, IO_READ32(OSD_SC_CLOCK_BASE, 0x40c) | 0x00000006U);
    }
    #endif
    // adjust scaler to fit new resolution and also reset LPF
    // defaule enable scaler for better bandwidth utilization
    IGNORE_RET(OSD_SC_Scale((UINT32)OSD_SCALER_1, TRUE, 0, 0, 0, 0));
    IGNORE_RET(OSD_SC_Scale((UINT32)OSD_SCALER_2, TRUE, 0, 0, 0, 0));
    IGNORE_RET(OSD_SC_Scale((UINT32)OSD_SCALER_3, TRUE, 0, 0, 0, 0));
    _OSD_SC_SetAlphaCountEn(OSD_SCALER_2, TRUE);
    IGNORE_RET(_OSD_SC_UpdateHwReg(OSD_SCALER_2));
    _OSD_SC_SetAlphaCountEn(OSD_SCALER_3, TRUE);
    IGNORE_RET(_OSD_SC_UpdateHwReg(OSD_SCALER_3));

    _OSD_RGN_InitApi();

    IGNORE_RET(OSD_PLA_Init((UINT32)OSD_PLANE_1));
    IGNORE_RET(OSD_PLA_Init((UINT32)OSD_PLANE_2));
    IGNORE_RET(OSD_PLA_Init((UINT32)OSD_PLANE_3));
#ifdef OSD_PLA5_SUPPORT
    IGNORE_RET(OSD_PLA_Init((UINT32)OSD_PLANE_5));
#endif
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetEnHdrIrq(OSD_PLANE_1,1));
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_1));
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetEnHdrIrq(OSD_PLANE_2,1));
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_2));
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetEnHdrIrq(OSD_PLANE_3,1));
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_3));

#endif
    IGNORE_RET(OSD_RGN_LIST_Init());
    IGNORE_RET(OSD_RGN_Init());
    IGNORE_RET(OSD_RGN_LIST_SCALER_Init());
    OSD_PLA_Info_Init();

    IO_WRITE32(
            OSD_INT_ENABLE_REG,
            0,
            OSD1_CFUD_IEN | OSD2_CFUD_IEN | OSD2_PFUD_IEN | OSD2_VUTO_IEN
                    | OSD2_VDTO_IEN | OSD3_CFUD_IEN | OSD3_PFUD_IEN
                    | OSD3_VUTO_IEN | OSD3_VDTO_IEN);

#ifdef DRV_SUPPORT_EXTMJC
    OSD_DMA_Init();
#endif
    // update 3x3 matrix for YUV-to-RGB
    IO_WRITE32(OSD_BASE, 0x260, 0x01000000);
    IO_WRITE32(OSD_BASE, 0x264, 0x0164034e);
    IO_WRITE32(OSD_BASE, 0x268, 0x01001058);
    IO_WRITE32(OSD_BASE, 0x26c, 0x10b40086);
    IO_WRITE32(OSD_BASE, 0x270, 0x010001c4);
    IO_WRITE32(OSD_BASE, 0x274, 0x0000031e);

    IO_WRITE32(OSD_BASE, 0x360, 0x01000000);
    IO_WRITE32(OSD_BASE, 0x364, 0x0164034e);
    IO_WRITE32(OSD_BASE, 0x368, 0x01001058);
    IO_WRITE32(OSD_BASE, 0x36c, 0x10b40086);
    IO_WRITE32(OSD_BASE, 0x370, 0x010001c4);
    IO_WRITE32(OSD_BASE, 0x374, 0x0000031e);

#ifdef OSD_RESET_BASE_ZERO
    IO_WRITE32(OSD_BASE, 0, 0);
#endif
#ifndef CC_MTK_LOADER

    if(IS_DISP_4K2K_TYPE3 || OSD_EXP_IS_GFX())
    {
        OSD_EXP_BypPQPath();
    }
#endif

#ifdef SMALL_OUTWINDOW_TTD_NEWSETTING    
     // when scaler input <= 192, the video can not dispaly,So as DE FC's suggestion
    vIO32WriteFldAlign(TTDPO_01  , 0x80 ,   SYNC_DELAY_CYCLES); 
    vIO32WriteFldAlign(TTDIR_02  , 0x55 ,   IR_FIFO_LENGTH); 
#endif

#if defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
    OSD_BASE_SetPlaneSwitchOrder(u4OSDTopLayer, u4OSDMiddLayer, u4OSDBotLayer);
#endif
    return (INT32) OSD_RET_OK;
}
INT32 OSD_SwInit(void)
{
    if (_fgOsdInit == 0)
    {
	
        OSD_InitCpblty();
#ifndef CC_MTK_LOADER
        // Create Logo Semphore

        if (x_sema_create(&_hOsdLogoSema, X_SEMA_TYPE_MUTEX,
                X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            ASSERT(0);
        }
#endif
        IGNORE_RET(OSD_SwReset());

#ifndef CC_MTK_LOADER
        // Create OSD Flip Semphore
        if (x_sema_create(&_hOsdWaitVsyncSema, X_SEMA_TYPE_BINARY,
                X_SEMA_STATE_LOCK) != OSR_OK)
        {
            ASSERT(0);
        }

       if (x_sema_create(&_hOsdWaitVsyncSema2, X_SEMA_TYPE_BINARY,
               X_SEMA_STATE_LOCK) != OSR_OK)
       {
           ASSERT(0);
       }

        if (x_sema_create(&_hOsdFlipToSema, X_SEMA_TYPE_BINARY,
                X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            ASSERT(0);
        }
#endif

#ifndef CC_FLIP_MIRROR_FROM_DDI
        _OsdGetMirrorFlipSetting();
#endif
        _fgOsdInit = 1;
#ifdef CC_MT5890
		LOG(3,"OSDRoute init\n");
		vDrvSwitchOSDRoute();
#endif
        OSD_Base_SetHStart();
    }
    return (INT32) OSD_RET_OK;
}

INT32 OSD_SwReset(void)
{
    UINT32 u4Base8Reg = IO_READ32(OSD_BASE_REG, 0x8);
    if(u4Base8Reg & 0x200000)
    {
        return OSD_Reset();
    }
    // to choose osd clock as oclk
    IGNORE_RET(OSD_BASE_SetClock(OSD_CK_OCLK));

    // reset the whole OSD
    //IGNORE_RET(_OSD_BASE_SetReg(NULL));
    _OSD_BASE_SwSetReg();

    //IGNORE_RET(_OsdBaseInit());
    IGNORE_RET(_OSD_BASE_SetYuvOut(_fgYuvOutput));
    IGNORE_RET(OSD_UpdateTiming());

    //IGNORE_RET(_OSD_BASE_SetHsEdge(OSD_BASE_HS_EDGE));
    //IGNORE_RET(_OSD_BASE_SetVsEdge(OSD_BASE_VS_EDGE));
    IGNORE_RET(_OSD_3D_SetReg(NULL));

    // adjust scaler to fit new resolution and also reset LPF
    // defaule enable scaler for better bandwidth utilization

    _OSD_RGN_InitApi();
    IGNORE_RET(OSD_PLA_SwReset((UINT32)OSD_PLANE_1));

    IGNORE_RET(OSD_PLA_SwReset((UINT32)OSD_PLANE_2));

    IGNORE_RET(OSD_PLA_SwReset((UINT32)OSD_PLANE_3));

    IGNORE_RET(OSD_RGN_LIST_Init());
    IGNORE_RET(OSD_RGN_Init());
    IGNORE_RET(OSD_RGN_LIST_SCALER_Init());
    OSD_PLA_Info_Init();

    IO_WRITE32(
            OSD_INT_ENABLE_REG,
            0,
            OSD1_CFUD_IEN | OSD2_CFUD_IEN | OSD2_PFUD_IEN | OSD2_VUTO_IEN
                    | OSD2_VDTO_IEN | OSD3_CFUD_IEN | OSD3_PFUD_IEN
                    | OSD3_VUTO_IEN | OSD3_VDTO_IEN);

#ifdef DRV_SUPPORT_EXTMJC
    OSD_DMA_Init();
#endif
    // update 3x3 matrix for YUV-to-RGB
    IO_WRITE32(OSD_BASE, 0x260, 0x01000000);
    IO_WRITE32(OSD_BASE, 0x264, 0x0164034e);
    IO_WRITE32(OSD_BASE, 0x268, 0x01001058);
    IO_WRITE32(OSD_BASE, 0x26c, 0x10b40086);
    IO_WRITE32(OSD_BASE, 0x270, 0x010001c4);
    IO_WRITE32(OSD_BASE, 0x274, 0x0000031e);

    IO_WRITE32(OSD_BASE, 0x360, 0x01000000);
    IO_WRITE32(OSD_BASE, 0x364, 0x0164034e);
    IO_WRITE32(OSD_BASE, 0x368, 0x01001058);
    IO_WRITE32(OSD_BASE, 0x36c, 0x10b40086);
    IO_WRITE32(OSD_BASE, 0x370, 0x010001c4);
    IO_WRITE32(OSD_BASE, 0x374, 0x0000031e);

#ifndef CC_MTK_LOADER
		if(IS_DISP_4K2K_TYPE3 || OSD_EXP_IS_GFX())
		{
			OSD_EXP_BypPQPath();
		}
#endif

#ifdef SMALL_OUTWINDOW_TTD_NEWSETTING    
     // when scaler input <= 192, the video can not dispaly,So as DE FC's suggestion
    vIO32WriteFldAlign(TTDPO_01  , 0x80 ,   SYNC_DELAY_CYCLES); 
    vIO32WriteFldAlign(TTDIR_02  , 0x55 ,   IR_FIFO_LENGTH); 
#endif

//#if defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
//	OSD_BASE_SetPlaneSwitchOrder(u4OSDTopLayer, u4OSDMiddLayer, u4OSDBotLayer);
//#endif
    return (INT32) OSD_RET_OK;
}

INT32 OSD_UpdateTiming(void)
{
#ifdef CC_MT5391_FPGA
    IGNORE_RET(_OSD_BASE_SetScrnHSize(720));
    IGNORE_RET(_OSD_BASE_SetScrnVSize(480));
    // osd2 hstart=0xb, osd vstart=0x5
    IO_WRITE32MSK(OSD_BASE, 0x2c, 0x350b0500, 0xffffff00);
    // osd3 hstart=0x1b
    IO_WRITE32MSK(OSD_BASE, 0x38, 0x091d, 0xffff);
#else
    IGNORE_RET(_OSD_BASE_SetScrnHSize(OSD_BASE_GetDisplayWidth()));
    IGNORE_RET(_OSD_BASE_SetScrnVSize(OSD_BASE_GetDisplayHeight()));
    IO_WRITE32MSK(OSD_BASE, 0x2c, 0x1A0510, 0x00ffff00);
    IO_WRITE32MSK(OSD_BASE, 0x38, 0x1A, 0xffff);
#endif

    // write hardware
    VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** set whether osd1/osd2 is top plane
 *  @param  u4TopPlane OSD_PLANE_1/OSD_PLANE_2/OSD_PLANE_NONE(video plane is on top)
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_BASE_SetTopPlane(UINT32 u4TopPlane)
{
    // this function should be call in vsync, and update HW immediately

    if (u4TopPlane == (UINT32) OSD_PLANE_2)
    {
        _OSD_BASE_SetOSD12Ex(1);
    }
    else
    {
        _OSD_BASE_SetOSD12Ex(0);
    }
    VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());

    return (INT32) OSD_RET_OK;

}
INT32 OSD_BASE_AdjustPlanePosition(UINT32 u4TopPlane, UINT32 u4MidPlane,
        UINT32 u4BotPlane,INT32 i4Position,UINT32 delay)
{

    UINT32 u4OSD1Postion = 0;
    UINT32 u4OSD2Postion = 0;
    UINT32 u4OSD3Postion = 0;
    INT32 i4OSDPosition = 0;
    i4OSDPosition = i4Position - (INT32)delay;
    switch(u4TopPlane)
    {
        case OSD_PLANE_2:
            u4OSD2Postion = i4Position + delay;
            break;
        case OSD_PLANE_3:
            u4OSD3Postion = i4Position + delay;
            break;
        case OSD_PLANE_1:
            u4OSD1Postion = i4Position + delay;
            break;
        default:
            u4OSD1Postion = i4Position + delay;
            break;
    }
    switch(u4MidPlane)
    {
        case OSD_PLANE_2:
            u4OSD2Postion = i4Position;
            break;
        case OSD_PLANE_3:
            u4OSD3Postion = i4Position;
            break;
        case OSD_PLANE_1:
            u4OSD1Postion = i4Position;
            break;
        default:
            u4OSD3Postion = i4Position;
            break;
    }
    switch(u4BotPlane)
    {
        case OSD_PLANE_2:
            u4OSD2Postion = (i4OSDPosition < 0) ? i4Position : i4OSDPosition;
            break;
        case OSD_PLANE_3:
            u4OSD3Postion = (i4OSDPosition < 0) ? i4Position : i4OSDPosition;
            break;
        case OSD_PLANE_1:
            u4OSD1Postion = (i4OSDPosition < 0) ? i4Position : i4OSDPosition;
            break;
        default:
            u4OSD2Postion = (i4OSDPosition < 0) ? i4Position : i4OSDPosition;
            break;
    }
    _OSD_BASE_SetScrnHStartOsd1(u4OSD1Postion);
    _OSD_BASE_SetScrnHStartOsd2(u4OSD2Postion);
    _OSD_BASE_SetScrnHStartOsd3(u4OSD3Postion);

    return (INT32) OSD_RET_OK;
}

INT32 OSD_BASE_SetPlaneSwitchOrder(UINT32 u4TopPlane, UINT32 u4MidPlane,
        UINT32 u4BotPlane)
{
    // default define: plan1 top , plane3  Mid; plane2 Bot,  need update when IC back
    UINT32 u4Osd1Switch = 0x0;
    UINT32 u4Osd2Switch = 0x0;
    UINT32 u4Osd3Switch = 0x0;
    #if defined(CC_MT5881)
    UINT32 u4PmxOption ;
    #endif
    #if defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
    u4OSDTopLayer = u4TopPlane;
    u4OSDMiddLayer = u4MidPlane;
    u4OSDBotLayer = u4BotPlane;
    #endif
    if (u4TopPlane == OSD_PLANE_2)
    {
        if (u4MidPlane == OSD_PLANE_1)
        {
            u4Osd1Switch = 0x2;
            u4Osd2Switch = 0x1;
            u4Osd3Switch = 0x3;
        }
        else
        {
            u4Osd1Switch = 0x2;
            u4Osd2Switch = 0x3;
            u4Osd3Switch = 0x1;
        }
    }
    else if (u4TopPlane == OSD_PLANE_1)
    {
        if (u4MidPlane == OSD_PLANE_2)
        {
            u4Osd1Switch = 0x1;
            u4Osd2Switch = 0x2;
            u4Osd3Switch = 0x3;
        }
        else
        {
            u4Osd1Switch = 0x1;
            u4Osd2Switch = 0x3;
            u4Osd3Switch = 0x2;
        }
    }
    else if (u4TopPlane == OSD_PLANE_3)
    {
        if (u4MidPlane == OSD_PLANE_1)
        {
            u4Osd1Switch = 0x3;
            u4Osd2Switch = 0x1;
            u4Osd3Switch = 0x2;
        }
        else
        {
            u4Osd1Switch = 0x3;
            u4Osd2Switch = 0x2;
            u4Osd3Switch = 0x1;
        }
    }

    _OSD_BASE_SetOSD1Switch(u4Osd1Switch);
    _OSD_BASE_SetOSD3Switch(u4Osd2Switch);
    _OSD_BASE_SetOSD2Switch(u4Osd3Switch);

    LOG(5,"\nOSD_SetPlaneSwitchOrder[%d,%d,%d],Switch[%d,%d,%d]\n",
        u4TopPlane,u4MidPlane,u4BotPlane,u4Osd1Switch,u4Osd2Switch,u4Osd3Switch);
    #if defined(CC_MT5881)
    u4PmxOption = IO_READ32(0xf0027800,0x4) & 0xC ;
    if(u4PmxOption != 4)
    {
        OSD_BASE_AdjustPlanePosition(u4OSDTopLayer,u4OSDMiddLayer,u4OSDBotLayer,i4Hstart,1);
    }
    else
    {
        OSD_BASE_AdjustPlanePosition(u4OSDTopLayer,u4OSDMiddLayer,u4OSDBotLayer,i4Hstart,11);
    }
    #endif
    #if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    OSD_BASE_AdjustPlanePosition(u4TopPlane,u4MidPlane,u4BotPlane,i4Hstart,2);
    #endif
    VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());

    return (INT32) OSD_RET_OK;
}

INT32 OSD_BASE_SetDisplayMode(UINT32 u4DisplayMode)
{
    if (BSP_IsFPGA())
    {
        OSD_VERIFY_DISPLAY_MODE(u4DisplayMode);

        if (u4DisplayMode == (UINT32) OSD_DM_480P) // 2
        {
            // 720x480
            IO_WRITE32MSK(SCPOS_BASE, 0x24, ((UINT32) 0x359 << 20) | 0x20c,
                    0xfff007ff);
            PANEL_SetPanelWidth(720);
            PANEL_SetPanelHeight(480);
        }
        else if (u4DisplayMode == (UINT32) OSD_DM_1080P) // 7
        {
            // 1920x1080
            IO_WRITE32MSK(SCPOS_BASE, 0x24, ((UINT32) 0x897 << 20) | 0x464,
                    0xfff007ff);
            PANEL_SetPanelWidth(1920);
            PANEL_SetPanelHeight(1080);
        }
    }

    UNUSED(u4DisplayMode);
    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** set osd plane position
 *  @param  u4Plane plane id
 *  @param  u4X x position
 *  @param  u4Y y position
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_BASE_SetOsdPosition(UINT32 u4Plane, UINT32 u4X, UINT32 u4Y)
{

    OSD_VERIFY_PLANE(u4Plane);

    //#ifdef CC_MT5363
#if 0
    if(gfgSwap)
    {
        if(u4Plane==OSD_PLANE_2)
        u4Plane=OSD_PLANE_3;
        else if(u4Plane==OSD_PLANE_3)
        u4Plane=OSD_PLANE_2;
    }
#endif
#if defined(CC_MT5398)
  if(/*IS_PANEL_2D_N_3D_L12R12 &&*/ IS_PANEL_L12R12)
      u4Y <<= 1;
#endif

    if (u4Plane == (UINT32) OSD_PLANE_1)
    {
        IGNORE_RET(_OSD_BASE_SetOsd1HStart(u4X));
        IGNORE_RET(_OSD_BASE_SetOsd1VStart(u4Y));
    }
    else if (u4Plane == (UINT32) OSD_PLANE_2)
    {
        IGNORE_RET(_OSD_BASE_SetOsd2HStart(u4X));
        IGNORE_RET(_OSD_BASE_SetOsd2VStart(u4Y));
        //OSD_SC_UpdateDstSize((UINT32)OSD_SCALER_2);
    }
    //#ifdef CC_MT5363
    else if (u4Plane == (UINT32) OSD_PLANE_3)
    {
        IGNORE_RET(_OSD_BASE_SetOsd3HStart(u4X));
        IGNORE_RET(_OSD_BASE_SetOsd3VStart(u4Y));
        //OSD_SC_UpdateDstSize((UINT32)OSD_SCALER_3);
    }
    //#endif

    VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());

    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** get osd plane position
 *  @param  u4Plane plane id
 *  @param  pu4X x position pointer
 *  @param  pu4Y y position pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_BASE_GetOsdPosition(UINT32 u4Plane, UINT32* pu4X, UINT32* pu4Y)
{
    OSD_VERIFY_PLANE(u4Plane);

    //#ifdef CC_MT5363
#if 0
    if(gfgSwap)
    {
        if(u4Plane==OSD_PLANE_2)
        u4Plane=OSD_PLANE_3;
        else if(u4Plane==OSD_PLANE_3)
        u4Plane=OSD_PLANE_2;
    }
#endif

    if (u4Plane == (UINT32) OSD_PLANE_1)
    {
        IGNORE_RET(_OSD_BASE_GetOsd1HStart(pu4X));
        IGNORE_RET(_OSD_BASE_GetOsd1VStart(pu4Y));
    }
    else if (u4Plane == (UINT32) OSD_PLANE_2)
    {
        IGNORE_RET(_OSD_BASE_GetOsd2HStart(pu4X));
        IGNORE_RET(_OSD_BASE_GetOsd2VStart(pu4Y));
    }
    //#ifdef CC_MT5363
    else if (u4Plane == (UINT32) OSD_PLANE_3)
    {
        IGNORE_RET(_OSD_BASE_GetOsd3HStart(pu4X));
        IGNORE_RET(_OSD_BASE_GetOsd3VStart(pu4Y));
    }
    //#endif
    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** dump osd debug info
 *  @param  void
 *  @return void
 */
//-----------------------------------------------------------------------------
void OSD_BASE_Dump(void)
{
    UINT32 u4IntStatus;
    OSD_3D_UNION_T *_pr3DReg=NULL;

    u4IntStatus = IO_READ32(OSD_INT_STATUS_REG, 0);
    _pr3DReg    = (OSD_3D_UNION_T *)_OSD_BASE_Get3DReg();

    Printf("\nOSD FMT\n");

    Printf("\tOSD INTERRUPT=0x%x ",u4IntStatus);
    if (u4IntStatus == 0)
    {
        Printf("None\n");
    }
    else
    {
        if ((u4IntStatus & OSD_INT) > 0)
        {
            Printf("(OSD_INT)");
        }
        if ((u4IntStatus & OSD1_CFUD_IID) > 0)
        {
            Printf("(OSD1_CFUD_IID)");
        }
        if ((u4IntStatus & OSD2_CFUD_IID) > 0)
        {
            Printf("(OSD2_CFUD_IID)");
        }
        if ((u4IntStatus & OSD2_PFUD_IID) > 0)
        {
            Printf("(OSD2_PFUD_IID)");
        }
        if ((u4IntStatus & OSD2_VUTO_IID) > 0)
        {
            Printf("(OSD2_VUTO_IID)");
        }
        if ((u4IntStatus & OSD2_VDTO_IID) > 0)
        {
            Printf("(OSD2_VDTO_IID)");
        }
        if ((u4IntStatus & OSD3_CFUD_IID) > 0)
        {
            Printf("(OSD3_CFUD_IID)");
        }
        if ((u4IntStatus & OSD3_PFUD_IID) > 0)
        {
            Printf("(OSD3_PFUD_IID)");
        }
        if ((u4IntStatus & OSD3_VUTO_IID) > 0)
        {
            Printf("(OSD3_VUTO_IID)");
        }
        if ((u4IntStatus & OSD3_VDTO_IID) > 0)
        {
            Printf("(OSD3_VDTO_IID)");
        }
        Printf("\n");
        IO_WRITE32(OSD_INT_STATUS_REG, 0, 0);
    }

    Printf("\tHCNT_STROBE=%d, VCNT_STROBE=%d\n",
        _OSD_BASE_GetHcntStrobe(),
        _OSD_BASE_GetVcntStrobe());
    Printf("\tHCNT_STATUS=%d, VCNT_STATUS=%d, FIFO MONITOR=0x%x\n",
        _OSD_BASE_GetHcntStatus(),
        _OSD_BASE_GetVcntStatus(),
        OSD_BASE_GetFifoMonitor());

    if(!_pr3DReg)
    {
        return;
    }

    Printf("\tOSD 3D\n");

    Printf("\t\tShutter LR\n");
    Printf("\t\t  SHUTTER_SHIFT_EN=%d, SHUTTER_LR_FLAG=%d\n",
            _pr3DReg->rField.fgShutterShiftEn,
            _pr3DReg->rField.fgShutterLRFlag);
    Printf("\t\t  TGEN_(LR_EN=%d, LLRR=%d, LR_DLY=%d, LR_INV=%d)\n",
            _pr3DReg->rField.fgTgenLREn,
            _pr3DReg->rField.fgTgenLLRREn,
            _pr3DReg->rField.fgTgenLRDly,
            _pr3DReg->rField.fgTgenLRInv);

    Printf("\t\tPR\n");
    Printf("\t\t  PLANE_SWITCH_EN=%d, PLANE_SWITCH_INV=%d\n",
        _pr3DReg->rField.fgPlaneSwitchEn,
        _pr3DReg->rField.fgPlaneSwitchInv);

    Printf("\t\tLine Shift(2D->3D)\n");
    Printf("\t\t  _LINE_SHIFT_EN ( OSD1=%d, OSD2=%d, OSD3=%d, CSR=%d )\n",
        _pr3DReg->rField.fgOsd1LShiftEn,
        _pr3DReg->rField.fgOsd2LShiftEn,
        _pr3DReg->rField.fgOsd3LShiftEn,
        _pr3DReg->rField.fgCsrLShiftEn);
    Printf("\t\t  _LINE_SHIFT_INV( OSD1=%d, OSD2=%d, OSD3=%d, CSR=%d )\n",
        _pr3DReg->rField.fgOsd1LShiftInv,
        _pr3DReg->rField.fgOsd2LShiftInv,
        _pr3DReg->rField.fgOsd3LShiftInv,
        _pr3DReg->rField.fgCsrLShiftInv);
    Printf("\t\t  _LINE_SHIFT    ( OSD1=%d, OSD2=%d, OSD3=%d, CSR=%d )\n",
        _pr3DReg->rField.fgOsd1ShiftValue,
        _pr3DReg->rField.fgOsd2ShiftValue,
        _pr3DReg->rField.fgOsd3ShiftValue,
        _pr3DReg->rField.fgCsrShiftValue);

#ifndef CC_MTK_LOADER
#ifdef LINUX_TURNKEY_SOLUTION
    Printf("DFB Buffer[addr:0x%x,size:0x%x] FB Buffer[addr:0x%x,size:0x%x]\n",fb_osd_get_dfb_addr(),fb_osd_get_dfb_size(),
        fb_osd_get_fb_addr(),fb_osd_get_fb_size());
#endif
#endif

    return;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
UINT32 OSD_BASE_GetFifoMonitor(void)
{
    UINT32 u4_fifo_monitor=0x0;

    u4_fifo_monitor = *((UINT32 *)OSD_FIFO_MONITOR_REG);

    return u4_fifo_monitor;
}


//Set 5363 SD TVE output config

INT32 OSD_SetTVEOut(BOOL fgEnable)
{
    LOG(5,"OSD_SetTVEOut: %x\n", fgEnable);
    if (fgEnable)
    {
        #if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        IO_WRITE32(OSD_BASE, 0x7c,0x680000);
        IO_WRITE32(OSD_BASE, 0x900,0x00400401);
        IO_WRITE32(OSD_BASE, 0x90c,0x006137fb);


        // OSD5 color space conversion
        IO_WRITE32(OSD_BASE,0x960,0x00420081);
        IO_WRITE32(OSD_BASE,0x964,0x00190010);
        IO_WRITE32(OSD_BASE,0x968,0x1026104a);
        IO_WRITE32(OSD_BASE,0x96c,0x00700080);
        IO_WRITE32(OSD_BASE,0x970,0x0070105e);
        IO_WRITE32(OSD_BASE,0x974,0x10120080);

        IO_WRITE32(OSD_BASE,0x18,0x00050005);
        IO_WRITE32(OSD_BASE,0xc0,0x109); // OSD,TVE timing select , external timing
        IO_WRITE32(OSD_BASE,0xc4,0x00300001); // OSD5 V_START

        IO_WRITE32(OSD_BASE,0xd8,0x3);
        IO_WRITE32(OSD_BASE,0x8,IO_READ32(OSD_BASE,0x8) | 0x40000);// OSD5 TVE Hsync/vsync output
        IO_WRITE32(OSD_BASE,0,0x9);
        // temp solution
        IO_WRITE32(IO_VIRT,0xd368, 0x8c000822);

        #else
        //IO_WRITE32(OSD_BASE, 0x8, 0x40128);
        IO_WRITE32(OSD_BASE, 0xc0, 0xa);
        IO_WRITE32(OSD_BASE, 0x10, 0x20766610);
        IO_WRITE32(OSD_BASE, 0, 8);
#if (defined(OSD_IC_GROUP_TWO)||defined(OSD_IC_GROUP_THREE))
        IO_WRITE32(OSD_BASE, 0x900,0);
#else
        IO_WRITE32(OSD_BASE, 0x100, 0);
#endif
        IO_WRITE32(OSD_BASE, 0x4, 2);
        IO_WRITE32(OSD_BASE, 0x4, 3);
        IO_WRITE32(OSD_BASE, 0, 0);
#if (defined(OSD_IC_GROUP_TWO)||defined(OSD_IC_GROUP_THREE))
        IO_WRITE32(OSD_BASE, 0x900,1);
#else
        IO_WRITE32(OSD_BASE, 0x100, 1);
#endif
        IO_WRITE32(OSD_BASE, 0, 8);
        IO_WRITE32(OSD_BASE, 0xc8, 0x03600271);
        IO_WRITE32(OSD_BASE, 0xcc, 0x03600271);
        //IO_WRITE32(OSD_BASE, 0x20, 0x001e000c);
        //IO_WRITE32(OSD_BASE, 0x20, 0x0019002b);
        IGNORE_RET(_OSD_BASE_SetOsd1HStart(0x19));
        IGNORE_RET(_OSD_BASE_SetOsd1VStart(0x2b));
        _OSD_BASE_UpdateHwReg();
        //IO_WRITE32(OSD_BASE, 0,8);
        #endif
    }
    else
    {
        //IO_WRITE32MSK(OSD_BASE, 0x8, 0, 0x40128);
        IO_WRITE32MSK(OSD_BASE, 0xc0, 0, 0xa);
#if defined(OSD_IC_GROUP_TWO)
        IO_WRITE32(OSD_BASE, 0x900,0);
#else
        IO_WRITE32(OSD_BASE, 0x100, 0);
#endif
        IO_WRITE32MSK(OSD_BASE, 0xc8, 0, 0x03600271);
        IO_WRITE32MSK(OSD_BASE, 0xcc, 0, 0x03600271);
        //IO_WRITE32MSK(OSD_BASE, 0x20, 0,0x001e000c);
        IO_WRITE32MSK(OSD_BASE, 0x20, 0, 0x0019002b);
        IO_WRITE32(OSD_BASE, 0, 8);
    }

    return (INT32) OSD_RET_OK;
}

INT32 OSD_SetPowerDwn(BOOL fgEnable)
{
    LOG(5,"OSD_SetPowerDwn: %x\n", fgEnable);
#ifdef USE_OSD_CK_REG
    if(fgEnable)
    {
        IO_WRITE32(OSD_CK_REG, 0, (_u4Clock |0x80));
    }
    else
    {
        IO_WRITE32(OSD_CK_REG, 0, _u4Clock );
    }
#else
    UNUSED(_u4Clock);
#endif

    return (INT32) OSD_RET_OK;
}
#ifndef CC_MTK_LOADER
#ifdef CC_SUPPORT_STR
#ifdef CC_FASTBOOT_DRIVER_LOGO
INT32 OSD_SetFastLogoPlane(void)
{
    if(bFastBootDriverLogo)
    {
        OSD_PLA_Enable(OSD_PLANE_2, pb_osd_enable[0]);
        OSD_PLA_Enable(OSD_PLANE_3, pb_osd_enable[1]);
        bFastBootDriverLogo = FALSE;
    }
    return (INT32) OSD_RET_OK;
}
#endif
#endif
#endif

INT32 OSD_ShowLogo(void)
{
#ifndef CC_MTK_LOADER
    static UINT32 u4RegionList;
    static UINT32 u4Region = 0;
    UINT32 u4BmpPitch, u4ColorMode, u4DataSize;

    UINT32 u4PanelWidth, u4PanelHeight;
    static UINT32 u4OutWidth, u4OutHeight,u4Width, u4Height,u4OutX, u4OutY ;
    UINT32 i;
    UINT32 u4BmpAddr;//,u4PalAddr;
    UINT32 u4LogoBuf;
    UINT32 u4OsdPlane = OSD_PLANE_3;
    UINT32 u4Scaler = OSD_SCALER_3;
    INT32 i4OriRegionList = (-1);
    UINT8 u1OriPLA_Enable;
    UINT32 u4OriSC_Enable = 0;
    UINT32 u4OriSrcW = 0;
    UINT32 u4OriSrcH = 0;
    UINT32 u4OriDstW = 0;
    UINT32 u4OriDstH = 0;
    UINT32 u4OriIs16Bpp = 0;
    INT32 ret;
    if (DRVCUST_OptGet(eDrvLogoEnable) == 0)
    {
        Printf("Driver Logo Disabled!\n");
        return (INT32) OSD_RET_OK;
    }
    //get original setting
    i4OriRegionList = OSD_PLA_GetFirstRegionList(u4OsdPlane);
    if (OSD_PLA_GetEnable(u4OsdPlane, &u1OriPLA_Enable) < 0)
    {
        Printf("OSD_PLA_GetEnable fail\n");
    }
#ifndef CC_MTK_LOADER
#ifdef CC_SUPPORT_STR
    if(u1OriPLA_Enable == FALSE)
    {
        u1OriPLA_Enable = pb_osd_enable[u4OsdPlane];
    }
    pb_osd_enable[u4OsdPlane] = FALSE;
#endif
#endif
    UNUSED(OSD_SC_GetScalerInfo(u4Scaler, &u4OriSC_Enable, &u4OriSrcW,
                    &u4OriSrcH, &u4OriDstW, &u4OriDstH, &u4OriIs16Bpp));
    UNUSED(u4OriIs16Bpp);

    if (fgLogoInit == TRUE)
    {
        ret = OSD_PLA_FlipTo(u4OsdPlane, u4RegionList);
        ret = OSD_SC_Scale(u4Scaler, 1, u4Width, u4Height, u4OutWidth,
                u4OutHeight);
        if (ret != OSD_RET_OK)
            return 5;
        OSD_BASE_SetOsdPosition(u4OsdPlane,u4OutX,u4OutY);
    }
    else
    {
        // power on panel
        //vApiPanelPowerSequence(TRUE);
        OSD_Init();
        OSD_PLA_Enable(u4OsdPlane, FALSE);

        u4PanelWidth = OSD_BASE_GetDisplayWidth();
        u4PanelHeight = OSD_BASE_GetDisplayHeight();

        LOG(5,"Panel %ld %ld \n", u4PanelWidth, u4PanelHeight);

        ret = OSD_RGN_LIST_Create(&u4RegionList);
        if (ret != OSD_RET_OK)
            return 1;

        u4BmpPitch = 0;
        u4ColorMode = DRVCUST_OptGet(eDrvLogoColorMode);
        u4Width = DRVCUST_OptGet(eDrvLogoWidth);
        u4Height = DRVCUST_OptGet(eDrvLogoHeight);
        u4BmpAddr = DRVCUST_OptGet(eDrvLogoBmpAddr);
        if(u4Width == 1366)
        {
            OSD_GET_PITCH_SIZE(u4ColorMode, u4Width+2, u4BmpPitch); // to set u4BmpPitch by u4ColorMode and u4Width.
        }
        else
        {
            OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch);
        }
        u4BmpPitch = (u4BmpPitch +0xF) & (~0xF);
        u4DataSize = u4BmpPitch * u4Height;
        if (u4DataSize == 0)
        {
            Printf("u4DataSize is 0\n");
            return FALSE;
        }
        u4LogoBuf = (UINT32) BSP_AllocAlignedDmaMemory(u4DataSize, 16);
        if (u4LogoBuf == 0)
        {
            Printf("u4LogoBuf null\n");
            return FALSE;
        }
        x_memcpy((void*) VIRTUAL(u4LogoBuf), (void*) u4BmpAddr, u4DataSize);
        HalFlushInvalidateDCacheMultipleLine((UINT32)(VIRTUAL(u4LogoBuf)),
                u4DataSize);
        u4BmpAddr = u4LogoBuf;
        ret = OSD_RGN_Create(&u4Region, u4Width, u4Height, (void *) u4BmpAddr,
                u4ColorMode, u4BmpPitch, 0, 0, u4Width, u4Height);
        if (ret != OSD_RET_OK)
            return 2;
        OSD_RGN_Set(u4Region,OSD_RGN_DEC_EN,0);
        OSD_RGN_Set(u4Region, (INT32) OSD_RGN_V_FLIP, OSD_GetCpblty(E_OSD_CPBLTY_FLIP, 0x0));
        OSD_RGN_Set(u4Region, (INT32) OSD_RGN_H_MIRROR, OSD_GetCpblty(E_OSD_CPBLTY_MIRROR,0x0));
        ret = OSD_RGN_Insert(u4Region, u4RegionList);
        if (ret != OSD_RET_OK)
            return 3;
        ret = OSD_PLA_FlipTo(u4OsdPlane, u4RegionList);
        if (ret != OSD_RET_OK)
            return 4;
        if(u4PanelWidth!=1366)
        {
            u4OutWidth = (u4Width * u4PanelWidth)/1366;
            u4OutHeight = (u4Height * u4PanelHeight)/768;
        }
        else
        {
            u4OutWidth=u4Width;
            u4OutHeight=u4Height;
        }
        u4OutX = ((u4PanelWidth > u4OutWidth))? ((u4PanelWidth - u4OutWidth)>> 1):0;
        u4OutY = ((u4PanelHeight > u4OutHeight))? ((u4PanelHeight - u4OutHeight)>> 1):0;
        LOG(5,"u4OutWidth=%d,u4OutHeight=%d,u4OutX=%d,u4OutY=%d.\n",
            u4OutWidth,u4OutHeight,u4OutX,u4OutY);
        ret = OSD_SC_Scale(u4Scaler, 1, u4Width, u4Height, u4OutWidth,
                u4OutHeight);
        if (ret != OSD_RET_OK)
            return 5;
        ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X,0);
        if (ret != OSD_RET_OK)
            return 6;
        ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y,0);
        if (ret != OSD_RET_OK)
            return 7;
        OSD_BASE_SetOsdPosition(u4OsdPlane,u4OutX,u4OutY);
        fgLogoInit = TRUE;
    }
    OSD_LockLogoDisplaying();
    bLogoDisplay = TRUE;
    ret = OSD_PLA_Enable(u4OsdPlane, TRUE);
    if (ret != OSD_RET_OK)
        return 8;
    OSD_PLA_SetAvailable(u4OsdPlane, FALSE);
#if OSD_LOGO_FADING_ENABLE
    OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, 0);
#else
    OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, 0xff);
#endif
    OSD_RGN_Set(u4Region, OSD_RGN_MIX_SEL, (UINT32) OSD_BM_REGION);
#if OSD_LOGO_FADING_ENABLE
    for (i = 0; i < 16; i++)
    {
        x_thread_delay(62);
        OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, ((i + 1) * 16 - 1));
    }
    x_thread_delay(1000);
    for (i = 16; i > 0; i--)
    {
        OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, (i * 16 - 1));
        x_thread_delay(62);
    }
#else
    x_thread_delay(2000);
#endif
    OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, 0);
    bLogoDisplay = FALSE;
    OSD_PLA_Enable(u4OsdPlane, FALSE);
    OSD_PLA_SetAvailable(u4OsdPlane, TRUE);
    OSD_UnLockLogoDisplaying();
    OSD_BASE_SetOsdPosition(u4OsdPlane,0,0);
    if (u4OriSC_Enable != 0)
    {
        UNUSED(OSD_SC_Scale(u4Scaler, u4OriSC_Enable, u4OriSrcW, u4OriSrcH,
                        u4OriDstW, u4OriDstH));
    }
    if (i4OriRegionList >= 0)
    {
        OSD_PLA_FlipTo(u4OsdPlane, (UINT32) i4OriRegionList);
    }
    else
    {
        OSD_PLA_FlipToNone(u4OsdPlane);
    }
    if (u1OriPLA_Enable)
    {
        OSD_PLA_Enable(u4OsdPlane, TRUE);
    }
    #ifdef CC_FASTBOOT_DRIVER_LOGO
    OSD_SetFastLogoPlane();
    #endif
#endif  //#ifndef CC_MTK_LOADER
    return (INT32) OSD_RET_OK;
}

// Get OSD FIFO underflow status
BOOL OSD_GetUnderflowStatus(void)
{
    return (OSD_READ32(0xFC)&0x1);
}


// Clear OSD FIFO underflow status
void OSD_ClearUnderflowStatus(void)
{
IO_WRITE32(OSD_BASE, 0x034, 0x0);
IO_WRITE32(OSD_BASE, 0x0FC, 0x4);
}

INT32 OSD_BASE_EnableAutoLR(UINT32 u4Enable)
{

_OSD_BASE_SetAutoLR(u4Enable);
VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());
return (INT32) OSD_RET_OK;
}

INT32 OSD_BASE_GetLRStatus(UINT32* pu4Status)
{

_OSD_BASE_GetLRFlag(pu4Status);
return (INT32) OSD_RET_OK;
}
INT32 OSD_BASE_Get3DOsdShiftEn(UINT32 OSD_Plane,UINT32 *pu4Value)
{
    switch(OSD_Plane)
    {
       case OSD_PLANE_2:
            _OSD_3D_GetOsd2LShiftEn(pu4Value);
            break;
       case OSD_PLANE_3:
            _OSD_3D_GetOsd3LShiftEn(pu4Value);
            break;
       case OSD_PLANE_1:
            _OSD_3D_GetOsd1LShiftEn(pu4Value);
            break;
       default:
            break;
    }
    return (INT32) OSD_RET_OK;
}

INT32 OSD_BASE_Get3DOsd2LShiftEn(UINT32 *pu4Value)
{
_OSD_3D_GetOsd2LShiftEn(pu4Value);
return (INT32) OSD_RET_OK;
}

INT32 OSD_BASE_SetPlaneSwitch(UINT32 u4Enable)
{
UINT32 u4Rgn2, u4Rgn3;
UINT32 u4BmpH2, u4BmpH3;
u4Rgn2 = OSD_PLA_GetFirstRegion(OSD_PLANE_2);
if (u4Rgn2 >= (UINT32) OSD_MAX_NUM_RGN)
{
    Printf(" OSD_PLANE  get region 2 fail \n");
    //return (INT32)OSD_RET_OUT_OF_REGION;
}
else
{
    IGNORE_RET(OSD_RGN_Get(u4Rgn2, (INT32)OSD_RGN_BMP_H, &u4BmpH2));
    LOG(5,"test  u4BmpH2 = %ld\n", u4BmpH2);
    if (u4Enable == TRUE)
    {
        IGNORE_RET(OSD_RGN_Set(u4Rgn2, (INT32)OSD_RGN_DISP_H, u4BmpH2 / 2));
        _OSD_RGN_SetVStep(u4Rgn2, 0x2000);
    }
    else
    {
        IGNORE_RET(OSD_RGN_Set(u4Rgn2, (INT32)OSD_RGN_DISP_H, u4BmpH2));
        _OSD_RGN_SetVStep(u4Rgn2, 0x1000);
    }
}

u4Rgn3 = OSD_PLA_GetFirstRegion(OSD_PLANE_3);
if (u4Rgn3 >= (UINT32) OSD_MAX_NUM_RGN)
{
    Printf(" OSD_PLANE  get region 3  fail \n");
    //  return (INT32)OSD_RET_OUT_OF_REGION;
}
else
{
    IGNORE_RET(OSD_RGN_Get(u4Rgn3, (INT32)OSD_RGN_BMP_H, &u4BmpH3));
    LOG(5,"test  u4BmpH3 = %ld\n", u4BmpH3);
    if (u4Enable == TRUE)
    {
        IGNORE_RET(OSD_RGN_Set(u4Rgn3, (INT32)OSD_RGN_DISP_H, u4BmpH3 / 2));
        _OSD_RGN_SetVStep(u4Rgn3, 0x2000);
    }
    else
    {
        IGNORE_RET(OSD_RGN_Set(u4Rgn3, (INT32)OSD_RGN_DISP_H, u4BmpH3));
        _OSD_RGN_SetVStep(u4Rgn3, 0x1000);
    }
}

_OSD_BASE_SetPlaneSwitch(u4Enable);
_OSD_BASE_SetLineDup(u4Enable);
VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());
return (INT32) OSD_RET_OK;
}

INT32 OSD_BASE_SetLineDup(UINT32 u4Enable)
{
_OSD_BASE_SetLineDup(u4Enable);
VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());
return (INT32) OSD_RET_OK;
}

INT32 OSD_BASE_SetPlaneLineShift(UINT32 u4Plane, BOOL fgEnable, INT32 i4Value)
{
OSD_VERIFY_PLANE(u4Plane);

switch (u4Plane)
{
case OSD_PLANE_1:
    if (fgEnable)
    {
        OSD_3D_SetOsd1LShiftEn(TRUE);
        _OSD_3D_SetOsd1ShiftValue((i4Value > 0) ? i4Value : (0 - i4Value));
    }
    else//OSD_3DPrLshift_ENABLE_FLAG
    {
        OSD_3D_SetOsd1LShiftEn(FALSE);
        _OSD_3D_SetOsd1ShiftValue(0);
    }
    break;
case OSD_PLANE_2:
    if (fgEnable)
    {

        OSD_3D_SetOsd2LShiftEn(TRUE);

        _OSD_3D_SetOsd2ShiftValue((i4Value > 0) ? i4Value : (0 - i4Value));
    }
    else
    {
        OSD_3D_SetOsd2LShiftEn(FALSE);

        _OSD_3D_SetOsd2ShiftValue(0);
    }
    break;
case OSD_PLANE_3:
    if (fgEnable)
    {
        OSD_3D_SetOsd3LShiftEn(TRUE);
        _OSD_3D_SetOsd3ShiftValue((i4Value > 0) ? i4Value : (0 - i4Value));
    }
    else
    {
        OSD_3D_SetOsd3LShiftEn(FALSE);
        _OSD_3D_SetOsd3ShiftValue(0);
    }
    break;
default:
    break;
}
VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());
VERIFY((INT32) OSD_RET_OK == _OSD_3D_UpdateHwReg());
HalFlushDCache();

return (INT32) OSD_RET_OK;
}

INT32 OSD_BASE_Set3DPrLineShift(UINT32 u4Enable, UINT32 u4Shift)
{
INT32 i4Plane2Region = -1, i4Plane3Region = -1, i4Plane1Region = -1;
INT32 u4PlaneOutputXShift = 0, u4PlaneOutputX = 0;
static UINT32 u4Enable_tag = 0;
static UINT32 u4Plane1OutputH_tag = 0, u4Plane2OutputH_tag = 0,
        u4Plane3OutputH_tag = 0;
if (_fgOsdInit == 0)
    return (INT32) OSD_RET_OK;

//printf("u4Enable_tag = 0x%x\n", u4Enable_tag);

if (!(u4Enable_tag))
{
    _OSD_BASE_GetScrnHStartOsd1(&u4Plane1OutputH_tag);
    _OSD_BASE_GetScrnHStartOsd2(&u4Plane2OutputH_tag);
    _OSD_BASE_GetScrnHStartOsd3(&u4Plane3OutputH_tag);

    u4Enable_tag = 1;
    //printf(">>u4Enable_tag = 0x%x\n", u4Enable_tag);
}

if (u4Shift < 0x80)
{
    u4PlaneOutputXShift = -u4Shift / 2;
    _OSD_3D_SetOsd2LShiftInv((UINT32) FALSE);
}
else if (u4Shift > 0x80)
{
    u4PlaneOutputXShift = (u4Shift - 0x80) / 2;
    _OSD_3D_SetOsd2LShiftInv((UINT32) TRUE);
}

i4Plane2Region = OSD_PLA_GetFirstRegion(OSD_PLANE_2);
//Printf(" OSD_PLANE get region = %d \n", i4Plane2Region);
if ((i4Plane2Region >= (UINT32) OSD_MAX_NUM_RGN) || (i4Plane2Region == (-1)))
{
    //Printf(" OSD_PLANE get region fail \n");
    //return (INT32)OSD_RET_OK;
}
else if (i4Plane2Region < OSD_MAX_NUM_RGN)
{
    if (u4Enable == 1)
    {
        u4PlaneOutputX = u4Plane2OutputH_tag + u4PlaneOutputXShift;
        _OSD_BASE_SetScrnHStartOsd2(u4PlaneOutputX);
        OSD_BASE_SetPlaneLineShift(OSD_PLANE_2, TRUE, u4Shift);
    }
    else
    {
        OSD_BASE_SetPlaneLineShift(OSD_PLANE_2, FALSE, 0);
        _OSD_BASE_SetScrnHStartOsd2(u4Plane2OutputH_tag);
        u4Enable_tag = 0;
    }
}

i4Plane3Region = OSD_PLA_GetFirstRegion(OSD_PLANE_3);
if (i4Plane3Region >= (UINT32) OSD_MAX_NUM_RGN)
{
    //Printf(" OSD_PLANE get region fail \n");
    // return (INT32)OSD_RET_OK;
}
else if ((i4Plane3Region < OSD_MAX_NUM_RGN) || (i4Plane3Region == (-1)))
{
    if (u4Enable == 1)
    {
        u4PlaneOutputX = u4Plane3OutputH_tag + u4PlaneOutputXShift;
        _OSD_BASE_SetScrnHStartOsd3(u4PlaneOutputX);
        OSD_BASE_SetPlaneLineShift(OSD_PLANE_3, TRUE, u4Shift);
    }
    else
    {
        OSD_BASE_SetPlaneLineShift(OSD_PLANE_3, FALSE, 0);
        _OSD_BASE_SetScrnHStartOsd3(u4Plane3OutputH_tag);
        u4Enable_tag = 0;
    }
}

i4Plane1Region = OSD_PLA_GetFirstRegion(OSD_PLANE_1);
if ((i4Plane1Region >= (UINT32) OSD_MAX_NUM_RGN) || (i4Plane3Region == (-1)))
{
    //Printf(" OSD_PLANE get region fail \n");
    // return (INT32)OSD_RET_OK;
}
else if (i4Plane1Region < OSD_MAX_NUM_RGN)
{
    if (u4Enable == 1)
    {
        u4PlaneOutputX = u4Plane1OutputH_tag + u4PlaneOutputXShift;
        _OSD_BASE_SetScrnHStartOsd1(u4PlaneOutputX);
        OSD_BASE_SetPlaneLineShift(OSD_PLANE_1, TRUE, u4Shift);
    }
    else
    {
        OSD_BASE_SetPlaneLineShift(OSD_PLANE_1, FALSE, 0);
        _OSD_BASE_SetScrnHStartOsd1(u4Plane1OutputH_tag);
        u4Enable_tag = 0;
    }
}

VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());
HalFlushDCache();

return (INT32) OSD_RET_OK;
}

INT32 OSD_BASE_Set3DShutterLineShift(UINT32 u4Enable, UINT32 u4Shift)
{
UINT32 u4Plane2Region = 0, u4Plane3Region = 0, u4Plane1Region = 0;
INT32 u4PlaneOutputXShift = 0, u4PlaneOutputX = 0;
static UINT32 u4Enable_tag = 0;
static UINT32 u4Plane1OutputH_tag = 0, u4Plane2OutputH_tag = 0,
        u4Plane3OutputH_tag = 0;

LOG(5,"u4Enable_tag = 0x%x\n", u4Enable_tag);

if (!(u4Enable_tag))
{
    _OSD_BASE_GetScrnHStartOsd1(&u4Plane1OutputH_tag);
    _OSD_BASE_GetScrnHStartOsd2(&u4Plane2OutputH_tag);
    _OSD_BASE_GetScrnHStartOsd3(&u4Plane3OutputH_tag);

    u4Enable_tag = 1;
}
if (u4Shift < 0x80)
{
    u4PlaneOutputXShift = -u4Shift / 2;
}
else if (u4Shift > 0x80)
{
    u4PlaneOutputXShift = (u4Shift - 0x80) / 2;
}

u4Plane2Region = OSD_PLA_GetFirstRegion(OSD_PLANE_2);
if (u4Plane2Region >= (UINT32) OSD_MAX_NUM_RGN)
{
    Printf(" OSD_PLANE get region fail \n");
    //return (INT32)OSD_RET_OK;
}
else if (u4Plane2Region < OSD_MAX_NUM_RGN)
{
    if (u4Enable == 1)
    {
        u4PlaneOutputX = u4Plane2OutputH_tag + u4PlaneOutputXShift;
        _OSD_BASE_SetScrnHStartOsd2(u4PlaneOutputX);

        _OSD_3D_SetShutterShiftEn(TRUE);
        _OSD_3D_SetTgenLREn(TRUE);
        _OSD_3D_SetOsd2ShiftValue((u4Shift > 0) ? u4Shift : (0 - u4Shift));
    }
    else
    {
        _OSD_3D_SetShutterShiftEn(FALSE);
        _OSD_3D_SetTgenLREn(FALSE);
        _OSD_3D_SetOsd2ShiftValue(0);

        _OSD_BASE_SetScrnHStartOsd2(u4Plane2OutputH_tag);
        u4Enable_tag = 0;
    }
}

u4Plane3Region = OSD_PLA_GetFirstRegion(OSD_PLANE_3);
if (u4Plane3Region >= (UINT32) OSD_MAX_NUM_RGN)
{
    Printf(" OSD_PLANE get region fail \n");
    // return (INT32)OSD_RET_OK;
}
else if (u4Plane3Region < OSD_MAX_NUM_RGN)
{
    if (u4Enable == 1)
    {
        u4PlaneOutputX = u4Plane3OutputH_tag + u4PlaneOutputXShift;
        _OSD_BASE_SetScrnHStartOsd3(u4PlaneOutputX);

        _OSD_3D_SetShutterShiftEn(TRUE);
        _OSD_3D_SetTgenLREn(TRUE);
        _OSD_3D_SetOsd2ShiftValue((u4Shift > 0) ? u4Shift : (0 - u4Shift));
    }
    else
    {
        _OSD_3D_SetShutterShiftEn(FALSE);
        _OSD_3D_SetTgenLREn(FALSE);
        _OSD_3D_SetOsd2ShiftValue(0);

        _OSD_BASE_SetScrnHStartOsd3(u4Plane3OutputH_tag);
        u4Enable_tag = 0;
    }
}

u4Plane1Region = OSD_PLA_GetFirstRegion(OSD_PLANE_1);
if (u4Plane1Region >= (UINT32) OSD_MAX_NUM_RGN)
{
    Printf(" OSD_PLANE get region fail \n");
    // return (INT32)OSD_RET_OK;
}
else if (u4Plane1Region < OSD_MAX_NUM_RGN)
{
    if (u4Enable == 1)
    {
        u4PlaneOutputX = u4Plane1OutputH_tag + u4PlaneOutputXShift;
        _OSD_BASE_SetScrnHStartOsd1(u4PlaneOutputX);

        _OSD_3D_SetShutterShiftEn(TRUE);
        _OSD_3D_SetTgenLREn(TRUE);
        _OSD_3D_SetOsd2ShiftValue((u4Shift > 0) ? u4Shift : (0 - u4Shift));
    }
    else
    {
        _OSD_3D_SetShutterShiftEn(FALSE);
        _OSD_3D_SetTgenLREn(FALSE);
        _OSD_3D_SetOsd2ShiftValue(0);

        _OSD_BASE_SetScrnHStartOsd1(u4Plane1OutputH_tag);
        u4Enable_tag = 0;
    }
}
VERIFY((INT32) OSD_RET_OK == _OSD_BASE_UpdateHwReg());
VERIFY((INT32) OSD_RET_OK == _OSD_3D_UpdateHwReg());
return (INT32) OSD_RET_OK;
}

INT32 OSD_BASE_SetLineShift(UINT32 u4Enable, UINT32 u4Shift)
{

switch (u4Enable)
{
case 0:
    OSD_BASE_Set3DPrLineShift(FALSE, 0);
    OSD_BASE_Set3DShutterLineShift(FALSE, 0);
    break;
case 1:
    OSD_BASE_Set3DPrLineShift(TRUE, u4Shift);
    break;
case 2:
    OSD_BASE_Set3DShutterLineShift(TRUE, u4Shift);
    break;
case 3:
    OSD_BASE_Set3DPrLineShift(FALSE, 0);
    break;
case 4:
    OSD_BASE_Set3DShutterLineShift(FALSE, 0);
    break;
default:
    break;

}
return (INT32) OSD_RET_OK;
}

INT32 OSD_BASE_GetHTotalPixel(UINT32 *pu4Value)
{
if (pu4Value == NULL)
{
    return -(INT32) OSD_RET_INV_ARG;
}

*pu4Value = (OSD_READ32(0x48) & 0x3FFF0000) >> 16;
return (INT32) OSD_RET_OK;
}
BOOL OSD_BASE_Get_3DOSD2PLsEn()
{
return OSD2_3DPrLshift_ENABLE_FLAG;
}

void OSD_BASE_Set_3DOSD2PLsEn(BOOL u4Enable)
{
OSD2_3DPrLshift_ENABLE_FLAG = u4Enable;
}

INT32 OSD_3D_SetOsd2LShiftEn(UINT32 u4Value)
{
    UINT32 u4_rgn   = 0x0;
    UINT32 u4_dec   = 0x0;

    u4_rgn = OSD_PLA_GetFirstRegion(OSD_PLANE_2);
    VERIFY(OSD_RGN_Get(u4_rgn,OSD_RGN_DEC_EN,&u4_dec) == OSD_RET_OK);

    if(u4_dec)
    {
        OSD_BASE_Set_3DOSD2PLsEn(u4Value);
    }
    else
    {
        _OSD_3D_SetOsd2LShiftEn(u4Value);
        if(u4Value)
        {
            OSD_RGN_Set(u4_rgn, OSD_RGN_STEP_V, 0x2000);
        }
        else
        {
            OSD_RGN_Set(u4_rgn, OSD_RGN_STEP_V, 0x1000);
        }
    }

    return (INT32)OSD_RET_OK;
}

INT32 OSD_3D_SetOsd1LShiftEn(UINT32 u4Value)
{
    UINT32 u4_rgn   = 0x0;
    UINT32 u4_dec   = 0x0;

    u4_rgn = OSD_PLA_GetFirstRegion(OSD_PLANE_1);
    VERIFY(OSD_RGN_Get(u4_rgn,OSD_RGN_DEC_EN,&u4_dec) == OSD_RET_OK);

    if(u4_dec)
    {
        OSD_BASE_Set_3DOSD2PLsEn(u4Value);
    }
    else
    {
        _OSD_3D_SetOsd1LShiftEn(u4Value);
        if(u4Value)
        {
            OSD_RGN_Set(u4_rgn, OSD_RGN_STEP_V, 0x2000);
        }
        else
        {
            OSD_RGN_Set(u4_rgn, OSD_RGN_STEP_V, 0x1000);
        }
    }

    return (INT32)OSD_RET_OK;
}


INT32 OSD_3D_SetOsd3LShiftEn(UINT32 u4Value)
{
    UINT32 u4_rgn   = 0x0;
    UINT32 u4_dec   = 0x0;

    u4_rgn = OSD_PLA_GetFirstRegion(OSD_PLANE_3);
    VERIFY(OSD_RGN_Get(u4_rgn,OSD_RGN_DEC_EN,&u4_dec) == OSD_RET_OK);

    if(u4_dec)
    {
        OSD_BASE_Set_3DOSD2PLsEn(u4Value);
    }
    else
    {
        _OSD_3D_SetOsd3LShiftEn(u4Value);
        if(u4Value)
        {
            OSD_RGN_Set(u4_rgn, OSD_RGN_STEP_V, 0x2000);
        }
        else
        {
            OSD_RGN_Set(u4_rgn, OSD_RGN_STEP_V, 0x1000);
        }
    }

    return (INT32)OSD_RET_OK;
}



BOOL OSD_BASE_IsDisplaying(void)
{
    BOOL fgEnable = 0;
    UINT32 u4Enabe1 = 0, u4Enable2 = 0;
    #ifdef CONFIG_OSD_COMPRESSION
    UINT32 u4ResetValue=0;
    #endif
    _OSD_PLA_GetEnable(OSD_PLANE_2, &u4Enabe1);
    _OSD_PLA_GetEnable(OSD_PLANE_3, &u4Enable2);
    #ifdef CONFIG_OSD_COMPRESSION
    u4ResetValue=IO_READ32(OSD_BASE_REG, 0x4);
    IO_WRITE32(OSD_BASE_REG, 0x4, u4ResetValue & 0xFFFFFFFBU);
    OSD_UpdateReg();
    OSD_WaitVsync();
    OSD_WaitVsync();
    #endif
    if ((u4Enabe1 && IO_READ32(0xF0021000, 0x5F8)) || (u4Enable2 && IO_READ32(
            0xF0021000, 0x6F8)))
    {
        fgEnable = TRUE;
    }
    else
    {
        fgEnable = FALSE;
    }
    #ifdef CONFIG_OSD_COMPRESSION
    IO_WRITE32(OSD_BASE_REG, 0x4, u4ResetValue);
    OSD_UpdateReg();
    #endif
    return fgEnable;
}

BOOL OSD_BASE_SonyIsDisplaying(void)
{
    static BOOL fgEnable = 0;
    UINT32 u4Enabe1 = 0, u4Enable2 = 0;
#ifdef CONFIG_OSD_COMPRESSION
    static UINT32 u4ResetValue=0;
#endif
    //static UINT8 bVyncCouter = 3;

    _OSD_PLA_GetEnable(OSD_PLANE_2, &u4Enabe1);
    _OSD_PLA_GetEnable(OSD_PLANE_3, &u4Enable2);

#ifdef CONFIG_OSD_COMPRESSION
    if( bVyncCouter == 3)
    {
        u4ResetValue=IO_READ32(OSD_BASE_REG, 0x4);
        IO_WRITE32(OSD_BASE_REG, 0x4, u4ResetValue & 0xFFFFFFFBU);
        OSD_UpdateReg();
        //OSD_WaitVsync();
        //OSD_WaitVsync();
       // OSD_WaitVsync();
    }
#endif

    if(bVyncCouter-- == 0)
    {
        if ((u4Enabe1 && IO_READ32(0xF0021000, 0x5F8)) || (u4Enable2 && IO_READ32(
                0xF0021000, 0x6F8)))
        {
            fgEnable = TRUE;
        }
        else
        {
            fgEnable = FALSE;
        }
    #ifdef CONFIG_OSD_COMPRESSION
        IO_WRITE32(OSD_BASE_REG, 0x4, u4ResetValue);
        OSD_UpdateReg();
    #endif

        bVyncCouter = 3;
    }
    return fgEnable;
}


VOID OSD_BASE_GetPanelSize(UINT32 *u4Width, UINT32 *u4Height)
{
     if(u4Width)
        *u4Width = OSD_BASE_GetDisplayWidth();

     if(u4Height)
        *u4Height = OSD_BASE_GetDisplayHeight();
}


INT32 OSD_3D_SetPlaneSwitch(UINT32 u4Enable)
{
#ifdef CC_ANDROID_3D_UI_SUPPORT
_OSD_3D_SetPlaneSwitchEn(u4Enable);
VERIFY((INT32)OSD_RET_OK == _OSD_3D_UpdateHwReg());
#endif
return (INT32) OSD_RET_OK;

}

#ifdef CC_TDTV_3D_OSD_ENABLE
INT32 OSD_BASE_Enabe3DShutter( UINT32 u4Enable, UINT32 u4Shift)
{
UINT32 u4Plane2Region = 0, u4Plane3Region = 0;
UINT32 u4Plane2RightRegion = 0, u4Plane3RightRegion = 0;
UINT32 u4u4LeftRgnAddr;
UINT32 u4u4RightRgnAddr;
UINT32 u4RegionPosX = 0;

if (u4Enable == 0)
{
    if (_u1Plane2RightRegion != 0)
    {
        OSD_VERIFY_REGION(_u1Plane2RightRegion);
        OSD_RGN_Delete(_u1Plane2RightRegion);
        _u1Plane2RightRegion = 0;
        _OSD_PLA_SetRightHeaderAddr(OSD_PLANE_2, 0);
        OSD_PLA_Set3DMode(OSD_PLANE_2, u4Enable);
        VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_2));
    }
    if (_u1Plane3RightRegion != 0)
    {
        OSD_VERIFY_REGION(_u1Plane3RightRegion);
        OSD_RGN_Delete(_u1Plane3RightRegion);
        _u1Plane3RightRegion = 0;
        _OSD_PLA_SetRightHeaderAddr(OSD_PLANE_3, 0);
        OSD_PLA_Set3DMode(OSD_PLANE_3, u4Enable);
        VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_3));
    }
    return (INT32)OSD_RET_OK;
}

u4Plane2Region = OSD_PLA_GetFirstRegion(OSD_PLANE_2);
u4Plane3Region = OSD_PLA_GetFirstRegion(OSD_PLANE_3);
if ((u4Plane2Region >= (UINT32)OSD_MAX_NUM_RGN) && (u4Plane3Region >= (UINT32)OSD_MAX_NUM_RGN))
{
    Printf(" OSD_PLANE get region fail \n");
    return (INT32)OSD_RET_OK;
}
if (u4Plane2Region < OSD_MAX_NUM_RGN)
{
    UINT32 u4BmpW, u4BmpH, u4DispW, u4DispH, u4ColorMode;
    UINT32 u4BmpAddr, u4BmpPitch;
    UINT32 u4ret;

    if (_u1Plane2RightRegion != 0)
    {
        Printf(" OSD Plane 2 3D  Shutter have enable l \n");
    }
    else
    {
        OSD_VERIFY_REGION(u4Plane2Region);

        IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_BMP_W, &u4BmpW));
        IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_BMP_H, &u4BmpH));
        IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_DISP_W, &u4DispW));
        IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_DISP_H, &u4DispH));
        IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_COLORMODE, &u4ColorMode));
        IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_BMP_ADDR, &u4BmpAddr));
        IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_BMP_PITCH, &u4BmpPitch));

        u4ret = OSD_RGN_Create(&u4Plane2RightRegion, u4BmpW, u4BmpH, (void *)u4BmpAddr,
                u4ColorMode, u4BmpPitch, 0, 0, u4DispW, u4DispH);
        if (u4ret != OSD_RET_OK)
        {
            Printf("OSD create region fail \n");
            return 2;
        }

        VERIFY((INT32)OSD_RET_OK ==
                _OSD_RGN_GetAddress((UINT32)u4Plane2Region, &u4u4LeftRgnAddr));

        VERIFY((INT32)OSD_RET_OK ==
                _OSD_RGN_GetAddress((UINT32)u4Plane2RightRegion, &u4u4RightRgnAddr));

        x_memcpy((void*)u4u4RightRgnAddr, (void*)u4u4LeftRgnAddr, sizeof(OSD_RGN_UNION_T));

        if (u4Shift < 0x40 && u4Shift > 0)
        {
            IGNORE_RET(OSD_RGN_Get(u4Plane2Region, (INT32)OSD_RGN_POS_X, &u4RegionPosX));
            u4RegionPosX = u4RegionPosX + u4Shift;
            LOG(5," OSD Plane test 000000   %d l \n", u4RegionPosX);
            IGNORE_RET(OSD_RGN_Set(u4Plane2RightRegion, (INT32)OSD_RGN_POS_X, u4RegionPosX));
        }

        VERIFY((INT32)OSD_RET_OK ==
                _OSD_PLA_SetRightHeaderAddr(OSD_PLANE_2, u4u4RightRgnAddr));

        OSD_PLA_Set3DMode(OSD_PLANE_2, u4Enable);
        VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_2));

        _u1Plane2RightRegion = u4Plane2RightRegion;
    }
}

if (u4Plane3Region < OSD_MAX_NUM_RGN)
{
    UINT32 u4BmpW, u4BmpH, u4DispW, u4DispH, u4ColorMode;
    UINT32 u4BmpAddr, u4BmpPitch;
    UINT32 u4ret;

    if (_u1Plane3RightRegion != 0)
    {
        Printf(" OSD Plane 3 3D  Shutter have enable l \n");
    }
    else
    {
        OSD_VERIFY_REGION(u4Plane3Region);

        IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_BMP_W, &u4BmpW));
        IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_BMP_H, &u4BmpH));
        IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_DISP_W, &u4DispW));
        IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_DISP_H, &u4DispH));
        IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_COLORMODE, &u4ColorMode));
        IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_BMP_ADDR, &u4BmpAddr));
        IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_BMP_PITCH, &u4BmpPitch));

        u4ret = OSD_RGN_Create(&u4Plane3RightRegion, u4BmpW, u4BmpH, (void *)u4BmpAddr,
                u4ColorMode, u4BmpPitch, 0, 0, u4DispW, u4DispH);
        if (u4ret != OSD_RET_OK)
        {
            Printf("OSD create region fail \n");
            return 2;
        }
        VERIFY((INT32)OSD_RET_OK ==
                _OSD_RGN_GetAddress((UINT32)u4Plane3Region, &u4u4LeftRgnAddr));

        VERIFY((INT32)OSD_RET_OK ==
                _OSD_RGN_GetAddress((UINT32)u4Plane3RightRegion, &u4u4RightRgnAddr));

        x_memcpy((void*)u4u4LeftRgnAddr, (void*)&u4u4RightRgnAddr, sizeof(OSD_RGN_UNION_T));

        if (u4Shift < 0x40 && u4Shift > 0)
        {
            IGNORE_RET(OSD_RGN_Get(u4Plane3Region, (INT32)OSD_RGN_POS_X, &u4RegionPosX));
            u4RegionPosX = u4RegionPosX + u4Shift;
            IGNORE_RET(OSD_RGN_Set(u4Plane3RightRegion, (INT32)OSD_RGN_POS_X, u4RegionPosX));
        }

        VERIFY((INT32)OSD_RET_OK ==
                _OSD_PLA_SetRightHeaderAddr(OSD_PLANE_3, u4u4RightRgnAddr));
        OSD_PLA_Set3DMode(OSD_PLANE_3, u4Enable);
        VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_3));
        _u1Plane3RightRegion = u4Plane3RightRegion;
    }
}
return (INT32)OSD_RET_OK;
}

INT32 OSD_BASE_Set3DStatus(void)
{
S_TDTV_OSD_CONFIG sOsdCfg=eDrvTDTVOSDModeQuery();
if(_u13DOsdShift !=0) //for cli test use

{
    return (INT32)OSD_RET_OK;
}

switch(sOsdCfg.eOSDType)
{
    case E_TDTV_OSD_2D:
    OSD_BASE_Enabe3DShutter(FALSE, 0);
    gfgOsd_Sbs = 0;
    gfgOsd_Tbb = 0;
    _u13DOsdShift = 0;
    break;
    case E_TDTV_OSD_FS:
    gfgOsd_Sbs = 0;
    gfgOsd_Tbb = 0;
    _u13DOsdShift = 0;
    OSD_BASE_Enabe3DShutter(TRUE, 0);
    break;
    case E_TDTV_OSD_SBS:
    OSD_BASE_Enabe3DShutter(FALSE, 0);
    gfgOsd_Sbs = 1;
    gfgOsd_Tbb = 0;
    _u13DOsdShift = 0;
    break;
    case E_TDTV_OSD_TB:
    OSD_BASE_Enabe3DShutter(FALSE, 0);
    gfgOsd_Sbs = 0;
    gfgOsd_Tbb = 1;
    _u13DOsdShift = 0;
    break;
    default:
    OSD_BASE_Enabe3DShutter(FALSE, 0);
    gfgOsd_Sbs = 0;
    gfgOsd_Tbb = 0;
    _u13DOsdShift = 0;
    break;
}
return (INT32)OSD_RET_OK;
}

#endif

#ifdef OSD_TRANS_CTRL
UINT32 OSD_BASE_GetTranBuf_Current(void)
{
return _OSD_PlANE2_TransferBuf_CURRENT;
}

UINT32 OSD_BASE_GetTranBuf_Back(void)
{
UINT32 u4Buf = OSD_BASE_GetTranBuf_Current();
if(u4Buf == _OSD_PlANE2_TransferBuf_1)
{
    return _OSD_PlANE2_TransferBuf_2;
}
else
{
    return _OSD_PlANE2_TransferBuf_1;
}

}

INT32 OSD_BASE_TransferTo_SYNC_Header(UINT32 u4Region)
{
UINT32 u4Value = 0;
OSD_VERIFY_REGION(u4Region);

OSD_RGN_Get(u4Region,OSD_RGN_COLORMODE,&u4Value);
if(u4Value == OSD_CM_ARGB8888_DIRECT32)
{
    OSD_BASE_SetFRCHeader(OSD_FRCHD_FORMAT,4);
}
else if(u4Value == OSD_CM_AYCBCR8888_DIRECT32)
{
    OSD_BASE_SetFRCHeader(OSD_FRCHD_FORMAT,5);
}

//OSD_RGN_Get(u4Region,,&u4Value);
//OSD_BASE_SetFRCHeader(OSD_FRCHD_PIXORDER,u4Value);

OSD_RGN_Get(u4Region,OSD_RGN_BMP_H,&u4Value);
OSD_BASE_SetFRCHeader(OSD_FRCHD_BMPHEIGHT,u4Value);
OSD_RGN_Get(u4Region,OSD_RGN_BMP_W,&u4Value);
OSD_BASE_SetFRCHeader(OSD_FRCHD_BMPWIDTH,u4Value);
OSD_RGN_Get(u4Region,OSD_RGN_ALPHA,&u4Value);
OSD_BASE_SetFRCHeader(OSD_FRCHD_GLOBALALPHA,u4Value);

OSD_RGN_Get(u4Region,OSD_RGN_ALPHA_SEL,&u4Value);
if(u4Value == 1)
{
    OSD_BASE_SetFRCHeader(OSD_FRCHD_PIXELALPHAEN,TRUE);
    OSD_BASE_SetFRCHeader(OSD_FRCHD_GLOBALALPHAEN,FALSE);
}
else
{
    OSD_BASE_SetFRCHeader(OSD_FRCHD_GLOBALALPHAEN,TRUE);
    OSD_BASE_SetFRCHeader(OSD_FRCHD_PIXELALPHAEN,FALSE);
}

OSD_RGN_Get(u4Region,OSD_RGN_POS_Y,&u4Value);
OSD_BASE_SetFRCHeader(OSD_FRCHD_DISPYOFF,u4Value);
OSD_RGN_Get(u4Region,OSD_RGN_POS_X,&u4Value);
OSD_BASE_SetFRCHeader(OSD_FRCHD_DISPXOFF,u4Value);
OSD_RGN_Get(u4Region,OSD_RGN_OUT_H,&u4Value);
OSD_BASE_SetFRCHeader(OSD_FRCHD_DISPHEIGHT,u4Value);
OSD_RGN_Get(u4Region,OSD_RGN_OUT_W,&u4Value);
OSD_BASE_SetFRCHeader(OSD_FRCHD_DISPWIDTH,u4Value);
return OSD_RET_OK;
}

INT32 OSD_BASE_TransferTo(UINT32 u4SrcAddr,UINT32 u4DstAddr)
{
#ifndef CC_MTK_LOADER
UINT32 u4Region,u4ColorMode,u4Pitch,u4Width,u4Height;
u4Region = OSD_PLA_GetFirstRegion(OSD_PLANE_2);
OSD_VERIFY_REGION(u4Region);

OSD_RGN_Get(u4Region,OSD_RGN_COLORMODE,&u4ColorMode);
OSD_RGN_Get(u4Region,OSD_RGN_BMP_PITCH,&u4Pitch);
OSD_RGN_Get(u4Region,OSD_RGN_BMP_W,&u4Width);
OSD_RGN_Get(u4Region,OSD_RGN_BMP_H,&u4Height);

OSD_BASE_TransferTo_SYNC_Header(u4Region);

_OSD_COPY_FRCHeader(VIRTUAL(u4DstAddr));
GFX_SetSrc((UINT8 *)u4SrcAddr,u4ColorMode,u4Pitch);
GFX_SetDst((UINT8 *)u4DstAddr,u4ColorMode,u4Pitch);
GFX_SetBltOpt(0,0,0);
GFX_BitBlt(0,0,0,1,u4Width,u4Height);
GFX_Flush();

OSD_RGN_Set(u4Region,OSD_RGN_BMP_ADDR,u4DstAddr);
#endif
return (INT32)OSD_RET_OK;
}

INT32 OSD_BASE_COPY_FRCHeader(UINT32 u4DstAddr)
{
_OSD_COPY_FRCHeader(u4DstAddr);
return (INT32)OSD_RET_OK;
}

INT32 OSD_BASE_InitFRCHeader(void)
{
_OSD_InitFRCHeader();
return (INT32)OSD_RET_OK;
}

INT32 OSD_BASE_SetTransferCtl(UINT32 u4Enable)
{
_OSD_BASE_SetTransferCtl(u4Enable);
VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
return (INT32)OSD_RET_OK;
}

INT32 OSD_BASE_GetTransferCtl(UINT32 * u4Enable)
{
_OSD_BASE_GetTransferCtl(u4Enable);
//VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
return (INT32)OSD_RET_OK;
}

INT32 OSD_BASE_GetFRCHeader(UINT32 u4SetCMD, UINT32 * u4Value )
{
switch(u4SetCMD)
{
    case OSD_FRCHD_FORMAT:
    return _OSD_FRCHeader_GetFormat( u4Value);

    case OSD_FRCHD_PIXORDER:
    return _OSD_FRCHeader_GetPixOrder( u4Value);
    case OSD_FRCHD_LRINDEX:
    return _OSD_FRCHeader_GetLRindex( u4Value);
    case OSD_FRCHD_BMPINDEX:
    return _OSD_FRCHeader_GetBmpindex( u4Value);
    case OSD_FRCHD_FRAMENUM:
    return _OSD_FRCHeader_GetFramenum( u4Value);
    case OSD_FRCHD_PREFIX:
    return _OSD_FRCHeader_GetPrefix( u4Value);
    case OSD_FRCHD_BMPHEIGHT:
    return _OSD_FRCHeader_GetBmpHeight( u4Value);
    case OSD_FRCHD_BMPWIDTH:
    return _OSD_FRCHeader_GetBmpWidth( u4Value);
    case OSD_FRCHD_GLOBALALPHA:
    return _OSD_FRCHeader_GetGlobalAlpha( u4Value);
    case OSD_FRCHD_PIXELALPHAEN:
    return _OSD_FRCHeader_GetPixelAlphaEn( u4Value);
    case OSD_FRCHD_GLOBALALPHAEN:
    return _OSD_FRCHeader_GetGlobalAlphaEn( u4Value);
    case OSD_FRCHD_ODPUPPER:
    return _OSD_FRCHeader_GetOsdUpper( u4Value);
    case OSD_FRCHD_DISPYOFF:
    return _OSD_FRCHeader_GetDispYOFF( u4Value);
    case OSD_FRCHD_DISPXOFF:
    return _OSD_FRCHeader_GetDispXOFF( u4Value);
    case OSD_FRCHD_DISPHEIGHT:
    return _OSD_FRCHeader_GetDispHeight( u4Value);
    case OSD_FRCHD_DISPWIDTH:
    return _OSD_FRCHeader_GetDispWidth( u4Value);
    default:
    break;
}
LOG(5,"u4Value = %d",u4Value);
return (INT32)OSD_RET_OK;
}

INT32 OSD_BASE_SetFRCHeader(UINT32 u4SetCMD, UINT32 u4Value )
{
switch(u4SetCMD)
{
    case OSD_FRCHD_FORMAT:
    _OSD_FRCHeader_SetFormat( u4Value);
    break;
    case OSD_FRCHD_PIXORDER:
    _OSD_FRCHeader_SetPixOrder( u4Value);
    break;
    case OSD_FRCHD_LRINDEX:
    _OSD_FRCHeader_SetLRindex( u4Value);
    break;
    case OSD_FRCHD_BMPINDEX:
    _OSD_FRCHeader_SetBmpindex( u4Value);
    break;
    case OSD_FRCHD_FRAMENUM:
    _OSD_FRCHeader_SetFramenum( u4Value);
    break;
    case OSD_FRCHD_PREFIX:
    _OSD_FRCHeader_SetPrefix( u4Value);
    break;
    case OSD_FRCHD_BMPHEIGHT:
    _OSD_FRCHeader_SetBmpHeight( u4Value);
    break;
    case OSD_FRCHD_BMPWIDTH:
    _OSD_FRCHeader_SetBmpWidth( u4Value);
    break;
    case OSD_FRCHD_GLOBALALPHA:
    _OSD_FRCHeader_SetGlobalAlpha( u4Value);
    break;
    case OSD_FRCHD_PIXELALPHAEN:
    _OSD_FRCHeader_SetPixelAlphaEn( u4Value);
    break;
    case OSD_FRCHD_GLOBALALPHAEN:
    _OSD_FRCHeader_SetGlobalAlphaEn( u4Value);
    break;
    case OSD_FRCHD_ODPUPPER:
    _OSD_FRCHeader_SetOsdUpper( u4Value);
    break;
    case OSD_FRCHD_DISPYOFF:
    _OSD_FRCHeader_SetDispYOFF( u4Value);
    break;
    case OSD_FRCHD_DISPXOFF:
    _OSD_FRCHeader_SetDispXOFF( u4Value);
    break;
    case OSD_FRCHD_DISPHEIGHT:
    _OSD_FRCHeader_SetDispHeight( u4Value);
    break;
    case OSD_FRCHD_DISPWIDTH:
    _OSD_FRCHeader_SetDispWidth( u4Value);
    break;
    default:
    break;

}
return (INT32)OSD_RET_OK;
}

#endif

INT32 OSD_BASE_SetScrnHStart(UINT32 u4_plane,UINT32 u4_value)
{
    INT32 i4_ret = OSD_RET_OK;
    OSD_VERIFY_PLANE(u4_plane);
    switch(u4_plane)
    {
        case OSD_PLANE_1:
            _OSD_BASE_SetScrnHStartOsd1(u4_value);
            break;
        case OSD_PLANE_2:
            _OSD_BASE_SetScrnHStartOsd2(u4_value);
            break;
        case OSD_PLANE_3:
            _OSD_BASE_SetScrnHStartOsd3(u4_value);
            break;
        #ifdef OSD_PLA5_SUPPORT
        case OSD_PLANE_5:
            _OSD_BASE_SetScrnHStartOsd3(u4_value);
            break;
        #endif
        default:
            i4_ret = OSD_RET_INV_ARG;
            break;
    }
    return i4_ret;
}

INT32 OSD_BASE_SetScrnVStartTop(UINT32 u4_value)
{
    INT32 i4_ret = OSD_RET_OK;
    i4_ret = _OSD_BASE_SetScrnVStartTop(u4_value);
    return i4_ret;
}

INT32 OSD_BASE_UpdateHwReg(void)
{
    return _OSD_BASE_UpdateHwReg();
}
void  OSD_BASE_SwitchResolution(void)
{
  #if  defined(GAL_DDI_SUPPORT)
    static BOOL fgPrev3DModeOn[OSD_SCALER_MAX_NUM]; // the flag if the previous mode was in s3d or not.
    UINT32 u4Scaler;
    UINT32 u4Plane;
    UINT32 u4OriSC_Enable = 0;
    UINT32 u4OriSrcW = 0;
    UINT32 u4OriSrcH = 0;
    UINT32 u4OriDstW = 0;
    UINT32 u4OriDstH = 0;
    UINT32 u4OriIs16Bpp = 0;
    INT32  nMaxScalers;
    BOOL fgEnable;
    UINT32 i = 0;

    if (_fgOsdInit == 0)
        return;

    _OSD_BASE_SetScrnHSize(OSD_BASE_GetDisplayWidth());
    _OSD_BASE_SetScrnVSize(OSD_BASE_GetDisplayHeight());

    nMaxScalers = OSD_SCALER_MAX_NUM - 1;

    if(IS_PANEL_2D_N_3D_L12R12)
        nMaxScalers += 1;

    for(i = 0; i < nMaxScalers; i++ )
    {
        u4Scaler = i;
        u4Plane =  u4Scaler +1;

        OSD_PLA_GetEnable( u4Plane, &fgEnable);
        OSD_PLA_Enable( u4Plane, FALSE);

        OSD_SC_GetScalerInfo(u4Scaler, &u4OriSC_Enable,
                     &u4OriSrcW, &u4OriSrcH,
                     &u4OriDstW, &u4OriDstH,
                     &u4OriIs16Bpp);
        if(u4OriDstW == 0) u4OriDstW = OSD_BASE_GetDisplayWidth();
        if(u4OriDstH == 0) u4OriDstH  = OSD_BASE_GetDisplayHeight();

        LOG(5,"scaler[%d]: src_w=%d, src_h=%d, dst_w=%d, dst_h=%d\n", u4Scaler, u4OriSrcW, u4OriSrcH, u4OriDstW, u4OriDstH);

        if(IS_PANEL_L12R12)
        {
            if(fgPrev3DModeOn[i]) u4OriDstH <<= 1;
            OSD_SC_Scale(u4Scaler, 1, u4OriSrcW, u4OriSrcH, u4OriDstW, u4OriDstH);
            fgPrev3DModeOn[i] = TRUE;
        }
        else
        {
            if(u4OriDstH > OSD_BASE_GetDisplayHeight()) u4OriDstH >>= 1;
            OSD_SC_Scale(u4Scaler, 1, u4OriSrcW, u4OriSrcH, u4OriDstW, u4OriDstH);
            fgPrev3DModeOn[i] = FALSE;
        }
        LOG(5,"   => switch to src_w=%d, src_h=%d, dst_w=%d, dst_h=%d\n", u4OriSrcW, u4OriSrcH, u4OriDstW, u4OriDstH);
        OSD_PLA_Enable(u4Plane, fgEnable);
    }
   #endif
}
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
static void vOsdHeaderUpdateCallbak(UINT16 u2Vector)
{
    UINT32 u4Status =0;
    UNUSED(u2Vector);
    u4Status = IO_REG32(OSD_BASE, 0x34);

    if (0 == (u4Status & 0x20000)) //DMA done
    {
        printf("[OSD]Error, no interrupt status \n");
        if (!BIM_ClearIrq(VECTOR_OSD))
        {
        }
        return;
    }
    // check OSD1 header irq status
    u4Status = IO_READ32(OSD_PLA1_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        //printf("[OSD]Receive OSD1 header update irq \n");
        // clea irq
        VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetClrHdrIrq(OSD_PLANE_1,1));
        VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_1));
    }

    // check OSD2 header irq status
    u4Status = IO_READ32(OSD_PLA2_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        //printf("[OSD]Receive OSD2 header update irq \n");
        // clea irq
        VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetClrHdrIrq(OSD_PLANE_2,1));
        VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_2));
    }

    // check OSD3 header irq status
    u4Status = IO_READ32(OSD_PLA3_REG, 0);
    if ((u4Status & 0x1000000) !=0)
    {
        //printf("[OSD]Receive OSD3 header update irq \n");
        // clea irq
        VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetClrHdrIrq(OSD_PLANE_3,1));
        VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(OSD_PLANE_3));
    }
    // clear global isr
    if (!BIM_ClearIrq(VECTOR_OSD))
    {
    }

    return;
}
#endif



BOOL OSD_BaseIsSupport4K2K(void)
{
    #ifdef OSD_4K2K_SUPPORT
    return TRUE;
    #else
    return FALSE;
    #endif
}
#ifdef OSD_4K2K_SUPPORT
UINT32 OSD_GetVsyncDetectionTimer(void)
{
    return _hOsdVsyncDetectTimer;
}
void OSD_GetMJC(UINT32 *pMJC)
{
    pMJC= 0;
}
void OSD_BaseDisplayFor4K2K(HANDLE_T pt_tm_handle, VOID* pv_tag)
{
#ifndef CC_MTK_LOADER
    UINT32 MJC_Step=0;
#endif
    BOOL bOSD2Enable=FALSE;
    BOOL bOSD3Enable=FALSE;
    OSD_PLA_GetEnable(OSD_PLANE_2,&bOSD2Enable);
    OSD_PLA_GetEnable(OSD_PLANE_3,&bOSD3Enable);
    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);
#ifndef CC_MTK_LOADER
    if(u4LinuxUiMode4K2K!=OSD_4K2K_MODE_MVC)
    {
        return ;
    }
    if(u4LinuxUiMode4K2K == OSD_4K2K_MODE_MVC)
    {
        OSD_GetMJC(&MJC_Step);
        switch(MJC_Step)
        {
           case 0:
            if(bOSD2Enable)
            {
               OSD_PLA_SetFastMute(OSD_PLANE_2, TRUE);
               _OSD_RGN_SetDataAddr(OSD2OriInformation.u4RegionId,OSD2MvcModeCurrentSet.u4LeftFrameAddress);
               OSD_PLA_SetFastMute(OSD_PLANE_2, FALSE);
            }
            if(bOSD3Enable)
            {
               OSD_PLA_SetFastMute(OSD_PLANE_3, TRUE);
               _OSD_RGN_SetDataAddr(OSD3OriInformation.u4RegionId,OSD3MvcModeCurrentSet.u4LeftFrameAddress);
               OSD_PLA_SetFastMute(OSD_PLANE_3, FALSE);
            }
            break;
           case 1:
            if(bOSD2Enable)
            {
                OSD_PLA_SetFastMute(OSD_PLANE_2, TRUE);
                _OSD_RGN_SetDataAddr(OSD2OriInformation.u4RegionId,OSD2MvcModeCurrentSet.u4RightFrameAddress);
                OSD_PLA_SetFastMute(OSD_PLANE_2, FALSE);
            }
            if(bOSD3Enable)
            {
                OSD_PLA_SetFastMute(OSD_PLANE_3, TRUE);
                _OSD_RGN_SetDataAddr(OSD3OriInformation.u4RegionId,OSD3MvcModeCurrentSet.u4RightFrameAddress);
                OSD_PLA_SetFastMute(OSD_PLANE_3, FALSE);
            }
            break;
           default:
            break;
        }
    }
#endif
}
#endif

UINT32 OSD_BASE_GetDisplayWidth(VOID)
{
#ifdef CC_MT5890
    if( fgIsOSDFromGFX())
    {
    #if defined(SUPPORT_PANEL_GRAPH_MODE)
         return GRAPHMODE_GetPanelWidth();
    #endif
    }
#endif

#if 0
    return PANEL_GetPanelWidth();
#else
	return 1920;
#endif
}

UINT32 OSD_BASE_GetDisplayHeight(VOID)
{
#ifdef CC_MT5890
    if(fgIsOSDFromGFX())
    {
    #if defined(SUPPORT_PANEL_GRAPH_MODE)
        return GRAPHMODE_GetPanelHeight();
    #endif
    }
#endif

#if 0
    return PANEL_GetPanelHeight();
#else
	return 1080;
#endif
}
