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
 * $Date: 2015/04/08 $
 * $RCSfile: aud_if.c,v $
 * $Revision: #19 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_if.c
 *  Audio decoder control interface primitive API
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_util.h"

#include "feeder_if.h"
#include "aud_debug.h"
#include "aud_if.h"
#include "aud_drv.h"
#include "aud_cfg.h"
#include "aud_notify.h"
#include "adac_if.h"
#include "dsp_common.h"
#include "drv_adsp.h"
#include "drvcust_if.h"
#include "aud_hw.h"
#include "x_aud_dec.h"
#include "sif_if.h"
#include "codec_AD82581.h"
#include "dsp_intf.h"

#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#endif
#include "aud_dsp_cfg.h"
#ifdef SUPPORT_FLASH_AQ
#include "nor_if.h"
#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif
#endif
#include "storage_if.h"

#ifdef ENABLE_MULTIMEDIA
#include "swdmx_if.h"
#endif
#include "vdec_drvif.h"

#include "psr_drvif.h"


#ifdef LINUX_TURNKEY_SOLUTION
#include <asm/uaccess.h>
#endif

#ifdef CC_AUD_USE_FLASH_AQ
#include "panel.h"
#include "storage_if.h"
#endif //CC_AUD_USE_FLASH_AQ

#include "../../lzma/LzmaEnc.h"
#include "../../lzma/LzmaLib.h"


LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define CFO_ACC_PROB_SRAM_ADDR 0x36F6

#define USE_16BIT_ALIGNMENT

//#define CC_AUD_DUMP_LOG    //turn on for debug by log dump
#define AUD_DUMP_LOG_NUM        60
#define AUD_DUMP_LOG_LENGTH     70

//#define CC_COMPRESSED_FLASH_AQ_DATA

#define AUD_INVALID_CTRL_TYPE   (0xffff)

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
extern void _MT5365_Mute(UINT8 u1DacId, BOOL fgEnable);

extern AUD_AOUT_PAD_CFG_T _grAOutPadCfg;
extern LGSE_FN_PARAM_T _argLgseFnPara[11];

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern UINT32 u4ReadDspSram(UINT8 u1DspId, UINT32 u4Addr);
extern void vADSPIRQEnable(BOOL fgEnable);
#ifdef CC_AUD_DDI
extern void AUD_LGSEFN000(UINT8 fNo, VOID* u1CV_param_buf, UINT16 noParam, UINT8  dataOption, UINT8 varOption);
extern void AUD_GetLGSEFN010Out(UINT32* pParam);
#endif
#ifdef CC_AUD_ARM_POST
extern void vAproc_Enable(UINT32 u4Enable);
extern void vAprocS_EnableConfig(BOOL fgEnable);
#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifdef CC_DUAL_AUD_DEC_SUPPORT
static BOOL bSkipDSPCmdLst[AUD_DSP_NUM][AUD_DEC_MAX] = {{FALSE, FALSE, FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE, FALSE, FALSE}};
UINT8 _u1MW_SetFocusOn = AUD_DEC_MAIN;
#endif

static HANDLE_T _hSema;
static HANDLE_T _hCmdWaitSema[AUD_DSP_NUM][AUD_DEC_MAX];
static HANDLE_T _hAsynCmdSema;
static HANDLE_T _hMwCtrlSema;

static UINT32 _au4AudCtrlType[AUD_DSP_NUM][AUD_DEC_MAX] = { { AUD_INVALID_CTRL_TYPE, AUD_INVALID_CTRL_TYPE, AUD_INVALID_CTRL_TYPE, AUD_INVALID_CTRL_TYPE },
                                                            { AUD_INVALID_CTRL_TYPE, AUD_INVALID_CTRL_TYPE, AUD_INVALID_CTRL_TYPE, AUD_INVALID_CTRL_TYPE } }; 
static AUD_DEC_CMD_T _aeAudCtrlCmd[AUD_DSP_NUM][AUD_DEC_MAX] = { { AUD_CMD_NULL, AUD_CMD_NULL, AUD_CMD_NULL, AUD_CMD_NULL },
                                                                 { AUD_CMD_NULL, AUD_CMD_NULL, AUD_CMD_NULL, AUD_CMD_NULL } };

static BOOL _afgDecPause[AUD_DSP_NUM][AUD_DEC_MAX] = {{FALSE, FALSE, FALSE, FALSE},{FALSE, FALSE, FALSE, FALSE}};

static CHAR* _paAudCmd[] =
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


static UINT8 _au1ChannelVolume[AUD_DEC_MAX][AUD_CH_NUM];
static UINT8 _au1ChannelDelay[AUD_DEC_MAX][AUD_CH_NUM];
static BOOL  _fgClipChnVolChanged = FALSE;

static AUD_HP_COND_NFY_T _aeHPCond[AUD_DEC_MAX] = 
{
    AUD_HP_COND_HEADPHONE_COND_UNPLUGED, 
    AUD_HP_COND_HEADPHONE_COND_UNPLUGED,
    AUD_HP_COND_HEADPHONE_COND_UNPLUGED,
    AUD_HP_COND_HEADPHONE_COND_UNPLUGED
};
static BOOL _bfgMMDecFmtConflict[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};
static BOOL _bfgMMDecFmtScramble[AUD_DEC_MAX] = {FALSE, FALSE, FALSE, FALSE};

#ifdef CC_AUD_SETUP_MELODY
//The default value of _fgSetupPlayFinish should be FALSE.
//If APP finish init before logo music starting play, it will get TRUE when query logo music status.
static BOOL _fgSetupPlayFinish = FALSE;
#endif

//static PCM_INPUT_TYPE_T rPcmEndian = PCM_16_BITS_BIG_ENDIAN;
static BOOL _fgLpcmTpyePS = FALSE;
#ifdef CC_AUD_DDI
static UINT8 _DTVFifoId=1;
#endif

static BOOL _fgPCMDetectRawMute = FALSE;

#ifdef CC_AUD_SETUP_MELODY
#ifdef  LINUX_TURNKEY_SOLUTION
VOID* pMelody_buf;
static UINT8 u1Melodyflag;
#endif
#endif

#ifdef CC_AUD_USE_FLASH_AQ
static UINT8 _u1FlashAQ_ID = 1;  // 1 .. 11,  AQ1 ~ AQ11.
#endif
#ifdef CC_AUD_VBASS_SUPPORT
UINT8 u1VBassCutOffFreq = 0;
#endif

static BOOL _fgAudInit = FALSE;
static BOOL _fgAudHWInit = FALSE;
static BOOL _fgAudInitOK = FALSE;
static BOOL _fgAud4KBEMuteDone = TRUE;
static HANDLE_T _hSemaAudInit = NULL_HANDLE;
#ifdef CC_SUPPORT_STR
static BOOL  _fgAudSpeakerPortOn = TRUE;
#endif

#ifdef CC_AUD_HDMI_SPDIF_CFG
AUD_OUTPUT_STATE _u1OutputState = DigitalMute_AnalogMute;
#endif

#ifdef  LINUX_TURNKEY_SOLUTION
static AUD_CLIP_TYPE_T _u1AudClip[3];
VOID* u1Clip_buf;
static UINT8 u1Clipfg;
#endif
static UINT16 u2AvSyncDbgFlag = 0;

#ifdef CC_VOCAL_MUTE
UINT8 _gu1VocalMuteFilterIdx1 = 0;
UINT8 _gu1VocalMuteFilterIdx2 = 0;
#endif

#ifdef CC_AUD_DUMP_LOG
static UINT8 u1AudDumpLogFIdx  = 0;
CHAR _gcAudDumpLog[AUD_DUMP_LOG_NUM][AUD_DUMP_LOG_LENGTH];
#endif
#ifdef CC_AUD_DDI
static UINT8 _GstreamerId = 4;
static BOOL _u1GstENCFlag = 0;
#endif
#ifdef  LINUX_TURNKEY_SOLUTION
static UINT32 ClipBufferSize = 0x110000;
#endif
BOOL _gfgVdoPictureOffStatus = FALSE;

//#ifdef CC_DUAL_AUD_DEC_SUPPORT
UINT8 gSubDecId = AUD_DEC_SUB;
UINT8 gMainDecId = AUD_DEC_MAIN;
//#define GET_SUB_ID(inId) ( ((inId) == AUD_DEC_SUB) ? gSubDecId : inId ) 
//#endif

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
AUD_DEC_CMD_STATE_T _aeAudDecCmdState[AUD_DSP_NUM][AUD_DEC_MAX] = {{AUD_CMD_ST_NULL, AUD_CMD_ST_NULL, AUD_CMD_ST_NULL, AUD_CMD_ST_NULL},
	                                                               {AUD_CMD_ST_NULL, AUD_CMD_ST_NULL, AUD_CMD_ST_NULL, AUD_CMD_ST_NULL}};
#endif

#ifdef CC_SUPPORT_STR
#include "hw_ckgen.h"

BOOL fgAudPmStrMode = FALSE;
BOOL fgAudPmStrResume = FALSE;
BOOL fgPsrPmStrResumeMode = FALSE;
BOOL fgEQSpetrumInfoStrResumeMode = FALSE;
UINT32 u4AudPmStrSkip = 0;
extern void _MT5398_DACDOWN(BOOL fgEnalbe);
extern void AudPllPowerDown(UINT8 PLL_ID, BOOL fgPD);
extern void vDspPowerOff (UINT8 u1DspId);
extern void vDspPowerOn(UINT8 u1DspId);
#if 0   // Unused
extern INT32 u4ADSP_DspReset(void);
#endif
extern void DSP_DrvSuspend(void);
extern BOOL fgDecoderStopped(UINT8 u1DspId, UINT8 u1DecId);
extern BOOL _afgMute[AUD_DEC_NUM];
extern AUD_DEC_MUTE_TYPE_T _aeMuteType[AUD_DEC_NUM];
#ifdef CC_AUD_APOLLO_SUPPORT
extern void vAprocMain_Close(void);
#endif
#endif

const AUD_ENUM_TO_NAME_T eAudStreamFromTbl[] = 
{
    {AUD_STREAM_FROM_OTHERS, "OTHERS"},
    {AUD_STREAM_FROM_DIGITAL_TUNER, "DTV"},
    {AUD_STREAM_FROM_ANALOG_TUNER, "ATV"},
    {AUD_STREAM_FROM_SPDIF, "SPDIF_IN"},
    {AUD_STREAM_FROM_LINE_IN, "LINE_IN"},
    {AUD_STREAM_FROM_HDMI, "HDMI"},
    {AUD_STREAM_FROM_MEMORY, "MEMORY"},
    {AUD_STREAM_FROM_BUF_AGT, "BUF_AGT"},
    {AUD_STREAM_FROM_FEEDER, "FEEDER"},
    {AUD_STREAM_FROM_MULTI_MEDIA, "MEDIA"},
    {AUD_STREAM_FROM_LINE_IN_2, "LINE_IN2"},
    {AUD_STREAM_FROM_LINE_IN_3, "LINE_IN3"},
#ifdef CC_ENABLE_AOMX 
    {AUD_STREAM_FROM_GST, "Gstreamer"},
#endif
#ifdef CC_AUD_I2S_IN
    {AUD_STREAM_FROM_I2S, "I2S_IN"}
#endif
};

const AUD_ENUM_TO_NAME_T eAudDrvFmtTbl[] = 
{
    {AUD_FMT_UNKNOWN, "UNKNOWN"},
    {AUD_FMT_MPEG, "MPEG"},
    {AUD_FMT_AC3, "AC3"},
    {AUD_FMT_PCM, "PCM"},
    {AUD_FMT_MP3, "MP3"},
    {AUD_FMT_AAC, "AAC"},
    {AUD_FMT_DTS, "DTS"},
    {AUD_FMT_WMA, "WMA"},
    {AUD_FMT_RA, "RA"},
    {AUD_FMT_MTS, "MTS"},
    {AUD_FMT_PAL, "PAL"},
    {AUD_FMT_A2, "PAL"},
    {AUD_FMT_DETECTOR, "DETECTOR"},
    {AUD_FMT_LPCM, "LPCM"},
    {AUD_FMT_FMRDO, "FMRDO"},
    {AUD_FMT_FMRDO_DET, "FMRDO_DET,"},
    {AUD_FMT_SBCDEC, "SBCDEC"},
    {AUD_FMT_SBCENC, "SBCENC"},
    {AUD_FMT_G729DEC, "G729"},
    {AUD_FMT_VORBISDEC, "VORBIS"},
    {AUD_FMT_WMAPRO, "WMAPRO"},
    {AUD_FMT_HE_AAC, "HE_AAC"},
    {AUD_FMT_HE_AAC_V2, "HE_AAC_V2"},
    {AUD_FMT_AMR, "AMR"},
    {AUD_FMT_AWB, "AWB"},
    {AUD_FMT_FLAC, "FLAC"},
    {AUD_FMT_G726, "G726"},
    {AUD_FMT_TV_SYS, "TV_SYS"},
    {AUD_FMT_WMA10LOSSLESS, "WMA10LOSSLESS"},
    {AUD_FMT_WMA10SPEECH, "WMA10SPEECH"}
};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _AudAsynCmdSemaRelease(UINT8 u1DecId, AUD_DEC_CMD_T eAudDecCmd)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    LOG(7, "Aud dec(%d) Async AUD_Command <%s> done\n", u1DecId, _paAudCmd[eAudDecCmd]);
    VERIFY(x_sema_unlock(_hAsynCmdSema) == OSR_OK);
}

static void _AudIfFixWarning(void)
{
    UNUSED(_paAudCmd);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// AUD_CommandDone
//
/** Audio notify command check.
 *
 *  @param  ucDecId         Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eAudDecCmd      audio command.
 *  @param  u4Result        audio status result.
 *
 *  @retval void
 */
void AUD_CommandDone(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_CMD_T eAudDecCmd, UINT32 u4Result)
{
    AudDecNfyFct   pfAudDecNfy = NULL;
    UINT8 u1NtyDecId = AUD_DEC_MAIN;
    UNUSED(u1NtyDecId);    

    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_DSP_ID_VALIDATE(u1DspId);
    LOG(7, "Aud dsp(%d) dec(%d) AUD_Command <%s> done\n", u1DspId, u1DecId, _paAudCmd[eAudDecCmd]);

    // Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));

    if (u1DspId == AUD_DSP0)
    {
    u1NtyDecId = u1DecId;
#ifdef CC_DUAL_AUD_DEC_SUPPORT
    if(eAudDecCmd == AUD_CMD_PLAY_SKIP)
    {
        eAudDecCmd=AUD_CMD_PLAY;
    }
    else if(eAudDecCmd == AUD_CMD_STOP_SKIP)
    {
        eAudDecCmd=AUD_CMD_STOP;
    }
    /*  Handle gSubDecId in MW_IF (Disconnect)    
    else if(u1DecId == gSubDecId)
    {
        u1NtyDecId=AUD_DEC_SUB;
    }
    */
#endif 

#ifdef CC_AUD_SETUP_MELODY
    if (((eAudDecCmd == AUD_CMD_STOP) || (eAudDecCmd == AUD_CMD_STOPSTREAM)) && (!_fgSetupPlayFinish))
    {
        _AUD_IgnoreSetupPlayNotify(FALSE);
        AUD_SetSetupPlayStatue(TRUE);
    }
#endif

#ifndef WITHOUT_MTK_MW_SOLUTION
#if defined(CC_DRIVER_BUILD)||defined(CC_AUD_SELF_UNMUTE)
    if (pfAudDecNfy != NULL)  // only for Nucleus demo program & linux demo program MM (both not from middleware)
#else
    if ((pfAudDecNfy != NULL) && ((_au4AudCtrlType[u1DspId][u1NtyDecId] != AUD_INVALID_CTRL_TYPE) || (eAudDecCmd == AUD_CMD_STOPSTREAM)))
#endif      
    {
        if (_aeAudCtrlCmd[u1DspId][u1NtyDecId] == eAudDecCmd)
        {
            _aeAudCtrlCmd[u1DspId][u1NtyDecId] = AUD_CMD_NULL;
            AUD_MWIgnoreDecNotify(u1DecId, FALSE);
        }
        // Notify middleware
#if 0//def CC_AUD_SETUP_MELODY
        if ( (eAudDecCmd == AUD_CMD_STOP) && (  _fgSetupPlayFinish == FALSE))
        {
            pfAudDecNfy((void*)AUD_NFY_STREAM, u1DecId, AUD_COND_AUD_CLIP_DONE,
                        _au4AudCtrlType[u1DecId], u4Result);
            _au4AudCtrlType[u1DecId] = AUD_INVALID_CTRL_TYPE;
        }

        else if ((eAudDecCmd == AUD_CMD_PLAY) || (eAudDecCmd == AUD_CMD_PAUSE) ||
                 (eAudDecCmd == AUD_CMD_STOP) || (eAudDecCmd == AUD_CMD_RESET) ||
                 (eAudDecCmd == AUD_CMD_RESUME))
        {
            pfAudDecNfy((void*)AUD_NFY_STREAM, u1DecId, AUD_COND_CTRL_DONE,
                        _au4AudCtrlType[u1DecId], u4Result);
            _au4AudCtrlType[u1DecId] = AUD_INVALID_CTRL_TYPE;
        }

#else
        if ((eAudDecCmd == AUD_CMD_PLAY) || (eAudDecCmd == AUD_CMD_PAUSE) ||
            (eAudDecCmd == AUD_CMD_STOP) || (eAudDecCmd == AUD_CMD_RESET) ||
            (eAudDecCmd == AUD_CMD_RESUME))
        {
            pfAudDecNfy((void*)AUD_NFY_STREAM, u1NtyDecId, AUD_COND_CTRL_DONE,
#ifdef CC_DRIVER_BUILD
                        eAudDecCmd, u4Result);
#else
                        _au4AudCtrlType[u1DspId][u1NtyDecId], u4Result);
#endif
            _au4AudCtrlType[u1DspId][u1NtyDecId] = AUD_INVALID_CTRL_TYPE;
        }

#endif
        else if (eAudDecCmd == AUD_CMD_STOPSTREAM)
        {
            pfAudDecNfy((void*)AUD_NFY_STREAM, u1NtyDecId, AUD_COND_AUD_CLIP_DONE,
                        _au4AudCtrlType[u1DspId][u1NtyDecId], u4Result);
            //_au4AudCtrlType[u1DecId] = AUD_INVALID_CTRL_TYPE;
        }

        AUD_MWIgnoreDecNotify(u1DecId, TRUE);   // Reset mw ignore notify
    }
#else
    if (pfAudDecNfy != NULL)
    {
        pfAudDecNfy((void*)AUD_NFY_STREAM, u1NtyDecId, AUD_COND_CTRL_DONE,
                        (UINT32)eAudDecCmd, u4Result);
    }
#endif
    }

    _AudAsynCmdSemaRelease(u1DecId, eAudDecCmd);
    VERIFY(x_sema_unlock(_hCmdWaitSema[u1DspId][u1DecId]) == OSR_OK);
}

//-----------------------------------------------------------------------------
// AUD_WaitCommandDone
//
/** Audio wait notify command check.
 *
 *  @param  ucDecId         Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eAudDecCmd      audio command.
 *
 *  @retval void
 */
void AUD_WaitCommandDone(UINT8 u1DspId, UINT8 u1DecId, AUD_DEC_CMD_T eAudDecCmd)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    LOG(7, "Aud dec(%d) AUD_WaitCommand <%s> to be done\n", u1DecId , _paAudCmd[eAudDecCmd]);
    VERIFY(x_sema_lock(_hCmdWaitSema[u1DspId][u1DecId], X_SEMA_OPTION_WAIT) == OSR_OK);
}

#ifdef LINUX_TURNKEY_SOLUTION
extern int alsa_card_dummy_init(void);
#endif

void AUD_SetMMAudioOnly(UINT8 u1DecId, BOOL fgMMAudioOnly)
{
    AUD_DrvSetMMAudioOnly(u1DecId, fgMMAudioOnly);
}
#ifdef CC_AUD_DDI
BOOL AUD_GetMMAudioOnly(UINT8 u1DecId)
{
    return AUD_DrvGetMMAudioOnly(u1DecId);
}
#endif

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// AUD_Init
//
/** Aud driver initialization. This function will create driver thread and let
 *  DSP go on.
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
INT32 AUD_Init(void)
{
    UINT32 u4Index = 0;

    FBM_Init();

    if (NULL_HANDLE == _hSemaAudInit)
    {
        VERIFY(x_sema_create(&_hSemaAudInit, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
    }
    VERIFY(x_sema_lock(_hSemaAudInit, X_SEMA_OPTION_WAIT) == OSR_OK);
    if (_fgAudInitOK)
    {
        VERIFY(x_sema_unlock(_hSemaAudInit) == OSR_OK);
        return AUD_OK;
    }

    if (!_fgAudInit)
    {
        VERIFY (x_sema_create(&_hSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DSP0][AUD_DEC_MAIN]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DSP0][AUD_DEC_AUX]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DSP0][AUD_DEC_THIRD]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DSP0][AUD_DEC_4TH]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DSP1][AUD_DEC_MAIN]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DSP1][AUD_DEC_AUX]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DSP1][AUD_DEC_THIRD]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY (x_sema_create(&(_hCmdWaitSema[AUD_DSP1][AUD_DEC_4TH]), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);        

        VERIFY (x_sema_create(&_hAsynCmdSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        VERIFY (x_sema_create(&_hMwCtrlSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    AUD_WRITE32(REG_AOUT_DRAM_CFG, AOUT2_USE_AOUT1_CLK | (0x1F << ADD_DEC2_MIX_POS)); //enable MixSnd2 mix to AOUT1 10 CH + AOUT2 use AOUT1 clock

#ifdef DSP_TEST_PSRDMA
    PARSER_WRITE32(REG_PAR_CTRL, 0);          // Reset parser
    PARSER_WRITE32(REG_PAR_CTRL, 0x3);        // Enable parser
#endif

    AUD_HwInit();
    _fgAudHWInit = TRUE;
    DRVCUST_SendAudEvent(E_CUST_AUD_MCK_STATE_CHANGE, AUD_DEC_MAIN);

#ifdef CC_AUD_USE_NVM
    AUD_NVM_Read();
#endif

#ifdef CC_AUD_USE_FLASH_AQ
    AUD_FlashAQ_Init();
#endif
#ifdef SUPPORT_FLASH_AQ
    #ifndef CC_COMPRESSED_FLASH_AQ_DATA
    AUD_FlashAQInitEx();
    #else
    AUD_FlashAQInitEx_LZMA();
    #endif

#endif

    AUD_DRVInit();
    AUD_DspEqCfgInit();
#ifdef CC_AUD_SOUND_MODE
    AUD_DspPSMCfgInit();
#endif
#ifdef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
    AUD_DspVSMCfgInit();
#endif
    AUD_DspChDelayInit();
    AUD_DspSpdifInit();
    AUD_DspVolumeInit();
    AUD_DspAsrcInit();
    AUD_DspSetCapability();
#ifdef CC_AUD_ARM_RENDER//CC_AUD_APROC_POST 
    //AUD_AprocBMangInit(); /*Aproc Bmang*/
#endif
#ifdef CC_MT5881
    AUD_SpdifinInit();
#endif

    _AudIfFixWarning();

    //FIXME //busy waiting until init finished
    if (!_fgAudInit)
    {
        while (AUD_DRVGetAudioState(AUD_DSP0, AUD_DEC_MAIN) != AUD_IDLE)
        {
            u4Index++;
            x_thread_delay(10);

            if (u4Index > 500)
            {
                LOG(1, "DSP0 NOT POWER ON\n");
                ASSERT(0);
                //  return AUD_FAIL;
            }
        }
#ifndef CC_AUD_DISABLE_2ND_DSP        
        while (AUD_DRVGetAudioState(AUD_DSP1, AUD_DEC_MAIN) != AUD_IDLE)
        {
            u4Index++;
            x_thread_delay(10);

            if (u4Index > 500)
            {
                LOG(1, "DSP1 NOT POWER ON\n");
                ASSERT(0);
                //  return AUD_FAIL;
            }
        }
#endif        

        _fgAudInit = TRUE;
    }

    _fgAudInitOK = TRUE;

    AUD_EnableMixSndClip();
	AUD_ToneGen_MemAlloc();
#ifdef LINUX_TURNKEY_SOLUTION
#if defined (CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    if (u1Clip_buf == NULL)
    {
        u1Clip_buf = (void *)x_mem_alloc(ClipBufferSize);
    if(NULL == u1Clip_buf)
    {
            LOG(0, "allocate memory init for AUD_CLIP_BUFF fail!\n");
    }
    }
#endif
#endif

#ifdef CC_MT5391_AUD_3_DECODER
#ifdef CC_SUPPORT_STR
    if(!fgAudPmStrResume)
#endif 
    {
        AUD_DspChannelMute(AUD_DSP0, AUD_DEC_AUX, AUD_CH_FRONT_LEFT, TRUE);
        AUD_DspChannelMute(AUD_DSP0, AUD_DEC_AUX, AUD_CH_FRONT_RIGHT, TRUE);
    }
#endif

    VERIFY(x_sema_unlock(_hSemaAudInit) == OSR_OK);
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_IsAudInit
//
/** Query audio initialization status.
 *
 *  @retval TRUE    Audio is initialized
 *  @retval FALSE   Audio is not initialized
 */
//-----------------------------------------------------------------------------
BOOL AUD_IsAudInit(void)
{
    return _fgAudInit;
}

BOOL AUD_IsAudInitOK(void)
{
    return _fgAudInitOK;
}

//-----------------------------------------------------------------------------
// AUD_IsAudHWInit
//
/** Query audio HW initialization status. (APLL, I2S setting)
 *
 *  @retval TRUE    Audio HW is initialized
 *  @retval FALSE   Audio HW is not initialized
 */
//-----------------------------------------------------------------------------
BOOL AUD_IsAudHWInit(void)
{
    UNUSED(AUD_IsAudHWInit);
    return _fgAudHWInit;
}

//-----------------------------------------------------------------------------
// AUD_set4kBeMuteDone
//
/** Set 4k BE mute status 
 *
 */
//-----------------------------------------------------------------------------
void AUD_set4kBeMuteDone( BOOL isMute)
{
    UNUSED(AUD_set4kBeMuteDone);
    _fgAud4KBEMuteDone = isMute;
}

//-----------------------------------------------------------------------------
// AUD_get4kBeMuteDone
//
/** Query k BE mute status 
 *
 *  @retval TRUE    Audio 4K be is muted
 *  @retval FALSE   Audio 4K be is in-muted
 */
//-----------------------------------------------------------------------------
BOOL AUD_get4kBeMuteDone(void)
{
    UNUSED(AUD_get4kBeMuteDone);
    return _fgAud4KBEMuteDone;
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// AUD_GetDecType
//
/** Get audio decoder type.
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom   Stream source
 *  @param      prDecType      Stream format
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
INT32 AUD_GetDecType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T* peStreamFrom, AUD_FMT_T* prDecType)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    if ((peStreamFrom != NULL) && (prDecType != NULL))
    {
        *peStreamFrom = AUD_STREAM_FROM_DIGITAL_TUNER;  //FIXME
        *prDecType    = AUD_FMT_AC3;    //FIXME
    }

    return AUD_OK;
}
#endif

//-----------------------------------------------------------------------------
// AUD_SetDecMemBuffer
//
/** Brief of AUD_SetDecMemBuffer.
 *  Details of AUD_SetDecMemBuffer (optional).
 *
 *  @param      u1DecId           Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prDecMemBuffer    Memory buffer
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_SetDecMemBuffer(UINT8 u1DspId, UINT8 u1DecId, const MEM_BUFFER_INFO_T* prDecMemBuffer)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
    if (prDecMemBuffer != NULL)
    {
        VERIFY(AUD_DRVSetDecMemBuffer(u1DspId, u1DecId, prDecMemBuffer));
    }

    return AUD_OK;
}

#ifdef CC_DUAL_AUD_DEC_SUPPORT
//-----------------------------------------------------------------------------
// AUD_Check_Dual_DecId
//
/** Brief of AUD_Check_Dual_DecId.
 *  Details of AUD_Check_Dual_DecId (optional).
 *
 *  To allocate a DecID for Dual decode Source. 
 */
//-----------------------------------------------------------------------------
UINT8 AUD_Check_Dual_DecId(UINT8 u1DecId)
{
    UINT8 u1RetId;

    u1RetId = u1DecId;
    if(u1DecId==AUD_DEC_SUB)                     //If MW wants to start SUB
    {
        if(gSubDecId==AUD_DEC_SUB)               //If SUB is not assigned yet
        {
            if(gMainDecId == AUD_DEC_MAIN)       //Find the ID which main decoder is not occupied.
            {
                u1RetId=AUD_DEC_AUX;
            }
            else 
            {
                u1RetId=AUD_DEC_MAIN;
            }
            gSubDecId = u1RetId;                //Record SubID
        }
        else
        {
            u1RetId = gSubDecId;                //SubID already assigned, just return it back.
        }                
    }
    else                                        //If MW wants to start MAIN
    {
        if (gSubDecId != AUD_DEC_SUB)           //If SUB wss already assigned 
        {
            if(gSubDecId == AUD_DEC_MAIN)       // Find the ID which sub decoder is not occuied.
            {
                u1RetId = AUD_DEC_AUX;
            }
            else if (gSubDecId==AUD_DEC_AUX)
            {
                u1RetId = AUD_DEC_MAIN;
            }
        }
        gMainDecId = u1RetId;  
    }

    LOG(5, "(Dual)_Check_Dual_DecId %d->%d, m=%d,s=%d\n", u1DecId, u1RetId, gMainDecId, gSubDecId);
    return u1RetId;
}
#else
UINT8 AUD_Check_Dual_DecId(UINT8 u1DecId)
{
    UNUSED(AUD_Check_Dual_DecId);
    return u1DecId;
}

#endif

//-----------------------------------------------------------------------------
// AUD_SetDecType
//
/** Set audio decoder type.
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      eStreamFrom    Stream source
 *  @param      eDecFmt        Stream format
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
INT32 AUD_SetDecType(UINT8 u1DspId, UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    AudDecNfyFct   pfAudDecNfy = NULL;
    INT32 i4Ret;
    AUD_DEC_STREAM_FROM_T eFrom;
    AUD_FMT_T eFmt;
    UINT8 u1OriMainID;

    UNUSED(u1OriMainID);    
    LOG(1, "CMD: set DecType: Dsp(%d) AUD_DEC%d Input(%s) Codec(%s)\n", u1DspId, u1DecId, 
        AUD_EnumToName(eAudStreamFromTbl, AUD_ARRAY_SIZE(eAudStreamFromTbl), eStreamFrom), 
        AUD_EnumToName(eAudDrvFmtTbl, AUD_ARRAY_SIZE(eAudDrvFmtTbl), eDecFmt));

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);

#ifdef CC_DUAL_AUD_DEC_SUPPORT
    if(u1DecId==AUD_DEC_SUB)        
    {
        // This is simple case, MW try to start SUB, call function to assign an ID for it.
        u1DecId = AUD_Check_Dual_DecId(u1DecId);      
        bSkipDSPCmdLst[u1DspId][u1DecId]=FALSE;
    }
    else if(((eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) ||
             (eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)) &&
             (eDecFmt == AUD_FMT_PCM))    
    {        
        //AUD_CommandDone(u1DecId, AUD_CMD_LOADCODE, AUD_OK);
        VERIFY(x_sema_unlock(_hAsynCmdSema) == OSR_OK);
        VERIFY(x_sema_unlock(_hSema) == OSR_OK);
        // This is for MW want to play TV on Dec0_PCM of (Dec0_PCM + Dec1_TV). Just skip it.        
        bSkipDSPCmdLst[u1DspId][u1DecId]=TRUE;
        // For DTV only case, MW_IF play Dec1 TV, than Dec0 TV.
        // We should set main id in Dec1.
        gMainDecId = AUD_DEC_AUX;  
        return AUD_OK;    
    }
    else 
    {
        u1OriMainID = gMainDecId;
        // This is for MW want to play source on MAIN of (MAIN, SUB). Find an ID for MAIN source.
            u1DecId = AUD_Check_Dual_DecId(u1DecId);
        bSkipDSPCmdLst[u1DspId][u1DecId]=FALSE;
        // For HDMI + TV SCART case.
        // MW_IF play HDMI Dec0, then TV Dec1.
        // MainID is changed to Dec1 after check dual DecID.
        // We should restore it back.
        // If it's TV + TV Scart case, main id will be set to Dec1 soon in previous else if case.
        if(((eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||(eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)) 
            &&(u1DecId == AUD_DEC_AUX))
        {
            gMainDecId = u1OriMainID;            
        }
    }
    LOG(5, "(Dual) set DecType: Dec(%d) StreamFrom(%d) Fmt(%d)\n", u1DecId, (UINT8)eStreamFrom, (UINT8)eDecFmt);
        
#endif //"CC_DUAL_AUD_DEC_SUPPORT"    

#ifndef __MODEL_slt__
#ifdef CC_DYNAMIC_POWER_ONOFF
    //ADC Power down/on
#endif
#endif

#if 1 //Use LPCM decoder if enable this code.
    if ((eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) && (AUD_GetMultimediaMode(AUD_DEC_MAIN) != AUD_MM_MODE_GAME))
    {
#ifndef CC_AUD_SKYPE_SUPPORT
        if (eDecFmt == AUD_FMT_PCM)
#else
        if ((eDecFmt == AUD_FMT_PCM) && (u1DecId == AUD_DEC_MAIN))
#endif
        {
            eDecFmt = AUD_FMT_LPCM;
        }
    }
#endif

#ifdef USE_16BIT_ALIGNMENT
    if ((eStreamFrom == AUD_STREAM_FROM_LINE_IN) ||
        (eStreamFrom == AUD_STREAM_FROM_HDMI) || 
        (eStreamFrom == AUD_STREAM_FROM_LINE_IN_2) ||
        (eStreamFrom == AUD_STREAM_FROM_LINE_IN_3)
#ifdef CC_AUD_I2S_IN
        || (eStreamFrom == AUD_STREAM_FROM_I2S)
#endif
        )
    {
        AUD_PCM_SETTING_T  ePcmSetting;
        //gallen 20140428, use 52k for Oryx internal ADC 46.875k usage
        if ((eStreamFrom == AUD_STREAM_FROM_LINE_IN) || 
            (eStreamFrom == AUD_STREAM_FROM_LINE_IN_2) || 
            (eStreamFrom == AUD_STREAM_FROM_LINE_IN_3))
        {
            #if defined(CC_MT5890)
                ePcmSetting.eSampleFreq = FS_52K;
            #else
                ePcmSetting.eSampleFreq = FS_48K;
            #endif
        }
        else
        {
            ePcmSetting.eSampleFreq = FS_48K;
        }       
        ePcmSetting.ePcmDataInfo = PCM_16_BITS_BIG_ENDIAN;
    #ifdef CC_HDMI_PCM_MULT_CHANNEL
        ///Set HDMI Audio Channel Number for Dsp
        if(eStreamFrom == AUD_STREAM_FROM_HDMI)
        {
            ePcmSetting.ePCMChNum = API_HDMI_Get_Aud_ChannelNum()+1;
        }
        else
        {
            ePcmSetting.ePCMChNum = 2;
        }
        AUD_LineInChNumSetting(u1DecId, ePcmSetting.ePCMChNum);
    #endif
        AUD_PcmSetting(u1DspId, u1DecId, &ePcmSetting);
    }
#endif

    // notify
    AUD_GetDecodeType(u1DspId, u1DecId, &eFrom, &eFmt);
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));
    if ((eStreamFrom != eFrom) || (eDecFmt != eFmt))
    {
        if (pfAudDecNfy != NULL)
        {
            pfAudDecNfy((void*)AUD_NFY_STREAM, u1DecId, AUD_COND_STREAM_FORMAT_CHG,
                        (UINT32)eStreamFrom, (UINT32)eDecFmt);  
        }
    }
    
	if (eStreamFrom == AUD_STREAM_FROM_HDMI)
	{
	    if (eDecFmt == AUD_FMT_DTS)
        {
            AUD_DspSpeakerOutputConfig(u1DecId, 2);//DTS fixed as Lo/Ro    
        }
        else if (eDecFmt == AUD_FMT_AC3)
        {
            AUD_DspSpeakerOutputConfig(u1DecId, 0);//DD/DDP fixed as Lt/Rt    
        }
        else
        {
            AUD_DspSpeakerOutputConfig(u1DecId, 0);//default as Lt/Rt    
        }
	}

    if (!AUD_IsDecoderRealPlay(u1DspId, u1DecId))
    {
        // Try lock to make sure the semaphore was locked
        i4Ret = x_sema_lock(_hCmdWaitSema[u1DspId][u1DecId], X_SEMA_OPTION_NOWAIT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

        VERIFY(AUD_DRVSetDecodeType(u1DspId, u1DecId, eStreamFrom, eDecFmt));
        AUD_WaitCommandDone(u1DspId, u1DecId, AUD_CMD_LOADCODE);
    }
    else
    {
        // Try lock to make sure the semaphore was locked
        i4Ret = x_sema_lock(_hCmdWaitSema[u1DspId][u1DecId], X_SEMA_OPTION_NOWAIT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

        UNUSED(AUD_DRVChangeDecodeFormat(u1DspId, u1DecId, eStreamFrom, eDecFmt));
        AUD_WaitCommandDone(u1DspId, u1DecId, AUD_CMD_CHANGE_FORMAT);
    }

    VERIFY(x_sema_unlock(_hSema) == OSR_OK);

    return AUD_OK;
}

INT32 AUD_SetSourceFrom(UINT8 u1DecId,  AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    LOG(5, "CMD: set SourceFrom: Dec(%d) StreamFrom(%d)\n",
        u1DecId, (UINT8)eStreamFrom );

    AUD_DEC_ID_VALIDATE(u1DecId);
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    AUD_DRVSetStreamFrom(AUD_DSP0, u1DecId, eStreamFrom);
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    return AUD_OK;
}

#ifdef CC_AUD_DDI
UINT8 AUD_GetAudDtvId(void)
{
    return _DTVFifoId;
}

void AUD_SetDmxId(UINT8 u1DecId, UINT8 u1StcId)
{

    UINT32 pu4FifoStart1; 
    UINT32 pu4FifoEnd1;
    UINT32 pu4FifoStart2; 
    UINT32 pu4FifoEnd2;
    UNUSED(u1DecId);
    AUD_GetAudFifo(AUD_DEC_AUX, &pu4FifoStart1,  &pu4FifoEnd1);
    AUD_GetAudFifo(AUD_DEC_THIRD, &pu4FifoStart2,  &pu4FifoEnd2);
    //if(u1StcId==_DTVFifoId)
    /*
    {
        if(u1StcId==AUD_DEC_AUX)   
        {
            DMX_ChangeBufPointer(u1StcId,pu4FifoStart1,pu4FifoEnd1,pu4FifoStart1,pu4FifoEnd1);
        }
        else 
        {
            DMX_ChangeBufPointer(u1StcId,pu4FifoStart2,pu4FifoEnd2,pu4FifoStart2,pu4FifoEnd2);  
        }
    }
    */
    //else
    {
        if(u1StcId==AUD_DEC_AUX)   
        {
            DMX_ChangeBufPointer(u1StcId,pu4FifoStart2,pu4FifoEnd2,pu4FifoStart1,pu4FifoEnd1);
        }
        else
        {
            DMX_ChangeBufPointer(u1StcId,pu4FifoStart1,pu4FifoEnd1,pu4FifoStart2,pu4FifoEnd2);  
        }
        
    }

    _DTVFifoId=u1StcId;

    
}
#endif

#ifdef CC_AUD_DDI
UINT8 AUD_GetGstId(void)
{
       return _GstreamerId;   
}

void AUD_SetGstId(UINT8 u1DecId)
{
    _GstreamerId=u1DecId;   
}

void AUD_SetGstEncFlag(BOOL u1EncFlag)
{
    _u1GstENCFlag=u1EncFlag;
}

UINT8 AUD_GetGstEncFlag(void)
{
    return _u1GstENCFlag;
}
#endif

#ifdef CC_AUD_DDI
UINT8 AUD_GetResource(AUD_DEC_STREAM_FROM_T eStreamFrom, BOOL fgIsAD, BOOL fgForRec)
{
    UINT8 u1DecId, u1Ret;
    AUD_ENC_MODE_T eEncMode;
    AUD_DEC_STREAM_FROM_T eStreamFromCur1, eStreamFromCur2;

    //clear AD record
    AUD_DRVSetADType(AUD_DEC_THIRD,FALSE);
    
    AUD_GetStreamFrom(AUD_DSP0, AUD_DEC_AUX, &eStreamFromCur1);
    AUD_GetStreamFrom(AUD_DSP0, AUD_DEC_THIRD, &eStreamFromCur2);
    DSP_GetEncMode(&u1DecId, &eEncMode);

    if(AUD_STREAM_FROM_DIGITAL_TUNER == eStreamFrom)
    {
        if (fgIsAD)
        {
            AUD_DRVSetADType(AUD_DEC_THIRD,TRUE);
            u1Ret = AUD_DEC_THIRD;
        }
        else
        {
            if((AUD_DRVCheckDecAvailable(AUD_DEC_AUX, fgForRec) && ((!fgForRec) || ((eStreamFromCur1 != AUD_STREAM_FROM_DIGITAL_TUNER) || (AUD_DEC_AUX == u1DecId)))) || 
                ((!fgForRec) && ((AUD_DEC_AUX != u1DecId) || (AUD_ENC_MODE_OFF == eEncMode))))
            {
                u1Ret = AUD_DEC_AUX;
            }
            else if(AUD_DRVCheckDecAvailable(AUD_DEC_THIRD, fgForRec))
            {
                u1Ret = AUD_DEC_THIRD;
            }
            else
            {
                if(AUD_DRVCheckDecRecordFlag(AUD_DEC_THIRD))
                {
                    u1Ret = AUD_DEC_AUX;
                }
                else if(AUD_DRVCheckDecRecordFlag(AUD_DEC_AUX))
                {
                    u1Ret = AUD_DEC_THIRD;
                }
                else
                {
                    LOG(0, "[AUD_GetResource] Resouse is shortage, use the default DecId for DTV: AUD_DEC_AUX.\n");
                    u1Ret = AUD_DEC_AUX;
                }
            }
        }
    }

    if(AUD_STREAM_FROM_ANALOG_TUNER == eStreamFrom)
    {
        if((!AUD_DRVCheckDecAvailable(AUD_DEC_AUX, fgForRec)) && (AUD_STREAM_FROM_ANALOG_TUNER == eStreamFromCur1))
        {
            u1Ret = AUD_DEC_AUX;
        }
        else if((!AUD_DRVCheckDecAvailable(AUD_DEC_THIRD, fgForRec)) && (AUD_STREAM_FROM_ANALOG_TUNER == eStreamFromCur2))
        {
            u1Ret = AUD_DEC_THIRD;
        }
        else
        {
            if(AUD_DRVCheckDecAvailable(AUD_DEC_AUX, fgForRec) ||
                ((!fgForRec) && ((AUD_DEC_AUX != u1DecId) || (AUD_ENC_MODE_OFF == eEncMode))))
            {
                u1Ret = AUD_DEC_AUX;
            }
            else if(AUD_DRVCheckDecAvailable(AUD_DEC_THIRD, fgForRec))
            {
                u1Ret = AUD_DEC_THIRD;
            }
            else
            {
                if(AUD_DRVCheckDecRecordFlag(AUD_DEC_THIRD))
                {
                    u1Ret = AUD_DEC_AUX;
                }
                else if(AUD_DRVCheckDecRecordFlag(AUD_DEC_AUX))
                {
                    u1Ret = AUD_DEC_THIRD;
                }
                else
                {
                    LOG(0, "[AUD_GetResource] Resouse is shortage, use the default DecId for ATV: AUD_DEC_AUX.\n");
                    u1Ret = AUD_DEC_AUX;
                }
            }
        }
    }

    #ifdef CC_ENABLE_AOMX 
    if(AUD_STREAM_FROM_GST == eStreamFrom)
    {
        if (AUD_DRVCheckDecAvailable(AUD_DEC_THIRD, fgForRec))
        {
            u1Ret = AUD_DEC_THIRD;
        }
        else if(AUD_DRVCheckDecAvailable(AUD_DEC_AUX, fgForRec))
        {
            u1Ret = AUD_DEC_AUX;
        }
        else
        {
            if(AUD_DRVCheckDecRecordFlag(AUD_DEC_THIRD))
            {
                u1Ret = AUD_DEC_AUX;
            }
            else if(AUD_DRVCheckDecRecordFlag(AUD_DEC_AUX))
            {
                u1Ret = AUD_DEC_THIRD;
            }
            else
            {
                LOG(0, "[AUD_GetResource] Resouse is shortage, use the default DecId for GST: AUD_DEC_THIRD.\n");
                u1Ret = AUD_DEC_THIRD;
            }
        }
    }
    #endif
    
    if (eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        u1Ret = AUD_DEC_MAIN;
    }

    if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
    {
        if((!AUD_DRVCheckDecAvailable(AUD_DEC_AUX, fgForRec)) && (AUD_STREAM_FROM_LINE_IN == eStreamFromCur1))
        {
            u1Ret = AUD_DEC_AUX;
        }
        else if((!AUD_DRVCheckDecAvailable(AUD_DEC_THIRD, fgForRec)) && (AUD_STREAM_FROM_LINE_IN == eStreamFromCur2))
        {
            return (AUD_DEC_THIRD);
        }
        else
        {           
            if (AUD_DRVCheckDecAvailable(AUD_DEC_THIRD, fgForRec))
            {
                u1Ret = AUD_DEC_THIRD;
            }
            else if(AUD_DRVCheckDecAvailable(AUD_DEC_AUX, fgForRec))
            {
                u1Ret = AUD_DEC_AUX;
            }
            else
            {
                if(AUD_DRVCheckDecRecordFlag(AUD_DEC_AUX))
                {
                    u1Ret = AUD_DEC_THIRD;
                }
                else if(AUD_DRVCheckDecRecordFlag(AUD_DEC_THIRD))
                {
                    u1Ret = AUD_DEC_AUX;
                }
                else
                {
                    LOG(0, "[AUD_GetResource] Resouse is shortage, use the default DecId for Line-In: AUD_DEC_THIRD.\n");
                    u1Ret = AUD_DEC_THIRD;
                }
            }
        }
    }

    if(AUD_DEC_AUX == u1Ret)
    {
        AUD_DRVSetAudPath(AUD_DEC_AUX, (fgForRec ? AUD_PATH_SUB : AUD_PATH_MAIN));
        AUD_DRVSetAudPath(AUD_DEC_THIRD, (fgForRec ? AUD_PATH_MAIN : AUD_PATH_SUB));
    }
    else
    {
        AUD_DRVSetAudPath(AUD_DEC_THIRD, (fgForRec ? AUD_PATH_SUB : AUD_PATH_MAIN));
        AUD_DRVSetAudPath(AUD_DEC_AUX, (fgForRec ? AUD_PATH_MAIN : AUD_PATH_SUB));
    }
    return u1Ret;
    
#if 0   
    if (eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        u1DecId = AUD_DEC_MAIN;
        return (u1DecId);
    }
    else if ((eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) || (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
    {
        u1DecId = AUD_DEC_AUX;
        if ((fgIsAD) && (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
        {
            u1DecId = AUD_DEC_THIRD;
            AUD_DRVSetADType(AUD_DEC_THIRD,TRUE);
        }
        else if (!AUD_DRVCheckDecAvailable(u1DecId))
        {
            u1DecId = AUD_DEC_THIRD;
        }    
    }
#ifdef CC_ENABLE_AOMX    
    else if ((eStreamFrom == AUD_STREAM_FROM_GST) || (eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) || (eStreamFrom == AUD_STREAM_FROM_LINE_IN))
#else
    else if ((eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) || (eStreamFrom == AUD_STREAM_FROM_LINE_IN))
#endif
    {
        u1DecId = AUD_DEC_THIRD;
        if (!AUD_DRVCheckDecAvailable(u1DecId))
        {
            u1DecId = AUD_DEC_AUX;
        }    
    }
    else
    {
        u1DecId = AUD_DEC_MAIN;
    }
    //final check
    if (eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
    {
        //for ATV, must prevent another getrecource when ATV is playing already => return the same DecId
        AUD_DEC_STREAM_FROM_T eStreamFromCur;
        AUD_GetStreamFrom(AUD_DEC_AUX, &eStreamFromCur);
        if (AUD_IsDecoderRealPlay(AUD_DEC_AUX) && (eStreamFromCur == AUD_STREAM_FROM_ANALOG_TUNER))
        {
            return (AUD_DEC_AUX);
        }
        else
        {
            AUD_GetStreamFrom(AUD_DEC_THIRD, &eStreamFromCur);
            if (AUD_IsDecoderRealPlay(AUD_DEC_THIRD) && (eStreamFromCur == AUD_STREAM_FROM_ANALOG_TUNER))
            {
                return (AUD_DEC_THIRD);
            }
        }
    }

    if (!AUD_DRVCheckDecAvailable(u1DecId))
    {
        //should not happen, use default ID
        if ((eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) || (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
        {
            u1DecId = AUD_DEC_AUX;
        }
#ifdef CC_ENABLE_AOMX    
        else if ((eStreamFrom == AUD_STREAM_FROM_GST) || (eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) || (eStreamFrom == AUD_STREAM_FROM_LINE_IN))
#else
        else if ((eStreamFrom == AUD_STREAM_FROM_MULTI_MEDIA) || (eStreamFrom == AUD_STREAM_FROM_LINE_IN))
#endif
        {
            u1DecId = AUD_DEC_THIRD;
        }
        else
        {
            //MM and HDMI
            u1DecId = AUD_DEC_MAIN;
        }
        LOG(0,"===Cannot find available RESOURCES!!!!!\n");
    }
    return (u1DecId);
#endif
}
#endif

#if defined(CC_AUD_APOLLO_SUPPORT) || defined(CC_AUD_ARM_SUPPORT)
extern BOOL bIsSupportAproc (void);
#endif
//-----------------------------------------------------------------------------
// AUD_DSPCmdPlay
//
/** Brief of AUD_DSPCmdPlay.
 *  Issue play command to audio decoder, note that this is a synchronous play
 *  function
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdPlay(UINT8 u1DspId, UINT8 u1DecId)
{
    INT32 i4Ret;
    BOOL bSkipDSPCmd = FALSE;    

#ifdef CC_AUD_ARM_RENDER    
#ifdef CC_DUAL_AUD_DEC_SUPPORT   
    INT32 isSub=0;
    bSkipDSPCmd = AUD_Aproc_Chk_Cmd(u1DecId);
    if(u1DecId==AUD_DEC_SUB)
    {
        isSub = 1;
        u1DecId = gSubDecId;
    }
#endif
#endif    

    LOG(2, "CMD: set Play: Dsp(%d) AUD_DEC%d\n", u1DspId, u1DecId);

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_PLAY_START);

    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);

#ifdef CC_AUD_ARM_SUPPORT
#ifdef CC_AUD_ARM_RENDER
#ifndef CC_DUAL_AUD_DEC_SUPPORT
    bSkipDSPCmd = AUD_Aproc_Chk_Cmd(u1DecId);
#endif
	
    if (!bSkipDSPCmd)
    {
		vAprocDec_Open (AudDecId2MixerId(u1DecId));
        x_thread_delay(30);
        _AudAprocInputMute(u1DecId, FALSE);
    }
    LOG(5, "(Dual) CMD: set Play: Dec(%d), skip=%d\n", u1DecId, bSkipDSPCmd);
#else
    if ((u1DecId == AUD_DEC_MAIN)&&(u1DspId == AUD_DSP0))
    {
        vAprocPostProc_Open ();
    }
#endif
#endif

#ifdef CC_MT5391_AUD_3_DECODER
#ifndef CC_AUD_DDI    //For DDI APP will enable/disable AD
    if ((u1DecId == AUD_DEC_THIRD) && (!bSkipDSPCmd))
    {
        AUD_DspADEnable(AUD_DEC_MAIN, TRUE);
    }
#endif    
#endif

    if(bSkipDSPCmd)
    {
#ifdef CC_DUAL_AUD_DEC_SUPPORT          
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PLAY_SKIP, AUD_OK);
#else
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PLAY, AUD_OK);
#endif
#ifdef CC_AUD_ARM_RENDER
		if (AUD_IsDecoderRealPlay(AUD_DSP0, AUD_DEC_AUX))
			AUD_Aproc_ChkTVDecInfoChgNfy(u1DspId, u1DecId);
#endif
    }
    else if (_afgDecPause[u1DspId][u1DecId])
    {
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PLAY, AUD_OK);
    }
    else
    {
        // Try lock to make sure the semaphore was locked
        i4Ret = x_sema_lock(_hCmdWaitSema[u1DspId][u1DecId], X_SEMA_OPTION_NOWAIT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
        VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_PLAY));
    }
    _afgDecPause[u1DspId][u1DecId] = FALSE;
#ifdef CC_AUD_ARM_RENDER
	_aeAudDecCmdState[AUD_DSP0][u1DecId] = AUD_CMD_ST_PLAY;
#endif
    AUD_WaitCommandDone(u1DspId, u1DecId, AUD_CMD_PLAY);
#ifdef CC_AUD_ARM_RENDER 
#ifdef CC_DUAL_AUD_DEC_SUPPORT    
    if(isSub)
    {
        _AudAprocInputMute(u1DecId, FALSE);
    }
#endif    
#endif    
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);

    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdPlayAsyn
//
/** Brief of AUD_DSPCmdPlayAsyn.
 *  Issue play command to audio decoder, note that this is a asynchronous play
 *  function. There will be a audio decoder notification when the command was done
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdPlayAsyn(UINT8 u1DspId, UINT8 u1DecId)
{
    BOOL bSkipDSPCmd = FALSE;

#ifdef CC_AUD_ARM_RENDER    
#ifdef CC_DUAL_AUD_DEC_SUPPORT    
    UINT32 isSub=0;
    bSkipDSPCmd = AUD_Aproc_Chk_Cmd(u1DecId);    
    if(u1DecId==AUD_DEC_SUB)
    {
        isSub = 1;
        u1DecId = gSubDecId;
    }
#endif
#endif

    LOG(5, "CMD: set Play Asyn: Dsp(%d) Dec(%d)\n", u1DspId, u1DecId);

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

#ifdef CHANNEL_CHANGE_LOG
    HAL_TIME_T dt;
    HAL_GetTime(&dt);
    LOG(0, " %u.%06u s [AV SYNC] 3 MW aud play\n", dt.u4Seconds, dt.u4Micros);
#endif

    MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_PLAY_START);
    
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);

#ifdef CC_AUD_ARM_SUPPORT
#ifdef CC_AUD_ARM_RENDER
#ifndef CC_DUAL_AUD_DEC_SUPPORT
    bSkipDSPCmd = AUD_Aproc_Chk_Cmd(u1DecId);
#endif

    if (!bSkipDSPCmd)
    {
		vAprocDec_Open (AudDecId2MixerId(u1DecId));
        _AudAprocInputMute(u1DecId, FALSE);	
    }
    LOG(5, "(Dual) CMD: set Play: Dec(%d), skip=%d\n", u1DecId, bSkipDSPCmd);    
#else
    if ((u1DecId == AUD_DEC_MAIN)&&(u1DspId == AUD_DSP0))
    {
        vAprocPostProc_Open (); // fix me!!!!, neeed to opt.?
    }
#endif
#endif

#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521
    if ((u1DecId == AUD_DEC_THIRD) && (!bSkipDSPCmd))
    {
        AUD_DspADEnable(AUD_DEC_MAIN, TRUE);
    }
#endif
    if(bSkipDSPCmd)
    {
#ifdef CC_DUAL_AUD_DEC_SUPPORT          
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PLAY_SKIP, AUD_OK);

#else   
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PLAY, AUD_OK);
#endif
#ifdef CC_AUD_ARM_RENDER
		if (AUD_IsDecoderRealPlay(AUD_DSP0, AUD_DEC_AUX))
			AUD_Aproc_ChkTVDecInfoChgNfy(u1DspId, u1DecId);
#endif
    }
    else if (_afgDecPause[u1DspId][u1DecId])
    {
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PLAY, AUD_OK);
    }
    else
    {
        VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_PLAY));
    }
    _afgDecPause[u1DspId][u1DecId] = FALSE;
	
#ifdef CC_AUD_ARM_RENDER
	_aeAudDecCmdState[AUD_DSP0][u1DecId] = AUD_CMD_ST_PLAY;
#endif

#ifdef CC_AUD_ARM_RENDER
#ifdef CC_DUAL_AUD_DEC_SUPPORT    
    if(isSub)
    {
        _AudAprocInputMute(u1DecId, FALSE);
    }
#endif
#endif
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdResume
//
/** Brief of AUD_DSPCmdResume.
 *  Details of AUD_DSPCmdResume (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdResume(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DEC_STREAM_FROM_T eStreamFrom = AUD_STREAM_FROM_OTHERS;
    AUD_GetStreamFrom(AUD_DSP0, u1DecId, &eStreamFrom);

    LOG(2, "CMD: set Resume: Dsp(%d) AUD_DEC%d\n", u1DspId, u1DecId);

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_RESUME_START);
    
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (AUD_IsDecoderPlay(u1DspId, u1DecId))
    {
        _afgDecPause[u1DspId][u1DecId] = FALSE;
        VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_RESUME));
#ifdef CC_ENABLE_AOMX
        if ((eStreamFrom != AUD_STREAM_FROM_GST) || !AUD_GetMMAudioOnly(u1DecId))
        {
            _AudAprocInputMute(u1DecId, FALSE); 
        }
#endif        
    }
    else
    {
        VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_PLAY));
    }
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);

    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdPause
//
/** Brief of AUD_DSPCmdPause.
 *  Details of AUD_DSPCmdPause (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdPause(UINT8 u1DspId, UINT8 u1DecId)
{
    LOG(2, "CMD: set Pause: Dsp(%d) AUD_DEC%d\n", u1DspId, u1DecId);

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_PAUSE_START);

    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    if (AUD_IsDecoderPlay(u1DspId, u1DecId))
    {
        _afgDecPause[u1DspId][u1DecId] = TRUE;
#ifdef CC_ENABLE_AOMX        
        _AudAprocInputMute(u1DecId, TRUE);
        x_thread_delay(40);
#endif        
    }
    VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_PAUSE));

    VERIFY(x_sema_unlock(_hSema) == OSR_OK);

    return AUD_OK;
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// AUD_DSPCmdPauseAsyn
//
/** Brief of AUD_DSPCmdPauseAsyn.
 *  Details of AUD_DSPCmdPauseAsyn (optional).
 *
 *  @param[in]        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdPauseAsyn(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_PAUSE_START);
    
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    _afgDecPause[u1DspId][u1DecId] = TRUE;
    AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_PAUSE, AUD_OK);
    //VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_STOP));
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);

    return AUD_OK;
}
#endif

//-----------------------------------------------------------------------------
// AUD_DSPCmdStop
//
/** Brief of AUD_DSPCmdStop.
 *  Details of AUD_DSPCmdStop (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdStop(UINT8 u1DspId, UINT8 u1DecId)
{
    INT32 i4Ret;

    BOOL bSkipDSPCmd = FALSE;
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER) 
#ifdef CC_DUAL_AUD_DEC_SUPPORT   
    INT32 isSub=0;    
    bSkipDSPCmd = AUD_Aproc_Chk_Cmd(u1DecId);        
    if(u1DecId==AUD_DEC_SUB)
    {
        isSub = 1;
        u1DecId = gSubDecId;
    }
	
	if (AUD_Aproc_ChkAVPVR(u1DecId) == 1)
	{
		LOG(5, "[Debug] Skip Dec0 stop command when AV PVR.\n");
		bSkipDSPCmd = TRUE;
	}
	else if (AUD_Aproc_ChkAVPVR(u1DecId) == 2)
	{
		LOG(5, "[Debug] Keep Dec0 stop command when AV PVR.\n");
		bSkipDSPCmd = FALSE;
	}
#endif    
#endif

    LOG(2, "CMD: set Stop: Dsp(%d) AUD_DEC%d\n", u1DspId, u1DecId);

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_STOP_START);

    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);

#ifndef CC_DUAL_AUD_DEC_SUPPORT
#ifdef CC_AUD_ARM_RENDER
    bSkipDSPCmd = AUD_Aproc_Chk_Cmd(u1DecId);
#endif
#endif

#ifdef CC_AUD_ARM_SUPPORT
#ifdef CC_AUD_ARM_RENDER
#ifdef CC_DUAL_AUD_DEC_SUPPORT       
    if(isSub)
    {
        gSubDecId=AUD_DEC_SUB;
    }
    else if(!bSkipDSPCmd)
    {
        _AudAprocInputMute(u1DecId, TRUE);
        x_thread_delay(30);
    }
#else
    if (!bSkipDSPCmd)
    {
        _AudAprocInputMute(u1DecId, TRUE);
        x_thread_delay(30);
    }
#endif //CC_DUAL_AUD_DEC_SUPPORT
	_aeAudDecCmdState[AUD_DSP0][u1DecId] = AUD_CMD_ST_STOP;
#else

#endif
#endif

#ifndef CC_AUD_FAST_STOP_BY_HW_GAIN
    // Try lock to make sure the semaphore was locked
    if (bSkipDSPCmd)
    {        
#ifdef CC_DUAL_AUD_DEC_SUPPORT    
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_STOP_SKIP, AUD_OK);
#else
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_STOP, AUD_OK);
#endif
    }
    else
    {    
        i4Ret = x_sema_lock(_hCmdWaitSema[u1DspId][u1DecId], X_SEMA_OPTION_NOWAIT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

        VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_STOP));
        AUD_WaitCommandDone(u1DspId, u1DecId, AUD_CMD_STOP);
    }    
#else
    if (!AUD_IsDecoderStop(u1DspId, u1DecId))
    {
        AudAoutGainCtrl(u1DecId, 0, 0, AOUT_VOL_SETP_4DB);
        x_thread_delay(80 / AOUT_VOL_SETP_4DB);   //wait fadeout done, must be fadeout bank length
        i4Ret = x_sema_lock(_hCmdWaitSema[u1DspId][u1DecId], X_SEMA_OPTION_NOWAIT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

        VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_STOP));
        AUD_WaitCommandDone(u1DspId, u1DecId, AUD_CMD_STOP);
    }
    else
    {
        LOG(5, "SKIP AUD_DSPCmdStop!!\n");
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_STOP, AUD_OK);
    }
#endif
#ifdef CC_MT5391_AUD_3_DECODER
#ifndef CC_AUD_DDI    //For DDI APP will enable/disable AD
    if ((u1DecId == AUD_DEC_THIRD) && (!bSkipDSPCmd))
    {
#ifndef CC_AUD_DDI
        AUD_DspADEnable(AUD_DEC_MAIN, FALSE);
#endif
    }
#endif    
#endif

#ifdef CC_AUD_DDI
    AUD_DRVSetEncodeType(AUD_DEC_MAIN,FALSE);       //clear fgRecord for all decoders
#endif

#ifdef CC_AUD_ARM_SUPPORT
#ifdef CC_AUD_ARM_RENDER
#ifdef CC_DUAL_AUD_DEC_SUPPORT       
		if(!bSkipDSPCmd)
		{
			vAprocDec_Close (AudDecId2MixerId(u1DecId));
		}	 
		LOG(5, "(Dual) CMD: set Stop: Dec(%d), skip=%d\n", u1DecId, bSkipDSPCmd);	 
#else
		if (!bSkipDSPCmd)
		{
			vAprocDec_Close (AudDecId2MixerId(u1DecId));
		}
#endif //CC_DUAL_AUD_DEC_SUPPORT
#else
		if ((u1DecId == AUD_DEC_MAIN)&&(u1DspId == AUD_DSP0))
		{
			vAprocPostProc_Close ();
		}
#endif
#endif

    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    _afgDecPause[u1DspId][u1DecId] =  FALSE;

    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_DSPCmdStopAsyn
//
/** Brief of AUD_DSPCmdStopAsyn.
 *  Details of AUD_DSPCmdStopAsyn (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdStopAsyn(UINT8 u1DspId, UINT8 u1DecId)
{

    BOOL bSkipDSPCmd = FALSE;
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER) 
#ifdef CC_DUAL_AUD_DEC_SUPPORT       
    INT32 isSub=0;
    bSkipDSPCmd = AUD_Aproc_Chk_Cmd(u1DecId);        
    if(u1DecId==AUD_DEC_SUB)
    {
        isSub = 1;
        u1DecId = gSubDecId;
    }
	
	if (AUD_Aproc_ChkAVPVR(u1DecId) == 1)
	{
		LOG(5, "[Debug] Skip Dec0 stop command when AV PVR.\n");
		bSkipDSPCmd = TRUE;
	}
	else if (AUD_Aproc_ChkAVPVR(u1DecId) == 2)
	{
		LOG(5, "[Debug] Keep Dec0 stop command when AV PVR.\n");
		bSkipDSPCmd = FALSE;
	}
#endif
#endif
    LOG(5, "CMD: set Stop Asyn: Dsp(%d) Dec(%d)\n", u1DspId, u1DecId);

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    MMPlaybackSpeedTestAddTimeStamp(TIME_STAMP_AUD_STOP_START);
    
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    
#ifndef CC_DUAL_AUD_DEC_SUPPORT
#ifdef CC_AUD_ARM_RENDER
        bSkipDSPCmd = AUD_Aproc_Chk_Cmd(u1DecId);
#endif
#endif

#ifdef CC_AUD_ARM_SUPPORT
#ifdef CC_AUD_ARM_RENDER
#ifdef CC_DUAL_AUD_DEC_SUPPORT   
    if(isSub)
    {
        gSubDecId=AUD_DEC_SUB;
    }
    else if(!bSkipDSPCmd)
    {
        _AudAprocInputMute(u1DecId, TRUE);
        x_thread_delay(30);
    }
    if(!bSkipDSPCmd)
    {
        vAprocDec_Close (AudDecId2MixerId(u1DecId));
    }
#else
    if (!bSkipDSPCmd)
    {
        _AudAprocInputMute(u1DecId, TRUE);
        x_thread_delay(30); 
        vAprocDec_Close (AudDecId2MixerId(u1DecId));
    }
#endif  //CC_DUAL_AUD_DEC_SUPPORT
	_aeAudDecCmdState[AUD_DSP0][u1DecId] = AUD_CMD_ST_STOP;
#else
    if ((u1DecId == AUD_DEC_MAIN)&&(u1DspId == AUD_DSP0))
    {
        vAprocPostProc_Close (); // fix me!!!!, need to opt.?
    }
#endif
#endif


#ifndef CC_AUD_FAST_STOP_BY_HW_GAIN
    if (bSkipDSPCmd)
    {    
#ifdef CC_DUAL_AUD_DEC_SUPPORT           
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_STOP_SKIP, AUD_OK);
#else
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_STOP, AUD_OK);
#endif
    }
    else
    {
        VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_STOP));
    }    
#else
    if (!AUD_IsDecoderStop(u1DspId, u1DecId))
    {
        AudAoutGainCtrl(u1DecId, 0, 0, AOUT_VOL_SETP_4DB);
        x_thread_delay(80 / AOUT_VOL_SETP_4DB);   //wait fadeout done, must be fadeout bank length
        VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_STOP));
    }
    else
    {
        LOG(5, "SKIP AUD_DSPCmdStopAsyn!!\n");
        AUD_CommandDone(u1DspId, u1DecId, AUD_CMD_STOP, AUD_OK);
    }
#endif
#ifdef CC_MT5391_AUD_3_DECODER  //gallen 0521
    if ((u1DecId == AUD_DEC_THIRD) && (!bSkipDSPCmd))
    {
        AUD_DspADEnable(AUD_DEC_MAIN, FALSE);
    }
#endif
#ifdef CC_AUD_DDI
    AUD_DRVSetEncodeType(AUD_DEC_MAIN,FALSE);       //clear fgRecord for all decoders
#endif

    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    _afgDecPause[u1DspId][u1DecId] =  FALSE;

    return AUD_OK;
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// AUD_DSPCmdReset
//
/** Brief of AUD_DSPCmdReset.
 *  Details of AUD_DSPCmdReset (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdReset(UINT8 u1DspId, UINT8 u1DecId)
{
    INT32 i4Ret = 0;

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    // Try lock to make sure the semaphore was locked
    i4Ret = x_sema_lock(_hCmdWaitSema[u1DspId][u1DecId], X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_STOP));
    AUD_WaitCommandDone(u1DspId, u1DecId, AUD_CMD_STOP);
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
    return AUD_OK;
}
#endif

//-----------------------------------------------------------------------------
// AUD_DSPCmdResetAsyn
//
/** Brief of AUD_DSPCmdResetAsyn.
 *  Details of AUD_DSPCmdResetAsyn (optional).
 *
 *  @param        u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_DSPCmdResetAsyn(UINT8 u1DspId, UINT8 u1DecId)
{
    LOG(5, "CMD: set Reset Asyn: Dsp(%d) Dec(%d)\n", u1DspId, u1DecId);

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(x_sema_lock(_hAsynCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(AUD_DRVCmd(u1DspId, u1DecId, AUD_CMD_STOP));
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);

    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_RegNotifyFunc
//
/** Register audio notify function.
 *
 *  @param     pu4Handler   Notify function handler
 *  @param     pfNfyFunc    Notify function pointer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL AUD_RegNotifyFunc(UINT32* pu4Handler, AudDecNfyFct pfNfyFunc)
{
    return _AUD_RegNotifyFunc(pu4Handler, pfNfyFunc);
}

//-----------------------------------------------------------------------------
// AUD_DeRegNotifyFunc
//
/** Deregister audio notify function.
 *
 *  @param  u4Handler   Handler
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL AUD_DeRegNotifyFunc(UINT32 u4Handler)
{
    return _AUD_DeRegNotifyFunc(u4Handler);
}

//-----------------------------------------------------------------------------
// AUD_IsDecoderPlay
//
/** Brief of AUD_IsDecoderPlay.
 *  Details of AUD_IsDecoderPlay (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval     TRUE
 *  @retval     FALSE
 */
//-----------------------------------------------------------------------------
BOOL AUD_IsDecoderPlay(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DRV_STATE_T eDrvState;
    eDrvState = AUD_DRVGetAudioState(u1DspId, u1DecId);
    return ((eDrvState == AUD_IDLE) || (eDrvState == AUD_UNINITIALIZED)) ? FALSE : TRUE;
}

//-----------------------------------------------------------------------------
// AUD_IsDecoderPlay
//
/** Brief of AUD_IsDecoderRealPlay.
 *  Details of AUD_IsDecoderRealPlay (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval     TRUE
 *  @retval     FALSE
 */
//-----------------------------------------------------------------------------
BOOL AUD_IsDecoderRealPlay(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DRV_STATE_T eDrvState;
    eDrvState = AUD_DRVGetAudioState(u1DspId, u1DecId);
    return (eDrvState == AUD_ON_PLAY) ? TRUE : FALSE;
}


//-----------------------------------------------------------------------------
// AUD_IsDSPRealPlay
//
/** Brief of AUD_IsDSPRealPlay.
 *  Details of AUD_IsDSPRealPlay (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval     TRUE
 *  @retval     FALSE
 */
//-----------------------------------------------------------------------------
BOOL AUD_IsDSPRealPlay(UINT8 u1DspId, UINT8 u1DecId)
{
    return _rDspStatus[u1DspId][u1DecId].fgDspRealPlay;
}


// AUD_IsDecoderStop
//
/** Brief of AUD_IsDecoderStop.
 *  Details of AUD_IsDecoderStop (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval     TRUE
 *  @retval     FALSE
*/
BOOL AUD_IsDecoderStop(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DRV_STATE_T eDrvState;
    eDrvState = AUD_DRVGetAudioState(u1DspId, u1DecId);
    return ( (eDrvState == AUD_WAIT_STOP) || (eDrvState == AUD_WAIT_STOPSTREAM) || (eDrvState == AUD_IDLE)) ? TRUE : FALSE;
}
//-----------------------------------------------------------------------------
// AUD_GetDecodeType
//
/** Brief of AUD_GetDecodeType.
 *  Details of AUD_GetDecodeType (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom     Stream source
 *  @param      peDecType        Stream format
 */
//-----------------------------------------------------------------------------
void AUD_GetDecodeType(UINT8 u1DspId, UINT8 u1DecId, AUD_DEC_STREAM_FROM_T* peStreamFrom, AUD_FMT_T* peDecType)
{
    UNUSED(AUD_DRVGetDecodeType(u1DspId, u1DecId, peStreamFrom,  peDecType));
}

//-----------------------------------------------------------------------------
// AUD_GetHDMIDecodeType
//
/** Brief of AUD_GetHDMIDecodeType.
 *  Details of AUD_GetHDMIDecodeType (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom     Stream source
 *  @param      peDecType        Stream format
 */
//-----------------------------------------------------------------------------
void AUD_GetHDMIDecodeType(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T* peStreamFrom, AUD_FMT_T* peDecType)
{
    UNUSED(AUD_DRVGetHDMIDecodeType(u1DecId, peStreamFrom,  peDecType));
}


// AUD_GetAudioPTS
//
/** Brief of AUD_GetAudioPTS.
 *  Details of AUD_GetAudioPTS (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prAudioPts       AudioPts
 */
//-----------------------------------------------------------------------------
void AUD_GetAudioPTS(UINT8 u1DecId, UINT32* rAudioPts )
{
    UNUSED(AUD_GetAudioPts(u1DecId, rAudioPts));
}

//-----------------------------------------------------------------------------
// AUD_GetStreamFrom
//
/** Brief of AUD_GetStreamFrom.
 *  Details of AUD_GetStreamFrom (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peStreamFrom     Stream source
 */
//-----------------------------------------------------------------------------
void AUD_GetStreamFrom(UINT8 u1DspId, UINT8 u1DecId, AUD_DEC_STREAM_FROM_T *peStreamFrom)
{
     UNUSED(AUD_DRVGetStreamFrom(u1DspId, u1DecId,  peStreamFrom));
}

#ifdef CC_AUD_DDI
void AudLGSEFN000(UINT8 fNo, UINT32 pParamsPtr, UINT16 noParam, UINT8 dataOption, UINT8 varOption)
{

#ifdef LINUX_TURNKEY_SOLUTION
    UINT32 u4Ret;
    //VOID * u1CV_param_buf;
    if (_argLgseFnPara[fNo].pParams != NULL)
    {
        x_mem_free(_argLgseFnPara[fNo].pParams);
    }
    _argLgseFnPara[fNo].pParams = (void*)x_mem_alloc(noParam*4);
    if (_argLgseFnPara[fNo].pParams == NULL)
    {
        LOG(0, "allocate memory for _argLgseFnPara[fNo].pParams fail!\n");
        ASSERT(0);
    }
    u4Ret = copy_from_user(_argLgseFnPara[fNo].pParams, (UINT8*)pParamsPtr,
                           (noParam*4));
#else

#endif
    AUD_LGSEFN000(fNo, _argLgseFnPara[fNo].pParams, noParam, dataOption, varOption);
}

void AudGetLGSEFN010Out(UINT32* pParam)
{
	AUD_GetLGSEFN010Out(pParam);
}
#endif

#ifdef  LINUX_TURNKEY_SOLUTION
#if 1
//-----------------------------------------------------------------------------
// AudLoadAudioClip
//
/** Brief of AudLoadAudioClip.
 *  Details of AudLoadAudioClip (optional).
 *
 *  @param      u1DecId                   Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u1BufferSize             ClipBufferSize
 *  @param      u1ClipBufferPointer   u1ClipBufferPointer
 *  @param      u1DecFmt                 DecFmt
 *  @param      u1ReaptNumber        ReaptNumber
 */
//-----------------------------------------------------------------------------
void AudLoadAudioClip(UINT8 u1DspId, UINT8 u1DecId, UINT32 u1BufferSize, UINT32 u1ClipBufferPointer, AUD_FMT_T u1DecFmt, UINT32 u1ReaptNumber )
{
    UINT32 u4Ret;
    AUD_CLIP_TYPE_T u1AudClip;
    u1AudClip.fgPlayTestTone = FALSE;
    u1AudClip.u4OutputChlSelect = 0;
    u1AudClip.u4ToneFreq = 1000;
    u1AudClip.eAudFmt = u1DecFmt;
    u1AudClip.eMemBuf.u4LoopCount = 1;
    u1AudClip.u4RepeatCount = u1ReaptNumber;
    u1AudClip.u4AudCtrlTypeAddr = 0;

    //u1Clip_buf = (void *)x_mem_alloc(u1BufferSize);
    LOG(2, "===========AudLoadAudioClip=========== u1BufferSize = 0x%x\n",u1BufferSize);
    u1Clipfg = 1;
    if ((u1Clip_buf == NULL) || (ClipBufferSize < u1BufferSize))
    {
        if(u1Clip_buf != NULL)
        {
            x_mem_free(u1Clip_buf);
        }
        u1Clip_buf = (void *)x_mem_alloc(u1BufferSize);
    if(NULL == u1Clip_buf)
    {
            LOG(0, "allocate memory for AUD_CLIP_BUFF fail!\n");
            ASSERT(0);
            return ;
        }
    else
    {
        //u1Clip_buf = (void *)x_mem_alloc(u1BufferSize);
        ClipBufferSize = u1BufferSize;
        LOG(3, "allocate memory for AUD_CLIP_BUFF success!\n");
    }
    }
    u4Ret = copy_from_user(u1Clip_buf, (UINT8*)u1ClipBufferPointer,
                           u1BufferSize);
    u1AudClip.eMemBuf.pData = u1Clip_buf;

    u1AudClip.eMemBuf.u4Length = u1BufferSize;
    //u1AudClip.u4RepeatCount = prClip->ui4_repeat_cnt;
    //u1AudClip.u4OutputChlSelect = prClip->ui4_ch_mask;
    //u1AudClip.u4AudCtrlTypeAddr = (UINT32)AUD_DEC_CTRL_PLAY_AUD_CLIP;
    x_memcpy((VOID*)VIRTUAL((UINT32)&_u1AudClip[u1DecId]), &u1AudClip, sizeof(AUD_CLIP_TYPE_T));
    AUD_SetAudClip(u1DspId, u1DecId, &_u1AudClip[u1DecId]);
}
#else
//-----------------------------------------------------------------------------
// AudLoadAudioClip
//
/** Brief of AudLoadAudioClip.
 *  Details of AudLoadAudioClip (optional).
 *
 *  @param      u1DecId                   Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u1BufferSize             ClipBufferSize
 *  @param      u1ClipBufferPointer   u1ClipBufferPointer
 */
//-----------------------------------------------------------------------------

void AudLoadAudioClip(UINT8 u1DecId, UINT32 u1BufferSize, UINT32 u1ClipBufferPointer)
{

    AUD_CLIP_TYPE_T u1AudClip;
    u1AudClip.fgPlayTestTone = FALSE;
    u1AudClip.u4OutputChlSelect = 0;
    u1AudClip.u4ToneFreq = 1000;
    u1AudClip.eAudFmt = AUD_FMT_PCM;
#ifdef  LINUX_TURNKEY_SOLUTION
    u1Clip_buf = (void*)x_mem_alloc(u1BufferSize);
    u1Clipfg = 1;
    if (u1Clip_buf == NULL)
    {
        LOG(0, "allocate memory for AUD_CLIP_BUFF fail!\n");
        ASSERT(0);
    }
    rm_copy_from_user(u1Clip_buf, (UINT8*)u1ClipBufferPointer,
                      u1BufferSize);
    u1AudClip.eMemBuf.pData = u1Clip_buf;

#else
    //u1AudClip.eMemBuf.pData = prClip->u.t_clip_buff.pui1_clip_buff;
#endif
    u1AudClip.eMemBuf.u4Length = u1BufferSize;
    //u1AudClip.u4RepeatCount = prClip->ui4_repeat_cnt;
    //u1AudClip.u4OutputChlSelect = prClip->ui4_ch_mask;
    //u1AudClip.u4AudCtrlTypeAddr = (UINT32)AUD_DEC_CTRL_PLAY_AUD_CLIP;
    x_memcpy((VOID*)VIRTUAL((UINT32)&_u1AudClip[u1DecId]), &u1AudClip, sizeof(AUD_CLIP_TYPE_T));
    AUD_SetAudClip(u1DecId, &_u1AudClip[u1DecId]);

}
#endif
#endif

#ifdef CC_AUD_SUPPORT_ENC
extern UINT32 u4GetUploadFIFOStart(void);
extern UINT32 u4GetUploadFIFOEnd(void);
extern UINT32 u4GetUploadWritePnt(void);
void AUD_GetEncData(UINT32 pDest,UINT32 pBufAddr,UINT32 datasize,UINT32 pRStart,UINT32 pREnd)
{
    UINT32 u4Ret;
    UINT32 u4Start,u4End, u4CopyDataSize,u4ENCCurrectRp;
    static UINT32 u4TotalSize = 0;
    
    u4Start = pRStart;
    u4End = pREnd;
    u4CopyDataSize=datasize;
    u4ENCCurrectRp=pBufAddr;
    //LOG(5,"for encmp3 pDest=%x, pData=%x. dataLen=%x,pRStart=%x,pREnd=%x \n",pDest,pBufAddr,datasize,pRStart,pREnd);


    #ifdef  LINUX_TURNKEY_SOLUTION            
    u4Ret=copy_to_user((VOID*)(pDest),(VOID*)VIRTUAL(pBufAddr),datasize);
    #endif
        
    u4TotalSize += datasize;
    LOG(5,"recv: pData=%x. dataLen=%x, pRStart=%x,pREnd=%x,totalsz:%x\n\n",pBufAddr,datasize,pRStart,pREnd,u4TotalSize);
}

#if 0   // Unused
void AUD_RecordPCM(UINT32 u4BufferSize, UINT32 u4ClipBufferPointer)
{
    UINT32 u4Start, u4End, u4WP, u4RP, u4SendSize, u4TotalSendSize;

    // 0. Enable Upload Buffer
    AUD_DspSetUploadMode(4);
    AUD_DspUploadDataEnable();

    // 1. Wait WP exceeds the u4BufferSize
    u4Start = u4GetUploadFIFOStart();
    u4End = u4GetUploadFIFOEnd();
    Printf("u4Start: 0x%08x u4End: 0x%08x\n", u4Start, u4End);
    u4RP = u4Start;
    u4TotalSendSize = 0;

    // 2. Copy to user
    while (u4BufferSize)
    {
        u4WP = u4GetUploadWritePnt();
        if (u4BufferSize > 0x1000)
        {
            u4SendSize = 0x1000;
        }
        else
        {
            u4SendSize = u4BufferSize;
        }
        if (((u4WP >= u4RP) && ((u4WP - u4RP) >= u4SendSize)) ||
            ((u4WP < u4RP) && ((u4End - u4Start - u4RP + u4WP) >= u4SendSize)))
        {
#ifdef LINUX_TURNKEY_SOLUTION
            UINT32 u4Ret;
#endif

            Printf(".");
#ifdef  LINUX_TURNKEY_SOLUTION
            u4Ret = copy_to_user((VOID*)(u4ClipBufferPointer + u4TotalSendSize), (VOID*)VIRTUAL(u4RP), u4SendSize);
#endif
            u4BufferSize -= u4SendSize;
            u4TotalSendSize += u4SendSize;
            u4RP += u4SendSize;
            if (u4RP == u4End)
            {
                u4RP = u4Start;
            }
        }
        else
        {
            x_thread_delay(10);
        }
        //Printf("u4BufferSize: 0x%08x u4ClipBufferPointer: 0x%08x\n", u4BufferSize, u4ClipBufferPointer);
    }

    // 3. Disable Upload
    AUD_DspSetUploadMode(0);
    AUD_DspUploadDataEnable();
}
#endif
#endif

#ifdef CC_SUPPORT_BLUETOOTH_UPLOAD
extern UINT32 u4GetBluetoothFIFOStart(void);
extern UINT32 u4GetBluetoothFIFOEnd(void);
extern UINT32 u4GetBluetoothWritePnt(void);

void AUD_GetBluetoothDate(UINT32 pDest,UINT32 pBufAddr,UINT32 datasize)
{
    UINT32 u4Start,u4End,u4ENCCurrectRp;//u4CopyDataSize,u4SendSize,u4TotalSendSize,
    u4Start = u4GetBluetoothFIFOStart();
    u4End = u4GetBluetoothFIFOEnd();
    //u4CopyDataSize=datasize;
    u4ENCCurrectRp=pBufAddr;
    LOG(5,"for Bluetooth pDest=%x, pData=%x. dataLen=%x,pRStart=%x,pREnd=%x \n",pDest,pBufAddr,datasize,u4Start,u4End);
    copy_to_user((VOID*)(pDest),(VOID*)VIRTUAL(u4ENCCurrectRp),datasize);

    /*
    u4TotalSendSize = 0;
    while (u4CopyDataSize)
        {        
            if(u4CopyDataSize<(u4End-u4ENCCurrectRp))
            {
                u4SendSize=u4CopyDataSize;
            }
            else
            {
                u4SendSize=(u4End-u4ENCCurrectRp);
            }
            
            LOG(5,"copy upload buffer date to ENC buffer.rp=%x size=%x\n",u4ENCCurrectRp,u4SendSize);
    #ifdef  LINUX_TURNKEY_SOLUTION            
            u4Ret=copy_to_user((VOID*)(pDest+u4TotalSendSize),(VOID*)VIRTUAL(u4ENCCurrectRp),u4SendSize);
    #endif
            u4CopyDataSize -= u4SendSize;
            u4TotalSendSize += u4SendSize;
            u4ENCCurrectRp += u4SendSize;
            
            if (u4ENCCurrectRp >= u4End)
            {
                u4ENCCurrectRp = u4Start;
            }
     
        }*/

}
#endif

//-----------------------------------------------------------------------------
// AUD_SetAudClip
//
/** Brief of AUD_SetAudClip.
 *  Details of AUD_SetAudClip (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      peAudClip
 */
//-----------------------------------------------------------------------------
void AUD_SetAudClip(UINT8 u1DspId, UINT8 u1DecId, AUD_CLIP_TYPE_T* peAudClip)
{
    if (peAudClip)
    {
        UINT8 u1Sel, i;
        UINT8 u4OutChannel = 0;

        if (peAudClip->u4ToneFreq <= 200)
        {
            u1Sel = TEST_TONE_200_0dB;
        }
        else if (peAudClip->u4ToneFreq == 400)
        {
            u1Sel = TEST_TONE_400_0dB;
        }
        else if (peAudClip->u4ToneFreq <= (1 * 1000))
        {
            u1Sel = TEST_TONE_1K_0dB;
        }
        else if (peAudClip->u4ToneFreq <= (2 * 1000))
        {
            u1Sel = TEST_TONE_2K_0dB;
        }
        else if (peAudClip->u4ToneFreq <= (3 * 1000))
        {
            u1Sel = TEST_TONE_3K_0dB;
        }
        else if (peAudClip->u4ToneFreq <= (5 * 1000))
        {
            u1Sel = TEST_TONE_5K_0dB;
        }
        else if (peAudClip->u4ToneFreq <= (8 * 1000))
        {
            u1Sel = TEST_TONE_8K_0dB;
        }
        else if (peAudClip->u4ToneFreq <= (12 * 1000 + 500))
        {
            u1Sel = TEST_TONE_12_5K_0dB;
        }
        else
        {
            u1Sel = TEST_TONE_20K_0dB;
        }

        if (peAudClip->fgPlayTestTone)
        {
            AUD_PCM_SETTING_T rPcmSetting;

            // Notes that tone table is 48K, 24-bits format
            rPcmSetting.eSampleFreq = FS_48K;
            rPcmSetting.ePcmDataInfo = PCM_24_BITS;

            peAudClip->eAudFmt = AUD_FMT_PCM;
            AUD_StrSelect(u1DspId, u1DecId, u1Sel, &peAudClip->eMemBuf);
            AUD_DrvPcmSetting(u1DspId, u1DecId, &rPcmSetting);
        }

        for (i = (UINT8)AUD_CH_FRONT_LEFT; i <= (UINT8)AUD_CH_SUB_WOOFER; i++)
        {
            if (MAKE_BIT_MASK(i) & peAudClip->u4OutputChlSelect)
            {
                u4OutChannel = i;
                break;
            }
        }

        switch (u4OutChannel / 2)
        {
            case 0:  // L/R channel
                AUD_AoutPADMux(AUD_CH_FRONT_LEFT, AUD_CH_AUX_FRONT_LEFT,
                               AUD_CH_AUX_FRONT_LEFT, _grAOutPadCfg.ePadSdata3,
                               _grAOutPadCfg.ePadSdata4, _grAOutPadCfg.ePadSdata5,
                               0);
                break;
            case 1: // LS/RS channel
                AUD_AoutPADMux(AUD_CH_AUX_FRONT_LEFT, AUD_CH_FRONT_LEFT,
                               AUD_CH_AUX_FRONT_LEFT, _grAOutPadCfg.ePadSdata3,
                               _grAOutPadCfg.ePadSdata4, _grAOutPadCfg.ePadSdata5,
                               0);
                break;
            case 2: // C/Subwoofer channel
                AUD_AoutPADMux(AUD_CH_AUX_FRONT_LEFT, AUD_CH_AUX_FRONT_LEFT,
                               AUD_CH_FRONT_LEFT, _grAOutPadCfg.ePadSdata3,
                               _grAOutPadCfg.ePadSdata4, _grAOutPadCfg.ePadSdata5,
                               0);
                break;
            default:
                break;
        }

        peAudClip->eMemBuf.u4LoopCount = peAudClip->u4RepeatCount;
        UNUSED(AUD_SetDecType(u1DspId, u1DecId, AUD_STREAM_FROM_MEMORY, peAudClip->eAudFmt));
        UNUSED(AUD_SetDecMemBuffer(u1DspId, u1DecId, &peAudClip->eMemBuf));
    }
}

#ifdef CC_AUD_SUPPORT_CLIP_NOTIFY_FUNC
//-----------------------------------------------------------------------------
// AUD_ClipSetNotifyFunc
//
/** Brief of AUD_ClipSetNotifyFunc.
 *  Details of AUD_ClipSetNotifyFunc (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      pfClipNotify
 */
//-----------------------------------------------------------------------------
void AUD_ClipSetNotifyFunc(UINT8 u1DecId, AUD_CLIP_NOTIFY pfClipNotify)
{
    AUD_DrvSetNotifyFunc(u1DecId, pfClipNotify);
}
#endif

//-----------------------------------------------------------------------------
// AUD_PlayAudClipWithRPN
//
/** Brief of AUD_PlayAudClipWithRPN.
 *  Details of AUD_PlayAudClipWithRPN (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u4Channel
 *  @param      ReaptNumber
 *  @param      fgPlayTestTone
 */
void AUD_PlayAudClipWithRPN(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4Channel, UINT32 ReaptNumber, BOOL fgPlayTestTone)
{

    AUD_SentReaptNumber(u1DecId,  ReaptNumber);
    AUD_PlayAudClip( u1DspId, u1DecId,  u4Channel,  fgPlayTestTone );
}

//-----------------------------------------------------------------------------
// AUD_PlayAudClip
//
/** Brief of AUD_PlayAudClip.
 *  Details of AUD_PlayAudClip (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u4Channel
 *  @param      fgPlayTestTone
 */
//-----------------------------------------------------------------------------
void AUD_PlayAudClip(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4Channel, BOOL fgPlayTestTone)
{
    UINT8 i;
    UINT8 u4ClipChlVolumn[AUD_CH_NUM];
    ///UINT8 u4ClipChlDelay[AUD_CH_NUM];

    AUD_DEC_ID_VALIDATE(u1DecId);

    x_memset((VOID*)VIRTUAL((UINT32)u4ClipChlVolumn), 0, sizeof(UINT8) * (UINT8)AUD_CH_NUM);

    for (i = (UINT8)AUD_CH_FRONT_LEFT; i <= (UINT8)AUD_CH_SUB_WOOFER; i++)
    {
        // Get the original channel volumn
        _au1ChannelVolume[u1DecId][i] =
            AUD_DspGetChannelVolume(u1DecId, (AUD_CH_T)i);

        // Set the new channel volumn
        // Only the select channel keeps the original volumn,
        // Others will set to be zero
        if (MAKE_BIT_MASK(i) & u4Channel)
        {
            if (fgPlayTestTone)
            {
                // If play the test tone,
                // Output pad LS/RS, C/Subwoofer will set as L/R
                u4ClipChlVolumn[i % (UINT8)AUD_CH_REAR_LEFT] =
                    _au1ChannelVolume[u1DecId][i];
            }
            else
            {
                u4ClipChlVolumn[i] = _au1ChannelVolume[u1DecId][i];
            }
        }
    }

    // Set the new channel volumn
    for (i = (UINT8)AUD_CH_FRONT_LEFT; i <= (UINT8)AUD_CH_SUB_WOOFER; i++)
    {
        AUD_DspChannelVolume(u1DspId, u1DecId, (AUD_CH_T)i, u4ClipChlVolumn[i]);
    }
    for (i = (UINT8)AUD_CH_FRONT_LEFT; i <= (UINT8)AUD_CH_DMX_RIGHT; i++)
    {
        _au1ChannelDelay[u1DecId][i] =
            AUD_DspGetChannelDelay(u1DspId, (AUD_CH_T)i);
        AUD_DspChannelDelay(u1DspId, 0,(AUD_CH_T)i,u1DecId);
    }
    _fgClipChnVolChanged = TRUE;

    UNUSED(AUD_DSPCmdPlay(u1DspId, u1DecId));
}

//-----------------------------------------------------------------------------
// AUD_StopAudClip
//
/** Brief of AUD_StopAudClip.
 *  Details of AUD_StopAudClip (optional).
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 */
//-----------------------------------------------------------------------------
void AUD_StopAudClip(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT8 i;

    LOG(5, "CMD: set Stop Aud Clip: Dec(%d)\n", u1DecId);

    AUD_StopMemStr();
    UNUSED(AUD_DSPCmdStop(u1DspId, u1DecId));

    AUD_AoutPADMux(_grAOutPadCfg.ePadSdata0, _grAOutPadCfg.ePadSdata1,
                   _grAOutPadCfg.ePadSdata2, _grAOutPadCfg.ePadSdata3,
                   _grAOutPadCfg.ePadSdata4, _grAOutPadCfg.ePadSdata5, 0);

    // Restore the channel volumn setting before play aud clip
    if (_fgClipChnVolChanged)
    {
        for (i = (UINT8)AUD_CH_FRONT_LEFT; i <= (UINT8)AUD_CH_SUB_WOOFER; i++)
        {
            AUD_DspChannelVolume(u1DspId, u1DecId, (AUD_CH_T)i, _au1ChannelVolume[u1DecId][i]);
        }
        for (i = (UINT8)AUD_CH_FRONT_LEFT; i <= (UINT8)AUD_CH_DMX_RIGHT; i++)
        {
            AUD_DspChannelDelay(u1DspId, _au1ChannelDelay[u1DecId][i], (AUD_CH_T)i, u1DecId);
        }
    }
    _fgClipChnVolChanged = FALSE; //Andrew Wen 2007/9/10 Sometime AP call StopClip before PlayClip
}

// AUD_DeleteAudClip

//-----------------------------------------------------------------------------
void AUD_DeleteAudioClip(void)
{
#ifdef  LINUX_TURNKEY_SOLUTION
    x_mem_free(u1Clip_buf);
    u1Clip_buf = NULL;
#endif
}

//-----------------------------------------------------------------------------
// Aud_SetAudClipVolume
//
/** Brief of Aud_SetAudClipVolume.
 *  Details of Aud_SetAudClipVolume (optional).
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *              u1Volume       Clip Vol  0 ~ 30 mapping to -20 ~ +10dB
 */
//-----------------------------------------------------------------------------
void Aud_SetAudClipVolume(UINT8 u1DspId, UINT8 u1DecId, UINT32 u4Channel, UINT8 u1Volume)
{
    INT32 i4Vol = 0;

    UNUSED(u4Channel);
    UNUSED(u1DecId);
    UNUSED(u1DspId);

    //0 ~ 30 mapping to -20 ~ +10 dB
    i4Vol = (u1Volume - 20) * 2;

    AUD_DspChannelVolGain(u1DecId, (AUD_CH_T)u4Channel, i4Vol);
}

//-----------------------------------------------------------------------------
// AUD_AvInputMuxSel
//
/** Brief of AUD_AvInputMuxSel.
 *  Details of AUD_AvInputMuxSel (optional).
 *
 *  @param  eInputId   AV component id
 *
 */
//-----------------------------------------------------------------------------
void AUD_AvInputMuxSel(AUD_INPUT_ID_T eInputId)
{
    AUD_AvMuxSel(eInputId);
}

// AUD_AvInputMuxSelByAD
//
/** Brief of AUD_AvInputMuxSelByAD.
 *  Details of AUD_AvInputMuxSelByAD(optional).
 *
 *  @param  UINT8 eADNum
 *
 */
//-----------------------------------------------------------------------------
void AUD_AvInputMuxSelByAD(UINT8 eADNum)
{
    AUD_AvMuxSelByAD(eADNum);
}

//-----------------------------------------------------------------------------
// AUD_SetCtrlType
//
/** Brief of AUD_SetCtrlType.
 *  Details of AUD_SetCtrlType (optional).
 *
 *  @param  u4MWSetCtrlType
 *
 */
//-----------------------------------------------------------------------------
void AUD_SetCtrlType(UINT8 u1DspId,  UINT8 u1DecId, UINT32 u4MWSetCtrlType)
{
    _au4AudCtrlType[u1DspId][u1DecId] = u4MWSetCtrlType;
}

//-----------------------------------------------------------------------------
// AUD_SetCtrlCmd
//
/** Brief of AUD_SetCtrlCmd.
 *  Details of AUD_SetCtrlCmd (optional).
 *
 *  @param  eMWSetCtrlCmd
 *
 */
//-----------------------------------------------------------------------------
void AUD_SetCtrlCmd(UINT8 u1DspId,  UINT8 u1DecId, AUD_DEC_CMD_T eMWSetCtrlCmd)
{
    _aeAudCtrlCmd[u1DspId][u1DecId]= eMWSetCtrlCmd;
}

//-----------------------------------------------------------------------------
// AUD_Mute
//
/** Brief of AUD_Mute.
 *  Details of AUD_Mute (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_Mute(UINT8 u1DecId)
{
    BOOL fMuteAllSource;

    LOG(5, "CMD: set Mute: Dec(%d)\n", u1DecId);

    AUD_DEC_ID_VALIDATE(u1DecId);

    AUD_DspMuteEnable(u1DecId, TRUE);

#ifdef CC_MT5391_AUD_3_DECODER
    if ((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX))
#endif
    {
        fMuteAllSource = (BOOL)DRVCUST_OptGet(eAudioMuteAllSource);
        if (fMuteAllSource)
        {
            ADAC_Mute(TRUE);
        }
    }
    return AUD_OK;
}

//-----------------------------------------------------------------------------
// AUD_UnMute
//
/** Brief of AUD_UnMute.
 *  Details of AUD_UnMute (optional).
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_UnMute(UINT8 u1DecId)
{
    BOOL fMuteAllSource;

    LOG(5, "CMD: set UnMute: Dec(%d)\n", u1DecId);

    AUD_DEC_ID_VALIDATE(u1DecId);

    AUD_DspMuteEnable(u1DecId, FALSE);

#ifdef CC_MT5391_AUD_3_DECODER
    if ((u1DecId == AUD_DEC_MAIN) || (u1DecId == AUD_DEC_AUX))
#endif
    {
        fMuteAllSource = (BOOL)DRVCUST_OptGet(eAudioMuteAllSource);
        if (fMuteAllSource)
        {
            ADAC_Mute(FALSE);
        }
    }
    return AUD_OK;
}

#ifdef CC_MPEG_LEVEL_ADJ
//-----------------------------------------------------------------------------
// AUD_MpegLevelAdjust
//
/** Brief of AUD_MpegLevelAdjust.
 *  Details of AUD_MpegLevelAdjust (optional).
 *
 *  @param
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
void AUD_MpegLevelAdjust (UINT8 u1Level)
{
    AUD_DrvMpegLevelAdjust(u1Level);
}
#endif
//-----------------------------------------------------------------------------
// AUD_FeederSetRequestInfo
//
/** For feeder to set request information.
 *
 *  @param     prTransmit   Notify function handler
 *
 *  @retval    TRUE   ok
 *  @retval    FALSE  fail
 */
//-----------------------------------------------------------------------------
BOOL AUD_FeederSetRequestInfo(FEEDER_TRANSMIT* prTransmit)
{
    return AUD_DrvFeederSetRequestInfo(prTransmit);
}

//-----------------------------------------------------------------------------
// AUD_FlacSetting
//
/** FLAC format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)
 *  @param      prFlacSetting
 *  paul_flac
 */
//-----------------------------------------------------------------------------
void AUD_FlacSetting(UINT8 u1DecID, AUD_FLAC_INFO_T* prFlacSetting)
{
    AUD_DEC_ID_VALIDATE(u1DecID);
    if (AUD_DEC_MAIN != u1DecID)
    {
        LOG(5, "Currently we only support FLAC in dec1!");
        return;
    }
    if (NULL != prFlacSetting)
    {
        AUD_DrvFlacSetting(u1DecID, prFlacSetting);
    }
}

//-----------------------------------------------------------------------------
// AUD_PcmSetting
//
/** PCM format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting
 */
//-----------------------------------------------------------------------------
void AUD_PcmSetting(UINT8 u1DspId, UINT8 u1DecId, const AUD_PCM_SETTING_T* prPcmSetting)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (prPcmSetting != NULL)
    {
        AUD_DrvPcmSetting(u1DspId, u1DecId, prPcmSetting);
    }
}

#ifdef CC_HDMI_PCM_MULT_CHANNEL
//-----------------------------------------------------------------------------
// AUD_LineInChNumSetting
//
/** Line-In Channel Number Setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      u1LineInChNum    Line-In Channel Number Setting
 */
//-----------------------------------------------------------------------------
void AUD_LineInChNumSetting(UINT8 u1DecId, UINT8 u1LineInChNum)
{
    UINT8   u1LineInChNumSetting = 0;

   AUD_DEC_ID_VALIDATE(u1DecId);

    switch(u1LineInChNum)
    {
        case 1:
        case 2:
            u1LineInChNumSetting = 0;
            break;
        case 3:
        case 4:
            u1LineInChNumSetting = 1;
            break;
        case 5:
        case 6:
            u1LineInChNumSetting = 2;
            break;
        case 7:
        case 8:
            u1LineInChNumSetting = 3;
            break;
        default:
            u1LineInChNumSetting = 0;
            break;
    }

    if(u1DecId == AUD_DEC_MAIN)
    {
        LOG(1, "AUD_LineInChNumSetting -> REG_LIN_CFG0 = 0x%x \n", AUD_READ32(REG_LIN_CFG0));
        AUD_WRITE32_MSK(REG_LIN_CFG0, (u1LineInChNumSetting << PCM_CH_NUM_POS), PCM_CH_NUM_MASK);
        LOG(1, "AUD_LineInChNumSetting -> REG_LIN_CFG0 = 0x%x \n", AUD_READ32(REG_LIN_CFG0));
    }
}

//-----------------------------------------------------------------------------
// AUD_LineInChNumFormat
//
/** Line-In Channel Number Format
 *
 *  @param      u1LineInChNum    Line-In Channel Number Setting
 *  @param      pu4Format        
 */
//-----------------------------------------------------------------------------
void AUD_LineInChNumFormat(UINT8 u1LineInChNum, UINT32* pu4Format)
{
    UINT8   u1LineInChNumSetting = 0;

    switch(u1LineInChNum)
    {
        case 1:
        case 2:
            u1LineInChNumSetting = 0;
            break;
        case 3:
        case 4:
            u1LineInChNumSetting = 1;
            break;
        case 5:
        case 6:
            u1LineInChNumSetting = 2;
            break;
        case 7:
        case 8:
            u1LineInChNumSetting = 3;
            break;
        default:
            u1LineInChNumSetting = 0;
            break;
    }

    LOG(0, "AUD_LineInChNumFormat -> *pu4Format = 0x%x \n", *pu4Format);
    *pu4Format = ((*pu4Format & (~PCM_CH_NUM_MASK)) | (u1LineInChNumSetting << PCM_CH_NUM_POS));
    LOG(0, "AUD_LineInChNumFormat -> *pu4Format = 0x%x \n", *pu4Format);
}

#endif
void AUD_LPcmSetting(UINT8 u1DecId, AUD_LPCM_INFO_T* prLPcmInfo)
{
    UINT32 u4LpcmSetting = 0;
    AUD_LPCM_SETTING_T rLpcmSetting;

    UNUSED(u4LpcmSetting);
    UNUSED(rLpcmSetting);
    UNUSED(_fgLpcmTpyePS);

    _fgLpcmTpyePS = (prLPcmInfo->b_info_not_specific);

    if (!_fgLpcmTpyePS) // AVI
    {
        // Set LPCM format
        // common default
        rLpcmSetting.u1LpcmBitShift = 0;
        rLpcmSetting.u1LpcmDrcValue = 0x80;
        rLpcmSetting.u1LpcmDrcFlag = 0;
        // sampling rare
        rLpcmSetting.u1LpcmFreqGrp2 = 0xf;
        switch (prLPcmInfo->eSampleFreq)
        {
            case FS_5K:
                rLpcmSetting.u1LpcmFreqGrp1 = 22;
                break;
            case FS_6K:
                rLpcmSetting.u1LpcmFreqGrp1 = 21; //LPCM_6K
                break;
            case FS_8K:
                rLpcmSetting.u1LpcmFreqGrp1 = 20;
                break;
            case FS_12K:
                rLpcmSetting.u1LpcmFreqGrp1 = 4;
                break;
            case FS_16K:
                rLpcmSetting.u1LpcmFreqGrp1 = 19;
                break;
            case FS_32K:
                rLpcmSetting.u1LpcmFreqGrp1 = 16;
                break;
            case FS_11K:
                rLpcmSetting.u1LpcmFreqGrp1 = 12;
                break;
            case FS_22K:
                rLpcmSetting.u1LpcmFreqGrp1 = 11;
                break;
            case FS_24K:
                rLpcmSetting.u1LpcmFreqGrp1 = 3;
                break;
            case FS_44K:
                rLpcmSetting.u1LpcmFreqGrp1 = 8;
                break;
            case FS_176K:
                rLpcmSetting.u1LpcmFreqGrp1 = 10;
                break;
            case FS_192K:
                rLpcmSetting.u1LpcmFreqGrp1 = 2;
                break;
            case FS_88K:
                rLpcmSetting.u1LpcmFreqGrp1 = 9;
                break;
            case FS_96K:
                //rLpcmSetting.u1LpcmFreqGrp2 = 0;
                rLpcmSetting.u1LpcmFreqGrp1 = 1;
                break;
            default : //Set to 48K
                rLpcmSetting.u1LpcmFreqGrp2 = 0;
                rLpcmSetting.u1LpcmFreqGrp1 = 0;
                break;
        }

        // bit resolution
        rLpcmSetting.u1LpcmBitResGrp2 = 0;
        if (prLPcmInfo->ui2_bits_per_sample == 8) //there maybe 8 or 16 bit PCM
        {
            rLpcmSetting.u1LpcmBitResGrp1 = 3;
        }
        else if (prLPcmInfo->ui2_bits_per_sample == 24) //24 bit PCM comes from WAV
        {
            rLpcmSetting.u1LpcmBitResGrp1 = 2;
        }
        else if (prLPcmInfo->ui2_bits_per_sample == 32) //32 bit PCM comes from WAV
        {
            rLpcmSetting.u1LpcmBitResGrp1 = 4;
        }
        else
        {
            rLpcmSetting.u1LpcmBitResGrp1 = 0;
        }


        // channel assignment
        rLpcmSetting.u1LpcmChAssignment = (prLPcmInfo->ui2_num_ch - 1);

        // decoding type
        if (prLPcmInfo->eLPcmType == LPCM_TYPE_MS_ADPCM)
        {
            rLpcmSetting.u2LpcmDecodingType = 0x11;
            rLpcmSetting.u2ADPCMDecodingType = 0x11;
            // block align
            rLpcmSetting.u2ADPCMBlockAlign = prLPcmInfo->ui2_block_align;
        }
        else if (prLPcmInfo->eLPcmType == LPCM_TYPE_IMA_ADPCM)
        {
#if 0  //After MW can set correct TYPE, remove this.
            rLpcmSetting.u1LpcmDecodingType = 0x11;
            rLpcmSetting.u1ADPCMDecodingType = 0x11;
            // block align
            rLpcmSetting.u2ADPCMBlockAlign = prLPcmInfo->ui2_block_align;
#else  //This is correct IMA ADPCM setting.
            rLpcmSetting.u2LpcmDecodingType = 0x31;
            rLpcmSetting.u2ADPCMDecodingType = 0x31;
            // block align
            rLpcmSetting.u2ADPCMBlockAlign = prLPcmInfo->ui2_block_align * 2;
#endif
        }
        else if ((prLPcmInfo->eLPcmType == LPCM_TYPE_A_LAW) || (prLPcmInfo->eLPcmType == LPCM_TYPE_U_LAW))
        {
            rLpcmSetting.u2LpcmDecodingType = 0x101;
            if (prLPcmInfo->eLPcmType == LPCM_TYPE_U_LAW)
            {
                rLpcmSetting.u2LpcmDecodingType |= 0x200;
            }
            // block align
            rLpcmSetting.u2ADPCMBlockAlign = prLPcmInfo->ui2_block_align;
            rLpcmSetting.u2ADPCMDecodingType = 0;
        }
        else
        {
            if ((prLPcmInfo->b_big_endian) || (prLPcmInfo->ui2_bits_per_sample == 8)) //8 bit PCM should inverse byte??
            {
                rLpcmSetting.u2LpcmDecodingType = 0x5;
                if (prLPcmInfo->b_signed_data)
                {
                    rLpcmSetting.u2LpcmDecodingType |= 0x80; //bit7 : Signed 8 bit
                }
            }
            else
            {
                rLpcmSetting.u2LpcmDecodingType = 0xd;
            }

            if (prLPcmInfo->ui2_bits_per_sample == 24)
            {
                rLpcmSetting.u2LpcmDecodingType |= 0x2; //24 bit PCM comes from WAV
            }

            rLpcmSetting.u2ADPCMDecodingType = 0;
            // block align
            rLpcmSetting.u2ADPCMBlockAlign = prLPcmInfo->ui2_block_align;
        }

        AUD_SetLpcmTable(u1DecId, rLpcmSetting);
    }
}

//-----------------------------------------------------------------------------
// AUD_Ac3Setting
//
/** AC3 format setting
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      prPcmSetting
 */
//-----------------------------------------------------------------------------
void AUD_Ac3Setting(UINT8 u1DecId, const AUD_AC3_SETTING_T* prAC3Setting)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (prAC3Setting != NULL)
    {
        AUD_DrvAc3Setting(u1DecId, prAC3Setting);
    }
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// AUD_DtvLock
//
/** DTV audio lock check
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 */
//-----------------------------------------------------------------------------
BOOL AUD_DtvLock(UINT8 u1DecId)
{
    return AUD_DrvDtvLock(u1DecId);
}
#endif

//-----------------------------------------------------------------------------
// AUD_SetAvSynMode
//
/** DTV audio sync check
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN or AUD_DEC_AUX)
 *  @param      eSynMode       Sync mode, 0:Freerun 1: Slave, 2:Master
 */
//-----------------------------------------------------------------------------
void AUD_SetAvSynMode(UINT8 u1DecId, AV_SYNC_MODE_T eSynMode)
{
    AUD_DrvSetAvSynMode(AUD_DSP0, u1DecId,eSynMode);
}

//-----------------------------------------------------------------------------
// AUD_GetMultimediaMode
//
/**Get Audio Multimedia Mode
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)
 *  @retval      Multimedia mode, 0:Normal 1: Karaoke
 */
//-----------------------------------------------------------------------------
AUD_MM_MODE_T AUD_GetMultimediaMode(UINT8 u1DecId)
{
    return AUD_DrvGetMultimediaMode(u1DecId);
}

//-----------------------------------------------------------------------------
// AUD_SetMultimediaMode
//
/** Set Audio Multimedia Mode
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)
 *  @param      eMultimediaMode  Multimedia mode, 0: Normal 1: Karaoke
 *                                                2: Karaoke mic pass
 */
//-----------------------------------------------------------------------------
void AUD_SetMultimediaMode(UINT8 u1DecId, AUD_MM_MODE_T eMultimediaMode)
{
    LOG(5, "CMD: set MultimediaMode: Dec(%d) Mode(%d)\n",
        u1DecId, (UINT8)eMultimediaMode);
    AUD_DrvSetMultimediaMode(u1DecId, eMultimediaMode);
}

//-----------------------------------------------------------------------------
// AUD_SetAdMode
//
/** Set AD Mode
 *
 *  @param      fgAdMode  0: AD will not be turned on in this connection, 1: AD will be turned on
 */
void AUD_SetAdMode(BOOL fgAdMode)
{
    AUD_DrvSetAdMode(fgAdMode);
}

void _AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_OutPadEnable(u1DecId, fgEnable);
}

void AUD_DrvClkCfgDualDecMode(BOOL fgEnable)
{
    AUD_ClkCfgDualDecMode(fgEnable);
}

void AUD_SetHeadphoneCond(UINT8 u1DecId, UINT8 u1HPCond)
{
    _aeHPCond[u1DecId] = (AUD_HP_COND_NFY_T)u1HPCond;
}

UINT8 AUD_GetHeadphoneCond(UINT8 u1DecId)
{
    return _aeHPCond[u1DecId];
}

void AUD_AtunerSigStable(void)
{
    // FIXME
    // Should porting from 71 later
    AUD_DrvATunerLock();
}

void Aud_SetTvSystemMask(UINT8 u1DecId, UINT32 u4Mask)
{
    _AudSetTvSysMask(u1DecId,  u4Mask);
}

#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
void Aud_SetTvAudSystemMask(UINT8 u1DecId, UINT32 u4TvMask, UINT32 u4TvAudMask, ISO_3166_COUNT_T country_code)
{
    _AudSetTvAudSysMask(u1DecId,  u4TvMask, u4TvAudMask, country_code);
}
#endif

UINT32 Aud_GetTvSystemMask(UINT8 u1DecId)
{
    return  _AudGetTvSysMask(u1DecId);
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// AUD_ReadDspPROM
//
/** Read PROM 16-bits instruction.
 *  NOTICE: Do not use this function with Rom booting!
 *
 *  @param[in]        u4Addr           read PROM address
 *
 *  @retval PROM value
 */
//-----------------------------------------------------------------------------
UINT16 AUD_ReadPROM (UINT8 u1DspId, UINT32 u4Addr)
{
    vWriteDspSram (u1DspId, DspPROMTstAddr, DspPROMEnTst + (u4Addr << 8));
    return (u4ReadDspSram (u1DspId, DspPROMRdAddr) & DspPROMMsk);
}
#endif

//-----------------------------------------------------------------------------
// AUD_ReadDspDROM
//
/** Read DROM 24-bits data.
 *
 *  @param[in]        u4Addr           read DROM offset
 *
 *  @retval DROM value
 */
//-----------------------------------------------------------------------------
UINT16 AUD_ReadDROM (UINT8 u1DspId, UINT32 u4Addr)
{
    return (u4ReadDspSram (u1DspId, DspDROMRdAddr + u4Addr) & DspDROMMsk);
}

//-----------------------------------------------------------------------------
// AUD_Read32ByteID
//
/** Read ROM 32 bytes ID.
 *
 *  @param[in]        u1ID_p           ROM ID pointer
 *                    u4Num            ROM ID byte number
 *
 *  @retval AUD_OK    Success
 *  @retval AUD_FAIL  Fail
 */
//-----------------------------------------------------------------------------
INT32 AUD_Read32ByteID (UINT8 u1DspId, UINT8* u1ID_p, UINT32 u4Num)
{
    INT32 i;
    UINT32 tmp;
    UINT32 start = DspDROMVerOffset;

    for (i = u4Num >> 1; i != 0; i --)
    {
        tmp = AUD_ReadDROM (u1DspId, start ++);
        *u1ID_p ++ = tmp & 0xFF;
        *u1ID_p ++ = (tmp >> 8) & 0xFF;
    }

    return AUD_OK;
}
static BOOL _fgAudMwRealCtrl[AUD_DEC_MAX] = {0, 0, 0, 0};

void AUD_MWCtrl_Set(UINT8 u1DecId, BOOL fgPlay)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    VERIFY(x_sema_lock(_hMwCtrlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _fgAudMwRealCtrl[u1DecId] = fgPlay;

    VERIFY(x_sema_unlock(_hMwCtrlSema) == OSR_OK);
}

BOOL AUD_MWCtrl_Get(UINT8 u1DecId)
{
    BOOL fgRet = 0;

    AUD_DEC_ID_VALIDATE(u1DecId);

    VERIFY(x_sema_lock(_hMwCtrlSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    fgRet = _fgAudMwRealCtrl[u1DecId];

    VERIFY(x_sema_unlock(_hMwCtrlSema) == OSR_OK);

    return fgRet;
}
void AUD_SetLPCMFmt(UINT8 u1DecId, UINT32 u4PriData)
{
    // This function is for Program Stream Only.

    AUD_LPCM_SETTING_T rLpcmSetting;

    /*======================================================================================
    *               u4PriData (This is Specification)
    *
    *   [ Program Stream ]
    * sampling_freq         5   0x1F000000 (>>24) (internal definition)
    *
    * audio_emphasis_flag   1   0x800000 (>>23)
    * audio_mute_flag       1   0x400000 (>>22)
    * reserved              1   0x200000
    * audio_frame_number    5   0x1F0000 (>>16)
    *
    * quantization_word_len 2   0xC000   (>>14) (0 = 16bit, 1 = 20bit, 2 = 24bit,  3 = 8bit, 4 = 32bit(new, only from WAV))
    * sampling_freq         2   0x3000   (>>12) (0 = fs_48, 1 = fs_96, DVD spec.)
    *                                           (if == 3, then check bit28 ~ bit 24, internal definition)
    * reserved              1   0x0800
    * num of audio channels 3   0x0700   (>>8)
    *
    * drc control           8   0xFF
    *
    *
    *   [ Transport Stream ]
    * num of audio channels 4   0x0F000  (>>12) (0 = reserved. 1 = 2ch mono, 2 = reserverd,  3 = 2ch stereo)
    * sampling_freq         4   0x0F00   (>>8)  (1 = fs_48, 4 = fs_96, 5 = fs_192)
    * quantization_word_len 2   0x00C0   (>>6)  (1 = 16bit, 2 = 20bit, 3 = 24bit)
    *
    *
    *           u1LpcmDecodingType  (internal definition)
    *  bit 0: dvd audio / dvd-video,
    *  bit 1: non-wav / not dvd video (wav)
    *  bit 2: aob / vob
    *  bit 3: large / little endian
    *  bit 4: non / ADPCM
    *
    *
    *           Sampling Rate  (internal definition)
    *  low  (Grp2)   0 = fs_48  1 = fs_96   8 = fs_441   9 = fs_882
    *  high (Grp1)   0 = fs_48  1 = fs_96   2 = fs_192   3 = fs_24
    *                4 = fs_12  8 = fs_441  9 = fs_882  10 = fs_176
    *               11 = fs_22 12 = fs_11  16 = fs_32   19 = fs_16
    *               20 = fs_8  21 = fs_6   22 = fs_5
    *               ==> if Grp2 is 0xf, then Grp1 is taken.
    *
    *
    ============================================================================================*/

    UINT8 u1QuantWord;
    UINT8 u1SampRate;
    UINT8 u1Channels;
    UINT8 u1Drc;
    BOOL  fgTypePS = TRUE;
    UINT32 u4SwDmxType;
    UINT8 u1SwDmxSrcId;

    UNUSED(u1QuantWord);
    UNUSED(u1Drc);
    UNUSED(u1SampRate);
    UNUSED(u1Channels);
    UNUSED(fgTypePS);
    UNUSED(u4SwDmxType);
    UNUSED(u1SwDmxSrcId);

    AUD_GET_SWDMXSrcId(u1DecId, &u1SwDmxSrcId);
#ifdef ENABLE_MULTIMEDIA
#ifdef CC_53XX_SWDMX_V2
    SWDMX_GetInfo(u1SwDmxSrcId, eSWDMX_CONTAINER_TYPE, &u4SwDmxType, 0, 0);
    if ( (u4SwDmxType == SWDMX_FMT_MPEG2_TS) || (u4SwDmxType == SWDMX_FMT_MPEG2_TS_192)
         || (u4SwDmxType == SWDMX_FMT_MPEG2_TS_ZERO_192) || (u4SwDmxType == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)
         || (u4SwDmxType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) )
    {
        fgTypePS = FALSE;
    }
#endif //CC_53XX_SWDMX_V2
#endif //ENABLE_MULTIMEDIA

    //#if 1
    if (fgTypePS)
    {
        u1QuantWord = (u4PriData & (0xC000)) >> 14;
        u1Channels = (u4PriData & (0x0700)) >> 8;
        u1Drc = (u4PriData & (0xFF));

        u1SampRate = (u4PriData & (0x3000)) >> 12;
        if (u1SampRate == 3)
        {
            u1SampRate = (u4PriData & (0x1F000000)) >> 24;
            rLpcmSetting.u1LpcmFreqGrp1 = u1SampRate;
            rLpcmSetting.u1LpcmFreqGrp2 = 0xF;
        }
        else
        {
            rLpcmSetting.u1LpcmFreqGrp1 = 0;
            rLpcmSetting.u1LpcmFreqGrp2 = u1SampRate;
        }
    }
    //#else
    else // not PS, it's TS
    {
        u1Channels = (u4PriData & (0x0F000)) >> 12;
        switch (u1Channels)
        {
            case 1:
                u1Channels = 0; //mono
                break;
            case 2:
            case 3:
                u1Channels = 1; //stereo
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                u1Channels = 3; // 4 ch
                break;
            case 8:
            case 9:
                u1Channels = 5; // 6 ch
                break;
            case 10:
            case 11:
                u1Channels = 7; // 8 ch
                break;
            default:
                u1Channels = 1; //stereo
                break;
        }

        u1SampRate = (u4PriData & (0x0F00)) >> 8;
        if (u1SampRate == 4)
        {
            rLpcmSetting.u1LpcmFreqGrp1 = 0;
            rLpcmSetting.u1LpcmFreqGrp2 = 1;   //96K
        }
        else if (u1SampRate == 5)
        {
            rLpcmSetting.u1LpcmFreqGrp1 = 2;
            rLpcmSetting.u1LpcmFreqGrp2 = 0xF;   //192K
        }
        else
        {
            rLpcmSetting.u1LpcmFreqGrp1 = 0;
            rLpcmSetting.u1LpcmFreqGrp2 = 0;   //48K
        }

        u1QuantWord = (u4PriData & (0x0C0)) >> 6;
        if (u1QuantWord == 0)
        {
            u1QuantWord = 0;   //default is 16 bit.
        }
        else
        {
            u1QuantWord -= 1; //16,20 and 24 bit
        }
    }
    //#endif

    //LOG(9,"Q=0x%x, S=0x%x, C=0x%x, D=0x%x\n", u1QuantWord, u1SampRate, u1Channels, u1Drc);

    rLpcmSetting.u1LpcmBitShift = 0;
    rLpcmSetting.u1LpcmDrcValue = 0x80;
    rLpcmSetting.u1LpcmBitResGrp2 = 0;
    rLpcmSetting.u1LpcmDrcFlag = 0;

    rLpcmSetting.u2LpcmDecodingType = 0x5; // big endian, non-wav, vob
    rLpcmSetting.u1LpcmChAssignment = u1Channels;
    rLpcmSetting.u1LpcmBitResGrp1 = u1QuantWord;
    rLpcmSetting.u2ADPCMDecodingType = 0;
    if ((u1QuantWord == 2) && !fgTypePS) //TS 24bits big endian
    {
        rLpcmSetting.u2LpcmDecodingType |= 0x400; //24 bits PCM, bit 10: TS 24bits
    }
    
    AUD_SetLpcmTable(u1DecId, rLpcmSetting);
}

#if 1   // Unused
void AUD_SetLPCMFmtPS(UINT8 u1DecId, UINT32 u4PriData)
{
    // This function is for Program Stream Only.

    AUD_LPCM_SETTING_T rLpcmSetting;

    /*======================================================================================
    *               u4PriData (This is Specification)
    *
    *   [ Program Stream ]
    * sampling_freq         5   0x1F000000 (>>24) (internal definition)
    *
    * audio_emphasis_flag   1   0x800000 (>>23)
    * audio_mute_flag       1   0x400000 (>>22)
    * reserved              1   0x200000
    * audio_frame_number    5   0x1F0000 (>>16)
    *
    * quantization_word_len 2   0xC000   (>>14) (0 = 16bit, 1 = 20bit, 2 = 24bit,  3 = 8bit  )
    * sampling_freq         2   0x3000   (>>12) (0 = fs_48, 1 = fs_96, DVD spec.)
    *                                           (if == 3, then check bit28 ~ bit 24, internal definition)
    * reserved              1   0x0800
    * num of audio channels 3   0x0700   (>>8)
    *
    * drc control           8   0xFF
    *
    *
    *   [ Transport Stream ]
    * num of audio channels 4   0x0F000  (>>12) (0 = reserved. 1 = 2ch mono, 2 = reserverd,  3 = 2ch stereo)
    * sampling_freq         4   0x0F00   (>>8)  (1 = fs_48, 4 = fs_96, 5 = fs_192)
    * quantization_word_len 2   0x00C0   (>>6)  (1 = 16bit, 2 = 20bit, 3 = 24bit)
    *
    *
    *           u1LpcmDecodingType  (internal definition)
    *  bit 0: dvd audio / dvd-video,
    *  bit 1: non-wav / not dvd video (wav)
    *  bit 2: aob / vob
    *  bit 3: large / little endian
    *  bit 4: non / ADPCM
    *
    *
    *           Sampling Rate  (internal definition)
    *  low  (Grp2)   0 = fs_48  1 = fs_96   8 = fs_441   9 = fs_882
    *  high (Grp1)   0 = fs_48  1 = fs_96   2 = fs_192   3 = fs_24
    *                4 = fs_12  8 = fs_441  9 = fs_882  10 = fs_176
    *               11 = fs_22 12 = fs_11  16 = fs_32   19 = fs_16
    *               20 = fs_8
    *               ==> if Grp2 is 0xf, then Grp1 is taken.
    *
    *
    ============================================================================================*/

    UINT8 u1QuantWord;
    UINT8 u1SampRate;
    UINT8 u1Channels;
    UINT8 u1Drc;

    UNUSED(u1QuantWord);
    UNUSED(u1Drc);
    UNUSED(u1SampRate);
    UNUSED(u1Channels);


    {
        u1QuantWord = (u4PriData & (0xC000)) >> 14;
        u1Channels = (u4PriData & (0x0700)) >> 8;
        u1Drc = (u4PriData & (0xFF));

        u1SampRate = (u4PriData & (0x3000)) >> 12;
        if (u1SampRate == 3)
        {
            u1SampRate = (u4PriData & (0x1F000000)) >> 24;
            rLpcmSetting.u1LpcmFreqGrp1 = u1SampRate;
            rLpcmSetting.u1LpcmFreqGrp2 = 0xF;
        }
        else
        {
            rLpcmSetting.u1LpcmFreqGrp1 = 0;
            rLpcmSetting.u1LpcmFreqGrp2 = u1SampRate;
        }
    }

    //LOG(9,"Q=0x%x, S=0x%x, C=0x%x, D=0x%x\n", u1QuantWord, u1SampRate, u1Channels, u1Drc);

    rLpcmSetting.u1LpcmBitShift = 0;
    rLpcmSetting.u1LpcmDrcValue = 0x80;
    rLpcmSetting.u1LpcmBitResGrp2 = 0;
    rLpcmSetting.u1LpcmDrcFlag = 0;

    rLpcmSetting.u2LpcmDecodingType = 0x5; // big endian, non-wav, vob
    rLpcmSetting.u1LpcmChAssignment = u1Channels;
    rLpcmSetting.u1LpcmBitResGrp1 = u1QuantWord;
    rLpcmSetting.u2ADPCMDecodingType = 0;

    AUD_SetLpcmTable(u1DecId, rLpcmSetting);
}

void AUD_SetLPCMFmtTS(UINT8 u1DecId, UINT32 u4PriData)
{
    // This function is for Program Stream Only.

    AUD_LPCM_SETTING_T rLpcmSetting;

    /*======================================================================================
    *               u4PriData (This is Specification)
    *
    *   [ Program Stream ]
    * sampling_freq         5   0x1F000000 (>>24) (internal definition)
    *
    * audio_emphasis_flag   1   0x800000 (>>23)
    * audio_mute_flag       1   0x400000 (>>22)
    * reserved              1   0x200000
    * audio_frame_number    5   0x1F0000 (>>16)
    *
    * quantization_word_len 2   0xC000   (>>14) (0 = 16bit, 1 = 20bit, 2 = 24bit,  3 = 8bit  )
    * sampling_freq         2   0x3000   (>>12) (0 = fs_48, 1 = fs_96, DVD spec.)
    *                                           (if == 3, then check bit28 ~ bit 24, internal definition)
    * reserved              1   0x0800
    * num of audio channels 3   0x0700   (>>8)
    *
    * drc control           8   0xFF
    *
    *
    *   [ Transport Stream ]
    * num of audio channels 4   0x0F000  (>>12) (0 = reserved. 1 = 2ch mono, 2 = reserverd,  3 = 2ch stereo)
    * sampling_freq         4   0x0F00   (>>8)  (1 = fs_48, 4 = fs_96, 5 = fs_192)
    * quantization_word_len 2   0x00C0   (>>6)  (1 = 16bit, 2 = 20bit, 3 = 24bit)
    *
    *
    *           u1LpcmDecodingType  (internal definition)
    *  bit 0: dvd audio / dvd-video,
    *  bit 1: non-wav / not dvd video (wav)
    *  bit 2: aob / vob
    *  bit 3: large / little endian
    *  bit 4: non / ADPCM
    *
    *
    *           Sampling Rate  (internal definition)
    *  low  (Grp2)   0 = fs_48  1 = fs_96   8 = fs_441   9 = fs_882
    *  high (Grp1)   0 = fs_48  1 = fs_96   2 = fs_192   3 = fs_24
    *                4 = fs_12  8 = fs_441  9 = fs_882  10 = fs_176
    *               11 = fs_22 12 = fs_11  16 = fs_32   19 = fs_16
    *               20 = fs_8
    *               ==> if Grp2 is 0xf, then Grp1 is taken.
    *
    *
    ============================================================================================*/

    UINT8 u1QuantWord;
    UINT8 u1SampRate;
    UINT8 u1Channels;

    UNUSED(u1QuantWord);
    UNUSED(u1SampRate);
    UNUSED(u1Channels);

    {
        u1Channels = (u4PriData & (0x0F000)) >> 12;
#if 0
        if (u1Channels == 1)
        {
            u1Channels = 0; //mono
        }
        else
        {
            u1Channels = 1; //stereo
        }
#else
        switch (u1Channels)
        {
            case 1:
                u1Channels = 0; //mono
                break;
            case 2:
            case 3:
                u1Channels = 1; //stereo
                break;
            case 4:
            case 5:
            case 6:
            case 7:
                u1Channels = 3; // 4 ch
                break;
            case 8:
            case 9:
                u1Channels = 5; // 6 ch
                break;
            case 10:
            case 11:
                u1Channels = 7; // 8 ch
                break;
            default:
                u1Channels = 1; //stereo
                break;
        }
#endif

        u1SampRate = (u4PriData & (0x0F00)) >> 8;
        if (u1SampRate == 1)
        {
            u1SampRate = 0; //48k
        }
        rLpcmSetting.u1LpcmFreqGrp1 = 0;
        rLpcmSetting.u1LpcmFreqGrp2 = u1SampRate;

        u1QuantWord = (u4PriData & (0x0C0)) >> 6;
        u1QuantWord -= 1;
    }

    //LOG(9,"Q=0x%x, S=0x%x, C=0x%x, D=0x%x\n", u1QuantWord, u1SampRate, u1Channels, u1Drc);

    rLpcmSetting.u1LpcmBitShift = 0;
    rLpcmSetting.u1LpcmDrcValue = 0x80;
    rLpcmSetting.u1LpcmBitResGrp2 = 0;
    rLpcmSetting.u1LpcmDrcFlag = 0;

    rLpcmSetting.u2LpcmDecodingType = 0x5; // big endian, non-wav, vob
    rLpcmSetting.u1LpcmChAssignment = u1Channels;
    rLpcmSetting.u1LpcmBitResGrp1 = u1QuantWord;
    rLpcmSetting.u2ADPCMDecodingType = 0;

    AUD_SetLpcmTable(u1DecId, rLpcmSetting);
}

void AUD_SetLPCMFmtWIFI(UINT8 u1DecId, UINT32 u4PriData)
{   // This function is for Program Stream Only.

    AUD_LPCM_SETTING_T rLpcmSetting;

    /*======================================================================================
    *               u4PriData (This is Specification)
    *
    * sub_stream_id                              8 (skip before this function)
    * number_of_frame_headers            8 (skip before this function)
    * reserved                                      7   
    * audio_emphasis_flag                     1   
    * quantization_word_len                   2
    * sampling_freq                               3
    * num of audio channels                   3
    ============================================================================================*/

    UINT8 u1QuantWord;
    UINT8 u1SampRate;
    UINT8 u1Channels;

    UNUSED(u1QuantWord);
    UNUSED(u1SampRate);
    UNUSED(u1Channels);

    {
        u1Channels = (u4PriData & (0x07)) >> 0;

        switch (u1Channels)
        {
        case 0:
            u1Channels = 0; //mono
            break;
        case 1:
            u1Channels = 1; //stereo
            break;
        default:
            Printf("[AOMX][OMX] WIFI LPCM header channel error\n");
            u1Channels = 1; //stereo
            break;
        }


        u1SampRate = (u4PriData & (0x38)) >> 3;
        if (u1SampRate == 1)//44.1K
        {
            rLpcmSetting.u1LpcmFreqGrp1 = 0x8;
            rLpcmSetting.u1LpcmFreqGrp2 = 0xF;
        }
        else if (u1SampRate == 2)//48K
        {
            rLpcmSetting.u1LpcmFreqGrp1 = 0;
            rLpcmSetting.u1LpcmFreqGrp2 = 0;
        }
        else//default 48K
        {
            rLpcmSetting.u1LpcmFreqGrp1 = 0;
            rLpcmSetting.u1LpcmFreqGrp2 = 0;
            Printf("[AOMX][OMX] WIFI LPCM header sample rate error\n");
        }

        u1QuantWord = (u4PriData & (0x0C0)) >> 6;
        if (u1QuantWord != 0)
        {
            u1QuantWord = 0;//16bit
            Printf("[AOMX][OMX] WIFI LPCM header quantization_wor error\n");
        }

    }

    //LOG(9,"Q=0x%x, S=0x%x, C=0x%x, D=0x%x\n", u1QuantWord, u1SampRate, u1Channels, u1Drc);

    rLpcmSetting.u1LpcmBitShift = 0;
    rLpcmSetting.u1LpcmDrcValue = 0x80;
    rLpcmSetting.u1LpcmBitResGrp2 = 0;
    rLpcmSetting.u1LpcmDrcFlag = 0;

    rLpcmSetting.u2LpcmDecodingType = 0x5; // big endian, non-wav, vob
    rLpcmSetting.u1LpcmChAssignment = u1Channels;
    rLpcmSetting.u1LpcmBitResGrp1 = u1QuantWord;
    rLpcmSetting.u2ADPCMDecodingType = 0;

    AUD_SetLpcmTable(u1DecId, rLpcmSetting);
}
#endif

BOOL AUD_IsLpcmFmtPS(void)
{
    BOOL fgFlag = FALSE;

    // if LPCM from program stream ?
    fgFlag = _fgLpcmTpyePS;

    return fgFlag;
}

void AUD_PCM_Detect_Raw_Mute(BOOL fgFlag)
{
    _fgPCMDetectRawMute = fgFlag;
}

BOOL AUD_Is_PCMDetectRaw(void)
{
    return _fgPCMDetectRawMute;
}

//-----------------------------------------------------------------------------
// AUD_MM_Set_Dec_Fmt_Conflict
//
/** For SWDMX callback AUD_DRV to notify that audio fomat is conflict.
 *  e.x. mpeg2 program stream with mp3 audio codec inside.
 *
 *  @param[in]        u1DecId          Decoder ID
 *                    fgFlag           Enable Flag
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_MM_Set_Dec_Fmt_Conflict(UINT8 u1DecId, BOOL fgFlag)
{
#ifdef CC_AUD_DDI
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_DRVGetStreamFrom(AUD_DSP0, u1DecId, &eAudDecStreamFrom);
    if  (eAudDecStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
#endif
    {
        _bfgMMDecFmtConflict[u1DecId] = fgFlag;
    }
}

BOOL AUD_Is_MMDecFmtConflict(UINT8 u1DecId)
{
    return _bfgMMDecFmtConflict[u1DecId];
}

//-----------------------------------------------------------------------------
// AUD_MM_Set_Dec_Fmt_Scramble
//
/** For SWDMX callback AUD_DRV to notify that audio fomat is scramble.
 *  e.x. ts file with scramble audio data
 *
 *  @param[in]        u1DecId          Decoder ID
 *                    fgFlag           Enable Flag
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_MM_Set_Dec_Fmt_Scramble(UINT8 u1DecId, BOOL fgFlag)
{
    if (_bfgMMDecFmtScramble[u1DecId] != fgFlag)
    {
        _bfgMMDecFmtScramble[u1DecId] = fgFlag;
        VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_AVSYNC));
    }
}

BOOL AUD_Is_MMDecFmtScramble(UINT8 u1DecId)
{
    return _bfgMMDecFmtScramble[u1DecId];
}

//-----------------------------------------------------------------------------
// AUD_MM_Set_Avsync_event
//
/** For SWDMX callback AUD_DRV to notify that audio avsync event.
 *  e.x. seek to file end could trigger audio check no data and send eos without wait for 4.5s time out
 *
 *  @param[in]
 *
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_MM_Set_Avsync_event(UINT8 u1DecId)
{
    VERIFY(AUD_DRVCmd(AUD_DSP0, u1DecId, AUD_CMD_AVSYNC));
}

//-----------------------------------------------------------------------------
// AUD_MMClearEOSFlag
//
/** Clear Audio Multimedia EOS Flag
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)
 */
//-----------------------------------------------------------------------------
void AUD_MMClearEOSFlag(UINT8 u1DecId)
{
    _AudClearEOSFlag(u1DecId);
}

//-----------------------------------------------------------------------------
// AUD_MMClearPesCnt
//
/** Clear Audio Multimedia Pes Count
 *
 *  @param      u1DecId          Decoder ID (AUD_DEC_MAIN)
 */
//-----------------------------------------------------------------------------
void AUD_MMClearPesCnt(UINT8 u1DecId)
{
    AUD_DrvClearPesCnt(u1DecId);
}

//-----------------------------------------------------------------------------
// AUD_MTS_CFO_IDX
//
/** For CID ATD query usage
 *
 *  @param[in]        none
 *
 *  @retval number of grids of CFO units
 */
//-----------------------------------------------------------------------------
#define DRAM_QMIX_ADDR 0xe5f
INT32 AUD_MTS_CFO_IDX(void)
{
    UINT32 u4Qmix, u4DcVal, u4SndSys, eDec2Type, u4SndSysAddr;
    INT32 i4ShiftBlocks;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;

    AUD_GetDecodeType(AUD_DSP0, AUD_DEC_MAIN, &eStreamFrom, &eDecType);

    DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    //eDec2Type = DSP_CommDramRead(ADDR_D2RC_DEC2RAM_CODE_VERSION) & 0x0F00; // decoder2 version
    eDec2Type = DSP_CommDramRead(AUD_DSP0, 0x3) & 0x0F00; // decoder2 version
    if ((eDecType == AUD_FMT_MTS) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_MAIN)) //BTSC decoder 1
    {
        if (!AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_MTS))
        {
            LOG(11, "\n\nNot opening carrier shift mode for MTS\n\n");
            return 999;
        }

        //u4Qmix = u4ReadDspSram(0x2b2);// Q-mixer, used to judge if shifted (0x2aaab0 is no shift)
        u4Qmix = DSP_CommDramRead(AUD_DSP0, DRAM_QMIX_ADDR); //Use dram is more stable than sram. (DSP code releated.)

        u4DcVal = u4ReadDspSram(AUD_DSP0, CFO_ACC_PROB_SRAM_ADDR);
        if (u4DcVal > 0x800000)
        {
            u4DcVal = (u4DcVal ^ 0xffffff) + 1;
            i4ShiftBlocks = (INT32)u4DcVal * -1;
            i4ShiftBlocks = (i4ShiftBlocks - 0x39580) / 0x72b00;
        }
        else
        {
            i4ShiftBlocks = (INT32)u4DcVal;
            i4ShiftBlocks = (i4ShiftBlocks + 0x39580) / 0x72b00;
        }

        if (u4Qmix < 0x2aaab0) //bigger frequency, smaller value
        {
            i4ShiftBlocks += 1;
        }

        if (u4Qmix > 0x2aaab0)
        {
            i4ShiftBlocks -= 1;
        }

        LOG(11, "\n\n BTSC carrier shift %d steps\n\n", i4ShiftBlocks);
    }
    else if ((eDec2Type == 0x0600) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_AUX)) //BTSC decoder 2
    {
        LOG(11, "\n\nNot opening carrier shift mode Query for MTS_DEC2\n\n");
        return 999;
    }
    else if (((eDecType == AUD_FMT_A2) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_MAIN))
               ||((eDec2Type == 0x0700) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_AUX))) //A2 decoder 2
    {
        if (!AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_A2))
        {
            LOG(11, "\n\nNot opening carrier shift mode for A2\n\n");
            return 999;
        }
        if (AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_MAIN))
        {
            u4SndSysAddr = 0x150;
        }
        else
        {
            u4SndSysAddr = 0x1A0;
        }

        u4SndSys = DSP_CommDramRead(AUD_DSP0, u4SndSysAddr)>>8;
        //u4Qmix = u4ReadDspSram(0x2b9);// Q-mixer, used to judge if shifted (0x2aaab0 is no shift)
        u4Qmix = DSP_CommDramRead(AUD_DSP0, DRAM_QMIX_ADDR);

        u4DcVal = u4ReadDspSram(AUD_DSP0, CFO_ACC_PROB_SRAM_ADDR);
        if (u4DcVal > 0x800000)
        {
            u4DcVal = (u4DcVal ^ 0xffffff) + 1;
            i4ShiftBlocks = (INT32)u4DcVal * -1;
            i4ShiftBlocks = (i4ShiftBlocks - 0x52000) / 0x0A4000;
        }
        else
        {
            i4ShiftBlocks = (INT32)u4DcVal;
            i4ShiftBlocks = (i4ShiftBlocks + 0x52000) / 0x0A4000;
        }
        if (u4SndSys & 0x1)
        {
            LOG(11, "\n\nNot support CFO query for A2_M\n\n");
            return 999;
        }
        else if (u4SndSys & 0x2)
        {
            //A2 B/G
            if (u4Qmix < 0x3425F0) //bigger frequency, smaller value
            {
                i4ShiftBlocks += 1;
            }
            if (u4Qmix > 0x3425F0)
            {
                i4ShiftBlocks -= 1;
            }
        }
        else if (u4SndSys & 0x1C)
        {
            //A2 D/K series
            if (u4Qmix < 0x3DA130) //bigger frequency, smaller value
            {
                i4ShiftBlocks += 1;
            }
            if (u4Qmix > 0x3DA130)
            {
                i4ShiftBlocks -= 1;
            }
        }
        else
        {
            LOG(11, "\n\nIncorrect  A2 sound system\n\n");
            LOG(11, "u4SndSys = %x\n", u4SndSys);
            return 999;
        }
        LOG(7, "\n\n A2 carrier shift %d steps\n\n", i4ShiftBlocks);
    }
    else if (((eDecType == AUD_FMT_PAL) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_MAIN))
               ||((eDec2Type == 0x0900) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_AUX))) //PAL decoder 2
    {
        if (!AUD_DspGetCarrierShiftMode(AUD_ATV_DECODER_PAL))
        {
            LOG(11, "\n\nNot opening carrier shift mode for PAL\n\n");
            return 999;
        }
        if (AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_MAIN))
        {
            u4SndSysAddr = 0x150;
        }
        else
        {
            u4SndSysAddr = 0x1A0;
        }

        u4SndSys = DSP_CommDramRead(AUD_DSP0, u4SndSysAddr)>>8;

        //u4Qmix = u4ReadDspSram(0x2b9);// Q-mixer, used to judge if shifted (0x2aaab0 is no shift)
        u4Qmix = DSP_CommDramRead(AUD_DSP0, DRAM_QMIX_ADDR);
        if ((u4Qmix != 0x3DA130) && (u4Qmix != 0x38E390) && (u4Qmix != 0x3425F0))
        {
            LOG(8, "u4Qmix = 0x%x\n", u4Qmix);
        }

        u4DcVal = u4ReadDspSram(AUD_DSP0, CFO_ACC_PROB_SRAM_ADDR);
        if (u4DcVal > 0x800000)
        {
            u4DcVal = (u4DcVal ^ 0xffffff) + 1;
            i4ShiftBlocks = (INT32)u4DcVal * -1;
            i4ShiftBlocks = (i4ShiftBlocks - 0x14F80) / 0x029F00;
        }
        else
        {
            i4ShiftBlocks = (INT32)u4DcVal;
            i4ShiftBlocks = (i4ShiftBlocks + 0x14F80) / 0x029F00;
        }
        if (u4SndSys & 0x1)
        {
            //PAL I
            UINT8 cal_cnt = 0;
            while (u4Qmix != 0x38E390)
            {
                if (u4Qmix < 0x38E390)
                {
                    u4Qmix += 0x26D0;
                    i4ShiftBlocks += 1;
                    cal_cnt++;
                }
                else if (u4Qmix > 0x38E390)
                {
                    u4Qmix -= 0x26D0;
                    i4ShiftBlocks -= 1;
                    cal_cnt++;
                }
                if (cal_cnt >= 25)
                {
                    i4ShiftBlocks = 0;
                    break;
                }
            }
        }
        else if (u4SndSys & 0x2)
        {
            //PAL B/G
            UINT8 cal_cnt = 0;
            while (u4Qmix != 0x3425F0)
            {
                if (u4Qmix < 0x3425F0)
                {
                    u4Qmix += 0x26D0;
                    i4ShiftBlocks += 1;
                    cal_cnt++;
                }
                else if (u4Qmix > 0x3425F0)
                {
                    u4Qmix -= 0x26D0;
                    i4ShiftBlocks -= 1;
                    cal_cnt++;
                }
                if (cal_cnt >= 25)
                {
                    i4ShiftBlocks = 0;
                    break;
                }
            }
        }
        else if (u4SndSys & 0x4)
        {
            //PAL D/K
            UINT8 cal_cnt = 0;
            while (u4Qmix != 0x3DA130)
            {
                if (u4Qmix < 0x3DA130)
                {
                    u4Qmix += 0x26D0;
                    i4ShiftBlocks += 1;
                    cal_cnt++;
                }
                else if (u4Qmix > 0x3DA130)
                {
                    u4Qmix -= 0x26D0;
                    i4ShiftBlocks -= 1;
                    cal_cnt++;
                }
                if (cal_cnt >= 25)
                {
                    i4ShiftBlocks = 0;
                    break;
                }
            }
        }
        else
        {
            LOG(11, "\n\nIncorrect  PAL sound system for CFO query\n\n");
            LOG(11, "u4SndSys = %x\n", u4SndSys);
            return 999;
        }
        LOG(11, "\n\n PAL carrier shift %d steps\n\n", i4ShiftBlocks);
    }
    else
    {
        LOG(11, "\nDecoder Type Incorrect(not BTSC, A2, PAL) for CFO query!\n\n");
        LOG(11, " eDecType = %d, eDec2Type  %d\n", eDecType, eDec2Type);
        return 999;
    }

    return i4ShiftBlocks;
}
//-----------------------------------------------------------------------------
// AudCmdAtvHdevDetQuery
//
/** For CID ATD query usage
 *
 *  @param[in]        none
 *
 *  @retval TRUE(Hdev detected) / FALSE(Hdev not detected) /999 (Invalid)
 */
//-----------------------------------------------------------------------------
INT32 AudCmdAtvHdevDetQuery(void)
{

    UINT32 eDec2Type, i4HdevDetResult, u4DramValue;
    AUD_DEC_STREAM_FROM_T eStreamFrom;
    AUD_FMT_T eDecType;

    AUD_GetDecodeType(AUD_DSP0, AUD_DEC_MAIN, &eStreamFrom, &eDecType);

    DSP_FlushInvalidateDCache(AUD_DSP0, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);

    //eDec2Type = DSP_CommDramRead(ADDR_D2RC_DEC2RAM_CODE_VERSION) & 0x0F00; // decoder2 version
    eDec2Type = DSP_CommDramRead(AUD_DSP0, 0x3) & 0x0F00; // decoder2 version
    if ((eDecType == AUD_FMT_MTS) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_MAIN)) //BTSC decoder 1
    {
        LOG(11, "\n\nQuery for Hdev mode detection  of PAL decoder1...\n\n");
         i4HdevDetResult = u4ReadDspSram(AUD_DSP0, 0x15fa);
        LOG(11, "  ovm_det_result  = 0x%x \n", i4HdevDetResult);
        return i4HdevDetResult;
    }
    else if ((eDec2Type == 0x0600) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_AUX)) //BTSC decoder 2
    {
        LOG(11, "\n\nNot opening Hdev detection Query for MTS_DEC2\n\n");
        return 999;
    }
    else if ((eDecType == AUD_FMT_A2) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_MAIN)) //A2 decoder1
    {
        LOG(11, "\n\nQuery for Hdev mode detection  of A2 decoder1...\n\n");
        return 0;
    }
    else if ((eDec2Type == 0x0700) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_AUX)) //A2 decoder 2
    {
        LOG(11, "\n\nQuery for Hdev mode detection  of A2 decoder2...\n\n");
        return 0;
    }
    else if ((eDecType == AUD_FMT_PAL) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_MAIN)) //PAL decoder
    {
        u4DramValue = DSP_DecDramRead(AUD_DSP0, 9);
        LOG(11, "\n\nQuery for Hdev mode detection  of PAL decoder1...\n\n");
        LOG(11, "   hdev_a2_result  = 0x%x \n", u4DramValue);
        i4HdevDetResult = (u4DramValue & 0x0100) >> 8;
        return i4HdevDetResult;
    }
    else if ((eDec2Type == 0x0900) && AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_AUX)) //PAL decoder 2
    {
        //ReadDspDec2CacheDram(9,u4DramValue);
        u4DramValue = DSP_Dec2DramRead(AUD_DSP0, 9);
        LOG(11, "\n\nQuery for Hdev mode detection  of PAL decoder2...\n\n");
        LOG(11, "   hdev_a2_result  = 0x%x \n", u4DramValue);
        i4HdevDetResult = (u4DramValue & 0x0100) >> 8;
        return i4HdevDetResult;
    }
    else
    {
        LOG(11, "\nDecoder Type Incorrect(not BTSC, A2, PAL) for ATV Hdev mode query!\n\n");
        LOG(11, " eDecType = %d, eDec2Type  %d\n", eDecType, eDec2Type);
        return 999;
    }

}

#ifdef DSP_FORCE_RESET_ENABLE
//-----------------------------------------------------------------------------
// AUD_DSPCmdSema_Lock
//
/** Brief of AUD_DSPCmdSema_Lock.
 *  Details of AUD_DSPCmdStop (optional).
 *
 *  @param
 *
 *  @retval
 */
//-----------------------------------------------------------------------------
void AUD_DSPCmdSema_Lock(void)              //  -- DSP Force --
{
    VERIFY(x_sema_lock(_hSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}
#endif

#if defined(DSP_FORCE_RESET_ENABLE) || defined(CC_FAST_INIT) || defined(CC_SUPPORT_STR) || defined(__MODEL_slt__)
void AUD_DSPCmdSema_UnLock(void)
{
    VERIFY(x_sema_unlock(_hSema) == OSR_OK);
}
#endif

UINT8 AUD_GetPIPVideoPath(void)
{
    UNUSED(AUD_GetPIPVideoPath);
    return AUD_DrvGetPIPVideoPath();
}

#ifdef CC_AUD_HDMI_SPDIF_CFG    ///[Joel] 20101011
///#define DBG_LVL     2
void AUD_HdmiCfgAnalogOutput(AUD_OUTPUT_STATE u1OutputState)
{
    _u1OutputState = u1OutputState;

    switch (u1OutputState)
    {
        case DigitalMute_AnalogMute:
            AUD_SPDIF_Output_HDMI_Rx(FALSE);
            AUD_SPDIF_Output_Mute(TRUE);
            AUD_DspHdmiMuteAnalogOutput(AUD_DEC_MAIN, TRUE);
            LOG(6, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            LOG(6, "**** DigitalMute_AnalogMute **** \n");
            LOG(6, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            break;
        case DigitalUnMute_AnalogMute:
            AUD_SPDIF_Output_HDMI_Rx(TRUE);
            AUD_SPDIF_Output_Mute(FALSE);
            AUD_DspHdmiMuteAnalogOutput(AUD_DEC_MAIN, TRUE);
            LOG(6, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            LOG(6, "**** DigitalUnMute_AnalogMute **** \n");
            LOG(6, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            break;
        case DigitalUnMute_AnalogUnMute:
            AUD_SPDIF_Output_HDMI_Rx(TRUE);
            AUD_SPDIF_Output_Mute(FALSE);
            AUD_DspHdmiMuteAnalogOutput(AUD_DEC_MAIN, FALSE);
            LOG(6, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            LOG(6, "**** DigitalUnMute_AnalogUnMute **** \n");
            LOG(6, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            break;
        case DigitalMute_AnalogUnMute:
            AUD_SPDIF_Output_HDMI_Rx(FALSE);
            AUD_SPDIF_Output_Mute(TRUE);
            AUD_DspHdmiMuteAnalogOutput(AUD_DEC_MAIN, FALSE);
            LOG(6, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            LOG(6, "**** DigitalMute_AnalogUnMute **** \n");
            LOG(6, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
            break;
        default:
            AUD_SPDIF_Output_HDMI_Rx(FALSE);
            AUD_SPDIF_Output_Mute(FALSE);
            AUD_DspHdmiMuteAnalogOutput(AUD_DEC_MAIN, FALSE);
            break;
    }
}
#endif

UINT32 AUD_GetTargetCountry(void)
{
    UINT32 _u4AudTargetCountry;
    UINT32 u4Ret;

    UNUSED(u4Ret);

#ifdef NEW_COUNTRY_TYPE // MT5365, MT5395
    _u4AudTargetCountry = GetTargetCountryMask(COUNTRY_AUDIO_MASK);
    switch (_u4AudTargetCountry)
    {
        case COUNTRY_AUD_US:
            u4Ret = COUNTRY_US;
            break;
        case COUNTRY_AUD_EU:
            u4Ret = COUNTRY_EU;
            break;
        case COUNTRY_AUD_TW:
            u4Ret = COUNTRY_TW;
            break;
        case COUNTRY_AUD_JP:
            u4Ret = COUNTRY_JP;
            break;
        case COUNTRY_AUD_BZ:
            u4Ret = COUNTRY_BZ;
            break;
        default:
            u4Ret = COUNTRY_EU;
            break;
    }
    return u4Ret;

#else                   // MT5363, MT5388 and old ICs

    _u4AudTargetCountry = GetTargetCountry();
#ifdef COUNTRY_FROM_EEPROM
    _u4AudTargetCountry &= COUNTRY_AUDIO_MASK;
    _u4AudTargetCountry = _u4AudTargetCountry >> 5;
#endif
    return _u4AudTargetCountry;
#endif // NEW_COUNTRY_TYPE
}

#ifdef CC_AUD_SETUP_MELODY
void AUD_PlaySetupMelody(UINT8 u1DecId, UINT32 u4Channel, UINT32 u4RepeatCount)
{
    AUD_CLIP_TYPE_T rAudClip;
    AUD_SETUP_MELODY_TABLE_T* rAudSetupMelody;
    UINT8 u1Volume;

    _fgSetupPlayFinish = FALSE;

    x_memset((VOID*)VIRTUAL((UINT32)&rAudClip), 0, sizeof(AUD_CLIP_TYPE_T));

    if (u1DecId >= AUD_DEC_NUM)
    {
        u1DecId = AUD_DEC_MAIN;
    }

    rAudClip.u4OutputChlSelect = u4Channel;
    rAudClip.u4RepeatCount = u4RepeatCount;

    rAudClip.fgPlayTestTone = FALSE;
    rAudClip.eAudFmt = AUD_FMT_MP3;

    //Aud_GetSetupMelodyInfo(&rAudClip);
    rAudSetupMelody = (AUD_SETUP_MELODY_TABLE_T*)DRVCUST_OptGet(eAudioLoaderMusicData);
    u1Volume = DRVCUST_OptGet(eAudioPowerOnMusicVol);

    if (rAudSetupMelody == 0)
    {
        LOG(1, "No setup melody is prepared !!!!\n");
        _fgSetupPlayFinish = TRUE;
        return;
    }
#ifdef  LINUX_TURNKEY_SOLUTION
    pMelody_buf = (void*)x_mem_alloc((UINT32)rAudSetupMelody->u4Size);
    u1Melodyflag = 1;
    if (pMelody_buf == NULL)
    {
        ASSERT(0);
        printf("allocate fail!\n");
    }
#endif

#ifdef  LINUX_TURNKEY_SOLUTION
    x_memcpy((VOID*)VIRTUAL((UINT32)pMelody_buf), (UINT8*)rAudSetupMelody->prAudSetupMelody,
             rAudSetupMelody->u4Size);
    rAudClip.eMemBuf.pData = pMelody_buf;
#else
    rAudClip.eMemBuf.pData = (UINT8*)rAudSetupMelody->prAudSetupMelody;
#endif
    rAudClip.eMemBuf.u4Length = rAudSetupMelody->u4Size;

    AUD_DspSetMelodyVol(u1Volume);
    //_aau1ChannelVolume[AUD_DEC_MAIN][AUD_CH_ALL] = u1Volume;

    AUD_DspAVCEnable(AUD_DEC_MAIN, 0);
    _AUD_IgnoreDecNotify(u1DecId, TRUE);
    _AUD_IgnoreSetupPlayNotify(TRUE);
    AUD_SetAudClip(AUD_DSP0, u1DecId, &rAudClip);
    AUD_PlayAudClip(AUD_DSP0, u1DecId, rAudClip.u4OutputChlSelect,
                    rAudClip.fgPlayTestTone);
    _AUD_IgnoreDecNotify(u1DecId, FALSE);
}

UINT8 AUD_GetSetupMelodyVol(void)
{
    return (UINT8)DRVCUST_OptGet(eAudioPowerOnMusicVol);

}
BOOL AUD_IsSetupMelodyFinish(void)
{
    return _fgSetupPlayFinish;
}

void AUD_SetSetupPlayStatue(BOOL fgIsFinish)
{
    _fgSetupPlayFinish = fgIsFinish;
#ifdef  LINUX_TURNKEY_SOLUTION
    if (fgIsFinish == TRUE)
    {
        if (u1Melodyflag == 1)
        {
            x_mem_free(pMelody_buf);
            u1Melodyflag = 0;
        }
    }
#endif
}

void AUD_StopSetupMelody(UINT8 u1DecId)
{
    if (u1DecId >= AUD_DEC_NUM)
    {
        u1DecId = AUD_DEC_MAIN;
    }

    _AUD_IgnoreDecNotify(u1DecId, TRUE);
    AUD_StopAudClip(AUD_DSP0, u1DecId);
    _AUD_IgnoreDecNotify(u1DecId, FALSE);
}

#endif /*CC_AUD_SETUP_MELODY*/


//================== Begin of Audio Flash AQ Functions ===============================

INT32 AUD_FlashAQReadEx(UINT8 u1AQPartition, UINT32 u4Offset, UINT8 *u1Data, UINT32 u4Length)
{
    //UINT8 u1AQPartition;
    //u1AQPartition = (UINT8)DRVCUST_OptGet(eAudioFlashAQPartition);
    if (u1AQPartition == 0xFF)
    {
        Printf("AudioFlashAQPartition is not defined !!\n");
        return -1;
    }
    
    return (STORG_SyncRead(u1AQPartition, u4Offset, (VOID*)u1Data, u4Length));
}
#ifdef CC_MAPLE_CUST_DRV     
CUST_AUD_SOUND_SETTING_T gSoundSetting = {0};
#endif

#ifdef SUPPORT_FLASH_AQ

FLASH_AQ_T rAudFlashAQ;

#if MULTIPLE_FLASH_AQ
extern UINT32 GetCurrentPanelIndex(void);
extern UINT16 vApiFlashPqGetQtyId(UINT8 bIndex);
#ifdef CC_ENABLE_MTK_MODEL_INDEX
extern UINT16 vApiFlashaqGetQtyId(UINT8 bIndex);
extern UINT32 GetCurrentAQIndex(void);
#endif
#endif

void AUD_FlashAQInitEx(void)
{
    UINT8 u1Partition;
    UINT32 u4ChkSum = 0;
    UINT32 u4Tmp;
    UINT32 u4Size;
#if MULTIPLE_FLASH_AQ
    UINT16 u2FlashAqIndex;
    UINT32 u4Offset;
#endif
#ifdef CC_SUPPORT_STR
    static BOOL fgInit = FALSE;
    if (!fgInit)
    {
#endif

        u1Partition = (UINT8)DRVCUST_OptGet(eAudioFlashAQPartition);
            
#ifndef __MODEL_slt__         
#if MULTIPLE_FLASH_AQ
#ifdef CC_ENABLE_MTK_MODEL_INDEX
        u2FlashAqIndex = vApiFlashaqGetQtyId(GetCurrentAQIndex());
#else
        u2FlashAqIndex = vApiFlashPqGetQtyId(GetCurrentPanelIndex());
#endif
        u4Size = ((UINT32)(&(rAudFlashAQ.u1ChkSum[3])) - (UINT32)(&(rAudFlashAQ.au1AudId[0])) + 1 - FLASH_AQ_HDR_OFFSET);
        u4Offset = FLASH_AQ_HDR_OFFSET + u2FlashAqIndex * u4Size;
        AUD_FlashAQReadEx(u1Partition, 0, (UINT8*)(&rAudFlashAQ), FLASH_AQ_HDR_OFFSET); //Read 'aqda' and vendor_version ID
        //AUD_FlashAQReadEx(u1Partition, u4Offset, (UINT8*)(&rAudFlashAQ)+FLASH_AQ_HDR_OFFSET, u4Size);
        AUD_FlashAQReadEx(u1Partition, u4Offset, (UINT8*)(rAudFlashAQ.aai2AudInputSrcVol), u4Size);
        for (u4Tmp = 0; u4Tmp < u4Size - 4; u4Tmp++)
        {
            //u4ChkSum += *((UINT8*)(&rAudFlashAQ)+FLASH_AQ_HDR_OFFSET+u4Tmp);
            u4ChkSum += *((UINT8*)(rAudFlashAQ.aai2AudInputSrcVol) + u4Tmp);
        }
#else
        u4Size = ((UINT32)(&(rAudFlashAQ.u1ChkSum[3])) - (UINT32)(&(rAudFlashAQ.au1AudId[0])) + 1);
                 AUD_FlashAQReadEx(u1Partition, 0, (UINT8*)(&rAudFlashAQ), u4Size);
        for (u4Tmp = 0; u4Tmp < u4Size - 4; u4Tmp++)
        {
            u4ChkSum += *((UINT8*)(&rAudFlashAQ) + u4Tmp);
        }
#endif

#else
        Printf("!!!! [SLT] skip AUD_FlashAQInitEx \n\n");
#endif //__MODEL_slt__

#ifndef __MODEL_slt__   
        if ((rAudFlashAQ.au1AudId[0] != 'a') ||
            (rAudFlashAQ.au1AudId[1] != 'q') ||
            (rAudFlashAQ.au1AudId[2] != 'd') ||
            (rAudFlashAQ.au1AudId[3] != 'a') ||
            (((u4ChkSum >> 0) & 0xff) != rAudFlashAQ.u1ChkSum[0]) ||
            (((u4ChkSum >> 8) & 0xff) != rAudFlashAQ.u1ChkSum[1]) ||
            (((u4ChkSum >> 16) & 0xff) != rAudFlashAQ.u1ChkSum[2]) ||
            (((u4ChkSum >> 24) & 0xff) != rAudFlashAQ.u1ChkSum[3]))
#else
        if (1)
            
#endif //__MODEL_slt__              
            
        {
            UINT32 u4Ptr;

            Printf("!!!! AUD_FlashAQInitEx - Partition: %d AQ ID: %d Offset: 0x%x size: %d",
                   u1Partition, u2FlashAqIndex, u4Offset, u4Size);
            Printf("!!!! AudioFlashAQPartition not update to flash correctly - %s !!!!\n",
                   ((rAudFlashAQ.au1AudId[0] != 'a') ||
                    (rAudFlashAQ.au1AudId[1] != 'q') ||
                    (rAudFlashAQ.au1AudId[2] != 'd') ||
                    (rAudFlashAQ.au1AudId[3] != 'a')) ?
                   "aqda not match" : "checksum not match");

            VERIFY(DRVCUST_OptQuery(eAudioInputVolTable, (UINT32*)&u4Ptr) == 0);
            x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.aai2AudInputSrcVol), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.aai2AudInputSrcVol));

            VERIFY(DRVCUST_OptQuery(eAudioVolTable, (UINT32*)&u4Ptr) == 0);
            x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.au4VolumeTable), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.au4VolumeTable));
#ifdef CC_AUD_NEW_CV_TABLE
            VERIFY(DRVCUST_OptQuery(eAudioChannelVolTable, (UINT32*)&u4Ptr) == 0);
            x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.au4ChannelVolumeTable), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.au4ChannelVolumeTable));
#endif

            VERIFY(DRVCUST_OptQuery(eAudioChlVolExtraGain, (UINT32*)&u4Ptr) == 0);
            x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.aai1AudChVolExtraGain), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.aai1AudChVolExtraGain));

            VERIFY(DRVCUST_OptQuery(eAudioAvcTarget, (UINT32*)&u4Ptr) == 0);
            x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcTarget), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcTarget));

            VERIFY(DRVCUST_OptQuery(eAudioAvcGainUp, (UINT32*)&u4Ptr) == 0);
            x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcGainUp), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcGainUp));

            VERIFY(DRVCUST_OptQuery(eAudioAvcAdjustRate, (UINT32*)&u4Ptr) == 0);
            x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcAdjRate), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcAdjRate));

            VERIFY(DRVCUST_OptQuery(eAudioAvcUiAdjustRate, (UINT32*)&u4Ptr) == 0);
            x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcUiAdjRate), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcUiAdjRate));

            VERIFY(DRVCUST_OptQuery(eAudioAvChlEqTable, (UINT32*)&u4Ptr) == 0);
            x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.aai1AudChlEqCfg), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.aai1AudChlEqCfg));

            VERIFY(DRVCUST_OptQuery(eAudioChannelDelay, (UINT32*)&u4Ptr) == 0);
            x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.au1AudChDelay), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.au1AudChDelay));

            rAudFlashAQ.u4AudioVsClarity = DRVCUST_OptGet(eAudioMtkVsClarity);   //center gain,
            rAudFlashAQ.u4AudioVsWidth = DRVCUST_OptGet(eAudioMtkVsWidth);   //surround gain,
            rAudFlashAQ.u4AudioVsLRGain = DRVCUST_OptGet(eAudioMtkVsLRGain); //L/R gain,
            rAudFlashAQ.u4AudioVsXTalk = DRVCUST_OptGet(eAudioMtkVsXtalk); //crosstalk,
            rAudFlashAQ.u4AudioVsOutputGain = DRVCUST_OptGet(eAudioMtkVsOutputGain); //output gain,
            rAudFlashAQ.u4AudioVsBassGain = DRVCUST_OptGet(eAudioMtkVsBassGain); //bass enhancement,
            rAudFlashAQ.u4AudioVsFO = DRVCUST_OptGet(eAudioMtkVsFo); //FO,

            rAudFlashAQ.u4AudioLimiterMode = DRVCUST_OptGet(eAudioLimiterMode);

            rAudFlashAQ.u4AudioLimiterThreshold = DRVCUST_OptGet(eAudioLimiterFixThreshold);

            rAudFlashAQ.u1AudioLRSpkTypeLarge = DRVCUST_OptGet(eAudioSpkType);
            rAudFlashAQ.u1AudioSLRSpkTypeLarge = DRVCUST_OptGet(eAudioSurroundSpkType);
            rAudFlashAQ.u1AudioCenterSpkTypeLarge = DRVCUST_OptGet(eAudioCenterSpkType);
            rAudFlashAQ.u1AudioCH910SpkTypeLarge = DRVCUST_OptGet(eAudioCh910SpkType);
            rAudFlashAQ.u1AudioSubwooferOn = DRVCUST_OptGet(eAudioSubwooferOn);
            rAudFlashAQ.u2AudioBassMngCutoffFreq = DRVCUST_OptGet(eAudioBassMngCutOffFreq);
        }
        else
        {
#if MULTIPLE_FLASH_AQ
            Printf("!!!! FlashAQ(%d) Init OK !!!!\n", u2FlashAqIndex);
#else
            Printf("!!!! FlashAQ Init OK !!!!\n");
#endif
        }
#ifdef CC_SUPPORT_STR
        fgInit = TRUE;
    }
#endif
}

#ifdef CC_COMPRESSED_FLASH_AQ_DATA
void AUD_FlashAQInitEx_LZMA(void)
{
        UINT8 u1Partition;
        UINT32 u4ChkSum = 0;
        UINT32 u4Tmp;
        UINT32 u4Size;
        UINT8* pImageAddr = NULL;
        UINT8* pAudFlashAQ = NULL;
        LZMA_FILE_HEADER* prLzmaHdr = NULL;
        UINT32 u4DstSize, u4DstAddr, u4SrcAddr, u4SrcSize; 
        UINT32 aqBaseAddr;
        UINT32 u4Ret;
#if MULTIPLE_FLASH_AQ
        UINT16 u2FlashAqIndex;
        UINT32 u4Offset;
#endif
#ifdef CC_SUPPORT_STR
        static BOOL fgInit = FALSE;
        if (!fgInit)
        {
#endif
    
            u1Partition = (UINT8)DRVCUST_OptGet(eAudioFlashAQPartition);
            Printf("AUD_FlashAQInitEx_LZMA start %d \n", u1Partition);   
            // Get AQ base address.
            aqBaseAddr = DRVCUST_OptGet(eAudioFlashAQBaseAddress) + DRVCUST_OptGet(eAudioFlashAQOffset);
            Printf("AUD_FlashAQInitEx_LZMA aqBaseAddr 0x%x \n", aqBaseAddr);  

#if MULTIPLE_FLASH_AQ
#ifdef CC_ENABLE_MTK_MODEL_INDEX
            u2FlashAqIndex = vApiFlashaqGetQtyId(GetCurrentAQIndex());
#else
            u2FlashAqIndex = vApiFlashPqGetQtyId(GetCurrentPanelIndex());
#endif
            prLzmaHdr = (LZMA_FILE_HEADER* )x_mem_alloc(sizeof(LZMA_FILE_HEADER));
            if (prLzmaHdr == NULL)
            {
                Printf("Flash AQ Allocate Memory failed\n");     
                return;        
            }
            //Printf("AUD_FlashAQInitEx_LZMA u2FlashAqIndex 0x%x \n", u2FlashAqIndex);  
            AUD_FlashAQReadEx(u1Partition, aqBaseAddr, (UINT8*)(prLzmaHdr), sizeof(LZMA_FILE_HEADER)); //Read LZMA_FILE_HEADER
            pImageAddr = (UINT8*)x_mem_alloc(prLzmaHdr->compressed_size);
            //Printf("AUD_FlashAQInitEx_LZMA pImageAddr 0x%x \n", pImageAddr); 
            //Printf("Flash AQ  level %x\n", prLzmaHdr->props.level);
            //Printf("Flash AQ  lc %x\n", prLzmaHdr->props.lc);
            //Printf("Flash AQ  pb %x\n", prLzmaHdr->props.pb);
            //Printf("Flash AQ  algo %x\n", prLzmaHdr->props.algo);
            //Printf("Flash AQ  fb %x\n", prLzmaHdr->props.fb);
            //Printf("Flash AQ  btMode %x\n", prLzmaHdr->props.btMode);
            //Printf("Flash AQ  numHashBytes %x\n", prLzmaHdr->props.numHashBytes);
            //Printf("Flash AQ  mc %x\n", prLzmaHdr->props.mc);
            //Printf("Flash AQ  writeEndMark %x\n", prLzmaHdr->props.writeEndMark);
            //Printf("Flash AQ  numThreads %x\n", prLzmaHdr->props.numThreads);

            if (pImageAddr == NULL)
            {
                x_mem_free(prLzmaHdr);
                Printf("Flash AQ Allocate Memory failed,compressed_size %x\n", prLzmaHdr->compressed_size);  
                Printf("Flash AQ Allocate Memory failed,org_size %x\n", prLzmaHdr->org_size); 
                return;        
            }
            AUD_FlashAQReadEx(u1Partition, aqBaseAddr+sizeof(LZMA_FILE_HEADER), pImageAddr, prLzmaHdr->compressed_size);

            pAudFlashAQ = (UINT8*)x_mem_alloc(prLzmaHdr->org_size);
            if (pAudFlashAQ == NULL)
            {
                Printf("Flash AQ Allocate Memory failed,org_size %x \n", prLzmaHdr->org_size);  
                x_mem_free(prLzmaHdr);
                x_mem_free(pImageAddr);
                return;        
            }
            u4DstSize = prLzmaHdr->org_size;
            u4DstAddr = (UINT32)(pAudFlashAQ);
            u4SrcAddr = (UINT32)pImageAddr;
            u4SrcSize = prLzmaHdr->compressed_size;

            Printf("\n==========   AQ LZMA decode ==========\n");
            Printf("Src  addr=0x%08x   len=%d(0x%08x) \n", u4SrcAddr, u4SrcSize, u4SrcSize);    
            Printf("Dst  addr=0x%08x   len=%d(0x%08x) \n", u4DstAddr, u4DstSize, u4DstSize);

            u4Ret = (UINT32)LzmaUncompress((unsigned char*)u4DstAddr, (size_t*)&u4DstSize,
                (unsigned char*)u4SrcAddr, (size_t*)&u4SrcSize,
                (unsigned char *)&prLzmaHdr->props, sizeof(CLzmaEncProps));
        
            if (u4Ret != SZ_OK)
            {
                Printf("AQ LZMA fail, code=%d\n", u4Ret);
                // Flash AQ data in dram 
                x_mem_free(prLzmaHdr);
                x_mem_free(pImageAddr);
                x_mem_free(pAudFlashAQ);
                return;        
            }
            else
            {
                Printf("AQ LZMA decode ok\n");
            }
            u4Size = ((UINT32)(&(rAudFlashAQ.u1ChkSum[3])) - (UINT32)(&(rAudFlashAQ.au1AudId[0])) + 1 - FLASH_AQ_HDR_OFFSET);
            u4Offset = FLASH_AQ_HDR_OFFSET + u2FlashAqIndex * u4Size;
            x_memcpy((VOID*)VIRTUAL((UINT32)(&rAudFlashAQ)), pAudFlashAQ, FLASH_AQ_HDR_OFFSET);
            x_memcpy((VOID*)VIRTUAL((UINT32)(rAudFlashAQ.aai2AudInputSrcVol)), (pAudFlashAQ+u4Offset), u4Size);

            x_mem_free(prLzmaHdr);
            x_mem_free(pImageAddr);
            x_mem_free(pAudFlashAQ);
 
            //u4Size = ((UINT32)(&(rAudFlashAQ.u1ChkSum[3])) - (UINT32)(&(rAudFlashAQ.au1AudId[0])) + 1 - FLASH_AQ_HDR_OFFSET);
            //u4Offset = FLASH_AQ_HDR_OFFSET + u2FlashAqIndex * u4Size;
            //AUD_FlashAQReadEx(u1Partition, 0, (UINT8*)(&rAudFlashAQ), FLASH_AQ_HDR_OFFSET); //Read 'aqda' and vendor_version ID
            //AUD_FlashAQReadEx(u1Partition, u4Offset, (UINT8*)(rAudFlashAQ.aai2AudInputSrcVol), u4Size);
            for (u4Tmp = 0; u4Tmp < u4Size - 4; u4Tmp++)
            {
                //u4ChkSum += *((UINT8*)(&rAudFlashAQ)+FLASH_AQ_HDR_OFFSET+u4Tmp);
                u4ChkSum += *((UINT8*)(rAudFlashAQ.aai2AudInputSrcVol) + u4Tmp);
            }
#else
            // Get AQ base address.
            aqBaseAddr = DRVCUST_OptGet(eAudioFlashAQBaseAddress) + DRVCUST_OptGet(eAudioFlashAQOffset);
            prLzmaHdr = (LZMA_FILE_HEADER* )x_mem_alloc(sizeof(LZMA_FILE_HEADER));
            if (prLzmaHdr == NULL)
            {
                Printf("Flash AQ Allocate Memory failed\n");     
                return;        
            }
            AUD_FlashAQReadEx(u1Partition, aqBaseAddr, (UINT8*)(prLzmaHdr), sizeof(LZMA_FILE_HEADER)); //Read LZMA_FILE_HEADER
            pImageAddr = (UINT8*)x_mem_alloc(prLzmaHdr->compressed_size);
            if (pImageAddr == NULL)
            {
                Printf("Flash AQ Allocate Memory %x failed\n", prLzmaHdr->compressed_size);  
                x_mem_free(prLzmaHdr);
                return;        
            }
            AUD_FlashAQReadEx(u1Partition, aqBaseAddr+sizeof(LZMA_FILE_HEADER), pImageAddr, prLzmaHdr->compressed_size);
            pAudFlashAQ = (UINT8*)x_mem_alloc(prLzmaHdr->org_size);
            if (pAudFlashAQ == NULL)
            {
                Printf("Flash AQ Allocate Memory %x failed\n", prLzmaHdr->org_size);  
                x_mem_free(prLzmaHdr);
                x_mem_free(pImageAddr);
                return;        
            }

            u4DstSize = prLzmaHdr->org_size;
            u4DstAddr = (UINT32)(pAudFlashAQ);
            u4SrcAddr = (UINT32)pImageAddr;
            u4SrcSize = prLzmaHdr->compressed_size;

            Printf("\n==========  AQ LZMA decode ==========\n");
            Printf("Src  addr=0x%08x   len=%d(0x%08x) \n", u4SrcAddr, u4SrcSize, u4SrcSize);    
            Printf("Dst  addr=0x%08x   len=%d(0x%08x) \n", u4DstAddr, u4DstSize, u4DstSize);

            u4Ret = (UINT32)LzmaUncompress((unsigned char*)u4DstAddr, (size_t*)&u4DstSize,
                (unsigned char*)u4SrcAddr, (size_t*)&u4SrcSize,
                (unsigned char *)&prLzmaHdr->props, sizeof(CLzmaEncProps));
        
            if (u4Ret != SZ_OK)
            {
                Printf("AQ LZMA fail, code=%d\n", u4Ret);
                // Flash AQ data in dram
                x_mem_free(prLzmaHdr);
                x_mem_free(pImageAddr);
                x_mem_free(pAudFlashAQ);       
                return;        
            }
            else
            {
                Printf("AQ LZMA decode ok\n");
            }
            u4Size = ((UINT32)(&(rAudFlashAQ.u1ChkSum[3])) - (UINT32)(&(rAudFlashAQ.au1AudId[0])) + 1);
            x_memcpy((VOID*)VIRTUAL((UINT32)(rAudFlashAQ.au1AudId)), pAudFlashAQ, u4Size);

            x_mem_free(prLzmaHdr);
            x_mem_free(pImageAddr);
            x_mem_free(pAudFlashAQ);
            //u4Size = ((UINT32)(&(rAudFlashAQ.u1ChkSum[3])) - (UINT32)(&(rAudFlashAQ.au1AudId[0])) + 1);
                     //AUD_FlashAQReadEx(u1Partition, 0, (UINT8*)(&rAudFlashAQ), u4Size);
            for (u4Tmp = 0; u4Tmp < u4Size - 4; u4Tmp++)
            {
                u4ChkSum += *((UINT8*)(&rAudFlashAQ) + u4Tmp);
            }
#endif
    
            if ((rAudFlashAQ.au1AudId[0] != 'a') ||
                (rAudFlashAQ.au1AudId[1] != 'q') ||
                (rAudFlashAQ.au1AudId[2] != 'd') ||
                (rAudFlashAQ.au1AudId[3] != 'a') ||
                (((u4ChkSum >> 0) & 0xff) != rAudFlashAQ.u1ChkSum[0]) ||
                (((u4ChkSum >> 8) & 0xff) != rAudFlashAQ.u1ChkSum[1]) ||
                (((u4ChkSum >> 16) & 0xff) != rAudFlashAQ.u1ChkSum[2]) ||
                (((u4ChkSum >> 24) & 0xff) != rAudFlashAQ.u1ChkSum[3]))
            {
                UINT32 u4Ptr;
    
                Printf("!!!! AUD_FlashAQInitEx_LZMA - Partition: %d AQ ID: %d Offset: 0x%x size: %d",
                       u1Partition, u2FlashAqIndex, u4Offset, u4Size);
                Printf("!!!! AudioFlashAQPartition not update to flash correctly - %s !!!!\n",
                       ((rAudFlashAQ.au1AudId[0] != 'a') ||
                        (rAudFlashAQ.au1AudId[1] != 'q') ||
                        (rAudFlashAQ.au1AudId[2] != 'd') ||
                        (rAudFlashAQ.au1AudId[3] != 'a')) ?
                       "aqda not match" : "checksum not match");
    
                VERIFY(DRVCUST_OptQuery(eAudioInputVolTable, (UINT32*)&u4Ptr) == 0);
                x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.aai2AudInputSrcVol), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.aai2AudInputSrcVol));
    
                VERIFY(DRVCUST_OptQuery(eAudioVolTable, (UINT32*)&u4Ptr) == 0);
                x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.au4VolumeTable), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.au4VolumeTable));
#ifdef CC_AUD_NEW_CV_TABLE
                VERIFY(DRVCUST_OptQuery(eAudioChannelVolTable, (UINT32*)&u4Ptr) == 0);
                x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.au4ChannelVolumeTable), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.au4ChannelVolumeTable));
#endif
    
                VERIFY(DRVCUST_OptQuery(eAudioChlVolExtraGain, (UINT32*)&u4Ptr) == 0);
                x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.aai1AudChVolExtraGain), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.aai1AudChVolExtraGain));
    
                VERIFY(DRVCUST_OptQuery(eAudioAvcTarget, (UINT32*)&u4Ptr) == 0);
                x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcTarget), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcTarget));
    
                VERIFY(DRVCUST_OptQuery(eAudioAvcGainUp, (UINT32*)&u4Ptr) == 0);
                x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcGainUp), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcGainUp));
    
                VERIFY(DRVCUST_OptQuery(eAudioAvcAdjustRate, (UINT32*)&u4Ptr) == 0);
                x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcAdjRate), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcAdjRate));
    
                VERIFY(DRVCUST_OptQuery(eAudioAvcUiAdjustRate, (UINT32*)&u4Ptr) == 0);
                x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.ai2AudAvcUiAdjRate), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.ai2AudAvcUiAdjRate));
    
                VERIFY(DRVCUST_OptQuery(eAudioAvChlEqTable, (UINT32*)&u4Ptr) == 0);
                x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.aai1AudChlEqCfg), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.aai1AudChlEqCfg));
    
                VERIFY(DRVCUST_OptQuery(eAudioChannelDelay, (UINT32*)&u4Ptr) == 0);
                x_memcpy((VOID*)VIRTUAL((UINT32)rAudFlashAQ.au1AudChDelay), (const VOID*)u4Ptr, sizeof(rAudFlashAQ.au1AudChDelay));
    
                rAudFlashAQ.u4AudioVsClarity = DRVCUST_OptGet(eAudioMtkVsClarity);   //center gain,
                rAudFlashAQ.u4AudioVsWidth = DRVCUST_OptGet(eAudioMtkVsWidth);   //surround gain,
                rAudFlashAQ.u4AudioVsLRGain = DRVCUST_OptGet(eAudioMtkVsLRGain); //L/R gain,
                rAudFlashAQ.u4AudioVsXTalk = DRVCUST_OptGet(eAudioMtkVsXtalk); //crosstalk,
                rAudFlashAQ.u4AudioVsOutputGain = DRVCUST_OptGet(eAudioMtkVsOutputGain); //output gain,
                rAudFlashAQ.u4AudioVsBassGain = DRVCUST_OptGet(eAudioMtkVsBassGain); //bass enhancement,
                rAudFlashAQ.u4AudioVsFO = DRVCUST_OptGet(eAudioMtkVsFo); //FO,
    
                rAudFlashAQ.u4AudioLimiterMode = DRVCUST_OptGet(eAudioLimiterMode);
    
                rAudFlashAQ.u4AudioLimiterThreshold = DRVCUST_OptGet(eAudioLimiterFixThreshold);
    
                rAudFlashAQ.u1AudioLRSpkTypeLarge = DRVCUST_OptGet(eAudioSpkType);
                rAudFlashAQ.u1AudioSLRSpkTypeLarge = DRVCUST_OptGet(eAudioSurroundSpkType);
                rAudFlashAQ.u1AudioCenterSpkTypeLarge = DRVCUST_OptGet(eAudioCenterSpkType);
                rAudFlashAQ.u1AudioCH910SpkTypeLarge = DRVCUST_OptGet(eAudioCh910SpkType);
                rAudFlashAQ.u1AudioSubwooferOn = DRVCUST_OptGet(eAudioSubwooferOn);
                rAudFlashAQ.u2AudioBassMngCutoffFreq = DRVCUST_OptGet(eAudioBassMngCutOffFreq);
            }
            else
            {
#if MULTIPLE_FLASH_AQ
                Printf("!!!! FlashAQ(%d) Init OK !!!!\n", u2FlashAqIndex);
#else
                Printf("!!!! FlashAQ Init OK !!!!\n");
#endif
            }
#ifdef CC_SUPPORT_STR
            fgInit = TRUE;
        }
#endif
}
#endif



void AUD_GetFlashAQVer(CHAR* str)
{
    x_memcpy((VOID*)VIRTUAL((UINT32)str), (VOID*)VIRTUAL((UINT32)rAudFlashAQ.au1AudVersion), 8);
}
#else
void AUD_GetFlashAQVer(CHAR* str)
{
    UNUSED(str);
}
#endif

UINT32 GetAudioBassMngCutOffFreq(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u2AudioBassMngCutoffFreq;
#else
    return DRVCUST_OptGet(eAudioBassMngCutOffFreq);
#endif
}

UINT32 GetAudioLRSpkTypeLarge(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u1AudioLRSpkTypeLarge;
#else
    return DRVCUST_OptGet(eAudioSpkType);
#endif
}

UINT32 GetAudioSLRSpkTypeLarge(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u1AudioSLRSpkTypeLarge;
#else
    return DRVCUST_OptGet(eAudioSurroundSpkType);
#endif
}

UINT32 GetAudioCenterSpkTypeLarge(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u1AudioCenterSpkTypeLarge;
#else
    return DRVCUST_OptGet(eAudioCenterSpkType);
#endif
}

UINT32 GetAudioCH910SpkTypeLarge(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u1AudioCH910SpkTypeLarge;
#else
    return DRVCUST_OptGet(eAudioCh910SpkType);
#endif
}

UINT32 GetAudioSubwooferOn(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u1AudioSubwooferOn;
#else
    return DRVCUST_OptGet(eAudioSubwooferOn);
#endif
}

UINT32 GetAudioVsClarity(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsClarity;
#else
    return DRVCUST_OptGet(eAudioMtkVsClarity);
#endif
}

UINT32 GetAudioVsWidth(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsWidth;
#else
    return DRVCUST_OptGet(eAudioMtkVsWidth);
#endif
}

UINT32 GetAudioVsLRGain(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsLRGain;
#else
    return DRVCUST_OptGet(eAudioMtkVsLRGain);
#endif
}

UINT32 GetAudioVsXTalk(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsXTalk;
#else
    return DRVCUST_OptGet(eAudioMtkVsXtalk);
#endif
}

UINT32 GetAudioVsOutputGain(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsOutputGain;
#else
    return DRVCUST_OptGet(eAudioMtkVsOutputGain);
#endif
}

UINT32 GetAudioVsBassGain(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsBassGain;
#else
    return DRVCUST_OptGet(eAudioMtkVsBassGain);
#endif
}

UINT32 GetAudioVsFO(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioVsFO;
#else
    return DRVCUST_OptGet(eAudioMtkVsFo);
#endif
}

UINT32 GetAudioLimiterMode(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioLimiterMode;
#else
    return DRVCUST_OptGet(eAudioLimiterMode);
#endif
}

UINT32 GetAudioLimiterThreshold(void)
{
#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.u4AudioLimiterThreshold;
#else
    return DRVCUST_OptGet(eAudioLimiterFixThreshold);
#endif
}

#ifdef AUD_SUPPORT_SRS_BYPASS
INT8 AUD_GetFlashAQChVolExtraGain(UINT8 u1DecId, UINT8 u4ChNum)
{
    UNUSED(AUD_GetFlashAQChVolExtraGain);

    if (u1DecId > AUD_DEC_NUM-1)
    {
        u1DecId = AUD_DEC_NUM-1;
    }
    if (u4ChNum > AUD_CH_NUM-1)
    {
        u4ChNum = AUD_CH_NUM-1;
    }

#ifdef SUPPORT_FLASH_AQ
    return rAudFlashAQ.aai1AudChVolExtraGain[u1DecId][u4ChNum];
#else
    return 0;
#endif
}
#endif // DSP_SUPPORT_SRS_SSHD

//-----------------------------------------------------------------------------
/** Aud_FlashAQRead
 *  Read data from NAND Flash
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifdef CC_AUD_USE_FLASH_AQ
INT32 Aud_FlashAQRead(UINT32 u4Offset, UINT8* u1Data, UINT32 u4Length)
{
    UINT8 u1AQPartition;
    UINT32 u2AQSize;

    UNUSED(u2AQSize);
    u1Data[0] = '\0';
    u1AQPartition = (UINT8)DRVCUST_OptGet(eAudioFlashAQPartition);
    if (u1AQPartition == 0xFF)
    {
        LOG(3, "AudioFlashAQPartition is not defined !!\n");
        return -1;
    }

    return (STORG_SyncRead(u1AQPartition, u4Offset, (VOID*)u1Data, u4Length));
}

//-----------------------------------------------------------------------------
/** Aud_FlashAQWrite
 *  !!!! CAUTION !!!! This API is for CLI Testing ONLY !!
 *  !!!! CAUTION !!!! Do not Apply to any normal flow besides Testing !!
 *  NANDPART_Write is NOT allowed to be called here.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void Aud_FlashAQWrite(UINT32 u4Offset, UINT8* u1Data, UINT32 u4size)
{
    UINT8 u1AQPartition;
    UINT32 u2AQSize;
    //UINT64 u8Offset;

    UNUSED(u2AQSize);
    u1AQPartition = (UINT8)DRVCUST_OptGet(eAudioFlashAQPartition);
    if (u1AQPartition == 0xFF)
    {
        LOG(3, "AudioFlashAQPartition is not defined !!\n");
        return;
    }

#if 1
    // check boundary.
    if ((u4Offset + u4size) > AUD_FLASH_AQ_SIZE)
    {
        LOG(0, "AQ test write length larger than 0x%x\n", AUD_FLASH_AQ_SIZE);
        return;
    }
#endif
    STORG_SyncNandWriteBlock(u1AQPartition, u4Offset, (UINT8*)(VOID*)u1Data, u4size);
}

BOOL Aud_FlashAQCompVersion(CHAR* szCurVer, CHAR* szUpgVer)
{
    if (x_strcmp(szUpgVer, szCurVer) >= 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/** Aud_FlashAQSetDefault
 *  Set default data from drvcust to NAND Flash.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void Aud_FlashAQ_SetDefault(void)
{
    UNUSED(Aud_FlashAQ_SetDefault);
}

//-----------------------------------------------------------------------------
/** Aud_FlashAQGetDscrpt
 *  Get the data description from drvcust.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL Aud_FlashAQ_GetDscrpt(AUD_AQ_DSCRPT_T** pAudFlashAQDscrpt)
{
    if ((DRVCUST_OptQuery(eAudioFlashAQDataDscrpt, (UINT32*)(void*)pAudFlashAQDscrpt)) != 0)
    {
        LOG(0, "No Flash AQ Table defined!");
        return FALSE;
    }

    if (pAudFlashAQDscrpt != 0)
    {
        return TRUE;
    }

    LOG(0, "Flash AQ Table is NULL");
    return FALSE;
}

BOOL Aud_FlashAQ_GetDscrpt1(AUD_AQ_DSCRPT_T** pAudFlashAQDscrpt)
{
    if ((DRVCUST_OptQuery(eAudioFlashAQDataDscrpt1, (UINT32*)(void*)pAudFlashAQDscrpt)) != 0)
    {
        LOG(0, "No Flash AQ Amp Table defined!");
        return FALSE;
    }

    if (pAudFlashAQDscrpt != 0)
    {
        return TRUE;
    }

    LOG(0, "Flash AQ Amp Table is NULL");
    return FALSE;
}

static void Aud_FlashAQ_PrintData(UINT8* pBuffer, UINT32 u4Size)
{
    UINT32 i;
    UINT8 u1Data;

    UNUSED(Aud_FlashAQ_PrintData);

    for (i = 0; i < u4Size; i++)
    {
        u1Data = pBuffer[i];
        if ((i % 16) == 0)
        {
            Printf("\n");
        }
        Printf("0x%02x ", u1Data);
    }
    Printf("\n");
}

//-----------------------------------------------------------------------------
/** Aud_FlashAQ_Fill_Data
 *  Get the data from NAND flash.
 *  Parsing them according to data description and fill them to data structure.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
/*
----------------
  Common
----------------
  AQ 1 ~ 11
---------------- <--- FLASH_AQ_VER_OFFSET + Comm_size + (AQ_size * 11)
  5.1CH AQ
---------------- <--- FLASH_AQ_VER_OFFSET + Comm_size + (AQ_size * 11) + TypeIII_size
  EXT PEQ. 1~11
---------------- <--- FLASH_AQ_VER_OFFSET + Comm_size + (AQ_size * 11) + TypeIII_size + (PEQ_SIZE * 11)
  5.1CH PEQ
----------------


    (FY11 )
----------------
  Common
---------------- <--- FLASH_AQ_VER_OFFSET + Comm_size
  AQ 1 ~ N
---------------- <--- FLASH_AQ_VER_OFFSET + Comm_size + (AQ_size * N)
  EXT PEQ. 1~ N
---------------- <--- FLASH_AQ_VER_OFFSET + Comm_size + (AQ_size * N) + (PEQ_SIZE * 11)

*/
static UINT8 pData[AUD_FLASH_AQ_BUF_SIZE];
void Aud_FlashAQ_Fill_Data(BOOL fgDelayMode)
{

    AUD_AQ_DSCRPT_T* eAudAQDscrpt;
    UINT8 u1AudAQDscrptSize;
    UINT32 u4Offset;
    UINT32 u4Temp, u4Temp1;
    //UINT8 pData[AUD_FLASH_AQ_BUF_SIZE];
    UINT8 i;
    //UINT32 u4AQPostScale[2];
    UINT32 u4CommTableSize = 0;
    UINT32 u4AQTableSize = 0;
    UINT32 u4ExtPEQSize = 0;
    AUD_AQ_DSCRPT_T* eAudAQDscrpt1;
    UINT32 u4AQAmpTableSize = 0;
    UINT8 u1AudAQDscrptSize1 = 0;

    UINT8 _u1FlashAQ_ID_Offset;

    //UNUSED(u4AQPostScale);
    UNUSED(u4Temp1);
    UNUSED(u4CommTableSize);
    UNUSED(u4AQTableSize);
    UNUSED(u4ExtPEQSize);
    UNUSED(fgDelayMode);

    // Check first 2 bytes
    pData[0] = '\0';
    Aud_FlashAQRead(FLASH_AQ_VER_OFFSET, (UINT8*)pData, FLASH_AQ_VER_LENGTH);
    if (x_strncmp((CHAR*)pData, "AQ", 2) != 0)
    {
        LOG(0, "Flash AQ data is not correct, using default value\n");
        ; //todo : Using default value if no flash AQ is available.
        return;
    }

    u1AudAQDscrptSize = (UINT8)DRVCUST_OptGet(eAudioFlashAQDataDscrptSize);

    if (Aud_FlashAQ_GetDscrpt(&eAudAQDscrpt)) //Get the Data Structure.
    {
        u4Temp = 0;
        // ========== Calc Common Table Size =====================
        for (i = 0; i < FLASH_AQ_COMM_TABLE_ITEMS; i++)
        {
            u4Temp += eAudAQDscrpt[i].u4Size;
        }
        u4CommTableSize = u4Temp;
        // ========== Calc Single AQ Table Size =====================
        u4Temp = 0;
        for (i = FLASH_AQ_COMM_TABLE_ITEMS; i < u1AudAQDscrptSize; i++)
        {
            u4Temp += eAudAQDscrpt[i].u4Size;
        }
        u4AQTableSize = u4Temp;

        // ========== The Common Tables.  =====================
        u4Offset = FLASH_AQ_VER_OFFSET;

        LOG(3, "Fill Common Table, u4Offset = %d\n", u4Offset);
        for (i = 0; i < FLASH_AQ_COMM_TABLE_ITEMS; i++)
        {
            u4Temp = eAudAQDscrpt[i].u4Size;
            Aud_FlashAQRead(u4Offset, pData, u4Temp);
            Aud_FlashAQParsing(eAudAQDscrpt[i].eDataType, pData);
            if (fgDelayMode) //Print Data for Debug
            {
                Printf(" [%s] offset : %d ", eAudAQDscrpt[i].pChar, u4Offset);
                Aud_FlashAQ_PrintData(pData, u4Temp);
            }
            u4Offset += u4Temp;
        }
        // ========== The AQ Tables.  =====================
        // Skip to the AQ ID offset
        LOG(3, "Fill AQ ID [%d] Data, u4Offset = %d\n", _u1FlashAQ_ID, u4Offset);

        //AQ ID starts from 1, so decrease 1 here...
        _u1FlashAQ_ID_Offset = _u1FlashAQ_ID - 1;
        u4Offset = FLASH_AQ_VER_OFFSET + u4CommTableSize + (u4AQTableSize * _u1FlashAQ_ID_Offset);

        // Parsing it.
        for (i = FLASH_AQ_COMM_TABLE_ITEMS; i < u1AudAQDscrptSize; i++)
        {
            u4Temp = eAudAQDscrpt[i].u4Size;
            Aud_FlashAQRead(u4Offset, pData, u4Temp);
            Aud_FlashAQParsing(eAudAQDscrpt[i].eDataType, pData);
            if (fgDelayMode) //Print Data for Debug
            {
                Printf(" [%s] offset : %d ", eAudAQDscrpt[i].pChar, u4Offset);
                Aud_FlashAQ_PrintData(pData, u4Temp);
            }
            u4Offset += u4Temp;
        }
    }
    else
    {
        LOG(0, "Flash AQ data is not correct, using default value\n");
        ; //todo : Using default value if no flash AQ is available.
    }

    ///_arSonyAmpNvmDscrpt
    if (Aud_FlashAQ_GetDscrpt1(&eAudAQDscrpt1)) //Get the Data Structure.
    {
        u1AudAQDscrptSize1 = (UINT8)DRVCUST_OptGet(eAudioFlashAQDataDscrptSize1);

        u4Temp = 0;
        // ========== Calc Common Table Size =====================
        for (i = 0; i < u1AudAQDscrptSize1; i++)
        {
            u4Temp += eAudAQDscrpt1[i].u4Size;
        }
        u4AQAmpTableSize = u4Temp;   /// u4AQAmpTableSize

        //AQ ID starts from 1, so decrease 1 here...
        _u1FlashAQ_ID_Offset = _u1FlashAQ_ID - 1;

        /// "AMP Function Field" sheet, start offset
        u4Offset = FLASH_AQ_VER_OFFSET + u4CommTableSize + (u4AQTableSize * FLASH_AQ_ID_NUM);
        u4Offset += (u4AQAmpTableSize * _u1FlashAQ_ID_Offset);

        // Parsing it.
        for (i = 0; i < u1AudAQDscrptSize1; i++)
        {
            u4Temp = eAudAQDscrpt1[i].u4Size;
            Aud_FlashAQRead(u4Offset, pData, u4Temp);
            Aud_FlashAQParsing(eAudAQDscrpt1[i].eDataType, pData);
            if (fgDelayMode) //Print Data for Debug
            {
                Printf(" 2K13 [%s] offset : %d ", eAudAQDscrpt1[i].pChar, u4Offset);
                Aud_FlashAQ_PrintData(pData, u4Temp);
            }
            u4Offset += u4Temp;
        }
    }
    else
    {
        LOG(0, "Flash AQ Amp data is not correct, using default value\n");
        ; //todo : Using default value if no flash AQ is available.
    }
}

void AUD_FlashAQ_Init(void)
{
    _u1FlashAQ_ID = Panel_GetAQIndex(0);

    if ((_u1FlashAQ_ID == 0) || (_u1FlashAQ_ID > FLASH_AQ_ID_NUM))
    {
        _u1FlashAQ_ID = 1; //The minimal ID is AQ1
    }

    //Set all flag to False;
    Aud_FlashAQSetValue();

    //Fill all data into dram.
    Aud_FlashAQ_Fill_Data(FALSE);
}

UINT8 AUD_FlashAQ_GetTableNum(void)
{
    return _u1FlashAQ_ID;
}

UINT8 AUD_FlashAQ_ModelQueryFunc(void)
{
    UNUSED(AUD_FlashAQ_ModelQueryFunc);
    return 0;
}

#endif /* CC_AUD_USE_FLASH_AQ */
//=================== END of Audio Flash AQ Functions ==============================

#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM
#ifdef CC_AUD_DDI
AUD_FMT_T AudAtvFmtDetection(UINT8 u1DecId)
{
    return(_AudAtvFmtDetection(u1DecId));
}
#else
AUD_FMT_T AudAtvFmtDetection(void)
{
    return(_AudAtvFmtDetection());
}
#endif
#endif


void AUD_GetAudioTrickPts(UINT8 u1DecId, UINT32* prAudioPts, BOOL fgOldPts)
{
    AUD_DrvGetAudioTrickPts(u1DecId, prAudioPts, fgOldPts);
}

#ifdef CC_AUD_VBASS_SUPPORT
VOID AUD_SetVBassCutOff(UINT8 u1Idx)
{
    UNUSED(u1VBassCutOffFreq);
    UNUSED(AUD_SetVBassCutOff);
    if (u1Idx > (VBASS_CUTOFF_NUM - 1))
    {
        u1Idx = (VBASS_CUTOFF_NUM - 1);
    }
    u1VBassCutOffFreq = u1Idx;
}
UINT8 AUD_GetVBassCutOff(void)
{
    UNUSED(AUD_GetVBassCutOff);

    return u1VBassCutOffFreq;
}
#endif //CC_AUD_VBASS_SUPPORT

#if defined(__MODEL_slt__)
extern void DSP_DrvSuspend(void);
extern void AudPllPowerDown(UINT8 PLL_ID, BOOL fgPD);
extern void vDspPowerOff (UINT8 u1DspId);
#endif

#if defined(CC_FAST_INIT) || defined(__MODEL_slt__)

static void AUD_pd_suspend(void)
{
    /////// Power Down Audio Module ///////
    /// Disable audio output stage.
    LOG(0, "[AUDS] AUD_pm_suspend - Power Down.\n");
    ADAC_Mute(TRUE);
    ADAC_GpioCodecMute(TRUE);
    ADAC_GpioAmpEnable(FALSE);
    ADAC_CodecMute(TRUE);
    ADAC_MuteCLK(TRUE); ///PWM CLK Fade-out to 0V.

    /// Disable MCLK Output
    _AUD_OutPadEnable(AUD_DEC_MAIN, FALSE);
    _AUD_OutPadEnable(AUD_DEC_AUX, FALSE);

#ifndef __MODEL_slt__
    /// Disable SPDIF
    AUD_Spdif_Power_Down();
#endif

    /// Reset Amplifier
    ///_AUD_ResetAmp(TRUE);     /// by ADAC_GpioAmpEnable(FALSE); no need reset.

    /// Disable AADC
    ADAC_ADCDown(TRUE);

    /// Power down Aud Pll
    AudPllPowerDown(APLL_ID_MAIN, 1);
    AudPllPowerDown(APLL_ID_AUX, 1);
    
    /// Power down DSP
#ifdef __MODEL_slt__
    vDspPowerOff(AUD_DSP0);
#ifndef CC_AUD_DISABLE_2ND_DSP    
    vDspPowerOff(AUD_DSP1);   
#endif     
#endif    
    /////// Power Down Audio Module End ///////
}

//-----------------------------------------------------------------------------
/** AUD_pm_resume
 *  Resume function.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_pm_resume(void)
{
    LOG(0, "[AUDS] AUD_pm_resume.\n");
    
    /// Power on Aud Pll
#ifdef __MODEL_slt__    
    AudPllPowerDown(APLL_ID_MAIN, 0);
    AudPllPowerDown(APLL_ID_AUX, 0);    
#endif    
    DSP_DrvResume();
    AUD_Init();

    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_MAIN);
    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_AUX);
#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_THIRD);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_4TH);
#endif
#ifndef CC_AUD_DISABLE_2ND_DSP
    //AUD_DrvThreadWakeup(AUD_DSP1, AUD_DEC_MAIN);
    //AUD_DrvThreadWakeup(AUD_DSP1, AUD_DEC_AUX);
#ifdef CC_MT5391_AUD_3_DECODER
    //AUD_DrvThreadWakeup(AUD_DSP1, AUD_DEC_THIRD);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    //AUD_DrvThreadWakeup(AUD_DSP1, AUD_DEC_4TH);
#endif
#endif
    LOG(0, "[AUDS] AUD_pm_resume Done.\n");
}

//-----------------------------------------------------------------------------
/** AUD_pm_suspend
 *  suspend function.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_pm_suspend(void)
{
    LOG(0, "[AUDS] AUD_pm_suspend.\n");
    AUD_DrvSuspend();
#ifndef __MODEL_slt__    
    ADAC_Suspend();
#endif    
    AUD_DSPCmdSema_UnLock();
    AUD_NotifySuspend();

    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP0][AUD_DEC_MAIN]) == OSR_OK);
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP0][AUD_DEC_AUX]) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP0][AUD_DEC_THIRD]) == OSR_OK);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP0][AUD_DEC_4TH]) == OSR_OK);
#endif
#ifndef CC_AUD_DISABLE_2ND_DSP
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP1][AUD_DEC_MAIN]) == OSR_OK);
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP1][AUD_DEC_AUX]) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP1][AUD_DEC_THIRD]) == OSR_OK);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP1][AUD_DEC_4TH]) == OSR_OK);
#endif
#endif
    VERIFY(x_sema_unlock(_hAsynCmdSema) == OSR_OK);
    AUD_DspCfgSuspend();
    DSP_DrvSuspend();

    /// Power Down Audio Module.
    AUD_pd_suspend();

    _fgAudInitOK = FALSE;
    LOG(0, "[AUDS] AUD_pm_suspend End.\n");
}
#endif

#ifdef CC_SUPPORT_STR 
static void _AmpUnMuteThread(void *pvArgs)
{
    UINT8 u1Data=0;
    UINT32 u4ReadCount = 0;

    UNUSED(pvArgs);
    UNUSED(_AmpUnMuteThread);
    while (u4ReadCount <= 0)
    {
        u4ReadCount = SIF_X_Read(AUD_AMP_BUS_ID, SIF_CLK_DIV, AD82581_DEV_ADDR, 1 , 0x2, &u1Data, 1);
        LOG(5, "===sif_read error===\n");
        x_thread_delay(1);
    }
    LOG(5, "===sif_read succeed, u1Data = 0x%x===\n", u1Data);
    //audio resume set unmute
    // for middleware get 
    _aeMuteType[AUD_DEC_MAIN] = AUD_DEC_MUTE_OFF;
    _aeMuteType[AUD_DEC_THIRD] = AUD_DEC_MUTE_OFF;
    // for driver
    _afgMute[AUD_DEC_MAIN] = FALSE;
    _afgMute[AUD_DEC_THIRD] = FALSE;
    ADAC_SpeakerEnable(TRUE);
    AUD_UnMute(AUD_DEC_MAIN);
    AUD_UnMute(AUD_DEC_THIRD);
#ifdef CC_SUPPORT_STR
    if (!_fgAudSpeakerPortOn) ADAC_SpeakerEnable(FALSE);;        
#endif      
    
}

void AUD_pm_str_suspend(void)
{
    int i;

    LOG(0, "[AUDS] AUD_pm_str_suspend. (20120621a)\n"); 
    for (i = AUD_DEC_MAIN; i <= AUD_DEC_4TH; i++)
    {
#ifndef CC_MT5391_AUD_3_DECODER
        if (i == AUD_DEC_THIRD)
        {
            continue;
        }
#endif
        if (!fgDecoderStopped(AUD_DSP0, i))
        {
            AUD_DSPCmdStop(AUD_DSP0, i);
            while (!fgDecoderStopped(AUD_DSP0, i))
            {
                LOG(0, "[AUDS] waiting DSP0 decoder %d stop\n", i);
                x_thread_delay(10);
            }
        }
    }
#ifndef CC_AUD_DISABLE_2ND_DSP
    for (i = AUD_DEC_MAIN; i <= AUD_DEC_AUX; i++)
    {
        if (!fgDecoderStopped(AUD_DSP1, i))
        {
            AUD_DSPCmdStop(AUD_DSP1, i);
            while (!fgDecoderStopped(AUD_DSP1, i))
            {
                LOG(0, "[AUDS] waiting DSP1 decoder %d stop\n", i);
                x_thread_delay(10);
            }
        }
    }
#endif        
    fgAudPmStrMode = TRUE;

    AUD_DrvSuspend();
    ADAC_Suspend();
    AUD_DSPCmdSema_UnLock();
    AUD_NotifySuspend();

    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP0][AUD_DEC_MAIN]) == OSR_OK);
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP0][AUD_DEC_AUX]) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP0][AUD_DEC_THIRD]) == OSR_OK);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP0][AUD_DEC_4TH]) == OSR_OK);
#endif
#ifndef CC_AUD_DISABLE_2ND_DSP
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP1][AUD_DEC_MAIN]) == OSR_OK);
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP1][AUD_DEC_AUX]) == OSR_OK);
#ifdef CC_MT5391_AUD_3_DECODER
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP1][AUD_DEC_THIRD]) == OSR_OK);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    VERIFY(x_sema_unlock(_hCmdWaitSema[AUD_DSP1][AUD_DEC_4TH]) == OSR_OK);
#endif
#endif
    VERIFY(x_sema_unlock(_hAsynCmdSema) == OSR_OK);
    AUD_DspCfgSuspend();
    DSP_DrvSuspend();

    /////// Power Down Audio Module ///////
    /// Disable audio output stage.
    LOG(0, "[AUDS] AUD_pm_str_suspend - Power Down.\n");
    ADAC_SpeakerEnable(FALSE);
    ADAC_Mute(TRUE);
    //ADAC_GpioCodecMute(TRUE);
    //ADAC_GpioAmpEnable(FALSE);
    ADAC_CodecMute(TRUE);
#if 0 //TODO
    ADAC_MuteCLK(TRUE); ///PWM CLK Fade-out to 0V.
#endif

#ifdef CC_AUD_HP_DEPOP
    _AudHpDePopTaskStatusChange(AUD_HP_DEPOP_DC_OFF);
#endif

    /// Disable MCLK Output
    _AUD_OutPadEnable(AUD_DEC_MAIN, FALSE);
    _AUD_OutPadEnable(AUD_DEC_AUX, FALSE);

#if 0 //TODO
    /// Disable SPDIF
    AUD_Spdif_Power_Down();
#endif

    /// Reset Amplifier
    ///_AUD_ResetAmp(TRUE);     /// by ADAC_GpioAmpEnable(FALSE); no need reset.

    /// Disable AADC
    ADAC_ADCDown(TRUE);

    /// Disable PWMDAC
    _MT5398_DACDOWN(TRUE);

    /// Power down Aud Pll
    AudPllPowerDown(APLL_ID_MAIN, 1);
    AudPllPowerDown(APLL_ID_AUX, 1);

    _fgAudInitOK = FALSE;
    /// Power down DSP
    vDspPowerOff(AUD_DSP0);
#ifndef CC_AUD_DISABLE_2ND_DSP    
    vDspPowerOff(AUD_DSP1);
#endif    
#ifdef CC_AUD_ARM_SUPPORT
    vAprocMain_Close( );
#endif    
    
    /// Disable all audio clock and enter chip_reset mode
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG1, 0,  FLD_AUDIO_RST_2);
    vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG1, 0, FLD_AUDIO_CKEN_2);
#else
    vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG1, 0, FLD_AUDIO_RST);
    vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG1, 0, FLD_AUDIO_CKEN);
#endif
    /////// Power Down Audio Module End ///////

    AUD_PostProc_Suspend();

    fgPsrPmStrResumeMode = FALSE;
    fgEQSpetrumInfoStrResumeMode = FALSE;
    LOG(0, "[AUDS] AUD_pm_str_suspend End.\n");
}

//WORKAROUND_START
#ifdef CC_AUD_MIXSOUND_SUPPORT
extern BOOL _fgMixSndFlag;
extern BOOL _fgMixSndMute;
extern UINT8 _u1MixSndClipIdx;
#endif
//WORKAROUND_END

void AUD_pm_str_resume(void)
{
    HANDLE_T hAmpUnMuteThread = NULL_HANDLE;

    LOG(0, "[AUDS] AUD_pm_str_resume. (20120621a)\n");

    fgAudPmStrResume = TRUE;
    fgPsrPmStrResumeMode = TRUE;
    fgEQSpetrumInfoStrResumeMode = TRUE;

    /////// Power On Audio Module ///////
    /// Enable all audio clock and enter chip_reset mode
    //vIO32WriteFldAlign(CKGEN_REG_DSP_CKCFG, 1, FLD_DSP_PD);
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG1, 1, FLD_AUDIO_CKEN_2);
    vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG1, 1,  FLD_AUDIO_RST_2);
#else
    vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG1, 1, FLD_AUDIO_CKEN);
    vIO32WriteFldAlign(CKGEN_BLOCK_RST_CFG1, 1, FLD_AUDIO_RST);
#endif
    /// Power on Aud Pll
    AudPllPowerDown(APLL_ID_MAIN, 0);
    AudPllPowerDown(APLL_ID_AUX, 0);

    /// Enable PWMDAC
    _MT5398_DACDOWN(FALSE);
    /// Enable AADC
    ADAC_ADCDown(FALSE);

    // Enable MCLK Output
    AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
    AUD_OutPadEnable(AUD_DEC_AUX, TRUE);

    DSP_DrvResume();
    AUD_Init();

    PSR_SoftReset(AUD_DSP0);
#ifndef CC_AUD_DISABLE_2ND_DSP 
    PSR_SoftReset(AUD_DSP1);
#endif    
    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_MAIN);
    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_AUX);
#ifdef CC_MT5391_AUD_3_DECODER
    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_THIRD);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
    AUD_DrvThreadWakeup(AUD_DSP0, AUD_DEC_4TH);
#endif
#ifndef CC_AUD_DISABLE_2ND_DSP 
    AUD_DrvThreadWakeup(AUD_DSP1, AUD_DEC_MAIN);
    AUD_DrvThreadWakeup(AUD_DSP1, AUD_DEC_AUX);
#endif
    AUD_DspChannelMute(AUD_DSP0, AUD_DEC_AUX, AUD_CH_FRONT_LEFT, TRUE);
    AUD_DspChannelMute(AUD_DSP0, AUD_DEC_AUX, AUD_CH_FRONT_RIGHT, TRUE);

    LOG(0, "[AUDS] AUD_pm_str_resume Done. (20120618b)\n");

    /////// Power On Audio Module End ///////
    fgAudPmStrMode = FALSE;
    
    //WORKAROUND_START
#ifdef CC_AUD_MIXSOUND_SUPPORT
    //_fgMixSndFlag = FALSE;
    //_fgMixSndMute = FALSE;
    {
        UINT8 tmp = _u1MixSndClipIdx;
        _u1MixSndClipIdx = 0xff;
        AUD_DspMixSndClip(tmp);
    }
#endif

#ifdef CC_AUD_HP_DEPOP
    _AudHpDePopTaskStatusChange(AUD_HP_DEPOP_DC_ON);
#endif

    x_thread_create(&hAmpUnMuteThread, "AmpUnMuteThread",
                    2048,
                    100,
                    _AmpUnMuteThread, 0, NULL);

    AUD_PostProc_Resume();
    AUD_MixsndResume();
    
    fgAudPmStrResume = FALSE;
}
#endif

//-----------------------------------------------------------------------------
/** AUD_SetSyncDbgLvl
 *  Set AV Sync Debug Level.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_SetSyncDbgLvl(UINT16 u2Flag)
{
    UINT16 u2Tmp = 0;

    UNUSED(AUD_SetSyncDbgLvl);
    u2Tmp = DSP_GetPtsStcCtrlFlag();

    if ((u2Flag & AUD_DBG_SYNC_PTS_FRESH) == AUD_DBG_SYNC_PTS_FRESH)
    {
        u2Tmp |= (UINT16)(1 << 4); //bit 4 is enable log.
    }
    else
    {
        u2Tmp &= (~(UINT16)(1 << 4));
    }
    DSP_SetPtsStcCtrlFlag(u2Tmp);

    u2AvSyncDbgFlag = u2Flag;
}
UINT16 AUD_GetSyncDbgLvl(void)
{
    UNUSED(AUD_GetSyncDbgLvl);
    UNUSED(u2AvSyncDbgFlag);

    return u2AvSyncDbgFlag;
}

#ifdef KARAOKE_MIC_LINE_IN2
void AUD_SetAout2MixAout1(UINT32 u4ChSetting)
{
    AUD_WRITE32_MSK(REG_AOUT_DRAM_CFG, (u4ChSetting << ADD_DEC2_POS), ADD_DEC2_MASK);
}
#endif

#ifdef CC_ENABLE_AV_SYNC
//-----------------------------------------------------------------------------
/** Aud_AddLogDump
 *  Add Log to a list.
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void Aud_AddLogDump(CHAR acMsg[AUD_DUMP_LOG_LENGTH], INT32 i4Value)
{
    /*
      1. Just pass 0 to i4Value even don't need to print it.
         (not sure if we can support optional parameter)
    */
#ifdef CC_AUD_DUMP_LOG
    HAL_TIME_T _rStartTime;
    CHAR cTmp[AUD_DUMP_LOG_LENGTH];

    HAL_GetTime(&_rStartTime);
    x_sprintf(cTmp, " %d.%d [%d]", _rStartTime.u4Seconds, _rStartTime.u4Micros, i4Value);
    x_strcat(cTmp, acMsg);
    x_strcpy(_gcAudDumpLog[u1AudDumpLogFIdx], cTmp);

    u1AudDumpLogFIdx ++;
    if (u1AudDumpLogFIdx == AUD_DUMP_LOG_NUM)
    {
        u1AudDumpLogFIdx = 0;
    }
#else
    UNUSED(Aud_AddLogDump);
#endif
}
void Aud_ListLogDump(void)
{
#ifdef CC_AUD_DUMP_LOG
    UINT8 i;

    LOG(0, "Current Index = %d\n", u1AudDumpLogFIdx);
    for (i = 0; i < AUD_DUMP_LOG_NUM; i++)
    {
        LOG(0, "%02d, %s ", i, _gcAudDumpLog[i]);
    }
#else
    UNUSED(Aud_ListLogDump);
#endif
}
#endif

//-----------------------------------------------------------------------------
// AUD_OpenGamePcm
//
/** Brief of AUD_OpenGamePcm.
 *  Details of AUD_OpenGamePcm (optional).
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN)
 */
//-----------------------------------------------------------------------------

void AUD_OpenGamePcm(UINT8 u1DecId, AUD_FMT_T u1DecFmt, SAMPLE_FREQ_T SampleRate)
{
    UINT32 u4AFifoWp;

    //AUD_PCM_SETTING_T  rPcmSetting;

    u4AFifoWp = VIRTUAL(u4GetAFIFOStart(AUD_DSP0, u1DecId));
    //rPcmSetting.eSampleFreq = FS_22K;
    //rPcmSetting.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
    AUD_SetMultimediaMode(u1DecId,  AUD_MM_MODE_GAME);
    AUD_SetAvSynMode(u1DecId, AV_SYNC_FREE_RUN);
    AUD_SetDecType(AUD_DSP0, u1DecId, AUD_STREAM_FROM_MULTI_MEDIA, u1DecFmt);  
    DSP_SetPcmFs(AUD_DSP0, u1DecId, SampleRate);
    //AUD_PcmSetting(u1DecId, (const AUD_PCM_SETTING_T *)&rPcmSetting);
    //_AUD_IgnoreDecNotify(u1DecId, TRUE);
    //AUD_DSPCmdPlay(u1DecId);
    //PSR_RiscSetAudioWp(AUD_DSP0, u1DecId, PHYSICAL(u4AFifoWp));  
    _AUD_DMX_UpdateWritePtr(u1DecId, PHYSICAL(u4AFifoWp));
    //_AUD_IgnoreDecNotify(u1DecId, FALSE);
}

//-----------------------------------------------------------------------------
// AUD_CloseGamePcm
//
/** Brief of AUD_CloseGamePcm.
 *  Details of AUD_CloseGamePcm (optional).
 *
 *  @param      u1DecId        Decoder ID (AUD_DEC_MAIN)
 */
//-----------------------------------------------------------------------------

void AUD_CloseGamePcm(UINT8 u1DecId)
{
    _AUD_IgnoreDecNotify(u1DecId, TRUE);
    AUD_DSPCmdStop(AUD_DSP0, u1DecId);
    _AUD_IgnoreDecNotify(u1DecId, FALSE);
    AUD_SetMultimediaMode(u1DecId,  AUD_MM_MODE_NORMAL);
}

void AUD_GetPcmDelay(UINT32 u1DecId, UINT32* pdelay2)
{
    UINT32 u4AFfioFreeSize;
    UINT32 u4AFifoRp;
    UINT32 u4AFifoWp;
    UINT32 u4AFifoSize;

    u4AFifoSize = u4GetAFIFOEnd(AUD_DSP0, u1DecId) - u4GetAFIFOStart(AUD_DSP0, u1DecId);
    u4AFifoRp = VIRTUAL(DSP_GetDspReadPtrPhysicalAddr(AUD_DSP0, u1DecId));
    //u4AFifoWp = VIRTUAL(DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp(AUD_DSP0)));    
    _AUD_DMX_GetAudWrtPtr(u1DecId, &u4AFifoWp);
    u4AFifoWp = VIRTUAL(u4AFifoWp);

    u4AFfioFreeSize = ((u4AFifoWp > u4AFifoRp) ?
                       (u4AFifoWp - u4AFifoRp) : (u4AFifoSize + u4AFifoWp - u4AFifoRp));

    if (u4AFfioFreeSize > 288 * 1024)
    {
        u4AFfioFreeSize = 0;
    }
    if (pdelay2 != NULL)
    {
        *pdelay2 = u4AFfioFreeSize;
    }
}
/*-----------------------------------------------------------------------------
 *AUD_CircularBufferRead
 *Move data from non circular buffer to circular buffer
 *@param  u4DstAddr: Destination address which is circular buffer
 *             u4SrcAddr: Source address which is non cirular buffer
 *             u4Size: How many bytes to write
 *             u4SrcFifoStart: Start address of circular buffer
 *             u4SrcFifoEnd: End address of circular buffer
 *@retval   BOOL TRUE: Success
 *                      FALSE:Fail
-----------------------------------------------------------------------------*/
extern void DSP_FlushInvalidateDCacheFree(UINT32 u4Addr, UINT32 u4Len);
BOOL AUD_PcmBufferWrite(UINT32 u4DstAddr, UINT32* u4SrcAddr, UINT32 u4Size,
                        UINT32 u4DstFifoStart, UINT32 u4DstFifoEnd)
{
    UINT32 u4FifoSize;
    u4FifoSize = u4DstFifoEnd - u4DstFifoStart;

    if (u4DstAddr < u4DstFifoStart || u4DstAddr >= u4DstFifoEnd)
    {
        return FALSE;
    }
    if (u4Size == 0 || u4Size >= u4FifoSize)
    {
        return FALSE;
    }

    if (u4DstAddr + u4Size < u4DstFifoEnd)
    {
        x_memcpy((void*)u4DstAddr, (const void*)u4SrcAddr, u4Size);
        DSP_FlushInvalidateDCacheFree(u4DstAddr, u4Size);
    }
    else
    {
        x_memcpy((void*)u4DstAddr, (const void*)u4SrcAddr, u4DstFifoEnd - u4DstAddr);
        x_memcpy((void*)u4DstFifoStart, (const void*)(u4SrcAddr + u4DstFifoEnd - u4DstAddr),
                 u4Size + u4DstAddr - u4DstFifoEnd);
        DSP_FlushInvalidateDCacheFree(u4DstAddr, (u4DstFifoEnd - u4DstAddr));
        DSP_FlushInvalidateDCacheFree(u4DstFifoStart,  (u4Size + u4DstAddr - u4DstFifoEnd));
    }


    return TRUE;

}

UINT32 AUD_FillPcmBufer(UINT8 u1DecId, UINT32* lpBuffer2, UINT32 size2)
{
    UINT32 u4AFifoWp;
    UINT32 u4AFifoRp;
    UINT32 u4AFifoSize;
    UINT32 u4AFfioFreeSize;

    u4AFifoSize = u4GetAFIFOEnd(AUD_DSP0, u1DecId) - u4GetAFIFOStart(AUD_DSP0, u1DecId);
    u4AFifoRp = VIRTUAL(DSP_GetDspReadPtrPhysicalAddr(AUD_DSP0, u1DecId));
    //u4AFifoWp = VIRTUAL(DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp(AUD_DSP0)));
    _AUD_DMX_GetAudWrtPtr(u1DecId, &u4AFifoWp);
    u4AFifoWp = VIRTUAL(u4AFifoWp);

    u4AFfioFreeSize = ((u4AFifoWp < u4AFifoRp) ?
                       (u4AFifoRp - u4AFifoWp) : (u4AFifoSize + u4AFifoRp - u4AFifoWp));

    if (size2 < u4AFfioFreeSize)
    {
        VERIFY(AUD_PcmBufferWrite(u4AFifoWp, lpBuffer2, size2,
            VIRTUAL(u4GetAFIFOStart(AUD_DSP0, u1DecId)), VIRTUAL(u4GetAFIFOEnd(AUD_DSP0, u1DecId)))); 

    u4AFifoWp = ((u4AFifoWp + size2) < VIRTUAL(u4GetAFIFOEnd(AUD_DSP0, u1DecId))) ? 
                    (u4AFifoWp + size2) : (u4AFifoWp + size2 - u4AFifoSize);
        //PSR_RiscSetAudioWp(AUD_DSP0, u1DecId, PHYSICAL(u4AFifoWp));
        _AUD_DMX_UpdateWritePtr(u1DecId, PHYSICAL(u4AFifoWp));
        return size2;
    }
    else
    {
        LOG(7, "Audio fifo is full\n");
        return FALSE;
    }
}

#ifdef CC_AUD_DDI
BOOL AUD_PlayMuteSetChannelDelayReady(UINT16 u2ChannId)
{
    BOOL bRet = TRUE;
    UINT8 u1DecId = AUD_DEC_MAIN;    

    #ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
    bRet = AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_SET_DELAY_READY, u2ChannId);
    #endif
    return bRet;
}
#endif
#ifdef CC_ENABLE_AOMX
BOOL AUD_PlayMuteSetAoutEnableReady(UINT16 u2Timeout)
{
    BOOL bRet = TRUE;
    UINT8 u1DecId = AUD_DEC_MAIN;    

    #ifdef CC_AUD_ENABLE_PLAY_MUTE_CONTROL
    bRet = AUD_PlayMuteCmd(u1DecId, AUD_PM_CMD_AOUT_ENABLE, u2Timeout);
    #endif
    return bRet;
}
#endif

#ifdef CC_VOCAL_MUTE
void AUD_SetVoiceMuteFilter1(UINT8 u1Idx)
{
    UNUSED(_gu1VocalMuteFilterIdx1);
    UNUSED(AUD_SetVoiceMuteFilter1);
    
    if (u1Idx >= (VM_FILTER1_NUM-1))
    {
        u1Idx = (VM_FILTER1_NUM-1);
    }
    _gu1VocalMuteFilterIdx1 = u1Idx;
    _AUD_DspSetAoutReinit();
}

UINT8 AUD_GetVocaleMuteFilter1(void)  ///#if (defined(KARAOKE_SUPPORT) || defined(CC_VOCAL_MUTE))
{
    UNUSED(AUD_GetVocaleMuteFilter1);

    return _gu1VocalMuteFilterIdx1;
}

void AUD_SetVoiceMuteFilter2(UINT8 u1Idx)
{
    UNUSED(_gu1VocalMuteFilterIdx2);
    UNUSED(AUD_SetVoiceMuteFilter2);
    
    if (u1Idx >= (VM_FILTER2_NUM-1))
    {
        u1Idx = (VM_FILTER2_NUM-1);
    }
    _gu1VocalMuteFilterIdx2 = u1Idx;
    _AUD_DspSetAoutReinit();
}

UINT8 AUD_GetVocaleMuteFilter2(void)
{
    UNUSED(AUD_GetVocaleMuteFilter2);

    return _gu1VocalMuteFilterIdx2;
}
#endif

void AUD_SetVdoPictureOffStatus(BOOL fgEnable)
{
    // if Picture is set to off, "Line in mute with video" should be disabled, or audio will be muted.
    UNUSED(_gfgVdoPictureOffStatus);
    UNUSED(AUD_SetVdoPictureOffStatus);

    _gfgVdoPictureOffStatus = fgEnable;
}

UINT32 AUD_AudioDecoderState(UINT8 u1DecId)
{
    UINT32 u4Data = 0x0;
    if(AUD_DEC_MAIN == u1DecId)
    {
        u4Data = u4ReadDspSram(AUD_DSP0, SRAM_DECODER_CONTROL);
    }
    else if(AUD_DEC_AUX == u1DecId)
    {
        u4Data = u4ReadDspSram(AUD_DSP0, SRAM_DECODER_CONTROL_DEC2);
    }
    return u4Data;    
}

#ifdef KARAOKE_MIC_LINE_IN2
void AUD_MicInput_Enable(BOOL fgEnable) //CC_AUD_SUPPORT_MIC_INPUT
{
    UINT32 u4Aout2MixToAout1 = 0x1F; //All channel
    UNUSED(AUD_MicInput_Enable);
    if (fgEnable)
    {
        // Disable notify
        _AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE);
        // Stop Dec2
        AUD_DSPCmdStop(AUD_DEC_AUX);
        // Set Dec2 Source from Line_in_2
        AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_LINE_IN_2, AUD_FMT_PCM);    
        
        //ADC mux to VGA audio in
        AUD_AvMuxSel(AUD_INPUT_ID_VGA_0);

        //DEC2 Aout mix to DEC1 Aout
        AUD_SetAout2MixAout1(u4Aout2MixToAout1);
        // Play Decoder2
        AUD_DSPCmdPlay(AUD_DEC_AUX);
    }
    else
    {  
        // Stop Dec2
        AUD_DSPCmdStop(AUD_DEC_AUX);  
        
        AUD_SetAout2MixAout1(0);
        // Enable notify
        _AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE);
    }
    
}
#endif

#include "vdec_if.h"

CHAR * _paszVdecFormat[] =
{
    "VDEC_FMT_MPV",
    "VDEC_FMT_MP4",
    "VDEC_FMT_H264",
    "VDEC_FMT_WMV",
    "VDEC_FMT_H264VER",
    "VDEC_FMT_MJPEG",      // 5
    "VDEC_FMT_RV",
    "VDEC_FMT_AVS",
    "VDEC_FMT_VP6",
    "VDEC_FMT_VP8",
    "VDEC_FMT_RAW",      // 10
    "VDEC_FMT_OGG",
    "VDEC_FMT_JPG",
    "VDEC_FMT_PNG",
    "VDEC_FMT_MAX"
};

static INT16 rAudVdecTbl[VDEC_FMT_MAX] = 
{
    -170, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0
};

INT16 AUD_GetAudioVdecDelay(ENUM_VDEC_FMT_T eCodecType)
{
    if((UINT32)eCodecType < (UINT32)VDEC_FMT_MAX)
    {
        return rAudVdecTbl[eCodecType];
    }
    else
    {
        return 0;
    }
}

VOID AUD_SetAudioVdecDelay(ENUM_VDEC_FMT_T eCodecType, INT16 i2Delay)
{
    UINT32 i;
    
    if((UINT32)eCodecType < (UINT32)VDEC_FMT_MAX)
    {
        rAudVdecTbl[eCodecType] = i2Delay;
    }

    for(i = 0; i < (UINT32)VDEC_FMT_MAX; i++)
    {
        Printf("%s -- %d\n", _paszVdecFormat[i], rAudVdecTbl[i]);
    }
}
#ifdef CC_AUD_DDI
void AUD_CheckPvrPause(INT32 i4Speed)
{
    _AUD_CheckPvrPause(i4Speed);
}
void AUD_SetDelayMode (BOOL fgEnable)
{
    _AUD_SetDelayMode(fgEnable);
}
#endif

#ifdef CC_AUD_ARM_POST
//-----------------------------------------------------------------------------
/** AUD_AprocReconfig
 *  Reset Apollo post for DD banner usage
 *
 *  @param fgEnable : DD_banner enable/disable
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AprocReconfig(BOOL fgEnable)
{
    if (AUD_IsDSPRealPlay(AUD_DSP0, AUD_DEC_MAIN))
    {
        AUD_DSPCmdStop (AUD_DSP0, AUD_DEC_MAIN);
        if (fgEnable) 
        { 
            vAproc_Enable(0);                    // if DD_banner enable => disable upload_post
        }    
        else                                      // else                => depends on bIsSupportAproc
        {
            if (bIsSupportAproc () == TRUE) vAproc_Enable(1);
            else vAproc_Enable(0);
        }
        AUD_DSPCmdPlay (AUD_DSP0, AUD_DEC_MAIN);
    }
    else
    {
        if (fgEnable) 
        {
            vAprocS_EnableConfig(0);
        }
        else
        {
            if (bIsSupportAproc () == TRUE) vAprocS_EnableConfig(1);
            else vAprocS_EnableConfig(0);
        }       
    }
}
#endif

#ifdef CC_AUD_DDI
//-----------------------------------------------------------------------------
/** AUD_LineInHWCtrlEnable
 *  Add function for LG
 *
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_LineInHWCtrlEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_STREAM_FROM_T eStreamFrom = AUD_STREAM_FROM_OTHERS;
    AUD_GetStreamFrom(AUD_DSP0, AUD_DEC_MAIN, &eStreamFrom);
    if((AUD_STREAM_FROM_HDMI != eStreamFrom) || (u1DecId != AUD_DEC_AUX))
    {
        AUD_LineInCtrlEnable(u1DecId,fgEnable);
    }
}
#endif //CC_AUD_DDI

void AUD_QueryPacketCount(UINT8 u1DecId,  UINT32 * pPacketCount)
{
#ifdef LINUX_TURNKEY_SOLUTION
    AUD_DECODER_T arAudDecoder = {0};
#else
    AUD_DECODER_T arAudDecoder;
#endif

    AUD_GetDecoderStatus(AUD_DSP0, u1DecId, &arAudDecoder);
    *pPacketCount = arAudDecoder.u4ReceivePesCount;
}

void AUD_QueryFrameCount(UINT8 u1DecId,  UINT32 * pFrameCount)
{
    UINT32 u4DecFrameCount = 0;
    if (u1DecId == AUD_DEC_MAIN)
    {
        u4DecFrameCount = u4ReadDspSram(AUD_DSP0,SRAM_DECODE_FRAME_COUNT);
    }
    else if(u1DecId == AUD_DEC_AUX)
    {
        u4DecFrameCount = u4ReadDspSram(AUD_DSP0,SRAM_DECODE_FRAME_COUNT_DEC2);
    }
    
    *pFrameCount = u4DecFrameCount;
}


void AUD_QueryDecoderStatus(UINT8 u1DecId,  AUD_DEC_STATE * pDecoderStatus)
{
#ifdef LINUX_TURNKEY_SOLUTION
    AUD_DECODER_T arAudDecoder = {0};
#else
    AUD_DECODER_T arAudDecoder;
#endif

    AUD_GetDecoderStatus(AUD_DSP0, u1DecId, &arAudDecoder);
    *pDecoderStatus = (AUD_DEC_STATE)arAudDecoder.eDecState;
}



void AUD_QueryStreamType(UINT8 u1DecId,  AUD_FMT_T * pStreamType)
{
#ifdef LINUX_TURNKEY_SOLUTION
    AUD_DECODER_T arAudDecoder = {0};
#else
    AUD_DECODER_T arAudDecoder;
#endif

    AUD_GetDecoderStatus(AUD_DSP0, u1DecId, &arAudDecoder);
    *pStreamType = arAudDecoder.eDecFormat;
}

void AUD_QuerySampleRate(UINT8 u1DecId,  UINT32 * pSampleRate)
{
    SAMPLE_FREQ_T eSmpRate;
    eSmpRate = AUD_GetSampleFreq(u1DecId);
    
    switch (eSmpRate)
    {
    case FS_8K:
       *pSampleRate = 8000;
        break;
    case FS_11K:
        *pSampleRate = 11025;
        break;
    case FS_12K:
        *pSampleRate = 12000;
        break;        
    case FS_16K:
        *pSampleRate = 16000;
        break;
    case FS_22K:
        *pSampleRate = 22050;
        break;
    case FS_24K:
        *pSampleRate = 24000;
        break;
    case FS_32K:
        *pSampleRate = 32000;
        break;
    case FS_44K:
        *pSampleRate = 44100;
        break;
    case FS_48K:
        *pSampleRate = 48000;
        break;
    case FS_64K:
        *pSampleRate = 64000;
        break;
    case FS_88K:
        *pSampleRate = 88200;
        break;
    case FS_96K:
        *pSampleRate = 96000;
        break;
    case FS_176K:
        *pSampleRate = 176400;
        break;
    case FS_192K:
        *pSampleRate = 192000;
        break;
    default:
        *pSampleRate = 48000;
        break;
    }
}

void AUD_QueryStc(UINT8 u1DecId,  UINT32 * pStc)
{
    #if 0
    if(STC_GetSrc(STC_SRC_A1, &rStc) == STC_VALID)
    {
        *pStc = rStc.u4Base;
    }
    #endif
}

void AUD_QueryPts(UINT8 u1DecId,  UINT32 * pPts)
{
#ifdef LINUX_TURNKEY_SOLUTION
    AUD_DECODER_T arAudDecoder = {0};
#else
    AUD_DECODER_T arAudDecoder;
#endif

    AUD_GetDecoderStatus(AUD_DSP0, u1DecId, &arAudDecoder);
    *pPts = arAudDecoder.u4StartPts;
}

UINT32 AUD_GetAoutBandNum(UINT8 u1DecId)
{
    return 0;
}

void AUD_FlushAudioFifo(UINT8 u1DecId)
{
    UNUSED(u1DecId);
}




void AUD_QueryFirstPts(UINT8 u1DecId,  UINT32 * pPts)
{
#ifdef LINUX_TURNKEY_SOLUTION
    AUD_DECODER_T arAudDecoder = {0};
#else
    AUD_DECODER_T arAudDecoder;
#endif

    AUD_GetDecoderStatus(AUD_DSP0, u1DecId, &arAudDecoder);
    *pPts = arAudDecoder.u4StartPts;
}


#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
BOOL AUD_Aproc_Chk_Notify(UINT8 *u1DecId, UINT32 u4NfyTag, UINT32 eNfyCond)
{
    BOOL bRet = FALSE;
    AUD_DEC_STREAM_FROM_T eStreamFrom = AUD_STREAM_FROM_OTHERS;    

    UNUSED(u1DecId);
    UNUSED(u4NfyTag);
    UNUSED(eNfyCond);

#ifdef CC_MT5391_AUD_3_DECODER      
    LOG(5, "(DUAL) AUD_Aproc_Chk_Notify, Id=%d, u4NfyTag=0x%x, eNfyCond=0x%x\n", *u1DecId, u4NfyTag, eNfyCond); 
    AUD_GetStreamFrom(AUD_DSP0, *u1DecId, &eStreamFrom);

    if (gSubDecId != AUD_DEC_SUB) // //CC_DUAL_AUD_DEC_SUPPORT. In Dual Case, MAIN decoder may in 0 or 1. Change to correct one.
    {
         // Handled in mw_if(_AudMwNfyFunc) to skip many check in aud_notify.c     
    }
    else
    {
    // EU TV, Notify is from Dec0. 
    if ((AUD_NFY_TAG_T)u4NfyTag == AUD_NFY_STREAM)
    {
        if ((eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) || (eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER))
        {
            if ((eNfyCond == AUD_COND_AUD_INFO_CHG) && (*u1DecId == AUD_DEC_AUX))
            {   // for APRCO case, TV only on Dec1 but Notify is for Dec0.  
#ifdef CC_DUAL_AUD_DEC_SUPPORT                
                if (*u1DecId != gSubDecId) // But with DUAL case, DecID should not be changed. It maybe notify for SUB. Notify ID will convert in MW_IF layer.
#endif                        
                {
                    *u1DecId = AUD_DEC_MAIN;
                    bRet = TRUE;
                    LOG(7, "AUD_Aproc_Chk_Notify, change u1DecId AUX to Main in TV case\n");                
                }
            }
        }
    }
    }
#endif 

    return bRet;
}

BOOL  AUD_Aproc_Chk_Cmd(UINT8 u1DecId)
{
#ifdef CC_DUAL_AUD_DEC_SUPPORT
    return bSkipDSPCmdLst[AUD_DSP0][u1DecId];
#if 0
    BOOL bRet = TRUE;
    if (u1DecId == gMainDecId )
    {
        bRet = FALSE;
    }
    else if (u1DecId == AUD_DEC_THIRD )
    {
        bRet = FALSE;
    }
    else if (u1DecId == AUD_DEC_SUB )
    {
        bRet = FALSE;
    }    
    return bRet;
#endif
#else //CC_DUAL_AUD_DEC_SUPPORT

    BOOL bRet = FALSE;
    AUD_DEC_STREAM_FROM_T eStreamFrom = AUD_STREAM_FROM_OTHERS;
    AUD_FMT_T eFmt = AUD_FMT_UNKNOWN;
    
    UNUSED(eStreamFrom);
#ifdef CC_AUD_DDI
    return FALSE;
#endif
#ifdef CC_MT5391_AUD_3_DECODER      
    AUD_GetDecodeType(AUD_DSP0, u1DecId, &eStreamFrom, &eFmt);    
    // EU TV only need DSP Dec1.
    if ( ((eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||(eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER))
          && (eFmt!=AUD_FMT_DETECTOR) )
    {
        if (u1DecId == AUD_DEC_MAIN)
        {
            bRet = TRUE;
        }
    }

	if ((eStreamFrom == AUD_STREAM_FROM_LINE_IN) || (eStreamFrom == AUD_STREAM_FROM_LINE_IN_2))
	{
		if ((_AUD_DspIsTriOnlyDecMode()) && (u1DecId == AUD_DEC_MAIN))
		{
			LOG(0, "[paul debug] Skip linein dec0 when tridonly!!!\n");
			bRet = TRUE;
		}
	}
#endif //CC_MT5391_AUD_3_DECODER         

    return bRet;
#endif //CC_DUAL_AUD_DEC_SUPPORT
}

void AUD_Aproc_Chk_DecId(UINT8* u1DecId)
{
#ifdef CC_DUAL_AUD_DEC_SUPPORT
    LOG(5, "(Dual) AUD_Aproc_Chk_DecId, input %d\n", *u1DecId);
    if (*u1DecId == AUD_DEC_SUB)
    {
        *u1DecId = gSubDecId;            // Assign decoder ID for sub decoder in Dual case.
    }
    else
    {
        if (gSubDecId != AUD_DEC_SUB) //means alreay in DUAL mode, gMainDecId is assigned, need change
        {    
        *u1DecId = gMainDecId;
    }    
        // Else it's NOT is DUAL mode, gMainDecId is not assigned, should not change dec id.
    }    
#else //CC_DUAL_AUD_DEC_SUPPORT
    AUD_DEC_STREAM_FROM_T eStreamFrom = AUD_STREAM_FROM_OTHERS;
    AUD_FMT_T eFmt = AUD_FMT_UNKNOWN;
    
    UNUSED(eStreamFrom);
    *u1DecId = AUD_DEC_MAIN;        

#ifdef CC_MT5391_AUD_3_DECODER      
    AUD_GetDecodeType(AUD_DSP0, AUD_DEC_MAIN, &eStreamFrom, &eFmt);
    LOG(5, "AUD_Aproc_Chk_DecId: u1DecId = %d, eStreamFrom = %d, eFmt = %d\n", *u1DecId, eStreamFrom, eFmt);
    // EU TV only need DSP Dec1.
    if ( ((eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)||(eStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER))
          && (eFmt!=AUD_FMT_DETECTOR) )
    {
        *u1DecId = AUD_DEC_AUX;        
    }
#endif //CC_MT5391_AUD_3_DECODER         
#endif //CC_DUAL_AUD_DEC_SUPPORT
}

#ifdef CC_DUAL_AUD_DEC_SUPPORT
UINT8 AUD_Aproc_ChkAVPVR(UINT8 u1DecId)
{
	AUD_BS_SEL_T eGetbitsFrom = AUD_BS_SEL_PSR;

	eGetbitsFrom = AUD_GetBitStrSel(AUD_DSP0, AUD_DEC_MAIN);
	LOG(5, "[Debug] eGetbitsFrom = %d\n", eGetbitsFrom);
	if ((u1DecId == AUD_DEC_MAIN) &&
		((eGetbitsFrom == AUD_BS_SEL_LINE_IN1) || (eGetbitsFrom == AUD_BS_SEL_LINE_IN2)))
	{
		if (AUD_Aproc_GetMp3Enc())
		{
			_AudAprocInputMute(u1DecId, TRUE);
			return 1;
		}
		if ((AUD_IsDecoderRealPlay(AUD_DSP0, u1DecId)) && (bSkipDSPCmdLst[AUD_DSP0][u1DecId]))
		{
			return 2;
		}
	}
	return 0;
}

void AUD_Aproc_Show_SkipList(void)
{
	UINT8 i;
	Printf("Skip Command List:\n");
	Printf("DSP0:\n");
	for (i = 0; i < 5; i++)
	{
		Printf("%d  ", bSkipDSPCmdLst[0][i]);
	}
	Printf("\n");
	Printf("DSP1:\n");
	for (i = 0; i < 5; i++)
	{
		Printf("%d  ", bSkipDSPCmdLst[1][i]);
	}
	Printf("\n");
}
#endif

#ifndef CC_AUD_DDI
void AUD_Aproc_OutCtrl(UINT8 u1DecId, APROC_MW_CTRL_T* arMwCtrl)
{
    //UINT32 u4Idx;
    UINT32 u4Reg0;
    UINT32 u4Reg1[2];
	static UINT32 u4DEC1Enable = 0;  // Select DEC1 out flag     active low

    switch (arMwCtrl->eCmd)
    {
    case AUD_CMD_PLAY:
        // AV, MM, HDMI
        // non-TV always from Input0
        if ((arMwCtrl->_aeInSrc[AUD_DEC_MAIN] != AUD_STREAM_FROM_DIGITAL_TUNER) 
            && (arMwCtrl->_aeInSrc[AUD_DEC_MAIN] != AUD_STREAM_FROM_ANALOG_TUNER))
        {
            u4Reg0 = 0;
			if (((arMwCtrl->_aeInSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_LINE_IN) ||
				(arMwCtrl->_aeInSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_LINE_IN_2)) &&
				((arMwCtrl->_aeInSrc[AUD_DEC_THIRD] == AUD_STREAM_FROM_LINE_IN) ||
				(arMwCtrl->_aeInSrc[AUD_DEC_THIRD] == AUD_STREAM_FROM_LINE_IN_2)))
			{
				u4Reg0 = 2;
			}
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);        
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SPDIF_IN, &u4Reg0, 1);
			// Special case -> ATV timeshift: Dec0 MM, Dec1 ATV, should select SRC1 according to ATV recording
			if ((arMwCtrl->_aeInSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_MULTI_MEDIA)
				&& (arMwCtrl->_aeInSrc[AUD_DEC_AUX] == AUD_STREAM_FROM_ANALOG_TUNER))
			{
				u4Reg0 = 1;	
			}
			_vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_ENC_IN, &u4Reg0, 1);
        }
        else if ((arMwCtrl->_aeInSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_DIGITAL_TUNER) 
                  || (arMwCtrl->_aeInSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_ANALOG_TUNER))
        {
        // DTV, ATV
        // If TV is Dec0, select output according to TV decoder is at Job0 or Job1.
            #ifdef CC_MT5391_AUD_3_DECODER        
            u4Reg0 = 1;    
            #else
            u4Reg0 = 0;    
            #endif
            if (u1DecId == AUD_DEC_MAIN)
            {
                _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SPDIF_IN, &u4Reg0, 1);
                // Check if AD is ON
                _vAUD_Aproc_Get (APROC_CONTROL_TYPE_AD, 0, u4Reg1, 1);
                LOG(4, "Check_AD, u4Reg1[1]=%d, u4Reg1[0]=%d\n", u4Reg1[0],u4Reg1[1]);
                if (u4Reg1[0] != 0)
                {
                    u4Reg0 = 0; //AD mix result is Input 0
                }
                _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
                _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);
				// Special case -> Change to TV source when AV PVR, must keep AV recording, so encoder source must be 0
				if (AUD_Aproc_GetMp3Enc() == FALSE)
				{
					_vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_ENC_IN, &u4Reg0, 1);
				}
            }
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_SCART_IN, &u4Reg0, 1);
            if (u1DecId == AUD_DEC_THIRD)
            {
                // Turn on AD Flag.
                u4Reg1[0] = 1;
                u4Reg1[1] = 0;                
                _vAUD_Aproc_Set(APROC_CONTROL_TYPE_AD, 0, u4Reg1, 1);
                // ADMix Result is at Input0
                u4Reg0 = 0;
                //_vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
                //_vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);
                _AUD_DspADEnable(AUD_DEC_THIRD, 1);
                
            }            
        }

        break;

    case AUD_CMD_STOP:
		if ((arMwCtrl->_aeInSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_DIGITAL_TUNER)
             || (arMwCtrl->_aeInSrc[AUD_DEC_MAIN] == AUD_STREAM_FROM_ANALOG_TUNER))
        {
			if (u1DecId == AUD_DEC_THIRD) // MW wants to control AD
            {
                // Close AD Flag.
                u4Reg1[0] = 0;
                u4Reg1[1] = 0;                
                _vAUD_Aproc_Set(APROC_CONTROL_TYPE_AD, 0, u4Reg1, 1);
                #ifdef CC_MT5391_AUD_3_DECODER        
                u4Reg0 = 1;    
                #else
                u4Reg0 = 0;    
                #endif
				if (!u4DEC1Enable)
				{
					//just stop DEC2, set HP/SP/SPDIF_IN to Dec1 to output DTV/ATV 
                	_vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
                	_vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);
				}
				else
				{
					//stop DEC0 then stop DEC2, just disable ADEnable, keep HP/SP_IN to DEC0
                	_AUD_DspADEnable(AUD_DEC_THIRD, 0);
					u4DEC1Enable = 0;    //enable output DEC1 for next command
				}
            }
			else if (u1DecId == AUD_DEC_MAIN)
            {
                // MW wants to stop Dec0 TV, 
                // Maybe it's 1. DTV going to stop
                //            2. DTV going to AV/HDMI, but DTV is still in SCART.
                // Anyway, we should set HP/SP/SPDIF_IN to Dec0
                _vAUD_Aproc_Get (APROC_CONTROL_TYPE_AD, 0, u4Reg1, 1);
				LOG(4, "Check_AD_IN_STOP_case, u4Reg1[1]=%d, u4Reg1[0]=%d\n", u4Reg1[0],u4Reg1[1]);
				if (u4Reg1[0] != 0)
				{
					//if need to stop DEC2, disable output DEC1
					u4DEC1Enable = 1;
				}
                u4Reg0 = 0;
                _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SPDIF_IN, &u4Reg0, 1);
                _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg0, 1);
                _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg0, 1);            
                
            }            
        }

        break;        

    default:
        break;

    }
        
#ifdef CC_DUAL_AUD_DEC_SUPPORT    
    if (gSubDecId != AUD_DEC_SUB) 
    {
        AUD_Aproc_ChgFocus(u1DecId, _u1MW_SetFocusOn);    
    }
#endif    
        
}    
#else
void AUD_Aproc_OutCtrl(UINT8 u1DecId, APROC_MW_CTRL_T* arMwCtrl)
{
#if 0    
    UINT32 u4Reg;
    if((u1DecId != AUD_DEC_MAIN) &&  (u1DecId != AUD_DEC_AUX))
    {
        return;
    }

    u4Reg = (UINT32)u1DecId;
    switch (arMwCtrl->eCmd)
    {
    case AUD_CMD_PLAY:
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4Reg, 1);
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4Reg, 1);
        _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SPDIF_IN, &u4Reg, 1);
        break;

    case AUD_CMD_STOP:
        break;        

    default:
        break; 
    } 
#endif    
}

#endif

#ifdef CC_DUAL_AUD_DEC_SUPPORT
void AUD_Aproc_ChgFocus(UINT8 u1DecId, UINT8 u1Focus)
{
    UINT32 u4SpkSrc, u4HpSrc;
    UNUSED(u1DecId);

    LOG(5, "(DUAL)AUD_Aproc_ChgFocus to %d, m=%d,s=%d\n", u1Focus, gMainDecId, gSubDecId);
    
    if (gSubDecId == AUD_DEC_SUB) //Not in Dual Mode, Focus is meaningless.
    {
        u4SpkSrc = gMainDecId;  
        u4HpSrc = gMainDecId;    
    }    
    else if (u1Focus == AUD_DEC_MAIN)
    {
        u4SpkSrc = gMainDecId;  
        u4HpSrc = gSubDecId;
    }
    else
    {
        u4SpkSrc = gSubDecId;  
        u4HpSrc = gMainDecId;
    }
    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SP_IN, &u4SpkSrc, 1);
    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_HP_IN, &u4HpSrc, 1);        
    _vAUD_Aproc_Set (APROC_CONTROL_TYPE_SEL, APROC_REG_SEL_DSP_SPDIF_IN, &u4SpkSrc, 1);   
}

void AUD_Aproc_SetFocus(UINT8 u1DecId, UINT8 u1Focus)
{
    static UINT8 u1OriFocus = AUD_DEC_MAIN;
    UNUSED(u1DecId);

    LOG(5, "(DUAL)AUD_Aproc_SetFocus to %d, u1OriFocus=%d\n", u1Focus, u1OriFocus);
    _u1MW_SetFocusOn =  u1Focus;
    if (_u1MW_SetFocusOn != u1OriFocus)
    {
        AUD_Aproc_ChgFocus(u1DecId, _u1MW_SetFocusOn);
    }
    u1OriFocus = _u1MW_SetFocusOn;    
}
#endif

void AUD_Aproc_ChkTVDecInfoChgNfy(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DEC_INFO_T rAudDecInfo;
    AudDecNfyFct   pfAudDecNfy = NULL;
    AUD_DEC_STREAM_FROM_T eMainStreamFrom = AUD_STREAM_FROM_OTHERS;
    AUD_DEC_STREAM_FROM_T eAuxStreamFrom = AUD_STREAM_FROM_OTHERS;
    AUD_FMT_T eMainFmt = AUD_FMT_UNKNOWN;
    AUD_FMT_T eAuxFmt = AUD_FMT_UNKNOWN;
    UINT32 u4Tmp = 0;

    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);

	rAudDecInfo.ui4_data_rate = 0;
    if ((u1DecId == AUD_DEC_MAIN) && (u1DspId == AUD_DSP0))
    {
        AUD_GetDecodeType(AUD_DSP0, AUD_DEC_MAIN, &eMainStreamFrom, &eMainFmt);
        AUD_GetDecodeType(AUD_DSP0, AUD_DEC_AUX, &eAuxStreamFrom, &eAuxFmt);
		LOG(0, "_aeAudDecCmdState[AUD_DSP0][%d] = %d, _aeAudDecCmdState[AUD_DSP0][2] = %d\n", u1DecId, _aeAudDecCmdState[AUD_DSP0][u1DecId], _aeAudDecCmdState[AUD_DSP0][2]);
        if ((_aeAudDecCmdState[AUD_DSP0][AUD_DEC_MAIN] != AUD_CMD_ST_PLAY) &&
            (_aeAudDecCmdState[AUD_DSP0][AUD_DEC_THIRD] != AUD_CMD_ST_PLAY))
        {
			if (eAuxStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER)
        	{
				if (pfAudDecNfy != NULL)
            	{
                	rAudDecInfo.e_aud_fmt = AUD_FMT_TV_SYS; // FIXME;
                	rAudDecInfo.e_aud_type = DSP_GetAudChlInfo(AUD_DEC_AUX);
                	rAudDecInfo.ui4_sample_rate = 44;
                	rAudDecInfo.ui1_bit_depth = 24;
			  
                	if ((_rDspVars[u1DspId][AUD_DEC_AUX].bDspStrTyp == PAL_STREAM) ||
                    	(_rDspVars[u1DspId][AUD_DEC_AUX].bDspStrTyp == A2_STREAM) ||
                    	(_rDspVars[u1DspId][AUD_DEC_AUX].bDspStrTyp == NTSC_STREAM))
                	{
                    	rAudDecInfo.ui4_sample_rate = 32;
                    	if (_rDspVars[u1DspId][AUD_DEC_AUX].bDspStrTyp == NTSC_STREAM)
                    	{
                        	rAudDecInfo.e_aud_fmt = AUD_FMT_MTS;
                    	}
                    	if (rAudDecInfo.e_aud_type == AUD_TYPE_UNKNOWN)
                    	{
                        	rAudDecInfo.e_aud_type = AUD_TYPE_MONO;
                    	}
                    	LOG(0, "EU Mode: Change source to ATV from other sources, need notify MW info change!\n");
                    	pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                        (UINT32)&rAudDecInfo, u4Tmp);
                	}
            	}
        	}
			if ((eMainStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) && (eAuxStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) && (eAuxFmt == AUD_FMT_MPEG))
			{
				if (pfAudDecNfy != NULL)
				{
					UNUSED(_GetMpegAudInfo(AUD_DEC_AUX, &rAudDecInfo));
                    pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
					LOG(0, "EU Mode: Change source to DTV from other sources, MPEG need notify MW info change!\n");
				}
			}
			if ((eMainStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) && (eAuxStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) && (eAuxFmt == AUD_FMT_AC3))
			{
				if (pfAudDecNfy != NULL)
				{
					rAudDecInfo.e_aud_fmt = AUD_FMT_AC3;
                    rAudDecInfo.ui1_bit_depth = 24;
					_GetAudInfo(AUD_DEC_AUX, &rAudDecInfo);
                    pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
					LOG(0, "EU Mode: Change source to DTV from other sources, AC3 need notify MW info change!\n");
				}
			}
			if ((eMainStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) && (eAuxStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) && (eAuxFmt == AUD_FMT_AAC))
			{
				if (pfAudDecNfy != NULL)
				{
					_GetAacAudInfo(AUD_DEC_AUX, &rAudDecInfo);
                    pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
					LOG(0, "EU Mode: Change source to DTV from other sources, AAC need notify MW info change!\n");
				}
			}
			if ((eMainStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) && (eAuxStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER) && (eAuxFmt == AUD_FMT_DRA))
			{
				if (pfAudDecNfy != NULL)
				{
					_GetDraAudInfo(AUD_DEC_AUX, &rAudDecInfo);
                    pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_MAIN, AUD_COND_AUD_INFO_CHG,
                    (UINT32)&rAudDecInfo, u4Tmp);
					LOG(0, "EU Mode: Change source to DTV from other sources, DRA need notify MW info change!\n");
				}
			}
        }
    }
}
#endif //defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
static BOOL bMp3EncSt = FALSE;
void AUD_Aproc_SetMp3Enc(BOOL fgEnable)
{
    _vAUD_Aproc_SetMp3Enc(fgEnable);
	bMp3EncSt = fgEnable;
}

BOOL AUD_Aproc_GetMp3Enc(void)
{
	return bMp3EncSt;
}

UINT32 AudDecId2MixerId(UINT8 u1DecId)
{
	UINT32 u4Ret;
	AUD_DEC_STREAM_FROM_T eStreamFrom;

	switch (u1DecId)
	{
		case AUD_DEC_MAIN:
			u4Ret = 0;
			break;
		case AUD_DEC_AUX:
			u4Ret = 1;
			break;
		case AUD_DEC_THIRD:
			eStreamFrom = _AudGetStrSource(u1DecId);
			if (eStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER)
				u4Ret = 4;
			else
				u4Ret = 2;
			break;
		case AUD_DEC_4TH:
			u4Ret = 3;
			break;
		default:
			u4Ret = 0;
			break;
	}

	return u4Ret;
}
#endif

#ifndef CC_MAPLE_CUST_DRV
void DRVCUST_SendAudEvent(E_CUST_AUD_EVENT eCustEvent, UINT8 u1Option)
{
    UNUSED(eCustEvent);
}
#endif

#ifdef CC_SUPPORT_STR
void AUD_SetUiSpkOnOff(BOOL fgEnable)
{
    _fgAudSpeakerPortOn = fgEnable;
}
#endif

const CHAR* AUD_EnumToName(const AUD_ENUM_TO_NAME_T *eTable, UINT32 u4Size, UINT32 u4Value)
{
    UINT32 i;
    for (i = 0; i < u4Size; i++) 
    {
        if (eTable[i].u4Value == u4Value) 
        {
            return eTable[i].paName;
        }
    }
    return "";
}

void AUD_SetUserCommmand(UINT32 u4CmdType, UINT32 u4Index,
            UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4)
{
    LOG(8, "%s, type = %d index = %d \n", __FUNCTION__, u4CmdType, u4Index);
    switch (u4CmdType)
    {
#ifdef CC_AUD_DDI 
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)        
    case AUD_USER_SET_DEC_INPUT_MUTE:
        _AUD_UserSetDecInputMute(u4Index, u4Arg1); 
        break;        
    case AUD_USER_SET_DEC_INPUT_VOL:
        _AUD_UserSetDecInputVol(u4Index, u4Arg1, u4Arg2); 
        break;
    case AUD_USER_SET_DEC_OUTPUT_VOL:
        _AUD_UserSetDecOutputVol(u4Index, u4Arg1, u4Arg2); 
        break;
    case AUD_USER_SET_DEC_CHANNEL_GAIN:
        _AUD_UserSetDecChannelGain(u4Index, u4Arg1, u4Arg2, u4Arg3, u4Arg4);
        break;
    case AUD_USER_SET_MIX_INPUT_MUTE:
        _AUD_UserSetMixSndMute(u4Index, u4Arg1); 
        break;
    case AUD_USER_SET_MIX_INPUT_VOL:
        _AUD_UserSetMixSndInputVol(u4Index, u4Arg1, u4Arg2); 
        break;
    case AUD_USER_SET_MIX_OUTPUT_VOL:
        _AUD_UserSetMixSndOutputVol(u4Index, u4Arg1, u4Arg2); 
        break;
    case AUD_USER_SET_DEC_INPUT_DELAY:
        _AUD_UserSetDecInputDelay(u4Index, u4Arg1);
        break;
    case AUD_USER_SET_DEC_OUT_CTRL:
        _AUD_UserSetDecOutCtrl((AUD_OUT_PORT_T)u4Index, u4Arg1, (BOOL)u4Arg2);
        break;
    case AUD_USER_SET_SPDIF_RAW_DEC:
        _AUD_UserSetSpdifRawDec(u4Index);
        break;
#endif
#endif 
    default:
        break;
    } 
}


