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
 * $Date: 2015/03/25 $
 * $RCSfile: aud_drv.c,v $
 * $Revision: #23 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_drv.c
 *  Brief of file aud_drv.c.
 *  Details of file aud_drv.c (optional).
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
#include "aud_drvif.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
//#include "x_pinmux.h"
#include "x_stl_lib.h"

#include "codec_mt5398.h"
#include "aud_debug.h"
#include "psr_drvif.h"
#include "drv_common.h"
//#include "drv_dsp_cfg.h"
#include "drv_adsp.h"
//#include "drv_stc.h"
#include "stc_drvif.h"
//#include "drv_slt.h"
#include "mpv_drvif.h"
#include "drvcust_if.h"
#include "drv_dbase.h"
#include "mute_if.h"

#include "aud_drv.h"
#include "aud_if.h"
#include "spdif_drvif.h"
#include "aud_hw.h"
#include "aud_cfg.h"
#include "dsp_intf.h"
#include "aud_detect.h"
#include "aud_notify.h"
#include "adac_if.h"
#include "aud_pll.h"
#include "aud_dsp_cfg.h"

#include "../adsp/include_API/dsp_uop.h"
#include "../adsp/include_API/dsp_common.h"
#ifdef ENABLE_MULTIMEDIA
#include "swdmx_if.h"
#include "swdmx_drvif.h"
#endif
#include "vdec_drvif.h"
#include "b2r_drvif.h"

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
#include "srm_drvif.h"
#include "vdo_misc.h"
#include "sv_const.h"
#include "mute_if.h"
#include "vdo_if.h"     //CC_AUD_QUERY_VDO_INFO
#include "video_timing.h"
#include "x_timer.h"
#include "vdec_if.h"
#endif

#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
#include "pd_atd_if.h"
#endif

#include "x_aud_dec.h"
#include "dsp_shm.h" //FLASHLITE_CONFLICT

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
#include "oss_adap.h"
#endif

#ifdef LINUX_TURNKEY_SOLUTION
#include "dsp_common.h"
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/param.h>
#include <linux/time.h>

#ifdef CC_SUPPORT_STR
#include <linux/freezer.h>
#endif

#endif // __KERNEL__

#ifdef CC_SUPPORT_MUXER
#include "muxer_if.h"
#endif

#if defined (CC_AUD_MP3ENC_FAKEDATA_SUPPORT) && defined (CONFIG_HIGH_RES_TIMERS)
#include "linux/hrtimer.h"
#endif
LINT_EXT_HEADER_END

#if defined (CC_AUD_MP3ENC_FAKEDATA_SUPPORT) && defined (CONFIG_HIGH_RES_TIMERS)
static struct hrtimer timer;
static ktime_t base_time;
static ktime_t period_time;
static BOOL fgIsTimerCreated = FALSE;
static UINT32 gu4UploadTimeExceed = 0;
#endif

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#define USE_16BIT_ALIGNMENT

#define CC_AUD_SUPPORT_MM_AV_INIT_SYNC

#define CC_AUD_BLUETOOTH_DSP_IRQ_QUEUE
#define CC_AUD_UPLOAD_DSP_IRQ_QUEUE
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define ADRV_CMD_Q_NAME1        "AUD CMDQ1"
#define ADRV_CMD_Q_NAME2        "AUD CMDQ2"
#define ADRV_CMD_Q_NAME5        "AUD CMDQ5"
#define ADRV_CMD_Q_NAME6        "AUD CMDQ6"
#ifdef CC_MT5391_AUD_3_DECODER
#define ADRV_CMD_Q_NAME3        "AUD CMDQ3"
#define ADRV_CMD_Q_NAME7        "AUD CMDQ7"
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
#define ADRV_CMD_Q_NAME4        "AUD CMDQ4"
#define ADRV_CMD_Q_NAME8        "AUD CMDQ8"
#endif

#define ADRV_CMD_Q_DUAL_DEC     "AUD_DUAL_DEC_CMD_Q"
#define ADRV_APLL_CMD_Q1         "AUD_APLL_CMD_Q1"
#define ADRV_APLL_CMD_Q2         "AUD_APLL_CMD_Q2" 
#ifdef CC_AUD_BLUETOOTH_DSP_IRQ_QUEUE
#define ADRV_BLUETOOTH_DSP_IRQ_Q         "ADRV_BLUETOOTH_DSP_IRQ_Q"
#endif
#ifdef CC_AUD_UPLOAD_DSP_IRQ_QUEUE
#define ADRV_UPLOAD_DSP_IRQ_Q         "ADRV_UPLOAD_DSP_IRQ_Q"
#endif
#define AUD_DRV_DTV_MAIN_THREAD_NAME "AUD_DTV_THREAD_MAIN"
#define AUD_DRV_DTV_AUX_THREAD_NAME "AUD_DTV_THREAD_AUX"
#ifdef CC_MT5391_AUD_3_DECODER
#define AUD_DRV_DTV_THIRD_THREAD_NAME "AUD_DTV_THREAD_THIRD"
#endif

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
#define ADRV_PLAY_MUTE_CMD_Q1    "AUD_PLAY_MUTE_Q1"
#define ADRV_PLAY_MUTE_CMD_Q2    "AUD_PLAY_MUTE_Q2"
#ifdef CC_MT5391_AUD_3_DECODER
#define ADRV_PLAY_MUTE_CMD_Q3    "AUD_PLAY_MUTE_Q3"
#endif
#endif

#ifdef CC_AUD_DDI
#define ADRV_SOUND_BAR_CMD_Q    "AUD_SOUND_BAR_CMD_Q"
#endif

#define AUD_CMD_QUEUE_SIZE      128

#define PES_CNT_THRESHOLD       50
// Light: DTV takes longer time to wait for I frame.
#define PES_CNT_THRES_I_FRAME   200
#define PES_CNT_THRES_I_FRAME_1st   (2000)
#define PES_VALID_CNT           3
#define PTS_THRESHOLD           2800
#define PTS_AC3_CNT             2880
#define PTS_THRESHOLD_MAX       ((UINT32)(PTS_AC3_CNT*120))
#define PTS_THRESHOLD_NOT_VALID       ((UINT32)(PTS_AC3_CNT*2000))
#define PTS_DRIFT_MAX           ((UINT32)12000)

//#define PTS_DRIFT_MAX           12000

#define TRANSFER_SZ             (UINT32)(0x00000400)

#define AUD_EVENT_TIMEOUT       (UINT32)(1500*0.75)
// Light: Since DTV needs to wait for 1st I frame, it takes longer time to wait for the event.
#define AUD_EVENT_TIMEOUT_DTV    (UINT32)(4500)//(UINT32)(1600)
#define AUD_EVENT_TIMEOUT_DTV_1st    (UINT32)(10000)
static BOOL gfgFirstTimeout = TRUE;   /// when AC on in DTV source, Audio driver may miss
static BOOL gfgFirstTimeout1 = TRUE;  /// Start pts setting from Video, and lead to Free run mode.
static UINT8 _gu1AudUploadMode = 0; //0-off, 1-PCM, 2-MP3, 3-SBC
#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
static UINT8 _gu1AudBluetoothMode = 0; //0-off, 1-PCM, 2-MP3, 3-SBC
#endif

#define AUD_EVENT_TIMEOUT_DTV_AD    (UINT32)(500)   //AD check if data available timeout
// Laihui: MM wait longer than DTV, for some file have error data in head
#define AUD_EVENT_TIMEOUT_MM    (UINT32)(8500)

#define AUD_HDMI_DETECT_PERIOD  (20)  // For HDMI pop noise
#define AUD_PTR_UPDATE_TIMEOUT  (20)
#define APLL_MON_THREAD_PERIOD  (300)
#define APLL_ADJUST_PERIOD      (200)

#define DTV_FIFO_FULL_THRESHOLD      2000
#define DTV_AUD_FIFO_FULL_THRESHOLD  188

// Dual decoder time out ...
// for dual decoder thread synchronize main/aux decoder thread
#define DUAL_DEC_SYNC_TIME_OUT 1000

// Feeder relative setting
#define FEEDER_REQUEST_DATA_SIZE  (16*1024)

// Waiting for analog tuner lock signal ..
//2007/12/27 3000->1000, because it takes too long and would block dec0 starting play when there is no ATV source
//           under source HDMI and AV.
#define ATUNER_WAITING_TIME_OUT 500//from 1000 -> 500, to prevent change channel to decoded ATV channel will take a long time to wait semiphone un-lock

#define AIN_CHANGE_TYPE_THRESHOLD (500/AUD_HDMI_DETECT_PERIOD)
#define FEED_STREAM_WAITING_TIME_OUT    100
//#define HDMI_WAIT_VIDEO_THRESHOLD (100/AUD_HDMI_DETECT_PERIOD)

#define HDMI_AUD_OK               0
#define HDMI_AUD_UNSTABLE_LVL_1   1
#define HDMI_AUD_UNSTABLE_LVL_2   2
#define HDMI_AUD_UNSTABLE_LVL_3   3

// DTV lock/unlock checking threshold, ie. 2000 ms
#define DTV_LOCK_CHECK_THRESHOLD (2000/AUD_PTR_UPDATE_TIMEOUT)
#define MM_AAC_CHECK_PROFILE_THRESHOLD DTV_LOCK_CHECK_THRESHOLD + (1000/AUD_PTR_UPDATE_TIMEOUT)
#define DTV_DECODE_ERR_CHECK_THRESHOLD (5000/AUD_PTR_UPDATE_TIMEOUT)   //5s, same as video
#define NET_MM_AAC_CHECK_PROFILE_THRESHOLD  (10000/AUD_PTR_UPDATE_TIMEOUT)  // 10s for worse network
#define NET_DTV_DECODE_ERR_CHECK_THRESHOLD  (10000/AUD_PTR_UPDATE_TIMEOUT)  // 10s, network tv
#define MM_DTS_CHECK_ERROR_THRESHOLD   MM_AAC_CHECK_PROFILE_THRESHOLD
#define MM_AC3_CHECK_ERROR_THRESHOLD   5000
#define NET_MM_AC3_CHECK_ERROR_THRESHOLD 20000

#define MM_SYNC_INFO_NUM 2048          // 1K ==> 2k
#define MM_TICK_INFO_NUM 100

#define MPEG_FRAME_SIZE 0x870 //1152 sample for 48 kHZ, in 90K Hz Clock.

//  -- DSP Force --
//#define DSP_FORCE_RESET_ENABLE
#define DSP_MONITOR_THREAD_PERIOD       100
#define DSP_RESUCE_THRESHOLD            (3 * 1000)
#define DSP_RESUCE_THRESHOLD_1          (5 * 1000)
#define DSP_MONITOR_THREAD_START_TIME   (10 * 1000)
#define OK_CNT_INC_TH 5 //15
#define ERR_CNT_INC_TH 1 //1
#define ERR_ENABLE_TH 2 //3
#define ERR_DISABLE_TH 100 //100
//
#define AUDIO_BOUND 3000              //DTV_AVSYNC_ENH
#define VIDEO_BOUND_60_P 1500
#define AUDIO_BOUND_AAC 3800
#define VIDEO_BOUND_50_I 3600
#define VIDEO_BOUND_60_I 3000

#define APLL_ADJUST_WFDL 15 //500ms
#define APLL_ADJUST_WFDH 4  //900ms
#define APLL_ADJUST_WFDWS 4 //10s

#define AV_ADJUST_BOUND 3
#define AUD_ADJUST_QUOTA  3           //=============


// PTS Queue Related    //PTSQueue_V2
#define PES_INFO_Q_LEN 1024
#define PES_INFO_Q_LEN_1 (PES_INFO_Q_LEN-1)
#define PES_INFO_Q_ITEMS 16
#define PEQ_Q  ((AUD_PES_INFO_T*) VIRTUAL(_arPesQueue[u1DecID]) )
#define PES_INFO_DLY_NUM 20 //-----------


//Audio Codec Change
#define APUT_FREEZE_THRESHOLD       (5) //5*30 =0.15s

#define ARRAY_LENGTH(array)                 (sizeof(array)/sizeof((array)[0]))
#define GET_PCHAR_ARRAY_AT(array, index)    ((index >= ARRAY_LENGTH(array)) ? "" : (array)[index])

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
extern BOOL _fgPwmClkEnable;
extern UINT8 _guAudDemodTypeChange;
extern UINT8 _guAudDspTrigAtvFmtchg;
extern AUD_FMT_T _eAtvFmtChgDecFmt;

#ifdef ENABLE_MULTIMEDIA
extern UINT32 u4AudDspPts[];
#endif
extern BOOL _fgAudMMnotifySWDMX[AUD_DEC_MAX];
extern UINT32 u4ParserFullFlag ;

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void _AUD_DspSetIEC(UINT8 u1DecId);
#if 0   // Unused
extern AUD_IEC_T _GetIECFlag(UINT8 u1DecId);
#endif
#ifndef CC_AUD_SUPPORT_MS10
extern void _AUD_DspSetDDCOEnable(BOOL fgEnable);
#endif
#ifdef FIX_AUD_UOP_TIMEOUT
extern void AUD_WaitUOPLockSema(UINT8 u1DspId, UINT8 u1DecId);
#endif
extern BOOL AUD_IsUopCommandDone(UINT8 u1DspId, UINT8 u1DecId);
extern INT32 AUD_WaitUopCommandDone(UINT8 u1DspId, UINT8 u1DecId);
extern void vDspSetFifoReadPtr(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4ReadPtr);
extern void vDspSetSyncMode(UINT8 u1DecId, AV_SYNC_MODE_T eMode);
extern void vAudOutReInit(UINT8 ucDecId, BOOL fgEnable);
extern BOOL fgDspReadPtrSet (UINT8 u1DspId, UCHAR ucDecId, UINT32 u4Address);
//extern BOOL fgApiVideoStable(UINT8 bVideoPath);
//Replace by _u4DrvVideoGetMute
//extern BOOL fgApiVideoStable(UINT8 u1VideoFrame); // 0: main frame, 1: sub frame for PIP/POP

extern void AUD_SetSrc(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);
extern void AUD_DspDvdEnable(void);
extern BOOL _DMX_GetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32* pu4Bytes);
extern BOOL _DMX_SetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32 u4Bytes);
extern UINT8 u1HDMIGetAudioSamplingFreq(void);
extern void bHDMIClockDetect(void);
extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
#if 0   // Unused
void AUD_ClearFifo(UCHAR ucDecId);
void AUD_GetAudioPes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes);
BOOL AUD_UpdateAudioFifo(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4SartAddress, UINT32 u4Size, UINT32 u4WritePtr);
void AUD_SetStcThreshold(UINT8 u1DecId, UINT16 u2Threshold);
#endif
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
BOOL AUD_DrvCheckHDMISize(AUD_FIFO_INFO_T arfifo,UINT32 u4CheckSize);
BOOL AUD_DrvCheckIECSyncWord(AUD_HDMI_PARSER_INFO_T * prParserInfo);
UINT16 AUD_DrvSwapWord(UINT16 u2swap);
UINT32 AUD_DrvGetCircularAddress(UINT32 u4StartAddress,UINT32 u4EndAddress,UINT32 u4Address);
void AUD_DrvGetParserFormat(UINT8 u1DecId, AUD_FMT_T *peDecFmt);
void AUD_DrvGetParserDetect(UINT8 u1DecId, BOOL *pfgDeteced);
AUD_FMT_T AUD_DrvPcToFormat(UINT8 u1DecId, UINT16 u2Pc);
#endif

extern UINT8 bHDMIAudStatus(void);
extern void ADAC_HdmiUnstableSpkEnable(BOOL fgFlag);
#if 0   // Unused
extern UINT32 AUD_GetDecFrameCnt(UINT8 u1DecId);
#endif
extern BOOL AUD_IsPcmDecMute(UINT8 u1DecId);

extern INT32 IS_MultiMediaSupport(void);
extern void AUD_ClearPtsInISR(UINT8 u1DspId, UINT8 u1DecId);

#if 0// def ENABLE_MULTIMEDIA
extern BOOL _SWDMX_AudioNotify(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);
#endif
extern UINT32 u4ReadD2RInfo_AputBank(UINT8 u1DspId, UINT8 u1DecId);
#ifdef DSP_FORCE_RESET_ENABLE
extern UINT32 u4ReadD2RInfo_InputUnderRun(UINT8 u1DspId, UINT8 u1DecId);  //-- DSP Force --
extern UINT32 u4ReadD2RInfo_ReadPtr(UINT8 u1DspId, UINT8 u1DecId); //-- DSP Force --
extern void vDSPClearDecType(void);                  //-- DSP Force --
extern INT32 u4ADSP_DspReset(void);                  //-- DSP Force --
#endif
extern void AUD_DSPCmdSema_Lock(void);            //-- DSP Force --
extern void AUD_DSPCmdSema_UnLock(void); //-- DSP Force --
extern BOOL AUD_DrvDspCallBackProcess(UINT8 u1DecId, AUD_DEC_CMD_T eCmd);
extern void AUD_ClkSetToInternal(UINT8 u1DecId, AUD_FMT_T eFmt);
extern UINT32 u4ReadD2RInfo_StcDiff(UINT8 u1DspId, UINT8 u1DecId);
#ifdef CC_ENABLE_AOMX
extern void vAudOmxGetGstDbgMsk(UINT32 u4Index,UINT32* pu4DbgMsk);
#endif
extern UINT32 u4AudGetDecErrorCnt(UINT8 u1DspId, UINT8 u1DecId);
extern BOOL DSP_GetAoutEnable(UINT8 u1DspId, UINT8 u1DecId);

extern BOOL _AudCheckAputBankFreeze(UINT8 u1DecID);
extern void _AudNotifyCodecChg(UINT8 u1DecID);
extern BOOL PSR_Reset(UINT8 u1PsrId);

#ifdef CC_AUD_ARM_SUPPORT
extern void AUD_Aproc_Avsync_Ctrl(UINT8 u1DecID);
extern void AUD_Aproc_Calc_StcDiff(UINT8 u1DecID, UINT32 u4PTS);
extern UINT32 u4ReadD2RInfo_AACVersion(UINT8 u1DecId);
extern void DSP_SetSkipFrame(UINT8 u1DecId, UINT32 u4FrameNum);
extern void AUD_Aproc_Chg_AVSyncCtrl(UINT8 u1DecID, APROC_AVSYNC_CTRL_MODE_T eMode);
extern void _vAprocSetRoutine (UINT32 u4Id);
#endif //CC_AUD_ARM_SUPPORT 


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static void _DrvThreadInit(void);
static void _ChangeAudioState(UINT8 u1DspId, UINT8 u1DecId, AUD_DRV_STATE_T eNewState);
//static void _FlushAudioFifo(UINT8 u1DecId);
static void _AudDrvThread(void* pvArg);
static void _AudFeedStreamThread(const void* pvArg);
static void _AudSetEvent(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4Event);
static BOOL _AudWaitEvent(UINT8 u1DspId, UINT8 u1DecId, UINT32 * pu4Event, BOOL fgWait, UINT32 u2TimeOut);
static BOOL _IsPtsValid(UINT8 u1PidIdx, UINT8 ucDevId, const DMX_AUDIO_PES_T * prPes);
static BOOL _IsPtsAlign(UINT8 u1DecId, UINT32 u4Ptr);
static void _LoadAdspCode(UINT8 u1DspId, UINT8 u1DecId, AUD_FMT_T eStreamFormat);
//static void _UpdatePtsToShm(UINT8 u1DecId, UINT32 u4Pts, UINT32 u4Addr);
static void _AudApllMonitorThread(void* pvArg);
#ifdef CC_AUD_DDI
static void _AudSoundBarThread(void* pvArg);
#endif
#ifdef CC_AUD_BITRATE_CHG_NOTIFY
static void _AudResetBitRateChangedPts(void);
#endif
static void _AudDtvOnPlayThread(void* pvArg);
static void _DtvResetLockStatus(UINT8 u1DecId);
inline void _AudDrvThread_WaitResume(UINT8 u1DspId, UINT8 u1DecId);
#ifdef DSP_FORCE_RESET_ENABLE
static void _AudDspMonitorThread(void* pvArg);  //  -- DSP Force --
#endif
static void _AudDrvResetDSP(UINT8 u1DecId);     //  -- DSP Force --
#ifdef CC_AUD_NCSTOOL_SUPPORT
extern void _AudToolThread(void* pvArg);
#endif
#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
static void _AudPlayMuteThread(void* pvArg);
#endif
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
static TV_AUD_SYS_T _AudGetTvSysFormat(MW_TV_AUD_SYS_T rAudTvSysFromMW);
static UINT8 _AudTvSys2Tuner(UINT32 u4TvSysMask);
#endif
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
static void _AudDataUploadThread(void* pvArg);
static void _AudMuxerInit(void);
//#endif
#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
static void _AudDataBluetoothThread(void* pvArg);
static void _AudBluetoothMuxerInit(void);
#endif

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
static void _AudHDMIParserThread(void* pvArg);
#endif

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
static void _AudDataTransferThread(void* pvArg);
#endif

#ifdef CC_AUD_SKYPE_ZERO_DELAY
bool _fgSkypeMode = FALSE;
extern BOOL FeederGetSkypeSourceFlag(void);
#endif

#ifdef CC_AUD_HP_DEPOP  ///For Remy, need remove "CC_DAC_DE_POP".
static void _AudHpDePopThread(void* pvArg);
#endif
UINT32 _AUD_ReadPsrStc1 (void);

static void _AudCodecChangeThread(void* pvArg);
//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
UINT32 _gu1AoutDramErr;
#ifdef CC_SUPPORT_MUXER
static UINT8  _ui1MuxId=0;
#endif
BOOL  _fgMMPlusAD[AUD_DEC_MAX] = {FALSE,FALSE,FALSE,FALSE};  //Multimedia File with AD support. we share DTV input source.

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
#ifdef CC_AUD_SMALL_PTS_CHECK
static UINT32 u4OldPts1 = 0;
#endif
static UINT32 isEsExist = TRUE;
static BOOL isEsExist1 = FALSE;
static UINT32 u4PrevValidPes = 0;
static UINT32  u4NoPESCount=0;
static UINT32  u4NoPESCount1=0;
static UINT32  _u4DecframeCount=0;
static UINT32  _u4ThreadTime=0;

static UINT32 _u4FeedStreamResult;
static INT32  _i4MemStrLoopCnt;
static BOOL   _fgInfinite = FALSE;
static BOOL   _fgForceStopStrmDma = FALSE;
static BOOL _fgMMAudioOnly[AUD_DEC_MAX] = {FALSE, FALSE,FALSE, FALSE};
static BOOL _fgAVAudioOnly = FALSE;
static BOOL _fgAdMode = FALSE;

#ifdef CC_AUD_HP_DEPOP  ///For Remy, need remove "CC_DAC_DE_POP".
AUD_HP_DEPOP_CMD_T _eAudHpDePopTaskStatus = AUD_HP_DEPOP_DC_ON;
static UINT32  _u4HpDePopDelayCnt = 0;
static BOOL    _fgHpStateLock = FALSE;
static HANDLE_T _hSemaHpDepopWait;
#endif

// Thread hanlder
static HANDLE_T _hAudDrvThread[AUD_DSP_NUM][AUD_DEC_MAX] = {{NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE},{NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE}};
static HANDLE_T _hAudApllMonThread[2] = {NULL_HANDLE, NULL_HANDLE};
#ifdef CC_AUD_DDI
static HANDLE_T _hAudSoundBarThread;
#endif
static HANDLE_T _hAudFeedStrThread = NULL_HANDLE;
static HANDLE_T _hAudFeederThread = NULL_HANDLE;
#ifdef DSP_FORCE_RESET_ENABLE
static HANDLE_T _hAudDspMonThread; //  -- DSP Force --
#endif
static HANDLE_T _hAudDtvThread[AUD_DEC_MAX] = {NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE};
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
static HANDLE_T _hAudHDMIParserThread[AUD_DEC_MAX] = {NULL_HANDLE};
#endif
#ifdef CC_AUD_NCSTOOL_SUPPORT
static HANDLE_T _hAudToolThread = NULL_HANDLE;
extern void vAllocateSWReg(void);
#endif

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
static HANDLE_T _hAudPlayMuteThread[AUD_DEC_MAX] = {NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE};
#endif
static HANDLE_T _hAudCodecChangThread = NULL_HANDLE;

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
static HANDLE_T _hAudDataUploadThread = NULL_HANDLE;
#ifdef CC_AUD_UPLOAD_DSP_IRQ_QUEUE
static HANDLE_T _hAudUploadDspIRQCmdQueue;
typedef struct AUD_UPLOAD_MESSAGE
{
    UINT32 u4Index;
    UINT32 u4STC;
}_AUD_UPLOAD_MESSAGE;
#else
static HANDLE_T _hSemaDataUploadWait;
#endif

#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
static HANDLE_T _hAudDataBluetoothThread = NULL_HANDLE;
#ifdef CC_AUD_BLUETOOTH_DSP_IRQ_QUEUE
static HANDLE_T _hAudBluetoothDspIRQCmdQueue;
#else
static HANDLE_T _hSemaDataBluetoothWait;
#endif
#endif
//#endif
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
static HANDLE_T _hAudDataTransferThread = NULL_HANDLE;
static HANDLE_T _hSemaDataTransferWait;
static HANDLE_T _hSemaDataTransferWait1;

#endif

#ifdef CC_AUD_HP_DEPOP  ///For Remy, need remove "CC_DAC_DE_POP".
static HANDLE_T _hAudHpDePopThread = NULL_HANDLE;
#endif


// Semaphore and message queue
static HANDLE_T _hSemaLoadCode;
static HANDLE_T _ahSemaUopComDone[AUD_DSP_NUM][AUD_DEC_MAX];
static HANDLE_T _hDetectionSyncSema;
static HANDLE_T _hFeedStreamSyncSema;
static HANDLE_T _ahAudCmdQueue[AUD_DSP_NUM][AUD_DEC_MAX];
static HANDLE_T _hAudApllCmdQueue[2];
#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
static HANDLE_T _hAudPlayMuteCmdQueue[AUD_DEC_MAX];
#endif
#ifdef CC_AUD_DDI
static HANDLE_T _hAudSoundBarCmdQueue;
#endif

static DMX_AUDIO_PES_T _arAudPes[AUD_DEC_MAX];

static HANDLE_T _hAtunerSyncSema;

static HANDLE_T _arAudDrvStateSema[AUD_DSP_NUM][AUD_DEC_MAX];

static BOOL      _afgDtvOnPlayThreadGo[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};

static AUD_DECODER_T _arAudDecoder[AUD_DSP_NUM][AUD_DEC_MAX];
static AUD_DECODER_T _arAudDecoderHDMI[AUD_DEC_MAX];

#ifdef CC_AUD_MULTI_BUFFER_MEMORY_PLAY
static MEM_INFO_T _arAudMemBuf[MAX_MEM_BUFFER_NUMBER];
static UINT32 _au4AudMemBufCurrIdx = 0;
static UINT32 _au4AudMemBufCurrBid = 0;
static BOOL   _afgAudMemBufStart = FALSE;
static BOOL   _afgAudMultiMemBufMode = FALSE;
#endif

//Audio Codec Change 
static BOOL _fgAudCodecChang = FALSE;
static UINT32 _u4AputBank = 0;
static UINT32 _u4PrvAputBank = 0xFFFFFFFF;
static UINT32 _u4AputKeepCount = 0;
static AUD_FMT_T _eCodecChangeFmt = AUD_FMT_UNKNOWN;

static BOOL _fgAudDrvThreadInit = FALSE;

static BOOL _afgFirstIPicArrived[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};
static BOOL _afgIssuePlayComToDsp[AUD_DSP_NUM][AUD_DEC_MAX] = {{FALSE, FALSE, FALSE, FALSE},{FALSE, FALSE, FALSE, FALSE}};
static BOOL _afgAtvVideoDisplayFlag[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};

//static BOOL _afgUseModifiedPts[2] = {FALSE, FALSE};
static UINT8 _uAudioDelayPTSFlag[AUD_DEC_MAX] = {0, 0, 0, 0};

//static UINT16 _uAudioEventTimeout[2] = {AUD_EVENT_TIMEOUT, AUD_EVENT_TIMEOUT};
// Video PIP/POP displayed frame indicator, 0: display main frame, 1: display sub frame.
static UINT8 _u1VideoFrameFlag = 0;
static BOOL _fgDDBannerOn = FALSE;

// Dual decoder variables
static AUD_DRV_STATE_T _arAudDrvState[AUD_DSP_NUM][AUD_DEC_MAX] = {{AUD_UNINITIALIZED, AUD_UNINITIALIZED, AUD_UNINITIALIZED, AUD_UNINITIALIZED}, {AUD_UNINITIALIZED, AUD_UNINITIALIZED, AUD_UNINITIALIZED, AUD_UNINITIALIZED}};

static PARAM_MPV_T* _prMPV;
static DRV_DBASE_T* prDbase;

// For memory clip
#ifdef CC_AUD_SUPPORT_CLIP_NOTIFY_FUNC
static AUD_CLIP_NOTIFY _hClipNotifyFunc = NULL;
#endif

// Feeder varaibles
static FEEDER_TRANSMIT _rFeedTransmitInfo;
//static BOOL _fgDtvLock = TRUE;
static BOOL _afgDtvLock[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};
static BOOL _afgDtvPesLock[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};

// HDMI relative
static BOOL  _fgHdmiMute = FALSE;
static UINT8 _u1HDMIMuteCnt = 0;

#ifdef CC_AUD_I2S_IN
//I2S relative
static BOOL  _fgI2SinMute = FALSE;
static UINT8 _u1I2SinMuteCnt = 0;
static UINT8 _u1I2SinUnDetectCnt = 0;
#endif

#ifdef CC_MT5881
// Spdif-in relative
static UINT8 _u1SpdifinMuteCnt = 0;
#endif

// DTV lock status
static UINT32 _au4LastPesCnt[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT32 _au4FrameCnt[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT32 _au4ErrCnt[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT32 _au4LastErrCnt[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT32 _au1NoErrCntIncrease[AUD_DEC_MAX] = {0, 0, 0, 0};        
static UINT32 _au1NoOkCntIncrease[AUD_DEC_MAX] = {OK_CNT_INC_TH, OK_CNT_INC_TH, OK_CNT_INC_TH, OK_CNT_INC_TH};        
static UINT32 _au4LastFrameCnt[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT32 _au4PesCnt[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT16  _au2NoFrameCntIncrease[AUD_DEC_MAX] = {0, 0, 0, 0};
static UINT16  _au2NoPesCntIncrease[AUD_DEC_MAX] = {0, 0, 0, 0};
static AUD_DECODE_STATUS_T  _aeDecStatus[AUD_DEC_MAX];
static BOOL   _fgMMSet[AUD_DEC_MAX] = {FALSE,FALSE,FALSE,FALSE};    //MM EOS relative
static AUD_DECODE_STATUS_T  _aeHDMIDecStatus[AUD_DEC_MAX];
static AUD_SOURCE_STATE_T   _aeSourceState[AUD_DEC_MAX];
static BOOL _fgWaitAvSync[AUD_DEC_MAX] = {FALSE,FALSE,FALSE,FALSE};
static BOOL _fgAudOutReady[AUD_DEC_MAX] = {FALSE,FALSE,FALSE,FALSE};
static UINT32 _au1NoErrCntAcc[AUD_DEC_MAX] = {0, 0, 0, 0};       
static UINT32 _au1NoOkCntAcc[AUD_DEC_MAX] = {0, 0, 0, 0};
static BOOL _au1ErrEnable[AUD_DEC_MAX] = {FALSE,FALSE,FALSE,FALSE};
static BOOL _au1MuteEnable = FALSE;   

// Bit rate changed
#ifdef CC_AUD_BITRATE_CHG_NOTIFY
#define BIT_RATE_CHANGED_PTS_Q_SIZE     (16)

static UINT32 _au4BitRateChangedPts[BIT_RATE_CHANGED_PTS_Q_SIZE];
static UINT16 _au2InIdx = 0;
static UINT16 _au2OutIdx = 0;
#endif

// MM : MultiMedia Releated.
static AUD_MM_SYNC_INFO_T *_aeMMSyncInfo = NULL; //[MM_SYNC_INFO_NUM];
//static AUD_MM_SYNC_INFO_T* _paeMMSyncInfo;
static UINT16 u2CurrIdx = 0;
static UINT16 u2LatestIdx = 0;
static BOOL fgLoopBack = FALSE;
static BOOL fgSyncInfoReset = TRUE;
static UINT64 u8PreSyncPtsInfo = 0;
#ifdef TIME_SHIFT_SUPPORT
static AUD_MM_TICK_INFO_T _aeMMTickInfo[MM_TICK_INFO_NUM];
static UINT16 u2TickCurrIdx = 0;
#endif
static UINT32 u4OldPts = 0;
#ifdef CC_ENABLE_AOMX
static UINT64 u8LastGetPts = 0;
static UINT16 u2LastGetIdx = 0xFFFF;
#ifdef CC_AUD_ARM_RENDER
typedef struct
{
    BOOL fgStart;
    HANDLE_T hSema;
    UINT32 u4PtsLow;
    UINT32 u4PtsHigh;
    UINT32 u4PtsLowLast;
    struct timeval nTimeVal;
} GST_TIME_STAMPL_T;
static GST_TIME_STAMPL_T GstTimeStampe[AUD_DEC_MAX] = {{0}};
#endif
#endif
static BOOL _bPanFadeUpdateEn = TRUE;
static BOOL _bPanEnable = TRUE;
static BOOL _bFadeEnable = TRUE;
static BOOL _fgHdmiAoutMute = FALSE;
#ifdef CC_MT5881
static BOOL _fgSpdifinAoutMute = FALSE;
#endif
#ifdef CC_AUD_DDI
static BOOL _fgWatchSpdifEnable = FALSE;
static BOOL _fgPvrPause = FALSE;
#endif

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
#define AUD_PLAY_MUTE_THREAD_DELAY      20
#define VDP_MOD_CHG_DONE_WAITING_TIME_OUT 800
#define VDP_MOD_CHG_WAITING_TIME_OUT    40
#define AUD_MUTE_WITH_VDO_TIMEOUT       4000    // 4000ms
#define AUD_MUTE_WITH_AV_VDO_TIMEOUT    4000    // 4000ms
#define AUD_PIP_MUTE_WITH_VDO_TIMEOUT   100     // 100ms
#define AUD_EXTRA_MUTE_AFTER_VDP        200     // 200ms

#define AUD_PLAY_MUTE_MSG_Q_SIZE        32

struct AUD_PLAY_MUTE_MSG_Q_T
{
    HAL_TIME_T eTime;
    AUD_PLAY_MUTE_CMD_T eAudPmCmd;
    UINT16 u2Arg;
};

static struct AUD_PLAY_MUTE_MSG_Q_T _stAudPmMsgQ[AUD_DEC_MAX][AUD_PLAY_MUTE_MSG_Q_SIZE];
static UINT8 _u1CmdQIdx[AUD_DEC_MAX] = {0,0,0};
static char _cAudPmCmdName[AUD_PM_CMD_NUM+1][30] = {
    // Opeartion
    "AUD_PM_OP_SET_DELAY",
    "AUD_PM_OP_UNMUTE",
    // Command
    "AUD_PM_CMD_RESET",
    "AUD_PM_CMD_VDP_MODE_CHG",
    "AUD_PM_CMD_MUTE",
    "AUD_PM_CMD_VDP_MODE_CHG_DONE",
    "AUD_PM_CMD_DSP_PLAY",
    "AUD_PM_CMD_UNMUTE_WITH_VDP",
    "AUD_PM_CMD_UNMUTE_TIME",
    "AUD_PM_CMD_SET_PIP_VDO_PATH",
    "AUD_PM_CMD_SET_DELAY_READY",
    "AUD_PM_CMD_AOUT_ENABLE",
    ""
};

static UINT16 _u2VideoDelay[AUD_DEC_MAX], _u2AudioDelay[AUD_DEC_MAX];
static BOOL _fgPlayMuteEnable = TRUE;
#endif

#if 0
//#ifdef CC_AUD_HP_DEPOP  ///For Remy, need remove "CC_DAC_DE_POP".
static CHAR * _aszHpDePopCase[] =
{
    "CASE_DC_ON Flowchart",
    "CASE_D Flowchart",
    "CASE_E Flowchart",
    "CASE_G Flowchart",
    "CASE_I Flowchart",
    "CASE_DC_OFF Flowchart",
    "CASE_IDLE Flowchart",
};

#endif

static CHAR * _aszCodeType[] =
{
    "UNKNOWN",      // 0
    "MPEG",
    "AC3",
    "PCM",
    "MP3",
    "AAC",          // 5
    "DTS",
    "WMA",
    "RA",
    "HDCD",
    "MLP",          // 10
    "MTS",
    "CANAL+",
    "PAL",
    "A2",
    "FMFM",         // 15
    "NICAM",
    "TTXAAC",
    "DETECTOR",
    "MINER",
    "LPCM",         // 20
    "FM_RADIO",
    "FM_RADIO_DET",
    "SBCDEC",
    "SBCENC",
    "MP3ENC",       // 25, MP3ENC_SUPPORT
    "G711DEC",
    "G711ENC",
    "DRA",
    "COOK",
    "G729DEC",      // 30
    "VORBIS",        // 31, CC_VORBIS_SUPPORT
    "WMAPRO",
    "WMALSL",
    "WMASPEECH",
    "AMR",
    "AWB",
    "APE",              // 37 //ian APE decoder
    "",                        //38
    "FLAC"                 //39, paul_flac
};

static CHAR * _paszAudState[] =
{
    "UNINITIALIZED",
    "TRIGGER_ADSP",
    "WAIT_POWER_ON",
    "INIT",
    "WAIT_CMD",
    "WAIT_PLAY",
    "WAIT_PAUSE",
    "WAIT_RESUME",
    "WAIT_STOP",
    "WAIT_STOPSTREAM",
    "IDLE",
    "ON_PLAY",
    "ON_PAUSE",
    "STOPPED",
    "DECODE_INIT",
    "CHANGE_FORMAT",
    "DSP_ERROR"
};

static CHAR * _paszAudCmd[] =
{
    "AUD_CMD_PLAY",
    "AUD_CMD_STOP",
    "AUD_CMD_RESET",
    "AUD_CMD_PAUSE",
    "AUD_CMD_AVSYNC",
    "AUD_CMD_LOADCODE",
    "AUD_CMD_RESUME",
    "AUD_CMD_CHANGE_FORMAT",
    "AUD_CMD_STOPSTREAM",
    "AUD_CMD_UNMUTE",
    "AUD_CMD_PLAY_SKIP",
    "AUD_CMD_STOP_SKIP",
    "AUD_CMD_NULL"
};

static DRV_DBASE_T* _prDbase = NULL;
static UINT32 _au4TvSysMask[2] = {0, 0}; //2008/01/16
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
static UINT32 _au4TvAudSysMask[2] = {0, 0}; //2008/01/16
static ISO_3166_COUNT_T _au4TvSysCountry =  "";
#endif
#ifdef DSP_FORCE_RESET_ENABLE
static UINT32 u4DspResueTrigger = 0;     //  -- DSP Force --
#endif

static UINT8 _u1SwdmxSrcId[AUD_DEC_NUM] = {0};

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
static AUD_HDMI_PARSER_INFO_T _arParserInfo[AUD_DEC_NUM];
static UINT16 _au2Pc[AUD_RAW_NUM]={0x1,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xb,0xc,0xd,0x11,0x15,0x16};
#endif

static BOOL   _fgFeedStrmFinshed = TRUE;    // Feed_and_Auddrv Thread sync

#ifdef LINUX_TURNKEY_SOLUTION
BOOL skip_create_feed_stream = FALSE;
#endif
INT16 _u2AudConrolVdpQuota =0;   //DTV_AVSYNC_ENH
INT32 i4AudioBound =0;
INT32 i4VideoBound =0;
INT32 i4AudioMinusBound =0;
INT32 i4VideoMinusBound =0;
INT32 i4HalfBound =0;           //================

// PTS Queue Related    //PTSQueue_V2
UINT32 u4_Q_Uinit[AUD_DEC_NUM];                     // (FIFO_Length / QUEUE_Length)  e.x. 298K/1024  If PES size is smaller than this, we should skip some PES.
UINT32 u4_Prev_Wp[AUD_DEC_NUM] = {0xffffffff,0xffffffff,0xffffffff};  // Previous Write Wp.
UINT16 u2PesQIdx[AUD_DEC_NUM] = {0};                // Current Index of PTS Queue.
UINT16 u2PesSize[AUD_DEC_NUM] = {0};                // Calculate PES size put into the queue.
UINT16 u2MaxIdx[AUD_DEC_NUM] = {PES_INFO_Q_LEN_1,PES_INFO_Q_LEN_1,PES_INFO_Q_LEN_1}; //The max index driver had inserted in current round.
UINT32 u4LoopCnt[AUD_DEC_NUM] = {1,1,1};                // FIFO read loop counter, init is 1, and Wp<init of PrevWp, become 0
UINT16 u2PesQIdxBackup[AUD_DEC_NUM] = {0,0,0};          // Current Index of PTS Queue backup.

BOOL fgFirstRound[AUD_DEC_NUM] = {TRUE,TRUE,TRUE};  // If it's the first round using the queue. After the 2nd round, driver will delay some PEQ for not overwriting not used PES info.
UINT16 u2QDelayNum[AUD_DEC_NUM] = {0};              // Delay some info in driver buffer.
UINT16 u2QBufIn[AUD_DEC_NUM] ={0};
UINT16 u2QBufOut[AUD_DEC_NUM] ={0};
AUD_PES_INFO_T aPTSDrvBuf[AUD_DEC_NUM][PES_INFO_DLY_NUM];
UINT32 _arPesQueue[3];
//-------------------------

#ifdef AUD_DECODER3_SELF_TEST
UINT8 u1ForceDecId = AUD_DEC_NUM;    //parson MM3 default no force id
AV_SYNC_MODE_T eForceSyncMode = AV_SYNC_AUDIO_MASTER;    //parson MM3
#endif

BOOL fgHDMIDTS = FALSE;
#if defined(CC_AUD_ARM_SUPPORT)
INT32 _gi4StcDiff[AUD_DEC_NUM] = {0};
APROC_AVSYNC_CTRL_MODE_T gsAprocSyncCtrl[AUD_DEC_NUM] = {APROC_AVSYNC_CTRL_WORK};
UINT32 _gu4ValidPtsCounter[AUD_DEC_NUM];
BOOL fgSkipUnderHold[AUD_DEC_MAX] = {FALSE,FALSE,FALSE}; 
#endif

#if defined(CC_MT5890) || defined(CC_MT5882)
static void vWritePWMG1(UINT32 addr, UINT32 data)
{
#ifdef CC_SUPPORT_STR
    if (!_fgPwmClkEnable)
    {
        return;
    }
#endif
    AUD_WRITE32(REG_INDIRECT_G1_ADDR, addr);
    AUD_WRITE32(REG_INDIRECT_G1_DATA, data);
}
static void vWritePWMG2(UINT32 addr, UINT32 data)
{
#ifdef CC_SUPPORT_STR
    if (!_fgPwmClkEnable)
    {
        return;
    }
#endif
    AUD_WRITE32(REG_INDIRECT_G2_ADDR, addr);
    AUD_WRITE32(REG_INDIRECT_G2_DATA, data);
}

static void vWritePWMG3(UINT32 addr, UINT32 data)
{
#ifdef CC_SUPPORT_STR
    if (!_fgPwmClkEnable)
    {
        return;
    }
#endif
    AUD_WRITE32(REG_INDIRECT_G3_ADDR, addr);
    AUD_WRITE32(REG_INDIRECT_G3_DATA, data);
}
#endif

//      --  DUMP AFIFO      --  //
#ifdef LINUX_TURNKEY_SOLUTION
extern int StartAudDataDump(UINT32 u1DecId, char* fileName);
extern void StopAudDataDump(UINT32 u1DecId);
extern void AudDataDumpQueryInfo(void);
extern BOOL GetAudDataDumpAutoSave(UINT32 u1DecId);
extern void SetAudDataDumpAutoSave(UINT32 u1DecId, BOOL bAutoSave);
static void AudDataDumpNextFile(UINT32 u1DecId);

#define MAX_PATH 256

static const char *AudDataDumpThreadName[AUD_DEC_NUM] = {"_AudDataDumpThread 0", "_AudDataDumpThread 1",
                                                         "_AudDataDumpThread 2", "_AudDataDumpThread 3"};
static HANDLE_T _hAudDataArrived[AUD_DEC_NUM] = {NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE};
static HANDLE_T _hAudDataDumpThread[AUD_DEC_NUM] = {NULL_HANDLE, NULL_HANDLE, NULL_HANDLE, NULL_HANDLE};
static HANDLE_T _hAudDataDumpThreadState[AUD_DEC_NUM] = {-1, -1, -1, -1};
static BOOL _fgForceStopAudData[AUD_DEC_NUM] = {FALSE, FALSE, FALSE, FALSE};
struct file *_pAudDumpFile[AUD_DEC_NUM] = {NULL, NULL, NULL, NULL};
#if 1 //20120801, wonder, to clean warning msg.
static UINT32 u4AudDataCurRp[AUD_DEC_NUM] = {0, 0, 0, 0};
#else
static UINT32 u4AudDataCurRp[AUD_DEC_NUM] = {NULL, NULL, NULL, NULL};
#endif
static BOOL _fgAudDataDumpAutoSave[AUD_DEC_NUM] = { FALSE, FALSE, FALSE, FALSE };
static BOOL _fgAudDataDumpNextFile[AUD_DEC_NUM] = { FALSE, FALSE, FALSE, FALSE };
static char _szAudDataDumpFileName[AUD_DEC_NUM][MAX_PATH] = { "\0", "\0", "\0", "\0" };

void write_to_file(struct file *filep, const char *addr, UINT32 size)
{
    mm_segment_t oldfs;
    oldfs = get_fs();
    set_fs(KERNEL_DS);

    filep->f_op->write(filep, (const char *)addr, (size_t)size, &filep->f_pos);

    set_fs(oldfs);
}
static void _AudDataDumpThread(void* pvArg)
{
    UINT32 u1DecId = AUD_DEC_MAIN;
    UINT32 u4Rp;
    UINT32 u4Wp;
    UINT32 u4DataSZ;
    UINT32 u4FifoSA;
    UINT32 u4FifoEA;

    UINT32 runCount = 0;
    UINT32 dataSize = 0;

    if (pvArg != NULL)
    {
        u1DecId = *(UINT32 *)pvArg;
    }

    _fgAudDataDumpNextFile[u1DecId] = FALSE;
    _hAudDataDumpThreadState[u1DecId] = 1;
    LOG(0, "_AudDataDumpThread %d is created !\n", u1DecId);

    UNUSED(AUD_GetAudFifo(u1DecId, &u4FifoSA, &u4FifoEA));
    u4FifoSA = VIRTUAL(u4FifoSA);
    u4FifoEA = VIRTUAL(u4FifoEA);
    u4AudDataCurRp[u1DecId] = u4FifoSA;

    while (1)
    {

        if (_fgForceStopAudData[u1DecId])
        {
            break;
        }

        VERIFY(x_sema_lock(_hAudDataArrived[u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);

        if ((_fgAudDataDumpAutoSave[u1DecId]) && (_fgAudDataDumpNextFile[u1DecId]) && AUD_IsDecoderPlay(AUD_DSP0, u1DecId))
        {
             AudDataDumpNextFile(u1DecId);
             runCount = 0;
             dataSize = 0;
             LOG(5, "_AudDataDumpThread %d opens a new file \"%s\" to dump.\n", u1DecId, _szAudDataDumpFileName[u1DecId]);
        }
        _fgAudDataDumpNextFile[u1DecId] = FALSE;

        AUD_GetRWPtr(AUD_DSP0, u1DecId, &u4Rp, &u4Wp, &u4DataSZ);
        u4Wp = VIRTUAL(u4Wp);
        u4Rp = u4AudDataCurRp[u1DecId];

        if (_pAudDumpFile[u1DecId] && AUD_IsDecoderPlay(AUD_DSP0, u1DecId))
        {
            if (u4Wp >= u4FifoEA)
            {
                LOG(0, "_AudDataDumpThread %d, wrong WP(0x%x)!!\n", u1DecId, u4Wp);
                u4Wp = u4FifoSA;
            }

            if(u4Wp < u4Rp)
            {
                DSP_FlushInvalidateDCacheFree(u4Rp,(u4FifoEA - u4Rp));
                write_to_file(_pAudDumpFile[u1DecId], (const char *) u4Rp, u4FifoEA - u4Rp);
                DSP_FlushInvalidateDCacheFree(u4FifoSA,(u4Wp - u4FifoSA));
                write_to_file(_pAudDumpFile[u1DecId], (const char *) u4FifoSA, u4Wp - u4FifoSA);
                dataSize += u4FifoEA - u4Rp;
                dataSize += u4Wp - u4FifoSA;
            }
            else
            {
                DSP_FlushInvalidateDCacheFree(u4Rp,(u4Wp - u4Rp));
                write_to_file(_pAudDumpFile[u1DecId], (const char *) u4Rp, u4Wp - u4Rp);
                dataSize += u4Wp - u4Rp;
            }
            u4AudDataCurRp[u1DecId] = u4Wp;
        }

        runCount ++;
        LOG(5, "_AudDataDumpThread %d is active , runCount = %d , dataSize = 0x%x \n", u1DecId, runCount, dataSize);
    }

    LOG(0, "_AudDataDumpThread %d exited !\n", u1DecId);
    _hAudDataDumpThreadState[u1DecId] = -1;
}

int StartAudDataDump(UINT32 u1DecId, char* fileName)
{
    mm_segment_t oldfs;
    oldfs = get_fs();
    set_fs(KERNEL_DS);

    if (u1DecId >= AUD_DEC_NUM)
    {
        LOG(0, "Invalid decode ID : %d \n", u1DecId);
        goto fail_invalid_arg;
    }

    if (_hAudDataDumpThreadState[u1DecId] != -1)
    {
        LOG(0, "%s already running .\n", AudDataDumpThreadName[u1DecId]);
        goto fail_already_running;
    }

    _pAudDumpFile[u1DecId] = filp_open(fileName, (O_CREAT | O_WRONLY ), 0);
    if((IS_ERR(_pAudDumpFile[u1DecId])) || (NULL == _pAudDumpFile[u1DecId]))
    {
        LOG(0, "_AudCmdDump() create file failed.\n");
        goto fail_create_file;
    }

    if(OSR_OK != x_thread_create(&_hAudDataDumpThread[u1DecId], AudDataDumpThreadName[u1DecId], AUD_DRV_THREAD_STACK_SIZE,
                                    AUD_DRV_THREAD_PRIORITY, _AudDataDumpThread, sizeof(UINT32), (void *)&u1DecId))
    {
        LOG(0, "Failed to start %s .\n", AudDataDumpThreadName[u1DecId]);
        goto fail_start_thread;
    }

    x_strncpy(_szAudDataDumpFileName[u1DecId], fileName, x_strlen(fileName)+1);
    LOG(0, "Dumping audio fifo %d to \"%s\" ...\n", u1DecId, fileName);
    set_fs(oldfs);
    return 0;

    fail_start_thread:
        filp_close(_pAudDumpFile[u1DecId], 0);
    fail_create_file:
        _pAudDumpFile[u1DecId] = NULL;
    fail_already_running:
    fail_invalid_arg:
        set_fs(oldfs);
        return -1;
}

void StopAudDataDump(UINT32 u1DecId)
{
    UINT32 count = 0;
    mm_segment_t oldfs;

    if (u1DecId >= AUD_DEC_NUM)
    {
        LOG(0, "Invalid decode ID : %d \n", u1DecId);
        return;
    }

    _fgForceStopAudData[u1DecId] = TRUE;
    VERIFY(x_sema_unlock(_hAudDataArrived[u1DecId]) == OSR_OK);

    while (_hAudDataDumpThreadState[u1DecId] != -1)
    {
        x_thread_delay(5);
        count ++;
        if (count >= 5) {
            LOG(0, "Stop _AudDataDumpThread %d fail !\n", u1DecId);
        }
    }

    _fgForceStopAudData[u1DecId] = FALSE;
    if (_pAudDumpFile[u1DecId])
    {
        oldfs = get_fs();
        set_fs(KERNEL_DS);
        filp_close(_pAudDumpFile[u1DecId], 0);
        _pAudDumpFile[u1DecId] = NULL;
        set_fs(oldfs);
    }

    LOG(0, "Stop data dump %d OK !\n", u1DecId);
}

void AudDataDumpQueryInfo(void)
{
    UINT8 i;

    for (i = 0; i < AUD_DEC_NUM; i++)
    {
        LOG(0, "#%s: %s\n", AudDataDumpThreadName[i], (_hAudDataDumpThreadState[i] == 1) ? "running" : "exited");
        if ((_hAudDataDumpThreadState[i] == 1) && (_pAudDumpFile != NULL))
        {
            LOG(0, "  Dumping to file: %s\n\n", _szAudDataDumpFileName[i]);
        }
        else
        {
            LOG(0, "\n");
        }
    }
}

BOOL GetAudDataDumpAutoSave(UINT32 u1DecId)
{
    if (u1DecId >= AUD_DEC_NUM)
    {
        LOG(0, "Invalid decode ID : %d \n", u1DecId);
        return FALSE;
    }

    return _fgAudDataDumpAutoSave[u1DecId];
}

void SetAudDataDumpAutoSave(UINT32 u1DecId,BOOL bAutoSave)
{
    if (u1DecId >= AUD_DEC_NUM)
    {
        LOG(0, "Invalid decode ID : %d \n", u1DecId);
        return;
    }

    _fgAudDataDumpAutoSave[u1DecId] = bAutoSave;
}

void AudDataDumpNextFile(UINT32 u1DecId)
{
    mm_segment_t oldfs;
    INT32 i;
    UINT32 len, iR = 0, iDot, id;
    char *fileName, szTmp[MAX_PATH];
    char szID[16];

    fileName = _szAudDataDumpFileName[u1DecId];
    len = x_strlen(fileName);
    iDot = len;
    for (i = len-1; i >= 0; i--)
    {
        switch (fileName[i])
        {
            case '(':
                if (iR > i)
                {
                    if ((iR == i+1) || (iR - i) > sizeof(szID))
                    {
                        id = 1;
                    }
                    else
                    {
                        x_strncpy(szID, fileName+i+1, iR - i - 1);
                        szID[iR - i - 1] = '\0';
                        id = (UINT32)StrToInt(szID);
                        id++;
                    }
                    x_snprintf(szID, sizeof(szID), "%d", id);
                    x_strncpy(szTmp, fileName+iR, len-iR+1);
                    x_strncpy(fileName+i+1, szID, x_strlen(szID)+1);
                    x_strncpy(fileName+i+1+x_strlen(szID), szTmp, len-iR+1);

                    goto get_file_name_end;
                }
                break;
            case ')':
                if ((iDot < len) || (i == len-1))
                    iR = i;
                break;
            case '/':
                goto for_end;
            case '.':
                if (iDot == len)
                {
                    iDot = i;
                    if ((i == 0) || (fileName[i-1] != ')'))
                    {
                        goto for_end;
                    }
                }
                break;
        }
    }
for_end:
    if (iDot < len)
    {
        x_strncpy(szID, "(1)", sizeof("(1)"));
        x_strncpy(szTmp, fileName+iDot, len-iDot+1);
        x_strncpy(fileName+iDot, szID, x_strlen(szID)+1);
        x_strncpy(fileName+iDot+x_strlen(szID), szTmp, len-iDot+1);
    }
    else
    {
        x_strncpy(szID, "(1)", sizeof("(1)"));
        x_strncpy(fileName+len, szID, x_strlen(szID)+1);
    }

get_file_name_end:
    oldfs = get_fs();
    set_fs(KERNEL_DS);

    if (_pAudDumpFile[u1DecId])
    {
        filp_close(_pAudDumpFile[u1DecId], 0);
    }

    _pAudDumpFile[u1DecId] = filp_open(fileName, (O_CREAT | O_WRONLY ), 0);
    if((IS_ERR(_pAudDumpFile[u1DecId])) || (NULL == _pAudDumpFile[u1DecId]))
    {
        LOG(0, "AudDataDumpNextFile() create file failed.\n");
        _pAudDumpFile[u1DecId] = NULL;
    }

    set_fs(oldfs);
}
#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static AUD_FMT_T AUD_CheckMpegLayer(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT32 u4Rp;
    AUD_FMT_T eFmt = AUD_FMT_UNKNOWN;
    
    u4Rp = VIRTUAL(u4GetAFIFOStart(u1DspId, u1DecId));

    //Only check the first 4bytes MPEG header
    //Return AUD_FMT_UNKNOWN if it is not a MPEG header
    DSP_FlushInvalidateDCacheFree(u4Rp, 32); 
    if ((((UINT8*)u4Rp)[0] == 0xff) && ((((UINT8*)u4Rp)[1]&0xe0)==0xe0) && 
        ((((UINT8*)u4Rp)[2]&0xf0)!=0xf0) && ((((UINT8*)u4Rp)[2]&0x0c)!=0x0c))
    {
        //FFFx or FFEx
        switch (((UINT8*)u4Rp)[1] & 0x6)
        {
        case 0x2: // 01b
            //MP3
            eFmt = AUD_FMT_MP3; 
            break;
        case 0x4: // 10b
        case 0x6: // 11b
            //MP12
            eFmt = AUD_FMT_MPEG;
            break;                
        default:
            break;                            
        }
    }
    return eFmt;
}

#if 1//def CC_AUD_QUERY_VDO_INFO
static INT16 _AudSrmGetVdoInfo(void)
{
    static BOOL fgAudVdoDelayTblInit = FALSE;
    static AUD_SRM_VDO_DELAY_TBL *rAudSrmVdoDelayTblDtv = NULL; //CC_AUD_QUERY_VDO_INFO
    static AUD_SRM_VDO_DELAY_TBL *rAudSrmVdoDelayTblHdmi = NULL;

#if 1//defined(CC_MT5396) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5881)
    VDO_INPUT_INFO rVdoInfo;
    AUD_SRM_VDO_HEIGHT_T eAudSrmVdoH;
    AUD_SRM_VDO_IP_T eAudSrmVdoIP;
    AUD_SRM_VDO_RATE_T eAudSrmVdoRate;
    INT16 i2Delay = 0;

    x_memset((VOID*)VIRTUAL((UINT32)&rVdoInfo), 0, sizeof(rVdoInfo));
    
    // 1. Get Video info
    DrvVideoGetInputTimingInfo(SV_VP_MAIN, &rVdoInfo);
    LOG(3,"Video : H=%d, W=%d, T=%d, I=%d, R=%d, P=%d\n",
        rVdoInfo.u2Height, rVdoInfo.u2Width, rVdoInfo.u1Timing,
        rVdoInfo.u1Interlace,rVdoInfo.u1RefreshRate, rVdoInfo.u1PanelDelay);
    // 2. Transform to audio emu types.
    switch (rVdoInfo.u2Height)
    {
        case 480:
            eAudSrmVdoH = AUD_SRM_VDO_480;
            break;
        case 576:
            eAudSrmVdoH = AUD_SRM_VDO_576;
            break;
        case 720:
            eAudSrmVdoH = AUD_SRM_VDO_720;
            break;
        case 1080:
            eAudSrmVdoH = AUD_SRM_VDO_1080;
            break;
        default:
            eAudSrmVdoH = AUD_SRM_VDO_480;
            break;
    }

    if (rVdoInfo.u1Interlace == 0)
    {
        eAudSrmVdoIP = AUD_SRM_VDO_P;
    }
    else
    {
        eAudSrmVdoIP = AUD_SRM_VDO_I;
    }

    if (rVdoInfo.u1RefreshRate == 50)
    {
        eAudSrmVdoRate = AUD_SRM_VDO_50HZ;
    }
    else
    {
        eAudSrmVdoRate = AUD_SRM_VDO_60HZ;
    }

    // Check Table
    if (!fgAudVdoDelayTblInit)
    {
        if ((DRVCUST_OptQuery(eAudioVdoDelayTableDtv, (UINT32*)&rAudSrmVdoDelayTblDtv) == 0) &&
             (DRVCUST_OptQuery(eAudioVdoDelayTableHdmi, (UINT32*)&rAudSrmVdoDelayTblHdmi) == 0) )
        {
            fgAudVdoDelayTblInit = TRUE;
        }
    }
    if (_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        if (rAudSrmVdoDelayTblDtv != NULL)
        {
            i2Delay = rAudSrmVdoDelayTblDtv->i2Delay[eAudSrmVdoH][eAudSrmVdoIP][eAudSrmVdoRate];
        }
    }
    else if (_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_HDMI)
    {
        if (rAudSrmVdoDelayTblHdmi != NULL)
        {
            i2Delay = rAudSrmVdoDelayTblHdmi->i2Delay[eAudSrmVdoH][eAudSrmVdoIP][eAudSrmVdoRate];
        }
    }

    return i2Delay;

#else
    UNUSED(_AudSrmGetVdoInfo);
    LOG(3, " VDO Info Qry is not support in this IC ......\n");
    return 0;
#endif // define (IC Chip)

}
#endif //CC_AUD_QUERY_VDO_INFO

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL

void _AudSrmStableCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    UINT8 u1DecId = AUD_DEC_MAIN;

    UNUSED(u4Arg1);
    UNUSED(u4Arg2);
    UNUSED(u4Arg3);

    if ( VDEC_IsExistDtvIFrmCase()&&(_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_DIGITAL_TUNER))
    {
        LOG(0,"===> DTV MHEG 5 Case, Cancel Video Mode Change\n");
        return; // if DTV MHEG 5 case , not sent mode change command , no need av sync
    }

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    //AUD_Aproc_Chk_DecId(&u1DecId);
#endif  

    if (u4Arg4 == AUD_VIDEO_IS_STABLE) //Stable Callback : means SRM is ready.
    {
        VERIFY(AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_VDP_MODE_CHG_DONE, 0));
    }
    else
    {
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        if (u1DecId <= AUD_DEC_THIRD)
#endif
            VERIFY(AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_VDP_MODE_CHG, 0));        
    }
    
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    // This is just temp solution.
    // Render may run decoder in Dec0 or 1.
    // Send message to both.
    // Final solution is request SRM to send MAIN and SUB callback.
    // And we send message to related decoders according to some logic.
    u1DecId = AUD_DEC_AUX;

    if (u4Arg4 == AUD_VIDEO_IS_STABLE)
    {
        VERIFY(AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_VDP_MODE_CHG_DONE, 0));
    }
    else
    {
        VERIFY(AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_VDP_MODE_CHG, 0));        
    }
#endif
    
}

#endif  // CC_AUD_ENABLE_PLAY_MUTE_CONTROL

static BOOL _AudDmxDtvInitPlaybackFifoFull(UINT8 u1DecId)
{
    UINT32 u4WritePtr;
//    UINT32 u4ReadPtr;
    UINT32 u4FifoEnd;
    UINT32 u4FifoStart;

    UNUSED(_AUD_GetAudFifo(AUD_DSP0, u1DecId, &u4FifoStart, &u4FifoEnd));
//    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
    _AUD_DMX_GetAudWrtPtr(u1DecId, &u4WritePtr);

    if (_arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount == 0)
    {
        return FALSE;
    }
    else
    {
        // Check if write pointer is within audio fifo
        VERIFY((u4WritePtr >= u4FifoStart) && (u4FifoEnd >= u4WritePtr));

        // Check fifo full
        if ((u4FifoEnd - u4WritePtr) < DTV_AUD_FIFO_FULL_THRESHOLD)
        {
            return TRUE;
        }
    }

    return FALSE;
}

static void _AudSetEvent(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4Event)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    _arAudDecoder[u1DspId][u1DecId].u4EventFlag = u4Event;
}

 void AUD_DrvSetMMAudioOnly(UINT8 u1DecId, BOOL fgMMAudioOnly)
{
    UNUSED(_fgMMAudioOnly);
    _fgMMAudioOnly[u1DecId] = fgMMAudioOnly;
}

#ifdef CC_ENABLE_AOMX
extern int _vdec_opencnt;
BOOL AUD_DrvGetMMAudioOnly(UINT8 u1DecId)
{
    if(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_GST)
    {
        return ((0 == _vdec_opencnt)?TRUE:FALSE);
    }   
    else
    {
        return _fgMMAudioOnly[u1DecId];
    }
}
#endif

#if (defined(CC_AUD_SUPPORT_MS10) && defined(CC_MT5391_AUD_3_DECODER))
static void _AudWaitEventAD(UINT32 * pu4Event, BOOL fgWait)
{
    //first check if there is data in 500ms
    if (!_AudWaitEvent(AUD_DSP0, AUD_DEC_THIRD, pu4Event, fgWait, AUD_EVENT_TIMEOUT_DTV_AD))
    {
        //no data in 500ms, set AD flag to one buffer mode
        if (_arAudDecoder[AUD_DSP0][AUD_DEC_THIRD].u4ReceivePesCount ==0)
        {
        	DSP_SetAdFreeRun(TRUE);
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
			*pu4Event = (UINT32)AUD_CMD_AVSYNC;
#else
        }
 
        //wait another 4000ms
        if (!_AudWaitEvent(AUD_DSP0, AUD_DEC_THIRD, pu4Event, fgWait, AUD_EVENT_TIMEOUT_DTV- AUD_EVENT_TIMEOUT_DTV_AD))
        {
            vDspSetFifoReadPtr(AUD_DSP0, AUD_DEC_THIRD, _arAudDecoder[AUD_DSP0][AUD_DEC_THIRD].rCurrentPes.u4Wp);
            DSP_SetStartPtsToShm(AUD_DSP0, AUD_DEC_THIRD, (UINT32)(_arAudDecoder[AUD_DSP0][AUD_DEC_THIRD].rCurrentPes.u4Pts & 0xffffffff), _arAudDecoder[AUD_DSP0][AUD_DEC_THIRD].rCurrentPes.u4Wp);
            *pu4Event = (UINT32)AUD_CMD_AVSYNC;
#endif
            LOG(7, "NO AV sync message retrieve in DTV AD Path\n");
        }
    }
    return;
}
#endif
static BOOL _AudWaitEvent(UINT8 u1DspId, UINT8 u1DecId, UINT32 * pu4Event, BOOL fgWait, UINT32 u2TimeOut)
{
    UINT32 u4Event;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(UINT32);
    AUD_DEC_CMD_T eCmd;
    INT32 i4CmdOk;
    INT32 i4Result;
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    do {
        if (!fgWait)
        {
            i4Result = x_msg_q_receive_timeout(&u2MsgIdx, &u4Event, &zMsgSize,
                               &_ahAudCmdQueue[u1DspId][u1DecId], 1, u2TimeOut);
            VERIFY((i4Result == OSR_OK) || (i4Result == OSR_TIMEOUT));
            if (i4Result == OSR_TIMEOUT)
            {
                return FALSE;
            }
        }
        else // wait until get new event
        {
            VERIFY(x_msg_q_receive(&u2MsgIdx, &u4Event, &zMsgSize,
                               &_ahAudCmdQueue[u1DspId][u1DecId], 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
        }

        eCmd = (AUD_DEC_CMD_T)u4Event;
        /* Check if decoder state is the same as command */
        if(((eCmd  == AUD_CMD_PLAY) && (_arAudDecoder[u1DspId][u1DecId].eDecState == AUD_DEC_PLAYING)) ||
           ((eCmd == AUD_CMD_STOP) && (_arAudDecoder[u1DspId][u1DecId].eDecState == AUD_DEC_STOP)) ||
           ((eCmd == AUD_CMD_PAUSE) && (_arAudDecoder[u1DspId][u1DecId].eDecState == AUD_DEC_PAUSE)))
        {
            LOG(7, "Aud dsp(%d) dec(%d) thread Get duplicated <%s> event\n", u1DspId, u1DecId, _paszAudCmd[u4Event]);
            AUD_CommandDone(u1DspId, u1DecId, eCmd, AUD_OK);
            i4CmdOk = FALSE;
        }
        else if (AUD_DrvDspCallBackProcess(u1DecId, eCmd))
        {
            i4CmdOk = FALSE;
            LOG(5, " DSP Callback AUD_DRVCmd %s\n",_paszAudCmd[eCmd]);
        }
        else
        {
            /* Check command and decoder event flag */
            if ((1 << (UINT8)eCmd) & _arAudDecoder[u1DspId][u1DecId].u4EventFlag)
            {
                i4CmdOk = TRUE;
                LOG(7, "Aud dsp(%d) dec(%d) thread Get valid <%s> event and wake up\n", u1DspId, u1DecId, _paszAudCmd[u4Event]);
            }
            else
            {
                //avoid swdmx hang by error resume command
                //ERROR CMD must command done to release semaphone!!! 2011/07/07
                AUD_CommandDone(u1DspId, u1DecId, eCmd, AUD_OK);
                LOG(0, "ERROR AUD_DRVCmd %s\n",_paszAudCmd[eCmd]);
                i4CmdOk = FALSE;
            }
        }
    } while (!i4CmdOk);

    if (pu4Event != NULL)
    {
        *pu4Event = u4Event;
    }

    return TRUE;
}

#ifndef CC_AUD_DDI
static void _WaitAtunerLockSignal(void)
{
    INT32 i4Ret;

    i4Ret = x_sema_lock_timeout(_hAtunerSyncSema, ATUNER_WAITING_TIME_OUT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

    if (i4Ret == OSR_TIMEOUT)
    {
        LOG(1, "_WaitAtunerLockSignal timeout\n");
    }
}
#endif

static void _DrvThreadInit(void)
{
    UINT8 u1DecId;
    UINT8 u1DspDecId[2];
    UINT8 u1StcId;

#ifdef CC_AUD_NCSTOOL_SUPPORT
    //Printf("!!!!!!!!!! Allocate SW Reg !!!!!!!!!!\n");
    vAllocateSWReg();
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    _AudMuxerInit();
#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
    _AudBluetoothMuxerInit();
#endif
//#endif

    if (!_fgAudDrvThreadInit)
    {
        //-----------------------------------------------------------
        // Create semaphore
        //-----------------------------------------------------------
        VERIFY(x_sema_create(&_hAtunerSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

        VERIFY(x_sema_create(&_hSemaLoadCode, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DSP0][AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DSP0][AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DSP0][AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DSP0][AUD_DEC_4TH], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DSP1][AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DSP1][AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DSP1][AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_ahSemaUopComDone[AUD_DSP1][AUD_DEC_4TH], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);        

        VERIFY(x_sema_create(&_hDetectionSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hFeedStreamSyncSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DSP0][AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DSP0][AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DSP0][AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DSP0][AUD_DEC_4TH], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DSP1][AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DSP1][AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DSP1][AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arAudDrvStateSema[AUD_DSP1][AUD_DEC_4TH], X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);        

#ifdef LINUX_TURNKEY_SOLUTION
        VERIFY(x_sema_create(&_hAudDataArrived[AUD_DEC_MAIN], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hAudDataArrived[AUD_DEC_AUX], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hAudDataArrived[AUD_DEC_THIRD], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hAudDataArrived[AUD_DEC_4TH], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif

#ifdef CC_AUD_UPLOAD_DSP_IRQ_QUEUE
        VERIFY((x_msg_q_create(&_hAudUploadDspIRQCmdQueue, ADRV_UPLOAD_DSP_IRQ_Q, sizeof(_AUD_UPLOAD_MESSAGE), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#else
        VERIFY(x_sema_create(&_hSemaDataUploadWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif

#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
#ifdef CC_AUD_BLUETOOTH_DSP_IRQ_QUEUE
        VERIFY((x_msg_q_create(&_hAudBluetoothDspIRQCmdQueue, ADRV_BLUETOOTH_DSP_IRQ_Q, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#else
        VERIFY(x_sema_create(&_hSemaDataBluetoothWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif
#endif
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
        VERIFY(x_sema_create(&_hSemaDataTransferWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hSemaDataTransferWait1, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
        VERIFY(x_sema_create(&_arParserInfo[AUD_DEC_MAIN]._hSemaHDMIParserThreadWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_arParserInfo[AUD_DEC_AUX]._hSemaHDMIParserThreadWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif

#ifdef CC_AUD_HP_DEPOP  ///For Remy, need remove "CC_DAC_DE_POP".
        VERIFY(x_sema_create(&_hSemaHpDepopWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
#endif

#ifdef CC_ENABLE_AOMX
#ifdef CC_AUD_ARM_RENDER
        VERIFY(x_sema_create(&GstTimeStampe[AUD_DEC_MAIN].hSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&GstTimeStampe[AUD_DEC_AUX].hSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&GstTimeStampe[AUD_DEC_THIRD].hSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY(x_sema_create(&GstTimeStampe[AUD_DEC_4TH].hSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif
#endif

        //-----------------------------------------------------------
        // Create message queue
        //-----------------------------------------------------------
        VERIFY((x_msg_q_create(&_hAudApllCmdQueue[0], ADRV_APLL_CMD_Q1, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
        VERIFY((x_msg_q_create(&_hAudApllCmdQueue[1], ADRV_APLL_CMD_Q2, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DSP0][AUD_DEC_MAIN], ADRV_CMD_Q_NAME1, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DSP0][AUD_DEC_AUX], ADRV_CMD_Q_NAME2, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DSP1][AUD_DEC_MAIN], ADRV_CMD_Q_NAME5, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DSP1][AUD_DEC_AUX], ADRV_CMD_Q_NAME6, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DSP0][AUD_DEC_THIRD], ADRV_CMD_Q_NAME3, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DSP1][AUD_DEC_THIRD], ADRV_CMD_Q_NAME7, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DSP0][AUD_DEC_4TH], ADRV_CMD_Q_NAME4, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
        VERIFY((x_msg_q_create(&_ahAudCmdQueue[AUD_DSP1][AUD_DEC_4TH], ADRV_CMD_Q_NAME8, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#endif

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
        VERIFY((x_msg_q_create(&_hAudPlayMuteCmdQueue[AUD_DEC_MAIN], ADRV_PLAY_MUTE_CMD_Q1, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
        VERIFY((x_msg_q_create(&_hAudPlayMuteCmdQueue[AUD_DEC_AUX], ADRV_PLAY_MUTE_CMD_Q2, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER
        VERIFY((x_msg_q_create(&_hAudPlayMuteCmdQueue[AUD_DEC_THIRD], ADRV_PLAY_MUTE_CMD_Q3, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#endif
#endif

#ifdef CC_AUD_DDI
        VERIFY((x_msg_q_create(&_hAudSoundBarCmdQueue, ADRV_SOUND_BAR_CMD_Q, sizeof(UINT32), AUD_CMD_QUEUE_SIZE)) == OSR_OK);
#endif

        //-----------------------------------------------------------
        // Create thread
        //-----------------------------------------------------------

        // Create audio thread1
        u1DspDecId[0] = AUD_DSP0;
        u1DspDecId[1] = AUD_DEC_MAIN;
        _ChangeAudioState(u1DspDecId[0], u1DspDecId[1], AUD_TRIGGER_ADSP);
        VERIFY(x_thread_create(&_hAudDrvThread[u1DspDecId[0]][u1DspDecId[1]], AUD_DRV_THREAD1_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDrvThread, 2*sizeof(UINT8), (void *)&u1DspDecId) == OSR_OK);

        // Create audio thread2
        u1DspDecId[0] = AUD_DSP0;
        u1DspDecId[1] = AUD_DEC_AUX;
        _ChangeAudioState(u1DspDecId[0], u1DspDecId[1], AUD_WAIT_POWER_ON);
        VERIFY(x_thread_create(&_hAudDrvThread[u1DspDecId[0]][u1DspDecId[1]], AUD_DRV_THREAD2_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDrvThread, 2*sizeof(UINT8), (void *)&u1DspDecId) == OSR_OK);

#ifdef CC_MT5391_AUD_3_DECODER
        // Create audio thread3
        u1DspDecId[0] = AUD_DSP0;
        u1DspDecId[1] = AUD_DEC_THIRD;
         _ChangeAudioState(u1DspDecId[0], u1DspDecId[1], AUD_WAIT_POWER_ON);
        VERIFY(x_thread_create(&_hAudDrvThread[u1DspDecId[0]][u1DspDecId[1]], AUD_DRV_THREAD3_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDrvThread, 2*sizeof(UINT8), (void *)&u1DspDecId) == OSR_OK);
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
        // Create audio thread4
        u1DspDecId[0] = AUD_DSP0;
        u1DspDecId[1] = AUD_DEC_4TH;
         _ChangeAudioState(u1DspDecId[0], u1DspDecId[1], AUD_WAIT_POWER_ON);
        VERIFY(x_thread_create(&_hAudDrvThread[u1DspDecId[0]][u1DspDecId[1]], AUD_DRV_THREAD4_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDrvThread, 2*sizeof(UINT8), (void *)&u1DspDecId) == OSR_OK);
#endif
#ifndef CC_AUD_DISABLE_2ND_DSP
        // Create audio thread5
        u1DspDecId[0] = AUD_DSP1;
        u1DspDecId[1] = AUD_DEC_MAIN;
        _ChangeAudioState(u1DspDecId[0], u1DspDecId[1], AUD_TRIGGER_ADSP);
        VERIFY(x_thread_create(&_hAudDrvThread[u1DspDecId[0]][u1DspDecId[1]], AUD_DRV_THREAD5_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDrvThread, 2*sizeof(UINT8), (void *)&u1DspDecId) == OSR_OK);

        // Create audio thread6
        u1DspDecId[0] = AUD_DSP1;
        u1DspDecId[1] = AUD_DEC_AUX;
        _ChangeAudioState(u1DspDecId[0], u1DspDecId[1], AUD_WAIT_POWER_ON);
        VERIFY(x_thread_create(&_hAudDrvThread[u1DspDecId[0]][u1DspDecId[1]], AUD_DRV_THREAD6_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDrvThread, 2*sizeof(UINT8), (void *)&u1DspDecId) == OSR_OK);
#endif
        // Create apll monitor thread
        u1StcId = 0;
        VERIFY(x_thread_create(&_hAudApllMonThread[u1StcId], AUD_APLL_MONITOR1_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudApllMonitorThread, sizeof(UINT8), (void *)&u1StcId) == OSR_OK);
        u1StcId = 1;
        VERIFY(x_thread_create(&_hAudApllMonThread[u1StcId], AUD_APLL_MONITOR2_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudApllMonitorThread, sizeof(UINT8), (void *)&u1StcId) == OSR_OK);   
#ifdef CC_AUD_DDI
        VERIFY(x_thread_create(&_hAudSoundBarThread, AUD_SOUND_BAR_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudSoundBarThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521  -- 2
        u1DecId = AUD_DEC_MAIN;
        // Create DTV playback thread
        VERIFY(x_thread_create(&_hAudDtvThread[u1DecId], AUD_DRV_DTV_MAIN_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDtvOnPlayThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#else
        u1DecId = AUD_DEC_AUX;
        // Create DTV playback thread
        VERIFY(x_thread_create(&_hAudDtvThread[u1DecId], AUD_DRV_DTV_AUX_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDtvOnPlayThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif

    // Create DSP monitor thread  //  -- DSP Force --
#ifdef DSP_FORCE_RESET_ENABLE
        VERIFY(x_thread_create(&_hAudDspMonThread, AUD_DSP_MONITOR_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDspMonitorThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
        u1DecId = AUD_DEC_MAIN;
        VERIFY(x_thread_create(&_hAudPlayMuteThread[u1DecId], AUD_DRV_PLAY_MUTE_THREAD1, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudPlayMuteThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
        u1DecId = AUD_DEC_AUX;
        VERIFY(x_thread_create(&_hAudPlayMuteThread[u1DecId], AUD_DRV_PLAY_MUTE_THREAD2, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudPlayMuteThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);        
#ifdef CC_MT5391_AUD_3_DECODER
        u1DecId = AUD_DEC_THIRD;
        VERIFY(x_thread_create(&_hAudPlayMuteThread[u1DecId], AUD_DRV_PLAY_MUTE_THREAD3, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudPlayMuteThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif
#endif //CC_AUD_ENABLE_PLAY_MUTE_CONTROL

#ifdef CC_AUD_HP_DEPOP  ///For Remy, need remove "CC_DAC_DE_POP".
        VERIFY(x_thread_create(&_hAudHpDePopThread, AUD_DRV_HP_DEPOP_THREAD, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudHpDePopThread, 0, NULL) == OSR_OK);
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
        VERIFY(x_thread_create(&_hAudDataUploadThread, AUD_DRV_DATA_UPLOAD_THREAD, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDataUploadThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
        VERIFY(x_thread_create(&_hAudDataBluetoothThread, AUD_DRV_DATA_BLUETOOTH_THREAD, AUD_DRV_THREAD_STACK_SIZE, 61,
            _AudDataBluetoothThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif
//#endif

        VERIFY(x_thread_create(&_hAudCodecChangThread, AUD_DRV_CODEC_CHG_THREAD, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudCodecChangeThread, 0, NULL) == OSR_OK);
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
        VERIFY(x_thread_create(&_hAudDataTransferThread, AUD_DRV_DATA_TRANSFER_THREAD, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudDataTransferThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
        u1DecId = AUD_DEC_MAIN;
        VERIFY(x_thread_create(&_hAudHDMIParserThread[0], AUD_DRV_Parser_THREAD1, AUD_PRE_PARSER_STACK_SIZE, AUD_PRE_PARSER_PRIORITY,
            _AudHDMIParserThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
        u1DecId = AUD_DEC_AUX;
        VERIFY(x_thread_create(&_hAudHDMIParserThread[1], AUD_DRV_Parser_THREAD2, AUD_PRE_PARSER_STACK_SIZE, AUD_PRE_PARSER_PRIORITY,
            _AudHDMIParserThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
#endif

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
        _AudOssAdap_Init();
#endif

#ifdef CC_AUD_NCSTOOL_SUPPORT
        //Printf("!!!!!!!!!! Create _AudToolThread !!!!!!!!!!\n");
        VERIFY(x_thread_create(&_hAudToolThread, AUD_TOOL_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
            _AudToolThread, 0, NULL) == OSR_OK);
#endif
        _fgAudDrvThreadInit = TRUE;
    }
}

static void _ChangeAudioState(UINT8 u1DspId, UINT8 u1DecId, AUD_DRV_STATE_T eNewState)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    UNUSED(_paszAudState);
    
    VERIFY(x_sema_lock(_arAudDrvStateSema[u1DspId][u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);
    if (_arAudDrvState[u1DspId][u1DecId] != eNewState)
    {
        LOG(7, "Aud dsp(%d)dec(%d) thread ChangeAudioState (%s -> %s)\n",u1DspId, u1DecId,_paszAudState[_arAudDrvState[u1DspId][u1DecId]], _paszAudState[eNewState]);
        _arAudDrvState[u1DspId][u1DecId] = eNewState;
    }
    VERIFY(x_sema_unlock(_arAudDrvStateSema[u1DspId][u1DecId]) == OSR_OK);

}

static void _FlushAudioFifo(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    DSP_GetAFIFOPhysicalAddr(u1DspId, u1DecId, &u4FifoStart, &u4FifoEnd);
    LOG( 9, "Flush audio fifo = (%x,%x)\n", u4FifoStart, u4FifoEnd);

    VERIFY(u4FifoEnd >= u4FifoStart);
    x_memset((VOID*)VIRTUAL(u4FifoStart), 0, u4FifoEnd - u4FifoStart);
    DSP_FlushInvalidateDCacheFree( u4FifoStart, u4FifoEnd - u4FifoStart);
}

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
static void _FlushParserFifo(void)
{
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;


    DSP_GetHDMIParserFIFOPhysicalAddr(&u4FifoStart, &u4FifoEnd);
    LOG( 5, "Flush parser fifo = (%x,%x)\n", u4FifoStart, u4FifoEnd);

    VERIFY(u4FifoEnd >= u4FifoStart);
    x_memset((VOID*)VIRTUAL(u4FifoStart), 0, u4FifoEnd - u4FifoStart);
    DSP_FlushInvalidateDCacheFree( u4FifoStart, u4FifoEnd - u4FifoStart);
}
#endif

static void _FlushUploadFifo(void)
{
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;

    DSP_GetUploadPhysicalAddr(&u4FifoStart, &u4FifoEnd);
    LOG( 9, "Flush upload fifo = (%x,%x)\n", u4FifoStart, u4FifoEnd);

    VERIFY(u4FifoEnd >= u4FifoStart);
    x_memset((VOID*)VIRTUAL(u4FifoStart), 0, u4FifoEnd - u4FifoStart);
    DSP_FlushInvalidateDCacheFree( u4FifoStart, u4FifoEnd - u4FifoStart);
}

#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
static void _FlushBluetoothFifo(void)
{
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;

    DSP_GetBluetoothPhysicalAddr(&u4FifoStart, &u4FifoEnd);
    LOG( 9, "Flush upload fifo = (%x,%x)\n", u4FifoStart, u4FifoEnd);

    VERIFY(u4FifoEnd >= u4FifoStart);
    x_memset((VOID*)VIRTUAL(u4FifoStart), 0, u4FifoEnd - u4FifoStart);
    DSP_FlushInvalidateDCacheFree( u4FifoStart, u4FifoEnd - u4FifoStart);
}
#endif

static void _AudFeedStreamThread(const void* pvArg)
{
    AUD_MEM_STATE_T eMemState = AUD_MEM_UNKNOWN;
    AUD_MEM_STATE_T eNewState;
    UINT8 u1DspId = AUD_DSP0;
    UINT8 u1DecId = AUD_DEC_MAIN;
    UINT8* tmpptr;
    UINT32 u4Residual;
    UINT32 u4TransferSZ;
    UINT32 u4DesAddr;
    UINT32 u4AFifofSZ;
    UINT32 u4StreamAddr;
    UINT32 u4Retry = 0;
    INT32 i4Ret;
#ifdef CC_AUD_MULTI_BUFFER_MEMORY_PLAY
    UINT32 i, u4Idx, u4Bid;
#endif

    UINT32 u4ReadPtr;
    UINT32 u4WritePtrNext;
    //UINT32 u4DataSize=0;
    
    AudDecNfyFct  pfAudDecNfy = NULL;
    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));
    
    if (pvArg != NULL)
    {
        tmpptr = (UINT8 *)pvArg;
        u1DspId = tmpptr[0];
        u1DecId = tmpptr[1];
    }
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    AUD_SetPaserRpSrc(u1DspId, u1DecId);

    _fgForceStopStrmDma = FALSE;
    _u4FeedStreamResult = (UINT32)AUD_OK;
    
    _fgFeedStrmFinshed = FALSE; // Feed_and_Auddrv Thread sync

    i4Ret = x_sema_lock(_hFeedStreamSyncSema, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

#ifdef CC_AUD_MULTI_BUFFER_MEMORY_PLAY
    // Wait start
    if (_afgAudMultiMemBufMode)
    {
        while (!_afgAudMemBufStart)
        {
            x_thread_delay(10);
            if (_fgForceStopStrmDma)
            {
                goto _FEED_STREAM_THREAD_END;
            }

            eNewState = AUD_MEM_NO_DATA;
            if ((pfAudDecNfy != NULL) && (eNewState != eMemState))
            {
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_MEM_STATE_CHG, eNewState, 0);
                eMemState = eNewState;
                LOG(7, "[MULTI_BUFFER] no data!\n");
            }
        }

        LOG(7, "[MULTI_BUFFER] feed stream: currIdx (%d), pData (0x%x), u4Length (0x%x)\n", _au4AudMemBufCurrIdx,
                _arAudMemBuf[_au4AudMemBufCurrIdx].pData, _arAudMemBuf[_au4AudMemBufCurrIdx].u4Length);
        _arAudDecoder[u1DspId][u1DecId].rMemBuffer.pData = _arAudMemBuf[_au4AudMemBufCurrIdx].pData;
        _arAudDecoder[u1DspId][u1DecId].rMemBuffer.u4Length = _arAudMemBuf[_au4AudMemBufCurrIdx].u4Length;
        _arAudDecoder[u1DspId][u1DecId].rMemBuffer.u4LoopCount = 1;
    }
#endif

    u4DesAddr = u4GetAFIFOStart(u1DspId, u1DecId);
    u4AFifofSZ = u4GetAFIFOEnd(u1DspId, u1DecId) - u4GetAFIFOStart(u1DspId, u1DecId);
    u4Residual = _arAudDecoder[u1DspId][u1DecId].rMemBuffer.u4Length;
    u4StreamAddr = PHYSICAL((UINT32)_arAudDecoder[u1DspId][u1DecId].rMemBuffer.pData);

    _i4MemStrLoopCnt = (INT32)_arAudDecoder[u1DspId][u1DecId].rMemBuffer.u4LoopCount;
    _fgInfinite = (_i4MemStrLoopCnt <= 0) ? TRUE : FALSE;

    eMemState = AUD_MEM_NORMAL;
    UNUSED(eNewState);
    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_MEM_STATE_CHG, eMemState, 0);
    }

    while (u4Residual > 0)
    {
        u4TransferSZ = (u4Residual > TRANSFER_SZ) ? TRANSFER_SZ : u4Residual;

        LOG(9, "Dec(%d) Des: 0x%08x, Size: 0x%08x, Res: 0x%08x, Loop Cnt: %d\n",
               u1DecId, u4DesAddr, u4TransferSZ, u4Residual, _i4MemStrLoopCnt);

        u4WritePtrNext = ((u4DesAddr + u4TransferSZ) >= u4GetAFIFOEnd(u1DspId, u1DecId)) ?
                      ((u4DesAddr + u4TransferSZ) - u4AFifofSZ) :
                      (u4DesAddr + u4TransferSZ);
                      
    //add to ensure not to overlap AFIFO data.    
        while (_arAudDecoder[u1DspId][u1DecId].eDecState != AUD_DEC_STOP 
            && _arAudDecoder[u1DspId][u1DecId].eDecState != AUD_DEC_INIT)
        {
            u4ReadPtr = DSP_GetDspReadPtrPhysicalAddr(u1DspId, u1DecId);
            if (u4WritePtrNext < u4DesAddr)
            {
                //  u4WritePtrNext  u4DesAddr
                if ((u4ReadPtr > u4WritePtrNext) && (u4ReadPtr <= u4DesAddr))
                {
                    break;
                }
            }
            else
            {
                //  u4DesAddr   u4WritePtrNext
                if ((u4ReadPtr > u4WritePtrNext) || (u4ReadPtr <= u4DesAddr))
                {
                    break;
                }
            }
            x_thread_delay(1);
        }

        if (!PSR_SoftTransfer(PSR_STREAM_AUDIO, u1DspId, u1DecId,
                              u4StreamAddr, u4TransferSZ,
                              u4DesAddr, u4GetAFIFOStart(u1DspId, u1DecId),
                              u4GetAFIFOEnd(u1DspId, u1DecId), FALSE))
        {
            if (u4ParserFullFlag > 0)
            {
                LOG(5, "Aud parser full!!! Flag (%d)\n", u4ParserFullFlag);
                x_thread_delay(1);
                if (_fgForceStopStrmDma)
                {
                    goto _FEED_STREAM_THREAD_END;
                }
                continue;
            }
            _u4FeedStreamResult = (UINT32)AUD_FAIL; // error
            LOG(0, "Aud soft transter fail/terminated\n");
            eMemState = AUD_MEM_ERROR;
            if (pfAudDecNfy != NULL)
            {
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_MEM_STATE_CHG, eMemState, 0);
            }
            break;
        }

        if (_fgForceStopStrmDma)
        {
            goto _FEED_STREAM_THREAD_END;
        }

        u4DesAddr = ((u4DesAddr + u4TransferSZ) < u4GetAFIFOEnd(u1DspId, u1DecId)) ?
                     (u4DesAddr + u4TransferSZ) : ((u4DesAddr + u4TransferSZ) - u4AFifofSZ);
        u4Residual -= u4TransferSZ;
        u4StreamAddr += u4TransferSZ;

        // Check loop count and end condition
        if (u4Residual == 0)
        {
            _i4MemStrLoopCnt--;
            if ((_i4MemStrLoopCnt > 0) || _fgInfinite)
            {
                u4Residual = _arAudDecoder[u1DspId][u1DecId].rMemBuffer.u4Length;
                u4StreamAddr = (UINT32)_arAudDecoder[u1DspId][u1DecId].rMemBuffer.pData;
                eMemState = AUD_MEM_DECODE_DONE;
                if (pfAudDecNfy != NULL)
                {
                    pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_MEM_STATE_CHG, eMemState, 0);
                }
            }
        }
        if ((_arAudDecoder[u1DspId][u1DecId].eDecFormat != AUD_FMT_PCM)&&(_arAudDecoder[u1DspId][u1DecId].eDecFormat != AUD_FMT_LPCM))
        {
            x_thread_delay(10); /*if data is garbage, hand over control*/
        }
        
    #ifdef CC_AUD_MULTI_BUFFER_MEMORY_PLAY
        if (_afgAudMultiMemBufMode && (u4Residual == 0))
        {
            _arAudMemBuf[_au4AudMemBufCurrIdx].fgValid = FALSE;
            eMemState = AUD_MEM_DECODE_DONE;
            if (pfAudDecNfy != NULL)
            {
                pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_MEM_STATE_CHG, eMemState, 0);
            }
            // End of block
            if (_arAudMemBuf[_au4AudMemBufCurrIdx].fgEOB)
            {
                eMemState = AUD_MEM_ENDOF_DATA;
                if (pfAudDecNfy != NULL)
                {
                    pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_MEM_STATE_CHG, eMemState, 0);
                }
                LOG(7, "[MULTI_BUFFER] parse data done!\n");
                break;
            }

            u4Bid = _au4AudMemBufCurrBid;
            u4Idx = (_au4AudMemBufCurrIdx + 1) % MAX_MEM_BUFFER_NUMBER;
            for (i = (u4Idx + 1) % MAX_MEM_BUFFER_NUMBER; 
                    i != _au4AudMemBufCurrIdx; i = (i + 1) % MAX_MEM_BUFFER_NUMBER)
            {
                if ((_arAudMemBuf[i].fgValid) && 
                    ((!_arAudMemBuf[u4Idx].fgValid) || (_arAudMemBuf[i].u4Bid < _arAudMemBuf[u4Idx].u4Bid)))
                {
                    u4Idx = i;
                }
            }
            
            if (_arAudMemBuf[u4Idx].fgValid)
            {
                _au4AudMemBufCurrIdx = u4Idx;
            }
            else
            {
                // Wait data
                while (u4Bid == _au4AudMemBufCurrBid)
                {
                    x_thread_delay(5);
                    if (_fgForceStopStrmDma)
                    {
                        goto _FEED_STREAM_THREAD_END;
                    }
                    eNewState = AUD_MEM_NO_DATA;
                    if ((pfAudDecNfy != NULL) && (eNewState != eMemState))
                    {
                        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_MEM_STATE_CHG, eNewState, 0);
                        eMemState = eNewState;
                    }
                }

                for (i = 0; i < MAX_MEM_BUFFER_NUMBER; i++)
                {
                    if ((_arAudMemBuf[i].fgValid) && (_arAudMemBuf[i].u4Bid == u4Bid))
                    {
                        _au4AudMemBufCurrIdx = i;
                        break;
                    }
                }

                eMemState = AUD_MEM_NORMAL;
                if (pfAudDecNfy != NULL)
                {
                    pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_MEM_STATE_CHG, eMemState, 0);
                }
            }

            LOG(7, "[MULTI_BUFFER] feed stream: currIdx (%d), pData (0x%x), u4Length (0x%x)\n", _au4AudMemBufCurrIdx,
                _arAudMemBuf[_au4AudMemBufCurrIdx].pData, _arAudMemBuf[_au4AudMemBufCurrIdx].u4Length);
                
            // Update mem info
            _arAudDecoder[u1DspId][u1DecId].rMemBuffer.pData = _arAudMemBuf[_au4AudMemBufCurrIdx].pData;
            _arAudDecoder[u1DspId][u1DecId].rMemBuffer.u4Length = _arAudMemBuf[_au4AudMemBufCurrIdx].u4Length;

            u4Residual = _arAudDecoder[u1DspId][u1DecId].rMemBuffer.u4Length;
            u4StreamAddr = PHYSICAL((UINT32)_arAudDecoder[u1DspId][u1DecId].rMemBuffer.pData);
            _i4MemStrLoopCnt = (INT32)_arAudDecoder[u1DspId][u1DecId].rMemBuffer.u4LoopCount;
        }
    #endif
    }

    // Check if the audio fifo has been empty
    while (1)
    {
        if (_fgForceStopStrmDma)
        {
            goto _FEED_STREAM_THREAD_END;
        }

        if ((AUD_IsFifoEmpty(u1DspId, u1DecId)) || (u4Retry > 100))
        {
            break;
        }

        if((_arAudDecoder[u1DspId][u1DecId].eDecState != AUD_DEC_PAUSE)&&(_arAudDecoder[u1DspId][u1DecId].eDecState != AUD_DEC_INIT))
        {
            u4Retry++;
        }

        i4Ret = x_sema_lock_timeout(_hFeedStreamSyncSema, FEED_STREAM_WAITING_TIME_OUT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
        //x_thread_delay(100);
    }
    UNUSED(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_STOPSTREAM));

#ifdef CC_AUD_SUPPORT_CLIP_NOTIFY_FUNC
    if (_hClipNotifyFunc != NULL)
    {
        _hClipNotifyFunc(u1DecId, 0, 0);
    }
#endif

    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_CLIP_PLAY_DONE, 0, 0);
    }
    LOG(9, "Dec(%d) stream size 0x%08x finished\n", u1DecId,
           _arAudDecoder[u1DspId][u1DecId].rMemBuffer.u4Length);

_FEED_STREAM_THREAD_END:
#ifdef CC_AUD_MULTI_BUFFER_MEMORY_PLAY
    AUD_DRVMultiBufferReset();
#endif
    _fgFeedStrmFinshed = TRUE;    // Feed_and_Auddrv Thread sync
    x_thread_exit();
}

static BOOL _fgFeederThreadGo = FALSE;

#define FEEDER_EMPTY_CNT_THRESHOLD 3

static HANDLE_T _hSemaFeederThreadWait;

static void _AudFeederThread (const void* pvArg)
{
    UINT8 u1DspId = AUD_DSP0;
    UINT8 u1DecId = AUD_DEC_MAIN;
    UINT8* tmpptr;
    UINT32 u4TransferSZ;
    UINT32 u4DesAddr;
    UINT32 u4AFifofSZ;
    UINT32 u4StreamAddr;
    FEEDER_TRANSMIT_COND rFeederTransmitData = {0};
    BOOL   fgFeedInit = FALSE;
    BOOL   fgFeedEnd = FALSE;

    if (pvArg != NULL)
    {
        tmpptr = (UINT8 *)pvArg;
        u1DspId = tmpptr[0];
        u1DecId = tmpptr[1];
    }
    AUD_DSP_ID_VALIDATE(u1DspId);

    AUD_DEC_ID_VALIDATE_2(u1DecId);

    while (1)
    {
        VERIFY(x_sema_lock(_hSemaFeederThreadWait, X_SEMA_OPTION_WAIT) == OSR_OK);

        fgFeedInit = FALSE;
        fgFeedEnd = FALSE;

        AUD_SetPaserRpSrc(u1DspId, u1DecId);
        
        // Initial setup
        _fgFeederThreadGo = TRUE;
        u4DesAddr = PHYSICAL(u4GetAFIFOStart(u1DspId, u1DecId));
        u4AFifofSZ = u4GetAFIFOEnd(u1DspId, u1DecId) - u4GetAFIFOStart(u1DspId, u1DecId);
        u4StreamAddr = PHYSICAL((UINT32)_arAudDecoder[u1DspId][u1DecId].rMemBuffer.pData);

        while (_fgFeederThreadGo)
        {
            INT32 i4Ret;

            // Get data source
            if (_rFeedTransmitInfo.pfnTransHandle)
            {
                _rFeedTransmitInfo.pfnTransHandle(FEEDER_AUDIO_SOURCE,
                         FEEDER_REQUEST_DATA_SIZE, 0, &rFeederTransmitData, 0);
                UNUSED(i4Ret);

                u4StreamAddr = PHYSICAL(rFeederTransmitData.u4BufAddr);
                u4TransferSZ = rFeederTransmitData.u4TransmitSize;

                LOG(9, "Feeder Dec(%d) Src: 0x%08x, Des: 0x%08x, Size: 0x%08x\n",
                    u1DecId, u4StreamAddr, u4DesAddr, u4TransferSZ);

                if (u4TransferSZ > 0)
                {
                    if (!PSR_SoftTransfer(PSR_STREAM_AUDIO, u1DspId, u1DecId,
                                      u4StreamAddr, u4TransferSZ,
                                      u4DesAddr, u4GetAFIFOStart(u1DspId, u1DecId),
                                      u4GetAFIFOEnd(u1DspId, u1DecId), FALSE))
                    {
                        LOG(5, "Aud soft transter fail/terminated\n");
                        break;
                    }

                    if ((_rFeedTransmitInfo.pfnNotify) && !fgFeedInit)
                    {
                        INT32 i4Ret;
                        i4Ret = _rFeedTransmitInfo.pfnNotify(FEEDER_AUDIO_SOURCE,
                                        FEEDER_NOTIFY_FILEBEGIN, 0);
                        UNUSED(i4Ret);
                        fgFeedInit = TRUE;
                    }
                }
                else // u4TransferSZ = 0  =>  no data
                {
                    x_thread_delay(50);

                    if ((_rFeedTransmitInfo.pfnNotify) && fgFeedInit && !fgFeedEnd)
                    {
                        if (AUD_IsFifoEmpty(u1DspId, u1DecId))
                        {
                            i4Ret = _rFeedTransmitInfo.pfnNotify(FEEDER_AUDIO_SOURCE,
                                        FEEDER_NOTIFY_FILEEND, 0);
                            fgFeedEnd = TRUE;
                            UNUSED(i4Ret);
                        }
                    }
                }

                u4DesAddr = ((u4DesAddr + u4TransferSZ) < u4GetAFIFOEnd(u1DspId, u1DecId)) ?
                             (u4DesAddr + u4TransferSZ) : ((u4DesAddr + u4TransferSZ) - u4AFifofSZ);

            }
            else
            {
                x_thread_delay(50);
            }

        }
    }
    // destroy thread
    //x_thread_exit();  //FIXME!! 2007/11/22 The statement is unreachable (no break condition in while(1)), so mark it temporary.
}

void AUD_SentReaptNumber(UINT8 u1DecId, UINT32 ReaptNumber)
{
    _arAudDecoder[AUD_DSP0][u1DecId].rMemBuffer.u4LoopCount = ReaptNumber;
}

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
static void _AudHDMIParserThread(void* pvArg)
{
    UINT8 u1DecId = AUD_DEC_MAIN;
    UINT32 u4TransferTotalSZ = 0;
    UINT32 u4DesAddr = 0;
    UINT32 u4StreamAddr = 0;
    UINT32 u4TransferSZ = 0;
    UINT32 u4Pd = 0;

    if (pvArg != NULL)
    {
        u1DecId = *(UINT8 *)pvArg;
    }
    AUD_DEC_ID_VALIDATE_2(u1DecId);//fix clockwork check
    _arParserInfo[u1DecId].eDSPBuffer.u4FifoStartAddr = PHYSICAL(u4GetAFIFOStart(AUD_DSP0, u1DecId));
    _arParserInfo[u1DecId].eDSPBuffer.u4FifoEndAddr   = PHYSICAL(u4GetAFIFOEnd(AUD_DSP0, u1DecId));
    _arParserInfo[u1DecId].eDSPBuffer.u4FifoSize      = u4GetAFIFOEnd(AUD_DSP0, u1DecId) - u4GetAFIFOStart(AUD_DSP0, u1DecId);
    _arParserInfo[u1DecId].eHDMIBuffer.u4FifoStartAddr = PHYSICAL((UINT32)u4GetHDMIParserFIFOStart());
    _arParserInfo[u1DecId].eHDMIBuffer.u4FifoEndAddr= PHYSICAL((UINT32)u4GetHDMIParserFIFOEnd());
    _arParserInfo[u1DecId].eHDMIBuffer.u4FifoSize= u4GetHDMIParserFIFOEnd()- u4GetHDMIParserFIFOStart();

    while (1)
    {
        VERIFY(x_sema_lock(_arParserInfo[u1DecId]._hSemaHDMIParserThreadWait, X_SEMA_OPTION_WAIT) == OSR_OK);
        _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_INIT;
        _arParserInfo[u1DecId]._fgReset = FALSE;
        while(_arParserInfo[u1DecId]._fgHDMIParserThreadGo)
        {
            if(_arParserInfo[u1DecId]._fgReset)
            {
                _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_INIT;
                _arParserInfo[u1DecId]._fgReset = FALSE;
            }
            switch(_arParserInfo[u1DecId].eHDMIParserSta )
            {
            case AUD_HDMI_PARSER_INIT:
                _FlushAudioFifo(AUD_DSP0, u1DecId);
                u4DesAddr = _arParserInfo[u1DecId].eDSPBuffer.u4FifoStartAddr;
                u4StreamAddr = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoStartAddr;
                u4TransferTotalSZ = 0;
                _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoStartAddr;
                _arParserInfo[u1DecId].eSpdifInfo.u4Size = 0;
                _arParserInfo[u1DecId].eSpdifInfo.u4Wp = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoStartAddr;
                _arParserInfo[u1DecId].u4LastTransferPtr = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoStartAddr;
                _arParserInfo[u1DecId].u4PaPb = 0;
                _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_FIND_VALID_FRAME;
                _arParserInfo[u1DecId].u4NoRawDataSize = 0;
                _arParserInfo[u1DecId].u4NoTransferSize = 0;
                _arParserInfo[u1DecId].e_aud_det_fmt = _arAudDecoder[AUD_DSP0][u1DecId].eDecFormat;
                _arParserInfo[u1DecId]._fgFmtChg = FALSE;
                _arParserInfo[u1DecId]._fgDTSCD = FALSE;
                _arParserInfo[u1DecId]._fgRaw = FALSE;
                _arParserInfo[u1DecId].u4TFrameCount = 0;
                break;
                
            case AUD_HDMI_PARSER_FIND_VALID_FRAME:
                if(AUD_DrvCheckHDMISize(_arParserInfo[u1DecId].eHDMIBuffer,HDMI_PARSER_PCM_TRANSFER_SIZE+6))
                {
                    DSP_InvalidateDCache(_arParserInfo->eHDMIBuffer.u4FifoPtr,HDMI_PARSER_PCM_TRANSFER_SIZE+6);
                    while (TRUE)
                    {
                        if(AUD_DrvCheckIECSyncWord(&_arParserInfo[u1DecId]))
                        {
                            u4Pd = AUD_DrvSwapWord(AUD_READIEC16(VIRTUAL(AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+2))));
                            if(_arParserInfo[u1DecId].eSpdifInfo.u2format==0x15)  //0x15 means E-AC3
                            {
                                u4TransferTotalSZ = u4Pd;//(u4Pd+1)&0xFFFFFFFE;  //EAC3 pd is in byte //16bit alignment
                            }
                            else
                            {
                                u4TransferTotalSZ = u4Pd/8;//(u4Pd/8+1)&0xFFFFFFFE;  //other format pd is in bit,but we transfer via parser is in  byte    //16bit alignment
                            }
                            _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+4);
                            _arParserInfo[u1DecId].eSpdifInfo.u4Size = u4Pd;
                            _arParserInfo[u1DecId].e_aud_det_fmt = AUD_DrvPcToFormat(u1DecId, _arParserInfo[u1DecId].eSpdifInfo.u2format);
                            if (u4TransferTotalSZ > 0)
                            {
                                _arParserInfo[u1DecId].u4NoRawDataSize = 8;
                                _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_WAIT_VALID_FRAME;
                            }
                            else
                            {
                                _arParserInfo[u1DecId].u4NoRawDataSize = 0;
                                _arParserInfo[u1DecId].u4NoTransferSize = 0;
                                _arParserInfo[u1DecId].u4LastTransferPtr = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr;
                            }
                            break;
                        }
                        else
                        {

                            if(_arParserInfo[u1DecId]._fgDTSSW)
                            {
                                _arParserInfo[u1DecId].u4NoRawDataSize = 2;
                                _arParserInfo[u1DecId].e_aud_det_fmt = AUD_DrvPcToFormat(u1DecId, _arParserInfo[u1DecId].eSpdifInfo.u2format);  //for DTS CD ,we set pc as 0XB in function AUD_DrvCheckIECSyncWord
                            }
                            _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+2);
                            _arParserInfo[u1DecId].u4NoRawDataSize += 2;
                            _arParserInfo[u1DecId].u4NoTransferSize += 2;
                            //_arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_FIND_VALID_FRAME;
                            if(_arParserInfo[u1DecId].u4NoRawDataSize >= HDMI_PARSER_DETECT_PCM_SIZE)
                            {
                                _arParserInfo[u1DecId]._fgRaw = FALSE;
                                _arParserInfo[u1DecId]._fgDTSCD =FALSE;
                                _arParserInfo[u1DecId].e_aud_det_fmt = AUD_FMT_PCM;
                            }
                            
                            // transfer all DTS CD data, the valid data length is not the same as FSIZE in DTS header
                            if((_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_PCM)||_arParserInfo[u1DecId]._fgDTSCD)
                            {
                                if(_arParserInfo[u1DecId].u4NoTransferSize>=HDMI_PARSER_PCM_TRANSFER_SIZE)
                                {
                                    u4TransferTotalSZ = _arParserInfo[u1DecId].u4NoTransferSize;
                                    _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = _arParserInfo[u1DecId].u4LastTransferPtr;
                                    _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_WAIT_VALID_FRAME;
                                    break;
                                }
                            }

                            if ((_arParserInfo[u1DecId].u4NoTransferSize - u4TransferTotalSZ)>=HDMI_PARSER_PCM_TRANSFER_SIZE)
                            {
                                u4TransferTotalSZ = _arParserInfo[u1DecId].u4NoTransferSize;
                                break;
                            }
                        }
                    }
                    
                    if(_arParserInfo[u1DecId].e_aud_det_fmt!=_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat)
                    {
                        if (!_arParserInfo[u1DecId]._fgFmtChg)
                        {
                            LOG(7, "Aud format change from (%d) to (%d).\n", _arAudDecoder[AUD_DSP0][u1DecId].eDecFormat, 
                                        _arParserInfo[u1DecId].e_aud_det_fmt);
                        }
                        _arParserInfo[u1DecId]._fgFmtChg = TRUE;
                    }
                    else
                    {
                        if(_arParserInfo[u1DecId]._fgFmtChg)
                        {
                            LOG(7,"Format change two times.\n");
                        }
                        _arParserInfo[u1DecId]._fgFmtChg = FALSE;
                    }
                }
                else
                {
                    LOG(9,"check sync word not enough data.\n");
                    x_thread_delay(1);
                }
                break;
                
            case  AUD_HDMI_PARSER_WAIT_VALID_FRAME:
                if(AUD_DrvCheckHDMISize(_arParserInfo[u1DecId].eHDMIBuffer,u4TransferTotalSZ+1))
                { 
                    if(!_arParserInfo[u1DecId]._fgFmtChg)
                    {
                        _arParserInfo[u1DecId].u4TFrameCount++;
                    }
                    _arParserInfo[u1DecId].eSpdifInfo.u4Wp = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr;
                    _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_TANSFER_FRAME;
                }
                else
                {
                    x_thread_delay(1);
                }
                break;
                
            case   AUD_HDMI_PARSER_TANSFER_FRAME:
               while(u4TransferTotalSZ>0)
               {
                   u4StreamAddr = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr;
                   if((u4StreamAddr+u4TransferTotalSZ)>u4GetHDMIParserFIFOEnd())
                   {
                       u4TransferSZ = u4GetHDMIParserFIFOEnd()-u4StreamAddr;
                   }
                   else
                   {
                       u4TransferSZ = u4TransferTotalSZ;
                   }
                   //parser max transfer size=15.5M always > raw data frame size
                   if(!_arParserInfo[u1DecId]._fgFmtChg)
                   {
                       if (!PSR_SoftTransfer(PSR_STREAM_AUDIO, AUD_DSP0, u1DecId,
                       u4StreamAddr, u4TransferSZ ,
                       u4DesAddr, u4GetAFIFOStart(AUD_DSP0, u1DecId),
                       u4GetAFIFOEnd(AUD_DSP0, u1DecId), FALSE))
                       {
                           u4TransferTotalSZ = 0;
                           if (u4ParserFullFlag == 1)
                           {
                               PSR_Reset(AUD_DSP0);
                           }
                           LOG(2, "Aud soft transter fail/terminated\n",u4ParserFullFlag);
                           break;
                       }
                       u4DesAddr = AUD_DrvGetCircularAddress(u4GetAFIFOStart(AUD_DSP0, u1DecId),u4GetAFIFOEnd(AUD_DSP0, u1DecId),u4DesAddr+u4TransferSZ);
                   }
                   else
                   {
                        LOG(7, "Don't transfer this frame, since audio format change.\n");
                   }
                   _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+u4TransferSZ);
                   _arParserInfo[u1DecId].u4LastTransferPtr = _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr;
                   u4TransferTotalSZ -= u4TransferSZ;
                   _arParserInfo[u1DecId].u4NoRawDataSize += u4TransferSZ;
               }
               _arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),((_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr+1)&0xFFFFFFFE));
               _arParserInfo[u1DecId].eHDMIParserSta = AUD_HDMI_PARSER_FIND_VALID_FRAME;
               _arParserInfo[u1DecId].u4NoTransferSize = 0;
               break;
            }
        }
    }
}
#endif

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
#if 0
extern UINT32 u4PREG_ARP ;     //1d4
extern UINT32 u4PREG_ARPOFF ; //1d8
extern UINT32 u4PREG_A0STA ; //0x14
extern UINT32 u4PREG_A0END ; //0x18
extern UINT32 u4PREG_PLENG ; //0x4
extern UINT32 u4PREG_AWP ; //0x5c
extern UINT32 u4PREG_BITTYPE ;// 0x3c
#endif
extern UINT32 u4ParserFullFlag;
void _AudHDMIShowStatus(UINT8 u1DecId)
{
    if(_arParserInfo[u1DecId]._fgHDMIParserThreadGo)
    {
        LOG(0,"Parser thread is enable.\n");
    }
    else
    {
        LOG(0,"Parser thread is disable.\n");
    }
    LOG(0,"PFIFO: start,end,rp[0x%08x,0x%08x,0x%08x].\n",_arParserInfo[u1DecId].eHDMIBuffer.u4FifoStartAddr,_arParserInfo[u1DecId].eHDMIBuffer.u4FifoEndAddr,_arParserInfo[u1DecId].eHDMIBuffer.u4FifoPtr);
    LOG(0,"AFIFO: start,end,rp[0x%08x,0x%08x,0x%08x].\n",_arParserInfo[u1DecId].eDSPBuffer.u4FifoStartAddr,_arParserInfo[u1DecId].eDSPBuffer.u4FifoEndAddr,_arParserInfo[u1DecId].eDSPBuffer.u4FifoPtr);
    LOG(0,"Parser detect audio format is %d.\n",_arParserInfo[u1DecId].e_aud_det_fmt);
    if(_arParserInfo[u1DecId]._fgFmtChg)
    {
        LOG(0,"Format change, parser won't transfer any data.\n");
    }
    else
    {
        LOG(0,"Format don't change, parser will transfer valid data.\n");
    }
    LOG(0,"Pc = 0x%08x, Pd = 0x%08x, frame start at 0x%08x.\n",_arParserInfo[u1DecId].eSpdifInfo.u2format,_arParserInfo[u1DecId].eSpdifInfo.u4Size,_arParserInfo[u1DecId].eSpdifInfo.u4Wp);
    LOG(0,"Parser is in status %d.\n",_arParserInfo[u1DecId].eHDMIParserSta);
    LOG(0,"Last transfer ptr = 0x%08x.\n",_arParserInfo[u1DecId].u4LastTransferPtr);
    LOG(0,"No Raw data size  = 0x%08x(0x9080).\n",_arParserInfo[u1DecId].u4NoRawDataSize);
    LOG(0,"_fgDTSCD = %d _fgDTSSW=%d.\n",_arParserInfo[u1DecId]._fgDTSCD,_arParserInfo[u1DecId]._fgDTSSW);
    LOG(0,"Parser transfer frame count is %u.\n",_arParserInfo[u1DecId].u4TFrameCount);
#if 0
    LOG(0," REG_ARP 0x%08x.\n",u4PREG_ARP);
    LOG(0," REG_ARPOFF 0x%08x.\n",u4PREG_ARPOFF);
    LOG(0," REG_A0STA 0x%08x.\n",u4PREG_A0STA);
    LOG(0," REG_A0END 0x%08x.\n",u4PREG_A0END);
    LOG(0," REG_PLENG 0x%08x.\n",u4PREG_PLENG);
    LOG(0," REG_AWP 0x%08x.\n",u4PREG_AWP);
    LOG(0," REG_BITTYPE 0x%08x.\n",u4PREG_BITTYPE);
#endif  
    LOG(0," FULL FLAG %d\n",u4ParserFullFlag);    
}
#endif

static void _LogPts(UINT8 u1PidIdx, UINT8 u1DevId, const DMX_AUDIO_PES_T * prPes)
{
    STC_CLOCK rStc;
    static UINT32 u4PayloadSize = 0;
    static UINT32 u4LastTimeWp = 0;


    u4PayloadSize = (prPes->u4Wp >= u4LastTimeWp) ? (prPes->u4Wp - u4LastTimeWp) :
                    (_arAudDecoder[AUD_DSP0][u1DevId].u4FifoSz + prPes->u4Wp - u4LastTimeWp);
    u4LastTimeWp = prPes->u4Wp;


    if (STC_GetSrc(AUD_DspQryStcID(u1DevId), &rStc) == STC_VALID)  //Audio Description - we only have 1 STC
    {
        if (prPes->u4Pts >= rStc.u4Base)
        {
            //lint -e{613}
            LOG(11, "PidIdx:%u, dev: %u, PTS: 0x%08x, Payload: 0x%08x, size: %d, STC:0x%08x, delta: %d\n",
                    u1PidIdx, u1DevId, (UINT32)(prPes->u4Pts & 0xffffffff),
                    prPes->u4Wp, u4PayloadSize, rStc.u4Base, prPes->u4Pts - rStc.u4Base);
        }
        else
        {
            //lint -e{613}
            LOG(11, "PidIdx:%u, dev: %u, PTS: 0x%08x, Payload: 0x%08x, size: %d, STC:0x%08x, delta: -%d\n",
                    u1PidIdx, u1DevId, (UINT32)(prPes->u4Pts & 0xffffffff),
                    prPes->u4Wp, u4PayloadSize, rStc.u4Base, rStc.u4Base - prPes->u4Pts);
        }
   }

    UNUSED(u4PayloadSize);

}


static BOOL _IsPtsValid(UINT8 u1PidIdx, UINT8 u1DevId, const DMX_AUDIO_PES_T * prPes)
{
    static UINT32 u4PayloadSize = 0;
    static UINT32 u4LastTimeWp = 0;
    static UINT32 u4OldPts = 0;
    static UINT8 u1PtsErrorFlag = 0;
    static UINT8 u1FirstPts = TRUE;
    STC_CLOCK rStc;

    if (prPes != NULL)
    {
        u4PayloadSize = prPes->u4Wp - u4LastTimeWp;
        u4LastTimeWp = prPes->u4Wp;

        //if (STC_GetSrc((u1DevId == AUD_DEC_MAIN) ? STC_SRC_A1 : STC_SRC_A2, &rStc) == STC_VALID)
        if (STC_GetSrc(AUD_DspQryStcID(u1DevId), &rStc) == STC_VALID)  //Audio Description - we only have 1 STC
        {
            if (prPes->u4Pts >= rStc.u4Base)
            {
                //lint -e{613}
                LOG(11, "PidIdx:%u, dev: %u, PTS: 0x%08x, Payload: 0x%08x, size: %d, STC:0x%08x, delta: %d\n",
                    u1PidIdx, u1DevId, (UINT32)(prPes->u4Pts & 0xffffffff),
                    prPes->u4Wp, u4PayloadSize, rStc.u4Base, prPes->u4Pts - rStc.u4Base);
            }
            else
            {
                //lint -e{613}
                LOG(11, "PidIdx:%u, dev: %u, PTS: 0x%08x, Payload: 0x%08x, size: %d, STC:0x%08x, delta: -%d\n",
                    u1PidIdx, u1DevId, (UINT32)(prPes->u4Pts & 0xffffffff),
                    prPes->u4Wp, u4PayloadSize, rStc.u4Base, rStc.u4Base - prPes->u4Pts);
            }
#if 0
            {
                UINT32 u4ReadPtr = 0;
                UINT32 u4WritePtr = 0;
                _AUD_DMX_GetAudPtr(u1DevId, &u4ReadPtr, &u4WritePtr);
                LOG(10, "RP: 0x%08x, WP: 0x%08x, DMX WP: 0x%08x\n", u4ReadPtr, u4WritePtr, prPes->u4Wp);
            }
#endif
            // To suppress warning messages for release version
            UNUSED(u4PayloadSize);

            if ((_arAudDecoder[AUD_DSP0][u1DevId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
                &&(_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eSynMode != AV_SYNC_SLAVE))
            {
                // For multimedia and not Timeshift case, always return TRUE
                return TRUE;
            }
            
            if (AUD_IsSourceFromWFD(u1DevId))
            {
                //Always return TURE for WFD playback
                return TRUE;
            }
                        

            // Check if PTS < STC
            #ifndef CC_USE_DDI
            if(prPes->u4Pts < rStc.u4Base)
            #else
            if ((prPes->u4Pts < rStc.u4Base) && ((_arAudDecoder[AUD_DSP0][u1DevId].eStreamFrom != AUD_STREAM_FROM_DIGITAL_TUNER) || 
                (_arAudDecoder[AUD_DSP0][u1DevId].eSynMode != AV_SYNC_AUDIO_MASTER)))
            #endif
            {
                LOG(11, "PTS error: invalid PTS (PTS - STC < 0)\n");
                return FALSE;
            }

            // Check if sudden different PTS occurs
            if (u1FirstPts)
            {
                u4OldPts = prPes->u4Pts;
                u1FirstPts = FALSE;
            }
            else if (((u4OldPts > prPes->u4Pts) && (u4OldPts - prPes->u4Pts > 1000000)) ||
                ((u4OldPts < prPes->u4Pts) && (prPes->u4Pts -u4OldPts > 1000000)))
            {
                if (u1PtsErrorFlag == 0)
                {
                    u1PtsErrorFlag = 1;
                    LOG(9, "PTS error: difference too large (%d -> %d)\n", u4OldPts, prPes->u4Pts);
                    return FALSE;
                }
                else
                {
                    u1PtsErrorFlag = 0;
                    u4OldPts = prPes->u4Pts;
                }
            }
            else
            {
                u1PtsErrorFlag = 0;
                u4OldPts = prPes->u4Pts;
            }

            return TRUE;
        }
    }

    return FALSE;
}

static void _LoadAdspCode(UINT8 u1DspId, UINT8 u1DecId, AUD_FMT_T eStreamFormat)
{
    UINT32 u4StreamFormat = AUD_PINK_DEC1;
    static AUD_FMT_T geStreamFormat[AUD_DSP_NUM][AUD_DEC_NUM];

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    UNUSED(_aszCodeType);
    if(geStreamFormat[u1DspId][u1DecId]!=eStreamFormat)
    {
        geStreamFormat[u1DspId][u1DecId]=eStreamFormat;
        DRVCUST_SendAudEvent(E_CUST_AUD_FORMAT_CHANGE, u1DecId);
    }
    if (u1DecId == AUD_DEC_MAIN)  // first decoder
    {
        switch(eStreamFormat)
        {
            case AUD_FMT_MPEG:
                u4StreamFormat = AUD_MPEG_DEC1;
                break;
            case AUD_FMT_AC3:
                u4StreamFormat = AUD_AC3_DEC1;
                break;
            case AUD_FMT_AAC:
                u4StreamFormat = AUD_AAC_DEC1;
                break;
            case AUD_FMT_MP3:
                u4StreamFormat = AUD_MP3_DEC1;
                break;
          case AUD_FMT_DTS:
                u4StreamFormat = AUD_DTS_DEC1;
                break;
            case AUD_FMT_WMA:
                u4StreamFormat = AUD_WMA_DEC1;
                break;
            case AUD_FMT_PCM:
                u4StreamFormat = AUD_PCM_DEC1;
                break;
            case AUD_FMT_MTS:
                u4StreamFormat = AUD_MTS_DEC1;
                break;
            case AUD_FMT_A2:
                u4StreamFormat = AUD_A2DEC_DEC1;
                break;
            case AUD_FMT_PAL:
                u4StreamFormat = AUD_PAL_DEC1;
                break;
            case AUD_FMT_FMFM:
                u4StreamFormat = AUD_FMFM_DEC1;
                break;
            case AUD_FMT_DETECTOR:
                u4StreamFormat = AUD_DETECTOR_DEC1;
                break;
            case AUD_FMT_MINER:
                u4StreamFormat = AUD_MINER_DEC1;
                break;
            case AUD_FMT_UNKNOWN:
                u4StreamFormat = AUD_PINK_DEC1;
                break;
            case AUD_FMT_LPCM:
                u4StreamFormat = AUD_LPCM_DEC1;
                break;
            case AUD_FMT_FMRDO:
                u4StreamFormat = AUD_FMRDO_DEC1;
                break;
            case AUD_FMT_FMRDO_DET:
                u4StreamFormat = AUD_FMRDODET_DEC1;
                break;
            case AUD_FMT_SBCDEC:
                u4StreamFormat = AUD_SBC_DEC1;
                break;
            case AUD_FMT_G711DEC:
                u4StreamFormat = AUD_G711_DEC1;
                break;
            case AUD_FMT_G711ENC:
                u4StreamFormat = AUD_G711_ENC1;
                break;
            case AUD_FMT_DRA:
                u4StreamFormat = AUD_DRA_DEC1;
                break;
            case AUD_FMT_COOK:
                u4StreamFormat = AUD_COOK_DEC1;
                break;
            case AUD_FMT_G729DEC:
                u4StreamFormat = AUD_G729_DEC1;
                break;
    #if 1///def CC_VORBIS_SUPPORT
            case AUD_FMT_VORBISDEC:
                u4StreamFormat = AUD_VORBIS_DEC1;
                break;
    #endif
            case AUD_FMT_WMAPRO:
                u4StreamFormat = AUD_WMAPRO_DEC1;
                break;
            case AUD_FMT_AMR:
                u4StreamFormat = AUD_AMR_DEC1;
                break;
            case AUD_FMT_AWB:
                u4StreamFormat = AUD_AWB_DEC1;
                break;
    #if 1 //CC_APE_SUPPORT
            // APE decoder
            case AUD_FMT_APE:
                u4StreamFormat = AUD_APE_DEC1;
                break;
    #endif
            case AUD_FMT_FLAC://paul_flac, == 39
                u4StreamFormat = AUD_FLAC_DEC1;// == 0x1C
                break;
            case AUD_FMT_G726:
                u4StreamFormat = AUD_G726_DEC1;// == 0x1D
                break;
            default:
                u4StreamFormat = AUD_PINK_DEC1;
                break;
        }
        // load ADSP decoder's code while decoder is stopped
        DSP_ResetDec(u1DspId, u1DecId);
        LOG( 7,"LoadAdspCode(%d)(0) = %s\n", u1DspId, GET_PCHAR_ARRAY_AT(_aszCodeType,eStreamFormat));
        DSP_LoadRamCode(u1DspId, u4StreamFormat);
    }
    else if (u1DecId == AUD_DEC_AUX)  // second decoder
    {
        switch(eStreamFormat)
        {
            case AUD_FMT_MPEG:
                u4StreamFormat = AUD_MPEG_DEC2;
                break;
            case AUD_FMT_AC3:
                u4StreamFormat = AUD_AC3_DEC2;
                break;
            case AUD_FMT_AAC:
                u4StreamFormat = AUD_AAC_DEC2;
                break;
            case AUD_FMT_PCM:
                u4StreamFormat = AUD_PCM_DEC2;
                break;
            case AUD_FMT_DTS:
                u4StreamFormat = AUD_DTS_DEC2;
                break;
            case AUD_FMT_MTS:
                u4StreamFormat = AUD_MTS_DEC2;
                break;
            case AUD_FMT_A2:
                u4StreamFormat = AUD_A2DEC_DEC2;
                break;
            case AUD_FMT_PAL:
                u4StreamFormat = AUD_PAL_DEC2;
                break;
            case AUD_FMT_FMFM:
                u4StreamFormat = AUD_FMFM_DEC2;
                break;
            case AUD_FMT_DETECTOR:
                u4StreamFormat = AUD_DETECTOR_DEC2;
                break;
            case AUD_FMT_MINER:
                u4StreamFormat = AUD_MINER_DEC2;
                break;
            case AUD_FMT_MP3:
                u4StreamFormat = AUD_MP3_DEC2;
                break;
            case AUD_FMT_LPCM:
                u4StreamFormat = AUD_LPCM_DEC2;
                break;
            case AUD_FMT_DRA:
                u4StreamFormat = AUD_DRA_DEC2;
                break;
            case AUD_FMT_G729DEC:
                u4StreamFormat = AUD_G729_DEC2;
                break;
    #if 1///def CC_VORBIS_SUPPORT
            case AUD_FMT_VORBISDEC:
                u4StreamFormat = AUD_VORBIS_DEC2;
                break;
    #endif
            case AUD_FMT_WMAPRO:
                u4StreamFormat = AUD_WMAPRO_DEC2;
                break;
            case AUD_FMT_COOK:
                u4StreamFormat = AUD_COOK_DEC2;
                break;
            case AUD_FMT_AMR:
                u4StreamFormat = AUD_AMR_DEC2;
                break;
            case AUD_FMT_AWB:
                u4StreamFormat = AUD_AWB_DEC2;
                break;
            case AUD_FMT_WMA:
                u4StreamFormat = AUD_WMA_DEC2;
                break;
            case AUD_FMT_FLAC://paul_flac_2
                u4StreamFormat = AUD_FLAC_DEC2;
                break;
            case AUD_FMT_G726:
                u4StreamFormat = AUD_G726_DEC2;
                break;
            default:
                LOG(1, "Invalid switch in _LoadAdspCode\n");
                break;
        }
        // load ADSP decoder's code while decoder is stopped
        DSP_ResetDec(u1DspId, u1DecId);
        LOG( 7,"LoadAdspCode(%d)(1) = %s\n", u1DspId, GET_PCHAR_ARRAY_AT(_aszCodeType,eStreamFormat));
        DSP_LoadRamCode(u1DspId, u4StreamFormat);
    }
#ifdef CC_MT5391_AUD_3_DECODER
    else if (u1DecId == AUD_DEC_THIRD)
    {
        switch(eStreamFormat)
        {
            case AUD_FMT_MPEG:
                u4StreamFormat = AUD_MPEG_DEC3;
                break;
            case AUD_FMT_PCM:
                u4StreamFormat = AUD_PCM_DEC3;
                break;
            case AUD_FMT_AAC:
                u4StreamFormat = AUD_AAC_DEC3;
                break;
            case AUD_FMT_AC3:
                u4StreamFormat = AUD_AC3_DEC3;
                break;
            case AUD_FMT_LPCM:
                u4StreamFormat = AUD_LPCM_DEC3; //lpcm_dec3
                break;
            case AUD_FMT_DTS:
                u4StreamFormat = AUD_DTS_DEC3;
                break;
            case AUD_FMT_AMR:
                u4StreamFormat = AUD_AMR_DEC3;
                break;
            case AUD_FMT_AWB:
                u4StreamFormat = AUD_AWB_DEC3;
                break;
            case AUD_FMT_MP3:
                u4StreamFormat = AUD_MP3_DEC3;
                break;
            case AUD_FMT_MTS:
                u4StreamFormat = AUD_MTS_DEC3;
                break;
            case AUD_FMT_A2:
                u4StreamFormat = AUD_A2DEC_DEC3;
                break;
            case AUD_FMT_PAL:
                u4StreamFormat = AUD_PAL_DEC3;
                break;
            case AUD_FMT_DETECTOR:
                u4StreamFormat = AUD_DETECTOR_DEC3;
                break;
    #if 1///def CC_VORBIS_SUPPORT
            case AUD_FMT_VORBISDEC:
                u4StreamFormat = AUD_VORBIS_DEC3;
                break;
    #endif
            case AUD_FMT_WMAPRO:
                u4StreamFormat = AUD_WMAPRO_DEC3;
                break;
            case AUD_FMT_WMA:
                u4StreamFormat = AUD_WMA_DEC3;
                break;
            case AUD_FMT_COOK:
                u4StreamFormat = AUD_COOK_DEC3;
                break;
            case AUD_FMT_FLAC://paul_flac_3
                u4StreamFormat = AUD_FLAC_DEC3;
                break;
            case AUD_FMT_G726:
                u4StreamFormat = AUD_G726_DEC3;
                break;
            case AUD_FMT_DRA:
                u4StreamFormat = AUD_DRA_DEC3;
                break;
            default:
                LOG(1, "Invalid switch in _LoadAdspCode\n");
                break;
        }
        // load ADSP decoder's code while decoder is stopped
        DSP_ResetDec(u1DspId, u1DecId);
        LOG( 7,"LoadAdspCode(%d)(2) = %s\n", u1DspId, GET_PCHAR_ARRAY_AT(_aszCodeType,eStreamFormat));
        DSP_LoadRamCode(u1DspId, u4StreamFormat);
    }
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    else if (u1DecId == AUD_DEC_4TH)
    {
        switch(eStreamFormat)
        {
            case AUD_FMT_SBCENC:
                u4StreamFormat = AUD_SBCENC_DEC4;
                break;
            case AUD_FMT_PCM:
                u4StreamFormat = AUD_PCM_DEC4;
                break;
    #ifdef MP3ENC_SUPPORT
            case AUD_FMT_MP3ENC:
                u4StreamFormat = AUD_MP3ENC_DEC4;
                break;
    #endif
            default:
                LOG(1, "Invalid switch in _LoadAdspCode\n");
                break;
        }
        // load ADSP decoder's code while decoder is stopped
        DSP_ResetDec(u1DspId, u1DecId);
        LOG( 7,"LoadAdspCode(%d)(3) = %s\n", u1DspId, GET_PCHAR_ARRAY_AT(_aszCodeType,eStreamFormat));
        DSP_LoadRamCode(u1DspId, u4StreamFormat);
    }
#endif
}

/*
static UINT8 _PidIdxToDecId(UINT8 u1PidIdx)
{
    INT32 i;

    for (i=0, i< AUD_DEC_NUM; i++)
    {
        if (_arAudDecoder[AUD_DEC_MAIN].u1DmxPidIdx == u1PidIdx)
        {
            break;
        }
    }

    UNUSED(u1PidIdx);
    return AUD_DEC_MAIN;
}
*/

static UINT8 _DecIdToPidIdx(UINT8 u1DecId)
{
    return _arAudDecoder[AUD_DSP0][u1DecId].u1DmxPidIdx;
}

static BOOL _DmxPidIdxValid(UINT8 u1DecId)
{
    return (_arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount > 0) ? TRUE : FALSE;
}

#ifndef CC_AUD_DDI
static BOOL _IsVideoStable(void)
{
#ifdef CC_FPGA
    return TRUE;
#else
    //return(fgApiVideoStable(_u1VideoFrameFlag));
    return (!(BOOL)_u4DrvVideoGetMute((UINT32)_u1VideoFrameFlag));
#endif
}
#endif

#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521
static BOOL _AvSyncControl(UINT8 u1DecId)
{
    BOOL fgRet = FALSE;

    // Only DTV & ATV need AV synchronization
    if (((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||(_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) ) &&
        (AUD_GetAvSynMode(u1DecId) != AV_SYNC_FREE_RUN) &&
        (!_AudDmxDtvInitPlaybackFifoFull(u1DecId)))
    {
        fgRet = TRUE;
    }
    else if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN) &&
        (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN_2) &&
        (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN_3) &&
        (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) &&
        (!_afgAtvVideoDisplayFlag[u1DecId]) &&
        (!_IsVideoStable()) &&
        (_arAudDecoder[u1DspId][u1DecId].eDecFormat !=  AUD_FMT_DETECTOR))
    {
        fgRet = TRUE;
    }
#ifdef CC_ENABLE_AOMX
    else if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_GST) && (AUD_GetAvSynMode(u1DecId) == AV_SYNC_AUDIO_MASTER))
    {
        fgRet = TRUE;
    }
#endif

    return fgRet;
}
#else
static BOOL _AvSyncControl(UINT8 u1DspId, UINT8 u1DecId)
{
    BOOL fgRet = FALSE;
    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_DSP_ID_VALIDATE(u1DspId);

    if(u1DspId == AUD_DSP1)
    {
        return FALSE;
    }    

    // Only DTV & ATV need AV synchronization
    if (((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||(_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)) &&
        (AUD_GetAvSynMode(u1DecId) != AV_SYNC_FREE_RUN) &&
        (!_AudDmxDtvInitPlaybackFifoFull(u1DecId)))
    {
        fgRet = TRUE;
    }
    else if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
            (AUD_GetAvSynMode(u1DecId) != AV_SYNC_FREE_RUN) &&
            (!_AudDmxDtvInitPlaybackFifoFull(u1DecId)))
    {
        fgRet = TRUE;
    }
    #ifndef CC_AUD_DDI
    else if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) &&
             (!_afgAtvVideoDisplayFlag[u1DecId]) &&
             (!_IsVideoStable()) &&
             ((_arAudDecoder[u1DspId][u1DecId].eDecFormat !=  AUD_FMT_DETECTOR)&&(_arAudDecoder[u1DspId][u1DecId].eDecFormat !=  AUD_FMT_FMRDO)
                 &&(_arAudDecoder[u1DspId][u1DecId].eDecFormat !=  AUD_FMT_FMRDO_DET))
             )
    {
        fgRet = TRUE;
    }
    #endif
    /*else if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN) &&
             (!_afgAtvVideoDisplayFlag[u1DecId]) &&
#ifdef CC_AUD_SKYPE_SUPPORT
             (u1DecId == AUD_DEC_MAIN) &&
#endif
             (!_IsVideoStable()))
    {
        fgRet = TRUE;
    }*/
#ifdef CC_ENABLE_AOMX
    else if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_GST) && (AUD_GetAvSynMode(u1DecId) == AV_SYNC_AUDIO_MASTER))
    {
        fgRet = TRUE;
    }
#endif

    return fgRet;
}
#endif

static void _ChkAudioOnlyControl(UINT8 u1DecId, BOOL fgReset)
{
    //Andrew Wen 2007/6/22 Audio only program set sync mode to free run.
    static UINT32 u4OldStcBase = 0;
    BOOL fgFreeRun = FALSE;
    STC_CLOCK rStc;
    static UINT32 u4LastRetrieveNs = 0;

    //if (STC_GetSrc((u1DecId == AUD_DEC_MAIN) ? STC_SRC_A1 : STC_SRC_A2, &rStc) != STC_VALID)
    if (STC_GetSrc(AUD_DspQryStcID(u1DecId), &rStc) != STC_VALID) //Audio Description
    {
        if (fgReset)
        {
            u4OldStcBase = 0;
        }
        return;
    }

    _prMPV = &(prDbase->rMpv[0]);

    if (fgReset)
    {
        u4OldStcBase = rStc.u4Base;

        //AUD_SetAvSynMode(u1DecId, AV_SYNC_SLAVE);
        if (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode == AV_SYNC_FREE_RUN)
        {
            LOG(7,"Dec(%d) Middleware sets start sync mode = Free run, u4RetrieveNs=%x, u4DecOkNs=%x\n", u1DecId, _prMPV->u4RetrieveNs, _prMPV->u4DecOkNs);
        }
        else if (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode == AV_SYNC_SLAVE)
        {
            LOG(7,"Dec(%d) Middleware sets start sync mode = AV sync, u4RetrieveNs=%x, u4DecOkNs=%x\n", u1DecId, _prMPV->u4RetrieveNs, _prMPV->u4DecOkNs);
        }
        else
        {
            LOG(7,"Dec(%d) Middleware sets start sync mode = Audio master, u4RetrieveNs=%x, u4DecOkNs=%x\n", u1DecId, _prMPV->u4RetrieveNs, _prMPV->u4DecOkNs);
        }

        u4LastRetrieveNs = _prMPV[0].u4RetrieveNs;
    }
    else 
    #ifdef CC_USE_DDI
    if ((_arAudDecoder[u1DspId][u1DecId].eSynMode != AV_SYNC_AUDIO_MASTER) || 
        (_arAudDecoder[u1DspId][u1DecId].eStreamFrom != AUD_STREAM_FROM_DIGITAL_TUNER))
    #endif
    {
        if ((rStc.u4Base - u4OldStcBase) > 90000)
        {
            /*
            LOG(7,"# STC=%x\n", rStc.u4Base);
            LOG(7,"  u4RetrieveNs=%x, u4DecOkNs=%x, mode =%d\n", _prMPV->u4RetrieveNs, _prMPV->u4DecOkNs, _arAudDecoder[u1DspId][u1DecId].eSynMode);
            LOG(7,"  pts_stc = %x, %x\n",u4ReadDspSram(0x156a), u4ReadDspSram(0x156b));
            */
            u4OldStcBase = rStc.u4Base;
            if (((u4LastRetrieveNs - _prMPV[0].u4RetrieveNs) ==0) && (_prMPV[0].u4DecOkNs == 0))
            {
                fgFreeRun = TRUE;
            }
            u4LastRetrieveNs = _prMPV[0].u4RetrieveNs;
#ifndef CC_AUD_DDI
            if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
            {
                if ((fgFreeRun) && (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode != AV_SYNC_FREE_RUN)
                                    && (_afgIssuePlayComToDsp[AUD_DSP0][u1DecId]))
                {
                    LOG(7,"_ChkAudioOnlyControl Set Sync to Free Run\n");
                    AUD_SetAvSynMode(u1DecId, AV_SYNC_FREE_RUN);
#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
                    if (u1DecId <= AUD_DEC_THIRD)
#endif
                        VERIFY(AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_UNMUTE_TIME, 0));
#endif
                    // Clear PTS in DSP
                    AUD_ClearPtsInISR(AUD_DSP0, u1DecId);
                }
                else if ((!fgFreeRun) && (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode == AV_SYNC_FREE_RUN))
                {
                    LOG(7,"_ChkAudioOnlyControl Set Sync to Slave Mode\n");
                    AUD_SetAvSynMode(u1DecId, AV_SYNC_SLAVE);
                }
            }
#endif
            if (fgFreeRun == AUD_DspIsWithVideo(u1DecId))
            {
                AUD_DspSetWithVideo(u1DecId, (!fgFreeRun));
            }
        }
    }
}

#ifdef CC_MT5881 // Remy spdif in
static SAMPLE_FREQ_T _GetSpdifinSmpRate(void)
{
    UINT8 u1SpdifinAudSf;
    SAMPLE_FREQ_T eAudDrvSampleFreq = FS_32K;

    // Remy spdif in sample rate refer to risc register 0x56ec(spdifin freq spdifin_freq status)
    u1SpdifinAudSf = (AUD_READ32(REG_SPDIFIN_ROUGH_FREQ) & SPDIF_RX_FREQ_IDX) >> 28;
    LOG(7, "Spdif in detect sample rate index = %d\n", u1SpdifinAudSf);
    switch (u1SpdifinAudSf)
    {
        case 0x1:
            eAudDrvSampleFreq = FS_32K;
            break;
        case 0x2:
            eAudDrvSampleFreq = FS_44K;
            break;
        case 0x3:
            eAudDrvSampleFreq = FS_48K;
            break;
        case 0x4:
            eAudDrvSampleFreq = FS_64K;
            break;
        case 0x5:
            eAudDrvSampleFreq = FS_88K;
            break;
        case 0x6:
            eAudDrvSampleFreq = FS_96K;
            break;
        case 0x9:
            eAudDrvSampleFreq = FS_176K;
            break;
        case 0xA:
            eAudDrvSampleFreq = FS_192K;
            break;
        default:
            break;
    }
    return eAudDrvSampleFreq;
}
#endif
static SAMPLE_FREQ_T _GetHdmiSmpRate(void)
{
    UINT8 u1HdmiAudSf;
    SAMPLE_FREQ_T eAudDrvSampleFreq;

    u1HdmiAudSf = (u1HDMIGetAudioSamplingFreq() & 0x0F);

    if (u1HdmiAudSf == 0)
    {
        eAudDrvSampleFreq = FS_44K;
    }
    else if (u1HdmiAudSf == 0x3)
    {
        eAudDrvSampleFreq = FS_32K;
    }
    else if (u1HdmiAudSf == 0x2)
    {
        eAudDrvSampleFreq = FS_48K;
    }
    else if (u1HdmiAudSf == 0x8)
    {
        eAudDrvSampleFreq = FS_88K;
    }
    else if (u1HdmiAudSf == 0xa)
    {
        eAudDrvSampleFreq = FS_96K;
    }
    else if (u1HdmiAudSf == 0xc)
    {
        eAudDrvSampleFreq = FS_176K;
    }
    else if (u1HdmiAudSf == 0xe)
    {
        eAudDrvSampleFreq = FS_192K;
    }
    else
    {   // 0x0 or default
        eAudDrvSampleFreq = FS_44K;
        LOG(3, "GetHdmiSmpRate (not indicate)\n");
    }

    return eAudDrvSampleFreq;
}

#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
/* Patch for wrong HDMI content from player
    Do not decode DDP and skip CRC error mute  if HDMI source to avoid this kind of HDMI content.
*/
static UINT32 _gu4HDMIcase = 0;

void _AudSetHDMICase (UINT32 u4HDMI)
{
    _gu4HDMIcase = u4HDMI;
}

UINT32 _AudGetHDMICase (void)
{
    return _gu4HDMIcase;
}
#endif


static SAMPLE_FREQ_T _eHdmiSmpRate = FS_48K;
#ifdef CC_HDMI_PCM_MULT_CHANNEL
static UINT8 _u1HdmiChNum = 2;
#endif
#ifdef  CC_MT5881
static SAMPLE_FREQ_T _eSpdifinSmpRate = FS_48K;
#endif
#ifdef CC_AUD_I2S_IN
static SAMPLE_FREQ_T _eI2SinSmpRate = FS_48K;
#endif

static void _DecoderInitSetup(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    UINT8 u1HdmiAudSf;
    SAMPLE_FREQ_T eAudDrvSampleFreq;
#ifdef CC_MT5881
    UINT8 u1SpdifinAudSf;
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
    if (u1DecId == AUD_DEC_4TH)
    {
        if (eDecFmt == AUD_FMT_SBCENC)
        {
            DSP_Set4thDecMode(TRUE);
        }
        else
        {
            DSP_Set4thDecMode(FALSE);
        }
    }
#endif

#ifdef CC_MT5391_AUD_3_DECODER
    if (u1DecId == AUD_DEC_MAIN)
    {
        DSP_SetDualDecMode(FALSE);
        AUD_ClkCfgDualDecMode(FALSE); //0714
#ifdef CC_AUD_LINEIN_USE_DEC3
        DSP_SetTriDecOnlyMode(FALSE);
#endif
    }
#else
    {
        // In Single Decoder mode, clear all mutliple decoder related info.
        DSP_SetDualDecMode(FALSE);
        AUD_ClkCfgDualDecMode(FALSE);
    }
#endif
    
    if (u1DecId == AUD_DEC_MAIN)
    {
        //For slave mode, ASRC should use tracing mode.
        if((eStreamFrom == AUD_STREAM_FROM_HDMI)||(eStreamFrom == AUD_STREAM_FROM_SPDIF))
        {
            DSP_SetAsrcTrackingMode(TRUE);
        }
        else
        {
            DSP_SetAsrcTrackingMode(FALSE);
        }
    }
    
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
    /* Patch for wrong HDMI content from player
        Do not decode DDP and skip CRC error mute if HDMI source to avoid this kind of HDMI content.
    */
    if (u1DecId == AUD_DEC_MAIN && eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        _AudSetHDMICase (1); // set HDMI work around
    }
    else
    {
        _AudSetHDMICase (0);
    }
#else
#ifndef CC_DUAL_AUD_DEC_SUPPORT
    if ((eStreamFrom == AUD_STREAM_FROM_HDMI) &&
        (_arParserInfo[u1DecId].eSpdifInfo.u2format == 0x15) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_AC3))
#else    
    if ((eStreamFrom == AUD_STREAM_FROM_HDMI) &&
        (_arParserInfo[u1DecId].eSpdifInfo.u2format == 0x15) && (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_AC3))
#endif
    {
        // HDMI, E-AC-3
        DSP_SetASRCNumeratorQuarter (TRUE);
    }
    else
    {
        DSP_SetASRCNumeratorQuarter (FALSE);
    }
#endif

    if (eDecFmt == AUD_FMT_PCM)
    {
    #ifdef CC_AUD_PCM_LINE_IN_LPF
        DSP_SetPCMLineInLPF(FALSE);
    #endif
        if ((eStreamFrom == AUD_STREAM_FROM_MEMORY) ||
            (eStreamFrom == AUD_STREAM_FROM_FEEDER))
        {
            DSP_SetPcmType(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].rPcmSetting.ePcmDataInfo);
            DSP_SetPcmFs(u1DspId, u1DecId, FS_48K); // default set as 48K
        }
        else if (eStreamFrom == AUD_STREAM_FROM_HDMI)
        {
            u1HdmiAudSf = (u1HDMIGetAudioSamplingFreq() & 0x0F);

            if (u1HdmiAudSf == 0)
            {
                eAudDrvSampleFreq = FS_44K;
            }
            if (u1HdmiAudSf == 0x3)
            {
                eAudDrvSampleFreq = FS_32K;
            }
            else if (u1HdmiAudSf == 0x2)
            {
                eAudDrvSampleFreq = FS_48K;
            }
            else if (u1HdmiAudSf == 0x8)
            {
                eAudDrvSampleFreq = FS_88K;
            }
            else if (u1HdmiAudSf == 0xa)
            {
                eAudDrvSampleFreq = FS_96K;
            }
            else if (u1HdmiAudSf == 0xc)
            {
                eAudDrvSampleFreq = FS_176K;
            }
            else if (u1HdmiAudSf == 0xe)
            {
                eAudDrvSampleFreq = FS_192K;
            }
            else
            {   // 0x0 or default
                eAudDrvSampleFreq = FS_44K;
            }

#ifdef USE_16BIT_ALIGNMENT
            DSP_SetPcmType(u1DspId, u1DecId, PCM_16_BITS_BIG_ENDIAN);
#else
            DSP_SetPcmType(u1DspId, u1DecId, PCM_24_BITS);
#endif
            DSP_SetPcmFs(u1DspId, u1DecId, eAudDrvSampleFreq);

#ifdef CC_HDMI_PCM_MULT_CHANNEL
            _u1HdmiChNum = API_HDMI_Get_Aud_ChannelNum()+1;
            DSP_SetPcmChNum(u1DecId, _u1HdmiChNum);
            AUD_LineInChNumSetting(u1DecId, _u1HdmiChNum);
            LOG(5, " ------------------ HDMI Decoder init Channel Number (%d) -----------\n", _u1HdmiChNum);
#endif
            DSP_SetPcmFs(u1DspId, u1DecId, eAudDrvSampleFreq);
            _eHdmiSmpRate = eAudDrvSampleFreq;
            // Setup sampling rate to apll and dac

            LOG(5, " ------------------ Decoder init set sampling rate (%d) -----------\n", _eHdmiSmpRate);
            AUD_SetSampleFreq(AUD_DEC_MAIN, eAudDrvSampleFreq);

        }
#ifdef CC_MT5881
        else if (eStreamFrom == AUD_STREAM_FROM_SPDIF)
        {
            u1SpdifinAudSf = (AUD_READ32(REG_SPDIFIN_ROUGH_FREQ) & SPDIF_RX_FREQ_IDX) >> 28;
            switch (u1SpdifinAudSf)
            {
                case 0x1:
                    eAudDrvSampleFreq = FS_32K;
                    break;
                case 0x2:
                    eAudDrvSampleFreq = FS_44K;
                    break;
                case 0x3:
                    eAudDrvSampleFreq = FS_48K;
                    break;
                case 0x4:
                    eAudDrvSampleFreq = FS_64K;
                    break;
                case 0x5:
                    eAudDrvSampleFreq = FS_88K;
                    break;
                case 0x6:
                    eAudDrvSampleFreq = FS_96K;
                    break;
                case 0x9:
                    eAudDrvSampleFreq = FS_176K;
                    break;
                case 0xA:
                    eAudDrvSampleFreq = FS_192K;
                    break;
                default:
                    eAudDrvSampleFreq = FS_48K;
                    break;
            }
#ifdef USE_16BIT_ALIGNMENT
            DSP_SetPcmType(u1DecId, PCM_16_BITS_BIG_ENDIAN);
#else
            DSP_SetPcmType(u1DecId, PCM_24_BITS);
#endif
            DSP_SetPcmFs(u1DecId, eAudDrvSampleFreq);

            _eSpdifinSmpRate = eAudDrvSampleFreq;
            // Setup sampling rate to apll and dac
            LOG(5, " ------------------ Decoder init set sampling rate (%d) -----------\n", _eSpdifinSmpRate);
            AUD_SetSampleFreq(AUD_DEC_MAIN, eAudDrvSampleFreq);
        }
#endif
    
        else if ((eStreamFrom == AUD_STREAM_FROM_LINE_IN)|| (eStreamFrom == AUD_STREAM_FROM_LINE_IN_2)|| (eStreamFrom == AUD_STREAM_FROM_LINE_IN_3))
        {
#ifdef USE_16BIT_ALIGNMENT
            DSP_SetPcmType(u1DspId, u1DecId, PCM_16_BITS_BIG_ENDIAN);
#else
            DSP_SetPcmType(u1DspId, u1DecId, PCM_24_BITS);
#endif
        #ifdef CC_FPGA
            DSP_SetPcmFs(u1DspId, u1DecId, FS_48K); // default set as 48K for FPGA
        #else 
            //DSP_SetPcmFs(u1DspId, u1DecId, FS_48K); // recover back to 48K for initial stage
        #if defined(CC_MT5890)
            DSP_SetPcmFs(u1DspId, u1DecId, FS_52K); //gallen 20140428, use 52k for Oryx internal ADC 46.875k usage
        #else
            DSP_SetPcmFs(u1DspId, u1DecId, FS_48K); // recover back to 48K for initial stage
        #endif
        #endif
        #ifdef CC_AUD_PCM_LINE_IN_LPF
            DSP_SetPCMLineInLPF(TRUE);
        #endif
        }
#ifdef CC_AUD_I2S_IN
        else if (eStreamFrom == AUD_STREAM_FROM_I2S)
        {
#ifdef USE_16BIT_ALIGNMENT
            DSP_SetPcmType(u1DspId, u1DecId, PCM_16_BITS_BIG_ENDIAN);
#else
            DSP_SetPcmType(u1DspId, u1DecId, PCM_24_BITS);
#endif
            ///DSP_SetPcmFs(u1DecId, FS_52K); // default set as 52K
            ///DSP_SetPcmFs(u1DecId, FS_62K); // default set as 62K
            DSP_SetPcmFs(u1DspId, u1DecId, FS_48K); // default set as 48K for FPGA
            DSP_SetAsrcTrackingMode(TRUE);
        }
#endif
#ifdef CC_MT5391_AUD_3_DECODER
        else if (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
        {
            DSP_SetPcmType(u1DspId, u1DecId, PCM_24_BITS);
            DSP_SetPcmFs(u1DspId, u1DecId, FS_48K);
            DSP_SetDualDecMode(TRUE);
            AUD_ClkCfgDualDecMode(TRUE);//0714
        }
        else if (eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
        {
            DSP_SetPcmType(u1DspId, u1DecId, PCM_24_BITS);
            DSP_SetPcmFs(u1DspId, u1DecId, FS_32K);
            DSP_SetDualDecMode(TRUE);
            AUD_ClkCfgDualDecMode(TRUE);//0714
        }
#endif
        else if ((eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
             #ifdef CC_ENABLE_AOMX
                 ||(eStreamFrom == AUD_STREAM_FROM_GST)
             #endif
                )
        {
            //DSP_SetPcmType(u1DecId, PCM_16_BITS_LITTLE_ENDIAN);
            if(_arAudDecoder[u1DspId][u1DecId].eMultimediaMode == AUD_MM_MODE_GAME)
            {
                DSP_SetPcmType(u1DspId, u1DecId, PCM_16_BITS_LITTLE_ENDIAN);
            }
            else
            {
            DSP_SetPcmType(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].rPcmSetting.ePcmDataInfo);
            }
#ifndef CC_AUD_SKYPE_SUPPORT
            if(_arAudDecoder[u1DspId][u1DecId].eMultimediaMode != AUD_MM_MODE_GAME)
            {
                DSP_SetPcmFs(u1DspId, u1DecId, FS_48K); // default set as 48K
            }
#endif
        }
        else // Other fix bit number
        {
#ifdef USE_16BIT_ALIGNMENT
            DSP_SetPcmType(u1DspId, u1DecId, PCM_16_BITS_BIG_ENDIAN);
#else
            DSP_SetPcmType(u1DspId, u1DecId, PCM_24_BITS);
#endif
            DSP_SetPcmFs(u1DspId, u1DecId, FS_48K); // default set as 48K
        }

    }
    else  if (eDecFmt == AUD_FMT_G711DEC)
    {
 #ifdef DSP_G711_A_LAW
            DSP_SetPcmType(u1DspId, u1DecId, PCM_A_LAW_LITTLE_ENDIAN);
 #else
            DSP_SetPcmType(u1DspId, u1DecId, PCM_U_LAW_LITTLE_ENDIAN);
 #endif
            DSP_SetPcmFs(u1DspId, u1DecId, FS_32K);
    }
    else  if (eDecFmt == AUD_FMT_G711ENC)
    {
 #ifdef DSP_G711_A_LAW
            DSP_SetPcmType(u1DspId, u1DecId, PCM_A_LAW_LITTLE_ENDIAN);
 #else
            DSP_SetPcmType(u1DspId, u1DecId, PCM_U_LAW_LITTLE_ENDIAN);
 #endif
    }

#ifdef CC_MT5391_AUD_3_DECODER
    if ( (eDecFmt == AUD_FMT_MPEG) ||
          (eDecFmt == AUD_FMT_AAC) ||
#ifdef CC_AUD_SKYPE_SUPPORT
           (eDecFmt == AUD_FMT_PCM) ||
#endif
          (eDecFmt == AUD_FMT_AC3) )
    {
        if (u1DecId == AUD_DEC_THIRD)
        {
            DSP_SetTriDecMode(TRUE);
        }
    }
#endif
}

#if 0
// *********************************************************************
// Function : static BOOL _AudDetectFrameIncrease(void)
// Description : Only check Dec1
// Parameter : none
// Return    : Frame increasing (1) or Stopped (0)
// *********************************************************************
static BOOL _AudDetectFrameIncrease(void)
{
    BOOL fgRet = TRUE;
    static UINT32 _u4PrevCnt = 0xFFFFFFFF;
    UINT32 _u4CurCnt;

    _u4CurCnt = u4ReadDspSram(DSP_SRAM_DECODE_FRAME_COUNT);

    if(_u4PrevCnt != _u4CurCnt)
    {
        _u4PrevCnt = _u4CurCnt;
    }
    else
    {
        fgRet = FALSE;
    }

    return fgRet;
}
#endif

UINT32 _u4DspFlowControlCmdTable[4][AUD_DEC_MAX] =
{
    {DSP_PLAY, DSP_PLAY_DEC2, DSP_PLAY_DEC3, DSP_PLAY_DEC4},
    {DSP_STOP, DSP_STOP_DEC2, DSP_STOP_DEC3, DSP_STOP_DEC4},
    {DSP_PAUSE, DSP_PAUSE_DEC2, DSP_PAUSE_DEC3, DSP_PAUSE_DEC4},
    {DSP_RESUME, DSP_RESUME_DEC2, DSP_RESUME_DEC3, DSP_RESUME_DEC4}
};

static void _AudSendDspFlowControlCmd(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4Cmd)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (u4Cmd == DSP_PLAY)
    {
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        AUD_Aproc_Aout_Routine(u1DecId);
#endif

#ifdef CC_AUD_ARM_SUPPORT
        if ((u1DecId == AUD_DEC_MAIN) && (u1DspId==AUD_DSP0))
        {  
            vAprocPostProc_Open ();   
        }    
#endif
        DSP_SendDspTaskCmd(u1DspId, _u4DspFlowControlCmdTable[0][u1DecId]);
        LOG(1, "DSP_SendDspTaskCmd(DSP_PLAY_DEC%d);\n", u1DecId+1);
    }
    else if (u4Cmd == DSP_STOP)
    {
#ifdef CC_AUD_ARM_SUPPORT
        //clear pause state to avoid dsp hangup
        AUD_Aproc_Chg_AVSyncCtrl(u1DecId, APROC_AVSYNC_CTRL_WORK);
        vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecId)), AVSYNC_CMD_WAIT_OFF);
#endif            
        DSP_SendDspTaskCmd(u1DspId, _u4DspFlowControlCmdTable[1][u1DecId]);
        LOG(1, "DSP_SendDspTaskCmd(DSP_STOP_DEC%d);\n", u1DecId+1);
    }
    else if (u4Cmd == DSP_PAUSE)
    {
#ifdef CC_AUD_ARM_SUPPORT
        AUD_Aproc_Chg_AVSyncCtrl(u1DecId, APROC_AVSYNC_CTRL_HOLD);
        vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecId)), AVSYNC_CMD_WAIT_ON);
#endif    
        DSP_SendDspTaskCmd(u1DspId, _u4DspFlowControlCmdTable[2][u1DecId]);
        LOG(1, "DSP_SendDspTaskCmd(DSP_PAUSE_DEC%d);\n", u1DecId+1);
    }
    else if (u4Cmd == DSP_RESUME)
    {
#ifdef CC_AUD_ARM_SUPPORT
        if ( (_arAudDecoder[u1DspId][u1DecId].eSynMode == AV_SYNC_FREE_RUN) || (_arAudDecoder[u1DspId][u1DecId].eSynMode == AV_SYNC_AUDIO_MASTER))
        {
            AUD_Aproc_Chg_AVSyncCtrl(u1DecId, APROC_AVSYNC_CTRL_WORK);
            vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecId)), AVSYNC_CMD_WAIT_OFF);
        }
        else
        {
            _fgAudMMnotifySWDMX[u1DecId] = TRUE;
            AUD_DrvMMAoutEnableNotify(u1DecId, TRUE);
        }
#endif    
        DSP_SendDspTaskCmd(u1DspId, _u4DspFlowControlCmdTable[3][u1DecId]);
        LOG(1, "DSP_SendDspTaskCmd(DSP_RESUME_DEC%d);\n", u1DecId+1);
    }
}

static void _ChangeDecFmtOnPlayState(UINT8 u1DecId)
{
    //MT5387Merge
    // Turn off stream input -- move to beginning of the function,
    //                          because sometimes change format is happened during no HDMI clock, and DSP can't STOP.
    // AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DspId][u1DecId].eStreamFrom, _arAudDecoder[u1DspId][u1DecId].eDecFormat, FALSE, 0);
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
#ifdef CC_AUD_I2S_IN
        ||(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_I2S)
#endif        
        )
    {
        AUD_ClkSetToInternal(u1DecId, AUD_FMT_PCM); //Format is not important here..
    }
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI) &&
        (_arParserInfo[u1DecId].eSpdifInfo.u2format == 0x15) && 
        (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_AC3))
    {
        // HDMI, E-AC-3
        DSP_SetASRCNumeratorQuarter (TRUE);
    }
    else
    {
        DSP_SetASRCNumeratorQuarter (FALSE);
    }
#endif

    #ifdef FIX_AUD_UOP_TIMEOUT
    AUD_WaitUOPLockSema(AUD_DSP0, u1DecId);
    #endif

    _AudSendDspFlowControlCmd(AUD_DSP0, u1DecId, DSP_STOP);
    AUD_WaitUopCommandDone(AUD_DSP0, u1DecId);

    //MT5387Merge
    // Turn off stream input -- move to beginning of the function,
    //                          because sometimes change format is happened during no HDMI clock, and DSP can't STOP.
    AUD_StrInputEnable(AUD_DSP0, u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom, _arAudDecoder[AUD_DSP0][u1DecId].eDecFormat, FALSE, 0);
    _DecoderInitSetup(AUD_DSP0, u1DecId,_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom,_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat);

    VERIFY(x_sema_lock(_hSemaLoadCode, X_SEMA_OPTION_WAIT) == OSR_OK);
#ifdef CC_AUD_ARM_SUPPORT
    if (u1DecId == AUD_DEC_MAIN)
    {  
        vAprocPostProc_Close();   
    }    
#endif
    _LoadAdspCode(AUD_DSP0, u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].eDecFormat);
#ifdef CC_AUD_ARM_SUPPORT
    if (u1DecId == AUD_DEC_MAIN)
    {  
        vAprocPostProc_Open ();   
    }    
#endif
    
    VERIFY(x_sema_unlock(_hSemaLoadCode) == OSR_OK);

    if ((_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_AC3) &&
        (u1DecId <= AUD_DEC_THIRD))
    {
       _FlushAudioFifo(AUD_DSP0, u1DecId);
    }
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
        _FlushParserFifo();
#endif

    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||
         (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
    {
        // Update DSP read pointer and pts
        AUD_ResetDspReadPtr(AUD_DSP0, u1DecId);
        DSP_SetStartPtsToShm(AUD_DSP0, u1DecId, (UINT32)(_arAudPes[u1DecId].u4Pts & 0xffffffff), _arAudPes[u1DecId].u4Wp);
    }

    // Turn on stream input
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||
         (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
    {
        // Turn on stream input
        AUD_StrInputEnable(AUD_DSP0, u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom, _arAudDecoder[AUD_DSP0][u1DecId].eDecFormat, TRUE, _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount);
    }
    else
    {
        // Turn on stream input
        AUD_StrInputEnable(AUD_DSP0, u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom, _arAudDecoder[AUD_DSP0][u1DecId].eDecFormat, TRUE, 0);
    }

    x_thread_delay(10);

    // Reset line-in interface after reseting HDMI audio clock to prevent from inverting L/R channels.
#ifdef FIX_AUD_UOP_TIMEOUT
    AUD_WaitUOPLockSema(AUD_DSP0, u1DecId);
#endif

    _AudSendDspFlowControlCmd(AUD_DSP0, u1DecId, DSP_PLAY);
    AUD_WaitUopCommandDone(AUD_DSP0, u1DecId);
}

static void _AudHdmiMute(UINT8 u1DecId,BOOL fgMute)
{
    if(_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_HDMI)   //  This mute also mute mixsound out for IC MT5368/96/89,
    {                                                                     //  please add condition &&_rAudMixSndStream[0].fgEnable == 0 if you don`t want mixsound be muted when HDMI mute,
        if(fgMute)
        {
            if(!_fgHdmiAoutMute)
            {
                AudAoutGainCtrl(0, 0, AOUT_VOL_GAIN_MUTE, AOUT_VOL_SETP_4DB);
                _fgHdmiAoutMute = TRUE;
                DRVCUST_SendAudEvent(E_CUST_AUD_SPEAKER_MUTE_REQUEST, AUD_DEC_MAIN); //BE_FIXME                    
            }
        }
        else
        {
            if(_fgHdmiAoutMute)
            {
                AudAoutGainCtrl(0, 0, AOUT_VOL_GAIN_0DB, AOUT_VOL_SETP_4DB);
                _fgHdmiAoutMute = FALSE;
                DRVCUST_SendAudEvent(E_CUST_AUD_SPEAKER_UNMUTE_REQUEST, AUD_DEC_MAIN); //BE_FIXME                    
            }
        }
    }

    if (fgMute != _fgHdmiMute)
    {
        // Mute channel volume
        //AUD_DspHdmiChangeFormatMuteEnable(AUD_DEC_MAIN, fgMute); 

        //use input mute
         _AUD_UserSetDecInputMute(u1DecId, fgMute);

        // Mute master volume
        //AUD_HdmiModeMute(AUD_DEC_MAIN, fgMute);
        // Mute amplifier
        ADAC_HdmiUnstableSpkEnable(!fgMute);
    }
    _fgHdmiMute = fgMute;
}

#ifdef CC_MT5881
static void _AudSpdifinMute(BOOL fgMute)
{ 
    if(_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_SPDIF)  
    {
        if(fgMute)
        {
            if(!_fgSpdifinAoutMute)
            {
                AudAoutGainCtrl(0,0,0,AOUT_VOL_SETP_4DB);
                _fgSpdifinAoutMute = TRUE;
            }
        }
        else
        {
            if(_fgSpdifinAoutMute)
            {
                AudAoutGainCtrl(0,0,0x1000,AOUT_VOL_SETP_4DB);
                _fgSpdifinAoutMute = FALSE;
            }
        }
    }
}
#endif


static BOOL _IsChangeHdmiSpdifAudFormat(UINT8 u1DecId, AUD_FMT_T* peDecFmt)
{
    BOOL fgDetected = TRUE;
    INT32     i4Ret = FALSE;
    AUD_FMT_T eMode = AUD_FMT_PCM;
    AUD_DECODE_STATUS_T  eDecStatus;
    AudDecNfyFct  pfAudDecNfy = NULL;
    UINT32        u4Tmp = 0;
    static UINT32 u4DetectCnt[AUD_DEC_MAX] = {0};

    AUD_DEC_ID_VALIDATE(u1DecId);

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

    // Get detected format
    if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT 
        AUD_DrvGetParserDetect(u1DecId, &fgDetected);
        AUD_DrvGetParserFormat(u1DecId, &eMode);     
#else
        AUD_AinGetMode(&fgDetected, &eMode);
#endif
    }
#ifdef CC_MT5881 // Remy spdif in
    else if (_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_SPDIF)
    {
        AUD_AinGetMode_Spdif(&fgDetected, &eMode);
    }
#endif
#ifdef CC_AUD_I2S_IN
    else if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_I2S)
    {
        AUD_AinGetMode(&fgDetected, &eMode);
    }
#endif
    _arAudDecoderHDMI[u1DecId].eDecFormat = eMode;
    // Check detected format and current decoder format
    if (fgDetected)
    {
        // Setup Line-in mode for PCM/RAW
        if (eMode == AUD_FMT_PCM)
        {
            AUD_AinSetMode(0);
        }
        else
        {
            AUD_AinSetMode(1);
        }

        eDecStatus = AUD_DECODE_NOT_SUPPORT;
        // SPDIF format not support, set SPDIF_out source to HDMI Rx
        if (eMode == AUD_FMT_UNKNOWN)
        {
            if(fgHDMIDTS) //From DTS spec, when don't support DTS, SPDIF can't output DTS either.
            {
                AUD_SPDIF_Output_HDMI_Rx(FALSE);  
            }
            else
            {
                AUD_SPDIF_Output_HDMI_Rx(TRUE);
            }
        }
        else
        {
            u4DetectCnt[u1DecId] = 0;
            eDecStatus = AUD_DECODE_NORMAL;
            AUD_SPDIF_Output_HDMI_Rx(FALSE); // It is set at setting IEC
        }

        // Check detected format and current decoder format
        if (eMode != _arAudDecoder[AUD_DSP0][u1DecId].eDecFormat)
        {

        }
        if ((eMode == AUD_FMT_PCM) && (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat != AUD_FMT_PCM))
        {
            *peDecFmt = AUD_FMT_PCM;
            LOG(3, "<< HDMI/SPDIFIN detect as PCM >>\n" );
            LOG(3, "################## Detected PCM and -- HDMI/SPDIFIN Mute! #################\n" );
            i4Ret = TRUE;
        }
        else if ((eMode == AUD_FMT_AC3) && (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat != AUD_FMT_AC3))
        {
             *peDecFmt = AUD_FMT_AC3;
            LOG(3, "<< HDMI/SPDIFIN detect as AC3 >>\n" );
            LOG(3, "################## Detected AC3 and -- HDMI/SPDIFIN Mute! #################\n" );
             i4Ret = TRUE;
        }
        else if ((eMode == AUD_FMT_AAC) && (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat != AUD_FMT_AAC))
        {
             *peDecFmt = AUD_FMT_AAC;
            LOG(3, "<< HDMI/SPDIFIN detect as AAC >>\n" );
            LOG(3, "################## Detected AAC and -- HDMI/SPDIFIN Mute! #################\n" );
             i4Ret = TRUE;
        }
        else if ((eMode == AUD_FMT_DTS) && (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat != AUD_FMT_DTS))
        {
             *peDecFmt = AUD_FMT_DTS;
            LOG(3, "<< HDMI/SPDIFIN detect as DTS >>\n" );
            LOG(3, "################## Detected DTS and -- HDMI/SPDIFIN Mute! #################\n" );
             i4Ret = TRUE;
        }
        else if ((eMode == AUD_FMT_MPEG) &&
                 (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat != AUD_FMT_MPEG) &&
                 (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat != AUD_FMT_MP3))
        {
             *peDecFmt = AUD_FMT_MPEG;
            LOG(3, "<< HDMI/SPDIFIN detect as MPEG >>\n" );
             i4Ret = TRUE;
        }
        else if (eMode == AUD_FMT_UNKNOWN)
        {
            LOG(3, "<< HDMI/SPDIFIN unknown >>\n" );
            u4DetectCnt[u1DecId]++;
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
            if (u4DetectCnt[u1DecId] > 5)
#else
            if ((u4DetectCnt[u1DecId] > 5)||_arParserInfo[u1DecId]._fgRaw)
#endif
            {
                // Default decoder type is AC3 to prevent from noises.
                *peDecFmt = AUD_FMT_AC3;
                LOG(3, "<< HDMI/SPDIFIN unknown >>===> Change to AC3\n" );
                u4DetectCnt[u1DecId] = 0;
                i4Ret = TRUE;
            }
        }
    }
    else
    {
        LOG(3, "<< HDMI/SPDIFIN undetect >>\n" );
        eDecStatus = AUD_DECODE_UNKNOWN;
        u4DetectCnt[u1DecId] ++;
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
        if (u4DetectCnt[u1DecId] > 5)
#else
        if ((u4DetectCnt[u1DecId] > 5)||_arParserInfo[u1DecId]._fgRaw)
#endif
        {
            if (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat != AUD_FMT_AC3)
            {
                // Default decoder type is AC3 to prevent from noises.
                *peDecFmt = AUD_FMT_AC3;
                LOG(3, "<< HDMI/SPDIFIN undetect >> --------> Change to AC3\n" );
                u4DetectCnt[u1DecId] = 0;
                i4Ret = TRUE;
            }
        }
    }

    // Notify middleware decoder status
    if ((pfAudDecNfy != NULL) && (eDecStatus != _aeHDMIDecStatus[u1DecId]))
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_DECODE_STATUS_CHG,
                    (UINT32)eDecStatus, u4Tmp);
    }
    _aeHDMIDecStatus[u1DecId] = eDecStatus;

    return i4Ret;
}

void _AudHdmiOnPlayStateHandler(UINT8 u1DecId)
{
    AUD_SOURCE_STATE_T eSrcState = AUD_SOURCE_STABLE;
    AudDecNfyFct  pfAudDecNfy = NULL;
    UINT8      u1HdmiUnstableMute = 0;
    BOOL    b_ChkHdmiFmt = TRUE;
    static UINT8 u1ChkHdmicount = 0; 
    AUD_FMT_T  eDecFmt=AUD_FMT_UNKNOWN;
#ifdef CC_HDMI_PCM_MULT_CHANNEL
    UINT8   u1HdmiChNum = 2;
#endif

    UNUSED(eDecFmt); 
    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);
    
    //[DTV00081796] change detection period from 200ms to 80ms to avoid noise for PCM->AC3
    if (b_ChkHdmiFmt && ((u1ChkHdmicount%4) == 0)) // For hdmi pop noise.
    {
        // Check format change     
        if (_IsChangeHdmiSpdifAudFormat(u1DecId, &eDecFmt))
        {
            // Already applied mute if change format
            LOG(3, "!!!!! HDMI/SPDIF change format !!!!!\n");

            // Set mute counter
            _u1HDMIMuteCnt = AIN_CHANGE_TYPE_THRESHOLD + HDMI_AUD_UNSTABLE_LVL_2;
            _AudHdmiMute(u1DecId,TRUE);

            // Change decoder
            if(_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat != eDecFmt)
            {            
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT            
                AUD_DrvSetFgHDMIParserthread(u1DecId,FALSE);     
#endif
                _arAudDecoder[AUD_DSP0][u1DecId].eDecFormat = eDecFmt;

                _ChangeDecFmtOnPlayState(u1DecId);
                _AUD_DspSetIEC(u1DecId); //Set SPDIF Type again because decoder type is changed.
            }

        }
    }

    u1HdmiUnstableMute = bHDMIAudStatus();

    // Check HDMI unstable case and apply muting
    if (u1HdmiUnstableMute != HDMI_AUD_OK)
    {
        if (_u1HDMIMuteCnt != 0)
        {
            LOG(6, "################## HDMI unstable -- HDMI Mute! #################\n");
        } 
        _u1HDMIMuteCnt = (UINT8)(AIN_CHANGE_TYPE_THRESHOLD + u1HdmiUnstableMute);
        _AudHdmiMute(u1DecId,TRUE);
        eSrcState = AUD_SOURCE_UNSTABLE;
        LOG(7, "HDMI mute count = %d\n", _u1HDMIMuteCnt);
    }
    // Polling HDMI sampling rate
    if ((u1ChkHdmicount%4) == 0)
    {
            SAMPLE_FREQ_T eHdmiSmpRate;

            eHdmiSmpRate = _GetHdmiSmpRate();

#ifdef CC_HDMI_PCM_MULT_CHANNEL
            u1HdmiChNum = API_HDMI_Get_Aud_ChannelNum()+1;
            if ((u1ChkHdmicount%120) == 0)
            {
                LOG(1,"API_HDMI_Get_Aud_ChannelNum = %d. \n", (API_HDMI_Get_Aud_ChannelNum()+1));
            }
            if ((_eHdmiSmpRate != eHdmiSmpRate) || (_u1HdmiChNum != u1HdmiChNum))
#else
            if (_eHdmiSmpRate != eHdmiSmpRate)
#endif
            {
                LOG(5, " ------------------ HDMI handler set sampling rate (%d) -----------\n", _eHdmiSmpRate);

                if (_u1HDMIMuteCnt == 0)
                {
                    _u1HDMIMuteCnt = AIN_CHANGE_TYPE_THRESHOLD;
                    _AudHdmiMute(u1DecId,TRUE);
                    LOG(5, "XXXX Sampling Rate Change XXXX \n");
                }
#ifdef CC_HDMI_PCM_MULT_CHANNEL
                DSP_SetPcmChNum(u1DecId, u1HdmiChNum);
                AUD_LineInChNumSetting(u1DecId, u1HdmiChNum);
                _u1HdmiChNum = u1HdmiChNum;
#endif
                DSP_SetPcmFs(AUD_DSP0, u1DecId, eHdmiSmpRate);
                AUD_SetSampleFreq(u1DecId, eHdmiSmpRate);
                _eHdmiSmpRate = eHdmiSmpRate;
                //DRVCUST_SendAudEvent(E_CUST_AUD_SPEAKER_UNMUTE_REQUEST, u1DecId); //BE_FIXME                    
            }
    }
    // PCM detect Raw mute
    if ((u1ChkHdmicount%10) == 0)
    {
        if (AUD_Is_PCMDetectRaw())
        {
            if (_u1HDMIMuteCnt == 0)
            {
                _u1HDMIMuteCnt = AIN_CHANGE_TYPE_THRESHOLD;
                _AudHdmiMute(u1DecId,TRUE);
                LOG(5, "XXXX PCM Detect Raw Mute XXXX \n");
            }
            AUD_PCM_Detect_Raw_Mute(FALSE);
        }
    }

    // Check unmute condition
    if (_u1HDMIMuteCnt > 0)
    {
        _u1HDMIMuteCnt --;
        LOG(7, "HDMI mute count = %d\n", _u1HDMIMuteCnt);

        if (_u1HDMIMuteCnt == 0)
        {
            // If format is PCM, then re-playback before signal stable.
            // Since there may be LRCK invert issue during clock unstable period
            if (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eDecFormat == AUD_FMT_PCM)
            {
                #if 0
                u4Temp = (UINT16)DRVCUST_OptGet(eAudioPcmPrebufBankNum);
                if ((_eHdmiSmpRate == FS_32K) && (u4Temp>3))
                {
                    AUD_DspSetPcmPreBuffer(AUD_DSP0, AUD_DEC_MAIN, (u4Temp-3));
                }
                else
                {
                    AUD_DspSetPcmPreBuffer(AUD_DSP0, AUD_DEC_MAIN, u4Temp);
                }
                #endif

                LOG(3, "<< Re-start PCM decoder >>\n");
                _ChangeDecFmtOnPlayState(u1DecId);
                u1ChkHdmicount = 0;
            }
            LOG(3, "@@@@@@@@@@@@@@@@@@@@ HDMI UnMute due to counter is 0! @@@@@@@@@@@@@@@@@@@@ \n" );
            _AudHdmiMute(u1DecId,FALSE);
        }
    }

    // Notify middleware source state
    if ((pfAudDecNfy != NULL) && (eSrcState!= _aeSourceState[u1DecId]))
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_DECODE_STATUS_CHG,
                    (UINT32)eSrcState, 0);
    }
    _aeSourceState[u1DecId] = eSrcState;
    
    u1ChkHdmicount =  u1ChkHdmicount+1 ;
}

#ifdef CC_MT5881 // Remy spdif in
void _AudSpdifinOnPlayStateHandler(UINT8 u1DecId)
{
    //UINT8         u1SpdifinUnstableMute = 0;
    BOOL          b_ChkSpdifinFmt = TRUE;
    static UINT8  u1ChkSpdifincount = 0;
    AUD_FMT_T     eDecFmt = AUD_FMT_UNKNOWN;
    //AUD_FMT_T     eDecFmtTmp = AUD_FMT_UNKNOWN;
    SAMPLE_FREQ_T eSpdifinSmpRate;

    //Todo: Spdif in only in Main & Aux
    if (u1DecId > AUD_DEC_AUX)
    {
        ASSERT(0);
    }

    if (!AUD_Spdifin_Err_Chk())
    {
        // When spdif in error happen, disable linein and spdifin
        AUD_Spdifin_Err_Handle();
        // Set mute counter
        _u1SpdifinMuteCnt = AIN_CHANGE_TYPE_THRESHOLD;
    }
    else
    {
        if (b_ChkSpdifinFmt && ((u1ChkSpdifincount%4)==0)) // For hdmi pop noise.
        {
            // Check format change
            if (_IsChangeHdmiSpdifAudFormat(u1DecId, &eDecFmt))
            {
                // Already applied mute if change format
                LOG(3, "\n\n\n\n !!!!! HDMI/SPDIF change format !!!!!\n\n\n");

                // Set mute counter
                _u1SpdifinMuteCnt = AIN_CHANGE_TYPE_THRESHOLD + HDMI_AUD_UNSTABLE_LVL_2;
                //_AudSpdifinMute(TRUE);

                // Change decoder
                if(_arAudDecoder[u1DspId][u1DecId].eDecFormat != eDecFmt)
                {
                    //eDecFmtTmp = _arAudDecoder[u1DspId][u1DecId].eDecFormat;
                    _arAudDecoder[u1DspId][u1DecId].eDecFormat = eDecFmt;

                    _ChangeDecFmtOnPlayState(u1DecId);
                    _AUD_DspSetIEC(u1DecId); //Set SPDIF Type again because decoder type is changed.
                    AUD_Spdifin_Enable(u1DecId, FALSE);
                }
            }
        }

        // Polling SPDIFIN sampling rate
        if ((u1ChkSpdifincount%4) == 0)
        {
            eSpdifinSmpRate = _GetSpdifinSmpRate();
            if (_eSpdifinSmpRate != eSpdifinSmpRate) // Remy spdif in detect sample rate change
            {
                LOG(5, " ------------------ SPDIF-rx handler set sampling rate (%d) -----------\n", _eSpdifinSmpRate);

                if (_u1SpdifinMuteCnt == 0)
                {
                    _u1SpdifinMuteCnt = AIN_CHANGE_TYPE_THRESHOLD;
                    _AudSpdifinMute(TRUE);
                    LOG(5, "XXXX SPDIF-rx Sampling Rate Change XXXX \n");
                }

                DSP_SetPcmFs(u1DecId, eSpdifinSmpRate);
                AUD_SetSampleFreq(AUD_DEC_MAIN, eSpdifinSmpRate);
                _eSpdifinSmpRate = eSpdifinSmpRate;
            }
        }

        // PCM detect Raw mute
        if ((u1ChkSpdifincount%10) == 0)
        {
            if (AUD_Is_PCMDetectRaw())
            {
                if (_u1SpdifinMuteCnt == 0)
                {
                    _u1SpdifinMuteCnt = AIN_CHANGE_TYPE_THRESHOLD;
                    _AudSpdifinMute(TRUE);
                    LOG(5, "XXXX SPDIF-rx PCM Detect Raw Mute XXXX \n");
                }
                AUD_PCM_Detect_Raw_Mute(FALSE);
            }
        }
    }

    // Check unmute condition
    if (_u1SpdifinMuteCnt > 0)
    {
        _u1SpdifinMuteCnt --;
        LOG(7, "HDMI mute count = %d\n", _u1SpdifinMuteCnt);

        if (_u1SpdifinMuteCnt == 0)
        {
            // If format is PCM, then re-playback before signal stable.
            // Since there may be LRCK invert issue during clock unstable period
            #if 0
            if (_arAudDecoder[AUD_DEC_MAIN].eDecFormat == AUD_FMT_PCM)
            {
                LOG(3, " \n << Re-start PCM decoder >> \n");
                _ChangeDecFmtOnPlayState(u1DecId);
                u1ChkSpdifincount = 0;
            }
            #endif
            LOG(3, "@@@@@@@@@@@@@@@@@@@@ SPDIF-rx UnMute due to counter is 0! @@@@@@@@@@@@@@@@@@@@ \n" );
            AUD_Spdifin_Enable(u1DecId, TRUE);
            _AudSpdifinMute(FALSE);
        }
    }

    u1ChkSpdifincount =  u1ChkSpdifincount+1 ;
}
#endif

#ifdef CC_AUD_I2S_IN
static void _AudI2SMute(BOOL fgMute)
{
    if (fgMute != _fgI2SinMute)
    {
        // Mute channel volume
        AUD_DspHdmiChangeFormatMuteEnable(AUD_DEC_MAIN, fgMute);
        // Mute master volume
        AUD_HdmiModeMute(AUD_DEC_MAIN, fgMute);
        // Mute amplifier
        ADAC_HdmiUnstableSpkEnable(!fgMute);
    }
    _fgI2SinMute = fgMute;
}

static SAMPLE_FREQ_T _GetI2SinSmpRate(void)
{
    UINT16 u2LrckDiv;
    SAMPLE_FREQ_T eAudDrvSampleFreq;

    AUD_WRITE32(REG_SPDFS,0x1);
    u2LrckDiv = AUD_READ32(REG_SPDFS) & SPDF_LRCK_DIV;
    LOG(7, "SPDIF detect sample rate LRCK DIV = 0x%x\n", u2LrckDiv);
    //sampling rate  =  27M/u2LrckDiv
    if((u2LrckDiv>0x349)&&(u2LrckDiv<0x34f))
    {
        eAudDrvSampleFreq = FS_32K;
    }
    else if((u2LrckDiv>0x261)&&(u2LrckDiv<0x267))
    {
        eAudDrvSampleFreq = FS_44K;
    }
    else
    {
        eAudDrvSampleFreq = FS_48K;
    }
    return eAudDrvSampleFreq;
}


void _AudI2SinOnPlayStateHandler(UINT8 u1DecId)
{
    UINT8         u1I2SinUnstableMute = 0;
    BOOL          b_ChkI2SinFmt = TRUE;
    static UINT8  u1ChkI2Sincount = 0;
    AUD_FMT_T  eDecFmt=AUD_FMT_UNKNOWN;
    
    //Todo: I2S in only in Main & Aux
    if (u1DecId > AUD_DEC_AUX)
    {
        ASSERT(0);
    }
    
    if  ((u1ChkI2Sincount%4)==0) // For I2S pop noise.
    {
        // Check format change
        if (_IsChangeHdmiSpdifAudFormat(&eDecFmt))
        {
            // Already applied mute if change format
            LOG(3, "\n\n\n\n !!!!! HDMI/SPDIF/I2S change format !!!!!\n\n\n");

            // Set mute counter
            _u1I2SinMuteCnt = AIN_CHANGE_TYPE_THRESHOLD + HDMI_AUD_UNSTABLE_LVL_2;
            _AudI2SMute(TRUE);

            // Change decoder
            if(_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat != eDecFmt)
            {
                _arAudDecoder[AUD_DSP0][u1DecId].eDecFormat = eDecFmt;
#if 0
            // Check SPDIF output format
            if (eDecFmt == AUD_FMT_AC3)
            {
                AUD_DspIECConfig(u1DecId, AUD_IEC_CFG_RAW, TRUE);
            }
            else
            {
                AUD_DspIECConfig(u1DecId, AUD_IEC_CFG_PCM, TRUE);
            }
#endif //if 0

                _ChangeDecFmtOnPlayState(u1DecId);
                _AUD_DspSetIEC(u1DecId); //Set SPDIF Type again because decoder type is changed.
            }

        }
    }


    // Polling I2SIN sampling rate
    if ((u1ChkI2Sincount%4) == 0)
    {
        SAMPLE_FREQ_T eI2SinSmpRate;

        eI2SinSmpRate = _GetI2SinSmpRate();
        if (_eI2SinSmpRate != eI2SinSmpRate) // Remy spdif in detect sample rate change
        {
            LOG(5, " ------------------ I2S in handler set sampling rate (%d) -----------\n", eI2SinSmpRate);

            if (_u1I2SinMuteCnt == 0)
            {
                _u1I2SinMuteCnt = AIN_CHANGE_TYPE_THRESHOLD;
                _AudI2SMute(TRUE);
                LOG(5, "XXXX I2S in Sampling Rate Change XXXX \n");
            }

            DSP_SetPcmFs(AUD_DSP0, u1DecId, eI2SinSmpRate);
            AUD_SetSampleFreq(AUD_DEC_MAIN, eI2SinSmpRate);
            _eI2SinSmpRate = eI2SinSmpRate;
        }
    }

    // PCM detect Raw mute
    if ((u1ChkI2Sincount%10) == 0)
    {
        if (AUD_Is_PCMDetectRaw())
        {
            if (_u1I2SinMuteCnt == 0)
            {
                _u1I2SinMuteCnt = AIN_CHANGE_TYPE_THRESHOLD;
                _AudI2SMute(TRUE);
                LOG(5, "XXXX PCM Detect Raw Mute XXXX \n");
            }
            AUD_PCM_Detect_Raw_Mute(FALSE);
        }
    }
        



    // Check unmute condition
    if (_u1I2SinMuteCnt > 0)
    {
        _u1I2SinMuteCnt --;
        LOG(7, "I2S mute count = %d\n", _u1I2SinMuteCnt);

        if (_u1I2SinMuteCnt == 0)
        {
            // If format is PCM, then re-playback before signal stable.
            // Since there may be LRCK invert issue during clock unstable period
        #if 0
            if (_arAudDecoder[AUD_DEC_MAIN].eDecFormat == AUD_FMT_PCM)
            {
                LOG(3, " \n << Re-start PCM decoder >> \n");
                _ChangeDecFmtOnPlayState(u1DecId);
                u1ChkI2Sincount = 0;
            }
        #endif
            LOG(3, "@@@@@@@@@@@@@@@@@@@@ I2S-rx UnMute due to counter is 0! @@@@@@@@@@@@@@@@@@@@ \n" );
            _AudI2SMute(FALSE);
        }
    }
    u1ChkI2Sincount = u1ChkI2Sincount+1;

}
#endif

// *********************************************************************
// Function : void _AudApllMonitorThread(void)
// Description :
// Parameter : none
// Return    : None
// *********************************************************************
static void _AudApllMonitorThread(void* pvArg)
{
    UINT32 u4Event;
    UINT16 u2MsgIdx;
    UINT8 u1StcId = 0;
    SIZE_T zMsgSize = sizeof(UINT32);

//sunman_aout
#ifdef BURN_IN_AOUT_TEST //sunman_aout
    static UINT32 u4PrevOutputUnderRun = 0;
    UINT32 u4OutputUnderRun = 0;
    UINT32 u4AoutCrcFail = 0;

    _gu1AoutDramErr=0;
#endif

    if (pvArg != NULL)
    {
        u1StcId = *(UINT8 *)pvArg;
    }
    UNUSED(pvArg);

    while (1)
    {
        VERIFY(x_msg_q_receive(&u2MsgIdx, &u4Event, &zMsgSize,
        &_hAudApllCmdQueue[u1StcId], 1, X_MSGQ_OPTION_WAIT) == OSR_OK);

#ifndef CC_DISABLE_SMART_PCR
        _AUD_DspAsrcSetSmartVCXO(u4Event);
#endif

#ifdef BURN_IN_AOUT_TEST //sunman_aout
        if (u1StcId == 0)
        {
        u4PrevOutputUnderRun = u4ReadDspSram(SRAM_DECODER_OUTPUT_UNDERRUN);
        AUD_WRITE32(REG_AOUT_CRC_CTL, 0x2);
        AUD_WRITE32(REG_AOUT_CRC_CTL, 0x11);
        x_thread_delay(20);
        LOG(3, "\n ===== Aout Dram Burning Test Begin ===== \n");
        while (1)
        {
            u4AoutCrcFail = (AUD_READ32(REG_AOUT_CRC_CTL) & 0x400);
            if (u4AoutCrcFail == 0x400)
            {
                LOG(3," CRC Fail Raised, check underrun ...........\n");
                u4OutputUnderRun = u4ReadDspSram(SRAM_DECODER_OUTPUT_UNDERRUN);
                if (u4OutputUnderRun == u4PrevOutputUnderRun)
                {
                    _gu1AoutDramErr++;
                    LOG(3, "Failed !!!!!!!!!!!!!! Underrun is not increased (%d, %d)...... \n", u4PrevOutputUnderRun, u4OutputUnderRun);
                }
                else
                {
                    LOG(3, "It's ok .. Underrun is increased...... (%d, %d)\n", u4PrevOutputUnderRun, u4OutputUnderRun);
                }
                u4PrevOutputUnderRun = u4OutputUnderRun;
                AUD_WRITE32(REG_AOUT_CRC_CTL, 0x2);
                AUD_WRITE32(REG_AOUT_CRC_CTL, 0x11);
            }
            x_thread_delay(20);
        }
        }
#else
        x_thread_delay(100);
#endif
    }
}
#ifdef CC_AUD_DDI
void AUD_WatchSpdifEnable(BOOL fgEnable)
{
    _fgWatchSpdifEnable = fgEnable;
}

static void _AudSoundBarThread(void* pvArg)
{
    static UINT32 u4State = 0;
    UINT32 u4Event;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(UINT32);

    UINT8 Id = 0;
    UINT8 data  = 0;
    UINT8 value = 0;
    UINT32 volumeInfo = 0;

    UNUSED(pvArg);
    _fgWatchSpdifEnable = FALSE;

    while (1)
    {
    VERIFY(x_msg_q_receive(&u2MsgIdx, &u4Event, &zMsgSize,
    &_hAudSoundBarCmdQueue, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);

    Id = uReadShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR_ID);
    data = uReadShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR_DATA);
    if(u4State==0)
    {
    value = ((Id & 0xF0) |((data & 0xF0) >>4));
    vWriteShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR, value);
    vSoundBarUpdate();
    u4State = 1;
    }
    else
    {
    value = (((Id & 0x0F) << 4) |(data & 0x0F));
    vWriteShmUINT8(AUD_DSP0, B_IEC_SOUNDBAR, value);
    vSoundBarUpdate();
    u4State = 0;
    }
    
    if (_fgWatchSpdifEnable)
    {
    LOG(0,"The spdif soundbar state register = 0x%x\n",u4ReadDspSram(AUD_DSP0, 0x2aa));
    }
    volumeInfo = u4Event;
    if(volumeInfo == 0)
    {
        x_thread_delay(20);
    }
    else
    {
        x_thread_delay(5);
    }

    }
}
#endif
//2008/01/15 added by ling
#ifndef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
static void AudSetDetTvSrcMode(UINT8 u1DecId)
{
    if ((_au4TvSysMask[u1DecId] & TV_SYS_MASK_L) == TV_SYS_MASK_L)
    {
        AUD_DspSetDetectDefaultMode(4); // SECAM-L
    }
    else if ((_au4TvSysMask[u1DecId] & TV_SYS_MASK_L_PRIME) == TV_SYS_MASK_L_PRIME)
    {
        AUD_DspSetDetectDefaultMode(4); // SECAM-L or SECAM-L'
    }
    else
    {
        AUD_DspSetDetectDefaultMode(0); // PAL-DK
    }
}
#endif

 void _AudSetTvSysMask(UINT8 u1DecId, UINT32 u4Mask)
{
     _au4TvSysMask[u1DecId]=u4Mask;
}

#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
   void _AudSetTvAudSysMask(UINT8 u1DecId, UINT32 u4TvMask, UINT32 u4TvAudMask, ISO_3166_COUNT_T country_code)
  {
       _au4TvSysMask[u1DecId]= u4TvMask;
       _au4TvAudSysMask[u1DecId]= u4TvAudMask;
       x_strncpy( (CHAR *) _au4TvSysCountry, (CHAR *) country_code, ISO_3166_COUNT_LEN);
  }
#endif

  UINT32 _AudGetTvSysMask(UINT8 u1DecId)
 {
    return  _au4TvSysMask[u1DecId];
 }





#ifndef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
#ifdef CC_AUD_DDI
 AUD_FMT_T _AudAtvFmtDetection(UINT8 u1ENCDecId)
#else
 AUD_FMT_T _AudAtvFmtDetection(void)
#endif
{
    TV_AUD_SYS_T eTvSys;
    AUD_FMT_T    eAudFmt;
#ifdef CC_AUD_DDI
    UINT8 u1DecId = u1ENCDecId;
#else
    UINT8 u1DecId = AUD_DEC_AUX;
    // Waiting for analog tuner lock signal ..
    _WaitAtunerLockSignal();
#endif
    UNUSED(AUD_SetDecType(AUD_DSP0, u1DecId, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_DETECTOR));
    UNUSED(_AUD_IgnoreDecNotify(u1DecId, TRUE));

    //2008/01/15 added by ling
    AudSetDetTvSrcMode(AUD_DEC_MAIN);

    // Try lock to make sure the semaphore was locked
    UNUSED(x_sema_lock(_hDetectionSyncSema, X_SEMA_OPTION_NOWAIT));
    UNUSED(AUD_DSPCmdPlay(AUD_DSP0, u1DecId));
    AUD_DetectionSyncSema(TRUE);
    UNUSED(AUD_DSPCmdStop(AUD_DSP0, u1DecId));
    UNUSED(_AUD_IgnoreDecNotify(u1DecId, FALSE));
    eTvSys = DSP_GetAtvTvSystem();
#ifndef CC_AUD_DDI
    DSP_SetTvSrcMode(eTvSys);
#endif

    switch (eTvSys)
    {
        case SV_A2_BG:
        case SV_A2_DK1:
        case SV_A2_DK2:
        case SV_A2_DK3:
            eAudFmt = AUD_FMT_A2;
            break;
        case SV_PAL_I:
        case SV_PAL_BG:
        case SV_PAL_DK:
            eAudFmt = AUD_FMT_PAL;
            break;
        case SV_MTS:
            eAudFmt = AUD_FMT_MTS; // FIXME
            break;
        default:
            eAudFmt = AUD_FMT_PAL;
            LOG(1, "Invalid _AudAtvFmtDetection\n");
    }

    return eAudFmt;
}
#else
 AUD_FMT_T _AudAtvFmtDetection(void)
{
    TV_AUD_SYS_T eTvSys = SV_NONE_DETECTED;
    AUD_FMT_T    eAudFmt;
    AudDecNfyFct   pfAudDecNfy = NULL;
    MW_TV_AUD_SYS_T rAudTvSys,rAudTvSysFromMW;
    UINT8 u1DecState = 0;
    UINT32 u4CandTVSysMask,u4TunerSysMask;
    UINT32 u4AddTVSysMask = 0;      //to remove build warning
    UINT8 u1DecId = AUD_DEC_MAIN;

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

    rAudTvSysFromMW.u4TvSysMask = _au4TvSysMask[u1DecId] ; //from _AudSetTvAudSysMask
    rAudTvSysFromMW.u4AudSysMask = _au4TvAudSysMask[u1DecId]; //from _AudSetTvAudSysMask


    if (x_strncmp( (CHAR *)_au4TvSysCountry, "GBR", sizeof("GBR")) == 0)
    {
        //I
        u4CandTVSysMask = AUD_TV_SYS_MASK_I;
    }
    else if (x_strncmp( (CHAR *)_au4TvSysCountry, "RUS", sizeof("RUS")) == 0)
    {
        //DK
        u4CandTVSysMask = AUD_TV_SYS_MASK_D | AUD_TV_SYS_MASK_K;
    }
    else
    {
        //BG
        u4CandTVSysMask = AUD_TV_SYS_MASK_B | AUD_TV_SYS_MASK_G;
    }
    u4TunerSysMask = rAudTvSysFromMW.u4TvSysMask & 0xFFFF;

    // Waiting for analog tuner lock signal ..
    _WaitAtunerLockSignal();    //TODO: check if need to do this every time after we config tuner BW

    UNUSED(AUD_SetDecType(AUD_DSP0, AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_DETECTOR));
    UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));
    UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));  // for 3 decoder, may send MAIN decoder notify.


    if (u4TunerSysMask == u4CandTVSysMask)
    {
        //MW setting = candidate list, jump to state 1
        u1DecState = 1;
    }

    //check if TV sys is L or M
    if ( (rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_L) == AUD_TV_SYS_MASK_L ||
         (rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_L_PRIME) == AUD_TV_SYS_MASK_L_PRIME)
    {
        rAudTvSys.u4TvSysMask = rAudTvSysFromMW.u4TvSysMask;
        rAudTvSys.u4AudSysMask = rAudTvSysFromMW.u4AudSysMask;
        eTvSys = SV_SECAM_L;
        LOG(1, "MW set _AudAtvFmtDetection = SV_SECAM_L, RETURN!\n");
    }
    else if ( (rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_M) == AUD_TV_SYS_MASK_M)
    {
        rAudTvSys.u4TvSysMask = rAudTvSysFromMW.u4TvSysMask;
        rAudTvSys.u4AudSysMask = rAudTvSysFromMW.u4AudSysMask;
        eTvSys = SV_MTS;
        LOG(1, "MW set _AudAtvFmtDetection = SV_MTS, RETURN!\n");
    }
    else
    {
        //detection loop
        //TODO: MT5365 should not need this loop
        AUD_DspSetDetectDefaultMode(0); // PAL-DK
        DSP_SetDetTvSrcMode(SV_SECAM_DK);
        while (1)
        {
            //config tuner BW
            AtdEU_AudSetSystem(_AudTvSys2Tuner(u4TunerSysMask),0);  //TODO: need new 5365 API


            // Try lock to make sure the semaphore was locked
            UNUSED(x_sema_lock(_hDetectionSyncSema, X_SEMA_OPTION_NOWAIT));
            UNUSED(AUD_DSPCmdPlay(AUD_DEC_MAIN));
            AUD_DetectionSyncSema(TRUE);
            UNUSED(AUD_DSPCmdStop(AUD_DEC_MAIN));

            DSP_GetAtvTvSys(&rAudTvSys);
            eTvSys = DSP_GetAtvTvSystem();
#if 0
            if (((rAudTvSys.u4TvSysMask == 0) && (rAudTvSys.u4AudSysMask == 0)))
            {
                //detector might be stopped before results are notified
                u1DecState = 0xFF;
                LOG(0,"Sound System Auto Detection Failed...\n");
                break;
            }
#endif
            //state 0: check if match MW setting
            if (u1DecState == 0)
            {
                LOG(5,"=====AUTO DETECT STATE 0...\n");
                if ((rAudTvSys.u4TvSysMask & 0xFFFF)== (rAudTvSysFromMW.u4TvSysMask & 0xFFFF))
                {
                    LOG(5,"Detected,match MW setting \n");
                    break;
                }
                LOG(5,"Not Detected\n");
                u1DecState = 1;
                //set tuner to candidate list
                u4TunerSysMask = u4CandTVSysMask;
            }
            //state 1:  check if match candidate list
            else if (u1DecState == 1)
            {
                LOG(5,"=====AUTO DETECT STATE 1...\n");
                if ((rAudTvSys.u4TvSysMask & 0xFFFF) == u4CandTVSysMask )
                {
                    LOG(5,"Detected,match candidate list  \n");
                    break;
                }
                else
                {
                    //add in additional list
                    u4AddTVSysMask = rAudTvSys.u4TvSysMask;
                    //set tuner to additional list
                    u4TunerSysMask = u4AddTVSysMask;
                    LOG(5,"Not Detected\n");
                }
                u1DecState = 3;
            }
            //state 3:  check if match additional list
            else if (u1DecState == 3)
            {
                LOG(5,"=====AUTO DETECT STATE 3...\n");
                if ((rAudTvSys.u4TvSysMask != u4AddTVSysMask) || (eTvSys < SV_A2_BG))       //block TV sys not in EU
                {
                    //cannot find valid system, use setting from MW
                    u1DecState = 0xFF;
                    LOG(0,"cannot find valid system\n");
                }
                else
                {
                    LOG(5,"Detected,match additional list  \n");
                }
                break;

            }


        }
        if (u1DecState == 0xFF)
        {
            //cannot find valid system, use setting from MW
            eTvSys = _AudGetTvSysFormat(rAudTvSysFromMW);
            rAudTvSys.u4TvSysMask = rAudTvSysFromMW.u4TvSysMask;
            rAudTvSys.u4AudSysMask = rAudTvSysFromMW.u4AudSysMask;
        }
        AtdEU_AudSetSystem(_AudTvSys2Tuner(rAudTvSys.u4TvSysMask),1);    //TODO: need new 5365 API
    }

    UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));
    UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));



    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_TV_SYS_DETECTED,
                        rAudTvSys.u4TvSysMask,rAudTvSys.u4AudSysMask);
    }


    LOG(5,"Set TV system = %d\n",eTvSys);
    _au4TvSysMask[u1DecId]=rAudTvSys.u4TvSysMask;
    DSP_SetTvSrcMode(eTvSys);
    switch (eTvSys)
    {
        case SV_A2_BG:
        case SV_A2_DK1:
        case SV_A2_DK2:
        case SV_A2_DK3:
            eAudFmt = AUD_FMT_A2;
            break;
        case SV_PAL_I:
        case SV_PAL_BG:
        case SV_PAL_DK:
        case SV_SECAM_L:
            eAudFmt = AUD_FMT_PAL;
            break;
        case SV_MTS:
            eAudFmt = AUD_FMT_MTS;
            break;
        case SV_NONE_DETECTED:
            eAudFmt = AUD_FMT_UNKNOWN;
            break;
        default:
            eAudFmt = AUD_FMT_PAL;
            LOG(1, "Invalid _AudAtvFmtDetection\n");
    }

    return eAudFmt;
}


TV_AUD_SYS_T _AudGetTvSysFormat(MW_TV_AUD_SYS_T rAudTvSysFromMW)
{
    TV_AUD_SYS_T eTvSys = SV_PAL_BG;
    if (((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2) == AUD_SYS_MASK_FM_A2) ||
        ((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2_DK1) == AUD_SYS_MASK_FM_A2_DK1) ||
        ((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2_DK2) == AUD_SYS_MASK_FM_A2_DK2))
    {
        if (((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_B) == AUD_TV_SYS_MASK_B) &&
            ((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_G) == AUD_TV_SYS_MASK_G))
        {
            eTvSys =  SV_A2_BG;
        }
        else if (((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2) == AUD_SYS_MASK_FM_A2))
        {
            eTvSys = SV_A2_DK3;
        }
        else if (((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2_DK1) == AUD_SYS_MASK_FM_A2_DK1))
        {
            eTvSys = SV_A2_DK1;
        }
        else if (((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_A2_DK2) == AUD_SYS_MASK_FM_A2_DK2))
        {
            eTvSys = SV_A2_DK2;
        }
    }
    else if (((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_FM_MONO) == AUD_SYS_MASK_FM_MONO) ||
        ((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_AM) == AUD_SYS_MASK_AM) ||
        ((rAudTvSysFromMW.u4AudSysMask & AUD_SYS_MASK_NICAM) == AUD_SYS_MASK_NICAM))
    {
        if ((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_I) == AUD_TV_SYS_MASK_I)
        {
            eTvSys =  SV_PAL_I;
        }
        else if (((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_D) == AUD_TV_SYS_MASK_D) &&
            ((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_K) == AUD_TV_SYS_MASK_K))
        {
            eTvSys = SV_PAL_DK;
        }
        else if (((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_B) == AUD_TV_SYS_MASK_B) &&
                 ((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_G) == AUD_TV_SYS_MASK_G))
        {
            eTvSys = SV_PAL_BG;
        }
        else if (((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_L) == AUD_TV_SYS_MASK_L))
        {
            eTvSys = SV_SECAM_L;
        }
        else if (((rAudTvSysFromMW.u4TvSysMask & AUD_TV_SYS_MASK_L_PRIME) == AUD_TV_SYS_MASK_L_PRIME))
        {
            eTvSys = SV_SECAM_L_PLUM;
        }
    }
    else
    {
        LOG(0,"MW set sound system error, Set system as PAL_BG\n");
        eTvSys = SV_PAL_BG;     //TODO
    }
    return eTvSys;
}

UINT8 _AudTvSys2Tuner(UINT32 u4TvSysMask)
{
    if (((u4TvSysMask & AUD_TV_SYS_MASK_B) == AUD_TV_SYS_MASK_B) &&
        ((u4TvSysMask & AUD_TV_SYS_MASK_G) == AUD_TV_SYS_MASK_G))
    {
        LOG(5,"Set tuner as MOD_PAL_BG\n");
        return(MOD_PAL_BG);
    }
    else if (((u4TvSysMask & AUD_TV_SYS_MASK_D) == AUD_TV_SYS_MASK_D) &&
        ((u4TvSysMask & AUD_TV_SYS_MASK_K) == AUD_TV_SYS_MASK_K))
    {
        LOG(5,"Set tuner as MOD_PAL_DK\n");
        return(MOD_PAL_DK);

    }
    else if (((u4TvSysMask & AUD_TV_SYS_MASK_I) == AUD_TV_SYS_MASK_I))
    {
        LOG(5,"Set tuner as MOD_PAL_I\n");
        return(MOD_PAL_I);
    }
    else
    {
        LOG(0,"Error Set tuner mode, Set tuner as MOD_PAL_BG\n");
        return(MOD_PAL_BG);     //TODO
    }
}
#endif

static void _AudWaitPlayProcess(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT16 u2FeedStreamWaitCnt =0; // Feed_and_Auddrv Thread sync

    _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_PLAYING;
    if((u1DecId==AUD_DEC_MAIN) || (u1DecId==AUD_DEC_THIRD))
    {
        DRVCUST_SendAudEvent(E_CUST_AUD_PLAY_STATE_CHANGE, u1DecId);
    }
#ifdef CC_AUD_FAST_STOP_BY_HW_GAIN
    AudAoutGainCtrl(u1DecId, 0, 0x1000,AOUT_VOL_SETP_4DB);
#endif
    // Reset rp for DTV source to avoid afifo full.
    if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        if (_arAudDecoder[u1DspId][u1DecId].u4ReceivePesCount > 0)
        {
            AUD_ResetDspReadPtr(u1DspId, u1DecId);
        }
    }
    
    //For DTV00080543==>move clear pes count to _AudConnect
    if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA)
    {
        _arAudDecoder[u1DspId][u1DecId].u4ReceivePesCount = 0;
    }

    _arAudDecoder[u1DspId][u1DecId].u4ReceiveValidPesCount = 0;
    // reset PTS table related information
    AUD_DrvPtsQueueReset(u1DecId); //PTSQueue_V2
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)            
    AUD_Aproc_Reset_AVSyncInfo(u1DecId); //new_arm_pts_table
#endif       
    //_afgFirstIPicArrived[u1DecId] = FALSE;
    _aeHDMIDecStatus[u1DecId] = AUD_DECODE_UNKNOWN;
    _aeSourceState[u1DecId] = AUD_SOURCE_UNKOWN;
    //_afgIssuePlayComToDsp[u1DecId] = FALSE;  //Laihui:for initial twice keep update write point, move to idle state

    //clear previous 1st PTS info
    DSP_SetStartPtsToShm(u1DspId, u1DecId, 0, DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DspId, u1DecId)));

    if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY)
    {
        _fgForceStopStrmDma = TRUE;
        while (!_fgFeedStrmFinshed)   // Feed_and_Auddrv Thread sync
        {
            x_thread_delay(10);       // If FeedStream is not exit yet, should wait. Or create thresd will fail.
            u2FeedStreamWaitCnt ++;
            if (u2FeedStreamWaitCnt > 10) break; //max watiung is 100ms.
        }
    }

    if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)||
        (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER && _fgMMPlusAD[u1DecId] ==TRUE))
    {
        _fgMMSet[u1DecId]=FALSE;
        //For DTV00080543==>move clear EOS Flag to _AudConnect
        //_AudClearEOSFlag(u1DecId);
    }

#ifdef CC_MT5391_AUD_3_DECODER
    if ((u1DecId == AUD_DEC_AUX) && (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
#else
    if ((u1DecId == AUD_DEC_MAIN) && (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
#endif
    {
        _AUD_DspAsrcResetSmartVCXO();
    }

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        _AUD_DspAsrcResetSmartVCXO();
        _AUD_AprocEnableVCXO(u1DecId, TRUE);
    }
    else
    {
        _AUD_AprocEnableVCXO(u1DecId, FALSE);        
    }
#endif

    if (u1DspId == AUD_DSP0)
    {
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    if (1) //Render System, each DecID is individual. (e.x EU DTV only Play Dec1, no PCM on Dec0)
#else    
    if (u1DecId == AUD_DEC_MAIN)
#endif      
    {
#ifndef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
        AUD_DspInputChannelDelay(_arAudDecoder[u1DspId][u1DecId].eStreamFrom);
#endif
#ifndef CC_AUD_DDI //cause Aout Reinit
#ifndef CC_AUD_USE_NVM
        AUD_DspInputChangeAVCPara(_arAudDecoder[u1DspId][u1DecId].eStreamFrom);
#endif
#endif
#ifdef CC_AUD_LOUDNESS_FROM_SOURCE_SOUNDMODE
        AUD_DspInputChangeLoudnessPara(_arAudDecoder[u1DspId][u1DecId].eStreamFrom);
#endif

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
        if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom != AUD_STREAM_FROM_FEEDER) &&
            (_arAudDecoder[u1DspId][u1DecId].eStreamFrom != AUD_STREAM_FROM_MEMORY) &&
            (_arAudDecoder[u1DspId][u1DecId].eDecFormat != AUD_FMT_FMRDO) &&
            (!((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)&&(_fgMMPlusAD[u1DecId] == TRUE))) &&
#ifdef CC_ENABLE_AOMX
             (!(((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)||(_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_GST)) && (AUD_DrvGetMMAudioOnly(u1DecId))))
#else
             (!((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) && (_fgMMAudioOnly[u1DecId])))
#endif
            )
            {
                if ((_fgPlayMuteEnable)
#ifdef CC_AUD_NOT_UNMUTE_WITH_VDP
                 && (_arAudDecoder[u1DspId][u1DecId].eStreamFrom != AUD_STREAM_FROM_DIGITAL_TUNER)  // Improve performance of DTV channel changed 
#endif
                                )
                {
                #if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
                    if (u1DecId <= AUD_DEC_THIRD)
                    {
                        AUD_DspDecPlayMuteEnable(u1DecId, TRUE);
                        VERIFY(AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_MUTE, 4000)); 
                    }                       
                #else
                    AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, TRUE);
                    VERIFY(AUD_PlayMuteCmd(AUD_DEC_MAIN,AUD_PM_CMD_MUTE, 4000));
                #endif
                }
                else
                {
                    // Init channel delay
                    AUD_DspChannelDelay_initial(0, AUD_CH_ALL, AUD_DEC_MAIN);
                    LOG(5, "Init channel delay\n");
                #if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
                    _AudAprocInputMute(u1DecId, FALSE);
                #endif               
                }
            }
#endif
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        _AUD_DspStopMuteEnable(AUD_DEC_AUX, FALSE);
    }
    else if (u1DecId == AUD_DEC_4TH)
    {
//#ifndef CC_MT5389
//        AUD_DspSetEncMode(AUD_DEC_MAIN, AUD_ENC_MODE_INPUT_BEFORE_POST_PROCESSING);
//#endif
    }
    }

    AUD_StrInputEnable(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].eStreamFrom, _arAudDecoder[u1DspId][u1DecId].eDecFormat, TRUE, _arAudDecoder[u1DspId][u1DecId].u4ReceivePesCount);

    _DMX_SetInterruptThreshold(DMX_PID_TYPE_ES_AUDIO,1+188*2); // 3*188
#ifdef ENABLE_MULTIMEDIA
#ifdef CC_53XX_SWDMX_V2
    if(_arAudDecoder[u1DspId][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
    {
        // comment by koro, 2011/10/21
        UINT32 u4HdMvc = 0;

        UNUSED(SWDMX_GetInfo(_u1SwdmxSrcId[u1DecId], eSWDMX_GET_HDMVC_INFO, &u4HdMvc, 0, 0));

        if(u4HdMvc)
        {
            _DMX_SetInterruptThreshold(DMX_PID_TYPE_ES_AUDIO,188*3*5); // 15*188
        }
    }
#endif
#endif
}

static void _AudWaitStopProcess(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DEC_CMD_T eDecCmd;
    UINT32 u4CmdResult = AUD_OK;
    
#ifdef CC_AUD_ARM_SUPPORT
    if ((u1DecId == AUD_DEC_MAIN) && (u1DspId==AUD_DSP0))
    {  
        vAprocPostProc_Close ();   
    }    
#endif  
    _FlushAudioFifo(u1DspId, u1DecId); // dirty fix for parser cann't reset single channel write pointer
    if(_gu1AudUploadMode != 0)
    {
        _FlushUploadFifo();
    }
#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
    if(_gu1AudBluetoothMode != 0)
    {
        _FlushBluetoothFifo();
    }
#endif
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
    _FlushParserFifo();
#endif

    eDecCmd = (_arAudDrvState[u1DspId][u1DecId] == AUD_WAIT_STOP) ? AUD_CMD_STOP : AUD_CMD_STOPSTREAM; 
    u4CmdResult = (_arAudDrvState[u1DspId][u1DecId] == AUD_WAIT_STOP) ? AUD_OK : _u4FeedStreamResult; 
    _ChangeAudioState(u1DspId, u1DecId, AUD_IDLE);
    AUD_CommandDone(u1DspId, u1DecId, eDecCmd, u4CmdResult);
    
    if (_fgMMPlusAD [u1DecId] ==TRUE)
    {
        _fgMMPlusAD [u1DecId] = FALSE;
    }
    // Terminate the feed stream thread
    if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY)
    {
        VERIFY(x_sema_unlock(_hFeedStreamSyncSema) == OSR_OK);
        _hAudFeedStrThread = NULL_HANDLE;
    }

    // Turn off stream input
    AUD_StrInputEnable(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].eStreamFrom, _arAudDecoder[u1DspId][u1DecId].eDecFormat, FALSE, 0);

    if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
    {
        _guAudDspTrigAtvFmtchg = 0;
        //Disable ATV format change if _arAudDecoder[u1DspId][u1DecId] == AUD_WAIT_STOP;
    }
    if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        _AudHdmiMute(u1DecId,FALSE);
        _u1HDMIMuteCnt = 0; // Reset mute counter
    }

#ifdef LINUX_TURNKEY_SOLUTION
    // Set next file flag
    if (_hAudDataDumpThreadState[u1DecId] == 1)
    {
        _fgAudDataDumpNextFile[u1DecId] = TRUE;
    }
#endif

    // Clear video scrambled flag
    AUD_VideoScrambled(FALSE);

    //_ChangeAudioState(u1DecId, AUD_IDLE);
#ifndef CC_AUD_SUPPORT_MS10
    _AUD_DspSetDDCOEnable(FALSE);
#endif
    //restroe Main decoder volume when AD disable
    if ((u1DecId == AUD_DEC_THIRD) && (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
    {
        AUD_DspAdFadeVolumeSet(0);
        AUD_DspAdFadeVolumeUpdate(AUD_DEC_MAIN, AUD_CH_ALL);
        AUD_DspAdPanVolumeSet(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT, 0);
        AUD_DspAdPanVolumeSet(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT, 0);
        AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT);
        AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT);
    }
    // reset PTS table related information
    AUD_DrvPtsQueueReset(u1DecId); //PTSQueue_V2
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)            
    AUD_Aproc_Reset_AVSyncInfo(u1DecId); //new_arm_pts_table
    //AUD_Aproc_Aout_Ok(u1DecId);
    _AUD_AprocEnableVCXO(u1DecId, FALSE); 
#endif 
}

// *********************************************************************
// Function :  AUD_IsSourceFromWFD(void)
// Description :
// Parameter : none
// Return    : None
// *********************************************************************
BOOL AUD_IsSourceFromWFD(UINT8 u1DecId)
{
#ifdef ENABLE_MULTIMEDIA
    SWDMX_SOURCE_TYPE_T SrcType = SWDMX_SRC_TYPE_UNKNOWN;
    AUD_DEC_ID_VALIDATE(u1DecId);

    SWDMX_GetInfo(_u1SwdmxSrcId[u1DecId], eSWDMX_GET_MM_SRC_TYPE, (UINT32*)&SrcType, 0, 0);
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) && 
        (SrcType == SWDMX_SRC_TYPE_WIFI_DISPLAY))
    {
        return TRUE;
    }
    else   
    {
        return FALSE;
    }
#else
    return FALSE;
#endif   
}

// *********************************************************************
// Function : void _AudDrvThread(void)
// Description : Main state machine of audio driver
// Parameter : none
// Return    : None
// *********************************************************************
static void _AudDrvThread(void* pvArg)
{
    UINT8 u1DspId = AUD_DSP0;
    UINT8 u1DecId = AUD_DEC_MAIN;
    UINT8* tmpptr;
    AUD_DRV_STATE_T eNewState = AUD_WAIT_STOP;
    UINT32 u4Event = 0;
    BOOL fgClipDone = FALSE;
    BOOL fgWaitPlayIssuePause = FALSE;
    BOOL fgWaitPlayOnPause = FALSE;
    UINT8 u1DspDecId[2];
    AUD_FMT_T eAudFormat;
    
    UINT8 u1StcDiffLo = u4GetStcDiffFactor(AUD_STC_LO);
    UINT8 u1StcDiffHi = u4GetStcDiffFactor(AUD_STC_HI);
    UINT8 u1StcDiffWs = u4GetStcDiffFactor(AUD_STC_WS);

    if (pvArg != NULL)
    {
        tmpptr = (UINT8 *)pvArg;
        u1DspId = tmpptr[0];
        u1DecId = tmpptr[1];
    }
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    while (1)
    {
        switch (_arAudDrvState[u1DspId][u1DecId])
        {
        case AUD_TRIGGER_ADSP:
            _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_INIT;
            vADSPPowerON(u1DspId);
            _ChangeAudioState(u1DspId, u1DecId, AUD_WAIT_POWER_ON);
            _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_INIT;
            break;

        case AUD_WAIT_POWER_ON:
            _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_INIT;

            if (!fgADSPIsIdle(u1DspId))
            {
                _ChangeAudioState(u1DspId, u1DecId, AUD_IDLE);
            }
            else
            {
                x_thread_delay(1);
            }
            break;

        case AUD_DECODE_INIT:
            _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_INIT;
            VERIFY(x_sema_lock(_hSemaLoadCode, X_SEMA_OPTION_WAIT) == OSR_OK);
            _LoadAdspCode(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].eDecFormat);
            AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_LOADCODE, AUD_OK);
            VERIFY(x_sema_unlock(_hSemaLoadCode) == OSR_OK);

            _ChangeAudioState(u1DspId, u1DecId, AUD_IDLE);
            break;

        case AUD_ON_PLAY:
            if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY)
            {
                _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_STOP  |
                                                      AUD_CMD_FLAG_STOPSTREAM |
                                                      AUD_CMD_FLAG_RESET |
                                                      AUD_CMD_FLAG_PAUSE);
                VERIFY(_AudWaitEvent(u1DspId, u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));

                switch (u4Event)
                {
                case AUD_CMD_RESET:
                    _AudDrvResetDSP(u1DecId); //  -- DSP Force --
                    eNewState = AUD_ON_PLAY;
                    break;

                case AUD_CMD_STOP:
                    eNewState = AUD_WAIT_STOP;
                    break;

                case AUD_CMD_STOPSTREAM:
                    eNewState = AUD_WAIT_STOPSTREAM;
                    break;

                case AUD_CMD_PAUSE:
                    eNewState = AUD_WAIT_PAUSE;
                    break;

                default:
                    ASSERT(0);
                }
                _ChangeAudioState(u1DspId, u1DecId, eNewState);
                break;
            }
            else if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_FEEDER))
            {
                _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_STOP  |
                                                      AUD_CMD_FLAG_RESET |
                                                      AUD_CMD_FLAG_PAUSE);
                VERIFY(_AudWaitEvent(u1DspId, u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));

                switch (u4Event)
                {
                case AUD_CMD_RESET:
                    _AudDrvResetDSP(u1DecId); //  -- DSP Force --
                    eNewState = AUD_ON_PLAY;
                    break;

                case AUD_CMD_STOP:
                    eNewState = AUD_WAIT_STOP;
                    break;

                case AUD_CMD_PAUSE:
                    eNewState = AUD_WAIT_PAUSE;
                    break;

                default:
                    ASSERT(0);
                }
                _ChangeAudioState(u1DspId, u1DecId, eNewState);
                break;
            }
            else if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||
                     (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
                    #ifdef CC_ENABLE_AOMX
                     ||(_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_GST)
                    #endif
                    )
            {
                _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_STOP   |
                                                      AUD_CMD_FLAG_RESET |
                                                      AUD_CMD_FLAG_PAUSE |
                                                      AUD_CMD_FLAG_CHANGE_FORMAT);
                if (u1DspId==AUD_DSP0)
                {
              #ifdef CC_ENABLE_AOMX
                    if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom != AUD_STREAM_FROM_GST)
              #endif
                {
                    _afgDtvOnPlayThreadGo[u1DecId] = TRUE;      //koro todo can we use onplaythread for GST
                }
                }
                VERIFY(_AudWaitEvent(u1DspId, u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));

                switch (u4Event)
                {
                case AUD_CMD_RESET:
                    _AudDrvResetDSP(u1DecId);  //  -- DSP Force --
                    eNewState = AUD_ON_PLAY;
                    break;

                case AUD_CMD_STOP:
                    //LOG(1, " ****** Got stop message in ON_PLAY state\n");
                    eNewState = AUD_WAIT_STOP;
                    if (u1DspId==AUD_DSP0)
                    {
                    _DtvResetLockStatus(u1DecId);
                    AUD_GetAudioPts(u1DecId, &u4OldPts);
                    AUD_MMQueueSyncInfo(u1DecId,0,0,TRUE); //Reset MM Sync info
                #ifdef TIME_SHIFT_SUPPORT
                    AUD_MMQueueTickInfo(u1DecId,0,0,TRUE); //Reset MM Sync info
                #endif
                    }    
                    break;

                case AUD_CMD_CHANGE_FORMAT:
                    eNewState = AUD_CHANGE_FORMAT;
                    break;

                case AUD_CMD_PAUSE:
                    eNewState = AUD_WAIT_PAUSE;
                    break;

                default:
                    ASSERT(0);
                }
                //LOG(1, "  <<<<<<<<<<<<<<<<< set _fgDtvOnPlayThreadGo = FALSE <<<<<<<\n");
                if (u1DspId==AUD_DSP0)
                {
                _afgDtvOnPlayThreadGo[u1DecId] = FALSE;
                }    
                _ChangeAudioState(u1DspId, u1DecId, eNewState);
                break;
            }
            else if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI) ||
                (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_SPDIF)
                #ifdef CC_AUD_I2S_IN
                ||(_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_I2S)
                #endif
                    )
            {
                _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_STOP  |
                                                      AUD_CMD_FLAG_RESET |
                                                      AUD_CMD_FLAG_CHANGE_FORMAT);

                if (!_AudWaitEvent(u1DspId, u1DecId, &u4Event, FALSE, AUD_HDMI_DETECT_PERIOD)
                    && _arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI) // No event got, then continue
                {
                    _AudHdmiOnPlayStateHandler(u1DecId);
                }
#ifdef CC_MT5881 //Remy spdif in
                else if(!_AudWaitEvent(u1DecId, &u4Event, FALSE, AUD_HDMI_DETECT_PERIOD)
                    && _arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_SPDIF)
                {
                    _AudSpdifinOnPlayStateHandler(u1DecId);
                }
#endif
#ifdef CC_AUD_I2S_IN  //I2S in
                else if(!_AudWaitEvent(u1DspId, u1DecId, &u4Event, FALSE, AUD_HDMI_DETECT_PERIOD)
                    && _arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_I2S)
                {
                    _AudI2SinOnPlayStateHandler(u1DecId);
                }
#endif

                else
                {
                    switch (u4Event)
                    {
                    case AUD_CMD_STOP:
                        // Move to WAIT_STOP state. When dsp is stopped, then unmute.
                        //_AudHdmiMute(FALSE);
                        //_u1HDMIMuteCnt = 0; // Reset mute counter
                        eNewState = AUD_WAIT_STOP;
                        break;

                    case AUD_CMD_RESET:
                        _AudDrvResetDSP(u1DecId);  //  -- DSP Force --
                        eNewState = AUD_ON_PLAY;
                        break;

                    case AUD_CMD_CHANGE_FORMAT:
                        eNewState = AUD_CHANGE_FORMAT;
                        break;

                    default:
                        ASSERT(0);
                    }
                    _ChangeAudioState(u1DspId, u1DecId, eNewState);
                }
            }
            else if(_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
          //if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) && (u1DecId != AUD_DEC_MAIN))
            {
                _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_STOP  |
                                                      AUD_CMD_FLAG_RESET |
                                                      AUD_CMD_FLAG_PAUSE |
                                                      AUD_CMD_FLAG_CHANGE_FORMAT);
                if (!_AudWaitEvent(u1DspId, u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT))
                {
                    if (_guAudDspTrigAtvFmtchg)
                    {
                        //trigger ATV change format
                        eNewState = AUD_CHANGE_FORMAT;
                        _ChangeAudioState(u1DspId, u1DecId, eNewState);
                    }
                }
                else
                {
                    switch (u4Event)
                    {
                    case AUD_CMD_RESET:
                        _AudDrvResetDSP(u1DecId);  //  -- DSP Force --
                        eNewState = AUD_ON_PLAY;
                        break;

                    case AUD_CMD_STOP:
                        eNewState = AUD_WAIT_STOP;
                        break;

                    case AUD_CMD_PAUSE:
                        eNewState = AUD_WAIT_PAUSE;
                        break;
                    case AUD_CMD_CHANGE_FORMAT:
                        eNewState = AUD_CHANGE_FORMAT;
                        break;
                    default:
                        ASSERT(0);
                    }
                    _ChangeAudioState(u1DspId, u1DecId, eNewState);
                }
            }
            else
            {
                _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_STOP  |
                                                      AUD_CMD_FLAG_RESET |
                                                      AUD_CMD_FLAG_PAUSE |
                                                      AUD_CMD_FLAG_CHANGE_FORMAT);

                VERIFY(_AudWaitEvent(u1DspId, u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));

                switch (u4Event)
                {
                case AUD_CMD_RESET:
                    _AudDrvResetDSP(u1DecId);  //  -- DSP Force --
                    eNewState = AUD_ON_PLAY;
                    break;

                case AUD_CMD_STOP:
                    eNewState = AUD_WAIT_STOP;
                    break;

                case AUD_CMD_PAUSE:
                    eNewState = AUD_WAIT_PAUSE;
                    break;

                case AUD_CMD_CHANGE_FORMAT:
                    eNewState = AUD_CHANGE_FORMAT;
                    break;

                default:
                    ASSERT(0);
                }
                _ChangeAudioState(u1DspId, u1DecId, eNewState);
                break;
            }
            break;

            case AUD_WAIT_PLAY:
                if (!fgWaitPlayIssuePause)
                {
                    _AudWaitPlayProcess(u1DspId, u1DecId);
                }
                
                if(AUD_IsSourceFromWFD(u1DecId))
                {
                      u1StcDiffLo = u4GetStcDiffFactor(AUD_STC_LO);
                      u1StcDiffHi = u4GetStcDiffFactor(AUD_STC_HI);
                      u1StcDiffWs = u4GetStcDiffFactor(AUD_STC_WS);
                      DSP_SetStcDiffBound(APLL_ADJUST_WFDL, APLL_ADJUST_WFDH, APLL_ADJUST_WFDWS);
                }


                // Check if need AV synchronization control
                if (_AvSyncControl(u1DspId, u1DecId))
                {
                    LOG(1,"######### NEED SYNC: Aud dsp(%d)dec(%d) wait AVSYNC CMD to real play\n",u1DspId, u1DecId);
                    if (!fgWaitPlayIssuePause)
                    {
                        // Release Play command API waiting semaphore
                        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PLAY, AUD_OK);
                    }
                    // Wait Av-Sync or User Stop command
                    _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_STOP   |
                                          AUD_CMD_FLAG_RESET  |
                                          AUD_CMD_FLAG_AVSYNC |
                                          AUD_CMD_FLAG_CHANGE_FORMAT |
                                          AUD_CMD_FLAG_PAUSE |
                                          AUD_CMD_FLAG_RESUME);

                    _fgWaitAvSync[u1DecId] = TRUE;
                    //if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||
                    //    (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
                    if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
                    {
#ifdef  CC_AUD_DDBANNER
                        _AUD_DDBannerEnable(u1DecId, TRUE); //Andrew : For DD test only
#endif
#if (defined(CC_AUD_SUPPORT_MS10) && defined(CC_MT5391_AUD_3_DECODER))
                        if (u1DecId == AUD_DEC_THIRD)
                        {
                            _AudWaitEventAD(&u4Event, FALSE);
                        }
                        else
#endif
                        {
                            // DTV must have Avsync command
                            // Light: Since DTV needs to wait for 1st I frame, it takes longer time to wait for tne event.
                            if((gfgFirstTimeout) && (AUD_DEC_AUX == u1DecId))
                            {
                                gfgFirstTimeout = FALSE;
                                if (!_AudWaitEvent(u1DspId, u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT_DTV_1st))
                                {
                                vDspSetFifoReadPtr(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Wp);
                                DSP_SetStartPtsToShm(u1DspId, u1DecId, (UINT32)(_arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Pts & 0xffffffff), _arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Wp);
                                    u4Event = (UINT32)AUD_CMD_AVSYNC;
                                    LOG(7, "NO AV sync message retrieve in DTV\n");
                                }
                            }
                            else
                            {
                                if (!_AudWaitEvent(u1DspId, u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT_DTV))
                                {
                                    vDspSetFifoReadPtr(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Wp);
                                    DSP_SetStartPtsToShm(u1DspId, u1DecId, (UINT32)(_arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Pts & 0xffffffff), _arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Wp);
                                    u4Event = (UINT32)AUD_CMD_AVSYNC;
                                    LOG(7, "NO AV sync message retrieve in DTV 1st\n");
                                }
                            }
                         }
                    }
                    else if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
                            #ifdef CC_ENABLE_AOMX
                            ||(_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_GST)
                            #endif
                            )
                    {
#ifdef  CC_AUD_DDBANNER
                        _AUD_DDBannerEnable(u1DecId, TRUE); //Andrew : For DD test only
#endif
                        //For DTV00080543==>aud_drv self trigger AUD_CMD_AVSYNC
                        // to avoid PES has been sent done for small size file
                        if (((_arAudDecoder[u1DspId][u1DecId].u4ReceivePesCount != 0) &&
                             ((_arAudDecoder[u1DspId][u1DecId].eSynMode == AV_SYNC_AUDIO_MASTER) ||
                              (_arAudDecoder[u1DspId][u1DecId].eSynMode == AV_SYNC_SLAVE && _arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))) || // Timeshift mode
                            AUD_Is_MMDecFmtScramble(u1DecId))
                        {
                            if (!fgWaitPlayOnPause)
                            {
                                vDspSetFifoReadPtr(u1DspId, u1DecId, DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DspId, u1DecId)));
                                DSP_SetStartPtsToShm(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].u4StartPts, DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DspId, u1DecId)));
                                VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_AVSYNC));
                                LOG(7, "===>aud_drv self trigger AUD_CMD_AVSYNC in  MM\n");
                            }
                        }

                        // MM must have Avsync command
                        // Laihui: MM also need to wait data until timeout, longer then DTV. Cause file maybe a lot of error data.
                        if (!_AudWaitEvent(u1DspId, u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT_MM))
                        {
                            //koro: if issue pause command in this state, should keep waiting for PES
                            if (!fgWaitPlayIssuePause)
                            {
                                vDspSetFifoReadPtr(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Wp);
                                DSP_SetStartPtsToShm(u1DspId, u1DecId, (UINT32)(_arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Pts & 0xffffffff), _arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Wp);
                                u4Event = (UINT32)AUD_CMD_AVSYNC;
                                LOG(7, "NO AV sync message retrieve in MM\n");
                            }
                        }
                    }
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521
                    else if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
                    {
                        // If wait Av sync message time out, play audio
                        if (!_AudWaitEvent(u1DspId, u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT))
                        {
                            u4Event = (UINT32)AUD_CMD_AVSYNC;
                            LOG(7, "NO AV sync message retrieve in ATV \n");
                        }
                    }
#else
                   else if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) ||
                               (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN) ||
                               (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN_2) ||
                               (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN_3))
                    {
                        // If wait Av sync message time out, play audio
                        if (!_AudWaitEvent(u1DspId, u1DecId, &u4Event, FALSE, AUD_EVENT_TIMEOUT))
                        {
                            u4Event = (UINT32)AUD_CMD_AVSYNC;
                            LOG(7, "NO AV sync message retrieve in ATV \n");
                        }
                    }
#endif
                    switch (u4Event)
                    {
                    case AUD_CMD_STOP:
                        //Laihui: 0428 If from line-in to idle, need to turn off line-in buffer, or other source,like multimedia, buf, will be affected by AFIFO.
                        AUD_StrInputEnable(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].eStreamFrom, _arAudDecoder[u1DspId][u1DecId].eDecFormat, FALSE, _arAudDecoder[u1DspId][u1DecId].u4ReceivePesCount);

                        //disable clear MM-related flags (done in swdmx_drvif.c, 20090915)
                        if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA)
                        { // If not MMP, clear conflict flag by audio driver
                            AUD_MM_Set_Dec_Fmt_Conflict(u1DecId,FALSE);
                        }
                        //AUD_MM_Set_Dec_Fmt_Scramble(u1DecId,FALSE);
                        // Release Stop command API waiting semaphore
                        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_STOP, AUD_OK);
                        _ChangeAudioState(u1DspId, u1DecId, AUD_IDLE);
                        fgWaitPlayIssuePause = FALSE;
                        fgWaitPlayOnPause= FALSE;
                        break;

                    case AUD_CMD_AVSYNC:
                        //Don't issue play when on pause
                        if (!fgWaitPlayOnPause)
                        {
                            if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) && 
                                ((_arAudDecoder[u1DspId][u1DecId].eDecFormat == AUD_FMT_MPEG) || 
                                (_arAudDecoder[u1DspId][u1DecId].eDecFormat == AUD_FMT_MP3)))
                            {
                                eAudFormat = AUD_CheckMpegLayer(u1DspId, u1DecId);
                                if ((eAudFormat != AUD_FMT_UNKNOWN) && (eAudFormat != _arAudDecoder[u1DspId][u1DecId].eDecFormat))
                                {
                                    LOG(2, "Format change: %s ==> %s\n", 
                                            GET_PCHAR_ARRAY_AT(_aszCodeType, _arAudDecoder[u1DspId][u1DecId].eDecFormat),
                                            GET_PCHAR_ARRAY_AT(_aszCodeType, eAudFormat));                                    
                                    _arAudDecoder[u1DspId][u1DecId].eDecFormat = eAudFormat;
                                    VERIFY(x_sema_lock(_hSemaLoadCode, X_SEMA_OPTION_WAIT) == OSR_OK);
                                    _LoadAdspCode(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].eDecFormat);
                                    VERIFY(x_sema_unlock(_hSemaLoadCode) == OSR_OK);                                    
                                } 
                            }
                            gfgFirstTimeout1 = FALSE;
                          #ifdef FIX_AUD_UOP_TIMEOUT
                            AUD_WaitUOPLockSema(u1DspId, u1DecId);
                          #endif
                            _AudSendDspFlowControlCmd(u1DspId, u1DecId, DSP_PLAY);

                            _afgIssuePlayComToDsp[u1DspId][u1DecId] = TRUE;
                            AUD_WaitUopCommandDone(u1DspId, u1DecId);
                            LOG( 3,"DSP(%d) Aud decoder %d Play\n",u1DspId, u1DecId);
                            _ChangeAudioState(u1DspId, u1DecId, AUD_ON_PLAY);
                            fgWaitPlayIssuePause = FALSE;
                            fgWaitPlayOnPause= FALSE;
                        }
                        break;

                    case AUD_CMD_CHANGE_FORMAT:
                        // Do nothing on this state, keep play back
                        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_CHANGE_FORMAT,TRUE);
                        fgWaitPlayIssuePause = FALSE;
                        fgWaitPlayOnPause= FALSE;

                        MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_PLAY_DONE);
                        break;

                    case AUD_CMD_PAUSE:
                        // Do nothing on this state, keep play back
                        if (!fgWaitPlayOnPause)
                        {
                            AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PAUSE,TRUE);
                            //_ChangeAudioState(u1DecId, AUD_WAIT_PAUSE);
                        }
                        fgWaitPlayIssuePause = TRUE;
                        fgWaitPlayOnPause= TRUE;
                        break;

                    case AUD_CMD_RESUME:
                        // Do nothing on this state, keep play back
                        if (fgWaitPlayOnPause)
                        {
                            AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_RESUME,TRUE);
                            //_ChangeAudioState(u1DecId, AUD_WAIT_PAUSE);
                        }
                        fgWaitPlayOnPause = FALSE;
                        break;

                    case AUD_CMD_RESET:
                        _AudDrvResetDSP(u1DecId); //  -- DSP Force --
                        eNewState = AUD_WAIT_PLAY;
                        break;

                    default:
                        LOG(0, "Dec(%d) receive Error command %s\n", u1DecId, _paszAudCmd[u4Event]);
                        ASSERT(0);
                    }
                }
                else  // No need of initial AV synchronization control
                {
                    LOG(1,"######### NO NEED SYNC ##########\n");

                    // In DTV case, set decoding start point at FIFO start
                    if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
                        (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
                    {
                        UINT32 u4FifoStart;
                        UINT32 u4FifoEnd;

                        UNUSED(_AUD_GetAudFifo(u1DspId, u1DecId, &u4FifoStart, &u4FifoEnd));
                        vDspSetFifoReadPtr(u1DspId, u1DecId, u4FifoStart);
                    }

                #ifdef FIX_AUD_UOP_TIMEOUT
                    AUD_WaitUOPLockSema(u1DspId, u1DecId);
                #endif

                _AudSendDspFlowControlCmd(u1DspId, u1DecId, DSP_PLAY);
                _afgIssuePlayComToDsp[u1DspId][u1DecId] = TRUE;

                    AUD_WaitUopCommandDone(u1DspId, u1DecId);
                    LOG(3, "DSP(%d) Aud decoder %d Play\n",u1DspId, u1DecId);

                    // Create the feed stream thread for audio clip play
                    if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY)
                    {
                    #ifdef LINUX_TURNKEY_SOLUTION
                        if (!skip_create_feed_stream)
                        {
                    #endif
                        // Create audio feed stream thread
                        u1DspDecId[0] = u1DspId;
                        u1DspDecId[1] = u1DecId;
                        if (OSR_OK != x_thread_create(&_hAudFeedStrThread,
                            AUD_DRV_FEEDSTR_THREAD_NAME,
                            AUD_DRV_THREAD_STACK_SIZE,
                            AUD_DRV_THREAD_PRIORITY,
                            (x_os_thread_main_fct) _AudFeedStreamThread,
                            2*sizeof(UINT8),
                            (void *)&u1DspDecId))
                        {
                            _u4FeedStreamResult = (UINT32)AUD_FAIL;
                            _ChangeAudioState(u1DspId, u1DecId, AUD_WAIT_STOPSTREAM);
                            break;
                        }
                    #ifdef LINUX_TURNKEY_SOLUTION
                            Printf("Create FeedStreamThread\n");
                        }
                    #endif
#ifdef CC_AUD_DDI
                        AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);
#endif
                    }
                    else if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_FEEDER)
                    {
                        static BOOL fgThreadCreate = FALSE;

                        if (!fgThreadCreate)
                        {
                            fgThreadCreate = TRUE;

                            VERIFY(x_sema_create(&_hSemaFeederThreadWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

                            u1DspDecId[0] = u1DspId;
                            u1DspDecId[1] = u1DecId;
                            if (OSR_OK != x_thread_create(&_hAudFeederThread,
                                AUD_DRV_FEEDER_THREAD,
                                AUD_DRV_THREAD_STACK_SIZE,
                                AUD_DRV_THREAD_PRIORITY,
                                (x_os_thread_main_fct) _AudFeederThread,
                                2*sizeof(UINT8),
                                (void *)&u1DspDecId))
                            {
                                _ChangeAudioState(u1DspId, u1DecId, AUD_WAIT_STOP);
                                LOG(1, "fail to create audio feeder thread\n");
                                break;
                            }
                        }

                        // Release semaphore
                        LOG(5, "**** Feeder thead ON ****** \n");
                        VERIFY(x_sema_unlock(_hSemaFeederThreadWait) == OSR_OK);
                    }
                    #ifdef CC_ENABLE_AOMX    
                    else if((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_GST) && AUD_DrvGetMMAudioOnly(u1DecId))
                    {
                        AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);
                    }
                    #endif
                                                             // Create the copy data thread for data from AFIFO3
                _ChangeAudioState(u1DspId, u1DecId, AUD_ON_PLAY);
                /* Release Play command API waiting semaphore */
                AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PLAY, AUD_OK);

                MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_PLAY_DONE);
            }

#ifndef CC_AUD_ARM_RENDER
            if (u1DecId == AUD_DEC_MAIN)
#endif
            {

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
                if ((_arAudDecoder[u1DspId][u1DecId].eDecFormat != AUD_FMT_DETECTOR)
                    #ifdef CC_ENABLE_AOMX
                    && (!((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_GST) && (AUD_DrvGetMMAudioOnly(u1DecId))))
                    &&(_arAudDecoder[u1DspId][u1DecId].eStreamFrom != AUD_STREAM_FROM_MEMORY)
                    #endif
                    )
                {
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
                    if (u1DecId <= AUD_DEC_THIRD)
#endif                      
                        VERIFY(AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_DSP_PLAY, 0));
            }
#endif
            }
            break;

        case AUD_CHANGE_FORMAT:
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DspId, u1DecId);
            #endif

            _AudSendDspFlowControlCmd(u1DspId, u1DecId, DSP_STOP);
            AUD_WaitUopCommandDone(u1DspId, u1DecId);

            // Turn off stream input
            AUD_StrInputEnable(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].eStreamFrom, _arAudDecoder[u1DspId][u1DecId].eDecFormat, FALSE, 0);

            VERIFY(x_sema_lock(_hSemaLoadCode, X_SEMA_OPTION_WAIT) == OSR_OK);
           // if (((u1DecId == AUD_DEC_AUX)||(u1DecId == AUD_DEC_THIRD)) && (_guAudDspTrigAtvFmtchg))
           if(_guAudDspTrigAtvFmtchg)
            {
                _guAudDspTrigAtvFmtchg = 0;
                _LoadAdspCode(AUD_DSP0, u1DecId, _eAtvFmtChgDecFmt);
                LOG(5, " ATV FMT CHG PLAY\n");
            }
            else
            {
#ifdef CC_AUD_ARM_SUPPORT
                if (u1DecId == AUD_DEC_MAIN)
                {  
                    vAprocPostProc_Close();   
                }    
#endif              
                _LoadAdspCode(AUD_DSP0, u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].eDecFormat);
#ifdef CC_AUD_ARM_SUPPORT
                if (u1DecId == AUD_DEC_MAIN)
                {  
                    vAprocPostProc_Open();   
                }    
#endif
            }
            VERIFY(x_sema_unlock(_hSemaLoadCode) == OSR_OK);

            // Update DSP read pointer and pts
            if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
                (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
            {
                AUD_ResetDspReadPtr(u1DspId, u1DecId);
                DSP_SetStartPtsToShm(u1DspId, u1DecId, (UINT32)(_arAudPes[u1DecId].u4Pts & 0xffffffff), _arAudPes[u1DecId].u4Wp);
            }

            // Setup individual decoder setting by source
            _DecoderInitSetup(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].eStreamFrom, _arAudDecoder[u1DspId][u1DecId].eDecFormat);

            // Turn on stream input
            if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
                (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
            {
                AUD_StrInputEnable(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].eStreamFrom, _arAudDecoder[u1DspId][u1DecId].eDecFormat, TRUE, _arAudDecoder[u1DspId][u1DecId].u4ReceivePesCount);
            }
            else
            {
                AUD_StrInputEnable(u1DspId, u1DecId, _arAudDecoder[u1DspId][u1DecId].eStreamFrom, _arAudDecoder[u1DspId][u1DecId].eDecFormat, TRUE, 0);
            }
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DspId, u1DecId);
            #endif

            _AudSendDspFlowControlCmd(u1DspId, u1DecId, DSP_PLAY);
            AUD_WaitUopCommandDone(u1DspId, u1DecId);
            AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_CHANGE_FORMAT, TRUE);
            _ChangeAudioState(u1DspId, u1DecId, AUD_ON_PLAY);
            break;

        case AUD_ON_PAUSE:
            _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_STOP  |
                                                  AUD_CMD_FLAG_RESET |
                                                  AUD_CMD_FLAG_STOPSTREAM |
                                                  AUD_CMD_FLAG_RESUME);
            VERIFY(_AudWaitEvent(u1DspId, u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));

            switch (u4Event)
            {
            case AUD_CMD_RESET:
                _AudDrvResetDSP(u1DecId);  //  -- DSP Force --
                eNewState = AUD_ON_PAUSE;
                break;

            case AUD_CMD_STOP:
                eNewState = AUD_WAIT_STOP;
                AUD_MMQueueSyncInfo(u1DecId,0,0,TRUE); //Reset MM Sync info
                break;

            case AUD_CMD_RESUME:
                eNewState = AUD_WAIT_RESUME;
                break;

            case AUD_CMD_STOPSTREAM:
                eNewState = AUD_ON_PAUSE;
                //AUD_CommandDone(u1DecId,  AUD_CMD_STOPSTREAM, AUD_OK);
                fgClipDone = TRUE;
                break;

            default:
                ASSERT(0);
            }
            _ChangeAudioState(u1DspId, u1DecId, eNewState);
            break;

        case AUD_WAIT_PAUSE:
            _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_PAUSE;
            if((u1DecId==AUD_DEC_MAIN) || (u1DecId==AUD_DEC_THIRD))
            {
                DRVCUST_SendAudEvent(E_CUST_AUD_PLAY_STATE_CHANGE, u1DecId);
            }
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DspId, u1DecId);
            #endif

//            _arAudDecoder[u1DspId][u1DecId].fgMMAoutEnable = FALSE; //do not change the flag here
            _AudSendDspFlowControlCmd(u1DspId, u1DecId, DSP_PAUSE);
            AUD_WaitUopCommandDone(u1DspId, u1DecId);
            LOG(3, "DSP(%d)Aud decoder %d Pause\n", u1DspId, u1DecId);
            _ChangeAudioState(u1DspId, u1DecId, AUD_ON_PAUSE);
            AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PAUSE, AUD_OK);

            MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_PAUSE_DONE);
            break;

        case AUD_WAIT_RESUME:
            if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
            {
                _AudDrvThread_WaitResume(u1DspId, u1DecId);
            }
            else
            {
                _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_PLAYING;
                if((u1DecId==AUD_DEC_MAIN) || (u1DecId==AUD_DEC_THIRD))
                {
                    DRVCUST_SendAudEvent(E_CUST_AUD_PLAY_STATE_CHANGE, u1DecId);
                }

                #ifdef FIX_AUD_UOP_TIMEOUT
                AUD_WaitUOPLockSema(u1DspId, u1DecId);
                #endif

                _AudSendDspFlowControlCmd(u1DspId, u1DecId, DSP_RESUME);
                AUD_WaitUopCommandDone(u1DspId, u1DecId);
                LOG(3, "DSP(%d)Aud decoder %d RESUME\n", u1DspId, u1DecId);
                _ChangeAudioState(u1DspId, u1DecId, AUD_ON_PLAY);
                AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_RESUME, AUD_OK);
                if (fgClipDone)
                {
                    fgClipDone = FALSE;
                    _ChangeAudioState(u1DspId, u1DecId, AUD_IDLE);
                    AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_STOPSTREAM, AUD_OK);
                }
            }

            break;

        case AUD_WAIT_STOP:
        case AUD_WAIT_STOPSTREAM:
            _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_STOP;
            if((u1DecId==AUD_DEC_MAIN) || (u1DecId==AUD_DEC_THIRD))
            {
                DRVCUST_SendAudEvent(E_CUST_AUD_PLAY_STATE_CHANGE, u1DecId);
            }
            
#ifndef CC_AUD_ARM_RENDER
            if (u1DecId == AUD_DEC_MAIN)
#endif                
            {
#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
#ifdef CC_AUD_ARM_RENDER
                if (u1DecId <= AUD_DEC_THIRD)
                {
                    VERIFY(AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_RESET, 0));
                }
#else
                VERIFY(AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_RESET, 0));
#endif
#endif
            }
        #ifdef CC_AUD_BITRATE_CHG_NOTIFY
            _AudResetBitRateChangedPts();   // Reset changed pts
        #endif
        
            _fgWaitAvSync[u1DecId] = FALSE;
            _arAudDecoder[u1DspId][u1DecId].u4ReceivePesCount = 0;
            _arAudDecoder[u1DspId][u1DecId].u4ReceiveValidPesCount = 0;
            _AudClearEOSFlag(u1DecId); 

            if(u1DecId==AUD_DEC_AUX)
            {
                u4PrevValidPes = 0;
                isEsExist = TRUE;
                u4NoPESCount=0;
                u4NoPESCount1=0;
            }
            _afgFirstIPicArrived[u1DecId] = FALSE;
            _arAudDecoder[u1DspId][u1DecId].fgMMAoutEnable = FALSE;
            _arAudDecoder[u1DspId][u1DecId].fgMMAoutReady = FALSE;

            if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY)
            {
                _fgForceStopStrmDma = TRUE;
            }

            if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_FEEDER)
            {
                _fgFeederThreadGo = FALSE;
            }

            if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
            {
                // Disable HDMI SPDIF bypass
                LOG(5,"Disable HDMI bypass mode when stopping\n");
                AUD_SPDIF_Output_HDMI_Rx(FALSE);
            }

            if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN) || 
                (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN_2) ||
                (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_LINE_IN_3))
            {
                _fgAVAudioOnly = FALSE;
                DSP_SetLineInLrInverse(u1DspId, u1DecId,FALSE);
            }
            if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
                (_arAudDecoder[u1DspId][u1DecId].eMultimediaMode == AUD_MM_MODE_GAME))
            {
                AUD_DrvSetMultimediaMode(u1DecId, AUD_MM_MODE_NORMAL);
            }
            if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
            {
                DSP_SetStcDiffBound(u1StcDiffLo, u1StcDiffHi, u1StcDiffWs);  
            }
            if (u1DecId == AUD_DEC_AUX)
            {
                _AUD_DspStopMuteEnable(AUD_DEC_AUX, TRUE);
                x_thread_delay(10); // Wait for mute finished
            }

            if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI)
            {
                //turn off source first, because HDMI may loss clock
                //AUD_StrInputEnable(u1DecId, _arAudDecoder[u1DspId][u1DecId].eStreamFrom, _arAudDecoder[u1DspId][u1DecId].eDecFormat, FALSE, 0);
                if (!AUD_IsHdmiClkExist())
                {
                    AUD_ClkSetToInternal(u1DecId, AUD_FMT_PCM); //Format is not important here..
                }
            }

            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DspId, u1DecId);
            #endif

            _AudSendDspFlowControlCmd(u1DspId, u1DecId, DSP_STOP);
            if ( AUD_WaitUopCommandDone(u1DspId, u1DecId) == OSR_TIMEOUT && !fgAUD_DspProbeHalt(u1DspId)) // if manual probe debug is on, don't halt driver
            {
                LOG(0,"Aud DSP Stop time out!!!");
                AUD_CommandDone(u1DspId, u1DecId, ((_arAudDrvState[u1DspId][u1DecId] == AUD_WAIT_STOP) ?
                    AUD_CMD_STOP : AUD_CMD_STOPSTREAM), (UINT32)AUD_FAIL);
                _ChangeAudioState(u1DspId, u1DecId, AUD_DSP_ERROR);
                break; //direct change to error state
            }
            
            // Audio stop process          
            _AudWaitStopProcess(u1DspId, u1DecId);

            MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_STOP_DONE);
            
            break;

        case AUD_IDLE:
            _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_STOP;
            if((u1DecId==AUD_DEC_MAIN) || (u1DecId==AUD_DEC_THIRD))
            {
                DRVCUST_SendAudEvent(E_CUST_AUD_PLAY_STATE_CHANGE, u1DecId);
            }
            _afgIssuePlayComToDsp[u1DspId][u1DecId] = FALSE;

            _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_PLAY   |
                                                  AUD_CMD_FLAG_STOP   |
                                                  AUD_CMD_FLAG_RESET  |
                                                  AUD_CMD_FLAG_RESUME |
                                                  AUD_CMD_FLAG_PAUSE |
                                                  AUD_CMD_FLAG_LOADCODE);
            VERIFY(_AudWaitEvent(u1DspId, u1DecId, &u4Event, TRUE, AUD_EVENT_TIMEOUT));

            switch (u4Event)
            {
            case AUD_CMD_PLAY:
                eNewState = AUD_WAIT_PLAY;
                break;

            case AUD_CMD_STOP:
                eNewState = AUD_WAIT_STOP;
                break;

            case AUD_CMD_RESET:
                eNewState = AUD_IDLE;  //  -- DSP Force --
                break;

            case AUD_CMD_LOADCODE:
                eNewState = AUD_DECODE_INIT;
                break;

            case AUD_CMD_PAUSE:
                eNewState = AUD_IDLE;
                AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PAUSE, AUD_OK);
                break;

            case AUD_CMD_RESUME:
                eNewState = AUD_IDLE;
                AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_RESUME, AUD_OK);
                break;

            default:
                eNewState = AUD_IDLE;
            }
            _ChangeAudioState(u1DspId, u1DecId, eNewState);
            break;

        case AUD_DSP_ERROR:
            //we think DSP now already be in some error state, so just let audio driver keep running in case of system halt
            //but never send flow control cmd to DSP to avoid DSP going to strange states thus make DEBUG complicated.

            //LOG(0," AUD Decoder in Error State\n");

            _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_ERROR;
            
            _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_PLAY   |
                                                  AUD_CMD_FLAG_STOP   |
                                                  AUD_CMD_FLAG_RESET  |
                                                  AUD_CMD_FLAG_RESUME |
                                                  AUD_CMD_FLAG_PAUSE |
                                                  AUD_CMD_FLAG_LOADCODE|
                                                  AUD_CMD_FLAG_RESET);
            if (_AudWaitEvent(u1DspId, u1DecId, &u4Event, FALSE, 100))
            {
                AUD_CommandDone(u1DspId, u1DecId,(AUD_DEC_CMD_T)u4Event, (UINT32)AUD_FAIL);       
            }
            
            if (AUD_IsUopCommandDone(u1DspId, u1DecId))
            {
                _ChangeAudioState(u1DspId, u1DecId, AUD_WAIT_STOP);

                // Audio stop process
                _AudWaitStopProcess(u1DspId, u1DecId);
            }

            break;
        default:
            ASSERT(0);
        } // end of switch
    }//end of _fgDrvThreadGo    //lint -e{818}
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

void _AUD_AtvAvInitialSync(BOOL fgStable)
{
#ifndef CC_MT5391_AUD_3_DECODER  //gallen 0521
    if ((_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) && (fgStable))
    {
        VERIFY(AUD_DRVCmd(AUD_DSP0, AUD_DEC_MAIN, AUD_CMD_AVSYNC));
    }
#else
    if ((_arAudDecoder[AUD_DSP0][AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) && (fgStable))
    {
        VERIFY(AUD_DRVCmd(AUD_DSP0, AUD_DEC_AUX, AUD_CMD_AVSYNC));
#ifndef CC_AUD_DDI
        VERIFY(AUD_DRVCmd(AUD_DSP0, AUD_DEC_MAIN, AUD_CMD_AVSYNC));
#endif
    }
    else if ((_arAudDecoder[AUD_DSP0][AUD_DEC_THIRD].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) && (fgStable) && _IsTriOnlyDecMode())
    {
        VERIFY(AUD_DRVCmd(AUD_DSP0, AUD_DEC_THIRD, AUD_CMD_AVSYNC));
    }
#endif

    LOG(1,"#### GOT _AUD_AtvAvInitialSync###### fgStable : %d\n", fgStable);

    _afgAtvVideoDisplayFlag[AUD_DEC_MAIN] = fgStable;
#ifdef CC_MT5391_AUD_3_DECODER
    _afgAtvVideoDisplayFlag[AUD_DEC_AUX] = fgStable;
    _afgAtvVideoDisplayFlag[AUD_DEC_THIRD] = fgStable;
#endif
}

void _AUD_AtvAvInitialSync1(BOOL fgStable)
{
    if ((_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_LINE_IN) && (fgStable))
    {
        //VERIFY(AUD_DRVCmd(AUD_DEC_AUX, AUD_CMD_AVSYNC));
#ifdef CC_AUD_DDI
        VERIFY(AUD_DRVCmd(AUD_DSP0, AUD_DEC_MAIN, AUD_CMD_AVSYNC));
#endif
    }

    _fgAVAudioOnly = TRUE;

    LOG(1,"#### GOT _AUD_AtvAvInitialSync1###### _fgAVAudioOnly:%d\n", _fgAVAudioOnly);
}

static BOOL _AudCheckDataRate(UINT8 u1DecId, UINT32 u4PesCnt) //Andrew : the function is for DD Test.
{
    static UINT32 u4LastDataBitrate = 0;
    UINT32 u4DataBitrate;
    static UINT32 u4PrevPesCnt = 0;
    static UINT32 u4PrevStc = 0;
    INT32 i4STCDiff = 0;
    STC_CLOCK rStc;

    u4DataBitrate = DSP_GetDataRate(AUD_DEC_MAIN);

    if (STC_GetSrc(AUD_DspQryStcID(u1DecId), &rStc) == STC_VALID)
    {
        if (u4PesCnt==1)  //The first packet comes, reset prevoius STC
        {
           u4PrevStc = rStc.u4Base;
           i4STCDiff = 0;
        }
        else
        {
            i4STCDiff = (INT32)(rStc.u4Base - u4PrevStc);
            if (i4STCDiff < 0)
            {
                i4STCDiff = -i4STCDiff;
            }
            u4PrevStc = rStc.u4Base;
        }
        LOG(5, "## u4PesCnt = %d, stc = (%d, %d), u4STCDiff=%d\n",u4PesCnt, u4PrevStc,rStc.u4Base ,i4STCDiff);
    }
    else
    {
        i4STCDiff = 0;
    }


    if (u4LastDataBitrate == 0xfffff) //640k to 32k, Stop and Play
    {
        LOG(5, "## _AudCheckDataRate,  Set Play\n");
        VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_PLAY));
        u4LastDataBitrate = u4DataBitrate;
        u4PrevPesCnt = u4PesCnt;
        return TRUE;
    }
    else if ((u4DataBitrate == 0x7d00) && (u4LastDataBitrate == 0x9c400)) //from 640k to 32k
    {
        LOG(5, "## _AudCheckDataRate,  u4LastDataBitrate == %x, u4DataBitrate =%x, Set Stop\n",u4LastDataBitrate,u4DataBitrate);
        VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_STOP));
        u4LastDataBitrate = 0xfffff;
        return TRUE;
    }
    else
    {
        if ((i4STCDiff > (90000*2)) &&
            (u4PesCnt == (u4PrevPesCnt+1)) &&
            (u4DataBitrate == 0x7d00))                  //If audio packet received interval > 2 sec, and comes a 32k audio packet...
        {
            LOG(1, "## _AudCheckDataRate, two audio packet received interval > 2 sec, Set Stop\n",u4LastDataBitrate,u4DataBitrate);
            VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_STOP));
            u4LastDataBitrate = 0xfffff;
            return TRUE;
        }

        // Nothing to do ...
        u4PrevPesCnt = u4PesCnt;
        u4LastDataBitrate = u4DataBitrate;
        return FALSE;
    }
}

static void _DtvResetLockStatus(UINT8 u1DecId)
{
    AudDecNfyFct  pfAudDecNfy = NULL;
    UINT32        u4Tmp = 0;

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);
   AUD_DEC_ID_VALIDATE(u1DecId);  //Fix Klocwork warning


    _afgDtvLock[u1DecId] = FALSE;
    _afgDtvPesLock[u1DecId] = TRUE;  // Initial keep pes as lock for the case of
                                     // initial of changing channel case

    _au4LastPesCnt[u1DecId] = 0;
   // _au4LastFrameCnt[u1DecId] = 0;        // Should not reset last frame count to 0, frame count is read from dsp dram and not changed.
   // _au4FrameCnt[u1DecId] = 0;
    _au4ErrCnt[u1DecId] = 0;
    _au1NoErrCntIncrease[u1DecId] = 0;
    _au1NoOkCntIncrease[u1DecId] = OK_CNT_INC_TH;
    _au1NoErrCntAcc[u1DecId] = 0;       
    _au1NoOkCntAcc[u1DecId] = 0;
    _au1ErrEnable[u1DecId] = FALSE;
    _au4LastErrCnt[u1DecId] = 0;  
    _au4PesCnt[u1DecId] = 0;
    _au2NoFrameCntIncrease[u1DecId] = 0;
    _au2NoPesCntIncrease[u1DecId] = 0;
    _aeDecStatus[u1DecId] = AUD_DECODE_UNKNOWN;
    _fgAudOutReady[u1DecId] = FALSE;
    if(_au1MuteEnable == TRUE)
    {
        _au1MuteEnable = FALSE;
#ifdef CC_AUD_DDI
      AudAoutGainCtrl(0,0,0x1000,AOUT_VOL_SETP_4DB); 
#endif
    }
    // Notify middleware decoder status
    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_DECODE_STATUS_CHG,
                    (UINT32)_aeDecStatus[u1DecId], u4Tmp);
    }

    //disable clear MM-related flags (done in swdmx_drvif.c, 20090915)
    if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA)
    { // If not MMP, clear conflict flag by audio driver
        AUD_MM_Set_Dec_Fmt_Conflict(u1DecId,FALSE);
    }
    //AUD_MM_Set_Dec_Fmt_Scramble(u1DecId,FALSE);
}

static void _DtvLockCheck(UINT8 u1DecId)
{
    AudDecNfyFct  pfAudDecNfy = NULL;
    UINT32        u4Tmp = 0;
    AUD_DECODE_STATUS_T  eDecStatus;
    UINT16        u2NoFrameCntIncreaseMax;
    UINT16        u2CheckProfileThreshold;
    UINT16        u2CheckErrorThreshold;

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

    // Playing network tv ?
#ifdef ENABLE_MULTIMEDIA
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        (_SWDMX_IsNetworkSrc(0)))

    {
        u2NoFrameCntIncreaseMax = NET_DTV_DECODE_ERR_CHECK_THRESHOLD;
        u2CheckProfileThreshold = NET_MM_AAC_CHECK_PROFILE_THRESHOLD;
        u2CheckErrorThreshold = NET_MM_AC3_CHECK_ERROR_THRESHOLD;
    }
    else
#endif
    {
        u2NoFrameCntIncreaseMax = DTV_DECODE_ERR_CHECK_THRESHOLD;
        u2CheckProfileThreshold = MM_AAC_CHECK_PROFILE_THRESHOLD;
        u2CheckErrorThreshold = MM_AC3_CHECK_ERROR_THRESHOLD;
    }

    // Get latest frame count and pes count
    _au4FrameCnt[u1DecId] = u4ReadD2RInfo_AputBank(AUD_DSP0, u1DecId);
    _au4ErrCnt[u1DecId] = u4AudGetDecErrorCnt(AUD_DSP0, u1DecId);
    _au4PesCnt[u1DecId]   = _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount;

    // Lock status update
    // Wait until decoding first frame, then change to lock
    if ((_au4LastPesCnt[u1DecId] != _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount) &&
        (_au4FrameCnt[u1DecId] > 0))
    {
        _afgDtvLock[u1DecId] = TRUE;
    }
    else
    {
        _afgDtvLock[u1DecId] = FALSE;
    }

    // If no frame decoder more than threshold, then change to un-lock
    if(_au4PesCnt[u1DecId]>0)
    {
        if (_au4LastFrameCnt[u1DecId] == _au4FrameCnt[u1DecId])
        {
            _au2NoFrameCntIncrease[u1DecId] ++;
            if (_au2NoFrameCntIncrease[u1DecId] > u2NoFrameCntIncreaseMax)
            {
                _afgDtvLock[u1DecId] = FALSE;
                _au2NoFrameCntIncrease[u1DecId] = u2NoFrameCntIncreaseMax;
            }
        }
        else
        {
            _au2NoFrameCntIncrease[u1DecId] = 0;
            _fgAudOutReady[u1DecId] = TRUE;
        }
    }
    if (AUD_IsMixSndClipPlay(0) && _au1MuteEnable == TRUE)
    {
#ifdef CC_AUD_DDI
        AudAoutGainCtrl(0,0,0x1000,AOUT_VOL_SETP_4DB); 
#endif
    }
    else if ( (u1DecId == AUD_DEC_AUX) && (DSP_GetAoutEnable(AUD_DSP0, u1DecId) == TRUE) && (_arAudDecoder[AUD_DSP0][AUD_DEC_AUX].eDecFormat == AUD_FMT_AAC) )
    {   
    
        if (_au4LastErrCnt[u1DecId] != _au4ErrCnt[u1DecId])
        {
            _au1NoErrCntAcc[u1DecId] += 1;        
            _au1NoOkCntAcc[u1DecId] = 0;
        }
        else
        {
              _au1NoOkCntAcc[u1DecId] += 1;
              _au1NoErrCntAcc[u1DecId] = 0;        
        }
        if( _au1NoErrCntAcc[u1DecId] > ERR_ENABLE_TH)
        {
            _au1ErrEnable[u1DecId] = TRUE;      
        }
        else if(_au1NoOkCntAcc[u1DecId] > ERR_DISABLE_TH)
        {
              _au1ErrEnable[u1DecId] = FALSE;       
        }   
        
        if(_au1ErrEnable[u1DecId] == TRUE)
        {
        
            // If no Error count increase => good
            if (_au4LastErrCnt[u1DecId] == _au4ErrCnt[u1DecId])
            {
                _au1NoOkCntIncrease[u1DecId] += 1;                
            }
            else
            {
                _au1NoErrCntIncrease[u1DecId] += 1;
                _au1NoOkCntIncrease[u1DecId]=0;                
            }
            if(_au1NoOkCntIncrease[u1DecId] >= OK_CNT_INC_TH)
            {
#ifdef CC_AUD_DDI
                AudAoutGainCtrl(0,0,0x1000,AOUT_VOL_SETP_4DB);   
#endif
                _au1NoErrCntIncrease[u1DecId] = 0;                      
            }
            if(_au1NoErrCntIncrease[u1DecId] >= ERR_CNT_INC_TH)
            {
#ifdef CC_AUD_DDI
                AudAoutGainCtrl(0,0,0,AOUT_VOL_SETP_4DB);   
#endif
                _au1MuteEnable = TRUE;              
            }
            _au4LastErrCnt[u1DecId] = _au4ErrCnt[u1DecId];
        }
    }
    // Audio pes status update
    if (_au4LastPesCnt[u1DecId] == _au4PesCnt[u1DecId])
    {
        _au2NoPesCntIncrease[u1DecId] ++;
        if (_au2NoPesCntIncrease[u1DecId] > DTV_LOCK_CHECK_THRESHOLD)
        {
            _afgDtvPesLock[u1DecId] = FALSE;
            _au2NoPesCntIncrease[u1DecId] = DTV_LOCK_CHECK_THRESHOLD;
        }
    }
    else
    {
        _au2NoPesCntIncrease[u1DecId] = 0;
        _afgDtvPesLock[u1DecId] = TRUE;
    }

    _au4LastFrameCnt[u1DecId] = _au4FrameCnt[u1DecId];
    _au4LastPesCnt[u1DecId] = _au4PesCnt[u1DecId];

#if 0
    LOG(5, " $$$$$$ Lock (%d) PesLock (%d), Cnt = %d  $$$$$$\n",
             _afgDtvLock[u1DecId], _au4PesCnt[u1DecId], _au4FrameCnt[u1DecId]);
#endif

#ifdef CC_AUD_DDI
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) &&
             (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_MPEG) &&
             (AUD_Is_MMDecFmtConflict(AUD_DEC_MAIN)))
    {
        if (AUD_IsDecoderPlay(AUD_DSP0,u1DecId))
        {
            AUD_DSPCmdStop(AUD_DSP0,u1DecId);
        }
        AUD_DRVSetDecodeType(AUD_DSP0,u1DecId, AUD_STREAM_FROM_DIGITAL_TUNER, AUD_FMT_MP3);
        AUD_DSPCmdPlay(AUD_DSP0,u1DecId);
        AUD_MM_Set_Dec_Fmt_Conflict(AUD_DEC_MAIN,FALSE);
        LOG(0,"@@@@@@@@@@@@@@@DTV_Is_Mp3 not MPEG Actually@@@@@@@@@@@@@\n.");
    }
#endif
/*
#ifdef ENABLE_MULTIMEDIA
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
             (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_MP3) &&
             (AUD_Is_MMDecFmtConflict(AUD_DEC_MAIN)))
    {
        if (AUD_IsDecoderPlay(AUD_DSP0,u1DecId))
        {
            AUD_DSPCmdStop(AUD_DSP0,u1DecId);
        }
        AUD_DRVSetDecodeType(AUD_DSP0,u1DecId, AUD_STREAM_FROM_MULTI_MEDIA, AUD_FMT_MPEG);
        AUD_DSPCmdPlay(AUD_DSP0,u1DecId);
        AUD_MM_Set_Dec_Fmt_Conflict(AUD_DEC_MAIN,FALSE);
        LOG(0,"@@@@@@@@@@@@@@@MMP Audio_Is_MPEG not Mp3 Actually@@@@@@@@@@@@@\n.");
    }
#else
     if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) &&
             (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_MP3) &&
             (AUD_Is_MMDecFmtConflict(AUD_DEC_MAIN)))
    {
        if (AUD_IsDecoderPlay(AUD_DSP0,u1DecId))
        {
            AUD_DSPCmdStop(AUD_DSP0,u1DecId);
        }
        AUD_DRVSetDecodeType(AUD_DSP0,u1DecId, AUD_STREAM_FROM_DIGITAL_TUNER, AUD_FMT_MPEG);
        AUD_DSPCmdPlay(AUD_DSP0,u1DecId);
        AUD_MM_Set_Dec_Fmt_Conflict(AUD_DEC_MAIN,FALSE);
        LOG(0,"@@@@@@@@@@@@@@@DTV Audio_Is_MPEG not Mp3 Actually@@@@@@@@@@@@@\n.");
    }
#endif
*/

#ifdef DATA_DISC_WMA_SUPPORT
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        ((_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_WMA) || (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_WMAPRO) ) &&
        (DSP_GetWmaErrorCode(u1DecId) != 0))        //0 means normal stop
    {
        //WMA goes here because error
        eDecStatus = AUD_DECODE_NOT_SUPPORT;
    }
    else if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_AAC) &&
        (_afgDtvPesLock[u1DecId]) &&
        (_au2NoFrameCntIncrease[u1DecId] >= u2CheckProfileThreshold) &&
        ((!_fgAudOutReady[u1DecId]) || (_au2NoFrameCntIncrease[u1DecId] >= u2NoFrameCntIncreaseMax) ) &&
        (!DSP_GetAoutEnable(AUD_DSP0, u1DecId)))
    {
        //AAC profile not support
        eDecStatus = AUD_DECODE_NOT_SUPPORT;
    }
    else if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_AC3) &&
        ((_afgDtvPesLock[u1DecId]) || (_au4ErrCnt[u1DecId] >= u2CheckErrorThreshold)) &&
        (_au2NoFrameCntIncrease[u1DecId] >= u2CheckProfileThreshold) &&
        ((!_fgAudOutReady[u1DecId]) || (_au2NoFrameCntIncrease[u1DecId] >= u2NoFrameCntIncreaseMax) ) &&
        (!DSP_GetAoutEnable(AUD_DSP0, u1DecId)))
    {
        //AC3 profile not support
        eDecStatus = AUD_DECODE_NOT_SUPPORT;
    }
    else if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_DTS) &&
        (_au2NoFrameCntIncrease[u1DecId] > MM_DTS_CHECK_ERROR_THRESHOLD) &&
        (!DSP_GetAoutEnable(AUD_DSP0, u1DecId)))
    {
        eDecStatus = AUD_DECODE_NOT_SUPPORT;
    }
    else if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_DRA) &&
        (_au2NoFrameCntIncrease[u1DecId] > MM_DTS_CHECK_ERROR_THRESHOLD) &&
        (!DSP_GetAoutEnable(AUD_DSP0, u1DecId)))
    {
        eDecStatus = AUD_DECODE_NOT_SUPPORT;
    }
    else if (((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) || (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)) &&
             (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_MPEG) &&
             (AUD_Is_MMDecFmtConflict(AUD_DEC_MAIN)))
    {
        eDecStatus = AUD_DECODE_FMT_CONFLICT;
    }
    else if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
             ((_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_MPEG) ||
              (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_MP3) ||
              (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_AC3) ||
              (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_AAC)) &&
             (AUD_Is_MMDecFmtScramble(u1DecId)))
    {
        eDecStatus = AUD_DECODE_NOT_SUPPORT;  
    }
    #if 1///def CC_VORBIS_SUPPORT ///CC_VORBIS_SUPPORT_ERR
    else if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_VORBISDEC) &&
        (DSP_GetVorbisErrorCode(u1DecId) != 0))
    {
        ///LOG(0,"@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n.");
        //Vorbis goes here because header parsing error
        eDecStatus = AUD_DECODE_NOT_SUPPORT;
    }
    #endif
    else if (_afgDtvPesLock[u1DecId] || _afgDtvLock[u1DecId])
#else
    if (_afgDtvPesLock[u1DecId] || _afgDtvLock[u1DecId])
#endif
    {
        eDecStatus = AUD_DECODE_NORMAL;
        if ((!_afgDtvLock[u1DecId]) && (_au2NoFrameCntIncrease[u1DecId] >= DTV_DECODE_ERR_CHECK_THRESHOLD))
        {
            //receive PES but cannot decode anymore => decode error
            eDecStatus = AUD_DECODE_ERROR;
        }

    }
    else
    {
        if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
            (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode != AV_SYNC_FREE_RUN))  //when free run, dtvonplaythread may turn on before PES is sent
        {
#ifdef CC_AUD_SKYPE_SUPPORT
            AUD_DrvMMNoDataNotify(u1DecId);
#else
            AUD_DrvMMNoDataNotify(AUD_DEC_MAIN);
#endif
        }
        if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
        {
            LOG(1, "DTV input DMX(SDEC) doesn't feed data to AUD_DEC%d\n", u1DecId);
        }
        eDecStatus = AUD_DECODE_NO_DATA;
    }
#ifndef CC_ENABLE_AOMX 
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) && 
        ((eDecStatus == AUD_DECODE_NOT_SUPPORT) ||(eDecStatus == AUD_DECODE_NO_DATA) ||
        (eDecStatus == AUD_DECODE_ERROR)) && Dsp_WaitResume(AUD_DSP0, u1DecId) &&
        (eDecStatus != _aeDecStatus[u1DecId]) && (_arAudDrvState[AUD_DSP0][u1DecId] == AUD_ON_PLAY))
    {
        LOG(5, "Resume done for AUD_DECODE_NOT_SUPPORT\n");
        _fgAudMMnotifySWDMX[u1DecId] = TRUE;
        AUD_DrvMMAoutEnableNotify(u1DecId, TRUE);
    }
#endif    
    // Notify middleware decoder status
    VERIFY(x_sema_lock(_arAudDrvStateSema[AUD_DSP0][u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);

    if ((pfAudDecNfy != NULL) && (eDecStatus != _aeDecStatus[u1DecId]) && (_arAudDrvState[AUD_DSP0][u1DecId] == AUD_ON_PLAY))
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_DECODE_STATUS_CHG,
                    (UINT32)eDecStatus, u4Tmp);
        LOG(5, "_DtvLockCheck, AUD_COND_AUD_DECODE_STATUS_CHG, u1DecId(%d), eDecStatus(%d => %d)\n", u1DecId, _aeDecStatus[u1DecId], eDecStatus);
#ifdef CC_MT5391_AUD_3_DECODER
        if (_IsDualDecMode()&&(u1DecId == AUD_DEC_AUX)&&
           (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eDecFormat == AUD_FMT_PCM))
        {
            pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_DECODE_STATUS_CHG,
                        (UINT32)eDecStatus, u4Tmp);
        }
#endif
    }

    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DSP0][u1DecId]) == OSR_OK);

    _aeDecStatus[u1DecId] = eDecStatus;

}

static void _MMEOSNotify(UINT8 u1DecId)
{
    AudDecNfyFct  pfAudDecNfy = NULL;

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));

    if (pfAudDecNfy != NULL)
    {
        // Notify middleware decoder status
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecId, AUD_COND_MM_EOS, (UINT32)(_arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes.fgForward), 0);
        LOG(5, "MM send EOS notify done, decoder_id = %d, fgForward = %d\n", u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes.fgForward);
    }
}

#ifdef CC_AUD_BITRATE_CHG_NOTIFY
BOOL _AUD_SetBitRateChangedPts(UINT32 u4Pts)
{
    UINT16 u2Idx;
    
    if (u4Pts == 0)
        return FALSE;

    u2Idx = (_au2InIdx+1) % BIT_RATE_CHANGED_PTS_Q_SIZE;
    if (u2Idx != _au2OutIdx)
    {
        _au4BitRateChangedPts[_au2InIdx] = u4Pts;
        _au2InIdx = u2Idx;

        LOG(7, "Input bitrate changed pts (0x%x).\n", u4Pts);
        return TRUE;
    }
    else
    {
        LOG(5, "Input bitrate changed pts fail!\n");
    }

    return FALSE;
}

BOOL _AUD_GetBitRateChangedPts(UINT32 *pu4Pts)
{    
    ASSERT(pu4Pts != NULL);

    if (_au2InIdx != _au2OutIdx)
    {
        *pu4Pts = _au4BitRateChangedPts[_au2OutIdx];
       // _au2OutIdx = (_au2OutIdx+1) % BIT_RATE_CHANGED_PTS_Q_SIZE;

        return TRUE;
    }

    return FALSE;
}

static void _AudTriggerNextBitRateChangedPts(void)
{
    if (_au2InIdx != _au2OutIdx)
    {
        _au2OutIdx = (_au2OutIdx+1) % BIT_RATE_CHANGED_PTS_Q_SIZE;
    }
}

static void _AudResetBitRateChangedPts(void)
{
    _au2InIdx = 0;
    _au2OutIdx = 0;
}

static void _AudBitRateChangedNotify(void)
{
    UINT32 u4ChangedPts = 0;
    UINT32 u4ReadPtr, u4WritePtr;
    UINT64 u8CurrPts;
    UINT32 u4CurrPts;
    UINT8 u1DecId = AUD_DEC_MAIN;
    AudDecNfyFct   pfAudDecNfy = NULL;

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);
    
    // Get bitrate changed pts
    if ((pfAudDecNfy != NULL) && (_AUD_GetBitRateChangedPts(&u4ChangedPts)))
    {        
        // Get current pts
        _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
        UNUSED(u4WritePtr);
        AUD_MMFindSyncInfo(u1DecId, &u8CurrPts, u4ReadPtr);
        u4CurrPts = (UINT32)u8CurrPts;
        
        // Send bitrate changed notify if pts reach
        if ((u4CurrPts >= u4ChangedPts) ||
            ((0xFFFFFFFF - u4ChangedPts + u4CurrPts) < 0x80000000))
        {
            _AudTriggerNextBitRateChangedPts();
            
            pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_BIT_RATE_CHG, 0, 0);
        }
    }
}
#endif

//added by ling
#define AVSYNC_BLIND_COUNT 10
#define ADDR_D2RC_CMM_END_SIZE     (0x000C9) ///080C9h
static void _AudDtvOnPlayThread(void* pvArg)
{
    UINT32 u4CmmSize;
    UINT32 u4Count = 0;
#if 0    //APROC debug only
    static HAL_TIME_T PrevTime;
    HAL_TIME_T CurrTime;
    static UINT32 u4DebugCnt = 0;
    UINT32 u4_0x80;    
    UINT32 u4_0x82;
    UINT32 u4_0xC3;    
    UINT32 u4_0xC0;
    UINT32 u4_0xC1;        
#endif

    UNUSED(pvArg);

#if defined(CC_SUPPORT_STR) && defined(LINUX_TURNKEY_SOLUTION)
    set_freezable();
#endif

    while (1)
    {
        UINT8 u1DecId;
        UINT32 u4ReadPtr;
        UINT32 u4WritePtr;
        
        if(_u4ThreadTime<10)
            _u4ThreadTime++;
        if(_u4ThreadTime==10)
        {
            if( _u4DecframeCount != u4ReadD2RInfo_AputBank(AUD_DSP0, AUD_DEC_AUX))
            {
                isEsExist1 = 1;
                _u4DecframeCount=u4ReadD2RInfo_AputBank(AUD_DSP0, AUD_DEC_AUX);
            }
            else
            {
                isEsExist1 = 0;
            }
            _u4ThreadTime=0;
        }

        u4CmmSize = DSP_COMMON_DRAM_READ (AUD_DSP0, ADDR_D2RC_CMM_END_SIZE);
        if(u4CmmSize > vGetFBMCMMSize())
        { 
            LOG(0, "Error!!! DSP CMM Size (0x%x) larger than FBM CMM Size (0x%x)\n", u4CmmSize,vGetFBMCMMSize());
            //ASSERT(0);
        }

        // Bitrate changed notify
    #ifdef CC_AUD_BITRATE_CHG_NOTIFY
        //vDspBitRateChangedNotify();
        _AudBitRateChangedNotify();
    #endif
    
        for (u1DecId = AUD_DEC_MAIN; u1DecId < AUD_DEC_MAX; u1DecId++)
        {
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
            if (_afgDtvOnPlayThreadGo[u1DecId])
            {
                if (bIsSupportAproc())
                {
                    AUD_Aproc_Avsync_Ctrl(u1DecId);
                }
            }
#endif
#if defined(CC_MT5391_AUD_3_DECODER) && !defined(CC_DUAL_AUD_DEC_SUPPORT) && !defined(CC_AUD_DDI)
            if ((u1DecId == AUD_DEC_MAIN )&& (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
              //  continue;   //EU DTV dec0 can't do following processing
#endif

            if (_afgDtvOnPlayThreadGo[u1DecId])
            {
                // updating read/write pointer to DMX and DSP
                if (_DmxPidIdxValid(u1DecId))
                {
                    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
                    //LOG(10, "rp, wp = (0x%08x, 0x%08x)\n",u4ReadPtr, u4WritePtr);
                    //Read pointer may be out of range of AFIFO, SKIP it.
                    if ((u4ReadPtr >= DSP_PHYSICAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId))) && 
                        (u4ReadPtr < DSP_PHYSICAL_ADDR(u4GetAFIFOEnd(AUD_DSP0, u1DecId))))
                    {
                        if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), u4ReadPtr, 0))
                        {
                            LOG(7, "3. Buffer pointer range checking...\n");
                        }
                    }
                }
                // DTV lock check
                _DtvLockCheck(u1DecId);
                AUD_DrvLogStcDiff(u1DecId);
                u4Count++;

                if (((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)) ||
                    (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER &&_fgMMPlusAD[u1DecId] ==TRUE ))
                {
                    if((AUD_IsSourceFromWFD(u1DecId)) && ( u4Count == APLL_ADJUST_PERIOD/AUD_PTR_UPDATE_TIMEOUT))
                    {
                        u4Count = 0;
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER) 
                        _AUD_AprocEnableVCXO(u1DecId, TRUE); 
#endif
                        AUD_WFD_DspAsrcSetSmartVCXO(u1DecId);
                    } 
                    
                    if (_arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes.fgEOS == TRUE)
                    {
                        if ((AUD_IsFifoEmpty(AUD_DSP0, u1DecId)))
                        {
                            if (_arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes.fgEOS)
                            {
                                //for MM auto stop. we need to stop dsp once FIFO emtpy true.. 20111208
                                LOG(1, "Aud FIFO empty driver auto stop decoder(%d) \n",u1DecId); 
                                AUD_WaitDspFlush(u1DecId);
                                if (_fgMMSet[u1DecId]==FALSE)
                                {
                                    _MMEOSNotify(u1DecId);
                                    _fgMMSet[u1DecId]=TRUE;
                                }
                            }
                        }
                        if (!AUD_IsMMAoutEnable(AUD_DEC_MAIN))
                        {
                             x_thread_delay(45);
                        }
                    }
#ifndef CC_AUD_ARM_RENDER                    
                    AUD_DrvCheckSpeedTime(u1DecId);
#endif
                }
            }
        }
        if ((_afgDtvOnPlayThreadGo[AUD_DEC_AUX]) && (_arAudDecoder[AUD_DSP0][AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
        {
            if( _arAudDecoder[AUD_DSP0][AUD_DEC_AUX].u4ReceiveValidPesCount != u4PrevValidPes)
            {
                u4NoPESCount1++;
                if(u4NoPESCount1 >= 15)
                {
                    isEsExist = TRUE;
                    u4NoPESCount1 = 20;
                }
                u4NoPESCount=0;
                u4PrevValidPes = _arAudDecoder[AUD_DSP0][AUD_DEC_AUX].u4ReceiveValidPesCount;
            }
            else
            {
                u4NoPESCount1 = 0;
                u4NoPESCount++;
                if(u4NoPESCount >= 5)
                {
                    isEsExist = FALSE;
                    u4NoPESCount = 10;
                }
            }
        }

        // If AD played normally, update main decoder "master volume" and "channel volume"
        if (_afgDtvOnPlayThreadGo[AUD_DEC_THIRD])
        {
            if (_bPanFadeUpdateEn == TRUE)
            {
                //fade
                AUD_DspAdFadeVolumeUpdate(AUD_DEC_MAIN, AUD_CH_ALL);

                //pan
                AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT);
                AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT);
            }
        }
        else
        {
            AUD_DspAdFadeVolumeSet(0);
            AUD_DspAdFadeVolumeUpdate(AUD_DEC_MAIN, AUD_CH_ALL);
            AUD_DspAdPanVolumeSet(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT, 0);
            AUD_DspAdPanVolumeSet(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT, 0);
            AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_RIGHT);
            AUD_DspAdPanVolumeUpdate(AUD_DEC_THIRD, AUD_CH_FRONT_LEFT);
        }

#if 0 //APROC debug only
        if ((u4DebugCnt % ((100)/AUD_PTR_UPDATE_TIMEOUT)) == 0)
        {
            HAL_GetTime(&CurrTime);
            u4_0xC0 = u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_AENV_2, 0x0));
            u4_0xC1 = u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_AENV_2, 0x1));
            u4_0xC3 = u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_AENV_2, 0x3));        
            u4_0x80 = u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_AENV_0, 0x0));
            u4_0x82 = u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_AENV_0, 0x2));        
        
            LOG(4, "\n(%d.%03d) - %d ms \n", CurrTime.u4Seconds, CurrTime.u4Micros, 
                ((CurrTime.u4Seconds-PrevTime.u4Seconds)*1000+(CurrTime.u4Micros-PrevTime.u4Micros)/1000) );
            LOG(4, "(SRC_M,Src_In,Aput,Aout)(0x%08x,0x%08x,0x%08x,0x%08x)\n", u4_0x82, u4_0xC3, u4_0xC0, u4_0xC1);
    
            PrevTime = CurrTime;
        }
        u4DebugCnt++;
#endif

        x_thread_delay(AUD_PTR_UPDATE_TIMEOUT);
        //LOG(1, "  ############################ _AudDtvOnPlayThread exit ####################\n");
#if defined(CC_SUPPORT_STR) && defined(LINUX_TURNKEY_SOLUTION)
        try_to_freeze();
#endif
    }
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


void AUD_DetectionSyncSema(BOOL fgLock)
{
    if (fgLock)
    {
        INT32 i4Ret;

        // Real lock ..
        i4Ret = x_sema_lock_timeout(_hDetectionSyncSema, DUAL_DEC_SYNC_TIME_OUT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

        if (i4Ret == OSR_TIMEOUT)
        {
            LOG(1, "AUD_DetectionSyncSema timeout\n");
        }
    }
    else
    {
        VERIFY(x_sema_unlock(_hDetectionSyncSema) == OSR_OK);
    }

}

#if 0   // Unused
void AUD_ScartOutInit(void)
{
    AUD_FMT_T eAudFmt;

    if ((_arAudDrvState[AUD_DSP0][AUD_DEC_MAIN] != AUD_ON_PLAY) &&
        (_arAudDrvState[AUD_DSP0][AUD_DEC_AUX] != AUD_ON_PLAY))
    {
#ifdef CC_AUD_DDI
        eAudFmt = _AudAtvFmtDetection(AUD_DEC_AUX);
#else
        eAudFmt = _AudAtvFmtDetection();
#endif
        UNUSED(AUD_SetDecType(AUD_DSP0, AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, eAudFmt));
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));
        UNUSED(AUD_DSPCmdPlay(AUD_DSP0, AUD_DEC_AUX));
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));
    }
}
#endif

BOOL AUD_IsSpdifInSrc(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_SPDIF)
        ||(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_FEEDER)
        ||(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

void AUD_UopCommandDone(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4Command)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    LOG(7, "Aud dsp(%d)dec(%d) AUD_UopCommandDone 0x%x done\n", u1DspId, u1DecId, u4Command);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[u1DspId][u1DecId]) == OSR_OK);
}

#ifdef FIX_AUD_UOP_TIMEOUT
void AUD_WaitUOPLockSema(UINT8 u1DspId, UINT8 u1DecId)
{
    INT32 i4Ret;

    // Try lock to make sure the semaphore was locked
    i4Ret = x_sema_lock(_ahSemaUopComDone[u1DspId][u1DecId], X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}
#endif

BOOL AUD_IsUopCommandDone(UINT8 u1DspId, UINT8 u1DecId)
{
    INT32 i4Ret;
    
    // Try lock to check whether the semaphore was locked
    i4Ret = x_sema_lock(_ahSemaUopComDone[u1DspId][u1DecId], X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

    if(i4Ret == OSR_OK)
    {
        VERIFY(x_sema_unlock(_ahSemaUopComDone[u1DspId][u1DecId]) == OSR_OK);
    }

    return (i4Ret == OSR_OK) ? TRUE : FALSE;
}

INT32 AUD_WaitUopCommandDone(UINT8 u1DspId, UINT8 u1DecId)
{
    INT32 i4Ret;

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    LOG(7, "Aud dec(%d) thread sleep to WaitUopCommand  to be done by DSP \n",u1DecId);

    #ifndef FIX_AUD_UOP_TIMEOUT
    // Try lock to make sure the semaphore was locked
    i4Ret = x_sema_lock(_ahSemaUopComDone[u1DspId][u1DecId], X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    #endif

    // Real lock ..
    i4Ret = x_sema_lock_timeout(_ahSemaUopComDone[u1DspId][u1DecId], 5000);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

    LOG(7, "Aud dsp(%d) dec(%d) thread wake up for UopCommandDone\n",u1DspId, u1DecId);
    if (i4Ret == OSR_TIMEOUT)
    {
        LOG(1, "Aud wait command TIMEOUT\n");
        #ifdef DSP_FORCE_RESET_ENABLE
        if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom != AUD_STREAM_FROM_HDMI)
        {
            u4DspResueTrigger = 1;  //  -- DSP Force --
        }
        #endif
    }
    return i4Ret;
}

void AUD_ResetDspReadPtr(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    LOG(5, "AUD_ResetDspReadPtr, u1DecId (%d), wp (0x%x)\n", u1DecId, _arAudPes[u1DecId].u4Wp);
    
    if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), _arAudPes[u1DecId].u4Wp, 0))
    {
        LOG(7, "Buffer pointer range checking...\n");
    }
    
    vDspSetFifoReadPtr(u1DspId, u1DecId, _arAudPes[u1DecId].u4Wp);
}

/******************************************************************************
* Function: AUD_SetStartPts
*
* Description:
*      For video to set start Pts when first I picture arrived.
*      For AV-Sync implementation
*
* Inputs: -
*      u1DecId: Audio decoder id (0: first decoder, 1: second decoder)
*      u8Pts:   Pts of first I picture
* Outputs: -
*
* Returns: -
*      TRUE: Succeed
*      FALSE: Fail
******************************************************************************/
BOOL AUD_SetStartPts(UINT8 u1DecId, UINT32 u4Pts)
{
    UINT8 u1DecIdTmp;
    LOG(0, "##### Dec(%d) AUD_SetStartPts = 0x%08x\n", u1DecId, u4Pts);
#ifndef CC_MT5391_AUD_3_DECODER
    if (u1DecId > AUD_DEC_AUX)
#else
    if (u1DecId > AUD_DEC_THIRD)
#endif
    {
        LOG(1, "Error: AUD_SetStartPts: invalud Dec ID\n", u1DecId, u4Pts);
        return FALSE;
    }
    else /* (ucDevId == AUD_DEC_MAIN) || (ucDevId == AUD_DEC_AUX) */
    {
#ifdef CC_AUD_SMALL_PTS_CHECK
        u4OldPts1 = 0;
#endif
#if 0//def CC_MT5391_AUD_3_DECODER
        //koro: for DTV case, must set decoder 1
        if (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER &&
            _arAudDecoder[AUD_DSP0][AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER &&
            u1DecId == AUD_DEC_MAIN)
        {
            u1DecId = AUD_DEC_AUX;    //set decoder 1
        }
#endif
#ifndef CC_MT5391_AUD_3_DECODER
        for (u1DecIdTmp = 0; u1DecIdTmp <=AUD_DEC_AUX; u1DecIdTmp++)
#else
        for (u1DecIdTmp = 0; u1DecIdTmp <=AUD_DEC_THIRD; u1DecIdTmp++)
#endif
        {
            //set all decoders (video only set one time, but dec2, dec3 both need this info!
            if(u4Pts == (UINT32)(-1))
            {
                _afgFirstIPicArrived[u1DecIdTmp] = FALSE;
            }
            else
            {
                _arAudDecoder[AUD_DSP0 ][u1DecIdTmp].u4StartPts = u4Pts;
                _afgFirstIPicArrived[u1DecIdTmp] = TRUE;
            _arAudDecoder[AUD_DSP0 ][u1DecIdTmp].u4PesCntAfterIPicReceived = 0;
            }
        }

        return TRUE;
    }
}

void AUD_PanFadePesUpdateEn(BOOL bEnable)
{
    _bPanFadeUpdateEn = bEnable;
}

void AUD_SetAdPanEnable(BOOL bEnable)
{
    _bPanEnable = bEnable;
}

void AUD_SetAdFadeEnable(BOOL bEnable)
{
    _bFadeEnable = bEnable;
}

#if 0   // Unused
BOOL AUD_GetAdPanEnable(void)
{
    return _bPanEnable;
}

BOOL AUD_GetAdFadeEnable(void)
{
    return _bFadeEnable;
}
#endif

BOOL AUD_SendAudioPes(const DMX_AUDIO_PES_T* prPes)
{
    UINT8 u1DecId;
    DMX_AUDIO_PES_T rPes = {0};
    STC_CLOCK rStc;

    static UINT32 u4DeltaPts[3] = {0,0,0};
    BOOL fgDulplicatedPts[3] = {FALSE,FALSE,FALSE};
    static UINT32 u4LastPts[3] = {0,0,0};
    static UINT32 u4StartDecWp[3] = {0,0,0};
    static UINT32 u4PrevWp[3] = {0,0,0};
//    static UINT32 u4StcPtsDiff[3] = {0,0,0};
//    static UINT32 u4CurMaxStcPtsDiff[3] = {0,0,0};
    static UINT32 u4PrevPts[3] = {0,0,0};  //DTV_AVSYNC_ENH

    UINT32 u4WpOffset;
    UINT32 u4ShiftDecWp;
    UINT32 u4DeltaWp;

    static UINT8 u1ADPanPrev = 0;
    static UINT8 u1ADFadPrev = 0;
    
    UINT8 u1Category;

    #if 0//def CC_AUD_DDI
    UINT8 u1PathID;
    #endif

    if (prPes == NULL)
    {
        return FALSE;
    }

    // Check valid DevicedId;
    AUD_DEC_ID_VALIDATE_3(prPes->u1DeviceId); //Klocwork .. Only DecId 0,1,2 use this function.
    // Update DeviceId <--> PidIdx
#ifdef AUD_DECODER3_SELF_TEST
    if (u1ForceDecId != AUD_DEC_NUM)    //parson MM3
    {
        _arAudDecoder[AUD_DSP0][u1ForceDecId].u1DmxPidIdx = prPes->u1PidIndex;
        u1DecId = u1ForceDecId;
    }
    else
    {
        _arAudDecoder[AUD_DSP0][prPes->u1DeviceId].u1DmxPidIdx = prPes->u1PidIndex;
        u1DecId = prPes->u1DeviceId;
    }
#else
    _arAudDecoder[AUD_DSP0][prPes->u1DeviceId].u1DmxPidIdx = prPes->u1PidIndex;
    u1DecId = prPes->u1DeviceId;
#endif

    #if 0//def CC_AUD_DDI
    if((_IsDualDecMode() && (AUD_DEC_AUX == u1DecId)) || (_IsTriOnlyDecMode() && (AUD_DEC_THIRD == u1DecId)))
    {
        u1PathID = 0;
    }
    else
    {
        u1PathID = 1;
    }
    #endif

    UNUSED(u4DeltaPts);
    UNUSED(u4PrevPts);

    x_memcpy((VOID *)VIRTUAL((UINT32)&rPes), (const VOID *)prPes, sizeof(DMX_AUDIO_PES_T));
    x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudPes[u1DecId]), (const VOID *)prPes, sizeof(DMX_AUDIO_PES_T));

    // Check if dulplicated PTS
    fgDulplicatedPts[u1DecId] = (rPes.u4Pts == u4LastPts[u1DecId]) ? TRUE : FALSE;

    if(_arAudDecoder[AUD_DSP0][u1DecId].u4ReceiveValidPesCount == 0)
    {
        #if 0  //if SWDMX send data first, then audio play. The sync info before audio play will be cleaned.
        AUD_MMQueueSyncInfo(u1DecId,0,0,TRUE); //Reset MM Sync info
        AUD_MMQueueTickInfo(u1DecId,0,0,TRUE); //Reset MM Sync info
        #endif
        AUD_DspSetWithVideo(u1DecId, FALSE);  //Avoid dsp reset sram but driver doesn't reset share memory.
        #if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        AUD_Aproc_Skip_Pts(u1DecId, 0, TRUE);
        #endif        
    }
    // Check if PTS = 0
    if (rPes.u4Pts == 0)
    {
            fgDulplicatedPts[u1DecId] = TRUE;
            rPes.u4Pts = u4LastPts[u1DecId];
         }
    else
    {
        u4LastPts[u1DecId] = rPes.u4Pts;
        _arAudDecoder[AUD_DSP0][u1DecId].u4ReceiveValidPesCount++;
    }

    if (!fgDulplicatedPts[u1DecId])
    {
        _LogPts(rPes.u1PidIndex, u1DecId, &rPes);
    }

    AUD_DEC_ID_VALIDATE_3(u1DecId);
    _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount ++;
    if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_SEND_PTS) == AUD_DBG_SYNC_SEND_PTS)
    {
        //LOG(7, "Audio receive pes count = %d\n", _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount);
    }
    
   // Store current PES data
    x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes), (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));

   //check if this PES is for EOS flag
   if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA )||
       (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER &&_fgMMPlusAD[u1DecId] ==TRUE ))
   {
       if (_arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes.fgEOS == TRUE)
       {
            if ( !_afgIssuePlayComToDsp[AUD_DSP0][u1DecId]  )
            {
                VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_AVSYNC));
                _afgIssuePlayComToDsp[AUD_DSP0][u1DecId] = TRUE;
            }
            if ((_arAudDecoder[AUD_DSP0][u1DecId].eDecState == AUD_DEC_STOP) &&
                (_arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount == 1))
            {
                LOG(2, "Notify EOS to AP/MW when the first PES is EOS PES\n");
                _MMEOSNotify(u1DecId);
            }
            LOG(5, "Audio received EOS Pes u1DecId=%d, fgForward=%d\n", u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes.fgForward);
            return TRUE;
       }
   }

    if ((prPes->u1DeviceId == AUD_DEC_THIRD) && (prPes->fgContainAD == TRUE)) // && (_aeDecStatus[u1DecId] == AUD_DECODE_NORMAL))
    {
        if (_bPanFadeUpdateEn == TRUE)
        {
            if (_bFadeEnable == TRUE)
            {
                AUD_DspAdFadeVolumeSet(rPes.u1ADFad);

                if (prPes->u1ADFad != u1ADFadPrev)
                {
                    LOG(3, "AUD_SendAudioPes prPes->u1ADFad = 0x%x, prPes->u1ADPan = 0x%x\n", prPes->u1ADFad, prPes->u1ADPan);
                    u1ADFadPrev = prPes->u1ADFad;
                }
            }
            else
            {
                AUD_DspAdFadeVolumeSet(0);
            }
            if (_bPanEnable == TRUE)
            {
                if (prPes->u1ADPan != u1ADPanPrev)
                {
                    LOG(3, "AUD_SendAudioPes prPes->u1ADFad = 0x%x, prPes->u1ADPan = 0x%x\n", prPes->u1ADFad, prPes->u1ADPan);
                    u1ADPanPrev = prPes->u1ADPan;
                }

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
				// Sign extension & channel setting handle in APROC, driver can ignore it, so use the same channel parameters...
				AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_LEFT, rPes.u1ADPan);
				AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_RIGHT, rPes.u1ADPan);
#else
                if (rPes.u1ADPan & 0x80)
                {
                    if (rPes.u1ADPan < 0xeb)
                    {
                        rPes.u1ADPan = 0xeb;
                    }
                    rPes.u1ADPan = 0x100 - rPes.u1ADPan;

                    AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_RIGHT, rPes.u1ADPan);
                    AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_LEFT, 0);
                }
                else
                {
                    if (rPes.u1ADPan > 0x15)
                    {
                        rPes.u1ADPan = 0x15;
                    }

                    AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_LEFT, rPes.u1ADPan);
                    AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_RIGHT, 0);
                }
#endif
            }
            else
            {
                AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_LEFT, 0);
                AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_RIGHT, 0);
            }
        }
    }

#ifdef LINUX_TURNKEY_SOLUTION
    if (_hAudDataDumpThreadState[u1DecId] == 1)
    {
        if (_arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount == 1)
        { // Reset read pointer
            u4AudDataCurRp[u1DecId]= VIRTUAL(u4GetAFIFOStart(AUD_DSP0, u1DecId));
            LOG(5, "_AudDataDump %d resets read pointer.\n", u1DecId);
        }
        VERIFY(x_sema_unlock(_hAudDataArrived[u1DecId]) == OSR_OK);
    }
#endif

    // Address translation, translate virtual address to DSP internal address
    rPes.u4Wp = DSP_INTERNAL_ADDR(rPes.u4Wp);

#ifndef CC_AUD_ARM_RENDER
    _AUD_DMX_UpdateWPtr(u1DecId, rPes.u4Wp);
#endif

    // FIXME !!
    if (_uAudioDelayPTSFlag[u1DecId])
    {
        rPes.u4Pts += PTS_AC3_CNT * (_uAudioDelayPTSFlag[u1DecId]);
    }

    if (_fgDDBannerOn)  //Andrew : For DD Test only.
    {
        if (_AudCheckDataRate(u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount))
        {
            return TRUE;
        }
    }


    // Laihui: add for update audio PTS and PTS-STC Delta to DBS
    // Store current PTS to Dbs
#ifdef CC_GET_PCR_PTS_INFO
    _prDbase->rPts.u4AudioPts = _arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Pts;
    {
        STC_CLOCK rStc;
        UINT32 u4CurrentStcPtsDiff;

        if ((STC_GetSrc(AUD_DspQryStcID(u1DecId), &rStc) == STC_VALID) &&  (!fgDulplicatedPts[u1DecId]))
        {
            if (_arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Pts < rStc.u4Base)
            {
                u4CurrentStcPtsDiff = rStc.u4Base - _arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Pts;
            }
            else
            {
                u4CurrentStcPtsDiff = _arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Pts - rStc.u4Base;
            }
            //Store STC-PTS diff to Dbs
            _prDbase->rPts.u4AudioPtsStcDelta = u4CurrentStcPtsDiff;
            LOG(11, "  DEC(%d) rPes.u4Wp = %08x, rPes.u4Pts=%08x \n", u1DecId, rPes.u4Wp ,rPes.u4Pts);
            LOG(11, "  _arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Pts=%08x\n", _arAudDecoder[u1DspId][u1DecId].rCurrentPes.u4Pts);
            LOG(11, "  rStc.u4Base=%08x\n", rStc.u4Base);
            LOG(11, "  u4CurrentStcPtsDiff=%08x\n", u4CurrentStcPtsDiff);
        }
    }
#endif

    // DTV Spdif copyright check Every Received audio PES.
#ifdef CC_MT5391_AUD_3_DECODER
    if ((((u1DecId == AUD_DEC_AUX)&&(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))&& _IsDualDecMode())||
         ((u1DecId == AUD_DEC_THIRD)&&(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)&&_IsTriOnlyDecMode()))
#else
    if ((u1DecId == AUD_DEC_MAIN)&&(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
#endif
    {
        if (AUD_DspGetSpdifUpdateMode(SPDIF_REG_TYPE_USER, AUD_STREAM_FROM_DIGITAL_TUNER) == IEC_CHANNEL_STATUS_BY_SOURCE)
        {
            if ((_arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes.fgCopyright == FALSE) && 
                    (AUD_DspGetSpdifCopyright(SPDIF_REG_TYPE_USER, AUD_STREAM_FROM_DIGITAL_TUNER) == IEC_COPY_NEVER))
            {
                // Set copy free
                u1Category = AUD_DspGetSpdifCategoryCode(SPDIF_REG_TYPE_USER, AUD_STREAM_FROM_DIGITAL_TUNER);
                u1Category = u1Category & IEC_CATEGORY_LBIT_MASK;
                AUD_DspSpdifSetInfo(AUD_STREAM_FROM_DIGITAL_TUNER, IEC_COPY_FREELY, u1Category);
                LOG (3, "DTV SPDIF by source: %d to Copy Free\n", _arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes.fgCopyright);
            }
            if ((_arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes.fgCopyright == TRUE) && 
                    (AUD_DspGetSpdifCopyright(SPDIF_REG_TYPE_USER, AUD_STREAM_FROM_DIGITAL_TUNER) == IEC_COPY_FREELY))
            {
                // Set copy never
                u1Category = AUD_DspGetSpdifCategoryCode(SPDIF_REG_TYPE_USER, AUD_STREAM_FROM_DIGITAL_TUNER);
                u1Category = u1Category | IEC_CATEGORY_LBIT;
                AUD_DspSpdifSetInfo(AUD_STREAM_FROM_DIGITAL_TUNER, IEC_COPY_NEVER, u1Category);                
                LOG (3, "DTV SPDIF by source: %d to Copy Never\n", _arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes.fgCopyright);
            }
        }
    
    }

    // Store first PES data
    if ((_arAudDecoder[AUD_DSP0][u1DecId].u4ReceiveValidPesCount == 1) && (!fgDulplicatedPts[u1DecId]))
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[AUD_DSP0][u1DecId].rFirstReceivePes), (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));

        u4DeltaPts[u1DecId] = 0;

        if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
            (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
        {
            _ChkAudioOnlyControl(u1DecId, TRUE);        //Andrew Wen 2007/6/22 Audio only program set sync mode to free run.
        }
        u4PrevWp[u1DecId] = DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId));
    }
    else if (_arAudDecoder[AUD_DSP0][u1DecId].u4ReceiveValidPesCount == 2)
    {
        u4DeltaPts[u1DecId] = rPes.u4Pts - _arAudDecoder[AUD_DSP0][u1DecId].rFirstReceivePes.u4Pts;
        LOG(9, "Delta PTS (%d) = %u\n",u1DecId,u4DeltaPts);
    }
    else if (_arAudDecoder[AUD_DSP0][u1DecId].u4ReceiveValidPesCount == 3)
    {
        // Store first avaible PES data
        x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[AUD_DSP0][u1DecId].rFirstAvaibleReceivePes), (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));
    }

    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
        (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
    {
        // Check if audio only program
        _ChkAudioOnlyControl(u1DecId, FALSE);  //Andrew Wen 2007/6/22 Audio only program set sync mode to free run.
    }

    if (rPes.u4Wp >= u4PrevWp[u1DecId])
    {
        u4DeltaWp = rPes.u4Wp - u4PrevWp[u1DecId];
    }
    else
    {
        u4DeltaWp = DSP_INTERNAL_ADDR(u4GetAFIFOEnd(AUD_DSP0, u1DecId))- DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)) + rPes.u4Wp - u4PrevWp[u1DecId];
    }

    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_LPCM))
    {
        if (AUD_IsLpcmFmtPS()) //only for program stream.
        {
            AUD_SetLPCMFmt(u1DecId, rPes.u4Info);
        }
    }
    if ((STC_GetSrc(AUD_DspQryStcID(u1DecId), &rStc) == STC_VALID) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode != AV_SYNC_FREE_RUN) &&
        (!fgDulplicatedPts[u1DecId]))
    {
        if (_IsPtsValid(rPes.u1PidIndex, u1DecId, &rPes))
        {
            if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
            {
                if (rPes.u4Pts == _arAudDecoder[AUD_DSP0][u1DecId].u4StartPts)
                {
                    AUD_MMQueueSyncInfo(u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].u4StartPts, DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)), FALSE);
                }
                else
                {
                //MM queue av sync info
                    AUD_MMQueueSyncInfo(u1DecId, (UINT64)(rPes.u4Pts), u4PrevWp[u1DecId], FALSE);
                }
#ifdef TIME_SHIFT_SUPPORT
                //MM queue tick info
                AUD_MMQueueTickInfo(u1DecId, rPes.u4TickNum, rPes.u4Wp, FALSE);
#endif
            }
        }
     }

    // Check if send PTS to DSP
    // Move shift wp out of the original condition. because in wait_play state we also have pts sent in and wp also need shift.
    if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
    {
        u4ShiftDecWp = u4PrevWp[u1DecId];
    }
    else
    {
        if (_DMX_GetInterruptThreshold(DMX_PID_TYPE_ES_AUDIO, &u4WpOffset))
        {
            if (u4DeltaWp < u4WpOffset)
            {
                u4WpOffset = u4DeltaWp;
            }
            u4ShiftDecWp = rPes.u4Wp - (u4WpOffset);
            if (DSP_INTERNAL_ADDR(u4ShiftDecWp) < DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)))
            {
                u4ShiftDecWp = DSP_INTERNAL_ADDR(u4GetAFIFOEnd(AUD_DSP0, u1DecId)) -
                    (DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)) - u4ShiftDecWp);
            }
        }
        else
        {
            u4ShiftDecWp = rPes.u4Wp;
        }
    }
    u4PrevWp[u1DecId] = rPes.u4Wp;
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    if (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode != AV_SYNC_FREE_RUN)
    {
        //Don't insert invalid PTS to queue.
        if (_IsPtsValid(rPes.u1PidIndex, u1DecId, &rPes) && _IsPtsAlign(u1DecId, u4ShiftDecWp))
        {
            if (!fgDulplicatedPts[u1DecId])
            {
                // Put the Start PTS into queue for MM
                if(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
                {
                    if (!(_arAudDecoder[AUD_DSP0][u1DecId].fgInsrtStartPts))
                    {
                        LOG(5," \n !!!!11 Insert Start PTS !!!!!!!!11 \n");
                        AUD_DrvPtsQueueInsert(u1DecId,DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)),_arAudDecoder[AUD_DSP0][u1DecId].u4StartPts);
                        _arAudDecoder[AUD_DSP0][u1DecId].fgInsrtStartPts = TRUE;
                    }
                }        
                AUD_DrvPtsQueueInsert(u1DecId, u4ShiftDecWp, rPes.u4Pts); //PTSQueue_V2        
            }
            else
            {
                AUD_DrvPtsQueueUpdateWp(u1DecId, u4ShiftDecWp); //new_arm_pts_table
            }
        }
    }
            
    _AUD_DMX_UpdateWPtr(u1DecId, rPes.u4Wp);     //After PTS Q is inserted, then update Wp for DSP.
#endif //defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
    
    if ((_arAudDrvState[AUD_DSP0][u1DecId] == AUD_ON_PLAY) &&
        (STC_GetSrc(AUD_DspQryStcID(u1DecId), &rStc) == STC_VALID) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode != AV_SYNC_FREE_RUN) &&
        (!fgDulplicatedPts[u1DecId]))
    {
        if (_IsPtsValid(rPes.u1PidIndex, u1DecId, &rPes) && _IsPtsAlign(u1DecId, u4ShiftDecWp))
        {

            if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
            {
                //u4ShiftDecWp = rPes.u4Wp; //This line makes DTV ShiftWp wrong. So I mark it.
                AUD_MMQueueSyncInfo(u1DecId, (UINT64)(rPes.u4Pts), u4ShiftDecWp, FALSE);
            }
#ifndef CC_AUD_ARM_RENDER            
            if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_SEND_PTS) == AUD_DBG_SYNC_SEND_PTS)
            {
#ifdef CC_AUD_SMALL_PTS_CHECK
                LOG(5," PTS=%x, OldPts=%x Wp=%x, Swp=%x, delta=%d, stc=%x\n", rPes.u4Pts, u4OldPts1,rPes.u4Wp, u4ShiftDecWp,u4DeltaWp ,rStc.u4Base);
#else
                LOG(5," PTS=%x, Wp=%x, Swp=%x, delta=%d, stc=%x\n", rPes.u4Pts, rPes.u4Wp, u4ShiftDecWp,u4DeltaWp ,rStc.u4Base);
#endif
            }
#endif //CC_AUD_ARM_RENDER            

#ifdef CC_AUD_SMALL_PTS_CHECK
        if(( rPes.u4Pts < u4OldPts1 ) && (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
        {

            if ( DSP_ResetPts(AUD_DSP0, u1DecId) == DSP_BUSY)
            {
                LOG(5, "DSP_ResetPts fail, DecId = %d\n", u1DecId);

            if (DSP_SendPts(AUD_DSP0, u1DecId, rPes.u4Pts, u4ShiftDecWp) == DSP_FAIL)
            {
                LOG(7, "DspSendPts fail, DecId = %d\n", u1DecId);
            }

        }
            else
            {

                if (DSP_SendPts(AUD_DSP0, u1DecId, rPes.u4Pts, u4ShiftDecWp) == DSP_FAIL)
                {
                    LOG(7, "DspSendPts fail, DecId = %d\n", u1DecId);
                }
                else
                {
                    u4OldPts1 = rPes.u4Pts;
                }
    }
            //vWriteDspSram32(SRAM_PTS_PRS_PNT,0x0);
            //vWriteDspSram32(SRAM_PTS_PUT_BANK,0xffff0000);
            LOG(5, "@@@@ Found Small PTS @@@@@\n");
        }
        else
        {
            if (DSP_SendPts(AUD_DSP0, u1DecId, rPes.u4Pts, u4ShiftDecWp) == DSP_FAIL)
            {
                LOG(7, "DspSendPts fail, DecId = %d\n", u1DecId);
            }
            else
            {
                u4OldPts1 = rPes.u4Pts;
            }

        }

#else
            if (u4PrevPts[u1DecId] > rPes.u4Pts) //DTV_AVSYNC_ENH
            {
                //LOG(5, "\n ?????????????????? WRAP Around ???????????????\n");
                AUD_SetAudControlVdpQuota(u1DecId, 0);
            }
            u4PrevPts[u1DecId] = rPes.u4Pts;

            if (DSP_SendPts(AUD_DSP0, u1DecId, rPes.u4Pts, u4ShiftDecWp) == DSP_FAIL)
            {
                LOG(7, "DspSendPts fail, DecId = %d\n", u1DecId);
            }
#endif
        }
    }

#if 0
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eSynMode != AV_SYNC_FREE_RUN) && (!fgDulplicatedPts[u1DecId]))
    {
        // Put the Start PTS into queue for MM
        if(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
        {
            if (!_afgIssuePlayComToDsp[AUD_DSP0][u1DecId])
            {
                AUD_DrvPtsQueueInsert(u1DecId,DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)),_arAudDecoder[AUD_DSP0][u1DecId].u4StartPts);
            }
        }
        AUD_DrvPtsQueueInsert(u1DecId, u4ShiftDecWp, rPes.u4Pts); //PTSQueue_V2
    }
#endif    

    // Check if send Play command to DSP ?
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eDecState == AUD_DEC_PLAYING) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode != AV_SYNC_FREE_RUN) &&
        (!_afgIssuePlayComToDsp[AUD_DSP0][u1DecId]))
    {
        //Andrew Wen 07/2/9 : The delay of dmux updating write pointer must be taken into consideration.
        if (!fgDulplicatedPts[u1DecId])
        {
            //koro: multi-media decode from begin of AFIFO
            if((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
                (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode != AV_SYNC_SLAVE))
            {
                u4StartDecWp[u1DecId] = DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId));
            }
            else if (_DMX_GetInterruptThreshold(DMX_PID_TYPE_ES_AUDIO, &u4WpOffset))
            {
                if((_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_WMA)||(_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_LPCM) ||(_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_PCM)) //for WMA /LPCM format, didn't shift write pointer!
                {
                    u4WpOffset = 0 ;
                }


                if (u4DeltaWp < u4WpOffset)
                {
                    u4WpOffset = u4DeltaWp;
                }
                u4StartDecWp[u1DecId] = rPes.u4Wp - u4WpOffset;
                if (DSP_INTERNAL_ADDR(u4StartDecWp[u1DecId]) < DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)))
                {
                    u4StartDecWp[u1DecId] = DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId));
                }
            }
            else
            {
                u4StartDecWp[u1DecId] = rPes.u4Wp;
            }

            LOG(7, "Dec(%d) u4StartDecWp=%x, rPes.u4Wp = %x, rPes.u4Pts=%x\n", u1DecId, u4StartDecWp[u1DecId],rPes.u4Wp ,rPes.u4Pts);
        }

        if((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) &&
           (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode != AV_SYNC_SLAVE))
        {
            x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[AUD_DSP0][u1DecId].rFristDecodePes),
                     (const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
            vDspSetFifoReadPtr(AUD_DSP0, u1DecId, DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)));
            DSP_SetStartPtsToShm(AUD_DSP0, u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].u4StartPts, DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)));

            VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_AVSYNC));
            _afgIssuePlayComToDsp[AUD_DSP0][u1DecId] = TRUE;
#ifdef CC_AUD_SMALL_PTS_CHECK
            if (u4OldPts1 > 0)
            {
                u4OldPts1 = 0;
            }
#endif
            LOG(5, "===> Dec (%d) Start PTS (0x%x), PTS(0x%x), PRS (0x%x), Aud PES cnt (%d) for multimedia\n",
                u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].u4StartPts, rPes.u4Pts,  u4StartDecWp[u1DecId], _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount);
        }
#ifdef CC_MT5391_AUD_3_DECODER
        else if ((u1DecId == AUD_DEC_THIRD) && ((_afgIssuePlayComToDsp[AUD_DSP0][AUD_DEC_AUX]) || (_afgIssuePlayComToDsp[AUD_DSP0][AUD_DEC_MAIN])) &&
                    (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) && !_IsTriOnlyDecMode())
        {
            x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[AUD_DSP0][u1DecId].rFristDecodePes),
                     (const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
            vDspSetFifoReadPtr(AUD_DSP0, u1DecId, u4StartDecWp[u1DecId]);
            DSP_SetStartPtsToShm(AUD_DSP0, u1DecId, rPes.u4Pts, u4StartDecWp[u1DecId]);
            VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_AVSYNC));
            _afgIssuePlayComToDsp[AUD_DSP0][u1DecId] = TRUE;
            LOG(0, "===> AD on Dec (%d) First decode PTS (0x%x), AudPTS = 0x%x, Aud PES cnt (%d) \n",
                u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].rFristDecodePes.u4Pts, (UINT32)(rPes.u4Pts & 0xffffffff), _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount);
        }
#endif
        else if (_afgFirstIPicArrived[u1DecId])
        {
            UINT64 u8DeltaPts = 0;

            u8DeltaPts = (_arAudDecoder[AUD_DSP0][u1DecId].u4StartPts >= (UINT32)(rPes.u4Pts & 0xffffffff)) ?
                         (_arAudDecoder[AUD_DSP0][u1DecId].u4StartPts - (UINT32)(rPes.u4Pts & 0xffffffff)) :
                         ((UINT32)(rPes.u4Pts & 0xffffffff) - _arAudDecoder[AUD_DSP0][u1DecId].u4StartPts);
            _arAudDecoder[AUD_DSP0][u1DecId].u4PesCntAfterIPicReceived ++;

            if ((u8DeltaPts < PTS_THRESHOLD)     ||
                (u8DeltaPts > PTS_THRESHOLD_MAX) ||
                ((UINT32)(rPes.u4Pts & 0xffffffff) >= _arAudDecoder[AUD_DSP0][u1DecId].u4StartPts)  ||
                 (_arAudDecoder[AUD_DSP0][u1DecId].u4PesCntAfterIPicReceived > (PES_CNT_THRESHOLD/2)))
            {
                x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[AUD_DSP0][u1DecId].rFristDecodePes), (const VOID *)&rPes, sizeof(DMX_AUDIO_PES_T));
                vDspSetFifoReadPtr(AUD_DSP0, u1DecId, u4StartDecWp[u1DecId]);
                DSP_SetStartPtsToShm(AUD_DSP0, u1DecId, (UINT32)(rPes.u4Pts & 0xffffffff), u4StartDecWp[u1DecId]);
                VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_AVSYNC));
                _afgIssuePlayComToDsp[AUD_DSP0][u1DecId] = TRUE;
                LOG(0, "===> Dec (%d) StartPTS = 0x%x, AudPTS = 0x%x, Aud PES cnt after I (%d) \n",
                    u1DecId, (UINT32)(_arAudDecoder[AUD_DSP0][u1DecId].u4StartPts & 0xffffffff),
                    (UINT32)(rPes.u4Pts & 0xffffffff),
                    _arAudDecoder[AUD_DSP0][u1DecId].u4PesCntAfterIPicReceived);
            }
            else
            {
                vDspSetFifoReadPtr(AUD_DSP0, u1DecId, u4StartDecWp[u1DecId]);
                if (!(fgDspReadPtrSet(AUD_DSP0, u1DecId, u4StartDecWp[u1DecId])))
                {
                    LOG(1, "Dec(%d) fgDspReadPtrSet fail\n", u1DecId);
                    //ASSERT(0);
                }
                if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), DSP_PHYSICAL_ADDR(u4StartDecWp[u1DecId]), 0))
                {
                    LOG(7, "5. Buffer pointer range checking...\n");
                }
                LOG(11, "1. Update RP = 0x%08x\n", DSP_PHYSICAL_ADDR(rPes.u4Wp));
            }
        }        // Fixed ES id,
                 // Video always use ES id 0. Workaround for some POP case, middleware doesn't set av-sync mode correctly
#if 0//def CC_AUD_DDI
        else if ((0 == u1PathID) && (MPV_IsDisplay(u1PathID) || (VDP_GetTimeShfitStartPlay(0) || _fgPvrPause)))
#else
        else if (MPV_IsDisplay(_u1VideoFrameFlag))          
#endif
        {
            LOG(5, "_u1VideoFrameFlag = %d (0: main, 1: sub)\n", _u1VideoFrameFlag);
            x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[AUD_DSP0][u1DecId].rFristDecodePes),
                     (const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
            vDspSetFifoReadPtr(AUD_DSP0, u1DecId, u4StartDecWp[u1DecId]);
            DSP_SetStartPtsToShm(AUD_DSP0, u1DecId, rPes.u4Pts, u4StartDecWp[u1DecId]);
            VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_AVSYNC));
            _afgIssuePlayComToDsp[AUD_DSP0][u1DecId] = TRUE;
            LOG(0, "===> Dec (%d) First decode PTS (0x%x), AudPTS = 0x%x, Aud PES cnt (%d) \n",
                u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].rFristDecodePes.u4Pts, (UINT32)(rPes.u4Pts & 0xffffffff), _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount);
        }
        else /* (_afgFirstIPicArrived[ucDevId] == FALSE) */
        {
            UINT32 u4DeltaPts = 0;
            UINT32 u4FifoStart;
            UINT32 u4FifoEnd;
            DSP_GetAFIFOPhysicalAddr(AUD_DSP0, u1DecId, &u4FifoStart, &u4FifoEnd);

            if (_arAudDecoder[AUD_DSP0][u1DecId].u4ReceiveValidPesCount >= PES_VALID_CNT)
            {
                u4DeltaPts = ((UINT32)(_arAudDecoder[AUD_DSP0][u1DecId].rFirstAvaibleReceivePes.u4Pts & 0xffffffff) >= (UINT32)(rPes.u4Pts & 0xffffffff)) ?
                             ((UINT32)(_arAudDecoder[AUD_DSP0][u1DecId].rFirstAvaibleReceivePes.u4Pts & 0xffffffff) - (UINT32)(rPes.u4Pts & 0xffffffff)) :
                             ((UINT32)(rPes.u4Pts & 0xffffffff) - (UINT32)(_arAudDecoder[AUD_DSP0][u1DecId].rFirstAvaibleReceivePes.u4Pts & 0xffffffff));
            }
            else
            {
                u4DeltaPts = 0;
            }

            // Light: DTV takes longer time to wait for I frame.
            // Since we will update Rp to Demux, we don't need to consider FIFO full condition.
            // Wait for I frame for longer time since H.264 I frame comes late very often.
            if (((_arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount > PES_CNT_THRES_I_FRAME)&&(!gfgFirstTimeout1)) ||
                ((_arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount > PES_CNT_THRES_I_FRAME_1st)&&(gfgFirstTimeout1)) ||
                ((u4DeltaPts > PTS_THRESHOLD_MAX)&&(u4DeltaPts < PTS_THRESHOLD_NOT_VALID)))//||
                //((DSP_PHYSICAL_ADDR(u4FifoEnd) - DSP_PHYSICAL_ADDR(rPes.u4Wp)) < DTV_FIFO_FULL_THRESHOLD))
            {
                /* No recieve I pic after a long time */
                x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[AUD_DSP0][u1DecId].rFristDecodePes), (const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
                vDspSetFifoReadPtr(AUD_DSP0, u1DecId, u4StartDecWp[u1DecId]);
                DSP_SetStartPtsToShm(AUD_DSP0, u1DecId, (UINT32)(rPes.u4Pts & 0xffffffff), u4StartDecWp[u1DecId]);
                VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_AVSYNC));
                _afgIssuePlayComToDsp[AUD_DSP0][u1DecId] = TRUE;
                LOG(0, "Dec (%d) No recieve I after a long time. Aud PES cnt (%d)\n", u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount);
                LOG(0, "Dec (%d) DeltaPts (0x%x)\n", u1DecId, u4DeltaPts);
                LOG(0, "Dec (%d) First decode PTS (0x%x), Aud PES cnt (%d) \n",
                    u1DecId, (UINT32)(_arAudDecoder[AUD_DSP0][u1DecId].rFristDecodePes.u4Pts & 0xffffffff), _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount);
            }
            else
            {
                if (!fgDulplicatedPts[u1DecId])
                {
                    /*
                    vDspSetFifoReadPtr(u1DecId, u4StartDecWp);
                    if (!(fgDspReadPtrSet(AUD_DSP0, u1DecId, u4StartDecWp)))
                    {
                        LOG(1, "Dec(%d) fgDspReadPtrSet fail\n", u1DecId);
                    }
                    */
                    if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), DSP_PHYSICAL_ADDR(u4StartDecWp[u1DecId]), 0))
                    {
                        LOG(7, "6. Buffer pointer range checking...\n");
                    }
                    LOG(11, "2. Update RP = 0x%08x\n", DSP_PHYSICAL_ADDR(u4StartDecWp[u1DecId]));
                }
            }
        }/* (_afgFirstIPicArrived[ucDevId] == FALSE) */
    }
    else if ((_arAudDecoder[AUD_DSP0][u1DecId].eDecState == AUD_DEC_PLAYING) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode == AV_SYNC_FREE_RUN) &&
        (_fgWaitAvSync[u1DecId]) && (!_afgIssuePlayComToDsp[AUD_DSP0][u1DecId]))
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudDecoder[AUD_DSP0][u1DecId].rFristDecodePes),(const VOID *)&rPes, sizeof(PSR_AUDIO_PES_T));
        vDspSetFifoReadPtr(AUD_DSP0, u1DecId, u4StartDecWp[u1DecId]);
        DSP_SetStartPtsToShm(AUD_DSP0, u1DecId, rPes.u4Pts, u4StartDecWp[u1DecId]);
        VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_AVSYNC));
        _afgIssuePlayComToDsp[AUD_DSP0][u1DecId] = TRUE;
        LOG(0, "===>Send Av sync command in free run mode it is being waited\n");
        LOG(0, "===> Dec (%d) First decode PTS (0x%x), Aud PES cnt (%d) \n",
            u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].rFristDecodePes.u4Pts, _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount);
    }
    else if ((_arAudDecoder[AUD_DSP0][u1DecId].eDecState == AUD_DEC_STOP) && (!_afgIssuePlayComToDsp[AUD_DSP0][u1DecId]) &&
             (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode != AV_SYNC_FREE_RUN) &&
             (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom != AUD_STREAM_FROM_MULTI_MEDIA))  ////koro: multi-media decode from begin of AFIFO
    {
        if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), DSP_PHYSICAL_ADDR(rPes.u4Wp), 0))
        {
            LOG(7, "7. Buffer pointer range checking...\n");
        }
    }

    MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_FRAME_FIRST);

    return TRUE;

}

#ifdef CC_ENABLE_AOMX
#ifndef CC_AUD_ARM_RENDER 
#ifdef CC_53XX_SWDMX_V2
extern void STC_SetStcValue(UCHAR ucStcId, UINT32 u4Stc);
extern UINT32 STC_GetStcValue(UCHAR ucStcId);
extern void STC_StopStc(UCHAR ucStcId);
extern void STC_StartStc(UCHAR ucStcId);
#else
extern void STC_SetStcValue(UINT32 u4Stc);
extern UINT32 STC_GetStcValue(void);
extern void STC_StopStc(void);
extern void STC_StartStc(void);
#endif

static UINT64 g_u8AomxPTS = 0;
static UINT32 g_u4AomxPTS_L = 0;
static UINT32 g_u4AomxPTS_H = 0;
static UINT32 g_u4AomxLastSTC = 0;

void GST_StartSTC(UINT8 stcId)
{
    #ifdef CC_53XX_SWDMX_V2
    STC_StartStc(stcId);
    #else
    STC_StartStc();
    #endif
}

void GST_StopSTC(UINT8 stcId)
{
    #ifdef CC_53XX_SWDMX_V2
    STC_StopStc(stcId);
    #else
    STC_StopStc();
    #endif
}

void GST_SetSTCVal(UINT8 stcId, UINT64 u8Stc)
{
    g_u8AomxPTS = u8Stc;
    g_u4AomxPTS_L = u8Stc & 0xffffffff;
    g_u4AomxPTS_H = (u8Stc>>32) & 0xffffffff;
    g_u4AomxLastSTC = g_u4AomxPTS_L;
    #ifdef CC_53XX_SWDMX_V2
    STC_SetStcValue(stcId, g_u4AomxPTS_L);
    #else
    STC_SetStcValue(g_u4AomxPTS_L);
    #endif
}

UINT64 GST_GetSTCVal(UINT8 stcId)
{
    UINT32 u4STC;
    UINT64 u8STC;
    #ifdef CC_53XX_SWDMX_V2
    u4STC = STC_GetStcValue(stcId);
    #else
    u4STC = STC_GetStcValue();
    #endif

    if((u4STC < g_u4AomxLastSTC) && (g_u4AomxLastSTC > (90000*60)) && (u4STC < (90000*60)))
    {
        g_u4AomxPTS_H++;
    }
    g_u4AomxLastSTC = u4STC;

    u8STC = (((UINT64)g_u4AomxPTS_H)<<32)|((UINT64)u4STC);
    return u8STC;
}
#else
void GST_StartSTC(UINT8 u1DecId)
{
    LOG(0, "GST_StartSTC(%d)\n", u1DecId);
    VERIFY(x_sema_lock(GstTimeStampe[u1DecId].hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    if (!GstTimeStampe[u1DecId].fgStart)
    {
        do_gettimeofday (&GstTimeStampe[u1DecId].nTimeVal);
        GstTimeStampe[u1DecId].fgStart = TRUE; 
    }
    VERIFY(x_sema_unlock(GstTimeStampe[u1DecId].hSema) == OSR_OK); 
}

void GST_StopSTC(UINT8 u1DecId)
{
    struct timeval tv;
    INT32 i4Diff;

    LOG(0, "GST_StopSTC(%d)\n", u1DecId);
    VERIFY(x_sema_lock(GstTimeStampe[u1DecId].hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    
    if (GstTimeStampe[u1DecId].fgStart)
    {
        GstTimeStampe[u1DecId].fgStart = FALSE; 
        do_gettimeofday(&tv);
        i4Diff = (tv.tv_sec - GstTimeStampe[u1DecId].nTimeVal.tv_sec) * 1000000 + 
            (tv.tv_usec - GstTimeStampe[u1DecId].nTimeVal.tv_usec);
        GstTimeStampe[u1DecId].u4PtsLow += (UINT32)u8Div6432((UINT64)i4Diff * 9,100,NULL);
    }
    VERIFY(x_sema_unlock(GstTimeStampe[u1DecId].hSema) == OSR_OK); 
}

void GST_SetSTCVal(UINT8 u1DecId, UINT64 u8Stc)
{
    LOG(0, "GST_SetSTCVal(%d), %lld\n", u1DecId, u8Stc);

    VERIFY(x_sema_lock(GstTimeStampe[u1DecId].hSema, X_SEMA_OPTION_WAIT) == OSR_OK); 
    GstTimeStampe[u1DecId].u4PtsLow = u8Stc & 0xffffffff;;
    GstTimeStampe[u1DecId].u4PtsHigh = (u8Stc>>32) & 0xffffffff;
    GstTimeStampe[u1DecId].u4PtsLowLast = GstTimeStampe[u1DecId].u4PtsLow;
    VERIFY(x_sema_unlock(GstTimeStampe[u1DecId].hSema) == OSR_OK); 
}

void GST_SetPtsVal(UINT8 u1DecId, UINT32 u4Stc)
{
    INT32 i4Ret;
    struct timeval tv;
    INT32 i4Diff;
    UINT32 u4TimeDiff, u4PtsDiff;
        
    //interrupt context: can't sleep
    i4Ret = x_sema_lock(GstTimeStampe[u1DecId].hSema, X_SEMA_OPTION_NOWAIT);
    if (i4Ret == OSR_OK)
    {
        if (GstTimeStampe[u1DecId].fgStart)
        {
            do_gettimeofday(&tv);
            i4Diff = (tv.tv_sec - GstTimeStampe[u1DecId].nTimeVal.tv_sec) * 1000000 + 
                (tv.tv_usec - GstTimeStampe[u1DecId].nTimeVal.tv_usec);
            u4TimeDiff = (UINT32)u8Div6432((UINT64)i4Diff * 9,100,NULL);
            if (u4Stc >= GstTimeStampe[u1DecId].u4PtsLow)
            {
                u4PtsDiff = u4Stc - GstTimeStampe[u1DecId].u4PtsLow;
            }
            else
            {
                u4PtsDiff = GstTimeStampe[u1DecId].u4PtsLow - u4Stc;
            }
            if (u4PtsDiff >= 10 * u4TimeDiff)
            {
                //SKIP abnormal update PTS
                LOG(2, "Error: update STC error STC(%d), PTS(%d), T_D(%d) \n",u4Stc, GstTimeStampe[u1DecId].u4PtsLow, u4TimeDiff);
            }
            else
            {
                GstTimeStampe[u1DecId].u4PtsLow = u4Stc;
                do_gettimeofday (&GstTimeStampe[u1DecId].nTimeVal);
            }
        }
        else
        {
            LOG(0, "GST_SetPtsVal(%d), %d\n", u1DecId, u4Stc);
        }
        VERIFY(x_sema_unlock(GstTimeStampe[u1DecId].hSema) == OSR_OK);
    }
}

UINT32 GST_GetPtsVal(UINT8 u1DecId)
{
    UINT32 u4PTS;
    struct timeval tv;
    INT32 i4Diff;    

    if (GstTimeStampe[u1DecId].fgStart)
    {
        do_gettimeofday(&tv);
        i4Diff = (tv.tv_sec - GstTimeStampe[u1DecId].nTimeVal.tv_sec) * 1000000 + 
            (tv.tv_usec - GstTimeStampe[u1DecId].nTimeVal.tv_usec);
        u4PTS = GstTimeStampe[u1DecId].u4PtsLow + (UINT32)u8Div6432((UINT64)i4Diff * 9,100,NULL);
    }
    else
    {
        u4PTS = GstTimeStampe[u1DecId].u4PtsLow;
    }
    return u4PTS; 
}

UINT64 GST_GetSTCVal(UINT8 u1DecId)
{
    UINT32 u4STC;
    UINT64 u8STC;

    VERIFY(x_sema_lock(GstTimeStampe[u1DecId].hSema, X_SEMA_OPTION_WAIT) == OSR_OK); 
    u4STC = GST_GetPtsVal(u1DecId);
    if((u4STC < GstTimeStampe[u1DecId].u4PtsLowLast) && 
        (GstTimeStampe[u1DecId].u4PtsLowLast > (4289567296U)/*(0xFFFFFFFF - 90000*60)*/) && 
        (u4STC < (90000*60)))
    {
        LOG(0, "H round plus one, %d, Curret %d, Last %d \n", u1DecId, u4STC, GstTimeStampe[u1DecId].u4PtsLowLast);
        GstTimeStampe[u1DecId].u4PtsHigh++;
    }
    GstTimeStampe[u1DecId].u4PtsLowLast = u4STC;
    VERIFY(x_sema_unlock(GstTimeStampe[u1DecId].hSema) == OSR_OK);

    u8STC = (((UINT64)GstTimeStampe[u1DecId].u4PtsHigh)<<32)|((UINT64)u4STC);
    return u8STC;
}
#endif

BOOL GST_SendAudioPes(const GST_AUDIO_PES_T rPesIn)
{
    UINT8 u1DecId = rPesIn.u1DecId;
    GST_AUDIO_PES_T rPes;
    BOOL fgDulplicatedPts[AUD_DEC_MAX] = {FALSE};
    static UINT64 u8LastPts[AUD_DEC_MAX] = {0};
    
    UINT64 u8Pts32;
    x_memcpy((VOID *)VIRTUAL((UINT32)&rPes), (VOID *)VIRTUAL((UINT32)&rPesIn), sizeof(GST_AUDIO_PES_T));
    // Check if dulplicated PTS
    fgDulplicatedPts[u1DecId] = (rPes.u8Pts == u8LastPts[u1DecId]) ? TRUE : FALSE; 

    u8LastPts[u1DecId] = rPes.u8Pts;
    _arAudDecoder[AUD_DSP0][u1DecId].u4ReceiveValidPesCount++; 
    _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount ++;

    // Address translation, translate virtual address to DSP internal address
    rPes.u4Wp = DSP_INTERNAL_ADDR(rPes.u4Wp);

    if (_arAudDecoder[AUD_DSP0][u1DecId].u4ReceiveValidPesCount == 1)
    {
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        AUD_Aproc_Skip_Pts(u1DecId, 0, TRUE);
#endif
    }

    if(!fgDulplicatedPts[u1DecId])
    {
        u8Pts32 = u8Div6432(rPes.u8Pts * 9,100,NULL);
        GST_MMQueueSyncInfo(u1DecId, rPes.u8Pts, rPes.u4Wp, FALSE,u8Pts32>>32);
        AUD_DrvPtsQueueInsert(u1DecId, rPes.u4Wp, (UINT32)u8Pts32);
    }

    if(!fgDulplicatedPts[u1DecId])
    {
        if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_SEND_PTS) == AUD_DBG_SYNC_SEND_PTS)
        {
            LOG(11,"PST=%lld, PTS=%08x, Wp=%x\n", rPes.u8Pts, (UINT32)u8Pts32, rPes.u4Wp);
        }

        if (DSP_SendPts(AUD_DSP0, u1DecId, (UINT32)u8Pts32, rPes.u4Wp) == DSP_FAIL)
        {
            LOG(11, "DspSendPts fail, DecId = %d\n", u1DecId);
        }
    }

    // Check if send Play command to DSP ?
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eDecState == AUD_DEC_PLAYING) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eSynMode != AV_SYNC_FREE_RUN) &&
        (!_afgIssuePlayComToDsp[AUD_DSP0][u1DecId]))
    {
        vDspSetFifoReadPtr(AUD_DSP0, u1DecId, DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)));
        DSP_SetStartPtsToShm(AUD_DSP0, u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].u4StartPts, DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)));

        VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_AVSYNC));
        _afgIssuePlayComToDsp[AUD_DSP0][u1DecId] = TRUE;
        LOG(1, "===> Dec (%d) First decode PTS (0x%x), Aud PES cnt (%d) for multimedia\n",
            u1DecId, _arAudDecoder[AUD_DSP0][u1DecId].u4StartPts,  _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount);
    }
    return TRUE;
}
#endif

#if (defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_ADECOMX)) || defined(CC_ENABLE_AOMX)
INT32 _AudDumpAudioData (const INT8 *fileName, const INT8 *buf, size_t count);
void GST_AOMX_DumpAudioData(UINT8 u1DecId, const char *addr, UINT32 size, BOOL fgnew)
{
    struct timezone tz;
    struct timeval tv;
    
    //static UINT32 u4Index = 0;
    //CHAR filename[100] ="/mnt/usb/usb1/Drive1/Tiger0";
    static CHAR filename[100] = "/mnt/usb/usb1/Drive1/undefine.bin";
    INT32 ret;

    UNUSED(tz);
    //if(fgnew)
    //{
    //  u4Index++;
    //  if(u4Index == 10)
    //      u4Index=0;
    //}

    //filename[26] = '0' + u4Index;
    //filename[27] = 0;

    if(fgnew)
    {
        do_gettimeofday(&tv);
        LOG(0,  "%s%d%06d.bin", "/mnt/usb/usb1/Drive1/", tv.tv_sec, tv.tv_usec);
        sprintf(filename, "%s%d%06d.bin", "/mnt/usb/usb1/Drive1/", tv.tv_sec, tv.tv_usec);
        LOG(0, "%s\n", filename);
    }
    
    ret = _AudDumpAudioData(filename, (INT8 *)(VIRTUAL((UINT32)addr)), size);
    if (ret < 0)
    {
        LOG (8, "_AudDumpAudioData ERROR!\n");
    }
    return;
}

#endif

#if 0   // Unused
void AUD_GetAudioPes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    x_memcpy((VOID *)VIRTUAL((UINT32)prAudioPes), (const VOID *)&_arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes, sizeof(PSR_AUDIO_PES_T));
}
#endif

void AUD_GetAudioPts(UINT8 u1DecId, UINT32 * prAudioPts)
{
    UINT32 u4ReadPtr;
    UINT32 u4WritePtr;
#ifdef CC_ENABLE_AOMX
    UINT32 u4PTSHigh;
#endif
    UINT32 u4PTS;
    UINT64 u8PTS;
#ifdef ENABLE_MULTIMEDIA
    UINT32 u4RefPts;
    UNUSED(u4RefPts);
#endif

    AUD_DEC_ID_VALIDATE(u1DecId);

    LOG(8, "AUD_GetAudioPts[0]  = 0x%x,0x%x\n",_aeMMSyncInfo[0].u8PTS,_aeMMSyncInfo[0].u4PtsWP);
    LOG(8, "AUD_GetAudioPts[%d] = 0x%x,0x%x\n",u2CurrIdx,_aeMMSyncInfo[u2CurrIdx].u8PTS,_aeMMSyncInfo[u2CurrIdx].u4PtsWP);
    LOG(8, "AUD_GetAudioPts[%d] = 0x%x,0x%x\n",MM_SYNC_INFO_NUM-1,_aeMMSyncInfo[MM_SYNC_INFO_NUM-1].u8PTS,_aeMMSyncInfo[MM_SYNC_INFO_NUM-1].u4PtsWP);

    if((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
#ifdef CC_ENABLE_AOMX
        || (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_GST)
#endif
        )
    {
#ifdef CC_ENABLE_AOMX
        u4ReadPtr = u4ReadD2RInfo_PtsCurPnt(AUD_DSP0, u1DecId);
        GST_MMFindSyncInfo(u1DecId,&u8PTS,u4ReadPtr,&u4PTSHigh);
        UNUSED(u4PTSHigh);
        u4PTS = (UINT32)u8PTS;
        *prAudioPts = u4PTS;
#else
        _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
        UNUSED(u4WritePtr);
        AUD_MMFindSyncInfo(u1DecId,&u8PTS,u4ReadPtr);
        u4PTS = (UINT32)u8PTS;
    #ifdef ENABLE_MULTIMEDIA
        u4RefPts = u4AudDspPts[0];
        if ((u4PTS == 0) && (u4RefPts != 1))
        {
            LOG(5, "Swdmxdsppts=%d\n",u4RefPts);
            *prAudioPts = u4RefPts;
        }
        else if ((u4RefPts != 1) && (u4PTS > u4RefPts))
        {
            *prAudioPts = u4RefPts;
        }
        else
    #endif
        {
            *prAudioPts = u4PTS;
        }
    #ifdef ENABLE_MULTIMEDIA
        LOG(8, "===Find sync audio pts = 0x%x, u4Pts = 0x%x, u4RefPts = 0x%x\n", *prAudioPts, u4PTS, u4RefPts);
    #endif
#endif
    }
    else if(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
        UNUSED(u4WritePtr);
        AUD_MMFindSyncInfo(u1DecId, &u8PTS, u4ReadPtr);
        u4PTS = (UINT32)u8PTS;
        *prAudioPts = u4PTS;
    }
      else
      {
          *prAudioPts = 0;
      }
}

void AUD_DrvGetAudioTrickPts(UINT8 u1DecId, UINT32 *prAudioPts, BOOL fgOldPts)
{
    UINT32 u4PTS;

    if (fgOldPts)
    {
        u4PTS = u4OldPts;
    }
    else
    {
        AUD_GetAudioPts(u1DecId, &u4PTS);
    }
    *prAudioPts = u4PTS;
}

#ifdef CC_SUPPORT_MUXER
void AUD_SetMuxId(UINT8 u1MuxId)
{
    UNUSED(_ui1MuxId);
    _ui1MuxId=u1MuxId;
}
#endif

/*
void AUD_GetFisrtAudioPes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes)
{
     VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
     x_memcpy((VOID *)VIRTUAL(prAudioPes), (const VOID *)&_arAudDecoder[u1DspId][u1DecId].rFirstReceivePes, sizeof(PSR_AUDIO_PES_T));
}

void AUD_GetFisrtAudioDecodePes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes)
{
     VERIFY((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX));
     x_memcpy((VOID *)VIRTUAL(prAudioPes), (const VOID *)&_arAudDecoder[u1DspId][u1DecId].rFristDecodePes, sizeof(PSR_AUDIO_PES_T));
}
*/

#if 0   // Unused
void AUD_ClearFifo(UCHAR ucDecId)
{
    UINT32 u4Rp;
    UINT32 u4Wp;
    UINT32 u4DataSZ;

    AUD_GetRWPtr(AUD_DSP0, ucDecId, &u4Rp, &u4Wp, &u4DataSZ);
    vDspSetFifoReadPtr(AUD_DSP0, ucDecId, u4Wp);
}

BOOL AUD_UpdateAudioFifo(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4SartAddress, UINT32 u4Size, UINT32 u4WritePtr)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Check if write pointer is within audio FIFO
    VERIFY((PHYSICAL(u4WritePtr) >= PHYSICAL(u4SartAddress)) &&
           ((PHYSICAL(u4SartAddress + u4Size)) >= PHYSICAL(u4WritePtr)));

    if (_arAudDecoder[u1DspId][u1DecId].eDecState == AUD_DEC_PLAYING)
    {
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DspId, u1DecId);
            #endif
        _AudSendDspFlowControlCmd(u1DspId, u1DecId, DSP_STOP);
            AUD_WaitUopCommandDone(u1DspId, u1DecId);
#ifdef CC_AUD_ARM_SUPPORT
            // move from _AudSendDspFlowControlCmd to here to prevent DSP not fade out problem
            if (u1DecId == AUD_DEC_MAIN)
            {  
                vAprocPostProc_Close ();   
            }    
#endif            
            vDspSetFifoReadPtr(u1DspId, u1DecId, u4SartAddress);
            if (!fgDspReadPtrSet(AUD_DSP0, u1DecId, u4WritePtr))
            {
                LOG(5, "DspReadPtrSet time out\n");
            }
            #ifdef FIX_AUD_UOP_TIMEOUT
            AUD_WaitUOPLockSema(u1DspId, u1DecId);
            #endif
            _AudSendDspFlowControlCmd(u1DspId, u1DecId, DSP_PLAY);
            AUD_WaitUopCommandDone(u1DspId, u1DecId);
        }
    else
    {
        vDspSetFifoReadPtr(u1DspId, u1DecId, u4SartAddress);
        if (!fgDspReadPtrSet(u1DspId, u1DecId, u4WritePtr))
        {
            LOG(5, "fgDspReadPtrSet fail\n");
        }
    }
    return TRUE;
}
#endif

// FALSE: push mode TRUE:pull mode
BOOL AUD_GetOperationMode(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    return _arAudDecoder[AUD_DSP0][u1DecId].fgOperationMode;
}

// FALSE: push mode TRUE:pull mode
void AUD_SetOperationMode(UINT8 u1DecId, BOOL fgMode)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _arAudDecoder[AUD_DSP0][u1DecId].fgOperationMode = fgMode;
}

AV_SYNC_MODE_T AUD_GetAvSynMode(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    return _arAudDecoder[AUD_DSP0][u1DecId].eSynMode;
}

void AUD_DrvSetAvSynMode(UINT8 u1DspId, UINT8 u1DecId, AV_SYNC_MODE_T eSynMode)
{
    LOG(5,"CMD: set AVSyncMode: Dsp(%d) Dec(%d) Mode(%d)\n", u1DspId, u1DecId, (UINT8)eSynMode);

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
#ifdef AUD_DECODER3_SELF_TEST
    if (u1ForceDecId != AUD_DEC_NUM)    //parson MM3
    {
        u1DecId = u1ForceDecId;
        eSynMode = eForceSyncMode;
    }
#endif
    //we  add this case only for MM AD sync mode convert.. other cases we just accept mw or swdmx setting.
    if ( (u1DecId == AUD_DEC_MAIN) && (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) && _fgMMPlusAD[u1DecId] ==TRUE)
    {
      
       if ( eSynMode == AV_SYNC_AUDIO_MASTER) //this is Multimedia support AD case
       {
 
            eSynMode = AV_SYNC_SLAVE;
           _arAudDecoder[AUD_DSP0][u1DecId].eSynMode = eSynMode;
            vDspSetSyncMode(u1DecId, eSynMode);
    
            u1DecId = AUD_DEC_AUX;
            eSynMode= AV_SYNC_AUDIO_MASTER; 
       }
 
    }
    _arAudDecoder[AUD_DSP0][u1DecId].eSynMode = eSynMode;
    vDspSetSyncMode(u1DecId, eSynMode);
#ifndef CC_AUD_SKYPE_SUPPORT
    if (u1DecId == AUD_DEC_THIRD)  // decoder 3 use FIFO0.
    {
        u1DecId =AUD_DEC_MAIN;
    }
#endif

    // 2008/10/16 Jessica: There is only 1 STC module. STC ID is forced to be 0
    if (STC_SetSyncTarget(AV_SYNC_TARGET_AUD, AUD_DspQryStcID(u1DecId), u1DecId) == STC_INVALID)
    {
        LOG(5, "STC_SetSyncTarget fail\n");
    }
//    VERIFY(STC_SetPtsDrift(u1DecId, PTS_DRIFT) == STC_VALID);
//  STC_SetAudDeviceId(u1DecId, u1DecId);
}

AUD_MM_MODE_T AUD_DrvGetMultimediaMode(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    return _arAudDecoder[AUD_DSP0][u1DecId].eMultimediaMode;
}

void AUD_DrvSetMultimediaMode(UINT8 u1DecId, AUD_MM_MODE_T eMultimediaMode)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _arAudDecoder[AUD_DSP0][u1DecId].eMultimediaMode = eMultimediaMode;
}

void AUD_DrvSetAdMode(BOOL fgAdMode)
{
    UNUSED(fgAdMode);

    _fgAdMode = fgAdMode;
}

AV_SYNC_STC_SELECT_T AUD_GetStcId(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    return _arAudDecoder[AUD_DSP0][u1DecId].eStcId;
}


void AUD_SetStcId(UINT8 u1DecId, AV_SYNC_STC_SELECT_T eStcId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _arAudDecoder[AUD_DSP0][u1DecId].eStcId = eStcId;
}

#if 0   // Unused
// Check if AV is syn
BOOL AUD_AvSynLock(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    return _arAudDecoder[AUD_DSP0][u1DecId].fgAvSynLock;
}

/***************************************************************************
     Function : AUD_SetStcThreshold
  Description : STC thrshold setting
                Note: set the difference before play
    Parameter :
                u2Threshold  : STC Difference worst bound for system master in ms
    Return    : None
***************************************************************************/
void AUD_SetStcThreshold(UINT8 u1DecId, UINT16 u2Threshold)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    DSP_SetStcDiff(AUD_DSP0, u1DecId, u2Threshold);
}
#endif

void AUD_GetDecoderStatus(UINT8 u1DspId, UINT8 u1DecId, AUD_DECODER_T *prAudDec)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    x_memcpy((VOID *)VIRTUAL((UINT32)prAudDec), (const VOID *)&_arAudDecoder[u1DspId][u1DecId], sizeof(AUD_DECODER_T));
}

void AUD_DRVInit(void)
{
    INT32 i, j;
    CRIT_STATE_T _rCritState;
    CRIT_STATE_T rState;

    for (j=0; j<AUD_DSP_NUM; j++)
    {
    for (i=0; i<AUD_DEC_NUM; i++)
    {
        // Clear decoder info ..
        x_memset((VOID *)VIRTUAL((UINT32)&(_arAudDecoder[j][i])), 0, sizeof(AUD_DECODER_T));

        // Variable intial value
            _arAudDecoder[j][i].rPcmSetting.ePcmDataInfo = PCM_24_BITS;
            _arAudDecoder[j][i].rPcmSetting.eSampleFreq = FS_44K;
            _arAudDecoder[j][i].fgMMAoutEnable = FALSE;
            _arAudDecoder[j][i].fgMMAoutReady = FALSE;
            _arAudDecoder[j][i].eMultimediaMode = AUD_MM_MODE_NORMAL;
            _arAudDecoder[j][i].ePath = AUD_PATH_NONE;
        }
    }

    // Init variables
    _rFeedTransmitInfo.eTargetMode = FEEDER_PULL_MODE;
    _rFeedTransmitInfo.u4BufferSize = 0;
    _rFeedTransmitInfo.u4Tag = 0;
    _rFeedTransmitInfo.pfnTransHandle = NULL;
    _rFeedTransmitInfo.pfnNotify = NULL;
    _rFeedTransmitInfo.pfnGetStatus = NULL;

    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));

    if (_aeMMSyncInfo == NULL)
    {
        _aeMMSyncInfo = (AUD_MM_SYNC_INFO_T *)x_mem_alloc(sizeof(AUD_MM_SYNC_INFO_T) * MM_SYNC_INFO_NUM);
        if (_aeMMSyncInfo != NULL) 
        {
            x_memset(_aeMMSyncInfo, 0, sizeof(AUD_MM_SYNC_INFO_T) * MM_SYNC_INFO_NUM);
        }     
        else
        {
            LOG(0, "%s: %d: Failed: allocatate memory for _aeMMSyncInfo!!!!\n", __FUNCTION__, __LINE__);
            ASSERT(0);
        }
    }

#ifdef ADSP_BIN_SUPPORT    
    vAdspBinMemInit();
#endif

    // Dsp thread init
    vADSPTaskInit();

#if defined(CC_AUD_ARM_SUPPORT)
    // Audio processor init, it must init after ADSP for share resource init
    vAprocMain_Init();
#ifdef CC_MAPLE_CUST_DRV    
    DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Set_Init, 0,0, 0,0);    
    I2S_AMP_Init();
#endif
#endif

    // Driver thread init
    _DrvThreadInit();
    // Detection thread init
    AUD_DetectInit();

#if defined(CC_ENABLE_AV_SYNC) || defined(CC_AUD_ENABLE_PLAY_MUTE_CONTROL)
    SRM_RegisterStableCB(_AudSrmStableCb);
#endif

    // Init AvSync setting
    // 2008/10/16 Jessica: There is only 1 STC module. STC ID is forced to be 0
    VERIFY(STC_SetSyncTarget(AV_SYNC_TARGET_AUD, AUD_DspQryStcID(AUD_DEC_MAIN), AUD_DEC_MAIN) == STC_VALID);
    VERIFY(STC_SetSyncTarget(AV_SYNC_TARGET_AUD, AUD_DspQryStcID(AUD_DEC_AUX), AUD_DEC_AUX) == STC_VALID);

    //Andrew Wen 2007/6/22 Audio only program set sync mode to free run.
    prDbase = DBS_Lock(&rState);
    ASSERT(prDbase != NULL);
    VERIFY(DBS_Unlock(prDbase, rState));

    AUD_DrvPtsQueueInit(); //PTSQueue_V2
    
#ifdef CC_AUD_MULTI_BUFFER_MEMORY_PLAY
    AUD_DRVMultiBufferInit();
#endif
}

/****************************************************************************
** Function AUD_DRVGetAudioState(): Returns the current audio state.
**
** Inputs:  Decoder index
**
** Outputs: None.
**
** Returns: Current AudioState.
****************************************************************************/
AUD_DRV_STATE_T AUD_DRVGetAudioState(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    return (_arAudDrvState[u1DspId][u1DecId]);
}

BOOL AUD_DRVCmd(UINT8 u1DspId, UINT8 u1DecId, AUD_DEC_CMD_T eCmd)
{
    UINT32 u4Msg = (UINT32)eCmd;
    AUD_DEC_ID_VALIDATE(u1DecId);
    VERIFY((x_msg_q_send(_ahAudCmdQueue[u1DspId][u1DecId], &u4Msg, sizeof(UINT32), 1)) == OSR_OK);
    return TRUE;
}

BOOL AUD_DRVChangeDecodeFormat(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _arAudDecoder[u1DspId][u1DecId].eStreamFrom = eStreamFrom;
    _arAudDecoder[u1DspId][u1DecId].eDecFormat =  eDecFmt;
    VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_CHANGE_FORMAT));
    return TRUE;
}
#ifdef CC_AUD_DDI
void AUD_DRVSetEncodeType(UINT8 u1DecId,  BOOL fgRecord)
{
    UINT8 i;

    AUD_DEC_ID_VALIDATE(u1DecId);
    //there is only 1 record source at a time
    if ((fgRecord) || ((u1DecId==AUD_DEC_4TH)&&!fgRecord))
    {
        for (i=0;i<AUD_DEC_NUM;i++)
        {
            _arAudDecoder[AUD_DSP0][i].fgRecord= FALSE;
        }
    }
    _arAudDecoder[AUD_DSP0][u1DecId].fgRecord= fgRecord;
    return;
}
BOOL AUD_DRVCheckDecAvailable(UINT8 u1DecId, BOOL fgForRec)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    if ((_arAudDecoder[AUD_DSP0][u1DecId].fgRecord) ||
        (_arAudDecoder[AUD_DSP0][u1DecId].eDecState != AUD_DEC_STOP) ||
        (fgForRec && (_arAudDecoder[AUD_DSP0][u1DecId].ePath == AUD_PATH_MAIN) 
        && (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_GST)))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL AUD_DRVCheckDecRecordFlag(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    return (_arAudDecoder[AUD_DSP0][u1DecId].fgRecord);
}
void AUD_DRVSetADType(UINT8 u1DecId,  BOOL fgIsAD)
{
    UNUSED(u1DecId);
    _arAudDecoder[AUD_DSP0][AUD_DEC_THIRD].fgIsAD= fgIsAD;
    return;
}
void AUD_DRVSetAudPath(UINT8 u1DecId, AUD_PATH_T ePath)
{
    AUD_DEC_ID_VALIDATE(u1DecId); 
    _arAudDecoder[AUD_DSP0][u1DecId].ePath = ePath;
}
#endif
BOOL AUD_DRVSetDecodeType(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // TRICKY HERE  !! SHOULD check in drv_hdmi.c
    // Check previous stream source, if souce is not HDMI, then unmute HDMI
    if ((_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_HDMI) &&
        (eStreamFrom != AUD_STREAM_FROM_HDMI))
    {
        AUD_DspHdmiModeMuteEnable(u1DecId, FALSE);
        AUD_SPDIF_Output_HDMI_Rx(FALSE);
    }

    // Save setting to _arAudDecoder
    _arAudDecoder[u1DspId][u1DecId].eStreamFrom = eStreamFrom;
    _arAudDecoder[u1DspId][u1DecId].eDecFormat =  eDecFmt;

    AUD_SetSrc(u1DecId,eStreamFrom, eDecFmt);
    // Setup individual decoder setting by source
    _DecoderInitSetup(u1DspId, u1DecId, eStreamFrom, eDecFmt);

    // Setup volume gain
    AUD_DspCfgSetDecType(u1DecId,  eStreamFrom, eDecFmt);

    VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_LOADCODE));

    // set SDPIF
    // NOTICE: this function uses _bDspStrTyp and _bDspStrTypDec2 to determine the enable case.
    // Thus, it must calls after DSP code is loaded. (_bDspStrTyp is set at loading DSP code)
    if ((u1DecId!=AUD_DEC_AUX)||(_arAudDecoder[u1DspId][AUD_DEC_MAIN].eStreamFrom != AUD_STREAM_FROM_HDMI))
    {
     //   _AUD_DspSetIEC(u1DecId);
    }
    //Setup DVD gpio
    //AUD_SetSrc(u1DecId,eStreamFrom, eDecFmt);
    AUD_DspDvdEnable();

#if 0 //LPCM_DBG , just for LPCM memplay debug.
    AUD_LPCM_INFO_T  prLPcmInfo;
    if ((u1DecId == AUD_DEC_MAIN)&&(_arAudDecoder[AUD_DEC_MAIN].eDecFormat == AUD_FMT_LPCM))
    {
        prLPcmInfo.b_big_endian = 0;
        prLPcmInfo.b_info_not_specific = 0;
        prLPcmInfo.b_signed_data = 0;
        prLPcmInfo.eLPcmType = LPCM_TYPE_MS_ADPCM;
        prLPcmInfo.eSampleFreq = FS_11K;
        prLPcmInfo.ui2_bits_per_sample = 4;
        prLPcmInfo.ui2_block_align = 256;
        prLPcmInfo.ui2_num_ch = 1;
        AUD_LPcmSetting(AUD_DEC_MAIN, &prLPcmInfo);
    }
#endif

    return TRUE;
}


BOOL AUD_DRVSetStreamFrom(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Save setting to _arAudDecoder
    _arAudDecoder[u1DspId][u1DecId].eStreamFrom = eStreamFrom;
    return TRUE;
}

BOOL AUD_DRVGetStreamFrom(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T * peStreamFrom)
{
     AUD_DSP_ID_VALIDATE(u1DspId);
     AUD_DEC_ID_VALIDATE(u1DecId);

     if (peStreamFrom != NULL)
     {
         *peStreamFrom = _arAudDecoder[u1DspId][u1DecId].eStreamFrom;
     }
     else
     {
         return FALSE;
     }
     return TRUE;
}

BOOL AUD_DRVGetDecodeType(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T * peStreamFrom,
                          AUD_FMT_T * peDecType)
{
     AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    if ((peStreamFrom != NULL) && (peDecType != NULL))
    {
        *peStreamFrom = _arAudDecoder[u1DspId][u1DecId].eStreamFrom;
        *peDecType = _arAudDecoder[u1DspId][u1DecId].eDecFormat;
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

BOOL AUD_DRVGetHDMIDecodeType(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T * peStreamFrom,
                          AUD_FMT_T * peDecType)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    if ((peStreamFrom != NULL) && (peDecType != NULL))
    {
        *peStreamFrom = _arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom;
        *peDecType = _arAudDecoderHDMI[u1DecId].eDecFormat;
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}


BOOL AUD_DRVSetDecMemBuffer(UINT8 u1DspId, UINT8 u1DecId, const MEM_BUFFER_INFO_T * prDecMemBuffer)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    if (prDecMemBuffer == NULL)
    {
        return FALSE;
    }

    _arAudDecoder[u1DspId][u1DecId].rMemBuffer.pData = prDecMemBuffer->pData;
    _arAudDecoder[u1DspId][u1DecId].rMemBuffer.u4Length = prDecMemBuffer->u4Length;
    _arAudDecoder[u1DspId][u1DecId].rMemBuffer.u4LoopCount = prDecMemBuffer->u4LoopCount;
    AUD_SetPaserRpSrc(u1DspId, u1DecId); 
    return TRUE;
}

#ifdef CC_AUD_MULTI_BUFFER_MEMORY_PLAY
void AUD_DRVMultiBufferInit(void)
{
    UINT32 i;

    for (i = 0; i < MAX_MEM_BUFFER_NUMBER; i++)
    {
        _arAudMemBuf[i].pData = NULL;
        _arAudMemBuf[i].u4Length = 0;
        _arAudMemBuf[i].u4BufferSize = 0;
        _arAudMemBuf[i].u4Bid = 0;
        _arAudMemBuf[i].fgValid = FALSE;
        _arAudMemBuf[i].fgEOB = FALSE;
    }

    LOG(7, "[MULTI_BUFFER] %s done!\n", __FUNCTION__);
}

void AUD_DRVMultiBufferReset(void)
{
    UINT32 i;

    if (_afgAudMemBufStart)
    {
        for (i = 0; i < MAX_MEM_BUFFER_NUMBER; i++)
        {
            if (_arAudMemBuf[i].pData != NULL)
            {
                x_mem_free(_arAudMemBuf[i].pData);
            }
            
            _arAudMemBuf[i].pData = NULL;
            _arAudMemBuf[i].u4Length = 0;
            _arAudMemBuf[i].u4BufferSize = 0;
            _arAudMemBuf[i].u4Bid = 0;
            _arAudMemBuf[i].fgValid = FALSE;
            _arAudMemBuf[i].fgEOB = FALSE;
        }

        _au4AudMemBufCurrIdx = 0;
        _au4AudMemBufCurrBid = 0;
        _afgAudMemBufStart = FALSE;
        _afgAudMultiMemBufMode = FALSE;
        
        LOG(7, "[MULTI_BUFFER] %s done!\n", __FUNCTION__);
    }
}

void AUD_DRVMultiBufferMode(BOOL fgMultiBuff)
{
    LOG(7, "[MULTI_BUFFER] %s : %d\n", __FUNCTION__, fgMultiBuff);
    
    _afgAudMultiMemBufMode = fgMultiBuff;
}

BOOL AUD_DRVMultiBufferAddData(UINT32 u4BufferPointer, UINT32 u4BufferSize, BOOL fgEndOfData)
{
    UINT32 i = _au4AudMemBufCurrIdx;

    do
    {
        if (!_arAudMemBuf[i].fgValid)
        {
            // free mem
            if ((_arAudMemBuf[i].pData != NULL) && (_arAudMemBuf[i].u4BufferSize < u4BufferSize))
            {
                x_mem_free(_arAudMemBuf[i].pData);
                _arAudMemBuf[i].pData = NULL;
                _arAudMemBuf[i].u4BufferSize = 0;
            }
            // alloc mem
            if (_arAudMemBuf[i].pData == NULL)
            {
                _arAudMemBuf[i].pData = (UINT8*)x_mem_alloc(u4BufferSize);
                if (_arAudMemBuf[i].pData == NULL)
                {
                    LOG(1, "FAIL! Can't alloc memory for multi buffer.\n");
                    continue;
                }
                _arAudMemBuf[i].u4BufferSize= u4BufferSize;
            }

            // copy data
            if (copy_from_user(_arAudMemBuf[i].pData, (UINT8*)u4BufferPointer, u4BufferSize))
            {
                    LOG(1, "FAIL! Can't copy data from user.\n");
                    return FALSE; 
            }
            _arAudMemBuf[i].u4Length = u4BufferSize;
            _arAudMemBuf[i].u4Bid = _au4AudMemBufCurrBid;
            _arAudMemBuf[i].fgValid = TRUE;
            _arAudMemBuf[i].fgEOB = fgEndOfData;
            _au4AudMemBufCurrBid ++;
            
            // start
            if (!_afgAudMemBufStart)
            {
                _au4AudMemBufCurrIdx = i;
                _afgAudMemBufStart = TRUE;
            }
            LOG(7, "[MULTI_BUFFER] add data: currIdx (%d), bid (%d), end (%d), pData (0x%x), u4Length (0x%x)\n", 
                _au4AudMemBufCurrIdx, i, fgEndOfData, _arAudMemBuf[i].pData, _arAudMemBuf[i].u4Length);
            return TRUE;
        }
    } while ((i = (i + 1) % MAX_MEM_BUFFER_NUMBER) != _au4AudMemBufCurrIdx);
    
    LOG(1, "FAIL! Mem buffers are full!\n");
    
    return FALSE;
}

UINT32 AUD_DRVGetMaxBufferNumber(void)
{
    return MAX_MEM_BUFFER_NUMBER;
}

UINT32 AUD_DRVGetRemainingBuffferNumber(void)
{
    UINT32 i;
    UINT32 u4RemainingCount = 0;
    
    for (i = 0; i < MAX_MEM_BUFFER_NUMBER; i++)
    {
        if (_arAudMemBuf[i].fgValid)
        {
            u4RemainingCount ++;
        }
    }

    return u4RemainingCount;
}
#endif

#if 0   // Unused
void AUD_DrvSetNfy(UINT8 u1DspId, UINT8 u1DecId, const AUD_NFY_INFO_T * prAudNfyInfo)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    if (prAudNfyInfo != NULL)
    {
        _arAudDecoder[u1DspId][u1DecId].rNfyInfo.pvTag = prAudNfyInfo->pvTag;
        _arAudDecoder[u1DspId][u1DecId].rNfyInfo.pfAudDecNfy = prAudNfyInfo->pfAudDecNfy;
    }
}

void AUD_DrvGetNfy(UINT8 u1DspId, UINT8 u1DecId, AUD_NFY_INFO_T * prAudNfyInfo)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
if (prAudNfyInfo != NULL)
    {
        prAudNfyInfo->pvTag = _arAudDecoder[u1DspId][u1DecId].rNfyInfo.pvTag;
        prAudNfyInfo->pfAudDecNfy = _arAudDecoder[u1DspId][u1DecId].rNfyInfo.pfAudDecNfy;
    }
}
#endif

void AUD_StopMemStr(void)
{
    _i4MemStrLoopCnt = 0;
    _fgInfinite = FALSE;
    _fgForceStopStrmDma = TRUE;
}

/***************************************************************************
  Function : AUD_AudioPTSDelayEnable
  Description : First audio PTS would be increased .
                For some low bitrate audio discontinues in the beginning.
  Parameter :
  Return    : None
                                            Andrew Wen 2007/1/31
***************************************************************************/
void AUD_AudioPTSDelayEnable(UINT8 u1DecId, UINT8 uDlyFactor)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _uAudioDelayPTSFlag[u1DecId] = uDlyFactor;
}

//-----------------------------------------------------------------------------
// AUD_DrvFeederSetRequestFunc
//
/** For feeder to set request information.
 *
 *  @param     prTransmit   Notify function handler
 *
 *  @retval    TRUE   ok
 *  @retval    FALSE  fail
 */
//-----------------------------------------------------------------------------
BOOL AUD_DrvFeederSetRequestInfo(FEEDER_TRANSMIT *prTransmit)
{
    if (prTransmit == NULL)
    {
        return FALSE;
    }

    _rFeedTransmitInfo.u4Tag = prTransmit->u4Tag;

    if (prTransmit->eTargetMode == FEEDER_PULL_MODE)
    {
        _rFeedTransmitInfo.u4BufferSize   = prTransmit->u4BufferSize;
        _rFeedTransmitInfo.pfnTransHandle = prTransmit->pfnTransHandle;
        _rFeedTransmitInfo.pfnGetStatus   = prTransmit->pfnGetStatus;
        _rFeedTransmitInfo.pfnNotify      = prTransmit->pfnNotify;
    }
    else if (prTransmit->eTargetMode == FEEDER_PUSH_MODE)
    {
        prTransmit->u4BufferSize = FEEDER_REQUEST_DATA_SIZE;
        _rFeedTransmitInfo.u4BufferSize = FEEDER_REQUEST_DATA_SIZE;
        prTransmit->u4BufferSize = 0; // Not support yet
    }
    else
    {
        // Do nothing ..
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
// AUD_DrvPcmSetting
//
/** PCM format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting
 */
//-----------------------------------------------------------------------------
void AUD_DrvPcmSetting(UINT8 u1DspId, UINT8 u1DecId, const AUD_PCM_SETTING_T * prPcmSetting)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    if (prPcmSetting != NULL)
    {
        _arAudDecoder[u1DspId][u1DecId].rPcmSetting.ePcmDataInfo = prPcmSetting->ePcmDataInfo;
        _arAudDecoder[u1DspId][u1DecId].rPcmSetting.eSampleFreq = prPcmSetting->eSampleFreq;

        DSP_SetPcmType(u1DspId, u1DecId, prPcmSetting->ePcmDataInfo);
        DSP_SetPcmFs(u1DspId, u1DecId, prPcmSetting->eSampleFreq);
    #ifdef CC_HDMI_PCM_MULT_CHANNEL
        _arAudDecoder[u1DspId][u1DecId].rPcmSetting.ePCMChNum = prPcmSetting->ePCMChNum;
        DSP_SetPcmChNum(u1DecId, prPcmSetting->ePCMChNum);
    #endif
    }
}

//-----------------------------------------------------------------------------
// AUD_DrvFlacSetting
//
/** FLAC format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)
 *  @param      prPcmSetting
 *  paul_flac
 */
//-----------------------------------------------------------------------------
void AUD_DrvFlacSetting(UINT8 u1DecID, AUD_FLAC_INFO_T * prFlacInfo)
{
    if (AUD_DEC_MAIN != u1DecID)
    {
        LOG(5, "Currently we only support FLAC in dec1!");
        return;
    }
    if (NULL != prFlacInfo)
    {
        _Aud_DspFLACHeader(u1DecID, prFlacInfo);
    }
}

//-----------------------------------------------------------------------------
// AUD_DrvAc3Setting
//
/** AC3 format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting
 */
//-----------------------------------------------------------------------------
void AUD_DrvAc3Setting(UINT8 u1DecId, const AUD_AC3_SETTING_T * prAc3Setting)
{
    if (prAc3Setting != NULL)
    {
        DSP_SetAc3Endian(u1DecId, prAc3Setting->eEndian);
    }
}

//-----------------------------------------------------------------------------
// AUD_DrvGetDecodeState
//
/** Get audio decoder status
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 */
//-----------------------------------------------------------------------------
AUD_DECODER_T * AUD_DrvGetDecodeState(UINT8 u1DspId, UINT8 u1DecId)
{
    return  &_arAudDecoder[u1DspId][u1DecId];
}

#ifdef CC_AUD_SUPPORT_CLIP_NOTIFY_FUNC
void AUD_DrvSetNotifyFunc(UINT8 u1DecId, AUD_CLIP_NOTIFY pfClipNotify)
{
    UNUSED(u1DecId);
    UNUSED(_hClipNotifyFunc);
    _hClipNotifyFunc =  pfClipNotify;
}
#endif

//-----------------------------------------------------------------------------
// AUD_DrvSetDDBanner
//
/** Turn on DDBanner.
 *
 *  @param     fgEnable     TRUE/FALSE
 *
 *  @retval    void
 */
//-----------------------------------------------------------------------------
void AUD_DrvSetDDBanner(BOOL fgEnable)  //Andrew : Enhance DD Banner
{
    _fgDDBannerOn = fgEnable;
}

void AUD_DrvATunerLock(void)
{
    UNUSED(x_sema_unlock(_hAtunerSyncSema));
}

//-----------------------------------------------------------------------------
// AUD_DrvSetPIPVideoPath
//
/** Set PIP/POP video path information
 *
 *  @param     u1VideoPath     0: main frame, 1: sub frame
 *
 *  @retval    void
 */
//-----------------------------------------------------------------------------
void AUD_DrvSetPIPVideoPath(UINT8 u1DecId, UINT8 u1VideoPath)
{
    UNUSED(u1DecId);

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
    if (_u1VideoFrameFlag != u1VideoPath)
    {
        VERIFY(AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_SET_PIP_VDO_PATH, 0));
    }
#endif

    _u1VideoFrameFlag = u1VideoPath;
}

UINT8 AUD_DrvGetPIPVideoPath(void)
{
    UNUSED(AUD_DrvGetPIPVideoPath);
    return _u1VideoFrameFlag; //0 : main, 1: sub.
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// AUD_DrvDtvLock
//
/** Check DTV lock status
 *
 *  @param     u1DecId
 *
 *  @retval    TRUE        Lock
 *  @retval    FALSE       Unlock
 */
//-----------------------------------------------------------------------------

BOOL AUD_DrvDtvLock(UINT8 u1DecId)
{
    return _afgDtvLock[u1DecId];
}
#endif

//-----------------------------------------------------------------------------
/** AUD_SendApllAdjustCmd
 *  Send APLL MODIN adjust command
 *
 *  @param  u4Level    video calculated level (0 ~ 255. 128 as center)
 *                     bit 8: 0 (slower), 1 (faster)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_SendApllAdjustCmd(UINT32 u4Level)
{
    static UINT32 u4PrevLevel[2] = {0};
    UINT8 u1StcId;
    INT32 i4Ret=0;

    u1StcId = (UINT8)((u4Level >> 9) & 0x1);
    
    if (u4PrevLevel[u1StcId] != u4Level)
    {
        u4PrevLevel[u1StcId] = u4Level;

#ifndef BURN_IN_AOUT_TEST //sunman_aout
        i4Ret = x_msg_q_send(_hAudApllCmdQueue[u1StcId], &u4Level, sizeof(UINT32), 1);
#endif

        if (i4Ret ==  OSR_TOO_MANY)
        {
             LOG(9, "AudApllCmdQueue Full ...\n");
        }
        else if (i4Ret != OSR_OK)
        {
            LOG(1, "AUD_SendApllAdjustCmd  Fail !\n");
        }
    }

}

//sunman_aout
#ifdef BURN_IN_AOUT_TEST
void AUD_SendAoutMonitorCmd(UINT32 u4Level)
{
    INT32 i4Ret=0;

    i4Ret = x_msg_q_send(_hAudApllCmdQueue[0], &u4Level, sizeof(UINT32), 1);

    if (i4Ret ==  OSR_TOO_MANY)
    {
         LOG(9, "AudApllCmdQueue Full ...\n");
    }
    else if (i4Ret != OSR_OK)
    {
        LOG(1, "AUD_SendApllAdjustCmd  Fail !\n");
    }
}
#endif

#ifdef CC_AUD_DDI
void AUD_SendSoundBarCmd(UINT32 u4Volume)
{
    INT32 i4Ret=0;

    i4Ret = x_msg_q_send(_hAudSoundBarCmdQueue, &u4Volume, sizeof(UINT32), 1);

    if (i4Ret ==  OSR_TOO_MANY)
    {
         LOG(9, "AudSoundBarCmdQueue Full ...\n");
    }
    else if (i4Ret != OSR_OK)
    {
        LOG(1, "AudSoundBarCmdQueue  Fail !\n");
    }
}
#endif

#if 0   // Unused
void AUD_SetAC3DataRateCfg(UINT8 u1DecId, UINT8 u1Config)
{
    if (u1Config)
    {
        // enlarge stc_diff bound.  check 0x82a4 ~ 82a6
        AUD_AudioSetStcDiffBound(AUD_DEC_MAIN,0,1,3);
        // add audio start decoding pts
        //AUD_AudioPTSDelayEnable(AUD_DEC_MAIN, 1);
    }
    else
    {
        // enlarge stc_diff bound.  check 0x82a4 ~ 82a6
        AUD_AudioSetStcDiffBound(AUD_DEC_MAIN, 0, 0, 0);
        // add audio start decoding pts
        AUD_AudioPTSDelayEnable(AUD_DEC_MAIN, 0);
    }
}
#endif

inline void _AudDrvThread_WaitResume(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT32 u4Event = 0;

    _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_PLAYING;
    if((u1DecId==AUD_DEC_MAIN) || (u1DecId==AUD_DEC_THIRD))
    {
        DRVCUST_SendAudEvent(E_CUST_AUD_PLAY_STATE_CHANGE, u1DecId);
    }    
    //_arAudDecoder[u1DspId][u1DecId].u4ReceiveValidPesCount = 0;
    _afgFirstIPicArrived[u1DecId] = FALSE;
    //_afgIssuePlayComToDsp[u1DspId][u1DecId] = FALSE;

    // Check if need AV synchronization control
    if (_AvSyncControl(u1DspId, u1DecId))
    {
        LOG(1,"######### NEED SYNC ##########\n");
        //AUD_CommandDone(u1DecId, AUD_CMD_RESUME, AUD_OK);
        _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_PLAYING;

        // Wait Av-Sync or User Stop command
        _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_STOP   |
                              AUD_CMD_FLAG_RESET  |
                              AUD_CMD_FLAG_AVSYNC |
                              AUD_CMD_FLAG_CHANGE_FORMAT);

        if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
        {
            UINT32 u4ReadPtr;
            UINT32 u4WritePtr;
            UINT32 u4PTS;
            UINT64 u8PTS;
            INT32  i4StcDiff;
            STC_CLOCK rStc;

            UNUSED(i4StcDiff);
            _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);

            {
                LOG(5,"_aeMMSyncInfo[0]  = 0x%x,0x%x\n",_aeMMSyncInfo[0].u8PTS,_aeMMSyncInfo[0].u4PtsWP);
                LOG(5,"_aeMMSyncInfo[%d] = 0x%x,0x%x\n",u2CurrIdx,_aeMMSyncInfo[u2CurrIdx].u8PTS,_aeMMSyncInfo[u2CurrIdx].u4PtsWP);
                LOG(5,"_aeMMSyncInfo[%d] = 0x%x,0x%x\n",MM_SYNC_INFO_NUM-1,_aeMMSyncInfo[MM_SYNC_INFO_NUM-1].u8PTS,_aeMMSyncInfo[MM_SYNC_INFO_NUM-1].u4PtsWP);
            }
            AUD_MMFindSyncInfo(u1DecId,&u8PTS,u4ReadPtr);
            u4PTS = (UINT32)u8PTS;
            LOG(7, "==> rp, wp = (0x%08x, 0x%08x)\n", (u4ReadPtr & 0xffffff), (u4WritePtr & 0xffffff));
            i4StcDiff = 0;
            if (STC_GetSrc(AUD_DspQryStcID(u1DecId), &rStc) == STC_VALID)
            {
                i4StcDiff = (INT32)(u4PTS - rStc.u4Base);
                LOG(5, "# STC  = 0x%x\n", rStc.u4Base);
                LOG(5, "# Diff = %d\n", i4StcDiff);
            }
            if((u4ReadPtr >= DSP_PHYSICAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId))) && 
              (u4ReadPtr < DSP_PHYSICAL_ADDR(u4GetAFIFOEnd(AUD_DSP0, u1DecId))))
            {
            if (!DMX_UpdateReadPointer(_DecIdToPidIdx(u1DecId), u4ReadPtr, 0))
            {
                LOG(7, "8. Buffer pointer range checking...\n");
            }
            }
#ifdef CC_ENABLE_AOMX //new resume sync flow: swdmx trigger audio output
            //Set Delay Here
            if (i4StcDiff > 0)
            {
                i4StcDiff = (i4StcDiff/900) * 10;  // The delay is (diff / 90k)ms
                if (i4StcDiff > 500) //at most 1000ms
                {
                    i4StcDiff = 500;
                }
                LOG(5,"######## !!!! Delay for %d ms #######################\n",i4StcDiff);
                x_thread_delay((UINT32)i4StcDiff);
            }
#endif
            u4Event = (UINT32)AUD_CMD_AVSYNC;
        }

        switch (u4Event)
        {
            case AUD_CMD_STOP:
                _arAudDecoder[u1DspId][u1DecId].u4ReceivePesCount = 0;
                // Release Stop command API waiting semaphore
                AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_STOP, AUD_OK);
                _ChangeAudioState(u1DspId, u1DecId, AUD_IDLE);
                break;

            case AUD_CMD_AVSYNC:
                #ifdef FIX_AUD_UOP_TIMEOUT
                AUD_WaitUOPLockSema(u1DspId, u1DecId);
                #endif
#ifndef CC_ENABLE_AOMX 
                LOG(3, "Clear Aout wait ok before resume command\n");
                AUD_DrvMMAoutEnable(u1DecId, FALSE);
#endif                
                _AudSendDspFlowControlCmd(u1DspId, u1DecId, DSP_RESUME);
                _afgIssuePlayComToDsp[u1DspId][u1DecId] = TRUE;
                AUD_WaitUopCommandDone(u1DspId, u1DecId);
                AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_RESUME, AUD_OK);
                LOG( 3,"DSP(%d) Aud decoder %d Resume\n",u1DspId, u1DecId);
                _ChangeAudioState(u1DspId, u1DecId, AUD_ON_PLAY);

                MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_RESUME_DONE);
                break;
            case AUD_CMD_CHANGE_FORMAT:
                _arAudDecoder[u1DspId][u1DecId].u4ReceivePesCount = 0;
                // Do nothing on this state, keep play back
                AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_CHANGE_FORMAT,TRUE);
                break;

            default:
                ASSERT(0);
        }
    }
    else  // No need of initial AV synchronization control
    {
        LOG(1,"######### NO NEED SYNC ##########\n");
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_RESUME, AUD_OK);
        _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_PLAYING;

        if (_arAudDecoder[u1DspId][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
        {
            UINT32 u4FifoStart;
            UINT32 u4FifoEnd;

            UNUSED(_AUD_GetAudFifo(u1DspId, u1DecId, &u4FifoStart, &u4FifoEnd));
            vDspSetFifoReadPtr(u1DspId, u1DecId, u4FifoStart);
        }

        #ifdef FIX_AUD_UOP_TIMEOUT
        AUD_WaitUOPLockSema(u1DspId, u1DecId);
        #endif
        _AudSendDspFlowControlCmd(u1DspId, u1DecId, DSP_RESUME);
        _afgIssuePlayComToDsp[u1DspId][u1DecId] = TRUE;
        AUD_WaitUopCommandDone(u1DspId, u1DecId);
        LOG(3, "DSP(%d) Aud decoder %d Resume\n",u1DspId, u1DecId);

        _ChangeAudioState(u1DspId, u1DecId, AUD_ON_PLAY);

        MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_RESUME_DONE);
    }

    return;
}

#ifdef TIME_SHIFT_SUPPORT
void AUD_MMQueueTickInfo(UINT8 u1DecId, UINT32 u4Tick, UINT32 u4PTSWp, BOOL fgReset)
{
    static UINT16 u2Idx = 0;
    UINT16 i;
    BOOL fgUpdate;

    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;

    UNUSED(_AUD_GetAudFifo(AUD_DSP0, u1DecId, &u4FifoStart, &u4FifoEnd));
    UNUSED(u1DecId);

    if (fgReset)
    {
        u2Idx = 0;
        //u2TickLatestIdx = 0;
        //fgTickLoopBack = FALSE;
        for (i=0; i<MM_TICK_INFO_NUM; i++)
        {
            _aeMMTickInfo[i].u4Tick = 0;
            _aeMMTickInfo[i].u4PtsWP = 0xfffffff;
        }
    }
    else
    {
        if (u2Idx >= MM_TICK_INFO_NUM)
        {
            LOG(7," ### MM Tick Info - The deepest u2Idx = %d ###\n", u2Idx);
            LOG(7,"_aeMMTickInfo[0]  = 0x%x,0x%x\n",_aeMMTickInfo[0].u4Tick,_aeMMTickInfo[0].u4PtsWP);
            LOG(7,"_aeMMTickInfo[%d] = 0x%x,0x%x\n",u2Idx-1,_aeMMTickInfo[u2Idx-1].u4Tick,_aeMMTickInfo[u2Idx-1].u4PtsWP);
            u2Idx = 0;
            //fgTickLoopBack = TRUE;
        }
        LOG(9,"MM Q %d, Tick 0x%x, Wp 0x%x\n",u2Idx,u4Tick,u4PTSWp);

        fgUpdate = TRUE;
        if(u2Idx == 0)
        {
            if(_aeMMTickInfo[MM_TICK_INFO_NUM - 1].u4Tick == u4Tick)
            {
                fgUpdate = FALSE;
                _aeMMTickInfo[MM_TICK_INFO_NUM - 1].u4PtsWP = u4PTSWp;
            }
        }
        else
        {
            if(_aeMMTickInfo[u2Idx - 1].u4Tick == u4Tick)
            {
                fgUpdate = FALSE;
                _aeMMTickInfo[u2Idx - 1].u4PtsWP = u4PTSWp;
            }
        }
        if(fgUpdate)
        {
            _aeMMTickInfo[u2Idx].u4Tick = u4Tick;
            _aeMMTickInfo[u2Idx].u4PtsWP = u4PTSWp;
            u2TickCurrIdx = u2Idx;
            u2Idx += 1;
        }
    }


}

BOOL AUD_MMFindTickInfo(UINT8 u1DecId, UINT32* u4Tick)  //, UINT32 u4PTSRp)
{

    UINT16 i;
    UINT16 u2Idx;
    BOOL fgFindFifoEnd = FALSE;
    BOOL fgFound = FALSE;
    UINT32 u4PTSRp;
    UINT32 u4ReadPtr;
    UINT32 u4WritePtr;

    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);

    UNUSED(u1DecId);
    UNUSED(u2Idx);
    UNUSED(u4WritePtr);

    u4PTSRp = u4ReadPtr;
    u4PTSRp &= 0xffffff;
    *u4Tick = 0;

    u2Idx = u2TickCurrIdx;
    VERIFY(u2Idx < MM_TICK_INFO_NUM);
    if (u2Idx < MM_TICK_INFO_NUM)
    {
        for (i=0; i<MM_TICK_INFO_NUM; i++)
        {
            if (fgFindFifoEnd)
            {
                if (u4PTSRp >= _aeMMTickInfo[u2Idx].u4PtsWP)
                {
                    LOG(5,"Request Rp = 0x%x\n", u4PTSRp);
                    LOG(5,"MM Find idx = %d, Rp = 0x%x, Tick = 0x%x @@@@@@@@@@@@@@@@@@@@@\n",u2Idx,
                    _aeMMTickInfo[u2Idx].u4PtsWP, _aeMMTickInfo[u2Idx].u4Tick);
                    *u4Tick = _aeMMTickInfo[u2Idx].u4Tick;
                    fgFound = TRUE;
                    break;
                }
            }
            else
            {
                if (u4PTSRp <= _aeMMTickInfo[u2Idx].u4PtsWP)
                {
                    fgFindFifoEnd = TRUE;
                }
            }

            if (u2Idx == 0)
            {
                u2Idx = MM_TICK_INFO_NUM;
            }
            u2Idx--;
        }
    }
    else
    {
        LOG(7,"MM Tick error\n");
    }
    return fgFound;
}
#endif

void AUD_MMQueueSyncInfo(UINT8 u1DecId, UINT64 u8PTS, UINT32 u4PTSWp, BOOL fgReset)
{
    static UINT16 u2Idx = 0;
    UINT16 i;
    static UINT8 u1Skip = 0;
    UINT32 u4DataRate = 0;
    UINT8 u1Mod = 0;

    UNUSED(u1DecId);
    UNUSED(u2CurrIdx);
    UNUSED(u2LatestIdx);
    UNUSED(fgLoopBack);

    if (fgReset)
    {
        u2Idx = 0;
        u1Skip = 0;
        u2LatestIdx = 0;
        fgLoopBack = FALSE;
        for (i=0; i<MM_SYNC_INFO_NUM; i++)
        {
            _aeMMSyncInfo[i].u8PTS = 0;
            _aeMMSyncInfo[i].u4PtsWP = 0xfffffff;
        }
        fgSyncInfoReset = TRUE;
    }
    else
    {
        u4DataRate = DSP_GetDataRate(u1DecId);
        if (u4DataRate < 128000)      //In low bitrate case, info array may overflow
        {
            u1Skip++;
            if (u4DataRate >= 60000) //Skip 1/2 data for about 64kbps. (take 60000, consider some VBR case)
            {
                u1Mod = 2;
            }
            else
            {
                u1Mod = 4;          //Skip 1/4 data for bitrate < 64kbps.
            }
            if (((u1Skip - 1) % u1Mod)!=0)
            {
                return;
            }
        }

        if (u8PTS == 0)
        {
            LOG(7, "MM ## PTS is zero.\n");
        }
        else if ((_aeMMSyncInfo[u2CurrIdx].u8PTS == u8PTS) || (_aeMMSyncInfo[u2CurrIdx].u4PtsWP == u4PTSWp))
        {
            LOG(7, "MM ## PTS or wp are duplicated: _aeMMSyncInfo[%d] = 0x%llx,0x%x <== 0x%llx,0x%x\n", u2CurrIdx,
                _aeMMSyncInfo[u2CurrIdx].u8PTS, _aeMMSyncInfo[u2CurrIdx].u4PtsWP, u8PTS, u4PTSWp);
            return;
        }
        else if (((u8PTS > _aeMMSyncInfo[u2CurrIdx].u8PTS) ? (u8PTS - _aeMMSyncInfo[u2CurrIdx].u8PTS) : 
                (u8PTS + ((~_aeMMSyncInfo[u2CurrIdx].u8PTS) & 0xFFFFFFFF) + 1)) < 900)  // less than 10ms
        {
            LOG(9, "MM ## PTS is small change : _aeMMSyncInfo[%d] = 0x%llx,0x%x <== 0x%llx,0x%x\n", u2CurrIdx,
                _aeMMSyncInfo[u2CurrIdx].u8PTS, _aeMMSyncInfo[u2CurrIdx].u4PtsWP, u8PTS, u4PTSWp);
            return;
        }
        
        if (u2Idx >= MM_SYNC_INFO_NUM)
        {
            LOG(7," ### MM Sync Info - The deepest u2Idx = %d ###\n", u2Idx);
            LOG(7,"_aeMMSyncInfo[0]  = 0x%x,0x%x\n",_aeMMSyncInfo[0].u8PTS,_aeMMSyncInfo[0].u4PtsWP);
            LOG(7,"_aeMMSyncInfo[%d] = 0x%x,0x%x\n",u2Idx-1,_aeMMSyncInfo[u2Idx-1].u8PTS,_aeMMSyncInfo[u2Idx-1].u4PtsWP);
            u2Idx = 0;
            fgLoopBack = TRUE;
        }
        LOG(9,"MM Q %d, P 0x%llx, W 0x%x\n",u2Idx,u8PTS,u4PTSWp);

        _aeMMSyncInfo[u2Idx].u8PTS = u8PTS;
        _aeMMSyncInfo[u2Idx].u4PtsWP = u4PTSWp;
        u2CurrIdx = u2Idx;
        fgSyncInfoReset = FALSE;
        u2Idx += 1;
    }
}

#ifdef CC_ENABLE_AOMX
void GST_MMQueueSyncInfo(UINT8 u1DecId, UINT64 u8PTS, UINT32 u4PTSWp, BOOL fgReset, UINT32 u4PTSHigh)
{
    static UINT16 u2Idx = 0;
    UINT16 i;

    UNUSED(u1DecId);
    UNUSED(u2CurrIdx);
    UNUSED(u2LatestIdx);
    UNUSED(fgLoopBack);

    if (fgReset)
    {
        u2Idx = 0;
        u2LatestIdx = 0;
        fgLoopBack = FALSE;
        u8LastGetPts = 0;
        u2LastGetIdx = 0xFFFF;
        for (i=0; i<MM_SYNC_INFO_NUM; i++)
        {
            _aeMMSyncInfo[i].u8PTS = 0;
            _aeMMSyncInfo[i].u4PtsWP = 0xfffffff;
            _aeMMSyncInfo[i].u4PTSHigh = 0;
        }
        LOG(1,"AUD_MMQueueSyncInfo RESET!\n");
    }
    else
    {
        if (u2Idx >= MM_SYNC_INFO_NUM)
        {
            LOG(7," ### MM Sync Info - The deepest u2Idx = %d ###\n", u2Idx);
            LOG(7,"_aeMMSyncInfo[0]  = 0x%x,0x%x\n",_aeMMSyncInfo[0].u8PTS,_aeMMSyncInfo[0].u4PtsWP);
            LOG(7,"_aeMMSyncInfo[%d] = 0x%x,0x%x\n",u2Idx-1,_aeMMSyncInfo[u2Idx-1].u8PTS,_aeMMSyncInfo[u2Idx-1].u4PtsWP);
            u2Idx = 0;
            fgLoopBack = TRUE;
        }

        _aeMMSyncInfo[u2Idx].u8PTS = u8PTS;
        _aeMMSyncInfo[u2Idx].u4PtsWP = u4PTSWp;
        _aeMMSyncInfo[u2Idx].u4PTSHigh = u4PTSHigh;

        LOG(9,"MM Q %d, P %lld, W 0x%x, H 0x%x\n",u2Idx,_aeMMSyncInfo[u2Idx].u8PTS,_aeMMSyncInfo[u2Idx].u4PtsWP,_aeMMSyncInfo[u2Idx].u4PTSHigh);
        u2CurrIdx = u2Idx;
        u2Idx += 1;
    }
}
#endif

void AUD_MMFindSyncInfo(UINT8 u1DecId, UINT64* u8PTS, UINT32 u4PTSRp)
{
    UINT16 u2Idx, u2PreIdx;
    BOOL fgOverRead = FALSE;
    BOOL fgFound = FALSE;
    UINT32 u4CurWp;
    UINT32 u4FindWp;
    UINT32 u4FifoStart, u4FifoEnd;
    UINT32 u4Pos, u4Size, u4Wp1, u4Wp2;
    UINT64 u8FindPts,u8Pts1, u8Pts2;
    UNUSED(u1DecId);
    UNUSED(u2Idx);
    UNUSED(u2PreIdx);

    *u8PTS = 0;
    _AUD_DMX_GetAudPtr(u1DecId, &u4PTSRp, &u4CurWp);
    u4PTSRp &= 0xffffff;
    u4CurWp &= 0xffffff;

    u4FindWp = 0;
    u8FindPts = 0;

    u2Idx = u2CurrIdx;
    VERIFY(u2Idx < MM_SYNC_INFO_NUM);
    u2PreIdx = (u2Idx > 0) ? (u2Idx - 1) : (MM_SYNC_INFO_NUM - 1);

    //check if RP over the newest WP in the Queue
    if ((_aeMMSyncInfo[u2Idx].u4PtsWP != 0xfffffff) && (_aeMMSyncInfo[u2PreIdx].u4PtsWP != 0xfffffff))
    {
        if (u4CurWp >= _aeMMSyncInfo[u2Idx].u4PtsWP)  // In this case, Rp is very close to Wp.
        {
            if ((u4PTSRp >= _aeMMSyncInfo[u2Idx].u4PtsWP) && (u4PTSRp < u4CurWp))
            {
                // [Wp in Lastest Q item] , [PTSRp] , [Current Wp]
                // Lastest item is what we need.
                u4FindWp =  _aeMMSyncInfo[u2Idx].u4PtsWP;
                u8FindPts=  _aeMMSyncInfo[u2Idx].u8PTS;
                fgOverRead = TRUE;

                ////LOG(3, "1. u4FindWp=0x%x, u8FindPts=0x%llx\n", u4FindWp, u8FindPts);
            }
        }
        else
        {
            // [Current Wp] , [Wp in Lastest Q item], [PTSRp]
            // [PTSRp] , [Current Wp] , [Wp in Lastest Q item]
            // Current Wp loop back
            if ((u4PTSRp >= _aeMMSyncInfo[u2Idx].u4PtsWP) || (u4PTSRp < u4CurWp))
            {
                u4FindWp =  _aeMMSyncInfo[u2Idx].u4PtsWP;
                u8FindPts=  _aeMMSyncInfo[u2Idx].u8PTS;
                fgOverRead = TRUE;

                ////LOG(3, "2. u4FindWp=0x%x, u8FindPts=0x%llx\n", u4FindWp, u8FindPts);
            }
        }
    }

    if (!fgOverRead)
    {
        UINT16 i;
        for (i=0; i<MM_SYNC_INFO_NUM-1; i++) //Find through all queue except the last one.
        {
            u2PreIdx = (u2Idx > 0) ? (u2Idx - 1) : (MM_SYNC_INFO_NUM - 1);
            if ((_aeMMSyncInfo[u2Idx].u4PtsWP != 0xfffffff) && (_aeMMSyncInfo[u2PreIdx].u4PtsWP != 0xfffffff))
            {
                if (_aeMMSyncInfo[u2Idx].u4PtsWP > _aeMMSyncInfo[u2PreIdx].u4PtsWP)
                {
                    if ((u4PTSRp >= _aeMMSyncInfo[u2PreIdx].u4PtsWP) && (u4PTSRp < _aeMMSyncInfo[u2Idx].u4PtsWP))
                    {
                        // [i-1].Wp < [PTSRp] < [i].Wp
                        u4FindWp = _aeMMSyncInfo[u2PreIdx].u4PtsWP;
                        u8FindPts = _aeMMSyncInfo[u2PreIdx].u8PTS;
                        fgFound = TRUE;

                        ////LOG(3, "3. u4FindWp=0x%x, u8FindPts=0x%llx\n", u4FindWp, u8FindPts);
                        break;
                    }
                }
                else
                {
                    if ((u4PTSRp >= _aeMMSyncInfo[u2PreIdx].u4PtsWP) || (u4PTSRp < _aeMMSyncInfo[u2Idx].u4PtsWP))
                    {
                        // [PTSRp] < [i].Wp < [i-1].Wp
                        // or
                        // [i].Wp < [i-1].Wp < [PTSRp]
                        u4FindWp = _aeMMSyncInfo[u2PreIdx].u4PtsWP;
                        u8FindPts = _aeMMSyncInfo[u2PreIdx].u8PTS;
                        fgFound = TRUE;

                        ////LOG(3, "4. u4FindWp=0x%x, u8FindPts=0x%llx\n", u4FindWp, u8FindPts);
                        break;
                    }
                }
            }
            else
            {
                break;
            }

            if (u2Idx == 0)
            {
                u2Idx = MM_SYNC_INFO_NUM;
            }
            u2Idx--;
        }
    }

    if (fgFound || fgOverRead)
    {
        u4FifoStart = u4GetAFIFOStart(AUD_DSP0, u1DecId);
        u4FifoEnd = u4GetAFIFOEnd(AUD_DSP0, u1DecId);
        u4Wp1 = _aeMMSyncInfo[u2PreIdx].u4PtsWP;
        u4Wp2 = _aeMMSyncInfo[u2Idx].u4PtsWP;
        u8Pts1 = _aeMMSyncInfo[u2PreIdx].u8PTS;
        u8Pts2 = _aeMMSyncInfo[u2Idx].u8PTS;
        u4Pos = (u4PTSRp >= u4FindWp) ? (u4PTSRp - u4FindWp) : (u4PTSRp + u4FifoEnd - u4FindWp - u4FifoStart);
        u4Size = (u4Wp2 >= u4Wp1) ? (u4Wp2 - u4Wp1) : (u4Wp2 + u4FifoEnd - u4Wp1 - u4FifoStart);
        if (u4Size > 0)
        {
            #if 0
            if(_arAudDecoder[u1DspId][u1DecId].eDecFormat == AUD_FMT_WMA)
            {
                *u8PTS = u8FindPts + u8Div6432((UINT64)u4Pos * (u8Pts2 - u8Pts1), (UINT64)u4Size, NULL) ;
            }
            else
            {
                //*u8PTS = u8FindPts + u8Div6432((UINT64)u4Pos * (u8Pts2 - u8Pts1), (UINT64)u4Size, NULL) ;
                //discard invalid pts when change FR to FF--zhengwen@mtk94060
                if(u8Pts2 >= u8Pts1)
                {
                    *u8PTS = u8FindPts + u8Div6432((UINT64)u4Pos * (u8Pts2 - u8Pts1), (UINT64)u4Size, NULL) ;
                }
                else
                {
                    *u8PTS = 0x7fffffffffffffff;
                }
            }
            #else
            if (u8Pts2 > u8Pts1)
            {
                *u8PTS = u8FindPts + u8Div6432((UINT64)u4Pos * (u8Pts2 - u8Pts1), (UINT64)u4Size, NULL) ;
            }
            else
            {
                *u8PTS = u8FindPts;
            }
            #endif
        }

        #if 0 //for debug
        LOG(3, "5. F 0x%x--0x%x, Idx(%d-*%d), W(0x%x,*0x%x)\n", u4FifoStart, u4FifoEnd, u2PreIdx, u2Idx, u4Wp1,u4Wp2);
        LOG(3, "5. P(0x%llx,*0x%llx), Rp = 0x%x\n", u8Pts1, u8Pts2, u4PTSRp);
        LOG(3, "5. 0x%llx/0x%llx = 0x%llx\n", ((UINT64)u4Pos * (u8Pts2 - u8Pts1)), (UINT64)u4Size, u8Div6432((UINT64)u4Pos * (u8Pts2 - u8Pts1), (UINT64)u4Size, NULL));
        LOG(3, "5. u8PTS=0x%llx, u2CurrIdx=%d\n", *u8PTS, u2CurrIdx);
        LOG(3, "5. (i-1)0x%x, 0x%llx  (i)0x%x, 0x%llx  (i+1)0x%x, 0x%llx\n",
            _aeMMSyncInfo[u2PreIdx].u4PtsWP, _aeMMSyncInfo[u2PreIdx].u8PTS, _aeMMSyncInfo[u2Idx].u4PtsWP, _aeMMSyncInfo[u2Idx].u8PTS,
            _aeMMSyncInfo[u2Idx+1].u4PtsWP, _aeMMSyncInfo[u2Idx+1].u8PTS);
        #endif
    }

    #if 0 // for debug
    if (*u8PTS == 0x0)
    {
        LOG(5, "Find pts: u4PtsRp (0x%x), u4CurWp (0x%x), u2PreIdx (%d), u2Idx (%d)\n",
            u4PTSRp, u4CurWp, u2PreIdx, u2Idx);
        LOG(5, "Find pts: u8FindPts (0x%llx), u4FindWp (0x%x), fgFound (%d), fgOverRead (%d)\n",
            u8FindPts, u4FindWp, fgFound, fgOverRead);
        LOG(5, "Find pts: u4Wp1 (0x%x), u4Wp2 (0x%x), u8Pts1 (0x%llx), u8Pts2 (0x%llx), u4Pos (0x%x), u4Size (%d)\n",
            u4Wp1, u4Wp2, u8Pts1, u8Pts2, u4Pos, u4Size);
        LOG(5, "Find pts: (Pts,Wp) : (i-1)0x%x, 0x%llx  (i)0x%x, 0x%llx, (cur-1)0x%x, 0x%llx\n",
            _aeMMSyncInfo[u2PreIdx].u4PtsWP, _aeMMSyncInfo[u2PreIdx].u8PTS, _aeMMSyncInfo[u2Idx].u4PtsWP, _aeMMSyncInfo[u2Idx].u8PTS,
                _aeMMSyncInfo[(u2CurrIdx > 0) ? (u2CurrIdx - 1) : (MM_SYNC_INFO_NUM - 1)].u4PtsWP,
                _aeMMSyncInfo[(u2CurrIdx > 0) ? (u2CurrIdx - 1) : (MM_SYNC_INFO_NUM - 1)].u8PTS);
    }
    #endif
    
    if ((!fgSyncInfoReset) && (*u8PTS != 0x0)) //Get the valid pts
    {
        u8PreSyncPtsInfo = *u8PTS;
    }
    else // Get the invalid pts because pts queue has reseted
    {
        *u8PTS = u8PreSyncPtsInfo;
    }
}

#ifdef CC_ENABLE_AOMX
BOOL GST_MMFindSyncInfo(UINT8 u1DecId, UINT64* u8PTS, UINT32 u4PTSRp, UINT32* u4PTSHigh)
{
    UINT16 i;
    UINT16 u2Idx,u2IdxPrev,u2IdxNext;
    BOOL fgFindFifoEnd = FALSE;
    BOOL fgFound = FALSE;
    UINT32 u4PtsWP1, u4PtsWP2,u4FifoStart,u4FifoEnd;
    UINT64 u8PTS1, u8PTS2,u8PTSInt;

    UNUSED(u1DecId);
    UNUSED(u2Idx);

    u4PTSRp &= 0xffffff;
    *u8PTS = 0;

    u2Idx = u2CurrIdx;
    VERIFY(u2Idx < MM_SYNC_INFO_NUM);
    for (i=0; i<MM_SYNC_INFO_NUM; i++)
    {
        if (u2Idx == 0)
        {
            u2IdxPrev = MM_SYNC_INFO_NUM-1;
        }
        else
        {
            u2IdxPrev = u2Idx - 1;
        }
        if (u2Idx == MM_SYNC_INFO_NUM-1)
        {
            u2IdxNext = 0;
        }
        else
        {
            u2IdxNext = u2Idx + 1;
        }

        if (fgFindFifoEnd)
        {
            if (u4PTSRp >= _aeMMSyncInfo[u2Idx].u4PtsWP)
            {
                fgFound = TRUE;
                break;
            }
            else if ((_aeMMSyncInfo[u2Idx].u4PtsWP > _aeMMSyncInfo[u2IdxNext].u4PtsWP))
            {
                fgFound = TRUE;
                break;
            }
        }
        else
        {
            if (u4PTSRp == _aeMMSyncInfo[u2Idx].u4PtsWP)
            {
                fgFound = TRUE;
                break;
            }
            else if (u4PTSRp < _aeMMSyncInfo[u2Idx].u4PtsWP)
            {
                fgFindFifoEnd = TRUE;
            }
            else if ((_aeMMSyncInfo[u2Idx].u4PtsWP < _aeMMSyncInfo[u2IdxPrev].u4PtsWP) &&
                        (u4PTSRp >= _aeMMSyncInfo[u2IdxPrev].u4PtsWP))
            {
                //queue wrap around, RP is between them
                fgFindFifoEnd = TRUE;
            }
        }
        if (u2Idx == u2LastGetIdx)
        {
            break;
        }

        if (u2Idx == 0)
        {
            u2Idx = MM_SYNC_INFO_NUM;
        }
        u2Idx--;
    }

/*
    u4PtsWP1 =  _aeMMSyncInfo[0].u4PtsWP;
    u4PtsWP2 =  _aeMMSyncInfo[1].u4PtsWP;
    u4PTS1 = _aeMMSyncInfo[0].u4PTS;
    u4PTS2 = _aeMMSyncInfo[1].u4PTS;
    if ((!fgFound) && (u4PTSRp < u4PtsWP1) && (u4PtsWP1 !=  u4PtsWP2) && (u4PtsWP1 != 0xfffffff) && (u4PtsWP2 != 0xfffffff))
    {
        LOG(0,"AUD_MMFindSyncInfo NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11\n");
        *u4PTS = u4PTS1 - (UINT32)u8Div6432((UINT64)((UINT64)(u4PtsWP1-u4PTSRp)*(UINT64)(u4PTS2-u4PTS1)), (UINT64)(u4PtsWP2-u4PtsWP1), NULL);
    }
*/
    if ((!fgFound) ||  (_aeMMSyncInfo[u2Idx].u4PtsWP == 0xfffffff))
    {
        LOG(8,"AUD_MMFindSyncInfo NOT FOUND!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        fgFound = FALSE;
    }
    else if (_aeMMSyncInfo[u2Idx].u8PTS < u8LastGetPts)
    {
        LOG(8,"PTS decrease!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%lld, %lld\n", _aeMMSyncInfo[u2Idx].u8PTS,u8LastGetPts );
    }
    *u8PTS = _aeMMSyncInfo[u2Idx].u8PTS;
    *u4PTSHigh = _aeMMSyncInfo[u2Idx].u4PTSHigh;
    LOG(8,"Request Rp = 0x%x\n", u4PTSRp);
    LOG(8,"MM Find idx = %d, Rp = 0x%x, PTS = %lld @@@@@@@@@@@@@@@@@@@@@\n",u2Idx,
    _aeMMSyncInfo[u2Idx].u4PtsWP, _aeMMSyncInfo[u2Idx].u8PTS);
    if (fgFound)
    {
        u8LastGetPts = *u8PTS;
        u2LastGetIdx = u2Idx;
        if ((u4PTSRp != _aeMMSyncInfo[u2Idx].u4PtsWP))
        {
            //do interpolation
            if (u2Idx == MM_SYNC_INFO_NUM-1)
            {
                u2IdxNext = 0;
            }
            else
            {
                u2IdxNext = u2Idx + 1;
            }
            u4PtsWP1 =  _aeMMSyncInfo[u2Idx].u4PtsWP;
            u4PtsWP2 =  _aeMMSyncInfo[u2IdxNext].u4PtsWP;
            u8PTS1 = _aeMMSyncInfo[u2Idx].u8PTS;
            u8PTS2 = _aeMMSyncInfo[u2IdxNext].u8PTS;
            if ((u8PTS2 > u8PTS1) && (u4PtsWP1 != u4PtsWP2))
            {
                u4FifoStart = DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId));
                u4FifoEnd = DSP_INTERNAL_ADDR(u4GetAFIFOEnd(AUD_DSP0, u1DecId));
                LOG(8,"#####INTERPOLATION 0x%x, 0x%x, 0x%x, %lld, %lld\n",u4PTSRp,u4PtsWP1,u4PtsWP2,u8PTS1,u8PTS2);
                if (u4PtsWP2 > u4PtsWP1)
                {
                    u8PTSInt = u8PTS1 + u8Div6432(((UINT64)(u4PTSRp-u4PtsWP1))* (u8PTS2 - u8PTS1),(UINT64)(u4PtsWP2 -u4PtsWP1),NULL);
                    //LOG(0,"CASE1\n");
                }
                else
                {
                    if (u4PTSRp > u4PtsWP1)
                    {
                        u8PTSInt = u8PTS1 + u8Div6432(((UINT64)(u4PTSRp-u4PtsWP1))* (u8PTS2 - u8PTS1),(UINT64)(u4PtsWP2 -u4PtsWP1 + u4FifoEnd - u4FifoStart ),NULL);
                        //LOG(0,"CASE2\n");
                    }
                    else
                    {
                        u8PTSInt = u8PTS1 + u8Div6432(((UINT64)(u4PTSRp-u4PtsWP1+u4FifoEnd - u4FifoStart))* (u8PTS2 - u8PTS1),(UINT64)(u4PtsWP2 -u4PtsWP1 + u4FifoEnd - u4FifoStart ),NULL);
                        //LOG(0,"CASE3\n");
                    }
                }
                //final check for interpolation
                if ((u8PTSInt > u8PTS1) && (u8PTSInt < u8PTS2))
                {
                    *u8PTS = u8PTSInt;
                }
                else
                {
                    LOG(8,"Interpolation ERR! PTS1 = %lld, PTS2 = %lld, PTS = %lld\n", u8PTS1,u8PTS2,u8PTSInt);
                }

            }
        }
    }

    return fgFound;
#if 0
    if (u2Idx < u2LastResult)
        LOG(0,"AAAAAAAAAAAAAAAAAAA\n");
    u2LastResult = u2Idx;
#endif
}

#endif

//-----------------------------------------------------------------------------
// AUD_DrvDmxIntThreshold
//
/** Set DMX interrupt threshold.
 *
 *  @param[in]          u1DecId         Decoder ID
 *                      u1Factor        threshold = 188 * u1Factor
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DrvDmxIntThreshold(UINT8 u1DecId, UINT8 u1Factor)
{
    if ((u1Factor > 0) &&
        (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
    {
        _DMX_SetInterruptThreshold(DMX_PID_TYPE_ES_AUDIO, (UINT32)(1+188*(u1Factor-1)));
        LOG(5, "Set DMX threshold to 188*%d\n", u1Factor);
    }
}

#if 0
void AUD_MMAllocSyncInfo(UINT8 u1DecId, BOOL fgFree)
{
    UINT32 u4Size;

    u4Size = sizeof(AUD_MM_SYNC_INFO_T) * MM_SYNC_INFO_NUM;
    if (fgFree)
    {
        VERIFY(BSP_FreeAlignedDmaMemory(_paeMMSyncInfo));
        _paeMMSyncInfo = 0;
        LOG(5, "Free MM av sync memory 0x%d \n",_paeMMSyncInfo);
    }
    else
    {
        _paeMMSyncInfo = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, 64);
        if (_paeMMSyncInfo == NULL)
        {
            LOG(5, "Memory request for MM av sync memory fail !!\n");
        }
    }
}
#endif

BOOL AUD_IsMMAVInitSync(UINT8 u1DecId)
{
    BOOL fgRet = FALSE;

#ifdef CC_AUD_SUPPORT_MM_AV_INIT_SYNC

    // Only DTV & ATV need AV synchronization
    if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
    {
        fgRet = TRUE;
    }
    else
    {
        fgRet = FALSE;
    }

#else

    UNUSED(u1DecId);
    fgRet = FALSE;

#endif

    return fgRet;
}

void AUD_DrvMMAoutEnable(UINT8 u1DecId, BOOL fgEnable)
{
    UNUSED(fgEnable);

    MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_AOUT_ENABLE);

    //add condition to avoid duplicated dsp cmd to lower message queue loading
    //also..when we pause to debug DSP.. _Dtv_lockCheck will think there is no data and make SWDMX send this cmd crazily. thus system hung!
    if (_fgAudMMnotifySWDMX[u1DecId] &&(_arAudDecoder[AUD_DSP0][u1DecId].eDecState != AUD_DEC_STOP))
    {
        _fgAudMMnotifySWDMX[u1DecId] = FALSE;
        if (fgEnable)
        {        
            _arAudDecoder[AUD_DSP0][u1DecId].fgMMAoutEnable = TRUE;
        }
        if (u1DecId == AUD_DEC_MAIN)
        {
            LOG(5, "AUD_MMAoutEnable for MAIN decoder \n");
            DSP_SendDspTaskCmd(AUD_DSP0, DSP_CLEAR_AOUT_WAIT);
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            LOG(5, "AUD_MMAoutEnable for AUX decoder \n");
            DSP_SendDspTaskCmd(AUD_DSP0, DSP_CLEAR_AOUT_WAIT_DEC2);
        }
        else if (u1DecId == AUD_DEC_THIRD)  //parson MM3
        {
            LOG(5, "AUD_MMAoutEnable for THIRD decoder \n");
            DSP_SendDspTaskCmd(AUD_DSP0, DSP_CLEAR_AOUT_WAIT_DEC3);
        }
        else
        {
            LOG(5, "Not support AUD_MMAoutEnable for decoder 3 \n");
        }
        #if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
        AUD_Aproc_Aout_Ok(u1DecId);
        #endif        
        #ifdef CC_ENABLE_AOMX     
        if(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_GST)       
            (void)AUD_PlayMuteSetAoutEnableReady(40);
        #endif
    }
}

BOOL AUD_IsMMAoutEnable(UINT8 u1DecId)
{
    return _arAudDecoder[AUD_DSP0][u1DecId].fgMMAoutEnable;
}

#if 0   // Unused
BOOL AUD_IsMMAoutReady(UINT8 u1DecId)
{
    return _arAudDecoder[AUD_DSP0][u1DecId].fgMMAoutReady;
}
#endif

void _AudClearEOSFlag(UINT8 u1DecId)
{
    _arAudDecoder[AUD_DSP0][u1DecId].rCurrentPes.fgEOS = FALSE;
}

void AUD_DrvClearPesCnt(UINT8 u1DecId)
{
    _arAudDecoder[AUD_DSP0][u1DecId].u4ReceivePesCount = 0;
}
AUD_DEC_STREAM_FROM_T _AudGetStrSource(UINT8 u1DecId)
{
    return _arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom;
}

#ifdef CC_AUD_LINEIN_USE_DEC3
BOOL _AudGetADType(UINT8 u1DecId)
{
    return _arAudDecoder[AUD_DSP0][u1DecId].fgIsAD;
}
#endif
void AUD_DrvMMAoutEnableNotify(UINT8 u1DecId, BOOL fgResume)
{

#ifdef ENABLE_MULTIMEDIA
    ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotify;
    UNUSED(eAudioNotify);
#endif
    LOG(5, "%s: u1DecId(%d), fgResume(%d)\n", __FUNCTION__, u1DecId, fgResume);
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)||
        ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)&&(_fgMMPlusAD[u1DecId] ==TRUE)))
    {
#ifdef ENABLE_MULTIMEDIA
#ifdef CC_53XX_SWDMX_V2
        eAudioNotify = fgResume ? ENUM_SWDMX_AUDIO_NOTIFY_RESUME_DONE : ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE;
        if (_fgAudMMnotifySWDMX[u1DecId])
        {
            SWDMX_AudioNotify(_u1SwdmxSrcId[u1DecId],  eAudioNotify);
        }
#else
        if (_fgAudMMnotifySWDMX[u1DecId])
        {
             SWDMX_AudioNotify(eAudioNotify);
        }
#endif
#endif
    }
}

void AUD_DrvMMNoDataNotify(UINT8 u1DecId)
{
    if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)||
        ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)&&(_fgMMPlusAD[u1DecId] ==TRUE)))
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifdef CC_53XX_SWDMX_V2
        SWDMX_AudioNotify(_u1SwdmxSrcId[u1DecId], ENUM_SWDMX_AUDIO_NOTIFY_NO_DATA);
        #else
        SWDMX_AudioNotify(ENUM_SWDMX_AUDIO_NOTIFY_NO_DATA);
        #endif
        #endif
    }
}

#ifndef CC_AUD_SKYPE_SUPPORT
void AUD_DrvMMAoutEnable_Flag(BOOL fgEnable)
{
    _arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].fgMMAoutEnable = fgEnable;
}

void AUD_DrvMMAoutReady_Flag(BOOL fgEnable)
{
    _arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].fgMMAoutReady = fgEnable;
}
#else
void AUD_DrvMMAoutEnable_Flag(UINT8 u1DecId, BOOL fgEnable)
{
    _arAudDecoder[AUD_DSP0][u1DecId].fgMMAoutEnable = fgEnable;
}

void AUD_DrvMMAoutReady_Flag(UINT8 u1DecId, BOOL fgEnable)
{
    _arAudDecoder[AUD_DSP0][u1DecId].fgMMAoutReady = fgEnable;
}
#endif

AUD_FMT_T _AudGetStrFormat(UINT8 u1DecId)
{
    return _arAudDecoder[AUD_DSP0][u1DecId].eDecFormat;
}

#ifdef DSP_FORCE_RESET_ENABLE
// msut add AD protection!!!!!! by Shawn
static void _AudDspMonitorThread(void* pvArg) //  -- DSP Force --
{
    UINT32 u4AputBank = 0, u4PrvAputBank = 0xFFFFFFFF;
    UINT32 u4InputUnerRun = 0, u4PrvInputUnerRun = 0xFFFFFFFF;
    UINT32 u4ReadPtr = 0, u4PrvReadPtr = 0;
    UINT32 u4Tick = 0, u4Tick1 = 0, u4Tick2 = 0;
    UINT8  u1DecId;
    UINT32 u4DspMayBeDead_st0 = 0;
    UINT32 u4DspMayBeDead_st1 = 0;
    UINT32 u4NeedToRescueDsp = 0, u4NeedToRecoverPlay = 0;
    INT8 i = 0;

    UNUSED(u4NeedToRecoverPlay);
    x_thread_delay(DSP_MONITOR_THREAD_START_TIME);

    while (1)
    {
        /* ====================================
            DSP Hang detection
           ==================================== */
        if ((_arAudDrvState[AUD_DEC_MAIN] == AUD_ON_PLAY)
            && (_arAudDecoder[AUD_DEC_MAIN].eDecState == AUD_DEC_PLAYING)
            && _afgIssuePlayComToDsp[AUD_DEC_MAIN]
            && (_arAudDecoder[AUD_DEC_MAIN].eDecFormat != AUD_FMT_DETECTOR)) //For detector
        {
            u4Tick ++;
            u4InputUnerRun = u4ReadD2RInfo_InputUnderRun(AUD_DSP0, AUD_DEC_MAIN);

            LOG(11, "u4AputBank = %X(%X), u4InputUnerRun =%X(%X) [%d, %d, %d, %d]\n",
                u4AputBank, u4PrvAputBank,
                u4InputUnerRun, u4PrvInputUnerRun,
                u4DspMayBeDead_st0,  u4NeedToRescueDsp, u4NeedToRecoverPlay, u4DspResueTrigger);

            //------------------------
            // check data input
            //------------------------
            // There are 3 cases to check if DSP "MAY BE DEAD":
            // 1. If u4InputUnerRun is 0, that means input may be not started. It may happens
            //     at the transient state. Thus, we need another longer threshold to avoid the false alarm.
            //     Once the case remains, detector think the DSP "MAY BE DEAD".
            // 2. If u4InputUnerRun != u4PrvInputUnerRun, that means the input buffer is still used.
            //     That is decoder is still "ALIVE" whether if demux is filling data or not.
            // 3. If u4InputUnerRun != u4PrvInputUnerRun, that means DSP "MAY BE DEAD". Detector
            //     must do further dectection.
            if (u4InputUnerRun == 0)
            {
                // for transient state
                if (u4Tick >= (DSP_RESUCE_THRESHOLD_1 / DSP_MONITOR_THREAD_PERIOD))
                {
                    u4DspMayBeDead_st0 = 1; //enter state 0
                }
                else
                {
                    u4DspMayBeDead_st0 = 0;
                    u4DspMayBeDead_st1 = 0;
                }
            }
            else if (u4InputUnerRun == u4PrvInputUnerRun)
            {
                u4DspMayBeDead_st0 = 1;
                u4Tick = 0;
            }
            else
            {
                // alive, reset info
                u4Tick = 0;
            }
            u4PrvInputUnerRun = u4InputUnerRun;

            //------------------------
            // check data output
            //------------------------
            // Once DSP "MAY BE DEAD", aput bank is checked. If it remains over the threshold,
            // detector think DSP is "DEAD".
            if (u4DspMayBeDead_st0)
            {
                u4Tick1 ++;
                u4AputBank = u4ReadD2RInfo_AputBank(AUD_DSP0, AUD_DEC_MAIN);
                LOG(11,"u4AputBank = 0x%x, prev = 0x%x\n",u4AputBank,u4PrvAputBank);
                // DSP may be dead, check DSP output
                if (u4AputBank == u4PrvAputBank)
                {
                    if (u4Tick1 >= (DSP_RESUCE_THRESHOLD / DSP_MONITOR_THREAD_PERIOD))
                    {
                        // Goto state 1 for check readpointer
                        u4DspMayBeDead_st0 = 0;
                        u4DspMayBeDead_st1 = 1;
                        u4Tick1 = 0;

                    }
                }
                else
                {
                    // alive, reset info
                    u4DspMayBeDead_st0 = 0;
                    u4DspMayBeDead_st1 = 0;
                    u4Tick1 = 0;
                }
            }
            u4PrvAputBank = u4AputBank;

            //------------------------
            // check Read pointer
            //------------------------
            //
            //check Read pointer as a last line of defense
            //
            if (u4DspMayBeDead_st1)
            {
                u4Tick2 ++;
                if ((_arAudDecoder[AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||
                    (_arAudDecoder[AUD_DEC_AUX].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER))
                {
                    u4ReadPtr = u4ReadD2RInfo_ReadPtr(AUD_DEC_AUX);
                }
                else
                {
                    u4ReadPtr = u4ReadD2RInfo_ReadPtr(AUD_DEC_MAIN);
                }

                LOG(11,"    u4ReadPtr = 0x%x, prev = 0x%x\n",u4ReadPtr,u4PrvReadPtr);
                // DSP may be dead, check DSP ReadPointer
                if (u4ReadPtr == u4PrvReadPtr)
                {
                    if (u4Tick2 >= (DSP_RESUCE_THRESHOLD / DSP_MONITOR_THREAD_PERIOD))
                    {
                        // DSP dead! Try ro rescure...
                        u4NeedToRescueDsp = 1;
                        u4NeedToRecoverPlay = 1;
                        u4DspMayBeDead_st0 = 0;
                        u4DspMayBeDead_st1 = 0;
                        u4Tick2 = 0;
                    }
                }
                else
                {
                    // alive, reset info
                    u4DspMayBeDead_st0 = 0;
                    u4DspMayBeDead_st1 = 0;
                    u4Tick2 = 0;
                }
            }
            u4PrvReadPtr = u4ReadPtr;
        }
        else // decoder is not playing
        {
            u4DspMayBeDead_st0 = 0;
            u4DspMayBeDead_st1 = 0;
            u4Tick = 0;
            u4Tick1 = 0;
            u4Tick2 = 0;
        }

        /* ====================================
            UOP Timeout Check
           ==================================== */
        if (u4DspResueTrigger)
        {
              LOG(0, "\n !!!!!!!!! UOP TIMEOUT !!!!!!!!!!!!!! \n");
              u4NeedToRescueDsp = 1;
              u4NeedToRecoverPlay = 0;
              u4DspResueTrigger = 0;
        }

        /* ====================================
            DSP Rescue Action
           ==================================== */
        if (u4NeedToRescueDsp)
        {
            LOG(0, "\n\n ###################==>DSP 0245789334 ! ##############\n\n");

            // critical section start for block MW's commands
            AUD_DSPCmdSema_Lock();
            u4ADSP_DspReset();  //Power off and Power on DSP
            vDSPClearDecType(); //clear all dsp code types to make sure loading code ok.

            // send RESET commmand to driver thread.
            for (i = (AUD_DEC_NUM-1); i >= 0; i--)
            {
                u1DecId = (UINT8)i;
                LOG(0,">>>>>>>>>> Decoder %d <<<<<<<<<<<,,\n", u1DecId);
                VERIFY(AUD_DRVCmd(u1DecId, AUD_CMD_RESET));
                x_thread_delay(20);
            }

            // alive, reset info
            u4PrvAputBank = 0xFFFFFFFF;
            u4PrvInputUnerRun = 0xFFFFFFFF;
            u4PrvReadPtr = 0xFFFFFFFF;
            u4NeedToRescueDsp = 0;
            AUD_DSPCmdSema_UnLock();
        }


        x_thread_delay(DSP_MONITOR_THREAD_PERIOD);
    }
}
#endif

//-----------------------------------------------------------------------------
// _AudDrvResetDSP
//
/** Driver Force Reset flow,
 *  reset related setting and decoder by _ChangeDecFmtOnPlayState
 *
 *  @param[in]
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _AudDrvResetDSP(UINT8 u1DecId) //  -- DSP Force --
{
    LOG(0," #  _AudDrv 1995 DSP[%d] = %d\n",u1DecId, _arAudDrvState[u1DecId] );

    AUD_DEC_ID_VALIDATE(u1DecId);

    if (_arAudDrvState[AUD_DSP0][u1DecId] == AUD_ON_PLAY)
    {
        _AUD_IgnoreDecNotify(u1DecId, TRUE);

        // Reload code and stop/play dsp.
        _ChangeDecFmtOnPlayState(u1DecId);

        // Setup individual decoder setting by source <=== Don't do this, this reset system control and cause problem in dec3
        //_DecoderInitSetup(u1DecId, _arAudDecoder[u1DspId][u1DecId].eStreamFrom, _arAudDecoder[u1DspId][u1DecId].eDecFormat);

        x_thread_delay(50);

        if (u1DecId == AUD_DEC_MAIN)
        {
            // For MM files, need to triger play by self ...
            if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA)
            {
                AUD_DrvMMAoutEnable(u1DecId, TRUE);
            }

            // For non-line in source,turn off line-in.
            if ((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom != AUD_STREAM_FROM_HDMI) &&
                (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom != AUD_STREAM_FROM_LINE_IN)                 
                #ifdef CC_AUD_I2S_IN
                &&(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom != AUD_STREAM_FROM_I2S)
                #endif
                )
            {
                AUD_LineInCtrlEnable(u1DecId,FALSE);
            }
        }

        _AUD_IgnoreDecNotify(u1DecId, FALSE);
    }
}

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
BOOL AUD_DrvCheckIECSyncWord(AUD_HDMI_PARSER_INFO_T * prParserInfo)
{
    UINT16 u2SyncWord;    //16 bit word to check
    UINT8 i;

    u2SyncWord = AUD_DrvSwapWord(AUD_READIEC16(VIRTUAL(prParserInfo->eHDMIBuffer.u4FifoPtr)));
    prParserInfo->u4PaPb = (prParserInfo->u4PaPb<<16)|u2SyncWord;
    if((prParserInfo->u4PaPb == IEC_SYNCWORD_DTS_14)||(prParserInfo->u4PaPb == IEC_SYNCWORD_DTS_16))
    {
        prParserInfo->eSpdifInfo.u2format = 0xB; //find DTS header set pc as dts
        prParserInfo->_fgDTSCD = TRUE;   //DTS CD parser flag
        prParserInfo->_fgDTSSW = TRUE;  //DTS CD detect flg
        LOG(9,"Detect as DTS CD.\n");
    }
    else
    {
        prParserInfo->_fgDTSSW = FALSE;
    }
    if(prParserInfo->u4PaPb!=IEC_SYNCWORD_PA_PB)
    {
        return FALSE;
    }
    else
    {

        prParserInfo->_fgDTSCD = FALSE; //find papb,so it's not DTS CD,DTS CD don't have papb
        prParserInfo->u4PaPb = 0;
        prParserInfo->eHDMIBuffer.u4FifoPtr = AUD_DrvGetCircularAddress(u4GetHDMIParserFIFOStart(),u4GetHDMIParserFIFOEnd(),prParserInfo->eHDMIBuffer.u4FifoPtr+2);
        u2SyncWord = AUD_DrvSwapWord(AUD_READIEC16(VIRTUAL(prParserInfo->eHDMIBuffer.u4FifoPtr)))&0x1f;//pc bits of pc 0~4 data type
        for(i=0;i<AUD_RAW_NUM;i++)
        {
            if(u2SyncWord == _au2Pc[i])
            {
                break;
            }
        }
        if (prParserInfo->eSpdifInfo.u2format != u2SyncWord)
        {
            LOG(7, "Hdmi parser Pc change from %d to %d.\n", prParserInfo->eSpdifInfo.u2format, u2SyncWord);
        }
        prParserInfo->eSpdifInfo.u2format = u2SyncWord;
        //LOG(8,"prParserInfo->eSpdifInfo.u2format = 0x%x.\n",prParserInfo->eSpdifInfo.u2format);
        if(i<AUD_RAW_NUM)
        {
            return TRUE;

        }
        else
        {
            LOG(8,"unknow pc: %x.\n",u2SyncWord);
            return FALSE;
        }
    }
}

UINT16 AUD_DrvSwapWord(UINT16 u2swap)
{
    return(((u2swap&0xff)<<8)|((u2swap&0xff00)>>8));
}
UINT32 AUD_DrvGetCircularAddress(UINT32 u4StartAddress,UINT32 u4EndAddress,UINT32 u4Address)
{
    if(u4Address>=PHYSICAL(u4EndAddress))
    {
        return (u4Address-u4EndAddress+u4StartAddress);
    }
    else if(u4Address<PHYSICAL(u4StartAddress))
    {
        return (u4Address+u4EndAddress-u4StartAddress);
    }
    else
    {
        return u4Address;
    }
}
void AUD_DrvSetFgHDMIParserthread(UINT8 u1DecId,BOOL fgHDMIParserThread)
{
    if(_arParserInfo[u1DecId]._fgHDMIParserThreadGo!= fgHDMIParserThread)
    {
        _arParserInfo[u1DecId]._fgHDMIParserThreadGo = fgHDMIParserThread;
        if(fgHDMIParserThread)
        {            
               VERIFY(x_sema_unlock(_arParserInfo[u1DecId]._hSemaHDMIParserThreadWait) == OSR_OK);
        }
        else
        {
            _arParserInfo[u1DecId]._fgReset = TRUE;  //every parser thread disable, should reset parser setting, but in multithread system, it can't reset ,so add a flag for reset.
        }
    }
}

BOOL AUD_DrvCheckHDMISize(AUD_FIFO_INFO_T arfifo,UINT32 u4CheckSize)
{
    UINT32 hdmi_wp = AUD_GetLineinWP();
    UINT32 u4RemainSize;

    if((hdmi_wp<arfifo.u4FifoStartAddr)||(hdmi_wp>=arfifo.u4FifoEndAddr))
    {
        return FALSE;
    }
    if(hdmi_wp<arfifo.u4FifoPtr)
    {
        u4RemainSize = hdmi_wp+arfifo.u4FifoEndAddr-arfifo.u4FifoStartAddr-arfifo.u4FifoPtr;
    }
    else
    {
        u4RemainSize = hdmi_wp - arfifo.u4FifoPtr;
    }
    if(u4RemainSize>u4CheckSize)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void AUD_DrvGetParserFormat(UINT8 u1DecId, AUD_FMT_T *peDecFmt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    *peDecFmt = _arParserInfo[u1DecId].e_aud_det_fmt;
}

void AUD_DrvGetParserDetect(UINT8 u1DecId, BOOL *pfgDeteced)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    if(_arParserInfo[u1DecId].e_aud_det_fmt == AUD_FMT_PCM)
    {
        *pfgDeteced = TRUE;
    }
    if((_arParserInfo[u1DecId].u4NoRawDataSize > HDMI_PARSER_NON_DETECT_SIZE)&&
       (_arParserInfo[u1DecId].u4NoRawDataSize < HDMI_PARSER_DETECT_PCM_SIZE))
    {
        *pfgDeteced = FALSE;
    }
    else
    {
        *pfgDeteced = TRUE;
    }
}

AUD_FMT_T AUD_DrvPcToFormat(UINT8 u1DecId,UINT16 u2Pc)
{
    AUD_FMT_T e_aud_fmt = AUD_FMT_UNKNOWN;
    _arParserInfo[u1DecId]._fgRaw = TRUE;
    fgHDMIDTS = FALSE;

    AUD_DEC_ID_VALIDATE(u1DecId);
    switch(u2Pc)
    {
        case 0x1:
          if(AUD_DspIsSupportAC3()||AUD_DspIsSupportEAC3())
          {
              e_aud_fmt = AUD_FMT_AC3;
          }
          break;
        case 0x15:
          if(AUD_DspIsSupportEAC3())
          {
            e_aud_fmt = AUD_FMT_AC3;
          }
          break;
          case 0x4:
          case 0x5:
          case 0x6:
          case 0x8:
          case 0x9:
          e_aud_fmt = AUD_FMT_MPEG;
          break;
        case 0x7:
          if(AUD_DspIsFormatValid(AUD_FMT_AAC))
          {
          e_aud_fmt = AUD_FMT_AAC;
          }
          break;
        case 0xB:
        case 0xC:
        case 0xD:
          fgHDMIDTS = TRUE;
          if(AUD_DspIsFormatValid(AUD_FMT_DTS))
          {
            e_aud_fmt = AUD_FMT_DTS;
          }
          break;
        case 0x3:
          e_aud_fmt =  _arParserInfo[u1DecId].e_aud_det_fmt;
          break;
        default :
          _arParserInfo[u1DecId]._fgRaw = FALSE;
          e_aud_fmt = AUD_FMT_UNKNOWN;
          break;
    }
    return e_aud_fmt;
}
#else
UINT16 AUD_DrvSwapWord(UINT16 u2swap)
{
    return(((u2swap&0xff)<<8)|((u2swap&0xff00)>>8));
}
#endif

extern void AUD_SET_SWDMXSrcId(UINT8 u1DecId, UINT8 u1SwdmxSrcId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    _u1SwdmxSrcId[u1DecId] = u1SwdmxSrcId;
    UNUSED(_u1SwdmxSrcId[u1DecId]);
}

extern void AUD_GET_SWDMXSrcId(UINT8 u1DecId, UINT8* u1SwdmxSrcId)
{
    UNUSED(AUD_GET_SWDMXSrcId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    *u1SwdmxSrcId = _u1SwdmxSrcId[u1DecId];
}

#ifdef CC_ENABLE_AV_SYNC
void AUD_DrvCallback(UINT8 u1DecId, AUD_DSP_CB_T eAudDspCbType)
{
    UNUSED(u1DecId);
    UNUSED(AUD_DrvCallback);

    switch (eAudDspCbType)
    {
    case AUD_DSP_CB_UNMUTE:
        Aud_AddLogDump("Callback AUD_CMD_UNMUTE\n", 0);
        AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_UNMUTE);
        break;
    default:
        break;
    }
}
#endif

BOOL AUD_DrvDspCallBackProcess(UINT8 u1DecId, AUD_DEC_CMD_T eCmd)
{
    BOOL fgRet = FALSE;  //Set TRUE means this is a DSP callback command, default is FALSE.

    UNUSED(u1DecId);
    switch (eCmd)
    {
    case AUD_CMD_UNMUTE:
        fgRet = TRUE;
        break;
    default:
        break;
    }

    return fgRet;
}

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
void AUD_PlayMuteCmdQuery(UINT8 u1DecId)
{
    INT8 i1CmdQIndex;
    BOOL fgPrintOneTime;

    if (_u1CmdQIdx[u1DecId] == 0)
    {
        fgPrintOneTime = TRUE;
    }
    else
    {
        fgPrintOneTime = FALSE;
    }

    i1CmdQIndex = _u1CmdQIdx[u1DecId];

    while (i1CmdQIndex < AUD_PLAY_MUTE_MSG_Q_SIZE)
    {
        Printf("[%u.%06u]\tCmd(%s)\tArg(%d)\n",
            _stAudPmMsgQ[u1DecId][i1CmdQIndex].eTime.u4Seconds,
            _stAudPmMsgQ[u1DecId][i1CmdQIndex].eTime.u4Micros,
            _cAudPmCmdName[(UINT8)_stAudPmMsgQ[u1DecId][i1CmdQIndex].eAudPmCmd],
            _stAudPmMsgQ[u1DecId][i1CmdQIndex].u2Arg);

        i1CmdQIndex = i1CmdQIndex + 1;
    }

    if (!fgPrintOneTime)
    {
        i1CmdQIndex = 0;
        while (i1CmdQIndex < _u1CmdQIdx[u1DecId])
        {
            Printf("[%u.%06u]\tCmd(%s)\tArg(%d)\n",
                _stAudPmMsgQ[u1DecId][i1CmdQIndex].eTime.u4Seconds,
                _stAudPmMsgQ[u1DecId][i1CmdQIndex].eTime.u4Micros,
                _cAudPmCmdName[(UINT8)_stAudPmMsgQ[u1DecId][i1CmdQIndex].eAudPmCmd],
                _stAudPmMsgQ[u1DecId][i1CmdQIndex].u2Arg);

            i1CmdQIndex = i1CmdQIndex + 1;
        }
    }
}

void AUD_PlayMuteClearHistory(UINT8 u1DecId)
{
    UINT8 u1Idx;
    for (u1Idx=0; u1Idx<AUD_PLAY_MUTE_MSG_Q_SIZE; u1Idx++)
    {
        _stAudPmMsgQ[u1DecId][u1Idx].eAudPmCmd = AUD_PM_CMD_NUM;
    }
}

void AUD_PlayMuteQueryDelayInfo(UINT8 u1DecId)
{
    Printf("u1DecID = %d\n",u1DecId);    
    Printf("Video delay = %d (ms)\n", _u2VideoDelay[u1DecId]);
    Printf("Audio delay = %d (ms)\n", _u2AudioDelay[u1DecId]);
    Printf("DSP delay = %d*34/5 = %d (cm)\n", _u2AudioDelay[u1DecId], _u2AudioDelay[u1DecId]*34/5);
    Printf("DSP real delay = %d (cm)\n", AUD_DspGetChannelDelay(AUD_DSP0, AUD_CH_FRONT_LEFT));
}

BOOL AUD_PlayMuteCmd(UINT8 u1DecID, AUD_PLAY_MUTE_CMD_T eCmd, UINT16 u2Arg)
{
    UINT32 u4Msg;
    INT32 ret;

    u4Msg= ((UINT32)eCmd << 16) | (UINT32)u2Arg;

    ret = (x_msg_q_send(_hAudPlayMuteCmdQueue[u1DecID], &u4Msg, sizeof(UINT32), 1));
    if (ret != OSR_OK)
    {
        Printf("x_msg_q_send(_hAudPlayMuteCmdQueue(%d)) return %d",u1DecID ,ret);
    }

    VERIFY(ret == OSR_OK);

    return TRUE;
}

void AUD_PlayMuteEnable(BOOL fgEnable)
{
    _fgPlayMuteEnable = fgEnable;
}

static void _AudAdjustDelayByAudFmt(UINT16 u2VdpDelay, UINT16* pu2AudDelay)
{
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_FMT_T eAudDecFormat;
    INT16 i2CustDelayMs = 0;
#ifdef CC_AUD_DDI     
    INT16 i2Auddelay = 0;
#endif
    UNUSED(eAudDecFormat);
    UNUSED(eAudDecStreamFrom);
    UNUSED(i2CustDelayMs);

    LOG(7, "========================================\n");
    LOG(7, "[PLAY_MUTE] Delay adjustment information\n");
    LOG(7, "### Video delay = \t%d\n", u2VdpDelay);

    *pu2AudDelay = u2VdpDelay;
    AUD_DRVGetDecodeType(AUD_DSP0, AUD_DEC_MAIN, &eAudDecStreamFrom, &eAudDecFormat);

    // Adjust delay for HDMI source
    if (eAudDecStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        if (eAudDecFormat == AUD_FMT_PCM)
        {
            *pu2AudDelay = (u2VdpDelay <= 37)? 0 : (u2VdpDelay - 37); //PCM Prebuffer.
            LOG(7, "### Adjust for HDMI PCM = \t%d\n", *pu2AudDelay);
        }
    }

#ifdef CC_ENABLE_AOMX
    // Adjust delay for GST source
    if (eAudDecStreamFrom == AUD_STREAM_FROM_GST)
    {
        UINT32 u4Delay;

        vAudOmxGetGstDbgMsk(2, &u4Delay);

        if (u4Delay)
        {
            *pu2AudDelay = (UINT16)u4Delay;
        }
        else
        {
            //*pu2AudDelay = u2VdpDelay + 100;      //offset by 100ms for video unknown delay (30Hz * 3 frame = 100ms)
            *pu2AudDelay = u2VdpDelay; 
            LOG(7, "### Adjust for GStreamer = \t%d\n", *pu2AudDelay);
        }
    }
#endif

    // Adjust delay by cust table according to video Info ===
    i2CustDelayMs = _AudSrmGetVdoInfo();
    if (i2CustDelayMs != 0)
    {
        *pu2AudDelay += i2CustDelayMs;
        LOG(7, "### Adjust by customer table = \t%d\n", *pu2AudDelay);
    }
#if 0//def CC_AUD_DDI   --a2 delay,a5 not need custdelay
    if (eAudDecStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)// LG delay only table delay + AP delay
    {
        if(i2CustDelayMs < 0)
        {
            i2CustDelayMs = 0;
        }
        *pu2AudDelay = i2CustDelayMs;
    }
#endif

#ifdef CC_AUD_DDI 
    if (eAudDecStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
       i2Auddelay = u2VdpDelay - 25;
       if(eAudDecFormat == AUD_FMT_AC3)
       {         
         i2Auddelay = u2VdpDelay - 30;
       }
       if(eAudDecFormat == AUD_FMT_MPEG)
       {
         i2Auddelay = u2VdpDelay - 35;
       }
       if(i2Auddelay<0)  i2Auddelay = 0;
       
       *pu2AudDelay = i2Auddelay;
    }
#endif

#ifdef CC_AUD_SKYPE_ZERO_DELAY
    _fgSkypeMode = FeederGetSkypeSourceFlag();

    if ((_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) && (_fgSkypeMode))
    {
        // Set zero channel delay for Skype
        *pu2AudDelay = 0;
        LOG(7, "### Adjust for skype mode = \t%d\n", *pu2AudDelay);
    }
#endif
    LOG(7, "### Audio delay = \t%d\n", *pu2AudDelay);
    LOG(7, "========================================\n");
}

static INT16 _AudSetUnMuteCnt(void)
{
    INT16 i2UnMuteCnt = 0;

    if (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
#ifdef CC_MT5391_AUD_3_DECODER
        if((_arAudDecoder[AUD_DSP0][AUD_DEC_AUX].eSynMode == AV_SYNC_FREE_RUN) || //EU Audio Only Channel
           ((_arAudDecoder[AUD_DSP0][AUD_DEC_THIRD].eSynMode == AV_SYNC_FREE_RUN) && _IsTriOnlyDecMode()))
        {
            i2UnMuteCnt = AUD_PIP_MUTE_WITH_VDO_TIMEOUT / AUD_PLAY_MUTE_THREAD_DELAY;
        }
#else
        if (_arAudDecoder[AUD_DEC_MAIN].eSynMode == AV_SYNC_FREE_RUN)//US Audio Only Channel
        {
            i2UnMuteCnt = AUD_PIP_MUTE_WITH_VDO_TIMEOUT / AUD_PLAY_MUTE_THREAD_DELAY;
        }
#endif
        else
        {
            i2UnMuteCnt = AUD_MUTE_WITH_VDO_TIMEOUT / AUD_PLAY_MUTE_THREAD_DELAY;
        }
    }
    else if ((_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_LINE_IN)||
                (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_LINE_IN_2)||
                (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_LINE_IN_3))
    {
        i2UnMuteCnt = AUD_MUTE_WITH_AV_VDO_TIMEOUT / AUD_PLAY_MUTE_THREAD_DELAY;
    }
    else if (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        i2UnMuteCnt = AUD_MUTE_WITH_AV_VDO_TIMEOUT / AUD_PLAY_MUTE_THREAD_DELAY;
    }
    else if (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
    {
        i2UnMuteCnt = AUD_MUTE_WITH_AV_VDO_TIMEOUT / AUD_PLAY_MUTE_THREAD_DELAY;
    }

    return i2UnMuteCnt;
}

static INT16 _AudSetExtraUnMuteCnt(void)
{
    INT16 i2ExtraMuteCnt = 0;

    if (_fgAdMode)
    {
        //AD will be turned on, set extra delay
        i2ExtraMuteCnt = AUD_EXTRA_MUTE_AFTER_VDP / AUD_PLAY_MUTE_THREAD_DELAY;
    }

    i2ExtraMuteCnt = i2ExtraMuteCnt + (UINT16)DRVCUST_OptGet(eAudioExtraMute);

    return i2ExtraMuteCnt;
}

static void _AudPlayMuteMsgProcess(UINT8 u1DecID, AUD_PLAY_MUTE_CMD_T eCmd, UINT16 u2Arg)
{
    HAL_TIME_T stTime;

    HAL_GetTime(&stTime);
    _stAudPmMsgQ[u1DecID][_u1CmdQIdx[u1DecID]].eTime = stTime;
    _stAudPmMsgQ[u1DecID][_u1CmdQIdx[u1DecID]].eAudPmCmd = eCmd;
    _stAudPmMsgQ[u1DecID][_u1CmdQIdx[u1DecID]].u2Arg = u2Arg;

    _u1CmdQIdx[u1DecID] = _u1CmdQIdx[u1DecID] + 1;

    if (_u1CmdQIdx[u1DecID] == AUD_PLAY_MUTE_MSG_Q_SIZE)
    {
        _u1CmdQIdx[u1DecID] = 0;
    }
}

#if 1//ndef CC_AUD_DDI 
static void _AudPlayMuteThread(void* pvArg)
{
    UINT8 u1DecId = AUD_DEC_MAIN;
    // Event variables
    UINT32 u4Event;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(UINT32);
    INT32 i4Result;
    AUD_PLAY_MUTE_CMD_T eCmd;
    UINT16 u2Arg;

    BOOL fgVdpModeChg[AUD_DEC_MAX] = {FALSE,FALSE,FALSE}; 
    BOOL fgSetDelayToDsp[AUD_DEC_MAX] = {FALSE,FALSE,FALSE}; 
    BOOL fgGetDelayAout[AUD_DEC_MAX] = {FALSE,FALSE,FALSE}; 
    BOOL fgDelayTimeoutMode[AUD_DEC_MAX] = {FALSE,FALSE,FALSE}; 
    BOOL fgEnableSetDelay[AUD_DEC_MAX] = {FALSE,FALSE,FALSE}; 
    INT16 i2SetDelayCount[AUD_DEC_MAX] = {-1,-1,-1}; 
    UINT8 u1GetVideoCBCount[AUD_DEC_MAX] = {0,0,0}; 
    INT16 i2ExtraMuteCnt[AUD_DEC_MAX] = {-1,-1,-1}; 
    UINT16 u2VdpDelayMs, u2AudDelayMs;
    UINT32 u4VdoMuteFlag[AUD_DEC_MAX] = {0,0,0}; 
    BOOL fgCheckVdoUnmute[AUD_DEC_MAX] = {FALSE,FALSE,FALSE}; 
    INT16 i2UnMuteCount[AUD_DEC_MAX] = {-1,-1,-1}; 

    _fgPlayMuteEnable = TRUE;

    if (pvArg != NULL)
    {
        u1DecId = *(UINT8 *)pvArg;
    }    

    while (1)
    {
        i4Result = x_msg_q_receive(&u2MsgIdx, &u4Event, &zMsgSize,
                                   &_hAudPlayMuteCmdQueue[u1DecId], 1, X_MSGQ_OPTION_NOWAIT);
        VERIFY((i4Result == OSR_OK) || (i4Result == OSR_NO_MSG));

        if (_fgPlayMuteEnable)
        {
            if (i4Result == OSR_OK)
            {
                eCmd = (AUD_PLAY_MUTE_CMD_T)((UINT16)(u4Event >> 16));
                u2Arg = (UINT16)(u4Event & 0x0ffff);

                // Command queue
                _AudPlayMuteMsgProcess(u1DecId, eCmd, u2Arg);

                switch (eCmd)
                {
                case AUD_PM_CMD_RESET:
                    LOG(7, "[PLAY_MUTE](%d)AUD_PM_CMD_RESET\n", u1DecId);
                    fgEnableSetDelay[u1DecId] = FALSE;
                    #ifdef CC_AUD_DDI
                    if(_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom != AUD_STREAM_FROM_HDMI)
                    {
                        AUD_DspChannelDelay_initial(0, AUD_CH_ALL, AUD_DEC_MAIN);
                    }
                    #endif

                    i2UnMuteCount[u1DecId] = -1;
                    i2SetDelayCount[u1DecId] = -1;
                    fgGetDelayAout[u1DecId] = TRUE;
                    fgSetDelayToDsp[u1DecId] = FALSE;
                    AUD_DspDecPlayMuteEnable(u1DecId, FALSE);
                    _AudPlayMuteMsgProcess(u1DecId, AUD_PM_OP_UNMUTE, 0);
                    break;
                case AUD_PM_CMD_VDP_MODE_CHG:
                    LOG(7, "[PLAY_MUTE](%d)AUD_PM_CMD_VDP_MODE_CHG\n", u1DecId);
                    fgVdpModeChg[u1DecId] = TRUE;
                    u1GetVideoCBCount[u1DecId] = 0;
                    fgSetDelayToDsp[u1DecId] = TRUE;
                    break;
                case AUD_PM_CMD_MUTE:
                    LOG(7, "[PLAY_MUTE](%d)AUD_PM_CMD_MUTE\n", u1DecId);
                    AUD_DspDecPlayMuteEnable(u1DecId, TRUE);
                    u1GetVideoCBCount[u1DecId] = 0;
                    i2UnMuteCount[u1DecId] = u2Arg / AUD_PLAY_MUTE_THREAD_DELAY;
                    fgCheckVdoUnmute[u1DecId] = FALSE;
                    fgEnableSetDelay[u1DecId] = TRUE;
                    fgGetDelayAout[u1DecId] = FALSE;
                    LOG(7, "### i2UnMuteCount = %d\n", i2UnMuteCount[u1DecId]);
                    LOG(5, "[PLAY_MUTE](%d)Audio Mute!!!!!\n", u1DecId);
                    break;
                case AUD_PM_CMD_VDP_MODE_CHG_DONE:
                    LOG(7, "[PLAY_MUTE](%d)AUD_PM_CMD_VDP_MODE_CHG_DONE\n", u1DecId);
                    fgVdpModeChg[u1DecId] = FALSE;
                    u1GetVideoCBCount[u1DecId] = u1GetVideoCBCount[u1DecId] + 1;
                    if (fgEnableSetDelay[u1DecId])
                    {
                        i2SetDelayCount[u1DecId] = 0;
                        LOG(7, "### i2SetDelayCount = %d\n", i2SetDelayCount);
                    }
                    break;
                case AUD_PM_CMD_DSP_PLAY:
                    LOG(7, "[PLAY_MUTE](%d)AUD_PM_CMD_DSP_PLAY\n",u1DecId);
                    LOG(7, "### fgEnableSetDelay(%d) = %d\n", u1DecId, fgEnableSetDelay[u1DecId]);
                    
                    if ((fgEnableSetDelay[u1DecId])&&(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom != AUD_STREAM_FROM_MEMORY))
                    {
                        fgDelayTimeoutMode[u1DecId] = FALSE;
                        if (fgVdpModeChg[u1DecId])
                        {
                            fgDelayTimeoutMode[u1DecId] = TRUE;
							
                            if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
                            {
#ifdef CC_MT5391_AUD_3_DECODER
                                if((_arAudDecoder[AUD_DSP0][AUD_DEC_AUX].eSynMode == AV_SYNC_FREE_RUN) || //EU Audio Only Channel
                                   ((_arAudDecoder[AUD_DSP0][AUD_DEC_THIRD].eSynMode == AV_SYNC_FREE_RUN) && _IsTriOnlyDecMode()))
                                {
                                    fgDelayTimeoutMode[u1DecId] = FALSE;
                                }
#else
                                if (_arAudDecoder[AUD_DEC_MAIN].eSynMode == AV_SYNC_FREE_RUN)//US Audio Only Channel
                                {
                                    fgDelayTimeoutMode[u1DecId] = FALSE;
                                }
#endif
                            }
                        }

                        if (fgDelayTimeoutMode[u1DecId])
                        {
                            i2SetDelayCount[u1DecId] = VDP_MOD_CHG_DONE_WAITING_TIME_OUT / AUD_PLAY_MUTE_THREAD_DELAY;
                        }
                        else
                        {
                            i2SetDelayCount[u1DecId] = VDP_MOD_CHG_WAITING_TIME_OUT/ AUD_PLAY_MUTE_THREAD_DELAY;
                        }
						fgVdpModeChg[u1DecId] = FALSE;
                        LOG(7, "### i2SetDelayCount(%d) = %d\n", u1DecId, i2SetDelayCount[u1DecId]);
                    }
                    if ((_fgMMAudioOnly[u1DecId])||(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_MEMORY))// CR[DTV00490537]Audio only do not set channel delay
                    {
                        fgGetDelayAout[u1DecId] = TRUE; // Do not set channel delay so not Aout reinit --> fgGetDelayAout always = TRUE
                    }
                    break;
                case AUD_PM_CMD_UNMUTE_WITH_VDP:
                    LOG(7, "[PLAY_MUTE](%d)AUD_PM_CMD_UNMUTE_WITH_VDP\n",u1DecId);
                    LOG(7, "### fgGetDelayAout(%d) = %d\n", u1DecId, fgGetDelayAout[u1DecId]);
                    if (fgGetDelayAout[u1DecId])
                    {
                        i2UnMuteCount[u1DecId] = _AudSetUnMuteCnt();
                        i2ExtraMuteCnt[u1DecId] = _AudSetExtraUnMuteCnt();

                        fgCheckVdoUnmute[u1DecId] = TRUE;

                        if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
                        {
                            if (AUD_GetVideoScrambleStatus())
                            {
                                // Scramble video, audio free run
                                i2UnMuteCount[u1DecId] = i2ExtraMuteCnt[u1DecId];
                                fgCheckVdoUnmute[u1DecId] = FALSE;
                                LOG(7, "###(%d) Scramble video. Audio free run\n", u1DecId);
                            }
                        }

                        LOG(7, "### (%d)i2UnMuteCount = %d\n", u1DecId, i2UnMuteCount[u1DecId]);
                        LOG(7, "### (%d)i2ExtraMuteCnt = %d\n", u1DecId, i2ExtraMuteCnt[u1DecId]);

                        fgGetDelayAout[u1DecId] = FALSE;
                    }
                    break;
                case AUD_PM_CMD_UNMUTE_TIME:
                    LOG(7, "[PLAY_MUTE](%d)AUD_PM_CMD_UNMUTE_TIME\n",u1DecId);
                    if (!fgEnableSetDelay[u1DecId])
                    {
                        i2UnMuteCount[u1DecId] = u2Arg / AUD_PLAY_MUTE_THREAD_DELAY;
                        fgCheckVdoUnmute[u1DecId] = FALSE;
                        LOG(7, "###(%d) i2UnMuteCount = %d\n", u1DecId, i2UnMuteCount[u1DecId]);
                    }
                    break;
                case AUD_PM_CMD_SET_PIP_VDO_PATH:
                    LOG(7, "[PLAY_MUTE](%d)AUD_PM_CMD_SET_PIP_VDO_PATH\n", u1DecId);
                    fgVdpModeChg[u1DecId] = FALSE;
                    fgSetDelayToDsp[u1DecId] = TRUE;
                    break;
                default:
                    break;
                }
            }

            // Mute operation
            if (i2UnMuteCount[u1DecId] >= 0)
            {
                if (fgCheckVdoUnmute[u1DecId])
                {
                    if (_u1VideoFrameFlag == 0) //0: PIP MAIN, 1: PIP Sub
                    {
                        u4VdoMuteFlag[u1DecId] = _u4DrvVideoGetMute(SV_VP_MAIN);
                    }
                    else
                    {
                        u4VdoMuteFlag[u1DecId] = _u4DrvVideoGetMute(SV_VP_PIP);
                    }

                    if (u4VdoMuteFlag[u1DecId] == 0)
                    {
                        i2UnMuteCount[u1DecId] = i2ExtraMuteCnt[u1DecId];
                        fgCheckVdoUnmute[u1DecId] = FALSE;
                        u4VdoMuteFlag[u1DecId] = 1;
                        LOG(5, "[PLAY_MUTE](%d)Got video unmute message\n", u1DecId);
                        LOG(7, "### (%d) i2UnMuteCount = %d\n", u1DecId, i2UnMuteCount[u1DecId]);
                    }
                }
                else
                {
                    u4VdoMuteFlag[u1DecId] = 1;
                }

                if ((i2UnMuteCount[u1DecId] == 0) || (u4VdoMuteFlag[u1DecId] == 0))
                {
                    if(_fgHdmiAoutMute && (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom != AUD_STREAM_FROM_HDMI))
                    {
                        AudAoutGainCtrl(AUD_DEC_MAIN, 0, 0x1000,AOUT_VOL_SETP_4DB);
                        _fgHdmiAoutMute = FALSE;
                        LOG(1, "[PLAY_MUTE]!!!!!gogogo Audio Aout Gain UnMute!!!!!\n");
                    }
                    AUD_DspDecPlayMuteEnable(u1DecId, FALSE);
                    // Command queue
                    _AudPlayMuteMsgProcess(u1DecId, AUD_PM_OP_UNMUTE, 0);
                    LOG(1, "[PLAY_MUTE](%d)!!!!!gogogo Audio UnMute!!!!!\n", u1DecId);
                    LOG(7, "###(%d) i2UnMuteCount = %d\n", u1DecId, i2UnMuteCount[u1DecId]);
                    LOG(7, "###(%d) u4VdoMuteFlag = %d\n", u1DecId, u4VdoMuteFlag[u1DecId]);
                }

                i2UnMuteCount[u1DecId] = i2UnMuteCount[u1DecId] - 1;
            }

            // Delay operation
            if (i2SetDelayCount[u1DecId] >= 0)
            {
                if (((i2SetDelayCount[u1DecId] == 0) || (u1GetVideoCBCount[u1DecId] == 1))
                    && (_fgMMAudioOnly[u1DecId] == FALSE) && (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom != AUD_STREAM_FROM_MEMORY))
                {
                    u2VdpDelayMs = bDrvVideoGetFrameDelay(SV_VP_MAIN);
                    _AudAdjustDelayByAudFmt(u2VdpDelayMs, &u2AudDelayMs);
                    #ifdef CC_ENABLE_AOMX
                    if (!((_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom==AUD_STREAM_FROM_GST)||
                        (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom==AUD_STREAM_FROM_DIGITAL_TUNER)))
                    {
                            u2AudDelayMs = 0;
                    }
                    #endif
                    _u2VideoDelay[u1DecId] = u2VdpDelayMs;
                    _u2AudioDelay[u1DecId] = u2AudDelayMs;

                    if (fgSetDelayToDsp[u1DecId])
                    {
                        AUD_DspChannelDelay(AUD_DSP0, ((u2AudDelayMs*34)/5), AUD_CH_ALL, u1DecId);
                        LOG(5, "[PLAY_MUTE](%d)AUD_DspChannelDelay(%d)!!!!!\n", u1DecId, (u2AudDelayMs*34)/5);
                        // Command queue
                        _AudPlayMuteMsgProcess(u1DecId, AUD_PM_OP_SET_DELAY, 0);
                    }
                    else
                    {
                        //Only Write share info, but not send UOP. In HDMI case, there maybe Decoder reset.
                        #ifdef CC_AUD_DDI
                        if(_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom != AUD_STREAM_FROM_HDMI)
                        #endif
                        {
                            AUD_DspChannelDelay_initial(((u2AudDelayMs*34)/5), AUD_CH_ALL, u1DecId);
                        }
                        i2UnMuteCount[u1DecId] = 0;
                        fgCheckVdoUnmute[u1DecId] = FALSE;
                        LOG(5, "[PLAY_MUTE](%d)AUD_DspChannelDelay_initial(%d)\n", u1DecId, (u2AudDelayMs*34)/5);
                    }

                    fgEnableSetDelay[u1DecId] = FALSE;
                    fgSetDelayToDsp[u1DecId] = FALSE;
                    fgGetDelayAout[u1DecId] = TRUE;
                }


                i2SetDelayCount[u1DecId] = i2SetDelayCount[u1DecId] - 1;
            }
        }

        x_thread_delay(AUD_PLAY_MUTE_THREAD_DELAY);
    }
}
#else
static void _AudPlayMuteThread(void* pvArg) 
{
    // Event variables
    UINT8 u1DecId = AUD_DEC_MAIN; 
    UINT32 u4Event;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(UINT32);
    INT32 i4Result;
    AUD_PLAY_MUTE_CMD_T eCmd;
    UINT16 u2Arg;

    BOOL fgVdpModeChg = FALSE, fgCheckVdoUnmute = FALSE;
    BOOL fgChannelDelayAP = FALSE;
    BOOL fgSetDelayToDsp = FALSE;
    BOOL fgSetDelayToDspDone = FALSE;
    BOOL fgGetDelayAout = TRUE;
    BOOL fgDelayTimeoutMode = FALSE;
    BOOL fgEnableSetDelay = FALSE;
    INT16 i2SetDelayCount = -1;
    UINT8 u1GetVideoCBCount = 0;
    INT16 i2UnMuteCount = -1, i2ExtraMuteCnt = -1;
    UINT16 u2VdpDelayMs, u2AudDelayMs;
    UINT32 u4VdoMuteFlag = 0;

    UINT32 u4SetChannelDelayReady = 0x0;
    BOOL fgAoutEnableReady = FALSE;
    INT16 i2UnmuteTime = -1;

    _fgPlayMuteEnable = TRUE;
    
    if (pvArg != NULL)
    {
        u1DecId = *(UINT8 *)pvArg;
    }    
    
    while (1)
    {
        i4Result = x_msg_q_receive(&u2MsgIdx, &u4Event, &zMsgSize,
                                   &_hAudPlayMuteCmdQueue[u1DecId], 1, X_MSGQ_OPTION_NOWAIT);
        VERIFY((i4Result == OSR_OK) || (i4Result == OSR_NO_MSG));

        if (_fgPlayMuteEnable)
        {
            if (i4Result == OSR_OK)
            {
                eCmd = (AUD_PLAY_MUTE_CMD_T)((UINT16)(u4Event >> 16));
                u2Arg = (UINT16)(u4Event & 0x0ffff);

                // Command queue
                _AudPlayMuteMsgProcess(u1DecId, eCmd, u2Arg);

                switch (eCmd)
                {
                case AUD_PM_CMD_RESET:
                    LOG(7, "[PLAY_MUTE]AUD_PM_CMD_RESET\n");
                    #ifdef CC_ENABLE_AOMX
                    if((_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom != AUD_STREAM_FROM_GST) && 
                        (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom != AUD_STREAM_FROM_HDMI) && fgVdpModeChg)
                    #endif
                    {
                        AUD_DspChannelDelay_initial(0, AUD_CH_ALL, AUD_DEC_MAIN);
                    }
                    i2UnMuteCount = -1;
                    i2SetDelayCount = -1;
                    fgSetDelayToDsp = FALSE;

                    fgSetDelayToDspDone = FALSE;
                    u4SetChannelDelayReady = 0x0;
                    fgAoutEnableReady = FALSE;
                    i2UnmuteTime = -1;
                    fgChannelDelayAP = FALSE;
                    
                    break;
                case AUD_PM_CMD_VDP_MODE_CHG:
                    LOG(7, "[PLAY_MUTE]AUD_PM_CMD_VDP_MODE_CHG\n");
                    fgVdpModeChg = TRUE;
                    u1GetVideoCBCount = 0;
                    fgSetDelayToDsp = TRUE;
                    break;
                case AUD_PM_CMD_MUTE:
                    LOG(7, "[PLAY_MUTE]AUD_PM_CMD_MUTE\n");
                    #ifdef CC_ENABLE_AOMX  
                    if(!((_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_GST) && (AUD_DrvGetMMAudioOnly(u1DecId))))
                    #endif
                    {
                        AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, TRUE);
                    }
                    u1GetVideoCBCount = 0;
                    i2UnMuteCount = u2Arg / AUD_PLAY_MUTE_THREAD_DELAY;
                    fgCheckVdoUnmute = FALSE;
                    fgEnableSetDelay = TRUE;
                    fgGetDelayAout = TRUE;
                    LOG(7, "### i2UnMuteCount = %d\n", i2UnMuteCount);
                    LOG(5, "[PLAY_MUTE]Audio Mute!!!!!\n");
                    break;
                case AUD_PM_CMD_VDP_MODE_CHG_DONE:
                    LOG(7, "[PLAY_MUTE]AUD_PM_CMD_VDP_MODE_CHG_DONE\n");
                    fgVdpModeChg = FALSE;
                    if(fgChannelDelayAP)
                    {
                        u1GetVideoCBCount = u1GetVideoCBCount + 1;
                    }
                    if (fgEnableSetDelay&&fgChannelDelayAP)
                    {
                        i2SetDelayCount = 0;
                        LOG(7, "### i2SetDelayCount = %d\n", i2SetDelayCount);
                    }
                    break;
                case AUD_PM_CMD_DSP_PLAY:
                    LOG(7, "[PLAY_MUTE]AUD_PM_CMD_DSP_PLAY\n");
                    LOG(7, "### fgEnableSetDelay = %d\n", fgEnableSetDelay);
                    if (fgEnableSetDelay)
                    {
                        fgDelayTimeoutMode = FALSE;
                        if (fgVdpModeChg)
                        {
                            fgDelayTimeoutMode = TRUE;

                            if (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
                            {
#ifdef CC_MT5391_AUD_3_DECODER
                                if((_arAudDecoder[AUD_DSP0][AUD_DEC_AUX].eSynMode == AV_SYNC_FREE_RUN) || //EU Audio Only Channel
                                   ((_arAudDecoder[AUD_DSP0][AUD_DEC_THIRD].eSynMode == AV_SYNC_FREE_RUN) && _IsTriOnlyDecMode()))
                                {
                                    fgDelayTimeoutMode = FALSE;
                                }
#else
                                if (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eSynMode == AV_SYNC_FREE_RUN)//US Audio Only Channel
                                {
                                    fgDelayTimeoutMode = FALSE;
                                }
#endif
                            }
                        }
                        if((!fgChannelDelayAP) && ((fgVdpModeChg) || (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom != AUD_STREAM_FROM_DIGITAL_TUNER)))
                        {
                            i2SetDelayCount = VDP_MOD_CHG_DONE_WAITING_TIME_OUT / AUD_PLAY_MUTE_THREAD_DELAY;
                        } 
                        else if (fgDelayTimeoutMode)
                        {
                            i2SetDelayCount = VDP_MOD_CHG_DONE_WAITING_TIME_OUT / AUD_PLAY_MUTE_THREAD_DELAY;
                        }
                        else
                        {
                            i2SetDelayCount = VDP_MOD_CHG_WAITING_TIME_OUT / AUD_PLAY_MUTE_THREAD_DELAY;
                        }

                        if((_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_LINE_IN) && (!fgChannelDelayAP))
                        {
                            i2SetDelayCount += 100;
                        }
                        
                        LOG(7, "### i2SetDelayCount = %d\n", i2SetDelayCount);
                    }
                    break;
                case AUD_PM_CMD_UNMUTE_WITH_VDP:
                    LOG(7, "[PLAY_MUTE]AUD_PM_CMD_UNMUTE_WITH_VDP\n");
                    LOG(7, "### fgGetDelayAout = %d\n", fgGetDelayAout);
                    if (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_GST)
                    {
                        if(!fgChannelDelayAP)
                        u4SetChannelDelayReady = 0x3c3;
                    }
                    if (!fgGetDelayAout)
                    {
                        i2UnMuteCount = _AudSetUnMuteCnt();
                        i2ExtraMuteCnt = _AudSetExtraUnMuteCnt();

                        fgCheckVdoUnmute = TRUE;

                        if (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
                        {
                            if (AUD_GetVideoScrambleStatus())
                            {
                                // Scramble video, audio free run
                                i2UnMuteCount = i2ExtraMuteCnt;
                                fgCheckVdoUnmute = FALSE;
                                LOG(7, "### Scramble video. Audio free run\n");
                            }
                        }

                        if(fgChannelDelayAP && (!fgVdpModeChg) && (!fgSetDelayToDsp))
                        {
                            i2UnMuteCount = 10;
                        }

                        if(fgChannelDelayAP && (!fgSetDelayToDsp) 
                            && (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_LINE_IN) && _fgAVAudioOnly)
                        {
                            i2UnMuteCount = 10;
                        }
                        
                        LOG(7, "### i2UnMuteCount = %d\n", i2UnMuteCount);
                        LOG(7, "### i2ExtraMuteCnt = %d\n", i2ExtraMuteCnt);

                        fgGetDelayAout = TRUE;
                    }
                    break;
                case AUD_PM_CMD_UNMUTE_TIME:
                    LOG(7, "[PLAY_MUTE]AUD_PM_CMD_UNMUTE_TIME\n");
                    if (!fgEnableSetDelay)
                    {
                        i2UnMuteCount = u2Arg / AUD_PLAY_MUTE_THREAD_DELAY;
                        fgCheckVdoUnmute = FALSE;
                        LOG(7, "### i2UnMuteCount = %d\n", i2UnMuteCount);
                    }
                    break;
                case AUD_PM_CMD_SET_PIP_VDO_PATH:
                    LOG(7, "[PLAY_MUTE]AUD_PM_CMD_SET_PIP_VDO_PATH\n");
                    fgVdpModeChg = FALSE;
                    fgSetDelayToDsp = TRUE;
                    break;
                case AUD_PM_CMD_SET_DELAY_READY:
                    LOG(1, "[PLAY_MUTE]AUD_PM_CMD_SET_DELAY_READY\n");
                    u4SetChannelDelayReady |= (1<<u2Arg);
                    //fgAoutEnableReady;
                    //i2UnmuteTime;
                    LOG(1, "### fgAoutEnableReady(%d) u4SetChannelDelayReady(0x%x)\n", (UINT8)fgAoutEnableReady, u4SetChannelDelayReady);
                    break;

                case AUD_PM_CMD_AOUT_ENABLE:
                    LOG(1, "[PLAY_MUTE]AUD_PM_CMD_AOUT_ENABLE\n");
                    //u4SetChannelDelayReady;
                    fgAoutEnableReady = TRUE;
                    i2UnmuteTime = u2Arg / AUD_PLAY_MUTE_THREAD_DELAY;
                    LOG(1, "### i2UnmuteTime(%d) u4SetChannelDelayReady(0x%x)\n", i2UnmuteTime, u4SetChannelDelayReady);
                    break;

                default:
                    break;
                }
            }

            // Mute operation
            if (i2UnMuteCount >= 0)
            {
                if (fgCheckVdoUnmute)
                {
                    if (_u1VideoFrameFlag == 0) //0: PIP MAIN, 1: PIP Sub
                    {
                        u4VdoMuteFlag = _u4DrvVideoGetMute(SV_VP_MAIN);
                    }
                    else
                    {
                        u4VdoMuteFlag = _u4DrvVideoGetMute(SV_VP_PIP);
                    }

                    if (u4VdoMuteFlag == 0)
                    {
                        i2UnMuteCount = i2ExtraMuteCnt;
                        fgCheckVdoUnmute = FALSE;
                        u4VdoMuteFlag = 1;
                        LOG(5, "[PLAY_MUTE]Got video unmute message\n");
                        LOG(7, "### i2UnMuteCount = %d\n", i2UnMuteCount);
                    }
                }
                else
                {
                    u4VdoMuteFlag = 1;
                }

                if ((i2UnMuteCount == 0) || (u4VdoMuteFlag == 0) 
                    #if 0//def CC_ENABLE_AOMX     
                    || ((_arAudDecoder[AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_GST)&&(i2UnmuteTime == 0))
                    #endif
                    )
                {
                    if(_fgHdmiAoutMute && (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom != AUD_STREAM_FROM_HDMI))
                    {
                        AudAoutGainCtrl(0,0,0x1000,AOUT_VOL_SETP_4DB);
                        _fgHdmiAoutMute = FALSE;
                        LOG(1, "[PLAY_MUTE]!!!!!gogogo Audio Aout Gain UnMute!!!!!\n");
                    }
                    AUD_DspDecPlayMuteEnable(AUD_DEC_MAIN, FALSE);
                    // Command queue
                    _AudPlayMuteMsgProcess(u1DecId, AUD_PM_OP_UNMUTE, 0);
                    LOG(1, "[PLAY_MUTE]!!!!!gogogo Audio UnMute!!!!!\n");
                    LOG(7, "### i2UnMuteCount = %d\n", i2UnMuteCount);
                    LOG(7, "### u4VdoMuteFlag = %d\n", u4VdoMuteFlag);
                    LOG(1, "### i2UnmuteTime = %d\n", i2UnmuteTime);
                    i2UnMuteCount = 0;
                }

                i2UnMuteCount = i2UnMuteCount - 1;            
            }

            // Delay operation
            if (i2SetDelayCount >= 0)
            {
                if ((i2SetDelayCount == 0) ||
                    ((u1GetVideoCBCount == 1) && (!AUD_DrvGetMMAudioOnly(u1DecId))))
                {
                    u2VdpDelayMs = bDrvVideoGetFrameDelay(SV_VP_MAIN);
                    _AudAdjustDelayByAudFmt(u2VdpDelayMs, &u2AudDelayMs);
                    #ifdef CC_ENABLE_AOMX
                    if (!((_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom==AUD_STREAM_FROM_GST)||
                        (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom==AUD_STREAM_FROM_DIGITAL_TUNER)))
                    {
                            u2AudDelayMs = 0;
                    }
                    #endif
                    _u2VideoDelay[u1DecId] = u2VdpDelayMs;
                    _u2AudioDelay[u1DecId] = u2AudDelayMs;

                    if (fgSetDelayToDsp)
                    {
                        if(!fgSetDelayToDspDone)
                        {
                            fgSetDelayToDspDone = TRUE;
                            AUD_DspChannelDelay(AUD_DSP0, ((u2AudDelayMs*34)/5), AUD_CH_ALL, AUD_DEC_MAIN);
                            LOG(5, "[PLAY_MUTE]AUD_DspChannelDelay(%d)!!!!!\n", (u2AudDelayMs*34)/5);
                            // Command queue
                            _AudPlayMuteMsgProcess(u1DecId, AUD_PM_OP_SET_DELAY, 0);
                        }
                    }
                    else
                    {
                        #ifdef CC_ENABLE_AOMX
                        if(_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom == AUD_STREAM_FROM_GST)
                        {
                            AUD_DspChannelDelay(AUD_DSP0, ((u2AudDelayMs*34)/5), AUD_CH_ALL, AUD_DEC_MAIN);
                        }
                        else
                        #endif
                        {
                            //Only Write share info, but not send UOP. In HDMI case, there maybe Decoder reset.
                            if((_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom != AUD_STREAM_FROM_HDMI) && 
                                (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom != AUD_STREAM_FROM_GST) && fgVdpModeChg)
                            {
                                AUD_DspChannelDelay_initial(((u2AudDelayMs*34)/5), AUD_CH_ALL, AUD_DEC_MAIN);
                            }
                        }

                        i2UnMuteCount = 0;
                        fgCheckVdoUnmute = FALSE;
                        LOG(5, "[PLAY_MUTE]AUD_DspChannelDelay_initial(%d)\n", (u2AudDelayMs*34)/5);
                    }

                    fgEnableSetDelay = FALSE;  
                    fgSetDelayToDsp = FALSE;
                    fgGetDelayAout = FALSE;
                }


                i2SetDelayCount = i2SetDelayCount - 1;
            }
        }
        if((u4SetChannelDelayReady&0x03c3) && (!fgChannelDelayAP) && 
            (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eStreamFrom != AUD_STREAM_FROM_MEMORY))
        {
            fgChannelDelayAP = TRUE;
            if(u4SetChannelDelayReady != 0x3c3)
            {
                i2SetDelayCount = 13;
            }
            else
            {
                i2SetDelayCount = 0;
            }
        }

        // Wait Channel delay setting ready.
        if((i2UnmuteTime > 0) && fgAoutEnableReady)
        {
            if((u4SetChannelDelayReady&0x03c3) == 0x3c3)
            {
                i2UnmuteTime = 0;
            }
            else
            {
                i2UnmuteTime = i2UnmuteTime - 1;
            }
        }
        
        x_thread_delay(AUD_PLAY_MUTE_THREAD_DELAY);
    }
}
#endif  //CC_AUD_DDI
#endif  //CC_AUD_ENABLE_PLAY_MUTE_CONTROL

static void _AudCodecChangeThread(void* pvArg)
{
    while(1)
    {
        //Check Audio Codec Change Flag
        if (_fgAudCodecChang)
        {
            if(_AudCheckAputBankFreeze(AUD_DEC_MAIN))
            {
                LOG(5," ==> Audio output freeze <== \n");
                AUD_DSPCmdStop(AUD_DSP0, AUD_DEC_MAIN);
                _AudNotifyCodecChg(AUD_DEC_MAIN);
                _u4AputKeepCount = 0; //get freeze flag and clear counter
                _fgAudCodecChang = FALSE;//clear codec change flag
            }
            //LOG(7,"========= _AudCodecChangeThread =========\n");
        }
    
        x_thread_delay(AUD_PTR_UPDATE_TIMEOUT);
    }
}
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT

//#include "dsp_rg_ctl.h"

static UINT32 _gu4AudUploadMuxerCount = 0;
static UINT32 _gu4AudUploadQueueIndex = 0xffffffff;
static UINT8 _gu1AudUploadDataFlag = 0;

static UINT32 _gu4AudUploadDataPts = 0;
static UINT32 _gu4AudUploadDataPtsPrev = 0xffffffff;
static UINT32 _gu4AudUploadDataAddr = 0;
static UINT32 _gu4AudUploadDataSize = 0;

static UINT32 _gu4AudUploadDataBaseAdr = 0;
static UINT32 _gu4AudUploadDataTotalSize = 0;


///#define DUMP_UPLOAD_BUF_TO_USB
///#define ENC_TEST        /// Only for Test
#ifdef ENC_TEST
#define OUT_BUF_SIZE 1024*1024    ///1024*1024
static UINT8 _gu1OutBuf[OUT_BUF_SIZE];
static UINT32 _gu4OutBufLen = 0;
#endif

#if defined (CC_AUD_MP3ENC_FAKEDATA_SUPPORT) && defined (CONFIG_HIGH_RES_TIMERS)
#define MP3ENC_FRAME_HEADER_SIZE (0x36)
static UINT8 * _gulFakeMp3Frame;

#if defined (MP3ENC_320K_BR)
static UINT8 _gulFakeMp3Frame_Header[MP3ENC_FRAME_HEADER_SIZE] =
{
    0xFF, 0xFB, 0xE4, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0D, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xA4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34,
    0x80, 0x00, 0x00, 0x00
};
#elif defined (MP3ENC_192K_BR)
static UINT8 _gulFakeMp3Frame_Header[MP3ENC_FRAME_HEADER_SIZE] =
{
    0xFF, 0xFB, 0xB4, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0D, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xA4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34,
    0x80, 0x00, 0x00, 0x00
};
#else ///defined (MP3ENC_128K_BR)
static UINT8 _gulFakeMp3Frame_Header[MP3ENC_FRAME_HEADER_SIZE] =
{
    0xFF, 0xFB, 0x94, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0D, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xA4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34,
    0x80, 0x00, 0x00, 0x00
};
#endif
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
static UINT32 _gu4AudReadPtr = 0;
#endif

#define AUD_UPLOAD_DATA_QUEUE_MAX_NUM 128
#define AUD_DEC_UPLOAD_NUM  1

typedef struct __audUploadQueue {
    UINT32 u4StartAddr; // in bytes
    UINT32 u4Size; // in bytes
} _audUploadQueue_t;

typedef enum
{
    PCM_INIT_BITS = -1,
    PCM_16BITS = 0,
    PCM_24BITS = 1,

} UPLOAD_PCM_BITS;

typedef enum
{
    PCM_INIT_CH = -1,
    PCM_2CH = 0,
    PCM_1CH = 1,
    PCM_4CH = 2,

} UPLOAD_PCM_CHANNEL;

typedef enum
{
    PCM_INIT_SR = -1,
    PCM_48K = 0,
    PCM_44K = 1,
    PCM_32K = 2,

} UPLOAD_PCM_FS;

typedef struct __audUploadPCMInfo {
    UPLOAD_PCM_BITS     uPCMBits;        // in bytes
    UPLOAD_PCM_CHANNEL  uPCMChannels;    // in bytes
    UPLOAD_PCM_FS       uPCMSampleRate;  // in bytes
} _audUploadPCMInfo_t;

typedef struct AENC_COPY_DATA_TYPE_T
{
    UINT32  channel;    // Encoder index
    UINT64  pts;        // PTS
    UINT32  *pData;     // pointer to Audio Data
    UINT32  dataLen;    // Audio Data Length
    UINT32  *pRStart;   // start pointer of buffer
    UINT32  *pREnd;     // end pointer of buffer
}AENC_COPY_DATA_TYPE_T;

static _audUploadQueue_t _gu4AudUploadQueue[AUD_UPLOAD_DATA_QUEUE_MAX_NUM];
static _audUploadPCMInfo_t _guAudUploadPCMInfo[AUD_DEC_UPLOAD_NUM];

void _AUD_DspDataUploadNotify (UINT32 u4Index)  /// call by dsp_irq.c
{
#ifdef CC_AUD_UPLOAD_DSP_IRQ_QUEUE
    _AUD_UPLOAD_MESSAGE strMsg;
    strMsg.u4Index = (UINT32)u4Index;
    strMsg.u4STC = _AUD_ReadPsrStc1();
    _gu4AudUploadQueueIndex = u4Index;
    if (_gu1AudUploadDataFlag == 0x1)
    {
        /// Send Command Queue.
        VERIFY((x_msg_q_send(_hAudUploadDspIRQCmdQueue, &strMsg, sizeof(_AUD_UPLOAD_MESSAGE), 1)) == OSR_OK);
    }
#else
    _gu4AudUploadQueueIndex = u4Index; //Daniel Hack??
    if (_gu1AudUploadDataFlag == 0x1)
    {
        VERIFY(x_sema_unlock(_hSemaDataUploadWait) == OSR_OK);
    }
#endif
}

#if 0   // Unused
UINT32 _AudDataUploadGetPts (void)
{
   return  _gu4AudUploadDataPts;
}

UINT8 _AUD_DataUploadGetMode(void)
{
    return _gu1AudUploadMode;
}
#endif

void _AUD_DataUploadSetMode(UINT8 u1Mode)
{
    _gu1AudUploadMode = u1Mode;

}

void _AUD_UploadPCMInfo_Init(void)
{
    /// Set initial value for MW query.
    _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMBits = PCM_16BITS;
    _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMChannels = PCM_2CH;
    _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMSampleRate = PCM_48K;
}

void _AUD_DataUpload_Init (void)
{
    _gu1AudUploadDataFlag = 0;
    _gu4AudUploadMuxerCount = 0;
    _gu4AudUploadDataPts = 0;
    _gu4AudUploadDataPtsPrev = 0xffffffff;
    _gu4AudUploadDataAddr = 0;
    _gu4AudUploadDataSize = 0;
    _gu4AudUploadQueueIndex = 0xffffffff;

#ifdef ENC_TEST   /// Only for Test
    _gu4OutBufLen = 0;
#endif
}

void _AUD_DataUpload_start (void)
{
    _gu1AudUploadDataFlag = 0x1;
}

void _AUD_DataUploadQueueInit (UINT32 u4BaseAddr, UINT32 u4BlockSize, UINT32 u4BlockNum)
{
    UINT32 u4Blk, u4StartAddr;

    VERIFY(u4BlockNum <= uReadShmUINT8(AUD_DSP0, B_UPLOAD_BLOCK_NUM));  /// AUD_UPLOAD_DATA_QUEUE_MAX_NUM);

    u4StartAddr = u4BaseAddr;

    _gu4AudUploadDataBaseAdr = u4BaseAddr;
    _gu4AudUploadDataTotalSize = u4BlockNum * u4BlockSize;

    for (u4Blk = 0; u4Blk < u4BlockNum; u4Blk ++)
    {
        _gu4AudUploadQueue[u4Blk].u4StartAddr = u4StartAddr;
        _gu4AudUploadQueue[u4Blk].u4Size = u4BlockSize;
        u4StartAddr += u4BlockSize;
    }
}

//-------------------------------------------------------------------------
/** _AUD_ReadPsrStc1
 *  Read Parser STC 1
 */
//-------------------------------------------------------------------------
UINT32 _AUD_ReadPsrStc1 (void)
{
    return PARSER_READ32(REG_V_STC_1);
}

//-------------------------------------------------------------------------
/** _AUD_SetPsrStc1
 *  Set Parser STC 1
 */
//-------------------------------------------------------------------------
void _AUD_SetPsrStc1 (UINT32 u4Val)
{
    PARSER_WRITE32(REG_V_STC_1, u4Val);
}


///#define AUD_UPLOAD_QUEUE_BLOCKSIZE 0x400
///#define AUD_UPLOAD_QUEUE_BLOCKNUM 64
///#define AUD_UPLOAD_BUFFER_BASE (0x39D00 * 4)

static void _AudMuxerInit (void)
{
    _gu4AudUploadMuxerCount = 0;
    _gu4AudUploadQueueIndex = 0xffffffff;

    _AUD_UploadPCMInfo_Init();

    _AUD_DataUploadQueueInit (_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE,
                                u2ReadShmUINT16(AUD_DSP0, W_UPLOAD_BLOCK_SIZE),
                                uReadShmUINT8(AUD_DSP0, B_UPLOAD_BLOCK_NUM));

    // enable upload data function
    //_gu1AudUploadDataFlag = 0x1;
    _gu1AudUploadDataFlag = 0;

    _gu4AudUploadDataPts = 0;

    _AUD_SetPsrStc1 (0);
}

#if defined(AUD_ENC_DUMP_ENABLE) || defined(DUMP_UPLOAD_BUF_TO_USB) || defined(CC_ENABLE_AOMX) || defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_ADECOMX)

#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/param.h>

INT32 _AudDumpAudioData (const INT8 *fileName, const INT8 *buf, size_t count)
{
    mm_segment_t oldfs;
    struct file *filep  = NULL;
    size_t ret;

    oldfs = get_fs ();
    set_fs (KERNEL_DS);
    filep = filp_open (fileName, (O_CREAT | O_WRONLY | O_APPEND), 0);
    if ((IS_ERR (filep)) || (NULL == filep) || (NULL == filep->f_op) || (NULL == filep->f_op->write))
    {
        set_fs (oldfs);
        printk (KERN_DEBUG "_AudDumpAudioData error!\n");
        return -1;
    }

    ret = filep->f_op->write (filep, buf, count, &filep->f_pos);

    filp_close (filep, 0);
    set_fs(oldfs);

    return 0;
}

#endif

extern UINT32 u4GetUploadFIFOStart(void);
extern UINT32 u4GetUploadFIFOEnd(void);

static void _AudInformMuxer(UINT32 u4Pts, UINT32 u4StartAddr, UINT32 u4Len, UINT32 u4Index)
{
    static UINT32 u4TotalSize = 0;
    // send msg to mux
    // picSa, picSz, PTS, picType
#ifdef CC_SUPPORT_MUXER
    AENC_PES_INFO_T rAEncPes;
#endif
    AENC_COPY_DATA_TYPE_T rAEncInfo;
    UINT32  u4UploadFIFOStart = u4GetUploadFIFOStart();
    UINT32  u4UploadFIFOEnd = u4GetUploadFIFOEnd();
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    AudDecNfyFct   pfAudDecNfy = NULL;
    AudDecNfyFct   pfAudDecNfy1 = NULL;
//#endif
#ifdef DUMP_UPLOAD_BUF_TO_USB
    size_t ret;
    /*
        FIXME: check USB device path (plug USB disk and find path log)

        Example:
        [FM-Linux]Try to mount /dev/sda1 to /mnt/usb/sda1...
        [FM-Linux]Mount /dev/sda1 to /mnt/usb/sda1 successfully.
                                                  -------------- USB path
    */
    char *fileName = "/mnt/usb/sda1/AENC.mp3";
#endif
    rAEncInfo.channel = 2;
    rAEncInfo.pts = u4Pts;
    rAEncInfo.pData = (UINT32 *)u4StartAddr;
    rAEncInfo.dataLen = u4Len;
    rAEncInfo.pRStart = &u4UploadFIFOStart;
    rAEncInfo.pREnd = &u4UploadFIFOEnd;

    u4TotalSize += u4Len;
    LOG(5,"send:pts=%x, pData=%x. dataLen=%x,pRStart=%x,pREnd=%x totalsz:%x\n",u4Pts,u4StartAddr,u4Len,u4UploadFIFOStart,u4UploadFIFOEnd, u4TotalSize);

#ifdef ENC_TEST   /// Only for Test
    if ((_gu4OutBufLen + u4Len) <= OUT_BUF_SIZE)
    {
        ///Make sure DRAM cache had flushed.
        //DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_COMM);
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

        LOG (7, "Data upload to Muxer = (%d) Pts = 0x%X, Start Addr = 0x%X, length = 0x%X, location = 0x%X\n",
            u4Index, u4Pts, VIRTUAL(u4StartAddr), u4Len, VIRTUAL(&_gu1OutBuf[_gu4OutBufLen]));

        LOG (7, "0x%x \n", (UINT16 *)VIRTUAL(u4StartAddr));    ///first 2 byte


        x_memcpy ((VOID *)VIRTUAL(&_gu1OutBuf[_gu4OutBufLen]), (VOID *)VIRTUAL(u4StartAddr), u4Len);

        LOG (7, "0x%02x%02x   0x%02x%02x\n", _gu1OutBuf[_gu4OutBufLen], _gu1OutBuf[_gu4OutBufLen+1], _gu1OutBuf[_gu4OutBufLen+2], _gu1OutBuf[_gu4OutBufLen+3]);

        if((_gu1OutBuf[_gu4OutBufLen]==0)&&(_gu1OutBuf[_gu4OutBufLen+1]==0)&&(_gu1OutBuf[_gu4OutBufLen+2]==0)&&(_gu1OutBuf[_gu4OutBufLen+3]==0))
        {
            LOG (7, "@@@@@@@@@@@@@@@\n");
        }

        _gu4OutBufLen += u4Len;
    }
    else
    {
        LOG(7, "Buffer Address = (0x%x), Length = (0x%x).\n", VIRTUAL(&_gu1OutBuf[0]), _gu4OutBufLen);
    }
#else
    ///Make sure DRAM cache had flushed.
    //DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_COMM);
    DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    LOG (7, "Data upload to Muxer = (%d) Pts = 0x%X, Start Addr = 0x%X, length = 0x%X\n",
            u4Index, u4Pts, u4StartAddr, u4Len);
#endif

#ifdef DUMP_UPLOAD_BUF_TO_USB
    ret = _AudDumpAudioData(fileName, (INT8 *)(VIRTUAL(u4StartAddr)), u4Len);
    if (ret < 0)
    {
        LOG (0, "_AudDumpAudioData ERROR!\n");
    }
#endif

#ifndef CC_AUD_SUPPORT_ENC
#ifdef CC_SUPPORT_MUXER
    rAEncPes.u4FifoStart = 0;
    rAEncPes.u4FifoEnd = 0;
    rAEncPes.u4StartAddr = u4StartAddr;
    rAEncPes.u4FrameSize = u4Len;
    rAEncPes.u4Pts = u4Pts;
    MUXER_SendAudioPes(_ui1MuxId, &rAEncPes);
#endif
#endif

    // Get notify function
#ifdef CC_AUD_SUPPORT_ENC
    _CB_PutEvent(CB_MTAL_AUDDEC_ENC_DATA_ENABLE_TRIGGER, sizeof(AENC_COPY_DATA_TYPE_T), &rAEncInfo);//for encmp3
#else
    UNUSED(rAEncInfo);
#endif    
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy1) == TRUE);

    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, 0, AUD_COND_UPLOAD_INFO_CHG, (UINT32)u4StartAddr, u4Len);
    }
    if (pfAudDecNfy1 != NULL)
    {
        pfAudDecNfy1((void *)AUD_NFY_YW, 0, (AUD_COND_NFY_T)AUD_COND_UPLOAD_INFO_CHG, (UINT32)u4StartAddr, u4Len);
    }

}

#if defined (CC_AUD_MP3ENC_FAKEDATA_SUPPORT) && defined (CONFIG_HIGH_RES_TIMERS)
BOOL AUD_GetEncDataStatus(void)
{
    UINT8 u1DecId;
    UINT32 u4CurrAputBank;
    static UINT32 u4LastAputBank = 0;

    DSP_GetEncDecId(&u1DecId);
    u4CurrAputBank = u4ReadD2RInfo_AputBank(u1DecId);
    if(u4LastAputBank == u4CurrAputBank)
    {
        return FALSE;
    }
    u4LastAputBank = u4CurrAputBank;
    return TRUE;
}

VOID AUD_InformMuxerFake(void)
{
    #ifdef CC_SUPPORT_MUXER
    AENC_PES_INFO_T rAEncPes;

    rAEncPes.u4FifoStart = 0;
    rAEncPes.u4FifoEnd = 0;
    rAEncPes.u4StartAddr = (UINT32)_gulFakeMp3Frame;
    #ifdef MP3ENC_SUPPORT
    rAEncPes.u4FrameSize = MP3ENC_FRAME_SIZE;
    #endif
    rAEncPes.u4Pts = _AUD_ReadPsrStc1();
    MUXER_SendAudioPes(_ui1MuxId, &rAEncPes);
    #endif
}

static enum hrtimer_restart AUD_TimerCallBack(struct hrtimer *timer)
{
    UINT32 u4STC;
        
    hrtimer_forward_now(timer, period_time);
    u4STC = _AUD_ReadPsrStc1();

    if((u4STC >= (gu4UploadTimeExceed + MP3ENC_FRAME_STC_ELAPSED/2)) && (!AUD_GetEncDataStatus()))
    {
        AUD_InformMuxerFake();
        LOG(1, "u4STC:%d  gu4UploadTimeExceed:%d  AUD_GetEncDataStatus:%d\n", u4STC, gu4UploadTimeExceed, AUD_GetEncDataStatus());
    }
    return HRTIMER_RESTART;
}

VOID AUD_CreateTimer(UINT32 u4period)
{
    if(!fgIsTimerCreated)
    {
        hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        timer.function = AUD_TimerCallBack;
        period_time = ktime_set(0, (u4period*1000000));
        fgIsTimerCreated = TRUE;
    }
}

VOID AUD_DeleteTimer(void)
{
    return;
}

VOID AUD_StartTimer(UINT32 u4delay)
{
    if(fgIsTimerCreated)
    {
        base_time = ktime_set(0, (u4delay*1000000));
        hrtimer_start(&timer, base_time, HRTIMER_MODE_REL);
    }
}

VOID AUD_StopTimer(void)
{
    if(fgIsTimerCreated)
    {
        hrtimer_cancel(&timer);
    }
}
#endif


#ifdef CC_AUD_SUPPORT_ENC
#define AUD_ENC_BUFFER_SIZE 8192 * 16   // The same as mtaudenc outBufferSize
#define AUD_ENC_DUMP_FILE_NAME "/mnt/usb/sda1/AENC.dump"

//#define AUD_ENC_DUMP_ENABLE
static UINT8 _gAudEncBuf[AUD_ENC_BUFFER_SIZE];
static UINT32 _gAudEncWp = 0;
void AUD_EncInformMuxer(UINT32 u4Pts, UINT32 u4StartAddr, UINT32 u4Len)
{
    UINT32 u4EncBufAddr = (UINT32)_gAudEncBuf;
    
    if (u4Len > AUD_ENC_BUFFER_SIZE)
    {
        LOG(5, "ENC data len exceed buffer size!!!\n");
        return ;
    }

    _gAudEncWp = (_gAudEncWp + u4Len) > AUD_ENC_BUFFER_SIZE ? 0 : _gAudEncWp;
    u4EncBufAddr += _gAudEncWp;
    _gAudEncWp += u4Len;
    
    x_memcpy(u4EncBufAddr, u4StartAddr, u4Len);

#ifdef AUD_ENC_DUMP_ENABLE
#ifdef LINUX_TURNKEY_SOLUTION
    _AudDumpAudioData(AUD_ENC_DUMP_FILE_NAME, u4EncBufAddr, u4Len);
#endif
#endif

#ifdef CC_SUPPORT_MUXER
    AENC_PES_INFO_T rAEncPes;

    rAEncPes.u4FifoStart = 0;
    rAEncPes.u4FifoEnd = 0;
    rAEncPes.u4StartAddr = u4EncBufAddr;
    rAEncPes.u4FrameSize = u4Len;
    rAEncPes.u4Pts = u4Pts;
    MUXER_SendAudioPes(_ui1MuxId, &rAEncPes);
#endif
}
#endif

static void _AudDataUploadSetPts (UINT32 u4Pts)
{
    _gu4AudUploadDataPts = u4Pts;
}

static void _AudDataUploadThread(void* pvArg)
{
    //UINT32 *u4QueueAddr;
    UINT32 u4UploadAddr;
    UINT32 u4UploadSize = 0xFFFFFFFF;
    UINT32 u4Time;

#ifdef CC_AUD_UPLOAD_DSP_IRQ_QUEUE
    _AUD_UPLOAD_MESSAGE strMsg;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(_AUD_UPLOAD_MESSAGE);
#endif

    #if defined (CC_AUD_MP3ENC_FAKEDATA_SUPPORT) && defined (CONFIG_HIGH_RES_TIMERS)
    _gulFakeMp3Frame = (UINT8 *)BSP_AllocAlignedDmaMemory(MP3ENC_FRAME_SIZE, 32);
    x_memset((VOID *)VIRTUAL((UINT32)_gulFakeMp3Frame), 0, MP3ENC_FRAME_SIZE);
    x_memcpy((VOID *)VIRTUAL((UINT32)_gulFakeMp3Frame), (VOID *)(_gulFakeMp3Frame_Header), MP3ENC_FRAME_HEADER_SIZE);
    AUD_CreateTimer(MP3ENC_FRAME_TIME_ELAPSED);
    #endif

    #ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
    AUD_OSS_ADAP_MSG_T tMsg;
    AUD_OSS_ADAP_DEV_T devId;
    AUD_OSS_ADAP_ST_TYPE_T status;
    AUD_OSS_ADAP_SRC_TYPE_T source;
    #endif
    while (1)
    {
        #ifdef CC_AUD_UPLOAD_DSP_IRQ_QUEUE
        /// Receive Command Queue.
        VERIFY(x_msg_q_receive(&u2MsgIdx, &strMsg, &zMsgSize,
            &_hAudUploadDspIRQCmdQueue, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
        LOG(3,"_gu4AudUploadQueueIndex=%d, strMsg.u4Index=%d, STC:%d\n", _gu4AudUploadQueueIndex, strMsg.u4Index, strMsg.u4STC);
        u4UploadAddr = _gu4AudUploadQueue[strMsg.u4Index].u4StartAddr;
        u4UploadSize = (UINT32)(u2ReadShmUINT16(AUD_DSP0, W_UPLOAD_BLOCK_SIZE) * uReadShmUINT8(AUD_DSP0, B_UPLOAD_BLK_INT_CNT));
        u4Time = strMsg.u4STC;
        #else
        VERIFY(x_sema_lock(_hSemaDataUploadWait, X_SEMA_OPTION_WAIT) == OSR_OK);
        u4UploadAddr = _gu4AudUploadQueue[_gu4AudUploadQueueIndex].u4StartAddr;
        u4UploadSize = (UINT32)(u2ReadShmUINT16(AUD_DSP0, W_UPLOAD_BLOCK_SIZE) * uReadShmUINT8(AUD_DSP0, B_UPLOAD_BLK_INT_CNT));
        u4Time = _AUD_ReadPsrStc1();
        #endif
        
        _AudDataUploadSetPts(u4Time);
        _gu4AudUploadDataAddr = u4UploadAddr;
        _gu4AudUploadDataSize = u4UploadSize;

        #if defined (CC_AUD_MP3ENC_FAKEDATA_SUPPORT) && defined (CONFIG_HIGH_RES_TIMERS)
        gu4UploadTimeExceed = u4Time;
        #endif

        _AudInformMuxer(u4Time, u4UploadAddr, u4UploadSize, _gu4AudUploadQueueIndex);

        #ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
        devId = _AudOssAdap_GetDevId();
        status = _AudOssAdap_GetStatus();
        source = _AudOssAdap_GetSourceType();
        if((AUD_OSS_ADAP_ST_WORKING == status) && (AUD_OSS_ADAP_UPLOAD == source))
        {

            tMsg.devId = devId;
            tMsg.cmd = AUD_OSS_ADAP_TRANSFER;
            tMsg.dataAddr = (VIRTUAL(_gu4AudUploadDataAddr));
            tMsg.dataSize = _gu4AudUploadDataSize;
            tMsg.CBfunc = NULL;
            _AudOssAdap_SendMsg(tMsg);
        }
        #endif

        _gu4AudUploadMuxerCount ++;
    }
    #if defined (CC_AUD_MP3ENC_FAKEDATA_SUPPORT) && defined (CONFIG_HIGH_RES_TIMERS)
    AUD_DeleteTimer();
    #endif
}

void _AUD_GetUploadInfo(UINT32* u4Pts,UINT32* u4Addr, UINT32* u4Size)
{
#ifdef CC_AUD_4_DECODER_SUPPORT
    if (_gu1AudUploadMode==3)
    {
        UINT32 u4FifoSA,u4FifoEA,u4WP;

        //SBCEnc
        *u4Pts = 0;
        *u4Addr = 0;
        *u4Size = 0;

        u4FifoSA = u4GetSBCEncFIFOStart();
        u4FifoEA = u4GetSBCEncFIFOEnd();
        u4WP = u4GetSBCEncWritePnt();

        if ((u4WP >= u4FifoSA) && (u4WP < u4FifoEA))
        {
            if (_gu4AudReadPtr < u4WP)
            {
                if ((u4WP - _gu4AudReadPtr) > 0x1000)
                {
                    *u4Addr = _gu4AudReadPtr;
                }
            }
            else if (_gu4AudReadPtr > u4WP)
            {
                if (((u4FifoEA - u4FifoSA) - (_gu4AudReadPtr - u4WP)) > 0x1000)
                {
                    *u4Addr = _gu4AudReadPtr;
                }
            }
        }

        if (*u4Addr)
        {
            _gu4AudReadPtr += 0x1000;
            if (_gu4AudReadPtr == u4FifoEA)
                _gu4AudReadPtr = u4FifoSA;
            *u4Size = 0x1000;
        }
    }
    else
#endif
    {
        *u4Pts = _gu4AudUploadDataPts;
        *u4Addr = _gu4AudUploadDataAddr;
        *u4Size = _gu4AudUploadDataSize;
        if (_gu4AudUploadDataPtsPrev == _gu4AudUploadDataPts)
        {
            *u4Size = 0;
        }
        _gu4AudUploadDataPtsPrev = _gu4AudUploadDataPts;
    }
}

void _AUD_GetYWUploadInfo(UINT32* u4base, UINT32* u4Size)
{
    *u4base = _gu4AudUploadDataBaseAdr;
    *u4Size = _gu4AudUploadDataTotalSize;
}

void _AUD_GetUploadPCMInfo(UINT32* uPCMBits, UINT32* uPCMChannels, UINT32* uPCMSampleRate)
{
  if (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eDecState == AUD_DEC_PLAYING)
    {
        *uPCMBits = _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMBits;
        *uPCMChannels = _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMChannels;
        *uPCMSampleRate = _guAudUploadPCMInfo[AUD_DEC_MAIN].uPCMSampleRate;
    }
    else
    {
        *uPCMBits = (UINT32)PCM_INIT_BITS;
        *uPCMChannels = (UINT32)PCM_INIT_CH;
        *uPCMSampleRate =(UINT32) PCM_INIT_SR;
    }
}

#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
static UINT32 _gu4AudBluetoothMuxerCount = 0;
static UINT32 _gu4AudBluetoothQueueIndex = 0xffffffff;
static UINT8 _gu1AudBluetoothDataFlag = 0;

static UINT32 _gu4AudBluetoothDataPts = 0;
static UINT32 _gu4AudBluetoothDataPtsPrev = 0xffffffff;
static UINT32 _gu4AudBluetoothDataAddr = 0;
static UINT32 _gu4AudBluetoothDataSize = 0;

static UINT32 _gu4AudBluetoothDataBaseAdr = 0;
static UINT32 _gu4AudBluetoothDataTotalSize = 0;


//#define DUMP_BLUETOOTH_BUF_TO_USB
//#define DUMP_TEST        /// Only for Test
#ifdef DUMP_TEST
#define OUT_BUF_SIZE_1 2*1024*1024    ///1024*1024
static UINT8 _gu1OutBuf_1[OUT_BUF_SIZE_1];
static UINT32 _gu4OutBufLen_1 = 0;
static UINT32 _u4preindex_1 = 0;
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
//static UINT32 _gu4AudReadPtr_1 = 0;
#endif

#define AUD_BLUETOOTH_DATA_QUEUE_MAX_NUM 128
#define AUD_DEC_BLUETOOTH_NUM  1

typedef struct __audBluetoothQueue {
    UINT32 u4StartAddr; // in bytes
    UINT32 u4Size; // in bytes
} _audBluetoothQueue_t;



typedef struct __audBluetoothPCMInfo {
    UPLOAD_PCM_BITS     uPCMBits;        // in bytes
    UPLOAD_PCM_CHANNEL  uPCMChannels;    // in bytes
    UPLOAD_PCM_FS       uPCMSampleRate;  // in bytes
} _audBluetoothPCMInfo_t;

typedef struct BLUETOOTH_COPY_DATA_TYPE_T
{
    UINT32  channel;    // Encoder index
    UINT64  pts;        // PTS
    UINT32  *pData;     // pointer to Audio Data
    UINT32  dataLen;    // Audio Data Length
    UINT32  *pRStart;   // start pointer of buffer
    UINT32  *pREnd;     // end pointer of buffer
}BLUETOOTH_COPY_DATA_TYPE_T;

static _audBluetoothQueue_t _gu4AudBluetoothQueue[AUD_BLUETOOTH_DATA_QUEUE_MAX_NUM];
static _audBluetoothPCMInfo_t _guAudBluetoothPCMInfo[AUD_DEC_BLUETOOTH_NUM];

void _AUD_DspDataBluetoothNotify (UINT32 u4Index)  /// call by dsp_irq.c
{
#ifdef CC_AUD_BLUETOOTH_DSP_IRQ_QUEUE
    UINT32 u4Msg = 0;
    u4Msg = (UINT32)u4Index;
    UNUSED(_gu1AudBluetoothDataFlag);
    /// Send Command Queue.
    VERIFY((x_msg_q_send(_hAudBluetoothDspIRQCmdQueue, &u4Msg, sizeof(UINT32), 1)) == OSR_OK);
#else
    _gu4AudBluetoothQueueIndex = u4Index; //Daniel Hack??
    if (_gu1AudBluetoothDataFlag == 0x1)
    {
        VERIFY(x_sema_unlock(_hSemaDataBluetoothWait) == OSR_OK);
    }
#endif
}

#if 0   // Unused
UINT32 _AudDataBluetoothGetPts (void)
{
   return  _gu4AudBluetoothDataPts;
}

UINT8 _AUD_DataBluetoothGetMode(void)
{
    return _gu1AudBluetoothMode;
}
#endif

void _AUD_DataBluetoothSetMode(UINT8 u1Mode)
{

    _gu1AudBluetoothMode = u1Mode;

}

void _AUD_BluetoothPCMInfo_Init(void)
{
    /// Set initial value for MW query.
    _guAudBluetoothPCMInfo[AUD_DEC_MAIN].uPCMBits = PCM_16BITS;
    _guAudBluetoothPCMInfo[AUD_DEC_MAIN].uPCMChannels = PCM_2CH;
    _guAudBluetoothPCMInfo[AUD_DEC_MAIN].uPCMSampleRate = PCM_48K;
}

void _AUD_DataBluetooth_Init (void)
{
    _gu1AudBluetoothDataFlag = 0;
    _gu4AudBluetoothMuxerCount = 0;
    _gu4AudBluetoothDataPts = 0;
    _gu4AudBluetoothDataPtsPrev = 0xffffffff;
    _gu4AudBluetoothDataAddr = 0;
    _gu4AudBluetoothDataSize = 0;
    _gu4AudBluetoothQueueIndex = 0xffffffff;

#ifdef DUMP_TEST   /// Only for Test
    _gu4OutBufLen_1 = 0;
    _u4preindex_1 = 0;  
#endif
}

void _AUD_DataBluetooth_start (void)
{
    _gu1AudBluetoothDataFlag = 0x1;
}

void _AUD_DataBluetoothQueueInit (UINT32 u4BaseAddr, UINT32 u4BlockSize, UINT32 u4BlockNum)
{
    UINT32 u4Blk, u4StartAddr;

    VERIFY(u4BlockNum <= uReadShmUINT8(AUD_DSP0, B_BLUETOOTH_BLOCK_NUM));  /// AUD_UPLOAD_DATA_QUEUE_MAX_NUM);

    u4StartAddr = u4BaseAddr;

    _gu4AudBluetoothDataBaseAdr = u4BaseAddr;
    _gu4AudBluetoothDataTotalSize = u4BlockNum * u4BlockSize;

    for (u4Blk = 0; u4Blk < u4BlockNum; u4Blk ++)
    {
        _gu4AudBluetoothQueue[u4Blk].u4StartAddr = u4StartAddr;
        _gu4AudBluetoothQueue[u4Blk].u4Size = u4BlockSize;
        u4StartAddr += u4BlockSize;
    }
}


///#define AUD_UPLOAD_QUEUE_BLOCKSIZE 0x400
///#define AUD_UPLOAD_QUEUE_BLOCKNUM 64
///#define AUD_UPLOAD_BUFFER_BASE (0x39D00 * 4)

static void _AudBluetoothMuxerInit (void)
{
    _gu4AudBluetoothMuxerCount = 0;
    _gu4AudBluetoothQueueIndex = 0xffffffff;

    _AUD_BluetoothPCMInfo_Init();

    _AUD_DataBluetoothQueueInit (_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX] + AUD_BLUETOOTH_BUFFER_BASE,
                                u2ReadShmUINT16(AUD_DSP0, W_BLUETOOTH_BLOCK_SIZE),
                                uReadShmUINT8(AUD_DSP0, B_BLUETOOTH_BLOCK_NUM));

    // enable upload data function
    //_gu1AudUploadDataFlag = 0x1;
    _gu1AudBluetoothDataFlag = 0;

    _gu4AudBluetoothDataPts = 0;

    _AUD_SetPsrStc1 (0);
}

#ifdef DUMP_BLUETOOTH_BUF_TO_USB
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/param.h>

static INT32 _AudDumpAudioData_1 (const INT8 *fileName, const INT8 *buf, size_t count)
{
    mm_segment_t oldfs;
    struct file *filep = NULL;
    size_t ret;

    oldfs = get_fs ();
    set_fs (KERNEL_DS);
    filep = filp_open (fileName, (O_CREAT | O_WRONLY | O_APPEND), 0);
    if ((IS_ERR (filep)) || (NULL == filep) || (NULL == filep->f_op) || (NULL == filep->f_op->write))
    {
        set_fs (oldfs);
        printk (KERN_DEBUG "_AudDumpAudioData1 error!\n");
        return -1;
    }

    ret = filep->f_op->write (filep, buf, count, &filep->f_pos);

    filp_close (filep, 0);
    set_fs(oldfs);

    return 0;
}
#endif

extern UINT32 u4GetBluetoothFIFOStart(void);
extern UINT32 u4GetBluetoothFIFOEnd(void);

static void _AudInformBluetoothMuxer(UINT32 u4Pts, UINT32* u4StartAddr, UINT32 u4Len, UINT32 u4Index)
{
    // send msg to mux
    // picSa, picSz, PTS, picType
    BLUETOOTH_COPY_DATA_TYPE_T rAEncInfo;
    UINT32  u4BluetoothFIFOStart=u4GetBluetoothFIFOStart();
    UINT32  u4BluetoothFIFOEnd=u4GetBluetoothFIFOEnd();
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    AudDecNfyFct   pfAudDecNfy=NULL;
    AudDecNfyFct   pfAudDecNfy1=NULL;
//#endif
#ifdef DUMP_BLUETOOTH_BUF_TO_USB
    size_t ret;
    /*
        FIXME: check USB device path (plug USB disk and find path log)

        Example:
        [FM-Linux]Try to mount /dev/sda1 to /mnt/usb/sda1...
        [FM-Linux]Mount /dev/sda1 to /mnt/usb/sda1 successfully.
                                                  -------------- USB path
    */
    char *fileName = "/mnt/usb/usb1/Drive1/BLUETOOTH.pcm";
#endif

rAEncInfo.channel=2;
rAEncInfo.pts=u4Pts;
rAEncInfo.pData=u4StartAddr;
rAEncInfo.dataLen=u4Len;
rAEncInfo.pRStart= &u4BluetoothFIFOStart;
rAEncInfo.pREnd= &u4BluetoothFIFOEnd;
LOG(5,"bluetooth pcm  pts=%x, pData=%x. dataLen=%x,pRStart=%x,pREnd=%x \n",u4Pts,u4StartAddr,u4Len,u4BluetoothFIFOStart,u4BluetoothFIFOEnd);

#ifdef DUMP_TEST   /// Only for Test
    if ((_gu4OutBufLen_1 + u4Len) <= OUT_BUF_SIZE_1)
    {
        ///Make sure DRAM cache had flushed.
        //DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_COMM);
        DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

        LOG (7, "Data upload to Muxer = (%d) Pts = 0x%X, Start Addr = 0x%X, length = 0x%X, location = 0x%X\n",
            u4Index, u4Pts, VIRTUAL(u4StartAddr), u4Len, VIRTUAL(&_gu1OutBuf_1[_gu4OutBufLen_1]));

        //LOG (7, "0x%x \n", (UINT16 *)VIRTUAL(u4StartAddr));    ///first 2 byte


        x_memcpy ((VOID *)VIRTUAL(&_gu1OutBuf_1[_gu4OutBufLen_1]), (VOID *)VIRTUAL(u4StartAddr), u4Len);

        //LOG (7, "0x%02x%02x   0x%02x%02x\n", _gu1OutBuf_1[_gu4OutBufLen_1], _gu1OutBuf_1[_gu4OutBufLen_1+1], _gu1OutBuf_1[_gu4OutBufLen_1+2], _gu1OutBuf_1[_gu4OutBufLen_1+3]);

        if((_gu1OutBuf_1[_gu4OutBufLen_1]==0)&&(_gu1OutBuf_1[_gu4OutBufLen_1+1]==0)&&(_gu1OutBuf_1[_gu4OutBufLen_1+2]==0)&&(_gu1OutBuf_1[_gu4OutBufLen_1+3]==0))
        {
            LOG (7, "@@@@@@@@@@@@@@@\n");
        }

        _gu4OutBufLen_1 += u4Len;
    }
    else
    {
        LOG(7, "Buffer Address = (0x%x), Length = (0x%x).\n", VIRTUAL(&_gu1OutBuf_1[0]), _gu4OutBufLen_1);
    }
#else
    ///Make sure DRAM cache had flushed.
    //DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_COMM);
    DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    LOG (7, "Data upload to Muxer = (%d) Pts = 0x%X, Start Addr = 0x%X, length = 0x%X\n",
            u4Index, u4Pts, u4StartAddr, u4Len);
#endif

#ifdef DUMP_BLUETOOTH_BUF_TO_USB
    ret = _AudDumpAudioData_1(fileName, (INT8 *)(VIRTUAL(u4StartAddr)), u4Len);
    if (ret < 0)
    {
        LOG (0, "_AudDumpAudioData ERROR!\n");
    }
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    // Get notify function
    //for encmp3 
#ifdef CC_AUD_DDI
    _CB_PutEvent(CB_MTAL_AUDDEC_BLUETOOTH_DATA_ENABLE_TRIGGER, sizeof(BLUETOOTH_COPY_DATA_TYPE_T), &rAEncInfo);//for encmp3
#else    
    UNUSED(rAEncInfo); 
#endif
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy1) == TRUE);

    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, 0, AUD_COND_BLUETOOTH_INFO_CHG, (UINT32)u4StartAddr, u4Len);
    }
    if (pfAudDecNfy1 != NULL)
    {
        pfAudDecNfy1((void *)AUD_NFY_YW, 0, (AUD_COND_NFY_T)AUD_COND_BLUETOOTH_INFO_CHG, (UINT32)u4StartAddr, u4Len);
    }
//#endif

}

static void _AudDataBluetoothSetPts (UINT32 u4Pts)
{
    _gu4AudBluetoothDataPts = u4Pts;
}

static void _AudDataBluetoothThread(void* pvArg)
{
    //UINT32 *u4QueueAddr;
    UINT32 u4UploadAddr, u4UploadSize;
    UINT32 u4Time;
#ifdef CC_AUD_BLUETOOTH_DSP_IRQ_QUEUE
    UINT32 u4Event;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(UINT32);
#endif

    #ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
    AUD_OSS_ADAP_MSG_T tMsg;
    AUD_OSS_ADAP_DEV_T devId;
    AUD_OSS_ADAP_ST_TYPE_T status;
    AUD_OSS_ADAP_SRC_TYPE_T source;
    #endif
    while (1)
    {
#ifdef CC_AUD_BLUETOOTH_DSP_IRQ_QUEUE
        /// Receive Command Queue.
        VERIFY(x_msg_q_receive(&u2MsgIdx, &u4Event, &zMsgSize,
            &_hAudBluetoothDspIRQCmdQueue, 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
        _gu4AudBluetoothQueueIndex = (UINT32)u4Event;
#else
        VERIFY(x_sema_lock(_hSemaDataBluetoothWait, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif

        u4UploadAddr = _gu4AudBluetoothQueue[_gu4AudBluetoothQueueIndex].u4StartAddr;
        u4UploadSize = (UINT32)(u2ReadShmUINT16(AUD_DSP0, W_BLUETOOTH_BLOCK_SIZE) * uReadShmUINT8(AUD_DSP0, B_BLUETOOTH_BLK_INT_CNT));

        u4Time = _AUD_ReadPsrStc1();
        _AudDataBluetoothSetPts(u4Time);
        _gu4AudBluetoothDataAddr = u4UploadAddr;
        _gu4AudBluetoothDataSize = u4UploadSize;

        _AudInformBluetoothMuxer(u4Time, (UINT32 *)u4UploadAddr, u4UploadSize, _gu4AudBluetoothQueueIndex);
#ifdef DUMP_TEST
        if((_gu4AudBluetoothQueueIndex - _u4preindex_1) != 1)
        {
            LOG(3,"_gu4AudBluetoothQueueIndex=%d, _u4preindex_1=%d \n", _gu4AudBluetoothQueueIndex, _u4preindex_1);
        }
        _u4preindex_1 = _gu4AudBluetoothQueueIndex;
#endif

        #ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
        devId = _AudOssAdap_GetDevId();
        status = _AudOssAdap_GetStatus();
        source = _AudOssAdap_GetSourceType();
        if((AUD_OSS_ADAP_ST_WORKING == status) && (AUD_OSS_ADAP_UPLOAD == source))
        {

            tMsg.devId = devId;
            tMsg.cmd = AUD_OSS_ADAP_TRANSFER;
            tMsg.dataAddr = (VIRTUAL(_gu4AudBluetoothDataAddr));
            tMsg.dataSize = _gu4AudBluetoothDataSize;

            _AudOssAdap_SendMsg(tMsg);
        }
        #endif

        _gu4AudBluetoothMuxerCount ++;
    }
}

void _AUD_GetBluetoothInfo(UINT32* u4Pts,UINT32* u4Addr, UINT32* u4Size)
{
    {
        *u4Pts = _gu4AudBluetoothDataPts;
        *u4Addr = _gu4AudBluetoothDataAddr;
        *u4Size = _gu4AudBluetoothDataSize;
        if (_gu4AudBluetoothDataPtsPrev == _gu4AudBluetoothDataPts)
        {
            *u4Size = 0;
        }
        _gu4AudBluetoothDataPtsPrev = _gu4AudBluetoothDataPts;
    }
}

void _AUD_GetYWBluetoothInfo(UINT32* u4base, UINT32* u4Size)
{
    *u4base = _gu4AudBluetoothDataBaseAdr;
    *u4Size = _gu4AudBluetoothDataTotalSize;
}

void _AUD_GetBluetoothPCMInfo(UINT32* uPCMBits, UINT32* uPCMChannels, UINT32* uPCMSampleRate)
{
  if (_arAudDecoder[AUD_DSP0][AUD_DEC_MAIN].eDecState == AUD_DEC_PLAYING)
    {
        *uPCMBits = _guAudBluetoothPCMInfo[AUD_DEC_MAIN].uPCMBits;
        *uPCMChannels = _guAudBluetoothPCMInfo[AUD_DEC_MAIN].uPCMChannels;
        *uPCMSampleRate = _guAudBluetoothPCMInfo[AUD_DEC_MAIN].uPCMSampleRate;
    }
    else
    {
        *uPCMBits = (UINT32)PCM_INIT_BITS;
        *uPCMChannels = (UINT32)PCM_INIT_CH;
        *uPCMSampleRate =(UINT32) PCM_INIT_SR;
    }
}
#endif


#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER

#define TRANSFER_BUF_SIZE (128*1024)
static AUD_DATA_TRANSFER_T * _gStrAudTrans = NULL;

void _AudDataTransferInit(void)
{
    if(NULL == _gStrAudTrans)
    {
        _gStrAudTrans = (AUD_DATA_TRANSFER_T*)x_mem_alloc(sizeof(AUD_DATA_TRANSFER_T));
        VERIFY(NULL != _gStrAudTrans);
        memset((void *)_gStrAudTrans, 0, sizeof(AUD_DATA_TRANSFER_T));

        _gStrAudTrans->u1InitFlag = 1;
        _gStrAudTrans->pu1AudTransBuf = (UINT8*)x_mem_alloc(TRANSFER_BUF_SIZE);
        VERIFY(NULL != _gStrAudTrans->pu1AudTransBuf);

        _gStrAudTrans->u4AudTransWritePtr = (UINT32)_gStrAudTrans->pu1AudTransBuf;
        _gStrAudTrans->u4AudTransReadPtr = (UINT32)_gStrAudTrans->pu1AudTransBuf;
        _gStrAudTrans->u4AudTransReadPtr1 = (UINT32)_gStrAudTrans->pu1AudTransBuf;

        VERIFY(x_sema_create(&_gStrAudTrans->hSemaAudTransLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    }
}

void _AudDataTransferUninit(void)
{
    if(NULL != _gStrAudTrans)
    {
        if(NULL != _gStrAudTrans->pu1AudTransBuf)
        {
            x_mem_free(_gStrAudTrans->pu1AudTransBuf);
        }
        x_mem_free(_gStrAudTrans);
    }

    _gStrAudTrans = NULL;
}


UINT32 _AudDataTransferGetStartAddr(void)
{
    return (UINT32)_gStrAudTrans->pu1AudTransBuf;
}

UINT32 _AudDataTransferGetEndAddr(void)
{
    return ((UINT32)_gStrAudTrans->pu1AudTransBuf + TRANSFER_BUF_SIZE);
}

UINT32 _AudDataTransferGetSize(void)
{
    return (TRANSFER_BUF_SIZE);
}

void _AudDataTransferSetWritePtr (UINT32 u4WP)
{
    if(u4WP >= ((UINT32)_gStrAudTrans->pu1AudTransBuf + TRANSFER_BUF_SIZE))
    {
        _gStrAudTrans->u4AudTransWritePtr = u4WP - TRANSFER_BUF_SIZE;
    }
    else
    {
        _gStrAudTrans->u4AudTransWritePtr = u4WP;
    }
}

UINT32 _AudDataTransferGetReadPtr (void)
{
    return _gStrAudTrans->u4AudTransReadPtr1;
}

UINT32 _AudDataTransferSetReadPtr (UINT32 u4RP)
{
    if(u4RP >= ((UINT32)_gStrAudTrans->pu1AudTransBuf + TRANSFER_BUF_SIZE))
    {
        _gStrAudTrans->u4AudTransReadPtr1 = u4RP - TRANSFER_BUF_SIZE;
    }
    else
    {
        _gStrAudTrans->u4AudTransReadPtr1 = u4RP;
    }
    return 0;
}


UINT32 _AudDataTransferGetFreeSize (void)
{
    UINT32 u4Free;

    if(_gStrAudTrans->u4AudTransWritePtr >= _gStrAudTrans->u4AudTransReadPtr)
    {
        u4Free = (TRANSFER_BUF_SIZE) - (_gStrAudTrans->u4AudTransWritePtr - _gStrAudTrans->u4AudTransReadPtr);
    }
    else
    {
        u4Free = _gStrAudTrans->u4AudTransReadPtr - _gStrAudTrans->u4AudTransWritePtr;
    }

    return u4Free;
}

UINT32 _AudDataTransferGetDataSize (void)
{
    UINT32 u4Avail;

    if(_gStrAudTrans->u4AudTransWritePtr >= _gStrAudTrans->u4AudTransReadPtr1)
    {
        u4Avail = (_gStrAudTrans->u4AudTransWritePtr - _gStrAudTrans->u4AudTransReadPtr1);
    }
    else
    {
        u4Avail = ((UINT32)_gStrAudTrans->pu1AudTransBuf + TRANSFER_BUF_SIZE) - _gStrAudTrans->u4AudTransReadPtr1;
    }

    if(u4Avail >= 0x1000)
    {
        u4Avail = 0x1000;
    }

    return u4Avail;
}

void _AudDataTransferCB(UINT32 u4ReadPtr, UINT32 u4DataSize)
{
    if((u4ReadPtr + u4DataSize) >= ((UINT32)_gStrAudTrans->pu1AudTransBuf + TRANSFER_BUF_SIZE))
    {
        _gStrAudTrans->u4AudTransReadPtr = u4ReadPtr + u4DataSize - (TRANSFER_BUF_SIZE);
    }
    else
    {
        _gStrAudTrans->u4AudTransReadPtr = u4ReadPtr + u4DataSize;
    }
}


void _AudDataTransferUnlock (void)
{
    VERIFY(x_sema_unlock(_gStrAudTrans->hSemaAudTransLock) == OSR_OK);
}

void _AudDataTransferLock(void)
{
    VERIFY(x_sema_lock(_gStrAudTrans->hSemaAudTransLock, X_SEMA_OPTION_WAIT) == OSR_OK);
}

static void _AudDataTransferThread(void* pvArg)
{
    AUD_OSS_ADAP_MSG_T tMsg;
    AUD_OSS_ADAP_DEV_T devId;
    AUD_OSS_ADAP_ST_TYPE_T status;

    UINT32 u4ReadAddr;
    UINT32 u4DataSize;

    _AudDataTransferInit();

    while (1)
    {
        u4ReadAddr = _AudDataTransferGetReadPtr();
        do{
            _AudDataTransferLock();
            u4DataSize = _AudDataTransferGetDataSize();
        }while(0 == u4DataSize);

        devId = _AudOssAdap_GetDevId();
        status = _AudOssAdap_GetStatus();
        if(AUD_OSS_ADAP_ST_WORKING == status)
        {
            tMsg.devId = devId;
            tMsg.cmd = AUD_OSS_ADAP_TRANSFER;
            tMsg.dataAddr = u4ReadAddr;
            tMsg.dataSize = u4DataSize;
            tMsg.CBfunc = _AudDataTransferCB;

            _AudOssAdap_SendMsg(tMsg);
            _AudDataTransferSetReadPtr(u4ReadAddr + u4DataSize);
        }
    }

    _AudDataTransferUninit();
}

#endif


void AUD_DrvCheckSpeedTime(UINT8 u1DecId)
{
    static UINT16 _u2Cnt = 0;
    UINT16 u2Speed = 0;
    UINT32 u4ReadPtr;
    UINT32 u4WritePtr;
    UINT32 u4PTS;
    UINT64 u8PTS;
    UINT32 u4DataBitrate;
    UINT32 u4Offset = 0;
    UINT32 u4Stc;
    UINT8  u1StcID = STC_SRC_A1;

    UNUSED(AUD_DrvCheckSpeedTime);
    UNUSED(u4WritePtr);
    UNUSED(u4DataBitrate);
    UNUSED(u4Stc);
    UNUSED(u1StcID);
    AUD_DEC_ID_VALIDATE(u1DecId);

    u1StcID = AUD_DspQryStcID(u1DecId);
    u4DataBitrate = DSP_GetDataRate(u1DecId);

    // Only pure audio file do FF.
    // Now enable this for all bitrate
    if (0)//(u4DataBitrate > 64000)   //only handle low bitrate file.
    {
        return;
    }

    _AUD_DMX_GetAudPtr(u1DecId, &u4ReadPtr, &u4WritePtr);
    _AUD_DspGetSpeed(u1DecId, &u2Speed);
    if (u2Speed > 0)            //only for Fast Forward
    {
        if (_u2Cnt < (240/AUD_PTR_UPDATE_TIMEOUT))  //for every X ms update once.
        {
            _u2Cnt++;
            return;
        }
        _u2Cnt = 0;
        AUD_MMFindSyncInfo(u1DecId,&u8PTS,u4ReadPtr);
        u4Stc = STC_GetStcValue(u1StcID);

        //LOG(6, "u4ReadPtr =0x%x, u8PTS = 0x%llx, STC=0x%x\n", u4ReadPtr, u8PTS, u4Stc);

        #if 0
        //discard invalid pts when change FR to FF--zhengwen@mtk94060
        if(u8PTS == 0x7fffffffffffffff)
        {
            return;
        }
        #endif

        u4PTS = (UINT32)u8PTS;
        if(u4PTS == 0)
        {
            return;
        }

        #if 0
        if (u4DataBitrate <= 12000)
        {
            u4Offset = 93000;   // 1.03 s
        }
        else if ((u4DataBitrate > 12000) && (u4DataBitrate <= 32000))
        {
            u4Offset = 50000;   // 0.55 s
        }
        else if (u4DataBitrate > 32000)
        {
            u4Offset = 15000;   // 0.16 s
        }
        #endif
        u4Offset = 2880;             // for aout is slower than Rp for about 6 256banks in 48KHZ
        if (u4PTS >= u4Offset)
        {
            u4PTS -= u4Offset;
        }

        if (u4PTS < STC_GetStcValue(u1StcID))
        {
            return;
        }

        STC_StopStc(u1StcID);
        STC_SetStcValue(u1StcID, u4PTS);
        //LOG(6, "update STC = 0x%x\n",u4PTS );
        return;
    }
    else
    {
        _u2Cnt = 0;
    }
}

#if defined(CC_SUPPORT_STR) || defined(CC_FAST_INIT) || defined(__MODEL_slt__)
//#if 1
//-----------------------------------------------------------------------------
/** AUD_DrvThreadWakeup
 *  Driver thread wakeup
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DrvThreadWakeup(UINT8 u1DspId, UINT8 u1DecId)
{
    if (u1DecId == AUD_DEC_MAIN)
    {
        vADSPPowerON(u1DspId);
    }

    while (fgADSPIsIdle(u1DspId))
    {
        x_thread_delay(1);
    }

    _ChangeAudioState(u1DspId, u1DecId, AUD_IDLE);

    _arAudDecoder[u1DspId][u1DecId].eDecState = AUD_DEC_STOP;
    _afgIssuePlayComToDsp[u1DspId][u1DecId] = FALSE;

    // Set event for IDLE state
    _AudSetEvent(u1DspId, u1DecId, AUD_CMD_FLAG_PLAY   |
        AUD_CMD_FLAG_STOP   |
        AUD_CMD_FLAG_RESET  |
        AUD_CMD_FLAG_RESUME |
        AUD_CMD_FLAG_PAUSE |
        AUD_CMD_FLAG_LOADCODE);
}

//-----------------------------------------------------------------------------
/** AUD_DrvSuspend
 *  Driver suspend
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DrvSuspend(void)
{
    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DSP0][AUD_DEC_MAIN]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DSP0][AUD_DEC_MAIN]) == OSR_OK);

    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DSP0][AUD_DEC_AUX]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DSP0][AUD_DEC_AUX]) == OSR_OK);

#ifdef CC_MT5391_AUD_3_DECODER
    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DSP0][AUD_DEC_THIRD]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DSP0][AUD_DEC_THIRD]) == OSR_OK);
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DSP0][AUD_DEC_4TH]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DSP0][AUD_DEC_4TH]) == OSR_OK);
#endif
#ifndef CC_AUD_DISABLE_2ND_DSP
    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DSP1][AUD_DEC_MAIN]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DSP1][AUD_DEC_MAIN]) == OSR_OK);

    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DSP1][AUD_DEC_AUX]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DSP1][AUD_DEC_AUX]) == OSR_OK);

#ifdef CC_MT5391_AUD_3_DECODER
    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DSP1][AUD_DEC_THIRD]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DSP1][AUD_DEC_THIRD]) == OSR_OK);
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
    VERIFY(x_sema_unlock(_arAudDrvStateSema[AUD_DSP1][AUD_DEC_4TH]) == OSR_OK);
    VERIFY(x_sema_unlock(_ahSemaUopComDone[AUD_DSP1][AUD_DEC_4TH]) == OSR_OK);
#endif
#endif
}
#endif

void AUD_DrvLogStcDiff(UINT8 u1DecId)
{

    INT32 i4StcLog = 0;
    UINT32 u4Temp = 0;

    INT32 i4VdpStcDiff = 0;
    UINT32 u4Tmp1;
    static UINT32 u4PrevEng = 0;
    static UINT16 u2AdjCounterDrop= 0;
    static UINT16 u2AdjCounterRpt = 0;
    VDO_INPUT_INFO rVdoInfo;
    BOOL fgDrop, fgRepeat;


    UNUSED(AUD_DrvLogStcDiff);
    UNUSED(i4StcLog);
    UNUSED(u4Temp);

    UNUSED(u4Tmp1);           //DTV_AVSYNC_ENH
    UNUSED(u4PrevEng);
    UNUSED(u2AdjCounterDrop);
    UNUSED(u2AdjCounterRpt);
    UNUSED(i4VdpStcDiff);
    UNUSED(i4AudioBound);
    UNUSED(i4VideoBound);
    UNUSED(i4AudioMinusBound);
    UNUSED(i4VideoMinusBound);
    UNUSED(i4HalfBound);
    UNUSED(rVdoInfo);
    UNUSED(fgDrop);
    UNUSED(fgRepeat);        //=============

    u4Temp = u4ReadD2RInfo_StcDiff(AUD_DSP0, u1DecId);

#if 0 //debug energy
    u4Tmp1 = dReadDspCmptDram(3, 0x6f408);
    if ((u4Tmp1 > 0) && (u4PrevEng==0))
    {
        LOG(6,"\t \t !@@@@!!!!! energy  = 0x%x, %d\n", u4Tmp1, u4AudGetDecErrorCnt(1));
    }
    u4PrevEng = u4Tmp1;
#endif

    if (u4Temp != 0)
    {
        if (u4Temp & 0x800000)
        {
            // (-) negative
            i4StcLog = (INT32)(u4Temp | 0xFF000000);
        }
        else
        {
            // (+) positive
            i4StcLog = (INT32)u4Temp;
        }
        if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_STC_DIFF) == AUD_DBG_SYNC_STC_DIFF)
        {
            LOG(5,"Dec(%d) Stc_diff = %d, V=%d \n", u1DecId, i4StcLog, VDP_GetPtsStcDelta(0));
        }

#ifdef CC_AUD_DDI  //Enable this to turn on the function !! //DTV_AVSYNC_ENH
//====================================================================================
        // Set up Variables
        DrvVideoGetInputTimingInfo(SV_VP_MAIN, &rVdoInfo);
        if (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_AAC)
        {
            i4AudioBound = AUDIO_BOUND_AAC;
        }
        else
        {
            i4AudioBound = AUDIO_BOUND;
        }

        if ((rVdoInfo.u1RefreshRate == 50)&&(rVdoInfo.u1Interlace==1))
        {
            i4VideoBound = VIDEO_BOUND_50_I;
        }
        else if ((rVdoInfo.u1RefreshRate == 60)&&(rVdoInfo.u1Interlace==1))
        {
            i4VideoBound = VIDEO_BOUND_60_I;
        }
        else
        {
            i4VideoBound = VIDEO_BOUND_60_P;
        }
        i4AudioMinusBound = (i4AudioBound*-1);
        i4VideoMinusBound = (i4VideoBound*-1);
        i4HalfBound = ((UINT32)(i4VideoBound))/2;

        // Do sync !!!!!!!
        i4VdpStcDiff = VDP_GetPtsStcDelta(0);  //B2R ID is 0

        fgDrop = FALSE;
        fgRepeat = FALSE;
        // audio < 0 case
        if ((i4StcLog < 0) && (i4StcLog > i4AudioMinusBound))
        {
            //video is 0 ~ 3000
            if (((i4VdpStcDiff > 0) && (i4VdpStcDiff < i4VideoBound)) &&
                ((i4VdpStcDiff-i4StcLog) > i4HalfBound))
            {
                fgDrop = TRUE;
            }
            //video is -3000 ~ 0
            else if (((i4VdpStcDiff < 0) && (i4VdpStcDiff > i4VideoMinusBound)) &&
                     ((i4StcLog-i4VdpStcDiff) > i4HalfBound))
            {
                fgRepeat = TRUE;
            }
        }
        // audio > 0 case
        else if ((i4StcLog > 0) && (i4StcLog < i4AudioBound))
        {
            //video is 0 ~ 3000
            if (((i4VdpStcDiff > 0) && (i4VdpStcDiff < i4VideoBound)) &&
                ((i4VdpStcDiff-i4StcLog) > i4HalfBound))
            {
                fgDrop = TRUE;
            }
            //video is -3000 ~ 0
            else if (((i4VdpStcDiff < 0) && (i4VdpStcDiff > i4VideoMinusBound)) &&
                     ((i4StcLog-i4VdpStcDiff) > i4HalfBound))
            {
                fgRepeat = TRUE;
            }
        }
#if 0  //not consider video stable, adjust too slow
        if ((fgDrop||fgRepeat) && (_VDP_GetVideoSyncStable(0)==FALSE))
        {
            LOG(5, "Video is not Stable. Give up drop/repeat !  A %d, V %d\n", i4StcLog, i4VdpStcDiff);
            fgDrop = FALSE;
            fgRepeat = FALSE;
        }
#endif
        if (fgDrop)
        {
            if (u2AdjCounterDrop == AV_ADJUST_BOUND) //Check the same condition for 3 time. (Prevent toogle)
            {
                if (_u2AudConrolVdpQuota < AUD_ADJUST_QUOTA) //Only taks action when having Quota.
                {
                    VDP_SetDropFrame(0);
                    LOG(5, "Please Video Drop!!!!!!! A %d, V %d\n", i4StcLog, i4VdpStcDiff);
                            _u2AudConrolVdpQuota++;
                }
                u2AdjCounterDrop = 0;
            }
            else
            {
                u2AdjCounterDrop ++;
                LOG(7, "Consider Drop, A %d, V %d\n", i4StcLog, i4VdpStcDiff);
            }
            u2AdjCounterRpt = 0;
        }
        else if (fgRepeat)
        {
            if (u2AdjCounterRpt == AV_ADJUST_BOUND)
            {
                if (_u2AudConrolVdpQuota < AUD_ADJUST_QUOTA) //Only taks action when having Quota.
                {
                    VDP_SetRepeatFrame(0);
                    LOG(5, "Please Video Repeat!!!!!! A %d, V %d\n", i4StcLog, i4VdpStcDiff);
                    _u2AudConrolVdpQuota++;
                }
                u2AdjCounterRpt =0;
            }
            else
            {
                u2AdjCounterRpt ++;
                LOG(7, "Consider Repeat, A %d, V %d\n", i4StcLog, i4VdpStcDiff);
            }
            u2AdjCounterDrop = 0;
        }
        else
        {
            u2AdjCounterDrop = 0;
            u2AdjCounterRpt = 0;
        }
//==================================================================================
#endif //DTV_AVSYNC_ENH

   } //if (u4Temp != 0)

}
BOOL AUD_IsEsExist(void)
{
    if(!isEsExist && !isEsExist1)
       return FALSE;
    else
       return TRUE;
}

#ifdef CC_AUD_DDI
void _AUD_CheckPvrPause(INT32 i4Speed)
{
    UNUSED(_fgPvrPause);
    if(i4Speed != 100)
    {
        if(i4Speed == 1)
        {
            _fgPvrPause = TRUE;
        }
        else
        {
            _fgPvrPause = FALSE;
        }
    }
}
void _AUD_SetDelayMode(BOOL fgEnable)   
{
    vWriteShmUINT8 (AUD_DSP0, B_DELAY_MODE, fgEnable); 
}
#endif

#if 0   // Unused
void AUD_LogVdoInfo(void)   //CC_AUD_QUERY_VDO_INFO
{
#if 1//(defined(CC_MT5396))
    VDO_INPUT_INFO rVdoInfo;

    UNUSED(AUD_LogVdoInfo);
    UNUSED(rVdoInfo);

    x_memset((VOID*)VIRTUAL((UINT32)&rVdoInfo), 0, sizeof(rVdoInfo));
    DrvVideoGetInputTimingInfo(SV_VP_MAIN, &rVdoInfo);
    LOG(3,"## VDO Info ## \n H=%d, W=%d, T=%d, I=%d, R=%d, PanelDelay=%d\n",
        rVdoInfo.u2Height, rVdoInfo.u2Width, rVdoInfo.u1Timing,
        rVdoInfo.u1Interlace,rVdoInfo.u1RefreshRate, rVdoInfo.u1PanelDelay);

    LOG(3," i4AudioBound=%d, i4VideoBound=%d, i4AudioMinusBound=%d, i4VideoMinusBound=%d, u4HalfBound=%d\n",
        i4AudioBound, i4VideoBound, i4AudioMinusBound, i4VideoMinusBound, i4HalfBound);
#else
    UNUSED(AUD_LogVdoInfo);
    LOG(3,"## VDO Info ## -- Query not support in this IC.....\n");
#endif
}
#endif

void AUD_SetAudControlVdpQuota(UINT8 u1DecID, UINT16 u2Quota)  //DTV_AVSYNC_ENH
{
    if ((u1DecID == AUD_DEC_MAIN)||(u1DecID == AUD_DEC_AUX))
    {
        _u2AudConrolVdpQuota = u2Quota;
    }
}

#ifdef AUD_DECODER3_SELF_TEST
void AudSetDualPlay(UINT8 u1DecId,  AV_SYNC_MODE_T eSyncMode) //parson MM3
{
    u1ForceDecId = u1DecId;
    eForceSyncMode = eSyncMode;
}
#endif

static BOOL _IsPtsAlign(UINT8 u1DecId, UINT32 u4Ptr)
{
    UINT16 u2Word;
    BOOL fgRet = TRUE;

    //for MS11 non align PES test
    if (_arAudDecoder[AUD_DSP0][u1DecId].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        u2Word = *(UINT16*)VIRTUAL(DSP_PHYSICAL_ADDR(u4Ptr));
        if (_arAudDecoder[AUD_DSP0][u1DecId].eDecFormat == AUD_FMT_AAC)
        {
            UINT16 u2SyncLOAS, u2SyncADTS;
            //check if LOAS
            u2SyncLOAS = (((u2Word&0xff)<<8)|((u2Word&0xff00)>>8)) >> 5;   //swap bytes and get 11 bits
            //check if ADTS
            u2SyncADTS = (((u2Word&0xff)<<8)|((u2Word&0xff00)>>8)) >> 4;   //swap bytes and get 12 bits
            if ((u2SyncLOAS != 0x2B7) && (u2SyncADTS !=0xFFF))
            {
                fgRet =  FALSE;
                if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_SEND_PTS) == AUD_DBG_SYNC_SEND_PTS)
                {
                    LOG(1,"PTS not align!!LOAS = %x, ADTS = %x\n",u2SyncLOAS,u2SyncADTS);
                }
            }
        }
        //Add new cases here
    }
    return (fgRet);
}

#if 0 //For IC/FPGA Verification //IC_FPGA_STV_VERIF
/*
   1. Enable code in aud_drv.c (here)
   2. Enable code in aud_cmd.c
   3. Put code to dmx driver.
   4. Prepare PCM decoder.
   5. Use memplay.
*/
extern UINT32 STC_Read_DMX_STC1_22c(void);
extern UINT32 STC_Read_DMX_STC1_23c(void);
extern UINT32 STC_Read_DMX_STC2_27c(void);
extern UINT32 STC_Read_DMX_STC2_28c(void);
extern void STC_Write_DMX_STC1(UINT32 u4Value);
extern void STC_Write_DMX_STC2(UINT32 u4Value);
extern void STC_Enable_AUD_Update(UINT8 u1Id);
void AUD_DrvTestSTC(UINT8 u1StcId, UINT32 u4Value, BOOL fgUpdate) //parson STC
{
    STC_StartStc(STC_SRC_A1);

    if (fgUpdate)
    {
        switch (u1StcId)
        {
        case 0: //Parser
            _AUD_SetPsrStc1 (u4Value);
            PARSER_WRITE32(REG_A_STC_1, u4Value);
            LOG(5, "Update Parser STC to 0x%x\n", u4Value);
            break;
        case 1: //DMX STC1
            STC_Write_DMX_STC1(u4Value);
            STC_Enable_AUD_Update(0);
            LOG(5, "Update DMX STC 1 to 0x%x\n", u4Value);
            break;
        case 2: //DMX STC2
            STC_Write_DMX_STC2(u4Value);
            STC_Enable_AUD_Update(1);
            LOG(5, "Update DMX STC 2 to 0x%x\n", u4Value);
            break;
        };
    }

    LOG(5, "PsrSTC is 0x%x, 0x%x\n", _AUD_ReadPsrStc1(), PARSER_READ32(REG_A_STC_1));
    LOG(5, "DmxSTC1 is 0x%x, 0x%x\n", STC_Read_DMX_STC1_22c(),STC_Read_DMX_STC1_23c());
    LOG(5, "DmxSTC2 is 0x%x, 0x%x\n", STC_Read_DMX_STC2_27c(),STC_Read_DMX_STC2_28c());
}

///////// move to the end of dmx_pcr.c /////////
UINT32 STC_Read_DMX_STC1_22c(void) //parson STC
{
    return DMXCMD_REG32(139);  // 139 * 4 =0x22c
}
UINT32 STC_Read_DMX_STC1_23c(void)
{
    return DMXCMD_REG32(143);  // 143 * 4 =0x23c
}
UINT32 STC_Read_DMX_STC2_27c(void)
{
    return DMXCMD_REG32(159);  // 159 * 4 =0x27c
}
UINT32 STC_Read_DMX_STC2_28c(void)
{
    return DMXCMD_REG32(163);  // 163 * 4 =0x28c
}

void STC_Write_DMX_STC1(UINT32 u4Value)
{
    DMXCMD_WRITE32(143, u4Value); // 143 * 4 =0x23c
}
void STC_Write_DMX_STC2(UINT32 u4Value)
{
    DMXCMD_WRITE32(163, u4Value); // 163 * 4 =0x28c
}
void STC_Enable_AUD_Update(UINT8 u1Id)
{
    UINT32 u4Value = 0;
    u4Value = DMXCMD_READ32(129);
    u4Value = u4Value & 0xFFFFFFFC;

    if (u1Id == 0)
        DMXCMD_WRITE32(129, ((u4Value)|2) ); // 129 * 4 =0x204
    else
        DMXCMD_WRITE32(129, ((u4Value)|3) ); // 129 * 4 =0x204
}
/////////////////////////////////////////////////////

#endif


//==============================================================================
//
// PTS Queue Related    //PTSQueue_V2
//
// 1. Put Wp/PES_info (e.x. PTS) into the queue
// 2. Queue is sequential (Wp from small to big)
//    |1,2,3,4,5,6...20, 0,0,0,0,0,0,0|  =>  |1',2,3,4,5,.......,20, 0,0,0,0,0,0..|
// 3. If PES_Size is too small and the Quese can't put all the PES, some of PES info is skipped.
//    |1,3,5,7,9,.........|
//
//==============================================================================
void AUD_DrvPtsQueueInit(void) //parson pts
{
    UINT32 u4FifoSA;
    UINT32 u4FifoEA;
    UINT8 u1DecID;

    for (u1DecID=AUD_DEC_MAIN; u1DecID<=AUD_DEC_THIRD; u1DecID++)
    {
        // 1. Set address of Queue, it's in the normal compact dram, it's in size of Bytes.
        _arPesQueue[u1DecID]=_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX] + (DRAM_PES_INFO_QUEUE) + ((PES_INFO_Q_ITEMS*4)*PES_INFO_Q_LEN)*u1DecID;

        // 2. Calculate Unit of Queue.
        //     If the PES size is too small (too many PES in FIFO), we can't record all of them in the Queue.
        u4FifoSA = u4GetAFIFOStart(AUD_DSP0, u1DecID);
        u4FifoEA = u4GetAFIFOEnd(AUD_DSP0, u1DecID);
        u4_Q_Uinit[u1DecID] = ((u4FifoEA-u4FifoSA)/PES_INFO_Q_LEN)*2; //new_arm_pts_table
        LOG(3, "AUD_DrvPtsQueueInit, Addr=0x%x, u4_Q_Uinit=%d\n",_arPesQueue[u1DecID], u4_Q_Uinit[u1DecID]);
    }
}
#if 0 //new_arm_pts_table
void AUD_DrvPtsQueueInsert(UINT8 u1DecID, UINT32 u4Wp, UINT32 u4PTS)
{
    UINT16 u2Size;
    UINT32 u4ReadPtr, u4WritePtr, u4FifoSz, u4DataSz;
    UINT32 u4FifoSA, u4FifoEA;
    UINT16 i,u2TmpSize;
    UINT32 u4NextIdx;
    UINT32 u4TmpWp;

    if (u1DecID > AUD_DEC_THIRD)
    {
        return;
    }

    ////// DEBUG //////
    //LOG(4, "Insert Wp = 0x%x, PTS = 0x%x\n", u4Wp, u4PTS);

    // ============ 1. Check if Wp may overwrite PTS not used by Rp. =================
    //  1.1 Find the next index we will insert into.
    //      If the next Wp < PrevWp, means it's going to wrap around.
    //      If there is delay in driver, we should check Wp in delay buffer.
    if (u2QDelayNum[u1DecID]>0)
    {
        u4TmpWp = aPTSDrvBuf[u1DecID][u2QBufOut[u1DecID]].u4Wp;     // Use Wp from delay buffer.
    }
    else
    {
        u4TmpWp = u4Wp;                                             // Use input Wp
    }
    if ((u4TmpWp<u4_Prev_Wp[u1DecID]) && (u4_Prev_Wp[u1DecID]!=0xffffffff))
    {
        u4NextIdx = 1;                                              // Wrap around, so target Idx is 0 and Next Idx is 1
    }
    else
    {
        u4NextIdx = u2PesQIdx[u1DecID]+1;                           // Normal case, Next Idx = Current Idx + 1;
        if (u4NextIdx >= PES_INFO_Q_LEN)
        {
            u4NextIdx = 0;
        }
    }
    //  1.2 Check if Rp not exceed Wp in next index+1.
    _AUD_DMX_GetAudPtr(u1DecID, &u4ReadPtr, &u4WritePtr);
    u4ReadPtr = DSP_INTERNAL_ADDR(u4ReadPtr);
    u4WritePtr = DSP_INTERNAL_ADDR(u4WritePtr);
    u4FifoSA = u4GetAFIFOStart(AUD_DSP0, u1DecID);
    u4FifoEA = u4GetAFIFOEnd(AUD_DSP0, u1DecID);
    u4FifoSz = u4FifoEA-u4FifoSA;
    if (u4WritePtr >= u4ReadPtr)
    {
        u4DataSz = u4WritePtr-u4ReadPtr;
    }
    else
    {
        u4DataSz = (u4FifoEA-u4ReadPtr)+(u4WritePtr-u4FifoSA);
    }
    /*  Check if Wp is very close to Rp ....
        1,2,3,4....25, Wp should wait Rp exceed 2, then 1' can be put into Q.  1',2,(Rp),3,4....
        Because if Rp is between 1 and 2, it may refresh and use PTS in 1.
    */
    ////// DEBUG //////
    //LOG(4,"#R=0x%x, NextId=%d,W=0x%x, u4TmpWp=0x%x,PrevWp=0x%x\n",u4ReadPtr, u4NextIdx,PEQ_Q[u4NextIdx].u4Wp, u4TmpWp,u4_Prev_Wp[u1DecID]);
    u4TmpWp = PEQ_Q[u4NextIdx].u4Wp;
    //  Check if Rp not exceed Wp, and Wp should not be 0 (Not used item in queue).
    //  If (Wp in Q)-Rp is too large, it means Rp may already wrap around. |R ...... W|,
    //     We should not wait Rp exceed Wp.
    //  If DataSz is not large, it means Wp is not following Rp, we don't worry overwrite problem.
    if ((u4ReadPtr<=u4TmpWp) && (u4TmpWp!=0)
        && ((u4TmpWp-u4ReadPtr)<(u4FifoSz/2))
        && (u4DataSz>(u4FifoSz/2)) )
    {
        // In this condition, 1,(Rp),2,3...   Rp<=2, (2-Rp) is not too large, and (datasize in FIFO) > (FIFO size/2)
        // Means Wp is very close to Rp
        //Put data into driver delay queue.
        aPTSDrvBuf[u1DecID][u2QBufIn[u1DecID]].u4Wp = u4Wp;
        aPTSDrvBuf[u1DecID][u2QBufIn[u1DecID]].u4Pts = u4PTS;
        u2QBufIn[u1DecID]++;
        if (u2QBufIn[u1DecID] >= PES_INFO_DLY_NUM) u2QBufIn[u1DecID] =0;
        //Count how many items put in driver.
        u2QDelayNum[u1DecID]++;
        ////// DEBUG //////
        //LOG(4,"  ...Dly@%d,R=0x%x,NextIdx=%d,W=0x%x, InWp=0x%x,DataSz=%d,FifoSz=%d\n", u2QBufIn[u1DecID], u4ReadPtr, u4NextIdx, PEQ_Q[u4NextIdx].u4Wp, u4Wp, u4DataSz,u4FifoSz);
        //Buffering ... Put info in driver delay buffer, and just return.
        return;
    }
    //  If there is items in delay buffer, get data from it.
    if (u2QDelayNum[u1DecID]> 0)
    {
        // Put new into drv delay buf, and get old from it.
        aPTSDrvBuf[u1DecID][u2QBufIn[u1DecID]].u4Wp = u4Wp;
        aPTSDrvBuf[u1DecID][u2QBufIn[u1DecID]].u4Pts = u4PTS;
        u4Wp = aPTSDrvBuf[u1DecID][u2QBufOut[u1DecID]].u4Wp;
        u4PTS = aPTSDrvBuf[u1DecID][u2QBufOut[u1DecID]].u4Pts;

        ////// DEBUG //////
        //LOG(4,"  ..Dly In %d,Out %d,Wp=0x%x\n",u2QBufIn[u1DecID],u2QBufOut[u1DecID],u4Wp);
        u2QBufIn[u1DecID]++;
        if (u2QBufIn[u1DecID] >= PES_INFO_DLY_NUM) u2QBufIn[u1DecID] =0;
        u2QBufOut[u1DecID]++;
        if (u2QBufOut[u1DecID] >= PES_INFO_DLY_NUM) u2QBufOut[u1DecID] =0;
    }

    // ============== 2. Check if FIFO wrap around, insert Info from the start of queue ================
    if (u4Wp < u4_Prev_Wp[u1DecID])
    {
        // Clear upper indexes. They are old data.
        // e.x. 1,2,3,...24,25, 1,2,3,...24, 1 => In this moment we should clear item 25.
        //// DEBUG /////
        //LOG(4, "PTS Q Wrap around, Clear  %d to %d\n", u2PesQIdx[u1DecID], u2MaxIdx[u1DecID]);
        u2TmpSize = 0;
        for(i=u2PesQIdx[u1DecID]; i<=u2MaxIdx[u1DecID]; i++)
        {
            PEQ_Q[i].u4Wp = 0;
            PEQ_Q[i].u4Pts = 0;
            u2TmpSize++;
        }
        DSP_FlushInvalidateDCacheFree((UINT32)&(PEQ_Q[u2PesQIdx[u1DecID]]), (UINT32)(sizeof(AUD_PES_INFO_T))*u2TmpSize  );
        // Record the max index used in this round.
        u2MaxIdx[u1DecID] = u2PesQIdx[u1DecID];
        // Use Index 0
        u2PesQIdx[u1DecID] = 0;
        u2PesSize[u1DecID] = u4_Q_Uinit[u1DecID];
        u4_Prev_Wp[u1DecID] = u4Wp;
    }

    // ==================== 3. Check if PES size is too small ===========================
    u2Size = u4Wp - u4_Prev_Wp[u1DecID];
    u2PesSize[u1DecID] += u2Size;
    u4_Prev_Wp[u1DecID] = u4Wp;
    //  If PES size is smaller than Q_Unit, means we should skip some. The queue can't accommodate all data of whole FIFO.
    if (u2PesSize[u1DecID] < u4_Q_Uinit[u1DecID])
    {
        return;
    }
    else
    {
        u2PesSize[u1DecID] = 0;
    }

    // ==================== 4. Finally, Fill into PES Info Queue ==================== 
    PEQ_Q[u2PesQIdx[u1DecID]].u4Wp = u4Wp;
    PEQ_Q[u2PesQIdx[u1DecID]].u4Pts = u4PTS;
    PEQ_Q[u2PesQIdx[u1DecID]].u4Expired = 0;
    PEQ_Q[u2PesQIdx[u1DecID]].u4Refresh = 0;            
    ////PEQ_Q[u2PesQIdx[u1DecID]].u4DmxWp = u4Wp + 0x200; //Test Dmx Wp
    //  remember to flush cache.
    DSP_FlushInvalidateDCacheFree((UINT32)&(PEQ_Q[u2PesQIdx[u1DecID]]), (UINT32)sizeof(AUD_PES_INFO_T));

    ////// DEBUG //////
    if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_SEND_PTS) == AUD_DBG_SYNC_SEND_PTS)
    {
        LOG(6, "Idx=%d, Wp=0x%x, PTS=0x%x, u4ReadPtr=0x%x\n",u2PesQIdx[u1DecID], u4Wp, u4PTS, u4ReadPtr);
    }    
    
    ////LOG(4, "Addr = 0x%x, Len = 0x%x\n", (UINT32)&(PEQ_Q[u2PesQIdx[u1DecID]]), (UINT32)sizeof(AUD_PES_INFO_T));
    u2PesQIdx[u1DecID]++;
    if (u2PesQIdx[u1DecID] >= PES_INFO_Q_LEN)
    {
        u2PesQIdx[u1DecID] = 0;
    }

}

#else

void AUD_DrvPtsQueueInsert(UINT8 u1DecID, UINT32 u4Wp, UINT32 u4PTS)
{
    UINT16 u2Size;
    UINT32 u4ReadPtr, u4WritePtr, u4FifoSz, u4DataSz;
    UINT32 u4FifoSA, u4FifoEA;
    UINT16 i;
    BOOL fgEven = FALSE;
    STC_CLOCK rStc;    
    // Use 1/2 size of table to put 2 rounds of data. (e.x. 0~511, 512~1023 in size 1024 case)
    // We should know it's even or odd round.
    UINT32 u4NextStart; //start of next loop.(e.x. even round use 0~511, nextstart is 512)
    UINT32 u4ShiftLen;  //We use size+shift to control flow. (0~511+"0" for even, 0~511+"512" for odd)
    //

    if (u1DecID > AUD_DEC_THIRD)
    {
        return;
    }

    if ((u4LoopCnt[u1DecID]%2) == 0)
    {
        fgEven = TRUE;
        u4ShiftLen = 0;                // use 0~511
        u4NextStart = PES_INFO_Q_LEN/2;// next loop start from 512
    }
    else
    {
        fgEven = FALSE;
        u4ShiftLen = PES_INFO_Q_LEN/2; // use 512~1023            
        u4NextStart = 0;               // next loop start from 0
    }
        
    ////// DEBUG //////
    ////LOG(4, "Insert Wp = 0x%x, PTS = 0x%x\n", u4Wp, u4PTS);

    // ============ 1. Check FIFO Status =================    
    _AUD_DMX_GetAudPtr(u1DecID, &u4ReadPtr, &u4WritePtr);
    u4ReadPtr = DSP_INTERNAL_ADDR(u4ReadPtr);
    u4WritePtr = DSP_INTERNAL_ADDR(u4WritePtr);
    u4FifoSA = u4GetAFIFOStart(AUD_DSP0, u1DecID);
    u4FifoEA = u4GetAFIFOEnd(AUD_DSP0, u1DecID);
    u4FifoSz = u4FifoEA-u4FifoSA;
    if (u4WritePtr >= u4ReadPtr)
    {
        u4DataSz = u4WritePtr-u4ReadPtr;
    }
    else
    {
        u4DataSz = (u4FifoEA-u4ReadPtr)+(u4WritePtr-u4FifoSA);
    }

    // ============== 2. Check if FIFO wrap around, insert Info from the start of queue ================
    if (u4Wp < u4_Prev_Wp[u1DecID])
    {
        //// DEBUG /////
        ////LOG(4, "(%d)PTS Q Wrap around, Clear %d to %d\n", u1DecID, u4NextStart, u4NextStart+PES_INFO_Q_LEN/2-1);
        // Clear data of 2 rounds ago.
        // Using 0..511 -> Using 512..1023 -> Clear 0..511 -> Using 0..511
        for(i=u4NextStart; i< (u4NextStart+PES_INFO_Q_LEN/2); i++)             
        {
            PEQ_Q[i].u4Wp = 0;
            PEQ_Q[i].u4Pts = 0;
        }
        DSP_FlushInvalidateDCacheFree((UINT32)&(PEQ_Q[u4NextStart]), (UINT32)(PES_INFO_Q_LEN/2));
        // Use Index from 0 to len/2 (and then add a shift length)
        u2PesQIdx[u1DecID] = 0;
        u2PesSize[u1DecID] = u4_Q_Uinit[u1DecID];
        u4_Prev_Wp[u1DecID] = u4Wp;
        u4LoopCnt[u1DecID] += 1;
        u4ShiftLen = u4NextStart;
    }

    // ==================== 3. Check if PES size is too small ===========================
    u2Size = u4Wp - u4_Prev_Wp[u1DecID];
    u2PesSize[u1DecID] += u2Size;
    u4_Prev_Wp[u1DecID] = u4Wp;
    //  If PES size is smaller than Q_Unit, means we should skip some. The queue can't accommodate all data of whole FIFO.
    if (u2PesSize[u1DecID] < u4_Q_Uinit[u1DecID])
    {
        return;
    }
    else
    {
        u2PesSize[u1DecID] = 0;
    }

    // ==================== 4. Finally, Fill into PES Info Queue ==================== 
    // Idx is 0 to len/2, with shift length added.
    PEQ_Q[u2PesQIdx[u1DecID]+u4ShiftLen].u4Wp = u4Wp;
    PEQ_Q[u2PesQIdx[u1DecID]+u4ShiftLen].u4Pts = u4PTS;
    PEQ_Q[u2PesQIdx[u1DecID]+u4ShiftLen].u4Expired = 0;
    PEQ_Q[u2PesQIdx[u1DecID]+u4ShiftLen].u4Refresh = 0;
    PEQ_Q[u2PesQIdx[u1DecID]+u4ShiftLen].u4DmxWp = u4Wp + 32; //First Pes use Wp+32 Byte for DmxWp
    u2PesQIdxBackup[u1DecID] = u2PesQIdx[u1DecID]+u4ShiftLen;
    //  remember to flush cache.
    DSP_FlushInvalidateDCacheFree((UINT32)&(PEQ_Q[u2PesQIdx[u1DecID]+u4ShiftLen]), (UINT32)sizeof(AUD_PES_INFO_T));

    ////// DEBUG //////
    if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_SEND_PTS) == AUD_DBG_SYNC_SEND_PTS)
    {
        STC_GetSrc(STC_SRC_A1, &rStc);
        LOG(4, "(%d)Idx=%d, Wp=0x%x, PTS=0x%x, u4RP=0x%x, stc=0x%x\n",u1DecID,u2PesQIdx[u1DecID]+u4ShiftLen, u4Wp, u4PTS, u4ReadPtr, rStc.u4Base);
    }    
    
    u2PesQIdx[u1DecID]++;
    if (u2PesQIdx[u1DecID] >= (PES_INFO_Q_LEN/2))
    {
        u2PesQIdx[u1DecID] = (PES_INFO_Q_LEN/2)-1; //keep using the last item
    }

}
#endif //new_arm_pts_table

void AUD_DrvPtsQueueUpdateWp(UINT8 u1DecID, UINT32 u4Wp) //new_arm_pts_table
{
    UNUSED(AUD_DrvPtsQueueUpdateWp);
    
    PEQ_Q[u2PesQIdxBackup[u1DecID]].u4DmxWp = u4Wp; 
    DSP_FlushInvalidateDCacheFree((UINT32)&(PEQ_Q[u2PesQIdxBackup[u1DecID]]), (UINT32)sizeof(AUD_PES_INFO_T));    
    ////// DEBUG //////
    if ((AUD_GetSyncDbgLvl()&AUD_DBG_SYNC_SEND_PTS) == AUD_DBG_SYNC_SEND_PTS)
    {
        LOG(9, "(%d)Idx=%d, DmxWp update to 0x%x\n",u1DecID, u2PesQIdxBackup[u1DecID]);
    }        
}

void AUD_DrvPtsQueueReset(UINT8 u1DecID)
{
    UINT16 i;

    if (u1DecID > AUD_DEC_THIRD)
    {
        LOG(3, "u1DecID = %d is invalid!\n", u1DecID);
        return;
    }

    for (i=0; i<PES_INFO_Q_LEN; i++)
    {
        PEQ_Q[i].u4Wp = 0;
        PEQ_Q[i].u4Pts = 0;
        PEQ_Q[i].u4Refresh = 0;        
        PEQ_Q[i].u4Expired = 0;         
    }
    u4_Prev_Wp[u1DecID] = 0xffffffff;
    u2PesSize[u1DecID] = 0;
    u2PesQIdx[u1DecID] = 0;

    UNUSED(fgFirstRound);
    fgFirstRound[u1DecID] = TRUE;
    u2QBufIn[u1DecID] = 0;
    u2QBufOut[u1DecID] = 0;
    u2QDelayNum[u1DecID] = 0;
    u2MaxIdx[u1DecID] = PES_INFO_Q_LEN_1;
    u4LoopCnt[u1DecID] = 1; //new_arm_pts_table. First Wp<0xffffffff, and will reset from next loop. We want to start from even loop.                            

    LOG(5, "\nAUD_DrvPtsQueueReset\n!!");    
}

extern UINT32 Dsp_ReadDspCmptDram(UINT8 u1Page, UINT32 u4Address);
void AUD_DrvPtsQueueQry(UINT8 u1DecID, UINT16 u2Size) //This is only for CLI test.
{
    UINT16 i;

    UINT32 u4ReadPtr;
    UINT32 u4WritePtr;
#if 0    
    UINT32 u4DspAddr;
    UINT32 u4Info, u4Bank, u4PTS;
#endif    

    if (u1DecID > AUD_DEC_THIRD)
    {
        LOG(3, "u1DecID = %d is invalid!\n", u1DecID);
        return;
    }

    // Dump the content of PES info Queue. Can set the size for dump.
    _AUD_DMX_GetAudPtr(u1DecID, &u4ReadPtr, &u4WritePtr);
    LOG(3, "PTS Q Dec(%d) is CMPT 0x%x(DWORD)\n", u1DecID, (_arPesQueue[u1DecID]-_u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX])/4);
    LOG(3, "u4ReadPtr = 0x%08x (0x%x) \n", u4ReadPtr, DSP_INTERNAL_ADDR(u4ReadPtr));
    LOG(3, "u2PesQIdx = %d \n", u2PesQIdx[u1DecID]);

    if ((u2Size == 0)||(u2Size > PES_INFO_Q_LEN))
    {
        u2Size = PES_INFO_Q_LEN;
    }
    for (i=0; i<u2Size; i++)
    {
        LOG(3, "[%04d] 0x%x, 0x%x\n", i, PEQ_Q[i].u4Wp, PEQ_Q[i].u4Pts);
    }
#if 0 //debug only
    // Dump the PTS Refresh Queue in DSP.
    u4DspAddr = u4ReadDspSram(0x9EF); // pts_q1_address
    u4DspAddr += 0x100 * u1DecID;     // Every Dec has 0x100 dword queue.
    DSP_FlushInvalidateDCacheFree((_u4DspCmptBuf[3]+((u4DspAddr)<<2)), (0x100 << 2));    
    LOG(3, "\nRefresh Q Dec(%d) is CMPT 0x%x\n", u1DecID, u4DspAddr);    
    for (i=0; i<32; i++)
    {
        u4Info = (Dsp_ReadDspCmptDram(3, u4DspAddr))>>16;
        u4Bank = Dsp_ReadDspCmptDram(3, u4DspAddr+1);
        u4PTS = Dsp_ReadDspCmptDram(3, u4DspAddr+2);        
        u4DspAddr += 8;
        LOG(3, "%d, 0x%08x, 0x%08x\n", u4Info, u4Bank, u4PTS);
    }
    #endif
}

INT16 AUD_DrvPtsQueueFindInfo(UINT8 u1DecID, UINT32 u4Wp) //This is only for CLI test in driver
{
    //
    // Specify a Wp and find the corresponding Info in the queue.
    //
    UINT16 u2Low = 0;
    UINT16 u2High = PES_INFO_Q_LEN-1;
    UINT16 u2Mid = 0;
    UINT32 u4Data = 0;

    UINT32 u4LowData = 0;
    UINT32 u4HighData = 0;
    UINT32 u4ReadPtr;
    UINT32 u4WritePtr;

    if (u1DecID > AUD_DEC_THIRD)
    {
        LOG(3, "u1DecID = %d is invalid!\n", u1DecID);
        return 0;
    }

    _AUD_DMX_GetAudPtr(0, &u4ReadPtr, &u4WritePtr);

    if (u4Wp == 0)                            // If Wp is 0, just pick the current Rp.
    {
        u4Wp = DSP_INTERNAL_ADDR(u4ReadPtr);
    }

    LOG(3, "Looking for 0x%x \n", u4Wp);

    // 1. Do Binary search
    while (u2Low < (u2High-1))
    {
        u2Mid = (u2Low+u2High)/2 ;

        u4Data = PEQ_Q[u2Mid].u4Wp;

        if (u4Data == 0)                     // Wp is 0 means this part of queue is not used.
        {
            u2High = u2Mid;
        }
        else if (u4Wp > u4Data)
        {
            u2Low = u2Mid;
        }
        else if (u4Wp < u4Data)
        {
            u2High = u2Mid;
        }
        else if (u4Wp == u4Data)            // Find a exactly matched Wp.
        {
            LOG(3, "!!!! Found !!! Index = %d,0x%x \n", u2Mid, u4Data);
            return 0;
        }

        u4LowData = PEQ_Q[u2Low].u4Wp;
        u4HighData = PEQ_Q[u2High].u4Wp;
        LOG(3,"Search...0x%x. LowWp=%d,0x%x, HighWp=%d,0x%x \n", u4Wp, u2Low, u4LowData, u2High, u4HighData);
    }

    // 2. Whole queue is searched. Check if the final result is valid.
    u4LowData = PEQ_Q[u2Low].u4Wp;
    u4HighData = PEQ_Q[u2High].u4Wp;
    LOG(3, "Search Stop!! Wp=0x%x, LowWp=%d,0x%x, HighWp=%d,0x%x\n", u4Wp, u2Low,u4LowData, u2High,u4HighData);
    if ((u4Wp>=u4LowData)&&(u4Wp<=u4HighData))
    {
        LOG(3, "Search Result OK !\n");
    }
    else
    {
        LOG(3, "No match data in queue !!\n");
        return -1;
    }

    return 0;
}


UINT32 u4AUD_DrvGetPtsTableAddr(void)
{
    UNUSED(u4AUD_DrvGetPtsTableAddr);

    return _u4DspCmptBuf[AUD_DSP0][TYPE_COMM_DRAM_IDX] + (DRAM_PES_INFO_QUEUE);
}
UINT32 u4AUD_DrvGetPtsTableSize(void)
{
    UNUSED(u4AUD_DrvGetPtsTableSize);
    
    return ((PES_INFO_Q_ITEMS*4)*PES_INFO_Q_LEN);
}


//--------- End of PTS Queue ---------------

#ifdef CC_AUD_HP_DEPOP  ///For Remy, need remove "CC_DAC_DE_POP".
void _AudHpDePopTaskStatusChange(AUD_HP_DEPOP_CMD_T u4Message)
{
    UINT32 u4Msg = 0;
    u4Msg = u4Message;
    switch(u4Msg)
    {
        case AUD_HP_DEPOP_DC_ON:
            while(_fgHpStateLock);
            _eAudHpDePopTaskStatus = AUD_HP_DEPOP_DC_ON;
            VERIFY(x_sema_unlock(_hSemaHpDepopWait) == OSR_OK);
            break;

        case AUD_HP_DEPOP_DC_OFF:
            while(_fgHpStateLock);
            _eAudHpDePopTaskStatus = AUD_HP_DEPOP_DC_OFF;
            VERIFY(x_sema_unlock(_hSemaHpDepopWait) == OSR_OK);
            break;

        default:
            break;
    }
}

///Delay Time Unit.
#define AUD_HP_DEPOP_CNT_DELAY      100     /// 100msec
///Delay Time (ratio: 100msec)
#define AUD_HP_DEPOP_DC_ON_DLY      400     /// 400msec
#define AUD_HP_DEPOP_CASE_D_DLY     1500    /// 1500ms
#define AUD_HP_DEPOP_CASE_E_DLY     0
#define AUD_HP_DEPOP_CASE_G_DLY     100     /// 100ms
#define AUD_HP_DEPOP_CASE_I_DLY     0
#define AUD_HP_DEPOP_DC_OFF_DLY     0
#define AUD_HP_DEPOP_IDLE_DLY       0

static void _AudHpDePopThread(void* pvArg)
{
    //UINT32 u4Event;
    //UINT16 u2MsgIdx;
    //SIZE_T zMsgSize = sizeof(UINT32);

    ///EV_GRP_EVENT_T  rAudHpDePopEvent;

    ///AC & DC On, Do "AUD_HP_DEPOP_CASE_D" first
    _eAudHpDePopTaskStatus = AUD_HP_DEPOP_DC_ON;
    _u4HpDePopDelayCnt = 0;
    _fgHpStateLock = TRUE;

    while (1)
    {
    #if 0   /// For Debug Use
        ///Toggle A_MUTE for scope monitor.
        AUD_GPIOOutput(203, 0);
        x_thread_delay(10);
        AUD_GPIOOutput(203, 1);
    #endif

        ///HP Register Setting Part
    #if 0   /// For Debug Use
        LOG(0,"=========================================================================\n");
        LOG(0,"[HP_DEPOP] %s\n", _aszHpDePopCase[_eAudHpDePopTaskStatus]);
    #endif
        _fgHpStateLock = TRUE;
        switch (_eAudHpDePopTaskStatus)
        {
            case AUD_HP_DEPOP_DC_ON:
                //En VMID and Dis 1Mohm for fast charge VMID voltage
                ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0)|VCMGEN_LP_1M));   //[5:4]=b'10, enable VMID
                ANA_CLR_BIT(REG_AADC_CFG1, ADC_VCMGEN_1M_R_ON);     //[15]=0, disable 1Mohm mode.   
                ///w 0xf0060108 0x00000000, initial value
                ANA_WRITE32(REG_ADAC_CFG2,  0);
                ///w 0xf006010C 0x00700222, HIZ, DFC, FS
           #if defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5882) 
                ANA_WRITE32(REG_ADAC_CFG3, (DAC_DFC_CH2|DAC_DFC_CH1|DAC_DFC_CH0)|(DAC_FS_CH2_P42|DAC_FS_CH1_P42|DAC_FS_CH0_P42));
                ///w 0xf0060110 0xAA100000, Bias I, RampGen setting (UP: 400 ms, DOWN: 17 ms)
                ANA_WRITE32(REG_ADAC_CFG4, (IB_PDET_10UA|IB_OPA_10UA|IB_PWMBUF_10UA|IB_STEP_10UA)|AC_OFF_PWRDET);
                ///w 0xf0060110 0xAA100604, Bias I, RampGen setting (UP: 400 ms, DOWN: 17 ms)
                ///ANA_WRITE32(REG_ADAC_CFG4, (IB_PDET_10UA|IB_OPA_10UA|IB_PWMBUF_10UA|IB_STEP_10UA)|AC_OFF_PWRDET|RAMP_STEP_398MV|CKF_DIV_8192);
                ///w 0xf0060114 0x50000003, RampGen setting, REV1
                ANA_WRITE32(REG_ADAC_CFG5, CKF_ST_DIV_16384|RAMP_INV_DIS|DAC_DESENSE_CH1);
                ///w 0xf0060118 0xC0010000, REV0
                ANA_WRITE32(REG_ADAC_CFG6, ACOFF_CKT_CH0|ACOFF_CKT_CH1|DAC_DESENSE_CH0);
           #elif defined(CC_MT5890) 
                ANA_CLR_BIT(REG_ADAC_CFG3, DAC_HIZ_EN);
                ANA_WRITE32(REG_ADAC_CFG4, (DAC_OC_EN | DAC_OC_HIGH_SEL00 | DAC_OC_LOW_SEL11 | DAC_DFC_CH0));
                ANA_WRITE32(REG_ADAC_CFG5, (DAC_FS_CH3_P42 | DAC_FS_CH2_P42 | DAC_FS_CH1_P42 | DAC_FS_CH0_P42 | DAC_IB_IBGEN_10UA | IB_PDET_10UA | IB_HPA_10UA | IB_LOA_10UA | IB_PWMBUF_10UA | IB_STEP_10UA));
                ANA_WRITE32(REG_ADAC_CFG6, (RAMP_STEP_398MV | RAMP_STEP_ST_53MV | CKF_DIV_1024) | CKF_ST_DIV_8192);
                ANA_WRITE32(REG_ADAC_CFG7, DAC_DESENSE_CH3 | DAC_DESENSE_CH2 | DAC_DESENSE_CH1 | DAC_DESENSE_CH0);
                if (IS_IC_5890_ES1())
                {
                    ANA_CLR_BIT(REG_ADAC_CFG9, DAC_432M_EN);
                }
                else
                {
                    ANA_CLR_BIT(REG_ADAC_CFG9, DAC_HADDS1_EN);
                }
                ANA_WRITE32(REG_ADAC_CFG10, DAC_RAMP_R2R_EN|DAC_RAMP_DELAY_EN);
           #endif
                ///w 0xf0060118 0xC2010000, REV0
                ///ANA_WRITE32(REG_ADAC_CFG6, ACOFF_CKT_CH0|ACOFF_CKT_CH1|RAMP_OUT_EN|DAC_DESENSE_CH0);
                ///Delay 400msec
                _u4HpDePopDelayCnt = (AUD_HP_DEPOP_DC_ON_DLY/AUD_HP_DEPOP_CNT_DELAY);     /// 4*100 = 400msec
                _eAudHpDePopTaskStatus = AUD_HP_DEPOP_CASE_D;
                break;
            case AUD_HP_DEPOP_CASE_D:
                ANA_SET_BIT(REG_AADC_CFG1, ADC_VCMGEN_1M_R_ON);     //[15]=1, Enable 1Mohm mode.   
                ///w 0xf0060108 0x00010AAA(d)
                ANA_WRITE32(REG_ADAC_CFG2, ACT_UP|(ACT_ENDP_CH2|ACT_SLEW_CH2)|(ACT_ENDP_CH1|ACT_SLEW_CH1)|(ACT_ENDP_CH0|ACT_SLEW_CH0));
                ///Delay 1000msec
                _u4HpDePopDelayCnt = (AUD_HP_DEPOP_CASE_D_DLY/AUD_HP_DEPOP_CNT_DELAY);
                _eAudHpDePopTaskStatus = AUD_HP_DEPOP_CASE_E;
                break;
            case AUD_HP_DEPOP_CASE_E:
                ///w 0xf0060108 0x00010666(e)
                ANA_WRITE32(REG_ADAC_CFG2, ACT_UP|(ACT_ENVO_CH2|ACT_SLEW_CH2)|(ACT_ENVO_CH1|ACT_SLEW_CH1)|(ACT_ENVO_CH0|ACT_SLEW_CH0));
                ///0msec
                _u4HpDePopDelayCnt = (AUD_HP_DEPOP_CASE_E_DLY/AUD_HP_DEPOP_CNT_DELAY);
                #if defined(CC_MT5890) || defined(CC_MT5882)
                //disable PWM & FIFO,then enable PWM & FIFO ,in case of noise issue happen
                AUD_WRITE32(REG_PWMIP_MON_PG1, 0x00);
                // 2. Set A_PGTRL0 0xf01 bit0 = 1 enable PWM
                vWritePWMG1(A_PGCTRL0, 0xF542C0);
                vWritePWMG1(A_PDATA, 0);
                vWritePWMG2(A_PGCTRL0, 0xF542C0);
                vWritePWMG2(A_PDATA, 0);
                vWritePWMG3(A_PGCTRL0, 0xF542C0);
                vWritePWMG3(A_PDATA, 0);
                
                AUD_WRITE32(REG_PWMIP_MON_PG1, PWMIP_MON_PG1_INIT);
                vWritePWMG1(A_PGCTRL0, C_PGCTRL0_PEN);
                vWritePWMG1(A_PDATA, 0);
                vWritePWMG2(A_PGCTRL0, C_PGCTRL0_PEN);
                vWritePWMG2(A_PDATA, 0);
                vWritePWMG3(A_PGCTRL0, C_PGCTRL0_PEN);
                vWritePWMG3(A_PDATA, 0);
                #endif
                _eAudHpDePopTaskStatus = AUD_HP_DEPOP_CASE_G;
                break;
            case AUD_HP_DEPOP_CASE_G:
                ///w 0xf0060108 0x00000777(g)
                ANA_WRITE32(REG_ADAC_CFG2, (ACT_ENVO_CH2|ACT_SLEW_CH2|ACT_PLAY_CH2)|(ACT_ENVO_CH1|ACT_SLEW_CH1|ACT_PLAY_CH1)|(ACT_ENVO_CH0|ACT_SLEW_CH0|ACT_PLAY_CH0));
                ///100msec
                _u4HpDePopDelayCnt = (AUD_HP_DEPOP_CASE_G_DLY/AUD_HP_DEPOP_CNT_DELAY);
                _eAudHpDePopTaskStatus = AUD_HP_DEPOP_CASE_I;
                break;
            case AUD_HP_DEPOP_CASE_I:
                ///w 0xf0060108 0x00000555(i)
                ANA_WRITE32(REG_ADAC_CFG2, (ACT_ENVO_CH2|ACT_PLAY_CH2)|(ACT_ENVO_CH1|ACT_PLAY_CH1)|(ACT_ENVO_CH0|ACT_PLAY_CH0));
                ///0msec
                _u4HpDePopDelayCnt = (AUD_HP_DEPOP_CASE_I_DLY/AUD_HP_DEPOP_CNT_DELAY);
                _eAudHpDePopTaskStatus = AUD_HP_DEPOP_IDLE;
                break;
            case AUD_HP_DEPOP_DC_OFF:
                ///w 0xf0060108 0x00000000 DC Off
                ANA_WRITE32(REG_ADAC_CFG2, 0x00000000);
                ///0msec
                _u4HpDePopDelayCnt = (AUD_HP_DEPOP_DC_OFF_DLY/AUD_HP_DEPOP_CNT_DELAY);
                _eAudHpDePopTaskStatus = AUD_HP_DEPOP_IDLE;
                break;
            /// IDLE State and semaphone lock
            case AUD_HP_DEPOP_IDLE:
                ///Delay after IDLE state unlock.
                _u4HpDePopDelayCnt = (AUD_HP_DEPOP_IDLE_DLY/AUD_HP_DEPOP_CNT_DELAY);
                ///For other thread change _eAudHpDePopTaskStatus.
                _fgHpStateLock = FALSE;
                VERIFY(x_sema_lock(_hSemaHpDepopWait, X_SEMA_OPTION_WAIT) == OSR_OK);
                break;
            default:
                _fgHpStateLock = FALSE;
                LOG(0, "[HP_DEPOP] !!!!!!SETTING ERROR!!!!!!\n");
                break;
        }
        _fgHpStateLock = FALSE;

    #if 0   /// For Debug Use
        #if defined (CC_MT5399)
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", (REG_AADC_CFG0+0x1000), ANA_READ32(REG_AADC_CFG0));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", (REG_AADC_CFG1+0x1000), ANA_READ32(REG_AADC_CFG1));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", (REG_ADAC_CFG2+0x1000), ANA_READ32(REG_ADAC_CFG2));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", (REG_ADAC_CFG3+0x1000), ANA_READ32(REG_ADAC_CFG3));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", (REG_ADAC_CFG4+0x1000), ANA_READ32(REG_ADAC_CFG4));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", (REG_ADAC_CFG5+0x1000), ANA_READ32(REG_ADAC_CFG5));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", (REG_ADAC_CFG6+0x1000), ANA_READ32(REG_ADAC_CFG6));
        LOG(0,"=========================================================================\n");
        #elif defined (CC_MT5881)
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", REG_AADC_CFG0, ANA_READ32(REG_AADC_CFG0));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", REG_AADC_CFG1, ANA_READ32(REG_AADC_CFG1));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", REG_ADAC_CFG2, ANA_READ32(REG_ADAC_CFG2));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", REG_ADAC_CFG3, ANA_READ32(REG_ADAC_CFG3));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", REG_ADAC_CFG4, ANA_READ32(REG_ADAC_CFG4));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", REG_ADAC_CFG5, ANA_READ32(REG_ADAC_CFG5));
        LOG(0,"[HP_DEPOP] 0xf006%04x = 0x%08x \n", REG_ADAC_CFG6, ANA_READ32(REG_ADAC_CFG6));
        LOG(0,"=========================================================================\n");
        #endif
    #endif

        ///Delay Part
        while(_u4HpDePopDelayCnt)
        {
            x_thread_delay(AUD_HP_DEPOP_CNT_DELAY);
            _u4HpDePopDelayCnt--;
        }
    }
}
#endif

#if defined(CC_AUD_ARM_SUPPORT) 
void AUD_Aproc_Reset_AVSyncInfo(UINT8 u1DecID) //new_arm_pts_table
{
    UNUSED(AUD_Aproc_Reset_AVSyncInfo);
    UNUSED(_gu4ValidPtsCounter);
        
    _gu4ValidPtsCounter[u1DecID] = 0;
    _gi4StcDiff[u1DecID] = 0;
    fgSkipUnderHold[u1DecID] = FALSE; //Set False when Hold or WAIT_STOP. Set Ture when trigger Skip under Hold.
}
void AUD_Aproc_Chg_AVSyncCtrl(UINT8 u1DecID, APROC_AVSYNC_CTRL_MODE_T eMode)
{
    gsAprocSyncCtrl[u1DecID] = eMode;
}

UINT8 _gu1AprocLogID;
UINT8 _gu1AprocLogItems = 0;
BOOL fgAVSyncTestMode[3] = {FALSE,FALSE,FALSE};
void AUD_Aproc_Log_ID(UINT8 u1DecID)
{
    //fix warning.
    UNUSED(AUD_Aproc_Chg_AVSyncCtrl);
    UNUSED(gsAprocSyncCtrl);
    UNUSED(_gu1AprocLogID);
    UNUSED(AUD_Aproc_Log_ID);
    UNUSED(AUD_Aproc_Avsync_Ctrl);
    UNUSED(AUD_Aproc_Calc_StcDiff);
    UNUSED(_gi4StcDiff);
    UNUSED(fgAVSyncTestMode);
    
    _gu1AprocLogID = u1DecID;
}
void AUD_Aproc_Log_Items(UINT8 u1Value)
{
    //fix warning.
    UNUSED(AUD_Aproc_Log_Items);
    UNUSED(_gu1AprocLogItems);

    _gu1AprocLogItems = u1Value;
}

BOOL AUD_Aproc_Skip_Pts(UINT8 u1DecID, UINT32 u4Pts, BOOL fgReset)    
{
    static UINT32 u4Counter[AUD_DEC_NUM];
    static BOOL fgFirstTime = TRUE;
    BOOL fg4Ret = FALSE;
    BOOL fgSpecial = FALSE;
    INT32 i4Bound;
    UINT32 u4Cnt;

    UNUSED(AUD_Aproc_Skip_Pts);
    UNUSED(fgFirstTime);
    UNUSED(u4Pts);

    // Reset information when first play
    if (fgReset)
    {
        u4Counter[u1DecID] = 0;           
        fgFirstTime = TRUE;
        return fg4Ret;
    }

    // Some decoders don't handle PTS well.
    // We should discard some abnormal PTS reported.
    if (_arAudDecoder[AUD_DSP0][u1DecID].eDecFormat == AUD_FMT_WMA)  
    {
        i4Bound = 2000;
        u4Cnt = 6;
        fgSpecial = TRUE;
    }

    if (fgSpecial)
    {
        if ((_gi4StcDiff[u1DecID]>i4Bound) || (_gi4StcDiff[u1DecID]<(-1*i4Bound)))        
        {
            u4Counter[u1DecID]++;
            fg4Ret = TRUE;
        }
        else 
        {
            u4Counter[u1DecID] = 0;
        }
        
        if ((!fgFirstTime) && (u4Counter[u1DecID] > u4Cnt))
        {
            u4Counter[u1DecID] = 0;
            fg4Ret = FALSE;
        }
        //LOG(8,"(%d) Diff=%d, Cnt=%d, Ret=%d\n", u1DecID, _gi4StcDiff[u1DecID], u4Counter[u1DecID], fg4Ret);        
    }

    fgFirstTime = FALSE;    
    return fg4Ret;
    
}
void AUD_Aproc_Calc_StcDiff(UINT8 u1DecID, UINT32 u4PTS) //ISR !!!
{
    // =====================================
    // Please take care !!!
    // This function is called by ISR !!!!!
    // Don't do any long time operation or wait anything here !!!!!!
    // =====================================
    
    STC_CLOCK rStc;
    UINT32 u4Valid =0;
    UINT32 u4RealPTS =0;
    UINT32 u4Putbank,u4Putbank_Asm;
    UINT32 u4AACVersion;
    static UINT32 au4PrevBank[3] = {0,0,0};
    static UINT32 au4PrevBankAsm[3] = {0,0,0};    
    static UINT32 au4PrevPTS[3] = {0,0,0};    
    UINT32 u4Reg0, u4Reg1;
    BOOL fgDiscardPts = FALSE;

    u4Valid = u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0, APROC_REG_AVSYNC_NOTIFY_APTS_SRC0+u1DecID));
    u4RealPTS = ((u4Valid) & 0x2)>>1;   //get bit1  : 1-Real APTS (not from Interpolate), 0-Interpolated PTS.
    u4Valid = (u4Valid) & 0x1;          //get bit0  : PTS Dectect (and valid, not under skip)

    u4Putbank = u4ReadD2RInfo_AputBank(AUD_DSP0, u1DecID);
    u4Putbank_Asm = u4AprocReg_Read (APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0, APROC_REG_APTS_APUT_BANK0+u1DecID));
    u4Putbank_Asm = u4Putbank_Asm >> 8; 
    u4AACVersion = u4ReadD2RInfo_AACVersion(u1DecID);

    u4Reg0 = _gu1AprocLogID&0xF;

//// parson debug
//    LOG(7, "(aproc_sync)Dec(%d), V%d, PTS=0x%x\n",u1DecID, u4Valid, u4PTS);
/////    

    u4Reg1 = (UINT32)STC_GetSrc(STC_SRC_A1, &rStc); 
    if ((u4Valid) && (u4Reg1 == STC_VALID) && (u4RealPTS)) //Use Real PTS is enough.
    {
        _gi4StcDiff[u1DecID] = rStc.u4Base - u4PTS;
    }
    else
    {
        _gi4StcDiff[u1DecID] = 0;
    }
    
    if ( ((u1DecID == u4Reg0) || (u4Reg0==5)) &&
          ((_gu1AprocLogItems&AUD_DBG_APROC_PTS)==AUD_DBG_APROC_PTS) )
    {
        if (u4RealPTS)
        {
            LOG(5, "PTS(%d)=0x%x(%d)(%d), PrevPTS=0x%x, STC=0x%x, Diff=%d, V%d, R%d\n",
                u1DecID, u4PTS, u4PTS-au4PrevPTS[u1DecID], u4Putbank_Asm, au4PrevPTS[u1DecID],
                rStc.u4Base, _gi4StcDiff[u1DecID], u4Valid, u4RealPTS);
        }
        else
        {
            LOG(6, "PTS(%d)=0x%x(%d)(%d), PrevPTS=0x%x, STC=0x%x, Diff=%d, V%d, R%d\n",
                u1DecID, u4PTS, u4PTS-au4PrevPTS[u1DecID], u4Putbank_Asm, au4PrevPTS[u1DecID],
                rStc.u4Base, _gi4StcDiff[u1DecID], u4Valid, u4RealPTS);
        }

        LOG(8, "K%06d(%02d)|%06d(%02d),v%d\n",            
        u4Putbank, (u4Putbank-au4PrevBank[u1DecID]),
        u4Putbank_Asm,(u4Putbank_Asm-au4PrevBankAsm[u1DecID]) ,u4AACVersion);
    }
    au4PrevBankAsm[u1DecID] = u4Putbank_Asm;
    au4PrevBank[u1DecID] = u4Putbank;
    au4PrevPTS[u1DecID]  = u4PTS;

    if (u4RealPTS == 1)
    {
        // Discard some abnormal PTS 
        fgDiscardPts = AUD_Aproc_Skip_Pts(u1DecID, u4PTS, FALSE);
    }

    u4Reg0 = (_gu1AprocLogID&0xF0) >> 4; // Check if need to print debug message
    if (_arAudDecoder[AUD_DSP0][u1DecID].eSynMode == AV_SYNC_AUDIO_MASTER)
    {
        if ((u4Valid) && (u4RealPTS) && (!fgDiscardPts))
        {
            if ((u4Reg0 == u1DecID) &&
                 ((_gu1AprocLogItems&AUD_DBG_APROC_UPDATE_STC)==AUD_DBG_APROC_UPDATE_STC))
            {
                LOG(6, "Dec(%d) Update STC to 0x%x\n", u1DecID, u4PTS);
            } 
#ifdef CC_ENABLE_AOMX
            if (_arAudDecoder[AUD_DSP0][u1DecID].eStreamFrom == AUD_STREAM_FROM_GST)
            {
                GST_SetPtsVal(u1DecID, u4PTS);
            }
            else
#endif
            {
                STC_SetStcValue(0, u4PTS); //Test, StcID is fixed to 0
            }
        }
        return;
    }
}

inline void AUD_Aproc_Do_Skip(UINT8 u1DecID, INT32 i4Size, INT32 i4Diff) //new_arm_pts_table
{
    UINT32 u4Reg0;
        
    u4Reg0 = u4AprocReg_Read(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_SKIP_FRMNUM_SRC0+u1DecID)));
    LOG(5,"(Aproc_Sync)Dec(%d) NeedSkip, FSz %d, SkipAsm %d\n", u1DecID, i4Size, u4Reg0);
    if ((i4Size > 0)&&(u4Reg0 == 0))  //i4FrameSz is info from dsp->aproc->driver
    {
        i4Size = (i4Diff/i4Size);             // STC(90k based) to ms
        //i4FrameSz = (i4FrameSz >= 9)? 9 : i4FrameSz;
        LOG(2,"(Aproc_Sync)Diff is %d, So Dec(%d) Skip %d\n", i4Diff, u1DecID, i4Size);
        if (i4Size > 0) //i4FrameSz is now number of frames need to be skipped.
        {
            DSP_SetSkipFrame(u1DecID, i4Size);
            _gi4StcDiff[u1DecID] = 0;
        }   
    }
}

void AUD_Aproc_Avsync_Ctrl(UINT8 u1DecID)
{
    INT32 i4FrameSz = 0;
    UINT32 u4Pts;
    STC_CLOCK rStc;    
    INT32 i4LocalStcDiff = 0;
    INT32 i4LowBound;
    INT32 i4HighBound; 
    //UINT32 u4Reg0;
    static BOOL  fgAudSendMMnotify[AUD_DEC_MAX]= {FALSE};

    if (_arAudDecoder[AUD_DSP0][u1DecID].eSynMode == AV_SYNC_AUDIO_MASTER)
    {
        if (gsAprocSyncCtrl[u1DecID] == APROC_AVSYNC_CTRL_HOLD_COMPLETE)
        {
            if (!fgAudSendMMnotify[u1DecID])
            {
                LOG(5, "[Aproc_Sync]Notify MM Aout Enable\n");
                fgAudSendMMnotify[u1DecID] = TRUE;
                _fgAudMMnotifySWDMX[u1DecID] = TRUE;
                AUD_DrvMMAoutEnableNotify(u1DecID, FALSE);
            }
        }
        else
        {
            fgAudSendMMnotify[u1DecID] = FALSE;
        }
        return;
    }

    i4FrameSz = (INT32)u4AprocReg_Read(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_APTS_FRM_SZ_SRC0+u1DecID)));
    if (i4FrameSz == 0)
    {
        return;
    }

    //LOG(0, "AUD_Aproc_Avsync_Ctrl(%d), ctrl=%d, diff=%d\n", u1DecID, gsAprocSyncCtrl[u1DecID], _gi4StcDiff[u1DecID]);
    
    // Hold Complet State
    STC_GetSrc(STC_SRC_A1, &rStc);
    if (gsAprocSyncCtrl[u1DecID] == APROC_AVSYNC_CTRL_HOLD_COMPLETE)
    {
        // PTS is from Share REG (for the next APTS after HOLD)
        u4Pts = u4AprocReg_Read(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_APTS_SRC0+u1DecID)));
        if (STC_GetSrc(STC_SRC_A1, &rStc) == STC_VALID)
        {
            _gi4StcDiff[u1DecID] = rStc.u4Base - u4Pts;
        }
        else
        {
            _gi4StcDiff[u1DecID] = 0;
        }
        LOG(5, "(Aproc_Sync)Dec(%d)HOLD C,u4Pts=0x%x,stc=0x%x,stc_diff=%d\n", u1DecID, u4Pts, rStc.u4Base, _gi4StcDiff[u1DecID]);   
        // If need skip under hold ?? (Usually it's for WAIT_PLAY -> ON_PLAY)
        if ((fgSkipUnderHold[u1DecID]==FALSE)
            && ((_gi4StcDiff[u1DecID] < i4FrameSz*60) && (_gi4StcDiff[u1DecID] > i4FrameSz*1))
            && (!fgAVSyncTestMode[u1DecID]))
        {
            LOG(5,"(Aproc_Sync)Dec(%d) SKIP %d under Hold Complete!\n",u1DecID);   
            AUD_Aproc_Do_Skip(u1DecID, i4FrameSz, _gi4StcDiff[u1DecID]);
            fgSkipUnderHold[u1DecID] = TRUE;
        }
        // Set Wait Off after Amixer get Skip command.
        //u4Reg0 = u4AprocReg_Read(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_SKIP_FRMNUM_SRC0+u1DecID)));
        if (/*(u4Reg0 == 0) &&*/ (fgSkipUnderHold[u1DecID]))
        {
            AUD_Aproc_Chg_AVSyncCtrl(u1DecID, APROC_AVSYNC_CTRL_WORK);
            vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecID)), AVSYNC_CMD_WAIT_OFF);
            fgSkipUnderHold[u1DecID] = FALSE;
            _gi4StcDiff[u1DecID] = 0; //this diff is calculated by hold status, not a newest value
            LOG(5,"(Aproc_Sync)Dec(%d) HOLD OFF after SKIP!!!\n",u1DecID);   
            // Wait for next stc_diff.
            return;
        }
    }

    i4LocalStcDiff = _gi4StcDiff[u1DecID]; //_gi4StcDiff may change by Interrupt at runtime
    
    if (fgAVSyncTestMode[u1DecID]) //Test Mode : AVSync is Control by CLI
    {
        return;
    }

    if (!AUD_IsSourceFromWFD(u1DecID))
    {
        i4LowBound = i4FrameSz*1;
        i4HighBound = i4FrameSz*60;
    }
    else
    {
        i4LowBound = APLL_ADJUST_BOUND4;
        i4HighBound = APLL_ADJUST_BOUND4 * 20;
    }
    
    if ((i4LocalStcDiff < -i4LowBound) && (i4LocalStcDiff > -i4HighBound))  // === Audio Fast, Hold ===
    {
        if (gsAprocSyncCtrl[u1DecID] == APROC_AVSYNC_CTRL_WORK)
        {
            AUD_Aproc_Chg_AVSyncCtrl(u1DecID, APROC_AVSYNC_CTRL_HOLD);
            vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecID)), AVSYNC_CMD_WAIT_ON);
            _gi4StcDiff[u1DecID] = 0;
            fgSkipUnderHold[u1DecID] = FALSE;
            LOG(2,"(Aproc_Sync)Dec(%d) stc_diff=%d, HOLD!!\n", u1DecID, i4LocalStcDiff);
        }
    }
    else if ((i4LocalStcDiff < i4HighBound) && (i4LocalStcDiff > i4LowBound)) // === Audio Slow, Skip ===
    {
        if (gsAprocSyncCtrl[u1DecID] == APROC_AVSYNC_CTRL_WORK)
        {
            AUD_Aproc_Do_Skip(u1DecID, i4FrameSz, i4LocalStcDiff);
        }        
    }
    else                                                                      // === Audio on time, OK ===
    {
        if (gsAprocSyncCtrl[u1DecID] == APROC_AVSYNC_CTRL_HOLD_COMPLETE)
        {
            AUD_Aproc_Chg_AVSyncCtrl(u1DecID, APROC_AVSYNC_CTRL_WORK);
            vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecID)), AVSYNC_CMD_WAIT_OFF);
            LOG(5,"(Aproc_Sync)Dec(%d) HOLD OFF , stc_diff=%d\n",u1DecID, i4LocalStcDiff);
        }
    }

}

#ifdef CC_AUD_ARM_RENDER
void AUD_Aproc_Qry_Sync(UINT8 u1DecID)
{
    CHAR *pSyncCtrl[] = {"WORKING", "HOLD", "HOLD_COMPLETE"};
    CHAR *pSyncMode[] = {"Free run", "System Master", "Audio Master"};

    UNUSED(AUD_Aproc_Qry_Sync); 

    LOG(0, "gsAprocSyncCtrl[%d]=%s\n", u1DecID, pSyncCtrl[gsAprocSyncCtrl[u1DecID]]);    
    LOG(0, "_gu4ValidPtsCounter[%d]=%d\n", u1DecID, _gu4ValidPtsCounter[u1DecID]);
    LOG(0, "_gi4StcDiff[%d]=%d\n", u1DecID, _gi4StcDiff[u1DecID]);
    LOG(0, "_arAudDecoder[%d].eSynMode=%s\n", u1DecID, pSyncMode[_arAudDecoder[AUD_DSP0][u1DecID].eSynMode]);    
}
void AUD_Aproc_Test_Cmd(UINT8 u1DecID, UINT8 u1Cmd, UINT8 u1FrameNum) //For CLI test Aproc AVSync Commands.
{
    if (u1DecID > AUD_DEC_THIRD)
    {
        return;
    }

    // ([command] 0=wait_on  1=wait_off  2=wait_offon  3=skip)
    fgAVSyncTestMode[u1DecID] = TRUE;    
    if (u1Cmd == 0)         // wait on
    {
        AUD_Aproc_Chg_AVSyncCtrl(u1DecID, APROC_AVSYNC_CTRL_HOLD);
        vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecID)), AVSYNC_CMD_WAIT_ON);
        LOG(6,"Dec(%d) Test Wait!!\n", u1DecID);
    }
    else if (u1Cmd == 1)    // wait off
    {
        AUD_Aproc_Chg_AVSyncCtrl(u1DecID, APROC_AVSYNC_CTRL_WORK);    
        vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecID)), AVSYNC_CMD_WAIT_OFF);
        LOG(6,"Dec(%d) Test Wait OFF!!\n", u1DecID);
    }
    else if (u1Cmd == 2)    // wait off on
    {
        AUD_Aproc_Chg_AVSyncCtrl(u1DecID, APROC_AVSYNC_CTRL_WORK);
        vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecID)), AVSYNC_CMD_WAIT_OFFON);
        LOG(6,"Dec(%d) Test Wait OFF_ON!!\n", u1DecID);
    }    
    else if (u1Cmd == 3)    // skip
    {
        AUD_Aproc_Chg_AVSyncCtrl(u1DecID, APROC_AVSYNC_CTRL_WORK);
        vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecID)), AVSYNC_CMD_WAIT_OFF);

        LOG(6,"Dec(%d) Skip for %d Frames \n", u1DecID, u1FrameNum);
        DSP_SetSkipFrame(u1DecID, u1FrameNum);
    }    
    else
    {
        fgAVSyncTestMode[u1DecID] = FALSE;
    }
    
    
}

void AUD_Aproc_Aout_Routine(UINT8 u1DecID) //new_arm_pts_table
{
    LOG(5, "(Aproc_Sync)Aproc Aout Routine Dec(%d)\n", u1DecID);
    
    if (((_arAudDecoder[AUD_DSP0][u1DecID].eSynMode == AV_SYNC_AUDIO_MASTER)&&(_arAudDecoder[AUD_DSP0][u1DecID].eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA))
        ||(((_arAudDecoder[AUD_DSP0][u1DecID].eSynMode == AV_SYNC_SLAVE) || (_arAudDecoder[AUD_DSP0][u1DecID].eSynMode == AV_SYNC_AUDIO_MASTER))
            &&(_arAudDecoder[AUD_DSP0][u1DecID].eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)))
    {
        if (gsAprocSyncCtrl[u1DecID] == APROC_AVSYNC_CTRL_WORK)
        {
            AUD_Aproc_Chg_AVSyncCtrl(u1DecID, APROC_AVSYNC_CTRL_HOLD);
            vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecID)), AVSYNC_CMD_WAIT_ON);
            LOG(5, "(Aproc_Sync)Start HOLD for Dec(%d)\n", u1DecID);
        }
    }
	//Routine is processed at APROC DSP thread, it is common for all decoder. 
	//If background DEC send ER Routine cmd to APROC, it will affect foregournd DEC playing.
    //_vAprocSetRoutine (APROC_ROUTINE_ID_ER_SP_PATH);
    //_vAprocSetRoutine (APROC_ROUTINE_ID_ER_HP_PATH);
    
}

void AUD_Aproc_Aout_Ok(UINT8 u1DecID)
{
    LOG(5, "Aproc Aout OK for Dec(%d)\n", u1DecID);
    
    AUD_Aproc_Chg_AVSyncCtrl(u1DecID, APROC_AVSYNC_CTRL_WORK);
    vAprocReg_Write(APROC_ASM_ADDR(APROC_ASM_ID_AVSYNC_0,(APROC_REG_AVSYNC_CMD_SRC0+u1DecID)), AVSYNC_CMD_WAIT_OFF);
}
#endif //CC_AUD_ARM_RENDER

#endif //CC_AUD_ARM_SUPPORT

void AUD_SetAudCodecChange(BOOL fgAudCdcChg, AUD_FMT_T eDecFm)
{
    _fgAudCodecChang = fgAudCdcChg;
    _eCodecChangeFmt = eDecFm;

    LOG(0," ==> SWDMX Set Audio Codec Change Flag = %d Codec = %d!!!\n", fgAudCdcChg,_eCodecChangeFmt);
}

BOOL _AudCheckAputBankFreeze(UINT8 u1DecID)
{
    _u4AputBank = u4ReadD2RInfo_AputBank(AUD_DSP0, u1DecID);
    
    LOG(7," $$$ AputBank = 0x%x ; PrvAputBank = 0x%x \n",_u4AputBank,_u4PrvAputBank);
    
    if (_u4AputBank == _u4PrvAputBank)
    {
        _u4AputKeepCount++;
        
    }
    else
    {
        _u4PrvAputBank = _u4AputBank;
        _u4AputKeepCount = 0;
    }
    
    if (_u4AputKeepCount >= APUT_FREEZE_THRESHOLD)
        return TRUE;
    else
        return FALSE;
}
void _AudNotifyCodecChg(UINT8 u1DecID)
{
    AudDecNfyFct   pfAudDecNfy = NULL;

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, u1DecID, AUD_COND_AUD_CODEC_CHG,
            (UINT32)_eCodecChangeFmt, 0);
    }
    LOG(0," ===> Notify MW Audio Codec Change Format = %d \n",_eCodecChangeFmt);
}
