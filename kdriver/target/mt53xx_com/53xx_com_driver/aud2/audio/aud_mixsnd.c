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
 * $Revision: #7 $
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
#include "aud_if.h"
#include "aud_debug.h"
#include "dsp_common.h"
#include "aud_notify.h"
#include "aud_drv.h"
#include "aud_cfg.h"
#include "drv_adsp.h"
#include "dsp_shm.h"
#include "drv_common.h"
#include "x_aud_dec.h"

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
#include "oss_adap.h"
#endif

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
#ifdef LINUX_TURNKEY_SOLUTION
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/param.h>
size_t ret;
char *fileName = "/mnt/usb/sda1/mixsnd.pcm";
char *upload_fileName = "/mnt/usb/sda1/upload.pcm";
char *upload2_fileName = "/mnt/usb/sda1/upload2.pcm";

#endif
#endif //CC_AUD_ARM_SUPPORT && CC_AUD_ARM_RENDER

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
//#define ALSA_AMIXER_PATH
#define ALSA_APROC_PATH
#define ALSA_MIXSND_ID	AUD_MIXSND_0 //0-1: SW mixsnd 2-3: HW mixsnd (Aout1,Aout2)
#define ALSA_SWDEC_ID	AUD_SWDEC_0 //
#define KEY_MIXSND_ID	AUD_MIXSND_1 //
static void AUD_AprocMixSndInfoInit(AUD_MIXSND_ID_T eMixSndId);
static void AUD_AprocMixSndReset(AUD_MIXSND_ID_T eMixSndId);
static void AUD_AprocMixSndEnable(AUD_MIXSND_ID_T eMixSndId, BOOL fgEnable);
static void AUD_AprocMixSndUpdateWp(AUD_MIXSND_ID_T eMixSndId, UINT32 u4Wp); 
#endif
LINT_EXT_HEADER_END

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY

#ifdef ALSA_PCMDEC_PATH   // defined in dsp_common.h
extern UINT32 u4GetAFIFOEnd(UINT8 u1DspId, UINT8 uDecIndex);
extern UINT32 u4GetAFIFOStart(UINT8 u1DspId,UINT8 uDecIndex);
extern UINT32 u4GetABufPnt(UINT8 u1DspId,UINT8 uDecIndex);
extern void vSetAWritePnt(UINT8 uDecIndex, UINT32 u4WritePointer);
extern void vDspSetAFIFO1SZ(UINT32 u4Size); //0 recover
extern BOOL skip_create_feed_stream;
#define ALSA_PCMDEC_PATH_BUFSIZE 0x2000
#define ALSA_DSPDEC_ID   3  //
#endif

#ifdef   ALSA_VERY_LOW_LATENCY
#define ALSA_VERY_LOW_LATENCY_ID   2
#endif
extern UINT16 AUD_DrvSwapWord(UINT16 u2swap);
extern UINT32 u4GetAprocMemoryMap (UINT32 u4Type);

//#define CC_AUD_APROC_HWMIX 

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define AUD_MIXSND_VERSION_CODE "201202132000"

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
extern void vAprocCmdSet (UINT32 u4Idx, UINT32 u4Data0, UINT32 u4Data1); //parson mixsnd
#endif
//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------

#define u1MixSndThread_Priority 50

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static BOOL _fgForceStopMixSndDma = FALSE;
static HANDLE_T _hAudFeedMixSndThread = NULL_HANDLE;
//static HANDLE_T _hAudFeedAlsaMixSndThread = NULL_HANDLE;
UINT32 u4AudMixSndDbgMsk = 0;

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
//parson mixsnd
#define MIXSND_768  768
#define MIXSND_256  256

static BOOL _fgForceStopMixSndDma_1[AUD_MIXSND_NUM] = {FALSE};
static HANDLE_T _hAudFeedMixSndThread_1[AUD_MIXSND_NUM] = {NULL_HANDLE};
BOOL fgAudFeedMixSndThreadEnable_1[AUD_MIXSND_NUM] = {FALSE};
INT32 i4TestTone[MIXSND_768*2];        // 768 samples , 2 ch
// upload
static HANDLE_T _hAudUploadThread = NULL_HANDLE;
static BOOL _fgForceStopCpuUpload = FALSE;
BOOL fgAudCpuUploadThreadEnable = FALSE;
static HANDLE_T _hAudUpload2Thread = NULL_HANDLE;
static BOOL _fgForceStopCpuUpload2 = FALSE;
BOOL fgAudCpuUpload2ThreadEnable = FALSE;

// risc post, //parson riscpost
static HANDLE_T _hAudRiscPostThread = NULL_HANDLE;
static BOOL _fgForceStopRiscPost = FALSE;
BOOL fgAudRiscPostThreadEnable = FALSE;
#endif

//---------------------------------------------------------------------------
// Function Implementation
//---------------------------------------------------------------------------

//#define MAX_AUD_MIXSND_STREAM_NUM 8 ==> Move to dsp_common.h
#define MAX_AUD_MIXSND_CLIP_NUM 9
#define MIXSND_TRANSFER_SZ             (UINT32)(0x00000100)
#ifdef LINUX_TURNKEY_SOLUTION
#define PRINT_TIME_MSG
#endif
#define TRACK_FIFO

#ifdef PRINT_TIME_MSG
#include <linux/time.h>
#endif

#ifdef MIXSOUND_MIPS_MEASURE
UINT32 _AUD_ReadPsrStc1 (void);
void _AUD_SetPsrStc1 (UINT32 u4Val);
#endif

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4Gain;
    UINT32 u4SampleRate;
    UINT8 u1StereoOnOff; //0: mono, 1: stereo
    UINT8 u1BitDepth; //16,8
    UINT8 u1Endian;// 1:big 0:little
} AUD_MIXSND_CLIP_TYPE_T;

typedef struct
{
    #ifdef CC_AUD_DDI
    UINT8 fgOccupied;
    UINT8 fgReserved[3];
    #endif
    
    UINT8 fgEnable;
    UINT8 fgStop;
    UINT8 fgPause;
    UINT8 fgRepeat;
    UINT8 fgClip;

    UINT8 u1ClipReadIndx;
    UINT8 u1ClipWriteIndx;
    UINT8 u1MixSndStereoOnOff;
    UINT8 u1BitDepth;
    UINT8 u1Endian;

    UINT32 u4SampleRate;
    UINT32 u4AFifoSA;
    UINT32 u4AFifoEA;
    UINT32 u4AFifoSZ;
    UINT32 u4DesAddr;
    UINT32 u4TransferSZ;
    UINT32 u4TransferSZ2;

    UINT32 u4StreamAddr;    //input addr
    UINT32 u4Residual;      //input size
    #ifdef TRACK_FIFO
    UINT32 u4AFifoFullness;
    #endif

    INT16  i2DelayBuf[2];

    INT32  segEnd[2];
    INT32  segStart[2];
    INT32  t_out;
    INT32  t_in;

    UINT32 u4Wp;
} AUD_MIXSND_STREAM_TYPE_T;

AUD_MIXSND_STREAM_TYPE_T _rAudMixSndStream[MAX_AUD_MIXSND_STREAM_NUM];
AUD_MIXSND_CLIP_TYPE_T _rAudMixSndClip[MAX_AUD_MIXSND_STREAM_NUM][MAX_AUD_MIXSND_CLIP_NUM];


#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
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
} _IO_BUFFER_T;

_IO_BUFFER_T _gsAprocMixSndBufInfo[AUD_MIXSND_NUM]; // There are 2 mixsound modules...
static UINT8 _u1AlsaAprocMixsndId[AUD_MIXSND_NUM] = {
    AUD_MIXSND_0, 
    AUD_MIXSND_1, 
    AUD_MIXSND_2, 
    AUD_MIXSND_3, 
    AUD_MIXSND_4, 
    AUD_MIXSND_5, 
    AUD_MIXSND_6, 
    AUD_MIXSND_7,
#ifdef CC_AUD_APROC_HWMIX
    AUD_HW_MIXSND_0,
    AUD_HW_MIXSND_1, 
#endif
};

typedef enum
{
    AUD_SWDEC_0 = 0,
    AUD_SWDEC_NUM
}   AUD_SWDEC_ID_T;
_IO_BUFFER_T _gsAprocSwDecBufInfo[AUD_SWDEC_NUM]; // There are 2 mixsound modules...

#endif //CC_AUD_ARM_SUPPORT && CC_AUD_ARM_RENDER

#ifdef ALSA_MIXSND_PATH
typedef struct
{
    UINT32 u4SA;
    UINT32 u4EA;
    UINT32 u4SZ;
    UINT32 u4RP;
    UINT32 u4WP;
    UINT32 u4Latency;
    UINT32 u4TotalRenderSize;
    UINT32 u4TotalRenderSize2;
} AUD_MIXSND_RINGFIFO_TYPE_T;

AUD_MIXSND_RINGFIFO_TYPE_T _rAudMixSndRingFifo[MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA];
UINT32 u4Latency = 0; //0x1000; //for AV sync issue
BOOL fgMixSndClipMute = FALSE;

#ifdef ALSA_PCMDEC_PATH
UINT32  u4DspDecoderType = 0;
#endif

#if defined(CC_AUD_MIXSND_FOR_ALSA) || defined(CC_ENABLE_AOMX)
UINT32 u4GetMixSndRingFifoRP(UINT8 u1StreamId)
{
    return _rAudMixSndRingFifo[u1StreamId].u4RP;
}

UINT32 u4GetMixSndRingFifoWP(UINT8 u1StreamId)
{
    return _rAudMixSndRingFifo[u1StreamId].u4WP;
}

void vSetMixSndRingFifoWP(UINT8 u1StreamId, UINT32 u4WritePtr)
{
    _rAudMixSndRingFifo[u1StreamId].u4WP = u4WritePtr;
  LOG(3," Enter vSetMixSndRingFifoWP(): _rAudMixSndRingFifo[%d].u4Wp=%x\n",u1StreamId, _rAudMixSndRingFifo[u1StreamId].u4WP);
	
}
#endif
#endif
void do_BigEnd_to_LittleEnd(INT16* input_buf, UINT32 u4Size)
{
    UINT32 i;
    
    for (i=0; i < u4Size; i++)
    {
        input_buf[i] = AUD_DrvSwapWord(input_buf[i]);
    }
}

void do_SRC_S16LE_v2(UINT8 u1StreamId, 
                    INT16* input_buf, INT16* output_buf,
                    UINT32 input_sample, UINT32 output_sample,
                    UINT32 length, UINT8 mono_stereo)
{
    UINT32 i;
    
    if (input_sample == output_sample)
    {
        // 1x upsample
        for (i=0;i<length;i++)
        {
            if (mono_stereo)
            {            
                output_buf[i] = input_buf[i];
            }
            else
            {                
                output_buf[i*2] = input_buf[i];
                output_buf[i*2+1] = input_buf[i];
            }
        }
        return;
    }

    if (mono_stereo == 1)
    {
        UINT8 toExit = 0;
        INT32 step = (0x10000 * (input_sample-1) / (output_sample-1));
        INT32 threshold = 0x10000;
        UINT32 count =0;

        //UINT32 output_length = (((length+1) * output_sample / input_sample)+2) & 0xfffffffe;
        UINT32 output_length=length*6+2;

        if (_rAudMixSndStream[u1StreamId].t_in == -1){
            _rAudMixSndStream[u1StreamId].segEnd[0] = input_buf[0];
            _rAudMixSndStream[u1StreamId].segEnd[1] = input_buf[1];   
        }else{
            _rAudMixSndStream[u1StreamId].segStart[0] = input_buf[_rAudMixSndStream[u1StreamId].t_in*2];
            _rAudMixSndStream[u1StreamId].segStart[1] = input_buf[_rAudMixSndStream[u1StreamId].t_in*2+1];
            _rAudMixSndStream[u1StreamId].segEnd[0] = input_buf[_rAudMixSndStream[u1StreamId].t_in*2+2];
            _rAudMixSndStream[u1StreamId].segEnd[1] = input_buf[_rAudMixSndStream[u1StreamId].t_in*2+3];    
        }

        for (i=0; i < output_length; i+=2){

            count +=2 ;

            output_buf[i] = ((INT32)_rAudMixSndStream[u1StreamId].segStart[0]*(0x10000 - _rAudMixSndStream[u1StreamId].t_out) + (INT32)_rAudMixSndStream[u1StreamId].segEnd[0]*_rAudMixSndStream[u1StreamId].t_out) >> 16;
            output_buf[i+1] = ((INT32)_rAudMixSndStream[u1StreamId].segStart[1]*(0x10000 - _rAudMixSndStream[u1StreamId].t_out) + (INT32)_rAudMixSndStream[u1StreamId].segEnd[1]*_rAudMixSndStream[u1StreamId].t_out) >> 16;

            _rAudMixSndStream[u1StreamId].t_out += step;
            if (_rAudMixSndStream[u1StreamId].t_out >= threshold){
                while (_rAudMixSndStream[u1StreamId].t_out >= threshold){
                    _rAudMixSndStream[u1StreamId].t_out -= threshold;
                    _rAudMixSndStream[u1StreamId].t_in++;
                    if (_rAudMixSndStream[u1StreamId].t_in == length/2-1){
                        toExit = 1;
                        _rAudMixSndStream[u1StreamId].t_in = -1;
                    }
                }

                if (toExit == 1) break;

                _rAudMixSndStream[u1StreamId].segStart[0] = input_buf[_rAudMixSndStream[u1StreamId].t_in*2];
                _rAudMixSndStream[u1StreamId].segStart[1] = input_buf[_rAudMixSndStream[u1StreamId].t_in*2+1];
                _rAudMixSndStream[u1StreamId].segEnd[0] = input_buf[_rAudMixSndStream[u1StreamId].t_in*2+2];
                _rAudMixSndStream[u1StreamId].segEnd[1] = input_buf[_rAudMixSndStream[u1StreamId].t_in*2+3];
            }
            
        }

        _rAudMixSndStream[u1StreamId].segStart[0] = input_buf[length-2];
        _rAudMixSndStream[u1StreamId].segStart[1] = input_buf[length-1];

        _rAudMixSndStream[u1StreamId].u4TransferSZ2 = count * 2;
    }
    else
    {
        UINT8 toExit = 0;
        INT32 step = (0x10000 * (input_sample-1) / (output_sample-1));
        INT32 threshold = 0x10000;
        UINT32 count =0;

        //UINT32 output_length = (((length+1) * output_sample / input_sample)+2) & 0xfffffffe;
        UINT32 output_length=length*6+2;

        if (_rAudMixSndStream[u1StreamId].t_in == -1){
            _rAudMixSndStream[u1StreamId].segEnd[0] = input_buf[0];
        }else{
            _rAudMixSndStream[u1StreamId].segStart[0] = input_buf[_rAudMixSndStream[u1StreamId].t_in];
            _rAudMixSndStream[u1StreamId].segEnd[0] = input_buf[_rAudMixSndStream[u1StreamId].t_in+1];
        }

        for (i=0; i < output_length; i++){

            count ++;

            output_buf[i*2] = ((INT32)_rAudMixSndStream[u1StreamId].segStart[0]*(0x10000 - _rAudMixSndStream[u1StreamId].t_out) + (INT32)_rAudMixSndStream[u1StreamId].segEnd[0]*_rAudMixSndStream[u1StreamId].t_out) >> 16;
            output_buf[i*2+1] = output_buf[i*2];

            _rAudMixSndStream[u1StreamId].t_out += step;
            if (_rAudMixSndStream[u1StreamId].t_out >= threshold){
                while (_rAudMixSndStream[u1StreamId].t_out >= threshold){
                    _rAudMixSndStream[u1StreamId].t_out -= threshold;
                    _rAudMixSndStream[u1StreamId].t_in++;
                    if (_rAudMixSndStream[u1StreamId].t_in == length-1){
                        toExit = 1;
                        _rAudMixSndStream[u1StreamId].t_in = -1;
                    }
                }

                if (toExit == 1) break;

                _rAudMixSndStream[u1StreamId].segStart[0] = input_buf[_rAudMixSndStream[u1StreamId].t_in];
                _rAudMixSndStream[u1StreamId].segEnd[0] = input_buf[_rAudMixSndStream[u1StreamId].t_in+1];
            }
            
        }

        _rAudMixSndStream[u1StreamId].segStart[0] = input_buf[length-1];

        _rAudMixSndStream[u1StreamId].u4TransferSZ2 = count * 4;    
    }

    //return count;
}

void do_SRC_S16LE(UINT8 u1StreamId,
                INT16* input_buf, INT16* output_buf,
                UINT32 input_sample, UINT32 output_sample,
                UINT32 length, UINT8 mono_stereo)
{
    UINT32 i,output_length;
    UINT32 u4SRCCoeff;
    UINT32 u4Integer;
    UINT32 u4Decimal;

    output_length = (((length * output_sample / input_sample)+1) & 0xfffffffe);
    u4SRCCoeff = (0xffff * (input_sample-1) / (output_sample-1));
    u4Integer = 0;
    u4Decimal = 0;
    i = 0;

    if (length == output_length)
    {
        // 1x upsample
        if (mono_stereo)
        {
            for (i=0;i<output_length;i++)
            {
                output_buf[i] = input_buf[i];
            }
        }
        else
        {
            for (i=0;i<output_length;i++)
            {
                output_buf[i*2] = input_buf[i];
                output_buf[i*2+1] = input_buf[i];
            }
        }
    }
    else
    {
        if (mono_stereo)
        {
            while (u4Decimal < 0x10000)
            {
                output_buf[i] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))) +
                                (((INT32)input_buf[0] * u4Decimal)))>>16);
                output_buf[i+1] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[1] * (0x10000 - u4Decimal))) +
                                  (((INT32)input_buf[1] * u4Decimal)))>>16);
                u4Decimal += u4SRCCoeff;
                i += 2;
            }
            u4Decimal -= 0x10000;

            //Stereo
            for (;i<output_length;i+=2)
            {
                output_buf[i] = (((((INT32)input_buf[u4Integer] * (0x10000 - u4Decimal))) +
                                (((INT32)input_buf[u4Integer+2] * u4Decimal)))>>16);
                output_buf[i+1] = (((((INT32)input_buf[u4Integer+1] * (0x10000 - u4Decimal))) +
                                  (((INT32)input_buf[u4Integer+3] * u4Decimal)))>>16);
                u4Decimal += u4SRCCoeff;
                if (u4Decimal >= 0x10000)
                {
                    u4Integer += 2;
                    u4Decimal -= 0x10000;
                }
            }

            _rAudMixSndStream[u1StreamId].i2DelayBuf[0] = input_buf[length-2];
            _rAudMixSndStream[u1StreamId].i2DelayBuf[1] = input_buf[length-1];
        }
        else
        {
            while (u4Decimal < 0x10000)
            {
                output_buf[i*2] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))) +
                                  (((INT32)input_buf[0] * u4Decimal)))>>16);
                output_buf[i*2+1] = output_buf[i*2];
                u4Decimal += u4SRCCoeff;
                i++;
            }
            u4Decimal -= 0x10000;

            for (;i<output_length;i++)
            {
                output_buf[i*2] = (((((INT32)input_buf[u4Integer] * (0x10000 - u4Decimal))) +
                                  (((INT32)input_buf[u4Integer+1] * u4Decimal)))>>16);
                output_buf[i*2+1] = output_buf[i*2];
                u4Decimal += u4SRCCoeff;
                if (u4Decimal >= 0x10000)
                {
                    u4Integer++;
                    u4Decimal -= 0x10000;
                }
            }
            _rAudMixSndStream[u1StreamId].i2DelayBuf[0] = input_buf[length-1];
        }
    }
}

#define U8toS16(val) (((INT16)(val)-128)<<8)

void do_SRC_U8_v2(UINT8 u1StreamId, 
                    UINT8* input_buf, INT16* output_buf,
                    UINT32 input_sample, UINT32 output_sample,
                    UINT32 length, UINT8 mono_stereo)
{
    UINT32 i;
    
    if (input_sample == output_sample)
    {
        // 1x upsample
        for (i=0;i<length;i++)
        {
            if (mono_stereo)
            {            
                output_buf[i] = U8toS16(input_buf[i]);
            }
            else
            {                
                output_buf[i*2] = U8toS16(input_buf[i]);
                output_buf[i*2+1] = U8toS16(input_buf[i]);
            }
        }
        return;
    }

    if (mono_stereo == 1)
    {
        UINT8 toExit = 0;
        INT32 step = (0x10000 * (input_sample-1) / (output_sample-1));
        INT32 threshold = 0x10000;
        UINT32 count =0;

        //UINT32 output_length = (((length+1) * output_sample / input_sample)+2) & 0xfffffffe;
        UINT32 output_length=length*6+2;

        if (_rAudMixSndStream[u1StreamId].t_in == -1){
            _rAudMixSndStream[u1StreamId].segEnd[0] = U8toS16(input_buf[0]);
            _rAudMixSndStream[u1StreamId].segEnd[1] = U8toS16(input_buf[1]);
        }else{
            _rAudMixSndStream[u1StreamId].segStart[0] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in*2]);
            _rAudMixSndStream[u1StreamId].segStart[1] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in*2+1]);
            _rAudMixSndStream[u1StreamId].segEnd[0] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in*2+2]);
            _rAudMixSndStream[u1StreamId].segEnd[1] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in*2+3]);
        }

        for (i=0; i < output_length; i+=2){

            count +=2 ;

            output_buf[i] = ((INT32)_rAudMixSndStream[u1StreamId].segStart[0]*(0x10000 - _rAudMixSndStream[u1StreamId].t_out) + (INT32)_rAudMixSndStream[u1StreamId].segEnd[0]*_rAudMixSndStream[u1StreamId].t_out) >> 16;
            output_buf[i+1] = ((INT32)_rAudMixSndStream[u1StreamId].segStart[1]*(0x10000 - _rAudMixSndStream[u1StreamId].t_out) + (INT32)_rAudMixSndStream[u1StreamId].segEnd[1]*_rAudMixSndStream[u1StreamId].t_out) >> 16;

            _rAudMixSndStream[u1StreamId].t_out += step;
            if (_rAudMixSndStream[u1StreamId].t_out >= threshold){
                while (_rAudMixSndStream[u1StreamId].t_out >= threshold){
                    _rAudMixSndStream[u1StreamId].t_out -= threshold;
                    _rAudMixSndStream[u1StreamId].t_in++;
                    if (_rAudMixSndStream[u1StreamId].t_in == length/2-1){
                        toExit = 1;
                        _rAudMixSndStream[u1StreamId].t_in = -1;
                    }
                }

                if (toExit == 1) break;

                _rAudMixSndStream[u1StreamId].segStart[0] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in*2]);
                _rAudMixSndStream[u1StreamId].segStart[1] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in*2+1]);
                _rAudMixSndStream[u1StreamId].segEnd[0] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in*2+2]);
                _rAudMixSndStream[u1StreamId].segEnd[1] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in*2+3]);
            }
            
        }

        _rAudMixSndStream[u1StreamId].segStart[0] = U8toS16(input_buf[length-2]);
        _rAudMixSndStream[u1StreamId].segStart[1] = U8toS16(input_buf[length-1]);

        _rAudMixSndStream[u1StreamId].u4TransferSZ2 = count * 2;
    }
    else
    {
        UINT8 toExit = 0;
        INT32 step = (0x10000 * (input_sample-1) / (output_sample-1));
        INT32 threshold = 0x10000;
        UINT32 count =0;

        //UINT32 output_length = (((length+1) * output_sample / input_sample)+2) & 0xfffffffe;
        UINT32 output_length=length*6+2;

        if (_rAudMixSndStream[u1StreamId].t_in == -1){
            _rAudMixSndStream[u1StreamId].segEnd[0] = U8toS16(input_buf[0]);
        }else{
            _rAudMixSndStream[u1StreamId].segStart[0] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in]);
            _rAudMixSndStream[u1StreamId].segEnd[0] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in+1]);
        }

        for (i=0; i < output_length; i++){

            count ++;

            output_buf[i*2] = ((INT32)_rAudMixSndStream[u1StreamId].segStart[0]*(0x10000 - _rAudMixSndStream[u1StreamId].t_out) + (INT32)_rAudMixSndStream[u1StreamId].segEnd[0]*_rAudMixSndStream[u1StreamId].t_out) >> 16;
            output_buf[i*2+1] = output_buf[i*2];

            _rAudMixSndStream[u1StreamId].t_out += step;
            if (_rAudMixSndStream[u1StreamId].t_out >= threshold){
                while (_rAudMixSndStream[u1StreamId].t_out >= threshold){
                    _rAudMixSndStream[u1StreamId].t_out -= threshold;
                    _rAudMixSndStream[u1StreamId].t_in++;
                    if (_rAudMixSndStream[u1StreamId].t_in == length-1){
                        toExit = 1;
                        _rAudMixSndStream[u1StreamId].t_in = -1;
                    }
                }

                if (toExit == 1) break;

                _rAudMixSndStream[u1StreamId].segStart[0] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in]);
                _rAudMixSndStream[u1StreamId].segEnd[0] = U8toS16(input_buf[_rAudMixSndStream[u1StreamId].t_in+1]);
            }
            
        }

        _rAudMixSndStream[u1StreamId].segStart[0] = U8toS16(input_buf[length-1]);

        _rAudMixSndStream[u1StreamId].u4TransferSZ2 = count * 4;    
    }

    //return count;
}

void do_SRC_U8(UINT8 u1StreamId,
                UINT8* input_buf, INT16* output_buf,
                UINT32 input_sample, UINT32 output_sample,
                UINT32 length, UINT8 mono_stereo)
{
    UINT32 i,output_length;
    UINT32 u4SRCCoeff;
    UINT32 u4Integer;
    UINT32 u4Decimal;

    output_length = (((length * output_sample / input_sample)+1) & 0xfffffffe);
    u4SRCCoeff = (0xffff * (input_sample-1) / (output_sample-1));
    u4Integer = 0;
    u4Decimal = 0;
    i = 0;

    if (length == output_length)
    {
        // 1x upsample
        if (mono_stereo)
        {
            for (i=0;i<output_length;i++)
            {
                output_buf[i] = U8toS16(input_buf[i]);
            }
        }
        else
        {
            for (i=0;i<output_length;i++)
            {
                output_buf[i*2] = U8toS16(input_buf[i]);
                output_buf[i*2+1] = U8toS16(input_buf[i]);
            }
        }
    }
    else
    {
        if (mono_stereo)
        {
            while (u4Decimal < 0x10000)
            {
                output_buf[i] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))) +
                                (((INT32)U8toS16(input_buf[0]) * u4Decimal)))>>16);
                output_buf[i+1] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[1] * (0x10000 - u4Decimal))) +
                                  (((INT32)U8toS16(input_buf[1]) * u4Decimal)))>>16);
                u4Decimal += u4SRCCoeff;
                i += 2;
            }
            u4Decimal -= 0x10000;

            //Stereo
            for (;i<output_length;i+=2)
            {
                output_buf[i] = (((((INT32)U8toS16(input_buf[u4Integer]) * (0x10000 - u4Decimal))) +
                                (((INT32)U8toS16(input_buf[u4Integer+2]) * u4Decimal)))>>16);
                output_buf[i+1] = (((((INT32)U8toS16(input_buf[u4Integer+1]) * (0x10000 - u4Decimal))) +
                                  (((INT32)U8toS16(input_buf[u4Integer+3]) * u4Decimal)))>>16);
                u4Decimal += u4SRCCoeff;
                if (u4Decimal >= 0x10000)
                {
                    u4Integer += 2;
                    u4Decimal -= 0x10000;
                }
            }

            _rAudMixSndStream[u1StreamId].i2DelayBuf[0] = U8toS16(input_buf[length-2]);
            _rAudMixSndStream[u1StreamId].i2DelayBuf[1] = U8toS16(input_buf[length-1]);
        }
        else
        {
            while (u4Decimal < 0x10000)
            {
                output_buf[i*2] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))) +
                                  (((INT32)U8toS16(input_buf[0]) * u4Decimal)))>>16);
                output_buf[i*2+1] = output_buf[i*2];
                u4Decimal += u4SRCCoeff;
                i++;
            }
            u4Decimal -= 0x10000;

            for (;i<output_length;i++)
            {
                output_buf[i*2] = (((((INT32)U8toS16(input_buf[u4Integer]) * (0x10000 - u4Decimal))) +
                                  (((INT32)U8toS16(input_buf[u4Integer+1]) * u4Decimal)))>>16);
                output_buf[i*2+1] = output_buf[i*2];
                u4Decimal += u4SRCCoeff;
                if (u4Decimal >= 0x10000)
                {
                    u4Integer++;
                    u4Decimal -= 0x10000;
                }
            }
            _rAudMixSndStream[u1StreamId].i2DelayBuf[0] = U8toS16(input_buf[length-1]);
        }
    }
}

//testing code

extern UINT32 AUD_DspGetMixSndReadPtr(UINT8 u1StreamID);
extern void AUD_DspSetMixSndWritePtr(UINT8 u1StreamID,UINT32 u4WritePtr);

#if defined(ALSA_AMIXER_PATH)
#define SAMPLE_UNIT 4 // 256/64 =4
#define SW_DEC_BLOCKS 6
UINT32 u4LoopCnt123  = SAMPLE_UNIT;  
UINT32 u4LoopCnt456  = 0;
#define APROC_ADSP_IOBUF_SWDEC_SIZE (256*4*8*6) //256 * 4 bytes * 8 blocks * 6 ch
#elif defined(ALSA_APROC_PATH)
#define SAMPLE_UNIT  768
UINT32 u4LoopCnt123[8]  = {SAMPLE_UNIT, SAMPLE_UNIT, SAMPLE_UNIT, SAMPLE_UNIT, SAMPLE_UNIT, SAMPLE_UNIT, SAMPLE_UNIT, SAMPLE_UNIT}; // 768/64 = 12
UINT32 u4LoopCnt456[8]  = {0, 0, 0, 0, 0, 0, 0, 0};
#endif

BOOL fgIsSoftTransferable(UINT8 u1StreamID)
{
    UINT32 u4ReadPtr;
    UINT32 u4WritePtr;

    u4WritePtr = ((_rAudMixSndStream[u1StreamID].u4DesAddr + _rAudMixSndStream[u1StreamID].u4TransferSZ2) >= _rAudMixSndStream[u1StreamID].u4AFifoEA) ?
                  ((_rAudMixSndStream[u1StreamID].u4DesAddr + _rAudMixSndStream[u1StreamID].u4TransferSZ2) - (_rAudMixSndStream[u1StreamID].u4AFifoEA-_rAudMixSndStream[u1StreamID].u4AFifoSA)) :
                  (_rAudMixSndStream[u1StreamID].u4DesAddr + _rAudMixSndStream[u1StreamID].u4TransferSZ2);

#ifdef ALSA_AMIXER_PATH
		if (u1StreamID == ALSA_MIXSND_STREAM_ID)
		{
	        u4ReadPtr = _gsAprocSwDecBufInfo[ALSA_SWDEC_ID].u4Rp;

	        if (u4LoopCnt123 != u4ReadPtr*SAMPLE_UNIT) 
	        {
	            return TRUE;
	        }
	        return FALSE;
		}
#elif defined(ALSA_APROC_PATH)
     #ifdef ALSA_PCMDEC_PATH
	   if (u1StreamID != ALSA_MIXSND_STREAM_ID+ALSA_DSPDEC_ID)
	   {
     #endif	 
		if (u1StreamID >= ALSA_MIXSND_STREAM_ID)
		{
			if(_u1AlsaAprocMixsndId[u1StreamID-ALSA_MIXSND_STREAM_ID] >= AUD_MIXSND_NUM )
			{
				Printf("No available Mixsnd Path for play !!!!");
				return FALSE; 
			}
			
	        u4ReadPtr = _gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1StreamID-ALSA_MIXSND_STREAM_ID]].u4Rp;
		//	printf("[ALSA]u4ReadPtr =%d Wp=%d\n",u4ReadPtr,_gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1StreamID-ALSA_MIXSND_STREAM_ID]].u4Wp);
	        if (_gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1StreamID-ALSA_MIXSND_STREAM_ID]].u4Wp != u4ReadPtr)  //768 samples = 64 samples * 12
	        {
	            return TRUE;
	        }
	        return FALSE;
		}	
     #ifdef ALSA_PCMDEC_PATH
	   }
     #endif		
#endif

#ifdef ALSA_PCMDEC_PATH
    if (u1StreamID == ALSA_MIXSND_STREAM_ID+ALSA_DSPDEC_ID) //<--- StreamID from Offload
    {
        //Check Read Ptr First
        u4ReadPtr = u4GetABufPnt(AUD_DSP0,AUD_DEC_MAIN);
    }
    else
#endif
    {
        //Check Read Ptr First
        u4ReadPtr = AUD_DspGetMixSndReadPtr(u1StreamID);
    }

    #ifdef TRACK_FIFO
    if (u4ReadPtr > _rAudMixSndStream[u1StreamID].u4DesAddr)
    {
        _rAudMixSndStream[u1StreamID].u4AFifoFullness = (_rAudMixSndStream[u1StreamID].u4AFifoEA-_rAudMixSndStream[u1StreamID].u4AFifoSA) - (u4ReadPtr - _rAudMixSndStream[u1StreamID].u4DesAddr);
    }
    else
    {
        _rAudMixSndStream[u1StreamID].u4AFifoFullness = (_rAudMixSndStream[u1StreamID].u4DesAddr - u4ReadPtr);
    }

    if (_rAudMixSndStream[u1StreamID].u4AFifoFullness == 0)
    {
        //Printf("\nU\n");
    }
    #endif

    if (u4WritePtr < _rAudMixSndStream[u1StreamID].u4DesAddr)
    {
        //  u4WritePtr  u4DesAddr
        if ((u4ReadPtr > u4WritePtr) && (u4ReadPtr <= _rAudMixSndStream[u1StreamID].u4DesAddr))
        {
            return TRUE;
        }
    }
    else
    {
        //  u4DesAddr   u4WritePtr
        if ((u4ReadPtr > u4WritePtr) || (u4ReadPtr <= _rAudMixSndStream[u1StreamID].u4DesAddr))
        {
            return TRUE;
        }
    }
    return FALSE;
}

#ifdef PRINT_TIME_MSG
int audio_mixsnd_log = 0;
#endif

void vSoftTransfer(UINT32 u4SrcAddr, UINT32 u4Size,
                       UINT32 u4DstAddr, UINT32 u4DstFifoStart, UINT32 u4DstFifoEnd,
                       UINT8 u1StreamID)
{
	
    UINT8 *u1DstTmp, *u1Dst, *u1Src;
    UINT32 j;
    UINT32 u4ChNum;
	UINT32 uSample;
	UINT8 u1AlsaStreamID;
	UINT8 tempWP=0;
	UINT32 tempCa=0;
	UINT32 i;
	UINT32 k=1;
	UINT32 SrcAddr_temp;
	
	BOOL uFlag=FALSE;
	i = u1StreamID;
 	uSample = u4Size/4;
  	SrcAddr_temp = u4SrcAddr;
	
	if (u1StreamID >= ALSA_MIXSND_STREAM_ID)
		u1AlsaStreamID = u1StreamID - ALSA_MIXSND_STREAM_ID;
	else
		u1AlsaStreamID = 0xff;
	
while(1)
	 {

		if(u1AlsaStreamID != 0xff)
		{
	
			if((u4LoopCnt123[u1AlsaStreamID] >= uSample)&&(tempCa==0))
			{
				u4LoopCnt123[u1AlsaStreamID] -=uSample;
			
				if ((u4LoopCnt123[u1AlsaStreamID]) == 0)
        		{
    
					tempWP = _gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1AlsaStreamID]].u4Wp;
        			if(_gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1AlsaStreamID]].u4Wp == 2 )
        			{
        		
        				tempWP = 0;
        			}
					else
					{
				
						tempWP +=1;
					}
			
            		AUD_AprocMixSndUpdateWp(_u1AlsaAprocMixsndId[u1AlsaStreamID], _gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1AlsaStreamID]].u4Wp); 
            		_rAudMixSndStream[u1StreamID].u4Wp = tempWP;
					_gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1AlsaStreamID]].u4Wp = tempWP;
			
					u4LoopCnt123[u1AlsaStreamID] = 768;
			
        		}

				uFlag = TRUE;
		}
		else{
				
				if(k == 1)
					{
					
						u4Size = u4LoopCnt123[u1AlsaStreamID]*4;
						
						tempCa = uSample - u4LoopCnt123[u1AlsaStreamID];

						tempWP = _gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1AlsaStreamID]].u4Wp;
        				if(_gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1AlsaStreamID]].u4Wp == 2 )
        				{
        		
        					tempWP = 0;
        				}
						else
						{
				
							tempWP +=1;
						}
			
            		AUD_AprocMixSndUpdateWp(_u1AlsaAprocMixsndId[u1AlsaStreamID], _gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1AlsaStreamID]].u4Wp); 
            		_rAudMixSndStream[u1StreamID].u4Wp = tempWP;
					_gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1AlsaStreamID]].u4Wp = tempWP;
		
					u4LoopCnt123[u1AlsaStreamID] = 768;
						
					}


				if(k == 2)
					{
				
						u4SrcAddr = SrcAddr_temp + u4Size;
						u4Size=tempCa*4;
				
						if (i >= ALSA_MIXSND_STREAM_ID)
						{
	                   		 _rAudMixSndStream[i].u4DesAddr = _rAudMixSndStream[i].u4AFifoSA + 
							  (_rAudMixSndStream[i].u4Wp*(768*4)) + ((768-u4LoopCnt123[i-ALSA_MIXSND_STREAM_ID])*4); 
						
							u4DstAddr = _rAudMixSndStream[i].u4DesAddr;
						}
						u4LoopCnt123[u1AlsaStreamID] = u4LoopCnt123[u1AlsaStreamID]-(u4Size/4);
						
						k=1;
						uFlag=TRUE;
						tempCa = 0;
					}
				k++;
				
			}
		
		}
	

   		 u1Dst = (UINT8*)VIRTUAL(u4DstAddr);
    	 u1Src = (UINT8*)VIRTUAL(u4SrcAddr);
	
	
		if(_u1AlsaAprocMixsndId[u1AlsaStreamID] >= AUD_MIXSND_NUM )
		{
			Printf("No available Mixsnd Path for play !!!!");
			return; 
		}
	
    	for (j=0; j<(u4Size/2/2); j++)
    	{
        	u4ChNum = 2;                                 
        	u1DstTmp = u1Dst;
        	for (; u4ChNum != 0; u4ChNum --)
        	{
            *u1DstTmp++ = 0;
            *u1DstTmp++ = 0;
            *u1DstTmp++ = *u1Src++;
            *u1DstTmp++ = *u1Src++;
            u1DstTmp = (UINT8 *) ((UINT32) u1Dst + (4*768*_gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1AlsaStreamID]].u4End));
        	}
        	u1Dst = (UINT8 *) ((UINT32) u1Dst + 4);                        
    	}
    	DSP_FlushInvalidateDCacheFree(u4DstAddr, u4Size);
    	DSP_FlushInvalidateDCacheFree(u4DstAddr+(4*768*_gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1AlsaStreamID]].u4End), u4Size); 
		
  	

        if ((_rAudMixSndStream[i].fgClip == 0) && (_rAudMixSndRingFifo[u1StreamID - ALSA_MIXSND_STREAM_ID].u4Latency))
    	{
        	if (_rAudMixSndRingFifo[u1StreamID - ALSA_MIXSND_STREAM_ID].u4Latency > u4Size)
        	{
            	_rAudMixSndRingFifo[u1StreamID - ALSA_MIXSND_STREAM_ID].u4Latency -= u4Size;
            	return;
        	}
        	else
        	{
            	Printf("DSP start decoding\n");
            	_rAudMixSndRingFifo[u1StreamID - ALSA_MIXSND_STREAM_ID].u4Latency = 0;
        	}
    	}
		if(uFlag)
			break;

		
   	}


     		       
#ifdef PRINT_TIME_MSG
    if (audio_mixsnd_log)
    {
        struct timeval tv;
        do_gettimeofday(&tv);
        LOG(5, "%d.%06d:Render End\n", tv.tv_sec, tv.tv_usec);
        audio_mixsnd_log = 0;

        if (u1StreamID >= ALSA_MIXSND_STREAM_ID)
        {
            LOG(5, "  Full: 0x%x\n", (_rAudMixSndRingFifo[u1StreamID-ALSA_MIXSND_STREAM_ID].u4WP >= _rAudMixSndRingFifo[u1StreamID-ALSA_MIXSND_STREAM_ID].u4RP) ?
                    (_rAudMixSndRingFifo[u1StreamID-ALSA_MIXSND_STREAM_ID].u4WP - _rAudMixSndRingFifo[u1StreamID-ALSA_MIXSND_STREAM_ID].u4RP) :
                    (_rAudMixSndRingFifo[u1StreamID-ALSA_MIXSND_STREAM_ID].u4SZ + _rAudMixSndRingFifo[u1StreamID-ALSA_MIXSND_STREAM_ID].u4WP - _rAudMixSndRingFifo[u1StreamID-ALSA_MIXSND_STREAM_ID].u4RP));
        }        
    }
#endif
}

BOOL fgAudFeedMixSndThreadEnable = FALSE; //for CMPB integration
#ifdef MIXSND_OUTPUT_DBG
UINT32 u4MixSndDumpBufAddr = 0;
UINT32 u4MixSndDumpSize = 0;
UINT8 u1DumpLocation = 0; // 0: Before SW ASRC 1: After SW ASRC
UINT8 u1DumpStreamId = 0;
UINT32 u4OutputBuf = 0; //TEST
UINT32 u4TotalOutputSize = 0;

void vSetMixSndDumpInfo(UINT32 u4Addr, UINT32 u4Size)
{
    u4MixSndDumpBufAddr = u4Addr;
    u4MixSndDumpSize = u4Size;
    u4OutputBuf = u4Addr;
    u4TotalOutputSize = 0;
    Printf("Set MixSnd Dump Start Addr: %x Size: %x\n", u4MixSndDumpBufAddr, u4MixSndDumpSize);
}

void vResetMixSndDumpInfo(void)
{
    u4OutputBuf = u4MixSndDumpBufAddr;
    u4TotalOutputSize = 0;
    Printf("Reset MixSnd Dump Start Addr: %x Size: %x\n", u4MixSndDumpBufAddr, u4MixSndDumpSize);
}
#endif

extern void _AUD_DspMixSndControl2(UINT16 u2Mode); //FLASHLITE_CONFLICT

#ifdef MIXSOUND_MIPS_MEASURE
UINT32 u4MixSndSendCnt = 0;
UINT32 u4MixSndMIPS = 0;

void vMixSndMIPSQuery(void)
{
    Printf("u4MixSndSendCnt: %d\n", u4MixSndSendCnt);
    Printf("u4MixSndMIPS: %d\n", u4MixSndMIPS);
    Printf("Cycle: %d\n", (u4MixSndSendCnt == 0) ? 0 : u4MixSndMIPS/u4MixSndSendCnt);
}

void vMixSndMIPSReset(void)
{
    u4MixSndSendCnt = 0;
    u4MixSndMIPS = 0;
}
#endif

static void _AudFeedMixSndThread(const void* pvArg)
{
    int i;
    UINT32 u4AFifoTA;
    AudDecNfyFct pfAudDecNfy = NULL;
    BOOL fgDataEmpty = FALSE;
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));

    Printf("[FeedMixSndThread] Init - %s\n", AUD_MIXSND_VERSION_CODE);

    x_memset((VOID*)VIRTUAL((UINT32)_rAudMixSndStream), 0, sizeof(AUD_MIXSND_STREAM_TYPE_T) * MAX_AUD_MIXSND_STREAM_NUM);
    x_memset((VOID*)VIRTUAL((UINT32)_rAudMixSndClip), 0, sizeof(AUD_MIXSND_CLIP_TYPE_T) * MAX_AUD_MIXSND_STREAM_NUM * MAX_AUD_MIXSND_CLIP_NUM);
    x_memset((VOID*)VIRTUAL((UINT32)_rAudMixSndRingFifo), 0, sizeof(AUD_MIXSND_RINGFIFO_TYPE_T)* MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA);

    for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
    {
        #ifdef CC_AUD_DDI
        _rAudMixSndStream[i].fgOccupied = 0;
        #endif
        
        _rAudMixSndStream[i].fgEnable = 0;
        _rAudMixSndStream[i].fgStop = 0;
        _rAudMixSndStream[i].fgPause = 0;
        _rAudMixSndStream[i].fgRepeat = 0;
        _rAudMixSndStream[i].fgClip = 0;

        _rAudMixSndStream[i].u1ClipReadIndx = 0;
        _rAudMixSndStream[i].u1ClipWriteIndx = 0;

        _rAudMixSndStream[i].u1MixSndStereoOnOff = 0;
        _rAudMixSndStream[i].u1BitDepth = 16;
        _rAudMixSndStream[i].u1Endian = 0;
        _rAudMixSndStream[i].u4SampleRate = 48000;
        _rAudMixSndStream[i].u4TransferSZ = 0;
        _rAudMixSndStream[i].u4TransferSZ2 = 0;
        _rAudMixSndStream[i].u4AFifoSZ = u4GetMixSoundBufSize2()/MAX_AUD_MIXSND_STREAM_NUM;
        _rAudMixSndStream[i].u4AFifoSA = u4GetMixSoundStartByteAddr2() + i * u4GetMixSoundBufSize2()/MAX_AUD_MIXSND_STREAM_NUM;
        _rAudMixSndStream[i].u4AFifoEA = _rAudMixSndStream[i].u4AFifoSA + _rAudMixSndStream[i].u4AFifoSZ;
        _rAudMixSndStream[i].u4DesAddr = _rAudMixSndStream[i].u4AFifoSA;

        _rAudMixSndStream[i].u4StreamAddr = 0;
        _rAudMixSndStream[i].u4Residual = 0;
        #ifdef TRACK_FIFO
        _rAudMixSndStream[i].u4AFifoFullness = 0;
        #endif

        _rAudMixSndStream[i].i2DelayBuf[0] = 0;
        _rAudMixSndStream[i].i2DelayBuf[1] = 0;
        
        _rAudMixSndStream[i].segEnd[0] = 0;
        _rAudMixSndStream[i].segEnd[1] = 0;
        _rAudMixSndStream[i].segStart[0] = 0;
        _rAudMixSndStream[i].segStart[1] = 0;
        _rAudMixSndStream[i].t_out = 0;
        _rAudMixSndStream[i].t_in = -1;
    }

    for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA;i++)
    {
        _rAudMixSndRingFifo[i].u4SZ = u4GetMixSoundBufSize3()/MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA;
        _rAudMixSndRingFifo[i].u4SA = u4GetMixSoundStartByteAddr3()+ (i * u4GetMixSoundBufSize3()/MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA);
        _rAudMixSndRingFifo[i].u4EA = _rAudMixSndRingFifo[i].u4SA + _rAudMixSndRingFifo[i].u4SZ;
        _rAudMixSndRingFifo[i].u4RP = _rAudMixSndRingFifo[i].u4WP = _rAudMixSndRingFifo[i].u4SA;
    }

    #ifdef ALSA_PCMDEC_PATH
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_DSPDEC_ID].u4AFifoSA = u4GetAFIFOStart(AUD_DSP0,AUD_DEC_MAIN); //< -------- Stream ID from offload
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_DSPDEC_ID].u4AFifoEA = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_DSPDEC_ID].u4AFifoSA + ALSA_PCMDEC_PATH_BUFSIZE;
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_DSPDEC_ID].u4AFifoSZ = ALSA_PCMDEC_PATH_BUFSIZE;
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_DSPDEC_ID].u4DesAddr = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_DSPDEC_ID].u4AFifoSA;
    #endif

    #ifdef ALSA_VERY_LOW_LATENCY
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_VERY_LOW_LATENCY_ID].u4AFifoSZ = u4GetMixSoundBufSize2()/MAX_AUD_MIXSND_STREAM_NUM/2;	
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_VERY_LOW_LATENCY_ID].u4AFifoSA = u4GetMixSoundStartByteAddr2() + (ALSA_MIXSND_STREAM_ID+ALSA_VERY_LOW_LATENCY_ID)* u4GetMixSoundBufSize2()/MAX_AUD_MIXSND_STREAM_NUM;
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_VERY_LOW_LATENCY_ID].u4AFifoEA = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_VERY_LOW_LATENCY_ID].u4AFifoSA + _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_VERY_LOW_LATENCY_ID].u4AFifoSZ;
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_VERY_LOW_LATENCY_ID].u4DesAddr = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+ALSA_VERY_LOW_LATENCY_ID].u4AFifoSA;	
    #endif

    u4AFifoTA = _rAudMixSndStream[MAX_AUD_MIXSND_STREAM_NUM-1].u4AFifoEA;

    if (!AUD_DspIsMixSndPlay())
    {
        AUD_DspSetMixSndLength(0);
        AUD_DspMixSndControl2(1);
        while (!AUD_DspIsMixSndPlay())
        {
            x_thread_delay(1);
        }
    }

    Printf("[FeedMixSndThread] Enter\n");
    fgAudFeedMixSndThreadEnable = TRUE;

    while (1)
    {
        UINT8 u1EmptyStreamNum;
        UINT8 u1SRC_Enable = 0;
        UINT8 u1Endian_convert = 0;

        //Check if force stop
        if (_fgForceStopMixSndDma)
        {
            AUD_DspMixSndControl2(8); //disable mixsound
            break;
        }

        //Check if there is stop command need to take care
        for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
        {
            if (_rAudMixSndStream[i].fgStop)
            {
                if (_rAudMixSndStream[i].fgClip == 0)
                {
                    _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4RP =
                        _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4WP =
                        _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4SA;
                }
                else
                {
                    while (_rAudMixSndStream[i].u1ClipReadIndx != _rAudMixSndStream[i].u1ClipWriteIndx)
                    {
                        #ifdef LINUX_TURNKEY_SOLUTION
                        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_AUD_CLIP_DONE, _rAudMixSndStream[i].u1ClipReadIndx,i);
                        LOG(4,"N_S(%d): %d/%d\n", i, _rAudMixSndStream[i].u1ClipReadIndx, _rAudMixSndStream[i].u1ClipWriteIndx);
                        #endif
                        _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size = 0;
                        _rAudMixSndStream[i].u1ClipReadIndx = ((_rAudMixSndStream[i].u1ClipReadIndx+1)%MAX_AUD_MIXSND_CLIP_NUM);
                    }
                }
                _rAudMixSndStream[i].fgEnable = 0;
                _rAudMixSndStream[i].fgPause = 0;
                _rAudMixSndStream[i].u4Residual = 0;
                _rAudMixSndStream[i].fgStop = 0;
            }
        }

        for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA;i++)
        {
            if (_rAudMixSndStream[i+ALSA_MIXSND_STREAM_ID].fgClip == 0)
            {
            _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+i].u4Residual =
                ((_rAudMixSndRingFifo[i].u4WP >= _rAudMixSndRingFifo[i].u4RP) ?
                    (_rAudMixSndRingFifo[i].u4WP - _rAudMixSndRingFifo[i].u4RP) :
                    (_rAudMixSndRingFifo[i].u4SZ - _rAudMixSndRingFifo[i].u4RP + _rAudMixSndRingFifo[i].u4WP));
            _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+i].u4StreamAddr = _rAudMixSndRingFifo[i].u4RP;
        }
        }

        //Check if there is data need to send
        u1EmptyStreamNum = 0;
        for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
        {
            if (_rAudMixSndStream[i].fgEnable)
            {
                if (_rAudMixSndStream[i].u4Residual == 0)
                {
                    //check if there is new clip
                    if ((_rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size)
                        && (_rAudMixSndStream[i].fgClip))
                    {
                        _rAudMixSndStream[i].u4Residual = (_rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size & 0xffffff00); //Clipping to align multiple of 0x100
                        _rAudMixSndStream[i].u4StreamAddr = _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Addr;
                        _rAudMixSndStream[i].u4SampleRate = _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4SampleRate;
                        _rAudMixSndStream[i].u1MixSndStereoOnOff = _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u1StereoOnOff;
                        _rAudMixSndStream[i].u1BitDepth = _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u1BitDepth;
                        _rAudMixSndStream[i].u1Endian = _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u1Endian;

                        if (_rAudMixSndStream[i].u4Residual == 0)
                        {
                            LOG(4,"_AudFeedMixSndThread: clip size < 0x100, clip and notify\n");
                        #ifdef LINUX_TURNKEY_SOLUTION
                            pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_AUD_CLIP_DONE, _rAudMixSndStream[i].u1ClipReadIndx,i);
                            LOG(4,"N_C(%d): %d/%d\n", i, _rAudMixSndStream[i].u1ClipReadIndx, _rAudMixSndStream[i].u1ClipWriteIndx);
                        #endif
                            _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size = 0;
                            _rAudMixSndStream[i].u1ClipReadIndx = ((_rAudMixSndStream[i].u1ClipReadIndx+1)%MAX_AUD_MIXSND_CLIP_NUM);
                            i--;
                        }                        
                    }
                    else
                    {
                        u1EmptyStreamNum++;
                    }
                }
            }
            else
            {
                u1EmptyStreamNum++;
            }
        }

        //Data Empty
        if (u1EmptyStreamNum == MAX_AUD_MIXSND_STREAM_NUM)
        {
            if (!fgDataEmpty)
            {
                fgDataEmpty = TRUE;
                #ifdef TRACK_FIFO
                //Printf("\nMixSnd Thread Underflow\n");
                #endif
            }
            x_thread_delay(1);
            continue;
        }
        else
        {
            fgDataEmpty = FALSE;
        }

        //Send Data
        #ifdef MIXSOUND_MIPS_MEASURE
        PARSER_WRITE32(REG_A_STC_1, 0);
        #endif
        u1EmptyStreamNum = 0;
        for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
        {
            if ((_rAudMixSndStream[i].u4Residual >= MIXSND_TRANSFER_SZ) && (!_rAudMixSndStream[i].fgPause))
            {
                BOOL fgTransfer = FALSE;

                _rAudMixSndStream[i].u4TransferSZ = (_rAudMixSndStream[i].u4Residual > MIXSND_TRANSFER_SZ) ? MIXSND_TRANSFER_SZ : _rAudMixSndStream[i].u4Residual;

                if ((_rAudMixSndStream[i].u1Endian) && (_rAudMixSndStream[i].u1BitDepth == 16))
                {
                    u1Endian_convert = 1;
                }
                else
                {
                    u1Endian_convert = 0;
                }
                
                if ((_rAudMixSndStream[i].u4SampleRate == 48000) && (_rAudMixSndStream[i].u1MixSndStereoOnOff == 1)
                     &&(_rAudMixSndStream[i].u1BitDepth == 16))
                {
                    _rAudMixSndStream[i].u4TransferSZ2 = _rAudMixSndStream[i].u4TransferSZ;                    
                    u1SRC_Enable = 0;
                }
                else
                {
                    _rAudMixSndStream[i].u4TransferSZ2 =
                        (((_rAudMixSndStream[i].u4TransferSZ/(_rAudMixSndStream[i].u1BitDepth/8) * 48000L / _rAudMixSndStream[i].u4SampleRate) + 1)&0xfffffffe) *
                        (2 - _rAudMixSndStream[i].u1MixSndStereoOnOff) * 2;
                    u1SRC_Enable = 1;
                    if (((u4AudMixSndDbgMsk & 0x1)==0) && (_rAudMixSndStream[i].u4SampleRate != 48000))
                    {
                        _rAudMixSndStream[i].u4TransferSZ2 += 0x200;
                    }
                }

                if ((i >= ALSA_MIXSND_STREAM_ID) &&
                    ((_rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4RP + _rAudMixSndStream[i].u4TransferSZ) > _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4EA))
                {
                    x_memcpy((VOID*)(VIRTUAL(_rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4EA)),
                             (VOID*)(VIRTUAL(_rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4SA)),
                             (_rAudMixSndStream[i].u4TransferSZ - (_rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4EA - _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4RP)));
                }

                if (fgIsSoftTransferable(i))
                {
           
                    if (u1Endian_convert == 1)
                    {
                        do_BigEnd_to_LittleEnd((INT16*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)), _rAudMixSndStream[i].u4TransferSZ/2);
                    }
                    
                    if (u1SRC_Enable == 1)
                    {
                        if (_rAudMixSndStream[i].u1BitDepth == 16)
                        {
                            if ((u4AudMixSndDbgMsk & 0x1)==0)
                            {
                                do_SRC_S16LE_v2(i,
                                    (INT16*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)),
                                    (INT16*)(VIRTUAL(u4AFifoTA)),
                                    _rAudMixSndStream[i].u4SampleRate,
                                    48000,
                                    _rAudMixSndStream[i].u4TransferSZ/2,
                                    _rAudMixSndStream[i].u1MixSndStereoOnOff);
                            }
                            else
                            {
                                do_SRC_S16LE(i,
                                        (INT16*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)),
                                        (INT16*)(VIRTUAL(u4AFifoTA)),
                                        _rAudMixSndStream[i].u4SampleRate,
                                        48000,
                                        _rAudMixSndStream[i].u4TransferSZ/2, //bit 16
                                        _rAudMixSndStream[i].u1MixSndStereoOnOff);
                            }
                        }
                        else
                        {
                            if ((u4AudMixSndDbgMsk & 0x1)==0)
                            {
                                do_SRC_U8_v2(i,
                                        (UINT8*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)),
                                        (INT16*)(VIRTUAL(u4AFifoTA)),
                                        _rAudMixSndStream[i].u4SampleRate,
                                        48000,
                                        _rAudMixSndStream[i].u4TransferSZ, //bit 8
                                        _rAudMixSndStream[i].u1MixSndStereoOnOff);

                            }
                            else
                            {
                                do_SRC_U8(i,
                                        (UINT8*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)),
                                        (INT16*)(VIRTUAL(u4AFifoTA)),
                                        _rAudMixSndStream[i].u4SampleRate,
                                        48000,
                                        _rAudMixSndStream[i].u4TransferSZ, //bit 8
                                        _rAudMixSndStream[i].u1MixSndStereoOnOff);
                            }
                        }
                    }
                    else
                    {
                        x_memcpy((VOID*)(VIRTUAL(u4AFifoTA)), (VOID*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)), _rAudMixSndStream[i].u4TransferSZ * 2);
                    }
                    fgTransfer = TRUE;
                    #ifdef TRACK_FIFO
                    //Printf("S");
                    #endif
                }

                if (!fgTransfer)
                {
                    //if fifo full
                    u1EmptyStreamNum++;
					LOG(1,"[AudMixSndStream] data can not vSoftTransfer\n");
                }
                else
                {
                    {
#if defined(ALSA_AMIXER_PATH) || defined(ALSA_APROC_PATH) 
                        LOG(3," _rAudMixSndStream[%d].u4Wp=%d, 123=%d, Dest=0x%x\n", i, _rAudMixSndStream[i].u4Wp, u4LoopCnt123, _rAudMixSndStream[i].u4DesAddr);
#endif

                   vSoftTransfer(u4AFifoTA, _rAudMixSndStream[i].u4TransferSZ2,_rAudMixSndStream[i].u4DesAddr,
                                               _rAudMixSndStream[i].u4AFifoSA, _rAudMixSndStream[i].u4AFifoEA,i);

#ifdef ALSA_AMIXER_PATH
                    // Wp  * Block Size
                    _rAudMixSndStream[i].u4DesAddr = _rAudMixSndStream[i].u4AFifoSA 
                               + (_rAudMixSndStream[i].u4Wp*(256*4)) + ((u4LoopCnt456%SAMPLE_UNIT)*(64*4));
                    u4LoopCnt456++;
                    if (u4LoopCnt456 == SAMPLE_UNIT)  u4LoopCnt456 = 0;
#elif defined(ALSA_APROC_PATH)
          #ifdef ALSA_PCMDEC_PATH
                    if (i == ALSA_MIXSND_STREAM_ID+ALSA_DSPDEC_ID)
                    {
                        _rAudMixSndStream[i].u4DesAddr = ((_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) < _rAudMixSndStream[i].u4AFifoEA) ?
                                        (_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) :
                                        ((_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) - _rAudMixSndStream[i].u4AFifoSZ);
                    }
                    else
          #endif
          	        {

					if (i >= ALSA_MIXSND_STREAM_ID)
					{
	                    _rAudMixSndStream[i].u4DesAddr = _rAudMixSndStream[i].u4AFifoSA + 
							(_rAudMixSndStream[i].u4Wp*(768*4)) + ((768-u4LoopCnt123[i-ALSA_MIXSND_STREAM_ID])*4); 
				
					}
          	         }
#else
                        _rAudMixSndStream[i].u4DesAddr = ((_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) < _rAudMixSndStream[i].u4AFifoEA) ?
                                        (_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) :
                                        ((_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) - _rAudMixSndStream[i].u4AFifoSZ);
#endif
                    }

                    #ifdef MIXSND_OUTPUT_DBG
                    if (i == u1DumpStreamId)
                    {
                        if (u4TotalOutputSize < u4MixSndDumpSize)
                        {
                            if (u1DumpLocation == 1)
                            {
                                //After SW ASRC
                                if (u4TotalOutputSize + _rAudMixSndStream[i].u4TransferSZ2 < u4MixSndDumpSize)
                                {
                                    x_memcpy((VOID*)(VIRTUAL(u4OutputBuf)), (VOID*)(VIRTUAL(u4AFifoTA)), _rAudMixSndStream[i].u4TransferSZ2);
                                    u4OutputBuf += _rAudMixSndStream[i].u4TransferSZ2;
                                    u4TotalOutputSize += _rAudMixSndStream[i].u4TransferSZ2;
                                }
                                else
                                {
                                    x_memcpy((VOID*)(VIRTUAL(u4OutputBuf)), (VOID*)(VIRTUAL(u4AFifoTA)), u4MixSndDumpSize - u4TotalOutputSize);
                                    u4OutputBuf = u4MixSndDumpBufAddr + u4MixSndDumpSize;
                                    u4TotalOutputSize = u4MixSndDumpSize;
                                    Printf("After SW ASRC Dump End. please check %08x~%08x\n", u4MixSndDumpBufAddr, u4MixSndDumpBufAddr + u4MixSndDumpSize - 1);
                                }
                            }
                            else
                            {
                                //Before SW ASRC
                                if (u4TotalOutputSize + _rAudMixSndStream[i].u4TransferSZ < u4MixSndDumpSize)
                                {
                                    x_memcpy((VOID*)(VIRTUAL(u4OutputBuf)), (VOID*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)), _rAudMixSndStream[i].u4TransferSZ);
                                    u4OutputBuf += _rAudMixSndStream[i].u4TransferSZ;
                                    u4TotalOutputSize += _rAudMixSndStream[i].u4TransferSZ;
                                }
                                else
                                {
                                    x_memcpy((VOID*)(VIRTUAL(u4OutputBuf)), (VOID*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)), u4MixSndDumpSize - u4TotalOutputSize);
                                    u4OutputBuf = u4MixSndDumpBufAddr + u4MixSndDumpSize;
                                    u4TotalOutputSize = u4MixSndDumpSize;
                                    Printf("Before SW SRC Dump End. please check %08x~%08x\n", u4MixSndDumpBufAddr, u4MixSndDumpBufAddr + u4MixSndDumpSize - 1);
                                }
                            }
                        }
                    }
                    #endif

                    if (_rAudMixSndStream[i].fgClip == 0)
                    {
                        //Update RingFifo RP
                        _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4RP =
                            (_rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4RP + _rAudMixSndStream[i].u4TransferSZ < _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4EA) ?
                            (_rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4RP + _rAudMixSndStream[i].u4TransferSZ) :
                            (_rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4RP + _rAudMixSndStream[i].u4TransferSZ - _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4SZ);

                        _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4TotalRenderSize += _rAudMixSndStream[i].u4TransferSZ;
                        _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4TotalRenderSize2 += _rAudMixSndStream[i].u4TransferSZ2;
						_rAudMixSndStream[i].u4Residual -= _rAudMixSndStream[i].u4TransferSZ;
						
                       LOG(3,"Update RingFifo RP:  _rAudMixSndRingFifo[%d].u4RP=%x \n", i, _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4RP);					   
                    }
                    else
                    {
                        _rAudMixSndStream[i].u4Residual -= _rAudMixSndStream[i].u4TransferSZ;
                        _rAudMixSndStream[i].u4StreamAddr += _rAudMixSndStream[i].u4TransferSZ;
                        if (_rAudMixSndStream[i].u4Residual == 0) 
                        {
                            if (_rAudMixSndStream[i].fgRepeat)
                            {
                                _rAudMixSndStream[i].fgRepeat --;
                            }
                          
                            if (_rAudMixSndStream[i].fgRepeat == 0)
                            {
                                #ifdef LINUX_TURNKEY_SOLUTION
                                pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_AUD_CLIP_DONE, _rAudMixSndStream[i].u1ClipReadIndx,i);
                                LOG(4,"N_C(%d): %d/%d\n", i, _rAudMixSndStream[i].u1ClipReadIndx, _rAudMixSndStream[i].u1ClipWriteIndx);
                                #endif
                                _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size = 0;
                                _rAudMixSndStream[i].u1ClipReadIndx = ((_rAudMixSndStream[i].u1ClipReadIndx+1)%MAX_AUD_MIXSND_CLIP_NUM);
                            }
                        }
                    }
                }
            }
            else
            {
                u1EmptyStreamNum++;
            }
        }

        //FIFO Full
        if (u1EmptyStreamNum == MAX_AUD_MIXSND_STREAM_NUM)
        {
            #ifdef TRACK_FIFO
            //Printf("F\n");
            #endif
            x_thread_delay(1);
        }
        #ifdef MIXSOUND_MIPS_MEASURE
        else
        {
            u4MixSndMIPS += PARSER_READ32(REG_A_STC_1);
            u4MixSndSendCnt++;
        }
        #endif
    }

    //flush clip pointer
    for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
    {
        if (_rAudMixSndStream[i].fgClip)
        {
            while (_rAudMixSndStream[i].u1ClipReadIndx != _rAudMixSndStream[i].u1ClipWriteIndx)
            {
                #ifdef LINUX_TURNKEY_SOLUTION
                pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_AUD_CLIP_DONE, _rAudMixSndStream[i].u1ClipReadIndx,i);
                LOG(4,"N_F(%d): %d/%d\n", i, _rAudMixSndStream[i].u1ClipReadIndx, _rAudMixSndStream[i].u1ClipWriteIndx);
                #endif
                _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size = 0;
                _rAudMixSndStream[i].u1ClipReadIndx = ((_rAudMixSndStream[i].u1ClipReadIndx+1)%MAX_AUD_MIXSND_CLIP_NUM);
            }
        }
    }

    #ifndef NEW_MIXSOUND
    AUD_DspSetMixSndGain(u4GainOld); //FLASHLITE_CONFLICT
    vWriteShmUINT16(W_MIXSOUND_CFG,u2MixSndCfg); //FLASHLITE_CONFLICT
    #endif
    _hAudFeedMixSndThread = NULL_HANDLE;
    _fgForceStopMixSndDma = FALSE;
    fgAudFeedMixSndThreadEnable = FALSE;

    #ifdef LINUX_TURNKEY_SOLUTION
    pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_CTRL_DONE, AUD_DEC_CTRL_STOP, AUD_OK);
    LOG(4,"N_AUD_COND_CTRL_DONE\n");
    #endif

    Printf("[FeedMixSndThread] Exit\n");
}

#ifndef NEW_MIXSOUND
#ifdef CC_AUD_MIXSOUND_SUPPORT
extern BOOL _fgMixSndFlag;
extern UINT8 _u1MixSndClipIdx;
#endif
#endif

#ifdef CC_SUPPORT_STR
void AUD_MixsndResume(void)
{
    LOG(5, "Trigger Mixsnd Resume!");

    #ifdef NEW_MIXSOUND
    if (!AUD_DspIsMixSndPlay())
    {
        AUD_DspSetMixSndLength(0);
        AUD_DspMixSndControl2(1);
        while (!AUD_DspIsMixSndPlay())
        {
            x_thread_delay(1);
        }
    }
    #else
    {
        //Pre-Init
        Printf("[FeedMixSndThread] Init MixSnd DSP\n");
        u4GainOld = u4ReadShmUINT32(D_MIXSOUND_GAIN); //FLASHLITE_CONFLICT
        u2MixSndCfg = u2ReadShmUINT16(W_MIXSOUND_CFG); //FLASHLITE_CONFLICT

        AUD_DspSetMixSndLength(0x10000);
        AUD_DspSetMixSndGain(0x20000);
        AUD_DspSetMixSndStereo(TRUE);
        AUD_DspSetMixSndUpSampleRatio(0);
        AUD_DspSetMixSndMemPlay(TRUE);
        //AUD_DspMixSndControl(1);
        _AUD_DspMixSndControl2(1); //FLASHLITE_CONFLICT
        while (!AUD_DspIsMixSndPlay())
        {
            x_thread_delay(1);
        }
    }
    #endif
}
#endif

//-----------------------------------------------------------------------------
/** AUD_EnableMixSndClip
 *
 *  @param void
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_EnableMixSndClip(void)
{
    if (_hAudFeedMixSndThread)
    {
        return;
    }

    if (OSR_OK != x_thread_create(&_hAudFeedMixSndThread,
                                   AUD_DRV_FEEDMIXSND_THREAD_NAME,
                                   AUD_DRV_THREAD_STACK_SIZE,
                                   u1MixSndThread_Priority, //AUD_DRV_THREAD_PRIORITY, // 2011/3/29, by Daniel, Zink said for Skype integration, it needs to set _AudFeedMixSndThread priority to 50 to prevent from data copy delay
                                   (x_os_thread_main_fct) _AudFeedMixSndThread,
                                   0,
                                   NULL))
    {
        LOG(5, "[AUD_EnableMixSndClip] !!!!Create _AudFeedMixSndThread Fail!!!!\n");
    }
    else
    {
        LOG(5, "[AUD_EnableMixSndClip] !!!!Create _AudFeedMixSndThread Succ!!!!\n");
        while (!fgAudFeedMixSndThreadEnable) //for CMPB integration
        {
            x_thread_delay(1);
        }
    }
}

//-----------------------------------------------------------------------------
/** AUD_DisableMixSndClip
 *
 *  @param void
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DisableMixSndClip(void)
{
    if (_hAudFeedMixSndThread)
    {
        LOG(5, "[AUD_DisableMixSndClip] trigger\n");
        _fgForceStopMixSndDma = TRUE;
        while (!_fgForceStopMixSndDma);
        x_thread_delay(1);
    }
    else
    {
        LOG(5, "[AUD_DisableMixSndClip] _hAudFeedMixSndThread not available\n");
    }
}

//-----------------------------------------------------------------------------
/** AUD_PlayMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_PlayMixSndClip(UINT8 u1StreamID)
{
    UINT8 u1StrDd = u1StreamID + ALSA_MIXSND_STREAM_ID;
    UINT8 u1MixId = _u1AlsaAprocMixsndId[u1StreamID]; 
    
    if (_hAudFeedMixSndThread)
    {
        AUD_AprocMixSndInfoInit(_u1AlsaAprocMixsndId[u1StreamID]);

        _rAudMixSndStream[u1StrDd].u4AFifoSA = _gsAprocMixSndBufInfo[u1MixId].u4Str;
        _rAudMixSndStream[u1StrDd].u4AFifoSZ = _gsAprocMixSndBufInfo[u1MixId].u4End*
            _gsAprocMixSndBufInfo[u1MixId].u4Size*_gsAprocMixSndBufInfo[u1MixId].u4Ch;      
        _rAudMixSndStream[u1StrDd].u4AFifoEA = _rAudMixSndStream[u1StrDd].u4AFifoSA+
            _rAudMixSndStream[u1StrDd].u4AFifoSZ;
        _rAudMixSndStream[u1StrDd].u4DesAddr = _rAudMixSndStream[u1StrDd].u4AFifoSA;
        
        Printf("\n SA=0x%x, SZ=%d, EA=0x%x, DestAddr=0x%x\n", _rAudMixSndStream[u1StrDd].u4AFifoSA, 
           _rAudMixSndStream[u1StrDd].u4AFifoSZ, _rAudMixSndStream[u1StrDd].u4AFifoEA,
           _rAudMixSndStream[u1StrDd].u4DesAddr);

        u4LoopCnt123[u1StreamID]  = SAMPLE_UNIT;
        u4LoopCnt456[u1StreamID]  = 0;
        _rAudMixSndStream[u1StrDd].fgClip = 1;
        
        // Request ARM9 to reset mixsound buffer info.
        AUD_AprocMixSndReset(u1MixId);
        // Send MixSound Enable Command to ARM9    
        x_thread_delay(20);        
        AUD_AprocMixSndEnable(_u1AlsaAprocMixsndId[u1StreamID], TRUE);

        _rAudMixSndStream[u1StrDd].fgEnable = 1;
        _rAudMixSndStream[u1StrDd].i2DelayBuf[0] = 0;
        _rAudMixSndStream[u1StrDd].i2DelayBuf[1] = 0;

        _rAudMixSndStream[u1StrDd].segEnd[0] = 0;
        _rAudMixSndStream[u1StrDd].segEnd[1] = 0;
        _rAudMixSndStream[u1StrDd].segStart[0] = 0;
        _rAudMixSndStream[u1StrDd].segStart[1] = 0;
        _rAudMixSndStream[u1StrDd].t_out = 0;
        _rAudMixSndStream[u1StrDd].t_in = -1;

        
        LOG(5, "[AUD_PlayMixSndClip] trigger %d\n", u1StreamID);
    }
    else
    {
        LOG(5, "[AUD_PlayMixSndClip] _hAudFeedMixSndThread not available\n");
    }
}

BOOL AUD_IsMixSndClipPlay(UINT8 u1StreamID)
{
    u1StreamID += ALSA_MIXSND_STREAM_ID; 
    return (BOOL)(_rAudMixSndStream[u1StreamID].fgEnable);
}

//-----------------------------------------------------------------------------
/** AUD_StopMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_StopMixSndClip(UINT8 u1StreamID)
{
    u1StreamID += ALSA_MIXSND_STREAM_ID;

    if (_hAudFeedMixSndThread)
    {
        _rAudMixSndStream[u1StreamID].fgClip = 0; 
        _rAudMixSndStream[u1StreamID].fgStop = 1;
        LOG(5, "[AUD_StopMixSndClip] trigger %d\n", u1StreamID);
    }
    else
    {
        LOG(5, "[AUD_StopMixSndClip] _hAudFeedMixSndThread not available\n");
    }

    while (_rAudMixSndStream[u1StreamID].fgStop)
    {
        x_thread_delay(1);
    }

    {
      AudDecNfyFct pfAudDecNfy = NULL;

      VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));

      #ifdef LINUX_TURNKEY_SOLUTION
      pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_CTRL_DONE, AUD_DEC_CTRL_STOP, AUD_OK);
      LOG(4,"N_AUD_COND_CTRL_DONE\n");
      #endif
    }
}

//-----------------------------------------------------------------------------
/** AUD_PauseMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_PauseMixSndClip(UINT8 u1StreamID)
{
    u1StreamID += ALSA_MIXSND_STREAM_ID;

    if (_hAudFeedMixSndThread)
    {
        _rAudMixSndStream[u1StreamID].fgPause = 1;
        LOG(0, "[AUD_PauseMixSndClip] trigger %d\n",u1StreamID);
    }
    else
    {
        LOG(0, "[AUD_PauseMixSndClip] _hAudFeedMixSndThread not available\n");
    }
}

//-----------------------------------------------------------------------------
/** AUD_ResumeMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_ResumeMixSndClip(UINT8 u1StreamID)
{
    u1StreamID += ALSA_MIXSND_STREAM_ID;

    if (_hAudFeedMixSndThread)
    {
        _rAudMixSndStream[u1StreamID].fgPause = 0;
        LOG(0, "[AUD_ResumeMixSndClip] trigger %d\n",u1StreamID);
    }
    else
    {
        LOG(0, "[AUD_ResumeMixSndClip] _hAudFeedMixSndThread not available\n");
    }
}

//-----------------------------------------------------------------------------
/** AUD_RepeatMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_RepeatMixSndClip(UINT8 u1StreamID, UINT32 u1RepeatNum)
{
    if (_hAudFeedMixSndThread)
    {
        _rAudMixSndStream[u1StreamID + ALSA_MIXSND_STREAM_ID].fgRepeat = u1RepeatNum;
        LOG(5, "[AUD_RepeatMixSndClip] trigger %d, u1RepeatNum %d\n", u1StreamID, u1RepeatNum);
    }
    else
    {
        LOG(5, "[AUD_RepeatMixSndClip] _hAudFeedMixSndThread not available\n");
    }
}

//-----------------------------------------------------------------------------
/** AUD_SetMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @param u4Addr               MixSound Clip Memory Physical Address
 *  @param u4Size               MixSound Clip Size (in byte)
 *  @param u4Gain               MixSound Clip Gain (0db => 0x20000)
 *  @param u1StereoOnOff        MixSound Clip Mono or Stereo (FALSE: mono, TRUE: stereo)
 *  @param u4SampleRate         MixSound Clip Sampling Rate (48000/44100/32000 ....)
 *  @param u1BitDepth           MixSound Clip Bit Depth (8/16)
 *  @retval BOOL                TRUE - set succ, FALSE - set fail
 */
//-----------------------------------------------------------------------------
BOOL AUD_SetMixSndClip(UINT8 u1StreamID, UINT32 u4Addr, UINT32 u4Size, UINT32 u4Gain, UINT8 u1StereoOnOff, UINT32 u4SampleRate, UINT8 u1BitDepth, UINT8 u1Endian)
{
    UINT8 u1StrDd = u1StreamID + ALSA_MIXSND_STREAM_ID;

    if (_hAudFeedMixSndThread)
    {
        if (_rAudMixSndStream[u1StrDd].u1ClipWriteIndx<MAX_AUD_MIXSND_CLIP_NUM)
        {
            if (((_rAudMixSndStream[u1StrDd].u1ClipWriteIndx+1)%MAX_AUD_MIXSND_CLIP_NUM) == _rAudMixSndStream[u1StrDd].u1ClipReadIndx)
            {
                LOG(5, "[AUD_SetMixSndClip] Stream %d Clip Full\n", u1StreamID);
                return FALSE;
            }

            LOG(5, "[AUD_SetMixSndClip] ID: %d u4Addr: 0x%08x Size: 0x%08x Gain: 0x%08x Stereo: %d SampleRate: %d u1BitDepth: %d u1Endian: %d\n",
                    u1StreamID, u4Addr, u4Size, u4Gain, u1StereoOnOff, u4SampleRate, u1BitDepth, u1Endian);

            _rAudMixSndClip[u1StrDd][_rAudMixSndStream[u1StrDd].u1ClipWriteIndx].u4Addr = u4Addr;
            _rAudMixSndClip[u1StrDd][_rAudMixSndStream[u1StrDd].u1ClipWriteIndx].u4Size = u4Size;
            _rAudMixSndClip[u1StrDd][_rAudMixSndStream[u1StrDd].u1ClipWriteIndx].u4Gain = u4Gain;
            _rAudMixSndClip[u1StrDd][_rAudMixSndStream[u1StrDd].u1ClipWriteIndx].u1StereoOnOff = u1StereoOnOff;
            _rAudMixSndClip[u1StrDd][_rAudMixSndStream[u1StrDd].u1ClipWriteIndx].u4SampleRate = u4SampleRate;
            _rAudMixSndClip[u1StrDd][_rAudMixSndStream[u1StrDd].u1ClipWriteIndx].u1BitDepth= u1BitDepth;
            _rAudMixSndClip[u1StrDd][_rAudMixSndStream[u1StrDd].u1ClipWriteIndx].u1Endian= u1Endian;
            _rAudMixSndStream[u1StrDd].u1ClipWriteIndx = ((_rAudMixSndStream[u1StrDd].u1ClipWriteIndx+1)%MAX_AUD_MIXSND_CLIP_NUM);

            return TRUE;
        }
        else
        {
            LOG(5, "[AUD_SetMixSndClip] Stream %d Error\n", u1StreamID);
            return FALSE;
        }
    }
    else
    {
        LOG(5, "[AUD_SetMixSndClip] _hAudFeedMixSndThread not available\n");
        return FALSE;
    }
 }

extern UINT32 u4GetMixSndGain(UINT8 u1StreamId);
void AUD_QueryMixSndClip(void)
{
    UINT8 i;

    if (_hAudFeedMixSndThread)
    {
        Printf("_hAudFeedMixSndThread ongoing\n");
        #ifdef MIXSND_OUTPUT_DBG
        Printf("u4OutputBuf: 0x%08x/0x%08x Size: 0x%08x/0x%08x\n", u4OutputBuf, u4MixSndDumpBufAddr, u4TotalOutputSize, u4MixSndDumpSize); //TEST
        #endif
        Printf("u4AudMixSndDbgMsk: 0x%08x(Addr: 0x%08x)\n", u4AudMixSndDbgMsk, &u4AudMixSndDbgMsk);
        for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
        {
            Printf("MixSound Stream ID: %d\n", i);
            Printf("  fgEnable: %d fgStop: %d fgPause: %d fgRepeat: %d\n",
                    _rAudMixSndStream[i].fgEnable,
                    _rAudMixSndStream[i].fgStop,
                    _rAudMixSndStream[i].fgPause,
                    _rAudMixSndStream[i].fgRepeat);

            if (_rAudMixSndStream[i].fgEnable)
            {
                Printf("  Read Indx: %d Write Indx: %d\n",
                        _rAudMixSndStream[i].u1ClipReadIndx,
                        _rAudMixSndStream[i].u1ClipWriteIndx);
                Printf("  Rate: %d\n", _rAudMixSndStream[i].u4SampleRate);
                Printf("  Stereo: %d\n", _rAudMixSndStream[i].u1MixSndStereoOnOff);
                Printf("  Bit Depth: %d\n", _rAudMixSndStream[i].u1BitDepth);
                Printf("  Endian: %d\n", _rAudMixSndStream[i].u1Endian);
                Printf("  Gain: 0x%x\n", u4GetMixSndGain(i));
                Printf("  SA: 0x%08x EA: 0x%08x CA: 0x%08x\n",
                        _rAudMixSndStream[i].u4AFifoSA,
                        _rAudMixSndStream[i].u4AFifoEA,
                        _rAudMixSndStream[i].u4DesAddr);
              #ifdef TRACK_FIFO
                Printf("  Full: 0x%0x\n", _rAudMixSndStream[i].u4AFifoFullness);
              #endif
              #ifdef ALSA_MIXSND_PATH
                if (i>=ALSA_MIXSND_STREAM_ID)
                {
                    Printf("  RingFifo.SA: 0x%08x\n", _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4SA);
                    Printf("  RingFifo.EA: 0x%08x\n", _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4EA);
                    Printf("  RingFifo.RP: 0x%08x\n", _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4RP);
                    Printf("  RingFifo.WP: 0x%08x\n", _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4WP);
                    Printf("  Full: 0x%x\n", _rAudMixSndStream[i].u4Residual);
                    Printf("  TotalRenderSize: %d\n", _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4TotalRenderSize);
                    Printf("  TotalRenderSize2: %d\n", _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4TotalRenderSize2);                            
                }
              #endif
            }
            else
            {
              #ifdef ALSA_MIXSND_PATH
                if (i>=ALSA_MIXSND_STREAM_ID)
                {
                    Printf("  TotalRenderSize: %d\n", _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4TotalRenderSize);
                    Printf("  TotalRenderSize2: %d\n", _rAudMixSndRingFifo[i-ALSA_MIXSND_STREAM_ID].u4TotalRenderSize2);                    
                }
              #endif
            }
        }
    }
    else
    {
        Printf("_hAudFeedMixSndThread not available\n");
    }
}

extern void DSP_FlushInvalidateDCacheFree(UINT32 u4Addr, UINT32 u4Len);
void AUD_ClearMixSndClip(void)
{
    UINT32 u4SA = VIRTUAL(u4GetMixSoundStartByteAddr2());
    UINT32 u4EA = VIRTUAL(u4GetMixSoundStartByteAddr3() + u4GetMixSoundBufSize3());

    x_memset((VOID*)u4SA, 0x00, u4EA-u4SA);
    DSP_FlushInvalidateDCacheFree(u4SA,u4EA-u4SA);
}

#ifdef ALSA_MIXSND_PATH
#if defined(CC_AUD_MIXSND_FOR_ALSA) || defined(CC_ENABLE_AOMX)
#ifdef  ALSA_PCMDEC_PATH  
void AUD_SetDSPDecSetting(UINT32 u4DspDecType)
{
     AUD_PCM_SETTING_T rPcmSetting;    
     AUD_AC3_SETTING_T rAC3Setting; 

     if (u4DspDecoderType == AUD_FMT_PCM)	
    {
        rPcmSetting.eSampleFreq = FS_48K;
        rPcmSetting.ePcmDataInfo = PCM_16_BITS_LITTLE_ENDIAN;
        AUD_PcmSetting(AUD_DSP0,AUD_DEC_MAIN, (const AUD_PCM_SETTING_T *)&rPcmSetting);
     }
    else if (u4DspDecoderType == AUD_FMT_AC3)	
    {
        rAC3Setting.eEndian = DEC_BIG_ENDIAN;
        AUD_Ac3Setting(AUD_DEC_MAIN, (const AUD_AC3_SETTING_T *)&rAC3Setting);
    }
     else	 
    {
      //TO-DO
	
     }
}
#endif

void AUD_PlayMixSndRingFifo(UINT8 u1StreamId, UINT32 u4SampleRate, UINT8 u1StereoOnOff, UINT8 u1BitDepth, UINT32 u4BufferSize)
{
  #ifdef ALSA_PCMDEC_PATH
    if (u1StreamId == ALSA_DSPDEC_ID) //< -------- Stream ID from offload
    {
        Printf("\AUD_PlayMixSndRingFifo(%d)   #### For DSP Decoder #### Need to PLAY DSP ! \n", u1StreamId);

        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+u1StreamId].u4AFifoSA = u4GetAFIFOStart(AUD_DSP0,AUD_DEC_MAIN); //< -------- Stream ID from offload
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+u1StreamId].u4AFifoEA = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+u1StreamId].u4AFifoSA + ALSA_PCMDEC_PATH_BUFSIZE;
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+u1StreamId].u4AFifoSZ = ALSA_PCMDEC_PATH_BUFSIZE;
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+u1StreamId].u4DesAddr = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID+u1StreamId].u4AFifoSA;
		                	
        skip_create_feed_stream = 1;
        if (AUD_IsDecoderPlay(AUD_DSP0,AUD_DEC_MAIN))
        {
            AUD_DSPCmdStop(AUD_DSP0,AUD_DEC_MAIN);  // ???
        }    
        vDspSetAFIFO1SZ(ALSA_PCMDEC_PATH_BUFSIZE);
        AUD_SetAvSynMode(AUD_DEC_MAIN, AV_SYNC_FREE_RUN);

        AUD_SetDecType(AUD_DSP0,AUD_DEC_MAIN, AUD_STREAM_FROM_MEMORY, u4DspDecoderType);
 	  AUD_SetDSPDecSetting(u4DspDecoderType);			
        AUD_DSPCmdPlay(AUD_DSP0,AUD_DEC_MAIN);
    }

  #endif

    AUD_StopMixSndRingFifo(u1StreamId);
    if (_hAudFeedMixSndThread)
    {
        if (u1StreamId < MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA)
        {
            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].fgEnable = 1;
            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].u4SampleRate = u4SampleRate;
            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].u1MixSndStereoOnOff = u1StereoOnOff;
            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].u1BitDepth = (u1BitDepth&0x3f);
            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].u4DesAddr &= 0xffffff00;//DANIEL_DEBUG

            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].i2DelayBuf[0] = 0;
            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].i2DelayBuf[1] = 0;

            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].segEnd[0] = 0;
            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].segEnd[1] = 0;
            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].segStart[0] = 0;
            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].segStart[1] = 0;
            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].t_out = 0;
            _rAudMixSndStream[u1StreamId+ALSA_MIXSND_STREAM_ID].t_in = -1;

            _rAudMixSndRingFifo[u1StreamId].u4SZ = u4BufferSize;
            _rAudMixSndRingFifo[u1StreamId].u4SA = u4GetMixSoundStartByteAddr3()+u1StreamId*u4GetMixSoundBufSize3()/MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA;
            _rAudMixSndRingFifo[u1StreamId].u4EA = _rAudMixSndRingFifo[u1StreamId].u4SA + _rAudMixSndRingFifo[u1StreamId].u4SZ;
            _rAudMixSndRingFifo[u1StreamId].u4RP = _rAudMixSndRingFifo[u1StreamId].u4WP = _rAudMixSndRingFifo[u1StreamId].u4SA;
            _rAudMixSndRingFifo[u1StreamId].u4Latency = u4Latency;
            _rAudMixSndRingFifo[u1StreamId].u4TotalRenderSize = 0;
            _rAudMixSndRingFifo[u1StreamId].u4TotalRenderSize2 = 0;            
          #ifdef PRINT_TIME_MSG
            audio_mixsnd_log = 1;
          #endif
        }
    }
    #if 0
    else
    {
        LOG(5, "[AUD_PlayMixSndRingFifo] _hAudFeedMixSndThread not available\n");
    }
    #endif
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(AUD_PlayMixSndRingFifo);
#endif

extern void DSP_FlushInvalidateDCacheFree(UINT32 u4Addr, UINT32 u4Len);
void AUD_ClearMixSndRingFifo(UINT8 u1StreamId)
{
    UINT32 u4SA = VIRTUAL(u4GetMixSoundStartByteAddr3() + u1StreamId * u4GetMixSoundBufSize3()/MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA);
    UINT32 u4EA = u4SA + u4GetMixSoundBufSize3()/MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA;

    x_memset((VOID*)u4SA, 0x00, u4EA-u4SA);
    DSP_FlushInvalidateDCacheFree(u4SA,u4EA-u4SA);

    u4SA = VIRTUAL(u4GetMixSoundStartByteAddr2() + (u1StreamId+ALSA_MIXSND_STREAM_ID) * u4GetMixSoundBufSize2()/MAX_AUD_MIXSND_STREAM_NUM);
    u4EA = u4SA + u4GetMixSoundBufSize2()/MAX_AUD_MIXSND_STREAM_NUM;

    x_memset((VOID*)u4SA, 0x00, u4EA-u4SA);
    DSP_FlushInvalidateDCacheFree(u4SA,u4EA-u4SA);
}

void AUD_StopMixSndRingFifo(UINT8 u1StreamId)
{
    if (_hAudFeedMixSndThread)
    {
        if (u1StreamId < MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA)
        {
          if (_rAudMixSndStream[u1StreamId + ALSA_MIXSND_STREAM_ID].fgEnable)
          {
              _rAudMixSndStream[u1StreamId + ALSA_MIXSND_STREAM_ID].fgStop = 1;
              while (_rAudMixSndStream[u1StreamId + ALSA_MIXSND_STREAM_ID].fgStop)
              {
                  x_thread_delay(1);
              }
              AUD_ClearMixSndRingFifo(u1StreamId);
          }
        }
    }
    #if 0
    else
    {
        LOG(5, "[AUD_StopMixSndRingFifo] _hAudFeedMixSndThread not available\n");
    }
    #endif
}

#ifdef CC_AUD_DDI
INT32 AUD_GetMixSndRingFifoId(void)
{
    UINT8 i;

    for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA;i++)
    {
        if (!_rAudMixSndStream[i + ALSA_MIXSND_STREAM_ID].fgOccupied)
        {
            _rAudMixSndStream[i + ALSA_MIXSND_STREAM_ID].fgOccupied = TRUE;
            return (INT32)i;
        }
    }

    return -1;
}
BOOL AUD_GetMixSndRingFifoIdOccupied(UINT8 u1StreamId)
{
    if (u1StreamId < MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA)
    {
        return (BOOL)_rAudMixSndStream[u1StreamId + ALSA_MIXSND_STREAM_ID].fgOccupied;
    }else
    {
        return TRUE;
    }
}

void AUD_FreeMixSndRingFifoId(UINT8 u1StreamId)
{
    _rAudMixSndStream[u1StreamId + ALSA_MIXSND_STREAM_ID].fgOccupied = FALSE;
}

UINT32 AUD_GetMixSndRingFifoRenderSize(UINT8 u1StreamId)
{
    return _rAudMixSndRingFifo[u1StreamId].u4TotalRenderSize;
}

#else
INT32 AUD_GetMixSndRingFifoId(void)
{
    UINT8 i;

    for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA;i++)
    {
        if (!_rAudMixSndStream[i + ALSA_MIXSND_STREAM_ID].fgEnable)
        {
            return (INT32)i;
        }
    }

    return -1;
}

UINT32 AUD_GetMixSndRingFifoRenderSize(UINT8 u1StreamId)
{
    return _rAudMixSndRingFifo[u1StreamId].u4TotalRenderSize;
}
#endif //CC_AUD_DDI

UINT32 AUD_GetMixSndRingFifoBufferLevel(UINT8 u1StreamId)
{
    if ((u1StreamId + ALSA_MIXSND_STREAM_ID) < MAX_AUD_MIXSND_STREAM_NUM)
    {
		_rAudMixSndStream[u1StreamId + ALSA_MIXSND_STREAM_ID].u4Residual =
                ((_rAudMixSndRingFifo[u1StreamId].u4WP >= _rAudMixSndRingFifo[u1StreamId].u4RP) ?
                (_rAudMixSndRingFifo[u1StreamId].u4WP - _rAudMixSndRingFifo[u1StreamId].u4RP) :
                (_rAudMixSndRingFifo[u1StreamId].u4SZ - _rAudMixSndRingFifo[u1StreamId].u4RP + _rAudMixSndRingFifo[u1StreamId].u4WP));
					
        return (_rAudMixSndStream[u1StreamId + ALSA_MIXSND_STREAM_ID].u4Residual);
    }
    else
    {
        return 0;
    }
}



#ifdef  ALSA_PCMDEC_PATH  
void AUD_SetDSPDecoderType(AUD_FMT_T u4DspDecType)
{
    u4DspDecoderType = u4DspDecType;
  LOG(3," Enter AUD_SetDSPDecoderType(): u4DspDecoderType=%x\n",u4DspDecoderType);	
}
#endif

#endif //ALSA_MIXSND_PATH
#endif

#ifdef LINUX_TURNKEY_SOLUTION
#include <linux/module.h>

#if defined(CC_AUD_MIXSND_FOR_ALSA) || defined(CC_ENABLE_AOMX)
static UINT8 u1InitALSAPlayback_MixSnd_flag = 0;
void AUD_InitALSAPlayback_MixSnd(UINT8 u1StreamId)
{
	if (u1InitALSAPlayback_MixSnd_flag == 0)
	{
	    AUD_Init();
	    AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
	    AUD_OutPadEnable(AUD_DEC_AUX, TRUE);
	    ADAC_Mute(FALSE);
		u1InitALSAPlayback_MixSnd_flag = 1;
	}

#ifdef ALSA_MIXSND_PATH
    AUD_EnableMixSndClip();
#else
    AUD_EnableMixSndClip();
    AUD_PlayMixSndClip(0);
#endif

#ifdef ALSA_PCMDEC_PATH
    if (u1StreamId == ALSA_DSPDEC_ID) //< -------- Stream ID from offload
    {
        Printf("\nAUD_InitALSAPlayback_MixSnd(%d)   #### For DSP Decoder #### do-nothing ! \n", u1StreamId);	
    }
    else
    {
#endif     


 
 #ifdef ALSA_AMIXER_PATH
    Printf("\nAUD_InitALSAPlayback_MixSnd(%d)\n", u1StreamId);
    if (u1StreamId == 0)
    {

        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoSA = _u4DspCmptBuf[TYPE_APROC_IDX] + 
                                 u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_IOBUF_0, APROC_REG_IOBUF_SWDEC0_START));
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoEA = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoSA+APROC_ADSP_IOBUF_SWDEC_SIZE;
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoSZ = APROC_ADSP_IOBUF_SWDEC_SIZE;
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4DesAddr = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoSA;
    
        //Trigger Amixer0 play
        vAprocDec_Open(AudDecId2MixerId(AUD_DEC_4TH));
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4Wp = 1;
        _gsAprocSwDecBufInfo[ALSA_SWDEC_ID].u4Rp = 0;

        u4LoopCnt123  = SAMPLE_UNIT;
        u4LoopCnt456  = 0;
    }
 #elif defined(ALSA_APROC_PATH)
    Printf("\n AUD_InitALSAPlayback_MixSnd(%d)##### For Aproc SW Mixsnd\n", u1StreamId);
 	if (u1StreamId < MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA)
    {
        // Init Mixsound for CA9
        AUD_AprocMixSndInfoInit(_u1AlsaAprocMixsndId[u1StreamId]);

		if(_u1AlsaAprocMixsndId[u1StreamId] >= AUD_MIXSND_NUM )
		{
			Printf("No available Mixsnd Path for play !!!!");
			return;	
		}
		
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].u4AFifoSA =/* (UINT8 *)*/_gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1StreamId]].u4Str;
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].u4AFifoSZ = _gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1StreamId]].u4End*
			_gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1StreamId]].u4Size*_gsAprocMixSndBufInfo[_u1AlsaAprocMixsndId[u1StreamId]].u4Ch;      
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].u4AFifoEA = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].u4AFifoSA+
			_rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].u4AFifoSZ;
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].u4DesAddr = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].u4AFifoSA;
		
        Printf("\n SA=0x%x, SZ=%d, EA=0x%x, DestAddr=0x%x\n", _rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].u4AFifoSA, 
           _rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].u4AFifoSZ, _rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].u4AFifoEA,
           _rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].u4DesAddr);
        
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID + u1StreamId].fgOccupied = TRUE;

        u4LoopCnt123[u1StreamId]  = SAMPLE_UNIT;
        u4LoopCnt456[u1StreamId]  = 0;
        
        // Request ARM9 to reset mixsound buffer info.
        AUD_AprocMixSndReset(_u1AlsaAprocMixsndId[u1StreamId]);
        // Send MixSound Enable Command to ARM9    
        x_thread_delay(20);
		AUD_AprocMixSndEnable(_u1AlsaAprocMixsndId[u1StreamId], TRUE); 
    }
#ifdef ALSA_PCMDEC_PATH
	}
#endif	
#endif  
}
EXPORT_SYMBOL(AUD_InitALSAPlayback_MixSnd);

void AUD_DeInitALSAPlayback_MixSnd(UINT8 u1StreamId)
{
#ifdef ALSA_MIXSND_PATH
    AUD_StopMixSndRingFifo(u1StreamId);
#else
    AUD_StopMixSndClip(0);
    AUD_DisableMixSndClip();
#endif

#ifdef ALSA_PCMDEC_PATH
  
    if (u1StreamId == ALSA_DSPDEC_ID) //< -------- Stream ID from offload
    {
       Printf("\AUD_DeInitALSAPlayback_MixSnd(%d)  #### For DSP Decoder\n", u1StreamId);    
        if (AUD_IsDecoderPlay(AUD_DSP0,AUD_DEC_MAIN))
        {
            AUD_DSPCmdStop(AUD_DSP0,AUD_DEC_MAIN);  
        }    
        skip_create_feed_stream = 0;
        vDspSetAFIFO1SZ(0); // In dsp_intf.c
    }
    else
    {
#endif
 #ifdef ALSA_AMIXER_PATH
    Printf("\AUD_DeInitALSAPlayback_MixSnd(%d)\n", u1StreamId);
    if (u1StreamId == 0)
    {
        vAprocDec_Close(AudDecId2MixerId(AUD_DEC_4TH));
    }
 #elif defined(ALSA_APROC_PATH)
    Printf("\n AUD_DeInitALSAPlayback_MixSnd(%d)##### For Aproc SW Mixsnd\n", u1StreamId);
    if (u1StreamId < MAX_AUD_MIXSND_STREAM_NUM_FOR_ALSA)
    {
		if(_u1AlsaAprocMixsndId[u1StreamId] >= AUD_MIXSND_NUM )
		{
			Printf("No available Mixsnd Path for play !!!!");
			return;	
		}
        AUD_AprocMixSndEnable(_u1AlsaAprocMixsndId[u1StreamId], FALSE);
    }    
#endif
#ifdef ALSA_PCMDEC_PATH
    	}
#endif

}
EXPORT_SYMBOL(AUD_DeInitALSAPlayback_MixSnd);
#endif

#ifdef CC_AUD_MIXSND_FOR_ALSA
void AUD_InitALSARecordSpeaker(void)
{
    AUD_DspSetUploadMode(4);
    AUD_DspUploadDataEnable();
}
EXPORT_SYMBOL(AUD_InitALSARecordSpeaker);

void AUD_DeInitALSARecordSpeaker(void)
{
    AUD_DspSetUploadMode(0);
    AUD_DspUploadDataEnable();
}
EXPORT_SYMBOL(AUD_DeInitALSARecordSpeaker);

extern UINT32 u4GetUploadFIFOStart(void);
UINT32 AUD_GetUploadFIFOStart(void)
{
    return u4GetUploadFIFOStart();
}
EXPORT_SYMBOL(AUD_GetUploadFIFOStart);

extern UINT32 u4GetUploadFIFOEnd(void);
UINT32 AUD_GetUploadFIFOEnd(void)
{
    return u4GetUploadFIFOEnd();
}
EXPORT_SYMBOL(AUD_GetUploadFIFOEnd);

extern UINT32 u4GetUploadWritePnt(void);
UINT32 AUD_GetUploadWritePnt(void)
{
    return u4GetUploadWritePnt();
}
EXPORT_SYMBOL(AUD_GetUploadWritePnt);

extern UINT32 u4GetMixSndReadPtr(UINT8 u1StreamId);
UINT32 AUD_GetMixSndReadPtr(UINT8 u1StreamId)
{
    return VIRTUAL(u4GetMixSndReadPtr(u1StreamId));
}
EXPORT_SYMBOL(AUD_GetMixSndReadPtr);

extern void vSetMixSndWritePtr(UINT8 u1StreamId, UINT32 u4WritePtr);
void AUD_SetMixSndWritePtr(UINT8 u1StreamId, UINT32 u4WritePtr)
{
    vSetMixSndWritePtr(u1StreamId, PHYSICAL(u4WritePtr));
}
EXPORT_SYMBOL(AUD_SetMixSndWritePtr);

extern UINT32 u4GetMixSndFIFOStart(UINT8 u1StreamId);
UINT32 AUD_GetMixSndFIFOStart(UINT8 u1StreamId)
{
    return VIRTUAL(u4GetMixSndFIFOStart(u1StreamId));
}
EXPORT_SYMBOL(AUD_GetMixSndFIFOStart);

extern UINT32 u4GetMixSndFIFOEnd(UINT8 u1StreamId);
UINT32 AUD_GetMixSndFIFOEnd(UINT8 u1StreamId)
{
    return VIRTUAL(u4GetMixSndFIFOEnd(u1StreamId));
}
EXPORT_SYMBOL(AUD_GetMixSndFIFOEnd);

extern void DSP_FlushInvalidateDCacheFree(UINT32 u4Addr, UINT32 u4Len);
void AUD_FlushDram(UINT32 u4Addr, UINT32 u4Len)
{
    DSP_FlushInvalidateDCacheFree(u4Addr, u4Len);
}
EXPORT_SYMBOL(AUD_FlushDram);

#endif
#endif //LINUX_TURNKEY_SOLUTION

#if defined(CC_AUD_ARM_SUPPORT) && defined(CC_AUD_ARM_RENDER)
//=========================================================
//         APROC MixSound
//=========================================================
void AUD_AprocSwDecUpdateRp(UINT8 u1ID, UINT32 u4Rp) 
{
    if (u1ID >= AUD_SWDEC_NUM)
    {
        return;
    }
    _gsAprocSwDecBufInfo[u1ID].u4Rp = u4Rp;
}
void AUD_AprocMixSndUpdateRp(UINT8 u1ID, UINT32 u4Rp) //parson mixsnd
{
    if (u1ID >= AUD_MIXSND_NUM)
    {
        return;
    }
    _gsAprocMixSndBufInfo[u1ID].u4Rp = u4Rp;
}

static void AUD_AprocMixSndInfoInit(AUD_MIXSND_ID_T u1ID)
{
    UINT16 jj;
    INT32* TestPnt;
    static BOOL fgTestTone = FALSE;

    if (u1ID>=AUD_MIXSND_NUM)
    {
        LOG(0, "APROC MixSound %d not Support !!\n", u1ID);
        return;
    }

    if (u1ID == AUD_MIXSND_0)
    {
        _gsAprocMixSndBufInfo[u1ID].u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
            u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT0_START));
        _gsAprocMixSndBufInfo[u1ID].u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT0_END));
        _gsAprocMixSndBufInfo[u1ID].u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT0_SIZE));
        _gsAprocMixSndBufInfo[u1ID].u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT0_CH));
    }
    else if (u1ID == AUD_MIXSND_1)
    {
        _gsAprocMixSndBufInfo[u1ID].u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
            u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT1_START));
        _gsAprocMixSndBufInfo[u1ID].u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT1_END));
        _gsAprocMixSndBufInfo[u1ID].u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT1_SIZE));
        _gsAprocMixSndBufInfo[u1ID].u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT1_CH));
    }
    else if (u1ID == AUD_MIXSND_2)
    {
        _gsAprocMixSndBufInfo[u1ID].u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
            u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_1, APROC_REG_IOBUF_MIXSND_INPUT2_START));
        _gsAprocMixSndBufInfo[u1ID].u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_1, APROC_REG_IOBUF_MIXSND_INPUT2_END));
        _gsAprocMixSndBufInfo[u1ID].u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_1, APROC_REG_IOBUF_MIXSND_INPUT2_SIZE));
        _gsAprocMixSndBufInfo[u1ID].u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_1, APROC_REG_IOBUF_MIXSND_INPUT2_CH));
    }
    else if (u1ID == AUD_MIXSND_3)
    {
        _gsAprocMixSndBufInfo[u1ID].u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
            u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_1, APROC_REG_IOBUF_MIXSND_INPUT3_START));
        _gsAprocMixSndBufInfo[u1ID].u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_1, APROC_REG_IOBUF_MIXSND_INPUT3_END));
        _gsAprocMixSndBufInfo[u1ID].u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_1, APROC_REG_IOBUF_MIXSND_INPUT3_SIZE));
        _gsAprocMixSndBufInfo[u1ID].u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_1, APROC_REG_IOBUF_MIXSND_INPUT3_CH));
    }
    else if (u1ID == AUD_MIXSND_4)
    {
        _gsAprocMixSndBufInfo[u1ID].u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
            u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_2, APROC_REG_IOBUF_MIXSND_INPUT4_START));
        _gsAprocMixSndBufInfo[u1ID].u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_2, APROC_REG_IOBUF_MIXSND_INPUT4_END));
        _gsAprocMixSndBufInfo[u1ID].u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_2, APROC_REG_IOBUF_MIXSND_INPUT4_SIZE));
        _gsAprocMixSndBufInfo[u1ID].u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_2, APROC_REG_IOBUF_MIXSND_INPUT4_CH));
    }
    else if (u1ID == AUD_MIXSND_5)
    {
        _gsAprocMixSndBufInfo[u1ID].u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
            u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_2, APROC_REG_IOBUF_MIXSND_INPUT5_START));
        _gsAprocMixSndBufInfo[u1ID].u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_2, APROC_REG_IOBUF_MIXSND_INPUT5_END));
        _gsAprocMixSndBufInfo[u1ID].u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_2, APROC_REG_IOBUF_MIXSND_INPUT5_SIZE));
        _gsAprocMixSndBufInfo[u1ID].u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_2, APROC_REG_IOBUF_MIXSND_INPUT5_CH));
    }
    else if (u1ID == AUD_MIXSND_6)
    {
        _gsAprocMixSndBufInfo[u1ID].u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
            u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_3, APROC_REG_IOBUF_MIXSND_INPUT6_START));
        _gsAprocMixSndBufInfo[u1ID].u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_3, APROC_REG_IOBUF_MIXSND_INPUT6_END));
        _gsAprocMixSndBufInfo[u1ID].u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_3, APROC_REG_IOBUF_MIXSND_INPUT6_SIZE));
        _gsAprocMixSndBufInfo[u1ID].u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_3, APROC_REG_IOBUF_MIXSND_INPUT6_CH));
    }
    else if (u1ID == AUD_MIXSND_7)
    {
        _gsAprocMixSndBufInfo[u1ID].u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
            u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_3, APROC_REG_IOBUF_MIXSND_INPUT7_START));
        _gsAprocMixSndBufInfo[u1ID].u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_3, APROC_REG_IOBUF_MIXSND_INPUT7_END));
        _gsAprocMixSndBufInfo[u1ID].u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_3, APROC_REG_IOBUF_MIXSND_INPUT7_SIZE));
        _gsAprocMixSndBufInfo[u1ID].u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_3, APROC_REG_IOBUF_MIXSND_INPUT7_CH));
    }
#ifdef CC_AUD_APROC_HWMIX
	else if (u1ID == AUD_HW_MIXSND_0)
	{
		_gsAprocMixSndBufInfo[u1ID].u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
			u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_HW_MIXSND_0, APROC_REG_IOBUF_HW_MIXSND_INPUT0_START));
		_gsAprocMixSndBufInfo[u1ID].u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_HW_MIXSND_0, APROC_REG_IOBUF_HW_MIXSND_INPUT0_END));
		_gsAprocMixSndBufInfo[u1ID].u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_HW_MIXSND_0, APROC_REG_IOBUF_HW_MIXSND_INPUT0_SIZE));
		_gsAprocMixSndBufInfo[u1ID].u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_HW_MIXSND_0, APROC_REG_IOBUF_HW_MIXSND_INPUT0_CH));
	}
	else if (u1ID == AUD_HW_MIXSND_1)
	{
		_gsAprocMixSndBufInfo[u1ID].u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
			u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_HW_MIXSND_0, APROC_REG_IOBUF_HW_MIXSND_INPUT1_START));
		_gsAprocMixSndBufInfo[u1ID].u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_HW_MIXSND_0, APROC_REG_IOBUF_HW_MIXSND_INPUT1_END));
		_gsAprocMixSndBufInfo[u1ID].u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_HW_MIXSND_0, APROC_REG_IOBUF_HW_MIXSND_INPUT1_SIZE));
		_gsAprocMixSndBufInfo[u1ID].u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_HW_MIXSND_0, APROC_REG_IOBUF_HW_MIXSND_INPUT1_CH));
	}

#endif

    _gsAprocMixSndBufInfo[u1ID].u4Rp = 0;    
    _gsAprocMixSndBufInfo[u1ID].u4Wp = 1;
    _gsAprocMixSndBufInfo[u1ID].u4Rcurp = 0;    
    _gsAprocMixSndBufInfo[u1ID].u4Wcurp = 0;

    //Generate Test Pattern only once 
    if (!fgTestTone)
    {
        //Input of L     
        TestPnt = &i4TestTone[0];
        for (jj=0; jj<(MIXSND_768/16); jj++)
        {
            #if 0
            *TestPnt++ = 0x0; // 1
            *TestPnt++ = 0x30FB0000; // 2
            *TestPnt++ = 0x5A820000; // 3
            *TestPnt++ = 0x76400000; // 4
            *TestPnt++ = 0x7FFF0000; // 5
            *TestPnt++ = 0x76400000; // 6
            *TestPnt++ = 0x5A820000; // 7
            *TestPnt++ = 0x30FB0000; // 8
            *TestPnt++ = 0x00000000; // 9
            *TestPnt++ = 0xCF040000; // 10
            *TestPnt++ = 0xA57E0000; // 11
            *TestPnt++ = 0x89BF0000; // 12
            *TestPnt++ = 0x80000000; // 13        
            *TestPnt++ = 0x89BF0000; // 14
            *TestPnt++ = 0xA57F0000; // 15
            *TestPnt++ = 0xCF050000; // 16
#endif
            *TestPnt++ = 0x0; // 1
            *TestPnt++ = 0x04e60000; // 2
            *TestPnt++ = 0x090c0000; // 3
            *TestPnt++ = 0x0bd30000; // 4
            *TestPnt++ = 0x0ccc0000; // 5
            *TestPnt++ = 0x0bd40000; // 6
            *TestPnt++ = 0x090d0000; // 7
            *TestPnt++ = 0x04e50000; // 8
            *TestPnt++ = 0x00000000; // 9
            *TestPnt++ = 0xfb1b0000; // 10
            *TestPnt++ = 0xf6f40000; // 11
            *TestPnt++ = 0xf42d0000; // 12
            *TestPnt++ = 0xf3330000; // 13        
            *TestPnt++ = 0xf42d0000; // 14
            *TestPnt++ = 0xf6f20000; // 15
            *TestPnt++ = 0xfb1a0000; // 16
        }    
         
        
        // -20 dB Signal.....
        TestPnt = &i4TestTone[MIXSND_768]; //Input of R
        for (jj=0; jj<(MIXSND_768/16); jj++)
        {
            *TestPnt++ = 0x0; // 1
            *TestPnt++ = 0x04e60000; // 2
            *TestPnt++ = 0x090c0000; // 3
            *TestPnt++ = 0x0bd30000; // 4
            *TestPnt++ = 0x0ccc0000; // 5
            *TestPnt++ = 0x0bd40000; // 6
            *TestPnt++ = 0x090d0000; // 7
            *TestPnt++ = 0x04e50000; // 8
            *TestPnt++ = 0x00000000; // 9
            *TestPnt++ = 0xfb1b0000; // 10
            *TestPnt++ = 0xf6f40000; // 11
            *TestPnt++ = 0xf42d0000; // 12
            *TestPnt++ = 0xf3330000; // 13        
            *TestPnt++ = 0xf42d0000; // 14
            *TestPnt++ = 0xf6f20000; // 15
            *TestPnt++ = 0xfb1a0000; // 16
        }    
        fgTestTone = TRUE;
    }
    
}

static void AUD_AprocMixSndEnable(AUD_MIXSND_ID_T eMixSndId, BOOL fgEnable)
{
    UINT32 u4Idx, u4Addr;
    switch (eMixSndId)
    {
	case AUD_MIXSND_0:
        u4Idx = APROC_ASM_ID_MIXSND_0;
        u4Addr = APROC_REG_IOBUF_MIXSND_INPUT0_ENABLE;
		break;
	case AUD_MIXSND_1:
        u4Idx = APROC_ASM_ID_MIXSND_0;
        u4Addr = APROC_REG_IOBUF_MIXSND_INPUT1_ENABLE;
		break;
	case AUD_MIXSND_2:
        u4Idx = APROC_ASM_ID_MIXSND_1;
        u4Addr = APROC_REG_IOBUF_MIXSND_INPUT2_ENABLE;        
		break;
	case AUD_MIXSND_3:
        u4Idx = APROC_ASM_ID_MIXSND_1;
        u4Addr = APROC_REG_IOBUF_MIXSND_INPUT3_ENABLE;
		break;
	case AUD_MIXSND_4:
        u4Idx = APROC_ASM_ID_MIXSND_2;
        u4Addr = APROC_REG_IOBUF_MIXSND_INPUT4_ENABLE;
		break;
	case AUD_MIXSND_5:
        u4Idx = APROC_ASM_ID_MIXSND_2;
        u4Addr = APROC_REG_IOBUF_MIXSND_INPUT5_ENABLE;
		break;
	case AUD_MIXSND_6:
        u4Idx = APROC_ASM_ID_MIXSND_3;
        u4Addr = APROC_REG_IOBUF_MIXSND_INPUT6_ENABLE;
		break;
	case AUD_MIXSND_7:
        u4Idx = APROC_ASM_ID_MIXSND_3;
        u4Addr = APROC_REG_IOBUF_MIXSND_INPUT7_ENABLE;
        break;
#ifdef CC_AUD_APROC_HWMIX 
	case AUD_HW_MIXSND_0:
        u4Idx = APROC_ASM_ID_HW_MIXSND_0;
        u4Addr = APROC_REG_IOBUF_HW_MIXSND_INPUT0_ENABLE;        
		break;
	case AUD_HW_MIXSND_1:
        u4Idx = APROC_ASM_ID_HW_MIXSND_0;
        u4Addr = APROC_REG_IOBUF_HW_MIXSND_INPUT1_ENABLE;        
		break;
#endif			
	default:
		return;
    }

    LOG(2, "%s %d : %#x : %d", __FUNCTION__, eMixSndId, u4Idx, fgEnable);
    if (fgEnable)
    {
        LOG(0, "%s  Play PCM_MIX%d\n", __FUNCTION__, eMixSndId); 
    }
    else
    {
        LOG(0, "%s Stop PCM_MIX%d\n", __FUNCTION__, eMixSndId); 
    }

    vAprocReg_Write(APROC_ASM_ADDR (u4Idx, u4Addr), fgEnable); 
}

static void AUD_AprocMixSndReset(AUD_MIXSND_ID_T eMixSndId)
{
    UINT32 u4Idx;
    switch (eMixSndId)
    {
	case AUD_MIXSND_0:
        u4Idx = APROC_SIG_IDX_MIXSND0;
		break;
	case AUD_MIXSND_1:
        u4Idx = APROC_SIG_IDX_MIXSND1;
		break;
	case AUD_MIXSND_2:
        u4Idx = APROC_SIG_IDX_MIXSND2;
		break;
	case AUD_MIXSND_3:
        u4Idx = APROC_SIG_IDX_MIXSND3;
		break;
	case AUD_MIXSND_4:
        u4Idx = APROC_SIG_IDX_MIXSND4;
		break;
	case AUD_MIXSND_5:
        u4Idx = APROC_SIG_IDX_MIXSND5;
		break;
	case AUD_MIXSND_6:
        u4Idx = APROC_SIG_IDX_MIXSND6;
		break;
	case AUD_MIXSND_7:
        u4Idx = APROC_SIG_IDX_MIXSND7;
        break;
#ifdef CC_AUD_APROC_HWMIX 
	case AUD_HW_MIXSND_0:
        u4Idx = APROC_SIG_IDX_HW_MIXSND0;
		break;
	case AUD_HW_MIXSND_1:
        u4Idx = APROC_SIG_IDX_HW_MIXSND1;
		break;
#endif			
	default:
		return;
    }

    LOG(2, "%s %d : %#x ",__FUNCTION__,  eMixSndId, u4Idx);

    vAprocCmdSet(u4Idx, APROC_SIG_SET_RESET, 0);    
}

static void AUD_AprocMixSndUpdateWp(AUD_MIXSND_ID_T eMixSndId, UINT32 u4Wp) 
{
    UINT32 u4Idx;
    switch (eMixSndId)
    {
	case AUD_MIXSND_0:
        u4Idx = APROC_SIG_IDX_MIXSND0;
		break;
	case AUD_MIXSND_1:
        u4Idx = APROC_SIG_IDX_MIXSND1;
		break;
	case AUD_MIXSND_2:
        u4Idx = APROC_SIG_IDX_MIXSND2;
		break;
	case AUD_MIXSND_3:
        u4Idx = APROC_SIG_IDX_MIXSND3;
		break;
	case AUD_MIXSND_4:
        u4Idx = APROC_SIG_IDX_MIXSND4;
		break;
	case AUD_MIXSND_5:
        u4Idx = APROC_SIG_IDX_MIXSND5;
		break;
	case AUD_MIXSND_6:
        u4Idx = APROC_SIG_IDX_MIXSND6;
		break;
	case AUD_MIXSND_7:
        u4Idx = APROC_SIG_IDX_MIXSND7;
        break;
#ifdef CC_AUD_APROC_HWMIX 
	case AUD_HW_MIXSND_0:
        u4Idx = APROC_SIG_IDX_HW_MIXSND0;
		break;
	case AUD_HW_MIXSND_1:
        u4Idx = APROC_SIG_IDX_HW_MIXSND1;
		break;
#endif			
	default:
		return;
    }

    LOG(2, "%s, %d : %#x : %#x", __FUNCTION__, eMixSndId, u4Idx,  u4Wp);
    vAprocCmdSet(u4Idx, APROC_SIG_SET_UPDATE_WP, u4Wp);    
}
static void _AudAprocFeedMixSndThread(const void* pvArg)
{
    UINT8 *pAprocMixSndBuf;
    UINT32 u4Loop = 1024*512;
    UINT16 i;
    UINT32 u4BlockSize,u4End,u4Cur,u4Rp,u4ChNum=0;
    UINT8 *u1Dst, *u1Src;
    AUD_MIXSND_ID_T eMixSndId = AUD_MIXSND_0;

    eMixSndId = *(AUD_MIXSND_ID_T *)pvArg;
    if (eMixSndId  >= AUD_MIXSND_NUM)
    {
        eMixSndId = AUD_MIXSND_0;
    }
    
    fgAudFeedMixSndThreadEnable_1[eMixSndId] = TRUE;

    // Init Mixsound for CA9
    AUD_AprocMixSndInfoInit(eMixSndId);

    _gsAprocMixSndBufInfo[eMixSndId].u4Rp = 0;    
    _gsAprocMixSndBufInfo[eMixSndId].u4Wp = 0;
    _gsAprocMixSndBufInfo[eMixSndId].u4Rcurp = 0;    
    _gsAprocMixSndBufInfo[eMixSndId].u4Wcurp = 1;    
    // Request ARM9 to reset mixsound buffer info.
    AUD_AprocMixSndReset(eMixSndId);    

    // Send MixSound Enable Command to ARM9    
    x_thread_delay(20);
    AUD_AprocMixSndEnable(eMixSndId, TRUE);
    
    // Buffer Address
    pAprocMixSndBuf = (UINT8 *) _gsAprocMixSndBufInfo[eMixSndId].u4Str;
    u4BlockSize = _gsAprocMixSndBufInfo[eMixSndId].u4Size; 
    u4End = _gsAprocMixSndBufInfo[eMixSndId].u4End ;
    u4Cur =  _gsAprocMixSndBufInfo[eMixSndId].u4Wcurp;

    LOG(0, "AUD_MIXSND_%d : pAprocMixSndBuf=0x%x, BlkSize=%d, u4End=%d, Cur=%d, ChNum=%d\n",eMixSndId, pAprocMixSndBuf, u4BlockSize, u4End, u4Cur, u4ChNum);

    u1Dst =(UINT8*)VIRTUAL((UINT32)pAprocMixSndBuf);  // to fix unintialized build error
                
    while (1)
    {
        for (i=0; i<u4Loop; i++)
        {
            if (_fgForceStopMixSndDma_1[eMixSndId] == TRUE)
            {
                break;
            }
            u4Rp = _gsAprocMixSndBufInfo[eMixSndId].u4Rp;
            u4ChNum = _gsAprocMixSndBufInfo[eMixSndId].u4Ch;             
            // check if ready to write
            if (u4Cur != u4Rp) 
            {
                // fill data
                u1Src = (UINT8*)&i4TestTone[0];
                u1Dst =(UINT8*)VIRTUAL((UINT32)(pAprocMixSndBuf + (u4Cur * u4BlockSize)));

                for (; u4ChNum != 0; u4ChNum --)
                {
                    x_memcpy (u1Dst, (UINT8*)VIRTUAL((UINT32)u1Src), u4BlockSize);                 
                    DSP_FlushInvalidateDCacheFree((UINT32) u1Dst, u4BlockSize);                    
                    u1Dst = (UINT8 *) ((UINT32) u1Dst + (u4BlockSize * u4End));
                    u1Src = (UINT8 *) ((UINT32) u1Src + u4BlockSize);
                }
            
                // update write pointer
                _gsAprocMixSndBufInfo[eMixSndId].u4Wp = u4Cur;
                // Send a notify to ARM9 for updating Wp
                AUD_AprocMixSndUpdateWp(eMixSndId, u4Cur);
                // upadte current pointer
                u4Cur ++;
                if (u4Cur >= u4End)
                {
                    u4Cur -= u4End;
                }
                // Update Write Current Pointer
                _gsAprocMixSndBufInfo[eMixSndId].u4Wcurp = u4Cur;
            }
            else
            {
                x_thread_delay(1);
            }

        }            

        break;        
    }

    LOG(0, "AUD_MIXSND_%d : pAprocMixSndBuf=0x%x, 0x%x. 0x%x\n", eMixSndId, pAprocMixSndBuf,(UINT8*)VIRTUAL((UINT32)pAprocMixSndBuf), (UINT8*)VIRTUAL((UINT32)(pAprocMixSndBuf+2*3072)));
    LOG(0, "AUD_MIXSND_%d : u4Cur=%d, u1Dst=0x%x\n", eMixSndId, u4Cur, (UINT32)u1Dst);
    
    _hAudFeedMixSndThread_1[eMixSndId] = NULL_HANDLE;    
    fgAudFeedMixSndThreadEnable_1[eMixSndId] = FALSE;
    _fgForceStopMixSndDma_1[eMixSndId] = FALSE;
    //Send MixSound Disable Command to ARM9    
    AUD_AprocMixSndEnable(eMixSndId, FALSE);
    
    LOG(0, "[FeedMixSndThread] (AUD_MIXSND%d) OK Exit\n", eMixSndId);
}

static void _AudAprocFeedMixSndThread_2(const void* pvArg)  // For APROC MixSound 1
{
#ifdef LINUX_TURNKEY_SOLUTION
    UINT8 *pAprocMixSndBuf;
    UINT32 u4Loop;
    //UINT16 i,j;
    UINT16 j;
    UINT32 u4BlockSize,u4End,u4Cur,u4Rp,u4ChNum;
    UINT8 *u1Dst, *u1Src, *u1DstTmp;
    UINT8 *u1DstTmp1;
    UINT8* pMixSndPlayBuf;
    UINT32 u4Size = 66*1024;
    INT32 i4Ret;

    mm_segment_t oldfs = (mm_segment_t)NULL;
    struct file *filep  = NULL ;
    //size_t ret;

    // Allocate Temp buffer
    LOG(0, "Allocate memory size 0x%x for memory play...\n",u4Size);
    pMixSndPlayBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, 32);
    if (pMixSndPlayBuf == NULL)
    {
        LOG(0, "Allocate memory %d Fail !!\n",u4Size);        
        goto _ERROR;
    }
    pMixSndPlayBuf = (UINT8*)VIRTUAL((UINT32)pMixSndPlayBuf);
    LOG(0, "pMixSndPlayBuf is (V)0x%x\n", pMixSndPlayBuf);
    
    // Open file on USB
    oldfs = get_fs ();
    set_fs (KERNEL_DS);
    filep = filp_open (fileName, O_RDONLY, 0);
    if (IS_ERR (filep))
    {
        set_fs (oldfs);
        LOG (0, "Open File error !!!!!!!!!!\n");
        goto _ERROR;
    }
            
    fgAudFeedMixSndThreadEnable_1[AUD_MIXSND_1] = TRUE;

    // Init Mixsound for CA9
    AUD_AprocMixSndInfoInit(AUD_MIXSND_1);
    
    // Request ARM9 to reset mixsound buffer info.
    vAprocCmdSet(APROC_SIG_IDX_MIXSND1, APROC_SIG_SET_RESET, 0);    

    // Send MixSound Enable Command to ARM9    
    x_thread_delay(20);
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT1_ENABLE), 1);
    
    // Buffer Address
    pAprocMixSndBuf = (UINT8 *) _gsAprocMixSndBufInfo[AUD_MIXSND_1].u4Str;
    u4BlockSize = _gsAprocMixSndBufInfo[AUD_MIXSND_1].u4Size; 
    u4End = _gsAprocMixSndBufInfo[AUD_MIXSND_1].u4End ;
    u4Cur =  _gsAprocMixSndBufInfo[AUD_MIXSND_1].u4Wcurp;
    u4ChNum = _gsAprocMixSndBufInfo[AUD_MIXSND_1].u4Ch;                                     

    LOG(0, "AUD_MIXSND_1 : pAprocMixSndBuf=(V)0x%x, BlkSize=%d, u4End=%d, Cur=%d, ChNum=%d\n", VIRTUAL((UINT32)pAprocMixSndBuf), u4BlockSize, u4End, u4Cur, u4ChNum);

    u1Dst =(UINT8*)VIRTUAL((UINT32)pAprocMixSndBuf); //to fix uninitialized build error
    
    while (1)
    {
        if (_fgForceStopMixSndDma_1[AUD_MIXSND_1]==TRUE)
        {
            break;
        }
         
        // Get 66K Bytes from USB...(multiple of 768)
        i4Ret = filep->f_op->read (filep, pMixSndPlayBuf, u4Size, &filep->f_pos);
        if (i4Ret <= 0)
        {
            LOG(0, "Read data from USB fail..\n");
            break;
        }   
        else
        {
            LOG(7, "i4Ret = %d\n", i4Ret);
            u4Loop = u4Size/MIXSND_768/2/2; // 768 samples, 2 bytes, 2ch
            u1Src = (UINT8*)VIRTUAL((UINT32)pMixSndPlayBuf);
                
            for ( ; u4Loop > 0; )
            {
                if (_fgForceStopMixSndDma_1[AUD_MIXSND_1]==TRUE)
                {
                    break;
                }        
                
                u4Rp = _gsAprocMixSndBufInfo[AUD_MIXSND_1].u4Rp;

                // check if ready to write
                if (u4Cur != u4Rp) 
                {
                    // fill data for 768 samples , 2ch
                    u1Dst =(UINT8*)VIRTUAL((UINT32)(pAprocMixSndBuf + (u4Cur * u4BlockSize)));
                    LOG(7, "Loopi=%d, Sta u1Dst=0x%x, u1Src=0x%x\n", u4Loop, u1Dst, u1Src);                    

                    // Put 786 samples , 4 Bytes.
                    u1DstTmp1 = u1Dst;
                    for (j=0; j<MIXSND_768; j++)
                    {
                        u4ChNum = _gsAprocMixSndBufInfo[AUD_MIXSND_1].u4Ch;                                 
                        u1DstTmp = u1Dst;
                        for (; u4ChNum != 0; u4ChNum --)
                        {
                            *u1DstTmp++ = 0;
                            *u1DstTmp++ = 0;
                            *u1DstTmp++ = *u1Src++;
                            *u1DstTmp++ = *u1Src++;
                            u1DstTmp = (UINT8 *) ((UINT32) u1Dst + (u4BlockSize * u4End));
                        }
                        u1Dst = (UINT8 *) ((UINT32) u1Dst + 4);                        
                    }
                    // Flush
                    DSP_FlushInvalidateDCacheFree((UINT32)(u1DstTmp1), u4BlockSize);
                    u1DstTmp1 = (UINT8 *) ((UINT32) u1DstTmp1 + (u4BlockSize * u4End));
                    DSP_FlushInvalidateDCacheFree((UINT32)(u1DstTmp1), u4BlockSize);        
                    x_thread_delay(1);
                    
                    // update write pointer
                    _gsAprocMixSndBufInfo[AUD_MIXSND_1].u4Wp = u4Cur;
                    // Send a notify to ARM9 for updating Wp
                    vAprocCmdSet(APROC_SIG_IDX_MIXSND1, APROC_SIG_SET_UPDATE_WP, u4Cur);
                    // upadte current pointer
                    u4Cur ++;
                    if (u4Cur >= u4End)
                    {
                        u4Cur -= u4End;
                    }
                    // Update Write Current Pointer
                    _gsAprocMixSndBufInfo[AUD_MIXSND_1].u4Wcurp = u4Cur;

                    // Loop Counter --
                    u4Loop--;
                    LOG(7, "Loopi=%d, End u1Dst=0x%x, u1Src=0x%x, %d\n", u4Loop, u1Dst, u1Src, j);
                }
                else
                {
                    x_thread_delay(1);
                }

            }            
        }

        if (_fgForceStopMixSndDma_1[AUD_MIXSND_1]==TRUE)
        {
            break;
        }
        
    }

    LOG(0, "AUD_MIXSND_1 : pAprocMixSndBuf=0x%x, 0x%x. 0x%x\n", pAprocMixSndBuf,(UINT8*)VIRTUAL((UINT32)pAprocMixSndBuf), (UINT8*)VIRTUAL((UINT32)(pAprocMixSndBuf+2*3072)));
    LOG(0, "AUD_MIXSND_1 : u4Cur=%d, u1Dst=0x%x\n", u4Cur, (UINT32)u1Dst);


_ERROR:    
    _hAudFeedMixSndThread_1[AUD_MIXSND_1] = NULL_HANDLE;    
    fgAudFeedMixSndThreadEnable_1[AUD_MIXSND_1] = FALSE;
    _fgForceStopMixSndDma_1[AUD_MIXSND_1] = FALSE;
    //Send MixSound Disable Command to ARM9    
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT1_ENABLE), 0); 

    // Free Temp Buffer
    VERIFY(BSP_FreeAlignedDmaMemory((UINT32)PHYSICAL((UINT32)pMixSndPlayBuf)));
    LOG(0, "Free pMixSndPlayBuf memory 0x%x \n",pMixSndPlayBuf);
    // Close File
    if (!IS_ERR (filep))
    {
        filp_close (filep, 0);
    }
    set_fs(oldfs);    
        
    LOG(0, "[FeedMixSndThread] (AUD_MIXSND_1) OK Exit\n");
#endif
}

#ifdef CC_AUD_APROC_KEYSOUND
extern UINT8 _u1MixSndClipIdx;
extern UINT32 _u4MixsoundLoop;
static void _AudAprocFeedKeySndThread(const void* pvArg)  // For APROC MixSound 0
{
    UINT8 *pAprocMixSndBuf;
    UINT16 i;
    UINT32 u4BlockSize,u4End,u4Cur,u4Rp,u4ChNum=0;
    UINT8 *u1Dst, *u1Src;

    fgAudFeedMixSndThreadEnable_1[KEY_MIXSND_ID] = TRUE;

    // Init Mixsound for CA9
    AUD_AprocMixSndInfoInit(KEY_MIXSND_ID);
    // Request ARM9 to reset mixsound buffer info.
    AUD_AprocMixSndReset(KEY_MIXSND_ID);	
	
	// Buffer Address
    pAprocMixSndBuf = (UINT8 *) _gsAprocMixSndBufInfo[KEY_MIXSND_ID].u4Str;
    u4BlockSize = _gsAprocMixSndBufInfo[KEY_MIXSND_ID].u4Size; 
    u4End = _gsAprocMixSndBufInfo[KEY_MIXSND_ID].u4End ;
    u4Cur =  _gsAprocMixSndBufInfo[KEY_MIXSND_ID].u4Wcurp;

    LOG(0, "KEY_MIXSND_ID %d : pAprocMixSndBuf=0x%x, BlkSize=%d, u4End=%d, Cur=%d, ChNum=%d\n", KEY_MIXSND_ID, pAprocMixSndBuf, u4BlockSize, u4End, u4Cur, u4ChNum);
    LOG(0, "LOOP %d \n", _u4MixsoundLoop);

    u1Dst =(UINT8*)VIRTUAL((UINT32)pAprocMixSndBuf);  // to fix unintialized build error
                
    while (1)
    {
        for (i=0; i<_u4MixsoundLoop; i++)
        {
            if (_fgForceStopMixSndDma_1[KEY_MIXSND_ID]==TRUE)
            {
                break;
            }
            u4Rp = _gsAprocMixSndBufInfo[KEY_MIXSND_ID].u4Rp;
            u4ChNum = _gsAprocMixSndBufInfo[KEY_MIXSND_ID].u4Ch;             
            // check if ready to write
            if (u4Cur != u4Rp) 
            {
                // fill data
                /*
				if (_u1MixSndClipIdx == AUD_MIXSND_CLIP0)
				{
					u1Src = (UINT8*)&_au4MixSndData0[0];
				}
				else if (_u1MixSndClipIdx == AUD_MIXSND_CLIP1)
				{
					u1Src = (UINT8*)&_au4MixSndData0_Stereo[0];
				}
				else if (_u1MixSndClipIdx == AUD_MIXSND_CLIP2)
				{
					u1Src = (UINT8*)&_au4MixSndData0_Stereo_LOnROff[0];
				}
				else
				{
					u1Src = (UINT8*)&_au4MixSndData0[0];
				}
				*/
                u1Src = (UINT8*)&i4TestTone[0];
                u1Dst =(UINT8*)VIRTUAL((UINT32)(pAprocMixSndBuf + (u4Cur * u4BlockSize)));

                for (; u4ChNum != 0; u4ChNum --)
                {
                    x_memcpy (u1Dst, (UINT8*)VIRTUAL((UINT32)u1Src), u4BlockSize);                 
                    DSP_FlushInvalidateDCacheFree((UINT32) u1Dst, u4BlockSize);                    
                    u1Dst = (UINT8 *) ((UINT32) u1Dst + (u4BlockSize * u4End));
                    u1Src = (UINT8 *) ((UINT32) u1Src + u4BlockSize);
                }
            
                // update write pointer
                _gsAprocMixSndBufInfo[KEY_MIXSND_ID].u4Wp = u4Cur;
                // Send a notify to ARM9 for updating Wp
                AUD_AprocMixSndUpdateWp(AUD_MIXSND_ID_T KEY_MIXSND_ID, u4Cur);			
#if 0
                HAL_TIME_T dt;
                HAL_GetTime(&dt);
                LOG(6, " %u.%06u s \n", dt.u4Seconds, dt.u4Micros);
#endif                   
                // upadte current pointer
                u4Cur ++;
                if (u4Cur >= u4End)
                {
                    u4Cur -= u4End;
                }
                // Update Write Current Pointer
                _gsAprocMixSndBufInfo[KEY_MIXSND_ID].u4Wcurp = u4Cur;
            }
            else
            {
                x_thread_delay(1);
            }

        }            

        break;        
    }

    LOG(0, "KEY_MIXSND_ID = %d : pAprocMixSndBuf=0x%x, 0x%x. 0x%x\n", KEY_MIXSND_ID, pAprocMixSndBuf,(UINT8*)VIRTUAL((UINT32)pAprocMixSndBuf), (UINT8*)VIRTUAL((UINT32)(pAprocMixSndBuf+2*3072)));
    LOG(0, "KEY_MIXSND_ID = %d : u4Cur=%d, u1Dst=0x%x\n", KEY_MIXSND_ID, u4Cur, (UINT32)u1Dst);
    
    _hAudFeedMixSndThread_1[KEY_MIXSND_ID] = NULL_HANDLE;    
    fgAudFeedMixSndThreadEnable_1[KEY_MIXSND_ID] = FALSE;
    _fgForceStopMixSndDma_1[KEY_MIXSND_ID] = FALSE;
    //Send MixSound Disable Command to ARM9
    AUD_AprocMixSndEnable(KEY_MIXSND_ID, FALSE);	
    LOG(0, "[FeedKeySndThread] (%d) OK Exit\n", KEY_MIXSND_ID);
}

#endif

#if defined(CC_AUD_APROC_KEYSOUND)
void AUD_AprocEnableKeySnd(void)
{
    INT32 i4Ret = -1;

	if (KEY_MIXSND_ID >= AUD_MIXSND_NUM )
	{
        LOG(0, "!!!! Wrong KEY_MIXSND_ID %d !!!!\n", KEY_MIXSND_ID);
        return;
	}
	
    if (_hAudFeedMixSndThread_1[KEY_MIXSND_ID])
    {
        LOG(0, "!!!! _AudFeedKeySndThread already created %d !!!!\n", KEY_MIXSND_ID);
        return;
    }

	i4Ret = x_thread_create(&_hAudFeedMixSndThread_1[KEY_MIXSND_ID],
							   "AprocMixSnd_1_1" ,
							   AUD_DRV_THREAD_STACK_SIZE,
							   u1MixSndThread_Priority, 
							   (x_os_thread_main_fct) _AudAprocFeedKeySndThread,
							   0,
							   NULL);
	
    if (OSR_OK != i4Ret)
    {
        LOG(0, "Create _AudFeedKeySndThread %d Fail!!!!\n", KEY_MIXSND_ID);
    }
    else
    {
        LOG(0, "Create _AudFeedKeySndThread %d Succ!!!!\n", KEY_MIXSND_ID);
    }
}

void AUD_AprocDisableKeySnd(void)
{
	AUD_AprocDisableMixSnd(KEY_MIXSND_ID);
}
#endif
void AUD_AprocEnableMixSnd(UINT8 u1MixID) //parson mixsnd
{
    INT32 i4Ret = -1;
    CHAR szThreadName[64];

    if (u1MixID >= AUD_MIXSND_NUM)
    {
        LOG(0, "Only %d Mixsound support\n", AUD_MIXSND_NUM);
        return;
    }

    if (_hAudFeedMixSndThread_1[u1MixID])
    {
        LOG(0, "!!!! _AudAprocFeedMixSndThread already created !!!!\n");
        return;
    }

    if (u1MixID < AUD_MIXSND_NUM)
    {
    sprintf(szThreadName, "AprocMixSnd_%d", u1MixID);
    i4Ret = x_thread_create(&_hAudFeedMixSndThread_1[u1MixID],
                           szThreadName,
                           AUD_DRV_THREAD_STACK_SIZE,
                           u1MixSndThread_Priority, 
                           (x_os_thread_main_fct) _AudAprocFeedMixSndThread,
                           sizeof(UINT8),
                           &u1MixID);
    } 
    else if (u1MixID == AUD_MIXSND_1)
    {
        i4Ret = x_thread_create(&_hAudFeedMixSndThread_1[u1MixID],
                                   "AprocMixSnd_1_2" ,
                                   AUD_DRV_THREAD_STACK_SIZE,
                                   u1MixSndThread_Priority, 
                                   (x_os_thread_main_fct) _AudAprocFeedMixSndThread_2,
                                   0,
                                   NULL);    
    }
    if (OSR_OK != i4Ret)
    {
        LOG(0, "Create _AudAprocFeedMixSndThread %d Fail!!!!\n", u1MixID);
    }
    else
    {
        LOG(0, "Create _AudAprocFeedMixSndThread %d Succ!!!!\n", u1MixID);
    }
}

void AUD_AprocDisableMixSnd(UINT8 u1MixID)
{
    if (u1MixID >= AUD_MIXSND_NUM)
    {
        LOG(0, "Only %d Mixsound support\n", AUD_MIXSND_NUM);
        return;
    }
    
    if (_hAudFeedMixSndThread_1[u1MixID])
    {
        LOG(0, "[AUD_DisableMixSndClip] %d trigger\n", u1MixID);
        _fgForceStopMixSndDma_1[u1MixID] = TRUE;
        while (!_fgForceStopMixSndDma_1[u1MixID]);
        x_thread_delay(1);
    }
    else
    {
        LOG(0, "_hAudFeedMixSndThread %d not available\n", u1MixID);
    }

    //Send MixSound Disable Command to ARM9  
    if (u1MixID == AUD_MIXSND_0)
    {
        vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT0_ENABLE), 0);    
    }
    else if (u1MixID == AUD_MIXSND_1)
    {
        vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_MIXSND_0, APROC_REG_IOBUF_MIXSND_INPUT1_ENABLE), 0);            
    }
#ifdef CC_AUD_APROC_HWMIX
	else if (u1MixID == AUD_HW_MIXSND_0)
	{
		vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_HW_MIXSND_0, APROC_REG_IOBUF_HW_MIXSND_INPUT0_ENABLE), 0);			 
	}
	else if (u1MixID == AUD_HW_MIXSND_1)
	{
		vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_HW_MIXSND_0, APROC_REG_IOBUF_HW_MIXSND_INPUT1_ENABLE), 0);			 
	}
#endif
}


//=========================================================
//         APROC Upload 
//=========================================================
_IO_BUFFER_T _gsAprocUploadBufInfo;

void AUD_AprocUploadUpdateWp(UINT32 u4Wp)
{
    _gsAprocUploadBufInfo.u4Wp = u4Wp;
}

void AUD_AprocCpuLoadInit(void)
{
    _gsAprocUploadBufInfo.u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
        u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_START));
    _gsAprocUploadBufInfo.u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_END));
    _gsAprocUploadBufInfo.u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_SIZE));
    _gsAprocUploadBufInfo.u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_CH));

    _gsAprocUploadBufInfo.u4Rp = 0;    
    _gsAprocUploadBufInfo.u4Wp = 0;
    _gsAprocUploadBufInfo.u4Rcurp = 0;    
    _gsAprocUploadBufInfo.u4Wcurp = 1;
}

static void _AudCpuUploadThread(const void* pvArg)
{
#ifdef LINUX_TURNKEY_SOLUTION
    UINT8 *pAprocUploadBuf;
    UINT32 u4Loop;
    //UINT16 i,j;
    UINT16 j;
    UINT32 u4BlockSize,u4End,u4Cur,u4Wp,u4ChNum;
    UINT8 *u1Dst, *u1Src, *u1SrcTmp;
    UINT8* pTempUploadBuf;    // Allocate a temp buffer to store some banks and then write to USB once.
    UINT32 u4Size = 33*1024; // 33k Byte
    INT32 i4Ret;

    mm_segment_t oldfs = (mm_segment_t)NULL;
    struct file *filep  = NULL ;
    //size_t ret;

    // Allocate Temp buffer
    LOG(0, "Allocate memory size 0x%x for testing ARM9 Upload CPU...\n", u4Size);
    pTempUploadBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, 32);
    if (pTempUploadBuf == NULL)
    {
        LOG(0, "Allocate memory %d Fail !!\n", u4Size);        
        goto _ERROR;
    }
    pTempUploadBuf = (UINT8*)VIRTUAL((UINT32)pTempUploadBuf);
    LOG(0, "pTempUploadBuf is (V)0x%x\n", pTempUploadBuf);
    
    // Open file on USB
    oldfs = get_fs ();
    set_fs (KERNEL_DS);
    filep = filp_open (upload_fileName, (O_CREAT | O_WRONLY) , 0);
    if (IS_ERR (filep))
    {
        set_fs (oldfs);
        LOG (0, "Open File for Write Error !!!!!!!!!!\n");
        goto _ERROR;
    }
            
    fgAudCpuUploadThreadEnable = TRUE;

    // Init Mixsound for CA9
    AUD_AprocCpuLoadInit();    
    
    // Request ARM9 to reset mixsound buffer info.
    vAprocCmdSet(APROC_SIG_IDX_UPLOAD0, APROC_SIG_SET_RESET, 0);    

    // Send MixSound Enable Command to ARM9    
    x_thread_delay(20);
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_ENABLE), 1);
       
    // Buffer Address
    pAprocUploadBuf = (UINT8 *)_gsAprocUploadBufInfo.u4Str;
    u4BlockSize = _gsAprocUploadBufInfo.u4Size; 
    u4End = _gsAprocUploadBufInfo.u4End ;
    u4Cur =  _gsAprocUploadBufInfo.u4Rcurp;
    u4ChNum = _gsAprocUploadBufInfo.u4Ch;                                     

    LOG(0, "CpuUpload : pAprocUploadBuf=(V)0x%x, BlkSize=%d, u4End=%d, Cur=%d, ChNum=%d\n", pAprocUploadBuf, u4BlockSize, u4End, u4Cur, u4ChNum);

    u1Dst = (UINT8*)VIRTUAL((UINT32)pTempUploadBuf); // to fix uninitialized build 

    while (1)
    {
        if (_fgForceStopCpuUpload==TRUE)
        {
            break;
        }
         
        // Generate u4Size Bytes data by several 768 samples ....
        u4Loop = u4Size/MIXSND_768/2/2; // 768 samples, 2 bytes, 2ch
        u1Dst = (UINT8*)VIRTUAL((UINT32)pTempUploadBuf);
                
        for ( ; u4Loop > 0; )
        {
            if (_fgForceStopCpuUpload == TRUE)
            {
                break;
            }        
                
            u4Wp = _gsAprocUploadBufInfo.u4Wp;
            // check if ready to Read
            if (u4Cur != u4Wp) 
            {
                // upadte current pointer
                u4Cur ++;
                if (u4Cur >= u4End)
                {
                    u4Cur -= u4End;
                }
                // Update Read Current Pointer
                _gsAprocUploadBufInfo.u4Rcurp = u4Cur;
                
                // Get data from ARM9 for 768 samples , 2ch
                u1Src =(UINT8*)VIRTUAL((UINT32)(pAprocUploadBuf + (u4Cur * u4BlockSize)));
                LOG(7, "Loopi=%d, Sta u1Dst=0x%x, u1Src=0x%x\n", u4Loop, u1Dst, u1Src);

                // Invalidate DCache
                u1SrcTmp = u1Src;
                DSP_FlushInvalidateDCacheFree((UINT32)(u1SrcTmp), u4BlockSize);
                u1SrcTmp = (UINT8 *) ((UINT32) u1SrcTmp + (u4BlockSize * u4End));
                DSP_FlushInvalidateDCacheFree((UINT32)(u1SrcTmp), u4BlockSize);        
                x_thread_delay(1);
                
                // Get 786 samples , 4 Bytes.
                u1SrcTmp = u1Src;
                for (j=0; j<MIXSND_768; j++)
                {
                    u4ChNum = _gsAprocUploadBufInfo.u4Ch;                                 
                    u1SrcTmp = u1Src;
                    for (; u4ChNum != 0; u4ChNum --)
                    {
                        //*u1SrcTmp++;
                        //*u1SrcTmp++;
                        u1SrcTmp++;
                        u1SrcTmp++;
                        *u1Dst++ = *u1SrcTmp++;
                        *u1Dst++ = *u1SrcTmp++;                        
                        u1SrcTmp = (UINT8 *) ((UINT32) u1Src + (u4BlockSize * u4End));
                    }
                    u1Src = (UINT8 *) ((UINT32) u1Src + 4);                        
                }
                    
                // Update Read Pointer
                _gsAprocUploadBufInfo.u4Rp = u4Cur;
                // Send a notify to ARM9 for updating Rp
                vAprocCmdSet(APROC_SIG_IDX_UPLOAD0, APROC_SIG_SET_UPDATE_RP, u4Cur);

                // Loop Counter --
                u4Loop--;
                LOG(7, "Loopi=%d, End u1Dst=0x%x, u1Src=0x%x, %d\n", u4Loop, u1Dst, u1Src, j);
            }
            else
            {
                x_thread_delay(1);
            }

        }            
                
        // Write u4Size Bytes to USB...
        i4Ret = filep->f_op->write (filep, pTempUploadBuf, u4Size, &filep->f_pos);
        if (i4Ret <= 0)
        {
            LOG(0, "Write data to USB fail..\n");
            break;
        }   
        
        if (_fgForceStopCpuUpload==TRUE)
        {
            break;
        }
        
    }

    LOG(0, "_AudCpuUploadThread : pAprocMixSndBuf=(V)0x%x, 0x%x\n", (UINT8*)VIRTUAL((UINT32)pAprocUploadBuf), (UINT8*)VIRTUAL((UINT32)(pAprocUploadBuf+2*3072)));
    LOG(0, "_AudCpuUploadThread : u4Cur=%d, u1Dst=0x%x\n", u4Cur, (UINT32)u1Dst);


_ERROR:    
    _hAudUploadThread = NULL_HANDLE;    
    fgAudCpuUploadThreadEnable = FALSE;
    _fgForceStopCpuUpload = FALSE;
    //Send Disable Command to ARM9    
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_ENABLE), 0); 

    // Free Temp Buffer
    VERIFY(BSP_FreeAlignedDmaMemory((UINT32)PHYSICAL((UINT32)pTempUploadBuf)));
    LOG(0, "Free pTempUploadBuf memory 0x%x \n",pTempUploadBuf);
    // Close File
    if (!IS_ERR (filep))
    {
        filp_close (filep, 0);
    }
    set_fs(oldfs);    
        
    LOG(0, "[_AudCpuUploadThread] OK Exit\n");
#endif    
}

void AUD_AprocEnableUpload(void)
{
    INT32 i4Ret = -1;

    if (_hAudUploadThread)
    {
        LOG(0, "!!!! _AudUploadThread already created !!!!\n");
        return;
    }

    i4Ret = x_thread_create(&_hAudUploadThread,
                             "AprocCpuUpload" ,
                              AUD_DRV_THREAD_STACK_SIZE,
                              u1MixSndThread_Priority, 
                              (x_os_thread_main_fct) _AudCpuUploadThread,
                              0,
                              NULL);
    if (OSR_OK != i4Ret)
    {
        LOG(0, "Create _AudUploadThread Fail!!!!\n");
    }
    else
    {
        LOG(0, "Create _AudUploadThread Succ!!!!\n");
    }
}

void AUD_AprocDisableUpload(void)
{
    if (_hAudUploadThread)
    {
        LOG(0, "[AUD_DisableCpuUpload] trigger\n");
        _fgForceStopCpuUpload = TRUE;
        while (!_fgForceStopCpuUpload);
        x_thread_delay(1);
    }
    else
    {
        LOG(0, "_AudUploadThread is not available\n");
    }

    //Send Disable Command to ARM9  
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD0_ENABLE), 0);
}


//=========================================================
//         APROC Upload2 
//=========================================================
_IO_BUFFER_T _gsAprocUpload2BufInfo;

void AUD_AprocUpload2UpdateWp(UINT32 u4Wp)
{
    _gsAprocUpload2BufInfo.u4Wp = u4Wp;
}

void AUD_AprocCpuLoad2Init(void)
{
    _gsAprocUpload2BufInfo.u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
        u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_START));
    _gsAprocUpload2BufInfo.u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_END));
    _gsAprocUpload2BufInfo.u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_SIZE));
    _gsAprocUpload2BufInfo.u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_CH));

    _gsAprocUpload2BufInfo.u4Rp = 0;    
    _gsAprocUpload2BufInfo.u4Wp = 0;
    _gsAprocUpload2BufInfo.u4Rcurp = 0;    
    _gsAprocUpload2BufInfo.u4Wcurp = 1;
}

static void _AudCpuUpload2Thread(const void* pvArg)
{
#ifdef LINUX_TURNKEY_SOLUTION
    UINT8 *pAprocUploadBuf;
    UINT32 u4Loop;
    //UINT16 i,j;
    UINT16 j;
    UINT32 u4BlockSize,u4End,u4Cur,u4Wp,u4ChNum;
    UINT8 *u1Dst, *u1Src, *u1SrcTmp;
    UINT8* pTempUploadBuf;    // Allocate a temp buffer to store some banks and then write to USB once.
    UINT32 u4Size = 33*1024; // 33k Byte
    INT32 i4Ret;

    mm_segment_t oldfs = (mm_segment_t)NULL;
    struct file *filep  = NULL ;
    //size_t ret;

    // Allocate Temp buffer
    LOG(0, "Allocate memory size 0x%x for testing ARM9 Upload CPU...\n", u4Size);
    pTempUploadBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, 32);
    if (pTempUploadBuf == NULL)
    {
        LOG(0, "Allocate memory %d Fail !!\n", u4Size);        
        goto _ERROR;
    }
    pTempUploadBuf = (UINT8*)VIRTUAL((UINT32)pTempUploadBuf);
    LOG(0, "pTempUploadBuf is (V)0x%x\n", pTempUploadBuf);
    
    // Open file on USB
    oldfs = get_fs ();
    set_fs (KERNEL_DS);
    filep = filp_open (upload2_fileName, (O_CREAT | O_WRONLY) , 0);
    if (IS_ERR (filep))
    {
        set_fs (oldfs);
        LOG (0, "Open File for Write Error !!!!!!!!!!\n");
        goto _ERROR;
    }
            
    fgAudCpuUpload2ThreadEnable = TRUE;

    // Init Mixsound for CA9
    AUD_AprocCpuLoad2Init();    
    
    // Request ARM9 to reset mixsound buffer info.
    vAprocCmdSet(APROC_SIG_IDX_UPLOAD2, APROC_SIG_SET_RESET, 0);    

    // Send MixSound Enable Command to ARM9    
    x_thread_delay(20);
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_ENABLE), 1);
       
    // Buffer Address
    pAprocUploadBuf = (UINT8 *)_gsAprocUpload2BufInfo.u4Str;
    u4BlockSize = _gsAprocUpload2BufInfo.u4Size; 
    u4End = _gsAprocUpload2BufInfo.u4End ;
    u4Cur =  _gsAprocUpload2BufInfo.u4Rcurp;
    u4ChNum = _gsAprocUpload2BufInfo.u4Ch;                                     

    LOG(0, "CpuUpload : pAprocUploadBuf=(V)0x%x, BlkSize=%d, u4End=%d, Cur=%d, ChNum=%d\n", pAprocUploadBuf, u4BlockSize, u4End, u4Cur, u4ChNum);

    u1Dst = (UINT8*)VIRTUAL((UINT32)pTempUploadBuf); // to fix uninitialized build 

    while (1)
    {
        if (_fgForceStopCpuUpload2==TRUE)
        {
            break;
        }
         
        // Generate u4Size Bytes data by several 768 samples ....
        u4Loop = u4Size/MIXSND_768/2/2; // 768 samples, 2 bytes, 2ch
        u1Dst = (UINT8*)VIRTUAL((UINT32)pTempUploadBuf);
                
        for ( ; u4Loop > 0; )
        {
            if (_fgForceStopCpuUpload2 == TRUE)
            {
                break;
            }        
                
            u4Wp = _gsAprocUpload2BufInfo.u4Wp;
            // check if ready to Read
            if (u4Cur != u4Wp) 
            {
                // upadte current pointer
                u4Cur ++;
                if (u4Cur >= u4End)
                {
                    u4Cur -= u4End;
                }
                // Update Read Current Pointer
                _gsAprocUpload2BufInfo.u4Rcurp = u4Cur;
                
                // Get data from ARM9 for 768 samples , 2ch
                u1Src =(UINT8*)VIRTUAL((UINT32)(pAprocUploadBuf + (u4Cur * u4BlockSize)));
                LOG(7, "Loopi=%d, Sta u1Dst=0x%x, u1Src=0x%x\n", u4Loop, u1Dst, u1Src);

                // Invalidate DCache
                u1SrcTmp = u1Src;
                DSP_FlushInvalidateDCacheFree((UINT32)(u1SrcTmp), u4BlockSize);
                u1SrcTmp = (UINT8 *) ((UINT32) u1SrcTmp + (u4BlockSize * u4End));
                DSP_FlushInvalidateDCacheFree((UINT32)(u1SrcTmp), u4BlockSize);        
                x_thread_delay(1);
                
                // Get 786 samples , 4 Bytes.
                u1SrcTmp = u1Src;
                for (j=0; j<MIXSND_768; j++)
                {
                    u4ChNum = _gsAprocUpload2BufInfo.u4Ch;                                 
                    u1SrcTmp = u1Src;
                    for (; u4ChNum != 0; u4ChNum --)
                    {
                        //*u1SrcTmp++;
                        //*u1SrcTmp++;
                        u1SrcTmp++;
                        u1SrcTmp++;
                        *u1Dst++ = *u1SrcTmp++;
                        *u1Dst++ = *u1SrcTmp++;                        
                        u1SrcTmp = (UINT8 *) ((UINT32) u1Src + (u4BlockSize * u4End));
                    }
                    u1Src = (UINT8 *) ((UINT32) u1Src + 4);                        
                }
                    
                // Update Read Pointer
                _gsAprocUpload2BufInfo.u4Rp = u4Cur;
                // Send a notify to ARM9 for updating Rp
                vAprocCmdSet(APROC_SIG_IDX_UPLOAD2, APROC_SIG_SET_UPDATE_RP, u4Cur);

                // Loop Counter --
                u4Loop--;
                LOG(7, "Loopi=%d, End u1Dst=0x%x, u1Src=0x%x, %d\n", u4Loop, u1Dst, u1Src, j);
            }
            else
            {
                x_thread_delay(1);
            }

        }            
                
        // Write u4Size Bytes to USB...
        i4Ret = filep->f_op->write (filep, pTempUploadBuf, u4Size, &filep->f_pos);
        if (i4Ret <= 0)
        {
            LOG(0, "Write data to USB fail..\n");
            break;
        }   
        
        if (_fgForceStopCpuUpload2==TRUE)
        {
            break;
        }
        
    }

    LOG(0, "_AudCpuUpload2Thread : pAprocMixSndBuf=(V)0x%x, 0x%x\n", (UINT8*)VIRTUAL((UINT32)pAprocUploadBuf), (UINT8*)VIRTUAL((UINT32)(pAprocUploadBuf+2*3072)));
    LOG(0, "_AudCpuUpload2Thread : u4Cur=%d, u1Dst=0x%x\n", u4Cur, (UINT32)u1Dst);


_ERROR:    
    _hAudUpload2Thread = NULL_HANDLE;    
    fgAudCpuUpload2ThreadEnable = FALSE;
    _fgForceStopCpuUpload2 = FALSE;
    //Send Disable Command to ARM9    
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_ENABLE), 0); 

    // Free Temp Buffer
    VERIFY(BSP_FreeAlignedDmaMemory((UINT32)PHYSICAL((UINT32)pTempUploadBuf)));
    LOG(0, "Free pTempUploadBuf memory 0x%x \n",pTempUploadBuf);
    // Close File
    if (!IS_ERR (filep))
    {
        filp_close (filep, 0);
    }
    set_fs(oldfs);    
        
    LOG(0, "[_AudCpuUploadThread] OK Exit\n");
#endif    
}

void AUD_AprocEnableUpload2(void)
{
    INT32 i4Ret = -1;

    if (_hAudUpload2Thread)
    {
        LOG(0, "!!!! _AudUploadThread already created !!!!\n");
        return;
    }

    i4Ret = x_thread_create(&_hAudUpload2Thread,
                             "AprocCpuUpload2" ,
                              AUD_DRV_THREAD_STACK_SIZE,
                              u1MixSndThread_Priority, 
                              (x_os_thread_main_fct) _AudCpuUpload2Thread,
                              0,
                              NULL);
    if (OSR_OK != i4Ret)
    {
        LOG(0, "Create _AudUpload2Thread Fail!!!!\n");
    }
    else
    {
        LOG(0, "Create _AudUpload2Thread Succ!!!!\n");
    }
}

void AUD_AprocDisableUpload2(void)
{
    if (_hAudUpload2Thread)
    {
        LOG(0, "[AUD_DisableCpuUpload2] trigger\n");
        _fgForceStopCpuUpload2 = TRUE;
        while (!_fgForceStopCpuUpload2);
        x_thread_delay(1);
    }
    else
    {
        LOG(0, "_AudUpload2Thread is not available\n");
    }

    //Send Disable Command to ARM9  
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_UPLOAD2_ENABLE), 0);
}


//=========================================================
//         APROC RISC Post    //parson riscpost
//=========================================================
_IO_BUFFER_T _gsAprocRiscPostUpBufInfo;
_IO_BUFFER_T _gsAprocRiscPostDownBufInfo;

void AUD_AprocRiscPostUpdateWp(UINT32 u4Wp)
{
    //Update Wp (Aproc -> RISC)
    _gsAprocRiscPostUpBufInfo.u4Wp = u4Wp;
}
void AUD_AprocRiscPostUpdateRp(UINT32 u4Rp)
{
    //Update Rp (Aproc -> RISC)
    _gsAprocRiscPostDownBufInfo.u4Rp = u4Rp;
}

void AUD_AprocRiscPostInit(void)
{
    //Upload info.
    _gsAprocRiscPostUpBufInfo.u4Str = _u4DspCmptBuf[AUD_DSP0][TYPE_APROC_IDX] + 
                   u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_RISCPOST_START));
    _gsAprocRiscPostUpBufInfo.u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_RISCPOST_END));
    _gsAprocRiscPostUpBufInfo.u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_RISCPOST_SIZE));
    _gsAprocRiscPostUpBufInfo.u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_RISCPOST_CH));

    _gsAprocRiscPostUpBufInfo.u4Rp = 0;    
    _gsAprocRiscPostUpBufInfo.u4Wp = 0;
    _gsAprocRiscPostUpBufInfo.u4Rcurp = 0;    
    _gsAprocRiscPostUpBufInfo.u4Wcurp = 1;

    //Download info 
    //Upload info.
    _gsAprocRiscPostDownBufInfo.u4Str = _gsAprocRiscPostUpBufInfo.u4Str + 
                                        (_gsAprocRiscPostUpBufInfo.u4End * _gsAprocRiscPostUpBufInfo.u4Size * _gsAprocRiscPostUpBufInfo.u4Ch);   
    _gsAprocRiscPostDownBufInfo.u4End = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_RISCPOST_END));
    _gsAprocRiscPostDownBufInfo.u4Size = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_RISCPOST_SIZE));
    _gsAprocRiscPostDownBufInfo.u4Ch = u4AprocReg_Read (APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_RISCPOST_CH));

    _gsAprocRiscPostDownBufInfo.u4Rp = 0;    
    _gsAprocRiscPostDownBufInfo.u4Wp = 0;
    _gsAprocRiscPostDownBufInfo.u4Rcurp = 0;    
    _gsAprocRiscPostDownBufInfo.u4Wcurp = 1;

}

static void _AudRiscPostThread(const void* pvArg)
{
#ifdef LINUX_TURNKEY_SOLUTION
    UINT8 *pAprocRiscPostUpBuf, *pAprocRiscPostDownBuf;
    UINT16 j;
    UINT32 u4BlockSize, u4End, u4Cur, u4Wp, u4Rp, u4ChNum;
    UINT32 u4BlockSize_down, u4End_down, u4Cur_down, u4ChNum_down;    
    UINT8 *u1Dst, *u1Src, *u1SrcTmp, *u1DstTmp;
    UINT8* pTempBuf;    
    //INT32 i4Ret;
    UINT32 u4Size = 256*4*2; //256 samples, 4 bytes per sample, 2 ch

    // Allocate Temp buffer
    LOG(0, "Allocate memory size 0x%x for testing risc_post...\n", u4Size);
    pTempBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(u4Size, 32);
    if (pTempBuf == NULL)
    {
        LOG(0, "Allocate memory %d Fail !!\n", u4Size);        
        goto _ERROR;
    }
    LOG(0, "pTempBuf is (P)0x%x\n", pTempBuf);    
    pTempBuf = (UINT8*)VIRTUAL((UINT32)pTempBuf);
    LOG(0, "pTempBuf is (V)0x%x\n", pTempBuf);
    
    fgAudRiscPostThreadEnable = TRUE;

    // Init for CA9
    AUD_AprocRiscPostInit();    
    
    // Request ARM9 to reset info.
    vAprocCmdSet(APROC_SIG_IDX_RISCPOST, APROC_SIG_SET_RESET, 0);    

    // Send Enable Command to ARM9    
    x_thread_delay(20);
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_RISCPOST_ENABLE), 1);
       
    // Buffer Address
    pAprocRiscPostUpBuf = (UINT8 *)_gsAprocRiscPostUpBufInfo.u4Str;
    u4BlockSize = _gsAprocRiscPostUpBufInfo.u4Size; 
    u4End = _gsAprocRiscPostUpBufInfo.u4End ;
    u4Cur =  _gsAprocRiscPostUpBufInfo.u4Rcurp;
    u4ChNum = _gsAprocRiscPostUpBufInfo.u4Ch;       

    pAprocRiscPostDownBuf = (UINT8 *)_gsAprocRiscPostDownBufInfo.u4Str;
    u4BlockSize_down = _gsAprocRiscPostDownBufInfo.u4Size; 
    u4End_down = _gsAprocRiscPostDownBufInfo.u4End ;
    u4Cur_down =  _gsAprocRiscPostDownBufInfo.u4Wcurp;
    u4ChNum_down = _gsAprocRiscPostDownBufInfo.u4Ch;           

    LOG(0, "RiscPost : pAprocRiscPostUpBuf=(V)0x%x, BlkSize=%d, u4End=%d, Cur=%d, ChNum=%d\n", VIRTUAL((UINT32)pAprocRiscPostUpBuf), u4BlockSize, u4End, u4Cur, u4ChNum);
    LOG(0, "RiscPost : pAprocRiscPostDownBuf=(V)0x%x, BlkSize=%d, u4End=%d, Cur=%d, ChNum=%d\n", VIRTUAL((UINT32)pAprocRiscPostDownBuf), u4BlockSize, u4End_down, u4Cur_down, u4ChNum_down);

    u1Dst = (UINT8*)VIRTUAL((UINT32)pTempBuf); // to fix uninitialized build error
    
    while (1)
    {
        if (_fgForceStopRiscPost==TRUE)
        {
            break;
        }
         
        u1Dst = (UINT8*)VIRTUAL((UINT32)pTempBuf);
        // ------------------------
        // Get data from APROC
        // ------------------------
        // check if ready to Read
        while (1)
        {
            u4Wp = _gsAprocRiscPostUpBufInfo.u4Wp;  //Wp from APROC
            if (u4Cur != u4Wp) 
            {
                // upadte current pointer
                u4Cur ++;
                if (u4Cur >= u4End)
                {
                    u4Cur -= u4End;
                }
                // Update Read Current Pointer
                _gsAprocRiscPostUpBufInfo.u4Rcurp = u4Cur;
                
                // Get data from APROC
               u1Src =(UINT8*)VIRTUAL((UINT32)(pAprocRiscPostUpBuf + (u4Cur * u4BlockSize)));

                // Invalidate DCache
                u1SrcTmp = u1Src;
                DSP_FlushInvalidateDCacheFree((UINT32)(u1SrcTmp), u4BlockSize);
                u1SrcTmp = (UINT8 *) ((UINT32) u1SrcTmp + (u4BlockSize * u4End));
                DSP_FlushInvalidateDCacheFree((UINT32)(u1SrcTmp), u4BlockSize);       
                //x_thread_delay(1);                
                
                // Get 256 samples , every sample is 4 Bytes.
                // Source is L L L L ....  R R R R ........ (256 samples)
                // Dst is  L R L R ... (256 samples)
                for (j=0; j<MIXSND_256; j++)
                {
                    u4ChNum = _gsAprocRiscPostUpBufInfo.u4Ch;                                 
                    u1SrcTmp = u1Src;
                    for (; u4ChNum != 0; u4ChNum --)
                    {
                        x_memcpy (u1Dst, (UINT8*)VIRTUAL((UINT32)u1SrcTmp), 4);
                        // goto next channel
                        u1SrcTmp = (UINT8 *) ((UINT32) u1Src + (u4BlockSize * u4End));
                        u1Dst += 4 ;
                    }
                    u1Src = (UINT8 *) ((UINT32) u1Src + 4);                        
                }                    
                // Update Read Pointer
                _gsAprocRiscPostUpBufInfo.u4Rp = u4Cur;
                // Send a notify to ARM9 for updating Rp
                vAprocCmdSet(APROC_SIG_IDX_RISCPOST, APROC_SIG_SET_UPDATE_RP, u4Cur);

                break;
            }
            else
            {
                x_thread_delay(1);
            }
            if (_fgForceStopRiscPost==TRUE)
            {
                break;
            }                    
        }//RCur != Wp loop
        // ------------------------
        // Do Post Function
        // ------------------------
               
        //u1Src = (INT32*)VIRTUAL((UINT32)pTempBuf);
        u1Src = (UINT8*)VIRTUAL((UINT32)pTempBuf);
        for (j=0; j<=MIXSND_256*_gsAprocRiscPostDownBufInfo.u4Ch; j++)
        {
            *(INT32*)u1Src /=  2;
            u1Src += 4;
        }
        LOG(7, "R Cur=%d,Dst=0x%x,0x%08x,0x%08x,0x%08x,0x%08x\n", u4Cur, (UINT32)u1Dst, *(UINT32*)(u1Src),*(UINT32*)(u1Src+4),*(UINT32*)(u1Src+8),*(UINT32*)(u1Src+12));

        // ------------------------
        // Put data down to APROC
        // ------------------------
        u1Src = (UINT8*)VIRTUAL((UINT32)pTempBuf);        
        while (1)
        {
            u4Rp = _gsAprocRiscPostDownBufInfo.u4Rp;  //Rp from APROC        
            if (u4Cur_down != u4Rp) 
            {
                // Get data from ARM9 for 768 samples , 2ch
                u1Dst =(UINT8*)VIRTUAL((UINT32)(pAprocRiscPostDownBuf + (u4Cur_down * u4BlockSize)));

                // Put 256 samples , every sample is 4 Bytes.
                // Copy L R L R ... (256 samples) to L L L L ....  R R R R ........ (256 samples)
                for (j=0; j<MIXSND_256; j++)
                {
                    u4ChNum = _gsAprocRiscPostDownBufInfo.u4Ch;
                    u1DstTmp = u1Dst;
                    for (; u4ChNum != 0; u4ChNum --)
                    {
                        x_memcpy (u1DstTmp, (UINT8*)VIRTUAL((UINT32)u1Src), 4);
                        // goto next channel
                        u1DstTmp = (UINT8 *) ((UINT32) u1Dst + (u4BlockSize * u4End));
                        u1Src += 4 ;
                    }
                    u1Dst = (UINT8 *) ((UINT32) u1Dst + 4);                        
                }
                // Flush Invalidate DCache
                u1DstTmp = (UINT8*)VIRTUAL((UINT32)(pAprocRiscPostDownBuf + (u4Cur_down * u4BlockSize)));
                DSP_FlushInvalidateDCacheFree((UINT32)(u1DstTmp), u4BlockSize);
                u1DstTmp = (UINT8 *) ((UINT32) u1DstTmp + (u4BlockSize * u4End));
                DSP_FlushInvalidateDCacheFree((UINT32)(u1DstTmp), u4BlockSize);    
                //x_thread_delay(1);                

                // Update Write Pointer
                _gsAprocRiscPostDownBufInfo.u4Wp = u4Cur_down;                
                // Send a notify to ARM9 for updating Wp (RISC W to APROC)
                vAprocCmdSet(APROC_SIG_IDX_RISCPOST, APROC_SIG_SET_UPDATE_WP, u4Cur_down);                

                // upadte current pointer
                u4Cur_down ++;
                if (u4Cur_down >= u4End)
                {
                    u4Cur_down -= u4End;
                }
                // Update Read Current Pointer
                _gsAprocRiscPostDownBufInfo.u4Wcurp = u4Cur_down;

                break;
            }
            else
            {
                x_thread_delay(1);
            }
            if (_fgForceStopRiscPost==TRUE)
            {
                break;
            }                    
        }//WCur != Rp loop

    }//Thread Loop

    LOG(0, "_AudRiscPostThread : u4Cur=%d, u1Dst=0x%x\n", u4Cur, (UINT32)u1Dst);
_ERROR:    
    // Free Temp Buffer
    pTempBuf = (UINT8*)PHYSICAL((UINT32)pTempBuf);
    VERIFY(BSP_FreeAlignedDmaMemory((UINT32)pTempBuf));
    LOG(0, "Free pTempBuf (P)0x%x memory ok!\n", (UINT32)pTempBuf);
    
    _hAudRiscPostThread = NULL_HANDLE;    
    fgAudRiscPostThreadEnable = FALSE;
    _fgForceStopRiscPost = FALSE;
    //Send Disable Command to ARM9    
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_RISCPOST_ENABLE), 0); 

    LOG(0, "[_AudRiscPostThread] OK Exit\n");
#endif    
}


void AUD_AprocEnableRiscPost(void)
{
    INT32 i4Ret = -1;

    if (_hAudRiscPostThread)
    {
        LOG(0, "!!!! _AudRiscPostThread already created !!!!\n");
        return;
    }

    vAprocCmdSet(APROC_SIG_IDX_RISCPOST, APROC_SIG_SET_DEV_WORKING, 0);            
    i4Ret = x_thread_create(&_hAudRiscPostThread,
                             "AprocRiscPost" ,
                              AUD_DRV_THREAD_STACK_SIZE,
                              u1MixSndThread_Priority, 
                              (x_os_thread_main_fct) _AudRiscPostThread,
                              0,
                              NULL);
    if (OSR_OK != i4Ret)
    {
        LOG(0, "Create _AudRiscPostThread Fail!!!!\n");
    }
    else
    {
        LOG(0, "Create _AudRiscPostThread Succ!!!!\n");
    }
}

void AUD_AprocDisableRiscPost(void)
{
    if (_hAudRiscPostThread)
    {
        LOG(0, "[_AudRiscPostThread] trigger exit\n");
        _fgForceStopRiscPost = TRUE;
        while (!_fgForceStopRiscPost);
        x_thread_delay(1);
    }
    else
    {
        LOG(0, "_AudRiscPostThread is not available\n");
    }

    //Send Disable Command to ARM9  
    vAprocReg_Write(APROC_ASM_ADDR (APROC_ASM_ID_CPU_0, APROC_REG_IOBUF_RISCPOST_ENABLE), 0);
    vAprocCmdSet(APROC_SIG_IDX_RISCPOST, APROC_SIG_SET_DEV_SLEEP, 0);    
}

#endif //(CC_AUD_ARM_SUPPORT) && (CC_AUD_ARM_RENDER)



#endif //CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
