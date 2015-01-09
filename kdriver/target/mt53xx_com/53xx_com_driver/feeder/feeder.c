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
 * Copyright (c) 2004, CrystalMedia Technology, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * 
 *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "drv_common.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "feeder.h"
#ifdef CC_ONLY_FOR_SH
#include "swdmx_if.h"
#endif
#include "fbm_drvif.h"
#include "x_util.h"
#ifdef __KERNEL__

#include <linux/mt53xx_cb.h>    // for Linux driver using callback function

extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
#endif

//#define FEEDER_TIME_PROFILE
//#define FEEDER_TIME_PROFILE_REQUEST

#define  FEEDER_MULT_INSTANCE_TYPE (1)//1,single-thread,mult-queue;2,mult-thread,mult-queues

#ifdef FEEDER_TIME_PROFILE
#include "x_timer.h"
#define FEEDER_TIME_PROFILE_ARRARY_SIZE	32
static FeederSourceType _feeder_time_profile_type=FEEDER_PROGRAM_SOURCE;
static HAL_TIME_T _rDmxEnd;
static HAL_TIME_T _rDmxAcc;
static HAL_TIME_T _rDmxdif;
#ifdef FEEDER_TIME_PROFILE_REQUEST
static HAL_TIME_T _rMtmPlayStart;
static HAL_TIME_T _rMtmPlayEnd;
static HAL_TIME_T _rMtmPlayAcc;
static HAL_TIME_T _rMtmPlaydif;
#endif
static UINT32 _u4DmxTimeIndex=0;
typedef struct 
{
 UINT32 u4ReqID;
 FeederSourceType feederSrcType; 
 HAL_TIME_T rDmxStart;
}T_DMX_TIME_PROFILE;

static T_DMX_TIME_PROFILE _rDmxStartArray[FEEDER_TIME_PROFILE_ARRARY_SIZE]={0};
#endif



extern BOOL AUD_FeederSetRequestInfo(FEEDER_TRANSMIT *prTransmit);
//VOID _FeederWatchTimeout(HANDLE_T hTimerHandle, VOID *pvTag);
BOOL _FeederSetRequestInfo(FEEDER_TRANSMIT *prTransmit);

#define FEEDER_SUPPORT_FBM_ALLOC

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

static FEEDER_INFO_T *pgrFeederInfo[FEEDER_SOURCE_INVALID] =
    {NULL,
    NULL,
    NULL,
    NULL,
    NULL,
#ifdef FEEDER_FIVE_INSTANCE
    NULL, NULL, NULL, NULL, NULL, NULL,
#endif
#ifdef CC_FBM_FOUR_INST_SUPPORT
    NULL,NULL,NULL,NULL,
#endif
    NULL
    };
static FeederState   geFeederState[FEEDER_SOURCE_INVALID] =
    {FEEDER_IDLE,
    FEEDER_IDLE,
    FEEDER_IDLE,
    FEEDER_IDLE,
    FEEDER_IDLE,
#ifdef FEEDER_FIVE_INSTANCE
    FEEDER_IDLE,
    FEEDER_IDLE,
    FEEDER_IDLE,
    FEEDER_IDLE,
    FEEDER_IDLE,
    FEEDER_IDLE,
#endif
#ifdef CC_FBM_FOUR_INST_SUPPORT
    FEEDER_IDLE,
    FEEDER_IDLE,
    FEEDER_IDLE,
    FEEDER_IDLE,
#endif
    FEEDER_IDLE
    };
static FEEDER_SET_TRANSMIT garTransmitFuncTbl[FEEDER_SOURCE_INVALID] =
{
    AUD_FeederSetRequestInfo,
     _FeederSetRequestInfo,
     _FeederSetRequestInfo,
     _FeederSetRequestInfo,
     _FeederSetRequestInfo,
#ifdef FEEDER_FIVE_INSTANCE
     _FeederSetRequestInfo,
     _FeederSetRequestInfo,
     _FeederSetRequestInfo,
     _FeederSetRequestInfo,
     _FeederSetRequestInfo,
     _FeederSetRequestInfo,
#endif
#ifdef CC_FBM_FOUR_INST_SUPPORT
     _FeederSetRequestInfo,
     _FeederSetRequestInfo,
     _FeederSetRequestInfo,
     _FeederSetRequestInfo,
#endif
     _FeederSetRequestInfo
};

static HANDLE_T _hFeederDataQ[FEEDER_SOURCE_INVALID] = {0};
#if FEEDER_MULT_INSTANCE_TYPE == 2
static HANDLE_T _hFeederThread[FEEDER_SOURCE_INVALID] = {0};
#else
static HANDLE_T _hFeederDummyDataQ  = 0;
static HANDLE_T _hFeederThread = 0;
#endif
static UINT32   _u4DataDelayTime=0;

static UINT32 _u4BlockSize[FEEDER_SOURCE_INVALID] =
{
    512,
    512,
    512,
    512,
    512,
#ifdef FEEDER_FIVE_INSTANCE
    512, 512, 512, 512, 512, 512,
#endif
#ifdef CC_FBM_FOUR_INST_SUPPORT
    512,512,512,512,
#endif
    512
};

static FeederSourceType_T _eFeederSrcType[FEEDER_SOURCE_INVALID] = {FEEDER_SRC_TYPE_HIGH_SPEED_STORAGE};
static BOOL _gfgUseMiddleware[FEEDER_SOURCE_INVALID] =    
{
    FALSE,
    FALSE,
    FALSE,
    FALSE,
    FALSE,
#ifdef FEEDER_FIVE_INSTANCE
    FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
#endif
#ifdef CC_FBM_FOUR_INST_SUPPORT
    FALSE,FALSE,FALSE,FALSE,
#endif
    FALSE
};


static BOOL _fgFeederSkypeSource =FALSE;


static FEEDER_CALBACK garNotifyTbl[FEEDER_SOURCE_TOTAL][FEEDER_NOTIFY_MAX];

static FEEDER_URI_DATA_T _rFeederUriData[FEEDER_SOURCE_INVALID];

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
#if 0
VOID _FeederFlushReqMsgQ(FeederSourceType eSrcType)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    FEEDER_REQ_DATA_T rData;

    zMsgSize = sizeof(FEEDER_REQ_DATA_T);   
    
    while (x_msg_q_receive(&u2MsgQIdx, (VOID *)&rData, &zMsgSize,
        &(_hFeederDataQ[eSrcType]), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
        {
    }

    VERIFY(x_msg_q_num_msgs(_hFeederDataQ[eSrcType], &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);    
}
#endif
BOOL _FeederNotifyHandle(FeederSourceType eSrcType, 
                         FEEDER_NOTIFY_COND eCondition, UINT32 u4Param)
{
    MM_NOTIFY_INFO_T rNotify;
    FEEDER_REQ_DATA_T *rData=NULL; 
#ifdef FEEDER_TIME_PROFILE 
    T_DMX_TIME_PROFILE *pDmxStart=_rDmxStartArray;
#endif

    rData=(FEEDER_REQ_DATA_T *)u4Param;
    UNUSED(rData);
    rNotify.u4SrcType   = (UINT32)eSrcType;
    rNotify.u4Condition = (UINT32)eCondition;
    rNotify.u4Param     = u4Param;
	if(!pgrFeederInfo[eSrcType])
	{
       return TRUE;
	}
#ifdef FEEDER_MULTI_INSTANCE
    rNotify.u1PlaymgrId = pgrFeederInfo[eSrcType]->rDataQuery.u1PlaymgrAttachedId;
    rNotify.u1SwdmxId = pgrFeederInfo[eSrcType]->rDataQuery.u1SwdmxAttachedId;
#endif
    LOG(9, "_FeederNotifyHandle: u4SrcType=%d, u4Condition=%d, u4Param=%d,swdmxid = %d\n", rNotify.u4SrcType, rNotify.u4Condition, rNotify.u4Param,rNotify.u1SwdmxId);
    if (u4Param >= 6 &&  // tmp solution for error notify in playmgr (DTV00078019)
        rData && (eCondition == FEEDER_NOTIFY_DATA_REQ_DONE || eCondition == FEEDER_NOTIFY_ERROR))
    {
    	if(rData->eFeederReqType == FEEDER_OFFSET_SEEK)
    	{
            LOG(7, "src[%d],ACK[%ld] Ofst=%X, Sz=%X, Addr=%X, A=%X, P=%d, E=%d\n",
                eSrcType,
                rData->u4Id,
                (UINT32)rData->u8FilePos - rData->u4ReadSize,
                rData->u4ReadSize,
                rData->u4WriteAddr,
                rData->u4AlignOffset,
                (UINT32)rData->fgPartial,
                (UINT32)rData->fgEof);
    	}
		else
    	{
    		LOG(7, "src[%d],ACK[%ld] u8FilePos=%llX, Sz=%X, Addr=%X, A=%X, P=%d, E=%d\n",
            	eSrcType,
            	rData->u4Id,
            	rData->u8FilePos,
            	rData->u4ReadSize,
            	rData->u4WriteAddr,
            	rData->u4AlignOffset,
            	(UINT32)rData->fgPartial,
            	(UINT32)rData->fgEof);
    	}
    }
    
    if ((geFeederState[eSrcType] == FEEDER_IDLE) ||
        (geFeederState[eSrcType] == FEEDER_INIT))
    {
        return TRUE;
    }
    
    if(_gfgUseMiddleware[eSrcType]==FALSE)
    {
#ifdef __KERNEL__
      switch (eCondition)
      {
        case FEEDER_NOTIFY_FILEBEGIN:
           pgrFeederInfo[eSrcType]->bReceiveFileBeginNfy = TRUE;
           break;
        case FEEDER_NOTIFY_FILEEND:
           if ((!pgrFeederInfo[eSrcType]->bReceiveFileBeginNfy) ||(pgrFeederInfo[eSrcType]->rFeedBuf.u4BytesInBuf > 0))
           {
             break;
           }
        default:
           break;
      }

      if (eCondition != FEEDER_NOTIFY_DATA_REQ_DONE)  
      {
         MM_CALLBACK_INFO_T rQueryInfo;
         x_memcpy((VOID *)&rQueryInfo.rMMNotify, (VOID *)&rNotify, sizeof(MM_NOTIFY_INFO_T));
         _CB_PutEvent(CB_MTMPLAYER_TRIGGER, sizeof(MM_CALLBACK_INFO_T), &rQueryInfo);
      }
#endif
    }

    if ((eSrcType >= FEEDER_SOURCE_TOTAL) || (eCondition >= FEEDER_NOTIFY_MAX))
    {
        return FALSE;
    }

	if(pgrFeederInfo[eSrcType])
	{
        pgrFeederInfo[eSrcType]->rFeedBuf.u4UnAckNum--;
	}
	
    if (garNotifyTbl[eSrcType][eCondition] != NULL)
    {
        FEEDER_CALBACK pfnCallback;
        
        pfnCallback = garNotifyTbl[eSrcType][eCondition];
#ifdef FEEDER_TIME_PROFILE
        HAL_GetTime(&_rDmxEnd);
        while(pDmxStart<_rDmxStartArray+FEEDER_TIME_PROFILE_ARRARY_SIZE)
        {
			if(rData->u4Id == pDmxStart->u4ReqID)
			{
              break;
			}
			pDmxStart++;
        }
		
		if(pDmxStart==_rDmxStartArray+FEEDER_TIME_PROFILE_ARRARY_SIZE)
		{
			HAL_GetDeltaTime(&_rDmxdif,&(pDmxStart->rDmxStart), &_rDmxEnd);
			_rDmxAcc.u4Seconds += _rDmxdif.u4Seconds;
			_rDmxAcc.u4Micros  += _rDmxdif.u4Micros;
			if (_rDmxAcc.u4Micros >= 1000000)
			{
			  _rDmxAcc.u4Seconds += _rDmxAcc.u4Micros / 1000000;
			  _rDmxAcc.u4Micros  %= 1000000;
			}
			LOG(4,"DataReq(Dmx-->Feeder):%d(%d)size=0x%x,Dur=%u.%06u,Total=%u.%06u\n",rData->eDataType,rData->u4Id,rData->u4ReadSize,_rDmxdif.u4Seconds,_rDmxdif.u4Micros,_rDmxAcc.u4Seconds,_rDmxAcc.u4Micros);
		}
		else
		{
			LOG(4,"DataReq(Dmx-->Feeder):%d(%d) can't get start time\n",rData->eDataType,rData->u4Id);
		}
#endif
        pfnCallback(&rNotify, u4Param);
    }
    else if (eCondition == FEEDER_NOTIFY_ERROR)
    {
        FEEDER_CALBACK pfnCallback = NULL;        
        
        if(_gfgUseMiddleware[eSrcType])
        {
            FEEDER_REQ_DATA_T *prData = NULL;
            pfnCallback=garNotifyTbl[eSrcType][FEEDER_NOTIFY_DATA_REQ_DONE];
            prData = (FEEDER_REQ_DATA_T *)u4Param;
            if (prData)
            {
                prData->u4WriteAddr = 0;  // to distinguish success and fail cases
            }
            else
            {
                UNUSED(prData);
            }
        }
#ifndef __KERNEL__
        else
        {
           pfnCallback=garNotifyTbl[eSrcType][FEEDER_NOTIFY_ERROR];
        }
#endif
        if(pfnCallback)
        {
            pfnCallback(&rNotify, u4Param);
        }
    }

    return TRUE;
}
INT32 SendFeederDataQMsg(FeederSourceType eSrcType,
                     HANDLE_T   h_msg_hdl,
                     const VOID *pv_msg,
                     SIZE_T     z_size,
                     UINT8      ui1_pri)
{
   INT32 dummy = 0,ret;
   UNUSED(eSrcType);
   UNUSED(ret);

   
   ret = x_msg_q_send(h_msg_hdl, pv_msg,z_size,ui1_pri);
   if(ret != OSR_OK)
       return ret;
       
   #if FEEDER_MULT_INSTANCE_TYPE == 2
   UNUSED(dummy);
   #else
   x_msg_q_send(_hFeederDummyDataQ,&dummy,sizeof(INT32),255);
   #endif

   return ret;
}
    

VOID _FeederQeuryDataTask(VOID* pvArg)
{
    FEEDER_BUF_INFO_T *pQryBuff;
    FEEDER_INFO_T *prFeederInfo;
    FEEDER_QUERY_CALLBACK pfnQuery;
    FEEDER_REQ_DATA_T rData;
    FeederSourceType eDataType;
    UINT32 u4WriteSize;
    INT32 ret,dummy;
    UINT16 u2MsgQIdx,index = 0,startindex = 0;
    SIZE_T zMsgSize = sizeof(FEEDER_REQ_DATA_T),zDummySz = sizeof(INT32);
    FeederSourceType eSrcType = (FeederSourceType)(*((FeederSourceType*)pvArg));

    x_memset(&rData,0,sizeof(FEEDER_REQ_DATA_T));
    
    LOG(0,"Feeder Task :%d\n",eSrcType);
    
    #if FEEDER_MULT_INSTANCE_TYPE == 2    
    UNUSED(index);
    UNUSED(zDummySz);
    UNUSED(dummy);
    #endif
    
    while (1)
    {
        
        #if FEEDER_MULT_INSTANCE_TYPE == 2
        
	    ret = x_msg_q_receive(&u2MsgQIdx, (VOID *)&rData, &zMsgSize,
		    		&(_hFeederDataQ[eSrcType]), 
		    		1, X_MSGQ_OPTION_WAIT);	
        #else
        
        x_msg_q_receive(&u2MsgQIdx, (VOID *)&dummy, &zDummySz,
		    		&(_hFeederDummyDataQ), 
		    		1, X_MSGQ_OPTION_WAIT);
		    		
		startindex = index;
		ret = OSR_NO_MSG;
	    do
	    {
	    //we may have no message at all.
	    //so this nested loop should exist when all q have been checked but nothing found,
	    if(_hFeederDataQ[index])
		   ret = x_msg_q_receive(&u2MsgQIdx, (VOID *)&rData, &zMsgSize,
		    		&(_hFeederDataQ[index]), 
		    		1, X_MSGQ_OPTION_NOWAIT);
		else
		   LOG(9,"feeder queue %d not set yet,skip\n",index);

        index++;
		(index) %= FEEDER_SOURCE_INVALID;
		
		}while(ret != OSR_OK && index != startindex);
		  
		  
        #endif
        
        eDataType = rData.eDataType;
        LOG(9,"src(%d),get data for %d\n",eSrcType,eDataType);
	    if (eDataType >= FEEDER_SOURCE_INVALID)
	    {
	        continue;
	    }
        prFeederInfo = pgrFeederInfo[eDataType];
        if(!prFeederInfo)
        {
          LOG(3,"Feeder Task,get a invalid request\n");
		  continue;
        }
        pQryBuff = &prFeederInfo->rFeedBuf;
	    pfnQuery = prFeederInfo->rDataQuery.pfnSource;	    
#ifdef FEEDER_TIME_PROFILE_REQUEST
		HAL_GetTime(&_rMtmPlayStart);
        
#endif
        if(_u4DataDelayTime)
        {
           x_thread_delay(_u4DataDelayTime);
        }
		
        if (prFeederInfo->eRequestType == FEEDER_OFFSET_SEEK 
			&& _rFeederUriData[eDataType].u4PreLoadSize)
        {
            u4WriteSize = rData.u4ReadSize;  // do not really send request to BAGT
        }
        else if(prFeederInfo && pfnQuery)
        {
            if(rData.fgPartial==FALSE)
            {
			   pQryBuff->u4UnAckNum++;
            }
			
#ifdef FEEDER_MULTI_INSTANCE
            u4WriteSize = (UINT32)pfnQuery(prFeederInfo->rDataQuery.u1PlaymgrAttachedId, eDataType, pQryBuff, &rData);
#else
            u4WriteSize = (UINT32)pfnQuery( eDataType, pQryBuff, &rData);
#endif
        }
		else
		{
           LOG(0,"Feeder task:Receive request,but feeder closed\n");
		   continue;
		}
#ifdef FEEDER_TIME_PROFILE_REQUEST
		HAL_GetTime(&_rMtmPlayEnd);
		HAL_GetDeltaTime(&_rMtmPlaydif, &_rMtmPlayStart, &_rMtmPlayEnd);
		_rMtmPlayAcc.u4Seconds += _rMtmPlaydif.u4Seconds;
		_rMtmPlayAcc.u4Micros  += _rMtmPlaydif.u4Micros;
		if (_rMtmPlayAcc.u4Micros >= 1000000)
		{
			_rMtmPlayAcc.u4Seconds += _rMtmPlayAcc.u4Micros / 1000000;
			_rMtmPlayAcc.u4Micros  %= 1000000;
		}
        LOG(4,"DataReq(Feeder-->Mtmplayer):%d(%d)size=0x%x,Dur=%u.%06u,Total=%u.%06u\n",eDataType,rData.u4Id,rData.u4ReadSize,_rMtmPlaydif.u4Seconds,_rMtmPlaydif.u4Micros,_rMtmPlayAcc.u4Seconds,_rMtmPlayAcc.u4Micros);
#endif

        if(!_gfgUseMiddleware[eDataType])
        {
	       rData.u4ReadSize = u4WriteSize;
	       rData.u4AlignOffset = 0;
		   if (prFeederInfo->eRequestType == FEEDER_OFFSET_SEEK)
		   {
	            rData.u8FilePos += (UINT64)u4WriteSize;
		   }
		   else
		   {
		   }
	       rData.eFeederIBC = FEEDER_IBC_NONE;
	       _FeederNotifyHandle(eDataType, FEEDER_NOTIFY_DATA_REQ_DONE, (UINT32)&rData);
        }
        else
        {
            if (prFeederInfo->eRequestType == FEEDER_OFFSET_SEEK 
				&& _rFeederUriData[eDataType].u4PreLoadSize)  // use the pre-loaded data directly
            {
                rData.u4WriteAddr = pQryBuff->u4StartAddr + rData.u8FilePos;  // return the address of the piece of pre-loaded data directly
                rData.u8FilePos += u4WriteSize;  // emulate the data that will be passed by BAGT
                rData.fgPartial = 0;
                rData.fgEof = 0;
                rData.eFeederIBC = FEEDER_IBC_NONE;
                _FeederNotifyHandle(eDataType, FEEDER_NOTIFY_DATA_REQ_DONE, (UINT32)&rData);
            }
            UNUSED(u4WriteSize);
        }
    }
}

    
INT32 _FeederTransmitHandler(FeederSourceType eSrcType, UINT32 u4QrySize, UINT64 u8Offset,
                             FEEDER_TRANSMIT_COND *pResult, UINT32 u4ReqId)
{
    UINT32 u4ReadSize;
    FEEDER_BUF_INFO_T *pQryBuff = &pgrFeederInfo[eSrcType]->rFeedBuf;
    FEEDER_INFO_T *prFeederInfo = pgrFeederInfo[eSrcType];
    UINT32 u4PreAlignOffset = 0;
    UINT64 u8TempRemainder;
    BOOL fgIoOverFlow = FALSE;
#ifdef FEEDER_TIME_PROFILE
	u4ReadSize=pQryBuff->u4WriteAddr>pQryBuff->u4ReadAddr ? pQryBuff->u4WriteAddr-pQryBuff->u4ReadAddr : pQryBuff->u4WriteAddr+pQryBuff->u4Size-pQryBuff->u4ReadAddr;
    LOG(4,"%d(%d) %s u4RemainSize=0x%x,u4QrySize=0x%x\n",eSrcType,u4ReqId,u4ReadSize<u4QrySize ? "WARNING!" : "",u4ReadSize,u4QrySize);
    if(eSrcType==_feeder_time_profile_type)
    {
	   _rDmxStartArray[_u4DmxTimeIndex].u4ReqID=u4ReqId;
	   _rDmxStartArray[_u4DmxTimeIndex].feederSrcType=eSrcType;
	   HAL_GetTime(&(_rDmxStartArray[_u4DmxTimeIndex].rDmxStart));
	   _u4DmxTimeIndex++;
	   if(_u4DmxTimeIndex==FEEDER_TIME_PROFILE_ARRARY_SIZE)
	     _u4DmxTimeIndex=0;
    }
	
#endif    
    if (pResult == NULL)
    {
        LOG(7, "_FeederTransmitHandler : NULL pResult pointer \n");
        return (FEEDER_E_OK);
    }

    if (!pgrFeederInfo[eSrcType] || pgrFeederInfo[eSrcType]->rTransmitParam.pfnTransHandle == NULL)
    {
        pResult->i4TransmitResult = (INT32)FEEDER_TRANSMIT_FAIL;
        pResult->u4BufAddr = (UINT32)NULL;
        pResult->u4TransmitSize = 0;
		LOG(3,"Feeder has stoped,Request fail\n");
        return (FEEDER_E_STOP);
    }

    if (_gfgUseMiddleware[eSrcType] && prFeederInfo->eRequestType == FEEDER_OFFSET_SEEK)
    {
        UINT32 u4BlockSize=_u4BlockSize[eSrcType];
        u8Div6432(u8Offset, (UINT64)u4BlockSize, &u8TempRemainder);
        u4PreAlignOffset = (UINT32)u8TempRemainder;
        u8Offset -= u4PreAlignOffset;
        u4QrySize += u4PreAlignOffset;
        u4QrySize = ((u4QrySize + u4BlockSize-1)/u4BlockSize) *u4BlockSize;
    }
    UNUSED(u8TempRemainder);

    if (prFeederInfo->eApplicationMode == FEEDER_PULL_MODE)
    {
        FEEDER_QUERY_CALLBACK pfnQuery;
        FEEDER_OVERFLOW_CALLBACK pfnOverflow;
        FEEDER_UNDERFLOW_CALLBACK pfnUnderflow;
        UINT32 u4WriteSize;

        pfnQuery = pgrFeederInfo[eSrcType]->rDataQuery.pfnSource;
        pfnOverflow = pgrFeederInfo[eSrcType]->rDataQuery.pfnOverflow;
        pfnUnderflow = pgrFeederInfo[eSrcType]->rDataQuery.pfnUnderflow;
        if (pQryBuff->fgRingBuf)
        {
            FEEDER_REQ_DATA_T rData;
            SIZE_T zMsgSize = sizeof(FEEDER_REQ_DATA_T);
            UINT32 u4NewWriteAddr;
            INT32 i4Ret;	
			if(prFeederInfo->eRequestType == FEEDER_OFFSET_SEEK)
    		{

                // check pre-load status for URI mode
                if (_rFeederUriData[eSrcType].eState == FEEDER_URI_PRE_LOADING)
                {
                    pResult->i4TransmitResult = FEEDER_TRANSMIT_BUFFER_FULL;
                    pResult->u4BufAddr = 0;  //pQryBuff->u4ReadAddr;
                    pResult->u4TransmitSize = 0;
                    LOG(5, "_FeederTransmitHandler : Waiting for pre-loading done\n");
                    return (FEEDER_E_BUFFER_FULL);
                }

                if (_rFeederUriData[eSrcType].u4PreLoadSize >= u8Offset + u4QrySize)  // use the pre-loaded data directly
                {
                }
                else
                {
                    _rFeederUriData[eSrcType].u4PreLoadSize = 0;  // discard the pre-loaded data since it is out of range
                }

                // check buffer status for URI mode
                if (_rFeederUriData[eSrcType].eState == FEEDER_URI_NORMAL &&
                    pQryBuff->u4FreeSize - u4QrySize < _rFeederUriData[eSrcType].u4KeepBufThres)
                {
                    _rFeederUriData[eSrcType].eState = FEEDER_URI_PENDING_KEEP_BUF;
                    
                    if (pfnOverflow)
                    {
                        pfnOverflow(eSrcType);  // notify to upper layer
                    }
                    
                    pResult->i4TransmitResult = FEEDER_TRANSMIT_BUFFER_FULL;
                    pResult->u4BufAddr = pQryBuff->u4ReadAddr;
                    pResult->u4TransmitSize = 0;
                    LOG(5,"Feeder Buffer is nearly keep buffer threshold\n");
                    return (FEEDER_E_BUFFER_FULL);
                }
                if (_rFeederUriData[eSrcType].eState == FEEDER_URI_PENDING_KEEP_BUF)  // ToDo: move to _FeederUpdateReadPointer?
                {
                    ASSERT((pQryBuff->u4BytesInBuf & 0xF0000000) == 0);
                    if (pQryBuff->u4BytesInBuf > _rFeederUriData[eSrcType].u4RebufThres)
                    {
                        pResult->i4TransmitResult = FEEDER_TRANSMIT_BUFFER_FULL;
                        pResult->u4BufAddr = pQryBuff->u4ReadAddr;
                        pResult->u4TransmitSize = 0;
                        LOG(5,"Feeder Buffer is nearly keep buffer threshold\n");
                        return (FEEDER_E_BUFFER_FULL);
                    }
                    else
                    {
                        if (pfnUnderflow)
                        {
                            pfnUnderflow(eSrcType);  // notify to upper layer
                        }
                        
                        _rFeederUriData[eSrcType].eState = FEEDER_URI_NORMAL;
                    }
                }
    		}
            //next addr
            u4NewWriteAddr = ((pQryBuff->u4WriteAddr + u4QrySize) < pQryBuff->u4EndAddr)?
                             (pQryBuff->u4WriteAddr + u4QrySize) :
                             ((pQryBuff->u4WriteAddr + u4QrySize) - pQryBuff->u4Size);
            //protect unremoving data
            if ((u4NewWriteAddr >= pQryBuff->u4ReadAddr && pQryBuff->u4WriteAddr <  pQryBuff->u4ReadAddr) ||
            	(u4NewWriteAddr <  pQryBuff->u4ReadAddr && pQryBuff->u4WriteAddr <  pQryBuff->u4ReadAddr && u4NewWriteAddr < pQryBuff->u4WriteAddr) ||
            	(u4NewWriteAddr >= pQryBuff->u4ReadAddr && pQryBuff->u4WriteAddr >= pQryBuff->u4ReadAddr && u4NewWriteAddr < pQryBuff->u4WriteAddr))
            {
                pResult->i4TransmitResult = FEEDER_TRANSMIT_BUFFER_FULL;
                pResult->u4BufAddr = pQryBuff->u4ReadAddr;
                pResult->u4TransmitSize = 0;
				LOG(5,"Feeder Buffer is nearly full\n");
                return (FEEDER_E_BUFFER_FULL);
            }
            
            rData.eDataType = eSrcType;
			rData.eFeederReqType = prFeederInfo->eRequestType;
            rData.u4Id = u4ReqId;
            if(_gfgUseMiddleware[eSrcType] && (pQryBuff->u4WriteAddr + u4QrySize) > pQryBuff->u4EndAddr)     //must divide into 2 part for bagt
            {
                LOG(3,"wa + sz = 0x%x > ea = 0x%x\n",pQryBuff->u4WriteAddr + u4QrySize,pQryBuff->u4EndAddr);
	            rData.u8FilePos	= u8Offset;
				rData.u4ReadSize = pQryBuff->u4EndAddr - pQryBuff->u4WriteAddr; 			
                rData.u4AlignOffset = u4PreAlignOffset;
			    rData.u4WriteAddr = pQryBuff->u4WriteAddr;
			    rData.fgPartial = TRUE;
                i4Ret = SendFeederDataQMsg(eSrcType,_hFeederDataQ[eSrcType], &rData, zMsgSize, 255);
    	        if (i4Ret == OSR_TOO_MANY)
	            {
                    LOG(1,"!!! Feeder Q overflow 604 !!!\n");
                    fgIoOverFlow = TRUE;
                }
				
                VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));
                if(fgIoOverFlow==FALSE)
                {
                	if(prFeederInfo->eRequestType == FEEDER_OFFSET_SEEK)
                	{
					    rData.u8FilePos = u8Offset + rData.u4ReadSize;
                	}
					else
					{
						rData.u8FilePos = (UINT64)DLNA_TIME_SEEK_INVALID_TIME;
					}
					rData.u4ReadSize = u4QrySize - rData.u4ReadSize;				
					rData.u4AlignOffset = 0;
					rData.u4WriteAddr = pQryBuff->u4StartAddr;
					rData.fgPartial = FALSE;
					if (_gfgUseMiddleware[eSrcType] && prFeederInfo->eRequestType == FEEDER_OFFSET_SEEK)		
				    {				        
					    UINT32 u4BlockSize = _u4BlockSize[eSrcType];	
					    UINT32 u4PreAlignOffset1;				
					    UINT64 u8TempRemainder1;				     
					    u8Div6432(rData.u8FilePos, (UINT64)u4BlockSize, &u8TempRemainder1);	
					    u4PreAlignOffset1 = (UINT32)u8TempRemainder1;
						if (u4PreAlignOffset1 <= pQryBuff->u4PhysicalAddrOffset)
						{
							rData.u8FilePos  -= u4PreAlignOffset1;
					    	rData.u4ReadSize += u4PreAlignOffset1;		
					    	rData.u4AlignOffset = u4PreAlignOffset1;
					    	rData.u4WriteAddr  -= u4PreAlignOffset1;
							rData.u4ReadSize = ((rData.u4ReadSize + u4BlockSize-1)/u4BlockSize) *u4BlockSize;
						}
				    }
					i4Ret = SendFeederDataQMsg(eSrcType,_hFeederDataQ[eSrcType], &rData, zMsgSize, 255);
					if (i4Ret == OSR_TOO_MANY)
					{
						fgIoOverFlow = TRUE;
					}
					VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));
                }

            }
            else
            {
                rData.u8FilePos	= u8Offset;
                rData.u4ReadSize = u4QrySize;	        	        
                rData.u4AlignOffset = u4PreAlignOffset;
                rData.u4WriteAddr = pQryBuff->u4WriteAddr;
                rData.fgPartial = FALSE;
#ifdef CC_FLASH_TEST
                rData.eMode = pResult->eMode;
                rData.fgFirst = pResult->fgFirst;
#endif          
	            i4Ret = SendFeederDataQMsg(eSrcType,_hFeederDataQ[eSrcType], &rData, zMsgSize, 255);
	            
                LOG(5,"feedertran,ret = %d,eSrcType = %d,wa = 0x%x,sa = 0x%x\n",i4Ret,eSrcType,pQryBuff->u4WriteAddr,pQryBuff->u4StartAddr);
            }
            if (i4Ret == OSR_TOO_MANY)
            {
                //u4WriteSize = 0;
                LOG(1,"!!! Feeder Q overflow 645 !!!\n");
                //ASSERT(0);
                fgIoOverFlow = TRUE;
            }
            VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));
            u4WriteSize = u4QrySize;
        }
        else
        {
            FEEDER_REQ_DATA_T rReqData;
            rReqData.u8FilePos = u8Offset;
            rReqData.u4ReadSize = u4QrySize;
			rReqData.eFeederReqType = prFeederInfo->eRequestType;
            if(_gfgUseMiddleware[eSrcType])
            {
                rReqData.u4WriteAddr = pQryBuff->u4WriteAddr;
#ifdef FEEDER_MULTI_INSTANCE
                u4WriteSize = (UINT32)pfnQuery(prFeederInfo->rDataQuery.u1PlaymgrAttachedId,  eSrcType, pQryBuff, &rReqData);
#else
                u4WriteSize = (UINT32)pfnQuery( eSrcType, pQryBuff, &rReqData);
#endif
            }
            else
            {
#ifndef __KERNEL__  // tmp solution, check with Jun later
                rReqData.u4WriteAddr = pQryBuff->u4WriteAddr;
#ifdef FEEDER_MULTI_INSTANCE
                u4WriteSize = (UINT32)pfnQuery(prFeederInfo->rDataQuery.u1PlaymgrAttachedId,  eSrcType, pQryBuff, &rReqData);
#else
                u4WriteSize = (UINT32)pfnQuery( eSrcType, pQryBuff, &rReqData);
#endif
#else
                rReqData.u4WriteAddr = pQryBuff->u4WriteAddr + FEEDER_DEFAULT_BUF_SIZE;
#ifdef FEEDER_MULTI_INSTANCE
                u4WriteSize = (UINT32)pfnQuery(prFeederInfo->rDataQuery.u1PlaymgrAttachedId,  eSrcType, pQryBuff, &rReqData);
#else
                u4WriteSize = (UINT32)pfnQuery( eSrcType, pQryBuff, &rReqData);
#endif
                if(rReqData.u4WriteAddr==0) // error 
                {
                   LOG(0,"_feederRransmitHandler get error\n");
                   return FEEDER_E_FAIL;
                }
#endif
            }
        }

        if (fgIoOverFlow)
        {
            u4WriteSize = 0;
        }

        pQryBuff->u4FreeSize   -= u4WriteSize;
        pQryBuff->u4BytesInBuf += u4WriteSize;
        if (pQryBuff->fgRingBuf)                            //ring buf, update wptr then return
        {
            if (prFeederInfo->eRequestType == FEEDER_OFFSET_SEEK 
				&& _rFeederUriData[eSrcType].u4PreLoadSize)  // set related pointers to the proper position if using pre-loaded data
            {
                pQryBuff->u4ReadAddr = pQryBuff->u4StartAddr + u8Offset;
                pQryBuff->u4WriteAddr = pQryBuff->u4ReadAddr + u4QrySize;
            }
            else
            {
                pQryBuff->u4WriteAddr = 
                ((pQryBuff->u4WriteAddr + u4WriteSize) < pQryBuff->u4EndAddr)?
                (pQryBuff->u4WriteAddr + u4WriteSize) :
                ((pQryBuff->u4WriteAddr + u4WriteSize) - pQryBuff->u4Size);
            }
      
            pResult->i4TransmitResult = FEEDER_TRANSMIT_OK;
            pResult->u4BufAddr = pQryBuff->u4ReadAddr;
            pResult->u4TransmitSize = u4WriteSize;
            return (FEEDER_E_OK);
        }
    }
    
    
    u4ReadSize = pQryBuff->u4LastReadSize;
    // if last write address is the same as the current write address
    // -> this is no data resumption in the last data request
    //pgrFeederInfo[eSrcType]->rCritState = x_crit_start();
    pQryBuff->u4LastReadAddr = pQryBuff->u4ReadAddr;
    if (pQryBuff->fgRingBuf)
    {
        pQryBuff->u4ReadAddr += pQryBuff->u4LastReadSize;
    }
    if (pQryBuff->u4ReadAddr == pQryBuff->u4EndAddr)
    {
        pQryBuff->u4ReadAddr = pQryBuff->u4StartAddr;
    }
    pQryBuff->u4BytesInBuf -= pQryBuff->u4LastReadSize;
    pQryBuff->u4FreeSize += pQryBuff->u4LastReadSize;
    pQryBuff->u4LastReadSize = 0;
    //x_crit_end(pgrFeederInfo[eSrcType]->rCritState);
    
    // Signal the buffer empty event
    if ((pQryBuff->u4BytesInBuf == 0) && 
        (pgrFeederInfo[eSrcType]->bWaitBufferEmpty) &&
        (pgrFeederInfo[eSrcType]->hDataEmptySema))
    {
        VERIFY(OSR_OK == x_sema_unlock(pgrFeederInfo[eSrcType]->hDataEmptySema));
        LOG(7, "_FeederTransmitHandler : Buffer empty \n");
    }
    
    // Check if the feeder is paused or its buffer is empty
    if ((geFeederState[eSrcType] == FEEDER_PAUSE) ||
        (pQryBuff->u4BytesInBuf == 0))
    {
        pResult->i4TransmitResult = (INT32)FEEDER_TRANSMIT_BUFFER_EMPTY;
        pResult->u4BufAddr = (UINT32)NULL;
        pResult->u4TransmitSize = 0;
        return (FEEDER_E_OK);
    }
    
    // Check the valid write size
    if (_eFeederSrcType[eSrcType] == FEEDER_SRC_TYPE_NETWORK_NETFLIX)
    {
        u4ReadSize = pQryBuff->u4BytesInBuf;
    }
    else
    {
        u4ReadSize = (u4QrySize > pQryBuff->u4BytesInBuf) ?
                  pQryBuff->u4BytesInBuf : u4QrySize;
    }
    if ((pQryBuff->u4EndAddr - pQryBuff->u4ReadAddr) < u4ReadSize)
    {
        u4ReadSize = pQryBuff->u4EndAddr - pQryBuff->u4ReadAddr;
    }
 
    
    // if last write address is the same as the current write address
    // -> this is the frist data qeury, and do not need to update write address
    //pgrFeederInfo[eSrcType]->rCritState = x_crit_start();
    pQryBuff->u4LastReadSize = u4ReadSize;
    pgrFeederInfo[eSrcType]->u4TotalTransmitBytes += u4ReadSize;
    //x_crit_end(pgrFeederInfo[eSrcType]->rCritState);

    // Return the transmit result
    pResult->i4TransmitResult = (pQryBuff->u4FreeSize == 0) ? 
                                (INT32)FEEDER_TRANSMIT_BUFFER_FULL :
                                (INT32)FEEDER_TRANSMIT_OK;
    pResult->u4BufAddr = pQryBuff->u4ReadAddr;
    pResult->u4TransmitSize = u4ReadSize;

    // Set data consumption event
    if ((pgrFeederInfo[eSrcType]->hGetDataSema) &&
        (pgrFeederInfo[eSrcType]->bWaitFreeSpace))
    {
        VERIFY(OSR_OK == x_sema_unlock(pgrFeederInfo[eSrcType]->hGetDataSema));
    }

    return (FEEDER_E_OK);
}

#if 0
VOID _FeederWatchTimeout(HANDLE_T hTimerHandle, VOID *pvTag)
{
    FeederSourceType eSrcType;

    eSrcType = (FeederSourceType)(UINT32)pvTag;
    
    // Release the sema lock of data empty
    if (pgrFeederInfo[eSrcType]->hDataEmptySema)
    {
        VERIFY(OSR_OK == x_sema_unlock(pgrFeederInfo[eSrcType]->hDataEmptySema));
        LOG(7, "_FeederWatchTimeout : Buffer empty \n");
    }
    
    UNUSED(hTimerHandle);
}
#endif
//-----------------------------------------------------------------------------
/** Wait the event if there is more buffer space free
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 *  @retval FEEDER_E_STOP   Feeder stops
 */
//-----------------------------------------------------------------------------
#if 0
INT32 _FeederWaitFreeBufferSpace(FeederSourceType eDataType)
{
    INT32 i4Ret;

    if (geFeederState[eDataType] == FEEDER_IDLE)
    {
        return (FEEDER_E_OK);
    }
    
    if (pgrFeederInfo[eDataType]->hGetDataSema == (HANDLE_T)NULL)
    {
        // Create a Sync lock to control data query task
        i4Ret = x_sema_create(&pgrFeederInfo[eDataType]->hGetDataSema,
                              X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
        
        if (i4Ret != OSR_OK)
        {
            LOG(5, "_FeederWaitFreeBufferSpace: Create the data query sema failed \n");
            return (FEEDER_E_FAIL);
        }
    }

    // Enable the wait free buffer sema    
    //pgrFeederInfo[eDataType]->rCritState = x_crit_start();
    pgrFeederInfo[eDataType]->bWaitFreeSpace = TRUE;
    //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
    
    // Wait data consumption of the full buffer
    i4Ret = x_sema_lock(pgrFeederInfo[eDataType]->hGetDataSema,
                        X_SEMA_OPTION_WAIT);
    
    // Disable the wait free buffer sema    
    //pgrFeederInfo[eDataType]->rCritState = x_crit_start();
    pgrFeederInfo[eDataType]->bWaitFreeSpace = FALSE;
    //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
    
    if ((geFeederState[eDataType] != FEEDER_PAUSE) && 
        (geFeederState[eDataType] != FEEDER_START))
    {
        LOG(5, "_FeederWaitFreeBufferSpace : Feeder stops ...\n");
        return (FEEDER_E_STOP);
    }
    
    return ((i4Ret == OSR_OK) ? FEEDER_E_OK : FEEDER_E_FAIL);
}
#endif
//-----------------------------------------------------------------------------
/** Wait the event if the buffer is empty
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 *  @retval FEEDER_E_STOP   Feeder stops
 */
//-----------------------------------------------------------------------------
#if 0
INT32 _FeederWaitBufferEmpty(FeederSourceType eDataType)
{
    INT32 i4Ret;

    if (geFeederState[eDataType] == FEEDER_IDLE)
    {
        return (FEEDER_E_OK);
    }
    
    if (pgrFeederInfo[eDataType]->hDataEmptySema == (HANDLE_T)NULL)
    {
        // Create a Sync lock to wake up data empty
        i4Ret = x_sema_create(&pgrFeederInfo[eDataType]->hDataEmptySema,
                              X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
        if (i4Ret != OSR_OK)
        {
            LOG(5, "_FeederWaitBufferEmpty: Create the data empty sema failed \n");
            return (FEEDER_E_FAIL);
        }
    }

#ifndef __KERNEL__
    VERIFY(x_timer_start(pgrFeederInfo[eDataType]->hWatchTimer, 30000,
                         X_TIMER_FLAG_ONCE, _FeederWatchTimeout,
                         (void*)(UINT32)eDataType) == OSR_OK);
#endif

    // Make sure the buffer-empty sema is in the lock state
    //pgrFeederInfo[eDataType]->rCritState = x_crit_start();
    pgrFeederInfo[eDataType]->bWaitBufferEmpty = TRUE;
    //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
    
    LOG(9, "_FeederWaitBufferEmpty : Lock sema\n");
    
    // Wait the empty buffer
    i4Ret = x_sema_lock(pgrFeederInfo[eDataType]->hDataEmptySema,
                        X_SEMA_OPTION_WAIT);
    
    LOG(9, "_FeederWaitBufferEmpty : unlock sema\n");

    VERIFY(x_timer_stop(pgrFeederInfo[eDataType]->hWatchTimer) == OSR_OK);
    
    return ((i4Ret == OSR_OK) ? FEEDER_E_OK : FEEDER_E_FAIL);
}
#endif
//-----------------------------------------------------------------------------
/** Allot the receiver-related hardware resources
 *   Init Feeder Param when system bootup
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
#if 0
INT32 _FeederInit()
{
   int i;
   x_memset(pgrFeederInfo,0,sizeof(FEEDER_INFO_T *)*FEEDER_SOURCE_INVALID);
   x_memset(geFeederState,0,sizeof(FeederState)*FEEDER_SOURCE_INVALID);
   //x_memset(garTransmitFuncTbl,_FeederSetRequestInfo,sizeof(FEEDER_SET_TRANSMIT)*FEEDER_SOURCE_INVALID);
   garTransmitFuncTbl[0]=AUD_FeederSetRequestInfo;
   
   for(i=0;i<FEEDER_SOURCE_INVALID;i++)
   {
        _u4BlockSize[i]=512;
   }
   
   //x_memset(_u4BlockSize,512,sizeof(UINT32)*FEEDER_SOURCE_INVALID);
   
#if 0 //def FEEDER_MULTI_INSTANCE
   x_memset(_fgMsgQFlashId,0,sizeof(BOOL)*FEEDER_SOURCE_INVALID);   
   x_memset(_u4LastFlashedDataId,0,sizeof(UINT32)*FEEDER_SOURCE_INVALID);
   x_memset(_u4LastReceiveDataId,0,sizeof(UINT32)*FEEDER_SOURCE_INVALID);
   x_memset(u4LastRquDataId,0,sizeof(UINT32)*FEEDER_SOURCE_INVALID);
#endif
   x_memset(_eFeederSrcType,FEEDER_SRC_TYPE_HIGH_SPEED_STORAGE,sizeof(FeederSourceType_T)*FEEDER_SOURCE_INVALID);

   x_memset(garNotifyTbl,0,sizeof(FEEDER_CALBACK)*FEEDER_SOURCE_TOTAL*FEEDER_NOTIFY_MAX);


   return FEEDER_E_OK;
}
#endif

#if defined (FEEDER_SUPPORT_FBM_ALLOC) && defined(CC_FBM_FOUR_INST_SUPPORT)

static FBM_POOL_TYPE_T _FeederFromDeviceToFbmType(UINT32  u4FeederDeviceId)
{
    FBM_POOL_TYPE_T eFbmType = FBM_POOL_TYPE_NONE;
    switch(u4FeederDeviceId)
    {
        case FEEDER_FBM_FEEDER:
            eFbmType = FBM_POOL_TYPE_FEEDER;
        break;
        case FEEDER_FBM_FEEDER2:
            eFbmType = FBM_POOL_TYPE_FEEDER2;
        break;
        case FEEDER_FBM_FEEDER3:
            eFbmType = FBM_POOL_TYPE_FEEDER3;
        break;
        case FEEDER_FBM_FEEDER4:
            eFbmType = FBM_POOL_TYPE_FEEDER4;
        break;
        case FEEDER_FBM_MMP:
            eFbmType = FBM_POOL_TYPE_FEEDER_MMP;
        break;
        default:
        break;
    }
    return eFbmType;
    
}

static FBM_POOL_T* _FeederFromDeviceToFbm(UINT32  u4FeederDeviceId)
{
    FBM_POOL_T* pFbmPool = NULL;
    FBM_POOL_TYPE_T eFbmType = FBM_POOL_TYPE_NONE;
    
    eFbmType = _FeederFromDeviceToFbmType(u4FeederDeviceId);
    if(eFbmType != FBM_POOL_TYPE_NONE)
    {
        pFbmPool = FBM_GetPoolInfo(eFbmType);
    }
    return pFbmPool;
}


static FBM_POOL_T* _FeederGetFbmBuffer(FeederSourceType eDataType)
{
    FBM_POOL_T* pFbmPool = NULL;
    FEEDER_INFO_T *prFeederInfo = NULL;
    
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        return NULL;
    }
    if(FeederGetSkypeSourceFlag()==TRUE)
    {
        #ifdef FEEDER_FIVE_INSTANCE
        switch(eDataType)
        {
        case FEEDER_AUDIO_SOURCE:
            prFeederInfo = pgrFeederInfo[FEEDER_AUDIO_SOURCE];
            if(prFeederInfo != NULL)
            {
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            break;
        case FEEDER_VIDEO_SOURCE:
        case FEEDER_PROGRAM_SOURCE:
        case FEEDER_AUDIO_SOURCE_3:
            if(pgrFeederInfo[FEEDER_VIDEO_SOURCE] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_VIDEO_SOURCE];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            else if(pgrFeederInfo[FEEDER_PROGRAM_SOURCE] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_PROGRAM_SOURCE];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            else if(pgrFeederInfo[FEEDER_AUDIO_SOURCE_3] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_AUDIO_SOURCE_3];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            break;
        case FEEDER_PROGRAM_SOURCE_2:
        case FEEDER_SOURCE_3:
            if(pgrFeederInfo[FEEDER_PROGRAM_SOURCE_2] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_PROGRAM_SOURCE_2];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            else if(pgrFeederInfo[FEEDER_SOURCE_3] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_SOURCE_3];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            break;
        case FEEDER_AUDIO_SOURCE_2:
        case FEEDER_SOURCE_4:
            if(pgrFeederInfo[FEEDER_AUDIO_SOURCE_2] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_AUDIO_SOURCE_2];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            else if(pgrFeederInfo[FEEDER_SOURCE_4] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_SOURCE_4];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            break;
        default:
            break;
        }
        #endif
    }
    else
    {
        switch(eDataType)
        {
        case FEEDER_VIDEO_SOURCE:
        case FEEDER_PROGRAM_SOURCE:
        case FEEDER_PARSING_SOURCE:
            if(pgrFeederInfo[FEEDER_VIDEO_SOURCE] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_VIDEO_SOURCE];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            else if(pgrFeederInfo[FEEDER_PROGRAM_SOURCE] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_PROGRAM_SOURCE];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            else if(pgrFeederInfo[FEEDER_PARSING_SOURCE] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_PARSING_SOURCE];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            break;
        case FEEDER_AUDIO_SOURCE:
        case FEEDER_PARSING_AUDIO:
            if(pgrFeederInfo[FEEDER_AUDIO_SOURCE] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_AUDIO_SOURCE];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            else if(pgrFeederInfo[FEEDER_PARSING_AUDIO] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_PARSING_AUDIO];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            break;
        case FEEDER_PARSING_SOURCE_3:
        case FEEDER_SOURCE_3:
            if(pgrFeederInfo[FEEDER_PARSING_SOURCE_3] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_PARSING_SOURCE_3];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            else if(pgrFeederInfo[FEEDER_SOURCE_3] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_SOURCE_3];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            break;
        case FEEDER_PARSING_SOURCE_4:
        case FEEDER_SOURCE_4:
            if(pgrFeederInfo[FEEDER_PARSING_SOURCE_4] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_PARSING_SOURCE_4];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            else if(pgrFeederInfo[FEEDER_SOURCE_4] != NULL)
            {
                prFeederInfo = pgrFeederInfo[FEEDER_SOURCE_4];
                pFbmPool = _FeederFromDeviceToFbm(prFeederInfo->rFeedBuf.FeederDeviceId);
            }
            break;
        default:
            break;
        }
    }
    return pFbmPool;
}
#endif

//-----------------------------------------------------------------------------
/** Allot the receiver-related hardware resources
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederOpen(FeederSourceType eDataType)
{
    INT32 i4Ret,iFbm;
    UINT32 u4BuffAddr,u4BufOffset=0;
    UINT32 u4BuffSize, u4MaxDftBuffSize;
    FEEDER_INFO_T *prFeederInfo;
    UINT32 u4RealBufStart = 0;
    FBM_POOL_T* pFbmPool = NULL;

    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "_FeederOpen : Invalid source type\n");
        return (FEEDER_E_INVALID_PARAM);
    }

    if (geFeederState[eDataType] != FEEDER_IDLE)
    {
        LOG(7, "_FeederOpen: Feeder driver init is done\n");
        return (FEEDER_E_OK);
    }

    for (i4Ret = 0; i4Ret < (INT32)FEEDER_NOTIFY_MAX; i4Ret++)
    {
        garNotifyTbl[eDataType][i4Ret] = NULL;
    }   
    
    prFeederInfo = x_mem_alloc(sizeof(FEEDER_INFO_T));
    if (prFeederInfo == NULL)
    {
        LOG(0, "_FeederOpen: Memory allocation failed (pgrFeederInfo)\n ");
        return (FEEDER_E_MEMORY_ALLOC);
    }
    
    x_memset(prFeederInfo, 0, sizeof(FEEDER_INFO_T));
    x_memset(&_rFeederUriData[eDataType], 0, sizeof(FEEDER_URI_DATA_T));
	
#if defined (FEEDER_SUPPORT_FBM_ALLOC) && defined(CC_FBM_FOUR_INST_SUPPORT) 
    pFbmPool = _FeederGetFbmBuffer(eDataType);
    if(pFbmPool != NULL)
    {
        LOG(0,"FBM buffer already allocated\n");
    }
    else
    {
        pFbmPool = FBM_Alloc(FBM_TYPE_FEEDER);
        if(pFbmPool == NULL)
        {
            LOG(0,"FBM_Alloc fail !!!\n");
            x_mem_free(prFeederInfo);
            return FEEDER_E_MEMORY_ALLOC;
        }
    }
    switch(pFbmPool->ePoolType)
    {
        case FBM_POOL_TYPE_FEEDER:
            iFbm = FEEDER_FBM_FEEDER;
            LOG(0,"use FEEDER_FBM_FEEDER!!!\n");
        break;
        case FBM_POOL_TYPE_FEEDER2:
            iFbm = FEEDER_FBM_FEEDER2;
            LOG(0,"use FEEDER_FBM_FEEDER2!!!\n");
        break;
        case FBM_POOL_TYPE_FEEDER3:
            iFbm = FEEDER_FBM_FEEDER3;
            LOG(0,"use FEEDER_FBM_FEEDER3!!!\n");
        break;
        case FBM_POOL_TYPE_FEEDER4:
            iFbm = FEEDER_FBM_FEEDER4;
            LOG(0,"use FEEDER_FBM_FEEDER4!!!\n");
        break;
        case  FBM_POOL_TYPE_FEEDER_MMP:
            LOG(0,"use FBM_POOL_TYPE_FEEDER_MMP!!!\n");
            iFbm = FEEDER_FBM_MMP;
        break;
        default:
            LOG(0,"FBM_Alloc PoolType %d invalid !!!\n",pFbmPool->ePoolType);
            FBM_Free(pFbmPool);
            x_mem_free(prFeederInfo);
            return FEEDER_E_MEMORY_ALLOC;
        break;
    }

#else
#if defined (CC_VOMX_TV_COEXIST) || defined(CC_FBM_TWO_FBP) || defined (CC_FBM_FOUR_INST_SUPPORT)
    if(eDataType == FEEDER_AUDIO_SOURCE || eDataType == FEEDER_PARSING_AUDIO)
    {
        iFbm = FEEDER_FBM_FEEDER2;
		pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER2);
    }
    else
#endif
#ifdef CC_FBM_FOUR_INST_SUPPORT
    if(eDataType == FEEDER_SOURCE_3 || eDataType == FEEDER_PARSING_SOURCE_3
        #ifdef FEEDER_FIVE_INSTANCE
        || (FeederGetSkypeSourceFlag()==TRUE && eDataType == FEEDER_PROGRAM_SOURCE_2)
        #endif
        )
    {
        iFbm = FEEDER_FBM_FEEDER3;
        pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER3);
    }
    else
    if(eDataType == FEEDER_SOURCE_4 || eDataType == FEEDER_PARSING_SOURCE_4
        #ifdef FEEDER_FIVE_INSTANCE
        || (FeederGetSkypeSourceFlag()==TRUE && eDataType == FEEDER_AUDIO_SOURCE_2)
        #endif
        )
    {
        iFbm = FEEDER_FBM_FEEDER4;
        pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER4);
    }
    else
#endif
    { 
       iFbm = FEEDER_FBM_FEEDER;
       pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER);
    }
#endif

    prFeederInfo->rFeedBuf.FeederDeviceId  = iFbm;
    //prFeederInfo->rFeedBuf.u4FeederMemAddr = pFbmPool->u4Addr;
    //dynamic config max default buf size
    //0x500000 is the threhold 
    //to be fixed
    if(pFbmPool->u4Size > 0x500000)
    {
        u4MaxDftBuffSize = (12288*1024*FEEDER_BUF_SIZE_EX);
    }
    else
    {
        u4MaxDftBuffSize = (2256*1024*FEEDER_BUF_SIZE_EX);
    }
    

#ifdef __KERNEL__        
    u4RealBufStart = pFbmPool->u4Addr & PAGE_MASK;
#else
    u4RealBufStart = (pFbmPool->u4Addr + 31) & (~31);
#endif


	if(FeederGetSkypeSourceFlag()==TRUE)
	{
#ifndef	CC_FBM_FOUR_INST_SUPPORT     
	#ifdef FEEDER_FIVE_INSTANCE
		switch(eDataType)
	    {
		   case FEEDER_AUDIO_SOURCE:
			  u4BufOffset=FEEDER_AUDIO_BUF_START_SKYPE;
			  u4BuffSize=FEEDER_AUDIO_BUF_SIZE_SKYPE;
			  break;
		   case FEEDER_VIDEO_SOURCE:
		   case FEEDER_PROGRAM_SOURCE:
			  u4BufOffset=FEEDER_PROGRAM_BUF_START_SKYPE;
#ifdef CC_ONLY_FOR_SH
              u4BuffSize=FEEDER_DEFAULT_BUF_SIZE_SKYPE / SWDMX_QueryDataSize();
#else
              u4BuffSize=FEEDER_DEFAULT_BUF_SIZE_SKYPE;
#endif
			  break;
	
	          case FEEDER_PROGRAM_SOURCE_2:
		   	  u4BufOffset=FEEDER_PROGRAM_SOURCE_2_BUF_START_SKYPE;
			  u4BuffSize=FEEDER_PROGRAM_SOURCE_2_BUF_SIZE_SKYPE;		 
			  break;
	          case FEEDER_AUDIO_SOURCE_2:
		   	  u4BufOffset=FEEDER_AUDIO_BUF_2_START_SKYPE;
			  u4BuffSize=FEEDER_AUDIO_BUF_2_SIZE_SKYPE;
			  break;
	          case FEEDER_AUDIO_SOURCE_3:
		   	  u4BufOffset=FEEDER_AUDIO_BUF_3_START_SKYPE;
			  u4BuffSize=FEEDER_AUDIO_BUF_3_SIZE_SKYPE;
			  break;	  
		   default:
		   	  x_mem_free(prFeederInfo);	
			  return FEEDER_E_INVALID_PARAM;
	    }			  
	#else
		x_mem_free(prFeederInfo);
		return FEEDER_E_INVALID_PARAM;
	#endif		  
#else
	    #ifdef FEEDER_FIVE_INSTANCE
		switch(eDataType)
        {
            case FEEDER_AUDIO_SOURCE:
			    u4BufOffset = 64*1024;
			    u4BuffSize = u4MaxDftBuffSize;
                break;
           case FEEDER_VIDEO_SOURCE:
		   case FEEDER_PROGRAM_SOURCE:
                u4BufOffset = 64*1024;
			    u4BuffSize = u4MaxDftBuffSize - 512*1024;//reserve 512k for audio_source_3
                break;
           case FEEDER_PROGRAM_SOURCE_2:
           case FEEDER_SOURCE_3:
                u4BufOffset = 64*1024;
			    u4BuffSize = u4MaxDftBuffSize;
                break;
            case FEEDER_AUDIO_SOURCE_2:
            case FEEDER_SOURCE_4:
    		   	u4BufOffset = 64*1024;
    			u4BuffSize = u4MaxDftBuffSize;
    			break;
            case FEEDER_AUDIO_SOURCE_3:
		   	    u4BufOffset = 64*1024 + (u4MaxDftBuffSize - 512*1024) + 64*1024;//the same fbm with feeder
			    u4BuffSize = 512*1024;
			    break;	  
           default:
		   	    x_mem_free(prFeederInfo);	
			    return FEEDER_E_INVALID_PARAM;
        }
	    #else
		x_mem_free(prFeederInfo);
        LOG(0, "_FeederOpen: Wrong Wrong data source\n");
		return FEEDER_E_INVALID_PARAM;
	    #endif
#endif

	}
	else
	{		
		switch(eDataType)
	    {
#if defined (CC_FBM_TWO_FBP) || defined(CC_FBM_FOUR_INST_SUPPORT)
            case FEEDER_PROGRAM_SOURCE:
            case FEEDER_VIDEO_SOURCE:
                u4BufOffset = 64*1024;
    			u4BuffSize = u4MaxDftBuffSize;
                break;
            case FEEDER_PARSING_SOURCE:
                u4BufOffset = 64*1024 + u4MaxDftBuffSize + 64*1024;
    			u4BuffSize = 512*1024;
                break;
            case FEEDER_AUDIO_SOURCE:
                u4BufOffset = 64*1024;
    			u4BuffSize = u4MaxDftBuffSize;
                break;
            case FEEDER_PARSING_AUDIO:
                u4BufOffset = 64*1024 + u4MaxDftBuffSize + 64*1024;
    			u4BuffSize = 512*1024;
                break;
            #ifdef CC_FBM_FOUR_INST_SUPPORT
            case FEEDER_SOURCE_3:
                u4BufOffset = 64*1024;
    			u4BuffSize = u4MaxDftBuffSize;
                break;
            case FEEDER_PARSING_SOURCE_3:
                u4BufOffset = 64*1024 + u4MaxDftBuffSize + 64*1024;
    			u4BuffSize = 512*1024;
                break;
            case FEEDER_SOURCE_4:
                u4BufOffset = 64*1024;
    			u4BuffSize = u4MaxDftBuffSize;
                break;
            case FEEDER_PARSING_SOURCE_4:
                u4BufOffset = 64*1024 + u4MaxDftBuffSize + 64*1024;
    			u4BuffSize = 512*1024;
                break;
            #endif
            default:
                LOG(0, "_FeederOpen: Wrong Wrong data source\n");
                x_mem_free(prFeederInfo);			  
			    return FEEDER_E_INVALID_PARAM;
#else
		   case FEEDER_AUDIO_SOURCE:
#ifdef CC_VOMX_TV_COEXIST
              u4BufOffset=FEEDER_PROGRAM_BUF_START;
#else
              u4BufOffset=FEEDER_AUDIO_BUF_START;
#endif

			  u4BuffSize=FEEDER_AUDIO_BUF_SIZE;
			  break;
		   case FEEDER_VIDEO_SOURCE:
		   case FEEDER_PROGRAM_SOURCE:
#ifdef CC_ONLY_FOR_SH
              u4BufOffset=FEEDER_PROGRAM_BUF_START;
              u4BuffSize=u4MaxDftBuffSize / SWDMX_QueryDataSize();
#else
			  u4BufOffset=FEEDER_PROGRAM_BUF_START;
			  u4BuffSize=u4MaxDftBuffSize;
#endif
			  break;
		   case FEEDER_PARSING_SOURCE:
			  u4BufOffset=FEEDER_PARSING_BUF_START;
			  u4BuffSize=FEEDER_PARSING_BUF_SIZE;
			  break;
		   case FEEDER_PARSING_AUDIO:
			  u4BufOffset=FEEDER_PARSING_AUD_BUF_START;
			  u4BuffSize=FEEDER_PARSING_AUD_BUF_SIZE;
			  break;  
		   default:
			  LOG(7, "_FeederOpen: Wrong Wrong data source\n");
		      x_mem_free(prFeederInfo);			  
			  return FEEDER_E_INVALID_PARAM;
#endif
	    }

	}


    LOG(5,"feeder size = 0x%x\n",u4BuffSize);

	//error check.
	if((u4BufOffset+u4BuffSize)>pFbmPool->u4Size)
	{
		LOG(7, "_FeederOpen: Wrong Wrong data source[%x]\n", eDataType);
		ASSERT(0);
        x_mem_free(prFeederInfo);		
		return FEEDER_E_INVALID_PARAM;		  
	}


     u4BuffAddr = VIRTUAL(u4RealBufStart+u4BufOffset);  // Pool Start Address

    if (!_hFeederDataQ[eDataType])
    {
    	char acBuf[32];
        x_memset(acBuf, 0, 32 * sizeof(char));
        x_snprintf(acBuf, sizeof(acBuf), "FEEDER_DataQ_%d", eDataType);
        
        i4Ret = x_msg_q_create(&_hFeederDataQ[eDataType], acBuf, sizeof(FEEDER_REQ_DATA_T), 
                                FEEDER_DATA_Q_SIZE);

        if (i4Ret != OSR_OK)
        {
            LOG(5, "_FeederOpen: Create the message queue failed \n");
	        x_mem_free(prFeederInfo);            
            return (FEEDER_E_FAIL);
        }
    }

  #if FEEDER_MULT_INSTANCE_TYPE == 1
	   if(!_hFeederDummyDataQ)
	   {
	   i4Ret = x_msg_q_create(&_hFeederDummyDataQ, "feederDummyQ", sizeof(INT32), 
							   (5+FEEDER_DATA_Q_SIZE)*FEEDER_SOURCE_INVALID);
	   LOG(0,"feeder create dummy queue,ret=%d\n",i4Ret);
	   ASSERT(i4Ret == OSR_OK);
	   }
  #endif

 
  #if FEEDER_MULT_INSTANCE_TYPE == 2
 
    if (!_hFeederThread[eDataType])
    {
    	char acBuf[32];
        x_memset(acBuf, 0, 32 * sizeof(char));
        x_snprintf(acBuf, sizeof(acBuf), "Feeder_QTask_%d", eDataType);
        // Create data qeury task to retrieve from the upper program
        i4Ret = x_thread_create(&_hFeederThread[eDataType], acBuf,
                                FEEDER_STACK_SIZE, FEEDER_THREAD_PRIORITY,
                                _FeederQeuryDataTask, sizeof(FeederSourceType), (VOID*)&eDataType);
        if (i4Ret != OSR_OK)
        {
            LOG(5, "_FeederOpen: Create the query thread failed \n");
	        x_mem_free(prFeederInfo);            
            return (FEEDER_E_FAIL);
        }
    }    
 #else
     if (!_hFeederThread)
    {
        char acBuf[32];
        x_memset(acBuf, 0, 32* sizeof(char));
        x_snprintf(acBuf, sizeof(acBuf), "Feeder_QTask_%d", eDataType);
        // Create data qeury task to retrieve from the upper program
        i4Ret = x_thread_create(&_hFeederThread, acBuf,
                                FEEDER_STACK_SIZE, FEEDER_THREAD_PRIORITY,
                                _FeederQeuryDataTask, sizeof(FeederSourceType), (VOID*)&eDataType);
        if (i4Ret != OSR_OK)
        {
            LOG(5, "_FeederOpen: Create the query thread failed \n");
	        x_mem_free(prFeederInfo);            
            return (FEEDER_E_FAIL);
        }
    }
 #endif

    pgrFeederInfo[eDataType] = prFeederInfo;
    
    prFeederInfo->bWaitBufferEmpty = FALSE;
    prFeederInfo->bWaitFreeSpace   = FALSE;
    prFeederInfo->bReceiveFileBeginNfy = FALSE;
    prFeederInfo->eApplicationMode = FEEDER_PULL_MODE;
    prFeederInfo->eReceiverMode    = FEEDER_PULL_MODE;
    prFeederInfo->rFeedBuf.u4Size  = u4BuffSize;
	prFeederInfo->eRequestType = FEEDER_OFFSET_SEEK;
    prFeederInfo->rFeedBuf.u4StartAddr = u4BuffAddr;

    LOG(5,"feeeeeeeee,Feeder Open,size = 0x%x,u4BuffSize = 0x%x\n",prFeederInfo->rFeedBuf.u4Size,u4BuffSize);
    prFeederInfo->rFeedBuf.u4EndAddr = 
    prFeederInfo->rFeedBuf.u4StartAddr + prFeederInfo->rFeedBuf.u4Size;
    prFeederInfo->rFeedBuf.u4ReadAddr = prFeederInfo->rFeedBuf.u4StartAddr;
    prFeederInfo->rFeedBuf.u4WriteAddr = prFeederInfo->rFeedBuf.u4StartAddr;
    prFeederInfo->rFeedBuf.u4LastReadAddr = prFeederInfo->rFeedBuf.u4ReadAddr;
    prFeederInfo->rFeedBuf.fgRingBuf = FALSE;
	prFeederInfo->rFeedBuf.u4PhysicalAddr = u4RealBufStart;
	prFeederInfo->rFeedBuf.u4PhysicalAddrOffset = u4BufOffset;

    LOG(5,"feeder sa = 0x%x,ea = 0x%x\n",prFeederInfo->rFeedBuf.u4StartAddr,prFeederInfo->rFeedBuf.u4EndAddr);
    
    _u4DataDelayTime=0;
    geFeederState[eDataType] = FEEDER_INIT;
    _gfgUseMiddleware[eDataType]=FALSE;
    return (FEEDER_E_OK);
}

//-----------------------------------------------------------------------------
/** Release the allotted resources
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederClose(FeederSourceType eDataType)
{
    FEEDER_INFO_T *prFeederInfo = pgrFeederInfo[eDataType];

#if defined (FEEDER_SUPPORT_FBM_ALLOC) && defined(CC_FBM_FOUR_INST_SUPPORT) 
    UINT32 u4FeederDeviceId = 0;
    FBM_POOL_T* pFbmPool = NULL;
#endif

	if(eDataType >= FEEDER_SOURCE_INVALID)
	{
		return (FEEDER_E_OK);
	}

    if (prFeederInfo == NULL)
    {
        geFeederState[eDataType] = FEEDER_IDLE;
        return (FEEDER_E_OK);
    }
    
    // Release allotted resource
    //prFeederInfo->rCritState =  x_crit_start();
    
    if (prFeederInfo->hWatchTimer != (HANDLE_T)NULL)
    {
        VERIFY(OSR_OK == x_timer_delete(prFeederInfo->hWatchTimer));
        prFeederInfo->hWatchTimer = (HANDLE_T)NULL;
    }
    
    if (prFeederInfo->hGetDataSema != (HANDLE_T)NULL)
    {
        VERIFY(OSR_OK == x_sema_delete(prFeederInfo->hGetDataSema));
        prFeederInfo->hGetDataSema = (HANDLE_T)NULL;
    }
    
    if (prFeederInfo->hDataEmptySema != (HANDLE_T)NULL)
    {
        VERIFY(OSR_OK == x_sema_delete(prFeederInfo->hDataEmptySema));
        prFeederInfo->hDataEmptySema = (HANDLE_T)NULL;
    }

#if defined (FEEDER_SUPPORT_FBM_ALLOC) && defined(CC_FBM_FOUR_INST_SUPPORT) 
    
    u4FeederDeviceId = prFeederInfo->rFeedBuf.FeederDeviceId;
    pFbmPool = _FeederFromDeviceToFbm(u4FeederDeviceId);
    if(pFbmPool != NULL)
    {
         FBM_Free(pFbmPool);
    }
#endif    

    geFeederState[eDataType] = FEEDER_IDLE;
    
    //x_crit_end(prFeederInfo->rCritState);

    x_mem_free((void *)prFeederInfo);
    pgrFeederInfo[eDataType] = NULL;
    x_memset(&_rFeederUriData[eDataType], 0, sizeof(FEEDER_URI_DATA_T));

    return (FEEDER_E_OK);
}

//-----------------------------------------------------------------------------
/** Initialize the variables which are required to transmit the data to the
 *  local buffer of the receiver
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederStart(FeederSourceType eDataType)
{
    FEEDER_INFO_T *prFeederInfo = pgrFeederInfo[eDataType];
    
    if (prFeederInfo == NULL)
    {
        return (FEEDER_E_FAIL);
    }
    
    //prFeederInfo->rCritState =  x_crit_start();
    prFeederInfo->bWaitBufferEmpty = FALSE;
    prFeederInfo->bWaitFreeSpace   = FALSE;
    prFeederInfo->bReceiveFileBeginNfy = FALSE;
    prFeederInfo->u4TotalTransmitBytes  = 0;
    prFeederInfo->rFeedBuf.u4BytesInBuf = 0;
    prFeederInfo->rFeedBuf.u4FreeSize = prFeederInfo->rFeedBuf.u4Size;
    prFeederInfo->rFeedBuf.u4ReadAddr = prFeederInfo->rFeedBuf.u4StartAddr;
    prFeederInfo->rFeedBuf.u4WriteAddr = prFeederInfo->rFeedBuf.u4StartAddr;
    prFeederInfo->rFeedBuf.u4LastReadSize = 0;
    prFeederInfo->rFeedBuf.u4LastReadAddr = prFeederInfo->rFeedBuf.u4ReadAddr;
    prFeederInfo->rFeedBuf.fgRingBuf = FALSE;
    geFeederState[eDataType] = FEEDER_START;
    //x_crit_end(prFeederInfo->rCritState);
    
    _eFeederSrcType[eDataType] = FEEDER_SRC_TYPE_HIGH_SPEED_STORAGE;

#ifdef FEEDER_TIME_PROFILE
    #ifdef FEEDER_TIME_PROFILE_REQUEST	
    _rMtmPlayAcc.u4Micros=0;
    _rMtmPlayAcc.u4Seconds=0;
    #endif
    _rDmxAcc.u4Micros=0;
    _rDmxAcc.u4Seconds=0;
	x_memset(_rDmxStartArray,0,sizeof(T_DMX_TIME_PROFILE)*FEEDER_TIME_PROFILE_ARRARY_SIZE);
#endif


    return (FEEDER_E_OK);
}

//-----------------------------------------------------------------------------
/** Initialize the local buffer by NULL
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederStop(FeederSourceType eDataType)
{
    FEEDER_INFO_T *prFeederInfo = pgrFeederInfo[eDataType];
    
    if (prFeederInfo == NULL)
    {
        return (FEEDER_E_OK);
    }
    
    // Unlock the data query sema locked when the buffer is full
    if (prFeederInfo->hGetDataSema != (HANDLE_T)NULL)
    {
        if (prFeederInfo->bWaitFreeSpace)
        {
            VERIFY(OSR_OK == x_sema_unlock(prFeederInfo->hGetDataSema));
        }
    }
    //prFeederInfo->rCritState =  x_crit_start();
    prFeederInfo->bWaitBufferEmpty = FALSE;
    prFeederInfo->bWaitFreeSpace   = FALSE;
    prFeederInfo->rPullThread.hPullThread = (HANDLE_T)NULL;
    prFeederInfo->eApplicationMode = FEEDER_PULL_MODE;
    prFeederInfo->eReceiverMode = FEEDER_PULL_MODE;
    geFeederState[eDataType] = FEEDER_STOP;
    //x_crit_end(prFeederInfo->rCritState);
    
    return (FEEDER_E_OK);
}

//-----------------------------------------------------------------------------
/** Pause the data transmit to the audio decoder
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederPause(FeederSourceType eDataType)
{
    if (geFeederState[eDataType] == FEEDER_START)
    {
        // Change the Feeder state to be FEEDER_PAUSE
        //pgrFeederInfo[eDataType]->rCritState =  x_crit_start();
        geFeederState[eDataType] = FEEDER_PAUSE;
        //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
    }

    return (FEEDER_E_OK);
}

//-----------------------------------------------------------------------------
/** Resume the data transmit to the audio decoder
 *
 *  @param  eDataType       Data type (audio, video..)
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederResume(FeederSourceType eDataType)
{
    if (geFeederState[eDataType] == FEEDER_PAUSE)
    {
        // Change the Feeder state to be FEEDER_PAUSE
        //pgrFeederInfo[eDataType]->rCritState =  x_crit_start();
        geFeederState[eDataType] = FEEDER_START;
        //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
    }

    return (FEEDER_E_OK);
}

//-----------------------------------------------------------------------------
/** Get the information of feeder local buffer
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  prBufferInfo    Memory address of the Feeder buffer information
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederGetBufferInfo(FeederSourceType eDataType, 
                           const FEEDER_BUF_INFO_T *prBufferInfo)
{
    if (geFeederState[eDataType] == FEEDER_IDLE || pgrFeederInfo[eDataType]==NULL)
    {
        return (FEEDER_E_FAIL);
    }
    
    if (prBufferInfo == NULL)
    {
        LOG(7, "_FeederGetBufferInfo: Invalid memory address of buffer information \n");
        return (FEEDER_E_INVALID_PARAM);
    }

    x_memcpy((void *)prBufferInfo, (void *)&pgrFeederInfo[eDataType]->rFeedBuf,
             sizeof(FEEDER_BUF_INFO_T));
    return (FEEDER_E_OK);
}

//-----------------------------------------------------------------------------
/** Reallocate the feeder local buffer with user-specific size
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  u4Size          Memory buffer size
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
#if 0
INT32 _FeederSetBufferSize(FeederSourceType eDataType, UINT32 u4Size)
{
#if 0  
    void *pBuff;
    FEEDER_INFO_T *prFeederInfo = pgrFeederInfo[eDataType];      
    if (prFeederInfo == NULL)
    {
        return (FEEDER_E_OK);
    }
    
    if (u4Size == prFeederInfo->rFeedBuf.u4Size)
    {
        LOG(7, "_FeederSetBufferSize: The current buffer size is the same as the setting value \n");
        return (FEEDER_E_OK);
    }

    // If the feeder starts, buffer re-allocation is not allowed
    if ((geFeederState[eDataType] == FEEDER_START) || 
        (geFeederState[eDataType] == FEEDER_PAUSE))
    {
        LOG(7, "_FeederSetBufferSize: The buffer size can not be changed \n");
        return (FEEDER_E_FAIL);
    }
    
    // Re-allocate the feeder buffer with the new size
    pBuff = x_mem_alloc(u4Size);
    if (pBuff == NULL)
    {
        LOG(7, "_FeederSetBufferSize: Feeder buffer allocation failed \n");
        return (FEEDER_E_MEMORY_ALLOC);
    }

    // Update the information of the feeder buffer
    prFeederInfo->rCritState = x_crit_start();
    if (prFeederInfo->rFeedBuf.u4StartAddr)
    {
        x_mem_free((void *)prFeederInfo->rFeedBuf.u4StartAddr);
    }
    
    prFeederInfo->rFeedBuf.u4Size = u4Size;
    prFeederInfo->rFeedBuf.u4FreeSize = u4Size;
    prFeederInfo->rFeedBuf.u4BytesInBuf = 0;
    prFeederInfo->rFeedBuf.u4StartAddr = (UINT32)pBuff;
    prFeederInfo->rFeedBuf.u4EndAddr = 
        prFeederInfo->rFeedBuf.u4StartAddr + u4Size;
    prFeederInfo->rFeedBuf.u4ReadAddr = prFeederInfo->rFeedBuf.u4StartAddr;
    prFeederInfo->rFeedBuf.u4WriteAddr = prFeederInfo->rFeedBuf.u4StartAddr;
    prFeederInfo->rFeedBuf.u4LastReadAddr = prFeederInfo->rFeedBuf.u4ReadAddr;
    x_crit_end(prFeederInfo->rCritState);
#endif    
    return (FEEDER_E_OK);
}
#endif
//-----------------------------------------------------------------------------
/** Update the write pointer of the feeder local buffer
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  u4WriteSize     Write data size
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederUpdateWritePointer(FeederSourceType eDataType, UINT32 u4WriteSize)
{
    UINT32 u4WriteAddr, u4TempAddr;
    FEEDER_BUF_INFO_T *pFeedBuf;
    
    if (geFeederState[eDataType] == FEEDER_IDLE || pgrFeederInfo[eDataType]==NULL)
    {
        return (FEEDER_E_FAIL);
    }
    
    pFeedBuf = &pgrFeederInfo[eDataType]->rFeedBuf;
    u4WriteAddr = pFeedBuf->u4WriteAddr;
    u4TempAddr  = u4WriteAddr + u4WriteSize;
    u4WriteAddr = (u4TempAddr >= pFeedBuf->u4EndAddr) ?
                  ((u4TempAddr + pFeedBuf->u4StartAddr) - pFeedBuf->u4EndAddr) :
                  (u4TempAddr);
    
    //pgrFeederInfo[eDataType]->rCritState = x_crit_start();
    pFeedBuf->u4FreeSize -= u4WriteSize;
    pFeedBuf->u4BytesInBuf += u4WriteSize;
    pFeedBuf->u4WriteAddr = u4WriteAddr;
    //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
    
    return (FEEDER_E_OK);
}


//-----------------------------------------------------------------------------
/** Set the write pointer of the feeder local buffer
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  u4WritePointer     WritePointer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederSetWritePointer(FeederSourceType eDataType, UINT32 u4WritePointer)
{
    //UINT32 u4WriteAddr, u4TempAddr;
    FEEDER_BUF_INFO_T *pFeedBuf;
    
    if (geFeederState[eDataType] == FEEDER_IDLE || pgrFeederInfo[eDataType]==NULL)
    {
        return (FEEDER_E_FAIL);
    }
    
    pFeedBuf = &pgrFeederInfo[eDataType]->rFeedBuf;

	if(u4WritePointer>=pFeedBuf->u4StartAddr+pFeedBuf->u4Size
		|| u4WritePointer <pFeedBuf->u4StartAddr)
	{
       return FEEDER_E_INVALID_PARAM;
	}
	
    pFeedBuf->u4WriteAddr = u4WritePointer;
    //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
    
    return (FEEDER_E_OK);
}

//-----------------------------------------------------------------------------
/** Update the read pointer of the feeder local buffer
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  u4ReadAddr      current read pointer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederUpdateReadPointer(FeederSourceType eDataType, UINT32 u4ReadAddr)
{
    UINT32 u4ReadSize;
    FEEDER_BUF_INFO_T *pFeedBuf;
    FEEDER_CONSUME_CALLBACK pfnConsume;
    
    if (geFeederState[eDataType] == FEEDER_IDLE || pgrFeederInfo[eDataType]==NULL)
    {
        return (FEEDER_E_FAIL);
    }
    
    pFeedBuf = &pgrFeederInfo[eDataType]->rFeedBuf;
    pfnConsume =  pgrFeederInfo[eDataType]->rDataQuery.pfnConsume;
  
    if ((u4ReadAddr > pFeedBuf->u4EndAddr) || (u4ReadAddr < pFeedBuf->u4StartAddr))
    {
        if (u4ReadAddr != 0)  // megaa 20100304: tmp solution (0 to distinguish push mode)
            return (FEEDER_E_INVALID_PARAM);
    }

    if (u4ReadAddr == 0)  // megaa 20100304: tmp solution (0 to distinguish push mode)
    {
        u4ReadSize = 1;
        u4ReadAddr = pFeedBuf->u4ReadAddr + 1;
        if (u4ReadAddr == pFeedBuf->u4EndAddr)
            u4ReadAddr = pFeedBuf->u4StartAddr;
    }
    else
    {
        u4ReadSize = (u4ReadAddr >= pFeedBuf->u4ReadAddr)? (u4ReadAddr - pFeedBuf->u4ReadAddr):
                                        //pFeedBuf->u4WriteAddr - pFeedBuf->u4ReadAddr;
                                       pFeedBuf->u4Size - (pFeedBuf->u4ReadAddr - u4ReadAddr);
    }
  
    if (pfnConsume != NULL && u4ReadSize > 0)
    {
        pfnConsume(eDataType, pFeedBuf, u4ReadAddr);
    }
  
    //pgrFeederInfo[eDataType]->rCritState = x_crit_start();
    pFeedBuf->u4FreeSize += u4ReadSize;
    pFeedBuf->u4BytesInBuf -= u4ReadSize;
    pFeedBuf->u4ReadAddr = u4ReadAddr;
    //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
    
    //if feeder data is under flow , feeder will notify mw to set pause
    return (FEEDER_E_OK);
}


//-----------------------------------------------------------------------------
/** Transmit the data inside buffer as getting the data of Application 
 *  through the pBuffer
 *
 *  @param  eDataType       Data type (audio, video..)
 *  @param  puiBuffer       Data buffer trasmitted by application
 *  @param  u4Size          Data size
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
#if 0
INT32 _FeederSubmitData(FeederSourceType eDataType, 
                        const UINT8 *puiBuffer, UINT32 u4Size)
{
    INT32 i4RetVal = FEEDER_E_OK;
    UINT32 u4WriteSize, u4CopySize;
    UINT32 u4WriteAddr;
    FEEDER_BUF_INFO_T *pFeedBuf;

    if (geFeederState[eDataType] == FEEDER_IDLE)
    {
        return (FEEDER_E_FAIL);
    }
    
    pFeedBuf = &pgrFeederInfo[eDataType]->rFeedBuf;
    u4WriteAddr = pFeedBuf->u4WriteAddr;
    u4WriteSize = u4Size;
    
    // Check if the read size is large than the feeder buffer
    if (u4WriteSize > pgrFeederInfo[eDataType]->rFeedBuf.u4FreeSize)
    {
        i4RetVal = FEEDER_E_BUFFER_FULL;
        u4WriteSize = pgrFeederInfo[eDataType]->rFeedBuf.u4FreeSize;
    }
    
    u4CopySize = ((u4WriteAddr + u4WriteSize) > pFeedBuf->u4EndAddr) ?
                 (pFeedBuf->u4EndAddr - u4WriteAddr) : u4WriteSize;
    x_memcpy((void *)u4WriteAddr, (void *)puiBuffer, u4CopySize);
    u4WriteAddr += u4WriteSize;
    if ((u4CopySize = (u4WriteSize - u4CopySize)) > 0)
    {
        x_memcpy((void *)pFeedBuf->u4StartAddr, (void *)puiBuffer, u4CopySize);
        u4WriteAddr = pFeedBuf->u4StartAddr + u4CopySize;
    }

    //pgrFeederInfo[eDataType]->rCritState = x_crit_start();
    pFeedBuf->u4FreeSize -= u4WriteSize;
    pFeedBuf->u4BytesInBuf += u4WriteSize;
    pFeedBuf->u4WriteAddr = u4WriteAddr;
    //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
    
    return (i4RetVal);
}
#endif
//-----------------------------------------------------------------------------
/** Application set its data query function for the feeder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  prSetQuery      Query setting
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederRegisterPullDataFunc(FeederSourceType eDataType, 
                                  const FEEDER_QUERY_DATA *prSetQuery)
{
    if (geFeederState[eDataType] == FEEDER_IDLE)
    {
        return (FEEDER_E_FAIL);
    }
    
    if (prSetQuery == NULL)
    {
        LOG(7, "_FeederRegisterPullDataFunc : NULL setting \n");
        return (FEEDER_E_INVALID_PARAM);
    }
    
    //pgrFeederInfo[eDataType]->rCritState = x_crit_start();
    x_memcpy((void *)&pgrFeederInfo[eDataType]->rDataQuery, (void *)prSetQuery,
             sizeof(FEEDER_QUERY_DATA));
    pgrFeederInfo[eDataType]->eApplicationMode = FEEDER_PULL_MODE;
    //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
    return (FEEDER_E_OK);
}

//-----------------------------------------------------------------------------
/** Set the transmit parameters to the receiver
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  fEnable         Enable / Disable data transmit to the receiver
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederSetTransmit(FeederSourceType eDataType, BOOL fEnable)
{
    FEEDER_SET_TRANSMIT pfnSetTransmit;
    
    if (geFeederState[eDataType] == FEEDER_IDLE)
    {
        return (FEEDER_E_FAIL);
    }
    
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "_FeederSetTransmit : Invalid source type setting \n");
        return (FEEDER_E_INVALID_PARAM);
    }

    pfnSetTransmit = garTransmitFuncTbl[eDataType];
    if (pfnSetTransmit != NULL)
    {
        FEEDER_TRANSMIT rTransmit;
        
        rTransmit.eTargetMode = FEEDER_PULL_MODE;
        rTransmit.u4Tag = (UINT32)eDataType;
        rTransmit.u4BufferSize = pgrFeederInfo[eDataType]->rFeedBuf.u4Size;
        rTransmit.pfnTransHandle = (fEnable) ? _FeederTransmitHandler : NULL;
        rTransmit.pfnNotify = (fEnable) ? _FeederNotifyHandle : NULL;
        rTransmit.pfnGetStatus = NULL;

        if (pfnSetTransmit(&rTransmit))
        {
            //pgrFeederInfo[eDataType]->rCritState = x_crit_start();
            x_memcpy((void *)&pgrFeederInfo[eDataType]->rTransmitParam,
                     (void *)&rTransmit, sizeof(FEEDER_TRANSMIT));
            //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
            
            return (FEEDER_E_OK);
        }
        
        return (FEEDER_E_FAIL);
    } 
    
    return (FEEDER_E_OK);
}

//-----------------------------------------------------------------------------
/** Get the state of the target decoder
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  prStatus        Decoder status
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
#if 0
INT32 _FeederGetDecodeStatus(FeederSourceType eDataType, 
                             FEEDER_DECODE_STATUS *prStatus)
{
    FEEDER_TRANSMIT *pTransmitParam;
    
    if (geFeederState[eDataType] == FEEDER_IDLE)
    {
        LOG(7, "_FeederGetDecodeStatus : Invalid source type setting \n");
        return (FEEDER_E_FAIL);
    }

    if (prStatus == NULL)
    {
        LOG(7, "_FeederGetDecodeStatus: NULL data buffer pointer setting \n");
        return (FEEDER_E_FAIL);
    }
    
    pTransmitParam = &pgrFeederInfo[eDataType]->rTransmitParam;
    if (pTransmitParam->pfnGetStatus)
    {
        UNUSED(pTransmitParam->pfnGetStatus(prStatus));
        return (FEEDER_E_OK);
    }

    return (FEEDER_E_FAIL);
}
#endif
//#ifndef __KERNEL__
//-----------------------------------------------------------------------------
/** Register the callback function of Feeder module
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  eCallbackType   Context parameter
 *  @param  pfnCallback     Callback function pointer
 *  @param  u4Param         
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _FeederInstallCallback(FeederSourceType eDataType,
                             FEEDER_NOTIFY_COND eCallbackType,
                             FEEDER_CALBACK pfnCallback, UINT32 u4Param)
{
    UNUSED(u4Param);
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "_FeederInstallCallback: Invalid source type \n");
        return (FEEDER_E_FAIL);
    }

    if (geFeederState[eDataType] == FEEDER_IDLE)
    {
        LOG(7, "_FeederInstallCallback : Invalid source type setting \n");
        return (FEEDER_E_FAIL);
    }
    
    if (pfnCallback == NULL)
    {
        LOG(7, "_FeederInstallCallback: NULL data buffer pointer setting \n");
        return (FEEDER_E_FAIL);
    }
    
    garNotifyTbl[eDataType][eCallbackType] = pfnCallback;
    return (FEEDER_E_OK);
}

//-----------------------------------------------------------------------------
/** Release the callback function of Feeder module
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  eCallbackType   Context parameter
 *  @param  pfnCallback     Callback function pointer
 *
 *  @retval FEEDER_E_OK     Succeed
 *  @retval FEEDER_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
#if 0
INT32 _FeederUnInstallCallback(FeederSourceType eDataType,
                               FEEDER_NOTIFY_COND eCallbackType,
                               FEEDER_CALBACK pfnCallback)
{
    if (eDataType >= FEEDER_SOURCE_TOTAL)
    {
        LOG(7, "_FeederUnInstallCallback: Invalid source type \n");
        return (FEEDER_E_FAIL);
    }

    if (geFeederState[eDataType] == FEEDER_IDLE)
    {
        LOG(7, "_FeederUnInstallCallback : Invalid source type setting \n");
        return (FEEDER_E_FAIL);
    }
    
    if (pfnCallback == NULL)
    {
        LOG(7, "_FeederUnInstallCallback: NULL data buffer pointer setting \n");
        return (FEEDER_E_FAIL);
    }
    
    if (garNotifyTbl[eDataType][eCallbackType] != pfnCallback)
    {
        LOG(5, "_FeederUnInstallCallback: fail to uninstall callback\n");
        LOG(5, "Set callback=%x, cancel callback=%x\n",
               garNotifyTbl[eCallbackType], pfnCallback);
        return (FEEDER_E_FAIL);
    }
        
    garNotifyTbl[eDataType][eCallbackType] = NULL;
    return (FEEDER_E_OK);
}
#endif
//#endif

//-----------------------------------------------------------------------------
/** Set current application to pull mode
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
VOID _FeederSetAppQueryMode(FeederSourceType eDataType, FeederQueryMode eQueryMode)
{
	   //only pull mode being used,push mode will be done throught bagt?
    //pgrFeederInfo[eDataType]->rCritState = x_crit_start();
    LOG(3,"register feeder mode:%d\n",eQueryMode);
    pgrFeederInfo[eDataType]->eApplicationMode = eQueryMode;
    //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
}


//-----------------------------------------------------------------------------
/** Set offset bytes for seeking
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  u8Offset          Offset bytes
 *  @param  u4ReqID
 *
 *  @retval currnet position
 */
//-----------------------------------------------------------------------------
//#ifdef CC_IC_VERIFY
INT64 _FeederSetOffset(FeederSourceType eDataType, UINT64 u8Offset, UINT8 u1Whence, UINT32 u4ReqID)
{
    FEEDER_SEEKFILEPOS_HANDLER pfnSeekPos;
    MM_FILE_OPS_T rFileOps;
    INT64 i8Pos = 0;

    if(!pgrFeederInfo[eDataType])
        return 0;

    pfnSeekPos = pgrFeederInfo[eDataType]->rDataQuery.pfnSeekPos;

    rFileOps.u8Offset = u8Offset;
    rFileOps.u4QueryID = u4ReqID;
    rFileOps.u1Whence = u1Whence;

    if (pfnSeekPos != NULL)
    {
    //pgrFeederInfo[eDataType]->rCritState = x_crit_start();
#ifdef FEEDER_MULTI_INSTANCE
        i8Pos = pfnSeekPos(pgrFeederInfo[eDataType]->rDataQuery.u1PlaymgrAttachedId, eDataType, &rFileOps);
#else
        i8Pos = pfnSeekPos(eDataType, &rFileOps);
#endif
    //x_crit_end(pgrFeederInfo[eDataType]->rCritState);
    }
    
    return i8Pos;
}
//#endif

//-----------------------------------------------------------------------------
/** Set feeder buffer to use ring buf or single buf
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *  @param  fgRing          True: ring buf, FALSE: single buf
 *
 *  @retval 
 */
//-----------------------------------------------------------------------------
VOID _FeederSetRing(FeederSourceType eDataType, BOOL fgRing)
{
    FEEDER_BUF_INFO_T *prFeedBuf = NULL;
    
    if(!pgrFeederInfo[eDataType])
        return ;

    LOG(3,"Feeder Set Ring\n");
    _FeederSetFlush(eDataType);
    prFeedBuf=&pgrFeederInfo[eDataType]->rFeedBuf;
    //pgrFeederInfo[eDataType]->rCritState = x_crit_start();
    prFeedBuf->u4ReadAddr = prFeedBuf->u4StartAddr;
    prFeedBuf->u4WriteAddr = prFeedBuf->u4StartAddr;
    prFeedBuf->u4LastReadAddr = prFeedBuf->u4ReadAddr;
    prFeedBuf->u4BytesInBuf = 0;
    prFeedBuf->u4FreeSize = prFeedBuf->u4Size;
    prFeedBuf->fgRingBuf = fgRing;
	prFeedBuf->u4LastReadSize=0;
    //x_crit_end(pgrFeederInfo[eDataType]->rCritState);

}

#if 0
VOID _FeederResetBuffer(FeederSourceType eDataType)
{
    FEEDER_BUF_INFO_T *prFeedBuf = NULL;
    //FEEDER_CONSUME_CALLBACK pfnConsume;
    //pfnConsume =  pgrFeederInfo[eDataType]->rDataQuery.pfnConsume;
    if(!pgrFeederInfo[eDataType])
        return ;
    prFeedBuf=&pgrFeederInfo[eDataType]->rFeedBuf;
    prFeedBuf->u4ReadAddr = prFeedBuf->u4StartAddr;
    prFeedBuf->u4WriteAddr = prFeedBuf->u4StartAddr;
    prFeedBuf->u4LastReadAddr = prFeedBuf->u4ReadAddr;
    prFeedBuf->u4BytesInBuf = 0;
    prFeedBuf->u4FreeSize = prFeedBuf->u4Size;
	prFeedBuf->u4LastReadSize=0;
	/*
        if (pfnConsume != NULL)
        {
            pfnConsume(eDataType, prFeedBuf, prFeedBuf->u4ReadAddr);
        }
      */
	return;
}
#endif
BOOL _FeederSetRequestInfo(FEEDER_TRANSMIT *prTransmit)
{
    return TRUE;
}


VOID _FeederSetBagtInfo(FeederSourceType eDataType, UINT32 u4Align, UINT32 u4BlockSize)
{
    if((UINT32)eDataType >= (UINT32)FEEDER_SOURCE_INVALID)
    {
        return;
    }
    //USE 192 alignment
    //_u4AlignReduce1 = u4Align - 1;
    _u4BlockSize[eDataType] = u4BlockSize;
    UNUSED(_u4BlockSize[eDataType]);
    UNUSED(u4Align);
}

UINT32 _FeederGetAlignSize(FeederSourceType eDataType)
{
    if((UINT32)eDataType >= (UINT32)FEEDER_SOURCE_INVALID)
    {
        return 0;
    }
    
    if (_gfgUseMiddleware[eDataType])
    {    
    	return _u4BlockSize[eDataType];
    }    	
    
    return 1;
}


//-----------------------------------------------------------------------------
/** flush feeder requests
 *
 *  @retval
*///-----------------------------------------------------------------------------
VOID _FeederSetFlush(FeederSourceType eDataType)
{
#if 0 //def FEEDER_MULTI_INSTANCE
    _fgMsgQFlashId[eDataType]=TRUE;
    _u4LastFlashedDataId[eDataType]=u4LastRquDataId[eDataType];
#else
    FEEDER_REQ_DATA_T rData,rData2;
    UINT16 u2MsgQIdx;
    INT32 ret;
    SIZE_T zMsgSize = sizeof(FEEDER_REQ_DATA_T);

    LOG(3,"Feeder Set Flush\n");
    /*
    while (x_msg_q_receive(&u2MsgQIdx, &rData, &zMsgSize,
                   &(_hFeederDataQ[eDataType]), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK);
    */            
    ret = x_msg_q_receive(&u2MsgQIdx, &rData, &zMsgSize,
                   &(_hFeederDataQ[eDataType]), 1, X_MSGQ_OPTION_NOWAIT);
    if(OSR_OK ==ret)
    {
     //FEEDER_BUF_INFO_T *pFeedBuf = &pgrFeederInfo[eDataType]->rFeedBuf;
     while (x_msg_q_receive(&u2MsgQIdx, &rData2, &zMsgSize,
                   &(_hFeederDataQ[eDataType]), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK);

     _FeederSetWritePointer(eDataType,rData.u4WriteAddr);
    }      
#endif
}

//-----------------------------------------------------------------------------
/** flush feeder requests id for multi instance
 *
 *  @retval
*///-----------------------------------------------------------------------------
#if 0
VOID _FeederResetFlushId(FeederSourceType eDataType)
{
#if 0//def FEEDER_MULTI_INSTANCE   
    u4LastRquDataId[eDataType]=0;
    _u4LastFlashedDataId[eDataType]=0;
    _fgMsgQFlashId[eDataType]=FALSE;
    _u4LastReceiveDataId[eDataType]=0;
#endif
}
#endif

VOID _FeederSetReadyThreshold(FeederSourceType eDataType,UINT32 u4Threshold)
{
}
    
VOID _FeederSetUnderFlowThreshold(FeederSourceType eDataType,UINT32 u4Threshold)
{
}

#if 0
BOOL _FeederIsUnderFlow(FeederSourceType eDataType)
{
    return FALSE;
}
#endif

#if defined (CC_DLNA_SUPPORT) || defined (FEEDER_THRESHOLD_ENABLE)
BOOL _FeederIsReady(FeederSourceType eDataType)
{
//#ifdef FEEDER_THRESHOLD_ENABLE
    UINT32 u4FeederValidSize;

    u4FeederValidSize = pgrFeederInfo[eDataType]->rFeedBuf.u4BytesInBuf;
    if (u4FeederValidSize > gatFSThresholdParam[eDataType].u4FeederReadyThreshold || gatFSThresholdParam[eDataType].fgSwdmxEos)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
//#else
//    return TRUE;
//#endif
}
#endif

VOID _FeederSetInfo(FeederSourceType eDataType,FeederSetInfoType eType,UINT32 u4Para)
{
    if (eType == FEEDER_SWDMX_FILESIZE)
    {
    }
    else if (eType == FEEDER_SWDMX_UNDERFLOW_CB)
    {
    }
    else if (eType == FEEDER_SWDMX_STATUS_CB)
    {
    }
    else if (eType == FEEDER_SWDMX_THRESHOLD)
    {
    }
    else if (eType == FEEDER_SWDMX_SRC_TYPE)
    {
       INT32 i4Index=0;
       for(i4Index=0;i4Index<FEEDER_SOURCE_INVALID;i4Index++)
       {
		   _eFeederSrcType[i4Index] = (FeederSourceType_T)(u4Para);
       }
    }
    else if(eType==FEEDER_SWDMX_INSTANCE)
    {
        pgrFeederInfo[eDataType]->rDataQuery.u1SwdmxAttachedId = u4Para;
    }
    else if (eType == FEEDER_SWDMX_EOS)
    {
    }
    else if (eType == FEEDER_MM_USE)
    {
        _gfgUseMiddleware[eDataType] = (BOOL)(u4Para);
    }
    else
    {
        ASSERT(0);
    }
}

//-----------------------------------------------------------------------------
/** MMP mode, switch feeder buffer to feeder_mmp buffer
 *
 *  @param  eDataType       Tramsmit data type (audio, video or program)
 *
 *  @retval
*///-----------------------------------------------------------------------------
INT32 _FeederSetMMP(FeederSourceType eDataType)
{
    FEEDER_INFO_T *prFeederInfo = pgrFeederInfo[eDataType];
    FBM_POOL_T* pFbmPool = NULL;
    UINT32 u4RealBufStart = 0;
    UINT32  u4BufOffset=0,u4BuffSize=0; 
#if defined (FEEDER_SUPPORT_FBM_ALLOC) && defined(CC_FBM_FOUR_INST_SUPPORT) 
    UINT32 u4FeederDeviceId = 0;
#endif
    
    if (prFeederInfo == NULL)
    {
        return (FEEDER_E_FAIL);
    }

    
#if defined (FEEDER_SUPPORT_FBM_ALLOC) && defined(CC_FBM_FOUR_INST_SUPPORT) 
        u4FeederDeviceId = prFeederInfo->rFeedBuf.FeederDeviceId;
        pFbmPool = _FeederFromDeviceToFbm(u4FeederDeviceId);
        if(pFbmPool != NULL)
        {
             FBM_Free(pFbmPool);
        }
#endif    

    pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER_MMP);
#ifdef __KERNEL__
    u4RealBufStart = pFbmPool->u4Addr & PAGE_MASK;
#else
    u4RealBufStart = (pFbmPool->u4Addr + 31) & ~31;
#endif

    u4BufOffset=FEEDER_RESERVED_SIZE;
    u4BuffSize=pFbmPool->u4Size-u4BufOffset;	
    
    prFeederInfo->rFeedBuf.u4PhysicalAddr = u4RealBufStart;
    u4RealBufStart = VIRTUAL(u4RealBufStart+u4BufOffset);

    
	LOG(0,"!!!!@@@@@_FeederSetMMP,feeder size reset to new value,osz = 0x%x,nsz = 0x%x\n",prFeederInfo->rFeedBuf.u4Size,u4BuffSize);

    prFeederInfo->rFeedBuf.FeederDeviceId  = FEEDER_FBM_MMP;
    //prFeederInfo->rFeedBuf.u4FeederMemAddr = pFbmPool->u4Addr;
    prFeederInfo->rFeedBuf.u4Size = u4BuffSize;
    prFeederInfo->rFeedBuf.u4StartAddr = VIRTUAL(u4RealBufStart);
    prFeederInfo->rFeedBuf.u4EndAddr = prFeederInfo->rFeedBuf.u4StartAddr + u4BuffSize;

    //prFeederInfo->rCritState =  x_crit_start();
    prFeederInfo->bWaitBufferEmpty = FALSE;
    prFeederInfo->bWaitFreeSpace   = FALSE;
    prFeederInfo->bReceiveFileBeginNfy = FALSE;
    prFeederInfo->u4TotalTransmitBytes  = 0;
    prFeederInfo->rFeedBuf.u4BytesInBuf = 0;
    prFeederInfo->rFeedBuf.u4FreeSize = prFeederInfo->rFeedBuf.u4Size;
    prFeederInfo->rFeedBuf.u4ReadAddr = prFeederInfo->rFeedBuf.u4StartAddr;
    prFeederInfo->rFeedBuf.u4WriteAddr = prFeederInfo->rFeedBuf.u4StartAddr;
    prFeederInfo->rFeedBuf.u4LastReadSize = 0;
    prFeederInfo->rFeedBuf.u4LastReadAddr = prFeederInfo->rFeedBuf.u4ReadAddr;
    prFeederInfo->rFeedBuf.u4PhysicalAddrOffset = u4BufOffset;
    return (FEEDER_E_OK);
}

VOID _FeederIgnoreReady(FeederSourceType eDataType)
{
}


INT32 _FeederSetUriMode(FeederSourceType eType, UINT32 u4BufSize, UINT32 u4KeepBufThres, UINT32 u4RebufThres)
{
    FEEDER_INFO_T *prFeederInfo;
    LOG(0,"feeder set uri mode,new size = 0x%x\n",u4BufSize);
    
    if (geFeederState[eType] != FEEDER_INIT && geFeederState[eType] != FEEDER_START)  // do nothing if _FeederOpen not called yet
    {
        return FEEDER_E_OK;
    }
    	
    prFeederInfo = pgrFeederInfo[eType];

    _rFeederUriData[eType].eState = FEEDER_URI_NORMAL;
    _rFeederUriData[eType].u4FeederLimitedBufSz = u4BufSize;
    _rFeederUriData[eType].u4KeepBufThres = u4KeepBufThres;
    _rFeederUriData[eType].u4RebufThres = u4RebufThres;

    
	LOG(0,"!!!!@@@@@_FeederSetUriMode,feeder size reset to new value,osz = 0x%x,nsz = 0x%x\n",prFeederInfo->rFeedBuf.u4Size,u4BufSize);
    if (prFeederInfo->rFeedBuf.u4Size == u4BufSize)
    {
        return FEEDER_E_OK;
    }

    prFeederInfo->rFeedBuf.u4Size  = u4BufSize;
    prFeederInfo->rFeedBuf.u4EndAddr = 
    prFeederInfo->rFeedBuf.u4StartAddr + prFeederInfo->rFeedBuf.u4Size;
    prFeederInfo->rFeedBuf.u4FreeSize = prFeederInfo->rFeedBuf.u4Size;

    return FEEDER_E_OK;
}

INT32 _FeederGetBufferFullness(FeederSourceType eType, UINT32 *pu4Len, UINT32 *pu4Percent)
{
    FEEDER_INFO_T *prFeederInfo;
    
    prFeederInfo = pgrFeederInfo[eType];
    *pu4Len = prFeederInfo->rFeedBuf.u4BytesInBuf;
    *pu4Percent = prFeederInfo->rFeedBuf.u4BytesInBuf * 100 / prFeederInfo->rFeedBuf.u4Size;
    
    return FEEDER_E_OK;
}

INT32 _FeederInitPreLoad(FeederSourceType eType, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    FEEDER_INFO_T *prFeederInfo;
    
    prFeederInfo = pgrFeederInfo[eType];
    *pu4Addr = prFeederInfo->rFeedBuf.u4StartAddr;
    *pu4Size = _rFeederUriData[eType].u4FeederLimitedBufSz - _rFeederUriData[eType].u4KeepBufThres;
    *pu4Size = *pu4Size / _u4BlockSize[eType] * _u4BlockSize[eType];
    _rFeederUriData[eType].eState = FEEDER_URI_PRE_LOADING;
    
    return FEEDER_E_OK;
}

INT32 _FeederSetPreLoadDone(FeederSourceType eType, UINT32 u4WriteSize)
{
    //FEEDER_INFO_T *prFeederInfo;
    
    _rFeederUriData[eType].u4PreLoadSize = u4WriteSize;  // record the pre-loaded size
    _rFeederUriData[eType].eState = FEEDER_URI_NORMAL;
    //prFeederInfo = pgrFeederInfo[eType];
    //prFeederInfo->rFeedBuf.u4WriteAddr += u4WriteSize;
    
    return FEEDER_E_OK;
}

VOID _FeederSetSkypeSourceFlag(BOOL bIsSkypeSrc)
{
	_fgFeederSkypeSource = bIsSkypeSrc;
}

BOOL _FeederGetSkypeSourceFlag(void)
{
	return _fgFeederSkypeSource;
}

INT32 _FeederSetDataDelay(FeederSourceType eDataType,UINT32 u4DelayTime)
{
   _u4DataDelayTime=u4DelayTime;
   UNUSED(eDataType);
   return FEEDER_E_OK;
}

VOID _FeederFlushBuffer(FeederSourceType eDataType)
{
    //UINT32 u4Ret;
     LOG(7, "_FeederFlushBuffer eDataType %d\n", eDataType);
    if(geFeederState[eDataType] == FEEDER_IDLE 
        || pgrFeederInfo[eDataType]==NULL
        ||(UINT32)eDataType >= (UINT32)FEEDER_SOURCE_INVALID)
    {
        return;
    }
    
	if(pgrFeederInfo[eDataType]->rDataQuery.pfFlush)
	{
       pgrFeederInfo[eDataType]->rDataQuery.pfFlush(eDataType);
	   //LOG(3,"_FeederFlushBuffer ret %d\n",u4Ret);
	}
	else
	{
	   LOG(3,"_FeederFlushBuffer rDataQuery.pfFlush is null\n");
	}
	return;
}


INT32 _FeederGetMemoryInfo(FeederMemInfo * pMemInfo)
{
  if(pMemInfo)
  {
   UINT32 c = 0;
   FBM_POOL_T* pFbmPool = NULL;

   LOG(0,"FEEDER get memory info\n");
   pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER_MMP);
   if(pFbmPool)
    {      
       ++c;
       pMemInfo->u4FeederMMpVirAddr = VIRTUAL(pFbmPool->u4Addr);
       pMemInfo->u4FeederMMpBufSz = pFbmPool->u4Size;
       LOG(0,"mmp vir:0x%x,size:0x%x\n",pMemInfo->u4FeederMMpVirAddr,pMemInfo->u4FeederMMpBufSz);
    }

    pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER);
    if(pFbmPool)
    {
       ++c;
       pMemInfo->u4FeederVirAddr = VIRTUAL(pFbmPool->u4Addr);
       pMemInfo->u4FeederBufSz = pFbmPool->u4Size;
       
       LOG(0,"feeder vir:0x%x,size:0x%x\n",pMemInfo->u4FeederVirAddr,pMemInfo->u4FeederBufSz);
    }

    pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER2);
	if(pFbmPool)
	 {
	     ++c;
		 pMemInfo->u4Feeder2VirAddr = VIRTUAL(pFbmPool->u4Addr);
		 pMemInfo->u4Feeder2BufSz = pFbmPool->u4Size;
		 LOG(0,"feeder2 vir:0x%x,size:0x%x\n",pMemInfo->u4Feeder2VirAddr,pMemInfo->u4Feeder2BufSz);
	 }
    #ifdef CC_FBM_FOUR_INST_SUPPORT
    pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER3);
	if(pFbmPool)
	{
	    ++c;
	    pMemInfo->u4Feeder3VirAddr = VIRTUAL(pFbmPool->u4Addr);
	    pMemInfo->u4Feeder3BufSz = pFbmPool->u4Size;
	    LOG(0,"feeder3 vir:0x%x,size:0x%x\n",pMemInfo->u4Feeder3VirAddr,pMemInfo->u4Feeder3BufSz);
	}
    pFbmPool = FBM_GetPoolInfo(FBM_POOL_TYPE_FEEDER4);
	if(pFbmPool)
	{
	    ++c;
	    pMemInfo->u4Feeder4VirAddr = VIRTUAL(pFbmPool->u4Addr);
	    pMemInfo->u4Feeder4BufSz = pFbmPool->u4Size;
	    LOG(0,"feeder4 vir:0x%x,size:0x%x\n",pMemInfo->u4Feeder4VirAddr,pMemInfo->u4Feeder4BufSz);
	}
    #endif

    pMemInfo->u4Counter = c;
    return 1;
  }
  return 0;
}

VOID _FeederSetRequestType(FeederSourceType eDataType, FeederRequestType eRequestType)
{
	FEEDER_INFO_T *prFeederInfo;
	LOG(3,"register feeder query:%d\n",eRequestType);
	if(eDataType >= FEEDER_SOURCE_TOTAL)
	{
		return;
	}
	prFeederInfo = pgrFeederInfo[eDataType];
	if(prFeederInfo != NULL)
	{
		prFeederInfo->eRequestType = eRequestType;
	}
}
