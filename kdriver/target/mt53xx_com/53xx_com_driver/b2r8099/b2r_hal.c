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
 * $Date: 2015/01/27 $
 * $RCSfile: b2r_hal.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file b2r_hal.c
 *  b2r  hardware interfaces
 *  purely hardware related functions, include crash detection routines,
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_common.h"
#include "x_os.h"
#include "x_util.h"
#include "x_bim.h"
#include "b2r_hal.h"
#include "b2r_hw_v1.h"
#ifdef B2R_SUPPORT_V2HW
#include "b2r_hw_v2.h"
#endif
#include "b2r_debug.h"
#include "vdp_hw.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define B2R_INSIDE_ACTIVE_REGION (10)   // <90% for Active Region

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define HANDLE_TO_OBJECT(h,o)    ((o*)(h))
#define _B2R_HAL_MEMBER_OFFSET(type, member)  ((UINT32)(&(((type*)0)->member)))

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
B2R_HAL_OBJECT_T * atObj[4];
#define B2R_OBJECT_NS (sizeof(atObj)/sizeof(atObj[0]))


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static UINT32 _B2rHalSet(B2R_HAL_OBJECT_T *this, B2R_HAL_TYPE_T eType, VOID *pvArg)
{
    UINT32 u4Ret = B2R_HAL_OK;

    do {

        if (NULL == this || NULL == pvArg)
        {
            u4Ret = B2R_MK_RET(B2R_HAL_INV_ARG);
            break;
        }

        switch(eType)
        {
        default:
            u4Ret = B2R_HAL_NOT_IMPL;
            break;
        case B2R_HAL_3DTYPE:
        {
            B2R_HAL_3D_T *ptArg = (B2R_HAL_3D_T *)pvArg;
            this->t3D.fg3D     = ptArg->fg3D;
            this->t3D.u4Layout = ptArg->u4Layout;
            this->t3D.u4Data   = ptArg->u4Data;
        }
            break;

        case B2R_HAL_TIMING:
        {
            B2R_HAL_TIMING_T * ptArg = (B2R_HAL_TIMING_T *)pvArg;
            this->tTiming.u4SrcW   = ptArg->u4SrcW;
            this->tTiming.u4SrcH   = ptArg->u4SrcH;
            this->tTiming.u4HTotal = ptArg->u4HTotal;
            this->tTiming.u4VTotal = ptArg->u4VTotal;
            this->tTiming.u4Clock  = ptArg->u4Clock;
            this->tTiming.u4Rate   = ptArg->u4Rate;
            this->tTiming.u4Scale  = ptArg->u4Scale;
            this->tTiming.fgProg   = ptArg->fgProg;
        }
            break;

        case B2R_HAL_DRAM_LAYOUT:
        {
            B2R_HAL_DRAM_LAYOUT_T *ptArg = (B2R_HAL_DRAM_LAYOUT_T *)pvArg;
            this->tDramLayout.eAddrMode  = ptArg->eAddrMode;
            this->tDramLayout.eSwapMode  = ptArg->eSwapMode;
            this->tDramLayout.ePackMode  = ptArg->ePackMode;
            this->tDramLayout.u4Pitch    = ptArg->u4Pitch;
        }
            break;

        case B2R_HAL_COLOR_FMT:
        {
            B2R_HAL_COLOR_FMT_T *ptArg = (B2R_HAL_COLOR_FMT_T *)pvArg;
            this->tColorFmt.u1Depth  = ptArg->u1Depth;
            this->tColorFmt.eMode    = ptArg->eMode;
            this->tColorFmt.fgUfoEn    = ptArg->fgUfoEn;
        }
            break;
        }        
    
    }while(0);

    B2R_HAL_TRACE("%s, %s, arg(%d), code=0x%08x\n", __FUNCTION__, B2R_RET(u4Ret) == B2R_HAL_OK?"ok":"fail", eType, u4Ret);

    return B2R_RET(u4Ret);
}


static UINT32 _B2rHalGet(B2R_HAL_OBJECT_T *this, B2R_HAL_GET_TYPE_T eType, VOID *pvArg)
{
    UINT32 u4Ret = B2R_HAL_OK;

    do {

        if (NULL == this || NULL == pvArg)
        {
            u4Ret = B2R_MK_RET(B2R_HAL_INV_ARG);
            break;
        }

        switch(eType)
        {
            case B2R_HAL_GET_TIMING:
            {
                B2R_HAL_TIMING_T *ptArg = (B2R_HAL_TIMING_T *)pvArg;
                x_memcpy(ptArg, &this->tTiming, sizeof(B2R_HAL_TIMING_T));
                break;
            }
            
            case B2R_HAL_GET_COLOR_FMT:
            {
                B2R_HAL_COLOR_FMT_T *ptArg = (B2R_HAL_COLOR_FMT_T *)pvArg;
                x_memcpy(ptArg, &this->tColorFmt, sizeof(B2R_HAL_COLOR_FMT_T));
                break;
            }
            
        default:
            u4Ret = B2R_HAL_NOT_IMPL;
            break;
        }        
    }while(0);

    B2R_HAL_TRACE("%s, %s, arg(%d), code=0x%08x\n", __FUNCTION__, B2R_RET(u4Ret) == B2R_HAL_OK?"ok":"fail", eType, u4Ret);

    return B2R_RET(u4Ret);
}

static UINT32 _B2rHalIs(B2R_HAL_OBJECT_T *this, B2R_HAL_IS_TYPE_T eType, VOID *pvArg)
{
    UINT32 u4Ret = B2R_HAL_OK;

    do {

        if (NULL == this || NULL == pvArg)
        {
            u4Ret = B2R_MK_RET(B2R_HAL_INV_ARG);
            break;
        }

        switch(eType)
        {
        case B2R_HAL_IN_BLANK_REGION:
        {
            B2R_HAL_STATUS_T tStatus;
            UINT32 *ptArg = (UINT32 *)pvArg;
            
            this->Get(this, B2R_HAL_STATUS, &tStatus);
            if (!(tStatus.u4LineCnt < this->u4InsideVActRegion))
            {
                u4Ret = B2R_MK_RET(B2R_HAL_FAIL);
            }
            *ptArg = tStatus.u4LineCnt;
        }
            break;
        case B2R_HAL_ENABLE_ST:
            if (!this->fgEnable)
            {
                u4Ret = B2R_HAL_FAIL;
            }
            break;    
        default:
            u4Ret = B2R_HAL_NOT_IMPL;
            break;
        }        
    }while(0);

    B2R_HAL_TRACE("%s, %s, arg(%d), code=0x%08x\n", __FUNCTION__, B2R_RET(u4Ret) == B2R_HAL_OK?"ok":"fail", eType, u4Ret);

    return B2R_RET(u4Ret);
}


VOID _B2rHalIsr(UINT16 u2Vector)
{
    UINT32 i;
    B2R_HAL_OBJECT_T *this;

    for (i = 0; i < B2R_OBJECT_NS; i++)
    {
        this = atObj[i];
        if (this && this->Enable)
        {
            this->Isr(u2Vector);
        }
    }
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------
UINT32 _B2R_HAL_SetFrc(B2R_HAL_OBJECT_T *this, UCHAR u2TargetNs, UCHAR u2ChangeFieldNs, UCHAR ucMode)
{
    UINT32 u4Ret = B2R_HAL_OK;
    B2R_HAL_FRC_T *ptFrc;

    do 
    {
        if (!this || (u2TargetNs < u2ChangeFieldNs))
        {
           u4Ret = B2R_HAL_INV_ARG;
           break;
        }

        ptFrc = &this->tFrc;

        if (this->tTiming.fgProg)
        {
            // Progressive mode, no FRC required
            ptFrc->ucFrcEn = 0;
            ptFrc->eRepeatField = B2R_FRC_FIELD_MAX;
            break;
        }
        
        if (ucMode == B2R_FRC_NONE)
        {
            // wrong field case
            ptFrc->ucFrcEn = 0;
            ptFrc->eRepeatField = B2R_FRC_FIELD_MAX;
        }
        else if ((ucMode == B2R_FRC_NORMAL) &&
                 ((u2TargetNs == 0) || (u2TargetNs == 3) ||
                  ((u2TargetNs == 2) && (u2ChangeFieldNs == 1)) ||
                  ((u2TargetNs == 4) && (u2ChangeFieldNs == 2))))
        {
            // Normal: Top + Bottom (Single Field)
    
            ptFrc->ucFrcEn = 0;
            ptFrc->eRepeatField = B2R_FRC_FIELD_MAX;
        }
        else if (ucMode == B2R_FRC_CANCEL)
        {
            //todo 
        }
        else
        {
            ptFrc->ucFrcEn = 1;
            #ifdef VDP_FRC_WA2
            u2ChangeFieldNs = u2TargetNs;
            #endif
            
            if (ucMode == B2R_FRC_STOP_AUTO)
            {
                //copy bottom field to top field WA
                u2ChangeFieldNs = u2TargetNs;
            }
    
            ptFrc->ucCurrentNs = 0;
            ptFrc->u2TargetNs = u2TargetNs;
            ptFrc->u2ChangeFieldNs = u2ChangeFieldNs;
            ptFrc->ucMode = ucMode;
    
        }

    }while (0);


    return u4Ret;

}


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
UINT32 B2R_HAL_Reset(B2R_HANDLE_T hB2R)
{

    UINT32 u4Ret = B2R_HAL_OK;
    B2R_HAL_OBJECT_T *this = NULL;

    do {
        UINT32 low, high;

        if (B2R_NULL_HANDLE == hB2R)
        {
            u4Ret = B2R_MK_RET(B2R_HAL_INV_ARG);
            break;
        }
        this = HANDLE_TO_OBJECT(hB2R,B2R_HAL_OBJECT_T);
        ASSERT(this->Reset);

        low  = _B2R_HAL_MEMBER_OFFSET(B2R_HAL_OBJECT_T, begin);
        high = _B2R_HAL_MEMBER_OFFSET(B2R_HAL_OBJECT_T, end);

        ASSERT(low < high);
        x_memset(((VOID *)this) + low, 0, high-low);

        this->Reset(this);
    
    }while(0);

    B2R_HAL_TRACE("%s, %s, code=0x%08x\n", __FUNCTION__, B2R_RET(u4Ret) == B2R_HAL_OK?"ok":"fail", u4Ret);
    
    return B2R_RET(u4Ret);
}

UINT32 B2R_HAL_Enable(B2R_HANDLE_T hB2R)
{

    UINT32 u4Ret = B2R_HAL_OK;
    B2R_HAL_OBJECT_T *this = NULL;

    do {

        if (B2R_NULL_HANDLE == hB2R)
        {
            u4Ret = B2R_MK_RET(B2R_HAL_INV_ARG);
            break;
        }

        this = HANDLE_TO_OBJECT(hB2R,B2R_HAL_OBJECT_T);

        this->Enable(this);
        this->fgEnable = TRUE;
    
    }while(0);

    B2R_HAL_TRACE("%s, %s, code=0x%08x\n", __FUNCTION__, B2R_RET(u4Ret) == B2R_HAL_OK?"ok":"fail", u4Ret);
    
    return B2R_RET(u4Ret);
}

UINT32 B2R_HAL_Disable(B2R_HANDLE_T hB2R)
{

    UINT32 u4Ret = B2R_HAL_OK;
    B2R_HAL_OBJECT_T *this = NULL;

    do {

        if (B2R_NULL_HANDLE == hB2R)
        {
            u4Ret = B2R_MK_RET(B2R_HAL_INV_ARG);
            break;
        }

        this = HANDLE_TO_OBJECT(hB2R,B2R_HAL_OBJECT_T);

        this->fgEnable = FALSE;
        this->Disable(this);
      //  NB2R_REG_VB1(0x34010,0x00);//always reset vb1 regsister when B2R will be closed 
    }while(0);

    B2R_HAL_TRACE("%s, %s, code=0x%08x\n", __FUNCTION__, B2R_RET(u4Ret) == B2R_HAL_OK?"ok":"fail", u4Ret);
    
    return B2R_RET(u4Ret);
}


UINT32 B2R_HAL_Config(B2R_HANDLE_T hB2R)
{

    UINT32 u4Ret = B2R_HAL_OK;
    B2R_HAL_OBJECT_T *this = NULL;

    do {

        if (B2R_NULL_HANDLE == hB2R)
        {
            u4Ret = B2R_MK_RET(B2R_HAL_INV_ARG);
            break;
        }

        this = HANDLE_TO_OBJECT(hB2R,B2R_HAL_OBJECT_T);

        if (this->tTiming.u4Clock == 0)
        {
            UINT64 u8Tmp;
            B2R_HAL_TIMING_T *pt = &this->tTiming;
            u8Tmp = pt->u4HTotal * pt->u4VTotal;
            u8Tmp = u8Tmp * pt->u4Rate;
            if(0 != pt->u4Scale)
            {
                u8Tmp = u8Div6432(u8Tmp, pt->u4Scale, NULL);
            }
            pt->u4Clock = (UINT32)u8Tmp;
            if (this->t3D.fg3D)
            {
                pt->u4Clock = pt->u4Clock << 1;
            }
        }

        this->u4InsideVActRegion = ((this->tTiming.u4SrcH * (100 - B2R_INSIDE_ACTIVE_REGION)) / 100);
        if (!this->tTiming.fgProg)
        {
            this->u4InsideVActRegion >>= 1;
        }

        this->Config(this);
    
    }while(0);

    B2R_HAL_TRACE("%s, %s, code=0x%08x\n", __FUNCTION__, B2R_RET(u4Ret) == B2R_HAL_OK?"ok":"fail", u4Ret);
    
    return B2R_RET(u4Ret);
}


UINT32 B2R_HAL_Set(B2R_HANDLE_T hB2R, B2R_HAL_TYPE_T eType, VOID *pvArg)
{

    UINT32 u4Ret = B2R_HAL_OK;
    B2R_HAL_OBJECT_T *this = NULL;
    do {

        if (B2R_NULL_HANDLE == hB2R || NULL == pvArg)
        {
            u4Ret = B2R_MK_RET(B2R_HAL_INV_ARG);
            break;
        }

        this = HANDLE_TO_OBJECT(hB2R,B2R_HAL_OBJECT_T);
        
        ASSERT(this->Set);

        u4Ret = this->Set(this, eType, pvArg);
        
        if (u4Ret == B2R_HAL_NOT_IMPL)
        {
            u4Ret =  _B2rHalSet(this, eType, pvArg);
            u4Ret = B2R_MK_RET(u4Ret);        
        }
        else
        {
            u4Ret = B2R_MK_RET(u4Ret);        
        }
    }while(0);
   
    B2R_HAL_TRACE("%s, %s, arg(%d), code=0x%08x\n", __FUNCTION__, B2R_RET(u4Ret) == B2R_HAL_OK?"ok":"fail", eType, u4Ret);
    
    return B2R_RET(u4Ret);
}

UINT32 B2R_HAL_Get(B2R_HANDLE_T hB2R, B2R_HAL_GET_TYPE_T eType, VOID *pvArg)
{

    UINT32 u4Ret = B2R_HAL_OK;
    B2R_HAL_OBJECT_T *this = NULL;

    do {

        if (B2R_NULL_HANDLE == hB2R || NULL == pvArg)
        {
            u4Ret = B2R_MK_RET(B2R_HAL_INV_ARG);
            break;
        }

        this = HANDLE_TO_OBJECT(hB2R,B2R_HAL_OBJECT_T);

        ASSERT(this->Get);

        u4Ret = this->Get(this, eType, pvArg);
        
        if (u4Ret == B2R_HAL_NOT_IMPL)
        {
            u4Ret =  _B2rHalGet(this, eType, pvArg);
            u4Ret = B2R_MK_RET(u4Ret);        
        }
        else
        {
            u4Ret = B2R_MK_RET(u4Ret);        
        }
    
    }while(0);

    B2R_HAL_TRACE("%s, %s, arg(%d), code=0x%08x\n", __FUNCTION__, B2R_RET(u4Ret) == B2R_HAL_OK?"ok":"fail", eType, u4Ret);
    
    return B2R_RET(u4Ret);
}


BOOL B2R_HAL_Is(B2R_HANDLE_T hB2R, B2R_HAL_IS_TYPE_T eType, VOID *pvArg)
{
    UINT32 u4Ret = B2R_HAL_OK;
    B2R_HAL_OBJECT_T *this = NULL;

    do {

        if (B2R_NULL_HANDLE == hB2R || NULL == pvArg)
        {
            u4Ret = B2R_MK_RET(B2R_HAL_INV_ARG);
            break;
        }

        this = HANDLE_TO_OBJECT(hB2R,B2R_HAL_OBJECT_T);

        u4Ret =  _B2rHalIs(this, eType, pvArg);
    
    }while(0);

    B2R_HAL_TRACE("%s, %s, arg(%d), code=0x%08x\n", __FUNCTION__, B2R_RET(u4Ret) == B2R_HAL_OK?"ok":"fail", eType, u4Ret);
    
    return (B2R_RET(u4Ret) == B2R_HAL_OK) ? TRUE : FALSE;
}


UINT32 B2R_HAL_Close(B2R_HANDLE_T hB2R)
{
    UINT32 u4Ret = B2R_HAL_OK;
    B2R_HAL_OBJECT_T *this = NULL;

    do {

        if (B2R_NULL_HANDLE == hB2R)
        {
            u4Ret = B2R_MK_RET(B2R_HAL_INV_ARG);
            break;
        }

        this = HANDLE_TO_OBJECT(hB2R,B2R_HAL_OBJECT_T);
        this->u1Used = 0;
        this->pvHost = NULL;
    
    }while(0);

    B2R_HAL_TRACE("%s, %s, arg(%d), code=0x%08x\n", __FUNCTION__, B2R_RET(u4Ret) == B2R_HAL_OK?"ok":"fail", u4Ret);
    
    return B2R_RET(u4Ret);

}

B2R_HANDLE_T B2R_HAL_Open(VOID* pvArg)
{
    B2R_HANDLE_T hB2r;
    UINT32 u4Arg;
    u4Arg = *(UINT32 *)pvArg;

    //just for emu test
    switch (u4Arg)
    {
    case 0x10: //old hd b2r
    case 0x11: //old sd b2r
    case 0x12: //new hd b2r
    case 0x13: //new sd b2r
    {
        typedef struct
        {
            UINT32 u4Data;
            VOID * pvData;
        } B2R_CfG_T;
        B2R_CfG_T *ptCfg = (B2R_CfG_T *)pvArg;
        hB2r = (B2R_HANDLE_T) atObj[(u4Arg & 0x0F)];
        atObj[(u4Arg & 0x0F)]->pvHost = ptCfg->pvData;
        atObj[(u4Arg & 0x0F)]->u1Used = 1;
        B2R_HAL_TRACE("emu open, %s\n", atObj[(u4Arg & 0x0F)]->acName);
    }
        break;
    case 0xFFFFFFFC://New B2R 
    case 0xFFFFFFFD://New B2R 
    case 0xFFFFFFFE://old B2R SD
    case 0xFFFFFFFF://old B2R HD
    {
        B2R_ICfG_T *ptCfg = (B2R_ICfG_T *)pvArg;
#ifndef NEW_B2R_SINGLE_TEST
        UINT32 id = (0xFFFFFFFF - u4Arg);
#else
        UINT32 id = (0xFFFFFFFD - u4Arg);
#endif
        hB2r = (B2R_HANDLE_T) atObj[id]; //just for debug
        atObj[id]->pvHost = ptCfg->pvData;
        atObj[id]->u1Used = 1;
        B2R_HAL_TRACE("b2r hal open, %s\n", atObj[id]->acName);
        break;
    }
    default:
        //todo 
        hB2r = B2R_NULL_HANDLE;
    }

    return hB2r;
}

VOID B2R_HAL_Init(VOID)
{
    static BOOL fgInit = 0;
#ifdef NEW_B2R_SINGLE_TEST
    B2R_HAL_OBJECT_T* pr_swap = NULL;
#endif

    if (!fgInit)
    {
        UINT8 i,j;
        for (i = 0; i < B2R_OBJECT_NS; i++)
        {
            atObj[i] = NULL;
        }

        B2R_HW_V1_Init();
        for (i = 0, j = 0; i < B2R_OBJECT_NS; i++,j++)
        {
            if (!B2R_HW_V1_Get_Object(j))
            {
                break;
            }
            atObj[i] = B2R_HW_V1_Get_Object(j);
        }

        #ifdef B2R_SUPPORT_V2HW
        B2R_HW_V2_Init();
        for (j = 0; i < B2R_OBJECT_NS; i++,j++)
        {
            //TODO hw v2
            if (!B2R_HW_V2_Get_Object(j))
            {
                break;
            }
            atObj[i] = B2R_HW_V2_Get_Object(j);
        }
        #endif

#ifdef NEW_B2R_SINGLE_TEST
        for(i = 0; i < B2R_OBJECT_NS/2; i++)
        {
            pr_swap    = atObj[i];
            atObj[i]   = atObj[i+2];
            atObj[i+2] = pr_swap;
        }
#endif
        //register isr
        if (1) //todo
        {
            x_os_isr_fct pfnOldIsr;
            VERIFY(x_reg_isr(VECTOR_DISPLAY, _B2rHalIsr, &pfnOldIsr) == OSR_OK);
        }
        
        fgInit = TRUE;
    }

    return;
}






