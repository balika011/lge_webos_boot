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
 * $Date: 2015/01/25 $
 * $RCSfile: adac_if.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_if.c
 *  Audio interface setting, including codec and speaker mute/unmute, headphone detect mute control
 *  and codec/amp gpio control.
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "adac_if.h"
#include "aud_if.h"
#include "aud_debug.h"
#include "drvcust_if.h"
#include "x_pinmux.h"
#include "aud_cfg.h"
#include "codec_mt5398.h"

#include "adac_apogee.h"
#include "digiamp_others.h"
#include "codec_AD82581.h"
#include "digiamp_tas5707.h"
#include "digiamp_tas5717.h"
//#include "adac_ntp7400l.h"
#include "adac_sta381.h"
#include "codec_NE3201.h"
#include "adac_ntp7515.h"

#ifdef CC_AUD_USE_EXTRA_PERIPHERAL_IC
#include "periph_if.h"
#endif

LINT_EXT_HEADER_END

extern AMP_T* _AMP_GetAmpNTPFunTbl(void);

#if defined(CC_AUD_USE_NVM) && defined(CC_AUD_USE_NVM_AMP)
extern UINT32 AUD_NVM_Init_Amp(void);
#endif

extern void _AUD_SetOutPortHwMute(AUD_OUT_PORT_T ePort, BOOL fgMute, BOOL fgNotify);

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

CODEC_T* _prCodecTbl = NULL;
CODEC_T* _prCodecTblNUM2 = NULL;

AMP_T*   _prAmpTbl = NULL;
#ifdef CC_MAPLE_CUST_DRV
AMP_T*   _prAmpTbl2 = NULL;
#endif
//ADAC_T*  _prDacTbl = NULL;
//CODEC_T*  _prAdcTbl = NULL;


// Semaphore and message queue
static HANDLE_T _hSemaCodecSetting = NULL_HANDLE;
static HANDLE_T _hSemaAmpSetting = NULL_HANDLE;

static BOOL _fgCodecSystemMute = FALSE;
static BOOL _fgCodecMWMute = FALSE;
static BOOL _fgCodecHpDetectMute = FALSE;
static BOOL _fgCodecClkChangeMute = FALSE;
static BOOL _fgCodecHdmiUnstableMute = FALSE;

static BOOL _fgSpkHpDetectSpkEnableFlag = TRUE;
static BOOL _fgSpkDcDetectSpkEnableFlag = TRUE;
static BOOL _fgVolumeSpkEnableFlag = TRUE;
static BOOL _fgSpkGpioEnableFlag = FALSE;
static BOOL _fgSpkSystemEnableFlag = TRUE;
static BOOL _fgSpkHdmiUnstableEnableFlag = TRUE;
#if defined(CC_AUD_SX1_FEATURE) || defined (CC_MAPLE_CUST_DRV)
static BOOL _fgSpkHpLinkEnableFlag = TRUE;
#endif
#ifdef CC_AUD_SX1_FEATURE
static BOOL _fgSpkSetPEQEnableFlag = TRUE;
#endif
static BOOL _fgSpkEnable = FALSE;

static BOOL _fgSpeakerInit = FALSE;

#ifdef CC_DSP_SUPPORT_ARC
BOOL _fgArcEnableFlag = FALSE;
extern BOOL  fgSPDIFEnable;
#endif

#ifdef CC_SUPPORT_STR
extern BOOL fgAudPmStrResume;
#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static BOOL _IsUseMT5398(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32*)(void*)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eCodec1 == AUD_CODEC_MT5398)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL _IsUseApogee(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32*)(void*)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp1 == AUD_AMP_APOGEE_DDX_2051)
    {
        return TRUE;
    }
    else if (prAdacUsage->eAudAmp1 == AUD_AMP_STA339)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL _IsUseSTA381(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32*)(void*)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp1 == AUD_AMP_STA381)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseDigiAmpOthers(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32*)(void*)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp1 == AUD_AMP_OTHERS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseAD82581(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32*)(void*)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp1 == AUD_AMP_AD82581)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseDigiAmpNTP7400L(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32*)(void*)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp1 == AUD_AMP_NTP7400L)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseDigiAmpNTP7515(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32*)(void*)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp1 == AUD_AMP_NTP7515)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseDigiAmpTAS5707(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32*)(void*)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp1 == AUD_AMP_TAS5707)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL _IsUseDigiAmpTAS5717(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32*)(void*)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp1 == AUD_AMP_TAS5717)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseNE3201(void)
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32*)(void*)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp1 == AUD_AMP_NE3201)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOL _IsUseDrvCustAmp(void)  //AMP_DRV_CUST
{
    AUD_ADAC_USAGE_T* prAdacUsage;
    UNUSED(DRVCUST_OptQuery(eAudioDacUsage, (UINT32*)(void*)&prAdacUsage));

    if (prAdacUsage == NULL)
    {
        return FALSE;
    }

    if (prAdacUsage->eAudAmp1 == AUD_AMP_DRVCUST)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void _LoadCodecTble(void)
{
    if (_IsUseMT5398())
    {
        _prCodecTbl = _CODEC_GetMT5398Tbl();
    }
}

static void _LoadAmpTble(void)
{
    if (_IsUseApogee())
    {
        _prAmpTbl = _AMP_GetAmpApogeeFunTbl();
    }
    else if (_IsUseAD82581())
    {
        _prAmpTbl = _AMP_GetAmp82581FunTbl();
    }
    else if (_IsUseDigiAmpOthers())
    {
        _prAmpTbl = _AMP_GetAmpCustFunTbl();
    }
    else if (_IsUseDigiAmpTAS5707())
    {
        _prAmpTbl = _AMP_GetAmpTAS5707FunTbl();
    }
    else if (_IsUseDigiAmpTAS5717())
    {
        _prAmpTbl = _AMP_GetAmpTAS5717FunTbl();
    }
    else if (_IsUseDigiAmpNTP7400L())
    {
        _prAmpTbl = _AMP_GetAmpNTPFunTbl();
    }
	else if (_IsUseDigiAmpNTP7515())
    {
        _prAmpTbl = _AMP_GetAmpNTP7515FunTbl();
    }
    else if (_IsUseSTA381())
    {
        _prAmpTbl = _AMP_GetAmpSTA381FunTbl();
    }
    else if (_IsUseNE3201())
    {
        _prAmpTbl = _AMP_GetAmpNE3201FunTbl();
    }
    else if (_IsUseDrvCustAmp())   //AMP_DRV_CUST
    {
        _prAmpTbl = AUD_AMP_GetDrvCustFunTbl();
    }
}

static void _AdacSpeakerChangeState(void)
{
    BOOL fgEnableFlag;
#if defined(CC_AUD_SX1_FEATURE) || defined (CC_MAPLE_CUST_DRV)
    BOOL fgLinkHpFlag;
    if (_fgSpkHpLinkEnableFlag)
    {
        fgLinkHpFlag = _fgSpkHpDetectSpkEnableFlag;
    }
    else
    {
        fgLinkHpFlag = TRUE;
    }

    fgEnableFlag = fgLinkHpFlag && _fgSpkGpioEnableFlag &&
                   _fgVolumeSpkEnableFlag && _fgSpkSystemEnableFlag &&
                   _fgSpkDcDetectSpkEnableFlag && _fgSpkHdmiUnstableEnableFlag;
#else
    fgEnableFlag = _fgSpkHpDetectSpkEnableFlag && _fgSpkGpioEnableFlag &&
                   _fgVolumeSpkEnableFlag && _fgSpkSystemEnableFlag &&
                   _fgSpkDcDetectSpkEnableFlag && _fgSpkHdmiUnstableEnableFlag;
#endif

    if ((fgEnableFlag != _fgSpkEnable) || (!_fgSpeakerInit))
    {
        if (fgEnableFlag) //unmute amp
        {
            // 1. hardware unmute first
            //ADAC_GpioAmpEnable(fgEnableFlag);
            // 2. software unmute
            if (_prAmpTbl != NULL)
            {
                if (_prAmpTbl->prAmpFunc->pfnMute != NULL)
                {
                    _prAmpTbl->prAmpFunc->pfnMute(!fgEnableFlag);
                }
            }
#ifdef CC_MAPLE_CUST_DRV
            AUD_DspChannelMute(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_FRONT_LEFT, FALSE);
            AUD_DspChannelMute(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_FRONT_RIGHT, FALSE);
            ADAC_GpioAmpEnable(fgEnableFlag);
#else
            else
            {
                ADAC_GpioAmpEnable(fgEnableFlag);
            }
#endif            
        }
        else //mute amp
        {
            // 1. software mute first
            if (_prAmpTbl != NULL)
            {
                if (_prAmpTbl->prAmpFunc->pfnMute != NULL)
                {
                    _prAmpTbl->prAmpFunc->pfnMute(!fgEnableFlag);
                }
            }
#ifdef CC_MAPLE_CUST_DRV
            ADAC_GpioAmpEnable(fgEnableFlag);
            AUD_DspChannelMute(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_FRONT_LEFT, TRUE);
            AUD_DspChannelMute(AUD_DSP0, AUD_DEC_MAIN, AUD_CH_FRONT_RIGHT, TRUE);
#else
            else
            {
                ADAC_GpioAmpEnable(fgEnableFlag);
            }
#endif            
            // 2. hardware mute
            //ADAC_GpioAmpEnable(fgEnableFlag);
        }
        _AUD_SetOutPortHwMute(AUD_SPEAKER, !fgEnableFlag, TRUE);
        LOG(8, "**** _AdacSpeakerChangeState = %x , 1: Unmute, 0: Mute\n", fgEnableFlag);
    }

    _fgSpeakerInit = TRUE;
    _fgSpkEnable = fgEnableFlag;
}

void ADAC_SpeakerStateQuery(void)
{
    Printf("Speaker enable state:\n");
#if defined(CC_AUD_SX1_FEATURE) || defined (CC_MAPLE_CUST_DRV)
    Printf("_fgSpkHpLinkEnableFlag = %d\n", _fgSpkHpLinkEnableFlag);
#endif
    Printf("_fgSpkHpDetectSpkEnableFlag = %d\n", _fgSpkHpDetectSpkEnableFlag);
    Printf("_fgSpkGpioEnableFlag = %d\n", _fgSpkGpioEnableFlag);
    Printf("_fgVolumeSpkEnableFlag = %d\n", _fgVolumeSpkEnableFlag);
    Printf("_fgSpkSystemEnableFlag = %d\n", _fgSpkSystemEnableFlag);
    Printf("_fgSpkDcDetectSpkEnableFlag = %d\n", _fgSpkDcDetectSpkEnableFlag);
    Printf("_fgSpkHdmiUnstableEnableFlag = %d\n", _fgSpkHdmiUnstableEnableFlag);
}

BOOL ADAC_AmpEnableFlag(void)
{
    UNUSED(ADAC_AmpEnableFlag);
    return (_fgSpkHpDetectSpkEnableFlag && _fgSpkGpioEnableFlag &&
            _fgVolumeSpkEnableFlag && _fgSpkSystemEnableFlag &&
            _fgSpkDcDetectSpkEnableFlag && _fgSpkHdmiUnstableEnableFlag);
}

static void _AdacCodecChangeState(void)
{
    BOOL fgMuteFlag;

    fgMuteFlag = _fgCodecSystemMute || _fgCodecMWMute || _fgCodecHpDetectMute || _fgCodecClkChangeMute || _fgCodecHdmiUnstableMute;

    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnMute != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnMute(3, fgMuteFlag);
        }
    }
    if (_prCodecTblNUM2 != NULL)
    {
        if (_prCodecTblNUM2->prCodecFuncTbl->pfnMute != NULL)
        {
            _prCodecTblNUM2->prCodecFuncTbl->pfnMute(3, fgMuteFlag);
        }
    }

    _AUD_SetOutPortHwMute(AUD_HEADPHONE, fgMuteFlag, TRUE);
    _AUD_SetOutPortHwMute(AUD_AV_OUT, fgMuteFlag, TRUE);
    _AUD_SetOutPortHwMute(AUD_MONITOR_SCART, fgMuteFlag, TRUE);
    //ADAC_GpioCodecMute(fgMuteFlag);
    LOG(8, "**** _AdacCodecChangeState = %x , 1: Mute, 0: Unmute\n", fgMuteFlag);
}

static void _AdacSwInit(void)
{
    if (_hSemaCodecSetting == NULL_HANDLE)
    {
        VERIFY(x_sema_create(&_hSemaCodecSetting, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    if (_hSemaAmpSetting == NULL_HANDLE)
    {
        VERIFY(x_sema_create(&_hSemaAmpSetting, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);
    }
}

static void _AdacTableInit(void)
{
    static BOOL fgInit = FALSE;

    if (!fgInit)
    {
        _LoadCodecTble();
        _LoadAmpTble();
        _AdacSwInit();
        fgInit = TRUE;
    }
}

//---------------------------------------------------------------------------
// Global functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ADAC_Init
//
/** Initial audio codec and digital amp.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_Init(void)
{
//#ifdef CC_MT5399
#if 0
    // to fix mt5399 linux demo program spk no sound issue
    static BOOL fgInit = FALSE;
    if (fgInit)
    {
        return;
    }    
    else
    {
        fgInit = TRUE;
    }
#endif
    
    //LOG(0, "\ADAC_Init\n");
    if (_prAmpTbl != NULL)
    {
        if (_prAmpTbl->prAmpFunc->pfnInit != NULL)
        {
            _prAmpTbl->prAmpFunc->pfnInit();
            _fgSpeakerInit = TRUE;
//#ifdef CC_AUD_SX1_FEATURE
            _fgSpkEnable = TRUE;
//#endif
        }
    }
#ifdef CC_MAPLE_CUST_DRV
    if (_prAmpTbl2 != NULL)
    {
        if (_prAmpTbl2->prAmpFunc->pfnInit != NULL)
        {
            _prAmpTbl2->prAmpFunc->pfnInit();
        }
    }
#endif      

    /// Set Mute after I2S initialize
#ifdef CC_SUPPORT_STR
    if (!fgAudPmStrResume)
    {
        ADAC_GpioCodecMute(TRUE);
    }
#else
    ADAC_GpioCodecMute(TRUE);
#endif

    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnInit != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnInit();
        }
    }

    if (_prCodecTblNUM2 != NULL)
    {
        if (_prCodecTblNUM2->prCodecFuncTbl->pfnInit != NULL)
        {
            _prCodecTblNUM2->prCodecFuncTbl->pfnInit();
        }
    }

    _AUD_SetOutPortHwMute(AUD_SPEAKER, !_fgSpkGpioEnableFlag, FALSE);
#ifdef CC_AUD_USE_EXTRA_PERIPHERAL_IC
    PERIPH_Init();
#endif
}

//-----------------------------------------------------------------------------
// I2S_AMP_Init
//
/** Initial Audio I2S digital amplifier.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void I2S_AMP_Init(void)
{
    static BOOL bAmpTblInit = FALSE;
    UNUSED(I2S_AMP_Init);
    UNUSED (bAmpTblInit);
#ifdef CC_MAPLE_CUST_DRV
    if(!bAmpTblInit)
    {
        DrvCust_AQ_SetGet(AUD_DrvCust_AQ_Get_DA_5760_SET, _prAmpTbl, _prAmpTbl2, 0/*key*/, 0);
        bAmpTblInit = TRUE;
    }
#endif
    if (_prAmpTbl != NULL)
    {
        if (_prAmpTbl->prAmpFunc->pfnInit != NULL)
        {
            _prAmpTbl->prAmpFunc->pfnInit();
        }
    }
#ifdef CC_MAPLE_CUST_DRV
    if (_prAmpTbl2 != NULL)
    {
        if (_prAmpTbl2->prAmpFunc->pfnInit != NULL)
        {
            _prAmpTbl2->prAmpFunc->pfnInit();
        }
    }
#endif  
    
    LOG(1, "Audio I2S_AMP_Init Done!!!\n");
}

//-----------------------------------------------------------------------------
// ADAC_Enable
//
/** Audio codec and amp function table init.
 *
 *  @param[in]     fgEnable   TRUE for initial.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_Enable(BOOL fgEnable)
{
    UNUSED(fgEnable);
    _AdacTableInit();
    // TODO !!
    /*
        if (_prDacTbl != NULL)
        {
            if (_prDacTbl->prDacFuncTbl->pfnEnable != NULL)
            {
                _prDacTbl->prDacFuncTbl->pfnEnable(fgEnable);
            }
        }
    */
}

//-----------------------------------------------------------------------------
// ADAC_Mute
//
/** Audio codec and speaker mute.
 *
 *  @param[in]     fgEnable   "TRUE" for mute,"FALSE" for unmute.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_Mute(BOOL fgEnable)
{
#ifdef CC_AUD_SX1_FEATURE
    static BOOL _fgSpkSystemEnablePreFlag = TRUE;
#endif
    _AdacTableInit();

    // Driver will mute codec for internal parameter setting
    VERIFY(x_sema_lock(_hSemaCodecSetting, X_SEMA_OPTION_WAIT) == OSR_OK);
    _fgCodecSystemMute = fgEnable;
    _AdacCodecChangeState();
    VERIFY(x_sema_unlock(_hSemaCodecSetting) == OSR_OK);

#if 0//Darren Weng(because amplify mute funetion always has pop noise, so we don't suggest use amplfiy mue when change sample rate,let's just mute DAC PGA mute funtion.
    // Driver will mute Amp (speaker) for internal parameter setting
    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);
    _fgSpkSystemEnableFlag = !fgEnable;
    _AdacSpeakerChangeState();
    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);
#endif
#ifdef CC_AUD_SX1_FEATURE
    //added by gallen, 20100718, Waffle_PAA sys build on MTK board power-on speaker no sound problem.
    if (_fgSpkSystemEnablePreFlag != _fgSpkSystemEnableFlag)
    {
        _AdacSpeakerChangeState();
        _fgSpkSystemEnablePreFlag = _fgSpkSystemEnableFlag;
    }
#endif
}

//-----------------------------------------------------------------------------
// ADAC_MuteCLK
//
/** Audio codec Mute PWM CLK - PWM Fade-in / Fade-out
 *
 *  @param[in]     fgEnable   "TRUE" for mute,"FALSE" for unmute.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_MuteCLK(BOOL fgEnable)
{
    static BOOL fgMute = FALSE;

    LOG(7, "ADAC_MuteCLK, fgEnable = %d\n", fgEnable);
    
    if (fgMute == fgEnable)
    {
        return;
    }

    fgMute = fgEnable;
    
    _AdacTableInit();

    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnMuteClk != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnMuteClk(AUD_DEC_MAIN, fgEnable);
        }
    }
}

//-----------------------------------------------------------------------------
// ADAC_DacFormatCfg
//
/** Setting DAC Data Format.
 *
 *  @param[in]     u1DecId   Decoder ID.
 *  @param[in]     eFormat   Audio data format,ex:LJ,RJ,I2S.
 *  @param[in]     eMclk   MCLK frequency.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_DacFormatCfg(UINT8 u1DecId, DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    UNUSED(u1DecId);
    _AdacTableInit();

    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnSetDacInFmt != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnSetDacInFmt(eFormat, eMclk);
        }
    }
    if (_prAmpTbl != NULL)
    {
        if (_prAmpTbl->prAmpFunc->pfnSetDacInFmt != NULL)
        {
            _prAmpTbl->prAmpFunc->pfnSetDacInFmt(eFormat, eMclk);
        }
    }
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// ADAC_AdcFormatCfg
//
/** Setting Codec Adc Data Format.
 *
 *  @param[in]     u1DecId   Decoder ID.
 *  @param[in]     eFormat   Audio data format,ex:LJ,RJ,I2S.
 *  @param[in]     eMclk   MCLK frequency.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_AdcFormatCfg(UINT8 u1DecId, DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
    UNUSED(u1DecId);
    _AdacTableInit();

    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnSetAdcOutFmt != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnSetAdcOutFmt(eFormat, eMclk);
        }
    }
}
#endif

//-----------------------------------------------------------------------------
// ADAC_HeadphoneVolCtl
//
/** Setting Codec Adc Data Format.
 *
 *  @param[in]     i2VolInHalfDb   volume value.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_HeadphoneVolCtl(INT16 i2VolInHalfDb)
{
    _AdacTableInit();
    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnSetHpGain != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnSetHpGain(0, i2VolInHalfDb);
        }
    }
}

//-----------------------------------------------------------------------------
// ADAC_GetHeadphoneVolMaxMin
/**  Setting the max and min value of headphone volume.
*
*  @param[in]     pi2Max   volume max value.
*  @param[in]     pi2Min   volume min value.
*
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_GetHeadphoneVolMaxMin(INT16* pi2Max, INT16* pi2Min)
{
    _AdacTableInit();
    //lint -e{613}
    if (_prCodecTbl != NULL)
    {
        *pi2Max = _prCodecTbl->i2MaxHpGain;
        *pi2Max = _prCodecTbl->i2MinHpGain;
    }
    else
    {
        *pi2Max = 0xff;
        *pi2Min = 0;
    }
}

//-----------------------------------------------------------------------------
// ADAC_DacChlSel
//
/** Configure DAC output channel
 *
 *  @param[in]     u1DacId: Dac ID, eChl: output channel
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_DacSetChl(UINT8 u1DacId, AUD_CHL_DEF_T eChl)
{
    _AdacTableInit();
    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnSetDacChl != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnSetDacChl(u1DacId, eChl);
        }
    }
}

//-----------------------------------------------------------------------------
// ADAC_ADCDown
//
/** Power on/down ADC
 *
 *  @param[in]     fgEnable: True: power on, FALSE: power off
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_ADCDown(BOOL fgEnable)
{
    _AdacTableInit();
    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnADCDown != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnADCDown(fgEnable);
        }
    }
}

//-----------------------------------------------------------------------------
// ADAC_DacChlSel
//
/** Get Dac output channel  config
 *
 *  @param[in]     void
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_DacGetChl(void)
{
    _AdacTableInit();
    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnGetDacChl != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnGetDacChl();
        }
    }
}

//-----------------------------------------------------------------------------
// ADAC_VolumeMuteSpeakerEnable
//
/** Mute speaker when volume value equal to zero.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_VolumeMuteSpeakerEnable(BOOL fgFlag)
{
    static BOOL fgInit = FALSE;
    //BOOL fgSpkEnable;

    _AdacTableInit();

    //fgSpkEnable = !fgFlag;
    if ((_fgVolumeSpkEnableFlag != fgFlag) || (!fgInit))
    {
        _fgVolumeSpkEnableFlag = fgFlag;
        _AdacSpeakerChangeState();
    }

    fgInit = TRUE;
}

//-----------------------------------------------------------------------------
// ADAC_DcDetectSpkEnable
//
/** Detect the DC and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_DcDetectSpkEnable(BOOL fgFlag)
{
    static BOOL fgInit = FALSE;

    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    if ((_fgSpkDcDetectSpkEnableFlag != fgFlag) || (!fgInit))
    {
        _fgSpkDcDetectSpkEnableFlag = fgFlag;
        _AdacSpeakerChangeState();
    }

    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);

    fgInit = TRUE;
}

//-----------------------------------------------------------------------------
// ADAC_HdmiUnstableSpkEnable
//
/** HDMI signal unstable and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_HdmiUnstableSpkEnable(BOOL fgFlag)
{
    static BOOL fgInit = FALSE;

    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    if ((_fgSpkHdmiUnstableEnableFlag != fgFlag) || (!fgInit))
    {
        _fgSpkHdmiUnstableEnableFlag = fgFlag;
        _AdacSpeakerChangeState();
    }

    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);

    fgInit = TRUE;
}

#ifdef CC_AUD_SX1_FEATURE
void ADAC_SetPEQSpkEnable(BOOL fgFlag)
{
    static BOOL fgInit = FALSE;

    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    if ((_fgSpkSetPEQEnableFlag != fgFlag) || (!fgInit))
    {
        _fgSpkSetPEQEnableFlag = fgFlag;
        _AdacSpeakerChangeState();
    }

    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);

    fgInit = TRUE;
}
#endif 
//-----------------------------------------------------------------------------
// ADAC_SpkHpLinkEnable
//
/** Speaker & headphone link enable.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------

#if defined(CC_AUD_SX1_FEATURE) || defined (CC_MAPLE_CUST_DRV)
void ADAC_SpkHpLinkEnable(BOOL fgFlag)
{
    static BOOL fgInit = FALSE;

#ifdef CC_MAPLE_CUST_DRV
    gSoundSetting.eHpSpLink = (E_CUST_AUD_HP_SP_LINK)fgFlag;   
    DRVCUST_SendAudEvent(E_CUST_AUD_SOUND_SETTING_CHANGE, 0);    
#endif    
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    if ((_fgSpkHpLinkEnableFlag != fgFlag) || (!fgInit))
    {
        _fgSpkHpLinkEnableFlag = fgFlag;
        _AdacSpeakerChangeState();
    }

    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);

    fgInit = TRUE;
}
#endif
//-----------------------------------------------------------------------------
// ADAC_HpDetectSpkEnable
//
/** Detect the headphone plug in and mute speaker.
 *
 *  @param[in]     fgFlag   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_HpDetectSpkEnable(BOOL fgFlag)
{
    static BOOL fgInit = FALSE;

    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    if ((_fgSpkHpDetectSpkEnableFlag != fgFlag) || (!fgInit))
    {
        _fgSpkHpDetectSpkEnableFlag = fgFlag;
        _AdacSpeakerChangeState();
    }

    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);

    fgInit = TRUE;
}

//-----------------------------------------------------------------------------
// ADAC_HpDetectCodecMute
//
/** Detect the headphone plug in and mute codec.
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_HpDetectCodecMute(BOOL fgMute)
{
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaCodecSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    _fgCodecHpDetectMute = fgMute;
    _AdacCodecChangeState();

    VERIFY(x_sema_unlock(_hSemaCodecSetting) == OSR_OK);
}

//-----------------------------------------------------------------------------
// ADAC_HpDetectDacMute
//
/** Detect the headphone plug in and mute internaldac.
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_HpDetectDacMute(BOOL fgMute)
{
    UNUSED(fgMute);
    _AdacTableInit();

    //    VERIFY(x_sema_lock(_hSemaDacSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    //    _fgDacHpDetectMute = fgMute;
    //    _AdacDacChangeState();

    //    VERIFY(x_sema_unlock(_hSemaDacSetting) == OSR_OK);

    //    VERIFY(x_sema_lock(_hSemaAdcSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    //    _fgAdcHpDetectMute = fgMute;
    //    _AadcAdcChangeState();

    //    VERIFY(x_sema_unlock(_hSemaAdcSetting) == OSR_OK);
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// ADAC_ClkChangeCodecMute
//
/** Some codec have to mute before clock source change
 *
 *  @param[in]     fgMute   "TRUE" for enable this function.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_ClkChangeCodecMute(BOOL fgMute)
{
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaCodecSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    // Mute first ..
    _fgCodecClkChangeMute = fgMute;
    _AdacCodecChangeState();

    VERIFY(x_sema_unlock(_hSemaCodecSetting) == OSR_OK);
}
#endif

//-----------------------------------------------------------------------------
// ADAC_SpeakerEnable
//
/** Speaker enable routine.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute speaker.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_SpeakerEnable(BOOL fgEnable)
{
    static BOOL fgInit = FALSE;

    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaAmpSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    if ((_fgSpkGpioEnableFlag != fgEnable) || (!fgInit))
    {
        _fgSpkGpioEnableFlag = fgEnable;
        _AdacSpeakerChangeState();
    }

    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);

    fgInit = TRUE;
}

//-----------------------------------------------------------------------------
// ADAC_SpeakerState_Reinit
//
/** Speaker State Reinit
 *
 *  Call at _SetOutPort for pwmdac de-pop noise flowchart which make
 *  - System boot & headphone plug-in, speaker un-mute (I2S non-mute)
 *  - UI Audio Speaker:Off & system re-boot speaker un-mute (I2S non-mute)
 *
 */
//-----------------------------------------------------------------------------
void ADAC_SpeakerState_Reinit(void)
{
    _fgSpeakerInit = FALSE;
    _AdacSpeakerChangeState();
}

#ifdef CC_DSP_SUPPORT_ARC
// ADAC_ArcEnable
//
/** HDMI ARC enable routine.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute speaker.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_ArcEnable(BOOL fgEnable)
{
    if (_fgArcEnableFlag != fgEnable)
    {
        _fgArcEnableFlag = fgEnable;

        if (! fgSPDIFEnable)
        {
            return;
        }

#if defined(CC_MT5398) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        if (fgEnable)
        {
            BSP_PinSet(PIN_ASPDIF0, PINMUX_FUNCTION1);
        }
        else
        {
            BSP_PinSet(PIN_ASPDIF0, PINMUX_FUNCTION0);
        }
#elif defined(CC_MT5880)
        if (fgEnable)
        {
            vIO32WriteFldAlign(CKGEN_PMUX1, 3, FLD_PAD_ASPDIF1);
        }
        else
        {
            vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_ASPDIF1);
        }
#elif defined(CC_MT5881) //may be fixed, paul
        if (fgEnable)
        {
            vIO32WriteFldAlign(CKGEN_PMUX1, 3, FLD_PAD_ASPDIF1);
        }
        else
        {
            vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_ASPDIF1);
        }
#elif defined(CC_MT5396)
        if (fgEnable)
        {
            vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF1);
        }
        else
        {
            vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_ASPDIF1);
        }
#elif defined(CC_MT5368)
        if (fgEnable)
        {
            vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_ASPDIF1);
        }
        else
        {
            vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_ASPDIF1);
        }
#elif defined(CC_MT5389)
        //NO SPDIF1
#else
        INT32 i4RegArcSwitch;
        UINT32 u4GpioArc;
        AUD_GPIO_POLARITY_T eArcPolariy;
        u4GpioArc = DRVCUST_OptGet(eAudioArcEnableGpioNum);
        if (u4GpioArc != 0xffffffff)
        {
            eArcPolariy = (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioArcGpioPolarity);
            if (eArcPolariy == AUD_GPIO_HIGH_ENALBE)
            {
                i4RegArcSwitch = fgEnable ? 1 : 0;
            }
            else
            {
                i4RegArcSwitch = fgEnable ? 0 : 1;
            }
            AUD_GPIOOutput(u4GpioArc, i4RegArcSwitch);
        }
#endif
    }
}
#endif

//-----------------------------------------------------------------------------
// ADAC_CodecMute
//
/** Codec enable routine.
 *
 *  @param[in]     fgMute   "TRUE" for enable, "FALSE" for mute codec.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_CodecMute(BOOL fgMute)
{
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaCodecSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    _fgCodecMWMute = fgMute;
    _AdacCodecChangeState();

    VERIFY(x_sema_unlock(_hSemaCodecSetting) == OSR_OK);
}

#if 0   // Unused
//-----------------------------------------------------------------------------
// ADAC_CodecHdmiUnstableMute
//
/** Codec mute when hdmi signal unstable.
 *
 *  @param[in]     fgMute   "TRUE" for enable, "FALSE" for mute codec.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_CodecHdmiUnstableMute(BOOL fgMute)
{
    _AdacTableInit();

    VERIFY(x_sema_lock(_hSemaCodecSetting, X_SEMA_OPTION_WAIT) == OSR_OK);

    _fgCodecHdmiUnstableMute = fgMute;
    _AdacCodecChangeState();

    VERIFY(x_sema_unlock(_hSemaCodecSetting) == OSR_OK);
}
#endif


//-----------------------------------------------------------------------------
// ADAC_GpioCodecMute
//
/** Codec GPIO control for mute/unmute.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute codec from GPIO.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_GpioCodecMute(BOOL fgEnable)
{
    INT32 i4RegAoutSwitch;
    UINT32 u4GpioMute;
    UINT32 u4OpctrlGpio = UNUSED_GPIO;
    AUD_MUTE_GPIO_POLARITY_T eMutePolariy;
    AUD_OPCTRL_MUTE_GPIO_POLARITY_T eOpctlMutePolariy;
    AUD_GPIO_POLARITY_T eGpioPalarity;
    static BOOL lastState = FALSE;
    static HANDLE_T codec_sema = NULL_HANDLE;
    static BOOL isUnmuteScart = 0;

    if (NULL_HANDLE == codec_sema)
    {
        VERIFY(x_sema_create(&codec_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    VERIFY(x_sema_lock(codec_sema, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (lastState == fgEnable)
    {
        VERIFY(x_sema_unlock(codec_sema) == OSR_OK);
        return;
    }

    eMutePolariy = (AUD_MUTE_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioMuteGpioPolarity);
    if (eMutePolariy == AUD_MUTE_GPIO_HIGH_ENALBE)
    {
        i4RegAoutSwitch = fgEnable ? 1 : 0;
    }
    else
    {
        i4RegAoutSwitch = fgEnable ? 0 : 1;
    }
    u4GpioMute = DRVCUST_OptGet(eAudioMuteGpioNum);
    if (u4GpioMute != 0xffffffff)
    {
        AUD_GPIOOutput(u4GpioMute, i4RegAoutSwitch);
    }

    u4OpctrlGpio = DRVCUST_OptGet(eAudioOpctrlMuteGpioNum);
    if (u4OpctrlGpio != 0xffffffff)
    {
        eOpctlMutePolariy = (AUD_OPCTRL_MUTE_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioOpctrlMuteGpioPolarity);
        if (eOpctlMutePolariy == AUD_OPCTRL_MUTE_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegAoutSwitch = fgEnable ? 0 : 1;
        }

        AUD_GPIOOutput(u4OpctrlGpio, i4RegAoutSwitch);
    }

    //AUD_GPIOOutput(AUD_GPIO_DAC_MUTE_CTL, i4RegAoutSwitch);

    if( (!fgEnable) && (!isUnmuteScart))
    {
        u4OpctrlGpio = DRVCUST_OptGet(eAudioScartEnableGpioNum);
        if (u4OpctrlGpio != 0xffffffff)
        {
           eGpioPalarity = (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioSCcartGpioPolarity);
           if (eGpioPalarity == AUD_GPIO_HIGH_ENALBE)
           {
               i4RegAoutSwitch = 1;
           }
           else
           {
               i4RegAoutSwitch = 0;
           }             
           AUD_GPIOOutput(u4OpctrlGpio, i4RegAoutSwitch);
        }
        isUnmuteScart = 1;
    }
#ifdef CC_SUPPORT_STR
    if (!fgAudPmStrResume)
#endif
    {
        if (_IsUseAD82581() && (TRUE == lastState) && (FALSE == fgEnable))
        {
            x_thread_delay(20);
        }
    }
    lastState = fgEnable;
    VERIFY(x_sema_unlock(codec_sema) == OSR_OK);

}

//-----------------------------------------------------------------------------
// ADAC_GpioAmpEnable
//
/** Speaker GPIO control for mute/unmute.
 *
 *  @param[in]     fgEnable   "TRUE" for enable, "FALSE" for mute speaker from GPIO.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_GpioAmpEnable(BOOL fgEnable)
{
    INT32 i4RegAoutSwitch;
    UINT32 u4GpioAmp;
    AUD_AMP_GPIO_POLARITY_T eAmpPolariy;
    static BOOL lastState = FALSE;
    static HANDLE_T amp_sema = NULL_HANDLE;

    if (NULL_HANDLE == amp_sema)
    {
        VERIFY(x_sema_create(&amp_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    VERIFY(x_sema_lock(amp_sema, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (lastState == fgEnable)
    {
        VERIFY(x_sema_unlock(amp_sema) == OSR_OK);
        return;
    }

#ifdef TPA3110D2
    static BOOL isFirstMute = 1;
    if (isFirstMute && fgEnable)
    {
        x_thread_delay(500);
        isFirstMute = 0;
    }
#endif
    u4GpioAmp = DRVCUST_OptGet(eAudioAmpEnableGpioNum);
    if (u4GpioAmp != 0xffffffff)
    {
        eAmpPolariy = (AUD_AMP_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioAmpGpioPolarity);
        if (eAmpPolariy == AUD_AMP_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegAoutSwitch = fgEnable ? 0 : 1;
        }

        AUD_GPIOOutput(u4GpioAmp, i4RegAoutSwitch);        
    }

#ifdef CC_SUPPORT_STR
    if (!fgAudPmStrResume)
#endif
    {
        if (_IsUseAD82581() && (FALSE == lastState) && (TRUE == fgEnable))
        {
            x_thread_delay(500);
        }
    }
    lastState = fgEnable;

    VERIFY(x_sema_unlock(amp_sema) == OSR_OK);
}

void ADAC_GpioAmpEnable1(BOOL fgEnable)
{
    INT32 i4RegAoutSwitch;
    UINT32 u4GpioAmp;
    AUD_AMP_GPIO_POLARITY_T eAmpPolariy;

#ifdef TPA3110D2
    static BOOL isFirstMute = 1;
    if (isFirstMute && fgEnable)
    {
        x_thread_delay(500);
        isFirstMute = 0;
    }
#endif
    u4GpioAmp = DRVCUST_OptGet(eAudioAmpEnableGpioNum);
    if (u4GpioAmp != 0xffffffff)
    {
        eAmpPolariy = (AUD_AMP_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioAmpGpioPolarity);
        if (eAmpPolariy == AUD_AMP_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegAoutSwitch = fgEnable ? 0 : 1;
        }

        AUD_GPIOOutput(u4GpioAmp, i4RegAoutSwitch);
    }
}

//-----------------------------------------------------------------------------
// ADAC_CodecAdChlSel
//
/** Codec ad channel select.
 *
 *  @param[in]     u1Chl   Input data channel.
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void ADAC_CodecAdChlSel(UINT8 u1Chl)
{
    _AdacTableInit();

    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnSetAdcMuxSel != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnSetAdcMuxSel(u1Chl);
        }
    }
}

//-----------------------------------------------------------------------------
// ADAC_CodecRegRead
//
/** Read register of external codec.
 *
 *  @param[in]     u1Offset   Address.
 *
 *  @retval     UINT8   Data.
 */
//-----------------------------------------------------------------------------
UINT8 ADAC_CodecRegRead(UINT8 u1Offset)
{
    _AdacTableInit();
    if ((_prCodecTbl != NULL) &&
        (_prCodecTbl->prCodecFuncTbl->pfnRegRead != NULL))
    {
        return _prCodecTbl->prCodecFuncTbl->pfnRegRead(u1Offset);
    }
    else
    {
        return 0xff;
    }
}

//-----------------------------------------------------------------------------
// ADAC_CodecRegWrite
//
/** Write register of external codec.
 *
 *  @param[in]     u1Offset     Address.
 *  @param[in]     u1Data       Data.
 *
 *  @retval     void.
 */
//-----------------------------------------------------------------------------
void ADAC_CodecRegWrite(UINT8 u1Offset, UINT8 u1Data)
{
    _AdacTableInit();

    if (_prCodecTbl != NULL)
    {
        if (_prCodecTbl->prCodecFuncTbl->pfnRegWrite != NULL)
        {
            _prCodecTbl->prCodecFuncTbl->pfnRegWrite(u1Offset, u1Data);
        }
    }
}

//-----------------------------------------------------------------------------
// ADAC_AmpRegRead
//
/** Read register of external digital amp.
 *
 *  @param[in]     u1Offset   Address.
 *
 *  @retval     UINT8   Data.
 */
//-----------------------------------------------------------------------------
UINT8 ADAC_AmpRegRead(UINT8 u1Offset)
{
    _AdacTableInit();

    if ((_prAmpTbl != NULL) && (_prAmpTbl->prAmpFunc->pfnRegRead != NULL))
    {
        return _prAmpTbl->prAmpFunc->pfnRegRead(u1Offset);
    }
    else
    {
        return 0xff;
    }
}

//-----------------------------------------------------------------------------
// ADAC_AmpRegWrite
//
/** Write register of external digital amplify.
 *
 *  @param[in]     u1Offset     Address.
 *  @param[in]     u1Data       Data.
 *
 *  @retval     void.
 */
//-----------------------------------------------------------------------------
void ADAC_AmpRegWrite(UINT8 u1Offset, UINT8 u1Data)
{
    _AdacTableInit();

    if (_prAmpTbl != NULL)
    {
        if (_prAmpTbl->prAmpFunc->pfnRegWrite != NULL)
        {
            _prAmpTbl->prAmpFunc->pfnRegWrite(u1Offset, u1Data);
        }
    }
}

void ADAC_AmpInit(void)
{
    _AdacTableInit();

    if (_prAmpTbl != NULL)
    {
        if (_prAmpTbl->prAmpFunc->pfnInit!= NULL)
        {
            _prAmpTbl->prAmpFunc->pfnInit();
        }
    }
#ifdef CC_MAPLE_CUST_DRV
    if (_prAmpTbl2 != NULL)
    {
        if (_prAmpTbl2->prAmpFunc->pfnInit!= NULL)
        {
            _prAmpTbl2->prAmpFunc->pfnInit();
        }
    }      
#endif    

    // init amp from nvm
#if defined(CC_AUD_USE_NVM) && defined(CC_AUD_USE_NVM_AMP)
    AUD_NVM_Init_Amp();
#endif

    // init speaker
    ADAC_SpeakerState_Reinit();
}

void ADAC_AmpMute(UINT8 u1Data)
{
    _AdacTableInit();

    if (_prAmpTbl != NULL)
    {
        if (_prAmpTbl->prAmpFunc->pfnMute != NULL)
        {
            _prAmpTbl->prAmpFunc->pfnMute(u1Data);
        }
    }
}

#if defined(CC_FAST_INIT) || defined(CC_SUPPORT_STR)
//-----------------------------------------------------------------------------
// ADAC_Suspend
//
/** Unlock sema
 *
 *  @retval     void.
 */
//-----------------------------------------------------------------------------
void ADAC_Suspend(void)
{
    VERIFY(x_sema_unlock(_hSemaCodecSetting) == OSR_OK);
    VERIFY(x_sema_unlock(_hSemaAmpSetting) == OSR_OK);

    _fgSpeakerInit = FALSE;
}
#endif

//-----------------------------------------------------------------------------
// ADAC_GetDrvCallBackFuncTbl
//
/**
 *
 *  @retval     AMP_DRV_CB_T driver call back function table.
 */
//-----------------------------------------------------------------------------
AMP_DRV_CB_T* ADAC_GetDrvCallBackFuncTbl (void)
{
    return (_prAmpTbl->prAmpFunc->prAmpDrvCBFunc);
}

//-----------------------------------------------------------------------------
// ADAC_AmpNvmFunction
//
/** Write NVM data to Amp register
 *
 *  @param[in]    eDataType
 *  @param[in]    u4Data
 *
 *  @retval     UINT32
 */
//-----------------------------------------------------------------------------

#if defined(CC_AUD_USE_NVM) && defined(CC_AUD_USE_NVM_AMP)
UINT32 ADAC_AmpNvmFunction(AUD_NVM_TYPE_T eDataType, UINT32 u4Data)
{
    _AdacTableInit();

    if (_prAmpTbl != NULL)
    {
        if ((_prAmpTbl->prAmpFunc->prAmpDrvCBFunc != NULL) && 
            (_prAmpTbl->prAmpFunc->prAmpDrvCBFunc->pfnNvmFunction != NULL))
        {
            return _prAmpTbl->prAmpFunc->prAmpDrvCBFunc->pfnNvmFunction(eDataType, u4Data);
        }
    }

    return 0;
}
#endif
