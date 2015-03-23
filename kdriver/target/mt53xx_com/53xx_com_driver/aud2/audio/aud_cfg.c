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
 * $Date: 2015/03/23 $
 * $RCSfile: aud_cfg.c,v $
 * $Revision: #10 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_cfg.c
 *  Brief of file aud_cfg.c.
 *  Details of file aud_cfg.c (optional).
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
// KERNEL


LINT_EXT_HEADER_BEGIN

//#include "adc_if.h"
#include "x_util.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
#include "x_os.h"
#include "x_hal_5381.h"
#ifdef CC_MT5881
#include "hw_pdwnc.h"
#endif

#include "sif_if.h"
#include "aud_debug.h"
#include "aud_cfg.h"
#include "adac_if.h"
#include "spdif_drvif.h"
#include "aud_if.h"
#include "aud_hw.h"
#include "aud_drv.h"
#include "aud_drvif.h"
#include "psr_drvif.h"
#include "drv_adsp.h"
#include "drvcust_if.h"
#include "dsp_intf.h"
#include "aud_pll.h"
#include "aud_dsp_cfg.h"
#include "dsp_common.h"
#include "x_pinmux.h"
#include "codec_mt5398.h"
#include "x_aud_dec.h"
#include "dsp_shm.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define USE_16BIT_ALIGNMENT

#if defined(CC_MT5881)
#define PADDRV_2MA 0
#define PADDRV_4MA 1
#define PADDRV_6MA 2
#define PADDRV_8MA 3
#endif

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define AUD_GPIO_SWITCH_0       (17)
#define AUD_GPIO_SWITCH_1       (15)
#define SW_NUMBER               (8)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef enum
{
    I2S_IN_LINEIN = 0,  // Line in
    I2S_IN_SPDIF,       // SPDIF
    I2S_IN_HDMI         // HDMI
}   I2S_IN_SEL;

//---------------------------------------------------------------------------
// Macro
//---------------------------------------------------------------------------
#define SAMP_CHG_LOCK()      VERIFY(x_sema_lock(_hSmpChgSema, X_SEMA_OPTION_WAIT) == OSR_OK)
#define SAMP_CHG_UNLOCK()    VERIFY(x_sema_unlock(_hSmpChgSema) == OSR_OK)

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void AUD_AoutPADMux(AUD_CH_T ePAD_SDATA0, AUD_CH_T ePAD_SDATA1, AUD_CH_T ePAD_SDATA2,
                           AUD_CH_T ePAD_SDATA3, AUD_CH_T ePAD_SDATA4, AUD_CH_T ePAD_SDATA5,
                           UINT8 PAD_MCLK0);
extern void vDspSetFifoReadPtr(UINT8 u1DspId, UCHAR ucDecId, UINT32 u4ReadPtr);
extern BOOL fgDspReadPtrSet (UINT8 u1DspId, UCHAR ucDecId, UINT32 u4Address);
extern BOOL fgHDMIAudioClkOk(void);

extern void AUD_DspDvdEnable(void);
extern void vHDMIHandleAudFmtChange(void);
extern void bHDMIClockDetect(void);
extern void bHDMIAudioSigmaDeltaSetting(UINT32, UINT8, UINT8);
extern BOOL AUD_get4kBeMuteDone(void);
#ifdef CC_COMBINE_4K_2K	
extern UINT32 DRVCUST_Get_2K_Model(void);
#endif


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
UINT32 AUD_DspGetPc(UINT8 u1DspId);
UINT32 AUD_DspProbePause(UINT8 u1DspId);
UINT32 AUD_DspProbeGo(UINT8 u1DspId);
UINT32 AUD_DspProbeStep(UINT8 u1DspId, UINT32 u4Count);
UINT32 fgAUD_DspProbeHalt(UINT8 u1DspId);
UINT32 u4AUD_DspProbeRead(UINT8 u1DspId, UINT32 u4Addr);
UINT32 AUD_DspProbeWrite(UINT8 u1DspId, UINT32 u4Addr, UINT32 u4Data);
UINT32 u4AUD_DspProbePBCfg(UINT8 u1DspId, BOOL fgSet,UINT32 u4Value);
UINT32 u4AUD_DspProbePB(UINT8 u1DspId, BOOL fgSet,UINT32 u4BPNum, UINT32 u4Addr);
UINT32 u4AUD_DspProbeStopOnWrite(UINT8 u1DspId, UINT32 u4Addr, UINT32 u4Data, UINT32 u4Mask);
UINT32 u4AUD_DspProbeChkStopOnWrite(UINT8 u1DspId, BOOL *fgEnable, UINT32 *u4Data, UINT32 *u4Mask);
static void AUD_LineinLRDetect(UINT8 u1LineInId, BOOL fgEnable);
static UINT8 AUD_DrvCurConvert(UINT8 u1DrvCur); 
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
UINT32 AUD_GetLineinWP(void);
void AUD_DrvSetFgHDMIParserthread(UINT8 u1DecId, BOOL fgHDMIParserThread);
extern UINT32 AUD_DrvGetCircularAddress(UINT32 u4StartAddress, UINT32 u4EndAddress, UINT32 u4Address);
#endif
extern void DSP_SetAenvSemaCtrl (UINT8 u1DspId, UINT16 u2Ctrl);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static AOUT_CFG_T    _arAudOutCfg[2];
static AIN_CFG_T     _rLineInCfg;
static AIN_CFG_T     _rHDMIInCfg;
static AIN_CFG_T     _rSPDIFInCfg;
#ifdef CC_AUD_I2S_IN
static AIN_CFG_T     _rI2SInCfg;
#endif
static HANDLE_T     _hSmpChgSema;

static UINT8         _au1MckDrvCur[2] = {2, 2};
static UINT8         _au1BckDrvCur[2] = {2, 2};
static UINT8         _au1LRckDrvCur[2] = {2, 2};
static UINT8         _au1AOSDATADrvCur[2] = {2, 2};

static BOOL _fgHwInit = FALSE;
static AUD_DEC_STREAM_FROM_T _aeStrSrcConf[AUD_DEC_MAX] = {AUD_STREAM_FROM_DIGITAL_TUNER, AUD_STREAM_FROM_DIGITAL_TUNER,
                                                           AUD_STREAM_FROM_DIGITAL_TUNER, AUD_STREAM_FROM_DIGITAL_TUNER
                                                          };
static BOOL _fgChangeSource[AUD_DEC_MAX] = {TRUE, TRUE, TRUE, TRUE};
static SAMPLE_FREQ_T _aeSampleRate[AUD_DEC_MAX] = {FS_48K, FS_48K, FS_48K, FS_48K};
static AUD_INPUT_ID_T _eInputIdToDvdGpioConfig;
static BOOL _fgPowerOff = FALSE;
static AUD_BS_SEL_T _arAudBitStrSel[AUD_DSP_NUM][AUD_DEC_MAX];

#ifdef CC_DSP_SUPPORT_ARC
BOOL  fgSPDIFEnable = TRUE;
extern BOOL _fgArcEnableFlag;
#endif
extern BOOL fgHDMIDTS;

#ifdef CC_AUD_DEPOP_SUPPORT
HANDLE_T hHpDepopCtrlThread = NULL_HANDLE;
#endif
//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
AUD_AOUT_PAD_CFG_T _grAOutPadCfg = {AUD_CH_FRONT_LEFT, AUD_CH_REAR_LEFT,
                                    AUD_CH_CENTER, AUD_CH_BYPASS_LEFT,
                                    AUD_CH_FRONT_LEFT, AUD_CH_AUX_FRONT_LEFT
                                   };

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _AudSetBI2SPinmux
 *  Set BI2S pinmux
 *
 *  @param  void
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _AudSetBI2SPinmux(void)
{
    BOOL fgBI2SEnable = FALSE;

    // Check if BI2S output from GPIO 10~13
    fgBI2SEnable = (BOOL)DRVCUST_OptGet(eAudioBI2SEnable);

    if (fgBI2SEnable)
    {

#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        BSP_PinSet(PIN_AOMCLK, PINMUX_FUNCTION2); //function 1-->A I2S function 2-->B I2S
        BSP_PinSet(PIN_AOLRCK, PINMUX_FUNCTION2);
        BSP_PinSet(PIN_AOBCK, PINMUX_FUNCTION2);        
        BSP_PinSet(PIN_AOSDATA0, PINMUX_FUNCTION2);
#elif defined(CC_MT5398)        
#if 1 //Use A I2S as B I2S
        BSP_PinSet(PIN_AOMCLK, PINMUX_FUNCTION2);//function 1-->A I2S function 2-->B I2S
        BSP_PinSet(PIN_AOLRCK, PINMUX_FUNCTION2);
        BSP_PinSet(PIN_AOBCK, PINMUX_FUNCTION2);        
        BSP_PinSet(PIN_AOSDATA0, PINMUX_FUNCTION2);
#else //Use GPIO2~5 as B
        BSP_PinSet(PIN_GPIO2, PINMUX_FUNCTION3); //MCLK
        BSP_PinSet(PIN_GPIO3, PINMUX_FUNCTION3); //LRCK
        BSP_PinSet(PIN_GPIO4, PINMUX_FUNCTION3); //BCK   
        BSP_PinSet(PIN_GPIO5, PINMUX_FUNCTION3); //DATA     
#endif        
#elif defined(CC_MT5880)        
        BSP_PinSet(PIN_GPIO0, PINMUX_FUNCTION2);
        BSP_PinSet(PIN_GPIO1, PINMUX_FUNCTION2);
        BSP_PinSet(PIN_GPIO2, PINMUX_FUNCTION2);        
        BSP_PinSet(PIN_GPIO3, PINMUX_FUNCTION2); 
#endif
    }
}

//
static void _AudPinmuxInit(void)
{
    CRIT_STATE_T rCritState;
    //AUD_DIGITAL_DATA_T* prDigitalDataCfg;

    // Enter critical section
    rCritState = x_crit_start();

    _AudSetBI2SPinmux();

    //Initially, set SPDIF0/SPDIF1 as SPDIF function, and unmute spdif
    AUD_CLR_BIT(REG_SPOCFG0, SPOUT_MUTE);

#ifdef CC_MT5881
    BSP_PinSet(PIN_ASPDIFIN, PINMUX_FUNCTION2);
#endif    

    //2013/07/16: remove audio pinmux initialization from audio driver.
    //we just need modify the audio part of pinmux init-table in 
    //mtk_pin_setup.h: _gu4SysPinmuxInit[][2]

#if 0 
    // SPDIF select
#ifdef CC_AUD_DDI
    BSP_PinSet(AUD_SPDIF0, PINMUX_FUNCTION0); 
#ifdef AUD_SPDIF1
    BSP_PinSet(AUD_SPDIF1, PINMUX_FUNCTION0);
#endif
#else
    BSP_PinSet(AUD_SPDIF0, PINMUX_FUNCTION1); 
#ifdef AUD_SPDIF1
    BSP_PinSet(AUD_SPDIF1, PINMUX_FUNCTION1);
#endif
#endif
#ifdef CC_MT5880
    //For 5880 ALIN --> SPDIF
    BSP_PinSet(PIN_ALIN, PINMUX_FUNCTION2); 
#endif

#ifdef PIN_ASPDIFIN
    BSP_PinSet(PIN_ASPDIFIN, PINMUX_FUNCTION2);
#endif


#ifdef CC_MT5881
    //Remy amp pd pin -> GPIO function
    BSP_PinSet(PIN_ADIN1, PINMUX_FUNCTION1);
#endif

    // Digital I2S output MCLK/LRCK/BCLK/DATA0/DATA1/DATA2/DATA3/DATA4/DATA5
    UNUSED(DRVCUST_OptQuery(eAudioDigitalDataConfig, (UINT32*)(void*)&prDigitalDataCfg));

    if (prDigitalDataCfg->fgAOSDATA0)
    {
        BSP_PinSet(PIN_AOSDATA0, PINMUX_FUNCTION1);
    }

#ifdef PIN_AOSDATA1 
    if (prDigitalDataCfg->fgAOSDATA1)
    {
        BSP_PinSet(PIN_AOSDATA1, PINMUX_FUNCTION1);
    }
#endif

#ifdef PIN_AOSDATA2 
    if (prDigitalDataCfg->fgAOSDATA2)
    {
        BSP_PinSet(PIN_AOSDATA2, PINMUX_FUNCTION1);
    }
#endif

#ifdef PIN_AOSDATA3
    if (prDigitalDataCfg->fgAOSDATA3)
    {
        BSP_PinSet(PIN_AOSDATA3, PINMUX_FUNCTION1);
    }
#endif 

#ifdef PIN_AOSDATA4 
    if (prDigitalDataCfg->fgAOSDATA4)
    {
        BSP_PinSet(PIN_AOSDATA4, PINMUX_FUNCTION1);
    }
#endif

    if (prDigitalDataCfg->fgCLK)
    {
        BSP_PinSet(PIN_AOMCLK, PINMUX_FUNCTION1);
        BSP_PinSet(PIN_AOLRCK, PINMUX_FUNCTION1);
        BSP_PinSet(PIN_AOBCK, PINMUX_FUNCTION1);
    }
#ifdef CC_AUD_I2S_IN
    //set I2S in pinmux here
#endif    
#endif
    // End of critical section
    x_crit_end(rCritState);
}

static void _AudInOutCfgInit(void)
{
    UINT8 u1DecId;
    AOUT_CFG_T* prAudOutFmt;
    AOUT_CFG_T* prAudOutFmt2;
    AOUT_CFG_T* prAudInFmt;

    // Query ADAC config
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32*)(void*) &prAudOutFmt));
    _arAudOutCfg[AUD_DEC_MAIN].eFormat = prAudOutFmt->eFormat;
    _arAudOutCfg[AUD_DEC_MAIN].eSampleFreq = prAudOutFmt->eSampleFreq;
    _arAudOutCfg[AUD_DEC_MAIN].eBits = prAudOutFmt->eBits;
    _arAudOutCfg[AUD_DEC_MAIN].eCycle = prAudOutFmt->eCycle;
    _arAudOutCfg[AUD_DEC_MAIN].fgDataInvert = prAudOutFmt->fgDataInvert;
    _arAudOutCfg[AUD_DEC_MAIN].fgLRInvert = prAudOutFmt->fgLRInvert;

    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable2, (UINT32*)(void*) &prAudOutFmt2));

    _arAudOutCfg[AUD_DEC_AUX].eFormat = prAudOutFmt2->eFormat;
    _arAudOutCfg[AUD_DEC_AUX].eSampleFreq = prAudOutFmt2->eSampleFreq;
    _arAudOutCfg[AUD_DEC_AUX].eBits = prAudOutFmt2->eBits;
    _arAudOutCfg[AUD_DEC_AUX].eCycle = prAudOutFmt2->eCycle;
    _arAudOutCfg[AUD_DEC_AUX].fgDataInvert = prAudOutFmt2->fgDataInvert;
    _arAudOutCfg[AUD_DEC_AUX].fgLRInvert = prAudOutFmt2->fgLRInvert;

    UNUSED(DRVCUST_OptQuery(eAudioInFmtTable, (UINT32*)(void*) &prAudInFmt));

    // Audio output format init
    for (u1DecId = AUD_DEC_MAIN; u1DecId < 2 ; u1DecId++)
    {
        _au1MckDrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
        _au1BckDrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
        _au1LRckDrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
        _au1AOSDATADrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
    }

    // Line-in format init
    _rLineInCfg.eStrSrc       = AUD_STREAM_FROM_LINE_IN;
    _rLineInCfg.eFormat       = prAudInFmt->eFormat;
    _rLineInCfg.eBits         = prAudInFmt->eBits;
    _rLineInCfg.eCycle        = prAudInFmt->eCycle;
    _rLineInCfg.eSampleFreq   = prAudInFmt->eSampleFreq;
    _rLineInCfg.fgDataInvert  = prAudInFmt->fgDataInvert;
    _rLineInCfg.fgLRInvert    = prAudInFmt->fgLRInvert;

    // Spdif-in format init
    _rSPDIFInCfg.eStrSrc      = AUD_STREAM_FROM_SPDIF;
    _rSPDIFInCfg.eFormat      = FORMAT_RJ;
    _rSPDIFInCfg.eBits        = DAC_24_BIT;
    _rSPDIFInCfg.eCycle       = LRCK_CYC_32;
    _rSPDIFInCfg.eSampleFreq  = MCLK_256FS;
    _rSPDIFInCfg.fgDataInvert = FALSE;
    _rSPDIFInCfg.fgLRInvert   = FALSE;

    // HDMI-in format init
    _rHDMIInCfg.eStrSrc       = AUD_STREAM_FROM_HDMI;
    _rHDMIInCfg.eFormat       = FORMAT_I2S;
    _rHDMIInCfg.eBits         = DAC_24_BIT;
    _rHDMIInCfg.eCycle        = LRCK_CYC_32;
    _rHDMIInCfg.eSampleFreq   = MCLK_256FS;
    _rHDMIInCfg.fgDataInvert  = FALSE;
    _rHDMIInCfg.fgLRInvert    = FALSE;
    
#ifdef CC_AUD_I2S_IN
    // I2S-in format init
    _rI2SInCfg.eStrSrc       = AUD_STREAM_FROM_I2S;
    _rI2SInCfg.eFormat       = FORMAT_I2S;
    _rI2SInCfg.eBits         = DAC_24_BIT;
    _rI2SInCfg.eCycle        = LRCK_CYC_32;
    _rI2SInCfg.eSampleFreq   = MCLK_256FS;
    _rI2SInCfg.fgDataInvert  = FALSE;
    _rI2SInCfg.fgLRInvert    = FALSE;   
#endif    
}

// This function is used to select clock source
// a. apll  b. hdmi  c. spdif (external pin)
static void _AudClkSrcSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eFmt)
{
    static AUD_DEC_STREAM_FROM_T ePreviouStream[AUD_DEC_MAX] = {AUD_STREAM_FROM_OTHERS};

    AUD_DEC_ID_VALIDATE(u1DecId);

    if ((eStreamFrom == AUD_STREAM_FROM_HDMI) || (ePreviouStream[u1DecId] == AUD_STREAM_FROM_HDMI) ||
        (eStreamFrom == AUD_STREAM_FROM_SPDIF) || (ePreviouStream[u1DecId] == AUD_STREAM_FROM_SPDIF)
#ifdef CC_AUD_I2S_IN
        ||(eStreamFrom == AUD_STREAM_FROM_I2S) || (ePreviouStream[u1DecId] == AUD_STREAM_FROM_I2S)
#endif
        )
    {
        bHDMIClockDetect();
    }

    // Main decoder set to apll0
    AUD_CLR_BIT(REG_ACK_CFG, CLK_SEL_APLL1);
    // Aux decoder set to apll0
    AUD_CLR_BIT(REG_ACK_CFG, DEC2_CLK_SEL_APLL1);

    // Line in from ACK_CFG
    AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_DEC0);
    if (eStreamFrom == AUD_STREAM_FROM_LINE_IN_2)
    {
        AUD_WRITE32(REG_DEC2_LIN_CFG0, (AUD_READ32(REG_DEC2_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_DEC0);
    }
    if (eStreamFrom == AUD_STREAM_FROM_LINE_IN_3)
    {
        AUD_WRITE32(REG_DEC3_LIN_CFG0, (AUD_READ32(REG_DEC3_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_DEC0);
    }
	
    AudPllSrcSel(u1DecId, eStreamFrom);

    if (_AUD_IsIECRaw() == TRUE)
    {
        // Select IEC refer to apll1
        AUD_CLR_BIT(REG_ACK2_CFG, IEC_AUD3_CLK_SEL);
        AUD_WRITE32(REG_IEC_DIV, IEC_AUDCK_SEL | IEC_CLK_INDEPENDENT_SEL | IEC_LRCK_CYCLE  | IEC_UPDATE_SEL_2T); // Sync from [DTV00094645] (IEC_UPDATE_SEL fine tune)
    }
    else
    {
        // Select IEC refer to apll0
        AUD_WRITE32(REG_IEC_DIV, (AUD_READ32(REG_IEC_DIV) & (~IEC_AUDCK_SEL)) & (~IEC_CLK_INDEPENDENT_SEL));
    }

    switch (eStreamFrom)
    {
        case AUD_STREAM_FROM_SPDIF:
        {
            // Disable AUD3
            AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_CLK_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_LRCK_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, LIN_CLK_AUD3_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, DEC2_LIN_CLK_AUD3_SEL);
            // Slave mode
            AUD_SET_BIT(REG_ACK_CFG, LIN_MS);
            // Line in from HDMI/SPDIF
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_DEC2);
            //  TODO: Need to set tracking
            break;
        }
        case AUD_STREAM_FROM_HDMI:
        {
            // Disable AUD3
            AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_CLK_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_LRCK_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, LIN_CLK_AUD3_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, DEC2_LIN_CLK_AUD3_SEL);
            // Slave mode
            AUD_SET_BIT(REG_ACK_CFG, LIN_MS);
            //  TODO: Need to set tracking
            break;
        }
        case AUD_STREAM_FROM_MULTI_MEDIA:
        {
            if (AUD_GetMultimediaMode(u1DecId) == AUD_MM_MODE_GAME)
            {
                AUD_CLR_BIT(REG_ACK_CFG, LIN_MS);
            }
            break;
        }
        case AUD_STREAM_FROM_LINE_IN:
        case AUD_STREAM_FROM_LINE_IN_2:
        case AUD_STREAM_FROM_LINE_IN_3:
        {
            // Select decoder refer to AUD3
#ifdef CC_FPGA
            AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_CLK_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_LRCK_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, LIN_CLK_AUD3_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, DEC2_LIN_CLK_AUD3_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, DEC3_LIN_CLK_AUD3_SEL);
#else
            AUD_SET_BIT(REG_ACK2_CFG, SDADC_AUD3_CLK_SEL);
            AUD_SET_BIT(REG_ACK2_CFG, SDADC_AUD3_LRCK_SEL);
            AUD_SET_BIT(REG_ACK2_CFG, LIN_CLK_AUD3_SEL);
            AUD_SET_BIT(REG_ACK2_CFG, DEC2_LIN_CLK_AUD3_SEL);
            AUD_SET_BIT(REG_ACK2_CFG, DEC3_LIN_CLK_AUD3_SEL);
#endif
            // Master mode
            AUD_CLR_BIT(REG_ACK_CFG, LIN_MS);
            break;
        }
#ifdef KARAOKE_MIC_LINE_IN2
        case AUD_STREAM_FROM_LINE_IN_2:
            
            AUD_SET_BIT(REG_ACK2_CFG, DEC2_LIN_CLK_AUD3_SEL);
            // Master mode
            AUD_CLR_BIT(REG_ACK_CFG, LIN2_MS);
            
            break;
#endif

#ifdef CC_AUD_I2S_IN
        case AUD_STREAM_FROM_I2S:
        {
            // Disable AUD3
           AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_CLK_SEL);
           AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_LRCK_SEL);
           AUD_CLR_BIT(REG_ACK2_CFG, LIN_CLK_AUD3_SEL);
           // Slave mode
           AUD_SET_BIT(REG_ACK_CFG, LIN_MS);
           //  TODO: Need to set tracking
           AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_HDMI);
            break;
        }
#endif
        default:
            break;
    }

    if ((eStreamFrom == AUD_STREAM_FROM_HDMI) || (ePreviouStream[u1DecId] == AUD_STREAM_FROM_HDMI) ||
        (eStreamFrom == AUD_STREAM_FROM_SPDIF) || (ePreviouStream[u1DecId] == AUD_STREAM_FROM_SPDIF)
#ifdef CC_AUD_I2S_IN
        ||(eStreamFrom == AUD_STREAM_FROM_I2S) || (ePreviouStream[u1DecId] == AUD_STREAM_FROM_I2S)
#endif        
        )
    {
        bHDMIClockDetect();
    }

    ePreviouStream[u1DecId] = eStreamFrom;
}

static void _AUD_SetCustSpecGpioEnable(BOOL fgEnable)
{
    UINT32 u4CustSpecGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4CustSpecGpio = DRVCUST_OptGet(eAudioCustSpecGpioCtrl);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioSpecGpioPolarity);

    if (u4CustSpecGpio != 0xffffffff)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
        {
            i4RegGpioSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegGpioSwitch = fgEnable ? 0 : 1;
        }
        LOG(8, "Aud Cust Spec Gpio Switch : %d\n", i4RegGpioSwitch);
        AUD_GPIOOutput(u4CustSpecGpio, i4RegGpioSwitch);
    }
    else
    {
        LOG(8, "Not support cust specGPIO\n");
    }
}

static void _AUD_SetCustSpec2GpioEnable(BOOL fgEnable)
{
    UINT32 u4CustSpecGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4CustSpecGpio = DRVCUST_OptGet(eAudioCustSpec2GpioCtrl);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioSpec2GpioPolarity);

    if (u4CustSpecGpio != 0xffffffff)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
        {
            i4RegGpioSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegGpioSwitch = fgEnable ? 0 : 1;
        }
        LOG(8, "Aud Cust Spec Gpio Switch : %d\n", i4RegGpioSwitch);
        AUD_GPIOOutput(u4CustSpecGpio, i4RegGpioSwitch);
    }
    else
    {
        LOG(8, "Not support cust specGPIO\n");
    }
}


static void _AUD_SetCustHpGpioEnable(BOOL fgEnable)
{
    UINT32 u4CustHpGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4CustHpGpio = DRVCUST_OptGet(eAudioCustHpGpioCtrl);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioHpGpioPolarity);

    if (u4CustHpGpio != 0xffffffff)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
        {
            i4RegGpioSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegGpioSwitch = fgEnable ? 0 : 1;
        }
        LOG(8, "Aud Cust Spec Gpio Switch : %d\n", i4RegGpioSwitch);
        AUD_GPIOOutput(u4CustHpGpio, i4RegGpioSwitch);
    }
    else
    {
        LOG(8, "Not support cust HP GPIO\n");
    }
}

//-----------------------------------------------------------------------------
/** _AUD_ResetAmp
 *  Reset Amp
 *
 *  @param  fgEnable   TRUE (shut down), FALSE (not shut down)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _AUD_ResetAmp(BOOL fgEnable)
{
    UINT32 u4AmpSdGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4AmpSdGpio = DRVCUST_OptGet(eAudioAmpResetGpioNum);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioAmpResetGpioPolarity);

    if (u4AmpSdGpio != 0xffffffffU)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
        {
            i4RegGpioSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegGpioSwitch = fgEnable ? 0 : 1;
        }
        LOG(8, "Aud Amp Reset Gpio Switch : %d\n", i4RegGpioSwitch);
        AUD_GPIOOutput(u4AmpSdGpio, i4RegGpioSwitch);
    }
}


//-----------------------------------------------------------------------------
/** _AUD_ResetDigiAmp
 *  Reset Amp
 *
 *  @param  fgEnable   TRUE (Normal state), FALSE (shut down)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _AUD_ResetDigiAmp(BOOL fgEnable)
{
    UINT32 u4AmpSdGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;
    
    UNUSED(_AUD_ResetDigiAmp);
    u4AmpSdGpio = DRVCUST_OptGet(eAudioDigiAMPResetGpio);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioDigiAMPResetGpioPolarity);

    if (u4AmpSdGpio != 0xffffffffU)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
        {
            i4RegGpioSwitch = fgEnable ? 1 : 0;
        }
        else
        {
            i4RegGpioSwitch = fgEnable ? 0 : 1;
        }
        LOG(8, "Aud Amp Reset Gpio Switch : %d\n", i4RegGpioSwitch);
        AUD_GPIOOutput(u4AmpSdGpio, i4RegGpioSwitch);
    }
}

//-----------------------------------------------------------------------------
/** AUD_PowerOffGpioMute
 *  Reset Amp
 *  @param  fgEnable   TRUE (Mute), FALSE (Unmute)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_PowerOffGpioMute(BOOL fgEnable)
{
    UNUSED(_fgPowerOff);
    _fgPowerOff = fgEnable;
    fgEnable = !fgEnable;
    ADAC_GpioAmpEnable(fgEnable);
    _AUD_ResetDigiAmp(fgEnable);
    _AUD_SetCustHpGpioEnable(fgEnable);
    _AUD_SetCustSpecGpioEnable(fgEnable);
    _AUD_SetCustSpec2GpioEnable(fgEnable);
}

#ifdef CC_SUPPORT_STR
extern BOOL fgAudPmStrResume;
#endif
#ifdef CC_AUD_DEPOP_SUPPORT
static void _CustomGpioCtrlThread(void *pvArgs)
{
    UNUSED(pvArgs);
    UNUSED(_CustomGpioCtrlThread);
#ifdef CC_SUPPORT_STR
    if (fgAudPmStrResume)
    {
        x_thread_delay(7000);
        _AUD_SetCustHpGpioEnable(TRUE);
        _AUD_SetCustSpecGpioEnable(TRUE);
        _AUD_SetCustSpec2GpioEnable(TRUE);
    }
	else
	{
		x_thread_delay(4000);
		if (!_fgPowerOff)
		{
			_AUD_SetCustHpGpioEnable(TRUE);
			_AUD_SetCustSpecGpioEnable(TRUE);
			_AUD_SetCustSpec2GpioEnable(TRUE);
		}
	}
#else
    x_thread_delay(4000);
    if (!_fgPowerOff)
    {
        _AUD_SetCustHpGpioEnable(TRUE);
        _AUD_SetCustSpecGpioEnable(TRUE);
        _AUD_SetCustSpec2GpioEnable(TRUE);
    }
#endif
}
#endif

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _AUD_DMX_SetAudFifo
 *
 *  @param  u1DecId     Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  u4FifoStr
 *  @param  u4FifoEnd
 *
 *  @retval TRUE
 *  @retval FALSE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DMX_SetAudFifo(UINT8 u1DecId, UINT32 u4FifoStr, UINT32 u4FifoEnd)
{

    AUD_DEC_ID_VALIDATE(u1DecId);

    /*
        // Check if FIFO start/end addresses are valid aligned
        if (!IsAligned(u4FifoStr, DMX_FIFO_ALIGNMENT) ||
            !IsAligned(u4FifoEnd, DMX_FIFO_ALIGNMENT))
        {
            return FALSE;
        }
    */

    // In 3 decoder flow, dec1 use DMX buffer, dec0/3 use parser buffer for Getbits.
    // In 1 decoder flow, dec0 use DMX buffer, others use parser buffer
#ifndef CC_MT5391_AUD_3_DECODER
    if (u1DecId == AUD_DEC_MAIN)
#else
    if (u1DecId == AUD_DEC_AUX)
#endif
    {
        // Fill in the registers in Unit of 4 bytes
        AUD_WRITE32(REG_DMX_STR, DSP_INTERNAL_ADDR(u4FifoStr >> 2));
        AUD_WRITE32(REG_DMX_END, DSP_INTERNAL_ADDR(u4FifoEnd >> 2));
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        AUD_WRITE32(REG_USER0_ST, DSP_INTERNAL_ADDR(u4FifoStr >> 2));
        AUD_WRITE32(REG_USER0_END, DSP_INTERNAL_ADDR(u4FifoEnd >> 2));
    }
    else
    {
#ifdef CC_DUAL_AUD_DEC_SUPPORT
        AUD_WRITE32(REG_USER1_ST, DSP_INTERNAL_ADDR(u4FifoStr >> 2));
        AUD_WRITE32(REG_USER1_END, DSP_INTERNAL_ADDR(u4FifoEnd >> 2));
#else    
        PARSER_WRITE32(REG_A0STA, DSP_INTERNAL_ADDR(u4FifoStr >> 8));
        PARSER_WRITE32(REG_A0END, DSP_INTERNAL_ADDR(u4FifoEnd >> 8));
#endif        
    }

    return TRUE;
}

#if 0   // Unused
//-----------------------------------------------------------------------------
/** _AUD_DMX_GetAudFifo
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DMX_GetAudFifo(UINT8 u1DecId, UINT32* pu4FifoStr, UINT32* pu4FifoEnd)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Registers in Unit of 4 bytes
    if ((pu4FifoStr != NULL) && (pu4FifoEnd != NULL))
    {
        *pu4FifoStr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_STR) << 2);
        *pu4FifoEnd = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_END) << 2);
    }
}
#endif

//-----------------------------------------------------------------------------
/** _AUD_DMX_GetAudPtr
 *    Return the physical DRAM address
 *  @param  u1DecId     Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  pu4ReadPtr
 *  @param  pu4WritePtr
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DMX_GetAudPtr(UINT8 u1DecId, UINT32* pu4ReadPtr, UINT32* pu4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // *pu4ReadPtr = AUD_READ32(REG_DMX_GTB_PNT);
    if ((pu4ReadPtr != NULL) && (pu4WritePtr != NULL))
    {
        *pu4ReadPtr = DSP_GetDspReadPtrPhysicalAddr(AUD_DSP0, u1DecId);

        // In 3 decoder flow, dec1 use DMX buffer, dec0/3 use parser buffer for Getbits.
        // In 1 decoder flow, dec0 use DMX buffer, others use parser buffer
#ifndef CC_MT5391_AUD_3_DECODER
        if (u1DecId == AUD_DEC_AUX)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp(AUD_DSP0));
        }
        else
        {
            if (AUD_GetMultimediaMode(u1DecId) == AUD_MM_MODE_GAME)
            {
                *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp(AUD_DSP0));
            }
            else
            {
                *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT));
            }

        }
#else
        if (u1DecId == AUD_DEC_MAIN)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_USER1_WP));
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT));
        }
        else if (u1DecId == AUD_DEC_THIRD)
        {

            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_USER0_WP));
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_USER1_WP));
        }
#endif
    }
}

void _AUD_DMX_GetAudWrtPtr(UINT8 u1DecId, UINT32* pu4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // *pu4ReadPtr = AUD_READ32(REG_DMX_GTB_PNT);
    if ((pu4WritePtr != NULL))
    {
        // In 3 decoder flow, dec1 use DMX buffer, dec0/3 use parser buffer for Getbits.
        // In 1 decoder flow, dec0 use DMX buffer, others use parser buffer
#ifndef CC_MT5391_AUD_3_DECODER
        if (u1DecId == AUD_DEC_AUX)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp(AUD_DSP0));
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT));
        }
#else
#if 0//ndef CC_AUD_SKYPE_SUPPORT
        if (u1DecId != AUD_DEC_AUX)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp(AUD_DSP0));
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT));
        }
#else
        if (u1DecId == AUD_DEC_MAIN)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_USER1_WP));
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT));
        }
        else if (u1DecId == AUD_DEC_THIRD)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_USER0_WP));
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_USER1_WP));
        }
#endif
#endif
    }
}
//-----------------------------------------------------------------------------
/** _AUD_DMX_UpdateWritePtr
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DMX_UpdateWritePtr(UINT8 u1DecId, UINT32 u4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Check if write pointer address is within FIFO range
    if ((DSP_INTERNAL_ADDR(u4WritePtr) >= DSP_INTERNAL_ADDR(u4GetAFIFOEnd(AUD_DSP0, u1DecId))) ||
        (DSP_INTERNAL_ADDR(u4WritePtr) < DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId))))
    {
        Printf("FIFO (%x, %x), Wp = %x\n",DSP_INTERNAL_ADDR(u4GetAFIFOStart(AUD_DSP0, u1DecId)),  DSP_INTERNAL_ADDR(u4GetAFIFOEnd(AUD_DSP0, u1DecId)), \
               DSP_INTERNAL_ADDR(u4WritePtr) );

        return FALSE;
    }

#ifndef CC_MT5391_AUD_3_DECODER
    if (u1DecId == AUD_DEC_MAIN)
    {
        AUD_WRITE32(REG_DMX_WRITE_PNT, DSP_INTERNAL_ADDR(u4WritePtr));
    }
    else
    {
        _AUD_BitStrPtrSel(AUD_DSP0, AUD_DEC_AUX, AUD_STREAM_FROM_MEMORY);  //Important!!
        PSR_RiscSetAudioWp(AUD_DSP0,u1DecId, DSP_INTERNAL_ADDR(u4WritePtr));
    }
#else
    if (u1DecId == AUD_DEC_MAIN)
    {
        AUD_WRITE32(REG_USER1_WP, DSP_INTERNAL_ADDR(u4WritePtr));
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        AUD_WRITE32(REG_DMX_WRITE_PNT, DSP_INTERNAL_ADDR(u4WritePtr));
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        AUD_WRITE32(REG_USER0_WP, DSP_INTERNAL_ADDR(u4WritePtr));
    }
    else
    {
        AUD_WRITE32(REG_USER1_WP, DSP_INTERNAL_ADDR(u4WritePtr));
    }
#endif

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DMX_UpdateWPtr
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID _AUD_DMX_UpdateWPtr(UINT8 u1DecId, UINT32 u4Wp)
{
    VERIFY(_AUD_DMX_UpdateWritePtr(u1DecId, u4Wp));
}



//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** AUD_SetPaserRpSrc -- set parser read pointer source
 *
 *  @param  u1DecId       Decoder Id
 *  @param  u1DspId       Dsp Id (AUD_DSP0 or AUD_DSP1). 
 *  @param  eStreamFrom
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_SetPaserRpSrc(UINT8 u1DspId, UINT8 u1DecId)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (u1DecId == AUD_DEC_MAIN)
    {
        AUD_CLR_BIT(REG_BS_CFG(u1DspId), PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG(u1DspId), DEC3_PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG(u1DspId), DEC4_PAR_SEL);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        AUD_SET_BIT(REG_BS_CFG(u1DspId), PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG(u1DspId), DEC3_PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG(u1DspId), DEC4_PAR_SEL);
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        AUD_CLR_BIT(REG_BS_CFG(u1DspId), PAR_SEL);
        AUD_SET_BIT(REG_BS_CFG(u1DspId), DEC3_PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG(u1DspId), DEC4_PAR_SEL);
    }
    else
    {
        AUD_CLR_BIT(REG_BS_CFG(u1DspId), PAR_SEL);
        AUD_CLR_BIT(REG_BS_CFG(u1DspId), DEC3_PAR_SEL);
        AUD_SET_BIT(REG_BS_CFG(u1DspId), DEC4_PAR_SEL);
    }
}

//-----------------------------------------------------------------------------
/** AUD_BitStrSelPolicy - get bits source policy setting
 *
 *  @param  u1DecId       Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  u1DspId       Dsp Id (AUD_DSP0 or AUD_DSP1). 
 *  @param  eStreamFrom
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL AUD_BitStrSelPolicy(UINT8 u1DspId, UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom)
{ 
    AUD_BS_SEL_T eAudBitStr = AUD_BS_SEL_PSR;
    BOOL fgRet = TRUE;

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    switch (eStreamFrom)
    {
        case AUD_STREAM_FROM_DIGITAL_TUNER:
        case AUD_STREAM_FROM_MULTI_MEDIA:
#ifndef CC_MT5391_AUD_3_DECODER
            if (u1DecId == AUD_DEC_MAIN)
            {
                eAudBitStr =  AUD_BS_SEL_DMX; 
            }
            else
            {
                eAudBitStr =  AUD_BS_SEL_PSR; 
            }
            if (AUD_GetMultimediaMode(u1DecId) == AUD_MM_MODE_GAME)
            {
                eAudBitStr = AUD_BS_SEL_PSR;
            }
#else
            if (u1DecId == AUD_DEC_MAIN)
            {
                eAudBitStr = AUD_BS_SEL_USER1; 
            }
            else if (u1DecId == AUD_DEC_AUX)
            {
                eAudBitStr =  AUD_BS_SEL_DMX;
            }
            else if (u1DecId == AUD_DEC_THIRD)
            {
                eAudBitStr =  AUD_BS_SEL_USER0;
            }
            else
            { 
                eAudBitStr = AUD_BS_SEL_USER1;                
            }
#endif // CC_MT5391_AUD_3_DECODER
            break;

        case AUD_STREAM_FROM_MEMORY:
        case AUD_STREAM_FROM_FEEDER:
            eAudBitStr =  AUD_BS_SEL_PSR; 
            break;
            
#ifdef CC_ENABLE_AOMX
        case AUD_STREAM_FROM_GST:
            if (u1DecId == AUD_DEC_MAIN)
            {
                eAudBitStr =  AUD_BS_SEL_USER1;
            }
            else if (u1DecId == AUD_DEC_AUX)
            {
                eAudBitStr =  AUD_BS_SEL_DMX;
            }
            else if (u1DecId == AUD_DEC_THIRD)
            {
                eAudBitStr =  AUD_BS_SEL_USER0;
            }else
            {
                eAudBitStr =  AUD_BS_SEL_USER1;
            }
            break;
#endif

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
        case AUD_STREAM_FROM_HDMI:
            eAudBitStr =  AUD_BS_SEL_PSR;
            break;
#endif

        case AUD_STREAM_FROM_SPDIF:
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
        case AUD_STREAM_FROM_HDMI:
#endif
        case AUD_STREAM_FROM_LINE_IN:
#ifdef CC_AUD_I2S_IN            
        case AUD_STREAM_FROM_I2S: 
#endif            
#ifdef CC_AUD_SKYPE_SUPPORT
            if (u1DecId == AUD_DEC_MAIN)
            {
                eAudBitStr =  AUD_BS_SEL_LINE_IN1; 
            }
            else if (u1DecId == AUD_DEC_AUX)
            {
                eAudBitStr =  AUD_BS_SEL_LINE_IN2; 
            }
            else if (u1DecId == AUD_DEC_THIRD) // THIRD Decoder, on-line sound/phone-ring
            {
                eAudBitStr =  AUD_BS_SEL_LINE_IN3; 
            }
            else if (u1DecId == AUD_DEC_4TH)
            {
                eAudBitStr =  AUD_BS_SEL_LINE_IN1; 
            }
#else
            eAudBitStr =  AUD_BS_SEL_LINE_IN1; 
#ifdef DEC3_USE_LINEIN2
            if (u1DecId == AUD_DEC_THIRD) // THIRD Decoder, on-line sound/phone-ring
            {
                eAudBitStr =  AUD_BS_SEL_LINE_IN2; 
            }
#endif
#endif 
            break;

        case AUD_STREAM_FROM_LINE_IN_2:
            eAudBitStr =  AUD_BS_SEL_LINE_IN2; 
            break;        

        case AUD_STREAM_FROM_LINE_IN_3:
            eAudBitStr =  AUD_BS_SEL_LINE_IN3; 
            break;
        case AUD_STREAM_FROM_OTHERS: 
        default:
            eAudBitStr = AUD_BS_SEL_NUM;
            fgRet = FALSE;
    }

    _arAudBitStrSel[u1DspId][u1DecId] = eAudBitStr;
    
    return fgRet;
}

//-----------------------------------------------------------------------------
/** AUD_GetBitStrSel - get getbits source setting
 *
 *  @param  u1DecId       Decoder Id (AUD_DEC_MAIN, AUD_DEC_AUX, AUD_DEC_THIRD).
 *  @param  u1DspId       Dsp Id (AUD_DSP0 or AUD_DSP1). 
 *
 *  @retval AUD_BS_SEL_T
 */
//-----------------------------------------------------------------------------
AUD_BS_SEL_T AUD_GetBitStrSel(UINT8 u1DspId, UINT8 u1DecId)
{
    return    _arAudBitStrSel[u1DspId][u1DecId]; 
}

//-----------------------------------------------------------------------------
/** AUD_BitStrPtrSel
 *
 *  @param  u1DecId       Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  u1DspId       Dsp Id (AUD_DSP0 or AUD_DSP1). 
 *  @param  eStreamFrom
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_BitStrPtrSel(UINT8 u1DspId, UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    UINT32 u4Format = 0; 
    UINT32 u4PtrSelBankMask[AUD_DEC_MAX] = {PTR_SEL_BANK0_MASK, 
        PTR_SEL_BANK1_MASK, PTR_SEL_BANK2_MASK, PTR_SEL_BANK3_MASK};
    
    UINT32 u4PtrSelBanPos[AUD_DEC_MAX] = {PTR_SEL_BANK0_POS, 
        PTR_SEL_BANK1_POS, PTR_SEL_BANK2_POS, PTR_SEL_BANK3_POS};
    
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (AUD_BitStrSelPolicy(u1DspId, u1DecId, eStreamFrom))
    {
        u4Format = AUD_READ32(REG_BS_CFG(u1DspId)) & (~u4PtrSelBankMask[u1DecId]);
        u4Format |= (_arAudBitStrSel[u1DspId][u1DecId] << u4PtrSelBanPos[u1DecId]);
        AUD_WRITE32(REG_BS_CFG(u1DspId), u4Format);
    }
}

void AUD_BitStrSetFIFO(UINT8 u1DspId, AUD_BS_SEL_T eAudBitStr, UINT32 u4FifoStart,  UINT32 u4FifoEnd)
{    
    AUD_DSP_ID_VALIDATE(u1DspId);

    LOG(3, "%s: eAudBitStr(%d), FifoStart(0x%x), FifoEnd(0x%x)\n ", __FUNCTION__, eAudBitStr, u4FifoStart, u4FifoEnd);
        
    switch (eAudBitStr)
    {
    case AUD_BS_SEL_PSR:
        VERIFY(PSR_SoftInit(u1DspId));
        VERIFY(PSR_SoftSetAudioFIFO(AUD_DEC_MAIN, u1DspId, u4FifoStart, u4FifoEnd));
        break;
    case AUD_BS_SEL_LINE_IN1:
        vSetSPDIFInFIFO(AUD_LINEIN_MAIN, u4FifoStart, u4FifoEnd);
        break;
    case AUD_BS_SEL_LINE_IN2:
        vSetSPDIFInFIFO(AUD_LINEIN_AUX, u4FifoStart, u4FifoEnd);
        break;
    case AUD_BS_SEL_LINE_IN3:
        vSetSPDIFInFIFO(AUD_LINEIN_THIRD, u4FifoStart, u4FifoEnd);
        break;
    case AUD_BS_SEL_DMX:
        AUD_WRITE32(REG_DMX_STR, DSP_INTERNAL_ADDR(u4FifoStart >> 2));
        AUD_WRITE32(REG_DMX_END, DSP_INTERNAL_ADDR(u4FifoEnd >> 2));
        break;
    case AUD_BS_SEL_USER0:
        AUD_WRITE32(REG_USER0_ST, DSP_INTERNAL_ADDR(u4FifoStart >> 2));
        AUD_WRITE32(REG_USER0_END, DSP_INTERNAL_ADDR(u4FifoEnd >> 2));
        break;
    case AUD_BS_SEL_USER1:
        AUD_WRITE32(REG_USER1_ST, DSP_INTERNAL_ADDR(u4FifoStart >> 2));
        AUD_WRITE32(REG_USER1_END, DSP_INTERNAL_ADDR(u4FifoEnd >> 2));
        break;
    default:
        break;
    }
}

//-----------------------------------------------------------------------------
/** AUD_HwInit
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_HwInit(void)
{
    UINT8 u1DecId;
    AUD_AOUT_PAD_CFG_T* prAOutPadCfg;
    UINT32 u4tmp32 = 0;

    if (!_fgHwInit)
    {
        VERIFY(x_sema_create(&_hSmpChgSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        _fgHwInit = TRUE;
    }
#ifdef CC_AUD_DEPOP_SUPPORT
    _AUD_SetCustSpecGpioEnable(FALSE);
    _AUD_SetCustSpec2GpioEnable(FALSE);
    _AUD_SetCustHpGpioEnable(FALSE);
#endif
    //ADAC_Mute(TRUE);
    //20100504 [DTV00080261]
#if 1
    //ADAC_GpioCodecMute(TRUE);
#else
    ADAC_GpioCodecMute(TRUE);
#endif

    //AudAoutGainHwInit();

    // Load audio in/out default setting
    // Must be called before _AudPllInit()
    _AudInOutCfgInit();

    // Setup apll, default set sampling rate = 48 KHz
    if (!BSP_IsFPGA())
    {
        AudPllInit();
    }
    //parser HW init
#ifndef CC_AUD_SUPPORT_DUAL_DSP
    PSR_SoftInit();
#else
    PSR_SoftInit(AUD_DSP0);
    PSR_SoftInit(AUD_DSP1);
#endif

    // module control for 2 DSP/ARM11 systems
    #ifdef CC_AUD_ARM_RENDER
    u4tmp32 =  AUD_READ32(REG_MODULE_CTRL)&(~AOUT1_SEL_MASK)&(~AOUT2_SEL_MASK)&(~IEC_SEL_MASK)&(~PUTBS_SEL_MASK)&(~ASRC1_SEL_MASK)&(~ASRC2_SEL_MASK)&(~ASRC3_SEL_MASK);
    u4tmp32 = u4tmp32 | (AOUT1_SEL_ARM11) | (AOUT2_SEL_ARM11) | (IEC_SEL_ARM11) | (PUTBS_SEL_DSPA) | (ASRC1_SEL_ARM11) | (ASRC2_SEL_ARM11) | (ASRC3_SEL_ARM11);
    AUD_WRITE32(REG_MODULE_CTRL, u4tmp32);
    DSP_SetAenvSemaCtrl(AUD_DSP0, 0x0);
    #else
    u4tmp32 =  AUD_READ32(REG_MODULE_CTRL)&(~AOUT1_SEL_MASK)&(~AOUT2_SEL_MASK)&(~IEC_SEL_MASK)&(~PUTBS_SEL_MASK)&(~ASRC1_SEL_MASK)&(~ASRC2_SEL_MASK)&(~ASRC3_SEL_MASK);
    u4tmp32 = u4tmp32 | (AOUT1_SEL_DSPA) | (AOUT2_SEL_DSPA) | (IEC_SEL_DSPA) | (PUTBS_SEL_DSPA) | (ASRC1_SEL_DSPA) | (ASRC2_SEL_DSPA) | (ASRC3_SEL_DSPA);
    AUD_WRITE32(REG_MODULE_CTRL, u4tmp32);
    DSP_SetAenvSemaCtrl(AUD_DSP0, 0xF);
    #endif
    DSP_SetAenvSemaCtrl(AUD_DSP1, 0x0);

    // Sync from [DTV00094645] (IEC_UPDATE_SEL fine tune)
    AUD_CLR_BIT(REG_IEC_DIV, (1 << 6));
    AUD_WRITE32(REG_IEC_DIV, (AUD_READ32(REG_IEC_DIV) | IEC_UPDATE_SEL_2T));

    // Select audio output pinmux
    _AudPinmuxInit();

    UNUSED(DRVCUST_OptQuery(eAudioAoutPadConfig, (UINT32*)(void*)&prAOutPadCfg));
    if (prAOutPadCfg != NULL)
    {
        x_memcpy((VOID*)VIRTUAL((UINT32)&_grAOutPadCfg), prAOutPadCfg, sizeof(AUD_AOUT_PAD_CFG_T));
    }

    AUD_AoutPADMux(_grAOutPadCfg.ePadSdata0, _grAOutPadCfg.ePadSdata1,
                   _grAOutPadCfg.ePadSdata2, _grAOutPadCfg.ePadSdata3,
                   _grAOutPadCfg.ePadSdata4, _grAOutPadCfg.ePadSdata5,
                   MCLK0);

    // Line in config
    AUD_WRITE32(REG_LIN_CFG0, (DATA_PCM_24_BITS | INT_ADC_INTLIN | RAW_LONG_DETECT));

    VERIFY(BIM_DisableIrq(VECTOR_SPDIF));

    // Setup audio output format
    for (u1DecId = AUD_DEC_MAIN; u1DecId < AUD_AOUT_NO; u1DecId++)
    {
        // Audio output format config
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);

        // Setup audio output(MCLK, BCLK , LRCK, AOSDATA) driving current
        AUD_AoutDrvCur(u1DecId, _au1MckDrvCur[u1DecId], _au1BckDrvCur[u1DecId] , _au1LRckDrvCur[u1DecId] , _au1AOSDATADrvCur[u1DecId]);
    }
#if defined(CC_MT5890)
    // Set AUD3 BCK_DIV=2
    AUD_WRITE32(REG_ACK2_CFG,
                ((AUD_READ32(REG_ACK2_CFG) & ~(AUD3_ACK_BCK_DIV_MASK)) | AUD3_ACK_BCK_DIV_2));
#else 
    // Set AUD3 BCK_DIV=4
    AUD_WRITE32(REG_ACK2_CFG,
                ((AUD_READ32(REG_ACK2_CFG) & ~(AUD3_ACK_BCK_DIV_MASK)) | AUD3_ACK_BCK_DIV_4));
#endif
    // Turn off audio output 128 bit mode
    AUD_CLR_BIT(REG_AOUT_DRAM_CFG, DRAM_ACCESS_128_BITS);

    // Open MCLK ,
    AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
    AUD_OutPadEnable(AUD_DEC_AUX, TRUE);

    // Audio DAC init
    ADAC_Init();

    //SPDIF enable
    //UNUSED(AUD_DspIECConfig(AUD_DEC_MAIN,AUD_IEC_CFG_PCM,1));
#ifdef CC_AUD_DEPOP_SUPPORT
    // Note that set CI or Demod GPIO in timer will cause EU model power-on assert, hence we
    // change coding by create a thread, 20110824
    x_thread_create(&hHpDepopCtrlThread, "CustHpGpioCtrlThread",
                    2048,
                    100,
                    _CustomGpioCtrlThread, 0, NULL);
    _AUD_ResetDigiAmp(TRUE);
#endif

    _AUD_ResetAmp(FALSE);

    // MT5365_Check
    //AUD_WRITE32(REG_DSPROM_CFG, DSP_ROM_CFG);
    //AUD_WRITE32(REG_AINROM_CFG, AIN_ROM_CFG);

    ///ADAC_GpioCodecMute(FALSE);  // For PWM first power on pop noise, 5388 let other module do GPIO unmute. MW: setoutport, DEMO : setSampFreq.

#if defined(CC_AUD_SELF_UNMUTE)

    // workaround for oryx linux demo program, DTV00591945
    // when HP is insetrted, HP detection flow tries to mute AD82581(write register). During this
    // time, AMP_PD can't be high
    ADAC_GpioCodecMute(FALSE);
#endif

    // Laihui: Get bit hold issue
    AUD_SET_BIT(REG_BS_CFG(AUD_DSP0), CLR_BS);
    AUD_SET_BIT(REG_BS_CFG(AUD_DSP1), CLR_BS);
    // Do not let 2nd decoder getbits default use parser.  
	AUD_WRITE32(REG_BS_CFG(AUD_DSP0), (AUD_READ32(REG_BS_CFG(AUD_DSP0)) & (~PTR_SEL_BANK1_MASK))|PTR_SEL_BANK1_DMX);
    ADAC_ADCDown(TRUE); //sunman power down Line in AD    
}

#ifdef CC_FAST_INIT
void AUD_Spdif_Power_Down(void)
{
    /// SPDIF Disable
    _AUD_SetCustSpecGpioEnable(FALSE);
    _AUD_SetCustSpec2GpioEnable(FALSE);
    _AUD_SetCustHpGpioEnable(FALSE);
}
#endif

void AUD_ClkCfgDualDecMode(BOOL fgEnable)
{
//    _fgDualDecMode = fgEnable;
	UNUSED(fgEnable);
}

void AUD_ClkSetToInternal(UINT8 u1DecId, AUD_FMT_T eFmt)
{
    _AudClkSrcSel(u1DecId, AUD_STREAM_FROM_LINE_IN, eFmt);
}

void AUD_StrInputEnable(UINT8 u1DspId, UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eFmt,BOOL fgEnable, UINT32 u4ReceivePesCnt)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);
#if 0//def CC_AUD_DDI
    if ((eStreamFrom == AUD_STREAM_FROM_LINE_IN) && (u1DecId == AUD_DEC_MAIN)&& fgEnable)
    {
        vWriteShmUINT8(AUD_DSP0, B_DELAY_MODE_BYPASS, 0x1); 
    }
    if((eStreamFrom != AUD_STREAM_FROM_LINE_IN) && (u1DecId == AUD_DEC_MAIN)&& fgEnable)
    {
        vWriteShmUINT8(AUD_DSP0, B_DELAY_MODE_BYPASS, 0x0); 
    }
    if ((eStreamFrom == AUD_STREAM_FROM_LINE_IN) && (u1DecId == AUD_DEC_MAIN))
    {
        LOG(0,"!!!!!!!!!!!!!!!LINEIN USE DEC3!!!!!!!!!!!!!!!!!!!!");
        return;
    }
#else
#ifdef CC_AUD_LINEIN_USE_DEC3
    if (_IsTriOnlyDecMode() && (u1DecId == AUD_DEC_MAIN))
    {
        LOG(0, "!!!!!!!!!!!!!!!LINEIN USE DEC3!!!!!!!!!!!!!!!!!!!!");
        return;
    }
#endif
#endif

    // Select clk source form apll ? spdif ? hdmi ?
#ifndef CC_MT5391_AUD_3_DECODER
    if (fgEnable)
#else
#ifndef CC_AUD_LINEIN_USE_DEC3
    if ((u1DecId <  AUD_DEC_THIRD) && (fgEnable))
        // gallen, for dec3 and dec4, we should not change any clock setting
#else
    if ((u1DecId <  AUD_DEC_4TH) && (fgEnable))
#endif
#endif
    {
        _AudClkSrcSel(u1DecId, eStreamFrom, eFmt);
    }

#if defined(CC_DRIVER_BUILD) || defined(CC_AUD_SELF_UNMUTE)

    // workaround since MT5880 driver build
    // driver build needs to do unmute, system build unmute is done by mw
    ADAC_GpioCodecMute(FALSE);
#endif

    if (fgEnable)
    {
        // Select audio getbs source
        _AUD_BitStrPtrSel(u1DspId, u1DecId, eStreamFrom);
        // Set getbs source buffer start/end
        AUD_BitStrSetFIFO(u1DspId, AUD_GetBitStrSel(u1DspId, u1DecId), 
            u4GetAFIFOStart(u1DspId, u1DecId), u4GetAFIFOEnd(u1DspId, u1DecId));
        
#ifdef CC_MT5391_AUD_3_DECODER
#ifndef CC_AUD_LINEIN_USE_DEC3
        if ((u1DecId <  AUD_DEC_THIRD))
            // gallen, for dec3 and dec4, we should not change any clock setting
#else
        if (u1DecId <  AUD_DEC_4TH)
#endif
#endif
        {
            _AudClkSrcSel(u1DecId, eStreamFrom, eFmt);
        }
                
        switch (eStreamFrom)
        {
            case AUD_STREAM_FROM_MEMORY:
            case AUD_STREAM_FROM_FEEDER:
                if ((eStreamFrom == AUD_STREAM_FROM_MEMORY) && 
                    (AUD_GetBitStrSel(u1DspId, u1DecId) == AUD_BS_SEL_PSR))
                {
                    PSR_RiscSetAudioWp(u1DspId, u1DecId, u4GetAFIFOStart(u1DspId, u1DecId));
                }                
                vDspSetFifoReadPtr(u1DspId, u1DecId, u4GetAFIFOStart(u1DspId, u1DecId));
                if (!fgDspReadPtrSet(u1DspId, u1DecId, u4GetAFIFOStart(u1DspId, u1DecId)))
                {
                    LOG(5, "DspReadPtrSet time out\n");
                }
                break;
#ifdef CC_ENABLE_AOMX
            case AUD_STREAM_FROM_GST:
                PSR_SoftInit(u1DspId);
                if(u1DecId == AUD_DEC_THIRD)
                {
                    if (u4ReceivePesCnt == 0)    //write pointer is not updated in SendAudPes
                    {
                        VERIFY(_AUD_DMX_UpdateWritePtr(u1DecId, u4GetAFIFOStart(AUD_DSP0, u1DecId)));
                    }
                }
                vDspSetFifoReadPtr(u1DspId, u1DecId, u4GetAFIFOStart(u1DspId, u1DecId));
                if (!fgDspReadPtrSet(u1DspId, u1DecId, u4GetAFIFOStart(u1DspId, u1DecId)))
                {
                    LOG(5, "DspReadPtrSet time out\n");
                }
#endif
            break;
            case AUD_STREAM_FROM_LINE_IN:
                // Turn off input when config format
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Format config
                AUD_AinSrcSel(u1DspId, u1DecId, eStreamFrom, eFmt);
                // Turn on input
                AUD_AinEnable(u1DecId, eStreamFrom, TRUE);
#ifndef DEC3_USE_LINEIN2
                AUD_LineinLRDetect(AUD_LINEIN_MAIN, TRUE);
#else
                if (u1DecId == AUD_DEC_THIRD)
                {
                    AUD_LineinLRDetect(AUD_LINEIN_AUX, TRUE);
                }
                else
                {
                    AUD_LineinLRDetect(AUD_LINEIN_MAIN, TRUE);
                }
#endif

                AUD_CLR_BIT(REG_INTADC_SDADC0, (1 << 24));
                AUD_SET_BIT(REG_INTADC_SDADC0, RST_FILTER);
                AUD_CLR_BIT(REG_INTADC_SDADC0, RST_FILTER);
                AUD_SET_BIT(REG_INTADC_SDADC0, (1 << 24));
                
                ADAC_ADCDown(FALSE); //sunman power down Line in AD
                break;

            case AUD_STREAM_FROM_LINE_IN_2: 
                // Turn off input when config format
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Format config
                AUD_AinSrcSel(u1DspId, u1DecId, eStreamFrom, eFmt);
                // Turn on input
                AUD_AinEnable(u1DecId, eStreamFrom, TRUE);
                AUD_LineinLRDetect(AUD_LINEIN_AUX, TRUE); 
                ADAC_ADCDown(FALSE);
                //AUD_SET_BIT(REG_INTADC_SDADC0, RST_FILTER);
                //AUD_CLR_BIT(REG_INTADC_SDADC0, RST_FILTER);
                break;

            case AUD_STREAM_FROM_LINE_IN_3: 
                // Turn off input when config format
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Format config
                AUD_AinSrcSel(u1DspId, u1DecId, eStreamFrom, eFmt);
                // Turn on input
                AUD_AinEnable(u1DecId, eStreamFrom, TRUE);
                AUD_LineinLRDetect(AUD_LINEIN_THIRD, TRUE); 
                ADAC_ADCDown(FALSE);
                //AUD_SET_BIT(REG_INTADC_SDADC0, RST_FILTER);
                //AUD_CLR_BIT(REG_INTADC_SDADC0, RST_FILTER); 	
                break;

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
            case AUD_STREAM_FROM_HDMI:
                AUD_SetPaserRpSrc(u1DspId, u1DecId);
                PSR_RiscSetAudioWp(u1DspId, u1DecId, u4GetAFIFOStart(u1DspId, u1DecId)); 
                AUD_BitStrSetFIFO(u1DspId, AUD_BS_SEL_LINE_IN1, u4GetHDMIParserFIFOStart(), u4GetHDMIParserFIFOEnd());
                vDspSetFifoReadPtr(u1DspId, u1DecId, u4GetAFIFOStart(u1DspId, u1DecId));
                if (!fgDspReadPtrSet(u1DspId, u1DecId, u4GetAFIFOStart(u1DspId, u1DecId)))
                {
                    LOG(5, "DspReadPtrSet time out\n");
                }
                // Turn off input when config format
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Format config
                AUD_AinSrcSel(u1DspId, u1DecId, eStreamFrom, eFmt);
                // Turn on input
                AUD_AinEnable(u1DecId, eStreamFrom, TRUE);
                AUD_DrvSetFgHDMIParserthread(u1DecId, fgEnable);
#ifdef CC_AUD_ARM_RENDER
                //APROC Tracking Mode.
                _AUD_AprocAsrcTrackingMode(u1DecId, fgEnable);
#endif
                // Prevent L/R reverse
                AUD_LineinLRDetect(AUD_LINEIN_MAIN, TRUE);
                break;
#endif

            case AUD_STREAM_FROM_SPDIF:
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
            case AUD_STREAM_FROM_HDMI:
#endif
#ifdef CC_AUD_I2S_IN
            case AUD_STREAM_FROM_I2S:
#endif
#ifdef CC_MT5881 //Remy spdif in
                AUD_SPDIFIN_CONF(u1DecId, eStreamFrom);
#endif
                // Turn off input when config format
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Format config
                AUD_AinSrcSel(u1DspId, u1DecId, eStreamFrom, eFmt);
                // Turn on input
                AUD_AinEnable(u1DecId, eStreamFrom, TRUE);
#ifdef CC_AUD_ARM_RENDER
                //APROC Tracking Mode.
                _AUD_AprocAsrcTrackingMode(u1DecId, fgEnable); 
#endif
                // Prevent L/R reverse
                AUD_LineinLRDetect(u1DecId, TRUE);
                break;

            case AUD_STREAM_FROM_DIGITAL_TUNER:
            case AUD_STREAM_FROM_MULTI_MEDIA:
#ifndef CC_AUD_SKYPE_SUPPORT
                PSR_SoftInit(u1DspId);
#else
                if (u1DecId == AUD_DEC_MAIN)
                {
                    PSR_SoftInit(u1DspId);
                }
#endif
                if (u4ReceivePesCnt == 0)    //write pointer is not updated in SendAudPes
                {
                    VERIFY(_AUD_DMX_UpdateWritePtr(u1DecId, u4GetAFIFOStart(u1DspId, u1DecId)));
                }
                break;

            case AUD_STREAM_FROM_ANALOG_TUNER:
                DSP_SetAIFMux(SIF_PATH);
                break;

            case AUD_STREAM_FROM_OTHERS:
                break;

            default:
                break;
        }
    }
    else // Turn off stream input
    {
        switch (eStreamFrom)
        {
            case AUD_STREAM_FROM_MEMORY:
                break;
#ifdef CC_ENABLE_AOMX
            case AUD_STREAM_FROM_GST:
                break;
#endif
            case AUD_STREAM_FROM_LINE_IN:
                
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                ADAC_ADCDown(TRUE); //sunman power down Line in AD
                AUD_LineinLRDetect(0, FALSE);
                break;

            case AUD_STREAM_FROM_LINE_IN_2:               
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                ADAC_ADCDown(TRUE);
                AUD_LineinLRDetect(AUD_LINEIN_AUX, FALSE);
                break;

            case AUD_STREAM_FROM_LINE_IN_3:               
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                ADAC_ADCDown(TRUE);
                AUD_LineinLRDetect(AUD_LINEIN_THIRD, FALSE);
                break;

            case AUD_STREAM_FROM_SPDIF:
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
            case AUD_STREAM_FROM_HDMI:
#endif
#ifdef CC_AUD_I2S_IN
            case AUD_STREAM_FROM_I2S:
#endif                
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Switch clock source to apll since if there is no external clock,
                // dsp may fail to stop (flush done)
                _AudClkSrcSel(u1DecId, AUD_STREAM_FROM_LINE_IN, eFmt);
#ifdef CC_AUD_ARM_RENDER
                //APROC Tracking Mode.
                _AUD_AprocAsrcTrackingMode(u1DecId, fgEnable);   
#endif
                AUD_LineinLRDetect(u1DecId, FALSE);
                break;

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
            case AUD_STREAM_FROM_HDMI:
                AUD_DrvSetFgHDMIParserthread(u1DecId, fgEnable);
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Switch clock source to apll since if there is no external clock,
                // dsp may fail to stop (flush done)
                _AudClkSrcSel(u1DecId, AUD_STREAM_FROM_LINE_IN, eFmt);
#ifdef CC_AUD_ARM_RENDER
                //APROC Tracking Mode.
                _AUD_AprocAsrcTrackingMode(u1DecId, fgEnable);    
#endif
                AUD_LineinLRDetect(AUD_DEC_MAIN, FALSE);
                break;
#endif

            case AUD_STREAM_FROM_DIGITAL_TUNER:
            case AUD_STREAM_FROM_MULTI_MEDIA:
                // middleware will turn off dmx
                break;

            case AUD_STREAM_FROM_ANALOG_TUNER:
                // middleware will turn off tuner
                break;

            case AUD_STREAM_FROM_OTHERS:
                break;

            default:
                break;
        }
    }
}

//-----------------------------------------------------------------------------
/** AUD_SetSampleFreq
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eSmpFreq  Samping frequency
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void  AUD_SetSampleFreq(UINT8 u1DecId, SAMPLE_FREQ_T eSmpFreq)
{
    BOOL isHiResChg = FALSE;
    UINT8 u1Count = 50;
    AUD_DEC_ID_VALIDATE(u1DecId);

    SAMP_CHG_LOCK();
    if ((_aeSampleRate[u1DecId] != eSmpFreq) || _fgChangeSource[u1DecId])
    {
        if( ((_aeSampleRate[u1DecId]== FS_96K) || (eSmpFreq == FS_96K)) && (!_fgChangeSource[u1DecId]))
        {
            DRVCUST_SendAudEvent(E_CUST_AUD_SPEAKER_MUTE_REQUEST, u1DecId); //BE_FIXME
            while( AUD_get4kBeMuteDone() == FALSE )
            {
                if(u1Count-- == 0)
                    break;
                x_thread_delay(10);   
            }
            DRVCUST_SendAudEvent(E_CUST_AUD_OUT_FORMAT_PRE_CHANGE, u1DecId); //BE_FIXME
            isHiResChg = TRUE;
        }
        //LOG(0,"\n\nisHiResChg=%d u1DecId=%d, _aeSampleRate[u1DecId]=%d eSmpFreq =%d _fgChangeSource[u1DecId]=%d\n\n",isHiResChg, u1DecId, _aeSampleRate[u1DecId], eSmpFreq,_fgChangeSource[u1DecId]);
        _fgChangeSource[u1DecId] = FALSE; // DTV00126551 kai wang for reset apll when change source
        //Light: Mute external DAC or amplifier
        //Zhaoyuan: mark the codec mute/unmute. no needed with ASRC
        //ADAC_Mute(TRUE);

        if (((u1DecId == AUD_DEC_MAIN) && (_aeStrSrcConf[AUD_DEC_MAIN] != AUD_STREAM_FROM_HDMI)) ||
            ((u1DecId == AUD_DEC_AUX) && (_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_DIGITAL_TUNER)) ||
#ifdef CC_ENABLE_AOMX
            ((u1DecId == AUD_DEC_AUX) && (_aeStrSrcConf[AUD_DEC_AUX] == AUD_STREAM_FROM_GST)) ||
#endif            
            ((u1DecId == AUD_DEC_THIRD) && _IsTriOnlyDecMode() && (_aeStrSrcConf[AUD_DEC_THIRD] != AUD_STREAM_FROM_HDMI)))
        {
            if (_AudGetStrFormat(u1DecId) == AUD_FMT_AAC) //for bypass DDCO 44.1K
            {
                _aeSampleRate[u1DecId] = eSmpFreq;
                _AUD_DspSetIEC(u1DecId);
            }
            
            if((_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_DIGITAL_TUNER)&&(_AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_AC3)&&(AUD_DRVGetAudioState(AUD_DSP0, AUD_DEC_AUX)==AUD_ON_PLAY))
            {
                _AUD_DspSetAoutReinit(); //for samplingr rate change , do aout reinit to fix SPDIF raw output issue
            }
            
            if (_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_AC3 || _AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_AC3
                || ((_AudGetStrFormat(AUD_DEC_THIRD) == AUD_FMT_AC3) && _IsTriOnlyDecMode())
                || _AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_DTS || _AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_DTS
                || ((_AudGetStrFormat(AUD_DEC_THIRD) == AUD_FMT_DTS) && _IsTriOnlyDecMode())
                || ((_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_AAC || _AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_AAC || ((_AudGetStrFormat(AUD_DEC_THIRD) == AUD_FMT_AAC) && _IsTriOnlyDecMode())) && !_AUD_DspIsDDCOEnable()))
            {
                //PLL setting
                AudPll2Setting(eSmpFreq);
            }
            else
            {
                AudPll2Setting(FS_48K);
            }
        }
        else if (_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_HDMI)
        {
            bHDMIAudioSigmaDeltaSetting(0, 0, 0);
            if ((_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_AC3) || 
                (_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_DTS) ||
                (_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_AAC))
            {
                if (eSmpFreq == FS_32K)
                {
                    AudPll2Setting(eSmpFreq);
                }
            }
        }
#ifdef CC_AUD_I2S_IN
        else if(_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_I2S)
        {
            //no use
        } 
#endif
    }

    _aeSampleRate[u1DecId] = eSmpFreq;
    if(isHiResChg)
    {
        DRVCUST_SendAudEvent(E_CUST_AUD_OUT_FORMAT_CHANGE, u1DecId); //BE_FIXME
        DRVCUST_SendAudEvent(E_CUST_AUD_FORMAT_CHANGE, u1DecId);
        isHiResChg = 0;
    }    
    SAMP_CHG_UNLOCK();
}

//-----------------------------------------------------------------------------
/** AUD_GetSampleFreq
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval Samping frequency
 */
//-----------------------------------------------------------------------------
SAMPLE_FREQ_T AUD_GetSampleFreq(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    return _aeSampleRate[u1DecId];
}

//-----------------------------------------------------------------------------
/** AUD_GetAinCfg
 *
 *  @param  eStreamFrom
 *  @param  prAinCfg
 *
 *  @retval TRUE
 *  @retval FALSE
 */
//-----------------------------------------------------------------------------
BOOL AUD_GetAinCfg(AUD_DEC_STREAM_FROM_T eStreamFrom, AIN_CFG_T* prAinCfg)
{
    if (!_fgHwInit)
    {
        return FALSE;
    }

    if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
    {
        x_memcpy((VOID*)VIRTUAL((UINT32)prAinCfg), (const VOID*)&_rLineInCfg, sizeof(AIN_CFG_T));
    }
    else if (eStreamFrom == AUD_STREAM_FROM_SPDIF)
    {
        x_memcpy((VOID*)VIRTUAL((UINT32)prAinCfg), (const VOID*)&_rSPDIFInCfg, sizeof(AIN_CFG_T));
    }
    else if (eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        x_memcpy((VOID*)VIRTUAL((UINT32)prAinCfg), (const VOID*)&_rHDMIInCfg, sizeof(AIN_CFG_T));
    }
#ifdef CC_AUD_I2S_IN
    else if (eStreamFrom == AUD_STREAM_FROM_I2S)
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)prAinCfg), (const VOID *)&_rI2SInCfg, sizeof(AIN_CFG_T)); 
    }
#endif    
    else
    {
        return FALSE;
    }

    return TRUE;
}

#ifdef CC_MT5881
//-----------------------------------------------------------------------------
/** AUD_AinGetMode_Spdif
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinGetMode_Spdif(BOOL* pfgDeteced, AUD_FMT_T* peMode)
{
    UINT32 u4Reg = 0;
    static AUD_FMT_T peOriMode_spdif = AUD_FMT_AC3;

    if ((pfgDeteced == NULL) || (peMode == NULL))
    {
        return;
    }

    u4Reg = AUD_READ32(REG_SPDFTYP);
    *pfgDeteced = ((u4Reg & TYPE_DETECT) == 0) ? FALSE : TRUE;

    if (*pfgDeteced)
    {
        if ((u4Reg & TYPE_ROUGH) == TYPE_PCM)
        {
            *peMode = AUD_FMT_PCM;
        }
        else if ((u4Reg & TYPE_ROUGH) == TYPE_DTS_16BITS ||
                 (u4Reg & TYPE_ROUGH) == TYPE_DTS_14BITS)
        {
            *peMode = AUD_FMT_DTS;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_DTS1 ||
                 (u4Reg & TYPE_DETAIL) == TYPE_DTS2 ||
                 (u4Reg & TYPE_DETAIL) == TYPE_DTS3)
        {
            *peMode = AUD_FMT_DTS;
        }
        else if (((u4Reg & TYPE_DETAIL) == TYPE_AC3) || ((u4Reg & TYPE_DETAIL) == TYPE_EAC3))
        {
            if (AUD_DspIsFormatValid(AUD_FMT_AC3))
            {
                *peMode = AUD_FMT_AC3;
            }
            else
            {
                *peMode = AUD_FMT_UNKNOWN;
            }
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_PAUSE)
        {
            *peMode = peOriMode_spdif;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG1_L1)
        {
            *peMode = AUD_FMT_MPEG;
        }
        // Note: MP2/MP3 has the same type in IEC61937!
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG1_L2L3)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG2_L1LSF)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG2_L2LSF)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else
        {
            *peMode = AUD_FMT_UNKNOWN;
            LOG(3, "SPDIF format not support\n");
        }
        peOriMode_spdif = *peMode;

    }
}
#endif

#if defined(CC_AUD_I2S_IN) || !defined(CC_AUD_HDMI_PARSER_2_0_SUPPORT)
//-----------------------------------------------------------------------------
/** AUD_AinGetMode
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinGetMode(BOOL* pfgDeteced, AUD_FMT_T* peMode)
{
    UINT32 u4Reg = 0;
    static AUD_FMT_T peOriMode = AUD_FMT_AC3;

    if ((pfgDeteced == NULL) || (peMode == NULL))
    {
        return;
    }

    u4Reg = AUD_READ32(REG_SPDFTYP);
    *pfgDeteced = ((u4Reg & TYPE_DETECT) == 0) ? FALSE : TRUE;

    if (*pfgDeteced)
    {
        fgHDMIDTS = FALSE;
        if ((u4Reg & TYPE_ROUGH) == TYPE_PCM)
        {
            *peMode = AUD_FMT_PCM;
        }
        else if (((u4Reg & TYPE_DETAIL) == TYPE_AC3) || ((u4Reg & TYPE_DETAIL) == TYPE_EAC3))
        {
            if(AUD_DspIsFormatValid(AUD_FMT_AC3))
            {
                *peMode = AUD_FMT_AC3;
            }
            else
            {
                *peMode = AUD_FMT_UNKNOWN;
            }
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_PAUSE)
        {
            *peMode = peOriMode;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG1_L1)
        {
            *peMode = AUD_FMT_MPEG;
        }
        // Note: MP2/MP3 has the same type in IEC61937!
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG1_L2L3)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG2_L1LSF)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG2_L2LSF)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else if (((u4Reg & TYPE_DETAIL) == TYPE_DTS1)||((u4Reg & TYPE_DETAIL) == TYPE_DTS2)||((u4Reg & TYPE_DETAIL) == TYPE_DTS3))
        {
            fgHDMIDTS = TRUE;
            if(AUD_DspIsFormatValid(AUD_FMT_DTS))
            {
                *peMode = AUD_FMT_DTS;
            }
            else
            {
                *peMode = AUD_FMT_UNKNOWN;
            }
        }
        else
        {
            *peMode = AUD_FMT_UNKNOWN;
            LOG(3, "SPDIF format not support\n");
        }
        peOriMode = *peMode;

    }
}
#endif

void AUD_AinSetMode(UINT8 u1Mode)
{
    if (u1Mode == 0) // PCM
    {
#ifdef USE_16BIT_ALIGNMENT
        AUD_CLR_BIT(REG_LIN_CFG0, DATA_PCM_24_BITS);
#else
        AUD_SET_BIT(REG_LIN_CFG0, DATA_PCM_24_BITS);
#endif

    }
    else if (u1Mode == 1) // RAW
    {
        AUD_CLR_BIT(REG_LIN_CFG0, DATA_PCM_24_BITS);
    }
}

//-----------------------------------------------------------------------------
/** AUD_AinSrcSel
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinSrcSel(UINT8 u1DspId, UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eAudFmt)
{
    UINT32 u4Format2 = 0;
#ifdef CC_HDMI_PCM_MULT_CHANNEL
    UINT8  u1ChNum = 2;
#endif
    UINT32 u4RegAddr = REG_LIN_CFG0;

    AUD_DEC_ID_VALIDATE(u1DecId);

    // Setup format
    if (eStreamFrom == AUD_STREAM_FROM_LINE_IN_3)
    {
        u4RegAddr = REG_DEC3_LIN_CFG0;
    }
    else if (eStreamFrom == AUD_STREAM_FROM_LINE_IN_2)
    {
        u4RegAddr = REG_DEC2_LIN_CFG0;
    }
    else
    {
        u4RegAddr = REG_LIN_CFG0;
    }

    u4Format2= AUD_READ32(u4RegAddr) & (~SPDIF_FMT_MASK);

    if ((eStreamFrom == AUD_STREAM_FROM_SPDIF) ||
        (eStreamFrom == AUD_STREAM_FROM_HDMI)
#ifdef CC_AUD_I2S_IN
        ||(eStreamFrom == AUD_STREAM_FROM_I2S)
#endif        
        )
    {
        if (eAudFmt == AUD_FMT_PCM)
        {
#ifdef  USE_16BIT_ALIGNMENT
            u4Format2 |= (DATA_RAW_16_BITS |
                          DATA_NO_SWAP |
                          RISC_INT_PERIOD_256 |
                          INT_ADC_EXTLIN |
                          PCM_CH_NUM_2_CH);
#else
            u4Format2 |= (DATA_RAW_16_BITS |
                          DATA_NO_SWAP |
                          RISC_INT_PERIOD_256 |
                          INT_ADC_EXTLIN |
                          PCM_CH_NUM_2_CH);
#endif

#ifdef CC_HDMI_PCM_MULT_CHANNEL
            if (eStreamFrom == AUD_STREAM_FROM_HDMI)
            {
                u1ChNum = API_HDMI_Get_Aud_ChannelNum()+1;
            }
            else
            {
                u1ChNum = 2;
            }
            AUD_LineInChNumFormat(u1ChNum, &u4Format2);
#endif
        }
        else
        {
            u4Format2 |= (DATA_RAW_16_BITS |
                          DATA_NO_SWAP |
                          RISC_INT_PERIOD_256 |
                          INT_ADC_EXTLIN |
                          PCM_CH_NUM_2_CH);
        }

#ifndef CC_MT5365    //No SPDIF in in MT5365
        // Init SPDIF-in receiver
        SPDIF_InEnable(TRUE);
#endif
#ifdef CC_MT5881
        // Remy use internal ADC path but not really use interdac ADC
        if (eStreamFrom == AUD_STREAM_FROM_SPDIF)
        {
            u4Format2 |= INT_ADC_INTLIN;
        }
#endif
        AUD_WRITE32(REG_LIN_CFG0, u4Format2);
    }
    else if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
    { 
#ifdef USE_16BIT_ALIGNMENT
        u4Format2 |= (DATA_RAW_16_BITS |
                      DATA_NO_SWAP |
                      RISC_INT_PERIOD_256 |
#ifdef CC_FPGA
                      INT_ADC_EXTLIN |
#else
                      INT_ADC_INTLIN |
#endif
                      PCM_CH_NUM_2_CH/*FIXME*/);
#else
        u4Format2 |= (DATA_PCM_24_BITS |
                      DATA_NO_SWAP |
                      RISC_INT_PERIOD_256 |
                      INT_ADC_INTLIN |
                      PCM_CH_NUM_2_CH/*FIXME*/);
#endif

#ifndef DEC3_USE_LINEIN2
        AUD_WRITE32(REG_LIN_CFG0, u4Format2);
#else
        if (u1DecId == AUD_DEC_THIRD)
        {
            AUD_WRITE32(REG_DEC2_LIN_CFG0, u4Format2);
            //AUD_WRITE32(REG_DEC3_LIN_CFG0, u4Format2);    //DEC3_USE_LINEIN3
        }
        else
        {
            AUD_WRITE32(REG_LIN_CFG0, u4Format2);
        }
#endif
    }
    else if (eStreamFrom == AUD_STREAM_FROM_LINE_IN_2)
    { 
#ifdef USE_16BIT_ALIGNMENT        
          u4Format2 |= (DATA_RAW_16_BITS |
                    DATA_NO_SWAP |
                    RISC_INT_PERIOD_256 |
                    INT_ADC_INTLIN |
                    PCM_CH_NUM_2_CH/*FIXME*/);  
#else
          u4Format2 |= (DATA_PCM_24_BITS |
                    DATA_NO_SWAP |
                    RISC_INT_PERIOD_256 |
                    INT_ADC_INTLIN |
                    PCM_CH_NUM_2_CH/*FIXME*/);        
#endif          
                    
        AUD_WRITE32(REG_DEC2_LIN_CFG0, u4Format2);
    }
    else if (eStreamFrom == AUD_STREAM_FROM_LINE_IN_3)
    {
        /// To Do
        /// I2S have Echo Cancel 4CH and BT 2CH need to change.

#ifdef USE_16BIT_ALIGNMENT        
        u4Format2 |= (DATA_RAW_16_BITS |
                    DATA_NO_SWAP |
                    RISC_INT_PERIOD_256 |
                    INT_ADC_INTLIN |
                    PCM_CH_NUM_2_CH);  
#else
        u4Format2 |= (DATA_PCM_24_BITS |
                    DATA_NO_SWAP |
                    RISC_INT_PERIOD_256 |
                    INT_ADC_INTLIN |
                    PCM_CH_NUM_2_CH);        
#endif          

        AUD_WRITE32(REG_DEC3_LIN_CFG0, u4Format2);
    }

    switch (eStreamFrom)
    {
        case AUD_STREAM_FROM_SPDIF:
            AUD_AinCfg(u1DecId, AUD_STREAM_FROM_SPDIF, (const AIN_CFG_T* )&_rSPDIFInCfg);
            break;
        case AUD_STREAM_FROM_LINE_IN:
            AUD_AinCfg(u1DecId, AUD_STREAM_FROM_LINE_IN, (const AIN_CFG_T * )&_rLineInCfg);
            break;
        case AUD_STREAM_FROM_LINE_IN_2:
            AUD_AinCfg(u1DecId, AUD_STREAM_FROM_LINE_IN_2, (const AIN_CFG_T * )&_rLineInCfg);
            break;
        case AUD_STREAM_FROM_LINE_IN_3:
            AUD_AinCfg(u1DecId, AUD_STREAM_FROM_LINE_IN_3, (const AIN_CFG_T * )&_rLineInCfg);
            break;
        case AUD_STREAM_FROM_HDMI:
            AUD_AinCfg(u1DecId, AUD_STREAM_FROM_HDMI, (const AIN_CFG_T* )&_rHDMIInCfg);
            break;
#ifdef CC_AUD_I2S_IN
        case AUD_STREAM_FROM_I2S:
            AUD_AinCfg(u1DecId, AUD_STREAM_FROM_I2S, (const AIN_CFG_T * )&_rI2SInCfg);
            break;  
#endif            
        default:
            ASSERT(0);
    }

}

//-----------------------------------------------------------------------------
/** AUD_AinEnable
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN, AUD_DEC_AUX, AUD_DEC_THIRD).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinEnable(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Enable input source
    switch (eStreamFrom)
    { 
        case AUD_STREAM_FROM_SPDIF:
        case AUD_STREAM_FROM_HDMI:
        case AUD_STREAM_FROM_LINE_IN:
#ifdef CC_AUD_I2S_IN
        case AUD_STREAM_FROM_I2S:    
#endif            
            if (fgEnable)
            {

#ifndef DEC3_USE_LINEIN2
                AUD_SET_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
#else
                if (u1DecId == AUD_DEC_THIRD)
                {
                    AUD_SET_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
                    //AUD_SET_BIT(REG_DEC3_LIN_CFG0, SPDIF_LINE_IN_ENABLE);     //DEC3_USE_LINEIN3
                }
                else
                {
                    AUD_SET_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
                }
#endif
            }
            else
            {
#ifndef DEC3_USE_LINEIN2
                AUD_CLR_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
#else
                if (u1DecId == AUD_DEC_THIRD)
                {
                    AUD_CLR_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
                    //AUD_CLR_BIT(REG_DEC3_LIN_CFG0, SPDIF_LINE_IN_ENABLE);     //DEC3_USE_LINEIN3
                }
                else
                {
                    AUD_CLR_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
                }
#endif
            }
            break;
       case AUD_STREAM_FROM_LINE_IN_2:   
            if (fgEnable)
            {
                AUD_SET_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);   
            }
            else
            {
                AUD_CLR_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);   
            }
            break;  
       case AUD_STREAM_FROM_LINE_IN_3:   
            if (fgEnable)
            {
                AUD_SET_BIT(REG_DEC3_LIN_CFG0, SPDIF_LINE_IN_ENABLE);   
            }
            else
            {
                AUD_CLR_BIT(REG_DEC3_LIN_CFG0, SPDIF_LINE_IN_ENABLE);   
            }
            break;  

        default:
            break;
    }
}

#if defined(CC_MT5881)
//-----------------------------------------------------------------------------
/** AUD_SPDIFIN_CONF
 *  Set value for audio input related configuration
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_SPDIFIN_CONF(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (AUD_STREAM_FROM_SPDIF == eStreamFrom)
    {
        // 5680 configure
        //AUD_SET_BIT(REG_SPDIFIN_CFG0, SPDIF_RX_INTEN);
        AUD_SET_BIT(REG_SPDIFIN_CFG0, SPDIF_RX_DECNT);
        AUD_WRITE32(REG_SPDIFIN_CFG0, AUD_READ32(REG_SPDIFIN_CFG0) | SPDIF_RX_DESEL);
        AUD_WRITE32(REG_SPDIFIN_CFG0, AUD_READ32(REG_SPDIFIN_CFG0) | SPDIF_RX_MAXLEN);

        AUD_SET_BIT(REG_SPDIFIN_CFG0, SPDIF_RX_DATA_FLIP);
        //AUD_SET_BIT(REG_SPDIFIN_CFG0, SPDIF_RX_ENABLE);

        // 5684 configure
        AUD_SET_BIT(REG_SPDIFIN_CFG1, SPDIF_RX_SEL);
        AUD_SET_BIT(REG_SPDIFIN_CFG1, SPDIF_RX_SEL_CLK);
        AUD_SET_BIT(REG_SPDIFIN_CFG1, SPDIF_RX_DEC2_SEL);
        AUD_SET_BIT(REG_SPDIFIN_CFG1, SPDIF_RX_DEC2_DEL_CLK);
        AUD_CLR_BIT(REG_SPDIFIN_CFG1, SPDIF_RX_BYPASS); // clear spdif-in bypass spdif-out bit
    }
}

void AUD_Spdifin_Enable(UINT8 u1DecId, BOOL bEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    if (bEnable)
    {
        AUD_SET_BIT(REG_SPDIFIN_CFG0, SPDIF_RX_ENABLE);
        LOG(3, "Audio Spdif-in Enable\n");
    }
    else
    {
        AUD_CLR_BIT(REG_SPDIFIN_CFG0, SPDIF_RX_ENABLE);
        LOG(3, "Audio Spdif-in Disable\n");
    }
}

BOOL AUD_Spdifin_Err_Chk(void)
{
    UINT32 u4SpdifErrStatus;
    UINT32 u4SpdifErrStatusEn;
    BOOL   bRet = TRUE;

    u4SpdifErrStatus = AUD_READ32(REG_SPDIFIN_DEBUG3);
    u4SpdifErrStatusEn = AUD_READ32(REG_SPDIFIN_CFG1);
    // First level error detect --> plug in to plug out
    if ((u4SpdifErrStatus & SPDIF_RX_TIMEOUT_ERR) == SPDIF_RX_TIMEOUT_ERR)
    {
        LOG(3, "First level error detect -- plug out error!\n");
        bRet = FALSE;
    }
    // Second level error detect --> Preamble error
    if ((((u4SpdifErrStatus & SPDIF_RX_PREAMBLE_ERR) == SPDIF_RX_PREAMBLE_ERR) ||
         ((u4SpdifErrStatus & SPDIF_RX_PREAMBLE_BERR) == SPDIF_RX_PREAMBLE_BERR) ||
         ((u4SpdifErrStatus & SPDIF_RX_PREAMBLE_MERR) == SPDIF_RX_PREAMBLE_MERR) ||
         ((u4SpdifErrStatus & SPDIF_RX_PREAMBLE_WERR) == SPDIF_RX_PREAMBLE_WERR)) &&
        ((u4SpdifErrStatusEn & SPDIF_RX_PREAMBLE_ERROR) == SPDIF_RX_PREAMBLE_ERROR))
    {
        LOG(3, "Second level error detect -- Preamble error!\n");
        bRet = FALSE;
    }
    // Third level error detect --> Decoding error
    if (((u4SpdifErrStatus & SPDIF_RX_SMP_COUNTER_ERR) == SPDIF_RX_SMP_COUNTER_ERR ||
         (u4SpdifErrStatus & SPDIF_RX_PARITY_ERR) == SPDIF_RX_PARITY_ERR) &&
        ((u4SpdifErrStatusEn & SPDIF_RX_INVALID_BITCNT) == SPDIF_RX_INVALID_BITCNT ||
         (u4SpdifErrStatusEn & SPDIF_RX_PARITY_CHKERR) == SPDIF_RX_PARITY_CHKERR))
    {
        LOG(3, "Third level error detect -- Decoding error!\n");
        bRet = FALSE;
    }
    return bRet;
}

void AUD_Spdifin_Err_Handle(void)
{
    // Disable Line-in
    AUD_SET_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
    // Disable Spdif-in
    AUD_CLR_BIT(REG_SPDIFIN_CFG0, SPDIF_RX_ENABLE);
    LOG(3, "Audio Spdif-in & Line-in Disable (For Error Handle)\n");
}
#endif

//-----------------------------------------------------------------------------
/** AUD_AinCfg
 *  Set value for audio input related configuration
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinCfg(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStrSrc, const AIN_CFG_T* prAinCfg)
{
    UINT32 u4AUDCFG = 0;
#ifdef KARAOKE_MIC_LINE_IN2
    UINT32 u4AUDCFG_0 = 0;
#endif

#ifdef KARAOKE_MIC_LINE_IN2
    UINT32 u4REG_LIN_CFG = 0;
    UINT32 u4AUDCFG_0 = 0;

    if (eStrSrc == AUD_STREAM_FROM_LINE_IN_2)
    {
        u4REG_LIN_CFG = REG_DEC2_LIN_CFG0;
    }
    else
    {
        u4REG_LIN_CFG = REG_LIN_CFG0;
    }
#endif

    // Update local variables
    switch (eStrSrc)
    {
        case AUD_STREAM_FROM_LINE_IN:
        case AUD_STREAM_FROM_LINE_IN_2:
        case AUD_STREAM_FROM_LINE_IN_3:
            x_memcpy((VOID*)VIRTUAL((UINT32)&_rLineInCfg), (const VOID*) prAinCfg, sizeof(AIN_CFG_T));
            break;
        case AUD_STREAM_FROM_HDMI:
            x_memcpy((VOID*)VIRTUAL((UINT32)&_rHDMIInCfg), (const VOID*) prAinCfg, sizeof(AIN_CFG_T));
            break;
        case AUD_STREAM_FROM_SPDIF:
            x_memcpy((VOID*)VIRTUAL((UINT32)&_rSPDIFInCfg), (const VOID*) prAinCfg, sizeof(AIN_CFG_T));
            break;
#ifdef CC_AUD_I2S_IN
        case AUD_STREAM_FROM_I2S:
            x_memcpy((VOID *)VIRTUAL((UINT32)&_rI2SInCfg), (const VOID *) prAinCfg, sizeof(AIN_CFG_T));
            break; 
#endif            
        default:
            LOG(1, "ERROR AUD_AinCfg\n");
            return;
    }
    
#ifdef KARAOKE_MIC_LINE_IN2
    // Save original audio output setting
    if (eStrSrc == AUD_STREAM_FROM_LINE_IN_2)
    {    
        u4AUDCFG_0 = AUD_READ32(REG_EXTADC_CFG) & (~(AUD_IN_CFG_MASK << 16)); 
        u4AUDCFG = (AUD_READ32(REG_EXTADC_CFG)>>16) & (~AUD_IN_CFG_MASK);
    }
    else
    {
        u4AUDCFG = AUD_READ32(REG_EXTADC_CFG) & (~AUD_IN_CFG_MASK);
    }
#else
    u4AUDCFG = AUD_READ32(REG_EXTADC_CFG) & (~AUD_IN_CFG_MASK);
#endif

    if (prAinCfg != NULL)
    {
        switch (prAinCfg->eFormat)
        {
            case FORMAT_I2S:
                u4AUDCFG |= SPLIN_I2S;
                break;
            case FORMAT_LJ:
                u4AUDCFG |= SPLIN_LJ;
                break;
            case FORMAT_RJ:
                u4AUDCFG |= SPLIN_RJ;
                break;
            default:
                break;
        }

        switch (prAinCfg->eBits)
        {
            case DAC_16_BIT:
                u4AUDCFG |= ADC_16BIT;
                break;
            case DAC_18_BIT:
                u4AUDCFG |= ADC_18BIT;
                break;
            case DAC_20_BIT:
                u4AUDCFG |= ADC_20BIT;
                break;
            case DAC_24_BIT:
                u4AUDCFG |= ADC_24BIT;
                break;
            default:
                break;
        }

        switch (prAinCfg->eCycle)
        {
            case LRCK_CYC_16:
                u4AUDCFG |= SPDIF_CYC16;
                break;
            case LRCK_CYC_24:
                u4AUDCFG |= SPDIF_CYC24;
                break;
            case LRCK_CYC_32:
                u4AUDCFG |= SPDIF_CYC32;
                break;
            default:
                break;
        }

        if (prAinCfg->fgLRInvert)
        {
            u4AUDCFG |= SPLIN_LR_INVT;
        }
    	if (eStrSrc == AUD_STREAM_FROM_LINE_IN_3)
        {
            AUD_WRITE32(REG_EXTADC_CFG2, (u4AUDCFG&0xffff)); //for line-in 3 control setting
            AUD_CLR_BIT(REG_DEC3_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
            AUD_SET_BIT(REG_DEC3_LIN_CFG0, SPDIF_LINE_IN_ENABLE);   
        }
        else if (eStrSrc == AUD_STREAM_FROM_LINE_IN_2)
        {
            AUD_WRITE32(REG_EXTADC_CFG, (u4AUDCFG&0xffff) | ((u4AUDCFG&0xffff)<<16)); //for line-in 2 control setting
            AUD_CLR_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
            AUD_SET_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
        }
        else
        {
#ifdef DEC3_USE_LINEIN2
            if (u1DecId == AUD_DEC_THIRD)
            {
                AUD_WRITE32(REG_EXTADC_CFG, (u4AUDCFG & 0xffff) | ((u4AUDCFG & 0xffff) << 16)); //for line-in 2 control setting
                AUD_CLR_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
                AUD_SET_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
            }
            else
            {
                AUD_WRITE32(REG_EXTADC_CFG, u4AUDCFG);
                AUD_CLR_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
                AUD_SET_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
            }
#else		
            AUD_WRITE32(REG_EXTADC_CFG, u4AUDCFG);
            AUD_CLR_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
            AUD_SET_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
#endif
        }
    }
    else
    {
        ASSERT(0);
    }
}

//-----------------------------------------------------------------------------
/** AUD_DrvCurConvert
 *
 *  @param  u1DrvCur
 *
 *  @retval UINT8
 */
//-----------------------------------------------------------------------------
static UINT8 AUD_DrvCurConvert(UINT8 u1DrvCur)
{
    UINT8 u1DrvCurRet = PADDRV_8MA;

    switch (u1DrvCur)
    {
        case 0:
        case 1:
        case 2:
            u1DrvCurRet = PADDRV_2MA;
            break;
        case 3:
        case 4:
            u1DrvCurRet = PADDRV_4MA;
            break;
        case 5:
        case 6:
            u1DrvCurRet = PADDRV_6MA;
            break;
        case 7:
        case 8:
            u1DrvCurRet = PADDRV_8MA;
            break;
        default:
            u1DrvCurRet = PADDRV_8MA;
            break; 
    }
    
    return u1DrvCurRet;
}

//-----------------------------------------------------------------------------
/** AUD_AoutDrvCur
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutDrvCur(UINT8 u1DecId, UINT8 u1MckDrvCur, UINT8 u1BckDrvCur , UINT8 u1LRckDrvCur, UINT8 u1AOSDATADrvCur)
{
    UINT8 u1MckCur, u1BckCur, u1LRckCur, u1SDATACur;
    
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    u1MckCur = AUD_DrvCurConvert(u1MckDrvCur);
    u1BckCur = AUD_DrvCurConvert(u1BckDrvCur);
    u1LRckCur = AUD_DrvCurConvert(u1LRckDrvCur);
    u1SDATACur = AUD_DrvCurConvert(u1AOSDATADrvCur);

    _au1MckDrvCur[u1DecId] = (u1MckCur + 1) *2;
    _au1BckDrvCur[u1DecId] = (u1BckCur + 1) *2;
    _au1LRckDrvCur[u1DecId] = (u1LRckCur + 1) *2;
    _au1AOSDATADrvCur[u1DecId] = (u1SDATACur + 1) *2;

#if defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    //PAD_E4_0: 0xF000d540, PAD_E2_0: 0xF000D550
    //(PAD_E4_0, PAD_E2_0): 00: 2mA, 01: 4mA, 10: 6mA, 11: 8mA
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE2_0, (u1MckCur & 1), FLD_PADDRV_AOMCLK);
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE4_0, ((u1MckCur>>1) & 1), FLD_PADDRV_AOMCLK);
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE2_0, (u1BckCur & 1), FLD_PADDRV_AOBCK);
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE4_0, ((u1BckCur>>1) & 1), FLD_PADDRV_AOBCK);    
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE2_0, (u1LRckCur & 1), FLD_PADDRV_AOLRCK);
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE4_0, ((u1LRckCur>>1) & 1), FLD_PADDRV_AOLRCK);
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE2_0, (u1SDATACur & 1), FLD_PADDRV_AOSDATA0);
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE4_0, ((u1SDATACur>>1) & 1), FLD_PADDRV_AOSDATA0);

    // Light: Set SPDIF output PAD driving current to 8mA
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE4_0, (PADDRV_8MA & 1), FLD_PADDRV_ASPDIF0);
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE2_0, ((PADDRV_8MA>>1) & 1), FLD_PADDRV_ASPDIF0);
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE4_0, (PADDRV_8MA & 1), FLD_PADDRV_ASPDIF1);
    vIO32WriteFldAlign(CKGEN_PADDRVCFGE2_0, ((PADDRV_8MA>>1) & 1), FLD_PADDRV_ASPDIF1);

#elif defined(CC_MT5881)
    vIO32WriteFldAlign(PDWNC_PADCFG0, (u1MckCur & 1), FLD_AOMCLK_E2);
    vIO32WriteFldAlign(PDWNC_PADCFG0, ((u1MckCur>>1) & 1), FLD_AOMCLK_E4);
    vIO32WriteFldAlign(PDWNC_PADCFG1, (u1BckCur & 1), FLD_AOBCK_E2);
    vIO32WriteFldAlign(PDWNC_PADCFG1, ((u1BckCur>>1) & 1), FLD_AOBCK_E4);    
    vIO32WriteFldAlign(PDWNC_PADCFG1, (u1LRckCur & 1), FLD_AOLRCK_E2);
    vIO32WriteFldAlign(PDWNC_PADCFG1, ((u1LRckCur>>1) & 1), FLD_AOLRCK_E4);
    vIO32WriteFldAlign(PDWNC_PADCFG1, (u1SDATACur & 1), FLD_AOSDATA0_E2);
    vIO32WriteFldAlign(PDWNC_PADCFG1, ((u1SDATACur>>1) & 1), FLD_AOSDATA0_E4);
    
    // Light: Set SPDIF output PAD driving current to 8mA
    vIO32WriteFldAlign(PDWNC_PADCFG0, (PADDRV_8MA & 1), FLD_ASPDIFO0_E2);
    vIO32WriteFldAlign(PDWNC_PADCFG0, ((PADDRV_8MA >> 1) & 1), FLD_ASPDIFO0_E4);
#else
    //Fix klocwork warning
    UNUSED(u1MckCur);
    UNUSED(u1BckCur);
    UNUSED(u1LRckCur);
    UNUSED(u1SDATACur); 
#endif

}

#ifdef CC_COMBINE_4K_2K
void AUD_AoutCfgFs(SAMPLE_FREQ_T eFs)
{
    UNUSED(eFs);
    UNUSED(AUD_AoutCfgFs);
    if(eFs==FS_96K)
    {
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(BCK_DIV_MASK)) | BCK_DIV_4);        
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(IEC_DIV_MASK)) | IEC_DIV_2);
    }
    else
    {
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(BCK_DIV_MASK)) | BCK_DIV_8);
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(IEC_DIV_MASK)) | IEC_DIV_4);
    }    
}
#endif //CC_4K_MODEL


//-----------------------------------------------------------------------------
/** AUD_AoutCfg
 *  Audio output configuration
 *  1. Setup Aout registers
 *  2. Setup ADAC
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutCfg(UINT8 u1DecId, const AOUT_CFG_T* prAoutCfg)
{
    UINT32 u4AUDCFG = 0;
    UINT32 u4AUDCFG2 = 0;
    UINT32 u4AUDCFG3 = 0;

    AUD_DEC_ID_VALIDATE_2(u1DecId);

    // Update configuration to local variable
    if (prAoutCfg != NULL)
    {
        x_memcpy((VOID*)VIRTUAL((UINT32)&_arAudOutCfg[u1DecId]), (const VOID*) prAoutCfg, sizeof(AOUT_CFG_T));
    }

    if (prAoutCfg != NULL)
    {
        switch (prAoutCfg->eFormat)
        {
            case FORMAT_RJ:
                u4AUDCFG |= AOFMT_RJ;
                break;
            case FORMAT_LJ:
                u4AUDCFG |= AOFMT_LJ;
                break;
            case FORMAT_I2S:
                u4AUDCFG |= AOFMT_I2S;
                break;
            default:
                u4AUDCFG |= AOFMT_I2S;
        }

        switch (prAoutCfg->eSampleFreq)
        {
            case MCLK_128FS:
                u4AUDCFG2 |= (BCK_DIV_2 );
                u4AUDCFG3 |= (DEC2_BCK_DIV_2 );
                break;
            case MCLK_256FS:
                u4AUDCFG2 |= (BCK_DIV_4 );
                u4AUDCFG3 |= (DEC2_BCK_DIV_4 );
                break;
            case MCLK_384FS:
                u4AUDCFG2 |= (BCK_DIV_6 );
                u4AUDCFG3 |= (DEC2_BCK_DIV_6);
                break;
            case MCLK_512FS:
                u4AUDCFG2 |= (BCK_DIV_8 );
                u4AUDCFG3 |= (DEC2_BCK_DIV_8);
                break;
            default:
                u4AUDCFG2 |= (BCK_DIV_4);
                u4AUDCFG3 |= (DEC2_BCK_DIV_4 );
                LOG( 3, "Sampling Freq not support\n");
        }

        switch (prAoutCfg->eCycle)
        {
            case LRCK_CYC_16:
                u4AUDCFG |= LRCK_CYC16;
                break;
            case LRCK_CYC_24:
                u4AUDCFG |= LRCK_CYC24;
                break;
            case LRCK_CYC_32:
                u4AUDCFG |= LRCK_CYC32;
                break;
            default:
                u4AUDCFG |= LRCK_CYC24;
        }

        switch (prAoutCfg->eBits)
        {
            case DAC_16_BIT:
                u4AUDCFG |= DAC_16BIT;
                break;
            case DAC_18_BIT:
                u4AUDCFG |= DAC_18BIT;
                break;
            case DAC_20_BIT:
                u4AUDCFG |= DAC_20BIT;
                break;
            case DAC_24_BIT:
                u4AUDCFG |= DAC_24BIT;
                break;
            default:
                u4AUDCFG |= DAC_24BIT;
        }

        // If invert the Sdata output
        if (prAoutCfg->fgDataInvert)
        {
            u4AUDCFG |= INV_SDATA;
        }

        // Light: For I2S format the INV_LRCK is not set in normal mode.
        // Light: For I2S format the INV_LRCK is set in swap mode.
        // Light: For LJ and RJ formats the INV_LRCK is set in normal mode.
        // Light: For LJ and RJ formats the INV_LRCK is not set in swap mode.
        if ((prAoutCfg->eFormat == FORMAT_RJ) || (prAoutCfg->eFormat == FORMAT_LJ))
        {
            if (!prAoutCfg->fgLRInvert)
            {
                u4AUDCFG |= INV_LRCK;
            }
        }
        else // For I2S format and default
        {
            // If invert L/R output
            if (prAoutCfg->fgLRInvert)
            {
                u4AUDCFG |= INV_LRCK;
            }
        }

        // setting driving current
        //u4AUDCFG |= IEC_CUR_4;  ???
        u4AUDCFG |= INV_BCK;
        AUD_AoutDrvCur(u1DecId, _au1MckDrvCur[u1DecId], _au1BckDrvCur[u1DecId], _au1LRckDrvCur[u1DecId] , _au1AOSDATADrvCur[u1DecId]);

    }

    if (u1DecId == AUD_DEC_MAIN)
    {
        AUD_WRITE32(REG_EXTDAC_CFG0, (AUD_READ32(REG_EXTDAC_CFG0) & (~AUD_OUT_CFG_MASK)) | u4AUDCFG);
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(BCK_DIV_MASK)) | u4AUDCFG2);
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(IEC_DIV_MASK)) | IEC_DIV_2);
    }
    else // AUD_DEC_AUX
    {
        AUD_WRITE32(REG_EXTDAC_CFG1, (AUD_READ32(REG_EXTDAC_CFG1) & (~AUD_OUT_CFG_MASK)) | u4AUDCFG);
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(DEC2_BCK_DIV_MASK)) | u4AUDCFG3);
        AUD_WRITE32(REG_ACK_CFG, (AUD_READ32(REG_ACK_CFG) & ~(DEC2_IEC_DIV_MASK)) | DEC2_IEC_DIV_2);
    }

    // Update ADAC
    if (prAoutCfg != NULL)
    {
        ADAC_DacFormatCfg(u1DecId, prAoutCfg->eFormat, prAoutCfg->eSampleFreq);
    }
#ifdef CC_COMBINE_4K_2K
    if((DRVCUST_Get_2K_Model()&0x01)==0x01) //4K model
    {   
        AUD_AoutCfgFs(FS_48K);
    }
#endif    
}

//-----------------------------------------------------------------------------
/** AUD_AoutFormat
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutFormat(UINT8 u1DecId, DATA_FORMAT_T eDataFormat)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if (_arAudOutCfg[u1DecId].eFormat != eDataFormat)
    {
        _arAudOutCfg[u1DecId].eFormat = eDataFormat;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }
}

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_AoutFormat
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutMclk(UINT8 u1DecId, MCLK_FREQUENCY_T eMclk)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if (_arAudOutCfg[u1DecId].eSampleFreq != eMclk)
    {
        _arAudOutCfg[u1DecId].eSampleFreq = eMclk;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }
}
#endif

//-----------------------------------------------------------------------------
/** AUD_AoutDacFs
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutDacFs(UINT8 u1DecId, SAMPLE_FREQ_T eFs)
{
    UNUSED(u1DecId);
    UNUSED(eFs);
}

//-----------------------------------------------------------------------------
/** AUD_AoutInvertData
 *  Invert the Sdata output for OP phase
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutInvertData(UINT8 u1DecId, BOOL fgInvert)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if (_arAudOutCfg[u1DecId].fgDataInvert != fgInvert)
    {
        _arAudOutCfg[u1DecId].fgDataInvert = fgInvert;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }
}

#if 0   // Unused
//-----------------------------------------------------------------------------
/** AUD_AoutDataNum
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutDataNum(UINT8 u1DecId, DAC_DATA_NUMBER_T eDataNum)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if (_arAudOutCfg[u1DecId].eBits != eDataNum)
    {
        _arAudOutCfg[u1DecId].eBits = eDataNum;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }
}

//-----------------------------------------------------------------------------
/** AUD_AoutLRCycle
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutLRCycle(UINT8 u1DecId, LRCK_CYC_T eCycle)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if (_arAudOutCfg[u1DecId].eCycle != eCycle)
    {
        _arAudOutCfg[u1DecId].eCycle = eCycle;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }
}
#endif

//-----------------------------------------------------------------------------
/** AUD_AoutPADMux
 *
 *  @param  PAD_SDATA0 (AOUTDATA0_MAIN ~ AOUTDATA4_MAIN, AOUTDATA0_AUX),
 *          PAD_SDATA1, ..., PAD_SDATA5,
 *          PAD_MCLK0 (MCLK0 / MCLK1)
 *          Notice that PAD_MCLK1 (MCLK0 /MCLK1) is decided by PAD_SDATA5
 *
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutPADMux(AUD_CH_T ePAD_SDATA0, AUD_CH_T ePAD_SDATA1, AUD_CH_T ePAD_SDATA2,
                    AUD_CH_T ePAD_SDATA3, AUD_CH_T ePAD_SDATA4, AUD_CH_T ePAD_SDATA5,
                    UINT8 PAD_MCLK0)
{
    UINT32 u4ADSP_ENV_BAK;

    //Conflict check
    if (((((UINT32)ePAD_SDATA0) / 2) != (((UINT32)ePAD_SDATA5) / 2)) &&
        ((((UINT32)ePAD_SDATA0) / 2) <= AOUTDATA4_MAIN) &&
        ((((UINT32)ePAD_SDATA5) / 2) <= AOUTDATA4_MAIN))
    {
        Printf("AUD_AoutPADMux DATA0/DATA5 conflict\n");
        return;
    }
    //End

    u4ADSP_ENV_BAK = AUD_READ32(REG_CH_CFG) & (~(CH_SEL | SDATA_MCLK_SYNC));

    //PAD_SDATA0
    switch (((UINT32)ePAD_SDATA0) / 2)
    {
        default:
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA0;
            break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA2;
            break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA3;
            break;
		case AOUTDATA4_MAIN:
			u4ADSP_ENV_BAK |= CH0_AOUTDATA4;
			break;
		case AOUTDATA5_MAIN:
			u4ADSP_ENV_BAK |= CH0_AOUTDATA5;
			break;
		case AOUTDATA6_MAIN:
			u4ADSP_ENV_BAK |= CH0_AOUTDATA6;
			break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH0_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA0);
            break;
    }

    //PAD_SDATA1
    switch (((UINT32)ePAD_SDATA1) / 2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH1_AOUTDATA0;
            break;
        default:
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH1_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH1_AOUTDATA2;
            break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH1_AOUTDATA3;
            break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK |= CH1_AOUTDATA4;
            break;
		case AOUTDATA5_MAIN:
			u4ADSP_ENV_BAK |= CH1_AOUTDATA5;
			break;
		case AOUTDATA6_MAIN:
			u4ADSP_ENV_BAK |= CH1_AOUTDATA6;
			break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH1_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA1);
            break;
    }

    //PAD_SDATA2
    switch (((UINT32)ePAD_SDATA2) / 2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH2_AOUTDATA0;
            break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH2_AOUTDATA1;
            break;
        default:
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH2_AOUTDATA2;
            break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH2_AOUTDATA3;
            break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK |= CH2_AOUTDATA4;
            break;
		case AOUTDATA5_MAIN:
			u4ADSP_ENV_BAK |= CH2_AOUTDATA5;
			break;
		case AOUTDATA6_MAIN:
			u4ADSP_ENV_BAK |= CH2_AOUTDATA6;
			break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH2_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA2);
            break;
    }

    //PAD_SDATA3
    switch (((UINT32)ePAD_SDATA3) / 2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH3_AOUTDATA0;
            break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH3_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH3_AOUTDATA2;
            break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH3_AOUTDATA3;
            break;
		case AOUTDATA4_MAIN:
			u4ADSP_ENV_BAK |= CH3_AOUTDATA4;
			break;
		case AOUTDATA5_MAIN:
			u4ADSP_ENV_BAK |= CH3_AOUTDATA5;
			break;
		case AOUTDATA6_MAIN:
			u4ADSP_ENV_BAK |= CH3_AOUTDATA6;
			break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH0_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA3);
            break;
    }

    //PAD_SDATA4
    switch (((UINT32)ePAD_SDATA4) / 2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH4_AOUTDATA0;
            break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH4_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH4_AOUTDATA2;
            break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH4_AOUTDATA3;
            break;
        default:
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK |= CH4_AOUTDATA4;
            break;
		case AOUTDATA5_MAIN:
     		u4ADSP_ENV_BAK |= CH4_AOUTDATA5;
			break;
		case AOUTDATA6_MAIN:
			u4ADSP_ENV_BAK |= CH4_AOUTDATA6;
			break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH1_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA4);
            break;
    }

    //PAD_SDATA5
    switch (((UINT32)ePAD_SDATA5) / 2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA0;
            break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA2;
            break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA3;
            break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA4;
            break;
		case AOUTDATA5_MAIN:
			u4ADSP_ENV_BAK |= CH0_AOUTDATA5;
			break;
		case AOUTDATA6_MAIN:
			u4ADSP_ENV_BAK |= CH0_AOUTDATA6;
			break;
        default:
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH0_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA5);
            break;
    }

#ifdef CC_AUD_EU_INTERNALDAC_SAME_APLL
    u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | AO_BCK_LRCK_SEL;
#endif
    AUD_WRITE32(REG_CH_CFG, u4ADSP_ENV_BAK);

    if ((BOOL)DRVCUST_OptGet(eAudioAI2SuseforSpeaker))
    {
        AUD_CLR_BIT(REG_CH_CFG, AO_BCK_LRCK_SEL);
    }
    //AUD_WRITE32(REG_CH_CFG, u4AENV_CFG);     // (Jessica)

    UNUSED (PAD_MCLK0);
}

static CHAR* _aszPadCh[] = {"LR", "SLR", "CLFE", "BYPASS", "DOWNMIX"};

void AUD_AoutPADMuxQuery(void)
{
    UINT8 u1Index;
    UINT32 u4Tmp = AUD_READ32(REG_CH_CFG);

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA0)
    {
        //DEC1
        u1Index = ((u4Tmp >> DEC2_CH0_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("AOSDATA0: DEC1 - %s\n", _aszPadCh[u1Index]);
        }
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp >> CH0_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("AOSDATA0: DEC0 - %s\n", _aszPadCh[u1Index]);
        }
    }

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA1)
    {
        //DEC1
        u1Index = ((u4Tmp >> DEC2_CH1_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("AOSDATA1: DEC1 - %s\n", _aszPadCh[u1Index]);
        }
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp >> CH1_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("AOSDATA1: DEC0 - %s\n", _aszPadCh[u1Index]);
        }
    }

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA2)
    {
        //DEC1
        u1Index = ((u4Tmp >> DEC2_CH2_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("AOSDATA2: DEC1 - %s\n", _aszPadCh[u1Index]);
        }
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp >> CH2_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("AOSDATA2: DEC0 - %s\n", _aszPadCh[u1Index]);
        }
    }

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA3)
    {
        //DEC1
        u1Index = ((u4Tmp >> DEC2_CH0_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("AOSDATA3: DEC1 - %s\n", _aszPadCh[u1Index]);
        }
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp >> CH3_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("AOSDATA3: DEC0 - %s\n", _aszPadCh[u1Index]);
        }
    }

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA4)
    {
        //DEC1
        u1Index = ((u4Tmp >> DEC2_CH1_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("AOSDATA4: DEC1 - %s\n", _aszPadCh[u1Index]);
        }
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp >> CH4_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("AOSDATA4: DEC0 - %s\n", _aszPadCh[u1Index]);
        }
    }

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA5)
    {
        //DEC1
        u1Index = ((u4Tmp >> DEC2_CH0_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("BOSDATA0: DEC1 - %s\n", _aszPadCh[u1Index]);
        }
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp >> CH0_SEL_POS) & 0x7);
        if (u1Index < 5)
        {
            Printf("BOSDATA0: DEC0 - %s\n", _aszPadCh[u1Index]);
        }
    }
}

//-----------------------------------------------------------------------------
/** AUD_LineInCtrlEnable
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
//#include "x_timer.h"
//static HAL_TIME_T _rTime;
void AUD_LineInCtrlEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (fgEnable)
    {
        // Turn on line-in buffer control
#ifndef DEC3_USE_LINEIN2
        AUD_SET_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);            //laihui
#else
        if (u1DecId == AUD_DEC_THIRD)
        {
            AUD_SET_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
        }
        else
        {
            AUD_SET_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
        }
#endif
    }
    else
    {
        // Turn off line-in buffer control
#ifndef DEC3_USE_LINEIN2
        AUD_CLR_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);        //laihui
#else
        if (u1DecId == AUD_DEC_THIRD)
        {
            AUD_CLR_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
        }
        else
        {
            AUD_CLR_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
        }
#endif
    }

    if (fgEnable)
    {
        LOG(5, "LineInCtrl start capturing\n");
        // HAL_GetTime(&_rTime);
        // Printf("LineInCtrl start capturing (%d.%06d)\n", _rTime.u4Seconds, _rTime.u4Micros);
    }
    else
    {
        LOG(5, "LineInCtrl stop capturing\n");
        // HAL_GetTime(&_rTime);
        // Printf("LineInCtrl stop capturing (%d.%06d)\n", _rTime.u4Seconds, _rTime.u4Micros);
    }
}

//-----------------------------------------------------------------------------
/** AUD_OutPadEnable
 *  Enable/disable audio output pad
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  fgEnable  1: enable (Set MCLK/LRCK/BCLK/DATA pad as output)   \n
 *                    0: diable (Set MCLK/LRCK/BCLK/DATA pad as tiastate)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable)
{
    if (fgEnable)
    {
        AUD_SET_BIT(REG_ACK_CFG, (u1DecId == AUD_DEC_MAIN) ? DEC0_MTRI : DEC1_MTRI);
    }
    else
    {
        AUD_CLR_BIT(REG_ACK_CFG, (u1DecId == AUD_DEC_MAIN) ? DEC0_MTRI : DEC1_MTRI);
    }

    LOG(8, "AUD_OutPadEnable u1DecId = %d\n", u1DecId);
}

void AUD_AvMuxSel(AUD_INPUT_ID_T eInputId)
{
    UINT32 u4GpioSelNum1 = UNUSED_GPIO;
    UINT32 u4GpioSelNum2 = UNUSED_GPIO;
    UINT32 u4GpioSelNum3 = UNUSED_GPIO;
    UINT32 u4GpioSelNum4 = UNUSED_GPIO;
    INT32  i4GpioSel1Value = NO_USED;
    INT32  i4GpioSel2Value = NO_USED;
    INT32  i4GpioSel3Value = NO_USED;
    INT32  i4GpioSel4Value = NO_USED;
    UINT8  u1AacChlSel;
    INT32  i4Out = 1;
    static BOOL fgInit = FALSE;
    AUD_INPUT_SW_GPIO_T* prInputSwGpio;
    AUD_INPUT_MUX_SEL_TABLE_T* prInputMuxSelTable;

    // Query ADAC config
    UNUSED(DRVCUST_OptQuery(eAudioInputSwGpioSel, (UINT32*)(void*)&prInputSwGpio));

    UNUSED(DRVCUST_OptQuery(eAudioInputMuxSelTable, (UINT32*)(void*)&prInputMuxSelTable));

#ifdef CC_AUD_USE_NVM
    if (!Aud_NVM_GetCountry())
    {
        if (!fgInit)
        {
            prInputMuxSelTable->prAudInMuxSel += ((INT32)(prInputMuxSelTable->u4Size)) / 2;
        }
    }
#endif // CC_AUD_USE_NVM

    u1AacChlSel = prInputMuxSelTable->prAudInMuxSel[eInputId].u1AdcMuxSel;
    if (u1AacChlSel >= (UINT8)AUD_INPUT_ID_MAX)
    {
        LOG(0, "Invalid AdacChlSel %d\n", (UINT32)u1AacChlSel);
    }

    // Set adac channel
    ADAC_CodecAdChlSel(prInputMuxSelTable->prAudInMuxSel[eInputId].u1AdcMuxSel);

    // Load configuration
    u4GpioSelNum1 = prInputSwGpio->u4AudInSwGpioNum1;
    u4GpioSelNum2 = prInputSwGpio->u4AudInSwGpioNum2;
    u4GpioSelNum3 = prInputSwGpio->u4AudInSwGpioNum3;
    u4GpioSelNum4 = prInputSwGpio->u4AudInSwGpioNum4;
    i4GpioSel1Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum1Val;
    i4GpioSel2Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum2Val;
    i4GpioSel3Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum3Val;
    i4GpioSel4Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum4Val;

    // Setup gpio  => Init gpio
    if (!fgInit)
    {
        if (u4GpioSelNum1 != UNUSED_GPIO)
        {
            AUD_GPIOEnable(u4GpioSelNum1, i4Out);
        }
        if (u4GpioSelNum2 != UNUSED_GPIO)
        {
            AUD_GPIOEnable(u4GpioSelNum2, i4Out);
        }
        if (u4GpioSelNum3 != UNUSED_GPIO)
        {
            AUD_GPIOEnable(u4GpioSelNum3, i4Out);
        }
        if (u4GpioSelNum4 != UNUSED_GPIO)
        {
            AUD_GPIOEnable(u4GpioSelNum4, i4Out);
        }

        fgInit = TRUE;
    }

    // Setup gpio  => write gpio
    if ((u4GpioSelNum1 != UNUSED_GPIO) && (i4GpioSel1Value != SW_NO_USED))
    {
        AUD_GPIOOutput(u4GpioSelNum1, i4GpioSel1Value);
    }
    if ((u4GpioSelNum2 != UNUSED_GPIO) && (i4GpioSel2Value != SW_NO_USED))
    {
        AUD_GPIOOutput(u4GpioSelNum2, i4GpioSel2Value);
    }
    if ((u4GpioSelNum3 != UNUSED_GPIO) && (i4GpioSel3Value != SW_NO_USED))
    {
        AUD_GPIOOutput(u4GpioSelNum3, i4GpioSel3Value);
    }
    if ((u4GpioSelNum4 != UNUSED_GPIO) && (i4GpioSel4Value != SW_NO_USED))
    {
        AUD_GPIOOutput(u4GpioSelNum4, i4GpioSel4Value);
    }

    //Setup DVD GPIO
    _eInputIdToDvdGpioConfig = eInputId;
    AUD_DspDvdEnable();
}

void AUD_AvMuxSelByAD(UINT8 eADNum)
{
    AUD_INPUT_SW_GPIO_T* prInputSwGpio;
    AUD_INPUT_MUX_SEL_TABLE_T* prInputMuxSelTable;

    // Query ADAC config
    UNUSED(DRVCUST_OptQuery(eAudioInputSwGpioSel, (UINT32*)(void*)&prInputSwGpio));

    UNUSED(DRVCUST_OptQuery(eAudioInputMuxSelTable, (UINT32*)(void*)&prInputMuxSelTable));
    // Set adac channel
    ADAC_CodecAdChlSel(eADNum);
    AUD_DspDvdEnable();
}


INT32 _AUD_GetAudFifo(UINT8 u1DspId, UINT8 u1DecId, UINT32 * pu4FifoStart, UINT32 * pu4FifoEnd)
{
    if ((pu4FifoStart != NULL) && (pu4FifoEnd != NULL))
    {
#ifdef CC_MT53XX_SUPPORT_2G_DRAM
        *pu4FifoStart = (u4GetAFIFOStart(u1DspId, u1DecId));
        *pu4FifoEnd = (u4GetAFIFOEnd(u1DspId, u1DecId));
#else
        *pu4FifoStart = DSP_PHYSICAL_ADDR(u4GetAFIFOStart(u1DspId, u1DecId));
        *pu4FifoEnd = DSP_PHYSICAL_ADDR(u4GetAFIFOEnd(u1DspId, u1DecId));
#endif
    }
    else
    {
        return AUD_FAIL;
    }

    //LOG(10,"DSP(%D) Audio Fifo (%d) = (%x,%x)\n", u1DspId, u1DecId, *pu4FifoStart, *pu4FifoEnd);

    return AUD_OK;
}


void AUD_SetSrc(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    _aeStrSrcConf[u1DecId] = eStreamFrom;
    _fgChangeSource[u1DecId] = TRUE;
}

MCLK_FREQUENCY_T AUD_GetMclkFs(UINT8 u1ApllId)
{
    return  _arAudOutCfg[u1ApllId].eSampleFreq;
}

// *********************************************************************
// Function : void AUD_DspDvdEnable(void)
// Description : turn on / off dvd gpio by input source and linein select
// Parameter :  none
// Return    :  None.
// *********************************************************************
void AUD_DspDvdEnable(void)
{
#ifdef CC_AUD_DVD_SUPPORT    
    UINT32 u4DvdGpio;
    AUD_INPUT_ID_T eDvdInputChl;
    AUD_DVD_GPIO_POLARITY_T eDvdPolariy;
    INT32 i4RegAoutSwitch;

    //Get DVD input source and DVD gpio Num
    u4DvdGpio = DRVCUST_OptGet(eAudioDvdGpio);
    eDvdInputChl = (AUD_INPUT_ID_T)DRVCUST_OptGet(eAudioDvdInputChl);

    eDvdPolariy = (AUD_DVD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioDvdPolarity);

    LOG(10, "-------- AUD_DspDvdEnable Configuration -------\n");
    LOG(10, "           DVD Gpio : %d\n", u4DvdGpio);
    LOG(10, "           Dvd Input Channel : %d\n", eDvdInputChl);
    LOG(10, "           DVD Polarity : %d\n", eDvdPolariy);
    LOG(10, "           Decoder Stream Source : %d\n", _aeStrSrcConf[AUD_DEC_MAIN]);
    LOG(10, "           _eInputIdToDvdGpioConfig = %d\n", _eInputIdToDvdGpioConfig);
    LOG(10, "-------- AUD_DspDvdEnable Configuration -------\n");

    if ((_aeStrSrcConf[AUD_DEC_MAIN] != AUD_STREAM_FROM_LINE_IN)  ||
        (_eInputIdToDvdGpioConfig != eDvdInputChl))
    {
        if (eDvdPolariy == AUD_DVD_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch = 0;
        }
        else
        {
            i4RegAoutSwitch = 1;
        }
        LOG(10, "Aout Switch (1) : %d\n", i4RegAoutSwitch);
        AUD_GPIOOutput(u4DvdGpio, i4RegAoutSwitch);
    }
    else if ((u4DvdGpio != 0xffffffff) && (_eInputIdToDvdGpioConfig == eDvdInputChl)
             && (_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_LINE_IN) )
    {
        if (eDvdPolariy == AUD_DVD_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch = 1;
        }
        else
        {
            i4RegAoutSwitch = 0;
        }
        LOG(10, "Aout Switch (2) : %d\n", i4RegAoutSwitch);
        AUD_GPIOOutput(u4DvdGpio, i4RegAoutSwitch);
    }
    else
    {
        LOG(3, "Invalid eStreamFrom in AUD_DspDvdEnable\n");
    }
#endif    
}

AUD_INPUT_ID_T AUD_GetAvInputSrcId(void)
{
    return _eInputIdToDvdGpioConfig;
}

#ifndef CC_MT5565_TURNKEY
#ifndef CC_LGE_PROTO_PCBA 
UINT8 bSpdifFunc = PINMUX_FUNCTION1; // 1: SPDIF 2: HDMI_BYPASS
#else
UINT8 bSpdifFunc = PINMUX_FUNCTION2; // 2: SPDIF
#endif
void AUD_SPDIF_Output_HDMI_Rx(BOOL fgEnable)
{
    if (BSP_PinGet(AUD_SPDIF0) != PINMUX_FUNCTION0)
    { 
        if (fgEnable)
        {
            bSpdifFunc = PINMUX_FUNCTION3; 
            AUD_SET_BIT(REG_SPOCFG0, HDMI_RX_SPDIF);
            BSP_PinSet(AUD_SPDIF0, PINMUX_FUNCTION3);
            LOG(9, "SPDIF output source from HDMI Rx\n");
        }
        else
        {
            bSpdifFunc = PINMUX_FUNCTION2; 
            AUD_CLR_BIT(REG_SPOCFG0, HDMI_RX_SPDIF);
            BSP_PinSet(AUD_SPDIF0, PINMUX_FUNCTION2);
            LOG(9, "SPDIF output source from audio module\n");
        }
    }
}
#else //Turnkey Viper-E use TCON3 as SPDIF out, ASPDIFO0 only for ARC
UINT8 bSpdifFunc = PINMUX_FUNCTION4; // TCON3 no HDMI SPDIF out function.

void AUD_SPDIF_Output_HDMI_Rx(BOOL fgEnable)
{
    //Fix me.
    LOG(5, "Viper-E SPDIF (TCON3 pin) no HDMI SPDIF out function\n");
}
#endif


#ifdef CC_AUD_HDMI_SPDIF_CFG
void AUD_SPDIF_Output_Mute(BOOL fgMute)
{
    if (fgMute)
    {
        BSP_PinSet(AUD_SPDIF0, PINMUX_FUNCTION0); 
        LOG(6, "SPDIF output Mute\n");
    }
    else
    {
        BSP_PinSet(AUD_SPDIF0, bSpdifFunc);
        LOG(6, "SPDIF output Un-Mute\n");
    }
}
#endif

void AUD_SPDIF_Enable(BOOL fgEnable)
{
#ifdef CC_DSP_SUPPORT_ARC
    fgSPDIFEnable = fgEnable;
#endif
    if (!fgEnable)
    {
        BSP_PinSet(AUD_SPDIF0, PINMUX_FUNCTION0);
#if defined(CC_DSP_SUPPORT_ARC) && defined(AUD_SPDIF1)
        BSP_PinSet(AUD_SPDIF1, PINMUX_FUNCTION0);
#endif            
    }
    else
    {
        BSP_PinSet(AUD_SPDIF0, bSpdifFunc);
#if defined(CC_DSP_SUPPORT_ARC) && defined(AUD_SPDIF1)
        if (_fgArcEnableFlag)
        {
            BSP_PinSet(AUD_SPDIF1, bSpdifFunc);
        }
#endif
    }
}

INT16 AUD_SPDIFGet(void)
{ 
    return  BSP_PinGet(AUD_SPDIF0); 
}

//-----------------------------------------------------------------------------
// vCloseIec
//
/** Close IEC.
 */
//-----------------------------------------------------------------------------
void vCloseIec(void)
{
    BSP_PinSet(AUD_SPDIF0, PINMUX_FUNCTION0);
}

//-----------------------------------------------------------------------------
// vOpenIec
//
/** Open IEC.
 */
//-----------------------------------------------------------------------------
void vOpenIec(void)
{
    BSP_PinSet(AUD_SPDIF0, bSpdifFunc);
}

UINT32 AUD_DspGetPc(UINT8 u1DspId)
{

    AUD_DSP_ID_VALIDATE(u1DspId);
    if (u1DspId==AUD_DSP0) 
    {
        return AUD_READ32(REG_DSP_PC);
    }    
    else
    {
        return AUD_READ32(REG_DSP_PC_DSPB);
    }     
}

static void AUD_DspProbeEnable(UINT8 u1DspId)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    if (u1DspId==AUD_DSP0) 
    {
        if(!(AUD_READ32(REG_DSP_CTRL_R) & 0x80))
        {
            AUD_WRITE32(REG_DSP_CTRL_R,0x80|AUD_READ32(REG_DSP_CTRL_R));
        }
    }
    else
    {
        //if(!(AUD_READ32(REG_DSP_CTRL_R_DSPB) & 0x80))
        {
            //AUD_WRITE32(REG_DSP_CTRL_R_DSPB,0x80|AUD_READ32(REG_DSP_CTRL_R_DSPB));
            AUD_WRITE32(REG_DSP_CTRL_R_DSPB,(0x80|(AUD_READ32(REG_DSP_CTRL_R_DSPB)&(~0x00010000))));  //workaround for FPGA bit file v071910
        }
    }
}

UINT32 AUD_DspProbePause(UINT8 u1DspId)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DspProbeEnable(u1DspId);
    if (u1DspId==AUD_DSP0) AUD_WRITE32(REG_DSP_PB_CTRL,0x100);
    if (u1DspId==AUD_DSP1) AUD_WRITE32(REG_DSP_PB_CTRL_DSPB,0x100);
 
    return 0;
}

UINT32 AUD_DspProbeGo(UINT8 u1DspId)
{
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DspProbeEnable(u1DspId);
    if (u1DspId==AUD_DSP0) AUD_WRITE32(REG_DSP_PB_CTRL,0x000);
    if (u1DspId==AUD_DSP1) AUD_WRITE32(REG_DSP_PB_CTRL_DSPB,0x000);

    return 0;
}

UINT32 AUD_DspProbeStep(UINT8 u1DspId, UINT32 u4Count)
{
    UINT32 u4Data;
    u4Data = u4Count << 8;
    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DspProbeEnable(u1DspId);
    if (u1DspId==AUD_DSP0) AUD_WRITE32(REG_DSP_PB_CTRL,u4Data);
    if (u1DspId==AUD_DSP1) AUD_WRITE32(REG_DSP_PB_CTRL_DSPB,u4Data);
    return 0;
}

UINT32 fgAUD_DspProbeHalt(UINT8 u1DspId)
{

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DspProbeEnable(u1DspId);
    if (u1DspId==AUD_DSP0) return (AUD_READ32(REG_DSP_PB_CTRL)&0x1);
    else  return (AUD_READ32(REG_DSP_PB_CTRL_DSPB)&0x1);
}

UINT32 u4AUD_DspProbeRead(UINT8 u1DspId, UINT32 u4Addr)
{
    UINT16 u2Addr;
    u2Addr = u4Addr & 0xFFFF;
    AUD_DspProbeEnable(u1DspId);
    if (u1DspId==AUD_DSP0) 
    {    
    AUD_WRITE32(REG_DSP_PB_INDEX, u2Addr);
    return (AUD_READ32(REG_DSP_PB_DATA));
}
    else
    {
        AUD_WRITE32(REG_DSP_PB_INDEX_DSPB,u2Addr);
        return (AUD_READ32(REG_DSP_PB_DATA_DSPB));
    }
}

UINT32 AUD_DspProbeWrite(UINT8 u1DspId, UINT32 u4Addr, UINT32 u4Data)
{
    UINT16 u2Addr;
    u2Addr = u4Addr & 0xFFFF;
    AUD_DspProbeEnable(u1DspId);
    if (u1DspId==AUD_DSP0) 
    {       
    AUD_WRITE32(REG_DSP_PB_WRDX, u2Addr);
    AUD_WRITE32(REG_DSP_PB_WR, u4Data);
    }
    else
    {
        AUD_WRITE32(REG_DSP_PB_WRDX_DSPB,u2Addr);
        AUD_WRITE32(REG_DSP_PB_WR_DSPB,u4Data);
    }

    return 0;
}

UINT32 u4AUD_DspProbeStopOnWrite(UINT8 u1DspId, UINT32 u4Addr, UINT32 u4Data, UINT32 u4Mask)
{
    UINT32 u4MaskD;
    UINT32 u4Temp;

    UNUSED(u4Addr);
    UNUSED(u4Temp);

    u4Addr = u4Addr & 0xFFFF;

    if (u1DspId==AUD_DSP0) 
    { 
    if (u4Addr != 0)
    {
        u4MaskD = 0x1FFFF;
            AUD_DspProbeEnable(AUD_DSP0);
        AUD_WRITE32 (REG_DSP_PB_HIDX, 0x084);
        AUD_WRITE32 (REG_DSP_PB_CMSK, u4MaskD);
        AUD_WRITE32 (REG_DSP_PB_CTAR, (0x010000 | u4Addr));
        AUD_WRITE32 (REG_DSP_PB_WMSK, u4Mask);
        AUD_WRITE32 (REG_DSP_PB_WTAR, u4Data);
        AUD_WRITE32 (REG_DSP_PB_TAR, 0x0100);
    }
    else
    {
        u4Temp = AUD_READ32(REG_DSP_PB_TAR);
        AUD_WRITE32 (REG_DSP_PB_TAR, (u4Temp & 0xfffffeff));
    }
    }
    else
    {
        if (u4Addr != 0)
        {
            u4MaskD = 0x1FFFF;
            AUD_DspProbeEnable(AUD_DSP1);
            AUD_WRITE32 (REG_DSP_PB_HIDX_DSPB, 0x084);
            AUD_WRITE32 (REG_DSP_PB_CMSK_DSPB, u4MaskD);
            AUD_WRITE32 (REG_DSP_PB_CTAR_DSPB, (0x010000|u4Addr));
            AUD_WRITE32 (REG_DSP_PB_WMSK_DSPB, u4Mask);
            AUD_WRITE32 (REG_DSP_PB_WTAR_DSPB, u4Data);
            AUD_WRITE32 (REG_DSP_PB_TAR_DSPB, 0x0100);
        }
        else
        {
            u4Temp = AUD_READ32(REG_DSP_PB_TAR_DSPB);
            AUD_WRITE32 (REG_DSP_PB_TAR_DSPB, (u4Temp & 0xfffffeff));
        }
    }

    return 0;
}


UINT32 u4AUD_DspProbeChkStopOnWrite(UINT8 u1DspId, BOOL *fgEnable, UINT32 *u4Data, UINT32 *u4Mask)
{
    UINT32 u4Addr;

    if (u1DspId==AUD_DSP0) 
    { 
    *fgEnable = (AUD_READ32(REG_DSP_PB_TAR) & (0x100)) >> 8;
    u4Addr = AUD_READ32(REG_DSP_PB_CTAR) & (0xFFFF);
    *u4Data = AUD_READ32(REG_DSP_PB_WTAR);
    *u4Mask = AUD_READ32(REG_DSP_PB_WMSK);
    }
    else
    {
        *fgEnable = (AUD_READ32(REG_DSP_PB_TAR_DSPB)&(0x100)) >> 8;
        u4Addr = AUD_READ32(REG_DSP_PB_CTAR_DSPB)& (0xFFFF);
        *u4Data = AUD_READ32(REG_DSP_PB_WTAR_DSPB);
        *u4Mask = AUD_READ32(REG_DSP_PB_WMSK_DSPB);
    }
    return u4Addr;

}

UINT32 u4AUD_DspProbePBCfg(UINT8 u1DspId, BOOL fgSet,UINT32 u4Value)
{
    AUD_DspProbeEnable(u1DspId);
    if (u1DspId==AUD_DSP0) 
{
    if (fgSet)
    {
        AUD_WRITE32(REG_DSP_PB_STOP, u4Value);
        return 0;
    }
    return (AUD_READ32(REG_DSP_PB_STOP));
}
    else
    {
        if(fgSet)
        {
            AUD_WRITE32(REG_DSP_PB_STOP_DSPB,u4Value);
            return 0;
        }
        return (AUD_READ32(REG_DSP_PB_STOP_DSPB));
    }
}

UINT32 u4AUD_DspProbePB(UINT8 u1DspId, BOOL fgSet,UINT32 u4BPNum, UINT32 u4Addr)
{
    AUD_DspProbeEnable(u1DspId);
    if (u1DspId==AUD_DSP0) 
{
    if (fgSet)
    {
        if (u4BPNum < 4)
        {
            AUD_WRITE32((REG_DSP_PB_STP0 + (u4BPNum << 2)), u4Addr);
            return 0;
        }
        return 0;
    }
    if (u4BPNum < 4)
    {
        return(AUD_READ32((REG_DSP_PB_STP0 + (u4BPNum << 2))));
    }
    return 0xFFFFFFFF;
}
    else
    {
        if(fgSet)
        {
            if(u4BPNum<4)
            {
                AUD_WRITE32((REG_DSP_PB_STP0_DSPB+(u4BPNum<<2)),u4Addr);
                return 0;
            }
            return 0;
        }
        if(u4BPNum<4)
        {
            return(AUD_READ32((REG_DSP_PB_STP0_DSPB+(u4BPNum<<2))));
        }
        return 0xFFFFFFFF;
    }
}
#if 0   // Unused
UINT32 AUD_AudioDescriptionChlSel(UINT8 u1DecId, AUD_CHL_DEF_T eChl)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    ADAC_DacSetChl(u1DecId, eChl);
    return 0;
}
#endif

static void AUD_LineinLRDetect(UINT8 u1LineInId , BOOL fgEnable)
{
#if 0    
    UNUSED(AUD_LineinLRDetect);
#ifndef DEC3_USE_LINEIN2
    if (fgEnable)
    {
        AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) | (LR_DETECT_ON)));
    }
    else
    {
        AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LR_DETECT_ON)));
    }
#else
    if (fgEnable)
    {
        AUD_WRITE32(((u1DecId == AUD_DEC_THIRD) ? REG_DEC2_LIN_CFG0 : REG_LIN_CFG0), (AUD_READ32(((u1DecId == AUD_DEC_THIRD) ? REG_DEC2_LIN_CFG0 : REG_LIN_CFG0)) | (LR_DETECT_ON)));
        //AUD_WRITE32(((u1DecId == AUD_DEC_THIRD)? REG_DEC3_LIN_CFG0: REG_LIN_CFG0), (AUD_READ32(((u1DecId == AUD_DEC_THIRD)? REG_DEC3_LIN_CFG0: REG_LIN_CFG0)) | (LR_DETECT_ON)));     //DEC3_USE_LINEIN3
    }
    else
    {
        AUD_WRITE32(((u1DecId == AUD_DEC_THIRD) ? REG_DEC2_LIN_CFG0 : REG_LIN_CFG0), (AUD_READ32(((u1DecId == AUD_DEC_THIRD) ? REG_DEC2_LIN_CFG0 : REG_LIN_CFG0)) & (~LR_DETECT_ON)));
        //AUD_WRITE32(((u1DecId == AUD_DEC_THIRD)? REG_DEC3_LIN_CFG0: REG_LIN_CFG0), (AUD_READ32(((u1DecId == AUD_DEC_THIRD)? REG_DEC3_LIN_CFG0: REG_LIN_CFG0)) & (~LR_DETECT_ON)));    //DEC3_USE_LINEIN3
    }
#endif
#else
    UINT32 u4RegAddr = REG_LIN_CFG0;

    UNUSED(AUD_LineinLRDetect);
    if (u1LineInId==AUD_LINEIN_THIRD)          u4RegAddr = REG_DEC3_LIN_CFG0;
    else if (u1LineInId==AUD_LINEIN_AUX)   u4RegAddr = REG_DEC2_LIN_CFG0;
    else                              u4RegAddr = REG_LIN_CFG0;

    if (fgEnable)
    {
        AUD_WRITE32(u4RegAddr, (AUD_READ32(u4RegAddr) | (LR_DETECT_ON)));
    }
    else
    {
        AUD_WRITE32(u4RegAddr, (AUD_READ32(u4RegAddr) & (~LR_DETECT_ON)));
    }
#endif
}

//-----------------------------------------------------------------------------
/** AUD_GPIOOutput
 *
 *  @param  u4GPIONum       GPIO number
 *  @param  i4polarity      polarity
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_GPIOOutput(UINT32 u4GPIONum, INT32 i4Polarity)
{
    UNUSED(GPIO_Output((INT32)u4GPIONum, (INT32*)&i4Polarity));
}

//-----------------------------------------------------------------------------
/** AUD_GPIOEnable
 *
 *  @param  u4GPIONum       GPIO number
 *  @param  i4polarity      polarity
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_GPIOEnable(UINT32 u4GPIONum, INT32 i4Polarity)
{
    UNUSED(GPIO_Enable((INT32)u4GPIONum, (INT32*)&i4Polarity));
}

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
UINT32 AUD_GetLineinWP(void)
{
    AUD_WRITE32(REG_ABUF_PNT(AUD_DSP0), LINE_IN_SEL_WP);
    return DSP_PHYSICAL_ADDR(AUD_READ32(REG_ABUF_PNT(AUD_DSP0)));
}
#endif

/*  AOUT HW Gain control  since MT5368
*  @param.  u1Decid (MAIN or AUX)
*  @param.  CHID (All channel or L. to CH14)
*  @param.  Gain..( 0x1000 --0 db, 0x0 -- mute)
*/
void AudAoutGainCtrl(UINT8 u1DecId, UINT8 CHID, UINT16 Gain, UINT8 u1Step)
{
    return ; // at present,do not need use this function
    
#ifndef __MODEL_slt__ // SLT bypass Aout HW Gain 
    if (u1DecId > AUD_DEC_AUX)
    {
        return;
    }

    if ( u1DecId == AUD_DEC_MAIN) //AOUT
    {
        //enable risc control aout gain
        AUD_WRITE32(REG_AOUT_DRAM_CFG, (AUD_READ32(REG_AOUT_DRAM_CFG))&~AOUT_VOL_CTRL_BY_DSP);
        //enable aout gain control
        AUD_WRITE32(REG_AOUT_VOL_GAIN_SETUP, (AUD_READ32(REG_AOUT_VOL_GAIN_SETUP))&~AOUT_GAIN_BYPASS);

        //init gain step
        AUD_WRITE32(REG_AOUT_VOL_GAIN_SETUP, ((AUD_READ32(REG_AOUT_VOL_GAIN_SETUP))&~AOUT_VOL_STEP_MARK) | u1Step);

        //setup  target aout channel
        AUD_WRITE32(REG_AOUT_VOL_GAIN_CTRL , ( (AUD_READ32(REG_AOUT_VOL_GAIN_CTRL)&~CH_GAIN_SEL_MARK) | CHID << 16));

        //setup target gain

        Gain &= 0xFFFF;

        AUD_WRITE32(REG_AOUT_VOL_GAIN_CTRL , ( (AUD_READ32(REG_AOUT_VOL_GAIN_CTRL)&~AOUT_VOL_GAIN_MARK) | Gain));
#if defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        //trigger Gain Ctrl
        AUD_WRITE32(REG_AOUT_VOL_GAIN_CTRL , ( (AUD_READ32(REG_AOUT_VOL_GAIN_CTRL)&~CH_GAIN_SEL_MARK) | CH_GAIN_SEL_LOAD));
#endif
        //swith  the hw control to DSP
        //AUD_WRITE32(REG_AOUT_DRAM_CFG, (AUD_READ32(REG_AOUT_DRAM_CFG))|AOUT_VOL_CTRL_BY_DSP);

    }
    else  //AOUT2
    {
        //enable risc control aout2 gain
        AUD_WRITE32(REG_AOUT_DRAM_CFG, (AUD_READ32(REG_AOUT_DRAM_CFG))&~AOUT2_VOL_CTRL_BY_DSP);
        //enable aout2 gain control
        AUD_WRITE32(REG_AOUT2_VOL_GAIN_SETUP, (AUD_READ32(REG_AOUT2_VOL_GAIN_SETUP))&~AOUT2_GAIN_BYPASS);

        //init gain step
        AUD_WRITE32(REG_AOUT2_VOL_GAIN_SETUP, ((AUD_READ32(REG_AOUT2_VOL_GAIN_SETUP))&~AOUT2_VOL_STEP_MARK) | u1Step);

        //setup  target aout2 channel
        AUD_WRITE32(REG_AOUT2_VOL_GAIN_CTRL , ( (AUD_READ32(REG_AOUT2_VOL_GAIN_CTRL)&~CH_GAIN_SEL_MARK) | CHID << 16));

        //setup target gain

        Gain &= 0xFFFF;

        AUD_WRITE32(REG_AOUT2_VOL_GAIN_CTRL , ( (AUD_READ32(REG_AOUT2_VOL_GAIN_CTRL)&~AOUT_VOL_GAIN_MARK) | Gain));

        //swith  the hw control to DSP
        //AUD_WRITE32(REG_AOUT_DRAM_CFG, (AUD_READ32(REG_AOUT_DRAM_CFG))|AOUT2_VOL_CTRL_BY_DSP);

    }
#endif

}

#if defined(CC_AUD_ARM11_SUPPORT)
void AudAprocJtagPinmux(void)
{
	#ifndef CC_MT5882
    // CKGEN
    //D600H=0x5 => PMUX0
    vIO32WriteFldAlign(CKGEN_PMUX0, 0x5, FLD_PAD_GPIO56);
    //D610H=0x55550000 => PMUX4
    vIO32WriteFldAlign(CKGEN_PMUX4, 0x5, FLD_PAD_GPIO57);
    vIO32WriteFldAlign(CKGEN_PMUX4, 0x5, FLD_PAD_GPIO58);
    vIO32WriteFldAlign(CKGEN_PMUX4, 0x5, FLD_PAD_GPIO59);
    vIO32WriteFldAlign(CKGEN_PMUX4, 0x5, FLD_PAD_GPIO60);

	#endif
#ifdef CC_MT5882
	BSP_PinSet(PIN_HDMI_0_SDA, PINMUX_FUNCTION3);
	BSP_PinSet(PIN_HDMI_0_SCL, PINMUX_FUNCTION3);
	BSP_PinSet(PIN_HDMI_1_SDA, PINMUX_FUNCTION3);
	BSP_PinSet(PIN_HDMI_1_SCL, PINMUX_FUNCTION3);
#endif    
}

void AudAprocModuleCtrl(UINT32 u4Module, UINT32 u4Flag)
{
    UINT32 u4Ctrl;

    u4Ctrl = AUD_READ32(REG_MODULE_CTRL);
    switch(u4Module)
    {
    case 0: // AOUT1
        u4Ctrl &= (UINT32)(~AOUT1_SEL_MASK);
        u4Ctrl |= ((u4Flag & 0x3) << AOUT1_SEL_POS);
        AUD_WRITE32(REG_MODULE_CTRL, u4Ctrl);
        break;
    case 1:
        u4Ctrl &= (UINT32)(~AOUT2_SEL_MASK);
        u4Ctrl |= ((u4Flag & 0x3) << AOUT2_SEL_POS);
        AUD_WRITE32(REG_MODULE_CTRL, u4Ctrl);
        break;
    case 2:
        u4Ctrl &= (UINT32)(~IEC_SEL_MASK);
        u4Ctrl |= ((u4Flag & 0x3) << IEC_SEL_POS);
        AUD_WRITE32(REG_MODULE_CTRL, u4Ctrl);
        break;
    case 3:
        u4Ctrl &= (UINT32)(~PUTBS_SEL_MASK);
        u4Ctrl |= ((u4Flag & 0x3) << PUTBS_SEL_POS);
        AUD_WRITE32(REG_MODULE_CTRL, u4Ctrl);
        break;
    case 4:
        u4Ctrl &= (UINT32)(~ASRC1_SEL_MASK);
        u4Ctrl |= ((u4Flag & 0x3) << ASRC1_SEL_POS);
        AUD_WRITE32(REG_MODULE_CTRL, u4Ctrl);
        break;
    case 5:
        u4Ctrl &= (UINT32)(~ASRC2_SEL_MASK);
        u4Ctrl |= ((u4Flag & 0x3) << ASRC2_SEL_POS);
        AUD_WRITE32(REG_MODULE_CTRL, u4Ctrl);
        break;
    case 6:
        u4Ctrl &= (UINT32)(~ASRC3_SEL_MASK);
        u4Ctrl |= ((u4Flag & 0x3) << ASRC3_SEL_POS);
        AUD_WRITE32(REG_MODULE_CTRL, u4Ctrl);
        break;
    default:
        Printf("Not supported Module\n");
        break;
    }
}
#endif // CC_AUD_ARM11_SUPPORT

