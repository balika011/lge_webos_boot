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
 * $Date: 2015/02/05 $
 * $RCSfile: drv_display.c,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_display.c
 *  Brief of file drv_display.c.
 *  Details of file drv_display.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"
#include "drv_vdoclk.h"
#include "util.h"
#include "hw_scpos.h"
#include "hw_mlvds.h"
#include "hw_tcon.h"
#include "hw_tg.h"
#include "hw_ycproc.h"
#include "hw_ospe.h"
#include "drv_scpos.h"
#include "drv_display.h"
#include "drv_scaler.h"
#include "drv_ycproc.h"
#include "drv_lvds.h"
#include "drv_di.h"
#include "vdo_misc.h"
#include "drv_hdtv.h"
#include "video_def.h"
#include "drv_video.h"
#include "drv_tcon.h"
#if defined(CC_MT5399) || defined (CC_MT5882)
#include "drv_lcdim_v2.h"
#else
#include "drv_lcdim.h"
#endif
#include "nptv_debug.h"
#include "api_backlt.h"
#include "mute_if.h"
#include "srm_drvif.h"
#include "drv_ldmspi.h"
#ifndef CC_COPLAT_MT82
#include "osd_drvif.h"
#include "pmx_drvif.h"
#endif
#include "x_assert.h"
#include "x_lint.h"
#include "x_pinmux.h"
#include "c_model.h"
#include "x_timer.h"
#include "ostg_if.h"
#include "drv_od.h"	
#ifdef THE_3RD_PARTY_HEADER_FILE_1
#include THE_3RD_PARTY_HEADER_FILE_1
#endif
#ifdef CC_DRV_SUPPORT_SMICRO
#include "SMicro/SMicro_if.h"
#endif
#include "x_gpio.h"
#ifdef CC_MT5399
#ifndef CC_MTK_LOADER
#include "drv_mjc.h"
#include "drv_mjc_if.h"
#include "hw_mjc.h"
#endif
#endif
#ifdef CC_SCPOS_3DTV_SUPPORT
#include "drv_tdtv.h"
#endif

#include "drv_pwm.h"
#include "drv_vdoclk.h"
#include "drv_scaler_drvif.h"
#include "frametrack_drvif.h"
#include "drvcust_if.h"  //mtk70763, 2010-7-21
#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif

#include "eeprom_if.h"
#include "osd_drvif.h"
#include "drv_scaler_psc.h"
#ifdef CC_SUPPORT_4K2K
#include "drv_mpeg.h"
#include "drv_dvi.h"
#endif
#ifdef CC_SUPPORT_HDMI_4K2K30
#include "drv_dvi.h"
#endif

#include "drv_vbyone.h"
#include "panel.h"


//luis@2007/7/5, for vIO32WriteFldXXX
/*lint -save -e666*/

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define CC_BACKPORCH_MIN (99 + 15)
#define CC_FRONTPORCH_MIN (10)
#define CC_BACKPORCH_MAX (2047)
#define CC_FRONTPORCH_MAX (30)
#define OSTG_UPDATE_PIXEL_SHIFT_H 1
#define OSTG_UPDATE_PIXEL_SHIFT_V 2

// force the HTotal to be 4x-aligned in Dispmode + Dual or Four port panel
#define CC_HTOTAL_4X_LIMIT_WA

// when VOUT_H is smaller than HTotal, VSync will be lost.
#define CC_OSTG_WA1

#if CC_OSD_MUTE_WA
UINT8 _bDddsDelayFlag = 0 ;
#endif

UINT32 _u4NSDHTotal=0xff;
UINT32 _u4NSDVTotal=0xff;
UINT32 _u4NSDFrameRate=0xff;

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
//hongjun marked for uboot build pass 
#ifdef CC_UBOOT
UINT32 _u4VPllFrequency;
#else
extern UINT32 _u4VPllFrequency;
#endif

//-----------------------------------------------------------------------------
// Public variables
//-----------------------------------------------------------------------------
//Give initial value instead of using "CC_DVBT_SUPPORT" define
UINT8 bSI_DISPLAY_DCLK_TYPE = SV_DCLK_UNKNOW;
UINT8 bOUTPUT_REFRESH_RATE = 0;
UINT8 bFORCE_FRAMERATE_TYPE  = ~0;

UINT8 bDISPLAY_INTERLACE_ENABLE;
BOOL _fgFRS_ENA;
BOOL fgDDDSEn;
enum eSyncLockMode _eSyncLockMode = eSYNCLOCK_IGNORED;

UINT8 u1CustFRC;

#define ERRORLIMIT_INI 0x1F
UINT32 _u4ErrorLimit = ERRORLIMIT_INI;

static UINT32 _u4OstgUpdateEvent = 0;

#ifdef SUPPORT_CW_CHANGE_BY_STEP
UINT32 _u4TargetCW = 0;
UINT32 _u4CurrentCW = 0;
UINT32 _u4CWStep = 0x500000;
#endif

#ifdef SUPPORT_DDDS_STEP_TRACKING
UINT32 _u4CenterCW= 0;
UINT16 _u2DDDSStepTrackingEn = 0;
UINT32 _u4DDDSTrackingErrorLimit = 2;        
UINT32 _u4StepInCWTolerance = 0x1000;  
UINT32 _u4CurrentStepCW = 0;

UINT32 _u4StepInCWUpperBound = 0;
UINT32 _u4StepInCWLowerBound = 0;
UINT32 _u4DefaultErrorLimit = 0xf;
#endif

#ifdef CC_MT5399
static UINT16 _u2IntFrameRate = 0;
BOOL bForceMJCMute = SV_FALSE;
#endif

UINT8 _bForceFrameRate = 0;
BOOL _fgFrTrkHalfStep = FALSE;
UINT8 _uDDDSDivision = 0;

UINT8 bSupportLocalDimming = SV_FALSE;

UINT16 w3DGlassesDelay = 0;

UINT8 fgPentouchMode = SV_OFF;
UINT8 fgApplyScalerSetData=0;
UINT8 fgBLCustomCtrl=0;  // 1: Backlight can be controled by the customer.

BOOL fgBrowserModeEn = SV_FALSE;
BOOL fgSpeIPTVSEn = SV_FALSE;
typedef struct{
    UINT16 u2FrameRate;	
    UINT16 u2Width;
    UINT16 u2Height;	
    UINT16 u2HTotal;
    UINT16 u2VTotal;
    UINT32 u4PixelClk;	
    UINT16 u4VsyncWidth;	
    UINT16 u4VsyncBp;	
#ifdef SUPPORT_DGO
	UINT16 u4HsyncWidth;
	UINT16 u4HsyncFp;
#endif
}S_PANEL_CONFIG;

typedef struct{
    UINT32 u4PixleClockMax;	
    UINT32 u4PixleClockMin;
    UINT16 u2HTotalMax;	
    UINT16 u2HTotalMin;
    UINT16 u2VTotalMax;	
    UINT16 u2VTotalMin;		
    UINT16 u2VClkMax;	
    UINT16 u2VClkMin;	
}S_PANEL_CONFIG_COMMON;

#ifdef SUPPORT_DGO
S_PANEL_CONFIG rPANEL_DEFAULT[3];
S_PANEL_CONFIG rPANEL_480P[3];  //  [0]: for 48Hz, [1]: for 50Hz, [2]: for 60Hz,  
S_PANEL_CONFIG rPANEL_576P[3]; 
S_PANEL_CONFIG rPANEL_720P[3];
S_PANEL_CONFIG rPANEL_1080P[3];
S_PANEL_CONFIG rPANEL_1080P_FP[3];

S_PANEL_CONFIG_COMMON rPANEL_DEFAULT_COMMON;
S_PANEL_CONFIG_COMMON rPANEL_480P_COMMON;
S_PANEL_CONFIG_COMMON rPANEL_576P_COMMON;
S_PANEL_CONFIG_COMMON rPANEL_720P_COMMON;
S_PANEL_CONFIG_COMMON rPANEL_1080P_COMMON;
S_PANEL_CONFIG_COMMON rPANEL_1080P_FP_COMMON;

UINT32 _u4CtrlWord_DEFAULT = 0;
UINT32 _u4CtrlWord2_DEFAULT = 0;

UINT8 _u1DGOMode = 0xff;

static HANDLE_T _hDGOUnmuteSema=(HANDLE_T)(NULL);
static HANDLE_T _hDGOSetPanelSema=(HANDLE_T)(NULL);

static void vSavePanelAttribute_DGO(void);
static void vDGOUnmute(void *pvArgs);
#endif	

#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
UINT32 _u4I2CDly = 800;     // ms
UINT8 _u14K2KMode = E_4K2K_NORMAL;
UINT8 _u1Force4K2KTiming =0;
S_PANEL_CONFIG rPANEL_FHD120[3];  // [0]: for 48Hz, [1]: for 50Hz, [2]: for 60Hz
S_PANEL_CONFIG rPANEL_FHD60[3]; 
S_PANEL_CONFIG rPANEL_4K2K30[3];

S_PANEL_CONFIG_COMMON rPANEL_FHD120_COMMON;
S_PANEL_CONFIG_COMMON rPANEL_FHD60_COMMON;
S_PANEL_CONFIG_COMMON rPANEL_4K2K30_COMMON;
HANDLE_T h4k2kUnMuteTimer ;

static BOOL fg4k2kTblInit=FALSE; 
UINT32 _4k2kPanelType = PANEL_4K2K_FHD120;
HANDLE_T _hFrcUnmuteSema=(HANDLE_T)(NULL);
static HANDLE_T _hFrcSetPanelSema=(HANDLE_T)(NULL);


static void vSavePanelAttribute_4K2K(void);
static void vFrcUnmute(void *pvArgs);
#endif	//4k2k

BOOL _fgESDProtect = FALSE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
#ifdef SUPPORT_DDDS_STEP_TRACKING
static void vDrvDDDSTrackingNormProc(void);
#endif

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
static INT32 i4OrigControlWord = 0x7fffffff;
#define STEP_CW (4)
#define STEP_CW_DELAY (50)
#define STEP_CW_DELAY_EPI (10)
#define STEP_CW_DELAY2 (250)
static UINT8 u1StepControlWord = 0;
static UINT8 u1StepDelay = 0;
static UINT8 u1StepDelay2 = 0;
static UINT8 u1DDDSLockStatus = 0;
static UINT8 u1SignalStatus = SV_VDO_NOSIGNAL;
UINT32 TargetErrlimit = ERRORLIMIT_INI;
#endif

#ifdef CC_SCPOS_3DTV_SUPPORT 
static void vDrvDisp3DModeCB(void);
static void vSavePanelAttribute(void);
static BOOL fgPDPL12R12 = ePanel_PDP_L1R1;
#ifdef DRV_SUPPORT_MT8283
BOOL fgForceChangeRegion = FALSE;
#endif
#endif

#endif

LCDTIMING_INFO_T lcdtimingInfo;

extern UINT8 u1FlipMirrorReset;    //flip&mirror is effective after resume


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
UINT16 wDrvGetOutputHTotal(void)
{
	if (fgApplyScalerSetData)
    {
		return ((UINT16)lcdtimingInfo.u4HTotal);
    }
    else
    {
	    if (fgIsMJCToOSTG())
	    {
			return ((UINT16) u4PscGetPanelHTotal());
	    }
		else
		{
			return (UINT16)u4ScpipGetPanelHtotal();
		}
	}
}

void wDrvSetOutputHTotal(UINT16 u2HTotal)
{
	#if defined(CC_MT5399)
    UINT16 u2OriHTotal;
    #endif 
    if (fgIsMJCToOSTG())
    {
        #if defined(CC_MT5399)
        u2OriHTotal = u4PscGetPanelHTotal();
        if(u2OriHTotal != u2HTotal)
        {
            bForceMJCMute = SV_TRUE;
            #ifndef CC_MTK_LOADER             
                _vDrvVideoSetMute(MUTE_MODULE_MJC, SV_VP_MAIN, 5, SV_TRUE);
		    #endif
        }
        #endif  
        vPscSetPanelHTotal(u2HTotal);
    }
    else
    {
        vScpipSetPanelHtotal(u2HTotal);
    }
#ifndef CC_MTK_LOADER    
    vVRMSetEventFlg(SV_VP_MAIN, VRM_EVENT_BY_PANEL);
    vVRMSetEventFlg(SV_VP_PIP , VRM_EVENT_BY_PANEL);
#endif

}

static UINT16 wDrvGetOutputHTotalStatus(void)
{
    if (fgIsMJCToOSTG())
    {
	    return ((UINT16) u4PscGetOutputHTotalStatus());
    }
	else
	{
        return ((UINT16) u4ScpipGetOutputHTotalStatus(0));
	}
}

UINT16 wDrvGetOutputVTotal(void)
{
	if (fgApplyScalerSetData)
    {
		return ((UINT16)lcdtimingInfo.u4VTotal);
    }
    else
    {
	    if (fgIsMJCToOSTG())
	    {
		    return ((UINT16) u4PscGetPanelVTotal());
	    }
		else
		{
	        return ((UINT16) u4ScpipGetPanelVtotal());
		}
	}
}

static UINT16 wDrvGetOutputVTotalStatus(void)
{
    if (fgIsMJCToOSTG())
    {
	    return ((UINT16) u4PscGetOutputVTotalStatus());
    }
	else
	{
        return ((UINT16) u4ScpipGetOutputVTotalStatus(0));
	}
}

void wDrvSetOutputVTotal(UINT16 u2VToal)
{

    if (fgIsMJCToOSTG())
    {
	    vPscSetPanelVTotal(u2VToal);
	
    }
	else
	{
	    vScpipSetPanelVtotal(u2VToal);
	}

	if((PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)||(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_MLVDS))
 	{
       vIO32WriteFldAlign(PIXEL_REORDER_15,1,RG_VWIDTH_R);
	   vIO32WriteFldAlign(PIXEL_REORDER_01,1,RG_VS_OUT_POL);
	   vIO32WriteFldAlign(PIXEL_REORDER_16,8,RG_VDE_VSTART_R);
	
       vIO32WriteFldAlign(VINTF_B3, 8, VINTF_PTGEN_V_START);
       vIO32WriteFldAlign(MUTE_PATTERN_05, 8, MUTE_V_START);
	   vIO32WriteFldAlign(EH_13,8,EH_PTGEN_V_START);	  
	}

#ifndef CC_MTK_LOADER    
    vVRMSetEventFlg(SV_VP_MAIN, VRM_EVENT_BY_PANEL);
    vVRMSetEventFlg(SV_VP_PIP , VRM_EVENT_BY_PANEL);
#endif

}

void wDrvSetOutputHActive(UINT16 u2HActive)
{
    if (fgIsMJCToOSTG())
    {
        vPscSetPanelWidth(u2HActive);
    }
    else
    {
        vScpipSetPanelWidth(u2HActive);
    }
    
#ifndef CC_MTK_LOADER    
    vVRMSetEventFlg(SV_VP_MAIN, VRM_EVENT_BY_PANEL);
    vVRMSetEventFlg(SV_VP_PIP , VRM_EVENT_BY_PANEL);
#endif

}

UINT16 wDrvGetOutputHActive(void)
{
	if (fgApplyScalerSetData)
    {
		return ((UINT16)lcdtimingInfo.u4HActive);
    }
    else
    {
	    if (fgIsMJCToOSTG())
	    {
		    return ((UINT16) u4PscGetPanelWidth());
	    }
		else
		{
	        return ((UINT16) u4ScpipGetPanelWidth());
		}
	}
}


void wDrvSetOutputVActive(UINT16 u2VActive)
{
    if (fgIsMJCToOSTG())
    {
        vPscSetPanelHeight(u2VActive);
    }
    else
    {
        vScpipSetPanelHeight(u2VActive);
    }

#ifndef CC_MTK_LOADER    
    vVRMSetEventFlg(SV_VP_MAIN, VRM_EVENT_BY_PANEL);
    vVRMSetEventFlg(SV_VP_PIP , VRM_EVENT_BY_PANEL);
#endif

}

UINT16 wDrvGetOutputVActive(void)
{
	return ((UINT16)lcdtimingInfo.u4VActive);
}

void vDrvSetHsyncFp(UINT16 u2Fp) 
{

    if (fgIsMJCToOSTG())
    {
        vPscSetPanelHSyncFp(u2Fp);
    }
    else
    {
        vScpipSetHsyncFp(u2Fp);
    }
}

void vDrvSetHsyncBp(UINT16 u2Bp) 
{
    if (fgIsMJCToOSTG())
    {
        vPscSetPanelHSyncBp(u2Bp);
    }
    else
    {
        vScpipSetHsyncBp(u2Bp);
    }
}

UINT16 wDrvGetHsyncBp(void) 
{
	if (fgApplyScalerSetData)
    {
		return ((UINT16)lcdtimingInfo.u4HBP);
    }
    else
    {
	    if (fgIsMJCToOSTG())
	    {
	        return ((UINT16) u4PscGetPanelHSyncBp());
	    }
	    else
	    {
	        return ((UINT16)u4ScpipGetHsyncBp());
	    }
	}
}
UINT16 wDrvGetHsyncFp(void)
{
	if (fgApplyScalerSetData)
    {
		return ((UINT16)lcdtimingInfo.u4HFP);
    }
    else
    {
		if (fgIsMJCToOSTG())
		{
		 return ((UINT16)u4PscGetPanelHSyncFp());
		} 
		else
		{
		 return ((UINT16)u4ScpipGetHsyncFp());
		}
	}
}

UINT16 wDrvGetHsyncWidth(void)
{
	UINT16 u2SyncWidth=0;
	
	u2SyncWidth =  wDrvGetOutputHTotal() - wDrvGetOutputHActive()- wDrvGetHsyncFp()-wDrvGetHsyncBp(); 	
	return ((UINT16)u2SyncWidth);			
}

void vDrvSetVsyncFp(UINT16 u2VFp)
{
    if (fgIsMJCToOSTG())
    {
        vPscSetPanelVSyncFp(u2VFp);
    } 
    else
    {
        vScpipSetVsyncFp(u2VFp);
    }
}

void vDrvSetVsyncBp(UINT16 u2VBp)
{
    if (fgIsMJCToOSTG())
    {
        vPscSetPanelVSyncBp(u2VBp);
    } 
    else
    {
        vScpipSetVsyncBp(u2VBp);
    }
}

UINT16 wDrvGetVsyncFp(void)
{
	if (fgApplyScalerSetData)
    {
		return ((UINT16)lcdtimingInfo.u4VFP);
    }
    else
    {
	    if (fgIsMJCToOSTG())
	    {
	        return ((UINT16)u4PscGetPanelVSyncFp());
	    } 
	    else
	    {
	        return ((UINT16)u4ScpipGetVsyncFp());
	    }
    }
}

UINT16 wDrvGetVsyncBp(void)
{
	if (fgApplyScalerSetData)
    {
		return ((UINT16)lcdtimingInfo.u4VBP);
    }
    else
    {
	    if (fgIsMJCToOSTG())
	    {
	        return ((UINT16)u4PscGetPanelVSyncBp());
	    } 
	    else
	    {
	        return ((UINT16)u4ScpipGetVsyncBp());
	    }
    }
}

UINT16 wDrvGetVsyncWidth(void)
{
	UINT16 u2SyncWidth=0;
	
	u2SyncWidth =  wDrvGetOutputVTotal() - wDrvGetOutputVActive()- wDrvGetVsyncFp()-wDrvGetVsyncBp(); 
 	return ((UINT16)u2SyncWidth);			
}

#ifndef CC_MTK_LOADER
#ifdef SUPPORT_PANEL_48HZ
/**
 * @brief fgDrvSupport24Hz

 * use variable to control the frame rate control under 24Hz

 * @param  void
 * @retval TRUE/FALSE
 */
UINT8 fgDrvSupport24Hz(void)
{
    if (IS_PANEL_2D_N_3D_L12R12)
    {
        if( bGetVideoDecType(SV_VP_MAIN) != SV_VD_DVI)			
        return (SV_FALSE);
		else
        return (SV_TRUE);			
    }
	else
	{
        return (SV_TRUE);
	}
}
#endif
#endif

/**
 * @brief bDrvCheckMTKGoodStatus

 * Check MTK Good Clock Lock or Unlock

 * @param  void
 * @retval TRUE/FALSE
 */
UINT8 bDrvCheckMTKGoodStatus(void)
{
    return (IO32ReadFldAlign(STA_DDDS_00, STA_DDDS_00_DDDS_LOCK));
}

UINT32 fgDrvSetMJCToOSTG(UINT32 fgOnOff)
{
    #ifdef CC_MT5399
    if (fgOnOff == IO32ReadFldAlign(OSTG_01, REG_MJC_IN_EN))
    {
        return FALSE;
    }

    vIO32WriteFldAlign(OSTG_01, fgOnOff, REG_MJC_IN_EN);
    vDrvSetPanelTiming();

	OSD_Base_SetHStart();

    return TRUE;
    #else
    return FALSE;
    #endif
}

UINT32 fgIsMJCToOSTG(void)
{
    #ifdef CC_MT5399
    return (IO32ReadFldAlign(OSTG_01, REG_MJC_IN_EN) == 1);
    #else
    return FALSE;
    #endif
}

 #ifndef CC_MTK_LOADER

#define SUPPORT_IN_LOW_FRAME_RATE 36
/**
 * @brief vDrvSetMTKGoodDclk

 * Set MTK Good Dclk Parameters

 * @param  wVERT_LINE: Input Total Lines in the Vertical Direction
 * @retval void
 */ 
void vDrvSetMTKGoodDclk(UINT16 u2InputVTotal, UINT8 bInputFrameRate) // Called by Video
{
    UINT32 DISPLAY_SIZE, HLEN_INT, HLEN_NUM;
    UINT32 u4HTotal, u4VTotal;
    #if ENABLE_DDDS2_FEEDBACK
     UINT8 u1VopllRatio;
    #endif

    if ((u2InputVTotal == 0) || (bInputFrameRate == 0))
    {
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return;
    }
	//for FP case, bInputFrameRate is half of real input video frame in vidoe path
	if(u4DrvTDTVGetDecTag3D(E_DEC_MOD_HDMI)==TDTV_TAG3D_FP )
	{
		bInputFrameRate =bInputFrameRate <<1;
	}
    
    u4HTotal = (UINT32)wDrvGetOutputHTotal();
    u4VTotal = (UINT32)wDrvGetOutputVTotal();
    DISPLAY_SIZE = u4HTotal * u4VTotal;

    DISPLAY_SIZE = DISPLAY_SIZE * bOUTPUT_REFRESH_RATE / bInputFrameRate;
    LOG(3, "5DDDS %dHz --> %dHz\n", bInputFrameRate, bOUTPUT_REFRESH_RATE);

    DISPLAY_SIZE = bDrvVideoIsSrcInterlace(SV_VP_MAIN) ? DISPLAY_SIZE * 2 : DISPLAY_SIZE;

    #if ENABLE_DDDS2_FEEDBACK
    if ((LVDS_OUTPUT_PORT == SINGLE_PORT)&&(IS_LVDS_HIGHSPEED)) u1VopllRatio=1;		
    else  if (LVDS_OUTPUT_PORT == SINGLE_PORT) u1VopllRatio=0;
    else  if ((LVDS_OUTPUT_PORT == DUAL_PORT)&&(IS_LVDS_HIGHSPEED)) u1VopllRatio=2;		
    else  if ((LVDS_OUTPUT_PORT == DUAL_PORT)&&(IS_PANEL_L12R12)) u1VopllRatio=2;			
    else  if (LVDS_OUTPUT_PORT == DUAL_PORT) u1VopllRatio=1;
    else  if ((LVDS_OUTPUT_PORT == FOUR_PORT)&&(IS_LVDS_HIGHSPEED)) u1VopllRatio=3;
    else  if ((LVDS_OUTPUT_PORT == FOUR_PORT)&&(IS_PANEL_L12R12)) u1VopllRatio=3;    
    else u1VopllRatio=2;
	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
    {
        if(u1VopllRatio == 1)
            DISPLAY_SIZE = DISPLAY_SIZE / 3;
        else if(u1VopllRatio == 2)
            DISPLAY_SIZE = DISPLAY_SIZE / 6;
            
    }
    else
    {
        DISPLAY_SIZE = DISPLAY_SIZE >> u1VopllRatio;
    }
    #endif

    #ifdef CC_DEBUG
    Printf("u1VopllRatio = %d\n",u1VopllRatio);
	#endif
    
    HLEN_INT = (UINT32) (DISPLAY_SIZE / u2InputVTotal);
    HLEN_NUM = DISPLAY_SIZE - (HLEN_INT * (UINT32) u2InputVTotal);
    LOG(6, "inVTaol: 0x%X, outV: 0x%X, outH: 0x%X, HLEN_INT: 0x%X, HLEN_NUM: 0x%X\n",
    u2InputVTotal, wDrvGetOutputVTotal(), wDrvGetOutputHTotal(), HLEN_INT, HLEN_NUM);
    
	#ifdef SUPPORT_DDDS_STEP_TRACKING
    vIO32WriteFldAlign(DDDS_03,_u4ErrorLimit,DDDS_03_DDDS_ERR_LIM);
	#endif
    vIO32WriteFldAlign(DDDS_02,(UINT16)u2InputVTotal,DDDS_02_HLEN_DEN);
    vIO32WriteFldAlign(DDDS_01,(UINT16)HLEN_NUM,DDDS_01_HLEN_NUM);    
    vIO32WriteFldAlign(DDDS_01,HLEN_INT,DDDS_01_HLEN_INT);

   // LOG(3, "HLEN=0x%x . [0x%x / 0x%x] \n",HLEN_INT, HLEN_NUM,u2InputVTotal);    
}
#endif // end of #ifndef CC_MTK_LOADER

UINT8 fgDisplayFreeRun = SV_FALSE;
void vDrvSetDisplayFreeRun(UINT8 bOn_Off)
{
	fgDisplayFreeRun = bOn_Off;
}

void vDrvSetDDDSCloseloop(UINT8 bOn_Off)
{
 static UINT8 fgCloseloop_2nd = 0;

 if ((IS_DISPLAY_FREERUN) || (fgDisplayFreeRun))
 {
	 vIO32WriteFldMulti(DDDS_00,P_Fld(0,DDDS_00_DISP_EN)|P_Fld(0,DDDS_00_FIX_FS_DDDS_SEL));
	 vIO32WriteFldAlign(DDDS_03, 1, DDDS_03_SPREAD_INIT);
     #ifdef CC_COPLAT_MT82
     vUtDelay2us(50);
     #else
     HAL_Delay_us(100);
     #endif
     vIO32WriteFldAlign(DDDS_03, 0, DDDS_03_SPREAD_INIT); 
 }
 else
 {
 	if (bOn_Off)
 	{
    	if(fgCloseloop_2nd)
    	{
			vIO32WriteFldMulti(DDDS_00,P_Fld(1,DDDS_00_DISP_EN)|P_Fld(1,DDDS_00_FIX_FS_DDDS_SEL));	
    	}
		else
		{
	    	// overcome the clock jump when 1st enter close loop
			vIO32WriteFldAlign(DDDS_00, 1, DDDS_00_DISP_EN);	
			HAL_Delay_us(20);
			vIO32WriteFldAlign(DDDS_00, 1, DDDS_00_FIX_FS_DDDS_SEL);
			fgCloseloop_2nd = 1;
		}
 	}
 	else
 	{
			vIO32WriteFldMulti(DDDS_00,P_Fld(0,DDDS_00_DISP_EN)|P_Fld(0,DDDS_00_FIX_FS_DDDS_SEL));
 	}
 }
}


/**
 * @brief vDrvSwitchMTKGoodDclk

 * MTK Good Dclk ON/OFF

 * @param  bOn_Off: SV_ON/SV_OFF
 * @retval void
 */
void vDrvSwitchMTKGoodDclk(UINT8 bOn_Off)
{
#ifndef CC_MTK_LOADER
#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
    UINT32 u4TmpCw;
    UINT8 u1Kp0=0;
    UINT8 u1Ki0=0;
#endif

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
    if((PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1) && (IS_STEP_CLOCK_CHG))
    {
        if(u1DDDSLockStatus != bOn_Off)
        {
            u1DDDSLockStatus = bOn_Off;
            u4TmpCw = IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW);
            u1StepControlWord = 0;
            u1StepDelay = 0;
            u1StepDelay2 = 0;
            if( i4OrigControlWord != _u4TargetCW )
            {
                i4OrigControlWord = _u4TargetCW;
            }
//                        if(TargetErrlimit >= 3)
                            vDrvSetErrorLimit(0);
        }
    }
    else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
    {
        if(u1DDDSLockStatus != bOn_Off)
        {
            u1DDDSLockStatus = bOn_Off;
            u1StepControlWord = 0;
            u1StepDelay = 0;
            u1StepDelay2 = 0;
            if( i4OrigControlWord != _u4TargetCW )
            {
                i4OrigControlWord = _u4TargetCW;
            }
        }
    }

#endif

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
        if (bOn_Off == SV_ON)
        {
          if(((PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1) && (IS_STEP_CLOCK_CHG))||
             (PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI))
          {
//            if( bGetVideoDecType(SV_VP_MAIN) != SV_VD_DVI)
//                vIO32WriteFldMulti(DDDS_00,P_Fld(1,DDDS_00_DISP_EN)|P_Fld(1,DDDS_00_FIX_FS_DDDS_SEL));
          }
          else
          {
             vDrvSetDDDSCloseloop(1);
          }
    
        }
#else
    if (bOn_Off == SV_ON)
    {
		vDrvSetDDDSCloseloop(1);
    }
#endif
    else
    {
#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
        if((PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1) && (IS_STEP_CLOCK_CHG))
        {
            u4TmpCw = IO32ReadFldAlign(STA_DDDS_00, STA_DDDS_00_DDDS_FREQ_CW);
            u1Ki0 = IO32ReadFldAlign(DDDS_02, DDDS_02_DDDS_KI_0);
            u1Kp0 = IO32ReadFldAlign(DDDS_02, DDDS_02_DDDS_KP_0);
            vIO32WriteFldMulti(DDDS_02, P_Fld(0,DDDS_02_DDDS_KI_0)|P_Fld(0,DDDS_02_DDDS_KP_0));
            vIO32WriteFldAlign(DDDS_00, u4TmpCw, DDDS_00_DDDS_FREQ_CW);
        }
#endif

        vDrvSetDDDSCloseloop(0); 

        vIO32WriteFldAlign(DDDS_03, 1, DDDS_03_SPREAD_INIT);
        #ifdef CC_COPLAT_MT82
        vUtDelay2us(50);
        #else
        HAL_Delay_us(100);
        #endif
        vIO32WriteFldAlign(DDDS_03, 0, DDDS_03_SPREAD_INIT);   

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
        if((PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1) && (IS_STEP_CLOCK_CHG))
        {
            vIO32WriteFldMulti(DDDS_02, P_Fld(u1Ki0,DDDS_02_DDDS_KI_0)|P_Fld(u1Kp0,DDDS_02_DDDS_KP_0));
        }
#endif
    }
    #endif //end of #ifndef CC_MTK_LOADER 
}

UINT32 u4GetPanelVSyncBp(void)
{
 UINT32 u4VPorchB;

 #ifdef SUPPORT_PANEL_CUSTOMER_SPEC 
 u4VPorchB = PANEL_GetVSyncBpByTiming(bSI_DISPLAY_DCLK_TYPE);
 #else 
 u4VPorchB = (V_POS < (0x400 - VS_WIDTH + 6))?((0x400 - V_POS - VS_WIDTH + 6)):(0);
 #endif

 return (u4VPorchB);
} 

// LVDS power------------------------------------------------------------------
void vDrvTurnOnPanel()
{
    do 
    { 
        UINT32 gpio, value; 
        if(DRVCUST_PanelQuery(ePanelPowerGpio, &gpio) == 0 && 
           DRVCUST_PanelQuery(ePanelPowerOnVal, &value) == 0)
        {
            vDrvGpioSet((INT32)gpio, (INT32)value);
        }
    } while (0);
}

void vDrvTurnOffPanel()
{
    do 
    { 
        UINT32 gpio, value; 
        if(DRVCUST_PanelQuery(ePanelPowerGpio, &gpio) == 0 && 
           DRVCUST_PanelQuery(ePanelPowerOnVal, &value) == 0)
        {
            vDrvGpioSet((INT32)gpio, (INT32)!value);
        }
    } while (0);
}
//-----------------------------------------------------------------------------
/** turn on/off panel lvds signal. (used by MTAL)
 *  @param fgEnable: Control mode. It could be TRUE or FALSE
 *  @return
 */
//-----------------------------------------------------------------------------

void vDrvLVDSSignalOnOff(BOOL fgOnOff)
{
    if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_LVDS)
    {
	if(fgOnOff)
	{
		vDrvLVDSOn();
	}
	else
	{
		vDrvLVDSOff();
	}
	}
}

//-----------------------------------------------------------------------------
/** turn on/off panel Vx1 signal. (used by MTAL)
 *  @param fgEnable: Control mode. It could be TRUE or FALSE
 *  @return
 */
//-----------------------------------------------------------------------------

void vDrvVByOneSignalOnOff(BOOL fgOnOff)
{
    if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
    {
		if(fgOnOff)
		{
			vDrvVByOneOn();
		}
		else
		{
			vDrvVByOneOff();
		}
	}
}

//-----------------------------------------------------------------------------
/** turn on/off panel lvds powe. (used by MTAL)
 *  @param fgEnable: Control mode. It could be TRUE or FALSE
 *  @return
 */
//-----------------------------------------------------------------------------

void vDrvLVDSPowerOnOff(BOOL fgOnOff)
{
	if(fgOnOff)
	{
		vDrvTurnOnPanel();
	}
	else
	{
		vDrvTurnOffPanel();
	}
}

void vDrvVsyncISRSetPosition(UINT16 u2INTRLine)
{
    vIO32WriteFldAlign(INTR_01, u2INTRLine, REG_INTR_LINE); 
}

void vDrvVsyncISRSetPosition_PDP(UINT16 u2INTRLine)
{
    vIO32WriteFldAlign(INTR_01, 1, REG_INTR_LINE_LATCH_OFF); 
    vIO32WriteFldAlign(INTR_01, u2INTRLine, REG_INTR_LINE); 
    vIO32WriteFldAlign(INTR_01, 0, REG_INTR_LINE_LATCH_OFF); 	
}

static UINT16 vDrvVsyncISRGetPosition(void)
{
    return(IO32ReadFldAlign(INTR_01, REG_INTR_LINE)); 
}

UINT8 u1DrvGetVsyncISRFCNTStatus(void)
{
    return(IO32ReadFldAlign(INTR_00, FCONT)); 
}

static void vDrvVsyncISRInit(UINT16 u2Vfp)
{
    if (IS_PANEL_L12R12)
    {
        vDrvVsyncISRSetPosition((wDrvGetOutputVTotal()/2)-u2Vfp); 
    }
    else
    {
        vDrvVsyncISRSetPosition(wDrvGetOutputVTotal()-u2Vfp); 
    }
}

static void vDrvVsyncISRInit_PDP(void)
{
    if (IS_PANEL_L12R12)
    {
        vDrvVsyncISRSetPosition_PDP(0); 
    }
}

void vDrvSetTCONIrqClear(UINT32 u4INTRSource)
{
    vIO32WriteFldAlign(INTR_02, u4INTRSource, C_INTR_CLEAR);
    vIO32WriteFldAlign(INTR_02, 0, C_INTR_CLEAR);
}

void vDrvGetTCONIrqStatus(UINT32 * u4INTRStatus)
{
    *u4INTRStatus = IO32ReadFldAlign(INTR_00, STATUS_INTR);
}

static void vDrvVsyncISRSetMask(UINT16 u2Mask)
{
    vIO32WriteFldAlign(INTR_02, u2Mask,C_INTR_MASK); 
}

static UINT16 vDrvVsyncISRGetMask(void)
{
    return(IO32ReadFldAlign(INTR_02, C_INTR_MASK)); 
}

void vDrvVsyncISRStatus(void)
{
    UINT16 u2MaskStatus;

	u2MaskStatus = vDrvVsyncISRGetMask();
		
	Printf("====Interrupt status====\n");
    Printf("[Interrupt Line] %d \n",vDrvVsyncISRGetPosition());
    Printf("[Interrupt Mask] 0x%x \n",u2MaskStatus);	
	if((u2MaskStatus >> 0) & 0x01)
	Printf("[Interrupt Mask] disable LR_RISIN \n");	
	if((u2MaskStatus >> 1) & 0x01)
	Printf("[Interrupt Mask] disable LR_FALLING \n");	
	if((u2MaskStatus >> 2) & 0x01)
	Printf("[Interrupt Mask] disable L_RISING \n");	
	if((u2MaskStatus >> 3) & 0x01)
	Printf("[Interrupt Mask] disable L_FALLING \n");	
	if((u2MaskStatus >> 4) & 0x01)
	Printf("[Interrupt Mask] disable R_RISING \n");	
	if((u2MaskStatus >> 5) & 0x01)
	Printf("[Interrupt Mask] disable R_FALLING \n");
	if((u2MaskStatus >> 6) & 0x01)
	Printf("[Interrupt Mask] disable OSTGLINE \n");
	if((u2MaskStatus >> 7) & 0x01)
	Printf("[Interrupt Mask] disable AMBILIT \n");
	
}

void vDrvVsyncISRSetting(UINT16 u2Line, UINT16 u2Mask)
{
    vDrvVsyncISRSetPosition(u2Line);
	vDrvVsyncISRSetMask(u2Mask);

	if ((u2Line>wDrvGetOutputVTotal())&&(!(IS_PANEL_L12R12)))
	{
     Printf("u2INTRLine(%d) over Panel V total (%d)\n",u2Line, wDrvGetOutputVTotal());
	 Printf("Interrupt will not active\n");
	}
}


static UINT8 wDrvGetScalerDISPR_INStatus(void)
{
    if (fgIsMJCToOSTG())
    {
	    return ((UINT8) u1PscGetInDispR());
    }
	else
	{
        return ((UINT8) u1ScpipGetInDispR());
	}
}

static UINT8 wDrvGetScalerDISPR_OUTStatus(void)
{
    if (fgIsMJCToOSTG())
    {
	    return ((UINT8) u1PscGetOutDispR());
    }
	else
	{
        return ((UINT8) u1ScpipGetOutDispR());
	}
}

static UINT8 wDrvGetLVDSDISPR_OUTStatus(void)
{
    return ((UINT8) IO32ReadFldAlign(LVDSB_REG14, ST_LVDS_DISP_R));
}

void vGetPostScalerStatus(void)
{
    UINT8 u1Cnt;

#ifndef CC_MTK_LOADER
#ifdef LVDS_ESD_PROTECT
#ifdef CC_MT5882
	Printf("_fgESDProtect=%d\n", _fgESDProtect);
#endif
#endif
#endif

    Printf("===== [Setting] ===== \n");
	Printf("[HSync] Htoal=%d Hactive=%d  Hfp= %d Hbp= %d Hwidth=%d\n", 
		   wDrvGetOutputHTotal(),
		   wDrvGetOutputHActive(), 
		   wDrvGetHsyncFp(), 
		   wDrvGetHsyncBp(),
           wDrvGetOutputHTotal()-wDrvGetOutputHActive()-wDrvGetHsyncFp()-wDrvGetHsyncBp()
	);
	Printf("[VSync] Vtoal=%d Vactive=%d  Vfp= %d Vbp= %d Vwidth=%d\n", 
		   wDrvGetOutputVTotal(),
		   wDrvGetOutputVActive(), 
		   wDrvGetVsyncFp(), 
		   wDrvGetVsyncBp(),
		   wDrvGetOutputVTotal()-wDrvGetOutputVActive()-wDrvGetVsyncFp()-wDrvGetVsyncBp()
	);
	if (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_48HZ)
	Printf("[Frame rate] SV_DCLK_48HZ\n");
	else if  (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_50HZ)
	Printf("[Frame rate] SV_DCLK_50HZ\n");
	else if  (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_60HZ)
	Printf("[Frame rate] SV_DCLK_60HZ\n");
	else if  (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_OTHERS)
	Printf("[Frame rate] SV_DCLK_OTHERS\n");
	else if  ( bSI_DISPLAY_DCLK_TYPE == SV_DCLK_UNKNOW)
	Printf("[Frame rate] SV_DCLK_UNKNOW\n");

	Printf("_fgPanelDynamicChg =%d\n", _fgPanelDynamicChg);
    Printf("_bForceFrameRatee =%d Hz\n", _bForceFrameRate);
    Printf("fgDisplayFreeRun =%d \n", fgDisplayFreeRun);	

    Printf("Output refresh rate =%d Hz\n", vDrvGetLCDFreq());	

    Printf("===== [Status] ===== \n");
	Printf("HTotal =%d\n", wDrvGetOutputHTotalStatus());
	Printf("VTotal =%d\n", wDrvGetOutputVTotalStatus());
	Printf("DISP_R_IN =%d\n", wDrvGetScalerDISPR_INStatus());
	Printf("DISP_R_OUT=%d\n", wDrvGetScalerDISPR_OUTStatus());	

    Printf("===== [OSTG Status] ===== \n");
	Printf("[Input]DISP_R=%d\n", IO32ReadFldAlign(DEBUG_01, DEBUG_01_PANEL_DISP_R));	
	Printf("[Input]HTotal =%d\n", IO32ReadFldAlign(DEBUG_02, DEBUG_02_INPUT_HTOTAL));
	Printf("[Input]VTotal =%d\n", IO32ReadFldAlign(DEBUG_02, DEBUG_02_INPUT_VTOTAL));

	Printf("[Color]HTotal =%d\n", IO32ReadFldAlign(TIMING_DETECTION_0, COLOR_HTOTAL_CNT));
	Printf("[Color]HActive =%d\n", IO32ReadFldAlign(TIMING_DETECTION_1, COLOR_HDE_CNT)); 
	Printf("[Color]VTotal =%d\n", IO32ReadFldAlign(TIMING_DETECTION_0, COLOR_VTOTAL_CNT));
	Printf("[Color]VActive =%d\n", IO32ReadFldAlign(TIMING_DETECTION_1, COLOR_VDE_CNT)); 
	
	vIO32WriteFldAlign(DEBUG_00, 1, DEBUG_00_R_DEBUG_OSTG_EN);
	HAL_Delay_us(20000);

    for (u1Cnt = 0; u1Cnt<4; u1Cnt++)
    {
	  vIO32WriteFldAlign(DEBUG_01, u1Cnt, DEBUG_01_R_HVM_INPUT_SEL);	
	  HAL_Delay_us(40000);

	  if (IO32ReadFldAlign(DEBUG_01,DEBUG_01_R_HVM_INPUT_SEL) == 0)
	  {
       Printf("The interface between OD to OSTG (default)\n");
	  }
	  else if (IO32ReadFldAlign(DEBUG_01,DEBUG_01_R_HVM_INPUT_SEL) == 1)
	  {
       Printf("The interface between OSTG to OD\n");
	  }	
	  else if (IO32ReadFldAlign(DEBUG_01,DEBUG_01_R_HVM_INPUT_SEL) == 2)
	  {
       Printf("The interface between MJC to OSTG\n");
	  }	
	  else if (IO32ReadFldAlign(DEBUG_01,DEBUG_01_R_HVM_INPUT_SEL) == 3)
	  {
       Printf("The input of OSTG (Note: this option will be fail when project exists MJC)\n");
	  }		  

	 Printf("[Output]HTotal =%d\n", IO32ReadFldAlign(DEBUG_03, DEBUG_03_HVM_HTOTAL_CNT));
	 Printf("[Output]H Active =%d\n", IO32ReadFldAlign(DEBUG_04, DEBUG_04_HVM_HDE_CNT));	 
	 
	 Printf("[Output]VTotal =%d\n", IO32ReadFldAlign(DEBUG_03, DEBUG_03_HVM_VTOTAL_CNT));	
	 Printf("[Output]V Active =%d\n", IO32ReadFldAlign(DEBUG_04, DEBUG_04_HVM_VDE_CNT));
    }

	vIO32WriteFldAlign(DEBUG_00, 0, DEBUG_00_R_DEBUG_OSTG_EN);

}


void vGetDISP_RStatus(void)
{
    Printf("==[DISP_R Setting - Normal Mode] == \n");
    if(IO32ReadFldAlign(LVDSB_REG06,RG_RES_FLD))
	Printf("DISP_R Encode in A Channel\n");		
    if(IO32ReadFldAlign(LVDSB_REG18,RG_RES_FIELD_L))
	Printf("DISP_R Encode in B Channel\n");	
    if(IO32ReadFldAlign(LVDSB_REG19,RG_RES_FIELD_U_4CH))
	Printf("DISP_R Encode in C Channel\n"); 	
    if(IO32ReadFldAlign(LVDSB_REG19,RG_RES_FIELD_L_4CH))
	Printf("DISP_R Encode in D Channel\n"); 
		
    Printf("==[DISP_R Setting - OSD Mode] == \n");
    if(IO32ReadFldAlign(LVDSB_REG08,RG_OSD_3D_IN_HS))
	Printf("DISP_R Encode in HSync\n");	
    if(IO32ReadFldAlign(LVDSB_REG08,RG_OSD_3D_IN_VS))
	Printf("DISP_R Encode in VSync\n");	
	
    Printf("==[DISP_R Status] == \n");
	Printf("[Scaler] DISP_R_IN =%d\n", wDrvGetScalerDISPR_INStatus());
	Printf("[Scaler] DISP_R_OUT=%d\n", wDrvGetScalerDISPR_OUTStatus());	
    Printf("[LVDS] DISP_R_OUT=%d\n", wDrvGetLVDSDISPR_OUTStatus());
}

void u1SetDISP_REncodeEn(UINT8 u1Enable)
{     
    vIO32WriteFldAlign(LVDSB_REG06,(u1Enable>>0 & 0x01),RG_RES_FLD);
    vIO32WriteFldAlign(LVDSB_REG18,(u1Enable>>1 & 0x01),RG_RES_FIELD_L);	
    vIO32WriteFldAlign(LVDSB_REG19,(u1Enable>>2 & 0x01),RG_RES_FIELD_U_4CH);	
    vIO32WriteFldAlign(LVDSB_REG19,(u1Enable>>3 & 0x01),RG_RES_FIELD_L_4CH);
    vIO32WriteFldAlign(LVDSB_REG08,(u1Enable>>4 & 0x01),RG_OSD_3D_IN_HS);
    vIO32WriteFldAlign(LVDSB_REG08,(u1Enable>>5 & 0x01),RG_OSD_3D_IN_VS);	
	if ((u1Enable>>4 & 0x01)==1 || (u1Enable>>5 & 0x01))
	{
	 vIO32WriteFldAlign(LVDSB_REG06,0x01,RG_RES_FLD);
	}
}

void u1SetDISP_REncodeEn_L4(BOOL bValue)
{     
    vIO32WriteFldAlign(LVDSB_REG06,bValue,RG_RES);
}

void u1SetDISP_REncodeEnHW_L4(BOOL bValue)
{     
	vIO32WriteFldAlign(LVDSB_REG19,bValue,RG_RES2_FLD);
	vIO32WriteFldAlign(LVDSB_REG19,bValue,RG_RES2_FLD_L);			
	vIO32WriteFldAlign(LVDSB_REG19,bValue,RG_RES2_FLD_L_4CH);
	vIO32WriteFldAlign(LVDSB_REG19,bValue,RG_RES2_FLD_U_4CH);	
}


static void vSetOSD_IndexEncodeEnable(BOOL bEn)
{     
	vIO32WriteFldAlign(LVDSB_REG19,bEn,RG_RES_OSD_INDEX);
	vIO32WriteFldAlign(LVDSB_REG19,bEn,RG_RES_OSD_INDEX_L);
	vIO32WriteFldAlign(LVDSB_REG19,bEn,RG_RES_OSD_INDEX_U_4CH);
	vIO32WriteFldAlign(LVDSB_REG19,bEn,RG_RES_OSD_INDEX_L_4CH);
}

/**
 * @brief u1SetDISP_REncode_L4

 * encode disp_r on Link 4 with register value

 * @param  void
 * @retval TRUE/FALSE
 */

void u1SetDISP_REncode_L4(void)
{     
    #ifdef CC_SCPOS_3DTV_SUPPORT
    static UINT8 u1LRResetFlag = 1;
    static UINT8 u13DCurDispR = 2;
    static UINT8 u13DPreDispR = 2;
    if(IS_DISPR_ENCODEL4)
    {
   	  	if ((BSP_GetIcVersion() == IC_VER_5398_AA)||(BSP_GetIcVersion() == IC_VER_5880_AA)||(BSP_GetIcVersion() == IC_VER_5860_AA))
   	  	{
		  	
        	u13DPreDispR = u13DCurDispR;
        	u13DCurDispR = IO32ReadFldAlign(LVDSB_REG14, ST_LVDS_DISP_R);

        	if (u1LRResetFlag && (u13DPreDispR == 0) && u13DCurDispR)
        	{
            	u1SetDISP_REncodeEn_L4(1);    
             	u1LRResetFlag = 0;
        	}
        	else
        	{
            	u1SetDISP_REncodeEn_L4(0);
            	u1LRResetFlag = 1;	  
        	}
   	  	}
    }
    #endif
}

void vGetLVDS_HVEncodeStatus(void)
{   
    Printf("==[LVDS H/V Encode Value] == \n");
	if (IO32ReadFldAlign(LVDSB_REG06,RG_VS_SEL))
	{
     Printf("[VSYNC Enocde] Vsync encode value=%d\n", IO32ReadFldAlign(LVDSB_REG06,RG_CNTLF));
	}
	if (IO32ReadFldAlign(LVDSB_REG06,RG_HS_SEL))
	{
     Printf("[HSYNC Enocde] Hsync encode value=%d\n", IO32ReadFldAlign(LVDSB_REG06,RG_CNTLE));
	}	
}

void u1SetLVDS_HVEncodeValue(UINT8 u1Enable)
{     
    vIO32WriteFldAlign(LVDSB_REG06,(u1Enable>>0 & 0x01),RG_VS_SEL);
    vIO32WriteFldAlign(LVDSB_REG06,(u1Enable>>1 & 0x01),RG_CNTLF);	
    vIO32WriteFldAlign(LVDSB_REG06,(u1Enable>>2 & 0x01),RG_HS_SEL);	
    vIO32WriteFldAlign(LVDSB_REG06,(u1Enable>>3 & 0x01),RG_CNTLE);	
}

BOOL fgIsVsyncPDPInit = SV_FALSE;
extern BOOL fgIsVsyncIsrStart;
extern UINT32 _u4VSyncCountSD;

extern UINT8 fgApplyScanPWMSetData;
extern UINT8 u1ScanPwmCrossFlg;
extern BOOL _fgVOPLLPowerOn;

void vDrvSetPanelTiming(void)
{
    UINT32 u4HSyncWidth, u4VSyncWidth, u4HPosition;
    UINT32 u4HFrontPorch, u4VFrontPorch;
    UINT32 u4HBackPorch, u4VBackPorch;
    UINT32 u4HTotal, u4VTotal;
	BOOL fgTimigChg = SV_FALSE;

    switch (bSI_DISPLAY_DCLK_TYPE)
    {
        case SV_DCLK_48HZ:
            #ifndef CC_MTK_LOADER
			if (SUPPORT_PANEL_48HZ)
			{
		        u4HPosition = H_POS_48HZ;
		        u4HTotal = HLEN_TOTAL_48HZ;
		        u4VTotal = VLEN_TOTAL_48HZ;
				break;
			}
            #endif

		#ifndef SUPPORT_DISP_48TO60
            bSI_DISPLAY_DCLK_TYPE = SV_DCLK_50HZ;
		#else
			bSI_DISPLAY_DCLK_TYPE = SV_DCLK_60HZ;
			goto OUTPUT_60HZ;
		#endif

        case SV_DCLK_50HZ:
            u4HPosition = H_POS_50HZ;
            u4HTotal = HLEN_TOTAL_50HZ;
            u4VTotal = VLEN_TOTAL_50HZ;
            break;

        case SV_DCLK_OTHERS:
        case SV_DCLK_60HZ:
        default:
#ifdef SUPPORT_DISP_48TO60
	OUTPUT_60HZ:
#endif
            u4HPosition = H_POS_60HZ;
            u4HTotal = HLEN_TOTAL_TYPI;
            u4VTotal = VLEN_TOTAL_TYPI;
            break;
    }

    #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
    u4HSyncWidth = PANEL_GetHSyncWidthByTiming(bSI_DISPLAY_DCLK_TYPE);
    u4VSyncWidth = PANEL_GetVSyncWidthByTiming(bSI_DISPLAY_DCLK_TYPE);
    u4VBackPorch = PANEL_GetVSyncBpByTiming(bSI_DISPLAY_DCLK_TYPE);
    u4HPosition = PANEL_GetHPositionByTiming(bSI_DISPLAY_DCLK_TYPE);
    #else
    u4HSyncWidth = PANEL_GetHSyncWidth();
    u4VSyncWidth = PANEL_GetVSyncWidth();
    u4VBackPorch = u4GetPanelVSyncBp();
    #endif

    u4HFrontPorch = (u4HPosition > 0x800) ? (u4HPosition - 0x800) : 0;
    u4HBackPorch = u4HTotal - wDISPLAY_WIDTH - u4HFrontPorch - u4HSyncWidth;

    if (PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI || PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_MLVDS)
    {
        u4VBackPorch = 7;
        u4VSyncWidth = 1;
    }

    u4VFrontPorch = u4VTotal - wDISPLAY_HEIGHT - u4VSyncWidth - u4VBackPorch;

    {
        #define ODFPLimit 4
        #define REORDERBPLimit 4
        u4HFrontPorch = (u4HFrontPorch < ODFPLimit) ? ODFPLimit : u4HFrontPorch;
        u4HBackPorch = (u4HBackPorch < REORDERBPLimit) ? REORDERBPLimit : u4HBackPorch;
    }
    if((lcdtimingInfo.u4HTotal != u4HTotal) ||
       (lcdtimingInfo.u4VTotal != u4VTotal) ||
       (lcdtimingInfo.u4HFP != u4HFrontPorch) || (lcdtimingInfo.u4HBP != u4HBackPorch) ||
       (lcdtimingInfo.u4VFP != u4VFrontPorch) || (lcdtimingInfo.u4VBP != u4VBackPorch) )
    {
        #ifndef CC_MTK_LOADER
        CRIT_STATE_T csState;
        csState = x_crit_start();
        #endif
        fgTimigChg = SV_TRUE;;
        lcdtimingInfo.u4HTotal = u4HTotal;
        lcdtimingInfo.u4HActive = wDISPLAY_WIDTH;
        lcdtimingInfo.u4HFP = u4HFrontPorch;
        lcdtimingInfo.u4HBP = u4HBackPorch;

        lcdtimingInfo.u4VTotal = u4VTotal;
        lcdtimingInfo.u4VActive = wDISPLAY_HEIGHT;
        lcdtimingInfo.u4VFP = u4VFrontPorch;
        lcdtimingInfo.u4VBP = u4VBackPorch;
        #ifndef CC_MTK_LOADER
        x_crit_end(csState);
        #endif
    }
	else
	{
	    fgTimigChg = SV_FALSE;
	}

    if ((IS_PANEL_L12R12) && (!fgIsVsyncPDPInit))
    {
        vDrvVsyncISRInit_PDP();
        fgIsVsyncPDPInit = SV_TRUE;
    }

    // When MJC on/off update PSC Global to  FSC Global 
    #ifndef CC_MTK_LOADER
    #ifdef CC_MT5399
    if (!fgIsMJCToOSTG())
    {
       vDrvUpdateFSCTimeGenPrm(); 
    }
    #endif
    #endif


    #ifndef CC_MTK_LOADER
    if ((fgIsVsyncIsrStart) && (_fgVOPLLPowerOn))
    {
        if(fgTimigChg)
    	{ 
    		fgApplyScalerSetData = 1;
		    fgApplyScanPWMSetData = 1;
		    u1ScanPwmCrossFlg = 2;
		    LOG(2, "fgApplyScalerSetData = SV_TRUE, _u4VSyncCountSD=%d \n", _u4VSyncCountSD);
    	}
    }	
    else
    #endif
    {
        vDrvSetLCDTiming_SETDATA();
        vDrvFireImportPortection();
    }
}

#ifdef SUPPORT_PANEL_SCAN_PWM
extern UINT32 _u4BrightLevel;
#endif

extern void vDrvTCONISRSetLineCounter(UINT32 u4VFp, UINT32 u4VActive, UINT32 u4VBp);
void vDrvSetLCDTiming_SETDATA(void)
{
	LOG(2,"vDrvSetLCDTiming_SETDATA\n");

	wDrvSetOutputHTotal(lcdtimingInfo.u4HTotal);
    wDrvSetOutputHActive(lcdtimingInfo.u4HActive);
	vDrvSetHsyncFp(lcdtimingInfo.u4HFP);
	vDrvSetHsyncBp(lcdtimingInfo.u4HBP);
	#ifdef CC_MT5399
    #ifndef CC_MTK_LOADER
	vDrvMJCSetOutputHTotal(lcdtimingInfo.u4HTotal);
    vDrvMJCSetOutputVTotal(lcdtimingInfo.u4VTotal);
    #endif
	#endif
	
	wDrvSetOutputVTotal(lcdtimingInfo.u4VTotal);	
    if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
    {        
        wDrvSetOutputVActive(lcdtimingInfo.u4VActive+2);
        vDrvSetVsyncFp(lcdtimingInfo.u4VFP-2);
        #ifdef CC_MT5399
		#ifndef CC_MTK_LOADER
        vDrvMJCSetVsyncDelay(lcdtimingInfo.u4VFP - 2, lcdtimingInfo.u4VTotal);
		#endif
		#endif
    }
    else
    {
        wDrvSetOutputVActive(lcdtimingInfo.u4VActive);
        vDrvSetVsyncFp(lcdtimingInfo.u4VFP);
		#ifdef CC_MT5399
		#ifndef CC_MTK_LOADER
        vDrvMJCSetVsyncDelay(lcdtimingInfo.u4VFP, lcdtimingInfo.u4VTotal);
		#endif
		#endif
    }
	#ifdef CC_MT5399
    #ifndef CC_MTK_LOADER
	//vIO32WriteFldAlign(MJC_SYS_CLK_CTRL, SV_ON, MJC_OUTPUT_RST);
   // vIO32WriteFldAlign(MJC_SYS_CLK_CTRL, SV_OFF, MJC_OUTPUT_RST);
    #endif
	#endif
    vDrvSetVsyncBp(lcdtimingInfo.u4VBP);

    // ISR Setting
    vDrvVsyncISRInit(lcdtimingInfo.u4VFP);
    vDrvTCONISRSetLineCounter(lcdtimingInfo.u4VFP, lcdtimingInfo.u4VActive, lcdtimingInfo.u4VBP);

    // For 3D_Glasses Control - software work around
    vIO32WriteFldAlign(VINTF_P8, lcdtimingInfo.u4VTotal,  VINTF_REYE_WIDTH); 
    vIO32WriteFldAlign(VINTF_P9, 1,  VINTF_REYE_RST_MODE);    
    vIO32WriteFldAlign(VINTF_P10, lcdtimingInfo.u4VTotal,  VINTF_LEYE_WIDTH); 
    vIO32WriteFldAlign(VINTF_P11, 1,  VINTF_LEYE_RST_MODE);

    vErrorHandleSetByTiming();
	// osd Hstart setting 
	OSD_Base_SetHStart();

#ifdef CC_LGE_PROTO_PCBA
	#ifndef CC_MTK_LOADER
	vDrvPWM_ApplyParamSet();
	#endif
#else
	// update dimming frequency
#ifdef SUPPORT_PANEL_SCAN_PWM
	vDrvSetScanPWMLatchMode(SCAN_PWM_LATCH_MODE_SETDATA,SCAN_PWM_LATCH_MODE_OFF,SCAN_PWM_ALIGN_MODE_VSYNC);  // Set data
    LOG(3, "_|-|_ vDrvSetLCDTiming_SETDATA Bright:%d%\n", _u4BrightLevel);
	BACKLT_SetDimming(BACKLT_GetDimmingFrequency(), _u4BrightLevel);
#ifndef CC_MTK_LOADER
    DRVCUST_UpdateBacklightState(BL_STATE_OUTPUT_FR_CHANGE, _u4BrightLevel);
#endif
#endif
#endif
	#ifndef CC_MTK_LOADER
	if(IS_PANEL_L12R12)
    {
	    Panel_3DGlassesCTL(e3DGlasses_P_3DBL,e3DGlasses_S_BL,e3DGlasses_POl_Bypass,e3DGlasses_POS_1stR,0,w3DGlassesDelay);
	    Panel_3DGlassesDutyCTL(e3DGlasses_S_BL,50);
	}
	#endif
}

/**
 * @brief vDrvOutputStageInit

 * Init the Register Settings of Output Stage Logic

 * @param  void
 * @retval void
 */

void vDrvVideoDitherInit(void)
{
    // DITHER
    UINT32 u4PanelDither = PANEL_GetDither();

    UINT32 u4Round, u4RDither, u4EDither, u4LFSR, u4Subpxl, u4LsbOff;   

    // 1. OSTG Dither
    // 1.1 Set Dither Depth
    if (DISP_BIT == DISP_30BIT)
    {
#ifndef CC_MTK_LOADER                
        if (_ODParam.u1ODCurrentONOFF == SV_TRUE)
#else        
        if(0)
#endif
        {        
            // Front Dither
            vIO32WriteFldAlign(OS_DITHER_02, 1, DRMOD_R);          // 12 to 10 Dither Mode
            vIO32WriteFldAlign(OS_DITHER_00, 2, REG_TABLE_EN);       // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0
            vIO32WriteFldAlign(OS_DITHER_00, 2, REG_FPHASE_CTRL);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)      
            // Post Dither
            vIO32WriteFldAlign(OS_DITHER_05, 1, REG_DRMOD_R_POST);     // 12 to 10 Dither Mode
            vIO32WriteFldAlign(OS_DITHER_03, 2, REG_TABLE_EN_POST);  // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0
            vIO32WriteFldAlign(OS_DITHER_03, 2, REG_FPHASE_CTRL_POST);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)                                                                        
        }
        else
        {
            // Front Dither
            vIO32WriteFldAlign(OS_DITHER_02, 1, DRMOD_R);          // 12 to 10 Dither Mode
            vIO32WriteFldAlign(OS_DITHER_00, 2, REG_TABLE_EN);       // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0
            vIO32WriteFldAlign(OS_DITHER_00, 2, REG_FPHASE_CTRL);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)      

            // Post Dither (Off When OD Disabled)
            vIO32WriteFldAlign(OS_DITHER_05, 0, REG_DRMOD_R_POST);     // 12 to 10 Dither Mode
            vIO32WriteFldAlign(OS_DITHER_03, 0, REG_TABLE_EN_POST);  // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0
            vIO32WriteFldAlign(OS_DITHER_03, 2, REG_FPHASE_CTRL_POST);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)     
        }        
    }
    else if (DISP_BIT == DISP_24BIT)
    {
#ifndef CC_MTK_LOADER                
        if (_ODParam.u1ODCurrentONOFF == SV_TRUE)
#else        
        if(0)
#endif
        {
            // Front Dither (12 to 10)
            vIO32WriteFldAlign(OS_DITHER_02, 1, DRMOD_R);          // 12 to  8 Dither Mode
            vIO32WriteFldAlign(OS_DITHER_00, 2, REG_TABLE_EN);       // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0                                                                        
            vIO32WriteFldAlign(OS_DITHER_00, 2, REG_FPHASE_CTRL);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)   

            // Post Dither (10 to 8)
            vIO32WriteFldAlign(OS_DITHER_05, 2, REG_DRMOD_R_POST);     // 12 to  8 Dither Mode
            vIO32WriteFldAlign(OS_DITHER_03, 1, REG_TABLE_EN_POST);  // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0                                                                        
            vIO32WriteFldAlign(OS_DITHER_03, 1, REG_FPHASE_CTRL_POST);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)                                                                
                                                                    
        }
        else
        {
            // Front Dither (12 to 8) 
            vIO32WriteFldAlign(OS_DITHER_02, 2, DRMOD_R);          // 12 to  8 Dither Mode
            vIO32WriteFldAlign(OS_DITHER_00, 1, REG_TABLE_EN);       // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0                                                                        
            vIO32WriteFldAlign(OS_DITHER_00, 1, REG_FPHASE_CTRL);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)   
            // Post Dither (Off When OD Disabled)
            vIO32WriteFldAlign(OS_DITHER_05, 0, REG_DRMOD_R_POST);     // 12 to  8 Dither Mode
            vIO32WriteFldAlign(OS_DITHER_03, 0, REG_TABLE_EN_POST);  // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0                                                                        
            vIO32WriteFldAlign(OS_DITHER_03, 1, REG_FPHASE_CTRL_POST);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)                                                                
                                                             

        }
    }
	else if (DISP_BIT == DISP_18BIT)
	{
#ifndef CC_MTK_LOADER				 
		if (_ODParam.u1ODCurrentONOFF == SV_TRUE)
#else		 
		if(0)
#endif
		{
			// Front Dither (12 to 10)
			vIO32WriteFldAlign(OS_DITHER_02, 1, DRMOD_R);		   // 12 to  10 Dither Mode
			vIO32WriteFldAlign(OS_DITHER_00, 2, REG_TABLE_EN);	   // active bit of magic table for running order.
																   //  01: phase active in bit3 ~ bit0 (12 to 8)
																   //  10: phase active in bit1 ~ bit0 (12 to 10)
																   //  else phase active in bit5 ~ bit0 																	   
			vIO32WriteFldAlign(OS_DITHER_00, 2, REG_FPHASE_CTRL);  // phase range select.
																   //  00 : 0~63. (12 to 6)
																   //  01 : 0~16. (12 to 8)
																   //  10 : 0~3.  (12 to 10)
			vIO32WriteFldAlign(OS_DITHER_00, 4, DITHER_FPHASE);    // Running order dithering frame phase increment.
																   // 1. If ROUND_EN = 1, the output will only rounding only 
																   // (no matter what RDITHER_EN and EDITHER_EN setting)
																   // 2. RDITHER_EN and EDITHER_EN can work together.	
			
			// Post Dither (10 to 6)
			vIO32WriteFldAlign(OS_DITHER_05, 3, REG_DRMOD_R_POST);	   // 12 to  6 Dither Mode
			vIO32WriteFldAlign(OS_DITHER_03, 0, REG_TABLE_EN_POST);    // active bit of magic table for running order.
																	   //  01: phase active in bit3 ~ bit0 (12 to 8)
																	   //  10: phase active in bit1 ~ bit0 (12 to 10)
																	   //  else phase active in bit5 ~ bit0 																	   
			vIO32WriteFldAlign(OS_DITHER_03, 0, REG_FPHASE_CTRL_POST); // phase range select.
																	   //  00 : 0~63. (12 to 6)
																	   //  01 : 0~16. (12 to 8)
																	   //  10 : 0~3.  (12 to 10) 
			vIO32WriteFldAlign(OS_DITHER_03, 3, REG_FPHASE_POST);	   // Running order dithering frame phase increment.
																	   // 1. If ROUND_EN = 1, the output will only rounding only 
																	   // (no matter what RDITHER_EN and EDITHER_EN setting)
																	   // 2. RDITHER_EN and EDITHER_EN can work together.  
		}
		else
		{
			// Front Dither (12 to 6) 
			vIO32WriteFldAlign(OS_DITHER_02, 3, DRMOD_R);		   // 12 to  8 Dither Mode
			vIO32WriteFldAlign(OS_DITHER_00, 0, REG_TABLE_EN);	   // active bit of magic table for running order.
																   //  01: phase active in bit3 ~ bit0 (12 to 8)
																   //  10: phase active in bit1 ~ bit0 (12 to 10)
																   //  else phase active in bit5 ~ bit0 																	   
			vIO32WriteFldAlign(OS_DITHER_00, 0, REG_FPHASE_CTRL);  // phase range select.
																   //  00 : 0~63. (12 to 6)
																   //  01 : 0~16. (12 to 8)
																   //  10 : 0~3.  (12 to 10) 
			vIO32WriteFldAlign(OS_DITHER_00, 4, DITHER_FPHASE);    // Running order dithering frame phase increment.
																   // 1. If ROUND_EN = 1, the output will only rounding only 
																   // (no matter what RDITHER_EN and EDITHER_EN setting)
																   // 2. RDITHER_EN and EDITHER_EN can work together.
			// Post Dither (Off When OD Disabled)
			vIO32WriteFldAlign(OS_DITHER_05, 0, REG_DRMOD_R_POST);	   // 12 to  8 Dither Mode
			vIO32WriteFldAlign(OS_DITHER_03, 0, REG_TABLE_EN_POST);    // active bit of magic table for running order.
																	   //  01: phase active in bit3 ~ bit0 (12 to 8)
																	   //  10: phase active in bit1 ~ bit0 (12 to 10)
																	   //  else phase active in bit5 ~ bit0 																	   
			vIO32WriteFldAlign(OS_DITHER_03, 0, REG_FPHASE_CTRL_POST); // phase range select.
																	   //  00 : 0~63. (12 to 6)
																	   //  01 : 0~16. (12 to 8)
																	   //  10 : 0~3.  (12 to 10) 
			vIO32WriteFldAlign(OS_DITHER_03, 3, REG_FPHASE_POST);	   // Running order dithering frame phase increment.
																	   // 1. If ROUND_EN = 1, the output will only rounding only 
																	   // (no matter what RDITHER_EN and EDITHER_EN setting)
																	   // 2. RDITHER_EN and EDITHER_EN can work together.  
		}
	}
	else
	{
		Printf("DISP_BIT=%d error\n", DISP_BIT);
	}


    // 1.2 Set Front Dither Type
    if (u4PanelDither != 0xffffffff)
    {
        u4LsbOff = (u4PanelDither & OSTG_LSBOFF_FUNCTION)? 1: 0; 
        u4Round = (u4PanelDither & OSTG_ROUND_FUNCTION)? 1: 0;
        u4RDither = (u4PanelDither & OSTG_RDITHER_FUNCTION)? 1 : 0;
        u4EDither = (u4PanelDither & OSTG_ERROR_DITHER_FUNCTION)? 1 : 0;
        u4LFSR = (u4PanelDither & OSTG_LFSR_FUNCTION)? 1 : 0;
        u4Subpxl = (u4PanelDither & OSTG_SUB_PIXEL_FUNCTION)? 1 : 0;
    }
    else    // No Define
    {
        u4LsbOff = 1;
        u4Round = 0;
        u4RDither = 1;
        u4EDither = 0;
        u4LFSR = 0;        
        u4Subpxl = 1;        
    }

    vIO32WriteFldAlign(OS_DITHER_02, u4LsbOff, DITHER_LSB_OFF);
    vIO32WriteFldAlign(OS_DITHER_00, u4Round, DITHER_ROUND_EN);
    vIO32WriteFldAlign(OS_DITHER_00, u4RDither, RDITHER_EN);        
    vIO32WriteFldAlign(OS_DITHER_00, u4EDither, EDITHER_EN);
    vIO32WriteFldAlign(OS_DITHER_00, u4LFSR, DITHER_LFSR_EN);      
    vIO32WriteFldAlign(OS_DITHER_00, u4Subpxl, REG_SUBPIX_EN);    

    // 1.3 Common Dither Setting
    vIO32WriteFldAlign(OS_DITHER_00, 0, REG_SUB_R);
    vIO32WriteFldAlign(OS_DITHER_00, 1, REG_SUB_G);        
    vIO32WriteFldAlign(OS_DITHER_00, 2, REG_SUB_B);
    
    vIO32WriteFldAlign(OS_DITHER_00, SV_TRUE, REG_FPHASE_EN);    
    vIO32WriteFldAlign(OS_DITHER_00, 0, REG_FPHASE_R);           // phase select for running order.
                                                                //  0: internal control register from reg_fphase[5:0]
                                                                //  1: LFSR

    vIO32WriteFldAlign(OS_DITHER_00, 3, DITHER_FPHASE);             // Running order dithering frame phase increment.
                                                                // 1. If ROUND_EN = 1, the output will only rounding only 
                                                                // (no matter what RDITHER_EN and EDITHER_EN setting)
                                                                // 2. RDITHER_EN and EDITHER_EN can work together.
    vIO32WriteFldAlign(OS_DITHER_00, 1, REG_FPHASE_SEL);         // fphase XOR function bit. Only bit2,bit4,bit6 is available.                                                                
    vIO32WriteFldAlign(OS_DITHER_00, 1, REG_FPHASE_BIT);         // fphase_bit XOR bus number. 
                                                                // Note: this function will check reg_fphase_sel.
                                                                // Only 0,1,2,4,6 is available."                     

    // 2. OD Dither
    // 2.1 Set Dither Type
    if (u4PanelDither != 0xffffffff)
    {
#ifndef CC_MTK_LOADER                
        if (_ODParam.u1ODCurrentONOFF == SV_TRUE)
#else        
        if(0)
#endif
        {        
            u4LsbOff = (u4PanelDither & TCON_LSBOFF_FUNCTION)? 1: 0; 
            u4Round = (u4PanelDither & TCON_ROUND_FUNCTION)? 1: 0;
            u4RDither = (u4PanelDither & TCON_RDITHER_FUNCTION)? 1 : 0;
            u4EDither = (u4PanelDither & TCON_ERROR_DITHER_FUNCTION)? 1 : 0;
            u4LFSR = (u4PanelDither & TCON_LFSR_FUNCTION)? 1 : 0;
            u4Subpxl = (u4PanelDither & TCON_SUB_PIXEL_FUNCTION)? 1 : 0;
        }
        else
        {
            u4LsbOff = 0;
            u4Round = 0;
            u4RDither = 0;
            u4EDither = 0;
            u4LFSR = 0;        
            u4Subpxl = 0;  
        }
    }
    else    // No Define
    {
#ifndef CC_MTK_LOADER                
        if (_ODParam.u1ODCurrentONOFF == SV_TRUE)
#else        
        if(0)
#endif
        {        
            u4LsbOff = 1;
            u4Round = 0;
            u4RDither = 1;
            u4EDither = 0;
            u4LFSR = 0;        
            u4Subpxl = 1;        
        }
        else
        {
            u4LsbOff = 0;
            u4Round = 0;
            u4RDither = 0;
            u4EDither = 0;
            u4LFSR = 0;        
            u4Subpxl = 0;  
        }
    }

    vIO32WriteFldAlign(OS_DITHER_05, u4LsbOff, REG_LSB_OFF_POST);
    vIO32WriteFldAlign(OS_DITHER_03, u4Round, REG_ROUND_EN_POST);
    vIO32WriteFldAlign(OS_DITHER_03, u4RDither, REG_RDITHER_EN_POST);        
    vIO32WriteFldAlign(OS_DITHER_03, u4EDither, REG_EDITHER_EN_POST);
    vIO32WriteFldAlign(OS_DITHER_03, u4LFSR, REG_LFSR_EN_POST);      
    vIO32WriteFldAlign(OS_DITHER_03, u4Subpxl, REG_SUBPIX_EN_POST);    

    // 2.2 Common Dither Setting
    vIO32WriteFldAlign(OS_DITHER_03, 0, REG_SUB_R_POST);
    vIO32WriteFldAlign(OS_DITHER_03, 1, REG_SUB_G_POST);        
    vIO32WriteFldAlign(OS_DITHER_03, 2, REG_SUB_B_POST);
    
    vIO32WriteFldAlign(OS_DITHER_03, SV_TRUE, REG_FPHASE_EN_POST);    
    vIO32WriteFldAlign(OS_DITHER_03, 0, REG_FPHASE_R_POST);      // phase select for running order.
                                                                //  0: internal control register from reg_fphase[5:0]
                                                                //  1: LFSR

    vIO32WriteFldAlign(OS_DITHER_03, 3, REG_FPHASE_POST);        // Running order dithering frame phase increment.
                                                                // 1. If ROUND_EN = 1, the output will only rounding only 
                                                                // (no matter what RDITHER_EN and EDITHER_EN setting)
                                                                // 2. RDITHER_EN and EDITHER_EN can work together.
    vIO32WriteFldAlign(OS_DITHER_03, 1, REG_FPHASE_SEL_POST);    // fphase XOR function bit. Only bit2,bit4,bit6 is available.                                                                
    vIO32WriteFldAlign(OS_DITHER_03, 1, REG_FPHASE_BIT_POST);    // fphase_bit XOR bus number. 
                                                                // Note: this function will check reg_fphase_sel.
                                                                // Only 0,1,2,4,6 is available."    


#ifdef __MODEL_slt__    
    vIO32WriteFldAlign(OS_DITHER_02, 0, DRMOD_R);
#endif
}


void vDrvOutputStageInit(void)
{

    #ifdef CC_SCPOS_3DTV_SUPPORT
	UINT32 u4EnPair;
    #endif

    #if (SRGB_FUNCTION_EN == SRGB_EN_ON)
    {
        vIO32WriteFldAlign(OSTG_PTGEN_00, 1, FLD_DEN_EN);
    }
    #else
    {
        vIO32WriteFldAlign(OSTG_PTGEN_00, 0, FLD_DEN_EN);
    }
    #endif

    #if (DEN_TYPE == DEN_ACIVE)
    {
        vIO32WriteFldAlign(OSTG_PTGEN_00, 1, FLD_DENTPY);
    }
    #else
    {
        vIO32WriteFldAlign(OSTG_PTGEN_00, 0, FLD_DENTPY);
    }
    #endif

    #if (DEN_POLARITY == DEN_LOW)
    {
        vIO32WriteFldAlign(OSTG_PTGEN_00, 1, FLD_DENP);
    }
    #else
    {
        vIO32WriteFldAlign(OSTG_PTGEN_00, 0, FLD_DENP);
    }
    #endif

    vIO32WriteFldAlign(OSTG_PTGEN_00, 0, FLD_HSYNCP);
    vIO32WriteFldAlign(OSTG_PTGEN_00, 0, FLD_VSYNCP);

//    vDrvVideoDitherInit();

    // Config HV alignment
    //vIO32WriteFldAlign(OUTSTG_OS_08, 3, OS_08_REG_VSYNC_OUT_SEL);

    if ((DISP_BIT == DISP_30BIT)
        #ifdef DRV_SUPPORT_EXTMJC
        || (IS_COMPANION_CHIP_ON() && (vDrvGetCurLvdsMode() !=  eLvdsVidNormal))
        #endif
        )
    {
        vIO32WriteFldAlign(LVDSB_REG06, 1, RG_10B_EN);                
    }
    else
    {
        vIO32WriteFldAlign(LVDSB_REG06, 0, RG_10B_EN);                
    }

#ifdef CC_SCPOS_3DTV_SUPPORT
		if (DRVCUST_PanelQuery(eDISP_RLVDSEncodeLink, &u4EnPair) == 0)
		{
		  u1SetDISP_REncodeEn((UINT8)u4EnPair);
		}	 
		else
		{
		 vIO32WriteFldAlign(LVDSB_REG06, 0x1, RG_RES_FLD);
		} 
		vIO32WriteFldAlign(OSTG_01, 0x1, REG_3DTV_OUT_SEL);
		if(IS_DISPR_ENCODEL4)
		u1SetDISP_REncodeEnHW_L4(1);
		else
		u1SetDISP_REncodeEnHW_L4(0);			
#endif

		if(IS_SUPPORT_OSD_INDEX)
		vSetOSD_IndexEncodeEnable(1);
		else
		vSetOSD_IndexEncodeEnable(0);
		

    // scaler timing initial
    /*  cobra don't have SET_DATA_POS_SEL
	if (fgIsMJCToOSTG())
    vIO32WriteFldAlign(PSPIP_OA_07, 1, PSPIP_OA_07_SCALER_SET_DATA_POS_SEL);
    else
    vIO32WriteFldAlign(SCPIP_PIP_OA_07, 1 ,PIP_OA_07_SCALER_SET_DATA_POS_SEL);			
    */
    // local dimming initialization
    #ifdef CC_MTK_LOADER
    // init local dimming panel with default setting.
    #else
    
    if (fgIsSWRegInit()!=SV_FALSE)
    {
        if(PANEL_IsSupportLocalDimming())
        {
            if(bDrvLcdimSpiInit() == SV_FAIL)
            {
                bSupportLocalDimming = SV_FALSE;
            }        
            else    // PQ setting is only initialized when SPI is ready
            {
                if(bDrvLcDimPanelInit() == SV_SUCCESS)
                {
                    vDrvLcDimQualityInit();
                    bSupportLocalDimming = SV_TRUE;
                }
                else
                {
                    bSupportLocalDimming = SV_FALSE;
                }
            }
        }    
        else
        {
            bSupportLocalDimming = SV_FALSE;
        }
    }
   
    #endif

    switch(PANEL_GetDisplayType())
    {
        default:
        case PANEL_DISPLAY_TYPE_MAX:
            // error display type, defaultly do nothing
            break;
        case PANEL_DISPLAY_TYPE_LVDS:
            vIO32WriteFldAlign(TMGR5, 3, TMGR5_RG_FIFO_CTRL);
            vIO32WriteFldAlign(MLVDS_CTRL_09, 0, RG_FIFO_SEL); 
//mt5396            vIO32WriteFldAlign(MLVDS_CTRL_09, 0, RG_FIFO_SEL);			
            break;
        case PANEL_DISPLAY_TYPE_MLVDS:
            vIO32WriteFldAlign(TMGR5, 4, TMGR5_RG_FIFO_CTRL);			
            vIO32WriteFldAlign(MLVDS_CTRL_09, 1, RG_FIFO_SEL); 
//mt5396            vIO32WriteFldAlign(MLVDS_CTRL_09, 1, RG_FIFO_SEL);						
            break;
        #if defined(CC_MT5399) || defined (CC_MT5882)
            case PANEL_DISPLAY_TYPE_VB1:
                vIO32WriteFldAlign(TMGR5, 0, TMGR5_RG_FIFO_CTRL);			
//                vIO32WriteFldAlign(FIFO_CTRL_01, 3, TCLK_FIFO_ACTIVE_PORT); 
                vIO32WriteFldAlign(TMGR2, 2, TMGR2_RG_FIFO_CK_SEL); 
                vIO32WriteFldAlign(MLVDS_CTRL_09, 2, RG_FIFO_SEL); 
//                vIO32WriteFldAlign(FIFO_CTRL_01, 1, TCLK_FIFO_START); 
                break;
        #endif
            case PANEL_DISPLAY_TYPE_EPI:
                vIO32WriteFldAlign(TMGR5, 2, TMGR5_RG_FIFO_CTRL);						
//                vIO32WriteFldAlign(FIFO_CTRL_01, 5, TCLK_FIFO_ACTIVE_PORT); 
                vIO32WriteFldAlign(TMGR2, 4, TMGR2_RG_FIFO_CK_SEL); 
                vIO32WriteFldAlign(MLVDS_CTRL_09, 4, RG_FIFO_SEL); 
//                vIO32WriteFldAlign(FIFO_CTRL_01, 1, TCLK_FIFO_START); 
                break;
    }
    vIO32WriteFldAlign(TMGR5, 1, TMGR5_RG_FIFO_EN);

#ifndef CC_MTK_LOADER
#ifdef CC_SCPOS_3DTV_SUPPORT 
    vSavePanelAttribute();
    fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_OSTG, (vTDTVModeChangeCB) vDrvDisp3DModeCB);
#endif
#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
	vSavePanelAttribute_4K2K();
#endif
#ifdef SUPPORT_DGO
	vSavePanelAttribute_DGO();
#endif
#endif
	
    vIO32WriteFldAlign(OSTG_01, 0, REG_MAIN_TOP_EN); // set sub plane mixer order above main
#if defined(CC_MT5399)
    vIO32WriteFldAlign(OSTG_01, 1, REG_MJC_IN_EN);//Selet OSTG input data path from MJC
#endif
}


/**

 * @brief vDrvDisplayInit

 * Init the Display Settings

 * @param  void
 * @retval void
 */
void vDrvDisplayInit(void)
{
	#ifdef SUPPORT_DGO
    HANDLE_T  hThread;
	#endif
    
#if defined(SCPIP_SUPPORT_POST_SCALER)
    vPscPreInit();
#endif

    
// vpll init
    #ifndef CC_MTK_LOADER
    #ifdef NEW_COUNTRY_TYPE
    if(GetTargetCountryMask(COUNTRY_ATV_MASK)==COUNTRY_ATV_EU)
    #else
    #ifdef COUNTRY_FROM_EEPROM
    u1TargetCountryInfo = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;;
    if (u1TargetCountryInfo == COUNTRY_EU)
    #else
    if(GetTargetCountry()==COUNTRY_EU)
    #endif
    #endif
    #else
    if (0)
    #endif
    {
        #ifdef CC_UBOOT
        vDDDSInit_uboot();
        #else
        vDDDSInit();    
        #endif	
        if (PANEL_IsSupport120Hz())
        {
            vDrvCalPanelTiming(SV_DCLK_50HZ, 100);
        }
        else
        {
            vDrvCalPanelTiming(SV_DCLK_50HZ, 50);
        }
    }
    else
    {
        // if panel already on, don't set again.
        if( !_fgVOPLLPowerOn)  //if(!bIsPanelOn())
        {
        #ifdef CC_UBOOT
         vDDDSInit_uboot();
        #else
         vDDDSInit();
        #endif  
        if (PANEL_IsSupport240Hz())
        {
            vDrvCalPanelTiming(SV_DCLK_60HZ, 240);
        }
        else if (PANEL_IsSupport120Hz())
            {
                vDrvCalPanelTiming(SV_DCLK_60HZ, 120);
            }
            else
            {
                vDrvCalPanelTiming(SV_DCLK_60HZ, 60);
            }
        }
        else
        {
            bSI_DISPLAY_DCLK_TYPE = SV_DCLK_60HZ;
        if (PANEL_IsSupport240Hz())
        {
            bOUTPUT_REFRESH_RATE = 240;
        }
        else if (PANEL_IsSupport120Hz())
            {
                bOUTPUT_REFRESH_RATE = 120;
            }
            else
            {
                bOUTPUT_REFRESH_RATE = 60;
            }
            _u4VPllFrequency = PANEL_GetPixelClk60Hz();//for dynamic MDR
            #ifndef CC_MTK_LOADER
            vDrvSetSSMDRAsInputHsync();//for dynamic MDR
            #ifdef CC_MT5399
            bDrvUpdateInternalFrameRate();
            #endif
            #endif
            vDrvSwitchImportProtection(0);
            #ifndef CC_MTK_LOADER
            #ifdef SUPPORT_DDDS_STEP_TRACKING
            vDrvUpdateDisplayCenterCW(IO32ReadFldAlign(DDDS_00,DDDS_00_DDDS_FREQ_CW));
            #endif
            #endif
    
        }
    }
    #if !defined(CC_MTK_LOADER) && defined(FORCE_3D_60HZ_OUTPUT_ENABLE)
    u4DrvSetCustFRC(SUPPORT_50TO60_3D);
    #endif
    vDrvOutputStageInit();
#if  !defined(CC_LGE_PROTO_PCBA)
		vDrvInitSCANPWM(DRVCUST_PanelGet(ePanelBacklightPwmPort));
#endif

	
	fgBLCustomCtrl = 1;

    //SET main/pip mute works immediately
    vIO32WriteFldAlign(MUTE_00, 1 ,R_MUTE_POST_SEL);
    vIO32WriteFldAlign(MUTE_00, 1 ,R_P_MUTE_FRONT_SEL);
    vIO32WriteFldAlign(MUTE_00, 1 ,R_M_MUTE_FRONT_SEL);       
#ifdef CC_UBOOT  //for ubbot garbage problem by yangyang
	vIO32WriteFldAlign(SCPIP_SCCTRL1_0A, 0 ,SCCTRL1_0A_SC_SET_RES_IMPORT_ENABLE_1); 
	vIO32WriteFldAlign(SCPIP_SCCTRL1_0A + 0x200, 0 ,SCCTRL1_0A_SC_SET_RES_IMPORT_ENABLE_1); 

	vIO32WriteFldAlign(SCPIP_SCCTRL1_11, 0 ,SCCTRL1_11_SC_OUTPUT_WIDTH_1); 
	vIO32WriteFldAlign(SCPIP_SCCTRL1_11, 0 ,SCCTRL1_11_SC_OUTPUT_HEIGHT_1); 
	vIO32WriteFldAlign(SCPIP_SCCTRL1_11 + 0x200, 0 ,SCCTRL1_11_SC_OUTPUT_WIDTH_1); 
	vIO32WriteFldAlign(SCPIP_SCCTRL1_11 + 0x200, 0 ,SCCTRL1_11_SC_OUTPUT_HEIGHT_1); 
#endif  
    // disable video plane
    //vIO32WriteFldAlign(SCPIP_PIP1_02, 0, PIP1_02_RVL_1);
    //vIO32WriteFldAlign(SCPIP_PIP2_02, 0, PIP2_02_RVL_2);

	#ifdef SUPPORT_DGO
	if(!_hDGOUnmuteSema)
	{
		VERIFY(x_sema_create(&_hDGOUnmuteSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);		
		VERIFY(x_sema_create(&_hDGOSetPanelSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);		
		VERIFY(x_thread_create(&hThread, "DGO Unmute", 4096, 60, vDGOUnmute, 0, NULL) == OSR_OK);	
	}
	#endif
#ifndef CC_MTK_LOADER
#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
	//if ((GetCurrentPanelIndex() == PANEL_CMI_4K2K_FHD120))
	{
		HANDLE_T  hThread;
		if(!_hFrcUnmuteSema)
		{
			VERIFY(x_sema_create(&_hFrcUnmuteSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);		
			VERIFY(x_sema_create(&_hFrcSetPanelSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);		
			VERIFY(x_thread_create(&hThread, "FRC Unmute", 4096, 60, vFrcUnmute, 0, NULL) == OSR_OK);	
			Printf("----[4k2k][I2C] @vDrvDisplayInit 4k2k  ----\n"); 	
			VERIFY(x_timer_create(&h4k2kUnMuteTimer) == OSR_OK);			
			vExt4k2kFrcInit();   			
		}			
	#if defined(CC_MT5399)	
    	BSP_PinSet(PIN_GPIO53, PINMUX_FUNCTION1);
	#elif defined(CC_MT5882)
        BSP_PinSet(PIN_OPCTRL5, PINMUX_FUNCTION7);
	#endif
		if(IS_DISPR_INVERSE)
		Panel_3DGlassesCTL(e3DGlasses_P_3DL,e3DGlasses_S_LEYE,e3DGlasses_POl_Bypass,e3DGlasses_POS_1stR,0,0);			
		else		
		Panel_3DGlassesCTL(e3DGlasses_P_3DL,e3DGlasses_S_DISPR,e3DGlasses_POl_Reverse,e3DGlasses_POS_1stR,0,0);
		Panel_DISPR_FIELD_Delay(eDispRFieldAlign_S_DispR,eDispRFieldAlign_M_Bypass,0);
    	Panel_3DGlassesCTLEn(e3DGlasses_P_3DL,1);   
  	
	}
    #endif
#endif

#ifdef CC_MT5399
	//Set OSD Timing from Post Scaler
	vIO32WriteFldAlign(SCPIP_SCSYS_14, 1, SCSYS_14_SC_OSD_SRC_SEL);
#else
	//Set OSD Timing from Front Scaler
	vIO32WriteFldAlign(SCPIP_SCSYS_14, 0, SCSYS_14_SC_OSD_SRC_SEL);
#endif
}

BOOL _fgPanelDynamicChg = 0;

void PanelSupportDynamicChg(BOOL fgEnable)
{
  	#ifdef SUPPORT_PANEL_DYNAMIC_CHANGE
 _fgPanelDynamicChg = fgEnable;
  	#endif
}
void vDrvLCDSetFrame(UINT16 wFrameRate)
{
#ifdef SUPPORT_PANEL_DYNAMIC_CHANGE
	if(_fgPanelDynamicChg)
	{
     bSI_DISPLAY_DCLK_TYPE = SV_DCLK_UNKNOW;
	} 
#endif

    if (wFrameRate==50)
    {
        vDrvCalPanelTiming(SV_DCLK_50HZ, 50);
    }
    else if (wFrameRate==60)
    {
        vDrvCalPanelTiming(SV_DCLK_60HZ, 60);
    }
    else if (wFrameRate==100)
    {
        vDrvCalPanelTiming(SV_DCLK_50HZ, 100);
    }
    else if (wFrameRate==120)
    {
        vDrvCalPanelTiming(SV_DCLK_60HZ, 120);
    }
	#ifndef CC_MTK_LOADER
    else if (SUPPORT_PANEL_48HZ && (wFrameRate == 48))
    {
        vDrvCalPanelTiming(SV_DCLK_48HZ, wFrameRate);
    }
    #endif
    else
    {
        vDrvCalPanelTiming(SV_DCLK_OTHERS, (UINT8)wFrameRate);
    }
	#ifdef SUPPORT_PANEL_DYNAMIC_CHANGE
    if (_fgPanelDynamicChg)
	{
        vDrvMJCSetModeChangeTrig();
        vDrvSetErrorLimit(0);
    } 
	#endif
}


#ifndef CC_MTK_LOADER
/**
 * @brief vDrvUpdateInternalFrameRate

 * update internal frame rate between up scalar and MJC

 * @param  void
 * @retval update or not
 */
UINT8 bDrvUpdateInternalFrameRate(void)
{
    #if defined(CC_MT5399)
        UINT32 u4PCLK_Sel;

        if (fgIsMJCToOSTG())
        {
            u4PCLK_Sel = 2; // poclk_dpll
        }
        else
        {
            u4PCLK_Sel = 0; // oclk
        }
        vIO32WriteFldAlign(CKGEN_DISP_CKCFG, u4PCLK_Sel, FLD_POCLK_SEL);                                   
    #endif

    return SV_TRUE;
}
#endif // CC_MTK_LOADER

//get internal frame rate between Front scaler and MJC
UINT16 u2DrvGetInternalFrameRate(void)
{
    #if defined(CC_MT5399)
    if (fgIsMJCToOSTG())
    {
        return _u2IntFrameRate;
    }
    else
    {
        return (UINT16)bOUTPUT_REFRESH_RATE;    
    }
    #else
    return (UINT16)bOUTPUT_REFRESH_RATE;
    #endif
}

//set internal frame rate between Front scaler and MJC
void vDrvSetInternalFrameRate(UINT16 frameRate)
{
    #if defined(CC_MT5399)
    _u2IntFrameRate = frameRate;
    #else
    //no-nothing
    #endif
}

void vDrvCalPanelTiming(UINT8 u1DCLKType, UINT32 u4FrameRate)
{
    UINT32 u4PanelClock;
    UINT32 u4PanelRatio;
    #ifndef SUPPORT_PANEL_SCAN_PWM
    static BOOL fgInit = FALSE;
    #endif

    LOG(2, "vDrvCalPanelTiming: (DCLKType, FrameRate) = (%d, %d) -> (%d, %d)\n",
        bSI_DISPLAY_DCLK_TYPE, bOUTPUT_REFRESH_RATE,
        u1DCLKType, u4FrameRate);

    if ((bSI_DISPLAY_DCLK_TYPE == u1DCLKType)
        #ifndef CC_MTK_LOADER
        && (bOUTPUT_REFRESH_RATE == u4FrameRate)
        #endif
        )
    {
        #ifndef CC_MTK_LOADER
        bDrvUpdateInternalFrameRate();
        vDrvSetSSMDRAsInputHsync();
        #endif
    }

    u4PanelRatio = u4DrvCalPanelRatio();
    bOUTPUT_REFRESH_RATE = u4FrameRate;

    switch (u1DCLKType)
    {
        case SV_DCLK_48HZ:
            #ifndef CC_MTK_LOADER
            if (SUPPORT_PANEL_48HZ)
            {
                u4PanelClock = PANEL_GetPixelClk48Hz();
                bSI_DISPLAY_DCLK_TYPE = SV_DCLK_48HZ;
                break;
            }
            #endif
		#ifdef SUPPORT_DISP_48TO60
			goto OUTPUT_CLK60HZ;
		#endif
		case SV_DCLK_50HZ:
            u4PanelClock = PANEL_GetPixelClk50Hz();
            bSI_DISPLAY_DCLK_TYPE = SV_DCLK_50HZ;
            break;
        case SV_DCLK_60HZ:
	#ifdef SUPPORT_DISP_48TO60
		OUTPUT_CLK60HZ:
	#endif
            u4PanelClock = PANEL_GetPixelClk60Hz();
            bSI_DISPLAY_DCLK_TYPE = SV_DCLK_60HZ;
            break;
        case SV_DCLK_OTHERS:
        default:
            if (u4FrameRate < (50 << u4PanelRatio))
            {
                u4PanelClock = (PANEL_GetPixelClk50Hz() / (50 << u4PanelRatio)) * bOUTPUT_REFRESH_RATE;
                bSI_DISPLAY_DCLK_TYPE = SV_DCLK_50HZ;
            }
            else
            {
                u4PanelClock = (PANEL_GetPixelClk60Hz() / (60 << u4PanelRatio)) * bOUTPUT_REFRESH_RATE;
                bSI_DISPLAY_DCLK_TYPE = SV_DCLK_60HZ;
            }
            break;
    }

    #if !defined(CC_MTK_LOADER) && defined(CC_DRV_SUPPORT_SMICRO)
    {
        SMICRO_VERTICAL_FREQ eFreq;
        eFreq = (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_48HZ)? FREQ_48HZ :
                (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_50HZ)? FREQ_50HZ : FREQ_60HZ;
        vSMicroPreSetFreq(eFreq);
    }
    #endif

    #ifndef SUPPORT_PANEL_SCAN_PWM
    if (fgInit)
    {
        BeforeOutputFrameRateChange(bOUTPUT_REFRESH_RATE);
    }
    #endif
    
    vDrvSwitchMTKGoodDclk(SV_OFF);
    vDrvSetPanelTiming();
    
    #if defined(CC_SCPOS_3DTV_SUPPORT) && !defined(CC_MTK_LOADER)
    if (u4DrvGetDisp3DModeDclk() != 0)
    {
        u4PanelClock = u4DrvGetDisp3DModeDclk();
        bOUTPUT_REFRESH_RATE = u1DrvGetDisp3DModeRefreshRate();
    }
    #endif

#ifdef CC_MTK_LOADER
    vDrvVOPLLFreqSet(u4PanelClock);
#else
    vDrvVOFreqSet(u4PanelClock);
#endif

#ifndef CC_MTK_LOADER
	vDrvSetSSMDRAsInputHsync(); // for dynamic MDR
#endif


    #if defined(SUPPORT_FREERUN_DCLK_BY_STEP) && !defined(SUPPORT_CW_CHANGE_BY_STEP) && !defined(CC_MTK_LOADER)
    // store the original control word
    i4OrigControlWord = IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW);
    #endif

    if (fgDDDSEn == SV_ON && _eSyncLockMode != eSYNCLOCK_OFF)
    {
        if (fgIsMainTvd3d())
        {
            _fgFRS_ENA = SV_ON;    //Delay for VCR mode: Only for TVD
        }
        else
        {
            vDrvSwitchMTKGoodDclk(SV_ON);
        }
    }
    else
    {
        _fgFRS_ENA = SV_OFF;
        vDrvSwitchMTKGoodDclk(SV_OFF);
    }

    #ifndef SUPPORT_PANEL_SCAN_PWM
    if (fgInit)
    {
        AfterOutputFrameRateChange();
    }
    #endif

    #if !defined(CC_MTK_LOADER) && defined(CC_DRV_SUPPORT_SMICRO)
    {
        SMICRO_VERTICAL_FREQ eFreq;
        SMICRO_MSG rMsg;       
        if (SUPPORT_PANEL_48HZ && (bOUTPUT_REFRESH_RATE == (UINT8)(48 * u4Ratio)))
        {
            eFreq = FREQ_48HZ;
        }
        else if (bOUTPUT_REFRESH_RATE < (UINT8)(55 * u4Ratio))
        {
            eFreq = FREQ_50HZ;
        }
        else
        {
            eFreq = FREQ_60HZ;
        }
        rMsg.u4ModuleId = MODULEID_PANEL_PARAM;
        rMsg.u4CommandId = CMD17_SET_VERTICAL_FREQ;
        rMsg.aPrm[0] = (UINT8)eFreq;
        rMsg.pCallbackFcn = NULL;
        vSMicroSendEvent(rMsg);  
    }
    #endif        

    #ifndef SUPPORT_PANEL_SCAN_PWM
    if (!fgInit)
    {
        fgInit = TRUE;
    }
    #endif
}


UINT8 vDrvGetLCDFreq(void)   
{
	return bOUTPUT_REFRESH_RATE;
}

void OSTG_SetPixelShift(BOOL fgEnable, INT32 i4HPixelShift, INT32 i4VPixelShift)
{
    BOOL fgOriPixelShiftEnable;
    
    if ((ABS(i4HPixelShift) > PIXEL_SHIFT_H_MAX) || (ABS(i4VPixelShift) > PIXEL_SHIFT_V_MAX))
    {
        //LOG(1, "pixel shift out of range, i4HPixelShift=%d, i4VPixelShift=%d\n",  i4HPixelShift, i4VPixelShift);
        return;
    }
    //LOG(5, "PMX_SetPixelShift, i4HPixelShift=%d, i4VPixelShift=%d\n",  i4HPixelShift, i4VPixelShift);

    fgOriPixelShiftEnable =  PANEL_IsPixelShift();
    PANEL_SetPixelShift(fgEnable, i4HPixelShift, i4VPixelShift);
     
    // turn on pixel shift
    if (fgEnable)
    {
//        _i4HPixelShift = i4HPixelShift;
        if (!fgOriPixelShiftEnable)
        {
        #ifndef CC_COPLAT_MT82
            IGNORE_RET(OSD_UpdateTiming());
        #endif
            //LOG(5, "turn on pixel shift, width=%d, height=%d\n", PANEL_GetPanelWidth(), PANEL_GetPanelHeight());
        }
        _u4OstgUpdateEvent |= OSTG_UPDATE_PIXEL_SHIFT_H;
        _u4OstgUpdateEvent |= OSTG_UPDATE_PIXEL_SHIFT_V;
    }
    else if (!fgEnable && fgOriPixelShiftEnable)     // previous is enable, restore timing
    {
        vDrvSetVsyncBp(u4GetPanelVSyncBp());
        #ifndef CC_COPLAT_MT82
        IGNORE_RET(OSD_UpdateTiming());
        #endif

      //  LOG(5, "turn off pixel shift, width=%d, height=%d\n", PANEL_GetPanelWidth(), PANEL_GetPanelHeight());
    }
}

void Panel_SetDisplayShift(BOOL fgEnable, UINT8 u1ShiftMode,UINT8 u1L1DisplayShift, UINT8 u1R2DisplayShift)	
{
  vIO32WriteFldAlign(VINTF_P17, fgEnable, VINTF_3D_DISP_SHIFT_EN);
  vIO32WriteFldAlign(VINTF_P17, u1ShiftMode, VINTF_3D_DISP_SHIFT_FRAME);  
  vIO32WriteFldAlign(VINTF_P17, 1, VINTF_3D_DISP_SHIFT_CTL);  
  vIO32WriteFldAlign(VINTF_P17, u1L1DisplayShift, VINTF_3D_DISP_SHIFT_VALUE_L);    
  vIO32WriteFldAlign(VINTF_P17, u1R2DisplayShift, VINTF_3D_DISP_SHIFT_VALUE_R);    
}

void Panel_SetDisplayShiftColor(UINT8 u1ShiftColorB,UINT8 u1ShiftColorG, UINT8 u1ShiftColorR)	
{
  vIO32WriteFldAlign(VINTF_P18, u1ShiftColorB, VINTF_3D_DISP_SHIFT_B);    
  vIO32WriteFldAlign(VINTF_P18, u1ShiftColorG, VINTF_3D_DISP_SHIFT_G);  
  vIO32WriteFldAlign(VINTF_P18, u1ShiftColorR, VINTF_3D_DISP_SHIFT_R);   
}

void Panel_DISPR_FIELD_Delay(UINT8 u1Tpye, UINT8 u1AlignType,UINT16 u1DelayLine)	
{
 if (u1Tpye == eDispRFieldAlign_S_Field)
 {

 }
 else
 {
	#if 0
   //#if defined(CC_MT5368) || defined(CC_MT5389)
   if ((BSP_GetIcVersion() >= IC_VER_5368_AB)||(BSP_GetIcVersion() >= IC_VER_5389_AB))
   {
    vIO32WriteFldAlign(VINTF_P19, u1AlignType, VINTF_DISP_R_ALLIGN_TYPE);
   } 
   else
   {
    vIO32WriteFldAlign(VINTF_P19, u1AlignType, VINTF_DISP_R_ALLIGN_TYPE_NOECO);
   }
   #else
   vIO32WriteFldAlign(VINTF_P19, u1AlignType, VINTF_DISP_R_ALLIGN_TYPE);
   #endif
   
   // DISP_R delay limitaion: "0~1122" OK, "1123" need bypass 6 line buffer, 1124 can't reach
   if(u1DelayLine < (wDrvGetOutputVTotal()-2))
   {
    vIO32WriteFldAlign(VINTF_P19, u1DelayLine, VINTF_DISP_R_H_DLY);	
   }
   else
   {
    vIO32WriteFldAlign(VINTF_P19, 0, VINTF_DISP_R_H_DLY); 
   }
 }
}

void Panel_3DGlassesCTL(UINT8 u1Pin, UINT8 u1Tpye, BOOL fgPol,BOOL fgPos, UINT8 u1DelayFrame, UINT16 u2DelayLine)
{
  #if defined(CC_MT5399)
  if (u1Pin == e3DGlasses_P_3DR)
  {
   UNUSED(BSP_PinSet(PIN_GPIO52, PINMUX_FUNCTION1)); 
  }
  else if (u1Pin == e3DGlasses_P_3DL)
  {
   UNUSED(BSP_PinSet(PIN_GPIO53, PINMUX_FUNCTION1)); 
  }
  else if (u1Pin == e3DGlasses_P_3DBL)
  {
   UNUSED(BSP_PinSet(PIN_GPIO54, PINMUX_FUNCTION1)); 
  }
  #elif defined(CC_MT5882)
  if (u1Pin == e3DGlasses_P_3DR)
  {
   UNUSED(BSP_PinSet(PIN_OPCTRL4, PINMUX_FUNCTION7)); 
  }
  else if (u1Pin == e3DGlasses_P_3DL)
  {
   UNUSED(BSP_PinSet(PIN_OPCTRL5, PINMUX_FUNCTION7)); 
  }
  else if (u1Pin == e3DGlasses_P_3DBL)
  {
   UNUSED(BSP_PinSet(PIN_OPCTRL6, PINMUX_FUNCTION7)); 
  }  
  #else  // 5880
  if (u1Pin == e3DGlasses_P_3DR)
  {
   UNUSED(BSP_PinSet(PIN_GPIO6, PINMUX_FUNCTION2)); 
  }
  else if (u1Pin == e3DGlasses_P_3DL)
  {
   UNUSED(BSP_PinSet(PIN_GPIO7, PINMUX_FUNCTION2)); 
  }
  else if (u1Pin == e3DGlasses_P_3DBL)
  {
   UNUSED(BSP_PinSet(PIN_GPIO8, PINMUX_FUNCTION2)); 
  }  
  #endif

  vIO32WriteFldAlign(VINTF_P9 + (u1Pin *8), u1Tpye, VINTF_REYE_SEL);
  
  vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), fgPos, VINTF_2EYE_POL);    
  vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), fgPos, VINTF_REYE_V_POS); // 2EYE with V_POS  
  
   if (u1Tpye == e3DGlasses_S_BL)
   {
    vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), 1, VINTF_3DBL_LRLR);  
   }	
   else
   {
   vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), 1, VINTF_2EYE_LRLR);  
   }	
  
  vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), u1DelayFrame, VINTF_2EYE_V_DLY);
  vIO32WriteFldAlign(VINTF_P6 + (u1Tpye *8), u2DelayLine, VINTF_2EYE_H_DLY);
  
  if (u1Tpye == e3DGlasses_S_2EYE)
  {
   vIO32WriteFldAlign(VINTF_P7, u1Tpye, VINTF_2EYE_SEL);
   vIO32WriteFldAlign(VINTF_P7, fgPol, VINTF_2EYE_POL);
   vIO32WriteFldAlign(VINTF_P7, u1DelayFrame, VINTF_2EYE_V_DLY);
   vIO32WriteFldAlign(VINTF_P6, u2DelayLine, VINTF_2EYE_H_DLY);
  }

  if (u1Tpye == e3DGlasses_S_BL2)
  {
   vIO32WriteFldAlign(VINTF_P26, u1Tpye, VINTF_3DBL_2_SEL);
   vIO32WriteFldAlign(VINTF_P26, fgPol, VINTF_3DBL_2_POL);
   vIO32WriteFldAlign(VINTF_P26, 1, VINTF_3DBL_2_LRLR);   
   vIO32WriteFldAlign(VINTF_P26, u1DelayFrame, VINTF_3DBL_2_V_DLY);
   vIO32WriteFldAlign(VINTF_P25, u2DelayLine, VINTF_3DBL_2_H_DLY);
  }  
}

void Panel_3DGlassesDelay(UINT8 u1Pin, UINT16 u2DelayLine)
{
    if (u1Pin == e3DGlasses_P_3DR)
    {
        vIO32WriteFldAlign(VINTF_P8, u2DelayLine, VINTF_REYE_H_DLY);
    }
    else if (u1Pin == e3DGlasses_P_3DL)
    {
        vIO32WriteFldAlign(VINTF_P10, u2DelayLine, VINTF_LEYE_H_DLY);
    }
    else if (u1Pin == e3DGlasses_P_3DBL)
    {
        vIO32WriteFldAlign(VINTF_P12, u2DelayLine, VINTF_3DBL_H_DLY);
    }

    w3DGlassesDelay = u2DelayLine;
}

void Panel_3DGlassesRSTCTL(UINT8 u1Tpye, BOOL fgOnOff, UINT16 u2RSTLine)
{  
  vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), fgOnOff, VINTF_2EYE_RST_MODE);    
  vIO32WriteFldAlign(VINTF_P6 + (u1Tpye *8), u2RSTLine, VINTF_2EYE_WIDTH);
}

void Panel_3DGlassesDutyCTL(UINT8 u1Tpye, UINT8 u1DutyH)
{ 
    UINT32 u4Vtotal,u4VtotalH;
	u4Vtotal = wDrvGetOutputVTotal();
	
	if (IS_PANEL_L12R12)
	{
     u4Vtotal = u4Vtotal >>1;
	}	
	
	 u4VtotalH = u4Vtotal *  u1DutyH / 100 ;


    if (u4VtotalH >= 1)
    {
      u4VtotalH = u4VtotalH - 1;
    }

    Panel_3DGlassesRSTCTL(u1Tpye, 1, u4VtotalH);
}

// fgonoff = 0 [off ] fgonoff = 1 [on ]
void Panel_3DGlassesCTLEn(UINT8 u1Pin, BOOL fgOnfff)
{
    vIO32WriteFldAlign(VINTF_P9 + (u1Pin * 8), !fgOnfff, VINTF_REYE_OP_EN);  
	LOG(0, "Panel_3DGlassesCTLEn: %d, %d\n", u1Pin, fgOnfff);
    if (e3DGlasses_P_3DBL == u1Pin)
    {
        LOG(0, "Panel_3DGlassesCTLEn: %d\n", fgOnfff);
        vIO32WriteFldAlign(VINTF_P9 + (u1Pin * 8), (fgOnfff) ? 3 : 7, VINTF_REYE_SEL);  
    }
}

void Panel_2ndChannelOnOff(BOOL fgOnfff)
{
    if (IS_PANEL_L12R12)
    {
        if (LVDS_OUTPUT_PORT == FOUR_PORT)
        {
			LOG(0, "Panel_2ndChannelOnOff: %d, %d\n", fgOnfff);
            vIO32WriteFldAlign(LVDSB_REG07, (fgOnfff)? 0x0 : 0xCFFC00, RG_PD);
        }
        else if (LVDS_OUTPUT_PORT == DUAL_PORT)
        {
			LOG(0, "Panel_2ndChannelOnOff: %d, %d\n", fgOnfff);
            vIO32WriteFldAlign(LVDSB_REG07, (fgOnfff)? 0x0 : 0xEFFFE0, RG_PD);
        }
    }
}

void vDrvDisplaySetPentouchMode(UINT32 fgOnOff)
{
    if (fgPentouchMode != fgOnOff)
    {
        fgPentouchMode = (UINT8)fgOnOff;
        vSetMainFlg(MAIN_FLG_MODE_CHG);
        vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
    }
}

UINT32 fgDrvDisplayGetPentouchMode(void)
{
    return fgPentouchMode;
}

void vDrvVideoSetMuteColor(UINT8 bPath, UINT32 u4Bg) 
{
    RDispInfo *prDispInfo;
    prDispInfo = getDispInfo(bPath);
    
    if (prDispInfo->u4MuteColor != u4Bg)
    {
        prDispInfo->u4MuteColor = u4Bg;
        if (bPath == SV_VP_MAIN)
        {
            vSetScposFlg(MAIN_MUTECOLOR_CHG);
        }
        else
        {
            vSetScposFlg(PIP_MUTECOLOR_CHG);
        }
    }
}

/**
* @brief Output stage normproc
*/
void vOstgNormProc(void)
{
#ifndef CC_MTK_LOADER
#if 0//def SUPPORT_CW_CHANGE_BY_STEP
    static UINT32 u4Val = 0;
    static UINT32 u4LocalTargetCW = 0;
    static BOOL isIncrease;
    static BOOL isFirstStep = TRUE;

    if( (u4LocalTargetCW != _u4TargetCW) && isFirstStep )
    {
        u4Val = _u4CurrentCW; //inital the first CW
        isIncrease = (_u4TargetCW > _u4CurrentCW ) ? TRUE : FALSE;
        isFirstStep = FALSE;

        //give-up first entry, delay the pll setting to next vsync (osd muted)
        return;
    }

    if( (_u4CurrentCW == _u4TargetCW) || (u4Val == _u4TargetCW) )
    {
        u4LocalTargetCW = _u4TargetCW;
        isFirstStep = TRUE;

        return;
    }



    if( 0xfffffff == _u4CWStep )
    {
        u4Val = _u4TargetCW;
    }
    else
    {
        if (isIncrease)
        {
            u4Val = ( (u4Val+_u4CWStep) >= _u4TargetCW ) ? _u4TargetCW : (u4Val+_u4CWStep);
        }
        else
        {
            u4Val = ( (u4Val-_u4CWStep) <= _u4TargetCW ) ? _u4TargetCW : (u4Val-_u4CWStep);
        }
    }

    if( u4Val == _u4TargetCW )
    {
        u4LocalTargetCW = u4Val;
        isFirstStep = TRUE;
    }
    
    vIO32WriteFldAlign(DDDS_00,u4Val,DDDS_00_DDDS_FREQ_CW);

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
    // store the original control word
    i4OrigControlWord = u4Val;
#endif
    
    vDrvVoDclkSpreadInit(1);
    HAL_Delay_us(100);
    vDrvVoDclkSpreadInit(0);   
#endif

    
    #ifdef SUPPORT_DDDS_STEP_TRACKING
    vDrvDDDSTrackingNormProc();
    #endif
    
#ifdef SUPPORT_FREERUN_DCLK_BY_STEP 
        static INT32 i4DeltaControlWord = 0;
        if((PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1) && (IS_STEP_CLOCK_CHG))
        {
    
        if (u1DDDSLockStatus == 1)    
        {
            if(u1SignalStatus != bDrvVideoSignalStatus(SV_VP_MAIN))
            {
                 u1SignalStatus = bDrvVideoSignalStatus(SV_VP_MAIN);
             }
            if(u1SignalStatus == SV_VDO_STABLE)
            {
               if(u1StepDelay2 < STEP_CW_DELAY2)
               {
                   u1StepDelay2++;
               }
               else
               {
                   if(_u4ErrorLimit < TargetErrlimit)
                   {
                       vDrvSetErrorLimit(_u4ErrorLimit+1);
                       LOG(2,"_u4ErrorLimit %d\n", _u4ErrorLimit);
                   }
				   vDrvSetDDDSCloseloop(1);
               }
            }
        }
    //    if (IO32ReadFldAlign(DDDS_00,DDDS_00_DISP_EN)== 1)    
        {
            if(u1StepDelay < STEP_CW_DELAY)
            {
                u1StepDelay++;
           }
            else
        {
            if(u1StepControlWord < STEP_CW)
            {
                u1StepControlWord++;       
    
                if(u1StepControlWord < STEP_CW && i4OrigControlWord != 0x7fffffff)
                {
                    if(u1StepControlWord == 1)
                    {
                        i4DeltaControlWord = ((INT32)IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW) - i4OrigControlWord)/STEP_CW;
                    }
                    
                    vIO32WriteFldAlign(DDDS_00, i4OrigControlWord + i4DeltaControlWord*(STEP_CW - u1StepControlWord), DDDS_00_DDDS_FREQ_CW);
                    
                }
                else if(u1StepControlWord == STEP_CW && i4OrigControlWord != 0x7fffffff)
                {
                    vIO32WriteFldAlign(DDDS_00, i4OrigControlWord, DDDS_00_DDDS_FREQ_CW);
                    vIO32WriteFldMulti(DDDS_00,P_Fld(1,DDDS_00_DISP_EN));
                    HAL_Delay_us(20);
                    vIO32WriteFldMulti(DDDS_00,P_Fld(0,DDDS_00_DISP_EN));
                }
    
                vIO32WriteFldAlign(DDDS_03, 1, DDDS_03_SPREAD_INIT);
                HAL_Delay_us(20);
                LOG(2,"step %d, freerun-CW: %x\n", u1StepControlWord, IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW));
                vIO32WriteFldAlign(DDDS_03, 0, DDDS_03_SPREAD_INIT);
            }
        }
        }
        }
        else if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
        {
    
    //    if (IO32ReadFldAlign(DDDS_00,DDDS_00_DISP_EN)== 1)    
        {
            if(u1StepDelay < STEP_CW_DELAY_EPI)
            {
                u1StepDelay++;
           }
            else
        {
            if(u1StepControlWord < STEP_CW)
            {
                u1StepControlWord++;       
    
                if(u1StepControlWord < STEP_CW && i4OrigControlWord != 0x7fffffff)
                {
                    if(u1StepControlWord == 1)
                    {
                        i4DeltaControlWord = ((INT32)IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW) - i4OrigControlWord)/STEP_CW;
                    }
                    
                    vIO32WriteFldAlign(DDDS_00, i4OrigControlWord + i4DeltaControlWord*(STEP_CW - u1StepControlWord), DDDS_00_DDDS_FREQ_CW);
                    
                }
                else if(u1StepControlWord == STEP_CW && i4OrigControlWord != 0x7fffffff)
                {
                    vIO32WriteFldAlign(DDDS_00, i4OrigControlWord, DDDS_00_DDDS_FREQ_CW);
                }
    
                vIO32WriteFldAlign(DDDS_03, 1, DDDS_03_SPREAD_INIT);
                HAL_Delay_us(20);
                LOG(2,"step %d, freerun-CW: %x\n", u1StepControlWord, IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW));
                vIO32WriteFldAlign(DDDS_03, 0, DDDS_03_SPREAD_INIT);
                if(u1StepControlWord == STEP_CW)
                {
                    if (u1DDDSLockStatus == 1)    
                        vDrvSetDDDSCloseloop(1);
                }
            }
        }
        }
        }
#endif

#endif // CC_MTK_LOADER
    return;
}


// OSTG Event on Output VSync
// Note: Keep this function simple since this function is called by ISR
void OSTG_OnOutputVSync(void)
{
    
#ifndef CC_MTK_PRELOADER
    if (_u4OstgUpdateEvent & OSTG_UPDATE_PIXEL_SHIFT_H)
    {
        _u4OstgUpdateEvent &= ~OSTG_UPDATE_PIXEL_SHIFT_H;
    }
    if (_u4OstgUpdateEvent & OSTG_UPDATE_PIXEL_SHIFT_V)
    {
        vDrvSetVsyncBp(u4GetPanelVSyncBp());
        _u4OstgUpdateEvent &= ~OSTG_UPDATE_PIXEL_SHIFT_V;
    }    
#endif

#ifndef CC_MTK_LOADER
    if (fgIsScposFlgSet(MAIN_MUTECOLOR_CHG))
    {
        vDrvVideoMuteColor(SV_VP_MAIN, _rMDispInfo.u4MuteColor);
        vClrScposFlg(MAIN_MUTECOLOR_CHG);
    }
    if (fgIsScposFlgSet(PIP_MUTECOLOR_CHG))
    {
        vDrvVideoMuteColor(SV_VP_PIP, _rPDispInfo.u4MuteColor);
        vClrScposFlg(PIP_MUTECOLOR_CHG);
    }
#endif

#ifndef CC_MTK_LOADER
#ifdef LVDS_ESD_PROTECT
#ifdef CC_MT5882
	if (_fgESDProtect)
	{
		if (fgIsLVDSFIFOSkew())
		{
			vLVDSFIFOSkew_Recover();
		}
	}
#endif
#endif
#endif 
}


void vDrvSwitchImportProtection(UINT8 bOn_Off)
{
}


void vDrvFireImportPortection(void)
{
	if (fgIsMJCToOSTG())
	{
		vPscSetData(1);
		LOG(3,"PSC Set data _u4VSyncCountSD = %d \n",_u4VSyncCountSD);		
	} 
	else
	{
		vScpipSetData(1);
		LOG(3,"FSC Set data _u4VSyncCountSD = %d \n",_u4VSyncCountSD);		
	} 
}

void vDrvSetErrorLimit(UINT32 u4Error)
{
    if (u4Error != _u4ErrorLimit)
    {
        _u4ErrorLimit = u4Error;
        vIO32WriteFldAlign(DDDS_03, _u4ErrorLimit, DDDS_03_DDDS_ERR_LIM);
    }
}

#ifdef CC_ENABLE_MTK_MODEL_INDEX
UINT8 u1UseSuperFlipMirror=0;
UINT8 u1SuperFlipMirrorCfg=0;
UINT8 u1SetFlipMirrorConfig(BOOL fgMirrorEn, BOOL fgFlipEn);

void vSetSuperFlipMirrorConfig(UINT8 u1SuperCfgOnOff, UINT8 u1SuperFlip, UINT8 u1SuperMirror)
{
	u1UseSuperFlipMirror =u1SuperCfgOnOff;

	if(u1SuperFlip)
	{
		u1SuperFlipMirrorCfg |= SYS_FLIP_CONFIG_ON;
	}
	else
	{
		u1SuperFlipMirrorCfg &= (~SYS_FLIP_CONFIG_ON);
	}

	if(u1SuperMirror)
	{
		u1SuperFlipMirrorCfg  |= SYS_MIRROR_CONFIG_ON;

	}
	else
	{
		u1SuperFlipMirrorCfg &= (~SYS_MIRROR_CONFIG_ON);
	}

    UNUSED(u1SetFlipMirrorConfig(u1SuperMirror, u1SuperFlip));
    
}
#endif

#ifdef CC_FLIP_MIRROR_FROM_DDI
#ifdef CC_FLIP_MIRROR_SUPPORT
static BOOL _fgIsMirrorOn = FALSE; 
static BOOL _fgIsFlipOn = FALSE; 

UINT8 u1SetFlipMirrorConfig(BOOL fgMirrorEn, BOOL fgFlipEn)
{
    _fgIsMirrorOn = fgMirrorEn;
    _fgIsFlipOn = fgFlipEn;

    return SV_SUCCESS;
}

UINT8 u1GetFlipMirrorConfig(void)
{
    static UINT8 u1FlipMirrorEnable= 0 ;
    if (_fgIsMirrorOn)
    {
        u1FlipMirrorEnable |= SYS_MIRROR_CONFIG_ON ;
    }
    if(_fgIsFlipOn)
    {
        u1FlipMirrorEnable |= SYS_FLIP_CONFIG_ON ;
    }
    
    return u1FlipMirrorEnable;
}
#endif

#else
UINT8 u1SetFlipMirrorConfig(BOOL fgMirrorEn, BOOL fgFlipEn)
{
#ifdef CC_FLIP_MIRROR_FROM_EEPROM
    DTVCFG_T rDtvCfg;
    
    if(EEPDTV_GetCfg(&rDtvCfg))
    {
        Printf("Read Flip-Mirror Config Fail\n");
        return SV_FAIL;
    }

    if (fgMirrorEn)
    {
        rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_MIRROR_ON);
    }
    else
    {
        rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_MIRROR_ON);
    }
    if(EEPDTV_SetCfg(&rDtvCfg))
    {
        Printf("Set Mirror Config Fail\n");
        return SV_FAIL;
    }

    if (fgFlipEn)
    {
        rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_FLIP_ON);
    }
    else
    {
        rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_FLIP_ON);
    }
    if(EEPDTV_SetCfg(&rDtvCfg))
    {
        Printf("Set Flip Config Fail\n");
        return SV_FAIL;
    }
    Printf("fgMirrorEn: %d\tfgFlipEn: %d\trDtvCfg.u1Flags2: %d\n", fgMirrorEn, fgFlipEn, rDtvCfg.u1Flags2);
    return SV_SUCCESS;
    
#else
    Printf("CC_FLIP_MIRROR_SUPPORT is not defined!\n");
    return SV_FAIL;
#endif
}

#ifdef CC_FLIP_MIRROR_SUPPORT
#include "eeprom_if.h"
UINT8 u1GetFlipMirrorConfig(void)
{
    static UINT8 u1Init = 0 ;
    static UINT8 u1FlipMirrorEnable= 0 ;
#ifdef CC_FLIP_MIRROR_FROM_EEPROM
	DTVCFG_T rDtvCfg;
#endif

	if((u1Init == 0) || (u1FlipMirrorReset == 1))
	{
#ifdef CC_FLIP_MIRROR_FROM_EEPROM
		if(EEPDTV_GetCfg(&rDtvCfg))  
		{
			Printf(" Get DTV config fail\n");
		}
		else
		{
			if(rDtvCfg.u1Flags2 & DTVCFG_FLAG2_FLIP_ON)	
			{
				u1FlipMirrorEnable |= SYS_FLIP_CONFIG_ON;
			}
			else
			{
				u1FlipMirrorEnable &= ~(SYS_FLIP_CONFIG_ON);
			}
			
			if(rDtvCfg.u1Flags2 & DTVCFG_FLAG2_MIRROR_ON)
			{
				u1FlipMirrorEnable |= SYS_MIRROR_CONFIG_ON;
			}
			else
			{
				u1FlipMirrorEnable &= ~(SYS_MIRROR_CONFIG_ON);
			}
			Printf("====u1FlipMirrorEnable %d\n", u1FlipMirrorEnable);
		}
#else
		#ifdef CC_FLIP_ENABLE
		u1FlipMirrorEnable |= SYS_FLIP_CONFIG_ON ;
		#endif
		#ifdef CC_MIRROR_ENABLE
		u1FlipMirrorEnable |= SYS_MIRROR_CONFIG_ON ;
		#endif
#endif
		u1Init = 1 ;
		u1FlipMirrorReset = 0;
	}      

#ifdef CC_ENABLE_MTK_MODEL_INDEX
	if(u1UseSuperFlipMirror)
	{
	    LOG(9, "======u1SuperFlipMirrorCfg %d\n\n", u1SuperFlipMirrorCfg);
	    return u1SuperFlipMirrorCfg;
	}
	else
#endif
	{
    	LOG(9, "======u1FlipMirrorEnable %d\n\n", u1FlipMirrorEnable);
	    return u1FlipMirrorEnable;
	}
}
#endif
#endif

#ifdef CC_FLIP_MIRROR_SUPPORT
UINT32 u4GetFlipMirrorModule(UINT32 u4VdpId)
{
#ifndef CC_MTK_LOADER
	UINT32 u4Module =FLIP_NONE;
	if(u1GetFlipMirrorConfig())
	{
		u4Module= u4QueryFlipModule(u4VdpId);
	}
	
       LOG(9, "======u4GetFlipMirrorModule(%d) %d\n\n", u4VdpId, u4Module);

	return u4Module;
#else
	return 0;
#endif
}
#endif  //CC_FLIP_MIRROR_SUPPORT

void vDrvSetAllMute(UINT8 bOnOff)
{
    vRegWriteFldAlign(MUTE_00, bOnOff,  R_MUTE_POST_EN);
}

UINT32 u4DrvCalPanelRatio(void)
{
    if (PANEL_IsSupport240Hz())
    {
        return 2;
    }
    else if (PANEL_IsSupport120Hz())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#ifndef CC_MTK_LOADER
#ifdef SUPPORT_DDDS_STEP_TRACKING
void vDrvDDDSStepTrackingOnOff(UINT16 u2OnOff)
{
    if (_u2DDDSStepTrackingEn == u2OnOff)
    {
        return;
    }
    LOG(3, "DDDSStepTrackingOnOff: %d\n", u2OnOff);
    _u2DDDSStepTrackingEn = u2OnOff;

    // update error limit
    if (u2OnOff)
    {
        _u4DefaultErrorLimit = IO32ReadFldAlign(DDDS_03,DDDS_03_DDDS_ERR_LIM);
        vDrvSetErrorLimit(_u4DDDSTrackingErrorLimit);
    }
    else
    {
        vDrvSetErrorLimit(_u4DefaultErrorLimit);
    }
    
}

void vDrvUpdateDisplayCenterCW(UINT32 u4CW)
{

    if (_u4CenterCW == u4CW)
    {
        return;
    }    

    // update center frequence
    _u4CenterCW = u4CW;
    _u4CurrentStepCW = u4CW;

    // calculate CW upper/lower bound
    _u4StepInCWUpperBound = _u4CenterCW + ((_u4DefaultErrorLimit - _u4DDDSTrackingErrorLimit) * 32768);    
    _u4StepInCWLowerBound = _u4CenterCW - ((_u4DefaultErrorLimit - _u4DDDSTrackingErrorLimit) * 32768);   

    // if target freq delta is over  _u4CenterFreqStep, FW will update display freq.  
//    _u4StepInCW = _u4CenterFreq * (_u2DDDSTrackingSpeed - _u4CenterFreqStepInTolerance)/1000000;   
}

static void vDrvDDDSTrackingNormProc(void)
{
    if (_u2DDDSStepTrackingEn)
    {
        UINT32 u4CW;
        UINT32 u4CWUpperBound, u4CWLowerBound;
        
        // only step tracking with DDDS close loop
        if (IO32ReadFldAlign(DDDS_00,DDDS_00_DISP_EN) == 0)
        {
            return;
        }
        if ((_u4CenterCW == 0) || (_u4CurrentStepCW == 0))
        {
            LOG(1, "Invalid DDDS center CW\n");
            return;
        }

        u4CW = IO32ReadFldAlign(STA_DDDS_00, STA_DDDS_00_DDDS_FREQ_CW);
        u4CWUpperBound = _u4CurrentStepCW + (_u4DDDSTrackingErrorLimit * 32768) - _u4StepInCWTolerance;
        u4CWLowerBound = _u4CurrentStepCW - (_u4DDDSTrackingErrorLimit * 32768) + _u4StepInCWTolerance;
        if (u4CWUpperBound <= u4CWLowerBound)
        {
            LOG(2, "Invalid DDDS Step Tracking Bound (%d, %d)\n", u4CWLowerBound, u4CWUpperBound);
            return;
        }
        
        if ((u4CW > u4CWUpperBound) || (u4CW < u4CWLowerBound))
        {
            UINT32 u4NewCW;

            if (u4CW > u4CWUpperBound)
            {
                u4NewCW = _u4CurrentStepCW + (_u4DDDSTrackingErrorLimit * 32768); 
            }
            else //             (u4CW < u4CWLowerBound))
            {
                u4NewCW = _u4CurrentStepCW - (_u4DDDSTrackingErrorLimit * 32768); 
            }

            if (u4NewCW > _u4StepInCWUpperBound)
            {               
                u4NewCW = _u4StepInCWUpperBound;               
            }
            else if (u4NewCW < _u4StepInCWLowerBound)
            {
                u4NewCW = _u4StepInCWLowerBound;               
            }

            if (u4NewCW != _u4CurrentStepCW)
            {
                LOG(3, "DDDS Step Tracking: CW (0x%X) --> (0x%X)\n", _u4CurrentStepCW, u4NewCW);
                _u4CurrentStepCW = u4NewCW;
                vIO32WriteFldAlign(DDDS_00,u4NewCW,DDDS_00_DDDS_FREQ_CW);                
            }
            
            
        }

    }
}
#endif

void vDrvSetSyncLockMode(enum eSyncLockMode eMode)
{
    _eSyncLockMode = eMode;

    if(_eSyncLockMode == eSYNCLOCK_OFF)
    {
        vDrvSwitchMTKGoodDclk(SV_OFF);
    }
    else
    if(_eSyncLockMode == eSYNCLOCK_ON)
    {
        vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN),bDrvVideoGetRefreshRate(SV_VP_MAIN));
        vDrvSwitchMTKGoodDclk(SV_ON);
        vFrameTrackDDDSConfig(0, 0);
    }
    else
    {
        /* do nothing. */
    }
    return;
}

/**
 * @brief vDrvSetDDDSFrameTrackTarget

 * Set DDDS Frame track Parameters

 * @param  u1SlowFast: slow/fast parameter
 * @param  u4TargetVTotal: target output Vtotal
 * @retval void
 */ 
void vDrvSetDDDSFrameTrackTarget(UINT8 u1SlowFast, UINT32 u4TargetVTotal) 
{
	UINT32 DISPLAY_SIZE, HLEN_INT, HLEN_NUM, InputVTotal;
	UINT32 u4HTotal, u4VTotal;
//	Printf("slowfast(%d), vtotal(%d)\n",u1SlowFast,u4TargetVTotal);

	InputVTotal = IO32ReadFldAlign(DDDS_02, DDDS_02_HLEN_DEN);
	HLEN_NUM = IO32ReadFldAlign(DDDS_01, DDDS_01_HLEN_NUM);
	HLEN_INT = IO32ReadFldAlign(DDDS_01, DDDS_01_HLEN_INT);
	DISPLAY_SIZE = (HLEN_INT * InputVTotal) + HLEN_NUM;

    u4HTotal = (UINT32)wDrvGetOutputHTotal();
    u4VTotal = (UINT32)wDrvGetOutputVTotal();

	if (_fgFrTrkHalfStep)
	{
    	_uDDDSDivision = 1;
	}
    else
    {
		_uDDDSDivision = 0;
	}
	if(u4TargetVTotal > u4VTotal)
		DISPLAY_SIZE = DISPLAY_SIZE +(((u4TargetVTotal - u4VTotal)*u4HTotal)>>_uDDDSDivision);
	else
		DISPLAY_SIZE = DISPLAY_SIZE -(((u4VTotal - u4TargetVTotal)*u4HTotal)>>_uDDDSDivision);

	if (InputVTotal ==0) InputVTotal = 1;
	
    HLEN_INT = (DISPLAY_SIZE / InputVTotal);
    HLEN_NUM = DISPLAY_SIZE - (HLEN_INT * InputVTotal);
	
    switch(u1SlowFast)
    {
    case SV_DDDS_FRAMETRACK_FAST1:
		vIO32WriteFldAlign(DDDS_0C,(UINT16)InputVTotal,DDDS_0C_HLEN_DEN_E1);
		vIO32WriteFldAlign(DDDS_0D,(UINT16)HLEN_NUM,DDDS_0D_HLEN_NUM_E1);	  
		vIO32WriteFldAlign(DDDS_0C,HLEN_INT,DDDS_0C_HLEN_INT_E1);
        break;

    case SV_DDDS_FRAMETRACK_FAST2:
		vIO32WriteFldAlign(DDDS_0E,(UINT16)InputVTotal,DDDS_0E_HLEN_DEN_E2);
		vIO32WriteFldAlign(DDDS_0E,(UINT16)HLEN_NUM,DDDS_0E_HLEN_NUM_E2);	  
		vIO32WriteFldAlign(DDDS_0D,HLEN_INT,DDDS_0D_HLEN_INT_E2);
        break;

    case SV_DDDS_FRAMETRACK_SLOW1:
		vIO32WriteFldAlign(DDDS_0F,(UINT16)InputVTotal,DDDS_0F_HLEN_DEN_L1);
		vIO32WriteFldAlign(DDDS_10,(UINT16)HLEN_NUM,DDDS_10_HLEN_NUM_L1);	  
		vIO32WriteFldAlign(DDDS_0F,HLEN_INT,DDDS_0F_HLEN_INT_L1);
        break;

	case SV_DDDS_FRAMETRACK_SLOW2:
		vIO32WriteFldAlign(DDDS_11,(UINT16)InputVTotal,DDDS_11_HLEN_DEN_L2);
		vIO32WriteFldAlign(DDDS_11,(UINT16)HLEN_NUM,DDDS_11_HLEN_NUM_L2);	  
		vIO32WriteFldAlign(DDDS_10,HLEN_INT,DDDS_10_HLEN_INT_L2);
		break;

    default:
        break;
    }
	
}

/**
 * @brief vDrvSetDDDSFrameTrackMode

 * Set DDDS Frame track mode

 * @param  u1Mode: 0 => off, 1 => form scaler, 2 => form MJC
 * @retval void
 */ 
void vDrvSetDDDSFrameTrackMode(UINT8 u1Mode) 
{
    
	static UINT8 u1PrevMode = 0xFF;
	
	if (u1PrevMode == u1Mode)
	{
		return;
	}

	u1PrevMode = u1Mode;
	
    switch(u1Mode)
    {
	case 0: //off
		vIO32WriteFldAlign(DDDS_0C,SV_OFF,DDDS_0C_VSYNC_TRACK_EN);
		break;

	case 1: //from scaler
		vIO32WriteFldAlign(DDDS_0C,SV_ON,DDDS_0C_VSYNC_TRACK_EN);
		vIO32WriteFldAlign(DDDS_03,1,DDDS_03_SC_MJC_TRACK_SEL);
		break;

	case 2: //from MJC
		vIO32WriteFldAlign(DDDS_0C,SV_ON,DDDS_0C_VSYNC_TRACK_EN);
		vIO32WriteFldAlign(DDDS_03,0,DDDS_03_SC_MJC_TRACK_SEL);
		break;

	default:
		break;
    }
}

UINT32 fgDrvIsSupport48(void)
{
	if(SUPPORT_PANEL_48HZ == 0)
	{
		return FALSE;
	}
    
    if (IS_PANEL_2D_N_3D_L12R12)
    {
        return fgDrvSupport24Hz() ? TRUE : FALSE;
    }
    #if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
    else if (_u14K2KMode == E_4K2K_NATIVE)
    {
        return TRUE;
    }
    #endif
    else
    {
        #if defined(SUPPORT_CUSTOMER_FRAME_RATE_SPEC)
		return TRUE;
        #else
        return (u1DrvVideoGetIs44PullDownInfo() && PANEL_IsSupport120Hz() == FALSE) ? TRUE : FALSE;
        #endif
    }
}

UINT32 fgDrvIsSupport60To50(void)
{
    #ifdef SUPPORT_60TO50
    if (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF)
    {
        return u1CustFRC & SUPPORT_60TO50_2D;
    }
    else
    {
        return u1CustFRC & SUPPORT_60TO50_3D;
    }
    #else
    return FALSE;
    #endif
}

UINT32 fgDrvIsSupport50To60(void)
{
    #ifdef SUPPORT_50TO60
    if (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF)
    {
        return u1CustFRC & SUPPORT_50TO60_2D;
    }
    else
    {
        return u1CustFRC & SUPPORT_50TO60_3D;
    }
    #else
    return FALSE;
    #endif
}

void VDP_SetBrowserInput(UINT8 uEnable)
{
    if(uEnable)
	{
	    fgBrowserModeEn = SV_TRUE;
	}
	else
	{
	    fgBrowserModeEn = SV_FALSE;
	}
		
}

void VDP_SetSpeIPTVS(UINT8 uEnable)
{
    if(uEnable)
	{
	    fgSpeIPTVSEn = SV_TRUE;
	}
	else
	{
	    fgSpeIPTVSEn = SV_FALSE;
	}
		
}


BOOL fgIsBrowseInput(void)
{
   return fgBrowserModeEn;
}

BOOL fgIsSpeIPTVS(void)
{
    return fgSpeIPTVSEn;
}
UINT32 fgIsSupportFixMode(void)
{
   #if defined(SUPPORT_CUSTOMER_FRAME_RATE_SPEC) && defined(CC_MT5890)
    UINT32 u4VdoFrameRate;
    u4VdoFrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
    u4VdoFrameRate >>= (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_FS) ? 1 : 0;
    if(fgIsBrowseInput() || fgIsSpeIPTVS())
	{   
    #ifndef CC_SUPPORT_4K2K
		return (PANEL_IsSupport120Hz() && ((u4VdoFrameRate == 8) || (u4VdoFrameRate == 24) || (u4VdoFrameRate == 48))) ? FALSE : TRUE;
    #else
		return TRUE;
    #endif  
	}
	else
	{
        return FALSE;
	}
	#else
	return FALSE;
	#endif
}
UINT32 fgIsSupportFlickerLessMode(void)
{
    #if defined(SUPPORT_CUSTOMER_FRAME_RATE_SPEC) && defined(CC_MT5890)
	    UINT32 u4VdoFrameRate;
	    u4VdoFrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
	    u4VdoFrameRate >>= (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_FS) ? 1 : 0;
		
	    if((u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF) 
		#ifndef CC_SUPPORT_4K2K//fhd120hz model or fhd60hz model
			&& (MJC_GetEffectLevel() != 0) && (PANEL_IsSupport120Hz())//only for 120hz model
		#endif
		)
		{
		    #ifndef CC_SUPPORT_4K2K
			return ((u4VdoFrameRate == 8) || (u4VdoFrameRate == 24) || (u4VdoFrameRate == 48)) ? FALSE : TRUE;//for fhd120hz model, 48hz input have no flickless mode
			#else
			return TRUE;
			#endif
	}
	else
	{
        return FALSE;
	}
	#else
	return FALSE;
	#endif
}
UINT32 u4DrvSetCustFRC(UINT32 u4Mask)
{
    if (u1CustFRC != u4Mask)
    {
        u1CustFRC = u4Mask;
        vDrvCalPanelFrameRate(wDrvVideoGetVTotal(SV_VP_MAIN), bDrvVideoGetRefreshRate(SV_VP_MAIN));
        vVrmOnVdoModeChangeDone(SV_VP_MAIN);
    }
    return u1CustFRC;
}

UINT32 u4DrvGetCustFRC(void)
{
    return u1CustFRC;
}

void vDrvSetFixedFrameRate(UINT8 u1FrameRate)
{
    _bForceFrameRate = u1FrameRate;
}

extern UINT8 DRVCUST_GetRealCinema(void);
void vDrvCalPanelFrameRate(UINT16 u2VdoVTotal, UINT32 u4VdoFrameRate)
{
    UINT32 u4DCLKType;
    UINT32 u4PanelFrameRate;
    UINT32 u4PanelRatio;

    u4PanelRatio = u4DrvCalPanelRatio();
    u4VdoFrameRate >>= (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_FS) ? 1 : 0;
    if (_bForceFrameRate != 0)
    {
        u4VdoFrameRate = _bForceFrameRate;
    }

    switch (u4VdoFrameRate)
    {
        case 25:
        case 50:
        case 100:
            u4DCLKType = (fgDrvIsSupport50To60() || fgIsSupportFixMode() || fgIsSupportFlickerLessMode()) ? SV_DCLK_60HZ : SV_DCLK_50HZ;
            break;
        case 30:
        case 60:
        case 120:
            u4DCLKType = fgDrvIsSupport60To50() ? SV_DCLK_50HZ : SV_DCLK_60HZ;
            break;
        case 24:
            u4DCLKType = (fgDrvIsSupport48() && (!(fgIsSupportFixMode() || fgIsSupportFlickerLessMode()) || (DRVCUST_GetRealCinema() == SV_TRUE))) ? SV_DCLK_48HZ : SV_DCLK_60HZ;
            break;
        case 48:
            if (SUPPORT_PANEL_48HZ)
            {
	            if(fgIsSupportFixMode() || fgIsSupportFlickerLessMode())
				{
				    u4DCLKType = SV_DCLK_60HZ;
				}
				else
				{
				    u4DCLKType = SV_DCLK_48HZ;
	    	    }
            }
            else
            {
            	u4DCLKType = SV_DCLK_60HZ;
            }
            break;
        case 20:
            if (fgIsMJCToOSTG())
            {
                #ifndef SUPPORT_20TO60
                u4DCLKType = SV_DCLK_50HZ;
                break;
                #endif
            }
        case 12:
        case 15:
            u4DCLKType = SV_DCLK_60HZ;
            break;
#ifdef SUPPORT_CUSTOMER_FRAME_RATE_SPEC
		case 8:
		u4DCLKType = (fgDrvIsSupport48() && !(fgIsSupportFixMode() || fgIsSupportFlickerLessMode())) ? SV_DCLK_48HZ : SV_DCLK_60HZ;
		break;
#endif
        default:
        #ifdef SUPPORT_CUSTOMER_FRAME_RATE_SPEC
			u4DCLKType = SV_DCLK_60HZ;
		#else
            if ((u4VdoFrameRate < 25) || 
                (u4VdoFrameRate > 30 && u4VdoFrameRate < 50) || 
                (u4VdoFrameRate > 60 && u4VdoFrameRate < 100))
            {
                u4DCLKType = SV_DCLK_50HZ;
            }
            else
            {
                u4DCLKType = SV_DCLK_60HZ;
            }
		#endif
            break;
    }
#ifdef SUPPORT_CUSTOMER_FRAME_RATE_SPEC
    if(bIsScalerInput444(SV_VP_MAIN)) //444 case
	{
	    u4DCLKType = SV_DCLK_60HZ;
	}
#endif

    u4PanelFrameRate = (u4DCLKType == SV_DCLK_48HZ)? 48 :
                       (u4DCLKType == SV_DCLK_50HZ)? 50 : 60;
    u4PanelFrameRate <<= u4PanelRatio;

    if  (u4VdoFrameRate == 24 || u4VdoFrameRate == 25 || u4VdoFrameRate == 30)
    {
        #if !defined(CC_MT5890) && !defined(CC_MT5861)
        #if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
        if (_u14K2KMode == E_4K2K_NATIVE)
        {
            u4PanelFrameRate = u4VdoFrameRate;
        }
        #endif
        #endif
        if (DRVCUST_PanelGet(ePanel242530pOutput) == 1)
        {
            u4DCLKType = SV_DCLK_OTHERS;
            u4PanelFrameRate = u4VdoFrameRate;
        }
    }

    if (_bForceFrameRate != 0)
    {
        u4PanelFrameRate = _bForceFrameRate;

        // Customer requests freerun at picture wizard mode to prevent VSYNC flicker for PDP
        fgDDDSEn = (IS_PANEL_2D_N_3D_L12R12) ? SV_OFF : SV_ON;
    }
    else if (fgDrvDisplayGetPentouchMode())
    {
        u4PanelFrameRate = 60;
        fgDDDSEn = SV_OFF;
    }
    else
    {
        fgDDDSEn = SV_ON;
    }

    vDrvCalPanelTiming(u4DCLKType, u4PanelFrameRate);
    vDrvSetMTKGoodDclk(u2VdoVTotal, u4VdoFrameRate);
    vFrameTrackDDDSConfig(0, 0);
}

void vDddsSettingCheck(void)
{

#if CC_OSD_MUTE_WA
    //channel change will reset out region
    if (_bDddsDelayFlag && (!(_fgAutoSearch || SRM_IsEpgMode())) && SRM_IsMainVideoStrictStable())
    {
        vDrvCalPanelFrameRate(wDrvVideoGetVTotal(SV_VP_MAIN), bDrvVideoGetRefreshRate(SV_VP_MAIN));
        // reset non-standard signal status
        _u4NSDHTotal=0xff;
        _u4NSDVTotal=0xff;
        _u4NSDFrameRate=0xff;
        _bDddsDelayFlag = 0 ;
        #ifdef DRV_SUPPORT_EXTMJC
        vDrvExtmjcRedoTwoChipConnection( SV_VP_MAIN, SV_TRUE);
        #endif
		#if defined(CC_MT5399) 
		vDrvMJCSetModeChangeTrig();
		#endif
    }
#endif

    if ((_fgFRS_ENA == SV_ON) && fgIsMainTvd3d() 
        && (_eSyncLockMode == eSYNCLOCK_IGNORED))  	// Switch MTK Good Clock
    {
        if (bhwVCRSetting||(_na_state == 0x03)) //||!_fgHLOCK) //When VCR mode or Signal unsatble will OFF
        {
            vDrvSwitchMTKGoodDclk(SV_OFF);
        }
        else
        {
            vDrvSwitchMTKGoodDclk(SV_ON);
        }
    }
}

void vDrvDDDSLockStatus(UINT32 u4Count)
{
 UINT32 u4LoopCnt=u4Count;
 UINT32 u4UnlockCnt=0;
 static UINT8 u1CheckGoing=0;

 if (u1CheckGoing)
 {
	return;
 }
 do
 {
  u1CheckGoing = 1;
  if (IO32ReadFldAlign(DDDS_00,DDDS_00_DISP_EN)== 1) 
  {
  	if (!bDrvCheckMTKGoodStatus())
  	{
   	u4UnlockCnt++;
  	}
  	u4LoopCnt--;
  }	
  else
  {
	Printf("Open loop\n");
	u4LoopCnt = 0;
  }
 }while(u4LoopCnt);
 if(u4UnlockCnt)
 {
 	Printf("[UNLOCK] vDrvDDDSLockStatus check Finished unlock/check = %d / %d\n",u4UnlockCnt,u4Count);
 }	
 else
 {
 	if (IO32ReadFldAlign(DDDS_00,DDDS_00_DISP_EN)== 1)
 	{
 		Printf("[LOCK] vDrvDDDSLockStatus check Finished OK\n");
 	}	
 }
 u1CheckGoing = 0;
}


#ifdef CC_SCPOS_3DTV_SUPPORT 
 
static void updatePanelAttribute(UINT16 u2HTotal,
                                 UINT16 u2VTotal,
                                 UINT16 u2Width,
                                 UINT16 u2Height,
                                 UINT16 u2FrameRate)
{
    PANEL_SetPanelWidth(u2Width);
    PANEL_SetPanelHeight(u2Height);

    if(u2FrameRate >= 96)
    {
        PANEL_SetVClkMax(122);
    }
    else
    {
        PANEL_SetVClkMax(61);
    }

    if(u2FrameRate == 25 || u2FrameRate == 50 || u2FrameRate == 100)
    {
        PANEL_SetHTotal50Hz(u2HTotal);
        PANEL_SetVTotal50Hz(u2VTotal);
    }
    else if(SUPPORT_PANEL_48HZ && (u2FrameRate == 48 || u2FrameRate == 96))
    {
        PANEL_SetHTotal48Hz(u2HTotal);
        PANEL_SetVTotal48Hz(u2VTotal);
    }
    else
    {
        PANEL_SetHTotal60Hz(u2HTotal);
        PANEL_SetVTotal60Hz(u2VTotal);
    }

		// update OSD scaler parameters
    {
    UINT32 srcW=0,srcH=0,dstW=0,dstH=0,Enable=0,u4bpp=0;
    OSD_SC_GetScalerInfo(OSD_SCALER_3,&Enable,&srcW,&srcH,&dstW,&dstH,&u4bpp);
    OSD_SC_Scale(OSD_SCALER_3,Enable,srcW,srcH,u2Width,u2Height);
    OSD_SC_GetScalerInfo(OSD_SCALER_2,&Enable,&srcW,&srcH,&dstW,&dstH,&u4bpp);
    OSD_SC_Scale(OSD_SCALER_2,Enable,srcW,srcH,u2Width,u2Height);
    }
    
    return;
}

static void updatePanelAttribute_PDP(UINT16 u2HTotal,
                                 UINT16 u2VTotal,
                                 UINT32 u2PixelClk,                                 
                                 UINT16 u2Width,
                                 UINT16 u2Height,
                                 UINT16 u2VSyncWidth,                                                                  
                                 UINT16 u2VSyncFP,                                                                                                   
                                 UINT16 u2FrameRate)
{
    PANEL_SetPanelWidth(u2Width);
    PANEL_SetPanelHeight(u2Height);

    if(u2FrameRate >= 96)
    {
        PANEL_SetVClkMax(122);
    }
    else
    {
        PANEL_SetVClkMax(61);
    }

    if(u2FrameRate == 25 || u2FrameRate == 50 || u2FrameRate == 100)
    {
        PANEL_SetHTotal50Hz(u2HTotal);
        PANEL_SetVTotal50Hz(u2VTotal);
        PANEL_SetPixelClk50Hz(u2PixelClk);  
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        PANEL_SetVSyncWidthByTiming(SV_DCLK_50HZ,u2VSyncWidth);                
        PANEL_SetVSyncBpByTiming(SV_DCLK_50HZ, u2VSyncFP);                
        #endif
    }
    else if(SUPPORT_PANEL_48HZ && (u2FrameRate == 48 || u2FrameRate == 96))
    {
        PANEL_SetHTotal48Hz(u2HTotal);
        PANEL_SetVTotal48Hz(u2VTotal);
        PANEL_SetPixelClk48Hz(u2PixelClk);
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        PANEL_SetVSyncWidthByTiming(SV_DCLK_48HZ,u2VSyncWidth);
        PANEL_SetVSyncBpByTiming(SV_DCLK_48HZ, u2VSyncFP);
        #endif
    }
    else
    {
        PANEL_SetHTotal60Hz(u2HTotal);
        PANEL_SetVTotal60Hz(u2VTotal);
        PANEL_SetPixelClk60Hz(u2PixelClk);        
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        PANEL_SetVSyncWidthByTiming(SV_DCLK_60HZ,u2VSyncWidth);                
        PANEL_SetVSyncBpByTiming(SV_DCLK_60HZ, u2VSyncFP);                        
        #endif
    }

    return;
}

static UINT16 au2OrigHTotal[3];
static UINT16 au2OrigVTotal[3];
static UINT16 u2OrigWidth, u2OrigHeight;
static UINT32 au4OrigPixelMinMax[2];
static UINT8 u1OrigVClkMax; 

#if 0  // need to review later
typedef struct{
    UINT16 u2FrameRate;	
    UINT16 u2Width;
    UINT16 u2Height;	
    UINT16 u2HTotal;
    UINT16 u2VTotal;
    UINT32 u4PixelClk;	
    UINT16 u4VsyncWidth;	
    UINT16 u4VsyncBp;	
}S_PANEL_CONFIG;

typedef struct{
    UINT32 u4PixleClockMax;	
    UINT32 u4PixleClockMin;
    UINT16 u2VTotalMax;	
    UINT16 u2VTotalMin;		
    UINT16 u2VClkMax;	;	
}S_PANEL_CONFIG_COMMON;
#endif

S_PANEL_CONFIG rPANEL_2D[3]; 
S_PANEL_CONFIG rPANEL_3D[3];

S_PANEL_CONFIG_COMMON rPANEL_2D_COMMON;
S_PANEL_CONFIG_COMMON rPANEL_3D_COMMON;
static UINT32 u43DModeDclk = 0;
static UINT8 u13DModeRefreshRate = 0;
#define DBG_3D_PANEL_ATTRIBUTE 0
void vPanelAttribute_Status(void)
{
	#if DBG_3D_PANEL_ATTRIBUTE
	UINT8 u1Cnt=0;

	if(SUPPORT_PANEL_48HZ)
	{
		u1Cnt=SV_DCLK_48HZ;
	}
	else
	{
		u1Cnt=SV_DCLK_50HZ;
	}

	for( ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
    {
	Printf("2D_u2FrameRate[%d] = %d\n",u1Cnt, rPANEL_2D[u1Cnt].u2FrameRate);		
	Printf("2D_u2Width[%d] = %d\n",u1Cnt, rPANEL_2D[u1Cnt].u2Width);
	Printf("2D_u2Height[%d] = %d\n", u1Cnt,rPANEL_2D[u1Cnt].u2Height ); 	   
	Printf("2D_u2HTotal[%d] = %d\n", u1Cnt,rPANEL_2D[u1Cnt].u2HTotal);
	Printf("2D_u2VTotal[%d] = %d\n", u1Cnt,rPANEL_2D[u1Cnt].u2VTotal);		
	Printf("2D_u4PixelClk[%d] = %d\n", u1Cnt,rPANEL_2D[u1Cnt].u4PixelClk);			
	Printf("2D_u4VsyncWidth[%d] = %d\n", u1Cnt, rPANEL_2D[u1Cnt].u4VsyncWidth);  
	Printf("2D_u4VsyncBp[%d] = %d\n", u1Cnt, rPANEL_2D[u1Cnt].u4VsyncBp);		   
	
	Printf("3D_u2FrameRate[%d] = %d\n",u1Cnt, rPANEL_3D[u1Cnt].u2FrameRate);		
	Printf("3D_u2Width[%d] = %d\n",u1Cnt, rPANEL_3D[u1Cnt].u2Width);
	Printf("3D_u2Height[%d] = %d\n", u1Cnt,rPANEL_3D[u1Cnt].u2Height ); 	   
	Printf("3D_u2HTotal[%d] = %d\n", u1Cnt,rPANEL_3D[u1Cnt].u2HTotal);
	Printf("3D_u2VTotal[%d] = %d\n", u1Cnt,rPANEL_3D[u1Cnt].u2VTotal);		
	Printf("3D_u4PixelClk[%d] = %d\n", u1Cnt,rPANEL_3D[u1Cnt].u4PixelClk);			
	Printf("3D_u4VsyncWidth[%d] = %d\n", u1Cnt, rPANEL_3D[u1Cnt].u4VsyncWidth);  
	Printf("3D_u4VsyncBp[%d] = %d\n", u1Cnt, rPANEL_3D[u1Cnt].u4VsyncBp);		   
	}		 
	
	Printf("rPANEL_2D_COMMON.u4PixleClockMax = %d \n ", rPANEL_2D_COMMON.u4PixleClockMax);
	Printf("rPANEL_2D_COMMON.u4PixleClockMin = %d \n ", rPANEL_2D_COMMON.u4PixleClockMin);
	Printf("rPANEL_2D_COMMON.u2VTotalMax = %d \n ", rPANEL_2D_COMMON.u2VTotalMax);
	Printf("rPANEL_2D_COMMON.u2VTotalMin = %d \n ", rPANEL_2D_COMMON.u2VTotalMin);
	Printf("rPANEL_2D_COMMON.u2VClkMax = %d \n ", rPANEL_2D_COMMON.u2VClkMax);		
	
	Printf("rPANEL_3D_COMMON.u4PixleClockMax = %d \n ", rPANEL_3D_COMMON.u4PixleClockMax);
	Printf("rPANEL_3D_COMMON.u4PixleClockMin = %d \n ", rPANEL_3D_COMMON.u4PixleClockMin);
	Printf("rPANEL_3D_COMMON.u2VTotalMax = %d \n ", rPANEL_3D_COMMON.u2VTotalMax);
	Printf("rPANEL_3D_COMMON.u2VTotalMin = %d \n ", rPANEL_3D_COMMON.u2VTotalMin);
	Printf("rPANEL_3D_COMMON.u2VClkMax = %d \n ", rPANEL_3D_COMMON.u2VClkMax);	
	#endif
	
}

void vSavePanelAttribute_PDP(void)
{
		UINT8 u1Cnt=0;

		if(!IS_PANEL_2D_N_3D_L12R12)
		{
			return;
		}

        if(LVDS_OUTPUT_PORT == DUAL_PORT)
            fgPDPL12R12 = ePanel_PDP_L12R12; // L12R12
        else
            fgPDPL12R12 = ePanel_PDP_L1R1; // L1R1

		vIO32WriteFldAlign(DDDS_03, 0x09, DDDS_03_DDDS_ERR_LIM); // ERROR_LIMIT_FOR_PDP // plase remove to vDDDSInit		

		rPANEL_2D[SV_DCLK_60HZ].u2FrameRate = 60;
		rPANEL_2D[SV_DCLK_60HZ].u2Width = PANEL_GetPanelWidth();
		rPANEL_2D[SV_DCLK_60HZ].u2Height = PANEL_GetPanelHeight();
		rPANEL_2D[SV_DCLK_60HZ].u2HTotal = PANEL_GetHTotal60Hz();
		rPANEL_2D[SV_DCLK_60HZ].u2VTotal = PANEL_GetVTotal60Hz();
		rPANEL_2D[SV_DCLK_60HZ].u4PixelClk = PANEL_GetPixelClk60Hz();
		#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
		rPANEL_2D[SV_DCLK_60HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_60HZ);
		rPANEL_2D[SV_DCLK_60HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_60HZ);
		#else
		rPANEL_2D[SV_DCLK_60HZ].u4VsyncWidth = PANEL_GetVSyncWidth();
		rPANEL_2D[SV_DCLK_60HZ].u4VsyncBp = u4GetPanelVSyncBp();
		#endif

		rPANEL_2D[SV_DCLK_50HZ].u2FrameRate = 50;
		rPANEL_2D[SV_DCLK_50HZ].u2Width = PANEL_GetPanelWidth();
		rPANEL_2D[SV_DCLK_50HZ].u2Height = PANEL_GetPanelHeight();
		rPANEL_2D[SV_DCLK_50HZ].u2HTotal = PANEL_GetHTotal50Hz();
		rPANEL_2D[SV_DCLK_50HZ].u2VTotal = PANEL_GetVTotal50Hz();
		rPANEL_2D[SV_DCLK_50HZ].u4PixelClk = PANEL_GetPixelClk50Hz();		
		#ifdef SUPPORT_PANEL_CUSTOMER_SPEC		
		rPANEL_2D[SV_DCLK_50HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_50HZ);
		rPANEL_2D[SV_DCLK_50HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_50HZ);
		#else
		rPANEL_2D[SV_DCLK_50HZ].u4VsyncWidth = PANEL_GetVSyncWidth();
		rPANEL_2D[SV_DCLK_50HZ].u4VsyncBp = u4GetPanelVSyncBp();
		#endif

        if (SUPPORT_PANEL_48HZ)
        {
	        rPANEL_2D[SV_DCLK_48HZ].u2FrameRate = 48;
	        rPANEL_2D[SV_DCLK_48HZ].u2Width = PANEL_GetPanelWidth();
	        rPANEL_2D[SV_DCLK_48HZ].u2Height = PANEL_GetPanelHeight();
	        rPANEL_2D[SV_DCLK_48HZ].u2HTotal = PANEL_GetHTotal48Hz();
	        rPANEL_2D[SV_DCLK_48HZ].u2VTotal = PANEL_GetVTotal48Hz();
	        rPANEL_2D[SV_DCLK_48HZ].u4PixelClk = PANEL_GetPixelClk48Hz();
	        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
	        rPANEL_2D[SV_DCLK_48HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_48HZ);
	        rPANEL_2D[SV_DCLK_48HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_48HZ);
	        #else
	        rPANEL_2D[SV_DCLK_48HZ].u4VsyncWidth = PANEL_GetVSyncWidth();
	        rPANEL_2D[SV_DCLK_48HZ].u4VsyncBp = u4GetPanelVSyncBp();
			#endif
		}

		rPANEL_2D_COMMON.u4PixleClockMax = PANEL_GetPixelClkMax();
		rPANEL_2D_COMMON.u4PixleClockMin = PANEL_GetPixelClkMin();
		rPANEL_2D_COMMON.u2VTotalMax = PANEL_GetVTotalMax();
		rPANEL_2D_COMMON.u2VTotalMin = PANEL_GetVTotalMin();
		rPANEL_2D_COMMON.u2VClkMax = PANEL_GetVClkMax();

		
		if(SUPPORT_PANEL_48HZ)
		{
			u1Cnt=SV_DCLK_48HZ;
		}
		else
		{
			u1Cnt=SV_DCLK_50HZ;
		}

        for( ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
        {
			rPANEL_3D[u1Cnt].u2FrameRate = rPANEL_2D[u1Cnt].u2FrameRate;        
			rPANEL_3D[u1Cnt].u2Width = rPANEL_2D[u1Cnt].u2Width;
			rPANEL_3D[u1Cnt].u2Height = rPANEL_2D[u1Cnt].u2Height << 1;
			rPANEL_3D[u1Cnt].u2HTotal = rPANEL_2D[u1Cnt].u2HTotal;
			rPANEL_3D[u1Cnt].u2VTotal = rPANEL_2D[u1Cnt].u2VTotal << 1;
			rPANEL_3D[u1Cnt].u4PixelClk = rPANEL_2D[u1Cnt].u4PixelClk << 1;
			rPANEL_3D[u1Cnt].u4VsyncWidth = rPANEL_2D[u1Cnt].u4VsyncWidth << 1;
			rPANEL_3D[u1Cnt].u4VsyncBp = rPANEL_2D[u1Cnt].u4VsyncBp << 1;
        }

		rPANEL_3D_COMMON.u4PixleClockMax = rPANEL_2D_COMMON.u4PixleClockMax << 1;
		rPANEL_3D_COMMON.u4PixleClockMin = rPANEL_2D_COMMON.u4PixleClockMin << 1;
		rPANEL_3D_COMMON.u2VTotalMax = rPANEL_2D_COMMON.u2VTotalMax << 1;
		rPANEL_3D_COMMON.u2VTotalMin = rPANEL_2D_COMMON.u2VTotalMin << 1;
		rPANEL_3D_COMMON.u2VClkMax = rPANEL_2D_COMMON.u2VClkMax << 1;	

		vPanelAttribute_Status();

}        

static void vSavePanelAttribute(void)
{
    u2OrigWidth = PANEL_GetPanelWidth();
    u2OrigHeight = PANEL_GetPanelHeight();

    au2OrigHTotal[0] = PANEL_GetHTotal60Hz();
    au2OrigVTotal[0] = PANEL_GetVTotal60Hz();

    au2OrigHTotal[1] = PANEL_GetHTotal50Hz();
    au2OrigVTotal[1] = PANEL_GetVTotal50Hz();

	if (SUPPORT_PANEL_48HZ)
	{
	    au2OrigHTotal[2] = PANEL_GetHTotal48Hz();
	    au2OrigVTotal[2] = PANEL_GetVTotal48Hz();
	}

    au4OrigPixelMinMax[0] = PANEL_GetPixelClkMin();
    au4OrigPixelMinMax[1] = PANEL_GetPixelClkMax();
    u1OrigVClkMax = PANEL_GetVClkMax();
    
}

void vDrvDisp3DModeChg(UINT8 u13DMode)
{
    static UINT8 u1fg3DMode_Backup=0; 
    UINT8 u1fg3DMode=0; 
    UINT8 u1fg3DMode_ModChg=0;     
    UINT32 u4CtlWord;
    UINT8 u1Cnt=0;
	UINT32 dwDCLK = PANEL_GetPixelClk60Hz();


	#if DBG_3D_PANEL_ATTRIBUTE
    Printf("[PDP_NEW] vDrvDisp3DModeChg u13DMode= %d\n", u13DMode);
	#endif
	
	if(!IS_PANEL_2D_N_3D_L12R12)
	{
		return;
	}
		
    if(u13DMode)
    {
        u1fg3DMode = 1;
    }
    else
    {
        u1fg3DMode = 0;
    }
   
    if (u1fg3DMode != u1fg3DMode_Backup)
    {
        u1fg3DMode_ModChg = 1;
        u1fg3DMode_Backup = u1fg3DMode;
    }
    else
    {
        u1fg3DMode_ModChg =0;
    }   

	#if DBG_3D_PANEL_ATTRIBUTE
	vPanelAttribute_Status();
	#endif

	#if DBG_3D_PANEL_ATTRIBUTE
    Printf("[PDP_NEW] vDrvDisp3DModeChg u1fg3DMode_ModChg= %d\n", u1fg3DMode_ModChg);
	#endif
    
    if(u1fg3DMode_ModChg)
    {

	vDrvSetAllMute(SV_ON);

    vDrvLVDSOff();
					
    if(u1fg3DMode)
    {     
        if(fgPDPL12R12)
            PANEL_SetLvdsPixel(2); // DUAL -> FHD
        else
            PANEL_SetLvdsPixel(1); // SINGLE -> DUAL
    }
    else
    {
        if(fgPDPL12R12)
           PANEL_SetLvdsPixel(1); // DUAL
        else
           PANEL_SetLvdsPixel(0); // SINGLE
    }
    
    u4CtlWord = PANEL_GetControlWord() & 0xFBFFFFFF;
	if(SUPPORT_PANEL_48HZ)
	{
		u1Cnt=SV_DCLK_48HZ;
	}
	else
	{
		u1Cnt=SV_DCLK_50HZ;
	}

    if(u1fg3DMode)
    {   
    	u4CtlWord = u4CtlWord | PANEL_L12R12;
		fgIsVsyncPDPInit = SV_FALSE;

		for( ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
        {
			updatePanelAttribute_PDP(rPANEL_3D[u1Cnt].u2HTotal,
                                rPANEL_3D[u1Cnt].u2VTotal,
                                rPANEL_3D[u1Cnt].u4PixelClk,
                                rPANEL_3D[u1Cnt].u2Width,
                                rPANEL_3D[u1Cnt].u2Height,
                                rPANEL_3D[u1Cnt].u4VsyncWidth,
                                rPANEL_3D[u1Cnt].u4VsyncBp,                                     
                                rPANEL_3D[u1Cnt].u2FrameRate);
		}			
		
        PANEL_SetPixelClkMax(rPANEL_3D_COMMON.u4PixleClockMax);		
        PANEL_SetPixelClkMin(rPANEL_3D_COMMON.u4PixleClockMin);
        PANEL_SetVTotalMax(rPANEL_3D_COMMON.u2VTotalMax);		
        PANEL_SetVTotalMin(rPANEL_3D_COMMON.u2VTotalMin);		
        PANEL_SetVClkMax(rPANEL_3D_COMMON.u2VClkMax);
     }
     else
     {
		 for( ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
		 {
			 updatePanelAttribute_PDP(rPANEL_2D[u1Cnt].u2HTotal,
								 rPANEL_2D[u1Cnt].u2VTotal,
								 rPANEL_2D[u1Cnt].u4PixelClk,
								 rPANEL_2D[u1Cnt].u2Width,
								 rPANEL_2D[u1Cnt].u2Height,
								 rPANEL_2D[u1Cnt].u4VsyncWidth,
								 rPANEL_2D[u1Cnt].u4VsyncBp,									 
								 rPANEL_2D[u1Cnt].u2FrameRate);
		 }			 		 

        PANEL_SetPixelClkMax(rPANEL_2D_COMMON.u4PixleClockMax);		
        PANEL_SetPixelClkMin(rPANEL_2D_COMMON.u4PixleClockMin);
        PANEL_SetVTotalMax(rPANEL_2D_COMMON.u2VTotalMax);		
        PANEL_SetVTotalMin(rPANEL_2D_COMMON.u2VTotalMin);		
        PANEL_SetVClkMax(rPANEL_2D_COMMON.u2VClkMax);		
        }
	 
    PANEL_SetControlWord(u4CtlWord);        
    vDrvLVDSConfig();
    vDrvLVDSInit();
    vErrorHandleSetByPanel();
    if ((bSI_DISPLAY_DCLK_TYPE == SV_DCLK_60HZ) || (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_OTHERS))	
    {
		dwDCLK = PANEL_GetPixelClk60Hz();
    }
    else if (SUPPORT_PANEL_48HZ && (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_48HZ))
    {
        dwDCLK = PANEL_GetPixelClk48Hz();
    }
	else // (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_50HZ)
	{
		dwDCLK = PANEL_GetPixelClk50Hz();
	}

	#if DBG_3D_PANEL_ATTRIBUTE
    Printf("[PDP_NEW] dwDCLK %d\n", dwDCLK);
	#endif    

	#ifdef CC_MTK_LOADER
	vDrvVOPLLFreqSet(dwDCLK);
	#else
    vDrvVOFreqSet(dwDCLK);		
	#endif
	fgIsVsyncIsrStart = 0;
    vDrvSetPanelTiming();
	
	OSD_BASE_SwitchResolution();

	#if defined(CC_MT5399)
	vDrvMJCSetModeChangeTrig();
	#endif
	
	if(bDrvVideoSignalStatus(SV_VP_MAIN) == (UINT8)SV_VDO_STABLE)
	{
		vSetMainFlg(MAIN_FLG_MODE_CHG);
		vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
	}
    vDrvLVDSOn();
	vDrvSetAllMute(SV_OFF);	
    }

    return;
}

static void vDrvDisp3DModeCB(void)
{
    S_TDTV_OSTG_CONFIG rOstgCnf;
    //UINT16 u2FrameRate; 

    rOstgCnf = eDrvTDTVOSTGModeQuery();
    if(rOstgCnf.eOSTGCfg == E_TDTV_OSTG_CFG_BY_PANEL_TBL)
    {
        //resotred the panel attributes
#ifdef PANEL_SUPPORT_48HZ
        updatePanelAttribute(au2OrigHTotal[2],
                             au2OrigVTotal[2],
                             u2OrigWidth,
                             u2OrigHeight,
                             48);
#endif
        updatePanelAttribute(au2OrigHTotal[1],
                             au2OrigVTotal[1],
                             u2OrigWidth,
                             u2OrigHeight,
                             50);
        updatePanelAttribute(au2OrigHTotal[0],
                             au2OrigVTotal[0],
                             u2OrigWidth,
                             u2OrigHeight,
                             60);

        u43DModeDclk = 0;
        PANEL_SetPixelClkMin(au4OrigPixelMinMax[0]);
        PANEL_SetPixelClkMax(au4OrigPixelMinMax[1]);
        PANEL_SetVClkMax(u1OrigVClkMax);
    }
    else
    {
        updatePanelAttribute(rOstgCnf.u4HTotal,
                             rOstgCnf.u4VTotal,
                             rOstgCnf.u4Width,
                             rOstgCnf.u4Height,
                             rOstgCnf.u4FrameRate);

        u43DModeDclk = rOstgCnf.u4Clk;
        u13DModeRefreshRate = rOstgCnf.u4FrameRate;

        // clock range to cover 24/25/30Hz output clock
        PANEL_SetPixelClkMin(50000000);
    }
    vDrvOutputStageInit();
    vDrvLVDSConfig();	
    vErrorHandleSetByPanel();
    vDrvSetPanelTiming();
    #ifdef DRV_SUPPORT_EXTMJC
    vDrvExtmjcRedoTwoChipConnection( SV_VP_MAIN, SV_TRUE);
    #endif
#ifdef DRV_SUPPORT_MT8283
    fgForceChangeRegion = TRUE;
#endif
    return;
}

UINT32 u4DrvGetDisp3DModeDclk(void)
{
    return u43DModeDclk;
}

UINT8 u1DrvGetDisp3DModeRefreshRate(void)
{
    return u13DModeRefreshRate;
}

#endif

#endif

#if defined(CC_FAST_INIT) || defined(CC_SUPPORT_STR)
extern void VOPLL_pm_resume(void);
void vDrvDisplayInit_pm_resume(void)
{
  bSI_DISPLAY_DCLK_TYPE = SV_DCLK_UNKNOW;
   u1FlipMirrorReset = 1;
  VOPLL_pm_resume();
  #if SUPPORT_DISPLAY
  printf("[SA7]vApiHwInit\n");	
  vDrvLVDSInit();
  vErrorHandleInit();
  vDrvDisplayInit();
  LOG(0, "Display Init");
  #endif
}
#endif

#define FRC_UNMUTE_DELAY 500 // 2 sec

#ifdef SUPPORT_DGO
static void vDGOUnmute(void *pvArgs)
{
	while(TRUE)
	{
		VERIFY (x_sema_lock(_hDGOUnmuteSema, X_SEMA_OPTION_WAIT) == OSR_OK);	
		vDrvSetAllMute(SV_OFF); 
		Printf("\n.....DGO Unmute\n");	
	}	
}
static VOID _DGOUnMuteTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
	x_sema_unlock(_hDGOUnmuteSema);
}

#define DBG_DGO_PANEL_ATTRIBUTE 0
void vPanelAttribute_DGO_Status(void)
{
#if DBG_DGO_PANEL_ATTRIBUTE
	UINT8 u1Cnt=0;

	#ifdef SUPPORT_PANEL_48HZ
	for(u1Cnt=SV_DCLK_48HZ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
	#else	
	for(u1Cnt=SV_DCLK_50HZ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
	#endif	
	{
		Printf("480P_u2FrameRate[%d] = %d\n",u1Cnt, rPANEL_480P[u1Cnt].u2FrameRate);		
		Printf("480P_u2Width[%d] = %d\n",u1Cnt, rPANEL_480P[u1Cnt].u2Width);
		Printf("480P_u2Height[%d] = %d\n", u1Cnt,rPANEL_480P[u1Cnt].u2Height ); 	   
		Printf("480P_u2HTotal[%d] = %d\n", u1Cnt,rPANEL_480P[u1Cnt].u2HTotal);
		Printf("480P_u2VTotal[%d] = %d\n", u1Cnt,rPANEL_480P[u1Cnt].u2VTotal);		
		Printf("480P_u4PixelClk[%d] = %d\n", u1Cnt,rPANEL_480P[u1Cnt].u4PixelClk);			
		Printf("480P_u4VsyncWidth[%d] = %d\n", u1Cnt, rPANEL_480P[u1Cnt].u4VsyncWidth);  
		Printf("480P_u4VsyncBp[%d] = %d\n", u1Cnt, rPANEL_480P[u1Cnt].u4VsyncBp);		   
		
		Printf("576P_u2FrameRate[%d] = %d\n",u1Cnt, rPANEL_576P[u1Cnt].u2FrameRate);		
		Printf("576P_u2Width[%d] = %d\n",u1Cnt, rPANEL_576P[u1Cnt].u2Width);
		Printf("576P_u2Height[%d] = %d\n", u1Cnt,rPANEL_576P[u1Cnt].u2Height ); 	   
		Printf("576P_u2HTotal[%d] = %d\n", u1Cnt,rPANEL_576P[u1Cnt].u2HTotal);
		Printf("576P_u2VTotal[%d] = %d\n", u1Cnt,rPANEL_576P[u1Cnt].u2VTotal);		
		Printf("576P_u4PixelClk[%d] = %d\n", u1Cnt,rPANEL_576P[u1Cnt].u4PixelClk);			
		Printf("576P_u4VsyncWidth[%d] = %d\n", u1Cnt, rPANEL_576P[u1Cnt].u4VsyncWidth);  
		Printf("576P_u4VsyncBp[%d] = %d\n", u1Cnt, rPANEL_576P[u1Cnt].u4VsyncBp);	

		Printf("720P_u2FrameRate[%d] = %d\n",u1Cnt, rPANEL_720P[u1Cnt].u2FrameRate);		
		Printf("720P_u2Width[%d] = %d\n",u1Cnt, rPANEL_720P[u1Cnt].u2Width);
		Printf("720P_u2Height[%d] = %d\n", u1Cnt,rPANEL_720P[u1Cnt].u2Height ); 	   
		Printf("720P_u2HTotal[%d] = %d\n", u1Cnt,rPANEL_720P[u1Cnt].u2HTotal);
		Printf("720P_u2VTotal[%d] = %d\n", u1Cnt,rPANEL_720P[u1Cnt].u2VTotal);		
		Printf("720P_u4PixelClk[%d] = %d\n", u1Cnt,rPANEL_720P[u1Cnt].u4PixelClk);			
		Printf("720P_u4VsyncWidth[%d] = %d\n", u1Cnt, rPANEL_720P[u1Cnt].u4VsyncWidth);  
		Printf("720P_u4VsyncBp[%d] = %d\n", u1Cnt, rPANEL_720P[u1Cnt].u4VsyncBp);

		Printf("1080P_u2FrameRate[%d] = %d\n",u1Cnt, rPANEL_1080P[u1Cnt].u2FrameRate);		
		Printf("1080P_u2Width[%d] = %d\n",u1Cnt, rPANEL_1080P[u1Cnt].u2Width);
		Printf("1080P_u2Height[%d] = %d\n", u1Cnt,rPANEL_1080P[u1Cnt].u2Height ); 	   
		Printf("1080P_u2HTotal[%d] = %d\n", u1Cnt,rPANEL_1080P[u1Cnt].u2HTotal);
		Printf("1080P_u2VTotal[%d] = %d\n", u1Cnt,rPANEL_1080P[u1Cnt].u2VTotal);		
		Printf("1080P_u4PixelClk[%d] = %d\n", u1Cnt,rPANEL_1080P[u1Cnt].u4PixelClk);			
		Printf("1080P_u4VsyncWidth[%d] = %d\n", u1Cnt, rPANEL_1080P[u1Cnt].u4VsyncWidth);  
		Printf("1080P_u4VsyncBp[%d] = %d\n", u1Cnt, rPANEL_1080P[u1Cnt].u4VsyncBp);
	}		 

	Printf("480P_COMMON.u4PixleClockMax = %d \n ", rPANEL_480P_COMMON.u4PixleClockMax);
	Printf("480P_COMMON.u4PixleClockMin = %d \n ", rPANEL_480P_COMMON.u4PixleClockMin);
	Printf("480P_COMMON.u2HTotalMax = %d \n ", rPANEL_480P_COMMON.u2HTotalMax);
	Printf("480P_COMMON.u2HTotalMin = %d \n ", rPANEL_480P_COMMON.u2HTotalMin);
	Printf("480P_COMMON.u2VTotalMax = %d \n ", rPANEL_480P_COMMON.u2VTotalMax);
	Printf("480P_COMMON.u2VTotalMin = %d \n ", rPANEL_480P_COMMON.u2VTotalMin);
	Printf("480P_COMMON.u2VClkMax = %d \n ", rPANEL_480P_COMMON.u2VClkMax);	
	Printf("480P_COMMON.u2VClkMin = %d \n ", rPANEL_480P_COMMON.u2VClkMin);

	Printf("576P_COMMON.u4PixleClockMax = %d \n ", rPANEL_576P_COMMON.u4PixleClockMax);
	Printf("576P_COMMON.u4PixleClockMin = %d \n ", rPANEL_576P_COMMON.u4PixleClockMin);
	Printf("576P_COMMON.u2HTotalMax = %d \n ", rPANEL_576P_COMMON.u2HTotalMax);
	Printf("576P_COMMON.u2HTotalMin = %d \n ", rPANEL_576P_COMMON.u2HTotalMin);
	Printf("576P_COMMON.u2VTotalMax = %d \n ", rPANEL_576P_COMMON.u2VTotalMax);
	Printf("576P_COMMON.u2VTotalMin = %d \n ", rPANEL_576P_COMMON.u2VTotalMin);
	Printf("576P_COMMON.u2VClkMax = %d \n ", rPANEL_576P_COMMON.u2VClkMax);	
	Printf("576P_COMMON.u2VClkMin = %d \n ", rPANEL_576P_COMMON.u2VClkMin);

	Printf("720P_COMMON.u4PixleClockMax = %d \n ", rPANEL_720P_COMMON.u4PixleClockMax);
	Printf("720P_COMMON.u4PixleClockMin = %d \n ", rPANEL_720P_COMMON.u4PixleClockMin);
	Printf("720P_COMMON.u2HTotalMax = %d \n ", rPANEL_720P_COMMON.u2HTotalMax);
	Printf("720P_COMMON.u2HTotalMin = %d \n ", rPANEL_720P_COMMON.u2HTotalMin);
	Printf("720P_COMMON.u2VTotalMax = %d \n ", rPANEL_720P_COMMON.u2VTotalMax);
	Printf("720P_COMMON.u2VTotalMin = %d \n ", rPANEL_720P_COMMON.u2VTotalMin);
	Printf("720P_COMMON.u2VClkMax = %d \n ", rPANEL_720P_COMMON.u2VClkMax);	
	Printf("720P_COMMON.u2VClkMin = %d \n ", rPANEL_720P_COMMON.u2VClkMin);

	Printf("1080P_COMMON.u4PixleClockMax = %d \n ", rPANEL_1080P_COMMON.u4PixleClockMax);
	Printf("1080P_COMMON.u4PixleClockMin = %d \n ", rPANEL_1080P_COMMON.u4PixleClockMin);
	Printf("1080P_COMMON.u2HTotalMax = %d \n ", rPANEL_1080P_COMMON.u2HTotalMax);
	Printf("1080P_COMMON.u2HTotalMin = %d \n ", rPANEL_1080P_COMMON.u2HTotalMin);
	Printf("1080P_COMMON.u2VTotalMax = %d \n ", rPANEL_1080P_COMMON.u2VTotalMax);
	Printf("1080P_COMMON.u2VTotalMin = %d \n ", rPANEL_1080P_COMMON.u2VTotalMin);
	Printf("1080P_COMMON.u2VClkMax = %d \n ", rPANEL_1080P_COMMON.u2VClkMax);	
	Printf("1080P_COMMON.u2VClkMin = %d \n ", rPANEL_1080P_COMMON.u2VClkMin);
#endif
}


static void vSavePanelAttribute_DGO(void)
{
// DEFAULT
	rPANEL_DEFAULT[SV_DCLK_60HZ].u2FrameRate = 60; 
	rPANEL_DEFAULT[SV_DCLK_60HZ].u2Width = PANEL_GetPanelWidth();
	rPANEL_DEFAULT[SV_DCLK_60HZ].u2Height = PANEL_GetPanelHeight();
	rPANEL_DEFAULT[SV_DCLK_60HZ].u2HTotal = PANEL_GetHTotal60Hz();
	rPANEL_DEFAULT[SV_DCLK_60HZ].u2VTotal = PANEL_GetVTotal60Hz();
	rPANEL_DEFAULT[SV_DCLK_60HZ].u4PixelClk = PANEL_GetPixelClk60Hz();
	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
	rPANEL_DEFAULT[SV_DCLK_60HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_60HZ);
	rPANEL_DEFAULT[SV_DCLK_60HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_60HZ);
	#else
	rPANEL_DEFAULT[SV_DCLK_60HZ].u4VsyncWidth = PANEL_GetVSyncWidth();
	rPANEL_DEFAULT[SV_DCLK_60HZ].u4VsyncBp = u4GetPanelVSyncBp();
	#endif
	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
	rPANEL_DEFAULT[SV_DCLK_60HZ].u4HsyncWidth = PANEL_GetHSyncWidthByTiming(SV_DCLK_60HZ);
	#else
	rPANEL_DEFAULT[SV_DCLK_60HZ].u4HsyncWidth = PANEL_GetHSyncWidth();
	#endif
	rPANEL_DEFAULT[SV_DCLK_60HZ].u4HsyncFp = ((H_POS_60HZ > 0x800)?(H_POS_60HZ-0x800):(0));

	rPANEL_DEFAULT[SV_DCLK_50HZ].u2FrameRate = 50; 
	rPANEL_DEFAULT[SV_DCLK_50HZ].u2Width = PANEL_GetPanelWidth();
	rPANEL_DEFAULT[SV_DCLK_50HZ].u2Height = PANEL_GetPanelHeight();
	rPANEL_DEFAULT[SV_DCLK_50HZ].u2HTotal = PANEL_GetHTotal50Hz();
	rPANEL_DEFAULT[SV_DCLK_50HZ].u2VTotal = PANEL_GetVTotal50Hz();
	rPANEL_DEFAULT[SV_DCLK_50HZ].u4PixelClk = PANEL_GetPixelClk50Hz();
	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
	rPANEL_DEFAULT[SV_DCLK_50HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_50HZ);
	rPANEL_DEFAULT[SV_DCLK_50HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_50HZ);
	#else
	rPANEL_DEFAULT[SV_DCLK_50HZ].u4VsyncWidth = PANEL_GetVSyncWidth();
	rPANEL_DEFAULT[SV_DCLK_50HZ].u4VsyncBp = u4GetPanelVSyncBp();
	#endif
	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
	rPANEL_DEFAULT[SV_DCLK_50HZ].u4HsyncWidth = PANEL_GetHSyncWidthByTiming(SV_DCLK_50HZ);
	#else
	rPANEL_DEFAULT[SV_DCLK_50HZ].u4HsyncWidth = PANEL_GetHSyncWidth();
	#endif
	rPANEL_DEFAULT[SV_DCLK_50HZ].u4HsyncFp = ((H_POS_50HZ > 0x800)?(H_POS_50HZ-0x800):(0));

	#ifdef SUPPORT_PANEL_48HZ
	rPANEL_DEFAULT[SV_DCLK_48HZ].u2FrameRate = 24;		
	rPANEL_DEFAULT[SV_DCLK_48HZ].u2Width = PANEL_GetPanelWidth();
	rPANEL_DEFAULT[SV_DCLK_48HZ].u2Height = PANEL_GetPanelHeight();
	rPANEL_DEFAULT[SV_DCLK_48HZ].u2HTotal = PANEL_GetHTotal48Hz();
	rPANEL_DEFAULT[SV_DCLK_48HZ].u2VTotal = PANEL_GetVTotal48Hz();
	rPANEL_DEFAULT[SV_DCLK_48HZ].u4PixelClk = PANEL_GetPixelClk48Hz();
	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
	rPANEL_DEFAULT[SV_DCLK_48HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_48HZ);
	rPANEL_DEFAULT[SV_DCLK_48HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_48HZ);
	#else
	rPANEL_DEFAULT[SV_DCLK_48HZ].u4VsyncWidth = PANEL_GetVSyncWidth();
	rPANEL_DEFAULT[SV_DCLK_48HZ].u4VsyncBp = u4GetPanelVSyncBp();
	#endif
	#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
	rPANEL_DEFAULT[SV_DCLK_48HZ].u4HsyncWidth = PANEL_GetHSyncWidthByTiming(SV_DCLK_48HZ);
	rPANEL_DEFAULT[SV_DCLK_48HZ].u4HsyncFp =(PANEL_GetHPositionByTiming(SV_DCLK_48HZ) > 0x800)?(PANEL_GetHPositionByTiming(SV_DCLK_48HZ)-0x800):(0);
	#else
	rPANEL_DEFAULT[SV_DCLK_48HZ].u4HsyncWidth = PANEL_GetHSyncWidth();
	rPANEL_DEFAULT[SV_DCLK_48HZ].u4HsyncFp = ((H_POS_60HZ > 0x800)?(H_POS_60HZ-0x800):(0));
	#endif
	#endif

	rPANEL_DEFAULT_COMMON.u4PixleClockMax = PANEL_GetPixelClkMax();
	rPANEL_DEFAULT_COMMON.u4PixleClockMin = PANEL_GetPixelClkMin();
	rPANEL_DEFAULT_COMMON.u2HTotalMax = PANEL_GetHTotalMax();
	rPANEL_DEFAULT_COMMON.u2HTotalMin = PANEL_GetHTotalMin();
	rPANEL_DEFAULT_COMMON.u2VTotalMax = PANEL_GetVTotalMax();
	rPANEL_DEFAULT_COMMON.u2VTotalMin = PANEL_GetVTotalMin();
	rPANEL_DEFAULT_COMMON.u2VClkMax = PANEL_GetVClkMax();
	rPANEL_DEFAULT_COMMON.u2VClkMin= PANEL_GetVClkMin();

	_u4CtrlWord_DEFAULT = PANEL_GetControlWord();
	#ifdef EnablePANELCW2
	_u4CtrlWord2_DEFAULT = PANEL_GetControlWord2();
	#endif
	
// 480P
	rPANEL_480P[SV_DCLK_60HZ].u2FrameRate = 60; 
	rPANEL_480P[SV_DCLK_60HZ].u2Width = 720;
	rPANEL_480P[SV_DCLK_60HZ].u2Height = 480;
	rPANEL_480P[SV_DCLK_60HZ].u2HTotal = 858;
	rPANEL_480P[SV_DCLK_60HZ].u2VTotal = 525;
	rPANEL_480P[SV_DCLK_60HZ].u4PixelClk = 27003000;
	rPANEL_480P[SV_DCLK_60HZ].u4VsyncWidth = 6;
	rPANEL_480P[SV_DCLK_60HZ].u4VsyncBp = 30;
	rPANEL_480P[SV_DCLK_60HZ].u4HsyncWidth = 60;
	rPANEL_480P[SV_DCLK_60HZ].u4HsyncFp = 61;
	
	rPANEL_480P_COMMON.u4PixleClockMax = 30000000;
	rPANEL_480P_COMMON.u4PixleClockMin = 13500000;
	rPANEL_480P_COMMON.u2HTotalMax = 900;
	rPANEL_480P_COMMON.u2HTotalMin = 800;
	rPANEL_480P_COMMON.u2VTotalMax = 680;
	rPANEL_480P_COMMON.u2VTotalMin = 500;
	rPANEL_480P_COMMON.u2VClkMax = 61;
	rPANEL_480P_COMMON.u2VClkMin= 60;

// 576P
	rPANEL_576P[SV_DCLK_50HZ].u2FrameRate = 50;
	rPANEL_576P[SV_DCLK_50HZ].u2Width = 720;
	rPANEL_576P[SV_DCLK_50HZ].u2Height = 576;
	rPANEL_576P[SV_DCLK_50HZ].u2HTotal = 864;
	rPANEL_576P[SV_DCLK_50HZ].u2VTotal = 625;
	rPANEL_576P[SV_DCLK_50HZ].u4PixelClk = 27000000;		
	rPANEL_576P[SV_DCLK_50HZ].u4VsyncWidth = 5;
	rPANEL_576P[SV_DCLK_50HZ].u4VsyncBp = 37;
	rPANEL_576P[SV_DCLK_50HZ].u4HsyncWidth = 74;
	rPANEL_576P[SV_DCLK_50HZ].u4HsyncFp = 69;

	rPANEL_576P_COMMON.u4PixleClockMax = 35000000;
	rPANEL_576P_COMMON.u4PixleClockMin = rPANEL_480P_COMMON.u4PixleClockMin;
	rPANEL_576P_COMMON.u2HTotalMax = rPANEL_480P_COMMON.u2HTotalMax;
	rPANEL_576P_COMMON.u2HTotalMin = rPANEL_480P_COMMON.u2HTotalMin;
	rPANEL_576P_COMMON.u2VTotalMax = rPANEL_480P_COMMON.u2VTotalMax;
	rPANEL_576P_COMMON.u2VTotalMin = rPANEL_480P_COMMON.u2VTotalMin;
	rPANEL_576P_COMMON.u2VClkMax = 51;
	rPANEL_576P_COMMON.u2VClkMin= 49;

// 720P
	rPANEL_720P[SV_DCLK_60HZ].u2FrameRate = 60;
	rPANEL_720P[SV_DCLK_60HZ].u2Width = 1280;
	rPANEL_720P[SV_DCLK_60HZ].u2Height = 720;
	rPANEL_720P[SV_DCLK_60HZ].u2HTotal = 1650;
	rPANEL_720P[SV_DCLK_60HZ].u2VTotal = 750;
	rPANEL_720P[SV_DCLK_60HZ].u4PixelClk = 74250000;
	rPANEL_720P[SV_DCLK_60HZ].u4VsyncWidth = 5;
	rPANEL_720P[SV_DCLK_60HZ].u4VsyncBp = 19;
	rPANEL_720P[SV_DCLK_60HZ].u4HsyncWidth = 31;
	rPANEL_720P[SV_DCLK_60HZ].u4HsyncFp = 77;

	rPANEL_720P[SV_DCLK_50HZ].u2FrameRate = 50;
	rPANEL_720P[SV_DCLK_50HZ].u2Width = rPANEL_720P[SV_DCLK_60HZ].u2Width;
	rPANEL_720P[SV_DCLK_50HZ].u2Height = rPANEL_720P[SV_DCLK_60HZ].u2Height;
	rPANEL_720P[SV_DCLK_50HZ].u2HTotal = 1980;
	rPANEL_720P[SV_DCLK_50HZ].u2VTotal = rPANEL_720P[SV_DCLK_60HZ].u2VTotal;
	rPANEL_720P[SV_DCLK_50HZ].u4PixelClk = rPANEL_720P[SV_DCLK_60HZ].u4PixelClk;		
	rPANEL_720P[SV_DCLK_50HZ].u4VsyncWidth = rPANEL_720P[SV_DCLK_60HZ].u4VsyncWidth;
	rPANEL_720P[SV_DCLK_50HZ].u4VsyncBp = rPANEL_720P[SV_DCLK_60HZ].u4VsyncBp;
	rPANEL_720P[SV_DCLK_50HZ].u4HsyncWidth = 32;
	rPANEL_720P[SV_DCLK_50HZ].u4HsyncFp = 406;

	#if 0  //def SUPPORT_PANEL_48HZ
	rPANEL_720P[SV_DCLK_48HZ].u2FrameRate = 24;		
	rPANEL_720P[SV_DCLK_48HZ].u2Width = rPANEL_480P[SV_DCLK_60HZ].u2Width << 1;
	rPANEL_720P[SV_DCLK_48HZ].u2Height = rPANEL_480P[SV_DCLK_60HZ].u2Height << 1;
	rPANEL_720P[SV_DCLK_48HZ].u2HTotal = 5500;
	rPANEL_720P[SV_DCLK_48HZ].u2VTotal = rPANEL_480P[SV_DCLK_60HZ].u2VTotal << 1;  // 2250
	rPANEL_720P[SV_DCLK_48HZ].u4PixelClk = rPANEL_480P[SV_DCLK_60HZ].u4PixelClk;
	rPANEL_720P[SV_DCLK_48HZ].u4VsyncWidth = rPANEL_480P[SV_DCLK_60HZ].u4VsyncWidth;
	rPANEL_720P[SV_DCLK_48HZ].u4VsyncBp = rPANEL_480P[SV_DCLK_60HZ].u4VsyncBp;
	#endif

	rPANEL_720P_COMMON.u4PixleClockMax = 78700000;
	rPANEL_720P_COMMON.u4PixleClockMin = 69795000;
	rPANEL_720P_COMMON.u2HTotalMax = 2000;
	rPANEL_720P_COMMON.u2HTotalMin = 1400;
	rPANEL_720P_COMMON.u2VTotalMax = 800;
	rPANEL_720P_COMMON.u2VTotalMin = 500;
	rPANEL_720P_COMMON.u2VClkMax = 63;
	rPANEL_720P_COMMON.u2VClkMin= 47;	

// 1080P
	rPANEL_1080P[SV_DCLK_60HZ].u2FrameRate = 60;
	rPANEL_1080P[SV_DCLK_60HZ].u2Width = 1920;
	rPANEL_1080P[SV_DCLK_60HZ].u2Height = 1080;
	rPANEL_1080P[SV_DCLK_60HZ].u2HTotal = 2200;
	rPANEL_1080P[SV_DCLK_60HZ].u2VTotal = 1125;
	rPANEL_1080P[SV_DCLK_60HZ].u4PixelClk = 148500000;
	rPANEL_1080P[SV_DCLK_60HZ].u4VsyncWidth = 5;
	rPANEL_1080P[SV_DCLK_60HZ].u4VsyncBp = 35;
	rPANEL_1080P[SV_DCLK_60HZ].u4HsyncWidth = 30;
	rPANEL_1080P[SV_DCLK_60HZ].u4HsyncFp = 57;

	rPANEL_1080P[SV_DCLK_50HZ].u2FrameRate = 50;
	rPANEL_1080P[SV_DCLK_50HZ].u2Width = rPANEL_1080P[SV_DCLK_60HZ].u2Width;
	rPANEL_1080P[SV_DCLK_50HZ].u2Height = rPANEL_1080P[SV_DCLK_60HZ].u2Height;
	rPANEL_1080P[SV_DCLK_50HZ].u2HTotal = 2640;
	rPANEL_1080P[SV_DCLK_50HZ].u2VTotal = rPANEL_1080P[SV_DCLK_60HZ].u2VTotal;
	rPANEL_1080P[SV_DCLK_50HZ].u4PixelClk = rPANEL_1080P[SV_DCLK_60HZ].u4PixelClk;		
	rPANEL_1080P[SV_DCLK_50HZ].u4VsyncWidth = rPANEL_1080P[SV_DCLK_60HZ].u4VsyncWidth;
	rPANEL_1080P[SV_DCLK_50HZ].u4VsyncBp = rPANEL_1080P[SV_DCLK_60HZ].u4VsyncBp;
	rPANEL_1080P[SV_DCLK_50HZ].u4HsyncWidth = 30;
	rPANEL_1080P[SV_DCLK_50HZ].u4HsyncFp = 497;

	#ifdef SUPPORT_PANEL_48HZ
	rPANEL_1080P[SV_DCLK_48HZ].u2FrameRate = 48;		
	rPANEL_1080P[SV_DCLK_48HZ].u2Width = rPANEL_1080P[SV_DCLK_60HZ].u2Width;
	rPANEL_1080P[SV_DCLK_48HZ].u2Height = rPANEL_1080P[SV_DCLK_60HZ].u2Height;
	rPANEL_1080P[SV_DCLK_48HZ].u2HTotal = rPANEL_1080P[SV_DCLK_60HZ].u2HTotal;  // 2200
	rPANEL_1080P[SV_DCLK_48HZ].u2VTotal = rPANEL_1080P[SV_DCLK_60HZ].u2VTotal;  //1125
	rPANEL_1080P[SV_DCLK_48HZ].u4PixelClk = 118800000;
	rPANEL_1080P[SV_DCLK_48HZ].u4VsyncWidth = rPANEL_1080P[SV_DCLK_60HZ].u4VsyncWidth;
	rPANEL_1080P[SV_DCLK_48HZ].u4VsyncBp = rPANEL_1080P[SV_DCLK_60HZ].u4VsyncBp;
	rPANEL_1080P[SV_DCLK_48HZ].u4HsyncWidth = rPANEL_1080P[SV_DCLK_60HZ].u4HsyncWidth;
	rPANEL_1080P[SV_DCLK_48HZ].u4HsyncFp = rPANEL_1080P[SV_DCLK_60HZ].u4HsyncFp;
	#endif

	rPANEL_1080P_COMMON.u4PixleClockMax = 2 * 77500000;
	rPANEL_1080P_COMMON.u4PixleClockMin = 2 * 66970000;
	rPANEL_1080P_COMMON.u2HTotalMax = 2800;
	rPANEL_1080P_COMMON.u2HTotalMin = 2080;
	rPANEL_1080P_COMMON.u2VTotalMax = 1406;
	rPANEL_1080P_COMMON.u2VTotalMin = 1089;
	rPANEL_1080P_COMMON.u2VClkMax = 63;
	rPANEL_1080P_COMMON.u2VClkMin= 47;	

// 1080P 3D FP
	rPANEL_1080P_FP[SV_DCLK_60HZ].u2FrameRate = 60;
	rPANEL_1080P_FP[SV_DCLK_60HZ].u2Width = 1920;
	rPANEL_1080P_FP[SV_DCLK_60HZ].u2Height = 1080;
	rPANEL_1080P_FP[SV_DCLK_60HZ].u2HTotal = 2200;
	rPANEL_1080P_FP[SV_DCLK_60HZ].u2VTotal = 1125;
	rPANEL_1080P_FP[SV_DCLK_60HZ].u4PixelClk = 148500000;
	rPANEL_1080P_FP[SV_DCLK_60HZ].u4VsyncWidth = 5;
	rPANEL_1080P_FP[SV_DCLK_60HZ].u4VsyncBp = 35;
	rPANEL_1080P_FP[SV_DCLK_60HZ].u4HsyncWidth = 44;
	rPANEL_1080P_FP[SV_DCLK_60HZ].u4HsyncFp = 138;

	rPANEL_1080P_FP[SV_DCLK_50HZ].u2FrameRate = 50;
	rPANEL_1080P_FP[SV_DCLK_50HZ].u2Width = rPANEL_1080P[SV_DCLK_60HZ].u2Width;
	rPANEL_1080P_FP[SV_DCLK_50HZ].u2Height = rPANEL_1080P[SV_DCLK_60HZ].u2Height;
	rPANEL_1080P_FP[SV_DCLK_50HZ].u2HTotal = 2640;
	rPANEL_1080P_FP[SV_DCLK_50HZ].u2VTotal = rPANEL_1080P[SV_DCLK_60HZ].u2VTotal;
	rPANEL_1080P_FP[SV_DCLK_50HZ].u4PixelClk = rPANEL_1080P[SV_DCLK_60HZ].u4PixelClk;		
	rPANEL_1080P_FP[SV_DCLK_50HZ].u4VsyncWidth = rPANEL_1080P[SV_DCLK_60HZ].u4VsyncWidth;
	rPANEL_1080P_FP[SV_DCLK_50HZ].u4VsyncBp = rPANEL_1080P[SV_DCLK_60HZ].u4VsyncBp;
	rPANEL_1080P_FP[SV_DCLK_50HZ].u4HsyncWidth = rPANEL_1080P_FP[SV_DCLK_60HZ].u4HsyncWidth;
	rPANEL_1080P_FP[SV_DCLK_50HZ].u4HsyncFp = rPANEL_1080P_FP[SV_DCLK_60HZ].u4HsyncFp;

	#ifdef SUPPORT_PANEL_48HZ
	rPANEL_1080P_FP[SV_DCLK_48HZ].u2FrameRate = 48;		
	rPANEL_1080P_FP[SV_DCLK_48HZ].u2Width = rPANEL_1080P[SV_DCLK_60HZ].u2Width;
	rPANEL_1080P_FP[SV_DCLK_48HZ].u2Height = rPANEL_1080P[SV_DCLK_60HZ].u2Height;
	rPANEL_1080P_FP[SV_DCLK_48HZ].u2HTotal = 2750;  // 2200
	rPANEL_1080P_FP[SV_DCLK_48HZ].u2VTotal = rPANEL_1080P[SV_DCLK_60HZ].u2VTotal;  //1125
	rPANEL_1080P_FP[SV_DCLK_48HZ].u4PixelClk = 148500000;
	rPANEL_1080P_FP[SV_DCLK_48HZ].u4VsyncWidth = rPANEL_1080P[SV_DCLK_60HZ].u4VsyncWidth;
	rPANEL_1080P_FP[SV_DCLK_48HZ].u4VsyncBp = rPANEL_1080P[SV_DCLK_60HZ].u4VsyncBp;
	rPANEL_1080P_FP[SV_DCLK_48HZ].u4HsyncWidth = rPANEL_1080P_FP[SV_DCLK_60HZ].u4HsyncWidth;
	rPANEL_1080P_FP[SV_DCLK_48HZ].u4HsyncFp = rPANEL_1080P_FP[SV_DCLK_60HZ].u4HsyncFp;
	#endif

	rPANEL_1080P_FP_COMMON.u4PixleClockMax = 2 * 77500000;
	rPANEL_1080P_FP_COMMON.u4PixleClockMin = 2 * 66970000;
	rPANEL_1080P_FP_COMMON.u2HTotalMax = 2800;
	rPANEL_1080P_FP_COMMON.u2HTotalMin = 2080;
	rPANEL_1080P_FP_COMMON.u2VTotalMax = 1406;
	rPANEL_1080P_FP_COMMON.u2VTotalMin = 1089;
	rPANEL_1080P_FP_COMMON.u2VClkMax = 63;
	rPANEL_1080P_FP_COMMON.u2VClkMin= 47;
	
	vPanelAttribute_DGO_Status();
}  

static void updatePanelAttribute_DGO(UINT16 u2HTotal,
                                 UINT16 u2VTotal,
                                 UINT32 u2PixelClk,                                 
                                 UINT16 u2Width,
                                 UINT16 u2Height,
                                 UINT16 u2VSyncWidth,                                                                  
                                 UINT16 u2VSyncBP,
                                 UINT16 u2HsyncWdith,
                                 UINT16 u2HsyncFP,
                                 UINT16 u2FrameRate)
{
    PANEL_SetPanelWidth(u2Width);
    PANEL_SetPanelHeight(u2Height);

    if(u2FrameRate == 25 || u2FrameRate == 50 || u2FrameRate == 100)
    {
    	if (u2Height==720 || u2Height==1080 || u2Height==576)  // 576p, 720p, and 1080p
    	{
	    	Printf("[DGO][LVDS]%dHz input --> 50Hz output\n", u2FrameRate);
	        PANEL_SetHTotal50Hz(u2HTotal);
	        PANEL_SetVTotal50Hz(u2VTotal);
	        PANEL_SetPixelClk50Hz(u2PixelClk);  
	        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
	        PANEL_SetVSyncWidthByTiming(SV_DCLK_50HZ,u2VSyncWidth);                
	        PANEL_SetVSyncBpByTiming(SV_DCLK_50HZ, u2VSyncBP);
	        PANEL_SetHSyncWidthByTiming(SV_DCLK_50HZ,u2HsyncWdith);
        	PANEL_SetHSyncFpByTiming(SV_DCLK_50HZ,u2HsyncFP);
        	#endif
        }
    }
    else if(SUPPORT_PANEL_48HZ && (u2FrameRate == 24 || u2FrameRate == 48 || u2FrameRate == 96))
    {
    	Printf("[DGO][LVDS]%dHz input --> 48Hz output\n", u2FrameRate);
        PANEL_SetHTotal48Hz(u2HTotal);
        PANEL_SetVTotal48Hz(u2VTotal);
        PANEL_SetPixelClk48Hz(u2PixelClk);     
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        PANEL_SetVSyncWidthByTiming(SV_DCLK_48HZ,u2VSyncWidth);                
        PANEL_SetVSyncBpByTiming(SV_DCLK_48HZ, u2VSyncBP);
        PANEL_SetHSyncWidthByTiming(SV_DCLK_48HZ,u2HsyncWdith);
        PANEL_SetHSyncFpByTiming(SV_DCLK_48HZ,u2HsyncFP);
        #endif
    }
    else 
    {
    	Printf("[4k2k][LVDS]%dHz input --> 60Hz output\n", u2FrameRate);
        PANEL_SetHTotal60Hz(u2HTotal);
        PANEL_SetVTotal60Hz(u2VTotal);
        PANEL_SetPixelClk60Hz(u2PixelClk);        
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        PANEL_SetVSyncWidthByTiming(SV_DCLK_60HZ,u2VSyncWidth);                
        PANEL_SetVSyncBpByTiming(SV_DCLK_60HZ, u2VSyncBP);
        PANEL_SetHSyncWidthByTiming(SV_DCLK_60HZ,u2HsyncWdith);
        PANEL_SetHSyncFpByTiming(SV_DCLK_60HZ,u2HsyncFP);
        #endif
    }
}

EXTERN UINT32 u4ForceFrameTrackCtrl;
void SetDGOTiming(UINT8 u1Mode)
{
	UINT8 u1Cnt=0;
	UINT8 u1ValH=0;
	UINT8 u1ValL=0;
	UINT32 u4CtlWord = 0;
	UINT32 u4CtlWord2 = 0;
	static HANDLE_T hUnMuteTimer ;
	HAL_TIME_T hStartMeasureTime;
	HAL_TIME_T hEndMeasureTime;
	HAL_TIME_T  rDeltaTime;

	if(_u1DGOMode == u1Mode)
	{
		Printf("[DGO] skip Set Panel Mode %d\n", _u1DGOMode);
		return ;
	}
	VERIFY (x_sema_lock(_hDGOSetPanelSema, X_SEMA_OPTION_WAIT) == OSR_OK);	
	HAL_GetTime(&hStartMeasureTime);

	if(!hUnMuteTimer)
	{
		VERIFY(x_timer_create(&hUnMuteTimer) == OSR_OK);
	}	
	
	x_timer_stop(hUnMuteTimer);
	
	vDrvSetAllMute(SV_ON);

	{		
		S_PANEL_CONFIG *prPANEL=NULL;
		S_PANEL_CONFIG_COMMON *prPANEL_COMMON=NULL ;
		
	    vDrvLVDSOff();

		u4CtlWord = PANEL_GetControlWord() & 0xFFFFFFE7;	
	#ifdef EnablePANELCW2
		u4CtlWord2 = PANEL_GetControlWord2();
	#endif
	
	    switch(u1Mode)
	    {
			case E_DGO_DEFAULT:
			#ifdef SUPPORT_PANEL_48HZ
				u1ValL = SV_DCLK_48HZ;
			#else
				u1ValL = SV_DCLK_50HZ;
			#endif
				u1ValH = SV_DCLK_OTHERS;
				prPANEL = rPANEL_DEFAULT;
				prPANEL_COMMON = &rPANEL_DEFAULT_COMMON;
				u4CtlWord = _u4CtrlWord_DEFAULT;
			#ifdef EnablePANELCW2
				u4CtlWord2 = _u4CtrlWord2_DEFAULT;
			#endif
	    		break;
	    	case E_DGO_576P :
	    		u1ValL = SV_DCLK_50HZ;
	    		u1ValH = SV_DCLK_60HZ;
				prPANEL =rPANEL_576P ;
				prPANEL_COMMON = &rPANEL_576P_COMMON ;	
				u4CtlWord = u4CtlWord | SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF | LVDS_DISP_3D_SHUTTER;
				Printf("[DGO] 576P mode\n");
				break;
			case E_DGO_720P:
			#ifdef SUPPORT_PANEL_48HZ
				u1ValL = SV_DCLK_48HZ;
			#else
				u1ValL = SV_DCLK_50HZ;
			#endif
				u1ValH = SV_DCLK_OTHERS;
				prPANEL =rPANEL_720P ;
				prPANEL_COMMON = &rPANEL_720P_COMMON ;
				u4CtlWord = u4CtlWord | SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF | LVDS_DISP_3D_SHUTTER;
				Printf("[DGO] 720P mode\n");
				break;
			case E_DGO_1080P:
			#ifdef SUPPORT_PANEL_48HZ
				u1ValL = SV_DCLK_48HZ;
			#else
				u1ValL = SV_DCLK_50HZ;
			#endif
				u1ValH = SV_DCLK_OTHERS;
				prPANEL =rPANEL_1080P ;
				prPANEL_COMMON = &rPANEL_1080P_COMMON ;
				u4CtlWord = u4CtlWord | DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF | LVDS_DISP_3D_SHUTTER;
				Printf("[DGO] 1080P mode\n");
				break;
			case E_DGO_3D_1080P_24_FP:
			case E_DGO_3D_1080P_25_FP:
			case E_DGO_3D_1080P_30_FP:
				#ifdef SUPPORT_PANEL_48HZ
				u1ValL = SV_DCLK_48HZ;
			#else
				u1ValL = SV_DCLK_50HZ;
			#endif
				u1ValH = SV_DCLK_OTHERS;
				prPANEL =rPANEL_1080P_FP;
				prPANEL_COMMON = &rPANEL_1080P_FP_COMMON;
				u4CtlWord = u4CtlWord | DUAL_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF | LVDS_DISP_3D_SHUTTER;
				Printf("[DGO] 1080P FP mode\n");
				break;
			case E_DGO_480P :
			default:
				u1ValL = SV_DCLK_60HZ;
				u1ValH = SV_DCLK_OTHERS;
				prPANEL =rPANEL_480P ;
				prPANEL_COMMON = &rPANEL_480P_COMMON ;
				u4CtlWord = u4CtlWord | SINGLE_PORT | DISP_24BIT | LVDS_MSB_SW_ON | LVDS_ODD_SW_OFF | LVDS_DISP_3D_SHUTTER;
				Printf("[DGO] 480P mode\n");
				break;
	    }
	    
		for(u1Cnt=u1ValL;u1Cnt<u1ValH;u1Cnt++)
		{
			updatePanelAttribute_DGO(prPANEL[u1Cnt].u2HTotal,
                                prPANEL[u1Cnt].u2VTotal,
                                prPANEL[u1Cnt].u4PixelClk,
                                prPANEL[u1Cnt].u2Width,
                                prPANEL[u1Cnt].u2Height,
                                prPANEL[u1Cnt].u4VsyncWidth,
                                prPANEL[u1Cnt].u4VsyncBp, 
                                prPANEL[u1Cnt].u4HsyncWidth,
                                prPANEL[u1Cnt].u4HsyncFp,
                                prPANEL[u1Cnt].u2FrameRate);
		}
		PANEL_SetPixelClkMax(prPANEL_COMMON->u4PixleClockMax);		
        PANEL_SetPixelClkMin(prPANEL_COMMON->u4PixleClockMin);
        PANEL_SetHTotalMax(prPANEL_COMMON->u2HTotalMax);
        PANEL_SetVTotalMin(prPANEL_COMMON->u2HTotalMin);
        PANEL_SetVTotalMax(prPANEL_COMMON->u2VTotalMax);		
        PANEL_SetVTotalMin(prPANEL_COMMON->u2VTotalMin);		
        PANEL_SetVClkMax(prPANEL_COMMON->u2VClkMax);
        PANEL_SetVClkMin(prPANEL_COMMON->u2VClkMin);			    

	    PANEL_SetControlWord(u4CtlWord);
	#ifdef EnablePANELCW2
		PANEL_SetControlWord2(u4CtlWord2);
	#endif
		
	    vDrvLVDSConfig();
	    vDrvLVDSInit();
	    if (u1Mode==E_DGO_DEFAULT)
	    {
	    	vErrorHandleSetByPanel();
	    }

		#ifdef CC_MTK_LOADER
			vDrvVOPLLFreqSet(
					(vDrvGetLCDFreq()==25 || vDrvGetLCDFreq()==50 || vDrvGetLCDFreq()==100)?
						(PANEL_GetPixelClk50Hz()):
						#ifdef SUPPORT_PANEL_48HZ
						(vDrvGetLCDFreq()==24 || vDrvGetLCDFreq()==48)?
						(PANEL_GetPixelClk48Hz()):
						#endif
						(PANEL_GetPixelClk60Hz()));			
		#else
			vDrvVOFreqSet(
					(vDrvGetLCDFreq()==25 || vDrvGetLCDFreq()==50 || vDrvGetLCDFreq()==100)?
						(PANEL_GetPixelClk50Hz()):
						#ifdef SUPPORT_PANEL_48HZ
						(vDrvGetLCDFreq()==24 || vDrvGetLCDFreq()==48)?
						(PANEL_GetPixelClk48Hz()):
						#endif
						(PANEL_GetPixelClk60Hz()));
	    #endif

		if (u1Mode!=E_DGO_DEFAULT)
		{
			vDrvSetSyncLockMode(eSYNCLOCK_OFF);  // DDDS off
			//u4ForceFrameTrackCtrl|=SCPIP_FRAME_TRACK_OFF_CLI;
	        //u4ForceFrameTrackCtrl&=(~SCPIP_FRAME_TRACK_ON_CLI);
	        //u4ForceFrameTrackMode = 0;
	        
	        //vScpipSwitchFrameTrackOnOff(SV_OFF,SV_OFF);
	        //vDrvMJCSwitchFrameTrackOnOff(SV_OFF);
        }
        else
        {
			if (_eSyncLockMode == eSYNCLOCK_OFF)
			{
				vDrvSetSyncLockMode(eSYNCLOCK_ON);  // DDDS on
			}
        }

        vDrvSetPanelTiming();

		if (u1Mode!=E_DGO_DEFAULT)
		{
			vIO32WriteFldMulti(OS_DITHER_02, P_Fld(0,DRMOD_B)|P_Fld(0,DRMOD_G)|P_Fld(0,DRMOD_R));  // dither off
			vIO32WriteFldMulti(OS_DITHER_05, P_Fld(0,REG_DRMOD_B_POST)|P_Fld(0,REG_DRMOD_G_POST)|P_Fld(0,REG_DRMOD_R_POST));
		}
		else
		{
			vIO32WriteFldMulti(OS_DITHER_02, P_Fld(1,DRMOD_B)|P_Fld(1,DRMOD_G)|P_Fld(1,DRMOD_R));  // dither on
			vIO32WriteFldMulti(OS_DITHER_05, P_Fld(1,REG_DRMOD_B_POST)|P_Fld(1,REG_DRMOD_G_POST)|P_Fld(1,REG_DRMOD_R_POST));
		}
	    
	    IGNORE_RET(OSD_UpdateTiming());
		IGNORE_RET(OSD_Base_SetHStart());

		vDrvLVDSOn();
	}
	//update current mode 
	_u1DGOMode = u1Mode;

	vSetMainFlg(MAIN_FLG_MODE_CHG);
	vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
	
#if 1//def AUO_DEMO_SUPPORT
	VERIFY(x_timer_start(hUnMuteTimer, FRC_UNMUTE_DELAY, X_TIMER_FLAG_ONCE,_DGOUnMuteTimeout, NULL) == OSR_OK);	
#else
	vDrvSetAllMute(SV_OFF); 
#endif	
	HAL_GetTime(&hEndMeasureTime);
	HAL_GetDeltaTime(&rDeltaTime, &hStartMeasureTime, &hEndMeasureTime);
	Printf("[DGO] set panel diff %d.%03d s\n", rDeltaTime.u4Seconds,  (rDeltaTime.u4Micros / 1000));x_sema_unlock(_hDGOSetPanelSema);
}
#endif

#ifndef CC_MTK_LOADER
#if defined(CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)

#define DBG_CMI_4K2K_PANEL_ATTRIBUTE 0
void vPanelAttribute_Status_4k2k(void)
{
#if DBG_CMI_4K2K_PANEL_ATTRIBUTE
	UINT8 u1Cnt=0;

	#ifdef SUPPORT_PANEL_48HZ
	for(u1Cnt=SV_DCLK_48HZ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
	#else	
	for(u1Cnt=SV_DCLK_50HZ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
	#endif	
	{
		Printf("FHD120_u2FrameRate[%d] = %d\n",u1Cnt, rPANEL_FHD120[u1Cnt].u2FrameRate);		
		Printf("FHD120_u2Width[%d] = %d\n",u1Cnt, rPANEL_FHD120[u1Cnt].u2Width);
		Printf("FHD120_u2Height[%d] = %d\n", u1Cnt,rPANEL_FHD120[u1Cnt].u2Height ); 	   
		Printf("FHD120_u2HTotal[%d] = %d\n", u1Cnt,rPANEL_FHD120[u1Cnt].u2HTotal);
		Printf("FHD120_u2VTotal[%d] = %d\n", u1Cnt,rPANEL_FHD120[u1Cnt].u2VTotal);		
		Printf("FHD120_u4PixelClk[%d] = %d\n", u1Cnt,rPANEL_FHD120[u1Cnt].u4PixelClk);			
		Printf("FHD120_u4VsyncWidth[%d] = %d\n", u1Cnt, rPANEL_FHD120[u1Cnt].u4VsyncWidth);  
		Printf("FHD120_u4VsyncBp[%d] = %d\n", u1Cnt, rPANEL_FHD120[u1Cnt].u4VsyncBp);		   
		
		Printf("FHD60_u2FrameRate[%d] = %d\n",u1Cnt, rPANEL_FHD60[u1Cnt].u2FrameRate);		
		Printf("FHD60_u2Width[%d] = %d\n",u1Cnt, rPANEL_FHD60[u1Cnt].u2Width);
		Printf("FHD60_u2Height[%d] = %d\n", u1Cnt,rPANEL_FHD60[u1Cnt].u2Height ); 	   
		Printf("FHD60_u2HTotal[%d] = %d\n", u1Cnt,rPANEL_FHD60[u1Cnt].u2HTotal);
		Printf("FHD60_u2VTotal[%d] = %d\n", u1Cnt,rPANEL_FHD60[u1Cnt].u2VTotal);		
		Printf("FHD60_u4PixelClk[%d] = %d\n", u1Cnt,rPANEL_FHD60[u1Cnt].u4PixelClk);			
		Printf("FHD60_u4VsyncWidth[%d] = %d\n", u1Cnt, rPANEL_FHD60[u1Cnt].u4VsyncWidth);  
		Printf("FHD60_u4VsyncBp[%d] = %d\n", u1Cnt, rPANEL_FHD60[u1Cnt].u4VsyncBp);		

		Printf("4K2K30_u2FrameRate[%d] = %d\n",u1Cnt, rPANEL_4K2K30[u1Cnt].u2FrameRate);		
		Printf("4K2K30_u2Width[%d] = %d\n",u1Cnt, rPANEL_4K2K30[u1Cnt].u2Width);
		Printf("4K2K30_u2Height[%d] = %d\n", u1Cnt,rPANEL_4K2K30[u1Cnt].u2Height ); 	   
		Printf("4K2K30_u2HTotal[%d] = %d\n", u1Cnt,rPANEL_4K2K30[u1Cnt].u2HTotal);
		Printf("4K2K30_u2VTotal[%d] = %d\n", u1Cnt,rPANEL_4K2K30[u1Cnt].u2VTotal);		
		Printf("4K2K30_u4PixelClk[%d] = %d\n", u1Cnt,rPANEL_4K2K30[u1Cnt].u4PixelClk);			
		Printf("4K2K30_u4VsyncWidth[%d] = %d\n", u1Cnt, rPANEL_4K2K30[u1Cnt].u4VsyncWidth);  
		Printf("4K2K30_u4VsyncBp[%d] = %d\n", u1Cnt, rPANEL_4K2K30[u1Cnt].u4VsyncBp);
	}		 

	Printf("FHD120_COMMON.u4PixleClockMax = %d \n ", rPANEL_FHD120_COMMON.u4PixleClockMax);
	Printf("FHD120_COMMON.u4PixleClockMin = %d \n ", rPANEL_FHD120_COMMON.u4PixleClockMin);
	Printf("FHD120_COMMON.u2HTotalMax = %d \n ", rPANEL_FHD120_COMMON.u2HTotalMax);
	Printf("FHD120_COMMON.u2HTotalMin = %d \n ", rPANEL_FHD120_COMMON.u2HTotalMin);
	Printf("FHD120_COMMON.u2VTotalMax = %d \n ", rPANEL_FHD120_COMMON.u2VTotalMax);
	Printf("FHD120_COMMON.u2VTotalMin = %d \n ", rPANEL_FHD120_COMMON.u2VTotalMin);
	Printf("FHD120_COMMON.u2VClkMax = %d \n ", rPANEL_FHD120_COMMON.u2VClkMax);	
	Printf("FHD120_COMMON.u2VClkMin = %d \n ", rPANEL_FHD120_COMMON.u2VClkMin);

	Printf("FHD60_COMMON.u4PixleClockMax = %d \n ", rPANEL_FHD60_COMMON.u4PixleClockMax);
	Printf("FHD60_COMMON.u4PixleClockMin = %d \n ", rPANEL_FHD60_COMMON.u4PixleClockMin);
	Printf("FHD60_COMMON.u2HTotalMax = %d \n ", rPANEL_FHD60_COMMON.u2HTotalMax);
	Printf("FHD60_COMMON.u2HTotalMin = %d \n ", rPANEL_FHD60_COMMON.u2HTotalMin);
	Printf("FHD60_COMMON.u2VTotalMax = %d \n ", rPANEL_FHD60_COMMON.u2VTotalMax);
	Printf("FHD60_COMMON.u2VTotalMin = %d \n ", rPANEL_FHD60_COMMON.u2VTotalMin);
	Printf("FHD60_COMMON.u2VClkMax = %d \n ", rPANEL_FHD60_COMMON.u2VClkMax);	
	Printf("FHD60_COMMON.u2VClkMin = %d \n ", rPANEL_FHD60_COMMON.u2VClkMin);

	Printf("4K2K30_COMMON.u4PixleClockMax = %d \n ", rPANEL_4K2K30_COMMON.u4PixleClockMax);
	Printf("4K2K30_COMMON.u4PixleClockMin = %d \n ", rPANEL_4K2K30_COMMON.u4PixleClockMin);
	Printf("4K2K30_COMMON.u2HTotalMax = %d \n ", rPANEL_4K2K30_COMMON.u2HTotalMax);
	Printf("4K2K30_COMMON.u2HTotalMin = %d \n ", rPANEL_4K2K30_COMMON.u2HTotalMin);
	Printf("4K2K30_COMMON.u2VTotalMax = %d \n ", rPANEL_4K2K30_COMMON.u2VTotalMax);
	Printf("4K2K30_COMMON.u2VTotalMin = %d \n ", rPANEL_4K2K30_COMMON.u2VTotalMin);
	Printf("4K2K30_COMMON.u2VClkMax = %d \n ", rPANEL_4K2K30_COMMON.u2VClkMax);	
	Printf("4K2K30_COMMON.u2VClkMin = %d \n ", rPANEL_4K2K30_COMMON.u2VClkMin);
#endif
}

static void vSavePanelAttribute_4K2K(void)
{
	if (IS_SUPPORT_4K2K_DISPLAY && (!fg4k2kTblInit))
	{
	if (IS_DISP_4K2K_TYPE1)
	{
		_4k2kPanelType = PANEL_4K2K_FHD120;
		Printf("[SA8] vSavePanelAttribute_4K2K --> IS_DISP_4K2K_TYPE1\n");
		vIO32WriteFldAlign(DDDS_03, 0x10, DDDS_03_DDDS_ERR_LIM); // ERROR_LIMIT_FOR_PDP // plase remove to vDDDSInit		

	// CMO_4K2K_FHD120
		rPANEL_FHD120[SV_DCLK_60HZ].u2FrameRate = 120; 
		rPANEL_FHD120[SV_DCLK_60HZ].u2Width = PANEL_GetPanelWidth();
		rPANEL_FHD120[SV_DCLK_60HZ].u2Height = PANEL_GetPanelHeight();
		rPANEL_FHD120[SV_DCLK_60HZ].u2HTotal = PANEL_GetHTotal60Hz();
		rPANEL_FHD120[SV_DCLK_60HZ].u2VTotal = PANEL_GetVTotal60Hz();
		rPANEL_FHD120[SV_DCLK_60HZ].u4PixelClk = PANEL_GetPixelClk60Hz();
		rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_60HZ);
		rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_60HZ);

		rPANEL_FHD120[SV_DCLK_50HZ].u2FrameRate = 100;
		rPANEL_FHD120[SV_DCLK_50HZ].u2Width = PANEL_GetPanelWidth();
		rPANEL_FHD120[SV_DCLK_50HZ].u2Height = PANEL_GetPanelHeight();
		rPANEL_FHD120[SV_DCLK_50HZ].u2HTotal = PANEL_GetHTotal50Hz();
		rPANEL_FHD120[SV_DCLK_50HZ].u2VTotal = PANEL_GetVTotal50Hz();
		rPANEL_FHD120[SV_DCLK_50HZ].u4PixelClk = PANEL_GetPixelClk50Hz();		
		rPANEL_FHD120[SV_DCLK_50HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_50HZ);
		rPANEL_FHD120[SV_DCLK_50HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_50HZ);

        if (SUPPORT_PANEL_48HZ)
        {
	        rPANEL_FHD120[SV_DCLK_48HZ].u2FrameRate = 96;
	        rPANEL_FHD120[SV_DCLK_48HZ].u2Width = PANEL_GetPanelWidth();
	        rPANEL_FHD120[SV_DCLK_48HZ].u2Height = PANEL_GetPanelHeight();
	        rPANEL_FHD120[SV_DCLK_48HZ].u2HTotal = PANEL_GetHTotal48Hz();
	        rPANEL_FHD120[SV_DCLK_48HZ].u2VTotal = PANEL_GetVTotal48Hz();
	        rPANEL_FHD120[SV_DCLK_48HZ].u4PixelClk = PANEL_GetPixelClk48Hz();
	        rPANEL_FHD120[SV_DCLK_48HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_48HZ);
	        rPANEL_FHD120[SV_DCLK_48HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_48HZ);
        }

		rPANEL_FHD120_COMMON.u4PixleClockMax = PANEL_GetPixelClkMax();
		rPANEL_FHD120_COMMON.u4PixleClockMin = PANEL_GetPixelClkMin();
		rPANEL_FHD120_COMMON.u2HTotalMax = PANEL_GetHTotalMax();
		rPANEL_FHD120_COMMON.u2HTotalMin = PANEL_GetHTotalMin();
		rPANEL_FHD120_COMMON.u2VTotalMax = PANEL_GetVTotalMax();
		rPANEL_FHD120_COMMON.u2VTotalMin = PANEL_GetVTotalMin();
		rPANEL_FHD120_COMMON.u2VClkMax = PANEL_GetVClkMax();
		rPANEL_FHD120_COMMON.u2VClkMin= PANEL_GetVClkMin();

	// CMO_4K2K_FHD60
		rPANEL_FHD60[SV_DCLK_60HZ].u2FrameRate = 60;
		rPANEL_FHD60[SV_DCLK_60HZ].u2Width = rPANEL_FHD120[SV_DCLK_60HZ].u2Width;
		rPANEL_FHD60[SV_DCLK_60HZ].u2Height = rPANEL_FHD120[SV_DCLK_60HZ].u2Height;
		rPANEL_FHD60[SV_DCLK_60HZ].u2HTotal = rPANEL_FHD120[SV_DCLK_60HZ].u2HTotal;
		rPANEL_FHD60[SV_DCLK_60HZ].u2VTotal = rPANEL_FHD120[SV_DCLK_60HZ].u2VTotal;
		rPANEL_FHD60[SV_DCLK_60HZ].u4PixelClk = rPANEL_FHD120[SV_DCLK_60HZ].u4PixelClk >> 1;
		rPANEL_FHD60[SV_DCLK_60HZ].u4VsyncWidth = rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncWidth;
		rPANEL_FHD60[SV_DCLK_60HZ].u4VsyncBp = rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncBp;

		rPANEL_FHD60[SV_DCLK_50HZ].u2FrameRate = 50;
		rPANEL_FHD60[SV_DCLK_50HZ].u2Width = rPANEL_FHD120[SV_DCLK_50HZ].u2Width;
		rPANEL_FHD60[SV_DCLK_50HZ].u2Height = rPANEL_FHD120[SV_DCLK_50HZ].u2Height;
		rPANEL_FHD60[SV_DCLK_50HZ].u2HTotal = rPANEL_FHD120[SV_DCLK_50HZ].u2HTotal;
		rPANEL_FHD60[SV_DCLK_50HZ].u2VTotal = rPANEL_FHD120[SV_DCLK_50HZ].u2VTotal;
		rPANEL_FHD60[SV_DCLK_50HZ].u4PixelClk = rPANEL_FHD120[SV_DCLK_50HZ].u4PixelClk >> 1;		
		rPANEL_FHD60[SV_DCLK_50HZ].u4VsyncWidth = rPANEL_FHD120[SV_DCLK_50HZ].u4VsyncWidth;
		rPANEL_FHD60[SV_DCLK_50HZ].u4VsyncBp = rPANEL_FHD120[SV_DCLK_50HZ].u4VsyncBp;

        if (SUPPORT_PANEL_48HZ)
        {
	        rPANEL_FHD60[SV_DCLK_48HZ].u2FrameRate = 48;
	        rPANEL_FHD60[SV_DCLK_48HZ].u2Width = rPANEL_FHD120[SV_DCLK_48HZ].u2Width;
	        rPANEL_FHD60[SV_DCLK_48HZ].u2Height = rPANEL_FHD120[SV_DCLK_48HZ].u2Height;
	        rPANEL_FHD60[SV_DCLK_48HZ].u2HTotal = rPANEL_FHD120[SV_DCLK_48HZ].u2HTotal;
	        rPANEL_FHD60[SV_DCLK_48HZ].u2VTotal = rPANEL_FHD120[SV_DCLK_48HZ].u2VTotal;
	        rPANEL_FHD60[SV_DCLK_48HZ].u4PixelClk = rPANEL_FHD120[SV_DCLK_48HZ].u4PixelClk >> 1;
	        rPANEL_FHD60[SV_DCLK_48HZ].u4VsyncWidth = rPANEL_FHD120[SV_DCLK_48HZ].u4VsyncWidth;
	        rPANEL_FHD60[SV_DCLK_48HZ].u4VsyncBp = rPANEL_FHD120[SV_DCLK_48HZ].u4VsyncBp;
        }

		rPANEL_FHD60_COMMON.u4PixleClockMax = rPANEL_FHD120_COMMON.u4PixleClockMax >> 1;
		rPANEL_FHD60_COMMON.u4PixleClockMin = rPANEL_FHD120_COMMON.u4PixleClockMin >> 1;
		rPANEL_FHD60_COMMON.u2HTotalMax = rPANEL_FHD120_COMMON.u2HTotalMax;
		rPANEL_FHD60_COMMON.u2HTotalMin = rPANEL_FHD120_COMMON.u2HTotalMin;
		rPANEL_FHD60_COMMON.u2VTotalMax = rPANEL_FHD120_COMMON.u2VTotalMax;
		rPANEL_FHD60_COMMON.u2VTotalMin = rPANEL_FHD120_COMMON.u2VTotalMin;
		rPANEL_FHD60_COMMON.u2VClkMax = 60;
		rPANEL_FHD60_COMMON.u2VClkMin= 47;

	// CMO_4K2K30
		rPANEL_4K2K30[SV_DCLK_60HZ].u2FrameRate = 30;
		rPANEL_4K2K30[SV_DCLK_60HZ].u2Width = rPANEL_FHD120[SV_DCLK_60HZ].u2Width << 1;
		rPANEL_4K2K30[SV_DCLK_60HZ].u2Height = rPANEL_FHD120[SV_DCLK_60HZ].u2Height << 1;
		rPANEL_4K2K30[SV_DCLK_60HZ].u2HTotal = 4400;
		rPANEL_4K2K30[SV_DCLK_60HZ].u2VTotal = rPANEL_FHD120[SV_DCLK_60HZ].u2VTotal << 1;
		rPANEL_4K2K30[SV_DCLK_60HZ].u4PixelClk = rPANEL_FHD120[SV_DCLK_60HZ].u4PixelClk;
		rPANEL_4K2K30[SV_DCLK_60HZ].u4VsyncWidth = rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncWidth;
		rPANEL_4K2K30[SV_DCLK_60HZ].u4VsyncBp = rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncBp;

		rPANEL_4K2K30[SV_DCLK_50HZ].u2FrameRate = 25;
		rPANEL_4K2K30[SV_DCLK_50HZ].u2Width = rPANEL_FHD120[SV_DCLK_50HZ].u2Width << 1;
		rPANEL_4K2K30[SV_DCLK_50HZ].u2Height = rPANEL_FHD120[SV_DCLK_50HZ].u2Height << 1;
		rPANEL_4K2K30[SV_DCLK_50HZ].u2HTotal = 5280;
		rPANEL_4K2K30[SV_DCLK_50HZ].u2VTotal = rPANEL_FHD120[SV_DCLK_60HZ].u2VTotal << 1;
		rPANEL_4K2K30[SV_DCLK_50HZ].u4PixelClk = rPANEL_FHD120[SV_DCLK_50HZ].u4PixelClk;		
		rPANEL_4K2K30[SV_DCLK_50HZ].u4VsyncWidth = rPANEL_FHD120[SV_DCLK_50HZ].u4VsyncWidth;
		rPANEL_4K2K30[SV_DCLK_50HZ].u4VsyncBp = rPANEL_FHD120[SV_DCLK_50HZ].u4VsyncBp;

        if (SUPPORT_PANEL_48HZ)
        {
	        rPANEL_4K2K30[SV_DCLK_48HZ].u2FrameRate = 24;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u2Width = rPANEL_FHD120[SV_DCLK_60HZ].u2Width << 1;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u2Height = rPANEL_FHD120[SV_DCLK_60HZ].u2Height << 1;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u2HTotal = 5500;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u2VTotal = rPANEL_FHD120[SV_DCLK_60HZ].u2VTotal << 1;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u4PixelClk = rPANEL_FHD120[SV_DCLK_60HZ].u4PixelClk;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u4VsyncWidth = rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncWidth;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u4VsyncBp = rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncBp;
        }

		rPANEL_4K2K30_COMMON.u4PixleClockMax = rPANEL_FHD120_COMMON.u4PixleClockMax;
		rPANEL_4K2K30_COMMON.u4PixleClockMin = rPANEL_FHD120_COMMON.u4PixleClockMin;
		rPANEL_4K2K30_COMMON.u2HTotalMax = 5760;
		rPANEL_4K2K30_COMMON.u2HTotalMin = 3960;
		rPANEL_4K2K30_COMMON.u2VTotalMax = 2450;
		rPANEL_4K2K30_COMMON.u2VTotalMin = 2200;
		rPANEL_4K2K30_COMMON.u2VClkMax = 31;
		rPANEL_4K2K30_COMMON.u2VClkMin= 23;	
	}
	else if (IS_DISP_4K2K_TYPE2)
	{

		_4k2kPanelType = PANEL_4K2K_FHD60;
		Printf("[SA8] vSavePanelAttribute_4K2K --> IS_DISP_4K2K_TYPE2\n");		
	// AUO_4K2K_FHD60
		rPANEL_FHD60[SV_DCLK_60HZ].u2FrameRate = 60; 
		rPANEL_FHD60[SV_DCLK_60HZ].u2Width = PANEL_GetPanelWidth();
		rPANEL_FHD60[SV_DCLK_60HZ].u2Height = PANEL_GetPanelHeight();
		rPANEL_FHD60[SV_DCLK_60HZ].u2HTotal = PANEL_GetHTotal60Hz();
		rPANEL_FHD60[SV_DCLK_60HZ].u2VTotal = PANEL_GetVTotal60Hz();
		rPANEL_FHD60[SV_DCLK_60HZ].u4PixelClk = PANEL_GetPixelClk60Hz();
		rPANEL_FHD60[SV_DCLK_60HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_60HZ);
		rPANEL_FHD60[SV_DCLK_60HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_60HZ);

		rPANEL_FHD60[SV_DCLK_50HZ].u2FrameRate = 50;
		rPANEL_FHD60[SV_DCLK_50HZ].u2Width = PANEL_GetPanelWidth();
		rPANEL_FHD60[SV_DCLK_50HZ].u2Height = PANEL_GetPanelHeight();
		rPANEL_FHD60[SV_DCLK_50HZ].u2HTotal = PANEL_GetHTotal50Hz();
		rPANEL_FHD60[SV_DCLK_50HZ].u2VTotal = PANEL_GetVTotal50Hz();
		rPANEL_FHD60[SV_DCLK_50HZ].u4PixelClk = PANEL_GetPixelClk50Hz();		
		rPANEL_FHD60[SV_DCLK_50HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_50HZ);
		rPANEL_FHD60[SV_DCLK_50HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_50HZ);

        if (SUPPORT_PANEL_48HZ)
        {
	        rPANEL_FHD60[SV_DCLK_48HZ].u2FrameRate = 48;
	        rPANEL_FHD60[SV_DCLK_48HZ].u2Width = PANEL_GetPanelWidth();
	        rPANEL_FHD60[SV_DCLK_48HZ].u2Height = PANEL_GetPanelHeight();
	        rPANEL_FHD60[SV_DCLK_48HZ].u2HTotal = PANEL_GetHTotal48Hz();
	        rPANEL_FHD60[SV_DCLK_48HZ].u2VTotal = PANEL_GetVTotal48Hz();
	        rPANEL_FHD60[SV_DCLK_48HZ].u4PixelClk = PANEL_GetPixelClk48Hz();
	        rPANEL_FHD60[SV_DCLK_48HZ].u4VsyncWidth = PANEL_GetVSyncWidthByTiming(SV_DCLK_48HZ);
	        rPANEL_FHD60[SV_DCLK_48HZ].u4VsyncBp = PANEL_GetVSyncBpByTiming(SV_DCLK_48HZ);
        }

		rPANEL_FHD60_COMMON.u4PixleClockMax = PANEL_GetPixelClkMax();
		rPANEL_FHD60_COMMON.u4PixleClockMin = PANEL_GetPixelClkMin();
		rPANEL_FHD60_COMMON.u2HTotalMax = PANEL_GetHTotalMax();
		rPANEL_FHD60_COMMON.u2HTotalMin = PANEL_GetHTotalMin();
		rPANEL_FHD60_COMMON.u2VTotalMax = PANEL_GetVTotalMax();
		rPANEL_FHD60_COMMON.u2VTotalMin = PANEL_GetVTotalMin();
		rPANEL_FHD60_COMMON.u2VClkMax = PANEL_GetVClkMax();
		rPANEL_FHD60_COMMON.u2VClkMin= PANEL_GetVClkMin();

	// AUO_4K2K_FHD120
		rPANEL_FHD120[SV_DCLK_60HZ].u2FrameRate = 120;
		rPANEL_FHD120[SV_DCLK_60HZ].u2Width = rPANEL_FHD60[SV_DCLK_60HZ].u2Width;
		rPANEL_FHD120[SV_DCLK_60HZ].u2Height = rPANEL_FHD60[SV_DCLK_60HZ].u2Height;
		rPANEL_FHD120[SV_DCLK_60HZ].u2HTotal = rPANEL_FHD60[SV_DCLK_60HZ].u2HTotal;
		rPANEL_FHD120[SV_DCLK_60HZ].u2VTotal = rPANEL_FHD60[SV_DCLK_60HZ].u2VTotal;
		rPANEL_FHD120[SV_DCLK_60HZ].u4PixelClk = rPANEL_FHD60[SV_DCLK_60HZ].u4PixelClk << 1;
		rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncWidth = rPANEL_FHD60[SV_DCLK_60HZ].u4VsyncWidth;
		rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncBp = rPANEL_FHD60[SV_DCLK_60HZ].u4VsyncBp;

		rPANEL_FHD120[SV_DCLK_50HZ].u2FrameRate = 100;
		rPANEL_FHD120[SV_DCLK_50HZ].u2Width = rPANEL_FHD60[SV_DCLK_50HZ].u2Width;
		rPANEL_FHD120[SV_DCLK_50HZ].u2Height = rPANEL_FHD60[SV_DCLK_50HZ].u2Height;
		rPANEL_FHD120[SV_DCLK_50HZ].u2HTotal = rPANEL_FHD60[SV_DCLK_50HZ].u2HTotal;
		rPANEL_FHD120[SV_DCLK_50HZ].u2VTotal = rPANEL_FHD60[SV_DCLK_50HZ].u2VTotal;
		rPANEL_FHD120[SV_DCLK_50HZ].u4PixelClk = rPANEL_FHD60[SV_DCLK_50HZ].u4PixelClk << 1;		
		rPANEL_FHD120[SV_DCLK_50HZ].u4VsyncWidth = rPANEL_FHD60[SV_DCLK_50HZ].u4VsyncWidth;
		rPANEL_FHD120[SV_DCLK_50HZ].u4VsyncBp = rPANEL_FHD60[SV_DCLK_50HZ].u4VsyncBp;

        if (SUPPORT_PANEL_48HZ)
        {
	        rPANEL_FHD120[SV_DCLK_48HZ].u2FrameRate = 96;
	        rPANEL_FHD120[SV_DCLK_48HZ].u2Width = rPANEL_FHD60[SV_DCLK_48HZ].u2Width;
	        rPANEL_FHD120[SV_DCLK_48HZ].u2Height = rPANEL_FHD60[SV_DCLK_48HZ].u2Height;
	        rPANEL_FHD120[SV_DCLK_48HZ].u2HTotal = rPANEL_FHD60[SV_DCLK_48HZ].u2HTotal;
	        rPANEL_FHD120[SV_DCLK_48HZ].u2VTotal = rPANEL_FHD60[SV_DCLK_48HZ].u2VTotal;
	        rPANEL_FHD120[SV_DCLK_48HZ].u4PixelClk = rPANEL_FHD60[SV_DCLK_48HZ].u4PixelClk << 1;
	        rPANEL_FHD120[SV_DCLK_48HZ].u4VsyncWidth = rPANEL_FHD60[SV_DCLK_48HZ].u4VsyncWidth;
	        rPANEL_FHD120[SV_DCLK_48HZ].u4VsyncBp = rPANEL_FHD60[SV_DCLK_48HZ].u4VsyncBp;
        }

		rPANEL_FHD120_COMMON.u4PixleClockMax = rPANEL_FHD60_COMMON.u4PixleClockMax << 1;
		rPANEL_FHD120_COMMON.u4PixleClockMin = rPANEL_FHD60_COMMON.u4PixleClockMin << 1;
		rPANEL_FHD120_COMMON.u2HTotalMax = rPANEL_FHD60_COMMON.u2HTotalMax;
		rPANEL_FHD120_COMMON.u2HTotalMin = rPANEL_FHD60_COMMON.u2HTotalMin;
		rPANEL_FHD120_COMMON.u2VTotalMax = rPANEL_FHD60_COMMON.u2VTotalMax;
		rPANEL_FHD120_COMMON.u2VTotalMin = rPANEL_FHD60_COMMON.u2VTotalMin;
		rPANEL_FHD120_COMMON.u2VClkMax = 120;
		rPANEL_FHD120_COMMON.u2VClkMin= 60;

	// AUO_4K2K30
		rPANEL_4K2K30[SV_DCLK_60HZ].u2FrameRate = 30;
		rPANEL_4K2K30[SV_DCLK_60HZ].u2Width = rPANEL_FHD120[SV_DCLK_60HZ].u2Width << 1;
		rPANEL_4K2K30[SV_DCLK_60HZ].u2Height = rPANEL_FHD120[SV_DCLK_60HZ].u2Height << 1;
        rPANEL_4K2K30[SV_DCLK_60HZ].u2HTotal = 4400;
		rPANEL_4K2K30[SV_DCLK_60HZ].u2VTotal = rPANEL_FHD120[SV_DCLK_60HZ].u2VTotal << 1;
		rPANEL_4K2K30[SV_DCLK_60HZ].u4PixelClk = rPANEL_FHD120[SV_DCLK_60HZ].u4PixelClk;
		rPANEL_4K2K30[SV_DCLK_60HZ].u4VsyncWidth = rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncWidth;
		rPANEL_4K2K30[SV_DCLK_60HZ].u4VsyncBp = rPANEL_FHD120[SV_DCLK_60HZ].u4VsyncBp;

		rPANEL_4K2K30[SV_DCLK_50HZ].u2FrameRate = 25;
		rPANEL_4K2K30[SV_DCLK_50HZ].u2Width = rPANEL_FHD120[SV_DCLK_50HZ].u2Width << 1;
		rPANEL_4K2K30[SV_DCLK_50HZ].u2Height = rPANEL_FHD120[SV_DCLK_50HZ].u2Height << 1;
        rPANEL_4K2K30[SV_DCLK_50HZ].u2HTotal = 5280;
		rPANEL_4K2K30[SV_DCLK_50HZ].u2VTotal = rPANEL_FHD120[SV_DCLK_50HZ].u2VTotal << 1;
		rPANEL_4K2K30[SV_DCLK_50HZ].u4PixelClk = rPANEL_FHD120[SV_DCLK_50HZ].u4PixelClk;		
		rPANEL_4K2K30[SV_DCLK_50HZ].u4VsyncWidth = rPANEL_FHD120[SV_DCLK_50HZ].u4VsyncWidth;
		rPANEL_4K2K30[SV_DCLK_50HZ].u4VsyncBp = rPANEL_FHD120[SV_DCLK_50HZ].u4VsyncBp;

        if (SUPPORT_PANEL_48HZ)
        {
	        rPANEL_4K2K30[SV_DCLK_48HZ].u2FrameRate = 24;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u2Width = rPANEL_FHD120[SV_DCLK_48HZ].u2Width << 1;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u2Height = rPANEL_FHD120[SV_DCLK_48HZ].u2Height << 1;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u2HTotal = 5500;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u2VTotal = rPANEL_FHD120[SV_DCLK_48HZ].u2VTotal << 1;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u4PixelClk = rPANEL_FHD120[SV_DCLK_48HZ].u4PixelClk;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u4VsyncWidth = rPANEL_FHD120[SV_DCLK_48HZ].u4VsyncWidth;
	        rPANEL_4K2K30[SV_DCLK_48HZ].u4VsyncBp = rPANEL_FHD120[SV_DCLK_48HZ].u4VsyncBp;
        }

		rPANEL_4K2K30_COMMON.u4PixleClockMax = rPANEL_FHD120_COMMON.u4PixleClockMax;
		rPANEL_4K2K30_COMMON.u4PixleClockMin = rPANEL_FHD120_COMMON.u4PixleClockMin;
        rPANEL_4K2K30_COMMON.u2HTotalMax = 5760;
        rPANEL_4K2K30_COMMON.u2HTotalMin = 3960;
        rPANEL_4K2K30_COMMON.u2VTotalMax = 2450;
        rPANEL_4K2K30_COMMON.u2VTotalMin = 2200;
		rPANEL_4K2K30_COMMON.u2VClkMax = 31;
		rPANEL_4K2K30_COMMON.u2VClkMin= 23; 
	}
	
	vPanelAttribute_Status_4k2k();
	fg4k2kTblInit = TRUE;
	}
}    

static void updatePanelAttribute_4k2k(UINT16 u2HTotal,
                                 UINT16 u2VTotal,
                                 UINT32 u2PixelClk,                                 
                                 UINT16 u2Width,
                                 UINT16 u2Height,
                                 UINT16 u2VSyncWidth,                                                                  
                                 UINT16 u2VSyncFP,                                                                                                   
                                 UINT16 u2FrameRate)
{
    PANEL_SetPanelWidth(u2Width);
    PANEL_SetPanelHeight(u2Height);

    if(u2FrameRate == 25 || u2FrameRate == 50 || u2FrameRate == 100)
    {
    	Printf("[4k2k][LVDS]%dHz input --> 50Hz output\n", u2FrameRate);
        PANEL_SetHTotal50Hz(u2HTotal);
        PANEL_SetVTotal50Hz(u2VTotal);
        PANEL_SetPixelClk50Hz(u2PixelClk);  
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        PANEL_SetVSyncWidthByTiming(SV_DCLK_50HZ,u2VSyncWidth);                
        PANEL_SetVSyncBpByTiming(SV_DCLK_50HZ, u2VSyncFP);                
        #endif
    }
#ifdef SUPPORT_PANEL_48HZ
    else if(u2FrameRate == 24 || u2FrameRate == 48 || u2FrameRate == 96)
    {
    	Printf("[4k2k][LVDS]%dHz input --> 48Hz output\n", u2FrameRate);
        PANEL_SetHTotal48Hz(u2HTotal);
        PANEL_SetVTotal48Hz(u2VTotal);
        PANEL_SetPixelClk48Hz(u2PixelClk);     
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        PANEL_SetVSyncWidthByTiming(SV_DCLK_48HZ,u2VSyncWidth);                
        PANEL_SetVSyncBpByTiming(SV_DCLK_48HZ, u2VSyncFP);                        
        #endif
    }
#endif
    else 
    {
    	Printf("[4k2k][LVDS]%dHz input --> 60Hz output\n", u2FrameRate);
        PANEL_SetHTotal60Hz(u2HTotal);
        PANEL_SetVTotal60Hz(u2VTotal);
        PANEL_SetPixelClk60Hz(u2PixelClk);        
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        PANEL_SetVSyncWidthByTiming(SV_DCLK_60HZ,u2VSyncWidth);                
        PANEL_SetVSyncBpByTiming(SV_DCLK_60HZ, u2VSyncFP);                        
        #endif
    }
}

UINT32 GetPANEL4K2K_TYPE(void)
{
    return _4k2kPanelType;
}

void vDrv4K2KChk(UINT8 eCond, UINT8 bSignalMode)
{
	if((!IS_SUPPORT_4K2K_DISPLAY) || fgIsBypassFlgSet(BYPASS_NPTV_4K2K))
	{
		return ;
	}
	
	if(eCond == E_MLOOP)
	{
		if ((!fgIs4k2kNativeTimingOut(SV_VP_MAIN))&&(_u14K2KMode == E_4K2K_NATIVE))
		{
			Printf("[4k2k] mloop check back to normal\n");
			SetPanel_4k2k(E_4K2K_NORMAL);			
		}
	}
	else if(eCond == E_MODE_DONE)
	{
		Printf("[4k2k] mode done check %d\n", bSignalMode);
		
		if (bSignalMode ==SV_VDO_NOSIGNAL)
		{
			if (!fgIs4k2kNativeTimingOut(SV_VP_MAIN))
			{							
				SetPanel_4k2k(E_4K2K_NORMAL);							
			}
		}
		else  //STABLE case
		{
			if (fgIs4k2kNativeTimingOut(SV_VP_MAIN))  // 4K2K native mode
			{
				SetPanel_4k2k(E_4K2K_NATIVE);					   
			}					
			else if (u4DrvTDTV3DModeQuery()!=E_TDTV_UI_3D_MODE_OFF)
			{
				// set in tdtv callback
			}
			else // Normal mode(FHD120) 	
			{
                SetPanel_4k2k(E_4K2K_NORMAL);
			}	
		}
	}
}


//#define FRC_UNMUTE_DELAY 500 // 2 sec
 
static void vFrcUnmute(void *pvArgs)
{
	while(TRUE)
	{
		VERIFY (x_sema_lock(_hFrcUnmuteSema, X_SEMA_OPTION_WAIT) == OSR_OK);	
		vExt4k2kFrcMute(SV_OFF);
		Printf("\n[4k2k].....FRC Unmute\n");	
		vDrvSetAllMute(SV_OFF);
	}	
}

void Panel4K2KStatus(void)
{
	Printf("[4k2k] Panel4K2K Mode: %d\n", _u14K2KMode);
}

extern r4k2kHandler *psCur4k2kHandler;
void SetPanel_4k2k( UINT8 u1Mode)
{
	UINT32 u4LastPNLType;
	UINT8 u1Cnt=0;
	BOOL fg2530 = 0;
	UINT32 u4CtlWord = 0;
	UINT32 u4CtlWord2 = 0;
	UINT32 i4panelType = E_4K2K_NORMAL;
	UINT32 u4OSDMode=OSD_4K2K_MODE_NONE;
#ifndef NDEBUG	
	HAL_TIME_T hStartMeasureTime;
	HAL_TIME_T hEndMeasureTime;
	HAL_TIME_T  rDeltaTime;
#endif	


	if(_u14K2KMode == u1Mode)
	{
		Printf("[4k2k] skip Set Panel Mode %d\n", _u14K2KMode);
		return ;
	}
	VERIFY (x_sema_lock(_hFrcSetPanelSema, X_SEMA_OPTION_WAIT) == OSR_OK);	
#ifndef NDEBUG	
	HAL_GetTime(&hStartMeasureTime);
#endif	
	switch(u1Mode)
	{
		case E_4K2K_SPLIT_PHOTO :
			u4OSDMode = OSD_4K2K_MODE_JPEG; 
			i4panelType = PANEL_4K2K_FHD60 ;
			Printf("[4k2k] Photo mode\n");
		break;
		case E_4K2K_SPLIT_VDO :
			u4OSDMode = OSD_4K2K_MODE_MVC;
			//todo
			i4panelType = (IS_DISP_4K2K_TYPE1?PANEL_4K2K_FHD120:PANEL_4K2K_FHD60);		
			Printf("[4k2k] Video mode\n");
		break;
		case E_4K2K_NATIVE:
			u4OSDMode = OSD_4K2K_MODE_HDMI_4K2K;
			i4panelType = PANEL_4K2K_30 ;
			Printf("[4k2k] Native mode\n");
			break;

		case E_4K2K_NORMAL_3D:  // for + companion chip, => FHD_120
			u4OSDMode = OSD_4K2K_MODE_NONE;
			i4panelType = PANEL_4K2K_FHD120;
			Printf("[4k2k] 3D mode\n");
			break;
	
		case E_4K2K_NORMAL:
		default:		
			u4OSDMode = OSD_4K2K_MODE_NONE;
			i4panelType = (IS_DISP_4K2K_TYPE1?PANEL_4K2K_FHD120:PANEL_4K2K_FHD60);
			Printf("[4k2k] Normal mode\n");
		break;
	}


	u4LastPNLType = GetPANEL4K2K_TYPE();  
	Printf("---[4k2k][LVDS]last/cur panelIDType = %d %d\n", u4LastPNLType, i4panelType);
	
	//Wait for review
#if 0	
	if ((u4LastPNLType==PANEL_4K2K_30)&&((_bDviTiming==MODE_2160p_25)||(_bDviTiming==MODE_2160p_30))&&(u1Mode==E_4K2K_NATIVE))
	{
		fg2530=1;
		Printf("---[4k2k][LVDS] 4k2k_24 & 4k2k_25 need reset panel timing!\n");
	}
#endif	

	

		
	if ((u4LastPNLType != i4panelType)||(fg2530))
	{		
		S_PANEL_CONFIG *prPANEL=NULL;
		S_PANEL_CONFIG_COMMON *prPANEL_COMMON=NULL ;
        //vSend4k2kFrcPanelTypeCmd(i4panelType);

		vExt4k2kFrcMute(SV_ON);
		
		vDrvSetAllMute(SV_ON);
	    vDrvLVDSOff();

		if (IS_DISP_4K2K_TYPE2)
		{
			u4CtlWord = PANEL_GetControlWord() & 0xFFFFFFE7;	
		}
		u4CtlWord2 = PANEL_GetControlWord2() & 0xFFFFFDFF;	// clean linebuffer bit first

	    switch(i4panelType)
	    {
			case PANEL_4K2K_FHD120:
				prPANEL =rPANEL_FHD120 ;
				prPANEL_COMMON = &rPANEL_FHD120_COMMON ;					
				if (IS_DISP_4K2K_TYPE2)
				{
					u4CtlWord = u4CtlWord | FOUR_PORT;	
				}				
		        Printf("---[4k2k][LVDS]CMO_4K2K_FHD120 done!\n");
				break;

			case PANEL_4K2K_30:
				prPANEL =rPANEL_4K2K30 ;
				prPANEL_COMMON = &rPANEL_4K2K30_COMMON ;
				if (IS_DISP_4K2K_TYPE2)
				{
					u4CtlWord = u4CtlWord | FOUR_PORT;	
				}					
				u4CtlWord2 = u4CtlWord2 | DISABLE_LVDS_LINEBUFFER;
				
		        Printf("---[4k2k][LVDS]CMO_4K2K_30 done!\n");				
				
				break;
			case PANEL_4K2K_FHD60:
			default:
				prPANEL =rPANEL_FHD60 ;
				prPANEL_COMMON = &rPANEL_FHD60_COMMON ;				
				if (IS_DISP_4K2K_TYPE2)
				{
					u4CtlWord = u4CtlWord | DUAL_PORT;	
				}					
		        Printf("---[4k2k][LVDS]CMO_4K2K_FHD60 done!\n");
				break;				
	    }
	    
		#ifdef SUPPORT_PANEL_48HZ
		for(u1Cnt=SV_DCLK_48HZ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
		#else	
		for(u1Cnt=SV_DCLK_50HZ;u1Cnt<SV_DCLK_OTHERS;u1Cnt++)
		#endif	
		{
			updatePanelAttribute_4k2k(prPANEL[u1Cnt].u2HTotal,
                                prPANEL[u1Cnt].u2VTotal,
                                prPANEL[u1Cnt].u4PixelClk,
                                prPANEL[u1Cnt].u2Width,
                                prPANEL[u1Cnt].u2Height,
                                prPANEL[u1Cnt].u4VsyncWidth,
                                prPANEL[u1Cnt].u4VsyncBp,                                     
                                prPANEL[u1Cnt].u2FrameRate);
		}
		PANEL_SetPixelClkMax(prPANEL_COMMON->u4PixleClockMax);		
        PANEL_SetPixelClkMin(prPANEL_COMMON->u4PixleClockMin);
        PANEL_SetHTotalMax(prPANEL_COMMON->u2HTotalMax);
        PANEL_SetVTotalMin(prPANEL_COMMON->u2HTotalMin);
        PANEL_SetVTotalMax(prPANEL_COMMON->u2VTotalMax);		
        PANEL_SetVTotalMin(prPANEL_COMMON->u2VTotalMin);		
        PANEL_SetVClkMax(prPANEL_COMMON->u2VClkMax);
        PANEL_SetVClkMin(prPANEL_COMMON->u2VClkMin);			    

		if (IS_DISP_4K2K_TYPE2)
		{
	    	PANEL_SetControlWord(u4CtlWord);
		}	    	
		PANEL_SetControlWord2(u4CtlWord2);

    	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
	    {		
			vDrvVB14K2KPanelInterfaceConfig();	    
    	}
		else
		{
	    	vDrvLVDSConfig();
	    	vDrvLVDSInit();
		}

			
	    vErrorHandleSetByPanel();
#ifdef CC_MTK_LOADER
			vDrvVOPLLFreqSet(
					(vDrvGetLCDFreq()==25 || vDrvGetLCDFreq()==50 || vDrvGetLCDFreq()==100)?
						(PANEL_GetPixelClk50Hz()):
				#ifdef SUPPORT_PANEL_48HZ
						(vDrvGetLCDFreq()==24 || vDrvGetLCDFreq()==48)?
						(PANEL_GetPixelClk48Hz()):
				#endif
						(PANEL_GetPixelClk60Hz()));			
#else

	    vDrvVOFreqSet(
	        (vDrvGetLCDFreq()==25 || vDrvGetLCDFreq()==50 || vDrvGetLCDFreq()==100)?
	            (PANEL_GetPixelClk50Hz()):
#ifdef SUPPORT_PANEL_48HZ
	            (vDrvGetLCDFreq()==24 || vDrvGetLCDFreq()==48)?
	            (PANEL_GetPixelClk48Hz()):
#endif
	            (PANEL_GetPixelClk60Hz()));
#endif 

        vDrvSetPanelTiming();
	    
	    IGNORE_RET(OSD_UpdateTiming());
		IGNORE_RET(OSD_Base_SetHStart());

    	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
    	{
			vDrvVByOneOn();
    	}
		else
		{
			vDrvLVDSOn();
		}
		
#if 0   //wait for review	
		if(IS_DISP_4K2K_TYPE1)
		{
			vUtDelay1ms(_u4I2CDly);
		}		
#endif		
		_4k2kPanelType = i4panelType;
		Printf("---[4k2k][LVDS]panelType change to: %d\n", i4panelType);	

    
	//Switch OSD mode
	OSD_PLA_SetLinuxUIMode(u4OSDMode,0,0,0,0);
        vSend4k2kFrcModeCmd(u1Mode);
       x_thread_delay(psCur4k2kHandler->u2CmdInterval);
	vSend4k2kFrcPanelTypeCmd(i4panelType);  // move to here

	if(psCur4k2kHandler && (psCur4k2kHandler->frc_snd_mute==NULL))
	{
		Printf("[4k2k]****No FRC mute --> set all mute! (%d,%d)\n",psCur4k2kHandler->u2MuteTime,psCur4k2kHandler->u2CmdInterval);
		x_thread_delay(psCur4k2kHandler->u2MuteTime);
		vDrvSetAllMute(SV_OFF); 
	}
	
#ifndef NDEBUG
	Printf("[4k2k]unmute timer start %d\n", u2Ext4k2kGetMuteTime()); 
#endif		
	vExt4k2kFrcUmMuteTimer(u2Ext4k2kGetMuteTime());
}

	//update current 4k2k mode 
	_u14K2KMode = u1Mode;
#ifndef NDEBUG	
	HAL_GetTime(&hEndMeasureTime);
	HAL_GetDeltaTime(&rDeltaTime, &hStartMeasureTime, &hEndMeasureTime);
	Printf("[4K2K] set panel diff %d.%03d s\n", rDeltaTime.u4Seconds,  (rDeltaTime.u4Micros / 1000));
#endif	
	x_sema_unlock(_hFrcSetPanelSema);
}
#endif
#endif // CC_MTK_LOADER
LINT_RESTORE
