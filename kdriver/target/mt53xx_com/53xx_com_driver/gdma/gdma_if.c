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
 * $RCSfile: gdma_if.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file pod_if.c
 *  This file contains implementation of exported APIs of POD.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "gdma_if.h"
#include "gdma_ctrl.h"

LINT_EXT_HEADER_BEGIN

#include "x_os.h"

LINT_EXT_HEADER_END

static UINT32 u4TaskId=0;
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** GDMA_Init
 */
//-----------------------------------------------------------------------------
INT32 GDMA_Init(VOID)
{
    return _GDMA_Init();
}


//-----------------------------------------------------------------------------
/** GDMA_Stop
 */
//-----------------------------------------------------------------------------
INT32 GDMA_Stop(VOID)
{
    return _GDMA_Stop();
}


/** GDMA_MemCpyAnsync
 */
//-----------------------------------------------------------------------------
UINT32 GDMA_MemCpyAnsync( VOID *dst, const VOID *src, UINT32 n )
{
	 GDMA_MSG_INFO_T *prMemOpInfo=NULL;
	 GDMA_MSG_INFO_T rMemOpInfo;
	 prMemOpInfo=&rMemOpInfo;
	 //x_memset((void *)prMemOpInfo, 0, sizeof(GDMA_MSG_INFO_T));
	 x_memset(&rMemOpInfo, 0, sizeof(GDMA_MSG_INFO_T));
     x_memcpy(&rMemOpInfo, prMemOpInfo, sizeof(GDMA_MSG_INFO_T));
	 if(u4TaskId<32767)
	 {
	 	u4TaskId++;
	 }
	 else
	 {
	 	u4TaskId=0;
	 }
	 //Printf("GDMA_MemCpyAnsync");
	  _GDMA_Init();
	 rMemOpInfo.distAddr=dst;
	 rMemOpInfo.sourceAddr=(void *)src;
	 rMemOpInfo.u4DataLenth=n;
	 rMemOpInfo.u4FillMode=FILL_MODE_DMA;
	 rMemOpInfo.u4TaskId=u4TaskId;
	_Gdma_SendMemOpCmd(prMemOpInfo);
	return u4TaskId;
}

#ifdef __KERNEL__
EXPORT_SYMBOL(GDMA_MemCpyAnsync);
#endif

UINT32 GDMA_MemSetAnsync( VOID *s, UINT32 c , UINT32 n)
{
	 GDMA_MSG_INFO_T *prMemOpInfo;
	 GDMA_MSG_INFO_T rMemOpInfo;
	 prMemOpInfo=&rMemOpInfo;
	 //x_memset((void *)prMemOpInfo, 0, sizeof(GDMA_MSG_INFO_T));
	 x_memset(&rMemOpInfo, 0, sizeof(GDMA_MSG_INFO_T));
     x_memcpy(&rMemOpInfo, prMemOpInfo, sizeof(GDMA_MSG_INFO_T));
	 if(u4TaskId<65535)
	 {
	 	u4TaskId++;
	 }
	 else
	 {
	 	u4TaskId=0;
	 }
	 //Printf("GDMA_MemSetAnsync\n");
	 rMemOpInfo.distAddr=s;
	 rMemOpInfo.u4ConstoFill=c;
	 rMemOpInfo.u4DataLenth=n;
	 rMemOpInfo.u4FillMode=FILL_MODE_FILL;
	 rMemOpInfo.u4TaskId=u4TaskId;
	 _GDMA_Init();
	_Gdma_SendMemOpCmd((void *)&rMemOpInfo);
	//return s;
	return u4TaskId;
}

//-----------------------------------------------------------------------------
/** GDMA_MemCpy
 */
//-----------------------------------------------------------------------------
INT32 GDMA_MemCpy( VOID *dst, const VOID *src, UINT32 n )
{
    return _GDMA_NonOverlapMemcpy ( (void *)dst , (void *)src , n );
    //return GDMA_MemCpyAnsync( (void *)dst, (void *)src, n );
}
#ifdef __KERNEL__
EXPORT_SYMBOL(GDMA_MemCpy);
#endif

VOID * GDMA_MemSet( VOID *s, UINT32 c , UINT32 n)
{
    return _GDMA_MemSet( (VOID *) s, (UINT32) c ,  (UINT32) n);
   //Printf("GDMA_MemSet\n");
   //return GDMA_MemSetAnsync( (void *)s, (UINT32) c , (UINT32) n);
}

BOOL IsMemOptionDone(UINT32 u4TsakID)
{
	UINT32 u4CurrentID;
	 u4CurrentID=_GetCurrentMemOpTaskID();
	 if(u4CurrentID < u4TsakID)
	 {
	 	return TRUE;
	 }
	 else
	 {
	 	return FALSE;
	 }
	 
}

#ifdef CC_SUPPORT_STR
void GDMA_pm_str_suspend(void)
{
    _GDMA_pm_suspend();
}

void GDMA_pm_str_resume(void)
{
    _GDMA_pm_resume(); 
}

#ifdef __KERNEL__
EXPORT_SYMBOL(GDMA_pm_str_suspend);
EXPORT_SYMBOL(GDMA_pm_str_resume);
#endif

#endif // CC_SUPPORT_STR
