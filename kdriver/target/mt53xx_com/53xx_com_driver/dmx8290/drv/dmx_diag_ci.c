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
 * $RCSfile: $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_diag_ci.c
 *  Demux driver - TS_output for CI SLT program
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "fvr_if.h"
#include "drv_common.h"


//#if defined(__MODEL_slt__) || defined(CC_DMX_ENABLE_TSOUT)

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_rand.h"
#include "x_hal_926.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
#include "fbm_drvif.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
// This number of length will not be compared because some CI cards may drop
// some packets at the end of data transmission.
#define DMX_IGNORE_TRAILING_PACKETS         100
#define DMX_IGNORE_TRAILING_LEN             (188 * DMX_IGNORE_TRAILING_PACKETS)


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define  DMX_CI_TS_COUNT  (4)    //demux support 4-way for multi-stream
#define  DMX_REG_HALF_OFFSET   (16) //the 16 low
#define  DMX_CIPLUS14_REGION_IDX  (4)
#define  DMX_CI_TICK_TIME            (500) //500ms/isr
#define  DMX_REMUX_TSOUT_IDX         (2) //default is 3.
#define  DMX_CI_PVR_THRESHOLD        (0x10000000)
#define  DMX_MULTI_TS_IDX            (3)
 
//monitor thread
#define DMX_CI_MON_THREAD_NAME             "DMXCIMONTH"
#define DMX_CI_MON_THREAD_STACK_SIZE       1024
#define DMX_CI_MON_THREAD_PRIORITY         100
#define DMX_CI_MON_TIME_PERIOD             100         //  Monitor period
#define DMX_CI_MON_RCV_THRESHOLD           30
#define DMX_CI_MON_DROP_THRESHOLD          100
//ciplus record thread
#define DMX_CI_QUEUE_NAME                  "DMXCI14"
#define DMX_CI_QUEUE_SIZE                  1024
#define DMX_CIPLUS14_THREAD_NAME           "DMXCIPLUS14THREAD"
#define DMX_CIPLUS14_THREAD_STACK_SIZE     2048
#define DMX_CIPLUS14_THREAD_PRIORITY       60


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
//this data structure for control ciplus1.4 status.
typedef struct
{
  UINT8                u1MultiStreamTsidx;  
  DMX_CI_STREAM_MODE_T e_StreamMode; //stream mode
  DMX_CI_LOCAL_TS_STRUCT_T       arLocalTsStruct[DMX_CI_TS_COUNT];
}DMX_CI_MULTI_STREAM_STRUCT_T;

static DMX_CI_MULTI_STREAM_STRUCT_T _arMultiStreamStruct;

//static message handle for record address.
static HANDLE_T _hDmxCiQueue = NULL_HANDLE;
//ciplus1.4 record thread
static HANDLE_T _hDmxCiThread = NULL_HANDLE;
//ciplus1.4 monitor thread to set ts-out3 path
static HANDLE_T _hDmxCiMonThread = NULL_HANDLE;

//for fvr record
typedef struct
{
    FVR_GBL_NOTIFY_CODE_T e_type;
    UINT8 u1TsIdx;
    UINT32 u4Addr;
    UINT32 u4Size;
} FVR_MSG_T;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** _DMX_CI_Set_StreamMode
 *  descritpion:use this function to set demux enable multi-stream function.
 *  @param  e_StreamMode       multi-stream or single-mode
 *
 */
//-----------------------------------------------------------------------------

VOID _DMX_CI_Set_StreamMode(DMX_CI_STREAM_MODE_T e_StreamMode)
{

  UINT32 u4_reg;

  u4_reg =DMXCMD_READ32(DMX_REG_DBM_MULTI_STREAM_MODE);

  if(e_StreamMode == MULTI_STREAM_MODE)
  {
    u4_reg |= (0x1<<5);  //enable playback local ts_idx check
	u4_reg |= (0x1<<4);  //enable record local ts_idx check
  }
  else
  {
    u4_reg &= ~(0x1<<5); //disable playback local ts_idx check
    u4_reg &= ~(0x1<<5); //disable record local ts_idx check
    e_StreamMode = SINGLE_STREAM_MODE;
  }
    DMXCMD_WRITE32(DMX_REG_DBM_MULTI_STREAM_MODE,u4_reg);
  _arMultiStreamStruct.e_StreamMode = e_StreamMode;

  return ;
}

//-----------------------------------------------------------------
/** _DMX_CI_Set_MultiStrem_TsIdx
 *  descritpion:use this function to set which Ts index is used for multi-stream input
                     source
 *  @param  u1Tsidx        TS index[0-3]
 *  @retval 
 */
//------------------------------------------------------------------
VOID _DMX_CI_Set_MultiStream_TsIdx(UINT8 u1Tsidx)
{
  UINT32 u4_reg;
  if(u1Tsidx >= DMX_CI_TS_COUNT)
  {
    LOG(0,"Set MultiStream Ts(%d) error.\r\n",u1Tsidx);
	return ;
  }

  u4_reg =DMXCMD_READ32(DMX_REG_DBM_MULTI_STREAM_MODE)&(~0xF);

  u4_reg |=(0x1<<u1Tsidx);	//set u1Tsidx as multi-stream input.
	
  DMXCMD_WRITE32(DMX_REG_DBM_MULTI_STREAM_MODE,u4_reg);

  //in multi-stream sync byte!=0x47,so sync byte=0x47 check cancel
 _DMX_SetFramerPacketErrorHandling(u1Tsidx,FALSE,0);

 _arMultiStreamStruct.u1MultiStreamTsidx = u1Tsidx;

 //set ts in data to serial
 _DMX_TsIn_SetDataWidth(u1Tsidx,DMX_TSOUT_DATAWIDTH_1);

}

//-----------------------------------------------------------------
/** _DMX_CI_Set_MultiStream_LocalTs
 *  descritpion:use this function to set which Ts index is used for local ts
                     source,this function must work when set the multi-stream ts_idx.
 *  @param  prLocalTs<local ts ts_idx,local ts identify, local ts valid>
 *  @retval 
 */
//------------------------------------------------------------------

VOID _DMX_CI_Set_MultiStream_LocalTs(DMX_CI_LOCAL_TS_STRUCT_T* prLocalTs)
{
  UINT32 u4_reg;
  UINT32 u4_local_reg_offset = DMX_REG_TS0_LOCAL_TS_ID_VALID;
  UINT8 u4_MultiTsidx = _arMultiStreamStruct.u1MultiStreamTsidx;
  UINT8 u1LocalTsid=0x47,u1LocalTsidx=0;
  BOOL  bValid= FALSE;
  if(prLocalTs == NULL)
  {
    LOG(0,"Local Ts NULL,return.\r\n");
	return ;
  }
  
  if(prLocalTs->uiLocalTsidx >= DMX_CI_TS_COUNT)
  {
	 LOG(0,"Set Local Ts(%d) error.\r\n",prLocalTs->uiLocalTsidx);
	 return ;
  }

  //[local ts id,local ts_idx ,valid bit]
  u1LocalTsid = prLocalTs->u1LocalId&0xFF;
  u1LocalTsidx = prLocalTs->uiLocalTsidx;
  bValid = prLocalTs->bValid;
  
  u4_reg = DMXCMD_READ32(DMX_REG_DBM_MULTI_STREAM_MODE);
  //set multi-stream ts_idx  local_id valid
  if(bValid)
  {
   u4_reg |= (0x1<<(DMX_REG_HALF_OFFSET+u4_MultiTsidx*4+u1LocalTsidx));
  }
  else
  {
   u4_reg &= ~(0x1<<(DMX_REG_HALF_OFFSET+u4_MultiTsidx*4+u1LocalTsidx));
  }

  DMXCMD_WRITE32(DMX_REG_DBM_MULTI_STREAM_MODE,u4_reg);

  //set multi-stream local ts sync byte.
  u4_local_reg_offset = u4_local_reg_offset+u4_MultiTsidx;
  u4_reg = DMXCMD_READ32(u4_local_reg_offset)&~(0xFF<<u1LocalTsidx*8);
  u4_reg |= (u1LocalTsid<<u1LocalTsidx*8);
  DMXCMD_WRITE32(u4_local_reg_offset,u4_reg);

  //update  information to _arMultiStreamStruct.
  _arMultiStreamStruct.arLocalTsStruct[u1LocalTsidx].u1LocalId = u1LocalTsid;
  _arMultiStreamStruct.arLocalTsStruct[u1LocalTsidx].uiLocalTsidx = u1LocalTsidx;
  _arMultiStreamStruct.arLocalTsStruct[u1LocalTsidx].bValid = bValid;

  return ;
}

//-----------------------------------------------------------------
/** _DMX_CI_GBLCallback
 *  descritpion:remux record is started, then timer notify send information to 
 *                   _DMX_CI_GBLCallback function.
 *  @retval 
 */
//------------------------------------------------------------------
VOID _DMX_CI_GetRecordBuffer(UINT32* pu4Addr, UINT32* pu4Size)
{
#ifndef CC_DMX_EMULATION
    FBM_POOL_T* prFbmPool;
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_REMUX);
    
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != 0);
    
    *pu4Addr = prFbmPool->u4Addr;
    *pu4Size = prFbmPool->u4Size;
    LOG(5, "CI Record buffer: Addr==0x%x, size=0x%x\n", prFbmPool->u4Addr, prFbmPool->u4Size);
#else
    *pu4Addr = 0x2600000;
    *pu4Size = 0x800000;
#endif
    return ;
}

//-----------------------------------------------------------------
/** _DMX_CI_Set_PvrBuffer
 *  descritpion:set ts out pvr buffer for this item
 *  @retval 
 */
//------------------------------------------------------------------
BOOL _DMX_CI_Set_PvrBuffer(UINT8 u1TsoutIdx)
{
    DMX_PVR_PLAY_T rPlay;
	UINT32 u4Addr=0,u4Size=0;

	_DMX_CI_GetRecordBuffer(&u4Addr,&u4Size);
   
    // Play init
    rPlay.fgAllocBuf = FALSE;
	rPlay.u4BufStart= u4Addr;
    rPlay.u4BufSize = u4Size;
    rPlay.u4ThresholdSize = DMX_CI_PVR_THRESHOLD;     // Not usable
	rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = FALSE;
	rPlay.eMode = DMX_PVR_PLAY_STREAM;
    rPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
    rPlay.pfnPVRNotify = NULL;

    if (!DMX_PVRPlay_SetEx(u1TsoutIdx, (UINT32)(PVRPLAY_FLAGS_THRESHOLD |PVRPLAY_FLAGS_BUFFER |
                                   PVRPLAY_FLAGS_CALLBACK|PVRPLAY_FLAGS_MODE|PVRPLAY_FLAGS_TIMESTAMP), &rPlay))
    {
        return FALSE;
    }

    DMX_PVRPlay_FlushBufEx(u1TsoutIdx);

    return TRUE ;
}

//-----------------------------------------------------------------
/** _DMX_CI_RecOverflow
 *  descritpion:remux record over flow ,stop record .
 *  
 *  @retval 
 */
//------------------------------------------------------------------
VOID _DMX_CI_RecOverflow(UINT8 u1TsIdx)
{
    if (!FVR_Stop(u1TsIdx))
    {
        LOG(0,"%s, FVR stop error\n", __FUNCTION__);
    }

    FVR_SetIsOverflow(u1TsIdx, TRUE);  

	return ;
}

//-----------------------------------------------------------------
/** _DMX_CI_RecRestart
 *  descritpion:when over flow ,receive message ,handle  it again.\r\n .
 *  
 *  @retval 
 */
//------------------------------------------------------------------
VOID _DMX_CI_RecRestart(UINT8 u1TsIdx)
{
    //flus mini-pvr buffer
	_DMX_PVRPlay_FlushBuf(DMX_REMUX_TSOUT_IDX);

	//flush record region buffer
	if(!FVR_FlushBuffer(u1TsIdx))
	{
	  LOG(0,"%s, FVR Flush error(%d)\n", __FUNCTION__,u1TsIdx); 
	}

	//start record again
    if (!FVR_Start(u1TsIdx))
    {
        LOG(0,"%s, FVR Start error(%d)\n", __FUNCTION__,u1TsIdx);
    }

	return ;
}


//-----------------------------------------------------------------
/** _DMX_CI_GBLCallback
 *  descritpion:remux record is started, then timer notify send information to 
 *                   _DMX_CI_GBLCallback function.
 *  @retval 
 */
//------------------------------------------------------------------
VOID _DMX_CI_GBLCallback(FVR_GBL_NOTIFY_CODE_T eCode,
                                       UINT32 u4Data, const void* pvTag)
{
    FVR_GBL_TIMENOTIFY_INFO_T *prInfo;
    FVR_MSG_T rPes;
	
    switch(eCode)
	{
	  case FVR_GBL_NFY_TIME:
	  	   prInfo = (FVR_GBL_TIMENOTIFY_INFO_T*)u4Data;
		   LOG(1,"Tsidx=%d,recorded addr: 0x%x size: 0x%x\n",prInfo->u1TsIndex, prInfo->u4Addr, prInfo->u4Size);
		  if( prInfo->u4Size > 0)
		  {
		    rPes.e_type = FVR_GBL_NFY_TIME;
			rPes.u4Addr = prInfo->u4Addr;
			rPes.u4Size = prInfo->u4Size;
			rPes.u1TsIdx = prInfo->u1TsIndex;
			if (x_msg_q_send(_hDmxCiQueue, (VOID*)&rPes, sizeof (FVR_MSG_T),128) != OSR_OK)
			{
			   LOG(3, "Demux queue full!\n");
			}
		   }
		   break;
	  case FVR_GBL_NFY_PKTCOUNT:
		   // Don't use
		   ASSERT(0);
		   break;
	  case FVR_GBL_NFY_OVERFLOW:
		    LOG(1, "CI+1.4 Buffer OverFlow....\r\n");
			_DMX_CI_RecOverflow(DMX_CIPLUS14_REGION_IDX);
		    rPes.e_type = FVR_GBL_NFY_OVERFLOW;
			rPes.u1TsIdx = (UINT8)u4Data;
			if (x_msg_q_send(_hDmxCiQueue, (VOID*)&rPes, sizeof (FVR_MSG_T),
			   128) != OSR_OK)
			{
			   LOG(3, "Demux queue full!\n");
			}
		   break;
	  default:
		   // Unexpected
		   ASSERT(0);
		   break;
	 }
}

//-----------------------------------------------------------------
/** _DMX_CI_Record_Start
 *  descritpion:use this function to remux local ts .remux ci+1.4 buffer ts_idx(4)
 *                   use timer isr to trigger datas.
 *  @retval 
 */
//------------------------------------------------------------------

VOID _DMX_CI_Record_Start(VOID)
{

  
 FVR_GBL_T rGblPid;
 FVR_TIMESTAMP_T rTimeStamp;
 UINT8 u1MultiTsidx=0;
 UINT32 u4Pattern=0;
 UINT8  u1SyncMask=0;
 UINT8  i =0;
 UINT32 u4Addr=0,u4Size=0;
 if(_arMultiStreamStruct.e_StreamMode != MULTI_STREAM_MODE)
 {
   LOG(0,"Not in multi stream mode,don't do remux.\r\n");
   return ;
 }
 //init global regioin buffer for ci+1.4
 x_memset((VOID*)&rGblPid, 0x0, sizeof(FVR_GBL_T));
 
 //multi-stream ts_idx.
 u1MultiTsidx = _arMultiStreamStruct.u1MultiStreamTsidx;
 //set remux sync pattern, remux replace sync byte  mask.
 for(i=0;i<DMX_CI_TS_COUNT;i++)
 {
   u4Pattern |= (_arMultiStreamStruct.arLocalTsStruct[i].u1LocalId&0xFF)<<(i*8);
   u1SyncMask |= ((_arMultiStreamStruct.arLocalTsStruct[i].bValid ? 0x1:0x0)<< i);
 }
 //get record buffer size
 _DMX_CI_GetRecordBuffer(&u4Addr,&u4Size);
 
 LOG(0,"%s,MultiTs=%d, Sync Pattern=0x%x,Sync mask=0x%x,Addr=0x%x,Size=0x%x.\r\n",__FUNCTION__,u1MultiTsidx, u4Pattern,u1SyncMask,
 	 u4Addr,u4Size);

 //set record buffer information. 
 rGblPid.fgAllocateBuffer = FALSE;
 rGblPid.u2TargetPktCount = 0x0; //disable pkt isr.
 rGblPid.u4BufStart = u4Addr;
 rGblPid.u4BufSize = u4Size;
 rGblPid.u4SyncPattern=u4Pattern;
 rGblPid.u1SyncMask = u1SyncMask;
 rGblPid.u1CiTsidx =u1MultiTsidx; /*ci tsindex=0x3*/
 rGblPid.u4TimeToNotify = DMX_CI_TICK_TIME;
 rGblPid.pfnNotifyFunc = _DMX_CI_GBLCallback;
 
 if (FVR_SetGBL((UINT8)DMX_CIPLUS14_REGION_IDX, FVR_GBL_FLAG_ALL, &rGblPid)==FALSE)
 {
	 return ;
 }
 
 // Enabe Record timestamp
 rTimeStamp.fgEnable = TRUE;
 rTimeStamp.u2ClkDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
 rTimeStamp.u4Mask = 0xFFFFFFFF;
 rTimeStamp.u4Counter= 0x0;
 FVR_SetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimeStamp);

 //start record
 FVR_Start(DMX_CIPLUS14_REGION_IDX);
 
  return ;
}


//-----------------------------------------------------------------
/** _DMX_CI_Record_Stop
 *  descritpion:use this function to stop remuxfunction.
 *  @retval 
 */
//------------------------------------------------------------------

VOID _DMX_CI_Record_Stop(VOID)
{
  FVR_Stop(DMX_CIPLUS14_REGION_IDX);
  FVR_FreeGBL(DMX_CIPLUS14_REGION_IDX);
  return ;
}


//-----------------------------------------------------------------
/** _Dmx_CI_MonThread
 *  descritpion:use this function to change the path
 *  @retval 
 */
//------------------------------------------------------------------

VOID _Dmx_CI_MonThread(VOID* pvArg)
{

  UNUSED(pvArg);

  return ;

  #if 0
  static BOOL fg_disable_monitor=FALSE;
  while(1)
  {
    if((fg_disable_monitor==FALSE)&&(_arMultiStreamStruct.e_StreamMode==MULTI_STREAM_MODE))
    {
      //set ts-out2->external
      _DMX_CI_SetTsoutPath(DMX_REMUX_TSOUT_IDX);
      //set mini-pvr2 buffer
      if(_DMX_CI_Set_PvrBuffer(DMX_REMUX_TSOUT_IDX)== TRUE)
      {//start mini-pvr2
         if (DMX_PVRPlay_StartEx(DMX_REMUX_TSOUT_IDX)== TRUE)
        {		      
          fg_disable_monitor = TRUE; 
        }
      }

    }
	else if((fg_disable_monitor==TRUE)&&(_arMultiStreamStruct.e_StreamMode==SINGLE_STREAM_MODE))
	{
	  //stop mini-pvr2
	  if(DMX_PVRPlay_StopEx(DMX_REMUX_TSOUT_IDX,TRUE))
      {      
	    //free mini-pvr2 buffer
        if (DMX_PVRPlay_FreeBufEx(DMX_REMUX_TSOUT_IDX))
        {
      	  fg_disable_monitor = FALSE; 
        }
	  }
	}
    
  }
  #endif
}

//-----------------------------------------------------------------
/** _Dmx_CI_MonThread
 *  descritpion:handle recrod message
 *  @retval 
 */
//------------------------------------------------------------------
VOID _Dmx_CI_Record_thread(VOID* pvArg)
{
   UNUSED(pvArg);
  while (1)
  {
     static FVR_MSG_T rMsg={0};
     INT32 i4Ret;
     SIZE_T zMsgSize;
     UINT16 u2QueueIndex;
     UINT32 u4Wp;
	 UINT32 u4Rp;
     zMsgSize = sizeof (rMsg);

     i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, &zMsgSize,
            &_hDmxCiQueue, 1, X_MSGQ_OPTION_WAIT);

	 if (i4Ret != OSR_OK)
     {
        LOG(1, "Error on receive record  message!\n");
        continue;
     }

     switch(rMsg.e_type)	
     {
       case FVR_GBL_NFY_TIME:
	   	{
           LOG(1,"%s,%d:rMsg.u1TsIdx=%d, rMsg.u4Addr=0x%x, rMsg.u4Size=0x%x!\n", __FUNCTION__, __LINE__, rMsg.u1TsIdx, rMsg.u4Addr, rMsg.u4Size);
		   //Update mini-pvr2 buffer for ts_out
           if(rMsg.u1TsIdx == DMX_CIPLUS14_REGION_IDX)
          {
            _DMX_PVRPlay_UpdateWp(DMX_REMUX_TSOUT_IDX, rMsg.u4Addr, rMsg.u4Size); //don't use dmx_if api to update it.
          }
          //wait the mini-pvr read is ok
          u4Wp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_WP3);
	      while (1)
	      {
	        u4Rp = DMXCMD_READ32(DMX_REG_PVR_PLAY_BUF_RP3);
            
	        if (u4Rp == u4Wp)
	        {
	            ASSERT((DMXCMD_READ32(DMX_REG_PVR_READ_BUFFER_STATUS) & 0x1) == 0); // 0x17380, pvr read buffer status
	            break;
	        }
	        x_thread_delay(1); //because data is very large ,dely hardware.
	      } 
		  //update record buffer read-pointer
           FVR_UnlockBuffer(rMsg.u1TsIdx, rMsg.u4Addr, rMsg.u4Size);
       	}break;

	   case FVR_GBL_NFY_OVERFLOW:
	   {
		 _DMX_CI_RecRestart(rMsg.u1TsIdx);
	   }
       break;

       default:
           break;
     }
    x_thread_delay(100);
   }  
}

//-----------------------------------------------------------------------------
/** _DMX_TsOut_TransmitData
 */
//-----------------------------------------------------------------------------
BOOL _DMX_CI_TsOut_TransmitDataForCITest(UINT8 u1TsIndex, UINT8 u1TsoutIdx, UINT32 u4Size)
{
    BOOL fgRet = FALSE;
    UINT32 u4SrcBufSize, u4DstBufSize;
    UINT32 u4SrcAddr = 0, u4SrcSize;
    UINT32 u4DstAddr = 0, u4DstEndAddr;
    UINT32 u4PesAddr = 0, u4PesSize;
    UINT32 u4Count;
    DMX_PVR_PLAY_T rPlay;
    UINT32 u4Flags;
    UINT32 u4Rp, u4Wp, u4Reg;
    UINT32 i;

    if (u4Size <= DMX_IGNORE_TRAILING_LEN)
    {
        Printf("Length %u is too short!\n", u4Size);
        return FALSE;
    }

    // Allocate source buffer.  (Mini-PVR RP/WP 8-byte alignment, 188 * 4 = 752)
    u4SrcSize = _DMX_Align(u4Size, 752);
    u4SrcBufSize = u4SrcSize + DMX_CA_BUFFER_ALIGNMENT;
    u4SrcAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4SrcBufSize, DMX_CA_BUFFER_ALIGNMENT);
    if (u4SrcAddr == 0)
    {
        Printf("Failed to allocate source buffer!\n");
        return FALSE;
    }

    // Allocate destination buffer
    // It's important to make the CA output buffer slightly larger.
    u4DstBufSize = _DMX_Align(u4SrcBufSize + 1000, DMX_CA_BUFFER_ALIGNMENT);
    u4DstAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4DstBufSize, DMX_CA_BUFFER_ALIGNMENT);
    if (u4DstAddr == 0)
    {
        Printf("Failed to allocate destination buffer!\n");
        fgRet = FALSE;
        goto FailAndFreeMemory;
    }

    //clear dst buffer
    for ( i = 0 ; i < u4DstBufSize/4 ; i++ )
    {
        ((UINT32*)u4DstAddr)[i] = 0;
    }
    u4DstEndAddr = u4DstAddr + u4DstBufSize;

    // Set PVR playback
    rPlay.fgAllocBuf = FALSE;
    rPlay.u4BufStart = u4SrcAddr;
    rPlay.u4BufSize = u4SrcBufSize;
    rPlay.u4ThresholdSize = u4SrcSize;  // FIXME
    rPlay.pfnPVRNotify = NULL;
    u4Flags = PVRPLAY_FLAGS_BUFFER | PVRPLAY_FLAGS_THRESHOLD |
              PVRPLAY_FLAGS_CALLBACK;
    if (!_DMX_PVRPlay_Set(u1TsoutIdx, u4Flags, &rPlay))
    {
        Printf("Failed to set PVR properties!\n");
        fgRet = FALSE;
        goto FailAndFreeMemory;
    }

    //Set CA output buffer
    if (!_DMX_SetCaOutputBuffer(TRUE, TRUE, u4DstAddr, u4DstEndAddr, u4SrcSize))
    {
        Printf("Failed to set CA output buffer!\n");
        fgRet = FALSE;
        goto FailAndFreeMemory;
    }

    _GenerateRandomPattern2(u4SrcAddr, u4SrcSize);

    u4PesAddr = (UINT32)x_mem_alloc(u4SrcSize);
    if (u4PesAddr == 0)
    {
        Printf("Failed to allocate payload buffer!\n");
        fgRet = FALSE;
        goto FailAndFreeMemory;
    }

    // The input to 8295 needs to be in TS packet format.
    if (!_GenerateSimpleTsHeader2(u4SrcAddr, u4SrcSize, 0, u4PesAddr,
                                 &u4PesSize))
    {
        Printf("Failed to generate TS packets!\n");
        fgRet = FALSE;
        goto FailAndFreeMemory;
    }

    HalFlushInvalidateDCache();

    Printf("Src address: 0x%08x\n", u4SrcAddr);
    Printf("Dst address: 0x%08x\n", u4DstAddr);
    Printf("Original Src size: %u.  New Src Size: %u\n", u4Size, u4SrcSize);

    // Set Full pull mode
    u4Reg = DMXCMD_READ32(DMX_REG_CONFIG2);
    u4Reg |= 0xC0000000;
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Reg);

    VERIFY(_DMX_PVRPlay_FlushBuf(u1TsoutIdx));
    VERIFY(_DMX_PVRPlay_UpdateWp(u1TsoutIdx, u4SrcAddr, u4SrcSize));
    VERIFY(_DMX_PVRPlay_Start(u1TsoutIdx));

    // Wait for some time.
    // x_thread_delay(3000);
    u4Count = 0;
    while (1)
    {
        u4Wp = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_WP);
        u4Rp = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_RP);
        if ((u4Wp - u4Rp) >= (u4SrcSize - DMX_IGNORE_TRAILING_LEN))  // Do not consider wrap around for now.
        {
            break;
        }
        if (((++u4Count) >= 300))  // two outmost pairs of parenthesis are necessary
        {
            Printf("Exceed loop count %u!\n", u4Count);
            fgRet = FALSE;
            goto FailAndFreeMemory;
        }
        x_thread_delay(1);
    }
    Printf("Finish!\n");
    
FailAndFreeMemory:
    VERIFY(_DMX_PVRPlay_Stop(u1TsoutIdx, TRUE));
    VERIFY(_DMX_PVRPlay_FreeBuf(u1TsoutIdx));
    VERIFY(BSP_FreeAlignedDmaMemory(u4SrcAddr));
    VERIFY(BSP_FreeAlignedDmaMemory(u4DstAddr));
    if (u4PesAddr != 0)
    {
        x_mem_free((void*)u4PesAddr);
    }

    // Set back
    u4Reg = DMXCMD_READ32(DMX_REG_CONFIG2);
    u4Reg &= ~0xC0000000;
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Reg);

    return fgRet;
}

//-----------------------------------------------------------------
/** _DMX_CI_SetTsoutPath
 *  descritpion:set tsout signal output 
 *  @retval 
 */
//------------------------------------------------------------------
BOOL _DMX_CI_SetTsoutPathForCITest(UINT8 u1TsoutIdx, DMX_TSOUT_DATAWIDTH_T eDataWidth)
{

    UINT32 u4Reg=0;
    
   //Set ts output enalbe
    _DMX_PVRPlay_SetPort(u1TsoutIdx,DMX_PVRPLAY_PORT_TSOUT);
 
    //Set output ts clock 108M
    _DMX_TsOut_SetClock(u1TsoutIdx, DMX_PVR_OUTPUT_CLOCK_108M);
   
    u4Reg = (CKGEN_READ32(CKGEN_TSIF_CFG) & (~0xFFFF)) | 0x60A8;
            
    //ci tso/ci tso2 data clock enable (1:diable,0:enable), default is disable
    //bit31:CI TSO2 sync,data,valid output disable
    //bit30:CI TSO   sync,data,valid output disable
    //bit29:CI TSO2 clock output disable
    //bit28:CI TSO   clock output disable
    u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) & ~(0xF<<28);
    CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);
    
    //bypass CI framer
    //0: from the CI framer 
    //1: bypass CI framer, mux reslut direct output. 
    //bit0:CI TSO output model select 
    //bit1:CI TSO2 output model select 
    u4Reg = CKGEN_READ32(CKGEN_TSIF2_CFG) | 0xF;
    CKGEN_WRITE32(CKGEN_TSIF2_CFG, u4Reg);
    
    //PAD_PVR_TSCLK-->CLK
    //PAD_PVR_TSVAL-->VALID
    //PAD_PVR_TSSYNC-->SYNC
    //PAD_PVR_TSDATA0-->DATA0
    //PAD_PVR_TSDATA1-->DATA1
    //tso
    BSP_PinSet(PIN_PVR_TSCLK, 1);
    BSP_PinSet(PIN_PVR_TSDATA1, 1);

   
   //PAD_GPIO15 --> TSCLK
   //PAD_GPIO16 --> TSVAL
   //PAD_GPIO17 --> TSSYNC
   //PAD_GPIO18 --> TSDATA0
   //tso2
   //BSP_PinSet(PIN_GPIO15, 5);
   //BSP_PinSet(PIN_GPIO18, 5);

   //select  cksel for ts_out
    if (u1TsoutIdx == 0)
   {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x2200; // //  dmxout1 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
   }
   else if (u1TsoutIdx == 1)
   {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x3300; // //  dmxout2 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
   }
   else
   {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x7700; // //  dmxout3 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
   }

   //set ts out data to serial
   _DMX_TsOut_SetDataWidth(u1TsoutIdx,eDataWidth);

   return TRUE;
   
}

BOOL _DMX_CI_OutputTwobitsTest(UINT8 u1TsIn, UINT8 u1TsOut, UINT8 u1DataWidth, UINT32 u4Size)
{

    DMX_TSOUT_DATAWIDTH_T eDataWidth = DMX_TSOUT_DATAWIDTH_1;
    BOOL fgInvert = FALSE;
    BOOL fgGating = FALSE;
    DMX_TSOUT_ENDIAN_T eEndian = DMX_TSOUT_ENDIAN_MSB;
    DMX_TSOUT_SYNC_CYCLE_T eSyncCycle = DMX_TSOUT_SYNC_CYCLE_8;
    UINT32 u4ValidSpace = 0;
    DMX_TSOUT_LATCH_EDGE_T eLatchEdge = DMX_TSOUT_LATCH_EDGE_NEGATIVE;
    
    if (u1DataWidth == 1)
    {
        eDataWidth = DMX_TSOUT_DATAWIDTH_1;
    }
    else if (u1DataWidth == 2)
    {
        eDataWidth = DMX_TSOUT_DATAWIDTH_2;
    }
    else if (u1DataWidth == 8)
    {
        eDataWidth = DMX_TSOUT_DATAWIDTH_8;
    }
    else
    {
        LOG(0, "Unknown data width!\n");
        return FALSE;
    }
    
    if (!_DMX_CI_SetTsoutPathForCITest(u1TsOut, eDataWidth))
    {
        Printf("Failed to set route!\n");
        return FALSE;
    }

    if (!DMX_TsOut_SetDataWidthEx(u1TsOut, eDataWidth))
    {
        Printf("Failed to set data width!\n");
        return FALSE;
    }
    
    if (!DMX_TsOut_SetEndianessEx(u1TsOut, eEndian))
    {
       Printf("Failed to set endian mode!\n");
       return FALSE;
    }
    
    if (!DMX_TsOut_SetSyncCycleEx(u1TsOut, eSyncCycle))
    {
        Printf("Failed to set sync cycle!\n");
        return FALSE;
    }
    
    if (!DMX_TsOut_SetValidCycleEx(u1TsOut, u4ValidSpace))
    {
        Printf("Failed to set valid space!\n");
        return FALSE;
    }
    
    if (!DMX_TsOut_SetLatchEdgeEx(u1TsOut, eLatchEdge))
    {
        Printf("Failed to set Latch edge!\n");
        return FALSE;
    }

    if (!_DMX_TsOut_SetClockInvertGating(u1TsOut, fgInvert, fgGating))
    {
        Printf("Failed to set Latch edge!\n");
        return FALSE;
    }

    if (!_DMX_CI_TsOut_TransmitDataForCITest(u1TsIn, u1TsOut, u4Size))
    {
        Printf("Failed\n");
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------
/** _DMX_CI_SetTsoutPath
 *  descritpion:set tsout signal output 
 *  @retval 
 */
//------------------------------------------------------------------
VOID _DMX_CI_SetTsoutPath(UINT8 u1TsoutIdx)
{
    UINT32 u4Reg=0;
   //Set ts output enalbe
    _DMX_PVRPlay_SetPort(u1TsoutIdx,DMX_PVRPLAY_PORT_TSOUT);

   
 
    //_DMX_PVRPlay_SetPort(u1TsoutIdx,DMX_PVRPLAY_PORT_FRAMER2);
    

   //Set output ts clock 108M
   //_DMX_TsOut_SetClock(u1TsoutIdx, DMX_PVR_OUTPUT_CLOCK_108M);
   
   _DMX_TsOut_SetClock(u1TsoutIdx, DMX_PVR_OUTPUT_CLOCK_54M);
   
   //PAD_PVR_TSCLK-->CLK
   //PAD_PVR_TSVAL-->VALID
   //PAD_PVR_TSSYNC-->SYNC
   //PAD_PVR_TSDATA0-->DATA0
   //PAD_PVR_TSDATA1-->DATA1
   //tso
   //BSP_PinSet(PIN_PVR_TSCLK, 1);
   //BSP_PinSet(PIN_PVR_TSDATA1, 1);

   
   //PAD_GPIO15 --> TSCLK
   //PAD_GPIO16 --> TSVAL
   //PAD_GPIO17 --> TSSYNC
   //PAD_GPIO18 --> TSDATA0
   //tso2
   BSP_PinSet(PIN_GPIO15, 5);
   BSP_PinSet(PIN_GPIO18, 5);

   //select  cksel for ts_out
    if (u1TsoutIdx == 0)
   {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x2200; // //  dmxout1 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
   }
   else if (u1TsoutIdx == 1)
   {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x3300; // //  dmxout2 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
   }
   else
   {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x7700; // //  dmxout3 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
   }

   //set ts out data to serial
   _DMX_TsOut_SetDataWidth(u1TsoutIdx,DMX_TSOUT_DATAWIDTH_1);

   return;
   
}

//-----------------------------------------------------------------
/** _DMX_CiPlus14_Init
 *  descritpion:Init data structure and create thread to notify datas.
 *  @retval 
 */
//------------------------------------------------------------------
VOID _DMX_CiPlus14_Init(VOID)
{
  static BOOL _fgInit = FALSE;

  if(!_fgInit)
  {
    //init global data structure
    x_memset((VOID*)&_arMultiStreamStruct,0,sizeof(DMX_CI_MULTI_STREAM_STRUCT_T));
    // default stream mode as :single mode
    _arMultiStreamStruct.e_StreamMode = SINGLE_STREAM_MODE;
	_arMultiStreamStruct.u1MultiStreamTsidx = DMX_MULTI_TS_IDX;

	//monitor thread
   if (x_thread_create(&_hDmxCiMonThread, DMX_CI_MON_THREAD_NAME,
            DMX_CI_MON_THREAD_STACK_SIZE, DMX_CI_MON_THREAD_PRIORITY, _Dmx_CI_MonThread, 0,
            NULL) != OSR_OK)
   {
      LOG(1, "Fail to create ciplus1.4 monitor thread!\n");
      return ;
   }

	 // Create ciplus1.4 record message
     if (x_msg_q_create(&_hDmxCiQueue, DMX_CI_QUEUE_NAME,
			   sizeof (FVR_MSG_T), DMX_CI_QUEUE_SIZE) != OSR_OK)
	 {
	   LOG(1, "Fail to create ciplus1.4  queue!\n");
	   return ;
     }
	
	 // Create ciplus1.4 record thread
	 if (x_thread_create(&_hDmxCiThread,DMX_CIPLUS14_THREAD_NAME,
			   DMX_CIPLUS14_THREAD_STACK_SIZE, DMX_CIPLUS14_THREAD_PRIORITY, _Dmx_CI_Record_thread, 0,
			   NULL) != OSR_OK)
    {
	  LOG(1, "Fail to create ciplus1.4  thread!\n");
	  return ;
    }

	_fgInit = TRUE;
  }
  return ;
}

//-----------------------------------------------------------------------------
/** _DMX_TsOut_SetLatchEdge
 *  negative means clock inversion, the clock rising edge is in the middle of data signal
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TsOut_SetLatchEdge(UINT8 u1TsOutIdx, DMX_TSOUT_LATCH_EDGE_T eEdge)
{
    BOOL fgRet = FALSE;
    UINT32 u4Reg;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (u1TsOutIdx == 0)
    {
        switch (eEdge)
        {
        case DMX_TSOUT_LATCH_EDGE_POSITIVE:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
            u4Reg &= ~((UINT32)1 << 22);  // clear bit 22
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        case DMX_TSOUT_LATCH_EDGE_NEGATIVE:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
            u4Reg |= (UINT32)1 << 22;  // set bit 22
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output latch edge!\n");
            fgRet = FALSE;
            break;
        }
    }
    else if (u1TsOutIdx == 1)
    {
        switch (eEdge)
        {
        case DMX_TSOUT_LATCH_EDGE_POSITIVE:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL);
            u4Reg &= ~((UINT32)1 << 22);  // clear bit 22
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        case DMX_TSOUT_LATCH_EDGE_NEGATIVE:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL);
            u4Reg |= (UINT32)1 << 22;  // set bit 22
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output latch edge!\n");
            fgRet = FALSE;
            break;
        }
    }
    else if(u1TsOutIdx == 2)
    {
        switch (eEdge)
        {
        case DMX_TSOUT_LATCH_EDGE_POSITIVE:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL);
            u4Reg &= ~((UINT32)1 << 22);  // clear bit 22
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        case DMX_TSOUT_LATCH_EDGE_NEGATIVE:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL);
            u4Reg |= (UINT32)1 << 22;  // set bit 22
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output latch edge!\n");
            fgRet = FALSE;
            break;
        }
    }
    else
    {
        ASSERT(0);
    }

    return fgRet;
}

//-----------------------------------------------------------------------------
/** _DMX_TsOut_SetValidCycle
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TsOut_SetValidCycle(UINT8 u1TsOutIdx, UINT32 u4Cycle)
{
    UINT32 u4Reg;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (u4Cycle >= 4)
    {
        Printf("Incorrect TS-output Valid cycle!\n");
        return FALSE;
    }
    u4Cycle = u4Cycle&0x3;
    if (u1TsOutIdx == 0)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
        u4Reg &= ~((UINT32)3 << 20);  // clear bit [21:20]
        u4Reg |= u4Cycle << 20;
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
    }
    else if (u1TsOutIdx == 1)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL);
        u4Reg &= ~((UINT32)3 << 20);  // clear bit [21:20]
        u4Reg |= u4Cycle << 20;
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4Reg);
    }
    else if(u1TsOutIdx == 2)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL);
        u4Reg &= ~((UINT32)3 << 20);  // clear bit [21:20]
        u4Reg |= u4Cycle << 20;
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4Reg);
    }
    else
    {
        ASSERT(0);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_TsOut_SetSyncCycle
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TsOut_SetSyncCycle(UINT8 u1TsOutIdx, DMX_TSOUT_SYNC_CYCLE_T eSyncCycle)
{
    BOOL fgRet = FALSE;
    UINT32 u4Reg;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (u1TsOutIdx == 0)
    {
        switch (eSyncCycle)
        {
        case DMX_TSOUT_SYNC_CYCLE_8:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
            u4Reg &= ~((UINT32)1 << 19);  // clear bit 19  serial
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        case DMX_TSOUT_SYNC_CYCLE_1:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
            u4Reg |= (UINT32)1 << 19;  // set bit 19 parallel
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output Sync cycle!\n");
            fgRet = FALSE;
            break;
        }
    }
    else if (u1TsOutIdx == 1)
    {
        switch (eSyncCycle)
        {
        case DMX_TSOUT_SYNC_CYCLE_8:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL);
            u4Reg &= ~((UINT32)1 << 19);  // clear bit 19
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        case DMX_TSOUT_SYNC_CYCLE_1:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL);
            u4Reg |= (UINT32)1 << 19;  // set bit 19
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output Sync cycle!\n");
            fgRet = FALSE;
            break;
        }
    }
    else if(u1TsOutIdx == 2)
    {
      switch (eSyncCycle)
        {
        case DMX_TSOUT_SYNC_CYCLE_8:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL);
            u4Reg &= ~((UINT32)1 << 19);  // clear bit 19
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        case DMX_TSOUT_SYNC_CYCLE_1:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL);
            u4Reg |= (UINT32)1 << 19;  // set bit 19
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output Sync cycle!\n");
            fgRet = FALSE;
            break;
        }
    }
    else
    {
        ASSERT(0);
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_TsOut_SetDataWidth
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TsOut_SetDataWidth(UINT8 u1TsOutIdx, DMX_TSOUT_DATAWIDTH_T eDataWidth)
{
    BOOL fgRet = FALSE;
    UINT32 u4Reg;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);
    if (u1TsOutIdx == 0)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
        u4Reg &= ~0x02800000;

        switch (eDataWidth)
        {
        case DMX_TSOUT_DATAWIDTH_1:
            fgRet = TRUE;
            break;

        case DMX_TSOUT_DATAWIDTH_2:
            u4Reg |= 0x02800000;
            fgRet = TRUE;
            break;

        case DMX_TSOUT_DATAWIDTH_8:
            u4Reg |= 0x00800000;
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output endian mode!\n");
            fgRet = FALSE;
            break;
        }

        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
    }
    else if (u1TsOutIdx == 1)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL);
        u4Reg &= ~0x02800000;

        switch (eDataWidth)
        {
        case DMX_TSOUT_DATAWIDTH_1:
            fgRet = TRUE;
            break;

        case DMX_TSOUT_DATAWIDTH_2:
            u4Reg |= 0x02800000;
            fgRet = TRUE;
            break;

        case DMX_TSOUT_DATAWIDTH_8:
            u4Reg |= 0x00800000;
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output endian mode!\n");
            fgRet = FALSE;
            break;
        }

        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4Reg);
    }
	 else if(u1TsOutIdx == 2)
    {
       u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL);
        u4Reg &= ~0x02800000;

        switch (eDataWidth)
        {
        case DMX_TSOUT_DATAWIDTH_1:
            fgRet = TRUE;
            break;

        case DMX_TSOUT_DATAWIDTH_2:
            u4Reg |= 0x02800000;
            fgRet = TRUE;
            break;

        case DMX_TSOUT_DATAWIDTH_8:
            u4Reg |= 0x00800000;
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output endian mode!\n");
            fgRet = FALSE;
            break;
        }

        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4Reg);
    }
	else
    {
        ASSERT(0);
    }

    return fgRet;
}

//-----------------------------------------------------------------------------
/** _DMX_TsOut_SetClockInvertGating
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TsOut_SetClockInvertGating(UINT8 u1TsOutIdx, BOOL fgInvert, BOOL fgGating)
{
    UINT32 u4Reg;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (u1TsOutIdx == 0)
    {
        if (fgInvert)
        {
            u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG);
            u4Reg |= (0x1 << 16);
            CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

#ifdef CC_DMX_EMULATION //change framer sample edge default framer 0

            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);    // Framer
            if (((u4Reg >> 3) & 0x1) == 0x1)//positive
            {
                u4Reg &= ~(0x1 << 3);
                DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);
            }
            else //negative
            {
                u4Reg |= (0x1 << 3);
                DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);
            }
#endif
        }
        else
        {
            u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG);
            u4Reg &= ~(0x1 << 16);
            CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);
        }

        if (fgGating)
        {
            u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG);
            u4Reg |= (0x1 << 17);
            CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);
        }
        else
        {
            u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG);
            u4Reg &= ~(0x1 << 17);
            CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);
        }
    }
    else if (u1TsOutIdx == 1)
    {
        if (fgInvert)
        {
            u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG);
            u4Reg |= (0x1 << 18);
            CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

#ifdef CC_DMX_EMULATION //change framer sample edge default framer 0

            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);    // Framer
            if (((u4Reg >> 3) & 0x1) == 0x1)//positive
            {
                u4Reg &= ~(0x1 << 3);
                DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);
            }
            else //negative
            {
                u4Reg |= (0x1 << 3);
                DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);
            }
#endif
        }
        else
        {
            u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG);
            u4Reg &= ~(0x1 << 18);
            CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);
        }

        if (fgGating)
        {
            u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG);
            u4Reg |= (0x1 << 19);
            CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);
        }
        else
        {
            u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG);
            u4Reg &= ~(0x1 << 19);
            CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);
        }
    }
    else if(u1TsOutIdx == 2)
    {
       LOG(5,"%s,don't set if for tsout2.\r\n",__FUNCTION__);
    }
    else
    {
        ASSERT(0);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_TsIn_SetDataWidth
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TsIn_SetDataWidth(UINT8 u1TsIndex, DMX_TSOUT_DATAWIDTH_T eDataWidth)
{
    BOOL fgRet = FALSE;

    ASSERT(u1TsIndex < DMX_FRAMER_COUNT);

    switch (eDataWidth)
    {
    case DMX_TSOUT_DATAWIDTH_1:
        _DMX_SetFramerMode(u1TsIndex,DMX_FRAMER_SERIAL, TRUE, TRUE); // serial input ,pos,external, enable
        fgRet = TRUE;
        break;

    case DMX_TSOUT_DATAWIDTH_2:
        _DMX_SetFramerMode(u1TsIndex,DMX_FRAMER_TWOBIT, TRUE, TRUE);
        fgRet = TRUE;
        break;

    case DMX_TSOUT_DATAWIDTH_8:
        _DMX_SetFramerMode(u1TsIndex,DMX_FRAMER_PARALLEL, TRUE, TRUE); // parallel input
        fgRet = TRUE;
        break;

    default:
        Printf("Incorrect TS-output endian mode!\n");
        fgRet = FALSE;
        break;
    }

    return fgRet;
}

//-----------------------------------------------------------------------------
/** _DMX_TsOut_SetEndianess
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TsOut_SetEndianess(UINT8 u1TsOutIdx, DMX_TSOUT_ENDIAN_T eEndian)
{
    BOOL fgRet = FALSE;
    UINT32 u4Reg;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (u1TsOutIdx == 0)
    {
        switch (eEndian)
        {
        case DMX_TSOUT_ENDIAN_MSB:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
            u4Reg &= ~((UINT32)1 << 18);  // clear bit 18
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        case DMX_TSOUT_ENDIAN_LSB:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
            u4Reg |= (UINT32)1 << 18;  // set bit 18
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output endian mode!\n");
            fgRet = FALSE;
            break;
        }
    }
    else if (u1TsOutIdx == 1)
    {
        switch (eEndian)
        {
        case DMX_TSOUT_ENDIAN_MSB:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL);
            u4Reg &= ~((UINT32)1 << 18);  // clear bit 18
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        case DMX_TSOUT_ENDIAN_LSB:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL);
            u4Reg |= (UINT32)1 << 18;  // set bit 18
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output endian mode!\n");
            fgRet = FALSE;
            break;
        }
    }
    else if(u1TsOutIdx == 2)
    {
        switch (eEndian)
        {
        case DMX_TSOUT_ENDIAN_MSB:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL);
            u4Reg &= ~((UINT32)1 << 18);  // clear bit 18
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        case DMX_TSOUT_ENDIAN_LSB:
            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL);
            u4Reg |= (UINT32)1 << 18;  // set bit 18
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4Reg);
            fgRet = TRUE;
            break;

        default:
            Printf("Incorrect TS-output endian mode!\n");
            fgRet = FALSE;
            break;
        }
    }
    else
    {
        ASSERT(0);
    }

    return fgRet;
}

//-----------------------------------------------------------------------------
/** _DMX_TsOut_SetRountDBM
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TsOut_SetRountDBMTsout(UINT8 u1TsOutIdx, BOOL Enable_DBM, BOOL Enable_Tsout)
{
    UINT32 u4Reg;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);

    if (u1TsOutIdx == 0)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL) & 0xFFFCFFFF;
        if (Enable_DBM)
        {
            u4Reg |= (1 << 17);     // Enable PES/DBM path
        }
        if (Enable_Tsout)
        {
            u4Reg |= (1 << 16);     // Enable Tsoutput path
        }
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
    }
    else if (u1TsOutIdx == 1)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER2_CTRL) & 0xFFFCFFFF;
        if (Enable_DBM)
        {
            u4Reg |= (1 << 17);     // Enable PES/DBM path
        }
        if (Enable_Tsout)
        {
            u4Reg |= (1 << 16);     // Enable Tsoutput path
        }
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER2_CTRL, u4Reg);
    }
	else if(u1TsOutIdx == 2)
	{//
	    u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER3_CTRL) & 0xFFFCFFFF;
        if (Enable_DBM)
        {
            u4Reg |= (1 << 17);     // Enable PES/DBM path
        }
        if (Enable_Tsout)
        {
            u4Reg |= (1 << 16);     // Enable Tsoutput path
        }
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER3_CTRL, u4Reg);
	}
    else
    {
        ASSERT(0);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_TsOut_SetClock
 */
//-----------------------------------------------------------------------------
VOID _DMX_TsOut_SetClock(UINT8 u1TsOutIdx, DMX_PVRPLAY_OUTPUT_CLOCK_T eClock)
{
    UINT32 u4Reg;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);
    ASSERT(eClock < DMX_PVR_OUTPUT_CLOCK_MAX);

    if (u1TsOutIdx == 0)
    {
        u4Reg = CKGEN_READ32(CKGEN_TSOUT_CKCFG);
        u4Reg &= ~0xF;
        u4Reg |= eClock;
        CKGEN_WRITE32(CKGEN_TSOUT_CKCFG, u4Reg);
    }
    else if (u1TsOutIdx == 1)
    {
        u4Reg = CKGEN_READ32(CKGEN_TSOUT_CKCFG2);
        u4Reg &= ~0xF;
        u4Reg |= eClock;
        CKGEN_WRITE32(CKGEN_TSOUT_CKCFG2, u4Reg);
    }
    else if(u1TsOutIdx == 2)
    {/*need define the CKGEN_TSOUT_CKCFG3 value later*/
        u4Reg = CKGEN_READ32(CKGEN_TSOUT_CKCFG3);
        u4Reg &= ~0xF;
        u4Reg |= eClock;
        CKGEN_WRITE32(CKGEN_TSOUT_CKCFG3, u4Reg);
    }
    else
    {
        ASSERT(0);
    }

    return ;
}

//-----------------------------------------------------------------------------
/** _DMX_TsIn_SetRountMux
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TsIn_SetRountMux(UINT8 u1TsIdx, UINT8 u1TsOutIdx)
{
    UINT32 u4Reg;

    ASSERT(u1TsOutIdx < DMX_MAX_TSOUT);
    ASSERT(u1TsIdx < DMX_FRAMER_COUNT);

	if(u1TsIdx == DMX_FRAMER_TOTAL_INDEX)
	{
	  LOG(0,"set the framer error.\r\n");
	  return FALSE;
	}
    //D160[7:0]  tsout-demux TS_in connect to ts[0,1,2,3]
    u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG);
	if(u1TsIdx == 0)
	{
	  u4Reg &= ~(0x3);
	  u4Reg |= (u1TsOutIdx&0x3)<<0;
	}
	else if(u1TsIdx == 1) 
	{
	  u4Reg &= ~(0x3<<2);
	  u4Reg |= (u1TsOutIdx&0x3)<<2;
	}
	else if(u1TsIdx == 2)
	{
	  u4Reg &= ~(0x3<<4);
	  u4Reg |= (u1TsOutIdx&0x3)<<4;
	}
	else if(u1TsIdx == 3)
	{
	  u4Reg &= ~(0x3<<6);
	  u4Reg |= (u1TsOutIdx&0x3)<<6;
	}
    CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);

    return TRUE;
}

BOOL _DMX_SetTsOut(UINT8 u1TsIndex, DMX_TSOUT_PATH_T ePath)
{
    BOOL fgRet = FALSE;
    UINT32 u4Reg;

    switch (ePath)
    {
    case DMX_TSOUT_PATH_INTERNAL:/*just for tsout0*/
        if ((u1TsIndex == 0) || (u1TsIndex == 1) || (u1TsIndex == 3))
        {
            if (u1TsIndex == 0)
            {
                if (!DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_FRAMER0))
                {
                    return FALSE;
                }
            }
            else if (u1TsIndex == 1)
            {
                if (!DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_FRAMER1))
                {
                    return FALSE;
                }
            }
            else if (u1TsIndex == 3)
            {
                if (!DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_FRAMER3))
                {
                    return FALSE;
                }
            }

            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL) & 0xFFFCFFFF;
            u4Reg |= (1 << 16);     // Enable TSOUT path
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
            _DMX_SetFrontEndEx(u1TsIndex, DMX_FE_TSOUT);
        }
        else if (u1TsIndex == 2)
        {
            if (!DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_DBM))
            {
                return FALSE;
            }

            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL) & 0xFFFCFFFF;
            u4Reg |= (1 << 17);     // Enable DBM path
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
        }
        else
        {
            Printf("Tsindex: 0x%d does not support tsout.\n", u1TsIndex);
            fgRet = FALSE;
        }
        fgRet = TRUE;
        break;

    case DMX_TSOUT_PATH_EXTERNAL:
        if (!DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_TSOUT))
        {
            return FALSE;
        }

        if (!_DMX_TsOut_SetLatchEdge(0, DMX_TSOUT_LATCH_EDGE_NEGATIVE))
        {
            return FALSE;
        }

        if (!_DMX_TsOut_SetValidCycle(0, 0))
        {
            return FALSE;
        }

        if (!_DMX_TsOut_SetSyncCycle(0, DMX_TSOUT_SYNC_CYCLE_8))
        {
            return FALSE;
        }

        if (!_DMX_TsOut_SetDataWidth(0, DMX_TSOUT_DATAWIDTH_1))
        {
            return FALSE;
        }

        if (!_DMX_TsOut_SetEndianess(0, DMX_TSOUT_ENDIAN_MSB))
        {
            return FALSE;
        }

        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL) & 0xFFFCFFFF;
        u4Reg |= (1 << 16);     // Enable TSOUT path
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);

        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x700;
        u4Reg |= 0x200; // 2: dmxout, 3: tsout, only demxout can config "positive/negative edge" and "serial/parallel" of TSOUT
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);

        u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) & ~(1<<6);
        CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

        // tsout enable
        u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) | (1<<13);
        CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

#if defined(CC_MT5398)
        // pmux1[18:16], function 1
        // pmux1[22:20], function 1, not set in here now
        BSP_PinSet(PIN_PVR_TSCLK, 1);
        BSP_PinSet(PIN_PVR_TSDATA1, 1);
#elif defined(CC_MT5880)
        BSP_PinSet(PIN_CIGPIO48, 3);
        BSP_PinSet(PIN_CIGPIO45, 3);
        BSP_PinSet(PIN_CIGPIO34, 3);
        BSP_PinSet(PIN_CIGPIO36, 3);
        BSP_PinSet(PIN_CIGPIO37, 3);
        BSP_PinSet(PIN_CIGPIO41, 3);
#endif
        fgRet = TRUE;
        break;

    case DMX_TSOUT_PATH_DISABLE:
        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL) & 0xFFFCFFFF; // clear dmx tsout
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);

#if defined(CC_MT5398)
        // pmux1[18:16], function 0
        BSP_PinSet(PIN_PVR_TSCLK, 0);
        BSP_PinSet(PIN_PVR_TSDATA1, 0);
#elif defined(CC_MT5880)

        BSP_PinSet(PIN_CIGPIO48, 0);
        BSP_PinSet(PIN_CIGPIO45, 0);
        BSP_PinSet(PIN_CIGPIO34, 0);
        BSP_PinSet(PIN_CIGPIO36, 0);
        BSP_PinSet(PIN_CIGPIO37, 0);
        BSP_PinSet(PIN_CIGPIO41, 0);
#endif


        // tsout pad disable
        u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) & ~(1<<13);
        CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

        // recover to demod
        u4Reg = CKGEN_READ32(0x2B4) & ~0x700;
        CKGEN_WRITE32(0x2B4, u4Reg);

        break;
    case DMX_TSOUT_STEER_DBM:
        if (u1TsIndex == 2)
        {
            if (!DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_FRAMER2_BYPASS))
            {
                return FALSE;
            }

            u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL) & 0xFFFCFFFF;
            u4Reg |= (1 << 17);     // Enable DBM path
            DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);

            fgRet = TRUE;
        }
        else
        {
            fgRet = FALSE;
        }
        break;

    case DMX_TSOUT_STEER_TSOUT:
        if (!DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_TSOUT))
        {
            return FALSE;
        }

        // set port to tsout is set to parallel, need to change back to serial
        if (!_DMX_TsOut_SetDataWidth(0, DMX_TSOUT_DATAWIDTH_1))
        {
            return FALSE;
        }

        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL) & 0xFFFCFFFF;
        u4Reg |= (1 << 16);     // Enable TSOUT path
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);
        fgRet = TRUE;
        break;
    default:
        Printf("Incorrect TS-output route!\n");
        fgRet = FALSE;
    }

    return fgRet;
}

#if defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)

//-----------------------------------------------------------------------------
/** _DMX_TsOut_SetRoute
 * _DMX_SetBypassMode() resets the registers in DMX.  Be sure to call it before
 * other operations are called.  This function must be called before the other
 * four functions above are called.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TsOut_SetRoute(UINT8 u1TsIndex, UINT8 u1TsoutIdx, DMX_TSOUT_ROUTE_T eRoute)
{
    BOOL fgRet = FALSE;
    UINT32 u4Reg;

    switch (eRoute)
    {
    case DMX_TSOUT_ROUTE_DBM_LOOPBACK:

        ASSERT(u1TsoutIdx < DMX_MAX_TSOUT);

        //Only ts index 2 and ts index 3 can bypass Framer
        ASSERT((u1TsIndex == 2) || (u1TsIndex == 3) ||(u1TsIndex == 4));

        //Set DBM bypass mode
        if (!_DMX_SetBypassMode(u1TsIndex, 188, FALSE, TRUE))
        {
            Printf("Failed to set bypass mode\n");
            return FALSE;
        }

        //Set TSout FrontEnd
        if (u1TsIndex == 2)
        {
            _DMX_PVRPlay_SetPort(u1TsoutIdx,DMX_PVRPLAY_PORT_FRAMER2_BYPASS);
        }
        else if (u1TsIndex == 3)
        {
            _DMX_PVRPlay_SetPort(u1TsoutIdx,DMX_PVRPLAY_PORT_FRAMER3_BYPASS);
        }
		else if (u1TsIndex == 4)
		{ 
		    _DMX_PVRPlay_SetPort(u1TsoutIdx,DMX_PVRPLAY_PORT_DBM);
		}
        else
        {
            ASSERT(0);
        }

        fgRet = TRUE;
        break;

    case DMX_TSOUT_ROUTE_FRAMER_LOOPBACK:

        ASSERT(u1TsoutIdx < DMX_MAX_TSOUT);
        ASSERT(u1TsIndex < DMX_FRAMER_COUNT);
        if(u1TsIndex == 4)
        {
          Printf("No framer in Ts_idx4.\r\n");
		  return FALSE;
        }
        //Set DBM bypass mode
        if (!_DMX_SetBypassMode(u1TsIndex, 188, FALSE, TRUE))
        {
            Printf("Failed to set bypass mode\n");
            return FALSE;
        }

        //Set TSout FrontEnd
        if (u1TsIndex == 0)
        {
            _DMX_PVRPlay_SetPort(u1TsoutIdx,DMX_PVRPLAY_PORT_FRAMER0);
        }
        else if (u1TsIndex == 1)
        {
            _DMX_PVRPlay_SetPort(u1TsoutIdx,DMX_PVRPLAY_PORT_FRAMER1);
        }
        else if (u1TsIndex == 2)
        {
            _DMX_PVRPlay_SetPort(u1TsoutIdx,DMX_PVRPLAY_PORT_FRAMER2);
        }
        else if (u1TsIndex == 3)
        {
            _DMX_PVRPlay_SetPort(u1TsoutIdx,DMX_PVRPLAY_PORT_FRAMER3);
        }
        else
        {
            ASSERT(0);
        }

        fgRet = TRUE;
        break;

    case DMX_TSOUT_ROUTE_EXT_LOOPBACK:

        ///-----------------------------------
        //  0x304 --> REG_TSOUT_CKCFG
        //   0x158 --> CI_TSIF_CFG
        //   0X160 --> EXTTS_CKCKG

        if (!_DMX_SetBypassMode(u1TsIndex, 188, FALSE, TRUE))
        {
            Printf("Failed to set bypass mode\n");
            return FALSE;
        }

        //Set ts output enalbe
        _DMX_PVRPlay_SetPort(u1TsoutIdx,DMX_PVRPLAY_PORT_TSOUT);

        //Set output ts clock 108M
        _DMX_TsOut_SetClock(u1TsoutIdx, DMX_PVR_OUTPUT_CLOCK_108M);

        //S2P Disable
        //u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) & ~(1<<6);
        //CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

        // tsout enable
        //u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) | (1<<13);
        //CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

        u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) & ~0xFFFF | 0x60A8;
        
        //ci tso/ci tso2 data clock enable (1:diable,0:enable), default is disable
        //bit31:CI TSO2 sync,data,valid output disable
        //bit30:CI TSO   sync,data,valid output disable
        //bit29:CI TSO2 clock output disable
        //bit28:CI TSO   clock output disable
        u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) & ~(0xF<<28);
        CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

        //bypass CI framer
        //0: from the CI framer 
        //1: bypass CI framer, mux reslut direct output. 
        //bit0:CI TSO output model select 
        //bit1:CI TSO2 output model select 
        u4Reg = CKGEN_READ32(CKGEN_TSIF2_CFG) | 0xF;
        CKGEN_WRITE32(CKGEN_TSIF2_CFG, u4Reg);


    //-----------------case 1 Serial out--------------------
    //PAD_PVR_TSCLK-->CLK
    //PAD_PVR_TSVAL-->VALID
    //PAD_PVR_TSSYNC-->SYNC
    //PAD_PVR_TSDATA0-->DATA0
    //PAD_PVR_TSDATA1-->DATA1

     //tso
     BSP_PinSet(PIN_PVR_TSCLK, 1);
     BSP_PinSet(PIN_PVR_TSDATA1, 1);

     //tso
     //BSP_PinSet(PIN_PVR_TSCLK, 4);
     //BSP_PinSet(PIN_PVR_TSDATA1, 4);

     //tso2
     //BSP_PinSet(PIN_PVR_TSCLK, 6);
     //BSP_PinSet(PIN_PVR_TSDATA1, 6);

     if (u1TsoutIdx == 0)
    {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x2200; // //  dmxout1 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
    }
    else if (u1TsoutIdx == 1)
    {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x3300; // //  dmxout2 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
    }
    else
    {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x7700; // //  dmxout3 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
    }

#if 0

        //-----------------case 2  Serial out--------------------
        //PAD_DEMOD_TSDATA1 -->CLK
        //PAD_DEMOD_TSDATA2 -->VALID
        //PAD_DEMOD_TSDATA3 -->SYNC
        //PAD_DEMOD_TSDATA4 -->DATA0

        BSP_PinSet(PIN_DEMOD_TSDATA1, 4);

        if (u1TsoutIdx == 0)
        {
            u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7000;
            u4Reg |= 0x2000; //  dmxout ts clk
            CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
        }
        else if (u1TsoutIdx == 1)
        {
            u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7000;
            u4Reg |= 0x3000; // dmxout2 ts clk
            CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
        }
        else
        {
            u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7000;
            u4Reg |= 0x7000; // dmxout3 ts clk
            CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
        }


        //-----------------case 3  Serial out--------------------
        //PAD_GPIO15 --> TSCLK
        //PAD_GPIO16 --> TSVAL
        //PAD_GPIO17 --> TSSYNC
        //PAD_GPIO18 --> TSDATA0

        BSP_PinSet(PIN_GPIO15, 5);
        BSP_PinSet(PIN_GPIO18, 5);

        if (u1TsoutIdx == 0)
        {
            u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7000;
            u4Reg |= 0x2000; //  dmxout ts clk
            CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
        }
        else if (u1TsoutIdx == 1)
        {
            u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7000;
            u4Reg |= 0x3000; // dmxout2 ts clk
            CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
        }
        else
        {
            u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7000;
            u4Reg |= 0x7000; // dmxout3 ts clk
            CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
        }

#endif

	fgRet = TRUE;
    break;

    case DMX_TSOUT_ROUTE_INTERNAL_CI:
        if (!_DMX_SetBypassMode(u1TsIndex, 188, FALSE, TRUE))
        {
            Printf("Failed to set bypass mode\n");
            return FALSE;
        }

        if ((u1TsIndex == 0) || (u1TsIndex == 1))
        {
            _DMX_SetFrontEndEx(u1TsIndex, DMX_FE_CI);
        }

        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
        u4Reg = (u4Reg & 0xFFFF00FF) | 0x0000FF00;  // reset Framer
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);
        u4Reg &= 0xFFFF00FF;                        // stop Framer reset
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);

        // Disable both Framer's error handling mechanism
        _DMX_SetFramerPacketErrorHandling(u1TsIndex, FALSE, 0);

        // Set the TS-index to be bypassed.
        u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL) & ~(0x3C000000);
        u4Reg |= 1 << (u1TsIndex + 26);
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);

        u4Reg = DMXCMD_READ32(DMX_REG_PES_DBM_STEER_CTRL);
        u4Reg |= (UINT32)1 << 16;       // Enable TS-output path
        u4Reg &= ~((UINT32)1 << 17);    // Disable PES/DBM path
        u4Reg |= (UINT32)1 << 23;       // TS parallel mode
        DMXCMD_WRITE32(DMX_REG_PES_DBM_STEER_CTRL, u4Reg);

        ///-----------------------------------
        //  0x304 --> REG_TSOUT_CKCFG
        //   0x158 --> CI_TSIF_CFG
        //   0X160 --> EXTTS_CKCKG
        CKGEN_WRITE32(CKGEN_TSOUT_CKCFG, 0);   // tsout clk: xtal

        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x700;
        u4Reg |= 0x200; // 2: dmxout, 3: tsout, only demxout can config "positive/negative edge" and "serial/parallel" of TSOUT
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);

        u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) & ~(1<<6);
        CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

        // tsout enable
        u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) | (1<<13);
        CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

        fgRet = TRUE;
        break;

    default:
        Printf("Incorrect TS-output route!\n");
        return FALSE;
    }

    return fgRet;
}

BOOL _DMX_TsOut_Config(UINT8 u1TsOutSel, UINT8 u1TsOutindex)
{
    UINT32 u4Reg;
    
    //S2P Disable
    //u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) & ~(1<<6);
    //CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

    // tsout enable
    //u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) | (1<<13);
    //CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

    u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) & ~0xFFFF | 0x60A8;
    
    //ci tso/ci tso2 data clock enable (1:diable,0:enable), default is disable
    //bit31:CI TSO2 sync,data,valid output disable
    //bit30:CI TSO   sync,data,valid output disable
    //bit29:CI TSO2 clock output disable
    //bit28:CI TSO   clock output disable
    u4Reg = CKGEN_READ32(CKGEN_TSIF_CFG) & ~(0xF<<28);
    CKGEN_WRITE32(CKGEN_TSIF_CFG, u4Reg);

    //bypass CI framer
    //0: from the CI framer 
    //1: bypass CI framer, mux reslut direct output. 
    //bit0:CI TSO output model select 
    //bit1:CI TSO2 output model select 
    u4Reg = CKGEN_READ32(CKGEN_TSIF2_CFG) | 0xF;
    CKGEN_WRITE32(CKGEN_TSIF2_CFG, u4Reg);


    switch(u1TsOutSel)
    {
    case 0:
    {
#if defined(CC_MT5890)
        //PAD_PVR_TSCLK-->CLK
        //PAD_PVR_TSVAL-->VALID
        //PAD_PVR_TSSYNC-->SYNC
        //PAD_PVR_TSDATA0-->DATA0
        //PAD_PVR_TSDATA1-->DATA1
        BSP_PinSet(PIN_PVR_TSCLK, 1);
        BSP_PinSet(PIN_PVR_TSDATA1, 1);
#else  //MT5882
        Printf("Not support this tsout pin!\n");
#endif
        break;
    }

    case 1:
    {
        //PAD_PVR_TSCLK-->CLK
        //PAD_PVR_TSVAL-->VALID
        //PAD_PVR_TSSYNC-->SYNC
        //PAD_PVR_TSDATA0-->DATA0
        //PAD_PVR_TSDATA1-->DATA1
        BSP_PinSet(PIN_PVR_TSCLK, 4);
        BSP_PinSet(PIN_PVR_TSDATA1, 4);
        break;
    }
    
    case 2:
    {
        //PAD_PVR_TSCLK-->CLK
        //PAD_PVR_TSVAL-->VALID
        //PAD_PVR_TSSYNC-->SYNC
        //PAD_PVR_TSDATA0-->DATA0
        //PAD_PVR_TSDATA1-->DATA1
#if defined(CC_MT5890)
        BSP_PinSet(PIN_PVR_TSCLK, 6);
        BSP_PinSet(PIN_PVR_TSDATA1, 6);
#else  //MT5882
        BSP_PinSet(PIN_PVR_TSCLK, 5);
        BSP_PinSet(PIN_PVR_TSDATA1, 5);
#endif
        break;
    }
    case 3:
    {
#if defined(CC_MT5890)
        //PAD_DEMOD_TSDATA1 -->CLK
        //PAD_DEMOD_TSDATA2 -->VALID
        //PAD_DEMOD_TSDATA3 -->SYNC
        //PAD_DEMOD_TSDATA4 -->DATA0
        BSP_PinSet(PIN_DEMOD_TSDATA1, 4);
#else  //MT5882
        Printf("Not support this tsout pin!\n");
#endif
        break;
    }

    case 4:
    {
        //PAD_GPIO15 --> TSCLK
        //PAD_GPIO16 --> TSVAL
        //PAD_GPIO17 --> TSSYNC
        //PAD_GPIO18 --> TSDATA0
        BSP_PinSet(PIN_GPIO15, 5);
        BSP_PinSet(PIN_GPIO18, 5);
        break;
    }
    
    case 5:
        ASSERT(0);
        break;
    }

    if (u1TsOutindex == 0)
    {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x2200; // //  dmxout1 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
    }
    else if (u1TsOutindex == 1)
    {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x3300; // //  dmxout2 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
    }
    else
    {
        //tso,tso2 both set
        u4Reg = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & ~0x7700;
        u4Reg |= 0x7700; // //  dmxout3 ts clk
        CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Reg);
    }

    return TRUE;
}
//-----------------------------------------------------------------------------
/** _DMX_TsOut_TransmitData
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TsOut_TransmitData(UINT8 u1TsIndex, UINT8 u1TsoutIdx, UINT32 u4Size)
{
    BOOL fgRet = FALSE;
    UINT32 u4SrcBufSize, u4DstBufSize;
    UINT32 u4SrcAddr = 0, u4SrcDiff, u4SrcSize;
    UINT32 u4DstAddr = 0, u4DstDiff, u4DstEndAddr;
    UINT32 u4PesAddr = 0, u4PesSize;
    UINT32 u4Count;
    DMX_PVR_PLAY_T rPlay;
    UINT32 u4Flags;
    UINT32 u4Rp, u4Wp, u4Reg;
    UINT32 i;

    if (u4Size <= DMX_IGNORE_TRAILING_LEN)
    {
        Printf("Length %u is too short!\n", u4Size);
        return FALSE;
    }

    //Init in DMX init
    #if 0
    // Init PVR
    if (!_DMX_PVRPlay_Init())
    {
        Printf("Failed to initialize PVR!\n");
        return FALSE;
    }
    #endif

    // Allocate source buffer.  (Mini-PVR RP/WP 8-byte alignment, 188 * 4 = 752)
    u4SrcSize = _DMX_Align(u4Size, 752);
    u4SrcBufSize = u4SrcSize + DMX_CA_BUFFER_ALIGNMENT;
    u4SrcAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4SrcBufSize, DMX_CA_BUFFER_ALIGNMENT);
    if (u4SrcAddr == 0)
    {
        Printf("Failed to allocate source buffer!\n");
        return FALSE;
    }

    // Allocate destination buffer
    // It's important to make the CA output buffer slightly larger.
    u4DstBufSize = _DMX_Align(u4SrcBufSize + 1000, DMX_CA_BUFFER_ALIGNMENT);
    u4DstAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4DstBufSize, DMX_CA_BUFFER_ALIGNMENT);
    if (u4DstAddr == 0)
    {
        Printf("Failed to allocate destination buffer!\n");
        fgRet = FALSE;
        goto FailAndFreeMemory;
    }

    //clear dst buffer
    for ( i = 0 ; i < u4DstBufSize/4 ; i++ )
    {
        ((UINT32*)u4DstAddr)[i] = 0;
    }
    u4DstEndAddr = u4DstAddr + u4DstBufSize;

    // Set PVR playback
    rPlay.fgAllocBuf = FALSE;
    rPlay.u4BufStart = u4SrcAddr;
    rPlay.u4BufSize = u4SrcBufSize;
    rPlay.u4ThresholdSize = u4SrcSize;  // FIXME
    rPlay.pfnPVRNotify = NULL;
    u4Flags = PVRPLAY_FLAGS_BUFFER | PVRPLAY_FLAGS_THRESHOLD |
              PVRPLAY_FLAGS_CALLBACK;
    if (!_DMX_PVRPlay_Set(u1TsoutIdx, u4Flags, &rPlay))
    {
        Printf("Failed to set PVR properties!\n");
        fgRet = FALSE;
        goto FailAndFreeMemory;
    }

    //Set CA output buffer
    if (!_DMX_SetCaOutputBuffer(TRUE, TRUE, u4DstAddr, u4DstEndAddr, u4SrcSize))
    {
        Printf("Failed to set CA output buffer!\n");
        fgRet = FALSE;
        goto FailAndFreeMemory;
    }

    _GenerateRandomPattern(u4SrcAddr, u4SrcSize);

    u4PesAddr = (UINT32)x_mem_alloc(u4SrcSize);
    if (u4PesAddr == 0)
    {
        Printf("Failed to allocate payload buffer!\n");
        fgRet = FALSE;
        goto FailAndFreeMemory;
    }

    // The input to 8295 needs to be in TS packet format.
    if (!_GenerateSimpleTsHeader(u4SrcAddr, u4SrcSize, 0, u4PesAddr,
                                 &u4PesSize))
    {
        Printf("Failed to generate TS packets!\n");
        fgRet = FALSE;
        goto FailAndFreeMemory;
    }

    HalFlushInvalidateDCache();

    Printf("Src address: 0x%08x\n", u4SrcAddr);
    Printf("Dst address: 0x%08x\n", u4DstAddr);
    Printf("Original Src size: %u.  New Src Size: %u\n", u4Size, u4SrcSize);

    // Set Full pull mode
    u4Reg = DMXCMD_READ32(DMX_REG_CONFIG2);
    u4Reg |= 0xC0000000;
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Reg);

    VERIFY(_DMX_PVRPlay_FlushBuf(u1TsoutIdx));
    VERIFY(_DMX_PVRPlay_UpdateWp(u1TsoutIdx, u4SrcAddr, u4SrcSize));
    VERIFY(_DMX_PVRPlay_Start(u1TsoutIdx));

    // Wait for some time.
    // x_thread_delay(3000);
    u4Count = 0;
    while (1)
    {
        u4Wp = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_WP);
        u4Rp = DMXCMD_READ32(DMX_REG_CA_OUT_BUF_RP);
        if ((u4Wp - u4Rp) >= (u4SrcSize - DMX_IGNORE_TRAILING_LEN))  // Do not consider wrap around for now.
        {
            break;
        }
        if (((++u4Count) >= 300))  // two outmost pairs of parenthesis are necessary
        {
            Printf("Exceed loop count %u!\n", u4Count);
            fgRet = FALSE;
            goto FailAndFreeMemory;
        }
        x_thread_delay(1);
    }

    // Check result
    if (!_MemCmp(u4SrcAddr, u4DstAddr,
                 u4SrcSize - DMX_IGNORE_TRAILING_LEN, &u4SrcDiff, &u4DstDiff))
    {
        Printf("Differ at src: 0x%08x, dst: 0x%08x\n", u4SrcDiff, u4DstDiff);
        fgRet = FALSE;
    }
    else
    {
        fgRet = TRUE;
    }

FailAndFreeMemory:
    VERIFY(_DMX_PVRPlay_Stop(u1TsoutIdx, TRUE));
    VERIFY(_DMX_PVRPlay_FreeBuf(u1TsoutIdx));
    VERIFY(BSP_FreeAlignedDmaMemory(u4SrcAddr));
    VERIFY(BSP_FreeAlignedDmaMemory(u4DstAddr));
    if (u4PesAddr != 0)
    {
        x_mem_free((void*)u4PesAddr);
    }

    // Set back
    u4Reg = DMXCMD_READ32(DMX_REG_CONFIG2);
    u4Reg &= ~0xC0000000;
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Reg);

    return fgRet;
}

#endif  // defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)
//#endif  // defined(CC_DMX_EMULATION) || defined(__MODEL_slt__) || defined(CC_DMX_ENABLE_TSOUT)


