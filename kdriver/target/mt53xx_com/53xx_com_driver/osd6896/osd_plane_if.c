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
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: osd_plane_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_plane_if.c
 *  osd plane header, incluse osd plane setting
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_hw.h"
#include "osd_drvif.h"
#include "osd_debug.h"
#include "drvcust_if.h"
#include "panel.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_5381.h"
LINT_EXT_HEADER_END
#include "x_timer.h"
#include "drv_tdtv_drvif.h"
#ifndef CC_MTK_LOADER
#include "gfx_if.h"
#endif
#ifdef CC_MTK_LOADER
#define CRIT_STATE_T    UINT32
#ifdef x_crit_start
#undef x_crit_start
#define x_crit_start()  (0)
#endif
#ifdef x_crit_end
#undef x_crit_end
#define x_crit_end(x)   UNUSED(x)
#endif
#endif /* CC_MTK_LOADER */

#ifndef CC_MTK_LOADER
#ifdef SYS_3D_SUPPORT
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#endif
#endif

#ifndef CC_MTK_LOADER
#ifdef CC_CONFIG_OSD_SBS
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "gfx_if.h"
#endif
#endif

#ifndef CC_MTK_LOADER
#ifdef CC_ANDROID_3D_UI_SUPPORT
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "gfx_if.h"
#endif
#endif

#ifdef CC_SUPPORT_4K2K_UI
#ifndef CC_MTK_LOADER
extern UINT32 _u4HWCAddWidth;
extern UINT32 _u4HWCAddHeight;
#endif
#endif
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// delay to avoid temporal status when switching display mode
#define MUTE_VSYNC 10

//define the DRAM access configuration for OSD plane 3
#define OSD3_ContReqLmt 3
#define OSD3_FifoSize 3
#define OSD3_VacTh 3
#define OSD3_PauseCnt 0xf
#define OSD3_ContReqLmt0 1
#define OSD3_BurstDis 0

#ifndef CC_MTK_LOADER
#ifdef SYS_3D_SUPPORT
#define UI_WIDTH    960
#define UI_HEIGHT    540
#define UI_BPP    4
#endif
#endif
#ifndef CC_MTK_LOADER
extern BOOL bLogoDisplay;
#endif
#ifdef OSD_4K2K_SUPPORT
UINT32 u4LinuxUiMode4K2K = OSD_4K2K_MODE_NONE;
UINT32 u4LinuxUiNoneModePanelWidth = 1920 ;
UINT32 u4LinuxUiHDMITime = 1;
BOOL b4K2KOSD2Enable=FALSE;
BOOL b4K2KOSD3Enable=FALSE;
OSD_4K2KOriInformation OSD2OriInformation={0};
OSD_4K2KOriInformation OSD3OriInformation={0};
OSD_4K2KJpecModeCurrentSet OSD2JpecModeCurrentSet={0};
OSD_4K2KJpecModeCurrentSet OSD3JpecModeCurrentSet={0};
OSD_4K2KMvcModeCurrentSet  OSD2MvcModeCurrentSet={0};
OSD_4K2KMvcModeCurrentSet  OSD3MvcModeCurrentSet={0};
BOOL b4K2KSaveInfo = FALSE;
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _OSD_PLANE_INFO_T
{
    UINT32 u4FirstRegionAddr;
    UINT32 u4RightRegionAddr;
    INT32 fgPlaneEnable;
    INT32 fgGlobeEnable;
    INT32 i4FlipedList;
    INT32 i4FirstRegionId;
    INT32 i4RightRegionId;
    UINT32 u4DecEn;
} OSD_PLANE_INFO_T;

typedef struct
{
    const UINT32 u4Plane;
    const UINT32 u4Scaler;
    UINT32 u4RgnList;
    UINT32 u4SCSrcWidth;
    UINT32 u4SCSrcHeight;
    UINT32 u4SCDstWidth;
    UINT32 u4SCDstHeight;
    BOOL fgEnable;
    BOOL fgAvailable;
    BOOL fgPlaCache;
    BOOL fgSCCache;
    BOOL fgEnCache;
}
OSD_PLA_CACHE_IFNO_T;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static OSD_PLANE_INFO_T _arOsdPlaneInfo[OSD_PLANE_MAX_NUM];
static UINT32 _u4MuteCounter = 0;
//static UINT8 _u1FadingCache[OSD_PLANE_MAX_NUM];
//static BOOL _fgInit = FALSE;
//#ifdef CC_MT5363
//static BOOL gfgPLASwap=FALSE;
//#endif

#if 0
//For 3D OSD
UINT8 _u13DMode=0;
UINT8 _u1Previous3DMode=0;
UINT32 _u4_3D_Plane;
UINT32 _u4_3D_Region;
UINT32 _au4_3D_L_Addr;
UINT32 _au4_3D_R_Addr;
//UINT32 _au4_3D_L_Addr[2];
//UINT32 _au4_3D_R_Addr[2];
#endif

#ifndef CC_MTK_LOADER
#ifdef SYS_3D_SUPPORT
static UINT32 _u43DPlane = 0;
static UINT32 _e3DMode = 0;
static UINT32 _u4Shift = 0;
static UINT32 _u4BmpWidth = 0;
static UINT32 _u4BmpHeight = 0;
static UINT32 _u4BmpPitch = 0;
static UINT32 _u4DstAddr = 0;
static UINT32 _eColorMode = 0;
static UINT32 _u4SrcAddr = 0;
static UINT32 _u4TmpBuf = 0;
static UINT32 _u4TmpBuf1 = 0;
#endif
#endif


#if defined(GAL_DDI_SUPPORT)
#ifndef CC_MTK_LOADER
extern UINT32 _u4Posd0Rgn0, _u4Posd0Rgn1;
extern UINT32 _u4Posd1Rgn0, _u4Posd1Rgn1;
#endif
#endif

#ifndef CC_MTK_LOADER
#ifdef CC_SUPPORT_STR
extern BOOL pb_osd_enable[OSD_PLANE_MAX_NUM];
#endif
#endif

OSD_PLA_CACHE_IFNO_T _arPlaneCacheInfo[] = {
    {
        .u4Plane = OSD_PLANE_1,
        .u4Scaler = OSD_SCALER_1,
        .fgAvailable = TRUE,
        .fgPlaCache = FALSE,
        .fgSCCache = FALSE
    },
    {
        .u4Plane = OSD_PLANE_2,
        .u4Scaler = OSD_SCALER_2,
        .fgAvailable = TRUE,
        .fgPlaCache = FALSE,
        .fgSCCache = FALSE
    },
    {
        .u4Plane = OSD_PLANE_3,
        .u4Scaler = OSD_SCALER_3,
        .fgAvailable = TRUE,
        .fgPlaCache = FALSE,
        .fgSCCache = FALSE
    },
    {
        .u4Plane = OSD_PLANE_4,
        .u4Scaler = OSD_SCALER_MAX_NUM,
        .fgAvailable = TRUE,
        .fgPlaCache = FALSE,
        .fgSCCache = FALSE
    },
    {
        .u4Plane = OSD_PLANE_5,
        .u4Scaler = OSD_SCALER_MAX_NUM,
        .fgAvailable = TRUE,
        .fgPlaCache = FALSE,
        .fgSCCache = FALSE
    },
};

VOID OSD_PLA_SetAvailable(UINT32 u4Plane, BOOL fgVailable)
{
    ASSERT((u4Plane < OSD_PLANE_MAX_NUM));

    _arPlaneCacheInfo[u4Plane].fgAvailable = fgVailable;
    
    if(fgVailable == TRUE)
    {
        if(_arPlaneCacheInfo[u4Plane].fgPlaCache == TRUE)
        {
            _arPlaneCacheInfo[u4Plane].fgPlaCache = FALSE;
            OSD_PLA_FlipTo(u4Plane, _arPlaneCacheInfo[u4Plane].u4RgnList);
        }
        
        if(_arPlaneCacheInfo[u4Plane].fgSCCache == TRUE)
        {
            _arPlaneCacheInfo[u4Plane].fgSCCache = FALSE;
            OSD_SC_Scale(_arPlaneCacheInfo[u4Plane].u4Scaler, 1, _arPlaneCacheInfo[u4Plane].u4SCSrcWidth,
                _arPlaneCacheInfo[u4Plane].u4SCSrcHeight, _arPlaneCacheInfo[u4Plane].u4SCDstWidth, _arPlaneCacheInfo[u4Plane].u4SCDstHeight);
        }

        if(_arPlaneCacheInfo[u4Plane].fgEnCache == TRUE)
        {
            _arPlaneCacheInfo[u4Plane].fgEnCache = FALSE;
            OSD_PLA_Enable(u4Plane, _arPlaneCacheInfo[u4Plane].fgEnable);
        }        
    }
}

BOOL OSD_PLA_GetAvailable(UINT32 u4Plane)
{
    ASSERT((u4Plane < OSD_PLANE_MAX_NUM));
    return _arPlaneCacheInfo[u4Plane].fgAvailable;
}


INT32 OSD_PLA_CacheRngList(UINT32 u4Plane, UINT32 u4RgnList)
{
    ASSERT((u4Plane < OSD_PLANE_MAX_NUM));

    if(_arPlaneCacheInfo[u4Plane].fgAvailable == FALSE)
    {
        _arPlaneCacheInfo[u4Plane].u4RgnList = u4RgnList;
        _arPlaneCacheInfo[u4Plane].fgPlaCache = TRUE;

        return 0;
    }

    return -1;
}

INT32 OSD_PLA_CacheScaler(UINT32 u4Scaler, UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4DstWidth, UINT32 u4DstHeight)
{
    UINT32 u4Plane;

    if(u4Scaler == OSD_SCALER_1)
        u4Plane = OSD_PLANE_1;
    else if(u4Scaler == OSD_SCALER_2)
        u4Plane = OSD_PLANE_2;
    else if(u4Scaler == OSD_SCALER_3)
        u4Plane = OSD_PLANE_3;
    else
        return -1;

    if(_arPlaneCacheInfo[u4Plane].fgAvailable == FALSE)
    {
        _arPlaneCacheInfo[u4Plane].u4SCSrcWidth = u4SrcWidth;
        _arPlaneCacheInfo[u4Plane].u4SCSrcHeight = u4SrcHeight;
        _arPlaneCacheInfo[u4Plane].u4SCDstWidth = u4DstWidth;
        _arPlaneCacheInfo[u4Plane].u4SCDstHeight = u4DstHeight;
        _arPlaneCacheInfo[u4Plane].fgSCCache = TRUE;
        return 0;
    }

    return -1;  
}

INT32 OSD_PLA_CacheEnable(UINT32 u4Plane, BOOL fgEnable)
{
    ASSERT((u4Plane < OSD_PLANE_MAX_NUM));

    if(_arPlaneCacheInfo[u4Plane].fgAvailable == FALSE)
    {
        _arPlaneCacheInfo[u4Plane].fgEnable = fgEnable;
        _arPlaneCacheInfo[u4Plane].fgEnCache = TRUE;

        return 0;
    }

    return -1;
}

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

BOOL _OsdPlaReadyToDisplay(UINT32 u4Plane)
{
    return (_arOsdPlaneInfo[u4Plane].fgGlobeEnable
            && _arOsdPlaneInfo[u4Plane].u4FirstRegionAddr
            && _arOsdPlaneInfo[u4Plane].fgPlaneEnable && (OSD_DMA_GetMode()
            == OSD_OP_NORMAL));
}

void _UpdatePlaneEnableState(UINT32 u4Plane)
{
    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void OSD_PLA_Mute(void)
{
#if 0
#ifndef OSD_4K2K_SUPPORT
    UINT32 u4Plane;
    CRIT_STATE_T rCritState;
    if (_fgInit == TRUE)
    {
        if (_u4MuteCounter == 0)
        {
            _OSD_AlwaysUpdateReg(TRUE);
            for (u4Plane = 0; u4Plane < (INT32) OSD_PLANE_MAX_NUM; u4Plane++)
            {
                IGNORE_RET(OSD_PLA_GetFading(u4Plane, &_u1FadingCache[u4Plane]));
                IGNORE_RET(OSD_PLA_SetFading(u4Plane, 0));
                //IGNORE_RET(OSD_PLA_SetGlobeEnable(u4Plane, FALSE));
            }
            _OSD_AlwaysUpdateReg(FALSE);
        }

        rCritState = x_crit_start();
        _u4MuteCounter = MUTE_VSYNC;
        x_crit_end(rCritState);
    }
#endif  
#endif
    LOG(5, "OSD_PLA_Mute\n");
}

void OSD_PLA_Unmute(void)
{
#if 0
#ifndef OSD_4K2K_SUPPORT
    UINT32 u4Plane;
    CRIT_STATE_T rCritState;

    if (_u4MuteCounter > 0)
    {
        if (_u4MuteCounter == 1)
        {
            for (u4Plane = 0; u4Plane < (INT32) OSD_PLANE_MAX_NUM; u4Plane++)
            {
                IGNORE_RET(OSD_PLA_SetFading(u4Plane, _u1FadingCache[u4Plane]));
                //IGNORE_RET(OSD_PLA_SetGlobeEnable(u4Plane, TRUE));
            }
            LOG(5, "OSD_PLA_Unmute\n");
        }
        rCritState = x_crit_start();
        _u4MuteCounter--;
        x_crit_end(rCritState);
    }
#endif  
#endif
}

//-----------------------------------------------------------------------------
/** set osd plane fast mute
 *  @param  u4Plane plane id
 *  @param  u1Mute 
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_SetFastMute(UINT32 u4Plane, UINT32 u4Mute)
{
    OSD_VERIFY_PLANE(u4Plane);

    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetFastMute(u4Plane, u4Mute));
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));

    return (INT32)OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** get osd plane fast mute
 *  @param  u4Plane plane id
 *  @param  u1Mute 
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_GetFastMute(UINT32 u4Plane, UINT32* pu4Mute)
{
    UINT32 u4FastMute=0x0;

    if (pu4Mute == NULL)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }
    
    OSD_VERIFY_PLANE(u4Plane);

    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_GetFastMute(u4Plane, &u4FastMute));
    *pu4Mute = (UINT8) u4FastMute;

    return (INT32) OSD_RET_OK;
}

INT32 OSD_PLA_SetGlobeEnable(UINT32 u4Plane, BOOL fgGlobeEnable)
{
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
#if 0
    if(gfgPLASwap)
    {
        if(u4Plane==OSD_PLANE_2)
        u4Plane=OSD_PLANE_3;
        else if(u4Plane==OSD_PLANE_3)
        u4Plane=OSD_PLANE_2;
    }
#endif
    _arOsdPlaneInfo[u4Plane].fgGlobeEnable = fgGlobeEnable;
    _UpdatePlaneEnableState(u4Plane);

    return (INT32) OSD_RET_OK;
}

void OSD_PLA_Info_Init(void)
{
    UINT32 u4Plane=0x0;
    
    x_memset(&_arOsdPlaneInfo, 0xff, sizeof(_arOsdPlaneInfo));
        
    for(u4Plane = 0; u4Plane < OSD_PLANE_MAX_NUM; ++u4Plane)
    {
        _arOsdPlaneInfo[u4Plane].fgPlaneEnable=0x0;
        _arOsdPlaneInfo[u4Plane].u4DecEn     = 0x0;
    }
    
    return;
}


INT32 OSD_PLA_Init(UINT32 u4Plane)
{
    OSD_VERIFY_PLANE(u4Plane);
    IGNORE_RET(OSD_PLA_Reset(u4Plane));

    return (INT32) OSD_RET_OK;
}

INT32 OSD_PLA_Reset(UINT32 u4Plane)
{
    OSD_VERIFY_PLANE(u4Plane);

    //#ifdef CC_MT5363
#if 0
    if(gfgPLASwap)
    {
        if(u4Plane==OSD_PLANE_2)
        u4Plane=OSD_PLANE_3;
        else if(u4Plane==OSD_PLANE_3)
        u4Plane=OSD_PLANE_2;
    }
#endif

    // reset plane's hardware state
    IGNORE_RET(_OSD_PLA_SetEnable(u4Plane, 0));
    IGNORE_RET(_OSD_PLA_SetReg(u4Plane, NULL));
    IGNORE_RET(_OSD_PLA_SetEnable(u4Plane, 0));
    IGNORE_RET(_OSD_PLA_SetHFilter(u4Plane, 1));
    // IGNORE_RET(_OSD_PLA_SetVFilter(u4Plane, 1));
#if 0 // def ANDROID
    IGNORE_RET(_OSD_PLA_SetBlending(u4Plane, 0xB0));
    IGNORE_RET(_OSD_PLA_SetFading(u4Plane, 0xB0));
#else   
    IGNORE_RET(_OSD_PLA_SetBlending(u4Plane, 0xFF));
    IGNORE_RET(_OSD_PLA_SetFading(u4Plane, 0xFF));
#endif    

    // reset plane's software state
    x_memset((void*) &_arOsdPlaneInfo[u4Plane], 0, sizeof(OSD_PLANE_INFO_T));

    //set i4FirstRegionId to -1 at initial state
    _arOsdPlaneInfo[u4Plane].i4FirstRegionId = (-1);
#ifdef OSD_4K2K_SUPPORT
    OSD2OriInformation.u4RegionId=(-1);
    OSD3OriInformation.u4RegionId=(-1);
#endif

    //#ifdef CC_MT5363
#if 0
    if(gfgPLASwap)
    {
        if(u4Plane==OSD_PLANE_2)
        u4Plane=OSD_PLANE_3;
        else if(u4Plane==OSD_PLANE_3)
        u4Plane=OSD_PLANE_2;
    }
#endif

    IGNORE_RET(OSD_PLA_SetGlobeEnable(u4Plane, (INT32)TRUE));
    // global enable is set to true by default
    //_UpdatePlaneEnableState(u4Plane);

    //_fgInit = TRUE;
    return (INT32) OSD_RET_OK;
}

INT32 OSD_PLA_SwReset(UINT32 u4Plane)
{
    OSD_VERIFY_PLANE(u4Plane);

    IGNORE_RET(_OSD_PLA_SetReg(u4Plane, NULL));
    // IGNORE_RET(_OSD_PLA_SetEnable(u4Plane, 0));
    IGNORE_RET(_OSD_PLA_SetHFilter(u4Plane, 1));
    // IGNORE_RET(_OSD_PLA_SetVFilter(u4Plane, 1));
    IGNORE_RET(_OSD_PLA_SetBlending(u4Plane, 0xff));
    IGNORE_RET(_OSD_PLA_SetFading(u4Plane, 0xff));

    // reset plane's software state
    x_memset((void*) &_arOsdPlaneInfo[u4Plane], 0, sizeof(OSD_PLANE_INFO_T));

    //set i4FirstRegionId to -1 at initial state
    _arOsdPlaneInfo[u4Plane].i4FirstRegionId = (-1);
    _arOsdPlaneInfo[u4Plane].fgGlobeEnable = TRUE;

    //IGNORE_RET(OSD_PLA_SetGlobeEnable(u4Plane, (INT32)TRUE));
    // global enable is set to true by default
    //_UpdatePlaneEnableState(u4Plane);

    //_fgInit = TRUE;
    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** enable/disable osd plane, it will also disable scaler when disable plane
 *  @param  u4Plane plane id
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_Enable(UINT32 u4Plane, BOOL fgEnable)
{
    OSD_VERIFY_PLANE(u4Plane);
    
    if(0 == OSD_PLA_CacheEnable(u4Plane, fgEnable))
    {
        return 0;
    }

    #ifdef OSD_4K2K_SUPPORT
    if(u4Plane==OSD_PLANE_2)
    {
        
        b4K2KOSD2Enable=fgEnable;
    }
    else if(u4Plane == OSD_PLANE_3)
    {
        
        b4K2KOSD3Enable=fgEnable;
    }
    #endif  
    #ifndef CC_MTK_LOADER
    if(bLogoDisplay && (!fgEnable))
    {
        return (INT32) OSD_RET_OK;
    }
    #endif
    if(_arOsdPlaneInfo[u4Plane].i4FirstRegionId == -1||
        _arOsdPlaneInfo[u4Plane].u4FirstRegionAddr == 0)
    {
        fgEnable=FALSE;
    }
    if( _arOsdPlaneInfo[u4Plane].fgPlaneEnable == fgEnable)
    {
        //return (INT32) OSD_RET_OK;
    }

    IGNORE_RET(_OSD_PLA_SetEnable(u4Plane, fgEnable));
        
    _arOsdPlaneInfo[u4Plane].fgPlaneEnable = fgEnable;
    
    _UpdatePlaneEnableState(u4Plane);
    // we suppose no abnormal case when using MJC, so don't care about
    // _UpdatePlaneEnableState now.
#if defined(DRV_SUPPORT_EXTMJC) && !defined(CC_MTK_LOADER)
    if (IS_COMPANION_CHIP_ON() && _OsdPlaReadyToDisplay(u4Plane))
    {
        LOG(5, "drv_extmjc_set_osd_enable, fgEnable=%d\n", fgEnable);
        drv_extmjc_set_osd_turn_on();
    }
#endif
#ifndef CC_MTK_LOADER
#ifdef CC_SUPPORT_STR
    pb_osd_enable[u4Plane] = FALSE;
#endif
#endif
    return (INT32) OSD_RET_OK;
}

INT32 OSD_PLA_Enables(UINT32* pu4Planes, BOOL* pfEnables, UINT32 u4Num)
{
    UINT32 i;

    for(i = 0; i < u4Num; ++i)
    {
        UINT32 u4Plane = pu4Planes[i];
        BOOL fEnable = pfEnables[i];

        OSD_VERIFY_PLANE(u4Plane);
        IGNORE_RET(_OSD_PLA_SetEnable(u4Plane, fEnable));
        _arOsdPlaneInfo[u4Plane].fgPlaneEnable = fEnable;
        _UpdatePlaneEnableState(u4Plane);
#ifndef CC_MTK_LOADER
#ifdef CC_SUPPORT_STR
        pb_osd_enable[u4Num] = FALSE;
#endif
#endif
    }
    return (INT32)OSD_RET_OK;
}
UINT32 OSD_PLA_GetRgnPlane(INT32 i4FirstRegion)
{
    UINT32 ui4_plane = 0x0;

    for (ui4_plane = 0x0; ui4_plane < OSD_PLANE_MAX_NUM; ui4_plane++)
    {
        if (_arOsdPlaneInfo[ui4_plane].i4FirstRegionId == i4FirstRegion)
        {
            break;
        }
    }

    return ui4_plane;
}

//-----------------------------------------------------------------------------
/** attach a specific list to osd plane
 *  @param  u4Plane plane id
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_FlipTo(UINT32 u4Plane, UINT32 u4List)
{
    INT32 i4FirstRegion;
    UINT32 u4FirstRegionAddr;
    static BOOL OSD_OSD2PLS_ENABLE = FALSE;
    BOOL fgOSD2PrLshift_ENABLE=FALSE;
    UINT32 u4OSD3DShiftEn =0;
    UINT32 u4_dec   = 0x0;
    OSD_VERIFY_PLANE(u4Plane);
    OSD_VERIFY_RGNLIST(u4List);
    VERIFY((INT32) OSD_RET_OK == OSD_RGN_LIST_GetHead(u4List, &i4FirstRegion));
    if(_arOsdPlaneInfo[u4Plane].i4FirstRegionId == i4FirstRegion)
    {
        return (INT32) OSD_RET_OK;
    }

    if(0 == OSD_PLA_CacheRngList(u4Plane, u4List))
    {
        return (INT32) OSD_RET_OK;
    }
    
    OSD_LockLogoDisplaying();

    OSD_LockFlipTo();
    
    if ((UINT32)i4FirstRegion >= (UINT32)OSD_MAX_NUM_RGN) 
    { 
        OSD_UnLockFlipTo();
        OSD_UnLockLogoDisplaying();
        return -(INT32)OSD_RET_INV_REGION; 
    } 

    _arOsdPlaneInfo[u4Plane].i4FirstRegionId = i4FirstRegion;
    VERIFY(OSD_RGN_Get(i4FirstRegion,OSD_RGN_DEC_EN,&u4_dec) == OSD_RET_OK);
    if(!u4_dec)
    {
        OSD_BASE_Get3DOsdShiftEn(u4Plane,&u4OSD3DShiftEn);
        if(u4OSD3DShiftEn)
        {
            OSD_RGN_Set((UINT32) i4FirstRegion, (INT32) OSD_RGN_STEP_V, 0x2000);
        }
        else
        {
            OSD_RGN_Set((UINT32) i4FirstRegion, (INT32) OSD_RGN_STEP_V, 0x1000);
        }
    }
    _arOsdPlaneInfo[u4Plane].u4DecEn = u4_dec;
    VERIFY((INT32) OSD_RET_OK == _OSD_RGN_GetAddress((UINT32) i4FirstRegion,
                    &u4FirstRegionAddr));

    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_SetHeaderAddr(u4Plane,
                    u4FirstRegionAddr));

    _arOsdPlaneInfo[u4Plane].u4FirstRegionAddr = u4FirstRegionAddr;
    #if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetClrHdrIrq(u4Plane,0));
    #endif
    //_UpdatePlaneEnableState(u4Plane);
    _OSD_PLA_SetHwReg(u4Plane);
    _arOsdPlaneInfo[u4Plane].i4FlipedList = (INT32) u4List;
    #ifndef CC_MTK_LOADER
    fgOSD2PrLshift_ENABLE = GFX_Get3DFlag();
    #endif
    if (OSD_OSD2PLS_ENABLE != fgOSD2PrLshift_ENABLE)
    {
        _OSD_3D_SetOsd2LShiftEn((UINT32) fgOSD2PrLshift_ENABLE);
        OSD_OSD2PLS_ENABLE = fgOSD2PrLshift_ENABLE;

    }
    
    VERIFY((INT32) OSD_RET_OK == _OSD_3D_UpdateHwReg());
    
    #ifdef OSD_4K2K_SUPPORT
    {
        if(u4Plane==OSD_PLANE_2)
        {   
            if(u4LinuxUiMode4K2K==OSD_4K2K_MODE_MVC)
            {
                OSD_PLA_ReductOriRGNInfo(OSD_PLANE_2,&OSD2OriInformation);
                OSD_PLA_Get4K2KOriRegionInformation(OSD_PLANE_2,&OSD2OriInformation);
                OSD_PLA_GetMVCFrameAddr(OSD_PLANE_2,&OSD2MvcModeCurrentSet);
                OSD_PLA_SetMVCRegionAttri(&OSD2OriInformation); 
            }
        }
        else if(u4Plane==OSD_PLANE_3)
        {
            if(u4LinuxUiMode4K2K==OSD_4K2K_MODE_MVC)
            {
               OSD_PLA_ReductOriRGNInfo(OSD_PLANE_3,&OSD3OriInformation);
               OSD_PLA_Get4K2KOriRegionInformation(OSD_PLANE_3,&OSD3OriInformation);
               OSD_PLA_GetMVCFrameAddr(OSD_PLANE_3,&OSD3MvcModeCurrentSet);
               OSD_PLA_SetMVCRegionAttri(&OSD3OriInformation); 
            }
        }
    }
    #endif  

    OSD_RGN_MirrorFlipUpdate(i4FirstRegion, TRUE, TRUE);
    OSD_UnLockFlipTo();
    OSD_UnLockLogoDisplaying();
#ifndef CC_MTK_LOADER
#ifdef CC_SUPPORT_STR
    if(pb_osd_enable[u4Plane] == TRUE)
    {
        OSD_PLA_Enable(u4Plane, TRUE);
    }
#endif
#endif
    return (INT32) OSD_RET_OK;
}


INT32 OSD_PLA_Get_En(UINT32 u4Plane)
{
    return _arOsdPlaneInfo[u4Plane].fgPlaneEnable;
}

UINT32 OSD_PLA_Get_Dec(UINT32 u4Plane)
{
    return _arOsdPlaneInfo[u4Plane].u4DecEn;
}

INT32 OSD_PLA_FlipToNone(UINT32 u4Plane)
{
    OSD_VERIFY_PLANE(u4Plane);

    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_SetHeaderAddr(u4Plane, 0));
    _arOsdPlaneInfo[u4Plane].u4FirstRegionAddr = 0;
    _arOsdPlaneInfo[u4Plane].i4FirstRegionId = -1;

    _UpdatePlaneEnableState(u4Plane);

    return (INT32) OSD_RET_OK;
}

INT32 OSD_PLA_Flip_RgnList(UINT32 u4Plane, UINT32 u4List)
{
    OSD_VERIFY_PLANE(u4Plane);
    OSD_VERIFY_RGNLIST(u4List);
    
    // VERIFY((INT32) OSD_RET_OK == OSD_SC_RGNLIST_Set(u4Plane,u4List,TRUE));
    VERIFY((INT32) OSD_RET_OK == OSD_PLA_FlipTo(u4Plane, u4List));
    // VERIFY((INT32) OSD_RET_OK == OSD_PLA_Enable(u4Plane,TRUE));
    
    return (INT32) OSD_RET_OK;
}

INT32 OSD_PLA_Reflip(UINT32 u4List)
{
    INT32 i4Index;
    UINT32 u4Value;

    for (i4Index = 0; i4Index < (INT32) OSD_PLANE_MAX_NUM; i4Index++)
    {
        if (u4List == (UINT32) _arOsdPlaneInfo[i4Index].i4FlipedList)
        {
            IGNORE_RET(_OSD_PLA_GetEnable((UINT32)i4Index, &u4Value));
            if (u4Value)
            {
                IGNORE_RET(OSD_PLA_FlipTo((UINT32)i4Index, u4List));
            }
        }
    }

    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** get osd plane alpha level
 *  @param  u4Plane plane id
 *  @param  pu1BlendLevel aplha level pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_GetBlendLevel(UINT32 u4Plane, UINT8* pu1BlendLevel)
{
    UINT32 u4Blending;

    if (pu1BlendLevel == NULL)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
#if 0
    if(gfgPLASwap)
    {
        if(u4Plane==OSD_PLANE_2)
        u4Plane=OSD_PLANE_3;
        else if(u4Plane==OSD_PLANE_3)
        u4Plane=OSD_PLANE_2;
    }
#endif
    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_GetBlending(u4Plane, &u4Blending));
    *pu1BlendLevel = (UINT8) u4Blending;

    return (INT32) OSD_RET_OK;
}

INT32 OSD_PLA_GetEnable(UINT32 u4Plane, BOOL* pfgEnable)
{
    UINT32 u4Enable;

    if (pfgEnable == NULL)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
#if 0
    if(gfgPLASwap)
    {
        if(u4Plane==OSD_PLANE_2)
        u4Plane=OSD_PLANE_3;
        else if(u4Plane==OSD_PLANE_3)
        u4Plane=OSD_PLANE_2;
    }
#endif
    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_GetEnable(u4Plane, &u4Enable));
#ifndef CC_MTK_LOADER
#ifdef CC_SUPPORT_STR
    if(u4Enable == FALSE)
    {
        u4Enable = pb_osd_enable[u4Plane];
    }
#endif
#endif

    *pfgEnable = (BOOL) u4Enable;

    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** set osd plane alpha level
 *  @param  u4Plane plane id
 *  @param  u1BlendLevel aplha level(0~255)
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_SetBlendLevel(UINT32 u4Plane, UINT8 u1BlendLevel)
{
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
#if 0
    if(gfgPLASwap)
    {
        if(u4Plane==OSD_PLANE_2)
        u4Plane=OSD_PLANE_3;
        else if(u4Plane==OSD_PLANE_3)
        u4Plane=OSD_PLANE_2;
    }
#endif
    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_SetBlending(u4Plane, u1BlendLevel));
    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** get osd plane fading value
 *  @param  u4Plane plane id
 *  @param  pu1Fading fading value pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_GetFading(UINT32 u4Plane, UINT8* pu1Fading)
{
    UINT32 u4Fading;

    if (pu1Fading == NULL)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }
    OSD_VERIFY_PLANE(u4Plane);
    //#ifdef CC_MT5363
#if 0
    if(gfgPLASwap)
    {
        if(u4Plane==OSD_PLANE_2)
        u4Plane=OSD_PLANE_3;
        else if(u4Plane==OSD_PLANE_3)
        u4Plane=OSD_PLANE_2;
    }
#endif
    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_GetFading(u4Plane, &u4Fading));
    *pu1Fading = (UINT8) u4Fading;

    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** set osd plane fading value
 *  @param  u4Plane plane id
 *  @param  u1Fading fading value
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_SetFading(UINT32 u4Plane, UINT8 u1Fading)
{
    OSD_VERIFY_PLANE(u4Plane);
    
    if (_u4MuteCounter > 1)
    {
        //_u1FadingCache[u4Plane] = u1Fading;
    }
    else
    {
        VERIFY((INT32) OSD_RET_OK == _OSD_PLA_SetFading(u4Plane, u1Fading));
        VERIFY((INT32) OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
    }
    return (INT32) OSD_RET_OK;
}

INT32 OSD_PLA_SetHFilter(UINT32 u4Plane, BOOL fgEnable)
{
    OSD_VERIFY_PLANE(u4Plane);
    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_SetHFilter(u4Plane, fgEnable));
    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
    return (INT32) OSD_RET_OK;
}

INT32 OSD_PLA_SetVFilter(UINT32 u4Plane, BOOL fgEnable)
{
    /*
     OSD_VERIFY_PLANE(u4Plane);
     //VERIFY((INT32)OSD_RET_OK == _OSD_PLA_SetVFilter(u4Plane, fgEnable));
     VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
     */
    return (INT32) OSD_RET_OK;
}

INT32 OSD_PLA_SetFifo(UINT32 u4Plane, BOOL fgFastReq, UINT8 u1ExVacThr,
        UINT8 u1VacThr, UINT8 u1FullThr)
{
    UNUSED(u4Plane);
    UNUSED(fgFastReq);
    UNUSED(u1ExVacThr);
    UNUSED(u1VacThr);
    UNUSED(u1FullThr);
    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** dump osd debug info
 *  @param  u4Plane osd plane
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_Dump(UINT32 u4Plane)
{
    OSD_PLA_UNION_T* prOsdPlaneReg=NULL;
    UINT32 u4Region, u4List;
    INT32 i4Count = 0;

    OSD_VERIFY_PLANE(u4Plane);
    
    // dump plane
    prOsdPlaneReg=(OSD_PLA_UNION_T *)_OSD_PLA_Get_HWReg(u4Plane);
    if(!prOsdPlaneReg)
    {
        return (INT32) OSD_RET_OK;
    }
    
    Printf("\n");
    Printf("OSD PLANE[ %d ]\n", u4Plane + 1);
    Printf("\tEnable   =%-2d FirstRegion=0x%-8x HStart  =%-4d VStart=%-4d\n", 
            prOsdPlaneReg->rField.fgEn,
            prOsdPlaneReg->rField.u4HeaderAddr<<4,
            OSD_PLA_GetHStart(u4Plane),
            OSD_PLA_GetVStart(u4Plane));
    
    if (!prOsdPlaneReg->rField.u4HeaderAddr)
    {
        return (INT32) OSD_RET_OK;
    }
        
    Printf("\t3DShutter=%-2d RightRegion=0x%-8x FastMute=%-4d Fading=%-4d\n", 
        prOsdPlaneReg->rField.fgShutter,
        prOsdPlaneReg->rField.u4RightHeaderAddr<<4,
        prOsdPlaneReg->rField.fgFastMute,
        prOsdPlaneReg->rField.u4Fading);

    #ifndef CC_MTK_LOADER
    #ifdef LINUX_TURNKEY_SOLUTION
    Printf("\tBufferMode[%d](0:unknown 1:frontonly 2:backvideo 4:backsys 8:triple 16:windows)\n", 
        fb_osd_get_buffer_mode(u4Plane));
    #endif
    #endif
    
    // dump scaler
    if(prOsdPlaneReg->rField.fgEn && (u4Plane <= (UINT32) OSD_PLANE_3))
    {
        IGNORE_RET(OSD_SC_Dump(OSD_PLANE_TO_SCALER(u4Plane)));
    }

    // dump region
    if(_arOsdPlaneInfo[u4Plane].u4FirstRegionAddr != (UINT32) NULL)
    {
        u4List = (UINT32) _arOsdPlaneInfo[u4Plane].i4FlipedList;
        IGNORE_RET(OSD_RGN_LIST_Get(u4List, (INT32)OSD_RGN_LIST_COUNT,(UINT32*)&i4Count));
        IGNORE_RET(OSD_RGN_LIST_Get(u4List, (INT32)OSD_RGN_LIST_HEAD,&u4Region));
        Printf("REGION List[ %d ]\n", u4List);
        while (i4Count--)
        {
            UINT32 u4Next;
            IGNORE_RET(OSD_RGN_Dump(u4Region));
            IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_NEXT, &u4Next));
            u4Region = u4Next;
        }
       
        if(prOsdPlaneReg->rField.u4RightHeaderAddr)
        {
            _OSD_RGN_GetRegionId((prOsdPlaneReg->rField.u4RightHeaderAddr<<4),&u4Region);
            Printf("3D Shutter Right Region[ %d ]:\n",u4Region);
            IGNORE_RET(OSD_RGN_Dump(u4Region));
        }
    }
    
    Printf("\n");

    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** disable loader logo
 *  @param  u4Plane osd plane
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------

INT32 OSD_PLA_DisableLoaderLogo(UINT32 u4Plane)
{
    INT32 i4List;
    OSD_PLA_UNION_T* prOsdPlaneReg=NULL;

    OSD_VERIFY_PLANE(u4Plane);

    prOsdPlaneReg=(OSD_PLA_UNION_T *)_OSD_PLA_Get_HWReg(u4Plane);
    if(!prOsdPlaneReg || !prOsdPlaneReg->rField.u4HeaderAddr)
    {
        return (INT32) OSD_RET_OK;
    }

    i4List = _arOsdPlaneInfo[u4Plane].i4FlipedList;

    if(prOsdPlaneReg->rField.fgEn && i4List < 0) //the plane is showing loader logo
    {   
        OSD_PLA_Reset(u4Plane);
        OSD_BASE_SetOsdPosition(u4Plane, 0, 0);
        //OSD_WaitVsync();
        //OSD_WaitVsync();
    }

    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** get the first region id of osd plane
 *  @param  u4Plane osd plane
 *  @retval region id, -1 is no region
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_GetFirstRegion(UINT32 u4Plane)
{
    OSD_VERIFY_PLANE(u4Plane);
    return _arOsdPlaneInfo[u4Plane].i4FirstRegionId;
}

//-----------------------------------------------------------------------------
/** get the first region id of osd plane
 *  @param  u4Plane osd plane
 *  @retval region id, -1 is no region
 */
//-----------------------------------------------------------------------------
INT32 OSD_PLA_GetFirstRegionList(UINT32 u4Plane)
{
    OSD_VERIFY_PLANE(u4Plane);

    //#ifdef CC_MT5363
#if 0
    if(gfgPLASwap)
    {
        if(u4Plane==OSD_PLANE_2)
        u4Plane=OSD_PLANE_3;
        else if(u4Plane==OSD_PLANE_3)
        u4Plane=OSD_PLANE_2;
    }
#endif
    return _arOsdPlaneInfo[u4Plane].i4FlipedList;
}

//-----------------------------------------------------------------------------
/** get the first region address of osd plane
 *  @param  u4Plane osd plane
 *  @retval region address, 0 is no region
 */
//-----------------------------------------------------------------------------
UINT32 OSD_PLA_GetFirstRegionAddress(UINT32 u4Plane)
{
    if (u4Plane >= (UINT32) OSD_PLANE_MAX_NUM)
    {
        return 0;
    }
    return _arOsdPlaneInfo[u4Plane].u4FirstRegionAddr;
}



//-----------------------------------------------------------------------------
/** wait OSD work stable
 *  @param  u4Plane osd plane
 *  @retval don't care
 */
//-----------------------------------------------------------------------------
void OSD_PLA_WaitStable(UINT32 u4_plane)
{
    UINT32 u4_bmp_addr  = 0x0;
    UINT32 u4_rgn;
    if(FALSE == OSD_PLA_GetAvailable(u4_plane))
    {
        return;
    }
    u4_rgn = OSD_PLA_GetFirstRegion(u4_plane);

    if( u4_rgn >= OSD_MAX_NUM_RGN )
    {
        return;
    }
    
    VERIFY((INT32) OSD_RET_OK == _OSD_RGN_GetDataAddr(u4_rgn, &u4_bmp_addr));
    OSD_RGN_Set_Bmp_Addr_Wait(u4_plane, u4_bmp_addr);

    return;
}

void OSD_PLA_SetWorkaround(UINT32 u4Plane, BOOL fgEnable)
{
}

void OSD_PLA_GetCompressMode(UINT32 u4Plane, UINT32 *pu4DecEnable,
        UINT32 *pu4AlphaMode)
{
    //IGNORE_RET(_OSD_PLA_GetDecEn(u4Plane, pu4DecEnable));
    //IGNORE_RET(_OSD_PLA_GetAlphaMod(u4Plane, pu4AlphaMode));
}

void OSD_PLA_SetCompressMode(UINT32 u4Plane, UINT32 u4DecEnable,
        UINT32 u4AlphaMode)
{
    /*
     UINT32 u4Region, u4List, u4Tmp;
     INT32 i4Count;

     // plane1 doesn't support compress mode
     if (u4Plane == (UINT32)OSD_PLANE_1)
     {
     return;
     }

     // set alpha mode no matter what compression mode it is
     //IGNORE_RET(_OSD_PLA_SetAlphaMod(u4Plane, u4AlphaMode));

     // check previous status
     //IGNORE_RET(_OSD_PLA_GetDecEn(u4Plane, &u4Tmp));
     if (u4Tmp != u4DecEnable)
     {
     if (_arOsdPlaneInfo[u4Plane].u4FirstRegionAddr != NULL)
     {
     u4List = (UINT32)_arOsdPlaneInfo[u4Plane].i4FlipedList;
     IGNORE_RET(OSD_RGN_LIST_Get(u4List, (INT32)OSD_RGN_LIST_COUNT,
     (UINT32*)&i4Count));
     IGNORE_RET(OSD_RGN_LIST_Get(u4List, (INT32)OSD_RGN_LIST_HEAD,
     &u4Region));

     while (i4Count--)
     {
     IGNORE_RET(OSD_RGN_Get(u4Region, OSD_RGN_BMP_PITCH, &u4Tmp));
     if (u4DecEnable)
     {
     // if enable, half the line size, and set little endian
     IGNORE_RET(OSD_RGN_Set(u4Region, OSD_RGN_BMP_PITCH, u4Tmp >> 1));
     }
     else
     {
     // restore line size
     IGNORE_RET(OSD_RGN_Set(u4Region, OSD_RGN_BMP_PITCH, u4Tmp << 1));
     }
     IGNORE_RET(OSD_RGN_SetBigEndian(u4Region, 0));
     IGNORE_RET(OSD_RGN_Get(u4Region, (INT32)OSD_RGN_NEXT, &u4Region));
     }
     }
     //IGNORE_RET(_OSD_PLA_SetDecEn(u4Plane, u4DecEnable));
     }

     VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
     */
}

//#ifdef CC_MT5363
#if 0
INT32 OSD_PLA_Swap(BOOL fgSwap)
{
    OSD_PLA_UNION_T rOsdPlaneRegTmp1,rOsdPlaneRegTmp2;
    OSD_PLANE_INFO_T rPlaneInfoTmp;

    if(gfgPLASwap==fgSwap)
    return (INT32)OSD_RET_OK;;

    gfgPLASwap=fgSwap;

    _OSD_PLA_GetReg(OSD_PLANE_2, rOsdPlaneRegTmp1.au4Reg); //get Plane2 reg to tmp1
    _OSD_PLA_GetReg(OSD_PLANE_3, rOsdPlaneRegTmp2.au4Reg); //get Planer3 reg to tmp2
    _OSD_PLA_SetReg(OSD_PLANE_2, rOsdPlaneRegTmp2.au4Reg); //Set Plane2 reg with tmp2(old scaler3)
    _OSD_PLA_SetReg(OSD_PLANE_3, rOsdPlaneRegTmp1.au4Reg); //Set Plane2 reg with tmp1(old scaler2)

    rPlaneInfoTmp=_arOsdPlaneInfo[OSD_PLANE_2];
    _arOsdPlaneInfo[OSD_PLANE_2]=_arOsdPlaneInfo[OSD_PLANE_3];
    _arOsdPlaneInfo[OSD_PLANE_3]=rPlaneInfoTmp;

    _OSD_PLA_UpdateHwReg(OSD_PLANE_2);
    _OSD_PLA_UpdateHwReg(OSD_PLANE_3);

    return (INT32)OSD_RET_OK;
}
#endif

//Add for Set DRAM parameters       
void OSD_PLA_SetContReqLmt(UINT32 u4Plane, UINT32 u4Value)
{
    IGNORE_RET(_OSD_PLA_SetContReqLmt(u4Plane, u4Value));
}
void OSD_PLA_SetFifoSize(UINT32 u4Plane, UINT32 u4Value)
{
    IGNORE_RET(_OSD_PLA_SetFifoSize(u4Plane, u4Value));
}
void OSD_PLA_SetVacTh(UINT32 u4Plane, UINT32 u4Value)
{
    IGNORE_RET(_OSD_PLA_SetVacTh(u4Plane, u4Value));
}
void OSD_PLA_SetPauseCnt(UINT32 u4Plane, UINT32 u4Value)
{
    IGNORE_RET(_OSD_PLA_SetPauseCnt(u4Plane, u4Value));
}
void OSD_PLA_SetContReqLmt0(UINT32 u4Plane, UINT32 u4Value)
{
    IGNORE_RET(_OSD_PLA_SetContReqLmt0(u4Plane, u4Value));
}

#if 0
//For 3D OSD
INT32 OSD_PLA_Set3DAddress(UINT32 u4Plane, UINT32 u4L_Address, UINT32 u4R_Address)
{
    if( (u4Plane!=OSD_PLANE_2)&&(u4Plane!=OSD_PLANE_3))
    {
        return -(INT32)OSD_RET_INV_PLANE;
    }
    _u4_3D_Plane = u4Plane;
    _au4_3D_L_Addr=u4L_Address;
    _au4_3D_R_Addr=u4R_Address;
    //_au4_3D_L_Addr[u4Plane-1]=u4L_Address;
    //_au4_3D_R_Addr[u4Plane-1]=u4R_Address;
    _u4_3D_Region = _arOsdPlaneInfo[u4Plane].i4FirstRegionId;

    return (INT32)OSD_RET_OK;
}

INT32 OSD_Vsync_3DUpdateLR(UINT32 u4L_R)
{
    if(_u13DMode==1) // 3D mode

    {
        if(u4L_R==0) // L
        //OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_L_Addr[_u4_3D_Plane]);
        OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_L_Addr);
        else
        //OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_R_Addr[_u4_3D_Plane]);
        OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_R_Addr);
    }
    else
    {
        if(_u1Previous3DMode !=_u13DMode)
        //OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_L_Addr[_u4_3D_Plane]);
        OSD_RGN_Set(_u4_3D_Region,OSD_RGN_BMP_ADDR,_au4_3D_L_Addr);
    }

    _u1Previous3DMode =_u13DMode;
    return (INT32)OSD_RET_OK;
}

INT32 OSD_Set3DMode(UINT8 u1Mode)
{
    _u13DMode=u1Mode;
    return (INT32)OSD_RET_OK;
}
#endif

INT32 OSD_PLA_Set3DMode(UINT32 u4Plane, UINT8 u1Mode)
{
    if ((u4Plane != OSD_PLANE_2) && (u4Plane != OSD_PLANE_3))
    {
        return -(INT32) OSD_RET_INV_PLANE;
    }

    _OSD_PLA_SetShutter(u4Plane, u1Mode);

    _OSD_3D_SetTgenLREn(u1Mode);
    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
    VERIFY((INT32) OSD_RET_OK == _OSD_3D_UpdateHwReg());

    return (INT32) OSD_RET_OK;
}

INT32 OSD_PLA_Set3DRegion(UINT32 u4Plane, UINT32 u4LeftRgn, UINT32 u4RightRgn)
{
    UINT32 u4u4LeftRgnAddr;
    UINT32 u4u4RightRgnAddr;
    UINT32 u4RealHeadAddr = 0x0;

    if ((u4Plane != OSD_PLANE_2) && (u4Plane != OSD_PLANE_3))
    {
        return -(INT32) OSD_RET_INV_PLANE;
    }

    OSD_VERIFY_REGION(u4LeftRgn);

    _arOsdPlaneInfo[u4Plane].i4FirstRegionId = u4LeftRgn;
    _arOsdPlaneInfo[u4Plane].i4RightRegionId = u4RightRgn;

    VERIFY(
            (INT32) OSD_RET_OK == _OSD_RGN_GetAddress((UINT32) u4LeftRgn,
                    &u4u4LeftRgnAddr));

    VERIFY(
            (INT32) OSD_RET_OK == _OSD_PLA_SetHeaderAddr(u4Plane,
                    u4u4LeftRgnAddr));

    VERIFY(
            (INT32) OSD_RET_OK == _OSD_RGN_GetAddress((UINT32) u4RightRgn,
                    &u4u4RightRgnAddr));

    VERIFY(
            (INT32) OSD_RET_OK == _OSD_PLA_SetRightHeaderAddr(u4Plane,
                    u4u4RightRgnAddr));

    //printf("u4u4RightRgnAddr = 0x%x\n",u4u4RightRgnAddr);

    _arOsdPlaneInfo[u4Plane].u4FirstRegionAddr = u4u4LeftRgnAddr;
    _arOsdPlaneInfo[u4Plane].u4RightRegionAddr = u4u4RightRgnAddr;

    VERIFY((INT32) OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4Plane));
    OSD_WaitVsync();

    _OSD_PLA_Get_Hw_RealHeaderAddr(u4Plane, &u4RealHeadAddr);

    if ((u4RealHeadAddr != u4u4LeftRgnAddr) && (u4RealHeadAddr
            != u4u4RightRgnAddr))
    {
        OSD_WaitVsync();
    }

    return (INT32) OSD_RET_OK;
}
INT32 OSD_PLA_GetAllRegion(UINT32 u4Plane, UINT32* u4Rgn1, UINT32* u4Rgn2)
{
#if 0
#ifndef CC_MTK_LOADER
    OSD_VERIFY_PLANE(u4Plane);
#ifdef OSD_DSC_PDP
    if(OSD_BASE_GetDisplayWidth() == 1024)
    {
        *u4Rgn1 = (UINT32) fb1_rgn_pdp;
        *u4Rgn2 =(UINT32) fb1_rgn_pdp_2;
    }
    else
#endif
    {
        *u4Rgn1 = (UINT32) fb1_rgn;
        *u4Rgn2 =(UINT32) fb1_rgn_2;
    }

    return (INT32)OSD_RET_OK;
#else
    return (INT32)OSD_RET_OK;
#endif
#else
#if defined(GAL_DDI_SUPPORT)
#ifndef CC_MTK_LOADER
    OSD_VERIFY_PLANE(u4Plane);
 if(u4Plane == OSD_PLANE_2)
     {
       *u4Rgn1 = _u4Posd0Rgn0;
     *u4Rgn2 = _u4Posd0Rgn1;
    }
 else if(u4Plane == OSD_PLANE_3)
     {
       *u4Rgn1 = _u4Posd1Rgn0;
     *u4Rgn2 = _u4Posd1Rgn1;
    }
#endif 
#endif
    return (INT32) OSD_RET_OK;

#endif
}

INT32 OSD_PLA_GetSrcContentRegion(UINT32 u4Plane, UINT32* u4Rgn1,
        UINT32* u4Rgn2)
{
#if 0

#ifndef CC_MTK_LOADER
    OSD_VERIFY_PLANE(u4Plane);
    *u4Rgn1 = (UINT32)fb1_rgn;
    *u4Rgn2 = (UINT32)fb1_rgn_2;
    return (INT32)OSD_RET_OK;
#else
    return (INT32)OSD_RET_OK;
#endif
#else
    return (INT32) OSD_RET_OK;

#endif
}

#ifdef CC_GPU_SUPPORT
extern UINT32 fb1_width, fb1_height;
extern UINT32 fb2_width, fb2_height;
#endif

INT32 OSD_PLA_GetSrcContentSize(UINT32 u4Plane, UINT32 *pu4Width,
        UINT32 *pu4Height)
{
#ifdef CC_GPU_SUPPORT
#ifndef CC_MTK_LOADER
    OSD_VERIFY_PLANE(u4Plane);
    if(u4Plane == OSD_PLANE_3 /*2*/)
    {
        *pu4Width = (UINT32)fb1_width;
        *pu4Height = (UINT32)fb1_height;
    }
    else if(u4Plane == OSD_PLANE_2 /*1*/)
    {
        *pu4Width = (UINT32)fb2_width;
        *pu4Height = (UINT32)fb2_height;
    }
    else
    {
        return -(INT32)OSD_RET_INV_ARG;
    }
    return (INT32)OSD_RET_OK;
#else
    return (INT32)OSD_RET_OK;
#endif
#endif

    return (INT32) OSD_RET_OK;

}

#ifndef CC_MTK_LOADER
#ifdef SYS_3D_SUPPORT
INT32 OSD_PLA_Set3DFlip(UINT32 u4Plane, OSD_3D_MODE_T e3DMode, UINT32 u4Shift,
        UINT32 u4BmpWidth, UINT32 u4BmpHeight, UINT32 u4BmpPitch,
        UINT32 u4BmpAddr, UINT32 eColorMode)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    E_RZ_INOUT_TYPE_T eRzType;

    _u43DPlane = u4Plane;
    _e3DMode = e3DMode;
    _u4Shift = u4Shift;
    _u4BmpWidth = u4BmpWidth;
    _u4BmpHeight = u4BmpHeight;
    _u4BmpPitch = u4BmpPitch;
    _u4DstAddr = u4BmpAddr;
    _eColorMode = eColorMode;

    if (_u4TmpBuf1 == 0)
    {
        _u4TmpBuf1 =
        (UINT32)BSP_AllocAlignedDmaMemory((UI_WIDTH*UI_HEIGHT*UI_BPP), (OSD_DMA_ALIGN_SIZE + 1));

        LOG(5,"TmpBuf1 = 0x%x\n", _u4TmpBuf1);

        _u4SrcAddr = _u4TmpBuf1;
    }

    GFX_Lock();
    GFX_SetSrc((UINT8 *)u4BmpAddr, eColorMode, u4BmpPitch);
    GFX_SetDst((UINT8 *)_u4TmpBuf1, eColorMode, u4BmpPitch);
    GFX_BitBlt(0, 0, 0, 0, u4BmpWidth, u4BmpHeight);

    GFX_Flush();
    GFX_Wait();
    GFX_Unlock();

    x_memset(&rSclParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));

    if (e3DMode == OSD_3D_SBSHALF)
    {
        rSclParam.u4TgW = u4BmpWidth >> 1;
        rSclParam.u4TgH = u4BmpHeight;
    }
    else if (e3DMode == OSD_3D_TPANDBT)
    {
        rSclParam.u4TgW = u4BmpWidth;
        rSclParam.u4TgH = u4BmpHeight >> 1;
    }

    rSclParam.u4SrcBufLen = u4BmpPitch;
    rSclParam.u4SrcW = u4BmpWidth;
    rSclParam.u4SrcH = u4BmpHeight;

    rSclParam.u4SrcBase = u4BmpAddr;
    rSclParam.u4IsSrcDirCM = 1;
    rSclParam.u4SrcCM = (eColorMode == OSD_CM_ARGB4444_DIRECT16)?
    (UINT32) E_RZ_OSD_DIR_CM_ARGB4444:(UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
    rSclParam.u4SrcHOffset = 0;
    rSclParam.u4SrcVOffset = 0;

    rSclParam.u4TgBase = u4BmpAddr;

    rSclParam.u4TgHOffset = 0;
    rSclParam.u4TgVOffset = 0;

    rSclParam.u4TgCM = rSclParam.u4SrcCM;
    rSclParam.u4TgBufLen = u4BmpPitch;

    eRzType = E_RZ_INOUT_TYPE_OSDMD;

    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(eRzType);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

    GFX_Lock();
    GFX_SetSrc((UINT8 *)u4BmpAddr, eColorMode, u4BmpPitch);
    GFX_SetDst((UINT8 *)u4BmpAddr, eColorMode, u4BmpPitch);
    GFX_SetColor(0x0);
    GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0x00000000, 0xffffffff);

    if (e3DMode == OSD_3D_SBSHALF)
    {
        GFX_Fill(rSclParam.u4TgW, 0, rSclParam.u4TgW, rSclParam.u4TgH);
        GFX_BitBlt((u4Shift >> 1), 0, rSclParam.u4TgW, 0, (rSclParam.u4TgW - (u4Shift >> 1)), rSclParam.u4TgH);
    }
    else if (e3DMode == OSD_3D_TPANDBT)
    {
        GFX_Fill(0, rSclParam.u4TgH, rSclParam.u4TgW, rSclParam.u4TgH);
        GFX_BitBlt(u4Shift, 0, 0, rSclParam.u4TgH, (rSclParam.u4TgW - u4Shift), rSclParam.u4TgH);
    }

    GFX_Flush();
    GFX_Wait();
    GFX_Unlock();

    return (INT32)OSD_RET_OK;
}

INT32 OSD_PLA_Set3DBlit(UINT32 u4Plane, OSD_3D_MODE_T e3DMode, UINT32 u4Shift,
        UINT32 u4BmpWidth, UINT32 u4BmpHeight, UINT32 u4BmpPitch,
        UINT32 u4DstAddr, UINT32 eColorMode, UINT32 u4SrcAddr)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    E_RZ_INOUT_TYPE_T eRzType;

    _u43DPlane = u4Plane;
    _e3DMode = e3DMode;
    _u4Shift = u4Shift;
    _u4BmpWidth = u4BmpWidth;
    _u4BmpHeight = u4BmpHeight;
    _u4BmpPitch = u4BmpPitch;
    _u4SrcAddr = u4SrcAddr;
    _u4DstAddr = u4DstAddr;
    _eColorMode = eColorMode;

    if (_u4TmpBuf == 0)
    {
        _u4TmpBuf =
        (UINT32)BSP_AllocAlignedDmaMemory((UI_WIDTH*UI_HEIGHT*UI_BPP), (OSD_DMA_ALIGN_SIZE + 1));

        LOG(5,"TmpBuf = 0x%x\n", _u4TmpBuf);
    }

    x_memset(&rSclParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));

    if (e3DMode == OSD_3D_SBSHALF)
    {
        rSclParam.u4TgW = u4BmpWidth >> 1;
        rSclParam.u4TgH = u4BmpHeight;
    }
    else if (e3DMode == OSD_3D_TPANDBT)
    {
        rSclParam.u4TgW = u4BmpWidth;
        rSclParam.u4TgH = u4BmpHeight >> 1;
    }

    rSclParam.u4SrcBufLen = u4BmpPitch;
    rSclParam.u4SrcW = u4BmpWidth;
    rSclParam.u4SrcH = u4BmpHeight;

    rSclParam.u4SrcBase = u4SrcAddr;
    rSclParam.u4IsSrcDirCM = 1;
    rSclParam.u4SrcCM = (eColorMode == OSD_CM_ARGB4444_DIRECT16)?
    (UINT32) E_RZ_OSD_DIR_CM_ARGB4444:(UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
    rSclParam.u4SrcHOffset = 0;
    rSclParam.u4SrcVOffset = 0;

    rSclParam.u4TgBase = _u4TmpBuf;

    rSclParam.u4TgHOffset = 0;
    rSclParam.u4TgVOffset = 0;

    rSclParam.u4TgCM = rSclParam.u4SrcCM;
    rSclParam.u4TgBufLen = u4BmpPitch;

    eRzType = E_RZ_INOUT_TYPE_OSDMD;

    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(eRzType);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

    GFX_Lock();
    GFX_SetSrc((UINT8 *)_u4TmpBuf, eColorMode, u4BmpPitch);

    GFX_SetDst((UINT8 *)_u4TmpBuf, eColorMode, u4BmpPitch);
    GFX_SetColor(0x0);
    GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0x00000000, 0xffffffff);

    if (e3DMode == OSD_3D_SBSHALF)
    {
        GFX_Fill(rSclParam.u4TgW, 0, rSclParam.u4TgW, rSclParam.u4TgH);
        GFX_BitBlt((u4Shift >> 1), 0, rSclParam.u4TgW, 0, (rSclParam.u4TgW - (u4Shift >> 1)), rSclParam.u4TgH);
    }
    else if (e3DMode == OSD_3D_TPANDBT)
    {
        GFX_Fill(0, rSclParam.u4TgH, rSclParam.u4TgW, rSclParam.u4TgH);
        GFX_BitBlt(u4Shift, 0, 0, rSclParam.u4TgH, (rSclParam.u4TgW - u4Shift), rSclParam.u4TgH);
    }

    GFX_SetSrc((UINT8 *)_u4TmpBuf, eColorMode, u4BmpPitch);
    GFX_SetDst((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);
    GFX_BitBlt(0, 0, 0, 0, u4BmpWidth, u4BmpHeight);

    GFX_Flush();
    GFX_Wait();
    GFX_Unlock();

    return (INT32)OSD_RET_OK;

}

INT32 OSD_PLA_Redraw3D(OSD_3D_MODE_T e3DMode)
{

    if ((_u4DstAddr != 0) && (e3DMode != _e3DMode))
    {
        if (e3DMode == OSD_3D_NONE)
        {
            GFX_Lock();
            GFX_SetSrc((UINT8 *)_u4SrcAddr, _eColorMode, _u4BmpPitch);
            GFX_SetDst((UINT8 *)_u4DstAddr, _eColorMode, _u4BmpPitch);
            GFX_BitBlt(0, 0, 0, 0, _u4BmpWidth, _u4BmpHeight);

            GFX_Flush();
            GFX_Wait();
            GFX_Unlock();
        }
        else
        {
            OSD_PLA_Set3DBlit(_u43DPlane, e3DMode, _u4Shift,
                    _u4BmpWidth, _u4BmpHeight, _u4BmpPitch,
                    _u4DstAddr, _eColorMode, _u4SrcAddr);
        }

        _e3DMode = e3DMode;

    }
    return (INT32)OSD_RET_OK;

}
#endif

#ifdef CC_CONFIG_OSD_SBS
INT32 OSD_PLA_Set3DDFB(UINT32 u4Plane, UINT32 e3DMode, UINT32 u4Shift,
        UINT32 u4BmpWidth, UINT32 u4BmpHeight, UINT32 u4BmpPitch,
        UINT32 u4DstAddr, UINT32 eColorMode, UINT32 u4SrcAddr)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    E_RZ_INOUT_TYPE_T eRzType;

    x_memset(&rSclParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));

    if (e3DMode == 0)//OSD_3D_SBSHALF

    {
        rSclParam.u4TgW = u4BmpWidth >> 1;
        rSclParam.u4TgH = u4BmpHeight;
    }
    else if (e3DMode == 1) //OSD_3D_TPANDBT

    {
        rSclParam.u4TgW = u4BmpWidth;
        rSclParam.u4TgH = u4BmpHeight >> 1;
    }
    else
    {
        GFX_Lock();
        GFX_MMU_Set_Enable(0x0);
        GFX_SetSrc((UINT8 *)u4SrcAddr, eColorMode, u4BmpPitch);
        GFX_SetDst((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);
        GFX_BitBlt(0, 0, 0, 0, u4BmpWidth, u4BmpHeight);

        GFX_Flush();
        GFX_Wait();
        GFX_Unlock();
        return (INT32)OSD_RET_OK;
    }

    rSclParam.u4SrcBufLen = u4BmpPitch;
    rSclParam.u4SrcW = u4BmpWidth;
    rSclParam.u4SrcH = u4BmpHeight;

    rSclParam.u4SrcBase = u4SrcAddr;
    rSclParam.u4IsSrcDirCM = 1;
    rSclParam.u4SrcCM = (eColorMode == OSD_CM_ARGB4444_DIRECT16)?
    (UINT32) E_RZ_OSD_DIR_CM_ARGB4444:(UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
    rSclParam.u4SrcHOffset = 0;
    rSclParam.u4SrcVOffset = 0;

    rSclParam.u4TgBase = u4DstAddr;

    rSclParam.u4TgHOffset = 0;
    rSclParam.u4TgVOffset = 0;

    rSclParam.u4TgCM = rSclParam.u4SrcCM;
    rSclParam.u4TgBufLen = u4BmpPitch;

    eRzType = E_RZ_INOUT_TYPE_OSDMD;

    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(eRzType);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

    GFX_Lock();
    GFX_MMU_Set_Enable(0x0);
    GFX_SetSrc((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);

    GFX_SetDst((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);
    GFX_SetColor(0x0);
    GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0x00000000, 0xffffffff);

    if (e3DMode == 0)// OSD_3D_SBSHALF

    {
        GFX_Fill(rSclParam.u4TgW, 0, rSclParam.u4TgW, rSclParam.u4TgH);
        GFX_BitBlt((u4Shift >> 1), 0, rSclParam.u4TgW, 0, (rSclParam.u4TgW - (u4Shift >> 1)), rSclParam.u4TgH);
    }
    else //OSD_3D_TPANDBT

    {
        GFX_Fill(0, rSclParam.u4TgH, rSclParam.u4TgW, rSclParam.u4TgH);
        GFX_BitBlt(u4Shift, 0, 0, rSclParam.u4TgH, (rSclParam.u4TgW - u4Shift), rSclParam.u4TgH);
    }

    GFX_SetSrc((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);
    GFX_SetDst((UINT8 *)u4DstAddr, eColorMode, u4BmpPitch);
    GFX_BitBlt(0, 0, 0, 0, u4BmpWidth, u4BmpHeight);

    GFX_Flush();
    GFX_Wait();
    GFX_Unlock();

    return (INT32)OSD_RET_OK;

}
#endif
#ifdef OSD_DSC_PDP

INT32 OSD_PLA_DsBlit(UINT32 u4SrcAddr, UINT32 u4SrcX, UINT32 u4SrcY,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4SrcPitch,
        UINT32 u4DstAddr, UINT32 u4DstX, UINT32 u4DstY,
        UINT32 u4DstWidth, UINT32 u4DstHeight,UINT32 u4DstpPitch,UINT32 eColorMode)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    E_RZ_INOUT_TYPE_T eRzType;

    x_memset(&rSclParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));

    rSclParam.u4TgW = u4DstWidth;
    rSclParam.u4TgH = u4DstHeight;

    rSclParam.u4SrcBufLen = u4SrcPitch;
    rSclParam.u4SrcW = u4SrcWidth;
    rSclParam.u4SrcH = u4SrcHeight;

    rSclParam.u4SrcBase = u4SrcAddr;
    rSclParam.u4IsSrcDirCM = 1;
    rSclParam.u4SrcCM = (eColorMode == OSD_CM_ARGB4444_DIRECT16)?
    (UINT32) E_RZ_OSD_DIR_CM_ARGB4444:(UINT32) E_RZ_OSD_DIR_CM_ARGB8888;
    rSclParam.u4SrcHOffset = u4SrcX;
    rSclParam.u4SrcVOffset = u4SrcY;

    rSclParam.u4TgBase = u4DstAddr;

    rSclParam.u4TgHOffset = u4DstX;
    rSclParam.u4TgVOffset = u4DstY;

    rSclParam.u4TgCM = rSclParam.u4SrcCM;
    rSclParam.u4TgBufLen = u4DstpPitch;

    eRzType = E_RZ_INOUT_TYPE_OSDMD;

    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(eRzType);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

    return (INT32)OSD_RET_OK;

}
#endif
#ifdef CC_ANDROID_3D_UI_SUPPORT
UINT32 u4UiMode = OSD_3D_NONE;
UINT32 u4Plane2RegionList = 0;
BOOL u4Plane2Enabel = 0;
BOOL fgUpdateAttr = 1;
UINT32 u4UiWidth = 1280, u4UiHeight = 720;
UINT32 u4OriSc2_Enable = 0;
UINT32 u4OriSc2SrcW = 0;
UINT32 u4OriSc2SrcH = 0;
UINT32 u4OriSc2DstW = 0;
UINT32 u4OriSc2DstH = 0;
UINT32 u4OriSc2Is16Bpp = 0;

UINT32 OSD_PLA_SetUIMode(UINT32 u4Mode, UINT32 u4WIdth, UINT32 u4Height)
{
    u4UiMode = u4Mode;
    fgUpdateAttr = 0;
    if((u4WIdth !=0) &&(u4Height!=0))
    {
        u4UiWidth = u4WIdth;
        u4UiHeight = u4Height;
    }
    OSD_PLA_FlipTo(OSD_PLANE_2,u4Plane2RegionList);
    OSD_PLA_Enable(OSD_PLANE_2,u4Plane2Enabel);
    OSD_SC_Scale(OSD_SCALER_2, TRUE, u4OriSc2SrcW, u4OriSc2SrcH, u4OriSc2DstW, u4OriSc2DstH);
    return (INT32)OSD_RET_OK;
}

INT32 OSD_PLA_Set3DSwitchRegion(UINT32 u4LeftRgn, UINT32 u4RightRgn)
{
    UINT32 u4u4LeftRgnAddr;
    UINT32 u4u4RightRgnAddr;
    UINT32 u4LeftPlane = OSD_PLANE_3, u4RightPlane = OSD_PLANE_2;

    OSD_VERIFY_REGION(u4LeftRgn);

    _arOsdPlaneInfo[u4LeftPlane].i4FirstRegionId = u4LeftRgn;
    _arOsdPlaneInfo[u4RightPlane].i4FirstRegionId = u4RightRgn;
    LOG(5," region = %d, %d,\n",u4LeftRgn, u4RightRgn);

    VERIFY((INT32)OSD_RET_OK ==
            _OSD_RGN_GetAddress((UINT32)u4LeftRgn, &u4u4LeftRgnAddr));

    VERIFY((INT32)OSD_RET_OK ==
            _OSD_PLA_SetHeaderAddr(u4LeftPlane, u4u4LeftRgnAddr));

    VERIFY((INT32)OSD_RET_OK ==
            _OSD_RGN_GetAddress((UINT32)u4RightRgn, &u4u4RightRgnAddr));

    LOG(5," 111 region  = %d, %d, 0x%x, 0x%x\n",u4LeftRgn, u4RightRgn, u4u4LeftRgnAddr, u4u4RightRgnAddr);

    x_memcpy((void*)u4u4RightRgnAddr, (void*)u4u4LeftRgnAddr, sizeof(OSD_RGN_UNION_T));

    VERIFY((INT32)OSD_RET_OK ==
            _OSD_PLA_SetHeaderAddr(u4RightPlane, u4u4RightRgnAddr));

    LOG(5," region= %d, %d, 0x%x, u4u4RightRgnAddr = 0x%x\n",u4LeftRgn, u4RightRgn, u4u4LeftRgnAddr, u4u4RightRgnAddr);

    _arOsdPlaneInfo[u4LeftPlane].u4FirstRegionAddr = u4u4LeftRgnAddr;
    _arOsdPlaneInfo[u4RightPlane].u4FirstRegionAddr = u4u4RightRgnAddr;

    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4LeftPlane));
    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_UpdateHwReg(u4RightPlane));

    return (INT32)OSD_RET_OK;
}

INT32 OSD_PLA_Set3DSwitchRgnAddr(UINT32 u4LeftRgn, UINT32 u4RightRgn,UINT32 u4LeftAddr, UINT32 u4RightAddr)
{
    // UINT32 u4u4LeftRgnAddr;
    // UINT32 u4u4RightRgnAddr;
    //OSD_LockFlipTo();
    //GFX_FlushInvalidateDCacheMultipleLine();
    _OSD_RGN_SetDataAddr(u4LeftRgn, u4LeftAddr);
    _OSD_RGN_SetDataAddr(u4RightRgn, u4RightAddr);
    //OSD_UnLockFlipTo();
    return (INT32)OSD_RET_OK;
}
#endif

#endif

UINT32 OSD_PLA_GetHStart(UINT32 u4Plane)
{
    UINT32 u4Start =OSD_INVALID_VALUE_U32;
    
    switch(u4Plane)
    {
        case OSD_PLANE_1:
            _OSD_BASE_GetOsd1HStart(&u4Start);
            break;
        case OSD_PLANE_2:
            _OSD_BASE_GetOsd2HStart(&u4Start);
            break;
        case OSD_PLANE_3:
            _OSD_BASE_GetOsd3HStart(&u4Start);
            break;
        default:
            break;
    }

    return u4Start;
}

UINT32 OSD_PLA_GetVStart(UINT32 u4Plane)
{
    UINT32 u4Start =OSD_INVALID_VALUE_U32;
    
    switch(u4Plane)
    {
        case OSD_PLANE_1:
            _OSD_BASE_GetOsd1VStart(&u4Start);
            break;
        case OSD_PLANE_2:
            _OSD_BASE_GetOsd2VStart(&u4Start);
            break;
        case OSD_PLANE_3:
            _OSD_BASE_GetOsd3VStart(&u4Start);
            break;
        default:
            break;
    }

    return u4Start;
}

INT32 OSD_PLA_SetPremulEn(UINT32 u4Plane, UINT32 u4Value)
{
     return _OSD_PLA_SetPremulEn(u4Plane, u4Value);
}

INT32 OSD_PLA_GetPremulEn(UINT32 u4Plane, UINT32 *pu4Value)
{
    UINT32 u4Enable;

    if (pu4Value == NULL)
    {
        return -(INT32)OSD_RET_INV_ARG;
    }
    OSD_VERIFY_PLANE(u4Plane);

    VERIFY((INT32)OSD_RET_OK == _OSD_PLA_GetPremulEn(u4Plane, &u4Enable));
    *pu4Value = u4Enable;

    return (INT32)OSD_RET_OK;
}

void OSD_PLA_Chg_Clock_Action(UINT32 u4_enable)
{
    UINT32 u4Plane;

    for(u4Plane = 0; u4Plane < OSD_PLANE_MAX_NUM; ++u4Plane)
    {
        if( _arOsdPlaneInfo[u4Plane].fgPlaneEnable)
        {
            _OSD_PLA_SetEnable(u4Plane,u4_enable);
            _UpdatePlaneEnableState(u4Plane);
        }
    }
    
    return;
}
UINT32 OSD_PLA_GetScaler(UINT32 u4Plane)
{
    UINT32 u4Scaler = 0;
    if(u4Plane == (UINT32)OSD_PLANE_2)
    {
        u4Scaler = (UINT32)OSD_SCALER_2;
    }
    else if(u4Plane == (UINT32)OSD_PLANE_3)
    {
        u4Scaler = (UINT32)OSD_SCALER_3;
    }
    else if(u4Plane == (UINT32)OSD_PLANE_1)
    {
        u4Scaler = (UINT32)OSD_SCALER_1;
    }
    return u4Scaler;
    
}

INT32 OSD_PLA_LineScalerShow(UINT32 u4Plane,UINT32 u4BufferAddr,UINT32 u4SrcWidth,
        UINT32 u4SrcHeight,UINT32 u4SrcPitch,UINT32 u4DstX,UINT32 u4DstY,
        UINT32 u4DstWidth,UINT32 u4DstHeight,UINT32 u4ColorMode)
{
    static UINT32 u4RegionList = 0xFFFFFFFF;
    UINT32 u4FirstRegion = 0;
    if(u4RegionList == 0xFFFFFFFF)
    {
        OSD_RGN_LIST_Create(&u4RegionList);
    }
    Printf("RegionList = %d\n",u4RegionList);
    OSD_RGN_LIST_LineScaler(u4RegionList,u4SrcWidth,u4SrcHeight,(void *)u4BufferAddr,
                            u4ColorMode,u4SrcPitch,u4DstX,u4DstY,u4DstWidth,u4DstHeight);
    OSD_PLA_FlipTo(u4Plane,u4RegionList);
    u4FirstRegion = OSD_PLA_GetFirstRegion(u4Plane);
    OSD_RGN_Set(u4FirstRegion,OSD_RGN_DISP_H,u4DstHeight/2);
    OSD_SC_Scale(OSD_PLA_GetScaler(u4Plane),FALSE,OSD_BASE_GetDisplayWidth(),OSD_BASE_GetDisplayHeight(),
                    OSD_BASE_GetDisplayWidth(),OSD_BASE_GetDisplayHeight());
    OSD_PLA_Enable(u4Plane,TRUE);

    return (INT32)OSD_RET_OK;
}
BOOL OSD_PLA_IsSupport4K2K(void)
{
    #ifdef OSD_4K2K_SUPPORT
    return TRUE;
    #else
    return FALSE;
    #endif
}
UINT32 OSD_PLA_Get_Hw_RealBmpAddr(UINT32 u4Plane, UINT32 *pu4Value)
{
    return _OSD_PLA_Get_Hw_RealBmpAddr(u4Plane, pu4Value);
}
#ifdef OSD_4K2K_SUPPORT
INT32 OSD_PLA_Get4K2KOriScalerInformation(UINT32 u4OSDPlane,OSD_4K2KOriInformation *pOSD4K2KInfo)
{   
    if(u4OSDPlane==OSD_PLANE_2)
    {
        OSD_SC_GetScalerInfo(OSD_SCALER_2,&(pOSD4K2KInfo->u4OriScalerEnable),
            &(pOSD4K2KInfo->u4OriScalerSrcWidth),
            &(pOSD4K2KInfo->u4OriScalerSrcHeight),
            &(pOSD4K2KInfo->u4OriScalerDstWidth),
            &(pOSD4K2KInfo->u4OriScalerDstHeight),
            &(pOSD4K2KInfo->u4OriScalerIs16Bpp));
    }
    if(u4OSDPlane==OSD_PLANE_3)
    {
        OSD_SC_GetScalerInfo(OSD_SCALER_3,&(pOSD4K2KInfo->u4OriScalerEnable),
            &(pOSD4K2KInfo->u4OriScalerSrcWidth),
            &(pOSD4K2KInfo->u4OriScalerSrcHeight),
            &(pOSD4K2KInfo->u4OriScalerDstWidth),
            &(pOSD4K2KInfo->u4OriScalerDstHeight),
            &(pOSD4K2KInfo->u4OriScalerIs16Bpp));
    }
    return (INT32)OSD_RET_OK;
}
INT32 OSD_PLA_Get4K2KOriRegionInformation(UINT32 u4OSDPlane,OSD_4K2KOriInformation *pOSD4K2KInfo)
{
    UINT32 u4CurrentRegionId=0;
    u4CurrentRegionId=OSD_PLA_GetFirstRegion(u4OSDPlane);
    pOSD4K2KInfo->u4OSDPlane=u4OSDPlane;
    pOSD4K2KInfo->u4RegionId=u4CurrentRegionId;
    OSD_RGN_Get(u4CurrentRegionId,OSD_RGN_OUT_W,&(pOSD4K2KInfo->u4OriRegionWidth));
    OSD_RGN_Get(u4CurrentRegionId,OSD_RGN_OUT_H,&(pOSD4K2KInfo->u4OriRegionHeight));
    OSD_RGN_Get(u4CurrentRegionId,OSD_RGN_BMP_PITCH,&(pOSD4K2KInfo->u4OriRegionPitch));
    OSD_RGN_Get(u4CurrentRegionId,OSD_RGN_BMP_ADDR,&(pOSD4K2KInfo->u4OriRegionAddr));
    b4K2KSaveInfo = TRUE;
    return (INT32)OSD_RET_OK;
}
INT32 OSD_PLA_GetJpegFramesAddr(UINT32 u4OSDPlane,OSD_4K2KJpecModeCurrentSet *pJpecModeCurrentSet)
{
    UINT32 u4RegionAddress=0;
    UINT32 u4RegionHeight=0;
    UINT32 u4RegionPitch=0;
    UINT32 u4CurrentId=0;
    u4CurrentId=OSD_PLA_GetFirstRegion(u4OSDPlane);
    OSD_RGN_Get(u4CurrentId,OSD_RGN_BMP_ADDR,&u4RegionAddress);
    OSD_RGN_Get(u4CurrentId,OSD_RGN_OUT_H,&u4RegionHeight);
    OSD_RGN_Get(u4CurrentId,OSD_RGN_BMP_PITCH,&u4RegionPitch);
    pJpecModeCurrentSet->u4FirstFrameAddress=u4RegionAddress;
    pJpecModeCurrentSet->u4SecondFrameAddress=pJpecModeCurrentSet->u4FirstFrameAddress+u4RegionPitch/2;
    pJpecModeCurrentSet->u4ThirdFrameAddress=u4RegionAddress+u4RegionPitch*(u4RegionHeight/2);
    pJpecModeCurrentSet->u4ForthFrameAddress=pJpecModeCurrentSet->u4ThirdFrameAddress+u4RegionPitch/2;
                

    return (INT32)OSD_RET_OK;
}
INT32 OSD_PLA_GetMVCFrameAddr(UINT32 u4OSDPlane,OSD_4K2KMvcModeCurrentSet *pMvcModeSet)
{
    UINT32 u4RegionAddress=0;
    UINT32 u4RegionPitch=0;
    UINT32 u4CurrentRegionId=0;
    OSD_RGN_Get(u4CurrentRegionId,OSD_RGN_BMP_ADDR,&u4RegionAddress);
    OSD_RGN_Get(u4CurrentRegionId,OSD_RGN_BMP_PITCH,&u4RegionPitch);
    pMvcModeSet->u4LeftFrameAddress=u4RegionAddress;
    pMvcModeSet->u4RightFrameAddress=u4RegionAddress+u4RegionPitch/2;
    
    return (INT32)OSD_RET_OK;
}
INT32 OSD_PLA_SetJpecRegionAttri(OSD_4K2KOriInformation *pJpecRegionOriSet)
{
    if((pJpecRegionOriSet->u4OriRegionWidth==0) 
        || (pJpecRegionOriSet->u4OriRegionHeight==0)
        ||(pJpecRegionOriSet->u4OriRegionPitch==0))
    {
        LOG(5,"%s:current region is not right.\n",__FUNCTION__);
        return (INT32)OSD_RET_UNDEF_ERR;
    }
    OSD_RGN_Set(pJpecRegionOriSet->u4RegionId, OSD_RGN_BMP_W, (pJpecRegionOriSet->u4OriRegionWidth)/2);
    OSD_RGN_Set(pJpecRegionOriSet->u4RegionId, OSD_RGN_BMP_H, (pJpecRegionOriSet->u4OriRegionHeight)/2);
    OSD_RGN_Set(pJpecRegionOriSet->u4RegionId, OSD_RGN_DISP_W, (pJpecRegionOriSet->u4OriRegionWidth)/2);
    OSD_RGN_Set(pJpecRegionOriSet->u4RegionId, OSD_RGN_DISP_H, (pJpecRegionOriSet->u4OriRegionHeight)/2);
    OSD_RGN_Set(pJpecRegionOriSet->u4RegionId, OSD_RGN_BMP_PITCH, pJpecRegionOriSet->u4OriRegionPitch);
    return (INT32)OSD_RET_OK;
}
INT32 OSD_PLA_ReductOriRGNInfo(UINT32 u4Plane,OSD_4K2KOriInformation *pOSD4K2KInfo)
{
    if((!b4K2KSaveInfo)||(pOSD4K2KInfo->u4RegionId == -1))
    {
        return OSD_RET_OK;
    }
    OSD_RGN_Set(pOSD4K2KInfo->u4RegionId,OSD_RGN_OUT_W,pOSD4K2KInfo->u4OriRegionWidth);
    OSD_RGN_Set(pOSD4K2KInfo->u4RegionId,OSD_RGN_OUT_H,pOSD4K2KInfo->u4OriRegionHeight);
    OSD_RGN_Set(pOSD4K2KInfo->u4RegionId,OSD_RGN_BMP_W,pOSD4K2KInfo->u4OriRegionWidth);
    OSD_RGN_Set(pOSD4K2KInfo->u4RegionId,OSD_RGN_BMP_H,pOSD4K2KInfo->u4OriRegionHeight);
    OSD_RGN_Set(pOSD4K2KInfo->u4RegionId,OSD_RGN_BMP_PITCH,pOSD4K2KInfo->u4OriRegionPitch);
    _OSD_RGN_SetDataAddr(pOSD4K2KInfo->u4RegionId,pOSD4K2KInfo->u4OriRegionAddr);
    b4K2KSaveInfo = FALSE;
    return (INT32)OSD_RET_OK;
}
INT32 OSD_PLA_SetMVCRegionAttri(OSD_4K2KOriInformation *pMvcRegionOriSet)
{
    UINT32 u4RegionConvertPitch=0;
    u4RegionConvertPitch=((pMvcRegionOriSet->u4OriRegionPitch)/2+0xF)&(~0xF);
    if((pMvcRegionOriSet->u4OriRegionWidth==0) 
        || (pMvcRegionOriSet->u4OriRegionHeight==0)
        ||(pMvcRegionOriSet->u4OriRegionPitch==0))
    {
        LOG(5,"%s:current region is not right.\n",__FUNCTION__);
        return (INT32)OSD_RET_UNDEF_ERR;
    }
    OSD_RGN_Set(pMvcRegionOriSet->u4RegionId, OSD_RGN_BMP_W, (pMvcRegionOriSet->u4OriRegionWidth)/2);
    OSD_RGN_Set(pMvcRegionOriSet->u4RegionId, OSD_RGN_BMP_H, pMvcRegionOriSet->u4OriRegionHeight);
    OSD_RGN_Set(pMvcRegionOriSet->u4RegionId, OSD_RGN_DISP_W, (pMvcRegionOriSet->u4OriRegionWidth)/2);
    OSD_RGN_Set(pMvcRegionOriSet->u4RegionId, OSD_RGN_DISP_H, pMvcRegionOriSet->u4OriRegionHeight);
    OSD_RGN_Set(pMvcRegionOriSet->u4RegionId, OSD_RGN_BMP_PITCH, u4RegionConvertPitch);
    return (INT32)OSD_RET_OK;
}
UINT32 OSD_PLA_SetLinuxUIMode(UINT32 u4Mode,UINT32 u4WIdth, UINT32 u4Height, UINT32 u4Value1, UINT32 u4Value2)
{
#ifdef CC_SUPPORT_4K2K_UI
        UINT32 u4Enable = 0;
        UINT32 u4SrcW = 0;
        UINT32 u4SrcH = 0;
        UINT32 u4DstW = 0;
        UINT32 u4DstH = 0;
        UINT32 u4Is16Bpp = 0;

    OSD_SC_GetScalerInfo(OSD_SCALER_2, &u4Enable,
                       &u4SrcW, &u4SrcH,
                       &u4DstW, &u4DstH,
                       &u4Is16Bpp);
    #ifndef CC_MTK_LOADER
    if((_u4HWCAddWidth == 3840) && (OSD_BASE_GetDisplayWidth() ==1920)) //for browser fullscreen play
    {
         OSD_SC_Scale(OSD_SCALER_2, u4Enable, OSD_BASE_GetDisplayWidth(), OSD_BASE_GetDisplayHeight(), OSD_BASE_GetDisplayWidth(), OSD_BASE_GetDisplayHeight());
    }   
    else if((_u4HWCAddWidth != u4SrcW) && (_u4HWCAddHeight != u4SrcH))
       {
         OSD_SC_Scale(OSD_SCALER_2, u4Enable, _u4HWCAddWidth, _u4HWCAddHeight, OSD_BASE_GetDisplayWidth(), OSD_BASE_GetDisplayHeight());
    }
    else
    #endif
    {
         OSD_SC_Scale(OSD_SCALER_2, u4Enable, u4SrcW, u4SrcH, OSD_BASE_GetDisplayWidth(), OSD_BASE_GetDisplayHeight());
    }
    
    OSD_SC_GetScalerInfo(OSD_SCALER_3, &u4Enable,
                       &u4SrcW, &u4SrcH,
                       &u4DstW, &u4DstH,
                       &u4Is16Bpp);
      OSD_SC_Scale(OSD_SCALER_3, u4Enable, u4SrcW, u4SrcH, OSD_BASE_GetDisplayWidth(), OSD_BASE_GetDisplayHeight());
#else
    u4LinuxUiMode4K2K=u4Mode;
    if(u4Mode == OSD_4K2K_MODE_HDMI_4K2K)
    {
        OSD_Base_SetHStart();
        u4LinuxUiHDMITime=OSD_BASE_GetDisplayWidth()/u4LinuxUiNoneModePanelWidth;
        if(b4K2KOSD2Enable)
        {
            OSD_SC_Scale(OSD_SCALER_2, TRUE, 
                OSD2OriInformation.u4OriScalerSrcWidth, 
                OSD2OriInformation.u4OriScalerSrcHeight,
                OSD2OriInformation.u4OriScalerDstWidth*u4LinuxUiHDMITime, 
                OSD2OriInformation.u4OriScalerDstHeight*u4LinuxUiHDMITime);
        }
        if(b4K2KOSD3Enable)
        {
            OSD_SC_Scale(OSD_SCALER_3, TRUE, 
                OSD3OriInformation.u4OriScalerSrcWidth, 
                OSD3OriInformation.u4OriScalerSrcHeight,
                OSD3OriInformation.u4OriScalerDstWidth*u4LinuxUiHDMITime, 
                OSD3OriInformation.u4OriScalerDstHeight*u4LinuxUiHDMITime);
        }
    }
    else if(u4Mode == OSD_4K2K_MODE_MVC)
    {
        if(b4K2KOSD2Enable)
        {
            OSD_PLA_GetMVCFrameAddr(OSD_PLANE_2,&OSD2MvcModeCurrentSet); 
            OSD_PLA_SetMVCRegionAttri(&OSD2OriInformation);
        }
        if(b4K2KOSD3Enable)
        {
            OSD_PLA_GetMVCFrameAddr(OSD_PLANE_3,&OSD3MvcModeCurrentSet);
            OSD_PLA_SetMVCRegionAttri(&OSD3OriInformation);
        }
        
        
    }
    else if(u4Mode == OSD_4K2K_MODE_JPEG)
    {
        OSD_Base_SetHStart();
        u4LinuxUiHDMITime=OSD_BASE_GetDisplayWidth()/u4LinuxUiNoneModePanelWidth;
        if(b4K2KOSD2Enable)
        {
            OSD_SC_Scale(OSD_SCALER_2, TRUE, 
                OSD2OriInformation.u4OriScalerSrcWidth, 
                OSD2OriInformation.u4OriScalerSrcHeight,
                OSD2OriInformation.u4OriScalerDstWidth*u4LinuxUiHDMITime, 
                OSD2OriInformation.u4OriScalerDstHeight*u4LinuxUiHDMITime);
        }
        if(b4K2KOSD3Enable)
        {
            OSD_SC_Scale(OSD_SCALER_3, TRUE, 
                OSD3OriInformation.u4OriScalerSrcWidth, 
                OSD3OriInformation.u4OriScalerSrcHeight,
                OSD3OriInformation.u4OriScalerDstWidth*u4LinuxUiHDMITime, 
                OSD3OriInformation.u4OriScalerDstHeight*u4LinuxUiHDMITime);
        }
    }
    else if(u4Mode == OSD_4K2K_MODE_NONE)
    {
        if(b4K2KOSD2Enable)
        {
            OSD_PLA_ReductOriRGNInfo(OSD_PLANE_2,&OSD2OriInformation);
            OSD_SC_Scale(OSD_SCALER_2, OSD2OriInformation.u4OriScalerEnable, 
            OSD2OriInformation.u4OriScalerSrcWidth,
            OSD2OriInformation.u4OriScalerSrcHeight, 
            OSD2OriInformation.u4OriScalerDstWidth, 
            OSD2OriInformation.u4OriScalerDstHeight);
        }
        if(b4K2KOSD3Enable)
        {
            OSD_PLA_ReductOriRGNInfo(OSD_PLANE_3,&OSD3OriInformation);
            OSD_SC_Scale(OSD_SCALER_3, OSD3OriInformation.u4OriScalerEnable, 
            OSD3OriInformation.u4OriScalerSrcWidth,
            OSD3OriInformation.u4OriScalerSrcHeight, 
            OSD3OriInformation.u4OriScalerDstWidth, 
            OSD3OriInformation.u4OriScalerDstHeight);
        }
    }
  #endif
    return OSD_RET_OK; 
}
#endif

