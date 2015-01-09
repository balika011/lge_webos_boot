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
 * $RCSfile: aud_drv.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_drv.h
 *  Brief of file aud_drv.h.
 *  Details of file aud_drv.h (optional).
 */

#ifndef _AUD_DRV_H_
#define _AUD_DRV_H_

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
//#include "x_aud_dec.h"
#include "aud_if.h"
#include "dmx_if.h"
#include "feeder_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

/* move thread priority and stack size setting to drv_common.h */
/*
#define AUD_DRV_THREAD_STACK_SIZE               2048
#define AUD_DRV_THREAD_PRIORITY            		101
*/
#define AUD_DRV_THREAD1_NAME                    "AudDrv1"
#define AUD_DRV_THREAD2_NAME                    "AudDrv2"
#ifdef CC_MT5391_AUD_3_DECODER
#define AUD_DRV_THREAD3_NAME                    "AudDrv3"
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
#define AUD_DRV_THREAD4_NAME                    "AudDrv4"
#endif
#define AUD_DRV_THREAD5_NAME                    "AudDrv5"
#define AUD_DRV_THREAD6_NAME                    "AudDrv6"
#define AUD_DRV_FEEDSTR_THREAD_NAME             "AudFeedStr"
#define AUD_APLL_MONITOR1_THREAD_NAME      	    "AudApllMon1"
#define AUD_APLL_MONITOR2_THREAD_NAME      	    "AudApllMon2" 
#ifdef CC_AUD_DDI
#define AUD_SOUND_BAR_THREAD_NAME      	        "SoundBar"
#endif
#define AUD_DSP_MONITOR_THREAD_NAME             "AudDspMon"  //  -- DSP Force --
#define AUD_DRV_FEEDER_THREAD                   "AudFeeder"
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
#define AUD_DRV_Parser_THREAD1                  "AudHDMIParser1"
#define AUD_DRV_Parser_THREAD2                  "AudHDMIParser2"
#endif
#ifdef CC_AUD_NCSTOOL_SUPPORT
#define AUD_TOOL_THREAD_NAME                    "AudTool"
#endif
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
#define AUD_DRV_FEEDMIXSND_THREAD_NAME          "AudFeedMixSnd"
#define AUD_DRV_ALSA_FEEDMIXSND_THREAD_NAME     "AudAlsaFeedMixSnd"
#endif

#ifdef CC_AUD_HP_DEPOP  ///For Remy, need remove "CC_DAC_DE_POP".
#define AUD_DRV_HP_DEPOP_THREAD                 "AudHpDePop"
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
#define AUD_DRV_DATA_UPLOAD_THREAD              "AudDataUpload"
#define AUD_DRV_DATA_BLUETOOTH_THREAD           "AudDataBluetooth"
//#endif

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
#define AUD_DRV_DATA_TRANSFER_THREAD            "AudDataTransfer"
#endif

#define AUD_DRV_CODEC_CHG_THREAD                "AudCdcChg"

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
#define AUD_DRV_PLAY_MUTE_THREAD1               "AudPlayMute1"
#define AUD_DRV_PLAY_MUTE_THREAD2               "AudPlayMute2"
#define AUD_DRV_PLAY_MUTE_THREAD3               "AudPlayMute3"
#endif

#ifndef CC_AUD_SMALL_PTS_CHECK
#define CC_AUD_SMALL_PTS_CHECK
#endif

#define APLL_ADJUST_BOUND1 2700        //WFD APLL Adjust
#define APLL_ADJUST_BOUND2 9000
#define APLL_ADJUST_BOUND3 18000
#define APLL_ADJUST_BOUND4 45000

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef enum _DECODER_STATE_T
{
    AUD_DEC_STOP = 0,
    AUD_DEC_PLAYING,
    AUD_DEC_PAUSE,
    AUD_DEC_INIT,
    AUD_DEC_ERROR
}   DECODER_STATE_T;

typedef struct _AUD_DECODER_T
{
    AUD_FMT_T    					eDecFormat;
    AUD_DEC_STREAM_FROM_T           eStreamFrom;
    MEM_BUFFER_INFO_T 		        rMemBuffer;
    AV_SYNC_MODE_T                  eSynMode;
    AUD_MM_MODE_T                   eMultimediaMode;
    AV_SYNC_STC_SELECT_T            eStcId;
    DECODER_STATE_T                 eDecState;
    BOOL                            fgAvSynLock;
    BOOL                            fgOperationMode; // FALSE: push mode TRUE:pull mode
    BOOL                            fgMMAoutReady;
    BOOL                            fgMMAoutEnable;
    UINT32                          u4EventFlag;
    DMX_AUDIO_PES_T                 rFirstReceivePes;
    DMX_AUDIO_PES_T                 rFirstAvaibleReceivePes;
    DMX_AUDIO_PES_T                 rFristDecodePes;
    DMX_AUDIO_PES_T                 rCurrentPes;
    UINT32                          u4StartPts;
    UINT32                          u4ReceivePesCount;
    UINT32                          u4ReceiveValidPesCount;
    UINT32                          u4PesCntAfterIPicReceived;
    UINT8                           u1DmxPidIdx;
    AUD_NFY_INFO_T                  rNfyInfo;
    AUD_PCM_SETTING_T               rPcmSetting;
    UINT32                          u4FifoStart;
    UINT32                          u4FifoEnd;
    UINT32                          u4FifoSz;
    BOOL                            fgRecord;
    BOOL                            fgIsAD;
    AUD_PATH_T                  ePath;
    BOOL                            fgInsrtStartPts;
} AUD_DECODER_T;

typedef struct
{
    UINT32       u4PtsWP;
    UINT64       u8PTS;         //for Gstreamer
    UINT32       u4PTSHigh;   //for Gstreamer
} AUD_MM_SYNC_INFO_T;
typedef struct
{
    UINT32       u4PtsWP;
    UINT32       u4Tick;
} AUD_MM_TICK_INFO_T;

typedef enum _AUD_DSP_CB_T
{
    AUD_DSP_CB_UNMUTE= 0,
    AUD_DSP_CB_NONE
} AUD_DSP_CB_T;
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

extern void AUD_DRVInit(void);
extern AUD_DRV_STATE_T AUD_DRVGetAudioState(UINT8 u1DspId, UINT8 ucDecId);
extern BOOL AUD_DRVCmd(UINT8 u1DspId, UINT8 ucDecId, AUD_DEC_CMD_T eCmd);
extern BOOL AUD_DRVSetDecodeType(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);
#ifdef CC_AUD_DDI
extern void AUD_DRVSetEncodeType(UINT8 u1DecId,  BOOL fgRecord);
extern BOOL AUD_DRVCheckDecAvailable(UINT8 u1DecId, BOOL fgForRec);
extern BOOL AUD_DRVCheckDecRecordFlag(UINT8 u1DecId);
extern void AUD_DRVSetADType(UINT8 u1DecId,  BOOL fgIsAD);
extern void AUD_DRVSetAudPath(UINT8 u1DecId, AUD_PATH_T ePath); 
#endif
extern BOOL AUD_DRVSetStreamFrom(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom);
extern BOOL AUD_DRVSetDecMemBuffer(UINT8 u1DspId, UINT8 ucDecId, const MEM_BUFFER_INFO_T * prDecMemBuffer);
#ifdef CC_AUD_MULTI_BUFFER_MEMORY_PLAY
extern void AUD_DRVMultiBufferInit(void);
extern void AUD_DRVMultiBufferReset(void);
extern void AUD_DRVMultiBufferMode(BOOL fgMultiBuff);
extern BOOL AUD_DRVMultiBufferAddData(UINT32 u4BufferPointer, UINT32 u4BufferSize, BOOL fgEndOfData);
extern UINT32 AUD_DRVGetMaxBufferNumber(void);
extern UINT32 AUD_DRVGetRemainingBuffferNumber(void);
#endif

extern BOOL AUD_DRVChangeDecodeFormat(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);
//extern BOOL AUD_SendPts(UCHAR ucChannel, UCHAR ucDevId, const PSR_AUDIO_PES_T * prPes);
extern AV_SYNC_MODE_T AUD_GetAvSynMode(UCHAR ucDecId);
extern void AUD_SetAvSynMode(UCHAR ucDecId, AV_SYNC_MODE_T eSynMode);
extern AUD_MM_MODE_T AUD_DrvGetMultimediaMode(UINT8 u1DecId);
extern void AUD_DrvSetMultimediaMode(UINT8 u1DecId, AUD_MM_MODE_T eMultimediaMode);
extern void AUD_DrvSetAdMode(BOOL fgAdMode);
#if 0   // Unused
extern BOOL AUD_AvSynLock(UCHAR ucDecId); // Check if AV is syn
#endif
extern void AUD_SetInputMode(UCHAR ucDecId, BOOL fgMode);
extern BOOL AUD_GetInputMode(UCHAR ucDecId);// FALSE: push mode TRUE:pull mode
extern AV_SYNC_STC_SELECT_T AUD_GetStcId(UCHAR ucDecId);
extern void AUD_SetStcId(UCHAR ucDecId, AV_SYNC_STC_SELECT_T eStcId);
extern BOOL AUD_GetOperationMode(UCHAR ucDecId); // FALSE: push mode TRUE:pull mode
extern void AUD_SetOperationMode(UCHAR ucDecId, BOOL fgMode); // FALSE: push mode TRUE:pull mode
//extern void AUD_GetFisrtAudioDecodePes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes);
extern void AUD_GetDecoderStatus(UINT8 u1DspId, UINT8 u1DecId, AUD_DECODER_T *prAudDec);
extern BOOL AUD_DRVGetDecodeType(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T * peStreamFrom,
	                             AUD_FMT_T * peDecType);
extern BOOL AUD_DRVGetStreamFrom(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T * peStreamFrom);
extern BOOL AUD_DRVGetHDMIDecodeType(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T * peStreamFrom, AUD_FMT_T * peDecType);
//extern void AUD_GetFisrtAudioDecodePes(UINT8 u1DecId, PSR_AUDIO_PES_T * prAudioPes);
#if 0   // Unused
extern void AUD_DrvSetNfy(UINT8 u1DspId, UINT8 u1DecId, const AUD_NFY_INFO_T * prAudNfyInfo);
extern void AUD_DrvGetNfy(UINT8 u1DspId, UINT8 u1DecId, AUD_NFY_INFO_T * prAudNfyInfo);
#endif
extern void _AUD_AtvAvInitialSync(BOOL fgStable);
extern void _AUD_AtvAvInitialSync1(BOOL fgStable);
extern void AUD_ResetDspReadPtr(UINT8 u1DspId, UINT8 u1DecId);
extern BOOL AUD_IsSpdifInSrc(UINT8 u1DecId);
extern void AUD_DetectionSyncSema(BOOL fgLock);
extern void AUD_StopMemStr(void);
extern void AUD_AudioPTSDelayEnable(UINT8 u1DecId, UINT8 uDlyFactor);
extern BOOL AUD_DrvFeederSetRequestInfo(FEEDER_TRANSMIT *prTransmit);
#if 0   // Unused
extern void AUD_ScartOutInit(void);
#endif
extern void AUD_AtunerSigStable(void);
extern void AUD_DrvPcmSetting(UINT8 u1DspId, UINT8 u1DecId, const AUD_PCM_SETTING_T * prPcmSetting);
#ifdef CC_AUD_SUPPORT_CLIP_NOTIFY_FUNC
extern void AUD_DrvSetNotifyFunc(UINT8 u1DecId, AUD_CLIP_NOTIFY pfClipNotify);
#endif
#if 0   // Unused
extern BOOL AUD_DrvDtvLock(UINT8 u1DecId);
#endif
#ifdef CC_AUD_BITRATE_CHG_NOTIFY
extern BOOL _AUD_SetBitRateChangedPts(UINT32 u4Pts);
extern BOOL _AUD_GetBitRateChangedPts(UINT32 *pu4Pts);
#endif

extern void AUD_DrvATunerLock(void);
extern void AUD_DrvSetAvSynMode(UINT8 u1DspId, UINT8 u1DecId, AV_SYNC_MODE_T eSynMode);
extern void AUD_DrvSetPIPVideoPath(UINT8 u1DecId, UINT8 u1VideoPath);
extern void _AudSetTvSysMask(UINT8 u1DecId, UINT32 u4Mask);
extern UINT32 _AudGetTvSysMask(UINT8 u1DecId);
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
extern void _AudSetTvAudSysMask(UINT8 u1DecId, UINT32 u4TvMask, UINT32 u4TvAudMask, ISO_3166_COUNT_T country_code);
#endif
#ifdef CC_AUD_DDI
extern AUD_FMT_T _AudAtvFmtDetection(UINT8 u1ENCDecId);
#else
extern AUD_FMT_T _AudAtvFmtDetection(void);
#endif
extern void  AUD_DspHdmiChangeFormatMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern void AUD_SendApllAdjustCmd(UINT32 u4Level);
#ifdef CC_AUD_DDI
extern void AUD_SendSoundBarCmd(UINT32 u4Volume);
#endif
extern void AUD_MMQueueSyncInfo(UINT8 u1DecId, UINT64 u8PTS, UINT32 u4PTSWp, BOOL fgReset);
#ifdef CC_ENABLE_AOMX
extern void GST_MMQueueSyncInfo(UINT8 u1DecId, UINT64 u8PTS, UINT32 u4PTSWp, BOOL fgReset, UINT32 u4PTSHigh);
#endif
extern void AUD_MMFindSyncInfo(UINT8 u1DecId, UINT64* u8PTS, UINT32 u4PTSWp);
#ifdef CC_ENABLE_AOMX
extern BOOL GST_MMFindSyncInfo(UINT8 u1DecId, UINT64* u8PTS, UINT32 u4PTSWp, UINT32* u4PTSHigh);
#endif
#ifdef TIME_SHIFT_SUPPORT
extern void AUD_MMQueueTickInfo(UINT8 u1DecId, UINT32 u4Tick, UINT32 u4PTSWp, BOOL fgReset);
#endif
extern void AUD_DrvMMAoutEnable(UINT8 u1DecId, BOOL fgEnable);
#ifndef CC_AUD_SKYPE_SUPPORT
extern void AUD_DrvMMAoutEnable_Flag(BOOL fgEnable);
extern void AUD_DrvMMAoutReady_Flag(BOOL fgEnable);
#else
extern void AUD_DrvMMAoutEnable_Flag(UINT8 u1DecId, BOOL fgEnable);
extern void AUD_DrvMMAoutReady_Flag(UINT8 u1DecId, BOOL fgEnable);
#endif
void AUD_SentReaptNumber(UINT8 u1DecId, UINT32 ReaptNumber);
extern BOOL AUD_IsMMAVInitSync(UINT8 u1DecId);
extern BOOL AUD_IsMMAoutEnable(UINT8 u1DecId);
#if 0   // Unused
extern BOOL AUD_IsMMAoutReady(UINT8 u1DecId);
#endif
extern void _AudClearEOSFlag(UINT8 u1DecId);
extern void AUD_DrvClearPesCnt(UINT8 u1DecId);
extern AUD_DEC_STREAM_FROM_T _AudGetStrSource(UINT8 u1DecId);
#ifdef CC_AUD_LINEIN_USE_DEC3
extern BOOL _AudGetADType(UINT8 u1DecId);
#endif
extern AUD_DECODER_T * AUD_DrvGetDecodeState(UINT8 u1DspId, UINT8 u1DecId);
extern void AUD_DrvMMAoutEnableNotify(UINT8 u1DecId, BOOL fgResume);
extern void AUD_DrvMMNoDataNotify(UINT8 u1DecId);

extern void _AUD_DspStopMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern AUD_FMT_T _AudGetStrFormat(UINT8 u1DecId);
#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
extern void _AudSrmStableCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);
#endif

extern void AUD_DspDecPowerOffMute(UINT8 u1DecId, BOOL fgEnable); 
extern void AUD_DspDecPlayMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern UINT8 AUD_DrvGetPIPVideoPath(void);

extern void AUD_SET_SWDMXSrcId(UINT8 u1DecId, UINT8 u1SwdmxSrcId);
extern void AUD_GET_SWDMXSrcId(UINT8 u1DecId, UINT8* u1SwdmxSrcId);
#ifdef CC_ENABLE_AV_SYNC
extern void AUD_DrvCallback(UINT8 u1DecId, AUD_DSP_CB_T eAudDspCbType);
#endif
extern void AUD_GetAudioPts(UINT8 u1DecId, UINT32 * prAudioPts);
extern void AUD_DrvGetAudioTrickPts(UINT8 u1DecId, UINT32 *prAudioPts, BOOL fgOldPts);
#ifdef CC_SUPPORT_MUXER
extern void AUD_SetMuxId(UINT8 u1MuxId);
#endif
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
extern void _AUD_DspDataUploadNotify (UINT32 u4Index);
//#endif
#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
extern void _AUD_DspDataBluetoothNotify (UINT32 u4Index);
#endif
#if 0   // Unused
extern BOOL AUD_GetAdPanEnable(void);
extern BOOL AUD_GetAdFadeEnable(void);
#endif
extern void AUD_DrvCheckSpeedTime(UINT8 u1DecId);

extern void AUD_DrvThreadWakeup(UINT8 u1DspId, UINT8 u1DecId);
extern void AUD_DrvSuspend(void);
extern void AUD_DrvLogStcDiff(UINT8 u1DecId);

extern void AUD_SetAudCodecChange(BOOL fgAudCdcChg, AUD_FMT_T eDecFm);
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER

typedef struct
{
	UINT8  u1InitFlag;
	HANDLE_T hSemaAudTransLock;
	UINT8* pu1AudTransBuf;
	UINT32 u4AudTransWritePtr;
	UINT32 u4AudTransReadPtr;
	UINT32 u4AudTransReadPtr1;
}AUD_DATA_TRANSFER_T;

extern void _AudDataTransferInit(void);
extern void _AudDataTransferUninit(void);
extern UINT32 _AudDataTransferGetStartAddr(void);
extern UINT32 _AudDataTransferGetEndAddr(void);
extern UINT32 _AudDataTransferGetSize(void);

extern void _AudDataTransferSetWritePtr (UINT32 u4WP);
extern UINT32 _AudDataTransferGetReadPtr (void);
extern UINT32 _AudDataTransferSetReadPtr (UINT32 u4RP);
extern UINT32 _AudDataTransferGetFreeSize (void);
extern UINT32 _AudDataTransferGetDataSize (void);

extern void _AudDataTransferUnlock (void);
extern void _AudDataTransferLock(void);
#endif

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
extern BOOL AUD_PlayMuteCmd(UINT8 u1DecID, AUD_PLAY_MUTE_CMD_T eCmd, UINT16 u2Arg);
extern void AUD_PlayMuteCmdQuery(UINT8 u1DecID);
extern void AUD_PlayMuteClearHistory(UINT8 u1DecID);
extern void AUD_PlayMuteQueryDelayInfo(UINT8 u1DecID);
extern void AUD_PlayMuteEnable(BOOL fgEnable);
#endif
extern void AUD_SetAudControlVdpQuota(UINT8 u1DecID, UINT16 u2Quota);  //DTV_AVSYNC_ENH
extern void AUD_WatchSpdifEnable(BOOL fgEnable);

extern void AUD_DrvPtsQueueInit(void); //PTSQueue_V2
extern void AUD_DrvPtsQueueInsert(UINT8 u1DecID, UINT32 u4Wp, UINT32 u4PTS);
extern void AUD_DrvPtsQueueUpdateWp(UINT8 u1DecID, UINT32 u4Wp);
extern void AUD_DrvPtsQueueReset(UINT8 u1DecID);
extern void AUD_DrvPtsQueueQry(UINT8 u1DecID, UINT16 u2Size);
extern INT16 AUD_DrvPtsQueueFindInfo(UINT8 u1DecID, UINT32 u4Wp);
#ifdef CC_AUD_DDI
extern void _AUD_CheckPvrPause(INT32 i4Speed);
extern void _AUD_SetDelayMode(BOOL fgEnable);
#endif
extern BOOL AUD_IsSourceFromWFD(UINT8 u1DecId); 
#ifdef AUD_DECODER3_SELF_TEST
extern UINT8 u1ForceDecId;
extern AV_SYNC_MODE_T eForceSyncMode;
#endif

#ifdef CC_S_SUBWOFFER_SUPPORT
extern void _MT5398_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
#endif
#ifdef CC_AUD_ARM_SUPPORT
#ifdef CC_AUD_ARM_RENDER
extern void AUD_Aproc_Qry_Sync(UINT8 u1DecID);
extern void AUD_Aproc_Aout_Routine(UINT8 u1DecID);
extern void AUD_Aproc_Aout_Ok(UINT8 u1DecID);
#endif
extern BOOL AUD_Aproc_Skip_Pts(UINT8 u1DecID, UINT32 u4Pts, BOOL fgReset);
extern void AUD_Aproc_Reset_AVSyncInfo(UINT8 u1DecID);
#endif //CC_AUD_ARM_SUPPORT
//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
#if defined (CC_AUD_MP3ENC_FAKEDATA_SUPPORT) && defined (CONFIG_HIGH_RES_TIMERS)
extern VOID AUD_InformMuxerFake(void);
extern BOOL AUD_GetEncDataStatus(void);
extern VOID AUD_CreateTimer(UINT32 u4period);
extern VOID AUD_DeleteTimer(void);
extern VOID AUD_StartTimer(UINT32 u4delay);
extern VOID AUD_StopTimer(void);
#endif


#endif /* _AUD_DRV_H_ */

