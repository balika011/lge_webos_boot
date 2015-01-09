/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: imgrz_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_if.c
 *  main interfaces of image resizer driver
 *
 */

/*
    lint inhibition(s)
        506 - Constant value Boolean [MTK Rule 6.1.3]
        534 - Ignoring return value of function
        774 - Boolean within 'right side of && within if' always
              evaluates to True [Reference: file imgrz_if.c: lines 919, 935]
        826 - Suspicious pointer-to-pointer conversion (area too small)
        831 - Loss of sign (assignment) (signed char to unsigned long)
*/


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "vdp_if.h"
#include "b2r_if.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define REMAP_HWRZID        ((u4Which >= IMGRZ_NUM) ? 0 : _rIdx2ImgrzHwIdTbl[u4Which])

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
static const UINT32 _rIdx2ImgrzHwIdTbl[IMGRZ_NUM] = 
{
#if defined(CC_MT5890)  //oryx  3 imgrz hw
    E_FIRST_RESIZER, E_THIRD_RESIZER, E_SECND_RESIZER    //0, 1, 2 map to  0, 2, 1
#elif (defined(CC_MT5396) || defined(CC_MT5398)|| defined(CC_MT5399) || defined(CC_MT5882))    // 2 imgrz hw IC
    E_FIRST_RESIZER, E_SECND_RESIZER    //0, 1 map to  0, 1
#else    // others 1 imgrz hw
    E_FIRST_RESIZER    //0 map to  0
#endif
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  IMGRZ_Init
//
/** Initialize image resizer driver.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Init(void)
{
    _IMGRZ_Init(E_RZ_DEFAULT_ID);
}
void IMGRZ_Init_Ex(UINT32 u4Which)
{
    _IMGRZ_Init(REMAP_HWRZID);
}

void IMGRZ_Cfg_MMU_IO(BOOL MMU_Read, BOOL MMU_Write)
{
    _IMGRZ_Cfg_MMU_IO(E_RZ_DEFAULT_ID, MMU_Read, MMU_Write);
}

void IMGRZ_Cfg_MMU_IO_Ex(UINT32 u4Which, BOOL MMU_Read, BOOL MMU_Write)
{
    _IMGRZ_Cfg_MMU_IO(REMAP_HWRZID, MMU_Read, MMU_Write);
}

//---------------------------------------------------------------------------
//  IMGRZ_Reset
//
/** Reset engine.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Reset(void)
{
    _IMGRZ_Reset(E_RZ_DEFAULT_ID);
}

void IMGRZ_Reset_Ex(UINT32 u4Which)
{
    _IMGRZ_Reset(REMAP_HWRZID);
}

//---------------------------------------------------------------------------
//  IMGRZ_Wait
//
/** Wait for flushing engine completely.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Wait(void)
{
    _IMGRZ_Wait(E_RZ_DEFAULT_ID);
}
void IMGRZ_Wait_Ex(UINT32 u4Which)
{
    _IMGRZ_Wait(REMAP_HWRZID);
}


#if !defined(IMGRZ_RISC_MODE)
//---------------------------------------------------------------------------
//  IMGRZ_MwFlush
//
/** Flush cmdque and force engine to do operations (for MW use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_MwFlush_Ex(UINT32 u4Which)
{
    _IMGRZ_MwFlush(REMAP_HWRZID);
}
#endif


//---------------------------------------------------------------------------
//  IMGRZ_Flush
//
/** Flush cmdque and force engine to do operations (for Drv use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Flush(void)
{
    _IMGRZ_Flush(E_RZ_DEFAULT_ID);
}
void IMGRZ_Flush_Ex(UINT32 u4Which)
{
    _IMGRZ_Flush(REMAP_HWRZID);
}


//---------------------------------------------------------------------------
//  IMGRZ_Lock
//
/** Take the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Lock(void)
{
    _IMGRZ_Lock(E_RZ_DEFAULT_ID);
}
void IMGRZ_Lock_Ex(UINT32 u4Which)
{
    _IMGRZ_Lock(REMAP_HWRZID);
}


#if 0
//---------------------------------------------------------------------------
//  IMGRZ_TryLock
//
/** Try to take the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_TryLock(UINT32 u4Which)
{
    _IMGRZ_TryLock(u4Which);
}
#endif

//---------------------------------------------------------------------------
//  IMGRZ_Unlock
//
/** Release the imgrz resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Unlock(void)
{
    _IMGRZ_Unlock(E_RZ_DEFAULT_ID);
}
void IMGRZ_Unlock_Ex(UINT32 u4Which)
{
    _IMGRZ_Unlock(REMAP_HWRZID);
}


//---------------------------------------------------------------------------
//  IMGRZ_LockCmdque
//
/** Take the imgrz-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
//void IMGRZ_LockCmdque(UINT32 u4Which)
//{
//    _IMGRZ_LockCmdque(u4Which);
//}


//---------------------------------------------------------------------------
//  IMGRZ_UnlockCmdque
//
/** Release the imgrz-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_UnlockCmdque(void)
{
   //LOG(0, "IMGRZ_UnlockCmdque.....\n");

    _IMGRZ_UnlockCmdque(E_RZ_DEFAULT_ID);
}
/*
void IMGRZ_UnlockCmdque_Ex(UINT32 u4Which)
{
    _IMGRZ_UnlockCmdque(u4Which);
}*/

//---------------------------------------------------------------------------
//  IMGRZ_LockWaitIsr
//
/** Take the imgrz- waitIsr cmd resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
/*
void IMGRZ_LockWaitIsr(UINT32 u4Which)
{
    _IMGRZ_LockWaitIsr(u4Which);
}
*/

//---------------------------------------------------------------------------
//  IMGRZ_UnlockWaitIsr
//
/** Release the imgrz-waitIsr cmd resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_UnlockWaitIsr(void)
{
    _IMGRZ_UnlockWaitIsr(E_RZ_DEFAULT_ID);
}
/*
void IMGRZ_UnlockWaitIsr_Ex(UINT32 u4Which)
{
    _IMGRZ_UnlockWaitIsr(u4Which);
}*/
#if 0
//---------------------------------------------------------------------------
//  IMGRZ_QueryHwIdle
//
/** Query engine status.
 *
 *  @retval 1   Idle
 *  @retval 0   Busy
 */
//---------------------------------------------------------------------------
INT32 IMGRZ_QueryHwIdle(UINT32 u4Which)
{
    return _IMGRZ_QueryHwIdle(u4Which);
}

//---------------------------------------------------------------------------
//  IMGRZ_SetNotify
//
/** Set notify function (for MW use).
 *
 *  @param  pfnNotifyFunc   The callback function pointer
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_SetNotify(UINT32 u4Which, void (*pfnNotifyFunc)(UINT32))
{
    _IMGRZ_SetNotify(u4Which, pfnNotifyFunc);
}
#endif


//---------------------------------------------------------------------------
//  IMGRZ_ReInit
//
/** Re initial imgrz.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_ReInit(void)
{
    _IMGRZ_ReInit(E_RZ_DEFAULT_ID);
}
void IMGRZ_ReInit_Ex(UINT32 u4Which)
{
    _IMGRZ_ReInit(REMAP_HWRZID);
}


//---------------------------------------------------------------------------
//  IMGRZ_SetScaleOpt
//
/** Set scale option.
 *
 *  @param  eInOutType   Scale option
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_T eInOutType)
{
    _IMGRZ_SetScaleOpt(E_RZ_DEFAULT_ID, eInOutType);
}
void IMGRZ_SetScaleOpt_Ex(UINT32 u4Which, E_RZ_INOUT_TYPE_T eInOutType)
{
    _IMGRZ_SetScaleOpt(REMAP_HWRZID, eInOutType);
}


//---------------------------------------------------------------------------
//  IMGRZ_Scale
//
/** Execute scale operation.
 *
 *  @param  pvSclParam  scale parameters
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Scale(void *pvSclParam)
{
    _IMGRZ_Scale(E_RZ_DEFAULT_ID, pvSclParam);
}
void IMGRZ_Scale_Ex(UINT32 u4Which, void *pvSclParam)
{
    _IMGRZ_Scale(REMAP_HWRZID, pvSclParam);
}


//---------------------------------------------------------------------------
//  IMGRZ_GetScaleOpt
//
/** get scale option.
 *  @retval void
 */
//---------------------------------------------------------------------------
E_RZ_INOUT_TYPE_T IMGRZ_GetScaleOpt(void)
{
    return _IMGRZ_GetScaleOpt(E_RZ_DEFAULT_ID);
}
/*
E_RZ_INOUT_TYPE_T IMGRZ_GetScaleOpt_Ex(UINT32 u4Which)
{
    return _IMGRZ_GetScaleOpt(u4Which);
}*/
#if defined (CC_MT5890) 
void IMGRZ_DecompressUFO(void *pvSrc)
{    
    RZ_VDO_SCL_PARAM_SET_T rSclParam;    
    VDP_CAPTURE_INTO_T *pSrc = (VDP_CAPTURE_INTO_T *)pvSrc;
    
    x_memset((void *)&rSclParam, 0, sizeof(RZ_VDO_SCL_PARAM_SET_T));
    rSclParam.u4IsRsIn      = pSrc->u4IsRsIn;
    rSclParam.u4YSrcBase    = pSrc->u4AddrY;
    rSclParam.u4YSrcBufLen  = D_RZ_ALIGN16(pSrc->u4MemHSize);
    rSclParam.u4CSrcBufLen  = D_RZ_ALIGN16(pSrc->u4MemHSize);
    rSclParam.u4YSrcHOffset = 0;
    rSclParam.u4YSrcVOffset = 0;
    rSclParam.u4YSrcW       = pSrc->u4HSize;
    rSclParam.u4YSrcH       = pSrc->u4VSize;
    rSclParam.u4CSrcBase    = pSrc->u4AddrC;
    rSclParam.u4CSrcHOffset = 0;
    rSclParam.u4CSrcVOffset = 0;
    rSclParam.u4CSrcW       = pSrc->u4HSize >> 1;
    rSclParam.u4CSrcH       = pSrc->u4VSize >> 1;
    rSclParam.u4IsRsOut     = 0;

    rSclParam.u4OutMode     = (UINT32) E_RZ_VDO_OUTMD_420;
    rSclParam.u4IsVdo2Osd   = 0;
    rSclParam.u4YTgBase     = (UINT32) pSrc->u4DstY;
    rSclParam.u4YTgCM       = (UINT32) E_RZ_VDO_OUTMD_420;
    rSclParam.u4YTgBufLen   = rSclParam.u4YSrcBufLen;
    rSclParam.u4CTgBufLen   = rSclParam.u4CSrcBufLen;
    rSclParam.u4CTgBase     = pSrc->u4DstC;
    rSclParam.u4YTgHOffset  = 0;
    rSclParam.u4YTgVOffset  = 0;
    rSclParam.u4CTgW        = rSclParam.u4CSrcW;
    rSclParam.u4CTgH        = rSclParam.u4CSrcH ;
    rSclParam.u4YTgW        = rSclParam.u4YSrcW;
    rSclParam.u4YTgH        = rSclParam.u4YSrcH;
    rSclParam.u4SrcSwap     = pSrc->u4SrcSwap;//(UINT32)IMGRZ_SWAP;
    rSclParam.u4OutSwap = 0;
    rSclParam.u4ABlend = 0x80;

    //ufo
    rSclParam.eUfoType = (RZ_UFO_TYPE)pSrc->u4UFOType;
    rSclParam.u4UfoEn = TRUE;
    rSclParam.u4UfoYBufSZSA = pSrc->u4AddrYLen;
    rSclParam.u4UfoCBufSZSA = pSrc->u4AddrCLen;

    IMGRZ_Lock_Ex(E_FIRST_RESIZER);
    IMGRZ_ReInit_Ex(E_FIRST_RESIZER);
    IMGRZ_SetScaleOpt_Ex(E_FIRST_RESIZER, E_RZ_INOUT_TYPE_VDOMD);
    IMGRZ_Scale_Ex(E_FIRST_RESIZER, &rSclParam);
    IMGRZ_Flush_Ex(E_FIRST_RESIZER);
    IMGRZ_Wait_Ex(E_FIRST_RESIZER);
    IMGRZ_Unlock_Ex(E_FIRST_RESIZER);
}
#endif

//---------------------------------------------------------------------------
//  IMGRZ_SetDigest
//
/** Set a video output frame to a thumbnail result
 *
 *  @param  pvSrc  source parameters
 *  @param  pvDst  distination parameters
 *  @param  fgOnePass FALSE: 2pass (only video scaling), TRUE: 1 pass (scaling and color conv)
 *  @param  u4WorkingBuf  working buffer address for 2 pass
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_SetDigest(void *pvSrc, void *pvDst, BOOL fgOnePass, UINT32 u4WorkingBuf)
{
    RZ_VDO_SCL_PARAM_SET_T rSclParam;
    VDP_CAPTURE_INTO_T *pSrc = (VDP_CAPTURE_INTO_T *)pvSrc;
    VDP_THUMBNAIL_INFO_T *pDst = (VDP_THUMBNAIL_INFO_T *)pvDst;
    x_memset((void *)&rSclParam, 0, sizeof(RZ_VDO_SCL_PARAM_SET_T));
    rSclParam.u4IsRsIn      = pSrc->u4IsRsIn;
    rSclParam.u4YSrcBase    = pSrc->u4AddrY;
    rSclParam.u4YSrcBufLen  = D_RZ_ALIGN16(pSrc->u4MemHSize);
    rSclParam.u4CSrcBufLen  = D_RZ_ALIGN16(pSrc->u4MemHSize);
    rSclParam.u4YSrcHOffset = 0;
    rSclParam.u4YSrcVOffset = 0;
    rSclParam.u4YSrcW       = pSrc->u4HSize;
    rSclParam.u4YSrcH       = pSrc->u4VSize;
    rSclParam.u4CSrcBase    = pSrc->u4AddrC;
    rSclParam.u4CSrcHOffset = 0;
    rSclParam.u4CSrcVOffset = 0;
    rSclParam.u4CSrcW       = pSrc->u4HSize >> 1;
    rSclParam.u4CSrcH       = pSrc->u4VSize >> 1;
    rSclParam.u4IsRsOut     = 1;

    if (fgOnePass)
    {
        //method 1: Y/C --> AYCbCr8888, 1 pass
        rSclParam.u4OutMode     = (UINT32) E_RZ_VDO_OUTMD_444;
        rSclParam.u4IsVdo2Osd   = 1;
        rSclParam.u4YTgBase     = (UINT32) pDst->pucCanvasBuffer;
        rSclParam.u4YTgCM       = (UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
        rSclParam.u4YTgBufLen   = D_RZ_ALIGN16(pDst->u4CanvasPitch);
        rSclParam.u4CTgBufLen   = D_RZ_ALIGN16(pDst->u4CanvasPitch);
        rSclParam.u4CTgBase     = (UINT32) pDst->pucCanvasBuffer;
        rSclParam.u4YTgHOffset  = pDst->u4ThumbnailX;
        rSclParam.u4YTgVOffset  = pDst->u4ThumbnailY;
        rSclParam.u4CTgW        = pDst->u4ThumbnailWidth;
        rSclParam.u4CTgH        = pDst->u4ThumbnailHeight;
    }
    else
    {
        //method 2: Y/C --> RGB, 2 pass
        // 1st pass: VDO scaling
        rSclParam.u4OutMode     = (UINT32) E_RZ_VDO_OUTMD_420;
        rSclParam.u4IsVdo2Osd   = 0;
        rSclParam.u4YTgBase     = u4WorkingBuf;
        rSclParam.u4YTgCM       = (UINT32) E_RZ_VDO_OUTMD_420;
        rSclParam.u4YTgBufLen   = D_RZ_ALIGN16(pDst->u4ThumbnailWidth);
        rSclParam.u4CTgBufLen   = D_RZ_ALIGN16(pDst->u4ThumbnailWidth);
        rSclParam.u4CTgBase     = u4WorkingBuf + (rSclParam.u4YTgBufLen * pDst->u4ThumbnailHeight);
        rSclParam.u4YTgHOffset  = 0;
        rSclParam.u4YTgVOffset  = 0;
        rSclParam.u4CTgW        = pDst->u4ThumbnailWidth >> 1;
        rSclParam.u4CTgH        = pDst->u4ThumbnailHeight >> 1;
    }

    rSclParam.u4YTgW        = pDst->u4ThumbnailWidth;
    rSclParam.u4YTgH        = pDst->u4ThumbnailHeight;
    rSclParam.u4SrcSwap     = pSrc->u4SrcSwap;//(UINT32)IMGRZ_SWAP;
    rSclParam.u4OutSwap=0;
    rSclParam.u4ABlend = 0x80;

    IMGRZ_Lock_Ex(E_FIRST_RESIZER);
    IMGRZ_ReInit_Ex(E_FIRST_RESIZER);
    IMGRZ_SetScaleOpt_Ex(E_FIRST_RESIZER, E_RZ_INOUT_TYPE_VDOMD);
    IMGRZ_Scale_Ex(E_FIRST_RESIZER, &rSclParam);
    IMGRZ_Flush_Ex(E_FIRST_RESIZER);
    IMGRZ_Wait_Ex(E_FIRST_RESIZER);
    IMGRZ_Unlock_Ex(E_FIRST_RESIZER);
}
//===================
// OSD mode
//===================

//---------------------------------------------------------------------------
//  IMGRZ_Break
//
/** Save imgrz parameters for breaking
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Break(void)
{
    _IMGRZ_Break(E_RZ_DEFAULT_ID);
}

void IMGRZ_Break_Ex(UINT32 u4Which)
{
    _IMGRZ_Break(REMAP_HWRZID);
}


//---------------------------------------------------------------------------
//  IMGRZ_Resume
//
/** Resume the process of imgrz of jpg mcu row mode
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_Resume(void)
{
    _IMGRZ_Resume(E_RZ_DEFAULT_ID);
}

void IMGRZ_Resume_Ex(UINT32 u4Which)
{
    _IMGRZ_Resume(REMAP_HWRZID);
}

//---------------------------------------------------------------------------
//  IMGRZ_OFF
//
/** disable clock of imgrz
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_OFF(void)
{
    _IMGRZ_OFF(E_RZ_DEFAULT_ID);
}

void IMGRZ_OFF_Ex(UINT32 u4Which)
{
    _IMGRZ_OFF(REMAP_HWRZID);
}

//-------------------------------------------------------------------------
/** _IMGRZ_GetScaleLine
 *  get current scale vertical line
 *
 */
//-------------------------------------------------------------------------
UINT32 IMGRZ_GetScaleLine(void)
{
    return _IMGRZ_GetScaleLine(E_RZ_DEFAULT_ID);
}

UINT32 IMGRZ_GetScaleLine_Ex(UINT32 u4Which)
{
    return _IMGRZ_GetScaleLine(REMAP_HWRZID);
}

#if 0
//---------------------------------------------------------------------------
//  IMGRZ_RegInfo
//
/** get imgrz register address and size for
 *  register dump
 *  @retval void
 */
//---------------------------------------------------------------------------
void IMGRZ_RegInfo(UINT32 u4Which, UINT32 *pu4Addr,UINT32 *pu4Size)
{
  return _IMGRZ_RegInfo(u4Which, pu4Addr,pu4Size);
}
#endif

#ifdef CC_SUPPORT_STR
BOOL IMGRZ_CkgenOnOff(UINT32 u4Which, BOOL fgEnable)
{
#if defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890) || defined(CC_MT5882)
#define BLOCK_RST_CFG0 0x1C0
#define IMGRZ0_RST ((UINT32)1 << 22)
#define BLOCK_CKEN_CFG0 0x1C8
#define IMGRZ0_CKEN ((UINT32)1 << 22)
#else
#define BLOCK_RST_CFG0 0x270
#define IMGRZ0_RST ((UINT32)1 << 3)
#define BLOCK_CKEN_CFG0 0x278
#define IMGRZ0_CKEN ((UINT32)1 << 3)
#endif

    if (u4Which == E_FIRST_RESIZER)
    {
        if (fgEnable)
        {
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~IMGRZ0_RST);
            IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) | IMGRZ0_CKEN);
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) | IMGRZ0_RST);
        }
        else
        {
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~IMGRZ0_RST);
            IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) & ~IMGRZ0_CKEN);
        }
    }

#ifdef IMGRZ_HW_2ND_RZ
    #if defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890) || defined(CC_MT5882)
    #define IMGRZ1_RST ((UINT32)1 << 23)
    #define IMGRZ1_CKEN ((UINT32)1 << 23)
    #else
    #define IMGRZ1_RST ((UINT32)1 << 0)
    #define IMGRZ1_CKEN ((UINT32)1 << 0)
    #endif
    else if (u4Which == E_SECND_RESIZER)
    {
        if (fgEnable)
        {
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~IMGRZ1_RST);
            IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) | IMGRZ1_CKEN);
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) | IMGRZ1_RST);
        }
        else
        {
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG0, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG0) & ~IMGRZ1_RST);
            IO_WRITE32(CKGEN_BASE, BLOCK_CKEN_CFG0, IO_READ32(CKGEN_BASE, BLOCK_CKEN_CFG0) & ~IMGRZ1_CKEN);
        }
    }
#endif /*IMGRZ_HW_2ND_RZ*/
#ifdef IMGRZ_HW_3NM_RZ
    #if defined(CC_MT5890)
    #define BLOCK_RST_CFG4 0x1d0
    #define BLOCK_CLK_CFG5 0x1cc
    #define IMGRZ2_RST ((UINT32)1 << 2)
    #define IMGRZ2_CKEN ((UINT32)1 << 2)
    #endif
    else if (u4Which == E_THIRD_RESIZER)
    {
        if (fgEnable)
        {
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG4, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG4) & ~IMGRZ2_RST);
            IO_WRITE32(CKGEN_BASE, BLOCK_CLK_CFG5, IO_READ32(CKGEN_BASE, BLOCK_CLK_CFG5) | IMGRZ2_CKEN);
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG4, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG4) | IMGRZ2_RST);
        }
        else
        {
            IO_WRITE32(CKGEN_BASE, BLOCK_RST_CFG4, IO_READ32(CKGEN_BASE, BLOCK_RST_CFG4) & ~IMGRZ2_RST);
            IO_WRITE32(CKGEN_BASE, BLOCK_CLK_CFG5, IO_READ32(CKGEN_BASE, BLOCK_CLK_CFG5) & ~IMGRZ2_CKEN);
        }
    }
#endif /*IMGRZ_HW_3NM_RZ*/
    UNUSED(fgEnable);
    return TRUE;
}
#endif

void IMGRZ_AYUV_ARGB_Conv(UINT32  u4TgBase,UINT32 u4BufLen,UINT32 u4Width, UINT32 u4Height,
    UINT32 u4OffsetX,  UINT32 u4OffsetY, UINT32 u4MMUTableAddr, BOOL fgAYUV2ARGB)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    x_memset((void *)&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));

    #ifdef IMGRZ_HW_IO_MMU
    if(u4MMUTableAddr!=0)
    {
		rSclParam.u4RzIOMMU_MMUTableAddr = u4MMUTableAddr;
    }
    #endif

    // set scaling parameters
    rSclParam.u4SrcBase     =  u4TgBase;
    rSclParam.u4IsSrcDirCM  = 1;
    rSclParam.u4SrcCM       = 3;
    rSclParam.u4SrcBufLen   = u4BufLen;
    rSclParam.u4SrcHOffset  = u4OffsetX;
    rSclParam.u4SrcVOffset  = u4OffsetY;
    rSclParam.u4SrcW        = u4Width;
    rSclParam.u4SrcH        = u4Height;

    rSclParam.u4TgBase      = u4TgBase;
    rSclParam.u4TgCM        = 3;
    rSclParam.u4TgBufLen    = u4BufLen;
    rSclParam.u4TgHOffset   = u4OffsetX;
    rSclParam.u4TgVOffset   = u4OffsetY;
    rSclParam.u4TgW         = u4Width;
    rSclParam.u4TgH         = u4Height;

    rSclParam.u4Csc_enable      = 1;
    rSclParam.csc_src_format      = (E_RZ_CSC_FORMAT_T)(fgAYUV2ARGB);
    rSclParam.csc_des_format      = (E_RZ_CSC_FORMAT_T) (!fgAYUV2ARGB);

    IMGRZ_Lock();
    IMGRZ_ReInit();

    #ifdef IMGRZ_HW_IO_MMU
    if(u4MMUTableAddr!=0)
    {
		IMGRZ_Cfg_MMU_IO((BOOL)1,(BOOL)1);
    }
    #endif

    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

    return;
}

void IMGRZ_AYUV_ARGB_Conv_Ex(UINT32 u4Which, UINT32  u4TgBase,UINT32 u4BufLen,UINT32 u4Width, UINT32 u4Height,
    UINT32 u4OffsetX,  UINT32 u4OffsetY, UINT32 u4MMUTableAddr, BOOL fgAYUV2ARGB)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    x_memset((void *)&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));

    #ifdef IMGRZ_HW_IO_MMU
    if(u4MMUTableAddr!=0)
    {
		rSclParam.u4RzIOMMU_MMUTableAddr = u4MMUTableAddr;
    }
    #endif

    // set scaling parameters
    rSclParam.u4SrcBase     =  u4TgBase;
    rSclParam.u4IsSrcDirCM  = 1;
    rSclParam.u4SrcCM       = 3;
    rSclParam.u4SrcBufLen   = u4BufLen;
    rSclParam.u4SrcHOffset  = u4OffsetX;
    rSclParam.u4SrcVOffset  = u4OffsetY;
    rSclParam.u4SrcW        = u4Width;
    rSclParam.u4SrcH        = u4Height;

    rSclParam.u4TgBase      = u4TgBase;
    rSclParam.u4TgCM        = 3;
    rSclParam.u4TgBufLen    = u4BufLen;
    rSclParam.u4TgHOffset   = u4OffsetX;
    rSclParam.u4TgVOffset   = u4OffsetY;
    rSclParam.u4TgW         = u4Width;
    rSclParam.u4TgH         = u4Height;

    rSclParam.u4Csc_enable      = 1;
    rSclParam.csc_src_format      = (E_RZ_CSC_FORMAT_T)(fgAYUV2ARGB);
    rSclParam.csc_des_format      = (E_RZ_CSC_FORMAT_T) (!fgAYUV2ARGB);
	//the imgrz id u4Which will be remap in IMGRZ_XX function call.
    IMGRZ_Lock_Ex(u4Which);
    IMGRZ_ReInit_Ex(u4Which);

    #ifdef IMGRZ_HW_IO_MMU
    if(u4MMUTableAddr!=0)
    {
		IMGRZ_Cfg_MMU_IO_Ex(u4Which, (BOOL)1,(BOOL)1);
    }
    #endif

    IMGRZ_SetScaleOpt_Ex(u4Which, E_RZ_INOUT_TYPE_OSDMD);
    IMGRZ_Scale_Ex(u4Which, &rSclParam);
    IMGRZ_Flush_Ex(u4Which);
    IMGRZ_Wait_Ex(u4Which);
    IMGRZ_Unlock_Ex(u4Which);

    return;
}

//-------------------------------------------------------------------------
/**IMGRZ_DMA
 *  Move Dram data form u4SrcAddr to u4DstAddr, size is u4Pitch x u4Height
 *  u4SrcAddr, u4DstAddr, u4Pitch should be 16 byte aligement.
 */
void IMGRZ_DMA_Ex(UINT32 u4Which, UINT32 u4SrcAddr,UINT32 u4DstAddr, UINT32 u4Pitch,UINT32 u4Height, UINT32 u4MMUTableAddr)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    x_memset((void *)&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));
    // set scaling parameters
    rSclParam.u4SrcBase     = u4SrcAddr;
    rSclParam.u4IsSrcDirCM  = IMGRZ_TRUE;
    rSclParam.u4SrcCM       = 3;
    rSclParam.u4SrcBufLen   = u4Pitch;
    rSclParam.u4SrcHOffset  = 0;
    rSclParam.u4SrcVOffset  = 0;
    rSclParam.u4SrcW        = u4Pitch>>2;
    rSclParam.u4SrcH        = u4Height;

    rSclParam.u4TgBase      = u4DstAddr;
    rSclParam.u4TgCM        = 3;
    rSclParam.u4TgBufLen    = u4Pitch;
    rSclParam.u4TgHOffset   = 0;
    rSclParam.u4TgVOffset   = 0;
    rSclParam.u4TgW         = u4Pitch>>2;
    rSclParam.u4TgH         = u4Height;
    rSclParam.fgRBExChange  = TRUE;
	//for IMGRZ ex functions, the imgrz id will be remapped
    IMGRZ_Lock_Ex(u4Which);
    IMGRZ_ReInit_Ex(u4Which);
#ifdef IMGRZ_HW_IO_MMU
    if(u4MMUTableAddr!=0)
    {
        rSclParam.u4RzIOMMU_MMUTableAddr = u4MMUTableAddr;
		IMGRZ_Cfg_MMU_IO_Ex(u4Which,(BOOL)1,(BOOL)1);
    }
#endif
    IMGRZ_SetScaleOpt_Ex(u4Which, E_RZ_INOUT_TYPE_OSDMD);
    IMGRZ_Scale_Ex(u4Which, &rSclParam);
    IMGRZ_Flush_Ex(u4Which);
    IMGRZ_Wait_Ex(u4Which);
    IMGRZ_Unlock_Ex(u4Which);
    return;
}
//-------------------------------------------------------------------------
/**IMGRZ_DMA
 *  Move Dram data form u4SrcAddr to u4DstAddr, size is u4Pitch x u4Height
 *  u4SrcAddr, u4DstAddr, u4Pitch should be 16 byte aligement.
 */
//-------------------------------------------------------------------------
void IMGRZ_DMA(UINT32 u4SrcAddr,UINT32 u4DstAddr, UINT32 u4Pitch,UINT32 u4Height, UINT32 u4MMUTableAddr)
{
    return IMGRZ_DMA_Ex((IMGRZ_NUM-1), u4SrcAddr, u4DstAddr, u4Pitch, u4Height, u4MMUTableAddr);
}

#if 0
//-------------------------------------------------------------------------
/**IMGRZ_OSD_CLR_MODE_CONV
 *  Move Dram data form u4SrcAddr to u4DstAddr, size is u4Pitch x u4Height
 *  u4SrcAddr, u4DstAddr, u4Pitch should be 16 byte aligement.
 */
//-------------------------------------------------------------------------
void IMGRZ_OSD_CLR_MODE_CONV(UINT32 u4Which, UINT32 u4SrcAddr,UINT32 u4DstAddr, UINT32 u4Pitch, UINT32 u4Height, UINT32 fromClrMode, UINT32 toClrMode,BOOL fgDoDither)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    x_memset((void *)&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));
    // set scaling parameters
    rSclParam.u4SrcBase     = u4SrcAddr;
    rSclParam.u4IsSrcDirCM  = IMGRZ_TRUE;
    rSclParam.u4SrcCM       = fromClrMode;
    rSclParam.u4SrcBufLen   = u4Pitch;
    rSclParam.u4SrcHOffset  = 0;
    rSclParam.u4SrcVOffset  = 0;
    rSclParam.u4SrcW        = u4Pitch>>2;
    rSclParam.u4SrcH        = u4Height;

    rSclParam.u4TgBase      = u4DstAddr;
    rSclParam.u4TgCM        = toClrMode;
    if(OSD_CLR_MODE_CONV_OSD_565 == toClrMode)
    {
        rSclParam.u4TgBufLen    = u4Pitch>>1;    /*565*/
    }
    else if(OSD_CLR_MODE_CONV_OSD_8888== toClrMode)
    {
        rSclParam.u4TgBufLen    = u4Pitch;  /*8888*/
    }
    else
    {
        rSclParam.u4TgBufLen    = u4Pitch;
    }
    rSclParam.u4TgHOffset   = 0;
    rSclParam.u4TgVOffset   = 0;
    rSclParam.u4TgW         = u4Pitch>>2;
    rSclParam.u4TgH         = u4Height;
	rSclParam.fgDoDither = fgDoDither;

    IMGRZ_Lock_Ex(u4Which);
    IMGRZ_ReInit_Ex(u4Which);
    IMGRZ_SetScaleOpt_Ex(u4Which, E_RZ_INOUT_TYPE_OSDMD);
    IMGRZ_Scale_Ex(u4Which, &rSclParam);
    IMGRZ_Flush_Ex(u4Which);
    IMGRZ_Wait_Ex(u4Which);
    IMGRZ_Unlock_Ex(u4Which);
    return;
}
#endif

