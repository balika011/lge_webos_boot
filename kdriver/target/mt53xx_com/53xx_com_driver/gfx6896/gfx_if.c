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
 * $RCSfile: gfx_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_if.c
 *  main interfaces of gfx driver
 *
 */

/*
 lint inhibition(s)
 506 - Constant value Boolean [MTK Rule 6.1.3]
 534 - Ignoring return value of function
 774 - Boolean within 'right side of && within if' always
 evaluates to True [Reference: file gfx_if.c: lines 919, 935]
 826 - Suspicious pointer-to-pointer conversion (area too small)
 831 - Loss of sign (assignment) (signed char to unsigned long)
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


#include "gfx_if.h"
#include "gfx_drvif.h"
#include "gfx_cmdque.h"
#include "vdp_if.h"

#include "osd_if.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "gfx_common_if.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//#ifndef CC_MTK_LOADER
#define UI_WIDTH    960
#define UI_HEIGHT    540
#define UI_BPP    4
#define OSD_ALIGN_SIZE 64
//#endif

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
/*typedef struct _GFX_DIGEST_INFO_T
 {
 UINT8                     *pcBuffer;
 GFX_COLOR_MODE_T       eColormode;
 UINT32                    u4CanvasWidth;
 UINT32                    u4CanvasHeight;
 UINT32                    u4X;
 UINT32                    u4Y;
 UINT32                    u4Width;
 UINT32                    u4Height;
 }   GFX_DIGEST_INFO_T;
 */

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
//EXTERN INT32 _OSD_3D_GetOsd2LShiftEn(UINT32 *pu4Value);
//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static INT32 _i4GfxInDMA = (INT32) FALSE;
static void *_pvDMATag;
static BOOL b3DShiftEnable = FALSE;

//#ifndef CC_MTK_LOADER

//#endif


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

#if defined(CC_SUPPORT_STR)||defined(CC_LG_SNAP_SHOT)

#define OSTAG_GFXRESET_REG       (IO_ADDR(0xd1c0))
#define OSTAG_GFXRESET_BIT       (1 << 6)
#define OSTAG_GFXBCLOCK_REG      (IO_ADDR(0xd1c8))
#define OSTAG_GFXRCLOCK_BIT      (1 << 6)

#ifdef LINUX_TURNKEY_SOLUTION
extern void mt53fb_setcmdque(void);
#endif

void GFX_pm_suspend(void)
{
    GFX_Wait();
	IO_WRITE32MSK(OSTAG_GFXRESET_REG, 0, 0x0, OSTAG_GFXRESET_BIT);
	IO_WRITE32MSK(OSTAG_GFXBCLOCK_REG, 0, 0x0, OSTAG_GFXRCLOCK_BIT);
}

void GFX_pm_resume(void)
{
    IO_WRITE32MSK(OSTAG_GFXRESET_REG, 0, OSTAG_GFXRESET_BIT, OSTAG_GFXRESET_BIT);
    IO_WRITE32MSK(OSTAG_GFXBCLOCK_REG, 0, OSTAG_GFXRCLOCK_BIT, OSTAG_GFXRCLOCK_BIT);
    
    GFX_Reset();
    GFX_CmdQueSetup();
    GFX_CmdQueReset();
    GFX_UnlockCmdque();
    GFX_Unlock();
}


#endif /*CC_SUPPORT_STR*/

//---------------------------------------------------------------------------
//  GFX_Init
//
/** Initialize gfx driver.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Init(void)
{
    _GFX_Init();
}

//---------------------------------------------------------------------------
//  GFX_Reset
//
/** Reset engine.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Reset(void)
{
    _GFX_Reset();
}

//---------------------------------------------------------------------------
//  GFX_Wait
//
/** Wait for flushing engine completely.
 *  
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Wait(void)
{
    _GFX_Wait();
}

void GFX_MMU_Set_Enable(UINT32 u4_enable)
{
    GFX_MMU_T       t_mmu;

    t_mmu.u4_init           = 0x1;
    t_mmu.u4_enable         = u4_enable;
    t_mmu.u4_op_md          = 0x0;
    t_mmu.u4_src_rw_mmu     = 0x1;
    t_mmu.u4_dst_rw_mmu     = 0x1;
    t_mmu.u4_vgfx_ord       = 0x0;
    t_mmu.u4_vgfx_slva      = 0x0;
    t_mmu.u4_pgt            = 0x0;
    
    _GFX_MMU_Set_Cfg(&t_mmu);

    return;
}


void GFX_MMU_Setting(UINT32 u4Enable, UINT32 u4SrcEnable, UINT32 u4DstEnable)
{
    GFX_MMU_T       t_mmu;

    t_mmu.u4_init           = 0x1;
    t_mmu.u4_enable         = u4Enable;
    t_mmu.u4_op_md          = 0x0;


    t_mmu.u4_src_rw_mmu     = u4SrcEnable;
    t_mmu.u4_dst_rw_mmu     = u4DstEnable;

    t_mmu.u4_vgfx_ord       = 0x0;
    t_mmu.u4_vgfx_slva      = 0x0;
    t_mmu.u4_pgt            = 0x0;
    
    _GFX_MMU_Set_Cfg(&t_mmu);

    return;
}
//---------------------------------------------------------------------------
//  GFX_Wait_vdp
//
/** Wait for flushing engine completely.
 *  
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Wait_Vdp(void)
{
    _GFX_Wait_Vdp();
}

//---------------------------------------------------------------------------
//  GFX_Wait
//
/** Wait for flushing engine completely.
 *  
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Wait_Timeout(INT32 i4Timeout)
{
    _GFX_Wait_Timeout(i4Timeout);
}

//---------------------------------------------------------------------------
//  GFX_MwFlush
//
/** Flush cmdque and force engine to do operations (for MW use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_MwFlush(void)
{
    _GFX_MwFlush();
}

//---------------------------------------------------------------------------
//  GFX_Flush
//
/** Flush cmdque and force engine to do operations (for Drv use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Flush(void)
{
    _GFX_Flush();
}

//---------------------------------------------------------------------------
//  GFX_DFB_Flush
//
/** Flush cmdque and force engine to do operations (for Drv use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_DFB_Flush(void)
{
    GFX_Flush_Action();
}

//---------------------------------------------------------------------------
//  GFX_Flush_Invalid_DCache
//
/** Take the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Flush_Invalid_DCache(void)
{
    GFX_FlushInvalidateDCacheMultipleLine();
}

//---------------------------------------------------------------------------
//  GFX_MMU_Set_Enable_Fg
//
/** Set gfx mmu disable/enable
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_MMU_Set_Cfg(void* pt_gfx_mmu)
{
    _GFX_MMU_Set_Cfg((GFX_MMU_T*) pt_gfx_mmu);
}

//---------------------------------------------------------------------------
//  GFX_Lock
//
/** Take the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Lock(void)
{
    _GFX_Lock();
}


//---------------------------------------------------------------------------
//  GFX_TryLock
//
/** Try to take the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_TryLock(void)
{
    _GFX_TryLock();
}

//---------------------------------------------------------------------------
//  GFX_Unlock
//
/** Release the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Unlock(void)
{
    _GFX_Unlock();
}

//---------------------------------------------------------------------------
//  GFX_LockCmdque
//
/** Take the gfx-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_LockCmdque(void)
{
    _GFX_LockCmdque();
}

//---------------------------------------------------------------------------
//  GFX_UnlockCmdque
//
/** Release the gfx-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_UnlockCmdque(void)
{
    _GFX_UnlockCmdque();
}

//---------------------------------------------------------------------------
//  GFX_QueryHwIdle
//
/** Query engine status.
 *
 *  @retval 1   Idle
 *  @retval 0   Busy
 */
//---------------------------------------------------------------------------
INT32 GFX_QueryHwIdle(void)
{
    return _GFX_QueryHwIdle();
}

//---------------------------------------------------------------------------
//  GFX_SetNotify
//
/** Set notify function (for MW use).
 *
 *  @param  pfnNotifyFunc   The callback function pointer
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetNotify(void(*pfnNotifyFunc)( UINT32))
{
    _GFX_SetNotify(pfnNotifyFunc);
}

//---------------------------------------------------------------------------
//  GFX_SetDst
//
/** Set destination region.
 *
 *  @param  pu1Base     The base address of the dst region
 *  @param  u4ColorMode The color mode of the dst region
 *  @param  u4Pitch     The pitch of the dst region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetDst(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch)
{
    _GFX_SetDst(pu1Base, u4ColorMode, u4Pitch);
}

//---------------------------------------------------------------------------
//  GFX_SetSrc
//
/** Set source region.
 *
 *  @param  pu1Base     The base address of the src region
 *  @param  u4ColorMode The color mode of the src region
 *  @param  u4Pitch     The pitch of the src region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetSrc(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch)
{
    _GFX_SetSrc(pu1Base, u4ColorMode, u4Pitch);
}

//---------------------------------------------------------------------------
//  GFX_SetAlpha
//
/** Set global alpha.
 *
 *  @param  u4Alpha     The global alpha
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetAlpha(UINT32 u4Alpha)
{
    _GFX_SetAlpha(u4Alpha);
}

void GFX_SetMaxAlphaEn(UINT32 u4Enable)
{
    _GFX_SetMaxAlphaEn(u4Enable);
}

//---------------------------------------------------------------------------
//  GFX_SetXORColor
//
/** Set XOR color.
 *
 *  @param  u4Color     The rectangular color
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetXORColor(UINT32 u4Color)
{
    _GFX_SetXORColor(u4Color);
}
//---------------------------------------------------------------------------
//  GFX_Set_PRE_COLORIZE
//
/** Set PRE_COLORIZE
 *
 *  @param  u4PreColorize     The value of PRE_COLORIZE  [BOOL]
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Set_PRE_COLORIZE(BOOL u4PreColorize)
{
    _GFX_Set_PRE_COLORIZE(u4PreColorize);
}

//---------------------------------------------------------------------------
//  _GFX_Set_SrcAlReplace
//
/** Set SrcAlReplace
 *
 *  @param  u4SrcAlRep     The value of SrcAlReplace  [BOOL]
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Set_SrcAlReplace(UINT32 u4SrcAlRep)
{
    _GFX_Set_SrcAlReplace(u4SrcAlRep);
}


//---------------------------------------------------------------------------
//  GFX_Set_FlashColCnvInfo
//
/** Set FlashColCnvInfo
 *
 *  @param  pu1SrcY     The value of src Y Adress  
 *  @param  u4SrcYPitch     The number of src Y bytes  
 *  @param  pu1SrcCbCr     The value of src CbCr Adress  
 *  @param  u4SrcCbCrPitch     The number of src CbCr bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  pu1SrcY     The number of src Y bytes  
 *  @param  u4SrcWidth     The value of src Y Width   
 *  @param  u4SrcHeight     The value of src Y Width   
 *  @retval void
 */
//---------------------------------------------------------------------------

void GFX_Set_FlashColCnvInfo(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight)
{
    _GFX_Set_FlashColCnvInfo(pu1SrcY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            u4DstAdress, u4SrcWidth, u4SrcHeight);
}

//---------------------------------------------------------------------------
//  GFX_Set_FlashColCnvInfo_WithPitch
//
/** Set FlashColCnvInfo
 *
 *  @param  pu1SrcY     The value of src Y Adress  
 *  @param  u4SrcYPitch     The number of src Y bytes  
 *  @param  pu1SrcCbCr     The value of src CbCr Adress  
 *  @param  u4SrcCbCrPitch     The number of src CbCr bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  pu1SrcY     The number of src Y bytes  
 *  @param  u4SrcWidth     The value of src Y Width   
 *  @param  u4SrcHeight     The value of src Y Width   
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Set_FlashColCnvInfo_WithPitch(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4Dst_Pitch)
{
    _GFX_Set_FlashColCnvInfo_WithPitch(pu1SrcY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            u4DstAdress, u4SrcWidth, u4SrcHeight, u4Dst_Pitch);
}

//---------------------------------------------------------------------------
//  GFX_Set_SrcDst
//
/** Set Source and Dst information
 *
 *  @param  pu1SrcY         The value of src Y Adress  
 *  @param  u4SrcYPitch     The number of src Y bytes  
 *  @param  pu1SrcCbCr      The value of src CbCr Adress  
 *  @param  u4SrcCbCrPitch  The number of src CbCr bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  u4DstPitch      The value of Dst pitch
 *  @param  u4ColorMode     The value of Dst ColorMode
 *  @param  u4DstX          The position of dst x
 *  @param  u4DstY          The position of dst y
 *  @param  u4SrcWidth      The value of Source Width   
 *  @param  u4SrcHeight     The value of Source Height   
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Set_SrcDst(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4DstPitch,UINT32 u4DstColorMode,UINT32 u4DstX,UINT32 u4DstY,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight)
{
    _GFX_Set_SrcDst(pu1SrcY,u4SrcYPitch,pu1SrcCbCr,u4SrcCbCrPitch,u4DstAdress,
                    u4DstPitch,u4DstColorMode,u4DstX,u4DstY,u4SrcWidth,u4SrcHeight);
}
//---------------------------------------------------------------------------
//  GFX_Set_ColConvInfo
//
/** Set colorformat conversion information
 *
 *  @param  u4YCFmt         The yc format
 *  @param  u4SwapMode      The swap mode
 *  @param  u4VidStd        The video standard
 *  @param  u4VidSys        The video system 
 *  @param  u4FieldPic      The choose of field  picture  ( 0 frame pic / 1 field pic )
 *  @param  u4Alpha         The global alpha 
 *  @retval void
 */
void GFX_Set_ColConvInfo(UINT32 u4YCFmt, UINT32 u4SwapMode, UINT32 u4VidStd,
        UINT32 u4VidSys,UINT32 u4FieldPic,UINT32 u4Alpha)
{
    _GFX_Set_ColConvInfo(u4YCFmt,u4SwapMode,u4VidStd,
                       u4VidSys,u4FieldPic,u4Alpha);
}

//---------------------------------------------------------------------------
//  GFX_YCbCr420ToARGB32
//
/** Convert YCbCr to ARGB8888
 *
 *  @param  u4AlphaValue     The value of Alpha  
 *  @param  u4FieldPic     The choose of field  picture  ( 0 frame pic / 1 field pic )
 */
//---------------------------------------------------------------------------

void GFX_YCbCr420ToARGB32(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4AlphaValue,
        UINT32 u4FieldPic)
{
    GFX_Set_FlashColCnvInfo(pu1SrcY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            u4DstAdress, u4SrcWidth, u4SrcHeight);
    _GFX_Set_YCbCr420ToARGB32(u4AlphaValue, u4FieldPic);
    _GFX_Flush();
    GFX_Task_Wait_Sync();
    _GFX_Set_WriteNoPitch(FALSE);
}

//---------------------------------------------------------------------------
//  GFX_YCbCrBlockToRaster_2Buf
//
/** make YCbCr to Y / CbCr
 *
 *  @param  pu1SrcY     The value of src Y Adress  
 *  @param  u4SrcYPitch     The number of src Y bytes  
 *  @param  pu1SrcCbCr     The value of src CbCr Adress  
 *  @param  u4SrcCbCrPitch     The number of src CbCr bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  pu1SrcY     The number of src Y bytes  
 *  @param  u4SrcWidth     The value of src Y Width   
 *  @param  u4SrcHeight     The value of src Y Width   
 *  @retval void
 */
//---------------------------------------------------------------------------

void GFX_YCbCrBlockToRaster_2Buf(UINT8 *pu1SrcY, UINT32 u4SrcYPitch, UINT8 *pu1SrcCbCr,
       UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress, UINT32 u4SrcWidth, UINT32 u4SrcHeight)      
{
    UINT8 *puTempY;
    UINT8 *puTempWrite;
    //pass1
    GFX_Set_FlashColCnvInfo(pu1SrcY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            u4DstAdress, u4SrcWidth, u4SrcHeight);
    _GFX_Set_YCbCrBlockToRaster(OP_YBLOCK_TO_YRASTER);
    _GFX_Flush();
    GFX_Task_Wait_Sync();
    //pass2
    puTempY = u4DstAdress + u4SrcWidth * u4SrcHeight * 5 / 4;
    puTempWrite = u4DstAdress + u4SrcWidth * u4SrcHeight;
    GFX_Set_FlashColCnvInfo(puTempY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            puTempWrite, u4SrcWidth, u4SrcHeight);
    _GFX_Set_YCbCrBlockToRaster(OP_CBCRBLOCK_TO_1CBCRRASTER);
    _GFX_Flush();

    _GFX_Set_WriteNoPitch(FALSE);
}


//---------------------------------------------------------------------------
//  GFX_YCbCrBlockToRaster_2Buf_WithPitch
//
/** make YCbCr to Y / CbCr
 *
 *  @param  pu1SrcY     The value of src Y Adress  
 *  @param  u4SrcYPitch     The number of src Y bytes  
 *  @param  pu1SrcCbCr     The value of src CbCr Adress  
 *  @param  u4SrcCbCrPitch     The number of src CbCr bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  pu1SrcY     The number of src Y bytes  
 *  @param  u4SrcWidth     The value of src Y Width   
 *  @param  u4SrcHeight     The value of src Y Width   
 *  @retval void
 */
//---------------------------------------------------------------------------

void GFX_YCbCrBlockToRaster_2Buf_WithPitch(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4Dst_Pitch)
{
    UINT8 *puTempY;
    UINT8 *puTempWrite;
    //pass1
    GFX_Set_FlashColCnvInfo_WithPitch(pu1SrcY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            u4DstAdress, u4SrcWidth, u4SrcHeight, u4Dst_Pitch);
    _GFX_Set_YCbCrBlockToRaster_WithPitch(OP_YBLOCK_TO_YRASTER , 1);
    _GFX_Flush();

    //pass2
    puTempY = u4DstAdress + u4SrcWidth * u4SrcHeight * 5 / 4;
    puTempWrite = u4DstAdress + u4SrcWidth * u4SrcHeight;
    GFX_Set_FlashColCnvInfo_WithPitch(puTempY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            puTempWrite, u4SrcWidth, u4SrcHeight, u4Dst_Pitch);
    _GFX_Set_YCbCrBlockToRaster_WithPitch(OP_CBCRBLOCK_TO_1CBCRRASTER , 1);
    _GFX_Flush();

    _GFX_Set_WriteNoPitch(FALSE);
}

//---------------------------------------------------------------------------
//  GFX_YCbCrBlockToRaster_2Buf_CbcrOrder
//
/** make YCbCr to Y / CbCr
 *
 *  @param  pu1SrcY     The value of src Y Adress  
 *  @param  u4SrcYPitch     The number of src Y bytes  
 *  @param  pu1SrcCbCr     The value of src CbCr Adress  
 *  @param  u4SrcCbCrPitch     The number of src CbCr bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  pu1SrcY     The number of src Y bytes  
 *  @param  u4SrcWidth     The value of src Y Width   
 *  @param  u4SrcHeight     The value of src Y Width   
 *  @retval void
 */
//---------------------------------------------------------------------------

void GFX_YCbCrBlockToRaster_2Buf_CbcrOrder(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight, BOOL b_Cbcr, UINT32 u4WriteNoPitch, UINT32 u4Dst_Pitch)
 {
    UINT8 *puTempY;
    UINT8 *puTempWrite;
    //pass1
    GFX_Set_FlashColCnvInfo_WithPitch(pu1SrcY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            u4DstAdress, u4SrcWidth, u4SrcHeight, u4Dst_Pitch);
    _GFX_Set_YCbCrBlockToRaster_WithPitch(OP_YBLOCK_TO_YRASTER, u4WriteNoPitch);
    _GFX_Flush();

    //pass2
    if(u4WriteNoPitch)
    {
        puTempY = u4DstAdress + u4SrcWidth * u4SrcHeight * 5 / 4;
        puTempWrite = u4DstAdress + u4SrcWidth * u4SrcHeight;
    }
    else
    {
        puTempY = u4DstAdress + u4Dst_Pitch * u4SrcHeight + u4Dst_Pitch * u4SrcHeight/2;
        puTempWrite = u4DstAdress + u4Dst_Pitch * u4SrcHeight;
    }
    GFX_Set_FlashColCnvInfo_WithPitch(puTempY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            puTempWrite, u4SrcWidth, u4SrcHeight, u4Dst_Pitch );
    _GFX_Set_WriteCbCrOder(b_Cbcr);
    _GFX_Set_YCbCrBlockToRaster_WithPitch(OP_CBCRBLOCK_TO_1CBCRRASTER, u4WriteNoPitch );
    _GFX_Flush();

    _GFX_Set_WriteNoPitch(FALSE);
    _GFX_Set_WriteCbCrOder(0);
    
}

//---------------------------------------------------------------------------
//  GFX_YCbCrBlockToRaster_3Buf
//
/** make YCbCr to Y / Cb / Cr
 *
 *  @param  pu1SrcY     The value of src Y Adress  
 *  @param  u4SrcYPitch     The number of src Y bytes  
 *  @param  pu1SrcCbCr     The value of src CbCr Adress  
 *  @param  u4SrcCbCrPitch     The number of src CbCr bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  pu1SrcY     The number of src Y bytes  
 *  @param  u4SrcWidth     The value of src Y Width   
 *  @param  u4SrcHeight     The value of src Y Width   
 *  @retval void
 */
//---------------------------------------------------------------------------

void GFX_YCbCrBlockToRaster_3Buf(UINT8 *pu1SrcY, UINT32 u4SrcYPitch, UINT8 *pu1SrcCbCr,
                         UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress, UINT32 u4SrcWidth,
                         UINT32 u4SrcHeight,void *pt_gfx_mmu)
{
    UINT8 *puTempY;
    UINT8 *puTempWrite;
    GFX_MMU_T* pGfxMMU;
    pGfxMMU=(GFX_MMU_T*)pt_gfx_mmu;
    if(pGfxMMU->u4_init&&pGfxMMU->u4_enable)
    {
        pGfxMMU->u4_op_md= 0;
        _GFX_MMU_Set_Cfg(pGfxMMU);
    }
    //pass1
    GFX_Set_FlashColCnvInfo(pu1SrcY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            u4DstAdress, u4SrcWidth, u4SrcHeight);
    _GFX_Set_YCbCrBlockToRaster(OP_YBLOCK_TO_YRASTER);
    _GFX_Flush();
    GFX_Task_Wait_Sync();
    
    if(pGfxMMU->u4_init&&pGfxMMU->u4_enable)
    {
        pGfxMMU->u4_op_md= OP_CBCRBLOCK_TO_2CBCRRASTER;
        _GFX_MMU_Set_Cfg(pGfxMMU);
    }
    //pass2
    puTempY = u4DstAdress + u4SrcWidth * u4SrcHeight * 5 / 4;
    puTempWrite = u4DstAdress + u4SrcWidth * u4SrcHeight;
    GFX_Set_FlashColCnvInfo(puTempY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            puTempWrite, u4SrcWidth, u4SrcHeight);
    _GFX_Set_YCbCrBlockToRaster(OP_CBCRBLOCK_TO_2CBCRRASTER);
    _GFX_Flush();
    
    _GFX_Set_WriteNoPitch(FALSE);
}


//---------------------------------------------------------------------------
//  GFX_YCbCrBlockToRaster_3Buf_WidthPitch
//
/** make YCbCr to Y / Cb / Cr
 *
 *  @param  pu1SrcY     The value of src Y Adress  
 *  @param  u4SrcYPitch     The number of src Y bytes  
 *  @param  pu1SrcCbCr     The value of src CbCr Adress  
 *  @param  u4SrcCbCrPitch     The number of src CbCr bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  pu1SrcY     The number of src Y bytes  
 *  @param  u4SrcWidth     The value of src Y Width   
 *  @param  u4SrcHeight     The value of src Y Width   
 *  @retval void
 */
//---------------------------------------------------------------------------

void GFX_YCbCrBlockToRaster_3Buf_WidthPitch(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4WriteNoPitch , UINT32 u4Dst_Pitch)
{
    UINT8 *puTempY;
    UINT8 *puTempWrite;
    //pass1
    GFX_Set_FlashColCnvInfo_WithPitch(pu1SrcY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            u4DstAdress, u4SrcWidth, u4SrcHeight, u4Dst_Pitch);
    _GFX_Set_YCbCrBlockToRaster_WithPitch(OP_YBLOCK_TO_YRASTER , u4WriteNoPitch);
    _GFX_Flush();
    
    _GFX_Wait();

    //pass2
    if(u4WriteNoPitch)
    {
        puTempY = u4DstAdress + u4SrcWidth * u4SrcHeight * 5 / 4;
        puTempWrite = u4DstAdress + u4SrcWidth * u4SrcHeight;
    }
    else
    {
        puTempY = u4DstAdress + u4Dst_Pitch * u4SrcHeight + u4Dst_Pitch * u4SrcHeight/2;
        puTempWrite = u4DstAdress + u4Dst_Pitch * u4SrcHeight;
    }
    GFX_Set_FlashColCnvInfo_WithPitch(puTempY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            puTempWrite, u4SrcWidth, u4SrcHeight, u4Dst_Pitch);
    _GFX_Set_YCbCrBlockToRaster_WithPitch(OP_CBCRBLOCK_TO_2CBCRRASTER , u4WriteNoPitch);
    _GFX_Flush();

    _GFX_Set_WriteNoPitch(FALSE);
}


//---------------------------------------------------------------------------
//  GFX_YUYVToRaster_2Buf
//
/** makeYUYV TO NV12/NV21
 *
 *  @param  pu1SrcY     The value of src YUYV Adress  
 *  @param  u4SrcYPitch     The number of src YUYV bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  u4SrcWidth     The value of src  Width   
 *  @param  u4SrcHeight     The value of src  Height   
 *  @param  u4DstPitch     The value of dst Pitch   
 *  @param  b_dstPitch_en     enable dst pitch   
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_YuyvToRaster_2Buf(UINT8 *pu1Src, UINT32 u4SrcPitch,
        UINT8 * u4DstAdress, UINT32 u4DstPitch, BOOL b_dstPitch_en,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight)
{
   UINT8 * u4CbcrAddr = NULL;
   //pass1
   GFX_SetSrc( pu1Src, CM_ARGB8888_DIRECT32, u4SrcPitch);
   if( b_dstPitch_en )
   {
     GFX_SetDst( u4DstAdress, CM_ARGB4444_DIRECT16, u4DstPitch);
   }
   else
   {
     GFX_SetDst(u4DstAdress, CM_ARGB4444_DIRECT16, u4SrcWidth);
   }
   //cm fmt,0x4284
   _GFX_Set_YuyvToRaster_4284( TRUE );
   _GFX_Set_WriteNoPitch( TRUE );
   _GFX_SetReadArgbOrder( TRUE, A_ORDER, R_ORDER, G_ORDER, B_ORDER);
   _GFX_SetWriteArgbOrder(  R_ORDER, A_ORDER, B_ORDER, G_ORDER);
    GFX_BitBlt(0, 0, 0, 0, u4SrcWidth, u4SrcHeight);
   
 //  _GFX_Set_YuyvToRaster_4284(FALSE);
 //  _GFX_Set_WriteNoPitch(FALSE);
 //  _GFX_SetWriteArgbOrder( A_ORDER, R_ORDER, G_ORDER, B_ORDER);
 //  _GFX_SetReadArgbOrder(TRUE, A_ORDER, R_ORDER, G_ORDER, B_ORDER);
   
   GFX_Flush();
   
   //pass2
   u4CbcrAddr = u4DstAdress + ( u4SrcWidth * u4SrcHeight )*2;
   GFX_SetSrc(pu1Src, CM_ARGB8888_DIRECT32, u4SrcPitch );
   if( b_dstPitch_en )
   {
     GFX_SetDst(u4CbcrAddr, CM_ARGB4444_DIRECT16, u4DstPitch);
   }
   else
   {
     GFX_SetDst(u4CbcrAddr, CM_ARGB4444_DIRECT16, u4SrcWidth);
   }
   //cm fmt,0x4284
   _GFX_Set_YuyvToRaster_4284( TRUE );
   _GFX_Set_WriteNoPitch( TRUE );
   _GFX_SetReadArgbOrder( TRUE,  A_ORDER, R_ORDER, G_ORDER, B_ORDER );
   _GFX_SetWriteArgbOrder( G_ORDER, R_ORDER, A_ORDER, B_ORDER );
    GFX_BitBlt(0, 0, 0, 0, u4SrcWidth, u4SrcHeight);
   
   GFX_Flush();
   
   _GFX_Set_YuyvToRaster_4284( FALSE ); //CLEAN register
   _GFX_Set_WriteNoPitch( FALSE );
   _GFX_SetWriteArgbOrder( A_ORDER, R_ORDER, G_ORDER, B_ORDER);
   GFX_CmdQueAction();

    
}

void GFX_YuyvToRaster_2Buf_2(UINT8 *pu1Src, UINT32 u4SrcPitch,
        UINT8 * u4DstAdress, UINT32 u4DstPitch, BOOL b_dstPitch_en,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight)
{
   UINT8 * u4CbcrAddr = NULL;
   //pass1
   GFX_SetSrc(pu1Src,CM_ARGB8888_DIRECT32,u4SrcPitch);
   if(b_dstPitch_en)
   {
     GFX_SetDst(u4DstAdress,CM_ARGB4444_DIRECT16,u4DstPitch);
   }
   else
   {
     GFX_SetDst(u4DstAdress,CM_ARGB4444_DIRECT16,u4SrcWidth);
   }
   //cm fmt,0x4284
   _GFX_Set_YuyvToRaster_4284(TRUE);
   _GFX_Set_WriteNoPitch(TRUE);
   _GFX_SetReadArgbOrder(TRUE, A_ORDER, R_ORDER, G_ORDER, B_ORDER);
   _GFX_SetWriteArgbOrder( R_ORDER, A_ORDER, B_ORDER, G_ORDER);
    GFX_BitBlt(0, 0, 0, 0, u4SrcWidth, u4SrcHeight);
   
 //  _GFX_Set_YuyvToRaster_4284(FALSE);
 //  _GFX_Set_WriteNoPitch(FALSE);
 //  _GFX_SetWriteArgbOrder( A_ORDER, R_ORDER, G_ORDER, B_ORDER);
 //  _GFX_SetReadArgbOrder(TRUE, A_ORDER, R_ORDER, G_ORDER, B_ORDER);
   
   GFX_Flush();
   
   //pass2
   u4CbcrAddr = u4DstAdress + (u4SrcWidth * u4SrcHeight)*2;
   GFX_SetSrc(pu1Src,CM_ARGB8888_DIRECT32,u4SrcPitch);
   if(b_dstPitch_en)
   {
     GFX_SetDst(u4CbcrAddr,CM_ARGB4444_DIRECT16,u4DstPitch);
   }
   else
   {
     GFX_SetDst(u4CbcrAddr,CM_ARGB4444_DIRECT16,u4SrcWidth);
   }
   //cm fmt,0x4284
   _GFX_Set_YuyvToRaster_4284(TRUE);
   _GFX_Set_WriteNoPitch(TRUE);
   _GFX_SetReadArgbOrder(TRUE,  A_ORDER, R_ORDER, G_ORDER, B_ORDER);
   _GFX_SetWriteArgbOrder(  A_ORDER, R_ORDER, G_ORDER, B_ORDER);
    GFX_BitBlt(0, 0, 0, 0, u4SrcWidth, u4SrcHeight);
   
    GFX_Flush();
   
   _GFX_Set_YuyvToRaster_4284(FALSE); //CLEAN register
   _GFX_Set_WriteNoPitch(FALSE);
   _GFX_SetWriteArgbOrder( A_ORDER, R_ORDER, G_ORDER, B_ORDER);
   GFX_CmdQueAction();

    
}



//---------------------------------------------------------------------------
//  GFX_YuyvBlockToRaster_3Buf
//
/** makeYUYV TO I420
 *
 *  @param  pu1SrcY     The value of src YUYV Adress  
 *  @param  u4SrcYPitch     The number of src YUYV bytes  
 *  @param  u4DstAdress     The value of u4DstAdress  
 *  @param  u4SrcWidth     The value of src  Width   
 *  @param  u4SrcHeight     The value of src  Height   
 *  @param  u4DstPitch     The value of dst Pitch   
 *  @param  b_dstPitch_en     enable dst pitch   
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_YuyvBlockToRaster_3Buf(UINT8 *pu1Src, UINT32 u4SrcPitch,
        UINT8 * u4DstAdress, UINT32 u4DstPitch, BOOL b_dstPitch_en,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight)
{
    UINT8 * u4CbAddr = NULL;
    UINT8 * u4CrAddr = NULL;
    UINT32 u4SrcWidth_c =  u4SrcWidth*4;//yuyv treat as 8 bpp.
    UINT32 u4SrcPitch_c =  u4SrcWidth_c;
    
    // stretch out y
    GFX_SetSrc(pu1Src, CM_RGB_CLUT8, u4SrcPitch_c*2);
    if(b_dstPitch_en)
    {
      GFX_SetDst(u4DstAdress, CM_RGB_CLUT8, u4DstPitch);
    }
    else
    {
      GFX_SetDst(u4DstAdress, CM_RGB_CLUT8, u4SrcWidth_c);
    }
    _GFX_Set_WriteNoPitch( TRUE );
     GFX_StretchBlt_forYBlock(0, 0, u4SrcWidth_c<<1, u4SrcHeight>>1,
            0, 0, u4SrcWidth_c, u4SrcHeight>>1); //1/2 stretch
    GFX_Flush();

    //stretch out u
    u4CbAddr = u4DstAdress + (u4SrcWidth * u4SrcHeight)*2;
    GFX_SetSrc(pu1Src+1, CM_RGB_CLUT8, u4SrcPitch_c*4);
    if(b_dstPitch_en)
    {
      GFX_SetDst(u4CbAddr, CM_RGB_CLUT8, u4DstPitch);
    }
    else
    {
      GFX_SetDst(u4CbAddr, CM_RGB_CLUT8, u4SrcWidth_c);
    }
    _GFX_Set_WriteNoPitch( TRUE );
    GFX_StretchBlt_forUVBlock(0, 0, u4SrcWidth_c<<2, u4SrcHeight>>2,
           0, 0, u4SrcWidth_c, u4SrcHeight>>2); //1/4 stretch
    GFX_Flush();
    
    //stretch out v
    u4CrAddr = u4DstAdress + (u4SrcWidth * u4SrcHeight)*3;
    GFX_SetSrc(pu1Src+3, CM_RGB_CLUT8, u4SrcPitch_c*4);
    if(b_dstPitch_en)
    {
      GFX_SetDst(u4CrAddr, CM_RGB_CLUT8, u4DstPitch);
    }
    else
    {
      GFX_SetDst(u4CrAddr, CM_RGB_CLUT8, u4SrcWidth_c);
    }
    _GFX_Set_WriteNoPitch( TRUE );
    GFX_StretchBlt_forUVBlock(0, 0, u4SrcWidth_c<<2, u4SrcHeight>>2,
           0, 0, u4SrcWidth_c, u4SrcHeight>>2); //1/4 stretch
    GFX_Flush();
    //recover register

}

//---------------------------------------------------------------------------
//  GFX_SetColor
//
/** Set rectangular color.
 *
 *  @param  u4Color     The rectangular color
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetColor(UINT32 u4Color)
{
    _GFX_SetColor(u4Color);
}

//---------------------------------------------------------------------------
//  GFX_Fill
//
/** Fill rectangle with rectangular color.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the rectangle
 *  @param  u4Height    The height of the rectangle
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Fill(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    _GFX_Fill(u4DstX, u4DstY, u4Width, u4Height);
}

//---------------------------------------------------------------------------
//  GFX_HLine
//
/** Draw horizontal line.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the line
 * 
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_HLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width)
{
    _GFX_HLine(u4DstX, u4DstY, u4Width);
}

//---------------------------------------------------------------------------
//  GFX_VLine
//
/** Draw vertical line.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Height    The height of the line
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_VLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Height)
{
    _GFX_VLine(u4DstX, u4DstY, u4Height);
}

//---------------------------------------------------------------------------
//  GFX_SetBltOpt
//
/** Set bitblt option.
 *
 *  @param  u4Switch    The option flag of BitBlt
 *  @param  u4ColorMin  The dst color key min
 *  @param  u4ColorMax  The dst color key max
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetBltOpt(UINT32 u4Switch, UINT32 u4ColorMin, UINT32 u4ColorMax)
{
    _GFX_SetBltOpt(u4Switch, u4ColorMin, u4ColorMax);
}

//---------------------------------------------------------------------------
//  GFX_BitBlt
//
/** Bit blit operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_BitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY,
        UINT32 u4Width, UINT32 u4Height)
{
    _GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
}

//---------------------------------------------------------------------------
//  GFX_Blend
//
/** Alpha blending operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Blend(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY,
        UINT32 u4Width, UINT32 u4Height)
{
    _GFX_Blend(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
}

//---------------------------------------------------------------------------
//  GFX_AlphaComposePass
//
/** Alpha compose pass operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *  @param  u4Pass      The pass method
 *  @param  u4Param     The global alpha
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_AlphaComposePass(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Pass,
        UINT32 u4Param)
{
    _GFX_AlphaComposePass(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height,
            u4Pass, u4Param);
}

//---------------------------------------------------------------------------
//  GFX_SetColCnvFmt
//
/** Set ycbcr to rgb color conversion format.
 *
 *  @param  u4YCFmt     The yc format
 *  @param  u4SwapMode  The swap mode
 *  @param  u4VidStd    The video standard
 *  @param  u4VidSys    The video system
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetColCnvFmt(UINT32 u4YCFmt, UINT32 u4SwapMode, UINT32 u4VidStd,
        UINT32 u4VidSys)
{
    _GFX_SetColCnvFmt(u4YCFmt, u4SwapMode, u4VidStd, u4VidSys);
}

#ifdef GFX_CC_YC
// 64*32 --> ARGB8888
BOOL GFX_ColorConvert_YC(void* pvDstInfo, UINT32 u4SrcAddrY, UINT32 u4SrcAddrC,
        UINT32 u4DstCM, UINT8 u1Alpha, UINT32 u4SrcPitch)
{
    VDP_THUMBNAIL_INFO_T *pDst = (VDP_THUMBNAIL_INFO_T*) pvDstInfo;

    GFX_COLOR_CONVERT_T rGfxColorConvert;
    UINT32 u4CbCrAddrAlign = (((pDst->u4ThumbnailWidth) + 15) & (~15));
    UINT32 u4CbCrAddr = u4SrcAddrC;

    UINT32 u4DstPitch = 0;
    //UINT32 u4DstCM = (UINT32)((UINT32)(pDst->eCanvasColormode) +2);

    x_memset(&rGfxColorConvert, 0, sizeof(GFX_COLOR_CONVERT_T));

    u4DstPitch = pDst->u4CanvasPitch;

    rGfxColorConvert.t_mmu.u4_enable = 0x0;

    rGfxColorConvert.pu1CbCr = (UINT8*) u4CbCrAddr;
    rGfxColorConvert.pu1DstAddr = pDst->pucCanvasBuffer;
    rGfxColorConvert.pu1SrcAddr = (UINT8*) u4SrcAddrY;
    rGfxColorConvert.u1Alpha = u1Alpha;
    rGfxColorConvert.u4DstCM = u4DstCM;
    rGfxColorConvert.u4DstPitch = u4DstPitch;
	rGfxColorConvert.u4SrcPitch = u4SrcPitch;	
    rGfxColorConvert.u4DstX = pDst->u4ThumbnailX;
    rGfxColorConvert.u4DstY = pDst->u4ThumbnailY;
    rGfxColorConvert.u4Height = pDst->u4ThumbnailHeight;
    rGfxColorConvert.u4Width = u4CbCrAddrAlign;

    return _GFX_ColorConvert_YC(&rGfxColorConvert);
}
#else
BOOL GFX_ColorConvert_YC(void* pvDstInfo,UINT32 u4SrcAddrY,UINT32 u4SrcAddrC, UINT32 u4DstCM, UINT8 u1Alpha, UINT32 u4SrcPitch)
{
    return FALSE;
}
#endif

//---------------------------------------------------------------------------
//  GFX_SetColCnvSrc
//
/** Set color conversion source region.
 *
 *  @param  pu1SrcLuma          The base address of the luma region
 *  @param  u4SrcLumaPitch      The pitch of the luma region
 *  @param  pu1SrcChroma        The base address of the chroma region
 *  @param  u4SrcChromaPitch    The pitch of the chroma region
 *  @param  u4FieldPic          The field mode
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetColCnvSrc(UINT8 *pu1SrcLuma, UINT32 u4SrcLumaPitch,
        UINT8 *pu1SrcChroma, UINT32 u4SrcChromaPitch, UINT32 u4FieldPic)
{
    _GFX_SetColCnvSrc(pu1SrcLuma, u4SrcLumaPitch, pu1SrcChroma,
            u4SrcChromaPitch, u4FieldPic);
}

//---------------------------------------------------------------------------
//  GFX_ColConv
//
/** Convert ycbcr to rgb.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_ColConv(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    _GFX_ColConv(u4DstX, u4DstY, u4Width, u4Height);
}

void GFX_SetDstWrRgb24Bit(UINT32 u4Enable)
{
    _GFX_SetDstWrRgb24Bit(u4Enable);
}

//---------------------------------------------------------------------------
//  GFX_StretchBlt
//
/** Stretch bitblt (repeat or drop).
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4SrcW      The width of the src extent
 *  @param  u4SrcH      The height of the src extent
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4DstW      The width of the dst extent
 *  @param  u4DstH      The height of the dst extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_StretchBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW, UINT32 u4SrcH,
        UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW, UINT32 u4DstH)
{
    _GFX_StretchBlt(u4SrcX, u4SrcY, u4SrcW, u4SrcH, u4DstX, u4DstY, u4DstW,
            u4DstH);
}


//---------------------------------------------------------------------------
//  GFX_StretchBlt_forYBlock
//
/** Stretch bitblt (repeat or drop).
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4SrcW      The width of the src extent
 *  @param  u4SrcH      The height of the src extent
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4DstW      The width of the dst extent
 *  @param  u4DstH      The height of the dst extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_StretchBlt_forYBlock(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW, UINT32 u4SrcH,
        UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW, UINT32 u4DstH)
{
    _GFX_StretchBlt_forYBlock(u4SrcX, u4SrcY, u4SrcW, u4SrcH, u4DstX, u4DstY, u4DstW,
            u4DstH);
}
    
//---------------------------------------------------------------------------
//  GFX_StretchBlt_forUVBlock
//
/** Stretch bitblt (repeat or drop).
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4SrcW      The width of the src extent
 *  @param  u4SrcH      The height of the src extent
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4DstW      The width of the dst extent
 *  @param  u4DstH      The height of the dst extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_StretchBlt_forUVBlock(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW, UINT32 u4SrcH,
        UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW, UINT32 u4DstH)
{
    _GFX_StretchBlt_forUVBlock(u4SrcX, u4SrcY, u4SrcW, u4SrcH, u4DstX, u4DstY, u4DstW,
            u4DstH);
}


//---------------------------------------------------------------------------
//  GFX_AlphaMapBitBlt
//
/** Alpha-map bitblt operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_AlphaMapBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    _GFX_AlphaMapBitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
}

//---------------------------------------------------------------------------
//  GFX_SetRopOpt
//
/** Set rop bitblt option.
 *
 *  @param  u4RopCode       The rop code
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetRopOpt(UINT32 u4RopCode)
{
    _GFX_SetRopOpt(u4RopCode);
}

//---------------------------------------------------------------------------
//  GFX_RopBitBlt
//
/** Rop bitblt operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_RopBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY,
        UINT32 u4Width, UINT32 u4Height)
{
    _GFX_RopBitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
}

//---------------------------------------------------------------------------
//  GFX_ComposeLoop
//
/** Loop mode alpha composition.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *  @param  u4Ar        The global alpha
 *  @param  u4OpCode    The PD rules
 *  @param  u4RectSrc   The flag of src extent is filled rectangular color
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_ComposeLoop(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar,
        UINT32 u4OpCode, UINT32 u4RectSrc)
{
#if 0
        _GFX_ComposeLoop(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height, u4Ar,
                u4OpCode, u4RectSrc);
#else
        _GFX_ComposeLoopImprove(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height, u4Ar,
                u4OpCode, u4RectSrc);
    
#endif

}

//#ifdef CC_GFX_COMPOSITION_IMPROVE
void GFX_ComposeLoopImprove(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar,
        UINT32 u4OpCode, UINT32 u4RectSrc)
{
    _GFX_ComposeLoopImprove(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height,
            u4Ar, u4OpCode, u4RectSrc);
}
//#endif

//---------------------------------------------------------------------------
//  GFX_SetIdx2DirSrc
//
/** Set index to direct color bitblt source region.
 *
 *  @param  pu1Base     The base address of the src region
 *  @param  u4SrcCM     The color mode of the src region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetIdx2DirSrc(UINT8 *pu1Base, UINT32 u4SrcCM)
{
    _GFX_SetIdx2DirSrc(pu1Base, u4SrcCM);
}

//---------------------------------------------------------------------------
//  GFX_SetIdx2DirOpt
//
/** Set index to direct color bitblt option.
 *
 *  @param  pu1PaletteBase      The base address of the palette
 *  @param  u4MsbLeft           The flag of left pixel location in one byte
 *  @param  u4StartByteAligned  The flag of src line start byte aligned
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetIdx2DirOpt(UINT8 *pu1PaletteBase, UINT32 u4MsbLeft,
        UINT32 u4StartByteAligned)
{
    _GFX_SetIdx2DirOpt(pu1PaletteBase, u4MsbLeft, u4StartByteAligned);
}

//---------------------------------------------------------------------------
//  GFX_Idx2DirBitBlt
//
/** index to direct color bitblt operation.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_Idx2DirBitBlt(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width,
        UINT32 u4Height)
{
    _GFX_Idx2DirBitBlt(u4DstX, u4DstY, u4Width, u4Height);
}

//---------------------------------------------------------------------------
//  GFX_2DMemCompare
//
/** 2D memory compare operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_2DMemCompare(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    _GFX_2DMemCompare(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
}

//---------------------------------------------------------------------------
//  GFX_GetMemCompareResult
//
/** Get memory compare result.
 *
 *  @retval 1   With difference
 *  @retval 0   No difference
 */
//---------------------------------------------------------------------------
INT32 GFX_GetMemCompareResult(void)
{
    return _GFX_GetMemCompareResult();
}

//---------------------------------------------------------------------------
//  GFX_SetHoriToVertLineOpt
//
/** Set horizontal line to vertical line option.
 *
 *  @param  u4Is90dCcw      The flag of 90" clockwise or counterclockwise
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetHoriToVertLineOpt(UINT32 u4Is90dCcw)
{
    _GFX_SetHoriToVertLineOpt(u4Is90dCcw);
}

//---------------------------------------------------------------------------
//  GFX_HoriToVertLine
//
/** Horizontal line to vertical line operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the horizontal line
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_HoriToVertLine(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4HoriLineWidth)
{
    _GFX_HoriToVertLine(u4SrcX, u4SrcY, u4DstX, u4DstY, u4HoriLineWidth);
}

//#ifdef CC_GFX_COMPOSITION_IMPROVE
void GFX_SetCharCM(BOOL flag, UINT32 u4SrcCM)
{
    _GFX_SetCharCM(flag, u4SrcCM);
}
//#endif

//---------------------------------------------------------------------------
//  GFX_SetExtCmdQue
//
/** Horizontal line to vertical line operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
void GFX_SetExtCmdQue(volatile GFX_CMDQUE_T *prGfxCmdQue,
        volatile UINT64 *pu8CmdQueBuf, UINT32 u4Config)
{
    GFX_CmdQueSetExtBuf(prGfxCmdQue, pu8CmdQueBuf, u4Config);
}

#if 1
BOOL GFX_ColorConvert(void* pvDstInfo, UINT32 u4SrcAddr, UINT32 u4DstCM,
        UINT8 u1Alpha)
{
    VDP_THUMBNAIL_INFO_T *pDst = (VDP_THUMBNAIL_INFO_T*) pvDstInfo;

    GFX_COLOR_CONVERT_T rGfxColorConvert;
    UINT32 u4CbCrAddrAlign = (((pDst->u4ThumbnailWidth) + 15) & (~15));
    UINT32 u4CbCrAddr = u4SrcAddr + ((pDst->u4ThumbnailHeight)
            * u4CbCrAddrAlign);

    UINT32 u4DstPitch = 0;

    x_memset(&rGfxColorConvert, 0, sizeof(GFX_COLOR_CONVERT_T));

    u4DstPitch = pDst->u4CanvasPitch;
    rGfxColorConvert.t_mmu.u4_enable = 0x0;
    rGfxColorConvert.pu1CbCr = (UINT8*) u4CbCrAddr;
    rGfxColorConvert.pu1DstAddr = pDst->pucCanvasBuffer;
    rGfxColorConvert.pu1SrcAddr = (UINT8*) u4SrcAddr;
    rGfxColorConvert.u1Alpha = u1Alpha;
    rGfxColorConvert.u4DstCM = u4DstCM;
    rGfxColorConvert.u4DstPitch = u4DstPitch;
    rGfxColorConvert.u4DstX = pDst->u4ThumbnailX;
    rGfxColorConvert.u4DstY = pDst->u4ThumbnailY;
    rGfxColorConvert.u4Height = pDst->u4ThumbnailHeight;
    rGfxColorConvert.u4Width = u4CbCrAddrAlign;
    return _GFX_ColorConvert(&rGfxColorConvert);
}
#endif
void GFX_Set3DFlag(BOOL b3DFlag)
{
    b3DShiftEnable=b3DFlag;
}
BOOL GFX_Get3DFlag(void)
{
    return b3DShiftEnable;
}

void GFX_NewCompressBlt(UINT8 *pu1SrcBase, UINT32 u4SrcX, UINT32 u4SrcY,
        UINT32 u4SrcCM, UINT32 u4SrcPitch, UINT8 *pu1DstBase, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4BufHeight, BOOL u4RollBackEn,
        UINT32 u4QualityMode, UINT32 u4BpCompAddrEnd, UINT32 u4_blt_fg,
        BOOL fgBurstRead, BOOL fgLineSeparate,void *pt_gfx_mmu)
{
#ifdef CC_MT8283_OSD_COMPRESS
    UINT32 original_line_bit = u4Width *4*8;
#else
    UINT32 original_line_bit = u4SrcPitch * 8;
#endif
    UINT32 line_bit, u4DstPitch;
    BOOL fgOSD2PrLshift_ENABLE;
    UINT8 *puTmpBase = NULL;
    GFX_MMU_T* pGfxMMU;
    pGfxMMU=(GFX_MMU_T*)pt_gfx_mmu;
    if (fgLineSeparate)
    {

        if (u4RollBackEn)
            line_bit = original_line_bit + 4;
        else
            line_bit = original_line_bit + 4 + original_line_bit / 16;

        if (fgBurstRead)
            line_bit = (line_bit % 512 == 0) ? line_bit : (line_bit / 512 + 1)
                    * 512;
        else
            line_bit = (line_bit % 128 == 0) ? line_bit : (line_bit / 128 + 1)
                    * 128;

        u4DstPitch = line_bit / 8;
    }
    else
    {
        u4DstPitch = original_line_bit / 8;
    }

#ifndef CONFIG_TV_DRV_VFY
    fgOSD2PrLshift_ENABLE = OSD_BASE_Get_3DOSD2PLsEn();
    GFX_Set3DFlag(fgOSD2PrLshift_ENABLE);
    if(fgOSD2PrLshift_ENABLE)
    {
        GFX_TmpBuf_Lock();
        puTmpBase =(UINT8 *)GFX_GetTmpBuf();
        if (!puTmpBase)
        {
            printf("[osd/gfx Error] cannot alloc memory\n");
            GFX_TmpBuf_Unock();
            return;
        }

        if(pGfxMMU&&pGfxMMU->u4_init&&pGfxMMU->u4_enable)
        {
            pGfxMMU->u4_op_md = 0;
            pGfxMMU->u4_dst_rw_mmu = 0;
            _GFX_MMU_Set_Cfg(pGfxMMU);
            GFX_SetSrc((UINT8 *) pu1SrcBase, u4SrcCM, u4SrcPitch);
            GFX_SetDst((UINT8 *) puTmpBase, u4SrcCM, u4SrcPitch);
            GFX_SetBltOpt(0, 0, 0);
            GFX_StretchBlt(0, 0, u4Width, u4Height, 0, 0, u4Width, u4Height >> 1);
            GFX_Flush();
            GFX_Wait();
            pu1SrcBase = puTmpBase;
            GFX_MMU_Set_Enable(FALSE);
            
        }
        else
        {
            GFX_MMU_Set_Enable(FALSE);
            GFX_SetSrc((UINT8 *) pu1SrcBase, u4SrcCM, u4SrcPitch);
            GFX_SetDst((UINT8 *) puTmpBase, u4SrcCM, u4SrcPitch);
            GFX_SetBltOpt(0, 0, 0);
            GFX_StretchBlt(0, 0, u4Width, u4Height, 0, 0, u4Width, u4Height >> 1);
            GFX_Flush();
            GFX_Wait();
            pu1SrcBase = (UINT8 *)puTmpBase;
        }
        if( u4_blt_fg &(D_GFXFLAG_FLIP))
        {
            _GFX_NewCompressBlt(pu1SrcBase, u4SrcX, u4SrcY, u4SrcCM, u4SrcPitch,
                pu1DstBase, u4Width, u4Height/2, u4BufHeight/2, u4RollBackEn,
                u4QualityMode, u4BpCompAddrEnd, u4_blt_fg, fgBurstRead,
                fgLineSeparate, u4DstPitch);
            GFX_TmpBuf_Unock();
            return ;
        }    
        GFX_TmpBuf_Unock();
    }
    else
    {
        _GFX_MMU_Set_Cfg(pGfxMMU);
    }
#else
        _GFX_MMU_Set_Cfg(pGfxMMU);
#endif  
    _GFX_NewCompressBlt(pu1SrcBase, u4SrcX, u4SrcY, u4SrcCM, u4SrcPitch,
                    pu1DstBase, u4Width, u4Height, u4BufHeight, u4RollBackEn,
                    u4QualityMode, u4BpCompAddrEnd, u4_blt_fg, fgBurstRead,
                    fgLineSeparate, u4DstPitch);

}

void GFX_StretchAlphaCom(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW,
        UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW,
        UINT32 u4DstH, UINT32 u4Ar, UINT32 u4OpCode)
{
    _GFX_StretchAlphaCom(u4SrcX, u4SrcY, u4SrcW, u4SrcH, u4DstX, u4DstY,
            u4DstW, u4DstH, u4Ar, u4OpCode);
}

void GFX_SetPremult(UINT32 u4PremultSrcR, UINT32 u4PremultDstR,
        UINT32 u4PremultDstW, UINT32 u4PremultOvrflw)
{
    _GFX_SetPremult(u4PremultSrcR, u4PremultDstR, u4PremultDstW,
            u4PremultOvrflw);
}

void GFX_SetRotate_90(UINT32 u4Is90Ccw, UINT32 u4_dst_rd, UINT32 u4_dst_wr)
{
    _GFX_SetRotate90(u4Is90Ccw,u4_dst_rd,u4_dst_wr);
}

void GFX_Rotate_Blit(GFX_ROTATE_BLIT_T *pt_this)
{
    if(!pt_this)
    {
        return;
    }
    
    if(pt_this->u4_dcm != pt_this->u4_scm)
    {
        return;
    }
        
    GFX_Lock();
    
    GFX_MMU_Set_Cfg(&pt_this->t_mmu);

    GFX_SetSrc((UINT8 *)pt_this->u4_src,pt_this->u4_scm, pt_this->u4_spitch);
    GFX_SetDst((UINT8 *)pt_this->u4_dst,pt_this->u4_dcm, pt_this->u4_dpitch);

    switch(pt_this->u4_op)
    {
        case E_GFX_ROTATE_90:
        {
            GFX_SetRotate_90(0,0,1);
            break;
        }
        case E_GFX_ROTATE_180:
        {
            GFX_SetBltOpt((D_GFXFLAG_MIRROR|D_GFXFLAG_FLIP),0,0xffffffff);
            break;
        }
        case E_GFX_ROTATE_270:
        {
            GFX_SetRotate_90(1,0,1);
            break;
        }  
        default:
            break;
    }
    
    GFX_BitBlt( pt_this->u4_sx,pt_this->u4_sy,
                pt_this->u4_dx,pt_this->u4_dy,
                pt_this->u4_w, pt_this->u4_h);
    GFX_Flush();
    GFX_Wait();
    GFX_Unlock();

    return;
}

void GFX_GeneralAlphaCom(UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar,
        UINT32 u4OpCode, UINT32 u4RectSrc, UINT32 u4ThirdEnable,
        BOOL fgAlcomNormal, BOOL fgOnePassCompress)
{
    _GFX_GeneralAlphaCom(u4Width, u4Height, u4Ar, u4OpCode, u4RectSrc,
            u4ThirdEnable, fgAlcomNormal, fgOnePassCompress);
}

void (*pfnGFX_DMANotify)( UINT32, void*);

void GFX_SetDMAStatus(INT32 i4Status)
{
    _i4GfxInDMA = i4Status;
}

INT32 GFX_IsDMA(void)
{
    return _i4GfxInDMA;
}

void* GFX_GetDMATag(void)
{
    return _pvDMATag;
}

UINT32 GFX_Get_Cm_Size(GFX_COLOR_MODE_T e_cm)
{
    UINT32 ui4_size = 0x4;

    switch (e_cm)
    {
    case CM_YCbCr_CLUT8:
    case CM_RGB_CLUT8:
        ui4_size = 0x1;
        break;
    case CM_CbYCrY422_DIRECT16:
    case CM_YCbYCr422_DIRECT16:
    case CM_RGB565_DIRECT16:
    case CM_ARGB1555_DIRECT16:
    case CM_ARGB4444_DIRECT16:
        ui4_size = 0x2;
        break;
    case CM_AYCbCr8888_DIRECT32:
    case CM_ARGB8888_DIRECT32:
        ui4_size = 0x4;
        break;
    default:
        break;
    }

    return ui4_size;
}

void GFX_SetLegalAddr(UINT32 u4Enable,UINT32 u4Start, UINT32 u4End)
{
    _GFX_SetLegalAddr(u4Enable,u4Start,u4End);

    return;
}

//-------------------------------------------------------------------------
/** GFX_Memset
 *  use GFX engine to do memset
 *  (high-level API)
 */
//-------------------------------------------------------------------------
void GFX_MemsetAsync(UINT8 *pu1Addr, UINT32 u4_width,UINT32 u4_height, UINT32 u4_value)
{
    GFX_Lock();
    GFX_MMU_Set_Enable(FALSE);
    GFX_SetDst(pu1Addr, (UINT32) CM_ARGB8888_DIRECT32, (u4_width*4));
    GFX_SetColor(u4_value);
    GFX_Fill(0, 0, u4_width, u4_height);
    GFX_Flush();
    GFX_Unlock();
}

UINT32 GFX_Get_IMGRZ_Idx(void)
{
    #ifdef IMGRZ_NUM
    return (IMGRZ_NUM -1);
    #else
    return E_FIRST_RESIZER;
    #endif
}

UINT32 GFX_Get_IMGRZ_CM(UINT32 u4_gfx_cm)
{
    UINT32 u4_img_cm = E_RZ_OSD_DIR_CM_ARGB8888;
    switch (u4_gfx_cm)
    {
        case CM_RGB565_DIRECT16:
            u4_img_cm = E_RZ_OSD_DIR_CM_RGB565;
            break;
        case CM_ARGB4444_DIRECT16:
            u4_img_cm = E_RZ_OSD_DIR_CM_ARGB4444;
            break;
        case CM_ARGB8888_DIRECT32:
            u4_img_cm = E_RZ_OSD_DIR_CM_ARGB8888;
            break;
        default:
            printf("GFX_Get_IMGRZ_CM() error: bad eSrcCm %d!\n",u4_gfx_cm);
            break;
    }
    return u4_img_cm;
}

void GFX_Stretchblit(void* pv_this)
{
    E_RZ_INOUT_TYPE_T       eRzType;
    UINT32 u4_imgrz_idx     =0x0;

    RZ_OSD_SCL_PARAM_SET_T *pt_this = (RZ_OSD_SCL_PARAM_SET_T *)pv_this;

    eRzType         = E_RZ_INOUT_TYPE_OSDUI;
    u4_imgrz_idx    = GFX_Get_IMGRZ_Idx();

    IMGRZ_Lock_Ex(u4_imgrz_idx);
    IMGRZ_ReInit_Ex(u4_imgrz_idx);
    IMGRZ_Cfg_MMU_IO_Ex((UINT32) u4_imgrz_idx, pt_this->u4IOMMURead, pt_this->u4IOMMUWrite);
    IMGRZ_SetScaleOpt_Ex(u4_imgrz_idx, eRzType);
    IMGRZ_Scale_Ex(u4_imgrz_idx,pt_this);
    IMGRZ_Flush_Ex(u4_imgrz_idx);
    IMGRZ_Wait_Ex(u4_imgrz_idx);
    IMGRZ_Unlock_Ex(u4_imgrz_idx);

    return;
}

void GFX_Stretch_Alphacom(void* pv_this)
{
    RZ_OSD_SCL_PARAM_SET_T      rSclParam;
    UINT32 u4TmpBufAddr         = 0x0;
    GFX_STRETCH_BLIT_T  *prStretch = (GFX_STRETCH_BLIT_T  *)pv_this;

    x_memset(&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));
    rSclParam.u4SrcCM = GFX_Get_IMGRZ_CM(prStretch->eSrcCm);
    rSclParam.u4TgCM  = GFX_Get_IMGRZ_CM(prStretch->eDstCm);
    rSclParam.u4SrcBase = (UINT32) prStretch->u4Src;
    rSclParam.u4IsSrcDirCM = (UINT32) 1;
    rSclParam.u4SrcBufLen = (UINT32) prStretch->u4SrcPitch;
    rSclParam.u4SrcHOffset = (UINT32) prStretch->u4SrcX;
    rSclParam.u4SrcVOffset = (UINT32) prStretch->u4SrcY;
    rSclParam.u4SrcW = (UINT32) prStretch->u4SrcWidth;
    rSclParam.u4SrcH = (UINT32) prStretch->u4SrcHeight;
    GFX_TmpBuf_Lock();
    u4TmpBufAddr = GFX_GetTmpBuf();
    ASSERT(u4TmpBufAddr != 0);
    rSclParam.u4TgBase = u4TmpBufAddr;

    rSclParam.u4TgHOffset = (UINT32) prStretch->u4DstX;
    rSclParam.u4TgVOffset = (UINT32) prStretch->u4DstY;
    rSclParam.u4TgW = (UINT32) prStretch->u4DstWidth;
    rSclParam.u4TgH = (UINT32) prStretch->u4DstHeight;
    rSclParam.u4TgBufLen = (UINT32) prStretch->u4DstPitch;
    
    #if 0
    printf("imgrz_stretchalcom [0x%x,0x%x,0x%x,0x%x,0x%x,0x%x]\n",
        (UINT32) prStretch->u4Src,prStretch->u4SrcWidth,prStretch->u4SrcHeight,
        (UINT32) prStretch->u4Dst,prStretch->u4DstWidth,prStretch->u4DstHeight);
    #endif
    
    if ((rSclParam.u4TgW != prStretch->u4ClipWidth) || 
        (rSclParam.u4TgH!= prStretch->u4ClipHeight))
    {
        rSclParam.u4PartialUpdate = TRUE;
        rSclParam.u4PartialUpdate_ClipTgx = (UINT32) prStretch->u4ClipX;
        rSclParam.u4PartialUpdate_ClipTgy = (UINT32) prStretch->u4ClipY;
        rSclParam.u4PartialUpdate_ClipTgw = (UINT32) prStretch->u4ClipWidth;
        rSclParam.u4PartialUpdate_ClipTgh = (UINT32) prStretch->u4ClipHeight;
    }
    rSclParam.u4RzIOMMU_MMUTableAddr = prStretch->mmu.u4_pgt;
    rSclParam.u4IOMMURead = prStretch->mmu.u4_src_rw_mmu;
    rSclParam.u4IOMMUWrite= 0x0;
    GFX_Stretchblit(&rSclParam);
    prStretch->mmu.u4_src_rw_mmu = 0x0;
    GFX_MMU_Set_Cfg(&prStretch->mmu);
    GFX_SetSrc((UINT8 *) u4TmpBufAddr, (UINT32) prStretch->eDstCm,
            rSclParam.u4TgBufLen);
    GFX_SetDst((UINT8 *) prStretch->u4Dst, (UINT32) prStretch->eDstCm,
            prStretch->u4DstPitch);

    GFX_SetPremult( prStretch->u4PremultSrcR,   prStretch->u4PremultDstR, 
                    prStretch->u4PremultDstW,   prStretch->u4PremultOvrflw);

    GFX_ComposeLoop(prStretch->u4DstX, prStretch->u4DstY, prStretch->u4DstX,
            prStretch->u4DstY, prStretch->u4ClipWidth, prStretch->u4ClipHeight,
            prStretch->u4Ar, prStretch->u4Porterdu, 0);
    
    GFX_DFB_Flush();
    GFX_Wait();
    GFX_TmpBuf_Unock();     
    
    return ;
}

void GFX_YCbCr420Swap2ToARGB32(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4AlphaValue,
        UINT32 u4FieldPic,UINT32 u4Sys,UINT32 u4Std,UINT32 u4Vfull)
{
    GFX_Set_FlashColCnvInfo(pu1SrcY, u4SrcYPitch, pu1SrcCbCr, u4SrcCbCrPitch,
            u4DstAdress, u4SrcWidth, u4SrcHeight);
    _GFX_Set_YCbCr420Swap2ToARGB32(u4AlphaValue, u4FieldPic,u4Sys,u4Std,u4Vfull);
    _GFX_Flush();
    
    _GFX_Set_WriteNoPitch(FALSE);
}


VOID GFX_Transform(UINT32 u4SrcAddr, UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4SrcPitch, UINT32 u4SrcColorMode, BOOL fgSrcVirtual,
	UINT32 u4DstAddr, UINT32 u4DstWidth, UINT32 u4DstHeight, UINT32 u4DstPitch, UINT32 u4DstColorMode, BOOL fgDstVirtual)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    //Printf("%s---src:0x%x %d %d %d %d %d \n", __FUNCTION__, u4SrcAddr, u4SrcWidth, u4SrcHeight, u4SrcPitch, u4SrcColorMode, fgSrcVirtual);
	//Printf("%s---dst:0x%x %d %d %d %d %d \n", __FUNCTION__, u4DstAddr, u4DstWidth, u4DstHeight, u4DstPitch, u4DstColorMode, fgDstVirtual);
    
    x_memset(&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));
    rSclParam.u4SrcCM = GFX_Get_IMGRZ_CM(u4SrcColorMode);
    rSclParam.u4TgCM  = GFX_Get_IMGRZ_CM(u4DstColorMode);
    rSclParam.u4SrcBase = (UINT32) u4SrcAddr;
    rSclParam.u4IsSrcDirCM = (UINT32) 1;
    rSclParam.u4SrcBufLen = (UINT32) u4SrcPitch;
    rSclParam.u4SrcW = (UINT32) u4SrcWidth;
    rSclParam.u4SrcH = (UINT32) u4SrcHeight;
    rSclParam.u4TgBase = u4DstAddr;
    rSclParam.u4TgW = (UINT32) u4DstWidth;
    rSclParam.u4TgH = (UINT32) u4DstHeight;
    rSclParam.u4TgBufLen = (UINT32) u4DstPitch;
#ifdef LINUX_TURNKEY_SOLUTION	
    rSclParam.u4RzIOMMU_MMUTableAddr = HalCpuGetPhyPgd();
#else
    rSclParam.u4RzIOMMU_MMUTableAddr = HalGetMMUTableAddress();
#endif
    rSclParam.u4IOMMURead = fgSrcVirtual;
    rSclParam.u4IOMMUWrite= fgDstVirtual;
	
	GFX_Stretchblit(&rSclParam);
}

