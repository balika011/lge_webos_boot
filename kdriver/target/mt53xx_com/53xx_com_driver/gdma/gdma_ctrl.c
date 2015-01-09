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
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: gdma_ctrl.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file pod_if.c
 *  This file contains implementation of exported APIs of POD.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "gdma_ctrl.h"
#include "x_ckgen.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define GDMA_DEBUG
#define GDMA_INTERRUPT_ON     1  
#define UNIT_LOCKTIMEOUT    (10*1000)           //10sec

//-----------------------------------------------------------------------------
// static variables
//-----------------------------------------------------------------------------
static BOOL _fgGdmaInit = FALSE;
static HANDLE_T sema_memcp_func_exit;
static HANDLE_T sema_memcp_func_entry;
static HANDLE_T hGdma_msg;
static HANDLE_T hGdmaThread;
static UINT32 u4CurrentID;

//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------
static VOID _GDMA_SetDataLenReg( UINT32 Val )
{
    _GDMA_WRITE32MSK( REG_GDMA_DATA_LEN, Val, 0x00FFFFFF );
    LOG(LOG_NOTE, "[_GDMA_SetDataLenReg] REG_GDMA_DATA_LEN: 0x%08X\n", _GDMA_READ32(REG_GDMA_DATA_LEN) );        
}

static VOID _GDMA_SetSrcAddrReg( UINT32 Val )
{
    _GDMA_WRITE32MSK( REG_GDMA_SRC_ADDR, Val , 0xFFFFFFFF );
    LOG(LOG_NOTE, "[_GDMA_SetSrcAddrReg] REG_GDMA_SRC_ADDR: 0x%08X\n", _GDMA_READ32(REG_GDMA_SRC_ADDR) );
}

static VOID _GDMA_SetDstAddrReg( UINT32 Val )
{
    _GDMA_WRITE32MSK( REG_GDMA_DST_ADDR, Val , 0xFFFFFFFF );
    LOG(LOG_NOTE, "[_GDMA_SetDstAddrReg] REG_GDMA_DST_ADDR: 0x%08X\n", _GDMA_READ32(REG_GDMA_DST_ADDR) );        
}

static VOID _GDMA_SetFire( VOID )
{
    //_GDMA_WRITE32MSK( REG_GDMA_FIRE, 0x00000001, 0x00000001 );
    _GDMA_WRITE32(REG_GDMA_FIRE, 0x1); //constand to fill
    LOG(LOG_NOTE, "[_GDMA_SetFire] REG_GDMA_FIRE: 0x%08X\n", _GDMA_READ32(REG_GDMA_FIRE) );        
}
/*
static VOID _GDMA_BusyWaiting(VOID)
{
    while ((_GDMA_READ32(REG_GDMA_STATUS) & 0x80000000) != 0);
    while ((_GDMA_READ32(REG_GDMA_STATUS) & 0xffffff) != 0);
}
*/
static VOID _GDMA_EnableIntBit( VOID )
{
    _GDMA_WRITE32MSK( REG_GDMA_CONFIG, 0x00000001, 0x00000001 );
    LOG(LOG_NOTE, "[_GDMA_EnableIntBit] REG_GDMA_CONFIG: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) );        
}

static VOID _GDMA_DisableIntBit( VOID )
{
    _GDMA_WRITE32MSK( REG_GDMA_CONFIG, 0x00000000, 0x00000001 );
    LOG(LOG_NOTE, "[_GDMA_DisableIntBit] REG_GDMA_CONFIG: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) );        
}

static VOID _GDMA_EnableClkGen( VOID )
{
    _GDMA_WRITE32MSK( REG_GDMA_CONFIG, 0x00000080, 0x00000080 );
    LOG(LOG_NOTE, "[_GDMA_EnableClkGen] REG_GDMA_CONFIG: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) );        
}

static VOID _GDMA_DisableClkGen( VOID )
{
    _GDMA_WRITE32MSK( REG_GDMA_CONFIG, 0x00000000, 0x00000080 );
    LOG(LOG_NOTE, "[_GDMA_DisableClkGen] REG_GDMA_CONFIG: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) );        
}

static VOID _GDMA_Isr( UINT16 u2Vector_ID)
{
    ASSERT( u2Vector_ID == VECTOR_GDMA );

    LOG(LOG_NOTE, "[_GDMA_Isr] u2Vector_ID=%d\n", u2Vector_ID );
    VERIFY(BIM_ClearIrq(VECTOR_GDMA));

    //release semaphore
    VERIFY( x_sema_unlock(sema_memcp_func_exit)==OSR_OK);
}
#if (GDMA_INTERRUPT_ON==1)
static VOID _GDMA_WaitTillDmaAvaliable(VOID)
{
    //lock entry semaphore
#if 0
    VERIFY(x_sema_lock_timeout(sema_memcp_func_entry, UNIT_LOCKTIMEOUT)==OSR_OK);
#else
    if (x_sema_lock_timeout(sema_memcp_func_entry, UNIT_LOCKTIMEOUT)!=OSR_OK)
    {
        Printf("[_GDMA_WaitTillDmaAvaliable] Lock sema_memcp_func_entry failed in %d milisec time\n",UNIT_LOCKTIMEOUT);
    }
#endif
}
static VOID _GDMA_WaitTillDmaDone(VOID)
{
   //lock semaphore
#if 0
    VERIFY(x_sema_lock_timeout(sema_memcp_func_exit, UNIT_LOCKTIMEOUT)==OSR_OK);
#else
    if((x_sema_lock_timeout(sema_memcp_func_exit, UNIT_LOCKTIMEOUT)!=OSR_OK))
    {
        Printf("[_GDMA_WaitTillDmaDone] Lock sema_memcp_func_exit failed in %d milisec time\n",UNIT_LOCKTIMEOUT);
    }
#endif
    //---->wake up by Gdma Isr

    //make dma avaliable again
    VERIFY(x_sema_unlock(sema_memcp_func_entry)==OSR_OK);    
}
#endif      //GDMA_INTERRUPT_ON
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#ifndef __KERNEL__
UINT32 HalGetMMUTableAddress(void);
void _GDMA_MMU_Init(void)
{
    /*
       0xF0068104 = 0x6400000 <- page table ¦b dram ¦ì¸m
       0xF0068100 = 0x7
       0xF0068108 = 0x130011
    */
    vIO32Write4B(0xF0068104, HalGetMMUTableAddress()); // base address of mmu table
    vIO32Write4B(0xF0068100, 0x7);
    vIO32Write4B(0xF0068108, 0x130011);
    vIO32Write4B(0xF0068110, 0xa);
}
#endif

//-----------------------------------------------------------------------------
/** GDMA_SendCmd
 */
//-----------------------------------------------------------------------------

void _Gdma_SendMemOpCmd(const GDMA_MSG_INFO_T *prMemOpInfo)
{
	INT32 i4Ret;
	//GDMA_MSG_INFO_T rMemOpInfo;
	ASSERT(prMemOpInfo != NULL);
	if(prMemOpInfo != NULL)
	{
		i4Ret = x_msg_q_send(hGdma_msg, (void *)prMemOpInfo,sizeof(GDMA_MSG_INFO_T), 249);
	}
	else
	{
		Printf("_Gdma_SendMemOpCmd none.\n");
	}
}

//-----------------------------------------------------------------------------
/** GDMA_ReceiveCmd
 */
//-----------------------------------------------------------------------------

GDMA_MSG_INFO_T * _GdmaReceiveMemOp(GDMA_MSG_INFO_T * pMemOpInfo)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
	//GDMA_MSG_INFO_T *pMemOpInfo=NULL;
	//GDMA_MSG_INFO_T rMemOpInfo;
    zMsgSize = sizeof(GDMA_MSG_Q_SIZE);
	VERIFY(x_msg_q_receive(&u2MsgQIdx, pMemOpInfo, &zMsgSize,
		&(hGdma_msg), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);
	//pMemOpInfo=&rMemOpInfo;
	return pMemOpInfo;
}


//-----------------------------------------------------------------------------
/** GDMA_MainLoop
 */
//-----------------------------------------------------------------------------

void _GDMA_MainLoop(void* pvArg)
{
	//Printf("gdma thread start.\n");
	while (_fgGdmaInit)
    {
    	GDMA_MSG_INFO_T *pMemOpInfo;
		GDMA_MSG_INFO_T rMemOpInfo;
		pMemOpInfo=&rMemOpInfo;
		//Printf("gdma thread GdmaInited.\n");
    	pMemOpInfo=_GdmaReceiveMemOp(pMemOpInfo);
		u4CurrentID=pMemOpInfo->u4TaskId;
		//Printf("gdma thread Receive Memory Option doen.\n");
		if(pMemOpInfo->u4FillMode == FILL_MODE_FILL)
		{
			//Printf("gdma memset start.\n");
			_GDMA_MemSet( pMemOpInfo->distAddr, pMemOpInfo->u4ConstoFill, pMemOpInfo->u4DataLenth);
		}

		if(pMemOpInfo->u4FillMode == FILL_MODE_DMA)
		{
			//Printf("gdma memcopy start.\n");
			_GDMA_NonOverlapMemcpy(pMemOpInfo->distAddr, pMemOpInfo->sourceAddr, pMemOpInfo->u4DataLenth );
		}
    }
	x_thread_exit();
}

//-----------------------------------------------------------------------------
/** GDMA_Init
 */
//-----------------------------------------------------------------------------
INT32 _GDMA_Init(void)
{
    x_os_isr_fct  pfOldIsr ;
	//add for ansync process
	UCHAR uGdmaId;
    CHAR szBuf[16];
	//
    if(  _fgGdmaInit != TRUE )
    {   //first time use
        _GDMA_WRITE32MSK(REG_GDMA_CONFIG, ((UINT32)0x3)<<30 , 0xC0000000 ); //soft reset
        LOG(LOG_NOTE, "[GDMA_Init] Reset: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) );        
        
        _fgGdmaInit = TRUE;
        
        _GDMA_WRITE32MSK(REG_GDMA_CONFIG, (0x0)<<30 , 0xC0000000 ); //un-reset
        LOG(LOG_NOTE, "[GDMA_Init] Un-Reset: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) ); 
        
        _GDMA_EnableIntBit();
        _GDMA_EnableClkGen();
		
        VERIFY( x_sema_create(&sema_memcp_func_entry, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK)==OSR_OK);       
        VERIFY(x_sema_create(&sema_memcp_func_exit, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK)==OSR_OK);      
//add for ansync process
		uGdmaId=0;
		x_snprintf(szBuf, sizeof(szBuf), "GDMA%3d", uGdmaId);
		VERIFY(x_msg_q_create(&hGdma_msg, szBuf, sizeof(GDMA_MSG_INFO_T), GDMA_MSG_Q_SIZE) == OSR_OK);
		Printf("init hGdma_msg:0x%08X",hGdma_msg);
		VERIFY(x_thread_create(&hGdmaThread, szBuf, GDMA_STACK_SIZE, GDMA_THREAD_PRIORITY,  _GDMA_MainLoop, sizeof(uGdmaId), (void*)&uGdmaId) == OSR_OK);


//end of ansync process



		
        
        VERIFY( x_reg_isr( VECTOR_GDMA, _GDMA_Isr, &pfOldIsr )==OSR_OK );
#ifndef __KERNEL__
        _GDMA_MMU_Init();
#endif
        _GDMA_WRITE32MSK(REG_GDMA_CONFIG, 0, WPROT_EN);
        // GDMA_EmuInit();
    } 
    else
    {
        LOG(LOG_NOTE, "was initialized before, will not init .\n");
    }
    
    return 0;
}


//-----------------------------------------------------------------------------
/** GDMA_Stop
 */
//-----------------------------------------------------------------------------
INT32 _GDMA_Stop(VOID)
{
    //clean flgas
    x_os_isr_fct  pfOldIsr ;
    _fgGdmaInit = FALSE;

    //stop isr
    VERIFY( x_reg_isr( VECTOR_GDMA, NULL, &pfOldIsr )==OSR_OK );
    VERIFY( BIM_DisableIrq(VECTOR_GDMA)==TRUE );

    //delete semaphores
    x_sema_delete(sema_memcp_func_entry);
    x_sema_delete(sema_memcp_func_exit);

    _GDMA_DisableIntBit();
    _GDMA_DisableClkGen();
    return 0;
}

VOID * _GDMA_MemSet( VOID *s, UINT32 c , UINT32 n)
{
#ifndef __KERNEL__
    HalFlushInvalidateDCache();
#endif
	_GDMA_Init();
#if (GDMA_INTERRUPT_ON==1)
    _GDMA_WaitTillDmaAvaliable();   //aovid other thread re-enter
#else
  #error polling mode not ready yet
#endif

    _GDMA_WRITE32MSK(REG_GDMA_CONFIG, FILL_MODE_FILL , FILL_MODE_MSK ); 
    _GDMA_SetDataLenReg( (UINT32)n );
    _GDMA_SetSrcAddrReg(0);
    _GDMA_SetDstAddrReg( (UINT32)s);
    _GDMA_WRITE32(REG_GDMA_FILLVALUE, (UINT32)c ); //constand to fill
    _GDMA_SetFire();
    //-----wait for DMA finish its job--------------
#if (GDMA_INTERRUPT_ON==1)
    _GDMA_WaitTillDmaDone();        //block I/O
#else
  #error polling mode not ready yet
#endif
    //x_thread_delay(1000);
#ifndef __KERNEL__
	HalFlushDCache();
    HalFlushInvalidateDCache();
#endif
    return s;
}


INT32 _GDMA_NonOverlapMemcpy( VOID *dst, const VOID *src, UINT32 n )
{
    UINT32 u4Diff=0;
#ifndef __KERNEL__
    BOOL fgIsSrcCached=CHECK_IS_CACHED( (UINT32)src);
    BOOL fgIsDstCached=CHECK_IS_CACHED( (UINT32)dst);
#endif
    //----check parameters-----------------------------------
    if( n==0 )  
    {
        Printf("[_GDMA__NonOverlapMemcpy] Invalid, Size equals to 0!\n");
        return 0;
    }

    u4Diff = (dst > src)? (UINT32)dst - (UINT32)src : (UINT32)src - (UINT32)dst ; 
    
    if ( u4Diff < n )
    {
        Printf("[_GDMA__NonOverlapMemcpy] Overlap not allowed!\n");
        return 0;	//no overlap!
    }
    //------------------------------------------------------

    _GDMA_Init(); 
    _GDMA_WRITE32MSK(REG_GDMA_CONFIG, FILL_MODE_DMA , FILL_MODE_MSK );  //dma memory copy
   //-----check if gdma engine is avaliable; if not, wait for it--------------
#if (GDMA_INTERRUPT_ON==1)
    _GDMA_WaitTillDmaAvaliable();   //aovid other thread re-enter
#else
  #error polling mode not ready yet
#endif
    //------------------------------------------------------------
    
#ifndef __KERNEL__
    if( fgIsSrcCached || fgIsDstCached )
    {
        HalFlushDCache();
    }
#endif
    //---------set gdma engine-----------------------------------
    _GDMA_SetSrcAddrReg( (UINT32)src );
    _GDMA_SetDstAddrReg( (UINT32)dst );
    _GDMA_SetDataLenReg( (UINT32)n );
    _GDMA_SetFire();
    //-----------------------------------------------------------

    //-----wait for DMA finish its job--------------
#if (GDMA_INTERRUPT_ON==1)
    _GDMA_WaitTillDmaDone();        //block I/O
#else
  #error polling mode not ready yet
#endif
    //-------------------------------------------

#ifndef __KERNEL__
    if( fgIsSrcCached || fgIsDstCached )
    {
        HalFlushInvalidateDCache();
    }
#endif

    return n;		//link this to real status later
}

UINT32 _GetCurrentMemOpTaskID(void)
{
	return u4CurrentID;
}

#ifdef CC_SUPPORT_STR
void _GDMA_pm_suspend(void)
{
	VERIFY(BIM_DisableIrq(VECTOR_GDMA));
	_GDMA_DisableIntBit();
	_GDMA_DisableClkGen();
	
    CKGEN_WRITE32(0x1c0, CKGEN_READ32(0x1c0) & (~(1<<19))); // 0xd1c0[19]=0
    CKGEN_WRITE32(0x1c8, CKGEN_READ32(0x1c8) & (~(1<<19))); // 0xd1c0[19]=0
}

void _GDMA_pm_resume(void)
{
    CKGEN_WRITE32(0x1c8, CKGEN_READ32(0x1c8) | (1<<19)); // 0xd1c0[19]=0
    CKGEN_WRITE32(0x1c0, CKGEN_READ32(0x1c0) | (1<<19)); // 0xd1c0[19]=0

		//reinit gdma controller
    _GDMA_WRITE32MSK(REG_GDMA_CONFIG, ((UINT32)0x3)<<30 , 0xC0000000 ); //soft reset
    LOG(LOG_NOTE, "[GDMA_Init] Reset: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) );        

    _GDMA_WRITE32MSK(REG_GDMA_CONFIG, (0x0)<<30 , 0xC0000000 ); //un-reset
    LOG(LOG_NOTE, "[GDMA_Init] Un-Reset: 0x%08X\n", _GDMA_READ32(REG_GDMA_CONFIG) ); 
	        
    _GDMA_EnableIntBit();
    _GDMA_EnableClkGen();

    _GDMA_WRITE32MSK(REG_GDMA_CONFIG, 0, WPROT_EN);
	VERIFY(BIM_EnableIrq(VECTOR_GDMA));
}
#endif // CC_SUPPORT_STR

