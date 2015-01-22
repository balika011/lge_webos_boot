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
 * $Date: 2015/01/22 $
 * $RCSfile: aud_dsp_cfg.h,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_dsp_cfg.h
 *  Brief of file aud_dsp_cfg.h.
 *  Details of file aud_dsp_cfg.h (optional).
 */

#ifndef _AUD_DSP_CFG_H_
#define _AUD_DSP_CFG_H_

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "aud_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
// Volume
#define MAX_VOL_LEVEL               (100)
#define VOL_ZERO_BASE               (0)

#define MAX_VOL_SHM					(0xFE2F5)		// 18dB

// Virutal surround constant
#define VSURR_GAIN_MAX  16

/*
// For equalizer preset
#define AUD_EQ_TYPE_NONE  0
#define AUD_EQ_ROCK       1
#define AUD_EQ_POP        2
#define AUD_EQ_LIVE       3
#define AUD_EQ_DANCE      4
#define AUD_EQ_TECHNO     5
#define AUD_EQ_CLASSIC    6
#define AUD_EQ_SOFT       7
#define AUD_EQ_TYPE_MAX   9
*/

#define AUD_EQ_BAND_1       1

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
#define CH_AEQ_BAND_NO  5
#define AUD_AEQ_IIR_BAND_NUM    5
#define AUD_AEQ_BAND_LAST   5
#endif

#ifdef CC_AUD_7BAND_EQUALIZER
#define CH_EQ_BAND_NO   7
#define AUD_EQ_IIR_BAND_NUM     7
#define AUD_EQ_BAND_LAST       7
#else
#define CH_EQ_BAND_NO   5
#define AUD_EQ_IIR_BAND_NUM     8
#define AUD_EQ_BAND_LAST       5
#endif

#define EQ_BAND_DRY   0
#ifdef CC_AUD_7BAND_EQUALIZER
#define EQ_BAND_MAX   (7+1)
#else
#define EQ_BAND_MAX   (5+1)
#endif

#define EQ_BAND_GAIN_DB_MAX             14

#define EQ_BAND_GAIN_INDEX_MAX          (((EQ_BAND_GAIN_DB_MAX*2)*2)+1) // range -14 ~ +14 dB, 0.5 dB/step
#define EQ_BAND_GAIN_SIDE_INDEX_MAX     (EQ_BAND_GAIN_INDEX_MAX >> 1)
#define EQ_BAND_GAIN_MID                (EQ_BAND_GAIN_INDEX_MAX >> 1)

#define EQ_DRY_GAIN_INDEX_MAX           (((10*2)*2)+1)  // range -10 ~ +10 dB, 0.5 dB/step
#define EQ_DRY_GAIN_SIDE_INDEX_MAX     (EQ_DRY_GAIN_INDEX_MAX >> 1)
#define EQ_DRY_GAIN_MID                 (EQ_DRY_GAIN_INDEX_MAX >> 1)

#define MID_BASS_TREBLE_OFFSET    (10)
// Reverb constant
#define AUD_REVERB_MODE_OFF     (0)
#define AUD_REVERB_MODE_MAX     (7)

#define DRC_RANGE_MAX (9)

#define BYPASS_BASS_MANAGEMENT                                 (0x1 << 12)
#define BYPASS_POST_PROCESSING                                 (0x1 << 13)
#define BYPASS_CHANNEL_VOLUME                                  (0x1 << 14)
#define BYPASS_DELAY                                           (0x1 << 15)
#define LFE_INPUT_GAIN_0_DB                                    (0x1 << 6)

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
#if 0   // Unused
extern  AUD_IEC_T _GetIECFlag(UINT8 u1DecId);
#endif
extern BOOL _GetVolMuteStatus(UINT8 bPath);
extern void _AUD_DspCfgSetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt);
extern void _AUD_DspFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4ShmValue);
extern UINT32 _AUD_DspGetFineTuneVolume(UINT8 u1DecId, AUD_CH_T eChannel);
extern void _AUD_DspVolumeInit(void);
extern void _AUD_DspEqCfgInit(void);
#ifdef CC_AUD_SOUND_MODE
extern void _AUD_DspPSMCfgInit(void);
extern void _AUD_DspPostSoundModeChange(UINT8 u1DecId, UINT8 u1ModeIdx);
#endif
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
extern void _AUD_DspVSMCfgInit(void);
#endif
#ifdef	CC_SONY_2K14_FLASH_AQ
extern void _AUD_DspSetTvPosition(UINT8 u1DecId,UINT8 u1Position,BOOL fgWithMute);
#endif
extern void _AUD_DspPostSoundMode(UINT8 u1DecId, UINT8 u1ModeIdx, BOOL fgWithMute);
extern void _AUD_DspPostSoundModeStart(UINT8 u1DecId, BOOL fgStart);
extern void _AUD_DspSetPEQ(UINT8 u1DecID, UINT8 u1ModeIdx);
#ifdef CC_AUD_PEQ_BASS_TREBLE
extern void _AUD_DspPEQBassTreble(UINT8 u1DecId,BOOL isTreble,UINT8 u1Level);
#endif

extern void _AUD_DspClearSoundEnable(UINT8 u1DecId, BOOL fgEnable);
#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
extern void _AUD_DspClearSoundCfg(UINT8 u1UserQIdx, UINT16 u2UserFc);
#endif
#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
extern void _AUD_DspSetRegDetection(BOOL fgRegDetection);
extern BOOL _AUD_DspGetRegDetection(void);
#endif
extern void _AUD_DspChDelayInit(void);
#ifdef CC_AUD_SPIDF_LVL
extern void _AUD_DspSpdifLevel(UINT8 u1Level);
#endif
extern void _AUD_DspSpdifInit(void);
#ifdef CC_MT5881
extern void _AUD_SpdifinInit(void);
#endif
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
extern void _AUD_AprocBMangInit(void);
#endif
extern void _AUD_DspAsrcInit(void);
extern void _AUD_DspReverbEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspReverbMode(UINT8 u1DecId, UINT8 u1Mode) ;
extern void _AUD_DspChannelEqEnable(UINT8 u1DecId, BOOL fgEnable) ;
extern BOOL _AUD_DspGetChannelEqEnable(UINT8 u1DecId);
extern void _AUD_DspSpkSizeCfg(UINT8 u1DecId, UINT32 u4SpkSize);
#ifdef CC_S_SUBWOFFER_SUPPORT  ///CC_BASS_MANGER_INDEP_LPF
extern void _AUD_DspSpkSizeLpfCfg(UINT8 u1DecId, UINT32 u4SpkSize);
#endif
extern void _AUD_DspBassTrebleCutOffFreq(UINT8 u1DecId,UINT8 u1LowFc, UINT8 u1HighFc);
extern UINT32 _AUD_DspBassCutOffFreqQry(UINT8 u1DecId);
extern UINT32 _AUD_DspTrebleCutOffFreqQry(UINT8 u1DecId);
extern void _AUD_DspBassBoostEnable(UINT8 u1DecId, BOOL fgEnable) ;
extern BOOL _AUD_DspGetBassBoostEnable(UINT8 u1DecId);
extern void _AUD_DspBassBoostGain(UINT8 u1DecId, UINT8 u1Level);
extern UINT32 _AUD_DspGetBassBoostGain(UINT8 u1DecId);
extern void _AUD_DspBassBoostGain(UINT8 u1DecId, UINT8 u1Level);
extern void _AUD_DspSuperBassEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspClearBoostEnable(UINT8 u1DecId, BOOL fgEnable);
extern BOOL _AUD_DspGetClearBoostEnable(UINT8 u1DecId);
extern void _AUD_DspClearBoostGain(UINT8 u1DecId, UINT8 u1Level);
extern UINT32 _AUD_DspGetClearBoostGain(UINT8 u1DecId);
#ifdef CC_AUD_FY12_LOUDNESS
extern void _AUD_DspBassBoostEnable2(UINT8 u1DecId, BOOL fgEnable);
extern BOOL _AUD_DspGetBassBoostEnable2(UINT8 u1DecId);
extern void _AUD_DspBassBoostGain2(UINT8 u1DecId, UINT8 u1Level);
extern UINT32 _AUD_DspGetBassBoostGain2(UINT8 u1DecId);
extern void _AUD_DspSuperBassEnable2(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspClearBoostEnable2(UINT8 u1DecId, BOOL fgEnable);
extern BOOL _AUD_DspGetClearBoostEnable2(UINT8 u1DecId);
extern void _AUD_DspClearBoostGain2(UINT8 u1DecId, UINT8 u1Level);
extern UINT32 _AUD_DspGetClearBoostGain2(UINT8 u1DecId);
#endif
extern void _AUD_DspEqEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspChEqBand(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify) ;
extern void _AUD_DspChEqLoadPreset(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType) ;
extern void _AUD_DspGetEqType(UINT8 u1DecId, AUD_EQ_TYPE_T* eEqType);
#ifdef CC_AUD_ARM_RENDER
extern void _AUD_DspChEqBandMode(UINT8 u1DecId, UINT8 u1BandIdx, INT8 i1Level);
#else
extern void _AUD_DspChEqBandMode(UINT8 u1DecId, UINT8 u1BandIdx, UINT8 u1Level, BOOL fgNotify) ;
#endif
extern void _AUD_DspChEqLoadPresetChgMode(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType) ;
extern void _AUD_DspSetLimiterMode(UINT8 u1Mode);
extern void _AUD_DspSetLimiterFlag(UINT8 u1Flag);
extern void _AUD_DspSetLimiterThreshold(UINT32 u4Thre);
#ifdef CC_AUD_DRC_V20
extern void _AUD_DspSetLimiterAttackRate(UINT32 u4AttackRate);
extern void _AUD_DspSetLimiterReleaseRate(UINT32 u4ReleaseRate);
extern void _AUD_DspSetLimiterGainRatio(UINT32 u4Ratio);
#endif
extern void _AUD_DspGetLimiterConfig(UINT8 *pu1Mode, UINT16 *pu2Flag, UINT32 *pu4Thre);
#ifdef CC_AUD_DRC_V20
extern void _AUD_DspGetLimiterConfig1(UINT32 *pu4Attack, UINT32 *pu4Release, UINT32 *pu4Ratio);
#endif
extern void _AUD_DspPL2Config(UINT8 uCtrl, UINT8 uMode);
extern void _AUD_DspPL2ConfigInit(UINT16 u2PL2cfg,UINT16 u2PL2Mode) ;
extern void _AUD_DspAC3KaraMode(UINT16 u2KaraMode,UINT8 uDecIndx) ;
extern UINT16 _AUD_DspGetAC3KaraMode(UINT8 uDecIndx) ;
extern void _AUD_DspAC3DualMono(UINT16 u2DMMode,UINT8 uDecIndx) ;
extern UINT16 _AUD_DspGetAC3DualMono(UINT8 uDecIndx) ;
extern void _AUD_DspAC3CompMode(UINT16 u2CompMode,UINT8 uDecIndx) ;
extern UINT16 _AUD_DspGetAC3CompMode(UINT8 uDecIndx) ;
extern void _AUD_DspAC3DRCRange100(UINT8 uDRCLevel,UINT8 u1DecId);
extern void _AUD_DspAC3DRCRange(UINT8 uDRCLevel,UINT8 u1DecId);
#if 0   // Unused
extern UINT8 _AUD_DspGetAC3DRCRange(UINT8 u1DecId);
#endif
extern UINT8 _AUD_DspGetAC3DRCRange100(UINT8 u1DecId);
extern void _Aud_G726DecCfg(UINT8 u1DecId, UINT8 u1rate);

#ifdef CC_AUD_SUPPORT_MS10
extern void _AUD_DspSetDDCControl (UINT16 u2Control, UINT8 uDecIndx);
extern void _AUD_DspSetDDCStrmId (UINT8 u1StrmId, UINT8 uDecIndx);
extern void _AUD_DspSetDDCMixer (UINT8 uMixer, UINT8 uDecIndx);
extern void _AUD_DspSetDDCAdEnable (UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspDdtDmxRfMode (UINT8 u1Mode, UINT8 u1DecId, BOOL bSetDmx);
extern void _AUD_DspDdtDualMode (UINT8 u1Mode, UINT8 u1DecId);
extern void _AUD_DspDdtCompValRange (UINT8 u1Mode, UINT8 u1DecId);
#endif
extern UINT8 _AUD_DspGetDdtDmxRfMode (UINT8 u1DecId,BOOL bGetDmx);
extern UINT8 _AUD_DspGetDdtDualMode (UINT8 u1DecId);
extern UINT8 _AUD_DspGetDdtDrcRange (UINT8 u1DecId);
extern void _AUD_DspSpeakerConfig(UINT32 u4SpkCfg,UINT8 uDecIndex);
extern UINT32 _AUD_DspGetSpeakerConfig(UINT8 uDecIndex);
extern void _AUD_DspSpeakerSizeConfig(UINT8 uDecIndex, BOOL fgFrontLarge, BOOL fgSurroundLarge, BOOL fgCenterLarge);
extern void _AUD_DspSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8 u1FrontPair, UINT8 u1Center, UINT8 u1Surround, UINT8 u1CH910);
extern void _AUD_DspGetSpeakerLargeSmallConfig(UINT8 u1DecIndex, UINT8* u1FrontPair, UINT8* u1Center, UINT8* u1Surround, UINT8* u1CH910);
extern void _AUD_DspSpeakerLsConfig(UINT8 u1DecIndex, UINT8 u1L, UINT8 u1R, UINT8 u1Ls, UINT8 u1Rs, UINT8 u1C);
extern void _AUD_DspSpeakerOutputConfig(UINT8 u1DecIndex, UINT8 u1SpkCfg);
extern UINT8 _AUD_DspGetSpeakerOutputConfig(UINT8 u1DecIndex);
extern void _AUD_DspSpeakerSubwooferEnable(UINT8 u1DecIndex, BOOL fgEnable);
extern BOOL _AUD_DspGetSpeakerSubwooferEnable(UINT8 u1DecIndex);
extern void _AUD_DspChannelDelayValue(INT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);
extern void _AUD_DspChannelDelayAP(AUD_CH_T eChIndex, UINT8 uDecIndex);
extern void _AUD_DspChannelDelay(UINT8 u1DspId, UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);
extern void _AUD_DspChannelDelayNoUop(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);
extern UINT16 _AUD_DspGetChannelDelay(UINT8 u1DspId, AUD_CH_T eChIndex);
extern void _AUD_DspChannelDelay_initial(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);
extern void _AUD_DspInputChannelDelay(AUD_DEC_STREAM_FROM_T eStreamFrom);
extern UINT8 _AUD_DspGetChannelVolume(UINT8 u1DecId, AUD_CH_T eChannel);
extern void _AUD_DspChannelVolume(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value);
extern void _AUD_DspChannelVolumeNew(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value, UINT8 u1Value1);
#ifdef CC_AUD_DDI
extern void _AUD_DspBluetoothVolume(UINT8 u1DecId, UINT8 u1Value, UINT8 u1Value1);
#endif
#ifdef CC_AUD_VOLUME_OFFSET
extern void _AUD_DspVolumeOffset(UINT8 u1DecId, UINT8 u1Index);
extern void _AUD_DspHpVolumeOffset(UINT8 u1DecId, UINT8 u1Index);
extern void _AUD_DspLineOutVolOffset( UINT8 u1DecId, UINT8 u1Value);
#ifdef CC_S_SUBWOFFER_SUPPORT
extern void _AUD_DspSWVolumeOffset( UINT8 u1DecId, UINT8 u1Value);
#endif
#endif
#ifdef CC_AUD_SX1_FEATURE
extern void _AUD_DspAdVolOffset(UINT8 u1Value);
#endif
extern void _AUD_DspPanFadePesUpdateEn(BOOL bEnable);
extern void _AUD_DspAdPanFadeEnable(BOOL bEnable);
extern UINT8 _AUD_DspGetAdPanFadeSetting(void);
extern void _AUD_DspAdFadeVolumeSet(UINT8 u1Value);
extern void _AUD_DspAdFadeVolumeUpdate(UINT8 u1DecId, AUD_CH_T eChannel);
extern void _AUD_DspAdPanVolumeSet(UINT8 u1DecId, AUD_CH_T eChannel, UINT8 u1Value);
extern void _AUD_DspAdPanVolumeUpdate(UINT8 u1DecId, AUD_CH_T eChannel);
extern void _AUD_DspChannelVolShmRawValue(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4VolShm);
extern void _AUD_DspChannelMute(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgMute);
extern void _AUD_DspChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value);
extern void _AUD_DspChannelVolDolbyGainTB11(UINT8 u1DecId, AUD_CH_T eChannel, INT32 i4Value);
extern void _AUD_DspDolbyGainTB11(UINT8 u1DecId);
extern INT32 _AUD_DspGetChannelVolGain(UINT8 u1DecId, AUD_CH_T eChannel);
extern INT32 _AUD_DspGetChannelVolGainNonLineIn(UINT8 u1DecId, AUD_CH_T eChannel);

extern void _AUD_DspChannelOutPort(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgEnable);
extern void _AUD_DspBalance(UINT8 u1DecId, UINT8 u1Balance);
extern void _AUD_DspMasterVolume(UINT8 u1DecId, UINT8 u1Volume);
extern UINT8 _AUD_DspGetMasterVolume(UINT8 u1DecId);
extern void _AUD_DspMixSndVolume(UINT8 u1DecId, UINT8 u1Volume);
extern void _AUD_DspMasterVolShmRawValue(UINT8 u1DecId, UINT32 u4VolShm);
extern void _AUD_DspAVCEnable(UINT8 u1DecId, BOOL fgEnable);
#ifdef CC_AUD_AVC_V20
extern void _AUD_DspChangeAVCPara (AUD_DRC_V20_INFO_T eType, UINT32 u4Value, UINT8 u1DecId);
extern void _AUD_DspGetAVCPara(AUD_DRC_V20_INFO_T eType, UINT32* u4Value, UINT8 u1DecId);
#else
extern void _AUD_DspChangeAVCPara(UINT16 u2Type, UINT16 u2Value, UINT8 u1DecId);
extern void _AUD_DspChange2BandAVCPara(UINT8 u1Band, UINT16 u2Type, UINT16 u2Value, UINT8 u1DecId);
extern void _AUD_DspGetAVCPara(UINT16 u2Type, UINT16* u2Value, UINT8 u1DecId);
#endif

#ifdef CC_AUD_DRC_V20
extern void _AUD_DspSetDrc(AUD_DRC_V20_INFO_T eType, UINT32 u4Value, UINT8 u1DecId);
extern void _AUD_DspGetDrc(AUD_DRC_V20_INFO_T eType, UINT32 *u4Value, UINT8 u1DecId);
#endif
extern void _AUD_DspInputChangeAVCPara(AUD_DEC_STREAM_FROM_T eStreamFrom);
extern void _AUD_DspMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspMuteAllEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspStopMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspHdmiModeMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern void  AUD_DspHdmiChangeFormatMuteEnable(UINT8 u1DecId, BOOL fgEnable);
#ifdef CC_AUD_HDMI_SPDIF_CFG
extern void AUD_DspHdmiMuteAnalogOutput(UINT8 u1DecId, BOOL fgEnable);
#endif
#ifdef CC_AUD_EFFECT_MUTE_PROTECT
extern void _AUD_DspSurroundMute(UINT8 u1DecId, AUD_CH_T eChannel, BOOL fgMute); 
extern void _AUD_DspVBassMute(AUD_CH_T eChannel, BOOL fgMute); 
#endif
extern void _AUD_DspBassMngEnalbe(UINT8 u1DspId, UINT8 u1DecId, BOOL fgEnable);
extern BOOL _AUD_DspGetBassMngEnable(UINT8 u1DspId, UINT8 u1DecId);
extern BOOL _AUD_DspGetChannelVolumeEnable(UINT8 u1DspId, UINT8 u1DecId);
extern BOOL _AUD_DspGetDelayEnable(UINT8 u1DspId, UINT8 u1DecId);
#ifdef CC_AUD_DDI
extern void _AUD_DspSetDelayEnable(UINT8 u1DspId, UINT8 u1DecId, BOOL fgEnable);
#endif
extern BOOL _AUD_DspGetPostProcessingEnable(UINT8 u1DspId, UINT8 u1DecId);
extern void _AUD_DspProcMode(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2Mode);
#if !defined(CC_AUD_NEW_POST_MULTI_VS_SUPPORT) && !defined(CC_AUD_USE_NVM)
extern void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, BOOL fgEnable);
#else
extern void _AUD_DspVirtualSurroundEnalbe(UINT8 u1DecId, UINT8 u1ModeIdx);
extern void _AUD_DspVSModeChange(UINT8 u1DecId, UINT8 u1ModeIdx);
#endif
extern BOOL _AUD_DspGetVirtualSurroundEnalbe(UINT8 u1DecId);
#ifdef CC_SET_VOLUME
extern void _AUD_DspSetVolTable(UINT8 u1Idx, UINT32 u4Vol);
#endif
extern void _AUD_DspMonoDownMix(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspSetMatrixOutMode(UINT8 u1DecId, AUD_MATRIX_OUT_T eMode);
extern UINT8 _AUD_DspGetMatrixOutMode(UINT8 u1DecId);
extern void _AUD_DspAutoDmx(UINT8 u1DecId, BOOL fgEnable);
extern BOOL _AUD_GetDspAutoDmx (UINT8 u1DecId);
extern void _AUD_DspBypassPostProcess(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2BypassMode);
extern void _AUD_DspSoundEffectFlag(UINT8 u1DecId, BOOL fgFlag);
extern BOOL _AUD_DspIECConfig(AUD_IEC_T eIecCfg, BOOL fgEnable);
extern BOOL _AUD_GetDspIECConfig(void);
extern BOOL _AUD_IsIECRaw(void);
extern void _AUD_DspSetIEC(UINT8 u1DecId);
extern BOOL _AUD_SetSPDIFEnable(BOOL fgEnable);
extern BOOL _AUD_DspIecChannel(AUD_IEC_CH_T eIecChannel);
extern void _AUD_DspSetIecRawFlag (UINT8 u1Flag);
extern void _AUD_DspSetIecRawDelay (INT16 i2Delay);
extern void _AUD_DspResetSpdifReg (SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom);
#ifndef CC_AUD_SUPPORT_SPDIF_V20
extern void _AUD_DspSetSpdifRegType (SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom);
#endif
extern void _AUD_DspSetSpdifChannelStatus (SPDIF_REG_TYPE_T type);
extern void _AUD_DspSetSpdifCopyright(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 copyright);
extern void _AUD_DspSetSpdifCategoryCode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 categoryCode);
extern void _AUD_DspSetSpdifWordLength(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 wordLength);
extern void _AUD_DspSetSpdifUpdateMode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 eUpdate);
extern UINT8 _AUD_DspGetSpdifCopyright(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom);
extern UINT8 _AUD_DspGetSpdifCategoryCode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom);
extern UINT8 _AUD_DspGetSpdifWordLength(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom);
extern UINT8 _AUD_DspGetSpdifupdateMode(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom);
extern BOOL _AUD_DspGetSpdifChannelStatus(SPDIF_CHANNEL_STATUS_T *rInfo);
extern UINT8 _AUD_DspGetSpdifOutputMode(UINT8 u1DecId);
extern BOOL _AUD_DspSpdifSetInfo(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1CopyRight, UINT8 u1CategoryCode);
extern BOOL _AUD_DspSpdifSetCopyProtect(SPDIF_REG_TYPE_T type, AUD_DEC_STREAM_FROM_T eStreamFrom, BOOL bCpBit, BOOL bLBit);
extern void _AUD_DspSetIEC(UINT8 u1DecId);
#ifdef CC_AUD_DDI
extern void _AUD_DspSetSoundBarOnOff(BOOL fgOnOff);
extern void _AUD_DspSetSoundBarIDData(UINT32 Id, UINT8 data, UINT8 volumeInfo);
extern void _AUD_DspGetSoundBarStatus(UINT8 *pId, UINT8 *pdata);
extern void _AUD_UserSetSpdifRawDec(UINT8 u1DecId); 
#endif
#ifdef CC_AUD_MIXSOUND_SUPPORT
extern void _AUD_DspMixSndEnable(BOOL fgEnable);
extern void _AUD_DspMixSndMute(BOOL fgMute);
extern void _AUD_DspMixSndControl(UINT16 u2Mode);
extern void _AUD_DspMixSndControl2(UINT16 u2Mode); //NEW_MIXSOUND
extern void _AUD_DspMixSndClip(UINT8 u1ClipIdx);
extern BOOL _AUD_DspGetMixSndEnable(void);
extern BOOL _AUD_DspGetMixSndClipIdx(void);
extern void _AUD_DspSetMixSndLoop(UINT32 u4Loop);
extern void _AUD_DspSetMixSndVol(UINT32 u4Vol);
extern UINT32 _AUD_DspGetMixSndLoop(void);
extern UINT32 _AUD_DspGetMixSndVol(void);
#endif
extern void _AUD_DspSetDemodOutputMode(UINT8 u1Output);

#ifdef DSP_SUPPORT_SRSTSXT
extern void _AUD_DspSetSRSTSXTSpkSz(UINT8 u1Mode);
extern void _AUD_DspSetSRSTSXTSetMode(UINT8 u1Mode);
extern void _AUD_DspSetSRSTSXTElev(UINT8 u1Mode);
extern void _AUD_DspSetSRSTSXTinputgain(UINT32 u4Mode);
extern void _AUD_DspSetSRSTSXTTBLvl(UINT8 u1Mode);
#endif

extern void _AUD_DspSetDetectDefaultMode(UINT8 u1Mode);
extern void _AUD_DspSetUserDefinedEqCfg(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8 i1Value);
extern BOOL _AUD_DspGetUserDefineEqCfg(AUD_EQ_TYPE_T eEqType, UINT8 u1BandIdx, INT8* pi1Value);
extern UINT8 _AUD_DspGetUserEqBandNum(void);
extern void _AUD_DspGetEqBandInfo(AUD_EQ_BAND_INFO_T* prEqBandInfo);
extern INT16 _AUD_DspGetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc);
extern void _AUD_DspSetSrcVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol);
#ifdef CC_AUD_INPUT_SRC_VOL_OFFSET
extern INT16 _AUD_DspGetSrcVolOffset(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc);
extern void _AUD_DspSetSrcVolOffset(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol);
#endif
extern void _AUD_SetOutPortHwMute(AUD_OUT_PORT_T ePort, BOOL fgMute, BOOL fgNotify);
extern void _AUD_SetOutPortVolMute(AUD_OUT_PORT_T ePort, BOOL fgMute, BOOL fgNotify);

extern void _AUD_DspVolumeChange(UINT8 u1DspId, UINT8 u1DecId, AUD_CH_T eAudChl);
#ifdef CC_AUD_CHL_VOL_MODE
extern void _AUD_DspLRChannelVolMode(UINT8 u1DecId, UINT8 u1Mode);
#endif

extern void _AUD_DspSetAvSrcVol(AUD_INPUT_ID_T eInputSrcId , INT16 i2Vol);
extern AUD_INPUT_ID_T _AUD_DspGetAvInputSrcId(void);
extern void _AUD_DspSetAvSrcVol(AUD_INPUT_ID_T eInputSrcId , INT16 i2Vol);
extern AUD_INPUT_ID_T _AUD_DspGetAvInputSrcId(void);
extern AUD_INPUT_ID_T _AUD_DspGetAvInputSrcDvdId(void);
extern void _AUD_DspGetAtvTvSys(MW_TV_AUD_SYS_T * prTvAudSys);
extern TV_AUD_SYS_T _AUD_DspGetAtvTvSystem(void);
extern UINT32 _AUD_DspGetAtvTvSystemStrength(TV_AUD_SYS_T);

#ifdef CC_AUD_TV_SYS_FINE_TUNE_VOL
extern void _AUD_SetTVSysFineTuneVol(TV_AUD_SYS_T eAudSys);
#endif
extern void _AUD_DspSetTvSrcMode(TV_AUD_SYS_T eAudSys);
extern TV_AUD_SYS_T _AUD_DspGetTvSrcMode(void);
extern void _AUD_DspSetAtvOutputMode(UINT8 u1DecId, AUD_SOUND_MODE_T eSoundMode);
extern BOOL _AUD_IsSIFExist(void);
#if 0   // Unused
extern void _AUD_DspHdmiSpdifOutEnable(BOOL fgEnable);
#endif
extern void _AUD_DspPlayMuteSpdifEnable(BOOL fgEnable);
extern UINT32 _AUD_DspGetSpectrumInfo(UINT8 u1DecId, UINT8 u1BandNum);
extern AUD_SOUND_MODE_T _AUD_DspGetSoundMode(UINT8 u1DecId);
extern void _AUD_DspSpectrumEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_AudioSetStcDiffBound(UINT8 u1DecId, UINT8 uStcDiffLo, UINT8 uStcDiffHi, UINT8 uStcDiffWs);
extern void _AUD_DDBannerEnable(UINT8 u1DecId, BOOL fgEnable);
extern BOOL _AUD_GetDDBannerEnable (void);
extern void _AUD_DspSetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eChangedItem,UINT16 u2Value);
extern UINT16 _AUD_DspGetPalDetection ( AUD_FACTORY_PAL_DETECTION_ITEM_T eItem) ;
extern void _AUD_DspSetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eChangedItem,UINT16 u2Value) ;
extern UINT16 _AUD_DspGetA2Detection ( AUD_FACTORY_A2_DETECTION_ITEM_T eItem);
extern void _AUD_DspSetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eChangedItem,UINT16 u2Value) ;
extern UINT16 _AUD_DspGetA2Threshold ( AUD_FACTORY_A2_THRESHOLD_ITEM_T eItem);
extern void _AUD_DspSetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eChangedItem,UINT16 u2Value);
extern UINT16 _AUD_DspGetMtsDetection ( AUD_FACTORY_MTS_DETECTION_ITEM_T eItem) ;
#if 0   // Unused
extern void _AUD_DspSetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eChangedItem, UINT16 u2Value);
extern UINT16 _AUD_DspGetFmfmDetection ( AUD_FACTORY_FMFM_DETECTION_ITEM_T eItem);
#endif
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT
extern void _AUD_DspSetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType, UINT16 u2Mode);
extern UINT8 _AUD_DspGetHdevMode (AUD_ATV_DECODER_TYPE_T eDecType);
#else
extern void _AUD_DspSetHdevMode ( AUD_ATV_DECODER_TYPE_T eDecType, UINT8 u2Mode);
extern UINT8 _AUD_DspGetHdevMode (AUD_ATV_DECODER_TYPE_T eDecType);
#endif
extern void _AUD_DspSetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eChangedItem,UINT8 u1Value);
extern UINT8 _AUD_DspGetAmMute( AUD_FACTORY_PAL_AM_MUTE_ITEM_T eItem);
extern void _AUD_DspSetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType, AUD_FACTORY_FM_MUTE_ITEM_T eChangedItem, UINT8 u1Value);
extern UINT8 _AUD_DspGetFmMute ( AUD_ATV_DECODER_TYPE_T eDecType,AUD_FACTORY_FM_MUTE_ITEM_T eItem);
extern void _AUD_DspSetCarrierShiftMode ( AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable);
extern BOOL _AUD_DspGetCarrierShiftMode (AUD_ATV_DECODER_TYPE_T eDecType);
extern void _AUD_DspSetNonEUMode (BOOL fgEnable) ;
extern BOOL _AUD_DspGetNonEUMode (void);
extern void _AUD_DspSetFMSatuMuteMode (AUD_ATV_DECODER_TYPE_T eDecType, BOOL fgEnable);
extern BOOL _AUD_DspGetFMSatuMuteMode (AUD_ATV_DECODER_TYPE_T eDecType);
extern void _AUD_DspSetPALFineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetPALFineVolume (void);
extern void _AUD_DspSetNicamFineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetNicamFineVolume (void) ;
extern void _AUD_DspSetAMFineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetAMFineVolume (void);
extern void _AUD_DspSetA2FineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetA2FineVolume (void);
extern void _AUD_DspSetMtsMonoFineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetMtsMonoFineVolume (void) ;
extern void _AUD_DspSetSAPFineVolume(UINT8 u1Index);
extern UINT8 _AUD_DspGetSapFineVolume (void) ;
#if 0   // Unused
extern void _AUD_DspSetFmfmMonoFineVolume (UINT8 u1Index);
extern UINT8 _AUD_DspGetFmfmMonoFineVolume (void) ;
extern void _AUD_DspSetFmfmDualFineVolume(UINT8 u1Index);
extern UINT8 _AUD_DspGetFmfmDualFineVolume (void) ;
#endif
extern void _AUD_DspSetFmRadioFineVolume (UINT8 u1Index) ;
extern void _AUD_DspSetMtsPilotDetection (BOOL fgEnable) ;
extern BOOL _AUD_DspGetMtsPilotDetection (void) ;
extern void _AUD_DspSetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eChangedItem, UINT8 u1Value);
extern UINT8 _AUD_DspGetSapMute( AUD_FACTORY_SAP_MUTE_ITEM_T eItem);
extern void _AUD_DspChSetEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[]);//Andrew Wen 07/8/7 Add EQ band gain CLI commands
extern void _AUD_DspChQryEqTable(UINT8 u1DecId, AUD_EQ_TYPE_T eEqType, INT8 eqValue[]);
extern void _AUD_DspSetFMSatuMuteTh( AUD_FACTORY_SATU_MUTE_ITEM_T eChangedItem,
                                                                                        UINT8 u1Value) ;
extern UINT8 _AUD_DspGetFMSatuMuteTh( AUD_FACTORY_SATU_MUTE_ITEM_T eItem) ;

#ifdef CC_SET_VOLUME
extern void _AUD_DspSetVolTable(UINT8 u1Idx, UINT32 u4Vol);
#endif
extern void _AUD_DspSetVolumeTable(UINT32* u4VolTabChkPnt);
extern UINT32 _AUD_DspGetVolumeTable(UINT8 u1VolTabIdx);

#ifdef CC_AUD_NEW_CV_TABLE
#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
extern void _AUD_DspSetCVTable(UINT8 u1Idx,UINT32 u4Vol);
extern UINT32 _AUD_DspGetCVTable(UINT8 u1VolTabIdx);
extern void _AUD_DspSetStepCVTable(UINT32* u4VolTabChkPnt);
#else
extern void _AUD_DspSetCVTable(AUD_NVM_VOL_TBL_T rVolTbl, UINT8 u1Idx,UINT32 u4Vol);
extern UINT32 _AUD_DspGetCVTable(AUD_NVM_VOL_TBL_T rVolTbl, UINT8 u1VolTabIdx);
extern void _AUD_DspSetStepCVTable(AUD_NVM_VOL_TBL_T rVolTbl, UINT32* u4VolTabChkPnt);
#endif
#endif

#ifdef CC_AUD_NEW_HP_VOL_TABLE
extern void _AUD_DspSetHpVolumeTable(UINT8 u1Idx,UINT32 u4Vol); 
extern UINT32 _AUD_DspGetHpVolumeTable(UINT8 u1VolTabIdx);
#endif
extern UINT32 _AUD_DspSpkSizeQry(UINT8 u1DecId);
#ifdef CC_S_SUBWOFFER_SUPPORT  ///CC_BASS_MANGER_INDEP_LPF
extern UINT32 _AUD_DspSpkSizeLpfQry(UINT8 u1DecId);
#endif
extern void _AUD_DspMasterVolShmRawValue(UINT8 u1DecId, UINT32 u4VolShm);
#if 0   // Unused
extern void _AUD_DspSetPCMChannelNumber(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2ChannelNo);
extern UINT8 _AUD_DspGetVolumeBaseLevel(void);
#endif

extern void _AUD_DspProcMode(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2Mode);
extern void _AUD_DspADModeSet(UINT8 u1DecId, UINT16 u2Value);
extern void _AUD_DspADEnable(UINT8 u1DecId, BOOL fgEnable);
#ifdef CC_AUD_SKYPE_SUPPORT
extern void _AUD_DspSkypeRingMix(BOOL fgEnable);
extern void _AUD_DspAuxMixToMain(BOOL fgEnable);
#endif

#ifdef CC_AUD_PCM_LINE_IN_LPF
extern void _AUD_DSPSetPCMLineInLPF(BOOL fgEnable);
#endif

extern void _AUD_Surround_Get_Config(AUD_VSURR_CFG_T* prVsurrCfg);
extern void _AUD_Surround_Set_Config(AUD_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4VSParameter);

#ifdef CC_3DTV_AUDIO_LINK_SUPPORT
extern void _AUD_3D_AVLINK_Config(UINT32 u4DepthField);
#endif //_AUD_DSP_CFG_H_

extern void _AUD_DspSetPcmPreBufferLength(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2Length);
extern UINT16 _AUD_DspGetPcmPreBufferLength(UINT8 u1DspId, UINT8 u1DecId);
extern BOOL _AUD_GetNicamStatus(void);
#ifdef SUPPORT_PHILIPS_LOUDNESS
extern AUD_TYPE_T _AUD_GetAudInfo(UINT8 u1DecId) ;
#endif
extern void _AUD_SetOutPortMode(UINT8 u1DecId, UINT8 fgMode);
#ifdef CC_AUD_LINEOUT_VOL_MODE
extern void _AUD_SetLineOutVolumeMode(UINT8 u1DecId, UINT8 fgMode);
extern BOOL AUD_GetLineOutVolumeMode(UINT8 u1DecId);
extern UINT8 _AUD_DspGetLineOutFixVol(void);
#endif
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
extern void _AUD_DspSrcLinoutVol(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrmSrc, INT16 i2Vol);
#endif
extern void _AUD_GetDtvAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo) ;
extern void _AUD_DspSpeed(UINT8 u1DecId, UINT16 u1Speed);
extern void _AUD_LGSEFN000(UINT8 fNo, VOID* u1CV_param_buf, UINT16 noParam, UINT8 dataOption, UINT8 varOption);
extern void _AUD_DspGetSpeed(UINT8 u1DecId, UINT16 *u1Speed);
extern void _AUD_DspDownmixPosition(UINT8 u1DspId, UINT8 u1DMPosition);
extern UINT8 _AUD_DspGetDownmixPosition(UINT8 u1DspId);
extern void _AUD_DspMultiPairOutput(UINT8 u1DspId, UINT8 u1DecId, UINT16 u1MultiPair);
extern void _AUD_DspLRDownmix(UINT8 u1DspId, UINT8 u1DecId, BOOL fgEnable);
extern UINT16 _AUD_DspGetSystemSetup(UINT8 u1DspId, UINT8 u1DecId);

extern BOOL _AUD_GetVideoScrambleStatus(void);
extern void  _AUD_VideoScrambled(BOOL fgEnable);

//paul_flac
void _Aud_DspFLACHeader(UINT8 u1DecId, AUD_FLAC_INFO_T * prFlacInfo);

//#ifdef DATA_DISC_WMA_SUPPORT
extern void _AUD_DspWMAHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo);
#if 1 //CC_APE_SUPPORT
extern void _AUD_DspAPEHeader(UINT8 u1DecId,AUD_APE_CFG_T* prApeInfo);  //ian APE decoder
extern void _AUD_DspAPEMuteBankNum(UINT8 u1DecId,UINT16 u2MuteBankNum);  //zhengwen add for ape seek/FF
#endif
extern void _AUD_DspWMAProHeader(UINT8 u1DecId,AUD_WMA_CFG_T* prWmaInfo);
#ifdef CC_53XX_SWDMX_V2
extern void AUD_DrvSetWMAErrFunc(UINT8 u1DecId, AUD_WMAErr_NOTIFY pfClipNotify, VOID *prInst);  // megaa 20090828
#else
extern void AUD_DrvSetWMAErrFunc(UINT8 u1DecId, AUD_WMAErr_NOTIFY pfClipNotify);
#endif
//#endif
#if 1   // Unused
extern void _AUD_DspSetDualDecMode(BOOL fgEnable);
#endif
#ifdef CC_AUD_LINEIN_USE_DEC3
extern void _AUD_DspSetTriOnlyDecMode(BOOL fgEnable);
#endif
extern BOOL _AUD_DspIsTriOnlyDecMode(void);
extern void _AUD_DspMixSndDec3(BOOL fgEnable);
#ifdef CC_AUD_HPF_SUPPORT
extern void _AUD_DspSetHPFEnable(BOOL fgEnable);
extern void _AUD_DspSetHPFFc(UINT16 u2Fc);
extern void _AUD_DspSetHPF2Fc(UINT16 u2Fc);

#endif

#ifdef DSP_SUPPORT_TCL_VBASS
extern void _AUD_DspSetTVBEnable(BOOL fgEnable);

#endif

#ifdef CC_AUD_CLIPPER_SUPPORT
extern void _AUD_DspSetClipperEnable(BOOL fgEnable);
extern void _AUD_DspSetClipperValue(AUD_CH_T eChannel, UINT32 u4Value);
#endif
#if (defined(CC_AUD_SPEAKER_HEIGHT) || defined(CC_AUD_SPEAKER_HEIGHT_V2))
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
extern void _AUD_DspSetSPHEnable(BOOL fgEnable);
#else
extern void _AUD_DspSetSPHMode(UINT8 u1Mode);
#endif
#endif
extern void _AUD_DspSetPEQEnable(BOOL fgEnable);
extern BOOL _AUD_DspGetPEQEnable(void);
extern void _AUD_DspSetPEQCfg(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT16 u2BW, INT16 i2Gain);
#ifdef CC_AUD_PEQ_LR
extern void _AUD_DspSetPEQCfg_R(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT16 u2BW, INT16 i2Gain);
#endif
#if 0 //sharp Q setting
extern void _AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            AUD_PEQ_Q_TYPE_T eQValue, INT16 i2Gain);
#else
extern void _AUD_DspSetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT32 eQValue, INT16 i2Gain);
extern void _AUD_DspGetPEQCfg2(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 *u4Frequency,
                            UINT32 *eQValue, UINT32 *u4Gain);
#ifdef CC_AUD_PEQ_LR
extern void _AUD_DspSetPEQCfg2_R(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 u4Frequency,
                            UINT32 eQValue, INT16 i2Gain);
extern void _AUD_DspGetPEQCfg2_R(UINT8 u1DecId, UINT8 u1PEQSet, UINT32 *u4Frequency,
                            UINT32 *eQValue, UINT32 *u4Gain);
#endif
#endif
#ifdef CC_AUD_VBASS_SUPPORT
extern void _AUD_DspSetVirBassEnable(BOOL fgEnable);
extern void _AUD_DspSetVirBassCfg(UINT8 u1DecId, UINT32 u4Frequency, UINT32 u4Gain,
                        UINT32 u4Gain2);
extern void _AUD_DspGetVirBassCfg(UINT8 u1DecId, BOOL *fgEnable, UINT32 *u4Gain, UINT32 *u4Gain2);
#endif

#ifdef CC_AUD_FIR_SUPPORT
extern void _AUD_DspSetFIREnable(BOOL fgEnable);
#endif

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
extern void _AUD_DspSetADVEnable(UINT8 fgEnable);
extern void _AudShowADVStatus(void);
extern void _AUD_DspADVSetChGain(UINT8 ch, UINT32 value);
extern void _AUD_DspABXConfig(UINT8 item, UINT8 val);
extern void _AUD_DspABXQuery(void);
extern void _AUD_DspAEQEnable(UINT8 u1DecId, BOOL fgEnable);
extern void _AUD_DspAEQQuery(void);
#endif


extern void _AUD_DspSetDDCOEnable(BOOL fgEnable);
extern BOOL _AUD_DspIsDDCOEnable(void);
extern void _AUD_DspSetDDCOAgcEnable(BOOL fgEnable);
extern void _AUD_DspSetDDCOLFEEnable(BOOL fgEnable);
extern void _AUD_DspSetAoutReinit(void);
extern void AUD_DspDecPlayMuteEnable(UINT8 u1DecId, BOOL fgEnable);
extern void AUD_DspDecPowerOffMute(UINT8 u1DecId, BOOL fgEnable);

#ifdef CC_AUD_USE_NVM
extern UINT32 _AUD_NVM_Get_Profile(SMART_AUD_GROUP_INFO_T* prSmartAudioInfo);
extern UINT8 _AUD_NVM_Get_Profile_Id(void);
extern void _AUD_NVM_Get_FlashAQ_List(UINT8 u1Profile, UINT8 *pu1Data);
extern BOOL Aud_NVM_GetCountry(void);
extern UINT32 _AUD_NVM_Read(void);
extern UINT32 AUD_NVM_Operation(AUD_NVM_OP_T uOP, UINT8* puNvmBuffer, UINT16 u2Offset);
#ifdef CC_AUD_USE_NVM_AMP
extern UINT32 AUD_NVM_Init_Amp(void);
#endif
extern UINT32 AUD_NVM_Init_From_EEPROM(void);
extern UINT32 AUD_NVM_QRY_ATV_GAIN(AUD_NVM_ATV_QRY_T eAudNvmQryT, AUD_NVM_ATV_FMT_T eAudQryFmt);
#endif

extern void _AUD_DspGetMpegAudInfo(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo);
extern void _AUD_DspGetMpegAudInfoWithLayer(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo, UINT8 *prMPEGLayer);
extern UINT8 _AUD_GetAacVersion(UINT8 u1DecId);
extern UINT8 _AUD_GetAacTSFormat(UINT8 u1DecId);
extern UINT8 _AUD_GetAC3_EAC3_Info(UINT8 u1DecId);

#if 0   // Unused
extern void _AUD_DspSetMp3PreparsingMode(BOOL fgEnable);
extern BOOL _AUD_DspIsMp3Vbr(void);
#endif
extern void _AUD_DspCOOKParameter(UINT8 u1DecId, AUD_COOK_CFG_T* prCookInfo);
#ifdef CC_AUD_USE_FLASH_AQ
extern BOOL Aud_FlashAQQuery(AUD_AQ_TYPE_T eAQDataType, VOID* *pData);
extern void Aud_FlashAQParsing(AUD_AQ_TYPE_T eAQDataType, UINT8* pData);
extern void Aud_FlashAQSetValue(void);
extern void _AUD_FlashAQPeriphSMChange(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1ModeCase);
extern void _AUD_FlashAQPeriph_EXTPEQ(AUD_DEC_STREAM_FROM_T eStreamFrom, UINT8 u1ModeCase, SAMPLE_FREQ_T eFS);
extern void _AUD_FlashAQPeriphRecover(void); 
extern void _AUD_DspSetBassTrebleOffset(UINT8 u1BassOffset, UINT8 u1TrebleOffset);
extern void _AUD_DspGetBassTrebleOffset(UINT8 *u1BassOffset, UINT8 *u1TrebleOffset);
#endif

#ifdef CC_AUD_SETUP_MELODY
extern void _AUD_DspSetMelodyVol(UINT8 u1Vol);
#endif

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
extern void _AUD_DspSetMixSndLength(UINT32 u4Len);
#ifndef NEW_MIXSOUND
extern void _AUD_DspSetMixSndGain(UINT32 u4Gain);
extern void _AUD_DspSetMixSndStereo(UINT8 bOnOff);
extern void _AUD_DspSetMixSndUpSampleRatio(UINT8 bRatio);
#endif
extern UINT32 _AUD_DspGetMixSndGain(void);
extern BOOL _AUD_DspGetMixSndStereo(void);
extern void _AUD_DspSetMixSndMemPlay(UINT8 bOnOff); // !NEW_MIXSOUND
extern BOOL _AUD_DspIsMixSndPlay(void);


extern void _AUD_DspSetMixSndChannel(UINT16 wCh);
extern UINT32 _AUD_DspGetMixSndReadPtr(UINT8 u1StreamID);
extern void _AUD_DspSetMixSndWritePtr(UINT8 u1StreamID, UINT32 u4WritePtr);
extern void _AUD_DspShowMixSndStatus(UINT8 u1StreamID);
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
extern void _AUD_DspSetUploadDataEnable (void);
extern void _AUD_DspSetUploadMode (UINT8 u1Mode);
#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
extern void _AUD_DspSetBluetoothDataEnable (void);
extern void _AUD_DspSetBluetoothCnt(UINT8 cnt);
extern void _AUD_DspSetBluetoothMode (UINT8 u1Mode);
#endif
//#endif
extern void _AUD_DspSetEncMode(UINT8 u1DecId, AUD_ENC_MODE_T eEncMode);
extern void _AUD_DspSetEncInfo(UINT8 u1DecId, UINT32 u4Bitrates);
extern void  _AUD_DspSetDTSENCEnable(BOOL fgEnable);
extern void _AUD_DspSetWithVideo(UINT8 u1DecId, BOOL fgIsWithVideo);
extern BOOL _AUD_DspIsWithVideo(UINT8 u1DecId);

extern void _AUD_DspAsrcSetBaseFreq(UINT8 u1SrcID, UINT32 u4BaseFreq);
extern void _AUD_DspAsrcSetSmartVCXO(UINT32 u4AdjustLevel);
extern void AUD_WFD_DspAsrcSetSmartVCXO(UINT8 u1DecId);
extern void _AUD_DspAsrcControl(UINT8 u1SrcID);
extern void _AUD_DspAsrcResetSmartVCXO(void);
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
extern void _AUD_AprocAsrcControl(UINT8 u1DecID);
extern void _AUD_AprocEnableVCXO(UINT8 u1DecID, BOOL fgEnable);
extern void _AUD_AprocAsrcTrackingMode(UINT8 u1DecId, BOOL fgEnable); 
#endif

extern void AUD_DspCfgSuspend(void);

extern void _AUD_DspSetCapability(void);
extern UINT32 _AUD_DspGetCapability(UINT8 u1Idx);
extern BOOL _AUD_DspIsFormatValid(UINT32 u4FormatInfo);
extern BOOL _AUD_DspIsSupportAC3(void);
extern BOOL _AUD_DspIsSupportEAC3(void);
#if 0   // Unused
extern BOOL _AUD_DspIsSupportDDCO(void);
#endif
extern UINT32 _AUD_DspQueryChannelDelay(UINT8 u1ChId);  //QUERY_DELAY
#ifdef CC_MPEG_LEVEL_ADJ
extern void _AUD_DspMpgLvlChlVolGain(UINT8 u1Level);
#endif

#ifdef CC_VOCAL_MUTE
extern void _AUD_VocalMuteOnOff(UINT8 bMode,UINT8 fgOnOff);
#endif

extern void _AUD_DspSetStcId(UINT8 u1DecId, UINT16 u2StcId); // 2_STC_Task
extern UINT8 _AUD_DspQryStcID(UINT8 u1DecId);
extern void _AUD_DspGetStcId(UINT8 u1DecId, UINT16 *u2StcId);

extern void AUD_SoftwareMuteQuery(void);
#ifdef CC_ENABLE_AOMX
extern void _AUD_DspPcmVolume(UINT8 u1Value, UINT8 u1Value1);
#endif
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
extern void _vAUD_Aproc_Set (UINT32 u4Type, UINT32 u4Item, UINT32 *pu4Addr, UINT32 u4Num);
extern void vAproc_PostProcVol_Get (UINT32 u4Item, UINT32 *pu4Addr);
#ifdef CC_AUD_DDI 
extern void _AUD_UserSetMixSndInputVol(UINT8 u4MixIdex, UINT8 u1MainVol, UINT8 u1FineVol);
extern void _AUD_UserSetMixSndMute(UINT8 u4MixIdex, UINT8 u1Mute);
extern void _AUD_UserSetMixSndOutputVol(UINT8 u4MixIdex, UINT8 u1MainVol, UINT8 u1FineVol);
extern void _AUD_UserSetDecInputMute(UINT8 u1DecId, BOOL fgMute);
extern void _AUD_UserSetDecInputVol(UINT8 u1DecId, UINT8 u1MainVol, UINT8 u1FineVol);
extern void _AUD_UserSetDecOutputVol(UINT8 u1DecId, UINT8 u1MainVol, UINT8 u1FineVol);
extern void _AUD_UserSetDecChannelGain(UINT8 u1DecId, UINT8 u1LetfMainVol, 
                    UINT8 u1LeftFineVol, UINT8 u1RightMainVol, UINT8 u1RightFineVol);
extern void _AUD_UserSetDecInputDelay(UINT8 u1DecId, UINT16 u2DelayTime);
extern void _AUD_UserSetDecOutCtrl(AUD_OUT_PORT_T eAudioOutPort, UINT32 u4OutSel, BOOL fgEnable); 
#endif
#endif
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
extern void _AudAprocInputMute(UINT8 u1DecId, BOOL fgMute);
#endif
#ifdef CC_AUD_LINEOUT_VOL_MODE
extern INT32 GPIO_Input(INT32 i4GpioNum);
#endif

#ifdef CC_SUPPORT_STR
extern void _AUD_PostProc_Resume(void);
extern void _AUD_PostProc_Suspend(void);
#endif
#endif //_AUD_DSP_CFG_H_ 

