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

#ifndef _TVE_IF_H_
#define _TVE_IF_H_

#include "x_typedef.h"
#include "u_common.h"
#if (defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)|| defined(CC_MT5882))
#include "feature.h"
#endif

#if (defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5881) || defined(CC_MT5399)||defined(CC_MT5890)|| defined(CC_MT5882))
#define CC_TVE6595
#endif

#if(defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)|| defined(CC_MT5882))
#define CC_TVE_SUPPORT_VDAC108M 1
#endif

/* TVE OUT Config */

#ifndef CC_MT5387
#define CC_TVE_INSERT_VPS
#endif

//ITU_R BT. 470.  Offset = 9 pixels
#define VIDEO_ALIGNMENT_SUPPORT 1
#if VIDEO_ALIGNMENT_SUPPORT
#define VIDEO_ALIGNMENT_OFFSET 10
#endif

enum
{
	SV_TVE_NA,
	SV_TVE_DAC_BYPASS_CVBS,		
	SV_TVE_DAC_BYPASS_DEMOD,
	SV_TVE_DAC_PIP,
	SV_TVE_DAC_VIDEO_IN, // Video In
	SV_TVE_HW_BYPASS, // Only for TV SCART
	SV_TVE_D2S_BYPASS, // Only for Monitor SCART
	SV_TVE_DAC_BYPASS_SIF,
	SV_TVE_MAX
};


/* TVE Video Status Notify Type */
enum
{	
	SV_TVE_NOTIFY_VIDEO_NO_SIGNAL,
	SV_TVE_NOTIFY_VIDEO_STABLE_SIGNAL,		
	SV_TVE_NOTIFY_VIDEO_MODE_CHANGE,
	SV_TVE_NOTIFY_VIDEO_UNMUTE,
	SV_TVE_NOTIFY_MAX
};


/* Scart Out ID */
enum
{
	SV_TVE_SCART_OUT_1,
	SV_TVE_SCART_OUT_2,
	SV_TVE_SCART_OUT_MAX
};

/* Scart Out Fmt */
enum
{
	SV_TVE_OFMT_PAL,
	SV_TVE_OFMT_NTSC,
	SV_TVE_OFMT_MAX,
	SV_TVE_OFMT_NONE = SV_TVE_OFMT_MAX
};

enum
{
    DAC_1_4_CURRENT = 0,
    DAC_1_2_CURRENT,
    DAC_3_4_CURRENT,
    DAC_FULL_CURRENT
};


/* SCART Src type */
#define TVE_SCART_DECODER 7

typedef struct TveUsage_s
{
    UINT8    TveIsUsedBy : 4;
    UINT8    TveReferenceCnt : 4;
} TveUsage_t;

#define TVE_DAC_MUTE_MASK_MW  (1)
#define TVE_DAC_MUTE_MASK_DRV (1<<1)

#define TVE_BYP_EXT_PATH   (1)
#define TVE_BYP_AVMUX_PATH (2)
#define TVE_BYP_PIP_PATH   (3)

#define TVE_BYP_EXT_MUX_PATH (0x80)

#include "x_typedef.h"
#include "x_os.h"

/* Maximum number of plane mixer */
#define TVE_1				0	/* Main display */
#define TVE_2				1	/* Aux display */
#define TVE_MAX_NS			2

/* TVE configuration return value */
#define TVE_SET_ERROR		0
#define TVE_SET_OK			1
#define TVE_SET_VBI_OVERFLOW	2

/* TVE output */
#define TVE_OUT_CVBS		0
#define TVE_OUT_S_VIDEO	    1
#define TVE_OUT_YBR			2
#define TVE_OUT_RGB			3
#define TVE_MAX_OUT			4

/* TVE mode */
#define TVE_MOD_COMPOSITE		0
#define TVE_MOD_COMPONENT		1
#define TVE_MOD_RGB			    2
#define TVE_MOD_VGA             TVE_MOD_RGB
#define TVE_MAX_MOD			    3

/* TVE format */

// Plane mixer mode
#define PMX_MODE_480I			0
#define PMX_MODE_576I			1
#define PMX_MODE_480P			2
#define PMX_MODE_576P			3
#define PMX_MODE_720P			4
#define PMX_MODE_1080I			5

#define PMX_MODE_768P			6
#define PMX_MODE_540P			7
#define PMX_MODE_768I			8
#define PMX_MODE_1536I			9
#define PMX_MODE_720I			10
#define PMX_MODE_1440I			11
#define PMX_MODE_UNKNOWN		12

#define TVE_FMT_480I                    PMX_MODE_480I
#define TVE_FMT_576I                    PMX_MODE_576I
#define TVE_FMT_480P                    PMX_MODE_480P
#define TVE_FMT_576P                    PMX_MODE_576P
#define TVE_FMT_720P                    PMX_MODE_720P
#define TVE_FMT_1080I                   PMX_MODE_1080I
#define TVE_FMT_768P                    PMX_MODE_768P
#define TVE_MAX_FMT                     2

/* TVE field */
#define TVE_FIELD_ODD			0
#define TVE_FIELD_EVEN			1
#define TVE_FIELD_BOTH			2

/* TVE Macrovision APS types */
#define TVE_MV_APS_Type0		0
#define TVE_MV_APS_Type1		1
#define TVE_MV_APS_Type2		2
#define TVE_MV_APS_Type3		3
#define TVE_MAX_MV_APS			4

#define TVE_MAX_MV_TEST_CASES   6

/* TVE Low/High Impedence */
#define TVE_LOW_IMPEDENCE       0   /* for MT5351_DEMO_BOARD */
#define TVE_HIGH_IMPEDENCE      1   /* for MT5351_M1_BOARD */

/* TVE WSS/CGMS Aspect Ratio
   NOR: Normal, LB: LetterBox, C: Center, T: Top, LG: Larger than
   525-Line (CGMS) */
#define CGMS_NOR_4_3			0
#define CGMS_NOR_16_9			1
#define CGMS_LB_4_3             2
#define CGMS_LB_16_9			3
/* 625-Line (WSS)  */
#define WSS_LB_14_9_C			1
#define WSS_LB_14_9_T			2
#define WSS_LB_16_9_T			4
#define WSS_ANA_16_9			7
#define WSS_FULL_4_3			8
#define WSS_LB_16_9_C			11
#define WSS_LB_LG_16_9_C		13
#define WSS_FULL_14_9_C			14

/* TVE Mixer select */
#define TVE_VDOIN_MIXER	0
#define TVE_DEMOD_MIXER	1
#define TVE_SIF_MIXER		2

//TVE field invert WA
#if defined(CC_MT5363) || defined(CC_TVE6595)
#define TVE_WA
#endif

/******************************************************************************
* TVE CTRL API
******************************************************************************/
void vApiTVESetNotifyFunc(void (*pSigChgNfy)(UINT8,UINT8), void (*pUnmuteNfy)(UINT8));
void vApiTVEVideoStatusNotify(UINT8 bPath, UINT8 bMode);
// Cassy
void vApiTVEVideoMuteChangeNotify(UINT8 bPath, UINT8 bMute);
extern void vApiTVESetDacMuteMask(UINT8 u1DacId, UINT8 u1Mask, UINT8 u1OnOff);
extern UINT8 bApiTVEGetBypPath(UINT8 u1DacId);
extern void vApiTVESetScartOutCtrl(UINT8 u1Scart_id, UINT8 *u1SrcType, void* pv_extra_arg);
extern void vApiTVESetOutputFormat(UINT32 u4fmt);
void vApiTVEQueryStatus(void);

/* customization API */
extern void vApiTVEOutPathSelect(UINT8 out_id, UINT8 out_type);
extern void vApiTVEGetConfig(UINT8 u1SrcType, UINT8* pu1Type);

/******************************************************************************
* TVE API
******************************************************************************/
extern UINT32 TVE_Init(void);
extern UINT32 TVE_UnInit(void);
extern UINT32 TVE_Reset(UCHAR ucTveId);
extern UINT32 TVE_QueryStatus(void);

extern UINT32 TVE_SetEnable(UCHAR ucTveId, UCHAR ucEnable);
extern UINT32 TVE_GetEnable(UCHAR ucTveId, UCHAR* pucEnable);

extern UINT32 TVE_SetMode(UCHAR ucTveId, UCHAR ucMode);
extern UINT32 TVE_GetMode(UCHAR ucTveId, UCHAR* pucMode);

extern UINT32 TVE_SetFmt(UCHAR ucTveId, UCHAR ucFmt);
extern UINT32 TVE_GetFmt(UCHAR ucTveId, UCHAR* pucFmt);

extern UINT32 TVE_SetImpedence(UCHAR ucTveId, UCHAR ucImpedence);
extern UINT32 TVE_GetImpedence(UCHAR ucTveId, UCHAR* pucImpedence);

extern UINT32 TVE_SetCurrent(UCHAR ucDacId, UCHAR ucCurrent);

extern UINT32 TVE_SetColorBar(UCHAR ucTveId, UCHAR ucOn);
extern UINT32 TVE_SetCcEnable(UCHAR ucTveId, UCHAR ucEnable);
extern UINT32 TVE_GetCcEnable(UCHAR ucTveId, UCHAR* pucCcEnable);
extern UINT32 TVE_SendCc(UCHAR ucTveId, UCHAR ucField, const UCHAR *pucData, PTS_T u8Pts);
extern UINT32 TVE_SendCcIsr(UCHAR ucTveId, UCHAR ucIsOddField, UINT32 u4PmxPts);

extern UINT32 TVE_SetWssEnable(UCHAR ucTveId, UCHAR ucEnable);
extern UINT32 TVE_GetWssEnable(UCHAR ucTveId, UCHAR* pucWssEnable);
extern UINT32 TVE_SendWss(UCHAR ucTveId,const UCHAR *pu1Buf, UINT32 length);
extern UINT32 TVE_SetAspectRatio(UCHAR ucTveId, UCHAR ucAspectRatio);
extern UINT32 TVE_GetAspectRatio(UCHAR ucTveId, UCHAR* pucAspectRatio);
extern UINT32 TVE_SetCgmsaValue(UCHAR ucTveId, UCHAR ucValue);
extern UINT32 TVE_GetCgmsaValue(UCHAR ucTveId, UCHAR* pucValue);
#ifdef CC_TVE_INSERT_VPS//  for vps
extern UINT32 TVE_SetVPSEnable(UCHAR ucTveId, UCHAR ucEnable);
extern UINT32 TVE_GetVPSEnable(UCHAR ucTveId, UCHAR* pucVPSEnable);
extern UINT32 TVE_SendVPS(UCHAR ucTveId, const UCHAR *pucData);
#endif

extern UINT32 TVE_PlayCc(UCHAR ucTveId, UCHAR ucEsId);
extern UINT32 TVE_StopCc(UCHAR ucTveId, UCHAR ucEsId);
extern UINT32 TVE_SetTTXEnable(UCHAR ucTveId, UCHAR ucEnable);
extern UINT32 TVE_GetTTXEnable(UCHAR ucTveId, UCHAR* pucTTXEnable);
extern UINT32 TVE_SendTTX(UCHAR ucTveId,const UCHAR *pu1Buf, UINT32 length);

extern UINT32 TVE_DACPower(UCHAR ucTveId, UCHAR DAC, UCHAR ucEnable);
extern UINT32 TVE_OnInputVSync(void);
extern UINT32 TVE_OnOutputVSync(void);

extern void Bypass_Init(void);
extern UINT32 TVE_GetFieldInfo(UCHAR ucTveId);
extern void  _TVE_Mainloop(void);

extern UINT32 TVE_SelectInput(UCHAR ucTveSrc);
#if defined(CC_MT5363) || defined(CC_TVE6595)
extern UINT32 TVE_PIPMixer(UCHAR ucTveId, UCHAR ucEnable);
#endif
extern void _vApiTveSetSclPara(UINT8 u1OnOff, INT32 i4Width, INT32 i4Height);
#if defined(CC_SUPPORT_TVE) && defined(CC_SUPPORT_TVE_VIDEO_CLIP_BY_MTAL)
extern UINT32 TVE_SetHDispArea(UCHAR ucFmt, UCHAR bSrc, UINT16 u2BgnValue, UINT16 u2EndValue);
extern UINT32 TVE_SetVDispArea(UCHAR ucFmt, UINT16 u2BgnValue, UINT16 u2EndValue);
#endif
#if (defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)|| defined(CC_MT5882))
#if CC_TVE_SUPPORT_VDAC_SOFTWARE_CAL
extern UINT32 TveSetAbistBypass(UINT8 bPattern);
extern UINT32 TveVdacStartSoftCalibration(void);
#if (defined(CC_MT5890))
extern UINT32 TveSetAbistDcWaveFullCmd(void);
extern UINT32 TveSetAbistDcWaveZeroCmd(void);
extern UINT32 TveReadAdcValueCmd(void);
#endif

#endif
#endif

extern BOOL TVE_SetEnableBypassMonOut(UCHAR ucTveId, BOOL ucEnable);
extern BOOL TVE_GetEnableBypassMonOut(UCHAR ucTveId);

#endif /* _TVE_IF_H_ */

