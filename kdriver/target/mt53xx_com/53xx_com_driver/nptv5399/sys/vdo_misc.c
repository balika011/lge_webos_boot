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

/*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: vdo_misc.c $
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 * Video Misc Function
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: p4admin $
 *
 * $Modtime: 04/05/31 8:25p $
 *
 * $Revision: #3 $
*************************************************************************/

#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "fbm_drvif.h"
#include "x_bim.h"
#include "x_hal_5381.h"

#include "general.h"
#include "hw_vdoin.h"
#include "hw_scpos.h"
#include "hw_ycproc.h"
#include "hw_ospe.h"
#include "hw_mlvds.h"
#include "hw_di_int.h"
#ifdef REAL_TIME_FRAME_DELAY_EN
#include "hw_di.h"
#endif

#include "drv_video.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "drv_tvd.h"
#include "drv_tdc.h"
#include "vga_auto.h"
#include "drv_di.h"
#include "drv_adcset.h"
#include "drv_vdoclk.h"
#include "drv_autocolor.h"
#include "drv_hdtv.h"
#include "drv_vga.h"
#include "drv_dvi.h"
#include "drv_ycproc.h"
#include "drv_od.h"
#include "vdo_chip.h"
#include "vdo_rm.h"
#include "drv_predown.h"
#include "drv_regen.h"
#include "drv_upscaler.h"
#include "drv_ldmspi.h"
#ifdef CC_SUPPORT_STR
 #if defined(CC_MT5399)
#include "drv_mjc.h"
 #endif
#endif

#if defined (CC_MT5399)|| defined (CC_MT5882)
#include "drv_lcdim_v2.h"
#else
#include "drv_lcdim.h"
#endif

#include "vga_table.h"
#include "drv_contrast.h"
#include "source_select.h"
#include "drv_display.h"
#include "vdo_misc.h"
#include "drv_nr.h"
#include "video_def.h"
#include "api_backlt.h"
#include "nptv_drvif.h"
#include "sv_const.h"
#include "drv_tdc.h"
#include "x_lint.h"
#include "x_assert.h"
#if SUPPORT_SCART
#include "drv_scart.h"
#endif
#include "drv_async.h"
#include "drv_hdmi.h"
#include "drv_dvi.h"
#if SUPPORT_VBI
#include "drv_vbi.h"
#endif
#include "drv_ttd.h"
#include "drv_di.h"
#include "sif_if.h"
#include "drv_lvds.h"
#include "mute_if.h"
#include "b2r_drvif.h"
// add for 5371
#include "drv_mpeg.h"
#include "x_os.h"
#include "c_model.h"
#include "x_hal_5381.h" //  BSP_GetIcVersion
#include "hw_ckgen.h"
#include "nptv_debug.h"
#include "x_gpio.h"
#include "panel.h"

#include "drv_tdtv_drvif.h"

#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#include "srm_drvif.h"
#include "hw_scpos.h"
#include "hw_lvds.h"
#include "u_drv_cust.h"
#endif

#include "hw_mlvds.h"
#include "hw_od.h"
#include "hw_di.h"
#include "drv_od.h"

#ifndef CC_MT5368	//#ifdef MT5368_TODO
#include "drv_tdnavi.h"
#endif

#include "x_pdwnc.h"
#include "drv_default.h"
#include "drv_mon_out.h"
#include "vdp_venc_ctrl.h"

//drv_display.c
extern UINT8 bSI_DISPLAY_DCLK_TYPE;
#ifdef DRV_SUPPORT_EXTMJC
extern UINT32 _u48280Unmte;
//UINT8 _bExtmjcICVersion;
#endif
//mloop_video.c

#ifdef LVDS_ESD_PROTECT
#ifdef CC_MT5882
extern BOOL _fgESDProtect;
#endif
#endif

//drv_dvi.c
extern UINT16 _wDviVTotal;
extern UINT16  _wDviHtotal;

//drv_ycproc.c
extern UINT8 u1SupportHdmiYCbCr444;
extern UINT8 u1SupportVgaYCbCr444;

//drv_matrix.c
extern UINT8 u1HdmiColorMode;
extern UINT8 u1VgaColorMode;
#ifdef CC_SUPPORT_PREPROC_TIMING
extern BOOL _fgPreProcInterlace;
#endif

#if !TDC_USE_FBM_POOL
/*lint -e(950) */

#ifdef __KERNEL__
// TDC_BUF_SIZE=0x251ca0 is too big for kernel BSS
#else
static UINT8 __align(32) bTdcArry[TDC_BUF_SIZE];
#endif
#endif

#ifdef CC_UP8032_ATV
UINT32 xdata u4SWRegArea[1]  _at_   0xdf00;
#endif

UINT32 u4VdoDramStatusMonCount = 0;
 
void vSetVGAColorMode(UINT8 bMode)
{
    if(bMode != u1VgaColorMode)
    {
        u1VgaColorMode = bMode ;
        vVgaTriggerModeChange();
    }
}

UINT8 u1GetVGAColorMode(void)
{
    return u1VgaColorMode;
}

void vSetHDMIColorMode(UINT8 bPath, UINT8 bMode)
{
    if(bMode != u1HdmiColorMode)
    {
        u1HdmiColorMode = bMode ;
        vApiHdmiColorModeChg(bPath);
    }
}

UINT8 u1GetHDMIColorMode(void)
{
    return u1HdmiColorMode ;
}


#if SUPPORT_SBS_CONVERT
extern UINT8 _bSBSOutput;
#endif

BOOL fgIs4k2kScalerBypass(UINT8 bPath)  
{
	return SV_FALSE;
}

BOOL fgIs4k2kNativeTimingOut(UINT8 bPath)
{
	BOOL bRtn = SV_FALSE;
	
    #if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
	if (((wDrvVideoInputWidth(bPath) >= 3840)
	    && (bDrvVideoSignalStatus(bPath) == SV_VDO_STABLE))
		||(_u1Force4K2KTiming ==1)) 
	{
		bRtn = SV_TRUE;
	}
	#endif
	
	return bRtn;
}	
#if defined(CC_SUPPORT_4K2K) && !defined(ANDROID)
//from mt53_fb.c
extern int fgIsPhoto3DModeSupport(void);
#endif

// MW always output 4k2k image, TDTV need to let menu enable when FHD image although it's 4k2k image.

BOOL fgIs4k2kPhoto3DSupport(UINT8 bPath)
{
	
    #ifdef CC_SUPPORT_4K2K
	#ifndef ANDROID
	if(fgIsPhoto3DModeSupport())
	{
		return SV_TRUE;
	}
	else
	#endif //ANDROID
	{
		return !fgIs4k2kNativeTimingOut(bPath);
	}
	#else //CC_SUPPORT_4K2K
	return SV_TRUE ;
	#endif

}

UINT8 bIsScalerInput444(UINT8 bPath) reentrant
{
    UINT8 bIs444;

    if(bPath == VDP_2 && bDrvVrmIsAppFlagSet(VRM_APP_VIDEO_ENC)&&
       ((u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_SCALER) || (u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_DI)))
    {
        return FALSE;
    }

    if(bPath == VDP_2 && bDrvVrmIsAppFlagSet(VRM_APP_GRAPHIC_DUMP)&& bGetICInputType(SV_VP_PIP) == P_OSDENCODE)
    {
        return SV_TRUE;
    }
        
    switch(bGetVideoDecType(bPath))
    {
#if SUPPORT_DVI
        case SV_VD_DVI:
            switch (u1HdmiColorMode)
            {
                case SV_HDMI_MODE_GRAPHIC:
                     if(bDrvVideoIsSrcInterlace(bPath))
                     {
                         bIs444 = SV_FALSE;
                     }
                     else
                     {
                        bIs444 = SV_TRUE;
                      }
                    break;
                case SV_HDMI_MODE_VIDEO:
                    bIs444 = SV_FALSE;
                    break;
                case SV_HDMI_MODE_AUTO:
                default:
                    bIs444 = fgIsVgaTiming(_bDviTiming) ? SV_TRUE : SV_FALSE;
                    break;
            }
            break;
#endif
        case SV_VD_VGA:
            bIs444 = (u1VgaColorMode == SV_VGA_MODE_GRAPHIC) ? SV_TRUE : SV_FALSE;
            break;
        default:
            bIs444 = fgApiVideoIsVgaTiming(bPath) ? SV_TRUE : SV_FALSE;
            break;
    }
    
#if SUPPORT_SBS_CONVERT
    if(_bSBSOutput)
    {
        bIs444 = SV_FALSE;
    }
#endif
	#if  SUPPORT_MIB_3D_CONVERT_444_TO_422
	if(	(TD_MIB_IN(u4DrvTDTVDIModeQuery()) != E_TD_IN_2D_I ||TD_MIB_IN(u4DrvTDTVDIModeQuery()) != E_TD_IN_2D_P )&&
	(TD_MIB_OUT(u4DrvTDTVDIModeQuery()) == E_TD_OUT_3D_FS ||TD_MIB_OUT(u4DrvTDTVDIModeQuery()) == E_TD_OUT_3D_LI)
	)
	{
		bIs444 = SV_FALSE;
	}
	#endif
	
    return bIs444;
}

UINT8 bIsScalerInputRGB(UINT8 bPath) reentrant
{
    UINT8 bIsRGB;

    if(bPath == VDP_2 && bDrvVrmIsAppFlagSet(VRM_APP_VIDEO_ENC)&&
       ((u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_SCALER) || (u1DrvScpipGetVencModule() == E_VENC_MODULE_SUB_DI)))
    {
        return FALSE;
    }
	//ostg to ccir for vss 
	if(bPath == VDP_2 && bDrvVrmIsAppFlagSet(VRM_APP_GRAPHIC_DUMP)&& bGetICInputType(SV_VP_PIP) == P_OSDENCODE)
    {
        return SV_TRUE;
    }        
    switch(bGetVideoDecType(bPath))
    {
#if SUPPORT_DVI
        case SV_VD_DVI:
            bIsRGB = u1SupportHdmiYCbCr444 ? SV_FALSE : bIsScalerInput444(bPath);
            break;
#endif
        case SV_VD_VGA:
            bIsRGB = u1SupportVgaYCbCr444 ? SV_FALSE : bIsScalerInput444(bPath);
            break;
        default:
            bIsRGB = bIsScalerInput444(bPath);
            break;
    }

    return bIsRGB;
}

void vSetVdoMPClk(UINT8 bSelect)
{
	switch(bSelect)
	{
		case MPCLK_300MHZ:
			vIO32WriteFldAlign(OMUX_00, 0, VDOIN_MPCLK_SEL); //clk180m
			vIO32WriteFldAlign(OMUX_VDOIN_CLK_SEL, 0x1, FLD_VDOIN_CLK_SEL_180); // select ethetll/2 = 300MHz
			//0 : sawless/2 = 432/2 = 216
			break;

		case MPCLK_PIPCLK:
			vIO32WriteFldAlign(OMUX_00, 2, VDOIN_MPCLK_SEL);
			break;

		case MPCLK_MAINCLK:
			vIO32WriteFldAlign(OMUX_00, 3, VDOIN_MPCLK_SEL);
			break;

		case MPCLK_324MHZ:
			vIO32WriteFldAlign(OMUX_00, 1, VDOIN_MPCLK_SEL); //clk 192m
			vIO32WriteFldAlign(OMUX_VDOIN_CLK_SEL, 0x0, FLD_VDOIN_CLK_SEL_180); // select syspll/2 = 324MHz
			//1:tapll/2 , whichi s not used in mt5398
		default:
			break;
	}
}

// *********************************************************************
// Function : void vDrvVideoSwInit(void)
// Description : This function is used for video initialization
// Parameter :
// Return    :
// *********************************************************************
void vDrvVideoSwInit(void)
{
    /* Software     */
    x_memset(&_rMChannel,0,sizeof(_rMChannel));
    x_memset(&_rPChannel,0,sizeof(_rPChannel));
    x_memset(&_rTvd3dStat, 0, sizeof(_rTvd3dStat));
    x_memset(&_rVGAStat, 0, sizeof(_rVGAStat));
    x_memset(&_rYPBPRStat, 0, sizeof(_rYPBPRStat));
    x_memset(&_rDVIStat, 0, sizeof(_rDVIStat));
    // add for 5371
    x_memset(&_rMpegSDStat, 0, sizeof(_rDVIStat));
    x_memset(&_rMpegHDStat, 0, sizeof(_rDVIStat));
    x_memset(&_rMDispInfo, 0, sizeof(_rMDispInfo));
    x_memset(&_rPDispInfo, 0, sizeof(_rPDispInfo));
    x_memset(&_rMDispPrm, 0, sizeof(_rMDispPrm));
    x_memset(&_rPDispPrm, 0, sizeof(_rPDispPrm));
    x_memset(&_rMPicInfo, 0, sizeof(_rMPicInfo));
    x_memset(&_rPPicInfo, 0, sizeof(_rPPicInfo));


    //added for mt5399 monitor out
    x_memset(&_rMonitorOutInfo,0,sizeof(_rMonitorOutInfo));

    
    _rMPicInfo.wUIHPorch = 1000;
    _rMPicInfo.wUIVPorch = 1000;
    _rPPicInfo.wUIHPorch = 1000;
    _rPPicInfo.wUIVPorch = 1000;
    //#ifdef DRV_SUPPORT_EXTMJC
    //    _bExtmjcICVersion= bGetICVersion();
    //Printf("_bExtmjcICVersion %d\n", _bExtmjcICVersion);
    //#endif
    // Event Flag
    vClrMainFlg(0xff);
    vClrPipFlg(0xff);
    vClrScposFlg(0xffffffff);
    _wEventFlag = 0;
    // Vdo Main/Pip State
    _bMainState = VDO_STATE_IDLE;
    _bPipState = VDO_STATE_IDLE;
    // TVD
    _bSetChannelCnt = 0x40;
	//AVD setting
	
#ifdef CC_SUPPORT_PIPELINE
	_fVFEAVDSourceMainNew=(UINT8)SV_VS_MAX;
    _fVFEAVDSourceMainOld=(UINT8)SV_VS_MAX;
    _fVFEAVDSourceSubNew=(UINT8)SV_VS_MAX;
    _fVFEAVDSourceSubOld=(UINT8)SV_VS_MAX;
	_fVFEAVDMainICPin=(UINT8)P_FA;
	_fVFEAVDSubICPin=(UINT8)P_FA;
	_fVSCConnectAVD=0x0;  // init set VSC do not  connect the AVD
#endif
    // source_select.c
    _bSrcMainOld = (UINT8)SV_VS_MAX;
    _bSrcSubOld = (UINT8)SV_VS_MAX;
    _bSrcMainNew = (UINT8)SV_VS_MAX;
    _bSrcSubNew = (UINT8)SV_VS_MAX;
    /* For Combi Component, we should keep monitored information */
    _bMonMainOld = (UINT8)SV_VS_MAX;
    _bMonSubOld = (UINT8)SV_VS_MAX;
    _bMonMainNew = (UINT8)SV_VS_MAX;
    _bMonSubNew = (UINT8)SV_VS_MAX;
    _bMainMonICIn = (UINT8)P_FA;
    _bSubMonICIn = (UINT8)P_FA;
#ifdef DRV_SUPPORT_EXTMJC
    _u48280Unmte = 0;
#endif
    // vga_auto.c
    _bAutoFlag = 0;
    _bVdoSP0AutoState = VDO_AUTO_NOT_BEGIN; //0;
    _bVgaDelayCnt = 0;
    //vdo_clk.c
    _bCLKSetFlag = 0;
    // drv_autocolor.c
    _bAutoColorState0 = VDO_AUTO_COLOR_NOT_BEGIN;
    // drv_tvd.c
#if TVD_AUTO_CALIB
    _bTVDAutoCalibState = VDO_AUTO_COLOR_NOT_BEGIN;
#endif
    // drv_hdtv.c
    _bHdtvTiming = NO_SIGNAL;
    // drv_vga.c
#if SUPPORT_VGA_USERMODE
    _bVgaUserMode = 0;
#endif
#if SUPPORT_DVI
    //drv_dvi.c
    _bDviExtSrc = SV_DV_NONE;
    vClrDviFlg(0xFF);
#endif
    // hw_clk.c
    _fgFRS_ENA = SV_OFF;
    // video_data.c
    // for mute control
    _fgMainBlueMute = FALSE;
    _fgSubBlueMute = FALSE;
    _fgMainBlackMute = FALSE;
    _fgSubBlackMute = FALSE;

    //VRM sw init
    vVRMSwInit();
	vVrmMonOutSwInit();
    //Predown sw init
    vPDSSwInit(SV_VP_MAIN);
    vPDSSwInit(SV_VP_PIP);
    //Regen sw init
    vRegenSwInit(SV_VP_MAIN,REGEN_MODULE_PRE_REGEN);
    vRegenSwInit(SV_VP_PIP,REGEN_MODULE_PRE_REGEN);
    //Scaler sw init
    vRegenSwInit(SV_VP_MAIN,REGEN_MODULE_FSC_REGEN);
    vRegenSwInit(SV_VP_PIP,REGEN_MODULE_FSC_REGEN);
    vScpipSwInit();
    // DI Init
    vDrvDISwInit(SV_VP_MAIN);
    vDrvDISwInit(SV_VP_PIP);
#if defined(SCPIP_SUPPORT_POST_SCALER)
    vPscSwInit();
#endif
    //#if SUPPORT_SCART_SFIRST
#if SUPPORT_SCART  //Gene Chang For the SCART Auto to remove the S first function 2006.05.12
    //_Enable_SCART_SF = 0;  //Gene Chang For the SCART Auto to remove the S first function 2006.05.12
    _bSFisSV =SV_FALSE;
#endif
#if SUPPORT_SCART
    _bTrigScartAuto= SV_FALSE;
#endif
    _fgVideoReady = FALSE;
    //_fgEnableMdChgMute = TRUE;
    _fgAutoSearch = FALSE;
	
#ifdef LVDS_ESD_PROTECT
#ifdef CC_MT5882
	_fgESDProtect = TRUE;
#endif
#endif

    //monitor out sw init
    #if defined(CC_MT5399) 
    vDrvMonOutInit();
	#endif

#ifdef CC_SUPPORT_VENC
    vDrvVencVDPInit();
#endif

    VSS_Support_Initial();
    PE_InitUiRange();
}
void vDrvVideoSysClockInit(void)
{
	vSetVdoMPClk(MPCLK_324MHZ);
	//select sync proc clock source : 27m/24m
	vIO32WriteFldAlign(OMUX_VDOIN_CLK_SEL, 0x0, FLD_VDOIN_CLK_SEL_CLK27);

    vIO32Write4B(CKGEN_VPCLK_STOP, 0); // enable all videoin clock
    
    //min valid h-sync width 48->40.
    vIO32WriteFldAlign(TG_00, 0x0A, MIN_HWIDTH); //MT5381TVDPort
    vIO32WriteFldAlign(SYS_00, 1, MAIN_DATA_SYNC_HIGH);
    vIO32WriteFldAlign(SYS_00, 1, PIP_DATA_SYNC_HIGH);
    // when mpclk < main clk, set DATA_SYNC_HIGH to 0 to avoid overflow (send request every 2 pixels)
    // when mpclk > main clk, set DATA_SYNC_HIGH to 1 to reduce HSync jitter
#if MANUAL_MAIN_RST
    //default value is 1
    vIO32WriteFldAlign(SYS_00, 0, DATA_SYNC_AUTO);
#endif
    // enable omux V align with H
    vIO32WriteFldAlign(SYS_05, 1, FV_OUT_ALIGN);
    vIO32WriteFldAlign(LPF_PIP_00, 1, MAIN_PROG_FLD_EN);
    vIO32WriteFldAlign(LPF_PIP_00, 1, PIP_PROG_FLD_EN);
    //1:3/4 Field change  0:Field change follow MAIN_FIELD_POS
    vIO32WriteFldAlign(OMUX_01, 1, PIP_F_SEL);
    vIO32WriteFldAlign(OMUX_01, 1, MAIN_F_SEL);
    vIO32WriteFldAlign(OMUX_01, 0, MAIN_FIELD_POS);
    //set this bit for debug status and pattern gen
    vIO32WriteFldAlign(SYS_05, 1, FIRST_LINE_V_POL);

    //3D Init , DISP_R Should not be inversed
    vIO32WriteFldAlign(LPF_PIP_00, 0, DISP_R_INV); 

}
UINT8 vDrvVideoHwInit_SWInited = FALSE;
// *********************************************************************
// Function : void vDrvVideoHwInit(void)
// Description :
// Parameter :
// Return    :
// *********************************************************************
void vDrvVideoHwInit(void)
{
#ifndef CC_UP8032_ATV
    FBM_POOL_T  *_prFbmPool;
#endif

    // Clock
    // Video ADC testing
    vDrvADCDefaultSetting();//5371
    vDrvVideoSysClockInit();
#ifndef CC_UP8032_ATV

    if(!vDrvVideoHwInit_SWInited)
    {
        _prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_VBI);
        // ASSERT(_prFbmPool != NULL);
        VBI_DRAM_BASE = _prFbmPool->u4Addr;
        VBI_DRAM_SIZE = _prFbmPool->u4Size;
        _prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_NR);
        // ASSERT(_prFbmPool != NULL);
        NR_DRAM_BASE = _prFbmPool->u4Addr;
        NR_DRAM_SIZE = _prFbmPool->u4Size;
#if /*__KERNEL__ ||*/ TDC_USE_FBM_POOL
        _prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_TDC);
        // ASSERT(_prFbmPool != NULL);
        TDC_DRAM_BASE = _prFbmPool->u4Addr;
        TDC_DRAM_SIZE = _prFbmPool->u4Size;
#else
        TDC_DRAM_BASE = (UINT32)bTdcArry;
        TDC_DRAM_SIZE = TDC_BUF_SIZE;
#endif
    }

    // TDC Dram
    vDrvTDCSetDramBase();
    vDrvTDTVInit(TRUE);
#endif
    // DI Init
    vDrvDIHwInit(SV_VP_MAIN);
    vDrvDIHwInit(SV_VP_PIP);
    // TDC init
    vDrvTDCOnOff(SV_OFF);
    // NR Init
    vDrvNRInit();
    // Adaptive control Init
    vDrvAdaptiveFlgInit();
    vDrvAdaptivePSCANFlgInit();
    vDrvVideoMuteColor(SV_VP_MAIN, 0);
    vDrvVideoMuteColor(SV_VP_PIP, 0);
    //OD init
    //vDrvOdInit();
    // 5371 -- MUTE OFF
    //// Marvin Start
    _vDrvVideoSetMute(MUTE_MODULE_BLUE, SV_VP_MAIN, SV_OFF, FALSE);
    _vDrvVideoSetMute(MUTE_MODULE_BLUE, SV_VP_PIP, SV_OFF, FALSE);
    _vDrvVideoSetMute(MUTE_MODULE_BLACK, SV_VP_MAIN, SV_OFF, FALSE);
    _vDrvVideoSetMute(MUTE_MODULE_BLACK, SV_VP_PIP, SV_OFF, FALSE);
    //// Marvin End
#if 0
#ifdef NEW_COUNTRY_TYPE

    if(GetTargetCountryMask(COUNTRY_ATV_MASK)==COUNTRY_ATV_EU)
#else
#ifdef COUNTRY_FROM_EEPROM
    u1TargetCountryInfo = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;;

    if(u1TargetCountryInfo == COUNTRY_EU)
#else
    if(GetTargetCountry()==COUNTRY_EU)
#endif
#endif
    {
        vDrvLCDFreqSet(SV_DCLK_50HZ, 50);
    }
    else
    {
        // if panel already on, don't set again.
        if(!bIsPanelOn())
        {
            vDrvLCDFreqSet(SV_DCLK_60HZ, 60);
        }
        else
        {
            bSI_DISPLAY_DCLK_TYPE = SV_DCLK_60HZ;
            bOUTPUT_REFRESH_RATE = 60;
            _u4VPllFrequency = dwSI_DISPLAY_DCLK_TYPICAL;//for dynamic MDR
#ifndef CC_MTK_LOADER
            vDrvSetSSMDRAsInputHsync();//for dynamic MDR
#endif
            vDrvSwitchImportProtection(0);
#ifndef CC_MTK_LOADER
#ifdef SUPPORT_DDDS_STEP_TRACKING
            vDrvUpdateDisplayCenterCW(IO32ReadFldAlign(DDDS_00,DDDS_00_DDDS_FREQ_CW));
#endif
#endif
        }
    }

#endif

    if(!vDrvVideoHwInit_SWInited)
    {
#if TVD_DFT_PAL
        //fg525 = FALSE;
        //vTvd3dSetDefType(fg525);//MT5381TVDPort Removed
        // PreSet Firmware TVD Status, for TVD mode reference
        _rTvd3dStatus.bTvdMode = SV_CS_PAL;
        _rTvd3dStatus.fgIs525 = FALSE;
        _rTvd3dStatus.fgVPres = FALSE;
#else
        //fg525 = TRUE;
        //vTvd3dSetDefType(fg525);//MT5381TVDPort Removed
        // PreSet Firmware TVD Status, for TVD mode reference
        _rTvd3dStatus.bTvdMode = SV_CS_NTSC358;
        _rTvd3dStatus.fgIs525 = TRUE;
        _rTvd3dStatus.fgVPres = FALSE;
#endif
        _rTvd3dStatus.fgSVPres = FALSE;
        _rTvd3dStatus.bColSys = SV_CS_AUTO;
        _bTvdHoldMode = SV_FALSE;
    }

    // YCPROC Init
    vDrvYcprocInit();

    //predown hw init
    vPDSHwInit(SV_VP_MAIN);
    vPDSHwInit(SV_VP_PIP);

    //regen hw init
    vRegenHwInit(SV_VP_MAIN,REGEN_MODULE_PRE_REGEN);
    vRegenHwInit(SV_VP_PIP,REGEN_MODULE_PRE_REGEN);

    //scaler hw init
    vRegenHwInit(SV_VP_MAIN,REGEN_MODULE_FSC_REGEN);
    vRegenHwInit(SV_VP_PIP,REGEN_MODULE_FSC_REGEN);
    vScpipHwInit(SV_VP_MAIN);
#if SUPPORT_POP
    vScpipHwInit(SV_VP_PIP);
#endif
    //post scaler hw init
#if defined(SCPIP_SUPPORT_POST_SCALER)
    vPscHwInit();
#endif
    vDrvSCPQInit();

    MJC_INIT(TRUE);

#ifdef __MODEL_slt__
    // front 3x3 error diffusion
    vIO32WriteFldAlign(TRANS_MAIN_07, 0, ERR_DFN);
    vIO32WriteFldAlign(TRANS_PIP_07, 0, ERR_DFN);
    // post and front dither
    vIO32Write4B(OS_DITHER_00, 0);
    vIO32Write4B(OS_DITHER_03, 0);
    // front scaler dither
    //vIO32Write4B(SCPIP_DS1_0B, 0);
    //vIO32Write4B(SCPIP_DS2_08, 0);
#endif
    //OD init
    vDrvODInit();

    vDrvVideoHwInit_SWInited = TRUE;
}

#ifdef CC_SUPPORT_STR
void vDrvNptvResume(void)
{
    vDrvVideoSysClockInit();
}

extern UINT32 SW_DRAM_BASE;    

void vDrvVideoResume(void)
{
    vIO32Write4B(SW_REG_BASE_ADDR, SW_DRAM_BASE);

    vDrvVideoMuteResume();
    vDrvScposResume();
    vDrvMIBResume();    
    vDrvPreprocResume();

    #if defined(CC_MT5399)
    vDrvMJCResume();
    vDrvTddcResume();
    #endif

    vDrvPostprocResume();
    vDrvODResume();
    vDrvLcDimPQResume();
    vDrvLcdimSpiResume();
    vDrvTTDResume();
}

void vDrvVideoSuspend(void)
{
    vDrvMIBSuspend();
    vDrvScposSuspend();

    #if defined(CC_MT5399)
	vDrvMJCSuspend();
	#endif

    vDrvPostprocSuspend();
    vDrvODSuspend();
    vDrvLcDimPQSuspend();
    vDrvLcdimSpiSuspend();
    vDrvTTDSuspend();
}
#endif

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
BOOL fgIsTvdHLock(void)
{
    return fgHwTvdHLock();
}

BOOL fgIsTvdVLock(void)
{
    return fgHwTvdVLock();
}

// *********************************************************************
// Function :
// Description :
//  (1) It is used for VBI parameter setting.
//  (2) The VDOSRC check should be reviewed.
//      Possible Setting:
//          VDOSRC_TVD3D - NTSC/PAL
//          VDOSRC_HDTV - 480i/576i/480p/576p
//          VDOSRC_DVI - 480i/576i/480p/576p (AD9883, MST9888 Digital Port input).
//  (3) The Video Size check should be reviewed.
//      Possible Setting
//          VDOSRC_TVD3D - by _rTVD3dStatus
//          VDOSRC_HDTV - by _bHdtvTiming
//          VDOSRC_DVI - by _bDviTiming
//  (4) (2)+(3) Merge to VDOSRC and timing check
//  (5) The return type should review again.
//          We only support VDOTYPE_NTSC and VDOTYPE_PAL two VBI system.
//          But we also need to set different setting for TVD/HDTV/DVI input.
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoGetType(UINT8 bPath)
{
    UINT8 bDecType;
    UINT8 u1Ret;
    // 1. Check Channel Decoder
    bDecType = bGetVideoDecType(bPath);

    // 2. Check VDOSRC and Timing
    switch(bDecType)
    {
        case SV_VD_TVD3D:
            if(_rTvd3dStatus.fgIs525)
            {
                u1Ret = VDOTYPE_NTSC;
            }
            else
            {
                u1Ret = VDOTYPE_PAL;
            }

            break;

        case SV_VD_YPBPR:
            if((_bHdtvTiming==MODE_525I) || (_bHdtvTiming==MODE_525I_OVERSAMPLE) ||
               (_bHdtvTiming==MODE_480P) || (_bHdtvTiming==MODE_480P_OVERSAMPLE))
            {
                u1Ret = VDOTYPE_NTSC;
            }
            else if((_bHdtvTiming==MODE_625I) || (_bHdtvTiming==MODE_625I_OVERSAMPLE) ||
                    (_bHdtvTiming==MODE_576P) || (_bHdtvTiming==MODE_576P_OVERSAMPLE))
            {
                u1Ret = VDOTYPE_PAL;
            }
			else if(_bHdtvTiming==MODE_720p_60)
			{
                u1Ret = VDOTYPE_720P;
            }
			else if(_bHdtvTiming==MODE_1080i)
			{
                u1Ret = VDOTYPE_1080I;
            }
            else
            {
                u1Ret = VDOTYPE_OTHER;
            }

            break;

        default:
            u1Ret = VDOTYPE_OTHER;
            break;
    }

    return (u1Ret);
}

#ifdef CC_SUPPORT_PIPELINE
UINT8 bDrvVideoGetTypeAVD(UINT8 bPath)
{
    UINT8 bDecType;
    UINT8 u1Ret;
    // 1. Check Channel Decoder
    bDecType = SV_VD_TVD3D;

    // 2. Check VDOSRC and Timing
    switch(bDecType)
    {
        case SV_VD_TVD3D:
            if(_rTvd3dStatus.fgIs525)
            {
                u1Ret = VDOTYPE_NTSC;
            }
            else
            {
                u1Ret = VDOTYPE_PAL;
            }

            break;

        case SV_VD_YPBPR:
            if((_bHdtvTiming==MODE_525I) || (_bHdtvTiming==MODE_525I_OVERSAMPLE) ||
               (_bHdtvTiming==MODE_480P) || (_bHdtvTiming==MODE_480P_OVERSAMPLE))
            {
                u1Ret = VDOTYPE_NTSC;
            }
            else if((_bHdtvTiming==MODE_625I) || (_bHdtvTiming==MODE_625I_OVERSAMPLE) ||
                    (_bHdtvTiming==MODE_576P) || (_bHdtvTiming==MODE_576P_OVERSAMPLE))
            {
                u1Ret = VDOTYPE_PAL;
            }
			else if(_bHdtvTiming==MODE_720p_60)
			{
                u1Ret = VDOTYPE_720P;
            }
			else if(_bHdtvTiming==MODE_1080i)
			{
                u1Ret = VDOTYPE_1080I;
            }
            else
            {
                u1Ret = VDOTYPE_OTHER;
            }

            break;

        default:
            u1Ret = VDOTYPE_OTHER;
            break;
    }

    return (u1Ret);
}
#endif
// *********************************************************************
// Function : void vDrvVideoAllocateDram(void)
// Description :
// Parameter :
// Return :
// *********************************************************************
void vDrvVideoAllocateDram(void)
{
}


// *********************************************************************
// The following are video call-back functions.
// *********************************************************************

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
void vDrvVideoConnect(UINT8 bPath, UINT8 bOnOff)
{
    UINT8 bType;
    bType = bGetVideoDecType(bPath);
#ifdef  SUPPORT_AV_COMP_AUTO

    if(bPath == SV_VP_MON)
    {
        //get monitor path decoder type
        bType = bGetDecodeType(_bMonMainNew);
    }

#endif
#ifndef CC_UP8032_ATV
    VERIFY(BIM_DisableIrq(VECTOR_VDOIN));
#else
    vDisableSysInt2(VDOIN_INT_EN);
#endif
#ifdef  SUPPORT_AV_COMP_AUTO

    if(bPath != SV_VP_MON)
#endif
    {
        vDrvSetPorchTune(bPath,SV_PORCHTUNE_DEC);
    }

    switch(bType)
    {
        case SV_VD_TVD3D:
#if SUPPORT_SCART

            /* considering SCART-RGB */
            //if(_bMainICIn == P_FB0)
            if(((bPath==SV_VP_MAIN) && fgIsMainScart()) ||
               ((bPath==SV_VP_PIP)&& fgIsPipScart()))
            {
                vDrvSetPorchTune(bPath, SV_PORCHTUNE_SCPOS);
            }

#endif
            vTvd3dConnect(bPath, bOnOff);
            break;

        case SV_VD_YPBPR:
            vHdtvConnect(bPath, bOnOff);
            break;

        case SV_VD_VGA:
            vVgaConnect(bPath, bOnOff);
#ifdef DRV_SUPPORT_EXTMJC_MODE7
            vDrvExtmjcRedoTwoChipConnection(SV_VP_MAIN,SV_TRUE);
#endif
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
			if(bPath==SV_VP_MAIN)
			{
				vDviConnect(bPath, bOnOff);
			}
			else
			{
#ifdef CC_OSD_ENCODE
				if(bGetICInputType(SV_VP_PIP)==P_OSDENCODE)
				{
					vCCIROSDConnect(bPath, bOnOff);
				}
				else
					
				{
					vCCIROSDConnect(bPath, 0);
					vDviConnect(bPath, bOnOff);
				}
#else
				vDviConnect(bPath, bOnOff);
#endif
			}
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

            // Add for 5371
        case SV_VD_MPEGHD:
            vDrvSetPorchTune(bPath,SV_PORCHTUNE_SCPOS);
            vMpegHdConnect(bPath, bOnOff);
            break;

        case SV_VD_MPEGSD:
            vDrvSetPorchTune(bPath,SV_PORCHTUNE_SCPOS);
            vMpegSdConnect(bPath, bOnOff);
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
			vPreProcConnect(bPath, bOnOff);
			break;
#endif

        default:
            break;
    }

#ifdef VGA_PDWNC_AFTER_ACON

	 if((bType != SV_VD_VGA) && (bType != SV_VD_YPBPR))
    {
        _PdwncSetupVGA(0);
	}
#endif

    if((bPath==SV_VP_MAIN)&&(bOnOff==ON))
    {
        vDrvSetDDDSInput(bType);
    }

#ifndef CC_UP8032_ATV
    VERIFY(BIM_EnableIrq(VECTOR_VDOIN));
#else
    vEnableSysInt2(VDOIN_INT_EN);
#endif
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
void vDrvVideoTrigModeDetect(UINT8 bPath)
{
    UINT8 bType;
    bType = bGetVideoDecType(bPath);

    if(bType == (UINT8)SV_VD_TVD3D)
    {
        vTvd3dTrigModeDet();
    }
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoInputWidthByDec(UINT8 bDecType)
{
    UINT16 wRet = 0;
    switch(bDecType)
    {
        case SV_VD_TVD3D:
            wRet = wTvdWidth();
            break;

        case SV_VD_YPBPR:
            wRet = wHdtvInputWidth();
            break;

        case SV_VD_VGA:
            wRet = wVgaInputWidth();
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            wRet = wDviInputWidth();
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
        #if (defined(CC_MT5882))
        //FIXME
            //wRet = u2MpegHdInputValidDataWidth(bPath);
        #else
            wRet = u2MpegHdInputValidDataWidth();
        #endif
            break;

        case SV_VD_MPEGSD:
        #if (defined(CC_MT5882))
        //FIXME
            //wRet = u2MpegSdInputValidDataWidth(bPath);
        #else
            wRet = u2MpegSdInputValidDataWidth();
        #endif
            break;
#endif
        
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
            wRet = u2PreProcInputWidth(bPath);
            break;
#endif

        default:
            break;
    }

    return wRet;
}


UINT16 wDrvVideoInputWidth(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wRet = 0;
    // irq_save(flags);
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_TVD3D:
            wRet = wTvdWidth();
            break;

        case SV_VD_YPBPR:
            wRet = wHdtvInputWidth();
            break;

        case SV_VD_VGA:
            wRet = wVgaInputWidth();
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            if(bPath==SV_VP_MAIN)
			{
				wRet = wDviInputWidth();
			}
			else
			{
#ifdef CC_OSD_ENCODE
				if(bGetICInputType(SV_VP_PIP)==P_OSDENCODE)
				{
					wRet = wCCIROSDInputWidth();
				}
				else
				{
					wRet = wDviInputWidth();
				}
#else
					wRet = wDviInputWidth();
#endif
			}
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
            #if (defined(CC_MT5882))
            wRet = u2MpegHdInputValidDataWidth(bPath);
            #else
            wRet = u2MpegHdInputValidDataWidth();
            #endif
            break;

        case SV_VD_MPEGSD:
            #if (defined(CC_MT5882))
            wRet = u2MpegSdInputValidDataWidth(bPath);
            #else
            wRet = u2MpegSdInputValidDataWidth();
            #endif
            break;
#endif
		
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
	   	    wRet = u2PreProcInputWidth(bPath);
            break;
#endif

        default:
            break;
    }

    // irq_restore(flags);
    return wRet;
}

UINT8 bDrvVideoIsNonStandardSignal(UINT8 bPath)
{
    UINT8 ret = SV_FALSE;
    UINT8 bType;
    RTvdNSTDStatus rStatus;
    RHDTVNSTDStatus rHDTVStatus;
    bType = bGetVideoDecType(bPath);

    if(bType == SV_VD_TVD3D)
    {
        vTvd3dGetNSTDStatus(&rStatus);

        if(rStatus.fgIsNSTD == SV_TRUE)
        {
            ret = SV_TRUE;   //non-standard signal
        }
    }
    else if(bType == SV_VD_YPBPR)
    {
        vHDTVGetNSTDStatus(&rHDTVStatus);

        if(rHDTVStatus.fgIsNSTD == SV_TRUE)
        {
            ret = SV_TRUE;  //non-standard signal
        }
    }

    return ret;
}
// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoInputDEWidth(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wRet = 0;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_DVI:
            wRet = wDviGetAvWidth();
            break;

        case SV_VD_YPBPR:
        case SV_VD_VGA:
            wRet = wHdtvDEInputWidth();
            break;

        default:
            wRet = wDrvVideoInputWidth(bPath);
            break;
    }

    return wRet;
}

#if !defined(CC_FPGA_SUPPORT)
// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************

UINT16 wDrvVideoInputHeightByDec(UINT8 bDecType)
{
    UINT16 wRet = 0;
    switch(bDecType)
    {
        case SV_VD_TVD3D:
            wRet = wTvd3dHeight();
            break;

        case SV_VD_YPBPR:
            wRet = wHdtvInputHeight();
            break;

        case SV_VD_VGA:
            wRet = wVgaInputHeight();
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            wRet = wDviInputHeight();

            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
        #if (defined(CC_MT5882))
        //FIXE
            //wRet = u2MpegHdInputHeight(bPath);
        #else
            wRet = u2MpegHdInputHeight();
        #endif
            break;

        case SV_VD_MPEGSD:
        #if (defined(CC_MT5882))
        //FIXME
            //wRet = u2MpegSdInputHeight(bPath);
        #else
            wRet = u2MpegSdInputHeight();
        #endif
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
            wRet = u2PreProcInputHeight(bPath);
            break;
#endif

        default:
            break;
    }

    return wRet;
}


UINT16 wDrvVideoInputHeight(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wRet = 0;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_TVD3D:
            wRet = wTvd3dHeight();
            break;

        case SV_VD_YPBPR:
            wRet = wHdtvInputHeight();
            break;

        case SV_VD_VGA:
            wRet = wVgaInputHeight();
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            if(bPath==SV_VP_MAIN)
			{
				wRet = wDviInputHeight();
			}
			else
			{
#ifdef CC_OSD_ENCODE
				if(bGetICInputType(SV_VP_PIP)==P_OSDENCODE)
				{
					wRet = wCCIROSDInputHeight();
				}
				else
				{
					wRet = wDviInputHeight();
				}
#else
				wRet = wDviInputHeight();
#endif
			}
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
            #if (defined(CC_MT5882))
            wRet = u2MpegHdInputHeight(bPath);
            #else
            wRet = u2MpegHdInputHeight();
            #endif
            break;

        case SV_VD_MPEGSD:
            #if (defined(CC_MT5882))
            wRet = u2MpegSdInputHeight(bPath);
            #else
            wRet = u2MpegSdInputHeight();
            #endif
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
	   	    wRet = u2PreProcInputHeight(bPath);
            break;
#endif

        default:
            break;
    }

    return wRet;
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoGetRefreshRateByDec(UINT8 bDecType)
{
    UINT8 bRet = 0;
    switch(bDecType)
    {
        case SV_VD_TVD3D:
            bRet = bTvd3dRefreshRate();
            break;

        case SV_VD_YPBPR:
            bRet = bHdtvRefreshRate();
            break;

        case SV_VD_VGA:
            bRet = bVgaRefreshRate();
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            bRet = bDviRefreshRate();
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
        #if defined(CC_MT5882)
        //FIXME
            //bRet = u1MpegHdRefreshRate(bPath);
        #else
            bRet = u1MpegHdRefreshRate();
        #endif
            break;

        case SV_VD_MPEGSD:
        #if defined(CC_MT5882)
        //FIXME
            //bRet = u1MpegSdRefreshRate(bPath);
        #else
            bRet = u1MpegSdRefreshRate();
        #endif
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
            bRet = bPreProcRefreshRate();
            break;
#endif

        default:
            break;
    }

    return bRet;
}


UINT8 bDrvVideoGetRefreshRate(UINT8 bPath)
{
    UINT8 bType;
    UINT8 bRet = 0;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_TVD3D:
            bRet = bTvd3dRefreshRate();
            break;

        case SV_VD_YPBPR:
            bRet = bHdtvRefreshRate();
            break;

        case SV_VD_VGA:
            bRet = bVgaRefreshRate();
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            if(bPath==SV_VP_MAIN)
			{
				bRet = bDviRefreshRate();
			}
			else
			{
#ifdef CC_OSD_ENCODE
				if(bGetICInputType(SV_VP_PIP)==P_OSDENCODE)
				{
					bRet = bCCIROSDRefreshRate();
				}
				else
				{
					bRet = bDviRefreshRate();
				}
#else
				bRet = bDviRefreshRate();
#endif
			}	
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
            #if defined(CC_MT5882)
            bRet = u1MpegHdRefreshRate(bPath);
            #else
            bRet = u1MpegHdRefreshRate();
            #endif
            break;

        case SV_VD_MPEGSD:
            #if defined(CC_MT5882)
            bRet = u1MpegSdRefreshRate(bPath);
            #else
            bRet = u1MpegSdRefreshRate();
            #endif
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
			bRet = bPreProcRefreshRate();
			break;
#endif

        default:
            break;
    }

    return bRet;
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoIsSrcInterlaceByDec(UINT8 bDecType)
{
    UINT8 bRet = 0;

    switch(bDecType)
    {
        case SV_VD_TVD3D:
            bRet = SV_TRUE;
            break;

        case SV_VD_YPBPR:
            bRet = bHdtvInterlace();
            break;

        case SV_VD_VGA:
            bRet = bVgaInterlace();
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            bRet = bDviInterlace();
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
			#if defined(CC_MT5882)
            //bRet = u1MpegHdInterlace(bPath);
			#else
			bRet = u1MpegHdInterlace();
			#endif
            break;

        case SV_VD_MPEGSD:
			#if defined(CC_MT5882)
            //bRet = u1MpegSdInterlace(bPath);
			#else
			bRet = u1MpegSdInterlace();
			#endif
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
            bRet = (UINT8)_fgPreProcInterlace ;
            break;
#endif 

        default:
            break;
    }

    return bRet;
}


UINT8 bDrvVideoIsSrcInterlace(UINT8 bPath)
{
    UINT8 bType;
    UINT8 bRet = 0;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_TVD3D:
            bRet = SV_TRUE;
            break;

        case SV_VD_YPBPR:
            bRet = bHdtvInterlace();
            break;

        case SV_VD_VGA:
            bRet = bVgaInterlace();
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            if(bPath==SV_VP_MAIN)
			{
				bRet = bDviInterlace();
			}
			else
			{
#ifdef CC_OSD_ENCODE
				if(bGetICInputType(SV_VP_PIP)==P_OSDENCODE)
				{
					bRet = bCCIROSDInterlace();
				}
				else
				{
					bRet = bDviInterlace();
				}
#else
				bRet = bDviInterlace();
#endif
			}		
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
			#if defined(CC_MT5882)
			bRet = u1MpegHdInterlace(bPath);
			#else
            bRet = u1MpegHdInterlace();
			#endif
            break;

        case SV_VD_MPEGSD:
			#if defined(CC_MT5882)
			bRet = u1MpegSdInterlace(bPath);
			#else
            bRet = u1MpegSdInterlace();
			#endif
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
            bRet = (UINT8)_fgPreProcInterlace ;
            break;
#endif 

        default:
            break;
    }

    return bRet;
}
#endif // CC_FPGA_SUPPORT

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoGetVPorch(UINT8 bPath)
{
    UINT8 bType;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_YPBPR:
            return wHdtvVPorch(bPath);

        case SV_VD_VGA:
            return Displaymode_delay; //Modify for Disaplymode delay

        case SV_VD_DVI:
            return 0;

        default:
            return 0;
    }
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoGetHPorch(UINT8 bPath)
{
    UINT8 bType;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
#ifdef MT5360B_WA4

        case SV_VD_VGA:
            return MT5360B_WA4_DLY1;

        case SV_VD_DVI:
            return MT5360B_WA4_DLY1;
#else

        case SV_VD_YPBPR:
            return wHdtvHPorch(bPath);
#endif

        default:
            return 0;
    }
}


UINT8 bDrvGetPorchTune(UINT8 bPath)
{
    if(bPath <SV_VP_NA)
    {
        return bDrvPorchTune[bPath];
    }
    else
        return 0;
}

void vDrvSetPorchTune(UINT8 bPath, UINT8 bModemsk)
{
    if(bPath <SV_VP_NA)
    {
        bDrvPorchTune[bPath]=bModemsk;
    }
}

// *********************************************************************
// Function : For dynamic backlight get status with different criteria (Snow screen is no signal)
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoSignalStatus_DBL(UINT8 bPath)
{
    UINT8 bRet = (UINT8)SV_VDO_NOSIGNAL;

    //irq_save(flags);
    if(bGetVideoDecType(bPath) == SV_VD_TVD3D)
    {
        bRet = _rTvd3dStatus.bSigStatus;
    }
    else
    {
        bRet = bDrvVideoSignalStatus(bPath);
    }

    return bRet;
}

#if 1//#ifdef CC_FPGA_SUPPORT
// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoSignalStatusByDec(UINT8 bDecType)
{
    UINT8 bRet = (UINT8)SV_VDO_NOSIGNAL;
    switch(bDecType)
    {
        case SV_VD_TVD3D:
            bRet = bTvd3dSignalStatus();
            break;

        case SV_VD_YPBPR:
            bRet = bHdtvSigStatus();
            break;

        case SV_VD_VGA:
            bRet = bVgaSigStatus();
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
			bRet = bDviSigStatus();
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
        //FIXME
            //bRet = u1MpegHdSigStatus(bPath);
            break;

        case SV_VD_MPEGSD:
        //FIXME
            //bRet = u1MpegSdSigStatus(bPath);
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
			bRet = (UINT8)SV_VDO_STABLE;
			break;
#endif

        default:
            break;
    }

    //irq_restore(flags);
    return bRet;
}
UINT8 bDrvVideoSignalStatus(UINT8 bPath)
{
    UINT8 bType;
    UINT8 bRet = (UINT8)SV_VDO_NOSIGNAL;
    //irq_save(flags);
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_TVD3D:
            bRet = bTvd3dSignalStatus();
            break;

        case SV_VD_YPBPR:
            bRet = bHdtvSigStatus();
            break;

        case SV_VD_VGA:
            bRet = bVgaSigStatus();
            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            if(bPath==SV_VP_MAIN)
			{
				bRet = bDviSigStatus();
			}
			else
			{
#ifdef CC_OSD_ENCODE
				if(bGetICInputType(SV_VP_PIP)==P_OSDENCODE)
				{
					bRet = bCCIROSDSigStatus();
				}
				else
				{
					bRet = bDviSigStatus();
				}
#else
				bRet = bDviSigStatus();
#endif
			}
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
            bRet = u1MpegHdSigStatus(bPath);
            break;

        case SV_VD_MPEGSD:
            bRet = u1MpegSdSigStatus(bPath);
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
			bRet = (UINT8)SV_VDO_STABLE;
			break;
#endif

        default:
            break;
    }

    //irq_restore(flags);
    return bRet;
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT16 wDrvVideoGetHTotalByDec(UINT8 bDecType)
{
    UINT16 wHTotal = 0;
    switch(bDecType)
    {
        case SV_VD_TVD3D:
            wHTotal =  wTvdHTotal();
            break;
        case SV_VD_YPBPR:
        case SV_VD_VGA:
        {
            UINT8 u1Timing ;
            u1Timing = (bDecType == SV_VD_YPBPR)?_bHdtvTiming :_bVgaTiming ;
            
            if(u1Timing < MAX_TIMING_FORMAT)
            {              
                wHTotal =  Get_VGAMODE_IHTOTAL(u1Timing);
                if(bHdtvCheckCenEnable())
                {
                    wHTotal = wHTotal / 2 ;
                }
            }
            break;
        }
#if SUPPORT_DVI

        case SV_VD_DVI:
            wHTotal = wDviInputHTotal();
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
        #if defined(CC_MT5882)
        //FIXME
            //wHTotal = u2MpegHdHTotal(bPath);
        #else
            wHTotal = u2MpegHdHTotal();
        #endif
            break;

        case SV_VD_MPEGSD:
        #if defined(CC_MT5882)
        //FIXME
            //wHTotal = u2MpegSdHTotal(bPath);
        #else
            wHTotal = u2MpegSdHTotal();
        #endif
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
       case SV_VD_PREPROC:
            wHTotal = u2PreProcHTotal(bPath);
            break;
#endif
        default:
            break;
    }

    return wHTotal;
}


UINT16 wDrvVideoGetHTotal(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wHTotal = 0;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_TVD3D:
            wHTotal =  wTvdHTotal();
            break;
        case SV_VD_YPBPR:
        case SV_VD_VGA:
        {
            UINT8 u1Timing ;
            u1Timing = (bType == SV_VD_YPBPR)?_bHdtvTiming :_bVgaTiming ;
            
            if(u1Timing < MAX_TIMING_FORMAT)
            {
                //wHTotal = vDrvCLKINGetHtotal();                    
                wHTotal =  Get_VGAMODE_IHTOTAL(u1Timing);
            
                if(bHdtvCheckCenEnable())
                {
                    wHTotal = wHTotal / 2 ;
                }
            }
            break;
        }
#if SUPPORT_DVI

        case SV_VD_DVI:
            if(bPath==SV_VP_MAIN)
			{
				wHTotal = wDviInputHTotal();
			}
			else
			{
#ifdef CC_OSD_ENCODE
				if(bGetICInputType(SV_VP_PIP)==P_OSDENCODE)
				{
					wHTotal = wCCIROSDInputHTotal();
				}
				else
				{
					wHTotal = wDviInputHTotal();
				}
#else
				wHTotal = wDviInputHTotal();
#endif
			}	
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
            #if defined(CC_MT5882)
            wHTotal = u2MpegHdHTotal(bPath);
            #else
            wHTotal = u2MpegHdHTotal();
            #endif
            break;

        case SV_VD_MPEGSD:
            #if defined(CC_MT5882)
            wHTotal = u2MpegSdHTotal(bPath);
            #else
            wHTotal = u2MpegSdHTotal();
            #endif
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
       case SV_VD_PREPROC:
	   	    wHTotal = u2PreProcHTotal(bPath);
            break;
#endif
        default:
            break;
    }

    return wHTotal;
}

// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
#define VGA_YPBPR_DECODER_VTOTAL     1

UINT16 wDrvVideoGetVTotalByDec(UINT8 bDecType)
{
    UINT16 wVTotal = 0;

    switch(bDecType)
    {
        case SV_VD_TVD3D:
            wVTotal =wTvdVTotal();
            break;

        case SV_VD_YPBPR:
            if(_bHdtvTiming < MAX_TIMING_FORMAT)
            {
                wVTotal =  Get_VGAMODE_IVTOTAL(_bHdtvTiming);
            }

            break;

        case SV_VD_VGA:
            if(_bVgaTiming < MAX_TIMING_FORMAT)
            {
#if VGA_YPBPR_DECODER_VTOTAL

                //wVTotal = Get_VGAMODE_IVTOTAL(_bVgaTiming);
                if(fgIsVideoTiming(_bVgaTiming))
                {
                    wVTotal = Get_VGAMODE_IVTOTAL(_bVgaTiming);
                }
                else
                {
                    wVTotal = _wSP0StableVtotal;
                }

#else

                // get from measured vtotal
                if(fgIsVideoTiming(_bVgaTiming))
                {
                    wVTotal = Get_VGAMODE_IVTOTAL(_bVgaTiming);
                }
                else
                {
                    if(bPath ==  SV_VP_MAIN)
                    {
                        wVTotal = IO32ReadFldAlign(SCPIP_DS1_RO_00, DS1_RO_00_DS1_INVLEN_1);
                    }
                    else // SuB
                    {
                        wVTotal = IO32ReadFldAlign(SCPIP_DS2_RO_00, DS2_RO_00_DS1_INVLEN_2);
                    }
                }

#endif
            }

            break;
#if SUPPORT_DVI
        case SV_VD_DVI:
			wVTotal=wDviInputVTotal();				
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
            #if defined(CC_MT5882)
            //FIXME
            //wVTotal = u2MpegHdVTotal(bPath);
            #else
            wVTotal = u2MpegHdVTotal();
            #endif
            break;

        case SV_VD_MPEGSD:
            #if defined(CC_MT5882)
            //FIXME
            //wVTotal = u2MpegSdVTotal(bPath);
            #else
            wVTotal = u2MpegSdVTotal();
            #endif
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
	   	    wVTotal = u2PreProcVTotal(bPath);
			break;
#endif

        default:
            break;
    }

    return wVTotal;
}

UINT16 wDrvVideoGetVTotal(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wVTotal = 0;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_TVD3D:
            wVTotal =wTvdVTotal();
            break;

        case SV_VD_YPBPR:
            if(_bHdtvTiming < MAX_TIMING_FORMAT)
            {
                wVTotal =  Get_VGAMODE_IVTOTAL(_bHdtvTiming);
            }

            break;

        case SV_VD_VGA:
            if(_bVgaTiming < MAX_TIMING_FORMAT)
            {
#if VGA_YPBPR_DECODER_VTOTAL

                //wVTotal = Get_VGAMODE_IVTOTAL(_bVgaTiming);
                if(fgIsVideoTiming(_bVgaTiming))
                {
                    wVTotal = Get_VGAMODE_IVTOTAL(_bVgaTiming);
                }
                else
                {
                    wVTotal = _wSP0StableVtotal;
                }

#else

                // get from measured vtotal
                if(fgIsVideoTiming(_bVgaTiming))
                {
                    wVTotal = Get_VGAMODE_IVTOTAL(_bVgaTiming);
                }
                else
                {
                    if(bPath ==  SV_VP_MAIN)
                    {
                        wVTotal = IO32ReadFldAlign(SCPIP_DS1_RO_00, DS1_RO_00_DS1_INVLEN_1);
                    }
                    else // SuB
                    {
                        wVTotal = IO32ReadFldAlign(SCPIP_DS2_RO_00, DS2_RO_00_DS1_INVLEN_2);
                    }
                }

#endif
            }

            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            if(bPath==SV_VP_MAIN)
			{
				wVTotal=wDviInputVTotal();				
			}
			else
			{
#ifdef CC_OSD_ENCODE
				if(bGetICInputType(SV_VP_PIP)==P_OSDENCODE)
				{
					wVTotal=wCCIROSDInputVTotal();	
				}
				else
				{
					wVTotal=wDviInputVTotal();	
				}
#else
				wVTotal=wDviInputVTotal();	
#endif
			}
#if 0

            if(_bDviTiming == MODE_DE_MODE)
            {
                wVTotal = _wDviVTotal ;
            }
            else if(_bDviTiming < MAX_TIMING_FORMAT)
            {
                wVTotal =  Get_VGAMODE_IVTOTAL(_bDviTiming);
            }

#endif
            break;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
            #if defined(CC_MT5882)
            wVTotal = u2MpegHdVTotal(bPath);
            #else
            wVTotal = u2MpegHdVTotal();
            #endif
            break;

        case SV_VD_MPEGSD:
            #if defined(CC_MT5882)
            wVTotal = u2MpegSdVTotal(bPath);
            #else
            wVTotal = u2MpegSdVTotal();
            #endif
            break;
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
	   	    wVTotal = u2PreProcVTotal(bPath);
			break;
#endif

        default:
            break;
    }

    return wVTotal;
}
#endif // CC_FPGA_SUPPORT

#if 0
// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoIsSrc1080i(UINT8 bPath)
{
    UINT8 bType;
    UNUSED(bPath);
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_YPBPR:
            if((_bHdtvTiming == MODE_1080i) || (_bHdtvTiming == MODE_1080i_50))
            {
                return TRUE;
            }

            break;

        case SV_VD_VGA:
            if((_bVgaTiming == MODE_1080i) || (_bVgaTiming == MODE_1080i_50))
            {
                return TRUE;
            }

            break;
#if SUPPORT_DVI

        case SV_VD_DVI:
            if((_bDviTiming == MODE_1080i) || (_bDviTiming == MODE_1080i_50))
            {
                return TRUE;
            }

            break;
#endif

        default:
            break;
    }

    return FALSE;
}
#endif
// *********************************************************************
// Function :
// Description :
// Parameter :
// Return    :
// *********************************************************************
UINT8 bDrvVideoGetTiming(UINT8 bPath)
{
    UINT8 bType;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_TVD3D:
            if(_rTvd3dStatus.fgIs525)
            {
                return MODE_525I;
            }
            else
            {
                return MODE_625I;
            }

        case SV_VD_YPBPR:
            return _bHdtvTiming;

        case SV_VD_VGA:
            return _bVgaTiming;
#if SUPPORT_DVI

        case SV_VD_DVI:
            return _bDviTiming;
#endif
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
            #if defined(CC_MT5882)
            return u1MpegHdTiming(bPath);
            #else
            return u1MpegHdTiming();
            #endif

        case SV_VD_MPEGSD:
            #if defined(CC_MT5882)
            return u1MpegSdTiming(bPath);
            #else
            return u1MpegSdTiming();
#endif
#endif
#ifdef CC_SUPPORT_PREPROC_TIMING
        case SV_VD_PREPROC:
	   	    return _bPreProcTiming;
			break;
#endif

        default:
            break;
    }

    return MODE_NOSIGNAL;
}

const CHAR *strDrvVideoGetTimingString(UINT8 bTiming)
{
    switch(bTiming)
    {
        case MODE_525I_OVERSAMPLE:
            return "MODE_525I_OVERSAMPLE";

        case MODE_625I_OVERSAMPLE:
            return "MODE_625I_OVERSAMPLE";

        case MODE_480P_OVERSAMPLE:
            return "MODE_480P_OVERSAMPLE";

        case MODE_576P_OVERSAMPLE:
            return "MODE_576P_OVERSAMPLE";

        case MODE_720p_50:
            return "MODE_720p_50";

        case MODE_720p_60:
            return "MODE_720p_60";

        case MODE_1080i_48:
            return "MODE_1080i_48";

        case MODE_1080i_50:
            return "MODE_1080i_50";

        case MODE_1080i:
            return "MODE_1080i";

        case MODE_1080p_24:
            return "MODE_1080p_24";

        case MODE_1080p_25:
            return "MODE_1080p_25";

        case MODE_1080p_30:
            return "MODE_1080p_30";

        case MODE_1080p_50:
            return "MODE_1080p_50";

        case MODE_1080p_60:
            return "MODE_1080p_60";

        case MODE_525I:
            return "MODE_525I";

        case MODE_625I:
            return "MODE_625I";

        case MODE_480P:
            return "MODE_480P";

        case MODE_576P:
            return "MODE_576P";

        case MODE_720p_24:
            return "MODE_720p_24";

        case MODE_720p_25:
            return "MODE_720p_25";

        case MODE_720p_30:
            return "MODE_720p_30";

        case MODE_240P:
            return "MODE_240P";

        case MODE_540P:
            return "MODE_540P";

        case MODE_480P_24:
            return "MODE_480P_24";

        case MODE_480P_30:
            return "MODE_480P_30";

        case MODE_576P_25:
            return "MODE_576P_25";

        case MODE_HDMI_640_480P:
            return "MODE_HDMI_640_480P";

        case MODE_HDMI_720p_24:
            return "MODE_HDMI_720p_24";

        case MODE_3D_720p_50_FP:
            return "MODE_3D_720p_50_FP";

        case MODE_3D_720p_60_FP:
            return "MODE_3D_720p_60_FP";

        case MODE_3D_1080p_24_FP:
            return "MODE_3D_1080p_24_FP";

        case MODE_3D_1080I_60_FP:
            return "MODE_3D_1080I_60_FP";

        case MODE_3D_480p_60_FP:
            return "MODE_3D_480p_60_FP";

        case MODE_3D_576p_50_FP:
            return "MODE_3D_576p_50_FP";

        case MODE_3D_720p_24_FP:
            return "MODE_3D_720p_24_FP";

        case MODE_3D_720p_30_FP:
            return "MODE_3D_720p_30_FP";

        case MODE_3D_1080p_30_FP:
            return "MODE_3D_1080p_30_FP";

        case MODE_3D_480I_60_FP:
            return "MODE_3D_480I_60_FP";

        case MODE_3D_576I_60_FP:
            return "MODE_3D_576I_60_FP";

        case MODE_3D_1080I_50_FP:
            return "MODE_3D_1080I_50_FP";

        case MODE_3D_1080p_50_FP:
            return "MODE_3D_1080p_50_FP";

        case MODE_3D_1080p_60_FP:
            return "MODE_3D_1080p_60_FP";

        case MODE_3D_1650_750_60_FP:
            return "MODE_3D_1650_750_60_FP";

        case MODE_3D_1650_1500_30_FP:
            return "MODE_3D_1650_1500_30_FP";

        case MODE_3D_1080p_24_SBS_FULL:
            return "MODE_3D_1080p_24_SBS_FULL";

        case MODE_3D_1080p_30_SBS_FULL:
            return "MODE_3D_1080p_30_SBS_FULL";

        case MODE_3D_1080I_50_SBS_FULL:
            return "MODE_3D_1080I_50_SBS_FULL";

        case MODE_3D_1080I_60_SBS_FULL:
            return "MODE_3D_1080I_60_SBS_FULL";

        case MODE_3D_720p_24_SBS_FULL:
            return "MODE_3D_720p_24_SBS_FULL";

        case MODE_3D_720p_30_SBS_FULL:
            return "MODE_3D_720p_30_SBS_FULL";

        case MODE_3D_720p_50_SBS_FULL:
            return "MODE_3D_720p_50_SBS_FULL";

        case MODE_3D_720p_60_SBS_FULL:
            return "MODE_3D_720p_60_SBS_FULL";

        case MODE_3D_480p_60_SBS_FULL:
            return "MODE_3D_480p_60_SBS_FULL";

        case MODE_3D_576p_50_SBS_FULL:
            return "MODE_3D_576p_50_SBS_FULL";

        case MODE_3D_480I_60_SBS_FULL:
            return "MODE_3D_480I_60_SBS_FULL";

        case MODE_3D_576I_50_SBS_FULL:
            return "MODE_3D_576I_50_SBS_FULL";
        case MODE_3840_2160P_15:
        case MODE_3840_2160P_24:
        case MODE_3840_2160P_25:
        case MODE_3840_2160P_30:  
        	return "MODE_2160P" ;

        case MODE_MAX:
        default:
            return "Not Support OR No Signal";
    }
}

// add for checking if main input source is TV(ATV,DTV) or not. Return 1 for TV source, 0 for others.
UINT8 bDrvVideoIsMainSrcTV(void)
{
    UINT8 bAppType = VDP_GetPlayMode(SV_VP_MAIN);
    UINT8 bSignalType = bGetSignalType(SV_VP_MAIN);

    //Printf("----MainSrc: App-%x, sig-%x\n",bAppType,bSignalType);
    if(SV_ST_TV==bSignalType)
    {
        return 1;
    }
    else if(SV_ST_MPEG==bSignalType)
    {
        if(bAppType==FBM_FBG_DTV_MODE)
            return 1;
        else
            return 0;
    }
    else
    {
        return 0;
    }
}

extern MMAppInfo sMMAppInfo;
UINT8 bDrvVideoGetSourceTypeTiming(UINT8 bPath) reentrant
{
    //TBD
    //    UINT8 bAppType = sMMAppInfo.u1AppType;
    UINT8 bAppType = VDP_GetPlayMode(bPath);
    UINT8 bVideoTiming = bDrvVideoGetTiming(bPath);
    UINT8 bSignalType = bGetSignalType(bPath);

#if SUPPORT_SCART

    if((bPath == SV_VP_MAIN && VSS_MAJOR(_bSrcMainNew) == VSS_SCART) ||
    (bPath == SV_VP_PIP && VSS_MAJOR(_bSrcSubNew) == VSS_SCART))
    {
        switch(bDrvGetScartInputMode())
        {
            case SV_SCART_RGB:
                return SOURCE_TYPE_TIMING_SCART_RGB;

            case SV_SCART_SV:
                switch((UINT8)bHwTvdMode())   // penggang add uint8
                {
                    case(UINT8)SV_CS_SECAM:
                        return SOURCE_TYPE_TIMING_SCART_SV_SECAM;

                    case(UINT8)SV_CS_NTSC358:
                        return SOURCE_TYPE_TIMING_SCART_SV_NTSC_358;

                    case(UINT8)SV_CS_NTSC443:
                        return SOURCE_TYPE_TIMING_SCART_SV_NTSC_443;

                    case(UINT8)SV_CS_PAL_M:
                        return SOURCE_TYPE_TIMING_SCART_SV_PAL_M;

                    case(UINT8)SV_CS_PAL_N:
                        return SOURCE_TYPE_TIMING_SCART_SV_PAL_N;

                    case(UINT8)SV_CS_PAL_60:
                        return SOURCE_TYPE_TIMING_SCART_SV_PAL_60;

                    case(UINT8)SV_CS_PAL:
                        return SOURCE_TYPE_TIMING_SCART_SV_PAL;

                    default:
                        return SOURCE_TYPE_TIMING_MAX;
                }

            default: //AUTO, MIX, AV
            case SV_SCART_CVBS:
                switch((UINT8)bHwTvdMode()) // penggang add uint8
                {
                    case(UINT8)SV_CS_SECAM:
                        return SOURCE_TYPE_TIMING_SCART_AV_SECAM;

                    case(UINT8)SV_CS_NTSC358:
                        return SOURCE_TYPE_TIMING_SCART_AV_NTSC_358;

                    case(UINT8)SV_CS_NTSC443:
                        return SOURCE_TYPE_TIMING_SCART_AV_NTSC_443;

                    case(UINT8)SV_CS_PAL_M:
                        return SOURCE_TYPE_TIMING_SCART_AV_PAL_M;

                    case(UINT8)SV_CS_PAL_N:
                        return SOURCE_TYPE_TIMING_SCART_AV_PAL_N;

                    case(UINT8)SV_CS_PAL_60:
                        return SOURCE_TYPE_TIMING_SCART_AV_PAL_60;

                    case(UINT8)SV_CS_PAL:
                        return SOURCE_TYPE_TIMING_SCART_AV_PAL;

                    default:
                        return SOURCE_TYPE_TIMING_MAX;
                }
        }
    }

#endif


    switch(bSignalType)
    {
        case SV_ST_VGA:
            return (bIsScalerInput444(bPath)?
                    SOURCE_TYPE_TIMING_PC_RGB : SOURCE_TYPE_TIMING_PC_YCBCR);

        case SV_ST_TV:
            if(!fgHwTvdVPres())
            {
                return SOURCE_TYPE_TIMING_MAX;
            }

            switch((UINT8)bHwTvdMode())
            {
                case(UINT8)SV_CS_SECAM:
                    return SOURCE_TYPE_TIMING_SECAM_RFTV;

                case(UINT8)SV_CS_NTSC358:
                    return SOURCE_TYPE_TIMING_RFTV_NTSC_358;

                case(UINT8)SV_CS_NTSC443:
                    return SOURCE_TYPE_TIMING_RFTV_NTSC_443;

                case(UINT8)SV_CS_PAL_M:
                    return SOURCE_TYPE_TIMING_RFTV_PAL_M;

                case(UINT8)SV_CS_PAL_N:
                    return SOURCE_TYPE_TIMING_RFTV_PAL_N;

                case(UINT8)SV_CS_PAL_60:
                    return SOURCE_TYPE_TIMING_RFTV_PAL_60;

                case(UINT8)SV_CS_PAL:
                    return SOURCE_TYPE_TIMING_RFTV_PAL;

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case SV_ST_SV:
            if(!((UINT8)fgHwTvdVPres()))
            {
                return SOURCE_TYPE_TIMING_MAX;
            }

            switch((UINT8)bHwTvdMode())
            {
                case(UINT8)SV_CS_SECAM:
                    return SOURCE_TYPE_TIMING_SECAM_SV;

                case(UINT8)SV_CS_NTSC358:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_SV_NTSC_358_OS : SOURCE_TYPE_TIMING_SV_NTSC_358);

                case(UINT8)SV_CS_NTSC443:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_SV_NTSC_443_OS : SOURCE_TYPE_TIMING_SV_NTSC_443);

                case(UINT8)SV_CS_PAL_M:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_SV_PAL_M_OS : SOURCE_TYPE_TIMING_SV_PAL_M);

                case(UINT8)SV_CS_PAL_N:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_SV_PAL_N_OS : SOURCE_TYPE_TIMING_SV_PAL_N);

                case(UINT8)SV_CS_PAL_60:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_SV_PAL_60_OS : SOURCE_TYPE_TIMING_SV_PAL_60);

                case(UINT8)SV_CS_PAL:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_SV_PAL_OS : SOURCE_TYPE_TIMING_SV_PAL);

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case SV_ST_AV:
            if(!((UINT8)fgHwTvdVPres()))
            {
                return SOURCE_TYPE_TIMING_MAX;
            }

            switch((UINT8)bHwTvdMode())
            {
                case(UINT8)SV_CS_SECAM:
                    return SOURCE_TYPE_TIMING_SECAM_CVBS;

                case(UINT8)SV_CS_NTSC358:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_CVBS_NTSC_358_OS : SOURCE_TYPE_TIMING_CVBS_NTSC_358);

                case(UINT8)SV_CS_NTSC443:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_CVBS_NTSC_443_OS : SOURCE_TYPE_TIMING_CVBS_NTSC_443);

                case(UINT8)SV_CS_PAL_M:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_CVBS_PAL_M_OS : SOURCE_TYPE_TIMING_CVBS_PAL_M);

                case(UINT8)SV_CS_PAL_N:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_CVBS_PAL_N_OS : SOURCE_TYPE_TIMING_CVBS_PAL_N);

                case(UINT8)SV_CS_PAL_60:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_CVBS_PAL_60_OS : SOURCE_TYPE_TIMING_CVBS_PAL_60);

                case(UINT8)SV_CS_PAL:
                    return (vTvdGetOverSampleEnable()?
                            SOURCE_TYPE_TIMING_CVBS_PAL_OS : SOURCE_TYPE_TIMING_CVBS_PAL);

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        default:
            break;
    }

    switch(bVideoTiming)
    {
        case MODE_1080i:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_1080I_60;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_1080IP_HD;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_1080I_60;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_1080IP_HD);
                    }

                case(UINT8)SV_ST_YP:
                    return SOURCE_TYPE_TIMING_YPBPR_1080I_60;

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_1080i_48:
        case MODE_1080i_50:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_1080I_50;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_1080IP_HD;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_1080I_50;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_1080IP_HD);
                    }

                case(UINT8)SV_ST_YP:
                    return SOURCE_TYPE_TIMING_YPBPR_1080I_50;

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_1080p_25:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_1080P_25;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_1080IP_HD;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_1080P_25;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_1080IP_HD);
                    }

                case(UINT8)SV_ST_YP:
                    return SOURCE_TYPE_TIMING_YPBPR_1080P_25;

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_1080p_50:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_1080P_50;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_1080IP_HD;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_1080P_50;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_1080IP_HD);
                    }

                case(UINT8)SV_ST_YP:
                    return SOURCE_TYPE_TIMING_YPBPR_1080P_50;

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_1080p_24:
        case MODE_1080p_30:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_1080P_24;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_1080IP_HD;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_1080P_24;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_1080IP_HD);
                    }

                case(UINT8)SV_ST_YP:
                    return SOURCE_TYPE_TIMING_YPBPR_1080P_24;

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_1080p_60:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_1080P_60;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_1080IP_HD;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_1080P_60;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_1080IP_HD);
                    }

                case(UINT8)SV_ST_YP:
                    return SOURCE_TYPE_TIMING_YPBPR_1080P_60;

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_720p_25:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_720P_25;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_720P_HD;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_720P_25;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_720P_HD);
                    }

                case(UINT8)SV_ST_YP:
                    return SOURCE_TYPE_TIMING_YPBPR_720P_25;

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_720p_50:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_720P_50;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_720P_HD;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_720P_50;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_720P_HD);
                    }

                case(UINT8)SV_ST_YP:
                    return SOURCE_TYPE_TIMING_YPBPR_720P_50;

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_720p_24:
        case MODE_720p_30:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_720P_24;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_720P_HD;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_720P_24;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_720P_HD);
                    }

                case(UINT8)SV_ST_YP:
                    return SOURCE_TYPE_TIMING_YPBPR_720P_24;

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_720p_60:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_720P_60;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_720P_HD;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_720P_60;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_720P_HD);
                    }

                case(UINT8)SV_ST_YP:
                    return SOURCE_TYPE_TIMING_YPBPR_720P_60;

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_625I:
        case MODE_625I_OVERSAMPLE:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_SD_I_50;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_SD_I;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_SD_I_50;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_SD_I);
                    }

                case(UINT8)SV_ST_YP:
                    return (vHdtvGetOversampleForSD()?
                            SOURCE_TYPE_TIMING_YPBPR_SD_I_50_OS : SOURCE_TYPE_TIMING_YPBPR_SD_I_50);

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_525I:
        case MODE_525I_OVERSAMPLE:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_SD_I_60;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_SD_I;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_SD_I_60;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_SD_I);
                    }

                case(UINT8)SV_ST_YP:
                    return (vHdtvGetOversampleForSD()?
                            SOURCE_TYPE_TIMING_YPBPR_SD_I_60_OS : SOURCE_TYPE_TIMING_YPBPR_SD_I_60);

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_576P_25:
        case MODE_576P:
        case MODE_576P_OVERSAMPLE:
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_SD_P_50;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_SD_P;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_SD_P_50;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_SD_P);
                    }

                case(UINT8)SV_ST_YP:
                    return (vHdtvGetOversampleForSD()?
                            SOURCE_TYPE_TIMING_YPBPR_SD_P_50_OS : SOURCE_TYPE_TIMING_YPBPR_SD_P_50);

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_480P:
        case MODE_480P_30:
        case MODE_480P_24:
        case MODE_480P_OVERSAMPLE:
#if  SET_640_480_VIDEO_MODE         	
        case MODE_HDMI_640_480P: 	
#endif
            switch(bSignalType)
            {
                case(UINT8)SV_ST_DVI:
                    return SOURCE_TYPE_TIMING_DIGI_SD_P_60;

                case(UINT8)SV_ST_MPEG:
                    if(bAppType == FBM_FBG_MM_MODE)
                    {
                        return SOURCE_TYPE_TIMING_MM_SD_P;
                    }
                    else if(bAppType == FBM_FBG_DTV_MODE)
                    {
                        return SOURCE_TYPE_TIMING_DTV_SD_P_60;
                    }
                    else
                    {
                        return DRVCUST_MMAPPGetTiming(SOURCE_TYPE_TIMING_MM_SD_P);
                    }

                case(UINT8)SV_ST_YP:
                    return (vHdtvGetOversampleForSD()?
                            SOURCE_TYPE_TIMING_YPBPR_SD_P_60_OS : SOURCE_TYPE_TIMING_YPBPR_SD_P_60);

                default:
                    return SOURCE_TYPE_TIMING_MAX;
            }

        case MODE_3D_720p_50_FP:
            return SOURCE_TYPE_TIMING_3D_720p_50_FP;

        case MODE_3D_720p_60_FP:
            return SOURCE_TYPE_TIMING_3D_720p_60_FP;

        case MODE_3D_1080p_24_FP:
            return SOURCE_TYPE_TIMING_3D_1080p_24_FP;

        case MODE_3D_1080I_60_FP:
            return SOURCE_TYPE_TIMING_3D_1080I_60_FP;

        case MODE_3D_480p_60_FP:
            return SOURCE_TYPE_TIMING_3D_480p_60_FP;

        case MODE_3D_576p_50_FP:
            return SOURCE_TYPE_TIMING_3D_576p_50_FP;

        case MODE_3D_720p_24_FP:
            return SOURCE_TYPE_TIMING_3D_720p_24_FP;

        case MODE_3D_720p_30_FP:
            return SOURCE_TYPE_TIMING_3D_720p_30_FP;

        case MODE_3D_1080p_30_FP:
            return SOURCE_TYPE_TIMING_3D_1080p_30_FP;

        case MODE_3D_480I_60_FP:
            return SOURCE_TYPE_TIMING_3D_480I_60_FP;

        case MODE_3D_576I_60_FP:
            return SOURCE_TYPE_TIMING_3D_576I_60_FP;

        case MODE_3D_1080I_50_FP:
            return SOURCE_TYPE_TIMING_3D_1080I_50_FP;

        case MODE_3D_1080p_50_FP:
            return SOURCE_TYPE_TIMING_3D_1080p_50_FP;

        case MODE_3D_1080p_60_FP:
            return SOURCE_TYPE_TIMING_3D_1080p_60_FP;

        case MODE_3D_1650_750_60_FP:
            return SOURCE_TYPE_TIMING_3D_1650_750_60_FP;

        case MODE_3D_1650_1500_30_FP:
            return SOURCE_TYPE_TIMING_3D_1650_1500_30_FP;

        case MODE_3D_640_480p_60_FP:
            return SOURCE_TYPE_TIMING_3D_640_480p_60_FP;

        case MODE_3D_1440_240p_60_FP:
            return SOURCE_TYPE_TIMING_3D_1440_240p_60_FP;

        case MODE_3D_1440_288p_50_FP:
            return SOURCE_TYPE_TIMING_3D_1440_288p_50_FP;

        case MODE_3D_1440_576p_50_FP:
            return SOURCE_TYPE_TIMING_3D_1440_576p_50_FP;

        case MODE_3D_720p_25_FP:
            return SOURCE_TYPE_TIMING_3D_720p_25_FP;

        case MODE_3D_1080p_25_FP:
            return SOURCE_TYPE_TIMING_3D_1080p_25_FP;

        case MODE_3D_1080I_1250TOTAL_50_FP:
            return SOURCE_TYPE_TIMING_3D_1080I_1250TOTAL_50_FP;

        case MODE_3D_1080p_24_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_1080p_24_SBS_FULL;

        case MODE_3D_1080p_30_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_1080p_30_SBS_FULL;

        case MODE_3D_1080I_50_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_1080I_50_SBS_FULL;

        case MODE_3D_1080I_60_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_1080I_60_SBS_FULL;

        case MODE_3D_720p_24_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_720p_24_SBS_FULL;

        case MODE_3D_720p_30_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_720p_30_SBS_FULL;

        case MODE_3D_720p_50_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_720p_50_SBS_FULL;

        case MODE_3D_720p_60_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_720p_60_SBS_FULL;

        case MODE_3D_480p_60_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_480p_60_SBS_FULL;

        case MODE_3D_576p_50_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_576p_50_SBS_FULL;

        case MODE_3D_480I_60_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_480I_60_SBS_FULL;

        case MODE_3D_576I_50_SBS_FULL:
            return SOURCE_TYPE_TIMING_3D_576I_50_SBS_FULL;

        case MODE_DE_MODE:
            return SOURCE_TYPE_TIMING_PC_RGB;

        case MODE_3840_1080P_24:
        case MODE_3840_1080P_25:
        case MODE_3840_1080P_30:
        case MODE_3840_2160P_15:
        case MODE_3840_2160P_24:
        case MODE_3840_2160P_25:
        case MODE_3840_2160P_30:  
		case MODE_4096_2160P_24:
        	return SOURCE_TYPE_TIMING_DIGI_2160P_30;
        	
        default:
            assert(0);
            return SOURCE_TYPE_TIMING_MAX;
    }
}


/* new API to get/set eaten porch decoder+nr+di+scpos
    accrording to current video timing.
    20070612 sv_porch_t*/
UINT16 wDrvVideoPorchStd(UINT8 bPath, UINT8 bPorchType)
{
    switch(bPorchType)
    {
        case SV_HPORCH_MAX:
            return (wDrvVideoGetHTotal(bPath)-wDrvVideoInputWidth(bPath));

        case SV_VPORCH_MAX:
            return ((wDrvVideoGetVTotal(bPath)-wDrvVideoInputHeight(bPath))*4)/3; //*1.33

        case SV_VPORCH_MIN:
            return  2;

        case SV_HPORCH_MIN:
        default:
            return 0;
    }
}

UINT8 bDrvPorchTune[2]=
{
    SV_PORCHTUNE_DEC,
    SV_PORCHTUNE_DEC
};

UINT16 wDrvVideoGetPorchByDec(UINT8 bDecType, UINT8 bPorchType)
{
    UINT16  wDecPorch=0;
    switch(bDecType)
    {
        case SV_VD_TVD3D:
            wDecPorch=wTvdGetPorch(0,bPorchType);
            break;

        case SV_VD_VGA:
            wDecPorch=wVgaGetPorch(0,bPorchType);
            break;

        case SV_VD_YPBPR:
            wDecPorch=wHdtvGetPorch(0,bPorchType);
            break;

        case SV_VD_DVI:
            wDecPorch=wDviGetPorch(0,bPorchType);
            break;
#ifndef COPLAT_EXCLUDE_DTV

        case SV_VD_MPEGHD:
        case SV_VD_MPEGSD:
#endif
        default:
            wDecPorch=0;    //rely on scpos's porch
            break;
    }
    return wDecPorch;
}

UINT16 wDrvVideoGetPorch(UINT8 bPath, UINT8 bPorchType)
{
    UINT8 bDecType;
    UINT16  wDecPorch=0,wScposPorch=0;

    /* implemention try to make DI/NR don't eat porch,scops eat porch
       for overscan or lbox*/
    if(bDrvPorchTune[bPath]&SV_PORCHTUNE_DEC)
    {
        bDecType = bGetVideoDecType(bPath);

        switch(bDecType)
        {
            case SV_VD_TVD3D:
                wDecPorch=wTvdGetPorch(bPath,bPorchType);
                break;

            case SV_VD_VGA:
                wDecPorch=wVgaGetPorch(bPath,bPorchType);
                break;

            case SV_VD_YPBPR:
                wDecPorch=wHdtvGetPorch(bPath,bPorchType);
                break;

            case SV_VD_DVI:
                wDecPorch=wDviGetPorch(bPath,bPorchType);
                break;
#ifndef COPLAT_EXCLUDE_DTV

            case SV_VD_MPEGHD:
            case SV_VD_MPEGSD:
#endif
            default:
                wDecPorch=0;    //rely on scpos's porch
                break;
        }
    }

    if(bDrvPorchTune[bPath]&SV_PORCHTUNE_SCPOS)
    {
        wScposPorch=_VDP_ScposGetPorch(bPath,bPorchType);
    }

    return wDecPorch+wScposPorch;
}
UINT16 wDrvVideoSetPorch(UINT8 bPath, UINT8 bPorchType, UINT16 wValue)
{
    UINT8 bDecType;
    UINT16  wDecPorch=0,wScposPorch=0,min,max;
    INT32 tmp;
    //INT32 i4VBIadj; /* VBI LSC adjustion */

    if((bPorchType!=SV_HPORCH_CURRENT)&&(bPorchType!=SV_VPORCH_CURRENT))
        return 0;

    /* only tune porch to decoder, SV_HPORCH_CURRENT|SV_HPORCH_CURRENT */

    if(bDrvPorchTune[bPath]==SV_PORCHTUNE_MIX)
    {
        wScposPorch=_VDP_ScposGetPorch(bPath,  bPorchType);
    }

    bDecType = bGetVideoDecType(bPath);
    tmp=wValue-wScposPorch;

    if(tmp<0)  tmp=0;

    if(bDrvPorchTune[bPath]&SV_PORCHTUNE_DEC)
    {
        switch(bDecType)
        {
            case SV_VD_TVD3D:
                min=wTvdGetPorch(bPath,bPorchType+2);
                max=wTvdGetPorch(bPath,bPorchType+3);

                if(tmp<min) tmp=min;

                if(tmp>max) tmp=max;

                vTvdSetPorch(bPorchType,tmp);
                wDecPorch=wTvdGetPorch(bPath,bPorchType);
                vTvd3dGetUIDelay();
                break;

            case SV_VD_VGA:
                min=wVgaGetPorch(bPath,bPorchType+2);
                max=wVgaGetPorch(bPath,bPorchType+3);

                if(tmp<min) tmp=min;

                if(tmp>max) tmp=max;

                vVgaSetPorch(bPath,bPorchType,tmp);
                wDecPorch=wVgaGetPorch(bPath,bPorchType);
                break;

            case SV_VD_YPBPR:
                min=wHdtvGetPorch(bPath,bPorchType+2);
                max=wHdtvGetPorch(bPath,bPorchType+3);

                if(tmp<min) tmp=min;

                if(tmp>max) tmp=max;

                vHdtvSetPorch(bPath,bPorchType,tmp);
                wDecPorch=wHdtvGetPorch(bPath,bPorchType);
                break;

            case SV_VD_DVI:
                min=wDviGetPorch(bPath,bPorchType+2);
                max=wDviGetPorch(bPath,bPorchType+3);

                if(tmp<min) tmp=min;

                if(tmp>max) tmp=max;

                vDviSetPorch(bPath,bPorchType,tmp);
                wDecPorch=wDviGetPorch(bPath,bPorchType);
                break;
#ifndef COPLAT_EXCLUDE_DTV

            case SV_VD_MPEGHD:
            case SV_VD_MPEGSD:
#endif
            default:
                wDecPorch=0;
                break;
        }

        if(bDrvPorchTune[bPath]==SV_PORCHTUNE_DEC)
        {
            return wDecPorch;
        }
    }

    /*  SV_PORCHTUNE_MIX  or SV_PORCHTUNE_SCPOS */
    tmp=wValue-wDecPorch;
    /* scpos's min/max constrain is controlled inside _VDP_ScposSetPorch */
    _VDP_ScposSetPorch(bPath,bPorchType,tmp);
    wScposPorch=_VDP_ScposGetPorch(bPath,bPorchType);
    return wDecPorch+wScposPorch;
}

UINT8 bDrvVideoGetWidthOverSampleByDec(UINT8 bDecType)
{
    switch(bDecType)
    {
        case SV_VD_YPBPR:
        case SV_VD_VGA:
        //FIXME
            return 0;//(bHdtvInputWidthOverSample(bDrvVideoGetTiming(bPath)));

        case SV_VD_DVI:
            return 0;

        case SV_VD_TVD3D:
            return (vTvdGetOverSampleEnable());

        default:
            return 0;
    }
}
UINT8 bDrvVideoGetWidthOverSample(UINT8 bPath)
{
    UINT8 bType;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_YPBPR:
        case SV_VD_VGA:
            return (bHdtvInputWidthOverSample(bDrvVideoGetTiming(bPath)));

        case SV_VD_DVI:
            return 0;

        case SV_VD_TVD3D:
            return (vTvdGetOverSampleEnable());

        default:
            return 0;
    }
}

#if MANUAL_MAINSUB_FIFO_RST
/**
 * @brief Get the video path frame dealy time (DI, Scaler)
 * @param Path: 1: PIP 0: Main
 * @return : none
 */
void vDrvMainSubFifoReset(UINT8 bPath)
{
    if(bPath == 1)
    {
        vPipFIFOResetOn();
        vUtDelay2us(1);
        vPipFIFOResetOff();
    }
    else
    {
        vMainFIFOResetOn();
        vUtDelay2us(1);
        vMainFIFOResetOff();
    }
}
#endif


/**
 * @brief Get the video path frame dealy time (DI, Scaler)
 * @param u1Path
 * @return delay time in mini-sencond.
 */
UINT16 bDrvVideoGetFrameDelay(UINT8 bPath)
{
    UINT16 u2FrameDelay = 0;
    UINT16 u2ModuleDelay[3]={0,0,0};
    UINT8  u1FrameRate;

    if(bPath != SV_VP_MAIN)
    {
        return 0;
    }

    u1FrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);

    if(u1FrameRate == 0)  // prevent divide by 0 case
    {
        return 0;
    }

#ifdef DRV_SUPPORT_MT8283
    d_custom_extmjc_get_delay((UINT8 *)&u2FrameDelay, NULL);
#endif
#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
    u2FrameDelay += PANEL_GetFrameDelay();
#endif

    u2ModuleDelay[0]=u1DrvDIGetFrameDelay10x(bPath);
    u2FrameDelay += u2ModuleDelay[0];
    u2ModuleDelay[1]=u1DrvScpipGetFrmDelay10x(bPath);
    u2FrameDelay += u2ModuleDelay[1];
    u2ModuleDelay[2]=MJC_FRAME_DELAY();
    u2FrameDelay += u2ModuleDelay[2];

    LOG(3, "bDrvVideoGetFrameDelay x10 frame (PScan %d, Scaler %d, MJC %d), (Total: %d)\n", u2ModuleDelay[0], u2ModuleDelay[1], u2ModuleDelay[2], u2FrameDelay);
    u2FrameDelay = (u2FrameDelay * 100) / u1FrameRate; // calculate by ms scale (1000ms / FrmRate)
    LOG(3, "Total frame delay time(ms) :%d\n", u2FrameDelay);
    return u2FrameDelay;
}

#if 1//def CC_AUD_QUERY_VDO_INFO
/**
 * Usage: include vdo_if.h and video_timing.h
 */
void DrvVideoGetInputTimingInfo(UINT8 bPath, VDO_INPUT_INFO *info)
{
    info->u2Width = wDrvVideoInputWidth(bPath);
    info->u2Height = wDrvVideoInputHeight(bPath);
    info->u1Timing = bDrvVideoGetTiming(bPath);
    info->u1Interlace = bDrvVideoIsSrcInterlace(bPath);
    info->u1RefreshRate = bDrvVideoGetRefreshRate(bPath);
    #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
    info->u1PanelDelay = PANEL_GetFrameDelay();
    #endif
}
#endif

UINT8 u1DrvVideoGetPullDownInfo(UINT8 bPath)
{
    UINT32 u4PdInfo = 0;

    if (bDrvVideoIsSrcInterlace(bPath))
    {
        u4PdInfo = IO32ReadFldAlign(MCVP_STATUS_21, FILM_CB_STATUS);
    }
    else
    {
        u4PdInfo = MJC_FD_INFO();
    }

    if (1 <= u4PdInfo && u4PdInfo <= 5)
    {
        return 1;
    }
    else if (6 <= u4PdInfo && u4PdInfo <= 7)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

UINT8 u1DrvVideoGetIs44PullDownInfo(void)
{
    #ifdef CC_MT5399
    return (IO32ReadFldAlign(MJC_MISC_00, MJC_IS_44PULLDOWN));
    #else
    return SV_TRUE;
    #endif
}
void vDrvVideoQueryInputTimingInfo(UINT32 u4Path)
{
    UINT8 u1VdoStatus = bDrvVideoSignalStatus(u4Path);
    UINT8 u1VdoType = bGetVideoDecType(u4Path);
    UINT8 u1VdoPulldown = u1DrvVideoGetPullDownInfo(u4Path);

    Printf("[%s Video Status]\n", (u4Path == SV_VP_MAIN) ? "Main" : "Sub");
    Printf("Source            = %-s\n",
        u1VdoType == SV_VD_TVD3D ? "TVD" :
        u1VdoType == SV_VD_YPBPR ? "YPbPr" :
        u1VdoType == SV_VD_VGA ? "VGA" :
        u1VdoType == SV_VD_DVI ? "DVI" :
        u1VdoType == SV_VD_MPEGHD ? "MPGEHD" :
        u1VdoType == SV_VD_MPEGSD ? "MPGESD" : "Unknown");
    Printf("Signal Status     = %-s\n",
        u1VdoStatus == SV_VDO_NOSIGNAL ? "No Signal" :
        u1VdoStatus == SV_VDO_NOSUPPORT ? "Not Support" :
        u1VdoStatus == SV_VDO_STABLE ? "Stable" : "Unknown");
    Printf("Video Timing      = %-s\n", strDrvVideoGetTimingString(bDrvVideoGetTiming(u4Path)));
    Printf("Video Resolution  = %dx%d, %dx%d, %dHz\n",
        wDrvVideoInputWidth(u4Path),
        wDrvVideoInputHeight(u4Path),
        wDrvVideoGetHTotal(u4Path),
        wDrvVideoGetVTotal(u4Path),
        bDrvVideoGetRefreshRate(u4Path));
    Printf("Pull-down         = %-s\n",
        u1VdoPulldown == 1 ? "3:2 Pull-down" :
        u1VdoPulldown == 2 ? "2:2 Pull-down" : "No Pull-down");
}

void vDrvLoadRegTbl(REGTBL_T const * pRegTbl) reentrant
{
    if(pRegTbl == NULL)
    {
        return;
    }

    while(pRegTbl->u4Reg != REGTBL_END)
    {
        vIO32Write4BMsk(pRegTbl->u4Reg, pRegTbl->u4Value, pRegTbl->u4Mask);
        pRegTbl++;
    }
}

UINT32 u4DrvVideoGetNSDOffset(UINT8 bPath)
{
    UINT8 bType;
    UINT32 u4Ret = (UINT32)0x800;
    bType = bGetVideoDecType(bPath);

    switch(bType)
    {
        case SV_VD_TVD3D:
        {
            RTvdNSTDStatus rStatus;
            vTvd3dGetNSTDStatus(&rStatus);

            if(rStatus.fgIsNSTD)
            {
                u4Ret = (UINT32)rStatus.wVTotalDiff;
            }

            break;
        }

        case SV_VD_YPBPR:
        {
            RHDTVNSTDStatus rHDTVStatus;
            vHDTVGetNSTDStatus(&rHDTVStatus);

            if(rHDTVStatus.fgIsNSTD)
            {
                u4Ret = (UINT32)rHDTVStatus.wVTotalDiff;
            }

            break;
        }

#if SUPPORT_DVI

        case SV_VD_DVI:
#endif
#ifndef COPLAT_EXCLUDE_DTV
        case SV_VD_MPEGHD:
        case SV_VD_MPEGSD:
#endif
        default:
            break;
    }

    return u4Ret;
}
#ifdef CC_SCPOS_PATTERN_GENERATOR
UINT8 u1ScposPtGen = 0;
void vSetScposPatternGen(UINT8 u1Enable)
{
    // be sure to enable this before change to VGA source
    u1ScposPtGen = u1Enable;
    vSetMainFlg(MAIN_FLG_MODE_CHG);
    vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
}
UINT8 u1GetScposPtGenEnable(void)
{
    return u1ScposPtGen;
}
#endif

UINT32 u43DGetOutputLR(void)
{
    return SCPIP_GET_DISPR();
}

VDO_DRAM_STATUS_T _arVdoDramStatus[] = 
{
    // OD
    {OD_STA02,          R_UNDERFLOW,            FIELD_TO_STR(R_UNDERFLOW)},
    {OD_STA02,          W_UNDERFLOW,            FIELD_TO_STR(W_UNDERFLOW)},

    // MIB
    {MCVP_DRAM_08,      STA_OVERFLOW,           FIELD_TO_STR(STA_OVERFLOW)},
    {MCVP_DRAM_08,      STA_UNDERFLOW,          FIELD_TO_STR(STA_UNDERFLOW)},
    {MCVP_DRAM_1A,      STA_SC_UNDERFLOW,       FIELD_TO_STR(STA_SC_UNDERFLOW)},
    {MCVP_DRAM_14,      STA_NR_OVERFLOW,        FIELD_TO_STR(STA_NR_OVERFLOW)},
    {MCVP_DRAM_07,      STA_FLAG_OVERFLOW,      FIELD_TO_STR(STA_FLAG_OVERFLOW)},
    {MCVP_DRAM_07,      STA_FLAG_UNDERFLOW,     FIELD_TO_STR(STA_FLAG_UNDERFLOW)},

    // Scaler
    {SCPIP_DRAM_M_02,   DRAM_M_02_STA_OVERFLOW, FIELD_TO_STR(DRAM_M_02_STA_OVERFLOW)},

    // MJC
    MJC_DRAM_STATUS

    // End
    {0xFFFFFFFF,        0x0,                    FIELD_TO_STR(0)}
};

VDO_DRAM_STATUS_CLR_T _arVdoDramStatusClr[] = 
{
    // OD
    {OD_REG47,          WOV_CLR},
    {OD_REG47,          ROV_CLR},

    // MIB
    {MCVP_DRAM_08,      DA_OVERFLOW_CLR},
    {MCVP_DRAM_08,      DA_UNDERFLOW_CLR},
    {MCVP_DRAM_18,      DA_SC_UNDERFLOW_CLR},
    {MCVP_DRAM_01,      DA_FLAG_OVERFLOW_CLR},
    {MCVP_DRAM_01,      DA_FLAG_UNDERFLOW_CLR},
    {MCVP_DRAM_14,      DA_NR_OVERFLOW_CLR},
    {MCVP_DRAM_08,      DA_STATUS_CLR},
    {MCVP_DRAM_14,      DA_NR_STATUS_CLR},
    {MCVP_DRAM_01,      DA_FLAG_STATUS_CLR},

    // Scaler
    {SCPIP_DRAM_M_00,   DRAM_M_00_DA_OVERFLOW_CLR},

    // MJC
    MJC_DRAM_STATUS_CLEAR

    // End
    {0xFFFFFFFF,        0x0}
};

void vDrvVideoDramStatusRst(void)
{
    UINT32 u4TableIndex = 0;

    while (_arVdoDramStatusClr[u4TableIndex].u4Addr != 0xFFFFFFFF)
    {
        vIO32WriteFldAlign(
            _arVdoDramStatusClr[u4TableIndex].u4Addr,
            SV_ON,
            _arVdoDramStatusClr[u4TableIndex].u4Fld);
        vIO32WriteFldAlign(
            _arVdoDramStatusClr[u4TableIndex].u4Addr,
            SV_OFF,
            _arVdoDramStatusClr[u4TableIndex].u4Fld);

        u4TableIndex ++;
    }
}

void vDrvVideoDramStatusChk(void)
{
    if (u4VdoDramStatusMonCount == 0)
    {
        return;
    }

    {
        UINT32 u4TableIndex = 0;
        while (_arVdoDramStatus[u4TableIndex].u4Addr != 0xFFFFFFFF)
        {
            if (IO32ReadFldAlign(_arVdoDramStatus[u4TableIndex].u4Addr, _arVdoDramStatus[u4TableIndex].u4Fld))
            {
                Printf("0x%08X: %s\n", u4VdoDramStatusMonCount, _arVdoDramStatus[u4TableIndex].szName);
                u4VdoDramStatusMonCount = 0;
            }

            u4TableIndex ++;
        }

        if ((u4VdoDramStatusMonCount & 0xF) == 0)
        {
            Printf(".");
        }
    }

    if (u4VdoDramStatusMonCount)
    {
        u4VdoDramStatusMonCount --;
    }
}

#if CC_CHECK_DEC_STABLE
void vCheckDeocerStable(UINT8 bState)
{
    static UINT8 bCheckStable =0, bCounter =0;

    switch(bState)
    {
        case SV_DISABLE_DEC_STABLE_CHK:
            bCheckStable = 0 ;
            break;

        case SV_ENABLE_DEC_STABLE_CHK:
            vIO32WriteFldAlign(SYS_05, 1, RAND_EN);
            bCheckStable = 1 ;
            bCounter = 0 ;
            break;

        case SV_CHECK_DEC_HV:
            if(bCheckStable && (bCounter < 50))
            {
                bCounter++;
                Printf("%d=========Htotal/hde %d %d\n",bCounter, IO32ReadFldAlign(STA_VFE_04, RANDOM_HTOTAL), IO32ReadFldAlign(STA_VFE_04, RANDOM_HDELEN));
                Printf("%d=========vtotal/vde %d %d\n", bCounter, IO32ReadFldAlign(STA_VFE_05, RANDOM_VTOTAL), IO32ReadFldAlign(STA_CTG_05, VDELEN));
            }

            break;

        default:
            break;
    }
}
#endif

void vShowOmuxStatus(UINT8 u1Path)
{
    Printf("=========================\n");
    vIO32WriteFldAlign(SYS_05, 1, RAND_EN);
    vIO32WriteFldAlign(SYS_05, (u1Path == SV_VP_MAIN) ? 0 : 1, RAND_CH_SEL);
    vUtDelay10ms(5);
    Printf("omux Htotal/hde %d %d\n", IO32ReadFldAlign(STA_VFE_04, RANDOM_HTOTAL), IO32ReadFldAlign(STA_VFE_04, RANDOM_HDELEN));
    Printf("omux vtotal/vde %d %d\n",  IO32ReadFldAlign(STA_VFE_05, RANDOM_VTOTAL), IO32ReadFldAlign(STA_CTG_05, VDELEN));
}

extern UINT32 _u4NSDHTotal;
extern UINT32 _u4NSDVTotal;
extern UINT32 _u4NSDFrameRate;
void vDrvForceFrameRate(UINT8 u1FrameRate)
{
    vDrvSetFixedFrameRate(u1FrameRate);
    //vSetMainFlg(MAIN_FLG_MODE_CHG);
    vDrvCalPanelFrameRate(wDrvVideoGetVTotal(SV_VP_MAIN), u1FrameRate);
    // reset non-standard signal status
    _u4NSDHTotal = 0xff;
    _u4NSDVTotal = 0xff;
    _u4NSDFrameRate = 0xff;
}

#include "eepdef.h"
#include "api_eep.h"
#include "x_drv_cli.h"

void vCheckBootPtGen(void)
{
    UINT8 bEnableBootCmd = bApiEepromReadByte(EEP_NPTV_BOOT_DEBUG_ENABLE);
    UINT8 u1PtGenNo ;
    CHAR sTmpStr[30];

    if(bEnableBootCmd == SV_BOOT_DEBUG_MAGIC)
    {
        UINT8 bBootCmd = bApiEepromReadByte(EEP_NPTV_BOOT_DEBUG_CMD);
        UINT8 bBootParam ;
        UINT8 bPath  = (bBootCmd >> 6) & 1; //bit[6]
        UINT8 bForceUnmute = (bBootCmd >> 7) & 1; //bit[7]
        bBootParam = bBootCmd & 0x1f ; //bit[4:0]
        bBootCmd= (bBootCmd >> 5) & 0x1 ;  // //bit[5:5]
        Printf("NPTV Boot Debug Cmd %d %d %d\n", bPath, bBootCmd, bBootParam);

        if(bBootCmd == 1)
        {
            if(bBootParam <= 9)
            {
                u1PtGenNo = bBootParam + '0';
            }
            else
            {
                u1PtGenNo = bBootParam - 10 + 'A';
            }

            x_snprintf(sTmpStr, 30, "n.pt.auto %d %c", bPath, u1PtGenNo);
            Printf("%s\n", sTmpStr);
            CLI_Parser(sTmpStr);
        }

        if(bForceUnmute)
        {
            Printf("Boot debug force Unmute all\n");
            _vDrvVideoForceUnMute(bPath, SV_TRUE);
        }
    }

}


BOOL fgIsVideoDone(UINT8 bPath)
{
    if((bPath == SV_VP_MAIN) && (_bMainState == VDO_STATE_IDLE) && !fgIsMainFlgSet(MAIN_FLG_MODE_CHG))
    {
        return TRUE;
    }
    else if((bPath == SV_VP_PIP) && (_bPipState == VDO_STATE_IDLE) && !fgIsPipFlgSet(PIP_FLG_MODE_CHG))
    {
        return TRUE;
    }

    return FALSE;
}




#ifdef CC_SUPPORT_PREPROC_TIMING
void vPreProcConnect(UINT8 u1Path, UINT8 fgIsOn)
{
    LOG(3,"vPreProcConnect and u1Path(%d)fgIsOn(%d)\n",u1Path,fgIsOn);
    if(fgIsOn == SV_ON)
    {
       // _vDrvVideoSetMute(MUTE_MODULE_MODECHG, u1Path, FOREVER_MUTE, FALSE);
		vDrvEnablePreprocSelfGen(u1Path,SV_ON);
    }
	else
	{
	    vDrvEnablePreprocSelfGen(u1Path,SV_OFF);
	}
	if (u1Path == SV_VP_MAIN)
	{
		vSetMainFlg(MAIN_FLG_MODE_CHG);
		vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
	}
	else
	{
		vSetPipFlg(PIP_FLG_MODE_CHG);
		vSetPipFlg(PIP_FLG_MODE_DET_DONE);
	}
}
#endif

#ifdef REAL_TIME_FRAME_DELAY_EN

typedef enum
{
    E_DLY_MODULE_DI,
    E_DLY_MODULE_SCALER,
    E_DLY_MODULE_MJC,
    E_DLY_MODULE_NONE,
}E_DLY_MEASURE_MODULE;

typedef enum
{
    E_CAPTURE_POINT_DI_IN,
    E_CAPTURE_POINT_SCALER_IN,
    E_CAPTURE_POINT_MJC_IN,
    E_CAPTURE_POINT_LVDS_IN,
    E_CAPTURE_POINT_NONE,
}E_CAPTURE_POINT;

/**
 * @brief Set capture data position(DI IN, Scaler IN, MJC IN,  LVDS IN)
 * @param none
 * @return void
 */
void vDrvVideoSetCapturePos(void)
{
    //
    vIO32WriteFldAlign(MCVP_KC_07, 0x10, IPTG_PRB_X_POS);
    vIO32WriteFldAlign(MCVP_KC_07, 0x10, IPTG_PRB_Y_POS);
    //
    vIO32WriteFldAlign(MCVP_BC_05, 0x10, FINAL_PRB_X_POS);
    vIO32WriteFldAlign(MCVP_BC_05, 0x10, FINAL_PRB_Y_POS);
    // 
    vIO32WriteFldAlign(POS_MAIN, 0x10, SCE_POS_X);
    vIO32WriteFldAlign(POS_MAIN, 0x10, SCE_POS_Y);
    vIO32WriteFldAlign(DBG_CFG_MAIN, 0x1, SCE_CAP_EN);
    //
    vIO32WriteFldAlign(DUMP_01, 0x10, POS_X);
    vIO32WriteFldAlign(DUMP_01, 0x10, POS_Y);
}


/**
 * @brief Set pre-proc input pattern, black and white
 * @param none
 * @return void
 */
void vDrvVideoSetPattern(UINT8 u1OnOff , UINT8 u1PatternType)
{
    if(u1OnOff)
    {
        if(u1PatternType == 1)
        {
            vIO32WriteFldAlign(PAT_MAIN_05, 0x3FF, C_MAIN_BC_Y);
            vIO32WriteFldAlign(PAT_MAIN_02, 0x5, C_MAIN_PAT_TYPE);
        }
        else
        {
            vIO32WriteFldAlign(PAT_MAIN_05, 0x0, C_MAIN_BC_Y);
            vIO32WriteFldAlign(PAT_MAIN_02, 0x5, C_MAIN_PAT_TYPE);
        }
    }
    else
    {
         vIO32WriteFldAlign(PAT_MAIN_02, 0x0, C_MAIN_PAT_TYPE);
    }
}

/**
 * @brief Get captured data.
 * @param Captured point: DI in ,Scaler in, MJC in, LVDS in
 * @return Captured data
 */
UINT16 vDrvVideoCaptureLevl(E_CAPTURE_POINT point)
{
    static UINT16 u2Level = 0;
    if(point > E_CAPTURE_POINT_NONE)
    {
        return 0;
    }
    switch(point)
    {
        case E_CAPTURE_POINT_DI_IN:
        u2Level = IO32ReadFldAlign(MCVP_KC_08, IPTG_PRB_OUT_Y);
        break;
        case E_CAPTURE_POINT_SCALER_IN:
        u2Level = IO32ReadFldAlign(MCVP_BC_07, FINAL_PROBE_Y);
        break;
        case E_CAPTURE_POINT_MJC_IN:
        u2Level = IO32ReadFldAlign(CAP_IN_DATA_MAIN,CAP_IN_Y);
        break;
        case E_CAPTURE_POINT_LVDS_IN:
        u2Level = IO32ReadFldAlign(DUMP_04,POS_OUT_G);
        break;
        default:
        break;
    }
    return u2Level;
    
}

UINT32 u4LevelChangeTime[E_CAPTURE_POINT_NONE]={0};
UINT16 u2ModuleDelay[E_DLY_MODULE_NONE]={0};
UINT16 u2PreLevel[E_CAPTURE_POINT_NONE]={0};

/**
 * @brief Calc out real-time frame delay
 * @param none
 * @return void
 */
void vDrvVideoGetFrameDelay(void)
{
    E_DLY_MEASURE_MODULE i;
    UINT16 u2VideoDelay = 0;
    for(i = E_DLY_MODULE_DI; i< E_DLY_MODULE_NONE; i++)
    {
        u2ModuleDelay[i]= (UINT16)(u4LevelChangeTime[i+1]-u4LevelChangeTime[i]);
        u2VideoDelay += u2ModuleDelay[i];
        LOG(0,"%d-->%d ms.\n",i,u2ModuleDelay[i]);
    }
    LOG(0,"Video total delay -->%d ms.\n",u2VideoDelay);

}


/**
 * @brief Get level change time of each measure point.
 * @param none
 * @return TRUE: loop break
 * @return TRUE: loop continue
 */
BOOL bDrvVideoProc(void)
{
    E_CAPTURE_POINT i;
    static HAL_TIME_T _rTime;
    for(i = E_CAPTURE_POINT_DI_IN; i <E_CAPTURE_POINT_NONE ;i++)
    {
        if(u2PreLevel[i]!=vDrvVideoCaptureLevl(i))
        {
            HAL_GetTime(&_rTime);
            u4LevelChangeTime[i] = _rTime.u4Seconds *1000 + _rTime.u4Micros/1000;
           //LOG(0,"==%d,%d,%d.\n",i,vDrvVideoCaptureLevl(i),u4LevelChangeTime[i]);
            u2PreLevel[i]= vDrvVideoCaptureLevl(i);
            if(i == E_CAPTURE_POINT_LVDS_IN)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

/**
 * @brief Main loop of frame delay measure thread.
 * @param UNUSED.
 * @return void
 */
void vDrvVideoDelayLoop(void* pvArgv)
{
    static UINT8 u1Continue = 1;
    UINT8 i;
    HAL_TIME_T TimeStart, TimeCurrent, TimeDelta;
    //set capture position.
    vDrvVideoSetCapturePos();

    //set black pattern to reset register value.
    vDrvVideoSetPattern(SV_ON, 0);

    //thread delay. 
    x_thread_delay(3000);

    //set white pattern to measure delay.
    vDrvVideoSetPattern(SV_ON, 1);

    HAL_GetTime(&TimeStart);    
    //get level change time of measured point.
    while(u1Continue)
    {
        if(bDrvVideoProc())
        {
            u1Continue = 0;
            LOG(0,"success.\n");
        }

        //avoid fail, force break
        HAL_GetTime(&TimeCurrent);
        HAL_GetDeltaTime(&TimeDelta, &TimeStart, &TimeCurrent);
        if(TimeDelta.u4Seconds > 1)
        {
            u1Continue = 0;
            LOG(0,"Fail.\n");
        }
    }

    //calc out frame delay
    vDrvVideoGetFrameDelay();

    //set pattern off
    vDrvVideoSetPattern(SV_OFF, 0);

    //reset parameters for next time
    u1Continue = 1;
    for(i = 0; i < E_CAPTURE_POINT_NONE ;i++)
    {
        u2PreLevel[i] = 0;
    }
    
}

void vDrvVideoDelayCreateThread(void)
{
    static HANDLE_T _hThread;
    INT32 i4Ret;
    i4Ret = x_thread_create(&_hThread, "Video real-time frame delay thread", 4096, 100,
        vDrvVideoDelayLoop, 0, NULL);

    if (i4Ret != OSR_OK)
    {
        Printf("Video real-time frame delay thread cannot be created\n");
    }

}

#endif

#ifdef TIME_MEASUREMENT

void vDrvVideoDelayLoopExInput(void* pvArgv)
{    
    while(1)
    {
        vIO32WriteFldAlign(PAT_MAIN_06, 0x1, C_MAIN_PRB_EN);
        vIO32WriteFldAlign(PAT_MAIN_06, 0x5, C_MAIN_PRB_X_POS);
        vIO32WriteFldAlign(PAT_MAIN_06, 0x5, C_MAIN_PRB_Y_POS);

        if((IO32ReadFldAlign(PAT_MAIN_07,S_MAIN_PRB_Y)== 64)&&
            (IO32ReadFldAlign(PAT_MAIN_07,S_MAIN_PRB_CB)== 512)&&
            (IO32ReadFldAlign(PAT_MAIN_07,S_MAIN_PRB_CR)== 960))
        {
            static HAL_TIME_T _rTime;
            HAL_GetTime(&_rTime);
            Printf("----->>>>>>%d,%d.\n",_rTime.u4Seconds,_rTime.u4Micros);
            TMS_DIFF_EX(TMS_FLAG_WFD_LATENCY,"WFD_DATA:","VIDEO PATH INPUT DUMP");
            x_thread_delay(40);
        }
        x_thread_delay(1);
    }

}

void vDrvVideoDelayLoopExOutput(void* pvArgv)
{    
    while(1)
    {
        vIO32WriteFldAlign(DUMP_01, 0x10, POS_X);
        vIO32WriteFldAlign(DUMP_01, 0x10, POS_Y);
        vIO32WriteFldAlign(DUMP_00, 0x1, DUMP_EN);
        
        if((IO32ReadFldAlign(DUMP_04,POS_OUT_R)!=0)&&
            (IO32ReadFldAlign(DUMP_04,POS_OUT_G)==0)&&
            (IO32ReadFldAlign(DUMP_05,POS_OUT_B)==0))
        {
            static HAL_TIME_T _rTime;
            HAL_GetTime(&_rTime);
            Printf("----->>>>>>%d,%d.\n",_rTime.u4Seconds,_rTime.u4Micros);
            TMS_DIFF_EX(TMS_FLAG_WFD_LATENCY,"WFD_DATA:","VIDEO PATH OUT DUMP");
            x_thread_delay(40);
        }
        x_thread_delay(1);
    }

}


void vDrvVideoDelayCreateThreadEx(void)
{
    static HANDLE_T _hThread1,_hThread2;
    INT32 i4Ret1,i4Ret2;
    i4Ret1 = x_thread_create(&_hThread1, "VidDlyInput", 4096, 100,
        vDrvVideoDelayLoopExInput, 0, NULL);
    i4Ret2 = x_thread_create(&_hThread2, "VidDlyOutput", 4096, 100,
        vDrvVideoDelayLoopExOutput, 0, NULL);

    if (i4Ret1 != OSR_OK)
    {
        Printf("Video real-time frame delay input thread cannot be created\n");
    }
    if (i4Ret2 != OSR_OK)
    {
        Printf("Video real-time frame delay output thread cannot be created\n");
    }
}
#endif

UINT8 u1ForceDualMode = FALSE;

UINT8 bDrvVideoIsDecDualMode(void)
{
    return FALSE;
}

void vDrvVideoForceDualMode(UINT8 u1OnOff)
{
    if (u1ForceDualMode != u1OnOff)
    {
        u1ForceDualMode = u1OnOff;
        vSetMainFlg(MAIN_FLG_MODE_CHG);
        vSetMainFlg(MAIN_FLG_MODE_DET_DONE);

        vSetPipFlg(PIP_FLG_MODE_CHG);		
        vSetPipFlg(PIP_FLG_MODE_DET_DONE);
    }
}



