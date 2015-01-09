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
 * $RCSfile: pe_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nptv_if.h
 *  NPTV video path control, include  input source select ,
 *  timing information, and video quality control
 */

#ifndef _PE_IF_H_
#define _PE_IF_H_

#include "drv_hdmi.h"
#include "util.h"
#include "drv_vga.h"
#include "drv_tvd.h"
#include "api_notify.h"
#include "u_drv_cust.h"
#include "drv_ycproc.h"
// Quality Item and Enumation
#include "pe_table.h"

#define PQ_OK                   0  ///< Success
#define PQ_NOT_OK              -1  ///< Not ok
#define PQ_NOT_SUPPORTED       -2  ///< Not support
#define PQ_PARAMETER_ERROR     -3  ///< Parameter error
#define PQ_DONT_CARE           -4  ///< Don't care
#define PQ_ERR_INV             -5  ///< Invalid argument passed
#define PQ_ERR_TIMEDOUT        -6  ///< Timeout occured
#define PQ_PARAMETER_NOCHANGE  -7  /// Paramenter no change

#define MJC_CTRL_EFCT_OFF		0x00            // MJC 1W1R (bypass)
#define MJC_CTRL_EFCT_LOW		0x40
#define MJC_CTRL_EFCT_MID		0x80
#define MJC_CTRL_EFCT_HIGH		0xff
#define MJC_CTRL_EFCT_MSK		0xff
enum
{
    MJC_CTRL_INTP_OFF,
    MJC_CTRL_INTP_HFBK,
    MJC_CTRL_INTP_NORM
};
typedef enum
{
    VDP_ARG_MJC_EFFECT_OFF,
    VDP_ARG_MJC_EFFECT_LOW,
    VDP_ARG_MJC_EFFECT_MIDDLE,
    VDP_ARG_MJC_EFFECT_HIGH
} MJC_EFFECT_MODE;

typedef enum
{
    VDP_ARG_MJC_MODE_NORM,
    VDP_ARG_MJC_MODE_NO_DEBLUR,
    VDP_ARG_MJC_MODE_NO_FILM
} MJC_DEJUDDER_MODE;

typedef enum
{
    VDP_ARG_MJC_DEMO_ALL,
    VDP_ARG_MJC_DEMO_RIGHT,     // Enable Right, disable left
    VDP_ARG_MJC_DEMO_LEFT       // Enable left, disable right
} MJC_DEMO_MODE;

enum
{
    SV_COLOR_HDMI_sRGB,
	SV_COLOR_HDMI_Adobe_RGB,
	SV_COLOR_HDMI_sYCC,
    SV_COLOR_HDMI_xvYCC,
    SV_COLOR_HDMI_BT2020,
    SV_COLOR_HDMI_BT2020c,
    SV_COLOR_HDMI_Normal,
};

enum
{
    SV_COLOR_HDMI_Colorimetry_RGB,
    SV_COLOR_HDMI_Colorimetry_601,
    SV_COLOR_HDMI_Colorimetry_709,
    SV_COLOR_HDMI_Colorimetry_XVYCC601,
    SV_COLOR_HDMI_Colorimetry_XVYCC709,
    SV_COLOR_HDMI_Colorimetry_sYCC601,
	SV_COLOR_HDMI_Colorimetry_AdobeYCC601,
	SV_COLOR_HDMI_Colorimetry_AdobeRGB,
	SV_COLOR_HDMI_Colorimetry_BT2020,
	SV_COLOR_HDMI_Colorimetry_UnKnown
};



#ifdef DRV_SUPPORT_EXTMJC

EXTERN INT32 d_custom_extmjc_set_ui(UINT8  ui2_level,
                                          UINT8  ui2_mode,
                                          UINT8  ui2_demo,
                                          UINT8* pui1_msg_error);

#define vMJCSetDemoMode(A, B, C)        d_custom_extmjc_set_ui(A, B, C, NULL)

#endif

enum
{
    PEQTY_APPLY_TIME_OUTPUT_VSYNC_ISR,
    PEQTY_APPLY_TIME_INPUT_VSYNC_ISR,        
    PEQTY_APPLY_TIME_MJC_INPUT_VSYNC_ISR,        
    PEQTY_APPLY_TIME_MJC_OUTPUT_VSYNC_ISR,
    PEQTY_APPLY_TIME_OD_VSYNC_ISR,
};


typedef enum { 
    E_CUST_NO_EVEMT                 = 0,
    E_CUST_SOURCE_CHANGE          	= (1<<0),
    E_CUST_SIGNAL_FMT_CHANGE        = (1<<1),
    E_CUST_PLANE_MUTE_CHANGE      	= (1<<2),
    E_CUST_OUT_MUTE_CHANGE      	= (1<<3),
    E_CUST_OUT_POSI_CHANGE          = (1<<4),
    E_CUST_SRC_AREA_CHANGE        	= (1<<5),
    E_CUST_SCALING_REQ_CHANGE     	= (1<<6),
    E_CUST_BLUR_REQ_CHANGE   	  	= (1<<7),
    E_CUST_MJC_FREQ_CHANGE	     	= (1<<8),
    E_CUST_OUT_FMT_CHANGE     		= (1<<9),
    E_CUST_BORDER_PLN_REQ_CHANGE    = (1<<10),
    E_CUST_BL_POWER_LIMIT_CHANGE    = (1<<11),
    E_CUST_VDO_DELAY_CHANGE       	= (1<<12),
    E_CUST_POWER_STATE_CHANGE       = (1<<13),
    E_CUST_SCREEN_SAVER_CHANGE      = (1<<14),
    E_CUST_AGING_MODE_CHANGE		= (1<<15),
    E_CUST_MUFFIN_SCALING_CHANGE    = (1<<16),
    E_CUST_PIC_SETTING_CHANGE       = (1<<17),
    E_CUST_SCR_SETTING_CHANGE       = (1<<18),
    E_CUST_3D_SETTING_CHANGE        = (1<<19),
    E_CUST_ECO_SETTING_CHANGE       = (1<<20),
    E_CUST_PHOTO_SETTING_CHANGE     = (1<<21),
    E_CUST_DEMO_SETTING_CHANGE      = (1<<22),
    E_CUST_DEMO_OSD_MASK_CHANGE     = (1<<23),
    E_CUST_SERVICE_SETTING_CHANGE   = (1<<24),
} E_CUST_EVENT;



//-----------------------------------------------------------------------------
//function Declare
//-----------------------------------------------------------------------------
/*****************************************************************************************/
/********************************   SYSTEM & DEBUG     ***********************************/
/*****************************************************************************************/
void vApiVideoPeInit(void);
void vApiApplyPQSetting(UINT8 bPath);
void vApiApplyVideoEvent(UINT8 bApplyTime);
void vApiRegisterVideoEvent(UINT8 bVdpArg, UINT8 bPath, UINT8 bOnOff);
void vApiRegisterVideoEventByApplyTime(UINT8 bVdpArg, UINT8 bPath, UINT8 bOnOff, UINT8 bApplyTime);
void vApiVideoDemoMode(UINT8 bmode, Region DemoRec, UINT8 bDEMOModules);
UINT8 bApiVideoProc(UINT8 u1VidPath, PE_ARG_TYPE u1UiItem);
UINT32 PE_SetArg(UCHAR ucVdpId, PE_ARG_TYPE ucArg, INT16 i2Value);
UINT32 PE_GetArg(UCHAR ucVdpId, PE_ARG_TYPE ucArg, INT16* pi2Value);
INT32 PE_SetPqInfo(UCHAR ucVdpId, UINT32 u4PqType, const void* pkvPqSetInfo, UINT32 u4Size);
INT32 PE_GetPqInfo(UCHAR ucVdpId, UINT32 u4PqType, void* pkvPqGetInfo, UINT32 u4Size);
UINT8 bApiGetPeUiDebugMode(void);
void vApiSetPeUiDebugMode(UINT8 bOnOff);
void DRVCUST_GetLcdimPanelOrder(UINT8 *u1PanelOrder, UINT8 u1BlockNum);
void DRVCUST_GetLcdimPanelDimGain(UINT8 *u1PanelDimGain, UINT8 u1BlockNum);

/*****************************************************************************************/
/********************************   QUALITY TABLE    ***********************************/
/*****************************************************************************************/
void PE_InitUiRange(void);
void PE_SetUiPicMode(UINT8 u1PicMode);
void PE_SetUiRange(UINT8 u1VdpVisType, PE_ARG_TYPE ucArgType, PE_UI_RANGE_T *prUiRange);
UINT32 PE_GetUiRange(UINT8 u1VidPath, PE_ARG_TYPE ucArgType, PE_UI_RANGE_T *prUiRange);

void bApiSetPWMDft(void);
void bApiParseCustomQtyItem(void);
UINT8 bApiSetQtyDft(UINT8 u1SrcTypTmg, UINT32 u4UiItem, UINT16 u2Value);
UINT8 bApiGetCustomSourceTypeTiming(UINT8 bSrcTypTmg);
UINT8 u1ApiVidInSrcType2VisType(UINT8 u1SrcTypTmg);
UINT8 bApiGetQtyDft(UINT16 wSearchItem, UINT8 bQtyIdx);
UINT8 bApiGetQtyMin(UINT16 u2QtyItem);
UINT8 bApiGetQtyMax(UINT16 u2QtyItem);
UINT16 bApiQtyMapping(UINT8 u1QtyIndex, UINT16 u2QtyItem,
                     INT16 const i2Min, INT16 const i2Max,
                     INT16 const i2Dft, INT16 const i2Cur);

/*****************************************************************************************/
/************************************   FLASHPQ    *************************************/
/*****************************************************************************************/
void vApiFlashPqReadFlash(UINT8 bIndex, UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
void vApiFlashPqSetGammaId(UINT8 bIndex);
void vApiFlashPqSetSmartPicId(UINT8 bIndex);
void vApiFlashPqSetSceId(UINT8 bIndex);
void vApiFlashPqSetOdId(UINT8 bIndex);
UINT8 * bApiFlashPqGetVersion(UINT8 bIndex, UINT8 bType);
UINT8 bApiFlashPqVerifyHeader(UINT8 bIndex);
UINT8 vApiFlashPqGetSmartPicId(void);
UINT8 bApiFlashPqUpdateGamma(void);
UINT8 bApiFlashPqUpdateSCE(void);
UINT8 bApiFlashPqUpdateQtyDft(UINT8 bIndex);
UINT8 bApiFlashPqUpdateQtyMinMax(UINT8 u1VidPath, UINT8 bIndex);
UINT8 bApiFlashPqUpdateOD(void);
UINT8 bApiFlashPqUpdatePanelTable(void);
UINT16 vApiFlashPqGetGammaId(void);
UINT16 vApiFlashPqGetSceId(void);
UINT8 bApiFlashPqRawDataInit(void);
UINT16 u2ApiFlashPqGetRawDataTblId(void);
UINT16 u2ApiFlashPqGetRawDataCaseId(void);
void vApiFlashPqSetRawDataTblId(UINT16 u2Index);
void vApiFlashPqSetRawDataCaseId(UINT16 u2Index);
UINT8 bApiFlashPqUpdateRawDataEx(UINT16 u2TblId, UINT16 u2CaseId);
UINT8 bApiFlashPqGetRawDataInfo(RAWDATA_INFO* pstInfo);
UINT8 bApiFlashPqUpdateRawData(void);
UINT8 bApiFlashPqSetRawData(UINT16 u2RegNum, UINT32 *pRegTbl, UINT8* pDataTbl);
#ifdef SCE_BY_SOURCE
UINT8 bApiFlashPqIsSupportSCEbySource(UINT8 index);
UINT8 uApiFlashPqGetSceSrcNum(UINT8 index);
#endif
UINT16 wApiFlashPqGetSceIndexBySrcTiming(UINT16 u1SrcTypTmg);
//void vApiFlashPqSetTiming(UINT8 u1TypeTiming);
UINT16 vApiFlashPqGetMinSceId(void);
UINT16 vApiFlashPqGetQtyId(UINT8 bIndex) ;
UINT16 vApiFlashPqGetOdId(void);
UINT16 wApiFlashPqGetPanelId(void);
UINT8 bApiFlashPqSystemInfo(void);
#ifdef CC_ENABLE_MTK_MODEL_INDEX
UINT16 vApiFlashaqGetQtyId(UINT8 bIndex);
#endif

/*****************************************************************************************/
/*****************************    CUSTOMER DRIVER    *************************************/
/*****************************************************************************************/
void DRVCUST_AutoColorTempUpdate(AUTO_COLORTEMP_SETS_INFO_T *aCTInfo);
void DRVCUST_PANEL_GAMMA_REMAP(UINT32 u4GammaSel);
void DRVCUST_SET_GAMMA(void);
UINT8 DRVCUST_EepromReadByte(UINT16 waddr);
BOOL DRVCUST_EepromWriteByte(UINT16 waddr, UINT8 u1Data);
UINT8 DRVCUST_AutoNRDefSet(void);
UINT8 DRVCUST_NoiseLevel(void);
UINT32 DRVCUST_RGBOfstRemap(UINT32 u4Value);
void DRVCUST_SetODTable(void);
#ifdef SCE_BY_SOURCE
void DRVCUST_vMappingSCETiming(void);
UINT16 DRVCUST_wApiFlashPqGetSceIndexBySrcTiming(UINT16 u1SrcTypTmg);
#endif
void DRVCUST_SendEvent(E_CUST_EVENT eCustEvent, UINT8 u1Option);
void DRVCUST_SendEcsCommand(UINT8* pData, UINT32 u4Size);
UINT8 DRVCUST_GET_CTMode(void);
UINT8 DRVCUST_BE_Connect(UINT8 u1TxPath, UINT8 u1BeSclType , UINT8 u1PlaneOrder, UINT8 bPath);
UINT8 DRVCUST_BE_Disconnect(UINT8 u1TxPath, UINT8 bPath);
void DRVCUST_PQ_Associate(UINT8 bPqPath, UINT8 bPath);
void DRVCUST_PQ_Disassociate(UINT8 bPqPath);

BOOL DRVCUST_HDMIWithValidAviInfo(void);
BOOL DRVCUST_HDMIIsPhotoMode(void);
UINT8 DRVCUST_HDMIGetVideoColorSpace(void);
UINT8 DRVCUST_HDMIGetPhotoColorSpace(void);
UINT8 DRVCUST_HDMIGetColorimetry(void);
UINT8 DRVCUST_HDMIGetAspectRatio(void);

// Dynamic Backlight / Customer Backlight Driver
void DRVCUST_AutoBacklightUpdate(AUTO_BACKLIGHT_SETS_INFO_T *aBLight);
void DRVCUST_UpdateBacklightState(UINT8 u1State, UINT16 u2Duty);
void DRVCUST_SetBacklightDuty(UINT8 u1RefIndex, UINT16 u2Duty);
void DRVCUST_AdaptiveBacklightParam(UINT8 bMode);
void DRVCUST_AdaptiveBacklightInit(void);
void DRVCUST_AdaptiveBacklightProc(void);

/*****************************************************************************************/
/*********************************      NR     *****************************************/
/*****************************************************************************************/
void vApiVideo3DNROnOff(UINT8 bOnOff);
void vApiVideoNRTIIROnOff(UINT8 bOnOff);
void vApiVideoSRMNRYCOnOff(UINT8 bYOnOff, UINT8 bCOnOff);
void vApiVideo2DNROnOff(UINT8 bOnOff);
void vApiVideoMPEGNROnOff(UINT8 bOnOff);

/*****************************************************************************************/
/************************************  SHARPNESS  *****************************************/
/*****************************************************************************************/
void vHalVideoSharpnessOnOff(UINT8 bPath, UINT8 bOnOff);
void vHalVideoSharpnessHorizontalOnOff(UINT8 bPath, UINT8 bOnOff);
void vHalVideoSharpnessVerticalOnOff(UINT8 bPath, UINT8 bOnOff);
void vHalVideoSharpnessDiagonalOnOff(UINT8 bPath, UINT8 bOnOff);
void vHalVideoLTIOnOff(UINT8 u1VidPath, UINT8 bOnOff);
void vHalVideoLTIParam(UINT8 u1VidPath);
void vHalVideoSharpnessParam(UINT8 bPath);
void vHalVideoSharpnessParamHorizontal(UINT8 bPath);
void vHalVideoSharpnessParamVertical(UINT8 bPath);
void vHalVideoSharpnessParamDiagonal(UINT8 bPath);
void vHalVideoLTIParam(UINT8 u1VidPath);
void vHalVideoSuperResolutionOnOff(UINT8 bPath, UINT8 bOnOff);
/*****************************************************************************************/
/********************************  ADAPTIVE LUMA    **************************************/
/*****************************************************************************************/
void vHalVideoAdaptiveLumaOnOff(UINT8 bPath, UINT8 bOnOff);
void vHalVideoAdaptiveLumaParam(void);

/*****************************************************************************************/
/********************************  LOCAL DIMMING    **************************************/
/*****************************************************************************************/
void vHalVideoLcDimOnOff(UINT8 bPath, UINT8 bOnOff);

/*****************************************************************************************/
/******************   CONTRAST BRIGHTNESS SATURATION HUE**********************************/
/*****************************************************************************************/
void vHalVideoContrast(UINT8 bPath, UINT16 u2Value);
void vHalVideoBrightness(UINT8 bPath, UINT8 bValue);
void vHalVideoSaturation(UINT8 bPath, UINT16 u2Value);
void vHalVideoHue(UINT8 bPath, UINT8 bValue);

/*****************************************************************************************/
/*************************************   SCE   *******************************************/
/*****************************************************************************************/
void vHalVideoSCEOnOff(UINT8 bPath);
void vHalSetSCEPat(UINT8 bOnOff, UINT16 wYdata);
void vHalSetSCEAngular(UINT8 bmodule, UINT8 bColor, UINT8 bValue);

/*****************************************************************************************/
/************************************   CTI  *******************************************/
/*****************************************************************************************/
void vHalVideoCTIOnOff(UINT8 bPath, UINT8 bOnOff);
void vHalVideoCTIParam(UINT8 bPath);

/*****************************************************************************************/
/*******************************   RGB GAIN  & OFFSET ************************************/
/*****************************************************************************************/
void vHalVideoRGBGain(UINT8 bPath, UINT8 bRGB, UINT16 wValue);
void vHalVideoRGBOffset(UINT8 bPath, UINT8 bRGB, UINT16 wValue);
void vHalVideoRGBOffset2(UINT8 bPath, UINT8 bRGB, UINT16 wValue);

/**************************************************************************************/
/***********************************Bypass All PQ Item****************************************/
/**************************************************************************************/
void vApiVideoAllPQItemOnOff(UINT8 bOnOff);

/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
void vHalVideoGammaLoadTable(void);
void vHalVideoGammaLoadTableEX(void);
void vHalVideoGammaTblUpdate(UINT8 u1Mode);
void vHalVideoExternalGammaInit(void);

/*****************************************************************************************/
/************************************  Noise Reduction  **********************************/
/*****************************************************************************************/
void vHalVideo2DAutoNROnOff(UINT8 bOnOff);
void vHalVideo3DAutoNROnOff(UINT8 bOnOff);
void vHalVideo2DNRParam(UINT8 bCurUI);
void vHalVideo3DNRParam(UINT8 bCurUI);
void vHalVideo3DNRStrength(UINT8 u1CurUI);
void vHalVideoMPEGNRParam(UINT8 bCurUI);

/*****************************************************************************************/
/***************************************  White Peak  ************************************/
/*****************************************************************************************/
void vHalVideoWhitePeakLimit(UINT8 bOnOff);

/*****************************************************************************************/
/************************************  Adaptive Backlight  *******************************/
/*****************************************************************************************/
void vHalSetAdpBacklight(UINT8 u1Level, UINT8 u1Value);
UINT8 bHalGetAdpBacklight(UINT8 u1Level);

void vHalOutRGBColorOnOff(UINT8 bOnOff);
void vHalOutRGBColor(UINT8 RValue, UINT8 GValue, UINT8 BValue);

/*****************************************************************************************/
/********************************** BLUE STRETCH  ****************************************/
/*****************************************************************************************/
void vHalVideoBlueStretchOnOff(UINT8 bPath, UINT8 bOnOff);

/******************************************************************************************/
/****************************************  LED Motion  **************************************/
/******************************************************************************************/
void vHalVideoLEDMotion(UINT8 OnOff);
BOOL DRVCUST_GetLEDMotionFlag(void);


#ifdef PQ_SPLIT_DEMO
/*****************************************************************************************/
/********************************** Demo Mode Region  ****************************************/
/*****************************************************************************************/
UINT32 DRVCUST_DemoRegion(void);
UINT32 DRVCUST_DemoModule(void);
#endif

/*************************Local dimming Block Info Demo************************/
/**************************************************************************/
#ifdef SUPPORT_LCDIM_AVG_DEMO
void DRVCUST_LcDimBlkInit(UINT32 u4BlockNumH, UINT32 u4BlockNumV);
UINT8 DRVCUST_Get_BLBlockData(UINT8 * pu1BLBlockDataTbl);
void vDrvUpdateLcDimBlockContentInfo();
#endif

#endif /* _PE_IF_H_ */

