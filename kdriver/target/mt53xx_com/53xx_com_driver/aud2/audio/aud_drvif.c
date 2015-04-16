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
 * $Date: 2015/04/16 $
 * $RCSfile: aud_drvif.c,v $
 * $Revision: #9 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_drvif.c
 *  Brief of file aud_drvif.c.
 *  Details of file aud_drvif.c (optional).
 */

#define AUD_EXTERN_FILE

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

// KERNEL
#include "x_util.h"
#include "x_typedef.h"
#include "aud_if.h"
#include "aud_cfg.h"
#include "aud_drv.h"
#include "aud_drvif.h"
#include "aud_notify.h"
#include "aud_debug.h"
#include "dsp_intf.h"
#include "x_os.h"
#include "x_assert.h"
#include "dsp_common.h"
#include "aud_hw.h"
#include "aud_dsp_cfg.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define FIFO_FULL_THRESHOLD     (UINT32)(5*1024)
#define FIFO_EMPTY_THRESHOLD    (UINT32)(2000) 
#define FIFO_EMPTY_RETRY_THRESHOLD (UINT32)(5)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern UINT32 u4ReadDspSram(UINT8 u1DspId, UINT32 u4Addr);
BOOL DSP_GetAoutEnable(UINT8 u1DspId, UINT8 u1DecId);

#ifdef CC_AUD_SKYPE_ZERO_DELAY
extern bool _fgSkypeMode;
extern BOOL FeederGetSkypeSourceFlag(void);
#endif
extern BOOL  _fgMMPlusAD[AUD_DEC_MAX];
//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT32 u4DataSizeLast[AUD_DEC_MAX] = {0,0,0,0};
static UINT32 u4RetryCnt[AUD_DEC_MAX] = {0,0,0,0};
static UINT32 u4FrmCntLast[AUD_DEC_MAX]= {0,0,0,0};


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
BOOL _fgFMRadioDetectionResult;


//-----------------------------------------------------------------------------
//  _SmpInit
//
/** Brief of _SmpInit.
 *  Details of _SmpInit (optional).
 *
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

BOOL AUD_IsFifoEmpty(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT32 u4Rp;
    UINT32 u4Wp;
    UINT32 u4DataSize;
    UINT32 u4FrameSize;
    UINT32 u4FrmCnt;
    UINT32 u4DataThreshold = FIFO_EMPTY_THRESHOLD;
    UINT32 u4RetryThres = AUD_GetEmptyRetryThres();	
    BOOL   fgRet;
    AUD_DRV_STATE_T eDrvState;
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_FMT_T eAudDecFormat;

    AUD_DSP_ID_VALIDATE(u1DspId);
    AUD_DEC_ID_VALIDATE(u1DecId);

    eDrvState = AUD_DRVGetAudioState(u1DspId, u1DecId);
    if(eDrvState != AUD_ON_PLAY )
     {
            fgRet = FALSE;
            return fgRet;
     }


    AUD_GetRWPtr(u1DspId, u1DecId, &u4Rp, &u4Wp, &u4DataSize);
    u4FrmCnt = u4ReadD2RInfo_AputBank(u1DspId, u1DecId);
    AUD_DRVGetDecodeType(u1DspId, u1DecId, &eAudDecStreamFrom, &eAudDecFormat);
    if (eAudDecFormat == AUD_FMT_MPEG)
    {
        u4FrameSize = DSP_GetMpegFrameSize(u1DecId);
        if (u4FrameSize != 0)
        { 
            u4DataThreshold = (u4FrameSize * 5) / 2;  // Set to 2.5 frame size
        }    
    }
    LOG(9, "Dsp(%d) Dec (%d) remaining data size = 0x%08x\n", u1DspId, u1DecId, u4DataSize);

#if 1  //zhaoyuan 20111208 add this case.
    if ( u1DecId == AUD_DEC_MAIN ) 
        u4DataThreshold = u4ReadDspSram(u1DspId, SRAM_CHK_FRAME_SIZE_BACKUP);
	else if ( u1DecId == AUD_DEC_AUX )
        u4DataThreshold = u4ReadDspSram(u1DspId, SRAM_CHK_FRAME_SIZE_BACKUP_DEC2);
	else if ( u1DecId == AUD_DEC_THIRD )
        u4DataThreshold = u4ReadDspSram(u1DspId, SRAM_CHK_FRAME_SIZE_BACKUP_DEC3);
    else if ( u1DecId == AUD_DEC_4TH )
        u4DataThreshold = u4ReadDspSram(u1DspId, SRAM_CHK_FRAME_SIZE_BACKUP_DEC4);
    // mpeg1/23 : 1 frame at least 384 samples ..
    // ac3: 1 frame 1536 samples
    // aac: 1 frame 1024 samples.
    // heaac: 1 frame 2048 samples
    // dts: 1frame 512 samples
    // cook: 1 frame at least 256 samples
    // adpcm: 
    // flac: 
    // vorbis:
    // wma:
    
    // here we must make sure there is 4 pcm banks left to let DSP do stop fade out.
    // now , we let threshold  be 3 frame size.. maybe future you can decide this value by decoder type..
    if (u4DataSize <= u4DataThreshold*3)
    {
        LOG(5, "FIFO(%d) is empty. Data (%d).Chkframesize (0x%x) \n", u1DecId, u4DataSize,u4DataThreshold); 
		return TRUE;
    }
	// if goes into  this case.. maybe decoder's behavier issue.. data is enough but can't decode..need to debug!!!
	else if ((u4DataSizeLast[u1DecId] == u4DataSize) && (u4FrmCntLast[u1DecId] == u4FrmCnt) && 
                    DSP_GetAoutEnable(u1DspId, u1DecId) && AUD_IsMMAoutEnable(u1DecId))
	{
		u4RetryCnt[u1DecId]++;
		if (u4RetryCnt[u1DecId] >= u4RetryThres)
		{
			fgRet = TRUE;
		}
		else
		{
			fgRet = FALSE;
		}
	}
	else
	{
		u4RetryCnt[u1DecId] = 0;
		fgRet = FALSE;
	}	

	u4DataSizeLast[u1DecId] = u4DataSize;
	u4FrmCntLast[u1DecId] = u4FrmCnt;

	if (fgRet)
    {
        LOG(2, "Auto Stop For FIFO(%d) is empty. Data (%d),  AputBank (%d), Retry (%d)\n", u1DecId, u4DataSizeLast[u1DecId], u4FrmCntLast[u1DecId], u4RetryCnt[u1DecId]);    
        u4DataSizeLast[u1DecId] = 0;
        u4FrmCntLast[u1DecId] = 0;
        u4RetryCnt[u1DecId] = 0;
    }
	
	return fgRet;

#else

    if (eDrvState == AUD_IDLE)
    {
        fgRet = TRUE;
    }
    else if (eDrvState == AUD_ON_PAUSE)
    {
        u4RetryCnt = 0;
        fgRet = FALSE;
    }
    else if ((u4DataSize < u4DataThreshold) && (u4DataSizeLast == u4DataSize) && (u4FrmCntLast == u4FrmCnt))
    {
        u4RetryCnt++;
        if (u4RetryCnt >= u4RetryThres)
        {
            fgRet = TRUE;
        }
        else
        {
            fgRet = FALSE;
        }
    }
    else if ((u4DataSizeLast == u4DataSize) && (u4FrmCntLast == u4FrmCnt))
    {
        u4RetryCnt++;
        if (u4RetryCnt >= u4RetryThres)
        {
            fgRet = TRUE;
        }
        else
        {
            fgRet = FALSE;
        }
    }
    else
    {
        u4RetryCnt = 0;
        fgRet = FALSE;
    }   

    x_thread_delay(10);  
    u4DataSizeLast = u4DataSize;
    u4FrmCntLast = u4FrmCnt;
    
    if (fgRet)
    {
        LOG(5, "FIFO(%d) is empty. Data (%d),  AputBank (%d), Retry (%d)\n", u1DecId, u4DataSizeLast, u4FrmCntLast, u4RetryCnt);    
        u4DataSizeLast = 0;
        u4FrmCntLast = 0;
        u4RetryCnt = 0;
    }

    return fgRet;
#endif

}

BOOL AUD_IsFifoFull(UINT8 u1DspId, UINT8 u1DecId)
{
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;
    UINT32 u4FifoSZ;
    UINT32 u4Rp;
    UINT32 u4Wp;
    UINT32 u4DataSZ;

    AUD_DEC_ID_VALIDATE(u1DecId);

    VERIFY(AUD_GetAudFifoB(u1DspId, u1DecId, &u4FifoStart, &u4FifoEnd) == AUD_OK);
    u4FifoSZ = u4FifoEnd - u4FifoStart;
    AUD_GetRWPtr(u1DspId, u1DecId, &u4Rp, &u4Wp, &u4DataSZ);

    if ((u4DataSZ + FIFO_FULL_THRESHOLD) > u4FifoSZ)
    {
        return TRUE;
    }   
    else
    {
        return FALSE;
    }   
}


INT32 AUD_GetAudFifo(UINT8 u1DecId, UINT32 * pu4FifoStart, UINT32 * pu4FifoEnd)
{
    return _AUD_GetAudFifo(AUD_DSP0, u1DecId, pu4FifoStart, pu4FifoEnd);
}
INT32 AUD_GetAudFifoB(UINT8 u1DspId, UINT8 u1DecId, UINT32 * pu4FifoStart, UINT32 * pu4FifoEnd)
{
    return _AUD_GetAudFifo(u1DspId, u1DecId, pu4FifoStart, pu4FifoEnd);
}

void AUD_HdmiModeMute(UINT8 u1DecId, BOOL fgMute)
{
    AUD_DspHdmiModeMuteEnable(u1DecId, fgMute);
}

extern void _AUD_UserSetDecInputMute(UINT8 u1DecId, BOOL fgMute);
void AUD_HdmiDrvModeMute(UINT8 u1DecId, BOOL fgMute)
{
    //AUD_DspHdmiChangeFormatMuteEnable(u1DecId, fgMute);
   //use input mute here
    //_AUD_UserSetDecInputMute(u1DecId, fgMute); //no need for LG A5LR
}

void AUD_GST_SetDecInputMute(UINT8 u1DecId, BOOL fgMute)
{
	_AUD_UserSetDecInputMute(u1DecId, fgMute);
}

#ifdef CC_ENABLE_AOMX
extern BOOL skip_create_feed_stream;
#endif
void AUD_AtvAvInitialSync(BOOL fgStable)
{
#ifdef CC_ENABLE_AOMX
    if (skip_create_feed_stream)
    {
        //Printf("!!!! Skip AUD_AtvAvInitialSync !!!!\n");
        return;
    }
#endif

    _AUD_AtvAvInitialSync(fgStable);
}

#ifdef CC_ENABLE_AOMX
extern BOOL skip_create_feed_stream;
#endif
void AUD_AtvAvInitialSync1(BOOL fgStable)
{
#ifdef CC_ENABLE_AOMX
    if (skip_create_feed_stream)
    {
        //Printf("!!!! Skip AUD_AtvAvInitialSync1 !!!!\n");
        return;
    }
#endif

    _AUD_AtvAvInitialSync1(fgStable);
}

void AUD_WaitDspFlush(UINT8 u1DecId)
{   
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
#ifndef CC_MT5391_AUD_3_DECODER 
    if (u1DecId == AUD_DEC_AUX)
    {
        return ;
    }
#endif

    if (!AUD_IsDecoderStop(AUD_DSP0, u1DecId))
    {
        LOG(5, "AUD_WaitDspFlush condition\n");
        //clear AoutWait bit if issue stop, to prevent DSP hang!
#ifndef CC_AUD_SKYPE_SUPPORT        
        if ((_AudGetStrSource(AUD_DEC_MAIN)== AUD_STREAM_FROM_MULTI_MEDIA)||
            ((_AudGetStrSource(AUD_DEC_MAIN)== AUD_STREAM_FROM_DIGITAL_TUNER)&& _fgMMPlusAD[u1DecId]==TRUE )
           #ifdef CC_ENABLE_AOMX
            || (_AudGetStrSource(AUD_DEC_MAIN)== AUD_STREAM_FROM_GST)
           #endif
            )
        {
            //AUD_DrvMMAoutEnable(AUD_DEC_MAIN, TRUE);
            AUD_DrvMMAoutEnable_Flag(FALSE);
            AUD_DrvMMAoutReady_Flag(FALSE);
        }       
#else
        if ((_AudGetStrSource(u1DecId)== AUD_STREAM_FROM_MULTI_MEDIA)|| 
            ((_AudGetStrSource(u1DecId)== AUD_STREAM_FROM_DIGITAL_TUNER)&& _fgMMPlusAD[u1DecId]==TRUE )
           #ifdef CC_ENABLE_AOMX
            || (_AudGetStrSource(u1DecId)== AUD_STREAM_FROM_GST)
           #endif
            )
        {
            //AUD_DrvMMAoutEnable(u1DecId, TRUE);
            AUD_DrvMMAoutEnable_Flag(u1DecId, FALSE);
            AUD_DrvMMAoutReady_Flag(u1DecId, FALSE);
        }    
#endif
        if ((u1DecId == AUD_DEC_AUX) ||
           ((u1DecId == AUD_DEC_THIRD) && _IsTriOnlyDecMode()))
        
        {
            AUD_DRVGetStreamFrom(AUD_DSP0, AUD_DEC_MAIN, &eAudDecStreamFrom);
            if ((eAudDecStreamFrom == AUD_STREAM_FROM_ANALOG_TUNER) ||
                (eAudDecStreamFrom == AUD_STREAM_FROM_DIGITAL_TUNER))
            {
#ifndef CC_AUD_FAST_STOP_BY_HW_GAIN
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
                UNUSED(AUD_DSPCmdStop(AUD_DSP0, AUD_DEC_MAIN));
                while (!DSP_RealStop(AUD_DSP0, AUD_DEC_MAIN))
                {
                    x_thread_delay(10);
                }
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
#else
                AudAoutGainCtrl(AUD_DEC_MAIN, 0, 0,AOUT_VOL_SETP_4DB);
                x_thread_delay(80/AOUT_VOL_SETP_4DB);     //wait fadeout done, must be fadeout bank length
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
                UNUSED(AUD_DSPCmdStop(AUD_DSP0, AUD_DEC_MAIN));
                UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
#endif
            }
        }
#ifndef CC_AUD_FAST_STOP_BY_HW_GAIN
        UNUSED(_AUD_IgnoreDecNotify(u1DecId, TRUE));
        UNUSED(AUD_DSPCmdStop(AUD_DSP0, u1DecId));
        while (!DSP_RealStop(AUD_DSP0, u1DecId))
        {
            x_thread_delay(10);
        }
        UNUSED(_AUD_IgnoreDecNotify(u1DecId, FALSE));
#else
        AudAoutGainCtrl(u1DecId, 0, 0,AOUT_VOL_SETP_4DB);
        x_thread_delay(80/AOUT_VOL_SETP_4DB);     //wait fadeout done, must be fadeout bank length
        UNUSED(_AUD_IgnoreDecNotify(u1DecId, TRUE));
        UNUSED(AUD_DSPCmdStop(AUD_DSP0, u1DecId));
        UNUSED(_AUD_IgnoreDecNotify(u1DecId, FALSE));
        
#endif
    }
    else
    {
        LOG(5, "Invalid AUD_WaitDspFlush condition\n");
    }
}

BOOL _AudFMRadioDetection(void)
{

    if (AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_MAIN))
    {
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
        UNUSED(AUD_DSPCmdStop(AUD_DSP0, AUD_DEC_MAIN));
        while (!DSP_RealStop(AUD_DSP0, AUD_DEC_MAIN))
        {
            x_thread_delay(10);
        }
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
    }
    x_thread_delay(30);
    if (AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_AUX))
    {
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));
        UNUSED(AUD_DSPCmdStop(AUD_DSP0, AUD_DEC_AUX));
        while (!DSP_RealStop(AUD_DSP0, AUD_DEC_AUX))
        {
            x_thread_delay(10);
        }
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));
    }
    _fgFMRadioDetectionResult = 0;
    UNUSED(AUD_SetDecType(AUD_DSP0, AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_FMRDO_DET));
    UNUSED(AUD_DSPCmdPlay(AUD_DSP0, AUD_DEC_MAIN));
    x_thread_delay(400);
    return _fgFMRadioDetectionResult;
    
}

BOOL _AudPlayFMRadio(void)
{
    if (AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_MAIN))
    {
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, TRUE));
        UNUSED(AUD_DSPCmdStop(AUD_DSP0, AUD_DEC_MAIN));
        while (!DSP_RealStop(AUD_DSP0, AUD_DEC_MAIN))
        {
            x_thread_delay(10);
        }
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_MAIN, FALSE));
    }
    if (AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_AUX))
    {
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));
        UNUSED(AUD_DSPCmdStop(AUD_DSP0, AUD_DEC_AUX));
        while (!DSP_RealStop(AUD_DSP0, AUD_DEC_AUX))
        {
            x_thread_delay(10);
        }
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));
    }
    
    UNUSED(AUD_SetDecType(AUD_DSP0, AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_FMRDO));
    x_thread_delay(20);
    UNUSED(AUD_DSPCmdPlay(AUD_DSP0, AUD_DEC_MAIN));
    return TRUE;

}

#ifdef CC_AUD_BITRATE_CHG_NOTIFY
BOOL AUD_SetBitRateChangedPts(UINT32 u4Pts)
{
    return _AUD_SetBitRateChangedPts(u4Pts);
}
#endif

#ifndef CC_MT5391_AUD_3_DECODER 
void Aud_SetTvSysMask(UINT8 u1DecId, UINT32 u4Mask)
{
    _AudSetTvSysMask(u1DecId,  u4Mask);
}

void AUD_TriggerDspPlay(UINT8 u1DecId)
{   
    AUD_DEC_STREAM_FROM_T  rAudDecStreamFrom;
    AUD_FMT_T              eAudDecFormat;

#ifndef CC_MT5391_AUD_3_DECODER 
    if (u1DecId == AUD_DEC_AUX)
    {
        return ;
    }
#endif

    // Do this when EURO dual decoder mode with AUX decoder in ATV
    //first need to do detection to get audio system then to set audio system information      
    UNUSED(AUD_DRVGetDecodeType(AUD_DSP0, AUD_DEC_MAIN, &rAudDecStreamFrom, &eAudDecFormat));
    if ((eAudDecFormat != AUD_FMT_DETECTOR) && (!AUD_IsDecoderPlay(AUD_DSP0, AUD_DEC_AUX)))
    {
        AUD_FMT_T eAudFmt;
        
#ifdef CC_AUD_DDI
        eAudFmt = _AudAtvFmtDetection(AUD_DEC_AUX);
#else
        eAudFmt = _AudAtvFmtDetection();
#endif
        UNUSED(AUD_SetDecType(AUD_DSP0, AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, eAudFmt)); 
            
        //Play AUX decoder 	  
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, TRUE));
        UNUSED(AUD_DSPCmdPlay(AUD_DSP0, AUD_DEC_AUX));
        UNUSED(_AUD_IgnoreDecNotify(AUD_DEC_AUX, FALSE));   
    }
}
#endif

void AUD_AtunerSigStableNotify(BOOL fgSatble)
{
    UNUSED(fgSatble);

	//if (AUD_GetTargetCountry() == COUNTRY_EU)
	if(_IsDualDecMode())
	{
        if (fgSatble)
        {
            AUD_AtunerSigStable();
        }
    }
}

void AUD_MMAoutEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DrvMMAoutEnable(u1DecId, fgEnable);
}

void AUD_MMSetSwdmxSrcId(UINT8 u1DecId, UINT8 u1SwdmxSrcId)
{
    AUD_SET_SWDMXSrcId(u1DecId, u1SwdmxSrcId);
}

#if defined(CC_ENABLE_AV_SYNC) || defined(CC_AUD_ENABLE_PLAY_MUTE_CONTROL)
void AUD_VdpModeChange(void)
{
    UNUSED(AUD_VdpModeChange);
    _AudSrmStableCb(0,0,0, AUD_VIDEO_IS_UNSTABLE);
}

void AUD_VdpModeChangeDone(void)
{
    UNUSED(AUD_VdpModeChangeDone);
    _AudSrmStableCb(0,0,0, AUD_VIDEO_IS_STABLE);
}
#endif

void AUD_ClearEOSFlag(UINT8 u1DecId)
{
    _AudClearEOSFlag(u1DecId);
}


UINT32 AUD_GetEmptyRetryThres(void)
{
	return 	FIFO_EMPTY_RETRY_THRESHOLD;
}

static DATA_ENDIAN_T _gDTSEnInfo[AUD_DEC_MAX] = {DEC_BIG_ENDIAN, DEC_BIG_ENDIAN, DEC_BIG_ENDIAN};

void AUD_SetDTSInfo (UINT8 u1DecId, DATA_ENDIAN_T endian)
{
    UINT32 u4Mode = 0; //bit0: 1: DTS CD, 0: others, bit

    if(endian != _gDTSEnInfo[u1DecId])
    {
        _gDTSEnInfo[u1DecId] = endian;
        if (AUD_GetSpdifRawDec() == u1DecId)
        {
    		if (AUD_GetDTSInfo(u1DecId) == DEC_LITTLE_ENDIAN)
    		{
    			u4Mode = 0x1;			  
    		}
    		else
    		{
    			u4Mode = 0x0;	
    		}
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_MODE, (UINT32 *) &u4Mode, 1); 
        }
        LOG (2, "AUD_SetDTSInfo(%d): %d\n", u1DecId, _gDTSEnInfo[u1DecId]);
    }
}

DATA_ENDIAN_T AUD_GetDTSInfo (UINT8 u1DecId)
{
    return _gDTSEnInfo[u1DecId];
}

static UINT32 _gu4DTSFrameSize[AUD_DEC_MAX] = {512, 512, 512};

void AUD_SetDTSFrameSize(UINT8 u1DecId, UINT32 u4Size)
{
    UINT32 u4Reg0;
    SPDIF_BURST_INFO_T eBurstInfo;
    UINT16 u2Nsnum;
		
    if(u4Size != _gu4DTSFrameSize[u1DecId])
    {
        _gu4DTSFrameSize[u1DecId] = u4Size;
        if (AUD_GetSpdifRawDec() == u1DecId)
        {
            switch (AUD_GetDTSFrameSize(u1DecId))
            {
            case 1024:
            	eBurstInfo = BURST_INFO_DTS_1024;
            	u4Reg0 = APROC_RAW_DTS_1024;
            	u2Nsnum = 0x400;
            	break;
            case 2048:
            	eBurstInfo = BURST_INFO_DTS_2048;
            	u4Reg0 = APROC_RAW_DTS_2048;
            	u2Nsnum = 0x800; 
            	break;
            case 512: 
            default:
            	u2Nsnum = 0x200; 
            	eBurstInfo = BURST_INFO_DTS_512;
            	u4Reg0 = APROC_RAW_DTS_512; 				   
            	break;
            }
            _vAUD_Aproc_Set (APROC_CONTROL_TYPE_IEC, APROC_IOCTRL_IEC_RAWAUDFMT, (UINT32 *)&u4Reg0, 1);
        }
        LOG (2, "AUD_SetDTSFrameSize: %d\n", _gu4DTSFrameSize[u1DecId]);
    }
}

UINT32 AUD_GetDTSFrameSize (UINT8 u1DecId)
{
    return _gu4DTSFrameSize[u1DecId];
}


