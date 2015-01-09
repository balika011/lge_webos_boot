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
 * $RCSfile: osd_region_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_region_if.c
 *  This header file includes public function definitions of OSD region.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_timer.h"
#include "osd_hw.h"
#include "osd_drvif.h"
#include "osd_debug.h"
#ifndef CC_MTK_LOADER
#include "gfx_drvif.h"
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define OSD_INVALID_REGION_HANDLE -1
#define OSD_RGN_NODE_ALLOCATED 1
#define OSD_RGN_LIST_ALLOCATED 1

#define CHECK_FAIL(stmt, exp_ret) \
do {\
    int _____ret = (stmt);\
    if(_____ret != (exp_ret))\
    {\
        printf("CHECK FAILED %s ret:%d\n", #stmt, _____ret);\
    }\
}while(0)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _OSD_REGION_NODE_T
{
    UINT32 u4NodeStatus;
    INT32 i4Prev, i4Next;
    UINT32 u4Original_X;
    UINT32 u4Original_Y;
    UINT32 u4Original_OutW;
    UINT32 u4Original_OutH;
    UINT32 u4Original_InW;
    UINT32 u4Original_InH;  
    UINT32 u4DecEn;
} OSD_REGION_NODE_T;

typedef struct _OSD_REGION_LIST_T
{
    UINT32 u4ListStatus;
    INT32 i4Head, i4Tail;
    INT32 i4Count;
} OSD_REGION_LIST_T;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static OSD_REGION_NODE_T _rAllRgnNode[OSD_MAX_NUM_RGN];
static OSD_REGION_LIST_T _rAllRgnList[OSD_MAX_NUM_RGN_LIST];
static OSD_SCALER_DATA_T _rAllRgnListScaler[OSD_MAX_NUM_RGN_LIST];

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//add for setting/getting the region list used by linux frame buffer
static UINT32 u4RGNList_FrameBuffer = OSD_MAX_NUM_RGN_LIST;

//for scroll osd
static UINT32 _u4ScrollRgn;
static UINT32 _u4BaseAddr;
static UINT32 _u4ScrollPitch;
static UINT32 _u4ScrollStep;
static UINT8 _u1ScrollMode = 0;

#ifndef CC_MTK_LOADER
#ifdef CC_SUPPORT_STR
extern BOOL pb_osd_enable[OSD_PLANE_MAX_NUM];
#endif
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _OsdRegionMirrorClipJudge(UINT32 RgnWidth)
{
    if(RgnWidth%8)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
static UINT32 _OsdRegionMirrorClipValue(UINT32 RgnWidth)
{
    return ((RgnWidth+0x7)&(~0x7))-RgnWidth;
}

static INT32 _OsdRegionCompare(UINT32 u4Rgn1, UINT32 u4Rgn2)
{
    UINT32 u4DispY1, u4DispH1, u4DispB1;
    UINT32 u4DispY2, u4DispH2, u4DispB2;
    INT32 i4Ignore;

    i4Ignore = _OSD_RGN_GetOutputPosY(u4Rgn1, &u4DispY1);
    i4Ignore = _OSD_RGN_GetOutputHeight(u4Rgn1, &u4DispH1);
    u4DispB1 = u4DispY1 + u4DispH1;
    i4Ignore = _OSD_RGN_GetOutputPosY(u4Rgn2, &u4DispY2);
    i4Ignore = _OSD_RGN_GetOutputHeight(u4Rgn2, &u4DispH2);
    u4DispB2 = u4DispY2 + u4DispH2;

    UNUSED(i4Ignore);

    if ((u4DispB1 >= u4DispY2) && (u4DispY1 >= u4DispB2))
    {
        return 1; // region 1 > region 2
    }
    else if ((u4DispB2 >= u4DispY1) && (u4DispY2 >= u4DispB1))
    {
        return -1; // region 2 > region 1
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

INT32 OSD_RGN_LIST_Init(void)
{
    INT32 i4Index;
    for (i4Index = 0; i4Index < OSD_MAX_NUM_RGN_LIST; i4Index++)
    {
        _rAllRgnList[i4Index].u4ListStatus = 0;
        _rAllRgnList[i4Index].i4Head = OSD_INVALID_REGION_HANDLE;
        _rAllRgnList[i4Index].i4Tail = OSD_INVALID_REGION_HANDLE;
        _rAllRgnList[i4Index].i4Count = 0;
    }
    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_Init(void)
{
    INT32 i4Index;
    for (i4Index = 0; i4Index < OSD_MAX_NUM_RGN; i4Index++)
    {
        _rAllRgnNode[i4Index].u4NodeStatus = 0;
        _rAllRgnNode[i4Index].i4Prev = OSD_INVALID_REGION_HANDLE;
        _rAllRgnNode[i4Index].i4Next = OSD_INVALID_REGION_HANDLE;
        _rAllRgnNode[i4Index].u4DecEn = 0;
    }
    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_LIST_SCALER_Init(void)
{
    x_memset(&_rAllRgnListScaler, 0xff, sizeof(_rAllRgnListScaler));
    
    return (INT32) OSD_RET_OK;
}

OSD_SCALER_DATA_T* OSD_RGN_LIST_SCALER_Get(UINT32 u4_idx)
{
    if( u4_idx >= (UINT32)OSD_MAX_NUM_RGN_LIST) 
    { 
        return NULL; 
    } 
    
    return &_rAllRgnListScaler[u4_idx];
}

INT32 OSD_RGN_LIST_SCALER_Set(UINT32 u4_idx,OSD_SCALER_DATA_T* pt_this)
{
    OSD_VERIFY_RGNLIST(u4_idx);
    
    if (pt_this == NULL)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }
    
    x_memcpy(&_rAllRgnListScaler[u4_idx],pt_this,sizeof(OSD_SCALER_DATA_T));   
   
    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_LIST_Get(UINT32 u4RgnList, INT32 i4Cmd, UINT32 * pu4Value)
{
    OSD_VERIFY_RGNLIST(u4RgnList);

    if (pu4Value == NULL)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }

    switch (i4Cmd)
    {
    case OSD_RGN_LIST_HEAD:
        *pu4Value = (UINT32) _rAllRgnList[u4RgnList].i4Head;
        break;

    case OSD_RGN_LIST_TAIL:
        *pu4Value = (UINT32) _rAllRgnList[u4RgnList].i4Tail;
        break;

    case OSD_RGN_LIST_COUNT:
        *pu4Value = (UINT32) _rAllRgnList[u4RgnList].i4Count;
        break;

    case OSD_LIST_FLAGS:
        *pu4Value = (UINT32) _rAllRgnList[u4RgnList].u4ListStatus;
        break;

    default:
        return -(INT32) OSD_RET_INV_ARG;
    }
    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_LIST_Set(UINT32 u4RgnList, INT32 i4Cmd, UINT32 u4Value)
{
    OSD_VERIFY_RGNLIST(u4RgnList);

    switch (i4Cmd)
    {
    case OSD_RGN_LIST_HEAD:
        if (u4Value >= OSD_MAX_NUM_RGN)
        {
            return -(INT32) OSD_RET_INV_REGION;
        }
        _rAllRgnList[u4RgnList].i4Head = (INT32) u4Value;
        break;

    case OSD_RGN_LIST_TAIL:
        if (u4Value >= OSD_MAX_NUM_RGN)
        {
            return -(INT32) OSD_RET_INV_REGION;
        }
        _rAllRgnList[u4RgnList].i4Tail = (INT32) u4Value;
        break;

    case OSD_RGN_LIST_COUNT:
        _rAllRgnList[u4RgnList].i4Count = (INT32) u4Value;
        break;

    case OSD_LIST_FLAGS:
        _rAllRgnList[u4RgnList].u4ListStatus = (UINT32) u4Value;
        break;

    default:
        return -(INT32) OSD_RET_INV_ARG;
    }
    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** create an osd list
 *  @param pu4RgnList list id pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_LIST_Create(UINT32 * pu4RgnList)
{
    INT32 i4Index;

    if (pu4RgnList == NULL)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }

    //[KLUDGE]
    //Because OSD_MAX_NUM_RGN_LIST is only 20 or 30,
    //a linear search might be acceptable here.
    //Creation of a region wont happen so often,
    //hence even we have hundreds of region list,
    //it should be ok here.
    for (i4Index = 0; i4Index < OSD_MAX_NUM_RGN_LIST; i4Index++)
    {
        if ((_rAllRgnList[i4Index].u4ListStatus & OSD_RGN_LIST_ALLOCATED) == 0)
        {
            // free cell found, allocate this list, init and return it
            _rAllRgnList[i4Index].u4ListStatus |= OSD_RGN_LIST_ALLOCATED;
            _rAllRgnList[i4Index].i4Head = OSD_INVALID_REGION_HANDLE;
            _rAllRgnList[i4Index].i4Tail = OSD_INVALID_REGION_HANDLE;
            _rAllRgnList[i4Index].i4Count = 0;
            *pu4RgnList = (UINT32) i4Index;
            return (INT32) OSD_RET_OK;
        }
    }

    return -(INT32) OSD_RET_OUT_OF_LIST;
}

//-----------------------------------------------------------------------------
/** delete an osd list
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_LIST_Delete(UINT32 u4RgnList)
{
    OSD_VERIFY_RGNLIST(u4RgnList);

    _rAllRgnList[u4RgnList].u4ListStatus &= ~OSD_RGN_LIST_ALLOCATED;
    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** get first region id of list
 *  @param  u4RgnList list id
 *  @param  pi4HeadRegion region id pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_LIST_GetHead(UINT32 u4RgnList, INT32 * pi4HeadRegion)
{
    OSD_VERIFY_RGNLIST(u4RgnList);

    if (pi4HeadRegion == NULL)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }

    *pi4HeadRegion = _rAllRgnList[u4RgnList].i4Head;
    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_LIST_GetNext(UINT32 u4RgnList, INT32 i4RgnCurr, INT32 *pi4RgnNext)
{
    INT32 i4Ret;
    INT32 i4RgnNext = -1;

    OSD_VERIFY_RGNLIST(u4RgnList);

    if (pi4RgnNext == NULL)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }

    i4Ret = OSD_RGN_Get((UINT32) i4RgnCurr, (INT32) OSD_RGN_NEXT,
            (UINT32*) &i4RgnNext);
    if (i4Ret)
    {
        return i4Ret;
    }
    *pi4RgnNext = i4RgnNext;
    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_LIST_DetachAll(UINT32 u4RgnList)
{
    INT32 i4CurrRgn, i4NextRgn;

    OSD_VERIFY_RGNLIST(u4RgnList);

    i4CurrRgn = _rAllRgnList[u4RgnList].i4Head;

    while (i4CurrRgn != OSD_INVALID_REGION_HANDLE)
    {
        i4NextRgn = _rAllRgnNode[i4CurrRgn].i4Next;
        _rAllRgnNode[i4CurrRgn].i4Prev = OSD_INVALID_REGION_HANDLE;
        _rAllRgnNode[i4CurrRgn].i4Next = OSD_INVALID_REGION_HANDLE;
        _rAllRgnNode[i4CurrRgn].u4NodeStatus &= ~OSD_RGN_NODE_ALLOCATED;
        //VERIFY((INT32)OSD_RET_OK == _OSD_RGN_Free((UINT32)i4CurrRgn));
        i4CurrRgn = i4NextRgn;
    }

    _rAllRgnList[u4RgnList].i4Head = OSD_INVALID_REGION_HANDLE;
    _rAllRgnList[u4RgnList].i4Tail = OSD_INVALID_REGION_HANDLE;
    _rAllRgnList[u4RgnList].i4Count = 0;

    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_LIST_GetCount(UINT32 u4RgnList, INT32 * pi4Count)
{
    INT32 i4Count;
    INT32 i4Ret;

    if (pi4Count == NULL)
    {
        return (INT32) OSD_RET_INV_ARG;
    }

    i4Ret = OSD_RGN_LIST_Get(u4RgnList, (INT32) OSD_RGN_LIST_COUNT,
            (UINT32*) &i4Count);
    if (i4Ret)
    {
        return i4Ret;
    }

    *pi4Count = i4Count;
    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** get osd region attribute
 *  @param  u4Region region id
 *  @param  i4Cmd attribute name, refer to OSD_RGN_CMD_T
 *  @param  pu4Value attribute value pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Get(UINT32 u4Region, INT32 i4Cmd, UINT32 * pu4Value)
{
    if (pu4Value == NULL)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }
    OSD_VERIFY_REGION(u4Region);

    switch (i4Cmd)
    {
    case OSD_RGN_PREV:
        *pu4Value = (UINT32) _rAllRgnNode[u4Region].i4Prev;
        break;

    case OSD_RGN_NEXT:
        *pu4Value = (UINT32) _rAllRgnNode[u4Region].i4Next;
        break;

    case OSD_RGN_FLAGS:
        *pu4Value = _rAllRgnNode[u4Region].u4NodeStatus;
        break;

    case OSD_RGN_POS_X:
        return _OSD_RGN_GetOutputPosX(u4Region, pu4Value);

    case OSD_RGN_POS_Y:
        return _OSD_RGN_GetOutputPosY(u4Region, pu4Value);

    case OSD_RGN_BMP_W:
        return _OSD_RGN_GetInputWidth(u4Region, pu4Value);

    case OSD_RGN_BMP_H:
        return _OSD_RGN_GetInputHeight(u4Region, pu4Value);

    case OSD_RGN_DISP_W:
    case OSD_RGN_OUT_W:
        return _OSD_RGN_GetOutputWidth(u4Region, pu4Value);

    case OSD_RGN_DISP_H:
    case OSD_RGN_OUT_H:
        return _OSD_RGN_GetOutputHeight(u4Region, pu4Value);

    case OSD_RGN_COLORMODE:
        return _OSD_RGN_GetColorMode(u4Region, pu4Value);

    case OSD_RGN_ALPHA:
        return _OSD_RGN_GetAlpha(u4Region, pu4Value);

    case OSD_RGN_BMP_ADDR:
        return _OSD_RGN_GetDataAddr(u4Region, pu4Value);

    case OSD_RGN_BMP_PITCH:
    {
        UINT32 u4LineSize;
    #if !defined(CC_MT5399) && !defined(CC_MT5890) && !defined(CC_MT5882)
        UINT32 u4LineSize9;
    #endif
        IGNORE_RET(_OSD_RGN_GetLineSize(u4Region, &u4LineSize));
    #if !defined(CC_MT5399) && !defined(CC_MT5890) && !defined(CC_MT5882)
        IGNORE_RET(_OSD_RGN_GetLineSize9(u4Region, &u4LineSize9));
    #endif
    #if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        *pu4Value = (u4LineSize << 4);
    #else
        *pu4Value = (u4LineSize9 << 13) | (u4LineSize << 4);
    #endif
        break;
    }

    case OSD_RGN_CLIP_V:
        return _OSD_RGN_GetVClip(u4Region, pu4Value);

    case OSD_RGN_CLIP_H:
        return _OSD_RGN_GetHClip(u4Region, pu4Value);

    case OSD_RGN_PAL_LOAD:
        return _OSD_RGN_GetLoadPalette(u4Region, pu4Value);

    case OSD_RGN_PAL_ADDR:
        return _OSD_RGN_GetPaletteAddr(u4Region, pu4Value);

    case OSD_RGN_PAL_LEN:
        return _OSD_RGN_GetPaletteLen(u4Region, pu4Value);

    case OSD_RGN_STEP_V:
        return _OSD_RGN_GetVStep(u4Region, pu4Value);

    case OSD_RGN_STEP_H:
        return _OSD_RGN_GetHStep(u4Region, pu4Value);

    case OSD_RGN_COLOR_KEY:
        return _OSD_RGN_GetColorKey(u4Region, pu4Value);

    case OSD_RGN_COLOR_KEY_EN:
        return _OSD_RGN_GetColorKeyEnable(u4Region, pu4Value);

    case OSD_RGN_MIX_SEL:
        return _OSD_RGN_GetBlendMode(u4Region, pu4Value);

    case OSD_RGN_V_FLIP:
        return _OSD_RGN_GetVFlip(u4Region, pu4Value);

    case OSD_RGN_H_MIRROR:
        return _OSD_RGN_GetHMirror(u4Region, pu4Value);

    case OSD_RGN_DEC_MODE:
        return _OSD_RGN_GetDecMode(u4Region, pu4Value);
    case OSD_RGN_DEC_LINE:
        return _OSD_RGN_GetDecLine(u4Region, pu4Value);
    case OSD_RGN_DEC_EN:
        return _OSD_RGN_GetDecEn(u4Region, pu4Value);

    case OSD_RGN_ALPHA_SEL:
        return _OSD_RGN_GetASel(u4Region, pu4Value);
    case OSD_RGN_YR_SEL:
        return _OSD_RGN_GetYrSel(u4Region, pu4Value);
    case OSD_RGN_UG_SEL:
        return _OSD_RGN_GetUgSel(u4Region, pu4Value);
    case OSD_RGN_NEXT_EN:
        return _OSD_RGN_GetNextEnable(u4Region, pu4Value);
    case OSD_RGN_NEXT_HDR_ADDR:
        return _OSD_RGN_GetNextRegion(u4Region, pu4Value);
    default:
        return -(INT32) OSD_RET_INV_ARG;
    }

    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** set osd region attribute
 *  @param  u4Region region id
 *  @param  i4Cmd attribute name, refer to OSD_RGN_CMD_T
 *  @param  u4Value attribute value
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
void OSD_RGN_Set_Bmp_Addr_Wait(UINT32 u4Plane, UINT32 u4Value)
{
    UINT32 u4RealBmpAddr    = 0x0;
    BOOL fgEnable           = 0x0;
    HAL_TIME_T          t_current,t_delta;
    HAL_TIME_T   t_last;
    UINT32              u4_delta_time = 0x0;

    if (u4Plane >= OSD_PLANE_MAX_NUM)
    {
        return;
    }

    OSD_PLA_GetEnable(u4Plane, &fgEnable);
    HAL_GetTime(&t_last);
    while (fgEnable && ((OSD_READ32((u4Plane+1)*0x100)&0x1)))
    {
        _OSD_PLA_Get_Hw_RealBmpAddr(u4Plane, &u4RealBmpAddr);
        if (u4Value == u4RealBmpAddr)
        {
            break;
        }
        HAL_GetTime(&t_current);
        HAL_GetDeltaTime(&t_delta,&t_last,&t_current);
        u4_delta_time = ((t_delta.u4Seconds*1000000) + t_delta.u4Micros);
        if(u4_delta_time > 1000000)
        {
            printf("OSD Plane update timeout\n");
            OSD_WRITE32(0x4, 0x12);
            OSD_WaitVsync();
            OSD_WRITE32(0x4, 0x13);
            _OSD_UpdateReg();       
            break;
        }
    }

    return;
}

//-----------------------------------------------------------------------------
/** set osd region attribute
 *  @param  u4Region region id
 *  @param  i4Cmd attribute name, refer to OSD_RGN_CMD_T
 *  @param  u4Value attribute value
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
void OSD_RGN_Set_Clip_H_Wait(UINT32 u4Plane, UINT32 u4Value)
{
    UINT32 u4RealData = 0x0;
    BOOL fgEnable = 0x0;

    if (u4Plane >= OSD_PLANE_MAX_NUM)
    {
        return;
    }

    OSD_PLA_GetEnable(u4Plane, &fgEnable);
    while (fgEnable)
    {
        (void) _OSD_PLA_Get_Hw_RealClipH(u4Plane, &u4RealData);

        if (u4Value == u4RealData)
        {
            break;
        }
    }

    return;
}

//-----------------------------------------------------------------------------
/** get osd region scaler
 *  @param  u4Region region id
 *  @param  u4Scaler scaker id,
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------

INT32 OSD_RGN_GetScaler(UINT32 u4Region, UINT32 *u4Scaler)
{
    INT32 u4Rgn = 0;

    do
    {
        u4Rgn = OSD_PLA_GetFirstRegion(OSD_PLANE_2);
        if(u4Rgn == u4Region)
        {
            *u4Scaler = OSD_SCALER_2;
            break;
        }

        u4Rgn = OSD_PLA_GetFirstRegion(OSD_PLANE_3);
        if(u4Rgn == u4Region)
        {
            *u4Scaler = OSD_SCALER_3;
            break;
        }
#ifdef OSD_PLA5_SUPPORT     
        u4Rgn = OSD_PLA_GetFirstRegion(OSD_PLANE_5);
        if(u4Rgn == u4Region)
        {
            *u4Scaler = OSD_SCALER_MAX_NUM;
            break;
        }
#endif
        return (INT32)OSD_RET_UNDEF_ERR;

    }
    while(0);

    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_TransformCood(UINT32 u4Region, UINT32 *retPosX, UINT32 *retPosY)
{
     UINT32 u4Scaler;
     UINT32 u4Mirror = 0;
     UINT32 u4PanelWidth =0;
     UINT32 u4Flip = 0;
     UINT32 u4PanelHeight =0;    
     UINT32 u4Tmp = 0;
     UINT32 u4ValueX = _rAllRgnNode[u4Region].u4Original_X;
     UINT32 u4ValueY = _rAllRgnNode[u4Region].u4Original_Y;

     if(_rAllRgnNode[u4Region].u4DecEn
        || OSD_RET_OK != OSD_RGN_GetScaler(u4Region, &u4Scaler))
     {
          goto TRANSFORMCOOD_END;
     }

     OSD_BASE_GetPanelSize(&u4PanelWidth, &u4PanelHeight);

     if(retPosX && OSD_GetCpblty(E_OSD_CPBLTY_MIRROR, 0x0))
     {
        _OSD_RGN_GetHMirror(u4Region, &u4Mirror);
        if(u4Mirror > 0)
        {
            if(u4Scaler < OSD_SCALER_MAX_NUM)
            {
                 _OSD_SC_GetSrcHSize(u4Scaler, &u4PanelWidth);
            }

            u4Tmp = _rAllRgnNode[u4Region].u4Original_OutW;
            if((u4Tmp + u4ValueX) < u4PanelWidth)
            {
                u4ValueX = u4PanelWidth - u4Tmp - u4ValueX;
            }
            else
            {
                if(u4Scaler == OSD_SCALER_MAX_NUM)//cursor 
                {
                    u4Tmp = (u4PanelWidth > u4ValueX) ? (u4PanelWidth - u4ValueX) : 1;
                    _OSD_RGN_SetOutputWidth(u4Region, u4Tmp);
                    _OSD_RGN_SetInputWidth(u4Region, u4Tmp);
                }            
                u4ValueX = 0;
            }
        }         
     }

     if(retPosY && OSD_GetCpblty(E_OSD_CPBLTY_FLIP, 0x0))
     {
        _OSD_RGN_GetVFlip(u4Region, &u4Flip);
        if(u4Flip > 0)
        {
            if(u4Scaler < OSD_SCALER_MAX_NUM)
            {
                 _OSD_SC_GetSrcVSize(u4Scaler, &u4PanelHeight);
            }

            u4Tmp = _rAllRgnNode[u4Region].u4Original_OutH;
            if((u4Tmp + u4ValueY) < u4PanelHeight)
            {
                u4ValueY = u4PanelHeight - u4Tmp - u4ValueY;
            }
            else
            {
                if(u4Scaler == OSD_SCALER_MAX_NUM)//cursor 
                {
                    u4Tmp = (u4PanelHeight > u4ValueY) ? (u4PanelHeight - u4ValueY) : 1;
                    _OSD_RGN_SetOutputHeight(u4Region, u4Tmp);
                    _OSD_RGN_SetInputHeight(u4Region, u4Tmp);
                }             
                u4ValueY = 0;
            }
        }    
     }

TRANSFORMCOOD_END:
    
     if(retPosX)
     {
         *retPosX = u4ValueX;
     }
     if(retPosY)
     {
         *retPosY = u4ValueY;
     }

     return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_MirrorFlipUpdate(UINT32 u4Region, BOOL updateMirror, BOOL updateFlip)
{
    UINT32 u4ValueX = 0, u4ValueY = 0;

    if(updateMirror && OSD_GetCpblty(E_OSD_CPBLTY_MIRROR, 0x0))
    {
         OSD_RGN_TransformCood(u4Region, &u4ValueX, NULL);
         _OSD_RGN_SetOutputPosX(u4Region, u4ValueX);
    }

    if(updateFlip && OSD_GetCpblty(E_OSD_CPBLTY_FLIP, 0x0))
    {
        OSD_RGN_TransformCood(u4Region, NULL, &u4ValueY);
        _OSD_RGN_SetOutputPosY(u4Region, u4ValueY);
    }

    return (INT32)OSD_RET_OK;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** set osd region attribute
 *  @param  u4Region region id
 *  @param  i4Cmd attribute name, refer to OSD_RGN_CMD_T
 *  @param  u4Value attribute value
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Set(UINT32 u4Region, INT32 i4Cmd, UINT32 u4Value)
{
    INT32 i4Ret = OSD_RET_OK;
    UINT32 u4Tmp = 0;
    //HAL_TIME_T          t_current,t_delta;
    //static HAL_TIME_T   t_last;
    //UINT32              u4_delta_time = 0x0;

    OSD_VERIFY_REGION(u4Region);

    switch (i4Cmd)
    {
    case OSD_RGN_PREV:
        _rAllRgnNode[u4Region].i4Prev = (INT32) u4Value;
        break;

    case OSD_RGN_NEXT:
        _rAllRgnNode[u4Region].i4Next = (INT32) u4Value;
        break;

    case OSD_RGN_FLAGS:
        _rAllRgnNode[u4Region].u4NodeStatus = u4Value;
        break;

    case OSD_RGN_POS_X:
#if LG_OSD_PATH_TEST
        if( u4Region == OSD_PLA_GetFirstRegion(OSD_PLANE_2))
        {
            OSD_BASE_SetFRCHeader(OSD_FRCHD_DISPXOFF,u4Value);
        }
#endif
        _rAllRgnNode[u4Region].u4Original_X = u4Value;
        OSD_RGN_TransformCood(u4Region, &u4Value, NULL);
      
        return _OSD_RGN_SetOutputPosX(u4Region, u4Value);

    case OSD_RGN_POS_Y:
#if LG_OSD_PATH_TEST
        if( u4Region == OSD_PLA_GetFirstRegion(OSD_PLANE_2))
        {
            OSD_BASE_SetFRCHeader(OSD_FRCHD_DISPYOFF,u4Value);
        }
#endif

        _rAllRgnNode[u4Region].u4Original_Y = u4Value;
        OSD_RGN_TransformCood(u4Region, NULL, &u4Value);

        return _OSD_RGN_SetOutputPosY(u4Region, u4Value);

    case OSD_RGN_BMP_W:
#if LG_OSD_PATH_TEST
        if( u4Region == OSD_PLA_GetFirstRegion(OSD_PLANE_2))
        {
            OSD_BASE_SetFRCHeader(OSD_FRCHD_BMPWIDTH,u4Value);
        }
#endif
        _rAllRgnNode[u4Region].u4Original_InW = u4Value;

        return _OSD_RGN_SetInputWidth(u4Region, u4Value);

    case OSD_RGN_BMP_H:
#if LG_OSD_PATH_TEST
        if( u4Region == OSD_PLA_GetFirstRegion(OSD_PLANE_2))
        {
            OSD_BASE_SetFRCHeader(OSD_FRCHD_BMPHEIGHT,u4Value);
        }
#endif
        _rAllRgnNode[u4Region].u4Original_InH = u4Value;

        return _OSD_RGN_SetInputHeight(u4Region, u4Value);

    case OSD_RGN_DISP_W:
        return OSD_RGN_SetDisplayWidth(u4Region, u4Value);

    case OSD_RGN_DISP_H:
        return OSD_RGN_SetDisplayHeight(u4Region, u4Value);

    case OSD_RGN_OUT_W:
#if LG_OSD_PATH_TEST
        if( u4Region == OSD_PLA_GetFirstRegion(OSD_PLANE_2))
        {
            OSD_BASE_SetFRCHeader(OSD_FRCHD_DISPWIDTH,u4Value);
        }
#endif
        _rAllRgnNode[u4Region].u4Original_OutW = u4Value;

        i4Ret = _OSD_RGN_SetOutputWidth(u4Region, u4Value);

        OSD_RGN_MirrorFlipUpdate(u4Region, TRUE, FALSE);

        return i4Ret;

    case OSD_RGN_OUT_H:
#if LG_OSD_PATH_TEST
        if( u4Region == OSD_PLA_GetFirstRegion(OSD_PLANE_2))
        {
            OSD_BASE_SetFRCHeader(OSD_FRCHD_DISPHEIGHT,u4Value);
        }
#endif
        _rAllRgnNode[u4Region].u4Original_OutH = u4Value;
 
        i4Ret = _OSD_RGN_SetOutputHeight(u4Region, u4Value);
        
        OSD_RGN_MirrorFlipUpdate(u4Region, FALSE, TRUE);

        return i4Ret;

    case OSD_RGN_COLORMODE:
#if LG_OSD_PATH_TEST
        if(u4Value == OSD_CM_ARGB8888_DIRECT32)
        {
            OSD_BASE_SetFRCHeader(OSD_FRCHD_FORMAT,4);
        }
        else if(u4Value == OSD_CM_AYCBCR8888_DIRECT32)
        {
            OSD_BASE_SetFRCHeader(OSD_FRCHD_FORMAT,5);
        }
#endif
        return _OSD_RGN_SetColorMode(u4Region, u4Value);

    case OSD_RGN_ALPHA:
#if LG_OSD_PATH_TEST
        if( u4Region == OSD_PLA_GetFirstRegion(OSD_PLANE_2))
        {
            OSD_BASE_SetFRCHeader(OSD_FRCHD_GLOBALALPHA,u4Value);
        }
#endif

        return _OSD_RGN_SetAlpha(u4Region, u4Value);

    case OSD_RGN_BMP_ADDR:
    {
#ifndef CC_MTK_LOADER
#ifdef CC_SUPPORT_STR
        INT32 i;
#endif
#endif
        UINT32 u4TempRgn;
        OSD_LockFlipTo();
        u4TempRgn = u4Region;
        i4Ret = _OSD_RGN_SetDataAddr(u4TempRgn, u4Value);
#if defined(GAL_DDI_SUPPORT)
        //if((IO_READ32(0xF0021108, 0) & 0xFF) == 0xE0)
          OSD_WaitVsync();
#endif
        OSD_UnLockFlipTo();
#ifndef CC_MTK_LOADER
#ifdef CC_SUPPORT_STR
        for(i = 0; i < OSD_PLANE_MAX_NUM; i++)
        {
            if(pb_osd_enable[i] == TRUE && u4Region == OSD_PLA_GetFirstRegion(i))
            {
                OSD_PLA_Enable(i, TRUE);
            }
        }
#endif
#endif

    }
        return i4Ret;
    case OSD_RGN_BMP_PITCH:

        #if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        IGNORE_RET(_OSD_RGN_SetLineSize(u4Region, ((u4Value+0xf) >> 4) & 0x7ff));
        #else
        IGNORE_RET(_OSD_RGN_SetLineSize(u4Region, ((u4Value+0xf) >> 4) & 0x1ff));
        IGNORE_RET(_OSD_RGN_SetLineSize9(u4Region, ((u4Value+0xf) >> 13) & 1));
        #endif
        break;

    case OSD_RGN_CLIP_V:
    {
        UINT32 u4SrcHeight, u4OldVClip;
        // recover orginal input height
        IGNORE_RET(_OSD_RGN_GetVClip(u4Region, &u4OldVClip));
        //IGNORE_RET(_OSD_RGN_GetInputHeight(u4Region, &u4SrcHeight));
        u4SrcHeight = _rAllRgnNode[u4Region].u4Original_InH;
        //u4SrcHeight += u4OldVClip;
        if (u4SrcHeight <= u4Value)
        {
            return -(INT32) OSD_RET_INV_ARG;
        }
        //ASSERT(u4SrcHeight > u4Value);
        // cut input height
        IGNORE_RET(_OSD_RGN_SetInputHeight(u4Region, u4SrcHeight - u4Value));
        
        return _OSD_RGN_SetVClip(u4Region, u4Value);
    }

    case OSD_RGN_CLIP_H:
    {
        UINT32 u4DstWidth, u4OldHClip,u4_dec;
        UINT32 u4SrcWidth;
        UINT32 bIsClip=FALSE;
        // recover orginal input width
        IGNORE_RET(_OSD_RGN_GetHClip(u4Region, &u4OldHClip));
        //IGNORE_RET(_OSD_RGN_GetInputWidth(u4Region, &u4SrcWidth));
        u4SrcWidth = _rAllRgnNode[u4Region].u4Original_InW;
        //IGNORE_RET(_OSD_RGN_GetOutputWidth(u4Region, &u4DstWidth));
        u4DstWidth = _rAllRgnNode[u4Region].u4Original_OutW;
        u4DstWidth += u4OldHClip;
        VERIFY(OSD_RGN_Get(u4Region,OSD_RGN_DEC_EN,&u4_dec) == OSD_RET_OK);
        if(OSD_GetCpblty(E_OSD_CPBLTY_MIRROR,0x0) && _OsdRegionMirrorClipJudge(u4DstWidth)&&(u4_dec))
        {
           u4Value=u4Value+_OsdRegionMirrorClipValue(u4DstWidth);
           bIsClip=TRUE;
        }
        //ASSERT(u4SrcWidth > u4Value);
        if(u4SrcWidth<=u4Value)
        {
           return OSD_RET_OK; 
        }

        // cut input width
        IGNORE_RET(_OSD_RGN_SetInputWidth(u4Region, u4SrcWidth - u4Value));
        if(bIsClip)
        {
            IGNORE_RET(_OSD_RGN_SetInputWidth(u4Region, u4SrcWidth));
        }
    
        return _OSD_RGN_SetHClip(u4Region, u4Value);
    }

    case OSD_RGN_PAL_LOAD:
        return _OSD_RGN_SetLoadPalette(u4Region, u4Value);

    case OSD_RGN_PAL_ADDR:
        return _OSD_RGN_SetPaletteAddr(u4Region, u4Value);

    case OSD_RGN_PAL_LEN:
        return _OSD_RGN_SetPaletteLen(u4Region, u4Value);

    case OSD_RGN_STEP_V:
    {
        UINT32 u4_dec = 0x0;
        VERIFY(OSD_RGN_Get(u4Region,OSD_RGN_DEC_EN,&u4_dec) == OSD_RET_OK);
        if(u4_dec)
        {
            break;
        }
        return _OSD_RGN_SetVStep(u4Region, u4Value);
    }
    case OSD_RGN_STEP_H:
        return _OSD_RGN_SetHStep(u4Region, u4Value);

    case OSD_RGN_COLOR_KEY:
        return _OSD_RGN_SetColorKey(u4Region, u4Value);

    case OSD_RGN_COLOR_KEY_EN:
        return _OSD_RGN_SetColorKeyEnable(u4Region, u4Value);

    case OSD_RGN_MIX_SEL:
#if LG_OSD_PATH_TEST
        if( u4Region == OSD_PLA_GetFirstRegion(OSD_PLANE_2))
        {
            if(u4Value == 1)
            {
                OSD_BASE_SetFRCHeader(OSD_FRCHD_PIXELALPHAEN,TRUE);
                OSD_BASE_SetFRCHeader(OSD_FRCHD_GLOBALALPHAEN,FALSE);
            }
            else
            {
                OSD_BASE_SetFRCHeader(OSD_FRCHD_GLOBALALPHAEN,TRUE);
                OSD_BASE_SetFRCHeader(OSD_FRCHD_PIXELALPHAEN,FALSE);
            }
        }
#endif

        return _OSD_RGN_SetBlendMode(u4Region, u4Value);

    case OSD_RGN_BIG_ENDIAN:
        return OSD_RGN_SetBigEndian(u4Region, u4Value);

    case OSD_RGN_V_FLIP:
        _OSD_RGN_GetDecEn(u4Region, &u4Tmp);
        if(u4Tmp != 0)
        {
            u4Value = 0;
        }
        i4Ret = _OSD_RGN_SetVFlip(u4Region, u4Value);

        OSD_RGN_MirrorFlipUpdate(u4Region, FALSE, TRUE);

        return i4Ret;

    case OSD_RGN_H_MIRROR:
        _OSD_RGN_GetDecEn(u4Region, &u4Tmp);
        if(u4Tmp != 0)
        {
            u4Value = 0;
        }

        i4Ret = _OSD_RGN_SetHMirror(u4Region, u4Value);

        OSD_RGN_MirrorFlipUpdate(u4Region, TRUE, FALSE);

        return i4Ret;

    case OSD_RGN_DEC_MODE:
        return _OSD_RGN_SetDecMode(u4Region, u4Value);
    case OSD_RGN_DEC_LINE:
        return _OSD_RGN_SetDecLine(u4Region, u4Value);
    case OSD_RGN_DEC_EN:
        _rAllRgnNode[u4Region].u4DecEn = u4Value;
        i4Ret =  _OSD_RGN_SetDecEn(u4Region, u4Value);
        if(u4Value != 0)
        {
            _OSD_RGN_SetVFlip(u4Region, 0);
            _OSD_RGN_SetHMirror(u4Region, 0);
        }
        
        OSD_RGN_MirrorFlipUpdate(u4Region, TRUE, TRUE);

        return i4Ret;

    case OSD_RGN_ENDIAN:
        return _OSD_RGN_SetEndian(u4Region, u4Value);

    case OSD_RGN_HINTP_MODE:
        return _OSD_RGN_SetHIntMode(u4Region, u4Value);

    case OSD_RGN_VSTEP_OFFSET:
        return _OSD_RGN_SetVStepOffset(u4Region, u4Value);

    case OSD_RGN_HSTEP_OFFSET:
        return _OSD_RGN_SetHStepOffset(u4Region, u4Value);

    case OSD_RGN_YR_SEL:
        return _OSD_RGN_SetYrSel(u4Region, u4Value);
    case OSD_RGN_UG_SEL:
        return _OSD_RGN_SetUgSel(u4Region, u4Value);
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    case OSD_RGN_VB_SEL:
        return _OSD_RGN_SetVbSel(u4Region, u4Value);
#endif
    case OSD_RGN_ALPHA_SEL:
        return _OSD_RGN_SetASel(u4Region, u4Value);
    default:
        return -(INT32) OSD_RET_INV_ARG;
    }

    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** create an osd region
 *  @param  pu4Region region id pointer
 *  @param  u4BmpWidth source bitmap width
 *  @param  u4BmpHeight source bitmap height
 *  @param  pvBitmap source bitmap address, MUST align with 16bytes
 *  @param  eColorMode, region color mode, refer to OSD_COLOR_MODE_T
 *  @param  u4BmpPitch, bytes per line, MUST align with 16bytes
 *  @param  u4DispX, region x position
 *  @param  u4DispY, region y position
 *  @param  u4DispW, output width
 *  @param  u4DispH, output height

 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_ARG invalid argument
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Create(UINT32 * pu4Region, UINT32 u4BmpWidth, UINT32 u4BmpHeight,
        void * pvBitmap, UINT32 eColorMode, UINT32 u4BmpPitch, UINT32 u4DispX,
        UINT32 u4DispY, UINT32 u4DispW, UINT32 u4DispH)
{
    UINT32 u4Region;
    INT32 i4Ret;

    if ((pu4Region == NULL) || (pvBitmap == NULL) || (((UINT32) pvBitmap & 0xf)
            != 0) || (u4BmpWidth == 0) || (u4BmpHeight == 0) || (u4DispW == 0)
            || (u4DispH == 0) || ((u4BmpPitch & 0xf) != 0) || ((u4BmpPitch
            >> 14) != 0))
    {
        Printf("OSD_RGN_Create failed\n");
        return -(INT32) OSD_RET_INV_ARG;
    }

    i4Ret = _OSD_RGN_Alloc(&u4Region);
    if (i4Ret != (INT32) OSD_RET_OK)
    {
        return i4Ret;
    }

    _rAllRgnNode[u4Region].u4Original_X=u4DispX;
    _rAllRgnNode[u4Region].u4Original_Y=u4DispY;
    _rAllRgnNode[u4Region].u4Original_OutW = u4DispW;
    _rAllRgnNode[u4Region].u4Original_InW = u4BmpWidth;
    _rAllRgnNode[u4Region].u4Original_OutH = u4DispH;
    _rAllRgnNode[u4Region].u4Original_InH = u4BmpHeight;


    if ((u4DispX + u4DispW) > (OSD_MAX_WIDTH - 1))
    {
        u4DispW = (OSD_MAX_WIDTH - 1) - u4DispX;
    }
    if ((u4DispY + u4DispH) > (OSD_MAX_HEIGHT - 1))
    {
        u4DispH = (OSD_MAX_HEIGHT - 1) - u4DispY;
    }

    i4Ret = _OSD_RGN_SetNextRegion(u4Region, 0);
    i4Ret = _OSD_RGN_SetNextEnable(u4Region, 0);
    i4Ret = _OSD_RGN_SetColorMode(u4Region, eColorMode);
    i4Ret = _OSD_RGN_SetDataAddr(u4Region, ((UINT32) pvBitmap));
    i4Ret = _OSD_RGN_SetAlpha(u4Region, (UINT32) 0xff);
    i4Ret = _OSD_RGN_SetInputWidth(u4Region, u4BmpWidth);
    i4Ret = _OSD_RGN_SetInputHeight(u4Region, u4BmpHeight);
    i4Ret = _OSD_RGN_SetOutputWidth(u4Region, u4DispW);
    i4Ret = _OSD_RGN_SetOutputHeight(u4Region, u4DispH);
    i4Ret = _OSD_RGN_SetOutputPosX(u4Region, u4DispX);
    i4Ret = _OSD_RGN_SetOutputPosY(u4Region, u4DispY);
    i4Ret = _OSD_RGN_SetColorKeyEnable(u4Region, 0);
    i4Ret = _OSD_RGN_SetColorKey(u4Region, 0);
    i4Ret = _OSD_RGN_SetHClip(u4Region, 0);
    i4Ret = _OSD_RGN_SetVClip(u4Region, 0);

    if (u4BmpPitch == 0)
    {
        if ((eColorMode == (UINT32) OSD_CM_YCBCR_CLUT2) || (eColorMode
                == (UINT32) OSD_CM_RGB_CLUT2))
        {
            u4BmpPitch = u4BmpWidth >> 2;
        }
        else if ((eColorMode == (UINT32) OSD_CM_YCBCR_CLUT4) || (eColorMode
                == (UINT32) OSD_CM_RGB_CLUT4))
        {
            u4BmpPitch = u4BmpWidth >> 1;
        }
        else if ((eColorMode == (UINT32) OSD_CM_YCBCR_CLUT8) || (eColorMode
                == (UINT32) OSD_CM_RGB_CLUT8))
        {
            u4BmpPitch = u4BmpWidth;
        }
        else if ((eColorMode == (UINT32) OSD_CM_AYCBCR8888_DIRECT32)
                || (eColorMode == (UINT32) OSD_CM_ARGB8888_DIRECT32))
        {
            u4BmpPitch = u4BmpWidth << 2;
        }
        else if (eColorMode == (UINT32) OSD_CM_ALPHA_BIT)
        {
            u4BmpPitch = u4BmpWidth >> 3;
        }
        else
        {
            u4BmpPitch = u4BmpWidth << 1;
        }
        //OSD BmpPitch must be 128 bit alignment
        u4BmpPitch = (u4BmpPitch + 0xf) & 0xfffffff0;
    }
    
    i4Ret = OSD_RGN_Set(u4Region, (INT32) OSD_RGN_BMP_PITCH, u4BmpPitch);

    i4Ret = _OSD_RGN_SetHStep(u4Region,
            (u4BmpWidth == u4DispW) ? 0x1000 : ((u4BmpWidth << 12) / u4DispW));
    i4Ret = _OSD_RGN_SetVStep(u4Region,
            (u4BmpHeight == u4DispH) ? 0x1000 : ((u4BmpHeight << 12) / u4DispH));
    i4Ret = _OSD_RGN_SetBlendMode(u4Region, (UINT32) OSD_BM_REGION);

    i4Ret = _OSD_RGN_SetASel(u4Region, 3);
    
    i4Ret = _OSD_RGN_SetYrSel(u4Region, 2);
    
    i4Ret = _OSD_RGN_SetUgSel(u4Region, 1);
    
    // palette mode
    if (((eColorMode <= (UINT32) OSD_CM_YCBCR_CLUT8) 
        || (eColorMode >= (UINT32) OSD_CM_RGB_CLUT2)) 
        && (eColorMode <= (UINT32) OSD_CM_RGB_CLUT8))
    {
        if ((eColorMode == (UINT32) OSD_CM_YCBCR_CLUT2) 
            || (eColorMode == (UINT32) OSD_CM_RGB_CLUT2))
        {
            i4Ret = OSD_RGN_Set(u4Region, (INT32) OSD_RGN_PAL_LEN, 0);
        }
        else if ((eColorMode == (UINT32) OSD_CM_YCBCR_CLUT4) 
                  || (eColorMode == (UINT32) OSD_CM_RGB_CLUT4))
        {
            i4Ret = OSD_RGN_Set(u4Region, (INT32) OSD_RGN_PAL_LEN, 1);
        }
        else if ((eColorMode == (UINT32) OSD_CM_YCBCR_CLUT8) 
                  || (eColorMode == (UINT32) OSD_CM_RGB_CLUT8))
        {
            i4Ret = OSD_RGN_Set(u4Region, (INT32) OSD_RGN_PAL_LEN, 2);
        }
        
        i4Ret = OSD_RGN_Set(  u4Region,
                              (INT32) OSD_RGN_PAL_ADDR,
                              (((UINT32) pvBitmap + (u4BmpPitch * u4BmpHeight) + 0x2000)
                               & ~0x1fff));
        
        i4Ret = OSD_RGN_Set(u4Region, (INT32) OSD_RGN_PAL_LOAD, 1);
    }
#ifndef CONFIG_OSD_COMPRESSION    
#ifdef CC_FLIP_MIRROR_SUPPORT
    OSD_RGN_Set(u4Region, (INT32) OSD_RGN_V_FLIP, OSD_GetCpblty(E_OSD_CPBLTY_FLIP, 0x0));
    OSD_RGN_Set(u4Region, (INT32) OSD_RGN_H_MIRROR, OSD_GetCpblty(E_OSD_CPBLTY_MIRROR,0x0));
#endif
#endif
    OSD_RGN_Set(u4Region,OSD_RGN_CLIP_H,0);
    UNUSED(i4Ret);
    
    _rAllRgnNode[u4Region].u4NodeStatus |= OSD_RGN_NODE_ALLOCATED;
    _rAllRgnNode[u4Region].i4Next = OSD_INVALID_REGION_HANDLE;
    _rAllRgnNode[u4Region].i4Prev = OSD_INVALID_REGION_HANDLE;

    *pu4Region = u4Region;
    return (INT32) OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** delete an osd region
 *  @param  u4Region region id
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Delete(UINT32 u4Region)
{
    OSD_VERIFY_REGION(u4Region);

    if (_rAllRgnNode[u4Region].i4Prev != OSD_INVALID_REGION_HANDLE)
    {
        INT32 i4Prev;
        i4Prev = _rAllRgnNode[u4Region].i4Prev;
        if ((i4Prev >= 0) && (i4Prev < OSD_MAX_NUM_RGN))
        {
            // assign [i4Prev].i4Next = [u4Region].i4Next
            _rAllRgnNode[i4Prev].i4Next = _rAllRgnNode[u4Region].i4Next;
        }
    }

    if (_rAllRgnNode[u4Region].i4Next != OSD_INVALID_REGION_HANDLE)
    {
        INT32 i4Next;
        i4Next = _rAllRgnNode[u4Region].i4Next;
        if ((i4Next >= 0) && (i4Next < OSD_MAX_NUM_RGN))
        {
            // assign [i4Next].i4Prev = [u4Region].i4Prev
            _rAllRgnNode[i4Next].i4Prev = _rAllRgnNode[u4Region].i4Prev;
        }
    }

    VERIFY((INT32) OSD_RET_OK == _OSD_RGN_Free(u4Region));
    _rAllRgnNode[u4Region].i4Prev = OSD_INVALID_REGION_HANDLE;
    _rAllRgnNode[u4Region].i4Next = OSD_INVALID_REGION_HANDLE;
    _rAllRgnNode[u4Region].u4NodeStatus &= ~OSD_RGN_NODE_ALLOCATED;

    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_AttachTail(UINT32 u4Region, UINT32 u4RgnList)
{
    INT32 i4OrigTail;
    UINT32 u4RgnAddr;
    UINT32 u4ColorMode;
    OSD_REGION_LIST_T * prRgnList;
    INT32 i4Ignore;

    OSD_VERIFY_REGION(u4Region);
    OSD_VERIFY_RGNLIST(u4RgnList);

    prRgnList = &_rAllRgnList[u4RgnList];
    i4OrigTail = prRgnList->i4Tail;

    if ((prRgnList->i4Head == (INT32) NULL)
            && (prRgnList->i4Count == (INT32) 0))
    {
        return OSD_RGN_Insert(u4Region, u4RgnList);
    }

    if (i4OrigTail == OSD_INVALID_REGION_HANDLE)
    {
        return -(INT32) OSD_RET_INV_LIST;
    }

    _rAllRgnList[u4RgnList].i4Count++;
    _rAllRgnList[u4RgnList].i4Tail = (INT32) u4Region;
    _rAllRgnNode[i4OrigTail].i4Next = (INT32) u4Region;
    _rAllRgnNode[u4Region].i4Prev = i4OrigTail;

    i4Ignore = _OSD_RGN_GetAddress(u4Region, &u4RgnAddr);
    ASSERT((u4RgnAddr & 0xf) == 0);
    i4Ignore = _OSD_RGN_SetNextRegion((UINT32) i4OrigTail, u4RgnAddr);
    i4Ignore = _OSD_RGN_SetNextEnable((UINT32) i4OrigTail, 1);
    i4Ignore = _OSD_RGN_SetNextEnable(u4Region, 0); // tail means 'no next'
    i4Ignore = _OSD_RGN_SetNextRegion(u4Region, 0);

    i4Ignore = _OSD_RGN_GetColorMode(u4Region, &u4ColorMode);
    if ((u4ColorMode == (UINT32) OSD_CM_YCBCR_CLUT2) || (u4ColorMode
            == (UINT32) OSD_CM_YCBCR_CLUT4) || (u4ColorMode
            == (UINT32) OSD_CM_YCBCR_CLUT8) || (u4ColorMode
            == (UINT32) OSD_CM_RGB_CLUT2) || (u4ColorMode
            == (UINT32) OSD_CM_RGB_CLUT4) || (u4ColorMode
            == (UINT32) OSD_CM_RGB_CLUT8))
    {
        // check if we should load new palette
        UINT32 u4Pal1, u4Pal2;
        UINT32 u4Len1, u4Len2;
        i4Ignore = _OSD_RGN_GetPaletteAddr(u4Region, &u4Pal1);
        i4Ignore = _OSD_RGN_GetPaletteLen(u4Region, &u4Len1);
        i4Ignore = _OSD_RGN_GetPaletteAddr((UINT32) i4OrigTail, &u4Pal2);
        i4Ignore = _OSD_RGN_GetPaletteLen((UINT32) i4OrigTail, &u4Len2);
        i4Ignore = _OSD_RGN_SetLoadPalette(u4Region,
                ((u4Pal1 == u4Pal2) && (u4Len1 == u4Len2)) ? 0 : 1);
    }
    else
    {
        // not index color mode, should not load palette
        i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 0);
    }

    UNUSED(i4Ignore);
    return (INT32) OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** attach region to list
 *  @param  u4Region region id
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_LIST invalid list
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Insert(UINT32 u4Region, UINT32 u4RgnList)
{
    INT32 i4RgnCurr, i4RgnNext = 0;
    OSD_REGION_LIST_T * prRgnList;
    UINT32 u4ColorMode, u4Address;
    INT32 i4Ignore = 0;

    OSD_VERIFY_REGION(u4Region);
    OSD_VERIFY_RGNLIST(u4RgnList);

    prRgnList = &_rAllRgnList[u4RgnList];
    i4RgnCurr = prRgnList->i4Head;
    if (i4RgnCurr == OSD_INVALID_REGION_HANDLE)
    {
        if (prRgnList->i4Count == 0)
        {
            _rAllRgnList[u4RgnList].i4Head = (INT32) u4Region;
            _rAllRgnList[u4RgnList].i4Tail = (INT32) u4Region;
            _rAllRgnNode[u4Region].i4Next = OSD_INVALID_REGION_HANDLE;
            _rAllRgnNode[u4Region].i4Prev = OSD_INVALID_REGION_HANDLE;
            _rAllRgnList[u4RgnList].i4Count = 1;

            i4Ignore = _OSD_RGN_SetNextEnable(u4Region, 0);
            i4Ignore = _OSD_RGN_SetNextRegion(u4Region, 0);

            i4Ignore = _OSD_RGN_GetColorMode(u4Region, &u4ColorMode);
            if ((u4ColorMode == (UINT32) OSD_CM_YCBCR_CLUT2) || (u4ColorMode
                    == (UINT32) OSD_CM_YCBCR_CLUT4) || (u4ColorMode
                    == (UINT32) OSD_CM_YCBCR_CLUT8) || (u4ColorMode
                    == (UINT32) OSD_CM_RGB_CLUT2) || (u4ColorMode
                    == (UINT32) OSD_CM_RGB_CLUT4) || (u4ColorMode
                    == (UINT32) OSD_CM_RGB_CLUT8))
            {

                i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 1);
            }
            else
            {
                // not index color mode, should not load palette
                i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 0);
            }
            return (INT32) OSD_RET_OK;
        }
        else
        {
            return -(INT32) OSD_RET_INV_LIST;
        }
    }

    OSD_VERIFY_REGION(i4RgnCurr);

    switch (_OsdRegionCompare(u4Region, (UINT32) i4RgnCurr))
    {
    case -1:
        prRgnList->i4Head = (INT32) u4Region;
        i4Ignore = OSD_PLA_Reflip(u4RgnList);
        _rAllRgnNode[u4Region].i4Next = i4RgnCurr;
        _rAllRgnNode[u4Region].i4Prev = OSD_INVALID_REGION_HANDLE;
        _rAllRgnNode[i4RgnCurr].i4Prev = (INT32) u4Region;
        prRgnList->i4Count++;

        i4Ignore = _OSD_RGN_GetAddress((UINT32) i4RgnCurr, &u4Address);
        i4Ignore = _OSD_RGN_SetNextRegion(u4Region, u4Address);
        i4Ignore = _OSD_RGN_SetNextEnable(u4Region, 1);

        i4Ignore = _OSD_RGN_GetColorMode(u4Region, &u4ColorMode);
        if ((u4ColorMode == (UINT32) OSD_CM_YCBCR_CLUT2) || (u4ColorMode
                == (UINT32) OSD_CM_YCBCR_CLUT4) || (u4ColorMode
                == (UINT32) OSD_CM_YCBCR_CLUT8) || (u4ColorMode
                == (UINT32) OSD_CM_RGB_CLUT2) || (u4ColorMode
                == (UINT32) OSD_CM_RGB_CLUT4) || (u4ColorMode
                == (UINT32) OSD_CM_RGB_CLUT8))
        {

            i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 1);
        }
        else
        {
            // not index color mode, should not load palette
            i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 0);
        }

        return (INT32) OSD_RET_OK;

    case 1:
        break;

    default:
        return -(INT32) OSD_RET_REGION_COLLISION;
    }

    while ((i4RgnNext = _rAllRgnNode[i4RgnCurr].i4Next)
            != OSD_INVALID_REGION_HANDLE)
    {
        if ((i4RgnNext < 0) || (i4RgnNext >= OSD_MAX_NUM_RGN))
        {
            i4RgnNext = OSD_INVALID_REGION_HANDLE;
            _rAllRgnNode[i4RgnCurr].i4Next = i4RgnNext;
            break;
        }

        switch (_OsdRegionCompare(u4Region, (UINT32) i4RgnNext))
        {
        case -1:
            // insert
            _rAllRgnNode[i4RgnCurr].i4Next = (INT32) u4Region;
            _rAllRgnNode[u4Region].i4Next = i4RgnNext;
            _rAllRgnNode[i4RgnNext].i4Prev = (INT32) u4Region;
            _rAllRgnNode[u4Region].i4Prev = i4RgnCurr;
            prRgnList->i4Count++;

            // maintain hw list
            i4Ignore = _OSD_RGN_GetAddress(u4Region, &u4Address);
            // current --> region
            i4Ignore = _OSD_RGN_SetNextRegion((UINT32) i4RgnCurr, u4Address);
            i4Ignore = _OSD_RGN_GetAddress((UINT32) i4RgnNext, &u4Address);
            // region --> next
            i4Ignore = _OSD_RGN_SetNextRegion(u4Region, u4Address);

            if (i4RgnNext != OSD_INVALID_REGION_HANDLE)
            {
                // valid next
                i4Ignore = _OSD_RGN_SetNextEnable(u4Region, 1);
            }
            else
            {
                // no next
                i4Ignore = _OSD_RGN_SetNextEnable(u4Region, 0);
            }
            i4Ignore = _OSD_RGN_SetNextEnable((UINT32) i4RgnCurr, 1);

            i4Ignore = _OSD_RGN_GetColorMode(u4Region, &u4ColorMode);
            if ((u4ColorMode == (UINT32) OSD_CM_YCBCR_CLUT2) || (u4ColorMode
                    == (UINT32) OSD_CM_YCBCR_CLUT4) || (u4ColorMode
                    == (UINT32) OSD_CM_YCBCR_CLUT8) || (u4ColorMode
                    == (UINT32) OSD_CM_RGB_CLUT2) || (u4ColorMode
                    == (UINT32) OSD_CM_RGB_CLUT4) || (u4ColorMode
                    == (UINT32) OSD_CM_RGB_CLUT8))
            {
                // check if we should load new palette
                UINT32 u4Pal1, u4Pal2;
                UINT32 u4Len1, u4Len2;
                i4Ignore = _OSD_RGN_GetPaletteAddr(u4Region, &u4Pal1);
                i4Ignore = _OSD_RGN_GetPaletteLen(u4Region, &u4Len1);
                i4Ignore = _OSD_RGN_GetPaletteAddr((UINT32) i4RgnCurr, &u4Pal2);
                i4Ignore = _OSD_RGN_GetPaletteLen((UINT32) i4RgnCurr, &u4Len2);
                i4Ignore = _OSD_RGN_SetLoadPalette(u4Region,
                        ((u4Pal1 == u4Pal2) && (u4Len1 == u4Len2)) ? 0 : 1);
            }
            else
            {
                // not index color mode, should not load palette
                i4Ignore = _OSD_RGN_SetLoadPalette(u4Region, 0);
            }
            return (INT32) OSD_RET_OK;

        case 1:
            i4RgnCurr = i4RgnNext;
            break;

        default:
            return -(INT32) OSD_RET_REGION_COLLISION;
        }
    }

    UNUSED(i4Ignore);

    _rAllRgnNode[i4RgnCurr].i4Next = i4RgnNext;
    return OSD_RGN_AttachTail(u4Region, u4RgnList);
}

//-----------------------------------------------------------------------------
/** dettach region from list
 *  @param  u4Region region id
 *  @param  u4RgnList list id
 *  @retval OSD_RET_OK success
 *  @retval OSD_RET_INV_REGION invalid region
 */
//-----------------------------------------------------------------------------
INT32 OSD_RGN_Detach(UINT32 u4Region, UINT32 u4RgnList)
{
    INT32 i4RgnCurr, i4RgnNext, i4RgnPrev;
    OSD_REGION_LIST_T * prRgnList;
    UINT32 u4Address;
    INT32 i4Ignore = 0;

    OSD_VERIFY_REGION(u4Region);
    OSD_VERIFY_RGNLIST(u4RgnList);

    prRgnList = &_rAllRgnList[u4RgnList];
    i4RgnCurr = prRgnList->i4Head;

    if (prRgnList->i4Head == OSD_INVALID_REGION_HANDLE)
    {
        return -(INT32) OSD_RET_INV_REGION;
    }

    while (i4RgnCurr != OSD_INVALID_REGION_HANDLE)
    {
        OSD_VERIFY_REGION(i4RgnCurr);
        if ((UINT32) i4RgnCurr == u4Region)
        {
            i4RgnNext = _rAllRgnNode[u4Region].i4Next;
            i4RgnPrev = _rAllRgnNode[u4Region].i4Prev;
            if (i4RgnNext != OSD_INVALID_REGION_HANDLE)
            {
                _rAllRgnNode[i4RgnNext].i4Prev = i4RgnPrev;
            }
            else // region is tail
            {
                // attached to list tail
                prRgnList->i4Tail = i4RgnPrev;
            }

            if (i4RgnPrev != OSD_INVALID_REGION_HANDLE)
            {
                _rAllRgnNode[i4RgnPrev].i4Next = i4RgnNext;
            }
            else // region is head
            {
                // new list head
                prRgnList->i4Head = i4RgnNext;
            }

            // maintain hardware list
            if ((i4RgnNext == OSD_INVALID_REGION_HANDLE) && (i4RgnPrev
                    != OSD_INVALID_REGION_HANDLE))
            {
                // region is tail
                i4Ignore = _OSD_RGN_SetNextRegion((UINT32) i4RgnPrev, 0);
                i4Ignore = _OSD_RGN_SetNextEnable((UINT32) i4RgnPrev, 0);
            }
            else if ((i4RgnNext != OSD_INVALID_REGION_HANDLE) && (i4RgnPrev
                    == OSD_INVALID_REGION_HANDLE))
            {
                // region is head, re-flip if necessary
                i4Ignore = OSD_PLA_Reflip(u4RgnList);
            }
            else if ((i4RgnNext != OSD_INVALID_REGION_HANDLE) && (i4RgnPrev
                    != OSD_INVALID_REGION_HANDLE))
            {
                i4Ignore = _OSD_RGN_GetAddress((UINT32) i4RgnNext, &u4Address);
                i4Ignore
                        = _OSD_RGN_SetNextRegion((UINT32) i4RgnPrev, u4Address);
            }
            else
            {
                /*
                 (i4RgnNext == OSD_INVALID_REGION_HANDLE) && \
                (i4RgnPrev == OSD_INVALID_REGION_HANDLE)
                 */
            }

            _rAllRgnNode[u4Region].i4Next = OSD_INVALID_REGION_HANDLE;
            _rAllRgnNode[u4Region].i4Prev = OSD_INVALID_REGION_HANDLE;
            prRgnList->i4Count--;
            if (prRgnList->i4Count == 0)
            {
                prRgnList->i4Head = OSD_INVALID_REGION_HANDLE;
                prRgnList->i4Tail = OSD_INVALID_REGION_HANDLE;
            }

            return (INT32) OSD_RET_OK;
        }
        else
        {
            i4RgnCurr = _rAllRgnNode[i4RgnCurr].i4Next;
        }
    }

    UNUSED(i4Ignore);
    return (INT32) OSD_RET_INV_REGION;
}

INT32 OSD_RGN_SetDisplayWidth(UINT32 u4Region, UINT32 u4Width)
{
    UINT32 u4SrcW;
    UINT32 u4Step;
    INT32 i4Ignore;
    UINT32 u4Output;
    OSD_VERIFY_REGION(u4Region);

    if (u4Width == 0)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }

    _rAllRgnNode[u4Region].u4Original_OutW = u4Width;

    u4Output = u4Width;

    i4Ignore = _OSD_RGN_GetInputWidth(u4Region, &u4SrcW);
    i4Ignore = _OSD_RGN_SetOutputWidth(u4Region, u4Output);
    u4Step = (u4SrcW == u4Width) ? 0x1000 : ((u4SrcW << 12) / u4Width);
    i4Ignore = _OSD_RGN_SetHStep(u4Region, (u4Step > 0xffff) ? 0xffff : u4Step);
    UNUSED(i4Ignore);

    OSD_RGN_MirrorFlipUpdate(u4Region, TRUE, FALSE);

    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_SetDisplayHeight(UINT32 u4Region, UINT32 u4Height)
{
    UINT32 u4SrcH;
    UINT32 u4Step;
    INT32 i4Ignore;
    UINT32 u4Output;

    OSD_VERIFY_REGION(u4Region);

    if (u4Height == 0)
    {
        return -(INT32) OSD_RET_INV_ARG;
    }
    _rAllRgnNode[u4Region].u4Original_OutH = u4Height;
    u4Output = u4Height;
    i4Ignore = _OSD_RGN_GetInputHeight(u4Region, &u4SrcH);
    i4Ignore = _OSD_RGN_SetOutputHeight(u4Region, u4Output);
    u4Step = (u4SrcH == u4Height) ? 0x1000 : ((u4SrcH << 12) / u4Height);
    i4Ignore = _OSD_RGN_SetVStep(u4Region, (u4Step > 0xffff) ? 0xffff : u4Step);
    UNUSED(i4Ignore);
    OSD_RGN_MirrorFlipUpdate(u4Region, FALSE, TRUE);

    return (INT32) OSD_RET_OK;
}
INT32 OSD_RGN_SetColorOrder(UINT32 u4Region,UINT32 u4AOrder,UINT32 u4ROrder,
                            UINT32 u4GOrder,UINT32 u4BOrder)
{
    OSD_VERIFY_REGION(u4Region);
    printf("%s:%d,%d,%d,%d,%d\n",__FUNCTION__,u4Region,u4AOrder,
        u4ROrder,u4GOrder,u4BOrder);
    if((u4AOrder ==u4ROrder) || (u4AOrder == u4GOrder) 
        || (u4AOrder ==u4BOrder) || (u4ROrder ==u4GOrder) 
        || (u4ROrder ==u4BOrder) || (u4GOrder ==u4BOrder))
    {
        printf("Each order should not equal.\n");
        return (INT32) OSD_RET_UNDEF_ERR;
    }
    #if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    IGNORE_RET(_OSD_RGN_SetASel(u4Region, u4AOrder));
    IGNORE_RET(_OSD_RGN_SetYrSel(u4Region, u4ROrder));
    IGNORE_RET(_OSD_RGN_SetUgSel(u4Region, u4GOrder));
    IGNORE_RET(_OSD_RGN_SetVbSel(u4Region, u4BOrder));
    #else
    IGNORE_RET(_OSD_RGN_SetASel(u4Region, u4AOrder));
    IGNORE_RET(_OSD_RGN_SetYrSel(u4Region, u4ROrder));
    IGNORE_RET(_OSD_RGN_SetUgSel(u4Region, u4GOrder));
    #endif
    return (INT32) OSD_RET_OK;
}
INT32 OSD_RGN_SetBigEndian(UINT32 u4Region, BOOL fgBE)
{
    OSD_VERIFY_REGION(u4Region);
    if (fgBE)
    {
        IGNORE_RET(_OSD_RGN_SetASel(u4Region, 0));
        IGNORE_RET(_OSD_RGN_SetYrSel(u4Region, 1));
        IGNORE_RET(_OSD_RGN_SetUgSel(u4Region, 2));
    }
    else
    {
        IGNORE_RET(_OSD_RGN_SetASel(u4Region, 3));
        IGNORE_RET(_OSD_RGN_SetYrSel(u4Region, 2));
        IGNORE_RET(_OSD_RGN_SetUgSel(u4Region, 1));
    }
    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_Dump(UINT32 u4Region)
{
    UINT32 u4Prev, u4Next, u4Flag, u4PosX, u4PosY;
    UINT32 u4BmpW, u4BmpH, u4DispW, u4DispH, u4ColorMode;
    UINT32 u4Alpha, u4BmpAddr, u4BmpPitch, u4ClipV, u4ClipH;
    UINT32 u4PalLoad, u4PalAddr, u4PalLen, u4StepV, u4StepH;
    UINT32 u4ColorKey, u4ColorKeyEn, u4MixSel, u4BigEndian;
    UINT32 u4VFlip, u4HMirror;
    UINT32 u4DecEn, u4DecMode,u4DecLine;

    OSD_VERIFY_REGION(u4Region);

    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_PREV, &u4Prev));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_NEXT, &u4Next));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_FLAGS, &u4Flag));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_POS_X, &u4PosX));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_POS_Y, &u4PosY));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_BMP_W, &u4BmpW));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_BMP_H, &u4BmpH));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_DISP_W, &u4DispW));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_DISP_H, &u4DispH));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_COLORMODE, &u4ColorMode));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_ALPHA, &u4Alpha));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_BMP_ADDR, &u4BmpAddr));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_BMP_PITCH, &u4BmpPitch));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_CLIP_V, &u4ClipV));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_CLIP_H, &u4ClipH));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_PAL_LOAD, &u4PalLoad));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_PAL_ADDR, &u4PalAddr));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_PAL_LEN, &u4PalLen));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_STEP_V, &u4StepV));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_STEP_H, &u4StepH));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_COLOR_KEY, &u4ColorKey));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_COLOR_KEY_EN, &u4ColorKeyEn));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_MIX_SEL, &u4MixSel));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_V_FLIP, &u4VFlip));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_H_MIRROR, &u4HMirror));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_BIG_ENDIAN, &u4BigEndian));

    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_DEC_EN, &u4DecEn));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_DEC_MODE, &u4DecMode));
    IGNORE_RET(OSD_RGN_Get(u4Region, (INT32) OSD_RGN_DEC_LINE, &u4DecLine));

    Printf("REGION[%ld]\n", u4Region);
    Printf("\tDATA_ADDR=0x%x, LINE_SIZE=%d, COLOR_MODE=%d\n", 
             u4BmpAddr,u4BmpPitch,u4ColorMode);
    Printf("\tSrcSize(IHW:%d, IVW:%d), DstSize(OHW:%d, OVW:%d)\n", 
             u4BmpW,u4BmpH,u4DispW,u4DispH);
    Printf("\tPos(OHS:%d, OVS:%d), Clip(HCLIP:%d, VCLIP:%d)\n", 
             u4PosX,u4PosY,u4ClipH,u4ClipV);
    Printf("\tVFLIP=%d, HMIRROR=%d,Step(H_STEP:0x%x, V_STEP:0x%x)\n", 
             u4VFlip,u4HMirror,u4StepH, u4StepV);
    Printf("\tMIX_WEIGHT=%d, MIX_SEL=%d, COLOR_KEY_EN:%d, COLOR_KEY:0x%x\n", 
             u4Alpha,u4MixSel,u4ColorKeyEn,u4ColorKey);
    Printf("\tPALETTE_ADDR=0x%x, PALETTE_LENGTH=%d, NEW_PALETTE=%d\n",
            u4PalAddr, u4PalLen, u4PalLoad);
    Printf("\tDec(DEC_EN:%d, DEC_MODE:%d, DEC_LINE:%d)\n", 
            u4DecEn, u4DecMode,u4DecLine);
    
    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_GetOriginalX(UINT32 u4Region, UINT32 * pu4Value)
{
    *pu4Value = (UINT32)_rAllRgnNode[u4Region].u4Original_X;
    return (INT32)OSD_RET_OK;
}


INT32 OSD_RGN_GetOriginalY(UINT32 u4Region, UINT32 * pu4Value)
{
    *pu4Value = (UINT32)_rAllRgnNode[u4Region].u4Original_Y;
    return (INT32)OSD_RET_OK;
}


//add for setting/getting the region list used by linux frame buffer
INT32 OSD_Set_FrameBuffer_RGN_LIST(UINT32 u4RgnList)
{
    u4RGNList_FrameBuffer = u4RgnList;
    return (INT32) OSD_RET_OK;
}


INT32 OSD_Get_FrameBuffer_RGN_LIST(UINT32 * pu4Value)
{
    if (OSD_MAX_NUM_RGN_LIST == u4RGNList_FrameBuffer)
    {
        return (INT32) OSD_RET_UNINIT;
    }
    if (pu4Value == NULL)
    {
        return (INT32) OSD_RET_INV_ARG;
    }
    *pu4Value = u4RGNList_FrameBuffer;
    return (INT32) OSD_RET_OK;
}


INT32 OSD_Get_FrameBuffer_Resolution(UINT32* pu4Width, UINT32* pu4Height)
{
    UINT32 u4width, u4height;

    if (OSD_MAX_NUM_RGN_LIST == u4RGNList_FrameBuffer)
    {
        return (INT32) OSD_RET_UNINIT;
    }
    if ((pu4Width == NULL) || (pu4Height == NULL))
    {
        return (INT32) OSD_RET_INV_ARG;
    }
    _OSD_RGN_GetInputWidth(_rAllRgnList[u4RGNList_FrameBuffer].i4Head, &u4width);
    _OSD_RGN_GetInputHeight(_rAllRgnList[u4RGNList_FrameBuffer].i4Head,
            &u4height);
    *pu4Width = u4width;
    *pu4Height = u4height;
    //Printf("OSD_Get_FrameBuffer_Resolution w=%d,h=%d\n",(*pu4Width),(*pu4Height));
    return (INT32) OSD_RET_OK;
}

//For OSD scroll
INT32 OSD_Vsync_UpdateScroll(void)
{
    UINT32 u4CurAddr;

    if (_u1ScrollMode == 1) // Scroll mode
    {
        OSD_RGN_Get(_u4ScrollRgn, OSD_RGN_BMP_ADDR, &u4CurAddr);

        u4CurAddr += _u4ScrollStep;

        if (u4CurAddr >= (_u4BaseAddr + _u4ScrollPitch))
        { // Need to loop
            u4CurAddr -= _u4ScrollPitch;
        }

        OSD_RGN_Set(_u4ScrollRgn, OSD_RGN_BMP_ADDR, u4CurAddr);
    }

    return (INT32) OSD_RET_OK;
}

INT32 OSD_RGN_SetScrollInfo(UINT32 u4Rgn, UINT32 u4BaseAddress, UINT32 u4Pitch,
        UINT32 u4Step)
{
    if (OSD_MAX_NUM_RGN_LIST == u4Rgn)
    {
        return (INT32) OSD_RET_UNINIT;
    }

    if ((u4BaseAddress == (UINT32) NULL) || (u4Pitch == (UINT32) NULL)
            || (u4Step == (UINT32) NULL))
    {
        return (INT32) OSD_RET_INV_ARG;
    }

    _u4ScrollRgn = u4Rgn;
    _u4BaseAddr = u4BaseAddress;
    _u4ScrollPitch = u4Pitch;
    _u4ScrollStep = u4Step;

    /*
     printf("OSD_RGN_SetScrollInfo : u4Rgn = %d, u4BaseAddress = 0x%x, u4Pitch = %d, u4Step = %d\n",
     _u4ScrollRgn, _u4BaseAddr, _u4ScrollPitch, _u4ScrollStep);
     */

    return (INT32) OSD_RET_OK;
}

INT32 OSD_SetScrollMode(UINT8 u1Mode)
{
    _u1ScrollMode = u1Mode;
    //printf("OSD_SetScrollMode : _u1ScrollMode = %d\n", _u1ScrollMode );
    return (INT32) OSD_RET_OK;
}
INT32 OSD_RGN_LIST_LineScaler(UINT32 u4RgnList, UINT32 u4BmpWidth, UINT32 u4BmpHeight,
                    void * pvBitmap, UINT32 eColorMode, UINT32 u4BmpPitch,
                    UINT32 u4DispX,UINT32 u4DispY, UINT32 u4DispW, UINT32 u4DispH)
{
    static INT32 i4RegionTop =-1;
    static INT32 i4RegionBottom =-1;
    UINT32 u4HalfBmpBufAddr = 0;
    UINT32 regionBotYPosition = 0;
    u4HalfBmpBufAddr = (UINT32) pvBitmap+u4BmpPitch*(u4BmpHeight/2);
    regionBotYPosition = u4DispY + u4DispH/2;
    ASSERT(regionBotYPosition < 2047);
    if(i4RegionTop == -1)
    {
        CHECK_FAIL(OSD_RGN_Create((UINT32 *)&i4RegionTop,u4BmpWidth,u4BmpHeight/2,
                        pvBitmap,eColorMode,u4BmpPitch,u4DispX,
                        u4DispY,u4DispW,u4DispH/2),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionTop,OSD_RGN_DISP_H,u4DispH/2),0);
    }
    else
    {
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionTop,OSD_RGN_BMP_W,u4BmpWidth),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionTop,OSD_RGN_BMP_H,u4BmpHeight/2),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionTop,OSD_RGN_BMP_ADDR,(UINT32)pvBitmap),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionTop,OSD_RGN_COLORMODE,eColorMode),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionTop,OSD_RGN_BMP_PITCH,u4BmpPitch),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionTop,OSD_RGN_POS_X,u4DispX),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionTop,OSD_RGN_POS_Y,u4DispY),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionTop,OSD_RGN_DISP_W,u4DispW),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionTop,OSD_RGN_DISP_H,u4DispH/2),0);
    }
    if(i4RegionBottom == -1)
    {
        CHECK_FAIL(OSD_RGN_Create((UINT32 *)&i4RegionBottom,u4BmpWidth,u4BmpHeight/2,
                        (void *)u4HalfBmpBufAddr,eColorMode,u4BmpPitch,
                        u4DispX,regionBotYPosition,u4DispW,u4DispH/2),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionBottom,OSD_RGN_DISP_H,u4DispH/2),0);
    }
    else
    {
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionBottom,OSD_RGN_BMP_W,u4BmpWidth),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionBottom,OSD_RGN_BMP_H,u4BmpHeight/2),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionBottom,OSD_RGN_BMP_ADDR,u4HalfBmpBufAddr),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionBottom,OSD_RGN_COLORMODE,eColorMode),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionBottom,OSD_RGN_BMP_PITCH,u4BmpPitch),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionBottom,OSD_RGN_POS_X,u4DispX),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionBottom,OSD_RGN_POS_Y,regionBotYPosition),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionBottom,OSD_RGN_DISP_W,u4DispW),0);
        CHECK_FAIL(OSD_RGN_Set((UINT32)i4RegionBottom,OSD_RGN_DISP_H,u4DispH/2),0);
    }
    ASSERT(i4RegionTop != -1);
    ASSERT(i4RegionBottom != -1);
    CHECK_FAIL(OSD_RGN_Insert((UINT32)i4RegionTop,u4RgnList),0);
    CHECK_FAIL(OSD_RGN_Insert((UINT32)i4RegionBottom,u4RgnList),0);
    
    return (INT32) OSD_RET_OK;
}

