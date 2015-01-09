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
 * $RCSfile: aud_detect.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file smp_sample.c
 *  Brief of file aud_detect.c.
 *  Details of file aud_detect.c (optional).
 */

#define AUD_EXTERN_FILE

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_os.h"
#include "x_gpio.h"
#include "x_assert.h"
#include "aud_debug.h"
#include "aud_if.h"
#include "drv_common.h"
#include "drvcust_if.h"
#include "x_typedef.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define AUD_HEADPHONE_PLUG_IN_MON_THREAD_NAME   "AudHpPlugIn"
#define AUD_DC_DETECTION_MON_THREAD_NAME        "AudDcDetect"
#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
#define AUD_AMP_INIT_GPIO_DETECTION_MON_THREAD_NAME	"AmpInitDetectGpio"
#define AUD_AMP_INIT_REG_DETECTION_MON_THREAD_NAME	"AmpInitDetectReg"
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
//#define CC_AUD_HP_POLLING 1
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern BOOL _AUD_GetNotifyFunc(AudDecNfyFct * pfNfyFunc);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

static BOOL _IsUseHeadphonePlugInMonThread(void);
static BOOL _IsUseDcDetectionMonThread(void);
#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
static BOOL _IsUseAmpInitGpioDetectionMonThread(void);
static BOOL _IsUseAmpInitRegDetectionMonThread(void);
#endif
//static void _AudHeadphonePlugInMonThread(void* pvArg);
//static void _AudDcDetectionMonThread(void* pvArg);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static HANDLE_T _hAudHeadphonePlugInMonThread;
static HANDLE_T _hAudDcDetectionMonThread;
#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
static HANDLE_T _hAudAmpInitGpioDetectionMonThread;
static HANDLE_T _hAudAmpInitRegDetectionMonThread;
#endif

static HANDLE_T _hSemaHpPlugInWait;
static HANDLE_T _hSemaDcDetectionWait;
#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
static HANDLE_T _hSemaAmpInitGpioDetectionWait;
#endif
#define HP_DETECT_ANDROID
#ifdef HP_DETECT_ANDROID
static BOOL isPlugin = FALSE;
BOOL isHpPlugIn(void) {
	return isPlugin;
}
EXPORT_SYMBOL(isHpPlugIn);
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static BOOL _IsUseHeadphonePlugInMonThread(void)
{
//#ifdef CC_DUAL_AUD_DEC_SUPPORT   
#if 0 
    return FALSE;
#else
    return (DRVCUST_OptGet(eAudioHeadphonePlugInGpio) != 0xffffffff) ? TRUE : FALSE;
#endif
}

static BOOL _IsUseDcDetectionMonThread(void)
{
    return (DRVCUST_OptGet(eAudioDcDetectGpio) != 0xffffffff) ? TRUE : FALSE;
}

#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
static BOOL _IsUseAmpInitGpioDetectionMonThread(void)
{
	return (DRVCUST_OptGet(eAudioAmpInitDetectGpio) != 0xffffffff) ? TRUE : FALSE;
}

static BOOL _IsUseAmpInitRegDetectionMonThread(void)
{
	return (DRVCUST_OptGet(eAudioAmpInitDetectRegEnable) != FALSE) ? TRUE : FALSE;
}
#endif

//-------------------------------------------------------------------------
/** _HeadphoneDetect
 *  @param  i4Gpio  card detect GPIO function.
 *  @param  fgStatus  1 = GPIO high, 0 = GPIO low.
 *  @return  void
 */
//-------------------------------------------------------------------------
static void _HeadphoneDetect(INT32 i4Gpio, BOOL fgStatus)
{
    UNUSED(fgStatus);
    VERIFY(((INT32)DRVCUST_OptGet(eAudioHeadphonePlugInGpio)) == i4Gpio);
    // Release semaphore
    LOG(8, "**** Detect headphone plug in/out \n");
    VERIFY(x_sema_unlock(_hSemaHpPlugInWait) == OSR_OK);
}

#ifdef CC_AUD_HP_POLLING
typedef enum
{
    NOT_USE = -1,
    PLUG_IN = 0,
    PLUG_OUT = 1,

} PLUGINSTATUS;
#endif
static void _AudHeadphonePlugInMonThread(void* pvArg)
{
    static UINT32 u4HpGpioNum = 0;
    static AUD_GPIO_POLARITY_T eHpDetectPolarity;
#ifdef CC_AUD_HP_POLLING
    static PLUGINSTATUS bPlugStatus = NOT_USE;
#endif
    static BOOL fLineOutMute, fLineOutDacUsage;
    static AudDecNfyFct pfAudDecNfy = NULL;    
    UINT8 u1DecId = AUD_DEC_MAIN;
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    INT32 i4Temp= 0;
#endif
    
    UNUSED(pvArg);

    // Load configuration 
    u4HpGpioNum = DRVCUST_OptGet(eAudioHeadphonePlugInGpio);
    eHpDetectPolarity = (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioHeadPhonePlugInPolarity);
    fLineOutMute = (BOOL)DRVCUST_OptGet(eAudioHeadPhonePlugInLineOutMute);
    fLineOutDacUsage = (BOOL)DRVCUST_OptGet(eAudioLineOutUseInterDac);
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882) ///Init first for ext_int GPI use(AUDIO_HP_PLUG_IN_GPIO).
    i4Temp = 0; /// Set 0 for GPI (1:GPO).
    GPIO_Enable(((INT32)u4HpGpioNum), &i4Temp);
    ///i4Temp = ((AUD_GPIO_POLARITY_T)GPIO_Input((INT32)u4HpGpioNum));
#endif

#ifndef CC_AUD_HP_POLLING
    // Register gpio interrupt handler
    VERIFY(GPIO_Reg((INT32)u4HpGpioNum, GPIO_TYPE_INTR_BOTH, _HeadphoneDetect) >= 0);
#else
	UNUSED(_HeadphoneDetect);
#endif

    //Get notify function
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy) == TRUE);
	//ADAC_GpioCodecMute(FALSE);
	ADAC_GpioAmpEnable(TRUE);

    //x_thread_delay(300);
    // Wait semaphore in while 1
    while (1)
    {
#ifndef CC_AUD_HP_POLLING
        VERIFY(x_sema_lock(_hSemaHpPlugInWait, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif

        if (((AUD_GPIO_POLARITY_T)GPIO_Input((INT32)u4HpGpioNum)) == eHpDetectPolarity)
        {
            //Headphone plug in
            #ifdef HP_DETECT_ANDROID
            isPlugin = TRUE;
			#endif
#ifndef CC_AUD_HP_POLLING
            ADAC_HpDetectSpkEnable(FALSE);
            if (fLineOutDacUsage)
            {
                ADAC_HpDetectDacMute(fLineOutMute);
            }
            else
            {
                ADAC_HpDetectCodecMute(fLineOutMute);
            }
            
            if (pfAudDecNfy != NULL)
            {
                pfAudDecNfy((void *)AUD_NFY_HP, u1DecId, (AUD_COND_NFY_T)AUD_HP_COND_HEADPHONE_COND_PLUGED, 0, 0);
            }
            AUD_SetHeadphoneCond(u1DecId, AUD_HP_COND_HEADPHONE_COND_PLUGED);
#else
            if (bPlugStatus != PLUG_IN)
            {
                ADAC_HpDetectSpkEnable(FALSE);
                if (fLineOutDacUsage)
                {
                    ADAC_HpDetectDacMute(fLineOutMute);
                }
                else
                {
                    ADAC_HpDetectCodecMute(fLineOutMute);
                }
            
                if (pfAudDecNfy != NULL)
                {
                    pfAudDecNfy((void *)AUD_NFY_HP, u1DecId, (AUD_COND_NFY_T)AUD_HP_COND_HEADPHONE_COND_PLUGED, 0, 0);
                }
                AUD_SetHeadphoneCond(u1DecId, AUD_HP_COND_HEADPHONE_COND_PLUGED);
                bPlugStatus = PLUG_IN;
            }
            else
            {
                x_thread_delay(100);
                continue;
            }
#endif
        }
        else
        {
            //Headphone plug out
            #ifdef HP_DETECT_ANDROID
            isPlugin = FALSE;
			#endif
#ifndef CC_AUD_HP_POLLING       
            if (pfAudDecNfy != NULL)
            {
                pfAudDecNfy((void *)AUD_NFY_HP, u1DecId, (AUD_COND_NFY_T)AUD_HP_COND_HEADPHONE_COND_UNPLUGED, 0, 0);
            }        
            AUD_SetHeadphoneCond(u1DecId, AUD_HP_COND_HEADPHONE_COND_UNPLUGED);

            // Add delay for applying sound effect (ex. volume)
            x_thread_delay(80);
            
            ADAC_HpDetectSpkEnable(TRUE);
            if (fLineOutDacUsage)
            {
                ADAC_HpDetectDacMute(FALSE);
            }
            else
            {
                ADAC_HpDetectCodecMute(FALSE);
            }
#else
            if (bPlugStatus != PLUG_OUT)
            {
                if (pfAudDecNfy != NULL)
                {
                    pfAudDecNfy((void *)AUD_NFY_HP, u1DecId, (AUD_COND_NFY_T)AUD_HP_COND_HEADPHONE_COND_UNPLUGED, 0, 0);
                }        
                AUD_SetHeadphoneCond(u1DecId, AUD_HP_COND_HEADPHONE_COND_UNPLUGED);

                // Add delay for applying sound effect (ex. volume)
                x_thread_delay(80);
            
                ADAC_HpDetectSpkEnable(TRUE);
                if (fLineOutDacUsage)
                {
                    ADAC_HpDetectDacMute(FALSE);
                }
                else
                {
                    ADAC_HpDetectCodecMute(FALSE);
                }
                bPlugStatus = PLUG_OUT;
            }   
            else
            {
                x_thread_delay(100);
                continue;
            }
#endif
        }
#ifdef CC_AUD_HP_POLLING
        x_thread_delay(100);
#endif
    }
}

#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
//-------------------------------------------------------------------------
/** _AmpDetect
 *  @param  i4Gpio  card detect GPIO function.
 *  @param  fgStatus  1 = GPIO high, 0 = GPIO low.
 *  @return  void
 */
//-------------------------------------------------------------------------
static void _AmpDetect(INT32 i4Gpio, BOOL fgStatus)
{
    UNUSED(fgStatus);
    VERIFY(((INT32)DRVCUST_OptGet(eAudioAmpInitDetectGpio)) == i4Gpio);
    // Release semaphore
    LOG(8, "**** Detect Amp init \n");
    VERIFY(x_sema_unlock(_hSemaAmpInitGpioDetectionWait) == OSR_OK);
}

static void _AudAmpInitGpioDetectionMonThread(void* pvArg)
{
    static UINT32 u4AmpGpioNum = 0;
    static AUD_GPIO_POLARITY_T eAmpDetectPolarity;
    
    UNUSED(pvArg);

    // Load configuration 
    u4AmpGpioNum = DRVCUST_OptGet(eAudioAmpInitDetectGpio);
    eAmpDetectPolarity = (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioAmpInitDetectPolarity);

    // Register gpio interrupt handler
    VERIFY(GPIO_Reg((INT32)u4AmpGpioNum, GPIO_TYPE_INTR_BOTH, _AmpDetect) >= 0);

    // Wait semaphore in while 1
    while (1)
    {
        VERIFY(x_sema_lock(_hSemaAmpInitGpioDetectionWait, X_SEMA_OPTION_WAIT) == OSR_OK);
        
        if (((AUD_GPIO_POLARITY_T)GPIO_Input((INT32)u4AmpGpioNum)) == eAmpDetectPolarity)
        {
			ADAC_AmpInit();
        }
        else
        {

        }
    }
}
#endif

//-------------------------------------------------------------------------
/** _DcDetect
 *  @param  i4Gpio  card detect GPIO function.
 *  @param  fgStatus  1 = GPIO high, 0 = GPIO low.
 *  @return  void
 */
//-------------------------------------------------------------------------
static void _DcDetect(INT32 i4Gpio, BOOL fgStatus)
{
    UNUSED(fgStatus);
    VERIFY(((INT32)DRVCUST_OptGet(eAudioDcDetectGpio)) == i4Gpio);
    // Release semaphore
    LOG(8, "**** Dc detection \n");
    VERIFY(x_sema_unlock(_hSemaDcDetectionWait) == OSR_OK);
}

static void _AudDcDetectionMonThread(void* pvArg)
{
    static UINT32 u4DcDetectGpioNum = 0;
    static AUD_GPIO_POLARITY_T eDcDetectPolarity;
    
    UNUSED(pvArg);

    // Load configuration 
    u4DcDetectGpioNum = DRVCUST_OptGet(eAudioDcDetectGpio);
    eDcDetectPolarity = (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioDcDetectPolarity);
   
    // Register gpio interrupt handler
    VERIFY(GPIO_Reg((INT32)u4DcDetectGpioNum, GPIO_TYPE_INTR_BOTH, _DcDetect) >= 0);

    // Wait semaphore in while 1
    while (1)
    {
        VERIFY(x_sema_lock(_hSemaDcDetectionWait, X_SEMA_OPTION_WAIT) == OSR_OK);
        if (((AUD_GPIO_POLARITY_T)GPIO_Input((INT32)u4DcDetectGpioNum)) == eDcDetectPolarity)
        {
            ADAC_DcDetectSpkEnable(FALSE);
        }
        else
        {
            ADAC_DcDetectSpkEnable(TRUE);
        }
    }
}

#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
// -----------------------------------------------------------------------

static void _AudAmpInitRegDetectionMonThread(void* pvArg)
{
    static UINT32 u4AmpInitDetectLen = 0;
    static AMP_INIT_DETECT_T * eAmpInitDetectTable = NULL;
	static UINT32 u4AmpInitDetectDelayMs = 0;
	UINT8 i, u1MaxId;
	UINT8 u1DetectTable[32];

#define DETECT_INIT	0
#define DETECT_OK	1
#define DETECT_FAIL 2

    UNUSED(pvArg);

    // Load configuration 
    u4AmpInitDetectLen = DRVCUST_OptGet(eAudioAmpInitDetectLen);
    eAmpInitDetectTable = (AMP_INIT_DETECT_T *)DRVCUST_OptGet(eAudioAmpInitDetectTable);
	u4AmpInitDetectDelayMs = DRVCUST_OptGet(eAudioAmpInitDetectDelayMs);

    // Verify
    VERIFY(u4AmpInitDetectLen > 0 && eAmpInitDetectTable != NULL);

	// Check length, must be less than 256
	if (u4AmpInitDetectLen > 0xff)
		u4AmpInitDetectLen = 0xff;

	// Check delay ms, must be more than 100
	if (u4AmpInitDetectDelayMs < 100)
		u4AmpInitDetectDelayMs = 100;
	
	// Search max condition id
	u1MaxId = 0;
	for (i = 0; i < u4AmpInitDetectLen; i++)
	{
		if (eAmpInitDetectTable[i].u1ConditionId < sizeof(u1DetectTable) 
			&& eAmpInitDetectTable[i].u1ConditionId > u1MaxId)
			u1MaxId = eAmpInitDetectTable[i].u1ConditionId;
	}

	// Delay 2s
	x_thread_delay(2000);
	
    // while 1
    while (1)
    {
        x_thread_delay(u4AmpInitDetectDelayMs);

        if (!AUD_DspGetRegDetection())
            continue;

		// Init detect table
		for (i = 0; i <= u1MaxId; i++)
		{
			u1DetectTable[i] = DETECT_INIT;
		}
		
		// Detect each register
        for (i = 0; i < u4AmpInitDetectLen; i++)
        {
			if (eAmpInitDetectTable[i].u1ConditionId >= sizeof(u1DetectTable))
				continue;
			
			if ((ADAC_AmpRegRead(eAmpInitDetectTable[i].u1Addr) & eAmpInitDetectTable[i].u1DataMask) ==
					eAmpInitDetectTable[i].u1Data)
			{
				if (u1DetectTable[eAmpInitDetectTable[i].u1ConditionId] == DETECT_INIT)
					u1DetectTable[eAmpInitDetectTable[i].u1ConditionId] = DETECT_OK;
			}
			else
			{
				u1DetectTable[eAmpInitDetectTable[i].u1ConditionId] = DETECT_FAIL;
			}
		}

		for (i = 0; i <= u1MaxId; i++)
		{
			// If detect ok, init amp
			if (u1DetectTable[i] == DETECT_OK)
			{
				ADAC_AmpInit();
				break;
			}
		}
    }
}
#endif

void AUD_DetectInit(void)
{
    static BOOL fgInit = FALSE;
    UINT8 u1DecId = AUD_DEC_MAIN;

    if (!fgInit)
    {    
        if (_IsUseHeadphonePlugInMonThread())
        {
            VERIFY(x_sema_create(&_hSemaHpPlugInWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
            // Create headphone plug in monitor thread
            VERIFY(x_thread_create(&_hAudHeadphonePlugInMonThread, AUD_HEADPHONE_PLUG_IN_MON_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
                   _AudHeadphonePlugInMonThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
        }   

        if (_IsUseDcDetectionMonThread())
        {
            VERIFY(x_sema_create(&_hSemaDcDetectionWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
            // Create Dc detection plug in monitor thread
            VERIFY(x_thread_create(&_hAudDcDetectionMonThread, AUD_DC_DETECTION_MON_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
                   _AudDcDetectionMonThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
        }   

#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
        if (_IsUseAmpInitGpioDetectionMonThread())
		{
			VERIFY(x_sema_create(&_hSemaAmpInitGpioDetectionWait, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
			// Create Amp Init gpio detection monitor thread
			VERIFY(x_thread_create(&_hAudAmpInitGpioDetectionMonThread, AUD_AMP_INIT_GPIO_DETECTION_MON_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
				  _AudAmpInitGpioDetectionMonThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
		}

		if (_IsUseAmpInitRegDetectionMonThread())
		{
			// Create Amp Init reg detection monitor thread
			VERIFY(x_thread_create(&_hAudAmpInitRegDetectionMonThread, AUD_AMP_INIT_REG_DETECTION_MON_THREAD_NAME, AUD_DRV_THREAD_STACK_SIZE, AUD_DRV_THREAD_PRIORITY,
				  _AudAmpInitRegDetectionMonThread, sizeof(UINT8), (void *)&u1DecId) == OSR_OK);
		}
#endif
        fgInit = TRUE;
    }
#ifdef CC_FAST_INIT
    else
    {
        UINT32 u4GpioNum = 0;
            
        if (_IsUseHeadphonePlugInMonThread())
        {
            u4GpioNum = DRVCUST_OptGet(eAudioHeadphonePlugInGpio);
            //VERIFY(GPIO_INTEN((INT32)u4GpioNum, GPIO_TYPE_INTR_BOTH) >= 0); // mark for mt5396.compile.fail
            VERIFY(x_sema_unlock(_hSemaHpPlugInWait) == OSR_OK);
        }

        if (_IsUseDcDetectionMonThread())
        {
            u4GpioNum = DRVCUST_OptGet(eAudioDcDetectGpio);
            //VERIFY(GPIO_INTEN((INT32)u4GpioNum, GPIO_TYPE_INTR_BOTH) >= 0);  // mark for mt5396.compile.fail
            VERIFY(x_sema_unlock(_hSemaDcDetectionWait) == OSR_OK);
        }
        
#ifdef CC_AUD_SUPPORT_AMP_INIT_DETECT_FUNCTION
        if (_IsUseAmpInitGpioDetectionMonThread())
		{
			u4GpioNum = DRVCUST_OptGet(eAudioAmpInitDetectGpio);
			//VERIFY(GPIO_INTEN((INT32)u4GpioNum, GPIO_TYPE_INTR_BOTH) >= 0);  
			VERIFY(x_sema_unlock(_hSemaAmpInitGpioDetectionWait) == OSR_OK);
		}
#endif
    }
#endif    
}

BOOL Aud_IsHeadphoneExist(void)
{
	UINT32 u4HpGpioNum = 0;
    AUD_GPIO_POLARITY_T eHpDetectPolarity;

	u4HpGpioNum = DRVCUST_OptGet(eAudioHeadphonePlugInGpio);
    eHpDetectPolarity = (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioHeadPhonePlugInPolarity);

	if (((AUD_GPIO_POLARITY_T)GPIO_Input((INT32)u4HpGpioNum)) == eHpDetectPolarity)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
