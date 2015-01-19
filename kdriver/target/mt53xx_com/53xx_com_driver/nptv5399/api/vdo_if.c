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
 * $RCSfile: vdo_if.c,v $
 * $Revision: #11 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdo_if.c
 *  Brief of file vdo_if.c.
 *  Details of file vdo_if.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "vdo_drvif.h"
#include "b2r_drvif.h"
#include "x_bim.h"
#include "x_assert.h"
#include "general.h"
#include "hw_vdoin.h"
#include "drv_video.h"
#include "video_def.h"
#include "nptv_drvif.h"
#include "source_table.h"
#include "source_select.h"
#include "api_backlt.h"
#include "api_eep.h"
#include "eepdef.h"
#include "util.h"
#include "drv_vga.h"
#include "drv_scaler.h"
#include "drv_adcset.h"
#include "drv_tvd.h"
#include "drvcust_if.h"
#include "drv_scart.h"
#include "drv_ycproc.h"
#include "drv_di.h"
#include "drv_dvi.h"
#include "drv_hdtv.h"
#include "drv_lvds.h"
#include "api_notify.h"
#include "nptv_debug.h"
#include "c_model.h"
#include "drv_vdoclk.h"
#include "drv_display.h"
#include "mute_if.h"
#include "drv_async.h"
//#if defined(CC_SUPPORT_TVE) ||defined(CC_SUPPORT_TVE_82xx)
#include "tve_if.h"
//#endif
#if SUPPORT_VBI
#include "drv_vbi.h"
#endif

#ifdef THE_3RD_PARTY_HEADER_FILE_1
#include THE_3RD_PARTY_HEADER_FILE_1
#endif

#ifdef CUSTOM_VIDEO_DRV_INCLUDE_FILE
#include CUSTOM_VIDEO_DRV_INCLUDE_FILE
#endif // #ifdef CUSTOM_VIDEO_DRV_INCLUDE_FILE

#include "vdp_drvif.h"
#include "srm_drvif.h"
#include "vdo_misc.h"
#ifdef CC_MT5399
#include "drv_vbyone.h"
#include "drv_mjc.h"
#endif


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
#if SUPPORT_SCART
//extern UINT8 u1ScartOutSwitch;
#endif

extern  HANDLE_T _hMainSubSrcSemaphore; /* from mloop_video_if.c */
extern UINT8 _bHDMIColorSpace;


#if 1 // defined(CC_FAST_INIT)
/* boot up measurement in release version, record once.*/
static BOOL b_boot_rec_once = FALSE;
#endif


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern void vApiVideoExternalGammaInit(void);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
PRIVATE void vDrvVideoPathShutDown(UINT8 path);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static SV_FIXCOLORSPACE_MODE _bFixColorSpace[2]= {SV_FIXCOLORSPACE_OFF,SV_FIXCOLORSPACE_OFF};
UINT8 _bOSDFixColorSpace[2]= {0,0}; //add by ciwu

UINT32 _u4ScanMode[2] = {0, 0};
static BOOL _fgSnowScreen= TRUE;

#if SUPPORT_VBI
static VBI_MODE_T _t_Vbi_Mode = VBI_CC_VCHIP_MODE;
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#ifndef CC_UP8032_ATV
#define MainSubSrc_Biglock_Release() \
	VERIFY(BIM_EnableIrq(VECTOR_VDOIN)); \
	VERIFY(AD_ReleaseControl()==OSR_OK); \
  	VERIFY (x_sema_unlock(_hMainSubSrcSemaphore) == OSR_OK);
#else
#define MainSubSrc_Biglock_Release() \
        vEnableSysInt2(VDOIN_INT_EN);
#endif



#ifdef CC_SUPPORT_STR
void DisableVsyncInt( void )
{
   BIM_DisableIrq(VECTOR_VDOIN);
}

void EnableVsyncInt( void )
{
    BIM_EnableIrq(VECTOR_VDOIN);
}
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

PRIVATE void vDrvVideoPathShutDown(UINT8 path)
{
    //[DTV00045974]
    //vModeChgMute(path, SV_OFF);
#ifdef CC_MT5399
    vDrvMJCSetOutMuteOnOff(SV_OFF);
#endif
    _vDrvVideoSetMute(MUTE_MODULE_MODECHG, path, FOREVER_MUTE, FALSE);
    /* Scaler */
    //    vDrvScpipWriteCtrl(path,SV_OFF);
    /* VBI */
#if SUPPORT_VBI
    (VBI_GetSlicerPath(VBI0) == path)?
    VBI_DisableBySlicer(VBI0):
    VBI_DisableBySlicer(VBI2);
#endif
    /* Disconnect the original function */
    vDrvVideoConnect(path, SV_OFF);
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void vApiVideoInit(void)
{
    LOG(0, "vApiVideoInit\n");
    vDrvADCOffsetCal();
    vApiVideoPeInit();
#ifndef CC_UP8032_ATV
    // Enable Video Interrupt
#ifndef CC_SCPOS_SW_ISR
    //UNUSED(BIM_EnableIrq(VECTOR_VDOIN));
#endif
#else
    // Enable Video Interrupt
    vEnableSysInt2(VDOIN_INT_EN);
    // Turn On Output VSync Interrupt
    vEnableSysInt2(POST_PROC_INT_EN);
#endif
    LboxDetectionInit();
#if SUPPORT_SCART
    //u1ScartOutSwitch = TRUE; //default to enable output through scart2
    //vDrvBypPD();
#endif
}

#ifdef CC_SOURCE_AUTO_DETECT
extern void vDrvDetectSourceInit(void);
#endif

#ifdef DRV_USE_EXTERNAL_3D_FRC // MT8283_FLIP_MIRROR
#ifdef CC_FLIP_MIRROR_SUPPORT
extern VOID DRVCUST_SetExtFrcFlipMirror(UINT32 u4FlipMirror);
extern UINT8 u1GetFlipMirrorConfig(void);
#endif
#endif

UINT8 _fgPostMuteInitStage = SV_FALSE;
extern void vDrvTCONInitISR(void);
void vApiHwInit(void)
{
#ifndef CC_UP8032_ATV
#if CC_BOOT_MUTE_LOG
    UINT8 bEnableLog;
#endif
#endif
#if SUPPORT_DISPLAY
    // overcome the garbage when backlight is on
   #ifdef CC_LOADER_LOGO_LONG_TIME
  #else
    vDrvSetAllMute(1);
  #endif
	_fgPostMuteInitStage = SV_TRUE;
    vDrvLVDSInit(); //for VOPLL CTS clock
    vDrvDisplayInit();

    switch(PANEL_GetDisplayType())
    {
        default:
        case PANEL_DISPLAY_TYPE_MAX:
            // error display type, defaultly do nothing
            break;

        case PANEL_DISPLAY_TYPE_LVDS:
            //        vDrvLVDSInit();
            break;

        case PANEL_DISPLAY_TYPE_MLVDS:
            break;
#ifdef CC_MT5399

        case PANEL_DISPLAY_TYPE_VB1:
            vVByOneInit();
            break;

        case PANEL_DISPLAY_TYPE_EPI:
            break;
#endif
    }
	vDrvTCONInitISR();
    LOG(0, "Display Init");
#endif
#if SUPPORT_VIDEO
    // video init
    vDrvVideoSwInit();
    LOG(0, "Video SW Init");
    vDrvVideoHwInit();
    LOG(0, "Video HW Init");
#endif
    // TVD init
    vTvd3dInit();
    // VBI Init
#if SUPPORT_VBI
#ifdef NEW_COUNTRY_TYPE

    if(GetTargetCountryMask(COUNTRY_VBI_MASK) == COUNTRY_VBI_CC)
    {
        _t_Vbi_Mode = VBI_CC_VCHIP_MODE;
    }
    else
    {
        #if FORCE_ENABLE_ANAS
        _t_Vbi_Mode = VBI_ANAS_MODE;
        #else
        _t_Vbi_Mode = VBI_TTX_MODE;
        #endif
    }

#else
#ifdef COUNTRY_FROM_EEPROM
    UINT8 u1TargetCountryInfo;
    u1TargetCountryInfo = GetTargetCountry() & 0xFF;

    if(u1TargetCountryInfo & COUNTRY_CC_ENABLE_MASK)   //Bit 7 CC Enable/Disable
    {
        _t_Vbi_Mode = VBI_CC_VCHIP_MODE;
    }
    else
    {
        #if FORCE_ENABLE_ANAS
        _t_Vbi_Mode = VBI_ANAS_MODE;
        #else
        _t_Vbi_Mode = VBI_TTX_MODE;
        #endif
    }

#else

    if(GetTargetCountry() == COUNTRY_US)
    {
        _t_Vbi_Mode = VBI_CC_VCHIP_MODE;
    }
    else if(GetTargetCountry() == COUNTRY_EU)
    {
        #if FORCE_ENABLE_ANAS
        _t_Vbi_Mode = VBI_ANAS_MODE;
        #else
        _t_Vbi_Mode = VBI_TTX_MODE;
        #endif
    }

#endif
#endif
    VBI_Init(_t_Vbi_Mode);
    VBI_Enable();
    //	#if DMP_HWINIT_DBGMSG
    LOG(0, "VBI_Init");
    //	#endif
#endif
    // HDMI Init
    vHDMIInit();
//#ifdef CC_SUPPORT_TVE_82xx
    /* TVE Bypass Init */
    Bypass_Init();
//#endif
    // Enable Interrupt
    // vKrEnableInt();
    //#if DMP_HWINIT_DBGMSG
    //	LogS("Enable Int");
    //#endif
#ifndef CC_UP8032_ATV
#ifndef CC_DISABLE_EEP_MAG_CHK

    if(!fgApiIsEepromMagicOK())
    {
        //reset EEPROM
        vApiEepromDefaultInit();
    }

#endif //#ifndef CC_DISABLE_EEP_MAG_CHK
#if CC_BOOT_MUTE_LOG
    bEnableLog = bApiEepromReadByte(EEP_BOOT_MUTE_LOG);

    if(bEnableLog == 0xaa)
    {
        INT32 i4Log;
        Printf("Mute log is ENABLE when system boot\n");
        i4Log = MUTE_i4LogLvlQuerySet(NULL);
        i4Log = (INT32)((UINT32)i4Log &0xffffff00);
        i4Log = (INT32)((UINT32)i4Log |0x3);
        VERIFY(i4Log == MUTE_i4LogLvlQuerySet(&i4Log));
    }

#endif
#endif
    // source detect initial
#ifdef CC_SOURCE_AUTO_DETECT
    vDrvDetectSourceInit();
#endif

	if (_fgPostMuteInitStage)
    {
	 // overcome the garbage when backlight is on
	 vDrvSetAllMute(0);   
	_fgPostMuteInitStage = SV_FALSE;
	}
	
#ifdef DRV_USE_EXTERNAL_3D_FRC // MT8283_FLIP_MIRROR
#ifdef CC_FLIP_MIRROR_SUPPORT
	DRVCUST_SetExtFrcFlipMirror(u1GetFlipMirrorConfig());
#endif
#endif

}

UINT8 bApiMonitorSetup(UINT8 u1Path,UINT8 u1MonSrc)
{
		//RVChannel  *chnl=u1Path?(&_rPChannel):(&_rMChannel);
		UINT8 *bMonOldSrc=u1Path?&_bMonSubOld:&_bMonMainOld;
		UINT8 *bMonSrc=u1Path?&_bMonSubNew:&_bMonMainNew;
		UINT8 *bMonNewICIn=u1Path?&_bSubMonICIn:&_bMainMonICIn;

		if(u1MonSrc!= (*bMonOldSrc))
		{			
			// record information about monitored component
			//chnl->t_mon_id = t_main_mon_comp;			
			*bMonNewICIn = u1GetMonICIn(u1MonSrc);			
			*bMonOldSrc = *bMonSrc;
			*bMonSrc=u1MonSrc;
		}
		return SV_SUCCESS;
}

/**
 * Enable/Disable Sub Clone mode
 * Make sub's input source always become the same with Main's
 * Only for old VENC MSI   Should avoid useing it
 * @param fgOnOff  SV_ON/SV_OFF
 */
static BOOL fgSubSrcClone = FALSE;
static UINT8 u1SubSrcBeforeClone = SV_VS_MAX;
void bSubSrcCloneMode(UINT8 fgOnOff)
{
    if (fgSubSrcClone != fgOnOff)
    {
        fgSubSrcClone = fgOnOff;
        if (fgOnOff)
        {
            u1SubSrcBeforeClone = _bSrcSubOld;          
            bApiVideoSetVideoSrc(SV_VP_PIP, _bSrcMainOld);
        }
        else
        {
            bApiVideoSetVideoSrc(SV_VP_PIP, u1SubSrcBeforeClone);          
        }
    }    
}

#ifdef CC_SUPPORT_PIPELINE
UINT8 u1VSCConnectStatus[2]={SV_VD_NA,SV_VD_NA};

UINT8 bApiQuearyVSCConnectStatus(UINT8 bPath)
{
	return u1VSCConnectStatus[bPath];
}

//only for AVD connet and disconnect
UINT8 bApiVFEAVDISConnect(UINT8 bSrc, UINT8 u4Port, UINT8 bEnable,UINT8 bType)
{
  UINT8 bAVDConnect;
  bAVDConnect=bType;
  LOG(0, "11111bApiVFEAVDConnect(bSrc=%d, u4Port=%d,bEnable=%d,bType=%d)\n",bSrc,u4Port,bEnable,bType);
  switch(bSrc)
	{ 
	case SV_VS_ATD1:
	case SV_VS_CVBS4:
	case SV_VS_SCART1:
		bSrc=SV_VS_CVBS4;
		bApiVFEAVDConnect(bAVDConnect,bSrc,SV_VS_NO_CHANGE);
		break;
	case SV_VS_MAX:
		break;
	default:
		break;
	}
   return SV_SUCCESS;

}

UINT8 bApiVFEConnectVideoSrc(UINT8 bSrc, UINT8 u4Port, UINT8 bEnable, UINT8 bType)
{
/*
	 switch(bSrc)
	{ 
		case SV_VD_TVD3D:
			break;

		case SV_VD_YPBPR:
			break;

		case SV_VD_VGA:
			break;
		case SV_VS_YPbPr1:
			vDrvAllHDADCPow(TRUE);
			vDrvSOY0EN(1);
			//vDrvSetInternalMux(0,_bSrcMainNew);// replaced by following code
			{
#if SUPPORT_SCART
        	vSCARTDisable();
#endif
        	initYPbPrVGA(P_YP0);
    		}


			//vSetMOutMux(bNewMainDec);//remove omux connent
			//vDrvVideoConnect(SV_VP_MAIN, SV_ON);//replaced by following code
			vHdtvConnect(0, 1);
			u1ADCConnentSrc=SV_VS_YPbPr1;
			u1ADCConnentSrcType=VSS_YPBPR;
			u1ADCConnentSrcPort=P_YP0;
			
			
			break;
#if SUPPORT_DVI

		case SV_VD_DVI:
			if(1)
			{
			
			}
			else
			{
#ifdef CC_OSD_ENCODE

#else

#endif
			}
			break;
#endif

#ifndef COPLAT_EXCLUDE_DTV
		case SV_VD_MPEGHD:
			break;
			
		case SV_VD_MPEGSD:
			break;
#endif
		default:
			break;
	}
	*/
	return SV_SUCCESS;

	

}


UINT8 bApiVSCConnectVideoSrc(UINT8 bPath, UINT8 bSrc, UINT8 bEnable, UINT8 u4Type)
{
	UINT8 bStatus;
	
	LOG(2, "Pipeline bApiVSCConnectVideoSrc(%d, %d, %d, %d)\n", bPath, bSrc,bEnable,u4Type);
	
	if(u4Type == 1)//connect source
	{
		if(bPath == SV_VP_MAIN)	//check the real source
	    {
	        bStatus = bApiVSCMainSubSrc(bSrc, SV_VS_NO_CHANGE, bEnable);

	    }
	    else
	    {
	        bStatus = bApiVSCMainSubSrc(SV_VS_NO_CHANGE, bSrc, bEnable);
	    }
		
	}
	else//disconnect source
	{
		if(bPath == SV_VP_MAIN)	//check the real source
	    {
	        bStatus = bApiVSCMainSubSrc(SV_VS_MAX, SV_VS_NO_CHANGE, bEnable);

	    }
	    else
	    {
	        bStatus = bApiVSCMainSubSrc(SV_VS_NO_CHANGE, SV_VS_MAX, bEnable);
	    }	
	}
	return SV_SUCCESS;

}

UINT8 bApiVSCMainSubSrc(UINT8 bMainSrc, UINT8 bSubSrc, UINT8 bEnable)
{
	static UINT8 bOldMainDec = 0xff;
	static UINT8 bOldSubDec = 0xff;
	UINT8 bNewMainDec, bNewSubDec;
	ExtInputTable NewExtInput;
	BOOL fgMainCh = FALSE;
	BOOL fgPipCh = FALSE;
	BOOL fgMainCombi;
	BOOL fgSubCombi;

#ifndef CC_UP8032_ATV
	VERIFY(x_sema_lock(_hMainSubSrcSemaphore, X_SEMA_OPTION_WAIT) == OSR_OK);
	VERIFY(AD_AcquireControl()==OSR_OK);
	VERIFY(BIM_DisableIrq(VECTOR_VDOIN));
#else
	vDisableSysInt2(VDOIN_INT_EN);
#endif

	if(bMainSrc ==SV_VS_NO_CHANGE)
	{
		bMainSrc = _bSrcMainOld;
	}

	if(bSubSrc ==SV_VS_NO_CHANGE)
	{
		bSubSrc = _bSrcSubOld;
	}

	NewExtInput.MapIntMode = bDrvGetMapIntMode(bMainSrc, bSubSrc);
	/* is changed ? */
	{
		if(NewExtInput.MapIntMode == 0xFFFF)  /* check PIP constrain */
		{
			MainSubSrc_Biglock_Release();
			return (SV_FAIL);
		}

		//fgMainCombi=fgApiMonitorSetup(SV_VP_MAIN,bMainSrc);
		fgMainCombi = (_bMonMainNew == SV_VS_MAX) ? FALSE: TRUE ;
		if(_bMonMainNew == bMainSrc)
		{
			fgMainCombi = FALSE;
		}
		//fgSubCombi=fgApiMonitorSetup(SV_VP_PIP,bSubSrc);
		fgSubCombi = (_bMonMainNew == SV_VS_MAX) ? FALSE: TRUE ;
		if(_bMonSubNew == bSubSrc)
		{
			fgSubCombi = FALSE;
		}

		if(!fgMainCombi)
		{
#ifndef CC_DRIVER_PROGRAM
			_rMChannel.t_mon_id.e_type = DRVT_UNKNOWN;
#endif
			_bMonMainOld = SV_VS_MAX;
			_bMonMainNew = SV_VS_MAX;
			_bMainMonICIn = (UINT8)P_FA;
		}

		if(!fgSubCombi)
		{
#ifndef CC_DRIVER_PROGRAM
			_rPChannel.t_mon_id.e_type = DRVT_UNKNOWN;
#endif
			_bMonSubOld = SV_VS_MAX;
			_bMonSubNew = SV_VS_MAX;
			_bSubMonICIn = (UINT8)P_FA;
		}

		/* Combi related behavior here */

		if(_bSrcMainOld != bMainSrc)
		{
			fgMainCh = TRUE;
			vApiVideoSetFixColorSpaceMode(SV_VP_MAIN,SV_FIXCOLORSPACE_OFF);
			vDrvSwitchMTKGoodDclk(SV_OFF);
		}

		if(_bSrcSubOld != bSubSrc)
		{
			fgPipCh = TRUE;
			vApiVideoSetFixColorSpaceMode(SV_VP_PIP,SV_FIXCOLORSPACE_OFF);
		}

		_bSrcMainNew = bMainSrc;
		_bSrcMainOld = bMainSrc;
		_bSrcSubNew = bSubSrc;
		_bSrcSubOld = bSubSrc;
		_bMainICIn = NewExtInput.MapIntMode >> 8;
		_bSubICIn = NewExtInput.MapIntMode & 0xff;
		bNewMainDec = bGetInternalDec(SV_VP_MAIN);
		bNewSubDec = bGetInternalDec(SV_VP_PIP);

		u1VSCConnectStatus[SV_VP_MAIN] = bNewMainDec;
		u1VSCConnectStatus[SV_VP_PIP] = bNewSubDec;

		if(bNewMainDec != bOldMainDec)
		{
			fgMainCh = TRUE;
		}

		if(bNewSubDec != bOldSubDec)
		{
			fgPipCh = TRUE;
		}

		bOldMainDec = bNewMainDec;
		bOldSubDec = bNewSubDec;
	   
		if((!fgMainCh) && (!fgPipCh))
		{
			MainSubSrc_Biglock_Release();
			return SV_NO_CHANGE;
		}
	}

	if(((fgMainCh== TRUE) && (bGetSignalType(SV_VP_MAIN) != SV_ST_TV) && (bGetSignalType(SV_VP_MAIN) != SV_ST_MPEG) && (bGetSignalType(SV_VP_MAIN) != SV_ST_MAX))||
	   ((fgPipCh== TRUE)&& (bGetSignalType(SV_VP_PIP) != SV_ST_TV)&& (bGetSignalType(SV_VP_PIP) != SV_ST_MPEG) && (bGetSignalType(SV_VP_PIP) != SV_ST_MAX)))
	{
		_fgAutoSearch = FALSE;
	}

	if(bMainSrc == SV_VS_MAX)
	{
		_rMChannel.bIsChannelOn = SV_OFF;
		_rMChannel.bDecType = SV_VD_NA;
		//		  vDrvScpipWriteCtrl(SV_VP_MAIN,SV_OFF);
	}
	else
	{
		_rMChannel.bIsChannelOn = SV_ON;
	#ifdef CC_DEMOD_FASTACQ
		fgApiEepromWriteByte(EEP_DEMOD_FASTBOOT_LASTSRC,bMainSrc);		
	#endif
#if 1 // defined(CC_FAST_INIT)

		//if(!b_boot_rec_once)
		{
		   
#ifdef TIME_MEASUREMENT
		   TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME_DRV, "bApiVideoMainSubSrc");
		   TMS_DIFF_EX(TMS_FLAG_BOOT, TMS_COOL_BOOT_TIME, "bApiVideoMainSubSrc");
#endif
			x_os_drv_set_timestamp("bApiVideoMainSubSrc");
			b_boot_rec_once = TRUE;
		}
#endif
	}

	if(bSubSrc == SV_VS_MAX)
	{
		_rPChannel.bIsChannelOn = SV_OFF;
		_rPChannel.bDecType = SV_VD_NA;
		//vDrvScpipWriteCtrl(SV_VP_PIP,SV_OFF);
	}
	else
	{
		_rPChannel.bIsChannelOn = SV_ON;
	}

	/* disconnect unused internal mux */
	vMuxCleanup();
	vDrvSetExternalMux(_bSrcMainNew, _bSrcSubNew);

	if(fgMainCh)
	{
		if((_bSrcMainNew != _bSrcSubNew) || (fgPipCh))
		{
			vDrvSetInternalMux(0,_bSrcMainNew);
		}
		vSetMOutMux(bNewMainDec);
	}

#ifdef SUPPORT_AV_COMP_AUTO

	if(fgMainCombi)
	{
		vDrvSetInternalMux(0,_bMonMainNew);
	}

#endif

	if(fgPipCh)
	{
		if((_bSrcMainNew != _bSrcSubNew) || (fgMainCh))
		{
			vDrvSetInternalMux(1,_bSrcSubNew);
		}
		vSetSOutMux(bNewSubDec);
	}

	// MDisableMainINT();

	if(fgMainCh)
	{
		_rMChannel.bDecType = bNewMainDec;

		_bMainState = VDO_STATE_IDLE; /* mode change state machine */

		vSetMainFlg(MAIN_FLG_MODE_CHG);
		vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
	}

	if(fgPipCh)
	{
		_rPChannel.bDecType = bNewSubDec;

		_bPipState = VDO_STATE_IDLE; /* mode change state machine */
		
		vSetPipFlg(PIP_FLG_MODE_CHG);
		vSetPipFlg(PIP_FLG_MODE_DET_DONE);
	}

#ifdef CC_SRM_ON
	SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SOURCE + (UINT32)VDP_1), (UINT32)bMainSrc, 0);
	SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SOURCE + (UINT32)VDP_2), (UINT32)bSubSrc, 0);
#endif
	MainSubSrc_Biglock_Release();
	return SV_SUCCESS;
}
#endif

/**
 * Set video source of the specified channel.
 *
 * @param bPath SV_VP_MAIN or SV_VP_PIP
 * @param bSrc Select the video source. See enum eVideoSrc.
 *
 * @retval SV_TRUE
 * @retval SV_FALSE
 */
#if SUPPORT_SBS_CONVERT
UINT8 _bCurrentInputMS = SV_VS_ATV1;
#endif
UINT8 bApiVideoSetVideoSrc(UINT8 bPath, UINT8 bSrc)
{
    UINT8 bStatus;
	
    #ifdef CC_SUPPORT_PIPELINE
	return 1;  //for new pipeline test
	#else
	//to do
	#endif
    LOG(0, "00000ApiVideoSetVideoSrc(%d, %d)\n", (UINT32)bPath, (UINT32)bSrc);

    if(bPath == SV_VP_MAIN)	//check the real source
    {
        bStatus = bApiVideoMainSubSrc(bSrc, SV_VS_NO_CHANGE);
#if SUPPORT_SBS_CONVERT
        _bCurrentInputMS = bSrc;
#endif
    }
    else
    {
        bStatus = bApiVideoMainSubSrc(SV_VS_NO_CHANGE, bSrc);
    }

#ifdef CC_SUPPORT_TVE_82xx
    vApiTVECtrl(&bSrc);
#endif
    return bStatus;
}

#if 0 // defined(CC_FAST_INIT)
/* boot up measurement in release version, record once.*/
static BOOL b_boot_rec_once = FALSE;
#endif
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif


UINT8 bApiVideoMainSubSrc(UINT8 bMainSrc, UINT8 bSubSrc)
{
    static UINT8 bOldMainDec = 0xff;
    static UINT8 bOldSubDec = 0xff;
    UINT8 bNewMainDec, bNewSubDec;
    ExtInputTable NewExtInput;
    BOOL fgMainCh = FALSE;
    BOOL fgPipCh = FALSE;
    BOOL fgMainCombi;
    BOOL fgSubCombi;

#ifndef CC_UP8032_ATV
    VERIFY(x_sema_lock(_hMainSubSrcSemaphore, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(AD_AcquireControl()==OSR_OK);
    VERIFY(BIM_DisableIrq(VECTOR_VDOIN));
#else
    vDisableSysInt2(VDOIN_INT_EN);
#endif
#if 0 // 5387
#if DISP_MODE_USE == DISP_MODE_ON
    // Disable Display Mode of the SCPOS. SJ Added 050615
    vSwitchDispMode(SV_OFF);
#endif
#endif

    if(bMainSrc ==SV_VS_NO_CHANGE)
    {
        bMainSrc = _bSrcMainOld;
    }

    if(bSubSrc ==SV_VS_NO_CHANGE)
    {
        bSubSrc = _bSrcSubOld;
    }

    // hack Sub clone mode   temp solution for MSI
    if (fgSubSrcClone)
    {
        bSubSrc = bMainSrc;
    }

    NewExtInput.MapIntMode = bDrvGetMapIntMode(bMainSrc, bSubSrc);
    /* is changed ? */
    {
        if(NewExtInput.MapIntMode == 0xFFFF)  /* check PIP constrain */
        {
            MainSubSrc_Biglock_Release();
            return (SV_FAIL);
        }

        //fgMainCombi=fgApiMonitorSetup(SV_VP_MAIN,bMainSrc);
		fgMainCombi = (_bMonMainNew == SV_VS_MAX) ? FALSE: TRUE ;
        if(_bMonMainNew == bMainSrc)
        {
            fgMainCombi = FALSE;
        }
        //fgSubCombi=fgApiMonitorSetup(SV_VP_PIP,bSubSrc);
		fgSubCombi = (_bMonMainNew == SV_VS_MAX) ? FALSE: TRUE ;
        if(_bMonSubNew == bSubSrc)
        {
            fgSubCombi = FALSE;
        }

        if(!fgMainCombi)
        {
#ifndef CC_DRIVER_PROGRAM
            _rMChannel.t_mon_id.e_type = DRVT_UNKNOWN;
#endif
            _bMonMainOld = SV_VS_MAX;
            _bMonMainNew = SV_VS_MAX;
            _bMainMonICIn = (UINT8)P_FA;
        }

        if(!fgSubCombi)
        {
#ifndef CC_DRIVER_PROGRAM
            _rPChannel.t_mon_id.e_type = DRVT_UNKNOWN;
#endif
            _bMonSubOld = SV_VS_MAX;
            _bMonSubNew = SV_VS_MAX;
            _bSubMonICIn = (UINT8)P_FA;
        }

        /* Combi related behavior here */

        if(_bSrcMainOld != bMainSrc)
        {
            fgMainCh = TRUE;
            vApiVideoSetFixColorSpaceMode(SV_VP_MAIN,SV_FIXCOLORSPACE_OFF);
            vDrvSwitchMTKGoodDclk(SV_OFF);
        }

        if(_bSrcSubOld != bSubSrc)
        {
            fgPipCh = TRUE;
            vApiVideoSetFixColorSpaceMode(SV_VP_PIP,SV_FIXCOLORSPACE_OFF);
        }

        _bSrcMainNew = bMainSrc;
        _bSrcMainOld = bMainSrc;
        _bSrcSubNew = bSubSrc;
        _bSrcSubOld = bSubSrc;
        _bMainICIn = NewExtInput.MapIntMode >> 8;
        _bSubICIn = NewExtInput.MapIntMode & 0xff;
        bNewMainDec = bGetInternalDec(SV_VP_MAIN);
        bNewSubDec = bGetInternalDec(SV_VP_PIP);

        if(bNewMainDec != bOldMainDec)
        {
            fgMainCh = TRUE;
        }

        if(bNewSubDec != bOldSubDec)
        {
            fgPipCh = TRUE;
        }

        bOldMainDec = bNewMainDec;
        bOldSubDec = bNewSubDec;
       
        if((!fgMainCh) && (!fgPipCh))
        {
            MainSubSrc_Biglock_Release();
            return SV_NO_CHANGE;
        }
    }

    /************************  input changed *********************/

    /* Move SRM_SendEvent after critical section */

    // Restore fgAutoSearch Flag
    //    if (((fgMainCh== TRUE) && (bGetSignalType(SV_VP_MAIN) != SV_ST_TV) && (bGetSignalType(SV_VP_MAIN) != SV_ST_MAX))||
    //            ((fgPipCh== TRUE)&& (bGetSignalType(SV_VP_PIP) != SV_ST_TV)&& (bGetSignalType(SV_VP_PIP) != SV_ST_MAX)))
    if(((fgMainCh== TRUE) && (bGetSignalType(SV_VP_MAIN) != SV_ST_TV) && (bGetSignalType(SV_VP_MAIN) != SV_ST_MPEG) && (bGetSignalType(SV_VP_MAIN) != SV_ST_MAX))||
       ((fgPipCh== TRUE)&& (bGetSignalType(SV_VP_PIP) != SV_ST_TV)&& (bGetSignalType(SV_VP_PIP) != SV_ST_MPEG) && (bGetSignalType(SV_VP_PIP) != SV_ST_MAX)))
    {
        _fgAutoSearch = FALSE;
    }

    /* shutdown hardware/firmware in main path */
    if(fgMainCh && fgIsMainDec())
    {
        vDrvVideoPathShutDown(SV_VP_MAIN);
    }

    /* shutdown hardware/firmware in pip path */
    if(fgPipCh && fgIsPipDec())
    {
        vDrvVideoPathShutDown(SV_VP_PIP);
    }

    /* bIsChannelOn -> vVdoInISR -> signal status -> WriteCtrl(on/off) */
    /* bSigStatus -> mode change/detect state machine */
    /* only in ISR, it can turn on WriteCtrl */
    if(bMainSrc == SV_VS_MAX)
    {
        _rMChannel.bIsChannelOn = SV_OFF;
        _rMChannel.bDecType = SV_VD_NA;
        //        vDrvScpipWriteCtrl(SV_VP_MAIN,SV_OFF);
    }
    else
    {
        _rMChannel.bIsChannelOn = SV_ON;
		#ifdef CC_DEMOD_FASTACQ
		fgApiEepromWriteByte(EEP_DEMOD_FASTBOOT_LASTSRC,bMainSrc);		
		#endif
#if 1 // defined(CC_FAST_INIT)

        //if(!b_boot_rec_once)
        {
           
#ifdef TIME_MEASUREMENT
           TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME_DRV, "bApiVideoMainSubSrc");
           TMS_DIFF_EX(TMS_FLAG_BOOT, TMS_COOL_BOOT_TIME, "bApiVideoMainSubSrc");
#endif
            x_os_drv_set_timestamp("bApiVideoMainSubSrc");
            b_boot_rec_once = TRUE;
        }

#endif
    }

    if(bSubSrc == SV_VS_MAX)
    {
        _rPChannel.bIsChannelOn = SV_OFF;
        _rPChannel.bDecType = SV_VD_NA;
        //        vDrvScpipWriteCtrl(SV_VP_PIP,SV_OFF);
    }
    else
    {
        _rPChannel.bIsChannelOn = SV_ON;
    }

    /* power down put here */
    if(((_bMainICIn < (UINT8)P_YP0) || (_bMainICIn > (UINT8)P_FB1)) && ((_bSubICIn < (UINT8)P_YP0) || (_bSubICIn > (UINT8)P_FB1))
#ifdef SUPPORT_AV_COMP_AUTO
       && (((_bMainMonICIn< (UINT8)P_YP0) || (_bMainMonICIn > (UINT8)P_FB1)))
#endif
      )
    {
        vDrvAllHDADCPow(SV_FALSE);
    }

    if(((_bMainICIn >= (UINT8)P_YP0)&&(_bMainICIn != (UINT8)P_FB0)&&(_bMainICIn !=(UINT8)P_FB1))&&((_bSubICIn >= (UINT8)P_YP0)&&(_bSubICIn !=(UINT8)P_FB0)&&(_bSubICIn !=(UINT8)P_FB1)))
    {
        vDrvCvbsVfePD();
    }

#if FIX_VGA_HDMI_INTERFERENCE

    if((VSS_MAJOR(bMainSrc) == VSS_VGA && VSS_MAJOR(bSubSrc) != VSS_HDMI) ||
       (VSS_MAJOR(bSubSrc) == VSS_VGA && VSS_MAJOR(bMainSrc) != VSS_HDMI))
    {
        vHDMIPowerOff();
    }

#endif
#if SUPPORT_SCART && SUPPORT_PIP_NOSCARTOUT

    if((_bSubICIn <= P_MA))
    {
        vDrvBypPD();
    }

#endif
    /* disconnect unused internal mux */
    vMuxCleanup();
#if 0 /*  not return so early, the next block of code     */

    /*   is needed to clear some flags for correct flow */
    /* off all hardware first */
    if((bMainSrc == SV_VS_MAX) && (bSubSrc == SV_VS_MAX))
    {
#ifdef CC_SUPPORT_TVE
        /* b2r setting clean-up */
        VDP_B2rSwitch(SV_VP_MAIN, B2R_NS);
        VDP_B2rSwitch(SV_VP_PIP, B2R_NS);
#endif
        MainSubSrc_Biglock_Release();
#ifdef CC_SRM_ON
        SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SOURCE + (UINT32)VDP_1), (UINT32)bMainSrc, 0);
        SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SOURCE + (UINT32)VDP_2), (UINT32)bSubSrc, 0);
#endif
        return SV_SUCCESS;
    }

#endif
    vDrvSetExternalMux(_bSrcMainNew, _bSrcSubNew);

    if(fgMainCh)
    {
		if((_bSrcMainNew != _bSrcSubNew) || (fgPipCh))
        {
            vDrvSetInternalMux(0,_bSrcMainNew);
        }
        vSetMOutMux(bNewMainDec);
    }

#ifdef SUPPORT_AV_COMP_AUTO

    if(fgMainCombi)
    {
        vDrvSetInternalMux(0,_bMonMainNew);
    }

#endif

    if(fgPipCh)
    {
		if((_bSrcMainNew != _bSrcSubNew) || (fgMainCh))
	    {
            vDrvSetInternalMux(1,_bSrcSubNew);
	    }
        vSetSOutMux(bNewSubDec);
    }

    // MDisableMainINT();

    if(fgMainCh)
    {
        _rMChannel.bDecType = bNewMainDec;

        /* Update information for Main Channel New Decoder */
        if(fgIsMainDec())
        {
            /* VBI */
#if SUPPORT_VBI
            if(VBI_GetSlicerPath(VBI0) == SV_VP_MAIN)
            {
                VBI_DisableBySlicer(VBI0);
            }
            else
            {
                VBI_DisableBySlicer(VBI2);
            }

#endif
            vDrvVideoConnect(SV_VP_MAIN, SV_ON);
        }

        _bMainState = VDO_STATE_IDLE; /* mode change state machine */
        vClrMainFlg(MAIN_FLG_MODE_DET_DONE);
        vSetMainFlg(MAIN_FLG_MODE_CHG);
        // Luis060627, for delay mute
        vApiVideoClrEvtFlg(VDO_FLG_MAIN_MODECHG_DONE);
#ifndef CC_COPLAT_MT82

        
#if defined(CC_B2R_RM_SUPPORT)
        if(bMainSrc == SV_VS_DT1)
       {
            VDP_SetVideSrc(SV_VP_MAIN, B2R_1);
        }
        else if(bMainSrc == SV_VS_DT2)
       {
            VDP_SetVideSrc(SV_VP_MAIN, B2R_2);
       }
       else
       {
            VDP_SetVideSrc(SV_VP_MAIN, B2R_NS);
       }
#elif defined(CC_B2R_RES_SUPPORT)
        if(bMainSrc == SV_VS_DT1)
        {
            UCHAR ucChgId = B2R_NS;
            
            if(B2R_ChkHwSwitch(SV_VP_MAIN, B2R_1, &ucChgId))
            {
                VDP_B2rSwitch(SV_VP_MAIN, ucChgId);
            }
            else
            {
                VDP_B2rSwitch(SV_VP_MAIN, B2R_1);
            }
        }
        else if(bMainSrc == SV_VS_DT2)
        {
            UCHAR ucChgId = B2R_NS;

            if(B2R_ChkHwSwitch(SV_VP_MAIN, B2R_2, &ucChgId))
            {
                VDP_B2rSwitch(SV_VP_MAIN, ucChgId);
            }
            else
            {
                VDP_B2rSwitch(SV_VP_MAIN, B2R_2);
            }
        }
        else
        {
            VDP_B2rSwitch(SV_VP_MAIN, B2R_NS);
        }
#else
        if(bMainSrc == SV_VS_DT1)
        {
            VDP_B2rSwitch(SV_VP_MAIN, B2R_1);
        }
        else if(bMainSrc == SV_VS_DT2)
        {
            VDP_B2rSwitch(SV_VP_MAIN, B2R_2);
        }
        else
        {
            VDP_B2rSwitch(SV_VP_MAIN, B2R_NS);
        }
#endif
        
#endif
    }

#ifdef SUPPORT_AV_COMP_AUTO

    if(fgMainCombi)
    {
        //doesn't connect to yppbr decoder , and just init hardware
        vDrvVideoConnect(SV_VP_MON, SV_ON);
    }

#endif

    if(fgPipCh)
    {
        _rPChannel.bDecType = bNewSubDec;

        /* Update information for Main Channel New Decoder */
        if(fgIsPipDec())
        {
            /* VBI */
#if SUPPORT_VBI
            if(VBI_GetSlicerPath(VBI0)== SV_VP_PIP)
            {
                VBI_DisableBySlicer(VBI0);
            }
            else
            {
                VBI_DisableBySlicer(VBI2);
            }

#endif
            vDrvVideoConnect(SV_VP_PIP, SV_ON);
        }

        _bPipState = VDO_STATE_IDLE; /* mode change state machine */
		
		#ifdef CC_OSD_ENCODE
	    if(bGetICInputType(SV_VP_PIP)!=P_OSDENCODE &&
	       bGetICInputType(SV_VP_PIP)!=P_OSTGENC)
	    {
            vClrPipFlg(PIP_FLG_MODE_DET_DONE);
            vSetPipFlg(PIP_FLG_MODE_CHG);
        }
		#else
            vClrPipFlg(PIP_FLG_MODE_DET_DONE);
            vSetPipFlg(PIP_FLG_MODE_CHG);
		#endif

        // Luis060627, for delay mute
        vApiVideoClrEvtFlg(VDO_FLG_PIP_MODECHG_DONE);
#ifndef CC_COPLAT_MT82
#if defined(CC_B2R_RM_SUPPORT)
        if(bSubSrc == SV_VS_DT1)
        {
            VDP_SetVideSrc(SV_VP_PIP, B2R_1);
        }
        else if(bSubSrc == SV_VS_DT2)
        {
            VDP_SetVideSrc(SV_VP_PIP, B2R_2);
        }
        else
        {
            VDP_SetVideSrc(SV_VP_PIP, B2R_NS);
        }
#elif defined(CC_B2R_RES_SUPPORT)
        if(bSubSrc == SV_VS_DT1)
        {
            UCHAR ucChgId = B2R_NS;
            
            if(B2R_ChkHwSwitch(SV_VP_PIP, B2R_1, &ucChgId))
            {
                VDP_B2rSwitch(SV_VP_PIP, ucChgId);
            }
            else
            {
                VDP_B2rSwitch(SV_VP_PIP, B2R_1);
            }
        }
        else if(bSubSrc == SV_VS_DT2)
        {
            UCHAR ucChgId = B2R_NS;

            if(B2R_ChkHwSwitch(SV_VP_PIP, B2R_2, &ucChgId))
            {
                VDP_B2rSwitch(SV_VP_PIP, ucChgId);
            }
            else
            {
                VDP_B2rSwitch(SV_VP_PIP, B2R_2);
            }
        }
        else
        {
            VDP_B2rSwitch(SV_VP_PIP, B2R_NS);
        }
#else
        // William 20060316
        if(bSubSrc == SV_VS_DT1)
        {
            VDP_B2rSwitch(SV_VP_PIP, B2R_1);
        }
        else if(bSubSrc == SV_VS_DT2)
        {
            VDP_B2rSwitch(SV_VP_PIP, B2R_2);
        }
        else
        {
            VDP_B2rSwitch(SV_VP_PIP, B2R_NS);
        }
#endif

#endif
    }

    // MEnableMainINT();
    
#ifdef CC_SRM_ON
    SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SOURCE + (UINT32)VDP_1), (UINT32)bMainSrc, 0);
    SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SOURCE + (UINT32)VDP_2), (UINT32)bSubSrc, 0);
#endif
    MainSubSrc_Biglock_Release();
    return SV_SUCCESS;
}

#ifdef CC_SUPPORT_PIPELINE

UINT8 bApiVFEAVDConnect(UINT8 bOnOff,UINT8 bMainSrc, UINT8 bSubSrc)
{
    BOOL fgMainCh = FALSE;
    ExtInputTable NewExtInput;
	// connect AVD is from here
	if(bOnOff==0x1)
		
{
    if(bMainSrc ==SV_VS_NO_CHANGE)
    {
        bMainSrc = _fVFEAVDSourceMainOld;
    }

    if(bSubSrc ==SV_VS_NO_CHANGE)
    {
        bSubSrc = _fVFEAVDSourceSubOld;
    }
	
    NewExtInput.MapIntMode = bDrvGetMapIntMode(bMainSrc, bSubSrc);

    /*AVD source is changed ? */
    {

        if(_fVFEAVDSourceMainOld != bMainSrc)
        {
            fgMainCh = TRUE;
        }
        _fVFEAVDSourceMainNew = bMainSrc;
        _fVFEAVDSourceMainOld = bMainSrc;
        _fVFEAVDSourceSubNew = bSubSrc;
        _fVFEAVDSourceSubOld = bSubSrc;
	    _fVFEAVDMainICPin = NewExtInput.MapIntMode >> 8;
	    _fVFEAVDSubICPin = NewExtInput.MapIntMode & 0xff;
       
    }
	LOG(0, "11111bApiVFEAVDConnect(_fVFEAVDSourceMainNew=%d, _fVFEAVDSourceMainOld=%d,_fVFEAVDSourceSubNew=%d,_fVFEAVDSourceSubOld=%d,_fVFEAVDMainICPin=%d,_fVFEAVDSubICPin=%d\n",
     _fVFEAVDSourceMainNew,_fVFEAVDSourceMainOld,_fVFEAVDSourceSubNew,_fVFEAVDSourceSubOld,_fVFEAVDMainICPin,_fVFEAVDSubICPin);
    //if(fgMainCh)
    {
            vDrvSetInternalMuxVFE_AVD(0,_fVFEAVDSourceMainNew);   // connect VFE and ADC
       // vSetMOutMux(bNewMainDec);  need video path to set it
    }

    //if(fgMainCh)
    {
			
#if SUPPORT_SCART
			/* considering SCART-RGB */
			//if(_bMainICIn == P_FB0)
			if(VSS_MAJOR(_fVFEAVDSourceMainNew) == VSS_SCART)
			{
				vDrvSetPorchTune(0x0, SV_PORCHTUNE_SCPOS);   // need to do ?
			}

#endif
		vTvd3dConnect(0x0, SV_ON);//just  connect TVD

     }

}
// disconnet  from here	
else
{   
	vDrvAvMux(0);     // tuner off the AVMUX 
	vDrvCvbsVfePD();  //power down cvbs ADC  and Scart RGB will not get the information
	vTvd3dConnect(0x0, SV_OFF);// disconnect TVD
}
    return SV_SUCCESS;
}
#endif


/**
 * Set color systrem.
 *
 * @param bColSys SV_CS_AUTO, SV_CS_PAL, SV_CS_PAL_M, SV_CS_PAL_N,
 * SV_CS_NTSC358, SV_CS_NTSC443 or SV_CS_SECAM
 * @return The exit status of this api.
 */
UINT8 fgApiVideoColorSystem(UINT8 bColSys)
{
    if(fgIsMainTvd3d()||fgIsPipTvd3d())
    {
        vDrvTvd3dSetColorSystem(bColSys);
        return (TRUE);
    }

    return FALSE;
}

/*
*  analog copy protect 
*  pu1Psp : pseudo sync and AGC pluese
*  pu1CStirpe: color stripe
*
*/
UINT8 bApiVideoGetAnalogCopyProtect(UINT8 bPath, UINT8 *pu1Psp, UINT8 *pu1CStripe)
{
	UINT8 bType = bGetVideoDecType(bPath);

	switch(bType)
	{
		case SV_VD_TVD3D:
			*pu1Psp = vTvd3dGetAnalogCopyProtectPSync() ? 1:0;
			*pu1CStripe = vTvd3dGetAnalogCopyProtectCStripe();
			LOG(1,"bApiVideoGetAnalogCopyProtect SV_VD_TVD3D PSYNC(%d), CSTRIPE(%d)\n",
				*pu1Psp,*pu1CStripe);
			break;
		case SV_VD_YPBPR:
		#ifdef HDTV_MV_DETECT_SUPPORT
			*pu1Psp = bDrvAsyncMvStatus() ? 1:0;
			*pu1CStripe =0;
			LOG(1,"defined ");
		#else
		    *pu1Psp =0;
		    *pu1CStripe =0;
			LOG(1,"undefined ");
		#endif
		    LOG(1,"bApiVideoGetAnalogCopyProtect SV_VD_YPBPR PSYNC(%d), CSTRIPE(%d)\n",
				*pu1Psp,*pu1CStripe);
			break;
		default:
			break;
	}

	return TRUE;
}

/**
 * Get Current TV color system
 *
 * @return Current Color System. SV_CS_AUTO / SV_CS_PAL / SV_CS_PAL_M
 * / SV_CS_PAL_N / SV_CS_NTSC358 / SV_CS_NTSC443 / SV_CS_SECAM
 */
UINT8 bApiVideoGetColorSystem(void)
{
    return bTvd3dGetColorSystem();
}


UINT8 bApiVideoGetInputTimingID(UINT8 bPath)
{
    switch(bGetVideoDecType(bPath))
    {
        case SV_VD_YPBPR:
            return _bHdtvTiming;

        case SV_VD_VGA:
        {
            UINT8 bTimingId;

            if(_bVgaTiming<bUserVgaTimingBegin)
            {
                return _bVgaTiming;
            }

            /* user mode */
            bTimingId = _bVgaTiming-bUserVgaTimingBegin;

            if(bTimingId >= USERMODE_TIMING)
            {
                return 0;
            }

            if(rVgaUsrEEP[bTimingId].id)
            {
                return _bVgaTiming;
            }
            else
            {
                return _bVgaTiming+USERMODE_TIMING;
            }
        }

        case SV_VD_DVI:
            return _bDviTiming;

        default:
            return 0;
    }
}


UINT8 bApiVideoGetVideoTiming(UINT8 bPath)
{
    UINT8 _bTiming;
    UINT8 _bDecType;

    if(bPath == SV_VP_MAIN)
    {
        _bDecType = _rMChannel.bDecType;
    }
    else
    {
        _bDecType = _rPChannel.bDecType;
    }

    switch(_bDecType)
    {
        case SV_VD_YPBPR:
            _bTiming = _bHdtvTiming;
            break;

        case SV_VD_VGA:
            _bTiming = _bVgaTiming;
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            _bTiming = _bDviTiming;
            break;
#endif

        default:
            return SV_VIDEO_TM_MAX;
    }

    switch(_bTiming)
    {
        case MODE_525I:
        case MODE_525I_OVERSAMPLE:
            return SV_VIDEO_TM_480I;	// "480I"

        case MODE_625I:
        case MODE_625I_OVERSAMPLE:
            return SV_VIDEO_TM_576I;	// "576I"

        case MODE_480P:
        case MODE_480P_OVERSAMPLE:	
            return SV_VIDEO_TM_480P;	// "480P"

        case MODE_576P:
        case MODE_576P_OVERSAMPLE:
            return SV_VIDEO_TM_576P;	// "576P"

        case MODE_720p_50:
        case MODE_720p_60:
            return SV_VIDEO_TM_720P;	// "720P"

        case MODE_1080i_50:
        case MODE_1080i:
            return SV_VIDEO_TM_1080I;// "1080i"

        case MODE_1080p_50:
        case MODE_1080p_60:
            return SV_VIDEO_TM_1080P;// "1080P"

        default:
            return SV_VIDEO_TM_MAX;
    }
}


// VGA Politary Check Modify by W.C Shih2006/10/30
UINT8 bApiVideoGetInputPolarity(UINT8 bPath)
{
    UINT8 bType,bPolarity;
    INT32 i;
    UINT32   PolarityCheckFlag;
    bType = bGetVideoDecType(bPath);
    i = DRVCUST_OptQuery(ePolitaryCheck,&PolarityCheckFlag);

    if((i<0)||(PolarityCheckFlag==0))
    {
        bPolarity = 0;
    }
    else
    {
        bPolarity = (fgSP0Hpol<<1) + (fgSP0Vpol);
    }

    switch(bType)
    {
        case SV_VD_VGA:
            return bPolarity;

        default:
            return 0;
    }
}

/**
 * @brief
 * @param
 * @retval
 * @example
 */
UINT16 wApiVideoGetInputWidth(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wRet = 0;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_YPBPR:
        case SV_VD_VGA:
            wRet = wDrvVideoInputWidth(bPath);//wHdtvInputWidth(); need include

            if(bHdtvInputWidthOverSample(bDrvVideoGetTiming(bPath)))
            {
                wRet>>=1;
            }

            return wRet;

        case SV_VD_TVD3D:
            wRet = wDrvVideoInputWidth(bPath);//wHdtvInputWidth(); need include

            if(vTvdGetOverSampleEnable())
            {
                wRet>>=1;
            }

            return wRet;

        default:
            return wDrvVideoInputWidth(bPath);
    }
}

/**
 * @brief
 * @param
 * @retval
 * @example
 */
UINT16 wApiVideoGetInputHeight(UINT8 bPath)
{
    return wDrvVideoInputHeight(bPath);
}

/**
 * @brief
 * @param
 * @retval
 * @example
 */
UINT8 bApiVideoGetRefreshRate(UINT8 bPath)
{
#if 0
    // ToDo
#if SUPPORT_VGA_USERMODE

    if(_bVgaUserMode)
    {
        return bSP0Vclk;    // Refresh Rate
    }

#endif
#endif
    return bDrvVideoGetRefreshRate(bPath);
}

UINT8 bApiVideoIsSrcInterlace(UINT8 bPath)
{
    return bDrvVideoIsSrcInterlace(bPath);
}

/**
 * @brief function for setting current fix color mode
 * @param bPath current video path to set
 * @param bMode current mode to set
 * @retval void
 */
void vApiVideoSetFixColorSpaceMode(UINT8 bPath, SV_FIXCOLORSPACE_MODE bMode)
{
    _bFixColorSpace[bPath]=bMode;
}

/**
 * @brief function for getting current fix color mode
 * @param bPath specify which video path to get
 * @retval current fix color mode
 */
SV_FIXCOLORSPACE_MODE bApiVideoGetFixColorSpaceMode(UINT8 bPath)
{
    return _bFixColorSpace[bPath];
}

void vApiHdmiColorModeChg(UINT8 bPath)
{
    if(fgIsMainDVI() || fgIsPipDVI())
    {
        //force hdmi color space mode change
        _bHDMIColorSpace = 0xff ;

        if(fgIsMainDVI())
        {
            _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, FOREVER_MUTE, FALSE);
        }

        if(fgIsPipDVI())
        {
            _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_PIP, FOREVER_MUTE, FALSE);
        }

        LOG(3, " HDMI color mode change from VDP\n");
    }
}

UINT8 fgApiVideoIsVgaTiming(UINT8 bPath)
{
    UINT8 bDecType;
    bDecType = bGetVideoDecType(bPath);

    switch(bDecType)
    {
        case SV_VD_VGA:
#ifdef CC_SCPOS_PATTERN_GENERATOR
            if(u1GetScposPtGenEnable())
            {
                return SV_TRUE ;
            }

#endif
            return (((_bVgaTiming) >= VGA_SEARCH_START) && ((_bVgaTiming) <= VGA_SEARCH_END) && (!bVgaInterlace()));
#if SUPPORT_DVI

        case SV_VD_DVI:
            if(fgIsVideoTiming(_bDviTiming))
            {
                return 0;	//video
            }
            else
            {
                return 1;	//vga
            }

#endif
 
#ifdef CC_SUPPORT_PREPROC_TIMING
	   case SV_VD_PREPROC:
			if(fgIsVideoTiming(_bPreProcTiming))
			{
				return 0;	//video
			}
			else
			{
				return 1;	//vga
			}
			
			break;
#endif

        default:
            return 0;
    }
}

// REMOVE ME!
UINT8 fgApiVideoIsVgaMode(UINT8 bPath, UINT8 bChkFlg) reentrant
{
    if((u1GetVGAColorMode() == SV_VGA_MODE_VIDEO) && (bGetSignalType(bPath) == SV_ST_VGA))
    {
        return 0;
    }
    else
    {
        return fgApiVideoIsVgaTiming(bPath);
    }
}

// REMOVE ME!
UINT8 fgApiVideoIsVgaModeEx(UINT8 bPath, UINT8 bChkFlg)
{
    return fgApiVideoIsVgaTiming(bPath);
}


BOOL fgApiVideoIsSourceCVBS(UINT8 bPath)
{
    if(((bPath == SV_VP_MAIN) && ((_rMChannel.bDecType == SV_VD_TVD3D))) ||
       ((bPath == SV_VP_PIP) && (_rPChannel.bDecType == SV_VD_TVD3D))
      )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 * @brief Get main/sub path video source type
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
UINT8 bApiVideoGetSrcType(UINT8 bPath)
{
    if(bPath == SV_VP_MAIN)
    {
        return _bSrcMainNew;
    }
    else
    {
        return _bSrcSubNew;
    }
}


void vApiVideoSetScanMode(UINT32 u4VdpId, UINT32 u4Mode)
{
    CRIT_STATE_T rCritState;

    if(u4VdpId >= 2)
    {
        return;
    }

    if(_u4ScanMode[u4VdpId] != u4Mode)
    {
        rCritState = x_crit_start();
        _u4ScanMode[u4VdpId] = u4Mode;
        x_crit_end(rCritState);
        LOG(3, "auto search path %d mode %d\n", u4VdpId, u4Mode);
        //set mute 20 for autosearch change, change mute interval from 10->20 for entering "analog fine tune" menu transition
        _vDrvVideoSetMute(MUTE_MODULE_AUTOSEARH, u4VdpId, 20, FALSE);

        if(_u4ScanMode[u4VdpId])
        {
            //set modechange count to 0 and invalid it
            _vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_MODECHG, u4VdpId, TRUE, MUTE_INVALID_AUTO_SEARCH);
        }
        else
        {
            //set invalid flag to false
            _vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_MODECHG, u4VdpId, FALSE, MUTE_INVALID_AUTO_SEARCH);
            //_vDrvVideoSetMute(MUTE_MODULE_MODECHG, u4VdpId, FOREVER_MUTE, FALSE);
        }

#ifdef CC_SRM_ON
        SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_SCAN_MODE, u4VdpId, u4Mode);
#endif

        //[DTV00069391] for fine tune , tvd will not notify no signal case , if stable doens't notify
        if(u4Mode == 0 && (bDrvVideoSignalStatus_DBL(u4VdpId) != SV_VDO_STABLE))
        {
            if(fgIsMainTvd3d())
            {
                vSetMainFlg(MAIN_FLG_MODE_CHG);
                vClrMainFlg(MAIN_FLG_MODE_DET_DONE);	// Clear this flag!
            }
            else if(fgIsPipTvd3d())
            {
                vSetPipFlg(PIP_FLG_MODE_CHG);
                vClrPipFlg(PIP_FLG_MODE_DET_DONE);	// Clear this flag!
            }

            vTvd3dTrigModeDet();
        }
    }
}

UINT32 u4ApiVideoGetScanMode(UINT32 u4VdpId)
{
    if(u4VdpId >= 2)
    {
        return 0;
    }

    return _u4ScanMode[u4VdpId];
}


void vApiVideoSetSnowScreen(UINT32 u4OnOff)
{
    _fgSnowScreen = u4OnOff;
}

UINT32 u4ApiVideoGetSnowScreen(void)
{
    return _fgSnowScreen;
}



