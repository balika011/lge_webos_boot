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
#define AUD_EXTERN_FILE

/*****************************************************************************
*  Audio : Diagnostic command
*****************************************************************************/
// KERNEL
#include "x_util.h"
#include "x_os.h"
#include "x_printf.h"
#include "x_stl_lib.h"

#define DEFINE_IS_LOG   CLI_IsLog
#include "x_debug.h"
#include "x_util.h"
#include "x_hal_5381.h"
#include "x_hal_io.h"

#include "aud_cmd.h"
#include "aud_if.h"
#include "sif_if.h"
#include "x_assert.h"
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
#include "oss_adap.h"
#endif

#ifndef __KERNEL__
#include "../../mt53xx_com/53xx_com_driver/aud/audio/aud_dsp_cfg.h"
#include "../../mt53xx_com/53xx_com_driver/aud2/adsp/include_API/dsp_common.h"
#include "../../mt53xx_com/53xx_com_driver/aud2/adsp/include_API/dsp_uop.h"
#include "../../53xx_com_driver/fm/x_fm.h"
#else // __KERNEL__
#include "aud_dsp_cfg.h"
#include "dsp_uop.h"
#include "dsp_common.h"
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/param.h>
#endif // __KERNEL__

#ifndef USB_PATH
//#define USB_PATH "/mnt/usb_0"
#define USB_PATH "/mnt/usb/Mass-000"
#endif


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable);
extern void SPDIF_InInit(void) ;
extern BOOL SPDIF_InLockCheck(void) ;
extern void SPDIF_InRegisterDump(void) ;
extern void AUD_SoftwareMuteQuery(void);
extern AUD_DEC_STREAM_FROM_T _AudGetStrSource(UINT8 u1DecId);

//ADC/DAC related
extern void _MT5398_Mute(UINT8 u1DacId,BOOL fgMute);
extern void _MT5398_MuteALRx(UINT8 u1ALRx,BOOL fgMute);
extern void _MT5398_InterPWMEnable(BOOL fgEnable);
extern void _MT5398_InterLDOEnable(BOOL fgEnable);
extern void _MT5398_InterPWMGainSet(INT8 pgroup,INT32 pgain);
extern void _MT5398_InterMonitorSet(INT8 pgroup,INT8 Monitor_ID);
extern void _MT5398_InterPWMDelay(INT8 pgroup, INT32 pwmdelay);
extern UINT32 _MT5398_InterPWMGainGet(INT8 pgroup);
extern void _MT5398_YRamRead(UINT8 staddr,UINT8 length);
extern void _MT5398_YRAMWrite(INT8 pgroup,UINT32 YRAM_Addr, UINT32 YRAM_Data);
extern void _MT5398_InterPWMDACRead(INT8 pgroup,UINT32 pwmdac_addr);
extern void _MT5398_InterPWMDACWrite(INT8 pgroup, UINT32 pwmdac_addr,UINT32 pwmdac_data);
extern void _MT5398_PWMSigGen(INT8 SigType);
extern void _MT5398_InterDacEnable(BOOL fgMute);
extern void _MT5398_Init(void);
extern void _MT5398_AADCMute(UINT8 u1CH,BOOL fgMute);
extern void _MT5398_AADCDOWNCH(UINT8 u1CH, BOOL fgDown);
extern void _MT5398_ADCGainQuery(void);
extern void _MT5398_SetADCGain(UINT32 u4Value);

extern UINT32 AUD_DspGetPc(UINT8 u1DspId);
extern void AUD_GetRWPtr(UINT8 u1DspId, UINT8 u1DecId, UINT32 * pu4Rp, UINT32 * pu4Wp, UINT32 *pu4Size);
extern UINT32 AUD_DspProbePause(UINT8 u1DspId);
extern UINT32 AUD_DspProbeGo(UINT8 u1DspId);
extern UINT32 AUD_DspProbeStep(UINT8 u1DspId, UINT32 u4Count);
extern UINT32 fgAUD_DspProbeHalt(UINT8 u1DspId);
extern UINT32 u4AUD_DspProbeRead(UINT8 u1DspId, UINT32 u4Addr);
extern UINT32 AUD_DspProbeWrite(UINT8 u1DspId, UINT32 u4Addr, UINT32 u4Data);
extern UINT32 u4AUD_DspProbePBCfg(UINT8 u1DspId, BOOL fgSet,UINT32 u4Value);
extern UINT32 u4AUD_DspProbePB(UINT8 u1DspId, BOOL fgSet,UINT32 u4BPNum, UINT32 u4Addr);
extern UINT32 u4AUD_DspProbeStopOnWrite(UINT8 u1DspId, UINT32 u4Addr, UINT32 u4Data, UINT32 u4Mask);
extern UINT32 u4AUD_DspProbeChkStopOnWrite(UINT8 u1DspId, BOOL *fgEnable, UINT32 *u4Data, UINT32 *u4Mask);
extern void AUD_AudioPTSDelayEnable(UINT8 u1DecId, UINT8 uDlyFactor);
extern void AUD_AoutPADMux(AUD_CH_T ePAD_SDATA0, AUD_CH_T ePAD_SDATA1, AUD_CH_T ePAD_SDATA2,
                    AUD_CH_T ePAD_SDATA3, AUD_CH_T ePAD_SDATA4, AUD_CH_T ePAD_SDATA5,
                    UINT8 PAD_MCLK0);
extern void AUD_AoutPADMuxQuery(void);
extern BOOL AUD_DspSetIecRawFlag(UINT8 u1Flag);
extern BOOL AUD_DspSetIecRawDelay(INT16 i2Delay);

extern void vWriteDspSram(UINT32 u4Addr,UINT32 u4Value);
extern void vWriteDspSram32(UINT8 u1DspId, UINT32 u4Addr,UINT32 u4Value);
extern UINT32 u4ReadDspSram(UINT8 u1DspId, UINT32 u4Addr);
extern UINT32 u4ReadDspSram32(UINT8 u1DspId, UINT32 u4Addr);
extern void AudAoutGainCtrl(UINT8 u1DecId, UINT8 CHID, UINT16 Gain, UINT8 u1Step);
extern void vADSPTaskInit(void);
//extern void AUD_GetAudioPes(UCHAR ucDecId, PSR_AUDIO_PES_T * prAudioPes);
extern AV_SYNC_MODE_T AUD_GetAvSynMode(UINT8 u1DecId);
extern void AUD_SetAvSynMode(UCHAR ucDecId, AV_SYNC_MODE_T eSynMode);
extern void AUD_AoutInvertData(UINT8 ucDecId, BOOL fgInvert);
extern void AUD_AoutFormat(UINT8 ucDecId, DATA_FORMAT_T eDataFormat);
extern void DSP_ShareInforDump(UINT8 u1DspId, UINT8 u1Group);
//Light added for AVC
extern void DSP_ShareInforWriteWord(UINT8 u1DspId, UINT32 ADDR, UINT16 VALUE);
extern void DSP_CommDramDump(UINT8 u1DspId, UINT32 u4Address, UINT32 u4Len);
extern void DSP_NormDramDump(UINT8 u1DspId, UINT8 u1Page, UINT32 u4Address, UINT32 u4Len);
extern void DSP_CmptDramDump(UINT8 u1DspId, UINT8 u1Page, UINT32 u4Address, UINT32 u4Len);
extern void Dsp_WriteDspCommDram(UINT8 u1DspId, UINT32 u4SrcAddr,UINT32 u4Value);
#ifdef CC_AUD_FIR_SUPPORT
extern UINT32 DSP_CommDramAddr(UINT8 u1DspId, UINT32 u4Address) ; //CC_AUD_FIR_SUPPORT
#endif
extern void AudShowDspStatus(UINT8 u1DspId);
extern void AudShowMtsStatus(UINT8 u1DecId);
extern void AudShowStatus(UINT8 u1DspId, UINT8 u1DecId);
extern void AudShowDspVer(UINT8 u1DspId, BOOL fgShowUnitChkSum);
extern void AudShowAsrcStatus(UINT8 u1SrcID);
extern void AudShowAsrcBufferStatus(UINT8 u1SrcID,UINT8 u1ChMsk,BOOL fgUpdateRWP);
extern void AudShowDspCapability(void);
extern void DSP_SetAudLogPeriod(UINT32 u4Period);
extern BOOL fgDspReadPtrSet (UINT8 u1DspId, UCHAR ucDecId, UINT32 u4Address);
extern void SPDIF_InChSel(UINT8 u1Ch);
extern void AUD_AoutDrvCur(UINT8 u1DecId, UINT8 u1MckDrvCur, UINT8 u1BckDrvCur ,UINT8 u1LRckDrvCur , UINT8 u1AOSDATADrvCur);
extern void AUD_StopMemStr(void);
extern void AUD_ChlOutDump(UINT8 u1DspId, UINT8 u1Chl);
extern void AUD_DspProcMode(UINT8 u1DspId, UINT8 u1DecId, UINT16 u2Mode);
extern void AUD_AvMuxSel(AUD_INPUT_ID_T eInputId);
extern UINT8 ADAC_CodecRegRead(UINT8 u1Offset);
extern void ADAC_CodecRegWrite(UINT8 u1Offset, UINT8 u1Data);
extern UINT8 ADAC_AmpRegRead(UINT8 u1Offset);
extern void ADAC_AmpRegWrite(UINT8 u1Offset, UINT8 u1Data);
extern void DSP_SetDualDecMode(BOOL fgEnable);
#ifdef CC_MT5391_AUD_3_DECODER
extern void DSP_SetTriDecMode(BOOL fgEnable);
#endif
extern void AUD_ClkCfgDualDecMode(BOOL fgEnable);
extern void AUD_DspSetSRSTSXTinputgain(UINT32 u4Mode);
//extern void DSP_LogAtvTvSys(void);
#if 0   // Unused
extern UINT32 AUD_AudioDescriptionChlSel(UINT8 u1DecId, AUD_CHL_DEF_T eChl);
#endif
extern AUD_TYPE_T DSP_GetAudChlInfo(UINT8 u1DecId);
extern TV_AUD_SYS_T DSP_GetAtvTvSystem(void);
extern void DSP_LogAtvTvSys(void);
extern UINT32 AUD_GetDspCacheHitRate(UINT8 u1DspId, UINT8 u1Cache);
extern void AUD_DumpUopCommand(void);
extern void AUD_DSPDumpIntHistory(UINT8 u1DspId);
extern INT32 AUD_Read32ByteID (UINT8 u1DspId, UINT8 *u1ID_p, UINT32 u4Num);
extern BOOL _AudFMRadioDetection(void);

#if (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD))
extern void DSP_SRSTSHDSurrLvl(UINT8 u1Mode);
extern void DSP_SRSTSHDInpg(UINT8 u1Mode);
extern void DSP_SRSTSHDOutg(UINT8 u1Mode);
extern void DSP_SRSTSHDSetSpkSize(UINT8 u1Mode);
extern void DSP_SRSTSHDSetTSCtrl(void);
extern void DSP_SRSTSHDTBLvl(UINT8 u1Mode);
extern void DSP_SRSTSHDDef(UINT8 u1Mode);
extern void DSP_SRSTSHDElev(UINT8 u1Mode);
extern void DSP_SRSTSHDFntCCtrl(UINT8 u1Mode);
extern void DSP_SRSTSHDFntSCtrl(UINT8 u1Mode);
extern void DSP_SRSTSHDMode(UINT32 u4Mode);
extern void DSP_SRSTSHDCOMPRLvl(UINT8 u1Mode);
//extern void DSP_SRSTSHDLEVINDEP(UINT8 u1Mode);
extern void DSP_SRSTSHDTBFLAG(UINT8 u1Mode,UINT8 u1Enable);
extern void DSP_SRSTSHDSetCustomCoef(UINT32 u4type, INT32 i4val);
extern void DSP_SRSTSHDSetCustomAnaSize(UINT8 u1Mode);
extern void DSP_SRSTSHDSetCustomSpkSize(UINT8 u1Mode);
extern void DSP_SRSTSHDSetAnaSize(UINT8 u1Mode);
extern void DSP_SRSTSHDSetHpfSize(UINT8 u1Mode);

#endif

#if (defined(DSP_SUPPORT_SRSCC3D))
extern void DSP_SRSCC3DFlag(UINT8 u1Mode,UINT8 u1Enable);
extern void DSP_SRSCC3DInMode(UINT8 u1Mode);
extern void DSP_SRSCC3DInpg(UINT8 u1Mode);
extern void DSP_SRSCC3DOutg(UINT8 u1Mode);
extern void DSP_SRSCC3DInMode(UINT8 u1Mode);
extern void DSP_SRSCC3D3DMode(UINT8 u1Mode);
extern void DSP_SRSCC3DSetHpfSize(UINT8 u1Mode);
extern void DSP_SRSCC3DSetIniSize(UINT8 u1Mode);
extern void DSP_SRSCC3DSetSpkSize(UINT8 u1Mode);
extern void DSP_SRSCC3DSetTSCtrl(void);
extern void DSP_SRSCC3DTBLvl(UINT8 u1Mode);
extern void DSP_SRSCC3DMode(UINT32 u4Mode);
extern void DSP_SRSCC3DCOMPRLvl(UINT8 u1Mode);
extern void DSP_SRSCC3DTBFLAG(UINT8 u1Mode,UINT8 u1Enable);
extern void DSP_SRSCC3DSetCustomCoef(UINT32 u4type, INT32 i4val);
extern void DSP_SRSCC3DSetCustomAnaSize(UINT8 u1Mode);
extern void DSP_SRSCC3DSetCustomSpkSize(UINT8 u1Mode);
extern void DSP_SRSCC3DSetAnaSize(UINT8 u1Mode);
extern void DSP_SRSCC3DSetMainTech(UINT8 u1Mode);
extern void DSP_SRSSRS3DCenterCtrl(UINT8 u1Mode);
extern void DSP_SRSSRS3DSpaceCtrl(UINT8 u1Mode);
extern void DSP_SRSSpaceCtrlTune(UINT8 u1Mode);
extern void DSP_SRSMixFadeCtrl(UINT8 u1Mode);
extern void DSP_SRSFrontCenterCtrl(UINT8 u1Mode);
extern void DSP_SRSFrontSpaceCtrl(UINT8 u1Mode);
extern void DSP_SRSRearSpaceCtrl(UINT8 u1Mode);
extern void DSP_SRSRearCenterCtrl(UINT8 u1Mode);
extern void DSP_SRSTruSurLev(UINT8 u1Mode);
extern void DSP_SRSMixFadeCtrlExt(UINT8 u1Mode);
extern void DSP_SRSFocusLev(UINT8 u1Mode);
extern void DSP_SRSLFocusLev(UINT8 u1Mode);
extern void DSP_SRSRFocusLev(UINT8 u1Mode);
extern void DSP_SRSLDefLev(UINT8 u1Mode);
extern void DSP_SRSRDefLev(UINT8 u1Mode);
extern void DSP_SRSCC3DSetIniDepth(UINT8 u1Mode);
extern void DSP_SRSRefLev(UINT8 u1Mode);
extern void DSP_SRSTbqLev(UINT8 u1Mode);
extern void DSP_SRSDcuLev(UINT8 u1Mode);

#endif


//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
extern void AUD_DspSetUploadMode(UINT8 u1Mode);
extern void AUD_DspUploadDataEnable(void);
extern void AUD_DataUpload_Init (void);
extern void AUD_DataUpload_start (void);
//#endif

#ifdef __MODEL_slt__ // by hotkey 0729 for compile
//Rice add for ADSP SLT
//extern void ADSP_SLT_Init(void);
//extern void ADSP_SLT_SetPatID(UINT8 u1PID);
extern INT32 SLT_SortingAud(void);
extern void SLT_FlashingAudInit(void);
extern INT32 SLT_DtvSorting(void);
extern INT32 SLT_AtvSorting(void);
extern INT32 SLT_IntAdSorting(void);
extern INT32 SLT_ROMSorting(void);
#ifndef CC_AUD_DISABLE_2ND_DSP
extern INT32 SLT_DSPBSorting(void);
#endif
#endif

#ifdef BURN_IN_AOUT_TEST
extern UINT32 AudGetAoutStatus(void);
extern void AudResetAoutStatus(void);
extern void AudSetAoutTest(void);
#endif

extern void AUD_SPDIF_Output_HDMI_Rx(BOOL fgEnable);
extern void AudPll1Setting(void);
extern void AudPll2Setting(SAMPLE_FREQ_T eFs);
extern void AudPllPowerDown(UINT8 PLL_ID,BOOL fgPD);
extern UINT16 bDrvVideoGetFrameDelay(UINT8 bPath);
extern AV_SYNC_MODE_T eDspGetSyncMode(UINT8 ucDecId);
#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
extern void AUD_PlayMuteCmdQuery(UINT8 u1DecId);
extern void AUD_PlayMuteClearHistory(UINT8 u1DecId);
extern void AUD_PlayMuteQueryDelayInfo(UINT8 u1DecId);
extern void AUD_PlayMuteEnable(BOOL fgEnable);
#endif
#ifdef CC_AUD_DDI
extern void AUD_WatchSpdifEnable(BOOL fgEnable);
#endif

extern UINT32 u4GetDspBinaryAddr(ADSP_BIN_T eAdspBin);
extern UINT32 u4GetDspBinarySize(ADSP_BIN_T eAdspBin);

//PTSQueue_V2
extern INT16 AUD_DrvPtsQueueFindInfo(UINT32 u4Wp);
extern void AUD_DrvPtsQueueQry(UINT8 u1DecID, UINT16 u2Size);
extern BOOL _fgEnableFMDetNotify;
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
extern BOOL _fgPsrDbg;
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
extern void AUD_DspSetCDNotchQandFc(UINT32 u4UserQIdx, UINT32 u4UserFc);
#endif

#ifdef CC_AUD_USE_NVM
extern UINT32 AUD_NVM_Operation(AUD_NVM_OP_T uOP, UINT8* puNvmBuffer, UINT16 u2Offset);
#endif

#if defined(CC_AUD_ARM11_SUPPORT)
extern void AudAprocJtagPinmux(void);
extern void AudAprocModuleCtrl(UINT32 u4Module, UINT32 u4Flag);
#endif // CC_AUD_ARM11_SUPPORT

//Speed Test
extern INT32 _AudMMPlaybackSpeedTestCmd(INT32 i4Argc, const CHAR ** szArgv);

#ifdef CC_DUAL_AUD_DEC_SUPPORT   
extern UINT8 gSubDecId;
extern UINT8 gMainDecId;
#endif


/******************************************************************************
* Declare the macros and reglist for RegTest functions
******************************************************************************/

#define AUD_BASE            AUDIO_BASE
#define AUD_REG_LENGTH      0

REG_TEST_T arAUDRgtList[] = {
    { 0x0000,   eRD_ONLY,   2,  0x00000007, 1,  0 },
    // End.
    { 0xffff, eNO_TYPE, -1 },
};

/* Declare the debug on/off/level and RegTest functions */
CLIMOD_DEBUG_FUNCTIONS(AUD)
//CLIMOD_REGTST_FUNCTIONS(AUD)
/* End of Declare */

/****************************************************************************
** Audio Command
****************************************************************************/

static INT32 _AudCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_Init();
    AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
    AUD_OutPadEnable(AUD_DEC_AUX, TRUE);
    ADAC_Mute(FALSE);

    return 1;
}

static INT32 _AudCmdSetDecType(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 ucCodeType;
    UINT8 ucStreamFrom;
    UINT8 u1DecId;
    UINT8 u1DspId;
    AUD_FMT_T   eAudDecFormat;
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;


    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1DecId = (UINT8)StrToInt(szArgv[2]);
    ucStreamFrom =  (UINT8)StrToInt(szArgv[3]);
    ucCodeType = (UINT8)StrToInt(szArgv[4]);

    if ((i4Argc != 5) ||(u1DecId >= AUD_DEC_NUM) ||(u1DspId >= AUD_DSP_NUM))
    {
        goto lbUsage;
    }
    // setup decode format
    eAudDecFormat = (AUD_FMT_T)ucCodeType;
    // stream from
    eAudDecStreamFrom = (AUD_DEC_STREAM_FROM_T)ucStreamFrom;
    AUD_SetDecType(u1DspId, u1DecId, eAudDecStreamFrom, eAudDecFormat);

    return 1;

lbUsage:
#ifdef CC_DUAL_AUD_DEC_SUPPORT   
    Printf("gMainDecId = %d, gSubDecId=%d\n\n",gMainDecId,gSubDecId);
#endif  
    Printf("Usage: setdec [Dsp id] [decoder id] [stream from] [stream type]\n");
    Printf("[DSP id]    0:DSP0  1:DSP1 \n");

#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD\n");
#else
    Printf("[decoder id]    0:MAIN  1:AUX \n");
#endif

    Printf("[stream from] 1:D-TUNER  2:A-TUNER 3:SPDIF 4.LINE-IN 5.HDMI 6.MEMORY (ex.Pink noise) 7:Reserve 8:INTERNAL 9:MM  10 : LINE-IN2 11: LINE-IN3 12: I2S-IN\n");
    Printf("[stream type] 0:INTERNAL (ex.Pink noise) 1:MPEG  2:AC3  3:PCM  4:MP3  5:AAC 6:DTSDec  \n");
    Printf("              7:WMA  11:MTS  12:CANAL+  13:PAL  14:A2  18:DETECTOR  20:LPCM  \n");
    Printf("              21:FMRDO, 22:FMRDODET, 23:SBCDEC , 24:SRCENC, 25:MP3ENC\n");
    Printf("              26:G711DEC, 27:G711ENC, 28:DRA, 29:COOK\n");
    Printf("              30:G729DEC, 31:VORBISDEC, 32:WMAPro 39:FLAC\n");
    return -1;
}

static INT32 _AudCmdDualDecMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag = FALSE;

    if (i4Argc < 2)
    {
        Printf("Usage: dual  [on/off] or q\n");
        Printf("[on/off]     1: on  \n");
        Printf("             0: off  \n");
        return -1;
    }

    if (i4Argc >= 2)
    {
        if (x_strcmp(szArgv[1], "on") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "1") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "off") == 0)
        {
            u1Flag = FALSE;
        }
        else if (x_strcmp(szArgv[1], "0") == 0)
        {
            u1Flag = FALSE;
        }
        else if (x_strcmp(szArgv[1], "q") == 0)
        {
            if(_IsDualDecMode())
            {
                Printf("Dual Decoder Mode: on\n");
            }
            else
            {
                Printf("Dual Decoder Mode: off\n");
            }
        }
        else
        {
              Printf("Usage: dual  [on/off] or q\n");
              Printf("[on/off]     1: on  \n");
              Printf("             0: off  \n");
              return -1;
        }
     }

    DSP_SetDualDecMode(u1Flag);
    AUD_ClkCfgDualDecMode(u1Flag);

    return 1;
}

#ifdef CC_MT5391_AUD_3_DECODER
static INT32 _AudCmdTriDecMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag = FALSE;

    if (i4Argc < 2)
    {
        Printf("Usage: tridec  [on/off] or q\n");
        Printf("[on/off]     1: on  \n");
        Printf("             0: off  \n");
        return -1;
    }

    if (i4Argc >= 2)
    {
        if (x_strcmp(szArgv[1], "on") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "1") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "off") == 0)
        {
            u1Flag = FALSE;
        }
        else if (x_strcmp(szArgv[1], "0") == 0)
        {
            u1Flag = FALSE;
        }
        else if (x_strcmp(szArgv[1], "q") == 0)
        {
            if(_IsTriOnlyDecMode())
            {
                Printf("Tri Decoder Mode: on\n");
            }
            else
            {
                Printf("Tri Decoder Mode: off\n");
            }
        }
        else
        {
              Printf("Usage: tridec  [on/off] or q\n");
              Printf("[on/off]     1: on  \n");
              Printf("             0: off  \n");
              return -1;
        }
     }

    DSP_SetTriDecMode(u1Flag);

    return 1;
}

#ifdef CC_AUD_LINEIN_USE_DEC3
static INT32 _AudCmdTriDecOnlyMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag = FALSE;

    if (i4Argc < 2)
    {
        Printf("Usage: tridec only [on/off]\n");
        Printf("[on/off]     1: on  \n");
        Printf("             0: off  \n");
        Printf("             %d\n",_IsTriOnlyDecMode());
        return -1;
    }

    if (i4Argc >= 2)
    {
        if (x_strcmp(szArgv[1], "on") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "1") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "off") == 0)
        {
            u1Flag = FALSE;
        }
        else if (x_strcmp(szArgv[1], "0") == 0)
        {
            u1Flag = FALSE;
        }
        else
        {
              Printf("Usage: tridec only [on/off]\n");
              Printf("[on/off]     1: on  \n");
              Printf("             0: off  \n");
              return -1;
        }
     }

    DSP_SetTriDecOnlyMode(u1Flag);

    return 1;
}
#endif
#endif

static INT32 _AudCmdResume(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId = AUD_DSP0;
    UINT8 u1DecId = AUD_DEC_MAIN;

    if ((i4Argc != 3) ||(u1DecId >= AUD_DEC_NUM) ||(u1DspId >= AUD_DSP_NUM))
        {
            goto lbUsage;
        }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1DecId = (UINT8)StrToInt(szArgv[2]);
    AUD_DSPCmdResume(u1DspId, u1DecId);


    return 1;

lbUsage:
     Printf("Usage: resume [DSP id] [decoder id]\n");
     Printf("[DSP id] 0:DSP0  1:DSP1 \n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD\n");
#else
    Printf("[decoder id]    0:MAIN  1:AUX \n");
#endif
    return -1;
}

static INT32 _AudCmdPause(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId = AUD_DSP0;
    UINT8 u1DecId = AUD_DEC_MAIN;

    if ((i4Argc != 3) ||(u1DecId >= AUD_DEC_NUM) ||(u1DspId >= AUD_DSP_NUM))
        {
            goto lbUsage;
        }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1DecId = (UINT8)StrToInt(szArgv[2]);
    AUD_DSPCmdPause(u1DspId, u1DecId);


    return 1;

lbUsage:
     Printf("Usage: pause [DSP id] [decoder id]\n");
     Printf("[DSP id] 0:DSP0  1:DSP1 \n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id]    0:MAIN  1:AUX 2:THIRD\n");
#else
    Printf("[decoder id]    0:MAIN  1:AUX \n");
#endif
    return -1;
}

static INT32 _AudCmdPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId;
    UINT8 u1DecId;


    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1DecId = (UINT8)StrToInt(szArgv[2]);

    if ((i4Argc != 3) ||(u1DecId >= AUD_DEC_NUM) ||(u1DspId >= AUD_DSP_NUM))
        {
            goto lbUsage;
        }

        AUD_DSPCmdPlay(u1DspId, u1DecId);
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521
    #if 1//def ENABLE_AUDIO_RECORD_SUPPORT
        if ((u1DecId == AUD_DEC_THIRD) )
    #else
        if (u1DecId == AUD_DEC_THIRD)
    #endif
        {
            AUD_DspADEnable(AUD_DEC_MAIN, TRUE);
        }
#endif

    return 1;

lbUsage:
     Printf("Usage: play [DSP id] [decoder id]\n");
     Printf("[DSP id] 0:DSP0  1:DSP1 \n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#endif
    return -1;
}

static INT32 _AudCmdStop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId;
    UINT8 u1DecId;

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1DecId = (UINT8)StrToInt(szArgv[2]);

    if ((i4Argc != 3) ||(u1DecId >= AUD_DEC_NUM) ||(u1DspId >= AUD_DSP_NUM))
        {
            goto lbUsage;
        }
    AUD_DSPCmdStop(u1DspId, u1DecId);

#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521
    #if 1 //def ENABLE_AUDIO_RECORD_SUPPORT
        if ((u1DecId == AUD_DEC_THIRD))
    #else
        if (u1DecId==AUD_DEC_THIRD)
    #endif
        {
            AUD_DspADEnable(AUD_DEC_MAIN, FALSE);
        }
#endif


    return 1;

lbUsage:
     Printf("Usage: stop [DSP id] [decoder id]\n");
     Printf("[DSP id] 0:DSP0  1:DSP1 \n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#endif
    return -1;
}

static INT32 _AudCmdQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1DspId;

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1DecId = (UINT8)StrToInt(szArgv[2]);
    
    if ((i4Argc != 3) ||(u1DecId >= AUD_DEC_NUM) ||(u1DspId >= AUD_DSP_NUM))
        {
            goto lbUsage;
        }

    if (u1DecId == 9)
    {
        int i;
        for (i=0;
             #ifdef CC_AUD_4_DECODER_SUPPORT
             i<4
             #elif defined(CC_MT5391_AUD_3_DECODER)
             i<3
             #else
             i<2
             #endif
             ;i++)
        {
            AudShowStatus(u1DspId, i);
        }
    }
    else
    {
        AudShowStatus(u1DspId, u1DecId);
    }

    return 1;

lbUsage:
    Printf("Usage: query [DSP id][decoder id]\n");
    Printf("[DSP id]    0:DSP0  1:DSP1 \n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id] 0:MAIN 1:AUX 2:THIRD 3:4TH 9: ALL\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id] 0:MAIN 1:AUX 2:THIRD 9: ALL\n");
#else
    Printf("[decoder id] 0:MAIN 1:AUX 9: ALL\n");
#endif
    return -1;
}

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
static INT32 _AudParserDebug(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;

    if (i4Argc < 2)
    {
        Printf("Usage: pd  [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    fgEnable = (BOOL)StrToInt(szArgv[1]);
    _fgPsrDbg = fgEnable;
    return 1;
}
#endif

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
static INT32 _AudOssAdap_Cmd(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_OSS_ADAP_MSG_T tMsg;
    UINT32 devId;
    UINT32 cmd;
    UINT32 dataAddr = 0;
    UINT32 dataSize = 0;

    if(i4Argc != 3)
    {
        Printf("[_AudOssAdap_Cmd] argc error. \n");
        goto lbUsage;
    }


    devId = (UINT32)StrToInt(szArgv[1]);
    cmd   = (UINT32)StrToInt(szArgv[2]);

    tMsg.cmd = cmd;
    tMsg.devId = devId;
    tMsg.dataAddr = dataAddr;
    tMsg.dataSize = dataSize;

    _AudOssAdap_SendMsg(tMsg);

    return 0;

    lbUsage:
    Printf("Usage: cmd [devId] [cmd]\n");

    Printf("[devId] 0: DEVICE_OUT_USB_HEADPHONE, 1: DEVICE_OUT_BLUETOOTH_EARPHONE\n");
    Printf("[cmd] \n");
    Printf("0: AUD_OSS_ADAP_CONNECT\n");
    Printf("1: AUD_OSS_ADAP_START\n");
    Printf("2: AUD_OSS_ADAP_TRANSFER\n");
    Printf("3: AUD_OSS_ADAP_STOP\n");
    Printf("4: AUD_OSS_ADAP_DISCONNECT\n");

    return -1;
}
static INT32 _AudOssAdap_Ops(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 devId;

    if(i4Argc != 2)
    {
        Printf("[_AudOssAdap_Ops] argc error. \n");
        goto lbUsage;
    }

    devId = (UINT32)StrToInt(szArgv[1]);
    _AudOssAdap_Enable(devId);

    return 0;

    lbUsage:
    Printf("Usage: e [devId]\n");
    Printf("[devId] 0: DEVICE_OUT_USB_HEADPHONE, 1: DEVICE_OUT_BLUETOOTH_EARPHONE, 2: BUILD-IN SPEAKER\n");

    return -1;
}

static INT32 _AudOssAdap_SetSrc(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 srcType;

    if(i4Argc != 2)
    {
        Printf("[_AudOssAdap_SetSrc] argc error. \n");
        goto lbUsage;
    }

    srcType = (UINT32)StrToInt(szArgv[1]);

    _AudOssAdap_SetSourceType(srcType);
    return 0;

    lbUsage:
    Printf("Usage: s [srcType]\n");
    Printf("[srcType] 0: MIXSOUND, 1: UPLOADER\n");

    return -1;
}




#endif

static INT32 _AudCmdDspVer(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc == 1)
    {
       AudShowDspVer(AUD_DSP0, 0);
       AudShowDspVer(AUD_DSP1, 0);
    }
    else
    {
       AudShowDspVer(AUD_DSP0, 1);
       AudShowDspVer(AUD_DSP1, 1);
    }
    return 1;
}

#ifdef CC_AUD_4_DECODER_SUPPORT

#define USE_UPLOAD_PATH_TEST

extern UINT32 u4GetSBCEncFIFOStart(void);
extern UINT32 u4GetSBCEncFIFOEnd(void);
extern UINT32 u4GetSBCEncWritePnt(void);
static INT32 _AudCmdSBCEncTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr;
    UINT32 u4Size;
    #ifdef USE_UPLOAD_PATH_TEST
    UINT32 u4Pts,u4SrcAddr,u4SrcSize;
    #else
    UINT32 u4FifoSA;
    UINT32 u4FifoEA;
    UINT32 u4RP, u4WP;
    #endif

    if (i4Argc != 3)
    {
        Printf("Usage: sbcd [output addr] [output size]\n");
        return -1;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    u4Size = (UINT32)StrToInt(szArgv[2]);

#ifdef USE_UPLOAD_PATH_TEST

    AUD_DspSetUploadMode(3);
    while (u4Size)
    {
        AUD_GetUploadInfo(&u4Pts, &u4SrcAddr, &u4SrcSize);
        if (u4SrcSize)
        {
            x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                        (VOID*)(VIRTUAL(u4SrcAddr)),
                        u4SrcSize);
            u4Addr += u4SrcSize;
            u4Size -= u4SrcSize;
            Printf(".");
        }
        else
        {
            x_thread_delay(10);
        }
    }
    AUD_DspSetUploadMode(0);
#else
    u4FifoSA = u4GetSBCEncFIFOStart();
    u4FifoEA = u4GetSBCEncFIFOEnd();
    u4RP = u4FifoSA;

    AUD_SetDecType(AUD_DEC_4TH, AUD_STREAM_FROM_OTHERS, AUD_FMT_SBCENC);
    AUD_DSPCmdPlay(AUD_DEC_4TH);

    while (u4Size)
    {
        u4WP = u4GetSBCEncWritePnt();
        if ((u4WP >= u4FifoSA) && (u4WP < u4FifoEA)) //validity check
        {
            if (u4RP < u4WP)
            {
                if ((u4WP - u4RP) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
            else if (u4RP > u4WP)
            {
                if (((u4FifoEA - u4FifoSA) - (u4RP - u4WP)) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
        }

        x_thread_delay(10);
    }
    AUD_DSPCmdStop(AUD_DEC_4TH);

#endif

    return 1;
}
#endif



static INT32 _AudCmdEncMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    AUD_ENC_MODE_T eEncMode;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1DecId =  (UINT8)StrToInt(szArgv[1]);

    if (u1DecId >= AUD_DEC_NUM)
    {
        goto lbUsage;
    }

    if (i4Argc == 2)
    {
        DSP_GetEncMode(&u1DecId, &eEncMode);
        Printf("enc source = %d\n",u1DecId);
        Printf("enc mode = %d\n",eEncMode);
    }
    else
    {
        // Set AV sync mode
        eEncMode = (AUD_ENC_MODE_T)((UINT8)StrToInt(szArgv[2]));
        AUD_DspSetEncMode(u1DecId, (AUD_ENC_MODE_T)eEncMode);
    }

    return 1;

lbUsage:
    Printf("Usage: enc [decoder id] [enc mode]\n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#endif
    Printf("[enc mode]  %d:OFF %d:BEFORE_POST %d:AFTER_POST\n",
        (UINT8)AUD_ENC_MODE_OFF, (UINT8)AUD_ENC_MODE_INPUT_BEFORE_POST_PROCESSING, (UINT8)AUD_ENC_MODE_INPUT_AFTER_POST_PROCESSING);
    return -1;
}


static INT32 _AudCmdSyncMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1SyncMode;
    UINT32 u4DspSyncmode;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1DecId =  (UINT8)StrToInt(szArgv[1]);

    if (u1DecId >= AUD_DEC_NUM)
    {
        goto lbUsage;
    }

    if (i4Argc == 2)
    {
        // Get AV sync mode
        AV_SYNC_MODE_T eSyncMode;
        eSyncMode = AUD_GetAvSynMode(u1DecId);
        Printf("Driver Setting :\n");
        if (eSyncMode == AV_SYNC_FREE_RUN)
        {
            Printf("Dec %d: Free run\n", u1DecId);
        }
        else if (eSyncMode == AV_SYNC_SLAVE)
        {
            Printf("Dec %d: System master\n", u1DecId);
        }
        else
        {
            Printf("Dec %d: Audio master\n", u1DecId);
        }
        Printf("DSP Setting:\n");

        u4DspSyncmode = u4ReadDspSram(AUD_DSP0, SRAM_SYSTEM_SETUP);
        u4DspSyncmode = u4DspSyncmode>>23;  //check dsp sram bit16

        if ( u4DspSyncmode == 1)
        {
           Printf("Dec %d: System master or Free run\n", u1DecId);
        }
        else
        {
           Printf("Dec %d: Audio master\n",u1DecId);
        }

        if((( eSyncMode == AV_SYNC_AUDIO_MASTER ) && ( u4DspSyncmode != 0)) || (( (eSyncMode == AV_SYNC_SLAVE) || (eSyncMode == AV_SYNC_FREE_RUN) ) && (u4DspSyncmode != 1) ) )
        {
           Printf("!!Driver and DSP setting not match!!\n");
        }

    }
    else
    {
        // Set AV sync mode
        u1SyncMode = (UINT8)StrToInt(szArgv[2]);
        AUD_SetAvSynMode(u1DecId, (AV_SYNC_MODE_T)u1SyncMode);
    }

    return 1;

lbUsage:
    Printf("Usage: sync [decoder id] [sync mode]\n");
#ifdef CC_AUD_4_DECODER_SUPPORT
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD 3:4TH\n");
#elif defined (CC_MT5391_AUD_3_DECODER)
    Printf("[decoder id] 0:MAIN  1:AUX  2:THIRD \n");
#else
    Printf("[decoder id] 0:MAIN  1:AUX \n");
#endif
    Printf("[sync mode]  %d:FREE RUN %d:SLAVE MODE %d:AUDIO MASTER\n",
        (UINT8)AV_SYNC_FREE_RUN, (UINT8)AV_SYNC_SLAVE, (UINT8)AV_SYNC_AUDIO_MASTER);
    return -1;
}

static INT32 _AudCmdSetLogPeriod(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4LogPeriod;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4LogPeriod = (UINT32)StrToInt(szArgv[1]);
    DSP_SetAudLogPeriod(u4LogPeriod);

    return 1;

lbUsage:
    Printf("Usage: sp [second]\n");
    return -1;
}

static INT32 _AudCmdSetStream(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1DecId;
    UINT32 u4StreamAddr;
    UINT32 u4StreamLen;
    MEM_BUFFER_INFO_T rMemBuf;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u4StreamAddr = (UINT32)StrToInt(szArgv[2]);
    u4StreamLen = (UINT32)StrToInt(szArgv[3]);

    rMemBuf.pData = (UINT8 *)u4StreamAddr;
    rMemBuf.u4Length = u4StreamLen;

    AUD_SetDecMemBuffer(AUD_DSP0, u1DecId, &rMemBuf);

    return 1;

lbUsage:
    Printf("Usage: str [decoder id] [addr] [len]\n");
    return -1;
}



/****************************************************************************
** spdif Command
****************************************************************************/

static INT32 _SpdifCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    SPDIF_InInit();

    return 1;

}

static INT32 _SpdifCmdReset(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    //TODO

    return 1;

}

static INT32 _SpdifCmdEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    //TODO

    return 1;

}

static INT32 _SpdifCmdDisable(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    //TODO

    return 1;

}

static INT32 _SpdifCmdCheckLock(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if(SPDIF_InLockCheck())
        Printf("Lock\n");
    else
        Printf("UnLock\n");

    return 1;
}

static INT32 _SpdifCmdSelectCh(INT32 i4Argc, const CHAR ** szArgv)
{

    UINT8 ucChannel;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    ucChannel = (UINT8)StrToInt(szArgv[1]);
    ucChannel = (ucChannel & ~0xf8);

    SPDIF_InChSel(ucChannel);

    return 1;

lbUsage:
    Printf("selectch [channel]\n");
    return -1;
}

static INT32 _SpdifCmdReadReg(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    SPDIF_InRegisterDump();

    return 1;
}

static INT32 _SpdifCmdWriteReg(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr;
    UINT8  ucData;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    ucData = (UINT8)StrToInt(szArgv[2]);

#define AK4114_DEV_ADDR    0x10
#define SIF_CLK_DIV 0x100

    SIF_Write(SIF_CLK_DIV, AK4114_DEV_ADDR, u4Addr, &ucData, 1);

    return 1;

lbUsage:
    Printf("Usage: writerg [addr] [data]\n");
    return -1;
}

static INT32 _SpdifIecSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecCfg;
    UINT8 u1IecEnable;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1IecCfg = (UINT8)StrToInt(szArgv[1]);
    u1IecEnable =  (UINT8)StrToInt(szArgv[2]);

    AUD_DspIECConfig((AUD_IEC_T)u1IecCfg, (BOOL)u1IecEnable);

    return 1;

lbUsage:
    Printf("Usage: iecs [iec mode] [enable]\n");
    Printf("[iec mode]  0:PCM 1:RAW \n");
    Printf("[enable]  0:OFF 1:ON \n");
    return -1;
}

//extern void vSetIecChannel(AUD_IEC_CH_T eIecChannel);
static INT32 _SpdifIecChannelSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecChannel;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1IecChannel = (UINT8)StrToInt(szArgv[1]);

    UNUSED(AUD_DspIecChannel((AUD_IEC_CH_T)u1IecChannel));
    //vSetIecChannel((AUD_IEC_CH_T)u1IecChannel); //for CLI only, originally this CLI will only take effect when stop then play decoder

    return 1;

lbUsage:
    Printf("Usage: iecc [iec mode]\n");
    Printf("[channel] 0:L_R  1:LS_RS 2:C_SW 3:7_8 4:LINE_IN 5:9_10 6:AUX\n");
    return -1;
}

extern void vIecQuery(void);
static INT32 _SpdifIecQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    vIecQuery();
    return 1;
}

static INT32 _SpdifSetRawDelayFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Flag =  (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetIecRawFlag(u1Flag);

    return 1;

lbUsage:
    Printf("Usage: flag [control]\n");
    Printf("[control] 0 = Not sync to channel delay, 1 = sync to channel delay\n");
    return -1;
}

static INT32 _SpdifSetRawDelay(INT32 i4Argc, const CHAR ** szArgv)
{
    INT16 i2Delay;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    // handle '-' for negative
    if ((UINT8)*szArgv[1] == '-')
    {
        i2Delay =  -(INT16)StrToInt(szArgv[1] + 1);
    }
    else
    {
        i2Delay =  (INT16)StrToInt(szArgv[1]);
    }

    AUD_DspSetIecRawDelay(i2Delay);

    return 1;

lbUsage:
    Printf("Usage: delay [delay]\n");
    Printf("[control] delay value in ms\n");
    return -1;
}

static char cStramFromName[AUD_STREAM_FROM_NUM][18] = {
"Others       ",
"Digital Tuner",
"Analog Tuner ",
"SPDIF In     ",
"Line In      ",
"HDMI In      ",
"Memory       ",
"Buffer Agent ",
"Feeder       ",
"MultiMedia   "
};

static char cSpdifRegModeName[SPDIF_REG_TYPE_NUM][18] = {
"Default",
"User",
};

static void _vSpdifListInstruction (void)
{
    int i;

    Printf("[input source]\n");
    for (i = 0; i < AUD_STREAM_FROM_NUM; i ++)
    {
        Printf("    %d = %s\n", i, &cStramFromName[i][0]);
    }
}

static INT32 _SpdifRegTypeReset(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);

    AUD_DspResetSpdifReg (eInputSrc);

    return 1;

lbUsage:
    Printf("Usage: reg [input source]\n");
    _vSpdifListInstruction();
    return -1;
}

static INT32 _SpdifCopyrightSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecCopyright;
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    u1IecCopyright = (UINT8)StrToInt(szArgv[2]);
#ifdef CC_AUD_SUPPORT_SPDIF_V20
    AUD_DspSetSpdifCopyright(eInputSrc, u1IecCopyright);
#else
    AUD_DspSetSpdifCopyright(SPDIF_REG_TYPE_USER, eInputSrc, u1IecCopyright);
#endif

    return 1;

lbUsage:
    Printf("Usage: copy [input source] [copyright]\n");
    _vSpdifListInstruction();
    Printf("[copyright]\n");
    Printf("    0 = copyright is asserted, 1 = no copyright is asserted\n");
    return -1;
}

static INT32 _SpdifCategoryCodeSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecCategoryCode;
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    u1IecCategoryCode = (UINT8)StrToInt(szArgv[2]);
    AUD_DspSetSpdifCategoryCode(eInputSrc, u1IecCategoryCode);

    return 1;

lbUsage:
    Printf("Usage: code [input source] [category code]\n");
    _vSpdifListInstruction();
    return -1;
}

static INT32 _SpdifWordLengthSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecWordLength;
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    u1IecWordLength = (UINT8)StrToInt(szArgv[2]);
    AUD_DspSetSpdifWordLength(eInputSrc, u1IecWordLength);

    return 1;

lbUsage:
    Printf("Usage: word [input source] [word length]\n");
    _vSpdifListInstruction();
    Printf("[word length]\n");
    Printf("    0 = 24 bits, 2 = 20 bits, 3 = 16 bits\n");
    return -1;
}

static INT32 _SpdifSetInfo (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Copyright, u1CategoryCode;
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    u1Copyright =  (UINT8)StrToInt(szArgv[2]);
    u1CategoryCode =  (UINT8)StrToInt(szArgv[3]);
    UNUSED(AUD_DspSpdifSetInfo(eInputSrc, u1Copyright, u1CategoryCode));

    return 1;

lbUsage:
    Printf("Usage: set [input source] [copy right] [category code]\n");
    _vSpdifListInstruction();
    return -1;
}

static INT32 _SpdifSetProtect (INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL bCpBit, bLbit;
    AUD_DEC_STREAM_FROM_T eInputSrc;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    bCpBit =  (BOOL)StrToInt(szArgv[2]);
    bLbit =  (BOOL)StrToInt(szArgv[3]);
    UNUSED(AUD_DspSpdifSetCopyProtect(eInputSrc, bCpBit, bLbit));

    return 1;

lbUsage:
    Printf("Usage: protect [input source] [cp bit] [L bit]\n");
    _vSpdifListInstruction();
    return -1;
}

static INT32 _SpdifUpdateModeSet(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_DEC_STREAM_FROM_T eInputSrc;
    UINT8 eUpdate;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    eInputSrc =  (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[1]);
    eUpdate = (UINT8)StrToInt(szArgv[2]);
    AUD_DspSetSpdifUpdateMode(eInputSrc, eUpdate);

    return 1;

lbUsage:
    Printf("Usage: copy [input source] [update mode]\n");
    _vSpdifListInstruction();
    Printf("[update mode]\n");
    Printf("    0 = by setup, 1 = by source content\n");
    return -1;
}

static INT32 _SpdifGetChannelStatus(INT32 i4Argc, const CHAR ** szArgv)
{
    SPDIF_CHANNEL_STATUS_T info;

    if (i4Argc < 1)
    {
        goto lbUsage;
    }

    AUD_DspGetSpdifChannelStatus ((SPDIF_CHANNEL_STATUS_T *) &info);

    Printf ("\nSPDIF channel status\n");
    printf ("=============================================\n");
    Printf ("Mode                   0x%02x\n", info.u1Mode);
    Printf ("SampleMode             0x%02x\n", info.u1SampleMode);
    Printf ("Copyright              0x%02x\n", info.u1Copyright);
    Printf ("Pre-emphasis           0x%02x\n", info.u1PreEmphasis);
    Printf ("Category code          0x%02x (L-bit = 0x%x)\n", info.u1Category, (info.u1Category >> 7) & 0x1);
    Printf ("Source number          0x%02x\n", info.u1SourceNum);
    Printf ("Channel number         0x%02x\n", info.u1CannelNum);
    Printf ("Sampling rate          0x%02x\n", info.u1SampleRate);
    Printf ("Clock accuracy         0x%02x\n", info.u1ClockAccuray);
    Printf ("Word length            0x%02x\n", info.u1WordLength);
    Printf ("Original sample rate   0x%02x\n", info.u1OrgSampleRate);
    printf ("=============================================\n");
    return 1;

lbUsage:
    Printf("Usage: cs\n");
    return -1;
}

static INT32 _SpdifGetInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    SPDIF_REG_TYPE_T eRegType;
    UINT8 u1CopyRight, u1CategoryCode, u1Word;
    UINT32 i;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    eRegType = (SPDIF_REG_TYPE_T)StrToInt(szArgv[1]);

    if (eRegType >= SPDIF_REG_TYPE_NUM)
    {
        goto lbUsage;
    }

    Printf ("\nSPDIF reg Info %d (%s):\n", eRegType, &cSpdifRegModeName[eRegType][0]);
    Printf ("-----------------------------------------------------------------\n");
    Printf ("Source               copyright       category code   word length\n");
    Printf ("-----------------------------------------------------------------\n");
    for (i = 0; i < AUD_STREAM_FROM_NUM; i ++)
    {
        u1CopyRight = AUD_DspGetSpdifCopyright(eRegType, (AUD_DEC_STREAM_FROM_T) i);
        u1CategoryCode = AUD_DspGetSpdifCategoryCode(eRegType, (AUD_DEC_STREAM_FROM_T) i);
        u1Word = AUD_DspGetSpdifWordLength(eRegType, (AUD_DEC_STREAM_FROM_T) i);
        if (AUD_DspGetSpdifUpdateMode(eRegType, (AUD_DEC_STREAM_FROM_T) i) == IEC_CHANNEL_STATUS_BY_SETUP)
        {
            Printf ("%2d %s     0x%02X            0x%02X            0x%02X\n",
                i, &cStramFromName[i][0], u1CopyRight, u1CategoryCode, u1Word);
        }
        else
        {
            Printf ("%2d %s(S)  0x%02X            0x%02X            0x%02X\n",
                i, &cStramFromName[i][0], u1CopyRight, u1CategoryCode, u1Word);
        }
    }
    Printf ("-----------------------------------------------------------------\n");
    Printf ("(S) = upadted by input source content.\n");

    return 1;

lbUsage:
    Printf("Usage: info [reg type]\n");
    Printf("[reg mode]\n");
    for (i = 0; i < SPDIF_REG_TYPE_NUM; i ++)
    {
        Printf("%d = %s\n", i, &cSpdifRegModeName[i][0]);
    }
    return -1;
}

static INT32 _SpdifAlwaysOutHdmi(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1IecBypass;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1IecBypass = (UINT8)StrToInt(szArgv[1]);

    UNUSED(AUD_SPDIF_Output_HDMI_Rx((BOOL)u1IecBypass));

    return 1;

lbUsage:
    Printf("Usage: iecb [enable]\n");
    Printf("[enable]    0: normal mode, 1: hdmi always bypass to spdif mode\n");
    return -1;
}


/****************************************************************************
** adac Command
****************************************************************************/
static INT32 _AdacCmdQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    ADAC_SpeakerStateQuery();

    return 1;
}

static INT32 _AdacCmdDataFormat(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Format;
    UINT8 u1DecId;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Format = (UINT8)StrToInt(szArgv[2]);

    AUD_AoutFormat(u1DecId, (DATA_FORMAT_T)u1Format);

    return 1;

lbUsage:
    Printf("Usage: format [decoder id] [format]\n");
    Printf("[decoder id] 0:MAIN  1:AUX \n");
    Printf("[format]     0:FORMAT_RJ  1:FORMAT_LJ 2:FORMAT_I2S \n");
    return -1;
}

static INT32 _AdacCmdDataInvert(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Invert;
    UINT8 u1DecId;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Invert = (UINT8)StrToInt(szArgv[2]);

    AUD_AoutInvertData(u1DecId, (BOOL)u1Invert);
    return 1;

lbUsage:
    Printf("Usage: datainvert [decoder id] [format]\n");
    Printf("[decoder id] 0:MAIN  1:AUX \n");
    Printf("[invert]     0:Not Invert  1:Invert\n");
    return -1;

}

static INT32 _AdacCmdDrvCur(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1MckDrv;
    UINT8 u1BckDrv;
    UINT8 u1LRckDrv;
    UINT8 u1AOSDATADrv;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    u1DecId  =  (UINT8)StrToInt(szArgv[1]);
    u1MckDrv =  (UINT8)StrToInt(szArgv[2]);
    u1BckDrv =  (UINT8)StrToInt(szArgv[3]);
    u1LRckDrv =  (UINT8)StrToInt(szArgv[4]);
    u1AOSDATADrv =  (UINT8)StrToInt(szArgv[5]);


    AUD_AoutDrvCur(u1DecId, u1MckDrv, u1BckDrv ,u1LRckDrv , u1AOSDATADrv);

    return 1;

lbUsage:
    Printf("Usage: drv [decoder id] [MCLK driving] [BCLK driving] [LRCLK driving] [AOSDATA driving]\n");
    Printf("[decoder id] 0:MAIN  1:AUX \n");
    Printf("[MCLK driving]  2 ~ 8 (mA)\n");
    Printf("[BCLK driving]  2 ~ 8 (mA)\n");
    Printf("[LRCLK driving]  2 ~ 8 (mA)\n");
    Printf("[AOSDATA driving]  2 ~ 8 (mA)\n");

    return -1;
}

static INT32 _AdacCmdCodecRead(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 u1Addr;
    UINT8 u1Data;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Addr = (UINT8)StrToInt(szArgv[1]);
    u1Data = ADAC_CodecRegRead(u1Addr);

    Printf("(Add, Data) = (0x%x, 0x%x)\n", u1Addr, u1Data);

    return 1;

lbUsage:
    Printf("Usage: cr [addr]\n");
    return -1;
}

static INT32 _AdacCmdCodecWrite(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 u1Addr;
    UINT8 u1Data;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1Addr = (UINT8)StrToInt(szArgv[1]);
    u1Data = (UINT8)StrToInt(szArgv[2]);

    ADAC_CodecRegWrite(u1Addr, u1Data);

    return 1;

lbUsage:
    Printf("Usage: cw [addr] [data]\n");
    return -1;
}

static INT32 _AdacCmdCodecChlSel(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Sel;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Sel = (UINT8)StrToInt(szArgv[1]);

    ADAC_CodecAdChlSel(u1Sel);

    return 1;

lbUsage:
    Printf("Usage: sel [sel]\n");
    return -1;
}

static INT32 _AdacCmdAmpInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    ADAC_AmpInit();

    return 1;
}

static INT32 _AdacCmdAmpMute(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Data;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1Data = (UINT8)StrToInt(szArgv[1]);
    ADAC_AmpMute(u1Data);

    return 1;

lbUsage:
    Printf("Usage: am [flag]\n");
    Printf("[flag] 1 : mute    0 : unmute\n");
    return -1;
}


static INT32 _AdacCmdAmpRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Addr;
    UINT8 u1Data;
    UINT32 u4Num;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }
    else if (i4Argc == 2)
    { 
        u1Addr = (UINT8)StrToInt(szArgv[1]);
        u1Data = ADAC_AmpRegRead(u1Addr); 
        Printf("(Add, Data) = (0x%x, 0x%x)\n", u1Addr, u1Data);
    }
    else
    {
        u1Addr = (UINT8)StrToInt(szArgv[1]);
        u4Num = (UINT8)StrToInt(szArgv[2]);

        for ( ; u4Num > 0; u4Num--)
        {
            u1Data = ADAC_AmpRegRead(u1Addr);
            Printf("(Add, Data) = (0x%x, 0x%x)\n", u1Addr, u1Data);
            u1Addr++;
        } 
    }

    return 1;

lbUsage:
    Printf("Usage: ar [addr] [num]\n");
    return -1;
}

static INT32 _AdacCmdAmpWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Addr;
    UINT8 u1Data;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1Addr = (UINT8)StrToInt(szArgv[1]);
    u1Data = (UINT8)StrToInt(szArgv[2]);

    ADAC_AmpRegWrite(u1Addr, u1Data);

    return 1;

lbUsage:
    Printf("Usage: aw [addr] [data]\n");
    return -1;
}

extern void _NTP_RegWriteN(UINT8 u1WordAddr, UINT32 u4Data, UINT16 u2ByteCnt); 
extern UINT32 _NTP_RegReadN(UINT8 u1WordAddr, UINT16 u2ByteCnt);

static INT32 _AdacCmdAmpReadN(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Addr;
    UINT32 u4Data;
    UINT32 u4Num;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }
    else if (i4Argc == 2)
    { 
        u1Addr = (UINT8)StrToInt(szArgv[1]);
        u4Data = _NTP_RegReadN(u1Addr, 4); 
        Printf("(Add, Data) = (0x%x, 0x%x)\n", u1Addr, u4Data);
    }
    else
    {
        u1Addr = (UINT8)StrToInt(szArgv[1]);
        u4Num = (UINT8)StrToInt(szArgv[2]);

        for ( ; u4Num > 0; u4Num--)
        {
            u4Data = _NTP_RegReadN(u1Addr, 4); 
            Printf("(Add, Data) = (0x%x, 0x%x)\n", u1Addr, u4Data); 
            u1Addr++;
        } 
    }

    return 1;

lbUsage:
    Printf("Usage: nr [addr] [num]\n");
    return -1;
}

static INT32 _AdacCmdAmpWriteN(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Addr;
    UINT32 u4Data;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1Addr = (UINT8)StrToInt(szArgv[1]);
    u4Data = (UINT32)StrToInt(szArgv[2]);

    _NTP_RegWriteN(u1Addr, u4Data, 4);

    return 1;

lbUsage:
    Printf("Usage: nw [addr] [data]\n");
    return -1;
}

static INT32 _AdacCmdAmpRamRead(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 u1AmpID;
   UINT8 u1Bank;
   UINT8 u1Addr;
   UINT8 u1Data[3];
   UINT8 i;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    u1AmpID = (UINT8)StrToInt(szArgv[1]);
    u1Bank = (UINT8)StrToInt(szArgv[2]);
    u1Addr = (UINT8)StrToInt(szArgv[3]);

    if (ADAC_DigiAmp_ReadRam((AUD_AMP_ID_T)u1AmpID, u1Bank, u1Addr, u1Data, FALSE))
    {
        for (i=0; i<3; i++)
        {
            Printf("0x%x ",u1Data[i]);
        }
        Printf("\n");
    }

    return 0;
lbUsage:
    Printf("Usage: arr [AmpID] [Bank] [Addr]\n");
    return -1;
}

static INT32 _AdacCmdAmpRamWrite(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 u1AmpID;
   UINT8 u1Bank;
   UINT8 u1Addr;
   AUD_AQ_AMP_RAM_DATA_T eAmpData;

    if (i4Argc < 7)
    {
        goto lbUsage;
    }

    u1AmpID = (UINT8)StrToInt(szArgv[1]);
    u1Bank = (UINT8)StrToInt(szArgv[2]);
    u1Addr = (UINT8)StrToInt(szArgv[3]);
    eAmpData.u1Data[0] = (UINT8)StrToInt(szArgv[4]);
    eAmpData.u1Data[1] = (UINT8)StrToInt(szArgv[5]);
    eAmpData.u1Data[2] = (UINT8)StrToInt(szArgv[6]);

    if (ADAC_DigiAmp_WriteRam((AUD_AMP_ID_T)u1AmpID, u1Bank, u1Addr, eAmpData, FALSE))
    {
        Printf("Write Addr 0x%x Succrss.\n", u1Addr);
    }

    return 0;
lbUsage:
    Printf("Usage: arr [AmpID] [Bank] [Addr] [Data1] [Data2] [Data3]\n");
    return -1;
}


#if 0
static INT32 _AdacCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    return 1;
}
#endif

static INT32 _InterDacChlSel(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT32 u4Channel;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[InterDac Channel Setting]\n");
            ADAC_DacGetChl();
            return 1;
        }
        Printf("Usage: ch [DAC id] [channel] or ch q\n");
        Printf("[DAC id]  0:FIRST  1:SECOND  2: THIRD 3: 4TH\n");
        Printf("[channel #]      0:L/R,     1:LS/RS,   2:C/Subwoofer\n");
        Printf("                 3:IEC,     4:HP,      5:Monitor\n");
        Printf("                 6:EXT1314, 7:AUX\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u4Channel = (AUD_CH_T)StrToInt(szArgv[2]);
    ADAC_DacSetChl(u1DecId, (AUD_CHL_DEF_T)u4Channel);
    return 1;
}

#ifdef XY_RAM_TEST
static INT32 _InterDacXYRamTest(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgxy;
    if (i4Argc < 2)
    {
        Printf("Usage: X Y Ram select [s]\n");
        Printf("[s]   0 : X Ram test\n");
        Printf("      1 : Y Ram test\n");
    }
    fgxy = (StrToInt(szArgv[1]) == 1) ? TRUE : FALSE;
    _MT5398_XYRamTest(fgxy);
    return 1;
}
static INT32 _InterDacXYRamRead(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgxy;
    if (i4Argc < 2)
    {
        Printf("Usage: X Y Ram select [s]\n");
        Printf("[s]   0 : X Ram test\n");
        Printf("      1 : Y Ram test\n");
        return -1;
    }
    fgxy = (StrToInt(szArgv[1]) == 1) ? TRUE : FALSE;
    _MT5398_XYRamRead(fgxy);

    return 1;
}
#endif

static INT32 _InterPWMDACYRAMRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 staddr;
    UINT8 length;
    if (i4Argc < 3)
    {
        Printf("Usage: pwmr [staddr] [length]\n");
        return -1;
    }
    else
    {
        staddr = (UINT8)StrToInt(szArgv[1]);
        length = (UINT8)StrToInt(szArgv[2]);
        _MT5398_YRamRead(staddr,length);
    }
    return 1;
} 

static INT32 _InterPWMDACYRAMWrite(INT32 i4Argc, const CHAR ** szArgv)
{

    INT8 pgroup;
    UINT32 YRAM_Addr;
    UINT32 YRAM_Data;
    if (i4Argc < 4)
    {
        Printf("Usage: pwmw [groupID][address] [data]\n");
        return -1;
    }
    else
    {
        pgroup = (INT8)(StrToInt(szArgv[1]));
        YRAM_Addr = (UINT8)StrToInt(szArgv[2])+0x100;
        YRAM_Data = (UINT32)StrToInt(szArgv[3]);
        _MT5398_YRAMWrite(pgroup,YRAM_Addr,YRAM_Data);
    }
    return 1;
}

static INT32 _InterPWMDACRead(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8 pgroup;
    UINT32 pwmaddr;
    if (i4Argc < 3)
    {
        Printf("Usage: dacr [group ID] [address]\n");
        return -1;
    }
    else
    {
        pgroup = (INT8)(StrToInt(szArgv[1]));
        pwmaddr = (UINT32)StrToInt(szArgv[2]);
         _MT5398_InterPWMDACRead(pgroup,pwmaddr);

    }
    return 1;
}

static INT32 _InterPWMDACWrite(INT32 i4Argc, const CHAR ** szArgv)
{

    INT8 pgroup;
    UINT32 PWMDAC_Addr;
    UINT32 PWMDAC_Data;
    if (i4Argc < 4)
    {
        Printf("Usage: pwmw [groupID][address 0x**] [data 0x**]\n");
        return -1;
    }
    else
    {
        pgroup = (INT8)(StrToInt(szArgv[1]));
        PWMDAC_Addr = (UINT32)StrToInt(szArgv[2]);
        PWMDAC_Data = (UINT32)StrToInt(szArgv[3]);
        _MT5398_InterPWMDACWrite(pgroup,PWMDAC_Addr,PWMDAC_Data);
    }
    return 1;
}

static INT32 _InterPWMSigGen(INT32 i4Argc,const CHAR ** szArgv)
{
    INT8 SigType;
    if (i4Argc < 2)
    {
        Printf("Usage: pwmsg [signal type]\n");
        Printf("1: 1K Tone\n");
        Printf("2: 2K Tone\n");
        Printf("3: 3K Tone\n");
        Printf("4: 4K Tone\n");
        Printf("5: 6K Tone\n");
        Printf("6: 8K Tone\n");
        Printf("7: 12K Tone\n");
        Printf("8: 16K Tone\n");
        Printf("9: Duty~=100%\n");
        Printf("10: Duty~=75%\n");
        Printf("11: Duty~=0%\n");
        Printf("12: Duty~=25%\n");
        return -1;
    }
    else
    {
        SigType = (INT8)(StrToInt(szArgv[1]));
        _MT5398_PWMSigGen(SigType);
        return 1;
    }
}



static INT32 _InterDacMute(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fMute;
    UINT8 u1ALRx;

    if (i4Argc < 3)
    {
        Printf("Usage: mute [ALRx] [flag]\n");
        Printf("[ALRx] 0:ALR0   1:ALR1   2:ALR2   3:ALR3   4:ALL ALR\n");
        Printf("[flag] 1 : mute   0 : unmute\n");
        return -1;
    }
    u1ALRx = (INT8)StrToInt(szArgv[1]);
    fMute = (StrToInt(szArgv[2]) == 1) ? TRUE : FALSE;

    if(u1ALRx < 4)
    {
        _MT5398_MuteALRx(u1ALRx,fMute);
    }
    else if(u1ALRx == 4)
    {
        _MT5398_Mute(0, fMute);
    }
    return 1;
}
static INT32 _InterPWMEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;
    if(i4Argc < 2)
    {
        Printf("Usage: pwm [flag]\n");
        Printf("[flag] 1:enable   0:disable\n");
        return -1;
    }
    fgEnable = (StrToInt(szArgv[1]) == 1)? TRUE:FALSE;
    _MT5398_InterPWMEnable(fgEnable);
    return 1;
}


static INT32 _InterPWMGain(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 pgain;
    INT8 pgroup;
    if(i4Argc < 3)
    {
        Printf("current PWMDAC digital gain:g1 = 0x%06x\n",_MT5398_InterPWMGainGet(1));
        Printf("                            g2 = 0x%06x\n",_MT5398_InterPWMGainGet(2));
        Printf("                            g3 = 0x%06x\n",_MT5398_InterPWMGainGet(3));
        Printf("Usage: pg [groupID] [value]\n");
        Printf("[groupID]: 1, 2, 3, 5:all group \n");
        Printf("[value]: 0x000000 ~ 0xffffff (+:0x0~0x7fffff   -:0x100000~0xffffff)\n");
        return -1;
    }
    else
    {
        pgroup = (INT8)(StrToInt(szArgv[1]));
        pgain = (INT32)(StrToInt(szArgv[2]));
        _MT5398_InterPWMGainSet(pgroup, pgain);
    }
    return 1;
}

static INT32 _InterMonitorSet(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8 pgroup;
    INT8 Monitor_ID;
    if(i4Argc < 3)
    {
        Printf("Usage: mt [GroupID] [Monitor ID]\n");
        Printf("[groupID]: 1, 2, 3 \n");
        Printf("[value 1]: Monitor A\n");
        Printf("[value 2]: Monitor B\n");

        return -1;
    }
    else
    {
        pgroup = (INT8)(StrToInt(szArgv[1]));
        Monitor_ID = (INT8)(StrToInt(szArgv[2]));
        _MT5398_InterMonitorSet(pgroup, Monitor_ID);
        return 1;
    }    
}

static INT32 _InterPWMDelay(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8 pgroup;
    INT32 pdelay;
    INT32 pdelay_reg;
    INT32 pwmdelay;
    if(i4Argc < 3)
    {
        Printf("Usage: pd [GroupID] [degree value=0 to 359]\n");
        return -1;
    }
    else
    {
        pgroup = (INT8)(StrToInt(szArgv[1]));
        pdelay = (INT32)(StrToInt(szArgv[2]));
        pdelay_reg=(pdelay*10)/48;
        pwmdelay = (INT32)(pdelay_reg+0x600);
        _MT5398_InterPWMDelay(pgroup, pwmdelay);
        return 1;
    }    
}


static INT32 _InterLDOEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;
    if(i4Argc < 2)
    {
        Printf("Usage: ldo [flag]\n");
        Printf("[flag] 1:enable   0:disable\n");
        return -1;
    }
    fgEnable = (StrToInt(szArgv[1]) == 1)? TRUE:FALSE;
    _MT5398_InterLDOEnable(fgEnable);
    return 1;
}

static INT32 _InterDacEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fEnable;

    if (i4Argc < 2)
    {
        Printf("Usage: enable [flag]\n");
        Printf("[flag]      1 : enable internal dac\n");
        Printf("            0 : disable internal dac\n");
    }

    fEnable = (StrToInt(szArgv[1]) == 1) ? TRUE : FALSE;
    _MT5398_InterDacEnable(fEnable);
    return 1;
}

static INT32 _InterDacCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    _MT5398_Init();
    return 1;
}

static INT32 _InterDacLRInvert(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fMute;
    UINT16 u1DACId;

    UNUSED(fMute);
    UNUSED(u1DACId);

    if (i4Argc < 3)
    {
        Printf("Usage: inv [DAC ID][flag]\n");
        Printf("[DAC ID]  0:FIRST  1:SECOND  2: THIRD  3:ALL\n");
        Printf("[flag]      1 : invert internal dac\n");
        Printf("            0 : no-invert internal dac\n");
    }

    u1DACId = (UINT8)StrToInt(szArgv[1]);

    fMute = (StrToInt(szArgv[2]) == 1) ? TRUE : FALSE;
    return 1;
}

static INT32 _AADCMute(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fMute;
    UINT8 u1CH;

    if (i4Argc < 3)
    {
        Printf("Usage: mute [ch] [flag]\n");
        Printf("[ch] 0:L   1:R   2:L & R\n");
        Printf("[flag] 1 : mute   0 : unmute\n");
        return -1;
    }
    u1CH = (INT8)StrToInt(szArgv[1]);
    fMute = (StrToInt(szArgv[2]) == 1) ? TRUE : FALSE;

    if(u1CH <= 2)
    {
        _MT5398_AADCMute(u1CH,fMute);
    }
    return 1;
}


static INT32 _AADCDown(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgDown;
    UINT8 u1CH;

    if (i4Argc < 3)
    {
        Printf("Usage: pd [ch] [flag]\n");
        Printf("[ch] 0:L   1:R   2:L & R\n");
        Printf("[flag] 1 : power down   0 : power on\n");
        return -1;
    }
    u1CH = (INT8)StrToInt(szArgv[1]);
    fgDown = (StrToInt(szArgv[2]) == 1) ? TRUE : FALSE;

    if(u1CH <= 2)
    {
        _MT5398_AADCDOWNCH(u1CH,fgDown);
    }

    return 1;
}


static INT32 _AADCGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Value;
    UNUSED(u4Value);

    if (i4Argc < 2)
    {
        Printf("Usage: gain [value]or q\n");
        Printf("[value]: shift[30:25]:0~7; factor[25:0]:0~0x1FFFFFF(+),0x2000000~0x2FFFFFF(-)\n");
        return -1;
    }

    if (szArgv[1][0] == 'q')
    {
        _MT5398_ADCGainQuery();
        return 1;
    }
    u4Value = (UINT32)StrToInt(szArgv[1]);
    _MT5398_SetADCGain(u4Value);

    return 1;
}

#if 0 //def __MODEL_slt__
/****************************************************************************
** DSP SLT Command
****************************************************************************/
static INT32 _ADSPSLTInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("_ADSPSLTInit\n");
    ADSP_SLT_Init();
    return 1;
}

static INT32 _ADSPSLTTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1PatID;
    enum
    {
        ADSP_TEST_PATTERNNULL           = 0,
        ADSP_TEST_PATTERN1,
        ADSP_TEST_PATTERN2,
        ADSP_TEST_PATTERN3,
        ADSP_TEST_PATTERN4,
        ADSP_TEST_PATTERN5,
        ADSP_TEST_PATTERN6,
        ADSP_PATTERN_CNT                = ADSP_TEST_PATTERN6
    };


    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u1PatID = (UINT8)StrToInt(szArgv[1]);

    if ( (u1PatID <= ADSP_TEST_PATTERNNULL) || (u1PatID>ADSP_PATTERN_CNT) )
    {
        goto lbUsage;
    }

    ADSP_SLT_SetPatID(u1PatID);

    return 1;

lbUsage:
    Printf("Usage: t    [pattern no.]                       \n");
    Printf("[pattern no.]       1 DSP Internal SRAM Test    \n");
    Printf("                    2 DSP Instruction Test      \n");
    Printf("                    3 DSP DRAM Interface Test   \n");
    Printf("                    4 Extra Test #1             \n");   //Getbits
    Printf("                    5 Extra Test #2             \n");   //Putbits
    Printf("                    6 Extra Test #3             \n");   //CORDIC

    return -1;

}
#endif

#ifdef CC_AUD_ARM_SUPPORT
/****************************************************************************
** audio processor Command
****************************************************************************/
extern void vAprocMain_Init (void);
extern UINT32 AUD_ProcGetPc (void);
extern UINT32 u4ReadProc(UINT32 u4Addr);
extern void vWriteProc(UINT32 u4Addr, UINT32 u4Data);
extern void vAprocCmdOpen (UINT32 u4Idx);
extern void vAproc_NotifyAdsp(UINT32 u4Item, UINT32 u4Type, UINT32 u4Data);
extern UINT32 u4AprocReg_Read (UINT32 u4Offset);
extern void vAprocReg_Write (UINT32 u4Offset, UINT32 u4Val);
extern void _vAUD_Aproc_Set (UINT32 u4Type, UINT32 u4Item, UINT32 *pu4Addr, UINT32 u4Num);
extern void _vAUD_Aproc_Get (UINT32 u4Type, UINT32 u4Item, UINT32 *pu4Addr, UINT32 u4Num);
extern void vAproc_Enable(UINT32 u4Enable);
extern UINT32 u4Aproc_IsEnable(void);
extern void vAproc_SetEnable(void);
extern UINT32 u4AprocGetWorkSpaceSize (void);
extern UINT32 u4GetAprocMemoryMap (UINT32 u4Type);
extern UINT32 u4AprocReadBIM(UINT32 u4Addr);
extern void vAprocWriteBIM(UINT32 u4Addr, UINT32 u4Value);

#define APROC_RST (0x908)

static INT32 _AprocProcnit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("_AprocProcnit\n");

    vADSPTaskInit(); // current stage, it must be initialized after ADSP init
    vAprocMain_Init();

    return 1;

}

static INT32 _AprocProcPC(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("aproc pc = %x\n", AUD_ProcGetPc());

    return 1;
}

static INT32 _AprocRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, u4Count = 1, i = 0;
    UINT32 u4Tmp;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    if (i4Argc == 3)
    {
        u4Count= (UINT16)StrToInt(szArgv[2]);
    }

    while (i < u4Count)
    {
        if ((i%4) == 0)
        {
            u4Tmp = u4ReadProc(u4Addr);
            Printf("\n(0x%04x) | %08x ", u4Addr, u4Tmp);
        }
        else
        {
            u4Tmp = u4ReadProc(u4Addr);
            Printf("%08x ", u4Tmp);
        }
        u4Addr+=4;
        i++;
    }

    return 1;

lbUsage:
    Printf("Usage: reads [addr] [cnt]\n");
    return -1;
}

static INT32 _AprocWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Addr;
    UINT32  u4Data;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    u4Data = (UINT32)StrToInt(szArgv[2]);

    vWriteProc (u4Addr, u4Data);

    return 1;

lbUsage:
    Printf("Usage: writes [addr] [data]\n");
    return -1;
}

static INT32 _AprocPostProcVol (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Ch;
    UINT32  u4Data, u4Data0, u4Data1, u4Data2, u4Data3;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Ch = (UINT32)StrToInt(szArgv[1]);
    u4Data = (UINT32)StrToInt(szArgv[2]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_VOL, u4Ch, (UINT32 *) &u4Data, 1);

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_SP, (UINT32 *) &u4Data0, 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_SP_LSRS, (UINT32 *) &u4Data1, 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_SPSW, (UINT32 *) &u4Data2, 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_HP, (UINT32 *) &u4Data3, 1);
    
    Printf("Current: SP vol = 0x%x\n", u4Data0);
    Printf("         SPLsRs vol = 0x%x\n", u4Data1);
    Printf("         SPSW vol = 0x%x\n", u4Data2);
    Printf("         HP vol =0x%x\n", u4Data3);
    Printf("Usage: pv [ch] [vol]\n");
    Printf("ch => SP:0, SPLsRs:1, SPSW:2, HP:3\n");
    Printf("vol => 0~0x7fffff\n");    
    return -1;
}


#define ADSP_GAIN_DB(gain) ((gain/20)-18)

const char FILTER_NAME[APROC_BIQUAD_TYPE_APF+1][16]={"LPF", "HPF", "Peaking EQ", "LOW Shelf", "High Shelf", "BPF", "APF"};
static UINT32 u4CtrlTmp;
#define GET_CTRL(flag) (flag & 1) 
#define SET_CTRL(grp, idx, flag)    do{\
                                    _vAUD_Aproc_Get (grp, idx, (UINT32 *) &u4CtrlTmp, 1);\
                                    u4CtrlTmp = ((u4CtrlTmp & 0xfffffffe) | flag);\
                                    _vAUD_Aproc_Set(grp, idx, (UINT32 *) &u4CtrlTmp, 1);\
                                    }while(0);

static INT32 _AprocPostProcBassTreble (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Ch, u4Flag, u4tmp;
    APROC_DSP_BIQ_INFO_T  iBiq; 
    const char *Argv;

    if (i4Argc == 6) 
    {
        u4Ch = (UINT32)StrToInt(szArgv[1]);
        iBiq.u4Type= (UINT32)StrToInt(szArgv[2]);
        iBiq.u4Fc= (UINT32)StrToInt(szArgv[3]);
        iBiq.u4Q= (UINT32)StrToInt(szArgv[4]);        
        Argv = szArgv[5];
        if (Argv[0] == '-')
        {
            u4tmp = (UINT32)StrToInt(&Argv[1]);
            u4tmp = (UINT32)0xFFFFFFFF - (u4tmp-1);
            iBiq.i4Gain= (INT32)ADSP_BIQ_DB((INT32)u4tmp);            
        }
        else
        {
            iBiq.i4Gain= (INT32)ADSP_BIQ_DB((UINT32)StrToInt(szArgv[5]));
        }        

        Printf("\n\niBiq.i4Gain = %d\n\n",iBiq.i4Gain);


        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_BASSTREBLE, u4Ch, (UINT32 *) &iBiq, 4);

        return 1;
    }
    else if (i4Argc == 3) 
    {
        u4Ch = (UINT32)StrToInt(szArgv[1]);
        u4Flag = (UINT32)StrToInt(szArgv[2]);
        SET_CTRL (APROC_CONTROL_TYPE_BASSTREBLE, u4Ch, u4Flag);

        return 1;
    }
    else
    {
        goto lbUsage;
    }

    
lbUsage:    
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_SP, (UINT32 *) &iBiq, 4);
    Printf("SP Bass: Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_TREBLE_SP, (UINT32 *) &iBiq, 4);
    Printf("SP Treble: Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_SP_LSRS, (UINT32 *) &iBiq, 4);
    Printf("SP LsRs Bass Treble: Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_TREBLE_SP_LSRS, (UINT32 *) &iBiq, 4);
    Printf("SP LsRs Treble: Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n",FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_HP, (UINT32 *) &iBiq, 4);
    Printf("HP Bass: Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_TREBLE_HP, (UINT32 *) &iBiq, 4);
    Printf("HP Treble: Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));    

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP, (UINT32 *) &u4Flag, 1);
    Printf("SP BassTreble flag: %d\n", GET_CTRL(u4Flag));  
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_SP_LSRS, (UINT32 *) &u4Flag, 1);
    Printf("SP LsRs BassTreble flag: %d\n", GET_CTRL(u4Flag));       
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BASSTREBLE, APROC_DSP_BIQ_ID_BASS_TREBLE_FLAG_HP, (UINT32 *) &u4Flag, 1);
    Printf("HP BassTreble flag: %d\n", GET_CTRL(u4Flag));   

    Printf("\nUsage1: pbt [ch set] [type] [fc] [q] [gain]\n");
    Printf("\tch set => Bass SP:0, Bass SP LsRs:1, Bass HP:2\n");
    Printf("\tch set => Treble SP:3, Treble SP LsRs:4, Treble HP:5\n");
    Printf("\tch set => Bass SP:0, Bass SP LsRs:1, Bass HP:2\n");
    Printf("\tch set => Treble SP:3, Treble SP LsRs:4, Treble HP:5\n");
    Printf("\ttype => LPF = 0, HPF = 1, PEAKINGEQ = 2, LOWSHELF = 3,\n");
    Printf("\ttype => HIGHSHELF = 4 BPF = 5, APF = 6\n");
    Printf("\tq => ONE is 0x3fffff\n");
    Printf("\tGain => how many dB\n");

    Printf("\nUsage2: pbt [ch set] [flag]\n");
    Printf("\tch set => SP flag:6, SP LsRs Flag:7, HP Flag: 8\n");
    Printf("\tflag => On:1, Off:0/n");
    
    return -1;
}

static INT32 _AprocPostProcChSel (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Ch, u4Flag, u4tmp;
    APROC_DSP_BIQ_INFO_T  iBiq; 
    const char *Argv;
    const char vMode[3][4] = {"4.0","2.2", "2.1"};

    if (i4Argc == 6) 
    {
        u4Ch = (UINT32)StrToInt(szArgv[1]);
        iBiq.u4Type= (UINT32)StrToInt(szArgv[2]);
        iBiq.u4Fc= (UINT32)StrToInt(szArgv[3]);
        iBiq.u4Q= (UINT32)StrToInt(szArgv[4]);        
        Argv = szArgv[5];
        if (Argv[0] == '-')
        {
            u4tmp = (UINT32)StrToInt(&Argv[1]);
            u4tmp = (UINT32)0xFFFFFFFF - (u4tmp-1);
            iBiq.i4Gain= (INT32)ADSP_BIQ_DB((INT32)u4tmp);            
        }
        else
        {
            iBiq.i4Gain= (INT32)ADSP_BIQ_DB((UINT32)StrToInt(szArgv[5]));
        }        

        Printf("\nType: %d, Fc: %d, Q: 0x%x, Gain %d\n\n",iBiq.u4Type, iBiq.u4Fc, iBiq.u4Q, iBiq.i4Gain);


        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_CHSEL, u4Ch, (UINT32 *) &iBiq, 4);

        return 1;
    }
    else if (i4Argc == 2) 
    {
        u4Flag = (UINT32)StrToInt(szArgv[1]);
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_CHSEL, APROC_DSP_BIQ_ID_CHSEL_MODE, (UINT32 *) &u4Flag, 1);

        return 1;
    }
    else
    {
        goto lbUsage;
    }

    
lbUsage:    
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_CHSEL, APROC_DSP_BIQ_ID_HPF_SP, (UINT32 *) &iBiq, 4);
    Printf("\nHPF SP: Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_CHSEL, APROC_DSP_BIQ_ID_LPF_SP_LSRS, (UINT32 *) &iBiq, 4);
    Printf("LPF LsRs: Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_CHSEL, APROC_DSP_BIQ_ID_CHSEL_MODE, (UINT32 *) &u4Flag, 1);
    Printf("Current output mode: %d -> (%s mode)", u4Flag, vMode[u4Flag]);
    
    
    Printf("\n\nUsage1: chs [ch set] [type] [fc] [q] [gain]\n");
    Printf("\tch set => LPF SP:0, HPF SP LsRs:1\n");
    Printf("\ttype => LPF = 0, HPF = 1, PEAKINGEQ = 2, LOWSHELF = 3,\n");
    Printf("\ttype => HIGHSHELF = 4 BPF = 5, APF = 6\n");
    Printf("\tq => ONE is 0x3fffff\n");
    Printf("\tGain => how many dB\n");

    Printf("\nUsage2: chs [mode]\n");
    Printf("\tmode => 4.0: %d, 2.2: %d, 2.1: %d\n",APROC_SPK_OUTPUT_4_0,APROC_SPK_OUTPUT_2_2,APROC_SPK_OUTPUT_2_1);
    
    return -1;
}

static INT32 _AprocPostProc3Drc (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Ch, u4Flag, u4tmp,u4Item;
    APROC_DSP_BIQ_INFO_T  iBiq; 
    const char *Argv;

    if (i4Argc == 6) 
    {
        u4Ch = (UINT32)StrToInt(szArgv[1]);
        iBiq.u4Type= (UINT32)StrToInt(szArgv[2]);
        iBiq.u4Fc= (UINT32)StrToInt(szArgv[3]);
        iBiq.u4Q= (UINT32)StrToInt(szArgv[4]);        
        Argv = szArgv[5];
        if (Argv[0] == '-')
        {
            u4tmp = (UINT32)StrToInt(&Argv[1]);
            u4tmp = (UINT32)0xFFFFFFFF - (u4tmp-1);
            iBiq.i4Gain= (INT32)ADSP_BIQ_DB((INT32)u4tmp);            
        }
        else
        {
            iBiq.i4Gain= (INT32)ADSP_BIQ_DB((UINT32)StrToInt(szArgv[5]));
        }        

        Printf("\nType: %d, Fc: %d, Q: 0x%x, Gain %d\n\n",iBiq.u4Type, iBiq.u4Fc, iBiq.u4Q, iBiq.i4Gain);


        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_DRC, u4Ch, (UINT32 *) &iBiq, 4);

        return 1;
    }
    else if (i4Argc == 3) 
    {
        u4Item = (UINT32)StrToInt(szArgv[1]);
        u4Flag = (UINT32)StrToInt(szArgv[2]);
        if(u4Item==APROC_DSP_BIQ_ID_3DRC_FLAG)
        {
            SET_CTRL (APROC_CONTROL_TYPE_DRC, u4Item, u4Flag);
        }
        else
        {
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_DRC, u4Item, (UINT32 *) &u4Flag, 1);
        }

        return 1;
    }
    else
    {
        goto lbUsage;
    }

    
lbUsage:    
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_3DRC_0, (UINT32 *) &iBiq, 4);
    Printf("\nBand 0 (LPF): Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_3DRC_1, (UINT32 *) &iBiq, 4);
    Printf("Band 1 (HPF): Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_3DRC_2_1, (UINT32 *) &iBiq, 4);
    Printf("Band 2 (filter 1): Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_3DRC_2_2, (UINT32 *) &iBiq, 4);
    Printf("Band 2 (filter 2): Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));


    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_3DRC_TARGET0, (UINT32 *) &u4Flag, 1);
    Printf("Current Target 0: 0x%x (0dB is 0x7ffff)\n", u4Flag);

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_3DRC_TARGET1, (UINT32 *) &u4Flag, 1);
    Printf("Current Target 1: 0x%x (0dB is 0x7ffff)\n", u4Flag);

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_3DRC_TARGET2, (UINT32 *) &u4Flag, 1);
    Printf("Current Target 2: 0x%x (0dB is 0x7ffff)\n", u4Flag);

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_3DRC_FLAG, (UINT32 *) &u4Flag, 1);
    Printf("3 Band DRC on/off: %d\n", GET_CTRL(u4Flag));


    Printf("\n\nUsage1: 3drc [ch set] [type] [fc] [q] [gain]\n");
    Printf("\tch set => Band 0 (LPF): %d, Band 1 (HPF): %d\n",APROC_DSP_BIQ_ID_3DRC_0,APROC_DSP_BIQ_ID_3DRC_1);
    Printf("\tch set => Band 2 (filter 1): %d, Band 2 (filter 2): %d\n",APROC_DSP_BIQ_ID_3DRC_2_1,APROC_DSP_BIQ_ID_3DRC_2_2);
    Printf("\ttype => LPF = 0, HPF = 1, PEAKINGEQ = 2, LOWSHELF = 3,\n");
    Printf("\ttype => HIGHSHELF = 4 BPF = 5, APF = 6\n");
    Printf("\tq => ONE is 0x3fffff\n");
    Printf("\tGain => how many dB\n");

    Printf("\nUsage2: 3drc [item] [val]\n");
    Printf("\titem => Target 0: %d, Target 1: %d, Target 2: %d\n",APROC_DSP_BIQ_ID_3DRC_TARGET0,APROC_DSP_BIQ_ID_3DRC_TARGET1,APROC_DSP_BIQ_ID_3DRC_TARGET2);
    Printf("\titem => On/off: %d\n",APROC_DSP_BIQ_ID_3DRC_FLAG );
    Printf("\tval => Target 0dB is 0x7ffff; On:1, Off: 0\n");
    
    return -1;
}


static INT32 _AprocPostProc1Drc (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Flag, u4tmp,u4Item;
    APROC_DSP_BIQ_INFO_T  iBiq; 
    const char *Argv;

    if (i4Argc == 5) 
    {
        iBiq.u4Type= (UINT32)StrToInt(szArgv[1]);
        iBiq.u4Fc= (UINT32)StrToInt(szArgv[2]);
        iBiq.u4Q= (UINT32)StrToInt(szArgv[3]);        
        Argv = szArgv[4];
        if (Argv[0] == '-')
        {
            u4tmp = (UINT32)StrToInt(&Argv[1]);
            u4tmp = (UINT32)0xFFFFFFFF - (u4tmp-1);
            iBiq.i4Gain= (INT32)ADSP_BIQ_DB((INT32)u4tmp);            
        }
        else
        {
            iBiq.i4Gain= (INT32)ADSP_BIQ_DB((UINT32)StrToInt(szArgv[5]));
        }        

        Printf("\nType: %d, Fc: %d, Q: 0x%x, Gain %d\n\n",iBiq.u4Type, iBiq.u4Fc, iBiq.u4Q, iBiq.i4Gain);


        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_1DRC_0, (UINT32 *) &iBiq, 4);

        return 1;
    }
    else if (i4Argc == 3) 
    {
        u4Item = (UINT32)StrToInt(szArgv[1]);
        u4Flag = (UINT32)StrToInt(szArgv[2]);
        if(u4Item==APROC_DSP_BIQ_ID_1DRC_FLAG)
        {
            SET_CTRL (APROC_CONTROL_TYPE_DRC, u4Item, u4Flag);
        }
        else
        {
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_DRC, u4Item, (UINT32 *) &u4Flag, 1);
        }

        return 1;
    }
    else
    {
        goto lbUsage;
    }

    
lbUsage:    
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_1DRC_0, (UINT32 *) &iBiq, 4);
    Printf("\nBand 0: Type=%s, Fc=%d, Q=0x%x, Gain=%ddB\n", FILTER_NAME[iBiq.u4Type], iBiq.u4Fc, iBiq.u4Q, ADSP_GAIN_DB(iBiq.i4Gain));
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_1DRC_TARGET0, (UINT32 *) &u4Flag, 1);
    Printf("Current Target 0: 0x%x (0dB is 0x7ffff)\n", u4Flag);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DRC, APROC_DSP_BIQ_ID_1DRC_FLAG, (UINT32 *) &u4Flag, 1);
    Printf("1 Band DRC on/off: %d\n", GET_CTRL(u4Flag));


    Printf("\n\nUsage1: 1drc [type] [fc] [q] [gain]\n");
    Printf("\ttype => LPF = 0, HPF = 1, PEAKINGEQ = 2, LOWSHELF = 3,\n");
    Printf("\ttype => HIGHSHELF = 4 BPF = 5, APF = 6\n");
    Printf("\tq => ONE is 0x3fffff\n");
    Printf("\tGain => how many dB\n");

    Printf("\nUsage2: 1drc [item] [val]\n");
    Printf("\titem => Target 0: %d\n",APROC_DSP_BIQ_ID_1DRC_TARGET0);
    Printf("\titem => On/off: %d\n",APROC_DSP_BIQ_ID_1DRC_FLAG );
    Printf("\tval => Target 0dB is 0x7ffff; On:1, Off: 0\n");
    
    return -1;
}

static INT32 _AprocPostProcMstVolume(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0, u4v1;

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_SP, (UINT32 *) &u4v0, 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_HP, (UINT32 *) &u4v1, 1);

    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);

        if ( u4type > 1 )
        {
            u4type = 0;
            Printf ("!Wrong Type!\n");
        }
        
        u4type = ((u4type == 0) ? APROC_IOCTRL_VOL_SP : APROC_IOCTRL_VOL_HP);
        
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_VOL, u4type , (UINT32 *) &u4v0, 1);
        
        return 1;
    }   
    else
    {
    
        Printf("\nMaster Volume Setting at Aproc:\n");
        Printf(" SPK = 0x%x, HP = 0x%x\n", u4v0, u4v1);

        Printf("\nUsage: mv  [type][v0]\n");
        Printf("type = 0 -> SP or 1 -> HP\n");
        Printf("v0   = 0 ~ 0x7fffff\n");
        return -1;
    }
}

static INT32 _AprocPostProcInputTrim(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0, u4v1, u4v2;

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_INPUT0, (UINT32 *) &u4v0, 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_INPUT1, (UINT32 *) &u4v1, 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_INPUT2, (UINT32 *) &u4v2, 1);

    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);

        if ( u4type > 2 )
        {
            u4type = 0;
            Printf ("!Wrong Type!\n");
        }
        
        u4type += APROC_IOCTR_TRIM_INPUT0;
        
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_TRIM, u4type , (UINT32 *) &u4v0, 1);
        
        return 1;
    }   
    else
    {
    
        Printf("\nInput Trim Setting at Aproc:\n");
        Printf(" Input0= 0x%x\n", u4v0);
        Printf(" Input1= 0x%x\n", u4v1);
        Printf(" Input2= 0x%x\n", u4v2);

        Printf("\nUsage: ipt  [type][v0]\n");
        Printf("type = 0 -> Input0 ; 1 -> Input1 ; 2 -> Input2\n");
        Printf("v0   = 0 ~ 0x7fffff\n");
        return -1;
    }

}

static INT32 _AprocPostProcAmixerTrim(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32  u4type, u4v0, u4v1, u4v2;

	_vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_AMIXER0, (UINT32 *) &u4v0, 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_AMIXER1, (UINT32 *) &u4v1, 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_AMIXER2, (UINT32 *) &u4v2, 1);
    //_vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_AMIXER2_AD, (UINT32 *) &u4v3, 1);

	if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);

        if ( u4type > 3 )
        {
            u4type = 0;
            Printf ("!Wrong Type!\n");
        }
        
        u4type += APROC_IOCTR_TRIM_AMIXER0;
        
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_TRIM, u4type , (UINT32 *) &u4v0, 1);
        
        return 1;
    }   
    else
    {
    
        Printf("\nAmixer Trim Setting at Aproc:\n");
        Printf(" Amixer0= 0x%x\n", u4v0);
        Printf(" Amixer1= 0x%x\n", u4v1);
        Printf(" Amixer2= 0x%x\n", u4v2);
		//Printf(" Amixer2_AD= 0x%x\n", u4v3);

        Printf("\nUsage: apt  [type][v0]\n");
        Printf("type = 0 -> Amixer0 ; 1 -> Amixer1 ; 2 -> Amixer2\n");
        Printf("v0   = 0 ~ 0x7fffff\n");
        return -1;
    }
}

static INT32 _AprocPostProcOutputTrim(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0, u4OutputTrim[8];

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_SP, (UINT32 *) &u4OutputTrim[0], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_SPLSRS, (UINT32 *) &u4OutputTrim[1], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_SPSW, (UINT32 *) &u4OutputTrim[2], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_HP, (UINT32 *) &u4OutputTrim[3], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_MON, (UINT32 *) &u4OutputTrim[4], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_IEC, (UINT32 *) &u4OutputTrim[5], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_SCART, (UINT32 *) &u4OutputTrim[6], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_TRIM, APROC_IOCTR_TRIM_BT, (UINT32 *) &u4OutputTrim[7], 1);

    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);

        if ( u4type > 7 )
        {
            u4type = 0;
            Printf ("!Wrong Type!\n");
        }
        
        u4type += APROC_IOCTR_TRIM_SP;
        
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_TRIM, u4type , (UINT32 *) &u4v0, 1);
        
        return 1;
    }   
    else
    {
    
        Printf("\nOutput Trim Setting at Aproc:\n");
        Printf(" SP_LR   = 0x%x\n", u4OutputTrim[0]);
        Printf(" SP_LSRS = 0x%x\n", u4OutputTrim[1]);
        Printf(" SP_C/SW = 0x%x\n", u4OutputTrim[2]);
        Printf(" HP      = 0x%x\n", u4OutputTrim[3]);
        Printf(" Monitor = 0x%x\n", u4OutputTrim[4]);
        Printf(" SPDIF   = 0x%x\n", u4OutputTrim[5]);
        Printf(" Scart   = 0x%x\n", u4OutputTrim[6]);
        Printf(" BT      = 0x%x\n", u4OutputTrim[7]);

        Printf("\nUsage: opt  [type][v0]\n");
        Printf("type = 0 -> SP L/R\n");
        Printf("type = 1 -> SP LS/RS\n");
        Printf("type = 2 -> SP C/SW\n");
        Printf("type = 3 -> HP\n");
        Printf("type = 4 -> Monitor\n");
        Printf("type = 5 -> SPDIF\n");
        Printf("type = 6 -> Scart\n");
        Printf("type = 7 -> BT\n");
        Printf("v0   = 0 ~ 0x7fffff\n");
        return -1;
    }

}

static INT32 _AprocPostProcOutputVol(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0, u4OutputVol[8];

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_SP, (UINT32 *) &u4OutputVol[0], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_SP_LSRS, (UINT32 *) &u4OutputVol[1], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_SPSW, (UINT32 *) &u4OutputVol[2], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_HP, (UINT32 *) &u4OutputVol[3], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_MON, (UINT32 *) &u4OutputVol[4], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_IEC, (UINT32 *) &u4OutputVol[5], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_SCART, (UINT32 *) &u4OutputVol[6], 1);
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_VOL, APROC_IOCTRL_VOL_BT, (UINT32 *) &u4OutputVol[7], 1);
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);

        if ( u4type > 7 )
        {
            u4type = 0;
            Printf ("!Wrong Type!\n");
        }
        
        u4type += APROC_IOCTRL_VOL_SP;
        
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_VOL, u4type , (UINT32 *) &u4v0, 1);
        
        return 1;
    }   
    else
    {
    
        Printf("\nOutput Volume Setting at Aproc:\n");
        Printf(" SP_LR   = 0x%x\n", u4OutputVol[0]);
        Printf(" SP_LSRS = 0x%x\n", u4OutputVol[1]);
        Printf(" SP_C/SW = 0x%x\n", u4OutputVol[2]);
        Printf(" HP      = 0x%x\n", u4OutputVol[3]);
        Printf(" Monitor = 0x%x\n", u4OutputVol[4]);
        Printf(" SPDIF   = 0x%x\n", u4OutputVol[5]);
        Printf(" Scart   = 0x%x\n", u4OutputVol[6]);
        Printf(" BT      = 0x%x\n", u4OutputVol[7]);

        Printf("\nUsage: opv  [type][v0]\n");
        Printf("type = 0 -> SP L/R\n");
        Printf("type = 1 -> SP LS/RS\n");
        Printf("type = 2 -> SP C/SW\n");
        Printf("type = 3 -> HP\n");
        Printf("type = 4 -> Monitor\n");
        Printf("type = 5 -> SPDIF\n");
        Printf("type = 6 -> Scart\n");
        Printf("type = 7 -> BT\n");
        Printf("v0   = 0 ~ 0x7fffff\n");
        return -1;
    }

}

static INT32 _AprocPostProcAmixerMute(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0, u4AmixerMute[3];

	_vAUD_Aproc_Get (APROC_CONTROL_TYPE_MUTE, APROC_IOCTR_MUTE_AMIXER0, (UINT32 *) &u4AmixerMute[0], 1);
	_vAUD_Aproc_Get (APROC_CONTROL_TYPE_MUTE, APROC_IOCTR_MUTE_AMIXER1, (UINT32 *) &u4AmixerMute[1], 1);
	_vAUD_Aproc_Get (APROC_CONTROL_TYPE_MUTE, APROC_IOCTR_MUTE_AMIXER2, (UINT32 *) &u4AmixerMute[2], 1);

    
    if (i4Argc == 3)
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);

        if ( u4type > 2 )
        {
            u4type = 0;
            Printf ("!Wrong Type!\n");
        }
        
        u4type += APROC_IOCTR_MUTE_AMIXER0;
        
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_MUTE, u4type , (UINT32 *) &u4v0, 1);
        
        return 1;
    }   
    else
    {
    
        Printf("\nAmixer Mute Setting at Aproc:\n");
        Printf(" Amixer0 = 0x%x\n", u4AmixerMute[0]);
        Printf(" Amixer1 = 0x%x\n", u4AmixerMute[1]);
        Printf(" Amixer2 = 0x%x\n", u4AmixerMute[2]);
        

        Printf("\nUsage: am  [type][v0]\n");
        Printf("type = 0 -> Amixer0\n");
        Printf("type = 1 -> Amixer1\n");
        Printf("type = 2 -> Amixer2\n");

        return -1;
    }

}

static INT32 _AprocPostProcBalancer(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0, u4BlcVal[2];
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);

        if ( u4type > APROC_IOCTR_BALANCE_NUM )
        {
            u4type = 0;
            Printf ("!Wrong Type!\n");
        }
        
        u4type += APROC_IOCTR_BALANCE_SP;
        
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_BALANCE, u4type , (UINT32 *) &u4v0, 1);
        
        return 1;
    }   
    else
    {
    
        Printf("\nBalancer Setting at Aproc:\n");
        _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BALANCE, APROC_IOCTR_BALANCE_SP, (UINT32 *) &u4BlcVal[0], 1);
        Printf(" SP L = 0x%x, R = 0x%x\n", u4BlcVal[0], u4BlcVal[1]);
        _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BALANCE, APROC_IOCTR_BALANCE_HP, (UINT32 *) &u4BlcVal[0], 1);
        Printf(" HP L = 0x%x, R = 0x%x\n", u4BlcVal[0], u4BlcVal[1]);
        _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BALANCE, APROC_IOCTR_BALANCE_IEC, (UINT32 *) &u4BlcVal[0], 1);
        Printf(" IEC L = 0x%x, R = 0x%x\n", u4BlcVal[0], u4BlcVal[1]);
        _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BALANCE, APROC_IOCTR_BALANCE_INPUT0, (UINT32 *) &u4BlcVal[0], 1);
        Printf(" INPUT0 L = 0x%x, R = 0x%x\n", u4BlcVal[0], u4BlcVal[1]);
        _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BALANCE, APROC_IOCTR_BALANCE_INPUT1, (UINT32 *) &u4BlcVal[0], 1);
        Printf(" INPUT1 L = 0x%x, R = 0x%x\n", u4BlcVal[0], u4BlcVal[1]);
        _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BALANCE, APROC_IOCTR_BALANCE_INPUT2, (UINT32 *) &u4BlcVal[0], 1);
        Printf(" INPUT2 L = 0x%x, R = 0x%x\n", u4BlcVal[0], u4BlcVal[1]);

        Printf("\nUsage: blc  [type][v0]\n");
        Printf("type = 0 -> SP ; 1 -> HP ; 2 -> IEC; 3 -> INPUT0, 4 -> INPUT1, 5 ->INPUT2\n");
        return -1;
    }

}




#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST
/*Aproc Bmang*/
static INT32 _AprocPostProcBmang (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0,u4v1,u4v2,u4v3,u4v4,u4v5;
    APROC_DSP_BMANG_SET_T  aBmangSet; 
    
    UNUSED(u4v1);
    UNUSED(u4v2);
    UNUSED(u4v3);
    UNUSED(u4v4);
    UNUSED(u4v5);

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_BMANG, 0, (UINT32 *) &aBmangSet, 1);
    
    if (i4Argc > 2 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);
        
        u4v1 = 1;
        u4v2 = 1;
        u4v3 = 1;
        u4v4 = 1;
        u4v5 = 1;
        
        if (u4type == 4)
        {   
            if(i4Argc < 8)
            {
                goto lbUsage;
            }
            else
            {
                u4v1 = (UINT32)StrToInt(szArgv[3]);
                u4v2 = (UINT32)StrToInt(szArgv[4]);
                u4v3 = (UINT32)StrToInt(szArgv[5]);
                u4v4 = (UINT32)StrToInt(szArgv[6]);
                u4v5 = (UINT32)StrToInt(szArgv[7]);
            }
        }
        
        switch (u4type)
        {
            case 0:
                aBmangSet.CtrlFlag = (BMANG_MODE_T)u4v0;
                break;
            case 1:
                aBmangSet.ChCfg = (BMANG_CHID_T)u4v0;
                break;
            case 2:
                aBmangSet.u4LFETrim = u4v0;
                break;
            case 3:
                aBmangSet.u4Volume = u4v0;
                break;
            case 4:
                aBmangSet.sSpkSize[0] = (BMANG_SPKSZ_T)u4v0;
                aBmangSet.sSpkSize[1] = (BMANG_SPKSZ_T)u4v1;
                aBmangSet.sSpkSize[2] = (BMANG_SPKSZ_T)u4v2;
                aBmangSet.sSpkSize[3] = (BMANG_SPKSZ_T)u4v3;
                aBmangSet.sSpkSize[4] = (BMANG_SPKSZ_T)u4v4;
                aBmangSet.sSpkSize[5] = (BMANG_SPKSZ_T)u4v5;
                break;
            case 5:
                aBmangSet.u4LFE2CH = u4v0;
                break;
            case 6:
                aBmangSet.u4CutFc = u4v0;
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_BMANG, 0, (UINT32 *) &aBmangSet, 1);
          
          return 1;
    }   
lbUsage:    

    Printf("\nBass Management Setting at Aproc:\n Flag = %d, SpkConfig = %d\n", aBmangSet.CtrlFlag, aBmangSet.ChCfg);
    Printf(" LFETrim = 0x%x, Volume = 0x%x\n", aBmangSet.u4LFETrim, aBmangSet.u4Volume);
    Printf(" Speaker Size L %d, R %d, C %d, Ls %d, Rs %d, SW %d\n", aBmangSet.sSpkSize[0],aBmangSet.sSpkSize[1],
        aBmangSet.sSpkSize[2],aBmangSet.sSpkSize[3],aBmangSet.sSpkSize[4],aBmangSet.sSpkSize[5]);
    Printf(" LFE2CH = %d, Fc =%d \n", aBmangSet.u4LFE2CH, aBmangSet.u4CutFc);

    Printf("\nUsage: Bmang [type][v0][v1][v2][v3][v4][v5]\n");
    Printf("Control Flag   : type = 0, v0 = 1 enable, = 0 disable,\n");
    Printf("Speaker Config : type = 1, v0 = 0~ 11, 2 means 200,\n");
    Printf("LEFTrim        : type = 2, v0 = 0x0000 ~ 0x08000000,0x02000000=0DB\n");
    Printf("Volume         : type = 3, v0 = 0~0x02000000,0x02000000=0DB\n");
    Printf("Speaker Size   : type = 4, v0~v5 0/1 : small/large, L,R,C,LS,RS,SW\n");
    Printf("LFE mix to LR  : type = 5, v0 = 1 enable, = 0 disable,\n");
    Printf("CutOff Freq    : type = 6, v0 = 50~ 300 \n");

    return -1;
}

static INT32 _AprocPostProcMVS (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    APROC_DSP_MVS_SET_T  aMVSSet; 

    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_MVS, 0, (UINT32 *) &aMVSSet, 1);
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);
        
        switch (u4type)
        {
            case 0:
                aMVSSet.fgMVSFlag= (BOOL)u4v0;
                break;
            case 1:
                aMVSSet.i4CrossTalkGain= u4v0;
                break;
            case 2:
                aMVSSet.i4ClarityGain= u4v0;
                break;
            case 3:
                aMVSSet.i4BassGain= u4v0;
                break;
            case 4:
                aMVSSet.i4LRGain= u4v0;
                break;
            case 5:
                aMVSSet.i4OutputGain= u4v0;
                break;
            case 6:
                aMVSSet.i4WidthGain= u4v0;
                break;
            case 7:
                aMVSSet.i4Fc= u4v0;
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_MVS, 0, (UINT32 *) &aMVSSet, 1);
          
          return 1;
    }   
    else
    {

        Printf("\nVirtualSurround Setting at Aproc:\n Flag = %d\n", aMVSSet.fgMVSFlag);
        Printf(" CrossTalkGain = 0x%x, ClarityGain = 0x%x\n", aMVSSet.i4CrossTalkGain, aMVSSet.i4ClarityGain);
        Printf(" BassGain = 0x%x, i4LRGain = 0x%x, OutputGain = 0x%x, WidthGain =  0x%x, Fc = 0x%x \n", 
            aMVSSet.i4BassGain, aMVSSet.i4LRGain, aMVSSet.i4OutputGain, aMVSSet.i4WidthGain, aMVSSet.i4Fc);

        Printf("\nUsage: mvs  [type][v0]\n");
        Printf("Control Flag  : type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("CrossTalkGain : type = 1, v0 = 0x0 ~ 0x7fff, 0x1000 = 0dB, 0x7fff = +18dB, linear gain.\n");
        Printf("ClarityGain   : type = 2, v0 = 0x0 ~ 0x7fff, 0x1000 = 0dB, 0x7fff = +18dB, linear gain.\n");
        Printf("BassGain      : type = 3, v0 = 0x0 ~ 0x7fff, 0x1000 = 0dB, 0x7fff = +18dB, linear gain.\n");
        Printf("LRGain        : type = 4, v0 = 0x0 ~ 0x7fff, 0x1000 = 0dB, 0x7fff = +18dB, linear gain.\n");
        Printf("OutputGain    : type = 5, v0 = 0x0 ~ 0x7fff, 0x1000 = 0dB, 0x7fff = +18dB, linear gain.\n");
        Printf("WidthGain     : type = 6, v0 = 0x0 ~ 0x7fff, 0x1000 = 0dB, 0x7fff = +18dB, linear gain.\n");
        Printf("Fc            : type = 7, v0 = 0x7f41 ~ 0x7f44 \n");

    return -1;
    }
}

static INT32 _AprocPostProcMVB (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    APROC_DSP_MVB_SET_T  aMVBSet; 
    
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_MVB, 0, (UINT32 *) &aMVBSet, 1);
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);
        
        switch (u4type)
        {
            case 0:
                aMVBSet.fgMVBFlag = (BOOL)u4v0;
                break;
            case 1:
                aMVBSet.i4BassGain = u4v0;
                break;
            case 2:
                aMVBSet.i4DetGain = u4v0;
                break;
            case 3:
                aMVBSet.i4OutputGain = u4v0;
                break;
            case 4:
                aMVBSet.u4AttackStep = u4v0;
                break;
            case 5:
                aMVBSet.u4ExpanderTh = u4v0;
                break;
            case 6:
                aMVBSet.u4Fc = u4v0;
                break;
            case 7:
                aMVBSet.u4ReleaseStep = u4v0;
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_MVB, 0, (UINT32 *) &aMVBSet, 1);
          
          return 1;
    }
    else
    {

        Printf("\nVirtualBass Setting at Aproc:\n Flag = %d\n", aMVBSet.fgMVBFlag);
        Printf(" i4BassGain = 0x%x, i4DetGain = 0x%x\n", aMVBSet.i4BassGain, aMVBSet.i4DetGain);
        Printf(" i4OutputGain = 0x%x, u4AttackStep = 0x%x, u4ExpanderTh = 0x%x, u4Fc =  0x%x, u4ReleaseStep = 0x%x \n", 
            aMVBSet.i4OutputGain, aMVBSet.u4AttackStep, aMVBSet.u4ExpanderTh, aMVBSet.u4Fc, aMVBSet.u4ReleaseStep);

        Printf("\nUsage: mvb [type][v0]\n");
        Printf("Control Flag: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("i4BassGain  : type = 1, v0 = 0x0000 ~ 0x2000, 0x1000 = 0dB, linear gain.\n");
        //Printf("i4DetGain         : type = 2, v0 = 0x0 ~ 0x7fff, 0x1000 = 0dB, 0x7fff = +18dB, linear gain.\n");
        Printf("i4OutputGain: type = 3, v0 = 0x1000 ~ 0x3000, 0x1000 = 0dB, linear gain.\n");
        //Printf("u4AttackStep      : type = 4, v0 = 0x0 ~ 0x7fff, 0x1000 = 0dB, 0x7fff = +18dB, linear gain.\n");
        //Printf("u4ExpanderTh  : type = 5, v0 = 0x0 ~ 0x7fff, 0x1000 = 0dB, 0x7fff = +18dB, linear gain.\n");
        Printf("u4Fc        : type = 6, v0 = 0 = 50Hz, 1 = 80Hz, 2 = 110Hz, 3 = 140Hz, 4 = 170Hz, 5 = 200Hz.\n");
        //Printf("u4ReleaseStep : type = 7, v0 = 0x7f41 ~ 0x7f44 \n");

    return -1;
    }
}
static INT32 _AprocPostProcPEQ (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0, u4v1,i;
    BOOL fgPEQ;
    APROC_DSP_PEQ_SET_T  aPEQSet[9]; 

    for (i=0;i<9;i++)
    {
        _vAUD_Aproc_Get (APROC_CONTROL_TYPE_PEQ, i, (UINT32 *) &aPEQSet[i], 1);
    }

    fgPEQ = AUD_DspGetPEQEnable();
    
    if (i4Argc > 2 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);

        if (u4type > 0) 
        {
            if (i4Argc < 4)
                goto lbUsage;
            else
                u4v1 = (UINT32)StrToInt(szArgv[3]);

            if( u4v0 > 8)
            {
                u4v0 = 8;
                Printf ("!! Acturely set Idx =8 !!\n");
            }

        }
            
        switch (u4type)
        {
            case 0:
                fgPEQ = (BOOL)u4v0;
                break;
            case 1:
                aPEQSet[u4v0].u4Fc = u4v1;
                break;
            case 2:
                aPEQSet[u4v0].u4Q= u4v1;
                break;
            case 3:
                aPEQSet[u4v0].u4Gain= u4v1;
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            if (u4type == 0)
                
                AUD_DspSetPEQEnable(fgPEQ);
            else
                
                _vAUD_Aproc_Set (APROC_CONTROL_TYPE_PEQ, u4v0, (UINT32 *) &aPEQSet[u4v0], 1);
          
          return 1;
    }
    else
    {
lbUsage:

        Printf("\nPEQ Setting at Aproc:\n Flag = %d\n", fgPEQ);
        for(i=0;i<9;i++)
        {
            Printf("Idx %d, Fc= %d, Q= 0x%x, Gain= %d\n", i, aPEQSet[i].u4Fc, aPEQSet[i].u4Q, aPEQSet[i].u4Gain);
        }

        Printf("\nUsage: peq [type][v0][v1]\n");
        Printf("Flag: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("Fc  : type = 1, v0 = Idx 0~8 , v1 = 20 ~ 20000 Hz\n");
        Printf("Q   : type = 2, v0 = Idx 0~8, v1 = 0x1 ~ 0x3fffff0, 0x3fffff0 = 16.0.\n");
        Printf("Gain: type = 3, v0 = Idx 0~8, v1 = 0 ~ 720, 0 = -18dB, 720 = +18dB. Step = 0.05dB.\n");
    return -1;
    }
}
static INT32 _AprocPostProcDRCLimiter (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Idx, u4Type, u4Val,i ;
    APROC_DSP_LIMT_SET_T  aLimtSet[APROC_LIMITER_IDX_NUM]; 

    for (i=0;i<APROC_LIMITER_IDX_NUM;i++)
    {
        _vAUD_Aproc_Get (APROC_CONTROL_TYPE_LIMITER, i, (UINT32 *) &aLimtSet[i], 1);
    }

    if (i4Argc >= 4 )
    {
        u4Idx = (UINT32)StrToInt(szArgv[1]);
        u4Type = (UINT32)StrToInt(szArgv[2]);
        u4Val = (UINT32)StrToInt(szArgv[3]);

        if( u4Idx > APROC_LIMITER_IDX_NUM - 1)
        {
            u4Idx = APROC_LIMITER_IDX_NUM - 1;
            Printf ("!! Acturely set Idx = %d !!\n", u4Idx);
        }

            
        switch (u4Type)
        {
            case 0:
                aLimtSet[u4Idx].fgLimtFlag = (BOOL)u4Val;
                break;
            case 1:
                aLimtSet[u4Idx].i4SetTargetLevel = (INT32)u4Val;
                break;
            case 2:
                aLimtSet[u4Idx].i4SetAttackStepDB = (INT32)u4Val;
                break;
            case 3:
                aLimtSet[u4Idx].i4SetReleaseStepDB = (INT32)u4Val;
                break;
            case 4:
                aLimtSet[u4Idx].i4SetRatio = (INT32)u4Val;
                break;
            case 5:
                aLimtSet[u4Idx].i4SetSilenceLevel = (INT32)u4Val;
                break;
            case 6:
                aLimtSet[u4Idx].i4SetMaxExpandGainDB = (INT32)u4Val;
                break;
            case 7:
                aLimtSet[u4Idx].i4SetPostGain = (INT32)u4Val;
                break;
            case 8:
                aLimtSet[u4Idx].i4SetHoldPeriod = (INT32)u4Val;
                break;
            case 9:
                aLimtSet[u4Idx].i4SetDetPeriod = (INT32)u4Val;
                break;
            case 10:
                aLimtSet[u4Idx].u4SetProcMode = u4Val;
                break;
            case 11:
                aLimtSet[u4Idx].i4SetSgainPeriod = (INT32)u4Val;
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_LIMITER, u4Idx, (UINT32 *) &aLimtSet[u4Idx], 1);
          
          return 1;
    }
    else
    {
        Printf("\nLimiter Setting at Aproc (Idx 0~3 : SP, SW, LSRS, HP):\n ");
        
        for(i=0;i<APROC_LIMITER_IDX_NUM;i++)
        {
            Printf("Idx %d, Flag= %d\n", i, aLimtSet[i].fgLimtFlag);
            Printf("TargetLevel= 0x%x, AttackStepDB= 0x%x\n", aLimtSet[i].i4SetTargetLevel,aLimtSet[i].i4SetAttackStepDB);
            Printf("ReleaseStepDB= 0x%x, SetRatio= 0x%x\n", aLimtSet[i].i4SetReleaseStepDB,aLimtSet[i].i4SetRatio);
            Printf("SilenceLevel= 0x%x, MaxExpandGain= 0x%x\n", aLimtSet[i].i4SetSilenceLevel,aLimtSet[i].i4SetMaxExpandGainDB);
            Printf("PostGain= 0x%x, AttackStepDB= 0x%x\n", aLimtSet[i].i4SetPostGain,aLimtSet[i].i4SetHoldPeriod);
            Printf("DetPeriod= 0x%x\n", aLimtSet[i].i4SetDetPeriod);
            Printf("\n");
        }
        Printf("ProcMode= 0x%x, SgainPeriod= 0x%x\n", i, aLimtSet[0].u4SetProcMode,aLimtSet[0].i4SetSgainPeriod);


        Printf("\nUsage: dl [Idx][type][val]\n");
        Printf("Idx         : Idx 0~3 : SP, SW, LSRS, HP\n");
        Printf("Flag        : type= 0, val= 1 enable, = 0 disable,\n");
        Printf("TargetLevel : type= 1, val= (0x7ff, 0x3fffff) -48dB ~ +18dB, 0x7ffff = 0dB\n");
        Printf("AttackStep  : type= 2, val= (0x1, 0x7fffffff) 0.0000014 dB/ms ~ 192 dB/ms\n");
        Printf("ReleaseStep : type= 3, val= (0x1, 0x7fffffff) 0.0000014 dB/ms ~ 192 dB/ms\n");
        Printf("SetRatio    : type= 4, val= (0x0, 0x7ffff) 0x0= infinite:1, 0x7ffff = 1:1. Linear ratio\n");
        Printf("SilenceLevel: type= 5, val= (0x0, 0x7ffff) 0x7= -96dB, 0x7ffff= 0dB, 0x0= no silenece level control Linear ratio\n");
        Printf("MaxExpandGain:type= 6, val= (0x0, 0x7fffff) 0x2fffff= +6dB, 0x7fffff= +16dB, 0x7ffff= 1dB, 0x0= no expanding\n");
        Printf("PostGain    : type= 7, val= (0x0, 0x7fffff) 0x0= off, 0x1 = -102 dB, 0x7fffff = 36dB\n");
        Printf("HoldPeriod  : type= 8, val= (0x0, 0x1000) 0x0 = 0 ms, 0x200 = 5.46 s. Step = 1.33 ms\n");
        Printf("DetPeriod   : type= 9, val= (0x0, 0x1000) 0x0 = 0 ms, 0x200 = 5.46 s. Step = 1.33 ms\n");
        Printf("ProcMode    : type= a, val= \n");
        Printf("SgainPeriod : type= b, val= (0x0, 0x1000) 0x0 = 0 ms, 0x200 = 5.46 s. Step = 1.33 ms\n");
    return -1;
    }
}

#if 1//def //CC_AUD_AQ_IN_DRVCUST
UINT32 _gu4CurrentAqKey = 0;
static INT32 _AprocTestFlashAQ (INT32 i4Argc, const CHAR ** szArgv) //CC_AUD_AQ_IN_DRVCUST
{
    UINT32 u4Type;
    UINT32 u4arg1, u4arg2;
        
    if (i4Argc < 3 )
    {
        Printf(" Usage: [Type] [arg1] [arg2]\n");
        Printf("   Type 1  AQ Init(copy data from flash to dram), arg1:0,      arg2:0\n");        
        Printf("   Type 2  AQ List(list AQ load on dram),         arg1:offset, arg2:length\n");
        Printf("\n");
        Printf("   Type 3  MM_SVT_SP,  arg1:0  arg2:0\n");
        Printf("   Type 4  MM_SVT_SPW  arg1:0  arg2:0\n");
        Printf("   Type 5  MM_SVT_HP,  arg1:0  arg2:0\n");
        Printf("   Type 6  MM_SVT_LO,  arg1:0  arg2:0\n");        
        Printf("   Type 7  MM_SVT_LOH, arg1:0  arg2:0\n");
        Printf("   Type 8  MM_SVT_ASW, arg1:0  arg2:0\n");
        Printf("   Type 9  MM_SVT_DSW, arg1:0  arg2:0\n");
        Printf("   Type 10 MM_SVT_BT,  arg1:0  arg2:0\n");
        Printf("   Type 11 DRC,   arg1:0 arg2:0\n");
        Printf("   Type 12 LIMT,  arg1:0 arg2:0\n");
        Printf("   Type 13 SMT,   arg1:0 arg2:0\n");
        Printf("   Type 14 SPEQ,  arg1:0 arg2:0\n");
        Printf("   Type 15 SLT,   arg1:0 arg2:0\n");        
        Printf("   Type 16 SWS,   arg1:0 arg2:0\n");
        Printf("   Type 17 LOUDT, arg1:0 arg2:0\n");
        Printf("   Type 18 ADCT,  arg1:0 arg2:0\n");        
        Printf("   Type 19 ACT,   arg1:0 arg2:0\n");
        Printf("   Type 20 ADT,   arg1:0 arg2:0\n");
        Printf("   Type 21 RSV,   arg1:0 arg2:0\n");        
        Printf("   Type 22 DA Gen  Config,  arg1:0 arg2:0\n");
        Printf("   Type 23 DA 5760 Config1, arg1:0 arg2:0\n");
        Printf("   Type 24 DA 5760 Config2, arg1:0 arg2:0\n");
        Printf("   Type 25 DA 5538 Config,  arg1:0 arg2:0\n");        
        Printf("   Type 26 DA 5538 BQ,      arg1:0 arg2:0\n");        
        Printf("   Type 27 DA 5538 BT,      arg1:0 arg2:0\n");        
        Printf("   Type 28 DA 5538 LOUD,    arg1:0 arg2:0\n");        
        Printf("   Type 29 DA 5538 DRC,     arg1:0 arg2:0\n");                

    }
    u4Type = (UINT32)StrToInt(szArgv[1]);
    u4arg1 = (UINT32)StrToInt(szArgv[2]);
    u4arg2 = (UINT32)StrToInt(szArgv[3]);

    switch (u4Type)        
    {
    case 1: //AQ Init 
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Set_Init, 0,0, 0,0);
        break;
    case 2: // AQ List
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_List, 0,0, u4arg1,u4arg2);
        break;    
    case 3: // MM_SVT_SP
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SVT_SP, 0, 0, _gu4CurrentAqKey, 0);
        break;        
    case 4: // MM_SVT_SPW
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SVT_SPW, 0, 0, _gu4CurrentAqKey, 0);
        break;
    case 5: // MM_SVT_HP
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SVT_HP, 0, 0, _gu4CurrentAqKey, 0);
        break;
    case 6: // MM_SVT_LO
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SVT_LO, 0, 0, _gu4CurrentAqKey, 0);
        break;        
    case 7: // MM_SVT_LOH
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SVT_LOH, 0, 0, _gu4CurrentAqKey, 0);
        break;        
    case 8: // MM_SVT_ASW
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SVT_ASW, 0, 0, _gu4CurrentAqKey, 0);
        break;
    case 9: // MM_SVT_DSW
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SVT_DSW, 0, 0, _gu4CurrentAqKey, 0);
        break;
    case 10: // MM_SVT_BT
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SVT_BT, 0, 0, _gu4CurrentAqKey, 0);
        break;        
    case 11: // MM_DRC
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_DRC, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 12: // MM_LIMT
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_LIMT, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 13: // MM_SMT
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SMT, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 14: // MM_SPEQ
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SPEQ, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 15: // MM_SLT
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SLT, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 16: // MM_SWS
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_SWS, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 17: // MM_LOUDT
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_LOUDT, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 18: // MM_ADCT
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_ADCT, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 19: // MM_ACT
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_ACT, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 20: // MM_ADT
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_ADT, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 21: // MM_RSV
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_RSV, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 22: // MM_DA_GEN_CFG
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_DA_GEN_CFG, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 23: // MM_DA_5760_CFG1
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_DA_5760_CFG1, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 24: // MM_DA_5760_CFG2
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_DA_5760_CFG2, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 25: // MM_DA_5538_CFG
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_DA_5538_CFG, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 26: // MM_DA_5538_BQ
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_DA_5538_BQ, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 27: // MM_DA_5538_BT
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_DA_5538_BT, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 28: // MM_DA_5538_LOUD
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_DA_5538_LOUD, 0, 0, _gu4CurrentAqKey, 0);
        break; 
    case 29: // MM_DA_5538_DRC
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_DA_5538_DRC, 0, 0, _gu4CurrentAqKey, 0);
        break; 

    }

    return 0;
}

static INT32 _AprocTestAQKey (INT32 i4Argc, const CHAR ** szArgv) //CC_AUD_AQ_IN_DRVCUST
{
    AUD_DrvCust_AQ_Key_T rAqKey;
    
    if (i4Argc < 7 )
    {
        Printf(" Usage: [FS] [MM Mode] [TV Pos] [Iuput] [Sound Mode] [Opt1] [Opt2] [Surr On/Off] [Auto On/Off]\n");
        Printf(" Fs         : 0 to %d\n", AQ_FS_96KHZ);
        Printf(" MM Mode    : 0 to %d\n", AQ_MM_MEASURE_MODE_1);
        Printf(" TV Pos     : 0 to %d\n", AQ_TV_POSITION_WALL_MOUNT);
        Printf(" Iuput      : 0 to %d\n", AQ_INPUT_RESERVED3);
        Printf(" Sound Mode : 0 to %d\n", AQ_SOUND_MODE_DEMO_S1A1);
        Printf(" Opt1 are 1 of the followings\n");        
        Printf("     Booster    : 0 to %d\n", AQ_BOOSTER_OFF);
        Printf("     Clear Aud  : 0 to %d\n", AQ_CLEAR_AUDIO_PLUS_ON);        
        Printf("     BT Type    : 0 to %d\n", AQ_BT_TYPE_HEADPHONE);
        Printf(" Opt2 are 1 of the followings\n");
        Printf("     Wireless   : 0 to %d\n", AQ_WIRELESS_SUBWOOFER_MODE_ON);
        Printf("     Pic Mode   : 0 to %d\n", AQ_PICTURE_MODE_OTHER);
        Printf(" Surround Off/On : 0/1\n");
        Printf(" Auto     Off/On : 0/1\n");
        Printf("Current Key = 0x%08x\n", _gu4CurrentAqKey);
        return -1;
    }

    rAqKey.u1Fs = (UINT32)StrToInt(szArgv[1]);
    rAqKey.u1MmMode = (UINT32)StrToInt(szArgv[2]);
    rAqKey.u1TvPos = (UINT32)StrToInt(szArgv[3]);
    rAqKey.u1Input = (UINT32)StrToInt(szArgv[4]);
    rAqKey.u1SoundMode = (UINT32)StrToInt(szArgv[5]);
    rAqKey.u1SoundBooster = (UINT32)StrToInt(szArgv[6]);
    rAqKey.u1WirelessWoofer = (UINT32)StrToInt(szArgv[7]);
    rAqKey.u1SurroundOnOff = (UINT32)StrToInt(szArgv[8]);
    rAqKey.u1AutoOnOff = (UINT32)StrToInt(szArgv[9]);

    DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Gen_Key, (VOID*)&rAqKey, (VOID*)&_gu4CurrentAqKey, 0, 0);
    Printf("Set _gu4CurrentAqKey to 0x%08x\n", _gu4CurrentAqKey);

    return 0;
}
#endif //CC_AUD_AQ_IN_DRVCUST

static INT32 _AprocPostProc8BandEQ (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0, u4v1,i;
    BOOL fgEQSp, fgEQLsRs, fgFirSp, fgFirLsRs;
    APROC_DSP_EQ_SET_T  aEQSet[8]; 

    for (i=0;i<8;i++)
    {
        _vAUD_Aproc_Get (APROC_CONTROL_TYPE_EQ, i, (UINT32 *) &aEQSet[i], 1);
    }

    fgEQSp = _AUD_DspGetAprocEqEnable(APROC_IOCTRL_SP,0);
    fgEQLsRs = _AUD_DspGetAprocEqEnable(APROC_IOCTRL_LSRS,0);
    fgFirSp = _AUD_DspGetAprocEqEnable(APROC_IOCTRL_SP,1);
    fgFirLsRs = _AUD_DspGetAprocEqEnable(APROC_IOCTRL_LSRS,1);
    
    if (i4Argc > 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);
        u4v1 = (UINT32)StrToInt(szArgv[3]);

        if ((u4type > 0) && ( u4v0 > 7))
        {
            u4v0 = 7;
            Printf ("!! Acturely set Idx =7 !!\n");

        }
            
        switch (u4type)
        {
            case 0:
                if (u4v0 > APROC_IOCTRL_LSRS)
                    _AUD_DspSetAprocEqEnable((APROC_IOCTRL_T)u4v0 - 1,1,(BOOL)u4v1);
                else
                    _AUD_DspSetAprocEqEnable((APROC_IOCTRL_T)u4v0, 0, (BOOL)u4v1);
                break;
            case 1:
                aEQSet[u4v0].u4Fc = u4v1;
                break;
            case 2:
                aEQSet[u4v0].u4Q = u4v1;
                break;
            case 3:
                aEQSet[u4v0].u4Gain = u4v1;
                break;
/*HIDE*/    case 4:
                aEQSet[u4v0].u4Type = u4v1;
                break;
            default:
                Printf ("!Wrong Type!\n");
                break;
        }       
            if (u4type != 0)
            {   
                _vAUD_Aproc_Set (APROC_CONTROL_TYPE_EQ, u4v0, (UINT32 *) &aEQSet[u4v0], 1);
          
            }
          return 1;
    }
    else
    {

        Printf("\n8Band EQ Setting at Aproc:\n");
        Printf("EQ_SP %d, EQ_LsRs %d, Fir_SP %d, Fir_LsRs %d\n",fgEQSp,fgEQLsRs, fgFirSp, fgFirLsRs);
        
        for(i=0;i<8;i++)
        {
            Printf("Band %d, Filter = %d, Fc= %d, Q= 0x%x, Gain= %d\n", i, 
                aEQSet[i].u4Type, aEQSet[i].u4Fc, aEQSet[i].u4Q, aEQSet[i].u4Gain);
        }

        Printf("\nUsage: eq [type][v0][v1]\n");
        Printf("Flag: type = 0, v0: 0 = EQ_SP, 1 = EQ_LSRS, 2 = FIR_SP, 3 = FIR_LSRS\n");
        //Printf("Type: type = 4, v0 =  \n");
        Printf("Fc  : type = 1, v0 = Idx 0~7 , v1 = 20 ~ 20000 Hz\n");
        Printf("Q   : type = 2, v0 = Idx 0~7,  v1 = 0x1 ~ 0x3fffff0, 0x3fffff0 = 16.0.\n");
        Printf("Gain: type = 3, v0 = Idx 0~7,  v1 = 0 ~ 720, 0 = -18dB, 720 = +18dB. Step = 0.05dB.\n");
    return -1;
    }
}

#ifdef ARM_SUPPORT_SRS_SS
static INT32 _AprocPostSRSAEQ(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type != 0) && (u4type != 4))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSAEQ_Set(u4type,u4v0);

        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSAEQ_Get();
        return 1;

    }
    else
    {

        Printf("\nUsage: aeq [type][v0]\n");
        Printf("Left AEQ Enable: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("Left InputGain: type = 1, v0=[0~100(1.0)]\n");
        Printf("Left OutputGain: type = 2, v0=[0~100(1.0)]\n");
        Printf("Left BypassGain: type = 3, v0=[0~100(1.0)]\n");
        Printf("Right AEQ Enable: type = 4, v0 = 1 enable, = 0 disable\n");
        Printf("Right InputGain: type = 5, ,v0=[0~100(1.0)]\n");
        Printf("Right OutputGain: type = 6, ,v0=[0~100(1.0)]\n");
        Printf("Right BypassGain: type = 7, ,v0=[0~100(1.0)]\n");

        return -1;
    }
}

static INT32 _AprocPostSRSBT(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type == 1) || (u4type == 2) || (u4type == 3))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSBT_Set(u4type,u4v0);
          
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSBT_Get();
        return 1;
        
    }
    else
    {

        Printf("\nUsage: bt [type][v0]\n");
        Printf("BassTreble Enable: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("InputGain: type = 1, v0=[0~100(1.0)]\n");
        Printf("OutputGain: type = 2, v0=[0~100(1.0)]\n");
        Printf("BypassGain: type = 3, v0=[0~100(1.0)]\n");
        Printf("BassGain: type = 4, v0=[-12~12]\n");
        Printf("TrebleGain: type = 5, ,v0=[-12~12]\n");

        return -1;
    }
}

static INT32 _AprocPostSRSCC3D(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type == 1) || (u4type == 2) || (u4type == 3))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;//SRS_FXP16(0.01.1)
        }
        else if((u4type == 10))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0xa3;//SRS_FXP16(0.01.2)
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSCC3D_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSCC3D_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: cc3d [type][v0]\n");
        Printf("CC3D Enable: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("InputGain: type = 1, v0=[0~100(1.0)]\n");
        Printf("OutputGain: type = 2, v0=[0~100(1.0)]\n");
        Printf("BypassGain: type = 3, v0=[0~100(1.0)]\n");
        Printf("DepthProcessEnable: type = 4, v0 = 1 enable, = 0 disable\n");
        Printf("SurEn: type = 5, v0 = 1 enable, = 0 disable\n");
        Printf("Mode: type = 6,v0=0(3D),=1(MUSIC),=2(MOVIE)\n");
        Printf("DepthControlMode: type = 7, v0=0(LOW),=1(MEDIUM),=2(HIGH)\n");
        Printf("DepthChannelFadeEnable: type = 8, v0 = 1 enable, = 0 disable\n");
        Printf("LREnhEnable: type = 9, v0 = 1 enable, = 0 disable\n");
        Printf("LREnhGain: type = 10, ,v0=[0~200]\n");
        //Printf("MixFadeCtrl: type = 11, ,v0=[0~0x7fff]\n");

        return -1;
    }
}
static INT32 _AprocPostSRSCSHP(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);
        _vAproc_PostProcSRSCSHP_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSCSHP_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: cshp [type][v0]\n");
        Printf("CSHP Enable: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("InputGain: type = 1, v0=[0~0x7fff]\n");
        Printf("OutputGain: type = 2, v0=[0~0x7fff]\n");
        Printf("BypassGain: type = 3, v0=[0~0x7fff]\n");
        Printf("rend mode: type = 4, \n");

        return -1;
    }
}
static INT32 _AprocPostSRSGEQ(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type == 3) || (u4type == 4) || (u4type == 5) || (u4type == 6) || (u4type == 7)
             || (u4type == 13) || (u4type == 14) || (u4type == 15) || (u4type == 16) || (u4type == 17))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x29;//SRS_FXP16(0.01, 4)
        }
        else if((u4type == 9) || (u4type == 19))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;//SRS_FXP16(0.01.1)
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSGEQ_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSGEQ_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: geq [type][v0]\n");
        Printf("left geq Enable: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("NumOfInternalBands: type = 1, v0=[0~5]\n");
        Printf("ExtraBandBehavior: type = 2, v0=0(DEFAULT),=1(UNITY),=2(SHELF)\n");
        Printf("band gain0: type = 3, v0=[0~600]\n");
        Printf("band gain1: type = 4, v0=[0~600]\n");
        Printf("band gain2: type = 5, ,v0=[0~600]\n");
        Printf("band gain3: type = 6, ,v0=[0~600]\n");
        Printf("band gain4: type = 7, ,v0=[0~600]\n");
        //Printf("gain update: type = 8, v0 = 1 update, = 0 not update\n");
        Printf("input gain: type = 9, ,v0=[0~100(1.0)]\n");
        Printf("right geq Enable: type = 10, v0 = 1 enable, = 0 disable\n");
        Printf("NumOfInternalBands: type = 11, ,v0=[0~5]\n");
        Printf("ExtraBandBehavior: type = 12, v0=0(DEFAULT),=1(UNITY),=2(SHELF)\n");
        Printf("band gain0: type = 13, v0=[0~600]\n");
        Printf("band gain1: type = 14, v0=[0~600]\n");
        Printf("band gain2: type = 15, ,v0=[0~600]\n");
        Printf("band gain3: type = 16, ,v0=[0~600]\n");
        Printf("band gain4: type = 17, ,v0=[0~600]\n");
        //Printf("gain update: type = 18, v0 = 1 update, = 0 not update\n");
        Printf("input gain: type = 19, ,v0=[0~100(1.0)]\n");

        return -1;
    }
}
static INT32 _AprocPostSRSHL(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type == 1) || (u4type == 2) || (u4type == 3) || (u4type == 7))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;//SRS_FXP16(0.01.1)
        }
        else if((u4type == 5))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0xa3d70;//SRS_FXP32(0.01.6)
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSHL_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSHL_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: hl [type][v0]\n");
        Printf("HardLimiter Enable: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("InputGain: type = 1, v0=[0~100(1.0)]\n");
        Printf("OutputGain: type = 2, v0=[0~100(1.0)]\n");
        Printf("BypassGain: type = 3, v0=[0~100(1.0)]\n");
        Printf("Delaylen: type = 4, v0 = [60~240]\n");
        //Printf("Limiterboost: type = 5, ,v0=[1~3200]\n");
        Printf("ACEnable: type = 6, ,v0= 1 enable, = 0 disable\n");
        Printf("ACLevel: type = 7, ,v0=[0~100(1.0)]\n");

        return -1;
    }
}
static INT32 _AprocPostSRSHPF(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);
        _vAproc_PostProcSRSHPF_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSHPF_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: hpf [type][v0]\n");
        Printf("left HPF Enable: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("section: type = 1, v0=[0~3]\n");
        Printf("right HPF Eanble: type = 2, v0 = 1 enable, = 0 disable\n");
        Printf("section: type = 3, v0=[0~3]\n");

        return -1;
    }
}


static INT32 _AprocPostSRSMVOL(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        u4v0 = (UINT32)StrToInt(szArgv[2]);
        _vAproc_PostProcSRSMVOL_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSMVOL_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: mvol [type][v0]\n");
        Printf("mvol Enable: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("step: type = 1, v0=[0~0x7fff]\n");
        Printf("pan: type = 2, \n");
        Printf("mute: type = 3, \n");
        Printf("bypass gain: type = 4, v0=[0~0x7fff]\n");
        Printf("process mode: type = 5\n");
        Printf("boost: type = 6, ,v0=[0~0x7fff]\n");

        return -1;
    }
}
static INT32 _AprocPostSRSSPHPF(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type == 1) || (u4type == 2) || (u4type == 3))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;//SRS_FXP16(0.01.1)
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSSPHPF_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSSPHPF_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: sphpf [type][v0]\n");
        Printf("SpaceHPF Enable: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("InputGain: type = 1, v0=[0~100(1.0)]\n");
        Printf("OutputGain: type = 2, v0=[0~100(1.0)]\n");
        Printf("BypassGain: type = 3, v0=[0~100(1.0)]\n");
        Printf("CutoffFreq: type = 4, v0=0(80),=1(120),=2(200)\n");
        Printf("                        3(240),=4(300),=5(400)\n");
        Printf("                        6(500),=7(600),=8(800),=9(custom)\n");
        

        return -1;
    }
}
static INT32 _AprocPostSRSSRS3D(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type == 1) || (u4type == 2) || (u4type == 3) || (u4type == 5) || (u4type == 6))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;//SRS_FXP16(0.01.1)
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSSRS3D_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSSRS3D_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: srs3d [type][v0]\n");
        Printf("SRS3D Enable: type = 0, v0 = 1 enable, = 0 disable\n");
        Printf("InputGain: type = 1, v0=[0~100(1.0)]\n");
        Printf("OutputGain: type = 2, v0=[0~100(1.0)]\n");
        Printf("BypassGain: type = 3, v0=[0~100(1.0)]\n");
        Printf("HighBitRate: type = 4, v0 = 1 enable, = 0 disable\n");
        Printf("SpaceCtrl: type = 5, ,v0=[0~100(1.0)]\n");
        Printf("CenterCtrl: type = 6, ,v0=[0~100(1.0)]\n");
        Printf("headphone: type = 7, v0 = 1 enable, = 0 disable\n");
        Printf("mode: type = 8, v0=0(MONO),=1(SINGLE_SPEAKER),=2(STERO),=3(MODENUM)\n");
        //Printf("NormGain: type = 9, ,v0=[0~0x7fff]\n");
        return -1;
    }
}
static INT32 _AprocPostSRSSS3D(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type == 1) || (u4type == 2) || (u4type == 4) || (u4type == 5) || (u4type == 6)
             || (u4type == 7) || (u4type == 8) || (u4type == 9))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;//SRS_FXP16(0.01.1)
        }
        else if((u4type == 11))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x28;//SRS_FXP16(0.01.4)
        }
        else if((u4type == 12))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0xa3d70;//SRS_FXP32(0.01.6)
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSSS3D_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSSS3D_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: ss3d [type][v0]\n");
        Printf("SS3D Enable: type = 0, v0 = 1 enable, = 0 disable\n");
        Printf("HeadroomGain: type = 1, v0=[0~100(1.0)]\n");
        Printf("BypassGain: type = 2, v0=[0~100(1.0)]\n");
        Printf("MainTech: type = 3, v0=0(NONE),=1(WOWHD),=2(TSHD),=3(CC3D),=4(CSHP),=5(TBHD)\n");
        Printf("MainTechInputGain_NONE: type = 4, v0=[0~100(1.0)]\n");
        Printf("MainTechInputGain_WOWHD: type = 5, v0=[0~100(1.0)]\n");
        Printf("MainTechInputGain_TSHD: type = 6, v0=[0~100(1.0)]\n");
        Printf("MainTechInputGain_CC3D: type = 7, v0=[0~100(1.0)]\n");
        Printf("MainTechInputGain_CSHP: type = 8, v0=[0~100(1.0)]\n");
        Printf("MainTechInputGain_TBHD: type = 9, v0=[0~100(1.0)]\n");
        Printf("MvolEnable: type = 10, v0 = 1 enable, = 0 disable\n");
        Printf("TBHDRefOffset: type = 11, v0=[0~800]\n");
        Printf("HdLmtBoostOffset: type = 12, v0=[0~3200]\n");
        
        return -1;
    }
}
static INT32 _AprocPostSRSTSHD(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type == 1) || (u4type == 2) || (u4type == 3) || (u4type == 6) || (u4type == 7)
            || (u4type == 8) || (u4type == 9) || (u4type == 10) || (u4type == 11) || (u4type == 12)
            || (u4type == 14) || (u4type == 18) || (u4type == 20) || (u4type == 22)
            || (u4type == 24) || (u4type == 26))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;//SRS_FXP16(0.01.1)
        }
        else if((u4type == 13))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x51;//SRS_FXP16(0.01.3)
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSTSHD_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSTSHD_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: tshd [type][v0]\n");
        Printf("TSHD Enable: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("InputGain: type = 1, v0=[0~100(1.0)]\n");
        Printf("OutputGain: type = 2, v0=[0~100(1.0)]\n");
        Printf("BypassGain: type = 3, v0=[0~100(1.0)]\n");
        Printf("Sure enable: type = 4, v0 = 1 enable, = 0 disable\n");
        Printf("inmode: type = 5,v0=0x3(SRS_IO_2_0_0),=0x103(SRS_IO_LtRt)\n");
        Printf("FrontSpaceCtrl: type = 6, v0=[0~100(1.0)]\n");
        Printf("FrontCenterCtrl: type = 7, v0=[0~100(1.0)]\n");
        Printf("RearSpaceCtrl: type = 8, v0=[0~100(1.0)]\n");
        Printf("RearCenterCtrl: type = 9, v0=[0~100(1.0)]\n");
        Printf("FrontRenderGain1: type = 10, v0=[0~100(1.0)]\n");
        Printf("FrontRenderGain2: type = 11, v0=[0~100(1.0)]\n");
        Printf("TruSurLevel: type = 12, v0=[0~100(1.0)]\n");
        Printf("SpaceCtrlTune: type = 13, v0=[0~400]\n");
        Printf("MixFadeCtrl: type = 14, v0=[0~100(1.0)]\n");
        //Printf("MixFadeCtrlExt: type = 15, v0=[0~0x7fffffff]\n");
        Printf("MixEnable: type = 16,v0 = 1 enable, = 0 disable \n");
        Printf("Ldef en: type = 17, v0 = 1 enable, = 0 disable\n");
        Printf("Ldef Level: type = 18, v0=[0~100(1.0)]\n");
        Printf("Rdef en: type = 19, v0 = 1 enable, = 0 disable\n");
        Printf("Rdef Level: type = 20, v0=[0~100(1.0)]\n");
        Printf("Lfocus en: type = 21, v0 = 1 enable, = 0 disable\n");
        Printf("Lfocus Level: type = 22, v0=[0~100(1.0)]\n");
        Printf("Rfocus en: type = 23, v0 = 1 enable, = 0 disable\n");
        Printf("Rfocus Level: type = 24, v0=[0~100(1.0)]\n");
        Printf("Cfocus en: type = 25, v0 = 1 enable, = 0 disable\n");
        Printf("Cfocus Level: type = 26, v0=[0~100(1.0)]\n");

        return -1;
    }
}
static INT32 _AprocPostSRSTBHD(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type == 3) || (u4type == 4) || (u4type == 5) || (u4type == 6) || (u4type == 7)
            || (u4type == 14) )
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;//SRS_FXP16(0.01.1)
        }
        else if((u4type == 8))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x28;//SRS_FXP16(0.01.4)
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSTBHD_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSTBHD_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: tbhd [type][v0]\n");
        Printf("TBHD Enable: type = 0, v0 = 1 enable, = 0 disable\n");
        Printf("LevelIndependentEnable : type = 1,v0 = 1 indep, = 0 dep\n");
        Printf("SplitAnalysisEnable: type = 2, v0 = 1 enable, = 0 disable\n");
        Printf("input gain: type = 3, v0=[0~100(1.0)]\n");
        Printf("output gain: type = 4, v0=[0~100(1.0)]\n");
        Printf("bypass gain: type = 5, v0=[0~100(1.0)]\n");
        Printf("TruBassCtrl: type = 6, v0=[0~100(1.0)]\n");
        Printf("CompressorCtrl: type = 7, ,v0=[0~100(1.0)]\n");
        //Printf("RefLevel: type = 8, ,v0=[0~800]\n");
        //Printf("intergrant limit: type = 9, ,v0=[0~0x7fff]\n");
        Printf("AudioFilter size: type = 10, v0=0(40),=1(60),=2(100)\n");
        Printf("                        3(120),=4(150),=5(200)\n");
        Printf("                        6(250),=7(300),=8(400),=9(custom)\n");
        Printf("AnalysisFilter size: type = 11, v0=0(40),=1(60),=2(100)\n");
        Printf("                        3(120),=4(150),=5(200)\n");
        Printf("                        6(250),=7(300),=8(400),=9(custom)\n");
        Printf("mode: type = 12, v0=0(MONO),=1(STEREO)\n");
        Printf("OutputOption: type = 13,v0=0(MIXED),=1(SEPARATED)\n");
        Printf("tbq: type = 14, ,v0=[0~100(1.0)]\n");
        //Printf("dcu: type = 15, ,v0=[0~0x7fff]\n");

        return -1;
    }
}

static INT32 _AprocPostSRSTRUD(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type == 1) || (u4type == 2) || (u4type == 5) )
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;//SRS_FXP16(0.01.1)
        }
        else if((u4type == 3))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x51;//SRS_FXP16(0.01.3)
        }
        else if((u4type == 4))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x51eb85;//SRS_FXP32(0.01.3)
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSTRUD_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSTRUD_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: trud [type][v0]\n");
        Printf("TRU Dialog Enable: type = 0, v0 = 1 enable, = 0 disable,\n");
        Printf("InputGain: type = 1, v0=[0~100(1.0)]\n");
        Printf("OutputGain: type = 2, v0=[0~100(1.0)]\n");
        Printf("BypassGain: type = 3, v0=[0~100]\n");
        Printf("ProcessGain: type = 4, v0=[0~300]\n");
        Printf("ClarityGain gain: type = 5, ,v0=[0~100(1.0)]\n");
       
        return -1;
    }
}

static INT32 _AprocPostSRSTVOL(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4type, u4v0;
    
    if (i4Argc == 3 )
    {
        u4type = (UINT32)StrToInt(szArgv[1]);
        if((u4type == 2) || (u4type == 3) || (u4type == 8) )
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x147;//SRS_FXP16(0.01.1)
        }
        else if((u4type == 1))
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0xa;//SRS_FXP16(0.01.6)
        }
        else if((u4type == 4) || (u4type == 7) )
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
            u4v0 = u4v0 * 0x0147ae14;//SRS_FXP32(0.01.1)
        }
        else
        {
            u4v0 = (UINT32)StrToInt(szArgv[2]);
        }
        _vAproc_PostProcSRSTVOL_Set(u4type,u4v0);
        return 1;
    }
    else if((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        _vAproc_PostProcSRSTVOL_Get();
        return 1;
         
    }
    else
    {

        Printf("\nUsage: tvol [type][v0]\n");
        Printf("TVOL Enable: type = 0, v0 = 1 enable, = 0 disable\n");
        Printf("InputGain: type = 1, v0=[0~3200]\n");
        Printf("OutputGain: type = 2, v0=[0~100(1.0)]\n");
        Printf("BypassGain: type = 3, v0=[0~100(1.0)]\n");
        Printf("LevelIntensity: type = 4, v0=[0~100]\n");
        Printf("mode: type = 5,v0=0(LIGHT),=1(NORM) \n");
        Printf("NormalizerEn: type = 6, v0 = 1 enable, = 0 disable\n");
        Printf("NormThresh: type = 7, ,v0=[1~50]\n");
        Printf("LKFS: type = 8, ,v0=[0~100(1.0)]\n");
        Printf("SmoothEn: type = 9, v0 = 1 enable, = 0 disable\n");

        return -1;
    }
}
static INT32 _AprocPostSRSINIT(INT32 i4Argc, const CHAR ** szArgv)
{
    //aud.spdif.iecc
    UNUSED(AUD_DspIecChannel((AUD_IEC_CH_T)0));

    _vAproc_PostProcSRSAEQ_Set(0,0);
    _vAproc_PostProcSRSAEQ_Set(4,0);

    _vAproc_PostProcSRSBT_Set(0,0);

    _vAproc_PostProcSRSCC3D_Set(0,0);

    _vAproc_PostProcSRSCSHP_Set(0,0);

    _vAproc_PostProcSRSGEQ_Set(0,0);
    _vAproc_PostProcSRSGEQ_Set(10,0);

    _vAproc_PostProcSRSHL_Set(0,0);

    _vAproc_PostProcSRSHPF_Set(0,0);
    _vAproc_PostProcSRSHPF_Set(2,0);
    
    _vAproc_PostProcSRSMVOL_Set(0,0);
    
    _vAproc_PostProcSRSSPHPF_Set(0,0);
    
    _vAproc_PostProcSRSSRS3D_Set(0,0);
    
    _vAproc_PostProcSRSSS3D_Set(0,1);
    
    _vAproc_PostProcSRSTSHD_Set(0,0);
    
    _vAproc_PostProcSRSTBHD_Set(0,0);
    
    _vAproc_PostProcSRSTRUD_Set(0,0);
    
    _vAproc_PostProcSRSTVOL_Set(0,0);
    
    return 1;

}
static INT32 _AprocPostSRSGETALL(INT32 i4Argc, const CHAR ** szArgv)
{

    _vAproc_PostProcSRSAEQ_Get();

    _vAproc_PostProcSRSBT_Get();

    _vAproc_PostProcSRSCC3D_Get();

    _vAproc_PostProcSRSCSHP_Get();

    _vAproc_PostProcSRSGEQ_Get();

    _vAproc_PostProcSRSHL_Get();

    _vAproc_PostProcSRSHPF_Get();
    
    _vAproc_PostProcSRSMVOL_Get();
    
    _vAproc_PostProcSRSSPHPF_Get();
    
    _vAproc_PostProcSRSSRS3D_Get();

    _vAproc_PostProcSRSSS3D_Get();
    
    _vAproc_PostProcSRSTSHD_Get();
    
    _vAproc_PostProcSRSTBHD_Get();
    
    _vAproc_PostProcSRSTRUD_Get();
    
    _vAproc_PostProcSRSTVOL_Get();
    
    return 1;

}


#endif

static INT32 _AprocPostProcEqSpectrum(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4v0;
    
    if (i4Argc == 2 )
    {
        if(szArgv[1][0] == 'q')
        {
            _vAproc_PostProcEqSpec_Get();

        }
        else
        {
            
            u4v0 = (UINT32)StrToInt(szArgv[1]);
            _vAproc_PostProcEqSpec_Set(u4v0);
        }
        
        return 1;
    }
    else
    {

        Printf("\nUsage: spec [v0]\n");
        Printf("eq spec Enable: v0 = 1 enable, = 0 disable,\n");
	    
    	return -1;
	}
}
#endif//#ifdef CC_AUD_ARM_RENDER


#ifdef CC_AUD_SUPPORT_DBXTV
static INT32 _AudDBXTVEnable(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32  u4Enable;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Enable = (UINT32)StrToInt(szArgv[1]);
    AUD_DBXTV_Enable (u4Enable);
    
    return 1;

lbUsage:
    Printf("Current: %s\n", (AUD_DBXTV_GetEnable()) ? "Enable" : "Disable");
    Printf("Usage: en [flag]\n");
    Printf("[flag] 0: Off, 1: On\n");
    return -1;
}


static INT32 _AudDBXTVSetMode(INT32 i4Argc, const CHAR **szArgv)
{
    DBXTV_TotSonMode TotSonMode = TOTSON_OFF;
    DBXTV_TotVolMode TotVolMode = TOTVOL_OFF;
    DBXTV_TotSurMode TotSurMode = TOTSUR_OFF;
    UINT32 Enable = 0;

    AUD_DBXTV_GetMode(&TotSonMode, &TotVolMode, &TotSurMode, &Enable);
    
    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    TotSonMode = (DBXTV_TotSonMode)StrToInt(szArgv[1]);
    TotVolMode = (DBXTV_TotVolMode)StrToInt(szArgv[2]); 
    TotSurMode = (DBXTV_TotSurMode)StrToInt(szArgv[3]);
    
    if ((TotSonMode>TOTSON_OFF) || (TotVolMode >TOTVOL_OFF) || (TotSurMode > TOTSUR_OFF))
    {
        Printf("Setting mode is out of range\n");
        goto lbUsage;
    }
    //Enable = (UINT32)StrToInt(szArgv[4]);

    //AUD_DSPCmdStop (AUD_DEC_MAIN);

    AUD_DBXTV_SetMode(TotSonMode, TotVolMode, TotSurMode, Enable_All);

    //AUD_DSPCmdPlay (AUD_DEC_MAIN);

    return 1;

lbUsage:
    Printf("Usage: m [TotSonics] [TotVolume] [TotSurround] \n");
    Printf("[TotSonics]: 0: On, 1: Off\n");    
    Printf("[TotVolume]: 0: Normal, 1: Night, 2: Off\n");    
    Printf("[TotSurround]: 0: On, 1: Off\n");    
    //Printf("[Enable]: 0: Off, 0x1f: Enable All\n");    

    Printf("Current Mode: TotSonics = %d, TotVolume=%d, TotSurround=%d, Enable=0x%x\n", 
        TotSonMode, TotVolMode, TotSurMode, Enable);
    return -1;
}

#ifdef CC_AUD_ARM_POST

static INT32 _AudDBXTVRead(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Type;
    UINT32 u4Offset;
    UINT32 u4Length = 1;
    UINT32 u4Base;
    UINT32 u4MaxLenth;
    UINT32 i;
    
    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Type = (UINT32)StrToInt(szArgv[1]);
    u4Offset = (UINT32)StrToInt(szArgv[2]);
    if (i4Argc == 4)
    {
        u4Length = (UINT32)StrToInt(szArgv[3]); 
    }
    
    u4Base = u4Type ? APROC_REG_DBX_COEFF_BASE : APROC_REG_DBX_PARAM_BASE;
    u4MaxLenth = u4Type ? DBXTV_Coeff_LENGTH : DBXTV_Param_LENGTH; 

    Printf("%s: Offset=%d, Length=%d", u4Type ? "Coefficient" : "Parameter", u4Offset, u4Length);

    if (u4Offset >= u4MaxLenth)
    {
        Printf("Offset=%d is out of rang 0~%d\n", u4Offset, u4MaxLenth - 1);
        goto lbUsage; 
    }
    
    if(u4Offset + u4Length >= u4MaxLenth)
    {
        u4Length = u4MaxLenth - u4Offset;
    }

    for (i = 0; i<u4Length; i++)
    {
        if (i%4 == 0)
        {
            Printf("\n %02d | ", i + u4Offset);
        }
        Printf("0x%08x ", u4AprocReg_Read(u4Base + u4Offset + i));
    }

    Printf("\n");

    return 1;

lbUsage:
    Printf("Usage: r [type] [offset] [length] \n");
    Printf("[type]   0: Parameter, 1: Coefficient\n");
    Printf("[offset] offset of Parameter(0~%d) or Coefficient(0~%d)\n", DBXTV_Param_LENGTH - 1, DBXTV_Coeff_LENGTH - 1);    
    Printf("[length] length to read\n");    
    
    return -1;
}

static INT32 _AudDBXTVWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Type;
    UINT32 u4Offset;
    UINT32 u4Value;
    UINT32 u4Base;
    UINT32 u4MaxLenth;
    UINT32 i;
    
    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    u4Type = (UINT32)StrToInt(szArgv[1]);
    u4Offset = (UINT32)StrToInt(szArgv[2]);
    
    
    u4Base = u4Type ? APROC_REG_DBX_COEFF_BASE : APROC_REG_DBX_PARAM_BASE;
    u4MaxLenth = u4Type ? DBXTV_Coeff_LENGTH : DBXTV_Param_LENGTH; 
    
    Printf("Write %s: Offset=%d\n", u4Type ? "Coefficient" : "Parameter", u4Offset);
    for (i = 3; (i<i4Argc) && (u4Offset<u4MaxLenth); i++, u4Offset++)
    {
        u4Value = (UINT32)StrToInt(szArgv[i]); 
        vAprocReg_Write(u4Base+u4Offset, u4Value);
    }

    AUD_DBXTV_ReInit();
    
    return 1;

lbUsage:
    Printf("Usage: w [type] [offset] [value] \n");
    Printf("[type]   0: Parameter, 1: Coefficient\n");
    Printf("[offset] offset of Parameter(0~%d) or Coefficient(0~%d)\n", DBXTV_Param_LENGTH - 1, DBXTV_Coeff_LENGTH - 1);    
    Printf("[value]  value to write\n");    
    
    return -1;
}
#endif

#ifdef CC_AUD_ARM_RENDER

static INT32 _AudDBXTVRead(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Type;
    UINT32 u4Offset;
    UINT32 u4Length = 1;
    UINT32 u4Base;
    UINT32 u4MaxLenth;
    UINT32 i;
	UINT32 u1Data[100] = {0};
    
    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Type = (UINT32)StrToInt(szArgv[1]);
    u4Offset = (UINT32)StrToInt(szArgv[2]);
    if (i4Argc == 4)
    {
        u4Length = (UINT32)StrToInt(szArgv[3]); 
    }
    
    //u4Base = u4Type ? APROC_DATAPORT_DBXTV_PARAM_BASE : APROC_DATAPORT_DBXTV_COEFF_BASE;
    u4Base = u4Type ? APROC_DATAPORT_DBXTV_COEFF_BASE : APROC_DATAPORT_DBXTV_PARAM_BASE;
    u4MaxLenth = u4Type ? DBXTV_Coeff_LENGTH : DBXTV_Param_LENGTH; 

    Printf("%s: Offset=%d, Length=%d", u4Type ? "Coefficient" : "Parameter", u4Offset, u4Length);

    if (u4Offset >= u4MaxLenth)
    {
        Printf("Offset=%d is out of rang 0~%d\n", u4Offset, u4MaxLenth - 1);
        goto lbUsage; 
    }
    
    if(u4Offset + u4Length >= u4MaxLenth)
    {
        u4Length = u4MaxLenth - u4Offset;
    }
	vAprocDataPort_Read (u4Base, u1Data, u4Length);

    for (i = 0; i<u4Length; i++)
    {
        if (i%4 == 0)
        {
            Printf("\n %02d | ", i + u4Offset);
        }
        Printf("0x%08x ", u1Data[i]);
    }

    Printf("\n");

    return 1;

lbUsage:
    Printf("Usage: r [type] [offset] [length] \n");
    Printf("[type]   0: Parameter, 1: Coefficient\n");
    Printf("[offset] offset of Parameter(0~%d) or Coefficient(0~%d)\n", DBXTV_Param_LENGTH - 1, DBXTV_Coeff_LENGTH - 1);    
    Printf("[length] length to read\n");    
    
    return -1;
}

static INT32 _AudDBXTVWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Type;
    UINT32 u4Offset;
    UINT32 u4Value;
    UINT32 u4Base;
    UINT32 u4MaxLenth;
    UINT32 i;
	UINT32 u1Data[100] = {0};
	UINT32 u4Offset_copy;
	UINT32 length = 0;
    
    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    u4Type = (UINT32)StrToInt(szArgv[1]);
    u4Offset = (UINT32)StrToInt(szArgv[2]);
	u4Offset_copy = u4Offset;
    
    
    //u4Base = u4Type ? APROC_DATAPORT_DBXTV_PARAM_BASE : APROC_DATAPORT_DBXTV_COEFF_BASE;
    u4Base = u4Type ? APROC_DATAPORT_DBXTV_COEFF_BASE : APROC_DATAPORT_DBXTV_PARAM_BASE;
    u4MaxLenth = u4Type ? DBXTV_Coeff_LENGTH : DBXTV_Param_LENGTH; 
    
    Printf("Write %s: Offset=%d\n", u4Type ? "Coefficient" : "Parameter", u4Offset);
    for (i = 3; (i<i4Argc) && (u4Offset<u4MaxLenth); i++, u4Offset++)
    {
        u4Value = (UINT32)StrToInt(szArgv[i]); 
        u1Data[u4Offset] = u4Value;
		length++;
    }
	vAprocDataPort_Write (u4Base+u4Offset_copy, u1Data, length);
    
    return 1;

lbUsage:
    Printf("Usage: w [type] [offset] [value] \n");
    Printf("[type]   0: Parameter, 1: Coefficient\n");
    Printf("[offset] offset of Parameter(0~%d) or Coefficient(0~%d)\n", DBXTV_Param_LENGTH - 1, DBXTV_Coeff_LENGTH - 1);    
    Printf("[value]  value to write\n");    
    
    return -1;
}
#endif

static INT32 _AudDBXTVTestEnable(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32  u4Enable;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Enable = (UINT32)StrToInt(szArgv[1]);
    AUD_DBXTV_SetTestEnable (u4Enable);
    
    return 1;

lbUsage:
    Printf("Current: %s\n", (AUD_DBXTV_GetTestEnable()) ? "Enable" : "Disable");
    Printf("Usage: en [flag]\n");
    Printf("[flag] 0: Off, 1: On\n");
    return -1;
}

static INT32 _AudDBXTVTestMode(INT32 i4Argc, const CHAR **szArgv)
{
    DBXTV_TotSonTestMode eMode = TOTSON_NORMAL;
    
    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    eMode = (DBXTV_TotSonTestMode)StrToInt(szArgv[1]);
    
    //if (eMode >= TOTSON_NUM)
    //{
    //    Printf("Setting mode is out of range\n");
    //    goto lbUsage;
    //}
    //AUD_DSPCmdStop (AUD_DEC_MAIN);

    AUD_DBXTV_SetTestMode(eMode, Enable_All);

    //AUD_DSPCmdPlay (AUD_DEC_MAIN);

    return 1;

lbUsage:
    Printf("Usage: tm [mode]\n");
    Printf("[mode]: 0: Normal, 1: Movie, 2: Night, 3: DRC\n");    
    Printf("Current Mode: %d\n", AUD_DBXTV_GetTestMode());    
    return -1;
}
#endif



static INT32 _AprocEnable (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Enable;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Enable = (UINT32)StrToInt(szArgv[1]);

#if defined(CC_AUD_SUPPORT_DUAL_DSP)
    AUD_DSPCmdStop(AUD_DSP0, AUD_DEC_MAIN);
#else
    AUD_DSPCmdStop (AUD_DEC_MAIN);
#endif // CC_AUD_SUPPORT_DUAL_DSP

    vAproc_Enable (u4Enable);

#if defined(CC_AUD_SUPPORT_DUAL_DSP)
    AUD_DSPCmdPlay (AUD_DSP0, AUD_DEC_MAIN);
#else
    AUD_DSPCmdPlay (AUD_DEC_MAIN);
#endif // CC_AUD_SUPPORT_DUAL_DSP

    return 1;

lbUsage:
    Printf("Current: %s\n", (u4Aproc_IsEnable()) ? "Enable" : "Disable");
    Printf("Usage: enable [on/off]\n");
    return -1;
}

static INT32 _AprocIceControl (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Enable;
    UINT32 u4Val;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Enable = (UINT32)StrToInt(szArgv[1]);
#if defined(CC_AUD_ARM11_SUPPORT)
        // CKGEN
        AudAprocJtagPinmux();
        // ABIM Reset 8908H[1]
        u4Val = u4AprocReadBIM(APROC_RST);
        u4Val &= (~(0x1 << 4));
        u4Val |= ((u4Enable & 0x4) << 1);
        vAprocWriteBIM(APROC_RST, u4Val);

        return 1;

    lbUsage:
        Printf("Current: %s\n", (u4AprocReadBIM (APROC_RST) & 0x2) ? "Connectedl" : "Not connected");
        Printf("Usage: ice [1: connected/0: not connected]\n");
        return -1;
#else
    u4Val = u4AprocReadBIM (APROC_RST);
    u4Val &= (~(0x1 << 4));
    u4Val |= ((u4Enable & 0x1) << 4);
    vAprocWriteBIM  (APROC_RST, u4Val);

    return 1;

lbUsage:
    Printf("Current: %s\n", (u4AprocReadBIM (APROC_RST) & 0x10) ? "Serial" : "Not serial");
    Printf("Usage: ice [serial/not serial]\n");
    return -1;
#endif // CC_AUD_ARM11_SUPPORT
}

#if defined(CC_AUD_ARM11_SUPPORT)
static INT32 _AprocModuleCtrl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Module;
    UINT32 u4Flag;

    if(i4Argc<3)
    {
        goto lbUsage;
    }
    u4Module = (UINT32)StrToInt(szArgv[1]);
    u4Flag = (UINT32)StrToInt(szArgv[2]);

    AudAprocModuleCtrl(u4Module, u4Flag);

    return 1;

lbUsage:
    Printf("Usage: modctrl [module] [flag]\n");
    Printf("module:\n");
    Printf("    0: AOUT1\n");
    Printf("    1: AOUT2\n");
    Printf("    2: IEC\n");
    Printf("    3: PUTBS (not available for Audio ARM11)\n");
    Printf("    4: ASRC1\n");
    Printf("    5: ASRC2\n");
    Printf("    6: ASRC3\n");
    Printf("flag:\n");
    Printf("    b'00: DSPA\n");
    Printf("    b'01: DSPB\n");
    Printf("    b'10: Audio ARM11\n");
    return -1;
}

extern UINT32 _gsu4AprocSoEasyDownloadFlag;
extern UINT32 _gsu4AprocSoEasyDownloadSize;

#if 0
static INT32 _AproSoEasyDownLoad (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(_AproSoEasyDownLoad);

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    _gsu4AprocSoEasyDownloadFlag = (UINT32)StrToInt(szArgv[1]);
    _gsu4AprocSoEasyDownloadSize = (UINT32)StrToInt(szArgv[2]);

    return 1;

lbUsage:
    Printf("Current Address: 0x%x, load code to here\n", _gsu4AprocSoEasyDownloadFlag);
    Printf("Current Size: 0x%x\n", _gsu4AprocSoEasyDownloadSize);
    Printf("Usage: sedl [address] [size]\n");
    return -1;
}
#endif

#endif // CC_AUD_ARM11_SUPPORT

static INT32 _AprocPostProcQuery (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  i;

    if (i4Argc < 1)
    {
        goto lbUsage;
    }

    Printf ("\n[Working]");
    for (i = 0x200; i <= 0x300; i ++) // hard code, fix me!!!!
    {
        if ((i%4) == 0)
        {
            Printf("\n(0x%04x) | %08x ", i, u4AprocReg_Read(i));
        }
        else
        {
            Printf("%08x ", u4AprocReg_Read(i));
        }
    }

    Printf ("\n\n[Setting]");
    for (i = 0x300; i <= 0x400; i ++)
    {
        if ((i%4) == 0)
        {
            Printf("\n(0x%04x) | %08x ", i, u4AprocReg_Read(i));
        }
        else
        {
            Printf("%08x ", u4AprocReg_Read(i));
        }
    }

    return 1;

lbUsage:
    Printf("Usage: pq\n");
    return -1;
}

static INT32 _AprocMemMapQuery(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 1)
    {
        Printf("Usage: qm\n");
        return -1;
    }

    Printf("\n@ MainARM\n");
    Printf("[work space] (P) addr = 0x%x, (V)  addr = 0x%x, size = 0x%x\n",
        u4GetAprocMemoryMap(APROC_MEMMAP_WORKBUF_PHY),
        u4GetAprocMemoryMap(APROC_MEMMAP_WORKBUF),
        u4AprocGetWorkSpaceSize());
    Printf("\n@ AudioProcessor\n");

    Printf("[dataport] addr = 0x%x\n", u4GetAprocMemoryMap(APROC_MEMMAP_DATAPORT));
    Printf("[regmap] addr = 0x%x\n", u4GetAprocMemoryMap(APROC_MEMMAP_REGMAP));
    Printf("[asdp dram ] addr = 0x%x\n", u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_DRAM));
    Printf("[asdp cmpt ] addr = 0x%x\n", u4GetAprocMemoryMap(APROC_MEMMAP_ADSP_CMPT));

    return 0;
}

static INT32 _AprocRegRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, u4Count = 1, i = 0;
    UINT32 u4Tmp;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    if (i4Argc == 3)
    {
        u4Count= (UINT16)StrToInt(szArgv[2]);
    }

    while (i < u4Count)
    {
        if ((i%4) == 0)
        {
            u4Tmp = u4AprocReg_Read(u4Addr);
            Printf("\n(0x%04x) | %08x ", u4Addr, u4Tmp);
        }
        else
        {
            u4Tmp = u4AprocReg_Read(u4Addr);
            Printf("%08x ", u4Tmp);
        }
        u4Addr+=1;
        i++;
    }

    return 1;

lbUsage:
    Printf("Usage: regread [addr] [cnt]\n");
    return -1;
}

static INT32 _AprocRegWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Addr;
    UINT32  u4Data;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    u4Data = (UINT32)StrToInt(szArgv[2]);

    vAprocReg_Write (u4Addr, u4Data);

    return 1;

lbUsage:
    Printf("Usage: regwrite [addr] [data]\n");
    return -1;
}


static INT32 _AprocCmdOpen(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Idx;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Idx = (UINT32)StrToInt(szArgv[1]);

    vAprocCmdOpen (u4Idx);

    return 1;

lbUsage:
    Printf("Usage: open [idx]\n");
    return -1;
}

static INT32 _AprocNotifyAdsp(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Item, u4Type, u4Data;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    u4Item = (UINT32)StrToInt(szArgv[1]);
    u4Type = (UINT32)StrToInt(szArgv[2]);
    u4Data = (UINT32)StrToInt(szArgv[3]);

    vAproc_NotifyAdsp (u4Item, u4Type, u4Data);

    return 1;

lbUsage:
    Printf("Usage: nod [Item] [type] [data]\n");
    return -1;
}

static INT32 _AprocWriteAdsp(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Addr, u4Data;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    u4Data = (UINT32)StrToInt(szArgv[2]);

    vAproc_NotifyAdsp (u4Addr, 0x2, u4Data);

    return 1;

lbUsage:
    Printf("Usage: wd [addr] [data]\n");
    return -1;
}

#ifdef CC_AUD_ARM_RENDER
static INT32 _AprocPowerOn (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("_AprocPowerOn\n");

    vAprocMain_Open ();    

    return 1;

}

static INT32 _AprocPowerDown (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("_AprocPowerDown\n");    

    vAprocMain_Close ();

    return 1;

}

static INT32 _AprocPostProcSgain (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Id;
    UINT32  u4Data, u4Data0, u4Data1, u4Data2, u4Data3, u4Data4;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Id = (UINT32)StrToInt(szArgv[1]);
    u4Data = (UINT32)StrToInt(szArgv[2]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SGAIN, u4Id, (UINT32 *) &u4Data, 1); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_SGAIN, APROC_IOCTRL_SGAIN_DSP_PERIOD, (UINT32 *) &u4Data0, 1); 
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_SGAIN, APROC_IOCTRL_SGAIN_AMIXER_PERIOD, (UINT32 *) &u4Data1, 1); 
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_SGAIN, APROC_IOCTRL_SGAIN_MUTE_AMIXER0_PERIOD, (UINT32 *) &u4Data2, 1); 
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_SGAIN, APROC_IOCTRL_SGAIN_MUTE_AMIXER1_PERIOD, (UINT32 *) &u4Data3, 1); 
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_SGAIN, APROC_IOCTRL_SGAIN_MUTE_AMIXER2_PERIOD, (UINT32 *) &u4Data4, 1); 
    Printf("Current: DSP = 0x%x, AMIXER = 0x%x, MUTE = 0x%x, 0x%x, 0x%x\n", 
        u4Data0, u4Data1, u4Data2, u4Data3, u4Data4);
    Printf("Usage: sg [id] [data]\n");
    return -1;
}

static INT32 _AprocPostProcAdMixing (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Value[2];

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Value[0] = (UINT32)StrToInt(szArgv[1]); // flag
    u4Value[1] = (UINT32)StrToInt(szArgv[2]); // mode

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_AD, 0, (UINT32 *) &u4Value[0], 2); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_AD, 0, (UINT32 *) &u4Value[0], 2); 
    Printf("Current: Flag = 0x%x, Mode = 0x%x\n", u4Value[0], u4Value[1]);
    Printf("Usage: ad [flag] [mode]\n");
    return -1;
}

static INT32 _AprocPostProcMonoMixing (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Value;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Value= (UINT32)StrToInt(szArgv[1]); // flag

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_MONOMIX, 0, (UINT32 *) &u4Value, 1); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_MONOMIX, 0, (UINT32 *) &u4Value, 1); 
    Printf("Current: Flag = 0x%x\n", u4Value);
    Printf("Usage: monomix [flag]\n");
    return -1;
}

static INT32 _AprocIecCs (INT32 i4Argc, const CHAR ** szArgv)
{
    APROC_IEC_CHANNELSTATUS_T  eInfo;

    if (i4Argc < 11)
    {
        goto lbUsage;
    }

    eInfo.u4CategoryCode= (UINT32)StrToInt(szArgv[1]);
    eInfo.u4WordLength = (UINT32)StrToInt(szArgv[2]);
    eInfo.u4SampleRate = (UINT32)StrToInt(szArgv[3]);
    eInfo.u4AdditionalInfo = (UINT32)StrToInt(szArgv[4]);
    eInfo.u4CopyRight = (UINT32)StrToInt(szArgv[5]);
    eInfo.u4DigitalInfo = (UINT32)StrToInt(szArgv[6]);
    eInfo.u4SourceNum = (UINT32)StrToInt(szArgv[7]);
    eInfo.u4ChannelNum0 = (UINT32)StrToInt(szArgv[8]);
    eInfo.u4ChannelNum1 = (UINT32)StrToInt(szArgv[9]);
    eInfo.u4ClockAccuracy = (UINT32)StrToInt(szArgv[10]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_CHANNELSTATUS, (UINT32 *) &eInfo, 10); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_CHANNELSTATUS, (UINT32 *) &eInfo, 10); 

    Printf("Current: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", 
        eInfo.u4CategoryCode,eInfo.u4PcmRawInfo, eInfo.u4WordLength, eInfo.u4SampleRate, eInfo.u4AdditionalInfo, eInfo.u4CopyRight,
        eInfo.u4DigitalInfo, eInfo.u4SourceNum, eInfo.u4ChannelNum0, eInfo.u4ChannelNum1, eInfo.u4ClockAccuracy);
    Printf("Usage: cs [categorycode] [pcmraw] [wordlen] [samplerate] [addition] [copyright] [digital] [sourcenum] [chnum0] [chnum1] [clock]\n");
    return -1;
}

static INT32 _AprocIecCopyProtect (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Data[2];

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Data[0] = (UINT32)StrToInt(szArgv[1]);
    u4Data[1] = (UINT32)StrToInt(szArgv[2]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_COPYPROTECT, (UINT32 *) &u4Data[0], 2); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_COPYPROTECT, (UINT32 *) &u4Data[0], 2); 

    Printf("Current: L-bit = 0x%x, C-bit = 0x%x\n", u4Data[0], u4Data[1]);
    Printf("Usage: protect [L-bit] [C-bit]\n");
    return -1;
}

static INT32 _AprocIecRawInfo (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Data[2];

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u4Data[0] = (UINT32)StrToInt(szArgv[1]);
    u4Data[1] = (UINT32)StrToInt(szArgv[2]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_RAWINFO, (UINT32 *) &u4Data[0], 2); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_RAWINFO, (UINT32 *) &u4Data[0], 2); 

    Printf("Current: burstinfo = 0x%x, nsum = 0x%x\n", u4Data[0], u4Data[1]);
    Printf("Usage: raw [burstinfo] [nsum]\n");
    return -1;
}

static INT32 _AprocIecRawInfoAudFmt (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Data[2];

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Data[0] = (UINT32)StrToInt(szArgv[1]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_RAWAUDFMT, (UINT32 *) &u4Data[0], 1); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_RAWINFO, (UINT32 *) &u4Data[0], 2); 

    Printf("Current: burstinfo = 0x%x, nsum = 0x%x\n", u4Data[0], u4Data[1]);
    Printf("Usage: audraw [audio format]\n");
    return -1;
}


static INT32 _AprocIecMute (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Data[2];

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Data[0] = (UINT32)StrToInt(szArgv[1]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_MUTE, (UINT32 *) &u4Data[0], 1); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_MUTE, (UINT32 *) &u4Data[0], 1); 

    Printf("Current: PCM = 0x%x, RAW = 0x%x\n", u4Data[0], u4Data[1]);
    Printf("Usage: mute [mute]\n");
    return -1;
}

static INT32 _AprocIecRawDelay (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Data;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Data = (UINT32)StrToInt(szArgv[1]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_RAWDELAY, (UINT32 *) &u4Data, 1); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_RAWDELAY, (UINT32 *) &u4Data, 1); 

    Printf("Current: 0x%x\n", u4Data);
    Printf("Usage: delay [frame]\n");
    return -1;
}

static INT32 _AprocIecPcmChSel (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Data;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Data = (UINT32)StrToInt(szArgv[1]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_PCMCHSEL, (UINT32 *) &u4Data, 1); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_PCMCHSEL, (UINT32 *) &u4Data, 1); 

    Printf("Current: 0x%x\n", u4Data);
    Printf("Usage: ch [chsel]\n");
    Printf("[chsel] 0 = L/R, 1 = LS/RS, 2 = CSW, 3 = IEC, 4 = SPDIFIn/LineIn, 5 = HP, \n");
    Printf("[chsel] 6 = LineIn2, 7 = LineIn3, 8 = Mon, 9 = CH13/14, 10 = AUX\n");

    return -1;
}

static INT32 _AprocIecOutputSel (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Data;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Data = (UINT32)StrToInt(szArgv[1]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_OUTPUT, (UINT32 *) &u4Data, 1); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_OUTPUT, (UINT32 *) &u4Data, 1); 

    Printf("Current: 0x%x\n", u4Data);
    Printf("Usage: out [output]\n");
    Printf("[output] 0 = mute (light-off), 1 = unmute, 2 = HDMI bypass\n");

    return -1;
}

static INT32 _AprocIecPcmBitsSel (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Data;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Data = (UINT32)StrToInt(szArgv[1]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_PCMBITS, (UINT32 *) &u4Data, 1); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_PCMBITS, (UINT32 *) &u4Data, 1); 

    Printf("Current: 0x%x\n", u4Data);
    Printf("Usage: bit [bits]\n");
    Printf("[chsel] 0 = 24-bits, 2 = 20-bits, 3 = 16-bits\n");

    return -1;
}


static INT32 _AprocIecFlag (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Data;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Data = (UINT32)StrToInt(szArgv[1]);

    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_FLAG, (UINT32 *) &u4Data, 1); 

    return 1;

lbUsage:
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_FLAG, (UINT32 *) &u4Data, 1); 

    Printf("Current: 0x%x\n", u4Data);
    Printf("Usage: flag [flag]\n");
    return -1;
}

static INT32 _SpostDbg (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Data[256];
    UINT32 i;

    if (i4Argc == 1)
    {
        goto lbUsage;
    }
    i4Argc -= 1;
    for(i=0;i<i4Argc;i++)
    {
        u1Data[i] = (UINT8)StrToInt(szArgv[i+1]);
    }
    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_DBG, 0, (UINT32 *)u1Data, i4Argc); 
    _vAUD_Aproc_Get (APROC_CONTROL_TYPE_DBG, 0, (UINT32 *)u1Data, 0); 

    return 1;

lbUsage:    
    Printf("[Usage] dbg xx xx xx xx .....\n");
    return -1;
}

static INT32 _SpostBypass (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 isBypass;

    if (i4Argc == 1)
    {
        goto lbUsage;
    }
    isBypass = (UINT32)StrToInt(szArgv[1]);    
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_COMM_2, APROC_REG_BYPASS_DSP), isBypass);    

    return 1;

lbUsage:    
    Printf("[Usage] b [bypass]\n");
    Printf("1:bypass on, 0: bypass off\n");
    return -1;
}

static INT32 _SpostLogoutEnable (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 isEnable;

    if (i4Argc == 1)
    {
        goto lbUsage;
    }
    isEnable = (UINT32)StrToInt(szArgv[1]);    
    vAprocReg_Write (APROC_ASM_ADDR (APROC_ASM_ID_3RD_POST_23, APROC_REG_SPOST_LOGOUT_FLAG), isEnable);    

    return 1;

lbUsage:    
    Printf("[Usage] lo [enable]\n");
    Printf("1:log out on, 0: log out off\n");
    return -1;
}

typedef enum {
    APROC_DEV_ID_TEST_TONE = 0,
    APROC_DEV_ID_TEST_OUTPUT,
    APROC_DEV_ID_DEC0,
    APROC_DEV_ID_DEC1,
    APROC_DEV_ID_DEC2,
    APROC_DEV_ID_AUDOUT0,
    APROC_DEV_ID_AUDOUT1,
    APROC_DEV_ID_AUDOUT2,
    APROC_DEV_ID_SRC0,
    APROC_DEV_ID_SRC1,
    APROC_DEV_ID_SRC2,
    APROC_DEV_ID_MIXPORT0,
    APROC_DEV_ID_MIXPORT1,
    APROC_DEV_ID_OUTPUT,
    APROC_DEV_ID_OUTPUT2,
    APROC_DEV_ID_IEC,
    APROC_DEV_ID_CPU,
    APROC_DEV_ID_SDSRC0,
    APROC_DEV_ID_SDSRC1,
    APROC_DEV_ID_SDSRC2,
    APROC_DEV_ID_DELAY0,  //parson delay
    APROC_DEV_ID_DELAY1,
    APROC_DEV_ID_DELAY2,   
    APROC_DEV_ID_DSP,
    APROC_DEV_ID_ROUTINE,    
    APROC_DEV_ID_AD,    
    APROC_DEV_ID_RISC_POST_UP,
    APROC_DEV_ID_RISC_POST_DOWN,     
    APROC_DEV_ID_NUM
} APROC_DEV_ID_T;

static char _gcAprocDevName[][32] = {
 "Test tone    ",
 "Test output  ",
 "Dec 0        ",
 "Dec 1        ",
 "Dec 2        ",
 "Amixer 0     ",
 "Amixer 1     ",
 "Amixer 2     ",
 "SRC 0        ",
 "SRC 1        ",
 "SRC 2        ",
 "Mixsnd 0     ",
 "Mixsnd 1     ",
 "Output       ",
 "Output2      ",
 "IEC          ",
 "Upload CPU   ",
 "SDSRC 0      ",
 "SDSRC 1      ",
 "SDSRC 2      ",
 "DELAY 0      ",
 "DELAY 1      ",
 "DELAY 2      ",
 "DSP          ", 
 "ROUTINE      ",  
 "AD           ",
 "POST_UPBUF   ",  //parson riscpost
 "POST_DOWNBUF ",
 
};

static char _gcAprocIoName[][32] = {
 "Test tone    ",
 "Test output  ",
 "Dec 0        ",
 "Dec 1        ",
 "Dec 2        ",
 "Amixer 0     ",
 "Amixer 1     ",
 "Amixer 2     ",
 "SRC 0        ",
 "SRC 1        ",
 "SRC 2        ",
 "Mixsnd 0     ",
 "Mixsnd 1     ",
 "Mixsnd 2     ",
 "Mixsnd 3     ",
 "Output       ",
 "Output2      ",
 "IEC          ",
 "Upload0      ",
 "Upload1      ",
 "Upload2      ",
 "SDSRC 0      ",
 "SDSRC 1      ",
 "SDSRC 2      ",
 "DELAY 0      ",
 "DELAY 1      ",
 "DELAY 2      ",
 "AD           ",
 "POST_UPBUF   ",  //parson riscpost
 "POST_DOWNBUF ",
 
};

static char _gcAprocDevIoApiName[][32] = {
 "Outside  ",
 "Open     ",
 "Close    ",
 "Set      ",
 "Get      ",
 "Read     ",
 "Write    ",
};

static char _gcAprocDevStatusName[][32] = {
 "Sleep",
 "Idle",
 "Working",
};

static char _gcAprocMosThreadStatusName[][32] = {
 "Idle",
 "Running",
 "Sleep",
 "Blocked",
 "Delayed",
 "Dead",
};


typedef struct {
    UINT32 u4MaxDev;
    UINT32 u4IoApiStatus[APROC_DEV_ID_NUM]; 
    UINT32 u4DevStatus[APROC_DEV_ID_NUM]; // APROC_DEV_STATUS_T
} APROC_DEV_INFO_T;

APROC_DEV_INFO_T *_pgsAprocDevInfo;


#define APROC_MOS_MAX_THREAD 32

/* Thread structure
*/
typedef struct {
    UINT32 *stack_ptr; // stack pointer
    INT32 tid; // thread ID
    CHAR *name; // thread name
    UINT32 state; // thread state
    UINT32 time_quantum; // time quantum
    UINT32 delayed_time; // delay time
} THREAD_STRUCT_T;

/* Thread list information
*/
typedef struct {
    UINT32 flag[APROC_MOS_MAX_THREAD]; // thread on/off flag
    UINT32 list[APROC_MOS_MAX_THREAD]; // thread created list
    INT32 curId; // current thread ID
    INT32 totalThreadCount; // total thread count
    INT32 totalCswCount; // total context switch count
    INT32 isOsStart; // is OS start or not?
} THREAD_LIST_T;

UINT32 _gu4AprocMosThreadTblBase;
THREAD_STRUCT_T *_pgsAprocMosThreadInfo;
THREAD_LIST_T *_pgsAprocMosThreadListInfo;


//---------------------------------------------------------------------------
// IO types
typedef enum {
    IO_BUF_TEST_TONE = 0,
    IO_BUF_TEST_OUTPUT,
    IO_BUF_DEC0BUF,
    IO_BUF_DEC1BUF,
    IO_BUF_DEC2BUF,
    IO_BUF_AUDOUT0BUF,
    IO_BUF_AUDOUT1BUF,
    IO_BUF_AUDOUT2BUF,
    IO_BUF_SRC0INBUF,
    IO_BUF_SRC1INBUF,
    IO_BUF_SRC2INBUF,
    IO_BUF_MIXPORT0BUF,
    IO_BUF_MIXPORT1BUF,
    IO_BUF_OUTPUTBUF,
    IO_BUF_OUTPUT2BUF,
    IO_BUF_IECBUF,
    IO_BUF_UPLOADBUF,
    IO_BUF_SDSRC0BUF,
    IO_BUF_SDSRC1BUF,
    IO_BUF_SDSRC2BUF,
    IO_BUF_DELAY0BUF,  // 20
    IO_BUF_DELAY1BUF,
    IO_BUF_DELAY2BUF,
    IO_BUF_AD,
    IO_BUF_RISCPOST_UPBUF,  //parson riscpost
    IO_BUF_RISCPOST_DOWNBUF,     
    IO_BUF_NUM
} APROC_IO_BUFFER_ID_T;

// IO buffer info
typedef struct {
    UINT32 u4Str;// start address
    UINT32 u4End; // total block number
    UINT32 u4Size; // bytes in one block per channel
    UINT32 u4Ch; // channel number
    UINT32 u4Wcurp; // Write port, current pointer
    UINT32 u4Rcurp; // Read port, current pointer
    UINT32 u4Wp; // write pointer
    UINT32 u4Rp; // read pointer
    UINT32 u4SiBase; // sideinformation base address
} APROC_IO_BUFFER_T;

APROC_IO_BUFFER_T *_pgsAprocIoBufBase;


static INT32 _AprocQuery (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, j;
    UINT32 u4Type, u4Id;
    UINT32 u4Num;
    CHAR *cString;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Type = (UINT32)StrToInt(szArgv[1]);
    u4Id = 0xffffffff;
    if (i4Argc == 3)
    {
        u4Id = (UINT32)StrToInt(szArgv[2]);
    }

    if (u4Type == 0)
    {
        _pgsAprocDevInfo = (APROC_DEV_INFO_T *) u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_COMM_1, APROC_REG_PROFILE_MOS_DEVINFO_BASE));
    
        Printf ("APROC Device Info:\n");
        for (i = 0; i < APROC_DEV_ID_NUM; i ++)
        {
            Printf ("    %s: %s, %s\n", _gcAprocDevName[i],
                 &_gcAprocDevIoApiName[u4ReadProc((UINT32) &_pgsAprocDevInfo->u4IoApiStatus[i])],
                 &_gcAprocDevStatusName[u4ReadProc((UINT32) &_pgsAprocDevInfo->u4DevStatus[i])]);
        }
    }
    else if (u4Type == 1)
    {
        _pgsAprocIoBufBase = (APROC_IO_BUFFER_T *) u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_COMM_1, APROC_REG_PROFILE_IOBUF_BASE));

        Printf ("APROC IO Buffer Info:\n");
        Printf ("                    Start      End    Size   Ch     Wcp    Rcp    Wp     Rp\n");
        
        if (u4Id == 0xffffffff)
        {
            u4Id = 0;
            u4Num = IO_BUF_NUM;
        }
        else
        {
            u4Num = u4Id + 1;
        }
        
        for (i = u4Id; i < u4Num; i ++)
        {
            Printf ("[%2d] %s: 0x%08x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x\n", 
                 i,
                 _gcAprocIoName[i],
                 u4ReadProc((UINT32) &_pgsAprocIoBufBase[i].u4Str),
                 u4ReadProc((UINT32) &_pgsAprocIoBufBase[i].u4End),
                 u4ReadProc((UINT32) &_pgsAprocIoBufBase[i].u4Size),
                 u4ReadProc((UINT32) &_pgsAprocIoBufBase[i].u4Ch),
                 u4ReadProc((UINT32) &_pgsAprocIoBufBase[i].u4Wcurp),
                 u4ReadProc((UINT32) &_pgsAprocIoBufBase[i].u4Rcurp),
                 u4ReadProc((UINT32) &_pgsAprocIoBufBase[i].u4Wp),
                 u4ReadProc((UINT32) &_pgsAprocIoBufBase[i].u4Rp));
        }
    }
    else if (u4Type == 2)
    {
        _gu4AprocMosThreadTblBase = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_COMM_1, APROC_REG_PROFILE_MOS_THREADTBL_BASE));
        _pgsAprocMosThreadListInfo = (THREAD_LIST_T *) u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_COMM_1, APROC_REG_PROFILE_MOS_THREADLIST_BASE));
        
        Printf ("APROC MOS thread Info:\n");

        Printf ("    Current = 0x%d\n", u4ReadProc((UINT32) &_pgsAprocMosThreadListInfo->curId));
        u4Num = u4ReadProc((UINT32) &_pgsAprocMosThreadListInfo->totalThreadCount);
        if (u4Num > APROC_MOS_MAX_THREAD)
        {
            u4Num = APROC_MOS_MAX_THREAD;
        }
        
        
        for (i = 0; i < u4Num; i ++)
        {
            _pgsAprocMosThreadInfo = (THREAD_STRUCT_T *) (_gu4AprocMosThreadTblBase + i * sizeof (THREAD_STRUCT_T));
            cString = (CHAR *) u4GetAprocMemoryMap(APROC_MEMMAP_WORKBUF) + u4ReadProc((UINT32) &_pgsAprocMosThreadInfo->name);
            
            Printf ("    [0x%x] ", u4ReadProc((UINT32) &_pgsAprocMosThreadInfo->tid));            
            for (j = 0; j < 16; j ++)
            {
                Printf ("%c", *cString ++);
            }
            Printf (": %s\n", &_gcAprocMosThreadStatusName[u4ReadProc((UINT32) &_pgsAprocMosThreadInfo->state)][0]);
            
        }
    }
    else
    {
        Printf ("Error type = 0x%x!!!!\n", u4Type);
    }
    
    return 1;

lbUsage:
    Printf("Usage: q [type]\n");
    Printf("      [type] 0 = device info, 1 = IO buffer info, 2 = thread info\n");
    return -1;    
}

static INT32 _AprocCmdAmxierOpen(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Idx;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Idx = (UINT32)StrToInt(szArgv[1]);

    vAprocDec_Open (u4Idx);

    return 1;

lbUsage:
    Printf("Usage: ao [MixerId]\n");
    return -1;
}

static INT32 _AprocCmdAmxierClose(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Idx;

    if (i4Argc < 2)
    {
        goto lbUsage;
    }

    u4Idx = (UINT32)StrToInt(szArgv[1]);

    vAprocDec_Close (u4Idx);

    return 1;

lbUsage:
    Printf("Usage: ac [MixerId]\n");
    return -1;
}


static INT32 _AprocMixsndTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Init;
    UINT8 u1ID;    
    
    if (i4Argc != 3)
    {
        Printf("Usage: ms [mixsnd ID] [0/1]\n");
        Printf("  mixsnd ID : 0/1\n");        
        Printf("  0/1       : 0 - disable, 1 - enable\n");
        return -1;
    }

    u1ID = (UINT8)StrToInt(szArgv[1]);
    u1Init = (UINT8)StrToInt(szArgv[2]);

    if (u1Init)
    {
        AUD_AprocEnableMixSnd(u1ID);
    }
    else
    {
        AUD_AprocDisableMixSnd(u1ID);
    }

    return 0;
}
#if defined(CC_AUD_APROC_KEYSOUND)
static INT32 _AprocKeysndTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Init;
   
    if (i4Argc != 2)
    {
        Printf("Usage: ks [0/1]\n");
        Printf("  0/1       : 0 - disable, 1 - enable\n");
        return -1;
    }

    u1Init = (UINT8)StrToInt(szArgv[1]);

    if (u1Init)
    {
        AUD_AprocEnableKeySnd();
    }
    else
    {
        AUD_AprocDisableKeySnd();
    }

    return 0;
}
#endif

static INT32 _AprocUploadTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Init;

    if (i4Argc != 2)
    {
        Printf("Usage: up [0/1]\n");
        Printf("  0 - disable, 1 - enable\n");
        return -1;
    }

    u1Init = (UINT8)StrToInt(szArgv[1]);

    if (u1Init)
    {
        AUD_AprocEnableUpload();
    }
    else
    {
        AUD_AprocDisableUpload();
    }

    return 0;
}

static INT32 _AprocRiscPostTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Init;

    if (i4Argc != 2)
    {
        Printf("Usage: rp [0/1]\n");
        Printf("  0 - disable, 1 - enable\n");
        return -1;
    }

    u1Init = (UINT8)StrToInt(szArgv[1]);

    if (u1Init)
    {
        AUD_AprocEnableRiscPost();
    }
    else
    {
        AUD_AprocDisableRiscPost();
    }

    return 0;
}

extern void AUD_Aproc_Test_Cmd(UINT8 u1DecID, UINT8 u1Cmd, UINT8 u1FrameNum);
extern void AUD_Aproc_Log_ID(UINT8 u1DecID);
extern void AUD_Aproc_Log_Items(UINT8 u1Value);
extern void AUD_Aproc_Qry_Sync(UINT8 u1DecID);
static INT32 _AudCmdLogID(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DecId;
    UINT16 u2Item;
    UINT16 fgOnOff;
    UINT16 u2CurrLvl = 0;;

    if (i4Argc < 2)
    {
        Printf("Usage: ld  [decoder ID] ( [Item] [OnOff] )\n");
        Printf("[decoder ID] = 0/1/2/5 \n");
        Printf("  0=Main, 1=Aux, 2=Third, 5=All\n");
        Printf("[Item] = 0/1 \n");
        Printf("  0=Aproc Report PTS, 1=Aproc Update PTS, 15=ALL\n");
        Printf("[OnOff] = 0/1\n");
        Printf("  0=Off, 1=On, default is Off\n");        
    }
    
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    AUD_Aproc_Log_ID(u1DecId);

    if (i4Argc == 4)
    {
        u2Item = (UINT16)StrToInt(szArgv[2]);
        fgOnOff = (UINT16)StrToInt(szArgv[3]);
        if (u2Item == 15)   // ALL On or Off
        {
            if (fgOnOff)
            {
                u2CurrLvl = (UINT16)(AUD_DBG_APROC_PTS|AUD_DBG_APROC_UPDATE_STC);
            }
            else
            {
                u2CurrLvl = (UINT16)0;
            }
        }
        else
        {
            if (fgOnOff)
            {
                u2CurrLvl |= (1<<u2Item);
            }
            else
            {
                u2CurrLvl &= (~(1<<u2Item));
            }
        }        
        AUD_Aproc_Log_Items(u2CurrLvl);
    }

    return 0;    
}

static INT32 _AudCmdSyncInfo(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DecId;

    if (i4Argc < 2)
    {
        Printf("Usage: si  [decoder ID]\n");
        return -1;
    }
    
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    AUD_Aproc_Qry_Sync(u1DecId);

    return 0;
}

static INT32 _AudCmdAproc(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Cmd;
    UINT8 u1FrameNum;    
    
    if (i4Argc < 3)
    {
        Printf("Usage: amcd  [decoder ID] [command] [frame_num]\n");
        Printf("[decoder ID = 0/1/2 \n");
        Printf("[command] 0=wait_on  1=wait_off  2=wait_offon  3=skip  4=Disable Test\n");
        Printf("[frame_num] number of frame to skip\n");                
    }
    
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Cmd = (UINT8)StrToInt(szArgv[2]);
    u1FrameNum = 0;
    if (i4Argc == 4)    
    {
        u1FrameNum = (UINT8)StrToInt(szArgv[3]);
    }

    Printf("u1DecId=%d, Cmd=%d, FrameNum=%d\n", u1DecId, u1Cmd, u1FrameNum);
    AUD_Aproc_Test_Cmd(u1DecId, u1Cmd, u1FrameNum);
    
    return 0;

}

#ifdef CC_DUAL_AUD_DEC_SUPPORT
extern UINT8 _u1MW_SetFocusOn;
static INT32 _AprocSwapFocus(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Focus;
    
    UNUSED(u1DecId);
    if (i4Argc < 2)
    {
        Printf("Usage: swp [focus]\n");
//        Printf("[decoder ID] ignore \n");
        Printf("[focus] 0:SP main, HP aux, 1~4:SP aux, HP main\n");       
        Printf("Current _u1MW_SetFocusOn = %d\n", _u1MW_SetFocusOn);     
        return 0;
    }
    
//    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1DecId = 0; //UNUSED now.
    u1Focus = (UINT8)StrToInt(szArgv[1]);

    AUD_Aproc_SetFocus(u1DecId, u1Focus);
    
    return 0;

}

static INT32 _AprocShowSkipList(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		return 0;
	}
	AUD_Aproc_Show_SkipList();
	return 1;
}
#endif //  CC_DUAL_AUD_DEC_SUPPORT

#endif //CC_AUD_ARM_RENDER

#endif

/****************************************************************************
** dsp Command
****************************************************************************/

static INT32 _AdspDSPInit(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("_AdspDSPInit\n");

    vADSPTaskInit();

    return 1;

}

static INT32 _AdspDSPPC(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId;

    if (i4Argc == 2)
    {
        u1DspId = (UINT8)StrToInt(szArgv[1]);
        if (u1DspId > 1)
        {
            goto lbUsage; 
        }
        if (u1DspId==0)
        {
            Printf("dsp0 pc = %x\n", AUD_DspGetPc(AUD_DSP0));
        }
        if (u1DspId==1)
        {
            Printf("dsp1 pc = %x\n", AUD_DspGetPc(AUD_DSP1));
        }
    }    
    else 
        goto lbUsage; 

    return 1;

lbUsage:
    Printf("Usage: dsp.pc [dsp id]\n");
    Printf("[dsp id]    0:DSP0  1:DSP1 \n");
    return -1;
}

static INT32 _AdspDSPShareInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId;
    UINT8 u1GroupId,u1GroupNumber;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1GroupId = StrToInt(szArgv[2]);
    if ((u1DspId > 1) || (u1GroupId>64))
    {
        goto lbUsage;
    }

    if (u1GroupId !=64)
    {
        DSP_ShareInforDump(u1DspId, u1GroupId);
    }
    else
    {
        for(u1GroupNumber=0;u1GroupNumber<64;u1GroupNumber++)
        {
            DSP_ShareInforDump(u1DspId, u1GroupNumber);
        }
    }
    return 1;

lbUsage:
    Printf("Usage: sh [dsp id][group id]\n");
    Printf("[dsp id]    0:DSP0  1:DSP1 \n");
    Printf("  group_id: 0~63 (64 will dump all group info\n");
    return -1;
}

// Light added for AVC
static INT32 _AdspDSPSharInfoWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId, u1GroupId, u1ByteAddr,u1Size;
    UINT32 u4Value;

    if(i4Argc!=6) {
        Printf("Usage: shw [dsp id][group id] [byte address][value][size]\n");
        Printf("[dsp id]    0:DSP0  1:DSP1 \n");
        return -1;
    }
    u1DspId = StrToInt(szArgv[1]);
    u1GroupId = StrToInt(szArgv[2]);
    u1ByteAddr = StrToInt(szArgv[3]);
    u4Value = StrToInt(szArgv[4]);
    u1Size = StrToInt(szArgv[5]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("dsp id range error \n");
        return -1;
    }
    switch(u1Size)
    {
        case 2:
           DSP_ShareInforWriteWord(u1DspId, u1GroupId*32+u1ByteAddr,u4Value);
       break;
       default:
       break;
    }

    return 1;
}

static INT32 _AdspDSPCommonDram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId;
    UINT32 u4SrcAddr;
    UINT32 u4Len;

    if (i4Argc != 4)
    {
        goto lbUsage;
    }

    u1DspId = StrToInt(szArgv[1]);
    u4SrcAddr = StrToInt(szArgv[2]);
    u4Len = StrToInt(szArgv[3]);

    if ((u4Len == 0)||(u1DspId >= AUD_DSP_NUM))
    {
        goto lbUsage;
    }
    DSP_CommDramDump(u1DspId, u4SrcAddr, u4Len);

    return 1;

    lbUsage:
    Printf("Usage: cm [dsp id][addr] [len]\n");
    Printf("[dsp id]    0:DSP0  1:DSP1 \n");
    return -1;
}

static INT32 _AdspDSPCommonDramWrite(INT32 i4Argc, const CHAR ** szArgv)
    {
    UINT8 u1DspId;
    UINT32 u4SrcAddr;
    UINT32 u4Value;

    if (i4Argc != 4)
    {
        goto lbUsage;
    }
    u1DspId = StrToInt(szArgv[1]);
    u4SrcAddr = StrToInt(szArgv[2]);
    u4Value= StrToInt(szArgv[3]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        goto lbUsage;
    }
    Dsp_WriteDspCommDram(u1DspId, u4SrcAddr, u4Value);
    return 1;

    lbUsage:
    Printf("Usage: cmw [dsp id][addr] [value]\n");
    Printf("[dsp id]    0:DSP0  1:DSP1 \n");

    return -1;
}

static INT32 _AdspDSPNormDram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId;
    UINT8 u1Page;
    UINT32 u4SrcAddr;
    UINT32 u4Len;

    if (i4Argc != 5)
    {
        goto lbUsage;
    }

    u1DspId = StrToInt(szArgv[1]);
    u1Page = StrToInt(szArgv[2]);
    u4SrcAddr = StrToInt(szArgv[3]);
    u4Len = StrToInt(szArgv[4]);

    if ((u4Len == 0)||(u1DspId >= AUD_DSP_NUM))
    {
        goto lbUsage;
    }
    DSP_NormDramDump(u1DspId, u1Page, u4SrcAddr, u4Len);

    return 1;

    lbUsage:
    Printf("Usage: norm [dsp id][page] [addr] [len]\n");
    return -1;
}

static INT32 _AdspDSPCmptDram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId;
    UINT8 u1Page;
    UINT32 u4SrcAddr;
    UINT32 u4Len;

    if (i4Argc != 5)
    {
        goto lbUsage;
    }
    u1DspId = StrToInt(szArgv[1]);
    u1Page = StrToInt(szArgv[2]);
    u4SrcAddr = StrToInt(szArgv[3]);
    u4Len = StrToInt(szArgv[4]);

    if ((u4Len == 0)||(u1DspId >= AUD_DSP_NUM))
    {
        goto lbUsage;
    }
    DSP_CmptDramDump(u1DspId, u1Page, u4SrcAddr, u4Len);

    return 1;

    lbUsage:
    Printf("Usage: cmpt [dsp id][page] [addr] [len]\n");
    return -1;
}

static INT32 _AdspCmdPtr(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId;
    UINT32 u4RpDec1, u4WpDec1;
    UINT32 u4RpDec2, u4WpDec2;
    UINT32 u4DataSZDec1, u4DataSZDec2;
#ifdef CC_MT5391_AUD_3_DECODER
    UINT32 u4RpDec3, u4WpDec3;
    UINT32 u4DataSZDec3;
#endif

    if(i4Argc!=2) {
        Printf("Usage: ptr [dsp id]\n");
        Printf("[dsp id]    0:DSP0  1:DSP1 \n");
        return -1;
    }    

    u1DspId = StrToInt(szArgv[1]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("dsp id range error \n");
        return -1;
    }

    // Get read/write pointer
    AUD_GetRWPtr(u1DspId, AUD_DEC_MAIN, &u4RpDec1, &u4WpDec1, &u4DataSZDec1);
    AUD_GetRWPtr(u1DspId, AUD_DEC_AUX, &u4RpDec2, &u4WpDec2, &u4DataSZDec2);
#ifdef CC_MT5391_AUD_3_DECODER
    if (u1DspId==AUD_DSP0)
        AUD_GetRWPtr(u1DspId, AUD_DEC_THIRD, &u4RpDec3, &u4WpDec3, &u4DataSZDec3);
#endif

    Printf("Decoder 1 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec1,u4WpDec1,u4DataSZDec1);
    Printf("Decoder 2 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec2,u4WpDec2,u4DataSZDec2);
#ifdef CC_MT5391_AUD_3_DECODER
    if (u1DspId==AUD_DSP0)
    Printf("Decoder 3 rp,wp,size = [0x%08x, 0x%08x, %u]\n",u4RpDec3,u4WpDec3,u4DataSZDec3);
#endif
    return 1;
}

static INT32 _AdspReadSram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, u4Count = 1, i = 0;
    UINT32 u4Tmp;
    UINT8 u1DspId;

    if (i4Argc < 3)
    {
        goto lbUsage;
    }

    u1DspId = (UINT32)StrToInt(szArgv[1]);
    u4Addr = (UINT32)StrToInt(szArgv[2]);
    if (i4Argc == 4)
    {
        u4Count= (UINT16)StrToInt(szArgv[3]);
    }
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("dsp id range error \n");
        return -1;
    }

    while (i < u4Count)
    {
        if ((i%4) == 0)
        { 
            u4Tmp = u4ReadDspSram32(u1DspId, u4Addr);
            Printf("\nSRAM (0x%04x) | %08x ", u4Addr, u4Tmp);
        }
        else
        { 
            u4Tmp = u4ReadDspSram32(u1DspId, u4Addr);
            Printf("%08x ", u4Tmp); 
        }
        u4Addr++;
        i++;
    }

    return 1;

lbUsage:
    Printf("Usage: reads [dsp id][addr] [cnt]\n");
    return -1;
}

static INT32 _AdspWriteSram(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Addr;
    UINT32  u4Data;
    UINT8 u1DspId;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    u1DspId = (UINT32)StrToInt(szArgv[1]);
    u4Addr = (UINT32)StrToInt(szArgv[2]);
    u4Data = (UINT32)StrToInt(szArgv[3]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("dsp id range error \n");
        return -1;
    }    

    vWriteDspSram32(u1DspId, u4Addr, u4Data);

    return 1;

lbUsage:
    Printf("Usage: writes [dsp id][addr] [data]\n");
    return -1;
}

static INT32 _AdspUpdateReadPtr(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32  u4Addr = 0;
    UINT8  u1DecId = 0;
    UINT8  u1DspId = 0;

    if (i4Argc < 4)
    {
        goto lbUsage;
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);   
    u4Addr = (UINT32)StrToInt(szArgv[3]);
    u1DecId = (UINT8)StrToInt(szArgv[2]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("dsp id range error \n");
        return -1;
    } 

    fgDspReadPtrSet(u1DspId, u1DecId, u4Addr);

    return 1;

lbUsage:
    Printf("Usage: rpts [dsp id][DecId] [Address]\n");
    Printf("[dsp id]    0:DSP0  1:DSP1 \n");
    return -1;
}

static INT32 _AdspDSPQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId;

    if (i4Argc != 2)
    {
        Printf("Usage: q [dsp id]\n");
        Printf("[dsp id]    0:DSP0  1:DSP1 \n");
        return -1;
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);

    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
        return -1;
    }

    AudShowDspStatus(u1DspId);

    return 1;
}

static INT32 _AdspDolbyQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AudShowDolbyInfo();

    return 1;
}

static INT32 _AdspMTSQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1DecId = 0;
    if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
    }
    AudShowMtsStatus(u1DecId);

    return 1;
}

#if 1///def CC_VORBIS_SUPPORT    ///CC_VORBIS_SUPPORT_Q
static INT32 _AdspVorbisQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1DecId = 0;
    if (i4Argc == 2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
    }
    AudShowVorbisStatus(u1DecId);

    return 1;
}
#endif

static INT32 _AudPreBuffer(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc == 2)
    {
        UINT16 u2Length = 0;
        u2Length = (UINT8)StrToInt(szArgv[1]);
        AUD_DspSetPcmPreBuffer(AUD_DSP0, AUD_DEC_MAIN, u2Length);
        return 1;
    }
    else
    {
        Printf("pcm pre-buffer bank setting \n");
        Printf("Usage: pb [delay bank no.] \n");
        Printf("1 bank is around 5.5 ms \n");
        Printf("current bank no. =  %d \n\n\n",AUD_DspGetPcmPreBuffer(AUD_DSP0, AUD_DEC_MAIN));
        return -1;
     }
}

static INT32 _AudCacheHitRate(INT32 i4Argc, const CHAR ** szArgv) //sunman_5365
{
    UINT8 u1DspId;
    UINT8  u1Cache = 0;
    UNUSED(i4Argc);
    UNUSED(szArgv);


    //u1Cache = 0: All, 1: Main, 2: Second, 3: Third
    if (i4Argc == 3)
    {
         u1DspId = (UINT8)StrToInt(szArgv[1]);
         u1Cache = (UINT8)StrToInt(szArgv[2]);
         Printf("DSP(%d) cache %d hit rate = %d (o/oo)\n", u1DspId, u1Cache, AUD_GetDspCacheHitRate(u1DspId, u1Cache));
    }
    else
    {
        Printf("Usage : aud.dsp.cache [dsp id][cache]\n\t 0: All, 1: Main, 2: Second, 3: Third \n");
    }

    return 1;
}

static INT32 _AdspDSPChlDump(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId;
    UINT8 u1Sel = 0;

    if (i4Argc == 3)
    {
        u1DspId = (UINT8)StrToInt(szArgv[1]);
        u1Sel = (UINT8)StrToInt(szArgv[2]);
    }
    else
    {
        Printf("Usage: c [dspi id][channal #] \n");
        Printf("dsp id: DSP  0, DSP 1 \n");
        Printf("[channal #]           0  L   \n");
        Printf("                      1  R   \n");
        Printf("                      2  Ls   \n");
        Printf("                      3  Rs   \n");
        Printf("                      4  C    \n");
        Printf("                      5  SW  \n");
        Printf("                      6  Bypass L  \n");
        Printf("                      7  Bypass R  \n");
        Printf("                      8  dmx L  \n");
        Printf("                      9  dmx R  \n");
        return -1;
     }

    AUD_ChlOutDump(u1DspId, u1Sel);

    return 1;
}

static INT32 _AdspDSPStcDiffBound(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StcDiffFactorLo = 0;
    UINT8 u1StcDiffFactorHi = 0;
    UINT8 u1StcDiffFactorWs = 0;

    if (i4Argc == 4)
    {
        u1StcDiffFactorLo = (UINT8)StrToInt(szArgv[1]);
        u1StcDiffFactorHi = (UINT8)StrToInt(szArgv[2]);
        u1StcDiffFactorWs = (UINT8)StrToInt(szArgv[3]);

        AUD_AudioSetStcDiffBound(AUD_DEC_MAIN, u1StcDiffFactorLo, u1StcDiffFactorHi, u1StcDiffFactorWs);
        printf("Set STC difference factor (lo,hi,worst) = (%d,%d,%d)\n", u1StcDiffFactorLo, u1StcDiffFactorHi, u1StcDiffFactorWs);
    }
    else
    {
        Printf("Usage: stcbd [factor Lo] [factor Hi] [factor Ws]\n");
        Printf("    factor : 0 ~ 255   \n");
        Printf("The STC difference bound would be x msec * (1+factor)\n");
        return -1;
     }

    return 1;
}


static INT32 _AudCmdUopBassMngEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Enable;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[bass management setting]\n");
            Printf("  DEC1: %d\n", AUD_DspGetBassMngEnable(AUD_DSP0, AUD_DEC_MAIN));
            Printf("  DEC2: %d\n", AUD_DspGetBassMngEnable(AUD_DSP0, AUD_DEC_AUX));
            return 1;
        }
        Printf("Usage: bs [decoder id] [on/off] or bs q\n");
        Printf("[decoder id]     0:FIRST      1:SECOND\n");
        Printf("[on/off]         0:OFF        1:ON \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DspBassMngEnalbe(AUD_DSP0, u1DecId, (BOOL)u1Enable);
    return 1;
}

#ifdef CC_AUD_FIR_SUPPORT
static INT32 _AudCmdUopFIREnable(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: flag [mode] \n");
        Printf("[mode]         0:OFF 1:Mode1 2:Mode2 3:Mode3... \n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetFIREnable(AUD_DEC_MAIN, u1Enable);
    return 1;
}

static INT32 _AudCmdUopGetPhyAddr(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    UINT32 u4DramAdrIdx;
    UINT32 u4VirAddr;
    UINT32 u4PhyAddr;

    u4DramAdrIdx = DSP_CommDramRead(0xAB5) + 0x1;//the address of coneq dram table

    u4VirAddr = DSP_CommDramAddr(u4DramAdrIdx);

    u4PhyAddr = PHYSICAL(u4VirAddr);

    Printf("CONEQ Table dump in address ==> 0x%08x\n", u4PhyAddr);
    return 0;
}

#endif

/******************************************************************************
* Function      : _AudCmdUop*
* Description   : accept UOP from RS232 command line
* Parameter     : None
* Return        : None
*******************************************************************************/
static INT32 _AudCmdUopPL2Init(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2cfg;
    UINT16 u2PL2Mode;
    if (i4Argc < 3)
    {
        Printf("Usage: init [config] [mode]\n");
        Printf("[config:16 bit] value for W_PLIICONFIG\n");
        Printf("[mode:16 bit] value for W_PLIIMODE\n");
        return -1;
    }

    u2PL2cfg =  (UINT16)StrToInt(szArgv[1]);
    u2PL2Mode = (UINT16)StrToInt(szArgv[2]);

    AUD_DspPL2ConfigInit(u2PL2cfg,u2PL2Mode);
    return 1;
};
static INT32 _AudCmdUopPL2Switch(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: switch [mode]\n");
        Printf("[mode] 0:ON 1:OFF 2:AUTO\n");
        return -1;
    }

    u2PL2Mode =  (UINT16)StrToInt(szArgv[1]);

    AUD_DspPL2Config(PL2CTRL_SWITCH,u2PL2Mode);

    return 1;
};
static INT32 _AudCmdUopPL2Mode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2Mode;
    UINT16 u2PL2cfg;
    if (i4Argc < 3)
    {
        Printf("Usage: mode [config] [manual mode]\n");
        Printf("[config]      0: MANUAL            1:AUTO \n");
        Printf("[manual mode] 0: PROLGIC EMULATION 1:VIRTUAL MODE\n");
        Printf("              2: MUSIC MODE        3:MOVIE MODE\n");
        Printf("              4: MATRIX MODE       7:CUSTOM MODE\n");
        return -1;
    }
    u2PL2cfg = (UINT16)StrToInt(szArgv[1]);
    u2PL2Mode =  (UINT16)StrToInt(szArgv[2]);

    u2PL2Mode = (u2PL2Mode <<1) | (u2PL2cfg & 0x1);
    AUD_DspPL2Config(PL2CTRL_MODE,u2PL2Mode);

    return 1;
};

static INT32 _AudCmdUopPL2Pano(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2cfg;
    if (i4Argc < 2)
    {
        Printf("Usage: pano [config]\n");
        Printf("[config] 0: ON  1:OFF \n");
        return -1;
    }
    u2PL2cfg = (UINT16)StrToInt(szArgv[1]);
    AUD_DspPL2Config(PL2CTRL_PANORAMA,u2PL2cfg);

    return 1;
};
static INT32 _AudCmdUopPL2Dim(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2cfg;
    if (i4Argc < 2)
    {
        Printf("Usage: dim [dimension control]\n");
        Printf("[dimension control] 0:-3,1:-2,2:-1,3:0,4:1,5:2,6:3 \n");
        return -1;
    }
    u2PL2cfg = (UINT16)StrToInt(szArgv[1]);
    AUD_DspPL2Config(PL2CTRL_DIMENSION,u2PL2cfg);

    return 1;
};
static INT32 _AudCmdUopPL2CWidth(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2PL2cfg;
    if (i4Argc < 2)
    {
        Printf("Usage: cw [center width spread value]\n");
        Printf("[center width spread value] 0~7 \n");
        return -1;
    }
    u2PL2cfg = (UINT16)StrToInt(szArgv[1]);
    AUD_DspPL2Config(PL2CTRL_C_WIDTH,u2PL2cfg);

    return 1;
};
static INT32 _AudCmdUopSpeakerConfig(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT32 u4SpkCfg;
    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[Speaker Config]\n");
            Printf("  #DEC1: 0x%08x\n", AUD_DspGetSpeakerConfig(AUD_DEC_MAIN));
            Printf("  #DEC2: 0x%08x\n", AUD_DspGetSpeakerConfig(AUD_DEC_AUX));
            Printf("  #DEC3: 0x%08x\n", AUD_DspGetSpeakerConfig(AUD_DEC_THIRD));
        }
        else
        {
            Printf("Usage: spkcfg [decoder id] [speaker config] or spkcfg q\n");
            Printf("[decoder id]     0:FIRST                 1:SECOND \n");
            Printf("                 2:THIRD \n");
            Printf("[speaker config] 32 bits                          \n");
        }
        Printf("bit 0 ~ 2:\n");
        Printf("  b000: 2/0 (LT/RT downmix: prologic compatible)  \n");
        Printf("  b001: 1/0\n");
        Printf("  b010: 2/0 (LO/RO)\n");
        Printf("  b011: 3/0\n");
        Printf("  b100: 2/1\n");
        Printf("  b101: 3/1\n");
        Printf("  b110: 2/2\n");
        Printf("  b111: 3/2\n");
        Printf("bit 5: Subwoofer Channel present(1)/absent(0)\n");
        Printf("bit 6: Auto downmix present(1)/absent(0)\n");
        Printf("bit 12: Center Channel large(1)/small(0)\n");
        Printf("bit 13: Left Channel large(1)/small(0)\n");
        Printf("bit 14: Right Channel large(1)/small(0)\n");
        Printf("bit 15: Left Surround Channel large(1)/small(0)\n");
        Printf("bit 16: Right Surround Channel large(1)/small(0)\n");
        Printf("bit 19: Downmix CH9 Channel large(1)/small(0)\n");
        Printf("bit 20: Downmix CH10 Channel large(1)/small(0)\n");

        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u4SpkCfg = (UINT32)StrToInt(szArgv[2]);

    AUD_DspSpeakerConfig(u4SpkCfg, u2DecId);

    return 1;
}

static INT32 _AudCmdUopSpeakerLargeSmallConfig(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1FrontPair;
    UINT8 u1Center;
    UINT8 u1Surround;
    UINT8 u1CH910;

    if (i4Argc < 6)
    {
        if ((i4Argc == 2) && (szArgv[1][0]=='q'))
        {
            AUD_DspGetSpeakerLargeSmallConfig(AUD_DEC_MAIN, &u1FrontPair, &u1Center, &u1Surround, &u1CH910);
            Printf("[Speaker L/S Setting]\n");
            Printf("  Front:    %s\n", u1FrontPair ? "Large" : "Small");
            Printf("  Center:   %s\n", u1Center ? "Large" : "Small");
            Printf("  Surround: %s\n", u1Surround ? "Large" : "Small");
            Printf("  CH9/10:   %s\n", u1CH910 ? "Large" : "Small");
            return 1;
        }
        Printf("Usage: ls [decoder id] [FrontPair] [Center] [Surround] [CH9/10] or ls q\n");
        Printf("[decoder id] 0:FIRST  1:SECOND \n");
        Printf("[FrontPair]  0:SMALL  1:LARGE \n");
        Printf("[Center]     0:SMALL  1:LARGE \n");
        Printf("[Surround]   0:SMALL  1:LARGE \n");
        Printf("[CH9/10]     0:SMALL  1:LARGE \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1FrontPair = (UINT8)StrToInt(szArgv[2]);
    u1Center = (UINT8)StrToInt(szArgv[3]);
    u1Surround = (UINT8)StrToInt(szArgv[4]);
    u1CH910 = (UINT8)StrToInt(szArgv[5]);

    //CLI_Parser("aud.uop.spkuop.bs 0 1");  // turn on bass management
    AUD_DspSpeakerLargeSmallConfig(u1DecId, u1FrontPair, u1Center, u1Surround,u1CH910);

    return 1;
}

static INT32 _AudCmdUopCutFreq(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT32 u4Freq;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("Cufoff Frequecy is %d\n", AUD_DspSpkSizeQry(AUD_DEC_MAIN));
            return 1;
        }
        Printf("Usage: cf [decoder id] ([cutoff freq]) or cf q\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[cutoff freq]  set frequency in xxx Hz \n");
        Printf("               70/100/200/300  Hz         \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u4Freq = StrToInt(szArgv[2]);
    AUD_DspSpkSizeCfg(u1DecId, u4Freq);
    return 1;
}

#ifdef CC_S_SUBWOFFER_SUPPORT  ///CC_BASS_MANGER_INDEP_LPF
static INT32 _AudCmdUopCutFreqLpf(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT32 u4Freq;
    UINT8 u1Index;
    UINT16 au2CutOffMap[AQ_BM_CUTOFF_NUM] = {50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,300};

    UNUSED(u1Index);
    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("Cufoff Frequecy of Lpf for LFE channel is %d\n", AUD_DspSpkSizeLpfQry(AUD_DEC_MAIN));
            return 1;
        }
        Printf("Usage: cf of Lpf for LFE channel [decoder id] ([cutoff freq]) or cf q\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[cutoff freq]  cut off frequency index \n");
        for (u1Index=0; u1Index<AQ_BM_CUTOFF_NUM; u1Index++)
        {
            Printf("%16d: %d Hz\n", u1Index, au2CutOffMap[u1Index]);
        }       
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u4Freq = au2CutOffMap[StrToInt(szArgv[2])];
    AUD_DspSpkSizeLpfCfg(u1DecId, u4Freq);
    return 1;
}
#endif  ///CC_BASS_MANGER_INDEP_LPF

static INT32 _AudCmdUopSpeakerOutputConfig(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[SPKCFG CH Setting]\n");
            Printf("  DEC1: %d\n", (AUD_DspGetSpeakerOutputConfig(AUD_DEC_MAIN) & 0x07));
            Printf("  DEC2: %d\n", (AUD_DspGetSpeakerOutputConfig(AUD_DEC_AUX) & 0x07));
            Printf("  DEC3: %d\n", (AUD_DspGetSpeakerOutputConfig(AUD_DEC_THIRD) & 0x07));
            return 1;
        }
        Printf("Usage: ch [decoder id] [mode] or ch q\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("               2:THIRD \n");
        Printf("[mode]         0: 2/0 (LT/RT downmix: prologic compatible)  \n");
        Printf("               1: 1/0\n");
        Printf("               2: 2/0 (LO/RO)\n");
        Printf("               3: 3/0\n");
        Printf("               4: 2/1\n");
        Printf("               5: 3/1\n");
        Printf("               6: 2/2\n");
        Printf("               7: 3/2\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);
    u1Mode = ((AUD_DspGetSpeakerOutputConfig(u1DecId) & 0x40) | u1Mode);

    AUD_DspSpeakerOutputConfig(u1DecId, u1Mode);

    return 1;
}

static INT32 _AudCmdUopSpeakerSubwooferEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0]=='q'))
        {
            Printf("[Subwoofer Setting]\n");
            Printf("  DEC1: %d\n", AUD_DspGetSpeakerSubwooferEnable(AUD_DEC_MAIN));
            Printf("  DEC2: %d\n", AUD_DspGetSpeakerSubwooferEnable(AUD_DEC_AUX));
            Printf("  DEC3: %d\n", AUD_DspGetSpeakerSubwooferEnable(AUD_DEC_THIRD));
            return 1;
        }
        Printf("Usage: sw [decoder id] [mode] or sw q\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("               2:THIRD \n");
        Printf("[mode]         0:OFF    1:ON \n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    AUD_DspSpeakerSubwooferEnable(u1DecId, u1Mode);

    return 1;
}

#if 1 //channel dealy for NorDig
static INT32 _AudCmdUopChannelDelay(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u2ChanNum;
    UINT16 u2Delay;
    UINT16 u2DecId;
    if (i4Argc < 4 || (UINT16)StrToInt(szArgv[3]) > 50)
    {
        if ((i4Argc == 2) && (szArgv[1][0]=='q'))
        {
            AUD_CH_T ch;
            Printf("[Delay Setting] %s\n", AUD_DspGetDelayEnable(AUD_DSP0, AUD_DEC_MAIN) ? "Enable" : "Disable");
            for (ch=AUD_CH_FRONT_LEFT;ch<=AUD_CH_AUX_FRONT_RIGHT;ch++)
            {
                if (ch == AUD_CH_FRONT_LEFT)
                {
                    Printf("  #DEC1\n");
                }
                else if (ch == AUD_CH_AUX_FRONT_LEFT)
                {
                    Printf("  #DEC2\n");
                }
                Printf("    CH%d: %d\n", ch, AUD_DspGetChannelDelay(AUD_DSP0, ch));
            }
            return 1;
        }
        Printf("Usage: delaycfg [decoder id] [channel #] [channel delay] or delaycfg q\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)\n");
        Printf("8(Downmix L), 9(Downmix R), 10(AUX L), 11(AUX R), 13(All), 14(LFE)\n");
        //Printf("[channel delay]  0.05 m/unit\n");
        Printf("[channel delay]  10ms /unit (0~50 unit)\n");
        return -1;
    }

    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2ChanNum = (UINT32)StrToInt(szArgv[2]);
    //u2Delay = (UINT32)StrToInt(szArgv[3]);
    u2Delay = (UINT32)StrToInt(szArgv[3])*68;//sunman

    AUD_DspChannelDelay(AUD_DSP0, u2Delay,(AUD_CH_T)u2ChanNum,u2DecId);
    return 1;
}
#else
static INT32 _AudCmdUopChannelDelay(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u2ChanNum;
    UINT16 u2Delay;
    UINT16 u2DecId;
    if (i4Argc < 4)
    {
        Printf("Usage: delaycfg [decoder id] [channel #] [channel delay]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0(L), 1(R), 2(LS), 3(RS), 4(C), 5(SUB), 6(Bypass L), 7(Bypass R)\n");
        Printf("8(Downmix L), 9(Downmix R), 10(AUD L), 11(AUD R), 13(All), 14(LFE)\n");
        Printf("[channel delay]  0.05 m/unit\n");
        return -1;
    }

    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2ChanNum = (UINT32)StrToInt(szArgv[2]);
    u2Delay = (UINT32)StrToInt(szArgv[3]);

    AUD_DspChannelDelay(u2Delay,(AUD_CH_T)u2ChanNum,u2DecId);
    return 1;
}
#endif

static INT32 _AudCmdUopChVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT32 u4Volume;
    AUD_CH_T eChannel;

    if (i4Argc < 4)
    {
        if ((i4Argc == 2) && (szArgv[1][0]=='q'))
        {
            Printf("[Channel Volume Setting] (%s)\n", AUD_DspGetChannelVolumeEnable(AUD_DSP0, AUD_DEC_MAIN) ? "Enable" : "Disable");
            Printf(" #DEC1\n");
            for (u1DecId = 0;u1DecId<10;u1DecId++)
                Printf("  CH%d: %d(0x%x)\n", u1DecId, AUD_DspGetChannelVolume(AUD_DEC_MAIN,(AUD_CH_T)u1DecId), AUD_DspGetFineTuneVolume(AUD_DEC_MAIN, (AUD_CH_T)u1DecId));
            Printf(" #DEC2\n");
            for (u1DecId = 0;u1DecId<2;u1DecId++)
                Printf("  CH%d: %d(0x%x)\n", u1DecId, AUD_DspGetChannelVolume(AUD_DEC_AUX,(AUD_CH_T)u1DecId), AUD_DspGetFineTuneVolume(AUD_DEC_AUX, (AUD_CH_T)u1DecId));
            return 1;
        }

        Printf("Usage: cv [decoder id] [channel] [volume] or cv q \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0:L, 1:R, 2:LS, 3:RS, 4:C, 5:SUBWOOFER\n");
        Printf("                 6:BYPASS L, 7: BYPASS R, 8: DMX L, 9: DMX R\n");
        Printf("[volume]         0 ~ 100\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    eChannel = (AUD_CH_T)StrToInt(szArgv[2]);
    u4Volume = (UINT8)StrToInt(szArgv[3]);

    AUD_DspChannelVolume(AUD_DSP0,u1DecId, eChannel, u4Volume);
    return 1;
}
#ifdef CC_AUD_LINEOUT_VOL_OFFSET
static INT32 _AudCmdUopLineOutVolOffset(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT8 u1Volume;

    if (i4Argc < 3)
    {
        Printf("Usage: lovo [decoder id] [volumeoffset] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[volume]         0 ~ 100\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Volume = (UINT8)StrToInt(szArgv[2]);

    AUD_DspLineOutVolOffset(u1DecId,u1Volume);
    return 1;
}
#endif
#ifdef CC_AUD_VOLUME_OFFSET
static INT32 _AudCmdUopVolumeOffset(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT8 u1Idx;

    if (i4Argc < 3)
    {
        Printf("Usage: vo [decoder id] [volumeoffset_idx] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[Idx]         0 ~ 12\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Idx = (UINT8)StrToInt(szArgv[2]);

    AUD_DspVolumeOffset(u1DecId,u1Idx);
    return 1;

}
#ifdef CC_S_SUBWOFFER_SUPPORT
static INT32 _AudCmdUopSWVolumeOffset(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT8 u1Idx;

    if (i4Argc < 3)
    {
        Printf("Usage: swvo [decoder id] [volumeoffset_idx] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[Idx]         0 ~ 12\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Idx = (UINT8)StrToInt(szArgv[2]);

    AUD_DspSWVolumeOffset(u1DecId,u1Idx);
    return 1;

}
#endif
#endif
static INT32 _AudCmdUopChVolumeGain(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    INT32  i4Gain;
    AUD_CH_T eChannel;

    if (i4Argc < 4)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[Extra Channel Volume Gain Setting]\n");
            Printf(" #DEC1  \t[line in]/[non line in]\n");
            for (u1DecId = 0;u1DecId<10;u1DecId++)
                Printf(" CH%d:\t\t%d/%d\n", u1DecId, AUD_DspGetChannelVolGain(AUD_DEC_MAIN,(AUD_CH_T)u1DecId),\
                                                      AUD_DspGetChannelVolGainNonLineIn(AUD_DEC_MAIN,(AUD_CH_T)u1DecId));
            Printf(" #DEC2  \t[line in]/[non line in]\n");
            for (u1DecId = 0;u1DecId<2;u1DecId++)
                Printf(" CH%d:\t\t%d/%d\n", u1DecId, AUD_DspGetChannelVolGain(AUD_DEC_AUX,(AUD_CH_T)u1DecId),\
                                                      AUD_DspGetChannelVolGainNonLineIn(AUD_DEC_MAIN,(AUD_CH_T)u1DecId));
            return 1;
        }

        Printf("Usage: cvg [decoder id] [channel] [gain] or cvg q\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0:L, 1:R, 2:LS, 3:RS, 4:C, 5:SUBWOOFER,\n");
        Printf("                 6:Bypass L, 7: Bypass R, 8: Downmix L, 9: Dowmix R\n");
        Printf("[gain]           -128 ~ 96,  every 0.5dB per step \n");
        printf("                 -128:  -64   dB\n");
        printf("                 -127:  -63.5 dB\n");
        printf("                 -126:  -63   dB\n");
        printf("                     .        . \n");
        printf("                     .        . \n");
        printf("                     .        . \n");
        printf("                    0:    0   dB\n");
        printf("                    1:    0.5 dB\n");
        printf("                     .        . \n");
        printf("                     .        . \n");
        printf("                   96:   48   dB\n");

        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    eChannel = (AUD_CH_T)StrToInt(szArgv[2]);
    i4Gain = (INT32)i4StrToInt(szArgv[3]);

    AUD_DspChannelVolGain(u1DecId, eChannel, i4Gain);
    return 1;
}

#ifdef CC_AUD_LINEOUT_VOL_MODE
static INT32 _AudCmdUopLinOutVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("Usage: lov [decoder id] [mode] [level] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[mode]   0:fixed      1:variable \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    AUD_DspSetLineOutVolumeMode(u1DecId, (UINT8)u1Mode);
    return 1;

}
#endif

static INT32 _AudCmdUopSrcVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    INT16 i2Volume;
    AUD_DEC_STREAM_FROM_T eStrmSrc;

    if (i4Argc < 4)
    {
        Printf("Usage: sv [decoder id] [source] [value] \n");
        Printf("[decoder id] 0:FIRST 1:SECOND 2:THIRD 3: 4TH\n");
        Printf("[source #]      0:OTHERS, 1:DIGITAL, 2:ANALOG, 3:SPDIF \n");
        printf("                4:LINE_IN, 5:HDMI, 6:MEMORY 7: BUFAGT 8: FEEDER 9: MM\n");
        Printf("[volume]        -128~36     every 0.5dB per step \n");
        printf("                  -128:  -64    dB\n");
        printf("                  -127:  -63.5 dB\n");
        printf("                  -126:  -63    dB\n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                     0:    0     dB\n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                     .       . \n");
        printf("                    36:   18   dB\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    eStrmSrc = (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[2]);
    i2Volume = (INT16)i4StrToInt(szArgv[3]);

    AUD_DspSetSrcVol(u1DecId, eStrmSrc, i2Volume);
    return 1;
}

#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
static INT32 _AudCmdUopSrcLinOutVolume(INT32 i4Argc,const CHAR** szArgv)
    {
        UINT16 u1DecId;
        UINT32 u4Volume;
        AUD_DEC_STREAM_FROM_T eStrmSrc;

        if (i4Argc < 4)
        {
            Printf("Usage: slov [decoder id] [source] [value] \n");
#ifdef CC_MT5391_AUD_3_DECODER
            Printf("[decoder id]    0:FIRST     1:SECOND \n");
#else
            Printf("[decoder id]    0:FIRST     1:SECOND     2:THIRD \n");
#endif
            Printf("[source #]      0:OTHERS, 1:DIGITAL, 2:ANALOG, 3:SPDIF \n");
            printf("                4:LINE_IN, 5:HDMI, 6:MEMORY \n");
            Printf("[volume]        0~164     every 0.5dB per step \n");
            printf("                     0:  -64    dB\n");
            printf("                     1:  -63.5 dB\n");
            printf("                     2:  -63    dB\n");
            printf("                     .       . \n");
            printf("                     .       . \n");
            printf("                     .       . \n");
            printf("                  128:    0     dB\n");
            printf("                     .       . \n");
            printf("                     .       . \n");
            printf("                     .       . \n");
            printf("                  164:    18   dB\n");
            return -1;
        }

        u1DecId = (UINT8)StrToInt(szArgv[1]);
        eStrmSrc = (AUD_DEC_STREAM_FROM_T)StrToInt(szArgv[2]);
        u4Volume = (UINT8)StrToInt(szArgv[3]);

        AUD_DspSrcLinoutVol(u1DecId, eStrmSrc, (INT16)(u4Volume-128));
        return 1;
    }
#endif

static INT32 _AudCmdUopGetSrcVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT32 u4Volume;
    UINT8 i;

    const CHAR* strStreamSrc[AUD_STREAM_FROM_NUM]= {"Others","DTuner","ATuner","SPDIF","Line_In","HDMI","Memory","Reserve", "Feeder","MultiMedia"};

    u1DecId = 0;

    for (i=0; i<AUD_STREAM_FROM_NUM; i++)
    {
        u4Volume = AUD_DspGetSrcVol(u1DecId, (AUD_DEC_STREAM_FROM_T)i);
        Printf("%s : %d\n", strStreamSrc[i] ,u4Volume);
    }

     return 1;
}

#ifdef CC_AUD_DRC_V20
//#if 1
typedef struct {
    UINT32 u4Flag;
    char cName[24];
    UINT32 u4InstructionIndex;
} DRC_COMMAND_T;

static char cDrcInstruction[10][48] = {
"0=Disable, 1=Enable", // 0
"0x1000~0x7fffff (-48~18 dB)", // 1
"0x0~0x7fffff (infinite:1~1:1)", // 2
"0x1~0xC000 (0.0039~192 dB/ms)", // 3
"0x0~0x600 (0~6 dB)", // 4
"0x1~0x1000 (-120~-48 dB)", // 5
"0x0~0xff (0~255/256 dB), 0x1=1/256dB", // 6
"0x1e~0x47 (390~20000 Hz)", // 7
"0x1~0xC000 (0.00098~48 dB/ms)", // 8
"0x0~0x100 (0~1364 ms), 0x1=5.33ms", // 9
};

static DRC_COMMAND_T cDrcCommand[DRC2_NUM] = {
{1, "Control Flag         ", 0},
{1, "Band 0 Target Level  ", 1},
{1, "Band 0 Attack rate   ", 3},
{1, "Band 0 Release rate  ", 8},
{1, "Band 0 Ratio         ", 2},
{0, "Band 0 Silence level ", 5},
{0, "Band 0 Max Expand    ", 4},
{1, "Band 0 Hysteresis    ", 6},
{1, "Band 0 Hold          ", 9},
{1, "Band 1 Target level  ", 1},
{1, "Band 1 Attack rate   ", 3},
{1, "Band 1 Release rate  ", 8},
{1, "Band 1 Ratio         ", 2},
{0, "Band 1 Silence level ", 5},
{0, "Band 1 Max Expand    ", 4},
{1, "Band 1 Hysteresis    ", 6},
{1, "Band 1 Hold          ", 9},
{1, "Limiter Post DRC Mode", 0},
};

#if 0
static void vLog4FcTable ()
{
#if 0
    int j, k;
    
    Printf ("\n   ");
    for (j = 30, k = 0; j < 72; j ++, k ++)
    {
        Printf ("0x%2x: %5d  ", j, _gu4AudFcTbl[j]);
        if ((j % 6) == 5)
        {
            Printf ("\n");
            Printf ("   ");
        }
    }
    if ((j % 6) != 0)
    {
        Printf ("\n");
    }

#else
    int j;

    for (j = 0; j < 72; j ++)
    {
        Printf ("0x%2x: %5d  ", j, _gu4AudFcTbl[j]);
        if ((j % 6) == 5)
        {
            Printf ("\n");
        }
    }

#endif
    
}
#endif

static INT32 _AudCmdUopDRC(INT32 i4Argc,const CHAR** szArgv)
{
    AUD_DRC_V20_INFO_T eType;
    UINT32 u4Value;
    UINT32 u4CurrVal;
    int i;

    if (i4Argc != 3)
    {
        goto _DRC_CLI_SHOW_ERR_MSG;        
    }
    eType = (AUD_DRC_V20_INFO_T) StrToInt(szArgv[1]);
    u4Value = (UINT32) StrToInt(szArgv[2]);

    if (eType < DRC2_NUM)
    {
        AUD_DspSetDrc(eType, u4Value, AUD_DEC_MAIN);
    }
    else
    {
        goto _DRC_CLI_SHOW_ERR_MSG;
    }
    return 1;

_DRC_CLI_SHOW_ERR_MSG:
    Printf ("\nUsage: drc [id] [value] \n\n");
    Printf ("[id] [item]                [current] [value]\n\n");
    for (i = 0; i < DRC2_NUM; i ++)
    {
        // function valid check
        if (cDrcCommand[i].u4Flag == 0)
        {
            continue;
        }
    
        if ((i == DRC2_TARGET_LEVEL) || (i == DRC2_TARGET_LEVEL_1) || (i == DRC2_LIMITER_POST_DRC_MODE))
        {
            Printf ("\n");
        }
        
        Printf (" %2.d: %s", i, &cDrcCommand[i].cName[0]);
        AUD_DspGetDrc((AUD_DRC_V20_INFO_T) i, &u4CurrVal, AUD_DEC_MAIN);
        Printf (" 0x%06.x  ", u4CurrVal);
        
        Printf ("%s\n", cDrcInstruction[cDrcCommand[i].u4InstructionIndex]);

         // addtional display for Fc
     //   if (i == DRC2_FC)
        {        
      //      vLog4FcTable ();
        }
    }  
    Printf ("\n");
    return -1;
}

#ifdef CC_AUD_AVC_V20
static DRC_COMMAND_T cAvcCommand[DRC2_NUM] = {
{1, "Control Flag         ", 0},
//{1, "Fc Index             ", 7},  
{1, "Band 0 Target Level  ", 1},
{1, "Band 0 Attack rate   ", 3},
{1, "Band 0 Release rate  ", 8},
{1, "Band 0 Ratio         ", 2},
{1, "Band 0 Silence level ", 5},
{1, "Band 0 Max Expand    ", 4},
{1, "Band 0 Post gain     ", 6},
{1, "Band 0 Hold          ", 9},
{1, "Band 1 Target level  ", 1},
{1, "Band 1 Attack rate   ", 3},
{1, "Band 1 Release rate  ", 8},
{1, "Band 1 Ratio         ", 2},
{1, "Band 1 Silence level ", 5},
{1, "Band 1 Max Expand    ", 4},
{1, "Band 1 Post gain     ", 6},
{1, "Band 1 Hold          ", 9},
};

static INT32 _AudCmdUopAVC(INT32 i4Argc,const CHAR** szArgv)
{
    AUD_DRC_V20_INFO_T eType;
    UINT32 u4Value;
    UINT32 u4CurrVal;
    int i;

    if (i4Argc != 3)
    {
        goto _AVC_CLI_SHOW_ERR_MSG;        
    }
    eType = (AUD_DRC_V20_INFO_T) StrToInt(szArgv[1]);
    u4Value = (UINT32) StrToInt(szArgv[2]);

    if (eType < DRC2_NUM)
    {
        AUD_DspChangeAVCPara (eType, u4Value, AUD_DEC_MAIN);
    }
    else
    {
        goto _AVC_CLI_SHOW_ERR_MSG;
    }
    return 1;

_AVC_CLI_SHOW_ERR_MSG:
    Printf ("\nUsage: avc [id] [value] \n\n");
    Printf ("[id] [item]                [current] [value]\n\n");
    for (i = 0; i < DRC2_NUM; i ++)
    {
        // function valid check
        if (cAvcCommand[i].u4Flag == 0)
        {
            continue;
        }
    
        if ((i == DRC2_TARGET_LEVEL) || (i == DRC2_TARGET_LEVEL_1))
        {
            Printf ("\n");
        }
        
        Printf (" %2.d: %s", i, &cAvcCommand[i].cName[0]);
        AUD_DspGetAVCPara((AUD_DRC_V20_INFO_T) i, &u4CurrVal, AUD_DEC_MAIN); 
        Printf (" 0x%06.x  ", u4CurrVal);
        
        Printf ("%s\n", cDrcInstruction[cAvcCommand[i].u4InstructionIndex]);

        // addtional display for Fc
   //     if (i == DRC2_FC)
        {        
      //      vLog4FcTable ();
        }
    }  
    Printf ("\n");
    return -1;
}
#endif // CC_AUD_AVC_V20

#endif // CC_AUD_DRC_V20

#ifndef CC_AUD_AVC_V20
static INT32 _AudCmdUopAVC(INT32 i4Argc,const CHAR** szArgv)
{
#if 0
    UINT16 u2AVCflag;
    if (i4Argc != 2)
    {
        Printf("Usage: avc [en] \n");
        Printf("[en] en/dis AVC, 1 to enable AVC, 0 to disable AVC \n");
        return -1;
    }
    u2AVCflag = (UINT8)StrToInt(szArgv[1]);
    if(u2AVCflag==1)
        AUD_DspAVCEnable(0,TRUE);
    else if(u2AVCflag==0)
        AUD_DspAVCEnable(0,FALSE);
#else
    UINT16 u2Type;
    UINT16 u2Value;
    const CHAR *Argv;
    UINT16 u2CurrVal[8];
    UINT8 i = 0;

    for (i=0; i<8; i++)
    {
        AUD_DspGetAVCPara(i,&u2CurrVal[i],0);
    }
    if (u2CurrVal[0]==3)
    {
        u2CurrVal[0] = 1;
    }

    if (i4Argc != 3)
    {
_AVC_CLI_SHOW_ERR_MSG:
        Printf("Usage: avc [type] [val] \n");
        Printf("[type] 0, val: 1: Enable, 0: Disable ");
        Printf(" [Current : 0x%x]\n", u2CurrVal[0]);
        Printf("[type] 1, val: Target Level ");
        Printf(" [Current : 0x%x (%d)]\n", u2CurrVal[1], (INT16)u2CurrVal[1]);
        //Printf("[type] 2, val: Silence Level\n");
        //Printf(" [Current : 0x%x]\n", u2CurrVal[2]);
        Printf("[type] 3, val: Max. Gain Up ");
        Printf(" [Current : 0x%x]\n", u2CurrVal[3]);
        //Printf("[type] 4, val: Max. Gain Down ");
        //Printf(" [Current : 0x%x (%d)]\n", u2CurrVal[4], (INT16)u2CurrVal[4]);
        Printf("[type] 5, val: Adjust Rate ");
        Printf(" [Current : 0x%x]\n", u2CurrVal[5]);
        Printf("[type] 6, val: UI Adjust Rate ");
        Printf(" [Current : 0x%x]\n", u2CurrVal[6]);
        //Printf("[type] 7, val: Attack Threshold ");
        //Printf(" [Current : 0x%x]\n", u2CurrVal[7]);
        return -1;
    }
    u2Type = (UINT16)StrToInt(szArgv[1]);
    Argv = szArgv[2];
    if (Argv[0] == '-')
    {
        u2Value = (UINT16)StrToInt(&Argv[1]);
        u2Value = (UINT16)0xFFFF - (u2Value-1);
    }
    else
    {
        u2Value = (UINT16)StrToInt(szArgv[2]);
    }
    switch(u2Type)
    {
    case 0:     //En/Dis
    case 1:     //Target Lvl
    case 2:     //Silence Lvl
    case 3:     //Max Gain Up
    case 4:     //Max Gain Down
    case 5:     //Adj Rate
    case 6:     //UI Adj Rate
    case 7:     //Attact threshold
        AUD_DspChangeAVCPara(u2Type, (INT16)u2Value, FALSE);
        break;
    default:
        goto _AVC_CLI_SHOW_ERR_MSG;
    }
#endif
    return 1;
}
#endif  // CC_AUD_AVC_V20

#ifdef KARAOKE_MIC_LINE_IN2
static INT32 _AudCmdUopKaraokeMicIn(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1kEnable;

    if (i4Argc == 2)
    {
        u1kEnable = (UINT8)StrToInt(szArgv[1]);
    }
    else
    {
        Printf("Usage: Karaoke [en] \n");
        Printf("[en]   0:Disable  1:Enable \n");
        return -1;
    }

    AUD_MicInput_Enable((BOOL)u1kEnable);
    return 1;
}
#endif

static INT32 _AudCmdUopMonoDownmix(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1kmode;

    if (i4Argc == 2)
    {
        u1DecId = AUD_DEC_MAIN;
        u1kmode = (UINT8)StrToInt(szArgv[1]);
    }
    else if (i4Argc == 3)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        u1kmode = (UINT8)StrToInt(szArgv[2]);
    }
    else
    {
        Printf("Usage: md {[decoder id]} [en] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[en]             0:Disable  1:Enable \n");
        return -1;
    }

    AUD_DspMonoDownMix(u1DecId, (BOOL)u1kmode);
    return 1;
}

static INT32 _AudCmdUopKaraokeMixRatio(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT32 u4kratio;

    if (i4Argc == 2)
    {
        u1DecId = AUD_DEC_MAIN;
        u4kratio = (UINT8)StrToInt(szArgv[1]);
    }
    else if (i4Argc == 3)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        u4kratio = (UINT32)StrToInt(szArgv[2]);
    }
    else
    {
        Printf("Usage: kr {[decoder id]} [ratio] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[ratio]             0x0 ~ 0x800000 (for R channel)\n");
        return -1;
    }

    AUD_DspSetKaraokeMixRatio(u1DecId, u4kratio);
    return 1;
}

// 2006/08/28, created for tuning volume table
static INT32 _AudCmdUopFineVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Volume;
    UINT8 u1DecId;

    if (i4Argc == 2)
    {
        if (szArgv[1][0] == 'q')
        {
            Printf("[Fine Volume Setting]\n");
            Printf("  DEC1: 0x%08x\n", AUD_DspGetFineTuneVolume(AUD_DEC_MAIN,AUD_CH_ALL));
            Printf("  DEC2: 0x%08x\n", AUD_DspGetFineTuneVolume(AUD_DEC_AUX,AUD_CH_ALL));
            Printf("  DEC3: 0x%08x\n", AUD_DspGetFineTuneVolume(AUD_DEC_THIRD,AUD_CH_ALL));
            Printf("  DEC4: 0x%08x\n", AUD_DspGetFineTuneVolume(AUD_DEC_4TH, AUD_CH_ALL));
            return 1;
        }
        u1DecId = AUD_DEC_MAIN;
        u4Volume = StrToInt(szArgv[1]);
    }
    else if (i4Argc == 3)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        u4Volume = StrToInt(szArgv[2]);
    }
    else
    {
        Printf("Usage: fv {[decoder id]} [volume] or fv q\n");
        Printf("[decoder id]     0:FIRST 1:SECOND 2:THIRD 3: 4TH\n");
        Printf("[volume]         0x0 ~ 0x20000\n");
        Printf("                 0x0: mute, 0x20000: full scale (0 dB)\n");
        return -1;
    }
    // Check if it is valid volume value
    if(u4Volume > 0x20000)
    {
        Printf("Warning: volume[0x%08x] exceeds full scale 0 dB [0x20000]!\n",u4Volume);
    }

    AUD_DspFineTuneVolume(u1DecId, AUD_CH_ALL, u4Volume);

    return 1;
}

static INT32 _AudCmdUopFineChannelVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Volume;
    UINT8 u1DecId;
    AUD_CH_T eChannel;

    if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
    {
        Printf("[Fine channel Volume Setting]\n");
        Printf(" #DEC1\n");
        for (u1DecId = 0;u1DecId<10;u1DecId++)
            Printf("  CH%d: 0x%x\n", u1DecId, AUD_DspGetFineTuneVolume(AUD_DEC_MAIN, (AUD_CH_T)u1DecId));
        Printf(" #DEC2\n");
        for (u1DecId = 0;u1DecId<2;u1DecId++)
            Printf("  CH%d: 0x%x\n", u1DecId, AUD_DspGetFineTuneVolume(AUD_DEC_AUX, (AUD_CH_T)u1DecId));
        return 1;
    }
    else if (i4Argc == 4)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        eChannel = (AUD_CH_T)StrToInt(szArgv[2]);
        u4Volume = StrToInt(szArgv[3]);
    }
    else
    {
        Printf("Usage: fcv [decoder id] [channel] [volume] or fcv q\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[channel #]      0:L, 1:R, 2:LS, 3:RS, 4:C, 5:SUBWOOFER\n");
        Printf("                 6:BYPASS L, 7: BYPASS R, 8: DMX L, 9: DMX R\n");
        Printf("[volume]         0x0 ~ 0x1fb2a7\n");
        Printf("                 0x0: mute, 0x1fb2a7: 24dB\n");
        return -1;
    }
    // Check if it is valid volume value
    if(u4Volume > 0x1fb2a7)
    {
        Printf("Warning: volume[0x%08x] exceeds 24dB [0x1fb2a7]!\n",u4Volume);
    }

    AUD_DspFineTuneVolume(u1DecId, eChannel, u4Volume);

    return 1;
}

static INT32 _AudCmdUopVolume(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u1DecId;
    UINT32 u4Volume;

    if (i4Argc == 2)
    {
        if (szArgv[1][0] == 'q')
        {
            Printf("[Volume Setting]\n");
            Printf("  DEC1: %d\n", AUD_DspGetMasterVolume(AUD_DEC_MAIN));
            Printf("  DEC2: %d\n", AUD_DspGetMasterVolume(AUD_DEC_AUX));
            Printf("  DEC3: %d\n", AUD_DspGetMasterVolume(AUD_DEC_THIRD));
            Printf("  DEC4: %d\n", AUD_DspGetMasterVolume(AUD_DEC_4TH));
            return 1;
        }
        u1DecId = AUD_DEC_MAIN;
        u4Volume = (UINT8)StrToInt(szArgv[1]);
    }
    else if (i4Argc == 3)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        u4Volume = (UINT8)StrToInt(szArgv[2]);
    }
    else
    {
        Printf("Usage: v {[decoder id]} [volume] or v q\n");
#if defined(CC_AUD_4_DECODER_SUPPORT)
        Printf("[decoder id]     0:FIRST 1:SECOND 2:THIRD 3: 4TH\n");
#elif defined(CC_MT5391_AUD_3_DECODER)
        Printf("[decoder id]     0:FIRST 1:SECOND 2:THIRD\n");
#else
        Printf("[decoder id]     0:FIRST 1:SECOND\n");
#endif
        Printf("[volume]         0 ~ 100\n");
        return -1;
    }

    AUD_DspMasterVolume(u1DecId, u4Volume);
    return 1;
}

#if 0
static UINT32 _StrToVol(const CHAR* pszStr)
{
    //Andrew wen 07/8/20 : Dynamic volume curve adjusetment.
    UINT8 u1Idx;
    UINT8 u1Len;
    UINT8 u1LenLeft;
    UINT32 u4ChkPnt = 0;
    UINT32 u4TmpVal = 0;

    if (pszStr == NULL)
    {
        return 0;
    }

    u1Len = x_strlen(pszStr);

    for (u1Idx=0; u1Idx<u1Len; u1Idx++)
    {
        if (pszStr[u1Idx] == '.')
            break;
    }

    if (u1Idx>0)
    {
        u4ChkPnt = (StrToInt(pszStr) * 10000);   //precision is 10^-4
    }

    u1LenLeft = u1Len - (u1Idx+1);
    u4TmpVal = StrToInt(&pszStr[u1Idx+1]);
    for (u1Idx=0; u1Idx<(4-u1LenLeft); u1Idx++)
    {
        u4TmpVal *= 10;
    }
    u4ChkPnt += u4TmpVal;

    //Printf("u4ChkPnt = %d\n", u4ChkPnt);
    return u4ChkPnt;
}
#endif //if 0
static INT32 _AudCmdUopVolumeCurve(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;
    UINT32 u4VolTabChkPnt[VOL_TABLE_CHK_PNT];
    //UINT64 u4TmpVar = 0;

    if ( i4Argc != (VOL_TABLE_CHK_PNT+1) )
    {
        Printf("Usage:  vc  [Step 0] ~ [Step 6] \n");
        Printf("[Step n]     0xXXXXX (e.x. 0x20000) \n");
        return -1;
    }
    #if 0
    for (u1Idx=0; u1Idx < VOL_TABLE_CHK_PNT; u1Idx++)
    {
        u4TmpVar = (UINT64)(_StrToVol(szArgv[u1Idx+1]))<<17; //0x20000 = 2^17
        if (u4TmpVar==0)
        {
            u4VolTabChkPnt[u1Idx] = 0;
        }
        else
        {   //normalize u4VolTabChkPnt[i]  and  times 0x20000
            u4VolTabChkPnt[u1Idx] = (UINT32)(u4TmpVar / _StrToVol(szArgv[VOL_TABLE_CHK_PNT]));
        }
        Printf("Vole[%d] = 0x%x\n", u1Idx, u4VolTabChkPnt[u1Idx]);
    }
    #endif
    //check if volume curve if increasing
    for (u1Idx=0; u1Idx < VOL_TABLE_CHK_PNT; u1Idx++)
    {
        u4VolTabChkPnt[u1Idx] = (UINT32)StrToInt(szArgv[u1Idx+1]);
        if (u1Idx > 0)
        {
            if (u4VolTabChkPnt[u1Idx] < u4VolTabChkPnt[u1Idx-1])
            {
                Printf("FAIL!! Vol[%d] = 0x%x is less than Vol[%d] = 0x%x\n, Must be greater than or euqal to!!",
                u1Idx, u4VolTabChkPnt[u1Idx], u1Idx-1, u4VolTabChkPnt[u1Idx-1] );
                return -1;
            }
        }
        Printf("Vol[%d] = 0x%x\n", u1Idx, u4VolTabChkPnt[u1Idx]);
    }

    AUD_DspSetVolumeTable(u4VolTabChkPnt);
    return 1;
}

static INT32 _AudCmdUopQueryVolumeCurve(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;

    if ( i4Argc != 1)
    {
        Printf("Usage:  vcq \n");
        return -1;
    }

    for (u1Idx=0; u1Idx < 101; u1Idx++) //FIXME !! should use definition.
    {
        if ((u1Idx % 5)==0)
        {
            Printf("\n");
        }
        Printf("0x%x ", AUD_DspGetVolumeTable(u1Idx) );
    }
    return 1;
}

#ifdef CC_AUD_NEW_HP_VOL_TABLE
static INT32 _AudCmdUopSetHpVolTable(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;
    UINT32 u4Volume;

    if ( i4Argc < 3)
    {
        Printf("Usage: shvt [set] [volume] \n");
        Printf("[set]         0 ~ 100 \n");
        Printf("[volume] 0x0 ~ 0x20000 \n");
        return -1;

    } 

    u1Idx = (UINT8)StrToInt(szArgv[1]);
    u4Volume = (UINT32)StrToInt(szArgv[2]);

    AUD_DspSetHpVolumeTable(u1Idx, u4Volume);
    return 1;
}

static INT32 _AudCmdUopQueryHpVolumeCurve(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;

    if ( i4Argc != 1)
    {
        Printf("Usage:  hpvcq \n");
        return -1;
    }

    for (u1Idx=0; u1Idx < 101; u1Idx++) //FIXME !! should use definition.
    {
        if ((u1Idx % 5)==0)
        {
            Printf("\n");
        }
        Printf("0x%x ", AUD_DspGetHpVolumeTable(u1Idx) );
    }
    return 1;
}
#endif

#ifdef CC_SET_VOLUME
static INT32 _AudCmdUopSetVolTable(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;
    UINT32 u4Volume;

    if ( i4Argc < 3)
    {
        Printf("Usage: svt [set] [volume] \n");
        Printf("[set]         0 ~ 100 \n");
        Printf("[volume] 0x0 ~ 0x20000 \n");
        return -1;

    }

        u1Idx = (UINT8)StrToInt(szArgv[1]);
        u4Volume = (UINT32)StrToInt(szArgv[2]);

   AUD_DspSetVolTable(u1Idx, u4Volume);
   return 1;
}
#endif

#ifdef CC_AUD_NEW_CV_TABLE
static INT32 _AudCmdUopSetChVolTable(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;
    UINT32 u4Volume;

#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
    if ( i4Argc < 3)
    {
        Printf("Usage: scvt [set] [volume] \n");
        Printf("[set]    0 ~ 100 \n");
        Printf("[volume] 0x0 ~ 0x1fb2a7 \n");
        return -1;

    }

    u1Idx = (UINT8)StrToInt(szArgv[1]);
    u4Volume = (UINT32)StrToInt(szArgv[2]);

   AUD_DspSetCVTable(u1Idx, u4Volume);
   return 1;
#else
       AUD_NVM_VOL_TBL_T rVolTbl;
   
       if ( i4Argc < 4)
       {
           goto u1Usage;
       }
   
       rVolTbl = (AUD_NVM_VOL_TBL_T)StrToInt(szArgv[1]);
       if (rVolTbl >= AUD_NVM_VOL_TBL_NUM)
       {
           goto u1Usage;
       }
   
       u1Idx = (UINT8)StrToInt(szArgv[2]);
       u4Volume = (UINT32)StrToInt(szArgv[3]);
   
       AUD_DspSetCVTable(rVolTbl, u1Idx, u4Volume);
       return 1;
   
   u1Usage:
       Printf("Usage: scvt [VolTbl] [set] [volume] \n");
       Printf("[VolTbl] 0: L    1: R    2: L(Skype)    3: R(Skype)    4. Common\n");
       Printf("[set]    0 ~ 100 \n");
       Printf("[volume] 0x0 ~ 0x1fb2a7 \n");
       return -1;
#endif
}

static INT32 _AudCmdUopQueryChannelVolumeCurve(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;

#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
    if ( i4Argc != 1)
    {
        Printf("Usage:  cvcq \n");
        return -1;
    }

    for (u1Idx=0; u1Idx < 101; u1Idx++) //FIXME !! should use definition.
    {
        if ((u1Idx % 5)==0)
        {
            Printf("\n");
        }
        Printf("0x%x ", AUD_DspGetCVTable(u1Idx) );
    }
    return 1;
#else
    AUD_NVM_VOL_TBL_T rVolTbl;

    if ( i4Argc != 2)
    {
        goto u1Usage;
    }

    rVolTbl = (AUD_NVM_VOL_TBL_T)StrToInt(szArgv[1]);
    if (rVolTbl >= AUD_NVM_VOL_TBL_NUM)
    {
        goto u1Usage;
    }
    
    for (u1Idx=0; u1Idx < 101; u1Idx++) //FIXME !! should use definition.
    {
        if ((u1Idx % 5)==0)
        {
            Printf("\n");
        }
        Printf("0x%x ", AUD_DspGetCVTable(rVolTbl, u1Idx) );
    }

    return 1;
    
u1Usage:
    Printf("Usage:  cvcq [VolTbl]\n");
    Printf("[VolTbl] 0: L    1: R    2: L(Skype)    3: R(Skype)    4. Common\n");
    return -1;
#endif
}

static INT32 _AudCmdUopChannelVolumeCurve(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8  u1Idx;
    UINT32 u4VolTabChkPnt[VOL_TABLE_CHK_PNT];
    //UINT64 u4TmpVar = 0;

#ifndef CC_AUD_NVM_LR_USE_DIFFER_VOL_TBL
    if ( i4Argc != (VOL_TABLE_CHK_PNT+1) )
    {
        Printf("Usage:  cvc  [Step 0] ~ [Step 6] \n");
        Printf("[Step n]     0xXXXXX (e.x. 0x20000) \n");
        return -1;
    }
    #if 0
    for (u1Idx=0; u1Idx < VOL_TABLE_CHK_PNT; u1Idx++)
    {
        u4TmpVar = (UINT64)(_StrToVol(szArgv[u1Idx+1]))<<17; //0x20000 = 2^17
        if (u4TmpVar==0)
        {
            u4VolTabChkPnt[u1Idx] = 0;
        }
        else
        {   //normalize u4VolTabChkPnt[i]  and  times 0x20000
            u4VolTabChkPnt[u1Idx] = (UINT32)(u4TmpVar / _StrToVol(szArgv[VOL_TABLE_CHK_PNT]));
        }
        Printf("Vole[%d] = 0x%x\n", u1Idx, u4VolTabChkPnt[u1Idx]);
    }
    #endif
    //check if volume curve if increasing
    for (u1Idx=0; u1Idx < VOL_TABLE_CHK_PNT; u1Idx++)
    {
        u4VolTabChkPnt[u1Idx] = (UINT32)StrToInt(szArgv[u1Idx+1]);
        if (u1Idx > 0)
        {
            if (u4VolTabChkPnt[u1Idx] < u4VolTabChkPnt[u1Idx-1])
            {
                Printf("FAIL!! Vol[%d] = 0x%x is less than Vol[%d] = 0x%x\n, Must be greater than or euqal to!!",
                u1Idx, u4VolTabChkPnt[u1Idx], u1Idx-1, u4VolTabChkPnt[u1Idx-1] );
                return -1;
            }
        }
        Printf("Vol[%d] = 0x%x\n", u1Idx, u4VolTabChkPnt[u1Idx]);
    }

    AUD_DspSetStepCVTable(u4VolTabChkPnt);
    return 1;
#else
    AUD_NVM_VOL_TBL_T rVolTbl;

    if ( i4Argc != (VOL_TABLE_CHK_PNT+2) )
    {
        goto u1Usage;
    }

    rVolTbl = (AUD_NVM_VOL_TBL_T)StrToInt(szArgv[1]);
    if (rVolTbl >= AUD_NVM_VOL_TBL_NUM)
    {
        goto u1Usage;
    }
    
    //check if volume curve if increasing
    for (u1Idx=0; u1Idx < VOL_TABLE_CHK_PNT; u1Idx++)
    {
        u4VolTabChkPnt[u1Idx] = (UINT32)StrToInt(szArgv[u1Idx+2]);
        if (u1Idx > 0)
        {
            if (u4VolTabChkPnt[u1Idx] < u4VolTabChkPnt[u1Idx-1])
            {
                Printf("FAIL!! Vol[%d] = 0x%x is less than Vol[%d] = 0x%x\n, Must be greater than or euqal to!!",
                u1Idx, u4VolTabChkPnt[u1Idx], u1Idx-1, u4VolTabChkPnt[u1Idx-1] );
                return -1;
            }
        }
        Printf("Vol[%d] = 0x%x\n", u1Idx, u4VolTabChkPnt[u1Idx]);
    }

    AUD_DspSetStepCVTable(rVolTbl, u4VolTabChkPnt);
    return 1;

u1Usage:
    Printf("Usage:  cvc  [VolTbl] [Step 0] ~ [Step 6] \n");
    Printf("[VolTbl] 0: L    1: R    2: L(Skype)    3: R(Skype)    4. Common\n");
    Printf("[Step n]     0xXXXXX (e.x. 0x20000) \n");
    return -1;
#endif
}
#endif
static INT32 _AudCmdUopAutoDmx(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Enable;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[dm Setting]\n");
            if(AUD_GetDspAutoDmx(AUD_DEC_MAIN))
            {
                Printf("  DEC1: On\n");
            }
            else
            {
                Printf("  DEC1: Off\n");
            }
            if(AUD_GetDspAutoDmx(AUD_DEC_AUX))
            {
                Printf("  DEC2: On\n");
            }
            else
            {
                Printf("  DEC2: Off\n");
            }
            if(AUD_GetDspAutoDmx(AUD_DEC_THIRD))
            {
                Printf("  DEC3: On\n");
            }
            else
            {
                Printf("  DEC3: Off\n");
            }

            return 1;
        }
        Printf("Usage: dm [decoder id] [On/Off] or dm q\n");
        Printf("[decoder id] 0:FIRST 1:SECOND \n");
        Printf("             2:THIRD \n");
        Printf("[On/Off] 1/0\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DspAutoDmx(u1DecId, (BOOL)u1Enable);
    return 1;
}
#ifdef CC_AUD_SOUND_MODE
static INT32 _AudCmdUopPostSoundMode(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1ModeIdx;

    if (i4Argc < 3)
    {
        Printf("Usage: psm [decoder id] [mode] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        //Printf("[Mode]         0:case 0     1:case 1      2:case 3... \n");
        Printf("[Mode]         0:Default     1:STANDARD      2:DYNAMIC     3:CLEAR_VOICE... \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1ModeIdx = (UINT8)StrToInt(szArgv[2]);

   //AUD_DspPostSoundModeChange(u1DecId, u1ModeIdx);
   AUD_DspPostSoundMode(u1DecId, u1ModeIdx);
    return 1;
}

#endif
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
static INT32 _AudCmdUopVirtualSurround(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Enable;

    if (i4Argc < 3)
    {
        Printf("Usage: vs [decoder id] [On/Off] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DspVirtualSurroundEnalbe(u1DecId, (BOOL)u1Enable);
    return 1;
}
#else
static INT32 _AudCmdUopVirtualSurround(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1ModeIdx;

    if (i4Argc < 3)
    {
        Printf("Usage: vs [decoder id] [On/Off] \n");
        Printf("[decoder id]     0:FIRST       1:SECOND \n");
        Printf("[Mode]    0:OFF  1:Normal  2:Mono to Stereo  3:Sports  4:Music  5:Cinema  6:Game\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1ModeIdx = (UINT8)StrToInt(szArgv[2]);

    AUD_DspVirtualSurroundEnalbe(u1DecId, u1ModeIdx);
    return 1;
}
#endif
static INT32 _AudCmdUopVSurrCfg(INT32 i4Argc,const CHAR** szArgv)
{
    AUD_VSURR_CFG_TYPE_T eType;
    UINT32 u4Parameter;
    AUD_VSURR_CFG_T rVSurrCfg;

    if ((i4Argc != 2) && (i4Argc != 3))
    {
        Printf("Usage: vscfg q\n");
        Printf("       Display surround parameter \n");
        //Printf("\n");
        Printf("       OR \n");
        //Printf("\n");
        Printf("Usage: vscfg [type] [parameter] \n");
        Printf("       [type]  0:Clarity      1:Width      2:L/R gain  3:Crosstalk  \n");
        Printf("               4:Output Gain  5:Bass Gain  6:Fo \n");
        Printf("       [parameter] \n");
        Printf("               Clarity     : Speech Clarity,       0 ~ 0x500000\n");
        Printf("               Width       : Sound Width,          0 ~ 0x500000\n");
        Printf("               L/R Gain    : L/R Boost Gain,       0 ~ 0x500000\n");
        Printf("               Crosstalk   : Crosstalk Boost Gain, 0 ~ 0x500000\n");
        Printf("               Output Gain : Output Boost Gain,    0 ~ 0x700000\n");
        Printf("               Bass Gain   : Bass Boost Gain,      0 ~ 0x500000\n");
        Printf("               FO          : Bass Cutoff,   0x7f41f2 ~ 0x7f4400\n");
        return -1;
    }

    if (i4Argc == 2)   //query config
    {
        AUD_Surround_Get_Config(&rVSurrCfg);
        Printf("The Settings of Virtual Surround are : \n");
        Printf("Enable      : %d\n", AUD_DspGetVirtualSurroundEnalbe(AUD_DEC_MAIN));
        Printf("Clarity     : %x\n", rVSurrCfg.u4Clarity);
        Printf("Width       : %x\n", rVSurrCfg.u4Width);
        Printf("L/R Gain    : %x\n", rVSurrCfg.u4LRGain);
        Printf("CrossTalk   : %x\n", rVSurrCfg.u4Xtalk);
        Printf("Output Gain : %x\n", rVSurrCfg.u4OutputGain);
        Printf("Bass Gain   : %x\n", rVSurrCfg.u4BassGain);
        Printf("FO          : %x\n", rVSurrCfg.u4Fo);
    }
    else if (i4Argc == 3)  //set config
    {
        eType = (AUD_VSURR_CFG_TYPE_T)StrToInt(szArgv[1]);
        u4Parameter = StrToInt(szArgv[2]);
        AUD_Surround_Set_Config(eType, u4Parameter);
        Printf("Set Type : %d, Value: %x\n", (UINT8)eType, u4Parameter);
    }

    return 0;
}

#ifdef CC_AUD_HPF_SUPPORT
static INT32 _AudCmdUopHpfEnable(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: hpf [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetHPFEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdUopHpfFc(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u2Fc;

    if (i4Argc < 2)
    {
        Printf("Usage: hpf_fc [fc] \n");
        Printf("[fc]         50~300\n");
        return -1;
    }

    u2Fc = (UINT16)StrToInt(szArgv[1]);

    AUD_DspSetHPFFc(u2Fc);
    return 1;
}
static INT32 _AudCmdUopHpf2Fc(INT32 i4Argc,const CHAR** szArgv)
{
    UINT16 u2Fc;

    if (i4Argc < 2)
    {
        Printf("Usage: hpf2_fc [fc] \n");
        Printf("[fc]         50~300\n");
        return -1;
    }

    u2Fc = (UINT16)StrToInt(szArgv[1]);

    AUD_DspSetHPF2Fc(u2Fc);
    return 1;
}

#endif

#ifdef CC_AUD_CLIPPER_SUPPORT
static INT32 _AudCmdUopClipper(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: clip [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetClipperEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdUopClipperVal(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Channel;
    UINT32 u4Val;

    if (i4Argc < 2)
    {
        Printf("Usage: clip [channel] [value] \n");
        Printf("[channel]       channel\n");
        Printf("[value]         val (0~0x7fffff)\n");
        return -1;
    }

    u4Channel = (UINT32)StrToInt(szArgv[1]);
    u4Val = (UINT32)StrToInt(szArgv[2]);

    AUD_DspSetClipperValue((AUD_CH_T)u4Channel, u4Val);
    return 1;
}
#endif

#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
static INT32 _AudCmdUopSPH(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: sph [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSPHEnable((BOOL)u1Enable);
    return 1;
}
#else
static INT32 _AudCmdUopSPH(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: sph [mode] \n");
        Printf("[mode]         0:OFF     1:2DB    2:4DB   3:6DB\n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

   // AUD_DspSetSPHEnable((BOOL)u1Enable);
    AUD_DspSetSPHMode(u1Mode);
    return 1;
}
#endif
#endif

#ifdef CC_AUD_VBASS_SUPPORT
static INT32 _AudCmdUopVirBassCut(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u4CutOff;
    UINT8  u1CurrCutOff;

    u1CurrCutOff = AUD_GetVBassCutOff();
    if (i4Argc < 2)
    {
        Printf("Usage: vbcuut [Cut]\n");
        Printf("[Cut] 0 :50Hz, 1 :80Hz, 2:110Hz, 3:140Hz, 4:170Hz, 5:200Hz \n");
        Printf("Current it's  %d \n",u1CurrCutOff);
        return -1;
    }

    u4CutOff    = (UINT8)StrToInt(szArgv[1]);
    if ((u4CutOff >5))
    {
        Printf("Please input 0~5\n");
        return -1;
    }

    AUD_SetVBassCutOff(u4CutOff);
    AUD_DspSetVirBassEnable(FALSE);
    AUD_DspSetVirBassEnable(TRUE);
    return 0;
}

static INT32 _AudCmdUopVirBass(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: vb [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetVirBassEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdUopVBCfg(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Frequency;
    UINT32 u4Gain1;
    UINT32 u4Gain2;
    BOOL   fgEnable;

    AUD_DspGetVirBassCfg(AUD_DEC_MAIN, &fgEnable, &u4Gain1, &u4Gain2);
    if (i4Argc < 3)
    {
        Printf("Usage: vbcfg [Gain1] [Gain2] \n");
        Printf("[Gain1--output gain(0x1000-->0x3000)] 0x1000 = 0DB\n");
        Printf("[Gain2--bass gain(0x0000-->0x2000)] 0x1000 = 0DB\n");
        Printf("Current setting : enable=%d, gain1=%d, gain2=%d \n",(UINT8)fgEnable, u4Gain1, u4Gain2);
        return -1;
    }

    u4Gain1    = (UINT32)StrToInt(szArgv[1]);
    u4Gain2 = (UINT32)StrToInt(szArgv[2]);
    u4Frequency = 0;//unused

    AUD_DspSetVirBassCfg(AUD_DEC_MAIN, u4Frequency, u4Gain1,u4Gain2);

    return 1;
}
#endif //CC_AUD_VBASS_SUPPORT

#if 1//def CC_AUD_PEQ_SUPPORT
static INT32 _AudCmdUopPEQ(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Enable, i;
    UINT32 u4Frequency, eQValue, u4Gain;

    if (i4Argc < 2)
    {
        Printf("Usage: peq [On/Off/Query PEQ settings] \n");
        Printf("[On/Off]         1/0/q\n");
        return -1;
    }
    if (szArgv[1][0] == 'q')
    {
        Printf("Current setting : PEQ enable = %d \n",AUD_DspGetPEQEnable());
        for (i=0;i<9;i++)
        {
            AUD_DspGetPEQCfg2(AUD_DEC_MAIN, i, &u4Frequency, &eQValue, &u4Gain);
            Printf("Current setting : PEQ(%d), Frequency=%x, Q=%x, Gain=%x \n",i, u4Frequency, eQValue, u4Gain);
        }
#ifdef CC_AUD_PEQ_LR
        for (i=0;i<9;i++)
        {
            AUD_DspGetPEQCfg2_R(AUD_DEC_MAIN, i, &u4Frequency, &eQValue, &u4Gain);
            Printf("R Current setting : PEQ(%d), Frequency=%x, Q=%x, Gain=%x \n",i, u4Frequency, eQValue, u4Gain);
        }
#endif
        Printf("Note that for PEQ Gain, 0dB = 0x200000 \n");
    }
    else
    {
    u1Enable = (UINT8)StrToInt(szArgv[1]);
    AUD_DspSetPEQEnable((BOOL)u1Enable);
    }
    return 1;
}

#if 0
static INT32 _AudCmdUopPEQCfg2(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1PEQSet;
    UINT32 u4Frequency;
    AUD_PEQ_Q_TYPE_T eQValue;
    INT16 i2Gain;
    const CHAR *Argv;

    if (i4Argc < 5)
    {
        Printf("Usage: peq [set] [freq] [Q] [Gain] \n");
        Printf("[set]         0 ~ 8 \n");
        Printf("[u4Frequency] 20 ~ 20K \n");
        Printf("[Q]           eQValue \n");
        Printf("[Gain]        Gain -24 ~ 24 (-12dB ~ 12dB) with 0.5dB/step \n");
        return -1;
    }

    u1PEQSet    = (UINT8)StrToInt(szArgv[1]);
    u4Frequency = (UINT32)StrToInt(szArgv[2]);
    eQValue     = (AUD_PEQ_Q_TYPE_T)StrToInt(szArgv[3]);

    Argv = szArgv[4];
    if (Argv[0] == '-')
    {
        i2Gain = (UINT16)StrToInt(&Argv[1]);
        i2Gain = (UINT16)0xFFFF - (i2Gain-1);
    }
    else
    {
        i2Gain = (UINT16)StrToInt(szArgv[4]);
    }

    AUD_DspSetPEQCfg2(AUD_DEC_MAIN, u1PEQSet, u4Frequency, eQValue, i2Gain);

    return 1;
}
#else
static INT32 _AudCmdUopPEQCfg2(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1PEQSet;
    UINT32 u4Frequency;
    UINT32 eQValue;
    INT16 i2Gain;
    const CHAR *Argv;

    if (i4Argc < 5)
    {
        Printf("Usage: peq [set] [freq] [Q] [Gain] \n");
        Printf("[set]         0 ~ 8 \n");
        Printf("[u4Frequency] 20 ~ 20K \n");
        Printf("[Q]           0 ~ 100, 35 means 3.5, 100 means 10 \n");
#ifdef CC_AUD_SX1_FEATURE
        Printf("[Gain]        Gain -120 ~ 120 (-12dB ~ 12dB) with 0.1dB/step \n");
#else
        Printf("[Gain]        Gain -24 ~ 24 (-12dB ~ 12dB) with 0.5dB/step \n");
#endif
        return -1;
    }

    u1PEQSet    = (UINT8)StrToInt(szArgv[1]);
    u4Frequency = (UINT32)StrToInt(szArgv[2]);
    eQValue     = (UINT32)StrToInt(szArgv[3]);

    Argv = szArgv[4];
    if (Argv[0] == '-')
    {
        i2Gain = (UINT16)StrToInt(&Argv[1]);
        i2Gain = (UINT16)0xFFFF - (i2Gain-1);
    }
    else
    {
        i2Gain = (UINT16)StrToInt(szArgv[4]);
    }


    AUD_DspSetPEQCfg2(AUD_DEC_MAIN, u1PEQSet, u4Frequency, eQValue, i2Gain);

    return 1;
}
#ifdef CC_AUD_PEQ_LR

static INT32 _AudCmdUopPEQCfg2_R(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1PEQSet;
    UINT32 u4Frequency;
    UINT32 eQValue;
    INT16 i2Gain;
    const CHAR *Argv;

    if (i4Argc < 5)
    {
        Printf("Usage: peq [set] [freq] [Q] [Gain] \n");
        Printf("[set]         0 ~ 8 \n");
        Printf("[u4Frequency] 20 ~ 20K \n");
        Printf("[Q]           0 ~ 100, 35 means 3.5, 100 means 10 \n");
#if 1
        Printf("[Gain]        Gain -120 ~ 120 (-12dB ~ 12dB) with 0.1dB/step \n");
#else
        Printf("[Gain]        Gain -24 ~ 24 (-12dB ~ 12dB) with 0.5dB/step \n");
#endif
        return -1;
    }

    u1PEQSet    = (UINT8)StrToInt(szArgv[1]);
    u4Frequency = (UINT32)StrToInt(szArgv[2]);
    eQValue     = (UINT32)StrToInt(szArgv[3]);

    Argv = szArgv[4];
    if (Argv[0] == '-')
    {
        i2Gain = (UINT16)StrToInt(&Argv[1]);
        i2Gain = (UINT16)0xFFFF - (i2Gain-1);
    }
    else
    {
        i2Gain = (UINT16)StrToInt(szArgv[4]);
    }


    AUD_DspSetPEQCfg2_R(AUD_DEC_MAIN, u1PEQSet, u4Frequency, eQValue, i2Gain);

    return 1;
}
#endif
#endif


static INT32 _AudCmdUopPEQCfg(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1PEQSet;
    UINT32 u4Frequency;
    UINT32 u2BW;
    INT16 i2Gain;
    const CHAR *Argv;

    if (i4Argc < 5)
    {
        Printf("Usage: peq [set] [freq] [Bandwidth] [Gain] \n");
        Printf("[set]         0 ~ 8 \n");
        Printf("[u4Frequency] 20 ~ 20K \n");
        Printf("[Bandwidth]   0 ~ 23K \n");
#ifdef CC_AUD_SX1_FEATURE
        Printf("[Gain]        Gain -120 ~ 120 (-12dB ~ 12dB) with 0.1dB/step \n");
#else
        Printf("[Gain]        Gain -24 ~ 24 (-12dB ~ 12dB) with 0.5dB/step \n");
#endif
        return -1;
    }

    u1PEQSet    = (UINT8)StrToInt(szArgv[1]);
    u4Frequency = (UINT32)StrToInt(szArgv[2]);
    u2BW        = (UINT32)StrToInt(szArgv[3]);

    Argv = szArgv[4];
    if (Argv[0] == '-')
    {
        i2Gain = (UINT16)StrToInt(&Argv[1]);
        i2Gain = (UINT16)0xFFFF - (i2Gain-1);
    }
    else
    {
        i2Gain = (UINT16)StrToInt(szArgv[4]);
    }
    if(u2BW > 23000)
    {
        u2BW = 23000;
    }


    AUD_DspSetPEQCfg(AUD_DEC_MAIN, u1PEQSet, u4Frequency, u2BW, i2Gain);

    return 1;
}
#ifdef CC_AUD_PEQ_LR

static INT32 _AudCmdUopPEQCfg_R(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1PEQSet;
    UINT32 u4Frequency;
    UINT32 u2BW;
    INT16 i2Gain;
    const CHAR *Argv;

    if (i4Argc < 5)
    {
        Printf("Usage: peq [set] [freq] [Bandwidth] [Gain] \n");
        Printf("[set]         0 ~ 8 \n");
        Printf("[u4Frequency] 20 ~ 20K \n");
        Printf("[Bandwidth]   0 ~ 23K \n");
#if 1
        Printf("[Gain]        Gain -120 ~ 120 (-12dB ~ 12dB) with 0.1dB/step \n");
#else
        Printf("[Gain]        Gain -24 ~ 24 (-12dB ~ 12dB) with 0.5dB/step \n");
#endif

        return -1;
    }

    u1PEQSet    = (UINT8)StrToInt(szArgv[1]);
    u4Frequency = (UINT32)StrToInt(szArgv[2]);
    u2BW        = (UINT32)StrToInt(szArgv[3]);

    Argv = szArgv[4];
    if (Argv[0] == '-')
    {
        i2Gain = (UINT16)StrToInt(&Argv[1]);
        i2Gain = (UINT16)0xFFFF - (i2Gain-1);
    }
    else
    {
        i2Gain = (UINT16)StrToInt(szArgv[4]);
    }
    if(u2BW > 23000)
    {
        u2BW = 23000;
    }


    AUD_DspSetPEQCfg_R(AUD_DEC_MAIN, u1PEQSet, u4Frequency, u2BW, i2Gain);

    return 1;
}
#endif
#endif

static INT32 _AudCmdUopEqFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Enable;

    if (i4Argc < 3)
    {
        Printf("Usage: flag      [decoder id] [on/off]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[on/off]       1: on 0: off\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DspChannelEqEnable(u1DecId, (BOOL)u1Enable);

    return 1;
}

static INT32 _AudCmdUopSbassFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Idx;
    UINT8 u1Enable;

    if (i4Argc < 4)
    {
        Printf("Usage: flag      [decoder id] [index] [on/off]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[index]         0:normal  1: bass  2: pipe  3: treble\n");
        Printf("[on/off]       1: on 0: off\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Idx = (UINT8)StrToInt(szArgv[2]);
    u1Enable = (UINT8)StrToInt(szArgv[3]);

    switch(u1Idx)
    {
    case 0:
        AUD_DspBassBoostEnable(u1DecId, (BOOL)u1Enable);
        AUD_DspSuperBassEnable(u1DecId, (BOOL)0);
        AUD_DspClearBoostEnable(u1DecId, (BOOL)u1Enable);
        break;
    case 1:
        AUD_DspBassBoostEnable(u1DecId, (BOOL)u1Enable);
        break;
    case 2:
        AUD_DspSuperBassEnable(u1DecId, (BOOL)u1Enable);
        break;
    case 3:
        AUD_DspClearBoostEnable(u1DecId, (BOOL)u1Enable);
        break;
    default:
        break;
    }

    return 1;
}

static INT32 _AudCmdUopBassTrebleCutOffFreq(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1BassFc,u1TrebleFc;

    if (i4Argc < 3)
    {
        Printf("Usage: cf    [Bass Fc] [Treble Fc]\n");
        Printf("[Bass Fc]    0:100  1:150 2:200 \n");
        Printf("[Treble Fc]  0:5K   1:6K  2:7K \n");
        return -1;
    }

    u1BassFc = (UINT8)StrToInt(szArgv[1]);
    u1TrebleFc = (UINT8)StrToInt(szArgv[2]);
    
    _AUD_DspBassTrebleCutOffFreq(AUD_DEC_MAIN, u1BassFc, u1TrebleFc);
    return 1;
}

static INT32 _AudCmdUopBassBoostGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Level;

    if (i4Argc < 3)
    {
        Printf("Usage: bb      [decoder id] [level]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[level]        0 ~ 28 \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Level = (UINT8)StrToInt(szArgv[2]);

    AUD_DspBassBoostGain(u1DecId, u1Level);
    return 1;
}

static INT32 _AudCmdUopClearBoostGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Level;

    if (i4Argc < 3)
    {
        Printf("Usage: cb      [decoder id] [level]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[level]        0 ~ 28 \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Level = (UINT8)StrToInt(szArgv[2]);

    AUD_DspClearBoostGain(u1DecId, u1Level);
    return 1;
}

static INT32 _AudCmdUopSbassTableQry(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4BassLevel, u4TrebleLevel, u4BassFc, u4TrebleFc;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    u4BassLevel = AUD_DspGetBassBoostGain(AUD_DEC_MAIN);
    u4TrebleLevel = AUD_DspGetClearBoostGain(AUD_DEC_MAIN);
    u4BassFc = _AUD_DspBassCutOffFreqQry(AUD_DEC_MAIN);
    u4TrebleFc = _AUD_DspTrebleCutOffFreqQry(AUD_DEC_MAIN);

    Printf("Bass Enable: %d\n", AUD_DspGetBassBoostEnable(AUD_DEC_MAIN));
    Printf("Bass level: %d\n", u4BassLevel);
    Printf("Bass Cut-off Freq: %d\n", (100 + u4BassFc*50));
    
    Printf("Treble Enable: %d\n", AUD_DspGetClearBoostEnable(AUD_DEC_MAIN));
    Printf("Treble level: %d\n", u4TrebleLevel);    
    Printf("Treble Cut-off Freq: %d\n", (5000 + u4TrebleFc*1000));

    return 1;
}

#ifdef CC_AUD_FY12_LOUDNESS
static INT32 _AudCmdUopSbassFlag2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Idx;
    UINT8 u1Enable;

    if (i4Argc < 4)
    {
        Printf("Usage: flag      [decoder id] [index] [on/off]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        Printf("[index]         0:normal  1: bass  2: pipe  3: treble\n");
        Printf("[on/off]       1: on 0: off\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Idx = (UINT8)StrToInt(szArgv[2]);
    u1Enable = (UINT8)StrToInt(szArgv[3]);

    switch(u1Idx)
    {
    case 0:
        AUD_DspBassBoostEnable2(u1DecId, (BOOL)u1Enable);
        AUD_DspSuperBassEnable2(u1DecId, (BOOL)0);
        AUD_DspClearBoostEnable2(u1DecId, (BOOL)u1Enable);
        break;
    case 1:
        AUD_DspBassBoostEnable2(u1DecId, (BOOL)u1Enable);
        break;
    case 2:
        AUD_DspSuperBassEnable2(u1DecId, (BOOL)u1Enable);
        break;
    case 3:
        AUD_DspClearBoostEnable2(u1DecId, (BOOL)u1Enable);
        break;
    default:
        break;
    }
    DSP_SendDspTaskCmd(UOP_DSP_AOUT_REINIT);    
    return 1;
}

static INT32 _AudCmdUopBassBoostGain2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Level;

    if (i4Argc < 3)
    {
        Printf("Usage: bb      [decoder id] [level]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        //Printf("[level]        4 ~ 14(-10~0dB) \n");
        Printf("[level]        28 ~ 56(0~14dB, with 0.5dB step) \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Level = (UINT8)StrToInt(szArgv[2]);

    AUD_DspBassBoostGain2(u1DecId, u1Level);
    return 1;
}

static INT32 _AudCmdUopClearBoostGain2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Level;

    if (i4Argc < 3)
    {
        Printf("Usage: cb      [decoder id] [level]\n");
        Printf("[decoder id]   0:FIRST  1:SECOND \n");
        //Printf("[level]        4 ~ 14(-10~0dB) \n");
        Printf("[level]        28 ~ 56(0~14dB, with 0.5dB step) \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Level = (UINT8)StrToInt(szArgv[2]);

    AUD_DspClearBoostGain2(u1DecId, u1Level);
    return 1;
}

static INT32 _AudCmdUopSbassTableQry2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4BassLevel, u4TrebleLevel;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    u4BassLevel = AUD_DspGetBassBoostGain2(AUD_DEC_MAIN);
    u4TrebleLevel = AUD_DspGetClearBoostGain2(AUD_DEC_MAIN);
    Printf("Bass ATTN Enable: %d\n", AUD_DspGetBassBoostEnable2(AUD_DEC_MAIN));
    Printf("Bass ATTN level: %d\n", u4BassLevel);
    Printf("Treble ATTN Enable: %d\n", AUD_DspGetClearBoostEnable2(AUD_DEC_MAIN));
    Printf("Treble ATTN level: %d\n", u4TrebleLevel);

    return 1;
}
#endif

#ifdef CC_AUD_SILENCE_SUPPORT
static INT32 _AudCmdUopSilenceFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc != 2)
    {
        Printf("Usage: flag    [on/off]\n");
        Printf("[on/off]       1: on 0: off\n");
        return -1;
    }

    u1Enable = (UINT32)StrToInt(szArgv[1]);

    if(u1Enable == 1)
    {
        DSP_SilenceSetMode(TRUE);
    }
    else
    {
        DSP_SilenceSetMode(FALSE);
    }

    return 1;
}

static INT32 _AudCmdUopSilenceThreshold(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Threshold;

    if (i4Argc != 2)
    {
        Printf("Usage: thre    [threshold]\n");
        Printf("[threshold]       0x0~0x1000\n");
        return -1;
    }

    u4Threshold = (UINT32)StrToInt(szArgv[1]);
    DSP_SilenceSetThreshold(u4Threshold);

    return 1;
}

static INT32 _AudCmdUopSilenceWait(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Wait;

    if (i4Argc != 2)
    {
        Printf("Usage: wait    [wait time:ms]\n");
        Printf("[wait time:ms]       0~10000\n");
        return -1;
    }

    u4Wait = (UINT32)StrToInt(szArgv[1]);
    DSP_SilenceSetWait(u4Wait);

    return 1;
}

static INT32 _AudCmdUopSilenceFixGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4AttackFixGain;

    if (i4Argc != 2)
    {
        Printf("Usage: gain    [u4AttackFixGain]\n");
        Printf("[gain]       0x0~0x7fffff\n");
        return -1;
    }
    u4AttackFixGain = (UINT32)StrToInt(szArgv[1]);
    DSP_SilenceSetFixGain(u4AttackFixGain);
    return 1;
}

static INT32 _AudCmdUopSilenceReleaseStep(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4ReleaseStep;

    if (i4Argc != 2)
    {
        Printf("Usage: thr    [Release:ms]\n");
        Printf("[threshold]       0~1000\n");
        return -1;
    }
    u4ReleaseStep = (UINT32)StrToInt(szArgv[1]);
    DSP_SilenceSetReleaseStep(u4ReleaseStep);
    return 1;
}
static INT32 _AudCmdUopSilenceAttackStep(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4AttackStep;

    if (i4Argc != 2)
    {
        Printf("Usage: thr    [Attack:ms]\n");
        Printf("[threshold]       0~1000\n");
        return -1;
    }
    u4AttackStep = (UINT32)StrToInt(szArgv[1]);
    DSP_SilenceSetAttackStep(u4AttackStep);
    return 1;
}

static INT32 _AudCmdUopSilenceSetQry(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Enable;
    UINT32 u4Threshold;
    UINT32 u4PeakValue;
    UINT32 u4Wait;
    UINT32 u4AttackFixGain;
    UINT32 u4ReleaseStep;
    UINT32 u4AttackStep;
    
    if (i4Argc != 1)
    {
        return -1;
    }

    DSP_QrySilenceMode(&u4Enable);
    DSP_QrySilenceSetThreshold(&u4Threshold);
    DSP_QrySilencePeakValue(&u4PeakValue);
    DSP_QrySilenceWait(&u4Wait);
    DSP_QrySilenceAttackFixGain(&u4AttackFixGain);
    DSP_QrySilenceReleaseStep(&u4ReleaseStep);
    DSP_QrySilenceAttackStep(&u4AttackStep);
    Printf("Enable               %d\n", u4Enable);
    Printf("Threshold          0x%x\n", u4Threshold);
    Printf("Noise Level        0x%x\n", u4PeakValue);
    Printf("Attack Wait        %dms\n", u4Wait);
    Printf("Attack Gain        0x%x\n", u4AttackFixGain);
    Printf("Release            %dms\n", u4ReleaseStep);
    Printf("Attack             %dms\n", u4AttackStep);
    return 1;
}
#endif

static INT32 _AudCmdUopEqMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("Usage: m     [decoder id] [mode]\n");
        Printf("[decoder id]  0:FIRST  1:SECOND \n");
        Printf("[mode]        0 NONE   \n");
        Printf("              1 ROCK   \n");
        Printf("              2 POP    \n");
        Printf("              3 LIVE   \n");
        Printf("              4 DANCE  \n");
        Printf("              5 TECHNO \n");
        Printf("              6 Classic\n");
        Printf("              7 Soft\n");
        Printf("              8 Custom 1\n");
        Printf("              9 Custom 2\n");
        Printf("              10 Custom 3\n");

        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    AUD_DspChEqLoadPreset(u1DecId, (AUD_EQ_TYPE_T)u1Mode);
    return 1;
}

static INT32 _AudCmdUopEqTableQry(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8  eqValue[CH_EQ_BAND_NO+1];
    int   i;
    AUD_EQ_TYPE_T eEqType;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_DspGetEqType(AUD_DEC_MAIN, &eEqType);
    AUD_DspChQryEqTable(AUD_DEC_MAIN, eEqType, eqValue);

    Printf("EQ Enable: %d\n", AUD_DspGetChannelEqEnable(AUD_DEC_MAIN));
    Printf("EQ type = %d\n", eEqType);
    for (i = 1; i <= CH_EQ_BAND_NO; i++)
    {
        printf ("band%d : %d\n", i, eqValue[i]);
    }
    return 1;
}

#ifdef CC_AUD_7BAND_EQUALIZER
static INT32 _AudCmdUopEqTableSet(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8  eqValue[CH_EQ_BAND_NO+1];
    INT8  eqRange;
    int   i;
    AUD_EQ_TYPE_T eEqType;

    eqRange  = (INT8)StrToInt(szArgv[1]);
    if ((i4Argc < 5)||((eqRange !=0) && (eqRange !=1)) )
    {

        Printf("Usage: set  [range=0] [band1] [band2] [band3] [band4] \n");
        Printf("Usage: set  [range=1] [band5] [band6] [band7]\n");

        Printf("[bandn]: -28 ~ 28 (for -14dB ~ 14dB, 0.5 dB/Step) \n");
        return -1;
    }

    AUD_DspGetEqType(AUD_DEC_MAIN, &eEqType);
#ifdef CC_AUD_USE_FLASH_AQ
    //This is for FlashAQ FY10 Spec only. (driver control EQ).
#else
    if (eEqType == AUD_EQ_OFF)
    {
        eEqType = AUD_EQ_CUST1;
    }
#endif
    Printf("\n Your setting to current EQ type : %d\n", eEqType);
    AUD_DspChQryEqTable(AUD_DEC_MAIN, eEqType, eqValue);

    if (eqRange == 0)
    {
        for (i = 1; i <= 4; i++)
        {
            Printf("Band[%d] : ", i);
            eqValue[i] = (INT8)i4StrToInt(szArgv[i+1]);
            Printf("%d \n", eqValue[i] );
            //eqValue[i] -= (EQ_BAND_GAIN_DB_MAX * 2);
        }
    }
    else if (eqRange == 1)
    {
        for (i = 1; i <= 3; i++)
        {
            Printf("Band[%d] : ", i+4);
            eqValue[i+4] = (INT8)i4StrToInt(szArgv[i+1]);
            Printf("%d \n", eqValue[i+4] );
            //eqValue[i+4] -= (EQ_BAND_GAIN_DB_MAX * 2);
        }
    }

    eqValue[0] = 0;
    AUD_DspChSetEqTable(AUD_DEC_MAIN, eEqType, eqValue); //user defined EQ
    AUD_DspChEqLoadPreset(AUD_DEC_MAIN, eEqType);
    return 1;
}
#else
static INT32 _AudCmdUopEqTableSet(INT32 i4Argc, const CHAR ** szArgv)
{
    INT8  eqValue[CH_EQ_BAND_NO+1];
    int   i;
    AUD_EQ_TYPE_T eEqType;

    if (i4Argc < 6)
    {

        Printf("Usage: set  [band1] [band2] [band3] [band4] [band5]\n");
        Printf("[bandn]: -28 ~ 28 (for -14dB ~ 14dB, 0.5 dB/Step) \n");
        return -1;
    }

    AUD_DspGetEqType(AUD_DEC_MAIN, &eEqType);

    if (eEqType == AUD_EQ_OFF)
    {
        eEqType = AUD_EQ_CUST1;
    }
    Printf("\n Your setting to current EQ type : %d\n", eEqType);

    eqValue[0] = 0;
    for (i = 1; i <= CH_EQ_BAND_NO; i++)
    {
        Printf("Band[%d] : ", i);
        eqValue[i] = (INT8)i4StrToInt(szArgv[i]);
        Printf("%d \n", eqValue[i] );
        //eqValue[i] -= (EQ_BAND_GAIN_DB_MAX * 2);
    }
    AUD_DspChSetEqTable(AUD_DEC_MAIN, eEqType, eqValue); //user defined EQ
    AUD_DspChEqLoadPreset(AUD_DEC_MAIN, eEqType);
    return 1;
}
#endif

#ifdef CC_AUD_DRC_V20
#if 0
static INT32 _AudCmdUopLimiterMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: m  [mode]\n");
        Printf("[mode]    0: off\n");
        Printf("          1: mtk style adaptive mode\n");
        Printf("          2: mtk style fixed mode\n");
        Printf("          3: adaptive mode\n");
        Printf("          4: fixed mode\n");        
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);
    AUD_DspSetLimiterMode(u1Mode);
    return 1;
}
#endif

static INT32 _AudCmdUopLimiterFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2Flag = 0, u2Temp;
    UINT8 i;

    if (i4Argc < (AUD_LIMITER_NUM+1))
    {
        Printf("Usage: flag  [vol] [vsurr] [sbass] [EQ] [reverb] [speaker] [voice]\n");
        Printf("0: off   1: on\n");
        return -1;
    }

    for (i=1; i<=AUD_LIMITER_NUM; i++)
    {
        u2Temp = (UINT8)StrToInt(szArgv[i]);
        u2Temp = ((u2Temp & 0x01) << (i - 1));
        u2Flag |= u2Temp;
    }

    AUD_DspSetLimiterFlag(u2Flag);
    return 1;
}

static INT32 _AudCmdUopLimiterThre(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Thre;

    if (i4Argc < 2)
    {
        goto lbUsage;    
    }

    u4Thre = (UINT32)StrToInt(szArgv[1]);
    if ((u4Thre < 0x1000) || (u4Thre > 0x7fffff))
    {
        goto lbUsage; 
    }
    AUD_DspSetLimiterThreshold(u4Thre);
    return 1;

lbUsage:
    Printf("Usage: thre  [threshold]\n");
    Printf("[threshold]     0x1000 ~ 0x7fffff (-48dB ~ 18dB)\n"); 
    return -1;    
}

#define DRC2_RATE_RATIO (5.33) // 256 samples at 48 kHz

#if 0
static INT32 _AudCmdUopLimiterAttackRate(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4AttackRate;

    if (i4Argc < 2)
    {
        goto lbUsage;    
    }

    u4AttackRate = (UINT32)StrToInt(szArgv[1]);
    if (u4AttackRate > 0xC000)
    {
        goto lbUsage; 
    }
    AUD_DspSetLimiterAttackRate(u4AttackRate);
    return 1;

lbUsage:
    Printf("Usage: attack  [attack rate]\n");
    Printf("[threshold]     0x0 ~ 0xC000 (0 ~ 192 dB/ms)\n");
    Printf("                0x1 = 0.0039 dB/ms\n");
    return -1;    
}
#endif

static INT32 _AudCmdUopLimiterRelaseRate(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4ReleaseRate;

    if (i4Argc < 2)
    {
        goto lbUsage;    
    }

    u4ReleaseRate = (UINT32)StrToInt(szArgv[1]);
    if (u4ReleaseRate > 0xC000)
    {
        goto lbUsage; 
    }
    AUD_DspSetLimiterRelaseRate(u4ReleaseRate);
    return 1;

lbUsage:
    Printf("Usage: release  [release rate]\n");
    Printf("[threshold]     0x0 ~ 0xC000 (0 ~ 48 dB/ms)\n");
    Printf("                0x1 = 0.00098 dB/ms\n");
    return -1;    
}

static INT32 _AudCmdUopLimiterGainRatio(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Ratio;

    if (i4Argc < 2)
    {
        Printf("Usage: ratio  [value]\n");
        Printf("[value]     0x0 ~ 0x7fffff (infinite:1 ~ 1:1)\n"); 
        return -1;
    }

    u4Ratio = (UINT32)StrToInt(szArgv[1]);
    AUD_DspSetLimiterGainRatio(u4Ratio);
    return 1;
}

static INT32 _AudCmdUopLimiterQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Mode;
    UINT16 u2Flag;
    UINT32 u4Thre, u4Attack, u4Release, u4Ratio;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_DspGetLimiterConfig(&u1Mode, &u2Flag, &u4Thre);
    AUD_DspGetLimiterConfig1(&u4Attack, &u4Release, &u4Ratio);
    
    if (u1Mode == 0)
    {
        Printf("Mode:          off\n");
    }
    else if (u1Mode == 1)
    {
        Printf("Mode:          MTK style adaptive mode\n");
    }
    else if (u1Mode == 2)
    {
        Printf("Mode:          MTK style fixed mode\n");
    }
    else if (u1Mode == 3)
    {
        Printf("Mode:          NEW style adaptive mode\n");
    }
    else if (u1Mode == 4)
    {
        Printf("Mode:          NEW style fixed mode\n");
    }
    else
    {
        Printf("Mode:          wrong mode setting\n");
    }

    Printf("Flag:          0x%x\n", u2Flag);
    Printf("Threshold:     0x%x\n", u4Thre);
    //Printf("Attack rate:   0x%x\n", u4Attack);
    Printf("Relase rate:   0x%x\n", u4Release);
    Printf("Ratio:         0x%x\n", u4Ratio);

    return 1;
}
#else
static INT32 _AudCmdUopLimiterMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: m  [mode]\n");
        Printf("[mode]    0: off\n");
        Printf("          1: adaptive mode\n");
        Printf("          2: fixed mode\n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);
    AUD_DspSetLimiterMode(u1Mode);
    return 1;
}

static INT32 _AudCmdUopLimiterFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2Flag = 0, u2Temp;
    UINT8 i;

    if (i4Argc < (AUD_LIMITER_NUM+1))
    {
        Printf("Usage: flag  [vol] [vsurr] [sbass] [EQ] [reverb] [speaker] [voice]\n");
        Printf("0: off   1: on\n");
        return -1;
    }

    for (i=1; i<=AUD_LIMITER_NUM; i++)
    {
        u2Temp = (UINT8)StrToInt(szArgv[i]);
        u2Temp = ((u2Temp & 0x01) << (i - 1));
        u2Flag |= u2Temp;
    }

    AUD_DspSetLimiterFlag(u2Flag);
    return 1;
}

static INT32 _AudCmdUopLimiterThre(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Thre;

    if (i4Argc < 2)
    {
        Printf("Usage: thre  [threshold]\n");
        Printf("[threshold]     0x0 ~ 0x7fffff\n");
        return -1;
    }

    u4Thre = (UINT32)StrToInt(szArgv[1]);
    AUD_DspSetLimiterThreshold(u4Thre);
    return 1;
}

#ifdef CC_RATIO_LIMITER
static INT32 _AudCmdUopLimiterGainRatio(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Ratio;

    if (i4Argc < 2)
    {
        Printf("Usage: ratio  [value]\n");
        Printf("[value]     0x0 ~ 0x7fffff\n");
        return -1;
    }

    u4Ratio = (UINT32)StrToInt(szArgv[1]);
    AUD_DspSetLimiterGainRatio(u4Ratio);
    return 1;
}
#endif

static INT32 _AudCmdUopLimiterQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Mode;
    UINT16 u2Flag;
    UINT32 u4Thre;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_DspGetLimiterConfig(&u1Mode, &u2Flag, &u4Thre);
    if (u1Mode == 0)
    {
        Printf("Mode: off\n");
    }
    else if (u1Mode == 1)
    {
        Printf("Mode: adaptive mode\n");
    }
    else
    {
        Printf("Mode: fixed mode\n");
    }

    Printf("Flag: 0x%x\n", u2Flag);
    Printf("Threshold: 0x%x\n", u4Thre);

    return 1;
}
#endif ///CC_AUD_DRC_V20

static INT32 _AudCmdUopDownmixPosition(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DMPosition;

    if (i4Argc < 2)
    {
        Printf("Usage: dmp [Downmix Channel(CH9/10) Postion]\n");
        Printf("[mode] 0  do ALL post processing   \n");
        Printf("       1  do downmix after input source gain and before AVC\n");
        Printf("       2  Skip post-proc & do manual output matrix\n");
        Printf("       3  Skip post-proc & skip manual output matrix\n");
        return -1;
    }

    u1DMPosition = (UINT8)StrToInt(szArgv[2]);

    AUD_DspDownmixPosition(AUD_DSP0, u1DMPosition);
    return 1;
}



static INT32 _AudCmdDspSpeed(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT16 u2Speed;

    if (i4Argc < 2)
    {
        Printf("Usage: cb     [decoder id] [FF Speed times]\n");
        Printf("[decoder id]  0:FIRST  1:SECOND  2:THIRD\n");
        Printf("[mode]        0 normal play   \n");
        Printf("                 2  FF 2-times   \n");
        Printf("                 4  FF 4-times    \n");
        Printf("                 8  FF 8-times   \n");
        Printf("                16  FF 16-times  \n");
        Printf("                32 FF 32-times \n");


        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u2Speed = (UINT16)StrToInt(szArgv[2]);

    if(u2Speed != 0)
    {
        u2Speed --;
    }

    AUD_DspSpeed(u1DecId, u2Speed);
    return 1;
}

static INT32 _AudCmdUopMultiPairOutput(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT16 u2Flag;

    if (i4Argc < 2)
    {
        Printf("Usage: MultiPair [decoder id] [ON/OFF]\n");
        Printf("[decoder id]  0:FIRST  1:SECOND \n");
        Printf("[mode]        0  Didn't Suppot multi-pair output \n");
        Printf("                 1  Suppot multi-pair output(Copy CH9/10 to LS/RS/C/SUB)   \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u2Flag = (UINT16)StrToInt(szArgv[2]);

    AUD_DspMultiPairOutput(AUD_DSP0, u1DecId, u2Flag);
    return 1;
}

static INT32 _AudCmdUopLRDownMix(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL u1Flag;

    if (i4Argc < 2)
    {
        Printf("Usage: LRDMix [ON/OFF]\n");
        Printf("[mode]        0  Didn't do L/R downmix \n");
        Printf("                 1  Downmix to L/R channel after Reverbation and before BassManagement   \n");
        return -1;
    }
    if (i4Argc >= 2)
    {
        if (x_strcmp(szArgv[1], "on") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "1") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "off") == 0)
        {
            u1Flag = FALSE;
        }
        else if (x_strcmp(szArgv[1], "0") == 0)
        {
            u1Flag = FALSE;
        }
        else
        {
           Printf("Usage: LRDMix [ON/OFF]\n");
           Printf("[mode]        0  Didn't do L/R downmix \n");
           Printf("                 1  Downmix to L/R channel after Reverbation and before BassManagement   \n");
               return -1;
           }
     }
    AUD_DspLRDownmix(AUD_DSP0, AUD_DEC_MAIN, u1Flag);
    return 1;
}


static INT32 _AudCmdDspUopDump(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_DumpUopCommand();
#ifdef CC_ENABLE_AV_SYNC
    Aud_ListLogDump(); //CC_AUD_DUMP_LOG
#endif
    return 1;
}

static INT32 _AUD_DSPDumpIntHistory(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_DSPDumpIntHistory(AUD_DSP0);
    AUD_DSPDumpIntHistory(AUD_DSP1);
    return 1;   
}

static INT32 _AudCfgAvInSwitch(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1InputId;

    if (i4Argc < 2)
    {
        Printf("Usage: isw    [source]\n");
        Printf("[source]      0 Composite video (CVBS) # 0   \n");
        Printf("              1 Composite video (CVBS) # 1   \n");
        Printf("              2 Composite video (CVBS) # 2   \n");
        Printf("              3 Composite video (CVBS) # 3   \n");
        Printf("              4 S-Video #0   \n");
        Printf("              5 S-Video #1  \n");
        Printf("              6 S-Video #2  \n");
        Printf("              7 YPbPr # 0 \n");
        Printf("              8 YPbPr # 1 \n");
        Printf("              9 YPbPr # 2 \n");
        Printf("              10 VGA, via headphone jet  # 0\n");
        Printf("              11 VGA, via headphone jet  # 1\n");
        Printf("              12 DVI  # 0 \n");
        Printf("              13 DVI  # 1 \n");
        Printf("              14 DVI  # 2 \n");
        Printf("              15 DVI  # 3 \n");
        Printf("              22 SCART0\n");
        Printf("              23 SCART1 \n");


        return -1;
    }

    u1InputId = (UINT8)StrToInt(szArgv[1]);
    AUD_AvMuxSel((AUD_INPUT_ID_T)u1InputId);

    return 1;
}

static INT32 _AudCfgChlOutSwitch(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Data0;
    UINT8 u1Data1;
    UINT8 u1Data2;
    UINT8 u1Data3;
    UINT8 u1Data4;
    //UINT8 u1Data5;    //MT5365 only has 5 I2S data pin (AOSDATA0~AOSDATA4)

    if (i4Argc < 6)
    {
        if ((i4Argc == 2) && (szArgv[1][0]=='q'))
        {
            Printf("[OSDATA setting]\n");
            AUD_AoutPADMuxQuery();
            return 1;
        }
        Printf("Usage: osw    [PAD_SDATA0] [PAD_SDATA1] [PAD_SDATA2] \n");
        Printf("              [PAD_SDATA3] [PAD_SDATA4]\n");
        Printf("       osw q\n");
        Printf("[PAD_DATA]    0 L/R   \n");
        Printf("              1 Ls/Rs \n");
        Printf("              2 C/Sw  \n");
        Printf("              3 CH7/8(IEC)  \n");
        Printf("              4 CH9/10(HP)   \n");
        Printf("              5 CH11/12(Mon) \n");
        Printf("              6 CH13/14 \n");
        Printf("              7 Aux decoder output \n");
        Printf("ex. osw 0 1 2 3 4\n");
        return -1;
    }

    u1Data0 = (UINT8)StrToInt(szArgv[1]);
    u1Data1 = (UINT8)StrToInt(szArgv[2]);
    u1Data2 = (UINT8)StrToInt(szArgv[3]);
    u1Data3 = (UINT8)StrToInt(szArgv[4]);
    u1Data4 = (UINT8)StrToInt(szArgv[5]);
    //u1Data5 = (UINT8)StrToInt(szArgv[6]);

    AUD_AoutPADMux((AUD_CH_T)(u1Data0*2), (AUD_CH_T)(u1Data1*2), (AUD_CH_T)(u1Data2*2),
                   (AUD_CH_T)(u1Data3*2), (AUD_CH_T)(u1Data4*2), AUD_CH_AUX_FRONT_LEFT, 0);

    return 1;
}

extern void AUD_SltCheckStream(void);
extern void AUD_WriteGoldenStream(void);

#ifdef LINUX_TURNKEY_SOLUTION
static UINT8* pTonePlayBuf;
#endif

static INT32 _AudCmdUniversalTone(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4LeftFreq, i4RightFreq;

    if (i4Argc < 3)
    {
        Printf("Usage: utone [left freq] [right freq]\n");
        return -1;
    }

    i4LeftFreq = (INT32)StrToInt(szArgv[1]);
    i4RightFreq = (INT32)StrToInt(szArgv[2]);
    AUD_UniversalTonePlay(i4LeftFreq, i4RightFreq);

    return 1;
}

static INT32 _AudCmdPlayTone(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Sel = 0;
    UINT8 u1DecId;
    UINT8 u1DspId;
    MEM_BUFFER_INFO_T  rMemBuf;
    AUD_PCM_SETTING_T  rPcmSetting;

    if (i4Argc < 4)
    {
        Printf("Usage: tone [tone_sel] [dec_id][dsp_id]\n");
        Printf("[tone_sel]   0  1K tone 0dB\n");
        Printf("             1  1K tone -20dB\n");
        Printf("             2  1K tone -60dB\n");
        Printf("             3  1K tone -120dB\n");
        Printf("             4  2K tone 0dB\n");
        Printf("             5  3K tone 0dB\n");
        Printf("             6  4K tone 0dB\n");
        Printf("             7  5K tone 0dB\n");
        Printf("             8  8K tone 0dB\n");
        Printf("             9  12.5K tone 0dB\n");
        Printf("             10  20K tone 0dB\n");
        Printf("             11  200 tone 0dB\n");
        Printf("             12  200 tone -60dB\n");
        Printf("             13  400 tone 0dB\n");
        Printf("             14  1K/3K tone 0dB\n");
        Printf("             15  1K tone 0dB L channel only\n");
        Printf("[dec_id]     0: Main, 1: AUX, 2: 3rd, 3: 4th\n");
        Printf("[dsp_id]     0: DSP0, 1: DSP1\n");
        return -1;
    }

    u1Sel = (UINT8)StrToInt(szArgv[1]);

    u1DecId = (UINT8)StrToInt(szArgv[2]);
    u1DspId = (UINT8)StrToInt(szArgv[3]);
    if (u1DecId >= AUD_DEC_NUM)
    {
        u1DecId = AUD_DEC_MAIN;
    }

    if (AUD_IsDecoderPlay(u1DspId, u1DecId))
    {
        AUD_StopMemStr();
        AUD_DSPCmdStop(u1DspId, u1DecId);
    }

    rMemBuf.u4LoopCount = 0; // Infinite loop

    rPcmSetting.eSampleFreq = FS_48K;
    rPcmSetting.ePcmDataInfo = PCM_24_BITS;

    AUD_DspProcMode(u1DspId, u1DecId, 0xdc00);
    AUD_SetDecType(u1DspId, u1DecId, AUD_STREAM_FROM_MEMORY, AUD_FMT_PCM);
    AUD_PcmSetting(u1DspId, u1DecId, (const AUD_PCM_SETTING_T *)&rPcmSetting);

#ifdef LINUX_TURNKEY_SOLUTION
    if (pTonePlayBuf)
    {
        BSP_FreeAlignedDmaMemory((UINT32)pTonePlayBuf);
        pTonePlayBuf = NULL;
    }
#endif

    AUD_StrSelect(u1DspId, u1DecId, u1Sel, &rMemBuf);

#ifdef LINUX_TURNKEY_SOLUTION
    pTonePlayBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(rMemBuf.u4Length, 32);
    x_memcpy((VOID*)VIRTUAL((UINT32)pTonePlayBuf), (VOID*)VIRTUAL((UINT32)rMemBuf.pData), rMemBuf.u4Length);
    rMemBuf.pData = (UINT8*)VIRTUAL((UINT32)pTonePlayBuf);
#endif

    AUD_SetDecMemBuffer(u1DspId, u1DecId, &rMemBuf);
    AUD_DSPCmdPlay(u1DspId, u1DecId);

    return 1;
}

extern UINT8 _au1TableImpulse0dB[];
static INT32 _AdspDolbyImpulse(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_PCM_SETTING_T rPcmSetting;
    MEM_BUFFER_INFO_T  rMemBuf;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_MAIN))
    {
        AUD_StopMemStr();
        AUD_DSPCmdStop(AUD_DSP0, AUD_DEC_MAIN);
    }

    rMemBuf.u4LoopCount = 0; // Infinite loop

    rPcmSetting.eSampleFreq = FS_48K;
    rPcmSetting.ePcmDataInfo = PCM_24_BITS;

    AUD_DspProcMode(AUD_DSP0, AUD_DEC_MAIN, 0xdc00);
    AUD_SetDecType(AUD_DSP0, AUD_DEC_MAIN, AUD_STREAM_FROM_MEMORY, AUD_FMT_PCM);
    AUD_PcmSetting(AUD_DSP0, AUD_DEC_MAIN, (const AUD_PCM_SETTING_T *)&rPcmSetting);

#ifdef LINUX_TURNKEY_SOLUTION
    if (pTonePlayBuf)
    {
        BSP_FreeAlignedDmaMemory((UINT32)pTonePlayBuf);
        pTonePlayBuf = NULL;
    }

    rMemBuf.u4Length = 3*2*1024;

    pTonePlayBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(rMemBuf.u4Length, 32);
    rMemBuf.pData = (UINT8*)VIRTUAL((UINT32)pTonePlayBuf);

    x_memset((VOID*)VIRTUAL((UINT32)pTonePlayBuf), 0, rMemBuf.u4Length);
    x_memcpy((VOID*)VIRTUAL((UINT32)pTonePlayBuf), (VOID*)VIRTUAL((UINT32)_au1TableImpulse0dB), 3*2);
#else
    // TODO!!!!
    LOG (0, "This function does not support for non-linux system!\n");
#endif

    AUD_SetDecMemBuffer(AUD_DSP0, AUD_DEC_MAIN, &rMemBuf);
    AUD_DSPCmdPlay(AUD_DSP0, AUD_DEC_MAIN);

    LOG (0, "Left channel Impulse signal is playing...\n");
    LOG (0, "1) Please check if only left channel has signals.\n");
    LOG (0, "2) Please use frequency analyser to check the frequency response. It must be flat.\n");

    return 1;
}

static INT32 _AudCmdSoundEffect(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag = FALSE;

    if (i4Argc < 2)
    {
        Printf("Usage: soundeffect  [on/off]\n");
        Printf("[on/off]     1: on  \n");
        Printf("                 0: off  \n");
        return -1;
    }

    if (i4Argc >= 2)
    {
        if (x_strcmp(szArgv[1], "on") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "1") == 0)
        {
            u1Flag = TRUE;
        }
        else if (x_strcmp(szArgv[1], "off") == 0)
        {
            u1Flag = FALSE;
        }
        else if (x_strcmp(szArgv[1], "0") == 0)
        {
            u1Flag = FALSE;
        }
        else
        {
               Printf("Usage: soundeffect  [on/off]\n");
               Printf("[on/off]     1: on  \n");
               Printf("                0: off  \n");
               return -1;
           }
     }

    AUD_DspSoundEffectFlag(AUD_DEC_MAIN, u1Flag);

    return 1;
}

static INT32 _AudCmdBypassPostProcessing(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2Flag = FALSE;
    UINT8 u1DecId = AUD_DEC_MAIN;
    UINT8 u1DspId;

    if (i4Argc != 4)
    {
        Printf("Usage: bypass [dsp id][dec id] [on/off]\n");
        Printf("[dsp id] 0: DSP0, 1: DSP1\n");
        Printf("[dec id] 0: Main, 1: Aux, 2: Third, 3: 4th\n");
        Printf("[on/off] 1: on (0xf000), 0: 0ff\n");
        Printf("         0x8000: bypass delay\n");
        Printf("         0x4000: bypass trim\n");
        Printf("         0x2000: bypass post-proc\n");
        Printf("         0x1000: bypass bass-management\n");
        return -1;
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1DecId = (UINT8)StrToInt(szArgv[2]);
    u2Flag = (UINT8)StrToInt(szArgv[3]);

    AUD_DspBypassPostProcess(u1DspId, u1DecId, u2Flag);

    return 1;
}

#ifdef __MODEL_slt__
static INT32 _AudCmdSltSorting(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    if (SLT_SortingAud() == 0)
    {
        Printf("Soring result (ok)");
    }
    else
    {
        Printf("Soring result (fail)");
    }
    return 1;
}

static INT32 _AudCmdSltFlashing(INT32 i4Argc, const CHAR ** szArgv)
{

    UNUSED(i4Argc);
    UNUSED(szArgv);
    SLT_FlashingAudInit();

    return 1;
}

static INT32 _AudCmdDtvSltSorting(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (SLT_DtvSorting() == 0)
    {
        Printf("DTV Soring result (ok)");
    }
    else
    {
        Printf("DTV Soring result (fail)");
    }

    return 1;
}

static INT32 _AudCmdAtvSltSorting(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (SLT_AtvSorting() == 0)
    {
        Printf("ATV Soring result (ok)");
    }
    else
    {
        Printf("ATV Soring result (fail)");
    }
    return 1;
}

static INT32 _AudCmdIntAdSltSorting(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (SLT_IntAdSorting() == 0)
    {
        Printf("Internal AD Soring result (ok)");
    }
    else
    {
        Printf("Internal AD Soring result (fail)");
    }
    return 1;
}

static INT32 _AudCmdROMSltSorting(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (SLT_ROMSorting() == 0)
    {
        Printf("ROM Soring result (ok)");
    }
    else
    {
        Printf("ROM Soring result (fail)");
    }
    return 1;
}

#ifndef CC_AUD_DISABLE_2ND_DSP
static INT32 _AudCmdDSPBSltSorting(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (SLT_DSPBSorting() == 0)
    {
        Printf("DSPB Soring result (ok)");
    }
    else
    {
        Printf("DSPB Soring result (fail)");
    }
    return 1;
}
#endif
#endif

static INT32 _AudCmdStopMemStr(INT32 i4Argc, const CHAR ** szArgv)
{

    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_StopMemStr();

    return 1;
}

#ifdef CC_AUD_MIXSOUND_SUPPORT
static INT32 _AudCmdMixSndQuery(INT32 i4Argc,const CHAR** szArgv)
{
    Printf("[MixSound Status]\n");
    Printf("  Enable: %d\n", AUD_DspGetMixSndEnable());
    Printf("  Gain: 0x%x\n", AUD_DspGetMixSndGain());
    Printf("  Stereo: %d\n", AUD_DspGetMixSndStereo());
    Printf("  Vol: %d\n", AUD_DspGetMixSndVol());
    Printf("  Loop: %d\n", AUD_DspGetMixSndLoop());
    return 1;
}

static INT32 _AudCmdMixSndLoop(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Loop;
    if (i4Argc == 2)
    {
        u4Loop= (UINT32)StrToInt(szArgv[1]);
        if(u4Loop<=256)
        {
            AUD_DspSetMixSndLoop(u4Loop);
            return 1;
        }
    }

    Printf("Usage: mixsound loop [loop] \n");
    Printf("[mixsound loop] 0 ~ 256\n");
    return -1;
}

static INT32 _AudCmdMixSndVol(INT32 i4Argc,const CHAR** szArgv)
{
    UINT32 u4Volume;

    if (i4Argc == 2)
    {
        u4Volume = (UINT8)StrToInt(szArgv[1]);
        if(u4Volume<=100)
        {
            AUD_DspSetMixSndVol(u4Volume);
            return 1;
        }
    }

    Printf("Usage: mixsound volume [volume]\n");
    Printf("[mixsound volume] 0 ~ 100\n");
    return -1;
}

static INT32 _AudCmdMixSndFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag;

    if (i4Argc != 2)
    {
        Printf("Usage: flag [on/off]\n");
        Printf("[on/off] 1: on 0: off\n");
        return -1;
    }

    u1Flag = (UINT8)StrToInt(szArgv[1]);
    AUD_DspMixSndEnable(u1Flag);
    return 1;
}

static INT32 _AudCmdMixSndPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2Mode;

    u2Mode = (UINT16)StrToInt(szArgv[1]);

    if (i4Argc != 2)
    {
        goto lbUsage;
    }

    if ((u2Mode == 1)||(u2Mode == 2)||(u2Mode == 4)
        #ifdef NEW_MIXSOUND
        ||(u2Mode ==8)
        #endif
        )
    {
        #ifndef NEW_MIXSOUND
        #ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
        AUD_DisableMixSndClip();
        #endif
        #endif
        AUD_DspMixSndControl(u2Mode);
        return 1;
    }

lbUsage:
    Printf("Usage: p [mode]\n");
    Printf("[mode]   1 PLAY\n");
    Printf("         2 STOP\n");
    Printf("         4 FORCE STOP AND PLAY \n");
  #ifdef NEW_MIXSOUND
    Printf("         8 DISABLE\n");
  #endif
    return -1;
}

static INT32 _AudCmdMixSndDec3(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;

    fgEnable = (StrToInt(szArgv[1]) == 1)? TRUE:FALSE;

    if (i4Argc != 2)
    {
        goto lbUsage;
    }

    AUD_DspMixSndDec3(fgEnable);
    return 1;

lbUsage:
    Printf("Usage: dec3 [on/off]\n");
    Printf("         1 on\n");
    Printf("         0 off\n");
    return -1;
}

static INT32 _AudCmdMixSndChannel(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 wCh;

    wCh = StrToInt(szArgv[1]);

    if (i4Argc != 2)
    {
        goto lbUsage;
    }

    AUD_DspSetMixSndChannel(wCh);
    return 1;

lbUsage:
    Printf("Usage: ch [mode]\n");
    Printf(" 0x0100: Mix to L/R\n");
    Printf(" 0x0200: Mix to LS/RS\n");
    Printf(" 0x0400: Mix to C/SW\n");
    Printf(" 0x0800: Mix to 7/8\n");
    Printf(" 0x1000: Mix to 9/10\n");
    return -1;
}

static INT32 _AudCmdMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1ClipIdx;

    if (i4Argc != 2)
    {
        Printf("Usage: str [clip idx]\n");
        Printf("[clip idx] 0 ~ 3\n");
        return -1;
    }

    u1ClipIdx = (UINT8)StrToInt(szArgv[1]);
    AUD_DspMixSndClip(u1ClipIdx);
    return 1;
}

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY

static INT32 _AudCmdQueryMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_QueryMixSndClip();
    return 0;
}

static INT32 _AudCmdQueryMixSndClipDsp(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;

    if (i4Argc != 2)
    {
        Printf("Usage: qdsp [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);
    AUD_DspShowMixSndStatus(u1StreamID);
    return 0;
}

static INT32 _AudCmdStopMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;

    if (i4Argc != 2)
    {
        Printf("Usage: stop [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);
    AUD_StopMixSndClip(u1StreamID);

    return 0;
}

static INT32 _AudCmdPauseMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;

    if (i4Argc != 2)
    {
        Printf("Usage: pause [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);

    AUD_PauseMixSndClip(u1StreamID);
    return 0;
}

static INT32 _AudCmdResumeMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;

    if (i4Argc != 2)
    {
        Printf("Usage: resume [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);

    AUD_ResumeMixSndClip(u1StreamID);
    return 0;
}

static INT32 _AudCmdRepeatMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;

    if (i4Argc != 2)
    {
        Printf("Usage: repeat [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);

    AUD_RepeatMixSndClip(u1StreamID, 8);
    return 0;
}

static INT32 _AudCmdSetMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID = 0;
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4Gain = 0x8000;
    UINT8 u1StereoOnOff = TRUE;
    UINT32 u4SampleRate = 48000;
    UINT8 u1BitDepth = 16;    
    UINT8 u1Endian = 0;

    if (i4Argc != 9)
    {
        Printf("set [id] [addr] [size] [vol] [stereo on/off] [sample rate] [bit depth] [endian]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);
    u4Addr = (UINT32)StrToInt(szArgv[2]);
    u4Size = (UINT32)StrToInt(szArgv[3]);
    u4Gain = (UINT32)StrToInt(szArgv[4]);
    u1StereoOnOff = (UINT8)StrToInt(szArgv[5]);
    u4SampleRate = (UINT32)StrToInt(szArgv[6]);
    u1BitDepth = (UINT8)StrToInt(szArgv[7]);    
    u1Endian = (UINT8)StrToInt(szArgv[8]);

    AUD_SetMixSndClip(u1StreamID, u4Addr, u4Size, u4Gain, u1StereoOnOff, u4SampleRate, u1BitDepth, u1Endian);
    return 0;
}

static INT32 _AudCmdPlayMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1StreamID;
    if (i4Argc != 2)
    {
        Printf("Usage: play [u1StreamID]\n");
        return -1;
    }

    u1StreamID = (UINT8)StrToInt(szArgv[1]);

    AUD_PlayMixSndClip(u1StreamID);
    return 0;
}

static INT32 _AudCmdInitMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Init;
    if (i4Argc != 2)
    {
        Printf("Usage: init [0/1]\n");
        Printf("  0 - disable, 1 - enable\n");
        return -1;
    }

    u1Init = (UINT8)StrToInt(szArgv[1]);

    if (u1Init)
    {
        AUD_EnableMixSndClip();
    }
    else
    {
        AUD_DisableMixSndClip();
    }

    return 0;
}

#ifdef MIXSOUND_MIPS_MEASURE
void vAoutMIPSQuery(void);
void vAoutMIPSReset(void);

void vMixSndMIPSQuery(void);
void vMixSndMIPSReset(void);

static INT32 _AudCmdQueryMixSndClipMIPS(INT32 i4Argc, const CHAR ** szArgv)
{
    vAoutMIPSQuery();
    vMixSndMIPSQuery();
    return 0;
}

static INT32 _AudCmdResetMixSndClipMIPS(INT32 i4Argc, const CHAR ** szArgv)
{
    vAoutMIPSReset();
    vMixSndMIPSReset();
    return 0;
}
#endif

static INT32 _AudCmdClearMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_ClearMixSndClip();
    return 0;
}

extern BOOL fgMixSndClipMute;
static INT32 _AudCmdMuteMixSndClip(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("mclip.m [fgMute]\n");
        Printf("  0: Unmute, 1: Mute\n");
        Printf("  Current Setting: %d\n",fgMixSndClipMute);
    }
    else
    {
        fgMixSndClipMute = (BOOL)StrToInt(szArgv[1]);
    }
    return 0;
}

extern UINT32 u4GetMixSndGain(UINT8 u1StreamId);
extern void vSetMixSndGain(UINT8 u1StreamId,UINT32 u4Gain);
static INT32 _AudCmdSetGain(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc == 2)
    {
        UINT8 u1StreamId = (UINT8)StrToInt(szArgv[1]);
        Printf("Stream Id: %d Gain: 0x%x\n", u1StreamId, u4GetMixSndGain(u1StreamId));
    }
    else if (i4Argc == 3)
    {
        UINT8 u1StreamId = (UINT8)StrToInt(szArgv[1]);
        UINT32 u4Gain = (UINT32)StrToInt(szArgv[2]);
        vSetMixSndGain(u1StreamId, u4Gain);
    }
    else
    {
        Printf("== Set Gain ==\n"
               "  mclip.gain [u1StreamID] [gain]\n"
               "== Show Gain ==\n"
               "  mclip.gain [u1StreamID]\n");
    }
    return 0;
}

#ifdef MIXSND_OUTPUT_DBG
extern void vSetMixSndDumpInfo(UINT32 u4Addr, UINT32 u4Size);
static INT32 _AudCmdMixSndDumpAlloc(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Size = 0;
    static UINT8* pMemPlayBufNew = 0;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc != 2)
    {
        Printf("Usage: mda [Length]\n");
        Printf(" Length = 0 for free\n");
        return -1;
    }

    u4Size = (UINT32)StrToInt(szArgv[1]);
    if (u4Size == 0)
    {
        x_mem_free((VOID*)pMemPlayBufNew);
        Printf( "Free memory 0x%x \n",pMemPlayBufNew);
        pMemPlayBufNew = 0;
        return 0;
    }
    else
    {
        x_mem_free((VOID*)pMemPlayBufNew);
        Printf( "Free memory 0x%x \n",pMemPlayBufNew);
    }

    Printf( "Allocate memory size 0x%x for memory play...\n",u4Size);
    pMemPlayBufNew = (UINT8 *)x_mem_alloc_virtual(u4Size);
    if (pMemPlayBufNew == NULL)
    {
        printf( "Memory request for memory play fail !!\n");
        return -1;
    }

    Printf( "Allocate memory from 0x%x to 0x%x Success, len = 0x%x\n",pMemPlayBufNew,pMemPlayBufNew+u4Size,u4Size);
    Printf( "data.load.binary FILENAME 0x%x--0x%x /ny /swap \n",pMemPlayBufNew,pMemPlayBufNew+u4Size);

    vSetMixSndDumpInfo((UINT32)pMemPlayBufNew, u4Size);
    return 0;
}

extern void vResetMixSndDumpInfo(void);
static INT32 _AudCmdMixSndDumpReset(INT32 i4Argc, const CHAR **szArgv)
{
    vResetMixSndDumpInfo();
    return 0;
}

extern UINT8 u1DumpLocation; // 0: Before SW ASRC 1: After SW ASRC
extern UINT8 u1DumpStreamId;
static INT32 _AudCmdMixSndDumpSet(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 3)
    {
        Printf("Usage: mds [stream ID] [location]\n");
        Printf("  location - 0: before SW SRC, 1: after SW SRC\n");
        Printf("  Current Setting: ID(%d) location(%d)\n", u1DumpStreamId, u1DumpLocation);
    }
    else
    {
        u1DumpStreamId = (UINT8)StrToInt(szArgv[1]);
        u1DumpLocation = (UINT8)StrToInt(szArgv[2]);
        Printf("Set location = %d\n", u1DumpLocation);
    }
    return 0;
}
#endif

#endif

#endif

static INT32 _AudCmdClipPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1DecId, u1DspId, u1ClipType;
    AUD_CLIP_TYPE_T rAudClip;

    if (i4Argc < 6)
    {
        goto lbUsage;
    }

    x_memset((VOID *)VIRTUAL((UINT32)&rAudClip), 0, sizeof(AUD_CLIP_TYPE_T));

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1DecId = (UINT8)StrToInt(szArgv[2]);
    if ((u1DspId >= AUD_DSP_NUM) || (u1DecId >= AUD_DEC_NUM) )
    {
        Printf("id range error \n");
        return -1;
    }

    rAudClip.u4OutputChlSelect = (UINT32)StrToInt(szArgv[3]);
    rAudClip.u4RepeatCount = (UINT32)StrToInt(szArgv[4]);

    u1ClipType = (UINT8)StrToInt(szArgv[5]);
    if (u1ClipType == 0)
    {
        // Play test tone
        rAudClip.fgPlayTestTone = TRUE;
        rAudClip.u4ToneFreq = (UINT32)StrToInt(szArgv[6]);
    }
    else
    {
        if (i4Argc < 9)
        {
            goto lbUsage;
        }

        // Play memory
        rAudClip.fgPlayTestTone = FALSE;
        rAudClip.eMemBuf.pData = (UINT8*)StrToInt(szArgv[6]);
        rAudClip.eMemBuf.u4Length = (UINT32)StrToInt(szArgv[7]);
        rAudClip.eAudFmt = (AUD_FMT_T)StrToInt(szArgv[8]);
    }

    AUD_SetAudClip(u1DspId, u1DecId, &rAudClip);
    AUD_PlayAudClip(u1DspId, u1DecId, rAudClip.u4OutputChlSelect,
                    rAudClip.fgPlayTestTone);
    return 0;

lbUsage:
    Printf("Usage: play [dsp Id] [Dec Id] [Channel] [Loop] [Type] [Frequence]\n");
    Printf("   or  play [dsp Id] [Dec Id] [Channel] [Loop] [Type] [Addr] [Len] [FMT]\n");
     Printf("[dsp Id]     0:DSP0        1:DSP1 \n");
    Printf("[Dec Id]     0:FIRST        1:SECOND \n");
    Printf("[Channel]    1:L, 2:R, 4:LS, 8:RS, 16:C, 32:SUBWOOFER\n");
    Printf("[Loop]       0: Infinite    ~ loop counter\n");
    Printf("[Type]       0:Test tone    1:memory\n");
    Printf("[Frequence]  0 ~ 20K \n");
    Printf("[FMT]        0:OTHERS 1:MPEG   2:AC3   3:PCM    4:MP3   5:AAC \n");
    return -1;
}

static INT32 _AudCmdClipStop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DspId, u1DecId;
        
    if (i4Argc != 3)
    {
        Printf("Usage: stop [dsp Id][decoder id]\n");
        Printf("[dsp Id]     0:DSP0        1:DSP1 \n");
         Printf("[decoder id] 0:MAIN  1:AUX \n");
        return -1;       
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1DecId = (UINT8)StrToInt(szArgv[2]);

    if ((u1DspId >= AUD_DSP_NUM) || (u1DecId >= AUD_DEC_NUM) )
        {
        Printf("id range error \n");
        return -1;
    }

    AUD_StopAudClip(u1DspId, u1DecId);
    
    return 0;
}

static INT32 _AudCmdClipVolume(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1Vol;
    UINT8  u1Channel;
    UINT8  u1DecId;

    if (i4Argc != 3)
    {
        goto lbUsage;
    }
    u1DecId = AUD_DEC_MAIN;

    u1Channel = (UINT8)StrToInt(szArgv[1]);
    u1Vol = (UINT8)StrToInt(szArgv[2]);

    if (u1Vol > 30)
    {
        u1Vol = 30;
    }
    Aud_SetAudClipVolume(AUD_DSP0, u1DecId, u1Channel, u1Vol);
    return 0;
lbUsage:
    Printf("Usage: volume [Channel] [Vol]\n");
    Printf("[Channel]    0:L, 1:R, 2:LS, 3:RS, 4:C, 5:SUBWOOFER\n");
    Printf("[Vol]        0 ~ 30 \n");
    Printf("                    \n");
    Printf("             0: -20 dB \n");
    Printf("             1: -19 dB \n");
    Printf("             ......    \n");
    Printf("            30: +10 dB \n");
    return -1;
}

static INT32 _AudCmdReadBytePhysical(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4PhysicalAddr;
    UINT32 u4Length;
    UINT32 i;

    if (i4Argc == 2)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = 1;
    }
    else if (i4Argc == 3)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = StrToInt(szArgv[2]);

        if (u4Length == 0)
        {
            u4Length = 1;
        }
    }
    else
    {
        goto lbUsage;
    }

    for (i=0;i<u4Length;i++)
    {
        if ((i&0xf) == 0)
        {
            Printf("\n0x%08x: ", u4PhysicalAddr + i);
        }
        Printf("%02x ", *((UINT8*)(VIRTUAL(u4PhysicalAddr + i))));
    }
    Printf("\n");
    return 0;

lbUsage:

    Printf("Usage: r [physical address] [len]\n");
    Printf("  [len] is optional, no specify => 1\n");
    return -1;
}

extern void DSP_FlushInvalidateDCacheFree(UINT32 u4Addr, UINT32 u4Len);
static INT32 _AudCmdFlushDspMemory(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4PhysicalAddr;
    UINT32 u4Length;
    //UINT32 i;

    if (i4Argc == 2)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = 1;
    }
    else if (i4Argc == 3)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = StrToInt(szArgv[2]);

        if (u4Length == 0)
        {
            u4Length = 1;
        }
    }
    else
    {
        goto lbUsage;
    }

    DSP_FlushInvalidateDCacheFree(u4PhysicalAddr, u4Length);
    return 0;

lbUsage:

    Printf("Usage: flush [physical address] [len]\n");
    Printf("  [len] is optional, no specify => 1\n");
    return -1;
}

#ifdef LINUX_TURNKEY_SOLUTION
#ifdef __KERNEL__
static INT32 _AudCmdDumpFifo2File(INT32 i4Argc, const CHAR ** szArgv)
{
    CHAR * fn;
    UINT32 addr;
    UINT32 size;
    INT32 ret;
    mm_segment_t oldfs;
    struct file * filep = NULL;

    UINT8  decId;
    UINT32 u4RpDec;
    UINT32 u4WpDec;
    UINT32 u4DataSZDec;
    UINT32 u4FifoStr;
    UINT32 u4FifoEnd;

    if (i4Argc == 4)
    {
        fn = (CHAR *)szArgv[1];
        addr = (UINT32)StrToInt(szArgv[2]);
        size = (UINT32)StrToInt(szArgv[3]);

        Printf("filename: %s  addr: 0x%x  size: 0x%x\n\n", fn, addr, size);

        oldfs = get_fs();
        set_fs(KERNEL_DS);
        filep = filp_open(fn,(O_CREAT | O_WRONLY | O_APPEND),0);
        if((IS_ERR(filep)) || (NULL == filep))
        {
            set_fs(oldfs);
            Printf("_AudCmdDump() create file failed.\n");
            return -1;
        }
        ret = (INT32)filep->f_op->write(filep, (const char *)addr, (size_t)size, &filep->f_pos);

        filp_close(filep, 0);
        set_fs(oldfs);
    }
    else if(i4Argc == 3)
    {
        fn = (CHAR *)szArgv[1];
        decId = (UINT8)StrToInt(szArgv[2]);

        if(decId > AUD_DEC_4TH)
        {
            goto lbUsage;
        }

        AUD_GetRWPtr(AUD_DSP0, decId, &u4RpDec, &u4WpDec, &u4DataSZDec);
        UNUSED(AUD_GetAudFifo(decId, &u4FifoStr, &u4FifoEnd));

        u4RpDec = VIRTUAL(u4RpDec);
        u4WpDec = VIRTUAL(u4WpDec);
        u4FifoStr = VIRTUAL(u4FifoStr);
        u4FifoEnd = VIRTUAL(u4FifoEnd);

        oldfs = get_fs();
        set_fs(KERNEL_DS);
        filep = filp_open(fn,(O_CREAT | O_WRONLY | O_APPEND),0);
        if((IS_ERR(filep)) || (NULL == filep))
        {
            set_fs(oldfs);
            Printf("_AudCmdDump() create file failed.\n");
            return -1;
        }
        ret = (INT32)filep->f_op->write(filep, (const char *)u4WpDec, (size_t)(u4FifoEnd - u4WpDec), &filep->f_pos);
        ret = (INT32)filep->f_op->write(filep, (const char *)u4FifoStr, (size_t)(u4WpDec - u4FifoStr), &filep->f_pos);

        filp_close(filep, 0);
        set_fs(oldfs);

    }
    else
    {
        goto lbUsage;
    }

    return 1;

lbUsage:
    Printf("Usage1: dump [filename] [startaddr] [size]\n");
    Printf("[filename]  ex:\"/tmp/AudioTrack.bin\"\n");
    Printf("[startaddr] Virtual Addr\n");
    Printf("[size]      data Size\n\n");

    Printf("Usage2: dump [filename] [decId]\n");
    Printf("[filename]  ex:\"/tmp/AudDecFifo.bin\"\n");
    Printf("[decId] 0: AUD_DEC_MAIN  1: AUD_DEC_AUX  2: AUD_DEC_THIRD  3: AUD_DEC_4TH\n");

    return -1;
}

static INT32 _AudCmdUlFile2Dram(INT32 i4Argc, const CHAR ** szArgv)
{
    CHAR * fn = NULL;
    UINT32 addr = 0;
    UINT32 size = 0;
    INT32 ret;
    mm_segment_t oldfs;
    struct file * filep = NULL;

    if(i4Argc == 3)
    {
        fn = (CHAR *)szArgv[1];
        addr = (UINT32)StrToInt(szArgv[2]);

        oldfs = get_fs();
        set_fs(KERNEL_DS);
        filep = filp_open(fn,(O_RDONLY),0);
        if((IS_ERR(filep)) || (NULL == filep))
        {
            set_fs(oldfs);
            Printf("_AudCmdUlFile2Dram() open file failed.\n");
            return -1;
        }
        while ((ret = (INT32)filep->f_op->read(filep, (char *)(addr + size), 4096, &filep->f_pos)) != 0)
        {
            size += ret;
        }
        Printf("_AudCmdUlFile2Dram() from file %s to Dram Addr: 0x%08x Size: %x\n", fn, addr, size);

        filp_close(filep, 0);
        set_fs(oldfs);
    }
    else
    {
        goto lbUsage;
    }

    return 1;

lbUsage:
    Printf("ul [filename] [startaddr]\n");
    Printf("  [filename]  ex:\"/tmp/AudioTrack.bin\"\n");
    Printf("  [startaddr] Virtual Addr\n");

    return -1;
}

static INT32 _AudCmdDramAlloc(INT32 i4Argc, const CHAR ** szArgv)
{
    static UINT32 u4MalBufAddr = 0;
    static UINT32 u4MalBufSize = 0;

    if(i4Argc == 2)
    {
        u4MalBufSize = (UINT32)StrToInt(szArgv[1]);
        if(u4MalBufAddr)
        {
            x_mem_free((VOID*)u4MalBufAddr);
            Printf( "Free memory 0x%x size: 0x%x\n",u4MalBufAddr, u4MalBufSize);
            u4MalBufAddr = 0;
        }

        if (u4MalBufSize)
        {
            u4MalBufAddr = (UINT32)x_mem_alloc_virtual(u4MalBufSize);
            if (u4MalBufAddr == 0)
            {
                printf( "Allocate memory size: 0x%x fail!!\n", u4MalBufSize);
                return -1;
            }
            else
            {
                Printf( "Allocate memory addr: 0x%x size: 0x%x for dram upload ...\n",u4MalBufAddr, u4MalBufSize);
            }
        }
    }
    else
    {
        goto lbUsage;
    }

    return 1;

lbUsage:
    Printf("# Memory Allocate Virtual Addr Continuous Dram\n");
    Printf("mal [u4MalBufSize]\n");
    Printf("  u4MalBufSize = 0 for free\n");
    Printf("  Current setting: u4MalBufAddr: 0x%x u4MalBufSize: 0x%x\n", u4MalBufAddr, u4MalBufSize);
    return -1;
}
#endif
#endif

#ifndef __KERNEL__
static INT32 _AudCmdDumpFifo2File_Nucle(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 ret;

    UINT8  decId;
    UINT32 u4RpDec;
    UINT32 u4WpDec;
    UINT32 u4DataSZDec;
    UINT32 u4FifoStr;
    UINT32 u4FifoEnd;
 
    HANDLE_T    h_file;
    //HANDLE_T    h_dir;
    UINT32      u4WriteByte;
    UINT32      u4Addr;
    UINT32      u4Size;
    const char*       pcFileName;
    char szFileNameWithPath[96];

    //if (x_fm_open_dir(FM_ROOT_HANDLE, USB_PATH, &h_dir) != FMR_OK)
    //{
    //    Printf("Open USB root directory fail, ");
    //    return 1;
    //}

    x_strcpy(szFileNameWithPath, USB_PATH);
    x_strcat(szFileNameWithPath, "/");

    if (i4Argc == 4)
    {
        pcFileName = (CHAR *)szArgv[1];        
        x_strcat(szFileNameWithPath, pcFileName);
        u4Addr = (UINT32)StrToInt(szArgv[2]);
        u4Size = (UINT32)StrToInt(szArgv[3]);

        Printf("filename: %s  addr: 0x%x  \n\n", szFileNameWithPath, u4Addr, u4Size);
        ret = x_fm_open(FM_ROOT_HANDLE, szFileNameWithPath, (FM_READ_WRITE | FM_OPEN_CREATE), 0777, FALSE, &h_file);
        if(FMR_OK != ret)
        {
            Printf("open file error\n");
            return -1;
        }
        ret = x_fm_write(h_file, (VOID*)u4Addr, u4Size, &u4WriteByte);
        if(FMR_OK != ret)
        {
            Printf("write file not ok\n");
            Printf("write file u4Addr: 0x%x\n",u4Addr);
            Printf("write file u4Size: 0x%x\n",u4Size);
            Printf("write file u4WriteByte: 0x%x\n",u4WriteByte);
            x_fm_close(h_file);
            return -1;
        }
        if(u4Size != u4WriteByte)
        {
            Printf("write file error\n");
            Printf("write file u4Addr: 0x%x\n",u4Addr);
            Printf("write file u4Size: 0x%x\n",u4Size);
            Printf("write file u4WriteByte: 0x%x\n",u4WriteByte);
            x_fm_close(h_file);
            return -1;
        }
        x_fm_close(h_file);

    }
    else if(i4Argc == 3)
    {
        pcFileName = (CHAR *)szArgv[1];
        x_strcat(szFileNameWithPath, pcFileName);
        decId = (UINT8)StrToInt(szArgv[2]);

        if(decId > AUD_DEC_4TH)
        {
            goto lbUsage;
        }
        
        Printf("filename: %s  decId: 0x%x\n\n", szFileNameWithPath, decId);
        ret = x_fm_open(FM_ROOT_HANDLE, szFileNameWithPath, (FM_READ_WRITE | FM_OPEN_CREATE), 0777, FALSE, &h_file);
        if(FMR_OK != ret)
        {
            Printf("open file error\n");
            return -1;
        }
        AUD_GetRWPtr(AUD_DSP0, decId, &u4RpDec, &u4WpDec, &u4DataSZDec);
        UNUSED(AUD_GetAudFifo(decId, &u4FifoStr, &u4FifoEnd));
        u4RpDec = VIRTUAL(u4RpDec);
        u4WpDec = VIRTUAL(u4WpDec);
        u4FifoStr = VIRTUAL(u4FifoStr);
        u4FifoEnd = VIRTUAL(u4FifoEnd);  
        u4Size = u4FifoEnd - u4WpDec;
        ret = x_fm_write(h_file, (VOID*)u4WpDec, (u4FifoEnd - u4WpDec), &u4WriteByte);
        if(FMR_OK != ret)
        {
            Printf("write file not ok\n");
            Printf("write file u4Size: 0x%x\n",u4Size);
            Printf("write file u4WriteByte: 0x%x\n",u4WriteByte);
            x_fm_close(h_file);
            return -1;
        }
        if(u4Size != u4WriteByte)
        {
            Printf("write file error\n");
            Printf("write file u4Size: 0x%x\n",u4Size);
            Printf("write file u4WriteByte: 0x%x\n",u4WriteByte);
            x_fm_close(h_file);
            return -1;
        }
        u4Size = u4WpDec - u4FifoStr;
        ret = x_fm_write(h_file, (VOID*)u4FifoStr, (u4WpDec - u4FifoStr), &u4WriteByte);
        if(FMR_OK != ret)
        {
            Printf("write file not ok1\n");  
            Printf("write file u4Size: 0x%x\n",u4Size);
            Printf("write file u4WriteByte: 0x%x\n",u4WriteByte);
            x_fm_close(h_file);
            return -1;
        }
        if(u4Size != u4WriteByte)
        {
            Printf("write file error\n");
            Printf("write file u4Size: 0x%x\n",u4Size);
            Printf("write file u4WriteByte: 0x%x\n",u4WriteByte);
            x_fm_close(h_file);
            return -1;
        }
        
        x_fm_close(h_file);


    }
    else
    {
        goto lbUsage;
    }

    return 1;

lbUsage:
    Printf("Usage1: dump [filename] [startaddr] [size]\n");
    Printf("[filename]  ex:fifo.bin\n");
    Printf("[startaddr] Addr\n");
    Printf("[size]      data Size\n\n");

    Printf("Usage2: dump [filename] [decId]\n");
    Printf("[filename]  ex:fifo.bin\n");
    Printf("[decId] 0: AUD_DEC_MAIN  1: AUD_DEC_AUX  2: AUD_DEC_THIRD  3: AUD_DEC_4TH\n");

    return -1;
}


#endif


#ifdef LINUX_TURNKEY_SOLUTION
extern int StartAudDataDump(UINT32 u1DecId, char* fileName);
extern void StopAudDataDump(UINT32 u1DecId);
extern void AudDataDumpQueryInfo(void);
extern BOOL GetAudDataDumpAutoSave(UINT32 u1DecId);
extern void SetAudDataDumpAutoSave(UINT32 u1DecId, BOOL bAutoSave);

static INT32 _AudCmdDumpFifo2FileInfinit(INT32 i4Argc, const CHAR ** szArgv)
{
    CHAR * fileName;
    UINT8  u1DecId;

    if (i4Argc == 3)
    {
        u1DecId = (UINT32)StrToInt(szArgv[1]);
        fileName = (CHAR *)szArgv[2];
        StartAudDataDump(u1DecId, fileName);
    }
    else if(i4Argc == 2)
    {
        if (szArgv[1][0] == 'q')
        {
            AudDataDumpQueryInfo();
        }
        else
        {
            u1DecId = (UINT32)StrToInt(szArgv[1]);
            StartAudDataDump(u1DecId, "/mnt/usb/sda1/AudDecFifo.bin");
        }
    }
    else
    {
        goto lbUsage;
    }

    return 1;

lbUsage:
    Printf("Usage1: dumpLoop [decId] [filename]\n");
    Printf("[decId] 0: AUD_DEC_MAIN  1: AUD_DEC_AUX  2: AUD_DEC_THIRD  3: AUD_DEC_4TH\n");
    Printf("[filename]  ex:\"/mnt/usb/sda1/AudDecFifo.bin\"\n\n");

    Printf("Usage2: dumpLoop [decId]\n");
    Printf("[decId] 0: AUD_DEC_MAIN  1: AUD_DEC_AUX  2: AUD_DEC_THIRD  3: AUD_DEC_4TH\n");
    Printf("Using default [fileName]=/mnt/usb/sda1/AudDecFifo.bin\n\n");

    Printf("Usage3: dumpLoop q\n");

    return -1;
}

static INT32 _AudCmdDumpFifo2FileInfinitStop(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8  u1DecId;

    if (i4Argc == 2)
    {
        u1DecId = (UINT32)StrToInt(szArgv[1]);
        StopAudDataDump(u1DecId);
    }
    else
    {
        goto lbUsage;
    }
    return 1;

lbUsage:
    Printf("Usage1: dumpStop [decId]\n");
    Printf("[decId] 0: AUD_DEC_MAIN  1: AUD_DEC_AUX  2: AUD_DEC_THIRD  3: AUD_DEC_4TH\n");
    return -1;
}


static INT32 _AudCmdDumpFifo2FileAutoSave(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DecId;
    UINT8 i;
    BOOL bAutoSave;

    if (i4Argc == 2)
    {
        if (szArgv[1][0] == 'q')
        {
            Printf("Current dump auto save settings:\n");
            for (i = 0; i < AUD_DEC_NUM; i++)
            {
                Printf(" #DEC%d: %s\n", i, (GetAudDataDumpAutoSave(i) ? "true" : "false"));
            }
            Printf("\n");
            return 0;
        }
    }
    else if (i4Argc == 3)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
        bAutoSave = (BOOL)StrToInt(szArgv[2]);
        SetAudDataDumpAutoSave(u1DecId, bAutoSave);
        return 0;
    }

    Printf("Usage: dumpAS [decId] [autoSave] or dumpAS q\n");
    Printf("[decId] 0: AUD_DEC_MAIN  1: AUD_DEC_AUX  2: AUD_DEC_THIRD  3: AUD_DEC_4TH\n");
    Printf("[autoSave] 1: true  0: false\n");
    return -1;
}
#endif

static INT32 _AudCmdReadDwrdPhysical(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4PhysicalAddr;
    UINT32 u4Length;
    UINT32 i;

    if (i4Argc == 2)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = 4;

        if ((u4PhysicalAddr&0x3) != 0)
        {
            goto lbUsage;
        }
    }
    else if (i4Argc == 3)
    {
        u4PhysicalAddr = StrToInt(szArgv[1]);
        u4Length = StrToInt(szArgv[2]);

        if ((u4PhysicalAddr&0x3) != 0)
        {
            goto lbUsage;
        }

        if (u4Length < 4)
        {
            u4Length = 4;
        }
    }
    else
    {
        goto lbUsage;
    }

    DSP_FlushInvalidateDCacheFree(u4PhysicalAddr, u4Length);
    for (i=0;i<u4Length;i+=4)
    {
        if ((i&0xf) == 0)
        {
            Printf("\n0x%08x: ", u4PhysicalAddr + i);
        }
        Printf("%08x ", *((UINT32*)(VIRTUAL(u4PhysicalAddr + i))));
    }
    Printf("\n");
    return 0;

lbUsage:

    Printf("Usage: r4 [physical address] [len]\n");
    Printf("  [physical address] must be 4 bytes aligned\n");
    Printf("  [len] is optional, no specify => 4\n");
    return -1;
}

static INT32 _AudCmdGetPhyAddr(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VirAddr;
    UINT32 u4PhyAddr;

    if (i4Argc != 2)
    {
        Printf("Usage: phy [vir addr]\n");
        return -1;
    }

    u4VirAddr = StrToInt(szArgv[1]);
    u4PhyAddr = PHYSICAL(u4VirAddr);

    Printf("0x%08x(V) ==> 0x%08x(P)\n", u4VirAddr, u4PhyAddr);
    return 0;
}

static INT32 _AudCmdGetVirAddr(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4VirAddr;
    UINT32 u4PhyAddr;

    if (i4Argc != 2)
    {
        Printf("Usage: vir [phy addr]\n");
        return -1;
    }

    u4PhyAddr = StrToInt(szArgv[1]);
    u4VirAddr = VIRTUAL(u4PhyAddr);

    Printf("0x%08x(P) ==> 0x%08x(V)\n", u4PhyAddr, u4VirAddr);
    return 0;
}

#if defined(__linux__)
static INT32 _AudCmdIsDecoderPlay(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Id, u1DspId;
    if (i4Argc != 3)
    {
        return -1;
    }
    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u4Id = StrToInt(szArgv[2]);
    if(u4Id >= 2)
    {
        return -1;
    }
    if(u1DspId >= 2)
    {
        return -1;
    }
    return (INT32)AUD_IsDecoderPlay(u1DspId, u4Id);
}
static INT32 _AudCmdUopBalance(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Id, u4Val;
    if (i4Argc != 3)
    {
        return -1;
    }
    u4Id = StrToInt(szArgv[1]);
    if(u4Id >= 2)
    {
        return -1;
    }
    u4Val = StrToInt(szArgv[2]);
    AUD_DspBalance(u4Id, u4Val);
    return 0;
}
#endif

#if 1 // PROBE_RELATED_API
static INT32 _AudCmdProbePause(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;

    if (i4Argc != 2)
    {
        Printf("Usage: Pause [Dsp Id]\n");
        Printf("[Dsp Id]     0:ADSP        1:DDSP \n");
        return -1;       
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);

    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
        return -1;
    }
    
    Printf("DSP(%d) Probe Pause\n", u1DspId);
    AUD_DspProbePause(u1DspId);
    return 0;
}

static INT32 _AudCmdProbeStep(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;
    UINT32 u4Count;
    if (i4Argc != 3)
    {
        Printf("Usage: Step [Dsp Id][#]\n");
        Printf("[Dsp Id]     0:DSP0        1:DSP1 \n");
        Printf("#: step number 1~255\n");
        return -1;
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u4Count = StrToInt(szArgv[2]);

    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
        return -1;
    }
        
    if(u4Count >= 255)
    {
        Printf("Usage: Step [Dsp Id][#]\n");
        Printf("[Dsp Id]     0:DSP0        1:DSP1 \n");
        Printf("#: step number 1~255\n");
        return -1;
    }
    
    Printf("DspId = %d, Probe Step %d\n", u1DspId, u4Count);
    AUD_DspProbeStep(u1DspId, u4Count);
    return 0;
}

static INT32 _AudCmdProbeGo(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;

    if (i4Argc != 2)
    {
        Printf("Usage: go [Dsp Id]\n");
        Printf("[Dsp Id]     0:DSP0        1:DSP1 \n");
        return -1;       
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
        return -1;
    }
    Printf("DSP(%d) Probe Go\n", u1DspId);
    AUD_DspProbeGo(u1DspId);
    return 0;
}

static INT32 _AudCmdProbeWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;
    UINT32 u4Addr,u4Data;
    UINT8 fgPreviousPause; //ZY_FPGA

    if (i4Argc != 4)
    {
        Printf("Usage: Write [Dsp Id][Addr] [Data]\n");
        Printf("[Dsp Id]     0:DSP0        1:DSP1 \n");
        Printf("Addr: SRAM address for Write\n");
        Printf("Data: SRAM data for Write\n");
        return -1;
    }
    u1DspId = (UINT8)StrToInt(szArgv[1]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
        return -1;
    }    

#if 1
    //ZY_FPGA
    fgPreviousPause = fgAUD_DspProbeHalt(u1DspId);
    if (!fgPreviousPause)
    {
        AUD_DspProbePause(u1DspId);
    }
    //ZY_FPGA
#else
    if (!fgAUD_DspProbeHalt())
    {
        Printf("DSP is not in halt state\n");
        Printf("Please execute [Pause] in advance\n");
        return -1;
    }
#endif

    u4Addr = StrToInt(szArgv[2]);
    u4Data = StrToInt(szArgv[3]);
    AUD_DspProbeWrite(u1DspId, u4Addr,u4Data);

    //ZY_FPGA
    if (!fgPreviousPause)
    {
        AUD_DspProbeGo(u1DspId);
    }
    //ZY_FPGA

    return 0;
}
#ifdef DSP_SUPPORT_SRSWOW
static INT32 _AudCmdUopSRSWOW(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: srswow  [mode]\n");
        Printf("[mode] 0: off 1: on\n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSWOW(u1Mode);
    return 1;
}
#endif
static INT32 _AudCmdProbeRead(INT32 i4Argc, const CHAR **szArgv)
{
#if 0
    UINT32 u4Id, u4Val;
    if (i4Argc != 3)
    {
        return -1;
    }
    u4Id = StrToInt(szArgv[1]);
    if(u4Id >= 2)
    {
        return -1;
    }
    u4Val = StrToInt(szArgv[2]);
#endif
    UINT8 u1DspId;
    UINT32 u4Addr;
    UINT16  u2Length;
    UINT16  u2Idx = 0;
    UINT8 fgPreviousPause; //ZY_FPGA

    if ((i4Argc != 3)&&(i4Argc != 4))
    {
        Printf("Usage: Read [Dsp Id] [Addr] ([Length]) \n");
        Printf("Addr: SRAM address for read\n");
        return -1;
    }

    u1DspId = StrToInt(szArgv[1]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
        return -1;
    }

    #if 1
    //ZY_FPGA
    fgPreviousPause = fgAUD_DspProbeHalt(u1DspId);
    if (!fgPreviousPause)
    {
        AUD_DspProbePause(u1DspId);
    }
    //ZY_FPGA
    #else
    if (!fgAUD_DspProbeHalt())
    {
        Printf("DSP is not in halt state\n");
        Printf("Please execute [Pause] in advance\n");
        return -1;
    }
    #endif

    u4Addr = StrToInt(szArgv[2]);
    if (i4Argc == 3)
    {
        Printf("0x%04x 0x%08x\n",u4Addr,u4AUD_DspProbeRead(u1DspId, u4Addr));
    }
    else if (i4Argc == 4)
    {
        u2Length = StrToInt(szArgv[3]);
        if (u2Length > 0x100)
        {
            u2Length = 0x100;
        }
        Printf("Probe Address: 0x%x, length = %x \n",u4Addr,u2Length);
        for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
        {
            if ((u2Idx % 4) == 0)
            {
                Printf("\n0x%04x | ",u4Addr+u2Idx);
            }
            Printf("%08x ",u4AUD_DspProbeRead(u1DspId, u4Addr+u2Idx));
        }
        Printf("\n");
    }

    //ZY_FPGA
    if (!fgPreviousPause)
    {
        AUD_DspProbeGo(u1DspId);
    }
    //ZY_FPGA

    return 0;
}

//ZY_FPGA
static INT32 _AudCmdProbeSramTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;
    UINT32 u4Addr;
    UINT16 u2Length;
    UINT16 u2Idx = 0;
    UINT8 u1PreviousHalt;
    UINT32 u4Val;

    if (i4Argc != 4)
    {
        Printf("Usage: sramtest [Dsp Id] [Addr] [Length]\n");
        return -1;
    }

    u1DspId = StrToInt(szArgv[1]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
    }  

    u1PreviousHalt = fgAUD_DspProbeHalt(u1DspId);
    if (!u1PreviousHalt)
    {
        AUD_DspProbePause(u1DspId);
    }

    u4Addr = StrToInt(szArgv[2]);
    u2Length = StrToInt(szArgv[3]);

    Printf("Sram Test - 0x%04x ~ 0x%04x\n", u4Addr, u4Addr + u2Length - 1);
    for (u2Idx=0;u2Idx < u2Length;u2Idx++)
    {
        AUD_DspProbeWrite(u1DspId, u4Addr + u2Idx, 0xa5a5a5a5);
    }

    for (u2Idx=0;u2Idx < u2Length;u2Idx++)
    {
        u4Val = u4AUD_DspProbeRead(u1DspId, u4Addr + u2Idx);
        if (u4Val != 0xa5a5a5a5)
        {
            Printf("Error addr: 0x%04x - 0x%08x<=>0xa5a5a5a5\n", u4Addr + u2Idx, u4Val);
        }
    }

    for (u2Idx=0;u2Idx < u2Length;u2Idx++)
    {
        AUD_DspProbeWrite(u1DspId, u4Addr + u2Idx, 0x5a5a5a5a);
    }

    for (u2Idx=0;u2Idx < u2Length;u2Idx++)
    {
        u4Val = u4AUD_DspProbeRead(u1DspId, u4Addr + u2Idx);
        if (u4Val != 0x5a5a5a5a)
        {
            Printf("Error addr: 0x%04x - 0x%08x<=>0x5a5a5a5a\n", u4Addr + u2Idx, u4Val);
        }
    }

    if (!u1PreviousHalt)
    {
        AUD_DspProbeGo(u1DspId);
    }

    return 0;
}

extern void vResetDSP(UINT8 u1DspId);
static INT32 _AudCmdProbeReset(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;
    if (i4Argc != 2)
    {
        Printf("Usage: reset [Dsp Id] \n");
        return -1;
    }
    u1DspId = StrToInt(szArgv[1]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
    }
    Printf("Reset DSP(%d)\n", u1DspId);
    vResetDSP(u1DspId);
    return 0;
}
extern void AUD_QueryDspMemory(UINT8 u1MemSpace);
static INT32 _AudCmdQueryDspMemory(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {
        Printf("qm [space id]\n");
        Printf("  0 - dram map\n"
               "  1 - dsp code\n"
               "  2 - normal\n"
               "  3 - compact\n"
               "  4 - afifo\n"
               "  5 - intbuf\n"
               "  6 - asrc\n"
               "  7 - aout\n"
               "  8 - upload\n"
               "  9 - mixsnd\n"            
               );
        return -1;
    }
    AUD_QueryDspMemory((UINT8)StrToInt(szArgv[1]));
    return 0;
}

extern void AUD_DumpDspMemory(UINT8 u1Option, UINT32 u4Size);
static INT32 _AudCmdDumpDspMemory(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Option;
    UINT32 u4Size;
    if ((i4Argc != 2) && (i4Argc != 3))
    {
        Printf("dm [option] [size]\n");
        Printf("  0 - afifo1\n"
               "  1 - afifo2\n"
               "  2 - afifo3\n"
               "  3 - afifo4\n"
               "  4 - upload\n"
               "  5 - alsa buffer\n"
               "  6 - mixclip buffer (alsa)\n"
               );
        return -1;
    }

    u1Option = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc == 3)
    {
        u4Size = (UINT32)StrToInt(szArgv[2]);
    }
    else
    {
        u4Size = 0x100;
    }

    AUD_DumpDspMemory(u1Option, u4Size);
    return 0;
}

#ifndef CC_AUD_DISABLE_DSP_PAT
extern void AUD_DspPatternTest(UINT8 u1DspId, UINT8 u1PatId,UINT8 u1ShowHelp);
static INT32 _AudCmdPatTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;
    UINT8 u1PatId;
    if (i4Argc != 3)
    {
        AUD_DspPatternTest(AUD_DSP0,0,1);
        return -1;
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1PatId = (UINT8)StrToInt(szArgv[2]);
    AUD_DspPatternTest(u1DspId, u1PatId,1);
    return 0;
}

extern void AUD_DspPatternAutoTest(UINT8 u1DspId);
static INT32 _AudCmdPatAuto(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;

    if (i4Argc != 2)
    {
        Printf("Usage: patauto [Dsp Id]\n");
        Printf("[Dsp Id]     0:DSP0        1:DSP1 \n");
        return -1;       
    }
    
    u1DspId = (UINT8)StrToInt(szArgv[1]); 
    AUD_DspPatternAutoTest(u1DspId);
    return 0;
}

extern void AUD_DspPatternSingle(UINT8 u1DspId, UINT8 u1PatId); 
static INT32 _AudCmdPatSingle(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;
    UINT8 u1PatId;

    if (i4Argc != 3)
    {
        Printf("Usage: pats [Dsp Id] [Pat ID]\n");
        Printf("[Dsp Id]     0:DSP0        1:DSP1 \n");
        return -1;       
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    u1PatId = (UINT8)StrToInt(szArgv[2]);
    
    AUD_DspPatternSingle(u1DspId, u1PatId);
    return 0;
}

#if 0
extern void AUD_FillAFIFO1Buffer(UINT8 u1Case,UINT32 u4Interval);
static INT32 _AudCmdFillBuf(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Case;
    UINT32 u4Interval;
    if (i4Argc != 3)
    {
        Printf("fillbuf [case] [interval]\n");
        Printf("  0: CH1~10 = 1K,2K,3K,4K,6K\n");
        Printf("  1: CH1=11h, CH2=22h, other=00h\n");
        Printf("  2: AC3 Raw (HBR=0,long-det=0,ch=2)\n");
        Printf("  4: E-AC3 Raw (HBR=1,long-det=1,ch=8)\n");
        Printf("  5: DTS-16 (HBR=0,long-det=0,ch=2)\n");
        Printf("  6: TTS-14 (HBR=0,long-det=0,ch=2)\n");
        return -1;
    }

    u1Case = (UINT8)StrToInt(szArgv[1]);
    u4Interval = (UINT32)StrToInt(szArgv[2]);

    AUD_FillAFIFO1Buffer(u1Case,u4Interval);
    return 0;
}
#endif

#endif
//ZY_FPGA


#if 1 //CC_APE_SUPPORT
extern void AUD_DspAPEMuteBankNum(UINT8 u1DecId, UINT16 u2MuteBankNum);
static INT32 _AudCmdapeinit(INT32 i4Argc, const CHAR **szArgv)
{
    AUD_APE_CFG_T a;
    AUD_DspAPEHeader(0,&a);
    return 0;
}
static INT32 _AudCmdapeparser(INT32 i4Argc, const CHAR **szArgv)
{
    AUD_APE_CFG_T apeheaderinfo;

    if (i4Argc != 12)
    {
        Printf("apeps [Parseflag] [FileVersion] [CompressLevel] [Samplerate] [Ch num] [BitWidth] [BlockPerFrm] [FinalFrmBlock] [TotalFrmNum] [MuteBankNum] [InvalidBytes]\n");
        Printf("  Parseflag        : 0 - no need dsp parse;   1- need dsp parse\n");
        Printf("  FileVersion      : 3970/3980/3990\n");
        Printf("  CompressLevel: 1000/2000/3000/4000\n");
        Printf("  Samplerate     : 8000~192000\n");
        Printf("  Ch num          : 1 or 2\n");
        Printf("  BitWidth         : 8/16/24\n");
        Printf("  BlockPerFrm    : \n");
        Printf("  FinalFrmBlock  : \n");
        Printf("  TotalFrmNum   : \n");
        Printf("  MuteBankNum   : \n");
        Printf("  InvalidBytes   : \n");
        return -1;
    }

    apeheaderinfo.u2ParseAudOnly          =       (UINT16)StrToInt(szArgv[1]);
    apeheaderinfo.u2FileVersion              =       (UINT16)StrToInt(szArgv[2]);
    apeheaderinfo.u4CompressLevel        =       (UINT32)StrToInt(szArgv[3]);
    apeheaderinfo.u4BlockPerFrm            =       (UINT32)StrToInt(szArgv[7]);
    apeheaderinfo.u4FinalFrmBlock          =       (UINT32)StrToInt(szArgv[8]);
    apeheaderinfo.u2TotalFrmNumLow     =       (UINT16)(StrToInt(szArgv[9]) & 0xFFFF);
    apeheaderinfo.u2TotalFrmNumHigh    =       (UINT16)(StrToInt(szArgv[9]) >> 16);
    apeheaderinfo.u2Bpersample             =       (UINT16)StrToInt(szArgv[6]);
    apeheaderinfo.u2CHNum                   =       (UINT16)StrToInt(szArgv[5]);
    apeheaderinfo.u4SamplingRate          =       (UINT32)StrToInt(szArgv[4]);
    apeheaderinfo.u2MuteBankNum          =      (UINT16)StrToInt(szArgv[10]);
    apeheaderinfo.u2InvalidBytes            =       (UINT16)StrToInt(szArgv[11]);

    Printf(" ------set ape header info: ---------------------\n");
    Printf("    !!!! header info must match with 010Editor!!!\n");
    Printf("  Parseflag           : %d\n", apeheaderinfo.u2ParseAudOnly);
    Printf("  FileVersion         : %d\n", apeheaderinfo.u2FileVersion);
    Printf("  CompressLevel   : %d\n", apeheaderinfo.u4CompressLevel);
    Printf("  Samplerate        : %d\n", apeheaderinfo.u4SamplingRate);
    Printf("  Ch num             : %d\n", apeheaderinfo.u2CHNum);
    Printf("  BitWidth            : %d\n", apeheaderinfo.u2Bpersample);
    Printf("  BlockPerFrm       : %d\n", apeheaderinfo.u4BlockPerFrm);
    Printf("  FinalFrmBlock     : %d\n", apeheaderinfo.u4FinalFrmBlock);
    Printf("  TotalFrmNum      : %d\n", (apeheaderinfo.u2TotalFrmNumHigh << 16) + apeheaderinfo.u2TotalFrmNumLow);
    Printf("  MuteBankNum    : %d\n", apeheaderinfo.u2MuteBankNum);
    Printf("  InvalidBytes       : %d\n", apeheaderinfo.u2InvalidBytes);

    AUD_DspAPEHeader(0,&apeheaderinfo);

    return 0;
}
#endif

static INT32 _AudProbeShowRegs(UINT8 u1DspId)
{

    UINT32 i;
    UINT32 u4temp1,u4temp2;
    
    if (!fgAUD_DspProbeHalt(u1DspId))
    {
        Printf("DSP is not in halt state\n");
        Printf("Please execute [Pause] in advance\n");
        return -1;
    }

    u4temp1= u4AUD_DspProbeRead(u1DspId, 0x80)>>16;
    u4temp2= u4AUD_DspProbeRead(u1DspId, 0x81)>>16;
    Printf("REG1: %08x %08x REG2: %08x %08x REG3: %08x REG4: %04x\n",
           u4AUD_DspProbeRead(u1DspId, 0x89),u4AUD_DspProbeRead(u1DspId, 0x88), // Accumulator
           u4AUD_DspProbeRead(u1DspId, 0x8d),u4AUD_DspProbeRead(u1DspId, 0x8c), // Product
           (u4AUD_DspProbeRead(u1DspId, 0x0c)),(u4AUD_DspProbeRead(u1DspId, 0x0d)>>16) // 
           );
    Printf("REG5: %08x %08x REG6: %04x REG7: R%x REG8: %x REG9: %x REG10: %x REG11: %04x\n",
           u4AUD_DspProbeRead(u1DspId, 0x8b),u4AUD_DspProbeRead(u1DspId, 0x8a), // 
           (u4AUD_DspProbeRead(u1DspId, 0x0e)>>16), // 
           ((u4temp1>>13)&0x7), ((u4temp1>>12) & 0x1), //
           ((u4temp1>>11) &0x1),((u4temp1>>9) & 0x1), //
           (u4temp1 & 0x1ff) //
           );
    Printf("REG12: %04x REG13: %04x REG14: %x REG15: %x REG16: %x REG17: %x REG18(shftd): %06x\n",
           u4temp1,u4temp2, //
           ((u4temp2>>11) & 0x1),((u4temp2)>>10 & 0x1), //
           ((u4temp2>>9) & 0x1), ((u4temp2) & 0x3), //
           ((u4temp1 & 0x1ff)<<7)
           );
    for(i=0;i<8;i++)
    {
        if(i==4) Printf("\n");
        Printf(" REG%d: %04x ",(i+19), (u4AUD_DspProbeRead(u1DspId, i+0x10)>>16));
    }
    Printf("\n");
    for(i=0;i<8;i++)
    {
        if(i==4) Printf("\n");
        Printf(" REG%d: %08x ",(i+27), u4AUD_DspProbeRead(u1DspId, i+0x60));
    }
    Printf("\n");
    Printf("REG35: %04x REG36: %04x REG37: %04x REG38: %04x REG39: %04x REG40: %04x\n",
            (u4AUD_DspProbeRead(u1DspId, 0x4)>>16),(u4AUD_DspProbeRead(u1DspId, 0x6)>>16),// 
            (u4AUD_DspProbeRead(u1DspId, 0x7)>>16),(u4AUD_DspProbeRead(u1DspId, 0x8)>>16),// 
            (u4AUD_DspProbeRead(u1DspId, 0x9)>>16),(u4AUD_DspProbeRead(u1DspId, 0xa)>>16)// 
           );
    Printf("REG41: %04x REG42: %04x REG43: %04x REG44: %04x REG45: %04x REG46: %04x\n",
            (u4AUD_DspProbeRead(u1DspId, 0xb)>>16),(u4AUD_DspProbeRead(u1DspId, 0xf)>>16),// 
            (u4AUD_DspProbeRead(u1DspId, 0x18)>>16),(u4AUD_DspProbeRead(u1DspId, 0x19)>>16),// 
            (u4AUD_DspProbeRead(u1DspId, 0x1a)>>16),(u4AUD_DspProbeRead(u1DspId, 0x1b)>>16)// 
           );
    Printf("REG47: %04x REG48: %04x REG49: %04x REG50: %04x REG51: %04x REG52: %04x\n",
            (u4AUD_DspProbeRead(u1DspId, 0x1c)>>16),(u4AUD_DspProbeRead(u1DspId, 0x1d)>>16),// 
            (u4AUD_DspProbeRead(u1DspId, 0x1e)>>16),(u4AUD_DspProbeRead(u1DspId, 0x1f)>>16),// 
            (u4AUD_DspProbeRead(u1DspId, 0x23)>>16),(u4AUD_DspProbeRead(u1DspId, 0x24)>>16)// 
           );
    Printf("REG53: %04x REG54: %04x REG55: %04x REG56: %04x REG57: %04x REG58: %04x\n",
            (u4AUD_DspProbeRead(u1DspId, 0x23)>>16),(u4AUD_DspProbeRead(u1DspId, 0x24)>>16),// 
            (u4AUD_DspProbeRead(u1DspId, 0x25)>>16),(u4AUD_DspProbeRead(u1DspId, 0x26)>>16),// 
            (u4AUD_DspProbeRead(u1DspId, 0x27)>>16),(u4AUD_DspProbeRead(u1DspId, 0x62)>>16)// 
           );
    Printf("PC: %04x REG59: %04x REG60: %08x REG61: %08x\n",
            u4AUD_DspProbeRead(u1DspId, 0xa0),u4AUD_DspProbeRead(u1DspId, 0xa2),// PC
            u4AUD_DspProbeRead(u1DspId, 0xa8),u4AUD_DspProbeRead(u1DspId, 0xa9)//
           );

    Printf("REG(3Eh): %08x\n", u4AUD_DspProbeRead(u1DspId, 0x3e));
    Printf("REG(6Dh): %08x\n", u4AUD_DspProbeRead(u1DspId, 0x6d));

    {
        UINT32 MONI_DSP_2 = u4AUD_DspProbeRead(u1DspId, 0xac);
        Printf("SXX1 - [15:13]XXX [12]XX [11]XXX [9]XXXX [8:0]XX\n");
        Printf("SXX2 - [15:13]XXX [12]XX [11]XX [10]XXX [9]X [6]XX [4]XX [1:0]XM\n");
        Printf("MONI_DSP_2: %04x\n", MONI_DSP_2);
        Printf("  [15] ic_rd: %d [14] undef_ins_d: %d\n"
            "  [13] wdcmpt_cmd&wdcmpt_full: %d [12] wdram_cmd&wdram_full: %d\n"
            "  [11] ins_read: %d [10] dummy_wr: %d\n"
            "  [9] dummy_nw: %d [8] dummy_rd: %d\n"
            "  [7] dummy_d: %d [6] hold_met_wr: %d\n"
            "  [5] hold_rmpfc: %d [4] hold_getbs: %d\n"
            "  [3] hold_ld: %d [2] hold_e: %d\n"
            "  [1] hold_m: %d [0] hold_d: %d\n",
            ((MONI_DSP_2>>15)&0x1), ((MONI_DSP_2>>14)&0x1), ((MONI_DSP_2>>13)&0x1), ((MONI_DSP_2>>12)&0x1),
            ((MONI_DSP_2>>11)&0x1), ((MONI_DSP_2>>10)&0x1), ((MONI_DSP_2>>9)&0x1), ((MONI_DSP_2>>8)&0x1),
            ((MONI_DSP_2>>7)&0x1), ((MONI_DSP_2>>6)&0x1), ((MONI_DSP_2>>5)&0x1), ((MONI_DSP_2>>4)&0x1),
            ((MONI_DSP_2>>3)&0x1), ((MONI_DSP_2>>2)&0x1), ((MONI_DSP_2>>1)&0x1), ((MONI_DSP_2>>0)&0x1)
            );
    }
    return 0;
}

static INT32 _AudCmdShowRegs(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;

    if (i4Argc != 2)
    {
        Printf("Usage: go [Dsp Id]\n");
        Printf("[Dsp Id]     0:DSP0        1:DSP1 \n");
        return -1;       
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
        return -1;
    }

    return _AudProbeShowRegs(u1DspId); 

}

static INT32 _AudCmdProbeGoThenShow(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;
    if (i4Argc != 2)
    {
        Printf("Usage: go [Dsp Id]\n");
        Printf("[Dsp Id]     0:DSP0        1:DSP1 \n");
        return -1;       
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
        return -1;
    }
    Printf("Probe Go\n");
    AUD_DspProbeGo(u1DspId);
    x_thread_delay(10);
    _AudProbeShowRegs(u1DspId);
    return 0;
}

static INT32 _AudCmdProbeStepThenShow(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;
    UINT32 u4Count;
    if (i4Argc != 3)
    {
        Printf("Usage: stsr [dsp id] [#]\n");
        Printf("[dsp Id]     0:DSP0        1:DSP1 \n");
        Printf("#: step number 1~255\n");
        return -1;
    }
    u1DspId = (UINT8)StrToInt(szArgv[1]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("Usage: stsr [dsp id] [#]\n");
        Printf("[dsp Id]     0:DSP0        1:DSP1 \n");
        return -1;
    }    
    u4Count = StrToInt(szArgv[2]);
    if(u4Count >= 255)
    {
        Printf("Usage: stsr [[dsp id]][#]\n");
        Printf("#: step number 1~255\n");
        return -1;
    }
    Printf("Probe Step %d\n", u4Count);
    AUD_DspProbeStep(u1DspId, u4Count);
    x_thread_delay(10);
    _AudProbeShowRegs(u1DspId);
    return 0;
}

static INT32 _AudCmdSetBPts(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;
    UINT32 u4BPt;
    UINT32 u4Addr;
    UINT32 u4temp1;
    if (i4Argc < 4)
    {
        Printf("Usage: %s [+/-] [Dsp Id][#] [Addr] \n",szArgv[0]);
        Printf("[+/-]: [enable/disable]\n");
        Printf("[Dsp Id]     0:DSP0        1:DSP1 \n");
        Printf("[#]: 0~3 number of break point\n");
        Printf("[Addr]: address of break point\n");
        return -1;
    }
    u1DspId = (UINT8)StrToInt(szArgv[2]);
    u4BPt = (UINT32)StrToInt(szArgv[3]);
    u4Addr = (UINT32) StrToInt(szArgv[4]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
        return -1;
    }
    if((szArgv[1][0]!='+'&&szArgv[1][0]!='-') || u4BPt> 3|| (u4Addr>0x1FFFF))
    {
        Printf("Usage: %s [+/-] [Dsp Id][#] [Addr] \n",szArgv[0]);
        Printf("[+/-]: [enable/disable]\n");
        Printf("[Dsp Id]     0:DSP0        1:DSP1 \n");
        Printf("[#]: 0~3 number of break point\n");
        Printf("[Addr]: address of break point\n");
        return -1;
    }
    u4temp1 = u4AUD_DspProbePBCfg(u1DspId, FALSE, 0x0);
    if(szArgv[1][0]=='+')
    {
        u4AUD_DspProbePB(u1DspId, TRUE, u4BPt,u4Addr);
        u4AUD_DspProbePBCfg(u1DspId, TRUE, (u4temp1|(UINT32)(0x1<<u4BPt)));
        return 0;
    }
    u4temp1=u4temp1&(~((UINT32)(0x1<<u4BPt)));
    u4AUD_DspProbePBCfg(u1DspId, TRUE, (u4temp1|(((UINT32)(0x1<<(u4BPt+8))))));
    return 0;
}

static INT32 _AudCmdShowBPts(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DspId;
    UINT32 i;
    UINT32 u4temp1;

    if (i4Argc != 2)
    {
        Printf("Usage: ShowBPts [Dsp Id]\n");
        Printf("[Dsp Id]     0:DSP0        1:DSP1 \n");
        return -1;       
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
        return -1;
    }

    u4temp1 = u4AUD_DspProbePBCfg(u1DspId, FALSE, 0x0);
    for(i=0;i<4;i++)
    {
        Printf("DspId = %d, %d Address: %05x ", u1DspId,i,u4AUD_DspProbePB(u1DspId, FALSE,i,0x0));
        if(u4temp1 & (1<<i))
        {
            Printf(" ENABLED\n");
        }
        else
        {
            Printf(" DISABLED\n");
        }
    }
    return 0;
}

static INT32 _AudCmdStopOnWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Addr, u4Data, u4Mask;
    BOOL   fgEnable;
    UINT8 u1DspId;

    UNUSED(fgEnable);

    if ((i4Argc != 5)&&(i4Argc != 3) && (i4Argc != 2))
    {
        Printf("Usage: sw [dsp id][Addr] [Data] [Mask] \n");
        Printf("Addr: SRAM address, 0 for clear\n");
        return -1;
    }

    u1DspId = (UINT8)StrToInt(szArgv[1]);
    if (u1DspId >= AUD_DSP_NUM)
    {
        Printf("id range error \n");
        return -1;
    }

    if (i4Argc == 2)
    {
        Printf("Usage: sw [dsp id][Addr] [Data] [Mask]\n");
        u4Addr = u4AUD_DspProbeChkStopOnWrite(u1DspId, &fgEnable, &u4Data, &u4Mask);
        if(u4Mask == 0)
        {
            Printf("Current : dsp(%d) Enable %d, Addr 0x%x\n",u1DspId, fgEnable,u4Addr);
        }
        else
        {
            Printf("Current : dsp(%d) Enable %d, Addr 0x%x, Data 0x%x, Mask 0x%x\n",u1DspId, fgEnable,u4Addr, u4Data, u4Mask);
        }
    }
    else if(i4Argc == 3)
    {
        u4Addr = StrToInt(szArgv[2]);
        u4Data = 0;
        u4Mask = 0;
        u4AUD_DspProbeStopOnWrite(u1DspId, u4Addr, u4Data, u4Mask);
    }
    else if(i4Argc == 5)
    {
        u4Addr = StrToInt(szArgv[2]);
        u4Data = StrToInt(szArgv[3]);
        u4Mask = StrToInt(szArgv[4]);
        u4AUD_DspProbeStopOnWrite(u1DspId, u4Addr, u4Data, u4Mask);
    }
    return 0;
}

#endif

#ifdef DSP_SUPPORT_SRSWOW
static INT32 _AudCmdUopSRSWOWSpkSz(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    /*
        Speaker size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz,
                            4: 200Hz, 5: 250Hz, 6: 300Hz, 7: 400Hz
    */
    if (i4Argc < 2)
    {
        Printf("Usage: wspk  [mode]\n");
        Printf("[mode] 0:  40Hz \n");
        Printf("       1:  60Hz \n");
        Printf("       2: 100Hz \n");
        Printf("       3: 150Hz \n");
        Printf("       4: 200Hz \n");
        Printf("       5: 250Hz \n");
        Printf("       6: 300Hz \n");
        Printf("       7: 400Hz \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSWOWSpkSz(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSWOWBright(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: wbrt  [mode]\n");
        Printf("[mode] 0:   0% \n");
        Printf("       1:  20% \n");
        Printf("       2:  40% \n");
        Printf("       3: 100% \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSWOWBright(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSWOWTBLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: wtb  [mode]\n");
        Printf("[mode] 0:   0% \n");
        Printf("       1:  50% \n");
        Printf("       2:  60% \n");
        Printf("       3: 100% \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSWOWTBLvl(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSWOWWidth(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: wwid  [mode]\n");
        Printf("[mode] 0:   0% \n");
        Printf("       1:  30% \n");
        Printf("       2:  80% \n");
        Printf("       3: 100% \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSWOWWidth(u1Mode);
    return 1;
}
#endif
#ifdef DSP_SUPPORT_SRSTSXT
static INT32 _AudCmdUopSRSTSXTMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1SrsMode;

    if (i4Argc < 2 )
    {
        Printf("Usage: enable [tsxtmode] \n");
        Printf("[tsxtmode]     0:SV_A_SRSTSXT_OFF   1:SV_A_SRSTSXT_3DMONO,  2:SV_A_SRSTSXT_STEREO \n");
        return -1;
    }

    u1SrsMode = (UINT8)StrToInt(szArgv[1]);
    AUD_DspSetSRSTSXTSetMode(u1SrsMode);
    return 0;
}

static INT32 _AudCmdUopSRSTSXTSpkSz(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    /*
        Speaker size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz,
                            4: 200Hz, 5: 250Hz, 6: 300Hz, 7: 400Hz
    */
    if (i4Argc < 2)
    {
        Printf("Usage: tspk  [mode]\n");
        Printf("[mode] 0:  40Hz \n");
        Printf("       1:  60Hz \n");
        Printf("       2: 100Hz \n");
        Printf("       3: 150Hz \n");
        Printf("       4: 200Hz \n");
        Printf("       5: 250Hz \n");
        Printf("       6: 300Hz \n");
        Printf("       7: 400Hz \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSTSXTSpkSz(u1Mode);
    return 1;
}


static INT32 _AudCmdUopSRSTSXTElev(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: telv  [mode]\n");
        Printf("[mode] 0:   0% \n");
        Printf("       1:  10% \n");
        Printf("       2:  50% \n");
        Printf("       3: 100% \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSTSXTElev(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSTSXTinputgain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: ing  [gain 32bits]\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    AUD_DspSetSRSTSXTinputgain(u4Mode);
    return 1;
}
static INT32 _AudCmdUopSRSTSXTTBLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: ttb  [mode]\n");
        Printf("[mode] 0:   0% \n");
        Printf("       1:  38% \n");
        Printf("       2:  60% \n");
        Printf("       3: 100% \n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetSRSTSXTTBLvl(u1Mode);
    return 1;
}
#endif

#ifdef FRAC_SPEED_SUPPORT
//sunman for play speed
static INT32 _AudCmdUopPlaySpeed(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode, u4step;

    if (i4Argc < 2)
    {
        Printf("Usage:fspeed  [s]\n");
        Printf("s: speed options  \n");
        return -1;
    }
    u4step = 0x0019999A ;  // 0.1  (0.24)
    //u4step = 0x000CCCCD ;  // 0.1  (1.23)
    u4Mode = (UINT32)StrToInt(szArgv[1]);// (1~40)
    u4Mode = u4Mode * u4step;
    u4Mode = u4Mode >> 2;

    if(u4Mode!=0)
        u4Mode -= 1;//(0.24) 0x400001-1=0x400000

    DSP_SpeedSetMode(u4Mode);
    return 1;
}

#endif

#if (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD))
//sunman_tshd
static INT32 _AudCmdUopSRSTSHDSetInit(INT32 i4Argc, const CHAR **szArgv){
    //aud.dsp.eq
    AUD_DspChannelEqEnable(0, 0);
       AUD_DspBassBoostEnable(0, 0);
       AUD_DspSuperBassEnable(0, 0);
       AUD_DspClearBoostEnable(0, 0);
    //aud.dsp.a
    #ifdef CC_AUD_AVC_V20
    AUD_DspChangeAVCPara (DRC2_FLAG, FALSE, AUD_DEC_MAIN);
    #else
    AUD_DspChangeAVCPara(0, 0, FALSE);
    #endif
    //aud.spdif.iecc
    UNUSED(AUD_DspIecChannel((AUD_IEC_CH_T)0));
        return 0;
}

static INT32   _AudCmdUopSRSTSHDInpGain(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: tinpg [lv] \n");
        Printf("Input gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDInpg(u1Mode);
    return 0;
}

static INT32    _AudCmdUopSRSTSHDOutGain(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: toutg [lv] \n");
        Printf("Output gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDOutg(u1Mode);
    return 0;
}

static INT32 _AudCmdUopSRSTSHDSurrLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: tsrlv  [lv] \n");
        Printf("surround level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDSurrLvl(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSTSHDSpkSz(INT32 i4Argc, const CHAR **szArgv)
{
     UINT8 u1Mode;
        //Speaker size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz,

    if (i4Argc < 2)
    {
        Printf("Usage: tspk  [mode]\n");
        Printf("[mode] 0:  40Hz \n");
        Printf("       1:  60Hz \n");
        Printf("       2: 80Hz \n");
        Printf("       3: 100Hz \n");
        Printf("       4: 120Hz \n");
        Printf("       5: 150Hz \n");
        Printf("       6: 200Hz \n");
        Printf("       7: 250Hz \n");
        Printf("       8: 300Hz \n");
        Printf("       9: 400Hz \n");
        Printf("       10: custom \n");

        return -1;
    }
    u1Mode = (UINT8)StrToInt(szArgv[1]);
   DSP_SRSTSHDSetSpkSize(u1Mode);
    return 1;
}
static INT32 _AudCmdUopSRSTSHDAnaSz(INT32 i4Argc, const CHAR **szArgv)
{
     UINT8 u1Mode;
        //Analysis size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz,

    if (i4Argc < 2)
    {
        Printf("Usage: tana  [mode]\n");
        Printf("[mode] 0:  40Hz \n");
        Printf("       1:  60Hz \n");
        Printf("       2: 80Hz \n");
        Printf("       3: 100Hz \n");
        Printf("       4: 120Hz \n");
        Printf("       5: 150Hz \n");
        Printf("       6: 200Hz \n");
        Printf("       7: 250Hz \n");
        Printf("       8: 300Hz \n");
        Printf("       9: 400Hz \n");
        Printf("       10: custom \n");
        return -1;
    }
    u1Mode = (UINT8)StrToInt(szArgv[1]);
   DSP_SRSTSHDSetAnaSize(u1Mode);
    return 1;
}
static INT32 _AudCmdUopSRSTSHDHpfSz(INT32 i4Argc, const CHAR **szArgv)
{
     UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: thpf  [mode]\n");
        Printf("[mode] 0: 80Hz \n");
        Printf("       1: 120Hz \n");
        Printf("       2: 200Hz \n");
        Printf("       3: 240Hz \n");
        Printf("       4: 300Hz \n");
        Printf("       5: 400Hz \n");
        Printf("       6: 500Hz \n");
        Printf("       7: 600Hz \n");
        Printf("       8: 800Hz \n");
        return -1;
    }
    u1Mode = (UINT8)StrToInt(szArgv[1]);
   DSP_SRSTSHDSetHpfSize(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSTSHDCustom(INT32 i4Argc, const CHAR **szArgv)
{
     UINT32 u4Type;
     INT32  i4Coef;

    if (i4Argc < 2)
    {
        Printf("custom speaker size coefficient\n");
        Printf("Usage: setcut  [type] [coef]\n");
        Printf("[type] 0:  Trubass  LowPass B0 \n");
        Printf("       1:  Trubass  LowPass A1 \n");
        Printf("       2:  Trubass  LowBand B0 \n");
        Printf("       3:  Trubass  LowBand A1 \n");
        Printf("       4:  Trubass  LowBand A2 \n");
        Printf("       5:  Trubass  MidBand B0 \n");
        Printf("       6:  Trubass  MidBand A1 \n");
        Printf("       7:  Trubass  MidBand A2 \n");
        Printf("       8:  Analysis LowBand B0 \n");
        Printf("       9:  Analysis LowBand A1 \n");
        Printf("       10: Analysis LowBand A2 \n");
        Printf("       11: Analysis MidBand B0 \n");
        Printf("       12: Analysis MidBand A1 \n");
        Printf("       13: Analysis MidBand A2 \n");
        return -1;
    }

    u4Type = (UINT32)StrToInt(szArgv[1]);
    i4Coef = (INT32)StrToInt(szArgv[2]);
    DSP_SRSTSHDSetCustomCoef(u4Type, i4Coef);
    return 1;
}


static INT32 _AudCmdUopSRSTSHDTBLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: ttb  [lv] \n");
        Printf("TruBass level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDTBLvl(u1Mode);
    return 1;
}
static INT32 _AudCmdUopSRSTSHDCOMPRLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: tcompr  [lv] \n");
        Printf("comprosser level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDCOMPRLvl(u1Mode);
    return 1;
}
//static INT32 _AudCmdUopSRSTSHDLEVINDEP(INT32 i4Argc, const CHAR **szArgv)
//{
//    UINT8 u1Mode;
//    u1Mode = (UINT8)StrToInt(szArgv[1]);
//    if (i4Argc < 2 || u1Mode >2)
//    {
//       Printf("Usage: tlevdep  [lv] \n");
//      Printf("Usage: level indepent \n");
//        return -1;
//    }
//    DSP_SRSTSHDLEVINDEP(u1Mode);
//    return 1;
//}
static INT32 _AudCmdUopSRSTSHDTBFLAG(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    UINT8 u1Enable;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);
    if (i4Argc < 3)
    {
        Printf("Usage: tbflag [mode] [1:on/0:0ff] \n");
        Printf("mode:  0: trubass stereo [1:on/0:0ff] \n");
        Printf("mode:  1: level indep [1:on/0:0ff] \n");
        Printf("mode:  2: analysis enable [1:on/0:0ff] \n");
        Printf("mode:  3: output mix [1:on/0:0ff] \n");

        return -1;
    }
    DSP_SRSTSHDTBFLAG(u1Mode,u1Enable);
    return 1;
}


static INT32 _AudCmdUopSRSTSHDDef(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: tdef  [lv] \n");
        Printf("Definition level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDDef(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSTSHDElev(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: telv [lv] \n");
        Printf("Elevation level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDElev(u1Mode);
    return 0;
}

static INT32 _AudCmdUopSRSTSHDFntCCtrl(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: fcc [lv] \n");
        Printf("Front Center Control level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDFntCCtrl(u1Mode);
    return 0;
}

static INT32 _AudCmdUopSRSTSHDFntSCtrl(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: fsc [lv] \n");
        Printf("Front Space Control level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTSHDFntSCtrl(u1Mode);
    return 0;
}

static INT32 _AudCmdUopSRSTSHDMode(INT32 i4Argc, const CHAR **szArgv)
{
   UINT32 u4Mode;
   u4Mode = (UINT32)StrToInt(szArgv[1]);
    if (i4Argc < 2 )
    {
        Printf("Usage: tmode [mode] \n");
        return -1;
    }
    DSP_SRSTSHDMode(u4Mode);
    return 0;
}
#endif //#if (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD))

#if (defined(DSP_SUPPORT_SRSCC3D))
static INT32   _AudCmdUopSRSCC3DFlag(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    UINT8 u1Enable;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);
    if (i4Argc < 3)
    {
        Printf("Usage: cflag [mode] [1:on/0:0ff] \n");
        Printf("mode:  0: cc_suren(bypass the depthprocess) [1:on/0:0ff] \n");
        Printf("mode:  1: cc_enable(bypass all the depthprocess and tshd) [1:on/0:0ff] \n");
        Printf("mode:  2: LR Enh Enable [1:on/0:0ff] \n");
        Printf("mode:  3: DepthChannelFadeEnable [1:on/0:0ff] \n");
        Printf("mode:  4: Right Def Enable [1:on/0:0ff] \n");
        Printf("mode:  5: Left Def Enable [1:on/0:0ff] \n");
        Printf("mode:  6: Focus enable [1:on/0:0ff] \n");
        Printf("mode:  7: Right Focus enable [1:on/0:0ff] \n");
        Printf("mode:  8: Left Focus enable [1:on/0:0ff] \n");
        Printf("mode:  9: Surround Enable [1:on/0:0ff] \n");
        Printf("mode:  10: srs3d Headphone [1:on/0:0ff] \n");
        Printf("mode:  11: Not Used [1:on/0:0ff] \n");
        Printf("mode:  12: MixEnable [1:on/0:0ff] \n");
        Printf("mode:  13: Tbhd Enable [1:on/0:0ff] \n");
        Printf("mode:  14: Space HPF Enable [1:on/0:0ff] \n");

        return -1;
    }

    DSP_SRSCC3DFlag(u1Mode,u1Enable);
    return 0;
}
static INT32   _AudCmdUopSRSCC3DMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    if (i4Argc < 2 )
    {
        Printf("Usage: cmode [mode] \n");       /* default */
        Printf("[mode] 0: SRS_CC3D_MODE_3D \n");
        Printf("       1: SRS_CC3D_MODE_MUSIC \n");
        Printf("       2: SRS_CC3D_MODE_MOVIE \n");
        return -1;
    }
    DSP_SRSCC3DMode(u4Mode);
    return 0;
}

static INT32   _AudCmdUopSRSCC3DInpGain(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: cinpg [lv] \n");
        Printf("Input gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSCC3DInpg(u1Mode);
    return 0;
}

static INT32    _AudCmdUopSRSCC3DOutGain(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: coutg [lv] \n");
        Printf("Output gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSCC3DOutg(u1Mode);
    return 0;
}
static INT32    _AudCmdUopSRSCC3DInMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    if (i4Argc < 2 )
    {
        Printf("Usage: cinmod [inmode] \n");
        Printf("[inmode] 0:SRS_IO_1_0_0 \n");
        Printf("       1:  SRS_IO_2_0_0 \n");
        Printf("       2:  SRS_IO_3_0_0 \n");
        Printf("       3:  SRS_IO_2_0_1 \n");
        Printf("       4:  SRS_IO_3_0_1 \n");
        Printf("       5:  SRS_IO_2_2_0 \n");
        Printf("       6:  SRS_IO_3_2_0 \n");
        Printf("       7:  SRS_IO_2_2_1 \n");
        Printf("       8:  SRS_IO_3_2_1 \n");
        Printf("       9:  SRS_IO_2_1_0 \n");
        Printf("       10: SRS_IO_3_1_0 \n");
        Printf("       11: SRS_IO_2_1_1 \n");
        Printf("       12: SRS_IO_3_1_1 \n");
        Printf("       13: SRS_IO_2_3_0 \n");
        Printf("       14: SRS_IO_3_3_0 \n");
        Printf("       15: SRS_IO_2_3_1 \n");
        Printf("       16: SRS_IO_3_3_1 \n");
        Printf("       17: SRS_IO_LtRt \n");
        Printf("       18: SRS_IO_Headphone \n");
        Printf("       19: SRS_IO_BSDigital \n");
        Printf("       20: SRS_IO_PL2Music \n");
        return -1;
    }
    DSP_SRSCC3DInMode(u4Mode);
    return 0;

}
static INT32    _AudCmdUopSRSCC3D3dMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    if (i4Argc < 2 )
    {
        Printf("Usage: c3dmod [3dmode] \n");
        Printf("[3dmode] 0:SRS_SRS3D_MONO \n");
        Printf("       1:  SRS_SRS3D_SINGLE_SPEAKER \n");
        Printf("       2:  SRS_SRS3D_STEREO \n");
        Printf("       3:  SRS_SRS3D_MODENUM \n");
        return -1;
    }
    DSP_SRSCC3D3DMode(u4Mode);
    return 0;

}
static INT32 _AudCmdUopSRSCC3DHpfSz(INT32 i4Argc, const CHAR **szArgv)
{
     UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: chpf  [mode]\n");
        Printf("[mode] 0:  80Hz \n");
        Printf("       1:  120Hz \n");
        Printf("       2: 200Hz \n");
        Printf("       3: 240Hz \n");
        Printf("       4: 300Hz \n");
        Printf("       5: 400Hz \n");
        Printf("       6: 500Hz \n");
        Printf("       7: 600Hz \n");
        Printf("       8: 800Hz \n");

        return -1;
    }
    u1Mode = (UINT8)StrToInt(szArgv[1]);
   DSP_SRSCC3DSetHpfSize(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSCC3DIniSz(INT32 i4Argc, const CHAR **szArgv)
{
     UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: cini  [mode]\n");
        Printf("[mode] 0:  HH \n");
        Printf("       1:  HL \n");
        Printf("       2:  NH \n");
        Printf("       3:  NL \n");

        return -1;
    }
    u1Mode = (UINT8)StrToInt(szArgv[1]);
   DSP_SRSCC3DSetIniSize(u1Mode);
    return 1;
}
static INT32 _AudCmdUopSRSCC3DIniDepth(INT32 i4Argc, const CHAR **szArgv)
{
     UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: cidep  [mode]\n");
        Printf("[mode] 0~128 \n");

        return -1;
    }
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    if(u1Mode>128)
    {
        Printf("[mode] 0~128 \n");
        return -1;
    }
    DSP_SRSCC3DSetIniDepth(u1Mode);
    return 1;
}

static INT32 _AudCmdUopSRSCC3DSpkSz(INT32 i4Argc, const CHAR **szArgv)
{
     UINT8 u1Mode;
        //Speaker size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz,

    if (i4Argc < 2)
    {
        Printf("Usage: cspk  [mode]\n");
        Printf("[mode] 0:  40Hz \n");
        Printf("       1:  60Hz \n");
        Printf("       2: 80Hz \n");
        Printf("       3: 100Hz \n");
        Printf("       4: 120Hz \n");
        Printf("       5: 150Hz \n");
        Printf("       6: 200Hz \n");
        Printf("       7: 250Hz \n");
        Printf("       8: 300Hz \n");
        Printf("       9: 400Hz \n");
        Printf("       10: custom \n");

        return -1;
    }
    u1Mode = (UINT8)StrToInt(szArgv[1]);
   DSP_SRSCC3DSetSpkSize(u1Mode);
    return 1;
}
static INT32 _AudCmdUopSRSCC3DAnaSz(INT32 i4Argc, const CHAR **szArgv)
{
     UINT8 u1Mode;
        //Analysis size table: 0: 40Hz, 1: 60Hz, 2: 100Hz, 3: 150Hz,

    if (i4Argc < 2)
    {
        Printf("Usage: cana  [mode]\n");
        Printf("[mode] 0:  40Hz \n");
        Printf("       1:  60Hz \n");
        Printf("       2: 80Hz \n");
        Printf("       3: 100Hz \n");
        Printf("       4: 120Hz \n");
        Printf("       5: 150Hz \n");
        Printf("       6: 200Hz \n");
        Printf("       7: 250Hz \n");
        Printf("       8: 300Hz \n");
        Printf("       9: 400Hz \n");
        Printf("       10: custom \n");
        return -1;
    }
    u1Mode = (UINT8)StrToInt(szArgv[1]);
   DSP_SRSCC3DSetAnaSize(u1Mode);
    return 1;
}
static INT32 _AudCmdUopSRSCC3DCustom(INT32 i4Argc, const CHAR **szArgv)
{
     UINT32 u4Type;
     INT32  i4Coef;

    if (i4Argc < 2)
    {
        Printf("custom speaker size coefficient\n");
        Printf("Usage: csetcut  [type] [coef]\n");
        Printf("[type] 0:  Trubass  LowPass B0 \n");
        Printf("       1:  Trubass  LowPass A1 \n");
        Printf("       2:  Trubass  LowBand B0 \n");
        Printf("       3:  Trubass  LowBand A1 \n");
        Printf("       4:  Trubass  LowBand A2 \n");
        Printf("       5:  Trubass  MidBand B0 \n");
        Printf("       6:  Trubass  MidBand A1 \n");
        Printf("       7:  Trubass  MidBand A2 \n");
        Printf("       8:  Analysis LowBand B0 \n");
        Printf("       9:  Analysis LowBand A1 \n");
        Printf("       10: Analysis LowBand A2 \n");
        Printf("       11: Analysis MidBand B0 \n");
        Printf("       12: Analysis MidBand A1 \n");
        Printf("       13: Analysis MidBand A2 \n");
        return -1;
    }

    u4Type = (UINT32)StrToInt(szArgv[1]);
    i4Coef = (INT32)StrToInt(szArgv[2]);
    DSP_SRSCC3DSetCustomCoef(u4Type, i4Coef);
    return 1;
}

static INT32 _AudCmdUopSRSCC3DTBLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: ctb  [lv] \n");
        Printf("TruBass level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSCC3DTBLvl(u1Mode);
    return 1;
}
static INT32 _AudCmdUopSRSCC3DCOMPRLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: ccompr  [lv] \n");
        Printf("comprosser level = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSCC3DCOMPRLvl(u1Mode);
    return 1;
}
static INT32 _AudCmdUopSRSCC3DTBFLAG(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;
    UINT8 u1Enable;
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);
    if (i4Argc < 3)
    {
        Printf("Usage: ctbflag [mode] [1:on/0:0ff] \n");
        Printf("mode:  0: trubass stereo [1:on/0:0ff] \n");
        Printf("mode:  1: level indep [1:on/0:0ff] \n");
        Printf("mode:  2: analysis enable [1:on/0:0ff] \n");
        Printf("mode:  3: output mix [1:on/0:0ff] \n");

        return -1;
    }
    DSP_SRSCC3DTBFLAG(u1Mode,u1Enable);
    return 1;
}

static INT32 _AudCmdUopSRSCC3DSetMainTech(INT32 i4Argc, const CHAR **szArgv)
{
     UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: cmtec  [tech]\n");
        Printf("[tech] 0:  WOWHD \n");
        Printf("       1:  TSHD \n");
        Printf("       2:  CC3D \n");
        Printf("       3:  CSHP \n");
        Printf("       4:  TBHD \n");

        return -1;
    }
    u1Mode = (UINT8)StrToInt(szArgv[1]);
    DSP_SRSCC3DSetMainTech(u1Mode);
    return 1;
}

static INT32   _AudCmdUopSRSSRS3DCenterCtrl(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: ccenctr [lv] \n");
        Printf("srs3d center ctrl gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSSRS3DCenterCtrl(u1Mode);
    return 0;
}
static INT32   _AudCmdUopSRSSRS3DSpaceCtrl(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: cspactr [lv] \n");
        Printf("srs3d space ctrl gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSSRS3DSpaceCtrl(u1Mode);
    return 0;
}
static INT32   _AudCmdUopSRSSpaceCtrlTune(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: csptun [lv] \n");
        Printf(" space ctrl tune gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSSpaceCtrlTune(u1Mode);
    return 0;
}
static INT32   _AudCmdUopSRSMixFadeCtrl(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: cmfad [lv] \n");
        Printf(" mix fade ctrl gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSMixFadeCtrl(u1Mode);
    return 0;
}
static INT32   _AudCmdUopSRSFrontCenterCtrl(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: cfcen [lv] \n");
        Printf(" front center ctrl gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSFrontCenterCtrl(u1Mode);
    return 0;
}
static INT32   _AudCmdUopSRSFrontSpaceCtrl(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: cfspa [lv] \n");
        Printf(" front space ctrl gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSFrontSpaceCtrl(u1Mode);
    return 0;
}
static INT32   _AudCmdUopSRSRearSpaceCtrl(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: crspa [lv] \n");
        Printf(" rear space ctrl gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSRearSpaceCtrl(u1Mode);
    return 0;
}
#if 0
static INT32   _AudCmdUopSRSRearCenterCtrl(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: crcen [lv] \n");
        Printf(" rear center ctrl gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSRearCenterCtrl(u1Mode);
    return 0;
}
#endif
static INT32   _AudCmdSRSUopTruSurLev(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: ctsl [lv] \n");
        Printf(" true surround level gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTruSurLev(u1Mode);
    return 0;
}
static INT32   _AudCmdSRSUopMixFadeCtrlExt(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: cmfce [lv] \n");
        Printf(" mix fade ctrl ext = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSMixFadeCtrlExt(u1Mode);
    return 0;
}
static INT32   _AudCmdSRSUopFocusLev(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: cflev [lv] \n");
        Printf(" focus level gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSFocusLev(u1Mode);
    return 0;
}
static INT32   _AudCmdSRSUopLFocusLev(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: clfoc [lv] \n");
        Printf(" left focus level gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSLFocusLev(u1Mode);
    return 0;
}
static INT32   _AudCmdSRSUopRFocusLev(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: crfoc [lv] \n");
        Printf(" right focus level gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSRFocusLev(u1Mode);
    return 0;
}
static INT32   _AudCmdSRSUopLDefLev(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: cldef [lv] \n");
        Printf(" left def level gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSLDefLev(u1Mode);
    return 0;
}
static INT32   _AudCmdSRSUopRDefLev(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: crdef [lv] \n");
        Printf(" right def level gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSRDefLev(u1Mode);
    return 0;
}
static INT32   _AudCmdSRSUopRefLev(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: cref [lv] \n");
        Printf(" reference level gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSRefLev(u1Mode);
    return 0;
}
static INT32   _AudCmdSRSUopTbqLev(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: ctbq [lv] \n");
        Printf(" tbq gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSTbqLev(u1Mode);
    return 0;
}
static INT32   _AudCmdSRSUopDcuLev(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: cdcu [lv] \n");
        Printf(" dcu level gain = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSDcuLev(u1Mode);
    return 0;
}



#endif //#if (defined(DSP_SUPPORT_SRSCC3D) )


#ifdef DSP_SUPPORT_SRSVIQ

extern void DSP_SRSVIQRefLvl(UINT32 u4Mode);
extern void DSP_SRSVIQMode(UINT32 u4Mode);
extern  void DSP_SRSVIQMaxGain(UINT32 u4Mode);
extern void DSP_SRSVIQInputGain(UINT32 u4Mode);
extern void DSP_SRSVIQOutputGain(UINT32 u4Mode);
extern void DSP_SRSVIQNoiseTh(UINT8 u1Mode);

//sunman_viq
static INT32 _AudCmdUopSRSVIQRefLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if(i4Argc == 2)
    {
        u4Mode = (UINT32)StrToInt(szArgv[1]);
        if (u4Mode > 20)
           u4Mode = 20;
       u4Mode *=  0x66666 ;
    }
   else if(i4Argc == 3)
   {
        u4Mode = (UINT32)StrToInt(szArgv[2]);
   }
   else
   {
        Printf("Usage: ref  [level]\n");
        Printf("Value = 0.05 * [level] : level between 0~20\n");
        //Printf("Enter levl > 20 for using IR control\n");
        return -1;
    }
    DSP_SRSVIQRefLvl(u4Mode);
    return 1;
}

#ifdef VIQ_MODE_OLD
static INT32 _AudCmdUopSRSVIQMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: mode  [mode]\n");
        Printf("[mode] 0:   Noise off, Full range\n");
        Printf("           2:   Noise off, 40   Hz\n");
        Printf("           4:   Noise off, 110 Hz\n");
        Printf("           6:   Noise off, 200 Hz\n");
        Printf("           8:   Noise off, 315 Hz\n");
        Printf("          10:  Noise off, 410 Hz\n");

     Printf("[mode] 1:  Noise on, Full range\n");
        Printf("           3:   Noise on, 40   Hz\n");
        Printf("           5:   Noise on, 110 Hz\n");
        Printf("           7:   Noise on, 200 Hz\n");
        Printf("           9:   Noise on, 315 Hz\n");
        Printf("          11:  Noise on, 410 Hz\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSVIQMode(u4Mode);
    return 1;
}
#else
static INT32 _AudCmdUopSRSVIQMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: mode  [mode]\n");
        Printf("[mode] 0:  Light\n");
        Printf("           1:   Normal\n");
        Printf("           2:   Heavy\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSVIQMode(u4Mode);
    return 1;
}
#endif

static INT32 _AudCmdUopSRSVIQMaxGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: maxg  [gain]\n");
        Printf("max gain = 0~256\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
     if ( u4Mode > 256 )
    {
        Printf("Usage: maxg  [gain]\n");
        Printf("max gain = 0~256\n");
        return -1;
    }

    DSP_SRSVIQMaxGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSVIQInputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;

    if(i4Argc == 2)
    {
        u4Mode = (UINT32)StrToInt(szArgv[1]);
        if (u4Mode > 80)
           u4Mode = 80;
           u4Mode *=  0x1999a;
    }
    else if(i4Argc == 3)
   {
        u4Mode = (UINT32)StrToInt(szArgv[2]);
   }
   else
   {
        Printf("Usage: inpg  [gain] scale 2 (0~4)\n");
        Printf("[gain] = 0~80 step 0.05\n");
        return -1;
    }

    DSP_SRSVIQInputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSVIQOutputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if(i4Argc == 2)
    {
        u4Mode = (UINT32)StrToInt(szArgv[1]);
        if (u4Mode > 80)
           u4Mode = 80;
        u4Mode *=  0x1999a;
    }
   else if(i4Argc == 3)
   {
        u4Mode = (UINT32)StrToInt(szArgv[2]);
   }
   else
   {
        Printf("Usage: outpg  [gain] scale 2 (0~4)\n");
        Printf("[gain] = 0~80 step 0.05\n");
        return -1;
    }
    DSP_SRSVIQOutputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSVIQDisableEQ(INT32 i4Argc, const CHAR **szArgv){
    //aud.dsp.eq
    AUD_DspChannelEqEnable(0, 0);
    AUD_DspBassBoostEnable(0, 0);
    AUD_DspSuperBassEnable(0, 0);
    AUD_DspClearBoostEnable(0, 0);
    AUD_DspVirtualSurroundEnalbe(0, 0);
    AUD_DspMasterVolume(0, 100);
    UNUSED(AUD_DspIecChannel( (AUD_IEC_CH_T)0));
    return 1;
}

static INT32 _AudCmdUopSRSVIQNoiseTh(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Mode;
   u1Mode = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2 || u1Mode >20)
    {
        Printf("Usage: tnoise [lv] \n");
        Printf("Noise TH = [lv]*0.05  \n");
        Printf("[lv]  = 0~20  \n");
        return -1;
    }
    DSP_SRSVIQNoiseTh(u1Mode);
    return 0;
}
#endif

#ifdef CC_AUD_SUPPORT_MS10
static INT32 _AudCmdBypassMDC(INT32 i4Argc, const CHAR **szArgv)
{

     UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: MDC Bypass [On/Off] \n");
        Printf("1: bypass, 0: no bypass\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspDDCOEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdDtvDdtDmxMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST      ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_MAIN,TRUE));
        Printf("                       SECOND  ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_AUX,TRUE));
        Printf("                       THIRD     ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_THIRD,TRUE));
        Printf("  \n");
        Printf("Usage: dm [decoder id] [dmx mode]\n");
        Printf("[decoder id]       0:FIRST      1:SECOND        2: THIRD\n");
        Printf("[dmx mode] 0:Lt/Rt         1:Lo/Ro\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    if(u1Mode<2)
    {
        AUD_DspDdtDmxRfMode(u1Mode, u1DecId,TRUE);
    }

    return 1;
};


static INT32 _AudCmdDtvDdtRfMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST      ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_MAIN,FALSE));
        Printf("                       SECOND  ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_AUX,FALSE));
        Printf("                       THIRD     ==> %d\n",AUD_DspGetDdtDmxRfMode(AUD_DEC_THIRD,FALSE));
        Printf("  \n");
        Printf("Usage: rf [decoder id] [RF mode]\n");
        Printf("[decoder id]       0:FIRST                 1:SECOND         2: THIRD\n");
        Printf("[RF mode] 0:Line         1:RF\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    if(u1Mode<2)
    {
        AUD_DspDdtDmxRfMode(u1Mode, u1DecId,FALSE);
    }

    return 1;
};

static INT32 _AudCmdDtvDdtDualMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST      ==> %d\n",AUD_DspGetDdtDualMode(AUD_DEC_MAIN));
        Printf("                       SECOND  ==> %d\n",AUD_DspGetDdtDualMode(AUD_DEC_AUX));
        Printf("                       THIRD     ==> %d\n",AUD_DspGetDdtDualMode(AUD_DEC_THIRD));
        Printf("  \n");
        Printf("Usage: dual [decoder id] [Dual mode]\n");
        Printf("[decoder id]       0:FIRST                 1:SECOND         2: THIRD\n");
        Printf("[Dual mode] 0:Stereo         1:Left only          2: Right only\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    if(u1Mode<3)
    {
        AUD_DspDdtDualMode(u1Mode, u1DecId);
    }

    return 1;
};

static INT32 _AudCmdDtvDdtDrcRange(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST      ==> %d\n",AUD_DspGetDdtDrcRange(AUD_DEC_MAIN));
        Printf("                       SECOND  ==> %d\n",AUD_DspGetDdtDrcRange(AUD_DEC_AUX));
        Printf("                       THIRD     ==> %d\n",AUD_DspGetDdtDrcRange(AUD_DEC_THIRD));
        Printf("  \n");
        Printf("Usage: drc [decoder id] [Cut/Boost Factor]\n");
        Printf("[decoder id]       0:FIRST                 1:SECOND         2: THIRD\n");
        Printf("[Cut/Boost Factor] 0 10 20 50 80 90 100\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    if(u1Mode<=100)
    {
        AUD_DspDdtCompVal(u1Mode, u1DecId);
    }

    return 1;
};

#endif  // CC_AUD_SUPPORT_MS10

#if defined(DSP_SUPPORT_SRS_PEQ)
extern void DSP_SRSPeqOn(UINT32 u4Mode);
extern void DSP_SRSPeqInputGain(UINT32 u4Mode);
extern void DSP_SRSPeqOutputGain(UINT32 u4Mode);
extern void DSP_SRSPEQHpfSetCoef(UINT32 u4Adr, UINT32 u4Coef);
extern void DSP_SRSPEQSetCoef(UINT32 u4Adr, UINT32 u4Coef);
extern void DSP_SRSHpfSetCoef(UINT32 u4Adr, UINT32 u4Coef);

static INT32 _AudCmdUopSRSPeqOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: on  [val]\n");
        Printf("val = 0 off\n");
        Printf("val = 1 on\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSPeqOn(u4Mode << (8+1));
    return 1;
}

static INT32 _AudCmdUopSRSPeqInputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: inpg  [level]\n");
        Printf("Value = 0.05 * [level] : level between 0~20\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSPeqInputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSPeqOutputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: outpg  [level]\n");
        Printf("Value = 0.05 * [level] : level between 0~20\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSPeqOutputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSPeqSetCoef(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Adr, u4Coef, u4Type;
    if (i4Argc < 4)
    {
        Printf("Usage: sc  [type] [adr] [coef]\n");
        Printf("type = 0 : SRS PEQ\n");
        //Printf("type = 1 : SRS 1st HFP\n");
        //Printf("type = 2 : SRS 2nd HPF\n");
        return -1;
    }
    u4Type = (UINT32)StrToInt(szArgv[1]);
    u4Adr = (UINT32)StrToInt(szArgv[2]);
    u4Coef = (UINT32)StrToInt(szArgv[3]);

    if(u4Type==0)
    {
        DSP_SRSPEQSetCoef(u4Adr, u4Coef);
    }
    //else if (u4Type==1)
    //{
    //    DSP_SRSHpfSetCoef(u4Adr, u4Coef);// now no use
    //}
    //else if (u4Type==2)
    //{
    //    DSP_SRSPEQHpfSetCoef(u4Adr, u4Coef);
    //}
    else
    {
        Printf("Usage: sc  [type] [adr] [coef]\n");
        Printf("type = 0 : SRS PEQ\n");
        //Printf("type = 1 : SRS 1st HFP\n");
        //Printf("type = 2 : SRS 2nd HPF\n");
        return -1;
    }
    return 1;
}


#endif

#if defined(DSP_SUPPORT_SRS_HPF)
extern void DSP_SRSHpfOn(UINT32 u4Mode);

static INT32 _AudCmdUopSRSHpfOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: on  [val]\n");
        Printf("val = 0 off\n");
        Printf("val = 1 on\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);

    DSP_SRSHpfOn(u4Mode << (8+2));
    return 1;
}

#endif

#ifdef DSP_SUPPORT_SRS_PURESOUND
extern void DSP_SRSPuresoundTrubsssOn(UINT32 u4Mode);
static INT32 _AudCmdUopSRSPuresoundTrubassOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: on  [val]\n");
        Printf("val = 0 off\n");
        Printf("val = 1 on\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);

    DSP_SRSPuresoundTrubsssOn(u4Mode << (8+4));
    return 1;
}

#endif


#ifdef DSP_SUPPORT_SRS_AEQ
extern void DSP_SRSAeqOn(UINT32 u4Mode);
extern void DSP_SRSAeqInputGain(UINT32 u4Mode);
extern void DSP_SRSAeqOutputGain(UINT32 u4Mode);
extern void DSP_SRSAeqFilterSize(UINT32 u4Mode);
extern void DSP_SRSAeqFirFilterSet(UINT32 u4Index, UINT32 u4Coef);
extern void DSP_SRSAeqIirFilterSet(UINT32 u4Index, UINT32 u4Coef);

static INT32 _AudCmdUopSRSAeqOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: on  [val]\n");
        Printf("val = 0 off\n");
        Printf("val = 1 on\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);

    DSP_SRSAeqOn(u4Mode << (8+5));
    return 1;
}

static INT32 _AudCmdUopSRSAeqInputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: inpg  [level] scale 2 (0~4)\n");
        Printf("Value = 0.05 * [level] : level between 0~80 (scale 2)\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSAeqInputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSAeqOutputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: outpg  [level] scale 2 (0~4)\n");
        Printf("Value = 0.05 * [level] : level between 0~80 (scale 2)\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSAeqOutputGain(u4Mode);
    return 1;
}
#if 0
static INT32 _AudCmdUopSRSAeqFilterSize(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: AeqFilterSize  [level]  (0~2)\n");
        Printf("level 0 default_aeq_filters.c\n");
        Printf("level 1 fir50_aeq_filters.c\n");
        Printf("level 2 iir_only_aeq_filters.c\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSAeqFilterSize(u4Mode);
    return 1;
}
#endif
static INT32 _AudCmdUopSRSAeqFirFilterSet(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Index;
    UINT32 u4Coef;
    if (i4Argc < 3)
    {
        Printf("Usage: firset [index][level]\n");      
        Printf("Usage: index should between 0~67\n");    
        return -1;
    }

    u4Index = (UINT32)StrToInt(szArgv[1]);
    u4Coef = (UINT32)StrToInt(szArgv[2]);
    if(u4Index > 67)
    {
        Printf("index should between 0~67\n"); 
        return -1;
    }
    DSP_SRSAeqFirFilterSet(u4Index, u4Coef);
    return 1;
}
static INT32 _AudCmdUopSRSAeqIirFilterSet(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Index;
    UINT32 u4Coef;
    if (i4Argc < 3)
    {
        Printf("Usage: iirset [index][level]\n");  
        Printf("Usage: index should between 0~50\n"); 
        return -1;
    }

    u4Index = (UINT32)StrToInt(szArgv[1]);
    u4Coef = (UINT32)StrToInt(szArgv[2]);
    if(u4Index > 50)
    {
        Printf("index should between 0~50\n"); 
        return -1;
    }
    DSP_SRSAeqIirFilterSet(u4Index, u4Coef);
    return 1;
}



#endif

#ifdef DSP_SUPPORT_SRS_LMT
extern void DSP_SRSLmtInputGain(UINT32 u4Mode);
extern void DSP_SRSLmtOutputGain(UINT32 u4Mode);
extern void DSP_SRSLmtBoost(UINT32 u4Mode);
extern void DSP_SRSLmtLvl(UINT32 u4Mode);
extern void DSP_SRSLmtDly(UINT32 u4Mode);
extern void DSP_SRSLmtOn(UINT32 u4Mode);
extern void DSP_SRSLmtCoeff(UINT32 u4Mode);
extern void DSP_SRSLmtThresh(UINT32 u4Mode);

static INT32 _AudCmdUopSRSLmtOn(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: on  [val]\n");
        Printf("val = 0 off\n");
        Printf("val = 1 on\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);

    DSP_SRSLmtOn(u4Mode << (8+3));
    return 1;
}

static INT32 _AudCmdUopSRSLmtInputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: inpg  [level] \n");
        Printf("Input gain = [level]*0.05  \n");
        Printf("[level]  = 0~20  \n");
        return -1;
    }
    
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    if (u4Mode >20)
    {
        Printf("Usage: inpg  [level] \n");
        Printf("Input gain = [level]*0.05  \n");
        Printf("[level]  = 0~20  \n");
        return -1;
    }  
    
    DSP_SRSLmtInputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtOutputGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: outpg  [level] \n");
        Printf("outpg gain = [level]*0.05  \n");
        Printf("[level]  = 0~20  \n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    if (u4Mode >20)
    {
        Printf("Usage: outpg  [level] \n");
        Printf("outpg gain = [level]*0.05  \n");
        Printf("[level]  = 0~20  \n");
        return -1;
    }
    DSP_SRSLmtOutputGain(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtBoost(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: bst  [level] \n");
        Printf("bst gain = [level]*SRS_FXP32(1.0, 6)/100  \n");
        Printf("[level]  = 100~3200  \n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    if (u4Mode < 100 || u4Mode > 3200)
    {
        Printf("[level] must be 100~3200  \n");
        return -1;
    }
    DSP_SRSLmtBoost(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtLvl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: lvl  [level] \n");
        Printf("lvl gain = [level]*0.005  \n");
        Printf("[level]  = 1~200  \n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    if (u4Mode < 1 || u4Mode > 200)
    {
        Printf("[level] must be 1~200  \n");
        return -1;
    }  
    DSP_SRSLmtLvl(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtDly(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: dly [count]\n");
        Printf("count = 1~48\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    if (u4Mode <1 || u4Mode > 48)
    {
        Printf("count must be 1~48  \n");
        return -1;
    }
    DSP_SRSLmtDly(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtCoeff(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: LmtCoeff [level]\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtCoeff(u4Mode);
    return 1;
}

static INT32 _AudCmdUopSRSLmtThresh(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: LmtThresh [level]\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);
    DSP_SRSLmtThresh(u4Mode);
    return 1;
}
#endif


#if (defined(DSP_SUPPORT_SRSCC3D))
#ifdef DSP_SUPPORT_SRS_LMT
#ifdef DSP_SUPPORT_SRS_AEQ
static INT32 _AudCmdUopSRSCC3DSetInit(INT32 i4Argc, const CHAR **szArgv){

    //aud.spdif.iecc
    UNUSED(AUD_DspIecChannel((AUD_IEC_CH_T)0));

    AUD_DspMasterVolume(0, 100);//aud.uop.v 100

    DSP_SRSAeqOn(0 << (8+5));//aud.uop.aeq.on 0
    //AUD_DspSetHPFEnable(0);//aud.uop.hpf 0
    DSP_SRSHpfOn(0);
    DSP_SRSLmtOn(0 << (8+3));//aud.uop.lmt.on 0

    AUD_DspBassMngEnalbe(0,0);//spkuop.bs 0 0

    //aud.uop.sbass.flag 0 0 0
    AUD_DspBassBoostEnable(0, (BOOL)0);
    AUD_DspSuperBassEnable(0, (BOOL)0);
    AUD_DspClearBoostEnable(0, (BOOL)0);

    AUD_DspChangeAVCPara(0, 0, FALSE);//aud.dsp.a 0 0


    return 0;
}
#endif
#endif
#endif

#if defined(DSP_SUPPORT_SRS_SSHD) || defined(DSP_SUPPORT_SRS_PURESOUND) || defined(DSP_SUPPORT_SRS_SS3D)
extern void  DSP_SRSHeadRoom(UINT32 u4Mode);
extern void  DSP_SRSCompensate(UINT32 u4Mode);

static INT32 _AudCmdUopSRSHeadRoom(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode;
    if (i4Argc < 2)
    {
        Printf("Usage: hroom  [val]\n");//0: 0db,1: 6db,2: 12db,3: 18db,4: 24db,5: 30db
        Printf("val = 0 0db\n");
        Printf("val = 1 6db\n");
        Printf("val = 2 12db\n");
        Printf("val = 3 18db\n");
        Printf("val = 4 24db\n");
        Printf("val = 5 30db\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);

    DSP_SRSHeadRoom(u4Mode);
    return 1;
}
static INT32 _AudCmdUopSRSCompensate(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Mode = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: hroom  [val]\n");//0: 0db,1: 6db,2: 12db,3: 18db,4: 24db,5: 30db
        Printf("val = 0 0db\n");
        Printf("val = 1 6db\n");
        Printf("val = 2 12db\n");
        Printf("val = 3 18db\n");
        Printf("val = 4 24db\n");
        Printf("val = 5 30db\n");
        return -1;
    }
    u4Mode = (UINT32)StrToInt(szArgv[1]);

    DSP_SRSCompensate(u4Mode);
    return 1;
}

#endif


#ifdef DSP_SUPPORT_TCL_VBASS
extern void DSP_TVBA0Peak(INT32 value);
extern void DSP_TVBA1Peak(INT32 value);
extern void DSP_TVBA2Peak(INT32 value);
extern void DSP_TVBB0Peak(INT32 value);
extern void DSP_TVBB1Peak(INT32 value);
extern void DSP_TVBB2Peak(INT32 value);
extern void DSP_TVBAT(INT32 value);
extern void DSP_TVBRT(INT32 value);
extern void DSP_TVBAPeak(INT32 value);
extern void DSP_TVBGAIN_HAMONICGEN(INT32 value);
extern void DSP_TVBG_OUT(INT32 value);
extern void DSP_TVBGAIN_BACK(INT32 value);
extern void DSP_TVBGAIN_OUT(INT32 value);
extern void DSP_TVBGAINHAMONICOUT(INT32 value);
extern void DSP_TVBCOEFF_HAM(INT32 value);
extern void DSP_TVBX1PEAK(INT32 value);
extern void DSP_TVBX2PEAK(INT32 value);
extern void DSP_TVBC1PEAK(INT32 value);
extern void DSP_TVBC2PEAK(INT32 value);
extern void DSP_TVBCOUT_PEAK(INT32 value);  
extern void DSP_TVBY_OUT(INT32 value);
extern void DSP_TVBSetCoef(INT32 u4Type, INT32 i4Coef );

static INT32 _AudCmdUopTVBSetInit(INT32 i4Argc, const CHAR **szArgv){

    //aud.spdif.iecc
    UNUSED(AUD_DspIecChannel((AUD_IEC_CH_T)0));

    AUD_DspMasterVolume(0, 100);//aud.uop.v 100

    AUD_DspBassMngEnalbe(0,0);//spkuop.bs 0 0

    //aud.uop.sbass.flag 0 0 0
    AUD_DspBassBoostEnable(0, (BOOL)0);
    AUD_DspSuperBassEnable(0, (BOOL)0);
    AUD_DspClearBoostEnable(0, (BOOL)0);

    AUD_DspChangeAVCPara(0, 0, FALSE);//aud.uop.a 0 0

    AUD_DspVirtualSurroundEnalbe( 0,  0);//aud.uop.vs 0 0

    AUD_DspSetTVBEnable(1);


    return 0;
}

static INT32 _AudCmdUopTVBEnable(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: tvb [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetTVBEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdUopTVBA0P(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: a0p  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBA0Peak(value);
    return 1;
}
static INT32 _AudCmdUopTVBA1P(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: a1p  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBA1Peak(value);
    return 1;
}
static INT32 _AudCmdUopTVBA2P(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: a2p  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBA2Peak(value);
    return 1;
}
static INT32 _AudCmdUopTVBB0P(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: b0p  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBB0Peak(value);
    return 1;
}
static INT32 _AudCmdUopTVBB1P(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: b1p  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBB1Peak(value);
    return 1;
}
static INT32 _AudCmdUopTVBB2P(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: b2p  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBB2Peak(value);
    return 1;
}

static INT32 _AudCmdUopTVBAT(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: at  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBAT(value);
    return 1;
}
static INT32 _AudCmdUopTVBRT(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: rt  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBRT(value);
    return 1;
}
static INT32 _AudCmdUopTVBAP(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: ap  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBAPeak(value);
    return 1;
}
static INT32 _AudCmdUopTVBGAIN_HAMONICGEN(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: gham  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBGAIN_HAMONICGEN(value);
    return 1;
}
static INT32 _AudCmdUopTVBG_OUT(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: gout  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBG_OUT(value);
    return 1;
}
static INT32 _AudCmdUopTVBGAIN_BACK(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: gback  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBGAIN_BACK(value);
    return 1;
}
static INT32 _AudCmdUopTVBGAIN_OUT(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: gaout  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBGAIN_OUT(value);
    return 1;
}
static INT32 _AudCmdUopTVBGAINHAMONICOUT(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: ghcout  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBGAINHAMONICOUT(value);
    return 1;
}
static INT32 _AudCmdUopTVBCOEFF_HAM(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: cham  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBCOEFF_HAM(value);
    return 1;
}
static INT32 _AudCmdUopTVBX1PEAK(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: x1p  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBX1PEAK(value);
    return 1;
}
static INT32 _AudCmdUopTVBX2PEAK(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: x2p  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBX2PEAK(value);
    return 1;
}
static INT32 _AudCmdUopTVBC1PEAK(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: c1p  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBC1PEAK(value);
    return 1;
}
static INT32 _AudCmdUopTVBC2PEAK(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: c2p  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBC2PEAK(value);
    return 1;
}
static INT32 _AudCmdUopTVBCOUT_PEAK(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: ctp  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBCOUT_PEAK(value);
    return 1;
}
static INT32 _AudCmdUopTVBY_OUT(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 value = 0;
    if (i4Argc < 2)
    {
        Printf("Usage: yt  [val]\n");//
        return -1;
    }
    value = (INT32)StrToInt(szArgv[1]);

    DSP_TVBY_OUT(value);
    return 1;
}

static INT32 _AudCmdUopTVBSetCoeff(INT32 i4Argc, const CHAR **szArgv)
{
     UINT32 u4Type;
     INT32  i4Coef;

    if (i4Argc < 2)
    {
        Printf("Usage: coef  [type] [coef]\n");
        Printf("[type] 0:  LPF B1[0] \n");
        Printf("       1:  LPF B1[1] \n");
        Printf("       2:  LPF B1[2] \n");
        Printf("       3:  LPF A1[1] \n");
        Printf("       4:  LPF A1[2] \n");
        Printf("       5:  LPF B2[0] \n");
        Printf("       6:  LPF B2[1] \n");
        Printf("       7:  LPF B2[2] \n");
        Printf("       8:  LPF A2[1] \n");
        Printf("       9:  LPF A2[2] \n");
        Printf("       10: LPF GAIN \n");

        Printf("       11:  HPF B1[0] \n");
        Printf("       12:  HPF B1[1] \n");
        Printf("       13:  HPF B1[2] \n");
        Printf("       14:  HPF A1[1] \n");
        Printf("       15:  HPF A1[2] \n");
        Printf("       16:  HPF B2[0] \n");
        Printf("       17:  HPF B2[1] \n");
        Printf("       18:  HPF B2[2] \n");
        Printf("       19:  HPF A2[1] \n");
        Printf("       20:  HPF A2[2] \n");
        Printf("       21:  HPF B3[0] \n");
        Printf("       22:  HPF B3[1] \n");
        Printf("       23:  HPF B3[2] \n");
        Printf("       24:  HPF A3[1] \n");
        Printf("       25:  HPF A3[2] \n");
        Printf("       26:  HPF GAIN \n");

        Printf("       27:  FB  B1[0] \n");
        Printf("       28:  FB  B1[1] \n");
        Printf("       29:  FB  B1[2] \n");
        Printf("       30:  FB  A1[1] \n");
        Printf("       31:  FB  A1[2] \n");
        Printf("       32:  FB  B2[0] \n");
        Printf("       33:  FB  B2[1] \n");
        Printf("       34:  FB  B2[2] \n");
        Printf("       35:  FB  A2[1] \n");
        Printf("       36:  FB  A2[2] \n");
        Printf("       37:  FB  GAIN \n");

        Printf("       38:  FOUT B1[0] \n");
        Printf("       39:  FOUT B1[1] \n");
        Printf("       40:  FOUT B1[2] \n");
        Printf("       41:  FOUT A1[1] \n");
        Printf("       42:  FOUT A1[2] \n");
        Printf("       43:  FOUT B2[0] \n");
        Printf("       44   FOUT B2[1] \n");
        Printf("       45:  FOUT B2[2] \n");
        Printf("       46:  FOUT A2[1] \n");
        Printf("       47:  FOUT A2[2] \n");
        Printf("       48:  FOUT B3[0] \n");
        Printf("       49:  FOUT B3[1] \n");
        Printf("       50:  FOUT B3[2] \n");
        Printf("       51:  FOUT A3[1] \n");
        Printf("       52:  FOUT A3[2] \n");
        Printf("       53:  FOUT GAIN \n");

        
        return -1;
    }

    u4Type = (UINT32)StrToInt(szArgv[1]);
    i4Coef = (INT32)StrToInt(szArgv[2]);
    DSP_TVBSetCoef(u4Type, i4Coef);
    return 1;
}



#endif


#ifdef CC_AUD_BBE_SUPPORT
static INT32 _AudCmdUopBBEQuery(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 i1BbeMode;
    INT8   i1BbeLevel;
    UINT8 u1BbeProcess;
    UINT8 u1BbeLoContour;
    UINT8 u1Bbe3dGain;

    i1BbeMode = AUD_DspGetBbeMode();
    i1BbeLevel = AUD_DspGetBbeLevel();
    u1BbeProcess = AUD_DspGetBbeProcess();
    u1BbeLoContour = AUD_DspGetBbeLoContour();
    u1Bbe3dGain = AUD_DspGetBbe3dGain();

    Printf("BBE Mode=%d, Level=%d, LowGain=%d, HighGain=%d, 3D=%d\n",
           i1BbeMode, i1BbeLevel, u1BbeLoContour, u1BbeProcess, u1Bbe3dGain);

        return 1;
}

static INT32 _AudCmdUopBBEMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mode;

        if (i4Argc < 2)
        {
        Printf("Usage: mode  [mode]\n");
            Printf("[mode] 0:  off,  1:  BBE,  2:  BBE VIVA4\n");
              return -1;
        }
        u1Mode = (UINT8)StrToInt(szArgv[1]);

        AUD_DspBbeMode(AUD_DEC_MAIN, u1Mode); //for main decoder only
        return 1;
}

static INT32 _AudCmdUopBBELevel(INT32 i4Argc, const CHAR **szArgv)
{
    INT8 i1Level;

        if (i4Argc < 2)
        {
        Printf("Usage: level [level]\n");
            Printf("[level] 0 ~ 5 \n");
            return -1;
        }
        i1Level = (UINT8)StrToInt(szArgv[1]);

        AUD_DspSetBbeLevel( i1Level); //for main decoder only
        return 1;
}

static INT32 _AudCmdUopBBEHighGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Gain;

        if (i4Argc < 2)
        {
        Printf("Usage: high [high gain]\n");
            Printf("[high gain] 3 ~ 12 \n");
            return -1;
        }
        u1Gain = (UINT8)StrToInt(szArgv[1]);

        AUD_DspSetBbeProcess( u1Gain); //for main decoder only
    return 1;
}

static INT32 _AudCmdUopBBELowGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Gain;

        if (i4Argc < 2)
        {
        Printf("Usage: low [low gain]\n");
            Printf("[low gain] 3 ~ 12 \n");
            return -1;
        }
        u1Gain = (UINT8)StrToInt(szArgv[1]);

        AUD_DspSetBbeLoContour( u1Gain); //for main decoder only
    return 1;
}

static INT32 _AudCmdUopBBE3DGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Gain;

        if (i4Argc < 2)
        {
        Printf("Usage: 3d [3d gain]\n");
            Printf("[3d gain] 0 ~ 100 \n");
            return -1;
        }
        u1Gain = (UINT8)StrToInt(szArgv[1]);

        AUD_DspSetBbe3dGain( u1Gain); //for main decoder only
    return 1;
}

 #endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
static INT32 _AudCmdUopCDNotchFlag(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Flag;

    if (i4Argc < 2)
    {
        Printf("Usage: cdnotch.flag [enable]\n");
        Printf("[flag] 0(disable) 1(enable)\n");
        return -1;
    }

    u1Flag = (UINT8)StrToInt(szArgv[1]);
    AUD_DspClearSoundEnable(AUD_DEC_MAIN, u1Flag);

    return 1;
}

static INT32 _AudCmdUopCDNotchConfig(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32  u4UserQInt, u4UserQFrac, u4UserQIdx, u4UserFc;
    const CHAR* p1strQ, *p1strFc;
    CHAR    p1strTemp[2];


    if (i4Argc != 3)
    {
        Printf("Usage: cdnotch.cfg [Q] [Fc]\n");
        Printf("[Q] 0 ~ 3.7 [Fc] 60Hz ~ 15kHz\n");
        return -1;
    }

    p1strFc = szArgv[2];
    u4UserFc = (UINT32)StrToInt(p1strFc);
    p1strQ = szArgv[1];

    switch ((UINT32)x_strlen(p1strQ))
    {
        case 1:
            u4UserQInt = (UINT32)StrToInt(p1strQ);
            u4UserQFrac = 0;
            break;
        case 3:
            p1strTemp[0] = p1strQ[0];
            p1strTemp[1] = '\0';
            u4UserQInt = (UINT32)StrToInt(p1strTemp);
            if (p1strQ[1] != '.')
            {
                Printf("Incorrect format of Q.\n");
                Printf("Q = x.x\n");
                return -1;
            }
            p1strTemp[0] = p1strQ[2];
            u4UserQFrac = (UINT32)StrToInt(p1strTemp);
            break;
        default:
            Printf("Incorrect format of Q.\n");
            Printf("Q = x.x\n");
            return -1;
    };

    u4UserQIdx = (u4UserQInt*10)+u4UserQFrac;
    AUD_DspSetCDNotchQandFc(u4UserQIdx, u4UserFc); //for main decoder only

    return 1;
}
#endif

#ifdef  KARAOKE_SUPPORT
extern void AUD_DspSetKeyshiftFlag(BOOL fgflag);
extern void AUD_DspSetKeyshiftKey(INT8 i1key);
static INT32 _AudCmdUopKeyShiftFlag(INT32 i4Argc, const CHAR **szArgv)
{
    BOOL    fgflag;
    INT32   u4flag;

    u4flag = StrToInt(szArgv[1]);

    if (i4Argc != 2 || (u4flag != 0 && u4flag != 1))
    {
        Printf("Usage: keyshift.flag [1/0], 1:On, 0:Off\n");
        return -1;
    }

    fgflag = (u4flag) ? TRUE : FALSE;
    AUD_DspSetKeyshiftFlag(fgflag);

    return 1;
}
static INT32 _AudCmdUopKeyShiftKey(INT32 i4Argc, const CHAR **szArgv)
{
    INT8   i1key;

    i1key = (INT8)StrToInt(szArgv[1]);
    if ( (i4Argc != 2) || (i1key < 0) || (i1key > 32))
    {
        Printf("Usage: keyshift.key [0~32(-16~+16)]\n");
        return -1;
    }

    AUD_DspSetKeyshiftKey(i1key-16);

    return 1;
}
#endif

#if 0
static INT32 _AudCmdUopSAPVolume(INT32 i4Argc,const CHAR** szArgv)
{

    UINT8 u1SAPIndex;
    u1SAPIndex = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc < 2)
    {
        Printf("Usage: sap [table index]  \n");
        Printf("[table index]    0 ~ 40 : -10 dB to 10 dB step 0.5 dB  \n");
        return -1;
    }
   else  if (u1SAPIndex<41)
    {
        AUD_DspSetSAPFineVolume(u1SAPIndex);
        return 1;
    }
   else
    {
        Printf("Usage: sap [table index]  \n");
        Printf("[table index]    0 ~ 40 : -10 dB to 10 dB step 0.5 dB  \n");
        return -1;
    }
}
#endif

static INT32 _AudCmdDDbanner(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1DecId;
   UINT8 u1Enable;

    if (i4Argc < 3)
    {
        Printf("Usage: banner [decoder id] [On/Off] \n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DDBannerEnable(u1DecId, (BOOL)u1Enable);
    return 0;
}

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
static INT32 _AudCmdUopUploadData(INT32 i4Argc, const CHAR **szArgv)
{

     UINT8 u1Mode;

    if (i4Argc < 2)
    {
        Printf("Usage: upload [Format] \n");
        Printf("0: off, 1: PCM, 2: MP3, 3: SBC Encoder, 4: ALSA, 5: WiFi\n");
        return -1;
    }

    u1Mode = (UINT8)StrToInt(szArgv[1]);

    AUD_DspSetUploadMode(u1Mode);
    AUD_DspUploadDataEnable();
    return 1;
}

static INT32 _AudCmdDataUploadInit(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc < 1)
    {
        Printf("Usage: init\n");
        return -1;
    }

    AUD_DataUpload_Init();
    AUD_DataUpload_start();

    return 1;
}

extern UINT32 u4GetUploadFIFOStart(void);
extern UINT32 u4GetUploadFIFOEnd(void);
extern UINT32 u4GetUploadWritePnt(void);
static INT32 _AudCmdDataUploadDump(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4FifoSA;
    UINT32 u4FifoEA;
    UINT32 u4RP, u4WP;

    if (i4Argc != 3)
    {
        Printf("Usage: upd [output addr] [output size]\n");
        return -1;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    u4Size = (UINT32)StrToInt(szArgv[2]);
    u4FifoSA = u4GetUploadFIFOStart();
    u4FifoEA = u4GetUploadFIFOEnd();

    Printf("Dump upload data - FIFO(%08x~%08x) to (%08x~%08x)\n", u4FifoSA, u4FifoEA, u4Addr, u4Addr+u4Size);

    u4RP = u4GetUploadWritePnt();
    if ((u4RP < u4FifoSA) || (u4RP >= u4FifoEA))
    {
        u4RP = u4FifoSA;
    }

    AUD_DspSetUploadMode(4);
    AUD_DspUploadDataEnable();

    while (u4Size)
    {
        u4WP = u4GetUploadWritePnt();
        if ((u4WP >= u4FifoSA) && (u4WP < u4FifoEA)) //validity check
        {
            if (u4RP < u4WP)
            {
                if ((u4WP - u4RP) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
            else if (u4RP > u4WP)
            {
                if (((u4FifoEA - u4FifoSA) - (u4RP - u4WP)) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
        }

        x_thread_delay(10);
    }

    AUD_DspSetUploadMode(0);
    AUD_DspUploadDataEnable();

    return 1;
}

//#ifdef LINUX_TURNKEY_SOLUTION
#if 0
extern void MTAUD_RecordPCM_query(void);
static INT32 _AudCmdRecordPcmQuery(INT32 i4Argc, const CHAR **szArgv)
{
    MTAUD_RecordPCM_query();
    return 1;
}
#endif
//#endif

extern UINT32 u4GetAFIFOStart(UINT8 u1DspId, UINT8 uDecIndex);
extern UINT32 u4GetAFIFOEnd(UINT8 u1DspId, UINT8 uDecIndex);
extern UINT32 u4GetAWritePnt(UINT8 u1DspId, UINT8 uDecIndex);
static INT32 _AudCmdADCDump(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4FifoSA;
    UINT32 u4FifoEA;
    UINT32 u4RP, u4WP;

    if (i4Argc != 3)
    {
        Printf("Usage: adcd [output addr] [output size]\n");
        return -1;
    }

    u4Addr = (UINT32)StrToInt(szArgv[1]);
    u4Size = (UINT32)StrToInt(szArgv[2]);
    u4FifoSA = u4GetAFIFOStart(AUD_DSP0, 1);
    u4FifoEA = u4GetAFIFOEnd(AUD_DSP0, 1);

    Printf("Dump line-in ADC data - FIFO(%08x~%08x) to (%08x~%08x)\n", u4FifoSA, u4FifoEA, u4Addr, u4Addr+u4Size);

    //TODO
    (*((volatile UINT32*)(0xf000501c)))=0x8c0; //Disable Line-In
    (*((volatile UINT32*)(0xf0005010)))=0x172e172e;
    (*((volatile UINT32*)(0xf0005024)))= (((u4FifoSA&0xffff00)<<8) | ((u4FifoEA&0xffff00)>>8));
    //(*((volatile UINT32*)(0xf0005040)))=0x300610;
    (*((volatile UINT32*)(0xf000d26c)))=0x00170001; //Original - 0x110001
    (*((volatile UINT32*)(0xf0005248)))=0x004804c0; //Input Selection   ([15:12] = F, power down internal adc)
    (*((volatile UINT32*)(0xf000501c)))=0x8c9; //Enable Line-In + LSB 16bits
    //TODO

    u4RP = (u4GetAWritePnt(AUD_DSP0, 1)&0xfffffc00);
    if ((u4RP < u4FifoSA) || (u4RP >= u4FifoEA))
    {
        u4RP = u4FifoSA;
    }

    while (u4Size)
    {
        u4WP = u4GetAWritePnt(AUD_DSP0, 1);
        if ((u4WP >= u4FifoSA) && (u4WP < u4FifoEA)) //validity check
        {
            if (u4RP < u4WP)
            {
                if ((u4WP - u4RP) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
            else if (u4RP > u4WP)
            {
                if (((u4FifoEA - u4FifoSA) - (u4RP - u4WP)) > 0x400)
                {
                    x_memcpy((VOID*)(VIRTUAL(u4Addr)),
                                (VOID*)(VIRTUAL(u4RP)),
                                0x400);
                    u4RP += 0x400;
                    if (u4RP == u4FifoEA)
                        u4RP = u4FifoSA;
                    u4Addr += 0x400;
                    u4Size -= 0x400;
                    Printf(".");
                    continue;
                }
            }
        }

        x_thread_delay(10);
    }

    //TODO
    (*((volatile UINT32*)(0xf000501c)))=0x8c0; //Disable Line-In
    (*((volatile UINT32*)(0xf000d26c)))=0x00110001;
    //TODO

    return 1;
}

static INT32 _AudCmdUopPostQuery(INT32 i4Argc, const CHAR **szArgv)
{
    //UINT8 u1ChId;
    UINT8 u1DownmixPosition;
    UINT8 u1MatrixMode;
    UINT8 u1PanFade;
    BOOL fgPostProcessingEnable;
#ifdef CC_AUD_SILENCE_SUPPORT
    UINT32 u4SilenceMode;
#endif
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    UINT32 u4AvcValue = 0;
    UINT8 u1LimiterMode;
    UINT16 u2LimiterFlag;
    UINT32 u4LimiterThre;
    UINT8 u1MasterVolume;
    UINT32 u4MasterVolume;
    BOOL fgSurroundEnable;
    BOOL fgBassEnable;
    BOOL fgTrebleEnable;
    BOOL fgEqEnable;
    BOOL fgBassManagementEnable;
    BOOL fgChannelVolumeEnable;
    BOOL fgChannelDelayEnable;
    UINT16 u2SystemSetup;

    /*
    if (i4Argc != 2)
    {
        Printf("Usage: pq [ch_id]\n");
        Printf("  ch_id - 0(L/R),1(SL/SR),2(C/LFE),3(CH7/8),4(CH9/10),5[Aux L/R]\n");
        return -1;
    }
    u1ChId = (UINT8)StrToInt(szArgv[1]);
    */

    u1DownmixPosition = AUD_DspGetDownmixPosition(AUD_DSP0);
    u1MatrixMode = AUD_DspGetMatrixOutMode(AUD_DEC_MAIN);
    u1PanFade = AUD_DspGetAdPanFadeSetting();
    fgPostProcessingEnable = AUD_DspGetPostProcessingEnable(AUD_DSP0, AUD_DEC_MAIN);
#ifdef CC_AUD_SILENCE_SUPPORT
    DSP_QrySilenceMode(&u4SilenceMode);
#endif
    AUD_GetStreamFrom(AUD_DSP0, AUD_DEC_MAIN,&eStreamFrom);
#ifdef CC_AUD_AVC_V20
    AUD_DspGetAVCPara(DRC2_FLAG, &u4AvcValue, AUD_DEC_MAIN);
#else
    AUD_DspGetAVCPara(0, (UINT16*)&u4AvcValue, AUD_DEC_MAIN);
#endif
    AUD_DspGetLimiterConfig(&u1LimiterMode, &u2LimiterFlag, &u4LimiterThre);
    u1MasterVolume = AUD_DspGetMasterVolume(AUD_DEC_MAIN);
    u4MasterVolume = AUD_DspGetFineTuneVolume(AUD_DEC_MAIN,AUD_CH_ALL);
    fgSurroundEnable = AUD_DspGetVirtualSurroundEnalbe(AUD_DEC_MAIN);
    fgBassEnable = AUD_DspGetBassBoostEnable(AUD_DEC_MAIN);
    fgTrebleEnable = AUD_DspGetClearBoostEnable(AUD_DEC_MAIN);
    fgEqEnable = AUD_DspGetChannelEqEnable(AUD_DEC_MAIN);
    fgBassManagementEnable = AUD_DspGetBassMngEnable(AUD_DSP0, AUD_DEC_MAIN);
    fgChannelVolumeEnable = AUD_DspGetChannelVolumeEnable(AUD_DSP0, AUD_DEC_MAIN);
    fgChannelDelayEnable = AUD_DspGetDelayEnable(AUD_DSP0, AUD_DEC_MAIN);
    u2SystemSetup = AUD_DspGetSystemSetup(AUD_DSP0, AUD_DEC_MAIN);

    Printf("[DEC1 Post-Proc Path]\n");
    Printf("  fill_asrc_buf\n");
    Printf("  get_asrc_output_data\n");
    //ToDo: upload path if upload_flag[0]
    if (u1DownmixPosition == 3)
    {
        Printf("  ==>CH9/10 output here (wo post-proc)\n");
    }
    Printf("  matrix mode: %d\n", u1MatrixMode);
    if (u1PanFade == 1)
    {
        Printf("  ad_pan_fade\n");
    }
    Printf("  ==>CH7/8 output here (wo post-proc)\n");
    if (u1PanFade == 2)
    {
        Printf("  ad_pan_fade\n");
    }
    if (u1DownmixPosition == 2)
    {
        Printf("  ==>CH9/10 output here (wo post-proc)\n");
    }
    if (fgPostProcessingEnable)
    {
#ifdef CC_AUD_SILENCE_SUPPORT
        Printf("  Silence %s\n", u4SilenceMode ? "Enable" : "Disable");
#endif
        Printf("  InputSrcGain %d (sv,svq)\n",AUD_DspGetSrcVol(AUD_DEC_MAIN, eStreamFrom));
        Printf("  Reserve headroom 3bits\n");
        if (u1DownmixPosition == 1)
        {
            Printf("  ==>CH9/10 output here (before AVC)\n");
        }
        if (u1PanFade==3)
        {
            Printf("  ad_pan_fade\n");
        }
        Printf("    AVC: %s (a)\n", u4AvcValue ? "On" : "Off");
        Printf("    Master Volume: %d(0x%08x) (v,fv)\n", u1MasterVolume, u4MasterVolume);
        Printf("    Limiter Mode: %d (limiter)\n", u1LimiterMode);
        //Printf("    Downmix to LR if no up-mix\n");
        Printf("    Surround: %s (vs,vscfg)\n", fgSurroundEnable ? "On" : "Off");
        Printf("    Limiter Mode: %d (limiter)\n", u1LimiterMode);
        Printf("    Bass: %s (sbass)\n", fgBassEnable ? "On" : "Off");
        Printf("    Treble: %s (sbass)\n", fgTrebleEnable ? "On" : "Off");
        Printf("    Limiter Mode: %d (limiter)\n", u1LimiterMode);
        Printf("    Equalizer: %s (eq)\n", fgEqEnable ? "On" : "Off");
        Printf("    Limiter Mode: %d (limiter)\n", u1LimiterMode);
        Printf("  Recover headroom 3bits\n");
        Printf("  EQ spectrum\n");
        if (u2SystemSetup & 0x0040)
        {
            Printf("  =>Downmix to CH L/R\n");
        }
    }
    else
    {
        Printf("  Post-Proc bypass\n");
    }
    //ToDo: upload path if upload_flag[3]
    if (u1DownmixPosition == 0)
    {
        Printf("  ==>CH9/10 output here (w/ Post-Proc)\n");
    }
    if (u2SystemSetup & 0x1000)
    {
        Printf("  ==>Copy CH7/8 to CH LS/RS\n");
    }
    Printf("  Bass Management: %s (spkuop)\n", fgBassManagementEnable ? "On" : "Off");
    Printf("  == Post-Proc End ==\n");
    //ToDo: HPF Here
    //ToDo: _2ch_to_4ch
    Printf("  Channel Volume: %s (cv)\n", fgChannelVolumeEnable ? "On" : "Off");
    Printf("  Delay: %s (spkuop.delaycfg)\n", fgChannelDelayEnable ? "On" : "Off");

    return 1;
}

static INT32 _AudCmdUopDDCO(INT32 i4Argc, const CHAR **szArgv)
{

     UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: DDCO [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspDDCOEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdUopDDCOAgc(INT32 i4Argc, const CHAR **szArgv)
{

     UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: DDCO AGC [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspDDCOAgcEnable((BOOL)u1Enable);
    return 1;
}

static INT32 _AudCmdUopDDCOLFE(INT32 i4Argc, const CHAR **szArgv)
{

     UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: DDCO LFE LPF [On/Off] \n");
        Printf("[On/Off]         1/0\n");
        return -1;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspDDCOLFEEnable((BOOL)u1Enable);
    return 1;
}

#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
static INT32 _AudCmdUopADV(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: ADV  \n");
        Printf("[Off]         0\n");
        Printf("[light compression]         1\n");
        Printf("[medium compression]         2\n");
        Printf("[heavy compression]         3\n");
        return -1;
    }
    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_DspADVEnable((UINT8)u1Enable);
    return 1;
}

static INT32 _AudCmdUopADV_Query(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    _AudADVQuery();
    return 1;
}

static INT32 _AudCmdUopADVSetChGain(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Item;
    UINT32 u4value;

    if (i4Argc < 3)
    {
        Printf("Usage: Set ADV channel Gain {[item indx]} [value] \n");
        Printf("  \n");
        Printf("             [item indx]                    [value in dB (10.14)] \n");
        Printf("    0  :  chCalbGain_L                     0x0~ 0xffffff         \n");
        Printf("    1  :  chCalbGain_R                     0x0~ 0xffffff       \n");
        return -1;
    }
    else
    {
        u1Item = (UINT8)StrToInt(szArgv[1]);
        u4value = (UINT32)StrToInt(szArgv[2]);
        switch (u1Item)
        {
            case 0:
                AUD_DspADVSetChGain(0,u4value);
                break;
            case 1:
                AUD_DspADVSetChGain (1,u4value);
                break;
          default:
              Printf("Invalid channel \n");
               return -1;

        }
    }
    UNUSED(i4Argc);
    UNUSED(szArgv);

    return 1;
}

static INT32 _AudCmdUopABXConfig(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1ItemId;
    UINT8 u1Val;

    if (i4Argc != 3)
    {
        Printf("Usage: abxcfg [item] [val]\n");
        AUD_DspABXQuery();
        return -1;
    }

    u1ItemId = (UINT8)StrToInt(szArgv[1]);
    u1Val = (UINT8)StrToInt(szArgv[2]);
    AUD_DspABXConfig(u1ItemId,u1Val);
    return 0;
}

static INT32 _AudCmdUopAeqFlag(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Enable;

    if (i4Argc < 3)
    {
        AUD_DspAEQQuery();
        Printf("\nUsage: aeq [decoder id] [on/off]\n");
        Printf("[decoder id]  0:FIRST  1:SECOND \n");
        Printf("[on/off]      1: on 0: off\n");
        return 1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    AUD_DspAEQEnable(u1DecId, (BOOL)u1Enable);

    return 1;
}
#endif

static INT32 _AudCmdMemAlloc(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Size = 0;
    static UINT8* pMemPlayBuf = 0;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc != 2)
    {
        Printf("Usage: t2 [Length]\n");
        Printf(" Length = 0 for free\n");
        return -1;
    }

    u4Size = (UINT32)StrToInt(szArgv[1]);
    if (u4Size == 0)
    {
        VERIFY(BSP_FreeAlignedDmaMemory((UINT32)pMemPlayBuf));
        Printf( "Free memory 0x%x \n",pMemPlayBuf);
        pMemPlayBuf = 0;
        return 0;
    }
    else
    {
        VERIFY(BSP_FreeAlignedDmaMemory((UINT32)pMemPlayBuf));
        Printf( "Free memory 0x%x \n",pMemPlayBuf);
    }

    Printf( "Allocate memory size 0x%x for memory play...\n",u4Size);
    pMemPlayBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, 32);
    if (pMemPlayBuf == NULL)
    {
        printf( "Memory request for memory play fail !!\n");
        return -1;
    }

    Printf("Allocate memory from 0x%x(0x%x) to 0x%x(0x%x) Success, len = 0x%x\n",pMemPlayBuf,VIRTUAL((UINT32)pMemPlayBuf),pMemPlayBuf+u4Size,VIRTUAL((UINT32)pMemPlayBuf+u4Size),u4Size);
    Printf( "data.load.binary FILENAME 0x%x--0x%x /ny /swap \n",VIRTUAL((UINT32)pMemPlayBuf),VIRTUAL((UINT32)pMemPlayBuf+u4Size));

    return 0;

}

#ifdef CC_AUD_SKYPE_SUPPORT
static INT32 _AudSkypeMixRingSnd(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1MixFlag;

    if (i4Argc != 2)
    {
        Printf("Usage: skprmix   [off/on]                \n");
        Printf("skprmix    0/1              \n");
        return -1;
    }

    u1MixFlag = (UINT8)StrToInt(szArgv[1]);
    AUD_DspSkypeRingMix((BOOL)u1MixFlag);
    return 1;
}
static INT32 _AudSkypeAuxMix2Main(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1MixFlag;

    if (i4Argc != 2)
    {
        Printf("Usage: skptxmix   [off/on]                \n");
        Printf("skprmix    0/1              \n");
        return -1;
    }

    u1MixFlag = (UINT8)StrToInt(szArgv[1]);
    AUD_DspAuxMixToMain((BOOL)u1MixFlag);
    return 1;
}
#endif

#ifdef CC_AUD_PCM_LINE_IN_LPF
static INT32 _AudDSPSetPCMLineInLPF(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag;

    if (i4Argc != 2)
    {
        Printf("Usage: PCM Line-in LPF.   [off/on]   \n");
        return -1;
    }

    u1Flag = (UINT8)StrToInt(szArgv[1]);
    AUD_DSPSetPCMLineInLPF((BOOL)u1Flag);
    return 1;
}
#endif

#ifdef CC_AUD_HDMI_SPDIF_CFG
static INT32 _AudHDMICfgSPDIFCLI(INT32 i4Argc, const CHAR **szArgv)
{
    AUD_OUTPUT_STATE u1OutputState;
    if (i4Argc < 2)
    {
        Printf("0: DigitalMute_AnalogMute.\n");
        Printf("1: DigitalUnMute_AnalogMute.\n");
        Printf("2: DigitalUnMute_AnalogUnMute.\n");
        Printf("3: DigitalMute_AnalogUnMute.\n");
    }
    u1OutputState = (AUD_OUTPUT_STATE)StrToInt(szArgv[1]);
    AUD_HdmiCfgAnalogOutput(u1OutputState);
    return 0;
}
#endif

#if 0 //For IC/FPGA Verification. //IC_FPGA_STV_VERIF
extern void AUD_DrvTestSTC(UINT8 u1StcId, UINT32 u4Value, BOOL fgUpdate);
static INT32 _AudCmdTestSTC(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Id = 0;
    UINT32 u4Vale = 0;

    if (i4Argc < 3)
    {
        Printf("stc : [id] [value]\n");
        AUD_DrvTestSTC(0,0, FALSE);
        return -1;
    }

    if (i4Argc == 3)
    {
        u1Id = (UINT8)StrToInt(szArgv[1]);
        u4Vale = (UINT32)StrToInt(szArgv[2]);
        AUD_DrvTestSTC(u1Id, u4Vale, TRUE);
    }

    return 0;
}
#endif

static CHAR * _aszAudOutputMode[] =
{
    "MONO",
    "MONO",                  /* 1/0 */
    "MONO+SAP",              /* 1+sub-language */
    "A2 DUAL",             /* 1+1 */
    "STEREO",                /* 2/0 */
    "STEREO+SAP",            /* 2+sub-language */
    "STEREO_DOLBY_SURROUND", /* 2/0, dolby surround */
    "SURROUND_2CH",          /* 2/1 */
    "SURROUND",              /* 3/1 */
    "3_0",                   /* 3/0 */
    "4_0",                   /* 2/2 */
    "5_1",                   /* 3/2 */
    "7_1",                   /* 5/2 */
    "FM_MONO_NICAM_MONO",
    "FM_MONO_NICAM_STEREO",
    "FM_MONO_NICAM_DUAL",
    "OTHERS",
    "5_0",                   /* 3/2 */
    "JOINT_STEREO",
    "FMRDO_MONO",
    "FMRDO_STEREO",

    "MONO_LFE",              /* 1/0.L */
    "DUAL_MONO_LFE",         /* 1+1.L */
    "STEREO_LFE",            /* 2/0.L */
    "SURROUND_2CH_LFE",      /* 2/1.L */
    "SURROUND_LFE",          /* 3/1.L */
    "3_1",                   /* 3/0.L */
    "4_1"                    /* 2/2.L */
};

static CHAR * _aszAudSys[] =
{
    "NONE_DETECTED ",
    "MTS           ",
    "FM_FM         ",
    "NTSC_M        ",
    "A2_BG         ",
    "A2_DK1        ",
    "A2_DK2        ",
    "A2_DK3        ",
    "PAL_I         ",
    "PAL_BG        ",
    "PAL_DK        ",
    "SECAM_L       ",
    "SECAM_L_PLUM  ",
    "SECAM_BG      ",
    "SECAM_DK    "
};

static INT32 _AudCmdAtvquery(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;
    UINT8 u1DecId;
    TV_AUD_SYS_T eTVAudSrcSys;

    //u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;
    u1DecId = _IsDualDecMode() ? AUD_DEC_AUX : AUD_DEC_MAIN;
    Printf("  \n");

    AUD_GetDecodeType(AUD_DSP0, u1DecId, &eStreamFrom, &eDecType);
    if  (eStreamFrom != AUD_STREAM_FROM_ANALOG_TUNER)
    {
        Printf("Current input source is not ATV\n");
        return -1 ;
    }
    UNUSED(eDecType);

    eTVAudSrcSys = AUD_DspGetTvSrcMode();

    Printf("Detected channel system:  %s\n",_aszAudSys[eTVAudSrcSys]);
    //Printf("Detected channel system:  %s\n",_aszAudSys[DSP_GetAtvTvSystem()]);
    Printf("Detected sound mode:      %s\n",_aszAudOutputMode[DSP_GetAudChlInfo(AUD_DEC_MAIN)]);
    return 1;
}
static INT32 _AudCmdAtvTvSys(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Sys;
    AUD_FMT_T eAudDecFormat;
    UINT8 u1DecId;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc < 2)
    {
        DSP_LogAtvTvSys();
        Printf("     \n");
        Printf("Usage: sys  [sys]\n");
        Printf("        1: BTSC          8: PAL_I\n");
        Printf("        2: FMFM          9: PAL_BG\n");
        Printf("        3: A2_KOREA     10: PAL_DK\n");
        Printf("        4: A2_BG        11: SECAM_L\n");
        Printf("        5: A2_DK1       12: SECAM_L' \n");
        Printf("        6: A2_DK2       13: SECAM_BG\n");
        Printf("        7: A2_DK3       14: SECAM_DK\n");

        return -1;
    }

    u1Sys = (UINT8)StrToInt(szArgv[1]);

    switch ((TV_AUD_SYS_T)u1Sys)
    {
        case SV_MTS:
            eAudDecFormat = AUD_FMT_MTS;
            break;
        case SV_FM_FM:
            eAudDecFormat = AUD_FMT_FMFM;
            break;
        case SV_NTSC_M:
        case SV_A2_BG:
        case SV_A2_DK1:
        case SV_A2_DK2:
        case SV_A2_DK3:
            eAudDecFormat = AUD_FMT_A2;
            break;
        case SV_PAL_I:
        case SV_PAL_BG:
        case SV_PAL_DK:
        case SV_SECAM_L:
        case SV_SECAM_L_PLUM:
        case SV_SECAM_BG:
        case SV_SECAM_DK:
            eAudDecFormat = AUD_FMT_PAL;
            break;
        default:
            Printf("Invalid sound system.\n");
            return -1;
            //break;
    }

        //u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX:AUD_DEC_MAIN;
        u1DecId = _IsDualDecMode() ? AUD_DEC_AUX:AUD_DEC_MAIN;
        AUD_DSPCmdStop(AUD_DSP0, u1DecId);
        AUD_DspSetTvSrcMode((TV_AUD_SYS_T)u1Sys);
        AUD_SetDecType(AUD_DSP0, u1DecId, AUD_STREAM_FROM_ANALOG_TUNER, eAudDecFormat);
        x_thread_delay(100);
        AUD_DSPCmdPlay(AUD_DSP0, u1DecId);

    return 1;
}

#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
static INT32 _AudCmdAtvAutoTvSys(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Sys,u1Country;
    AUD_FMT_T eAudDecFormat;
    UINT8 u1DecId;
    UINT32 u4TvSysMask, u4AudSysMask;
    ISO_3166_COUNT_T country_code;

    UNUSED(i4Argc);
    UNUSED(szArgv);
    if (i4Argc < 3)
    {
        DSP_LogAtvTvSys();
        Printf("     \n");
        Printf("Usage: asys  [sys] [country]\n");
        Printf("[sys] \n");
        Printf("        1: MTS          8: PAL_I\n");
        Printf("        2: FMFM          9: PAL_BG\n");
        Printf("        3: A2_KOREA     10: PAL_DK\n");
        Printf("        4: A2_BG        11: SECAM_L\n");
        Printf("        5: A2_DK1       12: SECAM_L' \n");
        Printf("        6: A2_DK2       13: SECAM_BG\n");
        Printf("        7: A2_DK3       14: SECAM_DK\n");
        Printf("[country] \n");
        Printf("        1: UK\n");
        Printf("        2: France\n");
        Printf("        3: Russia\n");
        Printf("        4: Others\n");

        return -1;
    }

    u1Sys = (UINT8)StrToInt(szArgv[1]);
    u1Country = (UINT8)StrToInt(szArgv[2]);
    switch ((TV_AUD_SYS_T)u1Sys)
    {
        case SV_A2_BG:
            u4TvSysMask = AUD_TV_SYS_MASK_B |
                                AUD_TV_SYS_MASK_G|
                                AUD_TV_SYS_MASK_A2  ;
            u4AudSysMask = AUD_SYS_MASK_FM_A2;
            break;
        case SV_A2_DK1:
            u4TvSysMask = AUD_TV_SYS_MASK_D |
                                AUD_TV_SYS_MASK_K |
                                AUD_TV_SYS_MASK_A2  ;
         u4AudSysMask = AUD_SYS_MASK_FM_A2_DK1;
            break;

        case SV_A2_DK2:
            u4TvSysMask = AUD_TV_SYS_MASK_D |
                                   AUD_TV_SYS_MASK_K |
                                   AUD_TV_SYS_MASK_A2  ;
            u4AudSysMask = AUD_SYS_MASK_FM_A2_DK2;
               break;
        case SV_A2_DK3:
            u4TvSysMask = AUD_TV_SYS_MASK_D |
                                   AUD_TV_SYS_MASK_K |
                                   AUD_TV_SYS_MASK_A2  ;
            u4AudSysMask = AUD_SYS_MASK_FM_A2;
               break;
        case SV_PAL_I:
         u4TvSysMask = AUD_TV_SYS_MASK_I |
                                AUD_TV_SYS_MASK_PAL;
         u4AudSysMask = AUD_SYS_MASK_FM_MONO |
                                                                AUD_SYS_MASK_NICAM;
            break;
        case SV_PAL_BG:
            u4TvSysMask = AUD_TV_SYS_MASK_B |
                                    AUD_TV_SYS_MASK_G |
                                    AUD_TV_SYS_MASK_PAL;
            u4AudSysMask = AUD_SYS_MASK_FM_MONO |
                                                                    AUD_SYS_MASK_NICAM;
            break;

        case SV_PAL_DK:
            u4TvSysMask = AUD_TV_SYS_MASK_D |
                                AUD_TV_SYS_MASK_K |
                                AUD_TV_SYS_MASK_PAL;
         u4AudSysMask = AUD_SYS_MASK_FM_MONO |
                                                                AUD_SYS_MASK_NICAM;
         break;
        case SV_SECAM_L:
        case SV_SECAM_L_PLUM:
        case SV_SECAM_BG:
        case SV_SECAM_DK:
         u4TvSysMask = AUD_TV_SYS_MASK_L |
                                                            AUD_TV_SYS_MASK_PAL;
         u4AudSysMask = AUD_SYS_MASK_FM_MONO |
                                                                AUD_SYS_MASK_NICAM;
            break;
        default:
            Printf("Invalid sound system.\n");
            return -1;
            //break;
    }

    if (u1Country == 1)
    {
        x_strncpy( (CHAR *) country_code, "GBR", ISO_3166_COUNT_LEN);

    }
    else if (u1Country == 2)
    {
        x_strncpy( (CHAR *) country_code, "FRA", ISO_3166_COUNT_LEN);
    }
    else if (u1Country == 3)
    {
        x_strncpy( (CHAR *) country_code, "RUS", ISO_3166_COUNT_LEN);
    }
    else if (u1Country == 4)
    {
        x_strncpy( (CHAR *) country_code, "DEU", ISO_3166_COUNT_LEN);
    }
    else
    {
        Printf("Invalid country.\n");
        return -1;
    }

        u1DecId = ((GetTargetCountry() == COUNTRY_EU)||(GetTargetCountry() == COUNTRY_BZ)) ? AUD_DEC_AUX : AUD_DEC_MAIN;
        AUD_DSPCmdStop(u1DecId);

        //do sound system detection
        Aud_SetTvAudSystemMask(AUD_DEC_MAIN, u4TvSysMask, u4AudSysMask, country_code);
#ifdef CC_AUD_DDI
        eAudDecFormat = AudAtvFmtDetection(u1DecId);
#else
        eAudDecFormat = AudAtvFmtDetection();
#endif
        LOG(0,"#### Detect TV Sys = %d #######\n ", eAudDecFormat);
        if (eAudDecFormat != AUD_FMT_UNKNOWN)
        {
            AUD_SetDecType(u1DecId, AUD_STREAM_FROM_ANALOG_TUNER, eAudDecFormat);
            AUD_DSPCmdPlay(u1DecId);
        }
    return 1;
}
#endif


static CHAR * _aszAudSoundMode[] =
{
    "MONO",
    "MONO",
    "STEREO",
    "SUB_LANG",
    "DUAL1",
    "DUAL2",
    "NICAM_MONO",
    "NICAM_STEREO",
    "NICAM_DUAL1",
    "NICAM_DUAL2",
    "A2_DUAL1_DUAL2",
    "NICAM_DUAL1_DUAL2",
    "FM_MONO",
    "FM_STEREO"
};

static INT32 _AudCmdAtvSoundMode(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1UserMode;
    UINT8 u1DecId;

    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;

    //u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;
    u1DecId = _IsDualDecMode() ? AUD_DEC_AUX : AUD_DEC_MAIN;
    Printf("  \n");

    AUD_GetDecodeType(AUD_DSP0, u1DecId, &eStreamFrom, &eDecType);
    if  (eStreamFrom != AUD_STREAM_FROM_ANALOG_TUNER)
    {
        Printf("Current input source is not ATV\n");
        return -1;
    }
    UNUSED(eDecType);

    if (i4Argc < 2)
    {
        Printf("Detected sound mode is  %s\n",_aszAudOutputMode[DSP_GetAudChlInfo(AUD_DEC_MAIN)]);
        Printf("Current sound mode is  %s\n\n",_aszAudSoundMode[AUD_DspGetSoundMode(AUD_DEC_MAIN)]);
        Printf("Usage: um [user mode]  \n");
        Printf("[user mode]     1:MONO        6:NICAM_MONO   \n");
        Printf("                2:STEREO      7:NICAM_STEREO  \n");
        Printf("                3:SAP         8:NICAM_DUAL1    \n");
        Printf("                4:A2_DUAL1    9:NICAM_DUAL2   \n");
        Printf("                5:A2_DUAL2   10:A2_DUAL1+A2_DUAL2   \n");

        return -1;
    }

    u1UserMode = (UINT8)StrToInt(szArgv[1]);
   AUD_DspSetAtvOutputMode(u1DecId, (AUD_SOUND_MODE_T)u1UserMode);

    return 1;
}

static INT32 _AudCmdFMRadioSoundMode(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1UserMode;
    UINT8 u1DecId;

    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;
    u1DecId = AUD_DEC_MAIN;
    Printf("  \n");

    AUD_GetDecodeType(AUD_DSP0, u1DecId, &eStreamFrom, &eDecType);
    if  (eStreamFrom != AUD_STREAM_FROM_ANALOG_TUNER)
    {
        Printf("Current input source is not FM radio\n");
        return -1;
    }
    UNUSED(eDecType);

    if (i4Argc < 2)
    {
        Printf("Detected sound mode is  %s\n",_aszAudOutputMode[DSP_GetAudChlInfo(AUD_DEC_MAIN)]);
        Printf("Current sound mode is  %s\n\n",_aszAudSoundMode[AUD_DspGetSoundMode(AUD_DEC_MAIN)]);
        Printf("Usage: fm2 [user mode]  \n");
        Printf("[user mode]     1:MONO        2:STEREO   \n");

        return -1;
    }

    u1UserMode = (UINT8)StrToInt(szArgv[1]);
    u1UserMode = u1UserMode + AUD_SOUND_MODE_FM_MONO - 1; // 0 is mono and 1 is stereo for fmrdodec
   AUD_DspSetAtvOutputMode(u1DecId, (AUD_SOUND_MODE_T)u1UserMode);

    return 1;
}
static INT32 _AudCmdAtvFacMts(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Item;
    UINT16 u2value;
    UINT8 u1DecId;
    UINT8 u1Tmp;

    //u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;
    u1DecId = _IsDualDecMode() ? AUD_DEC_AUX : AUD_DEC_MAIN;

    if (i4Argc >= 3)
    {
        u1Item = (UINT8)StrToInt(szArgv[1]);
        u2value = (UINT16)StrToInt(szArgv[2]);
        switch (u1Item)
            {
            case 0:
                AUD_DspSetMtsDetection(AUD_FACTORY_MTS_NUM_CHECK,(UINT16)u2value);
                break;
            case 1:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_NUM_PILOT,(UINT16)u2value);
                break;
            case 2:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_PILOT_HIGH,(UINT16)u2value);
                break;
            case 3:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_PILOT_LOW,(UINT16)u2value);
                break;
            case 4:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_NUM_SAP,(UINT16)u2value);
                break;
            case 5:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_SAP_HIGH,(UINT16)u2value);
                break;
            case 6:
                AUD_DspSetMtsDetection (AUD_FACTORY_MTS_SAP_LOW,(UINT16)u2value);
                break;
            case 7:
                AUD_DSPCmdStop(AUD_DSP0, u1DecId);
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT                
                AUD_DspSetHdevMode(AUD_ATV_DECODER_MTS, u2value);
#else
                AUD_DspSetHdevMode(AUD_ATV_DECODER_MTS, (UINT8)u2value);
#endif
                AUD_DSPCmdPlay(AUD_DSP0, u1DecId);
                break;
            case 8:
                AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_MTS, (BOOL)u2value);
                break;
            case 9:
                AUD_DspSetFMSatuMuteMode(AUD_ATV_DECODER_MTS, (BOOL)u2value);
                break;
            case 10:
                AUD_DspSetFMSatuMuteTh (AUD_FACTORY_SATU_MUTE_HIGHER,(UINT8)u2value);
                break;
            case 11:
                AUD_DspSetFMSatuMuteTh (AUD_FACTORY_SATU_MUTE_LOWER,(UINT8)u2value);
                break;
            case 12:
                AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE,(UINT8)u2value);
                break;
            case 13:
                AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)u2value);
                break;
            case 14:
                AUD_DspSetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_LOW,(UINT8)u2value);
                break;
            case 15:
                AUD_DspSetMtsMonoFineVolume((UINT8)u2value);
                break;
            case 16:
                AUD_DspSetSAPFineVolume((UINT8)u2value);
                break;
            case 17:
                AUD_DspSetMtsPilotDetection((BOOL)u2value);
                break;
            case 18:
                AUD_DspSetSapMute(AUD_FACTORY_SAP_MUTE_LOWER,(UINT8)u2value);
                break;
            case 19:
                AUD_DspSetSapMute(AUD_FACTORY_SAP_MUTE_HIGHER,(UINT8)u2value);
                break;
            case 20:
                AUD_DspSetSapMute(AUD_FACTORY_SAP_HP_MUTE_HIGHER,(UINT8)u2value);
                break;
            case 21:
                AUD_DspSetSapMute(AUD_FACTORY_SAP_HP_MUTE_LOWER,(UINT8)u2value);
                break;
            case 22:
                u2value = (u2value << 1) + (UINT16)StrToInt(szArgv[3]);
                AUD_DspSetSapMute(AUD_FACTORY_SAP_FILTER_SEL,(UINT8)u2value);
                break;
            case 23:
                AUD_DspSetSapMute(AUD_FACTORY_SAP_RATIO,(UINT8)u2value);
                break;

            default:
                break;
            }
        return 1;
    }
    else
    {
        Printf("Usage: mts {[item indx]} [value] \n");
        Printf("  \n");
        Printf("             [item indx]                    [value range][current value]      \n");
        Printf("    0  :  numbers of check                     30~80         %d     \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_NUM_CHECK));
        Printf("    1  :  numbers of pilot                      0~50         %d      \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_NUM_PILOT));
        Printf("    2  :  higher pilot threshold              120~200       %d    \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_PILOT_HIGH));
        Printf("    3  :  lower pilot threshold                80~150       %d     \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_PILOT_LOW));
        Printf("    4  :  numbers of SAP                        0~50         %d      \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_NUM_SAP));
        Printf("    5  :  higher SAP threshold                150~210       %d    \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_SAP_HIGH));
        Printf("    6  :  lower SAP threshold                  70~130       %d    \n",AUD_DspGetMtsDetection(AUD_FACTORY_MTS_SAP_LOW));
        Printf("    7  :  HDEV mode                            0 or 1         %d     \n",AUD_DspGetHdevMode(AUD_ATV_DECODER_MTS));
        Printf("    8  :  carrier shift function               0 or 1         %d      \n",AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_MTS));
        Printf("    9  :  FM saturation mute                   0 or 1         %d    \n",AUD_DspGetFMSatuMuteMode(AUD_ATV_DECODER_MTS));
        Printf("   10  :  higher saturation mute threshold     25~256        %d      \n",AUD_DspGetFMSatuMuteTh (AUD_FACTORY_SATU_MUTE_HIGHER));
        Printf("   11  :  lower saturation mute threshold       1~25         %d    \n",AUD_DspGetFMSatuMuteTh (AUD_FACTORY_SATU_MUTE_LOWER));
        Printf("   12  :  FM carrier mute mode                 0 or 1         %d     \n",AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE));
        Printf("   13  :  higher carrier mute threshold       110~180       %d    \n",AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_HIGH));
        Printf("   14  :  lower carrier mute threshold         20~80        %d   \n",AUD_DspGetFmMute(AUD_ATV_DECODER_MTS,AUD_FACTORY_FM_MUTE_LOW));
        Printf("   15  :  fine tune mono&stereo volume          0~40         %d     \n",AUD_DspGetMtsMonoFineVolume());
        Printf("   16  :  fine tune SAP volume                  0~40         %d    \n",AUD_DspGetSapFineVolume());
        Printf("   17  :  pilot offset detection               0 or 1         %d     \n",AUD_DspGetMtsPilotDetection());
        Printf("   18  :  SAP noise mute lower threshold       0~110         %d     \n",AUD_DspGetSapMute(AUD_FACTORY_SAP_MUTE_LOWER));
        Printf("   19  :  SAP noise mute higher threshold      0~256          %d     \n",AUD_DspGetSapMute(AUD_FACTORY_SAP_MUTE_HIGHER));
        Printf("   20  :  SAP HPF mute higher threshold       32~256          %d     \n",AUD_DspGetSapMute(AUD_FACTORY_SAP_HP_MUTE_HIGHER));
        Printf("   21  :  SAP HPF mute lower threshold         0~128         %d     \n",AUD_DspGetSapMute(AUD_FACTORY_SAP_HP_MUTE_LOWER));
        u1Tmp = AUD_DspGetSapMute(AUD_FACTORY_SAP_FILTER_SEL);
        Printf("   22  :  SAP Filter  [0:auto/1:manual] [0:big/1:small]  %d %d  \n", ((u1Tmp & 0x02)>>1), (u1Tmp & 0x01));
        Printf("   23  :  SAP ratio selection                0x0 ~ 0x30          %x     \n",AUD_DspGetSapMute(AUD_FACTORY_SAP_RATIO));
        return -1;
    }
}

static INT32 _AudCmdAtvFacA2(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Item;
    UINT16 u2value;

    if (i4Argc == 3)
    {
        u1Item = (UINT8)StrToInt(szArgv[1]);
        u2value = (UINT16)StrToInt(szArgv[2]);
        switch (u1Item)
            {
            case 0:
                AUD_DspSetA2Detection (AUD_FACTORY_A2_NUM_CHECK,(UINT16)u2value);
                break;
            case 1:
                AUD_DspSetA2Detection (AUD_FACTORY_A2_NUM_DOUBLE,(UINT16)u2value);
                break;
            case 2:
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT             
                AUD_DspSetHdevMode(AUD_ATV_DECODER_A2, u2value);
#else
                AUD_DspSetHdevMode(AUD_ATV_DECODER_A2, (UINT8)u2value);
#endif
                break;
            case 3:
                AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_A2, (BOOL)u2value);
                break;
            case 4:
                AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE,(UINT8)u2value);
                break;
            case 5:
                AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)u2value);
                break;
            case 6:
                AUD_DspSetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_LOW,(UINT8)u2value);
                break;
            case 7:
                AUD_DspSetA2FineVolume((UINT8)u2value);
                break;
            case 8:
                AUD_DspSetFMSatuMuteMode(AUD_ATV_DECODER_A2, (BOOL)u2value);
                break;
            case 9:
                AUD_DspSetNonEUMode((BOOL)u2value);
                break;
            case 10:
                AUD_DspSetA2Threshold(AUD_FACTORY_A2_MONO2STEREO,(UINT16)u2value);
                break;
            case 11:
                AUD_DspSetA2Threshold(AUD_FACTORY_A2_STEREO2MONO,(UINT16)u2value);
                break;
         case 12:
                AUD_DspSetA2Threshold(AUD_FACTORY_A2_WEAK_RF_LVL_MONO2STEREO,(UINT16)u2value);
                break;
            case 13:
                AUD_DspSetA2Threshold(AUD_FACTORY_A2_WEAK_RF_LVL_STEREO2MONO,(UINT16)u2value);
                break;
            default:
                break;
            }
        return 1;
    }
    else
    {
        Printf("Usage: a2 {[item indx]} [value] \n");
        Printf("  \n");
        Printf("             [item indx]                 [value range][current value]   \n");
        Printf("    0  :  numbers of check                   1~60          %d         \n",AUD_DspGetA2Detection (AUD_FACTORY_A2_NUM_CHECK));
        Printf("    1  :  numbers of double check            5~60          %d        \n",AUD_DspGetA2Detection (AUD_FACTORY_A2_NUM_DOUBLE));
        Printf("    2  :  HDEV mode                         0 or 1          %d      \n",AUD_DspGetHdevMode(AUD_ATV_DECODER_A2));
        Printf("    3  :  carrier shift function            0 or 1          %d    \n",AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_A2));
        Printf("    4  :  FM carrier mute mode              0 or 1          %d    \n",AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE));
        Printf("    5  :  higher carrier mute threshold      0~96          %d  \n",AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_HIGH));
        Printf("    6  :  lower carrier mute threshold       0~96          %d     \n",AUD_DspGetFmMute(AUD_ATV_DECODER_A2,AUD_FACTORY_FM_MUTE_LOW));
        Printf("    7  :  fine tune A2 volume                0~40          %d    \n",AUD_DspGetA2FineVolume());
        Printf("    8  :  FM saturation mute                0 or 1          %d  \n",AUD_DspGetFMSatuMuteMode(AUD_ATV_DECODER_A2));
        Printf("    9  :  non-EU mode                      0 or 1          %d      \n",AUD_DspGetNonEUMode());
        Printf("   10  :  mono to stereo threshold(pilot)         64~1024     %d     \n",AUD_DspGetA2Threshold(AUD_FACTORY_A2_MONO2STEREO));
        Printf("   11  :  stereo to mono threshold(pilot)          0~512        %d      \n",AUD_DspGetA2Threshold(AUD_FACTORY_A2_STEREO2MONO));
        Printf("   12  :  mono to stereo threshold(weak RF)   0~57344     %d     \n",AUD_DspGetA2Threshold(AUD_FACTORY_A2_WEAK_RF_LVL_MONO2STEREO));
        Printf("   13  :  stereo to mono threshold(weak RF)   256~65535        %d      \n",AUD_DspGetA2Threshold(AUD_FACTORY_A2_WEAK_RF_LVL_STEREO2MONO));
        return -1;
    }
}

static INT32 _AudCmdAtvFacPal(INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1Item;
    UINT16 u2value;
    UINT8 u1DecId;

    //u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;
    u1DecId = _IsDualDecMode() ? AUD_DEC_AUX : AUD_DEC_MAIN;

    if (i4Argc == 3)
    {
        u1Item = (UINT8)StrToInt(szArgv[1]);
        u2value = (UINT16)StrToInt(szArgv[2]);
        switch (u1Item)
            {
            case 0:
                AUD_DspSetPalDetection (AUD_FACTORY_PAL_PARITY_ERROR,(UINT16)u2value);
                break;
            case 1:
                AUD_DSPCmdStop(AUD_DSP0, u1DecId);
#ifndef CC_AUD_WAFFLE_OVM_SUPPORT                    
                AUD_DspSetHdevMode(AUD_ATV_DECODER_PAL, u2value);
#else
                AUD_DspSetHdevMode(AUD_ATV_DECODER_PAL, (UINT8)u2value);
#endif
                AUD_DSPCmdPlay(AUD_DSP0, u1DecId);
                break;
            case 2:
                AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE,(UINT8)u2value);
                break;
            case 3:
                AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE_HIGH,(UINT8)u2value);
                break;
            case 4:
                AUD_DspSetAmMute(AUD_FACTORY_PAL_AM_MUTE_LOW,(UINT8)u2value);
                break;
            case 5:
                AUD_DspSetCarrierShiftMode(AUD_ATV_DECODER_PAL, (BOOL)u2value);
                break;
            case 6:
                AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE,(UINT8)u2value);
                break;
            case 7:
                AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_HIGH,(UINT8)u2value);
                break;
            case 8:
                AUD_DspSetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_LOW,(UINT8)u2value);
                break;
            case 9:
                AUD_DspSetPALFineVolume((UINT8)u2value);
                break;
            case 10:
                AUD_DspSetAMFineVolume((UINT8)u2value);
                break;
            case 11:
                AUD_DspSetNicamFineVolume((UINT8)u2value);
                break;
            case 12:
                AUD_DspSetFMSatuMuteMode(AUD_ATV_DECODER_PAL,(BOOL)u2value);
                break;
            case 13:
                AUD_DspSetNonEUMode((BOOL)u2value);
                break;
            default:
                break;
            }
        return 1;
    }
    else
    {
        if (i4Argc == 2)
        {
            u1Item = (UINT8)StrToInt(szArgv[1]);
            if (u1Item == 14)
            {
                UNUSED(AUD_GetNicamStatus());
                return 1 ;
            }
        }
        Printf("Usage: pal {[item indx]} [value] \n");
        Printf("  \n");
        Printf("             [item indx]                [value range]  [current value]     \n");
        Printf("    0  :  parity error threshold           600~4096      %d    \n",AUD_DspGetPalDetection (AUD_FACTORY_PAL_PARITY_ERROR));
        Printf("    1  :  HDEV mode                        0 or 1           %d     \n",AUD_DspGetHdevMode(AUD_ATV_DECODER_PAL));
        Printf("    2  :  AM mute mode                     0 or 1           %d     \n",AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE));
        Printf("    3  :  higher AM mute threshold          0~255          %d    \n",AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE_HIGH));
        Printf("    4  :  lower AM mute threshold           0~255          %d  \n",AUD_DspGetAmMute(AUD_FACTORY_PAL_AM_MUTE_LOW));
        Printf("    5  :  carrier shift function           0 or 1           %d   \n",AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_PAL));
        Printf("    6  :  FM carrier mute mode             0 or 1           %d  \n",AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE));
        Printf("    7  :  higher carrier mute threshold     0~96           %d   \n",AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_HIGH));
        Printf("    8  :  lower carrier mute threshold      0~96           %d    \n",AUD_DspGetFmMute(AUD_ATV_DECODER_PAL,AUD_FACTORY_FM_MUTE_LOW));
        Printf("    9  :  fine tune FM volume               0~40           %d    \n",AUD_DspGetPALFineVolume());
        Printf("   10  :  fine tune AM volume               0~40           %d    \n",AUD_DspGetAMFineVolume());
        Printf("   11  :  fine tune NICAM volume            0~40           %d  \n",AUD_DspGetNicamFineVolume());
        Printf("   12  :  FM saturation mute               0 or 1           %d  \n",AUD_DspGetFMSatuMuteMode(AUD_ATV_DECODER_PAL));
        Printf("   13  :  non-EU mode                      0 or 1           %d  \n",AUD_DspGetNonEUMode());
        Printf("   14  :  check NICAM status            read only     ");
        UNUSED(AUD_GetNicamStatus());
        Printf("  \n");
        return -1;
    }
}

static CHAR * _aszAudFormat[] = {
    "UNKNOWN",
    "MPEG",
    "AC3",
    "PCM",
    "MP3",
    "AAC",
    "DTS",
    "WMA",
    "RA",
    "HDCD",
    "MLP"
};

/* Audio types. */
static CHAR * _aszAudACMode[] =
{
    "UNKNOWN",
    "MONO",
    "MONO_SUB",
    "DUAL_MONO",
    "STEREO",
    "STEREO_SUB",
    "STEREO_DOLBY_SURROUND",
    "SURROUND_2CH",
    "SURROUND",
    "3.0",
    "4.0",
    "5.1",
    "7.1",
    "UNKNOWN",
    "UNKNOWN",
    "UNKNOWN",
    "OTHERS",
    "5.0"
} ;

static INT32 _AudCmdAtvHdevDetQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 HdevDetResult;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    HdevDetResult = AudCmdAtvHdevDetQuery();
    Printf("CLI Result, HdevDetResult = %d!\n", HdevDetResult);

    return 1;

}

static INT32 _AudCmdDtvQuery (INT32 i4Argc,const CHAR** szArgv)
{
    UINT8 u1DecId;
    AUD_DEC_INFO_T  rAudInfo;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;

    //u1DecId = (AUD_GetTargetCountry() == COUNTRY_EU) ? AUD_DEC_AUX : AUD_DEC_MAIN;
    u1DecId = _IsDualDecMode() ? AUD_DEC_AUX : AUD_DEC_MAIN;
    Printf("  \n");

    AUD_GetDecodeType(AUD_DSP0, u1DecId, &eStreamFrom, &eDecType);
    if  (eStreamFrom != AUD_STREAM_FROM_DIGITAL_TUNER)
    {
        Printf("Current input source is not DTV\n");
        return -1 ;
    }

    if (!AUD_IsDecoderPlay(AUD_DSP0, u1DecId))
    {
        Printf("There is no audio decoder playing \n");
        return -1 ;
    }

    UNUSED(eDecType);

    AUD_GetDtvAudInfo(u1DecId, &rAudInfo);

    if (rAudInfo.e_aud_fmt > AUD_FMT_MLP)
    {
        rAudInfo.e_aud_fmt = AUD_FMT_UNKNOWN;
    }

    Printf(" Current audio format:  %s  \n",_aszAudFormat[rAudInfo.e_aud_fmt]);
    if(rAudInfo.e_aud_fmt == AUD_FMT_AC3)
    {
        Printf(" Extra info. of AC3: \n ");
     Printf("  >>>Current audio decode type: ");
        switch(rAudInfo.e_dec_type)
        {
        case AUD_DECODE_TYPE_AC3:
         Printf("AC3 \n");
         break;
     case AUD_DECODE_TYPE_EAC3:
         Printf("E-AC3 \n");
         break;
        default:
         Printf("Unknown \n");
         break;
        }
        Printf("   >>>Current stream");
        switch (rAudInfo.e_aud_type)
        {
        case AUD_TYPE_DUAL_MONO: // 1+1
            Printf("(1+1) contains no LFE \n");
            break;
        case AUD_TYPE_MONO: // 1/0
            Printf("(1/0) contains no LFE \n");
            break;
        case AUD_TYPE_STEREO: // 2/0
            Printf("(2/0) contains no LFE \n");
            break;
        case AUD_TYPE_3_0: // 3/0
            Printf("(3/0) contains no LFE \n");
            break;
        case AUD_TYPE_SURROUND_2CH: // 2/1
            Printf("(2/1) contains no LFE \n");
            break;
        case AUD_TYPE_SURROUND: // 3/1
            Printf("(3/1) contains no LFE \n");
            break;
        case AUD_TYPE_4_0: // 2/2
            Printf("(2/2) contains no LFE \n");
            break;
        case AUD_TYPE_5_0: // 3/2
            Printf("(3/2) contains no LFE \n");
            break;
        case AUD_TYPE_MONO_LFE: // 1/0.L
            Printf("(1/0.L) contains LFE \n");
            break;
        case AUD_TYPE_STEREO_LFE: // 2/0.L
            Printf("(2/0.L) contains LFE \n");
            break;
        case AUD_TYPE_3_1: // 3/0.L
            Printf("(3/0.L) contains LFE \n");
            break;
        case AUD_TYPE_SURROUND_2CH_LFE: // 2/1.L
            Printf("(2/1.L) contains LFE \n");
            break;
        case AUD_TYPE_SURROUND_LFE: // 3/1.L
            Printf("(3/1.L) contains LFE \n");
            break;
        case AUD_TYPE_4_1: // 2/2.L
            Printf("(2/2.L) contains LFE \n");
            break;
        case AUD_TYPE_5_1: // 3/2.L
            Printf("(3/2.L) contains LFE \n");
            break;
        default: //
            Printf(" contains no LFE \n");
            break;
        }
    }
    Printf(" Sampling rate:         %d Hz  \n",rAudInfo.ui4_sample_rate);
    Printf(" Data rate:             %d bps\n",rAudInfo.ui4_data_rate);
    Printf(" Audio decoding mode:   %s\n",_aszAudACMode[rAudInfo.e_aud_type]);

    return 1 ;
}

static INT32 _AudCmdDtvPTSDelay(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DecId;
    UINT8 uDelayFactor;

    if (i4Argc < 3)
    {
        Printf("Usage: PTSD [decoder id] [factor]\n");
        Printf("[decoder id]   0:FIRST                 1:SECOND \n");
        Printf("[factor] 0 ~ 255 \n\n");
        Printf("Audio start decoding PTS would increased (PTS_AC3_CNT * factor)\n");
        return -1;
    }
    else
    {
        u1DecId = StrToInt(szArgv[1]);
        uDelayFactor = StrToInt(szArgv[2]);
        AUD_AudioPTSDelayEnable(u1DecId, uDelayFactor);
        Printf(" Set DelayFactor to Dec(%d) = %d\n", u1DecId, uDelayFactor);
        return 1;
    }
}

static INT32 _AudCmdDtvDolbyLicenseSupport(INT32 i4Argc, const CHAR **szArgv)
{
    Printf("\n");
#if defined(CC_AUD_DOLBY_SUPPORT_AAC)
    Printf("Dolby Tech. Support: CC_AUD_DOLBY_SUPPORT_AAC\n");
#endif
#if defined(CC_AUD_DOLBY_SUPPORT_DDC)
    Printf("Dolby Tech. Support: CC_AUD_DOLBY_SUPPORT_DDC\n");
#endif
#if defined(CC_AUD_DOLBY_SUPPORT_DDT)
        Printf("Dolby Tech. Support: CC_AUD_DOLBY_SUPPORT_DDT\n");
#endif
#if defined(CC_AUD_DOLBY_SUPPORT_DDP)
        Printf("Dolby Tech. Support: CC_AUD_DOLBY_SUPPORT_DDP\n");
#endif
#if defined(CC_AUD_DOLBY_SUPPORT_DD)
        Printf("Dolby Tech. Support: CC_AUD_DOLBY_SUPPORT_DD\n");
#endif
#if defined(CC_AUD_DOLBY_SUPPORT_DDCO)
        Printf("Dolby Tech. Support: CC_AUD_DOLBY_SUPPORT_DDCO\n");
#endif

#if defined(CC_AUD_DOLBY_SUPPORT_AAC) && !defined(CC_AUD_DOLBY_SUPPORT_DDC) && !defined(CC_AUD_DOLBY_SUPPORT_DDT) && !defined(CC_AUD_DOLBY_SUPPORT_DDP) && !defined(CC_AUD_DOLBY_SUPPORT_DD) && !defined(CC_AUD_DOLBY_SUPPORT_DDCO)
    Printf("Dolby License Support: NON\n");
#elif !defined(CC_AUD_DOLBY_SUPPORT_AAC) && defined(CC_AUD_DOLBY_SUPPORT_DDC) && defined(CC_AUD_DOLBY_SUPPORT_DDT) && !defined(CC_AUD_DOLBY_SUPPORT_DDP) && !defined(CC_AUD_DOLBY_SUPPORT_DD) && !defined(CC_AUD_DOLBY_SUPPORT_DDCO)
    Printf("Dolby License Support: MS10\n");
#elif defined(CC_AUD_DOLBY_SUPPORT_AAC) && !defined(CC_AUD_DOLBY_SUPPORT_DDC) && !defined(CC_AUD_DOLBY_SUPPORT_DDT) && defined(CC_AUD_DOLBY_SUPPORT_DDP) && !defined(CC_AUD_DOLBY_SUPPORT_DD) && !defined(CC_AUD_DOLBY_SUPPORT_DDCO)
    Printf("Dolby License Support: DCV\n");
#elif defined(CC_AUD_DOLBY_SUPPORT_AAC) && !defined(CC_AUD_DOLBY_SUPPORT_DDC) && !defined(CC_AUD_DOLBY_SUPPORT_DDT) && !defined(CC_AUD_DOLBY_SUPPORT_DDP) && defined(CC_AUD_DOLBY_SUPPORT_DD) && !defined(CC_AUD_DOLBY_SUPPORT_DDCO)
    Printf("Dolby License Support: DD\n");
#elif !defined(CC_AUD_DOLBY_SUPPORT_AAC) && !defined(CC_AUD_DOLBY_SUPPORT_DDC) && defined(CC_AUD_DOLBY_SUPPORT_DDT) && !defined(CC_AUD_DOLBY_SUPPORT_DDP) && !defined(CC_AUD_DOLBY_SUPPORT_DD) && !defined(CC_AUD_DOLBY_SUPPORT_DDCO)
    Printf("Dolby License Support: DP\n");
#elif defined(CC_AUD_DOLBY_SUPPORT_AAC) && !defined(CC_AUD_DOLBY_SUPPORT_DDC) && !defined(CC_AUD_DOLBY_SUPPORT_DDT) && !defined(CC_AUD_DOLBY_SUPPORT_DDP) && !defined(CC_AUD_DOLBY_SUPPORT_DD) && defined(CC_AUD_DOLBY_SUPPORT_DDCO)
    Printf("Dolby License Support: DDCO\n");
#elif defined(CC_AUD_DOLBY_SUPPORT_AAC) && !defined(CC_AUD_DOLBY_SUPPORT_DDC) && !defined(CC_AUD_DOLBY_SUPPORT_DDT) && defined(CC_AUD_DOLBY_SUPPORT_DDP) && !defined(CC_AUD_DOLBY_SUPPORT_DD) && defined(CC_AUD_DOLBY_SUPPORT_DDCO)
    Printf("Dolby License Support: DCV+DDCO\n");
#elif defined(CC_AUD_DOLBY_SUPPORT_AAC) && !defined(CC_AUD_DOLBY_SUPPORT_DDC) && !defined(CC_AUD_DOLBY_SUPPORT_DDT) && !defined(CC_AUD_DOLBY_SUPPORT_DDP) && defined(CC_AUD_DOLBY_SUPPORT_DD) && defined(CC_AUD_DOLBY_SUPPORT_DDCO)
    Printf("Dolby License Support: DD+DDCO\n");
#elif !defined(CC_AUD_DOLBY_SUPPORT_AAC) && !defined(CC_AUD_DOLBY_SUPPORT_DDC) && defined(CC_AUD_DOLBY_SUPPORT_DDT) && !defined(CC_AUD_DOLBY_SUPPORT_DDP) && defined(CC_AUD_DOLBY_SUPPORT_DD) && !defined(CC_AUD_DOLBY_SUPPORT_DDCO)
    Printf("Dolby License Support: DD+DP\n");
#endif

    Printf("\n");

    return 1;
}

#ifdef NEW_COUNTRY_TYPE
static INT32 _AudCmdDtvTargetCountryQuery(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Country;

    u4Country = AUD_GetTargetCountry();
    switch(u4Country)
    {
    case COUNTRY_US:
        Printf("\nCurrent Target Country: COUNTRY_US \n\n");
        break;
    case COUNTRY_EU:
        Printf("\nCurrent Target Country: COUNTRY_EU \n\n");
        break;
    case COUNTRY_TW:
        Printf("\nCurrent Target Country: COUNTRY_TW \n\n");
        break;
    case COUNTRY_JP:
        Printf("\nCurrent Target Country: COUNTRY_JP \n\n");
        break;
    case COUNTRY_BZ:
        Printf("\nCurrent Target Country: COUNTRY_BZ \n\n");
        break;
    default:
        Printf("\nCurrent Target Country: Unknown(default: COUNTRY_EU) \n\n");
        break;
    }

    return 1;
}
#endif

#ifdef CC_MPEG_LEVEL_ADJ
static INT32 _AudCmdSetMpegLevelAdj(INT32 i4Argc, const CHAR **szArgv)
{
   UINT8 u1Level;
  
    if (i4Argc < 2)
    {
        Printf("Usage: mpglvl [level]\n");             
        Printf("[level] 0 ~ 10 \n\n");
        Printf("DTV Mpeg Level Adjust\n");
        return -1;       
    }

    if (i4Argc >= 2)
    {   
        u1Level = StrToInt(szArgv[1]);        
        AUD_MpegLevelAdjust(u1Level);  
        Printf(" Set u1Level to %d\n", u1Level);
    }    

    return 1;
}
#endif

static INT32 _AudCmdFMRadioDetect (INT32 i4Argc, const CHAR **szArgv)
{
    BOOL fm_det_result = 0;

    UNUSED(i4Argc);
    UNUSED(szArgv);
    fm_det_result = _AudFMRadioDetection();
    if (fm_det_result) Printf("CLI Result : FM channel detected!\n");
    else Printf("CLI Result : None FM channel detected!\n");

    return 1;
}

static INT32 _AudCmdFMRadioPlay (INT32 i4Argc, const CHAR **szArgv)
{

    UNUSED(i4Argc);
    UNUSED(szArgv);
    _AudPlayFMRadio();

    return 1;
}

static INT32 _AudCmdFMRadioQueryThre(INT32 i4Argc, const CHAR ** szArgv)
    {
         UINT32 u4Thre1,u4Thre2;
         INT32 i4Thre;

         u4Thre1 = vDspGetFMRadioNoiseThreshold();
         u4Thre2 = vDspGetFMRadioInBandThreshold();
         i4Thre = vDspGetFMRadioRatioThreshold();

         Printf("Noise Power Threshold = 0x%x \n",u4Thre1);
         Printf("In Band Power Threshold = 0x%x \n",u4Thre2);
         Printf("Ratio = %d \n",i4Thre);

         return 1;
    }

static INT32 _AudCmdFMRadioSetFineVolume(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u1Vol;

    if (i4Argc < 2)
    {
        Printf("Usage: vol [vol index]\n");
        Printf("[vol index] 0 ~ 40, -10 ~ 10dB, 0.5dB per step\n");
        return -1;
    }

    u1Vol = (UINT8)StrToInt(szArgv[1]);
    AUD_DspSetFmRadioFineVolume((UINT8)u1Vol);

    return 1;
}

static INT32 _AudCmdFMRadioEnableNotify(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Flag;

    if (i4Argc < 2)
    {
        Printf("Usage: en [flag]\n");
        Printf("[flag] 0: disable, 1: enable\n"); 
        return -1;  
    }
         
    u1Flag = (UINT8)StrToInt(szArgv[1]);
    _fgEnableFMDetNotify = u1Flag;

    Printf("_fgEnableFMDetNotify = %d\n", _fgEnableFMDetNotify);    
        
    return 1;    
}

static INT32 _AudCmdFMRadioThre(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 uType;
    UINT32 u4Thre;

    if (i4Argc < 3)
    {
        Printf("Usage: fmthl [Type] [threshold]\n");
        Printf("[Type] 1: Noise Power   2: In band Power  3: Ratio\n");
        Printf("[threshold]     0x0 ~ 0x7fffff\n");
        return -1;
    }

    uType = (UINT8)StrToInt(szArgv[1]);
    u4Thre = (UINT32)StrToInt(szArgv[2]);
    if(uType == 1)
        vDspSetFMRadioNoiseThreshold(u4Thre);
    if(uType == 2)
        vDspSetFMRadioInBandThreshold(u4Thre);
    if(uType == 3)
        vDspSetFMRadioRatioThreshold((int)u4Thre);

    return 1;
}


static INT32 _AUD_MTS_CFO_IDX(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_MTS_CFO_IDX();

    return 1;
}

#if 0
#define AC3_KARA_DISABLE    ((UINT16)0x0001) /* disable karaoke mode */
#define AC3_KARA_AWARE      ((UINT16)0x0002) /* karaoke aware mode */
#define AC3_KARA_NONE       ((UINT16)0x0004) /* none (in capable mode)*/
#define AC3_KARA_V1         ((UINT16)0x0008) /* V1 only (in capable mode)*/
#define AC3_KARA_V2         ((UINT16)0x0010) /* V2 only (in capable mode)*/
#define AC3_KARA_BOTH       ((UINT16)0x0020) /* V1+V2   (in capable mode)*/
#define AC3_KARA_VOC_ASIST  ((UINT16)(0x0040) /* vocal assist mode */
#define AC3_KARA_NO_MELODY  ((UINT16)0x0080) /* melody off */
#endif

#ifdef CC_VOCAL_MUTE
static INT32 _AudCmd_VocalMuteEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Enable;

    if (i4Argc < 2)
    {
        Printf("Usage: Vocal Mute [ON/OFF] \n");
        Printf("[ON/OFF]     0:OFF   1:ON \n");
        return -1;
    }    
    u1Enable = (UINT8)StrToInt(szArgv[1]);

    AUD_VocalMuteOnOff(1, (BOOL)u1Enable);
    Printf(" Set Vocal Mute to %d\n", u1Enable);

    return 0;
}
///vocal mute (L)&(R) path filter
#define ADDR_RC2D_VMF_GAIN          (0x121C)   /// 0921Ch, 1, same with ADDR_RC2D_KARA_VOICE_MUTE_BSF_GAIN
#define ADDR_RC2D_VMF2_GAIN         (0x121D)   /// 0921Dh, 1, same with ADDR_RC2D_KARA_VOICE_MUTE_BPF_GAIN
static INT32 _AudCmd_VocalMuteFilter1(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Idx;
    UINT32 u4Gain;

    u1Idx = AUD_GetVocaleMuteFilter1();
    if (i4Argc < 2)
    {
        Printf("Usage: vocal mute (L)&(R) path filter [Idx] \n");
        Printf("       [Idx] = 0 ~ 5 \n");
        Printf("       [Gain] 0x20000 is 0dB and [Idx] must equal '9' \n");
        Printf(" 0 : BSF 70~7000Hz \n");
        Printf(" 1 : BSF 70~10000Hz \n");
        Printf(" 2 : BSF 70~5000Hz \n");
        Printf(" 3 : BSF 100~7000Hz \n");
        Printf(" 4 : BSF 120~7000Hz \n");
        Printf(" 5 : HPF 3000Hz \n");        
        Printf(" 9 : Set Gain. (0x20000 is 0dB)\n");
        u4Gain = DSP_CommDramRead(ADDR_RC2D_VMF_GAIN);
        Printf(" Current [Idx]=%d, [Gain]=0x%x. \n", u1Idx, u4Gain);
        return -1;
    }

    u1Idx = (UINT16)StrToInt(szArgv[1]);
    if((u1Idx == 9) && (i4Argc != 3))
    {
        Printf("Usage: [Idx]=9 [Gain]=??? \n");
        Printf("       [Gain] 0x20000 is 0dB. \n");
        u4Gain = DSP_CommDramRead(ADDR_RC2D_VMF_GAIN);
        Printf(" Current [Gain]=0x%x. \n", u4Gain);
        return -1;
    }
    else if (u1Idx == 9)
    {
        u4Gain = (UINT32)StrToInt(szArgv[2]);
        Dsp_WriteDspCommDram(ADDR_RC2D_VMF_GAIN, u4Gain);
        return 0;
    }

    if (u1Idx >= (VM_FILTER1_NUM-1))
    {
        u1Idx = (VM_FILTER1_NUM-1);
    }    
    AUD_SetVoiceMuteFilter1(u1Idx);
    return 0;    
}
///vocal mute (L-R) path filter
static INT32 _AudCmd_VocalMuteFilter2(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1Idx;
    UINT32 u4Gain;

    u1Idx = AUD_GetVocaleMuteFilter2();
    if (i4Argc < 2)
    {
        Printf("Usage: vocal mute (L-R) path filter [Idx] \n");
        Printf("       [Idx] = 0 ~ 5 \n");
        Printf("       [Gain] 0x20000 is 0dB and [Idx] must equal '9' \n");
        Printf(" 0 : LPF 1~3000Hz(Default) \n");
        Printf(" 1 : LPF 1~1000Hz \n");
        Printf(" 2 : LPF 1~2000Hz \n");
        Printf(" 3 : LPF 1~3000Hz \n");
        Printf(" 4 : LPF 1~4000Hz \n");
        Printf(" 5 : FLAT 1~Fs/2Hz. \n");
        Printf(" 9 : Set Gain. (0x20000 is 0dB)\n");
        u4Gain = DSP_CommDramRead(ADDR_RC2D_VMF2_GAIN);
        Printf(" Current [Idx]=%d, [Gain]=0x%x. \n", u1Idx, u4Gain);
        return -1;
    }

    u1Idx = (UINT16)StrToInt(szArgv[1]);
    if((u1Idx == 9) && (i4Argc != 3))
    {
        Printf("Usage: [Idx]=9 [Gain]=??? \n");
        Printf("       [Gain] 0x20000 is 0dB. \n");
        u4Gain = DSP_CommDramRead(ADDR_RC2D_VMF2_GAIN);
        Printf(" Current [Gain]=0x%x. \n", u4Gain);
        return -1;
    }
    else if (u1Idx == 9)
    {
        u4Gain = (UINT32)StrToInt(szArgv[2]);
        Dsp_WriteDspCommDram(ADDR_RC2D_VMF2_GAIN, u4Gain);
        return 0;
    }

    if (u1Idx >= (VM_FILTER2_NUM-1))
    {
        u1Idx = (VM_FILTER2_NUM-1);
    }    
    AUD_SetVoiceMuteFilter2(u1Idx);
    return 0;    
}
#endif

static INT32 _AudCmdDtvAC3KaraMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT16 u2Mode;
    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST  ==> %d\n",AUD_DspGetAC3KaraMode(AUD_DEC_MAIN));
        Printf("               SECOND ==> %d\n",AUD_DspGetAC3KaraMode(AUD_DEC_AUX));
        Printf("               THIRD  ==> %d\n",AUD_DspGetAC3KaraMode(AUD_DEC_THIRD));
        Printf("  \n");
        Printf("Usage: karaMode [decoder id] [karaoke mode]\n");
        Printf("[decoder id]   0:FIRST                 1:SECOND \n");
        Printf("               2:THIRD \n");
        Printf("[karaoke mode] 0:DISABLE               1:AWARE \n");
        Printf("               2:NONE (capable mode)   3:V1 ONLY (capable mode) \n");
        Printf("               4:V2 ONLY (capable mode)5:V1+V2 (capable mode) \n");
        Printf("               other: wrong setting \n");
        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2Mode = (UINT16)StrToInt(szArgv[2]);

    if(u2Mode<6)
    {
        AUD_DspAC3KaraMode(0x1<<u2Mode, u2DecId);
    }

    return 1;
};

#ifdef CC_AUD_SUPPORT_MS10
static INT32 _AudCmdDtvDDCControl (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT16 u2Control;

    if (i4Argc < 3)
    {
        Printf("Usage: ddccontrol [decoder id] [control]\n");
        Printf("[decoder id]   0:FIRST                 1:SECOND \n");
        Printf("[control] DDC control \n");
        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2Control = (UINT16)StrToInt(szArgv[2]);

    AUD_DspSetDDCControl (u2Control, u2DecId);

    return 1;
};

static INT32 _AudCmdDtvDDCStrmId (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT16 u2StrmId;

    if (i4Argc < 3)
    {
        Printf("Usage: strmid [decoder id] [stream id]\n");
        Printf("[decoder id]   0:FIRST                 1:SECOND \n");
        Printf("[stream id] associated stream id \n");
        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2StrmId = (UINT16)StrToInt(szArgv[2]);

    AUD_DspSetDDCStrmId (u2StrmId, u2DecId);

    return 1;
};

static INT32 _AudCmdDtvDDCMixer (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT16 u2Mixer;

    if (i4Argc < 3)
    {
        Printf("Usage: mixer [decoder id] [stream id]\n");
        Printf("[decoder id]   0:FIRST                 1:SECOND \n");
        Printf("[mixer mode] 0: Main only, 1: Main+Assoc, 3: L = assoc, R = main \n");
        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2Mixer = (UINT16)StrToInt(szArgv[2]);

    AUD_DspSetDDCMixer (u2Mixer, u2DecId);

    return 1;
};
#endif

#if 0
#define AC3_DM_STEREO           0 /* AC3 dual mono mode stereo*/
#define AC3_DM_L_MONO           1 /* AC3 dual mono mode left mono*/
#define AC3_DM_R_MONO           2 /* AC3 dual mono mode right mono*/
#define AC3_DM_M_MONO           3 /* AC3 dual mono mode mix mono*/
#endif
static INT32 _AudCmdDtvAc3DualMono(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2DecId;
    UINT16 u2Mode;
    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST  ==> %d\n",AUD_DspGetAC3DualMono(AUD_DEC_MAIN));
        Printf("               SECOND ==> %d\n",AUD_DspGetAC3DualMono(AUD_DEC_AUX));
        Printf("               THIRD  ==> %d\n",AUD_DspGetAC3DualMono(AUD_DEC_THIRD));
        Printf("  \n");
        Printf("Usage: dualmono [decoder id] [dual mono mode]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("                 2:THIRD \n");
        Printf("[dual mono mode] 0:STEREO                1:L MONO \n");
        Printf("                 2:R MONO                3:MIX MONO\n");
        return -1;
    }
    u2DecId = (UINT16)StrToInt(szArgv[1]);
    u2Mode = (UINT16)StrToInt(szArgv[2]);

    if(u2Mode<5)
    {
        AUD_DspAC3DualMono(u2Mode, u2DecId);
    }

    return 1;
};

static INT32 _AudCmdDtvDualMono(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Mode;

    if (i4Argc < 3)
    {
        if ((i4Argc == 2) && (szArgv[1][0] == 'q'))
        {
            Printf("[Dual/Mono Setting]\n");
            Printf("  DEC1: %d\n", AUD_DspGetMatrixOutMode(AUD_DEC_MAIN));
            Printf("  DEC2: %d\n", AUD_DspGetMatrixOutMode(AUD_DEC_AUX));
        }
        Printf("Usage: dualmono [decoder id] [dual mono mode] or dualmono q\n");
        Printf("[decoder id]     0:FIRST                     1:SECOND \n");
        Printf("[dual mono mode] 0:STEREO (Disable)      1:L MONO \n");
        Printf("                 2:R MONO                3:MIX MONO\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Mode = (UINT8)StrToInt(szArgv[2]);

    if (u1Mode < 5)
    {
        AUD_DspSetMatrixOutMode(u1DecId, (AUD_MATRIX_OUT_T)u1Mode);
    }

    return 1;
};

static INT32 _AudCmdAdFadePanPesUpdate(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1AdUpdateEnable;
    if (i4Argc < 2)
    {
        Printf("Usage: adupdate [ad pan fade pes update enable]\n");
        Printf("[ad pan fade pes update enable]  0: disable, 1: enable\n");
        return -1;
    }

    u1AdUpdateEnable = (UINT8)StrToInt(szArgv[1]);

    if (u1AdUpdateEnable == 0)
    {
        AUD_DspPanFadePesUpdateEn(FALSE);
    }
    else if (u1AdUpdateEnable == 1)
    {
        AUD_DspPanFadePesUpdateEn(TRUE);
    }
    return 1;
}


static INT32 _AudCmdAdFade(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1Adfade;
    if (i4Argc < 3)
    {
        Printf("Usage: adfade [decoder id] [ad fade value]\n");
        Printf("[decoder id]     0:MAIN  1:AUX 2:THIRD\n");
        Printf("[adfade]  value 0~255\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1Adfade = (UINT8)StrToInt(szArgv[2]);

    AUD_DspAdFadeVolumeSet(u1Adfade);
    AUD_DspAdFadeVolumeUpdate(u1DecId, AUD_CH_ALL);
    return 1;
}

static INT32 _AudCmdAdPanFadeEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1AdFadeEnable;
    if (i4Argc < 2)
    {
        Printf("Usage: adpanfadeen [ad pan fade enable]\n");
        Printf("[ad pan fade enable]  0: disable, 1: enable\n");
        return -1;
    }

    u1AdFadeEnable = (UINT8)StrToInt(szArgv[1]);

    if (u1AdFadeEnable == 0)
    {
        AUD_DspAdPanFadeEnable(FALSE);
    }
    else if (u1AdFadeEnable == 1)
    {
        AUD_DspAdPanFadeEnable(TRUE);
    }
    return 1;
}

static INT32 _AudCmdAdPan(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT8 u1AdPan;
    if (i4Argc < 3)
    {
        Printf("Usage: adpan [decoder id] [ad fade value]\n");
        Printf("[decoder id]     0:MAIN 1:AUX 2:THIRD\n");
        Printf("[adpan]  value (0~255)\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1AdPan = (UINT8)StrToInt(szArgv[2]);

    if (u1AdPan & 0x80)
    {
        if (u1AdPan < 0xeb)
        {
            u1AdPan = 0xeb;
        }
        u1AdPan = 0x100 - u1AdPan;
        AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_RIGHT, u1AdPan);
        AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_LEFT, 0);
        AUD_DspAdPanVolumeUpdate(u1DecId, AUD_CH_FRONT_RIGHT);
        AUD_DspAdPanVolumeUpdate(u1DecId, AUD_CH_FRONT_LEFT);
    }
    else
    {
        if (u1AdPan > 0x15)
        {
            u1AdPan = 0x15;
        }
        AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_LEFT, u1AdPan);
        AUD_DspAdPanVolumeSet(u1DecId, AUD_CH_FRONT_RIGHT, 0);
        AUD_DspAdPanVolumeUpdate(u1DecId, AUD_CH_FRONT_RIGHT);
        AUD_DspAdPanVolumeUpdate(u1DecId, AUD_CH_FRONT_LEFT);
    }

    return 1;
}


#if 0
#define AC3_COMP_CUSTOM_0       0 /* AC3 compression mode custom 0*/
#define AC3_COMP_CUSTOM_1       1 /* AC3 compression mode custom 1*/
#define AC3_COMP_LINE           2 /* AC3 compression line mode P.S. default set as line mode*/
#define AC3_COMP_RF             3 /* AC3 compression RF mode*/
#define AC3_COMP_WLINE          4 /* AC3 compression wide range line mode*/
#endif
static INT32 _AudCmdDtvAc3CompMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT16 u2Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current mode : FIRST  ==> %d\n",AUD_DspGetAC3CompMode(AUD_DEC_MAIN));
        Printf("               SECOND ==> %d\n",AUD_DspGetAC3CompMode(AUD_DEC_AUX));
        Printf("               THIRD  ==> %d\n",AUD_DspGetAC3CompMode(AUD_DEC_THIRD));
        Printf("  \n");
        Printf("Usage: ac3cm [decoder id] [compression mode]\n");
        Printf("[decoder id]       0:FIRST                 1:SECOND \n");
        Printf("                   2:THIRD \n");
        Printf("[compression mode] 0:custom mode 0         1:custom mode 1\n");
        Printf("                   2:LINE mode             3:RF mode\n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u2Mode = (UINT16)StrToInt(szArgv[2]);

    if(u2Mode<4)
    {
        AUD_DspAC3CompMode(u2Mode, u1DecId);
    }
    return 1;
};
static INT32 _AudCmdDtvAc3DrcRange(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    UINT16 u2Mode;

    if (i4Argc < 3)
    {
        Printf("  \n");
        Printf("Current level : FIRST  ==> (%d) percentage\n",AUD_DspGetAC3DRCRange100(AUD_DEC_MAIN));
        Printf("                SECOND ==> (%d) percentage\n",AUD_DspGetAC3DRCRange100(AUD_DEC_AUX));
        Printf("                THIRD  ==> (%d) percentage\n",AUD_DspGetAC3DRCRange100(AUD_DEC_THIRD));
        Printf("  \n");
        Printf("Usage: ac3dr [decoder id] [percentage]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("                 2:THIRD \n");
        Printf("[range]          High cut and Low boost value     \n");
        Printf("                 0:0.0                   100:1.0    \n");
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u2Mode = (UINT16)StrToInt(szArgv[2]);

    if(u2Mode<=100)
    {
        AUD_DspAC3DRCRange100(u2Mode, u1DecId);
    }
    return 1;
}
static INT32 _AudCmdDtvAc3Query(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    AUD_DEC_INFO_T  rAudInfo;
    if(i4Argc==1)
    {
        u1DecId = AUD_DEC_MAIN;
    }
    else if(i4Argc==2)
    {
        u1DecId = (UINT8)StrToInt(szArgv[1]);
     }
    else
    {
        Printf("\n");
        Printf("Usage: q [decoder id]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("                 2:THIRD \n");
        Printf("\n");
          return -1;
    }
    AUD_DEC_ID_VALIDATE_3(u1DecId);
    if (!AUD_IsDecoderPlay(AUD_DSP0, u1DecId))
    {
        Printf("\n");
        Printf("There is no audio decoder playing \n");
        Printf("Usage: q [decoder id]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("                 2:THIRD \n");
        Printf("\n");
        return -1 ;
    }

    AUD_GetDtvAudInfo(u1DecId, &rAudInfo);

    if (rAudInfo.e_aud_fmt != AUD_FMT_AC3)
    {
        Printf("\n");
        Printf("There is no AC3/E-AC3 audio decoder playing in dec%d\n", u1DecId);
        Printf("Usage: q [decoder id]\n");
        Printf("[decoder id]     0:FIRST                 1:SECOND \n");
        Printf("                 2:THIRD \n");
        Printf("\n");
        return -1;
    }

    Printf(" Current audio format:  %s  \n",_aszAudFormat[rAudInfo.e_aud_fmt]);
    if(rAudInfo.e_aud_fmt == AUD_FMT_AC3)
    {
        Printf("\n");
        Printf(" Extra info. of AC3: \n ");
     Printf("  >>>Current audio decode type: ");
        switch(rAudInfo.e_dec_type)
        {
        case AUD_DECODE_TYPE_AC3:
         Printf("AC3 \n");
         break;
     case AUD_DECODE_TYPE_EAC3:
         Printf("E-AC3 \n");
         break;
        default:
         Printf("Unknown \n");
         break;
        }
        Printf("   >>>Current stream");
        switch (rAudInfo.e_aud_type)
        {
        case AUD_TYPE_DUAL_MONO: // 1+1
            Printf("(1+1) contains no LFE \n");
            break;
        case AUD_TYPE_MONO: // 1/0
            Printf("(1/0) contains no LFE \n");
            break;
        case AUD_TYPE_STEREO: // 2/0
            Printf("(2/0) contains no LFE \n");
            break;
        case AUD_TYPE_3_0: // 3/0
            Printf("(3/0) contains no LFE \n");
            break;
        case AUD_TYPE_SURROUND_2CH: // 2/1
            Printf("(2/1) contains no LFE \n");
            break;
        case AUD_TYPE_SURROUND: // 3/1
            Printf("(3/1) contains no LFE \n");
            break;
        case AUD_TYPE_4_0: // 2/2
            Printf("(2/2) contains no LFE \n");
            break;
        case AUD_TYPE_5_0: // 3/2
            Printf("(3/2) contains no LFE \n");
            break;
        case AUD_TYPE_MONO_LFE: // 1/0.L
            Printf("(1/0.L) contains LFE \n");
            break;
        case AUD_TYPE_STEREO_LFE: // 2/0.L
            Printf("(2/0.L) contains LFE \n");
            break;
        case AUD_TYPE_3_1: // 3/0.L
            Printf("(3/0.L) contains LFE \n");
            break;
        case AUD_TYPE_SURROUND_2CH_LFE: // 2/1.L
            Printf("(2/1.L) contains LFE \n");
            break;
        case AUD_TYPE_SURROUND_LFE: // 3/1.L
            Printf("(3/1.L) contains LFE \n");
            break;
        case AUD_TYPE_4_1: // 2/2.L
            Printf("(2/2.L) contains LFE \n");
            break;
        case AUD_TYPE_5_1: // 3/2.L
            Printf("(3/2.L) contains LFE \n");
            break;
        default: //
            Printf(" contains no LFE \n");
            break;
        }
    }
    Printf(" Sampling rate:         %d Hz  \n",rAudInfo.ui4_sample_rate);
    Printf(" Data rate:             %d bps\n",rAudInfo.ui4_data_rate);
    Printf(" Audio decoding mode:   %s\n",_aszAudACMode[rAudInfo.e_aud_type]);
    Printf("\n");

    return 1 ;
}
#ifdef CC_AUD_USE_NVM
static void _AudCmdPrintData(UINT8* pBuffer, UINT32 u4Offset, UINT32 u4Size)
{           
    UINT32 i;
    UINT8 u1Data;

    UNUSED(_AudCmdPrintData);
    for (i = 0; i < u4Size; i++)
    {
        u1Data = pBuffer[u4Offset+i];        
        if ((i%16) == 0)
        {
            Printf("\n");
        }                
        Printf("0x%02x ", u1Data);
    }
    Printf("\n");
}
static INT32 _AudCmdNvmRead(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 *pBuffer;
    UINT16 u2Idx;

    if (i4Argc != 1)
    {
        Printf("Usage: read \n");
        return -1;
    }

    //pBuffer = (UINT8 *)BSP_AllocAlignedDmaMemory(AUD_NVM_LENGTH, 32);
    pBuffer = ( UINT8 *)x_mem_alloc(AUD_NVM_LENGTH);


    if (pBuffer == NULL)
    {
        Printf("Memory Allocation fail !\n");
        return -1;
    }    

    if (AUD_NVM_Operation(AUD_NVM_READ, pBuffer, 0))
    {
        Printf("NVM Read Fail!\n");
        return -1;
    }

    Printf("Hex format : \n");
    for (u2Idx=0; u2Idx<AUD_NVM_LENGTH; u2Idx++)
    {
        if (u2Idx%16 == 0)
        {
            Printf("\n[%02x] ", u2Idx);
        }
        Printf("%02x ", pBuffer[u2Idx]);
    }
    Printf("\n");

    //VERIFY(BSP_FreeAlignedDmaMemory((UINT8*)pBuffer));
    x_mem_free(( UINT8 *)pBuffer);
    return 0;
}

static INT32 _AudCmdNvmWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1NvmData;
    UINT16 u2Offset;

    if (i4Argc < 3)
    {
        Printf("Usage: write [offset] [Data] \n");
        return -1;
    }

    u2Offset = StrToInt(szArgv[1]);
    u1NvmData = (UINT8)StrToInt(szArgv[2]);

    if (AUD_NVM_Operation(AUD_NVM_WRITE, &u1NvmData, u2Offset))
    {
        Printf("NVM Write Fail!\n");
        return -1;
    }

    Printf("NVM Write Success.\n");
    return 0;
}

static INT32 _AudCmdNvmInit(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1NvmData;

    if (i4Argc != 1)
    {
        Printf("Usage: init \n");
        return -1;
    }

    if (AUD_NVM_Operation(AUD_NVM_INIT, &u1NvmData, 0))
    {
        Printf("NVM Init Fail!\n");
        return -1;
    }

    Printf("NVM Init Success.\n");
    return 0;
}

static INT32 _AudCmdProfileRead(INT32 i4Argc, const CHAR **szArgv)
{
    SMART_AUD_GROUP_INFO_T *eSmartAudInfo;
    UINT8 i;
    UINT8 u1Ret = 0;
    UINT8* pSmartAudItem;

    //eSmartAudInfo.aSmartAud = 0;
    
    if (i4Argc != 1)
    {
        Printf("Usage: pfr \n");
        return -1;       
    }

    eSmartAudInfo = (SMART_AUD_GROUP_INFO_T *)x_mem_alloc(sizeof(SMART_AUD_GROUP_INFO_T));
    if (eSmartAudInfo == NULL)
    {
        Printf("Memory Allocation fail !\n");
        return -1;
    }
    
    u1Ret = AUD_NVM_Get_Profile(eSmartAudInfo);
    if (u1Ret)
    {
        printf("Get Profile address Fail!!");
        return -1;
    }
    printf("The Profile in EEPROM : \n");

    pSmartAudItem = (UINT8*)eSmartAudInfo;
    for(i=0; i<6*8; i++)    //FIXME!! nvm
    {
        if ((i % 6) == 0)
        {
            printf("\n");
        }
        printf("%02x ", *(pSmartAudItem+i) );
    }
    Printf("\n");

    x_mem_free(( UINT8 *)eSmartAudInfo);
    
    return 0;
}

static INT32 _AudCmdNvmFlashAQListAll(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1ProfileId;
    UINT8 u1ProfileTotalNumber = DRVCUST_OptGet(eAudioProfileTotalNumber);
    UINT8 *pBuffer;

    Printf("Usage: l [profile index]\n");
    Printf("[profile index] 0~%d\n\n", u1ProfileTotalNumber-1);

    if (i4Argc < 2)
    {
        u1ProfileId = AUD_NVM_Get_Profile_Id();
        Printf("Current profile ID = %d\n", u1ProfileId);
    }
    else
    {
        u1ProfileId = StrToInt(szArgv[1]);
        if (u1ProfileId >= u1ProfileTotalNumber)
        {
            Printf("Wrong profile ID\n");
            return -1;  
        }
        Printf("Profile ID = %d\n", u1ProfileId);
    }

    //pBuffer = (UINT8 *)BSP_AllocAlignedDmaMemory(AUD_FLASH_LENGTH, 32);
    pBuffer = (UINT8 *)x_mem_alloc(AUD_FLASH_LENGTH);

    if (pBuffer == NULL)
    {
        Printf("Memory Allocation fail !\n");
        return -1;
    }
    
    AUD_NVM_Get_FlashAQ_List(u1ProfileId, (UINT8 *)VIRTUAL((UINT32)pBuffer));
    Printf("[Volume (L ch)] offset : 0x0");
    _AudCmdPrintData(pBuffer, 0, 19);
    Printf("[HP Volume] offset : 0x13");
    _AudCmdPrintData(pBuffer, 19, 18);
    Printf("[VS] offset : 0x25");
    _AudCmdPrintData(pBuffer, 37, 15);
    Printf("[Cutoff Freq] offset : 0x34");
    _AudCmdPrintData(pBuffer, 52, 1);
    Printf("[Clear sound] offset : 0x35");
    _AudCmdPrintData(pBuffer, 53, 4);
    Printf("[Assistant A/B/C] offset : 0x39");
    _AudCmdPrintData(pBuffer, 57, 12);
    Printf("[EQ] offset : 0x45");
    _AudCmdPrintData(pBuffer, 69, 28);
    Printf("[VB] offset : 0x61");
    _AudCmdPrintData(pBuffer, 97, 6);
    Printf("[PEQ] offset : 0x67");
    _AudCmdPrintData(pBuffer, 103, 119);
    Printf("[BEEP] offset : 0xDE");
    _AudCmdPrintData(pBuffer, 222, 2);
    Printf("[LIMITER] offset : 0xE0");
    _AudCmdPrintData(pBuffer, 224, 12);
    Printf("[AVL] offset : 0xEC");
    _AudCmdPrintData(pBuffer, 236, 36);
    Printf("[Volume (R ch)] offset : 0x110");
    _AudCmdPrintData(pBuffer, 272, 18);
    Printf("[Volume (L ch for skype)] offset : 0x122");
    _AudCmdPrintData(pBuffer, 290, 18);
    Printf("[Volume (R ch for skype)] offset : 0x134");
    _AudCmdPrintData(pBuffer, 308, 18);
    Printf("[Bass Management] offset : 0x146");
    _AudCmdPrintData(pBuffer, 326, 1);
    Printf("[Dual Mono] offset : 0x147");
    _AudCmdPrintData(pBuffer, 327, 1);
    Printf("[PEQ Gain] offset : 0x148");
    _AudCmdPrintData(pBuffer, 328, 8);
    Printf("[EQ (clear sound off)] offset : 0x150");
    _AudCmdPrintData(pBuffer, 336, 14);
    Printf("[PEQ (clear sound off)] offset : 0x15E");
    _AudCmdPrintData(pBuffer, 350, 14);
    Printf("[Class D] offset : 0x170");
    _AudCmdPrintData(pBuffer, 368, 628);
    Printf("[Mode1] offset : 0x446");
    _AudCmdPrintData(pBuffer, 1094, 6);
    Printf("[Mode2] offset : 0x44C");
    _AudCmdPrintData(pBuffer, 1100, 6);
    Printf("[Mode3] offset : 0x452");
    _AudCmdPrintData(pBuffer, 1106, 6);
    Printf("[Mode4] offset : 0x458");
    _AudCmdPrintData(pBuffer, 1112, 6);
    Printf("[Mode5] offset : 0x45E");
    _AudCmdPrintData(pBuffer, 1118, 6);
    Printf("[Mode6] offset : 0x464");
    _AudCmdPrintData(pBuffer, 1124, 6);
    Printf("[Mode7] offset : 0x46A");
    _AudCmdPrintData(pBuffer, 1130, 6);

    //VERIFY(BSP_FreeAlignedDmaMemory((UINT8*)pBuffer));
    x_mem_free((UINT8*)pBuffer);

    return 0;
}
#endif //#ifdef CC_AUD_USE_NVM

static INT32 _AdspDSPROMID(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i;
    UINT32 u1IDbuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    UNUSED(i4Argc);
    UNUSED(szArgv);

    AUD_Read32ByteID (AUD_DSP0, (UINT8 *) &u1IDbuf[0], 32);
    printf ("DSP0 32 bytes ROM ID\n");
    for (i = 0; i < 8; i ++)
    {
        printf ("%08X ", u1IDbuf[i]);
    }
    printf ("\n");

    AUD_Read32ByteID (AUD_DSP1, (UINT8 *) &u1IDbuf[0], 32);
    printf ("DSP1 32 bytes ROM ID\n");
    for (i = 0; i < 8; i ++)
    {
        printf ("%08X ", u1IDbuf[i]);
    }
    printf ("\n");

    return 1;
}

///extern void AUD_LogVdoInfo(void); //CC_AUD_QUERY_VDO_INFO
static INT32 _AdspQChannelDelay(INT32 i4Argc, const CHAR ** szArgv)
{
//QUERY_DELAY
    UINT8 u1ChannelIdx;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    printf("Channel :\n(0)-C, \n(1)-L,  (2)-R, \n(3)-Ls, (4)-Rs, \n(5)-BypassL, (6)-ByPassR, \n(7)-Sw, \n(8)-DmxL,(9)-DmxR \n");
    for (u1ChannelIdx = 0; u1ChannelIdx < 10; u1ChannelIdx++)
    {
        printf(" Ch Id (%d) Delay is %d ms \n", u1ChannelIdx, AUD_DspQueryChannelDelay(u1ChannelIdx));
    }

    printf("Current Main path video delay is %d ms \n ",  bDrvVideoGetFrameDelay(0));

    ///AUD_LogVdoInfo();

    return 0;
}


//================================================================================
#ifdef SUPPORT_FLASH_AQ


#if MULTIPLE_FLASH_AQ
extern UINT32 GetCurrentPanelIndex(void);
extern UINT16 vApiFlashPqGetQtyId(UINT8 bIndex);
#ifdef CC_ENABLE_MTK_MODEL_INDEX
extern UINT16 vApiFlashaqGetQtyId(UINT8 bIndex);
extern UINT32 GetCurrentAQIndex(void);
#endif
#endif

#ifdef CC_COMPRESSED_FLASH_AQ_DATA
static INT32 _AudCmdFlashAQReadEx_LZMA(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u1Length = 0;    
    UINT32 i = 0;   
    CHAR * p = NULL;
    UINT16 u2FlashAqIndex;
    if (i4Argc < 2)
    {
        Printf("Usage: rx [length] \n");
        return -1;
    }
    p = rAudFlashAQ.au1AudId;
    u1Length = (UINT32)StrToInt(szArgv[1]);
    if(u1Length > sizeof(rAudFlashAQ))
    {
        u1Length = sizeof(rAudFlashAQ);
    }
    Printf("  ID: %c%c%c%c\n",rAudFlashAQ.au1AudId[0],rAudFlashAQ.au1AudId[1],rAudFlashAQ.au1AudId[2],rAudFlashAQ.au1AudId[3]);
    Printf("  Version: %c%c%c%c%c%c%c%c\n",rAudFlashAQ.au1AudVersion[0],rAudFlashAQ.au1AudVersion[1],
            rAudFlashAQ.au1AudVersion[2],rAudFlashAQ.au1AudVersion[3],rAudFlashAQ.au1AudVersion[4],rAudFlashAQ.au1AudVersion[5],rAudFlashAQ.au1AudVersion[6],rAudFlashAQ.au1AudVersion[7]);
    
#if MULTIPLE_FLASH_AQ
#ifdef CC_ENABLE_MTK_MODEL_INDEX
    u2FlashAqIndex = vApiFlashaqGetQtyId(GetCurrentAQIndex ());
#else
    u2FlashAqIndex = vApiFlashPqGetQtyId(GetCurrentPanelIndex());
#endif
    Printf("AQ Index is: %d\n", u2FlashAqIndex);
    Printf("AQ(%d) offset: 0x%04x\n",
                u2FlashAqIndex,
                FLASH_AQ_HDR_OFFSET + u2FlashAqIndex * (sizeof(rAudFlashAQ) - FLASH_AQ_HDR_OFFSET));
#endif
    for (i=0; i<u1Length; i++)
    {
      Printf("0x%x\n ", *p++);
    }

    return 0;            

}

#else
static INT32 _AudCmdFlashAQReadEx(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2Offset;
    UINT8 u1Partition = 0;
    UINT8 u1Length = 1;
    UINT8 u1Data = 0;
    UINT8 i;
    UINT16 u2FlashAqIndex;
    UINT8 u1ID[FLASH_AQ_HDR_OFFSET] = {0,0,0,0,0,0,0,0,0,0,0,0};

    UNUSED(i4Argc);
    UNUSED(szArgv);

    u1Partition = (UINT8)DRVCUST_OptGet(eAudioFlashAQPartition);
    Printf("eAudioFlashAQPartition: %d\n", u1Partition);
    AUD_FlashAQReadEx(u1Partition, 0, u1ID, FLASH_AQ_HDR_OFFSET);
    Printf("  ID: %c%c%c%c\n",u1ID[0],u1ID[1],u1ID[2],u1ID[3]);
    Printf("  Version: %c%c%c%c%c%c%c%c\n",u1ID[4],u1ID[5],u1ID[6],u1ID[7],u1ID[8],u1ID[9],u1ID[10],u1ID[11]);
    #if MULTIPLE_FLASH_AQ
    #ifdef CC_ENABLE_MTK_MODEL_INDEX
    u2FlashAqIndex = vApiFlashaqGetQtyId(GetCurrentAQIndex ());
    #else
    u2FlashAqIndex = vApiFlashPqGetQtyId(GetCurrentPanelIndex());
    #endif
    Printf("AQ Index is: %d\n", u2FlashAqIndex);
    Printf("  AQ(%d) offset: 0x%04x\n",
                u2FlashAqIndex,
                FLASH_AQ_HDR_OFFSET + u2FlashAqIndex * (sizeof(rAudFlashAQ) - FLASH_AQ_HDR_OFFSET));
    #endif

    if ((i4Argc != 3)&&(i4Argc != 4))
    {
        Printf("nvm.rx [u1Partition] [u2Offset] [u1length]\n");
        return -1;
    }

    u1Partition = StrToInt(szArgv[1]);
    u2Offset = StrToInt(szArgv[2]);
    if (i4Argc == 4)
    {
        u1Length = (UINT8)StrToInt(szArgv[3]);
    }

    for (i=0; i<u1Length; i++)
    {
      AUD_FlashAQReadEx(u1Partition, u2Offset+i, &u1Data, 1);
      Printf("0x%x ", u1Data);
    }

    Printf("\n");
    return 0;
}
#endif

#endif

//================================================================================
#ifdef CC_AUD_USE_FLASH_AQ
static INT32 _AudCmdFlashAQWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset;
    UINT8  u1Data;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc != 3)
    {
        Printf("nw [u2Offset][u1Data]\n");
        return -1;
    }

    u4Offset = StrToInt(szArgv[1]);    
    u1Data = (UINT8)StrToInt(szArgv[2]);

    Aud_FlashAQWrite(u4Offset, &u1Data, 1);

    Printf("Write Data 0x%x to %d\n", u1Data,u4Offset);    

    return 0;
}

static INT32 _AudCmdFlashAQRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Offset;
    UINT8  u1Length = 1;
    UINT8 u1Data;
    UINT8 i;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if ((i4Argc != 2)&&(i4Argc != 3))
    {
        Printf("nr [u2Offset][u1length]\n");
        return -1;
    }

    u4Offset = StrToInt(szArgv[1]);    
    if (i4Argc == 3)
    {
        u1Length = (UINT8)StrToInt(szArgv[2]);
    }

    for (i=0; i<u1Length; i++)
    {
        Aud_FlashAQRead(u4Offset+i, &u1Data, 1);
        Printf("0x%x ", u1Data);
    }

    Printf("\n");
    return 0;
}

static INT32 _AudCmdFlashAQGetVersion(INT32 i4Argc, const CHAR ** szArgv)
{
    CHAR pTestVer[FLASH_AQ_VER_LENGTH+1];
    UINT8 u1AQID;


    UNUSED(i4Argc);
    UNUSED(szArgv);

    pTestVer[FLASH_AQ_VER_LENGTH] = '\0';
    Aud_FlashAQRead(FLASH_AQ_VER_OFFSET, (UINT8*)pTestVer, FLASH_AQ_VER_LENGTH);

    Printf(" Your AQ Version on Flash is %s\n", pTestVer);
    u1AQID = AUD_FlashAQ_GetTableNum();
    Printf("(Your AQ Table ID is %d)\n",u1AQID);
    Printf("=======================\n");
    Printf("This firmware needs AQ version newer than %s\n", pTestVer);
    Printf("Please make sure you use correct AQ file.");

#if 0 //test Backlight
    UINT8 i;
    UINT8 pu1Data[AQ_BACKLIGHT_ITEMS];
    if (AUD_Get_BackLight(pu1Data))
    {
        for (i=0;i<AQ_BACKLIGHT_ITEMS;i++)
        {
            Printf("0x%x ",pu1Data[i]);
        }
        Printf("\n");
    }

#endif
    return 0;
}

static INT32 _AudCmdFlashAQComVersion(INT32 i4Argc, const CHAR ** szArgv)
{
    CHAR pUppVersion[FLASH_AQ_VER_LENGTH+1];
    CHAR pTestVer[FLASH_AQ_VER_LENGTH+1];

    if ((i4Argc != 2)&&(i4Argc != 3))
    {
        Printf("vcmp [UppVersion] ([TestVersion])\n");
        Printf("     0.00.000 \n");
        return -1;
    }
#ifdef LINUX_TURNKEY_SOLUTION
    x_strncpy(pUppVersion,szArgv[1], 8);
#else
    x_strcpy(pUppVersion,szArgv[1]);
#endif    
    Printf("Your version is %s\n", pUppVersion);

    if (i4Argc == 2)
    {
        pTestVer[FLASH_AQ_VER_LENGTH] = '\0';
        Aud_FlashAQRead(FLASH_AQ_VER_OFFSET, (UINT8*)pTestVer, FLASH_AQ_VER_LENGTH);
    }
    else
    {
#ifdef LINUX_TURNKEY_SOLUTION        
        x_strncpy(pTestVer,szArgv[2], 8);
#else
        x_strcpy(pTestVer,szArgv[2]);
#endif        
    }

    if (Aud_FlashAQCompVersion(pTestVer,pUppVersion))
    {
        Printf("%s is >= %s\n", pUppVersion, pTestVer);
    }
    else
    {
        Printf("%s is < %s\n", pUppVersion, pTestVer);
    }

    return 0;
}

static void _AudCmdPrintData(UINT8* pBuffer, UINT32 u4Size)
{
    UINT32 i;
    UINT8 u1Data;

    UNUSED(_AudCmdPrintData);
    for (i = 0; i < u4Size; i++)
    {
        u1Data = pBuffer[i];        
        if ((i%16) == 0)
        {
            Printf("\n");
        }                
        Printf("0x%02x ", u1Data);
    }
    Printf("\n");
}

static INT32 _AudCmdFlashAQListAll(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 i;
    UINT32 u4Offset;
    AUD_AQ_DSCRPT_T *eAudAQDscrpt;
    UINT32 u4Temp;
    UINT8* pBuffer;
    UINT8 u1AQID;
    UINT8 u1AQ_ID = 0;
    UINT8 u1AudAQDscrptSize;
    UINT32 u4CommTableSize, u4AQTableSize, u4ExtPEQSize;     

    AUD_AQ_DSCRPT_T *eAudAQDscrpt1;
    UINT32 u4AQAmpTableSize = 0;
    UINT8 u1AudAQDscrptSize1 = 0;

    UNUSED(_AudCmdFlashAQListAll);
    UNUSED(i4Argc);
    UNUSED(szArgv);
    UNUSED(u4Temp);
    UNUSED(u4ExtPEQSize);
    u4Offset = 0;

    if (i4Argc < 2)
    {
        Printf("Usage : l [AQ_ID] (1~%d)\n",FLASH_AQ_ID_NUM);
        return -1;    
    }

    u1AQ_ID = StrToInt(szArgv[1]);
    if ((u1AQ_ID > FLASH_AQ_ID_NUM) || (u1AQ_ID<1))
    {
        Printf("Invalid AQ ID - %d\n",u1AQ_ID);
        return -1;
    }
    u1AQ_ID = u1AQ_ID -1; //AQ ID starts from 1.
    Aud_FlashAQ_GetDscrpt(&eAudAQDscrpt);
    u1AudAQDscrptSize = (UINT8)DRVCUST_OptGet(eAudioFlashAQDataDscrptSize);    
    //u1AudAQDscrptSize = 18; //<=========================== !!!!!!!!!!! Fixme !! Items in AudAQDscrpt

    // Allocate buffer for NAND read.
    pBuffer = (UINT8 *)BSP_AllocAlignedDmaMemory(AUD_FLASH_AQ_BUF_SIZE, 32);
    if (pBuffer == NULL)
    {
        Printf("Memory Allocation fail !\n");
        return -1;
    }

    // For test API only
    u1AQID = AUD_FlashAQ_GetTableNum(); //just test the API.
    Printf("(Your AQ Table ID is %d)\n", u1AQID);
    Printf("The Data of ID %d on flash are : \n", u1AQ_ID);

    u4Temp = 0;
    // ========== Calc Common Table Size =========== 
    for (i=0; i<FLASH_AQ_COMM_TABLE_ITEMS; i++)
    {
        u4Temp += eAudAQDscrpt[i].u4Size;    
    }       
    u4CommTableSize = u4Temp;        
    // ========== Calc Single AQ Table Size =========== 
    u4Temp = 0;
    for (i=FLASH_AQ_COMM_TABLE_ITEMS; i<u1AudAQDscrptSize; i++)
    {
        u4Temp += eAudAQDscrpt[i].u4Size;    
    }
    u4AQTableSize = u4Temp;
    // ========== The Common Tables. ===========
    u4Offset = FLASH_AQ_VER_OFFSET;
    for (i=0; i<FLASH_AQ_COMM_TABLE_ITEMS; i++)
    {
        Printf(" [%s] offset : %d ", eAudAQDscrpt[i].pChar, u4Offset);
        u4Temp = eAudAQDscrpt[i].u4Size;
        Aud_FlashAQRead(u4Offset, ((UINT8*)VIRTUAL((UINT32)pBuffer)), u4Temp);
        _AudCmdPrintData(((UINT8*)VIRTUAL((UINT32)pBuffer)), u4Temp);
        u4Offset += u4Temp;
    }            
    // ========== The AQ Tables. ===========
    // Skip to the AQ ID offset 
    u4Offset = FLASH_AQ_VER_OFFSET + u4CommTableSize + (u4AQTableSize * u1AQ_ID);
    // Parsing it.
    for (i=FLASH_AQ_COMM_TABLE_ITEMS; i<u1AudAQDscrptSize; i++)
    {
        Printf(" [%s] offset : %d ", eAudAQDscrpt[i].pChar, u4Offset);        
        u4Temp = eAudAQDscrpt[i].u4Size;
        Aud_FlashAQRead(u4Offset, ((UINT8*)VIRTUAL((UINT32)pBuffer)), u4Temp);
        _AudCmdPrintData(((UINT8*)VIRTUAL((UINT32)pBuffer)), u4Temp);
        u4Offset += u4Temp;
    }
    
    if (Aud_FlashAQ_GetDscrpt1(&eAudAQDscrpt1)) //Get the Data Structure.
    {
        u1AudAQDscrptSize1 = (UINT8)DRVCUST_OptGet(eAudioFlashAQDataDscrptSize1);

        u4Temp = 0;
        // ========== Calc Common Table Size ===================== 
        for (i=0; i<u1AudAQDscrptSize1; i++)
        {
            u4Temp += eAudAQDscrpt1[i].u4Size;    
        }
        u4AQAmpTableSize = u4Temp;   /// u4AQAmpTableSize

        /// "AMP Function Field" sheet, start offset
        u4Offset = FLASH_AQ_VER_OFFSET + u4CommTableSize + (u4AQTableSize*FLASH_AQ_ID_NUM);
        u4Offset += (u4AQAmpTableSize * u1AQ_ID);

        // Parsing it.
        for (i=0; i<u1AudAQDscrptSize1; i++)
        {
            Printf(" [%s] offset : %d ", eAudAQDscrpt1[i].pChar, u4Offset);
            u4Temp = eAudAQDscrpt1[i].u4Size;
            Aud_FlashAQRead(u4Offset, ((UINT8*)VIRTUAL((UINT32)pBuffer)), u4Temp);
            _AudCmdPrintData(((UINT8*)VIRTUAL((UINT32)pBuffer)), u4Temp);

            u4Offset += u4Temp;
        }
    }
    else
    {
        LOG(0, "Flash AQ Amp data is not correct, using default value\n");
    } 
    VERIFY(BSP_FreeAlignedDmaMemory((UINT32)pBuffer));
    return 0;
}

#if 0
static INT32 _AudCmdFlashAQFillDataStruct(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    UNUSED(_AudCmdFlashAQFillDataStruct);    

    Aud_FlashAQ_Fill_Data(TRUE);

    return 0;
}
#endif

static INT32 _AudCmdFlashAQBassTrebleOffset(INT32 i4Argc, const CHAR ** szArgv)
{       
    UINT8  u1BassOffset;
    UINT8 u1TrebleOffset;
    UINT8 u1MaxOffset;
    
    UNUSED(i4Argc);
    UNUSED(szArgv);
    UNUSED(u1MaxOffset);
    
#ifdef CC_AUD_SM_SUPPORT
    u1MaxOffset = 28;
#else 
    u1MaxOffset = 20;
#endif

    if (i4Argc != 3)
    {
        AUD_DspGetBassTrebleOffset(&u1BassOffset, &u1TrebleOffset);
        Printf("bto [u1BassOffset][u1TrebleOffset]\n");
        Printf("  (Range is 0 ~ %d) \n", u1MaxOffset);
        Printf("Your current setting is \n");
        Printf("  BassOffset value: %d, TrebleOffset value: %d\n", u1BassOffset, u1TrebleOffset);
        Printf("  BassOffset: %d dB, TrebleOffset: %d dB\n", (u1BassOffset-10), (u1TrebleOffset-10));
        return -1;
    }

    u1BassOffset = StrToInt(szArgv[1]);
    u1TrebleOffset = StrToInt(szArgv[2]);
    if ((u1BassOffset > u1MaxOffset) || (u1TrebleOffset > u1MaxOffset))
    {
        Printf("Range is 0 ~ %d !!\n", u1MaxOffset);
        return -1;        
    }

    AUD_DspSetBassTrebleOffset(u1BassOffset, u1TrebleOffset);    
    return 0;
}

#endif /* CC_AUD_USE_FLASH_AQ */
//=================================================

#ifdef BURN_IN_AOUT_TEST
static INT32 _AdspAoutReset(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if ((i4Argc != 2))
    {
        Printf("[usage] raout 1  to reset counter\n");
        return -1;
    }

    AudResetAoutStatus();
    Printf("Reset total aout fail counter to 0\n");
    return 0;
}

static INT32 _AdspAoutQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("Total Aout Dram Fail Counter is : %d\n", AudGetAoutStatus());
    return 0;
}

static INT32 _AdspAoutSet(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    AudSetAoutTest();
    return 0;
}
#endif

static INT32 _AudCmdAsrcQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1ChMsk;
    UINT8 u1SrcID;
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc != 3)
    {
        Printf("q [ASRC ID] [ChMsk]\n");
        Printf(" [ASRC ID]: 0/1/2\n");
        Printf(" [ChMsk]:\n");
        Printf("  0x1 : CH01\n");
        Printf("  0x2 : CH23\n");
        Printf("  0x4 : CH45\n");
        Printf("  0x8 : CH67\n");
        Printf(" 0x10 : CH89\n");
        Printf(" 0x20 : CH1011\n");
        return -1;
    }

    u1ChMsk = (UINT8)StrToInt(szArgv[2]);
    u1SrcID = (UINT8)StrToInt(szArgv[1]);
    AudShowAsrcStatus(u1SrcID);
    AudShowAsrcBufferStatus(u1SrcID,u1ChMsk,TRUE);
    return 1;
}

extern void DSP_SetAsrcTrackingMode(BOOL fgEnable);
static INT32 _AudCmdAsrcTrackingMode(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;

    if (i4Argc != 2)
    {
        Printf("t [on/off])\n");
        Printf(" 1 - on, 0 - off\n");
        return -1;
    }
    fgEnable = (BOOL)StrToInt(szArgv[1]);

    DSP_SetAsrcTrackingMode(fgEnable);
    return 1;
}

static INT32 _AudAoutGain(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 chsel;
    UINT8 u1decid;
    UINT16 gain;
    if (i4Argc < 3)
    {
        Printf("Usage: aoutg  [dec] [channel] [gain]\n");
        Printf("[dec]        0  Main decoder \n");
        Printf("             1  Aux decoder \n");
        Printf("[channel]    0  all channels of decoder\n");
        Printf("             1   L ch      \n");
        Printf("             2   R ch      \n");
        Printf("             3   LS ch      \n");
        Printf("             4   RS ch      \n");
        Printf("             5   C ch  \n");
        Printf("             6   LFE ch \n");
        Printf("             7   ch7 \n");
        Printf("             8   ch8 \n");
        Printf("             9   ch9  \n");
        Printf("             10  ch10 \n");
        Printf("             11  ch11 \n");
        Printf("             12  ch12 \n");
        Printf("             13  ch13  \n");
        Printf("             14  ch14 \n");        
        Printf("[gain]       0x1000   --> 0 db \n");
        Printf("             0x0      --> mute \n");
        return -1;
    }

    u1decid = (UINT8)StrToInt(szArgv[1]);
    chsel   = (UINT8)StrToInt(szArgv[2]);
    gain    = (UINT16)StrToInt(szArgv[3]);

    AudAoutGainCtrl(u1decid, chsel, gain, 0x4);

    return 1;
}

static INT32 _AudCmdPll1Set(INT32 i4Argc, const CHAR ** szArgv)
{
    AudPll1Setting();

    return 1;
}

static INT32 _AudCmdPll2Set(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1ClkId;

    if (i4Argc != 2)
    {
        Printf("Usage: pll2 [clk]\n");
        Printf("[clk] 0:48KHz, 1:44.1KHz\n");
        return -1;
    }
    else
    {
        u1ClkId = (UINT8)StrToInt(szArgv[1]);
        if (u1ClkId == 0)
        {
            AudPll2Setting(FS_48K);
        }
        else if (u1ClkId == 1)
        {
            AudPll2Setting(FS_44K);
        }
    }
    return 1;
}

static INT32 _AudCmdPllPowerDown(INT32 i4Argc, const CHAR ** szArgv)
{
   UINT8 APLLID;
   BOOL fgPD;

   if (i4Argc != 3)
    {
        Printf ("Usage: pllpd [pllid] [fgPd]\n");
        Printf ("pllid: 0 APLL1; 1 APLL2\n");
        Printf ("fgPd:  1 PowerDown; 0 PowerOn\n");
    }
   else
    {

        APLLID = (UINT8)StrToInt(szArgv[1]);
        fgPD = (BOOL)StrToInt(szArgv[2]);
        AudPllPowerDown(APLLID,  fgPD);
    }
   return 1;
}

//CC_AUD_SUPPORT_OPENMAX
#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_AOMX)
static INT32 _AudCmdOmxLPCMSetting(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_PCM_SETTING_T rPCMInfo;
    AUD_LPCM_INFO_T rLPCMInfo;
    UINT32 u4SampleFreq;

    if (i4Argc != 9)
    {
        Printf("omx.lpcm [u4SampleFreq] [eLPcmType] [ui2_num_ch] [ui2_block_align]\n");
        Printf("         [ui2_bits_per_sample] [b_big_endian] [b_info_not_specific] [b_signed_data]\n");
        Printf("  u4SampleFreq: 48000/44100/32000 ...\n");
        Printf("  eLPcmType: 0: Normal, 1: MS_ADPCM, 2: IMA_ADPCM, 3: A_LAW, 4: U_LAW\n");
    }
    else
    {
        //eSampleFreq
        u4SampleFreq = (UINT32)StrToInt(szArgv[1]);
        switch (u4SampleFreq/1000)
        {
             case 48:
                 rLPCMInfo.eSampleFreq = FS_48K;
                 break;
             case 32:
                 rLPCMInfo.eSampleFreq = FS_32K;
                 break;
             case 44:
                 rLPCMInfo.eSampleFreq = FS_44K;
                 break;
             case 8:
                 rLPCMInfo.eSampleFreq = FS_8K;
                 break;
             case 11:
                 rLPCMInfo.eSampleFreq = FS_11K;
                 break;
             case 12:
                 rLPCMInfo.eSampleFreq = FS_12K;
                 break;
            case 16:
                 rLPCMInfo.eSampleFreq = FS_16K;
                 break;
             case 22:
                 rLPCMInfo.eSampleFreq = FS_22K;
                 break;
             case 24:
                 rLPCMInfo.eSampleFreq = FS_24K;
                 break;
             default:
                 rLPCMInfo.eSampleFreq = FS_48K;
                 break;
        }

        rLPCMInfo.eLPcmType = (LPCM_INPUT_TYPE_T)StrToInt(szArgv[2]);
        rLPCMInfo.ui2_num_ch = (UINT16)StrToInt(szArgv[3]);
        rLPCMInfo.ui2_block_align = (UINT16)StrToInt(szArgv[4]);
        rLPCMInfo.ui2_bits_per_sample = (UINT16)StrToInt(szArgv[5]);
        rLPCMInfo.b_big_endian = (BOOL)StrToInt(szArgv[6]);
        rLPCMInfo.b_info_not_specific = (BOOL)StrToInt(szArgv[7]);
        rLPCMInfo.b_signed_data = (BOOL)StrToInt(szArgv[8]);

        rPCMInfo.eSampleFreq = rLPCMInfo.eSampleFreq;
        if (rLPCMInfo.b_big_endian)
        {
            rPCMInfo.ePcmDataInfo = PCM_16_BITS_BIG_ENDIAN;
        }
        else
        {
            rPCMInfo.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
        }

        AUD_PcmSetting(AUD_DSP0, AUD_DEC_MAIN, &rPCMInfo);
        AUD_LPcmSetting(AUD_DEC_MAIN, &rLPCMInfo);
    }
    return 1;
}

static INT32 _AudCmdOmxWMASetting(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_WMA_CFG_T rWMAInfo = {0};

    if (i4Argc != 7)
    {
        Printf("omx.wma [u4Pktsz] [u2NumCh] [u4Sampersec] [u4Bpersec] [u4Blocksz] [u2Encoderopt]\n");
        Printf("  Using header detection mode\n");

        AUD_DspWMAHeader(AUD_DEC_MAIN, &rWMAInfo);
    }
    else
    {
        rWMAInfo.u2HdrSrc = 1;
        rWMAInfo.u4Pktsz = (UINT32)StrToInt(szArgv[1]);
        rWMAInfo.u2Numch = (UINT16)StrToInt(szArgv[2]);
        rWMAInfo.u4Sampersec = (UINT32)StrToInt(szArgv[3]);
        rWMAInfo.u4Bpersec = (UINT32)StrToInt(szArgv[4]);
        rWMAInfo.u4Blocksz = (UINT32)StrToInt(szArgv[5]);
        rWMAInfo.u2Encoderopt = (UINT16)StrToInt(szArgv[6]);

        AUD_DspWMAHeader(AUD_DEC_MAIN, &rWMAInfo);
    }
    return 1;
}

static INT32 _AudCmdOmxWMAProSetting(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_WMA_CFG_T rWMAInfo = {0};

    if (i4Argc != 10)
    {
        Printf("omx.wma10 [u2NumCh] [u4Sampersec] [u4Bpersec] [u4Blocksz] [u2Encoderopt]\n");
        Printf("        [u4ChannelMask] [u2BitsPerSample] [u2AdvEncoderopt] [u4AdvEncoderopt2]\n");

        AUD_DspWMAProHeader(AUD_DEC_MAIN,&rWMAInfo);
    }
    else
    {
        rWMAInfo.u2HdrSrc = 1;
        rWMAInfo.u2Numch = (UINT16)StrToInt(szArgv[1]);
        rWMAInfo.u4Sampersec = (UINT32)StrToInt(szArgv[2]);
        rWMAInfo.u4Bpersec = (UINT32)StrToInt(szArgv[3]);
        rWMAInfo.u4Blocksz = (UINT32)StrToInt(szArgv[4]);
        rWMAInfo.u2Encoderopt = (UINT16)StrToInt(szArgv[5]);

        rWMAInfo.u4ChannelMask = (UINT32)StrToInt(szArgv[6]);
        rWMAInfo.u2BitsPerSample = (UINT16)StrToInt(szArgv[7]);
        rWMAInfo.u2AdvEncoderopt = (UINT16)StrToInt(szArgv[8]);
        rWMAInfo.u4AdvEncoderopt2 = (UINT32)StrToInt(szArgv[9]);


        AUD_DspWMAProHeader(AUD_DEC_MAIN,&rWMAInfo);
    }
    return 1;
}

static INT32 _AudCmdOmxCOOKSetting(INT32 i4Argc, const CHAR ** szArgv)
{
     AUD_COOK_CFG_T rCookCfg = {0};

    if (i4Argc != 8)
    {
        Printf("omx.cook [u2SampPerFrame] [u2ChanNum] [u2SampRate] [u2FrameSzInBytes] [u2RegnNum]\n");
        Printf("         [u2StartRegn] [u2CplQBits]\n");
    }
    else
    {
        rCookCfg.u2SampPerFrame = (UINT16)StrToInt(szArgv[1]);
        rCookCfg.u2ChanNum = (UINT16)StrToInt(szArgv[2]);
        rCookCfg.u2SampRate = (UINT16)StrToInt(szArgv[3]);
        rCookCfg.u2FrameSzInBytes = (UINT16)StrToInt(szArgv[4]);
        rCookCfg.u2RegnNum = (UINT16)StrToInt(szArgv[5]);
        rCookCfg.u2StartRegn = (UINT16)StrToInt(szArgv[6]);
        rCookCfg.u2CplQBits = (UINT16)StrToInt(szArgv[7]);
        AUD_DspCOOKParameter(AUD_DEC_MAIN, &rCookCfg);
    }
    return 1;
}

extern UINT32 u4Latency;

static INT32 _AudCmdOmxDspDelaySetting(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.dspdelay [u4Latency]\n");
        Printf("  current Setting: u4Latency(%x) (0~0x1000)\n", u4Latency);
    }
    else
    {
        u4Latency = (UINT32)StrToInt(szArgv[1]);
    }
    return 1;
}

extern UINT8 bOmxSettingBypass;
static INT32 _AudCmdOmxSettingBypass(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.bypass [bypass]\n");
        Printf("  current setting: %d\n", bOmxSettingBypass);
    }
    else
    {
        bOmxSettingBypass = (UINT8)StrToInt(szArgv[1]);
    }
    return 1;
}

extern UINT32 u4AOmxDbgMsk;
extern void vAudOmxSetGstDbgMsk(UINT32 u4Index, UINT32 u4DbgMsk);
static INT32 _AudCmdOmxSetDbgMsk(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.dm [Debug Mask]\n");
        Printf("  current setting: %08x\n", u4AOmxDbgMsk);
        Printf("    (0001h) Reserved\n");
        Printf("    (0002h) [CTL] 1: Render ALSA path    0: Render MTAL path\n");
        Printf("    (0004h) [LOG] Queue empty buffer\n");
        Printf("    (0008h) Reserved\n");
        Printf("    (0010h) [LOG] PTS output log\n");
        Printf("    (0020h) [LOG] DecodeSize log\n");
        Printf("    (0040h) [LOG] Queue fill buffer log\n");
        Printf("    (0080h) [LOG] Enable adec_ddi.c log (with MTK_ENVIRONMENT=1)\n");
        Printf("    (0100h) [LOG] mtkaudiosink: render log\n");
        Printf("    (0200h) [CTL] Disable Error Cnt Monitor Notify\n");
        Printf("    (0400h) [LOG] Render Output PTS\n");
        Printf("    (0800h) [CTL] Disable AOMX_REAL_PTS mechanism\n");
        Printf("    (1000h) [LOG] Detail PTS gen log\n");
        Printf("    (2000h) [LOG] Effective Input PTS log\n");
        Printf("    (4000h) [CTL] mtkaudiosink: Disable AVSync early mechanism\n");
        Printf("    (8000h) [LOG] AOMX underrun log\n");
        Printf("   (10000h) [CTL] MTAL Render use decoder path\n");
        Printf("   (20000h) [LOG] VOMX Input PTS log\n");
        Printf("   (40000h) [LOG] VOMX Output PTS log\n");
        Printf("   (80000h) [LOG] Reserved\n");
        Printf("  (100000h) [CTL] mtkaudiosink: Disable fgEnableCurPts\n");
        Printf("  (200000h) [CTL] mtkaudiosink: Disable fgEnableSkipRedundantPts\n");
        Printf("  (400000h) [CTL] mtkaudiosink: Disable fgEnableAlignFirstPts\n");
        Printf("  (800000h) [LOG] mtkvideosink: render log\n");
        Printf(" (1000000h) [CTL] Disable AFIFO input control\n");
    }
    else
    {
        u4AOmxDbgMsk = (UINT32)StrToInt(szArgv[1]);
        vAudOmxSetGstDbgMsk(0,u4AOmxDbgMsk);
    }
    return 1;
}

extern void vAudOmxQuery(UINT8 uDecIndex);
static INT32 _AudCmdOmxQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 uDecIndex;
    if (i4Argc > 2)
    {
        Printf("aud.omx.q [aomx id]\n");
    }
    else
    {
        if (i4Argc == 1)
        {
            uDecIndex = 0;
        }
        else
        {
            uDecIndex = (UINT8)StrToInt(szArgv[1]);
        }
        vAudOmxQuery(uDecIndex);
    }
    return 1;
}

extern INT32 i4AOmxPtsOffset;
static INT32 _AudCmdOmxDelaySetting(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.delay [timestamp offset]\n");
        Printf("  Current Setting: %d\n", i4AOmxPtsOffset);
    }
    else
    {
        i4AOmxPtsOffset = i4StrToInt(szArgv[1]);
    }
    return 1;
}

extern UINT16 u2LPCM_CW;
static INT32 _AudCmdOmxLPCMCW(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.lpcm [u2LPCM_CW]\n");
        Printf("  Current Setting: %d\n", u2LPCM_CW);
    }
    else
    {
        u2LPCM_CW = (UINT16)StrToInt(szArgv[1]);
    }
    return 1;
}

static INT32 _AudCmdOmxAlsaQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    AUD_QueryMixSndClip();
    return 1;
}

extern void vSetMixSndGain(UINT8 u1StreamId,UINT32 u4Gain);
extern UINT32 u4GetMixSndGain(UINT8 u1StreamId);
static INT32 _AudCmdOmxAlsaVolume(INT32 i4Argc, const CHAR ** szArgv)
{
    int i;
    UINT8 u1StreamId;
    UINT32 u4Gain;

    if (i4Argc != 3)
    {
        Printf("omx.alsav [id] [volume]\n");
        Printf("  volume: 0 ~ 0x20000\n");
        Printf("  current setting:\n");
        for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA;i++)
        {
            Printf("    id %d: %x\n", i, u4GetMixSndGain(i+ALSA_MIXSND_STREAM_ID));
        }
    }
    else
    {
        u1StreamId = (UINT8)StrToInt(szArgv[1]);
        u4Gain = (UINT32)StrToInt(szArgv[2]);
        vSetMixSndGain(u1StreamId+ALSA_MIXSND_STREAM_ID, u4Gain);
    }
    return 1;
}

extern void vAudOmxSetDumpInfo(UINT32 u4Addr, UINT32 u4Size);
static INT32 _AudCmdOmxDumpAlloc(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Size = 0;
    static UINT8* pOmxBufNew = 0;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc != 2)
    {
        Printf("Usage: oda [Length]\n");
        Printf(" Length = 0 for free\n");
        return -1;
    }

    u4Size = (UINT32)StrToInt(szArgv[1]);
    if (u4Size == 0)
    {
        x_mem_free((VOID*)pOmxBufNew);
        Printf( "Free memory 0x%x \n",pOmxBufNew);
        pOmxBufNew = 0;
        return 0;
    }
    else
    {
        x_mem_free((VOID*)pOmxBufNew);
        Printf( "Free memory 0x%x \n",pOmxBufNew);
    }

    Printf( "Allocate memory size 0x%x for memory play...\n",u4Size);
    pOmxBufNew = (UINT8 *)x_mem_alloc_virtual(u4Size);
    if (pOmxBufNew == NULL)
    {
        printf( "Memory request for memory play fail !!\n");
        return -1;
    }

    Printf( "Allocate memory from 0x%x to 0x%x Success, len = 0x%x\n",pOmxBufNew,pOmxBufNew+u4Size,u4Size);
    Printf( "data.load.binary FILENAME 0x%x--0x%x /ny /swap \n",pOmxBufNew,pOmxBufNew+u4Size);

    vAudOmxSetDumpInfo((UINT32)pOmxBufNew, u4Size);
    return 0;
}

extern void vAudOmxResetDumpInfo(void);
static INT32 _AudCmdOmxDumpReset(INT32 i4Argc, const CHAR **szArgv)
{
    vAudOmxResetDumpInfo();
    return 0;
}

extern UINT8 u1AOMXDumpLocation;
static INT32 _AudCmdOmxDumpSet(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc != 2)
    {
        Printf("Usage: ods [location]\n");
        Printf("  0: Input Buf, 1: Output Buf, 2: AMR Output Buf\n");
        Printf("  Current Setting: %d\n", u1AOMXDumpLocation);
    }
    else
    {
        u1AOMXDumpLocation = ((UINT8)StrToInt(szArgv[1]));
        Printf("Set location = %d\n", u1AOMXDumpLocation);
    }
    return 0;
}

extern BOOL fgSkipMTAL;
static INT32 _AudCmdOmxSkipMTAL(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("omx.skipmtal [fgSkipMTAL]\n");
        Printf("  Current Setting: %d\n", fgSkipMTAL);
    }
    else
    {
        fgSkipMTAL = (BOOL)StrToInt(szArgv[1]);
    }
    return 1;
}

extern void vAudOmxGetGstDbgMsk(UINT32 u4Index,UINT32* pu4DbgMsk);
extern void vAudOmxSetGstDbgMsk(UINT32 u4Index, UINT32 u4DbgMsk);
static INT32 _AudCmdOmxGstDbgMsk(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Index, u4DbgMsk;
    if ((i4Argc != 2) && (i4Argc != 3))
    {
        Printf("omx.gst [index] [mask]\n");
    }
    else
    {
        u4Index = (UINT32)StrToInt(szArgv[1]);
        if (i4Argc > 2)
        {
            u4DbgMsk = (UINT32)StrToInt(szArgv[2]);
            vAudOmxSetGstDbgMsk(u4Index, u4DbgMsk);
        }
        else
        {
            vAudOmxGetGstDbgMsk(u4Index, &u4DbgMsk);
            Printf("Index: %d Value: 0x%08x\n", u4Index, u4DbgMsk);
        }
    }
    return 1;
}

#endif

#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_ADECOMX)
extern UINT32 u4ADecOmxDbgMsk;
static INT32 _AudCmdAdecOmxSetDbgMsk(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        Printf("adecomx.dm [Debug Mask]\n");
        Printf("  current setting: %08x\n", u4ADecOmxDbgMsk);
        Printf("  (00000001h) [LOG] _adecomx_inputbuf_copy log\n");
        Printf("  (00000002h) [LOG] _adecomx_outputbuf_copy log\n");
        Printf("  (00000004h) dump audio input data to file\n");        
    }
    else
    {
        u4ADecOmxDbgMsk = (UINT32)StrToInt(szArgv[1]);
    }
    return 1;
}
extern void adecomx_query(void);
static INT32 _AudCmdAdecOmxQuery(INT32 i4Argc, const CHAR ** szArgv)
{
    adecomx_query();
    return 1;
}

#endif

static void _AudCmdAvSyncCurrentStatus(UINT16 u2Lvl)
{
    printf("\n");
    if ((u2Lvl & AUD_DBG_SYNC_SEND_PTS) == AUD_DBG_SYNC_SEND_PTS)
    {
        printf("DBG : Send PTS is ON\n");
    }
    else
    {
        printf("DBG : Send PTS is OFF\n");
    }

    if ((u2Lvl & AUD_DBG_SYNC_STC_DIFF) == AUD_DBG_SYNC_STC_DIFF)
    {
        printf("DBG : STC_DIFF is ON\n");
    }
    else
    {
        printf("DBG : STC_DIFF is OFF\n");
    }

    if ((u2Lvl & AUD_DBG_SYNC_PTS_FRESH) == AUD_DBG_SYNC_PTS_FRESH)
    {
        printf("DBG : PTS_REFRESH is ON\n");
    }
    else
    {
        printf("DBG : PTS_REFRESH is OFF\n");
    }
    printf("\n");
}
static INT32 _AudCmdAvSyncDbgLvl(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT16 u2CurrLvl = 0;
    UINT16 u2Item = 0;
    BOOL fgOnOff = 0;

    u2CurrLvl = AUD_GetSyncDbgLvl();

    if (i4Argc < 3)
    {
        // Check debug level status
        printf("Usage : dbg [item] [0/1] \n");
        printf("   0: Send PTS \n");
        printf("   1: Stc Diff \n");
        printf("   2: PTS refresh \n");
        printf("  15: ALL \n");
        _AudCmdAvSyncCurrentStatus(u2CurrLvl);
        // Check SyncMode setting
        printf("  Sync Mode Setting ([Drv] 0:Free 1:Sys 2:Aud, [Dsp] 0:Aud, 1:Sys or Free)\n");
#ifdef CC_MT5391_AUD_3_DECODER
        printf("Drv: Dec0 %d, Dec1 %d, Dec2 %d\n", AUD_GetAvSynMode(AUD_DEC_MAIN),AUD_GetAvSynMode(AUD_DEC_AUX),AUD_GetAvSynMode(AUD_DEC_THIRD));
        printf("DSP: Dec0 %d, Dec1 %d, Dec2 %d\n", eDspGetSyncMode(AUD_DEC_MAIN),eDspGetSyncMode(AUD_DEC_AUX),eDspGetSyncMode(AUD_DEC_THIRD));
#else
        printf("Drv: Dec0 %d, Dec1 %d\n", AUD_GetAvSynMode(AUD_DEC_MAIN),AUD_GetAvSynMode(AUD_DEC_AUX));
        printf("DSP: Dec0 %d, Dec1 %d\n", eDspGetSyncMode(AUD_DEC_MAIN),eDspGetSyncMode(AUD_DEC_AUX));
#endif
        return -1;
    }

    u2Item = (UINT16)StrToInt(szArgv[1]);
    fgOnOff = (UINT16)StrToInt(szArgv[2]);
    if (u2Item == 15)   // ALL On or Off
    {
        if (fgOnOff)
        {
            u2CurrLvl = (UINT16)(AUD_DBG_SYNC_SEND_PTS|AUD_DBG_SYNC_STC_DIFF|AUD_DBG_SYNC_PTS_FRESH);
        }
        else
        {
            u2CurrLvl = (UINT16)0;
        }
    }
    else
    {
        if (fgOnOff)
        {
            u2CurrLvl |= (1<<u2Item);
        }
        else
        {
            u2CurrLvl &= (~(1<<u2Item));
        }
    }
    AUD_SetSyncDbgLvl(u2CurrLvl);
    _AudCmdAvSyncCurrentStatus(u2CurrLvl);

    return 0;
}


extern UINT8 AUD_DspQryStcID(UINT8 u1DecId);
static INT32 _AudCmdAvSyncSetStcID(INT32 i4Argc, const CHAR ** szArgv) // 2_STC_Task
{
    UINT8 u1DecId;
    UINT16 u2StcId;
    STC_CLOCK rStc;    

    AUD_DspGetStcId(AUD_DEC_MAIN, &u2StcId);
    printf("StcID of Dec Main is :%d, %d\n", u2StcId, AUD_DspQryStcID(AUD_DEC_MAIN));
    AUD_DspGetStcId(AUD_DEC_AUX, &u2StcId);
    printf("StcID of Dec Aux is :%d, %d\n", u2StcId, AUD_DspQryStcID(AUD_DEC_AUX));
#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DspGetStcId(AUD_DEC_THIRD, &u2StcId);
    printf("StcID of Dec Third is :%d, %d\n\n", u2StcId, AUD_DspQryStcID(AUD_DEC_THIRD));
#endif
    if (STC_GetSrc(0, &rStc) == STC_VALID)
    {
        printf("Stc0 is now 0x%x\n", rStc.u4Base);
    }
    if (STC_GetSrc(1, &rStc) == STC_VALID)
    {
        printf("Stc1 is now 0x%x\n", rStc.u4Base);
    }
    if (STC_GetSrc(2, &rStc) == STC_VALID)
    {
        printf("Stc2 is now 0x%x\n", rStc.u4Base);
    }    

    if (i4Argc < 3)
    {
        printf("Usage : sid [DecID] [StcID] \n");
        printf("   DecID: 0-Mian  1-Aux  2-Third \n");
        printf("   StcID: 0-Audio  1-Demux \n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u2StcId = (UINT16)StrToInt(szArgv[2]);
    AUD_DspSetStcId(u1DecId, u2StcId);
    printf ("set Dec(%d) StcID to %d\n", u1DecId, u2StcId);

    return 0;
}

#if 0 // 2_STC_Task_DEBUG_only
extern void _DMX_FIXME_ReadSTC(UINT8 u1StcId, UINT32* u4StcBase);
static INT32 _AudCmdAvSyncReadStc(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Stc;

    _DMX_FIXME_ReadSTC(0, &u4Stc);
    printf("AUDIO : 0x%x\n", u4Stc);
    _DMX_FIXME_ReadSTC(1, &u4Stc);
    printf("DMX STC1 : 0x%x\n", u4Stc);
    _DMX_FIXME_ReadSTC(2, &u4Stc);
    printf("DMX STC2 : 0x%x\n", u4Stc);
}
#endif

#if 1 //For Debug only
#define AUD_READ32(offset)                  IO_READ32(AUDIO_BASE, (offset))
INT32 _AudCmdDebug1(INT32 i4Argc, const CHAR **szArgv) //parson debug
{
    UINT16 i = 0;
    UINT16 u2Idx = 0;
    UINT16 u2Length = 0;
    UINT16 u4Addr = 0;
    UINT8 u1Data = 0;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    // ======== 1. Print PC for 20 Times.=========
    Printf ("\n This is 2011/8/5 21:21 Test image \n");

    Printf ("PC\n");
    for (i=0; i<60; i++)
    {
        //AUD_DspProbePause();
        Printf ("0x%x\n", u4AUD_DspProbeRead(AUD_DSP0, 0xa2));
        //AUD_DspProbeGo();
        x_thread_delay(10);
    }

    // ======== 2. SRAM 0, 0xA0, A1*7, A4, A7, 0x800, 0x980, 0x3680 ==========
    u4Addr = 0x3E8;
    Printf ("\n REG \n");
    Printf("0x3E8 is 0x%x\n",AUD_READ32(0x03E8));

    AUD_DspProbePause(AUD_DSP0);
    u2Length = 12;
    u4Addr = 0x0;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%08x ",u4AUD_DspProbeRead(AUD_DSP0, u4Addr+u2Idx));
    }
    Printf("\n");

    u2Length = 1;
    u4Addr = 0xA0;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%08x ",u4AUD_DspProbeRead(AUD_DSP0, u4Addr+u2Idx));
    }
    Printf("\n");

    u2Length = 7;
    u4Addr = 0xA1;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%08x ",u4AUD_DspProbeRead(AUD_DSP0, u4Addr+u2Idx));
    }
    Printf("\n");

    AUD_DspProbeGo(AUD_DSP0);
    x_thread_delay(10);
    u2Length = 1;
    u4Addr = 0xA4;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%08x ",u4AUD_DspProbeRead(AUD_DSP0, u4Addr+u2Idx));
    }
    Printf("\n");
    AUD_DspProbePause(AUD_DSP0);

    u2Length = 1;
    u4Addr = 0xA7;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%08x ",u4AUD_DspProbeRead(AUD_DSP0, u4Addr+u2Idx));
    }
    Printf("\n");

    u2Length = 5;
    u4Addr = 0x51;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%06x ",u4AUD_DspProbeRead(AUD_DSP0, u4Addr+u2Idx));
    }
    Printf("\n");
    u2Length = 0x100;
    Printf ("0x800 \n");
    u4Addr = 0x800;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%08x ",u4AUD_DspProbeRead(AUD_DSP0, u4Addr+u2Idx));
    }
    Printf("\n");
    Printf ("0x900 \n");
    u4Addr = 0x900;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%08x ",u4AUD_DspProbeRead(AUD_DSP0, u4Addr+u2Idx));
    }
    Printf("\n");
    Printf ("0x3680 \n");
    u4Addr = 0x3680;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%08x ",u4AUD_DspProbeRead(AUD_DSP0, u4Addr+u2Idx));
    }
    Printf("\n");
    Printf ("0xA00 \n");
    u4Addr = 0xA00;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 8) == 0)
        {
            Printf("\nAddress: 0x%04x \n",u4Addr+u2Idx);
        }
        Printf("0x%08x ",u4AUD_DspProbeRead(AUD_DSP0, u4Addr+u2Idx));
    }
    Printf("\n");

    Printf("\n C: %04x S: %04x d: %08x d: %08x\n",
            u4AUD_DspProbeRead(AUD_DSP0, 0xa0),u4AUD_DspProbeRead(AUD_DSP0, 0xa2),// PC, MCS
            u4AUD_DspProbeRead(AUD_DSP0, 0xa8),u4AUD_DspProbeRead(AUD_DSP0, 0xa9)// ins_d,ld_d
    );

    AUD_DspProbeGo(AUD_DSP0);
    x_thread_delay(20);

    // ============== 3. Common Dram =======================
    Printf ("\n Common Dram \n");
    DSP_CommDramDump(AUD_DSP0, 0x0, 1024);

    // ========== 4. Common Code. =================================
    Printf("\n");
    Printf ("\n Common Program \n");
    u2Length = 1024;
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 4)==0)
        {
            Printf("\n");
        }
#ifndef DSP_SUPPORT_DUAL_DECODE
        u1Data = (*((UINT8*)(VIRTUAL((_u4DspICacheAddr[AUD_DSP0][TYPE_COMM_ICACHE]<<8) + u2Idx))));
#else
        u1Data = (*((UINT8*)(VIRTUAL((_u4DspICacheAddr[AUD_DSP0][TYPE_COMMA_ICACHE]<<8) + u2Idx))));
#endif
        Printf("0x%02x ", u1Data);
    }

    // ============== 5. Decoder1 Code. ============================
    Printf("\n");
    Printf ("\n DEC1 \n");
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 4)==0)
        {
            Printf("\n");
        }
        u1Data = (*((UINT8*)(VIRTUAL((_u4DspICacheAddr[AUD_DSP0][TYPE_DEC1_ICACHE]<<8) + u2Idx))));
        Printf("0x%02x ", u1Data);
    }

    // ============= 6. Decoder2 Code =======================================
    Printf("\n");
    Printf ("\n DEC2 \n");
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 4)==0)
        {
            Printf("\n");
        }
        u1Data = (*((UINT8*)(VIRTUAL((_u4DspICacheAddr[AUD_DSP0][TYPE_DEC2_ICACHE]<<8) + u2Idx))));
        Printf("0x%02x ", u1Data);
    }

    // ============= 7. Decoder3 Code =======================================
    Printf("\n");
    Printf ("\n DEC3 \n");
    for (u2Idx = 0; u2Idx < u2Length; u2Idx++)
    {
        if ((u2Idx % 4)==0)
        {
            Printf("\n");
        }
        u1Data = (*((UINT8*)(VIRTUAL((_u4DspICacheAddr[AUD_DSP0][TYPE_DEC3_ICACHE]<<8) + u2Idx))));
        Printf("0x%02x ", u1Data);
    }

    return 0;
}
#endif //if 1 for debug only


static INT32 _AdspGetCap(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    AudShowDspCapability();
    return 1;
}

static INT32 _AudCmdUopQueryDSPVoumeSystem(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId=0;
    AUD_DEC_STREAM_FROM_T bInputSource;

    UNUSED(i4Argc);
    UNUSED(szArgv);



    printf("#----------DSP Volume System Setting----------#\n");

    bInputSource = _AudGetStrSource(AUD_DEC_MAIN);

    Printf("Input Source Gain:\n");

    Printf("    DEC1: %d DB\n", AUD_DspGetSrcVol(AUD_DEC_MAIN,bInputSource));

    Printf("    DEC2: %d DB\n\n", AUD_DspGetSrcVol(AUD_DEC_AUX,bInputSource));

    Printf("Master Volume: <0~100>\n");

    Printf("    DEC1: %d\n", AUD_DspGetMasterVolume(AUD_DEC_MAIN));
    Printf("    DEC2: %d\n", AUD_DspGetMasterVolume(AUD_DEC_AUX));
    Printf("    DEC3: %d\n", AUD_DspGetMasterVolume(AUD_DEC_THIRD));
    Printf("    DEC4: %d\n\n", AUD_DspGetMasterVolume(AUD_DEC_4TH));


    Printf("Channel Trim:  <0x20000 = 0 DB>\n");
    Printf("[Channel Volume Setting] (%s)\n", AUD_DspGetChannelVolumeEnable(AUD_DSP0, AUD_DEC_MAIN) ? "Enable" : "Disable");
            Printf(" #DEC1\n");
            for (u1DecId = 0;u1DecId<10;u1DecId++)
                Printf("    CH%d: %d(0x%x)\n", u1DecId, AUD_DspGetChannelVolume(AUD_DEC_MAIN,(AUD_CH_T)u1DecId), AUD_DspGetFineTuneVolume(AUD_DEC_MAIN, (AUD_CH_T)u1DecId));
            Printf(" #DEC2\n");
            for (u1DecId = 0;u1DecId<2;u1DecId++)
                Printf("    CH%d: %d(0x%x)\n", u1DecId, AUD_DspGetChannelVolume(AUD_DEC_AUX,(AUD_CH_T)u1DecId), AUD_DspGetFineTuneVolume(AUD_DEC_AUX, (AUD_CH_T)u1DecId));

    AUD_SoftwareMuteQuery();

    Printf("\nHardware relate mute flag: <1: unmute; 0: mute>\n");
    ADAC_SpeakerStateQuery();

    Printf("\n#------------------END-----------------------#\n");
    return 0;
}

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
static INT32 _AudCmdQueryPlayMuteHistory(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecID;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc < 2)
    {
        Printf("q [DecID]\n");
        return -1;
    }

    u1DecID = (UINT8)StrToInt(szArgv[1]);    
    AUD_PlayMuteCmdQuery(u1DecID);

    return 1;
}

static INT32 _AudCmdClearPlayMuteHistory(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecID;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc < 2)
    {
        Printf("c [DecID]\n");
        return -1;
    }    

    u1DecID = (UINT8)StrToInt(szArgv[1]);    
    AUD_PlayMuteClearHistory(u1DecID);

    return 1;
}

static INT32 _AudCmdClearPlayMuteQueryDelayInfo(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecID;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if (i4Argc < 2)
    {
        Printf("qd [DecID]\n");
        return -1;
    }    

    u1DecID = (UINT8)StrToInt(szArgv[1]); 
    AUD_PlayMuteQueryDelayInfo(u1DecID);

    return 1;
}

static INT32 _AudPlayMuteEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;

    if (i4Argc < 2)
    {
        Printf("Usage: e [flag]\n");
        Printf("flag: 0(disable). 1(enable)\n");
        return -1;
    }

    fgEnable = (BOOL)StrToInt(szArgv[1]);
    AUD_PlayMuteEnable(fgEnable);
    return 1;
}

#endif

static INT32 _AUDWatchSpdifEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;
    UNUSED(fgEnable);
    
    if (i4Argc < 2)
    {
        Printf("Usage: sb [flag]\n");
        Printf("flag: 0(disable). 1(enable)\n");
        return -1;
    }
    
    fgEnable = (BOOL)StrToInt(szArgv[1]);
#ifdef CC_AUD_DDI
    AUD_WatchSpdifEnable(fgEnable);
#endif
    return 1;
}

#include "vdec_if.h"
extern CHAR * _paszVdecFormat[];
extern INT16 AUD_GetAudioVdecDelay(ENUM_VDEC_FMT_T eCodecType);
extern VOID AUD_SetAudioVdecDelay(ENUM_VDEC_FMT_T eCodecType, INT16 i2Delay);
static INT32 _AUDSetVdecDelayTbl(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 i;

    ENUM_VDEC_FMT_T eCodecType;
    INT16 i2Delay;
    
    if (i4Argc != 3)
    {
        Printf("Usage: setdelay [Vdec] [Delay]\n");
        Printf("\n");

        for(i = 0; i < (UINT32)VDEC_FMT_MAX; i++)
        {
            Printf("%d: %s -- %d\n", i, _paszVdecFormat[i], AUD_GetAudioVdecDelay((ENUM_VDEC_FMT_T)i));
        }
        return -1;
    }

    eCodecType = (ENUM_VDEC_FMT_T)StrToInt(szArgv[1]);
    i2Delay = (INT16)i4StrToInt(szArgv[2]);
    
    AUD_SetAudioVdecDelay(eCodecType, i2Delay);
    return 1;
}
//PTSQueue_V2

#ifdef ADSP_BIN_SUPPORT
static INT32 _AudUsbBoot(INT32 i4Argc, const CHAR ** szArgv)
{
    vAdspBinMemInit();
	return 0;
}
#endif

extern BOOL _AUD_GetNotifyFunc(AudDecNfyFct * pfNfyFunc); 
static INT32 _AudUserDebugMask(INT32 i4Argc, const CHAR ** szArgv)
{
    AudDecNfyFct pfAudDecNfy = NULL;

    UINT32 u4DebugMask;

    if (i4Argc < 2)
    {
        Printf(" udm [DebugMask]\n");
        Printf("    (0001h) save log to file: /tmp/hal_audio.log.txt\n");
        Printf("    (0002h) HAL auido common log\n"); 
        Printf("    (0004h) HAL auido atv log\n"); 
        return -1;
    }

    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

    u4DebugMask = (UINT32)StrToInt(szArgv[1]);
    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_USER_DEBUG_MASK,
                    u4DebugMask, 0);
    }
    return 0;

}

static INT32 _AudQueryPtsQueue(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2Size;
    UINT8 u1DecID;

    if (i4Argc < 3)
    {
        Printf("qq [DecID][Size]\n");
        return -1;
    }

    u1DecID = (UINT8)StrToInt(szArgv[1]);
    u2Size = (UINT16)StrToInt(szArgv[2]);
    AUD_DrvPtsQueueQry(u1DecID, u2Size);
    return 0;
}
static INT32 _AudQueryPts(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Wp;

    if (i4Argc < 2)
    {
        Printf("qpts [Wp]\n");
        return -1;
    }

    u4Wp = (UINT32)StrToInt(szArgv[1]);
    AUD_DrvPtsQueueFindInfo(u4Wp);
    return 0;
}
#ifdef AUD_DECODER3_SELF_TEST
extern void AudSetDualPlay(UINT8 DecId, AV_SYNC_MODE_T eSyncMode); //parson MM3
extern UINT8 u1ForceDecId;
extern AV_SYNC_MODE_T eForceSyncMode;
static INT32 _AudCmdSetDualPlay(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId;
    static AV_SYNC_MODE_T eSyncMode;

    if (i4Argc < 3)
    {
        Printf("Usage: id  [decoder ID] [sync mode]\n");
        Printf("[decoder ID = 0/1/2/5] 5 = disable\n");
        Printf("[sync mode] 0 = free run\n");
        Printf("                   1 = slave\n");
        Printf("                   2 = audio master\n");
        if (u1ForceDecId != AUD_DEC_NUM)
        {
            Printf("current setting: ON, sync mode = %d\n",eForceSyncMode);
        }
        else
        {
            Printf("current setting: OFF, sync mode = %d\n",eForceSyncMode);
        }
        return -1;
    }
    u1DecId = (UINT8)StrToInt(szArgv[1]);
    eSyncMode = (UINT8)StrToInt(szArgv[2]);
    if (u1DecId ==5)
    {
        u1DecId = AUD_DEC_NUM;      //disable dual play
    }
    AudSetDualPlay(u1DecId, eSyncMode);
    return 1;
}
#endif

static INT32 _AudCmdUopG726Dec(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT8 u1DecId, u1rate;

    if (i4Argc != 3)
    {
        Printf("Usage: g726dec [decID] [rate]\n");
        Printf("[rate] 16/24/32/40, default 32\n");
        return -1;
    }

    u1DecId = (UINT8)StrToInt(szArgv[1]);
    u1rate = (UINT8)StrToInt(szArgv[2]);

    Aud_G726DecCfg(u1DecId, u1rate);

    return 1;
}

static INT32 _AudCmdUopCheckSum(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 total_chksum = 0;
    UINT32 u4Tmp = 0;
    UINT32 i = 0;
    UINT32 *ptr;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    u4Tmp = u4GetDspBinarySize(ADSP_BIN_MAIN)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_MAIN);
    for(i= 0; i < u4Tmp;i++)
    {
        total_chksum ^= ptr[i];
    }
    if(_u4pbdchecksum == total_chksum)
    {
        Printf("_pbD check sum right\n");
        Printf("_pbD check sum is : 0x%x\n", total_chksum);
    }
    else
    {
        Printf("_pbD check sum error\n");
        Printf("_pbD check sum is : 0x%x\n", total_chksum);
        Printf("_u4pbdchecksum is : 0x%x\n", _u4pbdchecksum);
    }
    
#if defined(CC_AUD_DOLBY_SUPPORT_DDC)
    u4Tmp = u4GetDspBinarySize(ADSP_BIN_DYNLD_DDC)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DDC);
    total_chksum = 0;
    for(i= 0; i < u4Tmp;i++)
    {
        total_chksum ^= ptr[i];
    }
    if(_u4_pbD_DDCchecksum == total_chksum)
    {
        Printf("_pbD_DDC check sum right\n");
        Printf("_pbD_DDC check sum is : 0x%x\n", total_chksum);
    }
    else
    {
        Printf("_pbD_DDC check sum error\n");
        Printf("_pbD_DDC check sum is : 0x%x\n", total_chksum);
        Printf("_u4_pbD_DDCchecksum is : 0x%x\n", _u4_pbD_DDCchecksum);
    }
#elif defined(CC_AUD_DOLBY_SUPPORT_DDP)
    u4Tmp = u4GetDspBinarySize(ADSP_BIN_DYNLD_DCV)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DCV);
    total_chksum = 0;
    for(i= 0; i < u4Tmp;i++)
    {
        total_chksum ^= ptr[i];
    }
    if(_u4_pbD_DCVchecksum == total_chksum)
    {
        Printf("_pbD_DCV check sum right\n");
        Printf("_pbD_DCV check sum is : 0x%x\n", total_chksum);
    }
    else
    {
        Printf("_pbD_DCV check sum error\n");
        Printf("_pbD_DCV check sum is : 0x%x\n", total_chksum);
        Printf("_u4_pbD_DCVchecksum is : 0x%x\n", _u4_pbD_DCVchecksum);
    }
#elif defined(CC_AUD_DOLBY_SUPPORT_DD)
    u4Tmp = u4GetDspBinarySize(ADSP_BIN_DYNLD_DD)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DD);
    total_chksum = 0;
    for(i= 0; i < u4Tmp;i++)
    {
        total_chksum ^= ptr[i];
    }
    if(_u4_pbD_DDchecksum == total_chksum)
    {
        Printf("_pbD_DD check sum right\n");
        Printf("_pbD_DD check sum is : 0x%x\n", total_chksum);
    }
    else
    {
        Printf("_pbD_DD check sum error\n");
        Printf("_pbD_DD check sum is : 0x%x\n", total_chksum);
        Printf("_u4_pbD_DDchecksum is : 0x%x\n", _u4_pbD_DDchecksum);
    }
#endif
// AAC
#if defined(CC_AUD_DOLBY_SUPPORT_DDT)
    u4Tmp = u4GetDspBinarySize(ADSP_BIN_DYNLD_DDT)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DDT);
    total_chksum = 0;
    for(i= 0; i < u4Tmp;i++)
    {
        total_chksum ^= ptr[i];
    }
    if(_u4_pbD_DDTchecksum == total_chksum)
    {
        Printf("_pbD_DDT check sum right\n");
        Printf("_pbD_DDT check sum is : 0x%x\n", total_chksum);
    }
    else
    {
        Printf("_pbD_DDT check sum error\n");
        Printf("_pbD_DDT check sum is : 0x%x\n", total_chksum);
        Printf("_u4_pbD_DDTchecksum is : 0x%x\n", _u4_pbD_DDTchecksum);
    }
#elif defined(CC_AUD_DOLBY_SUPPORT_AAC)
    u4Tmp = u4GetDspBinarySize(ADSP_BIN_DYNLD_AAC)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_AAC);
    total_chksum = 0;
    for(i= 0; i < u4Tmp;i++)
    {
        total_chksum ^= ptr[i];
    }
    if(_u4_pbD_AACchecksum == total_chksum)
    {
        Printf("_pbD_AAC check sum right\n");
        Printf("_pbD_AAC check sum is : 0x%x\n", total_chksum);
    }
    else
    {
        Printf("_pbD_AAC check sum error\n");
        Printf("_pbD_AAC check sum is : 0x%x\n", total_chksum);
        Printf("_u4_pbD_AACchecksum is : 0x%x\n", _u4_pbD_AACchecksum);
    }
#endif
// DDCO
#if defined(CC_AUD_DOLBY_SUPPORT_DDCO)
    u4Tmp = u4GetDspBinarySize(ADSP_BIN_DYNLD_DDCO)/4;
    ptr = (UINT32 *)u4GetDspBinaryAddr(ADSP_BIN_DYNLD_DDCO);
    total_chksum = 0;
    for(i= 0; i < u4Tmp;i++)
    {
        total_chksum ^= ptr[i];
    }
    if(_u4_pbD_DDCOchecksum == total_chksum)
    {
        Printf("_pbD_DDCO check sum right\n");
        Printf("_pbD_DDCO check sum is : 0x%x\n", total_chksum);
    }
    else
    {
        Printf("_pbD_DDCO check sum error\n");
        Printf("_pbD_DDCO check sum is : 0x%x\n", total_chksum);
        Printf("_u4_pbD_DDCOchecksum is : 0x%x\n", _u4_pbD_DDCOchecksum);
    }
#endif


    return 1;
}


/******************************************************************************
* Variable      : cli "aud" set command table
******************************************************************************/

static CLI_EXEC_T _arAudClipCmdTbl[] =
{
    {"play",    "p",    _AudCmdClipPlay,    NULL,   "play aud clip",    CLI_GUEST},
    {"stop",    "s",    _AudCmdClipStop,    NULL,   "stop aud clip",    CLI_GUEST},
    {"volume",  "v",    _AudCmdClipVolume,  NULL,   "set clip volume",    CLI_GUEST},

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudTestCmdTbl[] =
{
    {"tone",        "t",     _AudCmdPlayTone, NULL,      "play tone", CLI_SUPERVISOR},
    {"stoptone",    "stop",  _AudCmdStopMemStr, NULL,    "stop tone", CLI_SUPERVISOR},
    {"utone",       "utone", _AudCmdUniversalTone, NULL, "play universal tone", CLI_SUPERVISOR},
    {"soundeffect", "s",    _AudCmdSoundEffect, NULL,       "sound effect on/off", CLI_SUPERVISOR},
    {"bypass",      "b",    _AudCmdBypassPostProcessing, NULL,      "bypass post processing", CLI_GUEST},
#ifdef __MODEL_slt__ // by hotkey 0729 for compile
    {"SltSorting",  "slt",  _AudCmdSltSorting, NULL,    "slt sorting", CLI_SUPERVISOR},
    {"DtvSlt",  "dtvslt",   _AudCmdDtvSltSorting, NULL, "Dtv slt sorting", CLI_SUPERVISOR},
    {"AtvSlt",  "atvslt",   _AudCmdAtvSltSorting, NULL, "Atv slt sorting", CLI_SUPERVISOR},
    {"IntAdSlt",  "iadslt",   _AudCmdIntAdSltSorting, NULL, "Internal AD slt sorting", CLI_SUPERVISOR},
    {"SltFlashing", "flash",    _AudCmdSltFlashing, NULL,   "burning slt pattern to flash", CLI_SUPERVISOR},
    {"SltROM", "romslt",    _AudCmdROMSltSorting, NULL,   "ROM test", CLI_SUPERVISOR},
#ifndef CC_AUD_DISABLE_2ND_DSP    
    {"SltDSPB", "dspbslt",    _AudCmdDSPBSltSorting, NULL,   "DSPB test", CLI_SUPERVISOR},  
#endif      
#endif
    {"dd_banner",   "banner",   _AudCmdDDbanner, NULL,  "DD banner turn on for DD Test", CLI_GUEST },
    {"memalloc",    "mal",  _AudCmdMemAlloc, NULL,  "Allocate memory for test", CLI_SUPERVISOR },
#ifdef CC_AUD_SKYPE_SUPPORT
    {"skpringmix",  "skprmix",    _AudSkypeMixRingSnd, NULL,  "skype ring sound mix", CLI_GUEST},
    {"skptxmix",  "skptxmix",    _AudSkypeAuxMix2Main, NULL,  "skype local sound mix", CLI_GUEST},
#endif
    {"AoutGain", "aoutg", _AudAoutGain, NULL, "Aout gain control" , CLI_SUPERVISOR},
#ifdef CC_AUD_PCM_LINE_IN_LPF
    {"pcmlineinlpf",  "pcmlpf",    _AudDSPSetPCMLineInLPF, NULL,  "PCM Line-in LPF", CLI_GUEST},
#endif
#ifdef CC_AUD_HDMI_SPDIF_CFG
    {"hdmispdif",   "hdmispdif", _AudHDMICfgSPDIFCLI, NULL, "HDMI Configure SPDIF Test", CLI_SUPERVISOR},
#endif
#if 0 //For IC/FPGA Verification
    {"stc",      "stc",    _AudCmdTestSTC, NULL,      "Test STC Read Write", CLI_GUEST}, //IC_FPGA_STV_VERIF
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudSpdifCmdTbl[] =
{
    {"init",        "i",    _SpdifCmdInit, NULL,        "Spdif-in init", CLI_SUPERVISOR},
    {"reset",       NULL,   _SpdifCmdReset, NULL,       "Spdif-in reset", CLI_SUPERVISOR},
    {"enable",      "e",    _SpdifCmdEnable, NULL,      "Spdif-in enalbe", CLI_SUPERVISOR},
    {"disable",     "d",    _SpdifCmdDisable, NULL,     "Spdif-in diable", CLI_SUPERVISOR},
    {"checklock",   "c",    _SpdifCmdCheckLock, NULL,   "Spdif-in check locked?", CLI_SUPERVISOR},
    {"selectch",    "s",    _SpdifCmdSelectCh, NULL,    "Spdif-in select channel", CLI_SUPERVISOR},
    {"readrg",      "r",    _SpdifCmdReadReg, NULL,     "Spdif-in read registers ex. readrg", CLI_SUPERVISOR},
    {"writerg",     "w",    _SpdifCmdWriteReg, NULL,    "Spdif-in write registers ex. writerg [addr] [value]", CLI_SUPERVISOR},
    {"iec_set",     "iecs", _SpdifIecSet, NULL,        "IEC setting [decoder id] [flag] [enable]", CLI_GUEST},
    {"iec_ch",      "iecc", _SpdifIecChannelSet, NULL, "IEC Channel setting [decoder id] [channel]", CLI_GUEST},
    {"iec_bypass",  "iecb", _SpdifAlwaysOutHdmi, NULL, "IEC output HDMI always [enable]", CLI_SUPERVISOR},
    {"iec_query",   "iecq", _SpdifIecQuery, NULL, "IEC setting query", CLI_GUEST},
    {"iecraw_delay_flag",   "flag", _SpdifSetRawDelayFlag, NULL, "IEC RAW delay sync with channel delay control", CLI_SUPERVISOR},
    {"iecraw_delay",   "delay", _SpdifSetRawDelay, NULL, "IEC RAW delay", CLI_SUPERVISOR},
    {"info",        "info", _SpdifGetInfo, NULL,        "SPDIF get setup", CLI_SUPERVISOR},
    {"resetreg",        "resetreg",  _SpdifRegTypeReset, NULL,        "SPDIF reset registers", CLI_SUPERVISOR},
    {"copy",        "cpy",  _SpdifCopyrightSet, NULL,        "SPDIF copyright setup", CLI_SUPERVISOR},
    {"code",        "code", _SpdifCategoryCodeSet, NULL,        "SPDIF category code setup", CLI_SUPERVISOR},
    {"word",        "word", _SpdifWordLengthSet, NULL,        "SPDIF word length setup", CLI_SUPERVISOR},
    {"setInfo",     "set",  _SpdifSetInfo, NULL,        "SPDIF information setup", CLI_SUPERVISOR},
    {"setProtect",     "protect",  _SpdifSetProtect, NULL,        "SPDIF copy protect setup", CLI_SUPERVISOR},
    {"update_mode",     "mode",  _SpdifUpdateModeSet, NULL,        "SPDIF set update mode", CLI_SUPERVISOR},
    {"channelstatus",     "cs",  _SpdifGetChannelStatus, NULL,        "SPDIF get channel status", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudAdacCmdTbl[] =
{
    {"query",      "q",    _AdacCmdQuery, NULL,   "Query speaker enable state", CLI_SUPERVISOR},
    {"format",      "f",    _AdacCmdDataFormat, NULL,   "Data format ex. format 0 1", CLI_SUPERVISOR},
    {"datainvert",  "d",    _AdacCmdDataInvert, NULL,   "Data invert ex.  datainvert 0 1", CLI_SUPERVISOR},
    {"drvcurrent",  "drv",  _AdacCmdDrvCur, NULL,       "Select MCLK BCLK driving current ex. drv 0 8 8", CLI_GUEST},
    {"CodecRead",   "cr",   _AdacCmdCodecRead, NULL,    "WM8776/MT8291 register read", CLI_SUPERVISOR},
    {"CodecWrite",  "cw",   _AdacCmdCodecWrite, NULL,   "WM8776/MT8291 register write", CLI_SUPERVISOR},
    {"CodecChSel",  "ch",   _AdacCmdCodecChlSel, NULL,  "WM8776/MT8291 register write", CLI_SUPERVISOR},
    {"AmpRead",     "ar",   _AdacCmdAmpRead, NULL,  "Amplifier register read", CLI_SUPERVISOR},
    {"AmpWrite",    "aw",   _AdacCmdAmpWrite, NULL, "Amplifier register write", CLI_SUPERVISOR},
    {"AmpReadN",     "nr",   _AdacCmdAmpReadN, NULL,  "Amplifier register readn", CLI_SUPERVISOR},
    {"AmpWriteN",    "nw",   _AdacCmdAmpWriteN, NULL, "Amplifier register writen", CLI_SUPERVISOR}, 
    {"AmpInit",     "ai",   _AdacCmdAmpInit, NULL, "Amplifier init", CLI_SUPERVISOR},
    {"AmpMute",     "am",   _AdacCmdAmpMute, NULL, "Amplifier mute control", CLI_GUEST},
    {"AmpRamRead",  "arr",  _AdacCmdAmpRamRead, NULL, "Amplifier ram read", CLI_SUPERVISOR},
    {"AmpRamWrite", "awr",  _AdacCmdAmpRamWrite, NULL, "Amplifier ram write", CLI_SUPERVISOR},
    // last cli command record, NULL
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};

static CLI_EXEC_T _arAudInterDacCmdTbl[] =
{
    {"channel sel", "ch",   _InterDacChlSel,    NULL,   "Internal DAC channel Select", CLI_GUEST},
    {"mute",        "m",    _InterDacMute,      NULL,   "Internal DAC Mute", CLI_GUEST},
    {"enable",      "e",    _InterDacEnable,    NULL,   "Internal DAC Enable", CLI_SUPERVISOR},
    {"init",        "i",    _InterDacCmdInit,   NULL,   "Internal DAC Init", CLI_SUPERVISOR},
    {"LRInvert",    "inv",  _InterDacLRInvert,   NULL,  "Internal LR invert", CLI_SUPERVISOR},
    {"PWMenable",   "pwmen",  _InterPWMEnable,   NULL,  "PWM enable OR disable", CLI_SUPERVISOR},
    {"PWMgainSet",  "pg",   _InterPWMGain,   NULL,      "Set PWM gain", CLI_SUPERVISOR},
    {"Monitor Set",  "mt",   _InterMonitorSet,   NULL,      "Set Moniter Status", CLI_SUPERVISOR},
    {"PWMDelay",  "pd",   _InterPWMDelay,   NULL,       "Set PWM Delay", CLI_SUPERVISOR},
    {"PWMDACYRAMRead",  "pwmr",   _InterPWMDACYRAMRead,   NULL,     "PWMDAC Y YRAM Read", CLI_SUPERVISOR}, 
    {"PWMDACYRAMWrite",  "pwmw",   _InterPWMDACYRAMWrite,   NULL,   "PWMDAC Y YRAM Write", CLI_SUPERVISOR},
    {"PWMDACRead",  "dacr",   _InterPWMDACRead,   NULL,     "PWMDAC Read", CLI_SUPERVISOR}, 
    {"PWMDACWrite",  "dacw",   _InterPWMDACWrite,   NULL,   "PWMDAC Write", CLI_SUPERVISOR},    
    {"PWMSigGen",  "pwmsg",   _InterPWMSigGen,  NULL,   "PWMSigGen", CLI_SUPERVISOR}, 
    {"IntLDOen",    "ldo",  _InterLDOEnable,   NULL,    "internal LDO Enable OR disable", CLI_SUPERVISOR},
    //{"ramp",        "rmp",  _InterDacRamp,      NULL, "Internal DAC Ramp", CLI_GUEST},  // for pwm ramp function test
#ifdef XY_RAM_TEST
    {"XYRamRead",   "xyr",  _InterDacXYRamRead, NULL,   "Internal DAC XY Ram Read", CLI_SUPERVISOR},  //fro PWM xy ram test
    {"XYRamTest",   "xyt",  _InterDacXYRamTest, NULL,   "Internal DAC XY Ram Test", CLI_SUPERVISOR},  //fro PWM xy ram test
#endif
    // last cli command record, NULL
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


static CLI_EXEC_T _arAudAadcCmdTbl[] =
{
    {"mute",        "m",    _AADCMute,      NULL,   "AADC Mute PGA", CLI_GUEST},
    {"powerdown",   "pd",   _AADCDown,      NULL,   "AADC power down", CLI_GUEST},
    {"gain",        "gain", _AADCGain,      NULL,   "AADC power down", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#if 1 //APE SUPPORT
static CLI_EXEC_T _arAudAPECmdTbl[] =
{
    {"apeinit", "apeinit", _AudCmdapeinit, NULL, "apeinit", CLI_SUPERVISOR},
    {"apeps",        "ps",   _AudCmdapeparser,  NULL,       "apeheaderparser",  CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudProbeCmdTbl[] =
{
    {"pause",       "p",    _AudCmdProbePause,  NULL,       "Probe: Pause", CLI_SUPERVISOR},
    {"step",        "st",   _AudCmdProbeStep,  NULL,       "Probe: Step",  CLI_SUPERVISOR},
    {"go",          "g",    _AudCmdProbeGo,     NULL,       "Probe: Go",    CLI_SUPERVISOR},
    {"read",        "r",    _AudCmdProbeRead,   NULL,       "Probe: Read",  CLI_SUPERVISOR},
    {"write",       "w",    _AudCmdProbeWrite,   NULL,      "Probe: Write",  CLI_SUPERVISOR},
    {"ShowRegs",    "sr",   _AudCmdShowRegs,   NULL,       "Probe: Show Registers",    CLI_SUPERVISOR},
    {"SetBPts",     "stb",  _AudCmdSetBPts,   NULL,       "Probe: Setup BreakPoints", CLI_SUPERVISOR},
    {"ShowBPts",    "sb",   _AudCmdShowBPts,   NULL,       "Probe: Show BreakPoints",  CLI_SUPERVISOR},
    {"StopOnWrite", "sw",   _AudCmdStopOnWrite, NULL,       "Probe: Stop on Write",   CLI_SUPERVISOR},
    {"gsr",         "gsr",  _AudCmdProbeGoThenShow, NULL, "Probe: Go then Show",   CLI_SUPERVISOR},
    {"stsr",        "stsr", _AudCmdProbeStepThenShow, NULL, "Probe: Go then Show",   CLI_SUPERVISOR},
    //ZY_FPGA
    {"sramtest",    "sramtest", _AudCmdProbeSramTest, NULL, "Sram Test", CLI_SUPERVISOR},
    {"reset",       "reset", _AudCmdProbeReset, NULL, "Reset DSP", CLI_SUPERVISOR},
    //ZY_FPGA
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudDspCmdTbl[] =
{
    {"ptr",         "p",    _AdspCmdPtr, NULL,      "get AFIFO read/write pointer cmd ex. ptr", CLI_SUPERVISOR},
    {"init",        "i",    _AdspDSPInit, NULL,     "adsp task init ex. init", CLI_SUPERVISOR},
    {"pc",          NULL,   _AdspDSPPC, NULL,       "Get dsp pc ex. pc", CLI_GUEST},
    {"probe",       "pr",   NULL,  _arAudProbeCmdTbl, "dsp probe command", CLI_SUPERVISOR },
    {"reads",       "r",    _AdspReadSram, NULL,    "Read DSP sram ex. reads [addr]", CLI_GUEST},
    {"writes",      "w",    _AdspWriteSram, NULL,   "Write DSP sram ex. write [addr] [data]", CLI_GUEST},
    {"updateReadPts",   "rpts", _AdspUpdateReadPtr, NULL,       "update read ptr ex.rpts [DecId] [Addr]", CLI_SUPERVISOR},
    {"SetLogPeriod",    "sp",   _AudCmdSetLogPeriod, NULL,      "set error log period ex.sp [second] ", CLI_SUPERVISOR},
    {"shareinfo",   "sh",   _AdspDSPShareInfo, NULL,        "read dsp share memory ex.sh [group] ", CLI_GUEST},
    {"commondram",  "cm",   _AdspDSPCommonDram, NULL,       "read dsp common memory ex.cm [addr] [length] ", CLI_SUPERVISOR},
    {"commondram",  "cmw",  _AdspDSPCommonDramWrite, NULL,       "write dsp common memory ex.cm [addr] [value] ", CLI_SUPERVISOR},
    {"normdram",  "norm",   _AdspDSPNormDram, NULL,       "read dsp normal dram", CLI_SUPERVISOR},
    {"cmptdram",  "cmpt",   _AdspDSPCmptDram, NULL,       "read dsp compact dram", CLI_SUPERVISOR},
    {"query",       "q",    _AdspDSPQuery, NULL,        "query dsp status", CLI_GUEST},
#ifdef BURN_IN_AOUT_TEST
    {"Aout CRC query",  "qaout",  _AdspAoutQuery, NULL,        "query aout status", CLI_GUEST},
    {"Aout CRC reset",  "raout",   _AdspAoutReset, NULL,        "Reset aout status", CLI_GUEST},
    {"Aout CRC set",  "saout",    _AdspAoutSet, NULL,        "Set aout test", CLI_GUEST},
#endif
    {"shareinfowrite",  "shw",  _AdspDSPSharInfoWrite, NULL,        "write dsp share memory ex.sh [group][addr][val][size] ", CLI_SUPERVISOR},
    {"chldump",     "c",    _AdspDSPChlDump, NULL,      "dump channel ouput data", CLI_GUEST},
    {"StcDiffBound",   "stcbd", _AdspDSPStcDiffBound, NULL, "set stc difference bound", CLI_SUPERVISOR},
    {"query mts",       "qmts",    _AdspMTSQuery, NULL,        "query mts status", CLI_SUPERVISOR},
    {"query vorbis",    "qvobs",    _AdspVorbisQuery,   NULL,   "query vorbis status",  CLI_SUPERVISOR},
    {"pcm pre-buffer",       "pb",    _AudPreBuffer, NULL,        "pcm pre-buffer", CLI_SUPERVISOR},
    {"cache",       "cache",    _AudCacheHitRate, NULL,        "DSP cache hit rate", CLI_SUPERVISOR},
    {"ROMID",       "id",    _AdspDSPROMID, NULL,        "ROM ID", CLI_SUPERVISOR},
    {"qm",          "qm",    _AudCmdQueryDspMemory, NULL, "Query DSP Memory", CLI_SUPERVISOR},
    {"ver",         "v",    _AudCmdDspVer,          NULL,  "DSP Version Display", CLI_GUEST},
#ifndef CC_AUD_DISABLE_DSP_PAT
    {"pat", "pat", _AudCmdPatTest, NULL, "Pattern Test", CLI_SUPERVISOR},
    {"patauto", "patauto", _AudCmdPatAuto, NULL, "Pattern Auto", CLI_SUPERVISOR},
    {"patsingle", "pats", _AudCmdPatSingle, NULL, "Pattern Single", CLI_SUPERVISOR},
        
    //{"fillbuf", "fillbuf", _AudCmdFillBuf, NULL, "Fill AOUT Buf", CLI_SUPERVISOR},
#endif
    {"Query Delay",   "qdly",   _AdspQChannelDelay, NULL,  "Query Channel Delay", CLI_GUEST},
    {"dinfo",   "dinfo", _AudCmdDebug1, NULL,  "DEBUG INFO ~~", CLI_SUPERVISOR},
    {"Get capability", "cap",   _AdspGetCap, NULL, "Get DSP supported decoding format", CLI_SUPERVISOR},
#if 1 //CC_APE_SUPPORT
    {"ape", NULL,   NULL,  _arAudAPECmdTbl, "APE Operate command", CLI_SUPERVISOR },
#endif
    {"Dumpint", "int",_AUD_DSPDumpIntHistory, NULL, "Dump Interrupt History", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_AUD_ARM_SUPPORT

#ifdef CC_AUD_ARM_RENDER
static CLI_EXEC_T _arAudAprocIecCmdTbl[] =
{
    {"iecflag",      "flag",    _AprocIecFlag, NULL,   "iec flag", CLI_GUEST},
    {"ieccs",      "cs",    _AprocIecCs, NULL,   "iec channel status", CLI_GUEST},  
    {"iecprotect",      "protect",    _AprocIecCopyProtect, NULL,   "iec copy protect", CLI_GUEST},
    {"iecrawinfo",      "raw",    _AprocIecRawInfo, NULL,   "iec raw information", CLI_GUEST},
    {"iecrawinfo",      "audraw",    _AprocIecRawInfoAudFmt, NULL,   "iec raw information by audio format", CLI_GUEST},
    {"iecmute",      "mute",    _AprocIecMute, NULL,   "iec mute", CLI_GUEST},
    {"iecdelay",      "delay",    _AprocIecRawDelay, NULL,   "iec RAW delay", CLI_GUEST},
    {"iecpcmch",      "ch",    _AprocIecPcmChSel, NULL,   "iec PCM channel selection", CLI_GUEST},
    {"iecout",      "out",    _AprocIecOutputSel, NULL,   "iec output selection", CLI_GUEST},
    {"iecpcmbit",      "bit",    _AprocIecPcmBitsSel, NULL,   "iec PCM output bits", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudSpostCmdTbl[] =
{
    {"dbg",        "dbg",   _SpostDbg, NULL,     "Spost debug", CLI_GUEST},
    {"bypass",     "b",   _SpostBypass, NULL,     "bypass all post", CLI_GUEST},
    {"logout",     "lo",   _SpostLogoutEnable, NULL,     "Enable Log out", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif //CC_AUD_ARM_RENDER
#ifdef ARM_SUPPORT_SRS_SS
static CLI_EXEC_T _arAudAprocSRSTbl[] =
{
    {"SRS AEQ",        "aeq",    _AprocPostSRSAEQ, NULL,     "SRS AEQ Setting", CLI_SUPERVISOR},
    {"SRS BassTreble",          "bt",   _AprocPostSRSBT, NULL,       "SRS BassTreble Setting", CLI_SUPERVISOR},
    {"SRS CC3D",       "cc3d",    _AprocPostSRSCC3D, NULL,    "SRS CC3D Setting", CLI_GUEST},
    {"SRS CSHP",      "cshp",    _AprocPostSRSCSHP, NULL,   "SRS CSHP Setting", CLI_GUEST},
    {"SRS GEQ",      "geq",    _AprocPostSRSGEQ, NULL,   "SRS GEQ Setting", CLI_GUEST},
    {"SRS HardLimiter",      "hl",    _AprocPostSRSHL, NULL,   "SRS HardLimiter Setting", CLI_GUEST},
    {"SRS HPF",      "hpf",    _AprocPostSRSHPF, NULL,   "SRS HPF Setting", CLI_GUEST},
    {"SRS Mvol",        "mvol",    _AprocPostSRSMVOL, NULL,     "SRS Mvol Setting", CLI_SUPERVISOR},
    {"SRS SpaceHpf",          "shpf",   _AprocPostSRSSPHPF, NULL,       "SRS SpaceHpf Setting", CLI_SUPERVISOR},
    {"SRS SRS3D",       "srs3d",    _AprocPostSRSSRS3D, NULL,    "SRS SRS3D Setting", CLI_GUEST},
    {"SRS SS3D",      "ss3d",    _AprocPostSRSSS3D, NULL,   "SRS SS3D Setting", CLI_GUEST},
    {"SRS STTSHD",      "tshd",    _AprocPostSRSTSHD, NULL,   "SRS STTSHD Setting", CLI_GUEST},
    {"SRS TBHD",      "tbhd",    _AprocPostSRSTBHD, NULL,   "SRS TBHD Setting", CLI_GUEST},
    {"SRS Trudialog",      "trud",    _AprocPostSRSTRUD, NULL,   "SRS Trudialog Setting", CLI_GUEST},
    {"SRS TvolHD",      "tvol",    _AprocPostSRSTVOL, NULL,   "SRS TvolHD Setting", CLI_GUEST},
    {"SRS init",      "init",    _AprocPostSRSINIT, NULL,   "Only for SRS Cert", CLI_GUEST},
    {"SRS ALL Value",      "getall",    _AprocPostSRSGETALL, NULL,   "Only for Adjust", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef CC_AUD_SUPPORT_DBXTV
static CLI_EXEC_T _arDbxTVCmdTbl[] =
{
    {"enable",      "en",     _AudDBXTVEnable,   NULL,       "Enable/Disable DBX-TV processing", CLI_GUEST},
    {"mode",      "m",     _AudDBXTVSetMode,   NULL,       "Setting DBX-TV Mode", CLI_GUEST},
    {"read",      "r",     _AudDBXTVRead,   NULL,       "Read Parameter or Coefficient", CLI_GUEST},
    {"write",      "w",     _AudDBXTVWrite,   NULL,       "Write Parameter or Coefficient", CLI_GUEST},
    //{"testenble",      "ten",     _AudDBXTVTestEnable,   NULL,       "Enable/Disable Total Sonics Test Modet", CLI_GUEST},        
    {"testmode",      "tm",     _AudDBXTVTestMode,   NULL,       "Setting Total Sonics Test Mode", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}

};
#endif

#ifdef CC_AUD_ARM_RENDER
static CLI_EXEC_T _arAudAprocIECCmdTbl[] =
{
	{"iecflag",      "iecflag",    _AprocIecFlag, NULL,   "iec flag", CLI_GUEST},
    {"ieccs",      "ieccs",    _AprocIecCs, NULL,   "iec channel status", CLI_GUEST},  
    {"iecprotect",      "iecpro",    _AprocIecCopyProtect, NULL,   "iec copy protect", CLI_GUEST},
    {"iecrawinfo",      "iecraw",    _AprocIecRawInfo, NULL,   "iec raw information", CLI_GUEST},
    {"iecmute",      "iecmute",    _AprocIecMute, NULL,   "iec mute", CLI_GUEST},
    {"iecdelay",      "iecdelay",    _AprocIecRawDelay, NULL,   "iec RAW delay", CLI_GUEST},
    {"iecpcmch",      "iecc",    _AprocIecPcmChSel, NULL,   "iec PCM channel selection", CLI_GUEST},
    {"iecout",      "ieco",    _AprocIecOutputSel, NULL,   "iec output selection", CLI_GUEST},
    {"iecpcmbit",      "iecbit",    _AprocIecPcmBitsSel, NULL,   "iec PCM output bits", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


static CLI_EXEC_T _arAudAprocAmixerCmdTbl[] =
{
	{"amxopen",      "ao",    _AprocCmdAmxierOpen, NULL,   "amixer open", CLI_GUEST}, 
    {"amxclose",      "ac",    _AprocCmdAmxierClose, NULL,   "amixer close", CLI_GUEST}, 
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudAprocVolumeCmdTbl[] =
{
	{"Postvol",      "pv",    _AprocPostProcVol, NULL,   "post-processing vol", CLI_GUEST},
	{"Master volume",  "mv",    _AprocPostProcMstVolume, NULL,   "Master volume setting", CLI_GUEST}, 
    {"Input Trim",    "ipt",    _AprocPostProcInputTrim, NULL,   "Input trim setting", CLI_GUEST}, 
    {"Output Trim",    "opt",    _AprocPostProcOutputTrim, NULL,   "Output trim setting", CLI_GUEST},
    {"Amixer Trim",    "apt",    _AprocPostProcAmixerTrim, NULL,   "Amixer trim setting", CLI_GUEST}, 
    {"Output Volume",    "opv",    _AprocPostProcOutputVol, NULL,   "Output trim setting", CLI_GUEST},
#ifdef CC_AUD_ARM_RENDER
    {"Sgain",      "sg",    _AprocPostProcSgain, NULL,   "post-processing smooth gain", CLI_GUEST}, 
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


static CLI_EXEC_T _arAudAprocSndEfftCmdTbl[] =
{
	{"BassTreble",    "pbt",    _AprocPostProcBassTreble, NULL,   "post-processing Bass Treble", CLI_GUEST},
	{"Balancer",    "blc",    _AprocPostProcBalancer, NULL,   "Balancer setting", CLI_GUEST}, 
	#ifdef CC_AUD_ARM_RENDER    
    {"Bass Management",    "bmang",    _AprocPostProcBmang, NULL,   "Bass Managemant setting", CLI_GUEST},     
    {"Virtual Surround",    "mvs",    _AprocPostProcMVS, NULL,   "Virtual Surround Setting", CLI_GUEST},     
    {"Virtual Bass",        "mvb",    _AprocPostProcMVB, NULL,   "Virtual Bass setting", CLI_GUEST},     
    {"PEQ",           "peq",    _AprocPostProcPEQ, NULL,   "PEQ setting", CLI_GUEST},     
    {"DRC Limiter",         "dl",     _AprocPostProcDRCLimiter, NULL,   "DRC limiter setting", CLI_GUEST},     
    {"8Band EQ",            "eq",     _AprocPostProc8BandEQ, NULL,   "8Band EQ setting", CLI_GUEST},    
	{"Eq Spectrum",         "spec",     _AprocPostProcEqSpectrum, NULL,   "EQ spectrum enable", CLI_GUEST},
	{"monomix", 	 "monomix",    _AprocPostProcMonoMixing, NULL,	 "post-processing Mono mixing", CLI_GUEST}, 
    #endif 
	{"postq",      "pq",    _AprocPostProcQuery, NULL,   "post-processing query", CLI_GUEST},
	{"3 band DRC",    "3drc",    _AprocPostProc3Drc, NULL,   "3band DRC setting", CLI_GUEST},    
    {"1 band DRC",    "1drc",    _AprocPostProc1Drc, NULL,   "1band DRC setting", CLI_GUEST}, 

	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


static CLI_EXEC_T _arAudAprocCmdTbl[] =
{
    {"init",        "i",    _AprocProcnit, NULL,     "aproc task init ex. init", CLI_SUPERVISOR},
    {"pc",          NULL,   _AprocProcPC, NULL,       "Get aproc pc ex. pc", CLI_SUPERVISOR},
    {"reads",       "r",    _AprocRead, NULL,    "Read aproc ex. reads [addr]", CLI_GUEST},
    {"writes",      "w",    _AprocWrite, NULL,   "Write aproc ex. write [addr] [data]", CLI_GUEST},
    {"open",      "o",    _AprocCmdOpen, NULL,   "command: open", CLI_GUEST},
    {"notifyadsp",      "nod",    _AprocNotifyAdsp, NULL,   "command: notify adsp", CLI_GUEST},
    {"writeadsp",      "wd",    _AprocWriteAdsp, NULL,   "command: write adsp", CLI_GUEST},
    {"regread",       "rr",    _AprocRegRead, NULL,    "Reg read aproc ex. reads [addr]", CLI_GUEST},
    {"refwrite",      "rw",    _AprocRegWrite, NULL,   "Reg write aproc ex. write [addr] [data]", CLI_GUEST},   
    
    
    {"qmem",      "qm",    _AprocMemMapQuery, NULL,   "memory map query", CLI_GUEST},
    {"enable",      "enable",    _AprocEnable, NULL,   "enable/disable audio processor", CLI_GUEST},
#if defined(CC_AUD_ARM11_SUPPORT)
    {"ice",      "ice",    _AprocIceControl, NULL, "connect ICE", CLI_GUEST},
    {"modctrl",      "modctrl",    _AprocModuleCtrl, NULL, "module control", CLI_GUEST},
#else
    {"ice",      "ice",    _AprocIceControl, NULL,   "ICE in serial or not serial mode", CLI_GUEST},
#endif // CC_AUD_ARM11_SUPPORT
#ifdef CC_AUD_ARM_RENDER
    {"poweron",      "pon",    _AprocPowerOn, NULL,   "aproc power-on", CLI_GUEST}, 
    {"powerdown",      "pdown",    _AprocPowerDown, NULL,   "aproc power-down", CLI_GUEST}, 
	{"amxier",   "amx",    NULL, _arAudAprocAmixerCmdTbl,        "audio processor amixer command", CLI_SUPERVISOR },
    {"admix",      "ad",    _AprocPostProcAdMixing, NULL,   "post-processing AD mixing", CLI_GUEST}, 
	{"iec",   "iec",    NULL, _arAudAprocIECCmdTbl,        "audio processor iec command", CLI_SUPERVISOR },
    {"query",       "q",    _AprocQuery, NULL,        "query aproc", CLI_GUEST},    
    {"mixsound",    "ms",    _AprocMixsndTest, NULL,   "Aproc MixSound Test", CLI_GUEST},     
#if defined(CC_AUD_APROC_KEYSOUND)
    {"keysound",    "ks",    _AprocKeysndTest, NULL,   "Aproc KeySound Test", CLI_GUEST},     
#endif
    {"upload",    "up",    _AprocUploadTest, NULL,   "Aproc Upload Test", CLI_GUEST},             
    {"riscpost",  "rp",    _AprocRiscPostTest, NULL,   "Aproc RiscPost Test", CLI_GUEST},    
    {"aproc",   "acmd", _AudCmdAproc, NULL, "Ask APROC AVSync Command", CLI_GUEST},    
    {"logid",   "ld", _AudCmdLogID, NULL, "Print Log for which decoder", CLI_GUEST},    
    {"syncinfo",   "si", _AudCmdSyncInfo, NULL, "List AVSync information", CLI_GUEST},
#ifdef CC_DUAL_AUD_DEC_SUPPORT
    {"swap focus",    "swp",    _AprocSwapFocus, NULL,   "Swap SP/HP output", CLI_GUEST},
	{"skip list",  "sl", _AprocShowSkipList, NULL, "Show skip command list", CLI_GUEST},
#endif    
#endif //   CC_AUD_ARM_RENDER 
    
    {"2.2/4.0 sel",    "chs",    _AprocPostProcChSel, NULL,   "2.2/4.0 out select", CLI_GUEST},     
	{"amixer mute",     "am",		_AprocPostProcAmixerMute, NULL,	 "amixer mute setting", CLI_GUEST},
	
    {"volume",   "vol",    NULL, _arAudAprocVolumeCmdTbl,        "audio processor volume command", CLI_SUPERVISOR },
 	{"sound effect",   "se",    NULL, _arAudAprocSndEfftCmdTbl,        "audio processor sound effect command", CLI_SUPERVISOR },
#ifdef ARM_SUPPORT_SRS_SS
    {"SRS SS3D",            "srs",     NULL, _arAudAprocSRSTbl,   "SRS setting", CLI_GUEST},  
#endif
#ifdef CC_AUD_SUPPORT_DBXTV 
	{"dbxtv",		"dbx",	 NULL,	_arDbxTVCmdTbl, "DBXTV setting command", CLI_GUEST},
#endif 
	
#ifdef CC_AUD_ARM_RENDER
    {"Test DrvCust AQ",     "aq",     _AprocTestFlashAQ, NULL,   "Test Flash AQ", CLI_GUEST}, //CC_AUD_AQ_IN_DRVCUST
    {"DrvCust AQ Key",      "key",     _AprocTestAQKey, NULL,    "Test Flash AQ Key", CLI_GUEST}, //CC_AUD_AQ_IN_DRVCUST    
#endif 
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif   //CC_AUD_ARM_SUPPORT



static CLI_EXEC_T _arAudDolbyCmdTbl[] =
{
    {"query",       "q",    _AdspDolbyQuery, NULL,        "query Dolby related information", CLI_GUEST},
    {"impulse",       "impulse",    _AdspDolbyImpulse, NULL,        "generate impluse signal", CLI_SUPERVISOR},
    {"tone",       "t",    _AudCmdPlayTone, NULL,        "generate tone", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_VOCAL_MUTE
static CLI_EXEC_T _arAudVocalMuteCmdTbl[] =
{
    {"vocal_mute_enable",   "en",   _AudCmd_VocalMuteEnable,    NULL, "Vocal Mute Enable/Disable", CLI_GUEST},
    {"vocal_mute_filter1",  "vmf",  _AudCmd_VocalMuteFilter1,   NULL, "Vocal Mute (L)&(R) Path Filter Select", CLI_GUEST},
    {"vocal_mute_filter2",  "vmf2", _AudCmd_VocalMuteFilter2,   NULL, "Vocal Mute (L-R) Path Filter Select", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif

#if 0 //def __MODEL_slt__
static CLI_EXEC_T _arAudDSPSLTCmdTbl[] =
{
    {"init",        "i",    _ADSPSLTInit,   NULL,       "adsp SLT init. once",  CLI_SUPERVISOR},
    {"test",        "t",    _ADSPSLTTest,   NULL,       "adsp SLT testing",     CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudUopEqTbl[] =
{
    {"flag",        "flag",    _AudCmdUopEqFlag , NULL,   "EQ function on/off", CLI_GUEST},
    {"mode",        "m",     _AudCmdUopEqMode, NULL, "EQ mode configuration", CLI_GUEST},
    {"eqTableSet", "set", _AudCmdUopEqTableSet, NULL, "Set EQ Table", CLI_GUEST},
    {"eqTableQry", "q", _AudCmdUopEqTableQry, NULL, "Query EQ Table", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudUopSbassTbl[] =
{
    {"flag",        "flag", _AudCmdUopSbassFlag , NULL,   "sbass function on/off", CLI_GUEST},
    {"cut-off freq", "cf", _AudCmdUopBassTrebleCutOffFreq, NULL,   "Bass Treble cut-off freq", CLI_SUPERVISOR},
    {"bassboost",   "bb",     _AudCmdUopBassBoostGain, NULL, "Bass boost gain configuration", CLI_GUEST},
    {"clearboost",  "cb",     _AudCmdUopClearBoostGain, NULL, "Clear boost gain configuration", CLI_GUEST},
    {"query",       "q", _AudCmdUopSbassTableQry, NULL, "query sbass Table", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#ifdef CC_AUD_FY12_LOUDNESS
static CLI_EXEC_T _arAudUopSbassTbl2[] =
{
    {"flag",        "flag", _AudCmdUopSbassFlag2 , NULL,   "bass/treble attenuation(for loundess) on/off", CLI_GUEST},
    {"bassboost",   "bb",     _AudCmdUopBassBoostGain2, NULL, "Bass attenuation gain configuration", CLI_GUEST},
    {"clearboost",  "cb",     _AudCmdUopClearBoostGain2, NULL, "Clear attenuation gain configuration", CLI_GUEST},
    {"query",       "q", _AudCmdUopSbassTableQry2, NULL, "query bass/treble attenuation", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef CC_AUD_SILENCE_SUPPORT
static CLI_EXEC_T _arAudSilenceSetTbl[] =
{
    {"flag",        "flag", _AudCmdUopSilenceFlag , NULL,   "silence function on/off", CLI_GUEST},
    {"threshold",   "thre",     _AudCmdUopSilenceThreshold, NULL, "Silence Threshold configuration", CLI_GUEST},
    {"attack wait time", "wait", _AudCmdUopSilenceWait , NULL,   "Silence attack wait time  configuration", CLI_GUEST},
    {"attack fix gain", "gain", _AudCmdUopSilenceFixGain , NULL,   "Silence attack fix gain  configuration", CLI_GUEST},
    {"release step",  "rstep",     _AudCmdUopSilenceReleaseStep, NULL, "Silence Release Step configuration", CLI_GUEST},
    {"attack step",  "astep",     _AudCmdUopSilenceAttackStep, NULL, "Silence Attack Step configuration", CLI_GUEST},
    {"query",       "q", _AudCmdUopSilenceSetQry, NULL, "query Silence configuration", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudUopLimiterTbl[] =
{
#ifdef CC_AUD_DRC_V20
    //{"mode",        "m", _AudCmdUopLimiterMode , NULL,   "limiter mode", CLI_GUEST},
    {"flag",   "flag",     _AudCmdUopLimiterFlag, NULL, "individual limiter flag", CLI_GUEST},
    {"threshold",  "thre",     _AudCmdUopLimiterThre, NULL, "limiter fixed mode threshold", CLI_GUEST},
    //{"attackrate",  "attack",     _AudCmdUopLimiterAttackRate, NULL, "limiter attack rate", CLI_GUEST},
    {"releaserate",  "release",     _AudCmdUopLimiterRelaseRate, NULL, "limiter release rate", CLI_GUEST},
    {"ratio",  "ratio",     _AudCmdUopLimiterGainRatio, NULL, "limiter gain ratio", CLI_GUEST},
    {"query",       "q", _AudCmdUopLimiterQuery, NULL, "query limiter configuration", CLI_GUEST},
#else
    {"mode",        "m", _AudCmdUopLimiterMode , NULL,   "limiter mode", CLI_GUEST},
    {"flag",   "flag",     _AudCmdUopLimiterFlag, NULL, "individual limiter flag", CLI_GUEST},
    {"threshold",  "thre",     _AudCmdUopLimiterThre, NULL, "limiter fixed mode threshold", CLI_GUEST},
#ifdef CC_RATIO_LIMITER
    {"ratio",  "ratio",     _AudCmdUopLimiterGainRatio, NULL, "limiter gain ratio", CLI_GUEST},
#endif
    {"query",       "q", _AudCmdUopLimiterQuery, NULL, "query limiter configuration", CLI_GUEST},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudUopPL2Tbl[] =
{
    {"init",        "i",    _AudCmdUopPL2Init , NULL,   "initialization for prologic2", CLI_GUEST},
    {"switch",      "sw",   _AudCmdUopPL2Switch , NULL, "prologic2 On/Off/Auto switch", CLI_GUEST},
    {"mode",        "mode", _AudCmdUopPL2Mode , NULL,   "setup prologic2 mode        ", CLI_GUEST},
    {"pano",        "pano", _AudCmdUopPL2Pano , NULL,   "panorama on/off             ", CLI_GUEST},
    {"dim",         "dim",  _AudCmdUopPL2Dim , NULL,    "dimension control           ", CLI_GUEST},
    {"cw",          "cw",   _AudCmdUopPL2CWidth , NULL, "center width control        ", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#ifdef DSP_SUPPORT_SRSWOW
static CLI_EXEC_T _arAudUopSRSWOWTbl[] =
{
    {"OnOff",       "onoff", _AudCmdUopSRSWOW,       NULL,  "SRS WOW On/Off",   CLI_SUPERVISOR},
    {"wspk",        "wspk",  _AudCmdUopSRSWOWSpkSz,  NULL,  "Speaker Size",     CLI_SUPERVISOR},
    {"wbrt",        "wbrt",  _AudCmdUopSRSWOWBright, NULL,  "Brightness",       CLI_SUPERVISOR},
    {"wtb",         "wtb",   _AudCmdUopSRSWOWTBLvl,  NULL,  "TruBass Level",    CLI_SUPERVISOR},
    {"wwid",        "wwid",  _AudCmdUopSRSWOWWidth,  NULL,  "Width",            CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif
#ifdef DSP_SUPPORT_SRSTSXT
static CLI_EXEC_T _arAudUopSRSTSXTTbl[] =
{
    {"tspk",        "tspk",  _AudCmdUopSRSTSXTSpkSz,  NULL,  "Speaker Size",     CLI_SUPERVISOR},
    {"tsmode",      "tsmd",  _AudCmdUopSRSTSXTMode,  NULL,  "Mode",     CLI_SUPERVISOR},
    {"telv",        "telv",  _AudCmdUopSRSTSXTElev,   NULL,  "Elevation",        CLI_SUPERVISOR},
    {"ing",        "ing",  _AudCmdUopSRSTSXTinputgain,   NULL,  "input gain",        CLI_SUPERVISOR},
    {"ttb",         "ttb",   _AudCmdUopSRSTSXTTBLvl,  NULL,  "TruBass Level",    CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#if (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD))//sunman_tshd
static CLI_EXEC_T _arAudUopSRSTSHDTbl[] =
{
    {"tinit", "tinit",      _AudCmdUopSRSTSHDSetInit,   NULL,   "Initial settings ",            CLI_SUPERVISOR},
    {"tinpg",   "tinpg",    _AudCmdUopSRSTSHDInpGain,   NULL,   "Input gain",           CLI_SUPERVISOR},
       {"toutg",    "toutg",    _AudCmdUopSRSTSHDOutGain,   NULL,   "Output gain",             CLI_SUPERVISOR},
        {"tsrlv",   "tsrlv",    _AudCmdUopSRSTSHDSurrLvl,   NULL,   "Surround Level ",          CLI_SUPERVISOR},
    {"tspk",    "tspk", _AudCmdUopSRSTSHDSpkSz, NULL,   "Speaker Size",     CLI_SUPERVISOR},
    {"ttb", "ttb",  _AudCmdUopSRSTSHDTBLvl, NULL,   "TruBass Level",            CLI_SUPERVISOR},
    {"tdef",    "tdef", _AudCmdUopSRSTSHDDef,       NULL,   "Definition Control",   CLI_SUPERVISOR},
    {"telv",    "telv", _AudCmdUopSRSTSHDElev,      NULL,   "FOCUS Elevation",       CLI_SUPERVISOR},
    {"fcc", "fcc",  _AudCmdUopSRSTSHDFntCCtrl,      NULL,   "TS/WOW Front Center Countrol",       CLI_SUPERVISOR},
    {"fsc", "fsc",  _AudCmdUopSRSTSHDFntSCtrl,      NULL,   "TS/WOW Front Space Countrol",       CLI_SUPERVISOR},
       {"tmode","tmode",    _AudCmdUopSRSTSHDMode,  NULL,   "TSHD Mode",            CLI_SUPERVISOR},
    //{"tflag", "tflag",    _AudCmdUopSRSTSHDSetFlag,   NULL,   "TSHD Flag",            CLI_SUPERVISOR},
    //{"tmode","tmode", _AudCmdUopSRSTSHDSetLtRt_PL2M,  NULL,   "LtRt or PL2Music", CLI_SUPERVISOR},
    //{"tsspk", "tsspk",    _AudCmdUopSRSTSHDSubSpkSz,NULL, "Sub Speaker Size", CLI_SUPERVISOR},
    //{"tcofq", "tcofq",    _AudCmdUopSRSTSHDCOFREQ,    NULL,   "CrossOver Freq",       CLI_SUPERVISOR},
    //{"tts",   "tts",  _AudCmdUopSRSTSHDSetTSCtrl,     NULL,   "TruSurround Control",  CLI_SUPERVISOR},
    //{"tlim",      "tlim", _AudCmdUopSRSTSHDLimiter,   NULL,   "Limiter Control",          CLI_SUPERVISOR},
    //{"timod", "timod",    _AudCmdUopSRSTSHDInpuMode,  NULL,   "Set Input Mode ",          CLI_SUPERVISOR},
    //{"tomod", "tomod",    _AudCmdUopSRSTSHDOutputMode,    NULL,   "Set Output Mode ",         CLI_SUPERVISOR},
    //{"tbypass", "tbypass",        _AudCmdUopSRSTSHDSetBypass, NULL,   "Set Bypass Mode ",         CLI_SUPERVISOR},
    //{"tout", "tout",   _AudCmdUopSRSTSHDSetSubToL,    NULL,   "Set Sub, Center to L, R ",         CLI_SUPERVISOR},
    {"tana",    "tana", _AudCmdUopSRSTSHDAnaSz, NULL,   "Analysis Size",        CLI_SUPERVISOR},
    {"thpf",    "thpf", _AudCmdUopSRSTSHDHpfSz, NULL,   "Hpf Size",     CLI_SUPERVISOR},
    {"tcompr",  "tcompr",   _AudCmdUopSRSTSHDCOMPRLvl,  NULL,   "Compressor Level",         CLI_SUPERVISOR},
    //{"tlevdep",   "tlevdep",  _AudCmdUopSRSTSHDLEVINDEP,  NULL,   "Level indepent",           CLI_SUPERVISOR},
    {"tbflag",  "tbflag",   _AudCmdUopSRSTSHDTBFLAG,    NULL,   "Trubass flag",         CLI_SUPERVISOR},
    {"setcut",  "setcut",   _AudCmdUopSRSTSHDCustom,    NULL,   "Set custom coefficient",   CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#if (defined(DSP_SUPPORT_SRSCC3D))
static CLI_EXEC_T _arAudUopSRSCC3DTbl[] =
{
    {"cinit", "cinit",      _AudCmdUopSRSCC3DSetInit,   NULL,   "Initial settings,For test only ",    CLI_SUPERVISOR},
    {"cflag",   "cflag",    _AudCmdUopSRSCC3DFlag,  NULL,   "CC3D FLAG",            CLI_SUPERVISOR},
    {"cmode",   "cmode",    _AudCmdUopSRSCC3DMode,  NULL,   "CC3D MODE",               CLI_SUPERVISOR},
    {"cinpg",   "cinpg",    _AudCmdUopSRSCC3DInpGain,   NULL,   "Input gain",           CLI_SUPERVISOR},
    {"coutg",   "coutg",    _AudCmdUopSRSCC3DOutGain,   NULL,   "Output gain",             CLI_SUPERVISOR},
    {"cinmod",  "cinmod",   _AudCmdUopSRSCC3DInMode,    NULL,   "CC3D inmode",          CLI_SUPERVISOR},
    {"c3dmod",  "c3dmod",   _AudCmdUopSRSCC3D3dMode,    NULL,   "CC3D 3dmode",             CLI_SUPERVISOR},
    {"chpf",    "chpf", _AudCmdUopSRSCC3DHpfSz, NULL,   "Space HPF Size",       CLI_SUPERVISOR},
    {"cini",    "cini", _AudCmdUopSRSCC3DIniSz, NULL,   "INI Filter Size",      CLI_SUPERVISOR},
    {"cspk",    "cspk", _AudCmdUopSRSCC3DSpkSz, NULL,   "TruBass Speaker Size",     CLI_SUPERVISOR},
    {"ctb", "ctb",  _AudCmdUopSRSCC3DTBLvl, NULL,   "TruBass Level",            CLI_SUPERVISOR},
    {"cana",    "cana", _AudCmdUopSRSCC3DAnaSz, NULL,   "TruBass Analysis Size",        CLI_SUPERVISOR},
    {"ccompr",  "ccompr",   _AudCmdUopSRSCC3DCOMPRLvl,  NULL,   "TruBass Compressor Level",         CLI_SUPERVISOR},
    {"ctbflag", "ctbflag",  _AudCmdUopSRSCC3DTBFLAG,    NULL,   "TruBass flag",         CLI_SUPERVISOR},
    {"csetcut", "csetcut",  _AudCmdUopSRSCC3DCustom,    NULL,   "TruBass Set custom coefficient",   CLI_SUPERVISOR},
    {"cmtec",   "cmtec",    _AudCmdUopSRSCC3DSetMainTech,   NULL,   "Set main tech",   CLI_SUPERVISOR},
    {"ccenctr", "ccenctr",  _AudCmdUopSRSSRS3DCenterCtrl,   NULL,   "SRS3D Center Ctrl",   CLI_SUPERVISOR},
    {"cspactr", "cspactr",  _AudCmdUopSRSSRS3DSpaceCtrl,    NULL,   "SRS3D Space Ctrl",   CLI_SUPERVISOR},
    {"csptun",  "csptun",   _AudCmdUopSRSSpaceCtrlTune, NULL,   "Space Ctrl Tune",   CLI_SUPERVISOR},
    {"cmfade",  "cmfade",   _AudCmdUopSRSMixFadeCtrl,   NULL,   "mix fade ctrl",   CLI_SUPERVISOR},
    {"cfcen",   "cfcen",    _AudCmdUopSRSFrontCenterCtrl,   NULL,   "front center ctrl",   CLI_SUPERVISOR},
    {"cfspa",   "cfspa",    _AudCmdUopSRSFrontSpaceCtrl,    NULL,   "front space ctrl",   CLI_SUPERVISOR},
    {"crspa",   "crspa",    _AudCmdUopSRSRearSpaceCtrl, NULL,   "rear space ctrl",   CLI_SUPERVISOR},
    //{"crcen", "crcen",    _AudCmdUopSRSRearCenterCtrl,    NULL,   "rear center ctrl",   CLI_SUPERVISOR},
    {"ctsl",    "ctsl",     _AudCmdSRSUopTruSurLev, NULL,   "true surround level",   CLI_SUPERVISOR},
    {"cmfce",   "cmfce",    _AudCmdSRSUopMixFadeCtrlExt,    NULL,   "mix fade ctrl ext",   CLI_SUPERVISOR},
    {"cflev",   "cflev",    _AudCmdSRSUopFocusLev,  NULL,   "focus level gain",   CLI_SUPERVISOR},
    {"clfoc",   "clfoc",    _AudCmdSRSUopLFocusLev, NULL,   "left focus level gain",   CLI_SUPERVISOR},
    {"crfoc",   "crfoc",    _AudCmdSRSUopRFocusLev, NULL,   "right focus level gain",   CLI_SUPERVISOR},
    {"cldef",   "cldef",    _AudCmdSRSUopLDefLev,   NULL,   "left def level gain",   CLI_SUPERVISOR},
    {"crdef",   "crdef",    _AudCmdSRSUopRDefLev,   NULL,   "right def level gain",   CLI_SUPERVISOR},
    {"cidep",   "cidep",    _AudCmdUopSRSCC3DIniDepth,  NULL,   "set delaylineinitialdepth",        CLI_SUPERVISOR},  
    {"cref",    "cref",     _AudCmdSRSUopRefLev,    NULL,   "reference def level ",   CLI_SUPERVISOR},
    {"ctbq",    "ctbq",     _AudCmdSRSUopTbqLev,    NULL,   "tbq level ",   CLI_SUPERVISOR},
    {"cdcu",    "cdcu",     _AudCmdSRSUopDcuLev,    NULL,   "dcu  level ",   CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif


#ifdef DSP_SUPPORT_SRSVIQ
//sunman_viq
static CLI_EXEC_T _arAudUopSRSVIQTbl[] =
{
    {"ref",        "ref",  _AudCmdUopSRSVIQRefLvl,  NULL,  "Reference level",     CLI_GUEST},
    {"mode",    "mode",  _AudCmdUopSRSVIQMode,  NULL,  "Mode",     CLI_GUEST},
    {"maxg",    "maxg",  _AudCmdUopSRSVIQMaxGain,  NULL,  "Max gain",     CLI_GUEST},
    {"inpg",    "inpg",  _AudCmdUopSRSVIQInputGain,  NULL,  "Input gain",     CLI_GUEST},
    {"outpg",    "outpg",  _AudCmdUopSRSVIQOutputGain,  NULL,  "Output gain",     CLI_GUEST},
    {"noeq",    "noeq",   _AudCmdUopSRSVIQDisableEQ,  NULL,  "Close EQ",     CLI_GUEST},
    {"tnoise",   "tnoise",  _AudCmdUopSRSVIQNoiseTh, NULL,  "Noise TH",       CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#if defined(DSP_SUPPORT_SRS_PEQ)
//SRS PEQ
static CLI_EXEC_T _arAudUopSRSPeqTbl[] =
{
    {"on",     "on",  _AudCmdUopSRSPeqOn,  NULL,  "on/off", CLI_GUEST},
    {"inpg",   "inpg",  _AudCmdUopSRSPeqInputGain,  NULL,  "input gain",     CLI_GUEST},
    {"outpg", "outpg",  _AudCmdUopSRSPeqOutputGain,  NULL,  "output gain",     CLI_GUEST},
    {"sc",     "sc",  _AudCmdUopSRSPeqSetCoef,  NULL,  "peq Set coef",     CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif
#if defined(DSP_SUPPORT_SRS_HPF)
//SRS HPF
static CLI_EXEC_T _arAudUopSRSHpfTbl[] =
{
    {"on",     "on",  _AudCmdUopSRSHpfOn,  NULL,  "srs hpf on/off",     CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif


#ifdef DSP_SUPPORT_SRS_PURESOUND
//SRS puresound
static CLI_EXEC_T _arAudUopSRSPureTrubassTbl[] =
{
    {"on",     "on",  _AudCmdUopSRSPuresoundTrubassOn,  NULL,  "on/off", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif

#ifdef DSP_SUPPORT_SRS_AEQ
//SRS aeq
static CLI_EXEC_T _arAudUopSRSAeqTbl[] =
{
    {"on",     "on",  _AudCmdUopSRSAeqOn,  NULL,  "on/off", CLI_GUEST},
    {"inpg",   "inpg",  _AudCmdUopSRSAeqInputGain,  NULL,  "input gain",     CLI_GUEST},
    {"outpg", "outpg",  _AudCmdUopSRSAeqOutputGain,  NULL,  "output gain",     CLI_GUEST},
    //{"aeqsize",     "aeqsize",  _AudCmdUopSRSAeqFilterSize,  NULL,  "aeq filter size",     CLI_GUEST},
    {"firset",     "firset",  _AudCmdUopSRSAeqFirFilterSet,  NULL,  "aeq fir filter set coeff",     CLI_GUEST},
    {"iirset",     "iirset",  _AudCmdUopSRSAeqIirFilterSet,  NULL,  "aeq iir filter set coeff",     CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif
#ifdef DSP_SUPPORT_SRS_LMT
//SRS lmt
static CLI_EXEC_T _arAudUopSRSLmtTbl[] =
{
    {"on",     "on",  _AudCmdUopSRSLmtOn,  NULL,  "on/off",     CLI_GUEST},
    {"inpg",   "inpg",  _AudCmdUopSRSLmtInputGain,  NULL,  "input gain",     CLI_GUEST},
    {"outpg", "outpg",  _AudCmdUopSRSLmtOutputGain,  NULL,  "output gain",     CLI_GUEST},
    {"bst",   "bst",  _AudCmdUopSRSLmtBoost,  NULL,  "limiter boost",     CLI_GUEST},
    {"lvl", "lvl",  _AudCmdUopSRSLmtLvl,  NULL,  "limiter level",     CLI_GUEST},
    {"dly", "dly",  _AudCmdUopSRSLmtDly,  NULL,  "delay count",     CLI_GUEST},
    {"coeff", "coeff",  _AudCmdUopSRSLmtCoeff,  NULL,  "coeff level",     CLI_GUEST},
    {"thresh", "thresh",  _AudCmdUopSRSLmtThresh,  NULL,  "thresh level",     CLI_GUEST},

    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif



#ifdef DSP_SUPPORT_TCL_VBASS
static CLI_EXEC_T _arAudUopTCLVbass[] =
{    
    {"tvb",     "tvb",  _AudCmdUopTVBEnable,  NULL,  "tvb",     CLI_GUEST},
    {"tinit", "tinit",  _AudCmdUopTVBSetInit,   NULL,   "Initial settings,For test only ",    CLI_SUPERVISOR},  
    {"a0p",     "a0p",  _AudCmdUopTVBA0P,  NULL,  "a0peak",     CLI_GUEST},
    {"a1p",     "a1p",  _AudCmdUopTVBA1P,  NULL,  "a1peak",     CLI_GUEST},
    {"a2p",     "a2p",  _AudCmdUopTVBA2P,  NULL,  "a2peak",     CLI_GUEST},
    {"b0p",     "b0p",  _AudCmdUopTVBB0P,  NULL,  "b0peak",     CLI_GUEST},
    {"b1p",     "b1p",  _AudCmdUopTVBB1P,  NULL,  "b1peak",     CLI_GUEST},
    {"b2p",     "b2p",  _AudCmdUopTVBB2P,  NULL,  "b2peak",     CLI_GUEST},
    {"at",     "at",  _AudCmdUopTVBAT,  NULL,  "AT",     CLI_GUEST},
    {"rt",     "rt",  _AudCmdUopTVBRT,  NULL,  "RT",     CLI_GUEST},
    {"ap",     "ap",  _AudCmdUopTVBAP,  NULL,  "apeak",     CLI_GUEST},
    {"gham",     "gham",  _AudCmdUopTVBGAIN_HAMONICGEN,  NULL,  "GAIN_HAMONICGEN",     CLI_GUEST},
    {"gout",     "gout",  _AudCmdUopTVBG_OUT,  NULL,  "G_OUT",     CLI_GUEST},
    {"gback",     "gback",  _AudCmdUopTVBGAIN_BACK,  NULL,  "GAIN_BACK",     CLI_GUEST},
    {"gaout",     "gaout",  _AudCmdUopTVBGAIN_OUT,  NULL,  "GAIN_OUT",     CLI_GUEST},
    {"ghcout",     "ghcout",  _AudCmdUopTVBGAINHAMONICOUT,  NULL,  "GAINHAMONICOUT",     CLI_GUEST},
    {"cham",     "cham",  _AudCmdUopTVBCOEFF_HAM,  NULL,  "COEFF_HAM",     CLI_GUEST},
    {"x1p",     "x1p",  _AudCmdUopTVBX1PEAK,  NULL,  "X1PEAK",     CLI_GUEST},
    {"x2p",     "x2p",  _AudCmdUopTVBX2PEAK,  NULL,  "X2PEAK",     CLI_GUEST},
    {"c1p",     "c1p",  _AudCmdUopTVBC1PEAK,  NULL,  "C1PEAK",     CLI_GUEST},
    {"c2p",     "c2p",  _AudCmdUopTVBC2PEAK,  NULL,  "C2PEAK",     CLI_GUEST},
    {"ctp",     "ctp",  _AudCmdUopTVBCOUT_PEAK,  NULL,  "COUT_PEAK",     CLI_GUEST},
    {"yt",     "yt",  _AudCmdUopTVBY_OUT,  NULL,  "y_out",     CLI_GUEST},
    {"coef",     "coef",  _AudCmdUopTVBSetCoeff,  NULL,  "coef",     CLI_GUEST},
    
    
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif


#ifdef CC_AUD_BBE_SUPPORT
static CLI_EXEC_T _arAudUopBBETbl[] =
{
    {"query",    "q", _AudCmdUopBBEQuery,  NULL, "Query BBE Config", CLI_GUEST},
    {"setmode", "mode",   _AudCmdUopBBEMode, NULL, "Set BBE Mode", CLI_GUEST},
    {"setlevel", "level", _AudCmdUopBBELevel,  NULL, "Set BBE Level", CLI_GUEST},
    {"sethigh", "high",  _AudCmdUopBBEHighGain, NULL, "Set BBE High Gain", CLI_GUEST},
    {"setlow", "low", _AudCmdUopBBELowGain,  NULL, "Query BBE Low Gain", CLI_GUEST},
    {"set3d" , "3d", _AudCmdUopBBE3DGain, NULL, "Query BBE Config", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudUopSpkCfgTbl[] =
{
    {"spkcfg",     "spkcfg",    _AudCmdUopSpeakerConfig , NULL, "speaker config steup", CLI_SUPERVISOR},
    {"delaycfg",   "delaycfg",  _AudCmdUopChannelDelay , NULL,  "channel delay steup", CLI_GUEST},
    {"largesmall", "ls",        _AudCmdUopSpeakerLargeSmallConfig , NULL,   "speaker large/small config", CLI_GUEST},
    {"cutoff_freq","cf",        _AudCmdUopCutFreq , NULL,   "set cutoff frequency", CLI_GUEST},
#ifdef CC_S_SUBWOFFER_SUPPORT  ///CC_BASS_MANGER_INDEP_LPF
    {"cutoff_freq_lpf","cflpf", _AudCmdUopCutFreqLpf , NULL,   "set cutoff frequency of lpf for LFE channel", CLI_GUEST},
#endif
    {"channel",    "ch",        _AudCmdUopSpeakerOutputConfig , NULL,   "speaker config", CLI_GUEST},
    {"subwoofer",  "sw",        _AudCmdUopSpeakerSubwooferEnable , NULL,    "subwoofer on/off", CLI_GUEST},
    {"bass",       "bs",        _AudCmdUopBassMngEnable , NULL, "bass management on/off", CLI_GUEST},
    {"autodm",     "dm",        _AudCmdUopAutoDmx , NULL,   "auto downmix on/off", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
static CLI_EXEC_T _arAudUopADVTbl[] =
{
    {"ADV mode",     "mode",    _AudCmdUopADV , NULL, "ADV mode steup", CLI_GUEST},
    {"ADV query",   "q",  _AudCmdUopADV_Query , NULL,  "ADV query", CLI_GUEST},
    {"ADV set chGain",   "set",  _AudCmdUopADVSetChGain , NULL,  "ADV channel Gain setting", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
static CLI_EXEC_T _arAudUopCDNotchTbl[] =
{
    {"CDNotch Flag", "flag", _AudCmdUopCDNotchFlag, NULL, "CDNotch enable", CLI_GUEST},
        {"CDNotch Config", "cfg", _AudCmdUopCDNotchConfig, NULL, "CDNotch Q and Fc setup", CLI_GUEST},
        {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef CC_AUD_FIR_SUPPORT
static CLI_EXEC_T _arAudUopConeqTbl[] =
{
    {"CONEQ mode",  "flag",     _AudCmdUopFIREnable, NULL,  "Set CONEQ flag", CLI_GUEST},
    {"CONEQ dump address", "q",  _AudCmdUopGetPhyAddr,       NULL, "Get CONEQ table address", CLI_GUEST},

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif


#ifdef KARAOKE_SUPPORT
static CLI_EXEC_T _arAudUopKeyShiftTbl[] =
{
    {"Keyshift Flag", "flag", _AudCmdUopKeyShiftFlag, NULL, "Keyshift enable", CLI_GUEST},
    {"Keyshift key", "key", _AudCmdUopKeyShiftKey, NULL, "Keyshift key setup", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif

static CLI_EXEC_T _arAudUopCmdTbl[] =
{
    {"DSP volume system", "qv", _AudCmdUopQueryDSPVoumeSystem, NULL, "DSP volume system setting", CLI_SUPERVISOR},
    {"fine volume", "fv",    _AudCmdUopFineVolume, NULL, "Audio Master Volume fine control", CLI_SUPERVISOR},
    {"volume",      "v",     _AudCmdUopVolume, NULL, "Audio Volume control", CLI_GUEST},
    {"vol_curve",   "vc",    _AudCmdUopVolumeCurve, NULL, "Audio Volume Curve Set", CLI_GUEST},
    {"vol_curve_query","vcq", _AudCmdUopQueryVolumeCurve, NULL, "Audio Volume Curve Query", CLI_GUEST},
#ifdef CC_AUD_NEW_HP_VOL_TABLE
    {"set volume table", "shvt",    _AudCmdUopSetHpVolTable, NULL, "Audio Hp Volume table set ", CLI_GUEST},
    {"vol_curve_hp_query","hpvcq", _AudCmdUopQueryHpVolumeCurve, NULL, "Audio Hp Volume Curve Query", CLI_GUEST},
#endif
#ifdef CC_SET_VOLUME
    {"set volume table", "svt",    _AudCmdUopSetVolTable, NULL, "Audio Volume table set ", CLI_GUEST},
#endif
    {"fine ch volume", "fcv",    _AudCmdUopFineChannelVolume, NULL, "Audio Channel Volume fine control", CLI_SUPERVISOR},
#ifdef CC_AUD_NEW_CV_TABLE
    {"set CHVOl table", "scvt", _AudCmdUopSetChVolTable, NULL, "Audio Channle Volume table set ", CLI_GUEST},
    {"ch_vol_curve",   "cvc",    _AudCmdUopChannelVolumeCurve, NULL, "Audio Channel Volume Curve Set", CLI_GUEST},
    {"ch_vol_curve_query","cvcq", _AudCmdUopQueryChannelVolumeCurve, NULL, "Channel Audio Volume Curve Query", CLI_GUEST},
#endif
    {"ch volume",   "cv",     _AudCmdUopChVolume, NULL, "Audio Channel Volume control", CLI_GUEST},
    {"ch vol gain", "cvg",    _AudCmdUopChVolumeGain, NULL, "Audio Channel Volume Gain control", CLI_GUEST},
#ifdef CC_AUD_VOLUME_OFFSET
    {"volume offset", "vo",   _AudCmdUopVolumeOffset, NULL, "Audio Channel Volume offset", CLI_GUEST},
#ifdef CC_S_SUBWOFFER_SUPPORT
    {"sub woofer volume offset", "swvo",  _AudCmdUopSWVolumeOffset, NULL, "Audio Sub Woofer Channel Volume offset", CLI_GUEST},
#endif
#endif
#ifdef CC_AUD_LINEOUT_VOL_OFFSET
    {"linout vol offset",   "lovo",   _AudCmdUopLineOutVolOffset, NULL, "Audio Line out Volume offset", CLI_GUEST},
#endif
#ifdef CC_AUD_LINEOUT_VOL_MODE
    {"linout vol", "lov",     _AudCmdUopLinOutVolume, NULL, "Audio Line out Volume control", CLI_GUEST},
#endif
#ifdef CC_AUD_INPUT_SRC_LINEOUT_VOL
    {"src linout vol", "slov",    _AudCmdUopSrcLinOutVolume, NULL, "Audio Source Line out Volume set", CLI_GUEST},
#endif
    {"src volume",  "sv",  _AudCmdUopSrcVolume, NULL, "Audio Source Volume control", CLI_GUEST},
    {"src volume query", "svq", _AudCmdUopGetSrcVolume, NULL, "Query Audio Source Volume", CLI_GUEST},
    {"equalizer",   "eq",     NULL, _arAudUopEqTbl, "EQ configuration", CLI_GUEST},
    {"sbass",   "sbass",     NULL, _arAudUopSbassTbl, "Bass/treble configuration", CLI_GUEST},
#ifdef CC_AUD_FY12_LOUDNESS
    {"sbass2",   "sbass2",     NULL, _arAudUopSbassTbl2, "Bass/treble attenuation for FY12 Loudness", CLI_GUEST},
#endif
#ifdef CC_AUD_SILENCE_SUPPORT
    {"Silence set",   "silence",   NULL,  _arAudSilenceSetTbl, "Slience fuction set", CLI_GUEST},//for silence fuction
#endif
    {"limiter",   "limiter",     NULL, _arAudUopLimiterTbl, "Limiter configuration", CLI_GUEST},
    {"pl2cfg",      "pl2cfg", NULL, _arAudUopPL2Tbl,"Prologic 2 related configuration", CLI_SUPERVISOR},
    {"spkuop",      "spkuop", NULL, _arAudUopSpkCfgTbl,"Speaker related configuration", CLI_GUEST},
    {"AVC",      "a",     _AudCmdUopAVC, NULL, "Automatic Volume Control", CLI_GUEST},
#ifdef CC_AUD_DRC_V20
    {"drc",      "drc",     _AudCmdUopDRC, NULL, "DRC", CLI_GUEST},
#endif
    {"MonoDownmix", "md",     _AudCmdUopMonoDownmix, NULL, "Downmix to mono", CLI_SUPERVISOR},
    {"Karaoke mix ratio", "kr",     _AudCmdUopKaraokeMixRatio, NULL, "Karaoke mix ratio", CLI_SUPERVISOR},
#ifdef FRAC_SPEED_SUPPORT
    {"playSpeed", "fspeed",  _AudCmdUopPlaySpeed, NULL, "Set fractional play speed", CLI_GUEST},  //sunman for play speed
#endif
#if defined(__linux__)
    {"balance",      "bl",     _AudCmdUopBalance, NULL, "Set L/R balance", CLI_SUPERVISOR},
#endif
#ifdef DSP_SUPPORT_SRSWOW
    {"srswow",      "wow", NULL, _arAudUopSRSWOWTbl, "Set SRS WOW", CLI_GUEST},
#endif
#ifdef DSP_SUPPORT_SRSTSXT
    {"srstsxt",     "tsxt", NULL, _arAudUopSRSTSXTTbl, "Set SRS TSXT", CLI_GUEST},
#endif
    {"virtualsurround",   "vs",     _AudCmdUopVirtualSurround, NULL, "Virtual Surround flag", CLI_GUEST},
    {"vsurround config",   "vscfg", 	_AudCmdUopVSurrCfg, NULL, "Virtual Surround Config", CLI_GUEST},
#if (defined(DSP_SUPPORT_SRSTSHD) || defined(DSP_SUPPORT_SRSWOWHD))
    {"srstshd",     "tshd", NULL, _arAudUopSRSTSHDTbl, "Set SRS TSHD", CLI_GUEST},  //sunman_tshd
#endif
#if (defined(DSP_SUPPORT_SRSCC3D))
    {"srscc3d",     "cc3d", NULL, _arAudUopSRSCC3DTbl, "Set SRS CC3D", CLI_GUEST},
#endif
#ifdef DSP_SUPPORT_SRSVIQ
    {"VIQ",      "viq", NULL,  _arAudUopSRSVIQTbl, "SRS Volume IQ", CLI_GUEST},     //sunman_viq
#endif
#if defined(DSP_SUPPORT_SRS_PEQ)
    {"peq",    "peq", NULL,  _arAudUopSRSPeqTbl, "SRS PEQ", CLI_GUEST},     //
#endif
#if defined(DSP_SUPPORT_SRS_HPF)
    {"srshpf",    "srshpf", NULL,  _arAudUopSRSHpfTbl, "SRS HPF", CLI_GUEST},   //
#endif
#ifdef DSP_SUPPORT_SRS_PURESOUND
    {"purethd",    "purethd", NULL,  _arAudUopSRSPureTrubassTbl, "SRS PURE THD", CLI_GUEST},    //bofeng_pure
#endif
#ifdef DSP_SUPPORT_SRS_AEQ
    {"aeq",    "aeq", NULL,  _arAudUopSRSAeqTbl, "SRS AEQ", CLI_GUEST},     //bofeng_aeq
#endif
#ifdef DSP_SUPPORT_SRS_LMT
    {"lmt",    "lmt", NULL,  _arAudUopSRSLmtTbl, "SRS Limiter", CLI_GUEST},  //bofeng_lmt
#endif

#if defined(DSP_SUPPORT_SRS_SSHD) || defined(DSP_SUPPORT_SRS_PURESOUND) || defined(DSP_SUPPORT_SRS_SS3D)
    {"srs head room",   "hroom",     _AudCmdUopSRSHeadRoom, NULL, "set srs head room", CLI_GUEST},
    {"srs compensate",   "cpsate",     _AudCmdUopSRSCompensate, NULL, "set srs compensate", CLI_GUEST},
#endif

#ifdef DSP_SUPPORT_TCL_VBASS
    {"tcl",    "tcl", NULL,  _arAudUopTCLVbass, "TCL VBASS", CLI_GUEST},  //bofeng_TCL
#endif

#ifdef CC_AUD_SOUND_MODE
    {"post sound mode",   "psm",     _AudCmdUopPostSoundMode, NULL, "Sound mode change", CLI_GUEST},
#endif
    //{"surround mode",   "vsm",     _AudCmdUopVSMode, NULL, "surround mode change", CLI_GUEST},
#ifdef CC_AUD_BBE_SUPPORT
    {"bbe",         "bbe", NULL, _arAudUopBBETbl, "Set BBE Function", CLI_GUEST},
#endif
#ifdef CC_AUD_DSP_SUPPORT_CDNOTCH
    {"cfg-depth notch", "cdnotch", NULL, _arAudUopCDNotchTbl, "Set CDNotch Parameters", CLI_GUEST},
#endif
#ifdef KARAOKE_SUPPORT
    {"keyshift", "keyshift", NULL, _arAudUopKeyShiftTbl, "Set Keyshift Parameters", CLI_GUEST},
#endif
#ifdef CC_AUD_HPF_SUPPORT
    {"hpf",         "hpf",      _AudCmdUopHpfEnable, NULL,  "Set HPF enable",    CLI_GUEST},
    {"hpf_fc",      "fc",       _AudCmdUopHpfFc, NULL,      "Set HPF Fc",        CLI_GUEST},
  //{"hpf2_fc",      "fc2",       _AudCmdUopHpf2Fc, NULL,      "Set HPF2 Fc",        CLI_GUEST},
#endif
#ifdef CC_AUD_CLIPPER_SUPPORT
    {"clipper",     "clip",     _AudCmdUopClipper, NULL,    "Set clipper enable",CLI_GUEST},
    {"clip_val",    "clipval",  _AudCmdUopClipperVal, NULL, "Set clipper value", CLI_GUEST},
#endif
#if 1//def CC_AUD_PEQ_SUPPORT
    {"peq",         "peq",      _AudCmdUopPEQ, NULL,        "Set PEQ",           CLI_GUEST},
    {"peqc",        "peqc",     _AudCmdUopPEQCfg, NULL,     "Set PEQ Configuration", CLI_GUEST},
    {"peqc2",       "peqc2",    _AudCmdUopPEQCfg2, NULL,    "Set PEQ Configuration", CLI_GUEST},
#ifdef CC_AUD_PEQ_LR

    {"peqcr",        "peqcr",     _AudCmdUopPEQCfg_R, NULL,     "Set PEQ Configuration", CLI_GUEST},
    {"peqc2r",       "peqc2r",    _AudCmdUopPEQCfg2_R, NULL,    "Set PEQ Configuration", CLI_GUEST},
#endif
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT
    {"sph",         "sph",      _AudCmdUopSPH, NULL,        "Set Speaker Height", CLI_GUEST},
#endif
#ifdef CC_AUD_VBASS_SUPPORT
    {"vb",          "vb",       _AudCmdUopVirBass, NULL,        "Set Virtual Bass", CLI_GUEST},
    {"vbcfg",       "vbcfg",    _AudCmdUopVBCfg, NULL,      "Set Virtual Bass config", CLI_GUEST},
    {"vb cut",      "vbcut",    _AudCmdUopVirBassCut, NULL, "Set Virtual Bass Cutoff", CLI_GUEST},
#endif //#CC_AUD_VBASS_SUPPORT
    {"FF SPEED",    "speed",    _AudCmdDspSpeed , NULL,     "DSP Speed", CLI_SUPERVISOR},
    {"DMixPos",     "dmp",      _AudCmdUopDownmixPosition , NULL, "Downmix channel position", CLI_SUPERVISOR},
    {"MultiPair",   "MultiPair",_AudCmdUopMultiPairOutput , NULL, "Multi-Pair output", CLI_SUPERVISOR},
    {"LR DownMix",  "LRDMix",   _AudCmdUopLRDownMix, NULL, "LR DownMix", CLI_SUPERVISOR},
    {"dump",        "dump",     _AudCmdDspUopDump , NULL, "DSP Dump", CLI_SUPERVISOR},
    {"DDCO",        "DDCO",     _AudCmdUopDDCO , NULL, "DDCO setting", CLI_SUPERVISOR},
    {"DDCO AGC",        "DDCOAgc",     _AudCmdUopDDCOAgc, NULL, "DDCO AGC setting", CLI_SUPERVISOR},
    {"DDCO LFE LFP",        "DDCOLFE",     _AudCmdUopDDCOLFE, NULL, "DDCO AGC setting", CLI_SUPERVISOR},
    {"dualmono",    "dm",    _AudCmdDtvDualMono , NULL,    "dual-mono steup", CLI_GUEST},
#ifdef CC_AUD_DSP_SUPPORT_AUDYSSEY
    {"ADV",         "ADV", NULL, _arAudUopADVTbl, "Set ADV Function", CLI_GUEST},
    {"ABX Config",  "abxcfg",   _AudCmdUopABXConfig, NULL, "ABX Config", CLI_GUEST},
    {"aeq",         "aeq",      _AudCmdUopAeqFlag,   NULL, "Set AEQ Flag", CLI_GUEST},
#endif
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
    {"upload",        "up",     _AudCmdUopUploadData , NULL, "Upload data enable", CLI_SUPERVISOR},
    {"uploadinit",    "upi",    _AudCmdDataUploadInit, NULL, "Upload data init", CLI_SUPERVISOR},
    {"uploaddump",    "upd",    _AudCmdDataUploadDump, NULL, "Upload data dump", CLI_SUPERVISOR},
//#ifdef LINUX_TURNKEY_SOLUTION
#if 0
    {"rpq",           "rpq",    _AudCmdRecordPcmQuery, NULL, "Record PCM Query", CLI_GUEST},
#endif
    {"encmode",    "enc",    _AudCmdEncMode, NULL, "Set Encode mode", CLI_SUPERVISOR},
//#endif
    {"adupdate",    "adupdate",    _AudCmdAdFadePanPesUpdate , NULL,    "ad fade pan update setup", CLI_SUPERVISOR},
    {"adfade",    "adfade",    _AudCmdAdFade , NULL,    "ad fade setup", CLI_SUPERVISOR},
    {"adpan",    "adpan",    _AudCmdAdPan , NULL,    "ad pan setup", CLI_SUPERVISOR},
    {"adpanfaden",    "adpanfaden",   _AudCmdAdPanFadeEnable , NULL,    "ad pan fade enable", CLI_SUPERVISOR},
#ifdef CC_AUD_4_DECODER_SUPPORT
    {"sbcdump",   "sbcd", _AudCmdSBCEncTest, NULL, "SBC Encode Test", CLI_SUPERVISOR},
#endif
    {"adcdump",   "adcd", _AudCmdADCDump, NULL, "SBC Encode Test", CLI_SUPERVISOR},
    {"postq",     "pq",   _AudCmdUopPostQuery, NULL, "Post-Proc Query", CLI_GUEST},

#ifdef CC_AUD_FIR_SUPPORT
        {"coneq",     "coneq", NULL, _arAudUopConeqTbl,  "Set Coneq Function", CLI_GUEST},
#endif
    {"g726dec",        "g726dec",     _AudCmdUopG726Dec , NULL, "G726 decoder setting", CLI_SUPERVISOR},
    {"check sum _pbD",        "csum",     _AudCmdUopCheckSum , NULL, "check sum _pbD", CLI_SUPERVISOR},
#ifdef KARAOKE_MIC_LINE_IN2       
    {"Karaoke", "mic",     _AudCmdUopKaraokeMicIn, NULL, "Karaoke Line-in2 mix to Dec1", CLI_GUEST},
#endif    
  
#ifdef CC_VOCAL_MUTE    
    {"vm",     "vm", NULL, _arAudVocalMuteCmdTbl, "Vocal Mute Command", CLI_GUEST},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudCfgCmdTbl[] =
{
    {"AvInSwitch",    "isw",     _AudCfgAvInSwitch,   NULL, "Av input component select", CLI_GUEST},
    {"ChlOutCfg",     "osw",     _AudCfgChlOutSwitch, NULL, "Out channel switch", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudAtvCmdTbl[] =
{
    {"query",     "q",    _AudCmdAtvquery, NULL, "query ATV audio informantion", CLI_GUEST},
    {"Tvsys",     "sys",    _AudCmdAtvTvSys, NULL, "Tv channel system: B/G, D/K, I, L, M", CLI_GUEST},
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
    {"Tvsys auto",     "asys",    _AudCmdAtvAutoTvSys, NULL, "Set tvsys by detection", CLI_SUPERVISOR},
#endif
    {"sound mode",      "sm",     _AudCmdAtvSoundMode, NULL, "ATV Sound Mode: mono, stereo, dual,...", CLI_GUEST},
    {"MTS system", "mts",    _AudCmdAtvFacMts, NULL, "BTSC system factory options", CLI_GUEST},
    {"A2 system", "a2",     _AudCmdAtvFacA2, NULL, "A2 system factory options", CLI_GUEST},
    {"PAL system", "pal",     _AudCmdAtvFacPal, NULL, "PAL system factory options", CLI_GUEST},
    {"Query carrier shift",  "CFO",     _AUD_MTS_CFO_IDX, NULL, "Query MTS Carrier Shift", CLI_SUPERVISOR},
    {"query OVM  Detec",     "qovm",    _AudCmdAtvHdevDetQuery, NULL, "query ATV Hdev detection result", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudDtvAC3Tbl[] =
{
    {"ac3kara",        "ac3ka",    _AudCmdDtvAC3KaraMode , NULL,    "ac3 karaoke mode steup", CLI_SUPERVISOR},
    {"ac3dualmono",    "ac3dm",    _AudCmdDtvAc3DualMono , NULL,    "ac3 dual mono mode steup", CLI_SUPERVISOR},
    {"ac3compmode",    "ac3cm",    _AudCmdDtvAc3CompMode , NULL,    "ac3 compresssion mode steup", CLI_GUEST},
    {"ac3drcrange",    "ac3dr",    _AudCmdDtvAc3DrcRange , NULL,    "ac3 drc range  steup", CLI_SUPERVISOR},
    {"ac3query", "q", _AudCmdDtvAc3Query, NULL, "ac3 query decoder status", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_AUD_SUPPORT_MS10
static CLI_EXEC_T _arAudDtvDDCTbl[] =
{
    {"ddccontrol",        "ddc",    _AudCmdDtvDDCControl, NULL,    "DDC control steup", CLI_SUPERVISOR},
    {"strmid",        "id",    _AudCmdDtvDDCStrmId , NULL,    "DDC associated stream id steup", CLI_SUPERVISOR},
    {"mixer",        "m",    _AudCmdDtvDDCMixer , NULL,    "DDC mixer steup", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

#ifdef CC_AUD_SUPPORT_MS10
static CLI_EXEC_T _arAudDtvDDTTbl[] =
{
    {"MDC bypass",          "mdc",    _AudCmdBypassMDC , NULL,    "DDT bypass mode", CLI_SUPERVISOR},
    {"dmx mode",            "dmx",    _AudCmdDtvDdtDmxMode , NULL,    "DDT downmix mode", CLI_SUPERVISOR},
    {"RF mode",             "rf",    _AudCmdDtvDdtRfMode , NULL,    "DDC RF/Line in mode", CLI_SUPERVISOR},
    {"Dual mode",           "dual",    _AudCmdDtvDdtDualMode , NULL,    "DDC dual mode", CLI_SUPERVISOR},
    {"DRC range",           "drc",    _AudCmdDtvDdtDrcRange , NULL,    "DDC DRC Cut/Boost factor", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif

static CLI_EXEC_T _arAudDtvCmdTbl[] =
{
    {"query DTV",     "q",    _AudCmdDtvQuery, NULL, "Query DTV decoder informantion", CLI_GUEST},
    {"ptsdly",      "ptsd", _AudCmdDtvPTSDelay,     NULL,  "Delay audio startup by increasing PTS", CLI_SUPERVISOR},
    {"Set AC3",     "ac3",    NULL, _arAudDtvAC3Tbl, "Set AC3 decoder informantion", CLI_GUEST},
#ifdef CC_AUD_SUPPORT_MS10
    {"Set DDC",     "ddc",    NULL, _arAudDtvDDCTbl, "Set DDC decoder informantion", CLI_SUPERVISOR},
    {"Set DDT",     "ddt",    NULL, _arAudDtvDDTTbl, "Set DDT decoder informantion", CLI_SUPERVISOR},
#endif
    {"DolbyLicense", "dls", _AudCmdDtvDolbyLicenseSupport, NULL, "Query Supported Dolby License", CLI_SUPERVISOR},
#ifdef CC_MPEG_LEVEL_ADJ
    {"Set Mpeg Level",     "mpglvl", _AudCmdSetMpegLevelAdj, NULL, "Set Mpeg Level Adjust", CLI_GUEST},
#endif
#ifdef NEW_COUNTRY_TYPE
    {"Country", "c", _AudCmdDtvTargetCountryQuery, NULL, "Query Target Country", CLI_SUPERVISOR},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudFMRadioCmdTbl[] =
{
    {"detect FM",     "det",    _AudCmdFMRadioDetect, NULL, "Detect FM radio channel", CLI_SUPERVISOR},
    {"play FM",       "play",   _AudCmdFMRadioPlay,   NULL, "Play FM radio channel", CLI_GUEST},
    {"FM stereo",     "fm2",   _AudCmdFMRadioSoundMode,   NULL, "Setup FM radio channel", CLI_GUEST},
    {"FM THL",        "fmthl",   _AudCmdFMRadioThre,   NULL, "Setup FM radio threshold", CLI_GUEST},
    {"FM Query THL",        "fmthlq",   _AudCmdFMRadioQueryThre,   NULL, "Query FM radio threshold", CLI_GUEST},
    {"vol",        "vol",   _AudCmdFMRadioSetFineVolume,   NULL, "Fine tune volume", CLI_GUEST},
    {"enable notify", "en",  _AudCmdFMRadioEnableNotify,  NULL, "Enable FM detect Notify", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudIoCmdTbl[] =
{
    {"spdif",       NULL,   NULL, _arAudSpdifCmdTbl,    "spdif command", CLI_GUEST },
    {"adac",        NULL,   NULL, _arAudAdacCmdTbl,     "adac command", CLI_GUEST },
    {"interdac",    "idac",   NULL, _arAudInterDacCmdTbl, "inter dac command", CLI_GUEST },
    {"aadc",        NULL,   NULL, _arAudAadcCmdTbl,     "aadc command", CLI_SUPERVISOR },
    {"cfg",         NULL,   NULL, _arAudCfgCmdTbl,      "configuration", CLI_GUEST },
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudNvmCmdTbl[] =
{
#ifdef CC_AUD_USE_NVM
    {"Read",       "r",     _AudCmdNvmRead,  NULL,  "Read  NVM", CLI_GUEST},
    {"Wrtie",      "w",     _AudCmdNvmWrite, NULL,  "Write NVM",  CLI_GUEST},
    {"Initialize", "init",  _AudCmdNvmInit,  NULL,  "Initialize NVM", CLI_GUEST},
    {"ProfileRead", "pfr",  _AudCmdProfileRead, NULL, "Profile Read", CLI_GUEST},
    {"AQ list all", "l", _AudCmdNvmFlashAQListAll  , NULL, "AQ list all data on flash", CLI_GUEST },
#endif
#ifdef CC_AUD_USE_FLASH_AQ
    {"AQ write", "w",   _AudCmdFlashAQWrite, NULL,  "AQ Write", CLI_GUEST },
    {"AQ read",  "r",   _AudCmdFlashAQRead, NULL,   "AQ Read", CLI_GUEST },
    {"AQ version", "v", _AudCmdFlashAQGetVersion, NULL, "AQ version", CLI_GUEST },
    {"AQ version cmp", "vc", _AudCmdFlashAQComVersion, NULL, "AQ version compare", CLI_GUEST },
    //{"AQ set default", "df", _AudCmdFlashAQSetDefault  , NULL, "AQ put default to flash", CLI_GUEST },
    {"AQ list all", "l", _AudCmdFlashAQListAll  , NULL, "AQ list all data on flash", CLI_GUEST },
    //{"AQ fill to dram", "fill", _AudCmdFlashAQFillDataStruct  , NULL, "AQ fill data from flash to data structure", CLI_GUEST },
    {"Bass Offset", "bto", _AudCmdFlashAQBassTrebleOffset  , NULL, "Bass Treble Offset", CLI_GUEST },        
#endif //CC_AUD_USE_FLASH_AQ
#ifdef SUPPORT_FLASH_AQ
    #ifndef CC_COMPRESSED_FLASH_AQ_DATA
    {"AQ read extension", "rx", _AudCmdFlashAQReadEx, NULL, "AQ Read extension", CLI_GUEST},
    #else
    {"AQ read extension", "rx", _AudCmdFlashAQReadEx_LZMA, NULL, "AQ Read extension", CLI_GUEST}, 
    #endif
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arAudAsrcCmdTbl[] =
{
    {"query ASRC",     "q",    _AudCmdAsrcQuery, NULL, "Query ASRC status", CLI_SUPERVISOR},
    {"track mode",     "t",    _AudCmdAsrcTrackingMode, NULL, "Enable ASRC Tracking Mode", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
static CLI_EXEC_T _arAudOssCmdTbl[] =
{
    {"Oss_src",  "s",    _AudOssAdap_SetSrc, NULL,  "Oss Adaptation set src", CLI_SUPERVISOR},
    {"Oss_cmd",  "cmd",    _AudOssAdap_Cmd, NULL,  "Oss Adaptation command", CLI_SUPERVISOR},
    {"Oss_Ops",  "e",    _AudOssAdap_Ops, NULL,  "Oss Adaptation Operations", CLI_SUPERVISOR},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif


#ifdef CC_AUD_MIXSOUND_SUPPORT
static CLI_EXEC_T _arAudMixSndCmdTbl[] =
{
    //CC_AUD_MIXSND_CLI1
    {"mixsnd volume", "v",    _AudCmdMixSndVol,  NULL, "Mixsound Volume control", CLI_GUEST},
    {"mixsnd loop",   "l",    _AudCmdMixSndLoop, NULL, "Mixsound Loop control", CLI_GUEST},
    {"mixsnd query",  "q",    _AudCmdMixSndQuery,NULL, "Mixsound status query", CLI_GUEST},
    {"mixsnd flag",     "flag", _AudCmdMixSndFlag, NULL, "Mixsound flag", CLI_GUEST},
    {"mixsnd play",     "p",    _AudCmdMixSndPlay, NULL, "Mixsound playback", CLI_GUEST},
    {"mixsnd source",   "str",  _AudCmdMixSndClip, NULL, "Select clip source", CLI_GUEST},
    {"mixsnd dec3",   "dec3",  _AudCmdMixSndDec3, NULL, "Mixsound from Dec3", CLI_GUEST},
    {"mixsnd channel",   "ch",  _AudCmdMixSndChannel, NULL, "Set Mixsound channel", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
static CLI_EXEC_T _arAudMixSndClipCmdTbl[] =
{
    {"init",   "init",    _AudCmdInitMixSndClip,  NULL,  "Audio Mixsound Clip Init", CLI_GUEST},
    {"set",    "set",     _AudCmdSetMixSndClip,   NULL,  "Audio Mixsound Clip Set", CLI_GUEST},
    {"play",   "play",    _AudCmdPlayMixSndClip,   NULL,  "Audio Mixsound Clip Play", CLI_GUEST},
    {"stop",   "stop",    _AudCmdStopMixSndClip,   NULL,  "Audio Mixsound Clip Stop", CLI_GUEST},
    {"pause",  "pause",   _AudCmdPauseMixSndClip,   NULL,  "Audio Mixsound Clip Pause", CLI_GUEST},
    {"resume", "resume",  _AudCmdResumeMixSndClip,   NULL,  "Audio Mixsound Clip Resume", CLI_GUEST},
    {"repeat", "repeat",  _AudCmdRepeatMixSndClip,   NULL,  "Audio Mixsound Clip Repeat", CLI_GUEST},
    {"q",      "q",       _AudCmdQueryMixSndClip, NULL, "Audio Mixsound Clip Query", CLI_GUEST},
    {"qdsp",   "qdsp",    _AudCmdQueryMixSndClipDsp, NULL, "Audio Mixsound Clip Query(DSP)", CLI_GUEST},
    #ifdef MIXSOUND_MIPS_MEASURE
    {"mq",     "mq",      _AudCmdQueryMixSndClipMIPS, NULL, "Audio MixSound Clip Query(MIPS)", CLI_GUEST},
    {"mr",     "mr",      _AudCmdResetMixSndClipMIPS, NULL, "Audio MixSound Clip Reset(MIPS)", CLI_GUEST},
    #endif
    {"clr",    "clr",     _AudCmdClearMixSndClip, NULL, "Audio MixSound Clip Clear", CLI_GUEST},
    {"mute",   "mute",    _AudCmdMuteMixSndClip,  NULL, "Audio MixSound Clip Mute", CLI_GUEST},
    {"gain",   "gain",    _AudCmdSetGain, NULL, "Audio MixSound Clip SetGain", CLI_GUEST},
    #ifdef MIXSND_OUTPUT_DBG
    {"mda",  "mda", _AudCmdMixSndDumpAlloc, NULL,   "Allocate memory for MixSnd Dump", CLI_GUEST },
    {"mdr",  "mdr", _AudCmdMixSndDumpReset, NULL, "Reset MixSnd Dump", CLI_GUEST },
    {"mds",  "mds", _AudCmdMixSndDumpSet, NULL, "Set MixSnd Dump Location", CLI_GUEST},
    #endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif
#endif

static CLI_EXEC_T _arAudDramCmdTbl[] =
{
    {"qm",  "qm",   _AudCmdQueryDspMemory,   NULL, "Query AUD Dram Map", CLI_GUEST},
    {"dm",  "dm",   _AudCmdDumpDspMemory,    NULL, "dump dsp memory", CLI_SUPERVISOR},
    {"r",   "r",    _AudCmdReadBytePhysical, NULL, "Read physical memory in bytes", CLI_GUEST},
    {"r4",  "r4",   _AudCmdReadDwrdPhysical, NULL, "Read physical memory in dwrds", CLI_GUEST},
    {"phy", "phy",  _AudCmdGetPhyAddr,       NULL, "Virtual to Physical convert", CLI_GUEST},
    {"vir", "vir",  _AudCmdGetVirAddr,       NULL, "Physical to Virtual convert", CLI_GUEST},
    {"sh",  "sh",   _AdspDSPShareInfo,       NULL, "read dsp share memory: sh [group]", CLI_SUPERVISOR},
    {"shw", "shw",  _AdspDSPSharInfoWrite,   NULL, "write dsp share memory: sh [group] [addr] [val] [size]", CLI_SUPERVISOR},
    {"cm",  "cm",   _AdspDSPCommonDram,      NULL, "read dsp common memory: cm [addr] [length]", CLI_SUPERVISOR},
    {"cmw", "cmw",  _AdspDSPCommonDramWrite, NULL, "write dsp common memory: cm [addr] [value]", CLI_SUPERVISOR},
    {"norm","norm", _AdspDSPNormDram,        NULL, "read dsp normal dram", CLI_SUPERVISOR},
    {"cmpt","cmpt", _AdspDSPCmptDram,        NULL, "read dsp compact dram", CLI_SUPERVISOR},
    {"chldump","c", _AdspDSPChlDump,         NULL, "dump channel ouput data", CLI_SUPERVISOR},
    {"flush", "flush", _AudCmdFlushDspMemory, NULL, "Flush DSP memory", CLI_SUPERVISOR},
#ifdef LINUX_TURNKEY_SOLUTION
    #ifdef __KERNEL__
    {"dump", "dump", _AudCmdDumpFifo2File, NULL, "dump Fifo data to file", CLI_SUPERVISOR},
    {"ul", "ul", _AudCmdUlFile2Dram, NULL, "Upload file data to dram", CLI_SUPERVISOR},
    {"mal", "mal", _AudCmdDramAlloc, NULL, "Allocate Dram", CLI_SUPERVISOR},
    #endif
    {"dumpLoop", "dumpLoop", _AudCmdDumpFifo2FileInfinit, NULL, "dump Fifo data to file infinitely", CLI_SUPERVISOR},
    {"dumpStop", "dumpStop", _AudCmdDumpFifo2FileInfinitStop, NULL, "stop dumpLoop", CLI_SUPERVISOR},
    {"dumpAutoSave", "dumpAS", _AudCmdDumpFifo2FileAutoSave, NULL, "Set dumpLoop auto save to a new file", CLI_SUPERVISOR},
#endif
#ifndef __KERNEL__
        {"dump", "dump", _AudCmdDumpFifo2File_Nucle, NULL, "dump Fifo data to file", CLI_SUPERVISOR},
#endif

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_AOMX)
//CC_AUD_SUPPORT_OPENMAX
static CLI_EXEC_T _arAudOmxCmdTbl[] =
{
    {"lpcm", "lpcm", _AudCmdOmxLPCMSetting,  NULL, "LPCM Setting",  CLI_GUEST},
    {"wma", "wma", _AudCmdOmxWMASetting, NULL, "WMA Setting", CLI_GUEST},
    {"wma10", "wma10", _AudCmdOmxWMAProSetting, NULL, "WMA10 Setting", CLI_GUEST},
    {"cook", "cook", _AudCmdOmxCOOKSetting, NULL, "Cook Setting", CLI_GUEST},
    {"dspdelay", "dspdelay", _AudCmdOmxDspDelaySetting, NULL, "DSP Delay Setting", CLI_GUEST},
    {"bypass", "bypass", _AudCmdOmxSettingBypass, NULL, "Bypass OMX Setting", CLI_GUEST},
    {"dm","dm",_AudCmdOmxSetDbgMsk,NULL,"Set Debug Mask", CLI_GUEST},
    {"q", "q", _AudCmdOmxQuery, NULL,"Query", CLI_GUEST},
    {"delay", "delay", _AudCmdOmxDelaySetting, NULL, "Set OMX timestamp Offset", CLI_GUEST},
    {"alsaq", "alsaq", _AudCmdOmxAlsaQuery, NULL, "Query ALSA status", CLI_GUEST},
    {"alsav", "alsav", _AudCmdOmxAlsaVolume, NULL, "Set ALSA volume", CLI_GUEST},
    {"oda", "oda", _AudCmdOmxDumpAlloc, NULL, "OMX Dump Buf Alloc", CLI_GUEST},
    {"odr", "odr", _AudCmdOmxDumpReset, NULL, "OMX Dump Buf Reset", CLI_GUEST},
    {"ods", "ods", _AudCmdOmxDumpSet,   NULL, "OMX Dump Location Set", CLI_GUEST},
    {"lpcmcw", "lpcmcw", _AudCmdOmxLPCMCW, NULL, "LPCM Setting", CLI_GUEST},
    {"skipmtal", "skipmtal", _AudCmdOmxSkipMTAL, NULL, "Skip MTAL", CLI_GUEST},
    {"gst", "gst", _AudCmdOmxGstDbgMsk, NULL, "GST Debug Mask", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
//CC_AUD_SUPPORT_OPENMAX
#endif

#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_ADECOMX)
static CLI_EXEC_T _arAudDecOmxCmdTbl[] =
{
    {"dm", "dm", _AudCmdAdecOmxSetDbgMsk, NULL, "Set Debug Mask", CLI_GUEST},
    {"q", "q", _AudCmdAdecOmxQuery, NULL, "Query", CLI_GUEST},      
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}    
};
#endif

static CLI_EXEC_T _arAudAVSyncCmdTbl[] =
{
    {"debug",  "d",   _AudCmdAvSyncDbgLvl,   NULL, "Set AV Sync Debug On/Off", CLI_GUEST},
    {"stcid",  "sid", _AudCmdAvSyncSetStcID,   NULL, "Set Audio STC ID", CLI_SUPERVISOR},
    {"qptsq",   "qq", _AudQueryPtsQueue, NULL, "Query PTS Queue", CLI_SUPERVISOR}, //PTSQueue_V2
    {"qpts",   "qpts", _AudQueryPts, NULL, "Query PTS Info", CLI_SUPERVISOR},
#if 0 // 2_STC_Task_DEBUG_only
    {"stc",    "s",   _AudCmdAvSyncReadStc,  NULL, "Read all STC", CLI_GUEST},
#endif
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
static CLI_EXEC_T _arAudPlayMuteCmdTbl[] =
{
    {"clear",  "c",   _AudCmdClearPlayMuteHistory,   NULL, "Clear history", CLI_GUEST},
    {"query",  "q", _AudCmdQueryPlayMuteHistory,   NULL, "Query history", CLI_GUEST},
    {"qdelay",  "qd", _AudCmdClearPlayMuteQueryDelayInfo,   NULL, "Query delay", CLI_GUEST},
    {"enable", "e", _AudPlayMuteEnable,   NULL, "Enable play mute control", CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
#endif


/******************************************************************************
* Variable      : cli "aud" command table
******************************************************************************/

static CLI_EXEC_T _arAudCmdTbl[] =
{
    {"init",    "i",    _AudCmdInit, NULL,          "Aud init", CLI_SUPERVISOR},
    CLIMOD_DEBUG_CLIENTRY(AUD),
    //CLIMOD_REGTST_CLIENTRY(AUD),
    {"setdec",  "s",    _AudCmdSetDecType,   NULL,  "Aud set decoding type", CLI_SUPERVISOR},
    {"resume",  NULL,   _AudCmdResume,       NULL,  "Aud resume", CLI_SUPERVISOR},
    {"pause",   NULL,   _AudCmdPause,        NULL,  "Aud pause", CLI_SUPERVISOR},
    {"play",    NULL,   _AudCmdPlay,         NULL,  "Aud play", CLI_SUPERVISOR},
    {"stop",    NULL,   _AudCmdStop,         NULL,  "Aud stop", CLI_SUPERVISOR},
    {"dual",    "dual", _AudCmdDualDecMode, NULL,  "dual", CLI_SUPERVISOR},
#ifdef CC_MT5391_AUD_3_DECODER
    {"tri",     "tridec", _AudCmdTriDecMode, NULL,  "tri_dec", CLI_SUPERVISOR},
#ifdef CC_AUD_LINEIN_USE_DEC3
    {"trionly",     "trideconly", _AudCmdTriDecOnlyMode, NULL,  "tri_dec_only", CLI_SUPERVISOR},
#endif
#endif
    {"sync",    NULL,   _AudCmdSyncMode,     NULL,  "Aud sync mode setting", CLI_SUPERVISOR},
    {"stream",  "str",  _AudCmdSetStream,    NULL,  "Playback stream src", CLI_SUPERVISOR},
    {"query",   "q",    _AudCmdQuery,        NULL,  "Audio status query", CLI_GUEST},
    #ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
    {"oss",     NULL,   NULL,  _arAudOssCmdTbl,  "OSS adaption layer command", CLI_GUEST},
    #endif
#ifdef CC_AUD_MIXSOUND_SUPPORT
    {"m",       NULL,   NULL, _arAudMixSndCmdTbl,   "Audio mixsound playback", CLI_SUPERVISOR},
#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
    {"mclip",   NULL,   NULL, _arAudMixSndClipCmdTbl, "Audio mixsound clip playback", CLI_SUPERVISOR},
#endif
    {"pll1",    NULL,   _AudCmdPll1Set,         NULL,  "Aud Pll1 setting", CLI_SUPERVISOR},
    {"pll2",    NULL,   _AudCmdPll2Set,         NULL,  "Aud Pll2 setting", CLI_SUPERVISOR},
    {"pllpd",   NULL,   _AudCmdPllPowerDown,    NULL,  "Aud Pll PoweDown", CLI_SUPERVISOR},
#endif
    {"dsp",     NULL,   NULL, _arAudDspCmdTbl,      "dsp command", CLI_GUEST },
#ifdef CC_AUD_ARM_SUPPORT
    {"aproc",   "a",    NULL, _arAudAprocCmdTbl,        "audio processor command", CLI_SUPERVISOR },
#ifdef CC_AUD_ARM_RENDER
    {"iec",     "iec",  NULL, _arAudAprocIecCmdTbl,     "IEC related command", CLI_GUEST },
    {"spost",   "spost",NULL, _arAudSpostCmdTbl,        "Spost related command", CLI_GUEST },
#endif    
#endif
    {"uop",     NULL,   NULL, _arAudUopCmdTbl,      "audio uop", CLI_GUEST },
    {"t",       NULL,   NULL, _arAudTestCmdTbl,     "test", CLI_GUEST },
    {"Clip",    NULL,   NULL, _arAudClipCmdTbl,     "Aud Clip", CLI_SUPERVISOR },
#if 0 //def __MODEL_slt__
    {"dspslt",  NULL,   NULL, _arAudDSPSLTCmdTbl,   "DSP SLT", CLI_SUPERVISOR },
#endif
#if defined(__linux__)
    {"isplay",  "ip",   _AudCmdIsDecoderPlay, NULL,     "Check if audio decoder is playing", CLI_SUPERVISOR},
#endif
    {"atv",     NULL,  NULL,  _arAudAtvCmdTbl,  "ATV command", CLI_GUEST},
    {"dtv",     NULL,  NULL,  _arAudDtvCmdTbl,  "DTV command", CLI_GUEST},
    {"fmrdo",   NULL, NULL, _arAudFMRadioCmdTbl, "FM Radio command", CLI_SUPERVISOR},
    {"io",      NULL,  NULL,  _arAudIoCmdTbl,  "I/O command", CLI_GUEST},
    {"nvm",     "nvm",   NULL, _arAudNvmCmdTbl,  "Nvm Read/Wrtie", CLI_SUPERVISOR },
    {"src",     NULL,   NULL, _arAudAsrcCmdTbl,  "ASRC status query", CLI_SUPERVISOR},
    {"dram",    NULL,   NULL, _arAudDramCmdTbl,  "DRAM command", CLI_GUEST},
#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_AOMX)
    {"omx",     NULL,   NULL, _arAudOmxCmdTbl,   "OMX command", CLI_GUEST}, //CC_AUD_SUPPORT_OPENMAX
#endif
#if defined(LINUX_TURNKEY_SOLUTION) && defined(CC_ENABLE_ADECOMX)
    {"adecomx", NULL,   NULL, _arAudDecOmxCmdTbl,"ADECOMX command", CLI_GUEST},
#endif
    {"syncdbg", "sd",   NULL, _arAudAVSyncCmdTbl,  "AV Sync Debug command", CLI_GUEST},
    {"dolby",     "db",   NULL, _arAudDolbyCmdTbl,      "Dolby related command", CLI_GUEST },
#ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
    {"pm",  NULL,    NULL,   _arAudPlayMuteCmdTbl,  "Query play mute history", CLI_SUPERVISOR},
#endif
#ifdef AUD_DECODER3_SELF_TEST
    {"dualplay",     "dp",   _AudCmdSetDualPlay, NULL,      "Set Dual Play", CLI_GUEST },
#endif
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
    {"psrdbg",     "pd",   _AudParserDebug, NULL,      "Set Parser debug flag", CLI_SUPERVISOR },
#endif    
    {"soundbar",    "sb",    _AUDWatchSpdifEnable,  NULL,"Watch sound bar register", CLI_SUPERVISOR},
    {"setdelay",    "setdelay",  _AUDSetVdecDelayTbl,   NULL,"Set Vdep Delay Tbl", CLI_SUPERVISOR},
    {"SpeedTest", "speed",   _AudMMPlaybackSpeedTestCmd,     NULL,    "mm playback speed test",   CLI_SUPERVISOR},
#ifdef ADSP_BIN_SUPPORT
    {"UsbBoot", "ubt",	_AudUsbBoot,			NULL,	"Test Usb boot"},
#endif
    {"udbgmsk", "udm",	_AudUserDebugMask,			NULL,	"user debug mask", CLI_GUEST },        
    {NULL,      NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Aud)
{
    "aud",
    NULL,
    NULL,
    _arAudCmdTbl,
    "Aud command",
    CLI_GUEST
};

#if (0)
#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetAudMwCmdTbl(void)
#else
CLI_EXEC_T* GetAudCmdTbl(void)
#endif
{
    return &_rAudModCmdTbl;
}
#endif

