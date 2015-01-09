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
 * $RCSfile:  $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 *
 * Scaler Help 264 Decoder to Write DRAM
 *
 *---------------------------------------------------------------------------*/


////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////
#ifdef CC_SUPPORT_VENC

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "sv_const.h"
#include "vdo_misc.h"
#include "video_def.h"
#include "video_timing.h"

#include "x_ckgen.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_util.h"
#include "x_hal_5381.h"

#include "fbm_drvif.h"
#include "drv_mon_out.h"
#include "venc_drvif.h"
#include "venc_util_drvif.h"
#include "drv_scaler.h"
#include "scpos_debug.h"
#include "vdp_venc_ctrl.h"



//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum {
    E_SCALER_VENC_IDLE,
    E_SCALER_WAIT_BUF,
    E_SCALER_WRITING,
    E_SCALER_OUT_OF_ISR,
    E_SCALER_DONE	
} E_SCALER_VENC_STATE;


// for debug
BOOL bEncInputDbg = FALSE; 
BOOL bEncFldModeDbg = FALSE;
UINT8  eEncInputDbg = 0;
UINT8  eEncFldTypeDbg = 1;

UINT8 bVss420=OFF;
UINT8 Vss420DropFrame;
UINT8 Vss420Frame_M;
UINT8 Vss420Frame_N;
static UINT16 u2VssCnt=0;
static UINT8  fgPreFieldStatus=0;

extern void (*_pfVSS420Cb)(void);

#define SCALER_NORMAL_MODE (0)
#define SCALER_H264_MODE   (1)

#define SCALER_CONTINUE_MODE (0)
#define SCALER_READY_MODE    (1)

#define DRAM_FIELD_MODE  (0)
#define DRAM_FRAME_MODE  (1)


typedef enum {
    E_SCALER_VENC_CMD_START,
    E_SCALER_VENC_CMD_STOP
} E_SCALER_VENC_CMD;  

typedef struct _ScalerVencInfo
{
    UINT8  u1Init;                   // initial
    UINT8  u1Input;                  // input src: main/sub
    UINT8  u1Src;                    // venc engine: main/sub/mon
    UINT8  u1Interlace;              // interlace source
    UINT16 wDumpW;                   //(in pixel) output width
    UINT16 wDumpH;                   //(in pixel) output height
    UINT8  u1Mode;                   // ready/continuous
    UINT8  u1H264En;                 // Normal mode or H.264 mode
    UINT8  u1Idx;                    // frame index
    UINT8  u1DropEn;                 // drop enable
    UINT8  u1DropM;                  // Drop M
    UINT8  u1DropN;                  // Drop N
    VENC_FRM_T tAddr[3];             // VENC dram address 0~2
} ScalerVencInfo;


//static E_SCALER_VENC_CMD   _u1ScalerVencCmd = E_SCALER_VENC_CMD_STOP; //

#define scalerVencDlyCounter 10

#define ISR_LEAK_INTERRUPT_WA
//#define CLI_DUMP_SCALER_DRAM_TO_USB
#define WEAVE_MODE_SEND_AT_BOT_FLD


static UINT16 wScalerDlyCounter = 0;    //for scaler delay notify VENC read frame buffer
static BOOL bScalerClearBit = FALSE;
static UINT32 u4ISRLeakIntrpCnt = 0 ;

E_SCALER_VENC_RECORD_MODE eScalerVencFldMode = E_SCALER_VENC_PROGRESSIVE;
E_SCALER_VENC_INPUT_FLE_INFO eScalerVencFldInfo = E_SCALER_NOT_INI;



//static VENC_BUFMGR_BASE_T *_ptScalerVEncFrmBuffMgr;         // 6:4
static VENC_BUFMGR_BASE_T *_ptScalerVEncFrmBuffMgrResize;   // 6:6
static VENC_HANDLE   _hScalerVEnc;                          // 6:4
static VENC_IN_PIC_T _tScalerVEncIn;                        // 8:8
//static VENC_FRM_T    _tScalerVEncFrm;                       // 15:11
//static VENC_FILTER_T * _ptScalerVEncFilter;
//static UINT32 _u4ScalerWriteProtectLow, _u4ScalerWriteProtectHigh;
static ScalerVencInfo _tScalerVencPrm =
{
    0,        // init
    0xff,     // input
    0xff,     // src
    0,        // interlace
    0,        // width
    0,        // height
    0,        // mode: ready/continuous
    0,        // Normal mode or H.264 mode
    0,        // frame index
    0,        // drop enable
    0,        // Drop M
    0,        // Drop N
    {{0}, {0}, {0}} // addr
};

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern UINT32 _AUD_ReadPsrStc1(VOID);
extern VDP_VENC_INFO_t       tVencInfo;
extern VDP_VENC_INPUT_RES_t  tVencRes;




//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/**
 * @brief Send frame to video encoder routine
 */
void vDrvScpipVEncISR(void)
{
    static UINT8 u1PreIdx,u1PreFldInfo; 
    UINT8 u1Idx = u1DrvScpipH264GetCurrentFrameIdx();

    if(u1Idx > 2) u1Idx = 2; // for klocwork warning

    if(wScalerDlyCounter != 0)
    {   
        LOG(5,"u1Idx:%d  wScalerDlyCounter:%d\n",u1Idx, wScalerDlyCounter);
        LOG(5,"ReadyBit%d:%d\n",u1Idx, u1DrvScpipH264GetCurrentReadyBit(u1Idx));
        LOG(5,"ReadyBit%d:%d\n",(u1Idx+1)%3, u1DrvScpipH264GetCurrentReadyBit((u1Idx+1)%3));
        LOG(5,"ReadyBit%d:%d\n",(u1Idx+2)%3, u1DrvScpipH264GetCurrentReadyBit((u1Idx+2)%3));  
        
        vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src,u1Idx); 
        vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src,(u1Idx+1)%3); 
        vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src,(u1Idx+2)%3); 

        u4ISRLeakIntrpCnt = 0;
        u1PreIdx = u1Idx;	
        u1PreFldInfo = eScalerVencFldInfo;
        wScalerDlyCounter --;
        
        return ;           
    }
    
    if(_tScalerVencPrm.u1Src == E_VENC_MODULE_MAIN_SCALER || _tScalerVencPrm.u1Src == E_VENC_MODULE_SUB_SCALER)
    {
        if (bDrvVideoSignalStatus(_tScalerVencPrm.u1Src) != SV_VDO_STABLE)
        {
            LOG(0,"vDrvScpipVEncISR Venc Path %d is not stable\n",_tScalerVencPrm.u1Src);
            return;
        } 
    }
    else if(_tScalerVencPrm.u1Src == E_VENC_MODULE_MON_SCALER)
    {
        if(vMonOutGetDramReadEnable()!= 1 || vMonOutGetDramWriteEnable()!= 1)
        {
            LOG(0,"vDrvScpipVEncISR Venc Path %d is not stable\n",_tScalerVencPrm.u1Src);
            return; 
        }    
    }
    else
    {
        LOG(3, "Encoder Module %d %s Retrun \n", _tScalerVencPrm.u1Src, __FUNCTION__);
        return;
    }
 
    if(bScalerClearBit == TRUE)
    {      
        bScalerClearBit = FALSE; 
        
        LOG(5,"First Nty Index :%d\n",u1Idx);
        LOG(5,"First Nty ReadyBit%d:%d\n",u1Idx, u1DrvScpipH264GetCurrentReadyBit(u1Idx));
        LOG(5,"First Nty ReadyBit%d:%d\n",(u1Idx+1)%3, u1DrvScpipH264GetCurrentReadyBit((u1Idx+1)%3));
        LOG(5,"First Nty ReadyBit%d:%d\n",(u1Idx+2)%3, u1DrvScpipH264GetCurrentReadyBit((u1Idx+2)%3));      
    }

    if (VENC_NULL_HANDLE == _hScalerVEnc)
    {
        LOG(3, "%s: _hScalerVEnc is NULL!!\n", __FUNCTION__);
        return;
    }
        
    #ifdef CC_AUD_DATA_UPLOAD_SUPPORT
        _tScalerVEncIn.u4Pts = _AUD_ReadPsrStc1();
    #endif

    LOG(5,"Index  :        %d\n",u1Idx);
    LOG(5,"FldInf :        %c\n",eScalerVencFldInfo == 0?'T':'B');

   // Weave write dram for i timing
    if(vScpipH264GetFrameMode(_tScalerVencPrm.u1Src) == DRAM_FIELD_MODE && eScalerVencFldMode == E_SCALER_VENC_BOTH_FLD)
    { 
    #ifdef WEAVE_MODE_SEND_AT_BOT_FLD
        if(eScalerVencFldInfo == E_SCALER_TOP_FLD || 
            (eScalerVencFldInfo == E_SCALER_BOT_FLD && eScalerVencFldInfo == u1PreFldInfo))             // repeat B fld should drop && when write doen T fld the frame is not ready so should return
        {
            u1PreFldInfo = eScalerVencFldInfo;
            vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src,u1Idx);  
            return;
        }
        else
        {
            u1PreFldInfo = eScalerVencFldInfo;
        } 

        if(_tScalerVencPrm.u1Src == E_VENC_MODULE_MON_SCALER)
        {
           u1Idx = (u1Idx+2)%3; //Monitor out write dram enable the ahead interupt so should  send preinx to encoder
        }
    #else
        if(eScalerVencFldInfo == E_SCALER_BOT_FLD)             // repeat T fld should drop && when write doen T fld send address to venc
        {
            u1PreFldInfo = eScalerVencFldInfo;
            return;             
        }
        else if(eScalerVencFldInfo == E_SCALER_TOP_FLD && eScalerVencFldInfo == u1PreFldInfo)
        {
            vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src,u1Idx);
            u1PreFldInfo = eScalerVencFldInfo;
            return ;
        } 
        else
        {
            vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src,u1Idx);
            u1PreFldInfo = eScalerVencFldInfo;
        }

        if(_tScalerVencPrm.u1Src == E_VENC_MODULE_MON_SCALER)
        {
           u1Idx = (u1Idx+2)%3; //Monitor out write dram enable the ahead interupt so should  send preinx to encoder
        }
     #endif
    }

    _tScalerVEncIn.u4YAddr = _tScalerVencPrm.tAddr[u1Idx].u4YAddr << 4;
    _tScalerVEncIn.u4CAddr = _tScalerVencPrm.tAddr[u1Idx].u4UVAddr << 4;
    _tScalerVEncIn.fgRzSrc = (_ptScalerVEncFrmBuffMgrResize)? TRUE : FALSE;
    

    #ifdef ISR_LEAK_INTERRUPT_WA
    if(eScalerVencFldMode != E_SCALER_VENC_BOTH_FLD)
    {
        if((u1PreIdx+1)%3 != u1Idx)
        {
            //vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src,u1Idx); 
            vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src,(u1Idx+1)%3); 
            vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src,(u1Idx+2)%3); 
            u1PreIdx = u1Idx;
            LOG(2, "IRS leak send interrupt counter %d\n",++u4ISRLeakIntrpCnt);
        }
        else
        {
           u1PreIdx = u1Idx;
        }
    }
    #endif

    if(((eScalerVencFldMode == E_SCALER_VENC_TOP_FLD)&&(eScalerVencFldInfo == E_SCALER_BOT_FLD))
        || ((eScalerVencFldMode == E_SCALER_VENC_BOT_FLD)&&(eScalerVencFldInfo == E_SCALER_TOP_FLD)))
    {
            u1DrvScpipH264FreeCurrentFb(_tScalerVEncIn.u4YAddr);
            return;
    }
    
    if (VENC_OK != _VENC_EncFrameAsync(_hScalerVEnc, &_tScalerVEncIn, NULL))
    {
        LOG(0, "%s Send Address to VENC Fail\n", __FUNCTION__);
        ASSERT(0);
    }

}

/*
VSS procee in VENC path 
The buffer content will be store with YUV420 NV12 format
*/
void vDrvScpipVssDropFieldInt( void )
{
	UINT8 u1Idx; // = u1DrvScpipH264GetCurrentFrameIdx();
	u1Idx = u1DrvScpipH264GetCurrentFrameIdx();
	vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src,u1Idx);  
}

void vDrvScpipVss420DropMN( UCHAR u1DropRatio)
{
   Vss420Frame_M=u1DropRatio & 0x0f;
   Vss420Frame_N=(u1DropRatio >> 4) &0x0f;
   LOG(0,"VSS420 drop n/M(%d/%d)\n",Vss420Frame_N,Vss420Frame_M);
}

UCHAR vDrvScpipGetVss420DropMN( void )
{
   return((Vss420Frame_N<<4)|Vss420Frame_M);
}



void vDrvScpipVss420ISR( void )
{
   LOG(0,"VSS420 ISR cnt(%d)\n",u2VssCnt);
   u2VssCnt++;
    if(vScpipH264GetFrameMode(_tScalerVencPrm.u1Src) == DRAM_FIELD_MODE && eScalerVencFldMode == E_SCALER_VENC_BOTH_FLD)
    {
       if(eScalerVencFldInfo == E_SCALER_TOP_FLD)
       {
		  //top field
            fgPreFieldStatus=eScalerVencFldInfo;
			vDrvScpipVssDropFieldInt();
            return;
       }
	   else
	   {   //bottom field
	       if ( fgPreFieldStatus == E_SCALER_BOT_FLD )
	       {
	          u1DrvScpipH264TriggerNext();
			  return;
	       }
	       fgPreFieldStatus=eScalerVencFldInfo;
	   }
    }
	
    Vss420DropFrame += Vss420Frame_N;
	if ( Vss420DropFrame < Vss420Frame_M )
	{
		 u1DrvScpipH264TriggerNext();
	}
	else
    {   
        Vss420DropFrame-=Vss420Frame_M;
	if ( _pfVSS420Cb != NULL )
	   _pfVSS420Cb(); //venc 420 buffer write done, trigger venc call back	  
  }   
}


void u1DrvScpipH264forVss(UINT8 u1Vss420_OnOff)
{
   bVss420=u1Vss420_OnOff;
   if ( bVss420 == ON )
   	   Vss420DropFrame=0;
}


/**
 * @brief Drop half frame due to the venc hw limitation
 * @param fgOnOff   TREU drop half frame   FALSE not drop
 */
void vDrvScpipVencDropFrame(UINT8 u1Src, UINT8 fgOnOff, UINT8 u1dropM, UINT8 u1dropN)
{
    if (u1Src > E_VENC_MODULE_MON_SCALER || u1dropN > u1dropM || u1dropM > 0xff || u1dropN > 0xff )
    {
        LOG(0, "%s Function Fail Src:%d  dropM:%d  dropN:%d\n", __FUNCTION__, u1Src, u1dropM, u1dropN); 
        return;
    }
    
    _tScalerVencPrm.u1DropEn = fgOnOff;
    _tScalerVencPrm.u1DropM = u1dropM;
    _tScalerVencPrm.u1DropN = u1dropN;

    vScpipH264SetDropFrame(u1Src,fgOnOff,u1dropM,u1dropN);
}

/**
 * @brief H.264 initialize
 */
void vDrvScpipSupportH264Init(void) //TODO
{
    //_ptScalerVEncFrmBuffMgr       = NULL;
    _ptScalerVEncFrmBuffMgrResize = NULL;
    _hScalerVEnc = NULL;
//    _ptScalerVEncFilter = NULL;
}

/**
 * @brief Set H.264 encoder handle
 */
void vDrvScpipSetVEncHandle(void *handle, void *ptFrmBuffMgr, void *ptFrmBuffMgrResize, void *ptFilterPts) //TODO
{
   // _ptScalerVEncFrmBuffMgr       = ptFrmBuffMgr;
    _ptScalerVEncFrmBuffMgrResize = ptFrmBuffMgrResize;
    _hScalerVEnc = handle;
  //  _ptScalerVEncFilter = (VENC_FILTER_T *)ptFilterPts;
}

/**
 * @brief Query whether Venc is started
 **/
#if 0
UINT8 _u1DrvScpipVencRunning(void)
{
    if (_u1ScalerVencCmd == E_SCALER_VENC_CMD_STOP)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }   
}
#endif


/**
 * @brief Disable send buffer to VENC
 **/
void vDrvScpipStopSendBufToVENC(BOOL bFlag)
{
    if (bFlag)
    {
//        _u1HalfFps = SV_OFF;
        //_u1ScalerVencCmd = E_SCALER_VENC_CMD_STOP;
    }
    else
    {
        //_u1ScalerVencCmd = E_SCALER_VENC_CMD_START;

    }
}

void vDrvScpipVencFldRecordMode(E_SCALER_VENC_RECORD_MODE eMode)
{   
    eScalerVencFldMode = eMode;
    
    LOG(3, "Set Venc Fld Record Mode %d\n",eScalerVencFldMode);
}

UINT8 u1DrvScpipGetVencRecordMode(void)
{
    return eScalerVencFldMode;
}


void u1DrvScpipH264Input(UINT8 u1Src)
{
    if (u1Src > MON_SIGNAL_FROM_OSTG_OSD)
    {
        LOG(0, "Invalid Venc Signal input from %d!!\n", u1Src);
        return;
    }

    if (_tScalerVencPrm.u1Input != u1Src)
    {
        LOG(1, "VENC src input [%d]=>[%d]\n", _tScalerVencPrm.u1Input, u1Src);
        _tScalerVencPrm.u1Input  = u1Src;
    }    
}

void u1DrvScpipH264SourceSel(UINT8 u1Src)
{
    if (u1Src > E_VENC_MODULE_MON_SCALER)
    {
        LOG(0, "Invalid input source %d!!\n",u1Src);
        return;
    }
    
    if (_tScalerVencPrm.u1Src != u1Src)
    {
        LOG(1, "VENC src [%d]=>[%d]\n",_tScalerVencPrm.u1Src, u1Src);
        _tScalerVencPrm.u1Src  = u1Src;
    }

    if(u4DrvVrmGetAppFlag() & VRM_APP_VIDEO_ENC) // VRM trigger
    {
        if (u1Src == E_VENC_MODULE_MAIN_SCALER)
        {
            vVRMSetEventFlg(SV_VP_MAIN, VRM_EVENT_BY_APP);
        }
        else if (u1Src == E_VENC_MODULE_SUB_SCALER)
        {
            vVRMSetEventFlg(SV_VP_PIP,  VRM_EVENT_BY_APP);
            vScpipReconfigScl(VDP_2);//config back dram mode
        }
		else if (u1Src == E_VENC_MODULE_MON_SCALER)   //monintor out venc
		{		   
			vMonOutSetOutput(E_MON_OUT_VENC);	 // monitor out
		}
		else
		{
			LOG(0, "Error: do not seclect signal input!!\n");
		}

    }
}

void u1DrvScpipH264ReadyMode(UINT8 u1WMode)
{
    if (_tScalerVencPrm.u1Src > E_VENC_MODULE_MON_SCALER)
    {
        LOG(0, "Invalid input source!!\n");
        return;
    }

    _tScalerVencPrm.u1Mode = u1WMode;
    
    vScpipH264SetReadyMode(_tScalerVencPrm.u1Src, u1WMode);      // ready mode
}

void u1DrvScpipH264Enable(UINT8 u1Mode)
{
    if (_tScalerVencPrm.u1Src > E_VENC_MODULE_MON_SCALER)
    {
        LOG(0, "Invalid input source!!\n");
        return;
    }

    if(u1Mode == SCALER_NORMAL_MODE && vScpipH264GetReadyMode(_tScalerVencPrm.u1Src) == SCALER_READY_MODE)
    {
       vScpipH264SetReadyMode(_tScalerVencPrm.u1Src, SCALER_CONTINUE_MODE); // when h264 write norm, clear ready mode 
    }

    if (u1Mode == SCALER_H264_MODE)
    {
        switch(_tScalerVencPrm.u1Src)
        {
            case E_VENC_MODULE_MAIN_SCALER:

                break;
            case E_VENC_MODULE_SUB_SCALER:
                vScpipSetISREn(ISR_DRAM_SUB_WRITE,1);
                if(eScalerVencFldMode != E_SCALER_VENC_PROGRESSIVE)
                {
                    vScpipSetISREn(ISR_FSC_SUB_VSYNC_IN_FALLING,1);     // sub input vsync falling
                }
				
                break;
            case E_VENC_MODULE_MON_SCALER:
                vScpipSetISREn(ISR_DRAM_MON_WRITE,1);
                break;
            default:
                LOG(0, "Set ISR_DRAM_MON_WRITE Fail!!\n");
                break;
        }
        
        wScalerDlyCounter = scalerVencDlyCounter;         //delay  counter
        bScalerClearBit = TRUE;
    }
    else if (u1Mode == SCALER_NORMAL_MODE)
    {
        switch(_tScalerVencPrm.u1Src)
        {
            case E_VENC_MODULE_MAIN_SCALER:
                            
                break;
            case E_VENC_MODULE_SUB_SCALER:
                vScpipSetISREn(ISR_DRAM_SUB_WRITE,0);               
                if(eScalerVencFldMode != E_SCALER_VENC_PROGRESSIVE)
                {
                    vScpipSetISREn(ISR_FSC_SUB_VSYNC_IN_FALLING,0);     // sub input vsync falling
                }
                break;
            case E_VENC_MODULE_MON_SCALER:
                vScpipSetISREn(ISR_DRAM_MON_WRITE,0);               
                break;
            default:
                LOG(0, "Disable ISR_DRAM_MON_WRITE Fail!!\n");
                break;
        }
        
        wScalerDlyCounter = 0;                                          //clear delay counter
        bScalerClearBit = FALSE;
        vDrvScpipVencDropFrame(_tScalerVencPrm.u1Src, 0, 0, 0);         //disable frame drop      
        vScpipH264SetEndVCnt(_tScalerVencPrm.u1Src,0,1079);
    }

    _tScalerVencPrm.u1H264En = u1Mode;
    
    if (u1Mode == SCALER_H264_MODE)
    {
        if(eScalerVencFldMode == E_SCALER_VENC_TOP_FLD || eScalerVencFldMode == E_SCALER_VENC_BOT_FLD || eScalerVencFldMode == E_SCALER_VENC_PROGRESSIVE)
        {
           vScpipH264SetFrameMode(_tScalerVencPrm.u1Src, DRAM_FRAME_MODE);              //encoder one field  Frame mode  
        }
        else if(eScalerVencFldMode == E_SCALER_VENC_BOTH_FLD)
        {
           vScpipH264SetFrameMode(_tScalerVencPrm.u1Src, DRAM_FIELD_MODE);  // Weave:Field mode   

        }
		
		if(_tScalerVencPrm.u1Src == E_VENC_MODULE_MON_SCALER && eScalerVencFldMode == E_SCALER_VENC_BOTH_FLD)
		{
		   vScpipH264SetEndVCnt(_tScalerVencPrm.u1Src,1,5); 
		}
    }
    else if (u1Mode == SCALER_NORMAL_MODE) // SCALER_NORMAL_MODE && After ECO
    { 
        vScpipH264SetFrameMode(_tScalerVencPrm.u1Src, DRAM_FRAME_MODE); // set to frame mode as default
    }

    vScpipH264SetChromaLine(_tScalerVencPrm.u1Src, 1);                                      // odd/even line
    vScpipH264SetWriteMode(_tScalerVencPrm.u1Src, u1Mode);                                  // enable 264 write   
    vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src, 0);       // clear next ready bit
    vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src, 1);       // clear next ready bit
    vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src, 2);       // clear next ready bit
    
}

void u1DrvScpipH264TriggerNext(void)
{
    UINT8 u1Idx;
    
    if (_tScalerVencPrm.u1Src > E_VENC_MODULE_MON_SCALER)
    {
        LOG(0, "Invalid input source!!\n");
        return;
    }

    if (_tScalerVencPrm.u1Mode != SCALER_READY_MODE)
    {
        LOG(0, "Not ready mode!!\n");    
    }
    
    u1Idx = u1DrvScpipH264GetCurrentFrameIdx();
    u1Idx = (u1Idx+1)%3;
    vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src, u1Idx);       // clear next ready bit

    LOG(5,"ScpipH264TriggerNext Index :%d\n",u1Idx);
}

void u1DrvScpipH264FreeCurrentFb(UINT32 u4YAddr)
{ 
    if (_tScalerVencPrm.u1Src > E_VENC_MODULE_MON_SCALER)
    {
        LOG(0, "Invalid input source!!\n");
        return;
    }

    if (_tScalerVencPrm.u1Mode != SCALER_READY_MODE)
    {
        LOG(0, "Not ready mode!!\n");    
    }

    if (u4YAddr == 0)
    {
        LOG(0, "u4YAddr IS NULL!!\n");    
    }
    
    
    if(_tScalerVencPrm.tAddr[0].u4YAddr << 4 == u4YAddr)
    {
        vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src, 0);       // clear  ready bit 0
        LOG(5, "VENC Clear ReadyBit 0 !!\n");
    }
    else if(_tScalerVencPrm.tAddr[1].u4YAddr << 4 == u4YAddr)
    {
        vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src, 1);       // clear  ready bit 1
        LOG(5, "VENC Clear ReadyBit 1 !!\n"); 
    }
    else if(_tScalerVencPrm.tAddr[2].u4YAddr << 4 == u4YAddr)
    {
        vScpipH264ClrReadyIdx(_tScalerVencPrm.u1Src, 2);       // clear  ready bit 2
        LOG(5, "VENC Clear ReadyBit 2 !!\n");
    }
    else
    {
        LOG(5, "VENC Clear ReadyBit Fail!!\n");
    }  
}


#ifdef CLI_DUMP_SCALER_DRAM_TO_USB
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/param.h>

static INT32 u1DumpData (CHAR *pcFileName, const INT8 *buf, size_t count)
{
    mm_segment_t oldfs;
    struct file *filep;
    size_t ret;

    oldfs = get_fs ();
    set_fs (KERNEL_DS);
    filep = filp_open (pcFileName, (O_CREAT | O_WRONLY), 0);
    if (IS_ERR (filep))
    {
        set_fs (oldfs);
        LOG(0,"unable to open file: %s\n", pcFileName);
        return -1;
    }

    ret = filep->f_op->write (filep, buf, count, &filep->f_pos);

    filp_close (filep, 0);
    set_fs(oldfs);

    return 0;
}
#endif

UINT8 u1DrvScpipDumpDataToUsb(void)
{
#ifdef CLI_DUMP_SCALER_DRAM_TO_USB
    UINT32 u1Idx = 0, u4YSize = 0, u4CSize = 0;
    CHAR ucFileName[255];
    
    memset(ucFileName, 0, sizeof(ucFileName));

    if(_tScalerVencPrm.u1H264En != SCALER_H264_MODE)
    {
        LOG(0,"Fail:please open venc first then dump dram\n");
        return 0;
    }

    for(u1Idx = 0; u1Idx < 3; u1Idx++)
    {
        u4YSize = (_tScalerVencPrm.tAddr[u1Idx].u4UVAddr << 4) - (_tScalerVencPrm.tAddr[u1Idx].u4YAddr << 4);
        u4CSize = u4YSize >> 1;
        
        //for android /mnt/usbdisk/dump1/venc_dump_input_Y_%03d.bin
        snprintf(ucFileName, sizeof(ucFileName), "/mnt/usb/sda1/dump1/venc_dump_input_Y_%03d.bin", u1Idx);
        u1DumpData(ucFileName, (INT8 *)(VIRTUAL(_tScalerVencPrm.tAddr[u1Idx].u4YAddr << 4)), u4YSize);
        
        snprintf(ucFileName, sizeof(ucFileName), "/mnt/usb/sda1/dump1/venc_dump_input_C_%03d.bin", u1Idx);
        u1DumpData(ucFileName, (INT8 *)(VIRTUAL(_tScalerVencPrm.tAddr[u1Idx].u4UVAddr << 4)), u4CSize); 
    } 
    
    return 1;
#else
    return 0;
#endif
}

void u1DrvScpipH264UpdateResolution(UINT16 u2Width, UINT16 u2Height, UINT8 u1Interlace)
{
    if (_tScalerVencPrm.u1Src > E_VENC_MODULE_MON_SCALER)
    {
        LOG(0, "Invalid input source!!\n");
        return;
    }

    _tScalerVencPrm.wDumpW = u2Width;
    _tScalerVencPrm.wDumpH = u2Height;
    _tScalerVencPrm.u1Interlace = u1Interlace;
}

void u1DrvScpipH264SetDramAddress(UINT32 u4BaseAddr, UINT32 u4YFrameSize)
{
    UINT8 u1Idx=0;
    UINT32 u4CFrameSize, u4FrameSize;

    if (_tScalerVencPrm.u1Src > E_VENC_MODULE_MON_SCALER)
    {
        LOG(0, "Invalid input source!!\n");
        return;
    }	

    u4BaseAddr   >>= 4;
    u4YFrameSize >>= 4;
    u4CFrameSize = u4YFrameSize>>1;
    u4FrameSize  = u4YFrameSize + u4CFrameSize;

    // update frame addresses
    for(u1Idx=0; u1Idx<3; u1Idx++)
    {
        vScpipH264SetDramAddr(_tScalerVencPrm.u1Src, u1Idx, u4BaseAddr+(u4FrameSize*u1Idx), u4BaseAddr+(u4FrameSize*u1Idx)+u4YFrameSize);
        _tScalerVencPrm.tAddr[u1Idx].u4YAddr = u4BaseAddr+(u4FrameSize*u1Idx);
        _tScalerVencPrm.tAddr[u1Idx].u4UVAddr = u4BaseAddr+(u4FrameSize*u1Idx)+u4YFrameSize; 
    }

    // Set dram proteciton region.
    if ((u4BaseAddr>>7) < u4ScpipGetDramLimit(_tScalerVencPrm.u1Src, 0))
    {
        LOG(3,"update Da low limit=(%x)  %x\n",(u4BaseAddr>>7),u4BaseAddr);
        vScpipSetLimitAddr(_tScalerVencPrm.u1Src, 0, (u4BaseAddr>>7)); // update low limit
    }

    if (((u4BaseAddr+3*u4FrameSize)>>7) > u4ScpipGetDramLimit(_tScalerVencPrm.u1Src, 1))
    {
	    LOG(3,"update Da high limit=(%x)  %x\n",((u4BaseAddr+3*u4FrameSize)>>7),(u4BaseAddr+3*u4FrameSize));
        vScpipSetLimitAddr(_tScalerVencPrm.u1Src, ((u4BaseAddr+3*u4FrameSize)>>7) + 10, 0xFFFFFFFF); // update high limit
    }
}


UINT8 u1DrvScpipH264GetDramAddress(UINT8 u1Idx,UINT32* u4BufAdr_y, UINT32* u4BufAdr_c) //ENC_BUF_ADR_REG_T* tBufAdr )
{
	
    if (_tScalerVencPrm.u1Src > E_VENC_MODULE_MON_SCALER)
    {
        LOG(0, "Invalid input source!!\n");
        return FALSE;
    }	
    vScpipH264GetDramAddr(_tScalerVencPrm.u1Src, u1Idx, u4BufAdr_y, u4BufAdr_c);
	LOG(3, "venc_buff(y_:0x%x,c_:0x%x)\n",*u4BufAdr_y, *u4BufAdr_c);
    return TRUE;
}


UINT8 u1DrvScpipH264GetInput(void)
{
    return _tScalerVencPrm.u1Input;
}

UINT8 u1DrvScpipH264GetSource(void)
{
    return _tScalerVencPrm.u1Src;
}

UINT32 u4DrvScpipH264GetWidth(void)
{
    return _tScalerVencPrm.wDumpW;
}

UINT32 u4DrvScpipH264GetHeight(void)
{
    return _tScalerVencPrm.wDumpH;
}

UINT32 u4DrvScpipH264IsInterlace(void)
{
    return _tScalerVencPrm.u1Interlace;
}

UINT8 u1DrvScpipH264GetCurrentFrameIdx(void)
{
    if (_tScalerVencPrm.u1Src > E_VENC_MODULE_MON_SCALER)
    {
        LOG(1, "Invalid input source!!\n");
        return 1;
    }  

    return u1ScpipH264GetFrameIdx(_tScalerVencPrm.u1Src);
}

UINT8 u1DrvScpipH264GetCurrentReadyBit(UINT8 u1Idx)
{
    if (_tScalerVencPrm.u1Src > E_VENC_MODULE_MON_SCALER)
    {
        LOG(1, "Invalid input source!!\n");
        return 1;
    }

    return u1ScpipH264GetReadyIdx(_tScalerVencPrm.u1Src, u1Idx);
}

void vDrvScpipH264SetMux(UINT8 u1Mux)
{
    bEncInputDbg = TRUE;
    eEncInputDbg = u1Mux;
}

void vDrvScpipH264SetFldMode(UINT8 u1FldMode)
{
    bEncFldModeDbg = TRUE;
    eEncFldTypeDbg = u1FldMode;    
}

/**
 * @brief query VENC setting for cli cmd
 * @param bPath
 * @return true
 */
void vDrvScpipH264Query(void)
{ 
    LOG(0,"Leak Interrupt %d VencModule %d VencFldMode %d(0:Both 1:T 2:B 3:P)\n",u4ISRLeakIntrpCnt,u1DrvScpipGetVencModule(),eScalerVencFldMode);
    LOG(0,"SignalFrom(%d)EncModule(%d)=(%dx%d)%c\n",_tScalerVencPrm.u1Input,_tScalerVencPrm.u1Src,_tScalerVencPrm.wDumpW,_tScalerVencPrm.wDumpH,_tScalerVencPrm.u1Interlace?'I':'P');
    LOG(0,"WMode(%d) Wh264EN(%d)\n",_tScalerVencPrm.u1Mode,_tScalerVencPrm.u1H264En);
    LOG(0,"Frame drop: en(%d),M(%d),N(%d)\n",_tScalerVencPrm.u1DropEn,_tScalerVencPrm.u1DropM,_tScalerVencPrm.u1DropN);
    LOG(0,"FrameIdx:(%d), ReadyBitSta:(%d,%d,%d)\n", u1DrvScpipH264GetCurrentFrameIdx(), u1DrvScpipH264GetCurrentReadyBit(0), u1DrvScpipH264GetCurrentReadyBit(1), u1DrvScpipH264GetCurrentReadyBit(2));
    LOG(0,"FrameNumber = 0,u4YAddr(0x%x),u4UVAddr(0x%x)\n",_tScalerVencPrm.tAddr[0].u4YAddr,_tScalerVencPrm.tAddr[0].u4UVAddr);
    LOG(0,"FrameNumber = 1,u4YAddr(0x%x),u4UVAddr(0x%x)\n",_tScalerVencPrm.tAddr[1].u4YAddr,_tScalerVencPrm.tAddr[1].u4UVAddr);
    LOG(0,"FrameNumber = 2,u4YAddr(0x%x),u4UVAddr(0x%x)\n",_tScalerVencPrm.tAddr[2].u4YAddr,_tScalerVencPrm.tAddr[2].u4UVAddr);
	LOG(0,"=================VDP VENC CTRL CONFIG=================\n");
    LOG(0,"FldType  %d EncInput  %d EncModule  %d ProfileDef  %d\n",tVencInfo.eEncFldType,tVencInfo.eEncInput,tVencInfo.eEncModule,tVencInfo.eProfileDef);
    LOG(0,"fgInterlace %d EncInWidth  %d  EncInHeight %d Fps  %d\n",tVencRes.fgInterlace,tVencRes.u4EncInWidth,tVencRes.u4EncInHeight,tVencRes.u4Fps);
    LOG(0,"SubEncInput %d TrgtWidth   %d  TrgtHeight  %d\n",tVencInfo.eSubEncInput,tVencInfo.u4TrgtWidth,tVencInfo.u4TrgtHeight);
    LOG(0,"=================VDP VENC CTRL CONFIG=================\n");
}
#endif


