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
 * $Date: 2015/03/31 $
 * $RCSfile: aud_dsp_cfg.c,v $
 * $Revision: #44 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_dsp_cfg.c
 *  Primitive APIs for audio and DSP configuration.
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
// KERNEL


LINT_EXT_HEADER_BEGIN
#include "x_util.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_hal_io.h"
#include "aud_dsp_cfg.h"
#include "aud_debug.h"
#include "aud_drv.h"
#include "aud_cfg.h"
#include "drv_adsp.h"

#include "dsp_intf.h"
#include "dsp_common.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
//#include "dsp_data.h"
#include "drv_adsp.h"
#include "aud_if.h"
//#include "aud_hw.h"
#include "drvcust_if.h"
#include "adac_if.h"
//#include "drv_dsp_cfg.h"
//#include "dsp_func.h"
#include "dsp_table.h"
#include "aud_hw.h"
#include "vdec_if.h"
#include "x_pinmux.h"

#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "codec_mt5398.h"
#ifdef CC_AUD_USE_NVM
#include "eeprom_if.h"
#endif


LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define CC_AUD_TV_ONLY_AGC
//#define CC_AUD_TPV_SPECIAL_AGC
//Capri and Mustang has the same apll setting
#if defined(CC_MT5399) || defined(CC_MT5882)
#define APLL2_PCW_ADJUST_LVL_48k 0x14d867c3
#define APLL2_PCW_ADJUST_LVL_44k 0x131205bc
#else
#define APLL2_PCW_ADJUST_LVL_48k 0X624DD2F1
#define APLL2_PCW_ADJUST_LVL_44k 0x5A5119CE
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// Audio channel number: AUD_CH_FRONT_LEFT = 0, AUD_CH_FRONT_RIGHT, AUD_CH_REAR_LEFT,
//                       AUD_CH_REAR_RIGHT, AUD_CH_CENTER, AUD_CH_SUB_WOOFER,
//                       AUD_CH_ALL

#define CHANNEL_VOL_MAX_SHM_VALUE   (0x20000)  /* 0 dB*/
#define MAX_VOL_LEVEL               (100)
#define VOL_ZERO_BASE               (0)

#define MAX_BALANCE_LEVEL           (100)

#define EQ_FLAG                     (1 << 0)
#define EQ_TREBLE_BOOST             (1 << 1)
#define EQ_SUPERBASS_BOOST          (1 << 2)
#define EQ_BASS_BOOST               (1 << 3)

#define VOL_SHM_0_DB                (0x20000)
#define VOL_SHM_NEG_6_DB            (0x20000/2)
#define VOL_SHM_NEG_12_DB           (0x20000/4)

#define INPUT_SRC_VOL_MAX           96
#define INPUT_SRC_VOL_MIN           -128

#define DSP_SHM_SPK_SIZE_MASK       ((UINT32)(0x1f) << 12)

/* used for ATV factory mode */
#define PAL_MASK (1 << 0)
#define A2_MASK (1 << 1)
#define MTS_MASK (1 << 2)
#define FMFM_MASK (1 << 3)
#define MTS_NEW_MASK (1 << 4)

#define AUD_ADPAN_CH_NUM    2

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define VOL_CTL_SEMA_LOCK(u1DecId)      VERIFY(x_sema_lock(_ahVolCtlSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK)
#define VOL_CTL_SEMA_UNLOCK(u1DecId)    VERIFY(x_sema_unlock(_ahVolCtlSema[u1DecId]) == OSR_OK)

UINT8 _au1MWSetSoundModeIdx = 0xff;
#ifdef CC_SONY_2K14_FLASH_AQ
UINT8 _au1MWSetPositionIdx = 0xff;
UINT8 _au1PositionCurrentIdx = 0xff;
UINT8 _au1MWSetVsModeIdx = 0xff;
#endif
UINT8 _u1MWSetPEQMode = 0xff;
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
UINT8 _au1VSCurrentIdx = 0xff;
UINT8 _au1MWSetVSIdx = 0xff;
#endif

typedef enum
{
    ADEC_LGSE_INIT_ONLY = 0, // init only parameter will be writen.
    ADEC_LGSE_VARIABLES = 1, // variables will be written.
    ADEC_LGSE_ALL   = 2 // init only parameter and variables wiil be written simultaneously
} ADEC_LGSE_DATA_MODE_T;

typedef enum
{
    SE2012_CLEARVOICE   = 0,
    SE2012_AUTOVOLUME = 1,
    SE2012_BASS              = 2,
    SE2012_DYNAMICEQ   =3,
    SE2012_PEQMODE1     = 4,
    SE2012_PEQMODE2     = 5,
    SE2012_PEQMODE3     = 6,
    SE2012_SURROUND     = 7,
    SE2012_FUNC_UNKNOWN   = 8,
    SE2012_FUNC_UNKNOWN1 = 9,
    SE2012_FUNC_SETTING     = 10,
    SE2012_FUNC_HARM     = 11,
    SE2012_FUNC_LGSEFN010 = 12
} SE2012_DDI_FUNCTION_T;

typedef enum
{
    SE2015_CLEARVOICE   = 0,
    SE2015_AUTOVOLUME = 1,
    SE2015_BASS              = 2,
    SE2015_DYNAMICEQ   =3,
    SE2015_PEQMODE1     = 4,
    SE2015_PEQMODE2     = 5,
    SE2015_PEQMODE3     = 6,
    SE2015_SURROUND     = 7,
    SE2015_VOLUME       = 8,
    SE2015_FUNC_UNKNOWN1 = 9,
    SE2015_FUNC_SETTING     = 10,
    SE2015_FUNC_HARM     = 11,
    SE2015_FUNC_LGSEFN010 = 12
} SE2015_HAL_FUNCTION_T;


typedef enum
{
	AUDIO_LGSE_LGSEFN000		= 0,
	AUDIO_LGSE_LGSEFN001		= 1,
	AUDIO_LGSE_LGSEFN003		= 2,
	AUDIO_LGSE_LGSEFN004		= 3,	//new webOS 1.5
	AUDIO_LGSE_LGSEFN005		= 4,	//new webOS 1.0
	AUDIO_LGSE_LGSEFN006		= 5,
	AUDIO_LGSE_LGSEFN007		= 6,	//old
	AUDIO_LGSE_LGSEFN008		= 7,
	AUDIO_LGSE_LGSEFN009		= 8,
	AUDIO_LGSE_LGSEFN010		= 9,
	AUDIO_LGSE_LGSEFN011		= 10,
	AUDIO_LGSE_LGSEFN012		= 11,
	AUDIO_LGSE_LGSEFN013		= 12,
	AUDIO_LGSE_LGSEFN014		= 13,	//new webOS 1.0
	AUDIO_LGSE_LGSEFN016		= 14,	//new webOS 1.0
	AUDIO_LGSE_LGSEFN017		= 15,	//new webOS 1.5
	AUDIO_LGSE_LGSEFN019		= 16,	//new webOS 1.5
	AUDIO_LGSE_LGSEFN020		= 17,	//new webOS 1.5
	AUDIO_LGSE_LGSEFN022		= 18,	//new webOS 1.5
	AUDIO_LGSE_LGSEFN023		= 19,	//new webOS 1.5
	AUDIO_LGSE_LGSEFN024		= 20,	//new webOS 1.5
	AUDIO_LGSE_LGSEFN004MODE1	= 21,
	AUDIO_LGSE_LGSEFN004MODE2	= 22,
	AUDIO_LGSE_LGSEFN004MODE3	= 23,
	AUDIO_LGSE_MODE 			= 24,
	AUDIO_LGSE_MAIN 			= 25,	//new webOS 1.0
	AUDIO_LGSE_GET_DATA         = 26,
	AUDIO_LGSE_FUNC_NUM
} AUDIO_LGSE_FUNCLIST_T;

typedef enum {
	HAL_APROC_LGSE_MAIN = 0,
	HAL_APROC_LGSE_FN000,
	HAL_APROC_LGSE_FN001,
	HAL_APROC_LGSE_FN004_MODE1,
	HAL_APROC_LGSE_FN004_MODE2,
	HAL_APROC_LGSE_FN009,
	HAL_APROC_LGSE_FN010,
	HAL_APROC_LGSE_MODE,
	HAL_APROC_LGSE_GET_DATA,
	HAL_APROC_LGSE_NUM
} HAL_LGSE_MAPPING_T; // From ARM 11 mapping 

typedef enum
{
    ADEC_LGSE_MODE_VARIABLES0   = 0, // VARIABLES_00 will be writen.
    ADEC_LGSE_MODE_VARIABLES1   = 1, // VARIABLES_01 will be written.
    ADEC_LGSE_MODE_VARIABLES2   = 2, // VARIABLES_02 will be written.
    ADEC_LGSE_MODE_VARIABLES3   = 3, // VARIABLES_03 will be written.
    ADEC_LGSE_MODE_VARIABLES4   = 4, // VARIABLES_04 will be written.
    ADEC_LGSE_MODE_VARIABLESALL = 5 // All VARIABLES will be written simultaneously. Data will be arranged from 0 to 4.
} ADEC_LGSE_VARIABLE_MODE_T;

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
typedef enum {
    APROC_RAW_UNKNOWN = 0,
    APROC_RAW_PCM,
    APROC_RAW_AC3,
    APROC_RAW_AAC,
    APROC_RAW_DTS_512,
    APROC_RAW_DTS_1024,
    APROC_RAW_DTS_2048,
    APROC_RAW_NUM
} APROC_IEC_AUDFMT_T;
#endif

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern void bHDMIAudioSigmaDeltaFineTuneSetting(UINT32 value1, UINT8 value2, UINT32 target, UINT8 bSDINOVR);
extern UINT32 DSP_GetAtvTvSystemStrength(TV_AUD_SYS_T eAudSys);
#ifdef SUPPORT_PHILIPS_LOUDNESS
extern void AUD_DspChEqLoudness(UINT8 u1DecId, INT8 *prEqvalue);
#endif
extern void vHDMISpdifOutEnable(BOOL fgEnable);
extern void vHDMIAudChannelStatus(BOOL* pfgCopyright, UINT8* pu1CategoryCode,
                                UINT8* p1WordLength, BOOL* pfgIsPcm);
extern AUD_WMAErr_NOTIFY _hWMAErrNotifyFunc;
#ifdef CC_53XX_SWDMX_V2
extern VOID *_prWMAErrInst;  // megaa 20090828
#endif
extern UINT32 u4ReadDspSram(UINT8 u1DspId, UINT32 u4Addr);
extern BOOL _fgHpPlugFlag;
extern void vAudPostSetParas(
        SE2012_DDI_FUNCTION_T u4fnid,
        UINT32 *pParams, UINT16 noParam,
        ADEC_LGSE_DATA_MODE_T dataOption,
        ADEC_LGSE_VARIABLE_MODE_T varOption);
extern INT16 AUD_GetAudioVdecDelay(ENUM_VDEC_FMT_T eCodecType);

extern BOOL _IsUseSTA381(void);
extern BOOL _IsUseDigiAmpTAS5717(void);
extern BOOL _IsUseApogee(void);

#ifdef CC_AUD_USE_NVM
extern INT32 EEPROM_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
extern INT32 EEPROM_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
extern UINT32 DSP_GetFineTuneVol(UINT8 u1Idx);
#endif

#ifdef CC_LGSE_WORK_AROUND
extern void DSP_LGSEFN009_Hack(UINT32 *pParams, UINT16 noParam, UINT16 dataOption);
#endif

#ifdef CC_AUD_ARM_POST    
extern void AUD_AprocReconfig(BOOL fgEnable);
#endif


extern BOOL _AUD_GetNotifyFunc(AudDecNfyFct * pfNfyFunc);

#ifdef CC_SUPPORT_STR
extern BOOL fgEQSpetrumInfoStrResumeMode;
#endif

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
extern void vAprocCmdSet (UINT32 u4Idx, UINT32 u4Data0, UINT32 u4Data1);
extern void _vAprocSetRoutine (UINT32 u4Id);
extern void AUD_Aproc_Chk_DecId(UINT8* u1DecId);
#endif
#ifdef CC_AUD_ARM_SUPPORT
extern INT32 _gi4StcDiff[AUD_DEC_NUM];
#endif
//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static AUD_DEC_STREAM_FROM_T _AudDspEuAtvStrmSrcTransfer(AUD_DEC_STREAM_FROM_T eStreamFrom);
#define AUD_EU_ATV_STREAM_SRC_TRANSFER(eStrSrc) _AudDspEuAtvStrmSrcTransfer(eStrSrc)

#ifdef CC_AUD_USE_NVM
static UINT32 AUD_NVM_Init_To_EEPROM(void);
#endif

#if 1//def CC_AUD_PEQ_SUPPORT
#ifdef CC_AUD_DIFFER_PEQ_GAIN_FOR_VOL
static INT16 _AudDspGetPeqGain(UINT8 u1DecId, UINT8 u1ModeIdx, UINT8 u1PEQSet);
#endif
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef CC_AUD_MIXSOUND_SUPPORT
UINT32 _u4MixsoundVol = 10;
UINT32 _u4MixsoundLoop = 64;
#endif

// Dolby banner
static BOOL  fgBannerEnable = FALSE;

static HANDLE_T _ahVolCtlSema[AUD_DEC_NUM];
static HANDLE_T _ahEqCtlSema;
static HANDLE_T _ahChDelayCtlSema;
#ifdef CC_AUD_SOUND_MODE
static HANDLE_T _ahPSMCtlSema;
#endif
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
static HANDLE_T _ahVSMCtlSema;
#endif
static HANDLE_T _ahSpdifCtlSema;
static HANDLE_T _hAsrcControlSema;
#if 0 //ndef CC_MT5391_AUD_3_DECODER
static UINT32   _au4AdjVol[AUD_DEC_NUM] = {VOL_SHM_0_DB, VOL_SHM_0_DB};
#else
static UINT32   _au4AdjVol[AUD_DEC_NUM] = {VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB};
#endif

AUD_STC_ID _au1DspStcId[AUD_DEC_MAX] = {AUD_STC_AUDIO,AUD_STC_AUDIO,AUD_STC_AUDIO,AUD_STC_AUDIO}; // 2_STC_Task, default is 0 (as original STC_SRC_A1)

// Customization table, will load via DRVCUST_OptQuery() function
#if 1 //20120801, wonder, to clean warning msg.
static INT16  _aai2InputSrcVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] = {{VOL_0_DB,},};
#else
static INT16  _aai2InputSrcVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] = {VOL_0_DB};
#endif
#ifdef CC_AUD_INPUT_SRC_VOL_OFFSET
static INT16  _aai2InputSrcVolOffset[AUD_DEC_NUM][AUD_STREAM_FROM_NUM] = {{VOL_0_DB,},};
#endif
static UINT32 _aau4InputSrcAgcVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM];
static INT16  _ai2AvInputSrcVol[AUD_INPUT_ID_MAX];
static BOOL   _aafgInputSrcAgcFlag[AUD_DEC_NUM][AUD_STREAM_FROM_NUM];
static UINT8 _u1VolBaseLevel;
#ifdef CC_AUD_NEW_VOL_BASE_LEVEL
static UINT8 _u1NewVolBaseLevel = MAX_VOL_LEVEL;
#endif

static UINT32 _au4VOLUME[MAX_VOL_LEVEL + 1];
#ifdef CC_AUD_NEW_CV_TABLE
#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
static UINT32 _au4CHVOLUME[MAX_VOL_LEVEL + 1];
#else
static UINT32 _au4CHVOLUME[AUD_NVM_VOL_TBL_NUM][MAX_VOL_LEVEL + 1];
#endif
#endif
#ifdef CC_AUD_NEW_HP_VOL_TABLE
static UINT32 _au4HpVolume[MAX_VOL_LEVEL + 1];
#endif
#if 0 //def CC_ENABLE_AOMX
static UINT32 _au4LGSEVOLUME[MAX_VOL_LEVEL + 1] =
{
    0x0,0x20,0x40,0x92,0x130,0x20e,0x328,0x459,0x666,0x900,
    0xc04,0x1296,0x1632,0x1b72,0x1f59,0x2441,0x2840,0x2c5c,0x2f98,0x336d,
    0x3786,0x395f,0x3ca6,0x4058,0x4361,0x47be,0x49d2,0x4bf2,0x4e53,0x5093,
    0x53fc,0x554a,0x57fa,0x5a58,0x5d26,0x5f43,0x61d1,0x6351,0x679a,0x68d2,
    0x6af5,0x6ce3,0x6dec,0x6f7a,0x715b,0x7355,0x74c9,0x7713,0x77b3,0x7a35,
    0x7bb6,0x7d3a,0x800b,0x8300,0x8478,0x8784,0x8a91,0x8dcc,0x8f47,0x927d,
    0x9600,0x9978,0x9cf4,0xa090,0xa25b,0xa60c,0xa9d2,0xadb8,0xb1b9,0xb7d0,
    0xbbfc,0xc040,0xc6dc,0xcb5b,0xd250,0xd70a,0xdbd9,0xe355,0xeb17,0xf320,
    0xfb63,0x106ee,0x10fee,0x11946,0x12300,0x12d0e,0x13778,0x14231,0x14d4d,0x158d7,
    0x164ae,0x170b9,0x17d1b,0x189e6,0x1971b,0x1a4b4,0x1b2b7,0x1c132,0x1d017,0x1df88,
    0x1e9bf,
};
#endif
#ifdef CC_AUD_SOUND_MODE
static UINT16 _aai1PeqFq[AUD_SM_NUM][PEQ_SETTING_MAX];
static UINT16 _aai1PeqBw[AUD_SM_NUM][PEQ_SETTING_MAX];
static INT8 _aai1PeqGain[AUD_SM_NUM][PEQ_SETTING_MAX];
static INT8 _aai1BassTreble[AUD_SM_NUM][2];
#endif

#if 1//def CC_AUD_PEQ_SUPPORT
static AUD_PEQ_CFG_T _arPEQTable[AUD_PEQ_CASE_NUM][PEQ_SETTING_MAX];
#ifdef CC_AUD_DIFFER_PEQ_GAIN_FOR_VOL
static INT16 _ai2PEQGainTable[2][PEQ_GAIN_NUM_FOR_VOL];
#endif
#endif

static AUD_POST_SM_T _arSoundModeCase[AUD_SOUND_MODE_NUM][AUD_STREAM_FROM_NUM];
static UINT8 _au1SoundModeCurrentIdx = 0xff;

static AUD_DEC_OUTPORT_MODE_T _aeOutPortMode = AUD_DEC_OUTPORT_MODE_UNKNOW;
#ifdef CC_AUD_VOLUME_OFFSET
static INT8 _aai1ChannelVolOffset[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static INT32 _ai4VolOffsetMap[AUD_VOLUME_OFFSET_IDX_NUM] = {-18, -15, -12, -9, -6, -3, 0, 3, 6, 9, 12, 15, 18};
#ifdef CC_S_SUBWOFFER_SUPPORT
static INT32 _ai4SWVolOffsetMap[AUD_SW_VOLUME_OFFSET_IDX_NUM] = 
{
    -24, -22, -20, -18, -16, -14, -12, -10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24
};
#endif
#endif
static INT8   _aai1ChEqCfg[AUD_EQ_NUM][CH_EQ_BAND_NO+1];
static INT8   _aai1ChEqCfgPrev[CH_EQ_BAND_NO+1];
#ifdef CC_AUD_EQ_BASED_SBASS
static INT8   _ai1ChEqFinalCfg[CH_EQ_BAND_NO+1];
#endif
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
static INT16  _aai2InputSrcLineOutVol[AUD_DEC_NUM][AUD_STREAM_FROM_NUM];
#endif
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
static UINT32 _aai1VSMode[AUD_VS_NUM][AUD_SUR_PRAR_NUM];
#endif
static UINT8 _au1ChDelay[AUD_STREAM_FROM_NUM];
static INT16 _ai2AvcTargetLevel[AUD_STREAM_FROM_NUM];
static INT16 _ai2AvcGainUp[AUD_STREAM_FROM_NUM];
static INT16 _ai2AvcAdjRate[AUD_STREAM_FROM_NUM];
static INT16 _ai2AvcUiAdjRate[AUD_STREAM_FROM_NUM];
//static UINT32 _aai1InputSrcGain[AUD_STREAM_FROM_NUM] = {VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB,
//                                                      VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB};

// For SPDIF
typedef struct _AUD_SPDIF_INFO_T
{
    UINT8 u1Copyright[SPDIF_REG_TYPE_NUM][AUD_STREAM_FROM_NUM];
    UINT8 u1Category[SPDIF_REG_TYPE_NUM][AUD_STREAM_FROM_NUM];
    UINT8 u1WordLength[SPDIF_REG_TYPE_NUM][AUD_STREAM_FROM_NUM];
    UINT8 eUpdateMode[SPDIF_REG_TYPE_NUM][AUD_STREAM_FROM_NUM];
}   AUD_SPDIF_INFO_T;

#ifdef CC_SUPPORT_STR
typedef struct _AUD_RESUME_POST_INFO_T
{
    BOOL fgBassEnable;
    BOOL fgTrebleEnable;
    BOOL fgVirSur1Enable;
    BOOL fgVirSur2Enable;
    BOOL fgEQEnable;
    BOOL fgADSpkEnable;
    BOOL fgADHpEnable;
    BOOL fgSpkHpEnable;
    BOOL fgAvcEnable;
    UINT8 u1DownmixType[3];
    UINT8 u1DRCMode[3];
    UINT8 u1DualmonoMode;
    BOOL fgNonEUmode;
}   AUD_RESUME_Post_INFO_T;

static AUD_RESUME_Post_INFO_T _aResumePostInfo;
#endif

static AUD_SPDIF_INFO_T _aSpdifInfo;
/*Aproc Bmang*/
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
static APROC_DSP_BMANG_SET_T _aAprocDspBmangSetInfo;
//static APROC_DSP_MVS_SET_T _aAprocDspMVSSetInfo;  //defined but not used??
//static APROC_DSP_MVB_SET_T _aAprocDspMVBSetInfo;
//static APROC_DSP_LIMT_SET_T _aAprocDspLimitSetInfo[APROC_LIMITER_IDX_NUM];
#endif

//static BOOL   _aafgPlayMuteSetSpdifFlag=TRUE;
static BOOL   _aafgHdmiSetSpdifFlag=TRUE;
static BOOL   _aafgMWSetSpdifFlag=TRUE;
static AUD_IEC_T _aeMWIec;
#ifdef CC_AUD_LINEOUT_VOL_MODE
static UINT8  _aafgLineOutVolChangeFlag = 0;
static UINT8  _aau1LineOutFixVol;
#endif

#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
static UINT8  _aau1TVFineTuneVolume[SV_AUD_SYS_NUM];
#endif

static BOOL  _fgVideoScrambled = FALSE;

// used for ATV factory mode fine tune volume
UINT8 _u1MtsFineVol=20;
UINT8 _u1SapFineVol=20;
static UINT8 _u1A2FineVol=20;
static UINT8 _u1PalFineVol=20;
static UINT8 _u1AMFineVol=20;
static UINT8 _u1NicamFineVol=20;
#if 0   // Unused
static UINT8 _u1FmfmMonoFineVol=20;
static UINT8 _u1FmfmDualFineVol=20;
#endif

#ifdef CC_AUD_BBE_SUPPORT
static UINT8 _u1BbeMode = 0;
static INT8 _i1BbeLevel=0;
static UINT8 _u1BbeProcess=7*2;
static UINT8 _u1BbeLoContour=7*2;
static UINT8 _u1Bbe3dGain=80;
#endif
#ifdef CC_AUD_DDI
static UINT8 SpdifSoundBarId = 0;
static UINT8 SpdifSoundBarData = 0;
UINT8 _u1SpdifRawDec = AUD_DEC_MAIN; 
#endif

static UINT8 u1HdevMode = 0;
#ifdef CC_AUD_MIXSOUND_SUPPORT
BOOL _fgMixSndFlag = FALSE;
BOOL _fgMixSndMute = FALSE;
UINT8 _u1MixSndClipIdx = 0xff;
#endif

#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
static BOOL _afgAudRegDetection = FALSE;
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
// Q index for on/off
static UINT8 _au1CDNotchQIdx[2] = {16, 0};
static UINT16 _au1CDNotchFc[2] = {1000, 0};
#endif
//-127dB~0dB(0x00~0x7F)
static const UINT32 _au4CHVOLUMENEW[256] =
{
    0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,
    0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,
    0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,0x0000000,
    0x0000000,0x0000001,0x0000001,0x0000001,0x0000001,0x0000001,0x0000001,0x0000002,
    0x0000002,0x0000002,0x0000002,0x0000003,0x0000003,0x0000004,0x0000004,0x0000005,
    0x0000005,0x0000006,0x0000007,0x0000008,0x0000009,0x000000a,0x000000b,0x000000d,
    0x000000e,0x0000010,0x0000012,0x0000014,0x0000017,0x000001a,0x000001d,0x0000020,
    0x0000024,0x0000029,0x000002e,0x0000034,0x000003a,0x0000041,0x0000049,0x0000052,
    0x000005c,0x0000068,0x0000074,0x0000083,0x0000093,0x00000a5,0x00000b9,0x00000cf,
    0x00000e9,0x0000105,0x0000125,0x0000149,0x0000171,0x000019e,0x00001d1,0x0000209,
    0x0000249,0x0000290,0x00002e1,0x000033b,0x000039f,0x0000411,0x0000490,0x000051e,
    0x00005be,0x0000672,0x000073b,0x000081d,0x000091a,0x0000a37,0x0000b76,0x0000cdc,
    0x0000e6e,0x0001030,0x000122a,0x0001462,0x00016de,0x00019a9,0x0001cca,0x000204e,
    0x000243f,0x00028ab,0x0002da1,0x0003333,0x0003972,0x0004074,0x0004852,0x0005125,
    0x0005b0c,0x0006628,0x000729f,0x000809b,0x000904d,0x000a1e8,0x000b5aa,0x000cbd4,
    0x000e4b3,0x001009b,0x0011feb,0x001430c,0x0016a77,0x00196b2,0x001c852,0x0020000,
    0x0023e79,0x0028491,0x002d338,0x0032b77,0x0038e7a,0x003fd93,0x0047a39,0x0050615,
    0x005a303,0x0065316,0x00718a5,0x007f64f,0x008ef05,0x00a0614,0x00b3f30,0x00c9e80,
    0x00e28ae,0x00fe2f5,0x011d334,0x0140000,0x01670bc,0x0092db2,0x01c4031,0x01fb2a7,
    0x02390ca,0x027e7be,0x02cc640,0x0323cdb,0x0385e1e,0x03f3edc,0x046f672,0x04f9f16,
    0x0595633,0x0643cc9,0x07077e0,0x07e3104,0x08d96d2,0x09edd9b,0x0b2400c,0x0c80000,
    0x0e06758,0x0fbc8fb,0x11a81ec,0x13cfa88,0x163a7e7,0x18f0d6e,0x1bfbe85,0x1f66095,
    0x233ad34,0x278749d,0x2c5a074,0x31c36df,0x37d5dff,0x3ea5fdd,0x464aec2,0x4edea29,
    0x587e43d,0x634a80e,0x6f6807a,0x7d00000,0x8c40972,0x9d5d9cf,0xb091339,0xc61c950,
    0xde48f0e,0xf96864e,0x117d7139,0x139fc5da,0x1604c40f,0x18b48e29,0x1bb8448d,0x1f1a24b6,
    0x22e5abfd,0x2727bea2,0x2beed395,0x314b259a,0x374eea6a,0x3e0e908f,0x45a104cd,0x4e200000,
    0x57a85e7c,0x625a821b,0x6e5ac03a,0x7bd1dd25,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,
    0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,
    0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,
    0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,
    0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,
    0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff
};
//0~15/16dB(0x0~0xF)
static const UINT32 _au4CHVOLUMENEW1[16] =
{
    0x0020000,0x00203b2,0x002076b,0x0020b2c,0x0020ef3,0x00212c1,0x0021696,0x0021a73,
    0x0021e56,0x0022241,0x0022632,0x0022a2c,0x0022e2c,0x0023234,0x0023643,0x0023a5a
};

static const UINT32 _au4Ac3DrcRange[DRC_RANGE_MAX] =
{
    0x00000000,0x000FFFFF,0x001FFFFF,0x002FFFFF,0x003FFFFF,0x004FFFFF,0x005FFFFF,
    0x006FFFFF,0x007FFFFF
};

/*
static const UINT32 _au4VirSurGain[VSURR_GAIN_MAX] =
{
    0x00001031,0x00001462,0x000019A9,0x0000204E,0x000028AB,0x00003333,0x00004075,0x00005126,
    0x00006628,0x0000809C,0x0000A1E9,0x0000CBD5,0x0001009C,0x0001430D,0x000196B2,0x00020000
};
*/

static const REVERB_MODE_T _arReverMode[AUD_REVERB_MODE_MAX] = {
    {0x200000, 79, 67, 59, 47},     /* 01 Concert */
    {0x180000, 41, 31, 23, 13},     /* 02 Living Room */
    {0x300000, 79, 59, 37, 19},     /* 03 Hall */
    {0x400000, 19, 17, 11,  7},     /* 04 Bathroom */
    {0x600000, 59, 47, 37, 23},     /* 05 Cave */
    {0x100000, 79, 73, 71, 67},     /* 06 Arena */
    {0x500000, 73, 67, 31, 11}      /* 07 Church */
};

// -10 ~ +10 dB, 0.5 dB/step
static const UINT32 _au4DryGain[EQ_DRY_GAIN_INDEX_MAX] =
{
    0x000050f4, 0x000055c0, 0x00005ad5, 0x00006037, 0x000065ea, 0x00006bf4, 0x0000725a, 0x00007920,
    0x0000804e, 0x000087e8, 0x00008ff6, 0x0000987d, 0x0000a186, 0x0000ab19, 0x0000b53c, 0x0000bff9,
    0x0000cb59, 0x0000d766, 0x0000e429, 0x0000f1ae, 0x00010000, 0x00010f2b, 0x00011f3d, 0x00013042,
    0x00014249, 0x00015562, 0x0001699c, 0x00017f09, 0x000195bc, 0x0001adc6, 0x0001c73d, 0x0001e237,
    0x0001feca, 0x00021d0e, 0x00023d1d, 0x00025f12, 0x0002830b, 0x0002a925, 0x0002d182, 0x0002fc42,
    0x0003298b
};

#ifdef CC_AUD_7BAND_EQUALIZER
static const UINT8 _aau1EqBandExtend[CH_EQ_BAND_NO][2] =
{
    {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}
};
#else
static const UINT8 _aau1EqBandExtend[CH_EQ_BAND_NO][2] =
{
    {0, 1}, {1, 2}, {3, 2}, {5, 2}, {7, 1}
};
#endif

static const AUD_EQ_BAND_INFO_T _arEqInfo[] =
{
    {0, 20, -20, 20, 200},      // 100 Hz
    {1, 20, -20, 200, 500},     // 300 Hz
    {2, 20, -20, 500, 2000},    // 1K Hz
    {3, 20, -20, 2000, 6000},   // 3K Hz
    {4, 20, -20, 6000, 20000},  // 10K Hz
};

static PL2_SETTING_T _rPL2Setting;

// Channel volume relative
static UINT8 _aau1ChannelVolume[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#ifdef CC_AUD_DDI
static UINT32 _aau1ChannelVolumeNew[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static UINT32 _aau1ChannelVolumeMain[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static UINT32 _aau1ChannelVolumeFine[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#endif
static UINT8 _u1AdFadeVolume;
static UINT8 _u1AdFadeVolumePrev;
static UINT8 _aau1AdPanVol[AUD_ADPAN_CH_NUM];
static UINT8 _aau1AdPanVolPrev[AUD_ADPAN_CH_NUM];
static UINT16 _u2AdFadeMode;
static INT8 _aai1ChlVolExtraGain[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static INT8 _aai1ChlVolDolbyGainTB11[(UINT32)AUD_CH_NUM]; // Dolby Technical Bulletin 11.5
static INT8 _aai1ChlVolExtraGainNonLineIn[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static UINT32 _aau4ChannelVolumeRawData[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static BOOL  _aafgChlMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static BOOL  _aafgChlOutPort[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static BOOL _aafgSoundModeMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static BOOL  _aafgChlHdmiChgFmtMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static BOOL _aafgSurroundMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
static BOOL _aafgVBassMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#ifdef CC_AUD_HDMI_SPDIF_CFG
static BOOL  _aafgChlHdmiMuteAnalogOut[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#endif
static BOOL  _aafgChDecPlayMute[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#ifdef CC_AUD_LINEOUT_VOL_OFFSET
static UINT8 _aau1ChannelVolOffset[AUD_DEC_NUM][(UINT32)AUD_CH_NUM];
#endif
static AUD_IEC_T _aeIecMode = AUD_IEC_CFG_PCM;
static BOOL _aeVolMuteFlag = TRUE;
static AUD_IEC_RAWSRC_T _aeIecRawSrc = IEC_RAW_SRC_OFF;
static BOOL _afgIecEnable = TRUE;
static BOOL _afgIecMute = FALSE;
static AUD_IECINFO_T _gSetIecInfo;

static AUD_EQ_TYPE_T _eEqMode = AUD_EQ_OFF;

#if 0 //ndef CC_MT5391_AUD_3_DECODER
static UINT32 _au1Balance[2] = {(MAX_BALANCE_LEVEL/2), (MAX_BALANCE_LEVEL/2)};
static BOOL _afgMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE};
static BOOL _afgStopMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE};
static BOOL _afgHdmiModeMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE};
static BOOL _afgAgcEnable[AUD_DEC_NUM] = {TRUE, FALSE};
static BOOL _afgUserAgcFlag[AUD_DEC_NUM] = {TRUE, FALSE};
static AUD_DEC_STREAM_FROM_T _aeStreamSrc[AUD_DEC_NUM] = {AUD_STREAM_FROM_OTHERS, AUD_STREAM_FROM_OTHERS};
static BOOL _afgAllChannelMute[AUD_DEC_NUM] = {FALSE, FALSE};
#else
#ifdef CC_AUD_SX1_FEATURE
static INT32 _aau4AdLvlAdj=15;
#endif
static UINT32 _au1Balance[3] = {(MAX_BALANCE_LEVEL/2), (MAX_BALANCE_LEVEL/2), (MAX_BALANCE_LEVEL/2)};
static BOOL _afgMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE, FALSE};
static BOOL _afgStopMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE, FALSE};
static BOOL _afgHdmiModeMuteEnable[AUD_DEC_NUM] = {FALSE, FALSE, FALSE};
static BOOL _afgAgcEnable[AUD_DEC_NUM] = {TRUE, FALSE, FALSE};
static BOOL _afgUserAgcFlag[AUD_DEC_NUM] = {TRUE, FALSE, FALSE};
static AUD_DEC_STREAM_FROM_T _aeStreamSrc[AUD_DEC_NUM] = {AUD_STREAM_FROM_OTHERS, AUD_STREAM_FROM_OTHERS,AUD_STREAM_FROM_OTHERS};
static BOOL _afgAllChannelMute[AUD_DEC_NUM] = {FALSE, FALSE, FALSE};
#endif

static BOOL _afgOutPortVolMute[AUD_OUT_PORT_NUM] = {FALSE};
static BOOL _afgOutPortHwMute[AUD_OUT_PORT_NUM] = {FALSE};
static BOOL _afgOutPortMute[AUD_OUT_PORT_NUM] = {FALSE};

#ifdef CC_MPEG_LEVEL_ADJ
static INT32 _aau1MpgLvlAdj=0;
#endif
#ifdef CC_AUD_SX1_FEATURE
// Negtive dB table, in steps of 0.1 dB
// 0dB, -0.1dB, -0.2dB, -0.3dB, -0.4dB, -0.5dB,.....
static const UINT32 _au4NegDBTablePEQ[] = {
    0x0020000,0x001fa23,0x001f458,0x001ee9e,0x001e8f4,0x001e35b,
    0x001ddd3,0x001d85b,0x001d2f3,0x001cd9a,0x001c852,0x001c318,
    0x001bdee,0x001b8d4,0x001b3c8,0x001aecb,0x001a9dc,0x001a4fc,
    0x001a02b,0x0019b67,0x00196b2,0x001920a,0x0018d70,0x00188e3,
    0x0018464,0x0017ff2,0x0017b8d,0x0017734,0x00172e9,0x0016eaa,
    0x0016a77,0x0016651,0x0016237,0x0015e29,0x0015a27,0x0015631,
    0x0015246,0x0014e67,0x0014a93,0x00146ca,0x001430c,0x0013f5a,
    0x0013bb2,0x0013815,0x0013482,0x00130fa,0x0012d7c,0x0012a09,
    0x00126a0,0x0012340,0x0011feb,0x0011c9f,0x001195d,0x0011624,
    0x00112f5,0x0010fd0,0x0010cb3,0x00109a0,0x0010695,0x0010394,
    0x001009b,0x000fdab,0x000fac4,0x000f7e5,0x000f50f,0x000f240,
    0x000ef7b,0x000ecbd,0x000ea07,0x000e759,0x000e4b3,0x000e215,
    0x000df7f,0x000dcf0,0x000da68,0x000d7e8,0x000d56f,0x000d2fe,
    0x000d094,0x000ce30,0x000cbd4,0x000c97f,0x000c730,0x000c4e9,
    0x000c2a8,0x000c06d,0x000be39,0x000bc0c,0x000b9e5,0x000b7c4,
    0x000b5aa,0x000b395,0x000b187,0x000af7f,0x000ad7c,0x000ab80,
    0x000a98a,0x000a799,0x000a5ae,0x000a3c8,0x000a1e8,0x000a00e,
    0x0009e39,0x0009c69,0x0009a9f,0x00098d9,0x000971a,0x000955f,
    0x00093a9,0x00091f8,0x000904d,0x0008ea6,0x0008d04,0x0008b66,
    0x00089ce,0x000883a,0x00086ab,0x0008520,0x000839a,0x0008219,
    0x000809b
};

// Postive dB table, in steps of 0.1 dB
// 0dB, 0.1dB, 0.2dB, 0.3dB, 0.4dB, 0.5dB,.....
static const UINT32 _au4PosDBTablePEQ[] = {
    0x0020000,0x00205ed,0x0020bed,0x00211fe,0x0021821,0x0021e56,
    0x002249e,0x0022af8,0x0023165,0x00237e5,0x0023e79,0x0024520,
    0x0024bda,0x00252a9,0x002598c,0x0026083,0x002678f,0x0026eaf,
    0x00275e5,0x0027d31,0x0028491,0x0028c08,0x0029395,0x0029b38,
    0x002a2f2,0x002aac3,0x002b2ab,0x002baaa,0x002c2c1,0x002caf0,
    0x002d338,0x002db97,0x002e410,0x002eca2,0x002f54d,0x002fe12,
    0x00306f1,0x0030fea,0x00318fe,0x003222d,0x0032b77,0x00334dc,
    0x0033e5d,0x00347fb,0x00351b5,0x0035b8c,0x0036580,0x0036f91,
    0x00379c0,0x003840e,0x0038e7a,0x0039905,0x003a3af,0x003ae79,
    0x003b963,0x003c46d,0x003cf98,0x003dae4,0x003e651,0x003f1e1,
    0x003fd92,0x0040967,0x004155e,0x0042179,0x0042db8,0x0043a1b,
    0x00446a2,0x004534f,0x0046022,0x0046d1a,0x0047a39,0x004877f,
    0x00494ec,0x004a281,0x004b03e,0x004be24,0x004cc33,0x004da6c,
    0x004e8cf,0x004f75d,0x0050615,0x00514fa,0x005240a,0x0053347,
    0x00542b2,0x005524a,0x0056210,0x0057205,0x005822a,0x005927e,
    0x005a303,0x005b3b8,0x005c49f,0x005d5b8,0x005e704,0x005f884,
    0x0060a37,0x0061c1e,0x0062e3b,0x006408d,0x0065316,0x00665d5,
    0x00678cc,0x0068bfc,0x0069f64,0x006b306,0x006c6e2,0x006daf8,
    0x006ef4b,0x00703d9,0x00718a5,0x0072dae,0x00742f6,0x007587c,
    0x0076e43,0x007844a,0x0079a92,0x007b11c,0x007c7e9,0x007defa,
    0x007f64f
};

#endif
// Negtive dB table, in steps of 0.5 dB
// 0dB, -0.5dB, -1dB, -1.5dB, -2dB, -2.5dB,.....
static const UINT32 _au4NegDBTable[] = {
    0x00020000,0x0001e35b,0x0001c852,0x0001aecb,0x000196b2,0x00017ff2,
    0x00016a77,0x00015631,0x0001430c,0x000130fa,0x00011feb,0x00010fd0,
    0x0001009b,0x0000f240,0x0000e4b3,0x0000d7e8,0x0000cbd4,0x0000c06d,
    0x0000b5aa,0x0000ab80,0x0000a1e8,0x000098da,0x0000904d,0x0000883a,
    0x0000809b,0x0000796a,0x0000729f,0x00006c35,0x00006628,0x00006071,
    0x00005b0c,0x000055f4,0x00005125,0x00004c9b,0x00004852,0x00004446,
    0x00004074,0x00003cd9,0x00003972,0x0000363b,0x00003333,0x00003055,
    0x00002da1,0x00002b14,0x000028ab,0x00002665,0x0000243f,0x00002238,
    0x0000204e,0x00001e7f,0x00001cca,0x00001b2e,0x000019a9,0x00001839,
    0x000016de,0x00001597,0x00001462,0x0000133e,0x0000122a,0x00001126,
    0x00001030,0x00000f49,0x00000e6e,0x00000d9f,0x00000cdc,0x00000c24,
    0x00000b76,0x00000ad2,0x00000a37,0x000009a4,0x0000091a,0x00000898,
    0x0000081d,0x000007a9,0x0000073b,0x000006d3,0x00000672,0x00000615,
    0x000005be,0x0000056c,0x0000051e,0x000004d5,0x00000490,0x0000044e,
    0x00000411,0x000003d6,0x0000039f,0x0000036c,0x0000033b,0x0000030c,
    0x000002e1,0x000002b7,0x00000290,0x0000026c,0x00000249,0x00000228,
    0x00000209,0x000001ec,0x000001d1,0x000001b7,0x0000019e,0x00000187,
    0x00000171,0x0000015c,0x00000149,0x00000136,0x00000125,0x00000115,
    0x00000105,0x000000f6,0x000000e9,0x000000dc,0x000000cf,0x000000c4,
    0x000000b9,0x000000ae,0x000000a5,0x0000009b,0x00000093,0x0000008a,
    0x00000083,0x0000007b,0x00000074,0x0000006e,0x00000068,0x00000062,
    0x0000005c,0x00000057,0x00000052,0x0000004e,0x00000049,0x00000045,
    0x00000041,0x0000003e,0x0000003a,0x00000037,0x00000034,0x00000031,
    0x0000002e,0x0000002b,0x00000029,0x00000027,0x00000024,0x00000022,
    0x00000020,0x0000001f,0x0000001d,0x0000001b,0x0000001a,0x00000018,
    0x00000017,0x00000016,0x00000014,0x00000013,0x00000012,0x00000011,
    0x00000010,0x0000000f,0x0000000e,0x0000000d,0x0000000d,0x0000000c,
    0x0000000b,0x0000000b,0x0000000a,0x00000009,0x00000009,0x00000008,
    0x00000008,0x00000007,0x00000007,0x00000006,0x00000006,0x00000006,
    0x00000005,0x00000005,0x00000005,0x00000004,0x00000004,0x00000004,
    0x00000004,0x00000003,0x00000003,0x00000003,0x00000003,0x00000003,
    0x00000002,0x00000002,0x00000002,0x00000002,0x00000002,0x00000002,
    0x00000002,0x00000001,0x00000001,0x00000001,0x00000001,0x00000001,
    0x00000001,0x00000001,0x00000001,0x00000001,0x00000001,0x00000001,
    0x00000001,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000
};

// Postive dB table, in steps of 0.5 dB
// 0dB, 0.5dB, 1dB, 1.5dB, 2dB, 2.5dB,.....
static const UINT32 _au4PosDBTable[] = {
    0x00020000,0x00021e56,0x00023e79,0x00026083,0x00028491,0x0002aac3,
    0x0002d338,0x0002fe12,0x00032b77,0x00035b8c,0x00038e7a,0x0003c46d,
    0x0003fd93,0x00043a1b,0x00047a39,0x0004be24,0x00050615,0x0005524a,
    0x0005a303,0x0005f884,0x00065316,0x0006b305,0x000718a5,0x00078449,
    0x0007f64f,0x00086f16,0x0008ef05,0x00097688,0x000a0614,0x000a9e20,
    0x000b3f30,0x000be9ca,0x000c9e80,0x000d5deb,0x000e28ae,0x000eff75,
    0x000fe2f5,0x0010d3f1,0x0011d334,0x0012e197,0x00140000,0x00152f61,
    0x001670bc,0x0017c521,0x00192db2,0x001aaba1,0x001c4031,0x001decb9,
    0x001fb2a7,0x0021937a,0x002390ca,0x0025ac48,0x0027e7be,0x002a4510,
    0x002cc640,0x002f6d6e,0x00323cdb,0x003536e9,0x00385e1e,0x003bb528,
    0x003f3edc,0x0042fe3b,0x0046f672,0x004b2ae0,0x004f9f16,0x005456dc,
    0x00595633,0x005ea158,0x00643cc9,0x006a2d48,0x007077e0,0x007721e6,
    0x007e3104,0x0085ab35,0x008d96d2,0x0095fa95,0x009edd9b,0x00a8476e,
    0x00b2400c,0x00bccfea,0x00c80000,0x00d3d9ca,0x00e06758,0x00edb350,
    0x00fbc8fb,0x010ab44c,0x011a81ec,0x012b3f43,0x013cfa88,0x014fc2c4,
    0x0163a7e7,0x0178bad5,0x018f0d6e,0x01a6b2a3,0x01bfbe85,0x01da4655,
    0x01f66095
};


// Fade dB table, -0.3dB per step
static const UINT32 _au4FadeDBTable[] = {
    0x00020000, 0x1EE9E, 0x1DDD3, 0x1CD9A, 0x1BDEE, 0x1AECB,
    0x1A02B, 0x1920A, 0x18464, 0x17734, 0x16A77, 0x15E29,
    0x15246, 0x146CA, 0x13BB2, 0x130FA, 0x126A0, 0x11C9F,
    0x112F5, 0x109A0, 0x1009B, 0xF7E5, 0xEF7A, 0xE759,
    0xDF7F, 0xD7E8, 0xD094, 0xC97F, 0xC2A8, 0xBC0C,
    0xB5AA, 0xAF7F, 0xA98A, 0xA3C8, 0x9E39, 0x98DA,
    0x93A9, 0x8EA6, 0x89CE, 0x8520, 0x809B, 0x7C3E,
    0x7806, 0x73F3, 0x7003, 0x6C35, 0x6889, 0x64FC,
    0x618F, 0x5E3F, 0x5B0C, 0x57F4, 0x54F8, 0x5216,
    0x4F4C, 0x4C9B, 0x4A01, 0x477E, 0x4511, 0x42B8,
    0x4074, 0x3E44, 0x3C27, 0x3A1C, 0x3823, 0x363B,
    0x3464, 0x329D, 0x30E5, 0x2F3C, 0x2DA1, 0x2C15,
    0x2A96, 0x2923, 0x27BE, 0x2665, 0x2517, 0x23D4,
    0x229D, 0x2170, 0x204E, 0x1F35, 0x1E26, 0x1D20,
    0x1C22, 0x1B2E, 0x1A42, 0x195D, 0x1881, 0x17AC,
    0x16DE, 0x1618, 0x1557, 0x149E, 0x13EB, 0x133E,
    0x1296, 0x11F5, 0x1159, 0x10C2, 0x1030, 0xFA4,
    0xF1C, 0xE98, 0xE1A, 0xD9F, 0xD29, 0xCB6,
    0xC48, 0xBDD, 0xB76, 0xB12, 0xAB2, 0xA55,
    0x9FB, 0x9A4, 0x951, 0x900, 0x8B1, 0x866,
    0x81D, 0x7D6, 0x792, 0x750, 0x711, 0x6D3,
    0x698, 0x65F, 0x627, 0x5F2, 0x5BE, 0x58C,
    0x55C, 0x52D, 0x500, 0x4D5, 0x4AB, 0x482,
    0x45B, 0x435, 0x411, 0x3ED, 0x3CB, 0x3AA,
    0x38A, 0x36C, 0x34E, 0x331, 0x315, 0x2FA,
    0x2E1, 0x2C8, 0x2AF, 0x298, 0x281, 0x26C,
    0x257, 0x242, 0x22F, 0x21C, 0x209, 0x1F8,
    0x1E6, 0x1D6, 0x1C6, 0x1B7, 0x1A8, 0x199,
    0x18B, 0x17E, 0x171, 0x164, 0x158, 0x14D,
    0x141, 0x136, 0x12C, 0x122, 0x118, 0x10E,
    0x105, 0xFC, 0xF4, 0xEB, 0xE3, 0xDC,
    0xD4, 0xCD, 0xC6, 0xBF, 0xB9, 0xB2,
    0xAC, 0xA6, 0xA1, 0x9B, 0x96, 0x91,
    0x8C, 0x87, 0x83, 0x7E, 0x7A, 0x76,
    0x72, 0x6E, 0x6A, 0x66, 0x63, 0x60,
    0x5C, 0x59, 0x56, 0x53, 0x50, 0x4E,
    0x4B, 0x48, 0x46, 0x44, 0x41, 0x3F,
    0x3D, 0x3B, 0x39, 0x37, 0x35, 0x33,
    0x31, 0x30, 0x2E, 0x2C, 0x2B, 0x29,
    0x28, 0x27, 0x25, 0x24, 0x23, 0x22,
    0x20, 0x1F, 0x1E, 0x1D, 0x1C, 0x1B,
    0x1A, 0x19, 0x18, 0x18, 0x17, 0x16,
    0x15, 0x15, 0x14, 0
};

// Pan dB table
static const UINT32 _au4PanDBTable[] = {
    0x20000,
    0x1CF5C,
    0x1A326,
    0x17ADA,
    0x155F4,
    0x1340C,
    0x114DD,
    0xF809,
    0xDD5E,
    0xC4A3,
    0xADA0,
    0x9833,
    0x843A,
    0x718B,
    0x6010,
    0x4FAC,
    0x404B,
    0x31D8,
    0x243F,
    0x1772,
    0xB61,
    0
};

static UINT32 _u4FadeDBTableSz = sizeof (_au4FadeDBTable)/sizeof(UINT32);
static UINT32 _u4PanDBTableSz = sizeof (_au4PanDBTable)/sizeof(UINT32);
#ifdef CC_AUD_SX1_FEATURE
static UINT32 _u4PosDBTableSzPEQ = sizeof(_au4PosDBTablePEQ)/sizeof(UINT32);
static UINT32 _u4NegDBTableSzPEQ = sizeof (_au4NegDBTablePEQ)/sizeof(UINT32);
static const UINT32 _au4PosDBTableAd[] = {
        0x1009B, 
        0x10CB3, 
        0x1195D, 
        0x126A0, 
        0x13482, 
        0x1430C, 
        0x15246, 
        0x16237, 
        0x172E9, 
        0x18464, 
        0x196B2, 
        0x1A9DC, 
        0x1BDEE, 
        0x1D2F3, 
        0x1E8F4, 
        0x20000, 
        0x21821, 
        0x23165, 
        0x24BDA, 
        0x2678F, 
        0x28491, 
        0x2A2F2, 
        0x2C2C1, 
        0x2E410, 
        0x306F1, 
        0x32B77, 
        0x351B5, 
        0x379C1, 
        0x3A3AF, 
        0x3CF98, 
        0x3FD93
};

static UINT32 _u4PosDBTableAdSz = sizeof(_au4PosDBTableAd)/sizeof(UINT32);
#endif
static UINT32 _u4PosDBTableSz = sizeof(_au4PosDBTable)/sizeof(UINT32);
static UINT32 _u4NegDBTableSz = sizeof (_au4NegDBTable)/sizeof(UINT32);


#if 0 //ndef CC_MT5391_AUD_3_DECODER
static UINT32 _au4MasterVolShm[AUD_DEC_NUM] = {VOL_SHM_0_DB, VOL_SHM_0_DB};
#else
static UINT32 _au4MasterVolShm[AUD_DEC_NUM] = {VOL_SHM_0_DB, VOL_SHM_0_DB, VOL_SHM_0_DB};
#endif
static BOOL   _fgMasterVolUseShmRawData = FALSE;
static BOOL   _fgChannelVolUseShmRawData = FALSE;
#ifdef SUPPORT_PHILIPS_LOUDNESS
static BOOL   _fgLoudnessEnable = FALSE;
#endif

#ifdef CC_AUD_VBASS_SUPPORT
static UINT8 _gu1SubWooferOffset = 0x0;
static UINT8 _gu1SubWooferGain = 0x15;
#endif

static INT32 _i4AsrcBaseFreq = ASRC_BASE_FREQ_48K;
static INT32 _i4AsrcSmartVCXOAdjustLevel = 0;
static INT32 _i4AsrcSmartVCXOB2rtLevel = 0;
static INT32 _i4AsrcAVSyncAdjustLevel = 0;

//-------------------------------------------------------------------------------------
// NVM Releated declarations
//-------------------------------------------------------------------------------------
static UINT8  _au1VolStep[VOL_TABLE_CHK_PNT] = {0,16,32,49,66,83,100};
#ifdef CC_AUD_USE_NVM   // For customers use EEPROM and FLASH audio parameters
// ==== EEPROM Releated =====
static UINT8  auAudNvmBuffer[AUD_NVM_LENGTH];   //Buffer read from EEPROM
static BOOL   _fgNvmInit = FALSE;
#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
static UINT32 u4NvmVolTab[VOL_TABLE_CHK_PNT] = {0, 0x1030, 0x4ca0, 0xbb00, 0x13100, 0x17510, 0x20000};
#else
static UINT32 u4NvmVolTab[AUD_NVM_VOL_TBL_NUM][VOL_TABLE_CHK_PNT] = 
{
    {0, 0x1030, 0x4ca0, 0xbb00, 0x13100, 0x17510, 0x20000},
    {0, 0x1030, 0x4ca0, 0xbb00, 0x13100, 0x17510, 0x20000},
    {0, 0x1030, 0x4ca0, 0xbb00, 0x13100, 0x17510, 0x20000},
    {0, 0x1030, 0x4ca0, 0xbb00, 0x13100, 0x17510, 0x20000}
};
#endif
static UINT32 u4NvmHPVolTab[VOL_TABLE_CHK_PNT] = {0, 0x1030, 0x4ca0, 0xbb00, 0x13100, 0x17510, 0x20000};
static AUD_IPT_SRC_GAIN_T _rNVMIuputSrcGain;     // Input Source Gain
static AUD_VSURR_CFG_T _rNVMSurrCfg[2];             // Surround
static BOOL _fgNvmFlat = FALSE;                  // Flat Frequency Response
static AUD_NVM_DATA_T aeNvmData[AUD_NVM_LENGTH]; // Customer NVM Data Structure
static UINT8 au1NvmCrc = 0;

// ==== PROFILE Releated ====
static UINT8  _u1AudProfile[3] = {0,0,0};        // The profile number to be referenced is stored in NVM
static BOOL   _fgAudNVMLipSync[3] = {FALSE,FALSE,FALSE};
static UINT16 _u2AudNVMProfileOffset[3] = {AUD_NVM_PROFILE_OFFSET1, AUD_NVM_PROFILE_OFFSET2, AUD_NVM_PROFILE_OFFSET3};
#ifdef SUPPORT_PHILIPS_LOUDNESS
static INT8 i1Loundness[CH_EQ_BAND_NO] = {+5,-3,+2,0,+2,+2,+5};
#endif
static UINT8  auAudFlashCommBuffer[AUD_FLASH_LENGTH];   //Buffer read from Flash
static AUD_NVM_DATA_T aeFlashCommData[AUD_FLASH_LENGTH]; // Customer Flash Structure
static AUD_NVM_ATV_PREGAIN eAudNVMAtvPreGain;
static INT32 _i4NvmExtraVolGain[2];
static INT32 _i4NvmExtraVolGain1[2];
#endif //CC_AUD_USE_NVM

#ifdef CC_AUD_SPIDF_LVL
static UINT8 _u1AudSpdifLevel[3] = {MAX_VOL_LEVEL, MAX_VOL_LEVEL, MAX_VOL_LEVEL};   // More (0dB), Medium (-4dB), Less (-8dB)
#endif
#ifdef CC_AUD_CHL_VOL_MODE
static BOOL  _fgAudSkype = FALSE;   // in skype mode
#endif

#ifdef CC_AUD_USE_FLASH_AQ
static UINT8 _au1AQLevel[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_LEVEL_ITEMS];
static UINT8 _au1AQ_EQLevel[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_EQ_ITEMS];
static UINT8 _au1AQ_DRC[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_DRC_ITEMS];
static UINT8 _au1AQ_AVC[FLASH_AQ_SRC_NUM][AQ_AVC_ITEMS];
static UINT8 _au1AQ_LIMITER[FLASH_AQ_SRC_NUM][AQ_LIMITER_ITEMS];
static UINT32 _au4AQ_PostScale[AQ_POSTSCL_ITEMS];
#ifdef CC_SONY_2K14_FLASH_AQ
static UINT8 _au1AQ_PEQ_TABLE[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_PEQ_ITEMS];
static UINT8 _au1AQ_PEQ_WALL[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_PEQ_ITEMS];
static UINT8 _au1AQBassTrebleOffset_TABLE[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_BASS_ITEMS];
static UINT8 _au1AQBassTrebleOffset_WALL[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_BASS_ITEMS];
static UINT8 _au1AQBassTrebleCutoff[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_BASS_ITEMS];
static UINT8 _au1AQ_VBass_TABLE[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_VBASS_ITEMS];
static UINT8 _au1AQ_VBass_WALL[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_VBASS_ITEMS];
static UINT8 _au1AQ_SPH_TABLE[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_SPH_ITEMS];
static UINT8 _au1AQ_SPH_WALL[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_SPH_ITEMS];
static UINT32 _au4AQ_MVS_TABLE[FLASH_AQ_SCENE_NUM][AQ_MVS_ITEMS];
static UINT32 _au4AQ_MVS_WALL[FLASH_AQ_SCENE_NUM][AQ_MVS_ITEMS];
static UINT8 _au1AQBassManage_TABLE[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_BASS_MANAGE_ITEMS];
static UINT8 _au1AQBassManage_WALL[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_BASS_MANAGE_ITEMS];
static UINT32 _au1AQ_FIR_TABLE[AQ_FIR_ITEMS];
static UINT32 _au1AQ_FIR_WALL[AQ_FIR_ITEMS];
static UINT8 _au1AQ_LOUDNESS_TABLE[FLASH_AQ_SM_NUM][AQ_LOUDNESS_ITEMS];
static UINT8 _au1AQ_LOUDNESS_WALL[FLASH_AQ_SM_NUM][AQ_LOUDNESS_ITEMS];
static UINT8 _u1LoudnessFlag_TABLE[FLASH_AQ_SM_NUM] = {0,0,0,0};   
static UINT8 _u1LoudnessFlag_WALL[FLASH_AQ_SM_NUM] = {0,0,0,0};   
static UINT8 _au1AQ_MTKDRC_TABLE[FLASH_AQ_SM_NUM][AQ_MTK_DRC_ITEMS];
static UINT8 _au1AQ_MTKDRC_WALL[FLASH_AQ_SM_NUM][AQ_MTK_DRC_ITEMS];
#else
static UINT8 _au1AQ_PEQ[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_PEQ_ITEMS];
static UINT8 _au1AQBassTrebleOffset[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_BASS_ITEMS];
static UINT8 _au1AQ_VBass[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_VBASS_ITEMS];
static UINT8 _au1AQ_SPH[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_SPH_ITEMS];
static UINT32 _au4AQ_MVS[FLASH_AQ_SCENE_NUM][AQ_MVS_ITEMS];
static UINT8 _au1AQBassManage[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_BASS_MANAGE_ITEMS];
static UINT32 _au1AQ_FIR[AQ_FIR_ITEMS];
#ifdef CC_AUD_FY12_LOUDNESS
#ifdef CC_AUD_LOUDNESS_FROM_SOURCE_SOUNDMODE
static UINT8 _au1AQ_LOUDNESS[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_LOUDNESS_ITEMS];
#else
    #if 1///def CC_SONY_2K13_FLASH_AQ
static UINT8 _au1AQ_LOUDNESS[AQ_LOUDNESS_ITEMS];
static UINT8 _u1LoudnessFlag = 0;   /// Save _au1AQ_LOUDNESS[0]
    #else
static UINT8 _au1AQ_LOUDNESS[AQ_LOUDNESS_ITEMS];
    #endif
#endif
#endif
static UINT8 _au1AQ_MTKDRC[AQ_MTK_DRC_ITEMS];

#endif
static UINT8 _au1AQ_SP_Vol[FLASH_AQ_SRC_NUM];
static UINT8 _au1AQ_LO_Vol[FLASH_AQ_SRC_NUM];
static UINT32 _au4AQ_EXT_PEQ[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_AMP_FS_NUM][AQ_EXT_PEQ_ITEMS];
static UINT32 _au4AQ_EXT_PEQ_LsRS[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_AMP_FS_NUM][AQ_EXT_PEQ_ITEMS];
static UINT32 _au4AQ_EXT_PEQ_CSw[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_AMP_FS_NUM][AQ_EXT_PEQ_CSW_ITEMS];
static UINT8 _au1AQ_BackLight[AQ_BACKLIGHT_ITEMS];
UINT8 _au1AQChVolume[FLASH_AQ_COMM_SRC_NUM][FLASH_AQ_COMM_CH_NUM];
#ifdef CC_AUD_SUPPORT_STC_COMPRESSOR
static UINT32 _au1AQ_381STCCoeff[AQ_STC381Coeff_ITEMS];
static UINT32 _au4AQ_EXT_381STCFilter[AQ_AMP_FS_NUM][AQ_EXT_PEQ_ITEMS];
#endif
static BOOL  _fgAQ_Enable[AQ_TYPE_NUM];
static UINT8 _au1AQLevel_LsRs[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_LEVEL_ITEMS];
static UINT8 _au1AQLevel_CSw[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_LEVEL_ITEMS];
static UINT8 _au1AQ_DRC_LsRs[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_DRC_ITEMS];
static UINT8 _au1AQ_DRC_CSw[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][AQ_DRC_ITEMS];
static UINT32 _au4AQ_PostScale_LsRs[AQ_POSTSCL_ITEMS];
static UINT32 _au4AQ_PostScale_CSw[AQ_POSTSCL_ITEMS];
static UINT32 _u4AQ_Mic_Gain;
static AUD_AQ_TYPE3_DATA_T _rAQ_TYPE3_TBL;
static UINT8 _au1CurrentSoudMode = AUD_SOUND_MODE_NUM+1; //set initial value to an non-exist sound mode.
static INT8 _aai1BassTreble[AQ_BASS_ITEMS];
static UINT8 _u1MWBassLevel = BASS_TREBLE_GAIN_DB_MAX;
static UINT8 _u1MWTrebleLevel = BASS_TREBLE_GAIN_DB_MAX;
static BOOL _fgFlashAQPeriphRecover = FALSE;
#ifdef CC_SONY_2K14_FLASH_AQ
///Channel Volume Gain
///2K13 Merge USB -> HDMI and change source sequence
///Analog   0 -> 0
///ATV      1 -> 2
///HDMI     2 -> 1
///DTV      3 -> 3
///USB      4 -> 1
static UINT8 _au1AQSrc_Comm_Convert[AUD_STREAM_FROM_NUM] = {0, //AUD_STREAM_FROM_OTHERS
                                                            3, //AUD_STREAM_FROM_DIGITAL_TUNER
                                                            2, //AUD_STREAM_FROM_ANALOG_TUNER
                                                            0, //AUD_STREAM_FROM_SPDIF,
                                                            0, //AUD_STREAM_FROM_LINE_IN
                                                            1, //AUD_STREAM_FROM_HDMI,
                                                            1, //AUD_STREAM_FROM_MEMORY,
                                                            1, //AUD_STREAM_FROM_BUF_AGT,
                                                            1, //AUD_STREAM_FROM_FEEDER,
                                                            1};//AUD_STREAM_FROM_MULTI_MEDIA
///Other Post Processor
///2K13 Merge HDMI/USB to Analog Input
///Analog   0 -> 0
///HDMI/USB 1 -> 0
///ATV      2 -> 1
///DTV      3 -> 2
static UINT8 _au1AQSrc_Convert[AUD_STREAM_FROM_NUM] = {0, //AUD_STREAM_FROM_OTHERS
                                                       0, //AUD_STREAM_FROM_DIGITAL_TUNER
                                                       1, //AUD_STREAM_FROM_ANALOG_TUNER
                                                       0, //AUD_STREAM_FROM_SPDIF,
                                                       0, //AUD_STREAM_FROM_LINE_IN
                                                       0, //AUD_STREAM_FROM_HDMI,
                                                       0, //AUD_STREAM_FROM_MEMORY,
                                                       0, //AUD_STREAM_FROM_BUF_AGT,
                                                       0, //AUD_STREAM_FROM_FEEDER,
                                                       0};//AUD_STREAM_FROM_MULTI_MEDIA
#else
///Channel Volume Gain
///2K13 Merge USB -> HDMI and change source sequence
///Analog   0 -> 0
///ATV      1 -> 2
///HDMI     2 -> 1
///DTV      3 -> 3
///USB      4 -> 1
static UINT8 _au1AQSrc_Comm_Convert[AUD_STREAM_FROM_NUM] = {0, //AUD_STREAM_FROM_OTHERS
                                                            3, //AUD_STREAM_FROM_DIGITAL_TUNER
                                                            2, //AUD_STREAM_FROM_ANALOG_TUNER
                                                            0, //AUD_STREAM_FROM_SPDIF,
                                                            0, //AUD_STREAM_FROM_LINE_IN
                                                            1, //AUD_STREAM_FROM_HDMI,
                                                            1, //AUD_STREAM_FROM_MEMORY,
                                                            1, //AUD_STREAM_FROM_BUF_AGT,
                                                            1, //AUD_STREAM_FROM_FEEDER,
                                                            1};//AUD_STREAM_FROM_MULTI_MEDIA
///Other Post Processor
///2K13 Merge HDMI/USB to Analog Input
///Analog   0 -> 0
///HDMI/USB 1 -> 0
///ATV      2 -> 1
///DTV      3 -> 2
static UINT8 _au1AQSrc_Convert[AUD_STREAM_FROM_NUM] = {0, //AUD_STREAM_FROM_OTHERS
                                                       2, //AUD_STREAM_FROM_DIGITAL_TUNER
                                                       1, //AUD_STREAM_FROM_ANALOG_TUNER
                                                       0, //AUD_STREAM_FROM_SPDIF,
                                                       0, //AUD_STREAM_FROM_LINE_IN
                                                       0, //AUD_STREAM_FROM_HDMI,
                                                       0, //AUD_STREAM_FROM_MEMORY,
                                                       0, //AUD_STREAM_FROM_BUF_AGT,
                                                       0, //AUD_STREAM_FROM_FEEDER,
                                                       0};//AUD_STREAM_FROM_MULTI_MEDIA

#endif
static UINT8 _au1AQSmod_Convert[AUD_SOUND_MODE_NUM+1] = {0,1,0,2,3}; //mapping to Dynamic, Standard, Clear, Cust
static UINT16 _au2AQBMCutOffMap[AQ_BM_CUTOFF_NUM] = {50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,300};
#endif  // CC_AUD_USE_FLASH_AQ
#ifdef KARAOKE_SUPPORT
static UINT8 _u1MicVol_lock = 0;
#endif

#ifdef CC_AUD_FY12_LOUDNESS
//static INT8 _ai1BassAttnTable[10] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4};  //-13~-4 means -1dB~-10dB
//static INT8 _ai1TrebleAttnTable[10] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4}; //-13~-4 means -1dB~-10dB
#ifdef CC_AUD_LOUDNESS_FROM_SOURCE_SOUNDMODE
static INT8 _ai1BassAttnTable[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][10] = {
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
}; //28 meand 0dB  
static INT8 _ai1TrebleAttnTable[FLASH_AQ_SRC_NUM][FLASH_AQ_SM_NUM][10] = {
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
};  // 0~56 maps to -14~14dB, 0.5dB steps

#else
    #ifdef CC_SONY_2K14_FLASH_AQ
static INT8 _ai1BassAttnTable_TABLE[FLASH_AQ_SM_NUM][20] = {
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
};
static INT8 _ai1TrebleAttnTable_TABLE[FLASH_AQ_SM_NUM][20] = {
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
};

static INT8 _ai1BassAttnTable_WALL[FLASH_AQ_SM_NUM][20] = {
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
};
static INT8 _ai1TrebleAttnTable_WALL[FLASH_AQ_SM_NUM][20] = {
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
};

    #else ///def CC_SONY_2K13_FLASH_AQ
static INT8 _ai1BassAttnTable[12] = {28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28};  //28 meand 0dB
static INT8 _ai1TrebleAttnTable[12] = {28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28}; // 0~56 maps to -14~14dB, 0.5dB steps
    #endif
#endif  // CC_AUD_NEW_AQ_PARSER
#endif  // CC_AUD_USE_FLASH_AQ

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
LGSE_FN_PARAM_T _argLgseFnPara[HAL_APROC_LGSE_NUM];
#else
LGSE_FN_PARAM_T _argLgseFnPara[AUDIO_LGSE_FUNC_NUM];
#endif

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
#define APROC_DELAYBUF_BLOCK_SAMPLE_NUM	256   // samples in one block
const UINT32 u4DelayConstant = APROC_DELAYBUF_BLOCK_SAMPLE_NUM*34/(48*5);
#endif

const AUD_ENUM_TO_NAME_T eChannelOutMap[] = 
{
    {AUD_CH_FRONT_LEFT, "Speaker"},
    {AUD_CH_IEC_LEFT, "SPDIF"},
    {AUD_CH_DMX_LEFT, "HeadPhone"},
    {AUD_CH_AUX_FRONT_LEFT, "SCART"},
    {AUD_CH_BYPASS_LEFT, "MONITER"},
};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static AUD_DEC_STREAM_FROM_T _AudDspEuAtvStrmSrcTransfer(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    if (eStreamFrom >= AUD_STREAM_FROM_NUM)
    {
        LOG(0, "Incorrect input src setting!\n");
        ASSERT(0);
    }

    return eStreamFrom;
}

#if 0   // Unused
AUD_IEC_T _GetIECFlag(UINT8 u1DecId)
{
    return _aeIecMode;
}
#endif

BOOL _GetVolMuteStatus(UINT8 bPath)
{
    UNUSED(bPath);
    UNUSED(_GetVolMuteStatus);
    return _aeVolMuteFlag;
}

#if 0
static void _DSP_IecOff()
{
    vWriteShmUINT8(B_IECFLAG, (UINT8)AUD_IEC_CFG_PCM);
    vWriteShmUINT8(B_IEC_MUTE, 0x1);
    DSP_SendDspTaskCmd(UOP_DSP_IEC_FLAG);

    x_thread_delay(100);
    _afgIecEnable = FALSE;
}
#endif

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
INT32 _AudDspToAprocVol(UINT32 u4Volume)
{
    INT32 i4Vol;

    if (u4Volume == 0) 
    {
        i4Vol = 0; 
    }
    else if (u4Volume < VOL_SHM_0_DB * 0x100)
    {
         i4Vol = (INT32)u4Volume * 0x40 - 1; 
    }
    else 
    {
         i4Vol = 0xFFFFFFFF;
    }

    return i4Vol;
}
#endif

#ifdef CC_AUD_SX1_FEATURE
static UINT32 _DbToShmPEQ(INT32 i4Level)
{
    INT32 i4Val;

    // Check max and min value
    if (i4Level >= (INT32)(_u4PosDBTableSzPEQ-1))
    {
        i4Val = (INT32)(_u4PosDBTableSzPEQ-1);
    }
    else if (i4Level < (-(INT32)_u4NegDBTableSzPEQ))
    {
        i4Val = -(INT32)_u4NegDBTableSzPEQ;
    }
    else
    {
        i4Val = i4Level;
    }

    return (i4Val <= 0) ? _au4NegDBTablePEQ[-i4Val] : _au4PosDBTablePEQ[i4Val];
}

#endif
static UINT32 _DbToShm(INT32 i4Level)
{
    INT32 i4Val;

    // Check max and min value
    if (i4Level >= (INT32)(_u4PosDBTableSz-1))
    {
        i4Val = (INT32)(_u4PosDBTableSz-1);
    }
    else if (i4Level < (-(INT32)_u4NegDBTableSz))
    {
        i4Val = -(INT32)_u4NegDBTableSz;
    }
    else
    {
        i4Val = i4Level;
    }

    return (i4Val <= 0) ? _au4NegDBTable[-i4Val] : _au4PosDBTable[i4Val];
}
#ifdef CC_AUD_SX1_FEATURE

static UINT32 _DbToShmAd(INT32 i4Level)
{
    INT32 i4Val;

    // Check max and min value
    if (i4Level >= (INT32)_u4PosDBTableAdSz)
    {
        i4Val = (INT32)_u4PosDBTableAdSz;    
    }
    else
    {
        i4Val = i4Level;
    }
    
    return _au4PosDBTableAd[i4Val];
}
#endif

static UINT32 _DbToShmFade(INT32 i4Level)
{
    INT32 i4Val;

    // Check max and min value
    if (i4Level >= (INT32)_u4FadeDBTableSz)
    {
        i4Val = (INT32)_u4FadeDBTableSz;
    }
    else
    {
        i4Val = i4Level;
    }

    return _au4FadeDBTable[i4Val];
}

static UINT32 _DbToShmPan(INT32 i4Level)
{
    INT32 i4Val;

    // Check max and min value
    if (i4Level >= (INT32)_u4PanDBTableSz)
    {
        i4Val = (INT32)_u4PanDBTableSz;
    }
    else if (i4Level < (-(INT32)_u4PanDBTableSz))
    {
        i4Val = -(INT32)_u4PanDBTableSz;
    }
    else
    {
        i4Val = i4Level;
    }

    if ( i4Val < 0 )
        i4Val = -i4Val;

    return _au4PanDBTable[i4Val];
}

#ifndef CC_AUD_DDI
static UINT8 _BalancedVolume(AUD_CH_T eChannel, UINT8 u1Balance, UINT8 u1Volume)
{
    UINT32 u4BalancedVolume = u1Volume;

    if (u1Balance != (MAX_BALANCE_LEVEL/2))
    {
        if (u1Balance < (MAX_BALANCE_LEVEL/2))
        {
            if ((eChannel == AUD_CH_FRONT_RIGHT) ||
                ((eChannel == AUD_CH_DMX_RIGHT) &&
                 ((UINT8)DRVCUST_OptGet(eAudioDownmixWithBalance)==TRUE)))
            {
                u4BalancedVolume = ((u1Balance*2) * u1Volume) / MAX_VOL_LEVEL;
            }
        }
        else
        {
            if ((eChannel == AUD_CH_FRONT_LEFT) ||
                ((eChannel == AUD_CH_DMX_LEFT) &&
                 ((UINT8)DRVCUST_OptGet(eAudioDownmixWithBalance)==TRUE)))
            {
                u4BalancedVolume = (((MAX_VOL_LEVEL - u1Balance)*2) * u1Volume) / MAX_VOL_LEVEL;
            }
        }
    }
    return (UINT8)u4BalancedVolume;
}
#endif

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
void _AudAprocInputMute(UINT8 u1DecId, BOOL fgMute)
{
    //UINT32 u4Idx;
    UINT32 u4Vol;
    
    if (fgMute)
    {
        u4Vol = 0; 
    }
    else
    {
        u4Vol = 0x7FFFFF;
    }

    // For Stop Fade Out
    //AUD_Aproc_Chk_DecId(&u1DecId);
    if (u1DecId == AUD_DEC_MAIN)
    {
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_MUTE, APROC_IOCTR_MUTE_AMIXER0, &u4Vol, 1);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_MUTE, APROC_IOCTR_MUTE_AMIXER1, &u4Vol, 1);
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_MUTE, APROC_IOCTR_MUTE_AMIXER0, &u4Vol, 1);
    }

    LOG(5, "_AudAprocInputMute Dec%d to %d\n", u1DecId, fgMute);

}
#endif //defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)

static void _AudOutPortMuteStateNotify(AUD_OUT_PORT_T ePort, BOOL fgMute)
{
    AudDecNfyFct  pfAudDecNfy = NULL;
    
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));
    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void*)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_MUTE_STATE_CHG,
                        (UINT32)ePort, (UINT32)fgMute);
    }
}

void _AUD_SetOutPortHwMute(AUD_OUT_PORT_T ePort, BOOL fgMute, BOOL fgNotify)
{
    BOOL fgNewMuteState;
    _afgOutPortHwMute[ePort] = fgMute;

    fgNewMuteState = (_afgOutPortHwMute[ePort] || _afgOutPortVolMute[ePort]) ? TRUE : FALSE;
    if (fgNotify && (fgNewMuteState != _afgOutPortMute[ePort]))
    {
        _AudOutPortMuteStateNotify(ePort, fgNewMuteState);
    }
    _afgOutPortMute[ePort] = fgNewMuteState;
}

void _AUD_SetOutPortVolMute(AUD_OUT_PORT_T ePort, BOOL fgMute, BOOL fgNotify)
{
    BOOL fgNewMuteState;
    _afgOutPortVolMute[ePort] = fgMute;

    fgNewMuteState = (_afgOutPortHwMute[ePort] || _afgOutPortVolMute[ePort]) ? TRUE : FALSE;
    if (fgNotify && (fgNewMuteState != _afgOutPortMute[ePort]))
    {
        _AudOutPortMuteStateNotify(ePort, fgNewMuteState);
    }
    _afgOutPortMute[ePort] = fgNewMuteState;
}

void _VolumeCtrl(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4Value)
{
    BOOL fgMute;
    UINT16 u2ShmIndex = 0;
    UINT16 u2UopIndex = 0; // invalid setting
    UINT32 u4VolumeShm = 0; /* 0 ~ 0x7fffff */

//=========== For Render ================
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    UINT32 u4Idx;
    INT32 i4Vol;

    UNUSED(u2ShmIndex);
    UNUSED(u2UopIndex);
    UNUSED(u4VolumeShm);

 

    // TODO: Need Review volume system
    //Output TRIM: channel volume
    //Output Volume: mater volume
    //What about audio description volume??
    if (eChannel == AUD_CH_ALL)
    {
        i4Vol = _AudDspToAprocVol(u4Value);
        // AUD_Aproc_Chk_DecId(&u1DecId);
        if (u1DecId == AUD_DEC_MAIN)
        {
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_SP, &i4Vol, 1);    
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_HP, &i4Vol, 1);
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_SCART, &i4Vol, 1);
        }
        else if (u1DecId == AUD_DEC_THIRD)
        {
            //FIXME: AD volume: input trim
            //_vAUD_Aproc_Set (APROC_CONTROL_TYPE_VOL, APROC_IOCTR_TRIM_INPUT2, &i4Vol, 1); 
            // change to amixer2 trim for AD only, not AD+main
            i4Vol = _AudDspToAprocVol(u4Value);
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_AMIXER2, &i4Vol, 1);             
        }
        else
        {
            LOG(1, "wrong decoder ID\n");
        } 
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        switch (eChannel)
        {
        case AUD_CH_FRONT_LEFT:
        case AUD_CH_FRONT_RIGHT:
            u4Idx = APROC_IOCTR_TRIM_SCART;
            break;
        default: 
            u4Idx = APROC_IOCTR_TRIM_SCART;
            break;             
        }
    
        i4Vol = _AudDspToAprocVol(u4Value);
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_TRIM, u4Idx, &i4Vol, 1);
    }
    else
    {
        switch (eChannel)
        {
        case AUD_CH_FRONT_LEFT:
        case AUD_CH_FRONT_RIGHT:
            u4Idx = APROC_IOCTR_TRIM_SP;
            break;
        case AUD_CH_CENTER:
        case AUD_CH_SUB_WOOFER:
            u4Idx = APROC_IOCTR_TRIM_SPSW;
            break;   
        case AUD_CH_REAR_LEFT:
        case AUD_CH_REAR_RIGHT:
            u4Idx = APROC_IOCTR_TRIM_SPLSRS;
            break;
        case AUD_CH_IEC_LEFT:
        case AUD_CH_IEC_RIGHT:
            u4Idx = APROC_IOCTR_TRIM_IEC;
            break;        
        case AUD_CH_BYPASS_LEFT:
        case AUD_CH_BYPASS_RIGHT:
            u4Idx = APROC_IOCTR_TRIM_MON;
            break;        
        case AUD_CH_DMX_LEFT:
        case AUD_CH_DMX_RIGHT:
            u4Idx = APROC_IOCTR_TRIM_HP;
            break; 
        default:
            u4Idx = APROC_IOCTR_TRIM_SP;
            break;                   
        }
    
        i4Vol = _AudDspToAprocVol(u4Value);
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_TRIM, u4Idx, &i4Vol, 1);
    }

    return;
#endif    //defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
//=========== End of Render ================

    u4VolumeShm = u4Value;

    LOG(5, "_VolumeCtrl, u1DecId (%d), eChannel (%d), u4Value (0x%x)\n", u1DecId, eChannel, u4Value);
#ifdef CC_AUD_DDI
    if((eChannel==AUD_CH_REAR_LEFT)||(eChannel==AUD_CH_REAR_RIGHT))
        return;
#endif
    if (u1DecId == AUD_DEC_MAIN)
    {
        switch (eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L;
                u2UopIndex = UOP_DSP_TRIM_L;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R;
                u2UopIndex = UOP_DSP_TRIM_R;
                break;
            case AUD_CH_REAR_LEFT:
                u2ShmIndex = D_TRIM_LS;
                u2UopIndex = UOP_DSP_TRIM_LS;
                break;
            case AUD_CH_REAR_RIGHT:
                u2ShmIndex = D_TRIM_RS;
                u2UopIndex = UOP_DSP_TRIM_RS;
                break;
            case AUD_CH_CENTER:
                u2ShmIndex = D_TRIM_C;
                u2UopIndex = UOP_DSP_TRIM_C;
                break;
            case AUD_CH_SUB_WOOFER:
                u2ShmIndex = D_TRIM_SUB;
                u2UopIndex = UOP_DSP_TRIM_SUBWOOFER;
                break;
            case AUD_CH_BYPASS_LEFT:
                u2ShmIndex = D_TRIM_CH7;
                u2UopIndex = UOP_DSP_TRIM_CH7;
                break;
            case AUD_CH_BYPASS_RIGHT:
                u2ShmIndex = D_TRIM_CH8;
                u2UopIndex = UOP_DSP_TRIM_CH8;
                break;
            case AUD_CH_DMX_LEFT:
                u2ShmIndex = D_TRIM_CH9;
                u2UopIndex = UOP_DSP_TRIM_CH9;
                break;
            case AUD_CH_DMX_RIGHT:
                u2ShmIndex = D_TRIM_CH10;
                u2UopIndex = UOP_DSP_TRIM_CH10;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
                u2ShmIndex = D_VOL;
                u2UopIndex = UOP_DSP_MASTER_VOLUME;
                break;
            case AUD_CH_LFE:
                u2ShmIndex = D_TRIM_LFE;
                u2UopIndex = UOP_DSP_TRIM_LFE;
                break;
            default:
                break;
        }
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = D_VOL_DEC2;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC2;

        switch(eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L_DEC2;
                u2UopIndex = UOP_DSP_TRIM_L_DEC2;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R_DEC2;
                u2UopIndex = UOP_DSP_TRIM_R_DEC2;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
                u2ShmIndex = D_VOL_DEC2;
                u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC2;
                break;

            default:
                return;
        }
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = D_VOL_DEC3;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC3;
        switch(eChannel)
         {
             case AUD_CH_FRONT_LEFT:
                 u2ShmIndex = D_TRIM_L_DEC3;
                 u2UopIndex = UOP_DSP_TRIM_L_DEC3;
                 break;
             case AUD_CH_FRONT_RIGHT:
                 u2ShmIndex = D_TRIM_R_DEC3;
                 u2UopIndex = UOP_DSP_TRIM_R_DEC3;
                 break;
             case AUD_CH_ALL: /* MASTER_VOLUME */
                 u2ShmIndex = D_VOL_DEC3;
                 u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC3;
                 break;

             default:
                 return;
         }
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = D_VOL_DEC4;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC4;
    }
#endif

    if (u2UopIndex != 0)
    {
        // valid setting
        vWriteShmUINT32(u1DspId, u2ShmIndex, u4VolumeShm);
        DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
    }
#ifdef CC_AUD_DDI
    if (u1DecId == AUD_DEC_AUX) 
    {
        switch(eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_LS;
                u2UopIndex = UOP_DSP_TRIM_LS;
                vWriteShmUINT32(u1DspId, u2ShmIndex, u4VolumeShm);
                DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_RS;
                u2UopIndex = UOP_DSP_TRIM_RS;
                vWriteShmUINT32(u1DspId, u2ShmIndex, u4VolumeShm);
                DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
                break;
            default:
                return;
        }
    }
#endif

    // Out port vol mute
    if (u1DecId == AUD_DEC_MAIN)
    {
        // Speaker
        fgMute = ((u4ReadShmUINT32(AUD_DSP0, D_VOL) == 0) ||
            ((u4ReadShmUINT32(AUD_DSP0, D_TRIM_L) == 0) && (u4ReadShmUINT32(AUD_DSP0, D_TRIM_R) == 0)));
        _AUD_SetOutPortVolMute(AUD_SPEAKER, fgMute, TRUE);


        // Headphone
        fgMute = ((u4ReadShmUINT32(AUD_DSP0, D_TRIM_CH9) == 0) && (u4ReadShmUINT32(AUD_DSP0, D_TRIM_CH10) == 0));
        _AUD_SetOutPortVolMute(AUD_HEADPHONE, fgMute, TRUE);


        // Spdif, AV Out, Monitor Scart
        fgMute = ((u4ReadShmUINT32(AUD_DSP0, D_TRIM_CH7) == 0) && (u4ReadShmUINT32(AUD_DSP0, D_TRIM_CH8) == 0));
        _AUD_SetOutPortVolMute(AUD_SPDIF, fgMute, TRUE);
        _AUD_SetOutPortVolMute(AUD_AV_OUT, fgMute, TRUE);
        _AUD_SetOutPortVolMute(AUD_MONITOR_SCART, fgMute, TRUE);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        // TV scart
        fgMute = ((u4ReadShmUINT32(AUD_DSP0, D_VOL_DEC2) == 0) || 
                ((u4ReadShmUINT32(AUD_DSP0, D_TRIM_L_DEC2) == 0) && (u4ReadShmUINT32(AUD_DSP0, D_TRIM_R_DEC2) == 0)));
        _AUD_SetOutPortVolMute(AUD_TV_SCART, fgMute, TRUE);
    }
}

static void _AudDspChannelVolChange(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eChl)
{
    AUD_DEC_STREAM_FROM_T eStreamSrc;
    UINT8  u1Value;
    UINT32 u4Volumn, u4Volume100;
    UINT64 u8ChlVol;
    UINT32 u4ChlMute = FALSE;
    UINT32 u4HdmiChgMute = FALSE;
#ifdef CC_AUD_HDMI_SPDIF_CFG
    UINT32 u4HdmiMuteAnalogOut = FALSE;
#endif
    UINT8 u1DecPlayMute = 1;
    BOOL bVolMuteFlag = FALSE;
#ifdef CC_MPEG_LEVEL_ADJ
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;
#endif

#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,For flash-lite clip using DEC3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif
    UNUSED(u1Value);

    VERIFY(eChl < AUD_CH_NUM);
    eStreamSrc = _aeStreamSrc[u1DecId];

#ifdef CC_AUD_NEW_VOL_BASE_LEVEL
    if (u1DecId == AUD_DEC_MAIN && (eChl == AUD_CH_BYPASS_LEFT || eChl == AUD_CH_BYPASS_RIGHT) &&
        _aau1ChannelVolume[u1DecId][eChl] > _u1NewVolBaseLevel)
    {
        _aau1ChannelVolume[u1DecId][eChl] = _u1NewVolBaseLevel;
    }

#endif

    // Channel volume = ChannelMute * Balance * ExtraGain

    // Caculate balance ..
    if (!_fgChannelVolUseShmRawData)
    {
#ifdef CC_AUD_DDI
        u4Volumn = _aau1ChannelVolumeNew[u1DecId][eChl];
#else
        u1Value = _BalancedVolume(eChl, _au1Balance[u1DecId], _aau1ChannelVolume[u1DecId][eChl]);
        if (u1Value > MAX_VOL_LEVEL)
        {
            u1Value = MAX_VOL_LEVEL;
        }
#if (!defined(CC_AUD_NEW_CV_TABLE) || defined(CC_SONY_2K14_FLASH_AQ))
        u4Volumn = _au4VOLUME[u1Value];
#else
#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
        u4Volumn = _au4CHVOLUME[u1Value];
#else
        if ((u1DecId == AUD_DEC_MAIN) && ((eChl == AUD_CH_FRONT_LEFT) || (eChl == AUD_CH_FRONT_RIGHT)))
        {
            u4Volumn = _au4VOLUME[u1Value];
        }
        else
        {
            u4Volumn = _au4CHVOLUME[AUD_NVM_COMMON_VOL_TBL][u1Value];
        }
#endif
#endif
#endif
#ifdef CC_AUD_NEW_CV_TABLE
        if ((eChl == AUD_CH_REAR_LEFT) || (eChl == AUD_CH_REAR_RIGHT) || (eChl == AUD_CH_SUB_WOOFER))
        {
            //_au4CHVOLUME is for Line out & Subwoofer voume table for Sony 2K14
            u4Volumn = _au4CHVOLUME[u1Value];
        }
#endif
#ifdef CC_AUD_NEW_HP_VOL_TABLE
        if ((eChl == AUD_CH_DMX_LEFT) || (eChl == AUD_CH_DMX_RIGHT))
        {
            u4Volumn = _au4HpVolume[u1Value];
        }
#endif
    }
    else
    {
        // Notes that channel volume with raw data doen't support balance function
        u4Volumn = _aau4ChannelVolumeRawData[u1DecId][eChl];
    }

#ifdef CC_MPEG_LEVEL_ADJ
    AUD_GetDecodeType(u1DspId, AUD_DEC_AUX, &eStreamFrom, &eDecType);

    if ((eStreamSrc == AUD_STREAM_FROM_DIGITAL_TUNER) && \
    ((eDecType == AUD_FMT_AAC) || (eDecType == AUD_FMT_MPEG)) && ((eChl == AUD_CH_FRONT_LEFT) \
     || (eChl == AUD_CH_FRONT_RIGHT) || (eChl == AUD_CH_FRONT_RIGHT) || (eChl == AUD_CH_DMX_LEFT) || (eChl == AUD_CH_DMX_RIGHT)))
    {
        u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(((-1)*((INT32)_aau1MpgLvlAdj*2)))));
        u4Volumn = (UINT32)u8Div6432(u8ChlVol, (UINT64)VOL_SHM_0_DB, NULL);                
    }
#endif

    // Caculate extra gain
    if (eStreamSrc == AUD_STREAM_FROM_LINE_IN)
    {
        u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai1ChlVolExtraGain[u1DecId][eChl])));
    }
    else
    {
        u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai1ChlVolExtraGainNonLineIn[u1DecId][eChl])));
    }
    u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
    // Caculate DolbyGainTB11
    u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai1ChlVolDolbyGainTB11[eChl])));
    u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);

    // if play AD and update MAIN DECODER left/right channel volume, apply pan value to MAIN DECODER left/right channel volume
    if ((u1DecId == AUD_DEC_THIRD) && ((eChl == AUD_CH_FRONT_LEFT) || (eChl == AUD_CH_FRONT_RIGHT)))
    {
        // if didn't play AD, don't apply Pan value
        if ((eStreamSrc == AUD_STREAM_FROM_DIGITAL_TUNER) && (AUD_DRVGetAudioState(u1DspId, u1DecId) == AUD_ON_PLAY))
        {
            u8ChlVol = (UINT64)u4Volumn * ((UINT64)_DbToShmPan(_aau1AdPanVol[eChl]));
            u4Volumn = (UINT32)u8Div6432(u8ChlVol, (UINT64)VOL_SHM_0_DB, NULL);
        }
    }

#ifdef CC_AUD_VOLUME_OFFSET
    // Apply volume offset
    if ((eChl == AUD_CH_DMX_LEFT) || (eChl == AUD_CH_DMX_RIGHT))
    {   // HP final offset = HP volume offset + volume offset
        u4Volumn = (UINT32)(((UINT64)u4Volumn * (UINT64)(_DbToShm((INT32)_aai1ChannelVolOffset[u1DecId][eChl]+(INT32)_aai1ChannelVolOffset[u1DecId][AUD_CH_ALL]))) /
            (UINT64)VOL_SHM_0_DB);    
    }
    else if ((eChl == AUD_CH_REAR_LEFT) || (eChl == AUD_CH_REAR_RIGHT))
    {   // Line out case. check variable or fixed mode.
        if(_aafgLineOutVolChangeFlag)
        {
            u4Volumn = (UINT32)(((UINT64)u4Volumn * (UINT64)(_DbToShm((INT32)_aai1ChannelVolOffset[u1DecId][eChl]+(INT32)_aai1ChannelVolOffset[u1DecId][AUD_CH_ALL]))) /
                (UINT64)VOL_SHM_0_DB);
        }
    }
#ifdef CC_S_SUBWOFFER_SUPPORT
    else if ((eChl == AUD_CH_CENTER) || (eChl == AUD_CH_SUB_WOOFER))
    {     // Subwoofer final offset = Subwoofer volume offset + volume offset
             u4Volumn = (UINT32)(((UINT64)u4Volumn * (UINT64)(_DbToShm((INT32)_aai1ChannelVolOffset[u1DecId][eChl]+(INT32)_aai1ChannelVolOffset[u1DecId][AUD_CH_ALL]))) /
            (UINT64)VOL_SHM_0_DB);       
    }
#endif 
    else
    {
        u4Volumn = (UINT32)(((UINT64)u4Volumn * (UINT64)(_DbToShm((INT32)_aai1ChannelVolOffset[u1DecId][eChl]))) /
            (UINT64)VOL_SHM_0_DB);
    }
#endif        
    // Caculate channel mute
    // u4Volumn = _aafgChlMute[u1DecId][eChl] ? 0 : u4Volumn;

    if ((!_aafgChlMute[u1DecId][eChl]) &&
        _aafgChlOutPort[u1DecId][eChl] &&
        (!_afgAllChannelMute[u1DecId]) &&
        (!_aafgSoundModeMute[u1DecId][eChl]) &&
        (!_aafgSurroundMute[u1DecId][eChl]) &&
        (!_aafgVBassMute[u1DecId][eChl]))
    {
        u4ChlMute = 1;
    }
    else
    {
        u4ChlMute = 0;
    }

#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
    if ((eChl == AUD_CH_REAR_LEFT)||(eChl == AUD_CH_REAR_RIGHT))
    {

            VERIFY(eStreamSrc < AUD_STREAM_FROM_NUM);
            u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai2InputSrcLineOutVol[u1DecId][eStreamSrc])));
            u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
            u4Volumn *= u4ChlMute;

            LOG(7, "Bypass Channle Vol = %x\n", u4Volumn);
    }
#endif /* CC_AUD_INPUT_SRC_LINEOUT_VOL */
#ifdef CC_AUD_NEW_HP_VOL_TABLE
    if ((eChl == AUD_CH_DMX_LEFT) || (eChl == AUD_CH_DMX_RIGHT))
    {
        u4Volume100 = _au4HpVolume[100];
    }
    else
#endif
    {
        // Limit to 100 index value
#ifdef CC_AUD_NEW_CV_TABLE
        u4Volume100 = _au4CHVOLUME[100];
#else
        u4Volume100 = _au4VOLUME[100];
#endif
    }

    UNUSED(u4Volume100);

    u4HdmiChgMute = _aafgChlHdmiChgFmtMute[u1DecId][eChl] ? 0 : 1;

    u1DecPlayMute = _aafgChDecPlayMute[u1DecId][eChl] ? 0 : 1;

    if (eStreamSrc == AUD_STREAM_FROM_HDMI)
    {
        u4Volumn = u4ChlMute *  u4HdmiChgMute * u4Volumn * u1DecPlayMute;
    }
    else
    {
        u4Volumn = u4ChlMute *  u4Volumn * u1DecPlayMute;
    }

#ifdef CC_AUD_HDMI_SPDIF_CFG
    u4HdmiMuteAnalogOut = _aafgChlHdmiMuteAnalogOut[u1DecId][eChl] ? 0 : 1;
    if (eStreamSrc == AUD_STREAM_FROM_HDMI)
    {
        u4Volumn = u4Volumn * u4HdmiMuteAnalogOut;
    }
#endif
#if 0//def CC_ENABLE_AOMX
    if((eChl == AUD_CH_DMX_LEFT)&& (!_afgIecEnable)) //(eStreamSrc == AUD_STREAM_FROM_MEMORY)
    {
        UINT32 u4TrimValue;
        if ((UINT8)DRVCUST_OptGet(eAudioDownmixPosition) == DOWNMIX_CH_BEFORE_AVC)
        {
            u4TrimValue = u4Volumn << 3;
            if (u4TrimValue > 0x07fffff)
            {
                u4TrimValue = 0x07fffff;
            }
        }
        else
        {
            u4TrimValue = u4Volumn;
        }
        if (u4Volumn !=  0)
            u4TrimValue = _au4LGSEVOLUME[u1Value];
        if (eStreamSrc == AUD_STREAM_FROM_DIGITAL_TUNER)//japan clip volume
        {
            u4TrimValue = u4TrimValue/2;
        }
        if (AUD_GetMultimediaMode(u1DecId) != AUD_MM_MODE_GAME)
        {
            AUD_DspSetGstGain(u4TrimValue);
        }
    }
#endif

    if(eChl == AUD_CH_FRONT_LEFT || eChl == AUD_CH_ALL)
    {
        if(u4Volumn == 0)
        {
            bVolMuteFlag = TRUE;
        }
        if(_aeVolMuteFlag != bVolMuteFlag)
        {
#if 0
            if(bVolMuteFlag)
            {
                DRVCUST_SendAudEvent(E_CUST_AUD_SPEAKER_MUTE_REQUEST, u1DecId); //BE_FIXME
            }
            else
            {
                DRVCUST_SendAudEvent(E_CUST_AUD_SPEAKER_UNMUTE_REQUEST, u1DecId); //BE_FIXME
            }                
            DRVCUST_SendAudEvent(E_CUST_AUD_CH_MUTE_STATE_CHANGE, u1DecId); //BE_FIXME            
#endif            
            _aeVolMuteFlag = bVolMuteFlag;
        }       
    } 
    _VolumeCtrl(u1DspId, u1DecId, eChl, u4Volumn);
}

static void _AudDspChannelVolChangeNew(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eChl, UINT8 u1Value, UINT8 u1Value1)
{
    AUD_DEC_STREAM_FROM_T eStreamSrc;
    UINT32 u4Volumn;
    UINT64 u8ChlVol;
    UINT32 u4ChlMute = FALSE;
    UINT32 u4HdmiChgMute = FALSE;
#ifdef CC_AUD_HDMI_SPDIF_CFG
    UINT32 u4HdmiMuteAnalogOut = FALSE;
#endif
    UINT8 u1DecPlayMute = 1;

#ifndef CC_MT5391_AUD_3_DECODER
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    VERIFY(eChl < AUD_CH_NUM);
    eStreamSrc = _aeStreamSrc[u1DecId];

    // Channel volume = ChannelMute * Balance * ExtraGain
    // Caculate balance ..
    if (u1Value > 157)//actually not large than +30dB
    {
        u1Value = 157;
    }

    if (u1Value1 >15)
    {
        u1Value1 = 15;
    }
    
    if(eChl == AUD_CH_ALL)
    {
        // Check if mute
        if (eStreamSrc == AUD_STREAM_FROM_HDMI) //HDMI source
        {
            // Apply mute control
            u4ChlMute =  (_afgMuteEnable[u1DecId] || _afgHdmiModeMuteEnable[u1DecId] ||_afgStopMuteEnable[u1DecId]||_aafgChlMute[u1DecId][AUD_CH_ALL]) ? TRUE : FALSE;
            if (u4ChlMute)
            {
                LOG(9, "**** AUD_MUTE ****\n");
            }
            else
            {
                LOG(9, "**** AUD_UnMUTE ****\n");
            }
        }
        else //Other source;
        {
            u4ChlMute = (_afgMuteEnable[u1DecId]||_afgStopMuteEnable[u1DecId]||_aafgChlMute[u1DecId][AUD_CH_ALL])? TRUE : FALSE;
        }

        u8ChlVol = ((UINT64)_au4CHVOLUMENEW[u1Value]) * ((UINT64)_au4CHVOLUMENEW1[u1Value1]);
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
#ifdef CC_AUD_DDI
        _aau1ChannelVolumeNew[u1DecId][eChl] = u4Volumn;
#endif
        u4Volumn = u4ChlMute ? 0 : u4Volumn;

    }
    else
    {
        u8ChlVol = ((UINT64)_au4CHVOLUMENEW[u1Value]) * ((UINT64)_au4CHVOLUMENEW1[u1Value1]);
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);

        // Caculate balance

        if (_au1Balance[AUD_DEC_MAIN] != (MAX_BALANCE_LEVEL/2))
        {
            if (_au1Balance[AUD_DEC_MAIN] < (MAX_BALANCE_LEVEL/2))
            {
                if ((eChl == AUD_CH_FRONT_RIGHT) ||
                    ((eChl == AUD_CH_DMX_RIGHT) &&
                     ((UINT8)DRVCUST_OptGet(eAudioDownmixWithBalance)==TRUE)))
                {
                    u4Volumn = ((_au1Balance[AUD_DEC_MAIN]*2) * u4Volumn) / MAX_VOL_LEVEL;
                }
            }
            else
            {
                if ((eChl == AUD_CH_FRONT_LEFT) ||
                    ((eChl == AUD_CH_DMX_LEFT) &&
                     ((UINT8)DRVCUST_OptGet(eAudioDownmixWithBalance)==TRUE)))
                {
                    u4Volumn = (((MAX_VOL_LEVEL - _au1Balance[AUD_DEC_MAIN])*2) * u4Volumn) / MAX_VOL_LEVEL;
                }
            }
        }
#ifdef CC_AUD_DDI
        _aau1ChannelVolumeNew[u1DecId][eChl] = u4Volumn;
#endif

        // Caculate extra gain
        u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai1ChlVolExtraGain[u1DecId][eChl])));
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
        // Caculate DolbyGainTB11
        u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai1ChlVolDolbyGainTB11[eChl])));
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);

        // if play AD and update MAIN DECODER left/right channel volume, apply pan value to MAIN DECODER left/right channel volume
        if ((u1DecId == AUD_DEC_THIRD) && ((eChl == AUD_CH_FRONT_LEFT) || (eChl == AUD_CH_FRONT_RIGHT)))
        {
            // if didn't play AD, don't apply Pan value
            if ((eStreamSrc == AUD_STREAM_FROM_DIGITAL_TUNER) && (AUD_DRVGetAudioState(u1DspId, u1DecId) == AUD_ON_PLAY))
            {
                u8ChlVol = (UINT64)u4Volumn * ((UINT64)_DbToShmPan(_aau1AdPanVol[eChl]));
                u4Volumn = (UINT32)u8Div6432(u8ChlVol, (UINT64)VOL_SHM_0_DB, NULL);
            }
        }

        // Caculate channel mute
        // u4Volumn = _aafgChlMute[u1DecId][eChl] ? 0 : u4Volumn;

        if ((!_aafgChlMute[u1DecId][eChl]) &&
            _aafgChlOutPort[u1DecId][eChl] &&
            (!_afgAllChannelMute[u1DecId]) &&
            (!_aafgSoundModeMute[u1DecId][eChl]))
        {
            u4ChlMute = 1;
        }
        else
        {
            u4ChlMute = 0;
        }

    #ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
        if ((eChl == AUD_CH_BYPASS_LEFT)||(eChl == AUD_CH_BYPASS_RIGHT))
        {

                VERIFY(eStreamSrc < AUD_STREAM_FROM_NUM);
                u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai2InputSrcLineOutVol[u1DecId][eStreamSrc])));
                u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
                u4Volumn *= u4ChlMute;

                LOG(7, "Bypass Channle Vol = %x\n", u4Volumn);

    #ifdef CC_AUD_LINEOUT_VOL_OFFSET


                VERIFY(eChl < AUD_CH_NUM);
                u1Value = _aau1ChannelVolOffset[u1DecId][eChl];
                if (u1Value >MAX_VOL_LEVEL) u1Value = MAX_VOL_LEVEL;

                u8ChlVol = (UINT64)u4Volumn * (UINT64)(_au4CHVOLUME[u1Value]);
                u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);

                LOG(7, "Bypass Channle Vol offset = %x\n", u4Volumn);
    #endif

        }
    #endif /* CC_AUD_INPUT_SRC_LINEOUT_VOL */

        u4HdmiChgMute = _aafgChlHdmiChgFmtMute[u1DecId][eChl] ? 0 : 1;

        //u1DecPlayMute = _aafgChDecPlayMute[u1DecId][eChl] ? 0 : 1;

        if (eStreamSrc == AUD_STREAM_FROM_HDMI)
        {
            u4Volumn = u4ChlMute *  u4HdmiChgMute * u4Volumn * u1DecPlayMute;
        }
        else
        {
            u4Volumn = u4ChlMute *  u4Volumn * u1DecPlayMute;
        }

    #ifdef CC_AUD_HDMI_SPDIF_CFG
        u4HdmiMuteAnalogOut = _aafgChlHdmiMuteAnalogOut[u1DecId][eChl] ? 0 : 1;
        if (eStreamSrc == AUD_STREAM_FROM_HDMI)
        {
            u4Volumn = u4Volumn * u4HdmiMuteAnalogOut;
        }
    #endif
    #if 0//def CC_ENABLE_AOMX
        if((eChl == AUD_CH_DMX_LEFT)&& (!_afgIecEnable)) //(eStreamSrc == AUD_STREAM_FROM_MEMORY)
        {
            UINT32 u4TrimValue;
            if ((UINT8)DRVCUST_OptGet(eAudioDownmixPosition) == DOWNMIX_CH_BEFORE_AVC)
            {
                u4TrimValue = u4Volumn << 3;
                if (u4TrimValue > 0x07fffff)
                {
                    u4TrimValue = 0x07fffff;
                }
            }
            else
            {
                u4TrimValue = u4Volumn;
            }
            if (u4Volumn !=  0)
                u4TrimValue = _au4LGSEVOLUME[u1Value];
            if (eStreamSrc == AUD_STREAM_FROM_DIGITAL_TUNER)//japan clip volume
            {
                u4TrimValue = u4TrimValue/2;
            }
            if (AUD_GetMultimediaMode(u1DecId) != AUD_MM_MODE_GAME)
            {
                AUD_DspSetGstGain(u4TrimValue);
            }
        }
    #endif
    } 

    _VolumeCtrl(u1DspId, u1DecId, eChl, u4Volumn);

}

static void _AudDspMasterVolChange(UINT8 u1DecId)
{
    AUD_DEC_STREAM_FROM_T eStreamSrc;
    UINT32 u4ValShm;
    UINT64 u8ValueShm;
    BOOL fgMute;

    eStreamSrc = _aeStreamSrc[u1DecId];

    // Apply AGC flag and calculate adjusted volume
#ifdef CC_AUD_A1
    _au4AdjVol[u1DecId] = VOL_SHM_0_DB;
#else
    if (_afgUserAgcFlag[u1DecId])
    {
        UINT64 u8AgcAdj;
        u8AgcAdj = (_aafgInputSrcAgcFlag[u1DecId][AUD_EU_ATV_STREAM_SRC_TRANSFER(eStreamSrc)] && _afgUserAgcFlag[u1DecId]) ?
                   (UINT64)_aau4InputSrcAgcVol[u1DecId] [AUD_EU_ATV_STREAM_SRC_TRANSFER(eStreamSrc)] : (UINT64)VOL_SHM_0_DB;
        _au4AdjVol[u1DecId] = (UINT32)(u8AgcAdj);
    }
    else
    {
        _au4AdjVol[u1DecId] = VOL_SHM_0_DB;
    }
#endif
    // Check if mute
    if (eStreamSrc == AUD_STREAM_FROM_HDMI) //HDMI source
    {
        // Apply mute control
        fgMute =  (_afgMuteEnable[u1DecId] || _afgHdmiModeMuteEnable[u1DecId] ||_afgStopMuteEnable[u1DecId]||_aafgChlMute[u1DecId][AUD_CH_ALL]) ? TRUE : FALSE;
        if (fgMute)
        {
            LOG(9, "**** AUD_MUTE ****\n");
        }
        else
        {
            LOG(9, "**** AUD_UnMUTE ****\n");
        }
    }
    else //Other source;
    {
        fgMute = (_afgMuteEnable[u1DecId]||_afgStopMuteEnable[u1DecId]||_aafgChlMute[u1DecId][AUD_CH_ALL])? TRUE : FALSE;
    }

    // Master volume = Mute * AdjVol * Master volume
    if (_fgMasterVolUseShmRawData)
    {
        u8ValueShm = (((UINT64)_au4MasterVolShm[u1DecId]) * ((UINT64)_au4AdjVol[u1DecId]))/(UINT64)VOL_SHM_0_DB;
    }
    else
    {
#ifdef CC_AUD_DDI
        u8ValueShm = _aau1ChannelVolumeNew[u1DecId][AUD_CH_ALL];
#else
        if (AUD_DEC_AUX == u1DecId)
        {
            u8ValueShm = (UINT64)_au4AdjVol[u1DecId];
        }
        else
        {
            u8ValueShm = (((UINT64)_au4VOLUME[_aau1ChannelVolume[u1DecId][AUD_CH_ALL]]) * ((UINT64)_au4AdjVol[u1DecId]))/(UINT64)VOL_SHM_0_DB;
        }
#endif
    }

#ifdef CC_AUD_VOLUME_OFFSET
    // Apply volume offset
    u8ValueShm = ((UINT64)u8ValueShm * (UINT64)(_DbToShm((INT32)_aai1ChannelVolOffset[u1DecId][AUD_CH_ALL]))) / (UINT64)VOL_SHM_0_DB;
#endif
    u4ValShm = (UINT32)u8ValueShm;

#if 0 //def CC_AUD_PANFADE_SUPPORT
    // if play Ad and update MAIN decoder master volume, apply fade value to to master volume
    if (u1DecId == AUD_DEC_MAIN)
    {
        // if didn't play AD, don't apply Fade value
        if (eStreamSrc == AUD_STREAM_FROM_DIGITAL_TUNER)
        {
            u8ValueShm = (UINT64)u4ValShm * ((UINT64)(_DbToShmFade(_u1AdFadeVolume)));
            u4ValShm = (UINT32)u8Div6432(u8ValueShm, (UINT64)VOL_SHM_0_DB, NULL);
        }
    }
#endif

    if (u4ValShm > _au4VOLUME[100])
    {
        u4ValShm = _au4VOLUME[100];
    }
#ifdef CC_AUD_SX1_FEATURE
    if (u1DecId == AUD_DEC_THIRD)
    {    
        u8ValueShm = (UINT64)u4ValShm * (UINT64)(_DbToShmAd((_aau4AdLvlAdj * 30 + 50) / 100));
        u4ValShm = (UINT32)(u8ValueShm / VOL_SHM_0_DB);
        LOG(7, "AD Mixed Vol offset = %x\n", u4ValShm);
    }
#endif
    u4ValShm = fgMute ? 0 : u4ValShm;

    // Send UOP to DSP
    _VolumeCtrl(AUD_DSP0, u1DecId, AUD_CH_ALL, u4ValShm);

    // Auto turn off speaker when master volume is 0
    if ((DRVCUST_OptGet(eAudioSpkAutoOffWhenVolMute) == 1) && (u1DecId == AUD_DEC_MAIN))
    {
        if (u4ValShm == 0)
        {
            // Wait DSP fade out
            x_thread_delay(50);
            ADAC_VolumeMuteSpeakerEnable(FALSE);
            LOG(8, "**** SPK Auto off due to volume is 0\n");;
        }
        else
        {
            ADAC_VolumeMuteSpeakerEnable(TRUE);
            LOG(8, "**** SPK Auto on due to volume is not 0\n");;
        }
    }
}

#ifdef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
static void _AudDspSetChVol(UINT8 u1DecId, AUD_CH_T eChl)
{
    UINT8   u1MasterVol;
    UINT8   u1ChIdOffset;
    UINT32  u4Volumn;
    UINT64  u8ChlVol;
    AUD_NVM_VOL_TBL_T rVolTbl;
    
    AUD_DEC_ID_VALIDATE(u1DecId);

    if ((u1DecId == AUD_DEC_MAIN) && ((eChl == AUD_CH_FRONT_LEFT) || (eChl == AUD_CH_FRONT_RIGHT)))
    {
        u1ChIdOffset = eChl - AUD_CH_FRONT_LEFT;
        u4Volumn = VOL_SHM_0_DB;
        
        // Caculate volume curve gain
    #ifdef CC_AUD_DDI
        u1MasterVol = _aau1ChannelVolumeNew[u1DecId][eChl];
    #else
        u1MasterVol = _aau1ChannelVolume[u1DecId][AUD_CH_ALL];
    #endif
        
        if (_au4VOLUME[u1MasterVol] > 0)
        {
#ifdef CC_AUD_CHL_VOL_MODE
            if (_fgAudSkype)
            {
                rVolTbl = AUD_NVM_L_VOL_TBL_SKYPE + u1ChIdOffset;
            }
            else
#endif
            {
                rVolTbl = AUD_NVM_L_VOL_TBL + u1ChIdOffset;
            }
            
            u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_au4CHVOLUME[rVolTbl][u1MasterVol]));
            u4Volumn = (UINT32)u8Div6432(u8ChlVol, (UINT64)_au4VOLUME[u1MasterVol], NULL);
        }

        // If volume change, update it
        if (u4Volumn != u4ReadShmUINT32(D_VOL_L + (u1ChIdOffset << 2)))
        {
            vWriteShmUINT32((D_VOL_L + (u1ChIdOffset << 2)), u4Volumn);
            DSP_SendDspTaskCmd(UOP_DSP_VOL_L + (u1ChIdOffset << 8));
        }
    }
}
#endif

#ifdef CC_AUD_DDI
static void _AudDspBluetoothVolChange(UINT8 u1DecId, UINT8 u1Value, UINT8 u1Value1)
{
    UINT32 u4Volumn;
    UINT64 u8ChlVol;
    
    UNUSED(u1DecId);

    if (u1Value >157)//actually not large than +30dB
    {
        u1Value = 157;
    }

    if (u1Value1 >15)
    {
        u1Value1 = 15;
    }
    u8ChlVol = ((UINT64)_au4CHVOLUMENEW[u1Value]) * ((UINT64)_au4CHVOLUMENEW1[u1Value1]);
    u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
    vWriteShmUINT32(AUD_DSP0, D_UPLOAD_VOL, u4Volumn);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_BLUETOOTH_VOL);


}
#endif

static void _AudDspSrcVolChange(UINT8 u1DecId, UINT32 u4Value)
{
    UINT16 u2ShmIndex = D_INSRC_GAIN;
    UINT16 u2UopIndex = UOP_DSP_INPUT_SRC_GAIN;
#ifdef CC_AUD_ARM_RENDER //CC_AUD_APROC_POST
    UINT32 u4Idx;
    INT32 i4Vol;
#endif         

#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE(u1DecId);
#else
    AUD_DEC_ID_VALIDATE(u1DecId);
#endif

#ifdef CC_AUD_ARM_RENDER //CC_AUD_APROC_POST
    UNUSED(u2ShmIndex);
    UNUSED(u2UopIndex);

    u4Idx = APROC_IOCTR_TRIM_INPUT0;

    if (u1DecId == AUD_DEC_AUX)
    {
        u4Idx = APROC_IOCTR_TRIM_INPUT1;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4Idx = APROC_IOCTR_TRIM_INPUT2;
    }
#endif
    
    i4Vol = _AudDspToAprocVol(u4Value);
    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_TRIM, u4Idx, &i4Vol, 1);
        
#else

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = D_INSRC_GAIN_DEC2;
        u2UopIndex = UOP_DSP_INPUT_SRC_GAIN_DEC2;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = D_INSRC_GAIN_DEC3;
        u2UopIndex = UOP_DSP_INPUT_SRC_GAIN_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = D_INSRC_GAIN_DEC4;
        u2UopIndex = UOP_DSP_INPUT_SRC_GAIN_DEC4;
    }
#endif

    vWriteShmUINT32(AUD_DSP0, u2ShmIndex, u4Value);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
#endif //CC_AUD_ARM_RENDER

}

static void _AudDspAgcEnalbe(UINT8 u1DecId, BOOL fgEnable)
{
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST

    if (_afgAgcEnable[AUD_DEC_MAIN] != fgEnable)
    {
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_3DRC_FLAG, (UINT32*)&fgEnable, 0);
        
    }
    _afgAgcEnable[AUD_DEC_MAIN] = fgEnable;

#else
    UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;

    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if (u1DecId == AUD_DEC_MAIN)
    {
        if (_afgAgcEnable[AUD_DEC_MAIN] != fgEnable)
        {
            if (fgEnable)
            {
                vWriteShmUINT16(AUD_DSP0, W_AVC_FLAG,0x23); // enable AVC slow mode for UI enable/disable
            }
            else
            {
                vWriteShmUINT16(AUD_DSP0, W_AVC_FLAG,0);
            }
            DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
            LOG(8, "AUD AGC FLAG = %x\n", fgEnable);
            //AUD_DspProcMode(u1DecId, (fgEnable == TRUE) ? 0xc00 : 0xfc00); // FIXME !!
        }
        _afgAgcEnable[AUD_DEC_MAIN] = fgEnable;
    }
    else // 2nd decoder doesn't support AVC (post-processing)
    {
    #ifndef CC_AUD_AVC_V20
        if (_afgAgcEnable[AUD_DEC_AUX] != fgEnable)
        {
            if (fgEnable)
            {
                vWriteShmUINT16(AUD_DSP0, W_AVC_FLAG_DEC2,0x23); // enable AVC slow mode for UI enable/disable
            }
            else
            {
                vWriteShmUINT16(AUD_DSP0, W_AVC_FLAG_DEC2,0);
            }
            DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
            LOG(8, "AUD AGC FLAG = %x\n", fgEnable);
            //AUD_DspProcMode(u1DecId, (fgEnable == TRUE) ? 0xc00 : 0xfc00); // FIXME !!
        }
    #endif
        _afgAgcEnable[AUD_DEC_AUX] = fgEnable;
    }
#endif
}

static void _AudDspIecConvertIecFlag (UINT8 u1DecId ,AUD_IECINFO_T *SetIecInfo, AUD_IEC_T *eIecFlag_p, BOOL *bLoadTrans_p)
{
    AUD_IEC_T eIecFlag;
    BOOL bLoadTrans;
    AUD_FMT_T eDecFormat = SetIecInfo->eDecFormat;
    AUD_IEC_T eIecCfg = SetIecInfo->eIecCfg;
    bLoadTrans = FALSE; // default do not load transcoder
    eIecFlag = AUD_IEC_CFG_PCM; // default PCM
    
    if (eIecCfg == AUD_IEC_CFG_RAW)
    {
#ifdef CC_MT5391_AUD_3_DECODER
        if (eDecFormat == AUD_FMT_AC3)
        {
            // all input sources, AC3 can output RAW
            eIecFlag = AUD_IEC_CFG_RAW;
        }
        else if (eDecFormat == AUD_FMT_AAC)
        {
#if defined(CC_AUD_DOLBY_SUPPORT_DDCO) || defined(CC_AUD_DOLBY_SUPPORT_DDT)
            if (_AUD_DspIsDDCOEnable())
            {
                if (SetIecInfo->eStreamSrc == AUD_STREAM_FROM_DIGITAL_TUNER)
                {
#ifndef CC_ENABLE_AOMX
                    // for source = DTV
                    if (_IsTriOnlyDecMode())
                    {
                        u1DecId = AUD_DEC_THIRD;
                    }
                    else
                    {
                        u1DecId = AUD_DEC_AUX;
                    }
#endif					
                    if ((AUD_GetSampleFreq(u1DecId) == FS_48K)
#ifdef  CC_AUD_DOLBY_SUPPORT_DDT
                       ||(AUD_GetSampleFreq(u1DecId) == FS_32K)
#endif
                        )
                    {
                        if (!(DRVCUST_OptGet(eAudioMultichlDDCO) &&
                            (u1GetAacChNum(u1DecId) <= 2)))
                        {
                            eIecFlag = AUD_IEC_CFG_RAW;
                            bLoadTrans = TRUE;
                            LOG(5,"Enable DDCO by IEC\n");
                        }
                    }
                }
                else
                {

#ifndef CC_ENABLE_AOMX				
                    // for source = MM, HDMI...
                    if (_IsTriOnlyDecMode())
                    {
                        u1DecId = AUD_DEC_THIRD;
                    }
                    else
                    {
                        u1DecId = AUD_DEC_MAIN;
                    }
#endif					
                    if ((AUD_GetSampleFreq(u1DecId) == FS_48K)
#ifdef  CC_AUD_DOLBY_SUPPORT_DDT
                        ||(AUD_GetSampleFreq(u1DecId) == FS_32K)
#endif
                        )
                    {
                        eIecFlag = AUD_IEC_CFG_RAW;
                        bLoadTrans = TRUE;
                    }
                }
            }
            else
            {
                    //AAC bypass RAW
                    eIecFlag = AUD_IEC_CFG_RAW;
                    bLoadTrans = FALSE;
            }
#else   //!defined(CC_AUD_DOLBY_SUPPORT_DDCO) || defined(CC_AUD_DOLBY_SUPPORT_DDT)
            if (!_AUD_DspIsDDCOEnable())
            {
                // IC is not bonded with DDCO
                //AAC bypass RAW
                eIecFlag = AUD_IEC_CFG_RAW;
                bLoadTrans = FALSE;
            }
#endif  //defined(CC_AUD_DOLBY_SUPPORT_DDCO) || defined(CC_AUD_DOLBY_SUPPORT_DDT)

        }
        else if (eDecFormat == AUD_FMT_DTS)
        {
                eIecFlag = AUD_IEC_CFG_RAW;
                bLoadTrans = TRUE;
                LOG(5, "Enable DTS encoder \n");
        }
#else // CC_MT5391_AUD_3_DECODER
        if (eDecFormat == AUD_FMT_AC3)
        {
                eIecFlag = AUD_IEC_CFG_RAW;
            }
            else if (eDecFormat == AUD_FMT_DTS)
            {
                eIecFlag = AUD_IEC_CFG_RAW;
                bLoadTrans = TRUE;
            }
#endif // CC_MT5391_AUD_3_DECODER
        }

    *eIecFlag_p = eIecFlag;
    *bLoadTrans_p = bLoadTrans;
}

static void _AudDspSetIec(AUD_IEC_T eIecCfg, BOOL fgEnable)
{
    UINT8 u1fgMute;
    AUD_IEC_T eIecFlag;
    BOOL bLoadTrans;
    AUD_DEC_STREAM_FROM_T eStreamSrc;
    AUD_FMT_T eDecFormat;
    SPDIF_REG_TYPE_T eSpdifRegType;
    UINT8 u1Copyright;
    UINT8 u1CategoryCode;
    UINT8 u1WordLength;
    AUD_IEC_RAWSRC_T eRawSource = IEC_RAW_SRC_OFF; // bit0 = dec1, bit1 = dec2
    SPDIF_BURST_INFO_T eBurstInfo;
    UINT16 u2Nsnum;
    AUD_INPUT_ID_T eInputId = _AUD_DspGetAvInputSrcId();
    CHAR *pAudFmt = "";
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    UINT32 u4Reg0;
    UINT32 u4Mode = 0; //bit0: 1: DTS CD, 0: others, bit
    APROC_IEC_CHANNELSTATUS_T  eInfo; 
#endif    
    UINT32 u4Factor;
    UINT32 u4FrameSample;
    UINT32 u4RawDelayFrame;

#ifdef CC_AUD_AD_FORCE_PCM      
    /* Check if AD enable and MIX to L/R ,force to PCM format*/
    if((uReadShmUINT8(B_ADFLAG) & 0x09) == 0x09)
    if(((uReadShmUINT8(B_ADFLAG) & 0x09) == 0x09) || AUD_IsSourceFromWFD(AUD_DEC_MAIN))
    {
        eIecCfg = AUD_IEC_CFG_PCM;
        LOG(3, " SPDIF format force to PCM when AD playing and Wifi display\n");
    }
#endif

    /* Get stream source and user setting */
    eStreamSrc = _aeStreamSrc[AUD_DEC_MAIN]; // only need to check main decoder
#ifdef CC_AUD_DDI
    eStreamSrc = _aeStreamSrc[_u1SpdifRawDec]; // only need to check main decoder
#endif    
    if (eStreamSrc >= AUD_STREAM_FROM_NUM)
    {
        eStreamSrc = AUD_STREAM_FROM_OTHERS;
    }

    // DVI case, convert stream source
    if ((eStreamSrc == AUD_STREAM_FROM_LINE_IN) &&
        (eInputId >= AUD_INPUT_ID_HDMI_0) && (eInputId <= AUD_INPUT_ID_DVI_4))
    {
        eStreamSrc = AUD_STREAM_FROM_OTHERS;
    }

    _gSetIecInfo.eStreamSrc  = eStreamSrc;
    _gSetIecInfo.eIecCfg= eIecCfg;

    /* Get decoder format */
#ifdef  CC_MT5391_AUD_3_DECODER
    switch (eStreamSrc)
    {
    case AUD_STREAM_FROM_DIGITAL_TUNER:
        eDecFormat = _AudGetStrFormat(AUD_DEC_AUX);
        break;
    default:
        eDecFormat = _AudGetStrFormat(AUD_DEC_MAIN);
        break;
    }
    #ifdef CC_AUD_LINEIN_USE_DEC3
    if (_IsTriOnlyDecMode())
    {
        eDecFormat = _AudGetStrFormat(AUD_DEC_THIRD);
    }
    #endif
#else
    eDecFormat = _AudGetStrFormat(AUD_DEC_MAIN);
#endif
#ifdef CC_AUD_DDI
    eDecFormat = _AudGetStrFormat(_u1SpdifRawDec);
#endif

    _gSetIecInfo.eDecFormat = eDecFormat;

    /* Convert IEC type for RAW type */
    _AudDspIecConvertIecFlag (_u1SpdifRawDec, &_gSetIecInfo, &eIecFlag, &bLoadTrans);

    eSpdifRegType = SPDIF_REG_TYPE_USER;
    if (eSpdifRegType >= SPDIF_REG_TYPE_NUM)
    {
        eSpdifRegType = SPDIF_REG_TYPE_DEFAULT;
    }
    _gSetIecInfo.eSpdifRegType = eSpdifRegType;

    // HDMI input, by source, update channel status
    if ((eStreamSrc == AUD_STREAM_FROM_HDMI) &&
     (_aSpdifInfo.eUpdateMode[eSpdifRegType][(UINT8)eStreamSrc] == IEC_CHANNEL_STATUS_BY_SOURCE))
    {
        // HDMI, by source mode, set channel status by HDMI driver
        BOOL fgCopyright, fgIsPcm;
        UINT8 u1WordLengthTmp;
        UNUSED(fgIsPcm);
        UNUSED(u1WordLengthTmp);

        vHDMIAudChannelStatus (&fgCopyright, &u1CategoryCode, &u1WordLengthTmp, &fgIsPcm);
        if (fgCopyright)
        {
            u1Copyright = IEC_COPY_NEVER;
        }
        else
        {
            u1Copyright = IEC_COPY_FREELY;
        }
        _aSpdifInfo.u1Copyright[eSpdifRegType][(UINT8)eStreamSrc] = u1Copyright;
    }

    /* get spdif information */
    u1Copyright = _aSpdifInfo.u1Copyright[eSpdifRegType][(UINT8)eStreamSrc];
    u1CategoryCode = _aSpdifInfo.u1Category[eSpdifRegType][(UINT8)eStreamSrc];
    u1WordLength = _aSpdifInfo.u1WordLength[eSpdifRegType][(UINT8)eStreamSrc];

    // setup channel status to shared info....
    vWriteShmUINT8(AUD_DSP0, B_IEC_COPYRIGHT, u1Copyright);
    vWriteShmUINT8(AUD_DSP0, B_IEC_CATEGORY_CODE, u1CategoryCode);
    vWriteShmUINT8(AUD_DSP0, B_IEC_WORD_LENGTH, u1WordLength);

    /* Load transcoder */
    switch (eDecFormat)
    {
    case AUD_FMT_AAC:
        //_AUD_DspSetDDCOEnable(bLoadTrans);
        break;
    case AUD_FMT_DTS:
        //if (_AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_DTS)
        if (eStreamSrc == AUD_STREAM_FROM_DIGITAL_TUNER)
            _AUD_DspSetDTSENCEnable(bLoadTrans);
        break;
    default:
        break;
    }

    /* set RAW data output source */
    if (eIecFlag)
    {
        switch (eStreamSrc)
        {
        case AUD_STREAM_FROM_DIGITAL_TUNER:
#ifdef  CC_MT5391_AUD_3_DECODER
            eRawSource = IEC_RAW_SRC_DEC2_ON; // dec1 off, dec2 on
#else
            eRawSource = IEC_RAW_SRC_DEC1_ON; // dec1 on, dec2 off
#endif
            break;
        default:
            eRawSource = IEC_RAW_SRC_DEC1_ON; // dec1 on, dec2 off
            break;
        }
        #ifdef CC_AUD_LINEIN_USE_DEC3
        if (_IsTriOnlyDecMode())
        {
            eRawSource = IEC_RAW_SRC_DEC3_ON; // dec1 on, dec2 off
        }
        #endif
#ifdef CC_AUD_DDI
        switch (_u1SpdifRawDec)
        {
        case AUD_DEC_MAIN:
            eRawSource = IEC_RAW_SRC_DEC1_ON;
            break;
        case AUD_DEC_AUX:
            eRawSource = IEC_RAW_SRC_DEC2_ON;
            break;
        case AUD_DEC_THIRD:
            eRawSource = IEC_RAW_SRC_DEC3_ON;
            break;
        default:
            eRawSource = IEC_RAW_SRC_DEC1_ON;
            break; 
        }
#endif        
    }

    /*set HDMI */
    if (eStreamSrc == AUD_STREAM_FROM_HDMI)
    {
        fgEnable = _aafgHdmiSetSpdifFlag && _aafgMWSetSpdifFlag && (!_afgIecMute);//&&_aafgPlayMuteSetSpdifFlag;
    }
    else
    {
        fgEnable = _aafgMWSetSpdifFlag && (!_afgIecMute);//&&_aafgPlayMuteSetSpdifFlag;
    }
    vHDMISpdifOutEnable(fgEnable);
    u1fgMute = (fgEnable) ? FALSE : TRUE;

    /* set IEC output status */
    {
        LOG (3,"Set SPDIF = %d, rc = 0x%x)\n", eIecFlag, eRawSource);

        _AUD_SetOutPortHwMute(AUD_SPDIF, u1fgMute, TRUE);
        
        vWriteShmUINT8(AUD_DSP0, B_IECFLAG, eIecFlag);
        vWriteShmUINT8(AUD_DSP0, B_IEC_MUTE, u1fgMute);
        vWriteShmUINT8(AUD_DSP0, B_IEC_RAWSRC, (UINT8)eRawSource);

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        u4Reg0 = eRawSource>>4;
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_FLAG, (UINT32 *) &u4Reg0, 1); 
        u4Reg0 = APROC_RAW_PCM;
#endif
        
        if (eIecFlag == AUD_IEC_CFG_RAW)
        {
            switch (eDecFormat)
            {
            case AUD_FMT_AC3:
                pAudFmt = "AC3"; 
                eBurstInfo = BURST_INFO_AC3;
                u2Nsnum = 0x600; // 1536 samples
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
                u4Reg0 = APROC_RAW_AC3;
#endif                
                break;
            case    AUD_FMT_AAC: // DDCO
                if (_AUD_DspIsDDCOEnable())
                {
                    pAudFmt = "AC3(DDCO)"; 
                    eBurstInfo = BURST_INFO_AC3;
                    u2Nsnum = 0x600; // 1536 samples
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
                    u4Reg0 = APROC_RAW_AC3;
#endif                        
                }
                else
                {
                    pAudFmt = "AAC"; 
                    eBurstInfo = BURST_INFO_AAC;
                    u2Nsnum = 0x400;//0x400; // 1024 samples
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
                    u4Reg0 = APROC_RAW_AAC;
#endif                        
                }
                break;
            case AUD_FMT_DTS:
                pAudFmt = "DTS";
                eBurstInfo = BURST_INFO_DTS;
                u2Nsnum = 0x200; // 512 samples
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
                if (AUD_GetDTSInfo() == DEC_LITTLE_ENDIAN)
                {
                    u4Mode = 0x1;             
                }
                switch (AUD_GetDTSFrameSize())
                {
                case 1024:
                    eBurstInfo = BURST_INFO_DTS_1024;
                    u4Reg0 = APROC_RAW_DTS_1024;
                    u2Nsnum = 0x400;
                    break;
                case 2048:
                    eBurstInfo = BURST_INFO_DTS_2048;
                    u4Reg0 = APROC_RAW_DTS_2048;
                    break;
                    u2Nsnum = 0x800; 
                case 512: 
                default:
                    u2Nsnum = 0x200; 
                    eBurstInfo = BURST_INFO_DTS_512;
                    u4Reg0 = APROC_RAW_DTS_512;                    
                    break;
                }
#endif                    
                break;
            default: // error case
                eBurstInfo = BURST_INFO_NULL_DATA;
                u2Nsnum = 0x0;
                break;
           }
        }
        else
        { // PCM
            pAudFmt = "PCM";
            eBurstInfo = BURST_INFO_NULL_DATA;
            u2Nsnum = 0x100; // 256 samples
        }

        LOG(0, "SPDIF Output AUD_DEC%d, Enable(%d) Codec = %s\n", _u1SpdifRawDec, fgEnable, pAudFmt);

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_MODE, (UINT32 *) &u4Mode, 1); 
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_RAWAUDFMT, (UINT32 *)&u4Reg0, 1);

        _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_CHANNELSTATUS, (UINT32 *) &eInfo, 10);
        eInfo.u4CategoryCode = u1CategoryCode;
        eInfo.u4CopyRight = u1Copyright;
        switch(u1WordLength)  
        {
        case IEC_WORDLENGTH_24BITS:
            eInfo.u4WordLength = SPDIF_WORDLENGTH_MAX24_24BITS;
            break;
        case IEC_WORDLENGTH_20BITS:
            eInfo.u4WordLength = SPDIF_WORDLENGTH_MAX24_20BITS;
            break;
        case IEC_WORDLENGTH_16BITS:
            eInfo.u4WordLength = SPDIF_WORDLENGTH_MAX20_16BITS;
            break;
        default:
            eInfo.u4WordLength = SPDIF_WORDLENGTH_MAX20_NOT_INDICATE;
            break;
        }
        if (eIecFlag == AUD_IEC_CFG_RAW)
        {  
            eInfo.u4DigitalInfo = 1; //raw
            eInfo.u4PcmRawInfo = IEC_CFG0_PCM_RAW_ISRAW >> IEC_CFG0_PCM_RAW_BIT;
            switch (AUD_GetSampleFreq(_u1SpdifRawDec))
            {
            case FS_32K:
                eInfo.u4SampleRate = IEC_CFG1_SAMPLERATE_32K;
                break;
            case FS_44K:
                eInfo.u4SampleRate = IEC_CFG1_SAMPLERATE_44K;
                break;
            default:
                eInfo.u4SampleRate = IEC_CFG1_SAMPLERATE_48K;
                break;
            }
        }
        else
        {
            eInfo.u4DigitalInfo = 0; //pcm
            eInfo.u4PcmRawInfo = IEC_CFG0_PCM_RAW_ISPCM >> IEC_CFG0_PCM_RAW_BIT;
            eInfo.u4SampleRate = IEC_CFG1_SAMPLERATE_48K;
        }
    	_vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_CHANNELSTATUS, (UINT32 *) &eInfo, 10);
#endif        
        vWriteShmUINT16(AUD_DSP0, W_IEC_BURST_INFO, eBurstInfo);
        vWriteShmUINT16(AUD_DSP0, W_IEC_NSNUM, u2Nsnum);
    }

#ifdef CC_AUD_DDI
				switch (AUD_GetSampleFreq(_u1SpdifRawDec))
				{
				case FS_192K:
					u4Factor = 192;
					break;
				case FS_96K:
					u4Factor = 96;
					break;
				case FS_48K:
					u4Factor = 48;
					break;
				case FS_44K:
					u4Factor = 44;
					break;
				case FS_32K:
					u4Factor = 32;
					break;
				default:
					u4Factor = 48; 
					break;
				}
				
				switch (u2ReadShmUINT16(AUD_DSP0, W_IEC_BURST_INFO))
				{
				case BURST_INFO_AC3:
				case BURST_INFO_DTS:
				case BURST_INFO_AAC:
					if ((u4FrameSample = u2ReadShmUINT16(AUD_DSP0, W_IEC_NSNUM)) != 0)
					{
						//u4RawDelayFrame = (u2DelayBypassMix * 5 * u4Factor / 34 + u4FrameSample/2)/u4FrameSample;
						u4RawDelayFrame = ((u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SPDIF_PCM))*u4DelayConstant) * 5 * u4Factor / 17 + u4FrameSample)/(2*u4FrameSample);
						if ((_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_HDMI) &&
							(uReadShmUINT8(AUD_DSP0, B_IECFLAG) != 0) &&
							(_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_DTS))
						{
							u4RawDelayFrame = u4RawDelayFrame*2;
						}
	                    if(u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_RAWDELAY)) != u4RawDelayFrame)
	                    	{
						_vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_RAWDELAY, (UINT32 *) &u4RawDelayFrame, 1); 
	                    	}
					}
				    break;
				default:
					break;
				}
#endif
    if ((_aeIecMode != eIecFlag) || (_afgIecEnable != fgEnable) || (_aeIecRawSrc != eRawSource))
    {
        /* set IEC clock */
        if ((eIecFlag==AUD_IEC_CFG_RAW))
        {
            AUD_WRITE32(REG_IEC_DIV, IEC_AUDCK_SEL | IEC_CLK_INDEPENDENT_SEL | IEC_LRCK_CYCLE | IEC_UPDATE_SEL_2T); // Sync from [DTV00094645] (IEC_UPDATE_SEL fine tune)
        }
        else
        {
            AUD_WRITE32(REG_IEC_DIV, (AUD_READ32(REG_IEC_DIV) & (~IEC_AUDCK_SEL)) & (~IEC_CLK_INDEPENDENT_SEL));
        }

        DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_IEC_FLAG);

        _aeIecMode = eIecFlag;
        _afgIecEnable = fgEnable;
        _aeIecRawSrc = eRawSource;
    }


}


static void _AudSetSPDIFEnable(BOOL fgEnable, BOOL fgLight)
{
    UINT8 u1fgMute;
        //check HDMI's command
    if (_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_HDMI)
    {
        fgEnable = _aafgHdmiSetSpdifFlag && _aafgMWSetSpdifFlag && (!_afgIecMute);//&&_aafgPlayMuteSetSpdifFlag;
    }
    else
    {
        fgEnable = _aafgMWSetSpdifFlag && (!_afgIecMute);//&&_aafgPlayMuteSetSpdifFlag;
    }

    if (fgEnable)
    {
        u1fgMute = FALSE;
    }
    else
    {
        u1fgMute = TRUE;
    }
    vHDMISpdifOutEnable(fgEnable);
    if(_afgIecEnable != fgEnable)
    {
        vWriteShmUINT8(AUD_DSP0, B_IEC_MUTE, u1fgMute);
    }
    _afgIecEnable = fgEnable;
    AUD_SPDIF_Enable(fgLight);
    if((fgEnable) && _fgHpPlugFlag)
    {
         _fgHpPlugFlag = FALSE;
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspVolumeChange
 *   Setup master volume, or input source gain, or channel trim to DSP.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  eAudChl  : 0: front left channel.
 *                              1: front right channel.
 *                              2: rear left channel.
 *                              3: rear right channel.
 *                              4: center channel.
 *                              5: sub woofer channel.
 *                              6: bypass left channel.
 *                              7: bypass right channel.
 *                              8: downmix left channel.
 *                              9: downmix right channel.
 *                            10: aux front left channel.
 *                            11: aux front right channel.
 *                            12: input source gain.
 *                            13: all channels    ( For master volume control ).
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspVolumeChange(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eAudChl)
{
    AUD_DEC_STREAM_FROM_T eStreamSrc;

#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE(u1DecId);
#else
    AUD_DEC_ID_VALIDATE(u1DecId);
#endif

    if (eAudChl >= AUD_CH_NUM)
    {
        return;
    }

    eStreamSrc = AUD_EU_ATV_STREAM_SRC_TRANSFER(_aeStreamSrc[u1DecId]);

    if (eAudChl == AUD_CH_ALL)
    {
        _AudDspMasterVolChange(u1DecId);
    }
    else if (eAudChl == AUD_CH_INPUT_SRC)
    {
        INT32 i4InputSrcVol;

        #ifndef CC_AUD_INPUT_SRC_VOL_OFFSET
        i4InputSrcVol = _aai2InputSrcVol[u1DecId][eStreamSrc];
        #else
        i4InputSrcVol = _aai2InputSrcVol[u1DecId][eStreamSrc] + _aai2InputSrcVolOffset[u1DecId][eStreamSrc];
        #endif
        if (eStreamSrc == AUD_STREAM_FROM_LINE_IN)
        {
            // i4InputSrcVol = InputSrcVol + AvInputSrcVol (in dB)
            i4InputSrcVol += _ai2AvInputSrcVol[AUD_DspGetAvInputSrcId()];
            _AudDspSrcVolChange(u1DecId, _DbToShm(i4InputSrcVol));
        }
        else
        {
            _AudDspSrcVolChange(u1DecId, _DbToShm(i4InputSrcVol));
        }
    }
#ifndef CC_MT5391_AUD_3_DECODER   // gallen 0821,
    else    // channel volume
#else
    else if ((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX) || (u1DecId == AUD_DEC_THIRD))
#endif
    {
        _AudDspChannelVolChange(u1DspId, u1DecId, eAudChl);
    }
}

#ifdef CC_AUD_CHL_VOL_MODE
//-----------------------------------------------------------------------------
/** AUD_DspLRChannelVolMode
 *   Set LR channel trim mode to DSP.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  u1Mode  : 0: Normal mode.
 *                              1: Skeype mode.
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspLRChannelVolMode(UINT8 u1DecId, UINT8 u1Mode)
{
    UNUSED(_fgAudSkype);
    if (u1Mode > 1)
    {
        LOG(1, "_AUD_DspLRChannelVolMode, invlid mode %d\n", u1Mode);
        u1Mode = 0;
    }

    // Always main decoder
    u1DecId = AUD_DEC_MAIN;

    VOL_CTL_SEMA_LOCK(u1DecId);
    // u1Mode = 1, skype mode
    _fgAudSkype = (u1Mode == 1) ? TRUE : FALSE;
#ifdef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
    _AudDspSetChVol(u1DecId, AUD_CH_FRONT_LEFT);
    _AudDspSetChVol(u1DecId, AUD_CH_FRONT_RIGHT);
#endif
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}
#endif

void _AUD_DspCfgSetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    UNUSED(eDecFmt);
    AUD_DEC_ID_VALIDATE(u1DecId);

    VOL_CTL_SEMA_LOCK(u1DecId);   
   
   if( _aeStreamSrc[u1DecId] != eStreamFrom) //BE_FIXME         
   {       
       _aeStreamSrc[u1DecId] = eStreamFrom;
       if(u1DecId==AUD_DEC_MAIN)
       {
           DRVCUST_SendAudEvent(E_CUST_AUD_SOURCE_CHANGE, u1DecId);
       }
   }   
#ifndef CC_AUD_DDI
    if (u1DecId<= AUD_DEC_AUX)
    {
        // Adjust agc
        if ((_aafgInputSrcAgcFlag[u1DecId][eStreamFrom] && _afgUserAgcFlag[u1DecId]))
        {
            _AudDspAgcEnalbe(u1DecId, TRUE);
        }
        else
        {
            _AudDspAgcEnalbe(u1DecId, FALSE);
        }
    }
#endif

#ifdef CC_MT5391_AUD_3_DECODER
    if (u1DecId < AUD_DEC_THIRD)
#endif
    {
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_ALL);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_INPUT_SRC);
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_REAR_LEFT);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_REAR_RIGHT);
#endif

#ifdef CC_AUD_USE_FLASH_AQ
        _AUD_FlashAQOperation(u1DecId,AQ_TYPE_CH_VOLUME, AQ_OP_EXE, (UINT16)eStreamFrom);
#endif //CC_AUD_USE_FLASH_AQ

    }
    VOL_CTL_SEMA_UNLOCK(u1DecId);
    // DolbyGainTB11 (Dolby Technical Bulletin 11.5)
    LOG(5, "_AUD_DspCfgSetDecType\n");
    _AUD_DspDolbyGainTB11(u1DecId);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** AUD_DspFineTuneVolume
 *  Fine tune master volume/trim, and set it to DSP
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u4ShmValue       Volume (0x20000 is equal to 0 dBFs, max value: 0x7fffff)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4ShmValue)
{
    UINT16 u2ShmIndex = 0;
    UINT16 u2UopIndex = 0;
    UINT32 u4VolumeShm = 0; /* 0 ~ 0x7fffff */

     UINT64 u8Vol;

    // Share memory = volume * adjust_ratio
    u8Vol = ((UINT64)u4ShmValue *(UINT64)_au4AdjVol[u1DecId])/(UINT64)VOL_SHM_0_DB;
    u4VolumeShm = (UINT32)u8Vol;

    if (u1DecId == AUD_DEC_MAIN)
    {
        switch (eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L;
                u2UopIndex = UOP_DSP_TRIM_L;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R;
                u2UopIndex = UOP_DSP_TRIM_R;
                break;
            case AUD_CH_REAR_LEFT:
                u2ShmIndex = D_TRIM_LS;
                u2UopIndex = UOP_DSP_TRIM_LS;
                break;
            case AUD_CH_REAR_RIGHT:
                u2ShmIndex = D_TRIM_RS;
                u2UopIndex = UOP_DSP_TRIM_RS;
                break;
            case AUD_CH_CENTER:
                u2ShmIndex = D_TRIM_C;
                u2UopIndex = UOP_DSP_TRIM_C;
                break;
            case AUD_CH_SUB_WOOFER:
                u2ShmIndex = D_TRIM_SUB;
                u2UopIndex = UOP_DSP_TRIM_SUBWOOFER;
                break;
            case AUD_CH_DMX_LEFT:
                u2ShmIndex = D_TRIM_CH9;
                u2UopIndex = UOP_DSP_TRIM_CH9;
                break;
            case AUD_CH_DMX_RIGHT:
                u2ShmIndex = D_TRIM_CH10;
                u2UopIndex = UOP_DSP_TRIM_CH10;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
                u2ShmIndex = D_VOL;
                u2UopIndex = UOP_DSP_MASTER_VOLUME;
                break;
            default:
                break;
        }
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = D_VOL_DEC2;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC2;

        switch(eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L_DEC2;
                u2UopIndex = UOP_DSP_TRIM_L_DEC2;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R;
                u2UopIndex = UOP_DSP_TRIM_R_DEC2;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
                u2ShmIndex = D_VOL_DEC2;
                u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC2;
                break;

            default:
                return;
        }
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = D_VOL_DEC3;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = D_VOL_DEC4;
        u2UopIndex = UOP_DSP_MASTER_VOLUME_DEC4;
    }
#endif

    vWriteShmUINT32(AUD_DSP0, u2ShmIndex, u4VolumeShm);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}

UINT32 _AUD_DspGetFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel)
{
    UINT16 u2ShmIndex = 0;
    UINT32 u4VolumeShm = 0; /* 0 ~ 0x7fffff */

    if (u1DecId == AUD_DEC_MAIN)
    {
        switch (eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R;
                break;
            case AUD_CH_REAR_LEFT:
                u2ShmIndex = D_TRIM_LS;
                break;
            case AUD_CH_REAR_RIGHT:
                u2ShmIndex = D_TRIM_RS;
                break;
            case AUD_CH_CENTER:
                u2ShmIndex = D_TRIM_C;
                break;
            case AUD_CH_SUB_WOOFER:
                u2ShmIndex = D_TRIM_SUB;
                break;
            case AUD_CH_BYPASS_LEFT:
                u2ShmIndex = D_TRIM_CH7;
                break;
            case AUD_CH_BYPASS_RIGHT:
                u2ShmIndex = D_TRIM_CH8;
                break;
            case AUD_CH_DMX_LEFT:
                u2ShmIndex = D_TRIM_CH9;
                break;
            case AUD_CH_DMX_RIGHT:
                u2ShmIndex = D_TRIM_CH10;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
                u2ShmIndex = D_VOL;
                break;
            default:
                break;
        }
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = D_VOL_DEC2;
        switch(eChannel)
        {
            case AUD_CH_FRONT_LEFT:
                u2ShmIndex = D_TRIM_L_DEC2;
                break;
            case AUD_CH_FRONT_RIGHT:
                u2ShmIndex = D_TRIM_R_DEC2;
                break;
            case AUD_CH_ALL: /* MASTER_VOLUME */
            default:
                u2ShmIndex = D_VOL_DEC2;
                break;
        }
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = D_VOL_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = D_VOL_DEC4;
    }
#endif

    if (u2ShmIndex != 0)
    {
      u4VolumeShm = u4ReadShmUINT32(AUD_DSP0, u2ShmIndex);
    }

    return u4VolumeShm;
}

//-----------------------------------------------------------------------------
/** AUD_DspVolumeInit
 *  Initialize all variables for volume control
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspVolumeInit(void)
{
    static BOOL fgInit = FALSE;

#ifdef CC_AUD_USE_FLASH_AQ
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_SP_VOL, AQ_OP_INIT, 0);
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_LO_VOL, AQ_OP_INIT, 0);
#ifdef  CC_SONY_2K14_FLASH_AQ
    if(_au1MWSetPositionIdx==1)
    {
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_MTKDRC_TABLE, AQ_OP_INIT, 0);
    }
    else    
    {
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_MTKDRC_WALL, AQ_OP_INIT, 0);    
    }
#else
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_MTKDRC, AQ_OP_INIT, 0);
#endif
#endif // CC_AUD_USE_FLASH_AQ
    // Init volume value
    if (!fgInit)
    {
        UINT32 u4Ptr;
        UINT8  u1DecId;
        UINT8  u1ChlNum;

        // Initialize customization table
        #ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai2InputSrcVol), (const VOID *)(rAudFlashAQ.aai2AudInputSrcVol), sizeof(_aai2InputSrcVol));
        #else
        #ifdef CC_AUD_USE_FLASH_AQ
        if(! _fgAQ_Enable[AQ_TYPE_SRC_VOLUME] )
        #endif
        {
            VERIFY(DRVCUST_OptQuery(eAudioInputVolTable, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_aai2InputSrcVol), (const VOID *)u4Ptr, sizeof(_aai2InputSrcVol));
        }
        #endif
#ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_au4VOLUME), (const VOID *)(rAudFlashAQ.au4VolumeTable), sizeof(_au4VOLUME));
    #ifdef CC_AUD_NEW_CV_TABLE
        x_memcpy((VOID *)VIRTUAL((UINT32)_au4CHVOLUME), (const VOID *)(rAudFlashAQ.au4ChannelVolumeTable), sizeof(_au4CHVOLUME));
    #endif
#elif !defined(CC_AUD_USE_FLASH_AQ)
        VERIFY(DRVCUST_OptQuery(eAudioVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_au4VOLUME), (const VOID *)u4Ptr, sizeof(_au4VOLUME));

#ifdef CC_AUD_NEW_CV_TABLE
        VERIFY(DRVCUST_OptQuery(eAudioChannelVolTable, (UINT32 *)&u4Ptr) == 0);
#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
        x_memcpy((VOID *)VIRTUAL(_au4CHVOLUME), (const VOID *)u4Ptr, sizeof(_au4CHVOLUME));
#else
        x_memcpy((VOID *)VIRTUAL(_au4CHVOLUME[AUD_NVM_COMMON_VOL_TBL]), (const VOID *)u4Ptr, sizeof(_au4CHVOLUME[AUD_NVM_COMMON_VOL_TBL]));
        x_memcpy((VOID *)VIRTUAL(_au4CHVOLUME[AUD_NVM_L_VOL_TBL]), (const VOID *)u4Ptr, sizeof(_au4CHVOLUME[AUD_NVM_L_VOL_TBL]));
        x_memcpy((VOID *)VIRTUAL(_au4CHVOLUME[AUD_NVM_R_VOL_TBL]), (const VOID *)u4Ptr, sizeof(_au4CHVOLUME[AUD_NVM_R_VOL_TBL]));
        x_memcpy((VOID *)VIRTUAL(_au4CHVOLUME[AUD_NVM_L_VOL_TBL_SKYPE]), (const VOID *)u4Ptr, sizeof(_au4CHVOLUME[AUD_NVM_L_VOL_TBL_SKYPE]));
        x_memcpy((VOID *)VIRTUAL(_au4CHVOLUME[AUD_NVM_R_VOL_TBL_SKYPE]), (const VOID *)u4Ptr, sizeof(_au4CHVOLUME[AUD_NVM_R_VOL_TBL_SKYPE]));
#endif //CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
#endif //CC_AUD_NEW_CV_TABLE
#endif //SUPPORT_FLASH_AQ

#ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1ChlVolExtraGain), (const VOID *)rAudFlashAQ.aai1AudChVolExtraGain, sizeof(_aai1ChlVolExtraGain));
#else
        VERIFY(DRVCUST_OptQuery(eAudioChlVolExtraGain, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1ChlVolExtraGain), (const VOID *)u4Ptr, sizeof(_aai1ChlVolExtraGain));
#endif

        #if 0
        VERIFY(DRVCUST_OptQuery(eAudioChlVolExtraGainNonLineIn, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1ChlVolExtraGainNonLineIn), (const VOID *)u4Ptr, sizeof(_aai1ChlVolExtraGainNonLineIn));
        #else
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1ChlVolExtraGainNonLineIn), (const VOID *)VIRTUAL((UINT32)_aai1ChlVolExtraGain), \
                sizeof(_aai1ChlVolExtraGainNonLineIn));
        #endif
        
#ifdef CC_AUD_USE_NVM
        AUD_NVM_Operation(AUD_NVM_VOL_INIT, NULL, 0);   // Init _aai2InputSrcVol, _au4VOLUME, _au4CHVOLUME, _aai1ChlVolExtraGain[L/R], _aai1ChlVolExtraGainNonLineIn[L/R]
#endif //CC_AUD_USE_NVM

#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
        VERIFY(DRVCUST_OptQuery(eAudioInputLineOutVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai2InputSrcLineOutVol), (const VOID *)u4Ptr, sizeof(_aai2InputSrcLineOutVol));
#endif
        VERIFY(DRVCUST_OptQuery(eAudioAvInputSrcVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvInputSrcVol), (const VOID *)u4Ptr, sizeof(_ai2AvInputSrcVol));

        VERIFY(DRVCUST_OptQuery(eAudioInputSrcAgcFlag, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aafgInputSrcAgcFlag), (const VOID *)u4Ptr, sizeof(_aafgInputSrcAgcFlag));

        VERIFY(DRVCUST_OptQuery(eAudioInputSrcAgcVol, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aau4InputSrcAgcVol), (const VOID *)u4Ptr, sizeof(_aau4InputSrcAgcVol));

#ifndef CC_AUD_USE_FLASH_AQ
    #ifndef SUPPORT_FLASH_AQ
        #ifdef CC_AUD_NEW_HP_VOL_TABLE
        VERIFY(DRVCUST_OptQuery(eAudioHpVolTable, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL(_au4HpVolume), (const VOID *)u4Ptr, sizeof(_au4HpVolume));
        #endif        
    #endif
#else //CC_AUD_USE_FLASH_AQ
        if(! _fgAQ_Enable[AQ_TYPE_VOL_TABLE] )
        {
            VERIFY(DRVCUST_OptQuery(eAudioVolTable, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_au4VOLUME), (const VOID *)u4Ptr, sizeof(_au4VOLUME));
        }
        if(! _fgAQ_Enable[AQ_TYPE_LO_VOL_TABLE])
        {
    #ifdef CC_AUD_NEW_CV_TABLE
            VERIFY(DRVCUST_OptQuery(eAudioChannelVolTable, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_au4CHVOLUME), (const VOID *)u4Ptr, sizeof(_au4CHVOLUME));
    #endif
        }
        if(! _fgAQ_Enable[AQ_TYPE_HP_VOL_TABLE])
        {
    #ifdef CC_AUD_NEW_HP_VOL_TABLE
            VERIFY(DRVCUST_OptQuery(eAudioHpVolTable, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_au4HpVolume), (const VOID *)u4Ptr, sizeof(_au4HpVolume));
    #endif
        }        
#endif  // CC_AUD_USE_FLASH_AQ

        VERIFY(DRVCUST_OptQuery(eAudioPSMCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_arSoundModeCase), (const VOID *)u4Ptr, sizeof(_arSoundModeCase));

        _u1VolBaseLevel = DRVCUST_OptGet(eAudioVolBaseLevel);

#ifdef CC_AUD_LINEOUT_VOL_MODE

        _aau1LineOutFixVol = DRVCUST_OptGet(eAudioLineOutFixVol);
#endif

#ifdef CC_AUD_USE_FLASH_AQ
        if(! _fgAQ_Enable[AQ_TYPE_AVC] )
        {
            VERIFY(DRVCUST_OptQuery(eAudioAvcTarget, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcTargetLevel), (const VOID *)u4Ptr, sizeof(_ai2AvcTargetLevel));

            VERIFY(DRVCUST_OptQuery(eAudioAvcGainUp, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcGainUp), (const VOID *)u4Ptr, sizeof(_ai2AvcGainUp));

            VERIFY(DRVCUST_OptQuery(eAudioAvcAdjustRate, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcAdjRate), (const VOID *)u4Ptr, sizeof(_ai2AvcAdjRate));

            VERIFY(DRVCUST_OptQuery(eAudioAvcUiAdjustRate, (UINT32 *)&u4Ptr) == 0);
            x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcUiAdjRate), (const VOID *)u4Ptr, sizeof(_ai2AvcUiAdjRate));
        }
#else
    #ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcTargetLevel), (const VOID *)rAudFlashAQ.ai2AudAvcTarget, sizeof(_ai2AvcTargetLevel));
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcGainUp), (const VOID *)rAudFlashAQ.ai2AudAvcGainUp, sizeof(_ai2AvcGainUp));
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcAdjRate), (const VOID *)rAudFlashAQ.ai2AudAvcAdjRate, sizeof(_ai2AvcAdjRate));
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcUiAdjRate), (const VOID *)rAudFlashAQ.ai2AudAvcUiAdjRate, sizeof(_ai2AvcUiAdjRate));
    #else
        VERIFY(DRVCUST_OptQuery(eAudioAvcTarget, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcTargetLevel), (const VOID *)u4Ptr, sizeof(_ai2AvcTargetLevel));

        VERIFY(DRVCUST_OptQuery(eAudioAvcGainUp, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcGainUp), (const VOID *)u4Ptr, sizeof(_ai2AvcGainUp));

        VERIFY(DRVCUST_OptQuery(eAudioAvcAdjustRate, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcAdjRate), (const VOID *)u4Ptr, sizeof(_ai2AvcAdjRate));

        VERIFY(DRVCUST_OptQuery(eAudioAvcUiAdjustRate, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai2AvcUiAdjRate), (const VOID *)u4Ptr, sizeof(_ai2AvcUiAdjRate));
    #endif
#endif //CC_AUD_USE_FLASH_AQ

#ifdef CC_AUD_VOLUME_OFFSET
        VERIFY(DRVCUST_OptQuery(eAudioVolumeOffset, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_ai4VolOffsetMap), (const VOID *)u4Ptr, sizeof(_ai4VolOffsetMap));
#endif
#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
        VERIFY(DRVCUST_OptQuery(eAudioTvSysFineTuneVol, (UINT32 *)&u4Ptr) == 0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aau1TVFineTuneVolume), (const VOID *)u4Ptr, sizeof(_aau1TVFineTuneVolume));
#endif
        // Initialize local variables ..
        for (u1DecId = 0; u1DecId < (UINT8)AUD_DEC_NUM; u1DecId ++)
        {
            for (u1ChlNum = 0; u1ChlNum < (UINT8)AUD_CH_NUM; u1ChlNum++)
            {
                _aafgChlMute[u1DecId][u1ChlNum] = FALSE;
                _aafgChlOutPort[u1DecId][u1ChlNum] = TRUE;
                _aafgSurroundMute[u1DecId][u1ChlNum] = FALSE;
                _aafgVBassMute[u1DecId][u1ChlNum] = FALSE;
                _aau1ChannelVolume[u1DecId][u1ChlNum] = _u1VolBaseLevel;
#ifdef CC_AUD_VOLUME_OFFSET
                _aai1ChannelVolOffset[u1DecId][u1ChlNum] = 0;  
#endif
                _aafgChlHdmiChgFmtMute[u1DecId][u1ChlNum] = FALSE;
                _aau4ChannelVolumeRawData[u1DecId][u1ChlNum] = VOL_SHM_0_DB;
#ifdef CC_AUD_DDI
                _aau1ChannelVolumeNew[u1DecId][u1ChlNum] = VOL_SHM_0_DB;
                _aau1ChannelVolumeMain[u1DecId][u1ChlNum] = VOL_SHM_0_DB;
                _aau1ChannelVolumeFine[u1DecId][u1ChlNum] = VOL_SHM_0_DB;
#endif
                _aafgChDecPlayMute[u1DecId][u1ChlNum] = FALSE;
            }
        }

        VERIFY(x_sema_create(&_ahVolCtlSema[AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahVolCtlSema[AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#if 1 //def CC_MT5391_AUD_3_DECODER
        VERIFY(x_sema_create(&_ahVolCtlSema[AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif
#if 1 //def CC_AUD_4_DECODER_SUPPORT
        VERIFY(x_sema_create(&_ahVolCtlSema[AUD_DEC_4TH], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif

        _u2AdFadeMode = (((UINT16)DRVCUST_OptGet(eAudioPanFadeControl))<<9);

        // Update mixsound gain after table _au4VOLUME updated
        _AUD_DspSetMixSndVol(_u4MixsoundVol);

#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
        _afgAudRegDetection = DRVCUST_OptGet(eAudioAmpInitDetectRegEnable);
#endif
        fgInit = TRUE;
    }
}


//-----------------------------------------------------------------------------
/** AUD_DspEqCfgInit
 *  Initialize variables for equalizer preset configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspEqCfgInit(void)
{
    static BOOL fgInit = FALSE;
    UINT8 u1Idx;

    // Init volume value
    if (!fgInit)
    {
#ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1ChEqCfg), (const VOID *)rAudFlashAQ.aai1AudChlEqCfg, sizeof(_aai1ChEqCfg));
#else
#ifndef CC_AUD_USE_NVM
        UINT32 u4Ptr;
        VERIFY(DRVCUST_OptQuery(eAudioAvChlEqTable, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1ChEqCfg), (const VOID *)u4Ptr, sizeof(_aai1ChEqCfg));
#endif
#endif
        for(u1Idx = 0; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
        {
            _aai1ChEqCfgPrev[u1Idx] = 0x80;
        }
        VERIFY(x_sema_create(&_ahEqCtlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
        fgInit = TRUE;
    }
}

#ifdef CC_AUD_SOUND_MODE
//-----------------------------------------------------------------------------
/** AUD_DspPeqCfgInit
 *  Initialize variables for PEQ configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspPSMCfgInit(void)
{
    static BOOL fgInit = FALSE;

    // Init volume value
    if (!fgInit)
    {
        UINT32 u4Ptr;
        VERIFY(DRVCUST_OptQuery(eAudioPeqFrCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1PeqFq), (const VOID *)u4Ptr, sizeof(_aai1PeqFq));

        VERIFY(DRVCUST_OptQuery(eAudioPeqBwCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1PeqBw), (const VOID *)u4Ptr, sizeof(_aai1PeqBw));

        VERIFY(DRVCUST_OptQuery(eAudioPeqGainCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1PeqGain), (const VOID *)u4Ptr, sizeof(_aai1PeqGain));

        VERIFY(DRVCUST_OptQuery(eAudioBassTreble, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1BassTreble), (const VOID *)u4Ptr, sizeof(_aai1BassTreble));
        VERIFY(DRVCUST_OptQuery(eAudioPSMCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_arSoundModeCase), (const VOID *)u4Ptr, sizeof(_arSoundModeCase));

#ifdef CC_AUD_USE_FLASH_AQ
        UINT8 i,j,k;
        UINT16 u2Temp;
       if(_fgAQ_Enable[AQ_TYPE_PEQ])
       {
         VERIFY(FLASH_AQ_SRC_NUM*FLASH_AQ_SM_NUM < AUD_SM_NUM);
         for(i=0; i< FLASH_AQ_SRC_NUM ;i++)
         {
            for(k = 0; k<FLASH_AQ_SM_NUM; k++)
            {
                for(j=0; j< AQ_PEQ_ITEMS;j+=5)
                {
                    u2Temp =_au1AQ_PEQ[i][k][j]<<8;
                    u2Temp += _au1AQ_PEQ[i][k][j+1];
                    _aai1PeqFq[i*k][j/5] = u2Temp;

                    u2Temp = _au1AQ_PEQ[i][k][j+2]<<8;
                    u2Temp += _au1AQ_PEQ[i][k][j+3];
                    _aai1PeqBw[i*k][j/5] = u2Temp;

                    _aai1PeqGain[i*k][j/5] = (INT8)_au1AQ_PEQ[i][k][j+4];
                }
            }
         }
       }
       if(_fgAQ_Enable[AQ_TYPE_BASS_TREBLE])
       {
           VERIFY(sizeof(_aai1BassTreble) >= sizeof(_au1AQBassTrebleOffset));
           x_memcpy((VOID *)VIRTUAL((UINT32)_aai1BassTreble), (const VOID *)_au1AQBassTrebleOffset, sizeof(_au1AQBassTrebleOffset));
       }

#endif //CC_AUD_USE_FLASH_AQ
        VERIFY(x_sema_create(&_ahPSMCtlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
        fgInit = TRUE;
    }

}
#endif
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
void _AUD_DspVSMCfgInit(void)
{
    static BOOL fgInit = FALSE;

    // Init volume value
    if (!fgInit)
    {
        UINT32 u4Ptr;

        VERIFY(DRVCUST_OptQuery(eAudioVSModeCfg, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_aai1VSMode), (const VOID *)u4Ptr, sizeof(_aai1VSMode));
#ifdef CC_AUD_USE_FLASH_AQ
#ifdef CC_SONY_2K14_FLASH_AQ
        if(_au1MWSetPositionIdx==1)
        {
            if(_fgAQ_Enable[AQ_TYPE_MVS_TABLE])
            {
                VERIFY(sizeof(_aai1VSMode) >= sizeof(_au4AQ_MVS_TABLE));
                x_memcpy((VOID *)VIRTUAL((UINT32)_aai1VSMode), (const VOID *)_au4AQ_MVS_TABLE, sizeof(_au4AQ_MVS_TABLE));
            }
        }
        else
        {
            if(_fgAQ_Enable[AQ_TYPE_MVS_WALL])
            {
                VERIFY(sizeof(_aai1VSMode) >= sizeof(_au4AQ_MVS_WALL));
                x_memcpy((VOID *)VIRTUAL((UINT32)_aai1VSMode), (const VOID *)_au4AQ_MVS_WALL, sizeof(_au4AQ_MVS_WALL));
            }
        }           
                
#else
        if(_fgAQ_Enable[AQ_TYPE_MVS])
        {
            VERIFY(sizeof(_aai1VSMode) >= sizeof(_au4AQ_MVS));
            x_memcpy((VOID *)VIRTUAL((UINT32)_aai1VSMode), (const VOID *)_au4AQ_MVS, sizeof(_au4AQ_MVS));
        }
#endif
#endif

        VERIFY(x_sema_create(&_ahVSMCtlSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_UNLOCK) == OSR_OK);
        fgInit = TRUE;
    }
}

#endif
//-----------------------------------------------------------------------------
/** _AUD_DspChDelayInit
 *  Initialize table for channel delay configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChDelayInit(void)
{
    static BOOL fgInit = FALSE;

    // Init volume value
    if (!fgInit)
    {

#ifdef SUPPORT_FLASH_AQ
        x_memcpy((VOID *)VIRTUAL((UINT32)_au1ChDelay), (const VOID *)rAudFlashAQ.au1AudChDelay, sizeof(_au1ChDelay));
#else
        UINT32 u4Ptr;
        VERIFY(DRVCUST_OptQuery(eAudioChannelDelay, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)VIRTUAL((UINT32)_au1ChDelay), (const VOID *)u4Ptr, sizeof(_au1ChDelay));
#endif

        VERIFY(x_sema_create(&_ahChDelayCtlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
        fgInit = TRUE;
    }
}

#ifdef CC_AUD_SPIDF_LVL
//-----------------------------------------------------------------------------
/** _AUD_DspSpdifLevel
 *  Change spdif level
 *
 *  @param u1Level
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSpdifLevel(UINT8 u1Level)
{
    if (u1Level >= sizeof(_u1AudSpdifLevel))
    {
        LOG(1, "_AUD_DspSpdifLevel, invlid level %d\n", u1Level);
        u1Level = 0;
    }

    VOL_CTL_SEMA_LOCK(AUD_DEC_MAIN);
    _fgChannelVolUseShmRawData = FALSE;
    _aau1ChannelVolume[AUD_DEC_MAIN][AUD_CH_BYPASS_LEFT] = _u1AudSpdifLevel[u1Level];
    _aau1ChannelVolume[AUD_DEC_MAIN][AUD_CH_BYPASS_RIGHT] = _u1AudSpdifLevel[u1Level];

    _AudDspChannelVolChange(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_BYPASS_LEFT);
    _AudDspChannelVolChange(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_BYPASS_RIGHT);
    VOL_CTL_SEMA_UNLOCK(AUD_DEC_MAIN);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSpdifInit
 *  Initialize SPDIF configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSpdifInit(void)
{
    static BOOL fgInit = FALSE;

    if (!fgInit)
    {
        UINT32 u4Ptr;

        // For Default
        VERIFY(DRVCUST_OptQuery(eAudioIecCopyright, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)(&_aSpdifInfo.u1Copyright[SPDIF_REG_TYPE_DEFAULT][0]),
            (const VOID *)u4Ptr, sizeof(UINT8) * AUD_STREAM_FROM_NUM);
        VERIFY(DRVCUST_OptQuery(eAudioIecCategory, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)(&_aSpdifInfo.u1Category[SPDIF_REG_TYPE_DEFAULT][0]),
            (const VOID *)u4Ptr, sizeof(UINT8) * AUD_STREAM_FROM_NUM);
        VERIFY(DRVCUST_OptQuery(eAudioIecWordLength, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)(&_aSpdifInfo.u1WordLength[SPDIF_REG_TYPE_DEFAULT][0]),
            (const VOID *)u4Ptr, sizeof(UINT8) * AUD_STREAM_FROM_NUM);

        // For others, copy from default
        x_memcpy((VOID *)(&_aSpdifInfo.u1Copyright[SPDIF_REG_TYPE_USER][0]),
               (VOID *)(&_aSpdifInfo.u1Copyright[SPDIF_REG_TYPE_DEFAULT][0]),
               sizeof(UINT8) * AUD_STREAM_FROM_NUM);
        x_memcpy((VOID *)(&_aSpdifInfo.u1Category[SPDIF_REG_TYPE_USER][0]),
               (VOID *)(&_aSpdifInfo.u1Category[SPDIF_REG_TYPE_DEFAULT][0]),
               sizeof(UINT8) * AUD_STREAM_FROM_NUM);
        x_memcpy((VOID *)(&_aSpdifInfo.u1WordLength[SPDIF_REG_TYPE_USER][0]),
               (VOID *)(&_aSpdifInfo.u1WordLength[SPDIF_REG_TYPE_DEFAULT][0]),
               sizeof(UINT8) * AUD_STREAM_FROM_NUM);

        // By source or not
        VERIFY(DRVCUST_OptQuery(eAudioIecChannelStatusBySource, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)(&_aSpdifInfo.eUpdateMode[SPDIF_REG_TYPE_DEFAULT][0]),
            (const VOID *)u4Ptr, sizeof(UINT8) * AUD_STREAM_FROM_NUM);

        x_memcpy((VOID *)(&_aSpdifInfo.eUpdateMode[SPDIF_REG_TYPE_USER][0]),
               (VOID *)(&_aSpdifInfo.eUpdateMode[SPDIF_REG_TYPE_DEFAULT][0]),
               sizeof(UINT8) * AUD_STREAM_FROM_NUM);


        VERIFY(x_sema_create(&_ahSpdifCtlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
        fgInit = TRUE;
    }
}

/*Aproc Bmang*/
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
void _AUD_AprocBMangInit(void)
{
    static BOOL fgInit = FALSE;
    UINT32 u4Ptr;    

    UNUSED(u4Ptr);

    if (!fgInit)
    {
#if 0///  note !!!!! fix me sunne       
        VERIFY(DRVCUST_OptQuery(eAudioBmangSetting, (UINT32 *)&u4Ptr)==0);
        x_memcpy((VOID *)&_aAprocDspBmangSetInfo, (const VOID *)u4Ptr, sizeof(_aAprocDspBmangSetInfo));

        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_BMANG, 0, (UINT32 *) &_aAprocDspBmangSetInfo, 1);
#endif
        fgInit = TRUE;
   }
    
}
#endif
#ifdef CC_MT5881
void _AUD_SpdifinInit(void)
{
    // 56d8 configure
    AUD_SET_BIT(REG_SPDIFIN_EDGE_INTERRUPT, SPDIF_RX_EDGE_DET_EN); // Enable optical input edge detection
    // 56dc configure
    AUD_SET_BIT(REG_SPDIFIN_ROUGH_FREQ, SPDIF_RX_MODE_432M_EN);
    // 5684 configure
    AUD_SET_BIT(REG_SPDIFIN_CFG1, SPDIF_RX_TIMEOUT_INT); // Enable time out interrupt
    AUD_SET_BIT(REG_SPDIFIN_CFG1, SPDIF_RX_PREAMBLE_ERROR); // Enable preamble error interrupt
    AUD_SET_BIT(REG_SPDIFIN_CFG1, SPDIF_RX_INVALID_BITCNT);
    AUD_SET_BIT(REG_SPDIFIN_CFG1, SPDIF_RX_PARITY_CHKERR);
    // 56c0 configure
    AUD_WRITE32(REG_SPDIFIN_BR, (AUD_READ32(REG_SPDIFIN_BR) & (~0x1f000)) | SPDIF_RX_BC_LOWBOUND);
    
    //vIO32WriteFldAlign(CKGEN_REG_SPDIFIN_CKCFG, 1, FLD_SPDIFIN_CK_SEL);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspAsrcInit
 *  Initialize ASRC configuration
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAsrcInit(void)
{
    static BOOL fgInit = FALSE;

    if (!fgInit)
    {
        VERIFY(x_sema_create(&_hAsrcControlSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
        fgInit = TRUE;
    }
}


// *********************************************************************
// Function : void AUD_DspReverbEnable(UINT8 u1DecId, BOOL fgEnable)
// Description : turn on / off reverb control
// Parameter :  TRUE / FALSE
// Return    :  None.
// *********************************************************************
void _AUD_DspReverbEnable(UINT8 u1DecId, BOOL fgEnable)
{
// Remove for new post-processing
    UINT16 u2ReverbFlagIdx;
    UINT32 u4UopCmd;

    if (u1DecId == AUD_DEC_MAIN)
    {
        u2ReverbFlagIdx = B_REVERBFLAG;
        u4UopCmd = UOP_DSP_REVERB_FLAG;
    }
    else
    {
        u2ReverbFlagIdx = B_REVERBFLAG_DEC2;
        u4UopCmd = UOP_DSP_REVERB_FLAG_DEC2;
    }

    vWriteShmUINT8(AUD_DSP0, u2ReverbFlagIdx, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
}

//-----------------------------------------------------------------------------
/** AUD_DspReverbMode
 *  load the predefined reverberation parameters sets.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  bMode From mode 0 to mode 7. Mode 0 will turn the reverberation effect off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspReverbMode(UINT8 u1DecId, UINT8 u1Mode)
{
    if (u1Mode == AUD_REVERB_MODE_OFF)
    {
        AUD_DspReverbEnable(u1DecId, FALSE);
    }
    else if (u1Mode <= AUD_REVERB_MODE_MAX)
    {
        u1Mode = u1Mode - 1;

        if (u1DecId == AUD_DEC_MAIN)
        {
            vWriteShmUINT32(AUD_DSP0, D_REVERBGAIN,  _arReverMode[u1Mode].u4Gain);
            vWriteShmUINT8(AUD_DSP0, B_REVERBBANK0, _arReverMode[u1Mode].u1ReverbBank0);
            vWriteShmUINT8(AUD_DSP0, B_REVERBBANK1, _arReverMode[u1Mode].u1ReverbBank1);
            vWriteShmUINT8(AUD_DSP0, B_REVERBBANK2, _arReverMode[u1Mode].u1ReverbBank2);
            vWriteShmUINT8(AUD_DSP0, B_REVERBBANK3, _arReverMode[u1Mode].u1ReverbBank3);
        }
        else
        {
            vWriteShmUINT32(AUD_DSP0, D_REVERBGAIN_DEC2,  _arReverMode[u1Mode].u4Gain);
            vWriteShmUINT8(AUD_DSP0, B_REVERBBANK0_DEC2, _arReverMode[u1Mode].u1ReverbBank0);
            vWriteShmUINT8(AUD_DSP0, B_REVERBBANK1_DEC2, _arReverMode[u1Mode].u1ReverbBank1);
            vWriteShmUINT8(AUD_DSP0, B_REVERBBANK2_DEC2, _arReverMode[u1Mode].u1ReverbBank2);
            vWriteShmUINT8(AUD_DSP0, B_REVERBBANK3_DEC2, _arReverMode[u1Mode].u1ReverbBank3);
        }

        AUD_DspReverbEnable(u1DecId, TRUE);
    }
}
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
void _AUD_DspSetAprocEqEnable(APROC_IOCTRL_T ePort, BOOL isFir, BOOL fgEnable)
{
    switch(ePort)
    {
        case APROC_IOCTRL_SP:
            if (isFir)
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_EQ_FIR_SP_FLAG), fgEnable);
            else
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_BIQ_SP_EQ_FLAG), fgEnable);

            _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);            
            break;
        case APROC_IOCTRL_LSRS:
            if (isFir)
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_EQ_FIR_SP_LSRS_FLAG), fgEnable);
            else
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_BIQ_SP_LSRS_EQ_FLAG), fgEnable);

            _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_LSRS_PATH);           
            break;
        case APROC_IOCTRL_HP:
            if (isFir)
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_EQ_FIR_HP_FLAG), fgEnable);
            else
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_BIQ_HP_EQ_FLAG), fgEnable);

            _vAprocSetRoutine (APROC_ROUTINE_ID_ER_HP_PATH); 
            break;
        default:
            break;
    }
    
}

BOOL _AUD_DspGetAprocEqEnable(APROC_IOCTRL_T ePort, BOOL isFir)
{
    UINT8   fgEnable = 0;
    
    switch(ePort)
    {
        case APROC_IOCTRL_SP:
            if (isFir)
                fgEnable = u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_EQ_FIR_SP_FLAG));
            else
                fgEnable = u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_BIQ_SP_EQ_FLAG));
            break;
        case APROC_IOCTRL_LSRS:
            if (isFir)
                fgEnable = u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_EQ_FIR_SP_LSRS_FLAG));
            else
                fgEnable = u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_BIQ_SP_LSRS_EQ_FLAG));
            break;
        case APROC_IOCTRL_HP:
            if (isFir)
                fgEnable = u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_EQ_FIR_HP_FLAG));
            else
                fgEnable = u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_12, APROC_REG_BIQ_HP_EQ_FLAG));
            break;
        default:
            break;
    }
    return (BOOL)fgEnable;
}

#endif

// *********************************************************************
// Function :  void AUD_DspBassBoostEnable(UINT8 u1DecId, BOOL fgEnable)
// Description : turn on /off the bass boost control
// Parameter : bMode - ADSP_ON / ADSP_OFF
// Return    : None.
// *********************************************************************
void _AUD_DspChannelEqEnable(UINT8 u1DecId, BOOL fgEnable)
{
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, APROC_REG_BIQ_SP_EQ_FLAG), fgEnable);

#else
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    EQ_FLAG_UNION_T rEqFlag;
  
    AUD_DspChEqLoadPreset(u1DecId, _eEqMode);

    if (u1DecId == AUD_DEC_MAIN)
    {
        u2EqFlagIdx = B_EQFLAG;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    }
    else
    {
        u2EqFlagIdx = B_EQFLAG_DEC2;
        u4UopCmd = UOP_DSP_EQUALIZER_FLAG_DEC2;
    }

    rEqFlag.u1Eq = uReadShmUINT8(AUD_DSP0, u2EqFlagIdx);
    rEqFlag.rField.fgChEqOn = fgEnable;

    vWriteShmUINT8(AUD_DSP0, u2EqFlagIdx, rEqFlag.u1Eq);
    DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
#endif
}

BOOL _AUD_DspGetChannelEqEnable(UINT8 u1DecId)
{
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
    UINT8   fgEnable;
    
    fgEnable = u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, APROC_REG_BIQ_SP_EQ_FLAG));
    
    return (BOOL)fgEnable;

#else
    UINT16 u2EqFlagIdx;
    if (u1DecId == AUD_DEC_MAIN)
    {
        u2EqFlagIdx = B_EQFLAG;
    }
    else
    {
        u2EqFlagIdx = B_EQFLAG_DEC2;
    }

    if (uReadShmUINT8(AUD_DSP0, u2EqFlagIdx)&EQ_FLAG)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
}

// *********************************************************************
// Function :  void AUD_DspSpkSizeCfg(UINT8 u1DecId, UINT32 u4SpkSize)
// Description : Set the bass management cut-off frequency
// Parameter : u2SpkSize in xx Hz
// Return    : None.
// *********************************************************************
void _AUD_DspSpkSizeCfg(UINT8 u1DecId, UINT32 u4SpkSize)
{
    UINT16 u2ShmIdx;
    UINT32 u4UopCmd;
    UINT32 u4CutFreq;

    if (u1DecId == AUD_DEC_MAIN)
    {
        u2ShmIdx = D_CUTOFF_FREQ;
        u4UopCmd = UOP_DSP_CHANGE_SPK_SIZE;
    }
    else
    {
        u2ShmIdx = D_CUTOFF_FREQ_DEC2;
        u4UopCmd = UOP_DSP_CHANGE_SPK_SIZE;   // since only do aout re-init, therefore re-use uop of dec1.
    }

    // Check if duplicated setting
    u4CutFreq = u4ReadShmUINT32(AUD_DSP0, u2ShmIdx);
    if (u4CutFreq == u4SpkSize)
    {
        return;
    }

    vWriteShmUINT32(AUD_DSP0, u2ShmIdx, u4SpkSize);
    DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
}


// *********************************************************************
// Function :  UINT32 _AUD_DspSpkSizeQry(UINT8 u1DecId)
// Description : Query the bass management cut-off frequency
// Parameter : Decoder ID
// Return    : None.
// *********************************************************************
UINT32 _AUD_DspSpkSizeQry(UINT8 u1DecId) //Andrew Wen 07/8/20
{
    UINT32 iCutFreq;

    iCutFreq = u4ReadShmUINT32 (AUD_DSP0, D_CUTOFF_FREQ);
    return iCutFreq;
}

#ifdef CC_S_SUBWOFFER_SUPPORT  ///CC_BASS_MANGER_INDEP_LPF
// *********************************************************************
// Function :  void _AUD_DspSpkSizeLpfCfg(UINT8 u1DecId, UINT32 u4SpkSize)
// Description : Set the bass management cut-off frequency
// Parameter : u2SpkSize in xx Hz
// Return    : None.
// *********************************************************************
void _AUD_DspSpkSizeLpfCfg(UINT8 u1DecId, UINT32 u4SpkSize)
{
    UINT16 u2ShmIdx;
    UINT32 u4UopCmd;
    UINT32 u4CutFreq;

    if (u1DecId == AUD_DEC_MAIN)
    {
        u2ShmIdx = D_CUTOFF_FREQ_LPF;
        u4UopCmd = UOP_DSP_CHANGE_SPK_SIZE;
    }
    else
    {
        u2ShmIdx = D_CUTOFF_FREQ_LPF_DEC2;
        u4UopCmd = UOP_DSP_CHANGE_SPK_SIZE;   // since only do aout re-init, therefore re-use uop of dec1.
    }

    // Check if duplicated setting
    u4CutFreq = u4ReadShmUINT32(u2ShmIdx);
    if (u4CutFreq == u4SpkSize)
    {
        return;
    }

    vWriteShmUINT32(u2ShmIdx, u4SpkSize);
    DSP_SendDspTaskCmd(u4UopCmd);
}


// *********************************************************************
// Function :  UINT32 _AUD_DspSpkSizeLpfQry(UINT8 u1DecId)
// Description : Query the bass management cut-off frequency
// Parameter : Decoder ID
// Return    : None.
// *********************************************************************
UINT32 _AUD_DspSpkSizeLpfQry(UINT8 u1DecId) //Andrew Wen 07/8/20
{
    UINT32 iCutFreq;

    if (u1DecId == AUD_DEC_MAIN)
    {
        iCutFreq = u4ReadShmUINT32 (D_CUTOFF_FREQ_LPF);
    }
    else
    {
        iCutFreq = u4ReadShmUINT32 (D_CUTOFF_FREQ_LPF_DEC2);
    }
    return iCutFreq;
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspBassBoostEnable
 *  turn on /off the bass boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspBassBoostEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    //EQ_FLAG_UNION_T rEqFlag;
    UINT8 u1ShmFlag;
#ifdef CC_AUD_ARM_RENDER
    UINT32 u4Flag;    
#endif

    UNUSED(u1DecId);

    LOG(5,"CMD: set BassEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);
#ifdef CC_AUD_ARM_RENDER
    UNUSED(u2EqFlagIdx);
    UNUSED(u4UopCmd);
    UNUSED(u1ShmFlag);    

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP, &u4Flag, 1);
    
    if (fgEnable != (BOOL)u4Flag)
    {
    
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP, (UINT32*)&fgEnable, 1);
    }
#else

    u2EqFlagIdx = B_EQFLAG;
    u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    u1ShmFlag = uReadShmUINT8(AUD_DSP0, B_EQFLAG);

    if ((fgEnable && !(u1ShmFlag& EQ_BASS_BOOST)) ||
        (!fgEnable && ((u1ShmFlag & EQ_BASS_BOOST) == EQ_BASS_BOOST)))
    {
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_BASS_BOOST) : (u1ShmFlag & ~ EQ_BASS_BOOST);
        vWriteShmUINT8(AUD_DSP0, u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
    }
#endif
}

BOOL _AUD_DspGetBassBoostEnable(UINT8 u1DecId)
{
    UINT16 u2EqFlagIdx;

    UNUSED(u1DecId);

    u2EqFlagIdx = B_EQFLAG;

    if (uReadShmUINT8(AUD_DSP0, u2EqFlagIdx) & EQ_BASS_BOOST)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
// *********************************************************************
// Function :  void _AUD_DspBassTrebleCutOffFreq(UINT8 u1DecId,UINT8 u1LowFc, UINT8 u1HighFc)
// Description : Set the bass/treble  cut-off frequency
// Parameter : u1LowFc in 0, 1, 2(100, 150, 200), u1HighFc in 0, 1, 2 (5K, 6K, 7K)
// Return    : None.
// *********************************************************************
void _AUD_DspBassTrebleCutOffFreq(UINT8 u1DecId,UINT8 u1LowFc, UINT8 u1HighFc)
{
    UINT32 u4UopCmd;
    UINT32 u1LowCutFreq, u1HighCutFreq;
    UNUSED(u1DecId);
    
    LOG(5,"CMD: set Bass/Treble Cut-off Freq: Dec(%d) Bass(%d) Treble(%d)\n", u1DecId, u1LowFc, u1HighFc);
    
    // Check if duplicated setting
    u1LowCutFreq = uReadShmUINT8(AUD_DSP0, B_SBASSBOOSTCUTOFF);    
    u1HighCutFreq = uReadShmUINT8(AUD_DSP0, B_SBASSCLEARCUTOFF);
    
    if ((u1LowCutFreq == u1LowFc)&&(u1HighCutFreq == u1HighFc))
    {
        return;
    }
    
    vWriteShmUINT8(AUD_DSP0, B_SBASSBOOSTCUTOFF, u1LowFc);
    vWriteShmUINT8(AUD_DSP0, B_SBASSCLEARCUTOFF, u1HighFc);

    u4UopCmd = UOP_DSP_SUPER_BASS_BOOST_GAIN;
    
    DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
}
// *********************************************************************
// Function :  UINT32 _AUD_DspBassCutOffFreqQry(UINT8 u1DecId)
// Description : Query the bass cut-off frequency
// Parameter : Decoder ID
// Return    : None.
// *********************************************************************
UINT32 _AUD_DspBassCutOffFreqQry(UINT8 u1DecId)
{
    UINT32 iCutFreq;
    UNUSED(u1DecId);
   
    iCutFreq = uReadShmUINT8 (AUD_DSP0, B_SBASSBOOSTCUTOFF);

    return iCutFreq;
}
// *********************************************************************
// Function :  UINT32 _AUD_DspBassCutOffFreqQry(UINT8 u1DecId)
// Description : Query the bass cut-off frequency
// Parameter : Decoder ID
// Return    : None.
// *********************************************************************
UINT32 _AUD_DspTrebleCutOffFreqQry(UINT8 u1DecId)
{
    UINT32 iCutFreq;
    UNUSED(u1DecId);
   
    iCutFreq = uReadShmUINT8 (AUD_DSP0, B_SBASSCLEARCUTOFF);

    return iCutFreq;
}
//-----------------------------------------------------------------------------
/** AUD_DspBassBoostGain
 *   set the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1Level the setting value is from 0 to 28, level 14 means 0db. A large
 *                         number will boost the bass and a smaller number will attenuate the bass.
 *  @retval void
 */
//-----------------------------------------------------------------------------
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
void _AUD_DspBassBoostGain(UINT8 u1DecId, UINT8 u1Level)
{
    APROC_DSP_BIQ_INFO_T  aBiqSet; 

    LOG(5,"Aproc Bass: Level(%d)\n", u1Level);
        
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_SP, (UINT32 *) &aBiqSet, 4);

    aBiqSet.i4Gain = (INT32)ADSP_BIQ_DB((INT32)(u1Level - BASS_TREBLE_GAIN_DB_MAX)) ; // uiLevel = BASS_TREBLE_GAIN_DB_MAX -> ODB

    if ( aBiqSet.i4Gain > 720 )
    {
        aBiqSet.i4Gain = 720;   //max 18db
    }
    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_SP, (UINT32*)&aBiqSet, 4);

}
#else
void _AUD_DspBassBoostGain(UINT8 u1DecId, UINT8 u1Level)
{
#ifndef CC_AUD_EQ_BASED_SBASS
    // Use original sbass
    UINT16 u2BassBoostGainIdx;
    UINT32 u4UopCmd;
    static BOOL fgEnable = FALSE;
    INT8 i1Level;

    UNUSED(u1DecId);
    UNUSED(i1Level);    

    LOG(5,"CMD: set BassGain: Dec(%d) Level(%d)\n", u1DecId, u1Level);

    if (!fgEnable)
    {
        AUD_DspBassBoostEnable(AUD_DEC_MAIN, TRUE);
        fgEnable = TRUE;
    }

    u2BassBoostGainIdx = D_SBASSBOOSTGAIN;
    u4UopCmd = UOP_DSP_SUPER_BASS_BOOST_GAIN;
#ifdef CC_AUD_USE_FLASH_AQ   
    i1Level = _aai1BassTreble[0] + (INT8)u1Level;
    
    if (i1Level < 0)
    {
        u1Level = 0;
    }
    else
    {
        u1Level= (UINT8)i1Level;
    }
#endif

    if (u1Level >= BASS_TREBLE_GAIN_MAX)
    {
        u1Level = BASS_TREBLE_GAIN_MAX - 1;
    }

    vWriteShmUINT32(AUD_DSP0, u2BassBoostGainIdx, u1Level);
    DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
    AUD_DspBassBoostEnable(AUD_DEC_MAIN, TRUE);
    x_thread_delay(10);

    #ifdef CC_AUD_BASS_ADJ_LFE    //andrew wen 07/1/25
    u1Level = u1Level<<2;
    if (u1Level > 100)
    {
        u1Level = 100;
    }
    _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_LFE, _au4VOLUME[u1Level]);
    #endif
#else
    // Use EQ-based sbass
    INT8 i1SbassDb, i1SbassDbStart, i1SbassStep, i1TotalDb;
    INT8 i1LoopCnt, i1LoopIdx;
    UINT8     u1Idx, u1EqType;
    static INT8 _i1SbassDbOld=EQ_BAND_GAIN_MID;

    UNUSED(u1DecId);

    LOG(5,"CMD: set BassGain: Dec(%d) Level(%d)\n", u1DecId, u1Level);

    u1EqType = _eEqMode;
    i1SbassDb = (((INT8)u1Level * 2) * EQ_BAND_GAIN_DB_MAX) / BASS_TREBLE_GAIN_DB_MAX;

    if (i1SbassDb == _i1SbassDbOld)
    {
        return;
    }
    else if (i1SbassDb == EQ_BAND_GAIN_MID)
    {   /* add for "reset default" option in menu */
        i1SbassDbStart = EQ_BAND_GAIN_MID;
        i1SbassStep = 0;
        i1LoopCnt = 1;
    }
    else if (i1SbassDb > _i1SbassDbOld)
    {
        i1SbassDbStart = _i1SbassDbOld + 1;
        i1SbassStep = 1;
        i1LoopCnt = i1SbassDb - _i1SbassDbOld;
    }
    else
    {
        i1SbassDbStart = _i1SbassDbOld - 1;
        i1SbassStep = -1;
        i1LoopCnt = _i1SbassDbOld - i1SbassDb;
    }

    for (i1LoopIdx=0; i1LoopIdx<i1LoopCnt; i1LoopIdx++)
    {
        _aai1ChEqCfg[AUD_EQ_SBASS][AUD_EQ_BAND_1] = i1SbassDbStart - EQ_BAND_GAIN_MID;
        i1TotalDb = _aai1ChEqCfg[u1EqType][AUD_EQ_BAND_1] + _aai1ChEqCfg[AUD_EQ_SBASS][AUD_EQ_BAND_1];

        if (i1TotalDb > EQ_BAND_GAIN_SIDE_INDEX_MAX)
        {
            i1TotalDb = EQ_BAND_GAIN_SIDE_INDEX_MAX;
        }

        if (i1TotalDb < -EQ_BAND_GAIN_SIDE_INDEX_MAX)
        {
            i1TotalDb = -EQ_BAND_GAIN_SIDE_INDEX_MAX;
        }

        _ai1ChEqFinalCfg[AUD_EQ_BAND_1] = i1TotalDb;

        VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        AUD_DspChEqBand(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[EQ_BAND_DRY]+EQ_DRY_GAIN_MID), FALSE);
        for (u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
        {
            AUD_DspChEqBand(AUD_DEC_MAIN, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
        }
        VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);

        i1SbassDbStart += i1SbassStep;
    }

    AUD_DspEqEnable(AUD_DEC_MAIN, TRUE);
    _i1SbassDbOld = i1SbassDb;
#endif
}
#endif
//-----------------------------------------------------------------------------
/** _AUD_DspGetBassBoostGain
 *   get the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @retval bass gain value
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetBassBoostGain(UINT8 u1DecId)
{
    UINT32 u4Level;

    UNUSED(u1DecId);

    u4Level = u4ReadShmUINT32(AUD_DSP0, D_SBASSBOOSTGAIN);
    return u4Level;
}

//-----------------------------------------------------------------------------
/** AUD_DspTrebleBoostCtrl
 *   turn on / off treble boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable  ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSuperBassEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    UINT8  u1ShmFlag;

    UNUSED(u1DecId);

    LOG(5,"CMD: set SBassEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

    u2EqFlagIdx = B_EQFLAG;
    u4UopCmd = UOP_DSP_EQUALIZER_FLAG;

    u1ShmFlag = uReadShmUINT8(AUD_DSP0, u2EqFlagIdx);
    if ((fgEnable && !(u1ShmFlag& EQ_SUPERBASS_BOOST)) ||
        (!fgEnable && ((u1ShmFlag & EQ_SUPERBASS_BOOST) == EQ_SUPERBASS_BOOST)))
    {
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_SUPERBASS_BOOST) : (u1ShmFlag & ~ EQ_SUPERBASS_BOOST);
        vWriteShmUINT8(AUD_DSP0, u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
    }
}

// *********************************************************************
// Function : void AUD_DspTrebleBoostCtrl(BYTE bMode)
// Description : turn on / off treble boost control
// Parameter : ADSP_ON / ADSP_OFF
// Return    : None.
// *********************************************************************
void _AUD_DspClearBoostEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    //EQ_FLAG_UNION_T rEqFlag;
    UINT8   u1ShmFlag;
#ifdef CC_AUD_ARM_RENDER
    UINT32 u4Flag;    
#endif

    UNUSED(u1DecId);

    LOG(5,"CMD: set TrebleEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

#ifdef CC_AUD_ARM_RENDER
    UNUSED(u2EqFlagIdx);
    UNUSED(u4UopCmd);
    UNUSED(u1ShmFlag);    

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP, &u4Flag, 1);
    
    if (fgEnable != (BOOL)u4Flag)
    {
    
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP, (UINT32*)&fgEnable, 1);
    }
#else

    u2EqFlagIdx = B_EQFLAG;
    u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    u1ShmFlag = uReadShmUINT8(AUD_DSP0, u2EqFlagIdx);

    if ((fgEnable && !(u1ShmFlag& EQ_TREBLE_BOOST)) ||
        (!fgEnable && ((u1ShmFlag & EQ_TREBLE_BOOST) == EQ_TREBLE_BOOST)))
    {
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_TREBLE_BOOST) : (u1ShmFlag & ~ EQ_TREBLE_BOOST);
        vWriteShmUINT8(AUD_DSP0, u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
    }
#endif
}

BOOL _AUD_DspGetClearBoostEnable(UINT8 u1DecId)
{
    UINT16 u2EqFlagIdx;

    UNUSED(u1DecId);

    u2EqFlagIdx = B_EQFLAG;

    if (uReadShmUINT8(AUD_DSP0, u2EqFlagIdx) & EQ_TREBLE_BOOST)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspClearBoostGain
 *   set the clear boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1Level  range from 0 to 28, level 14 means 0db.
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifdef CC_AUD_ARM_RENDER
void _AUD_DspClearBoostGain(UINT8 u1DecId, UINT8 u1Level)
{
    APROC_DSP_BIQ_INFO_T  aBiqSet; 
        
    LOG(5,"Aproc Treble: Level(%d)\n", u1Level);
    
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_TREBLE_SP, (UINT32 *) &aBiqSet, 4);

    aBiqSet.i4Gain = (INT32)ADSP_BIQ_DB((INT32)(u1Level - BASS_TREBLE_GAIN_DB_MAX)) ; // uiLevel = BASS_TREBLE_GAIN_DB_MAX -> ODB

    if ( aBiqSet.i4Gain > 720 )
    {
        aBiqSet.i4Gain = 720;   //max 18db
    }
    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_TREBLE_SP, (UINT32*)&aBiqSet, 4);

}
#else
void _AUD_DspClearBoostGain(UINT8 u1DecId, UINT8 u1Level)
{
#ifndef CC_AUD_EQ_BASED_SBASS
    // Use original sbass
    UINT16 u2ClearBoostGainIdx;
    UINT32 u4UopCmd;
    static BOOL fgEnable = FALSE;
    INT8 i1Level;

    UNUSED(u1DecId);
    UNUSED(i1Level);

    LOG(5,"CMD: set TrebleGain: Dec(%d) Level(%d)\n", u1DecId, u1Level);

    if (!fgEnable)
    {
        AUD_DspClearBoostEnable(AUD_DEC_MAIN, TRUE);
        fgEnable = TRUE;
    }

    u2ClearBoostGainIdx = D_SBASSCLEARGAIN;
    u4UopCmd = UOP_DSP_SUPER_BASS_CLEAR_GAIN;
#ifdef CC_AUD_USE_FLASH_AQ   
    i1Level = _aai1BassTreble[1] + (INT8)u1Level;
    
    if (i1Level < 0)
    {
        u1Level = 0;
    }
    else
    {
        u1Level= (UINT8)i1Level;
    }
#endif

    if (u1Level >= BASS_TREBLE_GAIN_MAX)
    {
        u1Level = BASS_TREBLE_GAIN_MAX - 1;
    }

    vWriteShmUINT32(AUD_DSP0, u2ClearBoostGainIdx, u1Level);
    DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
    AUD_DspClearBoostEnable(AUD_DEC_MAIN, TRUE);
    x_thread_delay(10);

#else
    // Use EQ-based sbass
    INT8 i1SbassDb, i1SbassDbStart, i1SbassStep, i1TotalDb;
    INT8 i1LoopCnt, i1LoopIdx;
    UINT8     u1Idx, u1EqType;
    static INT8 _i1SbassDbOld=EQ_BAND_GAIN_MID;

    UNUSED(u1DecId);

    LOG(5,"CMD: set TrebleGain: Dec(%d) Level(%d)\n", u1DecId, u1Level);

    u1EqType = _eEqMode;
    i1SbassDb = (((INT8)u1Level * 2) * EQ_BAND_GAIN_DB_MAX) / BASS_TREBLE_GAIN_DB_MAX;

    if (i1SbassDb == _i1SbassDbOld)
    {
        return;
    }
    else if (i1SbassDb == EQ_BAND_GAIN_MID)
    {   /* add for "reset default" option in menu */
        i1SbassDbStart = EQ_BAND_GAIN_MID;
        i1SbassStep = 0;
        i1LoopCnt = 1;
    }
    else if (i1SbassDb > _i1SbassDbOld)
    {
        i1SbassDbStart = _i1SbassDbOld + 1;
        i1SbassStep = 1;
        i1LoopCnt = i1SbassDb - _i1SbassDbOld;
    }
    else
    {
        i1SbassDbStart = _i1SbassDbOld - 1;
        i1SbassStep = -1;
        i1LoopCnt = _i1SbassDbOld - i1SbassDb;
    }

    for (i1LoopIdx=0; i1LoopIdx<i1LoopCnt; i1LoopIdx++)
    {
        _aai1ChEqCfg[AUD_EQ_SBASS][AUD_EQ_BAND_LAST] = i1SbassDbStart - EQ_BAND_GAIN_MID;
        i1TotalDb = _aai1ChEqCfg[u1EqType][AUD_EQ_BAND_LAST] + _aai1ChEqCfg[AUD_EQ_SBASS][AUD_EQ_BAND_LAST];

        if (i1TotalDb > EQ_BAND_GAIN_SIDE_INDEX_MAX)
        {
            i1TotalDb = EQ_BAND_GAIN_SIDE_INDEX_MAX;
        }

        if (i1TotalDb < -EQ_BAND_GAIN_SIDE_INDEX_MAX)
        {
            i1TotalDb = -EQ_BAND_GAIN_SIDE_INDEX_MAX;
        }

        _ai1ChEqFinalCfg[AUD_EQ_BAND_LAST] = i1TotalDb;

        VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        AUD_DspChEqBand(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[EQ_BAND_DRY]+EQ_DRY_GAIN_MID), FALSE);
        for (u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
        {
            AUD_DspChEqBand(AUD_DEC_MAIN, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
        }
        VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);

        i1SbassDbStart += i1SbassStep;
    }

    AUD_DspEqEnable(AUD_DEC_MAIN, TRUE);
    _i1SbassDbOld = i1SbassDb;
#endif
}
#endif
//-----------------------------------------------------------------------------
/** _AUD_DspGetClearBoostGain
 *   get the clear boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @retval treble gain value
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetClearBoostGain(UINT8 u1DecId)
{
    UINT32 u4Level;

    UNUSED(u1DecId);
    u4Level = u4ReadShmUINT32(AUD_DSP0, D_SBASSCLEARGAIN);
    return u4Level;
}

#ifdef CC_AUD_FY12_LOUDNESS
//-----------------------------------------------------------------------------
/** AUD_DspBassBoostEnable
 *  turn on /off the bass boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspBassBoostEnable2(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    //EQ_FLAG_UNION_T rEqFlag;
    UINT8   u1ShmFlag;

    UNUSED(u1DecId);

    LOG(5,"CMD: set BassAttnEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

    u2EqFlagIdx = B_EQFLAG2;
    u4UopCmd = UOP_DSP_FY12_LOUDNESS_FLAG;
    u1ShmFlag = uReadShmUINT8(B_EQFLAG2);

    if ((fgEnable && !(u1ShmFlag& EQ_BASS_BOOST)) ||
        (!fgEnable && ((u1ShmFlag & EQ_BASS_BOOST) == EQ_BASS_BOOST)))
    {
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_BASS_BOOST) : (u1ShmFlag & ~ EQ_BASS_BOOST);
        vWriteShmUINT8(u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(u4UopCmd);
    }
}

BOOL _AUD_DspGetBassBoostEnable2(UINT8 u1DecId)
{
    UINT16 u2EqFlagIdx;

    UNUSED(u1DecId);

    u2EqFlagIdx = B_EQFLAG2;

    if (uReadShmUINT8(u2EqFlagIdx) & EQ_BASS_BOOST)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspBassBoostGain
 *   set the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1Level the setting value is from 0 to 28, level 14 means 0db. A large
 *                         number will boost the bass and a smaller number will attenuate the bass.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspBassBoostGain2(UINT8 u1DecId, UINT8 u1Level)
{

    // Use original sbass
    UINT16 u2BassBoostGainIdx;
    UINT32 u4UopCmd;
    static BOOL fgEnable = FALSE;
    INT8 i1Level;

    UNUSED(u1DecId);
    UNUSED(i1Level);    
#ifdef CC_AUD_LOUDNESS_FROM_SOURCE_SOUNDMODE
    UINT8 eConvertSrc;
    UINT8 eConvertSoundMode;
    UINT8 u1Volume,u1TableIdx,u1TableIdx_temp;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_DRVGetStreamFrom(AUD_DEC_MAIN, &eStreamFrom);
    //The StreamFrom in AQ Table is not in the same order as our emu type.          
    eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];    

    if (_au1MWSetSoundModeIdx < sizeof(_au1AQSmod_Convert))
    {
        eConvertSoundMode = _au1AQSmod_Convert[_au1MWSetSoundModeIdx];
    }
    else
    {
        eConvertSoundMode = 0;
    }
            
    //Note that vol 45~49 maps to tbl[0], volume 50~54 maps to tbl[1], vol 55~59 maps to tbl[2], ..., 90~100 maps to tbl[9]
    u1Volume = _AUD_DspGetMasterVolume(AUD_DEC_MAIN);
    u1TableIdx = (u1Volume-1)/10;
    u1TableIdx_temp = (u1Volume-1)%10;
    if( u1TableIdx < 5) 
    {
        u1TableIdx = 0;
    }
    else if( u1TableIdx >= 9 ) 
    {
        u1TableIdx = 9;
    }
    else if( u1TableIdx_temp < 5) 
    {
        u1TableIdx = (u1TableIdx-4)*2-1;
    }
    else 
    {
        u1TableIdx = (u1TableIdx-4)*2;
    } 
    if( u1Volume > 45 )
    {
        _ai1BassAttnTable[eConvertSrc][eConvertSoundMode][u1TableIdx] = u1Level;
    }   
    else
    {
        u1Level = BASS_TREBLE_ATTN_GAIN_DB_MAX;
    }
    LOG(7, "==> AUD_DspBassBoostGain2 :Bass = 0x%x; vol(%d),source(%d),sm(%d)\n",_ai1BassAttnTable[eConvertSrc][eConvertSoundMode][u1TableIdx],u1Volume,eConvertSrc,eConvertSoundMode);
#endif

    LOG(5,"CMD: set BassAttnGain: Dec(%d) Level(%d)\n", u1DecId, u1Level);

    if (!fgEnable)
    {
        AUD_DspBassBoostEnable2(AUD_DEC_MAIN, TRUE);
        fgEnable = TRUE;
    }

    u2BassBoostGainIdx = D_SBASSBOOSTGAIN2;
    u4UopCmd = UOP_DSP_FY12_LOUDNESS_BASS_ATTN_GAIN;
#ifdef CC_AUD_USE_FLASH_AQ   
    //i1Level = _aai1BassTreble[0] + (INT8)u1Level;
    i1Level = (INT8)u1Level;
    
    if (i1Level < BASS_TREBLE_ATTN_GAIN_DB_MIN)
    {
        u1Level = BASS_TREBLE_ATTN_GAIN_DB_MIN;
    }
    else
    {
        u1Level= (UINT8)i1Level;
    }
#endif

    if (u1Level >= BASS_TREBLE_ATTN_GAIN_DB_MAX)
    {
        u1Level = BASS_TREBLE_ATTN_GAIN_DB_MAX;
    }

    vWriteShmUINT32(u2BassBoostGainIdx, u1Level);
    DSP_SendDspTaskCmd(u4UopCmd);
    AUD_DspBassBoostEnable2(AUD_DEC_MAIN, TRUE);
    x_thread_delay(10);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetBassBoostGain
 *   get the bass boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @retval bass gain value
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetBassBoostGain2(UINT8 u1DecId)
{
    UINT32 u4Level;

    UNUSED(u1DecId);

    u4Level = u4ReadShmUINT32(D_SBASSBOOSTGAIN2);
    return u4Level;
}

//-----------------------------------------------------------------------------
/** AUD_DspTrebleBoostCtrl
 *   turn on / off treble boost control.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable  ADSP_ON / ADSP_OFF.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSuperBassEnable2(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    UINT8  u1ShmFlag;

    UNUSED(u1DecId);

    LOG(5,"CMD: set SBassAttnEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

    u2EqFlagIdx = B_EQFLAG2;
    u4UopCmd = UOP_DSP_FY12_LOUDNESS_FLAG;

    u1ShmFlag = uReadShmUINT8(u2EqFlagIdx);
    if ((fgEnable && !(u1ShmFlag& EQ_SUPERBASS_BOOST)) ||
        (!fgEnable && ((u1ShmFlag & EQ_SUPERBASS_BOOST) == EQ_SUPERBASS_BOOST)))
    {
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_SUPERBASS_BOOST) : (u1ShmFlag & ~ EQ_SUPERBASS_BOOST);
        vWriteShmUINT8(u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(u4UopCmd);
    }
}

// *********************************************************************
// Function : void AUD_DspTrebleBoostCtrl(BYTE bMode)
// Description : turn on / off treble boost control
// Parameter : ADSP_ON / ADSP_OFF
// Return    : None.
// *********************************************************************
void _AUD_DspClearBoostEnable2(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    //EQ_FLAG_UNION_T rEqFlag;
    UINT8   u1ShmFlag;

    UNUSED(u1DecId);

    LOG(5,"CMD: set TrebleAttnEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

    u2EqFlagIdx = B_EQFLAG2;
    u4UopCmd = UOP_DSP_FY12_LOUDNESS_FLAG;
    u1ShmFlag = uReadShmUINT8(u2EqFlagIdx);

    if ((fgEnable && !(u1ShmFlag& EQ_TREBLE_BOOST)) ||
        (!fgEnable && ((u1ShmFlag & EQ_TREBLE_BOOST) == EQ_TREBLE_BOOST)))
    {
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_TREBLE_BOOST) : (u1ShmFlag & ~ EQ_TREBLE_BOOST);
        vWriteShmUINT8(u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(u4UopCmd);
    }
}

BOOL _AUD_DspGetClearBoostEnable2(UINT8 u1DecId)
{
    UINT16 u2EqFlagIdx;

    UNUSED(u1DecId);

    u2EqFlagIdx = B_EQFLAG2;

    if (uReadShmUINT8(u2EqFlagIdx) & EQ_TREBLE_BOOST)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspClearBoostGain
 *   set the clear boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1Level  range from 0 to 28, level 14 means 0db.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspClearBoostGain2(UINT8 u1DecId, UINT8 u1Level)
{

    // Use original sbass
    UINT16 u2ClearBoostGainIdx;
    UINT32 u4UopCmd;
    static BOOL fgEnable = FALSE;
    INT8 i1Level;

    UNUSED(u1DecId);
    UNUSED(i1Level);
#ifdef CC_AUD_LOUDNESS_FROM_SOURCE_SOUNDMODE
    UINT8 eConvertSrc;
    UINT8 eConvertSoundMode;
    UINT8 u1Volume,u1TableIdx,u1TableIdx_temp;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_DRVGetStreamFrom(AUD_DEC_MAIN, &eStreamFrom);
    //The StreamFrom in AQ Table is not in the same order as our emu type.          
    eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];    
            
    if (_au1MWSetSoundModeIdx < sizeof(_au1AQSmod_Convert))
    {
        eConvertSoundMode = _au1AQSmod_Convert[_au1MWSetSoundModeIdx];
    }
    else
    {
        eConvertSoundMode = 0;
    }
            
    //Note that vol 45~49 maps to tbl[0], volume 50~54 maps to tbl[1], vol 55~59 maps to tbl[2], ..., 90~100 maps to tbl[9]
    u1Volume = _AUD_DspGetMasterVolume(AUD_DEC_MAIN);
    u1TableIdx = (u1Volume-1)/10;
    u1TableIdx_temp = (u1Volume-1)%10;
    if( u1TableIdx < 5) 
    {
        u1TableIdx = 0;
    }
    else if( u1TableIdx >= 9 ) 
    {
        u1TableIdx = 9;
    }
    else if( u1TableIdx_temp < 5) 
    {
        u1TableIdx = (u1TableIdx-4)*2-1;
    }
    else 
    {
        u1TableIdx = (u1TableIdx-4)*2;
    } 
    if( u1Volume > 45 )
    {
        _ai1TrebleAttnTable[eConvertSrc][eConvertSoundMode][u1TableIdx] = u1Level;
    }   
    else
    {
        u1Level = BASS_TREBLE_ATTN_GAIN_DB_MAX;
    }
    LOG(7, "==> AUD_DspBassBoostGain2 :Bass = 0x%x; vol(%d),source(%d),sm(%d)\n",_ai1BassAttnTable[eConvertSrc][eConvertSoundMode][u1TableIdx],u1Volume,eConvertSrc,eConvertSoundMode);
#endif

    LOG(5,"CMD: set TrebleAttnGain: Dec(%d) Level(%d)\n", u1DecId, u1Level);

    if (!fgEnable)
    {
        AUD_DspClearBoostEnable2(AUD_DEC_MAIN, TRUE);
        fgEnable = TRUE;
    }

    u2ClearBoostGainIdx = D_SBASSCLEARGAIN2;
    u4UopCmd = UOP_DSP_FY12_LOUDNESS_TREBLE_ATTN_GAIN;
  
#ifdef CC_AUD_USE_FLASH_AQ   
    //i1Level = _aai1BassTreble[1] + (INT8)u1Level;
    i1Level =  (INT8)u1Level;

    if (i1Level < BASS_TREBLE_ATTN_GAIN_DB_MIN)
    {
        u1Level = BASS_TREBLE_ATTN_GAIN_DB_MIN;
    }
    else
    {
        u1Level= (UINT8)i1Level;
    }
#endif

    if (u1Level >= BASS_TREBLE_ATTN_GAIN_DB_MAX)
    {
        u1Level = BASS_TREBLE_ATTN_GAIN_DB_MAX;
    }

    vWriteShmUINT32(u2ClearBoostGainIdx, u1Level);
    DSP_SendDspTaskCmd(u4UopCmd);
    AUD_DspClearBoostEnable2(AUD_DEC_MAIN, TRUE);
    x_thread_delay(10);

}

//-----------------------------------------------------------------------------
/** _AUD_DspGetClearBoostGain
 *   get the clear boost gain value.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *
 *  @retval treble gain value
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetClearBoostGain2(UINT8 u1DecId)
{
    UINT32 u4Level;

    UNUSED(u1DecId);
    u4Level = u4ReadShmUINT32(D_SBASSCLEARGAIN2);
    return u4Level;
}

static void _AUD_DspSetSbass2ByMasterVolume(UINT8 u1Volume)
{

    UINT8   u1TableIdx,u1TableIdx_temp, u1NewBassAttnLvl, u1NewTrebleAttnLvl;
    BOOL    fgBassAttnEna, fgTrebleAttnEna;
    UINT8   u1CurrentBassAttnLvl, u1CurrentTrebleAttnLvl;

#ifdef CC_SONY_2K14_FLASH_AQ
    //Note that vol 0~5 maps to tbl[0], volume 6~10 maps to tbl[1], vol 11~15 maps to tbl[2], ..., 96~100 maps to tbl[19]    
    UNUSED(u1TableIdx_temp);
    if(u1Volume == 0)
    {
        u1TableIdx = 0;
    }
    else
    {
        u1TableIdx = (u1Volume - 1)/5;
    }
    if(u1TableIdx > 19)
    {
        u1TableIdx = 19;
    }
    
#else///def CC_SONY_2K13_FLASH_AQ  
    UNUSED(u1TableIdx_temp);
    u1TableIdx = (u1Volume-1)/5;
    ///u1TableIdx_temp = (u1Volume-1)%5;
    if(u1TableIdx >= 8)
    {
        u1TableIdx -= 8;
    }
    else
    {
        u1TableIdx = 0;
    }
    
    if (u1TableIdx > 11)
    {
        u1TableIdx = 11;
    }
#endif
    LOG(7, "==> LOUDNESS u1TableIdx = 0x%x", u1TableIdx);


#ifdef CC_SONY_2K14_FLASH_AQ
    UINT8 eConvertSoundMode;
    
    if (_au1MWSetSoundModeIdx < sizeof(_au1AQSmod_Convert))
    {
        eConvertSoundMode = _au1AQSmod_Convert[_au1MWSetSoundModeIdx];
    }
    else
    {
        eConvertSoundMode = 0;
    }                    
    VERIFY( eConvertSoundMode < FLASH_AQ_SM_NUM); //fix clockwork      
    if(_au1MWSetPositionIdx==1)
    {
        if(!_u1LoudnessFlag_TABLE[eConvertSoundMode])
        {
            AUD_DspBassBoostEnable2(AUD_DEC_MAIN, FALSE);
            AUD_DspClearBoostEnable2(AUD_DEC_MAIN, FALSE);
            return;
        }
      
        u1NewBassAttnLvl = _ai1BassAttnTable_TABLE[eConvertSoundMode][u1TableIdx];
        u1NewTrebleAttnLvl = _ai1TrebleAttnTable_TABLE[eConvertSoundMode][u1TableIdx];
    }
    else
    {
        if(!_u1LoudnessFlag_WALL[eConvertSoundMode])
        {
            AUD_DspBassBoostEnable2(AUD_DEC_MAIN, FALSE);
            AUD_DspClearBoostEnable2(AUD_DEC_MAIN, FALSE);
            return;
        }
          
        u1NewBassAttnLvl = _ai1BassAttnTable_WALL[eConvertSoundMode][u1TableIdx];
        u1NewTrebleAttnLvl = _ai1TrebleAttnTable_WALL[eConvertSoundMode][u1TableIdx];
    }
    #else
    u1NewBassAttnLvl = _ai1BassAttnTable[u1TableIdx];
    u1NewTrebleAttnLvl = _ai1TrebleAttnTable[u1TableIdx];
    #endif

#ifndef CC_SONY_2K14_FLASH_AQ  
    if( u1Volume <= 40 )
    {
        u1NewBassAttnLvl = BASS_TREBLE_ATTN_GAIN_DB_MAX;
        u1NewTrebleAttnLvl = BASS_TREBLE_ATTN_GAIN_DB_MAX;
    }
#endif
    fgBassAttnEna = _AUD_DspGetBassBoostEnable2(AUD_DEC_MAIN);
    fgTrebleAttnEna = _AUD_DspGetClearBoostEnable2(AUD_DEC_MAIN);
    u1CurrentBassAttnLvl = _AUD_DspGetBassBoostGain2(AUD_DEC_MAIN);
    u1CurrentTrebleAttnLvl = _AUD_DspGetClearBoostGain2(AUD_DEC_MAIN);
    
    
    //if level diff or ena_flag diff => set bass/treble attenuation
    if ((fgBassAttnEna == FALSE)||(u1NewBassAttnLvl != u1CurrentBassAttnLvl))
    {
        //LOG(7, "==> LOUDNESS :Bass = 0x%x, vol(%d),source(%d),sm(%d)\n",u1NewBassAttnLvl,u1Volume,eConvertSrc,eConvertSoundMode);
        _AUD_DspBassBoostGain2(AUD_DEC_MAIN, u1NewBassAttnLvl);
    }   
    if ((fgTrebleAttnEna == FALSE)||(u1NewTrebleAttnLvl != u1CurrentTrebleAttnLvl))
    {
        //LOG(7, "==> LOUDNESS :treble = 0x%x, vol(%d),source(%d),sm(%d)\n",u1NewTrebleAttnLvl,u1Volume,eConvertSrc,eConvertSoundMode);
        _AUD_DspClearBoostGain2(AUD_DEC_MAIN, u1NewTrebleAttnLvl);
    }   
    
}   

#endif

// *********************************************************************
// Function : void AUD_DspEqEnable(UINT8 u1DecId, BOOL fgEnable)
// Description : turn on / off equalizer control
// Parameter :  TRUE / FALSE
// Return    :  None.
// *********************************************************************
void _AUD_DspEqEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2EqFlagIdx;
    UINT32 u4UopCmd;
    UINT8   u1ShmFlag;
#ifdef CC_AUD_ARM_RENDER
    UINT32 u4Flag;

    UNUSED(u2EqFlagIdx);
    UNUSED(u4UopCmd);
    UNUSED(u1ShmFlag);    

    u4Flag = _AUD_DspGetAprocEqEnable(APROC_IOCTRL_SP, 0);
    if (u4Flag != fgEnable)
    {
        _AUD_DspSetAprocEqEnable(APROC_IOCTRL_SP, 0, fgEnable);
        _AUD_DspSetAprocEqEnable(APROC_IOCTRL_HP, 0, fgEnable);
    }
#else
    UNUSED(u1DecId);

    u2EqFlagIdx = B_EQFLAG;
    u4UopCmd = UOP_DSP_EQUALIZER_FLAG;
    u1ShmFlag = uReadShmUINT8(AUD_DSP0, u2EqFlagIdx);

    if ((fgEnable && !(u1ShmFlag& EQ_FLAG)) ||
        (!fgEnable && ((u1ShmFlag & EQ_FLAG) == EQ_FLAG)))
    {
        u1ShmFlag = fgEnable ? (u1ShmFlag | EQ_FLAG) : (u1ShmFlag & ~ EQ_FLAG);
        vWriteShmUINT8(AUD_DSP0, u2EqFlagIdx, u1ShmFlag);
        DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
    }
#endif
}

// *********************************************************************
// Function : AUD_DspChEqBand(strucDspChBand rChBand, BYTE bLevel, BYTE fgNotify)
// Description : used to adjusting the equalizer's parameter for each channel
// Parameter : u1BandIdx -- EQ band index
//             bLevel  -- range is from 0x0 to 0x28
//             fgNotify -- to indicate changing the parameter immediately or not
// Return    : None.
// *********************************************************************
void _AUD_DspChEqBand(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify)
{
    UINT8 u1Idx;
    UINT32 u4Val;
    UINT16 u2ShmIndex;
    UINT32 u4UopCmd;

    UNUSED(u1DecId);

    if (u1BandIdx >= EQ_BAND_MAX)
    {
        return;
    }

    u2ShmIndex = D_EQ_DRY;
    u4UopCmd = UOP_DSP_EQUALIZER_CHANNEL_CHANGE + 0x01000000;

    if (u1BandIdx  == EQ_BAND_DRY)
    {
        if (u1Level >= EQ_DRY_GAIN_INDEX_MAX)
        {
            u1Level = EQ_DRY_GAIN_INDEX_MAX - 1;
        }

        u4Val = _au4DryGain[u1Level];
        vWriteShmUINT32(AUD_DSP0, u2ShmIndex, u4Val);
    }
    else
    {
        if (u1Level >= EQ_BAND_GAIN_INDEX_MAX)
        {
            u1Level = EQ_BAND_GAIN_INDEX_MAX - 1;
        }

        if (u1BandIdx > CH_EQ_BAND_NO)
        {
            u1BandIdx = CH_EQ_BAND_NO;
        }

        u2ShmIndex += (4 + _aau1EqBandExtend[u1BandIdx-1][0]);

        for (u1Idx=0; u1Idx<_aau1EqBandExtend[u1BandIdx-1][1]; u1Idx++)
        {
            vWriteShmUINT8(AUD_DSP0, (u2ShmIndex+u1Idx), u1Level);
        }
    }

    if (fgNotify)
    {
        DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
    }
}
//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPreset
 *   load the predefined channel equalizer setting.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChEqLoadPreset(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType)
{
    UINT8     u1Idx;
#ifdef CC_AUD_EQ_BASED_SBASS
    INT8    i1TotalDb;
#endif

    UNUSED(u1DecId);

    if (eEqType >= AUD_EQ_NUM)
    {
        eEqType = AUD_EQ_OFF;
    }

#ifdef CC_AUD_DIFFER_EQ_PEQ_BY_CS
    if (((eEqType == AUD_EQ_CUST5) || (eEqType == AUD_EQ_CUST6)) && (BOOL)uReadShmUINT8(B_CDNOTCH_FLAG))
    { // clear sound on
        _eEqMode = eEqType - (AUD_EQ_CUST5 - AUD_EQ_CUST1);
    }
    else if (((eEqType == AUD_EQ_CUST1) || (eEqType == AUD_EQ_CUST2)) && !(BOOL)uReadShmUINT8(B_CDNOTCH_FLAG))
    { // clear sound off
        _eEqMode = eEqType + (AUD_EQ_CUST5 - AUD_EQ_CUST1);
    }
    else
#endif
    {
        _eEqMode = eEqType;
    }

#ifdef CC_AUD_EQ_BASED_SBASS
    for (u1Idx=0; u1Idx<(CH_EQ_BAND_NO + 1); u1Idx++)
    {
        i1TotalDb = _aai1ChEqCfg[eEqType][u1Idx] + _aai1ChEqCfg[AUD_EQ_SBASS][u1Idx];
        if (u1Idx == (UINT8)EQ_BAND_DRY)
        {
            // Dry gain value
            if (i1TotalDb > EQ_DRY_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = EQ_DRY_GAIN_SIDE_INDEX_MAX;
            }

            if (i1TotalDb < -EQ_DRY_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = -EQ_DRY_GAIN_SIDE_INDEX_MAX;
            }
        }
        else
        {
            // Band gain value
            if (i1TotalDb > EQ_BAND_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = EQ_BAND_GAIN_SIDE_INDEX_MAX;
            }

            if (i1TotalDb < -EQ_BAND_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = -EQ_BAND_GAIN_SIDE_INDEX_MAX;
            }
        }
        _ai1ChEqFinalCfg[u1Idx] = i1TotalDb;
    }
#endif

    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#ifndef  CC_AUD_EQ_BASED_SBASS
    AUD_DspChEqBand(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_aai1ChEqCfg[eEqType][0]+EQ_DRY_GAIN_MID), FALSE);
#else
    AUD_DspChEqBand(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[0]+EQ_DRY_GAIN_MID), FALSE);
#endif

    for (u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {
#ifndef  CC_AUD_EQ_BASED_SBASS
        AUD_DspChEqBand(AUD_DEC_MAIN, u1Idx, (UINT8)(_aai1ChEqCfg[eEqType][u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#else
        AUD_DspChEqBand(AUD_DEC_MAIN, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#endif
    }
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);

    AUD_DspEqEnable(AUD_DEC_MAIN, TRUE);
}


// *********************************************************************
// Function : AUD_DspChEqBandMode(strucDspChBand rChBand, BYTE bLevel, BYTE fgNotify)
// Description : used to adjusting the equalizer's parameter for each channel and
// Parameter : u1BandIdx -- EQ band index
//             bLevel  -- range is from 0 to 28
//             fgNotify -- to indicate changing the parameter immediately or not
// Return    : None.
// *********************************************************************
#ifdef CC_AUD_ARM_RENDER
void _AUD_DspChEqBandMode(UINT8 u1DecId, UINT8 u1BandIdx, INT8 i1Level)
{
    APROC_DSP_BIQ_INFO_T  aBiqSet; 

    LOG(1,"Aproc EQ: BandIdx(%d), Level(%d)\n", u1BandIdx, i1Level);

    // SP
    _vAUD_Aproc_Get(APROC_CONTROL_TYPE_EQ, u1BandIdx, (UINT32 *) &aBiqSet, 4);

    aBiqSet.i4Gain = (INT32)(360 + i1Level * 10) ;

    if ( aBiqSet.i4Gain > 720 )
    {
        aBiqSet.i4Gain = 720;   //max 18db
    }
    else if ( aBiqSet.i4Gain < 0 )
    {
        aBiqSet.i4Gain = 0;
    }
    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_EQ, u1BandIdx, (UINT32 *)&aBiqSet, 4);

    // HP
    _vAUD_Aproc_Get(APROC_CONTROL_TYPE_EQ, u1BandIdx, (UINT32 *) &aBiqSet, 1);

    aBiqSet.i4Gain = (INT32)(360 + i1Level * 10) ;

    if ( aBiqSet.i4Gain > 720 )
    {
        aBiqSet.i4Gain = 720;   //max 18db
    }
    else if ( aBiqSet.i4Gain < 0 )
    {
        aBiqSet.i4Gain = 0;
    }
    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_EQ, u1BandIdx, (UINT32 *)&aBiqSet, 1);
}

#else
void _AUD_DspChEqBandMode(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify)
{
    UINT8 u1Idx;
    UINT32 u4Val;
    UINT16 u2ShmIndex;
    UINT32 u4UopCmd;

    UNUSED(u1DecId);

    if (u1BandIdx >= EQ_BAND_MAX)
    {
        return;
    }

    u2ShmIndex = D_EQ_DRY;
    u4UopCmd = UOP_DSP_EQUALIZER_FLAG;

    if (u1BandIdx == EQ_BAND_DRY)
    {
        if (u1Level >= EQ_DRY_GAIN_INDEX_MAX)
        {
            u1Level = EQ_DRY_GAIN_INDEX_MAX - 1;
        }

        u4Val = _au4DryGain[u1Level];
        vWriteShmUINT32(AUD_DSP0, u2ShmIndex, u4Val);
    }
    else
    {
        if (u1Level >= EQ_BAND_GAIN_INDEX_MAX)
        {
            u1Level = EQ_BAND_GAIN_INDEX_MAX - 1;
        }

        if (u1BandIdx > CH_EQ_BAND_NO)
        {
            u1BandIdx = CH_EQ_BAND_NO;
        }

        u2ShmIndex += (4 + _aau1EqBandExtend[u1BandIdx-1][0]);

        for (u1Idx=0; u1Idx<_aau1EqBandExtend[u1BandIdx-1][1]; u1Idx++)
        {
            vWriteShmUINT8(AUD_DSP0, (u2ShmIndex+u1Idx), u1Level);
        }
    }

    if (fgNotify)
    {
        DSP_SendDspTaskCmd(AUD_DSP0, u4UopCmd);
    }
}
#endif
//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPresetChgMode
 *   load the predefined channel equalizer setting for mode changing.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChEqLoadPresetChgMode(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType)
{
    UINT8     u1Idx;
#ifdef CC_AUD_EQ_BASED_SBASS
    INT8    i1TotalDb;
#endif
    static BOOL   bCmp = FALSE;

    LOG(1,"CMD: set EQ mode: Dec(%d) Type(%d)\n", u1DecId, (UINT8)eEqType);

    UNUSED(u1DecId);

    if (eEqType >= AUD_EQ_NUM)
    {
        eEqType = AUD_EQ_OFF;
    }

#ifdef CC_AUD_USE_FLASH_AQ
    #if 0 //This is for FlashAQ FY10 Spec only. (driver control EQ).
    if (eEqType == AUD_EQ_OFF) //In Flash AQ mode, when Sound mode is not custom, audio driver set EQ.
    {
        eEqType = AUD_EQ_CUST1;
    }
    #endif
#endif // CC_AUD_USE_FLASH_AQ

#ifdef CC_AUD_DIFFER_EQ_PEQ_BY_CS
    if (((eEqType == AUD_EQ_CUST5) || (eEqType == AUD_EQ_CUST6)) && (BOOL)uReadShmUINT8(B_CDNOTCH_FLAG))
    { // clear sound on
        _eEqMode = eEqType - (AUD_EQ_CUST5 - AUD_EQ_CUST1);
    }
    else if (((eEqType == AUD_EQ_CUST1) || (eEqType == AUD_EQ_CUST2)) && !(BOOL)uReadShmUINT8(B_CDNOTCH_FLAG))
    { // clear sound off
        _eEqMode = eEqType + (AUD_EQ_CUST5 - AUD_EQ_CUST1);
    }
    else
#endif
    {
        _eEqMode = eEqType;
    }

#ifdef CC_AUD_EQ_BASED_SBASS
    for (u1Idx=0; u1Idx<(CH_EQ_BAND_NO + 1); u1Idx++)
    {
        i1TotalDb = _aai1ChEqCfg[eEqType][u1Idx] + _aai1ChEqCfg[AUD_EQ_SBASS][u1Idx];
        if (u1Idx == (UINT8)EQ_BAND_DRY)
        {
            // Dry gain value
            if (i1TotalDb > EQ_DRY_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = EQ_DRY_GAIN_SIDE_INDEX_MAX;
            }

            if (i1TotalDb < -EQ_DRY_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = -EQ_DRY_GAIN_SIDE_INDEX_MAX;
            }
        }
        else
        {
            // Band gain value
            if (i1TotalDb > EQ_BAND_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = EQ_BAND_GAIN_SIDE_INDEX_MAX;
            }

            if (i1TotalDb < -EQ_BAND_GAIN_SIDE_INDEX_MAX)
            {
                 i1TotalDb = -EQ_BAND_GAIN_SIDE_INDEX_MAX;
            }
        }
        _ai1ChEqFinalCfg[u1Idx] = i1TotalDb;
    }
#endif

#ifdef SUPPORT_PHILIPS_LOUDNESS
#ifndef  CC_AUD_EQ_BASED_SBASS
    AUD_DspChEqLoudness(AUD_DEC_MAIN, &(_aai1ChEqCfg[eEqType][1]));
#else
    AUD_DspChEqLoudness(AUD_DEC_MAIN, &(_ai1ChEqFinalCfg[1]));
#endif
#endif
    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    for(u1Idx = 0; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {
        if(_aai1ChEqCfg[eEqType][u1Idx] != (_aai1ChEqCfgPrev[u1Idx]))
        {
            bCmp = TRUE;
        }        
    }

    if (bCmp == TRUE)
    {
#ifdef   CC_AUD_ARM_RENDER
        for(u1Idx = 0; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
        {
            AUD_DspChEqBandMode(AUD_DEC_MAIN, u1Idx, _aai1ChEqCfg[eEqType][u1Idx]);
        }   
#else
#ifndef  CC_AUD_EQ_BASED_SBASS
    AUD_DspChEqBandMode(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_aai1ChEqCfg[eEqType][0]+EQ_DRY_GAIN_MID), FALSE);
#else
    AUD_DspChEqBandMode(AUD_DEC_MAIN, (UINT8)EQ_BAND_DRY, (UINT8)(_ai1ChEqFinalCfg[0]+EQ_DRY_GAIN_MID), FALSE);
#endif

    for(u1Idx = 1; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {
#ifndef  CC_AUD_EQ_BASED_SBASS
        AUD_DspChEqBandMode(AUD_DEC_MAIN, u1Idx, (UINT8)(_aai1ChEqCfg[eEqType][u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#else
        AUD_DspChEqBandMode(AUD_DEC_MAIN, u1Idx, (UINT8)(_ai1ChEqFinalCfg[u1Idx]+EQ_BAND_GAIN_MID), (u1Idx == CH_EQ_BAND_NO));
#endif
    }
#endif      
    }

    for(u1Idx = 0; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {
        _aai1ChEqCfgPrev[u1Idx] = _aai1ChEqCfg[eEqType][u1Idx];
    }
    bCmp = FALSE;
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);

    AUD_DspEqEnable(AUD_DEC_MAIN, TRUE);
}

//-----------------------------------------------------------------------------
/** AUD_DspChEqLoadPreset
 *   Get the current equalizer type.
 *
 *  @param  u1DecId Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetEqType(UINT8 u1DecId, AUD_EQ_TYPE_T* eEqType)
{
    UNUSED(u1DecId);
    *eEqType = _eEqMode;
}

//-----------------------------------------------------------------------------
/** _AUD_DspChSetEqTable
 *   set the band gain of a specific equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @param  eqValue  band gain, valid value from -20 ~ 20
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChSetEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[])
{
    UINT8     u1Idx;

    UNUSED(u1DecId);

    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    for (u1Idx = 0; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {
        _aai1ChEqCfg[eEqType][u1Idx] = eqValue[u1Idx];
    }
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);

}

//-----------------------------------------------------------------------------
/** _AUD_DspChQryEqTable
 *   query the band gain of a specific equalizer.
 *
 *  @param  u1DecId  Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  eEqType  the valid type number is from 0 to type 7.
 *  @param  eqValue  band gain, valid value from -20 ~ 20
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChQryEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[])
{
    UINT8     u1Idx;

    UNUSED(u1DecId);

    for (u1Idx = 0; u1Idx < (CH_EQ_BAND_NO + 1); u1Idx++)
    {
        eqValue[u1Idx] = _aai1ChEqCfg[eEqType][u1Idx];
    }
}
#ifdef CC_AUD_DRC_V20
//-----------------------------------------------------------------------------
UINT32 _gu4DrcBound[DRC2_NUM][2] = {
{0xffffff, 0x0}, // flag
{0x7fffff, 0x1000}, // band 0 target level
{0xc000, 0x0}, // band 0 attack rate
{0xc000, 0x1}, // band 0 release rate
{0x7fffff, 0x0}, // band 0 raio
{0x1000, 0x1}, // band 0 silence level
{0x600, 0x0}, // band 0 max expand
{0xff, 0x0}, // band 0 hysteresis
{0x100, 0x0}, // band 0 hold
{0x7fffff, 0x1000}, // band 1 target level
{0xc000, 0x0}, // band 1 attack rate
{0xc000, 0x1}, // band 1 release rate
{0x7fffff, 0x0}, // band 1 raio
{0x1000, 0x1}, // band 1 silence level
{0x600, 0x0}, // band 1 max expand
{0xff, 0x0}, // band 1 hysteresis
{0x100, 0x0}, // band 1 hold
{0xffffff, 0x0}, // DRC_Limiter flag
};

#define DRC2_BOUND_MAX 0
#define DRC2_BOUND_MIN 1

BOOL _AUD_DspCheckDrcBound (AUD_DRC_V20_INFO_T eType, UINT32 *u4Value)
{
    UINT32 u4Tmp = *u4Value;
    UINT32 u4Err;

    if (u4Tmp > _gu4DrcBound[eType][DRC2_BOUND_MAX])
    {
        *u4Value = _gu4DrcBound[eType][DRC2_BOUND_MAX];
        u4Err = 1;
    }
    else if (u4Tmp < _gu4DrcBound[eType][DRC2_BOUND_MIN])
    {
        *u4Value = _gu4DrcBound[eType][DRC2_BOUND_MIN];
        u4Err = 1;
    }
    else
    {
        u4Err = 0;    
    }

    if (u4Err == 0)
    {
        return 0;
    }
    else
    {
        LOG (3, "_AUD_DspSetDrc: ERROR input %d, value = 0x%x (0x%x is set)\n", 
            (UINT32) eType, u4Tmp, *u4Value);
        return 1;
    }
}

UINT16 _gu2DrcControl[DRC2_NUM] = {
        W_DRC_FLAG, // DRC2_FLAG
        D_DRC_TARGET_LEVEL, // DRC2_TARGET_LEVEL
        D_DRC_ATTACK_RATE, // DRC2_ATTACK_RATE
        D_DRC_RELEASE_RATE, // DRC2_RELEASE_RATE  
        D_DRC_RATIO, // DRC2_RATIO
        D_DRC_SILENCE_LEVEL, // DRC2_SILENCE_LEVEL
        D_DRC_MAX_EXPAND, // DRC2_MAX_EXPAND
        D_DRC_HYSTERESIS, // DRC2_HYSTERESIS
        D_DRC_HOLD, // DRC2_HOLD
        D_DRC_TARGET_LEVEL_1, // DRC2_TARGET_LEVEL_1
        D_DRC_ATTACK_RATE_1, // DRC2_ATTACK_RATE_1
        D_DRC_RELEASE_RATE_1, // DRC2_RELEASE_RATE_1
        D_DRC_RATIO_1, // DRC2_RATIO_1
        D_DRC_SILENCE_LEVEL_1, // DRC2_SILENCE_LEVEL_1
        D_DRC_MAX_EXPAND_1, // DRC2_MAX_EXPAND_1    
        D_DRC_HYSTERESIS_1, // DRC2_HYSTERESIS_1
        D_DRC_HOLD_1, // DRC2_HOLD_1
        W_DRC_LIMITER_POST_DRC_MODE, //DRC2_LIMITER_POST_DRC_MODE
};

void _AUD_DspSetDrc(AUD_DRC_V20_INFO_T eType, UINT32 u4Value, UINT8 u1DecId)
{
    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    if (eType < DRC2_NUM)
    {
        _AUD_DspCheckDrcBound (eType, &u4Value); 
        if (eType < DRC2_TARGET_LEVEL)
        {
            vWriteShmUINT16(_gu2DrcControl[eType], (UINT16) u4Value);
        }
        else
        {
            vWriteShmUINT32(_gu2DrcControl[eType], u4Value);
        }
        DSP_SendDspTaskCmd(UOP_DSP_DRC_CONTROL); 
    }
    else
    {
        LOG (3, "_AUD_DspSetDrc: error command (%d)!\n", eType);
    }
}

void _AUD_DspSetDrc2(AUD_DRC_V20_INFO_T eType, UINT32 u4Value, UINT8 u1DecId)
{
    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    if (eType < DRC2_NUM)
    {
        _AUD_DspCheckDrcBound (eType, &u4Value); 
        if (eType < DRC2_TARGET_LEVEL)
        {
            vWriteShmUINT16(_gu2DrcControl[eType], (UINT16) u4Value);
        }
        else
        {
            vWriteShmUINT32(_gu2DrcControl[eType], u4Value);
        }
    }
    else
    {
        LOG (3, "_AUD_DspSetDrc: error command (%d)!\n", eType);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetDrc
 *  get DRC control parameters.
 *
 *  @param  
 *  @param  
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetDrc(AUD_DRC_V20_INFO_T eType, UINT32 *u4Value, UINT8 u1DecId)
{
    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    if (eType < DRC2_NUM)
    {
        if (eType < DRC2_TARGET_LEVEL)
        {
            *u4Value = (UINT32) u2ReadShmUINT16(_gu2DrcControl[eType]);
        }
        else
        {
            *u4Value = u4ReadShmUINT32(_gu2DrcControl[eType]);
        }
    }
    else
    {
        LOG (3, "_AUD_DspGetDrc: error command (%d)!\n", eType);
    }    
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterMode
 *   set limiter mode
 *
 *  @param  u1Mode  limiter mode (0: off, 1: adaptive mode, 2: fixed mode).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterMode(UINT8 u1Mode)
{
    vWriteShmUINT16 (AUD_DSP0, W_LIMITER_MODE, u1Mode);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_LIMITER_CONFIG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterFlag
 *   set limiter flag
 *
 *  @param  u1Flag  limiter flag (0: off, 1: on)
 *                         bit 0: vol
 *                         bit 1: vsurr
 *                         bit 2: sbass
 *                         bit 3: EQ
 *                         bit 4: reverb
 *                         bit 5: speaker
 *                         bit 6: voice
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterFlag(UINT8 u1Flag)
{
    vWriteShmUINT16 (AUD_DSP0, W_LIMITER_FLAG, u1Flag);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_LIMITER_CONFIG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterThreshold
 *   set limiter threshold
 *
 *  @param  u4Thre  limiter threshold (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterThreshold(UINT32 u4Thre)
{
    _AUD_DspCheckDrcBound (DRC2_TARGET_LEVEL, &u4Thre);
    vWriteShmUINT32 (AUD_DSP0, D_LIMITER_FIX_THRESHOLD, u4Thre);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_LIMITER_CONFIG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterAttackRate
 *   set limiter attack rate
 *
 *  @param  u4AttackRate  limiter attack rate (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterAttackRate(UINT32 u4AttackRate)
{
    _AUD_DspCheckDrcBound (DRC2_ATTACK_RATE, &u4AttackRate);
    vWriteShmUINT32 (AUD_DSP0, D_LIMITER_ATTACK_RATE, u4AttackRate);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_LIMITER_CONFIG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterReleaseRate
 *   set limiter release rate
 *
 *  @param  u4ReleaseRate  limiter release rate (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterReleaseRate(UINT32 u4ReleaseRate)
{
    _AUD_DspCheckDrcBound (DRC2_RELEASE_RATE, &u4ReleaseRate);
    vWriteShmUINT32 (AUD_DSP0, D_LIMITER_RELEASE_RATE, u4ReleaseRate);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_LIMITER_CONFIG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetLimiterConfig
 *   get limiter configuration
 *
 *  @param  *pu1Mode  mode
 *  @param  *pu2Flag  flag
 *  @param  *pu4Thre  threshold
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetLimiterConfig(UINT8 *pu1Mode, UINT16 *pu2Flag, UINT32 *pu4Thre)
{
    *pu1Mode = (UINT8)u2ReadShmUINT16 (AUD_DSP0, W_LIMITER_MODE);
    *pu2Flag = u2ReadShmUINT16 (AUD_DSP0, W_LIMITER_FLAG);
    *pu4Thre = u4ReadShmUINT32 (AUD_DSP0, D_LIMITER_FIX_THRESHOLD);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetLimiterConfig
 *   get limiter configuration
 *
 *  @param  *pu1Mode  mode
 *  @param  *pu2Flag  flag
 *  @param  *pu4Thre  threshold
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetLimiterConfig1(UINT32 *pu4Attack, UINT32 *pu4Release, UINT32 *pu4Ratio)
{
    *pu4Attack = u4ReadShmUINT32 (AUD_DSP0, D_LIMITER_ATTACK_RATE);
    *pu4Release = u4ReadShmUINT32 (AUD_DSP0, D_LIMITER_RELEASE_RATE);
    *pu4Ratio = u4ReadShmUINT32 (AUD_DSP0, D_LIMITER_GAIN_RATIO);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterGainRatio
 *   set limiter gain ratio
 *
 *  @param  u4Ratio  limiter gain ratio (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterGainRatio(UINT32 u4Ratio)
{
    _AUD_DspCheckDrcBound (DRC2_RATIO, &u4Ratio);
    vWriteShmUINT32 (AUD_DSP0, D_LIMITER_GAIN_RATIO, u4Ratio);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_LIMITER_CONFIG);
}

#else

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterMode
 *   set limiter mode
 *
 *  @param  u1Mode  limiter mode (0: off, 1: adaptive mode, 2: fixed mode).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterMode(UINT8 u1Mode)
{
    vWriteShmUINT16 (AUD_DSP0, W_LIMITER_MODE, u1Mode);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_LIMITER_CONFIG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterFlag
 *   set limiter flag
 *
 *  @param  u1Flag  limiter flag (0: off, 1: on)
 *                         bit 0: vol
 *                         bit 1: vsurr
 *                         bit 2: sbass
 *                         bit 3: EQ
 *                         bit 4: reverb
 *                         bit 5: speaker
 *                         bit 6: voice
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterFlag(UINT8 u1Flag)
{
    vWriteShmUINT16 (AUD_DSP0, W_LIMITER_FLAG, u1Flag);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_LIMITER_CONFIG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterThreshold
 *   set limiter threshold
 *
 *  @param  u4Thre  limiter threshold (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterThreshold(UINT32 u4Thre)
{
    vWriteShmUINT32 (AUD_DSP0, D_LIMITER_FIX_THRESHOLD, u4Thre);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_LIMITER_CONFIG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetLimiterConfig
 *   get limiter configuration
 *
 *  @param  *pu1Mode  mode
 *  @param  *pu2Flag  flag
 *  @param  *pu4Thre  threshold
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetLimiterConfig(UINT8 *pu1Mode, UINT16 *pu2Flag, UINT32 *pu4Thre)
{
    *pu1Mode = (UINT8)u2ReadShmUINT16 (AUD_DSP0, W_LIMITER_MODE);
    *pu2Flag = u2ReadShmUINT16 (AUD_DSP0, W_LIMITER_FLAG);
    *pu4Thre = u4ReadShmUINT32 (AUD_DSP0, D_LIMITER_FIX_THRESHOLD);
}

#ifdef CC_RATIO_LIMITER
//-----------------------------------------------------------------------------
/** _AUD_DspSetLimiterGainRatio
 *   set limiter gain ratio
 *
 *  @param  u4Ratio  limiter gain ratio (0x0 ~ 0x7fffff)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLimiterGainRatio(UINT32 u4Ratio)
{
    vWriteShmUINT32 (AUD_DSP0, D_LIMITER_GAIN_RATIO, u4Ratio);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_LIMITER_CONFIG);
}
#endif
#endif

//-----------------------------------------------------------------------------
/** AUD_DspPL2Config
 *  main processing routine for Prologic 2.
 *
 *  @param  uCtrl control
                         PL2CTRL_SWITCH          0.
                         PL2CTRL_MODE            1.
                         PL2CTRL_PANORAMA        2.
                         PL2CTRL_DIMENSION       3.
                         PL2CTRL_C_WIDTH         4.
 *  @param  uMode corresponding parameters for PL2 control (uCtrl).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspPL2Config(UINT8 uCtrl, UINT8 uMode)
{
    UINT16 u2Config;
    UINT16 u2Mode;
    _rPL2Setting.u2Config = u2ReadShmUINT16(AUD_DSP0, W_PLIICONFIG);
    _rPL2Setting.u2Mode = u2ReadShmUINT16(AUD_DSP0, W_PLIIMODE);
    u2Config = _rPL2Setting.u2Config;
    u2Mode = _rPL2Setting.u2Mode;
    switch (uCtrl)
    {
    case PL2CTRL_SWITCH:
        if (uMode == PL2CTRL_ON)
        {
            _rPL2Setting.u2Config |=PL2_ENABLE;
            _rPL2Setting.u2Config &=~(PL2_AUTO_ENABLE|PL2_MODE_AUTO);
        }
        else if (uMode == PL2CTRL_AUTO)
        {
            _rPL2Setting.u2Config |=(PL2_ENABLE|PL2_AUTO_ENABLE|PL2_MODE_AUTO);
        }
        else if (uMode == PL2CTRL_OFF)
        {
            _rPL2Setting.u2Config &=~(PL2_ENABLE|PL2_AUTO_ENABLE|PL2_MODE_AUTO);
        }
        break;
    case PL2CTRL_MODE:
        if ((uMode & 0x1) == PL2CTRL_MODE_AUTO)
        {
            _rPL2Setting.u2Config |=(PL2_PCD_AUTO|PL2_MODE_AUTO);
        }
        else
        {
            _rPL2Setting.u2Config &=~(PL2_MODE_AUTO);
            _rPL2Setting.u2Mode = uMode>>1;
        }
        break;
    case PL2CTRL_PANORAMA:
        if (uMode == PL2CTRL_PAN_ON)
        {
            _rPL2Setting.u2Config |=PL2_PANORAMA;
        }
        else
        {
            _rPL2Setting.u2Config &=~PL2_PANORAMA;
        }
        break;
    case PL2CTRL_DIMENSION:
        /* clean field for dimension control*/
        _rPL2Setting.u2Config &= ~(PL2_DIM(0xf));
        _rPL2Setting.u2Config |= PL2_DIM((UINT16)uMode);
        break;
    case PL2CTRL_C_WIDTH:
        /* clean field for center width configuration*/
        _rPL2Setting.u2Config &= ~(PL2_CWIDTH(0xf));
        _rPL2Setting.u2Config |= PL2_CWIDTH((UINT16)uMode);
        break;
    default:
        return;
    }
    if ((u2Config == _rPL2Setting.u2Config) &&
        (u2Mode == _rPL2Setting.u2Mode))
    {
        /* if configuration is the same, don't send UOP*/
        return;
    }
    vWriteShmUINT16(AUD_DSP0, W_PLIICONFIG, _rPL2Setting.u2Config);
    vWriteShmUINT16(AUD_DSP0, W_PLIIMODE, _rPL2Setting.u2Mode);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_PRO_LOGICII_CONFIG);
}
/******************************************************************************
* Function      : AUD_DspPL2ConfigInit
* Description   : initialization for PL2 configuration
* Parameter     : u2PL2cfg--> W_PLIICONFIG,u2PL2Mode-->W_PLIIMODE
* Return        : None
******************************************************************************/
void _AUD_DspPL2ConfigInit(UINT16 u2PL2cfg,UINT16 u2PL2Mode)
{
    /*
      NOTE:
          We should call this function before audio play.
          We can have some special initialization ex. last memory from EEPROM
    */
    vWriteShmUINT16(AUD_DSP0, W_PLIICONFIG,u2PL2cfg);
    vWriteShmUINT16(AUD_DSP0, W_PLIIMODE,u2PL2Mode);
}
/******************************************************************************
* Function      : AUD_DspAC3KaraMode
* Description   : setup for AC3 karaoke mode
* Parameter     : u2KaraMode: Karaoke mode,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None
******************************************************************************/
void _AUD_DspAC3KaraMode(UINT16 u2KaraMode,UINT8 uDecIndx)
{
    UINT16 u2OldKaraMode;
    UINT16 u2ShmIndex; //=W_AC3KARAMOD;
    UINT16 u2UopIndex; //=UOP_DSP_AC3_KARAOKE_MODE;
/*
    if (uDecIndx!=0)
    {
        u2ShmIndex = W_AC3KARAMOD_DEC2;
        u2UopIndex = UOP_DSP_AC3_KARAOKE_MODE_DEC2;
    }
*/
    AUD_DEC_ID_VALIDATE_3(uDecIndx);
    switch(uDecIndx)
    {
    case AUD_DEC_AUX:
        u2ShmIndex = W_AC3KARAMOD_DEC2;
        u2UopIndex = UOP_DSP_AC3_KARAOKE_MODE_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2ShmIndex = W_AC3KARAMOD_DEC3;
        u2UopIndex = UOP_DSP_AC3_KARAOKE_MODE_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2ShmIndex = W_AC3KARAMOD;
        u2UopIndex = UOP_DSP_AC3_KARAOKE_MODE;
        break;
    }
    u2OldKaraMode = u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);

    if (u2OldKaraMode !=u2KaraMode)
    {
        vWriteShmUINT16(AUD_DSP0, u2ShmIndex,u2KaraMode);
        DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
    }
}
/******************************************************************************
* Function      : AUD_DspGetAC3KaraMode
* Description   : setup for AC3 karaoke mode
* Parameter     : uDecIndx: 0: first decoder 1: seconder decoder
* Return        : u2KaraMode
******************************************************************************/
UINT16 _AUD_DspGetAC3KaraMode(UINT8 uDecIndx)
{
    UINT16 u2KaraMode;
    UINT16 u2ShmIndex; //=W_AC3KARAMOD;

    AUD_DEC_ID_VALIDATE_3(uDecIndx);
    switch(uDecIndx)
    {
    case AUD_DEC_AUX:
        u2ShmIndex = W_AC3KARAMOD_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2ShmIndex = W_AC3KARAMOD_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2ShmIndex = W_AC3KARAMOD;
        break;
    }
    u2KaraMode = u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);
    switch(u2KaraMode)
    {
    case 1:
        u2KaraMode = 0;
        break;
    case 2:
        u2KaraMode = 1;
        break;
    case 4:
        u2KaraMode = 2;
        break;
    case 8:
        u2KaraMode = 3;
        break;
    case 16:
        u2KaraMode = 4;
        break;
    case 32:
        u2KaraMode = 5;
        break;
    default:
        u2KaraMode = 6;
        break;
    }

    return u2KaraMode;
}

/******************************************************************************
* Function      : AUD_DspAC3DualMono
* Description   : setup for AC3 dual mono mode
* Parameter     : u2DMMode: dual mono mode,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None
******************************************************************************/
void _AUD_DspAC3DualMono(UINT16 u2DMMode,UINT8 uDecIndx)
{
    UINT16 u2OldDMMode;
    UINT16 u2ShmIndex; //=W_AC3DUALMODE;
    UINT16 u2UopIndex; //=UOP_DSP_AC3_DUAL_MONO_MODE;
/*
    if (uDecIndx!=0)
    {
        u2ShmIndex = W_AC3DUALMODE_DEC2;
        u2UopIndex = UOP_DSP_AC3_DUAL_MONO_MODE;
    }
*/
    AUD_DEC_ID_VALIDATE_3(uDecIndx);
    switch(uDecIndx)
    {
    case AUD_DEC_AUX:
        u2ShmIndex = W_AC3DUALMODE_DEC2;
        u2UopIndex = UOP_DSP_AC3_DUAL_MONO_MODE_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2ShmIndex = W_AC3DUALMODE_DEC3;
        u2UopIndex = UOP_DSP_AC3_DUAL_MONO_MODE_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2ShmIndex = W_AC3DUALMODE;
        u2UopIndex = UOP_DSP_AC3_DUAL_MONO_MODE;
        break;
    }
    u2OldDMMode = u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);

    if (u2DMMode != u2OldDMMode)
    {
        vWriteShmUINT16(AUD_DSP0, u2ShmIndex,u2DMMode);
        DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
    }
}

/******************************************************************************
* Function      : AUD_DspGetAC3DualMono
* Description   : setup for AC3 dual mono mode
* Parameter     : u2DMMode: dual mono mode,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None
******************************************************************************/
UINT16 _AUD_DspGetAC3DualMono(UINT8 uDecIndx)
{
    UINT16 u2DMMode;
    UINT16 u2ShmIndex; //=W_AC3DUALMODE;

    AUD_DEC_ID_VALIDATE_3(uDecIndx);
    switch(uDecIndx)
    {
    case AUD_DEC_AUX:
        u2ShmIndex = W_AC3DUALMODE_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2ShmIndex = W_AC3DUALMODE_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2ShmIndex = W_AC3DUALMODE;
        break;
    }
    u2DMMode = u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);

    return u2DMMode;
}

//-----------------------------------------------------------------------------
/** AUD_DspAC3CompMode
 *  setup for compression mode.
 *
 *  @param  u2CompMode compression mode.
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAC3CompMode(UINT16 u2CompMode,UINT8 uDecIndx)
{
    UINT16 u2OldDMMode;
    UINT16 u2ShmIndex; //=W_AC3COMPMOD;
    UINT16 u2UopIndex; //=UOP_DSP_AC3_COMPRESSION_MODE;
/*
    if (uDecIndx!=0)
    {
        u2ShmIndex = W_AC3COMPMOD_DEC2;
        u2UopIndex = UOP_DSP_AC3_COMPRESSION_MODE_DEC2;
    }
*/
    AUD_DEC_ID_VALIDATE_3(uDecIndx);
    switch(uDecIndx)
    {
    case AUD_DEC_AUX:
        u2ShmIndex = W_AC3COMPMOD_DEC2;
        u2UopIndex = UOP_DSP_AC3_COMPRESSION_MODE_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2ShmIndex = W_AC3COMPMOD_DEC3;
        u2UopIndex = UOP_DSP_AC3_COMPRESSION_MODE_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2ShmIndex = W_AC3COMPMOD;
        u2UopIndex = UOP_DSP_AC3_COMPRESSION_MODE;
        break;
    }
    u2OldDMMode = u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);

    if (u2CompMode != u2OldDMMode)
    {
        vWriteShmUINT16(AUD_DSP0, u2ShmIndex,u2CompMode);
        DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetAC3CompMode
 *  setup for compression mode.
 *
  *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval u2CompMode compression mode.
 */
//-----------------------------------------------------------------------------
UINT16 _AUD_DspGetAC3CompMode(UINT8 uDecIndx)
{
    UINT16 u2DMMode;
    UINT16 u2ShmIndex;//=W_AC3COMPMOD;

    //u2ShmIndex = (uDecIndx == AUD_DEC_MAIN)? W_AC3COMPMOD : W_AC3COMPMOD_DEC2;
    AUD_DEC_ID_VALIDATE_3(uDecIndx);
    switch(uDecIndx)
    {
    case AUD_DEC_AUX:
        u2ShmIndex = W_AC3COMPMOD_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2ShmIndex = W_AC3COMPMOD_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2ShmIndex = W_AC3COMPMOD;
        break;
    }

    u2DMMode = u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);
    return u2DMMode;

}

//-----------------------------------------------------------------------------
/** AUD_DspAC3DRCRange
 *  setup range for dynamix range compression.
 *
 *  @param  uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 *                               0.0 ~ 1.0 step 0.125
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAC3DRCRange(UINT8 uDRCLevel,UINT8 u1DecId)
{
    UINT16 u2DRCLowIdx; //=D_AC3DYN_LOW;
    UINT16 u2DRCHighIdx; //=D_AC3DYN_HIGH;
    UINT16 u2DRCLowUop; //=UOP_DSP_AC3_DYNAMIC_LOW;
    UINT16 u2DRCHighUop; //=UOP_DSP_AC3_DYNAMIC_HIGH;

    AUD_DEC_ID_VALIDATE_3(u1DecId);
    if (uDRCLevel >= DRC_RANGE_MAX)
    {
        uDRCLevel = DRC_RANGE_MAX - 1;
    }
/*
    if (u1DecId != 0)
    {
        u2DRCLowIdx=D_AC3DYN_LOW_DEC2;
        u2DRCHighIdx=D_AC3DYN_HIGH_DEC2;
        u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW_DEC2;
        u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH_DEC2;
    }
*/
    switch(u1DecId)
    {
    case AUD_DEC_AUX:
        u2DRCLowIdx=D_AC3DYN_LOW_DEC2;
        u2DRCHighIdx=D_AC3DYN_HIGH_DEC2;
        u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW_DEC2;
        u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2DRCLowIdx=D_AC3DYN_LOW_DEC3;
        u2DRCHighIdx=D_AC3DYN_HIGH_DEC3;
        u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW_DEC3;
        u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2DRCLowIdx=D_AC3DYN_LOW;
        u2DRCHighIdx=D_AC3DYN_HIGH;
        u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW;
        u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH;
        break;
    }
    vWriteShmUINT32(AUD_DSP0, u2DRCLowIdx, _au4Ac3DrcRange[uDRCLevel]);
    DSP_SendDspTaskCmd(AUD_DSP0, u2DRCLowUop);
    vWriteShmUINT32(AUD_DSP0, u2DRCHighIdx, _au4Ac3DrcRange[uDRCLevel]);
    DSP_SendDspTaskCmd(AUD_DSP0, u2DRCHighUop);
}

/** AUD_DspAC3DRCRange100
 *  setup range for dynamix range compression.
 *
 *  @param  uDRCLevel  DRC range 0~100.
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAC3DRCRange100(UINT8 uDRCLevel,UINT8 u1DecId)
{
    UINT16 u2DRCLowIdx; //=D_AC3DYN_LOW;
    UINT16 u2DRCHighIdx; //=D_AC3DYN_HIGH;
    UINT16 u2DRCLowUop; //=UOP_DSP_AC3_DYNAMIC_LOW;
    UINT16 u2DRCHighUop; //=UOP_DSP_AC3_DYNAMIC_HIGH;
    UINT32 u2drclEVEL;

    AUD_DEC_ID_VALIDATE_3(u1DecId);
    u2drclEVEL= (UINT32)uDRCLevel*0x7fffff/100;
/*
    if (u1DecId != 0)
    {
        u2DRCLowIdx=D_AC3DYN_LOW_DEC2;
        u2DRCHighIdx=D_AC3DYN_HIGH_DEC2;
        u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW_DEC2;
        u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH_DEC2;
    }
*/
    switch(u1DecId)
    {
    case AUD_DEC_AUX:
        u2DRCLowIdx=D_AC3DYN_LOW_DEC2;
        u2DRCHighIdx=D_AC3DYN_HIGH_DEC2;
        u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW_DEC2;
        u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2DRCLowIdx=D_AC3DYN_LOW_DEC3;
        u2DRCHighIdx=D_AC3DYN_HIGH_DEC3;
        u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW_DEC3;
        u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2DRCLowIdx=D_AC3DYN_LOW;
        u2DRCHighIdx=D_AC3DYN_HIGH;
        u2DRCLowUop=UOP_DSP_AC3_DYNAMIC_LOW;
        u2DRCHighUop=UOP_DSP_AC3_DYNAMIC_HIGH;
        break;
    }
    vWriteShmUINT32(AUD_DSP0, u2DRCLowIdx, u2drclEVEL);
    DSP_SendDspTaskCmd(AUD_DSP0, u2DRCLowUop);
    vWriteShmUINT32(AUD_DSP0, u2DRCHighIdx, u2drclEVEL);
    DSP_SendDspTaskCmd(AUD_DSP0, u2DRCHighUop);
}

#if 0   // Unused
//-----------------------------------------------------------------------------
/** _AUD_DspGetAC3DRCRange
 *  setup range for dynamix range compression.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 *                               0.0 ~ 1.0 step 0.125
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetAC3DRCRange(UINT8 u1DecId)
{
    UINT16 u2DRCIdx;
    UINT32 u4DRCLevel;
    UINT8 uDRCLevelIndex;

    AUD_DEC_ID_VALIDATE_3(u1DecId);
    //u2DRCIdx=(u1DecId==AUD_DEC_MAIN)? D_AC3DYN_LOW: D_AC3DYN_LOW_DEC2;
    switch(u1DecId)
    {
    case AUD_DEC_AUX:
        u2DRCIdx = D_AC3DYN_LOW_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2DRCIdx = D_AC3DYN_LOW_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2DRCIdx = D_AC3DYN_LOW;
        break;
    }
    u4DRCLevel = u4ReadShmUINT32(AUD_DSP0, u2DRCIdx);

    for (uDRCLevelIndex = 0; uDRCLevelIndex<DRC_RANGE_MAX; uDRCLevelIndex++ )
    {
        if (u4DRCLevel == _au4Ac3DrcRange[uDRCLevelIndex])
        {
            break;
        }
    }
    return uDRCLevelIndex;

}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspGetAC3DRCRange100
 *  setup range for dynamix range compression.
 *
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval uDRCLevel  DRC range.
 *                               0x00000000-->0.0
 *                               0x007FFFFF-->1.0
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetAC3DRCRange100(UINT8 u1DecId)
{
    UINT16 u2DRCIdx;
    UINT32 u4DRCLevel;
    UINT8 uDRCLevelIndex;

    AUD_DEC_ID_VALIDATE_3(u1DecId);
    //u2DRCIdx=(u1DecId==AUD_DEC_MAIN)? D_AC3DYN_LOW: D_AC3DYN_LOW_DEC2;
    switch(u1DecId)
    {
    case AUD_DEC_AUX:
        u2DRCIdx = D_AC3DYN_LOW_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2DRCIdx = D_AC3DYN_LOW_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2DRCIdx = D_AC3DYN_LOW;
        break;
    }
    u4DRCLevel = u4ReadShmUINT32(AUD_DSP0, u2DRCIdx);

    for (uDRCLevelIndex = 0; uDRCLevelIndex <= 100; uDRCLevelIndex ++)
    {
        if (u4DRCLevel == (UINT32)uDRCLevelIndex*0x7fffff/100)
        {
            break;
        }
    }
    return uDRCLevelIndex;
}

void _Aud_G726DecCfg(UINT8 u1DecId, UINT8 u1rate)
{
    UINT16 u2UopIndex=UOP_DSP_G726_CFG;

    if (u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT8(AUD_DSP0, B_G726_RATE, u1rate);
        u2UopIndex = UOP_DSP_G726_CFG;
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        vWriteShmUINT8(AUD_DSP0, B_G726_RATE_DEC2, u1rate);
        u2UopIndex=UOP_DSP_G726_CFG_DEC2;
    }
    else //if (u1DecId == AUD_DEC_THIRD)
    {
        vWriteShmUINT8(AUD_DSP0, B_G726_RATE_DEC3, u1rate);
        u2UopIndex=UOP_DSP_G726_CFG_DEC3;
    }

    DSP_SendDspTaskCmd (AUD_DSP0, u2UopIndex);

    return;

}

#ifdef CC_AUD_SUPPORT_MS10
//-----------------------------------------------------------------------------
/** _AUD_DspSetDDCControl
 *  setup for DDC control.
 *
 *  @param  u2Control DDC control
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCControl (UINT16 u2Control, UINT8 uDecIndx)
{
    UINT16 u2OldMode;
    UINT16 u2ShmIndex=W_DDC_CONTROL;
    UINT16 u2UopIndex=UOP_DSP_DDC_CONTROL;

    LOG(5, "_AUD_DspSetDDCControl = %d, %d\n", u2Control, uDecIndx);

    if (uDecIndx!=0)
    {
        u2ShmIndex = W_DDC_CONTROL_DEC2;
        u2UopIndex = UOP_DSP_DDC_CONTROL_DEC2;
    }

    u2OldMode = u2ReadShmUINT16 (AUD_DSP0, u2ShmIndex);

    // only for DDC control, bit 2, 3
    u2Control = (u2OldMode & 0x3) | (u2Control << 2);

    if (u2Control != u2OldMode)
    {
        vWriteShmUINT16 (AUD_DSP0, u2ShmIndex, u2Control);
        DSP_SendDspTaskCmd (AUD_DSP0, u2UopIndex);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspDDCStrmId
 *  setup for DDC associated stream id.
 *
 *  @param  u1StrmId stream id
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCStrmId (UINT8 u1StrmId, UINT8 uDecIndx)
{
    UINT8 u1OldMode;
    UINT16 u2ShmIndex=B_DDC_STRMID;
    UINT16 u2UopIndex=UOP_DSP_DDC_STRMID;

    if (uDecIndx!=0)
    {
        u2ShmIndex = B_DDC_STRMID_DEC2;
        u2UopIndex = UOP_DSP_DDC_STRMID_DEC2;
    }

    u1OldMode = uReadShmUINT8 (AUD_DSP0, u2ShmIndex);

    if (u1StrmId != u1OldMode)
    {
        vWriteShmUINT8 (AUD_DSP0, u2ShmIndex, u1StrmId);
        DSP_SendDspTaskCmd (AUD_DSP0, u2UopIndex);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetDDCAdEnable
 *  setup for.
 *
 *  @param  u1StrmId stream id
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCAdEnable (UINT8 u1DecId, BOOL fgEnable)
{
#ifndef CC_MT5391_AUD_3_DECODER
    UNUSED(u1DecId);
    UNUSED(fgEnable);
    return;
#else
    UINT16 u2ShmIndex = B_ADFLAG;
    UINT32 u2UopIndex = UOP_DSP_AD_FLAG;
    UINT8   u1ADFLAGValue,u1OldAdFlag;

    DSP_SetTriDecMode(fgEnable);
    u1OldAdFlag = uReadShmUINT8(AUD_DSP0, B_ADFLAG);
    u1ADFLAGValue = ((u1OldAdFlag & (0xFE)) | fgEnable) | (0x40);       //bit6: one buffer mode

    if (u1OldAdFlag != u1ADFLAGValue)
    {
        vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)u1ADFLAGValue);
        DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
    }
#endif
}

//-----------------------------------------------------------------------------
/** _AUD_DspDDCStrmId
 *  setup for DDC associated stream id.
 *
 *  @param  uMixer control of mixer
 *  @param  uDecIndx 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCMixer (UINT8 uMixer, UINT8 uDecIndx)
{
    UINT8 u1OldMode;
    UINT16 u2ShmIndex=B_DDC_ASSOC_MIX;
    UINT16 u2UopIndex=UOP_DSP_DDC_ASSOC_MIX;

    if (uDecIndx!=0)
    {
        u2ShmIndex = B_DDC_ASSOC_MIX_DEC2;
        u2UopIndex = UOP_DSP_DDC_ASSOC_MIX_DEC2;
    }

    u1OldMode = uReadShmUINT8 (AUD_DSP0, u2ShmIndex);

    if (uMixer != u1OldMode)
    {
        vWriteShmUINT8 (AUD_DSP0, u2ShmIndex, uMixer);
        DSP_SendDspTaskCmd (AUD_DSP0, u2UopIndex);
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspDdtDmxRfMode
 *  setup for DDT dmx mode or RF mode.
 *
 *  @param  u1Mode: dmxmode/RF mode, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder, bSetDmx 0: set dmx, 1: set RF
 *  @retval void
 *  u1Mode:
 *  00b: Line, Lt/Rt
 *  01b: Line, Lo/Ro
 *  10b: RF, Lt/Rt
 *  11b: RF, Lo/Ro
 */
//-----------------------------------------------------------------------------
void _AUD_DspDdtDmxRfMode (UINT8 u1Mode, UINT8 u1DecId, BOOL bSetDmx)
{
    UINT8 u1SpkCfgDMMode, u1DdtDmxMode;

    u1DdtDmxMode = u1Mode;

    if (bSetDmx)        // set LtRt/LoRo dmx
    {
        u1SpkCfgDMMode = _AUD_DspGetSpeakerOutputConfig(u1DecId);
        switch(u1DdtDmxMode)
        {
            case 0: // LtRt
                if (u1SpkCfgDMMode != DMX_SPKCFG_LTRT)
                    _AUD_DspSpeakerOutputConfig(u1DecId, DMX_SPKCFG_LTRT);
                break;
            case 1: // LoRo
                if (u1SpkCfgDMMode != DMX_SPKCFG_LORO)
                    _AUD_DspSpeakerOutputConfig(u1DecId, DMX_SPKCFG_LORO);
                break;
            default:
                printf("incorrect downmix mode\n");
                break;
        }

    }
    else                // set Rf/Line dmx
    {

        UINT8  u1OldDMMode;
        UINT16 u2ShmIndex=B_DDT_COMPMODE;
        UINT16 u2UopIndex=UOP_DSP_DDT_COMPMODE;

        if (u1DecId == AUD_DEC_AUX)
        {
            u2ShmIndex = B_DDT_COMPMODE_DEC2;
            u2UopIndex = UOP_DSP_DDT_COMPMODE_DEC2;
        }
        else if (u1DecId == AUD_DEC_THIRD)
        {
            u2ShmIndex = B_DDT_COMPMODE_DEC3;
            u2UopIndex = UOP_DSP_DDT_COMPMODE_DEC3;
        }

        u1OldDMMode = uReadShmUINT8(AUD_DSP0, u2ShmIndex);
        switch(u1DdtDmxMode)
        {
            case 0: // Line
                if (u1OldDMMode != DMX_MS10_LINE)
                    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, DMX_MS10_LINE);
                break;
            case 1: // RF
                if (u1OldDMMode != DMX_MS10_RF)
                    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, DMX_MS10_RF);
                break;
            default:
                printf("incorrect downmix mode\n");
                return;
        }

        DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
    }

}

//-----------------------------------------------------------------------------
/** AUD_DspDdtDualMode
 *  setup for DDT dual mode.
 *
 *  @param  u1Mode: dual mode, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspDdtDualMode (UINT8 u1Mode, UINT8 u1DecId)
{
    UINT8 u1OldDMMode;
    UINT16 u2ShmIndex=B_DDT_DUALMODE;
    UINT16 u2UopIndex=UOP_DSP_DDT_DUAL_MODE;

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = B_DDT_DUALMODE_DEC2;
        u2UopIndex = UOP_DSP_DDT_DUAL_MODE_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = B_DDT_DUALMODE_DEC3;
        u2UopIndex = UOP_DSP_DDT_DUAL_MODE_DEC3;
    }
    u1OldDMMode = uReadShmUINT8(AUD_DSP0, u2ShmIndex);

    if (u1Mode != u1OldDMMode)
    {
        vWriteShmUINT8(AUD_DSP0, u2ShmIndex,u1Mode);
        DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
    }

}


//-----------------------------------------------------------------------------
/** _AUD_DspDdtCompValRange
 *  setup for DDT DRC cut/boost factor.
 *
 *  @param  u1Mode: cut/boost factor 0~100, u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspDdtCompValRange (UINT8 u1Mode, UINT8 u1DecId)
{
    UINT8 u1OldDMMode, u1NewDMMode;
    UINT16 u2ShmIndex1=B_DDT_COMPVAL;
    UINT16 u2UopIndex1=UOP_DSP_DDT_COMPVAL;

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex1 = B_DDT_COMPVAL_DEC2;
        u2UopIndex1 = UOP_DSP_DDT_COMPVAL_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex1 = B_DDT_COMPVAL_DEC3;
        u2UopIndex1 = UOP_DSP_DDT_COMPVAL_DEC3;
    }
    u1OldDMMode = uReadShmUINT8(AUD_DSP0, u2ShmIndex1);
    u1NewDMMode = u1Mode;
    if (u1NewDMMode != u1OldDMMode)
    {
        vWriteShmUINT8(AUD_DSP0, u2ShmIndex1,u1NewDMMode);
        DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex1);
    }


}
#endif  // CC_AUD_SUPPORT_MS10

//-----------------------------------------------------------------------------
/** _AUD_DspGetDdtDmxRfMode
 *  get DDT dmx mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder. 2: third decoder, bGetDmx 0: get dmx, 1: get RF
 *  u1Mode:
 *  00b: Line, Lt/Rt
 *  01b: Line, Lo/Ro
 *  10b: RF, Lt/Rt
 *  11b: RF, Lo/Ro
 *  @retval u1Mode: dmxmode/RF mode
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetDdtDmxRfMode (UINT8 u1DecId,BOOL bGetDmx)
{
    UINT8 u1Mode;

    if (bGetDmx)
    {
        u1Mode = _AUD_DspGetSpeakerOutputConfig(u1DecId);
        if (u1Mode == DMX_SPKCFG_LORO)
            return 1;                       // LoRo
        else if (u1Mode == DMX_SPKCFG_LTRT)
            return 0;                       // LtRt
        else
            return 0xFF;
    }
    else
    {
        UINT16 u2ShmIndex;

        u2ShmIndex = (u1DecId == AUD_DEC_MAIN)? B_DDT_COMPMODE : B_DDT_COMPMODE_DEC2;
        u1Mode = uReadShmUINT8(AUD_DSP0, u2ShmIndex);

        if (u1Mode == DMX_MS10_LINE)        //Line
            return 0;
        else if (u1Mode == DMX_MS10_RF)     //RF
            return 1;
        else
            return 0xFF;

    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetDdtDualMode
 *  get DDT dual mode.
 *
 *  @param   u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval u1Mode: dual mode
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetDdtDualMode (UINT8 u1DecId)
{
    UINT16 u2DmxIdx = B_DDT_DUALMODE;

    if (u1DecId==AUD_DEC_AUX || u1DecId==AUD_DEC_THIRD)
    {
        u2DmxIdx = B_DDT_DUALMODE_DEC2;
    }

    return(uReadShmUINT8(AUD_DSP0, u2DmxIdx));

}


//-----------------------------------------------------------------------------
/** _AUD_DspGetDdtDrcRange
 *  get DDT DRC cut/boost factor.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder. 2: third decoder
 *  @retval u1Mode: cut/boost factor 0~100
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetDdtDrcRange (UINT8 u1DecId)
{
    UINT16 u2DmxIdx = B_DDT_COMPVAL;
    UINT8 u1DrcFactor;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2DmxIdx = B_DDT_COMPVAL_DEC2;
    }
    u1DrcFactor = uReadShmUINT8(AUD_DSP0, u2DmxIdx);
    return u1DrcFactor;
}

/******************************************************************************
* Function      : AdspSpeakerConfig
* Description   : speaker configuration setup
* Parameter     : u4SpkCfg: Speaker config,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None
******************************************************************************/
/*
  D_SPKCFG,D_SPKCFG_DEC2 (currently second decoder only support 2/0,1/0,3/0 configuration)
  length: 24 bits
  description is the following
  bit 0 ~ 2:
    b000: 2/0 (LT/RT downmix: prologic compatible)
    b001: 1/0
    b010: 2/0 (LO/RO)
    b011: 3/0
    b100: 2/1
    b101: 3/1
    b110: 2/2
    b111: 3/2
  bit 5: Subwoofer Channel present(1)/absent(0)
  bit 12: Center Channel large(1)/small(0)
  bit 13: Left Channel large(1)/small(0)
  bit 14: Right Channel large(1)/small(0)
  bit 15: Left Surround Channel large(1)/small(0)
  bit 16: Right Surround Channel large(1)/small(0)
*/
void _AUD_DspSpeakerConfig(UINT32 u4SpkCfg,UINT8 uDecIndex)
{
    UINT16 u2SpkCfgIndx; // = D_SPKCFG;
    UINT16 u2SpkCfgUop; // = UOP_DSP_CONFIG_SPEAKER;

    AUD_DEC_ID_VALIDATE_3(uDecIndex);
/*
    if (uDecIndex!=0)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
    }
*/
    switch(uDecIndex)
    {
    case AUD_DEC_AUX:
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2SpkCfgIndx = D_SPKCFG_DEC3;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2SpkCfgIndx = D_SPKCFG;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
        break;
    }

    vWriteShmUINT32(AUD_DSP0, u2SpkCfgIndx, u4SpkCfg);
    DSP_SendDspTaskCmd(AUD_DSP0, u2SpkCfgUop);
}

UINT32 _AUD_DspGetSpeakerConfig(UINT8 uDecIndex)
{
    UINT16 u2SpkCfgIndx; // = D_SPKCFG;

    AUD_DEC_ID_VALIDATE_3(uDecIndex);
/*
    if (uDecIndex != 0)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
    }
*/
    switch(uDecIndex)
    {
    case AUD_DEC_AUX:
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2SpkCfgIndx = D_SPKCFG_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2SpkCfgIndx = D_SPKCFG;
        break;
    }

    return u4ReadShmUINT32(AUD_DSP0, u2SpkCfgIndx);
}

#if 0   // Unused
/******************************************************************************
* Function      : AUD_DspSpeakerSizeConfig
* Description   : speaker configuration setup
* Parameter     : u4SpkCfg: Speaker config,uDecIndx: 0: first decoder 1: seconder decoder
* Return        : None
******************************************************************************/
/*
  D_SPKCFG,D_SPKCFG_DEC2 (currently second decoder only support 2/0,1/0,3/0 configuration)
  length: 24 bits
  description is the following
  bit 0 ~ 2:
    b000: 2/0 (LT/RT downmix: prologic compatible)
    b001: 1/0
    b010: 2/0 (LO/RO)
    b011: 3/0
    b100: 2/1
    b101: 3/1
    b110: 2/2
    b111: 3/2
  bit 5: Subwoofer Channel present(1)/absent(0)
  bit 12: Center Channel large(1)/small(0)
  bit 13: Left Channel large(1)/small(0)
  bit 14: Right Channel large(1)/small(0)
  bit 15: Left Surround Channel large(1)/small(0)
  bit 16: Right Surround Channel large(1)/small(0)
*/

void _AUD_DspSpeakerSizeConfig(UINT8 uDecIndex, BOOL fgFrontLarge, BOOL fgSurroundLarge, BOOL fgCenterLarge)
{
    UINT16 u2SpkCfgIndx; // = D_SPKCFG;
    UINT16 u2SpkCfgUop; // = UOP_DSP_CONFIG_SPEAKER;
    UINT32 u4SpkCfg = 0;

    AUD_DEC_ID_VALIDATE_3(uDecIndex);
    UNUSED(fgSurroundLarge);
/*
    if (uDecIndex!=0)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
        u4SpkCfg = u4ReadShmUINT32(D_SPKCFG_DEC2) & ~DSP_SHM_SPK_SIZE_MASK;
    }
    else
    {
        u4SpkCfg = u4ReadShmUINT32(D_SPKCFG) & ~DSP_SHM_SPK_SIZE_MASK;
    }
*/
    switch(uDecIndex)
    {
    case AUD_DEC_AUX:
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2SpkCfgIndx = D_SPKCFG_DEC3;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2SpkCfgIndx = D_SPKCFG;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
        break;
    }
    u4SpkCfg = u4ReadShmUINT32(AUD_DSP0, u2SpkCfgIndx) & ~DSP_SHM_SPK_SIZE_MASK;
    if (fgFrontLarge)
    {
        u4SpkCfg |= ((UINT32)0x3 << 13);
    }
    if (fgFrontLarge)
    {
        u4SpkCfg |= ((UINT32)0x3 << 15);
    }
    if (fgCenterLarge)
    {
        u4SpkCfg |= ((UINT32)0x1 << 12);
    }

    vWriteShmUINT32(AUD_DSP0, u2SpkCfgIndx, u4SpkCfg);
    DSP_SendDspTaskCmd(AUD_DSP0, u2SpkCfgUop);
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerLargeSmallConfig
 *  Speaker size configuration of each channel pair.
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  u1FrontPair          Speaker size configuration for front left and right channels. (1: large, 0: small)
 *  @param  u2Center             Speaker size configuration for center channel. (1: large, 0: small)
 *  @param  u1Surround          Speaker size configuration for left and right surround channels. (1: large, 0: small)
 *  @param  u1CH910              Speaker size configuration for CH9/10 channels. (1: large, 0: small)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
// large(1)/small(0)
void _AUD_DspSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8 u1FrontPair, UINT8 u1Center, UINT8 u1Surround, UINT8 u1CH910)
{
    UINT32 u4Config = 0;
    UINT16 u2SpkCfgIndx; // = D_SPKCFG;
    UINT16 u2SpkCfgUop; // = UOP_DSP_CONFIG_SPEAKER;

    AUD_DEC_ID_VALIDATE_3(u1DecIndex);
/*
    if (u1DecIndex != AUD_DEC_MAIN)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
    }
*/
    switch(u1DecIndex)
    {
    case AUD_DEC_AUX:
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2SpkCfgIndx = D_SPKCFG_DEC3;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2SpkCfgIndx = D_SPKCFG;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
        break;
    }
    if (u1Center == 1)
    {
        u4Config |= 0x01 << 12; // bit 12
    }
    if (u1FrontPair == 1)
    {
        u4Config |= 0x06 << 12; // bit 13 & 14
    }
    if (u1Surround == 1)
    {
        u4Config |= 0x18 << 12; // bit 15 & 16
    }
    if (u1CH910 == 1)
    {
        u4Config |= 0x3 << 19; // bit 20 & 19
    }

    vWriteShmUINT32(AUD_DSP0, u2SpkCfgIndx, (u4ReadShmUINT32(AUD_DSP0, u2SpkCfgIndx) & 0xffe60fff) | u4Config);
    DSP_SendDspTaskCmd(AUD_DSP0, u2SpkCfgUop);
}

void _AUD_DspGetSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8* u1FrontPair, UINT8* u1Center, UINT8* u1Surround, UINT8* u1CH910)
{
    UINT16 u2SpkCfgIndx; // = D_SPKCFG;
    UINT32 u4Config;

    AUD_DEC_ID_VALIDATE_3(u1DecIndex);
/*
    if (u1DecIndex != AUD_DEC_MAIN)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
    }
*/
    switch(u1DecIndex)
    {
    case AUD_DEC_AUX:
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2SpkCfgIndx = D_SPKCFG_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2SpkCfgIndx = D_SPKCFG;
        break;
    }
    u4Config = u4ReadShmUINT32(AUD_DSP0, u2SpkCfgIndx);

    *u1FrontPair = ((u4Config & (0x06<<12))>0);
    *u1Center = ((u4Config & (0x3<<12))>0);
    *u1Surround = ((u4Config & (0x18<<12))>0);
    *u1CH910 = ((u4Config & (0x3<<19))>0);
}

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerLsConfig
 *  setup speaker size of each channel.
 *
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @param  u1L  large(1)/small(0).
 *  @param  u1R  large(1)/small(0).
 *  @param  u1Ls   large(1)/small(0).
 *  @param  u1Rs   large(1)/small(0).
 *  @param  u1C  large(1)/small(0).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSpeakerLsConfig(UINT8 u1DecIndex, UINT8 u1L, UINT8 u1R, UINT8 u1Ls, UINT8 u1Rs, UINT8 u1C)
{
    UINT32 u4Config = 0;
    UINT16 u2SpkCfgIndx; // = D_SPKCFG;
    UINT16 u2SpkCfgUop; // = UOP_DSP_CONFIG_SPEAKER;

    AUD_DEC_ID_VALIDATE_3(u1DecIndex);
/*
    if (u1DecIndex != AUD_DEC_MAIN)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
    }
*/
    switch(u1DecIndex)
    {
    case AUD_DEC_AUX:
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2SpkCfgIndx = D_SPKCFG_DEC3;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2SpkCfgIndx = D_SPKCFG;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
        break;
    }
    if (u1C == 1)
    {
        u4Config |= 0x01 << 12; // bit 12
    }
    if (u1L == 1)
    {
        u4Config |= 0x01 << 13; // bit 13
    }
    if (u1R == 1)
    {
        u4Config |= 0x01 << 14; // bit 14
    }
    if (u1Ls == 1)
    {
        u4Config |= 0x01 << 15; // bit 15
    }
    if (u1Rs == 1)
    {
        u4Config |= 0x01 << 16; // bit 16
    }

    vWriteShmUINT32(AUD_DSP0, u2SpkCfgIndx, (u4ReadShmUINT32(AUD_DSP0, u2SpkCfgIndx) & 0xfffe0fff) | u4Config);
    DSP_SendDspTaskCmd(AUD_DSP0, u2SpkCfgUop);
}

//static UINT32 _au4SpeakerOutCfg[AUD_DEC_NUM]= {7,7};
//#define SPK_CFG_MASK (0x07)

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerOutputConfig
 *  set speaker output config.
 *
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @param  u1SpkCfg.
 *                              bit 0 ~ 2:.
 *                              b000: 2/0 (LT/RT downmix: prologic compatible).
 *                              b001: 1/0.
 *                              b010: 2/0 (LO/RO).
 *                              b011: 3/0.
 *                              b100: 2/1.
 *                              b101: 3/1.
 *                              b110: 2/2.
 *                              b111: 3/2.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSpeakerOutputConfig(UINT8 u1DecIndex, UINT8 u1SpkCfg)
{
    UINT16 u2SpkCfgIndx; // = D_SPKCFG;
    UINT16 u2SpkCfgUop; // = UOP_DSP_CONFIG_SPEAKER;
    UINT8 u1CurSpkCfg = 0;

    AUD_DEC_ID_VALIDATE_3(u1DecIndex);
//    if ((u1SpkCfg & SPK_CFG_MASK) != _au4SpeakerOutCfg[u1DecIndex])
    {
/*
        if (u1DecIndex != AUD_DEC_MAIN)
        {
            u2SpkCfgIndx = D_SPKCFG_DEC2;
            u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
        }
*/
        switch(u1DecIndex)
        {
        case AUD_DEC_AUX:
            u2SpkCfgIndx = D_SPKCFG_DEC2;
            u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
            break;
        case AUD_DEC_THIRD:
            u2SpkCfgIndx = D_SPKCFG_DEC3;
            u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC3;
            break;
        case AUD_DEC_MAIN:
        default:
            u2SpkCfgIndx = D_SPKCFG;
            u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
            break;
        }
        // If new setting is the same as the original
        u1CurSpkCfg= (u4ReadShmUINT32(AUD_DSP0, u2SpkCfgIndx) & 0x47);
        if (u1CurSpkCfg == u1SpkCfg)
        {
            return;
        }

        vWriteShmUINT32(AUD_DSP0, u2SpkCfgIndx, (u4ReadShmUINT32(AUD_DSP0, u2SpkCfgIndx) & 0xffffffb8) | (UINT32)u1SpkCfg);
        DSP_SendDspTaskCmd(AUD_DSP0, u2SpkCfgUop);
    }
}

UINT8 _AUD_DspGetSpeakerOutputConfig(UINT8 u1DecIndex)
{
    UINT16 u2SpkCfgIndx; // = D_SPKCFG;

    AUD_DEC_ID_VALIDATE_3(u1DecIndex);
/*
    if (u1DecIndex != AUD_DEC_MAIN)
    {
        u2SpkCfgIndx = D_SPKCFG_DEC2;
    }
*/
    switch(u1DecIndex)
    {
    case AUD_DEC_AUX:
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2SpkCfgIndx = D_SPKCFG_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2SpkCfgIndx = D_SPKCFG;
        break;
    }

    return (u4ReadShmUINT32(AUD_DSP0, u2SpkCfgIndx) & 0x47);
}

//-----------------------------------------------------------------------------
/** AUD_DspSpeakerSubwooferEnable
 *  Turn On/Off subwoofer
 *  @param  u1DecIndex          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgEnable              Subwoofer channel is turned on or off. (1: on, 0: off)
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
// bit 5: Subwoofer Channel present(1)/absent(0)
void _AUD_DspSpeakerSubwooferEnable(UINT8 u1DecIndex, BOOL fgEnable)
{
    UINT32 u4Config = 0;
    UINT16 u2SpkCfgIndx;
    UINT16 u2SpkCfgUop;

    AUD_DEC_ID_VALIDATE_3(u1DecIndex);

    switch(u1DecIndex)
    {
    case AUD_DEC_AUX:
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2SpkCfgIndx = D_SPKCFG_DEC3;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2SpkCfgIndx = D_SPKCFG;
        u2SpkCfgUop = UOP_DSP_CONFIG_SPEAKER;
        break;
    }
    if (fgEnable)
    {
        u4Config |= 1<<5;
    }
    else
    {
        u4Config = 0;
    }

    vWriteShmUINT32(AUD_DSP0, u2SpkCfgIndx, (u4ReadShmUINT32(AUD_DSP0, u2SpkCfgIndx) & 0xffffffdf) | u4Config);
    DSP_SendDspTaskCmd(AUD_DSP0, u2SpkCfgUop);
}

BOOL _AUD_DspGetSpeakerSubwooferEnable(UINT8 u1DecIndex)
{
    UINT16 u2SpkCfgIndx;

    AUD_DEC_ID_VALIDATE_3(u1DecIndex);
    
    switch(u1DecIndex)
    {
    case AUD_DEC_AUX:
        u2SpkCfgIndx = D_SPKCFG_DEC2;
        break;
    case AUD_DEC_THIRD:
        u2SpkCfgIndx = D_SPKCFG_DEC3;
        break;
    case AUD_DEC_MAIN:
    default:
        u2SpkCfgIndx = D_SPKCFG;
        break;
    }
    if (u4ReadShmUINT32(AUD_DSP0, u2SpkCfgIndx) & (1<<5))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static INT16 u2DelayLR=0,u2DelayBypass=0,u2DelayDownmix=0;
void _AUD_DspChannelDelayValue(INT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex)
{
    if(eChIndex==AUD_CH_FRONT_LEFT)
    {
        u2DelayLR=u2Delay;
    }
    else if((eChIndex==AUD_CH_BYPASS_LEFT)||(eChIndex==AUD_CH_IEC_LEFT))
    {
        u2DelayBypass=u2Delay;
    }
    else if(eChIndex==AUD_CH_DMX_LEFT)
    {
        u2DelayDownmix=u2Delay;
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelDelay
 *  setup channel delay for individual channel.
 *
 *  @param  u2Delay 0.05 m/unit.
 *  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
static UINT16 u2DrvDelay=0;
void _AUD_DspChannelDelay(UINT8 u1DspId, UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex)
{
    UINT16 u2ShmIndex;
    UINT16 u2UopIndex;
    UINT16 u2DelayLRMix=0,u2DelayBypassMix=0,u2DelayDownmixMix=0;
    INT16  u2ExtraDelay=0;
    VDEC_HDR_INFO_T rHdrInfo;
    UINT32 u4Factor;
    UINT32 u4FrameSample;
    UINT32 u4RawDelayFrame;

    AUD_DSP_ID_VALIDATE(u1DspId);
#if 0//def CC_AUD_DDI
    if((_AudGetStrSource(AUD_DEC_MAIN) != AUD_STREAM_FROM_DIGITAL_TUNER) && 
        (_AudGetStrSource(AUD_DEC_MAIN) != AUD_STREAM_FROM_GST))
    {
        u2Delay = 0;
    }
#endif
    if ((eChIndex > AUD_CH_NUM) || (eChIndex == AUD_CH_INPUT_SRC))
    {
        return;
    }

    if(_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        if(VDEC_QueryInfo(0, &rHdrInfo) == FALSE)
        {
            rHdrInfo.eCodecType = VDEC_FMT_MPV;
        }
        
        u2ExtraDelay = AUD_GetAudioVdecDelay(rHdrInfo.eCodecType);
    }
    else
    {
        u2ExtraDelay = 0;
    }
#ifndef CC_AUD_DDI
        u2ExtraDelay = 0;
#endif
    if((INT16)(u2DelayLR+u2Delay+u2ExtraDelay)<0)
    {
        u2DelayLRMix=0;
    }
    else 
    {
        u2DelayLRMix=u2DelayLR+u2Delay+u2ExtraDelay;
    }
    if(u2DelayLRMix>=3400)
    {
        u2DelayLRMix=3400;//no more than 500ms..
    }
    if((INT16)(u2DelayBypass+u2Delay+u2ExtraDelay)<0)
    {
        u2DelayBypassMix=0;
    }
    else 
    {
        u2DelayBypassMix=u2DelayBypass+u2Delay+u2ExtraDelay;
    }
    if(u2DelayBypassMix>=3400)
    {
        u2DelayBypassMix=3400;//no more than 500ms..
    }
    if((INT16)(u2DelayDownmix+u2Delay+u2ExtraDelay)<0)
    {
        u2DelayDownmixMix=0;
    }
    else 
    {
        u2DelayDownmixMix=u2DelayDownmix+u2Delay+u2ExtraDelay;
    }
    if(u2DelayDownmixMix>=3400)
    {
        u2DelayDownmixMix=3400;//no more than 500ms..
    }

    if (eChIndex == AUD_CH_ALL)
    {       
        if(u2DrvDelay != u2Delay)
        {
            DRVCUST_SendAudEvent(E_CUST_AUD_DELAY_CHANGE,uDecIndex);
        }
        u2DrvDelay = u2Delay;
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        // u2Delay is 5cm unit. Need to change 5cm to 64bank unit
        // 64/48 = 1.3 ms
        // 5cm/34000cm = 0.14 ms
        // 1.3/0.14 = 9
        if ((_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_HDMI) && 
			(uReadShmUINT8(AUD_DSP0, B_IECFLAG) != 0) &&
			(_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_DTS))
        {
            vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SP), ((u2DelayLRMix/u4DelayConstant) + 14));  
            LOG(5, "### Adjust for HDMI PCM 14 bank for DTS\n");
        }
		else
		{
            vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SP), u2DelayLRMix/u4DelayConstant);   
		}
        _vAprocSetRoutine(APROC_ROUTINE_ID_DR_SP_PATH);
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_HP), u2DelayLRMix/u4DelayConstant);
        _vAprocSetRoutine(APROC_ROUTINE_ID_DR_HP_PATH);
        vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SPDIF_PCM), u2DelayLRMix/u4DelayConstant);
        _vAprocSetRoutine(APROC_ROUTINE_ID_DR_SPDIF_PCM_PATH);

        // Work Around for PlayMute, need a Aout Enable to let PlayMute know DSP delay is set and done.
        if (uDecIndex == AUD_DEC_AUX)
        {
            DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_AOUT_REINIT_DEC2);
        }
        else
        {
            DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_AOUT_REINIT);        
        }
        return;
#endif //defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        // Delay for all channels
        u2ShmIndex = W_CHDELAY_C;
        //if (AUD_GetTargetCountry() == COUNTRY_EU)
        if(_IsDualDecMode())
        {
#ifdef CC_ENABLE_AOMX
            if (_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_GST)
            {
                if ((u2ReadShmUINT16(u1DspId, W_CHDELAY_C) == u2Delay) &&
                    (u2ReadShmUINT16(u1DspId, W_CHDELAY_L) == u2DelayLRMix) &&
                    (u2ReadShmUINT16(u1DspId, W_CHDELAY_R) == u2DelayLRMix) &&
                    (u2ReadShmUINT16(u1DspId, W_CHDELAY_LS) == u2Delay) &&
                    (u2ReadShmUINT16(u1DspId, W_CHDELAY_RS) == u2Delay) &&
                    (u2ReadShmUINT16(u1DspId, W_CHDELAY_SUB) == u2Delay) &&
                    (u2ReadShmUINT16(u1DspId, W_CHDELAY_CH9) == u2DelayDownmixMix) &&
                    (u2ReadShmUINT16(u1DspId, W_CHDELAY_CH10) == u2DelayDownmixMix))
                {
                    AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);
                    return;
                }
            }
#endif

            vWriteShmUINT16(u1DspId, W_CHDELAY_C , u2Delay);
            vWriteShmUINT16(u1DspId, W_CHDELAY_L , u2DelayLRMix);
            vWriteShmUINT16(u1DspId, W_CHDELAY_R , u2DelayLRMix);
            vWriteShmUINT16(u1DspId, W_CHDELAY_LS , u2Delay);
            vWriteShmUINT16(u1DspId, W_CHDELAY_RS , u2Delay);
            vWriteShmUINT16(u1DspId, W_CHDELAY_SUB , u2Delay);
            vWriteShmUINT16(u1DspId, W_CHDELAY_CH9 , u2DelayDownmixMix);
            vWriteShmUINT16(u1DspId, W_CHDELAY_CH10 , u2DelayDownmixMix);
        }
        else
        {
#ifdef CC_ENABLE_AOMX
        BOOL fgChange = FALSE;
        if ((u2ReadShmUINT16(u1DspId, W_CHDELAY_C) != u2Delay) ||
            (u2ReadShmUINT16(u1DspId, W_CHDELAY_L) != u2DelayLRMix) ||
            (u2ReadShmUINT16(u1DspId, W_CHDELAY_R) != u2DelayLRMix) ||
            (u2ReadShmUINT16(u1DspId, W_CHDELAY_LS) != u2Delay) ||
            (u2ReadShmUINT16(u1DspId, W_CHDELAY_RS) != u2Delay) ||
            (u2ReadShmUINT16(u1DspId, W_CHDELAY_CH7) != u2DelayBypassMix) ||
            (u2ReadShmUINT16(u1DspId, W_CHDELAY_CH8) != u2DelayBypassMix) ||
            (u2ReadShmUINT16(u1DspId, W_CHDELAY_SUB) != u2Delay) ||
            (u2ReadShmUINT16(u1DspId, W_CHDELAY_CH9) != u2DelayDownmixMix) ||
            (u2ReadShmUINT16(u1DspId, W_CHDELAY_CH10) != u2DelayDownmixMix))
            {
                fgChange = TRUE;
            }
#endif
        vWriteShmUINT16(u1DspId, W_CHDELAY_C , u2Delay);
        vWriteShmUINT16(u1DspId, W_CHDELAY_L , u2DelayLRMix);
        vWriteShmUINT16(u1DspId, W_CHDELAY_R , u2DelayLRMix);
        vWriteShmUINT16(u1DspId, W_CHDELAY_LS , u2Delay);
        vWriteShmUINT16(u1DspId, W_CHDELAY_RS , u2Delay);
        vWriteShmUINT16(u1DspId, W_CHDELAY_CH7 , u2DelayBypassMix);
        vWriteShmUINT16(u1DspId, W_CHDELAY_CH8 , u2DelayBypassMix);
        vWriteShmUINT16(u1DspId, W_CHDELAY_SUB , u2Delay);
        vWriteShmUINT16(u1DspId, W_CHDELAY_CH9 , u2DelayDownmixMix);
        vWriteShmUINT16(u1DspId, W_CHDELAY_CH10 , u2DelayDownmixMix);

#ifdef CC_ENABLE_AOMX
            if (_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_GST)
            {
                if (!fgChange)
                {
                    AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);
                    return;
                }
            }
#endif
        }
        u2UopIndex=UOP_DSP_CONFIG_DELAY_ALL;
        DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
        LOG(3,"$$$$$$ Enter _AUD_DspChannelDelay #######\n");
    }
    else
    {
        // Delay for individual channel
        switch (eChIndex)
        {
        case AUD_CH_FRONT_LEFT:
            u2ShmIndex = W_CHDELAY_L;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_L;
            if (u2ReadShmUINT16(u1DspId, u2ShmIndex) == u2DelayLRMix)
            {
                return;
            }
            else
            {
                DRVCUST_SendAudEvent(E_CUST_AUD_DELAY_CHANGE,uDecIndex);
                vWriteShmUINT16(u1DspId, u2ShmIndex, u2DelayLRMix);
                DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
            }
#if 0//defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
            vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SP), u2Delay/u4DelayConstant);
            _vAprocSetRoutine(APROC_ROUTINE_ID_DR_SP_PATH);      
#endif //defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)               
            break;
        case AUD_CH_FRONT_RIGHT:
            u2ShmIndex = W_CHDELAY_R;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_R;
            if (u2ReadShmUINT16(u1DspId, u2ShmIndex) == u2DelayLRMix)
            {
                return;
            }
            else
            {
                vWriteShmUINT16(u1DspId, u2ShmIndex, u2DelayLRMix);
                DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
            }
            break;
        case AUD_CH_REAR_LEFT:
            u2ShmIndex = W_CHDELAY_LS;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_LS;
            break;
        case AUD_CH_REAR_RIGHT:
            u2ShmIndex = W_CHDELAY_RS;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_RS;
            break;
        case AUD_CH_CENTER:
            u2ShmIndex = W_CHDELAY_C;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_C;
            break;
        case AUD_CH_SUB_WOOFER:
        case AUD_CH_LFE:
            u2ShmIndex = W_CHDELAY_SUB;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_SUBWOOFER;
            break;
        case AUD_CH_BYPASS_LEFT:
            u2ShmIndex = W_CHDELAY_CH7;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH7;
            if (u2ReadShmUINT16(u1DspId, u2ShmIndex) == u2DelayBypassMix)
            {
                return;
            }
            else
            {
                vWriteShmUINT16(u1DspId, u2ShmIndex, u2DelayBypassMix);
                DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
            }
#if 0//defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
            vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SPDIF_PCM), u2Delay/u4DelayConstant);
            _vAprocSetRoutine(APROC_ROUTINE_ID_DR_SPDIF_PCM_PATH);
#ifdef CC_AUD_DDI
            switch (AUD_GetSampleFreq(_u1SpdifRawDec))
            {
            case FS_192K:
                u4Factor = 192;
                break;
            case FS_96K:
                u4Factor = 96;
                break;
            case FS_48K:
                u4Factor = 48;
                break;
            case FS_44K:
                u4Factor = 44;
                break;
            case FS_32K:
                u4Factor = 32;
                break;
            default:
                u4Factor = 48; 
                break;
            }
            
            switch (u2ReadShmUINT16(AUD_DSP0, W_IEC_BURST_INFO))
            {
            case BURST_INFO_AC3:
            case BURST_INFO_DTS:
            case BURST_INFO_AAC:
                if ((u4FrameSample = u2ReadShmUINT16(AUD_DSP0, W_IEC_NSNUM)) != 0)
                {
                    //u4RawDelayFrame = (u2DelayBypassMix * 5 * u4Factor / 34 + u4FrameSample/2)/u4FrameSample;
                    u4RawDelayFrame = (u2DelayBypassMix * 5 * u4Factor / 17 + u4FrameSample)/(2*u4FrameSample);
					if ((_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_HDMI) &&
						(uReadShmUINT8(AUD_DSP0, B_IECFLAG) != 0) &&
						(_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_DTS))
					{
					    u4RawDelayFrame = u4RawDelayFrame*2;
					}
                    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_RAWDELAY, (UINT32 *) &u4RawDelayFrame, 1); 
                }
                break;
            default:
                break;
            }
#endif
#endif //defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)            
            break;
        case AUD_CH_BYPASS_RIGHT:
        case AUD_CH_IEC_LEFT:
            u2ShmIndex = W_CHDELAY_CH8;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH8;
            if (u2ReadShmUINT16(u1DspId, u2ShmIndex) == u2DelayBypassMix)
            {
                return;
            }
            else
            {
                vWriteShmUINT16(u1DspId, u2ShmIndex, u2DelayBypassMix);
                DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
            }
            break;
        case AUD_CH_DMX_LEFT:
            u2ShmIndex = W_CHDELAY_CH9;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH9;
            if (u2ReadShmUINT16(u1DspId, u2ShmIndex) == u2DelayDownmixMix)
            {
                return;
            }
            else
            {
                vWriteShmUINT16(u1DspId, u2ShmIndex, u2DelayDownmixMix);
                DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
            }
#if 0//defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
            vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_3, APROC_REG_DELAY_HP), u2Delay/u4DelayConstant);
            _vAprocSetRoutine(APROC_ROUTINE_ID_DR_HP_PATH);      
#endif //defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)            
            break;
        case AUD_CH_DMX_RIGHT:
            u2ShmIndex = W_CHDELAY_CH10;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH10;
            if (u2ReadShmUINT16(u1DspId, u2ShmIndex) == u2DelayDownmixMix)
            {
                return;
            }
            else
            {
                vWriteShmUINT16(u1DspId, u2ShmIndex, u2DelayDownmixMix);
                DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
            }
            break;
        case AUD_CH_AUX_FRONT_LEFT:
            u2ShmIndex = W_CHDELAY_L_DEC2;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_L_DEC2;
            break;
        case AUD_CH_AUX_FRONT_RIGHT:
            u2ShmIndex = W_CHDELAY_R_DEC2;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_R_DEC2;
            break;
        case AUD_CH_THIRD:
            u2ShmIndex = W_CHDELAY_DEC3;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_DEC3;
            break;
        default:
            u2ShmIndex = W_CHDELAY_C;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_C;
            break;
        }
        if ((eChIndex != AUD_CH_FRONT_LEFT) && (eChIndex != AUD_CH_FRONT_RIGHT) &&
            (eChIndex != AUD_CH_BYPASS_LEFT) && (eChIndex != AUD_CH_BYPASS_RIGHT) &&
            (eChIndex != AUD_CH_DMX_LEFT) && (eChIndex != AUD_CH_DMX_RIGHT))
        {
            if (u2ReadShmUINT16(u1DspId, u2ShmIndex) == u2Delay)
            {
                return;
            }
            else
            {
                vWriteShmUINT16(u1DspId, u2ShmIndex, u2Delay);
                DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
            }
        }
    }
}
void _AUD_DspChannelDelayAP(AUD_CH_T eChIndex, UINT8 uDecIndex)
{
    UINT16 u2ShmIndex;
    UINT16 u2UopIndex;
    UINT16 u2DelayLRMix=0,u2DelayBypassMix=0,u2DelayDownmixMix=0;
    INT16  u2ExtraDelay=0;
    VDEC_HDR_INFO_T rHdrInfo;
    UINT32 u4Factor;
    UINT32 u4FrameSample;
    UINT32 u4RawDelayFrame;
   
    UNUSED(u2ShmIndex);
    if(_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        if(VDEC_QueryInfo(0, &rHdrInfo) == FALSE)
        {
            rHdrInfo.eCodecType = VDEC_FMT_MPV;
        }
        
        u2ExtraDelay = AUD_GetAudioVdecDelay(rHdrInfo.eCodecType);
    }
    else
    {
        u2ExtraDelay = 0;
    }
#ifdef CC_AUD_DDI   
    if((_AudGetStrSource(AUD_DEC_MAIN) != AUD_STREAM_FROM_DIGITAL_TUNER) && 
        (_AudGetStrSource(AUD_DEC_MAIN) != AUD_STREAM_FROM_GST))
    {
        //u2DrvDelay = 0;
    }
#else
    u2ExtraDelay = 0;
#endif
    if((INT16)(u2DelayLR+u2DrvDelay+u2ExtraDelay)<0)
    {
        u2DelayLRMix=0;
    }
    else 
    {
        u2DelayLRMix=u2DelayLR+u2DrvDelay+u2ExtraDelay;
    }
    if(u2DelayLRMix>=3400)
    {
        u2DelayLRMix=3400;//no more than 500ms..
    }
    if((INT16)(u2DelayBypass+u2DrvDelay+u2ExtraDelay)<0)
    {
        u2DelayBypassMix=0;
    }
    else 
    {
        u2DelayBypassMix=u2DelayBypass+u2DrvDelay+u2ExtraDelay;
    }
    if(u2DelayBypassMix>=3400)
    {
        u2DelayBypassMix=3400;//no more than 500ms..
    }
    if((INT16)(u2DelayDownmix+u2DrvDelay+u2ExtraDelay)<0)
    {
        u2DelayDownmixMix=0;
    }
    else 
    {
        u2DelayDownmixMix=u2DelayDownmix+u2DrvDelay+u2ExtraDelay;
    }
    if(u2DelayDownmixMix>=3400)
    {
        u2DelayDownmixMix=3400;//no more than 500ms..
    }
    switch (eChIndex)
    {
    case AUD_CH_FRONT_LEFT:
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        if (u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SP)) != u2DelayLRMix/u4DelayConstant)
        {
            x_thread_delay(30);
			if ((_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_HDMI) && 
				(uReadShmUINT8(AUD_DSP0, B_IECFLAG) != 0) &&
				(_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_DTS))
            {
            if(u2DelayLRMix > 0x800)
            {
                vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SP), u2DelayLRMix/u4DelayConstant);  
            }
			else if(u2DelayLRMix > 0x700)
			{
				vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SP), ((u2DelayLRMix/u4DelayConstant)+7));  
			}
            else
            {
                vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SP), ((u2DelayLRMix/u4DelayConstant)+14));
            }
            }
            else
            {
            vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SP), u2DelayLRMix/u4DelayConstant);       
            }
            _vAprocSetRoutine(APROC_ROUTINE_ID_DR_SP_PATH);
        }
#else
        u2ShmIndex = W_CHDELAY_L;
        u2UopIndex = UOP_DSP_CONFIG_DELAY_L;
        if (u2ReadShmUINT16(AUD_DSP0, u2ShmIndex) == u2DelayLRMix)
        {
            return;
        }
        else
        {
            vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2DelayLRMix);
            DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
        }
        break;
    case AUD_CH_FRONT_RIGHT:
        u2ShmIndex = W_CHDELAY_R;
        u2UopIndex = UOP_DSP_CONFIG_DELAY_R;
        if (u2ReadShmUINT16(AUD_DSP0, u2ShmIndex) == u2DelayLRMix)
        {
            return;
        }
        else
        {
            vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2DelayLRMix);
            DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
        }
#endif 
        break;
    case AUD_CH_BYPASS_LEFT:
    case AUD_CH_IEC_LEFT:
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        if (u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SPDIF_PCM)) != u2DelayBypassMix/u4DelayConstant)
        {
            x_thread_delay(30);
            vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_SPDIF_PCM), u2DelayBypassMix/u4DelayConstant);
            _vAprocSetRoutine(APROC_ROUTINE_ID_DR_SPDIF_PCM_PATH);
#ifdef CC_AUD_DDI
            switch (AUD_GetSampleFreq(_u1SpdifRawDec))
            {
            case FS_192K:
                u4Factor = 192;
                break;
            case FS_96K:
                u4Factor = 96;
                break;
            case FS_48K:
                u4Factor = 48;
                break;
            case FS_44K:
                u4Factor = 44;
                break;
            case FS_32K:
                u4Factor = 32;
                break;
            default:
                u4Factor = 48; 
                break;
            }
            
            switch (u2ReadShmUINT16(AUD_DSP0, W_IEC_BURST_INFO))
            {
            case BURST_INFO_AC3:
            case BURST_INFO_DTS:
            case BURST_INFO_AAC:
                if ((u4FrameSample = u2ReadShmUINT16(AUD_DSP0, W_IEC_NSNUM)) != 0)
                {
                    //u4RawDelayFrame = (u2DelayBypassMix * 5 * u4Factor / 34 + u4FrameSample/2)/u4FrameSample;
                    u4RawDelayFrame = (u2DelayBypassMix * 5 * u4Factor / 17 + u4FrameSample)/(2*u4FrameSample);
					if ((_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_HDMI) &&
						(uReadShmUINT8(AUD_DSP0, B_IECFLAG) != 0) &&
						(_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_DTS))
					{
					    u4RawDelayFrame = u4RawDelayFrame*2;
					}
                    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_RAWDELAY, (UINT32 *) &u4RawDelayFrame, 1); 
                }
                break;
            default:
                break;
            }
#endif
        }
#else
        u2ShmIndex = W_CHDELAY_CH7;
        u2UopIndex = UOP_DSP_CONFIG_DELAY_CH7;
        if (u2ReadShmUINT16(AUD_DSP0, u2ShmIndex) == u2DelayBypassMix)
        {
            return;
        }
        else
        {
            vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2DelayBypassMix);
            DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
        }      
        break;
    case AUD_CH_BYPASS_RIGHT:
        u2ShmIndex = W_CHDELAY_CH8;
        u2UopIndex = UOP_DSP_CONFIG_DELAY_CH8;
        if (u2ReadShmUINT16(AUD_DSP0, u2ShmIndex) == u2DelayBypassMix)
        {
            return;
        }
        else
        {
            vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2DelayBypassMix);
            DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
        }
#endif 
        break;
    case AUD_CH_DMX_LEFT:
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        if (u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_HP)) != u2DelayDownmixMix/u4DelayConstant)
        {
            x_thread_delay(30);
            vAprocReg_Write (APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_4, APROC_REG_DELAY_HP), u2DelayDownmixMix/u4DelayConstant);
            _vAprocSetRoutine(APROC_ROUTINE_ID_DR_HP_PATH);
        }
#else
        u2ShmIndex = W_CHDELAY_CH9;
        u2UopIndex = UOP_DSP_CONFIG_DELAY_CH9;
        if (u2ReadShmUINT16(AUD_DSP0, u2ShmIndex) == u2DelayDownmixMix)
        {
            return;
        }
        else
        {
            vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2DelayDownmixMix);
            DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
        }  
        break;
    case AUD_CH_DMX_RIGHT:
        u2ShmIndex = W_CHDELAY_CH10;
        u2UopIndex = UOP_DSP_CONFIG_DELAY_CH10;
        if (u2ReadShmUINT16(AUD_DSP0, u2ShmIndex) == u2DelayDownmixMix)
        {
            return;
        }
        else
        {
            vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2DelayDownmixMix);
            DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
        }
#endif
        break;
    case AUD_CH_ALL:
        u2UopIndex=UOP_DSP_CONFIG_DELAY_ALL;
        DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
    default:
        break;
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelDelayNoUop
 *  setup channel delay for individual channel.
 *
 *  @param  u2Delay 0.05 m/unit.
 *  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelDelayNoUop(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex)
{
    UINT16 u2ShmIndex;
    UINT8  uIdx;

    if ((eChIndex > AUD_CH_NUM) || (eChIndex == AUD_CH_INPUT_SRC))
    {
        return;
    }

    if (eChIndex == AUD_CH_ALL)
    {
        // Delay for all channels
        u2ShmIndex = W_CHDELAY_C;
        if((GetTargetCountry() == COUNTRY_EU) || (GetTargetCountry() == COUNTRY_JP))
        {
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_C , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_L , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_R , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_LS , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_RS , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_SUB , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_CH9 , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_CH10 , u2Delay);
        }
        else
        {
            for (uIdx=0; uIdx<=AUD_CH_DMX_RIGHT; uIdx++)
            {
                vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Delay);
                u2ShmIndex += 2;
            }
        }
        LOG(3,"$$$$$$ Enter _AUD_DspChannelDelayNoUop #######\n");
    }
    else
    {
        // Delay for individual channel
        switch (eChIndex)
        {
        case AUD_CH_FRONT_LEFT:
            u2ShmIndex = W_CHDELAY_L;
            break;
        case AUD_CH_FRONT_RIGHT:
            u2ShmIndex = W_CHDELAY_R;
            break;
        case AUD_CH_REAR_LEFT:
            u2ShmIndex = W_CHDELAY_LS;
            break;
        case AUD_CH_REAR_RIGHT:
            u2ShmIndex = W_CHDELAY_RS;
            break;
        case AUD_CH_CENTER:
            u2ShmIndex = W_CHDELAY_C;
            break;
        case AUD_CH_SUB_WOOFER:
        case AUD_CH_LFE:
            u2ShmIndex = W_CHDELAY_SUB;
            break;
        case AUD_CH_BYPASS_LEFT:
            u2ShmIndex = W_CHDELAY_CH7;
            break;
        case AUD_CH_BYPASS_RIGHT:
            u2ShmIndex = W_CHDELAY_CH8;
            break;
        case AUD_CH_DMX_LEFT:
            u2ShmIndex = W_CHDELAY_CH9;
            break;
        case AUD_CH_DMX_RIGHT:
            u2ShmIndex = W_CHDELAY_CH10;
            break;
        case AUD_CH_AUX_FRONT_LEFT:
            u2ShmIndex = W_CHDELAY_L_DEC2;
            break;
        case AUD_CH_AUX_FRONT_RIGHT:
            u2ShmIndex = W_CHDELAY_R_DEC2;
            break;
        default:
            u2ShmIndex = W_CHDELAY_C;
            break;
        }

        if (u2ReadShmUINT16(AUD_DSP0, u2ShmIndex) == u2Delay)
        {
            return;
        }
        else
        {
            vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Delay);
        }
    }
}

UINT16 _AUD_DspGetChannelDelay(UINT8 u1DspId, AUD_CH_T eChIndex)
{
    UINT16 u2ShmIndex;

    // Delay for individual channel
    switch (eChIndex)
    {
    case AUD_CH_FRONT_LEFT:
        u2ShmIndex = W_CHDELAY_L;
        break;
    case AUD_CH_FRONT_RIGHT:
        u2ShmIndex = W_CHDELAY_R;
        break;
    case AUD_CH_REAR_LEFT:
        u2ShmIndex = W_CHDELAY_LS;
        break;
    case AUD_CH_REAR_RIGHT:
        u2ShmIndex = W_CHDELAY_RS;
        break;
    case AUD_CH_CENTER:
        u2ShmIndex = W_CHDELAY_C;
        break;
    case AUD_CH_SUB_WOOFER:
    case AUD_CH_LFE:
        u2ShmIndex = W_CHDELAY_SUB;
        break;
    case AUD_CH_BYPASS_LEFT:
        u2ShmIndex = W_CHDELAY_CH7;
        break;
    case AUD_CH_BYPASS_RIGHT:
        u2ShmIndex = W_CHDELAY_CH8;
        break;
    case AUD_CH_DMX_LEFT:
        u2ShmIndex = W_CHDELAY_CH9;
        break;
    case AUD_CH_DMX_RIGHT:
        u2ShmIndex = W_CHDELAY_CH10;
        break;
    case AUD_CH_AUX_FRONT_LEFT:
        u2ShmIndex = W_CHDELAY_L_DEC2;
        break;
    case AUD_CH_AUX_FRONT_RIGHT:
        u2ShmIndex = W_CHDELAY_R_DEC2;
        break;
    default:
        u2ShmIndex = W_CHDELAY_C;
        break;
    }

    return u2ReadShmUINT16(u1DspId, u2ShmIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspChannelDelay_initial
 *  setup channel delay for individual channel.
 *
 *  @param  u2Delay 0.05 m/unit.
 *  @param  eChIndex 0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)
 *                   8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)
 *  @param  u1DecIndex 0: first decoder 1: seconder decoder.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelDelay_initial(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex)
{
    UINT16 u2ShmIndex;
    UINT16 u2UopIndex;
    UINT8  uIdx;

    if ((eChIndex > AUD_CH_NUM) || (eChIndex == AUD_CH_INPUT_SRC))
    {
        return;
    }

    if (eChIndex == AUD_CH_ALL)
    {
        // Delay for all channels
        u2ShmIndex = W_CHDELAY_C;
        //if (AUD_GetTargetCountry() == COUNTRY_EU)
        if(_IsDualDecMode())
        {
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_C , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_L , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_R , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_LS , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_RS , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_SUB , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_CH9 , u2Delay);
            vWriteShmUINT16(AUD_DSP0,  W_CHDELAY_CH10 , u2Delay);
        }
        else
        {
            for (uIdx=0; uIdx<=AUD_CH_DMX_RIGHT; uIdx++)
            {
                vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Delay);
                u2ShmIndex += 2;
            }
        }
        //u2UopIndex=UOP_DSP_CONFIG_DELAY_ALL;
        //DSP_SendDspTaskCmd(u2UopIndex);
        LOG(3,"$$$$$$ Enter _AUD_DspChannelDelay INITIAL. This will not send uop to DSP #######\n");
    }
    else
    {
        // Delay for individual channel
        switch (eChIndex)
        {
        case AUD_CH_FRONT_LEFT:
            u2ShmIndex = W_CHDELAY_L;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_L;
            break;
        case AUD_CH_FRONT_RIGHT:
            u2ShmIndex = W_CHDELAY_R;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_R;
            break;
        case AUD_CH_REAR_LEFT:
            u2ShmIndex = W_CHDELAY_LS;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_LS;
            break;
        case AUD_CH_REAR_RIGHT:
            u2ShmIndex = W_CHDELAY_RS;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_RS;
            break;
        case AUD_CH_CENTER:
            u2ShmIndex = W_CHDELAY_C;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_C;
            break;
        case AUD_CH_SUB_WOOFER:
        case AUD_CH_LFE:
            u2ShmIndex = W_CHDELAY_SUB;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_SUBWOOFER;
            break;
        case AUD_CH_BYPASS_LEFT:
            u2ShmIndex = W_CHDELAY_CH7;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH7;
            break;
        case AUD_CH_BYPASS_RIGHT:
            u2ShmIndex = W_CHDELAY_CH8;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH8;
            break;
        case AUD_CH_DMX_LEFT:
            u2ShmIndex = W_CHDELAY_CH9;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH9;
            break;
        case AUD_CH_DMX_RIGHT:
            u2ShmIndex = W_CHDELAY_CH10;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_CH10;
            break;
        case AUD_CH_AUX_FRONT_LEFT:
            u2ShmIndex = W_CHDELAY_L_DEC2;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_L_DEC2;
            break;
        case AUD_CH_AUX_FRONT_RIGHT:
            u2ShmIndex = W_CHDELAY_R_DEC2;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_R_DEC2;
            break;
        default:
            u2ShmIndex = W_CHDELAY_C;
            u2UopIndex = UOP_DSP_CONFIG_DELAY_C;
            break;
        }

        if (u2ReadShmUINT16(AUD_DSP0, u2ShmIndex) == u2Delay)
        {
            return;
        }
        else
        {
            vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Delay);
            DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
        }
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolume
 *  Get channel volume of individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetChannelVolume(UINT8 u1DecId, AUD_CH_T eChannel)
{
    // Check input, for lint
    if (u1DecId >= AUD_DEC_NUM)
    {
        u1DecId = 0;
    }

    return _aau1ChannelVolume[u1DecId][eChannel];
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelDelay
 *  setup channel delay by input source.
 *
 *  @param  eStreamFrom
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspInputChannelDelay(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    _AUD_DspChannelDelay(AUD_DSP0, _au1ChDelay[(UINT8)eStreamFrom], AUD_CH_ALL, AUD_DEC_MAIN);
}

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolume
 *  setup channel volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelVolume(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value)
{
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif


    VOL_CTL_SEMA_LOCK(u1DecId);
    _fgChannelVolUseShmRawData = FALSE;
    _aau1ChannelVolume[u1DecId][eChannel] = u1Value;
    _AUD_DspVolumeChange(u1DspId, u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

void _AUD_DspChannelVolumeNew(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value, UINT8 u1Value1)
{
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    LOG(9,"CMD: set Channel Volume: Dec(%d) CH(%d) u1Value(%d) u1Value(%d)\n",
        u1DecId, (UINT8)eChannel, u1Value, u1Value1);

    VOL_CTL_SEMA_LOCK(u1DecId);
    //_fgChannelVolUseShmRawData = FALSE;
    //_aau1ChannelVolume[u1DecId][eChannel] = u1Value;
#ifdef CC_AUD_DDI
    _aau1ChannelVolumeMain[u1DecId][eChannel] =u1Value;
    _aau1ChannelVolumeFine[u1DecId][eChannel] =u1Value1;
#endif
    _AudDspChannelVolChangeNew(AUD_DSP0, u1DecId, eChannel, u1Value, u1Value1);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

#ifdef CC_AUD_DDI
void _AUD_DspBluetoothVolume(UINT8 u1DecId, UINT8 u1Value, UINT8 u1Value1)
{
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    LOG(2,"CMD: set Channel Volume: Dec(%d)  u1Value(%d) u1Value(%d)\n",
        u1DecId, u1Value, u1Value1);

    _AudDspBluetoothVolChange(u1DecId, u1Value, u1Value1);
}   
#endif

//-----------------------------------------------------------------------------
/** AUD_DspPanFadePesUpdateEn
 *  setup ad pan fade update by pes enable
 *
 *  @param  bEnable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspPanFadePesUpdateEn(BOOL bEnable)
{
    AUD_PanFadePesUpdateEn(bEnable);
}

//-----------------------------------------------------------------------------
/** _AUD_DspAdPanFadeEnable
 *  setup ad pan volume for individual channel.
 *
 *  @param  bEnable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAdPanFadeEnable(BOOL bEnable)
{
    UINT32 u2UopIndex = UOP_DSP_PANFADE_DSP_ENABLE;
    UINT16 u4Config = 0;

    AUD_SetAdFadeEnable(bEnable);
    AUD_SetAdPanEnable(bEnable);

    if (bEnable == TRUE)
    {
        if (_u2AdFadeMode == 0)
        {
            _u2AdFadeMode = 1<<9;   // If eAudioPanFadeControl is 0, but AP turn on AD PAN FADE (default mode 1)
        }
        u4Config |= _u2AdFadeMode;  // Enable Fade & Pan
    }
    else
    {
        _u2AdFadeMode = (u2ReadShmUINT16(AUD_DSP0, W_SYSTEM_SETUP) & 0x0600);
        u4Config |= 0x0;  // Disable Fade & Pan
    }

    vWriteShmUINT16(AUD_DSP0, W_SYSTEM_SETUP, (u2ReadShmUINT16(AUD_DSP0, W_SYSTEM_SETUP) & 0xF9FF) | u4Config);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}

// 00: Pan disable, Fade disable
// 01: Pan enable, Fade CH7/8,CH9/10,CHL/R
// 10: Pan enable, Fade CH9/10, CHL/R
// 11: Pan enable, Fade CHL/R
UINT8 _AUD_DspGetAdPanFadeSetting(void)
{
    return ((u2ReadShmUINT16(AUD_DSP0, W_SYSTEM_SETUP)&0x0600)>>9);
}

//-----------------------------------------------------------------------------
/** AUD_DspAdFadeVolume
 *  setup ad fade volume for individual channel.
 *
 *  @param  u1Value 0~255.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAdFadeVolumeSet(UINT8 u1Value)
{
    _u1AdFadeVolume = u1Value;
}

//-----------------------------------------------------------------------------
/** _AUD_DspAdFadeVolumeUpdate
 *  setup ad fade volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAdFadeVolumeUpdate(UINT8 u1DecId, AUD_CH_T eChannel)
{
    UINT16 u4AdFadeIndx = D_FADEVALUE;
    UINT16 u2AdFadeUop = UOP_DSP_AD_FADE;

    AUD_DEC_ID_VALIDATE(u1DecId);

    if (_u1AdFadeVolumePrev != _u1AdFadeVolume)
    {
        _u1AdFadeVolumePrev = _u1AdFadeVolume;
        VOL_CTL_SEMA_LOCK(u1DecId);
#ifdef CC_AUD_ARM_SUPPORT
#ifdef CC_AUD_ARM_RENDER
        // When Pan/Fade disabled, the volume will be reset to 0(In SendAudioPes)
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_COMM_2, APROC_REG_AMIXER2_ADMIX_FADE), _u1AdFadeVolume);
        LOG(0, "AD Fade Vol Update --> FadeVol = 0x%x\n", _u1AdFadeVolume);
#endif
#endif
        _fgChannelVolUseShmRawData = FALSE;
        //_AUD_DspVolumeChange(u1DecId, eChannel);
        vWriteShmUINT32(AUD_DSP0, u4AdFadeIndx, _DbToShmFade(_u1AdFadeVolume));
        DSP_SendDspTaskCmd(AUD_DSP0, u2AdFadeUop);
        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspAdPanVolumeSet
 *  setup ad pan volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u1Value -21~21.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAdPanVolumeSet(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_ADPAN_CHANNEL_VALIDATE(eChannel);

    _aau1AdPanVol[eChannel] = u1Value;
}

//-----------------------------------------------------------------------------
/** _AUD_DspAdPanVolumeUpdate
 *  setup ad pan volume for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAdPanVolumeUpdate(UINT8 u1DecId, AUD_CH_T eChannel)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_ADPAN_CHANNEL_VALIDATE(eChannel);

    if (_aau1AdPanVolPrev[eChannel] != _aau1AdPanVol[eChannel])
    {
        _aau1AdPanVolPrev[eChannel] = _aau1AdPanVol[eChannel];
        VOL_CTL_SEMA_LOCK(u1DecId);
        _fgChannelVolUseShmRawData = FALSE;
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        // When Pan/Fade disabled, the volume will be reset to 0(In SendAudioPes)
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_COMM_2, APROC_REG_AMIXER2_ADMIX_PAN), _aau1AdPanVol[eChannel]);
        LOG(0, "AD Pan Vol Update --> PanVol(%d) = 0x%x\n", eChannel, _aau1AdPanVol[eChannel]);
#else
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, eChannel);
#endif
        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }
}

#ifdef CC_AUD_VOLUME_OFFSET
//-----------------------------------------------------------------------------
/** _AUD_DspVolumeOffset
 *  set volume offset to master volume.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Index  : offset map index. 
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspVolumeOffset(UINT8 u1DecId, UINT8 u1Index)
{
    LOG(5,"CMD: set VolumeOffset: Dec(%d) Index(%d)\n", u1DecId, u1Index);

    AUD_DEC_ID_VALIDATE(u1DecId);    

    if (u1Index > (AUD_VOLUME_OFFSET_IDX_NUM-1))
    {
        u1Index = AUD_VOLUME_OFFSET_IDX_NUM - 1 ;
    }

    VOL_CTL_SEMA_LOCK(u1DecId);
    
    // Volume offset affects both speaker and HP
    _aai1ChannelVolOffset[u1DecId][AUD_CH_ALL] = (INT8)_ai4VolOffsetMap[u1Index];
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_ALL);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_RIGHT);
    // Also apply to Line out, since Line out maybe varibable mode.
    // 1. Modify here, apply volume offset to Ls/Rs and call AUD_DspVolumeChange.
    // 2. However, in _AUD_DspVolumeChange, should consider if line out is variable or fixed mode.
    //    If line out is varable mode, apply the offset.
    //_aai1ChannelVolOffset[u1DecId][AUD_CH_REAR_LEFT] = (INT8)_ai4VolOffsetMap[u1Index];
    //_aai1ChannelVolOffset[u1DecId][AUD_CH_REAR_RIGHT] = (INT8)_ai4VolOffsetMap[u1Index];
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_REAR_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_REAR_RIGHT);    

#ifdef CC_S_SUBWOFFER_SUPPORT
    ///_aai1ChannelVolOffset[u1DecId][AUD_CH_CENTER] = (INT8)_ai4VolOffsetMap[u1Index];
    //_aai1ChannelVolOffset[u1DecId][AUD_CH_SUB_WOOFER] = (INT8)_ai4VolOffsetMap[u1Index];
    ///_AUD_DspVolumeChange(u1DecId, AUD_CH_CENTER);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_SUB_WOOFER);
#endif

    // Also apply to third decoder for audio description
#ifdef CC_MT5391_AUD_3_DECODER
    if (u1DecId == AUD_DEC_MAIN)
    {
        _aai1ChannelVolOffset[AUD_DEC_THIRD][AUD_CH_ALL] = (INT8)_ai4VolOffsetMap[u1Index];
        _AUD_DspVolumeChange(AUD_DSP0, AUD_DEC_THIRD, AUD_CH_ALL);
    }
#endif
    VOL_CTL_SEMA_UNLOCK(u1DecId);    
}


//-----------------------------------------------------------------------------
/** _AUD_DspHpVolumeOffset
 *  set HP volume offset.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Index  : offset map index. 
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspHpVolumeOffset(UINT8 u1DecId, UINT8 u1Index)
{
    LOG(5,"CMD: set HpVolumeOffset: Dec(%d) Index(%d)\n", u1DecId, u1Index);

    AUD_DEC_ID_VALIDATE(u1DecId);

    if (u1Index > (AUD_VOLUME_OFFSET_IDX_NUM-1))
    {
        u1Index = AUD_VOLUME_OFFSET_IDX_NUM - 1 ;
    }

    VOL_CTL_SEMA_LOCK(u1DecId);
    
    _aai1ChannelVolOffset[u1DecId][AUD_CH_DMX_LEFT] = (INT8)_ai4VolOffsetMap[u1Index];
    _aai1ChannelVolOffset[u1DecId][AUD_CH_DMX_RIGHT] = (INT8)_ai4VolOffsetMap[u1Index];
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_RIGHT);
    
    VOL_CTL_SEMA_UNLOCK(u1DecId);    
}

//-----------------------------------------------------------------------------
/** _AUD_DspLineOutVolOffset
 *  setup bypass channel volume offset
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Index  : offset map index. 
 *  @retval void
 */
//-----------------------------------------------------------------------------

void _AUD_DspLineOutVolOffset(UINT8 u1DecId, UINT8 u1Index)
{
    LOG(5,"CMD: set LineOutVolumeOffset: Dec(%d) Index(%d)\n", u1DecId, u1Index);

    AUD_DEC_ID_VALIDATE(u1DecId);

    if (u1Index > (AUD_VOLUME_OFFSET_IDX_NUM-1))
    {
        u1Index = AUD_VOLUME_OFFSET_IDX_NUM - 1 ;
    }    

    VOL_CTL_SEMA_LOCK(u1DecId);
    _aai1ChannelVolOffset[u1DecId][AUD_CH_BYPASS_LEFT] = (INT8)_ai4VolOffsetMap[u1Index];
    _aai1ChannelVolOffset[u1DecId][AUD_CH_BYPASS_RIGHT] = (INT8)_ai4VolOffsetMap[u1Index];
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_BYPASS_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_BYPASS_RIGHT);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

#ifdef CC_S_SUBWOFFER_SUPPORT
//-----------------------------------------------------------------------------
/** _AUD_DspSWVolumeOffset
 *  setup Sub Woofer channel volume offset
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Index  : offset map index. 
 *  @retval void
 */
//-----------------------------------------------------------------------------

void _AUD_DspSWVolumeOffset(UINT8 u1DecId, UINT8 u1Index)
{
    LOG(5,"CMD: set SubWooferVolumeOffset: Dec(%d) Index(%d)\n", u1DecId, u1Index);

    AUD_DEC_ID_VALIDATE(u1DecId);

    if (u1Index > (AUD_SW_VOLUME_OFFSET_IDX_NUM-1))
    {
        u1Index = AUD_SW_VOLUME_OFFSET_IDX_NUM - 1 ;
    }    
    
    VOL_CTL_SEMA_LOCK(u1DecId);
    ///_aai1ChannelVolOffset[u1DecId][AUD_CH_CENTER] = (INT8)_ai4VolOffsetMap[u1Index];
    _aai1ChannelVolOffset[u1DecId][AUD_CH_SUB_WOOFER] = (INT8)_ai4SWVolOffsetMap[u1Index];
    ///_AUD_DspVolumeChange(u1DecId, AUD_CH_CENTER);
    _AUD_DspVolumeChange(u1DecId, AUD_CH_SUB_WOOFER);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}
#endif
#endif

#ifdef CC_AUD_SX1_FEATURE
//-----------------------------------------------------------------------------
/** _AUD_DspAdVolOffset
 *  setup AD channel volume offset 
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Value 0~100.
 *  @retval void
 */
//-----------------------------------------------------------------------------

void _AUD_DspAdVolOffset(UINT8 u1Value)
{
    UNUSED(_aau4AdLvlAdj);
    VOL_CTL_SEMA_LOCK(AUD_DEC_THIRD);
    _aau4AdLvlAdj = u1Value;
    _AudDspMasterVolChange(AUD_DEC_THIRD);
    VOL_CTL_SEMA_UNLOCK(AUD_DEC_THIRD);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspChannelVolShmRawValue
 *  Set master volume use share memory raw data .
 *
 *  Note that AUD_DspChannelVolume & _AUD_DspChannelVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4VolShm : 0 ~ 0x20000
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelVolShmRawValue(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4VolShm)
{
    VOL_CTL_SEMA_LOCK(u1DecId);
    _fgChannelVolUseShmRawData = TRUE;
    _aau4ChannelVolumeRawData[u1DecId][eChannel] = u4VolShm;
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** _AUD_DspChannelMute
 *  setup mute for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  fgMute TRUE, FALSE
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelMute(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgMute)
{
    const CHAR *pStr = NULL;
#ifndef CC_MT5391_AUD_3_DECODER
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif
#ifndef CC_AUD_DDI
    LOG(5,"CMD: set ChannelMute: Dec(%d) Ch(%d) Mute(%d)\n",u1DecId, (UINT8)eChannel, (UINT8)fgMute);
#else
    pStr = AUD_EnumToName(eChannelOutMap, AUD_ARRAY_SIZE(eChannelOutMap), eChannel);
    if (x_strcmp(pStr, "") != 0)
    {
        if (u1DecId == AUD_DEC_MAIN)
        {
            LOG(0, "Output Mute %-10s, MuteEnable(%d)\n", pStr, fgMute);
        }
        else if ((u1DecId == AUD_DEC_AUX) && (eChannel == AUD_CH_FRONT_LEFT))
        {
            LOG(0, "Output Mute SCART     , MuteEnable(%d)\n", fgMute);
        }
    }
#endif

    VOL_CTL_SEMA_LOCK(u1DecId);
    _aafgChlMute[u1DecId][eChannel] = fgMute;
    _AUD_DspVolumeChange(u1DspId, u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSoundModeMute
 *  setup mute for individual channel for sound mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  fgMute TRUE, FALSE
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSoundModeMute(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgMute)
{
    LOG(5,"CMD: set SoundModeMute: Dec(%d) Ch(%d) Mute(%d)\n",
        u1DecId, (UINT8)eChannel, (UINT8)fgMute);

    AUD_DEC_ID_VALIDATE(u1DecId);

    VOL_CTL_SEMA_LOCK(u1DecId);
    _aafgSoundModeMute[u1DecId][eChannel] = fgMute;
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

#ifdef CC_AUD_EFFECT_MUTE_PROTECT
void _AUD_DspSurroundMute(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgMute)
{
    LOG(5,"CMD: set Surroud Mute: Dec(%d) Ch(%d) Mute(%d)\n",
        u1DecId, (UINT8)eChannel, (UINT8)fgMute);

    AUD_DEC_ID_VALIDATE(u1DecId);
    UNUSED(_AUD_DspSurroundMute);
    
    VOL_CTL_SEMA_LOCK(u1DecId);
    _aafgSurroundMute[u1DecId][eChannel] = fgMute;
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

void _AUD_DspVBassMute(AUD_CH_T eChannel, BOOL fgMute)
{
    LOG(5,"CMD: set VBass Mute: Ch(%d) Mute(%d)\n",
        (UINT8)eChannel, (UINT8)fgMute);

    UNUSED(_AUD_DspVBassMute);
    
    VOL_CTL_SEMA_LOCK(AUD_DEC_MAIN);
    _aafgVBassMute[AUD_DEC_MAIN][eChannel] = fgMute;
    _AUD_DspVolumeChange(AUD_DSP0, AUD_DEC_MAIN, eChannel);
    VOL_CTL_SEMA_UNLOCK(AUD_DEC_MAIN);
}
#endif

void _AUD_DspClearSoundEnable(UINT8 u1DecId, BOOL fgEnable)
{
#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    static BOOL _fgCDNotchEnable = FALSE;
    static BOOL _fgInit = FALSE;

    if ((_fgCDNotchEnable != fgEnable) || (!_fgInit))
    {
        vWriteShmUINT8(B_CDNOTCH_FLAG, (UINT8)fgEnable);
        if (fgEnable)
        {
            vWriteShmUINT8(B_CDNOTCH_USER_Q, _au1CDNotchQIdx[0]);
            vWriteShmUINT16(W_CDNOTCH_USER_FC, _au1CDNotchFc[0]);
        }
        else
        {
            vWriteShmUINT8(B_CDNOTCH_USER_Q, _au1CDNotchQIdx[1]);
            vWriteShmUINT16(W_CDNOTCH_USER_FC, _au1CDNotchFc[1]);
        }

        DSP_SendDspTaskCmd(UOP_DSP_CDNOTCH_FLAG);
        _fgCDNotchEnable = fgEnable;
        _fgInit = TRUE;

    #ifdef CC_AUD_DIFFER_EQ_PEQ_BY_CS
        _AUD_DspChEqLoadPreset(u1DecId, _eEqMode);
        _AUD_DspSetPEQ(u1DecId, _u1MWSetPEQMode);
    #endif
    }
#endif

    UNUSED(u1DecId);
    UNUSED(fgEnable);
}

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
void _AUD_DspClearSoundCfg(UINT8 u1UserQIdx, UINT16 u2UserFc)
{
    if ((uReadShmUINT8(B_CDNOTCH_USER_Q) == u1UserQIdx) && (u2ReadShmUINT16(W_CDNOTCH_USER_FC) == u2UserFc))
    {
        return;
    }

    vWriteShmUINT8(B_CDNOTCH_USER_Q, u1UserQIdx);
    vWriteShmUINT16(W_CDNOTCH_USER_FC, u2UserFc);
    DSP_SendDspTaskCmd(UOP_DSP_CDNOTCH_CFG);
}
#endif

#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
void _AUD_DspSetRegDetection(BOOL fgRegDetection)
{
    _afgAudRegDetection = fgRegDetection;
}

BOOL _AUD_DspGetRegDetection(void)
{
    return _afgAudRegDetection;
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspChannelVolGain
 *  setup channel volume extra gain for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  i4Value     -256~96.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value)
{
#ifndef CC_MT5391_AUD_3_DECODER
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    if (eChannel >= AUD_CH_NUM)
    {
        LOG(0, "Incorrect parameter -- eChannel for _AUD_DspChannelVolGain\n");
        return;
    }

    VERIFY(x_sema_lock(_ahVolCtlSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);

    // Update local variables
    if (_aeStreamSrc[u1DecId] == AUD_STREAM_FROM_LINE_IN)
    {
        _aai1ChlVolExtraGain[u1DecId][eChannel] = i4Value;
    }
    else
    {
        _aai1ChlVolExtraGainNonLineIn[u1DecId][eChannel] = i4Value;
    }
    _AudDspChannelVolChange(AUD_DSP0, u1DecId, eChannel);

    VERIFY(x_sema_unlock(_ahVolCtlSema[u1DecId]) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** _AUD_DspChannelVolDolbyGainTB11
 *  setup channel volume Dolby gain TB11 for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  i4Value     -256~96.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelVolDolbyGainTB11(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value)
{
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    if (eChannel >= AUD_CH_NUM)
    {
        LOG(0, "Incorrect parameter -- eChannel for _AUD_DspChannelVolDolbyGainTB11\n");
        return;
    }

    VERIFY(x_sema_lock(_ahVolCtlSema[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);

    // Update local variables
    _aai1ChlVolDolbyGainTB11[eChannel] = i4Value;
    _AudDspChannelVolChange(AUD_DSP0, u1DecId, eChannel);

    VERIFY(x_sema_unlock(_ahVolCtlSema[u1DecId]) == OSR_OK);
}

/***************************************************************************
     Function : bAUD_IsDrcDec
     Description: check if the decoder has DRC functionality
     @param u1DecId
                AUD_DEC_MAIN: 1st dec
                AUD_DEC_AUX: 2nd dec
     @retval    TRUE : IsDrcDec
                     FALSE: IsNotDrcDec
***************************************************************************/
BOOL _bAUD_IsDrcDec(UINT8 u1DecId)
{
    BOOL bRet = FALSE;

    if(u1DecId==AUD_DEC_MAIN)
    {
        if( (_rDspVars[AUD_DSP0][u1DecId].bDspStrTyp==AC3_STREAM || _rDspVars[AUD_DSP0][u1DecId].bDspStrTyp==AAC_STREAM)
            || (_IsDualDecMode() && ( _rDspVars[AUD_DSP0][AUD_DEC_AUX].bDspStrTyp==AC3_STREAM || _rDspVars[AUD_DSP0][AUD_DEC_AUX].bDspStrTyp==AAC_STREAM))
            || (_IsTriOnlyDecMode() && ( _rDspVars[AUD_DSP0][AUD_DEC_THIRD].bDspStrTyp==AC3_STREAM || _rDspVars[AUD_DSP0][AUD_DEC_THIRD].bDspStrTyp==AAC_STREAM)))
        {
            LOG(5, "@@@@_bAUD_IsDrcDec: TRUE@@@@@\n");
            bRet = TRUE;
        }
    }

    return bRet;
}

/***************************************************************************
     Function : _AUD_DspDolbyGainTB11
     Description: Dolby Technical Bulletin 11.5 (addtional 3dB attenuation for EU)
     @param u1DecId
                AUD_DEC_MAIN: 1st dec
                AUD_DEC_AUX: 2nd dec
     @retval    void
***************************************************************************/
void _AUD_DspDolbyGainTB11(UINT8 u1DecId)
{
    UINT16    u2CompMode=2;
    BOOL    bIsDrcDec;
    INT8    i1Gain[(UINT32)AUD_CH_NUM];
    UINT8    aud_ch;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;

    LOG(5, "\n");
    if(u1DecId==AUD_DEC_MAIN)
    {
        LOG(5, "@@@@_AUD_DspDolbyGainTB11: u1DecId=AUD_DEC_MAIN@@@@\n");
    AUD_GetDecodeType(AUD_DSP0, AUD_DEC_MAIN, &eStreamFrom, &eDecType);
    }
    else
    {
        LOG(5, "@@@@_AUD_DspDolbyGainTB11: u1DecId is not equal to AUD_DEC_MAIN@@@@\n");
        LOG(5, "@@@@_AUD_DspDolbyGainTB11: No need to set DolbyGainTB11@@@@\n");

    return;
    }
    if(_rDspVars[AUD_DSP0][u1DecId].bDspStrTyp==AC3_STREAM 
        || (_IsDualDecMode() && _rDspVars[AUD_DSP0][AUD_DEC_AUX].bDspStrTyp==AC3_STREAM) 
        || (_IsTriOnlyDecMode() && _rDspVars[AUD_DSP0][AUD_DEC_THIRD].bDspStrTyp==AC3_STREAM))
    {
        u2CompMode = _AUD_DspGetAC3CompMode(u1DecId); // u2CompMode=(2=LINE Mode, 3=RF Mode)
    }
    else if(_rDspVars[AUD_DSP0][u1DecId].bDspStrTyp==AAC_STREAM 
        || (_IsDualDecMode() && _rDspVars[AUD_DSP0][AUD_DEC_AUX].bDspStrTyp==AAC_STREAM)
        ||(_IsTriOnlyDecMode() && _rDspVars[AUD_DSP0][AUD_DEC_THIRD].bDspStrTyp==AAC_STREAM))
    {
        u2CompMode = _AUD_DspGetDdtDmxRfMode(u1DecId, FALSE)+2;
    }
    bIsDrcDec = _bAUD_IsDrcDec(u1DecId); // bIsDrcDec=(TRUE, FALSE)
    LOG(5, "@@@@_AUD_DspDolbyGainTB11: u2CompMode=%d, bIsDrcDec=%s@@@@", u2CompMode, (bIsDrcDec==TRUE ? "TRUE" : "FALSE"));
    // initial
    for(aud_ch=(UINT8)AUD_CH_FRONT_LEFT; aud_ch<=(UINT8)AUD_CH_DMX_RIGHT; aud_ch++)
    {
        i1Gain[(UINT32)aud_ch] = 0; // reset
    }

    if(AUD_GetTargetCountry() == COUNTRY_EU || AUD_GetTargetCountry() == COUNTRY_BZ) // EU, BZ
    {
        LOG(5, "@@@@_AUD_DspDolbyGainTB11: COUNTRY_EU@@@@\n");
        if((eStreamFrom==AUD_STREAM_FROM_DIGITAL_TUNER) &&  (u2CompMode==3 || bIsDrcDec==FALSE)) // RF mode or Is not DrcDec
        {
            LOG(1, "@@@@_AUD_DspDolbyGainTB11: apply additional 8dB attenuation@@@@\n");
            i1Gain[(UINT32)AUD_CH_BYPASS_LEFT] = (INT8)(-8*2); // -8dB
            i1Gain[(UINT32)AUD_CH_BYPASS_RIGHT] = (INT8)(-8*2); // -8dB
        }
    }
    else // not EU, BZ
    {
        LOG(5, "@@@@_AUD_DspDolbyGainTB11: not COUNTRY_EU@@@@\n");
        if(u2CompMode==3 && bIsDrcDec==TRUE) // RF mode && IsDrcDec
        {
            for(aud_ch=(UINT8)AUD_CH_FRONT_LEFT; aud_ch<=(UINT8)AUD_CH_DMX_RIGHT; aud_ch++)
            {
                LOG(5, "@@@@@_AUD_DspDolbyGainTB11: compensate +3dB@@@@@\n");
                i1Gain[(UINT32)aud_ch] = (INT8)(3*2); // +3dB
            }
        }
    }
    LOG(5, "@@@@_AUD_DspDolbyGainTB11: Set Channel Volumn Gain@@@@\n");
    // set Channel Volumn Gain
    for(aud_ch=(UINT8)AUD_CH_FRONT_LEFT; aud_ch<=(UINT8)AUD_CH_DMX_RIGHT; aud_ch++)
    {
        LOG(5, "@@@@i1Gain[%d] = %d@@@@\n", aud_ch, (INT32)i1Gain[(UINT32)aud_ch]);
        AUD_DspChannelVolDolbyGainTB11(AUD_DEC_MAIN, (AUD_CH_T)aud_ch, (INT32)i1Gain[(UINT32)aud_ch]);
    }
}

INT32 _AUD_DspGetChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel)
{
    return _aai1ChlVolExtraGain[u1DecId][eChannel];
}

INT32 _AUD_DspGetChannelVolGainNonLineIn(UINT8 u1DecId, AUD_CH_T eChannel)
{
    return _aai1ChlVolExtraGainNonLineIn[u1DecId][eChannel];
}

#ifdef CC_MPEG_LEVEL_ADJ
//-----------------------------------------------------------------------------
/** AUD_DspMpgLvlChlVolGain
 *  setup channel volume extra gain for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel. 
 *                               1: front right channel.  
 *                               2: rear left channel. 
 *                               3: rear right channel. 
 *                               4: center channel.
 *                               5: sub woofer channel. 
 *                               6: bypass left channel. 
 *                               7: bypass right channel. 
 *                               8: downmix left channel. 
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ). 
 *  @param  i4Value     -256~96.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMpgLvlChlVolGain(UINT8 u1Level)
{
    VOL_CTL_SEMA_LOCK(AUD_DEC_MAIN);

    // Update local variables
    _aau1MpgLvlAdj = (INT32)u1Level;
    _AudDspChannelVolChange(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_FRONT_LEFT);
    _AudDspChannelVolChange(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_FRONT_RIGHT);
    _AudDspChannelVolChange(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_DMX_LEFT);
    _AudDspChannelVolChange(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_DMX_RIGHT);

    VOL_CTL_SEMA_UNLOCK(AUD_DEC_MAIN);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspChannelOutPort
 *  setup output port for individual channel.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eChannel  : 0: front left channel.
 *                               1: front right channel.
 *                               2: rear left channel.
 *                               3: rear right channel.
 *                               4: center channel.
 *                               5: sub woofer channel.
 *                               6: bypass left channel.
 *                               7: bypass right channel.
 *                               8: downmix left channel.
 *                               9: downmix right channel.
 *                             10: aux front left channel.
 *                             11: aux front right channel.
 *                             12: input source gain.
 *                             13: all channels    ( For master volume control ).
 *  @param  fgEnable TRUE, FALSE
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChannelOutPort(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgEnable)
{
    VOL_CTL_SEMA_LOCK(u1DecId);
    _aafgChlOutPort[u1DecId][eChannel] = fgEnable;
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, eChannel);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspBalance
 *  setup balance.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Balance  : balance value (0~100).
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifdef CC_AUD_ARM_RENDER
void _AUD_DspBalance(UINT8 u1DecId, UINT8 u1Balance)
{
    INT32 i4Balance;
    
    LOG(1,"Aproc Balance: Balance(%d)\n", u1Balance);

    i4Balance = (INT32)u1Balance;

    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_BALANCE, APROC_IOCTR_BALANCE_SP, &i4Balance, 1);
    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_BALANCE, APROC_IOCTR_BALANCE_HP, &i4Balance, 1);
}

#else
void _AUD_DspBalance(UINT8 u1DecId, UINT8 u1Balance)
{
    LOG(5,"CMD: set Balance: Dec(%d) Index(%d)\n", u1DecId, u1Balance);
    AUD_DEC_ID_VALIDATE_2(u1DecId);
    VOL_CTL_SEMA_LOCK(u1DecId);

    _au1Balance[u1DecId] = u1Balance;
#ifdef CC_AUD_DDI
    _AudDspChannelVolChangeNew(AUD_DSP0, u1DecId, AUD_CH_FRONT_LEFT,_aau1ChannelVolumeMain[u1DecId][AUD_CH_FRONT_LEFT],_aau1ChannelVolumeFine[u1DecId][AUD_CH_FRONT_LEFT]);
    _AudDspChannelVolChangeNew(AUD_DSP0, u1DecId, AUD_CH_FRONT_RIGHT,_aau1ChannelVolumeMain[u1DecId][AUD_CH_FRONT_RIGHT],_aau1ChannelVolumeFine[u1DecId][AUD_CH_FRONT_RIGHT]);
    _AudDspChannelVolChangeNew(AUD_DSP0, u1DecId, AUD_CH_REAR_LEFT,_aau1ChannelVolumeMain[u1DecId][AUD_CH_REAR_LEFT],_aau1ChannelVolumeFine[u1DecId][AUD_CH_REAR_LEFT]);
    _AudDspChannelVolChangeNew(AUD_DSP0, u1DecId, AUD_CH_REAR_RIGHT,_aau1ChannelVolumeMain[u1DecId][AUD_CH_REAR_RIGHT],_aau1ChannelVolumeFine[u1DecId][AUD_CH_REAR_RIGHT]);
    _AudDspChannelVolChangeNew(AUD_DSP0, u1DecId, AUD_CH_DMX_LEFT,_aau1ChannelVolumeMain[u1DecId][AUD_CH_DMX_LEFT],_aau1ChannelVolumeFine[u1DecId][AUD_CH_DMX_LEFT]);
    _AudDspChannelVolChangeNew(AUD_DSP0, u1DecId, AUD_CH_DMX_RIGHT,_aau1ChannelVolumeMain[u1DecId][AUD_CH_DMX_RIGHT],_aau1ChannelVolumeFine[u1DecId][AUD_CH_DMX_RIGHT]);
#else
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_FRONT_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_FRONT_RIGHT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_REAR_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_REAR_RIGHT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_RIGHT);
#endif
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}
#endif
//-----------------------------------------------------------------------------
/** AUD_DspMasterVolume
 *  set master volume.
 *
 *  Note that AUD_DspMasterVolume & _AUD_DspMasterVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Volume  : volume value (0~100).
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMasterVolume(UINT8 u1DecId, UINT8 u1Volume)
{
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    //INT32 i4Vol;
#endif
    LOG(5,"CMD: set Master Volume: Dec(%d) Volume(%d)\n", u1DecId, u1Volume);

#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    VOL_CTL_SEMA_LOCK(u1DecId);
    _fgMasterVolUseShmRawData = FALSE;
    if(_aau1ChannelVolume[u1DecId][AUD_CH_ALL] != u1Volume)
    {
        _aau1ChannelVolume[u1DecId][AUD_CH_ALL] = u1Volume;
        DRVCUST_SendAudEvent(E_CUST_AUD_VOLUME_CHANGE, u1DecId);
    }    
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_ALL);
    VOL_CTL_SEMA_UNLOCK(u1DecId);

#ifdef CC_AUD_FY12_LOUDNESS
    _AUD_DspSetSbass2ByMasterVolume(u1Volume);
#endif
#ifdef CC_AUD_LINEOUT_VOL_MODE
    if(_aafgLineOutVolChangeFlag)
    {
        _AUD_DspChannelVolume(AUD_DSP0, u1DecId, AUD_CH_REAR_LEFT, u1Volume);
        _AUD_DspChannelVolume(AUD_DSP0, u1DecId, AUD_CH_REAR_RIGHT, u1Volume);
    }
#endif

#if 0 //Already set by AUD_CH_ALL to _VolumeCtrl
//#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    i4Vol = (INT32) u1Volume * 0x147AE; // fix me!!!!

    if (u1DecId == AUD_DEC_MAIN)
    {
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_AMIXER0, &i4Vol, 1);
    }
    else
    {
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_AMIXER1, &i4Vol, 1);
    }
#endif

#ifdef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
    // Update channel volume
    if (u1DecId == AUD_DEC_MAIN)
    {
        _AudDspSetChVol(u1DecId, AUD_CH_FRONT_LEFT);
        _AudDspSetChVol(u1DecId, AUD_CH_FRONT_RIGHT);
    }
#endif

    // Update peq cfg
#if 1//def CC_AUD_PEQ_SUPPORT
#ifdef CC_AUD_DIFFER_PEQ_GAIN_FOR_VOL
    _AUD_DspSetPEQ(u1DecId, _u1MWSetPEQMode);
#endif
#endif

}

UINT8 _AUD_DspGetMasterVolume(UINT8 u1DecId)
{
    return _aau1ChannelVolume[u1DecId][AUD_CH_ALL];
}

void _AUD_DspMixSndVolume(UINT8 u1DecId, UINT8 u1Volume)
{
    UINT32 u4ValShm;
    UINT64 u8ValueShm;
    BOOL fgMute;

    LOG(9, "_AUD_DspMixSndVolume(), u1DecId(%d), Vol (%d)\n", u1DecId, u1Volume);

    _au4AdjVol[u1DecId] = VOL_SHM_0_DB;
    fgMute = (_afgMuteEnable[u1DecId]||_afgStopMuteEnable[u1DecId])? TRUE : FALSE;

    // Master volume = Mute * AdjVol * Master volume
    u8ValueShm = (((UINT64)_au4VOLUME[u1Volume]) * ((UINT64)_au4AdjVol[u1DecId]))/(UINT64)VOL_SHM_0_DB;

#ifdef CC_AUD_VOLUME_OFFSET
    // Apply volume offset
    u8ValueShm = ((UINT64)u8ValueShm * (UINT64)(_DbToShm((INT32)_aai1ChannelVolOffset[u1DecId][AUD_CH_ALL]))) / (UINT64)VOL_SHM_0_DB;
#endif
    u4ValShm = (UINT32)u8ValueShm;

    if (u4ValShm > _au4VOLUME[100])
    {
        u4ValShm = _au4VOLUME[100];
    }

    u4ValShm = fgMute ? 0 : u4ValShm;

    DSP_SetMixsndVolume(u4ValShm);

    // Send UOP to DSP
    //_VolumeCtrl(u1DecId, AUD_CH_ALL, u4ValShm);

    #if 0
    // Auto turn off speaker when master volume is 0
    if ((DRVCUST_OptGet(eAudioSpkAutoOffWhenVolMute) == 1) && (u1DecId == AUD_DEC_MAIN))
    {
        if (u4ValShm == 0)
        {
            // Wait DSP fade out
            x_thread_delay(50);
            ADAC_VolumeMuteSpeakerEnable(FALSE);
            LOG(8, "**** SPK Auto off due to volume is 0\n");;
        }
        else
        {
            ADAC_VolumeMuteSpeakerEnable(TRUE);
            LOG(8, "**** SPK Auto on due to volume is not 0\n");;
        }
    }
    #endif
}

//-----------------------------------------------------------------------------
/** _AUD_DspMasterVolShmRawValue
 *  Set master volume use share memory raw data .
 *
 *  Note that AUD_DspMasterVolume & _AUD_DspMasterVolShmRawValue can only select
 *  one to use
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4VolShm : 0 ~ 0x20000
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMasterVolShmRawValue(UINT8 u1DecId, UINT32 u4VolShm)
{
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif
    VOL_CTL_SEMA_LOCK(u1DecId);
    _fgMasterVolUseShmRawData = TRUE;
    _au4MasterVolShm[u1DecId] = u4VolShm;
    _AudDspMasterVolChange(u1DecId);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}
void _AUD_SetOutPortMode(UINT8 u1DecId, UINT8 fgMode)
{
    UNUSED(u1DecId);    

    _aeOutPortMode = (AUD_DEC_OUTPORT_MODE_T) fgMode;
#ifdef  CC_SONY_2K14_FLASH_AQ
    if (_aeOutPortMode == AUD_DEC_OUTPORT_MODE_SUB_WOOFER)
    {   

        AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWM_PAD_SUBWOFFER); //0x76763210 //for Subwoofer mono output..
    }
    else
    {
        AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWM_PAD_INIT); //0x76543210 //recover DAC channel setting..
    }
#endif
}

#ifdef CC_AUD_LINEOUT_VOL_MODE
BOOL AUD_GetLineOutVolumeMode(UINT8 u1DecId)
{
    return _aafgLineOutVolChangeFlag;
}

//-----------------------------------------------------------------------------
/** _AUD_SetLineOutVolumeMode
 *  Set line out volume control mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgMode : 0 : fixed 1:variable as master volume
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_SetLineOutVolumeMode(UINT8 u1DecId, UINT8 fgMode)
{
    _aafgLineOutVolChangeFlag = fgMode;
    vWriteShmUINT8(AUD_DSP0, B_LINEOUT_VOL_MODE, (UINT8)fgMode);
}
UINT8 _AUD_DspGetLineOutFixVol(void)
{
   return _aau1LineOutFixVol;
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspAVCEnable
 *  enable AVC.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAVCEnable(UINT8 u1DecId, BOOL fgEnable)
{
    BOOL fgAgcEnable;

    LOG(5,"CMD: set AVCEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

    AUD_DEC_ID_VALIDATE_2(u1DecId);
    VOL_CTL_SEMA_LOCK(u1DecId);

    if (_afgUserAgcFlag[u1DecId] != fgEnable)
    {
        // Check input source, then adjust AGC
        fgAgcEnable = fgEnable && _aafgInputSrcAgcFlag[u1DecId][AUD_EU_ATV_STREAM_SRC_TRANSFER(_aeStreamSrc[u1DecId])];
        _AudDspAgcEnalbe(u1DecId, fgAgcEnable);
        _afgUserAgcFlag[u1DecId] = fgEnable;
    }
    _afgUserAgcFlag[u1DecId] = fgEnable;

    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_ALL);
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

#ifdef CC_AUD_AVC_V20
//-----------------------------------------------------------------------------
/** AUD_DspChangeAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
UINT16 _gu2AvcControl[DRC2_NUM] = {
        W_AVC_FLAG, // DRC2_FLAG
    //  W_AVC_FC, // DRC2_FC
        D_AVC_TARGET_LEVEL, // DRC2_TARGET_LEVEL
        D_AVC_ATTACK_RATE, // DRC2_ATTACK_RATE
        D_AVC_RELEASE_RATE, // DRC2_RELEASE_RATE
        D_AVC_RATIO, // DRC2_RATIO
        D_AVC_SILENCE_LEVEL, // DRC2_SILENCE_LEVEL
        D_AVC_MAX_EXPAND, // DRC2_MAX_EXPAND
        D_AVC_POST_GAIN, // DRC2_POST_GAIN,
        D_AVC_HOLD, // DRC2_HOLD
        D_AVC_TARGET_LEVEL_1, // DRC2_TARGET_LEVEL_1
        D_AVC_ATTACK_RATE_1, // DRC2_ATTACK_RATE_1
        D_AVC_RELEASE_RATE_1, // DRC2_RELEASE_RATE_1
        D_AVC_RATIO_1, // DRC2_RATIO_1
        D_AVC_SILENCE_LEVEL_1, // DRC2_SILENCE_LEVEL_1
        D_AVC_MAX_EXPAND_1, // DRC2_MAX_EXPAND_1
        D_AVC_POST_GAIN_1, // DRC2_POST_GAIN_1
        D_AVC_HOLD_1, // DRC2_HOLD_1
};

void _AUD_DspChangeAVCPara (AUD_DRC_V20_INFO_T eType, UINT32 u4Value, UINT8 u1DecId)
{
    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    if (eType == DRC2_FLAG)
    {
         if (u4Value == 0)
        {
            AUD_DspAVCEnable(AUD_DEC_MAIN, FALSE);
        }
        else
        {
            AUD_DspAVCEnable(AUD_DEC_MAIN, TRUE);
        }
    }
    else if (eType < DRC2_NUM)
    {
        _AUD_DspCheckDrcBound (eType, &u4Value);
        vWriteShmUINT32(_gu2AvcControl[eType], u4Value);
        DSP_SendDspTaskCmd(UOP_DSP_AVC_CONTROL);
    }
    else
    {
        LOG (3, "_AUD_DspChangeAVCPara: error command (%d)!\n", eType);
    }
}

void _AUD_DspGetAVCPara(AUD_DRC_V20_INFO_T eType, UINT32* u4Value, UINT8 u1DecId)
{
    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    if (eType < DRC2_NUM)
    {
        if (eType < DRC2_TARGET_LEVEL)
        {
            *u4Value = (UINT32) u2ReadShmUINT16(_gu2AvcControl[eType]);
        }
        else
        {
            *u4Value = u4ReadShmUINT32(_gu2AvcControl[eType]);
        }
    }
    else
    {
        LOG (3, "_AUD_DspGetAVCPara: error command (%d)!\n", eType);
    }
}

#if 0   // Unused
void _AUD_DspChangeAVCPara_NoReinit(AUD_DRC_V20_INFO_T eType, UINT32 u4Value, UINT8 u1DecId)
{
    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    if (eType == DRC2_FLAG)
    {
         if (u4Value == 0)
        {
            AUD_DspAVCEnable(AUD_DEC_MAIN, FALSE);
        }
        else
        {
            AUD_DspAVCEnable(AUD_DEC_MAIN, TRUE);
        }
    }
    else if (eType < DRC2_NUM)
    {
        _AUD_DspCheckDrcBound (eType, &u4Value);
        vWriteShmUINT32(_gu2AvcControl[eType], u4Value);
    }
    else
    {
        LOG (3, "_AUD_DspChangeAVCPara_NoReinit: error command (%d)!\n", eType);
    }
}
#endif

#else

//-----------------------------------------------------------------------------
/** AUD_DspChangeAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChange2BandAVCPara(UINT8 u1Band, UINT16 u2Type, UINT16 u2Value, UINT8 u1DecId)
{
    UINT16 u2ShmIndex;
    UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;

    AUD_DEC_ID_VALIDATE_2(u1DecId);
    VERIFY((u1Band == 0) || (u1Band == 1));

    if (u1Band == 0)
    {
    switch(u2Type)
    {
    case 0:     //En/Dis
        if (u2Value==1)
            {
            AUD_DspAVCEnable(0,TRUE);
            }
        else if (u2Value==0)
            {
            AUD_DspAVCEnable(0,FALSE);
            }
        return;
    case 1:     //Target Lvl
        u2ShmIndex = W_AVC_TARGET_LEV;
        break;
    case 2:     //Silence Lvl
        u2ShmIndex = W_AVC_SILENCE_LEV;
        break;
    case 3:     //Max Gain Up
        u2ShmIndex = W_AVC_MAX_GAIN_UP;
        break;
    case 4:     //Max Gain Down
        u2ShmIndex = W_AVC_MAX_GAIN_DOWN;
        break;
    case 5:     //Adj Rate
        u2ShmIndex = W_AVC_ADJUST_RATE;
        break;
    case 6:     //UI Adj Rate
        u2ShmIndex = W_DSP_AVC_UI_ADJUST_RATE;
        break;
    case 7:     //Attack threshold
        u2ShmIndex = W_AVC_ATTACK_THRES;
        break;
    default:
        return;
    }
    }
    else
    {
    switch(u2Type)
    {
    case 0:     //En/Dis
        if (u2Value==1)
            {
            AUD_DspAVCEnable(AUD_DEC_AUX, TRUE);
            }
        else if (u2Value==0)
            {
            AUD_DspAVCEnable(AUD_DEC_AUX, FALSE);
            }
        return;
    case 1:     //Target Lvl
        u2ShmIndex = W_AVC_TARGET_LEV_DEC2;
        break;
    case 2:     //Silence Lvl
        u2ShmIndex = W_AVC_SILENCE_LEV_DEC2;
        break;
    case 3:     //Max Gain Up
        u2ShmIndex = W_AVC_MAX_GAIN_UP_DEC2;
        break;
    case 4:     //Max Gain Down
        u2ShmIndex = W_AVC_MAX_GAIN_DOWN_DEC2;
        break;
    case 5:     //Adj Rate
        u2ShmIndex = W_AVC_ADJUST_RATE_DEC2;
        break;
    case 6:     //UI Adj Rate
        u2ShmIndex = W_DSP_AVC_UI_ADJUST_RATE_DEC2;
        break;
    case 7:     //Attack threshold
        u2ShmIndex = W_AVC_ATTACK_THRES_DEC2;
        break;
    default:
        return;
    }
    }

    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);

}

//-----------------------------------------------------------------------------
/** AUD_DspChangeAVCPara
 *  setup automatic volume control parameters.
 *
 *  @param  u2Type     parameter selection
 *                             0: AVC on/off (u2Value = 0: AVC off, 1: AVC on)
 *                             1: AVC target level (dB) (u2Value = -6 ~ -24)
 *                             3: Max gain up (dB) (u2Value = 0 ~ 8)
 *                             5: Adjust rate (dB/128ms)
 *                             6: Adjust rate when AVC is turned on in UI (dB/128ms)
 *  @param  u2Value    parameter value
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspChangeAVCPara(UINT16 u2Type, UINT16 u2Value, UINT8 u1DecId)
{
    UINT16 u2ShmIndex;
    UINT16 u2UopIndex = UOP_DSP_AVC_CONTROL;

    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    switch (u2Type)
    {
    case 0:     //En/Dis
        if (u2Value==1)
        {
            AUD_DspAVCEnable(0,TRUE);
        }
        else if (u2Value==0)
        {
            AUD_DspAVCEnable(0,FALSE);
        }
        return;
    case 1:     //Target Lvl
        u2ShmIndex = W_AVC_TARGET_LEV;
        break;
    case 2:     //Silence Lvl
        u2ShmIndex = W_AVC_SILENCE_LEV;
        break;
    case 3:     //Max Gain Up
        u2ShmIndex = W_AVC_MAX_GAIN_UP;
        break;
    case 4:     //Max Gain Down
        u2ShmIndex = W_AVC_MAX_GAIN_DOWN;
        break;
    case 5:     //Adj Rate
        u2ShmIndex = W_AVC_ADJUST_RATE;
        break;
    case 6:     //UI Adj Rate
        u2ShmIndex = W_DSP_AVC_UI_ADJUST_RATE;
        break;
    case 7:     //Attack threshold
        u2ShmIndex = W_AVC_ATTACK_THRES;
        break;
    default:
        return;
    }

    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}

void _AUD_DspGetAVCPara(UINT16 u2Type, UINT16* u2Value, UINT8 u1DecId)
{
    UINT16 u2ShmIndex;

    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    switch (u2Type)
    {
    case 0:     //En/Dis
        u2ShmIndex = W_AVC_FLAG;
        break;
    case 1:     //Target Lvl
        u2ShmIndex = W_AVC_TARGET_LEV;
        break;
    case 2:     //Silence Lvl
        u2ShmIndex = W_AVC_SILENCE_LEV;
        break;
    case 3:     //Max Gain Up
        u2ShmIndex = W_AVC_MAX_GAIN_UP;
        break;
    case 4:     //Max Gain Down
        u2ShmIndex = W_AVC_MAX_GAIN_DOWN;
        break;
    case 5:     //Adj Rate
        u2ShmIndex = W_AVC_ADJUST_RATE;
        break;
    case 6:     //UI Adj Rate
        u2ShmIndex = W_DSP_AVC_UI_ADJUST_RATE;
        break;
    case 7:     //Attack threshold
        u2ShmIndex = W_AVC_ATTACK_THRES;
        break;
    default:
        return;
    }

    *u2Value = u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);

}

#ifdef CC_AUD_USE_FLASH_AQ
#ifndef CC_AUD_NEW_AQ_PARSER
void _AUD_DspChangeAVCPara_NoReinit(UINT16 u2Type, INT16 u2Value, UINT8 u1DecId)
{
    UINT16 u2ShmIndex;

    if (u1DecId != AUD_DEC_MAIN)
    {
        ASSERT(0);
    }

    switch(u2Type)
    {
    case 1:     //Target Lvl
        u2ShmIndex = W_AVC_TARGET_LEV;
        break;
    case 2:     //Silence Lvl
        u2ShmIndex = W_AVC_SILENCE_LEV;
        break;
    case 3:     //Max Gain Up
        u2ShmIndex = W_AVC_MAX_GAIN_UP;
        break;
    case 4:     //Max Gain Down
        u2ShmIndex = W_AVC_MAX_GAIN_DOWN;
        break;
    case 5:     //Adj Rate
        u2ShmIndex = W_AVC_ADJUST_RATE;
        break;
    case 6:     //UI Adj Rate
        u2ShmIndex = W_DSP_AVC_UI_ADJUST_RATE;
        break;
    case 7:     //Attack threshold
        u2ShmIndex = W_AVC_ATTACK_THRES;
        break;
    default:
        return;
    }

    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Value);
}
#endif
#endif
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspInputChangeAVCPara
 *  setup automatic volume control parameters by input source.
 *
 *  @param  eStreamFrom
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspInputChangeAVCPara(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
#ifdef CC_AUD_AVC_V20
    UNUSED(eStreamFrom);
#else
#ifdef CC_AUD_USE_FLASH_AQ
#ifndef CC_AUD_NEW_AQ_PARSER
    UINT8 eConvertSrc;

    eConvertSrc = _au1AQSrc_Convert[eStreamFrom];
    VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );

    if (_fgAQ_Enable[AQ_TYPE_AVC])
    {
        _AUD_DspChangeAVCPara_NoReinit(1, (UINT16)(INT8)(_au1AQ_AVC[eConvertSrc][0]), AUD_DEC_MAIN);
        _AUD_DspChangeAVCPara_NoReinit(3, _au1AQ_AVC[eConvertSrc][1], AUD_DEC_MAIN);
        _AUD_DspChangeAVCPara_NoReinit(5, _au1AQ_AVC[eConvertSrc][2], AUD_DEC_MAIN);
        _AUD_DspChangeAVCPara_NoReinit(6, _au1AQ_AVC[eConvertSrc][3], AUD_DEC_MAIN);
        DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_AVC_CONTROL);        
        return;
    }
#endif //CC_AUD_NEW_AQ_PARSER
#endif //CC_AUD_USE_FLASH_AQ

    _AUD_DspChangeAVCPara(1, _ai2AvcTargetLevel[(UINT8)eStreamFrom], AUD_DEC_MAIN);
    _AUD_DspChangeAVCPara(3, _ai2AvcGainUp[(UINT8)eStreamFrom], AUD_DEC_MAIN);
    _AUD_DspChangeAVCPara(5, _ai2AvcAdjRate[(UINT8)eStreamFrom], AUD_DEC_MAIN);
    _AUD_DspChangeAVCPara(6, _ai2AvcUiAdjRate[(UINT8)eStreamFrom], AUD_DEC_MAIN);
#endif
}

#ifdef CC_AUD_LOUDNESS_FROM_SOURCE_SOUNDMODE
void _AUD_DspInputChangeLoudnessPara(AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    #ifdef CC_AUD_USE_FLASH_AQ
    UINT8 eConvertSrc;
    UINT8 eConvertSoundMode;
    UINT8 u1Volume,u1TableIdx,u1TableIdx_temp;
    UNUSED(eConvertSrc);
    UNUSED(eConvertSoundMode);
    UNUSED(u1Volume);
    UNUSED(u1TableIdx);
    UNUSED(u1TableIdx_temp);    
    
    if (_fgAQ_Enable[AQ_TYPE_LOUDNESS])
    {
        eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
        VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
                
        if (_au1MWSetSoundModeIdx < sizeof(_au1AQSmod_Convert))
        {
            eConvertSoundMode = _au1AQSmod_Convert[_au1MWSetSoundModeIdx];
        }
        else
        {
            eConvertSoundMode = 0;
        }
        
        //Note that vol 45~49 maps to tbl[0], volume 50~54 maps to tbl[1], vol 55~59 maps to tbl[2], ..., 90~100 maps to tbl[9]
        u1Volume = _AUD_DspGetMasterVolume(AUD_DEC_MAIN);
        u1TableIdx = (u1Volume-1)/10;
        u1TableIdx_temp = (u1Volume-1)%10;
        if( u1TableIdx < 5) 
        {
            u1TableIdx = 0;
        }
        else if( u1TableIdx >= 9 ) 
        {
            u1TableIdx = 9;
        }
        else if( u1TableIdx_temp < 5) 
        {
            u1TableIdx = (u1TableIdx-4)*2-1;
        }
        else 
        {
            u1TableIdx = (u1TableIdx-4)*2;
        } 
        if( u1Volume <= 45 )
        {
            LOG(7, "==> AQ_TYPE_LOUDNESS :Bass = 0x%x; Treble = 0x%x, vol(%d),source(%d),sm(%d)\n",BASS_TREBLE_ATTN_GAIN_DB_MAX,BASS_TREBLE_ATTN_GAIN_DB_MAX,u1Volume,eConvertSrc,eConvertSoundMode);
            _AUD_DspBassBoostGain2(AUD_DEC_MAIN, BASS_TREBLE_ATTN_GAIN_DB_MAX);
            _AUD_DspClearBoostGain2(AUD_DEC_MAIN, BASS_TREBLE_ATTN_GAIN_DB_MAX);
        }
        else
        {
            LOG(7, "==> AQ_TYPE_LOUDNESS :Bass = 0x%x; Treble = 0x%x, vol(%d),source(%d),sm(%d)\n",_ai1BassAttnTable[eConvertSrc][eConvertSoundMode][u1TableIdx],_ai1TrebleAttnTable[eConvertSrc][eConvertSoundMode][u1TableIdx],u1Volume,eConvertSrc,eConvertSoundMode);
            _AUD_DspBassBoostGain2(AUD_DEC_MAIN, _ai1BassAttnTable[eConvertSrc][eConvertSoundMode][u1TableIdx]);
            _AUD_DspClearBoostGain2(AUD_DEC_MAIN, _ai1TrebleAttnTable[eConvertSrc][eConvertSoundMode][u1TableIdx]);
        }
    }
    #endif
}
#endif
//-----------------------------------------------------------------------------
/** AUD_DspMuteEnable
 *  audio mute control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0821
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
#ifdef CC_AUD_4_DECODER_SUPPORT
    AUD_DEC_ID_VALIDATE(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif
#endif
    VOL_CTL_SEMA_LOCK(u1DecId);

    _afgMuteEnable[u1DecId] = fgEnable;
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_ALL);

    VOL_CTL_SEMA_UNLOCK(u1DecId);

#ifdef CC_AUD_LINEOUT_VOL_MODE
    if(_aafgLineOutVolChangeFlag)
    {
        _AUD_DspChannelMute(AUD_DSP0, u1DecId, AUD_CH_REAR_LEFT, fgEnable);
        _AUD_DspChannelMute(AUD_DSP0, u1DecId, AUD_CH_REAR_RIGHT, fgEnable);
    }
#endif
    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** _AUD_DspMuteAllEnable
 *  audio mute control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMuteAllEnable(UINT8 u1DecId, BOOL fgEnable)
{
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0821
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    VOL_CTL_SEMA_LOCK(u1DecId);

    if (_afgAllChannelMute[u1DecId] != fgEnable)
    {
         _afgAllChannelMute[u1DecId] = fgEnable;
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_FRONT_LEFT);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_FRONT_RIGHT);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_REAR_LEFT);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_REAR_RIGHT);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_LEFT);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_RIGHT);
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
                _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_IEC_LEFT);
                _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_IEC_RIGHT);
#else
                _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_BYPASS_LEFT);
                _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_BYPASS_RIGHT);
#endif

    }

    VOL_CTL_SEMA_UNLOCK(u1DecId);

    if (_afgIecMute != fgEnable)
    {
        _afgIecMute = fgEnable;
        _AUD_DspIECConfig(_aeMWIec, _aafgMWSetSpdifFlag);
    }

}

//-----------------------------------------------------------------------------
/** _AUD_DspStopMuteEnable
 *  audio mute control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspStopMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    UNUSED(u1DecId);
    UNUSED(fgEnable);
    UNUSED(_AUD_DspStopMuteEnable);
#else
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0821
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif
    VOL_CTL_SEMA_LOCK(u1DecId);

    _afgStopMuteEnable[u1DecId] = fgEnable;
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_ALL);

    VOL_CTL_SEMA_UNLOCK(u1DecId);
#endif    
}

//-----------------------------------------------------------------------------
/** AUD_DspHdmiModeMuteEnable
 *  audio mute control in HDMI mode.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspHdmiModeMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);
    VOL_CTL_SEMA_LOCK(u1DecId);

    _afgHdmiModeMuteEnable[u1DecId] = fgEnable;
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_ALL);

    VOL_CTL_SEMA_UNLOCK(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspHdmiModeMuteEnable
 *  audio mute control in HDMI mode.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
 void AUD_DspHdmiChangeFormatMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);
    if (AUD_IsAudInitOK())
    {
#ifndef CC_MT5881
    VOL_CTL_SEMA_LOCK(u1DecId); // Remy verfication first
#endif

    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_FRONT_LEFT] = fgEnable;
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_FRONT_RIGHT] = fgEnable;
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_IEC_LEFT] = fgEnable;
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_IEC_RIGHT] = fgEnable;
#else
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_BYPASS_LEFT] = fgEnable;
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_BYPASS_RIGHT] = fgEnable;
#endif

    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_DMX_LEFT] = fgEnable;
    _aafgChlHdmiChgFmtMute[u1DecId][AUD_CH_DMX_RIGHT] = fgEnable;

    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_FRONT_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_FRONT_RIGHT);
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_IEC_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_IEC_RIGHT);
#else
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_BYPASS_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_BYPASS_RIGHT);
#endif

    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_RIGHT);
#ifndef CC_MT5881
    VOL_CTL_SEMA_UNLOCK(u1DecId); // Remy verfication first
#endif
    }
}
#ifdef CC_AUD_HDMI_SPDIF_CFG
//-----------------------------------------------------------------------------
/** AUD_DspHdmiMuteAnalogOutput
 *  audio mute control by HDMI change status.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
 void AUD_DspHdmiMuteAnalogOutput(UINT8 u1DecId, BOOL fgEnable)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    if (_ahVolCtlSema[u1DecId] == NULL)
    {
        return;
    }

    VOL_CTL_SEMA_LOCK(u1DecId);


#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    _aafgChlHdmiMuteAnalogOut[u1DecId][AUD_CH_IEC_LEFT] = fgEnable;
    _aafgChlHdmiMuteAnalogOut[u1DecId][AUD_CH_IEC_RIGHT] = fgEnable;
#else
    _aafgChlHdmiMuteAnalogOut[u1DecId][AUD_CH_BYPASS_LEFT] = fgEnable;
    _aafgChlHdmiMuteAnalogOut[u1DecId][AUD_CH_BYPASS_RIGHT] = fgEnable;
#endif

    _aafgChlHdmiMuteAnalogOut[u1DecId][AUD_CH_DMX_LEFT] = fgEnable;
    _aafgChlHdmiMuteAnalogOut[u1DecId][AUD_CH_DMX_RIGHT] = fgEnable;

    //Lineout
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_IEC_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_IEC_RIGHT);
#else
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_BYPASS_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_BYPASS_RIGHT);
#endif

    //Headphone
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_LEFT);
    _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_RIGHT);

    VOL_CTL_SEMA_UNLOCK(u1DecId);
}
#endif

 void AUD_DspDecPlayMuteEnable(UINT8 u1DecId, BOOL fgEnable)
{
    static BOOL _fgDecPlayMute[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};

    if (fgEnable != _fgDecPlayMute[u1DecId])
    {
        AUD_DEC_ID_VALIDATE(u1DecId);
        VOL_CTL_SEMA_LOCK(u1DecId);
    //background DEC play/stop affect SPDIF output.
#ifndef CC_AUD_DDI
        _AUD_DspPlayMuteSpdifEnable(!fgEnable); 
#endif
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        _AudAprocInputMute(u1DecId, fgEnable);
#else
        _aafgChDecPlayMute[u1DecId][AUD_CH_FRONT_LEFT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_FRONT_RIGHT] = fgEnable;
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        _aafgChDecPlayMute[u1DecId][AUD_CH_IEC_LEFT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_IEC_RIGHT] = fgEnable;
#else
        _aafgChDecPlayMute[u1DecId][AUD_CH_BYPASS_LEFT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_BYPASS_RIGHT] = fgEnable;
#endif

        _aafgChDecPlayMute[u1DecId][AUD_CH_DMX_LEFT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_DMX_RIGHT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_REAR_LEFT] = fgEnable;
        _aafgChDecPlayMute[u1DecId][AUD_CH_REAR_RIGHT] = fgEnable;


        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_FRONT_LEFT);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_FRONT_RIGHT);
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_IEC_LEFT);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_IEC_RIGHT);
#else
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_BYPASS_LEFT);
        AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_BYPASS_RIGHT);
#endif

        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_LEFT);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_DMX_RIGHT);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_REAR_LEFT);
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_REAR_RIGHT);
#endif
        
        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }

    _fgDecPlayMute[u1DecId] = fgEnable;
}

//-----------------------------------------------------------------------------
/** AUD_DspDecPowerOffMute
 *  Power off: aout gain mute to do fade out and then do HW mute later.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Mute/unmute. (1: mute audio, 0: unmute audio)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspDecPowerOffMute(UINT8 u1DecId, BOOL fgEnable)
{
    VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
    if (fgEnable)
    {
        AudAoutGainCtrl(u1DecId, 0, 0, AOUT_VOL_SETP_4DB);
    }
    else
    {
        AudAoutGainCtrl(u1DecId, 0, 0x1000, AOUT_VOL_SETP_4DB);
    } 
    LOG(0,"[Power off]AUD_DspDecPowerOffMute = %d\n", fgEnable); 
}
//-----------------------------------------------------------------------------
/** AUD_DspBassMngEnalbe
 *  Bass management control.
 *
 *  @param  u1DecId    0: first decoder 1: seconder decoder.
 *  @param  fgEnable   Bass management enable. (1: enable, 0: disable)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspBassMngEnalbe(UINT8 u1DspId, UINT8 u1DecId, BOOL fgEnable)
{
#ifndef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
    UINT16 u2ShmIndex = W_PROCMOD;
    UINT16 u2UopIndex = UOP_DSP_BYPASS_PROC_MODE;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC2;
    }

    if (fgEnable)
    {
        vWriteShmUINT16(u1DspId, u2ShmIndex, u2ReadShmUINT16(u1DspId, u2ShmIndex) & ~((UINT16)(BYPASS_BASS_MANAGEMENT | BYPASS_POST_PROCESSING)));
    }
    else
    {
        vWriteShmUINT16(u1DspId, u2ShmIndex, u2ReadShmUINT16(u1DspId, u2ShmIndex) | (UINT16)BYPASS_BASS_MANAGEMENT);
    }

    DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
#else
    /*Aproc Bmang*/

    UNUSED(u1DecId);

    if (_aAprocDspBmangSetInfo.CtrlFlag != fgEnable)
    {
        _aAprocDspBmangSetInfo.CtrlFlag = fgEnable;
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_BMANG, 0, (UINT32 *) &_aAprocDspBmangSetInfo, 1);
    }

#endif
}

BOOL _AUD_DspGetBassMngEnable(UINT8 u1DspId, UINT8 u1DecId)
{
#ifndef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
    UINT16 u2ShmIndex = W_PROCMOD;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
    }

    if (u2ReadShmUINT16(u1DspId, u2ShmIndex) & BYPASS_BASS_MANAGEMENT)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
#else
    /*Aproc Bmang*/

    if (_aAprocDspBmangSetInfo.CtrlFlag == 0)
        return FALSE;
    else
        return TRUE;
#endif
}

BOOL _AUD_DspGetChannelVolumeEnable(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT16 u2ShmIndex = W_PROCMOD;
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
    }

    if (u2ReadShmUINT16(u1DspId, u2ShmIndex) & BYPASS_CHANNEL_VOLUME)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL _AUD_DspGetDelayEnable(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT16 u2ShmIndex = W_PROCMOD;
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
    }

    if (u2ReadShmUINT16(u1DspId, u2ShmIndex) & BYPASS_DELAY)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

#ifdef CC_AUD_DDI
void _AUD_DspSetDelayEnable(UINT8 u1DspId, UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2Mode;
    UINT16 u2ShmIndex = W_PROCMOD;
    UINT32 u2UopIndex = UOP_DSP_BYPASS_PROC_MODE;

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC2;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = W_PROCMOD_DEC3;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = W_PROCMOD_DEC4;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC4;
    }
#endif

    u2Mode = u2ReadShmUINT16(u1DspId, u2ShmIndex);

    if(fgEnable)
    {
        u2Mode &= (~(BYPASS_DELAY));
    }
    else
    {
        u2Mode |= (BYPASS_DELAY);
    }

    vWriteShmUINT16(u1DspId, u2ShmIndex, u2Mode);
    DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
}
#endif

BOOL _AUD_DspGetPostProcessingEnable(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT16 u2ShmIndex = W_PROCMOD;
    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = W_PROCMOD_DEC3;
    }
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = W_PROCMOD_DEC4;
    }

    if (u2ReadShmUINT16(u1DspId, u2ShmIndex) & BYPASS_POST_PROCESSING)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void _AUD_DspProcMode(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2Mode)
{
    UINT16 u2ShmIndex = W_PROCMOD;
    UINT16 u2UopIndex = UOP_DSP_BYPASS_PROC_MODE;

#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC2;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = W_PROCMOD_DEC3;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = W_PROCMOD_DEC4;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC4;
    }
#endif

    vWriteShmUINT16(u1DspId, u2ShmIndex, u2Mode);
    DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
}

#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
#ifdef CC_AUD_USE_NVM
void _AUD_DspVSModeChange(UINT8 u1DecId, UINT8 u1ModeIdx)
{
    if (u1ModeIdx > 1)
        u1ModeIdx = 1;

    if (u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT32 (D_VSURR_CLARITY,   _rNVMSurrCfg[u1ModeIdx].u4Clarity);   //center gain,
        vWriteShmUINT32 (D_VSURR_WIDTH,   _rNVMSurrCfg[u1ModeIdx].u4Width);   //surround gain,
        vWriteShmUINT32 (D_VSURR_LR_GAIN,   _rNVMSurrCfg[u1ModeIdx].u4LRGain); //L/R gain,
        vWriteShmUINT32 (D_VSURR_CROS_TALK,   _rNVMSurrCfg[u1ModeIdx].u4Xtalk); //crosstalk,
        vWriteShmUINT32 (D_VSURR_OUTPUT_GAIN,   _rNVMSurrCfg[u1ModeIdx].u4OutputGain); //output gain,
        vWriteShmUINT32 (D_VSURR_BASS_GAIN,   _rNVMSurrCfg[u1ModeIdx].u4BassGain); //bass enhancement,
        vWriteShmUINT32 (D_VSURR_FO,   _rNVMSurrCfg[u1ModeIdx].u4Fo); //FO,

        // Update surround parameters
        DSP_SendDspTaskCmd (UOP_DSP_NEW_MTK_VSURR);

     }
}
#endif
#else
void _AUD_DspVSModeChange(UINT8 u1DecId, UINT8 u1ModeIdx)
{
    UINT32 u4VSParameter;
    UINT8 eVSCfgType;

    if (u1ModeIdx > AUD_VS_NUM-1)
        u1ModeIdx = AUD_VS_NUM-1;

#ifdef  CC_SONY_2K14_FLASH_AQ
    _au1MWSetVsModeIdx = u1ModeIdx;
#endif
    if (u1DecId == AUD_DEC_MAIN)
    {
      for (eVSCfgType = 0; eVSCfgType < AUD_SUR_PRAR_NUM; eVSCfgType++)
        {
           u4VSParameter = _aai1VSMode[u1ModeIdx][eVSCfgType];
           LOG(6,"CMD: _AUD_DspVSModeChange: eVSCfgType(0x%x) eVSCfgType(0x%x)\n", eVSCfgType, u4VSParameter);
           switch(eVSCfgType)
           {
               case AUD_SUR_CLARITY:
                   vWriteShmUINT32 (AUD_DSP0, D_VSURR_CLARITY,   u4VSParameter);   //center gain,
                   break;
               case AUD_SUR_WIDTH:
                   vWriteShmUINT32 (AUD_DSP0, D_VSURR_WIDTH,   u4VSParameter);   //surround gain,
                   break;
               case AUD_SUR_LRGAIN:
                   vWriteShmUINT32 (AUD_DSP0, D_VSURR_LR_GAIN,   u4VSParameter); //L/R gain,
                   break;
               case AUD_SUR_XTALK:
                   vWriteShmUINT32 (AUD_DSP0, D_VSURR_CROS_TALK,   u4VSParameter); //crosstalk,
                   break;
               case AUD_SUR_OUTPUT_GAIN:
                   vWriteShmUINT32 (AUD_DSP0, D_VSURR_OUTPUT_GAIN,   u4VSParameter); //output gain,
                   break;
               case AUD_SUR_BASS_GAIN:
                   vWriteShmUINT32 (AUD_DSP0, D_VSURR_BASS_GAIN,   u4VSParameter); //bass enhancement,
                   break;
               case AUD_SUR_FO:
                   vWriteShmUINT32 (AUD_DSP0, D_VSURR_FO,   u4VSParameter); //FO,
                   break;
               default :
                   break;
           }
            //DSP_SendDspTaskCmd (UOP_DSP_NEW_MTK_VSURR);
        }
    }
}
#endif //CC_AUD_NEW_POST_MULTI_VS_SUPPORT
#ifdef CC_AUD_SOUND_MODE
void _AUD_DspPostSoundModeChange(UINT8 u1DecId, UINT8 u1ModeIdx)
{
    UINT8 u1PEQSet;
    UINT32 u4Frequency;
    UINT16 u2BW;
    INT16 i2Gain;

    UINT8 u1Level;

    if (u1ModeIdx > AUD_SM_NUM-1)
        u1ModeIdx = AUD_SM_NUM-1;


    if (u1DecId == AUD_DEC_MAIN)
    {
      for (u1PEQSet=0; u1PEQSet < PEQ_SETTING_MAX; u1PEQSet++)
      {
        u4Frequency = _aai1PeqFq[u1ModeIdx][u1PEQSet];
        u2BW = _aai1PeqBw[u1ModeIdx][u1PEQSet];
        i2Gain = _aai1PeqGain[u1ModeIdx][u1PEQSet];
        _AUD_DspSetPEQCfg(u1DecId, u1PEQSet, u4Frequency, u2BW, i2Gain);
       }

    u1Level = _aai1BassTreble[u1ModeIdx][0];
    _AUD_DspBassBoostGain(u1DecId, u1Level);

    u1Level = _aai1BassTreble[u1ModeIdx][1];
    _AUD_DspClearBoostGain(u1DecId, u1Level);

    }

}
#endif // CC_AUD_SOUND_MODE
#ifdef  CC_SONY_2K14_FLASH_AQ
void _AUD_DspSetTvPosition(UINT8 u1DecId,UINT8 u1Position,BOOL fgWithMute)
{
    UINT8 u1ModeIdx;
    UINT8 i;
    UINT8 eConvertSrc;
    UINT32 u4VSParameter;
    AUD_DEC_STREAM_FROM_T eStreamFrom;

#ifdef  CC_AUD_PEQ_SUPPORT
    UINT16   u2PEQDataFc[9];
    UINT16   u2PEQDataQ[9];
    INT8   i1PEQDataGain[9];
    UINT8* u4TempPtr;
#endif

    UNUSED(u1Position);
    UNUSED(u1ModeIdx);
    UNUSED(i);
    UNUSED(u4VSParameter);

    AUD_DEC_ID_VALIDATE(u1DecId);

#if 1///def CC_SONY_2K13_FLASH_AQ
    if((u1Position > 2)||(u1Position == 0))   ///Orignal 
    {
        u1Position = 1;  ///Standard Mode
    }

#endif
    u1ModeIdx = _au1AQSmod_Convert[_au1MWSetSoundModeIdx];

    
    if (u1ModeIdx > (AUD_SOUND_MODE_NUM-1))
    {
        u1ModeIdx = AUD_SOUND_MODE_NUM - 1;
    }

    LOG(5,"CMD: set SoundMode: Dec(%d) u1Position(%d) u1ModeIdx(%d)\n", u1DecId, u1Position, u1ModeIdx);

    _au1MWSetPositionIdx = u1Position;

    if(_au1PositionCurrentIdx == u1Position)
    {
        LOG(5, "Redundent position setting!\n");
        return;
    }
    else
    {
        _au1PositionCurrentIdx = u1Position;
    }


    if (fgWithMute)
    {
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_LEFT, TRUE);
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_RIGHT, TRUE);
    }

    AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom) ;
    
    VERIFY(eStreamFrom < AUD_STREAM_FROM_NUM);

    eConvertSrc = _au1AQSrc_Convert[eStreamFrom];

#ifdef CC_AUD_USE_FLASH_AQ
    if(_au1MWSetPositionIdx==1)
    {
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_MTKDRC_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);
    }
    else    
    {
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_MTKDRC_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx);
    }
    DSP_SendDspTaskCmd(UOP_DSP_DRC_CONTROL2); 
    
    if(_au1MWSetPositionIdx==1)
    {
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_LOUDNESS_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);
    }
    else
    {
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_LOUDNESS_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx);
    }

    if(_au1MWSetPositionIdx==1)
    {
        if(_fgAQ_Enable[AQ_TYPE_PEQ_TABLE])
        {
            #ifdef CC_AUD_PEQ_SUPPORT
            u4TempPtr = (UINT8*)&(_au1AQ_PEQ_TABLE[eConvertSrc][u1ModeIdx]);
            for (i=0; i<9; i++)
            {
                u2PEQDataFc[i] = *(UINT8*)(u4TempPtr + i*5);
                u2PEQDataFc[i] = (*(UINT8*)(u4TempPtr + 1 + i*5)) + (u2PEQDataFc[i]<<8);
                u2PEQDataQ[i] = *(UINT8*)(u4TempPtr + 2 + i*5);
                u2PEQDataQ[i] = (*(UINT8*)(u4TempPtr + 3 + i*5)) + (u2PEQDataQ[i]<<8);
                i1PEQDataGain[i] = *(UINT8*)(u4TempPtr + 4 + i*5);
                AUD_DspSetPEQCfg2(AUD_DEC_MAIN, i, u2PEQDataFc[i], u2PEQDataQ[i], (INT16)i1PEQDataGain[i]);
            }
            #else
                LOG(0,"################################\n");
                LOG(0,"Please Enable CC_AUD_PEQ_SUPPORT\n");
                LOG(0,"################################\n");
            #endif
        }
    }
    else
    {
        if(_fgAQ_Enable[AQ_TYPE_PEQ_WALL])
        {
        #ifdef CC_AUD_PEQ_SUPPORT
            u4TempPtr = (UINT8*)&(_au1AQ_PEQ_WALL[eConvertSrc][u1ModeIdx]);
            for (i=0; i<9; i++)
            {
                u2PEQDataFc[i] = *(UINT8*)(u4TempPtr + i*5);
                u2PEQDataFc[i] = (*(UINT8*)(u4TempPtr + 1 + i*5)) + (u2PEQDataFc[i]<<8);
                u2PEQDataQ[i] = *(UINT8*)(u4TempPtr + 2 + i*5);
                u2PEQDataQ[i] = (*(UINT8*)(u4TempPtr + 3 + i*5)) + (u2PEQDataQ[i]<<8);
                i1PEQDataGain[i] = *(UINT8*)(u4TempPtr + 4 + i*5);
                AUD_DspSetPEQCfg2(AUD_DEC_MAIN, i, u2PEQDataFc[i], u2PEQDataQ[i], (INT16)i1PEQDataGain[i]);
            }
        #else
                LOG(0,"################################\n");
                LOG(0,"Please Enable CC_AUD_PEQ_SUPPORT\n");
                LOG(0,"################################\n");
        #endif
        }   
            
    }

    if (u1ModeIdx!=3) //CUST mode should not apple EQ, just let AP set EQ.
    {
        if(_au1MWSetPositionIdx==1)
        {
            _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_SPKHEIGHT_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);        
        }
        else
        {
            _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_SPKHEIGHT_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx);  
    
        }    
    }    
    else
    {
#if (defined(CC_AUD_SPEAKER_HEIGHT) || defined(CC_AUD_SPEAKER_HEIGHT_V2))
#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
        _AUD_DspSetSPHMode(0); //CUST mode doesn't support Speaker Shift, this is customer Spec. 20091027
#endif
#endif
    }

    
    if(_au1MWSetPositionIdx==1)
    {   
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_BASS_TREBLE_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);        
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_BASS_MANAGEMENT_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);
    }
    else
    {
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_BASS_TREBLE_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx); 
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_BASS_MANAGEMENT_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx);  
    }
    // Set Virtual Bass parameters.
    if(_au1MWSetPositionIdx==1)
    {
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_VBASS_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);  
    }
    else
    {
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_VBASS_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx); 
    }       



    if(_au1MWSetPositionIdx==1)
    {
        if(_fgAQ_Enable[AQ_TYPE_MVS_TABLE])
        {
            VERIFY(sizeof(_aai1VSMode) >= sizeof(_au4AQ_MVS_TABLE));
            x_memcpy((VOID *)VIRTUAL((UINT32)_aai1VSMode), (const VOID *)_au4AQ_MVS_TABLE, sizeof(_au4AQ_MVS_TABLE));
        }
    }
    else
    {
        if(_fgAQ_Enable[AQ_TYPE_MVS_WALL])
        {
            VERIFY(sizeof(_aai1VSMode) >= sizeof(_au4AQ_MVS_WALL));
            x_memcpy((VOID *)VIRTUAL((UINT32)_aai1VSMode), (const VOID *)_au4AQ_MVS_WALL, sizeof(_au4AQ_MVS_WALL));
        }
    }   

    for (i = 0; i < AUD_SUR_PRAR_NUM; i++)
    {
           u4VSParameter = _aai1VSMode[_au1MWSetVsModeIdx][i];
           LOG(6,"CMD: _AUD_DspVSModeChange: eVSCfgType(0x%x) eVSCfgType(0x%x)\n", i, u4VSParameter);
           switch(i)
           {
               case AUD_SUR_CLARITY:
                   vWriteShmUINT32 (D_VSURR_CLARITY,   u4VSParameter);   //center gain,
                   break;
               case AUD_SUR_WIDTH:
                   vWriteShmUINT32 (D_VSURR_WIDTH,   u4VSParameter);   //surround gain,
                   break;
               case AUD_SUR_LRGAIN:
                   vWriteShmUINT32 (D_VSURR_LR_GAIN,   u4VSParameter); //L/R gain,
                   break;
               case AUD_SUR_XTALK:
                   vWriteShmUINT32 (D_VSURR_CROS_TALK,   u4VSParameter); //crosstalk,
                   break;
               case AUD_SUR_OUTPUT_GAIN:
                   vWriteShmUINT32 (D_VSURR_OUTPUT_GAIN,   u4VSParameter); //output gain,
                   break;
               case AUD_SUR_BASS_GAIN:
                   vWriteShmUINT32 (D_VSURR_BASS_GAIN,   u4VSParameter); //bass enhancement,
                   break;
               case AUD_SUR_FO:
                   vWriteShmUINT32 (D_VSURR_FO,   u4VSParameter); //FO,
                   break;
               default :
                   break;
           }
    }
    DSP_SendDspTaskCmd (UOP_DSP_NEW_MTK_VSURR);

    DSP_SendDspTaskCmd (UOP_DSP_FIR_CHANGE);
    

#endif //CC_AUD_USE_FLASH_AQ

     if (fgWithMute)
     {
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_LEFT, FALSE);
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_RIGHT, FALSE);
     }
}
#endif
void _AUD_DspPostSoundMode(UINT8 u1DecId, UINT8 u1ModeIdx, BOOL fgWithMute)
{
    UINT8 u1ModeCase;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    SAMPLE_FREQ_T eFS = FS_48K;

    UNUSED(eFS);
    UNUSED(u1ModeCase);

    AUD_DEC_ID_VALIDATE(u1DecId);

#if 1///def CC_SONY_2K13_FLASH_AQ
    ///Make sure sound mode without Custom case.
    ///u1ModeIdx
    ///Dynamic:     2
    ///Standard:    1
    ///Clear Voice: 3
    if((u1ModeIdx > 3)||(u1ModeIdx == 0))   ///Orignal 
    {
        u1ModeIdx = 1;  ///Standard Mode
    }
#endif

    LOG(5,"CMD: set SoundMode: Dec(%d) Mode(%d)\n", u1DecId, u1ModeIdx);
    LOG(5,"_au1MWSetSoundModeIdx = %d, _au1SoundModeCurrentIdx = %d\n", _au1MWSetSoundModeIdx, _au1SoundModeCurrentIdx);
    _au1MWSetSoundModeIdx = u1ModeIdx;

#ifdef CC_AUD_USE_FLASH_AQ
    UNUSED(_au1CurrentSoudMode);
    u1ModeIdx = _au1AQSmod_Convert[u1ModeIdx];
#endif

    if (u1ModeIdx > (AUD_SOUND_MODE_NUM-1))
    {
        u1ModeIdx = AUD_SOUND_MODE_NUM - 1;
    }

    AUD_DRVGetStreamFrom(AUD_DSP0, u1DecId, &eStreamFrom)   ;

    VERIFY(eStreamFrom < AUD_STREAM_FROM_NUM);

#ifdef CC_AUD_SOUND_MODE
    u1ModeCase = _arSoundModeCase[u1ModeIdx][eStreamFrom];
    _AUD_DspPostSoundModeChange(u1DecId, u1ModeCase);
#else
    u1ModeCase = _arSoundModeCase[u1ModeIdx][eStreamFrom];

    if(_au1SoundModeCurrentIdx == u1ModeCase)
    {
        LOG(5, "Redundent sound mode setting!\n");
        return;
    }
    else
    {
        _au1SoundModeCurrentIdx = u1ModeCase;
    }
#endif //CC_AUD_SOUND_MODE

    if (fgWithMute)
    {
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_LEFT, TRUE);
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_RIGHT, TRUE);
    }


#ifdef CC_AUD_USE_FLASH_AQ
    if (u1ModeIdx!=3) //CUST mode should not apple EQ, just let AP set EQ.
    {
        // All EQ is controlled by AP now.
        //_AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_EQ, AQ_OP_EXE, (UINT16)u1ModeIdx);     
#ifdef  CC_SONY_2K14_FLASH_AQ
        if(_au1MWSetPositionIdx==1)
        {
            _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_MTKDRC_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);
        }
        else    
        {
            _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_MTKDRC_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx);
        }
        DSP_SendDspTaskCmd(UOP_DSP_DRC_CONTROL2); 

        if(_au1MWSetPositionIdx==1)
        {
            _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_SPKHEIGHT_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);        
        }
        else
        {
            _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_SPKHEIGHT_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx);  

        }
#else
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_SPKHEIGHT, AQ_OP_EXE, (UINT16)u1ModeIdx);    
#endif
    }    
    else
    {
#if (defined(CC_AUD_SPEAKER_HEIGHT) || defined(CC_AUD_SPEAKER_HEIGHT_V2))
#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
        _AUD_DspSetSPHMode(0); //CUST mode doesn't support Speaker Shift, this is customer Spec. 20091027
#endif
#endif
    }
#ifdef  CC_SONY_2K14_FLASH_AQ
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_BASS_TREBLE_CUTOFF, AQ_OP_EXE, (UINT16)u1ModeIdx);  
    if(_au1MWSetPositionIdx==1)
    {
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_LOUDNESS_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_BASS_TREBLE_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_BASS_MANAGEMENT_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);  
    }
    else
    {
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_LOUDNESS_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx);
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_BASS_TREBLE_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx);       
        _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_BASS_MANAGEMENT_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx);  
    }
#else
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_LOUDNESS, AQ_OP_EXE, (UINT16)u1ModeIdx);
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_BASS_TREBLE, AQ_OP_EXE, (UINT16)u1ModeIdx);        
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_BASS_MANAGEMENT, AQ_OP_EXE, (UINT16)u1ModeIdx);  
#endif
    _AUD_FlashAQPeriphSMChange(eStreamFrom, u1ModeIdx);
    /* MT5366 output clock is 48K.
    if (_dwDspFreq == SFREQ_32K)
    {
        eFS = FS_32K;
    }
    else if (_dwDspFreq == SFREQ_44K)
    {
        eFS = FS_44K;    
    }
    */
    _AUD_FlashAQPeriph_EXTPEQ(eStreamFrom, u1ModeIdx, eFS);

    // Set Virtual Bass parameters.
#ifdef  CC_SONY_2K14_FLASH_AQ
    if(_au1MWSetPositionIdx==1)
    {
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_VBASS_TABLE, AQ_OP_EXE, (UINT16)u1ModeIdx);  
    }
    else
    {
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_VBASS_WALL, AQ_OP_EXE, (UINT16)u1ModeIdx); 
    }
#else
    _AUD_FlashAQOperation(AUD_DEC_MAIN, AQ_TYPE_VBASS, AQ_OP_EXE, (UINT16)u1ModeIdx);   
#endif
#endif //CC_AUD_USE_FLASH_AQ

}

#if 1//def CC_AUD_PEQ_SUPPORT
#ifdef CC_AUD_DIFFER_PEQ_GAIN_FOR_VOL
static INT16 _AudDspGetPeqGain(UINT8 u1DecId, UINT8 u1ModeIdx, UINT8 u1PEQSet)
{
    UINT8 u1Vol;
    INT16 i2Gain;
    #define PEQ_BASE_VOL    (60)
    
    if (u1ModeIdx > AUD_PEQ_CASE_NUM-1)
        u1ModeIdx = 0;

    u1Vol = _aau1ChannelVolume[u1DecId][AUD_CH_ALL];

    if (((u1ModeIdx <= AUD_PEQ_CASE1) || (u1ModeIdx >= AUD_PEQ_CASE4)) && ((u1PEQSet == 7) || (u1PEQSet == 8)))
    {
        if (u1Vol <= (20*MAX_VOL_LEVEL/PEQ_BASE_VOL))
        {
            i2Gain = _arPEQTable[u1ModeIdx][u1PEQSet].i2PeqGain;
        }
        else if (u1Vol <= (30*MAX_VOL_LEVEL/PEQ_BASE_VOL))
        {
            i2Gain = _ai2PEQGainTable[u1PEQSet - 7][0];
        }
        else if (u1Vol <= (40*MAX_VOL_LEVEL/PEQ_BASE_VOL))
        {
            i2Gain = _ai2PEQGainTable[u1PEQSet - 7][1];
        }
        else if (u1Vol <= (50*MAX_VOL_LEVEL/PEQ_BASE_VOL))
        {
            i2Gain = _ai2PEQGainTable[u1PEQSet - 7][2];
        }
        else if (u1Vol <= (60*MAX_VOL_LEVEL/PEQ_BASE_VOL))
        {
            i2Gain = _ai2PEQGainTable[u1PEQSet - 7][3];
        }
        else
        {
            i2Gain = _arPEQTable[u1ModeIdx][u1PEQSet].i2PeqGain;
        }
    }
    else
    {
        i2Gain = _arPEQTable[u1ModeIdx][u1PEQSet].i2PeqGain;
    }

    return i2Gain;
}
#endif
#endif

void _AUD_DspSetPEQ(UINT8 u1DecID, UINT8 u1ModeIdx)
{
#if 1//def CC_AUD_PEQ_SUPPORT
    UINT8 u1PEQSet;
    UINT32 u4Frequency;
    UINT8 u1Q;
    INT16 i2Gain;

    LOG(5," PEQ : _AUD_DspSetPEQ : Dec(%d) u1ModeIdx(%d)\n", u1DecID,u1ModeIdx);

    _u1MWSetPEQMode = u1ModeIdx;

    if(u1ModeIdx > AUD_PEQ_CASE_NUM -1)
        u1ModeIdx = 0;
    
#ifdef CC_AUD_DIFFER_EQ_PEQ_BY_CS
    if (((u1ModeIdx == AUD_PEQ_CASE4) || (u1ModeIdx == AUD_PEQ_CASE5)) && (BOOL)uReadShmUINT8(B_CDNOTCH_FLAG))
    {
        u1ModeIdx = u1ModeIdx - (AUD_PEQ_CASE4 - AUD_PEQ_CASE0);
    }
    else if (((u1ModeIdx == AUD_PEQ_CASE0) || (u1ModeIdx == AUD_PEQ_CASE1)) && !(BOOL)uReadShmUINT8(B_CDNOTCH_FLAG))
    {
        u1ModeIdx = u1ModeIdx + (AUD_PEQ_CASE4 - AUD_PEQ_CASE0);
    }
#endif
    
    if (u1DecID == AUD_DEC_MAIN)
    {
        for (u1PEQSet=2; u1PEQSet < PEQ_SETTING_MAX; u1PEQSet++)
        {
            u4Frequency = _arPEQTable[u1ModeIdx][u1PEQSet].u2PeqFc;
            u1Q = _arPEQTable[u1ModeIdx][u1PEQSet].u1PeqQ;
#ifndef CC_AUD_DIFFER_PEQ_GAIN_FOR_VOL
            i2Gain = _arPEQTable[u1ModeIdx][u1PEQSet].i2PeqGain;
#else
            i2Gain = _AudDspGetPeqGain(u1DecID, u1ModeIdx, u1PEQSet);
#endif
            _AUD_DspSetPEQCfg2(u1DecID,u1PEQSet,u4Frequency,u1Q,i2Gain);
        }
    }
#else
    UNUSED(u1DecID);
    UNUSED(u1ModeIdx);
#endif
}

#ifdef CC_AUD_PEQ_BASS_TREBLE
void _AUD_DspPEQBassTreble(UINT8 u1DecId,BOOL isTreble,UINT8 u1Level)
{

#if 1//def CC_AUD_PEQ_SUPPORT
    UINT8 u1PEQSet;

    LOG(5," PEQ : _AUD_DspPEQBassTreble : Dec(%d) isTreble(%d) Level (%d)\n", u1DecId,isTreble,u1Level);

    if (isTreble)
        u1PEQSet = 1;
    else
        u1PEQSet = 0;

    _AUD_DspSetPEQCfg2(u1DecId,u1PEQSet,\
        _arPEQTable[0][u1PEQSet].u2PeqFc,\
        _arPEQTable[0][u1PEQSet].u1PeqQ,\
        (INT16)(u1Level-24));
#else
    UNUSED(u1DecId);
    UNUSED(isTreble);
    UNUSED(u1Level);
#endif
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspPostSoundModeStart
 *  Indicate sound mode start/end
 *
 *  @param  u1DecId          Audio decoder id (0: first decoder, 1: second decoder).
 *  @param  fgStart          start(TRUE), end(FALSE).
 *
 *  @return void
 */
//-----------------------------------------------------------------------------
void _AUD_DspPostSoundModeStart(UINT8 u1DecId, BOOL fgStart)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (fgStart)
    {
        // Move to set sound mode to avoid duplicated mute sound
        //_AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_LEFT, TRUE);
        //_AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_RIGHT, TRUE);
    }
    else
    {
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_LEFT, FALSE);
        _AUD_DspSoundModeMute(u1DecId, AUD_CH_FRONT_RIGHT, FALSE);
    }
}
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, BOOL fgEnable)
{
    APROC_DSP_MVS_SET_T  aMVSSet; 

    UNUSED(u1DecId);
    
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_MVS, 0, (UINT32 *) &aMVSSet, 1);
    
    aMVSSet.fgMVSFlag = fgEnable;

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_MVS, 0, (UINT32 *) &aMVSSet, 1);

}
#else
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspVirtualSurroundEnalbe
 *  enable virtual surround.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
//sunman_tshd
#ifndef CC_AUD_USE_NVM
void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, BOOL fgEnable)
{
#if defined(DSP_SUPPORT_SRSTSXT) ||  defined(DSP_SUPPORT_SRSTSHD) ||  defined(DSP_SUPPORT_SRSWOWHD) ||  defined(DSP_SUPPORT_SRSCC3D)
    // SRS TSXT or SRS TSHD
    UINT16 u2ShmIndex = B_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_SRS_TSXT_FLAG;
    UINT8 u1Flag = uReadShmUINT8(u2ShmIndex);
    static BOOL bInit=FALSE;
    static BOOL bEnable=TRUE;

    LOG(5,"CMD: set SRSEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);
    // to prevent the duplicated CMD for setting SRSEnable ==> reinit many times
    if(!bInit)
    {
        bInit = TRUE;
        bEnable = fgEnable;
        LOG(5, "_AUD_DspVirtualSurroundEnalbe:first time setting\n");
    }
    else
    {
        if(bEnable==fgEnable)
        {
            LOG(5, "_AUD_DspVirtualSurroundEnalbe:duplicated setting===>skip!\n");
            return;
        }
        else
        {
            bEnable = fgEnable;
        }
    }
    if (u1DecId == AUD_DEC_MAIN)
    {
        if (fgEnable)
        {
            //AUD_DspSpeakerOutputConfig(0, 0); // 2/0 LtRt // FIXME
            if ( (u1Flag & (0x01<<5)) != 0)  //VSS_SRS_TSXT
            {
                return;
            }
            vWriteShmUINT8(u2ShmIndex, (UINT8)(0x01<<5)|(UINT8)(fgEnable));
        }
        else
        {
            //AUD_DspSpeakerOutputConfig(0, 2);
            if ( (u1Flag & (0x01<<5)) == 0)
            {
                return;
            }
            vWriteShmUINT8(u2ShmIndex, 0);
        }
        DSP_SendDspTaskCmd(u2UopIndex);
    }
#else
    // MTK Virtual Surround
    UINT16 u2ShmIndex = B_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG;

    LOG(5,"CMD: set VSEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)fgEnable);

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_VSURRFLAG_DEC2;
        u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG_DEC2;
    }

    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
#endif
}
#else
void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, UINT8 u1ModeIdx)
{
#if defined(DSP_SUPPORT_SRSTSXT) ||  defined(DSP_SUPPORT_SRSTSHD) ||  defined(DSP_SUPPORT_SRSWOWHD)
    // SRS TSXT or SRS TSHD
    UINT16 u2ShmIndex = B_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_SRS_TSXT_FLAG;
    UINT8 u1Flag = uReadShmUINT8(u2ShmIndex);

    LOG(5,"CMD: set SRSEnable: Dec(%d) Enable(%d)\n", u1DecId, (UINT8)u1ModeIdx);

    if (u1DecId == AUD_DEC_MAIN)
    {
        if (u1ModeIdx)
        {
            //AUD_DspSpeakerOutputConfig(0, 0); // 2/0 LtRt // FIXME
            if ( (u1Flag & (0x01<<5)) != 0)  //VSS_SRS_TSXT
            {
                return;
            }
            vWriteShmUINT8(u2ShmIndex, (UINT8)(0x01<<5)|(UINT8)(u1ModeIdx));
        }
        else
        {
            //AUD_DspSpeakerOutputConfig(0, 2);
            if ( (u1Flag & (0x01<<5)) == 0)
            {
                return;
            }
            vWriteShmUINT8(u2ShmIndex, 0);
        }
        DSP_SendDspTaskCmd(u2UopIndex);
    }
#else
    // MTK Virtual Surround
    static UINT8  u1LastModeInx[AUD_DEC_NUM] = { 0xff, 0xff };
    UINT16 u2ShmIndex = B_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG;

    LOG(5,"CMD: set VSEnable: Dec(%d) u1ModeIdx(%d)\n", u1DecId, (UINT8)u1ModeIdx);

    if (u1LastModeInx[u1DecId] == u1ModeIdx)
        return;

    u1LastModeInx[u1DecId] = u1ModeIdx;
    
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_VSURRFLAG_DEC2;
        u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG_DEC2;
    }

    if( u1ModeIdx > 0 )
    {
        vWriteShmUINT8(u2ShmIndex, 1);
        _AUD_DspVSModeChange(u1DecId,u1ModeIdx-1);
    }
    else
    {
        vWriteShmUINT8(u2ShmIndex, 0);
    }

    DSP_SendDspTaskCmd(u2UopIndex);
#endif
}
#endif
#else // CC_AUD_NEW_POST_MULTI_VS_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspVirtualSurroundEnalbe
 *  enable virtual surround.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1ModeIdx  : 0: Off
 *                                 1: MVS
 *                                 2: M2S
 *                                 3: ....
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, UINT8 u1ModeIdx)
{
    // MTK Virtual Surround
    /*
    AP   SH
    0--> 0     MVS and M2S off
    1--> 1     MVS ON :Normal
    2--> 1<<8  M2S ON
    3--> 1     MVS ON :Sports
    4--> 1     MVS ON :Music
    5--> 1     MVS ON :Cinema
    6--> 1     MVS ON :Game
    */
#ifdef DSP_SUPPORT_SRSTSXT        // SRS TSXT
    UINT16 u2ShmIndex = W_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_SRS_TSXT_FLAG;

    if (u1DecId == AUD_DEC_MAIN)
    {
        if(u1ModeIdx == 1 )
        {
            AUD_DspSpeakerOutputConfig(0, 0); // 2/0 LtRt // FIXME
            vWriteShmUINT16(u2ShmIndex, (UINT16)(0x01<<5));
        }
        else
        {
            AUD_DspSpeakerOutputConfig(0, 0);
            vWriteShmUINT16(u2ShmIndex, 0);
        }
        DSP_SendDspTaskCmd(u2UopIndex);
    }
#else //DSP_SUPPORT_SRSTSXT

    UINT16 u2ShmIndex = W_VSURRFLAG;
    UINT16 u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG;

    _au1MWSetVSIdx = u1ModeIdx;
    LOG(5,"CMD: set MVSEnable: Dec(%d) Mode(%d)\n", u1DecId, u1ModeIdx);
    if (_au1VSCurrentIdx == u1ModeIdx)
    {
        LOG(5, "duplicated Vsrr setting!!\n");
        return;
    }
    
#ifdef CC_AUD_EFFECT_MUTE_PROTECT
    _AUD_DspSurroundMute(u1DecId, AUD_CH_FRONT_LEFT, TRUE);
    _AUD_DspSurroundMute(u1DecId, AUD_CH_FRONT_RIGHT, TRUE);
#endif
    _au1VSCurrentIdx = u1ModeIdx;  

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_VSURRFLAG_DEC2;
        u2UopIndex = UOP_DSP_VIRTUAL_SURROUND_FLAG_DEC2;
    }

    if (u1ModeIdx == 2)
    {
        vWriteShmUINT16(AUD_DSP0, u2ShmIndex, (UINT16)(0x01<<8));
    }
    else if (u1ModeIdx < 2)
    {
        vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u1ModeIdx);
    }
    else
    {
        vWriteShmUINT16(AUD_DSP0, u2ShmIndex, 0x01);
    }

    if (u1ModeIdx != 2)
    {
        if (u1ModeIdx > 2)
        {
            if (u1ModeIdx ==3)
            {
                u1ModeIdx = 5;              
            }
            else if (u1ModeIdx ==5)
            {
                u1ModeIdx = 3;
            }
            u1ModeIdx = u1ModeIdx - 2 ;
        }
        else
        {
            u1ModeIdx = 0;
        }

        _AUD_DspVSModeChange(u1DecId,u1ModeIdx);
    }
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
#endif
}
#endif //CC_AUD_NEW_POST_MULTI_VS_SUPPORT
#endif//CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
BOOL _AUD_DspGetVirtualSurroundEnalbe(UINT8 u1DecId)
{
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
    APROC_DSP_MVB_SET_T  aMVBSet; 

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_MVB, 0, (UINT32 *) &aMVBSet, 1);

    if (aMVBSet.fgMVBFlag == 1)
        return TRUE;
    else
        return FALSE;
    
#else
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    UINT16 u2ShmIndex = B_VSURRFLAG;
#else
    UINT16 u2ShmIndex = W_VSURRFLAG;
#endif
    if (u1DecId != AUD_DEC_MAIN)
    {
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
        u2ShmIndex = B_VSURRFLAG_DEC2;
#else
        u2ShmIndex = W_VSURRFLAG_DEC2;
#endif
    }

    if (uReadShmUINT8(AUD_DSP0, u2ShmIndex))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif //CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
}

void _AUD_DspADModeSet(UINT8 u1DecId, UINT16 u2Value)
{
    UINT16 u2ShmIndex = B_ADFLAG;
    UINT32 u2UopIndex = UOP_DSP_AD_FLAG;
    UINT16 u4Config = 0;
    UINT8   u1ADFLAGValue = 0x0;
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)    
    UINT32 u4Reg0;
#endif    

    LOG(5, "CMD: Set Dec(%d) AD Mode(%d), 0:CH9/10,1:L/R, 2:CH9/10 & L/R,4:default \n", u1DecId, u2Value);
    if (u2Value == 0)
    {
        u4Config |= 0x2 ;  // AD will added to CH9/10
    }
    if (u2Value == 1)
    {
        u4Config |= 0x8 ; //AD will added to L/R
    }
    if (u2Value == 2)
    {
        u4Config |= 0xA ; //AD will added to CH9/10 & L/R
    }

    #ifdef CC_AUD_AD_MIX_BYPASS
    u4Config |= 0x10 ; //AD will added to Bypass
    #endif
    u1ADFLAGValue = uReadShmUINT8(AUD_DSP0, B_ADFLAG) & (0x1);
    vWriteShmUINT8(AUD_DSP0, u2ShmIndex , (uReadShmUINT8(AUD_DSP0, B_ADFLAG) & 0xFFF1) | u4Config);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    if (u1ADFLAGValue == 0x1)
    {
        if (u2Value == 0) 
        {
            // AD will added to CH9/10
            u4Reg0 = 1;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
            u4Reg0 = 0;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);        
        }
        if (u2Value == 1)
        {
            //AD will added to L/R
            u4Reg0 = 0;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
            u4Reg0 = 1;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);        
        }
        if (u2Value == 2)
        {
            //AD will added to CH9/10 & L/R
            u4Reg0 = 0;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);        
        }  
        if (u2Value == 4)
        {
            //AD will not be added to CH9/10 & L/R
            u4Reg0 = 1;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);        
        }
    }
#endif
#ifdef CC_AUD_AD_FORCE_PCM      
    _AUD_DspSetIEC(AUD_DEC_MAIN);
#endif
}



//Audio Description
void _AUD_DspADEnable(UINT8 u1DecId, BOOL fgEnable)
{
#ifndef CC_AUD_SUPPORT_MS10
    UINT8   u1ADFLAGValue;
    UINT16 u2ShmIndex = B_ADFLAG;
    UINT32 u2UopIndex = UOP_DSP_AD_FLAG;

    u1ADFLAGValue = (uReadShmUINT8(AUD_DSP0, B_ADFLAG) & (0xFE)) | fgEnable;

#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
#endif

    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)u1ADFLAGValue);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
#else
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
#else

    UINT8   u1ADFLAGValue,u1OldAdFlag;
    UINT16 u2ShmIndex = B_ADFLAG;
    UINT32 u2UopIndex = UOP_DSP_AD_FLAG;
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)    
#ifndef CC_AUD_DDI
    UINT32 u4Reg0;
#endif
#endif     
    
#ifdef CC_AUD_DDI
    AUD_DRVSetADType(AUD_DEC_THIRD,fgEnable);
#endif
    DSP_SetTriDecMode(fgEnable);
#if 0//#ifdef CC_AUD_LINEIN_USE_DEC3
    if (!_AudGetADType(AUD_DEC_THIRD))
    {
        //not AD, bypass Dec3 post
        _AUD_DspProcMode(AUD_DEC_THIRD, (fgEnable?0xB000:0x9000));    //bypass post proc of Dec3
        return;
    }
#endif

    u1OldAdFlag = uReadShmUINT8(AUD_DSP0, B_ADFLAG);
    u1ADFLAGValue = ((u1OldAdFlag & (0xBE)) | fgEnable) | 0x80;      //clear bit6: one buffer mode, set bit7: AD do sample-by-sample
    LOG(5, "Set AD Enable = %d, u1OldAdFlag = 0x%x, u1ADFLAGValue = 0x%x\n", fgEnable, u1OldAdFlag, u1ADFLAGValue);
    if (u1OldAdFlag != u1ADFLAGValue)
    {
        vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)u1ADFLAGValue);
        if (!fgEnable)
        {
            DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
        }
    }
#ifdef CC_AUD_ARM_RENDER
#ifdef CC_AUD_DDI
      // LOG(0, "-------Set u1DecId = %d, fgEnable = %d\n", u1DecId, fgEnable);
       if((u1DecId == AUD_DEC_MAIN)&&(fgEnable==TRUE))
        {       
          vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_COMM_2, APROC_REG_AMIXER2_ADDEC_ENABLE), AUD_ADEC0_AD_ENABLE);
          _AudAprocInputMute(AUD_DEC_AUX, FALSE);    
        }
       else if((u1DecId== AUD_DEC_AUX)&&(fgEnable==TRUE))
        {
         vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_COMM_2, APROC_REG_AMIXER2_ADDEC_ENABLE), AUD_ADEC1_AD_ENABLE);
        }
       else if(fgEnable== FALSE)
        {       
         vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_COMM_2, APROC_REG_AMIXER2_ADDEC_ENABLE), AUD_AD_DISABLE);
        }
#else
    if (fgEnable)
    {
        if ((u1OldAdFlag&0xA) == 2)
        {
            // AD will added to CH9/10
            u4Reg0 = 1;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
            u4Reg0 = 0;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);      
        }
        if ((u1OldAdFlag&0xA) == 8)
        {
            //AD will added to L/R
            u4Reg0 = 0;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
            u4Reg0 = 1;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);          
        }   
        if ((u1OldAdFlag&0xA) == 0xA)
        {
            //AD will added to CH9/10 & L/R
            u4Reg0 = 0;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);       
        }     
        if ((u1OldAdFlag&0xA) == 0x0)
        {
            //AD will not be added to CH9/10 & L/R
            u4Reg0 = 1;
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);      
        }          
    }
#endif
#endif    
#endif
#endif
#ifndef CC_MT5391_AUD_3_DECODER
    if (fgEnable)
    {
        _AUD_DspProcMode(AUD_DEC_AUX, 0x9000); //turn off bit 9:  L/R is the same as Down-Mix Output
    }
    else
    {
        _AUD_DspProcMode(AUD_DEC_AUX, 0xB000);
    }
#else
    if (fgEnable)
    {
#ifdef CC_AUD_LINEIN_USE_DEC3 //if trionly mode, bypass dec 2 post processing
        if (_AUD_DspIsTriOnlyDecMode())
        {
            _AUD_DspProcMode(AUD_DSP0, AUD_DEC_THIRD, 0xf000); //turn off bit 9:  L/R is the same as Down-Mix Output
        }
        else
        {
            _AUD_DspProcMode(AUD_DSP0, AUD_DEC_THIRD, 0x9000); //turn off bit 9:  L/R is the same as Down-Mix Output
        }
#else
        _AUD_DspProcMode(AUD_DSP0, AUD_DEC_THIRD, 0x9000); //turn off bit 9:  L/R is the same as Down-Mix Output
#endif        
    }
    else
    {
        //_AUD_DspProcMode(AUD_DEC_THIRD, 0xB000);
         _AUD_DspProcMode(AUD_DSP0, AUD_DEC_THIRD, 0x9000); // gallen, test on 20080505
    }

#endif

#ifdef CC_AUD_AD_FORCE_PCM      
    _AUD_DspSetIEC(AUD_DEC_MAIN);
#endif
}

#ifdef CC_AUD_SKYPE_SUPPORT
//-----------------------------------------------------------------------------
/** _AUD_DspSkypeRingMix
 *  Enable Ring sound mix for skype application. Note that enable this means to enable
 *  "dec3->dec1 mixsing".
 *
 *  @param  fgEnable 0: disable 1: enable.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSkypeRingMix(BOOL fgEnable)
{
    UINT16 u2ShmIndex = B_ADFLAG;
    UINT32 u2UopIndex = UOP_DSP_AD_FLAG;
    UINT8   u1ADFLAGValue;

     u1ADFLAGValue = uReadShmUINT8(B_ADFLAG);

    if (fgEnable)
    {
        u1ADFLAGValue |= 0xB ;  // A =>Ring sound will be added to both L/R and CH9/10;
                                              // 1=> audio descript enable
    }
    else
    {
        u1ADFLAGValue &= 0xFFF0 ;
    }

    vWriteShmUINT8(u2ShmIndex, (UINT8)u1ADFLAGValue);
    DSP_SendDspTaskCmd(u2UopIndex);

}

//-----------------------------------------------------------------------------
/** _AUD_DspAuxMixToMain
 *  This function enables the mixing from AUX to Main, by setting internal_buffer_input_flag[9] and system_ctrl[0].
 *  "dec2->dec1 mixsing".
 *
 *  @param  fgEnable 0: disable 1: enable.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAuxMixToMain(BOOL fgEnable)
{
    UINT8   u2Value;
    UINT32 u2UopIndex = UOP_DSP_AUX_MIX_MAIN;

     u2Value = u2ReadShmUINT16 (W_INTERNAL_BUF_IN_FLAG);

    if (fgEnable)
    {
        vWriteShmUINT16 (W_INTERNAL_BUF_IN_FLAG, (u2Value|0x200));  // bit 9: read from internal buffer, after volume
    }
    else
    {
        vWriteShmUINT16 (W_INTERNAL_BUF_IN_FLAG, (u2Value&0xFDFF));
    }
    DSP_SetDualDecMode(fgEnable);
    DSP_SetAuxMixMainMode(fgEnable);

    DSP_SendDspTaskCmd(u2UopIndex);

}
#endif

#ifdef CC_AUD_PCM_LINE_IN_LPF
void _AUD_DSPSetPCMLineInLPF(BOOL fgEnable)
{
    DSP_SetPCMLineInLPF(fgEnable);
    return;
}
#endif


#ifdef CC_AUD_BBE_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspBbeMode
 *  Set BBE mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1BbeMode : 0: OFF, 1: BBE, 2: BBE ViVA4.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspBbeMode(UINT8 u1DecId, UINT8 u1BbeMode)
{
    UINT16 u2ShmIndex = B_BBE_FLAG;
    UINT32 u2UopIndex = UOP_DSP_BBE_MODE;

    AUD_DEC_ID_VALIDATE_2(u1DecId);

    // BBE is only for main decoder
    if (u1DecId == AUD_DEC_MAIN)
    {
       _u1BbeMode = u1BbeMode;
        vWriteShmUINT8(u2ShmIndex, u1BbeMode);
        DSP_SendDspTaskCmd(u2UopIndex);
    }
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspMonoDownMix
 *  enable mono downmix.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : on/off.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMonoDownMix(UINT8 u1DecId, BOOL fgEnable)
{
    UINT16 u2ShmIndex = B_KARAFLAG;
    UINT32 u2UopIndex = UOP_DSP_KARAOKE_FLAG; // Use karaoke UOP to do mono downmix

    UINT8 u1CurSetting = 0;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_KARAFLAG_DEC2;
        u2UopIndex = UOP_DSP_KARAOKE_FLAG_DEC2;
    }

    u1CurSetting = uReadShmUINT8(AUD_DSP0, u2ShmIndex);

    if ((BOOL)u1CurSetting == fgEnable)
    {
        return;
    }

    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetKaraokeMixRatio
 *  Set karaoke mix ratio
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u4MixRatio  : 0x0 ~ 0x800000 (for R channel)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetKaraokeMixRatio(UINT8 u1DecId, UINT32 u4MixRatio)
{
    UINT16 u2ShmIndex = D_LRMIXRATIO;
    UINT32 u2UopIndex = UOP_DSP_LR_MIX_RATIO;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = D_LRMIXRATIO_DEC2;
        u2UopIndex = UOP_DSP_LR_MIX_RATIO_DEC2;
    }

    vWriteShmUINT32(AUD_DSP0, u2ShmIndex, u4MixRatio);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetMatrixOutMode
 *  setup for matrix output mode.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  eMode 0: stereo (disable), 1: L mono, 2: R mono, 3: mix mono
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetMatrixOutMode(UINT8 u1DecId, AUD_MATRIX_OUT_T eMode)
{
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)

    UINT32 *pu4Addr;
    UINT32 u4Mode =  (UINT32)eMode;
    pu4Addr = &u4Mode;

    AUD_DEC_ID_VALIDATE(u1DecId);

    switch (u1DecId)
    {
    case AUD_DEC_MAIN:
        _vAUD_Aproc_Set(APROC_CONTROL_TYPE_DUALMONO, APROC_IOCTRL_DUALMONO_AMXIER0_MODE, pu4Addr, 1);
        break;
    case AUD_DEC_AUX:
        _vAUD_Aproc_Set(APROC_CONTROL_TYPE_DUALMONO, APROC_IOCTRL_DUALMONO_AMXIER1_MODE, pu4Addr, 1);
        break;
    case AUD_DEC_THIRD:
        _vAUD_Aproc_Set(APROC_CONTROL_TYPE_DUALMONO, APROC_IOCTRL_DUALMONO_AMXIER2_MODE, pu4Addr, 1);
        break;
    default:
        break;   
    }          

#else

    UINT8 u1OldMode, u1Mode;
    UINT16 u2ShmIndex=B_MANUAL_OUTPUT_MATRIX;
    UINT16 u2UopIndex=UOP_DSP_DUAL_MONO_MODE;

    AUD_DEC_ID_VALIDATE(u1DecId);

    u1Mode = (UINT8)eMode;

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_MANUAL_OUTPUT_MATRIX_DEC2;
        u2UopIndex = UOP_DSP_DUAL_MONO_MODE_DEC2;
    }

    u1OldMode = uReadShmUINT8(AUD_DSP0, u2ShmIndex);

    if (u1Mode != u1OldMode)
    {
        vWriteShmUINT8(AUD_DSP0, u2ShmIndex,u1Mode);
        DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
    }
#endif    

}

UINT8 _AUD_DspGetMatrixOutMode(UINT8 u1DecId)
{
    UINT16 u2ShmIndex=B_MANUAL_OUTPUT_MATRIX;
    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_MANUAL_OUTPUT_MATRIX_DEC2;
    }

    return uReadShmUINT8(AUD_DSP0, u2ShmIndex);
}

//-----------------------------------------------------------------------------
/** AUD_DspAutoDmx
 *  Setup auto downmix mode for AC3
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : 0: disable AC3 auto downmix, 1: enable AC3 auto downmix
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAutoDmx(UINT8 u1DecId, BOOL fgEnable)
{
#if 0
    UINT16 u2ShmIndex = B_AC3AUTODNMIX;
    UINT32 u2UopIndex = UOP_DSP_AC3_AC3AUTODNMIX;
    UINT32 u2AoutReinitUopIndex = UOP_DSP_AOUT_REINIT;

    //CLI_Parser("aud.uop.spkuop.ch 0 2"); // switch speaker config to 2/0 Lo/Ro
    //AUD_DspSpeakerOutputConfig(u1DecId, 2);

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = B_AC3AUTODNMIX_DEC2;
        u2UopIndex = UOP_DSP_AC3_AC3AUTODNMIX_DEC2;
        u2AoutReinitUopIndex = UOP_DSP_AOUT_REINIT_DEC2;
    }

    if (fgEnable)
    {
        vWriteShmUINT8(u2ShmIndex, TRUE);
    }
    else
    {
        vWriteShmUINT8(u2ShmIndex, FALSE);
    }

    DSP_SendDspTaskCmd(u2UopIndex);
    DSP_SendDspTaskCmd(u2AoutReinitUopIndex);
#else
    UINT8 u1SpkCfg;

    AUD_DEC_ID_VALIDATE_3(u1DecId);
    if(fgEnable)
    {
        u1SpkCfg = ((AUD_DspGetSpeakerOutputConfig(u1DecId) & 0x07) | 0x40);
    }
    else
    {
        u1SpkCfg = (AUD_DspGetSpeakerOutputConfig(u1DecId) & 0x07);
    }
    AUD_DspSpeakerOutputConfig(u1DecId, u1SpkCfg);
#endif
}

//-----------------------------------------------------------------------------
/** AUD_GetDspAutoDmx
 *  get auto downmix mode for AC3
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  fgEnable  : 0: disable AC3 auto downmix, 1: enable AC3 auto downmix
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL _AUD_GetDspAutoDmx (UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE_3(u1DecId);
    if((AUD_DspGetSpeakerOutputConfig(u1DecId) & 0x40) == 0x40)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspBypassPostProcess
 *  Bypass audio post-processing.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u2BypassMode: 1: bypass post-processing.
 *                      0x8000: bypass delay
 *                      0x4000: bypass trim
 *                      0x2000: bypass post-processing
 *                      0x1000: bypass bass management
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspBypassPostProcess(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2BypassMode)
{
    UINT16 u2Mode;
    UINT16 u2ShmIndex = W_PROCMOD;
    UINT32 u2UopIndex = UOP_DSP_BYPASS_PROC_MODE;

#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    //u2Mode = fgBypass ? 0xfc00 : 0xc00;
    if (u2BypassMode == 1)
    {
        u2Mode = 0xf000;
    }
    else
    {
        u2Mode = (u2BypassMode & 0xf000);
    }

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = W_PROCMOD_DEC2;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC2;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = W_PROCMOD_DEC3;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC3;
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndex = W_PROCMOD_DEC4;
        u2UopIndex = UOP_DSP_BYPASS_PROC_MODE_DEC4;
    }
#endif

    vWriteShmUINT16(u1DspId, u2ShmIndex, ((u2ReadShmUINT16(u1DspId, u2ShmIndex)&0x0fff) | u2Mode));
    DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
}

//-----------------------------------------------------------------------------
/** AUD_DspSoundEffectFlag
 *  Enable all audio sound effect. (EQ, Bass/Treble, Reverb, Surround, AVC)
 *
 *  @param  u1DecId  0: first decoder 1: seconder decoder.
 *  @param  fgFlag     0: disable, 1: enable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSoundEffectFlag(UINT8 u1DecId, BOOL fgFlag)
{
    UNUSED(u1DecId);

    AUD_DspChannelEqEnable(AUD_DEC_MAIN, fgFlag);
    AUD_DspBassBoostEnable(AUD_DEC_MAIN, fgFlag);
    AUD_DspClearBoostEnable(AUD_DEC_MAIN, fgFlag);
    AUD_DspReverbEnable(AUD_DEC_MAIN, fgFlag);
    AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, fgFlag);
    AUD_DspAVCEnable(AUD_DEC_MAIN, fgFlag);
}

//-----------------------------------------------------------------------------
/** AUD_DspIECConfig
 *  Routine handling IEC Configuration.
 *
 *  @param  eIecCfg  0: PCM; 1:RAW.
 *  @param  fgEnable  on/off.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspIECConfig(AUD_IEC_T eIecCfg, BOOL fgEnable)
{
	
    VERIFY(x_sema_lock(_ahSpdifCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    _aafgMWSetSpdifFlag = fgEnable;
    _aeMWIec = eIecCfg;
    _AudDspSetIec(_aeMWIec, fgEnable);
    VERIFY(x_sema_unlock(_ahSpdifCtlSema) == OSR_OK);

    return TRUE;
}

BOOL _AUD_GetDspIECConfig(void)
{
    return _aeMWIec;
} 

//-----------------------------------------------------------------------------
/** _AUD_SetSPDIFEnable
 *  @param  fgEnable  on/off.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_SetSPDIFEnable(BOOL fgEnable, BOOL fgLight)
{
    LOG(0, "Output Mute SPDIF     , MuteEnable(%d)\n", !fgEnable);
    VERIFY(x_sema_lock(_ahSpdifCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    if(fgEnable)
    {
        AudAoutGainCtrl(0,7,0x1000,AOUT_VOL_SETP_4DB);
        AudAoutGainCtrl(0,8,0x1000,AOUT_VOL_SETP_4DB);
    }
    else
    {
        AudAoutGainCtrl(0,7,0,AOUT_VOL_SETP_4DB);
        AudAoutGainCtrl(0,8,0,AOUT_VOL_SETP_4DB);
        x_thread_delay(50);
    }
    _aafgMWSetSpdifFlag = fgEnable;
   // _AudDspSetIec(_aeMWIec, fgEnable);
    _AudSetSPDIFEnable(fgEnable, fgLight);
    VERIFY(x_sema_unlock(_ahSpdifCtlSema) == OSR_OK);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_IsIECRaw
 *  Is IEC output RAW data.
 *
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_IsIECRaw(void)
{
    return ((_aeIecMode ==  AUD_IEC_CFG_RAW) & (_afgIecEnable == TRUE));
}


//-----------------------------------------------------------------------------
/** _AUD_DspSetIEC
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId
 *  @retval  N/A
 *
 *  Note: This function may be called twice. It can be done by more smart way
 *  if we can get decoder format early. New control flow is needed for better performance.
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetIEC(UINT8 u1DecId)
{
    UNUSED(u1DecId);

    _AUD_DspIECConfig(_aeMWIec, _aafgMWSetSpdifFlag);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetIecRawFlag
 *  Enable/disable sync IEC RAW delay to channel delay automatically.
 *
 *  @param  u1Flag
 *  @retval  N/A
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetIecRawFlag (UINT8 u1Flag)
{
    vWriteShmUINT8(AUD_DSP0, B_IECRAW_DELAY_FLAG, u1Flag);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_IEC_FLAG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetIecRawDelay
 *  Set IEC RAW delay.
 *
 *  @param  i2Delay (ms)
 *  @retval  N/A
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetIecRawDelay (INT16 i2Delay)
{
    vWriteShmUINT16(AUD_DSP0, W_IEC_RAW_OFFSET, (UINT16)i2Delay);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_IEC_FLAG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspIecChannel
 *  Routine handling IEC Configuration.
 *
 *  @param  eIecChannel  select audio channel,ex L/R.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspIecChannel(AUD_IEC_CH_T eIecChannel)
{
    vWriteShmUINT8(AUD_DSP0, B_IEC_PCMCH, (UINT8)eIecChannel);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_IEC_FLAG);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DspResetSpdifReg
 *   get SPDIF register type.
 *
 *  @param  SPDIF_REG_TYPE_T
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspResetSpdifReg (SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    _aSpdifInfo.u1Copyright[type][eStreamFrom] = _aSpdifInfo.u1Copyright[SPDIF_REG_TYPE_DEFAULT][eStreamFrom];
    _aSpdifInfo.u1Category[type][eStreamFrom] = _aSpdifInfo.u1Category[SPDIF_REG_TYPE_DEFAULT][eStreamFrom];
    _aSpdifInfo.u1WordLength[type][eStreamFrom] = _aSpdifInfo.u1WordLength[SPDIF_REG_TYPE_DEFAULT][eStreamFrom];
    _aSpdifInfo.eUpdateMode[type][eStreamFrom] = _aSpdifInfo.eUpdateMode[SPDIF_REG_TYPE_DEFAULT][eStreamFrom];

    _AUD_DspSetSpdifChannelStatus (type);
}

#ifndef CC_AUD_SUPPORT_SPDIF_V20
//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifRegType
 *   get SPDIF register type.
 *
 *  @param  SPIDF_REG_TYPE_T
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifRegType (SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    UNUSED(type);
    UNUSED(eStreamFrom);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifChannelStatus
 *   get SPDIF register type.
 *
 *  @param  SPDIF_REG_TYPE_T
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifChannelStatus (SPDIF_REG_TYPE_T type)
{
    AUD_DEC_STREAM_FROM_T eStreamFrom = _aeStreamSrc[AUD_DEC_MAIN]; // Now, it is only for main decoder.

    if (type >= SPDIF_REG_TYPE_NUM)
    {
        type = SPDIF_REG_TYPE_DEFAULT;
    }
    if (eStreamFrom >= AUD_STREAM_FROM_NUM)
    {
        eStreamFrom = AUD_STREAM_FROM_OTHERS;
    }

    vWriteShmUINT8(AUD_DSP0, B_IEC_COPYRIGHT, _aSpdifInfo.u1Copyright[type][eStreamFrom]);
    vWriteShmUINT8(AUD_DSP0, B_IEC_CATEGORY_CODE, _aSpdifInfo.u1Category[type][eStreamFrom]);
    vWriteShmUINT8(AUD_DSP0, B_IEC_WORD_LENGTH, _aSpdifInfo.u1WordLength[type][eStreamFrom]);

    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_IEC_SET_DATA);
}

#ifdef CC_AUD_ARM_RENDER

void _AUD_ArmSetSpdifChannelStatus (SPDIF_REG_TYPE_T type)
{
	AUD_DEC_STREAM_FROM_T eStreamFrom = _aeStreamSrc[AUD_DEC_MAIN]; // Now, it is only for main decoder.
	APROC_IEC_CHANNELSTATUS_T  eInfo;

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_CHANNELSTATUS, (UINT32 *) &eInfo, 10); 

	eInfo.u4CategoryCode= _aSpdifInfo.u1Category[type][eStreamFrom];
	eInfo.u4WordLength = _aSpdifInfo.u1WordLength[type][eStreamFrom];
	eInfo.u4CopyRight = _aSpdifInfo.u1Copyright[type][eStreamFrom];

	_vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_CHANNELSTATUS, (UINT32 *) &eInfo, 10); 

}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifCopyright
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifCopyright(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 copyright)
{
    _aSpdifInfo.u1Copyright[type][(UINT8)eStreamFrom] = copyright;

    LOG (3, "SPDIF set copyright (%d) = %d for src = %d\n", type, copyright, eStreamFrom);

    // setup register type, setup will be feed into DSP....
    _AUD_DspSetSpdifChannelStatus (type);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifCategoryCode
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifCategoryCode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 categoryCode)
{
    _aSpdifInfo.u1Category[type][(UINT8)eStreamFrom] = categoryCode;

    LOG (3, "SPDIF set category code (%d) = %d for src = %d\n", type, categoryCode, eStreamFrom);

    // setup register type, setup will be feed into DSP....
    #ifdef CC_AUD_ARM_RENDER
      _AUD_ArmSetSpdifChannelStatus(type);
    #else
      _AUD_DspSetSpdifChannelStatus (type);
    #endif

}

//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifWordLength
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifWordLength(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 wordLength)
{
    _aSpdifInfo.u1WordLength[type][(UINT8)eStreamFrom] = wordLength;

    LOG (3, "SPDIF set word length (%d) = %d for src = %d\n", type, wordLength, eStreamFrom);

    // setup register type, setup will be feed into DSP....
    _AUD_DspSetSpdifChannelStatus (type);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetSpdifUpdateMode
 *  Routine handling SPDIF Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSpdifUpdateMode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 eUpdate)
{
    _aSpdifInfo.eUpdateMode[type][(UINT8)eStreamFrom] = eUpdate;

    LOG (3, "SPDIF set update mode (%d) = %d for src = %d\n", type, eUpdate, eStreamFrom);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  copyright value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSpdifCopyright(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _aSpdifInfo.u1Copyright[type][(UINT8)eStreamFrom];
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifCopyright
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  category code value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSpdifCategoryCode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _aSpdifInfo.u1Category[type][(UINT8)eStreamFrom];
}


//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifWordLength
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  word length value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSpdifWordLength(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _aSpdifInfo.u1WordLength[type][(UINT8)eStreamFrom];
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifupdateMode
 *  Routine handling IEC Configuration.
 *
 *  @param  eStreamFrom     input source
 *
 *  @retval  word length value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSpdifupdateMode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    return _aSpdifInfo.eUpdateMode[type][(UINT8)eStreamFrom];
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifOutputMode
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId     decoder ID
 *
 *  @retval  value
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSpdifOutputMode(UINT8 u1DecId)
{
    UNUSED (u1DecId);

    return uReadShmUINT8(AUD_DSP0, B_IECFLAG);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetSpdifChannelStatus
 *  Routine handling get channel status.
 *
 *  @param
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspGetSpdifChannelStatus(SPDIF_CHANNEL_STATUS_T *rInfo)
{
    UINT8 u1WordLen, u1SampleRate;

    // get word length
    switch (uReadShmUINT8 (AUD_DSP0, B_IEC_WORD_LENGTH))
    {
    case IEC_WORDLENGTH_24BITS:
        u1WordLen = SPDIF_WORDLENGTH_MAX24_24BITS;
        break;
    case IEC_WORDLENGTH_20BITS:
        u1WordLen = SPDIF_WORDLENGTH_MAX24_20BITS;
        break;
    case IEC_WORDLENGTH_16BITS:
        u1WordLen = SPDIF_WORDLENGTH_MAX20_16BITS;
        break;
    default:
        u1WordLen = SPDIF_WORDLENGTH_MAX20_NOT_INDICATE;
        break;
    }

    // get sample rate
    switch (AUD_GetSampleFreq(AUD_DEC_MAIN)) // always main in current solution
    {
    case FS_22K:
        u1SampleRate = SPDIF_SAMPLE_RATE_22KHZ;
        break;
    case FS_44K:
        u1SampleRate = SPDIF_SAMPLE_RATE_44KHZ;
        break;
    case FS_88K:
        u1SampleRate = SPDIF_SAMPLE_RATE_88KHZ;
        break;
    case FS_176K:
        u1SampleRate = SPDIF_SAMPLE_RATE_176KHZ;
        break;
    case FS_24K:
        u1SampleRate = SPDIF_SAMPLE_RATE_24KHZ;
        break;
    case FS_48K:
        u1SampleRate = SPDIF_SAMPLE_RATE_48KHZ;
        break;
    case FS_96K:
        u1SampleRate = SPDIF_SAMPLE_RATE_96KHZ;
        break;
    case FS_192K:
        u1SampleRate = SPDIF_SAMPLE_RATE_192KHZ;
        break;
    case FS_32K:
        u1SampleRate = SPDIF_SAMPLE_RATE_32KHZ;
        break;
    default:
        u1SampleRate = SPDIF_SAMPLE_RATE_NOT_INDICATE;
        break;
    }

    rInfo->u1Mode = 0; // consumer mode only
    rInfo->u1SampleMode = uReadShmUINT8 (AUD_DSP0, B_IECFLAG);
    rInfo->u1Copyright = uReadShmUINT8 (AUD_DSP0, B_IEC_COPYRIGHT);
    rInfo->u1PreEmphasis = 0;
    rInfo->u1Category = uReadShmUINT8 (AUD_DSP0, B_IEC_CATEGORY_CODE);
    rInfo->u1SourceNum = 0;
    rInfo->u1CannelNum = 0;
    rInfo->u1SampleRate = u1SampleRate;
    rInfo->u1ClockAccuray = 0;
    rInfo->u1WordLength = u1WordLen;
    rInfo->u1OrgSampleRate = 0;

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DspSpdifSetInfo
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Enable 0: disable 1: enable
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspSpdifSetInfo(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1CopyRight, UINT8 u1CategoryCode)
{
    _aSpdifInfo.u1Copyright[type][(UINT8)eStreamFrom] = u1CopyRight;
    _aSpdifInfo.u1Category[type][(UINT8)eStreamFrom] = u1CategoryCode;

   // setup register type, setup will be feed into DSP....
    _AUD_DspSetSpdifChannelStatus (type);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DspSpdifSetCopyProtect
 *  Routine handling IEC Configuration.
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Enable 0: disable 1: enable
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
#define IEC_CATEGORY_LBIT_NUMBER 7
#define IEC_CATEGORY_LBIT (1 << IEC_CATEGORY_LBIT_NUMBER)
#define IEC_CATEGORY_LBIT_MASK (~IEC_CATEGORY_LBIT)

BOOL _AUD_DspSpdifSetCopyProtect(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, BOOL bCpBit, BOOL bLBit)
{
    UINT8 u1CategoryCode, u1CopyRight;

    u1CopyRight = (UINT8) bCpBit;
    u1CategoryCode = AUD_DspGetSpdifCategoryCode (type, eStreamFrom) & IEC_CATEGORY_LBIT_MASK;
    if (bLBit)
    {
        u1CategoryCode |= IEC_CATEGORY_LBIT;
    }

    _aSpdifInfo.u1Copyright[type][(UINT8)eStreamFrom] = u1CopyRight;
    _aSpdifInfo.u1Category[type][(UINT8)eStreamFrom] = u1CategoryCode;

   // setup register type, setup will be feed into DSP....
    _AUD_DspSetSpdifChannelStatus (type);

    return TRUE;
}

#ifdef CC_AUD_DDI
void _AUD_DspSetSoundBarOnOff(BOOL fgOnOff)
{
    if(fgOnOff)//open
    {

        vWriteShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR_ID, SpdifSoundBarId);
        vWriteShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR_DATA, SpdifSoundBarData);
        //vWriteShmUINT8(B_IEC_SOUNDBAR_FLAG, 1);
            
        //DSP_SendDspTaskCmd(UOP_DSP_IEC_SET_DATA);
        //_AUD_DspIecChannel(AUD_IEC_PCM_CH_L_R);
    }
    else//else
    {

        vWriteShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR_ID, 0);
        vWriteShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR_DATA, 0);
        //vWriteShmUINT8(B_IEC_SOUNDBAR_FLAG, 0);
    
        //DSP_SendDspTaskCmd(UOP_DSP_IEC_SET_DATA);
        //_AUD_DspIecChannel(AUD_IEC_PCM_CH_7_8);

    }
}

static UINT8 _AUD_CaclCheckSum(UINT8 *pu1Data, UINT32 u4Len)
{
    UINT8 i; 
    UINT8 u1CheckSum = 0;

    while (u4Len--)
    {
        u1CheckSum ^= *pu1Data++;
        for (i = 0; i < 8; i++)
        {
            if (u1CheckSum & 0x1)
            {
                u1CheckSum = (u1CheckSum >> 1) ^ 0x80;
            }
            else
            {
                u1CheckSum >>= 1;
            }
        } 
    }
    return u1CheckSum;
}

void _AUD_DspSetSoundBarIDData(UINT32 Id, UINT8 data, UINT8 volumeInfo)
{
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    UINT32 u4ChCfg2, u4ChCfg3, u4ChCfg4, u4ChCfg5, u4ChCfg6, u4ChCfg7;
    UINT32 u4Temp;
    UINT32 u4Data = (UINT32)data;
    UINT32 u4Volome = (UINT32)volumeInfo;
    UINT32 u4CheckSum = 0;
    UINT32 u4CheckData[2];
        
    u4Temp = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG2));
    u4ChCfg2 = (u4Temp & 0xFFF) | ((Id & 0xF) << 12); //ID[3:0]
    u4ChCfg3 = (Id >> 4) & 0xFFFF; //ID[19:4]
    u4ChCfg4 = ((Id >> 20) & 0xF) | //ID[23:20]
               ((u4Volome & 0xff) << 4) | //volume[7:0]
               ((u4Data & 0xF) << 12); //cmd[3:0]
    u4ChCfg5 = (u4Data>>4) & 0xF;

    u4CheckData[0] = (Id & 0xFFFFFF) | (u4Volome << 24);
    u4CheckData[1] = (u4Data & 0xFF);
    u4CheckSum = _AUD_CaclCheckSum((UINT8 *)u4CheckData, 8);    
    u4ChCfg6 = (u4CheckSum & 0x0F) << 12; //checksum
    u4ChCfg7 = (u4CheckSum & 0xF0) >> 4; //checksum
    

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG2), u4ChCfg2);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG3), u4ChCfg3);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG4), u4ChCfg4);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG5), u4ChCfg5);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG6), u4ChCfg6);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG7), u4ChCfg7);
#else
    SpdifSoundBarId = Id;
    SpdifSoundBarData = data;

    vWriteShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR_ID, Id);
    vWriteShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR_DATA, data);
#ifdef CC_AUD_DDI
    AUD_SendSoundBarCmd((UINT32)volumeInfo);
#endif
#endif
}

void _AUD_DspGetSoundBarStatus(UINT8 *pId, UINT8 *pdata)
{
    *pId = uReadShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR_ID);
    *pdata = uReadShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR_DATA);
}

void _AUD_UserSetSpdifRawDec(UINT8 u1DecId)
{
   _u1SpdifRawDec = u1DecId;
}
#endif

#ifdef CC_AUD_MIXSOUND_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspMixSndEnable
 *  Mix sound enable.
 *
 *  @param  fgEnable TRUE(enable) FALSE(disable).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMixSndEnable(BOOL fgEnable)
{
    _fgMixSndFlag = fgEnable;
}

//-----------------------------------------------------------------------------
/** AUD_DspMixSndMute
 *  Mix sound mute.
 *
 *  @param  fgMutre TRUE(mute) FALSE(un-mute).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMixSndMute(BOOL fgMute)
{
    _fgMixSndMute = fgMute;
}

#ifndef NEW_MIXSOUND
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
extern BOOL fgAudFeedMixSndThreadEnable; //FLASHLITE_CONFLICT
#endif
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspMixSndControl
 *  Mix sound control.
 *
 *  @param  u1Mode play/stop.
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
void _AUD_DspMixSndControl(UINT16 u2Mode)
{
#ifndef NEW_MIXSOUND
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY //FLASHLITE_CONFLICT
    if (fgAudFeedMixSndThreadEnable)
        return;
#endif
#endif

    // Trigger play only when clip has ever been selected
    if ((_u1MixSndClipIdx != (UINT8)0xff) && (_fgMixSndFlag) && (!_fgMixSndMute))
    {
        vWriteShmUINT16(AUD_DSP0, W_MIXSOUND_FLAG, u2Mode);
        DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_MIXSOUND_FLAG);
    }
    else
    {
        LOG(3, "ERROR: no mixsnd clip selected\n");
    }
}

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY //FLASHLITE_CONFLICT
void _AUD_DspMixSndControl2(UINT16 u2Mode)
{
    vWriteShmUINT16(AUD_DSP0, W_MIXSOUND_FLAG, u2Mode);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_MIXSOUND_FLAG);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspMixSndData
 *  Select mix sound data source.
 *
 *  @param  u1ClipIdx 0~3
 *  @retval  TRUE
 */
//-----------------------------------------------------------------------------
void _AUD_DspMixSndClip(UINT8 u1ClipIdx)
{
#ifdef CC_AUD_APROC_KEYSOUND
    _u1MixSndClipIdx = u1ClipIdx;
    AUD_AprocEnableKeySnd();
#else
    UINT32 u4Idx;
    UINT32 *pu4MixSndTab, u4TabSize;

    if (_u1MixSndClipIdx == u1ClipIdx)
    {
        return;
    }

    if (u1ClipIdx > AUD_MIXSND_CLIP_NUM)
    {
        u1ClipIdx = AUD_MIXSND_CLIP0;
    }

    if (u1ClipIdx == AUD_MIXSND_CLIP0)
    {
        u4TabSize = sizeof(_au4MixSndData0);
        pu4MixSndTab = (UINT32 *)&_au4MixSndData0[0];
        vWriteShmUINT32 (AUD_DSP0, D_MIXSOUND_PLEN, ((u4TabSize/2)*_u4MixsoundLoop));
        vWriteShmUINT16 (AUD_DSP0, W_MIXSOUND_CFG, 0x1f00); //mono
    }
    else if (u1ClipIdx == AUD_MIXSND_CLIP1)
    {
        u4TabSize = sizeof(_au4MixSndData0_Stereo);
        pu4MixSndTab = (UINT32 *)&_au4MixSndData0_Stereo[0];
        vWriteShmUINT32 (AUD_DSP0, D_MIXSOUND_PLEN, ((u4TabSize/2/2)*_u4MixsoundLoop));
        vWriteShmUINT16 (AUD_DSP0, W_MIXSOUND_CFG, 0x1f10); //stereo
    }
    else if (u1ClipIdx == AUD_MIXSND_CLIP2)
    {
        u4TabSize = sizeof(_au4MixSndData0_Stereo_LOnROff);
        pu4MixSndTab = (UINT32 *)&_au4MixSndData0_Stereo_LOnROff[0];
        vWriteShmUINT32 (AUD_DSP0, D_MIXSOUND_PLEN, ((u4TabSize/2/2)*_u4MixsoundLoop));
        vWriteShmUINT16 (AUD_DSP0, W_MIXSOUND_CFG, 0x1f10); //stereo
    }
    else
    {
        u4TabSize = sizeof(_au4MixSndData0);
        pu4MixSndTab = (UINT32 *)&_au4MixSndData0[0];
        vWriteShmUINT32 (AUD_DSP0, D_MIXSOUND_PLEN, ((u4TabSize/2)*_u4MixsoundLoop));
        vWriteShmUINT16 (AUD_DSP0, W_MIXSOUND_CFG, 0x1f00); //mono
    }

#ifndef NEW_MIXSOUND
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
    if (!fgAudFeedMixSndThreadEnable) //FLASHLITE_CONFLICT
    {
#endif
#endif
        for (u4Idx=0; u4Idx<_u4MixsoundLoop; u4Idx++)
        {
            x_memcpy((VOID *)VIRTUAL((UINT32)_u4MixSndBuf+(u4TabSize*u4Idx)),
                (const VOID *)pu4MixSndTab, u4TabSize);
        }

        DSP_FlushInvalidateDCacheFree( _u4MixSndBuf, u4GetMixSoundBufSize());
#ifndef NEW_MIXSOUND
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
    } //FLASHLITE_CONFLICT
#endif
#endif

    _u1MixSndClipIdx = u1ClipIdx;
#endif
}


//-----------------------------------------------------------------------------
/** _AUD_DspGetMixSndEnable
 *  Get mix sound enable.
 *
 *  @param  void.
 *  @retval  TRUE(enable) FALSE(disable).
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspGetMixSndEnable(void)
{
    return _fgMixSndFlag;
}


//-----------------------------------------------------------------------------
/** _AUD_DspGetMixSndClipIdx
 *  Get mix sound clip index.
 *
 *  @param   void.
 *  @retval  clip index.
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetMixSndClipIdx(void)
{
    return _u1MixSndClipIdx;
}
#endif


//-----------------------------------------------------------------------------
/** _AUD_GetVideoScrambleStatus
 *  Get VideoScrambleStatus
 *
 *  @param  void.
 *  @retval  TRUE(enable) FALSE(disable).
 */
//-----------------------------------------------------------------------------
BOOL _AUD_GetVideoScrambleStatus(void)
{
    return _fgVideoScrambled;
}

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspSetDemodOutputMode
 *  Set MTS output mode.
 *
 *  @param  u1Output    0: Output (Mono,Mono)
 *                               1: If stereo exists, Output (L,R) otherwise (Mono,Mono)
 *                               2: If SAP exists, Output (S,S) otherwise (L,R) otherwise (Mono,Mono)
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDemodOutputMode(UINT8 u1Output)
{
    DSP_SetDemodOutputMode(u1Output);
}
#endif

#ifdef DSP_SUPPORT_SRSTSXT
void _AUD_DspSetSRSTSXTSpkSz(UINT8 u1Mode)
{
    DSP_SRSTSXTSetSpkSize(u1Mode);
}

void _AUD_DspSetSRSTSXTSetMode(UINT8 u1Mode)
{
    DSP_SRSTSXTSetMode(u1Mode);
}

void _AUD_DspSetSRSTSXTElev(UINT8 u1Mode)
{
    DSP_SRSTSXTElev(u1Mode);
}
void _AUD_DspSetSRSTSXTinputgain(UINT32 u4Mode)
{
    DSP_SRSTSXTinputgain(u4Mode);
}
void _AUD_DspSetSRSTSXTTBLvl(UINT8 u1Mode)
{
    DSP_SRSTSXTTBLvl(u1Mode);
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetDetectDefaultMode
 *   call this function before issue detection command.
 *
 *  @param  u1Mode 0: PAL_DK, 4: SECAM-L.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDetectDefaultMode(UINT8 u1Mode)
{
    vWriteShmUINT8(AUD_DSP0, B_DETECT_DEFAULT, u1Mode);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetUserDefinedEqCfg
 *   setup user difined EQ config.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  u1BandIdx  0~4.
 *  @param  i1Value -100 ~ +100
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetUserDefinedEqCfg(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8 i1Value)
{

    UNUSED(u1DecId);

    if (u1BandIdx >= CH_EQ_BAND_NO)
    {
        return;
    }

    if (eEqType >= AUD_EQ_NUM)
    {
        return;
    }

    if (i1Value > EQ_BAND_GAIN_SIDE_INDEX_MAX)
    {
        i1Value = EQ_BAND_GAIN_SIDE_INDEX_MAX;
    }

    if (i1Value < -EQ_BAND_GAIN_SIDE_INDEX_MAX)
    {
        i1Value = -EQ_BAND_GAIN_SIDE_INDEX_MAX;
    }
    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    _aai1ChEqCfg[eEqType][u1BandIdx + 1] = i1Value;
    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
    if (_eEqMode == eEqType)
    {
        // FIXME !!
        AUD_DspChEqLoadPreset(AUD_DEC_MAIN, eEqType);
    }

}

//-----------------------------------------------------------------------------
/** AUD_DspSetUserDefinedEqCfg
 *   Get user difined EQ config.
 *
 *  @param  eEqType  0: off   others: 1~12.
 *  @param  u1BandIdx  0~4.
 *  @param  i1Value (-20 ~ +20)
 *
 *  @retval  TRUE: success
 *              FALSE: failed
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspGetUserDefineEqCfg(AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8* pi1Value)
{
    if (u1BandIdx >= CH_EQ_BAND_NO)
    {
        return FALSE;
    }
    if (eEqType >= AUD_EQ_NUM)
    {
        return FALSE;
    }

    if (pi1Value != NULL)
    {
        VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        *pi1Value = _aai1ChEqCfg[eEqType][u1BandIdx + 1] ;
        VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetUserEqBandNum
 *   Get user defined band number.
 *
 *  @retval  CH_EQ_BAND_NO = 5.
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetUserEqBandNum(void)
{
    return CH_EQ_BAND_NO;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetEqBandInfo
 *   Get user defined band info.
 *
 *  @param  prEqBandInfo  structure of eq band info (AUD_EQ_BAND_INFO_T).
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetEqBandInfo(AUD_EQ_BAND_INFO_T* prEqBandInfo)
{
    UINT8 u1BandIdx;
    if (prEqBandInfo != NULL)
    {
        u1BandIdx = prEqBandInfo->ui1_band_idx;
        if (u1BandIdx>=5) return;   //Currently 5 sets of EQ coeffs are defined.
        x_memcpy((VOID *)VIRTUAL((UINT32)prEqBandInfo), (const VOID *)&_arEqInfo[u1BandIdx], sizeof(AUD_EQ_BAND_INFO_T));
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSrcVol
 *   get source volume.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media
 *  @retval  input source volume
 */
//-----------------------------------------------------------------------------
INT16 _AUD_DspGetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc)
{
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return 0;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif
    if (eStrmSrc <AUD_STREAM_FROM_NUM)
    {
        return _aai2InputSrcVol[u1DecId][AUD_EU_ATV_STREAM_SRC_TRANSFER(eStrmSrc)];
    }
    else
    {
        LOG(3, "Invalid eStrmSrc in AUD_DspGetSrcVol\n");
        return 0;
    }
}
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
//-----------------------------------------------------------------------------
/** _AUD_DspLinoutVol
 *   set source volume.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media

 *  @param  i2Vol  -128 ~ 96
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSrcLinoutVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol)
{
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif

    if (i2Vol >= (INT16)INPUT_SRC_VOL_MAX)
    {
        i2Vol = INPUT_SRC_VOL_MAX;
    }
    else if (i2Vol <= (INT16)INPUT_SRC_VOL_MIN)
    {
        i2Vol = INPUT_SRC_VOL_MIN;
    }

    if (eStrmSrc <AUD_STREAM_FROM_NUM)
    {
        VOL_CTL_SEMA_LOCK(u1DecId);
        _aai2InputSrcLineOutVol[u1DecId][AUD_EU_ATV_STREAM_SRC_TRANSFER(eStrmSrc)] = i2Vol;
        _AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_LEFT);
        _AUD_DspVolumeChange(u1DecId, AUD_CH_REAR_RIGHT);
        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }
    else
    {
        LOG(3, "Invalid eStrmSrc in _AUD_DspLinoutVol\n");
    }
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetSrcVol
 *   set source volume.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media

 *  @param  i2Vol  -128 ~ 96
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol)
{
#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    if (i2Vol >= (INT16)INPUT_SRC_VOL_MAX)
    {
        i2Vol = INPUT_SRC_VOL_MAX;
    }
    else if (i2Vol <= (INT16)INPUT_SRC_VOL_MIN)
    {
        i2Vol = INPUT_SRC_VOL_MIN;
    }

    if (eStrmSrc <AUD_STREAM_FROM_NUM)
    {
        VOL_CTL_SEMA_LOCK(u1DecId);
        _aai2InputSrcVol[u1DecId][AUD_EU_ATV_STREAM_SRC_TRANSFER(eStrmSrc)] = i2Vol;
        _AUD_DspVolumeChange(AUD_DSP0, u1DecId, AUD_CH_INPUT_SRC);
        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }
    else
    {
        LOG(3, "Invalid eStrmSrc in AUD_DspSetSrcVol\n");
    }
}

#ifdef CC_AUD_INPUT_SRC_VOL_OFFSET
//-----------------------------------------------------------------------------
/** AUD_DspGetSrcVolOffset
 *   get source volume offset.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media
 *  @retval  input source volume offset
 */
//-----------------------------------------------------------------------------
INT16 _AUD_DspGetSrcVolOffset(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc)
{
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return 0;
    //End
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#endif
    if (eStrmSrc <AUD_STREAM_FROM_NUM)
    {
        return _aai2InputSrcVolOffset[u1DecId][AUD_EU_ATV_STREAM_SRC_TRANSFER(eStrmSrc)];
    }
    else
    {
        LOG(3, "Invalid eStrmSrc in AUD_DspGetSrcVolOffset\n");
        return 0;
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspSetSrcVolOffset
 *   set source volume offset.
 *
 *  @param  u1DecId  0:first decoder 1: seconder decoder.
 *  @param  eStrmSrc      AUD_STREAM_FROM_OTHERS = 0,         // ex. Pink Noise Generator.
                                   AUD_STREAM_FROM_DIGITAL_TUNER,      // Digital tuner.
                                   AUD_STREAM_FROM_ANALOG_TUNER,       // ex. Analog tuner.
                                   AUD_STREAM_FROM_SPDIF,              // SPDIF-in.
                                   AUD_STREAM_FROM_LINE_IN,            // Line in.
                                   AUD_STREAM_FROM_HDMI,               // HDMI in.
                                   AUD_STREAM_FROM_MEMORY,             // Memory.
                                   AUD_STREAM_FROM_MULTI_MEDIA,             // Multi-media

 *  @param  i2Vol  -128 ~ 96
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSrcVolOffset(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol)
{
#if defined(CC_AUD_4_DECODER_SUPPORT)
    AUD_DEC_ID_VALIDATE(u1DecId);
#elif defined(CC_MT5391_AUD_3_DECODER)
    AUD_DEC_ID_VALIDATE_3(u1DecId);
#else
    AUD_DEC_ID_VALIDATE_2(u1DecId);
#endif

    eStrmSrc = AUD_EU_ATV_STREAM_SRC_TRANSFER(eStrmSrc);
    if ((_aai2InputSrcVol[u1DecId][eStrmSrc] + i2Vol) >= (INT16)INPUT_SRC_VOL_MAX)
    {
        i2Vol = INPUT_SRC_VOL_MAX - _aai2InputSrcVol[u1DecId][eStrmSrc];
    }
    else if ((_aai2InputSrcVol[u1DecId][eStrmSrc] + i2Vol) <= (INT16)INPUT_SRC_VOL_MIN)
    {
        i2Vol = INPUT_SRC_VOL_MIN - _aai2InputSrcVol[u1DecId][eStrmSrc];
    }

    if (eStrmSrc <AUD_STREAM_FROM_NUM)
    {
        VOL_CTL_SEMA_LOCK(u1DecId);
        _aai2InputSrcVolOffset[u1DecId][eStrmSrc] = i2Vol;
        _AUD_DspVolumeChange(u1DecId, AUD_CH_INPUT_SRC);
        VOL_CTL_SEMA_UNLOCK(u1DecId);
    }
    else
    {
        LOG(3, "Invalid eStrmSrc in AUD_DspSetSrcVolOffset\n");
    }
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspSetAvSrcVol
 *   set AV source volume.
 *
 *  @param  eInputSrcId         AUD_INPUT_ID_COMP_VID_0 = 0,
                                          AUD_INPUT_ID_COMP_VID_1,
                                          AUD_INPUT_ID_COMP_VID_2,
                                          AUD_INPUT_ID_COMP_VID_3,
                                          AUD_INPUT_ID_S_VID_0,
                                          AUD_INPUT_ID_S_VID_1,
                                          AUD_INPUT_ID_S_VID_2,
                                          AUD_INPUT_ID_YPBPR_0,
                                          AUD_INPUT_ID_YPBPR_1,
                                          AUD_INPUT_ID_YPBPR_2,
                                          AUD_INPUT_ID_VGA_0,
                                          AUD_INPUT_ID_VGA_1,
                                          AUD_INPUT_ID_HDMI_0,
                                          AUD_INPUT_ID_HDMI_1,
                                          AUD_INPUT_ID_HDMI_2,
                                          AUD_INPUT_ID_HDMI_3,
                                          AUD_INPUT_ID_DVI_0,
                                          AUD_INPUT_ID_DVI_1,
                                          AUD_INPUT_ID_DVI_2,
                                          AUD_INPUT_ID_DVI_3,
                                          AUD_INPUT_ID_SCART_0,
                                          AUD_INPUT_ID_SCART_1,
                                          AUD_INPUT_ID_SCART_2,
                                          AUD_INPUT_ID_SCART_3,
                                          AUD_INPUT_ID_AUXIN_0,
                                          AUD_INPUT_ID_AUXIN_1,
 *  @param  i2Vol  -256 ~ 256.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetAvSrcVol(AUD_INPUT_ID_T eInputSrcId , INT16 i2Vol)
{
    if (eInputSrcId >= AUD_INPUT_ID_MAX)
    {
        return;
    }

    VOL_CTL_SEMA_LOCK(AUD_DEC_MAIN);

    _ai2AvInputSrcVol[eInputSrcId] = i2Vol;
    _AUD_DspVolumeChange(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_INPUT_SRC);

    VOL_CTL_SEMA_UNLOCK(AUD_DEC_MAIN);
}

AUD_INPUT_ID_T _AUD_DspGetAvInputSrcId(void)
{
    AUD_INPUT_ID_T eInputId;
    eInputId = AUD_GetAvInputSrcId();
    if (eInputId >= AUD_INPUT_ID_MAX)
    {
        LOG(0, "Incorrect AvInputSrcId!!!\n");
        ASSERT(0);
    }
    return eInputId;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAvInputSrcDvdId
 *
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
AUD_INPUT_ID_T _AUD_DspGetAvInputSrcDvdId(void)
{
    return (AUD_INPUT_ID_T)DRVCUST_OptGet(eAudioDvdInputChl);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAtvTvSys
 *
 *  @param  prTvAudSys  tv audio system structure.
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetAtvTvSys(MW_TV_AUD_SYS_T * prTvAudSys)
{
    if (prTvAudSys != NULL)
    {
        DSP_GetAtvTvSys(prTvAudSys);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAtvTvSystem
 *
 *  @param  void
 *  @retval  TV_AUD_SYS_T
 */
//-----------------------------------------------------------------------------
TV_AUD_SYS_T _AUD_DspGetAtvTvSystem(void)
{
    return DSP_GetAtvTvSystem();
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetAtvTvSystemStrength
 *
 *  @param  TV_AUD_SYS_T
 *  @retval  UINT32
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetAtvTvSystemStrength(TV_AUD_SYS_T eAudSys)
{
    return (UINT32)DSP_GetAtvTvSystemStrength(eAudSys);
}

#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
void _AUD_SetTVSysFineTuneVol(TV_AUD_SYS_T eAudSys)
{
    UINT8 u1Value;

    if ( eAudSys >= SV_AUD_SYS_NUM )
        eAudSys = SV_NONE_DETECTED;

    u1Value = _aau1TVFineTuneVolume[eAudSys];

    VERIFY (u1Value < sizeof(VOLUME_FINE_TUNE)/4);

    switch(eAudSys)
    {
    case SV_A2_BG:
    case SV_A2_DK1:
    case SV_A2_DK2:
    case SV_A2_DK3:
       vWriteShmUINT32(D_A2_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
       break;
    case SV_PAL_I:
    case SV_PAL_BG:
    case SV_PAL_DK:
       vWriteShmUINT32(D_PAL_FINE_TUNE_VOLUME, ((UINT32) VOLUME_FINE_TUNE[u1Value] << 8));
       break;
    default:
       break;
    }
}
#endif
//-----------------------------------------------------------------------------
/** AUD_DspSetTvSrcMode
 *   set tv source mode.
 *
 *  @param  eAudSys      SV_NONE_DETECTED = 0x0,
                                   SV_MTS           = 0x1,
                                   SV_FM_FM         = 0x2,
                                   SV_NTSC_M        = 0x3,
                                   SV_A2_BG         = 0x4,
                                   SV_A2_DK1        = 0x5,
                                   SV_A2_DK2        = 0x6,
                                   SV_A2_DK3        = 0x7,
                                   SV_PAL_I         = 0x8,
                                   SV_PAL_BG        = 0x9,
                                   SV_PAL_DK        = 0xa,
                                   SV_SECAM_L       = 0xb,
                                   SV_SECAM_L_PLUM  = 0xc,
                                   SV_SECAM_BG      = 0xd,
                                   SV_SECAM_DK      = 0xe
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetTvSrcMode(TV_AUD_SYS_T eAudSys)
{
    DSP_SetTvSrcMode(eAudSys);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetTvSrcMode
 *   get tv source mode.
 *
 *  @retval  eAudSys
 */
//-----------------------------------------------------------------------------
TV_AUD_SYS_T _AUD_DspGetTvSrcMode(void)
{
    return DSP_GetTvSrcMode();
}

//-----------------------------------------------------------------------------
/** AUD_DspSetAtvOutputMode
 *   set tv source mode.
 *
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *  @param  eSoundMode         UNKNOWN = 0,
                                           MONO,
                                           STEREO,
                                           SUB_LANG,
                                           DUAL1,
                                           DUAL2,
                                           NICAM_MONO,
                                           NICAM_STEREO,
                                           NICAM_DUAL1,
                                           NICAM_DUAL2,
                                           A2_DUAL1,
                                           A2_DUAL2,
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetAtvOutputMode(UINT8 u1DecId, AUD_SOUND_MODE_T eSoundMode)
{
    DSP_SetAtvOutputMode(u1DecId, eSoundMode);
}

BOOL _AUD_IsSIFExist(void)
{
    return u1IsSIFExist();
}

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspHdmiSpdifOutEnable
 *   SPDIF output control in HDMI mode.
 *
 *  @param  fgEnable    0: disable SPDIF output, 1: enable SPDIF output
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspHdmiSpdifOutEnable(BOOL fgEnable)
{
    VERIFY(x_sema_lock(_ahSpdifCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    _aafgHdmiSetSpdifFlag=fgEnable;
    _AudDspSetIec(_aeMWIec,fgEnable);
    VERIFY(x_sema_unlock(_ahSpdifCtlSema) == OSR_OK);
}
#endif

#if 0  // not used ??
static void _AudPlayMuteSetSPDIFEnable(BOOL fgEnable)
{
    UINT8 u1fgMute;
    
    UNUSED(_AudPlayMuteSetSPDIFEnable);
        //check HDMI's command
    if (_aeStreamSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_HDMI)
    {
        fgEnable = _aafgHdmiSetSpdifFlag && _aafgMWSetSpdifFlag && (!_afgIecMute);//&&_aafgPlayMuteSetSpdifFlag;
    }
    else
    {
        fgEnable = _aafgMWSetSpdifFlag && (!_afgIecMute);//&&_aafgPlayMuteSetSpdifFlag;
    }
    if (fgEnable)
    {
        u1fgMute = FALSE;
    }
    else
    {
        u1fgMute = TRUE;
    }
    vHDMISpdifOutEnable(fgEnable);
    if(_afgIecEnable != fgEnable)
    {
        vWriteShmUINT8(AUD_DSP0, B_IEC_MUTE, u1fgMute);
    }
    _afgIecEnable = fgEnable;
    AUD_SPDIF_Enable(fgEnable);
}
#endif

void _AUD_DspPlayMuteSpdifEnable(BOOL fgEnable)
{
    UINT32 u4IecConfig;

    //_aafgPlayMuteSetSpdifFlag=fgEnable;
    u4IecConfig = AUD_READ32(REG_SPOCFG0);
    if(!fgEnable)
    {
        AUD_WRITE32(REG_SPOCFG0, (u4IecConfig | SPOUT_MUTE));   // Bit[5]
    }
    else
    {
        AUD_WRITE32(REG_SPOCFG0, (u4IecConfig & (~SPOUT_MUTE)));    // Bit[5]
    }
}


//-----------------------------------------------------------------------------
/** AUD_DspGetSoundMode
 *   SPDIF output control in HDMI mode.
 *
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *
 *  @retval AUD_SOUND_MODE_T
 */
//-----------------------------------------------------------------------------
AUD_SOUND_MODE_T _AUD_DspGetSoundMode(UINT8 u1DecId)
{
    return DSP_GetSoundMode(u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSpectrumInfo
 *   Get audio spectrum information.
 *
 *  @param  u1DecId 0:first decoder 1: seconder decoder.
 *  @param  u1BandNum      1~15 .
 *  @retval  u4Spectrum
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetSpectrumInfo(UINT8 u1DecId, UINT8 u1BandNum)
{
    #ifndef CC_AUD_ARM_SUPPORT
    UINT32 prSpectrumAddr;
    #endif
    UINT32 u4Spectrum;
    static BOOL fgEnable = FALSE;

    if (!fgEnable)
    {
        AUD_DspSpectrumEnable(u1DecId, TRUE);
        fgEnable = TRUE;
    }
    
#ifdef CC_SUPPORT_STR
    if (fgEQSpetrumInfoStrResumeMode)
    {
        AUD_DspSpectrumEnable(u1DecId, TRUE);
        fgEnable = TRUE;
    }
#endif

    if ((u1BandNum < 1) || (u1BandNum > 15))
    {
        return 0;
    }
#ifdef CC_AUD_ARM_SUPPORT
    u4Spectrum = u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_0 + u1BandNum-1));
    u4Spectrum = u4Spectrum>>9;
#else
    prSpectrumAddr = DSP_VIRTUAL_ADDR(DSP_GetEQSpectrumAddr(u1DecId));

    //Band 1 start from <ADDR_D2RC_SPECTRUM_BASE> word 2
    prSpectrumAddr += (u1BandNum<<1)*4;

    if ((prSpectrumAddr&0x3) > 0) //address must be 4-alignment.
    {
        return 0;
    }

    u4Spectrum = *(UINT32*)prSpectrumAddr;
    u4Spectrum = u4Spectrum>>8;
#endif
    
    return  u4Spectrum;
}

//-----------------------------------------------------------------------------
/** AUD_DspSpectrumEnable
 *   Enable audio spectrum calculation.
 *
 *  @param  u1DecId    0:first decoder 1: seconder decoder.
 *  @param  fgEnable    0: disable calculation, 1: enable calculation.
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSpectrumEnable(UINT8 u1DecId, BOOL fgEnable)
{
#ifdef CC_AUD_ARM_SUPPORT
    UNUSED(u1DecId);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_ENABLE), (UINT32)fgEnable);
#else
    UINT16 u2Mode;
    UINT16 u2ShmIndex = W_PROCMOD;
    UINT32 u2UopIndex = UOP_DSP_PROCESSING_MODE;
#ifndef CC_MT5391_AUD_3_DECODER
    //20101018,By Daniel,for flash-lite clip using dec3 notify path
    if (u1DecId > AUD_DEC_AUX)
        return;
    //End
    if (u1DecId != AUD_DEC_MAIN)
#else
    if (u1DecId == AUD_DEC_AUX)
#endif
    {
        u2ShmIndex = W_PROCMOD_DEC2;
    }

    u2Mode = u2ReadShmUINT16(AUD_DSP0, W_PROCMOD)&(0xfffe);
    u2Mode |= (fgEnable);

    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Mode);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
#endif

}

/***************************************************************************
     Function : AUD_AudioSetStcDiffBound
  Description : Set STC PTS difference bound. Bound would be (x MS * (1+uStcDiff))
    Parameter : uStcDiff: 0 ~ 255  for low, high, and worst bound.
    Return    : None
    Andrew Wen 2007/2/2
***************************************************************************/
void _AUD_AudioSetStcDiffBound(UINT8 u1DecId, UINT8 uStcDiffLo, UINT8 uStcDiffHi, UINT8 uStcDiffWs)
{
    UINT32 u2UopIndex = UOP_DSP_SET_STC_DIFF_BOUND;
    UNUSED(u1DecId);

    DSP_SetStcDiffBound(uStcDiffLo, uStcDiffHi, uStcDiffWs);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}

/***************************************************************************
    Function : _AUD_VideoScrambled
    Description : Set Video Scramble Status
    Parameter : 1 : video srambled
    Return    : None
    Xiaowen Chen 2010/2/15
***************************************************************************/
void  _AUD_VideoScrambled(BOOL fgEnable)
{
    _fgVideoScrambled = fgEnable;

}

/***************************************************************************
    Function : AUD_DDBannerEnable
    Description : Turn on this funtion to close some effect and post processing.
                  For DD test only.
    Parameter :
    Return    : None
    Andrew Wen 2007/2/12
***************************************************************************/
void _AUD_DDBannerEnable(UINT8 u1DecId, BOOL fgEnable)
{
    //static BOOL  fgBannerEnable = FALSE;
    /*
    UINT16 u2ShmIndex;
    static UINT8 u1VsEnable = 0;
    static UINT8 u1AgcEnable = 0;
    static UINT32 u4DownmixMode = 0;
    */
    AMP_DRV_CB_T *pfnDrvCbFuncTbl;
    UINT16 i, j;
    UNUSED(i);
    UNUSED(j);

    AUD_DEC_ID_VALIDATE_2(u1DecId);
    if (fgBannerEnable == fgEnable)
    {
        return;
    }
    fgBannerEnable = fgEnable;

    AUD_DrvSetDDBanner(fgEnable);

    if (fgEnable)
    {
        //Backup original setting
        /*
        u2ShmIndex = B_VSURRFLAG;  //surr
        u1VsEnable = uReadShmUINT8(u2ShmIndex);
        u1AgcEnable = _afgUserAgcFlag[u1DecId];  //Agc
        u2ShmIndex = D_SPKCFG;     //downmix
        u4DownmixMode =(u4ReadShmUINT32(u2ShmIndex) & 0x7);
        */

#ifdef CC_AUD_USE_FLASH_AQ  
        //turn off all SPH
#ifdef CC_SONY_2K14_FLASH_AQ
        for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_SPH_ITEMS); i++)
        {
            *((UINT8*)(_au1AQ_SPH_TABLE) + i) = 0;
            *((UINT8*)(_au1AQ_SPH_WALL) + i) = 0;
        }
                
        //Bass Treble Offset
        for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_BASS_ITEMS); i++)
        {
            *((UINT8*)(_au1AQBassTrebleOffset_TABLE) + i) = MID_BASS_TREBLE_OFFSET; //mid value is 10.
            *((UINT8*)(_au1AQBassTrebleOffset_WALL) + i) = MID_BASS_TREBLE_OFFSET; //mid value is 10.
        }      
        //Bass Management
        for (i=0; i<FLASH_AQ_SRC_NUM; i++)
        {
            for (j=0; j<FLASH_AQ_SM_NUM; j++)
            {
                _au1AQBassManage_TABLE[i][j][0] = 1; //L/R is large.
                _au1AQBassManage_TABLE[i][j][1] = 1; //C is large.
                _au1AQBassManage_TABLE[i][j][2] = 1; //Ls/Rs is large.
                _au1AQBassManage_TABLE[i][j][3] = 1; //9/10 is large.
                _au1AQBassManage_WALL[i][j][0] = 1; //L/R is large.
                _au1AQBassManage_WALL[i][j][1] = 1; //C is large.
                _au1AQBassManage_WALL[i][j][2] = 1; //Ls/Rs is large.
                _au1AQBassManage_WALL[i][j][3] = 1; //9/10 is large.
        
            }
        } 
#else
        for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_SPH_ITEMS); i++)
        {
            *((UINT8*)(_au1AQ_SPH) + i) = 0;
        }
        //Bass Treble Offset
        for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_BASS_ITEMS); i++)
        {
            *((UINT8*)(_au1AQBassTrebleOffset) + i) = MID_BASS_TREBLE_OFFSET; //mid value is 10.
        }      
        //Bass Management
        for (i=0; i<FLASH_AQ_SRC_NUM; i++)
        {
            for (j=0; j<FLASH_AQ_SM_NUM; j++)
            {
                _au1AQBassManage[i][j][0] = 1; //L/R is large.
                _au1AQBassManage[i][j][1] = 1; //C is large.
                _au1AQBassManage[i][j][2] = 1; //Ls/Rs is large.
                _au1AQBassManage[i][j][3] = 1; //9/10 is large.
            }
        }      
#endif
#endif             
        // enlarge stc_diff bound.  check 0x82a4 ~ 82a6
        AUD_AudioSetStcDiffBound(u1DecId,3,3,2);
        // add audio start decoding pts
        AUD_AudioPTSDelayEnable(u1DecId, 1);
        // turn off surr        check 0x1549 (should be 0)
        AUD_DspVirtualSurroundEnalbe(u1DecId, FALSE);
        // turn off agc         check 0x1428 (should be 0)
        AUD_DspAVCEnable(u1DecId, FALSE);
        // turn off ChannelEQ   check 0x1543 (should be 0)
        AUD_DspChannelEqEnable(u1DecId, FALSE);
        // disable BassBoost    check 0x1543 (should be 0)
        AUD_DspBassBoostEnable(u1DecId, FALSE);
        // disable Treble       check 0x1543 (should be 0)
        AUD_DspClearBoostEnable(u1DecId, FALSE);
        // disable Reverb       check 0x1544 (should be 0)
        AUD_DspReverbEnable(u1DecId, FALSE);
#ifdef CC_AUD_DDBANNER
        // downmix Lt/Rt
        AUD_DspSpeakerOutputConfig(u1DecId, (u4ReadShmUINT32(D_SPKCFG) & 0xfffffff8));
#endif
        // Large Speaker
        AUD_DspSpeakerLsConfig(u1DecId,1,1,1,1,1);

        // enable new demux function
        DMX_SetIgnorePESLen(TRUE);

        pfnDrvCbFuncTbl = ADAC_GetDrvCallBackFuncTbl();
        if (pfnDrvCbFuncTbl->pfnDisableEffect != NULL)
        {
            pfnDrvCbFuncTbl->pfnDisableEffect ();
        }
    }
    else
    {
        //Reset related config
        AUD_AudioSetStcDiffBound(AUD_DEC_MAIN,0,0,0);
        AUD_AudioPTSDelayEnable(AUD_DEC_MAIN, 0);
        /*
        AUD_DspVirtualSurroundEnalbe(u1DecId, u1VsEnable);
        AUD_DspAVCEnable(u1DecId, u1AgcEnable);
        u2ShmIndex = D_SPKCFG;
        AUD_DspSpeakerOutputConfig(u1DecId, ((u4ReadShmUINT32(u2ShmIndex) & 0xfffffff8)|u4DownmixMode) );
        */
    }
#ifdef CC_AUD_ARM_POST  
    AUD_AprocReconfig(fgEnable);
#endif    

}

/***************************************************************************
    Function : _AUD_GetDDBannerEnable
    Description : Get Dolby banner information.
                  For DD test only.
    Parameter :
    Return    : None
***************************************************************************/
BOOL _AUD_GetDDBannerEnable (void)
{
    return (fgBannerEnable);
}

// *********************************************************************
// Function : AUD_DspSetPalDetection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eChangedItem,
                                                                                        UINT16 u2Value)
{
    UINT16 u2ShmIndex = W_PAL_CORRECT_THRESHOLD;

    switch (eChangedItem)
        {
        case AUD_FACTORY_PAL_CORRECT:
            u2ShmIndex = W_PAL_CORRECT_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_SYNC_LOOP:
            u2ShmIndex = W_PAL_TOTAL_SYNC1_LOOP;
            break;
        case AUD_FACTORY_PAL_ERROR:
            u2ShmIndex = W_PAL_ERR_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_PARITY_ERROR:
            u2ShmIndex = W_PAL_PARITY_ERR_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_NUM_FRAMES:
            u2ShmIndex = W_PAL_EVERY_NUM_FRAMES;
            break;
        default:
            break;
        }
    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : AUD_DspGetPalDetection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
UINT16 _AUD_DspGetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eItem)
{
    UINT16 u2ShmIndex = W_PAL_CORRECT_THRESHOLD;

    switch (eItem)
        {
        case AUD_FACTORY_PAL_CORRECT:
            u2ShmIndex = W_PAL_CORRECT_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_SYNC_LOOP:
            u2ShmIndex = W_PAL_TOTAL_SYNC1_LOOP;
            break;
        case AUD_FACTORY_PAL_ERROR:
            u2ShmIndex = W_PAL_ERR_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_PARITY_ERROR:
            u2ShmIndex = W_PAL_PARITY_ERR_THRESHOLD;
            break;
        case AUD_FACTORY_PAL_NUM_FRAMES:
            u2ShmIndex = W_PAL_EVERY_NUM_FRAMES;
            break;
        default:
            break;
        }
    return u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);
}

// *********************************************************************
// Function : AUD_DspSetA2Detection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eChangedItem,
                                                                                        UINT16 u2Value)
{
    UINT16 u2ShmIndex = W_A2_NUM_CHECK;

    switch (eChangedItem)
        {
        case AUD_FACTORY_A2_NUM_CHECK:
            u2ShmIndex = W_A2_NUM_CHECK;
            break;
        case AUD_FACTORY_A2_NUM_DOUBLE:
            u2ShmIndex = W_A2_NUM_DOUBLE_CHECK;
            break;
        case AUD_FACTORY_A2_MONO_WEIGHT:
            u2ShmIndex = W_A2_MONO_WEIGHT;
            break;
        case AUD_FACTORY_A2_STEREO_WEIGHT:
            u2ShmIndex = W_A2_STEREO_WEIGHT;
            break;
        case AUD_FACTORY_A2_DUAL_WEIGHT:
            u2ShmIndex = W_A2_DUAL_WEIGHT;
            break;
        default:
            break;
        }
    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : AUD_DspSetA2Detection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
UINT16 _AUD_DspGetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eItem)
{
    UINT16 u2ShmIndex = W_A2_NUM_CHECK;

    switch (eItem)
        {
        case AUD_FACTORY_A2_NUM_CHECK:
            u2ShmIndex = W_A2_NUM_CHECK;
            break;
        case AUD_FACTORY_A2_NUM_DOUBLE:
            u2ShmIndex = W_A2_NUM_DOUBLE_CHECK;
            break;
        case AUD_FACTORY_A2_MONO_WEIGHT:
            u2ShmIndex = W_A2_MONO_WEIGHT;
            break;
        case AUD_FACTORY_A2_STEREO_WEIGHT:
            u2ShmIndex = W_A2_STEREO_WEIGHT;
            break;
        case AUD_FACTORY_A2_DUAL_WEIGHT:
            u2ShmIndex = W_A2_DUAL_WEIGHT;
            break;
        default:
            break;
        }
    return u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);
}

// *********************************************************************
// Function : AUD_DspSetA2Threshold
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eChangedItem,
                                                                                        UINT16 u2Value)
{
    UINT16 u2ShmIndex = W_A2_MONO_STEREO_THRES_EU;
    UINT16 u2SrcMode = u2ReadShmUINT16(AUD_DSP0, W_SOURCE_MODE);

    switch (eChangedItem)
        {
        case AUD_FACTORY_A2_MONO2STEREO:
            u2ShmIndex = (u2SrcMode == SOURCE_NTSC_M)?
                W_A2_MONO_STEREO_THRES_M : W_A2_MONO_STEREO_THRES_EU;
            break;
        case AUD_FACTORY_A2_STEREO2MONO:
            u2ShmIndex = (u2SrcMode == SOURCE_NTSC_M)?
                W_A2_STEREO_MONO_THRES_M : W_A2_STEREO_MONO_THRES_EU;
            break;
    case AUD_FACTORY_A2_WEAK_RF_LVL_MONO2STEREO:
            u2ShmIndex = W_A2_WEAK_RF_LVL_M2S_THRES_EU;
            break;
    case AUD_FACTORY_A2_WEAK_RF_LVL_STEREO2MONO:
            u2ShmIndex = W_A2_WEAK_RF_LVL_S2M_THRES_EU;
            break;
        default:
            break;
        }
    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : AUD_DspGetA2Threshold
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
UINT16 _AUD_DspGetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eChangedItem)
{
    UINT16 u2ShmIndex = W_A2_MONO_STEREO_THRES_EU;
    UINT16 u2SrcMode = u2ReadShmUINT16(AUD_DSP0, W_SOURCE_MODE);

    switch (eChangedItem)
        {
        case AUD_FACTORY_A2_MONO2STEREO:
            u2ShmIndex = (u2SrcMode == SOURCE_NTSC_M)?
                W_A2_MONO_STEREO_THRES_M : W_A2_MONO_STEREO_THRES_EU;
            break;
        case AUD_FACTORY_A2_STEREO2MONO:
            u2ShmIndex = (u2SrcMode == SOURCE_NTSC_M)?
                W_A2_STEREO_MONO_THRES_M : W_A2_STEREO_MONO_THRES_EU;
            break;
    case AUD_FACTORY_A2_WEAK_RF_LVL_MONO2STEREO:
            u2ShmIndex = W_A2_WEAK_RF_LVL_M2S_THRES_EU;
            break;
    case AUD_FACTORY_A2_WEAK_RF_LVL_STEREO2MONO:
            u2ShmIndex = W_A2_WEAK_RF_LVL_S2M_THRES_EU;
            break;
        default:
            break;
        }
    return u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);
}

/******************************************************************************
* Function      : AUD_DspSetMtsDetection
* Description   : Set NPTV MTS Detection Configurations
* Parameter     : u2NumCheck, u2StereoMid, u2StereoConMid, u2Pilot3Mid,
*                 u2Pilot3ConMid, u2SapMid, u2SapConMid
* Return        : None
******************************************************************************/
void _AUD_DspSetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eChangedItem,
                                                                                        UINT16 u2Value)
{
    UINT16 u2ShmIndex = W_MTS_NUM_CHECK;

    switch (eChangedItem)
        {
        case AUD_FACTORY_MTS_NUM_CHECK:
            u2ShmIndex = W_MTS_NUM_CHECK;
            break;
        case AUD_FACTORY_MTS_NUM_PILOT:
            u2ShmIndex = W_MTS_NUM_PILOT;
            break;
        case AUD_FACTORY_MTS_PILOT_HIGH:
            u2ShmIndex = W_MTS_STEREO_CON_MID;
            break;
        case AUD_FACTORY_MTS_PILOT_LOW:
            u2ShmIndex = W_MTS_STEREO_MID;
            break;
        case AUD_FACTORY_MTS_NUM_SAP:
            u2ShmIndex = W_MTS_NUM_SAP;
            break;
        case AUD_FACTORY_MTS_SAP_HIGH:
            u2ShmIndex = W_MTS_SAP_CON_MID;
            break;
        case AUD_FACTORY_MTS_SAP_LOW:
            u2ShmIndex = W_MTS_SAP_MID;
            break;
        default:
            break;
        }
    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
}

/******************************************************************************
* Function      : AUD_DspGetMtsDetection
* Description   : Set NPTV MTS Detection Configurations
* Parameter     : u2NumCheck, u2StereoMid, u2StereoConMid, u2Pilot3Mid,
*                 u2Pilot3ConMid, u2SapMid, u2SapConMid
* Return        : None
******************************************************************************/
UINT16 _AUD_DspGetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eItem)
{
    UINT16 u2ShmIndex = W_MTS_NUM_CHECK;

    switch (eItem)
        {
        case AUD_FACTORY_MTS_NUM_CHECK:
            u2ShmIndex = W_MTS_NUM_CHECK;
            break;
        case AUD_FACTORY_MTS_NUM_PILOT:
            u2ShmIndex = W_MTS_NUM_PILOT;
            break;
        case AUD_FACTORY_MTS_PILOT_HIGH:
            u2ShmIndex = W_MTS_STEREO_CON_MID;
            break;
        case AUD_FACTORY_MTS_PILOT_LOW:
            u2ShmIndex = W_MTS_STEREO_MID;
            break;
        case AUD_FACTORY_MTS_NUM_SAP:
            u2ShmIndex = W_MTS_NUM_SAP;
            break;
        case AUD_FACTORY_MTS_SAP_HIGH:
            u2ShmIndex = W_MTS_SAP_CON_MID;
            break;
        case AUD_FACTORY_MTS_SAP_LOW:
            u2ShmIndex = W_MTS_SAP_MID;
            break;
        default:
            break;
        }
    return u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);
}

#if 0   // Unused
// *********************************************************************
// Function : AUD_DspSetFmfmDetection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eChangedItem,
                                                                                        UINT16 u2Value)
{
    UINT16 u2ShmIndex = W_FMFM_NUM_CHECK;

    switch (eChangedItem)
        {
        case AUD_FACTORY_FMFM_NUM_CHECK:
            u2ShmIndex = W_FMFM_NUM_CHECK;
            break;
        case AUD_FACTORY_FMFM_NUM_DOUBLE:
            u2ShmIndex = W_FMFM_NUM_DOUBLE_CHECK;
            break;
        case AUD_FACTORY_FMFM_MONO_WEIGHT:
            u2ShmIndex = W_FMFM_MONO_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_STEREO_WEIGHT:
            u2ShmIndex = W_FMFM_STEREO_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_DUAL_WEIGHT:
            u2ShmIndex = W_FMFM_DUAL_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_DETECT_CONFID:
            u2ShmIndex = W_FMFM_DETECT_CONFID;
            break;
        default:
            break;
        }
    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Value);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : AUD_DspGetFmfmDetection
// Description : demodulation related configuration
// Parameter :
// Return    :
// *********************************************************************
UINT16 _AUD_DspGetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eItem)
{
    UINT16 u2ShmIndex = W_FMFM_NUM_CHECK;

    switch (eItem)
        {
        case AUD_FACTORY_FMFM_NUM_CHECK:
            u2ShmIndex = W_FMFM_NUM_CHECK;
            break;
        case AUD_FACTORY_FMFM_NUM_DOUBLE:
            u2ShmIndex = W_FMFM_NUM_DOUBLE_CHECK;
            break;
        case AUD_FACTORY_FMFM_MONO_WEIGHT:
            u2ShmIndex = W_FMFM_MONO_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_STEREO_WEIGHT:
            u2ShmIndex = W_FMFM_STEREO_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_DUAL_WEIGHT:
            u2ShmIndex = W_FMFM_DUAL_WEIGHT;
            break;
        case AUD_FACTORY_FMFM_DETECT_CONFID:
            u2ShmIndex = W_FMFM_DETECT_CONFID;
            break;
        default:
            break;
        }
    return u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);
}
#endif

#ifndef CC_AUD_WAFFLE_OVM_SUPPORT  
// *********************************************************************
// Function : AUD_DspSetHdevMode
// Description : set high deveation mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void _AUD_DspSetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType, UINT16 u2Mode)
{
    UINT8 u1HdevTemp;
    UINT8 u1HdevMsak=0;
    AUD_MTS_FAC_T*     prMtsFactory;
    static UINT16 u2Mode_pre = 100;
#if defined(CC_AUD_DDI) ||  defined(CC_SONY_2K14_FLASH_AQ_NEW_HDEV)
   if((eDecType == AUD_ATV_DECODER_MTS) && (u2Mode !=0))
   {
       u2Mode = 2;
   }
#endif

   u1HdevMode = u2Mode;
   if( (eDecType == AUD_ATV_DECODER_MTS) && (u2Mode_pre != u2Mode))
   {
       u2Mode_pre = u2Mode;
       if(u2Mode)
       {
            vWriteShmUINT16(AUD_DSP0, W_MTS_STEREO_CON_MID, 190);
            vWriteShmUINT16(AUD_DSP0, W_MTS_STEREO_MID,  120);

            vWriteShmUINT16(AUD_DSP0, W_MTS_SAP_CON_MID, 400);
#if defined(CC_AUD_DDI) ||  defined(CC_SONY_2K14_FLASH_AQ_NEW_HDEV)
            vWriteShmUINT16(AUD_DSP0, W_MTS_SAP_MID, 260);
#else
            vWriteShmUINT16(AUD_DSP0, W_MTS_SAP_MID, 130);
#endif
            vWriteShmUINT8(AUD_DSP0, B_MTS_MUTE_SAP_HIGH, 5);
            vWriteShmUINT8(AUD_DSP0, B_MTS_MUTE_SAP_LOW, 62);

            vWriteShmUINT8(AUD_DSP0, B_SAP_HP_MUTE_HIGH, 90);
            vWriteShmUINT8(AUD_DSP0, B_SAP_HP_MUTE_LOW, 176);
       }
       else
       {
            UNUSED(DRVCUST_OptQuery(eAudioMtsFactory, (UINT32 *)(void *) &prMtsFactory));
            vWriteShmUINT16(AUD_DSP0, W_MTS_STEREO_CON_MID, prMtsFactory->u2PilotHigherThd);
            vWriteShmUINT16(AUD_DSP0, W_MTS_STEREO_MID,  prMtsFactory->u2PilotLowerThd);

            vWriteShmUINT16(AUD_DSP0, W_MTS_SAP_CON_MID, prMtsFactory->u2SapHigherThd);
            vWriteShmUINT16(AUD_DSP0, W_MTS_SAP_MID, prMtsFactory->u2SapLowerThd);

            vWriteShmUINT8(AUD_DSP0, B_MTS_MUTE_SAP_HIGH, prMtsFactory->u1SapNoiseMuteHigh);
            vWriteShmUINT8(AUD_DSP0, B_MTS_MUTE_SAP_LOW, prMtsFactory->u1SapNoiseMuteLow);

            vWriteShmUINT8(AUD_DSP0, B_SAP_HP_MUTE_HIGH, 35);
            vWriteShmUINT8(AUD_DSP0, B_SAP_HP_MUTE_LOW, 10);
       }
   }

    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1HdevMsak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1HdevMsak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
             if (u2Mode==2) u1HdevMsak = MTS_NEW_MASK;
          else if (u2Mode==1)   u1HdevMsak = MTS_MASK;
                else u1HdevMsak = MTS_MASK|MTS_NEW_MASK;
                break;
            case AUD_ATV_DECODER_FMFM:
                u1HdevMsak = FMFM_MASK;
                break;
            default:
                break;
        }
    if (u2Mode)
    {
        //u1HdevTemp = uReadShmUINT8(B_HDEV_MODE) |u1HdevMsak;
        u1HdevTemp = uReadShmUINT8(AUD_DSP0, B_HDEV_MODE) & (~u1HdevMsak);
        u1HdevTemp = u1HdevTemp | u1HdevMsak;
    }
    else
    {
        u1HdevTemp = uReadShmUINT8(AUD_DSP0, B_HDEV_MODE) & (~u1HdevMsak);
    }
    if (u1HdevTemp !=  uReadShmUINT8(AUD_DSP0, B_HDEV_MODE))
    {
        vWriteShmUINT8(AUD_DSP0, B_HDEV_MODE, u1HdevTemp);
    }
    if (((UINT8)u2Mode != uReadShmUINT8 (AUD_DSP0, B_NON_EU_FM_MODE)) && (eDecType == AUD_ATV_DECODER_PAL))//for LGE HDEV 300% distortion
    {
        vWriteShmUINT8 (AUD_DSP0, B_NON_EU_FM_MODE, (UINT8)u2Mode);
    }
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : AUD_DspGetHdevMode
// Description : set high deveation mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
UINT8 _AUD_DspGetHdevMode (AUD_ATV_DECODER_TYPE_T eDecType)
{
    UINT8 u1HdevMsak=0;
    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1HdevMsak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1HdevMsak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
                u1HdevMsak = MTS_NEW_MASK|MTS_MASK;
                break;
            case AUD_ATV_DECODER_FMFM:
                u1HdevMsak = FMFM_MASK;
                break;
            default:
                break;
        }
    if (eDecType== AUD_ATV_DECODER_MTS)
    {
        return ((uReadShmUINT8(AUD_DSP0, B_HDEV_MODE) & MTS_MASK)>>2)|((uReadShmUINT8(AUD_DSP0, B_HDEV_MODE) & MTS_NEW_MASK)>>3);
    }
    else
    {
   return !((uReadShmUINT8(AUD_DSP0, B_HDEV_MODE) & (u1HdevMsak)) == 0);
    }

}
#else
// *********************************************************************
// Function : AUD_DspSetHdevMode
// Description : set high deveation mode off/on or off/low/high
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void _AUD_DspSetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType, UINT8 u1Mode)
{
    UINT8 u1HdevTemp;  
    UINT8 u1HdevMsak = 0, u1HdecModeShift = 0;  
    u1HdevMode = u1Mode;

    switch (eDecType)
    {
        case AUD_ATV_DECODER_PAL:
            u1HdevMsak = NEW_PAL_MASK;
            u1HdecModeShift = NEW_PAL_SHIFT;
            break;
        case AUD_ATV_DECODER_A2:
            u1HdevMsak = NEW_A2_MASK;
            u1HdecModeShift = NEW_A2_SHIFT;
            break;
        case AUD_ATV_DECODER_MTS:
            u1HdevMsak = NEW_MTS_MASK;
            u1HdecModeShift = NEW_MTS_SHIFT;
            break;              
        case AUD_ATV_DECODER_FMFM:
            u1HdevMsak = NEW_FMFM_MASK;
            u1HdecModeShift = NEW_FMFM_SHIFT;
            break;
    default:
        break;
    }
    
    u1HdevTemp = (uReadShmUINT8(AUD_DSP0, B_HDEV_MODE) & (~u1HdevMsak)) | ((u1Mode << u1HdecModeShift) & u1HdevMsak);
    
    if (u1HdevTemp !=  uReadShmUINT8(AUD_DSP0, B_HDEV_MODE))
    {
        vWriteShmUINT8(AUD_DSP0, B_HDEV_MODE, u1HdevTemp);
    }
}

// *********************************************************************
// Function : AUD_DspGetHdevMode
// Description : set high deveation mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
UINT8 _AUD_DspGetHdevMode (AUD_ATV_DECODER_TYPE_T eDecType)
{
    UINT8 u1HdevMsak = 0, u1HdevShift = 0;  
    
    switch(eDecType)
    {
        case AUD_ATV_DECODER_PAL:
            u1HdevMsak = NEW_PAL_MASK;
            u1HdevShift = NEW_PAL_SHIFT;
            break;
        case AUD_ATV_DECODER_A2:
            u1HdevMsak = NEW_A2_MASK;
            u1HdevShift = NEW_A2_SHIFT;
            break;
        case AUD_ATV_DECODER_MTS:
            u1HdevMsak = NEW_MTS_MASK;
            u1HdevShift = NEW_MTS_SHIFT;
            break;
        case AUD_ATV_DECODER_FMFM:
            u1HdevMsak = NEW_FMFM_MASK;
            u1HdevShift = NEW_FMFM_SHIFT;
            break;   
        default:
            break;
    }

   return ((uReadShmUINT8(AUD_DSP0, B_HDEV_MODE) & u1HdevMsak) >> u1HdevShift);
}
#endif


// *********************************************************************
// Function : AUD_DspSetAmMute
// Description : set AM mute mode on/off and thresholds
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eChangedItem,
                                                                                        UINT8 u1Value)
{
    UINT16 u2ShmIndex = B_AM_MUTE_MODE;

    switch (eChangedItem)
        {
        case AUD_FACTORY_PAL_AM_MUTE:
            u2ShmIndex = B_AM_MUTE_MODE;
            break;
        case AUD_FACTORY_PAL_AM_MUTE_HIGH:
            u2ShmIndex = B_AM_MUTE_THRESHOLD_HIGHT;
            break;
        case AUD_FACTORY_PAL_AM_MUTE_LOW:
            u2ShmIndex = B_AM_MUTE_THRESHOLD_LOW;
            break;
        default:
            break;
        }
    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, u1Value);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : AUD_DspGetAmMute
// Description : set AM mute mode on/off and thresholds
// Parameter :
// Return    :
// *********************************************************************
UINT8 _AUD_DspGetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eItem)
{
    UINT16 u2ShmIndex = B_AM_MUTE_MODE;

    switch (eItem)
        {
        case AUD_FACTORY_PAL_AM_MUTE:
            u2ShmIndex = B_AM_MUTE_MODE;
            break;
        case AUD_FACTORY_PAL_AM_MUTE_HIGH:
            u2ShmIndex = B_AM_MUTE_THRESHOLD_HIGHT;
            break;
        case AUD_FACTORY_PAL_AM_MUTE_LOW:
            u2ShmIndex = B_AM_MUTE_THRESHOLD_LOW;
            break;
        default:
            break;
        }
    return uReadShmUINT8(AUD_DSP0, u2ShmIndex);
}

// *********************************************************************
// Function : Aud_DspSetFmMute
// Description : set FM mute mode on/off and thresholds
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void _AUD_DspSetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType,
                                                            AUD_FACTORY_FM_MUTE_ITEM_T eChangedItem,
                                                                  UINT8 u1Value)
{
    UINT16 u2ShmIndex = B_FM_MUTE_MODE;
    UINT8 u1Mask= 0;

    switch (eChangedItem)
        {
        case AUD_FACTORY_FM_MUTE:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u1Mask = PAL_MASK;
                    break;
                case AUD_ATV_DECODER_A2:
                    u1Mask = A2_MASK;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u1Mask = MTS_MASK;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u1Mask = FMFM_MASK;
                    break;
                default:
                    break;
                }
            u2ShmIndex = B_FM_MUTE_MODE;
            if (u1Value == 0)
                {
                u1Value = uReadShmUINT8(AUD_DSP0, B_FM_MUTE_MODE) & (~u1Mask);
                }
            else
                {
                u1Value = uReadShmUINT8(AUD_DSP0, B_FM_MUTE_MODE) | u1Mask;
                }
            break;
        case AUD_FACTORY_FM_MUTE_HIGH:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_PAL;
                    break;
                case AUD_ATV_DECODER_A2:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_A2;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_MTS;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_EIAJ;
                    break;
                default:
                    break;
                }
            break;
        case AUD_FACTORY_FM_MUTE_LOW:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_PAL;
                    break;
                case AUD_ATV_DECODER_A2:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_A2;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_MTS;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_EIAJ;
                    break;
                default:
                    break;
                }
            break;
        case AUD_FACTORY_FM_MUTE_HIGH_HDEV:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT;
                    break;
                default:
                    break;    
                }
            break;
        case AUD_FACTORY_FM_MUTE_LOW_HDEV:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW;
                    break;
                default:
                    break;
                }
            break;
        default:
            break;
        }
    UNUSED(u1Mask);
    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, u1Value);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : Aud_DspGetFmMute
// Description : set FM mute mode on/off and thresholds
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
UINT8 _AUD_DspGetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType,AUD_FACTORY_FM_MUTE_ITEM_T eItem)
{
    UINT16 u2ShmIndex = B_FM_MUTE_MODE;
    UINT8 u1Mask= 0;
    UINT8 u1Return= 0;
    switch (eItem)
        {
        case AUD_FACTORY_FM_MUTE:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u1Mask = PAL_MASK;
                    break;
                case AUD_ATV_DECODER_A2:
                    u1Mask = A2_MASK;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u1Mask = MTS_MASK;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u1Mask = FMFM_MASK;
                    break;
                default:
                    break;
                }
            u1Return = (UINT8)!((uReadShmUINT8(AUD_DSP0, B_FM_MUTE_MODE) & (u1Mask)) == 0);
            break;
        case AUD_FACTORY_FM_MUTE_HIGH:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_PAL;
                    break;
                case AUD_ATV_DECODER_A2:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_A2;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_MTS;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_HIGHT_EIAJ;
                    break;
                default:
                    break;
                }
            u1Return = uReadShmUINT8(AUD_DSP0, u2ShmIndex);
            break;
        case AUD_FACTORY_FM_MUTE_LOW:
            switch (eDecType)
                {
                case AUD_ATV_DECODER_PAL:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_PAL;
                    break;
                case AUD_ATV_DECODER_A2:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_A2;
                    break;
                case AUD_ATV_DECODER_MTS:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_MTS;
                    break;
                case AUD_ATV_DECODER_FMFM:
                    u2ShmIndex = B_FM_MUTE_THRESHOLD_LOW_EIAJ;
                    break;
                default:
                    break;
                }
            u1Return = uReadShmUINT8(AUD_DSP0, u2ShmIndex);
            break;
        default:
            break;
        }
    return u1Return;
}

// *********************************************************************
// Function : AUD_DspSetCarrierShiftMode
// Description :set carrier shift mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void _AUD_DspSetCarrierShiftMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable)
{
    UINT8 u1CarrierShift;
    UINT8 u1Msak=0;

    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1Msak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1Msak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
                u1Msak = MTS_MASK;
                break;
            case AUD_ATV_DECODER_FMFM:
                u1Msak = FMFM_MASK;
                break;
            default:
                break;
        }
    if (fgEnable)
        {
        u1CarrierShift = uReadShmUINT8(AUD_DSP0, B_CARRIER_SHIFT_MODE) |u1Msak;
        }
    else
        {
        u1CarrierShift = uReadShmUINT8(AUD_DSP0, B_CARRIER_SHIFT_MODE) & (~u1Msak);
        }
    if (u1CarrierShift !=  uReadShmUINT8(AUD_DSP0, B_CARRIER_SHIFT_MODE))
    {
        vWriteShmUINT8(AUD_DSP0, B_CARRIER_SHIFT_MODE, u1CarrierShift);
    }
}

// *********************************************************************
// Function : AUD_DspGetCarrierShiftMode
// Description : set high deveation mode on/off
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
BOOL _AUD_DspGetCarrierShiftMode (AUD_ATV_DECODER_TYPE_T eDecType)
{
    UINT8 u1Msak=0;
    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1Msak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1Msak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
                u1Msak = MTS_MASK;
                break;
            case AUD_ATV_DECODER_FMFM:
                u1Msak = FMFM_MASK;
                break;
            default:
                break;
        }
   return !((uReadShmUINT8(AUD_DSP0, B_CARRIER_SHIFT_MODE) & (u1Msak)) == 0);

}

// *********************************************************************
// Function : AUD_DspSetNonEUMode
// Description : set Non-EU mode on/off
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetNonEUMode (BOOL fgEnable)
{
    if (((UINT8)fgEnable != uReadShmUINT8 (AUD_DSP0, B_NON_EU_FM_MODE)))
    {
        vWriteShmUINT8 (AUD_DSP0, B_NON_EU_FM_MODE, (UINT8)fgEnable);
        DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
    }
}

// *********************************************************************
// Function : AUD_DspGetNonEUMode
// Description : set Non-EU mode on/off
// Parameter :
// Return    :
// *********************************************************************
BOOL _AUD_DspGetNonEUMode (void)
{
    return (BOOL)uReadShmUINT8(AUD_DSP0, B_NON_EU_FM_MODE);
}

// *********************************************************************
// Function : void AUD_DspSetFMSatuMuteMode() large
// Description : demodulation related configuration
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
void _AUD_DspSetFMSatuMuteMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable)
{
    UINT8 u1SatuMute;
    UINT8 u1Msak=0;

    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1Msak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1Msak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
                u1Msak = MTS_MASK;
                break;
            case AUD_ATV_DECODER_FMFM:
                u1Msak = FMFM_MASK;
                break;
            default:
                break;
        }
    if (fgEnable)
        {
        u1SatuMute = uReadShmUINT8(AUD_DSP0, B_FM_SATU_MUTE_MODE) |u1Msak;
        }
    else
        {
        u1SatuMute = uReadShmUINT8(AUD_DSP0, B_FM_SATU_MUTE_MODE) & (~u1Msak);
        }
    if (u1SatuMute !=  uReadShmUINT8(AUD_DSP0, B_FM_SATU_MUTE_MODE))
    {
        vWriteShmUINT8(AUD_DSP0, B_FM_SATU_MUTE_MODE, u1SatuMute);
    }
}

// *********************************************************************
// Function : void AUD_DspGetFMSatuMuteMode() large
// Description : demodulation related configuration
// Parameter : bit0 : PAL
//                  bit1 : A2
//                  bit2 : MTS
//                  bit3 : FMFM
// Return    :
// *********************************************************************
BOOL _AUD_DspGetFMSatuMuteMode (AUD_ATV_DECODER_TYPE_T eDecType)
{
    UINT8 u1Msak=0;
    switch(eDecType)
        {
            case AUD_ATV_DECODER_PAL:
                u1Msak = PAL_MASK;
                break;
            case AUD_ATV_DECODER_A2:
                u1Msak = A2_MASK;
                break;
            case AUD_ATV_DECODER_MTS:
                u1Msak = MTS_MASK;
                break;
            case AUD_ATV_DECODER_FMFM:
                u1Msak = FMFM_MASK;
                break;
            default:
                break;
        }
   return !((uReadShmUINT8(AUD_DSP0, B_FM_SATU_MUTE_MODE) & (u1Msak)) == 0);
}

//-----------------------------------------------------------------------------
/** AUD_DspSetPALFineVolume
 *   Set fine tune volume for PAL.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetPALFineVolume (UINT8 u1Index)
{
    DSP_SetPALFineVolume(u1Index) ;
    _u1PalFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetPALFineVolume
 *   Get fine tune volume for PAL.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetPALFineVolume (void)
{
    UINT8 i=0;
    for(i=0; i<sizeof(VOLUME_FINE_TUNE)/sizeof(UINT32); i++)
    {
        if((u4ReadShmUINT32(AUD_DSP0, D_PAL_FINE_TUNE_VOLUME)>>8) == (VOLUME_FINE_TUNE[i]))
            break;       
    }
    if(i>40)
        i=40;
    _u1PalFineVol = i;
    return _u1PalFineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetNicamFineVolume
 *   Set fine tune volume for NICAM.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetNicamFineVolume (UINT8 u1Index)
{
    DSP_SetNICAMFineVolume(u1Index);
    _u1NicamFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetNicamFineVolume
 *   Get fine tune volume for NICAM.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetNicamFineVolume (void)
{
    UINT8 i=0;
    for(i=0; i<sizeof(VOLUME_FINE_TUNE)/sizeof(UINT32); i++)
    {
        if((u4ReadShmUINT32(AUD_DSP0, D_NICAM_FINE_TUNE_VOLUME)>>8) == (VOLUME_FINE_TUNE[i]))
            break;       
    }
    if(i>40)
        i=40;
    _u1NicamFineVol = i;
    return _u1NicamFineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetAMFineVolume
 *   Set fine tune volume for AM.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetAMFineVolume (UINT8 u1Index)
{
    DSP_SetAMFineVolume(u1Index);
    _u1AMFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetAMFineVolume
 *   Get fine tune volume for AM.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetAMFineVolume (void)
{
    UINT8 i=0;
    for(i=0; i<sizeof(VOLUME_FINE_TUNE)/sizeof(UINT32); i++)
    {
        if((u4ReadShmUINT32(AUD_DSP0, D_AM_FINE_TUNE_VOLUME)>>8) == (VOLUME_FINE_TUNE[i]))
            break;       
    }
    if(i>40)
        i=40;
    _u1AMFineVol = i;
    return _u1AMFineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetA2FineVolume
 *   Set fine tune volume for A2.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetA2FineVolume (UINT8 u1Index)
{
    DSP_SetA2FineVolume(u1Index) ;
    _u1A2FineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetA2FineVolume
 *   Get fine tune volume for A2.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetA2FineVolume (void)
{
    UINT8 i=0;
    for(i=0; i<sizeof(VOLUME_FINE_TUNE)/sizeof(UINT32); i++)
    {
        if((u4ReadShmUINT32(AUD_DSP0, D_A2_FINE_TUNE_VOLUME)>>8) == (VOLUME_FINE_TUNE[i]))
            break;       
    }
    if(i>40)
        i=40;
    _u1A2FineVol = i;
    return _u1A2FineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetMtsMonoFineVolume
 *   Set fine tune volume for MTS mono.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetMtsMonoFineVolume (UINT8 u1Index)
{
    DSP_SetMTSFineVolume(u1Index) ;
    _u1MtsFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetMtsMonoFineVolume
 *   Get fine tune volume for MTS mono.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetMtsMonoFineVolume (void)
{
    UINT8 i=0;
    for(i=0; i<sizeof(VOLUME_FINE_TUNE)/sizeof(UINT32); i++)
    {
        if((u4ReadShmUINT32(AUD_DSP0, D_MTS_FINE_TUNE_VOLUME)>>8) == (VOLUME_FINE_TUNE[i]))
            break;       
    }
    if(i>40)
        i=40;
    _u1MtsFineVol = i;
    return _u1MtsFineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetSAPFineVolume
 *   Set fine tune volume for SAP.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSAPFineVolume(UINT8 u1Index)
{
    DSP_SetSAPFineVolume(u1Index);
    _u1SapFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetSapFineVolume
 *   Get fine tune volume for SAP.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetSapFineVolume (void)
{
    return _u1SapFineVol;
}

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmMonoFineVolume
 *   Set fine tune volume for FMFM mono.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetFmfmMonoFineVolume (UINT8 u1Index)
{
    DSP_SetFmfmMonoFineVolume(u1Index) ;
    _u1FmfmMonoFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmMonoFineVolume
 *   Get fine tune volume for FMFM mono.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetFmfmMonoFineVolume (void)
{
    return _u1FmfmMonoFineVol;
}

//-----------------------------------------------------------------------------
/** AUD_DspSetFmfmDualFineVolume
 *   Set fine tune volume for FMFM dual.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetFmfmDualFineVolume(UINT8 u1Index)
{
    DSP_SetFmfmDualFineVolume(u1Index);
    _u1FmfmDualFineVol = u1Index;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetFmfmDualFineVolume
 *   Get fine tune volume for FMFM dual.
 *
 *  @retval  u1Index (0~40)
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetFmfmDualFineVolume (void)
{
    return _u1FmfmDualFineVol;
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetFmRadioFineVolume
 *   Set fine tune volume for MTS mono.
 *
 *  @param  u1Index      0~40 .
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetFmRadioFineVolume (UINT8 u1Index)
{
    DSP_SetFmRadioFineVolume(u1Index) ;
}

// *********************************************************************
// Function : AUD_DspSetMtsPilotDetection
// Description : set MTS pilot offset detection on/off
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetMtsPilotDetection (BOOL fgEnable)
{
    if (((UINT8)fgEnable != uReadShmUINT8 (AUD_DSP0, B_MTS_PILOT_OFFSET_DETECTION)))
    {
        vWriteShmUINT8 (AUD_DSP0, B_MTS_PILOT_OFFSET_DETECTION, (UINT8)fgEnable);
        DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
    }
}

// *********************************************************************
// Function : AUD_DspGetMtsPilotDetection
// Description : Get MTS pilot offset detection on/off
// Parameter :
// Return    :
// *********************************************************************
BOOL _AUD_DspGetMtsPilotDetection (void)
{
    return (BOOL)uReadShmUINT8(AUD_DSP0, B_MTS_PILOT_OFFSET_DETECTION);
}
// *********************************************************************
// Function : _AUD_DspSetSapMute
// Description : set SAP mute thresholds
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eChangedItem,
                                                                                        UINT8 u1Value)
{
    UINT16 u2ShmIndex = B_MTS_MUTE_SAP_LOW;

    switch (eChangedItem)
        {
        case AUD_FACTORY_SAP_MUTE_LOWER:
            u2ShmIndex = B_MTS_MUTE_SAP_LOW;
            break;
        case AUD_FACTORY_SAP_MUTE_HIGHER:
            u2ShmIndex = B_MTS_MUTE_SAP_HIGH;
            break;
        case AUD_FACTORY_SAP_HP_MUTE_HIGHER:
            u2ShmIndex = B_SAP_HP_MUTE_HIGH;
            break;
        case AUD_FACTORY_SAP_HP_MUTE_LOWER:
            u2ShmIndex = B_SAP_HP_MUTE_LOW;
            break;
        case AUD_FACTORY_SAP_FILTER_SEL:
            u2ShmIndex = B_SAP_FILTER_SEL;
            break;
        case AUD_FACTORY_SAP_RATIO:
            u2ShmIndex = B_SAP_RATIO;
            break;

        default:
            break;
        }
    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, u1Value);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : _AUD_DspGetSapMute
// Description : set SAP mute thresholds
// Parameter :
// Return    :
// *********************************************************************
UINT8 _AUD_DspGetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eItem)
{
    UINT16 u2ShmIndex = B_MTS_MUTE_SAP_LOW;

    switch (eItem)
        {
        case AUD_FACTORY_SAP_MUTE_LOWER:
            u2ShmIndex = B_MTS_MUTE_SAP_LOW;
            break;
        case AUD_FACTORY_SAP_MUTE_HIGHER:
            u2ShmIndex = B_MTS_MUTE_SAP_HIGH;
            break;
        case AUD_FACTORY_SAP_HP_MUTE_HIGHER:
            u2ShmIndex = B_SAP_HP_MUTE_HIGH;
            break;
        case AUD_FACTORY_SAP_HP_MUTE_LOWER:
            u2ShmIndex = B_SAP_HP_MUTE_LOW;
            break;
        case AUD_FACTORY_SAP_FILTER_SEL:
            u2ShmIndex = B_SAP_FILTER_SEL;
            break;
        case AUD_FACTORY_SAP_RATIO:
            u2ShmIndex = B_SAP_RATIO;
            break;

        default:
            break;
        }
    return uReadShmUINT8(AUD_DSP0, u2ShmIndex);
}

// *********************************************************************
// Function : _AUD_DspSetFMSatuMuteTh
// Description : Set saturation mute thresholds
// Parameter :
// Return    :
// *********************************************************************
void _AUD_DspSetFMSatuMuteTh( AUD_FACTORY_SATU_MUTE_ITEM_T eChangedItem,
                                                                                        UINT8 u1Value)
{
    UINT16 u2ShmIndex = B_SATU_MUTE_THRESHOLD_HIGH;

    switch (eChangedItem)
        {
        case AUD_FACTORY_SATU_MUTE_LOWER:
            u2ShmIndex = B_SATU_MUTE_THRESHOLD_LOW;
            break;
        case AUD_FACTORY_SATU_MUTE_HIGHER:
            u2ShmIndex = B_SATU_MUTE_THRESHOLD_HIGH;
            break;
        default:
            break;
        }
    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, u1Value);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DETECTION_CONFIG);
}

// *********************************************************************
// Function : _AUD_DspGetFMSatuMuteTh
// Description : Set saturation mute thresholds
// Parameter :
// Return    :
// *********************************************************************
UINT8 _AUD_DspGetFMSatuMuteTh( AUD_FACTORY_SATU_MUTE_ITEM_T eItem)
{
    UINT16 u2ShmIndex = B_SATU_MUTE_THRESHOLD_HIGH;

    switch (eItem)
        {
        case AUD_FACTORY_SATU_MUTE_LOWER:
            u2ShmIndex = B_SATU_MUTE_THRESHOLD_LOW;
            break;
        case AUD_FACTORY_SATU_MUTE_HIGHER:
            u2ShmIndex = B_SATU_MUTE_THRESHOLD_HIGH;
            break;
        default:
            break;
        }
    return uReadShmUINT8(AUD_DSP0, u2ShmIndex);
}

#ifdef CC_AUD_BBE_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspSetBbeLevel
 *   Set BBE level
 *
 *  @param  i1Level         0 ~ -2
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetBbeLevel (INT8 i1Level)
{
    if (_i1BbeLevel != i1Level)
    {
        _i1BbeLevel = i1Level;
        DSP_SetBbeLevel((UINT8)(-i1Level));
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspSetBbeProcess
 *   Set BBE Process boost gain (high part)
 *
 *  @param  u1BoostDb         3*2~12*2
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetBbeProcess (UINT8 u1BoostDb)
{
    if (_u1BbeProcess != u1BoostDb)
    {
        _u1BbeProcess = u1BoostDb;
        DSP_SetBbeProcess(u1BoostDb);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspSetBbeLoContour
 *   Set BBE Lo Contour boost gain (low part)
 *
 *  @param  u1BoostDb         3*2~12*2
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetBbeLoContour(UINT8 u1BoostDb)
{
    if (_u1BbeLoContour != u1BoostDb)
    {
        _u1BbeLoContour = u1BoostDb;
        DSP_SetBbeLoContour(u1BoostDb);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspSetBbe3dGain
 *   Set BBE ViVA 3D gain
 *
 *  @param  u13dGain         0 ~ 101
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void AUD_DspSetBbe3dGain(UINT8 u13dGain)
{
    if (_u1Bbe3dGain != u13dGain)
    {
        _u1Bbe3dGain = u13dGain;
        DSP_SetBbe3dGain(u13dGain);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspGetBbeMode
 *   Get BBE mode
 *
 *  @retval  BBE mode
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetBbeMode(void)
{
    return _u1BbeMode;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetBbeLevel
 *   Get BBE level
 *
 *  @retval  BBE level
 */
//-----------------------------------------------------------------------------
INT8 AUD_DspGetBbeLevel(void)
{
    return _i1BbeLevel;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetBbeProcess
 *   Get BBE Process
 *
 *  @retval  BBE Process
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetBbeProcess(void)
{
    return _u1BbeProcess;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetBbeLoContour
 *   Get BBE Lo Contour
 *
 *  @retval  BBE Lo Contour
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetBbeLoContour(void)
{
    return _u1BbeLoContour;
}

//-----------------------------------------------------------------------------
/** AUD_DspGetBbe3dGain
 *   Get BBE 3D gain
 *
 *  @retval  BBE 3D gain
 */
//-----------------------------------------------------------------------------
UINT8 AUD_DspGetBbe3dGain(void)
{
    return _u1Bbe3dGain;
}
#endif // #ifdef CC_AUD_BBE_SUPPORT

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
void AUD_DspSetCDNotchQandFc(UINT32 u4UserQIdx, UINT32 u4UserFc)
{
    static UINT32 _u4CDNotchQIdx = 0;
    static UINT32 _u4CDNotchFc = 0;
    if (u4UserQIdx != _u4CDNotchQIdx || u4UserFc != _u4CDNotchFc)
    {
        _u4CDNotchQIdx = u4UserQIdx;
        _u4CDNotchFc = u4UserFc;
        DSP_SetCDNotchQandFc(u4UserQIdx, u4UserFc);
    }
}
#endif

#ifdef  KARAOKE_SUPPORT
void AUD_DspSetKeyshiftFlag(BOOL fgflag)
{
    static BOOL _fgflag = FALSE;
    static BOOL _fgInit = FALSE;

    if (fgflag != _fgflag || _fgInit == FALSE)
    {
        _fgInit = TRUE;
        _fgflag = fgflag;
        DSP_SetKeyshiftFlag(fgflag);
    }
}
void AUD_DspSetKeyshiftKey(INT8 i1key)
{
    static INT8 _i1key = 0;

    if (i1key != _i1key)
    {
        _i1key = i1key;
        DSP_SetKeyshiftKey(i1key);
    }
}

#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetVolTable
 *   Set volume curve
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
#ifdef CC_SET_VOLUME
void _AUD_DspSetVolTable(UINT8 u1Idx, UINT32 u4Vol)
{
    if((u1Idx <= MAX_VOL_LEVEL)&&(u4Vol <= VOL_SHM_0_DB))
        _au4VOLUME[u1Idx]  = u4Vol;
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetVolumeTable
 *   Modify volume table according to user setting.
 *
 *  @param  u4VolTabChkPnt
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetVolumeTable(UINT32* u4VolTabChkPnt)
{
    //Andrew Wen 07/8/20
    UINT8 i;
    UINT8 j;
    UINT8 u1Step;
    UINT32 u2VolStep;

    for (i=0; i<VOL_TABLE_CHK_PNT; i++)
    {
        LOG(7,"u4VolTabChkPnt[%d] = %x\n", i, u4VolTabChkPnt[i]);
        _au4VOLUME[_au1VolStep[i]] = u4VolTabChkPnt[i];
        if (i < (VOL_TABLE_CHK_PNT-1))
        {
            u1Step = (_au1VolStep[i+1] - _au1VolStep[i]);
            u2VolStep = u4VolTabChkPnt[i+1] - u4VolTabChkPnt[i];
            if (u2VolStep >  0)
            {
                u2VolStep = u2VolStep / u1Step;
            }

            for (j = (_au1VolStep[i]+1) ; j < _au1VolStep[i+1] ; j++)
            {
                if (j>MAX_VOL_LEVEL)
                {
                    LOG(0, "Inocrrect table index when setting vlume table\n");
                    break;
                }
                _au4VOLUME[j] = _au4VOLUME[j-1] + u2VolStep;
            }
        }
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetVolumeTable
 *   Query volume table.
 *
 *  @param  u1VolTabIdx
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetVolumeTable(UINT8 u1VolTabIdx)
{
    return _au4VOLUME[u1VolTabIdx];
}

#ifdef CC_AUD_NEW_CV_TABLE
#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
void _AUD_DspSetCVTable(UINT8 u1Idx,UINT32 u4Vol)
{
    if((u1Idx <= MAX_VOL_LEVEL)&&(u4Vol <= MAX_VOL_SHM))
        _au4CHVOLUME[u1Idx] = u4Vol;
}
UINT32 _AUD_DspGetCVTable(UINT8 u1VolTabIdx)
{
    return _au4CHVOLUME[u1VolTabIdx];
}

void _AUD_DspSetStepCVTable(UINT32* u4VolTabChkPnt)
{
    //Andrew Wen 07/8/20
    UINT8 i;
    UINT8 j;
    UINT8 u1Step;
    UINT32 u2VolStep;

    for (i=0; i<VOL_TABLE_CHK_PNT; i++)
    {
        LOG(7,"u4ChannelVolTabChkPnt[%d] = %x\n", i, u4VolTabChkPnt[i]);
        _au4CHVOLUME[_au1VolStep[i]] = u4VolTabChkPnt[i];
        if (i < (VOL_TABLE_CHK_PNT-1))
        {
            u1Step = (_au1VolStep[i+1] - _au1VolStep[i]);
            u2VolStep = u4VolTabChkPnt[i+1] - u4VolTabChkPnt[i];
            if (u2VolStep >  0)
            {
                u2VolStep = u2VolStep / u1Step;
            }

            for (j = (_au1VolStep[i]+1) ; j < _au1VolStep[i+1] ; j++)
            {
                if (j>MAX_VOL_LEVEL)
                {
                    LOG(0, "Inocrrect table index when setting channel vlume table\n");
                    break;
                }
                _au4CHVOLUME[j] = _au4CHVOLUME[j-1] + u2VolStep;
            }
        }
    }
}

#else
void _AUD_DspSetCVTable(AUD_NVM_VOL_TBL_T rVolTbl, UINT8 u1Idx,UINT32 u4Vol)
{
    UINT64 u8ChlVol;
    
    if((u1Idx <= MAX_VOL_LEVEL)&&(u4Vol <= MAX_VOL_SHM))
    {
        _au4CHVOLUME[rVolTbl][u1Idx] = u4Vol;

        if (rVolTbl == AUD_NVM_COMMON_VOL_TBL)
        {
            u8ChlVol = ((UINT64)_au4CHVOLUME[rVolTbl][u1Idx]) * (UINT64)VOL_SHM_0_DB;
            _au4VOLUME[u1Idx] = (UINT32)u8Div6432(u8ChlVol, (UINT64)_au4CHVOLUME[rVolTbl][MAX_VOL_LEVEL], NULL);
        }
    }
}
UINT32 _AUD_DspGetCVTable(AUD_NVM_VOL_TBL_T rVolTbl, UINT8 u1VolTabIdx)
{
    return _au4CHVOLUME[rVolTbl][u1VolTabIdx];
}

void _AUD_DspSetStepCVTable(AUD_NVM_VOL_TBL_T rVolTbl, UINT32* u4VolTabChkPnt)
{
    //Andrew Wen 07/8/20
    UINT8 i;
    UINT8 j;
    UINT8 u1Step;
    UINT32 u2VolStep;
    UINT64 u8ChlVol;
    
    for (i=0; i<VOL_TABLE_CHK_PNT; i++)
    {
        LOG(7,"u4ChannelVolTabChkPnt[%d] = %x\n", i, u4VolTabChkPnt[i]);
        _au4CHVOLUME[rVolTbl][_au1VolStep[i]] = u4VolTabChkPnt[i];
        if (i < (VOL_TABLE_CHK_PNT-1))
        {
            u1Step = (_au1VolStep[i+1] - _au1VolStep[i]);
            u2VolStep = u4VolTabChkPnt[i+1] - u4VolTabChkPnt[i];
            if (u2VolStep >  0)
            {
                u2VolStep = u2VolStep / u1Step;
            }

            for (j = (_au1VolStep[i]+1) ; j < _au1VolStep[i+1] ; j++)
            {
                if (j>MAX_VOL_LEVEL)
                {
                    LOG(0, "Inocrrect table index when setting channel vlume table\n");
                    break;
                }
                _au4CHVOLUME[rVolTbl][j] = _au4CHVOLUME[rVolTbl][j-1] + u2VolStep;
            }
        }
    }

    if (rVolTbl == AUD_NVM_COMMON_VOL_TBL)
    {
        for (i = 0; i <= MAX_VOL_LEVEL; i++)
        {
            u8ChlVol = ((UINT64)_au4CHVOLUME[rVolTbl][i]) * (UINT64)VOL_SHM_0_DB;
            _au4VOLUME[i] = (UINT32)u8Div6432(u8ChlVol, (UINT64)_au4CHVOLUME[rVolTbl][MAX_VOL_LEVEL], NULL);
        }
    }
}
#endif
#endif

#ifdef CC_AUD_NEW_HP_VOL_TABLE
void _AUD_DspSetHpVolumeTable(UINT8 u1Idx,UINT32 u4Vol)
{
    if((u1Idx <= 100)&&(u4Vol <=0x20000))
    {
        _au4HpVolume[u1Idx]  = u4Vol;
    }
}
UINT32 _AUD_DspGetHpVolumeTable(UINT8 u1VolTabIdx)
{
    return _au4HpVolume[u1VolTabIdx];
}
#endif

#if 0   // Unused
void _AUD_DspSetPCMChannelNumber(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2ChannelNo)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    if(u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT16(u1DspId, W_PCM_INPUT_CHANNEL_NUM, u2ChannelNo);
    }
    else if(u1DecId == AUD_DEC_AUX)
    {
        vWriteShmUINT16(u1DspId, W_PCM_INPUT_CHANNEL_NUM_DEC2, u2ChannelNo);
    }
    else if(u1DecId == AUD_DEC_THIRD)
    {
        vWriteShmUINT16(u1DspId, W_PCM_INPUT_CHANNEL_NUM_DEC3, u2ChannelNo);
    }
    else if(u1DecId == AUD_DEC_4TH)
    {
        vWriteShmUINT16(u1DspId, W_PCM_INPUT_CHANNEL_NUM_DEC4, u2ChannelNo);
    }
}
#endif

#ifdef CC_AUD_USE_NVM
//-----------------------------------------------------------------------------
/** _AUD_DspGetVolumeLevelFromShm
 *   Get volume level from shm.
 *
 *  @param  u4VolTabChkPnt
 *
 *  @param  u4VolShm
 *
 *  @retval  UINT8
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetVolumeLevelFromShm(UINT32* u4VolTabChkPnt, UINT32 u4VolShm)
{
    UINT8 i;
    UINT8 u1BaseLevel;

    for (i=0; i<VOL_TABLE_CHK_PNT; i++)
    {
        if (u4VolTabChkPnt[i] >= u4VolShm)
        {
            break;
        }
    }

    if (i == 0)
    {
        u1BaseLevel = _au1VolStep[0];
    }
    else if (i == VOL_TABLE_CHK_PNT)
    {
        u1BaseLevel = _au1VolStep[VOL_TABLE_CHK_PNT-1];
    }
    else
    {
        u1BaseLevel = _au1VolStep[i-1] + (u4VolShm-u4VolTabChkPnt[i-1])/(
            (u4VolTabChkPnt[i]-u4VolTabChkPnt[i-1])/(_au1VolStep[i]-_au1VolStep[i-1]));

        if (u1BaseLevel > MAX_VOL_LEVEL) u1BaseLevel = MAX_VOL_LEVEL;

        if (u1BaseLevel < MAX_VOL_LEVEL &&
            #ifdef CC_AUD_NEW_CV_TABLE
            #ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
            _au4CHVOLUME[u1BaseLevel+1] - u4VolShm < u4VolShm - _au4CHVOLUME[u1BaseLevel]
            #else
            _au4CHVOLUME[AUD_NVM_COMMON_VOL_TBL][u1BaseLevel+1] - u4VolShm < u4VolShm - _au4CHVOLUME[AUD_NVM_COMMON_VOL_TBL][u1BaseLevel]
            #endif
            #else
            _au4VOLUME[u1BaseLevel+1] - u4VolShm < u4VolShm - _au4VOLUME[u1BaseLevel]
            #endif
        )
            {
                u1BaseLevel++;
            }

        #ifdef CC_AUD_NEW_CV_TABLE
        #ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
        _au4CHVOLUME[u1BaseLevel] = u4VolShm;
        #else
        _au4CHVOLUME[AUD_NVM_COMMON_VOL_TBL][u1BaseLevel] = u4VolShm;
        #endif
        #else
        _au4VOLUME[u1BaseLevel] = u4VolShm;
        #endif
    }

    return u1BaseLevel;
}
#endif

#if 0   // Unused
//-----------------------------------------------------------------------------
/** _AUD_DspGetVolumeBaseLevel
 *   Get volume base level (0dB level).
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_DspGetVolumeBaseLevel(void)
{
    return _u1VolBaseLevel;
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_Surround_Get_Config
 *   Get new mtk vsurr config
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_Surround_Get_Config(AUD_VSURR_CFG_T* prVsurrCfg)
{
    DSP_Get_VSurr_Cfg(prVsurrCfg);
}

//-----------------------------------------------------------------------------
/** _AUD_Surround_Set_Config
 *   Set new mtk vsurr config
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_Surround_Set_Config(AUD_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4VSParameter)
{
    DSP_Set_VSurr_Cfg(eVSCfgType,u4VSParameter);
}

//-----------------------------------------------------------------------------
/** _AUD_3D_AVLINK_Config
 *   transform 3D-video depth to the "width setting" of mtk vsurr
 *  u4DepthField : 0~32
 *  @retval  void
 */
//-----------------------------------------------------------------------------
 //audio depths links with video depths, by gallen/sammy, 20110526
#ifdef CC_3DTV_AUDIO_LINK_SUPPORT
void _AUD_3D_AVLINK_Config(UINT32 u4DepthField)
{
    UINT32 u4VSParameter;

    if (u4DepthField > 32) u4DepthField = 32;
    // Note Width       : Sound Width,          0 ~ 0x500000\n");
    // The mapping of "u4DepthField" to "Sound Width" shall be returned on customer's set.s
    u4VSParameter = (32-u4DepthField)*65536+0x200000;
    //[0, 32] -> [0x400000, 0x200000];

    DSP_Set_VSurr_Cfg(AUD_SUR_WIDTH,u4VSParameter);
    LOG(4, "Set MVS according to 3D video depth, depth = %d!\n", u4DepthField);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetPcmPreBufferLength
 *   Set PCM pre-buffering length
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetPcmPreBufferLength(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2Length)
{
    UNUSED(u1DecId);

    vWriteShmUINT16(u1DspId, W_PCM_PREBUF_DELAY_BANK, u2Length);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetPcmPreBufferLength
 *   Get PCM pre-buffering length
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
UINT16 _AUD_DspGetPcmPreBufferLength(UINT8 u1DspId, UINT8 u1DecId)
{
    UNUSED(u1DecId);

    return u2ReadShmUINT16(u1DspId, W_PCM_PREBUF_DELAY_BANK);
}

//-----------------------------------------------------------------------------
/** _AUD_GetNicamStatus
 *   Get Nicam exists or not
 *
 *  @retval  TURE/FALSE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_GetNicamStatus(void)
{
    AUD_TYPE_T      _eAudType;
    _eAudType = DSP_GetAudChlInfo(AUD_DEC_AUX);
    if ((_eAudType == AUD_TYPE_FM_MONO_NICAM_MONO)
        ||(_eAudType == AUD_TYPE_FM_MONO_NICAM_STEREO)
        ||(_eAudType == AUD_TYPE_FM_MONO_NICAM_DUAL))
    {
         LOG(0,"NICAM exists\n");
        return TRUE;
    }
    else
    {
        LOG(0,"NICAM doesn't exist\n");
        return FALSE;
    }
}

#ifdef SUPPORT_PHILIPS_LOUDNESS
AUD_TYPE_T _AUD_GetAudInfo(UINT8 u1DecId)
{
    return DSP_GetAudChlInfo(u1DecId);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_GetDtvAudInfo
 *   Get Nicam exists or not
 *
 *  @retval  TURE/FALSE
 */
//-----------------------------------------------------------------------------

void _AUD_GetDtvAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    DSP_GetDtvAudInfo( u1DecId, prAudInfo);
}

//-----------------------------------------------------------------------------
/** AUD_DspSpeed
 *  Set dsp speed (non-fractional)
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  u1Speed  : 0:normal play
 *                            : 2:FF 2-times
 *                            : 4:FF 4-times
 *                            : 8:FF 8-times
 *                            : 16:FF 16-times
 *                            : 32:FF 32-times
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSpeed(UINT8 u1DecId, UINT16 u1Speed)
{
    UINT16 u2ShmIndex = W_SPEED;
    UINT32 u2UopIndex = UOP_DSP_SPEED;

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndex = W_SPEED_DEC2;
        u2UopIndex = UOP_DSP_SPEED_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndex = W_SPEED_DEC3;
        u2UopIndex = UOP_DSP_SPEED_DEC3;
    }
	
    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u1Speed);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
    
    LOG(3, "_AUD_DspSpeed, u1DecId = %d, u1Speed = %d\n", u1DecId, u1Speed);
}

typedef struct {
	UINT32 u4InitNo;
	UINT32 u4InitOffset;
	UINT32 u4VarNo;
	UINT32 u4VarOffset;
} APROC_LGSE_NO_PARAM_T;

APROC_LGSE_NO_PARAM_T Aproc_Lgse_ParamNo_Block[HAL_APROC_LGSE_NUM] = 
{
	{10, 0, 3, 0},   // MAIN
	{295, 10, 1, 3}, // FN000
	{21, 305, 130, 4}, // FN001
	{0, 326, 25, 134}, // FN004_mode1
	{0, 326, 15, 159},  // FN004_mode2
	{10, 326, 2, 174}, // FN009
	{10, 336, 1, 176},  // FN010
	{0, 346, 3, 177}, // MODE
	{0, 0, 0, 0}
};

typedef enum
{
	AUDIO_LGSE_MODE_VARIABLES0	= 0, // "VARIABLES_00"will be written
	AUDIO_LGSE_MODE_VARIABLES1	= 1, // "VARIABLES_01"will be written
	AUDIO_LGSE_MODE_VARIABLES2	= 2, // "VARIABLES_02"will be written
	AUDIO_LGSE_MODE_VARIABLES3	= 3, // "VARIABLES_03"will be written
	AUDIO_LGSE_MODE_VARIABLES4	= 4, // "VARIABLES_04"will be written
	AUDIO_LGSE_MODE_VARIABLESALL= 5  // All "VARIABLES" will be written simultaneously. Data will be arranged from 0 to 4.
} AUDIO_LGSE_VARIABLE_MODE_T;

#ifdef CC_AUD_DDI
void _AUD_LGSEFN010Out(UINT32* pParam)
{
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
	UINT32 u4Reg;

	u4Reg = 0x1;
	vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_LGSE_0, APROC_REG_LGSE_GET_FN010_OUT), u4Reg);
	x_thread_delay(50);
	pParam[0] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_LGSE_OUT, APROC_REG_LGSE_FN010_LGSE00663_0));
	pParam[1] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_LGSE_OUT, APROC_REG_LGSE_FN010_LGSE00663_1));
	Printf ("[LGSE][OUT] _AUD_LGSEFN010Out: pParam[0] = 0x%x, pParam[1] = 0x%x\n", pParam[0], pParam[1]);
#endif
}

void _AUD_LGSEFN000(UINT8 fNo, VOID* u1CV_param_buf, UINT16 noParam, UINT8 dataOption, UINT8 varOption)
{
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
	UINT32 u4Reg;
	UINT32 u4StartAddr;
	UINT32 i;
	UINT32 *pu4Value;
	UINT32 u4Mode;
#else
	static UINT32 u4state = 0xfffe;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
#endif
    _argLgseFnPara[fNo].isUpdate = 1;
    //_argLgseFnPara[fNo].pParams = u1CV_param_buf;
    _argLgseFnPara[fNo].noParam = noParam;
    _argLgseFnPara[fNo].dataOption = dataOption;
    _argLgseFnPara[fNo].varOption = varOption;

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
	// 1. Must set the value first (If set mask flag first, there will be timing issue)
	if (_argLgseFnPara[fNo].dataOption == ADEC_LGSE_INIT_ONLY)
	{
		if(fNo == HAL_APROC_LGSE_MODE)
		{
			Printf ("[LGSE] Mode cannot be init!\n");
			return;
		}

		u4StartAddr = APROC_ASM_ADDR(APROC_ASM_ID_LGSE_INIT, 0);

		if (_argLgseFnPara[fNo].noParam != Aproc_Lgse_ParamNo_Block[fNo].u4InitNo)
		{
			Printf ("[LGSE][INIT] Number of init param not match. _argLgseFnPara[%d].noParam=%d, Aproc_Lgse_ParamNo_Block[%d].u4InitNo=%d\n", fNo, _argLgseFnPara[fNo].noParam, fNo, Aproc_Lgse_ParamNo_Block[fNo].u4InitNo);
			return;
		}
	}
	else if (_argLgseFnPara[fNo].dataOption == ADEC_LGSE_VARIABLES)
	{
		u4StartAddr = APROC_ASM_ADDR(APROC_ASM_ID_LGSE_VAR, 0);

		if ((_argLgseFnPara[fNo].noParam != Aproc_Lgse_ParamNo_Block[fNo].u4VarNo) && (fNo != 3))
		{
			Printf ("[LGSE][VAR] Number of var param not match. _argLgseFnPara[%d].noParam = %d, Aproc_Lgse_ParamNo_Block[%d].u4VarNo=%d\n", fNo, _argLgseFnPara[fNo].noParam, fNo, Aproc_Lgse_ParamNo_Block[fNo].u4VarNo);
			return;
		}
		if ((fNo == 3) && (_argLgseFnPara[fNo].varOption != AUDIO_LGSE_MODE_VARIABLESALL) && (_argLgseFnPara[fNo].noParam != 5))
		{
			Printf ("[LGSE][VAR] Number of var param not match. _argLgseFnPara[%d].noParam = %d\n", fNo, _argLgseFnPara[fNo].noParam);
			return;
		}
		if ((fNo == 3) && (_argLgseFnPara[fNo].varOption == AUDIO_LGSE_MODE_VARIABLESALL) && (_argLgseFnPara[fNo].noParam != Aproc_Lgse_ParamNo_Block[fNo].u4VarNo))
		{
			Printf ("[LGSE][VAR] Number of var param not match. _argLgseFnPara[%d].noParam = %d, Aproc_Lgse_ParamNo_Block[%d].u4VarNo=%d\n", fNo, _argLgseFnPara[fNo].noParam, fNo, Aproc_Lgse_ParamNo_Block[fNo].u4VarNo);
			return;
		}
		if(fNo == HAL_APROC_LGSE_MODE)
		{	
			u4Mode = 0x0;
			u4Mode = ((*((UINT32 *)_argLgseFnPara[fNo].pParams) & 0x00ffffff));
			vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_LGSE_0, APROC_REG_LGSE_MODIFIED_MODE), u4Mode);
		}
	}
	
	pu4Value = (UINT32 *)_argLgseFnPara[fNo].pParams;
	for (i = 0; i < _argLgseFnPara[fNo].noParam; i++)
	{
		if (_argLgseFnPara[fNo].dataOption == ADEC_LGSE_INIT_ONLY)
		{
			vAprocReg_Write ((u4StartAddr + Aproc_Lgse_ParamNo_Block[fNo].u4InitOffset + i), pu4Value[i]);
		}
		else if (_argLgseFnPara[fNo].dataOption == ADEC_LGSE_VARIABLES)
		{
			if ((fNo == 3) && (_argLgseFnPara[fNo].varOption == AUDIO_LGSE_MODE_VARIABLESALL)) // 25 vars
			{
				vAprocReg_Write ((u4StartAddr + Aproc_Lgse_ParamNo_Block[fNo].u4VarOffset + i), pu4Value[i]);
			}
			else if ((fNo == 3) && (_argLgseFnPara[fNo].varOption != AUDIO_LGSE_MODE_VARIABLESALL)) // 5vars
			{
				vAprocReg_Write ((u4StartAddr + Aproc_Lgse_ParamNo_Block[fNo].u4VarOffset + (5 * _argLgseFnPara[fNo].varOption)+ i), pu4Value[i]);
			}
			else
			{
				vAprocReg_Write ((u4StartAddr + Aproc_Lgse_ParamNo_Block[fNo].u4VarOffset + i), pu4Value[i]);
			}
		}
	}

	// 2. Set the mask flag
	if (_argLgseFnPara[fNo].dataOption == ADEC_LGSE_INIT_ONLY)
	{
		u4Reg = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_LGSE_0, APROC_REG_LGSE_MODIFIED_INIT));
		u4Reg |= (1<<fNo);
		vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_LGSE_0, APROC_REG_LGSE_MODIFIED_INIT), u4Reg);
		Printf ("[LGSE][INIT] fNo=%d, noParam=%d, pParams[0]=0x%x\n", fNo, _argLgseFnPara[fNo].noParam, *((UINT32 *)_argLgseFnPara[fNo].pParams));
	}
	else if (_argLgseFnPara[fNo].dataOption == ADEC_LGSE_VARIABLES)
	{
		u4Reg = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_LGSE_0, APROC_REG_LGSE_MODIFIED_VAR));
		u4Reg |= (1<<fNo);
		vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_LGSE_0, APROC_REG_LGSE_MODIFIED_VAR), u4Reg);
		Printf ("[LGSE][VAR] fNo=%d noParam=%d, pParams[0]=0x%x\n", fNo, _argLgseFnPara[fNo].noParam, *((UINT32 *)_argLgseFnPara[fNo].pParams));
	}
#else

//case 1 (FN000/FN007/FN009 & ADEC_LGSE_VARIABLES) ' vAudPostSetParas (Set common DRAM)
//case 2 (Others) ' vAudPostSetParas 'reinit ' fadeout ' vDSPUpdateCommDramLGSE ' fadein
/*
      vAudPostSetParas(  fNo, _argLgseFnPara[fNo].pParams, _argLgseFnPara[fNo].noParam,
                                       _argLgseFnPara[fNo].dataOption, _argLgseFnPara[fNo].varOption);
*/
//vAudPostSetParas mark temp!!!!!!!!!!!!!!!! by xiaoyi 20120305
#ifdef CC_LGSE_WORK_AROUND
    if (fNo == SE2012_FUNC_UNKNOWN1)
    {
        DSP_LGSEFN009_Hack(_argLgseFnPara[fNo].pParams, _argLgseFnPara[fNo].noParam, _argLgseFnPara[fNo].dataOption);
    }
#endif
   AUD_DRVGetStreamFrom(AUD_DSP0, AUD_DEC_MAIN, &eStreamFrom);
#if 0//def CC_ENABLE_AOMX
   if((fNo == SE2012_FUNC_UNKNOWN1) && (_argLgseFnPara[fNo].dataOption == ADEC_LGSE_VARIABLES) )//&& eStreamFrom == AUD_STREAM_FROM_MEMORY)
    {
        UINT32 *u4Gain;
        UINT32 u4Volumn;
        UINT64 u8ChlVol;

        u4Gain = _argLgseFnPara[fNo].pParams;
        u4Volumn = _au4VOLUME[u4Gain[1]];
        if (u4Volumn !=  0)
            u4Volumn = _au4LGSEVOLUME[u4Gain[1]];
        // Caculate extra gain
        u8ChlVol = (UINT64)u4Volumn * ((UINT64)(_DbToShm(_aai1ChlVolExtraGain[AUD_DEC_MAIN][AUD_CH_FRONT_LEFT])));
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
        if (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)//japan clip volume
        {
            u4Volumn = u4Volumn/2;
        }
        if (AUD_GetMultimediaMode(AUD_DEC_MAIN) != AUD_MM_MODE_GAME)
        {
            AUD_DspSetGstGain(u4Volumn);
        }

    }
#endif


   if(fNo == SE2012_FUNC_SETTING)
   {
       UINT32 *tmp = (UINT32 *)_argLgseFnPara[fNo].pParams;
       if(tmp[0] == u4state)
        {
            return;
        }
        u4state = tmp[0];
   }
    if ( (fNo == SE2012_CLEARVOICE) ||
          (fNo == SE2012_FUNC_UNKNOWN) ||
          (fNo == SE2012_FUNC_UNKNOWN1))
   {
        if(_argLgseFnPara[fNo].dataOption == ADEC_LGSE_VARIABLES)
        {
            return;
        }
   }
   //20111219 no short mute for user eq (PEQ) setting
   if((fNo == SE2012_PEQMODE1) &&
     (_argLgseFnPara[fNo].dataOption != ADEC_LGSE_INIT_ONLY)&&
     (_argLgseFnPara[fNo].varOption != ADEC_LGSE_MODE_VARIABLESALL)){
            LOG(2, "Re-Init bypassed for the USER EQ!\n");
            return;
   }
   DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_CV);
#endif
}
#endif

/** _AUD_GetAC3_EAC3_Info
 *
 *  @retval  u1version
 */
//-----------------------------------------------------------------------------

UINT8 _AUD_GetAC3_EAC3_Info(UINT8 u1DecId)
{
    UINT32 u1DDP=0;
    UINT8 u1Version=0xff;
    if(u1DecId== AUD_DEC_MAIN)
    {
        u1DDP=(UINT32)(u4ReadDspSram(AUD_DSP0, 0x3500)>>8);
    }
    else if(u1DecId==AUD_DEC_AUX)
    {
        u1DDP=(UINT32)(u4ReadDspSram(AUD_DSP0, 0x1c80)>>8);
    }
    if(u1DDP==0x24)
    {
        u1Version=0x0;
    }
    if(u1DDP==0x25)
    {
        u1Version=0x1;
    }
    return u1Version;
}

void _AUD_DspGetSpeed(UINT8 u1DecId, UINT16 *u1Speed)
{
    UINT16 u2ShmIndex = W_SPEED;

    UNUSED(_AUD_DspGetSpeed);

    if (u1DecId != AUD_DEC_MAIN)
    {
        u2ShmIndex = W_SPEED_DEC2;
    }

    *u1Speed = u2ReadShmUINT16(AUD_DSP0, u2ShmIndex);
}


//-----------------------------------------------------------------------------
/** _AUD_DspDownmixPosition
 *  Set Downmix Channel (CH9/10) Position
 *
 *  @param  u1DMPosition   : 0: do ALL post processing
 *                         : 1: do downmix after input source gain and before AVC
 *                         : 2: Skip post-proc & do manual output matrix
 *                         : 3: Skip post-proc & skip manual output matrix
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspDownmixPosition(UINT8 u1DspId, UINT8 u1DMPosition)
{
    UINT32 u2UopIndex = UOP_DSP_DOWNMIX_POSITION;
    UINT16 u4Config = 0;
    if (u1DMPosition == 0)
    {
    u4Config |= 0x0 << 4;  // Downmix channel(CH9/10) do ALL post processing
    }
    if (u1DMPosition == 1)
    {
    u4Config |= 0x1 << 4; //doing downmix after input source gain and before AVC
    }
    if (u1DMPosition == 2)
    {
    u4Config |= 0x2 << 4;
    }
    if (u1DMPosition == 3)
    {
    u4Config |= 0x3 << 4; // Downmix channel(CH9/10) do not do post processing
    }
    vWriteShmUINT16(u1DspId, W_SYSTEM_SETUP, (u2ReadShmUINT16(u1DspId, W_SYSTEM_SETUP) & 0xFFCF) | u4Config);
    DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
}

UINT8 _AUD_DspGetDownmixPosition(UINT8 u1DspId)
{
    return ((u2ReadShmUINT16(u1DspId, W_SYSTEM_SETUP)>>4)&0x3);
}

//-----------------------------------------------------------------------------
/** AUD_DspMultiPairOutput
 *  Copy CH9/10 to other channel(LS/RS/C/SUB)
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1MultiPair   : 0: OFF
 *                                   : 1: ON

 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspMultiPairOutput(UINT8 u1DspId, UINT8 u1DecId, UINT16 u1MultiPair)
{
     UINT32 u2UopIndex = UOP_DSP_DOWNMIX_POSITION;
    UINT16 u4Config = 0;
    if (u1DecId == AUD_DEC_MAIN)
    {
      if (u1MultiPair == 0)
      {
        u4Config |= 0x0 << 3;  // Turn off bit 3
        vWriteShmUINT16(u1DspId, W_SYSTEM_SETUP, (u2ReadShmUINT16(u1DspId, W_SYSTEM_SETUP) & 0xFFF7) | u4Config);
        DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
        _AUD_DspSpeakerOutputConfig(AUD_DEC_MAIN,0x2); //set speaker cfg to be 3/2  channel

      }
      if (u1MultiPair == 1)
      {
        u4Config |= 0x1 << 3; //Turn on bit3
        vWriteShmUINT16(u1DspId, W_SYSTEM_SETUP, (u2ReadShmUINT16(u1DspId, W_SYSTEM_SETUP) & 0xFFF7) | u4Config);
        DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
         vWriteShmUINT32(u1DspId, D_SPKCFG, (u4ReadShmUINT32(u1DspId, D_SPKCFG) & 0xffffffdf) | ((UINT32)0x1<<5) );//turn on subwoofer channel
        _AUD_DspSpeakerOutputConfig(AUD_DEC_MAIN,0x7); //set speaker cfg to be 2/0  channel
      }
    }
}

//-----------------------------------------------------------------------------
/** _AUD_DspLRDownmix
 *  Downmix to L/R channel after Reverbation and before BassManagement
 *
 *  @param  u1DecId 0       : first decoder 1: seconder decoder.
 *  @param  u1MultiPair   : 0: OFF
 *                                   : 1: ON

 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspLRDownmix(UINT8 u1DspId, UINT8 u1DecId, BOOL fgEnable)
{
    UINT32 u2UopIndex = UOP_DSP_DOWNMIX_POSITION;
    UINT16 u4Config = 0;
    if (u1DecId == AUD_DEC_MAIN)
    {
        if (fgEnable)
        {
            u4Config |= 0x1 << 6;  // Turn on bit 6
        }
        else
        {
            u4Config |= 0x0 << 6; //Turn off bit6
        }
        vWriteShmUINT16(u1DspId, W_SYSTEM_SETUP, (u2ReadShmUINT16(u1DspId, W_SYSTEM_SETUP) & 0xFFBF) | u4Config);
        DSP_SendDspTaskCmd(u1DspId, u2UopIndex);
    }
}

UINT16 _AUD_DspGetSystemSetup(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT32 u2ShmIndx = W_SYSTEM_SETUP;

    if (u1DecId == AUD_DEC_AUX)
    {
        u2ShmIndx = W_SYSTEM_SETUP_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u2ShmIndx = W_SYSTEM_SETUP_DEC3;
    }
    else if (u1DecId == AUD_DEC_4TH)
    {
        u2ShmIndx = W_SYSTEM_SETUP_DEC4;
    }

    return u2ReadShmUINT16(u1DspId, u2ShmIndx);
}

//-----------------------------------------------------------------------------
/** AUD_DspFLACHeader
 *  Set dsp FLAC header
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  prWmaInfo Wma header information
 *  @retval void
 *  paul_flac
 */
//-----------------------------------------------------------------------------
void _Aud_DspFLACHeader(UINT8 u1DecId, AUD_FLAC_INFO_T * prFlacInfo)
{
#if 0
    if (AUD_DEC_MAIN != u1DecId)
    {
        return;
    }
    else
    {
#endif
        vWriteShmUINT16(AUD_DSP0, W_FLAC_MIN_BLK_SZ, prFlacInfo->u2MinBlkSz);
        vWriteShmUINT16(AUD_DSP0, W_FLAC_MAX_BLK_SZ, prFlacInfo->u2MaxBlkSz);
        vWriteShmUINT32(AUD_DSP0, W_FLAC_CHN_NUM, prFlacInfo->u2ChannelNum);
        vWriteShmUINT32(AUD_DSP0, W_FLAC_BPS, prFlacInfo->u2BPS);
        vWriteShmUINT16(AUD_DSP0, D_FLAC_MIN_FRM_SZ, prFlacInfo->u4MinFrmSz);
        vWriteShmUINT16(AUD_DSP0, D_FLAC_MAX_FRM_SZ, prFlacInfo->u4MaxFrmSz);
        vWriteShmUINT32(AUD_DSP0, D_FLAC_SMP_RATE, prFlacInfo->u4SmpRate);

        LOG(5, "[FLAC Setting]\n");
        LOG(5, "  u2MinBlkSz: %x\n", prFlacInfo->u2MinBlkSz);
        LOG(5, "  u2MaxBlkSz: %x\n", prFlacInfo->u2MaxBlkSz);
        LOG(5, "  u2ChannelNum: %x\n", prFlacInfo->u2ChannelNum);
        LOG(5, "  u2BPS: %x\n",  prFlacInfo->u2BPS);
        LOG(5, "  u4MinFrmSz: %x\n", prFlacInfo->u4MinFrmSz);
        LOG(5, "  u4MaxFrmSz: %x\n", prFlacInfo->u4MaxFrmSz);
        LOG(5, "  u4SmpRate: %x\n", prFlacInfo->u4SmpRate);
//  }
}

#ifdef DATA_DISC_WMA_SUPPORT
//-----------------------------------------------------------------------------
/** AUD_DspWMAHeader
 *  Set dsp WMA header
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  prWmaInfo Wma header information
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspWMAHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo)
{
    //if (u1DecId != AUD_DEC_MAIN)
    //{
    //    return; //currently we only support WMA in dec1
    //}
    if (u1DecId == AUD_DEC_4TH)
    {
        return; //currently not support WMA in dec4
    }
    if (prWmaInfo->u2HdrSrc == 0){
        vWriteShmUINT16(AUD_DSP0, W_WMA_PACKET_NO + u1DecId*32, prWmaInfo->u2Pktno);
        vWriteShmUINT16(AUD_DSP0, W_WMA_HEADER_SOURCE + u1DecId*32, 0);
    }else{
        vWriteShmUINT16(AUD_DSP0, W_WMA_PACKET_NO + u1DecId*32, 0);
        vWriteShmUINT16(AUD_DSP0, W_WMA_HEADER_SOURCE + u1DecId*32, 1);
        vWriteShmUINT32(AUD_DSP0, D_WMA_PACKET_COUNT + u1DecId*32, 0x7fff);
        vWriteShmUINT32(AUD_DSP0, D_WMA_PACKET_SIZE + u1DecId*32, prWmaInfo->u4Pktsz);
        vWriteShmUINT16(AUD_DSP0, W_WMA_VERSION + u1DecId*32, 0x161);
        vWriteShmUINT16(AUD_DSP0, W_WMA_NUM_CH + u1DecId*32, prWmaInfo->u2Numch);
        vWriteShmUINT32(AUD_DSP0, D_WMA_SAMPLE_PER_SEC + u1DecId*32, prWmaInfo->u4Sampersec);
        vWriteShmUINT32(AUD_DSP0, D_WMA_BYTE_PER_SEC + u1DecId*32, prWmaInfo->u4Bpersec);
        vWriteShmUINT32(AUD_DSP0, D_WMA_BLOCK_SIZE + u1DecId*32, prWmaInfo->u4Blocksz);
        vWriteShmUINT16(AUD_DSP0, W_WMA_ENCODER_OPTION + u1DecId*32, prWmaInfo->u2Encoderopt);

        LOG(5, "[WMA Setting]\n");
        LOG(5, "  u4Pktsz: 0x%x\n", prWmaInfo->u4Pktsz);
        LOG(5, "  u2NumCh: 0x%x\n", prWmaInfo->u2Numch);
        LOG(5, "  u4Sampersec: 0x%x\n", prWmaInfo->u4Sampersec);
        LOG(5, "  u4Bpersec: 0x%x\n",  prWmaInfo->u4Bpersec);
        LOG(5, "  u4Blocksz: 0x%x\n", prWmaInfo->u4Blocksz);
        LOG(5, "  u2Encoderopt: 0x%x\n", prWmaInfo->u2Encoderopt);
    }
}
#ifdef CC_53XX_SWDMX_V2
void AUD_DrvSetWMAErrFunc(UINT8 u1DecId, AUD_WMAErr_NOTIFY pfWMAErrNotify, VOID *prInst){  // megaa 20090828
    UNUSED(u1DecId);
    UNUSED(pfWMAErrNotify);
    UNUSED(prInst);
    _hWMAErrNotifyFunc =  pfWMAErrNotify;
    _prWMAErrInst = prInst;  // megaa 20090828
}
#else
void AUD_DrvSetWMAErrFunc(UINT8 u1DecId, AUD_WMAErr_NOTIFY pfWMAErrNotify){
    UNUSED(u1DecId);
    UNUSED(pfWMAErrNotify);
    _hWMAErrNotifyFunc =  pfWMAErrNotify;
}
#endif
#endif //DATA_DISC_WMA_SUPPORT

#if 1 //CC_APE_SUPPORT
//ian APE decoder
//-----------------------------------------------------------------------------
/** AUD_DspAPEHeader
 *  Set dsp APE header
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  prWmaInfo APE header information
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAPEHeader(UINT8 u1DecId,AUD_APE_CFG_T* prApeInfo)
{
    if (u1DecId != AUD_DEC_MAIN)
    {
        LOG(0, "ape DecId invalid!!!\n");
        return; //currently we only support APE in dec1
    }
#if 0 //do the headerparsing in DSP
        vWriteShmUINT16(AUD_DSP0, W_APE_PARSE_AUDIO_ONLY,    1   );
        vWriteShmUINT16(AUD_DSP0, W_APE_MUTE_BANK_NUMBERS,   0);
#else
    vWriteShmUINT16(AUD_DSP0, W_APE_PARSE_AUDIO_ONLY,    prApeInfo->u2ParseAudOnly   );
    vWriteShmUINT16(AUD_DSP0, W_APE_FILE_VESION,               prApeInfo->u2FileVersion    );
    vWriteShmUINT32(AUD_DSP0, D_APE_COMPRESS_LEVEL,        prApeInfo->u4CompressLevel  );
    vWriteShmUINT32(AUD_DSP0, D_APE_BLOCK_PER_FRAME,      prApeInfo->u4BlockPerFrm    );
    vWriteShmUINT32(AUD_DSP0, D_APE_FINAL_FRAME_BLOCK,   prApeInfo->u4FinalFrmBlock  );
    vWriteShmUINT16(AUD_DSP0, W_APE_TOTAL_FRAME_NUM_LOW, prApeInfo->u2TotalFrmNumLow );
    vWriteShmUINT16(AUD_DSP0, W_APE_TOTAL_FRAME_NUM_HIGH,prApeInfo->u2TotalFrmNumHigh);
    vWriteShmUINT16(AUD_DSP0, W_APE_BITS_PER_SAMPLE,     prApeInfo->u2Bpersample     );
    vWriteShmUINT16(AUD_DSP0, W_APE_CHANNEL_NUM,         prApeInfo->u2CHNum          );
    vWriteShmUINT32(AUD_DSP0, D_APE_INPUT_SAMPLING_RATE, prApeInfo->u4SamplingRate   );
    //vWriteShmUINT16(AUD_DSP0, W_APE_MUTE_BANK_NUMBERS,   prApeInfo->u2MuteBankNum    );
    //vWriteShmUINT16(AUD_DSP0, W_APE_INVALID_BYTES,       prApeInfo->u2InvalidBytes   );
#endif
    LOG(5, "[AUD_CODEC]W_APE_PARSE_AUDIO_ONLY = 0x%x\n", prApeInfo->u2ParseAudOnly);
    LOG(5, "[AUD_CODEC]W_APE_FILE_VESION = 0x%x\n", prApeInfo->u2FileVersion);
    LOG(5, "[AUD_CODEC]D_APE_COMPRESS_LEVEL = 0x%x\n", prApeInfo->u4CompressLevel);
    LOG(5, "[AUD_CODEC]D_APE_BLOCK_PER_FRAME = 0x%x\n", prApeInfo->u4BlockPerFrm);
    LOG(5, "[AUD_CODEC]D_APE_FINAL_FRAME_BLOCK = 0x%x\n", prApeInfo->u4FinalFrmBlock);
    LOG(5, "[AUD_CODEC]W_APE_TOTAL_FRAME_NUM_LOW = 0x%x\n", prApeInfo->u2TotalFrmNumLow);
    LOG(5, "[AUD_CODEC]W_APE_TOTAL_FRAME_NUM_HIGH = 0x%x\n", prApeInfo->u2TotalFrmNumHigh);
    LOG(5, "[AUD_CODEC]W_APE_BITS_PER_SAMPLE = 0x%x\n", prApeInfo->u2Bpersample);
    LOG(5, "[AUD_CODEC]W_APE_CHANNEL_NUM = 0x%x\n", prApeInfo->u2CHNum);
    LOG(5, "[AUD_CODEC]D_APE_INPUT_SAMPLING_RATE = 0x%x\n", prApeInfo->u4SamplingRate);
}
void _AUD_DspAPEMuteBankNum(UINT8 u1DecId,UINT16 u2MuteBankNum)
{
    if (u1DecId != AUD_DEC_MAIN)
    {
        LOG(0, "ape DecId invalid!!!\n");
        return; //currently we only support APE in dec1
    }

    vWriteShmUINT16(AUD_DSP0, W_APE_MUTE_BANK_NUMBERS,   u2MuteBankNum    );
    //vWriteShmUINT16(W_APE_INVALID_BYTES,       u2InvalidBytes   );
    vWriteShmUINT16(AUD_DSP0, W_APE_INVALID_BYTES, 0);

    LOG(5, "[AUD_CODEC]W_APE_MUTE_BANK_NUMBERS = 0x%x\n", u2MuteBankNum);
    LOG(5, "[AUD_CODEC]W_APE_INVALID_BYTES = 0x%x\n", 0);
}
#endif
void _AUD_DspWMAProHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo)
{
    if (prWmaInfo->u2HdrSrc == 0){
        vWriteShmUINT16(AUD_DSP0, W_WMAPRO_PACKET_NO + u1DecId*32, prWmaInfo->u2Pktno);
        vWriteShmUINT16(AUD_DSP0, W_WMAPRO_HEADER_SOURCE + u1DecId*32, 0);
    }else{
        vWriteShmUINT16(AUD_DSP0, W_WMAPRO_PACKET_NO + u1DecId*32, 0);
        vWriteShmUINT16(AUD_DSP0, W_WMAPRO_HEADER_SOURCE + u1DecId*32, 1);
        vWriteShmUINT16(AUD_DSP0, W_WMAPRO_NUM_CH + u1DecId*32, prWmaInfo->u2Numch);
        vWriteShmUINT32(AUD_DSP0, D_WMAPRO_SAMPLE_PER_SEC + u1DecId*32, prWmaInfo->u4Sampersec);
        vWriteShmUINT32(AUD_DSP0, D_WMAPRO_BYTE_PER_SEC + u1DecId*32, prWmaInfo->u4Bpersec);
        vWriteShmUINT32(AUD_DSP0, D_WMAPRO_BLOCK_SIZE + u1DecId*32, prWmaInfo->u4Blocksz);
        vWriteShmUINT16(AUD_DSP0, W_WMAPRO_ENCODER_OPTION + u1DecId*32, prWmaInfo->u2Encoderopt);

        //new for WMA Pro
        vWriteShmUINT32(AUD_DSP0, D_WMAPRO_CH_MASK + u1DecId*32, prWmaInfo->u4ChannelMask);
        vWriteShmUINT16(AUD_DSP0, W_WMAPRO_VALID_BITS + u1DecId*32, prWmaInfo->u2BitsPerSample);
        vWriteShmUINT16(AUD_DSP0, W_WMAPRO_ADV_OPTION + u1DecId*32, prWmaInfo->u2AdvEncoderopt);
        vWriteShmUINT32(AUD_DSP0, D_WMAPRO_ADV_OPTION2 + u1DecId*32, prWmaInfo->u4AdvEncoderopt2);

        LOG(5, "[WMA Pro Setting, DecId = %d]\n",u1DecId);
        LOG(5, "  u2NumCh: 0x%x\n", prWmaInfo->u2Numch);
        LOG(5, "  u4Sampersec: 0x%x\n", prWmaInfo->u4Sampersec);
        LOG(5, "  u4Bpersec: 0x%x\n",  prWmaInfo->u4Bpersec);
        LOG(5, "  u4Blocksz: 0x%x\n", prWmaInfo->u4Blocksz);
        LOG(5, "  u2Encoderopt: 0x%x\n", prWmaInfo->u2Encoderopt);
        LOG(5, "  u4ChannelMask: 0x%x\n", prWmaInfo->u4ChannelMask);
        LOG(5, "  u2BitsPerSample: 0x%x\n", prWmaInfo->u2BitsPerSample);
        LOG(5, "  u2AdvEncoderopt: 0x%x\n", prWmaInfo->u2AdvEncoderopt);
        LOG(5, "  u4AdvEncoderopt2: 0x%x\n", prWmaInfo->u4AdvEncoderopt2);
        
        
    if(1) // for advanced_encoder_option
    {
        UINT16  u2PlusEncOptions = prWmaInfo->u2AdvEncoderopt;
        UINT32  u4PlusVer = (u2PlusEncOptions & 0xE000) >> 13;

        //u4PlusVer=0;
        if (u4PlusVer == 0)
        { // base
            u2PlusEncOptions |= 0x1000;
        }
        else if ((u4PlusVer == 4)||(u4PlusVer == 6))
        { // plus v1
            u2PlusEncOptions |= 0x2000;
            u2PlusEncOptions &= 0x2FFF;
        }
        else if (u4PlusVer == 1)
        { // plus v2
            u2PlusEncOptions |= 0x4000;
            u2PlusEncOptions &= 0x4FFF;
        }
        else if (u4PlusVer == 2)
        { // plus v3
            u2PlusEncOptions |= 0xC000;
            u2PlusEncOptions &= 0xCFFF;
        }
        else if (u4PlusVer == 7)
        { // plus v3
            LOG(0, "[AUD][WMAPro] Failed: incorrect advance encode option.\n");
        }
        else
        { // base
            u2PlusEncOptions |= 0x1000;
            u2PlusEncOptions &= 0x1FFF;
        }

        LOG(5, "[DecInfo] advance_enc_options   = %d.\n", u2PlusEncOptions);
        vWriteShmUINT16 (AUD_DSP0, W_WMAPRO_ADV_OPTION + u1DecId*32, u2PlusEncOptions);
    }
    }
}

void AUD_SetLpcmTable(UINT8 u1DecId, AUD_LPCM_SETTING_T rLpcmSetting)
{
    UNUSED(u1DecId) ;

    //decoder 2, decoder 3 ... todo...
    if (u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT8(AUD_DSP0, B_LPCM_CH_ASSIGNMENT, rLpcmSetting.u1LpcmChAssignment);
        vWriteShmUINT16(AUD_DSP0, W_LPCM_FREQUENCY, (((UINT16)rLpcmSetting.u1LpcmFreqGrp1) << 8) + rLpcmSetting.u1LpcmFreqGrp2);
        vWriteShmUINT8(AUD_DSP0, B_LPCM_BIT_SHIFT, rLpcmSetting.u1LpcmBitShift);
        vWriteShmUINT8(AUD_DSP0, B_LPCM_DRC_VALUE, rLpcmSetting.u1LpcmDrcValue);
        vWriteShmUINT16(AUD_DSP0, W_LPCM_Q_VALUE, (rLpcmSetting.u1LpcmBitResGrp1 << 12)+rLpcmSetting.u1LpcmBitResGrp2);
        vWriteShmUINT8(AUD_DSP0, B_LPCM_DRC_FLAG, rLpcmSetting.u1LpcmDrcFlag);
        vWriteShmUINT16(AUD_DSP0, W_LPCM_DECODING_TYPE, rLpcmSetting.u2LpcmDecodingType);

        // ADPCM
        if (rLpcmSetting.u2ADPCMDecodingType!=0)
        {
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_CHANNEL_NUM, rLpcmSetting.u1LpcmChAssignment+1);
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_BIT_PERSAMPLE, rLpcmSetting.u1LpcmBitResGrp1);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_DECODING_TYPE, rLpcmSetting.u2ADPCMDecodingType);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_BLOCK_ALIGN, rLpcmSetting.u2ADPCMBlockAlign);
        }
        else
        {
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_CHANNEL_NUM, 0);
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_BIT_PERSAMPLE, 0);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_DECODING_TYPE, 0);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_BLOCK_ALIGN, 0);
        }
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        vWriteShmUINT8(AUD_DSP0, B_LPCM_CH_ASSIGNMENT_DEC2, rLpcmSetting.u1LpcmChAssignment);
        vWriteShmUINT16(AUD_DSP0, W_LPCM_FREQUENCY_DEC2, (rLpcmSetting.u1LpcmFreqGrp1 << 8) + rLpcmSetting.u1LpcmFreqGrp2);
        vWriteShmUINT8(AUD_DSP0, B_LPCM_BIT_SHIFT_DEC2, rLpcmSetting.u1LpcmBitShift);
        vWriteShmUINT8(AUD_DSP0, B_LPCM_DRC_VALUE_DEC2, rLpcmSetting.u1LpcmDrcValue);
        vWriteShmUINT16(AUD_DSP0, W_LPCM_Q_VALUE_DEC2, (rLpcmSetting.u1LpcmBitResGrp1 << 12)+rLpcmSetting.u1LpcmBitResGrp2);
        vWriteShmUINT8(AUD_DSP0, B_LPCM_DRC_FLAG_DEC2, rLpcmSetting.u1LpcmDrcFlag);
        vWriteShmUINT16(AUD_DSP0, W_LPCM_DECODING_TYPE_DEC2, rLpcmSetting.u2LpcmDecodingType);

        // ADPCM
        if (rLpcmSetting.u2ADPCMDecodingType!=0)
        {
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_CHANNEL_NUM_DEC2, rLpcmSetting.u1LpcmChAssignment+1);
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_BIT_PERSAMPLE_DEC2, rLpcmSetting.u1LpcmBitResGrp1);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_DECODING_TYPE_DEC2, rLpcmSetting.u2ADPCMDecodingType);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_BLOCK_ALIGN_DEC2, rLpcmSetting.u2ADPCMBlockAlign);
        }
        else
        {
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_CHANNEL_NUM_DEC2, 0);
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_BIT_PERSAMPLE_DEC2, 0);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_DECODING_TYPE_DEC2, 0);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_BLOCK_ALIGN_DEC2, 0);
        }
    }
    else if (u1DecId == AUD_DEC_THIRD) //parson MM3
    {
        vWriteShmUINT8(AUD_DSP0, B_LPCM_CH_ASSIGNMENT_DEC3, rLpcmSetting.u1LpcmChAssignment);
        vWriteShmUINT16(AUD_DSP0, W_LPCM_FREQUENCY_DEC3, (rLpcmSetting.u1LpcmFreqGrp1 << 8) + rLpcmSetting.u1LpcmFreqGrp2);
        vWriteShmUINT8(AUD_DSP0, B_LPCM_BIT_SHIFT_DEC3, rLpcmSetting.u1LpcmBitShift);
        vWriteShmUINT8(AUD_DSP0, B_LPCM_DRC_VALUE_DEC3, rLpcmSetting.u1LpcmDrcValue);
        vWriteShmUINT16(AUD_DSP0, W_LPCM_Q_VALUE_DEC3, (rLpcmSetting.u1LpcmBitResGrp1 << 12)+rLpcmSetting.u1LpcmBitResGrp2);
        vWriteShmUINT8(AUD_DSP0, B_LPCM_DRC_FLAG_DEC3, rLpcmSetting.u1LpcmDrcFlag);
        vWriteShmUINT16(AUD_DSP0, W_LPCM_DECODING_TYPE_DEC3, rLpcmSetting.u2LpcmDecodingType);

        // ADPCM
        if (rLpcmSetting.u2ADPCMDecodingType!=0)
        {
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_CHANNEL_NUM_DEC3, rLpcmSetting.u1LpcmChAssignment+1);
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_BIT_PERSAMPLE_DEC3, rLpcmSetting.u1LpcmBitResGrp1);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_DECODING_TYPE_DEC3, rLpcmSetting.u2ADPCMDecodingType);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_BLOCK_ALIGN_DEC3, rLpcmSetting.u2ADPCMBlockAlign);
        }
        else
        {
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_CHANNEL_NUM_DEC3, 0);
            vWriteShmUINT8(AUD_DSP0, B_ADPCM_BIT_PERSAMPLE_DEC3, 0);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_DECODING_TYPE_DEC3, 0);
            vWriteShmUINT16(AUD_DSP0, W_ADPCM_BLOCK_ALIGN_DEC3, 0);
        }
    }
}

/*
0x101 -ALAW
0x301 -ULAW
*/
UINT32 AUD_GetLPCMType(void)
{
    UINT32 u4Type;
    u4Type = u2ReadShmUINT16(AUD_DSP0, W_LPCM_DECODING_TYPE);
    return u4Type;
}

#if 1   // Unused
//-----------------------------------------------------------------------------
/** _AUD_DspSetDualDecMode
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDualDecMode(BOOL fgEnable)
{
    DSP_SetDualDecMode(fgEnable);
}
#endif

#ifdef CC_AUD_LINEIN_USE_DEC3
void _AUD_DspSetTriOnlyDecMode(BOOL fgEnable)
{
    DSP_SetTriDecOnlyMode(fgEnable);
}
#endif
BOOL _AUD_DspIsTriOnlyDecMode(void)
{
    return(_IsTriOnlyDecMode());
}


void _AUD_DspMixSndDec3(BOOL fgEnable)
{
    DSP_MixSndDec3Mode(fgEnable);
}


#ifdef CC_AUD_HPF_SUPPORT
//-----------------------------------------------------------------------------
/** _AUD_DspSetHPFEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetHPFEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_HPF_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_HPF_ENABLE;

    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetHPFFc
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetHPFFc(UINT16 u2Fc)
{
    UINT16  u2ShmIndex = W_HPF_FC;
    UINT16  u2UopIndex = UOP_DSP_HPF_FC;

    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Fc);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}
void _AUD_DspSetHPF2Fc(UINT16 u2Fc)
{
    UINT16  u2ShmIndex = W_HPF2_FC;
    UINT16  u2UopIndex = UOP_DSP_HPF_FC;

    vWriteShmUINT16(AUD_DSP0, u2ShmIndex, u2Fc);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}

#endif

#ifdef DSP_SUPPORT_TCL_VBASS
//-----------------------------------------------------------------------------
/** _AUD_DspSetTVBEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetTVBEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_TCLBASS_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_TCL_VBASS;

    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
}


#endif


#ifdef CC_AUD_CLIPPER_SUPPORT
//-----------------------------------------------------------------------------
/** _AUD_DspSetClipperEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetClipperEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_CLIPPER_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_CLIPPER_ENABLE;

    vWriteShmUINT8(u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetClipperValue
 *
 *  @param  value
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetClipperValue(AUD_CH_T eChannel, UINT32 u4Value)
{
    UINT16  u2ShmIndex;
    UINT16 u2UopIndex = UOP_DSP_CLIPPER_VAL;

    switch(eChannel)
    {
    case AUD_CH_FRONT_LEFT:
        u2ShmIndex = D_CLIPPER_L_VALUE;
        break;
    case AUD_CH_FRONT_RIGHT:
        u2ShmIndex = D_CLIPPER_R_VALUE;
        break;
    case AUD_CH_REAR_LEFT:
        u2ShmIndex = D_CLIPPER_LS_VALUE;
        break;
    case AUD_CH_REAR_RIGHT:
        u2ShmIndex = D_CLIPPER_RS_VALUE;
        break;
    case AUD_CH_CENTER:
        u2ShmIndex = D_CLIPPER_C_VALUE;
        break;
    case AUD_CH_SUB_WOOFER:
        u2ShmIndex = D_CLIPPER_SW_VALUE;
        break;
    case AUD_CH_BYPASS_LEFT:
        u2ShmIndex = D_CLIPPER_CH7_VALUE;
        break;
    case AUD_CH_BYPASS_RIGHT:
        u2ShmIndex = D_CLIPPER_CH8_VALUE;
        break;
    case AUD_CH_DMX_LEFT:
        u2ShmIndex = D_CLIPPER_CH9_VALUE;
        break;
    case AUD_CH_DMX_RIGHT:
        u2ShmIndex = D_CLIPPER_CH10_VALUE;
        break;
    default:
        u2ShmIndex = D_CLIPPER_L_VALUE;
    }

    vWriteShmUINT32(u2ShmIndex, u4Value);
    DSP_SendDspTaskCmd(u2UopIndex);
}
#endif

#if (defined(CC_AUD_SPEAKER_HEIGHT) || defined(CC_AUD_SPEAKER_HEIGHT_V2))
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
//-----------------------------------------------------------------------------
/** _AUD_DspSetSPHEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSPHEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_SPH_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_SPH_ENABLE;
    LOG(5,"CMD: set SpkHeightEnable(%d)\n", (UINT8)fgEnable);

   vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}
#else
//-----------------------------------------------------------------------------
/** _AUD_DspSetSPHMode
 *
 *  @param  0:off 1:2DB 2:4DB 3:6DB
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetSPHMode(UINT8 u1Mode)
{
    UINT16  u2ShmIndex = B_SPH_MODE;
    UINT16 u2UopIndex = UOP_DSP_SPH_ENABLE;
    UINT8 u1SphCurMode;

    LOG(5,"CMD: set SpkHeightMode: Mode(%d)\n", u1Mode);
    u1SphCurMode = uReadShmUINT8(AUD_DSP0, B_SPH_MODE);
    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)u1Mode);

    if ((u1Mode > 0) && (u1SphCurMode > 0))
    {
        u2UopIndex = UOP_DSP_SPH_CHANGE_MODE;
    }
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}
#endif
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetPEQEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetPEQEnable(BOOL fgEnable)
{
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_21, APROC_REG_PEQ_SP_FLAG), fgEnable);

#else
    UINT16  u2ShmIndex = B_PEQ_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_PEQ_ENABLE;

    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
#endif  
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetPEQEnable
 *
 *  @param  void
 *  @retval enable/disable
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspGetPEQEnable(void)
{
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
    UINT8   fgEnable;

    fgEnable = u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_21, APROC_REG_PEQ_SP_FLAG));
    return (BOOL)fgEnable;

#else
    UINT16  u2ShmIndex = B_PEQ_ENABLE;
    UINT8   fgEnable;

    fgEnable = uReadShmUINT8(AUD_DSP0, u2ShmIndex);
    return (BOOL)fgEnable;
#endif  
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetPEQCfg2
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u1Set                      PEQ set (0 ~ 7)
 *  @param u4Frequency                Frequency (20 ~ 20K)
 *  @param eQValue                    Q
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
#if 0
void _AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            AUD_PEQ_Q_TYPE_T eQValue, INT16 i2Gain)
{
    UINT16  u2ShmAddrPEQFc;
    UINT16  u2ShmAddrPEQGain;
    UINT16  u2ShmAddrPEQBW;
    UINT16  u2UopIndex = UOP_DSP_PEQ_CONFIG;

    UINT16  u2PEQFc;
    UINT32  u4PEQGain;
    UINT16  u2PEQBW;

    UNUSED(u1DecId);

    // Check u1PEQSet range
    if (u1PEQSet > 8)//(u1PEQSet > 7)
    {
        u1PEQSet = 8;//u1PEQSet = 7;
    }

    // Share memory address
    u2ShmAddrPEQFc   = W_PEQ_SET0_FC   + 8*u1PEQSet;
    u2ShmAddrPEQGain = D_PEQ_SET0_GAIN + 8*u1PEQSet;
    u2ShmAddrPEQBW   = W_PEQ_SET0_BW   + 8*u1PEQSet;

    switch(eQValue)
    {
    case AUD_PEQ_Q_0_33:
        u2PEQBW = (30303*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_0_43:
        u2PEQBW = (23255*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_0_56:
        u2PEQBW = (17857*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_0_75:
        u2PEQBW = (13333*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_1_00:
        u2PEQBW = (10000*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_1_20:
        u2PEQBW = (8333*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_1_50:
        u2PEQBW = (6666*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_1_80:
        u2PEQBW = (5555*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_2_20:
        u2PEQBW = (4545*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_2_27:
        u2PEQBW = (3703*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_3_30:
        u2PEQBW = (3030*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_3_39:
        u2PEQBW = (2564*u4Frequency / 10000);//u2PEQBW = (2949*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_4_70:
        u2PEQBW = (2127*u4Frequency / 10000);//u2PEQBW = (2727*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_5_60:
        u2PEQBW = (1785*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_6_80:
        u2PEQBW = (1470*u4Frequency / 10000);
        break;
    case AUD_PEQ_Q_8_20:
        u2PEQBW = (1219*u4Frequency / 10000);
        break;
    default:
        u2PEQBW = (10000*u4Frequency / 10000);
        break;
    }

    u2PEQFc = (UINT16)u4Frequency;
    u4PEQGain = _DbToShm((INT32)i2Gain) << 4;

    vWriteShmUINT16(u2ShmAddrPEQFc,   u2PEQFc);
    vWriteShmUINT32(u2ShmAddrPEQGain, u4PEQGain);
    vWriteShmUINT16(u2ShmAddrPEQBW,   u2PEQBW);

    DSP_SendDspTaskCmd(u2UopIndex);

}
#else
void _AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT32 eQValue, INT16 i2Gain)
{
    UINT16  u2ShmAddrPEQFc;
    UINT16  u2ShmAddrPEQGain;
    UINT16  u2ShmAddrPEQBW;
    UINT16  u2UopIndex = UOP_DSP_PEQ_CONFIG;

    UINT16  u2PEQFc;
    UINT32  u4PEQGain;
    UINT16  u2PEQBW;

    UNUSED(u1DecId);

    // Check u1PEQSet range
    if (u1PEQSet > 8)//(u1PEQSet > 7)
    {
        u1PEQSet = 8;//u1PEQSet = 7;
    }

    // Share memory address
    u2ShmAddrPEQFc   = W_PEQ_SET0_FC   + 8*u1PEQSet;
    u2ShmAddrPEQGain = D_PEQ_SET0_GAIN + 8*u1PEQSet;
    u2ShmAddrPEQBW   = W_PEQ_SET0_BW   + 8*u1PEQSet;

    // Check eQValue range
    if (eQValue == 0)
    {
        eQValue = 1;
    }
    else if (eQValue > 100)
    {
        eQValue = 100;
    }

    // Check FC range
    if (u4Frequency > 24000)
    {
        u4Frequency = 24000;
    }
    else if (u4Frequency < 20)
    {
        u4Frequency = 20;
    }

    u2PEQBW = (UINT16)(u4Frequency*10/eQValue);
    // Check BW range
    if ((u4Frequency*10/eQValue) > 23000)
    {
        u2PEQBW = 23000;
    }
    else if (u2PEQBW < 20)
    {
        u2PEQBW = 20;
    }
    u2PEQFc = (UINT16)u4Frequency;
#ifdef CC_AUD_SX1_FEATURE
    u4PEQGain = _DbToShmPEQ((INT32)i2Gain) << 4;
#else
    u4PEQGain = _DbToShm((INT32)i2Gain) << 4;
#endif

    vWriteShmUINT16(AUD_DSP0, u2ShmAddrPEQFc,   u2PEQFc);
    vWriteShmUINT32(AUD_DSP0, u2ShmAddrPEQGain, u4PEQGain);
    vWriteShmUINT16(AUD_DSP0, u2ShmAddrPEQBW,   u2PEQBW);

    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);

}

void _AUD_DspGetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 *u4Frequency,
                            UINT32 *eQValue, UINT32 *u4Gain)
{
    UINT16  u2ShmAddrPEQFc;
    UINT16  u2ShmAddrPEQGain;
    UINT16  u2ShmAddrPEQBW;

    UINT16  u2PEQFc;
    UINT32  u4PEQGain;
    UINT16  u2PEQBW;
    UINT32  u4PEQQValue;

    UNUSED(u1DecId);

    // Check u1PEQSet range
    if (u1PEQSet > 8)//(u1PEQSet > 7)
    {
        u1PEQSet = 8;//u1PEQSet = 7;
    }

    // Share memory address
    u2ShmAddrPEQFc   = W_PEQ_SET0_FC   + 8*u1PEQSet;
    u2ShmAddrPEQGain = D_PEQ_SET0_GAIN + 8*u1PEQSet;
    u2ShmAddrPEQBW   = W_PEQ_SET0_BW   + 8*u1PEQSet;

    u2PEQFc = u2ReadShmUINT16(AUD_DSP0, u2ShmAddrPEQFc);
    u4PEQGain = u4ReadShmUINT32(AUD_DSP0, u2ShmAddrPEQGain);
    u2PEQBW = u2ReadShmUINT16(AUD_DSP0, u2ShmAddrPEQBW);

    u4PEQQValue = (UINT32)(u2PEQFc*10);
    u4PEQQValue = u4PEQQValue/((UINT32)u2PEQBW);

    *u4Frequency = (UINT32)u2PEQFc;
    *eQValue = u4PEQQValue;
    *u4Gain = u4PEQGain;

}

#ifdef CC_AUD_PEQ_LR
void _AUD_DspSetPEQCfg2_R(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT32 eQValue, INT16 i2Gain)
{
    UINT16  u2ShmAddrPEQFc;
    UINT16  u2ShmAddrPEQGain;
    UINT16  u2ShmAddrPEQBW;
    UINT16  u2UopIndex = UOP_DSP_PEQ_CONFIG;

    UINT16  u2PEQFc;
    UINT32  u4PEQGain;
    UINT16  u2PEQBW;

    UNUSED(u1DecId);

    // Check u1PEQSet range
    if (u1PEQSet > 8)//(u1PEQSet > 7)
    {
        u1PEQSet = 8;//u1PEQSet = 7;
    }

    // Share memory address
    u2ShmAddrPEQFc   = W_PEQ_SET0_FC_R   + 8*u1PEQSet;
    u2ShmAddrPEQGain = D_PEQ_SET0_GAIN_R + 8*u1PEQSet;
    u2ShmAddrPEQBW   = W_PEQ_SET0_BW_R   + 8*u1PEQSet;

    // Check eQValue range
    if (eQValue == 0)
    {
        eQValue = 1;
    }
    if (eQValue > 100)
    {
        eQValue = 100;
    }

    // Check FC range
    if (u4Frequency > 24000)
    {
        u4Frequency = 24000;
    }

    u2PEQBW = (UINT16)(u4Frequency*10/eQValue);
    // Check BW range
    if ((u4Frequency*10/eQValue) > 23000)
    {
        u2PEQBW = 23000;
    }

    u2PEQFc = (UINT16)u4Frequency;
#if 1
    u4PEQGain = _DbToShmPEQ((INT32)i2Gain) << 4;
#else
    u4PEQGain = _DbToShm((INT32)i2Gain) << 4;
#endif

    vWriteShmUINT16(u2ShmAddrPEQFc,   u2PEQFc);
    vWriteShmUINT32(u2ShmAddrPEQGain, u4PEQGain);
    vWriteShmUINT16(u2ShmAddrPEQBW,   u2PEQBW);

    DSP_SendDspTaskCmd(u2UopIndex);

}

void _AUD_DspGetPEQCfg2_R(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 *u4Frequency,
                            UINT32 *eQValue, UINT32 *u4Gain)
{
    UINT16  u2ShmAddrPEQFc;
    UINT16  u2ShmAddrPEQGain;
    UINT16  u2ShmAddrPEQBW;

    UINT16  u2PEQFc;
    UINT32  u4PEQGain;
    UINT16  u2PEQBW;
    UINT32  u4PEQQValue;

    UNUSED(u1DecId);

    // Check u1PEQSet range
    if (u1PEQSet > 8)//(u1PEQSet > 7)
    {
        u1PEQSet = 8;//u1PEQSet = 7;
    }

    // Share memory address
    u2ShmAddrPEQFc   = W_PEQ_SET0_FC_R   + 8*u1PEQSet;
    u2ShmAddrPEQGain = D_PEQ_SET0_GAIN_R + 8*u1PEQSet;
    u2ShmAddrPEQBW   = W_PEQ_SET0_BW_R   + 8*u1PEQSet;

    u2PEQFc = u2ReadShmUINT16(u2ShmAddrPEQFc);
    u4PEQGain = u4ReadShmUINT32(u2ShmAddrPEQGain);
    u2PEQBW = u2ReadShmUINT16(u2ShmAddrPEQBW);

    u4PEQQValue = (UINT32)(u2PEQFc*10);
    u4PEQQValue = u4PEQQValue/((UINT32)u2PEQBW);

    *u4Frequency = (UINT32)u2PEQFc;
    *eQValue = u4PEQQValue;
    *u4Gain = u4PEQGain;

}
#endif
#endif
//-----------------------------------------------------------------------------
/** _AUD_DspSetPEQCfg
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u1Set                      PEQ set (0 ~ 7)
 *  @param u4Frequency                Frequency (20 ~ 20K)
 *  @param u2BW                       Bandwidth (xx Hz)
 *  @param i2Gain                     Gain (+-12dB) with 0.5dB/step
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetPEQCfg(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT16 u2BW, INT16 i2Gain)
{
    UINT16  u2ShmAddrPEQFc;
    UINT16  u2ShmAddrPEQGain;
    UINT16  u2ShmAddrPEQBW;
    UINT16  u2UopIndex = UOP_DSP_PEQ_CONFIG;

    UINT16  u2PEQFc;
    UINT32  u4PEQGain;
    UINT16  u2PEQBW;

    UNUSED(u1DecId);

    // Check u1PEQSet range
    if (u1PEQSet > 8)//(u1PEQSet > 7)
    {
        u1PEQSet = 8;//u1PEQSet = 7;
    }

    // Check FC range
    if (u4Frequency > 24000)
    {
        u4Frequency = 24000;
    }
    else if (u4Frequency < 20)
    {
        u4Frequency = 20;
    }

    // Check BW range
    if (u2BW > 23000)
    {
        u2BW = 23000;
    }
    else if (u2BW < 20)
    {
        u2BW = 20;
    }
    // Share memory address
    u2ShmAddrPEQFc   = W_PEQ_SET0_FC   + 8*u1PEQSet;
    u2ShmAddrPEQGain = D_PEQ_SET0_GAIN + 8*u1PEQSet;
    u2ShmAddrPEQBW   = W_PEQ_SET0_BW   + 8*u1PEQSet;

    u2PEQFc   = (UINT16)u4Frequency;
#ifdef CC_AUD_SX1_FEATURE
    u4PEQGain = _DbToShmPEQ((INT32)i2Gain) << 4;
#else
    u4PEQGain = _DbToShm((INT32)i2Gain) << 4;
#endif
    u2PEQBW   = (UINT16)u2BW;

    vWriteShmUINT16(AUD_DSP0, u2ShmAddrPEQFc,   u2PEQFc);
    vWriteShmUINT32(AUD_DSP0, u2ShmAddrPEQGain, u4PEQGain);
    vWriteShmUINT16(AUD_DSP0, u2ShmAddrPEQBW,   u2PEQBW);

    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);

}
#ifdef CC_AUD_PEQ_LR
void _AUD_DspSetPEQCfg_R(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT16 u2BW, INT16 i2Gain)
{
    UINT16  u2ShmAddrPEQFc;
    UINT16  u2ShmAddrPEQGain;
    UINT16  u2ShmAddrPEQBW;
    UINT16  u2UopIndex = UOP_DSP_PEQ_CONFIG;

    UINT16  u2PEQFc;
    UINT32  u4PEQGain;
    UINT16  u2PEQBW;

    UNUSED(u1DecId);

    // Check u1PEQSet range
    if (u1PEQSet > 8)//(u1PEQSet > 7)
    {
        u1PEQSet = 8;//u1PEQSet = 7;
    }

    // Share memory address
    u2ShmAddrPEQFc   = W_PEQ_SET0_FC_R   + 8*u1PEQSet;
    u2ShmAddrPEQGain = D_PEQ_SET0_GAIN_R + 8*u1PEQSet;
    u2ShmAddrPEQBW   = W_PEQ_SET0_BW_R   + 8*u1PEQSet;

    u2PEQFc   = (UINT16)u4Frequency;
#if 1//
    u4PEQGain = _DbToShmPEQ((INT32)i2Gain) << 4;
#else
    u4PEQGain = _DbToShm((INT32)i2Gain) << 4;
#endif
    u2PEQBW   = (UINT16)u2BW;

    vWriteShmUINT16(u2ShmAddrPEQFc,   u2PEQFc);
    vWriteShmUINT32(u2ShmAddrPEQGain, u4PEQGain);
    vWriteShmUINT16(u2ShmAddrPEQBW,   u2PEQBW);

    DSP_SendDspTaskCmd(u2UopIndex);

}
#endif

#ifdef CC_AUD_VBASS_SUPPORT
//-----------------------------------------------------------------------------
/** _AUD_DspSetVirBassEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetVirBassEnable(BOOL fgEnable)
{
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
    APROC_DSP_MVB_SET_T  aMVBSet; 

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_MVB, 0, (UINT32 *) &aMVBSet, 1);

    aMVBSet.fgMVBFlag = fgEnable;

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_MVB, 0, (UINT32 *) &aMVBSet, 1);
#else

    UINT16  u2ShmIndex = B_VIR_BASS_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_VIR_BASS_ENABLE;
    UINT8 u1CurEnable;

    u1CurEnable = uReadShmUINT8(AUD_DSP0, u2ShmIndex);
    if (u1CurEnable == (UINT8)fgEnable)
    {
        return;
    }
#if 0//def CC_AUD_EFFECT_MUTE_PROTECT
    _AUD_DspVBassMute(AUD_CH_FRONT_LEFT, TRUE);
    _AUD_DspVBassMute(AUD_CH_FRONT_RIGHT, TRUE);
#endif
    LOG(5,"CMD: set VBassEnable: Enable(%d)\n", (UINT8)fgEnable);

  vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
#endif
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetVirBassCfg
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param u4Frequency                Frequency (20 ~ xx)
 *  @param u4Gain
 *  @param u4Gain2
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetVirBassCfg(UINT8 u1DecId, UINT32 u4Frequency, UINT32 u4Gain,
                        UINT32 u4Gain2)
{
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
    APROC_DSP_MVB_SET_T  aMVBSet; 

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_MVB, 0, (UINT32 *) &aMVBSet, 1);

    aMVBSet.u4Fc = u4Frequency;
    aMVBSet.i4BassGain = u4Gain;
    aMVBSet.i4OutputGain = u4Gain2;
    
    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_MVB, 0, (UINT32 *) &aMVBSet, 1);
#else
    UINT16 u2ShmIndex = D_VIR_BASS_GAIN;
    UINT16 u2UopIndex = UOP_DSP_VIR_BASS_CONFIG;
    UINT32 u4CurGain1, u4CurGain2;
    UNUSED(u1DecId);
    UNUSED(u4Frequency);
    UNUSED(u2ShmIndex);


    LOG(5,"CMD: set VBassCfg: Dec(%d) Freq(%d) Gain(%d) Gain2(%d)\n",
        u1DecId, u4Frequency, u4Gain, u4Gain2);

    u4CurGain1 = u4ReadShmUINT32(AUD_DSP0, D_VIR_BASS_GAIN);
    u4CurGain2 = u4ReadShmUINT32(AUD_DSP0, D_VIR_BASS_GAIN_2);

    if ((u4CurGain1 == u4Gain) && (u4CurGain2==u4Gain2))
    {
        return;
    }
    vWriteShmUINT32(AUD_DSP0, D_VIR_BASS_GAIN, u4Gain);
    vWriteShmUINT32(AUD_DSP0, D_VIR_BASS_GAIN_2, u4Gain2);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);

#endif
}

void _AUD_DspGetVirBassCfg(UINT8 u1DecId, BOOL *fgEnable, UINT32 *u4Gain, UINT32 *u4Gain2)
{
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
    APROC_DSP_MVB_SET_T  aMVBSet; 

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_MVB, 0, (UINT32 *) &aMVBSet, 1);

    *fgEnable = aMVBSet.fgMVBFlag;
    *u4Gain = aMVBSet.i4BassGain;
    *u4Gain2 = aMVBSet.i4OutputGain;
    
#else
    UNUSED(u1DecId);
    UNUSED(_AUD_DspGetVirBassCfg);

    if (uReadShmUINT8(AUD_DSP0, B_VIR_BASS_ENABLE) == 0)
    {
        *fgEnable = FALSE;
    }
    else
    {
        *fgEnable = TRUE;
    }
    *u4Gain = u4ReadShmUINT32(AUD_DSP0, D_VIR_BASS_GAIN);
    *u4Gain2 = u4ReadShmUINT32(AUD_DSP0, D_VIR_BASS_GAIN_2);

#endif
}
#endif

#ifdef CC_AUD_FIR_SUPPORT
//-----------------------------------------------------------------------------
/** _AUD_DspSetFIREnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetFIREnable(UINT8 u1ConeqMode)
{
    UINT16  u2ShmIndex = B_FIR_ENABLE;
    UINT16 u2UopIndex = UOP_DSP_FIR_ENABLE;
    UINT8 u1CurEnable;

    u1CurEnable = uReadShmUINT8(u2ShmIndex);
    if (u1CurEnable == u1ConeqMode)
    {
        return;
    }

    vWriteShmUINT8(u2ShmIndex, u1ConeqMode);
    DSP_SendDspTaskCmd(u2UopIndex);
}
#endif  //FIR


#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
//-----------------------------------------------------------------------------
/** _AUD_DspSetADVEnable
 *
 *  @param  disable/light/medium/heavy compression
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetADVEnable(UINT8 fgEnable)
{
    UINT16  u2ShmIndex = B_ADV_FLAG;
    UINT16 u2UopIndex = UOP_DSP_ADV_ENABLE;
    UINT8  adv_mode=0;

#if 0
    if (fgEnable == 0x1 |fgEnable == 0x2 |fgEnable == 0x3 )
    {
       //Daniel, 2009/12/7, ADV code will be automatically loaded with AUD_POSTRAM.
       DSP_LoadRamCode(AUD_ADV);
    }
#endif
    if (fgEnable == 0x0)//OFF
    {
        adv_mode=0;
    }
    else if(fgEnable == 0x1)//light compressioj
    {
        adv_mode=3;
    }
    else if(fgEnable == 0x2)//medium compressioj
    {
        adv_mode=5;
    }
   else if(fgEnable == 0x3)//heavy compressioj
    {
        adv_mode=9;
    }

    vWriteShmUINT8(u2ShmIndex, (UINT8)adv_mode);
    DSP_SendDspTaskCmd(u2UopIndex);
}


void _AudShowADVStatus(void)
{
    UINT32 chCalbGain_L,chCalbGain_R;

    chCalbGain_L=u4ReadShmUINT32(D_ADV_chCalbGain_L)    ;
    chCalbGain_R=u4ReadShmUINT32(D_ADV_chCalbGain_R);
    LOG(1 ,"chCalbGain_L = 0x%08x\n", chCalbGain_L);
    LOG(1 ,"chCalbGain_R = 0x%08x\n", chCalbGain_R);

    // Fix -O2 warning message
    UNUSED(chCalbGain_L);
    UNUSED(chCalbGain_R);
}

//-----------------------------------------------------------------------------
/** _AUD_DspADVSetChGain
 *
 *  @param  ch, value
 *   ch:        0:L, 1:R
 *   value: dB (10.14), range: 0x0 ~ 0xffffff
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspADVSetChGain(UINT8 ch, UINT32 value)
{
    UINT32 u4ShmIndex = D_ADV_chCalbGain_L;
    UINT32 u2UopIndex = UOP_DSP_ADV_SET_chCalbGain;
    if (ch==0) //L
    {
       u4ShmIndex=D_ADV_chCalbGain_L;
    }
    else//R
    {
       u4ShmIndex=D_ADV_chCalbGain_R;
    }
     vWriteShmUINT32 (u4ShmIndex,value);//dB,(10.14)
    DSP_SendDspTaskCmd(u2UopIndex);
}

void _AUD_DspABXQuery(void)
{
    Printf("  ----------\n");
    Printf("  item 0: [DEC1] ABX Enable: %d (1-Enable,0-Disable)\n", uReadShmUINT8(B_ABX_ENABLE));
    Printf("  item 1: [DEC1] filter set: %d (0-Set 1,1-Set 2)\n", uReadShmUINT8(B_ABX_FILTER_SET));
    Printf("  item 2: [DEC1] Dry Gain Indx: %d (0~24, 0.5db/step)\n", uReadShmUINT8(B_ABX_DRY_GAIN_INDX));
    Printf("  item 3: [DEC1] Wet Gain Indx: %d (0~24, 0.5db/step)\n", uReadShmUINT8(B_ABX_WET_GAIN_INDX));
}

void _AUD_DspABXConfig(UINT8 item, UINT8 val)
{
    switch (item)
    {
        case 0:
            vWriteShmUINT8(B_ABX_ENABLE, val);
            DSP_SendDspTaskCmd(UOP_DSP_ABX_FLAG);
            break;

        case 1:
            vWriteShmUINT8(B_ABX_FILTER_SET, val);
            if (val == 0)
            {
                vWriteShmUINT8(B_ABX_DRY_GAIN_INDX,0);
                vWriteShmUINT8(B_ABX_WET_GAIN_INDX,6);
            }
            else
            {
                vWriteShmUINT8(B_ABX_DRY_GAIN_INDX,0);
                vWriteShmUINT8(B_ABX_WET_GAIN_INDX,12);
            }
            DSP_SendDspTaskCmd(UOP_DSP_ABX_FLAG);
            break;

        case 2:
            vWriteShmUINT8(B_ABX_DRY_GAIN_INDX, val);
            DSP_SendDspTaskCmd(UOP_DSP_ABX_FLAG);
            break;

        case 3:
            vWriteShmUINT8(B_ABX_WET_GAIN_INDX, val);
            DSP_SendDspTaskCmd(UOP_DSP_ABX_FLAG);
            break;
    }
}

void _AUD_DspAEQEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UINT32 u4UopCmd;
    UINT8  u1ShmFlag;

    UNUSED(u1DecId);

    u4UopCmd = UOP_DSP_AEQ_FLAG;
    u1ShmFlag = uReadShmUINT8(B_AEQ_FLAG);

    if ((fgEnable && !u1ShmFlag) || (!fgEnable && u1ShmFlag))
    {
        u1ShmFlag = fgEnable;
        vWriteShmUINT8(B_AEQ_FLAG, u1ShmFlag);
        DSP_SendDspTaskCmd(u4UopCmd);
    }
}

void _AUD_DspAEQQuery(void)
{
    UINT8   u1ShmFlag;
    BOOL    fgStatus;

    u1ShmFlag = uReadShmUINT8(B_EQFLAG);
    fgStatus = (BOOL)(u1ShmFlag);

    if (fgStatus)
    {
        Printf("\nAudyssey EQ Status: Enable");
    }
    else
    {
        Printf("\nAudyssey EQ Status: Disable");
    }

}
#endif
//zy_dts
void  _AUD_DspSetDTSENCEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_DTS_ENCODER_FLAG;
    UINT16  u2UopIndex = UOP_DSP_DTS_ENCODER_ENABLE;

    /*

    if ((fgEnable == TRUE) && (uReadShmUINT8(B_DTS_ENCODER_FLAG)==0x1))
    {
        return;
    }

    if ((fgEnable == FALSE) && (uReadShmUINT8(B_DTS_ENCODER_FLAG)==0x0))
    {
        return;
    }
    */
    if (fgEnable == TRUE)
    {
       DSP_LoadRamCode(AUD_DSP0, AUD_DTSENC);
    }


    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);

    //Fix AMODE = 9
    vWriteShmUINT8(AUD_DSP0, B_DTS_ENCODER_AMODE,9);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_DTS_ENCODER_AMODE);


}

//-----------------------------------------------------------------------------
/** _AUD_DspSetDDCOEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCOEnable(BOOL fgEnable)
{
#ifndef CC_AUD_SUPPORT_MS10
    UINT16  u2ShmIndex = B_DDCO_FLAG;
    UINT16 u2UopIndex = UOP_DSP_DDCO_ENABLE;

    if ((fgEnable == TRUE) && (uReadShmUINT8(AUD_DSP0, B_DDCO_FLAG)==0x1))
    {
        return;
    }

    if ((fgEnable == FALSE) && (uReadShmUINT8(AUD_DSP0, B_DDCO_FLAG)==0x0))
    {
        return;
    }

    if (fgEnable == TRUE)
    {
       DSP_LoadRamCode(AUD_DSP0, AUD_DDCO);
    }

    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
#else
    //turn off MDC
    if(fgEnable == TRUE)
    {
        vWriteShmUINT8(AUD_DSP0, B_DDT_CONTROL, 2 + (UINT8)fgEnable);    //to output AAC raw        
    }
    else
    {
        vWriteShmUINT8(AUD_DSP0, B_DDT_CONTROL, (UINT8)fgEnable);    //to output AAC raw
    }

#endif
}

BOOL _AUD_DspIsDDCOEnable(void)
{
    UINT8 u1Flag;
	
    u1Flag = uReadShmUINT8(AUD_DSP0, B_DDT_CONTROL) & 0x2;    //=1 means output AAC RAW
    return (u1Flag != 2);
}
//-----------------------------------------------------------------------------
/** _AUD_DspSetDDCOAgcEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCOAgcEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_DDCO_AGC_FLAG;
    UINT16 u2UopIndex = UOP_DSP_DDCO_AGC_ENABLE;

    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetDDCOLFEEnable
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetDDCOLFEEnable(BOOL fgEnable)
{
    UINT16  u2ShmIndex = B_DDCO_LFE_LPF_FLAG;
    UINT16 u2UopIndex = UOP_DSP_DDCO_LFE_LPF_ENABLE;

    vWriteShmUINT8(AUD_DSP0, u2ShmIndex, (UINT8)fgEnable);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
extern void _AUD_DataUploadSetMode(UINT8 u1Mode);
extern void _AUD_DataUpload_Init (void);
extern void _AUD_DataUpload_start (void);
extern void _AUD_DataUploadQueueInit (UINT32 u4BaseAddr, UINT32 u4BlockSize, UINT32 u4BlockNum);

//-----------------------------------------------------------------------------
/** _AUD_DspSetUploadDataEnable
 *
 *  @param  void
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetUploadDataEnable (void)
{
    /// Set W_UPLOAD_DATA_FLAG at _AUD_DspSetUploadMode();
    UINT16 u2UopIndex = UOP_DSP_UPLOAD_DATA_ENABLE;
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}

extern UINT32 u4AUD_UPLOAD_BUFFER_SIZE;

//-----------------------------------------------------------------------------
/** _AUD_DspSetUploadMode
 *
 *  @param  0: off, 1: PCM, 2: MP3, 3: SBC Encoder, 4: ALSA, 5: WiFi
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifdef  MP3ENC_SUPPORT
UINT32 u4MP3EncFrameSize = (MP3ENC_FRAME_SIZE);
#endif
void _AUD_DspSetUploadMode(UINT8 u1Mode)
{
    _AUD_DataUploadSetMode(u1Mode);
    _AUD_DataUpload_Init();
    LOG(0, "_AUD_DspSetUploadMode %d", u1Mode);
    switch (u1Mode)
    {
        case 0://Off
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, 0x8000); //reset when off is enabled
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, 128);
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 16);
            
            #if defined (CC_AUD_MP3ENC_FAKEDATA_SUPPORT) && defined (CONFIG_HIGH_RES_TIMERS)
            AUD_StopTimer();
            #endif
            break;
        case 1://PCM
            /// bit0 = 1
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, 1<<15 | 1<<(u1Mode-1)); ///1<<15 for PCM record initial.
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, 128);
            // Block size = 256*2(Ch)*2(16bits) = 1024 bytes
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 4);
            _AUD_DataUpload_start();
            break;
    #ifdef  MP3ENC_SUPPORT
        case 2://Mp3Enc
            /// bit1 = 1
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, 1<<(u1Mode-1));
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, 128);
            ///Block Size, MP3=0x3C0(320kbps)/0x240(192kbps)/0x180(128kbps)
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, u4MP3EncFrameSize);
            ///Evey 8 mp3 frame send one interrupt
            ///8*1152/48000 = 0.192sec
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 8);
            _AUD_DataUpload_start();
            
            #if defined (CC_AUD_MP3ENC_FAKEDATA_SUPPORT) && defined (CONFIG_HIGH_RES_TIMERS)
            AUD_StartTimer(MP3ENC_FRAME_TIME_ELAPSED);
            #endif
            break;
    #endif
        case 3://SBCEnc
            /// bit1 = 1
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, 1<<4);
            // note that B_UPLOAD_BLOCK_NUM cannot exceed 128 [UINT8 only]
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, 128);
            ///Block Size = 8 x SBCE Frame Size = 8x128 bytes
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 8*128);
            ///Evey 4*8 SBCE frame send one interrupt
            ///8*8*8*16/48000 = 0.17sec
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 8);
            _AUD_DataUpload_start();
            break;
        case 4:
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, 0xe008); // [15] for initial, [14] for skip interrupt notify, [13] for little endian, [3] upload pcm after post-proc
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, u4AUD_UPLOAD_BUFFER_SIZE/0x400);
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 16);
            break;
        case 5://WiFi
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, 0xa008); ///1<<15 for PCM record initial.
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, 128);
            // Block size = 256*2(Ch)*2(16bits) = 1024 bytes
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 16);
            _AUD_DataUpload_start();
            break;
       case 6:
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, (1<<15) | (1<< 13) | 1); ///1<<15 for PCM (inter)record initial.
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, 128);
            // Block size = 256*2(Ch)*2(16bits) = 1024 bytes
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 16);
            _AUD_DataUpload_start();
            break;
        case 7:
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, 0xe001); // [15] for initial, [14] for skip interrupt notify, [13] for little endian, [0] upload pcm before post-proc
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, u4AUD_UPLOAD_BUFFER_SIZE/0x400);
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 16);
            break;
        case 8:
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, 0xf001); // [15] for initial, [14] for skip interrupt notify, [13] for little endian, [12] post-proc wait upload RP/WP,[0] upload pcm before post-proc
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, u4AUD_UPLOAD_BUFFER_SIZE/0x400);
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 16);
            break;
/////////////////   AENC_SUPPORT
        case 10:    
            /// bit0 = 1
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, 1<<15 | (1<< 13) | 1 ); ///1<<15 for PCM record initial, [13] for little endian
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, 128);
            // Block size = 256*2(Ch)*2(16bits) = 1024 bytes
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 2);
            _AUD_DataUpload_start();
            break;
        case 11: // AAC encoder upload form AUX decoder
             vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, (1<<15) | (1<< 13) | (1<<11)); 
             vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, 128);
             // Block size = 256*2(Ch)*2(16bits)
             vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 0x400); 
             vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 2);
             _AUD_DataUpload_start();
             break;
        case 12: // AAC encoder upload form THIRD decoder
              vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, (1<<15) | (1<< 13) | (1<<12)); 
             vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, 128);
             vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 0x400);
             vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 2);
             _AUD_DataUpload_start();
              break;
/////////////////   AENC_SUPPORT                  
        default:
            //TODO
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_DATA_FLAG, 0);
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLOCK_NUM, 128);
            vWriteShmUINT16 (AUD_DSP0, W_UPLOAD_BLOCK_SIZE, 0x400);
            vWriteShmUINT8 (AUD_DSP0, B_UPLOAD_BLK_INT_CNT, 16);
            break;
    }
    _AUD_DataUploadQueueInit (_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE,
                                u2ReadShmUINT16(AUD_DSP0, W_UPLOAD_BLOCK_SIZE),
                                uReadShmUINT8(AUD_DSP0, B_UPLOAD_BLOCK_NUM));
}
//#endif

#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
extern void _AUD_DataBluetoothSetMode(UINT8 u1Mode);
extern void _AUD_DataBluetooth_Init (void);
extern void _AUD_DataBluetooth_start (void);
extern void _AUD_DataBluetoothQueueInit (UINT32 u4BaseAddr, UINT32 u4BlockSize, UINT32 u4BlockNum);

//-----------------------------------------------------------------------------
/** _AUD_DspSetUploadDataEnable
 *
 *  @param  void
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetBluetoothDataEnable (void)
{
    /// Set W_UPLOAD_DATA_FLAG at _AUD_DspSetUploadMode();
    UINT16 u2UopIndex = UOP_DSP_BLUETOOTH_DATA_ENABLE;
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}
void _AUD_DspSetBluetoothCnt(UINT8 cnt)
{
    vWriteShmUINT8 (AUD_DSP0, B_BLUETOOTH_BLK_INT_CNT, cnt);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetUploadMode
 *
 *  @param  0: off, 1: PCM, 2: MP3, 3: SBC Encoder, 4: ALSA, 5: WiFi
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetBluetoothMode(UINT8 u1Mode)
{
    _AUD_DataBluetoothSetMode(u1Mode);
    _AUD_DataBluetooth_Init();
    switch (u1Mode)
    {
        case 0://Off
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_DATA_FLAG, 0x8000); //reset when off is enabled
            vWriteShmUINT8 (AUD_DSP0, B_BLUETOOTH_BLOCK_NUM, 128);
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_BLOCK_SIZE, 0x400);
            //vWriteShmUINT8 (B_BLUETOOTH_BLK_INT_CNT, 16);
            break;
        case 1://PCM
            /// bit0 = 1
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_DATA_FLAG, 1<<15 | 1<<(u1Mode-1)); ///1<<15 for PCM record initial.
            vWriteShmUINT8 (AUD_DSP0, B_BLUETOOTH_BLOCK_NUM, 128);
            // Block size = 256*2(Ch)*2(16bits) = 1024 bytes
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_BLOCK_SIZE, 0x400);
            //vWriteShmUINT8 (B_BLUETOOTH_BLK_INT_CNT, 16);
            _AUD_DataBluetooth_start();
            break;
        case 4:
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_DATA_FLAG, 0xe008); // [15] for initial, [14] for skip interrupt notify, [13] for little endian, [3] upload pcm after post-proc
            vWriteShmUINT8 (AUD_DSP0, B_BLUETOOTH_BLOCK_NUM, AUD_UPLOAD_BUFFER_SIZE/0x400);
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_BLOCK_SIZE, 0x400);
            //vWriteShmUINT8 (B_BLUETOOTH_BLK_INT_CNT, 16);
            break;
        case 5://WiFi
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_DATA_FLAG, 0xa008); ///1<<15 for PCM record initial.
            vWriteShmUINT8 (AUD_DSP0, B_BLUETOOTH_BLOCK_NUM, 128);
            // Block size = 256*2(Ch)*2(16bits) = 1024 bytes
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_BLOCK_SIZE, 0x400);
            //vWriteShmUINT8 (B_BLUETOOTH_BLK_INT_CNT, 16);
            _AUD_DataBluetooth_start();
            break;
       case 6:
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_DATA_FLAG, (1<<15) | (1<< 13) | 1); ///1<<15 for PCM (inter)record initial.
            vWriteShmUINT8 (AUD_DSP0, B_BLUETOOTH_BLOCK_NUM, 64);
            // Block size = 256*2(Ch)*2(16bits) = 1024 bytes
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_BLOCK_SIZE, 0x400);
            //vWriteShmUINT8 (B_BLUETOOTH_BLK_INT_CNT, 4);
            _AUD_DataBluetooth_start();
            break;
        case 3://SBC
        default:
            //TODO
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_DATA_FLAG, 0);
            vWriteShmUINT8 (AUD_DSP0, B_BLUETOOTH_BLOCK_NUM, 128);
            vWriteShmUINT16 (AUD_DSP0, W_BLUETOOTH_BLOCK_SIZE, 0x400);
            //vWriteShmUINT8 (B_BLUETOOTH_BLK_INT_CNT, 16);
            break;
    }
    _AUD_DataBluetoothQueueInit (_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX] + AUD_BLUETOOTH_BUFFER_BASE,
                                u2ReadShmUINT16(AUD_DSP0, W_BLUETOOTH_BLOCK_SIZE),
                                uReadShmUINT8(AUD_DSP0, B_BLUETOOTH_BLOCK_NUM));
}
#endif

void _AUD_DspSetEncMode(UINT8 u1DecId, AUD_ENC_MODE_T eEncMode)
{
    DSP_SetEncMode(u1DecId, eEncMode);
#ifdef CC_AUD_DDI
    AUD_DRVSetEncodeType(u1DecId, eEncMode != AUD_ENC_MODE_OFF);
#endif
}

#if 0   // Unused
void _AUD_DspSetEncInfo(UINT8 u1DecId, UINT32 u4Bitrates)
{
    UNUSED(u1DecId);
    switch(u4Bitrates)
    {
        case 320000:
            #ifdef  MP3ENC_SUPPORT
            u4MP3EncFrameSize = 0x3C0;
            #endif
            break;

        case 192000:
            #ifdef  MP3ENC_SUPPORT
            u4MP3EncFrameSize = 0x240;
            #endif
            break;

        case 128000:
            #ifdef  MP3ENC_SUPPORT
            u4MP3EncFrameSize = 0x180;
            #endif
            break;

        default:
            break;
    }
}
#endif

#ifdef SUPPORT_PHILIPS_LOUDNESS
//-----------------------------------------------------------------------------
/** _AUD_DspSetLoudness
 *
 *  @param  u1DecId, enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetLoudness(UINT8 u1DecId, BOOL fgEnable)
{
    // 120Hz  : +5,
    // 200Hz  : -3
    // 500Hz  : +2
    // 1.2KHz :  0
    // 3  KHz : +2
    // 7.5KHz : +2
    // 12 KHz : +5
    UINT8 u1Idx;
    INT8 i1EqValue;
    AUD_EQ_TYPE_T  eEqType;

    UNUSED(u1DecId);
    UNUSED(fgEnable);

#ifdef CC_AUD_7BAND_EQUALIZER   // currently only support 7band EQ + Loudness
    if (_fgLoudnessEnable == fgEnable)
    {
        return;
    }

    VERIFY(x_sema_lock(_ahEqCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _AUD_DspGetEqType(u1DecId, &eEqType);
    _fgLoudnessEnable = fgEnable;

    if (eEqType != AUD_EQ_OFF)
    {
        for (u1Idx = 1; u1Idx <= CH_EQ_BAND_NO; u1Idx++)
        {
            i1EqValue = _aai1ChEqCfg[eEqType][u1Idx];

            if (fgEnable)
            {
                i1EqValue += i1Loundness[u1Idx-1];
            }
            else
            {
                i1EqValue -= i1Loundness[u1Idx-1];
            }

            i1EqValue = (i1EqValue > 28) ? 28: i1EqValue;
            i1EqValue = (i1EqValue <-28) ? -28: i1EqValue;
            _aai1ChEqCfg[eEqType][u1Idx] = i1EqValue;
        }
    }

    VERIFY(x_sema_unlock(_ahEqCtlSema) == OSR_OK);
#endif
}

void AUD_DspChEqLoudness(UINT8 u1DecId, INT8 *prEqvalue)
{
    UINT8 u1Idx;
    INT8 i1EqValue;

    for(u1Idx = 0; u1Idx <CH_EQ_BAND_NO; u1Idx++)
    {
        i1EqValue = prEqvalue[u1Idx];

        if (_fgLoudnessEnable)
        {
            i1EqValue += i1Loundness[u1Idx];
        }
        else
        {
            i1EqValue -= i1Loundness[u1Idx];
        }

        i1EqValue = (i1EqValue > 28) ? 28: i1EqValue;
        i1EqValue = (i1EqValue <-28) ? -28: i1EqValue;
        prEqvalue[u1Idx] = i1EqValue;
    }
}


BOOL _AUD_DspGetLoudness(UINT8 u1DecId, BOOL fgEnable)
{
    return _fgLoudnessEnable;
}

#endif

void _AUD_DspSetAoutReinit(void)
{
    UINT16 u2UopIndex = UOP_DSP_AOUT_REINIT_FOR_SAMPLING_RATE_CHANGE;
    UINT16 u2UopIndex2 = UOP_DSP_AOUT_REINIT_FOR_SAMPLING_RATE_CHANGE_DEC2;

    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex2);
    DSP_SendDspTaskCmd(AUD_DSP0, u2UopIndex);
}




//-----------------------------------------------------------------------------
// Begin of NVM and Profile Functions
//   for customers use EEPROM and FLASH audio parameters
//   1. _AUD_NVM_Get_Profile
//   2. _AUD_NVM_Read
//   3. _AUD_NVM_Function
//   4. AUD_NVM_Init_From_EEPROM
//   5. AUD_NVM_Init_To_EEPROM
//   6. AUD_NVM_Operation
//-----------------------------------------------------------------------------
#ifdef CC_AUD_USE_NVM
UINT32 _AUD_NVM_Get_Profile(SMART_AUD_GROUP_INFO_T* prSmartAudioInfo)
{
    UINT32 u4FlashBase;
    UINT8  u1PartId;
    //AUD_SMART_GROUP_INFO_T eSmartInfo;
    UINT8 eSmartInfo[(6*(8+3))+(2*CH_EQ_BAND_NO)] = {0};  //temp store data for D_INC
    UINT32 u1Ret = 0;
    UINT8 i;
#if 1 //20120808, wonder, to clean warning msg.
    UINT8* pu1Temp;
    pu1Temp = (UINT8*)x_mem_alloc(AUD_FLASH_LENGTH);
    if(pu1Temp==NULL) {return 0;}
#else //warning: the frame size of 1232 bytes is larger than 1024 bytes
    UINT8 u1Temp[AUD_FLASH_LENGTH]; //temp store data from Flash.
#endif

    UNUSED(_u1AudProfile);
//    UNUSED(_fgAudNVMLipSync);
    UNUSED(_u2AudNVMProfileOffset);

    // ROW use NAND Flash now.
    u4FlashBase = DRVCUST_OptGet(eAudioFlashAqBase);
    u1PartId = DRVCUST_OptGet(eAudioDataFlashPartId);

    u4FlashBase = u4FlashBase + (AUD_FLASH_LENGTH*_u1AudProfile[0]);// According to Profile number.

    // AQ : 0x00 ~ 0x12 (Common)
    //      0x13 ~ 0x1C (Common Reserved)
    //      0x00 ~ 0x1A (AQ)
    //      0x1B ~ 0x24 (AQ Reserved)
    //      1+4 Set. 177 Bytes.
    // V1.1
    // MODE 1- MODE 7 1094 - 1135
    // ASSISTANT 57 - 68
    // EQ 69 - 82

#if 1 //20120801, wonder, to clean warning msg.
    if (AUD_FlashAQReadEx(u1PartId, u4FlashBase, pu1Temp, AUD_FLASH_LENGTH) == AUD_FLASH_LENGTH)
#else
    if (AUD_FlashAQReadEx(u1PartId, u4FlashBase, (UINT8*)&u1Temp, AUD_FLASH_LENGTH) == AUD_FLASH_LENGTH)
#endif
    {
        for (i=0; i<7; i++) //skip common data (i=0)
        {
            // AQ mode 1-7
            //x_memcpy((UINT8*)VIRTUAL(&eSmartInfo[6*i]), (UINT8*)&u1Temp[(6*i)+69], 6);
#if 1 //20120808, wonder, to clean warning msg.
            x_memcpy((UINT8*)VIRTUAL((UINT32)eSmartInfo+(6*i)), (UINT8*)(pu1Temp+(6*i)+1094), 6);
#else
            x_memcpy((UINT8*)VIRTUAL(&eSmartInfo[6*i]), (UINT8*)&u1Temp[(6*i)+1094], 6);
#endif
        }

        i++; // for  mode 8 no data

        for (; i<(8+3); i++)
        {
            // Assistant mode
#if 1 //20120808, wonder, to clean warning msg.
            x_memcpy((UINT8*)VIRTUAL((UINT32)eSmartInfo+(6*i)), (UINT8*)(pu1Temp+(4*(i-8))+57), 4);
#else
            x_memcpy((UINT8*)VIRTUAL(&eSmartInfo[6*i]), (UINT8*)&u1Temp[(4*(i-8))+57], 4);
#endif
        }
        LOG(0, "Read NAND Flash AQ Data Success!\n");
        u1Ret = 0;
    }
    else
    {
        for (i=0; i<6*(8+3); i++)
        {
            eSmartInfo[i] = 0;
        }
        LOG(0, "Read NAND Flash AQ Data Failed!\n");
        u1Ret = 1;
    }

    // EQ band
    for (i=0; i<2; i++)
    {
#if 1 //20120808, wonder, to clean warning msg.
        x_memcpy((UINT8*)VIRTUAL((UINT32)eSmartInfo+66+(i*CH_EQ_BAND_NO)), (UINT8*)(pu1Temp+69+(i*CH_EQ_BAND_NO)), CH_EQ_BAND_NO);
#else
        x_memcpy((UINT8*)VIRTUAL(&eSmartInfo[66+(i*CH_EQ_BAND_NO)]), (UINT8*)&u1Temp[69+(i*CH_EQ_BAND_NO)], CH_EQ_BAND_NO);
#endif
    }

#if 1 //20120801, wonder, to clean warning msg.
    x_memcpy((UINT8*)VIRTUAL((UINT32)prSmartAudioInfo), (UINT8*)&eSmartInfo, sizeof(SMART_AUD_GROUP_INFO_T));
#else
    x_memcpy((UINT8*)VIRTUAL(prSmartAudioInfo), (UINT8*)&eSmartInfo, sizeof(SMART_AUD_GROUP_INFO_T));
#endif

#if 1 //20120808, wonder, to clean warning msg.
    x_mem_free(pu1Temp);
#endif
    return u1Ret;
}

UINT8 _AUD_NVM_Get_Profile_Id(void)
{
    return _u1AudProfile[0];
}

void _AUD_NVM_Get_FlashAQ_List(UINT8 u1Profile, UINT8 *pu1Data)
{
    UINT32 u4FlashBase;
    UINT8 u1PartId;

    if (u1Profile >= DRVCUST_OptGet(eAudioProfileTotalNumber))
    {
        LOG(0, "Profile index is not correct\n");
        return;
    }

    pu1Data[0] = '\0';

    u4FlashBase = DRVCUST_OptGet(eAudioFlashAqBase);
    u1PartId = DRVCUST_OptGet(eAudioDataFlashPartId);

    u4FlashBase = u4FlashBase + (AUD_FLASH_LENGTH * u1Profile);
    UNUSED(AUD_FlashAQReadEx(u1PartId, u4FlashBase, pu1Data, AUD_FLASH_LENGTH));
}
#endif //CC_AUD_USE_NVM

#ifdef CC_AUD_NVM_CHECK_VALID
UINT8 _AUD_NVM_Crc(UINT8 u1Data, UINT8 u1Crc)
{
    UINT8 i;

    for (i = 0; i < 8; i ++)
    {
        if (((u1Data << i) ^ u1Crc) & 0x80)
        {
            u1Crc |= 0x31;      // x8 + x5 + x4 + 1
        }
        u1Crc <<= 1;
    }

    return u1Crc;
}

BOOL _AUD_NVM_Valid(void)
{
    UINT16 i, j;
    UINT8 u1Buffer[2] = {0};
    UINT32 u4NvmLength;

    // Get Customer NVM Data Length
    u4NvmLength = DRVCUST_OptGet(eAudioNvmLength);

    au1NvmCrc = 0;
    // Calc checksum
    for (i = 0; i < u4NvmLength; i++)
    {
        for (j = 0; j < aeNvmData[i].u1Length; j++)
        {
            au1NvmCrc = _AUD_NVM_Crc(aeNvmData[i].u4DefaultValue >> (j*8), au1NvmCrc);
        }
    }
    
    if (EEPROM_Read((UINT64)AUD_NVM_OFFSET+AUD_NVM_LENGTH-2, (UINT32)u1Buffer, 2))
    {
        return FALSE;   //EEPROM Read Fail, return FLASE
    }

    return (u1Buffer[0] == AUD_NVM_FLAG) && (u1Buffer[1] == au1NvmCrc);
}
#endif /* CC_AUD_NVM_CHECK_VALID */

#ifdef CC_AUD_USE_NVM
UINT32 _AUD_NVM_Read(void)
{
    UINT8 uTempBuf[16];
    UINT8 i;
    UINT32 u4Ptr;
    UINT32 u4NvmLength; //Customer NVM Data Length
    UINT32 u4FlashBase;
    UINT8  u1PartId;

    UNUSED(_fgNvmFlat);

    // Check if NVM is enabled
    if ( !(DRVCUST_OptGet(eAudioNvmEnable)) )
    {
        return 1;   //NVM is not ON.
    }

    // Get Customer NVM Data Length
    u4NvmLength = DRVCUST_OptGet(eAudioNvmLength);

    // Get Customer NVM Data Structure and Data
    VERIFY(DRVCUST_OptQuery(eAudioNvmData, (UINT32 *)&u4Ptr) == 0);
    x_memcpy((VOID *)VIRTUAL((UINT32)aeNvmData), (const VOID *)u4Ptr, sizeof(AUD_NVM_DATA_T) * u4NvmLength);

#ifdef CC_AUD_NVM_CHECK_VALID
    // Check eeprom
    if (!_AUD_NVM_Valid())
    {
        LOG(0, "Aud nvm is invalid!! Go to initial eeprom!\n");
        UNUSED(AUD_NVM_Init_To_EEPROM());
    }
#endif

    // ------- Read NVM from EEPROM ----------------------
    if (EEPROM_Read((UINT64)AUD_NVM_OFFSET, (UINT32)auAudNvmBuffer, AUD_NVM_LENGTH))
    {
        return 1;   //EEPROM Read Fail, return 1
    }

    // ------- Read Profile Releated Data from EEPROM -----
    for (i=0; i<3; i++)
    {
        if (EEPROM_Read((UINT64)_u2AudNVMProfileOffset[i], (UINT32)uTempBuf, 16))
        {
            return 1;
        }
        _fgAudNVMLipSync[i] = uTempBuf[2] & 0x04; // bit2 of byte 3
        _u1AudProfile[i] = (uTempBuf[13] & 0x70) >> 4;  // bit4 ~ 6 of byte 14
    }
    if (_u1AudProfile[0] != _u1AudProfile[1])
    {
        if (_u1AudProfile[1] == _u1AudProfile[2])
        {
            _u1AudProfile[0] = _u1AudProfile[1];
            _fgAudNVMLipSync[0] = _fgAudNVMLipSync[1];
        }
    }
    if (_u1AudProfile[0] >= DRVCUST_OptGet(eAudioProfileTotalNumber))   //FIXME!!! if NVM not initialized, use profile 0
    {
        _u1AudProfile[0] = 0;
    }
    // ---------------------------------------------------------------------

    _fgNvmInit = TRUE;
    u4FlashBase = DRVCUST_OptGet(eAudioFlashAqBase);
    u1PartId = DRVCUST_OptGet(eAudioDataFlashPartId);

    u4FlashBase = u4FlashBase + (AUD_FLASH_LENGTH*_u1AudProfile[0]);// According to Profile number.

    UNUSED(AUD_FlashAQReadEx(u1PartId, u4FlashBase, (UINT8*)&auAudFlashCommBuffer, AUD_FLASH_COMMON_LENGTH));

    // Get Customer Comm Data Sturcture Length
    u4NvmLength = DRVCUST_OptGet(eAudioFlashCommLength);

    // Get Customer Comm Data Structure and Data
    VERIFY(DRVCUST_OptQuery(eAudioFlashCommData, (UINT32 *)&u4Ptr) == 0);
#if 1 //20120801, wonder, to clean warning msg.
    x_memcpy((VOID *)VIRTUAL((UINT32)aeFlashCommData), (const VOID *)u4Ptr, sizeof(AUD_NVM_DATA_T) * u4NvmLength);
#else
    x_memcpy((VOID *)VIRTUAL(aeFlashCommData), (const VOID *)u4Ptr, sizeof(AUD_NVM_DATA_T) * u4NvmLength);
#endif
    Printf("Read NAND Flash AQ Comm Data Success!\n");

    return 0;
}

UINT32 _AUD_NVM_Function(AUD_NVM_DATA_T rNvmData, UINT32 u4Data)
{
    INT16 i2PreScale;

    UNUSED(i2PreScale);

    switch (rNvmData.eDataType)
    {
        case AUD_NVM_MTS_OBSRV_TIME :
            vWriteShmUINT16(W_MTS_NUM_CHECK, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_STEREO_TIME :
            vWriteShmUINT16(W_MTS_NUM_PILOT, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_SAP_TIME :
            vWriteShmUINT16(W_MTS_NUM_SAP, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_UP_THR_BTSC :
            vWriteShmUINT16(W_MTS_STEREO_CON_MID, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_LO_THR_BTSC :
            vWriteShmUINT16(W_MTS_STEREO_MID, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_UP_THR_SAP :
            vWriteShmUINT16(W_MTS_SAP_CON_MID, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_LO_THR_SAP:
            vWriteShmUINT16(W_MTS_SAP_MID, (UINT16)u4Data);
            break;
        case AUD_NVM_MTS_NMUTE_FM_UPTHR :
            vWriteShmUINT8(B_FM_MUTE_THRESHOLD_HIGHT_MTS, (UINT8)u4Data);
            break;
        case AUD_NVM_MTS_NMUTE_FM_LOTHR :
            vWriteShmUINT8(B_FM_MUTE_THRESHOLD_LOW_MTS, (UINT8)u4Data);
            break;
        case AUD_NVM_MTS_NMUTE_SAP_UPTHR :
            vWriteShmUINT8(B_MTS_MUTE_SAP_HIGH, (UINT8)u4Data);
            break;
        case AUD_NVM_MTS_NMUTE_SAP_LOTHR :
            vWriteShmUINT8(B_MTS_MUTE_SAP_LOW, (UINT8)u4Data);
            break;
        case AUD_NVM_MTS_PRE_BTSC_STMONO :
            //vWriteShmUINT32(D_MTS_FINE_TUNE_VOLUME, u4Data);
            vWriteShmUINT32(D_MTS_FINE_TUNE_VOLUME, (VOLUME_FINE_TUNE[((u4Data>40)?40:u4Data)]) << 8);
            break;
        case AUD_NVM_MTS_PRE_SAP :
            //vWriteShmUINT32(D_SAP_FINE_TUNE_VOLUME, u4Data);
            vWriteShmUINT32(D_SAP_FINE_TUNE_VOLUME, (VOLUME_FINE_TUNE[((u4Data>40)?40:u4Data)]) << 8);
            break;
        case AUD_NVM_MTS_HIGH_DEV :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_HDEV_MODE) & 0xfb;
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x04;
            }
            vWriteShmUINT8(B_HDEV_MODE, u1Mode);
            break;
        }
        case AUD_NVM_MTS_CARRIER_SHIFT :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_CARRIER_SHIFT_MODE) & 0xfb;
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x04;
            }
            vWriteShmUINT8(B_CARRIER_SHIFT_MODE, u1Mode);
            break;
        }
        case AUD_NVM_MTS_FMCARR_MUTE :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_FM_MUTE_MODE) & 0xfb;
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x04;
            }
            vWriteShmUINT8(B_FM_MUTE_MODE, u1Mode);
            break;
        }
        // ========= PAL =========================
        case AUD_NVM_PAL_HDEV_MODE :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_HDEV_MODE) & 0xfe; // clear bit 0 for PAL
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x01;
            }
            vWriteShmUINT8(B_HDEV_MODE, u1Mode);
            break;
        }
        case AUD_NVM_PAL_CR_SHIFT :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_CARRIER_SHIFT_MODE) & 0xfe;
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x01;
            }
            vWriteShmUINT8(B_CARRIER_SHIFT_MODE, u1Mode);
            break;
        }
        case AUD_NVM_PAL_AM_MUTE :
            vWriteShmUINT8 (B_AM_MUTE_MODE,(UINT8)u4Data);
            break;
        case AUD_NVM_PAL_UP_THR_AM :
            vWriteShmUINT8 (B_AM_MUTE_THRESHOLD_HIGHT,(UINT8)u4Data);
            break;
        case AUD_NVM_PAL_LO_THR_AM :
            vWriteShmUINT8 (B_AM_MUTE_THRESHOLD_LOW,(UINT8)u4Data);
            break;
        case AUD_NVM_PAL_FM_MUTE :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_FM_MUTE_MODE) & 0xfe; // Clear bit 0 for PAL
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x01;
            }
            vWriteShmUINT8(B_FM_MUTE_MODE, u1Mode);
            break;
        }
        case AUD_NVM_PAL_UP_THR_FM :
            vWriteShmUINT8 (B_FM_MUTE_THRESHOLD_HIGHT_PAL,(UINT8)u4Data);
            break;
        case AUD_NVM_PAL_LO_THR_FM :
            vWriteShmUINT8 (B_FM_MUTE_THRESHOLD_LOW_PAL, (UINT8)u4Data);
            break;
        case AUD_NVM_PAL_PRE :
            /*
            vWriteShmUINT32( D_PAL_FINE_TUNE_VOLUME,
            (DSP_GetFineTuneVol((UINT8)u4Data)) << 8);
            */
            u4Data = (u4Data <= 5) ? 20 : u4Data; //It's not possible that the fine tune volume < 5, we think it's NVM data error.
            eAudNVMAtvPreGain.u4PAL_FineGain[AUD_NVM_NON_DK] = (DSP_GetFineTuneVol((UINT8)u4Data) << 8);
            eAudNVMAtvPreGain.u1PAL_FineGain[AUD_NVM_NON_DK] = (UINT8)u4Data;
            break;
        case AUD_NVM_PAL_PRE_DK :
            u4Data = (u4Data <= 5) ? 20 : u4Data; //It's not possible that the fine tune volume < 5, we think it's NVM data error.
            eAudNVMAtvPreGain.u4PAL_FineGain[AUD_NVM_DK] = (DSP_GetFineTuneVol((UINT8)u4Data) << 8);
            eAudNVMAtvPreGain.u1PAL_FineGain[AUD_NVM_DK] = (UINT8)u4Data;
            break;
        case AUD_NVM_PAL_AM_PRE :
            /*
            vWriteShmUINT32( D_AM_FINE_TUNE_VOLUME,
            (DSP_GetFineTuneVol((UINT8)u4Data)) << 8);
            */
            u4Data = (u4Data <= 5) ? 20 : u4Data; //It's not possible that the fine tune volume < 5, we think it's NVM data error.
            eAudNVMAtvPreGain.u4AM_FineGain[AUD_NVM_NON_DK] = (DSP_GetFineTuneVol((UINT8)u4Data) << 8);
            eAudNVMAtvPreGain.u1AM_FineGain[AUD_NVM_NON_DK] = (UINT8)u4Data;
            break;
        case AUD_NVM_PAL_AM_PRE_DK :
            u4Data = (u4Data <= 5) ? 20 : u4Data; //It's not possible that the fine tune volume < 5, we think it's NVM data error.
            eAudNVMAtvPreGain.u4AM_FineGain[AUD_NVM_DK] = (DSP_GetFineTuneVol((UINT8)u4Data) << 8);
            eAudNVMAtvPreGain.u1AM_FineGain[AUD_NVM_DK] = (UINT8)u4Data;
            break;
        case AUD_NVM_PAL_NICAM_PRE :
            /*
            vWriteShmUINT32( D_NICAM_FINE_TUNE_VOLUME,
            (DSP_GetFineTuneVol((UINT8)u4Data)) << 8);
            */
            u4Data = (u4Data <= 5) ? 20 : u4Data; //It's not possible that the fine tune volume < 5, we think it's NVM data error.
            eAudNVMAtvPreGain.u4NICAM_FineGain[AUD_NVM_NON_DK] = (DSP_GetFineTuneVol((UINT8)u4Data) << 8);
            eAudNVMAtvPreGain.u1NICAM_FineGain[AUD_NVM_NON_DK] = (UINT8)u4Data;
            break;
        case AUD_NVM_PAL_NICAM_PRE_DK :
            u4Data = (u4Data <= 5) ? 20 : u4Data; //It's not possible that the fine tune volume < 5, we think it's NVM data error.
            eAudNVMAtvPreGain.u4NICAM_FineGain[AUD_NVM_DK] = (DSP_GetFineTuneVol((UINT8)u4Data) << 8);
            eAudNVMAtvPreGain.u1NICAM_FineGain[AUD_NVM_DK] = (UINT8)u4Data;
            break;
        case AUD_NVM_PAL_NON_EU :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_NON_EU_FM_MODE) & 0xfe;
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x01;
            }
            vWriteShmUINT8(B_NON_EU_FM_MODE, u1Mode);
            break;
        }

        // ========== A2 ==========================
        case AUD_NVM_A2_HDEV_MODE :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_HDEV_MODE) & 0xfd; // clear bit 1 for A2
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x02;
            }
            vWriteShmUINT8(B_HDEV_MODE, u1Mode);
            break;
        }
        case AUD_NVM_A2_CR_SHIFT :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_CARRIER_SHIFT_MODE) & 0xfd;
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x02;
            }
            vWriteShmUINT8(B_CARRIER_SHIFT_MODE, u1Mode);
            break;
        }
        case AUD_NVM_A2_FM_MUTE :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_FM_MUTE_MODE) & 0xfd;
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x02;
            }
            vWriteShmUINT8(B_FM_MUTE_MODE, u1Mode);
            break;
        }
        case AUD_NVM_A2_UP_THR_FM :
            vWriteShmUINT8 (B_FM_MUTE_THRESHOLD_HIGHT_A2, (UINT8)u4Data);
            break;
        case AUD_NVM_A2_LO_THR_FM :
            vWriteShmUINT8 (B_FM_MUTE_THRESHOLD_LOW_A2,  (UINT8)u4Data);
            break;
        case AUD_NVM_A2_PRE :
            /*
            vWriteShmUINT32(D_A2_FINE_TUNE_VOLUME,
            (DSP_GetFineTuneVol((UINT8)u4Data)) << 8);
            */
            u4Data = (u4Data <= 5) ? 20 : u4Data; //It's not possible that the fine tune volume < 5, we think it's NVM data error.
            eAudNVMAtvPreGain.u4A2_FineGain[AUD_NVM_NON_DK] = (DSP_GetFineTuneVol((UINT8)u4Data) << 8);
            eAudNVMAtvPreGain.u1A2_FineGain[AUD_NVM_NON_DK] = (UINT8)u4Data;
            break;
        case AUD_NVM_A2_PRE_DK :
            u4Data = (u4Data <= 5) ? 20 : u4Data; //It's not possible that the fine tune volume < 5, we think it's NVM data error.
            eAudNVMAtvPreGain.u4A2_FineGain[AUD_NVM_DK] = (DSP_GetFineTuneVol((UINT8)u4Data) << 8);
            eAudNVMAtvPreGain.u1A2_FineGain[AUD_NVM_DK] = (UINT8)u4Data;
            break;
        case AUD_NVM_A2_NON_EU :
        {
            UINT8 u1Mode;
            u1Mode = uReadShmUINT8(B_NON_EU_FM_MODE) & 0xfd;
            if ((UINT8)u4Data)
            {
                u1Mode |= 0x02;
            }
            vWriteShmUINT8(B_NON_EU_FM_MODE, u1Mode);
            break;
        }
        case AUD_NVM_AVC_ADJUST_RATE :
            //vWriteShmUINT16(W_AVC_ADJUST_RATE, (UINT16)u4Data);
            break;
        case AUD_NVM_AVC_UI_ADJUST_RATE :
            //vWriteShmUINT16 (W_DSP_AVC_UI_ADJUST_RATE, (UINT16)u4Data); // (16.8) format, 4dB / sec @ 32k Hz
            break;
#ifdef CC_AUD_AVC_V20
        case AUD_NVM_AVC_LEVEL :
            vWriteShmUINT32 (D_AVC_TARGET_LEVEL, (UINT32)u4Data);
            vWriteShmUINT32 (D_AVC_TARGET_LEVEL_1, (UINT32)u4Data);
            break;
#endif
        case AUD_NVM_AVC_GAIN_UP :
            //vWriteShmUINT16 (W_AVC_MAX_GAIN_UP, (UINT16)u4Data);
            break;
        case AUD_NVM_LIPSYNC_A :
            vWriteShmUINT16 (W_CHDELAY_C, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_L, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_R, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_LS, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_RS, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH7, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH8, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_SUB, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH9, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH10, (UINT16)u4Data);
            break;
        case AUD_NVM_LIPSYNC_B : //HP delay for TPV_PHILIP
            vWriteShmUINT16 (W_CHDELAY_CH9, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH10, (UINT16)u4Data);
            break;
        case AUD_NVM_LIPSYNC_C : //SPDIF delay for TPV_PHILIP
            vWriteShmUINT16 (W_CHDELAY_CH7, (UINT16)u4Data);
            vWriteShmUINT16 (W_CHDELAY_CH8, (UINT16)u4Data);
            break;
        case AUD_NVM_PL2_ON :
        {
            UINT16 u2PL2Cfg;
            u2PL2Cfg = u2ReadShmUINT16(W_PLIICONFIG) & (0xFFFE);
            vWriteShmUINT16 (W_PLIICONFIG, (u2PL2Cfg | (UINT16)(u4Data & 0x1)));
            break;
        }
        case AUD_NVM_PL2_MODE :
            vWriteShmUINT16 (W_PLIIMODE, (UINT16)u4Data);
            break;
        case AUD_NVM_REVERB:
        {
            #if 0   // Reverb is removed
            UINT8 u1Mode;
            if ((UINT8)u4Data > 0)
            {
                u1Mode = (UINT8)u4Data - 1;
                vWriteShmUINT8(B_REVERBFLAG, 1);
                vWriteShmUINT32(D_REVERBGAIN,  _arReverMode[u1Mode].u4Gain);
                vWriteShmUINT8(B_REVERBBANK0, _arReverMode[u1Mode].u1ReverbBank0);
                vWriteShmUINT8(B_REVERBBANK1, _arReverMode[u1Mode].u1ReverbBank1);
                vWriteShmUINT8(B_REVERBBANK2, _arReverMode[u1Mode].u1ReverbBank2);
                vWriteShmUINT8(B_REVERBBANK3, _arReverMode[u1Mode].u1ReverbBank3);
            }
            else
            {
                vWriteShmUINT8(B_REVERBFLAG, 0);
            }
            #endif
            break;
        }
    #ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
        case AUD_NVM_VOL_MIN :
            u4NvmVolTab[0] = u4Data;
            u4NvmHPVolTab[0] = u4Data;
            break;
        case AUD_NVM_VOL_MAX :
            u4NvmVolTab[VOL_TABLE_CHK_PNT-1] = u4Data;
            break;
        case AUD_NVM_VOL_STEP1 :
            u4NvmVolTab[1] = u4Data;
            break;
        case AUD_NVM_VOL_STEP2 :
            u4NvmVolTab[2] = u4Data;
            break;
        case AUD_NVM_VOL_STEP3 :
            u4NvmVolTab[3] = u4Data;
            break;
        case AUD_NVM_VOL_STEP4 :
            u4NvmVolTab[4] = u4Data;
            break;
        case AUD_NVM_VOL_STEP5 :
            u4NvmVolTab[5] = u4Data;
            break;
    #else
        case AUD_NVM_VOL_MIN :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL][0] = u4Data;
            u4NvmVolTab[AUD_NVM_R_VOL_TBL][0] = u4Data;
            u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][0] = u4Data;
            u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][0] = u4Data;
            u4NvmHPVolTab[0] = u4Data;
            break;
        // Vol (L ch)
        case AUD_NVM_L_VOL_MAX :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL][VOL_TABLE_CHK_PNT-1] = u4Data;
            break;
        case AUD_NVM_L_VOL_STEP1 :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL][1] = u4Data;
            break;
        case AUD_NVM_L_VOL_STEP2 :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL][2] = u4Data;
            break;
        case AUD_NVM_L_VOL_STEP3 :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL][3] = u4Data;
            break;
        case AUD_NVM_L_VOL_STEP4 :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL][4] = u4Data;
            break;
        case AUD_NVM_L_VOL_STEP5 :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL][5] = u4Data;
            break;
        // Vol (R ch)
        case AUD_NVM_R_VOL_MAX :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL][VOL_TABLE_CHK_PNT-1] = u4Data;
            break;
        case AUD_NVM_R_VOL_STEP1 :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL][1] = u4Data;
            break;
        case AUD_NVM_R_VOL_STEP2 :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL][2] = u4Data;
            break;
        case AUD_NVM_R_VOL_STEP3 :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL][3] = u4Data;
            break;
        case AUD_NVM_R_VOL_STEP4 :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL][4] = u4Data;
            break;
        case AUD_NVM_R_VOL_STEP5 :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL][5] = u4Data;
            break;
        // Vol (L ch for skype)
        case AUD_NVM_L_VOL_MAX_SKYPE :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][VOL_TABLE_CHK_PNT-1] = u4Data;
            break;
        case AUD_NVM_L_VOL_STEP1_SKYPE :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][1] = u4Data;
            break;
        case AUD_NVM_L_VOL_STEP2_SKYPE :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][2] = u4Data;
            break;
        case AUD_NVM_L_VOL_STEP3_SKYPE :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][3] = u4Data;
            break;
        case AUD_NVM_L_VOL_STEP4_SKYPE :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][4] = u4Data;
            break;
        case AUD_NVM_L_VOL_STEP5_SKYPE :
            u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][5] = u4Data;
            break;
        // Vol (R ch for skype)
        case AUD_NVM_R_VOL_MAX_SKYPE :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][VOL_TABLE_CHK_PNT-1] = u4Data;
            break;
        case AUD_NVM_R_VOL_STEP1_SKYPE :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][1] = u4Data;
            break;
        case AUD_NVM_R_VOL_STEP2_SKYPE :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][2] = u4Data;
            break;
        case AUD_NVM_R_VOL_STEP3_SKYPE :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][3] = u4Data;
            break;
        case AUD_NVM_R_VOL_STEP4_SKYPE :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][4] = u4Data;
            break;
        case AUD_NVM_R_VOL_STEP5_SKYPE :
            u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][5] = u4Data;
            break;
   #endif
        case AUD_NVM_HP_VOL_MAX :
            u4NvmHPVolTab[VOL_TABLE_CHK_PNT-1] = u4Data;
            break;
        case AUD_NVM_HP_VOL_STEP1 :
            u4NvmHPVolTab[1] = u4Data;
            break;
        case AUD_NVM_HP_VOL_STEP2 :
            u4NvmHPVolTab[2] = u4Data;
            break;
        case AUD_NVM_HP_VOL_STEP3 :
            u4NvmHPVolTab[3] = u4Data;
            break;
        case AUD_NVM_HP_VOL_STEP4 :
            u4NvmHPVolTab[4] = u4Data;
            break;
        case AUD_NVM_HP_VOL_STEP5 :
            u4NvmHPVolTab[5] = u4Data;
            break;

        case AUD_NVM_PREHDMI :
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -24)
            {
                i2PreScale = -24; //at most -12dB
            }
            _rNVMIuputSrcGain.i2NvmInputSrcHDMI = i2PreScale;
            break;
        case AUD_NVM_PREAV :
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -24)
            {
                i2PreScale = -24; //at most -12dB
            }
            _rNVMIuputSrcGain.i2NvmInputSrcAV = i2PreScale;
            break;
        case AUD_NVM_PREUSB :
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -24)
            {
                i2PreScale = -24; //at most -12dB
            }
            _rNVMIuputSrcGain.i2NvmInputSrcUSB = i2PreScale;
            break;
        case AUD_NVM_PREATV :
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -24)
            {
                i2PreScale = -24; //at most -12dB
            }
            _rNVMIuputSrcGain.i2NvmInputSrcATV = i2PreScale;
            break;
        case AUD_NVM_PREDTV :
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -24)
            {
                i2PreScale = -24; //at most -12dB
            }
            _rNVMIuputSrcGain.i2NvmInputSrcDTV = i2PreScale;
            break;
        case AUD_NVM_AMP_MUTE:
            break;
        case AUD_NVM_LR_EXTRA_LINEIN:
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -128)
            {
                i2PreScale = -128; //at most -64dB
            }
            if (i2PreScale > 96)
            {
                i2PreScale = 96; //at most 48dB
            }
            _i4NvmExtraVolGain[AUD_CH_FRONT_LEFT] = i2PreScale;
            _i4NvmExtraVolGain[AUD_CH_FRONT_RIGHT] = i2PreScale;
            break;
        case AUD_NVM_LR_EXTRA_OTHERS:
            i2PreScale = ((UINT8)u4Data - 128);
            if (i2PreScale < -128)
            {
                i2PreScale = -128; //at most -64dB
            }
            if (i2PreScale > 96)
            {
                i2PreScale = 96; //at most 48dB
            }
            _i4NvmExtraVolGain1[AUD_CH_FRONT_LEFT] = i2PreScale;
            _i4NvmExtraVolGain1[AUD_CH_FRONT_RIGHT] = i2PreScale;
            break;
    #ifdef CC_AUD_SPIDF_LVL
        case AUD_NVM_SPDIF_MORE:
            _u1AudSpdifLevel[0] = (UINT8)u4Data;
            break;
        case AUD_NVM_SPDIF_MEDIUM:
            _u1AudSpdifLevel[1] = (UINT8)u4Data;
            break;
        case AUD_NVM_SPDIF_LESS:
            _u1AudSpdifLevel[2] = (UINT8)u4Data;
            break;
   #endif
        case AUD_NVM_BASS_SUBWOOFER :
        {
            UINT32 u4Config;
            u4Config = u4ReadShmUINT32(D_SPKCFG) & 0xffffffdf;
            if ((UINT8)u4Data)
            {
                u4Config |= 0x20;
            }
            vWriteShmUINT32(D_SPKCFG, u4Config);
            break;
        }
        case AUD_NVM_BASS_SPKSIZE :
        {
            UINT32 u4Config;
            u4Config = u4ReadShmUINT32(D_SPKCFG) & 0xffff9fff;
            if ((UINT8)u4Data)
            {
                u4Config |= 0x6000;
            }
            vWriteShmUINT32(D_SPKCFG, u4Config);
            break;
        }
        case AUD_NVM_BASS_MANA_ONOFF :
            if ((BOOL)u4Data)
            {
                vWriteShmUINT16(W_PROCMOD, u2ReadShmUINT16(W_PROCMOD) & ~((UINT16)(BYPASS_BASS_MANAGEMENT | BYPASS_POST_PROCESSING)));
            }
            else
            {
                vWriteShmUINT16(W_PROCMOD, u2ReadShmUINT16(W_PROCMOD) | (UINT16)BYPASS_BASS_MANAGEMENT);
            }
            break;
        case AUD_NVM_DUAL_MONO :
            vWriteShmUINT8(B_MANUAL_OUTPUT_MATRIX, (u4Data == 1) ? AUD_MATRIX_OUT_LR_MIX : AUD_MATRIX_OUT_OFF);
            break;
#ifdef CC_AUD_BBE_SUPPORT
        case AUD_NVM_BBE_LEVEL :
            vWriteShmUINT8 (B_BBE_LEVEL, (UINT8)u4Data);
            _i1BbeLevel = (UINT8)u4Data;
            break;
        case AUD_NVM_BBE_UPGAIN :
            vWriteShmUINT8 (B_BBE_PROCESS, (UINT8)u4Data);
            _u1BbeProcess = (UINT8)u4Data;
            break;
        case AUD_NVM_BBE_LOGAIN :
            vWriteShmUINT8 (B_BBE_LO_CONTOUR, (UINT8)u4Data);
            _u1BbeLoContour = (UINT8)u4Data;
            break;
        case AUD_NVM_BBE_3DGAIN :
            vWriteShmUINT8 (B_BBE_3D_GAIN, (UINT8)u4Data);
            _u1Bbe3dGain = (UINT8)u4Data;
            break;
#endif
        case AUD_NVM_VS_CLARITY :
            vWriteShmUINT32 (D_VSURR_CLARITY, (u4Data << 16));
            _rNVMSurrCfg[0].u4Clarity = u4Data << 16;
            break;
        case AUD_NVM_VS_WIDTH :
            vWriteShmUINT32 (D_VSURR_WIDTH, (u4Data << 16));
            _rNVMSurrCfg[0].u4Width = u4Data << 16;
            break;
        case AUD_NVM_VS_LRGAIN :
            vWriteShmUINT32 (D_VSURR_LR_GAIN, (u4Data << 16));
            _rNVMSurrCfg[0].u4LRGain = u4Data << 16;
            break;
        case AUD_NVM_VS_XTALK_GIAN :
            vWriteShmUINT32 (D_VSURR_CROS_TALK, (u4Data << 16));
            _rNVMSurrCfg[0].u4Xtalk = u4Data << 16;
            break;
        case AUD_NVM_VS_OPGAIN :
            vWriteShmUINT32 (D_VSURR_OUTPUT_GAIN, (u4Data << 16));
            _rNVMSurrCfg[0].u4OutputGain = u4Data << 16;
            break;
        case AUD_NVM_VS_SWGAIN :
            vWriteShmUINT32 (D_VSURR_BASS_GAIN, (u4Data << 16));
            _rNVMSurrCfg[0].u4BassGain = u4Data << 16;
            break;
        case AUD_NVM_3DVS_CLARITY :
            //vWriteShmUINT32 (D_VSURR_CLARITY, (u4Data << 16));
            _rNVMSurrCfg[1].u4Clarity = u4Data << 16;
            break;
        case AUD_NVM_3DVS_WIDTH :
            //vWriteShmUINT32 (D_VSURR_WIDTH, (u4Data << 16));
            _rNVMSurrCfg[1].u4Width = u4Data << 16;
            break;
        case AUD_NVM_3DVS_LRGAIN :
            //vWriteShmUINT32 (D_VSURR_LR_GAIN, (u4Data << 16));
            _rNVMSurrCfg[1].u4LRGain = u4Data << 16;
            break;
        case AUD_NVM_3DVS_XTALK_GIAN :
            //vWriteShmUINT32 (D_VSURR_CROS_TALK, (u4Data << 16));
            _rNVMSurrCfg[1].u4Xtalk = u4Data << 16;
            break;
        case AUD_NVM_3DVS_OPGAIN :
            //vWriteShmUINT32 (D_VSURR_OUTPUT_GAIN, (u4Data << 16));
            _rNVMSurrCfg[1].u4OutputGain = u4Data << 16;
            break;
        case AUD_NVM_3DVS_SWGAIN :
            //vWriteShmUINT32 (D_VSURR_BASS_GAIN, (u4Data << 16));
            _rNVMSurrCfg[1].u4BassGain = u4Data << 16;
            break;
        case AUD_NVM_VS_FO :
            vWriteShmUINT32 (D_VSURR_FO, u4Data);
            _rNVMSurrCfg[0].u4Fo = u4Data;
            _rNVMSurrCfg[1].u4Fo = u4Data;
            break;
        case AUD_NVM_FLAT :
        {
            UINT16 u2Mode;
            if ((UINT8)u4Data == 1)
            {
                //bypass delay and post-processing, keep trim and bass management (1010)
                u2Mode = u2ReadShmUINT16(W_PROCMOD) | (0xA000);
                vWriteShmUINT16 (W_PROCMOD, u2Mode);
                _fgNvmFlat = TRUE;
            }
            else
            {
                _fgNvmFlat = FALSE;
            }
            break;
        }
        case AUD_NVM_LIMITER_MODE:
            vWriteShmUINT16(W_LIMITER_MODE, (UINT16)u4Data);
            break;
        case AUD_NVM_LIMITER_FLAG:
            vWriteShmUINT16(W_LIMITER_FLAG, (UINT16)u4Data);
            break;
        case AUD_NVM_LIMITER_THRES:
            vWriteShmUINT32(D_LIMITER_FIX_THRESHOLD, u4Data); // default is 0x100000
            break;
#ifdef CC_AUD_DRC_V20
        case AUD_NVM_LIMITER_ATTACK_RATE:
            vWriteShmUINT32(D_LIMITER_ATTACK_RATE, u4Data);
            break;
        case AUD_NVM_LIMITER_RELEASE_RATE:
            vWriteShmUINT32(D_LIMITER_RELEASE_RATE, u4Data);
            break;
        case AUD_NVM_LIMITER_RATIO:
            vWriteShmUINT32(D_LIMITER_GAIN_RATIO, u4Data);
            break;
#ifdef CC_AUD_AVC_V20
       case AUD_NVM_AVL_FC:
            vWriteShmUINT16(W_AVC_FC, (UINT16)u4Data);
            break;
        case AUD_NVM_AVL_BAND0_TARGET:
            vWriteShmUINT32(D_AVC_TARGET_LEVEL, u4Data);
            break;
        case AUD_NVM_AVL_BAND0_ATTACK:
            vWriteShmUINT32(D_AVC_ATTACK_RATE, u4Data);
            break;
        case AUD_NVM_AVL_BAND0_RELEASE:
            vWriteShmUINT32(D_AVC_RELEASE_RATE, u4Data);
            break;
        case AUD_NVM_AVL_BAND0_RATIO:
            vWriteShmUINT32(D_AVC_RATIO, u4Data);
            break;
        case AUD_NVM_AVL_BAND0_SILENCE:
            vWriteShmUINT32(D_AVC_SILENCE_LEVEL, u4Data);
            break;
        case AUD_NVM_AVL_BAND0_EXPAND:
            vWriteShmUINT32(D_AVC_MAX_EXPAND, u4Data);
            break;
        case AUD_NVM_AVL_BAND0_HOLD:
            vWriteShmUINT32(D_AVC_HOLD, u4Data);
            break;
        case AUD_NVM_AVL_BAND1_TARGET:
            vWriteShmUINT32(D_AVC_TARGET_LEVEL_1, u4Data);
            break;
        case AUD_NVM_AVL_BAND1_ATTACK:
            vWriteShmUINT32(D_AVC_ATTACK_RATE_1, u4Data);
            break;
        case AUD_NVM_AVL_BAND1_RELEASE:
            vWriteShmUINT32(D_AVC_RELEASE_RATE_1, u4Data);
            break;
        case AUD_NVM_AVL_BAND1_RATIO:
            vWriteShmUINT32(D_AVC_RATIO_1, u4Data);
            break;
        case AUD_NVM_AVL_BAND1_SILENCE:
            vWriteShmUINT32(D_AVC_SILENCE_LEVEL_1, u4Data);
            break;
        case AUD_NVM_AVL_BAND1_EXPAND:
            vWriteShmUINT32(D_AVC_MAX_EXPAND_1, u4Data);
            break;
        case AUD_NVM_AVL_BAND1_HOLD:
            vWriteShmUINT32(D_AVC_HOLD_1, u4Data);
            break;
#endif  // CC_AUD_AVC_V20
#endif  // CC_AUD_DRC_V20
        case AUD_NVM_CUTOFF_FREQ:
            vWriteShmUINT32 (D_CUTOFF_FREQ, (u4Data*10));
            break;
#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
        case AUD_NVM_NOTCH_FC:
            vWriteShmUINT16 (W_CDNOTCH_USER_FC, (UINT16)u4Data);
            _au1CDNotchFc[0] = (UINT16)u4Data;
            break;
        case AUD_NVM_NOTCH_Q_ON:
            _au1CDNotchQIdx[0] = (UINT8)u4Data;
            break;
        case AUD_NVM_NOTCH_Q_OFF:
            _au1CDNotchQIdx[1] = (UINT8)u4Data;
            break;
#endif
        // Normal & clear soud on
        case AUD_NVM_EQ_USER1_1_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST1][1] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][1] = _aai1ChEqCfg[AUD_EQ_CUST1][1] + _ai1ChEqOffset[1];
            break;
        case AUD_NVM_EQ_USER1_2_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST1][2] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][2] = _aai1ChEqCfg[AUD_EQ_CUST1][2] + _ai1ChEqOffset[2];
            break;
        case AUD_NVM_EQ_USER1_3_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST1][3] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][3] = _aai1ChEqCfg[AUD_EQ_CUST1][3] + _ai1ChEqOffset[3];
            break;
        case AUD_NVM_EQ_USER1_4_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST1][4] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][4] = _aai1ChEqCfg[AUD_EQ_CUST1][4] + _ai1ChEqOffset[4];
            break;
        case AUD_NVM_EQ_USER1_5_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST1][5] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][5] = _aai1ChEqCfg[AUD_EQ_CUST1][5] + _ai1ChEqOffset[5];
            break;
#ifdef CC_AUD_7BAND_EQUALIZER
        case AUD_NVM_EQ_USER1_6_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST1][6] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][6] = _aai1ChEqCfg[AUD_EQ_CUST1][6] + _ai1ChEqOffset[6];
            break;
        case AUD_NVM_EQ_USER1_7_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST1][7] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][7] = _aai1ChEqCfg[AUD_EQ_CUST1][7] + _ai1ChEqOffset[7];
            break;
#endif
        case AUD_NVM_EQ_USER2_1_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST2][1] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][1] = _aai1ChEqCfg[AUD_EQ_CUST2][1] + _ai1ChEqOffset[1];
            break;
        case AUD_NVM_EQ_USER2_2_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST2][2] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][2] = _aai1ChEqCfg[AUD_EQ_CUST2][2] + _ai1ChEqOffset[2];
            break;
        case AUD_NVM_EQ_USER2_3_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST2][3] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][3] = _aai1ChEqCfg[AUD_EQ_CUST2][3] + _ai1ChEqOffset[3];
            break;
        case AUD_NVM_EQ_USER2_4_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST2][4] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][4] = _aai1ChEqCfg[AUD_EQ_CUST2][4] + _ai1ChEqOffset[4];
            break;
        case AUD_NVM_EQ_USER2_5_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST2][5] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][5] = _aai1ChEqCfg[AUD_EQ_CUST2][5] + _ai1ChEqOffset[5];
            break;
#ifdef CC_AUD_7BAND_EQUALIZER
        case AUD_NVM_EQ_USER2_6_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST2][6] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][6] = _aai1ChEqCfg[AUD_EQ_CUST2][6] + _ai1ChEqOffset[6];
            break;
        case AUD_NVM_EQ_USER2_7_CS_ON:
            _aai1ChEqCfg[AUD_EQ_CUST2][7] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][7] = _aai1ChEqCfg[AUD_EQ_CUST2][7] + _ai1ChEqOffset[7];
            break;
#endif
        // Normal & clear soud off
        case AUD_NVM_EQ_USER1_1_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST5][1] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][1] = _aai1ChEqCfg[AUD_EQ_CUST5][1] + _ai1ChEqOffset[1];
            break;
        case AUD_NVM_EQ_USER1_2_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST5][2] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][2] = _aai1ChEqCfg[AUD_EQ_CUST5][2] + _ai1ChEqOffset[2];
            break;
        case AUD_NVM_EQ_USER1_3_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST5][3] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][3] = _aai1ChEqCfg[AUD_EQ_CUST5][3] + _ai1ChEqOffset[3];
            break;
        case AUD_NVM_EQ_USER1_4_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST5][4] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][4] = _aai1ChEqCfg[AUD_EQ_CUST5][4] + _ai1ChEqOffset[4];
            break;
        case AUD_NVM_EQ_USER1_5_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST5][5] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][5] = _aai1ChEqCfg[AUD_EQ_CUST5][5] + _ai1ChEqOffset[5];
            break;
#ifdef CC_AUD_7BAND_EQUALIZER
        case AUD_NVM_EQ_USER1_6_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST5][6] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][6] = _aai1ChEqCfg[AUD_EQ_CUST5][6] + _ai1ChEqOffset[6];
            break;
        case AUD_NVM_EQ_USER1_7_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST5][7] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][7] = _aai1ChEqCfg[AUD_EQ_CUST5][7] + _ai1ChEqOffset[7];
            break;
#endif
        case AUD_NVM_EQ_USER2_1_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST6][1] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][1] = _aai1ChEqCfg[AUD_EQ_CUST6][1] + _ai1ChEqOffset[1];
            break;
        case AUD_NVM_EQ_USER2_2_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST6][2] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][2] = _aai1ChEqCfg[AUD_EQ_CUST6][2] + _ai1ChEqOffset[2];
            break;
        case AUD_NVM_EQ_USER2_3_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST6][3] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][3] = _aai1ChEqCfg[AUD_EQ_CUST6][3] + _ai1ChEqOffset[3];
            break;
        case AUD_NVM_EQ_USER2_4_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST6][4] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][4] = _aai1ChEqCfg[AUD_EQ_CUST6][4] + _ai1ChEqOffset[4];
            break;
        case AUD_NVM_EQ_USER2_5_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST6][5] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][5] = _aai1ChEqCfg[AUD_EQ_CUST6][5] + _ai1ChEqOffset[5];
            break;
#ifdef CC_AUD_7BAND_EQUALIZER
        case AUD_NVM_EQ_USER2_6_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST6][6] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][6] = _aai1ChEqCfg[AUD_EQ_CUST6][6] + _ai1ChEqOffset[6];
            break;
        case AUD_NVM_EQ_USER2_7_CS_OFF:
            _aai1ChEqCfg[AUD_EQ_CUST6][7] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][7] = _aai1ChEqCfg[AUD_EQ_CUST6][7] + _ai1ChEqOffset[7];
            break;
#endif
        // Skype
        case AUD_NVM_EQ_USER3_1:
            _aai1ChEqCfg[AUD_EQ_CUST3][1] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][1] = _aai1ChEqCfg[AUD_EQ_CUST3][1] + _ai1ChEqOffset[1];
            break;
        case AUD_NVM_EQ_USER3_2:
            _aai1ChEqCfg[AUD_EQ_CUST3][2] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][2] = _aai1ChEqCfg[AUD_EQ_CUST3][2] + _ai1ChEqOffset[2];
            break;
        case AUD_NVM_EQ_USER3_3:
            _aai1ChEqCfg[AUD_EQ_CUST3][3] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][3] = _aai1ChEqCfg[AUD_EQ_CUST3][3] + _ai1ChEqOffset[3];
            break;
        case AUD_NVM_EQ_USER3_4:
            _aai1ChEqCfg[AUD_EQ_CUST3][4] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][4] = _aai1ChEqCfg[AUD_EQ_CUST3][4] + _ai1ChEqOffset[4];
            break;
        case AUD_NVM_EQ_USER3_5:
            _aai1ChEqCfg[AUD_EQ_CUST3][5] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][5] = _aai1ChEqCfg[AUD_EQ_CUST3][5] + _ai1ChEqOffset[5];
            break;
#ifdef CC_AUD_7BAND_EQUALIZER
        case AUD_NVM_EQ_USER3_6:
            _aai1ChEqCfg[AUD_EQ_CUST3][6] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][6] = _aai1ChEqCfg[AUD_EQ_CUST3][6] + _ai1ChEqOffset[6];
            break;
        case AUD_NVM_EQ_USER3_7:
            _aai1ChEqCfg[AUD_EQ_CUST3][7] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST3][7] = _aai1ChEqCfg[AUD_EQ_CUST3][7] + _ai1ChEqOffset[7];
            break;
#endif
        case AUD_NVM_EQ_USER4_1:
            _aai1ChEqCfg[AUD_EQ_CUST4][1] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][1] = _aai1ChEqCfg[AUD_EQ_CUST4][1] + _ai1ChEqOffset[1];
            break;
        case AUD_NVM_EQ_USER4_2:
            _aai1ChEqCfg[AUD_EQ_CUST4][2] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][2] = _aai1ChEqCfg[AUD_EQ_CUST4][2] + _ai1ChEqOffset[2];
            break;
        case AUD_NVM_EQ_USER4_3:
            _aai1ChEqCfg[AUD_EQ_CUST4][3] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][3] = _aai1ChEqCfg[AUD_EQ_CUST4][3] + _ai1ChEqOffset[3];
            break;
        case AUD_NVM_EQ_USER4_4:
            _aai1ChEqCfg[AUD_EQ_CUST4][4] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][4] = _aai1ChEqCfg[AUD_EQ_CUST4][4] + _ai1ChEqOffset[4];
            break;
        case AUD_NVM_EQ_USER4_5:
            _aai1ChEqCfg[AUD_EQ_CUST4][5] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][5] = _aai1ChEqCfg[AUD_EQ_CUST4][5] + _ai1ChEqOffset[5];
            break;
#ifdef CC_AUD_7BAND_EQUALIZER
        case AUD_NVM_EQ_USER4_6:
            _aai1ChEqCfg[AUD_EQ_CUST4][6] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][6] = _aai1ChEqCfg[AUD_EQ_CUST4][6] + _ai1ChEqOffset[6];
            break;
        case AUD_NVM_EQ_USER4_7:
            _aai1ChEqCfg[AUD_EQ_CUST4][7] = ((UINT8)u4Data - EQ_BAND_GAIN_SIDE_INDEX_MAX);
            //_aai1ChEqCfg[AUD_EQ_CUST4][7] = _aai1ChEqCfg[AUD_EQ_CUST4][7] + _ai1ChEqOffset[7];
            break;
#endif
#ifdef CC_AUD_MIXSOUND_SUPPORT
        //case AUD_NVM_BEEP_FC:
        //    _u1MixSndClipIdx = (UINT8)u4Data;//TODO CHECK
        //    break;
        case AUD_NVM_BEEP_VOLUME:
            _AUD_DspSetMixSndVol((UINT8)u4Data);
            break;
        case AUD_NVM_BEEP_DURATION:
            _AUD_DspSetMixSndLoop((UINT8)u4Data);
            break;
#endif
#ifdef CC_AUD_VBASS_SUPPORT
        case AUD_NVM_VB_ONOFF:
            vWriteShmUINT8 (B_VIR_BASS_ENABLE, (UINT8)u4Data);
            break;
        case AUD_NVM_VB_FC:
            AUD_SetVBassCutOff((UINT8)u4Data);
            break;
        case AUD_NVM_VB_GAIN1:
            vWriteShmUINT32 (D_VIR_BASS_GAIN, (UINT16)u4Data);
            break;
        case AUD_NVM_VB_GAIN2:
            vWriteShmUINT32 (D_VIR_BASS_GAIN_2, (UINT16)u4Data);
            break;
#endif
#if 1//def CC_AUD_PEQ_SUPPORT
        case AUD_NVM_PEQ_ONOFF:
            vWriteShmUINT8 (B_PEQ_ENABLE, (UINT8)u4Data);
            break;
        case AUD_NVM_PEQ_FC_USER1_0:
            _arPEQTable[AUD_PEQ_CASE0][0].u2PeqFc = (UINT16)u4Data;// for Bass
            _arPEQTable[AUD_PEQ_CASE1][0].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE2][0].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE3][0].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][0].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][0].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER1_1:
            _arPEQTable[AUD_PEQ_CASE0][1].u2PeqFc = (UINT16)u4Data;// for Treble
            _arPEQTable[AUD_PEQ_CASE1][1].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE2][1].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE3][1].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][1].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][1].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER1_2: // Wall Normal
            _arPEQTable[AUD_PEQ_CASE0][2].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][2].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER1_3:
            _arPEQTable[AUD_PEQ_CASE0][3].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][3].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER1_4:
            _arPEQTable[AUD_PEQ_CASE0][4].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][4].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER1_5:
            _arPEQTable[AUD_PEQ_CASE0][5].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][5].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER1_6:
            _arPEQTable[AUD_PEQ_CASE0][6].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][6].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER1_7:
            _arPEQTable[AUD_PEQ_CASE0][7].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][7].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER1_8:
            _arPEQTable[AUD_PEQ_CASE0][8].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][8].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER2_2: // Table Normal
            _arPEQTable[AUD_PEQ_CASE1][2].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][2].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER2_3:
            _arPEQTable[AUD_PEQ_CASE1][3].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][3].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER2_4:
            _arPEQTable[AUD_PEQ_CASE1][4].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][4].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER2_5:
            _arPEQTable[AUD_PEQ_CASE1][5].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][5].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER2_6:
            _arPEQTable[AUD_PEQ_CASE1][6].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][6].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER2_7:
            _arPEQTable[AUD_PEQ_CASE1][7].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][7].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER2_8:
            _arPEQTable[AUD_PEQ_CASE1][8].u2PeqFc = (UINT16)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][8].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER3_2: // Wall Skype
            _arPEQTable[AUD_PEQ_CASE2][2].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER3_3:
            _arPEQTable[AUD_PEQ_CASE2][3].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER3_4:
            _arPEQTable[AUD_PEQ_CASE2][4].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER3_5:
            _arPEQTable[AUD_PEQ_CASE2][5].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER3_6:
            _arPEQTable[AUD_PEQ_CASE2][6].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER3_7:
            _arPEQTable[AUD_PEQ_CASE2][7].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER3_8:
            _arPEQTable[AUD_PEQ_CASE2][8].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER4_2: // Table Skype
            _arPEQTable[AUD_PEQ_CASE3][2].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER4_3:
            _arPEQTable[AUD_PEQ_CASE3][3].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER4_4:
            _arPEQTable[AUD_PEQ_CASE3][4].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER4_5:
            _arPEQTable[AUD_PEQ_CASE3][5].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER4_6:
            _arPEQTable[AUD_PEQ_CASE3][6].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER4_7:
            _arPEQTable[AUD_PEQ_CASE3][7].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_FC_USER4_8:
            _arPEQTable[AUD_PEQ_CASE3][8].u2PeqFc = (UINT16)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER1_0:
            _arPEQTable[AUD_PEQ_CASE0][0].u1PeqQ= (UINT8)u4Data;// for Bass
            _arPEQTable[AUD_PEQ_CASE1][0].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE2][0].u1PeqQ= (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE3][0].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][0].u1PeqQ= (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][0].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER1_1:
            _arPEQTable[AUD_PEQ_CASE0][1].u1PeqQ = (UINT8)u4Data;// for Treble
            _arPEQTable[AUD_PEQ_CASE1][1].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE2][1].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE3][1].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][1].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][1].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER1_2: // Wall Normal
            _arPEQTable[AUD_PEQ_CASE0][2].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][2].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER1_3:
            _arPEQTable[AUD_PEQ_CASE0][3].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][3].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER1_4:
            _arPEQTable[AUD_PEQ_CASE0][4].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][4].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER1_5:
            _arPEQTable[AUD_PEQ_CASE0][5].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][5].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER1_6:
            _arPEQTable[AUD_PEQ_CASE0][6].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][6].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER1_7:
            _arPEQTable[AUD_PEQ_CASE0][7].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][7].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER1_8:
            _arPEQTable[AUD_PEQ_CASE0][8].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE4][8].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER2_2: // Table Normal
            _arPEQTable[AUD_PEQ_CASE1][2].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][2].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER2_3:
            _arPEQTable[AUD_PEQ_CASE1][3].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][3].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER2_4:
            _arPEQTable[AUD_PEQ_CASE1][4].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][4].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER2_5:
            _arPEQTable[AUD_PEQ_CASE1][5].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][5].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER2_6:
            _arPEQTable[AUD_PEQ_CASE1][6].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][6].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER2_7:
            _arPEQTable[AUD_PEQ_CASE1][7].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][7].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER2_8:
            _arPEQTable[AUD_PEQ_CASE1][8].u1PeqQ = (UINT8)u4Data;
            _arPEQTable[AUD_PEQ_CASE5][8].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER3_2: // Wall Skype
            _arPEQTable[AUD_PEQ_CASE2][2].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER3_3:
            _arPEQTable[AUD_PEQ_CASE2][3].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER3_4:
            _arPEQTable[AUD_PEQ_CASE2][4].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER3_5:
            _arPEQTable[AUD_PEQ_CASE2][5].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER3_6:
            _arPEQTable[AUD_PEQ_CASE2][6].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER3_7:
            _arPEQTable[AUD_PEQ_CASE2][7].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER3_8:
            _arPEQTable[AUD_PEQ_CASE2][8].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER4_2: // Table Skype
            _arPEQTable[AUD_PEQ_CASE3][2].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER4_3:
            _arPEQTable[AUD_PEQ_CASE3][3].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER4_4:
            _arPEQTable[AUD_PEQ_CASE3][4].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER4_5:
            _arPEQTable[AUD_PEQ_CASE3][5].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER4_6:
            _arPEQTable[AUD_PEQ_CASE3][6].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER4_7:
            _arPEQTable[AUD_PEQ_CASE3][7].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_Q_USER4_8:
            _arPEQTable[AUD_PEQ_CASE3][8].u1PeqQ = (UINT8)u4Data;
            break;
        case AUD_NVM_PEQ_GAIN_USER1_0:
            _arPEQTable[AUD_PEQ_CASE0][0].i2PeqGain= (INT16)((UINT8)u4Data - 24);// for Bass
            _arPEQTable[AUD_PEQ_CASE1][0].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            _arPEQTable[AUD_PEQ_CASE2][0].i2PeqGain= (INT16)((UINT8)u4Data - 24);
            _arPEQTable[AUD_PEQ_CASE3][0].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            _arPEQTable[AUD_PEQ_CASE4][0].i2PeqGain= (INT16)((UINT8)u4Data - 24);
            _arPEQTable[AUD_PEQ_CASE5][0].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_1:
            _arPEQTable[AUD_PEQ_CASE0][1].i2PeqGain = (INT16)((UINT8)u4Data - 24);// for Treble
            _arPEQTable[AUD_PEQ_CASE1][1].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            _arPEQTable[AUD_PEQ_CASE2][1].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            _arPEQTable[AUD_PEQ_CASE3][1].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            _arPEQTable[AUD_PEQ_CASE4][1].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            _arPEQTable[AUD_PEQ_CASE5][1].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        // Normal & clear sound on
        case AUD_NVM_PEQ_GAIN_USER1_2_CS_ON: // Wall Normal
            _arPEQTable[AUD_PEQ_CASE0][2].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_3_CS_ON:
            _arPEQTable[AUD_PEQ_CASE0][3].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_4_CS_ON:
            _arPEQTable[AUD_PEQ_CASE0][4].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_5_CS_ON:
            _arPEQTable[AUD_PEQ_CASE0][5].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_6_CS_ON:
            _arPEQTable[AUD_PEQ_CASE0][6].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_7_CS_ON:
            _arPEQTable[AUD_PEQ_CASE0][7].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_8_CS_ON:
            _arPEQTable[AUD_PEQ_CASE0][8].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_2_CS_ON: // Table Normal
            _arPEQTable[AUD_PEQ_CASE1][2].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_3_CS_ON:
            _arPEQTable[AUD_PEQ_CASE1][3].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_4_CS_ON:
            _arPEQTable[AUD_PEQ_CASE1][4].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_5_CS_ON:
            _arPEQTable[AUD_PEQ_CASE1][5].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_6_CS_ON:
            _arPEQTable[AUD_PEQ_CASE1][6].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_7_CS_ON:
            _arPEQTable[AUD_PEQ_CASE1][7].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_8_CS_ON:
            _arPEQTable[AUD_PEQ_CASE1][8].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        // Normal & clear sound off
        case AUD_NVM_PEQ_GAIN_USER1_2_CS_OFF: // Wall Normal
            _arPEQTable[AUD_PEQ_CASE4][2].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_3_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE4][3].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_4_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE4][4].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_5_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE4][5].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_6_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE4][6].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_7_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE4][7].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER1_8_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE4][8].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_2_CS_OFF: // Table Normal
            _arPEQTable[AUD_PEQ_CASE5][2].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_3_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE5][3].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_4_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE5][4].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_5_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE5][5].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_6_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE5][6].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_7_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE5][7].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER2_8_CS_OFF:
            _arPEQTable[AUD_PEQ_CASE5][8].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        // Skype
        case AUD_NVM_PEQ_GAIN_USER3_2: // Wall Skype
            _arPEQTable[AUD_PEQ_CASE2][2].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER3_3:
            _arPEQTable[AUD_PEQ_CASE2][3].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER3_4:
            _arPEQTable[AUD_PEQ_CASE2][4].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER3_5:
            _arPEQTable[AUD_PEQ_CASE2][5].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER3_6:
            _arPEQTable[AUD_PEQ_CASE2][6].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER3_7:
            _arPEQTable[AUD_PEQ_CASE2][7].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER3_8:
            _arPEQTable[AUD_PEQ_CASE2][8].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER4_2: // Table Skype
            _arPEQTable[AUD_PEQ_CASE3][2].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER4_3:
            _arPEQTable[AUD_PEQ_CASE3][3].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER4_4:
            _arPEQTable[AUD_PEQ_CASE3][4].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER4_5:
            _arPEQTable[AUD_PEQ_CASE3][5].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER4_6:
            _arPEQTable[AUD_PEQ_CASE3][6].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER4_7:
            _arPEQTable[AUD_PEQ_CASE3][7].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_USER4_8:
            _arPEQTable[AUD_PEQ_CASE3][8].i2PeqGain = (INT16)((UINT8)u4Data - 24);
            break;
#ifdef CC_AUD_DIFFER_PEQ_GAIN_FOR_VOL
        case AUD_NVM_PEQ_GAIN_VOL1_7:
            _ai2PEQGainTable[0][0] = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_VOL2_7:
            _ai2PEQGainTable[0][1] = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_VOL3_7:
            _ai2PEQGainTable[0][2] = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_VOL4_7:
            _ai2PEQGainTable[0][3] = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_VOL1_8:
            _ai2PEQGainTable[1][0] = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_VOL2_8:
            _ai2PEQGainTable[1][1] = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_VOL3_8:
            _ai2PEQGainTable[1][2] = (INT16)((UINT8)u4Data - 24);
            break;
        case AUD_NVM_PEQ_GAIN_VOL4_8:
            _ai2PEQGainTable[1][3] = (INT16)((UINT8)u4Data - 24);
            break;
#endif
#endif
        default:
#ifdef CC_AUD_USE_NVM_AMP
            return ADAC_AmpNvmFunction(rNvmData.eDataType, u4Data);
#endif
            break;
    }

    return 0;
}

#ifdef CC_AUD_USE_NVM_AMP
UINT32 AUD_NVM_Init_Amp(void)
{
    UINT32 u4NvmLength;  //Customer NVM Data Length
    UINT32 u4NvmData = 0;
    UINT16  i,j;
    
    // Check if NVM is enabled
    if ( !(DRVCUST_OptGet(eAudioNvmEnable)) )
    {
        return 1;   //NVM is not ON.
    }

    if (!(_fgNvmInit))
    {
        return 1;   //NVM data have not been read from EEPROM.
    }

    // Get Customer Common Data Length.
    u4NvmLength = DRVCUST_OptGet(eAudioFlashCommLength);

    for (i=0; i < u4NvmLength; i++)
    {
        if (aeFlashCommData[i].eDataType == AUD_NVM_AMP_START)
        { // found amp start flag
            break;
        }
    }
    
    // Initialize Audio from Flash values
    for (i=((i >= u4NvmLength) ? 0 : (i+1)); ((i < u4NvmLength) && (aeFlashCommData[i].eDataType != AUD_NVM_AMP_END)); i++)
    {   // found amp end flag
        u4NvmData = 0;
        for (j=0; j < aeFlashCommData[i].u1Length; j++)
        {
            u4NvmData = ((auAudFlashCommBuffer[(aeFlashCommData[i].u2Offset)+j]  << (8*j)) | u4NvmData);
        }

        ADAC_AmpNvmFunction(aeFlashCommData[i].eDataType, u4NvmData);
    }

    return 0;
}
#endif

UINT32 AUD_NVM_Init_From_EEPROM(void)
{
    UINT32 u4NvmLength;  //Customer NVM Data Length
    UINT32 u4NvmData = 0;
    UINT16  i,j;

    // Check if NVM is enabled
    if ( !(DRVCUST_OptGet(eAudioNvmEnable)) )
    {
        return 1;   //NVM is not ON.
    }

    if (!(_fgNvmInit))
    {
        return 1;   //NVM data have not been read from EEPROM.
    }

    // Get Customer NVM Data Length.
    u4NvmLength = DRVCUST_OptGet(eAudioNvmLength);

    // Initialize Audio from NVM values
    for (i=0; i < u4NvmLength; i++)
    {
        u4NvmData = 0;
        for (j=0; j < aeNvmData[i].u1Length; j++)
        {
            u4NvmData =((auAudNvmBuffer[(aeNvmData[i].u2Offset)+j]  << (8*j)) | u4NvmData);
        }
        _AUD_NVM_Function(aeNvmData[i], u4NvmData);
    }
    // Get Customer Common Data Length.
    u4NvmLength = DRVCUST_OptGet(eAudioFlashCommLength);

    // Initialize Audio from Flash values
    for (i=0; i < u4NvmLength; i++)
    {
        u4NvmData = 0;
        for (j=0; j < aeFlashCommData[i].u1Length; j++)
        {
            u4NvmData = ((auAudFlashCommBuffer[(aeFlashCommData[i].u2Offset)+j]  << (8*j)) | u4NvmData);
        }

        _AUD_NVM_Function(aeFlashCommData[i], u4NvmData);
    }

    return 0;
}

UINT32 AUD_NVM_Init_To_EEPROM(void)
{
    UINT8  uNvmBuffer[AUD_NVM_LENGTH];    // Buffer read from EEPROM
    UINT32 u4NvmLength;                   // Customer NVM Data Length
    UINT16  i,j;

    // Check if NVM is enabled
    if ( !(DRVCUST_OptGet(eAudioNvmEnable)) )
    {
        return 1;   //NVM is not ON.
    }

    // Get Customer NVM Data Length.
    u4NvmLength = DRVCUST_OptGet(eAudioNvmLength);

    // Clean uNvmBuffer
    for (i=0; i < AUD_NVM_LENGTH-2; i++)
    {
        uNvmBuffer[i] = 0xFF;
    }
#ifdef CC_AUD_NVM_CHECK_VALID
    uNvmBuffer[i++] = AUD_NVM_FLAG;
    uNvmBuffer[i] = au1NvmCrc;
#else
    uNvmBuffer[i++] = 0xFF;
    uNvmBuffer[i] = 0xFF;
#endif

    // Write Default Value to NVM.
    for (i=0; i < u4NvmLength; i++)
    {
        for (j=0; j < aeNvmData[i].u1Length; j++)
        {
            uNvmBuffer[(aeNvmData[i].u2Offset + j)] = ( (aeNvmData[i].u4DefaultValue) >> (j*8) ) & 0xFF;
            LOG(9,"uNvmBuffer[%d] = 0x%x\n", (aeNvmData[i].u2Offset + j), uNvmBuffer[(aeNvmData[i].u2Offset + j)] );
        }
    }

    if (EEPROM_Write((UINT64)AUD_NVM_OFFSET, (UINT32)uNvmBuffer, AUD_NVM_LENGTH))
    {
        return 1;   //EEPROM Read Fail
    }

    return 0;
}

BOOL Aud_NVM_GetCountry(void)
{
#if 1
    return TRUE;
#else
    UINT8  u1AudProfile[3] = {0,0,0};
    UINT16 u2AudNVMProfileOffset[3] = {0x00D0,0x08E0,0x13C0};
    UINT8 uTempBuf;
    BOOL  fgLatam = TRUE;
    UINT8 i;

    //if ( !(DRVCUST_OptGet(eAudioNvmGetCountry)) )
    // by gallen at 20081107
    {
        return TRUE;
    }

    // LC09M InputMux Cust
    for (i=0; i<3; i++)
    {
        if (EEPROM_Read((UINT64)u2AudNVMProfileOffset[i], (UINT32)&uTempBuf, 1))
        {
            LOG(0,"### LC09M Get Country from NVM Fail!\n");
            return fgLatam;
        }
        u1AudProfile[i] = uTempBuf & 0x3;    // bit0~1
    }

    if (u1AudProfile[0] != u1AudProfile[1])
    {
        if (u1AudProfile[1] == u1AudProfile[2])
        {
            u1AudProfile[0] = u1AudProfile[1];
        }
    }
    LOG(0,"### LC09M Get Country =%d\n", u1AudProfile[0]);

    if (u1AudProfile[0] == 1)   // 1: AP, 2: Latam
    {
        fgLatam = FALSE;        // False means query specail input mux table
    }

    return fgLatam;
#endif
}


UINT32 AUD_NVM_Operation(AUD_NVM_OP_T uOP, UINT8* puNvmBuffer, UINT16 u2Offset)
{
    if ( !(DRVCUST_OptGet(eAudioNvmEnable)) )
    {
        return 1;   // NVM is not ON.
    }
    switch (uOP)
    {
        case AUD_NVM_READ :
            if (EEPROM_Read((UINT64)AUD_NVM_OFFSET, (UINT32)puNvmBuffer, AUD_NVM_LENGTH))
            {
                return 1;   //EEPROM Read Fail
            }
            break;

        case AUD_NVM_WRITE :
            if (u2Offset > (AUD_NVM_LENGTH))
            {
                return 1;
            }
            if (EEPROM_Write((UINT64)(AUD_NVM_OFFSET + u2Offset), (UINT32)puNvmBuffer, 1))
            {
                return 1;   //EEPROM Read Fail
            }
            break;

        case AUD_NVM_INIT :
            if (AUD_NVM_Init_To_EEPROM())
            {
                return 1;
            }
            break;

        case AUD_NVM_VOL_INIT :
#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
            if ((u4NvmVolTab[0] != u4NvmVolTab[VOL_TABLE_CHK_PNT-1]) &&
                (u4NvmVolTab[0] <= u4NvmVolTab[1]) &&
                (u4NvmVolTab[1] <= u4NvmVolTab[2]) &&
                (u4NvmVolTab[2] <= u4NvmVolTab[3]) &&
                (u4NvmVolTab[3] <= u4NvmVolTab[4]) &&
                (u4NvmVolTab[4] <= u4NvmVolTab[5]) &&
                (u4NvmVolTab[5] <= u4NvmVolTab[6]))
            {
                _AUD_DspSetVolumeTable(u4NvmVolTab);
            }

#ifdef CC_AUD_NEW_CV_TABLE
            if ((u4NvmHPVolTab[0] != u4NvmHPVolTab[VOL_TABLE_CHK_PNT-1]) &&
                (u4NvmHPVolTab[0] <= u4NvmHPVolTab[1]) &&
                (u4NvmHPVolTab[1] <= u4NvmHPVolTab[2]) &&
                (u4NvmHPVolTab[2] <= u4NvmHPVolTab[3]) &&
                (u4NvmHPVolTab[3] <= u4NvmHPVolTab[4]) &&
                (u4NvmHPVolTab[4] <= u4NvmHPVolTab[5]) &&
                (u4NvmHPVolTab[5] <= u4NvmHPVolTab[6]))
            {
                _AUD_DspSetStepCVTable(u4NvmHPVolTab);
#ifdef CC_AUD_NEW_VOL_BASE_LEVEL
                _u1NewVolBaseLevel = _AUD_DspGetVolumeLevelFromShm(u4NvmHPVolTab, VOL_SHM_0_DB);
#endif
#ifdef CC_AUD_SPIDF_LVL
                _u1AudSpdifLevel[0] = _AUD_DspGetVolumeLevelFromShm(u4NvmHPVolTab, _DbToShm(-(INT32)_u1AudSpdifLevel[0]));
                _u1AudSpdifLevel[1] = _AUD_DspGetVolumeLevelFromShm(u4NvmHPVolTab, _DbToShm(-(INT32)_u1AudSpdifLevel[1]));
                _u1AudSpdifLevel[2] = _AUD_DspGetVolumeLevelFromShm(u4NvmHPVolTab, _DbToShm(-(INT32)_u1AudSpdifLevel[2]));
#endif
            }
#endif
#else
            // L ch
            if ((u4NvmVolTab[AUD_NVM_L_VOL_TBL][0] != u4NvmVolTab[AUD_NVM_L_VOL_TBL][VOL_TABLE_CHK_PNT-1]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL][0] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL][1]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL][1] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL][2]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL][2] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL][3]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL][3] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL][4]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL][4] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL][5]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL][5] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL][6]))
            {
                _AUD_DspSetStepCVTable(AUD_NVM_L_VOL_TBL, u4NvmVolTab[AUD_NVM_L_VOL_TBL]);
            }
            // R ch
            if ((u4NvmVolTab[AUD_NVM_R_VOL_TBL][0] != u4NvmVolTab[AUD_NVM_R_VOL_TBL][VOL_TABLE_CHK_PNT-1]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL][0] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL][1]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL][1] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL][2]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL][2] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL][3]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL][3] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL][4]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL][4] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL][5]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL][5] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL][6]))
            {
                _AUD_DspSetStepCVTable(AUD_NVM_R_VOL_TBL, u4NvmVolTab[AUD_NVM_R_VOL_TBL]);
            }
            // L ch for skype
            if ((u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][0] != u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][VOL_TABLE_CHK_PNT-1]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][0] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][1]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][1] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][2]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][2] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][3]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][3] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][4]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][4] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][5]) &&
                (u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][5] <= u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE][6]))
            {
                _AUD_DspSetStepCVTable(AUD_NVM_L_VOL_TBL_SKYPE, u4NvmVolTab[AUD_NVM_L_VOL_TBL_SKYPE]);
            }
            // R ch for skype
            if ((u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][0] != u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][VOL_TABLE_CHK_PNT-1]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][0] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][1]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][1] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][2]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][2] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][3]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][3] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][4]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][4] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][5]) &&
                (u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][5] <= u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE][6]))
            {
                _AUD_DspSetStepCVTable(AUD_NVM_R_VOL_TBL_SKYPE, u4NvmVolTab[AUD_NVM_R_VOL_TBL_SKYPE]);
            }

#ifdef CC_AUD_NEW_CV_TABLE
            if ((u4NvmHPVolTab[0] != u4NvmHPVolTab[VOL_TABLE_CHK_PNT-1]) &&
                (u4NvmHPVolTab[0] <= u4NvmHPVolTab[1]) &&
                (u4NvmHPVolTab[1] <= u4NvmHPVolTab[2]) &&
                (u4NvmHPVolTab[2] <= u4NvmHPVolTab[3]) &&
                (u4NvmHPVolTab[3] <= u4NvmHPVolTab[4]) &&
                (u4NvmHPVolTab[4] <= u4NvmHPVolTab[5]) &&
                (u4NvmHPVolTab[5] <= u4NvmHPVolTab[6]))
            {
                _AUD_DspSetStepCVTable(AUD_NVM_COMMON_VOL_TBL, u4NvmHPVolTab);
#ifdef CC_AUD_NEW_VOL_BASE_LEVEL
                _u1NewVolBaseLevel = _AUD_DspGetVolumeLevelFromShm(u4NvmHPVolTab, VOL_SHM_0_DB);
#endif
#ifdef CC_AUD_SPIDF_LVL
                _u1AudSpdifLevel[0] = _AUD_DspGetVolumeLevelFromShm(u4NvmHPVolTab, _DbToShm(-(INT32)_u1AudSpdifLevel[0]));
                _u1AudSpdifLevel[1] = _AUD_DspGetVolumeLevelFromShm(u4NvmHPVolTab, _DbToShm(-(INT32)_u1AudSpdifLevel[1]));
                _u1AudSpdifLevel[2] = _AUD_DspGetVolumeLevelFromShm(u4NvmHPVolTab, _DbToShm(-(INT32)_u1AudSpdifLevel[2]));
#endif
            }
#endif
#endif

            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_HDMI] = _rNVMIuputSrcGain.i2NvmInputSrcHDMI;
            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_LINE_IN] = _rNVMIuputSrcGain.i2NvmInputSrcAV;
            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_BUF_AGT] = _rNVMIuputSrcGain.i2NvmInputSrcUSB;
            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_MULTI_MEDIA] = _rNVMIuputSrcGain.i2NvmInputSrcUSB;
            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_DIGITAL_TUNER] = _rNVMIuputSrcGain.i2NvmInputSrcDTV;
            _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_ANALOG_TUNER] = _rNVMIuputSrcGain.i2NvmInputSrcATV;
            _aai1ChlVolExtraGain[AUD_DEC_MAIN][AUD_CH_FRONT_LEFT] = _i4NvmExtraVolGain[AUD_CH_FRONT_LEFT];
            _aai1ChlVolExtraGain[AUD_DEC_MAIN][AUD_CH_FRONT_RIGHT] = _i4NvmExtraVolGain[AUD_CH_FRONT_RIGHT];
            _aai1ChlVolExtraGainNonLineIn[AUD_DEC_MAIN][AUD_CH_FRONT_LEFT] = _i4NvmExtraVolGain1[AUD_CH_FRONT_LEFT];
            _aai1ChlVolExtraGainNonLineIn[AUD_DEC_MAIN][AUD_CH_FRONT_RIGHT] = _i4NvmExtraVolGain1[AUD_CH_FRONT_RIGHT];
            #if 0
            //fix spdif output is 3dB lower than dali in atv
            _aai1ChlVolExtraGainNonLineIn[AUD_DEC_MAIN][AUD_CH_BYPASS_LEFT] = VOL_POS_3_DB;
            _aai1ChlVolExtraGainNonLineIn[AUD_DEC_MAIN][AUD_CH_BYPASS_RIGHT] = VOL_POS_3_DB;
            #endif
            break;

        case AUD_NVM_VS_INIT :
            DSP_Set_VSurr_Cfg(AUD_SUR_CLARITY, _rNVMSurrCfg[0].u4Clarity);
            DSP_Set_VSurr_Cfg(AUD_SUR_LRGAIN, _rNVMSurrCfg[0].u4LRGain);
            DSP_Set_VSurr_Cfg(AUD_SUR_XTALK, _rNVMSurrCfg[0].u4Xtalk);
            DSP_Set_VSurr_Cfg(AUD_SUR_OUTPUT_GAIN, _rNVMSurrCfg[0].u4OutputGain);
            DSP_Set_VSurr_Cfg(AUD_SUR_BASS_GAIN, _rNVMSurrCfg[0].u4BassGain);
            DSP_Set_VSurr_Cfg(AUD_SUR_FO, _rNVMSurrCfg[0].u4Fo);
            break;
#if 1 //20120801, wonder, to clean warning msg.
// /share/personal_space/wonder/2k13_phi_eu/release/vm_linux/chiling/driver/ko/mt5880/5880_driver/aud/audio/aud_dsp_cfg.c:14582:5: warning: enumeration value 'AUD_NVM_PROFILE_QRY' not handled in switch
// /share/personal_space/wonder/2k13_phi_eu/release/vm_linux/chiling/driver/ko/mt5880/5880_driver/aud/audio/aud_dsp_cfg.c:14582:5: warning: enumeration value 'AUD_NVM_PROFILE_OFFSET' not handled in switch

        default:
            break;
#endif
    }

    return 0;
}

UINT32 AUD_NVM_QRY_ATV_GAIN(AUD_NVM_ATV_QRY_T eAudNvmQryT, AUD_NVM_ATV_FMT_T eAudQryFmt)
{
    UINT32 u4RetGain = 0;
    switch (eAudNvmQryT)
    {
    case AUD_NVM_QRY_A2_GAIN:
        u4RetGain = eAudNVMAtvPreGain.u4A2_FineGain[eAudQryFmt];
        _u1A2FineVol = eAudNVMAtvPreGain.u1A2_FineGain[eAudQryFmt];
        break;
    case AUD_NVM_QRY_PAL_GAIN:
        u4RetGain = eAudNVMAtvPreGain.u4PAL_FineGain[eAudQryFmt];
        _u1PalFineVol = eAudNVMAtvPreGain.u1PAL_FineGain[eAudQryFmt];
        break;
    case AUD_NVM_QRY_AM_GAIN:
        u4RetGain = eAudNVMAtvPreGain.u4AM_FineGain[eAudQryFmt];
        _u1AMFineVol = eAudNVMAtvPreGain.u1AM_FineGain[eAudQryFmt];
        break;
    case AUD_NVM_QRY_NICAM_GAIN:
        u4RetGain = eAudNVMAtvPreGain.u4NICAM_FineGain[eAudQryFmt];
        _u1NicamFineVol = eAudNVMAtvPreGain.u1NICAM_FineGain[eAudQryFmt];
        break;
    }

    return u4RetGain;
}

#endif //#ifdef CC_AUD_USE_NVM
// =================== End of NVM and Profile Functions ====================================
//------------------------------------------------------------------------------------------
/** Aud_FlashAQParsing
 *  Parsing binary data from Flash
 *
 *  @param  eAQDataType         AQ data type
 *  @param  pData               pointer to binary data array
 *  @retval void
 */
//-------------------------------------------------------------------------------------------
#ifdef CC_AUD_USE_FLASH_AQ
void Aud_FlashAQSetValue(void)
{
    UINT8 i;

    for (i=0; i<AQ_TYPE_NUM; i++)
    {
        _fgAQ_Enable[i] = FALSE;
    }
}

BOOL Aud_FlashAQQuery(AUD_AQ_TYPE_T eAQDataType, VOID* *pData)
{
    BOOL fgRet = FALSE;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    UINT8 eConvertSrc;
    UINT8 eConvertSoundMode;

    AUD_DRVGetStreamFrom(AUD_DEC_MAIN, &eStreamFrom);
    VERIFY(eStreamFrom < AUD_STREAM_FROM_NUM);

    eConvertSrc = _au1AQSrc_Convert[eStreamFrom];
    VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );

    if (_au1MWSetSoundModeIdx < sizeof(_au1AQSmod_Convert))
    {
        eConvertSoundMode = _au1AQSmod_Convert[_au1MWSetSoundModeIdx];
    }
    else
    {
        eConvertSoundMode = 0;
    }

    switch (eAQDataType)
    {
    case AQ_TYPE_POST:
        if (_fgAQ_Enable[AQ_TYPE_POST])
        {
            *(UINT32*)pData = _au4AQ_PostScale[0];
            *(UINT32*)(pData+1) = _au4AQ_PostScale[1];
            fgRet = TRUE;
        }
        else
        {
            fgRet = FALSE;
        }
        break;
#ifdef CC_SONY_2K14_FLASH_AQ
    case AQ_TYPE_SPKHEIGHT_TABLE:
        if (_fgAQ_Enable[AQ_TYPE_SPKHEIGHT_TABLE])
        {
        
            *(UINT32*)pData = _au1AQ_SPH_TABLE[eConvertSrc][eConvertSoundMode][0];
            fgRet = TRUE;
        }
        else
        {
            fgRet = FALSE;
        }
        break;
        
    case AQ_TYPE_SPKHEIGHT_WALL:
        if (_fgAQ_Enable[AQ_TYPE_SPKHEIGHT_WALL])
        {
            
            *(UINT32*)pData = _au1AQ_SPH_WALL[eConvertSrc][eConvertSoundMode][0];
            fgRet = TRUE;
        }
        else
        {
            fgRet = FALSE;
        }
        break;
#else
    case AQ_TYPE_SPKHEIGHT:
        if (_fgAQ_Enable[AQ_TYPE_SPKHEIGHT])
        {

            *(UINT32*)pData = _au1AQ_SPH[eConvertSrc][eConvertSoundMode][0];
            fgRet = TRUE;
        }
        else
        {
            fgRet = FALSE;
        }
        break;
#endif
    case AQ_TYPE_LIMITER:
        if (_fgAQ_Enable[AQ_TYPE_LIMITER])
        {
            x_memcpy((VOID *)VIRTUAL(pData),&(_au1AQ_LIMITER[eConvertSrc][0]),7);
            //*(UINT32*)pData = _au4AQ_LIMITER[eConvertSrc][0];
            fgRet = TRUE;            
        }
        else
        {
            fgRet = FALSE;
        }        
        break;
    case AQ_TYPE_TYPE3_TBL:
        if (_fgAQ_Enable[AQ_TYPE_TYPE3_TBL] && (AUD_FlashAQ_ModelQueryFunc() > 0))
        {
            *(AUD_AQ_TYPE3_DATA_T*)pData = _rAQ_TYPE3_TBL;
            fgRet = TRUE;            
        }
        else
        {
            fgRet = FALSE;
        }        
        break;
    case AQ_TYPE_BACKLIGHT:
        if (_fgAQ_Enable[AQ_TYPE_BACKLIGHT])
        {
            x_memcpy((VOID *)VIRTUAL(pData),&(_au1AQ_BackLight[0]),AQ_BACKLIGHT_ITEMS);
            fgRet = TRUE;            
        }
        else
        {
            fgRet = FALSE;
        }        
        break;    
#ifdef CC_SONY_2K14_FLASH_AQ
    case AQ_TYPE_FIR_TABLE:
        if (_fgAQ_Enable[AQ_TYPE_FIR_TABLE])
        {
            x_memcpy((VOID *)VIRTUAL((UINT32)pData),&(_au1AQ_FIR_TABLE[0]),AQ_FIR_ITEMS*4);
            fgRet = TRUE;            
        }
        else
        {
            fgRet = FALSE;
        }        
        break;
                
    case AQ_TYPE_FIR_WALL:
        if (_fgAQ_Enable[AQ_TYPE_FIR_WALL])
        {
            x_memcpy((VOID *)VIRTUAL((UINT32)pData),&(_au1AQ_FIR_WALL[0]),AQ_FIR_ITEMS*4);
            fgRet = TRUE;            
        }
        else
        {
            fgRet = FALSE;
        }        
        break;
        
#else
    case AQ_TYPE_FIR:
        if (_fgAQ_Enable[AQ_TYPE_FIR])
        {
            x_memcpy((VOID *)VIRTUAL(pData),&(_au1AQ_FIR[0]),AQ_FIR_ITEMS*4);// memory copy as BYTE, FIR table is 32bits
            fgRet = TRUE;            
        }
        else
        {
            fgRet = FALSE;
        }        
        break;
#endif
    case AQ_TYPE_MIC_GAIN:
        if (_fgAQ_Enable[AQ_TYPE_MIC_GAIN])
        {
            if (_u4AQ_Mic_Gain > 0x20000)
            {
                _u4AQ_Mic_Gain = 0x20000;
            }
            *(UINT32 *)pData = _u4AQ_Mic_Gain;
            fgRet = TRUE;
        }
        else
        {
            fgRet = FALSE;
        }
        break;
    default:
        break;
    };

    return fgRet;
}

void Aud_FlashAQParsing(AUD_AQ_TYPE_T eAQDataType, UINT8* pData)
{
    UINT16 i;  //loop index
    UINT16 j; //loop index
    UINT16 u2Offset;
    UINT32 u4TmpValue;
    UINT8 *pTmpPtr;    

    //-- remove these after all data are applied to flow.
    UNUSED(_au1AQSrc_Convert);
    UNUSED(_au1AQLevel);
    UNUSED(_au1AQ_EQLevel);
    UNUSED(_au1AQ_DRC);
    UNUSED(_au1AQ_AVC);
    UNUSED(_au1AQ_LIMITER);
    UNUSED(_au4AQ_PostScale);
    UNUSED(_au1AQ_LO_Vol);
    UNUSED(_au4AQ_EXT_PEQ);
    UNUSED(_au4AQ_EXT_PEQ_LsRS);
    UNUSED(_au1AQLevel_LsRs);
    UNUSED(_au1AQ_DRC_LsRs);
    UNUSED(_au4AQ_PostScale_LsRs);
    UNUSED(_u4AQ_Mic_Gain);
#ifdef CC_SONY_2K14_FLASH_AQ
    UNUSED(_au1AQBassTrebleOffset_TABLE);
    UNUSED(_au1AQBassTrebleOffset_WALL);
    UNUSED(_au1AQBassManage_TABLE);
    UNUSED(_au1AQBassManage_WALL);
    UNUSED(_au1AQ_SPH_TABLE);
    UNUSED(_au1AQ_SPH_WALL);
    UNUSED(_au4AQ_MVS_TABLE);
    UNUSED(_au4AQ_MVS_WALL);
    UNUSED(_au1AQ_PEQ_TABLE);
    UNUSED(_au1AQ_PEQ_WALL);
    UNUSED(_au1AQ_VBass_TABLE);
    UNUSED(_au1AQ_VBass_WALL);
    UNUSED(_au1AQ_FIR_TABLE);
    UNUSED(_au1AQ_FIR_WALL);
    UNUSED(_au1AQ_MTKDRC_TABLE);
    UNUSED(_au1AQ_MTKDRC_WALL);
#else
    UNUSED(_au1AQBassTrebleOffset);
    UNUSED(_au1AQBassManage);
    UNUSED(_au1AQ_SPH);
    UNUSED(_au4AQ_MVS);
    UNUSED(_au1AQ_PEQ);
    UNUSED(_au1AQ_VBass);
    UNUSED(_au1AQ_FIR);
    UNUSED(_au1AQ_MTKDRC);
#endif

    switch (eAQDataType)
    {
        case AQ_TYPE_VERSION:
            //LOG(3,"AQ version is %s\n", pData);
            break;

        case AQ_TYPE_CH_VOLUME:
            u2Offset = 0;
            for (i=0; i<FLASH_AQ_COMM_SRC_NUM; i++)
            {
                for(j=0; j<FLASH_AQ_COMM_CH_NUM; j++)
                {
                    _au1AQChVolume[i][j] = pData[u2Offset];
                    u2Offset += 1;
                }
            }
            _fgAQ_Enable[AQ_TYPE_CH_VOLUME] = TRUE;
            break;

        case AQ_TYPE_SRC_VOLUME:
            for (i=0; i<=AUD_STREAM_FROM_MEMORY; i++)
            {
                _aai2InputSrcVol[AUD_DEC_MAIN][i] = (INT16)(pData[i] - 128);
            }
            // Table doesn't provide, so use the same value as MEMORY
            for (i=AUD_STREAM_FROM_BUF_AGT; i<=AUD_STREAM_FROM_MULTI_MEDIA; i++)
            {
                _aai2InputSrcVol[AUD_DEC_MAIN][i] = (INT16)(pData[AUD_STREAM_FROM_MEMORY] - 128);
            }
#if 0
            _i2InputSrcVolAtv = _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_ANALOG_TUNER];
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL 
            _i2InputSrcLoVolAtv = _aai2InputSrcLineOutVol[AUD_DEC_MAIN][AUD_STREAM_FROM_ANALOG_TUNER];
#endif
#endif
            _fgAQ_Enable[AQ_TYPE_SRC_VOLUME] = TRUE;
            break;

        case AQ_TYPE_VOL_TABLE:
            for (i=0; i<MAX_VOL_LEVEL+1; i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                _au4VOLUME[i]= u4TmpValue;
            }
            _fgAQ_Enable[AQ_TYPE_VOL_TABLE] = TRUE;
            break;
            
#ifdef CC_AUD_NEW_HP_VOL_TABLE
        case AQ_TYPE_HP_VOL_TABLE:
            for (i=0; i<MAX_VOL_LEVEL+1; i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                _au4HpVolume[i]= u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_HP_VOL_TABLE] = TRUE;
            break;
#endif            
#ifdef CC_AUD_NEW_CV_TABLE
        case AQ_TYPE_LO_VOL_TABLE:
            for (i=0; i<MAX_VOL_LEVEL+1; i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                _au4CHVOLUME[i]= u4TmpValue;
            }
            _fgAQ_Enable[AQ_TYPE_LO_VOL_TABLE] = TRUE;
            break;
#endif //CC_AUD_NEW_CV_TABLE

#ifdef CC_SONY_2K14_FLASH_AQ
        case AQ_TYPE_PEQ_TABLE:
            for (j=0; j<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_PEQ_ITEMS); j++)
            {
             *((UINT8*)(_au1AQ_PEQ_TABLE) + j) = (UINT8)pData[j];
            }
            _fgAQ_Enable[AQ_TYPE_PEQ_TABLE] = TRUE;
        break;
        case AQ_TYPE_PEQ_WALL:
            for (j=0; j<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_PEQ_ITEMS); j++)
            {
                *((UINT8*)(_au1AQ_PEQ_WALL) + j) = (UINT8)pData[j];
            }
             _fgAQ_Enable[AQ_TYPE_PEQ_WALL] = TRUE;
        break;
         
#else
        case AQ_TYPE_PEQ:
             for (j=0; j<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_PEQ_ITEMS); j++)
             {
                *((UINT8*)(_au1AQ_PEQ) + j) = (UINT8)pData[j];
             }
             _fgAQ_Enable[AQ_TYPE_PEQ] = TRUE;
        break;
#endif

         case AQ_TYPE_SP_VOL:
             for (i=0; i<(FLASH_AQ_SRC_NUM); i++)
             {
                 *((UINT8*)(_au1AQ_SP_Vol) + i) = (UINT8)pData[i];
             }
             _fgAQ_Enable[AQ_TYPE_SP_VOL] = TRUE;
         break;

         case AQ_TYPE_LO_VOL:
             for (i=0; i<(FLASH_AQ_SRC_NUM); i++)
             {
                 *((UINT8*)(_au1AQ_LO_Vol) + i) = (UINT8)pData[i];
             }
             _fgAQ_Enable[AQ_TYPE_LO_VOL] = TRUE;
            break;

        case AQ_TYPE_LEVEL:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_LEVEL_ITEMS); i++)
            {
                *((UINT8*)(_au1AQLevel) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_LEVEL] = TRUE;
            break;
#ifdef CC_SONY_2K14_FLASH_AQ
        case AQ_TYPE_BASS_TREBLE_TABLE:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_BASS_ITEMS); i++)
            {
                *((UINT8*)(_au1AQBassTrebleOffset_TABLE) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_BASS_TREBLE_TABLE] = TRUE;
            break;
        case AQ_TYPE_BASS_TREBLE_WALL:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_BASS_ITEMS); i++)
            {
                *((UINT8*)(_au1AQBassTrebleOffset_WALL) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_BASS_TREBLE_WALL] = TRUE;
            break;
#else
        case AQ_TYPE_BASS_TREBLE:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_BASS_ITEMS); i++)
            {
                *((UINT8*)(_au1AQBassTrebleOffset) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_BASS_TREBLE] = TRUE;
            break;
#endif

#ifdef CC_SONY_2K14_FLASH_AQ
        case AQ_TYPE_BASS_TREBLE_CUTOFF:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_BASS_ITEMS); i++)
            {
                *((UINT8*)(_au1AQBassTrebleCutoff) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_BASS_TREBLE_CUTOFF] = TRUE;
            break;
                    
        case AQ_TYPE_BASS_MANAGEMENT_TABLE:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_BASS_MANAGE_ITEMS); i++)
            {
                *((UINT8*)(_au1AQBassManage_TABLE) + i) = (UINT8)pData[i];
            }
#ifdef CC_AUD_AQ_WITH_BASSMANAGEMENT            
            _fgAQ_Enable[AQ_TYPE_BASS_MANAGEMENT_TABLE] = TRUE;
#else
            _fgAQ_Enable[AQ_TYPE_BASS_MANAGEMENT_TABLE] = FALSE;
#endif            
            break;
        
        case AQ_TYPE_BASS_MANAGEMENT_WALL:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_BASS_MANAGE_ITEMS); i++)
            {
                *((UINT8*)(_au1AQBassManage_WALL) + i) = (UINT8)pData[i];
            }
#ifdef CC_AUD_AQ_WITH_BASSMANAGEMENT            
            _fgAQ_Enable[AQ_TYPE_BASS_MANAGEMENT_WALL] = TRUE;
#else
            _fgAQ_Enable[AQ_TYPE_BASS_MANAGEMENT_WALL] = FALSE;
#endif            
            break;
        
#else
        case AQ_TYPE_BASS_MANAGEMENT:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_BASS_MANAGE_ITEMS); i++)
            {
                *((UINT8*)(_au1AQBassManage) + i) = (UINT8)pData[i];
            }
#ifdef CC_AUD_AQ_WITH_BASSMANAGEMENT
            _fgAQ_Enable[AQ_TYPE_BASS_MANAGEMENT] = TRUE;
#else
            _fgAQ_Enable[AQ_TYPE_BASS_MANAGEMENT] = FALSE;
#endif
            break;
#endif

        case AQ_TYPE_EQ:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_EQ_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_EQLevel) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_EQ] = TRUE;
            break;

        case AQ_TYPE_DRC:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_DRC_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_DRC) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_DRC] = TRUE;
            break;

        case AQ_TYPE_AVC:
#if 1///def CC_SONY_2K13_FLASH_AQ
            for (i=0; i<(AQ_AVC_ITEMS); i++)
            {
                for(j=0; j<(FLASH_AQ_SRC_NUM); j++)
                {
                    *((UINT8*)(_au1AQ_AVC) + i + j*AQ_AVC_ITEMS) = (UINT8)pData[i];
                }
            }
#else
            for (i=0; i<(FLASH_AQ_SRC_NUM * AQ_AVC_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_AVC) + i) = (UINT8)pData[i];
            }
#endif
#ifdef CC_AUD_NEW_AQ_PARSER
            for(i = 0; i < AUD_STREAM_FROM_NUM ; i++ )
            {
                j = _au1AQSrc_Convert[i];
                VERIFY( j < FLASH_AQ_SRC_NUM );
                _ai2AvcTargetLevel[i] = (INT16)_au1AQ_AVC[j][1];
                _ai2AvcGainUp[i] = (INT16)_au1AQ_AVC[j][2];
                _ai2AvcAdjRate[i]= (INT16)(_au1AQ_AVC[j][3]<<8 + _au1AQ_AVC[j][4]);
            }
#endif //CC_AUD_NEW_AQ_PARSER
            _fgAQ_Enable[AQ_TYPE_AVC] = TRUE;
            break;

        case AQ_TYPE_LIMITER:
            j = 0;
#if 1///def CC_SONY_2K13_FLASH_AQ
            for (i=0; i<(AQ_LIMITER_ITEMS); i++)
            {               
                for(j=0; j<(FLASH_AQ_SRC_NUM); j++)
                {
                    *((UINT8*)(_au1AQ_LIMITER) + i + j*(AQ_LIMITER_ITEMS)) = (UINT8)pData[i]; //There are 28 items in Dram from 48 items in Flash.
                }
            }
#else
            for (i=0; i<(FLASH_AQ_SRC_NUM * AQ_LIMITER_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_LIMITER) + i) = (UINT8)pData[j]; //There are 28 items in Dram from 48 items in Flash.
                if (((i+1)%7) == 0) //There are 5 reserverd bytes in Table, for every 7 items.
                {
                    j = j+5+1;
                }
                else
                {
                    j++;
                }
            }
#endif
            _fgAQ_Enable[AQ_TYPE_LIMITER] = TRUE;
            break;

#ifdef CC_SONY_2K14_FLASH_AQ
        case AQ_TYPE_SPKHEIGHT_TABLE:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_SPH_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_SPH_TABLE) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_SPKHEIGHT_TABLE] = TRUE;
            break;
                    
        case AQ_TYPE_SPKHEIGHT_WALL:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_SPH_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_SPH_WALL) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_SPKHEIGHT_WALL] = TRUE;
            break;  
                    
        case AQ_TYPE_MVS_TABLE:
            for (i=0; i<(FLASH_AQ_SCENE_NUM * AQ_MVS_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                *((UINT32*)(_au4AQ_MVS_TABLE) + i) = u4TmpValue;
            }
            _fgAQ_Enable[AQ_TYPE_MVS_TABLE] = TRUE;
            break;
                    
        case AQ_TYPE_MVS_WALL:
            for (i=0; i<(FLASH_AQ_SCENE_NUM * AQ_MVS_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                *((UINT32*)(_au4AQ_MVS_WALL) + i) = u4TmpValue;
            }
            _fgAQ_Enable[AQ_TYPE_MVS_WALL] = TRUE;
            break;
#else
        case AQ_TYPE_SPKHEIGHT:
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_SPH_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_SPH) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_SPKHEIGHT] = TRUE;
            break;
                
        case AQ_TYPE_MVS:
            for (i=0; i<(FLASH_AQ_SCENE_NUM * AQ_MVS_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                *((UINT32*)(_au4AQ_MVS) + i) = u4TmpValue;
            }
            _fgAQ_Enable[AQ_TYPE_MVS] = TRUE;
            break;
#endif

        case AQ_TYPE_POST:
            for (i=0; i<AQ_POSTSCL_ITEMS; i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                *((UINT32*)(_au4AQ_PostScale) + i) = u4TmpValue;
            }
            _fgAQ_Enable[AQ_TYPE_POST] = TRUE;
            break;

        case AQ_TYPE_EXT_PEQ:
            for (i=0; i<(FLASH_AQ_SRC_NUM*FLASH_AQ_SM_NUM*AQ_AMP_FS_NUM*AQ_EXT_PEQ_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                *((UINT32*)(_au4AQ_EXT_PEQ) + i) = u4TmpValue;
            }
            _fgAQ_Enable[AQ_TYPE_EXT_PEQ] = TRUE;
            break;
        case AQ_TYPE_EXT_PEQ_LSRS:
            for (i=0; i<(FLASH_AQ_SRC_NUM*FLASH_AQ_SM_NUM*AQ_AMP_FS_NUM*AQ_EXT_PEQ_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);                             
                *((UINT32*)(_au4AQ_EXT_PEQ_LsRS) + i) = u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_EXT_PEQ_LSRS] = TRUE;
            break;

        case AQ_TYPE_EXT_PEQ_CSW:
            for (i=0; i<(FLASH_AQ_SRC_NUM*FLASH_AQ_SM_NUM*AQ_AMP_FS_NUM*AQ_EXT_PEQ_CSW_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);                             
                *((UINT32*)(_au4AQ_EXT_PEQ_CSw) + i) = u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_EXT_PEQ_CSW] = TRUE;
            break;

        case AQ_TYPE_TYPE3_TBL:
            // Level for C/Sw
            pTmpPtr = pData;
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_LEVEL_ITEMS); i++)
            {
                *((UINT8*)(_au1AQLevel_CSw) + i) = (UINT8)pTmpPtr[i];
            }            
            // DRC for C/Sw
            pTmpPtr = pTmpPtr + i;
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_DRC_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_DRC_CSw) + i) = (UINT8)pTmpPtr[i];
            }
            // Post Scale for C/Sw
            pTmpPtr = pTmpPtr + i; //every items contains 3 bytes for post scale
            for (i=0; i<AQ_POSTSCL_ITEMS; i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pTmpPtr[j]<<16) + (UINT32)(pTmpPtr[j+1]<<8) + (UINT32)(pTmpPtr[j+2]);
                *((UINT32*)(_au4AQ_PostScale_CSw) + i) = u4TmpValue;
            }
            //bit 12: C  Channel large(1)/small(0)
            //bit 13: L  Channel large(1)/small(0)
            //bit 14: R  Channel large(1)/small(0)
            //bit 15: Ls Channel large(1)/small(0)
            //bit 16: Rs Channel large(1)/small(0)
            //bit 19: 9          large(1)/small(0)
            //bit 20: 10         large(1)/small(0)              
            pTmpPtr = pTmpPtr + i*3;  //every items contains 3 bytes for post scale
            //Set 9/10/Ls/Rs to Large. Avoid some downmix/bypass data in 9/10/Ls/Rs going to Bmanage.
            u4TmpValue = ((pTmpPtr[0]<<13) | (pTmpPtr[1]<<12) | (pTmpPtr[2]<<15) | 0x198030); 
            _rAQ_TYPE3_TBL.u4SpkCfg = u4TmpValue;
            if (pTmpPtr[3] > 16)
            {
                _rAQ_TYPE3_TBL.u4Cutoff = 50;
            }
            else if (pTmpPtr[3] == 16)
            {
                _rAQ_TYPE3_TBL.u4Cutoff = 300;
            }
            else
            {
                _rAQ_TYPE3_TBL.u4Cutoff = 50 + pTmpPtr[3]*10; // 0~15 means 50 ~ 200.
            }
            _rAQ_TYPE3_TBL.u1Panorama = pTmpPtr[4];
            _rAQ_TYPE3_TBL.u1Dimesion = pTmpPtr[5];
            _rAQ_TYPE3_TBL.u1CWidth = pTmpPtr[6];            
            _fgAQ_Enable[AQ_TYPE_TYPE3_TBL] = TRUE;            
            break;
#ifdef CC_SONY_2K14_FLASH_AQ
        case AQ_TYPE_VBASS_TABLE:
            for (i=0; i<(FLASH_AQ_SRC_NUM*FLASH_AQ_SM_NUM*AQ_VBASS_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_VBass_TABLE) + i) = pData[i];
            }            
            _fgAQ_Enable[AQ_TYPE_VBASS_TABLE] = TRUE;
            break;
        case AQ_TYPE_VBASS_WALL:
            for (i=0; i<(FLASH_AQ_SRC_NUM*FLASH_AQ_SM_NUM*AQ_VBASS_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_VBass_WALL) + i) = pData[i];
            }            
            _fgAQ_Enable[AQ_TYPE_VBASS_WALL] = TRUE;
            break;        
        case AQ_TYPE_FIR_TABLE:
            for (i=0; i<(AQ_FIR_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);                             
                *((UINT32*)(_au1AQ_FIR_TABLE) + i) = u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_FIR_TABLE] = TRUE;
            break;
        case AQ_TYPE_FIR_WALL:
            for (i=0; i<(AQ_FIR_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);                             
                *((UINT32*)(_au1AQ_FIR_WALL) + i) = u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_FIR_WALL] = TRUE;
            break;
#else
        case AQ_TYPE_VBASS:
            for (i=0; i<(FLASH_AQ_SRC_NUM*FLASH_AQ_SM_NUM*AQ_VBASS_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_VBass) + i) = pData[i];
            }            
            _fgAQ_Enable[AQ_TYPE_VBASS] = TRUE;
            break;
        
        case AQ_TYPE_FIR:
            for (i=0; i<(AQ_FIR_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);                             
                *((UINT32*)(_au1AQ_FIR) + i) = u4TmpValue;
            }            
            _fgAQ_Enable[AQ_TYPE_FIR] = TRUE;
            break;
#endif

        case AQ_TYPE_BACKLIGHT:
            for (i=0; i<(AQ_BACKLIGHT_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_BackLight) + i) = pData[i];
            }    
            _fgAQ_Enable[AQ_TYPE_BACKLIGHT] = TRUE;
            break;
        case AQ_TYPE_MIC_GAIN:
            u4TmpValue = (UINT32)(pData[0]<<16) + (UINT32)(pData[1]<<8) + (UINT32)(pData[2]);
            _u4AQ_Mic_Gain = u4TmpValue;
            _fgAQ_Enable[AQ_TYPE_MIC_GAIN] = TRUE;
            break;            
#ifdef CC_AUD_FY12_LOUDNESS            
#ifdef CC_SONY_2K14_FLASH_AQ
        case AQ_TYPE_LOUDNESS_TABLE:
            for (i=0; i<(FLASH_AQ_SM_NUM * AQ_LOUDNESS_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_LOUDNESS_TABLE) + i) = (UINT8)pData[i];
            }
            for(i=0; i< FLASH_AQ_SM_NUM ; i++)
            {
                for (j=0; j<20; j++)    ///exten from 12 to 20.
                {
                    _ai1BassAttnTable_TABLE[i][j] = (INT8)_au1AQ_LOUDNESS_TABLE[i][j+1];
                    _ai1TrebleAttnTable_TABLE[i][j] = (INT8)_au1AQ_LOUDNESS_TABLE[i][j+1+20];
                            
                }
                _u1LoudnessFlag_TABLE[i]= _au1AQ_LOUDNESS_TABLE[i][0];  /// Loudness On/Off
            }

            _fgAQ_Enable[AQ_TYPE_LOUDNESS_TABLE] = TRUE;
        break;    
        
        case AQ_TYPE_LOUDNESS_WALL:
            for (i=0; i<(FLASH_AQ_SM_NUM * AQ_LOUDNESS_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_LOUDNESS_WALL) + i) = (UINT8)pData[i];
            }
            for(i=0; i< FLASH_AQ_SM_NUM ; i++)
            {
                for (j=0; j<20; j++)    ///exten from 12 to 20.
                {
                    _ai1BassAttnTable_WALL[i][j] = (INT8)_au1AQ_LOUDNESS_WALL[i][j+1];
                    _ai1TrebleAttnTable_WALL[i][j] = (INT8)_au1AQ_LOUDNESS_WALL[i][j+1+20];
                            
                }
                _u1LoudnessFlag_WALL[i]= _au1AQ_LOUDNESS_WALL[i][0];    /// Loudness On/Off
            }

            _fgAQ_Enable[AQ_TYPE_LOUDNESS_WALL] = TRUE;
        break;    


#else
        case AQ_TYPE_LOUDNESS:
            #ifdef CC_AUD_LOUDNESS_FROM_SOURCE_SOUNDMODE
            for (i=0; i<(FLASH_AQ_SRC_NUM * FLASH_AQ_SM_NUM * AQ_LOUDNESS_ITEMS); i++)
            #else
            for (i=0; i<AQ_LOUDNESS_ITEMS; i++)
            #endif
            {
                *((UINT8*)(_au1AQ_LOUDNESS) + i) = (UINT8)pData[i];
            }
            #ifdef CC_AUD_LOUDNESS_FROM_SOURCE_SOUNDMODE
            UINT16 k;  //loop index
            for (i=0; i< FLASH_AQ_SRC_NUM ; i++)
            {
                for(j=0; j< FLASH_AQ_SM_NUM ; j++)
                {
                    for(k=0; k<10; k++)
                    {              
                        _ai1BassAttnTable[i][j][k] = (INT8)_au1AQ_LOUDNESS[i][j][k];
                        _ai1TrebleAttnTable[i][j][k] = (INT8)_au1AQ_LOUDNESS[i][j][k+10];                        
                    }
                }
            }
            #endif
            _u1LoudnessFlag = _au1AQ_LOUDNESS[0];   /// Loudness On/Off
            for (i=0; i<12; i++)    ///exten from 10 to 12.
            {
                _ai1BassAttnTable[i] = (INT8)_au1AQ_LOUDNESS[i+1];
                _ai1TrebleAttnTable[i] = (INT8)_au1AQ_LOUDNESS[i+1+12];
                        
            }
            _fgAQ_Enable[AQ_TYPE_LOUDNESS] = TRUE;
            break;     
#endif
#endif

#ifdef CC_SONY_2K14_FLASH_AQ
        case AQ_TYPE_MTKDRC_TABLE:
            for (i=0; i<(FLASH_AQ_SM_NUM * AQ_MTK_DRC_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_MTKDRC_TABLE) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_MTKDRC_TABLE] = TRUE;
            break;         

        case AQ_TYPE_MTKDRC_WALL:
            for (i=0; i<(FLASH_AQ_SM_NUM * AQ_MTK_DRC_ITEMS); i++)
            {
                *((UINT8*)(_au1AQ_MTKDRC_WALL) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_MTKDRC_WALL] = TRUE;
            break;  
#else
        case AQ_TYPE_MTKDRC:
            for (i=0; i<AQ_MTK_DRC_ITEMS; i++)
            {
                *((UINT8*)(_au1AQ_MTKDRC) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_MTKDRC] = TRUE;
            break; 
#endif

#ifdef CC_AUD_SUPPORT_STC_COMPRESSOR
        case AQ_TYPE_STC381COEFF:
            for (i=0; i<AQ_STC381Coeff_ITEMS; i++)
            {
                *((UINT8*)(_au1AQ_381STCCoeff) + i) = (UINT8)pData[i];
            }
            _fgAQ_Enable[AQ_TYPE_STC381COEFF] = TRUE;
            break;
        case AQ_TYPE_STC381FILTER:
            for (i=0; i<(AQ_AMP_FS_NUM*AQ_STC_FILTER_ITEMS); i++)
            {
                j = i*3;
                u4TmpValue = (UINT32)(pData[j]<<16) + (UINT32)(pData[j+1]<<8) + (UINT32)(pData[j+2]);
                *((UINT32*)(_au4AQ_EXT_381STCFilter) + i) = u4TmpValue;
            }
            _fgAQ_Enable[AQ_TYPE_STC381FILTER] = TRUE;
            break;            
#endif            
        default:
            break;
    }
}
#ifdef CC_AUD_INDIA_GAIN_SUPPORT 
BOOL fgIndiaGain=FALSE;
void Aud_DspSetIndiaSpeakerGain(AUD_DEC_OVM_MODE_T eMode)
{
    AUD_APP_COUNTRY_SEL u4Country = AUD_AP_COUNTRY_UNKNOWN;
    
    UNUSED(Aud_DspSetIndiaSpeakerGain);
    //add India check!!!
    DRVCUST_InitQuery(eAPPCountryGetValue,(UINT32 *)(void *)&u4Country);

    if ((u4Country == AUD_AP_COUNTRY_INDIA) && (eMode == AUD_DEC_OVM_HIGH)) 
    {
        LOG(5, "Country is India for Audio filter is High\n");
        if (fgIndiaGain == FALSE)
        { 
            _au1AQChVolume[1][AUD_CH_FRONT_LEFT] = _au1AQChVolume[1][AUD_CH_FRONT_LEFT] + (INDIA_GAIN_SP_DB*2);
            _au1AQChVolume[1][AUD_CH_FRONT_RIGHT] = _au1AQChVolume[1][AUD_CH_FRONT_RIGHT] + (INDIA_GAIN_SP_DB*2);
            _au1AQChVolume[1][AUD_CH_REAR_LEFT] = _au1AQChVolume[1][AUD_CH_REAR_LEFT] + (INDIA_GAIN_LO_DB*2);
            _au1AQChVolume[1][AUD_CH_REAR_RIGHT] = _au1AQChVolume[1][AUD_CH_REAR_RIGHT] + (INDIA_GAIN_LO_DB*2);
            _au1AQChVolume[1][AUD_CH_DMX_LEFT] = _au1AQChVolume[1][AUD_CH_DMX_LEFT] + (INDIA_GAIN_HP_DB*2);
            _au1AQChVolume[1][AUD_CH_DMX_RIGHT] = _au1AQChVolume[1][AUD_CH_DMX_RIGHT] + (INDIA_GAIN_HP_DB*2);
            
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_FRONT_LEFT, _au1AQChVolume[1][AUD_CH_FRONT_LEFT] - 128);
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_FRONT_RIGHT, _au1AQChVolume[1][AUD_CH_FRONT_RIGHT] - 128);
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_REAR_LEFT, _au1AQChVolume[1][AUD_CH_REAR_LEFT] - 128);
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_REAR_RIGHT, _au1AQChVolume[1][AUD_CH_REAR_RIGHT] - 128);
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_DMX_LEFT, _au1AQChVolume[1][AUD_CH_DMX_LEFT] - 128);
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_DMX_RIGHT, _au1AQChVolume[1][AUD_CH_DMX_RIGHT] - 128);
            fgIndiaGain = TRUE;
        } 
    }
    else
    {
        LOG(5, "Country isn't India or Audio filter is Low/Off\n"); 
        if (fgIndiaGain == TRUE)
        {
            _au1AQChVolume[1][AUD_CH_FRONT_LEFT] = _au1AQChVolume[1][AUD_CH_FRONT_LEFT] - (INDIA_GAIN_SP_DB*2);
            _au1AQChVolume[1][AUD_CH_FRONT_RIGHT] = _au1AQChVolume[1][AUD_CH_FRONT_RIGHT] - (INDIA_GAIN_SP_DB*2);
            _au1AQChVolume[1][AUD_CH_REAR_LEFT] = _au1AQChVolume[1][AUD_CH_REAR_LEFT] - (INDIA_GAIN_LO_DB*2);
            _au1AQChVolume[1][AUD_CH_REAR_RIGHT] = _au1AQChVolume[1][AUD_CH_REAR_RIGHT] - (INDIA_GAIN_LO_DB*2);
            _au1AQChVolume[1][AUD_CH_DMX_LEFT] = _au1AQChVolume[1][AUD_CH_DMX_LEFT] - (INDIA_GAIN_HP_DB*2);
            _au1AQChVolume[1][AUD_CH_DMX_RIGHT] = _au1AQChVolume[1][AUD_CH_DMX_RIGHT] - (INDIA_GAIN_HP_DB*2);
            
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_FRONT_LEFT, _au1AQChVolume[1][AUD_CH_FRONT_LEFT] - 128);
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_FRONT_RIGHT, _au1AQChVolume[1][AUD_CH_FRONT_RIGHT] - 128);
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_REAR_LEFT, _au1AQChVolume[1][AUD_CH_REAR_LEFT] - 128);
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_REAR_RIGHT, _au1AQChVolume[1][AUD_CH_REAR_RIGHT] - 128);
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_DMX_LEFT, _au1AQChVolume[1][AUD_CH_DMX_LEFT] - 128);
            AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_DMX_RIGHT, _au1AQChVolume[1][AUD_CH_DMX_RIGHT] - 128);
            fgIndiaGain = FALSE;            
        } 
    }
}
#else
void Aud_DspSetIndiaSpeakerGain(AUD_DEC_OVM_MODE_T eMode)
{
#if 0
    AUD_APP_COUNTRY_SEL u4Country = AUD_AP_COUNTRY_UNKNOWN;
    INT16 i2SpeakerOffset = 9;
    UNUSED(Aud_DspSetIndiaSpeakerGain);
    //add India check!!!
    DRVCUST_InitQuery(eAPPCountryGetValue,(UINT32 *)(void *)&u4Country);

    if ((u4Country == AUD_AP_COUNTRY_INDIA) && (eMode == AUD_DEC_OVM_HIGH)) 
    {
        _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_ANALOG_TUNER] = _i2InputSrcVolAtv + i2SpeakerOffset;
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
        INT16 i2LineOutOffset = 9;
        _aai2InputSrcLineOutVol[AUD_DEC_MAIN][AUD_STREAM_FROM_ANALOG_TUNER]= _i2InputSrcLoVolAtv + i2LineOutOffset;
#endif
        AUD_DspSetFmRadioFineVolume(2); 
    }
    else
    {
        _aai2InputSrcVol[AUD_DEC_MAIN][AUD_STREAM_FROM_ANALOG_TUNER] = _i2InputSrcVolAtv;
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
        _aai2InputSrcLineOutVol[AUD_DEC_MAIN][AUD_STREAM_FROM_ANALOG_TUNER] = _i2InputSrcLoVolAtv;
#endif
        AUD_DspSetFmRadioFineVolume(4);        
    }
#endif
}
#endif

void _AUD_FlashAQPeriphSMChange(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1ModeCase)
{
    UINT8 eConvertSrc,eConvertFs;
    UINT8 i;
    AUD_AQ_AMP_DRC_T eDrcData = {0};
    AUD_AQ_AMP_STC_COEFF_T rAQAmpStcCoeff;
    static AUD_DEC_STREAM_FROM_T eOriStreamFrom = AUD_STREAM_FROM_OTHERS;
    static UINT8 u1OriModeCase = 0xFF;
    UINT32 u4AQPostScale[2];

    UNUSED(u4AQPostScale);
    UNUSED(i);
    UNUSED(eConvertSrc);
    UNUSED(eDrcData);
    UNUSED(rAQAmpStcCoeff);
    UNUSED(eConvertFs);

    //Skip if mode and source is the same.
    if ((eOriStreamFrom == eStreamFrom) && (u1OriModeCase == u1ModeCase) && (!_fgFlashAQPeriphRecover))
    {
        return;
    }
    else
    {
        eOriStreamFrom = eStreamFrom;
        u1OriModeCase = u1ModeCase;
    }

    eConvertSrc = _au1AQSrc_Convert[eStreamFrom];
    VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );    
    VERIFY( u1ModeCase < FLASH_AQ_SM_NUM );    //Fix Klocwork warning

    // Channel Level
    if (_fgAQ_Enable[AQ_TYPE_LEVEL])
    {
        for (i=0; i<=2; i++)
        {
            if (_IsUseSTA381())
            {
                ADAC_STA381_SetChannelVol(AUD_AMP_ID_LR,i,_au1AQLevel[eConvertSrc][u1ModeCase][i]);
            }
             else if (_IsUseDigiAmpTAS5717())
            {    
                _TAS5717_SetChannelVol(AUD_AMP_ID_LR,i,((UINT16)_au1AQLevel[eConvertSrc][u1ModeCase][i])&0xFF);
                                                     // AQ excel only has 1 byte for 5717L channel volume control
            }
         else if (_IsUseApogee())
            {
                ADAC_DigiAmp_SetChannelVol(AUD_AMP_ID_LR,i,_au1AQLevel[eConvertSrc][u1ModeCase][i]);
            }
        }
    }

    // Dual Band DRC.
    if (_fgAQ_Enable[AQ_TYPE_DRC])
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)&eDrcData), &(_au1AQ_DRC[eConvertSrc][u1ModeCase]), sizeof(AUD_AQ_AMP_DRC_T));
        if (_IsUseSTA381())
        {
            ADAC_STA381_SetDualDRC(AUD_AMP_ID_LR, eDrcData);
        }
        else if (_IsUseDigiAmpTAS5717())
        {
            ; //to do
        } 
       else if (_IsUseApogee())
        {
            ADAC_DigiAmp_SetDualDRC(AUD_AMP_ID_LR, eDrcData);
        }
    }
    // Post Scale    
    if (Aud_FlashAQQuery(AQ_TYPE_POST, (void*)&u4AQPostScale))
    {
        if (_IsUseSTA381())
        {
            ADAC_STA381_SetPostScale(AUD_AMP_ID_LR, u4AQPostScale[0],u4AQPostScale[1]);
        }
        else if (_IsUseDigiAmpTAS5717())
        {
            _TAS5717_SetPostScale(AUD_AMP_ID_LR, u4AQPostScale[0]);
            _TAS5717_SetPreScale(AUD_AMP_ID_LR, u4AQPostScale[1]);
        }   
        else if (_IsUseApogee())
        {
        ADAC_DigiAmp_SetPostScale(AUD_AMP_ID_LR, u4AQPostScale[0],u4AQPostScale[1]);
        }   
    }   

#ifdef CC_AUD_SUPPORT_STC_COMPRESSOR        
    //STC Compressor coeff(Note that this is STA381BWS specific)
    if (_fgAQ_Enable[AQ_TYPE_STC381COEFF])
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)&rAQAmpStcCoeff), &(_au1AQ_381STCCoeff), sizeof(AUD_AQ_AMP_STC_COEFF_T));
        if (_IsUseSTA381())
        {
             ADAC_STA381_SetSTCCoeff(AUD_AMP_ID_LR, &rAQAmpStcCoeff);
        }
    }
#endif
    // Setting for TypeIII
    if (_fgAQ_Enable[AQ_TYPE_TYPE3_TBL] && (AUD_FlashAQ_ModelQueryFunc() > 0))
    {
        //Level
        for (i=0; i<=2; i++) //there are 3 items in Level item.
        {
            // No Ls/Rs
            //ADAC_DigiAmp_SetChannelVol(AUD_AMP_ID_LsRs,i,_au1AQLevel_LsRs[eConvertSrc][u1ModeCase][i]);
            if (_IsUseSTA381())
            {
                ADAC_STA381_SetChannelVol(AUD_AMP_ID_CSw,i,_au1AQLevel_CSw[eConvertSrc][u1ModeCase][i]);    
            }
            else if (_IsUseDigiAmpTAS5717())
            {
                ;//to do
            }   
         else if (_IsUseApogee())
            {            
                ADAC_DigiAmp_SetChannelVol(AUD_AMP_ID_CSw,i,_au1AQLevel_CSw[eConvertSrc][u1ModeCase][i]);            
            }      
        }      
        //DRC
        // No Ls/Rs
        //x_memcpy((VOID *)VIRTUAL(&eDrcData), &(_au1AQ_DRC_LsRs[eConvertSrc][u1ModeCase]), sizeof(AUD_AQ_AMP_DRC_T));
        //ADAC_DigiAmp_SetDualDRC(AUD_AMP_ID_LsRs, eDrcData);
        x_memcpy((VOID *)VIRTUAL((UINT32)&eDrcData), &(_au1AQ_DRC_CSw[eConvertSrc][u1ModeCase]), sizeof(AUD_AQ_AMP_DRC_T));
        // Special request from Yugi san. When BassBooster is OFF, reduce gain of Subwoofer.
        _gu1SubWooferGain = eDrcData.u1LfeGain;
        if (eDrcData.u1LfeGain <= (0xFF-_gu1SubWooferOffset))
        {
            eDrcData.u1LfeGain = eDrcData.u1LfeGain + _gu1SubWooferOffset;
        }        
        if (_IsUseSTA381())
        {
            ADAC_STA381_SetDualDRC(AUD_AMP_ID_CSw, eDrcData);
         //Post scale
            // No Ls/Rs
            //u4AQPostScale[0] = _au4AQ_PostScale_LsRs[0];
            //u4AQPostScale[1] = _au4AQ_PostScale_LsRs[1];
            //ADAC_DigiAmp_SetPostScale(AUD_AMP_ID_LsRs, u4AQPostScale[0],u4AQPostScale[1]);
            u4AQPostScale[0] = _au4AQ_PostScale_CSw[0];
            u4AQPostScale[1] = _au4AQ_PostScale_CSw[1];
            ADAC_STA381_SetPostScale(AUD_AMP_ID_CSw, u4AQPostScale[0],u4AQPostScale[1]);            
        }
     else if (_IsUseApogee())
        {       
            ADAC_DigiAmp_SetDualDRC(AUD_AMP_ID_CSw, eDrcData);        
            //Post scale
            // No Ls/Rs
            //u4AQPostScale[0] = _au4AQ_PostScale_LsRs[0];
            //u4AQPostScale[1] = _au4AQ_PostScale_LsRs[1];
            //ADAC_DigiAmp_SetPostScale(AUD_AMP_ID_LsRs, u4AQPostScale[0],u4AQPostScale[1]);
            u4AQPostScale[0] = _au4AQ_PostScale_CSw[0];
            u4AQPostScale[1] = _au4AQ_PostScale_CSw[1];
            ADAC_DigiAmp_SetPostScale(AUD_AMP_ID_CSw, u4AQPostScale[0],u4AQPostScale[1]);                
        }         
    }
}


void _AUD_FlashAQPeriph_EXTPEQ(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1ModeCase, SAMPLE_FREQ_T eFS)
{
    UINT8 eConvertSrc,eConvertFs;
    UINT8 i,j;
#if 1//def  CC_AUD_PEQ_SUPPORT
    UINT16   u2PEQDataFc[9];
    UINT16   u2PEQDataQ[9];
    INT8   i1PEQDataGain[9];
    UINT8* u4TempPtr;
#else
    UINT32* u4TempPtr;
#endif
    AUD_AQ_EXTPEQ_DATA_T rPEQData;
    static AUD_DEC_STREAM_FROM_T eOriStreamFrom = AUD_STREAM_FROM_OTHERS;
    static UINT8 u1OriModeCase = 0xFF;
    static SAMPLE_FREQ_T eOriFS = FS_64K;

    UNUSED(i);
    UNUSED(eConvertSrc);
    UNUSED(rPEQData);
    UNUSED(eFS);
    UNUSED(eConvertFs);

    //Skip if mode and source is the same.
    if ((eOriStreamFrom == eStreamFrom) && (u1OriModeCase == u1ModeCase) && (eOriFS == eFS) && (!_fgFlashAQPeriphRecover))
    {
        return;
    }
    else
    {
        eOriStreamFrom = eStreamFrom;
        u1OriModeCase = u1ModeCase;
        eOriFS = eFS;
    }
    #ifdef CC_AUD_SX1_FEATURE
    ADAC_SetPEQSpkEnable(FALSE);
    #endif
    
    eConvertSrc = _au1AQSrc_Convert[eStreamFrom];
    // External PEQ
        if (eFS == FS_32K)
            eConvertFs = AQ_AMP_FS_32K;
        else if(eFS == FS_44K)
            eConvertFs = AQ_AMP_FS_44K;
        else
            eConvertFs = AQ_AMP_FS_48K;

    // External PEQ
    //  FlashData Layout (L/R, Ls/Rs)   7       
    // 32K   PEQ1   Coeff1 ~ Coeff5
    // 44K   PEQ1   Coeff1 ~ Coeff5
    // 48K   PEQ1   Coeff1 ~ Coeff5
    // ......
    //
    // 32K   PEQ7   Coeff1 ~ Coeff5
    // 44K   PEQ7   Coeff1 ~ Coeff5
    // 48K   PEQ7   Coeff1 ~ Coeff5
    
    //  FlashData Layout (C/SE)  4*2         
    // 32K   PEQ1   Coeff1 ~ Coeff5
    // 44K   PEQ1   Coeff1 ~ Coeff5
    // 48K   PEQ1   Coeff1 ~ Coeff5
    // ......
    // 32K   PEQ4   Coeff1 ~ Coeff5
    // 44K   PEQ4   Coeff1 ~ Coeff5
    // 48K   PEQ4   Coeff1 ~ Coeff5
    // 32K   PEQ1   Coeff1 ~ Coeff5
    // 44K   PEQ1   Coeff1 ~ Coeff5
    // 48K   PEQ1   Coeff1 ~ Coeff5
    // ......
    // 32K   PEQ4   Coeff1 ~ Coeff5
    // 44K   PEQ4   Coeff1 ~ Coeff5
    // 48K   PEQ4   Coeff1 ~ Coeff5    
    
#ifdef CC_SONY_2K14_FLASH_AQ
    if (_au1MWSetPositionIdx == 1)
    {
        if(_fgAQ_Enable[AQ_TYPE_PEQ_TABLE])
        {
            #ifdef CC_AUD_PEQ_SUPPORT
            u4TempPtr = (UINT8*)&(_au1AQ_PEQ_TABLE[eConvertSrc][u1ModeCase]);
            for (i=0; i<9; i++)
            {
                u2PEQDataFc[i] = *(UINT8*)(u4TempPtr + i*5);
                u2PEQDataFc[i] = (*(UINT8*)(u4TempPtr + 1 + i*5)) + (u2PEQDataFc[i]<<8);
                u2PEQDataQ[i] = *(UINT8*)(u4TempPtr + 2 + i*5);
                u2PEQDataQ[i] = (*(UINT8*)(u4TempPtr + 3 + i*5)) + (u2PEQDataQ[i]<<8);
                i1PEQDataGain[i] = *(UINT8*)(u4TempPtr + 4 + i*5);
                AUD_DspSetPEQCfg2(AUD_DEC_MAIN, i, u2PEQDataFc[i], u2PEQDataQ[i], (INT16)i1PEQDataGain[i]);
            }
            #else
                LOG(0,"################################\n");
                LOG(0,"Please Enable CC_AUD_PEQ_SUPPORT\n");
                LOG(0,"################################\n");
            #endif
        }
    }
    else
    {   
        if(_fgAQ_Enable[AQ_TYPE_PEQ_WALL])
        {
            #ifdef CC_AUD_PEQ_SUPPORT
            u4TempPtr = (UINT8*)&(_au1AQ_PEQ_WALL[eConvertSrc][u1ModeCase]);
            for (i=0; i<9; i++)
            {
                u2PEQDataFc[i] = *(UINT8*)(u4TempPtr + i*5);
                u2PEQDataFc[i] = (*(UINT8*)(u4TempPtr + 1 + i*5)) + (u2PEQDataFc[i]<<8);
                u2PEQDataQ[i] = *(UINT8*)(u4TempPtr + 2 + i*5);
                u2PEQDataQ[i] = (*(UINT8*)(u4TempPtr + 3 + i*5)) + (u2PEQDataQ[i]<<8);
                i1PEQDataGain[i] = *(UINT8*)(u4TempPtr + 4 + i*5);
                AUD_DspSetPEQCfg2(AUD_DEC_MAIN, i, u2PEQDataFc[i], u2PEQDataQ[i], (INT16)i1PEQDataGain[i]);
            }
            #else
                LOG(0,"################################\n");
                LOG(0,"Please Enable CC_AUD_PEQ_SUPPORT\n");
                LOG(0,"################################\n");
            #endif
        }
    }
#else
    if (_fgAQ_Enable[AQ_TYPE_EXT_PEQ])
    {
#ifdef  CC_AUD_PEQ_SUPPORT
        u4TempPtr = (UINT8*)&(_au1AQ_PEQ[eConvertSrc][u1ModeCase]);
        for (i=0; i<9; i++)
        {
            u2PEQDataFc[i] = *(UINT8*)(u4TempPtr + i*5);
            u2PEQDataFc[i] = (*(UINT8*)(u4TempPtr + 1 + i*5)) + (u2PEQDataFc[i]<<8);
            u2PEQDataQ[i] = *(UINT8*)(u4TempPtr + 2 + i*5);
            u2PEQDataQ[i] = (*(UINT8*)(u4TempPtr + 3 + i*5)) + (u2PEQDataQ[i]<<8);
            i1PEQDataGain[i] = *(UINT8*)(u4TempPtr + 4 + i*5);
            AUD_DspSetPEQCfg2(AUD_DEC_MAIN, i, u2PEQDataFc[i], u2PEQDataQ[i], (INT16)i1PEQDataGain[i]);
        }       
#else
        #if 0 //The layout is incorrect. the same FS is not put together
        x_memcpy(&rPEQData, &(_au4AQ_EXT_PEQ[eConvertSrc][u1ModeCase][eConvertFs]), sizeof(AUD_AQ_EXTPEQ_DATA_T));
        #else
        u4TempPtr = (UINT32*)&(_au4AQ_EXT_PEQ[eConvertSrc][u1ModeCase]);
        u4TempPtr += ((UINT8)eConvertFs * 5);
        for (i=0; i<7; i++)
        {
            for (j=0; j<5; j++)
            {
                //LOG(3, "%d, 0x%x, 0x%x\n",(i*15+j), (UINT32*)(u4TempPtr + i*15 + j), *(UINT32*)(u4TempPtr + i*15 + j) );
                rPEQData.u4Data[(i*5)+j] = *(UINT32*)(u4TempPtr + i*15 + j);
            }            
        }
        #endif
        if (_IsUseSTA381())
        {
                ADAC_STA381_SetPEQ(AUD_AMP_ID_LR, &rPEQData);   
        }
        else if (_IsUseDigiAmpTAS5717())
        {
                ;//to do
        }   
        else
        {
        ADAC_DigiAmp_SetPEQ(AUD_AMP_ID_LR, &rPEQData);        
        }
#endif
    }
#endif  ///CC_SONY_2K13_FLASH_AQ

    // External PEQ for Ls/Rs, C/Sw
    if (AUD_FlashAQ_ModelQueryFunc() > 0)
    {
        #if 0 //L TYPE no Ls/Rs.
        if (_fgAQ_Enable[AQ_TYPE_EXT_PEQ_LSRS])
        {
            #if 0
            x_memcpy(&rPEQData, &(_au4AQ_EXT_PEQ_LsRS[eConvertSrc][u1ModeCase][eConvertFs]), sizeof(AUD_AQ_EXTPEQ_DATA_T));
            #else
            u4TempPtr = (UINT32*)&(_au4AQ_EXT_PEQ_LsRS[eConvertSrc][u1ModeCase]);
            u4TempPtr += ((UINT8)eConvertFs * 5);
            for (i=0; i<7; i++)
            {
                for (j=0; j<5; j++)
                {
                    //LOG(3, "%d, 0x%x, 0x%x\n",(i*15+j), (UINT32*)(u4TempPtr + i*15 + j), *(UINT32*)(u4TempPtr + i*15 + j) );
                    rPEQData.u4Data[(i*5)+j] = *(UINT32*)(u4TempPtr + i*15 + j);
                }            
            }            
            #endif
            ADAC_DigiAmp_SetPEQ(AUD_AMP_ID_LsRs, &rPEQData);
        }
        #endif
        if (_fgAQ_Enable[AQ_TYPE_EXT_PEQ_CSW])
        {   
            #if 0
            x_memcpy(&rPEQData, &(_au4AQ_EXT_PEQ_CSw[eConvertSrc][u1ModeCase][eConvertFs]), sizeof(AUD_AQ_EXTPEQ_DATA_T));
            #else           
                #if 1//def  CC_AUD_PEQ_SUPPORT
                u4TempPtr = (UINT8*)&(_au4AQ_EXT_PEQ_CSw[eConvertSrc][u1ModeCase]);
                #else           
                u4TempPtr = (UINT32*)&(_au4AQ_EXT_PEQ_CSw[eConvertSrc][u1ModeCase]);
                #endif
            u4TempPtr += ((UINT8)eConvertFs * 5);
            for (i=0; i<8; i++) // 4 PEQs for 2 Channels.
            {
                for (j=0; j<5; j++)
                {
                    //LOG(3, "%d, 0x%x, 0x%x\n",(i*15+j), (UINT32*)(u4TempPtr + i*15 + j), *(UINT32*)(u4TempPtr + i*15 + j) );
                    rPEQData.u4Data[(i*5)+j] = *(UINT32*)(u4TempPtr + i*15 + j);
                }
            }
            #endif            
            if (_IsUseSTA381())
            {
                ADAC_STA381_SetPEQ(AUD_AMP_ID_CSw, &rPEQData);   
            }
            else
            {
                ADAC_DigiAmp_SetPEQ(AUD_AMP_ID_CSw, &rPEQData);
            }
        }
    }

    // UNMute Speaker ...........
    #ifdef CC_AUD_SX1_FEATURE
    ADAC_SetPEQSpkEnable(TRUE);    
    #endif
    //LOG(5,"======  Set Speaker to UUUUUUUUUNNNNMute ==========\n");    
    
#ifdef CC_AUD_SUPPORT_STC_COMPRESSOR 
    //Set STC Compressor Filter Coefficents
    if (_fgAQ_Enable[AQ_TYPE_STC381FILTER])
    {    
        if (_IsUseSTA381()) 
        {       
            u4TempPtr = (UINT32*)&(_au4AQ_EXT_381STCFilter);
            u4TempPtr += ((UINT8)eConvertFs * 5);
            for (i=0; i<4; i++)  //4 IIR
            {
                for (j=0; j<5; j++)
                {
                    //LOG(3, "%d, 0x%x, 0x%x\n",(i*15+j), (UINT32*)(u4TempPtr + i*15 + j), *(UINT32*)(u4TempPtr + i*15 + j) );
                    rPEQData.u4Data[(i*5)+j] = *(UINT32*)(u4TempPtr + i*15 + j);
                }            
            }
            ADAC_STA381_SetSTCFilter(AUD_AMP_ID_LR, &rPEQData);   
         }
     }
#endif    
      
}

void _AUD_FlashAQPeriphRecover(void)
{
    AUD_DEC_STREAM_FROM_T eStreamFrom = AUD_STREAM_FROM_OTHERS;
    SAMPLE_FREQ_T eFS = FS_48K; 

    AUD_DRVGetStreamFrom(AUD_DEC_MAIN, &eStreamFrom);
    VERIFY(eStreamFrom < AUD_STREAM_FROM_NUM);
    
    _fgFlashAQPeriphRecover = TRUE;
    _AUD_FlashAQPeriphSMChange(eStreamFrom, _au1MWSetSoundModeIdx);
    _AUD_FlashAQPeriph_EXTPEQ(eStreamFrom, _au1MWSetSoundModeIdx, eFS);
    _fgFlashAQPeriphRecover = FALSE;
}
#ifdef CC_AUD_W_SUPPORT  
extern UINT32 CustomPCBAQueryCountry(void);
//#include "Wistron/d_Waffle_cust.h"   ==> to Fix
#include "../../../d_inc/Foxconn/d_Fiji_cust.h"
//#define DRV_CUSTOM_COUNTRY_PAA                      1
//#define DRV_CUSTOM_COUNTRY_PAD                      2
//#define DRV_CUSTOM_COUNTRY_COL                      3
#endif
void _AUD_FlashAQOperation(UINT8 u1DecId, AUD_AQ_TYPE_T eAQ_Type, AUD_AQ_OP_TYPE_T eAQ_OPType, UINT16 u2Data)
{
    UINT8 u1ChIdx;
    UINT8 eConvertSrc;
    UINT8 i;
    INT8  eqValue[CH_EQ_BAND_NO+1];
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    UINT32 u4Tmp1, u4Tmp2, u4Tmp3;
    UINT8  u1Volume,u1TableIdx,u1TableIdx_temp, u1NewBassAttnLvl, u1NewTrebleAttnLvl;
#ifdef CC_AUD_W_SUPPORT    
    UINT32 u4CountryIndex = CustomPCBAQueryCountry();
    //UINT32 u4CountryIndex = DRV_CUSTOM_COUNTRY_PAA;
    //if (u4CountryIndex == DRV_CUSTOM_COUNTRY_PAA) printf("Bingo PAA!\n\n");
    //else if (u4CountryIndex == DRV_CUSTOM_COUNTRY_EU) printf("Wrong EU!\n\n");
    //else printf("Wrong other!\n\n");        
#endif
    UNUSED(eStreamFrom);
    UNUSED(u1ChIdx);
    UNUSED(eConvertSrc);
    UNUSED(eAQ_OPType);
    UNUSED(u1DecId);
    UNUSED(i);
    UNUSED(eqValue);
#ifdef CC_AUD_W_SUPPORT      
    UNUSED(u4CountryIndex);
#endif

    switch (eAQ_Type)
    {

    case AQ_TYPE_CH_VOLUME:
        // Apply the Channel Volume Table.
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_CH_VOLUME]))
        {
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Comm_Convert[(UINT8)u2Data];
            VERIFY( eConvertSrc < FLASH_AQ_COMM_SRC_NUM );                
            for (u1ChIdx = AUD_CH_FRONT_LEFT; u1ChIdx<=AUD_CH_DMX_RIGHT; u1ChIdx++)
            {
                _aai1ChlVolExtraGain[u1DecId][(AUD_CH_T)u1ChIdx] = (INT8)(_au1AQChVolume[eConvertSrc][(AUD_CH_T)u1ChIdx]-128);
//#ifdef CC_AUD_W_SUPPORT
#if 0
                // for PAA & PAD, is source is ATV, add extra 6 dB in Speaker and HP (for compensate NO_EU_Mode)
                if ((u4CountryIndex == DRV_CUSTOM_COUNTRY_PAA) || (u4CountryIndex == DRV_CUSTOM_COUNTRY_PAD))
                {
                    if (((AUD_DEC_STREAM_FROM_T)u2Data == AUD_STREAM_FROM_ANALOG_TUNER) &&
                        ((u1ChIdx == AUD_CH_FRONT_LEFT)||(u1ChIdx == AUD_CH_FRONT_RIGHT)||(u1ChIdx == AUD_CH_DMX_LEFT)||(u1ChIdx == AUD_CH_DMX_RIGHT)))
                    {
                        _aai1ChlVolExtraGain[u1DecId][(AUD_CH_T)u1ChIdx]+=12;
                    }
                }
                #if 0 // 2010/12/8 , change to add 3 dB in MTS fine tune volume 
                // for COL, is source is ATV, add extra 3 dB in Speaker and HP.
                if (u4CountryIndex == DRV_CUSTOM_COUNTRY_COL)
                {
                    if (((AUD_DEC_STREAM_FROM_T)u2Data == AUD_STREAM_FROM_ANALOG_TUNER) &&
                        ((u1ChIdx == AUD_CH_FRONT_LEFT)||(u1ChIdx == AUD_CH_FRONT_RIGHT)||(u1ChIdx == AUD_CH_DMX_LEFT)||(u1ChIdx == AUD_CH_DMX_RIGHT)))
                    {
                        _aai1ChlVolExtraGain[u1DecId][(AUD_CH_T)u1ChIdx]+=6;
                    }
                }                
                #endif
#endif
                _AUD_DspVolumeChange(u1DecId, (AUD_CH_T)u1ChIdx);
                //Set TV SCART out Channel volume gain as "DTV LineOut channel volume gain"
                // => Set AUX L/R channel volume by drv_cust, not by AQ(Ls/Rs) channel volume
                //    gallen, 20110817 modify
                if (u1ChIdx == AUD_CH_REAR_LEFT)
                {
                    if (_aeStreamSrc[AUD_DEC_AUX] == AUD_STREAM_FROM_DIGITAL_TUNER)
                    {
                        //_aai1ChlVolExtraGain[AUD_DEC_AUX][(AUD_CH_T)AUD_CH_FRONT_LEFT] = 
                        //      (INT8)(_au1AQChVolume[_au1AQSrc_Comm_Convert[AUD_STREAM_FROM_DIGITAL_TUNER]][(AUD_CH_T)u1ChIdx]-128);
                    }
                    else
                    {
                        //_aai1ChlVolExtraGain[AUD_DEC_AUX][(AUD_CH_T)AUD_CH_FRONT_LEFT] = 
                        //      (INT8)(_au1AQChVolume[_au1AQSrc_Comm_Convert[AUD_STREAM_FROM_ANALOG_TUNER]][(AUD_CH_T)u1ChIdx]-128);                    
                    }
                    _AUD_DspVolumeChange(AUD_DEC_AUX, (AUD_CH_T)AUD_CH_FRONT_LEFT);    
                }
                else if (u1ChIdx == AUD_CH_REAR_RIGHT)
                {
                    if (_aeStreamSrc[AUD_DEC_AUX] == AUD_STREAM_FROM_DIGITAL_TUNER)
                    {
                        //_aai1ChlVolExtraGain[AUD_DEC_AUX][(AUD_CH_T)AUD_CH_FRONT_RIGHT] = 
                        //      (INT8)(_au1AQChVolume[_au1AQSrc_Comm_Convert[AUD_STREAM_FROM_DIGITAL_TUNER]][(AUD_CH_T)u1ChIdx]-128);
                    }
                    else
                    {
                        //_aai1ChlVolExtraGain[AUD_DEC_AUX][(AUD_CH_T)AUD_CH_FRONT_RIGHT] = 
                        //      (INT8)(_au1AQChVolume[_au1AQSrc_Comm_Convert[AUD_STREAM_FROM_ANALOG_TUNER]][(AUD_CH_T)u1ChIdx]-128);                        
                    }
                    _AUD_DspVolumeChange(AUD_DEC_AUX, (AUD_CH_T)AUD_CH_FRONT_RIGHT);
                }
            }
        }
        break;

    case AQ_TYPE_SP_VOL:
        if (_fgAQ_Enable[AQ_TYPE_SP_VOL])
        {
            for (i= 0;i<AUD_STREAM_FROM_NUM;i++)
            {
                eConvertSrc = _au1AQSrc_Convert[i];
                VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
                _aai2InputSrcVol[AUD_DEC_MAIN][i] = (INT16)(_au1AQ_SP_Vol[eConvertSrc]-128);
            }
        }
        break;

    case AQ_TYPE_LO_VOL:
        #ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
        if (_fgAQ_Enable[AQ_TYPE_LO_VOL])
        {
            for (i= 0;i<AUD_STREAM_FROM_NUM;i++)
            {
                eConvertSrc = _au1AQSrc_Convert[i];
                VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
                _aai2InputSrcLineOutVol[AUD_DEC_MAIN][i] = (INT16)(_au1AQ_LO_Vol[eConvertSrc]-128);
            }
        }
        #endif // CC_AUD_INPUT_SRC_LINEOUT_VOL
        break;

    case AQ_TYPE_EQ:
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_EQ]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );            
            VERIFY( u2Data < FLASH_AQ_SM_NUM); //fix clockwork
            eqValue[0] = 0;
            for (i = 1; i < (CH_EQ_BAND_NO+1); i++) //7 EQ bands are 0, 1~7. 
            {
                eqValue[i] = (INT8)_au1AQ_EQLevel[eConvertSrc][u2Data][i-1] - (EQ_BAND_GAIN_DB_MAX * 2);
            }
            AUD_DspChSetEqTable(AUD_DEC_MAIN, AUD_EQ_CUST1, eqValue); //Sound mode EQ use CUST1 ...
            _AUD_DspChEqLoadPresetChgMode(AUD_DEC_MAIN, AUD_EQ_CUST1);
        }
        break;

#ifdef CC_SONY_2K14_FLASH_AQ
    case AQ_TYPE_BASS_TREBLE_TABLE:
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_BASS_TREBLE_TABLE]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );                        
            for (i = 0; i < AQ_BASS_ITEMS; i++)
            {         
                // XXXX The range is 0~12 , means -6 ~ +6 dB
                // Change Spec to 0 ~ 20 , mapping to -10 ~ +10 dB.
                _aai1BassTreble[i] = (INT8)(_au1AQBassTrebleOffset_TABLE[eConvertSrc][u2Data][i] - MID_BASS_TREBLE_OFFSET);
            }
        }
        _AUD_DspBassBoostGain(AUD_DEC_MAIN, _u1MWBassLevel);
        _AUD_DspClearBoostGain(AUD_DEC_MAIN, _u1MWTrebleLevel);
        break;
    
    case AQ_TYPE_BASS_TREBLE_WALL:
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_BASS_TREBLE_WALL]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );                        
            for (i = 0; i < AQ_BASS_ITEMS; i++)
            {         
                // XXXX The range is 0~12 , means -6 ~ +6 dB
                // Change Spec to 0 ~ 20 , mapping to -10 ~ +10 dB.
                _aai1BassTreble[i] = (INT8)(_au1AQBassTrebleOffset_WALL[eConvertSrc][u2Data][i] - MID_BASS_TREBLE_OFFSET);
            }
        }
        _AUD_DspBassBoostGain(AUD_DEC_MAIN, _u1MWBassLevel);
        _AUD_DspClearBoostGain(AUD_DEC_MAIN, _u1MWTrebleLevel);
        break;
    
    case AQ_TYPE_BASS_MANAGEMENT_TABLE:
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_BASS_MANAGEMENT_TABLE]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );                        
            VERIFY( u2Data < FLASH_AQ_SM_NUM); //fix clockwork                   
            AUD_DspSpeakerLargeSmallConfig(AUD_DEC_MAIN, _au1AQBassManage_TABLE[eConvertSrc][u2Data][0], _au1AQBassManage_TABLE[eConvertSrc][u2Data][1], \
            _au1AQBassManage_TABLE[eConvertSrc][u2Data][2], _au1AQBassManage_TABLE[eConvertSrc][u2Data][3]);
            AUD_DspSpkSizeCfg(AUD_DEC_MAIN, (UINT32)_au2AQBMCutOffMap[_au1AQBassManage_TABLE[eConvertSrc][u2Data][4]]); 
        }        
        break;
            
    case AQ_TYPE_BASS_MANAGEMENT_WALL:
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_BASS_MANAGEMENT_WALL]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );                        
            VERIFY( u2Data < FLASH_AQ_SM_NUM); //fix clockwork                   
            AUD_DspSpeakerLargeSmallConfig(AUD_DEC_MAIN, _au1AQBassManage_WALL[eConvertSrc][u2Data][0], _au1AQBassManage_WALL[eConvertSrc][u2Data][1], \
            _au1AQBassManage_WALL[eConvertSrc][u2Data][2], _au1AQBassManage_WALL[eConvertSrc][u2Data][3]);
            AUD_DspSpkSizeCfg(AUD_DEC_MAIN, (UINT32)_au2AQBMCutOffMap[_au1AQBassManage_WALL[eConvertSrc][u2Data][4]]); 
        }        
        break;    
            
    case AQ_TYPE_SPKHEIGHT_TABLE:
    #if (defined(CC_AUD_SPEAKER_HEIGHT) || defined(CC_AUD_SPEAKER_HEIGHT_V2))
    #ifdef CC_AUD_SPEAKER_HEIGHT_MODE
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_SPKHEIGHT_TABLE]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
            VERIFY( u2Data < FLASH_AQ_SM_NUM);
            //!!!!!!!!!!!!! Wait for SPH pra is ready !!!!!!!!!!!!!!!!!!!!!            
            _AUD_DspSetSPHMode(_au1AQ_SPH_TABLE[eConvertSrc][u2Data][0]);
        }                
    #endif
    #endif
        break;
        
    case AQ_TYPE_SPKHEIGHT_WALL:
#if (defined(CC_AUD_SPEAKER_HEIGHT) || defined(CC_AUD_SPEAKER_HEIGHT_V2))
#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_SPKHEIGHT_WALL]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
            VERIFY( u2Data < FLASH_AQ_SM_NUM);
            //!!!!!!!!!!!!! Wait for SPH pra is ready !!!!!!!!!!!!!!!!!!!!!            
            _AUD_DspSetSPHMode(_au1AQ_SPH_WALL[eConvertSrc][u2Data][0]);
        }                
#endif
#endif
        break;


    case AQ_TYPE_BASS_TREBLE_CUTOFF:
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_BASS_TREBLE_CUTOFF]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );                        
            vWriteShmUINT8(B_SBASSBOOSTCUTOFF, _au1AQBassTrebleCutoff[eConvertSrc][u2Data][0]);
            vWriteShmUINT8(B_SBASSCLEARCUTOFF, _au1AQBassTrebleCutoff[eConvertSrc][u2Data][1]);             
        }
        break;  
#else
    case AQ_TYPE_BASS_TREBLE:
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_BASS_TREBLE]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );                        
            for (i = 0; i < AQ_BASS_ITEMS; i++)
            {         
                // XXXX The range is 0~12 , means -6 ~ +6 dB
                // Change Spec to 0 ~ 20 , mapping to -10 ~ +10 dB.
                _aai1BassTreble[i] = (INT8)(_au1AQBassTrebleOffset[eConvertSrc][u2Data][i] - MID_BASS_TREBLE_OFFSET);
            }
        }
        _AUD_DspBassBoostGain(AUD_DEC_MAIN, _u1MWBassLevel);
        _AUD_DspClearBoostGain(AUD_DEC_MAIN, _u1MWTrebleLevel);
        break;

    case AQ_TYPE_BASS_MANAGEMENT:
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_BASS_MANAGEMENT]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );                        
            VERIFY( u2Data < FLASH_AQ_SM_NUM); //fix clockwork                   
            AUD_DspSpeakerLargeSmallConfig(AUD_DEC_MAIN, _au1AQBassManage[eConvertSrc][u2Data][0], _au1AQBassManage[eConvertSrc][u2Data][1], \
            _au1AQBassManage[eConvertSrc][u2Data][2], _au1AQBassManage[eConvertSrc][u2Data][3]);
            AUD_DspSpkSizeCfg(AUD_DEC_MAIN, (UINT32)_au2AQBMCutOffMap[_au1AQBassManage[eConvertSrc][u2Data][4]]); 
        }        
        break;
        
    case AQ_TYPE_SPKHEIGHT:
    #if (defined(CC_AUD_SPEAKER_HEIGHT) || defined(CC_AUD_SPEAKER_HEIGHT_V2))
    #ifdef CC_AUD_SPEAKER_HEIGHT_MODE
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_SPKHEIGHT]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
            VERIFY( u2Data < FLASH_AQ_SM_NUM);
            //!!!!!!!!!!!!! Wait for SPH pra is ready !!!!!!!!!!!!!!!!!!!!!            
            _AUD_DspSetSPHMode(_au1AQ_SPH[eConvertSrc][u2Data][0]);
        }                
    #endif
    #endif
        break;
#endif
#ifdef CC_SONY_2K14_FLASH_AQ
    case AQ_TYPE_LOUDNESS_TABLE:        
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_LOUDNESS_TABLE]))
        {
            UNUSED(u1TableIdx_temp);
            if(!_u1LoudnessFlag_TABLE[u2Data])
            {
                AUD_DspBassBoostEnable2(AUD_DEC_MAIN, FALSE);
                AUD_DspClearBoostEnable2(AUD_DEC_MAIN, FALSE);
                return;
            }
            
            u1Volume = _AUD_DspGetMasterVolume(u1DecId);
            //Note that vol 0~5 maps to tbl[0], volume 6~10 maps to tbl[1], vol 11~15 maps to tbl[2], ..., 96~100 maps to tbl[19]    
            if(u1Volume == 0)
            {
                u1TableIdx = 0;
            }
            else
            {
                u1TableIdx = (u1Volume - 1)/5;
            }
            if(u1TableIdx > 19)
            {
                u1TableIdx = 19;
            }

            LOG(7, "==> LOUDNESS u1TableIdx = 0x%x", u1TableIdx);
            u1NewBassAttnLvl = _ai1BassAttnTable_TABLE[u2Data][u1TableIdx];
            u1NewTrebleAttnLvl = _ai1TrebleAttnTable_TABLE[u2Data][u1TableIdx];


            _AUD_DspBassBoostGain2(AUD_DEC_MAIN, u1NewBassAttnLvl);
            _AUD_DspClearBoostGain2(AUD_DEC_MAIN, u1NewTrebleAttnLvl);

        }
        break;

    case AQ_TYPE_LOUDNESS_WALL:     
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_LOUDNESS_WALL]))
        {
            UNUSED(u1TableIdx_temp);
            if(!_u1LoudnessFlag_WALL[u2Data])
            {
                AUD_DspBassBoostEnable2(AUD_DEC_MAIN, FALSE);
                AUD_DspClearBoostEnable2(AUD_DEC_MAIN, FALSE);
                return;
            }
            
            u1Volume = _AUD_DspGetMasterVolume(u1DecId);
            //Note that vol 0~5 maps to tbl[0], volume 6~10 maps to tbl[1], vol 11~15 maps to tbl[2], ..., 96~100 maps to tbl[19]    
            if(u1Volume == 0)
            {
                u1TableIdx = 0;
            }
            else
            {
                u1TableIdx = (u1Volume - 1)/5;
            }
            if(u1TableIdx > 19)
            {
                u1TableIdx = 19;
            }
    
            LOG(7, "==> LOUDNESS u1TableIdx = 0x%x", u1TableIdx);
            u1NewBassAttnLvl = _ai1BassAttnTable_WALL[u2Data][u1TableIdx];
            u1NewTrebleAttnLvl = _ai1TrebleAttnTable_WALL[u2Data][u1TableIdx];
    
    
            _AUD_DspBassBoostGain2(AUD_DEC_MAIN, u1NewBassAttnLvl);
            _AUD_DspClearBoostGain2(AUD_DEC_MAIN, u1NewTrebleAttnLvl);
    
        }
        break;

#else
    case AQ_TYPE_LOUDNESS:      
        #ifdef CC_AUD_LOUDNESS_FROM_SOURCE_SOUNDMODE
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_LOUDNESS]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
            LOG(4,"===>eStreamFrom(%d), eConvertSrc(%d)\n",eStreamFrom,eConvertSrc);
            //Note that vol 45~49 maps to tbl[0], volume 50~54 maps to tbl[1], vol 55~59 maps to tbl[2], ..., 90~100 maps to tbl[9]
            u1Volume = _AUD_DspGetMasterVolume(u1DecId);
            u1TableIdx = (u1Volume-1)/10;
            u1TableIdx_temp = (u1Volume-1)%10;
            if( u1TableIdx < 5) 
            {
                u1TableIdx = 0;
            }
            else if( u1TableIdx >= 9 ) 
            {
                u1TableIdx = 9;
            }
            else if( u1TableIdx_temp < 5) 
            {
                u1TableIdx = (u1TableIdx-4)*2-1;
            }
            else 
            {
                u1TableIdx = (u1TableIdx-4)*2;
            } 
            if( u1Volume <= 45 )
            {
                LOG(7, "==> AQ_TYPE_LOUDNESS :Bass = 0x%x; Treble = 0x%x, vol(%d),source(%d),sm(%d)\n",BASS_TREBLE_ATTN_GAIN_DB_MAX,BASS_TREBLE_ATTN_GAIN_DB_MAX,u1Volume,eConvertSrc,u2Data);
                _AUD_DspBassBoostGain2(AUD_DEC_MAIN, BASS_TREBLE_ATTN_GAIN_DB_MAX);
                _AUD_DspClearBoostGain2(AUD_DEC_MAIN, BASS_TREBLE_ATTN_GAIN_DB_MAX);
            }
            else
            {
                LOG(7, "==> AQ_TYPE_LOUDNESS :Bass = 0x%x; Treble = 0x%x, vol(%d),source(%d),sm(%d)\n",_ai1BassAttnTable[eConvertSrc][u2Data][u1TableIdx],_ai1TrebleAttnTable[eConvertSrc][u2Data][u1TableIdx],u1Volume,eConvertSrc,u2Data);
                _AUD_DspBassBoostGain2(AUD_DEC_MAIN, _ai1BassAttnTable[eConvertSrc][u2Data][u1TableIdx]);
                _AUD_DspClearBoostGain2(AUD_DEC_MAIN, _ai1TrebleAttnTable[eConvertSrc][u2Data][u1TableIdx]);
            }
        }
        #endif
        #if 1///def CC_SONY_2K13_FLASH_AQ
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_LOUDNESS]))
        {
            UNUSED(u1TableIdx_temp);
            if(!_u1LoudnessFlag)
            {
                AUD_DspBassBoostEnable2(AUD_DEC_MAIN, FALSE);
                AUD_DspClearBoostEnable2(AUD_DEC_MAIN, FALSE);
                return;
            }
            
            u1Volume = _AUD_DspGetMasterVolume(u1DecId);
            u1TableIdx = (u1Volume-1)/5;
            ///u1TableIdx_temp = (u1Volume-1)%5;
            if(u1TableIdx >= 8)
            {
                u1TableIdx -= 8;
            }
            else
            {
                u1TableIdx = 0;
            }
            
            if (u1TableIdx > 11)
            {
                u1TableIdx = 11;
            }
            LOG(7, "==> LOUDNESS u1TableIdx = 0x%x", u1TableIdx);
            u1NewBassAttnLvl = _ai1BassAttnTable[u1TableIdx];
            u1NewTrebleAttnLvl = _ai1TrebleAttnTable[u1TableIdx];

            if( u1Volume <= 40 )
            {
                u1NewBassAttnLvl = BASS_TREBLE_ATTN_GAIN_DB_MAX;
                u1NewTrebleAttnLvl = BASS_TREBLE_ATTN_GAIN_DB_MAX;
            }

            _AUD_DspBassBoostGain2(AUD_DEC_MAIN, u1NewBassAttnLvl);
            _AUD_DspClearBoostGain2(AUD_DEC_MAIN, u1NewTrebleAttnLvl);

        }
        #endif
        break;
#endif
#ifdef CC_SONY_2K14_FLASH_AQ
        case AQ_TYPE_VBASS_TABLE:
            if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_VBASS_TABLE]))
            {
                AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
                //The StreamFrom in AQ Table is not in the same order as our emu type.
                eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
                VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
        
                // gain1
                u4Tmp1 = (_au1AQ_VBass_TABLE[eConvertSrc][u2Data][0] << 16) | 
                         (_au1AQ_VBass_TABLE[eConvertSrc][u2Data][1] << 8)  |
                         (_au1AQ_VBass_TABLE[eConvertSrc][u2Data][2]);
                // gain2
                u4Tmp2 = (_au1AQ_VBass_TABLE[eConvertSrc][u2Data][3] << 16) | 
                         (_au1AQ_VBass_TABLE[eConvertSrc][u2Data][4] << 8)  |
                         (_au1AQ_VBass_TABLE[eConvertSrc][u2Data][5]);          
                // cutoff freq.
                u4Tmp3 = _au1AQ_VBass_TABLE[eConvertSrc][u2Data][6];
                
                if(u4Tmp3 == 0xFF)
                {
                    LOG(5,"!!!BassBoost Disable!!! \n");
                    _AUD_DspSetVirBassEnable(FALSE);
                }
                else
                {
                    LOG(5,"!!!BassBoost Enable!!! \n");
                    _AUD_DspSetVirBassEnable(TRUE);
                }
        
                UNUSED(u4Tmp1);
                UNUSED(u4Tmp2);
                UNUSED(u4Tmp3);
                AUD_SetVBassCutOff((UINT8)u4Tmp3);
                _AUD_DspSetVirBassCfg(u1DecId, 48000, u4Tmp1, u4Tmp2); //FS is not used in this function.            
            }                
            break;
            
        case AQ_TYPE_VBASS_WALL:
            if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_VBASS_WALL]))
            {
                AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
                //The StreamFrom in AQ Table is not in the same order as our emu type.
                eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
                VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );
        
                // gain1
                u4Tmp1 = (_au1AQ_VBass_WALL[eConvertSrc][u2Data][0] << 16) | 
                         (_au1AQ_VBass_WALL[eConvertSrc][u2Data][1] << 8)  |
                         (_au1AQ_VBass_WALL[eConvertSrc][u2Data][2]);
                // gain2
                u4Tmp2 = (_au1AQ_VBass_WALL[eConvertSrc][u2Data][3] << 16) | 
                         (_au1AQ_VBass_WALL[eConvertSrc][u2Data][4] << 8)  |
                         (_au1AQ_VBass_WALL[eConvertSrc][u2Data][5]);           
                // cutoff freq.
                u4Tmp3 = _au1AQ_VBass_WALL[eConvertSrc][u2Data][6];
                
                if(u4Tmp3 == 0xFF)
                {
                    LOG(5,"!!!BassBoost Disable!!! \n");
                    _AUD_DspSetVirBassEnable(FALSE);
                }
                else
                {
                    LOG(5,"!!!BassBoost Enable!!! \n");
                    _AUD_DspSetVirBassEnable(TRUE);
                }
        
                UNUSED(u4Tmp1);
                UNUSED(u4Tmp2);
                UNUSED(u4Tmp3);
                AUD_SetVBassCutOff((UINT8)u4Tmp3);
                _AUD_DspSetVirBassCfg(u1DecId, 48000, u4Tmp1, u4Tmp2); //FS is not used in this function.            
            }                
            break;      
            
#else
    case AQ_TYPE_VBASS:
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_VBASS]))
        {
            AUD_DRVGetStreamFrom(u1DecId, &eStreamFrom);
            //The StreamFrom in AQ Table is not in the same order as our emu type.
            eConvertSrc = _au1AQSrc_Convert[(UINT8)eStreamFrom];
            VERIFY( eConvertSrc < FLASH_AQ_SRC_NUM );

            // gain1
            u4Tmp1 = (_au1AQ_VBass[eConvertSrc][u2Data][0] << 16) | 
                     (_au1AQ_VBass[eConvertSrc][u2Data][1] << 8)  |
                     (_au1AQ_VBass[eConvertSrc][u2Data][2]);
            // gain2
            u4Tmp2 = (_au1AQ_VBass[eConvertSrc][u2Data][3] << 16) | 
                     (_au1AQ_VBass[eConvertSrc][u2Data][4] << 8)  |
                     (_au1AQ_VBass[eConvertSrc][u2Data][5]);            
            // cutoff freq.
            u4Tmp3 = _au1AQ_VBass[eConvertSrc][u2Data][6];
            
            if(u4Tmp3 == 0xFF)
            {
                LOG(5,"!!!BassBoost Disable!!! \n");
                _AUD_DspSetVirBassEnable(FALSE);
            }
            else
            {
                LOG(5,"!!!BassBoost Enable!!! \n");
                _AUD_DspSetVirBassEnable(TRUE);
            }

            UNUSED(u4Tmp1);
            UNUSED(u4Tmp2);
            UNUSED(u4Tmp3);
            AUD_SetVBassCutOff((UINT8)u4Tmp3);
            _AUD_DspSetVirBassCfg(u1DecId, 48000, u4Tmp1, u4Tmp2); //FS is not used in this function.            
        }                
        break;
#endif
#ifdef CC_SONY_2K14_FLASH_AQ
    case AQ_TYPE_MTKDRC_TABLE:
#ifdef CC_AUD_DRC_V20
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_MTKDRC_TABLE]))
        {
            /// Low-F Band
            u4Tmp1 = (_au1AQ_MTKDRC_TABLE[u2Data][1]<< 16)|(_au1AQ_MTKDRC_TABLE[u2Data][2]<<8)|(_au1AQ_MTKDRC_TABLE[u2Data][3]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_ATTACK_RATE, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_ATTACK_RATE, u4Tmp1, AUD_DEC_MAIN);
            u4Tmp1 = (_au1AQ_MTKDRC_TABLE[u2Data][4]<< 16)|(_au1AQ_MTKDRC_TABLE[u2Data][5]<<8)|(_au1AQ_MTKDRC_TABLE[u2Data][6]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_RELEASE_RATE, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_RELEASE_RATE, u4Tmp1, AUD_DEC_MAIN);
            u4Tmp1 = (_au1AQ_MTKDRC_TABLE[u2Data][7]<< 16)|(_au1AQ_MTKDRC_TABLE[u2Data][8]<<8)|(_au1AQ_MTKDRC_TABLE[u2Data][9]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_TARGET_LEVEL, u4Tmp1);
            //vWriteShmUINT32(D_DRC_RATIO, _au1AQ_MTKDRC[10]);
            _AUD_DspSetDrc2(DRC2_TARGET_LEVEL, u4Tmp1, AUD_DEC_MAIN);
            
            //_AUD_DspSetDrc2(DRC2_RATIO, _au1AQ_MTKDRC[10], AUD_DEC_MAIN);
            if (_au1AQ_MTKDRC_TABLE[u2Data][12] == 0xff)
            {    
                _AUD_DspSetDrc2(DRC2_RATIO, 0x7fffff, AUD_DEC_MAIN);    
            }
            else
            {
                _AUD_DspSetDrc2(DRC2_RATIO, (_au1AQ_MTKDRC_TABLE[u2Data][12]*0x7fff), AUD_DEC_MAIN);
            }
            /// High-F Band
            u4Tmp1 = (_au1AQ_MTKDRC_TABLE[u2Data][17]<< 16)|(_au1AQ_MTKDRC_TABLE[u2Data][18]<<8)|(_au1AQ_MTKDRC_TABLE[u2Data][19]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_ATTACK_RATE_1, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_ATTACK_RATE_1, u4Tmp1, AUD_DEC_MAIN);
            u4Tmp1 = (_au1AQ_MTKDRC_TABLE[u2Data][20]<< 16)|(_au1AQ_MTKDRC_TABLE[u2Data][21]<<8)|(_au1AQ_MTKDRC_TABLE[u2Data][22]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_RELEASE_RATE_1, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_RELEASE_RATE_1, u4Tmp1, AUD_DEC_MAIN);     
            u4Tmp1 = (_au1AQ_MTKDRC_TABLE[u2Data][23]<< 16)|(_au1AQ_MTKDRC_TABLE[u2Data][24]<<8)|(_au1AQ_MTKDRC_TABLE[u2Data][25]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_TARGET_LEVEL_1, u4Tmp1);
            //vWriteShmUINT32(D_DRC_RATIO_1, _au1AQ_MTKDRC[21]);
            _AUD_DspSetDrc2(DRC2_TARGET_LEVEL_1, u4Tmp1, AUD_DEC_MAIN);
            //_AUD_DspSetDrc2(DRC2_RATIO_1, _au1AQ_MTKDRC[21], AUD_DEC_MAIN);
            if (_au1AQ_MTKDRC_TABLE[u2Data][28] == 0xff)
            {    
                _AUD_DspSetDrc2(DRC2_RATIO_1, 0x7fffff, AUD_DEC_MAIN);  
            }
            else
            {
                _AUD_DspSetDrc2(DRC2_RATIO_1, (_au1AQ_MTKDRC_TABLE[u2Data][28]*0x7fff), AUD_DEC_MAIN);
            }
            //vWriteShmUINT16(W_DRC_FLAG, _au1AQ_MTKDRC[9]);
            _AUD_DspSetDrc2(DRC2_HYSTERESIS, _au1AQ_MTKDRC_TABLE[u2Data][11], AUD_DEC_MAIN);
            _AUD_DspSetDrc2(DRC2_HYSTERESIS_1, _au1AQ_MTKDRC_TABLE[u2Data][27], AUD_DEC_MAIN);
            _AUD_DspSetDrc2(DRC2_FLAG, _au1AQ_MTKDRC_TABLE[u2Data][0], AUD_DEC_MAIN);
            
        }      
#endif //CC_AUD_DRC_V20
        break; 

   case AQ_TYPE_MTKDRC_WALL:
#ifdef CC_AUD_DRC_V20
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_MTKDRC_WALL]))
        {
            /// Low-F Band
            u4Tmp1 = (_au1AQ_MTKDRC_WALL[u2Data][1]<< 16)|(_au1AQ_MTKDRC_WALL[u2Data][2]<<8)|(_au1AQ_MTKDRC_WALL[u2Data][3]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_ATTACK_RATE, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_ATTACK_RATE, u4Tmp1, AUD_DEC_MAIN);
            u4Tmp1 = (_au1AQ_MTKDRC_WALL[u2Data][4]<< 16)|(_au1AQ_MTKDRC_WALL[u2Data][5]<<8)|(_au1AQ_MTKDRC_WALL[u2Data][6]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_RELEASE_RATE, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_RELEASE_RATE, u4Tmp1, AUD_DEC_MAIN);
            u4Tmp1 = (_au1AQ_MTKDRC_WALL[u2Data][7]<< 16)|(_au1AQ_MTKDRC_WALL[u2Data][8]<<8)|(_au1AQ_MTKDRC_WALL[u2Data][9]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_TARGET_LEVEL, u4Tmp1);
            //vWriteShmUINT32(D_DRC_RATIO, _au1AQ_MTKDRC[10]);
            _AUD_DspSetDrc2(DRC2_TARGET_LEVEL, u4Tmp1, AUD_DEC_MAIN);
            
            //_AUD_DspSetDrc2(DRC2_RATIO, _au1AQ_MTKDRC[10], AUD_DEC_MAIN);
            if (_au1AQ_MTKDRC_WALL[u2Data][12] == 0xff)
            {    
                _AUD_DspSetDrc2(DRC2_RATIO, 0x7fffff, AUD_DEC_MAIN);    
            }
            else
            {
                _AUD_DspSetDrc2(DRC2_RATIO, (_au1AQ_MTKDRC_WALL[u2Data][12]*0x7fff), AUD_DEC_MAIN);
            }
            /// High-F Band
            u4Tmp1 = (_au1AQ_MTKDRC_WALL[u2Data][17]<< 16)|(_au1AQ_MTKDRC_WALL[u2Data][18]<<8)|(_au1AQ_MTKDRC_WALL[u2Data][19]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_ATTACK_RATE_1, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_ATTACK_RATE_1, u4Tmp1, AUD_DEC_MAIN);
            u4Tmp1 = (_au1AQ_MTKDRC_WALL[u2Data][20]<< 16)|(_au1AQ_MTKDRC_WALL[u2Data][21]<<8)|(_au1AQ_MTKDRC_WALL[u2Data][22]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_RELEASE_RATE_1, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_RELEASE_RATE_1, u4Tmp1, AUD_DEC_MAIN);     
            u4Tmp1 = (_au1AQ_MTKDRC_WALL[u2Data][23]<< 16)|(_au1AQ_MTKDRC_WALL[u2Data][24]<<8)|(_au1AQ_MTKDRC_WALL[u2Data][25]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_TARGET_LEVEL_1, u4Tmp1);
            //vWriteShmUINT32(D_DRC_RATIO_1, _au1AQ_MTKDRC[21]);
            _AUD_DspSetDrc2(DRC2_TARGET_LEVEL_1, u4Tmp1, AUD_DEC_MAIN);
            //_AUD_DspSetDrc2(DRC2_RATIO_1, _au1AQ_MTKDRC[21], AUD_DEC_MAIN);
            if (_au1AQ_MTKDRC_WALL[u2Data][28] == 0xff)
            {    
                _AUD_DspSetDrc2(DRC2_RATIO_1, 0x7fffff, AUD_DEC_MAIN);  
            }
            else
            {
                _AUD_DspSetDrc2(DRC2_RATIO_1, (_au1AQ_MTKDRC_WALL[u2Data][28]*0x7fff), AUD_DEC_MAIN);
            }
            //vWriteShmUINT16(W_DRC_FLAG, _au1AQ_MTKDRC[9]);
            _AUD_DspSetDrc2(DRC2_HYSTERESIS, _au1AQ_MTKDRC_WALL[u2Data][11], AUD_DEC_MAIN);
            _AUD_DspSetDrc2(DRC2_HYSTERESIS_1, _au1AQ_MTKDRC_WALL[u2Data][27], AUD_DEC_MAIN);
            _AUD_DspSetDrc2(DRC2_FLAG, _au1AQ_MTKDRC_WALL[u2Data][0], AUD_DEC_MAIN);
            
        }      
#endif //CC_AUD_DRC_V20
        break; 

#else
    case AQ_TYPE_MTKDRC:
#ifdef CC_AUD_DRC_V20
        if ((u1DecId == AUD_DEC_MAIN) && (_fgAQ_Enable[AQ_TYPE_MTKDRC]))
        {
            /// Low-F Band
            u4Tmp1 = (_au1AQ_MTKDRC[1]<< 16)|(_au1AQ_MTKDRC[2]<<8)|(_au1AQ_MTKDRC[3]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_ATTACK_RATE, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_ATTACK_RATE, u4Tmp1, AUD_DEC_MAIN);
            u4Tmp1 = (_au1AQ_MTKDRC[4]<< 16)|(_au1AQ_MTKDRC[5]<<8)|(_au1AQ_MTKDRC[6]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_RELEASE_RATE, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_RELEASE_RATE, u4Tmp1, AUD_DEC_MAIN);
            u4Tmp1 = (_au1AQ_MTKDRC[7]<< 16)|(_au1AQ_MTKDRC[8]<<8)|(_au1AQ_MTKDRC[9]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_TARGET_LEVEL, u4Tmp1);
            //vWriteShmUINT32(D_DRC_RATIO, _au1AQ_MTKDRC[10]);
            _AUD_DspSetDrc2(DRC2_TARGET_LEVEL, u4Tmp1, AUD_DEC_MAIN);
            
            //_AUD_DspSetDrc2(DRC2_RATIO, _au1AQ_MTKDRC[10], AUD_DEC_MAIN);
            if (_au1AQ_MTKDRC[12] == 0xff)
            {    
                _AUD_DspSetDrc2(DRC2_RATIO, 0x7fffff, AUD_DEC_MAIN);    
            }
            else
            {
                _AUD_DspSetDrc2(DRC2_RATIO, (_au1AQ_MTKDRC[12]*0x7fff), AUD_DEC_MAIN);
            }
            /// High-F Band
            u4Tmp1 = (_au1AQ_MTKDRC[17]<< 16)|(_au1AQ_MTKDRC[18]<<8)|(_au1AQ_MTKDRC[19]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_ATTACK_RATE_1, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_ATTACK_RATE_1, u4Tmp1, AUD_DEC_MAIN);
            u4Tmp1 = (_au1AQ_MTKDRC[20]<< 16)|(_au1AQ_MTKDRC[21]<<8)|(_au1AQ_MTKDRC[22]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_RELEASE_RATE_1, u4Tmp1);
            _AUD_DspSetDrc2(DRC2_RELEASE_RATE_1, u4Tmp1, AUD_DEC_MAIN);     
            u4Tmp1 = (_au1AQ_MTKDRC[23]<< 16)|(_au1AQ_MTKDRC[24]<<8)|(_au1AQ_MTKDRC[25]);
            u4Tmp1 = (u4Tmp1<<8)>>8;
            //vWriteShmUINT32(D_DRC_TARGET_LEVEL_1, u4Tmp1);
            //vWriteShmUINT32(D_DRC_RATIO_1, _au1AQ_MTKDRC[21]);
            _AUD_DspSetDrc2(DRC2_TARGET_LEVEL_1, u4Tmp1, AUD_DEC_MAIN);
            //_AUD_DspSetDrc2(DRC2_RATIO_1, _au1AQ_MTKDRC[21], AUD_DEC_MAIN);
            if (_au1AQ_MTKDRC[28] == 0xff)
            {    
                _AUD_DspSetDrc2(DRC2_RATIO_1, 0x7fffff, AUD_DEC_MAIN);  
            }
            else
            {
                _AUD_DspSetDrc2(DRC2_RATIO_1, (_au1AQ_MTKDRC[28]*0x7fff), AUD_DEC_MAIN);
            }
            //vWriteShmUINT16(W_DRC_FLAG, _au1AQ_MTKDRC[9]);
            _AUD_DspSetDrc2(DRC2_HYSTERESIS, _au1AQ_MTKDRC[11], AUD_DEC_MAIN);
            _AUD_DspSetDrc2(DRC2_HYSTERESIS_1, _au1AQ_MTKDRC[27], AUD_DEC_MAIN);
            _AUD_DspSetDrc2(DRC2_FLAG, _au1AQ_MTKDRC[0], AUD_DEC_MAIN);
            
        }      
#endif //CC_AUD_DRC_V20
        break;    
#endif       
    default:
        break;
    };

}

void _AUD_DspSetBassTrebleOffset(UINT8 u1BassOffset, UINT8 u1TrebleOffset)
{
    //This API connect to CLI command only.
    _aai1BassTreble[0] = (INT8)(u1BassOffset - MID_BASS_TREBLE_OFFSET);
    _aai1BassTreble[1] = (INT8)(u1TrebleOffset - MID_BASS_TREBLE_OFFSET);
    _AUD_DspBassBoostGain(AUD_DEC_MAIN, _u1MWBassLevel);
    _AUD_DspClearBoostGain(AUD_DEC_MAIN, _u1MWTrebleLevel);        
}
void _AUD_DspGetBassTrebleOffset(UINT8 *u1BassOffset, UINT8 *u1TrebleOffset)
{
    //This API connect to CLI command only.
    *u1BassOffset = (UINT8)(_aai1BassTreble[0] + MID_BASS_TREBLE_OFFSET);
    *u1TrebleOffset =  (UINT8)(_aai1BassTreble[1] + MID_BASS_TREBLE_OFFSET);
}

#endif /* CC_AUD_USE_FLASH_AQ */

// =================== End of Audio Flash AQ Functions ===============================================


//-----------------------------------------------------------------------------
/** _AUD_DspGetMpegAudInfo
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param *prAudInfo                audio info structure pointer
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetMpegAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo)
{
    UNUSED(_GetMpegAudInfo(u1DecId, prAudInfo));
}

/** _AUD_GetAacVersion
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @retval version
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_GetAacVersion(UINT8 u1DecId)
{
    return u1GetAacVersion(u1DecId);
}

//-----------------------------------------------------------------------------
/** _AUD_GetAacTSFormat;
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @retval TSFormat
 */
//-----------------------------------------------------------------------------
UINT8 _AUD_GetAacTSFormat(UINT8 u1DecId)
{
    return u1GetAacTSFormat(u1DecId);
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetMpegAudInfoWithLayer
 *
 *  @param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param *prAudInfo                audio info structure pointer
 *  @param *prMPEGLayer           audio info structure pointer
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspGetMpegAudInfoWithLayer(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo, UINT8 *prMPEGLayer)
{
    UNUSED(_GetMpegAudInfoWithLayer(u1DecId, prAudInfo, prMPEGLayer));
}

#if 0   // Unused
//-----------------------------------------------------------------------------
/** _AUD_DspSetMp3PreparsingMode
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetMp3PreparsingMode(BOOL fgEnable)
{
    DSP_SetMp3PreparsingMode(fgEnable);
}

//-----------------------------------------------------------------------------
/** _AUD_DspSetMp3PreparsingMode
 *
 *  @param  enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspIsMp3Vbr(void)
{
    return DSP_IsMp3Vbr();
}
#endif

//-----------------------------------------------------------------------------
/** AUD_DspCOOKParameter
 *  Set dsp COOK
 *
 *  @param  u1DecId 0: first decoder 1: seconder decoder.
 *  @param  prCookInfo Wma header information
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspCOOKParameter(UINT8 u1DecId, AUD_COOK_CFG_T* prCookInfo)
{
    UNUSED(u1DecId);

    vWriteShmUINT16 (AUD_DSP0, W_COOK_SAMP_PER_FRAME, prCookInfo->u2SampPerFrame);
    vWriteShmUINT16 (AUD_DSP0, W_COOK_CHANNEL_NUM, prCookInfo->u2ChanNum);
    vWriteShmUINT16 (AUD_DSP0, W_COOK_INPUT_SAMPLING_RATE, prCookInfo->u2SampRate);
    vWriteShmUINT16 (AUD_DSP0, W_COOK_FRM_SZ_IN_BYTES, prCookInfo->u2FrameSzInBytes);
    vWriteShmUINT16 (AUD_DSP0, W_COOK_REGION_NUM, prCookInfo->u2RegnNum);
    vWriteShmUINT16 (AUD_DSP0, W_COOK_COUPLING_START_REGN, prCookInfo->u2StartRegn);
    vWriteShmUINT16 (AUD_DSP0, W_COOK_COUPLING_QBITS, prCookInfo->u2CplQBits);
}

#ifdef CC_AUD_SETUP_MELODY
void _AUD_DspSetMelodyVol(UINT8 u1Vol)
{
    _aau1ChannelVolume[AUD_DEC_MAIN][AUD_CH_ALL] = u1Vol;

}
#endif

#ifdef CC_AUD_MIXSOUND_SUPPORT

void _AUD_DspSetMixSndLoop(UINT32 u4Loop)
{
    UINT8 u1OldIdx = _u1MixSndClipIdx;

    _u1MixSndClipIdx = 0xff;
    _u4MixsoundLoop = u4Loop;
    _AUD_DspMixSndClip(u1OldIdx);
}

void _AUD_DspSetMixSndVol(UINT32 u4Vol)
{
    _u4MixsoundVol=u4Vol;
    vWriteShmUINT32(AUD_DSP0, D_MIXSOUND_GAIN, _au4VOLUME[u4Vol]); // 100 --> 0
}

UINT32 _AUD_DspGetMixSndLoop(void)
{
    return _u4MixsoundLoop;
}

UINT32 _AUD_DspGetMixSndVol(void)
{
    return _u4MixsoundVol;
}
#endif

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
void _AUD_DspSetMixSndLength(UINT32 u4Len)
{
    vWriteShmUINT32 (AUD_DSP0, D_MIXSOUND_PLEN, u4Len); // 24*64 12kHz
}

#ifndef NEW_MIXSOUND
void _AUD_DspSetMixSndGain(UINT32 u4Gain)
{
    vWriteShmUINT32 (AUD_DSP0, D_MIXSOUND_GAIN,   u4Gain); // 0x20000 --> 0dB
}

void _AUD_DspSetMixSndStereo(UINT8 bOnOff)
{
    vWriteShmUINT16(AUD_DSP0, W_MIXSOUND_CFG,(u2ReadShmUINT16(AUD_DSP0, W_MIXSOUND_CFG)&0xffef) | (bOnOff ? 0x10 : 0x00));
}

// 0: 1x
// 1: 2x
// 2: 4x
// 3: 8x
void _AUD_DspSetMixSndUpSampleRatio(UINT8 bRatio)
{
    vWriteShmUINT16(AUD_DSP0, W_MIXSOUND_CFG,(u2ReadShmUINT16(AUD_DSP0, W_MIXSOUND_CFG)&0xfffc) | bRatio);
}
#endif

UINT32 _AUD_DspGetMixSndGain(void)
{
    return u4ReadShmUINT32(AUD_DSP0, D_MIXSOUND_GAIN);
}

BOOL _AUD_DspGetMixSndStereo(void)
{
    if (u2ReadShmUINT16(AUD_DSP0, W_MIXSOUND_CFG)&0x10)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifndef NEW_MIXSOUND
void _AUD_DspSetMixSndMemPlay(UINT8 bOnOff)
{
    vWriteShmUINT16(AUD_DSP0, W_MIXSOUND_CFG,(u2ReadShmUINT16(AUD_DSP0, W_MIXSOUND_CFG)&0xffdf) | (bOnOff ? 0x20 : 0x00));
}
#endif

BOOL _AUD_DspIsMixSndPlay(void)
{
    return u2GetMixSoundStatus();
}

// 0x0100: Mix to L/R
// 0x0200: Mix to SL/SR
// 0x0400: Mix to Center
// 0x0800: Mix to CH7/CH8
// 0x1000: Mix to CH9/CH10
void _AUD_DspSetMixSndChannel(UINT16 wCh)
{
    vWriteShmUINT16(AUD_DSP0, W_MIXSOUND_CFG,(u2ReadShmUINT16(AUD_DSP0, W_MIXSOUND_CFG)&0xe0ff) | wCh);
    DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_MIXSOUND_FLAG);
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetWithVideo
 *   Set is with video
 *
 *  @param  u1DecId         Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  fgIsWithVideo   TRUE (with video). FALSE (no video)
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetWithVideo(UINT8 u1DecId, BOOL fgIsWithVideo)
{
    if (u1DecId > AUD_DEC_AUX)
    {
        return;
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        vWriteShmUINT8(AUD_DSP0, B_IS_WITH_VIDEO, (UINT8)fgIsWithVideo);
        DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_IS_WITH_VIDEO);
    }
    else
    {
        vWriteShmUINT8(AUD_DSP0, B_IS_WITH_VIDEO_DEC2, (UINT8)fgIsWithVideo);
        DSP_SendDspTaskCmd(AUD_DSP0, UOP_DSP_IS_WITH_VIDEO_DEC2);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DspIsWithVideo
 *   Is with video
 *
 *  @param  u1DecId         Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspIsWithVideo(UINT8 u1DecId)
{
    BOOL fgIsWithVideo;

    if (u1DecId > AUD_DEC_AUX)
    {
        return TRUE;
    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        fgIsWithVideo = (BOOL)(uReadShmUINT8(AUD_DSP0, B_IS_WITH_VIDEO));
    }
    else
    {
        fgIsWithVideo = (BOOL)(uReadShmUINT8(AUD_DSP0, B_IS_WITH_VIDEO_DEC2));
    }

    return fgIsWithVideo;
}

//-----------------------------------------------------------------------------
/** _AUD_DspAsrcSetBaseFreq
 *   Set ASRC base frequency
 *
 *  @param  u4BaseFreq    Freq Idx
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAsrcSetBaseFreq(UINT8 u1SrcID, UINT32 u4BaseFreq)
{
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    UINT8 u1Byte;
    INT32 i4Level;

#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DRVGetStreamFrom(AUD_DSP0, AUD_DEC_AUX, &eStreamFrom);
#else
    AUD_DRVGetStreamFrom(AUD_DSP0, AUD_DEC_MAIN, &eStreamFrom);
#endif

    if ((eStreamFrom != AUD_STREAM_FROM_DIGITAL_TUNER) && (!AUD_IsSourceFromWFD(AUD_DEC_MAIN)))
    {
        // Only for DTV and Wifi-Display case
        return;
    }

    VERIFY(x_sema_lock(_hAsrcControlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (u4BaseFreq == FS_48K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_48K;
    }
    else if (u4BaseFreq == FS_44K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_44K;
    }
    else if (u4BaseFreq == FS_32K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_32K;
    }
    else if (u4BaseFreq == FS_24K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_24K;
    }
    else if (u4BaseFreq == FS_22K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_22K;
    }
    else if (u4BaseFreq == FS_16K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_16K;
    }
    else if (u4BaseFreq == FS_12K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_12K;
    }
    else if (u4BaseFreq == FS_11K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_11K;
    }
    else if (u4BaseFreq == FS_8K)
    {
        _i4AsrcBaseFreq = ASRC_BASE_FREQ_8K;
    }

    i4Level = (_i4AsrcBaseFreq * ((UINT32)_i4AsrcSmartVCXOB2rtLevel & 0x0ff)) >> 20;
    u1Byte = (UINT8)((_i4AsrcSmartVCXOB2rtLevel >> 8) & 0x01);

    if (u1Byte == 1)
    {
        // Adjust IFS faster
        _i4AsrcSmartVCXOAdjustLevel = i4Level;
    }
    else
    {
        // Adjust IFS slower
        _i4AsrcSmartVCXOAdjustLevel = -i4Level;
    }

    _AUD_DspAsrcControl(u1SrcID);

    VERIFY(x_sema_unlock(_hAsrcControlSema) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** APLL adjust for WiFi-Display
 */
//-----------------------------------------------------------------------------
void AUD_WFD_DspAsrcSetSmartVCXO(UINT8 u1DecId)
{
    INT32 i4Level2;
    INT32 i4FinalLevel;
    UINT32 u4Pts;
    STC_CLOCK rStc;
    SAMPLE_FREQ_T  eFs;
    AUD_DEC_STREAM_FROM_T eStreamFrom;

    INT32 i4StcDiff = 0;
    UINT32 u4StcDiff = 0; 
    INT32 i4Ratio = 0;
    static INT32 i4PrevRatio = 0;
    UINT32 u4SramSrcBassAddr;

    AUD_DRVGetStreamFrom(AUD_DSP0, u1DecId, &eStreamFrom);
    eFs = AUD_GetSampleFreq(u1DecId);

    if (eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA)
    {
        // Only for MMP Wifi-Display case
        return;
    }

    if (STC_GetSrc(AUD_DspQryStcID(u1DecId), &rStc) == STC_VALID)  //Audio Description - we only have 1 STC
    {
        AUD_GetAudioPTS(u1DecId, &u4Pts);
        i4StcDiff = rStc.u4Base - u4Pts;
#ifdef CC_AUD_ARM_SUPPORT 
        i4StcDiff = _gi4StcDiff[u1DecId]; //stc - pts
#endif
        if (i4StcDiff == 0)
        {
            return;
        }
        u4StcDiff = (i4StcDiff >= 0) ? i4StcDiff : (-1* i4StcDiff);
        if (u4StcDiff < APLL_ADJUST_BOUND1)
        {
            i4Ratio = 0;
        }
        else if (u4StcDiff < APLL_ADJUST_BOUND2)
        {
            i4Ratio = 1;
        }
        else if (u4StcDiff < APLL_ADJUST_BOUND3)
        {
            i4Ratio = 2;
        }
        else if (u4StcDiff < APLL_ADJUST_BOUND4)
        {
            i4Ratio = 3;
        }
        else
        {
            i4Ratio = 0;
        }

        //i4StcDiff < 0: slow down
        //i4StcDiff > 0: speed up
        i4Ratio = (i4StcDiff >= 0) ? i4Ratio : (-1 * i4Ratio);
        LOG(2, "stc_diff = %d, ratio=%d\n", i4StcDiff, i4Ratio);

        if (i4PrevRatio != i4Ratio)
        {
            i4PrevRatio = i4Ratio;
            i4Level2= APLL2_PCW_ADJUST_LVL_48k +  i4Ratio * APLL2_PCW_ADJUST_LVL_48k / 100 ;
            bHDMIAudioSigmaDeltaFineTuneSetting(0x867c3ece, 0x3d, i4Level2, 0x1);  //fine tune the IEC output clk in DTV
            //APLL2 adjust
            switch (eFs)
            {
            case FS_44K:
#if defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
                i4Level2= APLL2_PCW_ADJUST_LVL_44k +  i4Ratio * APLL2_PCW_ADJUST_LVL_44k / 100 ;
#else
                if (IS_XTAL_27MHZ())
                {
                    i4Level2= 0x131205bc +  i4Ratio * 0x131205bc / 100 ;
                }
                else if (IS_XTAL_24MHZ())
                {
                    i4Level2= 0x159353f7 +  i4Ratio * 0x159353f7 / 100 ;
                }
                else
                {
                    i4Level2 = 0x131205bc; //default 27MHz
                }
#endif
                bHDMIAudioSigmaDeltaFineTuneSetting(0x205bc01a, 0x21, i4Level2 , 0x1);  //fine tune the IEC output clk in DTV
                break;
            case FS_48K:
            case FS_32K:
            default:
#if defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
                i4Level2= APLL2_PCW_ADJUST_LVL_48k +  i4Ratio * APLL2_PCW_ADJUST_LVL_48k / 100 ;
#else
                if (IS_XTAL_27MHZ())
                {
                    i4Level2= 0x14d867c3 +  i4Ratio * 0x14d867c3 / 100 ;
                }
                else if (IS_XTAL_24MHZ())
                {
                    i4Level2= 0x179374bc +  i4Ratio * 0x179374bc / 100 ;
                }
                else
                {
                    i4Level2 = 0x14d867c3; //default 27MHz
                }
#endif
                bHDMIAudioSigmaDeltaFineTuneSetting(0x867c3ece, 0x3d, i4Level2 , 0x1);  //fine tune the IEC output clk in DTV
                break;
            }

            //ASRC asjust
            i4FinalLevel = _i4AsrcBaseFreq + _i4AsrcBaseFreq*i4Ratio / 100 + _i4AsrcAVSyncAdjustLevel;

            LOG(7, "PTS=0x%x, STC=0x%x, Diff=%d, Ratio=%d\n", u4Pts, rStc.u4Base, u4StcDiff, i4Ratio);
            LOG(7, "ASRC Base=0x%x, Final=0x%x, APLL2 Final=0x%x\n", _i4AsrcBaseFreq,i4FinalLevel, i4Level2);
            
            u4SramSrcBassAddr = REG_DSP_SRC1_BASE;
#ifdef CC_MT5391_AUD_3_DECODER
            // Set Dec2/Dec3 IFS
            vWriteDspSram(AUD_DSP0, u4SramSrcBassAddr + REG_DSP_SRC_FREQUENCY_2_OFST, (UINT32)i4FinalLevel);
            if (_IsDualDecMode())
            {
                // Set Dec1 IFS
                vWriteDspSram(AUD_DSP0, u4SramSrcBassAddr + REG_DSP_SRC_FREQUENCY_0_OFST, (UINT32)i4FinalLevel);
            }
            else if(_IsTriOnlyDecMode())
            {
                vWriteDspSram(AUD_DSP0, u4SramSrcBassAddr + REG_DSP_SRC_FREQUENCY_3_OFST, (UINT32)i4FinalLevel);
            }
#else
            // Set Dec1 IFS
            vWriteDspSram(AUD_DSP0, u4SramSrcBassAddr + REG_DSP_SRC_FREQUENCY_2_OFST, (UINT32)i4FinalLevel);
#endif
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER) 
            i4FinalLevel -= _i4AsrcBaseFreq;
            if (u1DecId == AUD_DEC_MAIN)
            {
                u4SramSrcBassAddr = APROC_REG_AENV_SRC0_ADJ_OFFSET;
            }
            else if (u1DecId == AUD_DEC_AUX)
            {
                u4SramSrcBassAddr = APROC_REG_AENV_SRC1_ADJ_OFFSET;
            }
            else
            {
                u4SramSrcBassAddr = APROC_REG_AENV_SRC2_ADJ_OFFSET;
            }
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_0, u4SramSrcBassAddr), i4FinalLevel);
#endif      
        }
    }
}

/** _AUD_DspAsrcSetSmartVCXO
 *   Adjust ASRC SmartVCXO level
 *
 *  @param  u4AdjustLevel    video calculated level (0 ~ 255)
 *                     bit 8: 0 (slower), 1 (faster)
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------


void _AUD_DspAsrcSetSmartVCXO(UINT32 u4AdjustLevel)
{
    UINT8 u1Byte;
    UINT8 u1DecId= AUD_DEC_AUX;
    UINT16 u2StcId, u2DecStcId;
    INT32 i4Level;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    INT32 i4Levl2;
    SAMPLE_FREQ_T  eFs;

#ifdef CC_MT5391_AUD_3_DECODER
    if(_IsDualDecMode())
    {
        u1DecId = AUD_DEC_AUX;
    }
    else if(_IsTriOnlyDecMode())
    {
        u1DecId = AUD_DEC_THIRD;
    }
    else
    {
        u1DecId = AUD_DEC_AUX;
    }
    AUD_DRVGetStreamFrom(AUD_DSP0, u1DecId, &eStreamFrom);
    eFs = AUD_GetSampleFreq(u1DecId);
#else
    AUD_DRVGetStreamFrom(AUD_DEC_MAIN, &eStreamFrom);
    eFs = AUD_GetSampleFreq(AUD_DEC_MAIN);
#endif

    if (eStreamFrom != AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        // Only for DTV case
        return;
    }

    VERIFY(x_sema_lock(_hAsrcControlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _i4AsrcSmartVCXOB2rtLevel = u4AdjustLevel;
    i4Level = (_i4AsrcBaseFreq * (u4AdjustLevel & 0x0ff)) >> 20;
    u1Byte = (UINT8)((u4AdjustLevel >> 8) & 0x01);
    u2StcId = (UINT16)((u4AdjustLevel >> 9) & 0x01);

    AUD_DspGetStcId(u1DecId, &u2DecStcId);
    AUD_DRVGetStreamFrom(AUD_DSP0, AUD_DEC_MAIN, &eStreamFrom);  //we issue IEC clk fine tune when DTV source 
    if (u1Byte == 1)
    {
        // Adjust IFS faster
        _i4AsrcSmartVCXOAdjustLevel = i4Level;
    }
    else
    {
        // Adjust IFS slower
        _i4AsrcSmartVCXOAdjustLevel = -i4Level;
    }
    
    if ((eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) && (u2StcId == u2DecStcId))
    {
        switch (eFs)
        {
            case FS_44K:
#if defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)// Mustang && Remy is fix to 27MHz
                i4Levl2 = APLL2_PCW_ADJUST_LVL_44k +  _i4AsrcSmartVCXOAdjustLevel *( APLL2_PCW_ADJUST_LVL_44k/_i4AsrcBaseFreq) ;
#else
                if (IS_XTAL_27MHZ())
                {
                        i4Levl2 = 0x131205bc +  _i4AsrcSmartVCXOAdjustLevel *( 0x131205bc/_i4AsrcBaseFreq) ;
                }
                else if (IS_XTAL_24MHZ())
                {
                        i4Levl2 = 0x159353f7 +  _i4AsrcSmartVCXOAdjustLevel *( 0x159353f7/_i4AsrcBaseFreq) ;
                }
                else
                {
                    i4Levl2 = 0x131205bc; //default 27MHz
                }
#endif

                bHDMIAudioSigmaDeltaFineTuneSetting(0x205bc01a, 0x21, i4Levl2 , 0x1);  //fine tune the IEC output clk in DTV
                break;
            case FS_48K:
            case FS_32K:
            default:
#if defined(CC_MT5399) || defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)// Mustang && Remy is fix to 27MHz
                i4Levl2 = APLL2_PCW_ADJUST_LVL_48k +  _i4AsrcSmartVCXOAdjustLevel *( APLL2_PCW_ADJUST_LVL_48k/_i4AsrcBaseFreq) ;
#else
                if (IS_XTAL_27MHZ())
                {
                    i4Levl2 = 0x14d867c3 +  _i4AsrcSmartVCXOAdjustLevel *( 0x14d867c3/_i4AsrcBaseFreq) ;
                }
                else if (IS_XTAL_24MHZ())
                {
                    i4Levl2 = 0x179374bc +  _i4AsrcSmartVCXOAdjustLevel *( 0x179374bc/_i4AsrcBaseFreq) ;
                }
                else
                {
                    i4Levl2 = 0x14d867c3; // default 27MHz
                }
#endif

                bHDMIAudioSigmaDeltaFineTuneSetting(0x867c3ece, 0x3d, i4Levl2 , 0x1);  //fine tune the IEC output clk in DTV
                break;
        }
    }

    _AUD_DspAsrcControl(0);
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    _AUD_AprocAsrcControl(u1DecId);
#endif

    VERIFY(x_sema_unlock(_hAsrcControlSema) == OSR_OK);
}

void _AUD_DspAsrcControl(UINT8 u4SrcID)
{
    INT32 i4FinalLevel;
    UINT32 u4SramSrcBassAddr;

    i4FinalLevel = _i4AsrcBaseFreq + _i4AsrcSmartVCXOAdjustLevel + _i4AsrcAVSyncAdjustLevel;

    if (u4SrcID == 0)
    {
        u4SramSrcBassAddr = REG_DSP_SRC1_BASE;
    }
    else if (u4SrcID == 1)
    {
        u4SramSrcBassAddr = REG_DSP_SRC2_BASE;
    }
    else
    {
        u4SramSrcBassAddr = REG_DSP_SRC3_BASE;
    }
#ifdef CC_MT5391_AUD_3_DECODER
    // Set Dec2/Dec3 IFS
    vWriteDspSram(AUD_DSP0, u4SramSrcBassAddr + REG_DSP_SRC_FREQUENCY_0_OFST, (UINT32)i4FinalLevel);

    if (_IsDualDecMode())
    {
        // Set Dec1 IFS
        vWriteDspSram(AUD_DSP0, u4SramSrcBassAddr + REG_DSP_SRC_FREQUENCY_2_OFST, (UINT32)i4FinalLevel);
    }
    else if(_IsTriOnlyDecMode())
    {
        vWriteDspSram(AUD_DSP0, u4SramSrcBassAddr + REG_DSP_SRC_FREQUENCY_3_OFST, (UINT32)i4FinalLevel);
    }
#else
    // Set Dec1 IFS
    vWriteDspSram(AUD_DSP0, u4SramSrcBassAddr + REG_DSP_SRC_FREQUENCY_2_OFST, (UINT32)i4FinalLevel);
#endif
}

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
void _AUD_AprocAsrcControl(UINT8 u1DecID)
{
    INT32 i4FinalLevel;
    UINT32 u4SramSrcBassAddr;

    i4FinalLevel = _i4AsrcSmartVCXOAdjustLevel;

    if (u1DecID == AUD_DEC_MAIN)
    {
        u4SramSrcBassAddr = APROC_REG_AENV_SRC0_ADJ_OFFSET;
    }
    else if (u1DecID == AUD_DEC_AUX)
    {
        u4SramSrcBassAddr = APROC_REG_AENV_SRC1_ADJ_OFFSET;
    }
    else
    {
        u4SramSrcBassAddr = APROC_REG_AENV_SRC2_ADJ_OFFSET;
    }

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_0, u4SramSrcBassAddr), i4FinalLevel);
    //LOG(0, "(%d)AprocVCXO : 0x%x\n", u4SrcID, i4FinalLevel);

}

void _AUD_AprocEnableVCXO(UINT8 u1DecID, BOOL fgEnable)
{
    UINT32 u4AsmAddr;
    
    if (u1DecID == AUD_DEC_MAIN)
    {
        u4AsmAddr = APROC_REG_AENV_SRC0_ADJ_SWITCH;
    }
    else if (u1DecID == AUD_DEC_AUX)
    {
        u4AsmAddr = APROC_REG_AENV_SRC1_ADJ_SWITCH;
    }
    else
    {
        u4AsmAddr = APROC_REG_AENV_SRC2_ADJ_SWITCH;
    }

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_0, u4AsmAddr), (UINT32)fgEnable);    
}

void _AUD_AprocAsrcTrackingMode(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    //APROC Tracking Mode.
    if (u1DecId == AUD_DEC_MAIN)
    {
        vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AENV_0, APROC_REG_AENV_SRC0_FREQMODE), (UINT32)fgEnable);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AENV_1, APROC_REG_AENV_SRC0_FREQMODE), (UINT32)fgEnable); 
    }
    else
    {
        vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AENV_2, APROC_REG_AENV_SRC0_FREQMODE), (UINT32)fgEnable); 
    } 
}
#endif //CC_AUD_ARM_SUPPORT

//-----------------------------------------------------------------------------
/** _AUD_DspAsrcResetSmartVCXO
 *   Reset ASRC SmartVCXO
 *
 *  @param  void
 *
 *  @retval  void
 */
//-----------------------------------------------------------------------------
void _AUD_DspAsrcResetSmartVCXO(void)
{
    _i4AsrcSmartVCXOAdjustLevel = 0;
    _i4AsrcSmartVCXOB2rtLevel = 0;
}

#if defined(CC_SUPPORT_STR) || defined(CC_FAST_INIT) || defined(__MODEL_slt__)
void AUD_DspCfgSuspend(void)
{
    VERIFY(x_sema_unlock(_ahVolCtlSema[AUD_DEC_MAIN]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahVolCtlSema[AUD_DEC_AUX]) == OSR_OK);

#ifdef CC_MT5391_AUD_3_DECODER
    VERIFY(x_sema_unlock(_ahVolCtlSema[AUD_DEC_THIRD]) == OSR_OK);
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
    VERIFY(x_sema_unlock(_ahVolCtlSema[AUD_DEC_4TH]) == OSR_OK);
#endif
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspSetCapability
 *   Set Capability
 *
 *  @param      void
 *  @retval     void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetCapability(void)
{
#ifdef CC_SUPPORT_STR
    static BOOL fgInit = FALSE;

    if (!fgInit)
    {
#endif
        DSP_SetCapability();
#ifdef CC_SUPPORT_STR
        fgInit = TRUE;
    }
#endif
}

//-----------------------------------------------------------------------------
/** _AUD_DspGetCapability
 *   Get Capability
 *
 *  @param      void
 *  @retval     format capability
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspGetCapability(UINT8 u1Idx)
{
    return DSP_GetCapability(u1Idx);
}

//-----------------------------------------------------------------------------
/** _AUD_DspIsFormatValid
 *  Is format valid
 *
 *  @param      u4FormatInfo: format index
 *  @retval     TRUE(valid). FALSE(invalid)
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DspIsFormatValid(UINT32 u4FormatInfo)
{
    return DSP_IsFormatValid(u4FormatInfo);
}

BOOL _AUD_DspIsSupportAC3(void)
{
    return DSP_IsSupportAC3();
}

BOOL _AUD_DspIsSupportEAC3(void)
{
    return DSP_IsSupportEAC3();
}

#if 0   // Unused
BOOL _AUD_DspIsSupportDDCO(void)
{
    return DSP_IsSupportDDCO();
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DspQueryChannelDelay
 *  Query current channel delay.
 *
 *  @param      u1ChId:
 *  @retval     Delay Value in ms
 */
//-----------------------------------------------------------------------------
UINT32 _AUD_DspQueryChannelDelay(UINT8 u1ChId) //QUERY_DELAY
{
    UNUSED(_AUD_DspQueryChannelDelay);

    return DSP_GetChannelDelay(u1ChId);
}

UINT32 _AUD_DspGetMasterVolRawValue(UINT8 u1DecId)
{
    return _au4VOLUME[_aau1ChannelVolume[u1DecId][AUD_CH_ALL]];
}

void AUD_DspMutePcmOut(BOOL fgEnable)
{
    if (fgEnable)
    {
        vWriteShmUINT16(AUD_DSP0, W_SYSTEM_SETUP, u2ReadShmUINT16(AUD_DSP0, W_SYSTEM_SETUP)|0x2000);
    }
    else
    {
        vWriteShmUINT16(AUD_DSP0, W_SYSTEM_SETUP, u2ReadShmUINT16(AUD_DSP0, W_SYSTEM_SETUP)&0xdfff);
    }
}

void AUD_DspFastDecode(BOOL fgEnable)
{
    if (fgEnable)
    {
        vWriteShmUINT16(AUD_DSP0, W_SYSTEM_SETUP, u2ReadShmUINT16(AUD_DSP0, W_SYSTEM_SETUP)|0x0080);
        // Set Fast Stop On
        vWriteShmUINT16(AUD_DSP0, W_SYSTEM_CTRL2, u2ReadShmUINT16(AUD_DSP0, W_SYSTEM_CTRL2)|0x0200);
    }
    else
    {
        vWriteShmUINT16(AUD_DSP0, W_SYSTEM_SETUP, u2ReadShmUINT16(AUD_DSP0, W_SYSTEM_SETUP)&0xff7f);
        // Set Fast Stop Off
        vWriteShmUINT16(AUD_DSP0, W_SYSTEM_CTRL2, u2ReadShmUINT16(AUD_DSP0, W_SYSTEM_CTRL2)&~0x0200);
    }
}

#if 0 // 2_STC_Task_DEBUG_only
//Fixmed.... This API should move to DMX Driver.
#include "x_bim.h"
#define DMXCMD_READ32(offset)           IO_READ32(DEMUX0_BASE, ((offset) * 4))
void _DMX_FIXME_ReadSTC(UINT8 u1StcId, UINT32* u4StcBase)
{
    *u4StcBase = 0;

    switch (u1StcId)
    {
    case 0:
         *u4StcBase = PARSER_READ32(REG_A_STC_1);
         break;
    case 1:
        *u4StcBase = DMXCMD_READ32(143); //0x23c
         break;
    case 2:
        *u4StcBase = DMXCMD_READ32(163); //0x28c Need ECO IC
         break;
    }
}
#endif

#if 0 //Use DMX API, don't access DMX register directly.
#define DMXCMD_WRITE32(offset, value)   IO_WRITE32(DEMUX0_BASE, ((offset) * 4), (value))
void _DMX_SwitchStcForAudio(UINT8 u1StcId)
{
    UINT32 u4Value = 0;

    u4Value = DMXCMD_READ32(129);
    u4Value = u4Value & 0xFFFFFFFC;  // bit 0 & 1

    // Select DMX STC_1 or DMX_STC_2
    if (u1StcId == 1) //STC_1 : ID=0
        DMXCMD_WRITE32(129, ((u4Value)|2) ); // 129 * 4 =0x204
    else              //STC_2 : ID=1
        DMXCMD_WRITE32(129, ((u4Value)|3) ); // 129 * 4 =0x204
}
#endif

#ifdef CC_SUPPORT_2STC //<--- Enable this when other module submit code.
extern BOOL DMX_SetStcToDspUpdate(UINT8 u1Id, BOOL fgEnable); //DMX_STC_1 : ID=0, DMX_STC_2 : ID=1
#endif
//-----------------------------------------------------------------------------
/** _AUD_DspSetStcId
 *  Set Stc ID for audio / DSP
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DspSetStcId(UINT8 u1DecId, UINT16 u2StcId) // 2_STC_Task
{
    UINT32 u4UopIndex = UOP_DSP_SET_STC_ID;
    UINT32 u4ShmIndex = W_SYSTEM_SETUP;
    UINT16 u2Config = 0;

    UNUSED(_AUD_DspSetStcId);

    // UOP according to Decoder ID.
    if (u1DecId == AUD_DEC_MAIN)
    {
        u4UopIndex = UOP_DSP_SET_STC_ID;
        u4ShmIndex = W_SYSTEM_SETUP;
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u4UopIndex = UOP_DSP_SET_STC_ID_DEC2;
        u4ShmIndex = W_SYSTEM_SETUP_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4UopIndex = UOP_DSP_SET_STC_ID_DEC3;
        u4ShmIndex = W_SYSTEM_SETUP_DEC3;
    }
    else
    {
        LOG(3, "_AUD_DspSetStcId : Invalid Decoder ID %d \n", u1DecId);
        return;
    }

    // Set STC ID. default value is Parser STC.
    // STC for System : 0 means AUDIO, 1 means Demux. (There are 3 STCs, STC_A, demux STC1, demux STC2)
    // DSP system_setup bit[14:13] : 1x-audio,  00-demux STC1, 01-demux STC2
    u2Config = (u2ReadShmUINT16(AUD_DSP0, u4ShmIndex) & 0x9FFF); //clear bit [14:13]
    if (u2StcId == AUD_STC_AUDIO)
    {
        u2Config |= 0x2 << 13;
    }
    else if (u2StcId == AUD_STC_DMX)
    {
        u2Config |= 0x1 << 13;
        //_DMX_SwitchStcForAudio(1); // Enable Audio Update DMX_STC_1, should use DMX API
        #ifdef CC_SUPPORT_2STC //<--- Enable this when other module submit code.
        //DMX_SetStcToDspUpdate(1, TRUE);  // Enable Audio Update DMX_STC_1 (ID=0.)
        // no need after MT5890
        #endif
    }
    else if (u2StcId == AUD_STC_DMX2)
    {
        LOG(3, "_AUD_DspSetStcId : Dec(%d) Invalid STC ID %d \n", u1DecId, u2StcId);
        LOG(3, "AUD_STC_DMX2 can't be st by MW now. \n");
        u2Config |= 0x2 << 13;
    }
    else
    {
        LOG(3, "_AUD_DspSetStcId : Dec(%d) Invalid STC ID %d \n", u1DecId, u2StcId);
        u2Config |= 0x2 << 13;
    }

    LOG(3, " _AUD_DspSetStcId, Set Dec(%d) STC ID to %d\n", u1DecId, u2StcId);
    vWriteShmUINT16(AUD_DSP0, u4ShmIndex, u2Config);
    DSP_SendDspTaskCmd(AUD_DSP0, u4UopIndex);

    _au1DspStcId[u1DecId] = (AUD_STC_ID)u2StcId;

}

UINT8 _AUD_DspQryStcID(UINT8 u1DecId)   // 2_STC_Task
{
    if (u1DecId > AUD_DEC_THIRD)
    {
        LOG(3, "_AUD_DspQryStcID : Invalid Decoder ID %d \n", u1DecId);
        return 0;
    }
    return (UINT8)_au1DspStcId[u1DecId];
}
void _AUD_DspGetStcId(UINT8 u1DecId, UINT16 *u2StcId) // 2_STC_Task
{
    UINT32 u4ShmIndex = W_SYSTEM_SETUP;
    UINT16 u2Config = 0;

    UNUSED(_AUD_DspGetStcId);

    // UOP according to Decoder ID.
    if (u1DecId == AUD_DEC_MAIN)
    {
        u4ShmIndex = W_SYSTEM_SETUP;
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u4ShmIndex = W_SYSTEM_SETUP_DEC2;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4ShmIndex = W_SYSTEM_SETUP_DEC3;
    }
    else
    {
        LOG(3, "_AUD_DspGetStcId : Invalid Decoder ID %d \n", u1DecId);
        return;
    }

    u2Config = (u2ReadShmUINT16(AUD_DSP0, u4ShmIndex) & 0x6000)>>13; //bit [14:13]

    // // STC for System : 0 means AUDIO, 1 means Demux STC1, 2 mean Demux STC2.
    // DSP system_setup bit[14:13] : 2-audio,  0-demux STC1, 1-demux STC2
    if (u2Config == 0)
    {
        *u2StcId = AUD_STC_DMX2; // ?????
    }
    else if (u2Config == 1)
    {
        *u2StcId = AUD_STC_DMX;  //AUD_STC_DMX2 -> AUD_STC_DMX for legacy heritage
    }
    else
    {
        *u2StcId = AUD_STC_AUDIO;
    }

}

void AUD_SoftwareMuteQuery(void)
{
    UINT8 i;

    Printf("\nChannel Outport Setting: <0: close; 1: open>\n");
    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {
        Printf("    _aafgChlOutPort[DEC0][%d] = %d \n", i, _aafgChlOutPort[AUD_DEC_MAIN][i]);
    }

    for ( i=0; i< AUD_CH_REAR_LEFT ;i++)
    {
        Printf("    _aafgChlOutPort[DEC1][%d] = %d \n", i, _aafgChlOutPort[AUD_DEC_AUX][i]);
    }

    Printf("\nSoftware mute control flag: <0: unmute; 1: mute>\n");

    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {
        Printf("    _aafgChlMute[DEC0][%d] = %d \n", i, _aafgChlMute[AUD_DEC_MAIN][i]);
    }
    printf("\n");
    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {
        Printf("    _aafgChlHdmiChgFmtMute[DEC0][%d] = %d \n",i, _aafgChlHdmiChgFmtMute[AUD_DEC_MAIN][i]);
    }
    printf("\n");

    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {
        Printf("    _aafgChDecPlayMute[DEC0][%d] = %d \n", i,_aafgChDecPlayMute[AUD_DEC_MAIN][i]);
    }
    printf("\n");

    for (i=0; i< AUD_DEC_NUM; i++)
    {

        Printf("    _afgMuteEnable[DEC%d] = %d \n", i,_afgMuteEnable[i]);

    }

    printf("\n");

    for (i=0; i< AUD_DEC_NUM; i++)
    {
        Printf("    _afgHdmiModeMuteEnable[DEC%d] = %d \n", i,_afgHdmiModeMuteEnable[i]);
    }

    printf("\n");

    for (i=0; i< AUD_DEC_NUM; i++)
    {
        Printf("    _afgStopMuteEnable[DEC%d] = %d \n", i,_afgStopMuteEnable[i]);
    }

    printf("\n");

    for (i=0; i< AUD_DEC_NUM; i++)
    {
        Printf("    _afgAllChannelMute[DEC%d] = %d \n", i,_afgAllChannelMute[i]);
    }

    printf("\n");


    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {
         Printf("    _aafgSoundModeMute[DEC0][%d] = %d \n", i, _aafgSoundModeMute[AUD_DEC_MAIN][i]);
    }
    printf("\n");

    for ( i=0; i< AUD_CH_REAR_LEFT ;i++)
    {
        Printf("    _aafgSoundModeMute[DEC1][%d] = %d \n", i, _aafgSoundModeMute[AUD_DEC_AUX][i]);
    }
    printf("\n");
    
    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {  
         Printf("    _aafgSurroundMute[DEC0][%d] = %d \n", i, _aafgSurroundMute[AUD_DEC_MAIN][i]);
    }
    printf("\n");

    for ( i=0; i< AUD_CH_REAR_LEFT ;i++)
    {
        Printf("    _aafgSurroundMute[DEC1][%d] = %d \n", i, _aafgSurroundMute[AUD_DEC_AUX][i]);  
    }
    printf("\n");

    for ( i=0; i< AUD_CH_AUX_FRONT_LEFT ;i++)
    {  
         Printf("    _aafgVBassMute[DEC0][%d] = %d \n", i, _aafgVBassMute[AUD_DEC_MAIN][i]);
    }
    printf("\n");

    for ( i=0; i< AUD_CH_REAR_LEFT ;i++)
    {
        Printf("    _aafgVBassMute[DEC1][%d] = %d \n", i, _aafgVBassMute[AUD_DEC_AUX][i]);  
    }
    printf("\n");
    
}

#ifdef CC_ENABLE_AOMX
void _AUD_DspPcmVolume(UINT8 u1Value, UINT8 u1Value1)
{
    UINT32 u4Volumn;
    UINT64 u8ChlVol;

    if (u1Value >157)//actually not large than +30dB
    {
        u1Value = 157;
    }

    if (u1Value1 >15)
    {
        u1Value1 = 15;
    }
    
    u8ChlVol = ((UINT64)_au4CHVOLUMENEW[u1Value]) * ((UINT64)_au4CHVOLUMENEW1[u1Value1]);
    u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);   
    AUD_DspSetGstGain(u4Volumn);
}
#endif

#ifdef CC_AUD_ARM_SUPPORT
//#A0007
#ifndef CC_AUD_ARM_RENDER

#if 0
/* Audio processor Volume set
*/
static void _vAproc_PostProcVol_Set (UINT32 u4Item, UINT32 *pu4Addr)
{
    UINT16 u2Idx;

    if (u4Item == 0)
    {
        u2Idx = D_APROC_POSTPROC_VOL0;
    }
    else
    {
        u2Idx = D_APROC_POSTPROC_VOL1;
    }

    vWriteShmUINT32 (AUD_DSP0, u2Idx, *pu4Addr);

    DSP_SendDspTaskCmd (AUD_DSP0, UOP_DSP_APROC_POSTPROC_VOL);
}

/* Audio processor Volume get
*/
static void _vAproc_PostProcVol_Get (UINT32 u4Item, UINT32 *pu4Addr)
{
    UINT16 u2Idx;

    if (u4Item == 0)
    {
        u2Idx = D_APROC_POSTPROC_VOL0;
    }
    else
    {
        u2Idx = D_APROC_POSTPROC_VOL1;
    }

    *pu4Addr = u4ReadShmUINT32 (AUD_DSP0, u2Idx);
}
#endif

 void _vAUD_Aproc_Set (UINT32 u4Type, UINT32 u4Item, UINT32 *pu4Addr, UINT32 u4Num)
{
    UNUSED(_vAUD_Aproc_Set);
    UNUSED(u4Type);    
    UNUSED(u4Item);
    UNUSED(pu4Addr);
    UNUSED(u4Num);    
}

void _vAUD_Aproc_Get (UINT32 u4Type, UINT32 u4Item, UINT32 *pu4Addr, UINT32 u4Num)
{
    UNUSED(_vAUD_Aproc_Get);
    UNUSED(u4Type);    
    UNUSED(u4Item);
    UNUSED(pu4Addr);
    UNUSED(u4Num);    
}
#endif // NOTdef CC_AUD_ARM_RENDER
#endif //CC_AUD_ARM_SUPPORT//#A0007

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER) //#A0005
void _vAprocSetRoutine (UINT32 u4Id)
{
    UINT32 u4Idx;

    switch (u4Id)
    {
    case APROC_ROUTINE_ID_ER_SP_PATH:
        u4Idx = APROC_SIG_SET_SP_PATH_ROUTINE;
        break;
    case APROC_ROUTINE_ID_ER_SP_LSRS_PATH:
        u4Idx = APROC_SIG_SET_SP_LSRS_PATH_ROUTINE;
        break;         
    case APROC_ROUTINE_ID_ER_HP_PATH:
        u4Idx = APROC_SIG_SET_HP_PATH_ROUTINE;
        break;
    case APROC_ROUTINE_ID_DR_SP_PATH:
        u4Idx = APROC_SIG_SET_DELAY0_ROUTINE;
        break;  
    case APROC_ROUTINE_ID_DR_HP_PATH:
        u4Idx = APROC_SIG_SET_DELAY1_ROUTINE;
        break;          
    case APROC_ROUTINE_ID_DR_SPDIF_SOUNDBAR_PATH:
        u4Idx = APROC_SIG_SET_DELAY2_ROUTINE;
        break;
    case APROC_ROUTINE_ID_DR_INPUT0_PATH:
        u4Idx = APROC_SIG_SET_DELAY_INPUT0_ROUTINE;
        break;  
    case APROC_ROUTINE_ID_DR_INPUT1_PATH:
        u4Idx = APROC_SIG_SET_DELAY_INPUT1_ROUTINE;
        break;  
    case APROC_ROUTINE_ID_DR_SPDIF_PCM_PATH:
        u4Idx = APROC_SIG_SET_DELAY6_ROUTINE;
        break;
    case APROC_ROUTINE_ID_CR_DMIX0_PATH:
        u4Idx = APROC_SIG_SET_DMIX0_ROUTINE;
        break;
    case APROC_ROUTINE_ID_CR_DMIX1_PATH:
        u4Idx = APROC_SIG_SET_DMIX1_ROUTINE;
        break;        
    case APROC_ROUTINE_ID_CR_DMIX2_PATH:
        u4Idx = APROC_SIG_SET_DMIX2_ROUTINE;
        break;  
    case APROC_ROUTINE_ID_AER_AMIXER0_PATH:
        u4Idx = APROC_SIG_SET_AMIXER0_ROUTINE;
        break;     
    case APROC_ROUTINE_ID_AER_AMIXER1_PATH:
        u4Idx = APROC_SIG_SET_AMIXER1_ROUTINE;
        break;     
    case APROC_ROUTINE_ID_AER_AMIXER2_PATH:
        u4Idx = APROC_SIG_SET_AMIXER2_ROUTINE;
        break;         
    default:
        LOG (5, "_vAprocSetRoutine: wrong id = 0x%x!!!\n", u4Id);
        return;
    }

    //vAprocCmdSet (APROC_SIG_IDX_POSTPROC, u4Idx, NULL);
    vAprocCmdSet (APROC_SIG_IDX_POSTPROC, u4Idx, 0);
}


static void _vAproc_PostProcSgain_Set (UINT32 u4Id, INT32 i4Value)
{
    UINT32 u4Addr;

    switch (u4Id)
    {
    case APROC_IOCTRL_SGAIN_DSP_PERIOD:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_0, APROC_REG_DSP_TRIMVOL_PERIOD);
        break;
    case APROC_IOCTRL_SGAIN_AMIXER_PERIOD:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_0, APROC_REG_AMIXER_TRIMVOL_PERIOD);
        break;
    case APROC_IOCTRL_SGAIN_MUTE_AMIXER0_PERIOD:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, APROC_REG_MUTE_AMIXER0_PERIOD);
        break;
    case APROC_IOCTRL_SGAIN_MUTE_AMIXER1_PERIOD:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, APROC_REG_MUTE_AMIXER1_PERIOD);
        break;
    case APROC_IOCTRL_SGAIN_MUTE_AMIXER2_PERIOD:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, APROC_REG_MUTE_AMIXER2_PERIOD);
        break;    
    default:
        LOG (5, "_vAproc_PostProcSgain_Set: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    vAprocReg_Write (u4Addr, i4Value);
}

static void _vAproc_PostProcSgain_Get (UINT32 u4Id, UINT32 *pu4Value)
{
    UINT32 u4Addr;

    switch (u4Id)
    {
    case APROC_IOCTRL_SGAIN_DSP_PERIOD:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_0, APROC_REG_DSP_TRIMVOL_PERIOD);
        break;
    case APROC_IOCTRL_SGAIN_AMIXER_PERIOD:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_0, APROC_REG_AMIXER_TRIMVOL_PERIOD);
        break;
    case APROC_IOCTRL_SGAIN_MUTE_AMIXER0_PERIOD:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, APROC_REG_MUTE_AMIXER0_PERIOD);
        break;
    case APROC_IOCTRL_SGAIN_MUTE_AMIXER1_PERIOD:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, APROC_REG_MUTE_AMIXER1_PERIOD);
        break;
    case APROC_IOCTRL_SGAIN_MUTE_AMIXER2_PERIOD:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, APROC_REG_MUTE_AMIXER2_PERIOD);
        break;    
    default:
        LOG (5, "_vAproc_PostProcSgain_Get: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    *pu4Value = u4AprocReg_Read (u4Addr);
}


static void _vAproc_PostProcVol_Set (UINT32 u4Id, INT32 i4Value)
{
    UINT32 u4Idx;

    switch (u4Id)
    {
    case APROC_IOCTRL_VOL_SP:
        u4Idx = APROC_REG_SP_VOL;
        break;
    case APROC_IOCTRL_VOL_SP_LSRS:
        u4Idx = APROC_REG_SP_LSRS_VOL;
        break;             
    case APROC_IOCTRL_VOL_SPSW:
        u4Idx = APROC_REG_SPSW_VOL;
        break;
    case APROC_IOCTRL_VOL_HP:
        u4Idx = APROC_REG_HP_VOL;
        break;
    case APROC_IOCTRL_VOL_MON:
        u4Idx = APROC_REG_MON_VOL;
        break;
    case APROC_IOCTRL_VOL_IEC:
        u4Idx = APROC_REG_IEC_VOL;
        break;    
    case APROC_IOCTRL_VOL_SCART:
        u4Idx = APROC_REG_SCART_VOL;
        break;
    case APROC_IOCTRL_VOL_BT:
        u4Idx = APROC_REG_BT_VOL;
        break;    
    case APROC_IOCTRL_VOL_AMIXER0:
        u4Idx = APROC_REG_AMIXER0_VOL;
        break;
    case APROC_IOCTRL_VOL_AMIXER1:
        u4Idx = APROC_REG_AMIXER1_VOL;
        break;    
    case APROC_IOCTRL_VOL_AMIXER2:
        u4Idx = APROC_REG_AMIXER2_VOL;
        break;  
    default:
        LOG (5, "_vAproc_PostProcVol_Set: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_0, u4Idx), i4Value);
}

static void _vAproc_PostProcVol_Get (UINT32 u4Id, INT32 *pi4Value)
{
    UINT32 u4Idx;

    switch (u4Id)
    {
    case APROC_IOCTRL_VOL_SP:
        u4Idx = APROC_REG_SP_VOL;
        break;
    case APROC_IOCTRL_VOL_SP_LSRS:
        u4Idx = APROC_REG_SP_LSRS_VOL;
        break;        
    case APROC_IOCTRL_VOL_SPSW:
        u4Idx = APROC_REG_SPSW_VOL;
        break;
    case APROC_IOCTRL_VOL_HP:
        u4Idx = APROC_REG_HP_VOL;
        break;
    case APROC_IOCTRL_VOL_MON:
        u4Idx = APROC_REG_MON_VOL;
        break;
    case APROC_IOCTRL_VOL_IEC:
        u4Idx = APROC_REG_IEC_VOL;
        break;    
    case APROC_IOCTRL_VOL_SCART:
        u4Idx = APROC_REG_SCART_VOL;
        break;
    case APROC_IOCTRL_VOL_BT:
        u4Idx = APROC_REG_BT_VOL;
        break;    
    case APROC_IOCTRL_VOL_AMIXER0:
        u4Idx = APROC_REG_AMIXER0_VOL;
        break;
    case APROC_IOCTRL_VOL_AMIXER1:
        u4Idx = APROC_REG_AMIXER1_VOL;
        break;    
    case APROC_IOCTRL_VOL_AMIXER2:
        u4Idx = APROC_REG_AMIXER2_VOL;
        break;  
    default:
        LOG (9, "_vAproc_PostProcVol_Set: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    *pi4Value = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_0, u4Idx));
}

static void _vAproc_PostProcTrim_Set (UINT32 u4Id, INT32 i4Value)
{
    UINT32 u4Idx;

    switch (u4Id)
    {
    case APROC_IOCTR_TRIM_INPUT0:
        u4Idx = APROC_REG_INPUT0_TRIM;
        break;
    case APROC_IOCTR_TRIM_INPUT1:
        u4Idx = APROC_REG_INPUT1_TRIM;
        break;    
    case APROC_IOCTR_TRIM_INPUT2:
        u4Idx = APROC_REG_INPUT2_TRIM;
        break;         
    case APROC_IOCTR_TRIM_SP:
        u4Idx = APROC_REG_SP_TRIM;
        break;
    case APROC_IOCTR_TRIM_SPLSRS:
        u4Idx = APROC_REG_SP_LSRS_TRIM;
        break;
    case APROC_IOCTR_TRIM_SPSW:
        u4Idx = APROC_REG_SPSW_TRIM;
        break;
    case APROC_IOCTR_TRIM_IEC:
        u4Idx = APROC_REG_IEC_TRIM;
        break;
    case APROC_IOCTR_TRIM_HP:
        u4Idx = APROC_REG_HP_TRIM;
        break;
    case APROC_IOCTR_TRIM_MON:
        u4Idx = APROC_REG_MON_TRIM;
        break;    
    case APROC_IOCTR_TRIM_SCART:
        u4Idx = APROC_REG_SCART_TRIM;
        break;    
    case APROC_IOCTR_TRIM_BT:
        u4Idx = APROC_REG_BT_TRIM;
        break;
    case APROC_IOCTR_TRIM_AMIXER0:
        u4Idx = APROC_REG_AMIXER0_TRIM;
        break;
    case APROC_IOCTR_TRIM_AMIXER1:
        u4Idx = APROC_REG_AMIXER1_TRIM;
        break;  
    case APROC_IOCTR_TRIM_AMIXER2:
        u4Idx = APROC_REG_AMIXER2_TRIM;
        break;        
    case APROC_IOCTR_TRIM_DELAY0:
        u4Idx = APROC_REG_DELAY0_TRIM;
        break;  
    case APROC_IOCTR_TRIM_DELAY1:
        u4Idx = APROC_REG_DELAY1_TRIM;
        break;       
    default:
        LOG (9, "_vAproc_PostProcTrim_Set: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_0, u4Idx), i4Value);
}
static void _vAproc_PostProcBalance_Set (UINT32 u4Id, INT32 i4Value)
{
    UINT32 u4Idx;
    UINT32 u4v0, u4v1;

    switch (u4Id)
    {
    case APROC_IOCTR_BALANCE_SP:
        u4Idx = APROC_REG_SP_BALANCE_0;
        break;
    case APROC_IOCTR_BALANCE_HP:
        u4Idx = APROC_REG_HP_BALANCE_0;
        break;    
    case APROC_IOCTR_BALANCE_IEC:
        u4Idx = APROC_REG_IEC_BALANCE_0;
        break;                
    case APROC_IOCTR_BALANCE_INPUT0:
        u4Idx = APROC_REG_INPUT0_BALANCE_L;
        break;
    case APROC_IOCTR_BALANCE_INPUT1:
        u4Idx = APROC_REG_INPUT1_BALANCE_L;
        break;
    case APROC_IOCTR_BALANCE_INPUT2:
        u4Idx = APROC_REG_INPUT2_BALANCE_L;
        break;
    default:
        LOG (5, "_vAproc_PostProcBalance_Set: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    if ( i4Value > 50)
    {
        u4v0 = (100 - i4Value)* 0x7fffff / 50;
        u4v1 = 0x7fffff;
    }
    else
    {
        u4v1 = (i4Value)* 0x7fffff / 50;
        u4v0 = 0x7fffff;
    }

    LOG (5, "_vAproc_PostProcBalance_Set: Value = %d v0= 0x%x!, v1= 0x%x!\n", i4Value, u4v0, u4v1);
    
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_1, u4Idx), u4v0);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_1, u4Idx+1), u4v1);
}

static void _vAproc_PostProcBalance_Get (UINT32 u4Id, INT32 *pi4Value)
{
    UINT32 u4Idx;

    switch (u4Id)
    {
    case APROC_IOCTR_BALANCE_SP:
        u4Idx = APROC_REG_SP_BALANCE_0;
        break;
    case APROC_IOCTR_BALANCE_HP:
        u4Idx = APROC_REG_HP_BALANCE_0;
        break;    
    case APROC_IOCTR_BALANCE_IEC:
        u4Idx = APROC_REG_IEC_BALANCE_0;
        break;
    case APROC_IOCTR_BALANCE_INPUT0:
        u4Idx = APROC_REG_INPUT0_BALANCE_L;
        break;
    case APROC_IOCTR_BALANCE_INPUT1:
        u4Idx = APROC_REG_INPUT1_BALANCE_L;
        break;
    case APROC_IOCTR_BALANCE_INPUT2:
        u4Idx = APROC_REG_INPUT2_BALANCE_L;
        break;        
    default:
        LOG (5, "_vAproc_PostProcBalance_Get: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    pi4Value[0] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_1, u4Idx));
    pi4Value[1] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_1, u4Idx+1));
}

static void _vAproc_PostProcMute_Set (UINT32 u4Id, INT32 i4Value)
{
    UINT32 u4Idx;

    switch (u4Id)
    {
    case APROC_IOCTR_MUTE_AMIXER0:
        u4Idx = APROC_REG_MUTE_AMIXER0_FLAG;
        break;
    case APROC_IOCTR_MUTE_AMIXER1:
        u4Idx = APROC_REG_MUTE_AMIXER1_FLAG;
        break;    
    case APROC_IOCTR_MUTE_AMIXER2:
        u4Idx = APROC_REG_MUTE_AMIXER2_FLAG;
        break;                
    default:
        LOG (5, "_vAproc_PostProcMute_Set: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, u4Idx), i4Value);
}

static void _vAproc_PostProcMute_Get (UINT32 u4Id, INT32 *pi4Value)
{
    UINT32 u4Idx;

    switch (u4Id)
    {
    case APROC_IOCTR_MUTE_AMIXER0:
        u4Idx = APROC_REG_MUTE_AMIXER0_FLAG;
        break;
    case APROC_IOCTR_MUTE_AMIXER1:
        u4Idx = APROC_REG_MUTE_AMIXER1_FLAG;
        break;    
    case APROC_IOCTR_MUTE_AMIXER2:
        u4Idx = APROC_REG_MUTE_AMIXER2_FLAG;
        break;                
    default:
        LOG (5, "_vAproc_PostProcMute_Get: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    *pi4Value = (INT32) u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, u4Idx));
}

static void _vAproc_PostProcTrim_Get (UINT32 u4Id, INT32 *pi4Value)
{
    UINT32 u4Idx;

    switch (u4Id)
    {
    case APROC_IOCTR_TRIM_INPUT0:
        u4Idx = APROC_REG_INPUT0_TRIM;
        break;
    case APROC_IOCTR_TRIM_INPUT1:
        u4Idx = APROC_REG_INPUT1_TRIM;
        break;    
    case APROC_IOCTR_TRIM_INPUT2:
        u4Idx = APROC_REG_INPUT2_TRIM;
        break;         
    case APROC_IOCTR_TRIM_SP:
        u4Idx = APROC_REG_SP_TRIM;
        break;
    case APROC_IOCTR_TRIM_SPSW:
        u4Idx = APROC_REG_SPSW_TRIM;
        break;
    case APROC_IOCTR_TRIM_HP:
        u4Idx = APROC_REG_HP_TRIM;
        break;
    case APROC_IOCTR_TRIM_MON:
        u4Idx = APROC_REG_MON_TRIM;
        break;    
    case APROC_IOCTR_TRIM_IEC:
        u4Idx = APROC_REG_IEC_TRIM;
        break;
    case APROC_IOCTR_TRIM_SCART:
        u4Idx = APROC_REG_SCART_TRIM;
        break;    
    case APROC_IOCTR_TRIM_BT:
        u4Idx = APROC_REG_BT_TRIM;
        break;
    case APROC_IOCTR_TRIM_AMIXER0:
        u4Idx = APROC_REG_AMIXER0_TRIM;
        break;
    case APROC_IOCTR_TRIM_AMIXER1:
        u4Idx = APROC_REG_AMIXER1_TRIM;
        break;  
    case APROC_IOCTR_TRIM_AMIXER2:
        u4Idx = APROC_REG_AMIXER2_TRIM;
        break;           
    case APROC_IOCTR_TRIM_DELAY0:
        u4Idx = APROC_REG_DELAY0_TRIM;
        break;  
    case APROC_IOCTR_TRIM_DELAY1:
        u4Idx = APROC_REG_DELAY1_TRIM;
        break;       
    default:
        LOG (5, "_vAproc_PostProcTrim_Get: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    *pi4Value = (INT32) u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_0, u4Idx));
}
static void _vAproc_PostProcBassTreble_Get (UINT32 u4Id, INT32 *pi4Value)
{
    UINT32 u4Idx, u4length=4;

    switch (u4Id)
    {
    case APROC_DSP_BIQ_ID_BASS_SP:
        u4Idx = APROC_REG_BIQ_SP_BASS_TYPE;
        break;
    case APROC_DSP_BIQ_ID_BASS_SP_LSRS:
        u4Idx = APROC_REG_BIQ_SP_LSRS_BASS_TYPE;
        break;    
    case APROC_DSP_BIQ_ID_BASS_HP:
        u4Idx = APROC_REG_BIQ_HP_BASS_TYPE;
        break;                
    case APROC_DSP_BIQ_ID_TREBLE_SP:
        u4Idx = APROC_REG_BIQ_SP_TREBLE_TYPE;
        break;                
    case APROC_DSP_BIQ_ID_TREBLE_SP_LSRS:
        u4Idx = APROC_REG_BIQ_SP_LSRS_TREBLE_TYPE;
        break;                
    case APROC_DSP_BIQ_ID_TREBLE_HP:
        u4Idx = APROC_REG_BIQ_HP_TREBLE_TYPE;
        break;                    
    case APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP:
        u4Idx = APROC_REG_BIQ_SP_BASSTREBLE_FLAG;
        u4length = 1;
        break;                        
    case APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP_LSRS:
        u4Idx = APROC_REG_BIQ_SP_LSRS_BASSTREBLE_FLAG;
        u4length = 1;        
        break;                        
    case APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_HP:
        u4Idx = APROC_REG_BIQ_HP_BASSTREBLE_FLAG;
        u4length = 1;        
        break;               
    default:
        LOG (5, "_vAproc_PostProcBassTreble_Get: wrong id = 0x%x!!!\n", u4Id);
        return;
    }

    if(u4length == 1)
    {
        *pi4Value = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, u4Idx));
    }
    else
    {
        pi4Value[0] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, u4Idx));
        pi4Value[1] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, u4Idx+1));
        pi4Value[2] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, u4Idx+2));
        pi4Value[3] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, u4Idx+3));
    }
    
}

static void _vAproc_PostProcChSel_Get (UINT32 u4Id, INT32 *pi4Value)
{
    UINT32 u4Idx, u4length=4;

    switch (u4Id)
    {
    case APROC_DSP_BIQ_ID_HPF_SP:
        u4Idx = APROC_REG_BIQ_SP_HPF_0_TYPE;
        break;
    case APROC_DSP_BIQ_ID_LPF_SP_LSRS:
        u4Idx = APROC_REG_BIQ_SP_LSRS_HPF_0_TYPE;
        break;       
    case APROC_DSP_BIQ_ID_CHSEL_MODE:
        u4Idx = APROC_REG_LSRS_FROM_LR_SEL;
        u4length = 1;        
        break;               
    default:
        LOG (5, "_vAproc_PostProcChSel_Get: wrong id = 0x%x!!!\n", u4Id);
        return;
    }

    if(u4length == 1)
    {
        *pi4Value = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, u4Idx));
    }
    else
    {
        pi4Value[0] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_6, u4Idx));
        pi4Value[1] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_6, u4Idx+1));
        pi4Value[2] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_6, u4Idx+2));
        pi4Value[3] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_6, u4Idx+3));
    }    
}


static void _vAproc_PostProcDRC_Get (UINT32 u4Id, INT32 *pi4Value)
{
    UINT32 u4Idx, u4length=4, u4Grp=APROC_ASM_ID_POSTPROC_3;

    switch (u4Id)
    {
    case APROC_DSP_BIQ_ID_3DRC_0:
        u4Idx = APROC_REG_BIQ_SP_AVC_0_TYPE_0;
        break;
    case APROC_DSP_BIQ_ID_3DRC_1:
        u4Idx = APROC_REG_BIQ_SP_AVC_1_TYPE_0;
        break;
    case APROC_DSP_BIQ_ID_3DRC_2_1:
        u4Idx = APROC_REG_BIQ_SP_AVC_2_TYPE_0;
        break;
    case APROC_DSP_BIQ_ID_3DRC_2_2:
        u4Idx = APROC_REG_BIQ_SP_AVC_2_TYPE_1;
        break;
        
    case APROC_DSP_BIQ_ID_1DRC_0:
       u4Idx = APROC_REG_BIQ_SP_LSRS_AVC_0_TYPE_0;
       u4Grp=APROC_ASM_ID_POSTPROC_F;
       break;

    case APROC_DSP_BIQ_ID_3DRC_TARGET0:
        u4Idx = APROC_REG_AVC_SP_TARGET_0;
        u4Grp=APROC_ASM_ID_POSTPROC_2;
        u4length = 1;        
        break;               
    case APROC_DSP_BIQ_ID_3DRC_TARGET1:
        u4Idx = APROC_REG_AVC_SP_TARGET_1;
        u4Grp=APROC_ASM_ID_POSTPROC_2;
        u4length = 1;        
        break;               
    case APROC_DSP_BIQ_ID_3DRC_TARGET2:
        u4Idx = APROC_REG_AVC_SP_TARGET_2;
        u4Grp=APROC_ASM_ID_POSTPROC_2;
        u4length = 1;        
        break;               
    case APROC_DSP_BIQ_ID_1DRC_TARGET0:
        u4Idx = APROC_REG_AVC_SP_LSRS_TARGET_0;
        u4Grp=APROC_ASM_ID_POSTPROC_F;
        u4length = 1;        
        break;      
        
    case APROC_DSP_BIQ_ID_3DRC_FLAG:
        u4Idx = APROC_REG_SP_AVC_FLAG;
        u4Grp=APROC_ASM_ID_POSTPROC_A;
        u4length = 1;        
        break;

    case APROC_DSP_BIQ_ID_1DRC_FLAG:
        u4Idx = APROC_REG_SP_LSRS_AVC_FLAG;
        u4Grp=APROC_ASM_ID_POSTPROC_A;
        u4length = 1;        
        break;
        
    default:
        LOG (5, "_vAproc_PostProcDRC_Get: wrong id = 0x%x!!!\n", u4Id);
        return;
    }

    if(u4length == 1)
    {
        *pi4Value = u4AprocReg_Read (APROC_ASM_ADDR (u4Grp, u4Idx));
    }
    else
    {
        pi4Value[0] = u4AprocReg_Read (APROC_ASM_ADDR (u4Grp, u4Idx));
        pi4Value[1] = u4AprocReg_Read (APROC_ASM_ADDR (u4Grp, u4Idx+1));
        pi4Value[2] = u4AprocReg_Read (APROC_ASM_ADDR (u4Grp, u4Idx+2));
        pi4Value[3] = u4AprocReg_Read (APROC_ASM_ADDR (u4Grp, u4Idx+3));
    }    
}

static void _vAproc_PostProcDBG_Get (UINT32 u4Id, UINT32 *pu4Value)
{

    UINT32 u4Num,i;
    UINT8 u1Data[SAPOST_DEBUG_SZ];

    UNUSED (u4Id);
    UNUSED (pu4Value);

    while(u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_SAP_MISC_23, APROC_REG_SPOST_GETVAR_FLAG)) != 0 )
    {
        x_thread_delay(100);
    }
    u4Num = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_SAP_MISC_23, APROC_REG_SPOST_GETVAR_OSIZE));
    vAprocDataPort_Read (APROC_DATAPORT_SAPOST_DEBUG_BASE, (UINT32*)u1Data, u4Num/4+1);        

    for(i=0;i<u4Num;i++)
    {
        Printf("0x%x ", u1Data[i]);
    }    
}

/*Aproc Bmang*/
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST

static void _vAproc_PostProcBmang_Get (UINT32 *pi4Value)
{
    APROC_DSP_BMANG_SET_T  *sBmangSet;

    sBmangSet = (APROC_DSP_BMANG_SET_T *)pi4Value;

    sBmangSet->CtrlFlag = 
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_CTRL_FLAG));
    sBmangSet->ChCfg = 
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_CHID_CFG));
    sBmangSet->u4LFETrim =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_LEF_TRIM));
    sBmangSet->u4Volume =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_VOLUME));
    sBmangSet->sSpkSize[0] =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_L_SIZE));
    sBmangSet->sSpkSize[1] =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_R_SIZE));
    sBmangSet->sSpkSize[2] =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_C_SIZE));
    sBmangSet->sSpkSize[3] =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_LS_SIZE));
    sBmangSet->sSpkSize[4] =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_RS_SIZE));
    sBmangSet->sSpkSize[5] =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_SW_SIZE));
    sBmangSet->u4LFE2CH =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_LFE2CH));
    sBmangSet->u4CutFc =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BIQ_BMANG_SP_HPF_FC));
}


static void _vAproc_PostProcBmang_Set (UINT32 *pu4Value)
{
    APROC_DSP_BMANG_SET_T  *sBmangSet;

    sBmangSet = (APROC_DSP_BMANG_SET_T *)pu4Value;

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_CTRL_FLAG), sBmangSet->CtrlFlag);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_CHID_CFG), sBmangSet->ChCfg);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_LEF_TRIM), sBmangSet->u4LFETrim);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_VOLUME), sBmangSet->u4Volume);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_L_SIZE), sBmangSet->sSpkSize[0]);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_R_SIZE), sBmangSet->sSpkSize[1]);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_C_SIZE), sBmangSet->sSpkSize[2]);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_LS_SIZE), sBmangSet->sSpkSize[3]);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_RS_SIZE), sBmangSet->sSpkSize[4]);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_SW_SIZE), sBmangSet->sSpkSize[5]);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BMANG_LFE2CH), sBmangSet->u4LFE2CH);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BIQ_BMANG_SP_HPF_FC), sBmangSet->u4CutFc);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_20, APROC_REG_BIQ_BMANG_SW_LPF_FC), sBmangSet->u4CutFc);

}

static void _vAproc_PostProcMVS_Get (UINT32 *pi4Value)
{
    APROC_DSP_MVS_SET_T  *sMVSSet;

    sMVSSet = (APROC_DSP_MVS_SET_T *)pi4Value;

    sMVSSet->fgMVSFlag=
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_FLAG));
    sMVSSet->i4ClarityGain =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_CLARITY));
    sMVSSet->i4WidthGain = 
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_WIDTH));
    sMVSSet->i4LRGain =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_LR_GAIN));
    sMVSSet->i4CrossTalkGain =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_CROSSTALK));
    sMVSSet->i4OutputGain =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_OUTPUT_GAIN));
    sMVSSet->i4BassGain =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_BASS_GAIN));
    sMVSSet->i4Fc =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_FO));
}


static void _vAproc_PostProcMVS_Set (UINT32 *pu4Value)
{
    APROC_DSP_MVS_SET_T  *sMVSSet;

    sMVSSet = (APROC_DSP_MVS_SET_T *)pu4Value;

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_FLAG), sMVSSet->fgMVSFlag);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_CLARITY), sMVSSet->i4ClarityGain);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_WIDTH), sMVSSet->i4WidthGain);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_LR_GAIN), sMVSSet->i4LRGain);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_CROSSTALK), sMVSSet->i4CrossTalkGain);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_OUTPUT_GAIN), sMVSSet->i4OutputGain);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_BASS_GAIN), sMVSSet->i4BassGain);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVS_SP_FO), sMVSSet->i4Fc);
}

static void _vAproc_PostProcMVB_Get (UINT32 *pi4Value)
{
    APROC_DSP_MVB_SET_T  *sMVBSet;

    sMVBSet = (APROC_DSP_MVB_SET_T *)pi4Value;

    sMVBSet->fgMVBFlag= 
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_FLAG));
    sMVBSet->i4OutputGain = 
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_OUTPUT_GAIN));
    sMVBSet->i4BassGain = 
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_BASS_GAIN));
    sMVBSet->u4Fc =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_FO));
    sMVBSet->u4AttackStep =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_ATTAK));
    sMVBSet->u4ReleaseStep =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_RELEASE));
    sMVBSet->u4ExpanderTh =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_TH));
    sMVBSet->i4DetGain =
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_DET_GAIN));
}

static void _vAproc_PostProcMVB_Set (UINT32 *pu4Value)
{
    APROC_DSP_MVB_SET_T  *sMVBSet;

    sMVBSet = (APROC_DSP_MVB_SET_T *)pu4Value;
    
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_FLAG), sMVBSet->fgMVBFlag);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_OUTPUT_GAIN), sMVBSet->i4OutputGain);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_BASS_GAIN), sMVBSet->i4BassGain);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_FO), sMVBSet->u4Fc);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_ATTAK), sMVBSet->u4AttackStep);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_RELEASE), sMVBSet->u4ReleaseStep);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_TH), sMVBSet->u4ExpanderTh);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_B, APROC_REG_MVB_SPSW_DET_GAIN), sMVBSet->i4DetGain);
}

static void _vAproc_PostProcPEQ_Get (UINT32 *pu4Value, UINT8 u1PEQSet)
{
    APROC_DSP_PEQ_SET_T *sPEQSet;

    if (u1PEQSet > 8)
        u1PEQSet = 8;
            
    sPEQSet = (APROC_DSP_PEQ_SET_T *)pu4Value;

    sPEQSet->u4Fc= 
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_21, APROC_REG_PEQ_0_FC + u1PEQSet));
    sPEQSet->u4Q = 
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_21, APROC_REG_PEQ_0_Q + u1PEQSet));
    sPEQSet->u4Gain = 
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_21, APROC_REG_PEQ_0_GAIN + u1PEQSet));
}

static void _vAproc_PostProcPEQ_Set (UINT32 *pu4Value, UINT8 u1PEQSet)
{
    APROC_DSP_PEQ_SET_T *sPEQSet;

    if (u1PEQSet > 8)
        u1PEQSet = 8;
            
    sPEQSet = (APROC_DSP_PEQ_SET_T *)pu4Value;

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_21, APROC_REG_PEQ_0_FC + u1PEQSet), sPEQSet->u4Fc);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_21, APROC_REG_PEQ_0_Q + u1PEQSet), sPEQSet->u4Q);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_21, APROC_REG_PEQ_0_GAIN + u1PEQSet), sPEQSet->u4Gain);
}

static void _vAproc_PostProcEQ_Get (UINT32 *pu4Value, UINT8 u1EQband, UINT8 u1Num)
{
    APROC_DSP_EQ_SET_T *sEQSet;

    if (u1EQband > 7)
        u1EQband = 7;

    if (u1Num == 4)
    {
        sEQSet = (APROC_DSP_EQ_SET_T *)pu4Value;
    
        sEQSet->u4Fc= 
            u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_7, APROC_REG_BIQ_SP_EQ_0_FC + u1EQband*4));
        sEQSet->u4Q = 
            u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_7, APROC_REG_BIQ_SP_EQ_0_Q + u1EQband*4));
        sEQSet->u4Gain = 
            u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_7, APROC_REG_BIQ_SP_EQ_0_GAIN + u1EQband*4));
        sEQSet->u4Type= 
            u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_7, APROC_REG_BIQ_SP_EQ_0_TYPE + u1EQband*4));
    }
    else
    {
        sEQSet = (APROC_DSP_EQ_SET_T *)pu4Value;
    
        sEQSet->u4Fc= 
            u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_3B, APROC_REG_BIQ_HP_EQ_0_FC + u1EQband*4));
        sEQSet->u4Q = 
            u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_3B, APROC_REG_BIQ_HP_EQ_0_Q + u1EQband*4));
        sEQSet->u4Gain = 
            u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_3B, APROC_REG_BIQ_HP_EQ_0_GAIN + u1EQband*4));
        sEQSet->u4Type= 
            u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_3B, APROC_REG_BIQ_HP_EQ_0_TYPE + u1EQband*4));
    }
}

static void _vAproc_PostProcEQ_Set (UINT32 *pu4Value, UINT8 u1EQband, UINT8 u1Num)
{
    APROC_DSP_EQ_SET_T *sEQSet;

    if (u1EQband > 7)
        u1EQband = 7;

    if (u1Num == 4)
    {
        sEQSet = (APROC_DSP_EQ_SET_T *)pu4Value;

        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_7, APROC_REG_BIQ_SP_EQ_0_TYPE + u1EQband*4), sEQSet->u4Type);
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_7, APROC_REG_BIQ_SP_EQ_0_FC + u1EQband*4), sEQSet->u4Fc);
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_7, APROC_REG_BIQ_SP_EQ_0_Q + u1EQband*4), sEQSet->u4Q);
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_7, APROC_REG_BIQ_SP_EQ_0_GAIN + u1EQband*4), sEQSet->u4Gain);
    }
    else
    {
        sEQSet = (APROC_DSP_EQ_SET_T *)pu4Value;

        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_3B, APROC_REG_BIQ_HP_EQ_0_TYPE + u1EQband*4), sEQSet->u4Type);
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_3B, APROC_REG_BIQ_HP_EQ_0_FC + u1EQband*4), sEQSet->u4Fc);
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_3B, APROC_REG_BIQ_HP_EQ_0_Q + u1EQband*4), sEQSet->u4Q);
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_3B, APROC_REG_BIQ_HP_EQ_0_GAIN + u1EQband*4), sEQSet->u4Gain);
    }
}

static void _vAproc_PostProcLimiter_Get (UINT8 u1LimtIdx, UINT32 *pu4Value)
{
    APROC_DSP_LIMT_SET_T *sLimtSet;

    sLimtSet = (APROC_DSP_LIMT_SET_T *)pu4Value;

    if (u1LimtIdx >= APROC_LIMITER_IDX_NUM)
        u1LimtIdx  = APROC_LIMITER_SP;

    sLimtSet->u4SetProcMode = 
        u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_MODE));

    sLimtSet->i4SetSgainPeriod = 
        (INT32)u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SGAIN_PERIOD));

    switch (u1LimtIdx)
    {
        case APROC_LIMITER_SP:
            sLimtSet->fgLimtFlag = 
                (BOOL)u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_FLAG));
            sLimtSet->i4SetTargetLevel =
                (INT32)u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_TARGET));
            sLimtSet->i4SetAttackStepDB = 
                (INT32)u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_ATTACK));
            sLimtSet->i4SetReleaseStepDB = 
                (INT32)u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_RELEASE));
            sLimtSet->i4SetRatio = 
                (INT32)u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_RATIO));
            sLimtSet->i4SetSilenceLevel = 
                (INT32)u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_SILENCE));
            sLimtSet->i4SetMaxExpandGainDB = 
                (INT32)u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_MAX_EXPAND));
            sLimtSet->i4SetPostGain = 
                (INT32)u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_POST_GAIN));
            sLimtSet->i4SetHoldPeriod = 
                (INT32)u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_HOLD));
            sLimtSet->i4SetDetPeriod = 
                (INT32)u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_DET_PERIOD));
            break;

        case APROC_LIMITER_SP_SW:
            sLimtSet->fgLimtFlag= 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_FLAG));
            sLimtSet->i4SetTargetLevel =
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_TARGET));
            sLimtSet->i4SetAttackStepDB = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_ATTACK));
            sLimtSet->i4SetReleaseStepDB = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_RELEASE));
            sLimtSet->i4SetRatio = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_RATIO));
            sLimtSet->i4SetSilenceLevel = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_SILENCE));
            sLimtSet->i4SetMaxExpandGainDB = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_MAX_EXPAND));
            sLimtSet->i4SetPostGain = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_POST_GAIN));
            sLimtSet->i4SetHoldPeriod = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_HOLD));
            sLimtSet->i4SetDetPeriod = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_DET_PERIOD));
            break;

        case APROC_LIMITER_SP_LSRS:
            sLimtSet->fgLimtFlag= 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_FLAG));
            sLimtSet->i4SetTargetLevel =
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_TARGET));
            sLimtSet->i4SetAttackStepDB = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_ATTACK));
            sLimtSet->i4SetReleaseStepDB = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_RELEASE));
            sLimtSet->i4SetRatio = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_RATIO));
            sLimtSet->i4SetSilenceLevel = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_SILENCE));
            sLimtSet->i4SetMaxExpandGainDB = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_MAX_EXPAND));
            sLimtSet->i4SetPostGain = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_POST_GAIN));
            sLimtSet->i4SetHoldPeriod = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_HOLD));
            sLimtSet->i4SetDetPeriod = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_DET_PERIOD));
            break;

        case APROC_LIMITER_HP:
            sLimtSet->fgLimtFlag= 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_FLAG));
            sLimtSet->i4SetTargetLevel =
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_TARGET));
            sLimtSet->i4SetAttackStepDB = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_ATTACK));
            sLimtSet->i4SetReleaseStepDB = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_RELEASE));
            sLimtSet->i4SetRatio = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_RATIO));
            sLimtSet->i4SetSilenceLevel = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_SILENCE));
            sLimtSet->i4SetMaxExpandGainDB = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_MAX_EXPAND));
            sLimtSet->i4SetPostGain = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_POST_GAIN));
            sLimtSet->i4SetHoldPeriod = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_HOLD));
            sLimtSet->i4SetDetPeriod = 
                u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_DET_PERIOD));
            break;

        default:
            break;
            
    }
}

static void _vAproc_PostProcLimiter_Set (UINT8 u1LimtIdx, UINT32 *pu4Value)
{
    APROC_DSP_LIMT_SET_T *sLimtSet;

    sLimtSet = (APROC_DSP_LIMT_SET_T *)pu4Value;

    if (u1LimtIdx >= APROC_LIMITER_IDX_NUM)
        u1LimtIdx  = APROC_LIMITER_SP;

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_MODE), sLimtSet->u4SetProcMode);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SGAIN_PERIOD), sLimtSet->i4SetSgainPeriod);

    switch (u1LimtIdx)
    {
        case APROC_LIMITER_SP:
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_FLAG), sLimtSet->fgLimtFlag);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_TARGET), sLimtSet->i4SetTargetLevel);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_ATTACK), sLimtSet->i4SetAttackStepDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_RELEASE), sLimtSet->i4SetReleaseStepDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_RATIO), sLimtSet->i4SetRatio);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_SILENCE), sLimtSet->i4SetSilenceLevel);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_MAX_EXPAND), sLimtSet->i4SetMaxExpandGainDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_POST_GAIN), sLimtSet->i4SetPostGain);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_HOLD), sLimtSet->i4SetHoldPeriod);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SP_DET_PERIOD), sLimtSet->i4SetDetPeriod);
            _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
            break;

        case APROC_LIMITER_SP_SW:
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_FLAG), sLimtSet->fgLimtFlag);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_TARGET), sLimtSet->i4SetTargetLevel);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_ATTACK), sLimtSet->i4SetAttackStepDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_RELEASE), sLimtSet->i4SetReleaseStepDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_RATIO), sLimtSet->i4SetRatio);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_SILENCE), sLimtSet->i4SetSilenceLevel);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_MAX_EXPAND), sLimtSet->i4SetMaxExpandGainDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_POST_GAIN), sLimtSet->i4SetPostGain);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_HOLD), sLimtSet->i4SetHoldPeriod);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_C, APROC_REG_LIMITER_SPSW_DET_PERIOD), sLimtSet->i4SetDetPeriod);
            _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
            break;

        case APROC_LIMITER_SP_LSRS:
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_FLAG), sLimtSet->fgLimtFlag);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_TARGET), sLimtSet->i4SetTargetLevel);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_ATTACK), sLimtSet->i4SetAttackStepDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_RELEASE), sLimtSet->i4SetReleaseStepDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_RATIO), sLimtSet->i4SetRatio);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_SILENCE), sLimtSet->i4SetSilenceLevel);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_MAX_EXPAND), sLimtSet->i4SetMaxExpandGainDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_POST_GAIN), sLimtSet->i4SetPostGain);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_HOLD), sLimtSet->i4SetHoldPeriod);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_SP_LSRS_DET_PERIOD), sLimtSet->i4SetDetPeriod);
            _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_LSRS_PATH);
            break;

        case APROC_LIMITER_HP:
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_FLAG), sLimtSet->fgLimtFlag);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_TARGET), sLimtSet->i4SetTargetLevel);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_ATTACK), sLimtSet->i4SetAttackStepDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_RELEASE), sLimtSet->i4SetReleaseStepDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_RATIO), sLimtSet->i4SetRatio);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_SILENCE), sLimtSet->i4SetSilenceLevel);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_MAX_EXPAND), sLimtSet->i4SetMaxExpandGainDB);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_POST_GAIN), sLimtSet->i4SetPostGain);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_HOLD), sLimtSet->i4SetHoldPeriod);
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_10, APROC_REG_LIMITER_HP_DET_PERIOD), sLimtSet->i4SetDetPeriod);
            _vAprocSetRoutine (APROC_ROUTINE_ID_ER_HP_PATH);
            break;

        default:
            break;
    
    }
}


#ifdef ARM_SUPPORT_SRS_SS

void _vAproc_PostProcSRSAEQ_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_LAEQ_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_LAEQ_SPSW_INPUTGAIN), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_LAEQ_SPSW_OUTGAIN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_LAEQ_SPSW_BPGAIN), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_RAEQ_SPSW_ENABLE), u4v0);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_RAEQ_SPSW_INPUTGAIN), u4v0);
                break;
            case 6:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_RAEQ_SPSW_OUTGAIN), u4v0);
                break;
            case 7:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_RAEQ_SPSW_BPGAIN), u4v0);
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSAEQ_Get()
{
    Printf("\nLeft AEQ Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_LAEQ_SPSW_ENABLE)));
    Printf("Left InputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_LAEQ_SPSW_INPUTGAIN)));
    Printf("Left OutputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_LAEQ_SPSW_OUTGAIN)));
    Printf("Left BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_LAEQ_SPSW_BPGAIN)));
    Printf("Right AEQ Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_RAEQ_SPSW_ENABLE)));
    Printf("Right InputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_RAEQ_SPSW_INPUTGAIN)));
    Printf("Right OutputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_RAEQ_SPSW_OUTGAIN)));
    Printf("Right BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_AEQ, APROC_REG_RAEQ_SPSW_BPGAIN)));
}
void _vAproc_PostProcSRSBT_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_EANBLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_INPUTGAIN), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_OUTGAIN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_BPGAIN), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_BASSGAIN), u4v0);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_TRBGAIN), u4v0);
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSBT_Get()
{
    Printf("\nBassTreble Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_EANBLE)));
    Printf("InputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_INPUTGAIN)));
    Printf("OutputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_OUTGAIN)));
    Printf("BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_BPGAIN)));
    Printf("BassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_BASSGAIN)));
    Printf("TrebleGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_BASSTREB, APROC_REG_BASTRB_SPSW_TRBGAIN)));
}
void _vAproc_PostProcSRSCC3D_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_INPUTGAIN), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_OUTGAIN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_BPGAIN), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_DPEN), u4v0);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_SUREN), u4v0);
                break;
            case 6:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_MODE), u4v0);
                break;
            case 7:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_DEPMODE), u4v0);
                break;
            case 8:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_DEPFADE_ENABLE), u4v0);
                break;
            case 9:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_LR_ENABLE), u4v0);
                break;
            case 10:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_LR_GAIN), u4v0);
                break;
            case 11:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_MIXFADE_CTRL), u4v0);
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSCC3D_Get()
{
    Printf("\nLeft CC3D Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_ENABLE)));
    Printf("InputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_INPUTGAIN)));
    Printf("OutputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_OUTGAIN)));
    Printf("BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_BPGAIN)));
    Printf("DepthProcessEnable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_DPEN)));
    Printf("SurEn= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_SUREN)));
    Printf("Mode= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_MODE)));
    Printf("DepthControlMode= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_DEPMODE)));
    Printf("DepthChannelFadeEnable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_DEPFADE_ENABLE)));
    Printf("LREnhEnable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_LR_ENABLE)));
    Printf("LREnhGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_LR_GAIN)));
    Printf("MixFadeCtrl= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_MIXFADE_CTRL)));
}
void _vAproc_PostProcSRSCSHP_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CSHP, APROC_REG_CSHP_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CSHP, APROC_REG_CSHP_SPSW_INPUTGAIN), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CSHP, APROC_REG_CSHP_SPSW_OUTGAIN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CSHP, APROC_REG_CSHP_SPSW_BPGAIN), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CSHP, APROC_REG_CSHP_SPSW_RENDMODE), u4v0);
                break;
            
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSCSHP_Get()
{
    Printf("\nLeft CSHP Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CSHP, APROC_REG_CSHP_SPSW_ENABLE)));
    Printf("InputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CSHP, APROC_REG_CSHP_SPSW_INPUTGAIN)));
    Printf("OutputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CSHP, APROC_REG_CSHP_SPSW_OUTGAIN)));
    Printf("BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CSHP, APROC_REG_CSHP_SPSW_BPGAIN)));
    Printf("rendmode= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_CSHP, APROC_REG_CSHP_SPSW_RENDMODE)));
}
void _vAproc_PostProcSRSGEQ_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_INTER_BNAD), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_EXBAND_BEH), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_BGAIN0), u4v0);
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_GAIN_UPDATE), 1);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_BGAIN1), u4v0);
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_GAIN_UPDATE), 1);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_BGAIN2), u4v0);
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_GAIN_UPDATE), 1);
                break;
            case 6:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_BGAIN3), u4v0);
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_GAIN_UPDATE), 1);
                break;
            case 7:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_BGAIN4), u4v0);
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_GAIN_UPDATE), 1);
                break;
            case 8:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_GAIN_UPDATE), u4v0);
                break;
            case 9:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_INPUTGAIN), u4v0);
                break;
            case 10:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_ENABLE), u4v0);
                break;
            case 11:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_INTER_BNAD), u4v0);
                break;
            case 12:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_EXBAND_BEH), u4v0);
                break;
            case 13:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_BGAIN0), u4v0);
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_GAIN_UPDATE), 1);
                break;
            case 14:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_BGAIN1), u4v0);
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_GAIN_UPDATE), 1);
                break;
            case 15:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_BGAIN2), u4v0);
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_GAIN_UPDATE), 1);
                break;
            case 16:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_BGAIN3), u4v0);
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_GAIN_UPDATE), 1);
                break;
            case 17:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_BGAIN4), u4v0);
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_GAIN_UPDATE), 1);
                break;
            case 18:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_GAIN_UPDATE), u4v0);
                break;
            case 19:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_INPUTGAIN), u4v0);
                break;
            
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSGEQ_Get()
{
    Printf("\nLeft GEQ Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_ENABLE)));
    Printf("Left Inter band= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_INTER_BNAD)));
    Printf("Left exband beh= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_EXBAND_BEH)));
    Printf("Left band gain0= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_BGAIN0)));
    Printf("Left band gain1= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_BGAIN1)));
    Printf("Left band gain2= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_BGAIN2)));
    Printf("Left band gain3= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_BGAIN3)));
    Printf("Left band gain4= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_BGAIN4)));
    Printf("Left Gain update= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_GAIN_UPDATE)));
    Printf("Left input gain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_LGEQ5B_SPSW_INPUTGAIN)));
    Printf("Right GEQ Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_ENABLE)));
    Printf("Right Inter band= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_INTER_BNAD)));
    Printf("Right exband beh= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_EXBAND_BEH)));
    Printf("Right band gain0= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_BGAIN0)));
    Printf("Right band gain1= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_BGAIN1)));
    Printf("Right band gain2= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_BGAIN2)));
    Printf("Right band gain3= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_BGAIN3)));
    Printf("Right band gain4= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_BGAIN4)));
    Printf("Right Gain update= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_GAIN_UPDATE)));
    Printf("Right input gain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_GEQ, APROC_REG_RGEQ5B_SPSW_INPUTGAIN)));
}
void _vAproc_PostProcSRSHL_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_INPUTGAIN), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_OUTGAIN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_BPGAIN), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_DEL_LEN), u4v0);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_BOOST), u4v0);
                break;
            case 6:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_AC_ENA), u4v0);
                break;
            case 7:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_AC_LEV), u4v0);
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSHL_Get()
{
    Printf("\nHL Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_ENABLE)));
    Printf("InputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_INPUTGAIN)));
    Printf("OutputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_OUTGAIN)));
    Printf("BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_BPGAIN)));
    Printf("Delay length= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_DEL_LEN)));
    Printf("boost= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_BOOST)));
    Printf("AC enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_AC_ENA)));
    Printf("AC level= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HL, APROC_REG_HL_SPSW_AC_LEV)));
    
}
void _vAproc_PostProcSRSHPF_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HPF, APROC_REG_LHPF_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HPF, APROC_REG_LHPF_SPSW_SECTION), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HPF, APROC_REG_RHPF_SPSW_ENABLE), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_HPF, APROC_REG_RHPF_SPSW_SECTION), u4v0);
                break;

            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSHPF_Get()
{
    Printf("\nLeft HPF enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HPF, APROC_REG_LHPF_SPSW_ENABLE)));
    Printf("left section= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HPF, APROC_REG_LHPF_SPSW_SECTION)));
    Printf("Right HPF enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HPF, APROC_REG_RHPF_SPSW_ENABLE)));
    Printf("right section= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_HPF, APROC_REG_RHPF_SPSW_SECTION)));
    
}

void _vAproc_PostProcSRSMVOL_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_VOLSTEP), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_PAN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_MUTE), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_BPGAIN), u4v0);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_PROCMODE), u4v0);
                break;
            case 6:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_HL_BOOST), u4v0);
                break;
            
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSMVOL_Get()
{
    Printf("\nMVOL Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_ENABLE)));
    Printf("step= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_VOLSTEP)));
    Printf("pan= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_PAN)));
    Printf("mute= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_MUTE)));
    Printf("bypass gain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_BPGAIN)));
    Printf("process mode= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_PROCMODE)));
    Printf("boost= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_MVOL, APROC_REG_MVOL_SPSW_HL_BOOST)));
    
}
void _vAproc_PostProcSRSSPHPF_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SPHPF, APROC_REG_SPHPF_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SPHPF, APROC_REG_SPHPF_SPSW_INPUTGAIN), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SPHPF, APROC_REG_SPHPF_SPSW_OUTGAIN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SPHPF, APROC_REG_SPHPF_SPSW_BPGAIN), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SPHPF, APROC_REG_SPHPF_SPSW_CUTOFF_FRQ), u4v0);
                break;
            
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSSPHPF_Get()
{
    Printf("\nSpace HPF Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SPHPF, APROC_REG_SPHPF_SPSW_ENABLE)));
    Printf("InputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SPHPF, APROC_REG_SPHPF_SPSW_INPUTGAIN)));
    Printf("OutputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SPHPF, APROC_REG_SPHPF_SPSW_OUTGAIN)));
    Printf("BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SPHPF, APROC_REG_SPHPF_SPSW_BPGAIN)));
    Printf("Cut off frq= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SPHPF, APROC_REG_SPHPF_SPSW_CUTOFF_FRQ)));
    
}
void _vAproc_PostProcSRSSRS3D_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_INPUTGAIN), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_OUTGAIN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_BPGAIN), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_HBITRATE), u4v0);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_SPACE_CTRL), u4v0);
                break;
            case 6:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_CEN_CTRL), u4v0);
                break;
            case 7:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_HP), u4v0);
                break;
            case 8:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_MODE), u4v0);
                break;
            case 9:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_NORM_GAIN), u4v0);
                break;
            
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSSRS3D_Get()
{
    Printf("\nSRS3D Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_ENABLE)));
    Printf("InputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_INPUTGAIN)));
    Printf("OutputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_OUTGAIN)));
    Printf("BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_BPGAIN)));
    Printf("hbitrate= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_HBITRATE)));
    Printf("space ctrl= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_SPACE_CTRL)));
    Printf("center ctrl= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_CEN_CTRL)));
    Printf("headphone= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_HP)));
    Printf("mode= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_MODE)));
    Printf("norm gain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SRS3D, APROC_REG_SRS3D_SPSW_NORM_GAIN)));   
    
}


void _vAproc_PostProcSRSSS3D_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_HEADGAIN), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_BPGAIN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MAINTECH), u4v0);
                if(u4v0 == 0)//NONE
                {   
                    //vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_NONE), 0x7fff);
                    ;
                }
                else if(u4v0 == 1)//WOWHD
                {
                    //vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_WOWHD), 0x7fff);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_SUREN), 1);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_MODE), 1);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_SURENABLE), 1);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_INMODE), 3);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LFOCUS_EN), 1);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RFOCUS_EN), 1);     

                }
                else if(u4v0 == 2)//TSHD
                {
                    //vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_TSHD), 0x7fff);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_SUREN), 1);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_MODE), 2);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_SURENABLE), 1);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_INMODE), 0x103);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_SPTUNE), 0x2000);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MF_CTRL), 0x7fff);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MR_CTRLEXT), 0x10002000);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_FC_CTRL), 0x2d71);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LFOCUS_EN), 0);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RFOCUS_EN), 0); 

                }
                else if(u4v0 == 3)//CC3D
                {
                    //vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_CC3D), 0x651f);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_SUREN), 1);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_MODE), 0);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_SURENABLE), 1);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_INMODE), 0x103);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MF_CTRL), 0x2000);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MR_CTRLEXT), 0x40000000);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_FC_CTRL), 0x4000);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LFOCUS_EN), 0);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RFOCUS_EN), 0);     
                    
                }
                else if(u4v0 == 4)//CSHP
                {
                    //vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_CSHP), 0x7fff);
                    ;
                }
                else if(u4v0 == 5)//TBHD
                {
                    //vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_TBHD), 0x7fff);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_SUREN), 0);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_CC3D, APROC_REG_CC3D_SPSW_MODE), 2);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_SURENABLE), 0);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_INMODE), 0x103);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_SPTUNE), 0x2000);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MF_CTRL), 0x7fff);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MR_CTRLEXT), 0x10002000);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_FC_CTRL), 0x2d71);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LFOCUS_EN), 0);
                    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RFOCUS_EN), 0); 
                }
                else
                {
                    Printf ("!Wrong Type maintech!\n");
                }
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_NONE), u4v0);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_WOWHD), u4v0);
                break;
            case 6:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_TSHD), u4v0);
                break;
            case 7:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_CC3D), u4v0);
                break;
            case 8:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_CSHP), u4v0);
                break;
            case 9:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_TBHD), u4v0);
                break;
            case 10:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MV_ENABLE), u4v0);
                break;
            case 11:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_TBHDRefOffset), u4v0);
                break;
            case 12:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_HdLmtBoostOffset), u4v0);
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSSS3D_Get()
{
    Printf("\nSS3D Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_ENABLE)));
    Printf("HeadroomGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_HEADGAIN)));
    Printf("BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_BPGAIN)));
    Printf("MainTech= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MAINTECH)));
    Printf("MainTechInputGain_NONE= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_NONE)));
    Printf("MainTechInputGain_WOWHD= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_WOWHD)));
    Printf("MainTechInputGain_TSHD= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_TSHD)));
    Printf("MainTechInputGain_CC3D= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_CC3D)));
    Printf("MainTechInputGain_CSHP= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_CSHP)));
    Printf("MainTechInputGain_TBHD= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MT_TBHD)));
    Printf("MvolEnable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_MV_ENABLE)));
    Printf("TBHDRefOffset= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_TBHDRefOffset)));
    Printf("HdLmtBoostOffset= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_SS3D, APROC_REG_SS3D_SPSW_HdLmtBoostOffset)));

}
void _vAproc_PostProcSRSTSHD_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_INPUTGAIN), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_OUTGAIN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_BPGAIN), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_SURENABLE), u4v0);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_INMODE), u4v0);
                break;
            case 6:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_FS_CTRL), u4v0);
                break;
            case 7:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_FC_CTRL), u4v0);
                break;
            case 8:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RS_CTRL), u4v0);
                break;
            case 9:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RC_CTRL), u4v0);
                break;
            case 10:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_FR_GAIN1), u4v0);
                break;
            case 11:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_FR_GAIN2), u4v0);
                break;
            case 12:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_TS_LEV), u4v0);
                break;
            case 13:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_SPTUNE), u4v0);
                break;
            case 14:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MF_CTRL), u4v0);
                break;
            case 15:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MR_CTRLEXT), u4v0);
                break;
            case 16:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MIX_EN), u4v0);
                break;
            case 17:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LDEF_EN), u4v0);
                break;
            case 18:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LDEF_Lev), u4v0);
                break;
            case 19:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RDEF_EN), u4v0);
                break;
            case 20:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RDEF_Lev), u4v0);
                break;
            case 21:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LFOCUS_EN), u4v0);
                break;
            case 22:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LFOCUS_Lev), u4v0);
                break;
            case 23:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RFOCUS_EN), u4v0);
                break;
            case 24:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RFOCUS_Lev), u4v0);
                break;
            case 25:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_CFOCUS_EN), u4v0);
                break;
            case 26:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_CFOCUS_Lev), u4v0);
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSTSHD_Get()
{
    Printf("\ntshd Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_ENABLE)));
    Printf("input gain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_INPUTGAIN)));
    Printf("output gain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_OUTGAIN)));
    Printf("BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_BPGAIN)));
    Printf("Sur enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_SURENABLE)));
    Printf("inmode= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_INMODE)));
    Printf("front space ctrl= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_FS_CTRL)));
    Printf("front center ctrl= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_FC_CTRL)));
    Printf("rear spce ctrl= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RS_CTRL)));
    Printf("rear center ctrl= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RC_CTRL)));
    Printf("front rear gain1= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_FR_GAIN1)));
    Printf("front rear gain2= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_FR_GAIN2)));
    Printf("trusurround lev= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_TS_LEV)));
    Printf("space tune= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_SPTUNE)));
    Printf("MF ctrl= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MF_CTRL)));
    Printf("MR CTRL EXT= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MR_CTRLEXT)));
    Printf("Mix enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_MIX_EN)));
    Printf("Ldef en= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LDEF_EN)));
    Printf("Ldef level= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LDEF_Lev)));
    Printf("Rdef en= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RDEF_EN)));
    Printf("Rdef level= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RDEF_Lev)));
    Printf("Lfocus en= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LFOCUS_EN)));
    Printf("Lfocus lev= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_LFOCUS_Lev)));
    Printf("Rfocus en= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RFOCUS_EN)));
    Printf("Rfocus lev= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_RFOCUS_Lev)));
    Printf("Cfocus en= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_CFOCUS_EN)));
    Printf("Cfocus lev= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TSHD, APROC_REG_TSHD_SPSW_CFOCUS_Lev)));
    
}
void _vAproc_PostProcSRSTBHD_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_INDEP), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_ANA_EN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_INPUTGAIN), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_OUTGAIN), u4v0);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_BPGAIN), u4v0);
                break;
            case 6:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_TB_CTRL), u4v0);
                break;
            case 7:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_CMP_CTRL), u4v0);
                break;
            case 8:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_RF_LEV), u4v0);
                break;
            case 9:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_INTGRA_LIMIT), u4v0);
                break;
            case 10:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_SPK_SIZE), u4v0);
                break;
            case 11:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_ANA_SIZE), u4v0);
                break;
            case 12:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_MODE), u4v0);
                break;
            case 13:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_OUT_OPT), u4v0);
                break;
            case 14:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_TBQ), u4v0);
                break;
            case 15:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_DCU_LEV), u4v0);
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSTBHD_Get()
{
    Printf("\ntbhd Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_ENABLE)));
    Printf("indepent= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_INDEP)));
    Printf("analysis enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_ANA_EN)));
    Printf("inputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_INPUTGAIN)));
    Printf("outputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_OUTGAIN)));
    Printf("bypass gain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_BPGAIN)));
    Printf("trubass lev= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_TB_CTRL)));
    Printf("compressor ctrl= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_CMP_CTRL)));
    Printf("reference lev= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_RF_LEV)));
    Printf("intergrant limit= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_INTGRA_LIMIT)));
    Printf("speaker filter size= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_SPK_SIZE)));
    Printf("analysis filter size= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_ANA_SIZE)));
    Printf("mode= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_MODE)));
    Printf("output option= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_OUT_OPT)));
    Printf("TBQ= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_TBQ)));
    Printf("DCU lev= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TBHD, APROC_REG_TBHD_SPSW_DCU_LEV)));
    
}
void _vAproc_PostProcSRSTRUD_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_INPUTGAIN), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_OUTGAIN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_BPGAIN), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_PRGAIN), u4v0);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_CLAGAIN), u4v0);
                break;
            
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSTRUD_Get()
{
    Printf("\nTRUD Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_ENABLE)));
    Printf("inputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_INPUTGAIN)));
    Printf("outputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_OUTGAIN)));
    Printf("BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_BPGAIN)));
    Printf("process gain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_PRGAIN)));
    Printf("cla gain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TRUD, APROC_REG_TRUD_SPSW_CLAGAIN)));
    
}
void _vAproc_PostProcSRSTVOL_Set (UINT32 u4type,UINT32 u4v0)
{
    switch (u4type)
        {
            case 0:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_ENABLE), u4v0);
                break;
            case 1:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_INPUTGAIN), u4v0);
                break;
            case 2:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_OUTGAIN), u4v0);
                break;
            case 3:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_BPGAIN), u4v0);
                break;
            case 4:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_LVINTER), u4v0);
                break;
            case 5:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_MODE), u4v0);
                break;
            case 6:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_NREN), u4v0);
                break;
            case 7:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_NRTH), u4v0);
                break;
            case 8:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_LKFS), u4v0);
                break;
            case 9:
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_SMOOTHEN), u4v0);
                break;
            
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
}
void _vAproc_PostProcSRSTVOL_Get()
{
    Printf("\ntvol Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_ENABLE)));
    Printf("inputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_INPUTGAIN)));
    Printf("outputGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_OUTGAIN)));
    Printf("BypassGain= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_BPGAIN)));
    Printf("LVinter= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_LVINTER)));
    Printf("Mode= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_MODE)));
    Printf("NR enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_NREN)));
    Printf("NR threshold= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_NRTH)));
    Printf("LKFS= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_LKFS)));
    Printf("Smooth enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_SRS_TVOL, APROC_REG_TVOL_SPSW_SMOOTHEN)));
        
}
#endif

void _vAproc_PostProcEqSpec_Get()
{
    Printf("\nEqSpec Enable= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_ENABLE)));
    Printf("band 0= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_0)));
    Printf("band 1= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_1)));
    Printf("band 2= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_2)));
    Printf("band 3= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_3)));
    Printf("band 4= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_4)));
    Printf("band 5= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_5)));
    Printf("band 6= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_6)));
    Printf("band 7= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_7)));
    Printf("band 8= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_8)));
    Printf("band 9= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_9)));
    Printf("band 10= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_10)));
    Printf("band 11= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_11)));
    Printf("band 12= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_12)));
    Printf("band 13= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_13)));
    Printf("band 14= 0x%x\n",u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_14)));

}
void _vAproc_PostProcEqSpec_Set (UINT32 u4v0)
{
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_EQSPECTRUM, APROC_REG_SPECTRUM_ENABLE), u4v0);
}

#endif



#ifdef CC_AUD_ARM_RENDER
#ifdef CC_AUD_SUPPORT_DBXTV

static BOOL _fgDBXTVTestEnable = FALSE;
static DBXTV_TotSonTestMode _eDBXTVTotSonMode = TOTSON_NORMAL;
static DBXTV_TotSonMode _eTotSonMode = TOTSON_OFF;
static DBXTV_TotVolMode _eTotVolMode = TOTVOL_OFF;
static DBXTV_TotSurMode _eTotSurMode = TOTSUR_OFF;
static UINT32 _DBXTVEnable = 0;

void AUD_DBXTV_Enable(BOOL fgEnable)
{
    LOG(5, "DBXTV fgEnable=%d\n", fgEnable);

    //vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE, pu4Value, 1);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_F, APROC_REG_DBXTV_EN), fgEnable);
    _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);

}

BOOL AUD_DBXTV_GetEnable(void)
{
    return (u4AprocReg_Read(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_F, APROC_REG_DBXTV_EN)) == 1);
}

/**
    Function to set the dbx-tv mode.
    DBXTV_SetMode() takes the Total Sonics, Total Volume, and Total Surround
    modes as inputs and determines the parameters to load into the data and
    coefficient memory.
    @param TSonMode one of the values of DBXTV_TSonMode
    @param TVolMode one of the values of DBXTV_TVolMode
    @param TSurMode one of the values of DBXTV_TSurMode
    @param Enable logical OR of Enable_XXX values defined in DBX_API_code.h
    @return None
*/
void AUD_DBXTV_SetMode(DBXTV_TotSonMode TotSonMode, DBXTV_TotVolMode TotVolMode, 
                                                  DBXTV_TotSurMode TotSurMode, UINT32 Enable)
{
    // Mute audio
    //OS_AudioMute();

    UINT32 temp = 0;
    _eTotSonMode = TotSonMode;
    _eTotVolMode = TotVolMode;
    _eTotSurMode = TotSurMode;
    _DBXTVEnable = Enable;

    LOG(5, "TotSonics=%d, TotVolume=%d, TotSurround=%d, Enable=0x%x\n", 
        TotSonMode, TotVolMode, TotSurMode, Enable);

    // Turn off dbx-tv
    temp = 0;
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_DVCOnOff, &temp, 1);
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_DPPOnOff, &temp, 1);
    temp = 1;
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_IWBypass, &temp, 1);
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_EQBypass, &temp, 1);
    temp = 0;
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_BEOnOff, &temp, 1);

    //Total Sonics: All
    if ( (TotSonMode != TOTSON_OFF) )
    {
        // Copy the Total Sonics preset to memory
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE, (UINT32 *)&TotSonParam_drv[TotSonMode], DBXTV_Param_LENGTH);
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_COEFF_BASE, (UINT32 *)&TotSonCoeff_drv[TotSonMode], DBXTV_Coeff_LENGTH);
    }

    //Total volume: DVC and DPP, [DBX_Param_OFFSET_DVC, DBX_Param_OFFSET_IW)
    //So if index in this range we should modify, TotVolParam
    //[DBX_Coeff_OFFSET_DVC, DBX_Coeff_OFFSET_IW)
    //TotVolCoeff
    if ( (TotVolMode != TOTVOL_OFF) )
    {
        // Copy the Total Volume DVC preset to memory
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE+DBX_Param_OFFSET_DVC, (UINT32 *)&TotVolParam[TotVolMode][DBX_Param_OFFSET_DVC], DBX_Param_LENGTH_DVC);//OnOff
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_COEFF_BASE+DBX_Coeff_OFFSET_DVC, (UINT32 *)&TotVolCoeff[TotVolMode][DBX_Coeff_OFFSET_DVC], DBX_Coeff_LENGTH_DVC);
        
        // Copy the Total Volume DPP preset to memory
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE+DBX_Param_OFFSET_DPP, (UINT32 *)&TotVolParam[TotVolMode][DBX_Param_OFFSET_DPP], DBX_Param_LENGTH_DPP);
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_COEFF_BASE+DBX_Coeff_OFFSET_DPP, (UINT32 *)&TotVolCoeff[TotVolMode][DBX_Coeff_OFFSET_DPP], DBX_Coeff_LENGTH_DPP);

    }

    if ( (TotSurMode != TOTSUR_OFF) )
    {
        if (TotVolMode == TOTVOL_OFF)
        {
            // Copy the Total Surround level parameters to memory
            vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE+DBX_Param_OFFSET_DVCHiLevel, (UINT32 *)&TotSurParam[TotSurMode][DBX_Param_OFFSET_DVCHiLevel], 1);
            vAprocDataPort_Write (APROC_DATAPORT_DBXTV_COEFF_BASE+DBX_Param_OFFSET_DVCLoLevel, (UINT32 *)&TotSurParam[TotSurMode][DBX_Param_OFFSET_DVCLoLevel],  1);
        }
        if (TotVolMode != TOTVOL_NIGHT)
        {
            // Copy the Total Surround IW preset to memory
            vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE+DBX_Param_OFFSET_IW, (UINT32 *)&TotSurParam[TotSurMode][DBX_Param_OFFSET_IW], DBX_Param_LENGTH_IW);
            vAprocDataPort_Write (APROC_DATAPORT_DBXTV_COEFF_BASE+DBX_Coeff_OFFSET_IW, (UINT32 *)&TotSurCoeff[TotSurMode][DBX_Coeff_OFFSET_IW], DBX_Coeff_LENGTH_IW);
            
            // Copy the Total Surround DPP preset to memory
            vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE+DBX_Param_OFFSET_DPP, (UINT32 *)&TotSurParam[TotSurMode][DBX_Param_OFFSET_DPP], DBX_Param_LENGTH_DPP);
            vAprocDataPort_Write (APROC_DATAPORT_DBXTV_COEFF_BASE+DBX_Coeff_OFFSET_DPP, (UINT32 *)&TotSurCoeff[TotSurMode][DBX_Coeff_OFFSET_DPP], DBX_Coeff_LENGTH_DPP);

        }
    }

    if (TotVolMode == TOTVOL_NIGHT)
    {
        // Turn off IW
        temp = 0x000001;
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_IWBypass, &temp, 1);//74

    }

    if ((Enable&Enable_DVC) == 0)
    {
        // Bypass DVC
        temp = 0x000000;
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_DVCOnOff, &temp, 1);

    }
    if ((Enable&Enable_DPP) == 0)
    {
        // Bypass DPP
        temp = 0x000000;
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_DPPOnOff, &temp, 1);

    }
    if ((Enable&Enable_IW) == 0)
    {
        // Bypass IW
        temp = 0x000001;
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_IWBypass, &temp, 1);

    }
    if ((Enable&Enable_EQ) == 0)
    {
        // Bypass EQ
        temp = 0x000001;
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_EQBypass, &temp, 1);

    }
    if ((Enable&Enable_BE) == 0)
    {
        // Bypass BE
        temp = 0x000000;
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_BEOnOff, &temp, 1);

    }

    _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);

    //AUD_DBXTV_ReInit();
    
    // Unmute audio
    //OS_AudioUnmute();

}


void AUD_DBXTV_GetMode(DBXTV_TotSonMode *pTotSonMode, DBXTV_TotVolMode *pTotVolMode, 
                                                  DBXTV_TotSurMode *pTotSurMode, UINT32 *pEnable)
{
    *pTotSonMode =  _eTotSonMode;
    *pTotVolMode = _eTotVolMode;
    *pTotSurMode = _eTotSurMode;
    *pEnable = _DBXTVEnable; 
}

//-----------------------------------------------------------------------------
/** 
 *   The following code is for DBX TV test vector verification
 */
//-----------------------------------------------------------------------------
void AUD_DBXTV_SetTestEnable(BOOL fgEnable)
{
    LOG(5, "DBX test fgEnable=%d\n", fgEnable);
    _fgDBXTVTestEnable= fgEnable;
}

BOOL AUD_DBXTV_GetTestEnable(void)
{
    return _fgDBXTVTestEnable;
}

DBXTV_TotSonTestMode AUD_DBXTV_GetTestMode(void)
{
    return _eDBXTVTotSonMode;
}

void AUD_DBXTV_SetTestMode(DBXTV_TotSonTestMode TotSonMode, UINT32 Enable)
{

    UINT32 temp = 0;
    _eDBXTVTotSonMode = TotSonMode;
    
    // Turn off dbx-tv  
    temp = 0;
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_DVCOnOff, &temp, 1);
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_DPPOnOff, &temp, 1);
    temp = 1;
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_IWBypass, &temp, 1);
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_EQBypass, &temp, 1);
    temp = 0;
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_BEOnOff, &temp, 1);
    
    if(TotSonMode >= TOTSON_NUM)
    {
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE, (UINT32 *)&TotSonParam_drv[0], DBXTV_Param_LENGTH);
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_COEFF_BASE, (UINT32 *)&TotSonCoeff_drv[0], DBXTV_Coeff_LENGTH);
    }
    else
    {
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE, (UINT32 *)&TotPresetParam[TotSonMode], DBXTV_Param_LENGTH);
    vAprocDataPort_Write (APROC_DATAPORT_DBXTV_COEFF_BASE, (UINT32 *)&TotPresetCoeff[TotSonMode], DBXTV_Coeff_LENGTH);
    }

    LOG(5, "AUD_DBXTV_SetMode TotSonMode=%d, Enable=%d\n", TotSonMode, Enable);
    if ((Enable&Enable_DVC) == 0)
    {
        // Bypass DVC
        temp = 0x000000;
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_DVCOnOff, &temp, 1);

    }
    if ((Enable&Enable_DPP) == 0)
    {
        // Bypass DPP
        temp = 0x000000;
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_DPPOnOff, &temp, 1);

    }
    if ((Enable&Enable_IW) == 0)
    {
        // Bypass IW
        temp = 0x000001;
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_IWBypass, &temp, 1);

    }
    if ((Enable&Enable_EQ) == 0)
    {
        // Bypass EQ
        temp = 0x000001;
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_EQBypass, &temp, 1);

    }
    if ((Enable&Enable_BE) == 0)
    {
        // Bypass BE
        temp = 0x000000;
        vAprocDataPort_Write (APROC_DATAPORT_DBXTV_PARAM_BASE + DBX_Param_OFFSET_BEOnOff, &temp, 1);

    }

    _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
    
}

#endif //CC_AUD_SUPPORT_DBXTV
#endif //CC_AUD_ARM_RENDER

static void _vAproc_PostProcDRC_Set (UINT32 u4Id, UINT32 *pu4Value)
{
    APROC_DSP_BIQ_INFO_T *sInfo;
    UINT32 u4Idx, u4Routine=APROC_ROUTINE_ID_ER_SP_PATH, u4Grp=APROC_ASM_ID_POSTPROC_3;    

    sInfo = (APROC_DSP_BIQ_INFO_T *) pu4Value;

    switch (u4Id)
    {
    case APROC_DSP_BIQ_ID_3DRC_0:
        u4Idx = APROC_REG_BIQ_SP_AVC_0_TYPE_0;
        break;
    case APROC_DSP_BIQ_ID_3DRC_1:
        u4Idx = APROC_REG_BIQ_SP_AVC_1_TYPE_0;
        break;
    case APROC_DSP_BIQ_ID_3DRC_2_1:
        u4Idx = APROC_REG_BIQ_SP_AVC_2_TYPE_0;
        break;
    case APROC_DSP_BIQ_ID_3DRC_2_2:
        u4Idx = APROC_REG_BIQ_SP_AVC_2_TYPE_1;
        break;

    case APROC_DSP_BIQ_ID_1DRC_0:
        u4Idx = APROC_REG_BIQ_SP_LSRS_AVC_0_TYPE_0;
        u4Routine=APROC_ROUTINE_ID_ER_SP_LSRS_PATH;
        u4Grp=APROC_ASM_ID_POSTPROC_F;
        break;   
        
    case APROC_DSP_BIQ_ID_3DRC_TARGET0:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_2, APROC_REG_AVC_SP_TARGET_0), *pu4Value);
        _vAprocSetRoutine(APROC_ROUTINE_ID_ER_SP_PATH);
        return; 
    case APROC_DSP_BIQ_ID_3DRC_TARGET1:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_2, APROC_REG_AVC_SP_TARGET_1), *pu4Value);
        _vAprocSetRoutine(APROC_ROUTINE_ID_ER_SP_PATH);
        return;               
    case APROC_DSP_BIQ_ID_3DRC_TARGET2:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_2, APROC_REG_AVC_SP_TARGET_2), *pu4Value);
        _vAprocSetRoutine(APROC_ROUTINE_ID_ER_SP_PATH);
        return;               
    case APROC_DSP_BIQ_ID_1DRC_TARGET0:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_F, APROC_REG_AVC_SP_LSRS_TARGET_0), *pu4Value);
        _vAprocSetRoutine(APROC_ROUTINE_ID_ER_SP_LSRS_PATH);
        return;               

    case APROC_DSP_BIQ_ID_3DRC_FLAG:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, APROC_REG_SP_AVC_FLAG), *pu4Value);
        _vAprocSetRoutine(APROC_ROUTINE_ID_ER_SP_PATH);
        return;
    case APROC_DSP_BIQ_ID_1DRC_FLAG:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_A, APROC_REG_SP_LSRS_AVC_FLAG), *pu4Value);
        _vAprocSetRoutine(APROC_ROUTINE_ID_ER_SP_LSRS_PATH);
        return;
        
    default:
        LOG (5, "_vAproc_PostProcDRC_Set: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    vAprocReg_Write (APROC_ASM_ADDR (u4Grp, u4Idx), sInfo->u4Type);
    vAprocReg_Write (APROC_ASM_ADDR (u4Grp, u4Idx+1), sInfo->u4Fc);
    vAprocReg_Write (APROC_ASM_ADDR (u4Grp, u4Idx+2), sInfo->u4Q);
    vAprocReg_Write (APROC_ASM_ADDR (u4Grp, u4Idx+3), sInfo->i4Gain);

    _vAprocSetRoutine (u4Routine);    
}

static void _vAproc_PostProcBassTreble_Set (UINT32 u4Id, UINT32 *pu4Value)
{
    APROC_DSP_BIQ_INFO_T *sInfo;
    UINT32 u4Idx, u4Routine=APROC_ROUTINE_ID_ER_SP_PATH;    

    sInfo = (APROC_DSP_BIQ_INFO_T *) pu4Value;

    switch (u4Id)
    {
    case APROC_DSP_BIQ_ID_BASS_SP:
        u4Idx = APROC_REG_BIQ_SP_BASS_TYPE;
        u4Routine=APROC_ROUTINE_ID_ER_SP_PATH;
        break;
    case APROC_DSP_BIQ_ID_BASS_SP_LSRS:
        u4Idx = APROC_REG_BIQ_SP_LSRS_BASS_TYPE;
        u4Routine=APROC_ROUTINE_ID_ER_SP_LSRS_PATH;
        break;
    case APROC_DSP_BIQ_ID_BASS_HP:
        u4Idx = APROC_REG_BIQ_HP_BASS_TYPE;
        u4Routine=APROC_ROUTINE_ID_ER_HP_PATH;
        break;    
    case APROC_DSP_BIQ_ID_TREBLE_SP:
        u4Idx = APROC_REG_BIQ_SP_TREBLE_TYPE;
        u4Routine=APROC_ROUTINE_ID_ER_SP_PATH;
        break;
    case APROC_DSP_BIQ_ID_TREBLE_SP_LSRS:
        u4Idx = APROC_REG_BIQ_SP_LSRS_TREBLE_TYPE;
        u4Routine=APROC_ROUTINE_ID_ER_SP_LSRS_PATH;        
        break;
    case APROC_DSP_BIQ_ID_TREBLE_HP:
        u4Idx = APROC_REG_BIQ_HP_TREBLE_TYPE;
        u4Routine=APROC_ROUTINE_ID_ER_HP_PATH;
        break;       
    case APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, APROC_REG_BIQ_SP_BASSTREBLE_FLAG), *pu4Value);
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);            
        return;      
    case APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP_LSRS:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, APROC_REG_BIQ_SP_LSRS_BASSTREBLE_FLAG), *pu4Value);
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_LSRS_PATH);    
        return;      
    case APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_HP:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, APROC_REG_BIQ_HP_BASSTREBLE_FLAG), *pu4Value);
        _vAprocSetRoutine(APROC_ROUTINE_ID_ER_HP_PATH);
        return;              
    default:
        LOG (5, "_vAproc_PostProcBass_Set: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, u4Idx), sInfo->u4Type);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, u4Idx+1), sInfo->u4Fc);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, u4Idx+2), sInfo->u4Q);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_5, u4Idx+3), sInfo->i4Gain);

    //_vAprocSetRoutine (u4Routine);    
}

static void _vAproc_PostProcChSel_Set (UINT32 u4Id, UINT32 *pu4Value)
{
    APROC_DSP_BIQ_INFO_T *sInfo;
    UINT32 u4Idx, u4Routine=APROC_ROUTINE_ID_ER_SP_PATH;    

    sInfo = (APROC_DSP_BIQ_INFO_T *) pu4Value;

    switch (u4Id)
    {
    case APROC_DSP_BIQ_ID_HPF_SP:
        u4Idx = APROC_REG_BIQ_SP_HPF_0_TYPE;
        u4Routine=APROC_ROUTINE_ID_ER_SP_PATH;
        break;
    case APROC_DSP_BIQ_ID_LPF_SP_LSRS:
        u4Idx = APROC_REG_BIQ_SP_LSRS_HPF_0_TYPE;
        u4Routine=APROC_ROUTINE_ID_ER_SP_LSRS_PATH;
        break;   
    case APROC_DSP_BIQ_ID_CHSEL_MODE:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_LSRS_FROM_LR_SEL), *pu4Value);
        _vAprocSetRoutine(APROC_ROUTINE_ID_ER_SP_PATH);
        _vAprocSetRoutine(APROC_ROUTINE_ID_ER_SP_LSRS_PATH);
        return;      
    default:
        LOG (5, "_vAproc_PostProcChSel_Set: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_6, u4Idx), sInfo->u4Type);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_6, u4Idx+1), sInfo->u4Fc);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_6, u4Idx+2), sInfo->u4Q);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_6, u4Idx+3), sInfo->i4Gain);

    _vAprocSetRoutine (u4Routine);    
}

static void _vAproc_PostProcFir_Set (UINT32 u4Id, UINT32 *pu4Value, UINT32 u4Num)
{
    if(u4Id == APROC_CONTROL_TYPE_FIR)
    {
        vAprocDataPort_Write (APROC_DATAPORT_SP_FIR_BASE, pu4Value, u4Num);
    }
}

static UINT32 _u4AprocSetFlagFunction (UINT32 u4AsmAddr, UINT32 u4Routine, UINT32 u4Flag)
{
    UINT32 u4Ret;

    vAprocReg_Write (u4AsmAddr, u4Flag);
    
    if (u4Flag & 0x100)
    {
        // with routine
        u4Ret = u4Routine;
    }
    else
    {
        // no routine
        u4Ret = APROC_IOCTRL_NO_ROUTINE;
    }

    return u4Ret;
}

static UINT32 _u4AprocGetFlagFunction (UINT32 u4AsmAddr, UINT32 u4Routine, UINT32 *pu4Flag)
{
    UINT32 u4Ret;
    UINT32 u4Flag;

    u4Flag = u4AprocReg_Read (u4AsmAddr);
    *pu4Flag = u4Flag;
    
    if (u4Flag & 0x100)
    {
        // with routine
        u4Ret = u4Routine;
    }
    else
    {
        // no routine
        u4Ret = APROC_IOCTRL_NO_ROUTINE;
    }

    return u4Ret;
}


static UINT32  _u4Aproc_PostProcDualMono_Set (UINT32 u4Item, UINT32 *pu4Value, UINT32 u4Num)
{
    UINT32 u4Routine;
    UINT32 u4Ret = APROC_IOCTRL_RET_OK;
    UINT32 u4Flag;

    switch (u4Item)
    {
    case APROC_IOCTRL_DUALMONO_AMXIER0_FLAG:        
        u4Routine = _u4AprocSetFlagFunction (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_0_FLAG),
            APROC_ROUTINE_ID_AER_AMIXER0_PATH,
            *pu4Value);
        break;
    case APROC_IOCTRL_DUALMONO_AMXIER1_FLAG:        
        u4Routine = _u4AprocSetFlagFunction (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_1_FLAG),
            APROC_ROUTINE_ID_AER_AMIXER1_PATH,
            *pu4Value);
        break;
    case APROC_IOCTRL_DUALMONO_AMXIER2_FLAG:        
        u4Routine = _u4AprocSetFlagFunction (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_2_FLAG),
            APROC_ROUTINE_ID_AER_AMIXER2_PATH,
            *pu4Value);
        break;
    case APROC_IOCTRL_DUALMONO_AMXIER0_MODE:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_0_MODE), *pu4Value);
        u4Routine = _u4AprocGetFlagFunction (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_0_FLAG),
            APROC_ROUTINE_ID_AER_AMIXER0_PATH,
            &u4Flag);
        break;
    case APROC_IOCTRL_DUALMONO_AMXIER1_MODE:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_1_MODE), *pu4Value);
        u4Routine = _u4AprocGetFlagFunction (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_1_FLAG),
            APROC_ROUTINE_ID_AER_AMIXER1_PATH,
            &u4Flag);
        break;    
    case APROC_IOCTRL_DUALMONO_AMXIER2_MODE:
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_2_MODE), *pu4Value);
        u4Routine = _u4AprocGetFlagFunction (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_2_FLAG),
            APROC_ROUTINE_ID_AER_AMIXER2_PATH,
            &u4Flag);
        break;
    default:
        return APROC_IOCTRL_RET_FAIL;
    }

    // issue routine
    if (u4Routine != APROC_IOCTRL_NO_ROUTINE)
    {
        _vAprocSetRoutine (u4Routine);
    }

    return u4Ret;
}

static void  _vAproc_PostProcDualMono_Get (UINT32 u4Item, UINT32 *pu4Value, UINT32 u4Num)
{
    UINT32 u4Addr;

    switch (u4Item)
    {
    case APROC_IOCTRL_DUALMONO_AMXIER0_FLAG:        
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_0_FLAG);
        break;    
    case APROC_IOCTRL_DUALMONO_AMXIER1_FLAG:        
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_1_FLAG);
        break;
    case APROC_IOCTRL_DUALMONO_AMXIER2_FLAG:        
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_2_FLAG);
        break;    
    case APROC_IOCTRL_DUALMONO_AMXIER0_MODE:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_0_MODE);
        break;        
    case APROC_IOCTRL_DUALMONO_AMXIER1_MODE:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_1_MODE);
        break;         
    case APROC_IOCTRL_DUALMONO_AMXIER2_MODE:
        u4Addr = APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_D, APROC_REG_DUALMONO_2_MODE);
        break;    
    default:
        //LOG (5, "_vAproc_PostProcVol_Set: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    *pu4Value = u4AprocReg_Read (u4Addr);
}


static void _vAproc_PostProcAd_Set (UINT32 *pu4Value, UINT32 u4Num)
{
    UINT32 u4Flag;
    UINT32 u4Mode;

    u4Flag = pu4Value[0] & 0x1;
    u4Mode = pu4Value[1] & 0x1;

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_COMM_2, APROC_REG_AMIXER2_ADMIX_FLAG), (u4Mode << 1) | u4Flag);
}

static void _vAproc_PostProcAd_Get (UINT32 *pu4Value, UINT32 u4Num)
{
    UINT32 u4Value;
    
    u4Value = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_COMM_2, APROC_REG_AMIXER2_ADMIX_FLAG));

    pu4Value[0] = u4Value & 0x1;
    pu4Value[1] = (u4Value >> 1) & 0x1;
}

static void _vAproc_PostProcMonoMix_Set (UINT32 u4Id, UINT32 *pu4Value, UINT32 u4Num)
{ 
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_1, APROC_REG_SPSW_MONOMIX_FLAG), *pu4Value);
}
 
static void _vAproc_PostProcMonoMix_Get (UINT32 u4Id, UINT32 *pu4Value, UINT32 u4Num)
{
    *pu4Value = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_1, APROC_REG_SPSW_MONOMIX_FLAG));
}

static void _vAproc_PostSel_Set (UINT32 u4Id, UINT32 *pu4Value)
{
    
    //LOG (0, "_vAproc_PostSel_Set : id [0x%x] = %0x!!!\n", u4Id, *pu4Value);

    if (u4Id>=APROC_REG_SEL_NUM)
    {
        //LOG (5, "_vAproc_PostSel_Set: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SEL_0, u4Id), *pu4Value);
}

static void _vAproc_PostSel_Get (UINT32 u4Id, UINT32 *pu4Value)
{
    
    if (u4Id>=APROC_REG_SEL_NUM)
    {
        //LOG (5, "_vAproc_PostSel_Get: wrong id = 0x%x!!!\n", u4Id);
        return;
    }
    *pu4Value = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_SEL_0, u4Id));
}

// IEC control flag set
// pu4Value[0] = flag, 0 = PCM, 1 = dec0 RAW, 2 = dec1 RAW
extern void vAdspSetIecFlag (UINT32 u4Flag);
static void _vAproc_PostProcIecFlagSetFunction (UINT32 *pu4Value)
{ 
    UINT32 u4Flag;
    UINT32 u4Dec;

    u4Flag = pu4Value[0];

    if (u4Flag == 0)
    {
        // PCM        
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_FLAG), 0); // change aproc iec to pcm
        // This register has already been set in _AudDspSetIec
        //AUD_WRITE32(REG_IEC_DIV, (AUD_READ32(REG_IEC_DIV) & (~IEC_AUDCK_SEL)) & (~IEC_CLK_INDEPENDENT_SEL)); // set IEC clock, from dec0        
        vAdspSetIecFlag (0x0); // stop ADSP raw output
    }
    else
    {
        // RAW
        // select RAW output decoder
        switch (u4Flag)
        {
        case 1:
            u4Dec = 0x10;
            break;
        case 2:
            u4Dec = 0x20;
            break;
        default:
            u4Dec = 0x0;
            break;
        }
        // This register has already been set in _AudDspSetIec
        //AUD_WRITE32(REG_IEC_DIV, (AUD_READ32(REG_IEC_DIV) | IEC_AUDCK_SEL) & (IEC_CLK_INDEPENDENT_SEL)); // set IEC clock, from dec1
        vAdspSetIecFlag (u4Dec | 0x1); // start ADSP raw output
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_FLAG), u4Flag); // change aproc iec to raw
    }
}

//pu4Value[0]: Mode: 1 = DTS CD, 0: others
static void _vAproc_PostProcIecModeSetFunction (UINT32 *pu4Value)
{ 
    UINT32 u4Mode;
    u4Mode = pu4Value[0];
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_MODE), u4Mode);
}

// IEC mute set
// pu4Value[0] = mute control, 0 = unmute, 1 = mute
static void _vAproc_PostProcIecMuteSetFunction (UINT32 *pu4Value)
{    
    // mute/unmmute both PCM and RAW
    if (*pu4Value)
    {
        // mute
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_RAWMUTE), 1);
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_0, APROC_REG_IEC_TRIM), 0x0);
    }
    else
    {
        // unmute
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_RAWMUTE), 0);
        vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_0, APROC_REG_IEC_TRIM), 0x7fffff);
    }
}

// IEC RAW information set
// pu4Value[0] = burst info
// pu4Value[1] = nsum (repitition period)
static void _vAproc_PostProcIecRawInfoSetFunction (UINT32 *pu4Value)
{    
    UINT32 u4BurstInfo, u4Nsum;

    u4BurstInfo = pu4Value[0];
    u4Nsum = pu4Value[1];

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_BURSTINFO), u4BurstInfo);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_NSNUM), u4Nsum);
}

// IEC RAW information get
// pu4Value[0] = burst info
// pu4Value[1] = nsum (repitition period)
static void _vAproc_PostProcIecRawInfoGetFunction (UINT32 *pu4Value)
{    
    pu4Value[0] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_BURSTINFO));
    pu4Value[1] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_NSNUM));
}

// IEC RAW delay set
// pu4Value[0] = delay frame number
static void _vAproc_PostProcIecRawDelaySetFunction (UINT32 *pu4Value)
{    
    UINT32 u4Delay;

    u4Delay = pu4Value[0];

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_RAWDELAY), u4Delay);
}

// IEC RAW delay get
// pu4Value[0] = delay frame number
static void _vAproc_PostProcIecRawDelayGetFunction (UINT32 *pu4Value)
{    
    pu4Value[0] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_RAWDELAY));
}

// IEC PCM channel selection set
// pu4Value[0] = channel selection
// 0 = SP (LR)
// 1 = SP (LsRs)
// 2 = SPSW (CLFE)
// 3 = IEC (78)
// 4 = SPDIF-in/Line-in
// 5 = HP (910)
// 6 = Line-in 2
// 7 = Line-in 3
// 8 = Mon 
// 9 = CH13/14 (Scart)
// 10 = AUX 
static void _vAproc_IecPcmChSelSetFunction (UINT32 *pu4Value)
{    
    UINT32 u4ChSel;
    UINT32 u4Ch0, u4Ch1,u4Dec;
    UINT32 u4Data;

    u4ChSel = pu4Value[0];

    u4Dec = 0;
    u4Ch0 = u4ChSel;
    u4Ch1 = 0;
    if (u4ChSel == 10)
    {
        u4Dec = 1;
        u4Ch0 = 0;
    }
    else if (u4ChSel >= 8)
    {
        u4Ch1 = 1;
        u4Ch0 -= 8; 
    }
        

    // audio HW
    u4Data = AUD_READ32 (REG_SPOCFG0);
    u4Data &= ~(0x0010011c);
    u4Data |= (u4Dec << 8);
    u4Data |= ((u4Ch0 & 0x7) << 2);
    u4Data |= ((u4Ch1 & 0x1) << 20);
    AUD_WRITE32 (REG_SPOCFG0, u4Data);
}

// IEC PCM channel selection get
// pu4Value[0] = delay frame number
static void _vAproc_IecPcmChSelGetFunction (UINT32 *pu4Value)
{    
    UINT32 u4ChSel;
    UINT32 u4Ch, u4Dec;
    UINT32 u4Data;

    // audio HW
    u4Data = AUD_READ32 (REG_SPOCFG0);

    u4Dec = (u4Data >> 8) & 0x1;
    u4Ch = (u4Data >> 2) & 0x7;

    u4ChSel = u4Ch;
    if (u4Dec == 1)
    {
        u4ChSel += 8;
    }

    pu4Value[0] = u4ChSel;
}

// IEC PCM output bits set
// pu4Value[0] = bits, 0 = 24-bits, 2 = 20-bits, 3 = 16-bits
static void _vAproc_IecPcmOutBitsSetFunction (UINT32 *pu4Value)
{    
    UINT32 u4Data;

    // audio HW
    u4Data = AUD_READ32 (REG_SPOCFG0);
    u4Data &= ~(0x00000003);
    u4Data |= (pu4Value[0] & 0x3);
    AUD_WRITE32 (REG_SPOCFG0, u4Data);
}

// IEC PCM output bits get
// pu4Value[0] = bits, 0 = 24-bits, 2 = 20-bits, 3 = 16-bits
static void _vAproc_IecPcmOutBitsGetFunction (UINT32 *pu4Value)
{    
    UINT32 u4Data;

    // audio HW
    u4Data = AUD_READ32 (REG_SPOCFG0);

    pu4Value[0] = u4Data & 0x3;
}

// IEC output set
// pu4Value[0] -> 0 = mute (no light), 1 = unmute, 2 = HDMI bypass
static void _vAproc_IecOutputSetFunction (UINT32 *pu4Value)
{    
    UINT32 u4Data;

    // FIXME!!!! audio HW
    //vIO32WriteFldAlign(CKGEN_PMUX0, pu4Value[0], FLD_PAD_ASPDIF0);
    
    if (pu4Value[0] == 2)
    {
        // HDMI bypass, need to setup IEC module to HDMI Rx
        u4Data = AUD_READ32 (REG_SPOCFG0);
        u4Data |= (1 << 12);
        AUD_WRITE32 (REG_SPOCFG0, u4Data);
    }
    else
    {
        // O.W. use audio module
        u4Data = AUD_READ32 (REG_SPOCFG0);
        u4Data &= ~(1 << 12);
        AUD_WRITE32 (REG_SPOCFG0, u4Data);
    }
}

// IEC output get
// pu4Value[0] -> 0 = mute (no light), 1 = unmute, 2 = HDMI bypass
static void _vAproc_IecOutputGetFunction (UINT32 *pu4Value)
{    
    //FIX ME!!!! audio HW
    //pu4Value[0] = IO32ReadFldAlign(CKGEN_PMUX0, FLD_PAD_ASPDIF0);
}

// IEC RAW info by audio format set
// pu4Value[0] -> audio format, APROC_IEC_AUDFMT_T
static void _vAproc_IecRawAudFmtSetFunction (UINT32 *pu4Value)
{    
    APROC_IEC_AUDFMT_T eAudFmt;
    SPDIF_BURST_INFO_T eBurstInfo;
    UINT32 u4Nsnum;
    UINT32 u4Data[2];

    eAudFmt = (APROC_IEC_AUDFMT_T) pu4Value[0];

    switch (eAudFmt)
    {
    case APROC_RAW_AC3:
        eBurstInfo = BURST_INFO_AC3;
        u4Nsnum = 0x600; // 1536 samples
        break;
    case    APROC_RAW_AAC: 
        eBurstInfo = BURST_INFO_AAC;
        u4Nsnum = 0x400; // 1024 samples
        break;
    case APROC_RAW_DTS_512:
        eBurstInfo = BURST_INFO_DTS_512;
        u4Nsnum = 0x200; // 512 samples
        break;
    case APROC_RAW_DTS_1024:
        eBurstInfo = BURST_INFO_DTS_1024;
        u4Nsnum = 0x400; // 1024 samples
        break;   
    case APROC_RAW_DTS_2048:
        eBurstInfo = BURST_INFO_DTS_2048;
        u4Nsnum = 0x800; // 2048 samples
        break;        
    case APROC_RAW_PCM:
        eBurstInfo = BURST_INFO_NULL_DATA;
        u4Nsnum = 0x0; 
        break;    
    default: // error case
        eBurstInfo = BURST_INFO_NULL_DATA;
        u4Nsnum = 0x0;
        break;
    }

    u4Data[0] = eBurstInfo;
    u4Data[1] = u4Nsnum;

    _vAproc_PostProcIecRawInfoSetFunction (&u4Data[0]);
}


// IEC channel status set
// pu4Value is defined as APROC_IEC_CHANNELSTATUS_T
static void _vAproc_PostProcIecChannelStatusSetFunction (UINT32 *pu4Value)
{    
    UINT32 u4ChCfg0, u4ChCfg1, u4ChCfg2;
    APROC_IEC_CHANNELSTATUS_T *eInfo;
    UINT32 u4Data = 0;

    eInfo = (APROC_IEC_CHANNELSTATUS_T *) pu4Value;

    u4ChCfg0 = (eInfo->u4CategoryCode & 0xFF) |
        ((eInfo->u4WordLength & 0xF) << 12);

    u4ChCfg1 = (eInfo->u4SampleRate & 0xF) |
        ((eInfo->u4AdditionalInfo & 0x7) << 4) |
        ((eInfo->u4CopyRight & 0x1) << 7) |
        ((eInfo->u4DigitalInfo & 0x1) << 8);

    u4Data = (eInfo->u4SourceNum & 0xF) |
        ((eInfo->u4ChannelNum0 & 0xF) << 4) |
        ((eInfo->u4ChannelNum1 & 0xF) << 8);

    u4ChCfg2 = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG2));
    u4ChCfg2 = (u4Data & 0xFFF) | (u4ChCfg2 & 0xF000);

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG0), u4ChCfg0);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG1), u4ChCfg1);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG2), u4ChCfg2);

    // audio HW
    u4Data = AUD_READ32 (REG_SPOCFG0);
    u4Data &= ~(0x00000c00);
    u4Data |= ((eInfo->u4ClockAccuracy & 0x3) << 10);
    AUD_WRITE32 (REG_SPOCFG0, u4Data);
}


// IEC channel status get
// pu4Value is defined as APROC_IEC_CHANNELSTATUS_T
static void _vAproc_PostProcIecChannelStatusGetFunction (UINT32 *pu4Value)
{    
    UINT32 u4ChCfg0, u4ChCfg1, u4ChCfg2;
    APROC_IEC_CHANNELSTATUS_T *eInfo;
    UINT32 u4Data;

    eInfo = (APROC_IEC_CHANNELSTATUS_T *) pu4Value;

    u4ChCfg0 = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG0));
    u4ChCfg1 = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG1));
    u4ChCfg2 = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG2));

    eInfo->u4CategoryCode = u4ChCfg0 & 0xFF;
    eInfo->u4WordLength = (u4ChCfg0 >> 12) & 0xF;

    eInfo->u4SampleRate = u4ChCfg1 & 0xF;
    eInfo->u4AdditionalInfo = (u4ChCfg1 >> 4) & 0x7;
    eInfo->u4CopyRight = (u4ChCfg1 >> 7) & 0x1;
    eInfo->u4DigitalInfo = (u4ChCfg1 >> 8) & 0x1;

    eInfo->u4SourceNum = u4ChCfg2 & 0xF;
    eInfo->u4ChannelNum0 = (u4ChCfg2 >> 4) & 0xF;
    eInfo->u4ChannelNum1 = (u4ChCfg2 >> 8) & 0xF;

    // audio HW
    u4Data = AUD_READ32 (REG_SPOCFG0);
    eInfo->u4ClockAccuracy = (u4Data >> 10) & 0x3;
}


// IEC mute get
// pu4Value[0] = pcm iec trim value
// pu4Value[1] = raw mute control
static void _vAproc_PostProcIecMuteGetFunction (UINT32 *pu4Value)
{    
    pu4Value[0] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_0, APROC_REG_IEC_TRIM));
    pu4Value[1] = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_RAWMUTE));
}

// IEC copy protect (L-bit, C-bit) set
// pu4Value[0] = L-bit
// pu4Value[1] = C-bit
static void _vAproc_PostProcIecCopyProtectSetFunction (UINT32 *pu4Value)
{ 
    UINT32 u4Cbit;
    UINT32 u4Lbit;
    UINT32 u4Cfg0, u4Cfg1;

    u4Lbit = pu4Value[0] & 0x1;
    u4Cbit = pu4Value[1] & 0x1;

    u4Cfg0 = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG0));
    u4Cfg0 &= (~(0x80));
    u4Cfg0 |= (u4Lbit << 7);
    u4Cfg1 = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG1));
    u4Cfg1 &= (~(0x80));
    u4Cfg1 |= (u4Cbit << 7);

    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG0), u4Cfg0);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG1), u4Cfg1);
}

// IEC control flag get
// pu4Value[0] = flag, 0 = PCM, 1 = RAW
static void _vAproc_PostProcIecFlagGetFunction (UINT32 *pu4Value)
{
    *pu4Value = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_FLAG));
}

static void _vAproc_PostProcIecModeGetFunction (UINT32 *pu4Value)
{
    *pu4Value = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_MODE));
}

// IEC copy protect (L-bit, C-bit) get
// pu4Value[0] = L-bit
// pu4Value[1] = C-bit
static void _vAproc_PostProcIecCopyProtectGetFunction (UINT32 *pu4Value)
{ 
    UINT32 u4Cbit;
    UINT32 u4Lbit;
    UINT32 u4Cfg0, u4Cfg1;

    u4Cfg0 = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG0));
    u4Lbit = (u4Cfg0 >> 7) & 0x1;
    u4Cfg1 = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_CHCFG1));
    u4Cbit= (u4Cfg1 >> 7) & 0x1;

    pu4Value[0] = u4Lbit;
    pu4Value[1] = u4Cbit;
}


static void _vAproc_PostProcIec_Set (UINT32 u4Id, UINT32 *pu4Value, UINT32 u4Num)
{
    switch (u4Id)
    {
    case APROC_IOCTRL_IEC_FLAG:
        _vAproc_PostProcIecFlagSetFunction (pu4Value);
        break;
    case APROC_IOCTRL_IEC_MODE:
        _vAproc_PostProcIecModeSetFunction (pu4Value);
        break;
    case APROC_IOCTRL_IEC_COPYPROTECT:
        _vAproc_PostProcIecCopyProtectSetFunction (pu4Value);
        break;
    case APROC_IOCTRL_IEC_MUTE:
        _vAproc_PostProcIecMuteSetFunction (pu4Value);
        break;
    case APROC_IOCTRL_IEC_CHANNELSTATUS:
        _vAproc_PostProcIecChannelStatusSetFunction (pu4Value);
        break;    
    case APROC_IOCTRL_IEC_RAWINFO:
        _vAproc_PostProcIecRawInfoSetFunction (pu4Value);
        break;
    case APROC_IOCTRL_IEC_RAWDELAY:
        _vAproc_PostProcIecRawDelaySetFunction (pu4Value);
        break;  
    case APROC_IOCTRL_IEC_PCMCHSEL:
        _vAproc_IecPcmChSelSetFunction (pu4Value);
        break;   
    case APROC_IOCTRL_IEC_PCMBITS:
        _vAproc_IecPcmOutBitsSetFunction (pu4Value);
        break;     
    case APROC_IOCTRL_IEC_OUTPUT:
        _vAproc_IecOutputSetFunction (pu4Value);
        break;    
    case APROC_IOCTRL_IEC_RAWAUDFMT:
        _vAproc_IecRawAudFmtSetFunction (pu4Value);
        break;             
    default:
        return;
    }
}

static void _vAproc_PostProcIec_Get (UINT32 u4Id, UINT32 *pu4Value, UINT32 u4Num)
{
    switch (u4Id)
    {
    case APROC_IOCTRL_IEC_FLAG:
        _vAproc_PostProcIecFlagGetFunction (pu4Value);
        break;
    case APROC_IOCTRL_IEC_MODE:
        _vAproc_PostProcIecModeGetFunction (pu4Value);
        break;
    case APROC_IOCTRL_IEC_COPYPROTECT:
        _vAproc_PostProcIecCopyProtectGetFunction (pu4Value);
        break;
    case APROC_IOCTRL_IEC_MUTE:
        _vAproc_PostProcIecMuteGetFunction (pu4Value);
        break;
    case APROC_IOCTRL_IEC_CHANNELSTATUS:
        _vAproc_PostProcIecChannelStatusGetFunction (pu4Value);
        break;
    case APROC_IOCTRL_IEC_RAWINFO:
        _vAproc_PostProcIecRawInfoGetFunction (pu4Value);
        break;    
    case APROC_IOCTRL_IEC_RAWDELAY:
        _vAproc_PostProcIecRawDelayGetFunction (pu4Value);
        break;
    case APROC_IOCTRL_IEC_PCMCHSEL:
        _vAproc_IecPcmChSelGetFunction (pu4Value);
        break;       
    case APROC_IOCTRL_IEC_PCMBITS:
        _vAproc_IecPcmOutBitsGetFunction (pu4Value);
        break;    
    case APROC_IOCTRL_IEC_OUTPUT:
        _vAproc_IecOutputGetFunction (pu4Value);
        break;        
    default:
        return;
    }
}
static void _vAproc_PostProcDBG_Set (UINT32 u4Id, UINT32 *pu4Value, UINT32 u4Num)
{
    UNUSED(u4Id);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SAP_MISC_23, APROC_REG_SPOST_GETVAR_ISIZE), u4Num); 
    u4Num = u4Num/4+1;
    vAprocDataPort_Write (APROC_DATAPORT_SAPOST_DEBUG_BASE, pu4Value, u4Num);
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_SAP_MISC_23, APROC_REG_SPOST_GETVAR_FLAG), 1); 
}

static void _vAproc_PostMixsndTrim (UINT32 *pu4Value)
{
    UINT32 u4Addr;
    UINT32 u4MixsondID;
    UINT32 u4TrimVol;
    u4MixsondID = pu4Value[0];
    u4TrimVol = pu4Value[1];

    switch (u4MixsondID)
    {
    case AUD_MIXSND_0:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_0, APROC_REG_INPUT_MIX0_TRIM);
        break;
    case AUD_MIXSND_1:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_0, APROC_REG_INPUT_MIX1_TRIM);
        break;
    case AUD_MIXSND_2:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_1, APROC_REG_INPUT_MIX2_TRIM);
        break;
    case AUD_MIXSND_3:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_1, APROC_REG_INPUT_MIX3_TRIM);
        break;
    case AUD_MIXSND_4:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_2, APROC_REG_INPUT_MIX4_TRIM);
        break;
    case AUD_MIXSND_5:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_2, APROC_REG_INPUT_MIX5_TRIM);
        break;
    case AUD_MIXSND_6:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_3, APROC_REG_INPUT_MIX6_TRIM);
        break;
    case AUD_MIXSND_7:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_3, APROC_REG_INPUT_MIX7_TRIM);
        break;
    default:
        return;
    }
    
    vAprocReg_Write(u4Addr, u4TrimVol);
    
}

static void _vAproc_PostMixsndInTrim (UINT32 *pu4Value) 
{
    UINT32 u4Addr;
    UINT32 u4MixsondID;
    UINT32 u4TrimVol;
    u4MixsondID = pu4Value[0];
    u4TrimVol = pu4Value[1];

    switch (u4MixsondID)
    {
    case AUD_MIXSND_0:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_0, APROC_REG_MIXSND0_IN_TRIM1);
        break;
    case AUD_MIXSND_1:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_0, APROC_REG_MIXSND1_IN_TRIM1);
        break;
    case AUD_MIXSND_2:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_1, APROC_REG_MIXSND2_IN_TRIM1);
        break;
    case AUD_MIXSND_3:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_1, APROC_REG_MIXSND3_IN_TRIM1);
        break;
    case AUD_MIXSND_4:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_2, APROC_REG_MIXSND4_IN_TRIM1);
        break;
    case AUD_MIXSND_5:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_2, APROC_REG_MIXSND5_IN_TRIM1);
        break;
    case AUD_MIXSND_6:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_3, APROC_REG_MIXSND6_IN_TRIM1);
        break;
    case AUD_MIXSND_7:
        u4Addr = APROC_ASM_ADDR(APROC_ASM_ID_MIXSND_3, APROC_REG_MIXSND7_IN_TRIM1);
        break;
    default:
        return;
    }
    
    vAprocReg_Write(u4Addr, u4TrimVol);
    
}

static void _vAproc_PostProcMixsnd_Set(UINT32 u4Id, UINT32 *pu4Value, UINT32 u4Num)
{
    switch (u4Id)
    {
    case APROC_IOCTRL_MIXSND_TRIM:
        _vAproc_PostMixsndTrim(pu4Value);
        break;
    case APROC_IOCTRL_MIXSND_IN_TRIM:
        _vAproc_PostMixsndInTrim(pu4Value);
        break;
    default: 
        break;
    }
}
 //-----------------------------------------------------------------------------
 /** _vAUD_Aproc_Set
  *  Audio processor set
  *
  *  @param  
 
  *  @retval void
  */
 //----------------------------------------------------------------------------- 
 void _vAUD_Aproc_Set (UINT32 u4Type, UINT32 u4Item, UINT32 *pu4Addr, UINT32 u4Num)
{
    
    UINT32 u4Ret;

    switch (u4Type)
    {
    case APROC_CONTROL_TYPE_SGAIN:
        _vAproc_PostProcSgain_Set (u4Item, *pu4Addr);
        break;
    case APROC_CONTROL_TYPE_VOL:        
        _vAproc_PostProcVol_Set (u4Item, *pu4Addr);
        break;
    case APROC_CONTROL_TYPE_TRIM:        
        _vAproc_PostProcTrim_Set (u4Item, *pu4Addr);
        break;    
    case APROC_CONTROL_TYPE_MUTE:
        _vAproc_PostProcMute_Set (u4Item, *pu4Addr);
        break;
    case APROC_CONTROL_TYPE_BALANCE:
        _vAproc_PostProcBalance_Set (u4Item, *pu4Addr);
        break;
    case APROC_CONTROL_TYPE_BASSTREBLE:
        _vAproc_PostProcBassTreble_Set (u4Item, pu4Addr);
        break;    
    case APROC_CONTROL_TYPE_FIR:
        _vAproc_PostProcFir_Set (u4Item, pu4Addr, u4Num);
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
        break;
    case APROC_CONTROL_TYPE_DUALMONO:
        u4Ret = _u4Aproc_PostProcDualMono_Set (u4Item, pu4Addr, u4Num);
        break; 
    case APROC_CONTROL_TYPE_AD:
        _vAproc_PostProcAd_Set (pu4Addr, u4Num);
        break;   
    case APROC_CONTROL_TYPE_MONOMIX:
        _vAproc_PostProcMonoMix_Set (u4Item, pu4Addr, u4Num);
        break;      
    case APROC_CONTROL_TYPE_IEC:
        _vAproc_PostProcIec_Set (u4Item, pu4Addr, u4Num);
        break;    
    case APROC_CONTROL_TYPE_SEL:
         //LOG (0, "APROC_CONTROL_TYPE_SEL: u4Item = 0x%x, data=%d \n", u4Item, *pu4Addr);
         _vAproc_PostSel_Set(u4Item, pu4Addr);
        break;
    case APROC_CONTROL_TYPE_CHSEL:
        _vAproc_PostProcChSel_Set(u4Item, pu4Addr);         
        break;                
    case APROC_CONTROL_TYPE_DRC:
        _vAproc_PostProcDRC_Set (u4Item, pu4Addr);
        break;              
    case APROC_CONTROL_TYPE_DBG:
        _vAproc_PostProcDBG_Set (u4Item, pu4Addr, u4Num);
        break;                      
/*Aproc Bmang*/
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
    case APROC_CONTROL_TYPE_BMANG:
        _vAproc_PostProcBmang_Set (pu4Addr);
        break;          
    case APROC_CONTROL_TYPE_MVS:
        _vAproc_PostProcMVS_Set (pu4Addr);
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
        break;          
    case APROC_CONTROL_TYPE_MVB:
        _vAproc_PostProcMVB_Set (pu4Addr);
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
        break;          
    case APROC_CONTROL_TYPE_PEQ:
        _vAproc_PostProcPEQ_Set (pu4Addr, u4Item);
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
        break;          
    case APROC_CONTROL_TYPE_EQ:
        _vAproc_PostProcEQ_Set (pu4Addr, u4Item, (UINT8)u4Num);
        _vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
        break;          
    case APROC_CONTROL_TYPE_LIMITER:
        _vAproc_PostProcLimiter_Set (u4Item, pu4Addr);
        break;          
#endif
    case APROC_CONTROL_TYPE_MIXSND:
        _vAproc_PostProcMixsnd_Set(u4Item, pu4Addr, u4Num); 
    default:
        LOG (0, "_AUD_AprocSet: error type = 0x%x!!!\n", u4Type);
        break;
    }    
}

//-----------------------------------------------------------------------------
 /** _vAUD_Aproc_Get
  *  Audio processor get
  *
  *  @param  
 
  *  @retval void
  */
 //-----------------------------------------------------------------------------
 void _vAUD_Aproc_Get (UINT32 u4Type, UINT32 u4Item, UINT32 *pu4Addr, UINT32 u4Num)
{

    switch (u4Type)
    {
    case APROC_CONTROL_TYPE_SGAIN:
        _vAproc_PostProcSgain_Get (u4Item, pu4Addr);
        break;
    case APROC_CONTROL_TYPE_VOL:        
        _vAproc_PostProcVol_Get (u4Item, pu4Addr);
        break;
    case APROC_CONTROL_TYPE_TRIM:        
        _vAproc_PostProcTrim_Get (u4Item, pu4Addr);
        break; 
    case APROC_CONTROL_TYPE_MUTE:
        _vAproc_PostProcMute_Get (u4Item, pu4Addr);
        break;
    case APROC_CONTROL_TYPE_BALANCE:
        _vAproc_PostProcBalance_Get (u4Item, pu4Addr);
        break;
    case APROC_CONTROL_TYPE_DUALMONO:        
        _vAproc_PostProcDualMono_Get (u4Item, pu4Addr, u4Num);
        break; 
    case APROC_CONTROL_TYPE_AD:        
        _vAproc_PostProcAd_Get (pu4Addr, u4Num);
        break;   
    case APROC_CONTROL_TYPE_MONOMIX:        
        _vAproc_PostProcMonoMix_Get (u4Item, pu4Addr, u4Num);
        break;      
    case APROC_CONTROL_TYPE_IEC:
        _vAproc_PostProcIec_Get (u4Item, pu4Addr, u4Num);
        break;     
    case APROC_CONTROL_TYPE_SEL:
         _vAproc_PostSel_Get(u4Item, pu4Addr);
        break;  
    case APROC_CONTROL_TYPE_BASSTREBLE:
        _vAproc_PostProcBassTreble_Get(u4Item, pu4Addr);         
        break;           
    case APROC_CONTROL_TYPE_CHSEL:
        _vAproc_PostProcChSel_Get(u4Item, pu4Addr);         
        break;        
    case APROC_CONTROL_TYPE_DRC:
        _vAproc_PostProcDRC_Get (u4Item, pu4Addr);
        break;        
    case APROC_CONTROL_TYPE_DBG:
        _vAproc_PostProcDBG_Get (u4Item, pu4Addr);
        break;                
/*Aproc Bmang*/
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
    case APROC_CONTROL_TYPE_BMANG:
        _vAproc_PostProcBmang_Get (pu4Addr);
        break;          
    case APROC_CONTROL_TYPE_MVS:
        _vAproc_PostProcMVS_Get (pu4Addr);
        break;          
    case APROC_CONTROL_TYPE_MVB:
        _vAproc_PostProcMVB_Get (pu4Addr);
        break;          
    case APROC_CONTROL_TYPE_PEQ:
        _vAproc_PostProcPEQ_Get (pu4Addr, u4Item);
        break;          
    case APROC_CONTROL_TYPE_EQ:
        _vAproc_PostProcEQ_Get (pu4Addr, u4Item, (UINT8)u4Num);
        break;          
    case APROC_CONTROL_TYPE_LIMITER:
        _vAproc_PostProcLimiter_Get (u4Item, pu4Addr);
        break;          
#endif      
    default:
        LOG (0, "_vAUD_Aproc_Get: error type = 0x%x!!!\n", u4Type);
        break;
    }
}

void _vAUD_Aproc_SetMp3Enc(BOOL fgEnable)
{
    UINT32 u4Value;
    u4Value = (UINT32)(fgEnable ? 0x1 : 0x0);
    vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_ENC_MP3, APROC_REG_IOBUF_INTBUF4_ENA), u4Value);
}

#ifdef CC_AUD_DDI
UINT32 _au4MixSoundInputVolume[AUD_MIXSND_NUM] = {VOL_SHM_0_DB,VOL_SHM_0_DB,VOL_SHM_0_DB,VOL_SHM_0_DB,VOL_SHM_0_DB,VOL_SHM_0_DB,VOL_SHM_0_DB,VOL_SHM_0_DB};
BOOL _fgMixSoundInputMute[AUD_MIXSND_NUM] = {FALSE};
BOOL _fgSpdifConnet[AUD_DEC_MAX] = {FALSE};
BOOL _fgInputMute[AUD_DEC_MAX] = {FALSE};

CHAR *_paAudOutPortName[AUD_OUT_PORT_NUM] = 
{
    "Speaker",
    "Headphone",
    "SPDIF",
    "AV_Out",
    "Monitor",
    "SCART",
    "Bluetooth",
    "LGSE"
};

AUD_ENUM_TO_NAME_T eAudOutSelTbl[] = 
{
    {APROC_OUT_SEL_DEC0, "AUD_DEC0"},
    {APROC_OUT_SEL_DEC1, "AUD_DEC1"},
    {APROC_OUT_SEL_MIX0, "PCM_MIX0"},
    {APROC_OUT_SEL_MIX1, "PCM_MIX1"},
    {APROC_OUT_SEL_MIX2, "PCM_MIX2"},
    {APROC_OUT_SEL_MIX3, "PCM_MIX3"},
    {APROC_OUT_SEL_MIX4, "PCM_MIX4"},
    {APROC_OUT_SEL_MIX5, "PCM_MIX5"},
    {APROC_OUT_SEL_MIX6, "PCM_MIX6"},
    {APROC_OUT_SEL_MIX7, "PCM_MIX7"},
    {APROC_OUT_SEL_LGSE, "LGSE"}
};
static UINT32 _AUD_UserGetDrvVol(UINT8 u1MainVol, UINT8 u1FineVol)
{
    UINT32 u4Volumn;
    UINT64 u8ChlVol;

    if (u1MainVol >157)//actually not large than +30dB
    {
        u1MainVol = 157;
    }

    if (u1FineVol >15)
    {
        u1FineVol = 15;
    }
    
    u8ChlVol = ((UINT64)_au4CHVOLUMENEW[u1MainVol]) * ((UINT64)_au4CHVOLUMENEW1[u1FineVol]);
    u4Volumn = (UINT32)u8Div6432(u8ChlVol, (UINT64)VOL_SHM_0_DB, NULL);
    return u4Volumn;
}

//currently Mixsound trim
void _AUD_UserSetMixSndInputVol(UINT8 u4MixIdex, UINT8 u1MainVol, UINT8 u1FineVol)
{
    UINT32 u4Volumn;
    INT32 i4Vol;
    UINT32 pu4Addr[2];

    AUD_MIXSND_ID_VALIDATE(u4MixIdex); 
    u4Volumn = _AUD_UserGetDrvVol(u1MainVol, u1FineVol);
    _au4MixSoundInputVolume[u4MixIdex] = u4Volumn;
    i4Vol = _AudDspToAprocVol(u4Volumn);
    if (_fgMixSoundInputMute[u4MixIdex])
    {
        i4Vol = 0;
    }
    pu4Addr[0] = u4MixIdex;
    pu4Addr[1] = (UINT32)i4Vol;
    
    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_MIXSND, APROC_IOCTRL_MIXSND_TRIM, pu4Addr, 2);
}

void _AUD_UserSetMixSndMute(UINT8 u4MixIdex, UINT8 u1Mute)
{
    INT32 i4Vol;
    UINT32 pu4Addr[2];

    AUD_MIXSND_ID_VALIDATE(u4MixIdex);

    LOG(0, "Mixer Mute PCM_MIX%d, MuteEnable(%d)\n", u4MixIdex, u1Mute); 
    _fgMixSoundInputMute[u4MixIdex] = (BOOL) u1Mute; 
    i4Vol = _AudDspToAprocVol(_au4MixSoundInputVolume[u4MixIdex]);
    if (_fgMixSoundInputMute[u4MixIdex])
    {
        i4Vol = 0;
    }
    pu4Addr[0] = u4MixIdex;
    pu4Addr[1] = (UINT32)i4Vol;
    
    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_MIXSND, APROC_IOCTRL_MIXSND_TRIM, pu4Addr, 2);
}

void _AUD_UserSetMixSndOutputVol(UINT8 u4MixIdex, UINT8 u1MainVol, UINT8 u1FineVol)
{
    UINT32 u4Volumn;
    INT32 i4Vol;
    UINT32 pu4Addr[2];

    AUD_MIXSND_ID_VALIDATE(u4MixIdex);
    
    u4Volumn = _AUD_UserGetDrvVol(u1MainVol, u1FineVol);
    i4Vol = _AudDspToAprocVol(u4Volumn);
    pu4Addr[0] = u4MixIdex;
    pu4Addr[1] = (UINT32)i4Vol;
    
    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_MIXSND, APROC_IOCTRL_MIXSND_IN_TRIM, pu4Addr, 2);
}

void _AUD_UserSetDecInputMute(UINT8 u1DecId, BOOL fgMute)
{  
    UINT32 u4Idx;
    INT32 i4Vol;

    AUD_DEC_ID_VALIDATE(u1DecId);

    LOG(0, "Input Mute AUD_DEC%d, MuteEnable(%d)\n", u1DecId, fgMute);

    if (fgMute)
    {
        i4Vol = 0;
    }
    else
    {
        i4Vol = 0x7fffff; 
    }
    u4Idx = APROC_IOCTR_TRIM_AMIXER0; 
    if (u1DecId == AUD_DEC_AUX)
    {
        u4Idx = APROC_IOCTR_TRIM_AMIXER1;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4Idx = APROC_IOCTR_TRIM_AMIXER2;
    } 
    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_TRIM, u4Idx, &i4Vol, 1);
    if (_u1SpdifRawDec == u1DecId)
    {   
        if(!fgMute && _fgSpdifConnet[u1DecId]) //unmute raw
        {
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_RAWMUTE), 0);
        }
        else
        {
            vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_RAWMUTE), 1); 
        }
    }
    if (fgMute && !_fgInputMute[u1DecId])
    {
        //wait fade out done to avoid pop noise when changing channel or input source
        x_thread_delay(30);
    }
    _fgInputMute[u1DecId] = fgMute; 

}

void _AUD_UserSetDecInputVol(UINT8 u1DecId, UINT8 u1MainVol, UINT8 u1FineVol)
{  
    UINT32 u4Idx;
    UINT32 u4Volumn;
    INT32 i4Vol;

    AUD_DEC_ID_VALIDATE(u1DecId);
    
    u4Volumn = _AUD_UserGetDrvVol(u1MainVol, u1FineVol); 
    i4Vol = _AudDspToAprocVol(u4Volumn);
    u4Idx = APROC_IOCTRL_VOL_AMIXER0;

    if (u1DecId == AUD_DEC_AUX)
    {
        u4Idx = APROC_IOCTRL_VOL_AMIXER1;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4Idx = APROC_IOCTRL_VOL_AMIXER2;
    } 
    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_VOL, u4Idx, &i4Vol, 1);
    
}

void _AUD_UserSetDecOutputVol(UINT8 u1DecId, UINT8 u1MainVol, UINT8 u1FineVol)
{  
    UINT32 u4Idx;
    UINT32 u4Volumn;
    INT32 i4Vol;
    
    AUD_DEC_ID_VALIDATE(u1DecId);

    u4Volumn = _AUD_UserGetDrvVol(u1MainVol, u1FineVol);
    i4Vol = _AudDspToAprocVol(u4Volumn);
    u4Idx = APROC_IOCTR_TRIM_INPUT0;

    if (u1DecId == AUD_DEC_AUX)
    {
        u4Idx = APROC_IOCTR_TRIM_INPUT1;
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4Idx = APROC_IOCTR_TRIM_INPUT2;
    } 
    _vAUD_Aproc_Set(APROC_CONTROL_TYPE_TRIM, u4Idx, &i4Vol, 1);
    
}

void _AUD_UserSetDecChannelGain(UINT8 u1DecId, UINT8 u1LetfMainVol, 
                    UINT8 u1LeftFineVol, UINT8 u1RightMainVol, UINT8 u1RightFineVol)
{  
    UINT32 u4Volumn;
    INT32 i4Vol;
    UINT32 u4Idx = APROC_REG_INPUT0_BALANCE_L;

    AUD_DEC_ID_VALIDATE(u1DecId);
    
    switch (u1DecId)
    {
    case AUD_DEC_MAIN:
        u4Idx = APROC_REG_INPUT0_BALANCE_L;
        break;
    case AUD_DEC_AUX:
        u4Idx = APROC_REG_INPUT1_BALANCE_L;
        break;
    case AUD_DEC_THIRD:
        u4Idx = APROC_REG_INPUT2_BALANCE_L;
        break;               
    default:
        LOG (5, "_vAproc_PostProcBalance_Set: wrong id = 0x%x!!!\n", u1DecId);
        return;
    }

    u4Volumn = _AUD_UserGetDrvVol(u1LetfMainVol, u1LeftFineVol); 
    i4Vol = _AudDspToAprocVol(u4Volumn);

    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_1, u4Idx), (UINT32)i4Vol);

    u4Volumn = _AUD_UserGetDrvVol(u1RightMainVol, u1RightFineVol); 
    i4Vol = _AudDspToAprocVol(u4Volumn);
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_POSTPROC_1, u4Idx+1), (UINT32)i4Vol);
    
}

void _AUD_UserSetDecInputDelay(UINT8 u1DecId, UINT16 u2DelayTime)
{ 
    u2DelayTime = u2DelayTime * 48 / APROC_POSTBUF_BLOCK_SIZE;

    AUD_DEC_ID_VALIDATE(u1DecId);

    if (u1DecId == AUD_DEC_MAIN)
    {
        vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_3C, APROC_REG_DELAY_INPUT0), u2DelayTime);
        _vAprocSetRoutine(APROC_ROUTINE_ID_DR_INPUT0_PATH);
    }
    else
    {
        vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_POSTPROC_3C, APROC_REG_DELAY_INPUT1), u2DelayTime); 
        _vAprocSetRoutine(APROC_ROUTINE_ID_DR_INPUT1_PATH);
    }
}

void _AUD_UserSetDecOutCtrl(AUD_OUT_PORT_T eAudioOutPort, UINT32 u4OutSel, BOOL fgEnable)
{
    UINT32 u4Reg, u4OutSelVal;
    UINT8 u1DecId = 0xFF;
    CHAR * paConnect[2] = {"Disconnect", "Connect"};
    
    AUD_OUT_PORT_VALIDATE(eAudioOutPort);

    LOG(0, "SoundConnect:  %-10s %-10s %s\n", _paAudOutPortName[eAudioOutPort],paConnect[fgEnable], 
        AUD_EnumToName(eAudOutSelTbl, AUD_ARRAY_SIZE(eAudOutSelTbl), u4OutSel));
    
    switch (eAudioOutPort)
    {
    case AUD_SPEAKER:
        u4Reg = APROC_REG_SEL_DSP_SP_IN;
        break;
    case AUD_HEADPHONE:
        u4Reg = APROC_REG_SEL_DSP_HP_IN;
        break;
    case AUD_SPDIF:
        u4Reg = APROC_REG_SEL_DSP_SPDIF_IN;
        if (u4OutSel & APROC_OUT_SEL_DEC0)
        {
            u1DecId = AUD_DEC_MAIN;
            _fgSpdifConnet[u1DecId] = fgEnable; 
        }
        else if (u4OutSel & APROC_OUT_SEL_DEC1)
        {
            u1DecId = AUD_DEC_AUX; 
            _fgSpdifConnet[u1DecId] = fgEnable; 
        }
        if (_u1SpdifRawDec == u1DecId)
        { 
            if (fgEnable && !_fgInputMute[u1DecId])
            { 
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_RAWMUTE), 0);
            }
            else
            {
                vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_AENV_1, APROC_REG_AENV_IEC_RAWMUTE), 1);
            }
        }
        break;
    case AUD_AV_OUT:
        u4Reg = APROC_REG_SEL_DSP_MON_IN;
        break;
    case AUD_MONITOR_SCART:
        u4Reg = APROC_REG_SEL_DSP_MON_IN;
        break;        
    case AUD_TV_SCART:
        u4Reg = APROC_REG_SEL_SCART_IN;
        break;
    case AUD_BLUETOOTH:
        u4Reg = APROC_REG_SEL_DSP_BT_IN;
        break;
    case AUD_PORT_LGSE:
        u4Reg = APROC_REG_SEL_LGSE_IN;
        if (u4OutSel == APROC_OUT_SEL_LGSE)
        {
            LOG(0, "LGSE input cound be himself\n");
            return;
        }
        break;        
    default:
        u4Reg = APROC_REG_SEL_DSP_SP_IN;
        return;        
    }

    //NOTE: LGSE input could not be itself
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_SEL, u4Reg, &u4OutSelVal, 1);

    if (fgEnable)
    {
        u4OutSelVal |= u4OutSel;
    }
    else
    {
        u4OutSelVal &= ~u4OutSel; 
    }
    
    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, u4Reg, &u4OutSelVal, 1);   
}
#endif
#endif //defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER) //#A0005

#ifdef CC_AUD_ARM_POST
#ifdef CC_AUD_SUPPORT_DBXTV

static BOOL _fgDBXTVTestEnable = FALSE;
static DBXTV_TotSonTestMode _eDBXTVTotSonMode = TOTSON_NORMAL;
static DBXTV_TotSonMode _eTotSonMode = TOTSON_OFF;
static DBXTV_TotVolMode _eTotVolMode = TOTVOL_OFF;
static DBXTV_TotSurMode _eTotSurMode = TOTSUR_OFF;
static UINT32 _DBXTVEnable = 0;


void AUD_DBXTV_Enable(BOOL fgEnable)
{
    LOG(5, "%s fgEnable=%d\n", __FUNCTION__, fgEnable);
    if (fgEnable)
    {
        vAprocReg_Write(APROC_REG_DBX_ENABLE, 1);
        AUD_DBXTV_ReInit();
    }
    else
    {
        vAprocReg_Write(APROC_REG_DBX_ENABLE, 0); 
    }
}

BOOL AUD_DBXTV_GetEnable(void)
{
    return (u4AprocReg_Read (APROC_REG_DBX_ENABLE) == 1);
}

void AUD_DBXTV_ReInit(void)
{
    AudAoutGainCtrl(AUD_DEC_MAIN, 0, 0, AOUT_VOL_SETP_4DB); 
    x_thread_delay(20);
    vAprocReg_Write(APROC_REG_DBX_REINIT, 1); 
    AudAoutGainCtrl(AUD_DEC_MAIN, 0, 0x1000, AOUT_VOL_SETP_4DB);

}

/**
    Function to set the dbx-tv mode.
    DBXTV_SetMode() takes the Total Sonics, Total Volume, and Total Surround
    modes as inputs and determines the parameters to load into the data and
    coefficient memory.
    @param TSonMode one of the values of DBXTV_TSonMode
    @param TVolMode one of the values of DBXTV_TVolMode
    @param TSurMode one of the values of DBXTV_TSurMode
    @param Enable logical OR of Enable_XXX values defined in DBX_API_code.h
    @return None
*/
void AUD_DBXTV_SetMode(DBXTV_TotSonMode TotSonMode, DBXTV_TotVolMode TotVolMode, 
                                                  DBXTV_TotSurMode TotSurMode, UINT32 Enable)
{
    // Mute audio
    //OS_AudioMute();
    _eTotSonMode = TotSonMode;
    _eTotVolMode = TotVolMode;
    _eTotSurMode = TotSurMode;
    _DBXTVEnable = Enable;

    LOG(5, "%s TotSonics=%d, TotVolume=%d, TotSurround=%d, Enable=0x%x\n", __FUNCTION__, 
        TotSonMode, TotVolMode, TotSurMode, Enable);

    // Turn off dbx-tv
    vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_DVCOnOff, 0x000000);
    vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_DPPOnOff, 0x000000);
    vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_IWBypass, 0x000001);
    vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_EQBypass, 0x000001);
    vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_BEOnOff, 0x000000);

    //Total Sonics: All
    if ( (TotSonMode != TOTSON_OFF) )
    {
        // Copy the Total Sonics preset to memory
        vAprocReg_Write_Multi(APROC_REG_DBX_PARAM_BASE, 
                (UINT32 *)&TotSonParam_drv[TotSonMode], DBXTV_Param_LENGTH);
        
        vAprocReg_Write_Multi(APROC_REG_DBX_COEFF_BASE, 
                (UINT32 *)&TotSonCoeff_drv[TotSonMode], DBXTV_Coeff_LENGTH);
    }

    //Total volume: DVC and DPP, [DBX_Param_OFFSET_DVC, DBX_Param_OFFSET_IW)
    //So if index in this range we should modify, TotVolParam
    //[DBX_Coeff_OFFSET_DVC, DBX_Coeff_OFFSET_IW)
    //TotVolCoeff
    if ( (TotVolMode != TOTVOL_OFF) )
    {
        // Copy the Total Volume DVC preset to memory
        vAprocReg_Write_Multi(APROC_REG_DBX_PARAM_BASE+DBX_Param_OFFSET_DVC, 
                (UINT32 *)&TotVolParam[TotVolMode][DBX_Param_OFFSET_DVC], DBX_Param_LENGTH_DVC);
        
        vAprocReg_Write_Multi(APROC_REG_DBX_COEFF_BASE+DBX_Coeff_OFFSET_DVC, 
                (UINT32 *)&TotVolCoeff[TotVolMode][DBX_Coeff_OFFSET_DVC], DBX_Coeff_LENGTH_DVC);
        
        // Copy the Total Volume DPP preset to memory
        vAprocReg_Write_Multi(APROC_REG_DBX_PARAM_BASE+DBX_Param_OFFSET_DPP, 
                (UINT32 *)&TotVolParam[TotVolMode][DBX_Param_OFFSET_DPP], DBX_Param_LENGTH_DPP);
        
        vAprocReg_Write_Multi(APROC_REG_DBX_COEFF_BASE+DBX_Coeff_OFFSET_DPP, 
                (UINT32 *)&TotVolCoeff[TotVolMode][DBX_Coeff_OFFSET_DPP], DBX_Coeff_LENGTH_DPP);
    }

    if ( (TotSurMode != TOTSUR_OFF) )
    {
        if (TotVolMode == TOTVOL_OFF)
        {
            // Copy the Total Surround level parameters to memory
            vAprocReg_Write_Multi(APROC_REG_DBX_PARAM_BASE+DBX_Param_OFFSET_DVCHiLevel,
                    (UINT32 *)&TotSurParam[TotSurMode][DBX_Param_OFFSET_DVCHiLevel], 1);
            
            vAprocReg_Write_Multi(APROC_REG_DBX_PARAM_BASE+DBX_Param_OFFSET_DVCLoLevel, 
                    (UINT32 *)&TotSurParam[TotSurMode][DBX_Param_OFFSET_DVCLoLevel],  1);
        }
        if (TotVolMode != TOTVOL_NIGHT)
        {
            // Copy the Total Surround IW preset to memory
            vAprocReg_Write_Multi(APROC_REG_DBX_PARAM_BASE+DBX_Param_OFFSET_IW, 
                    (UINT32 *)&TotSurParam[TotSurMode][DBX_Param_OFFSET_IW], DBX_Param_LENGTH_IW);
            
            vAprocReg_Write_Multi(APROC_REG_DBX_COEFF_BASE+DBX_Coeff_OFFSET_IW, 
                    (UINT32 *)&TotSurCoeff[TotSurMode][DBX_Coeff_OFFSET_IW], DBX_Coeff_LENGTH_IW);
            
            // Copy the Total Surround DPP preset to memory
            vAprocReg_Write_Multi(APROC_REG_DBX_PARAM_BASE+DBX_Param_OFFSET_DPP, 
                    (UINT32 *)&TotSurParam[TotSurMode][DBX_Param_OFFSET_DPP], DBX_Param_LENGTH_DPP);
            
            vAprocReg_Write_Multi(APROC_REG_DBX_COEFF_BASE+DBX_Coeff_OFFSET_DPP, 
                    (UINT32 *)&TotSurCoeff[TotSurMode][DBX_Coeff_OFFSET_DPP], DBX_Coeff_LENGTH_DPP);
        }
    }

    if (TotVolMode == TOTVOL_NIGHT)
    {
        // Turn off IW
        vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_IWBypass, 0x000001);
    }

    if ((Enable&Enable_DVC) == 0)
    {
        // Bypass DVC
        vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_DVCOnOff, 0x000000);
    }
    if ((Enable&Enable_DPP) == 0)
    {
        // Bypass DPP
        vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_DPPOnOff, 0x000000);
    }
    if ((Enable&Enable_IW) == 0)
    {
        // Bypass IW
        vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_IWBypass, 0x000001);
    }
    if ((Enable&Enable_EQ) == 0)
    {
        // Bypass EQ
        vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_EQBypass, 0x000001);
    }
    if ((Enable&Enable_BE) == 0)
    {
        // Bypass BE
        vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_BEOnOff, 0x000000);
    }

    AUD_DBXTV_ReInit();
    
    // Unmute audio
    //OS_AudioUnmute();

}


void AUD_DBXTV_GetMode(DBXTV_TotSonMode *pTotSonMode, DBXTV_TotVolMode *pTotVolMode, 
                                                  DBXTV_TotSurMode *pTotSurMode, UINT32 *pEnable)
{
    *pTotSonMode =  _eTotSonMode;
    *pTotVolMode = _eTotVolMode;
    *pTotSurMode = _eTotSurMode;
    *pEnable = _DBXTVEnable; 
}

//-----------------------------------------------------------------------------
/** 
 *   The following code is for DBX TV test vector verification
 */
//-----------------------------------------------------------------------------
void AUD_DBXTV_SetTestEnable(BOOL fgEnable)
{
    LOG(5, "%s fgEnable=%d\n", __FUNCTION__, fgEnable);
    _fgDBXTVTestEnable= fgEnable;
}

BOOL AUD_DBXTV_GetTestEnable(void)
{
    return _fgDBXTVTestEnable;
}

DBXTV_TotSonTestMode AUD_DBXTV_GetTestMode(void)
{
    return _eDBXTVTotSonMode;
}

void AUD_DBXTV_SetTestMode(DBXTV_TotSonTestMode TotSonMode, UINT32 Enable)
{
    _eDBXTVTotSonMode = TotSonMode;
    
    // Turn off dbx-tv
    vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_DVCOnOff, 0x000000);
    vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_DPPOnOff, 0x000000);
    vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_IWBypass, 0x000001);
    vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_EQBypass, 0x000001);
    vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_BEOnOff, 0x000000);

    vAprocReg_Write_Multi(APROC_REG_DBX_PARAM_BASE, (UINT32 *)&TotPresetParam[TotSonMode], DBXTV_Param_LENGTH);
    vAprocReg_Write_Multi(APROC_REG_DBX_COEFF_BASE, (UINT32 *)&TotPresetCoeff[TotSonMode], DBXTV_Coeff_LENGTH);

    LOG(5, "AUD_DBXTV_SetMode TotSonMode=%d, Enable=%d\n", TotSonMode, Enable);
    if ((Enable&Enable_DVC) == 0)
    {
        // Bypass DVC
        vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_DVCOnOff, 0x000000);
    }
    if ((Enable&Enable_DPP) == 0)
    {
        // Bypass DPP
        vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_DPPOnOff, 0x000000);
    }
    if ((Enable&Enable_IW) == 0)
    {
        // Bypass IW
        vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_IWBypass, 0x000001);
    }
    if ((Enable&Enable_EQ) == 0)
    {
        // Bypass EQ
        vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_EQBypass, 0x000001);
    }
    if ((Enable&Enable_BE) == 0)
    {
        // Bypass BE
        vAprocReg_Write(APROC_REG_DBX_PARAM_BASE + DBX_Param_OFFSET_BEOnOff, 0x000000);
    }

    AUD_DBXTV_ReInit();

}

#endif //CC_AUD_SUPPORT_DBXTV
#endif //CC_AUD_ARM_POST


#ifdef CC_SUPPORT_STR
void _AUD_PostProc_Resume(void)
{
    UINT8 i = 0;
    AUD_DspChannelEqEnable(AUD_DEC_MAIN, _aResumePostInfo.fgEQEnable);

#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    if (_aResumePostInfo.fgVirSur1Enable)
    {
        _AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, TRUE);
    }
    
    if (_aResumePostInfo.fgVirSur2Enable)
    {
        _AUD_DspVirtualSurroundEnalbe(AUD_DEC_AUX, TRUE);
    }
#else
    _au1VSCurrentIdx = 0xff;
    if (_aResumePostInfo.fgVirSur1Enable)
    {
        _AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, _au1MWSetVSIdx);
    }

    if (_aResumePostInfo.fgVirSur2Enable)
    {
        _AUD_DspVirtualSurroundEnalbe(AUD_DEC_AUX, _au1MWSetVSIdx);
    }
#endif
    if (_aResumePostInfo.fgADHpEnable)
    {
        _AUD_DspADModeSet(AUD_DEC_MAIN, 0);
    }
    
    if (_aResumePostInfo.fgADSpkEnable)
    {
        _AUD_DspADModeSet(AUD_DEC_MAIN, 1);
    }
    
    if (_aResumePostInfo.fgSpkHpEnable)
    {
        _AUD_DspADModeSet(AUD_DEC_MAIN, 2);
    }

    if ((_aResumePostInfo.fgSpkHpEnable == FALSE) &&
        (_aResumePostInfo.fgADHpEnable == FALSE) &&
        (_aResumePostInfo.fgADSpkEnable == FALSE))
    {
        _AUD_DspADModeSet(AUD_DEC_MAIN, 4); //default
    }
    
    if (_aResumePostInfo.fgBassEnable)
    {
        _AUD_DspBassBoostEnable(AUD_DEC_MAIN, TRUE);
    }
    
    if (_aResumePostInfo.fgTrebleEnable)
    {
        _AUD_DspClearBoostEnable(AUD_DEC_MAIN, TRUE);
    }
    
    _AUD_DspAVCEnable(AUD_DEC_MAIN, _aResumePostInfo.fgAvcEnable);
#ifdef CC_AUD_USE_FLASH_AQ 
    _au1SoundModeCurrentIdx = 0xff; //Reset current sound mode index
    _au1PositionCurrentIdx = 0xff;  //Reset current TV position index
#endif  
    
    LOG(8, "[Audio Resume]:\n");
    LOG(8, "EQ:       %s\n", (_aResumePostInfo.fgEQEnable == TRUE) ? "TRUE" : "FALSE");
    LOG(8, "VirSur1:  %s\n", (_aResumePostInfo.fgVirSur1Enable == TRUE) ? "TRUE" : "FALSE");
    LOG(8, "VirSur2:  %s\n", (_aResumePostInfo.fgVirSur2Enable == TRUE) ? "TRUE" : "FALSE");
    LOG(8, "ADMixHp:  %s\n", (_aResumePostInfo.fgADHpEnable == TRUE) ? "TRUE" : "FALSE");
    LOG(8, "ADMixSpk: %s\n", (_aResumePostInfo.fgADSpkEnable == TRUE) ? "TRUE" : "FALSE");
    LOG(8, "ADSpkHP:  %s\n", (_aResumePostInfo.fgSpkHpEnable == TRUE) ? "TRUE" : "FALSE");
    LOG(8, "Bass:     %s\n", (_aResumePostInfo.fgBassEnable == TRUE) ? "TRUE" : "FALSE");
    LOG(8, "Treble:   %s\n", (_aResumePostInfo.fgTrebleEnable == TRUE) ? "TRUE" : "FALSE");
    LOG(8, "AVC:   %s\n", (_aResumePostInfo.fgAvcEnable == TRUE) ? "TRUE" : "FALSE");
    LOG(8, "OutPortMode: %d\n",_aeOutPortMode);
    
    switch (_aeOutPortMode)
    {
    case AUD_DEC_OUTPORT_MODE_UNKNOW:
        break;
    case AUD_DEC_OUTPORT_MODE_HEADPHONE:
        ADAC_DacSetChl(INTERNAL_DAC_ID_AUX, AUD_CHL_DOWNMIX);
        break;
    case AUD_DEC_OUTPORT_MODE_LINEOUT:
        ADAC_DacSetChl(INTERNAL_DAC_ID_AUX, AUD_CHL_LS_RS);
        break;
    case AUD_DEC_OUTPORT_MODE_SUB_WOOFER:
#ifdef  CC_SONY_2K14_FLASH_AQ
        ADAC_DacSetChl(INTERNAL_DAC_ID_AUX, AUD_CHL_C_SW);
        AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWM_PAD_SUBWOFFER); //0x76763210 //for Subwoofer mono output..
#endif
        break;
    default:
        break;
    }
    //Downmix Type
    for (i = AUD_DEC_MAIN;i <= AUD_DEC_THIRD; i++)
    {
        AUD_DspSpeakerOutputConfig(i, _aResumePostInfo.u1DownmixType[i]); 
    }
    //DRC mode
    for (i = AUD_DEC_MAIN;i <= AUD_DEC_THIRD; i++)
    {
        AUD_DspAC3CompMode(_aResumePostInfo.u1DRCMode[i], i);
#ifdef CC_AUD_SUPPORT_MS10
        AUD_DspDdtDmxRfMode(_aResumePostInfo.u1DRCMode[i] - 2, i, FALSE);
#endif
    }
    //Matrix mode
    _AUD_DspSetMatrixOutMode(AUD_DEC_MAIN, (AUD_MATRIX_OUT_T)_aResumePostInfo.u1DualmonoMode);
    
    //Hdev mode
    _AUD_DspSetHdevMode(AUD_ATV_DECODER_PAL, u1HdevMode);
    _AUD_DspSetHdevMode(AUD_ATV_DECODER_MTS, u1HdevMode);

    //ATV non-EU mode
    _AUD_DspSetNonEUMode(_aResumePostInfo.fgNonEUmode);

    //Volume resume
    _AUD_DspVolumeChange(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_ALL);
    _AUD_DspVolumeChange(AUD_DSP0, AUD_DEC_AUX, AUD_CH_ALL);
    _AUD_DspVolumeChange(AUD_DSP0, AUD_DEC_THIRD, AUD_CH_ALL);
}

void _AUD_PostProc_Suspend(void)
{
    UINT8  u2ADMixFlag = 0;
    UINT8  i = 0;
    //EQ   
    _aResumePostInfo.fgEQEnable = AUD_DspGetChannelEqEnable(AUD_DEC_MAIN);
    //Virtual Surround
    _aResumePostInfo.fgVirSur1Enable = _AUD_DspGetVirtualSurroundEnalbe(AUD_DEC_MAIN);
    _aResumePostInfo.fgVirSur2Enable = _AUD_DspGetVirtualSurroundEnalbe(AUD_DEC_AUX);
    //AD
    u2ADMixFlag = uReadShmUINT8(AUD_DSP0, B_ADFLAG);
    switch (u2ADMixFlag & 0x0A)
    {   
        case 0x2:
            _aResumePostInfo.fgADHpEnable = TRUE;
            _aResumePostInfo.fgADSpkEnable = FALSE;
            _aResumePostInfo.fgSpkHpEnable = FALSE;
            break;
        case 0x8:
            _aResumePostInfo.fgADHpEnable = FALSE;
            _aResumePostInfo.fgADSpkEnable = TRUE;
            _aResumePostInfo.fgSpkHpEnable = FALSE;
            break;
        case 0xA:
            _aResumePostInfo.fgADHpEnable = FALSE;
            _aResumePostInfo.fgADSpkEnable = FALSE;
            _aResumePostInfo.fgSpkHpEnable = TRUE;
            break;
        default:
            _aResumePostInfo.fgADHpEnable = FALSE;
            _aResumePostInfo.fgADSpkEnable = FALSE;
            _aResumePostInfo.fgSpkHpEnable = FALSE;
            break;
    }
    
    //Bass
    _aResumePostInfo.fgBassEnable = _AUD_DspGetBassBoostEnable(AUD_DEC_MAIN);
    //Treble
    _aResumePostInfo.fgTrebleEnable = _AUD_DspGetClearBoostEnable(AUD_DEC_MAIN);
    //AVC
    _aResumePostInfo.fgAvcEnable = ((u2ReadShmUINT16(AUD_DSP0, W_AVC_FLAG)&0x3) ? TRUE : FALSE);
    _afgAgcEnable[AUD_DEC_MAIN] = !_aResumePostInfo.fgAvcEnable;
    _afgUserAgcFlag[AUD_DEC_MAIN]= !_aResumePostInfo.fgAvcEnable;
    for (i = AUD_DEC_MAIN;i <= AUD_DEC_THIRD; i++)
    {
        _aResumePostInfo.u1DownmixType[i] = (AUD_DspGetSpeakerOutputConfig(i) & 0x07);
        if(AUD_GetDspAutoDmx(i))
        {
            _aResumePostInfo.u1DownmixType[i]  = _aResumePostInfo.u1DownmixType[i]  | 0x40;
        }  
    }
    //DRC mode
    for (i = AUD_DEC_MAIN;i <= AUD_DEC_THIRD; i++)
    {
        _aResumePostInfo.u1DRCMode[i] = AUD_DspGetAC3CompMode(i);
    }
    //Matrix mode
    _aResumePostInfo.u1DualmonoMode = _AUD_DspGetMatrixOutMode(AUD_DEC_MAIN);

    //ATV non-EU mode
    _aResumePostInfo.fgNonEUmode = _AUD_DspGetNonEUMode();

}
#endif

#ifdef CC_VOCAL_MUTE
void _AUD_VocalMuteOnOff(UINT8 bMode,UINT8 fgOnOff)
{
    if (bMode==SV_ON)  // for mp3 & other format
    {
        if(fgOnOff==SV_ON)
        {
            vWriteShmUINT8(B_KARAFLAG,(uReadShmUINT8(B_KARAFLAG)|(0x02))); //enable vocal mute
            DSP_SendDspTaskCmd(UOP_DSP_KARAOKE_FLAG_NO_INT);
        }
        else if(fgOnOff==SV_OFF)
        {
            vWriteShmUINT8(B_KARAFLAG,(uReadShmUINT8(B_KARAFLAG)&(0xFD)));//disable vocal mute
            DSP_SendDspTaskCmd(UOP_DSP_KARAOKE_FLAG_NO_INT);
        }
    }
}
#endif //CC_SUPPORT_STR



