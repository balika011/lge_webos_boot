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
 * $RCSfile: gfx_drvif.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_drvif.c
 *  internal public interfaces of gfx driver
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

#ifdef LINUX_TURNKEY_SOLUTION
#include <linux/sched.h>
#endif

#include "gfx_if.h"
#include "gfx_drvif.h"
#include "gfx_dif.h"
#include "gfx_hw.h"
#include "gfx_cmdque.h"
#include "fbm_drvif.h"
#include "vdp_if.h"
#include "sv_const.h"


//#include "drv_common.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

/** gfx default internal buffer size (used in alpha composition)
 *  8 lines, 16bpp per pixel, 1920 pixels per line
 */

#define GFX_SUB_PIC_COLORS   4              // sub-picture total colors


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


// acronym of gfx register name in register file
#define REG(XREG)       _prRegFile->rField.XREG

#define REGT(TREG)      _prRegFile->rReg.TREG


// push back relative register value in register file.
#if defined(GFX_RISC_MODE)  // risc mode
#define GFX_CMDENQ(XREG)        \
    (GFX_RiscPushBack((UINT32)(XREG), _au4RegFile[(UINT32)(XREG)]))
#else                       // cmdque mode
#define GFX_CMDENQ(XREG)        \
    (GFX_CmdQuePushBack((UINT32)(XREG), _au4RegFile[(UINT32)(XREG)]))
#endif // #if defined(GFX_RISC_MODE)


static BOOL _fgGfxEnableBurstRead = 0;
static UINT32 _fgGfxBurstReadMode = 0;



void GFX_FlushInvalidateDCacheMultipleLine(void);

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

static const UINT8 _auColorModeShift[] =
{
    2, // CM_YCbCr_CLUT2
    1, // CM_YCbCr_CLUT4
    0, // CM_YCbCr_CLUT8
    0, // CM_Reserved0
    1, // CM_CbYCrY422_DIRECT16
    1, // CM_YCbYCr422_DIRECT16
    2, // CM_AYCbCr8888_DIRECT32
    0, // CM_Reserved1
    2, // CM_RGB_CLUT2
    1, // CM_RGB_CLUT4
    0, // CM_RGB_CLUT8
    1, // CM_RGB565_DIRECT16
    1, // CM_ARGB1555_DIRECT16
    1, // CM_ARGB4444_DIRECT16
    2, // CM_ARGB8888_DIRECT32
    0 // CM_Reserved2
};

// gfx is in flush (used in middleware).
static INT32 _i4GfxInFlush = (INT32) FALSE;

// output/input address base
static UINT8 *_pu1DstBase;
static UINT8 *_pu1SrcBase;
static UINT8 *_pu1ThirdBase;

// output/input region line size
static UINT32 _u4DstPitch;
static UINT32 _u4SrcPitch;
//static UINT32 _u4ThirdPitch;

// output/input region color mode
static UINT32 _u4DstCM;
static UINT32 _u4SrcCM;
static UINT32 _u4ThirdCM;

static UINT32 _u4DstShift;
static UINT32 _u4SrcShift;
//static UINT32 _u4ThirdShift;

// for "index to direct color bitblt" use
static UINT8 *_pu1Idx2DirSrcBase;
static UINT32 _u4CharCM;
static UINT32 _u4ThirdCharCM;

static UINT32 _au4RegFile[GREG_FILE_SIZE];

static HANDLE_T _semGfxOp = NULL_HANDLE;
static HANDLE_T _semGfxCmdque = NULL_HANDLE;
static HANDLE_T _hGfxCrashDetectTimer = NULL_HANDLE;

static BOOL _fgGfxInited = FALSE;

/*lint -save -e826 */
static MI_DIF_UNION_T *_prRegFile = (MI_DIF_UNION_T *) _au4RegFile;
/*lint -restore */

// internal working buffer for alpha-composition use
static UINT8 _au1WorkingBuf[GFX_DEF_BUF_SIZE];
static UINT8 *_pu1WorkingBuf = _au1WorkingBuf;
static UINT32 _u4WorkingBufSize = GFX_DEF_BUF_SIZE;
static UINT32 _u4WorkingPitch;

// for sub-picture decoding
static UINT8* _pu1BmpBase;
static UINT32 _u4GfxBltSwitch;

#ifndef __KERNEL__
extern UINT32 Image$$RO$$Limit;
#endif

#ifdef __KERNEL__
UINT32 _u4DirectFBAddr = 0;
UINT32 _u4DirectFBSize = 0;
#endif

GFX_OBJ_T t_gfx_obj;

static  GFX_TASK_LIST_T t_gfx_task_list[GFX_TASK_LIST_MAX];
static  GFX_TASK_THREAD_DATA_T          t_task_data;
static  GFX_TASK_THREAD_DATA_T*         pt_task_data = &t_task_data;

#define GFX_Task_List_Lock(h_sema)      (x_sema_lock(h_sema, X_SEMA_OPTION_WAIT))
#define GFX_Task_List_NLock(h_sema)     (x_sema_lock(h_sema, X_SEMA_OPTION_NOWAIT))

#define GFX_Task_List_Unlock(h_sema)    (x_sema_unlock(h_sema))

#define GFX_Task_List_Addr              t_gfx_task_list

/* gfx task cmdq buffer*/
#define GFX_TASK_CMDQ_ADDR(pt_this)      (pt_this->u4_buf)
#define GFX_TASK_CMDQ_SIZE(pt_this)      (pt_this->u4_buf_size)
#define GFX_TASK_CMDQ_END(pt_this)  (GFX_TASK_CMDQ_ADDR(pt_this) + GFX_TASK_CMDQ_SIZE(pt_this))
#define GFX_TASK_CMDQ_MID(pt_this)  (GFX_TASK_CMDQ_ADDR(pt_this) + (GFX_TASK_CMDQ_SIZE(pt_this)>>1))

#define GFX_TASK_SRC_CMDQ_ADDR           Gfx_Cmd_Get_Task_SrcAddr()
#define GFX_TASK_SRC_CMDQ_END            (UINT32)(GFX_TASK_SRC_CMDQ_ADDR+GFX_CMDQ_BUF_SIZE)


#define GFX_Task_List_Cmdq_Pop(pt_gfx_task_list)                                \
{                                                                               \
    GFX_Task_List_Lock(pt_gfx_task_list->h_sema);                             \
    pt_gfx_task_list->u4_cnt--;                                               \
    if (pt_gfx_task_list->u4_cnt)                                             \
    {                                                                           \
         pt_gfx_task_list->pt_first =  pt_gfx_task_list->pt_first->pt_next; \
    }                                                                           \
    else                                                                       \
    {                                                                           \
        pt_gfx_task_list->pt_first = NULL;                                    \
        pt_gfx_task_list->pt_end   = NULL;                                    \
        GFX_Task_List_Unlock(pt_gfx_task_list->h_sync);                       \
    }                                                                           \
    GFX_Task_List_Unlock(pt_gfx_task_list->h_sema);                           \
}

#define GFX_TASK_CHECK_NULL(pv_this)                                            \
{                                                                               \
    while(!pv_this)                                                           \
    {                                                                           \
        x_thread_delay(10);                                                     \
        GFX_PRINT("GFX_TASK_CHECK_NULL \n");                                    \
    }                                                                           \
}                                                                               


#define GFX_TASK_LIST_SRC_CRC_COPY(u4_src,u4_size,pv_dst,u4_mod)                \
GFX_TASK_CHECK_NULL((UINT32)u4_src);                                           \
GFX_TASK_CHECK_NULL((UINT32)pv_dst);                                           \
if(u4_mod)                                                                     \
{                                                                               \
    u4_crc_size = (GFX_TASK_SRC_CMDQ_END - (UINT32)u4_src);                    \
    x_memcpy((void *)pv_dst,(void *)u4_src,u4_crc_size);                      \
    GFX_TASK_CHECK_NULL(GFX_TASK_SRC_CMDQ_ADDR);                                \
    x_memcpy((void *)(pv_dst+u4_crc_size),(void *)GFX_TASK_SRC_CMDQ_ADDR,(u4_size - u4_crc_size));\
}                                                                               \
else                                                                           \
{                                                                               \
    x_memcpy((void *)pv_dst,(void *)u4_src,u4_size);                         \
}


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


#define REG_MMU(XREG)       _prGfxMMURegFile->rField.XREG

#if defined(GFX_RISC_MODE)  // risc mode
#define GFX_MMU_CMDENQ(XREG)        \
    (GFX_MMU_RiscPushBack((UINT32)(XREG), _au4GfxMMURegFile[(UINT32)(XREG)]))
#else                       // cmdque mode
#define GFX_MMU_CMDENQ(XREG)        \
    (GFX_MMU_CmdQuePushBack((UINT32)(XREG), _au4GfxMMURegFile[(UINT32)(XREG)]))
#endif // #if defined(GFX_RISC_MODE)
static UINT32 _au4GfxMMURegFile[GREG_MMU_LAST];

static MI_DIF_GFX_MMU_UNION_T *_prGfxMMURegFile =
        (MI_DIF_GFX_MMU_UNION_T *) _au4GfxMMURegFile;
static GFX_MMU_T _r_gfx_mmu;

static void _GFX_MMU_Set_Enable(UINT32 u4_enable);


#define GFX_MMU_SAME_CFG(pt_this)   (!x_memcmp(&_r_gfx_mmu,pt_this,sizeof(GFX_MMU_T)))
#define GFX_MMU_SAVE_CFG(pt_this)   (x_memcpy(&_r_gfx_mmu,pt_this,sizeof(GFX_MMU_T)))
#define GFX_MMU_INIT_CFG()          (x_memset(&_r_gfx_mmu, 0, sizeof(GFX_MMU_T)))
#define GFX_MMU_ENABLE_FG           (_r_gfx_mmu.u4_enable)

static void GFX_MMU_Enable_CmdQ(UINT32 u4_enable);

//-------------------------------------------------------------------------
/** _GfxAlphaCompPass
 *  alpha compose pass.
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass(UINT32 u4Width, UINT32 u4Height)
{
    REG(fg_OP_MODE) = (INT32) OP_ALPHA_COMPOSE;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = ((u4Height-1) & 0x007FF);
    REG(fg_SRC_HEIGHT_12) = (((u4Height -1)>>11) & 0x1);
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();
    GFX_CMDENQ(GREG_0x4284);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);

    REG(fg_SRC_HEIGHT_12) = 0;
}

//-------------------------------------------------------------------------
/** _GfxAlphaCompPass0
 *  alpha composite pass 0.
 *  apply ext alpha to color.
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass0(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Ar)
{
    UINT32 ui4_src_addr = 0x0;

    UNUSED(u4DstX);
    UNUSED(u4DstY);

    // pass 0 : apply ar to source (put into working buffer)
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4DstShift));
    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = (UINT32)(_pu1WorkingBuf);
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG((UINT32)(_pu1WorkingBuf));

    REG(fg_ALPHA_VALUE) = u4Ar;
    REG(fg_ALCOM_PASS) = 0;

    REG(fg_OSD_WIDTH) = (_u4WorkingPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4SrcPitch >> 4);

    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}

//-------------------------------------------------------------------------
/** _GfxAlphaCompPass1a
 *  alpha composite pass 1a.
 *  apply pixel alpha to color.
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass1a(void)
{
    // pass 1a : apply source alpha to color components
    REG(fg_SRC_BSAD) = (UINT32)(_pu1WorkingBuf);
    REG(fg_DST_BSAD) = (UINT32)(_pu1WorkingBuf);

    REG(fg_ALPHA_VALUE) = 255;
    REG(fg_ALCOM_PASS) = 1;

    REG(fg_OSD_WIDTH) = (_u4WorkingPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4WorkingPitch >> 4);

    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}

//-------------------------------------------------------------------------
/** _GfxAlphaCompPass1b
 *  alpha composite pass 1b.
 *  apply pixel alpha to color.
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass1b(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY)
{
    UINT32 ui4_src_addr = 0x0;

    UNUSED(u4SrcX);
    UNUSED(u4SrcY);

    // pass 1b : apply destination alpha to color components
    ui4_src_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_src_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);

    REG(fg_ALPHA_VALUE) = 255;
    REG(fg_ALCOM_PASS) = 1;

    REG(fg_OSD_WIDTH) = (_u4DstPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4DstPitch >> 4);

    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}

//-------------------------------------------------------------------------
/** _GfxAlphaCompPass2
 *  alpha composite pass 2.
 *
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass2(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Mode)
{
    UINT32 ui4_dst_addr = 0x0;
    UINT32 ui4_src_addr = 0x0;

    UNUSED(u4SrcX);
    UNUSED(u4SrcY);

    ui4_src_addr = (UINT32)(_pu1WorkingBuf);
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    REG(fg_ALPHA_VALUE) = u4Mode;
    REG(fg_ALCOM_PASS) = 2;

    REG(fg_OSD_WIDTH) = (_u4DstPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4WorkingPitch >> 4);

    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}

//-------------------------------------------------------------------------
/** _GfxAlphaCompPass3
 *  alpha composite pass 3.
 *  extrace color from composite color.
 */
//-------------------------------------------------------------------------
static void _GfxAlphaCompPass3(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY)
{
    UINT32 ui4_dst_addr = 0x0;

    UNUSED(u4SrcX);
    UNUSED(u4SrcY);

    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));

    REG(fg_SRC_BSAD) = ui4_dst_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    REG(fg_ALPHA_VALUE) = 255;
    REG(fg_ALCOM_PASS) = 3;

    REG(fg_OSD_WIDTH) = (_u4DstPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4DstPitch >> 4);

    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _GFX_IsFlushing
 *  check whether engine is flushing
 *  
 */
//-------------------------------------------------------------------------
INT32 _GFX_IsFlushing(void)
{
    return _i4GfxInFlush;
}

//-------------------------------------------------------------------------
/** _GFX_SetFlushStatus
 *  set engine flush status
 *  
 */
//-------------------------------------------------------------------------
void _GFX_SetFlushStatus(INT32 i4Status)
{
    _i4GfxInFlush = i4Status;
}

//-------------------------------------------------------------------------
/** _GFX_ReInit
 *  re-init
 *
 */
//-------------------------------------------------------------------------
void _GFX_ReInit(void)
{
    _fgGfxInited = FALSE;
    _GFX_Init();
}

//-------------------------------------------------------------------------
/** _GFX_Init
 *  init.
 *  reset gfx.
 *  reset gfx timer.
 *  clear soft reg file.
 *  init dif.
 *  init command que.
 */
//-------------------------------------------------------------------------
void _GFX_Init(void)
{
    if (_fgGfxInited)
    {
        return;
    }

    if (_semGfxOp == NULL_HANDLE)
    {
        VERIFY(
                x_sema_create(&_semGfxOp, X_SEMA_TYPE_BINARY,
                        X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    if (_semGfxCmdque == NULL_HANDLE)
    {
        VERIFY(
                x_sema_create(&_semGfxCmdque, X_SEMA_TYPE_BINARY,
                        X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    if (_hGfxCrashDetectTimer == NULL_HANDLE)
    {
        VERIFY(x_timer_create(&_hGfxCrashDetectTimer) == OSR_OK);
    }
    
    GFX_OBJ_INIT();
    
    GFX_Reset();

    GFX_UNUSED_RET(x_memset(_au4RegFile, 0, sizeof(_au4RegFile)));

    GFX_UNUSED_RET(x_memset(_au4GfxMMURegFile, 0, sizeof(_au4GfxMMURegFile)))
    // init dif
    GFX_DifInit();
    // init cmdque
    GFX_CmdQueInit();
    #ifndef CONFIG_TV_DRV_VFY
    // register GFX_Memset() into FBM module
    FBM_RegCbFunc(FBM_CB_FUNC_GFX_MEMSET, (UINT32) GFX_Memset);
    FBM_RegCbFunc(FBM_CB_FUNC_GFX_TASK_MEMSET, (UINT32) GFX_Task_Memset);
    FBM_RegCbFunc(FBM_CB_FUNC_GFX_TASK_MEMSET_DONE, (UINT32) GFX_Task_Check_Done);
    #endif
    
    GFX_Task_Init();
    
    _GFX_Set_FastBitBlt(TRUE);

    _fgGfxInited = TRUE;
}

//-------------------------------------------------------------------------
/** _GFX_Reset
 *  reset all.
 *  reset dif.
 */
//-------------------------------------------------------------------------
void _GFX_Reset(void)
{
    GFX_DifReset();
    GFX_MMU_Enable_CmdQ(TRUE);
}

//-------------------------------------------------------------------------
/** _GFX_Wait
 *  wait engine work complete.
 *  wait dif.
 */
//-------------------------------------------------------------------------

void _GFX_Wait(void)
{
    GFX_LockCmdque();
    GFX_UnlockCmdque();
}

//-------------------------------------------------------------------------
/** _GFX_Wait_Vdp
 *  wait Vdp work complete.
 *  wait dif.
 */
//-------------------------------------------------------------------------

void _GFX_Wait_Vdp(void)
{
    #ifndef CONFIG_TV_DRV_VFY
    x_thread_delay(VDP_GetCmdDelay());
    #endif
}

//-------------------------------------------------------------------------
/** _GFX_Wait
 *  wait engine work complete.
 *  wait dif.
 */
//-------------------------------------------------------------------------
void _GFX_Wait_Timeout(INT32 i4Timeout)
{
    if (OSR_TIMEOUT == x_sema_lock_timeout(_semGfxCmdque, (UINT32) i4Timeout))
    {
        // LOG(0, "gfx timeout\n");
    }

    GFX_UNUSED_RET(x_sema_unlock(_semGfxCmdque));
}

//-------------------------------------------------------------------------
/** _GFX_MwFlush
 *  force engine to do operations (for middleware use).
 *  set in flush flag.
 *  call command queue action.
 */
//-------------------------------------------------------------------------
void _GFX_MwFlush(void)
{
    _GFX_SetFlushStatus(TRUE); // for middleware use

#if defined(GFX_RISC_MODE)  // risc mode
    GFX_HwISR();
#else                       // cmdque mode
    GFX_CmdQueAction();
#endif
}

void GFX_ClearRegister(void)
{
    GFX_FLUSH_CLEAR_ACTION();
    
    if(GFX_DBG_LVL(0x2))
    {
        GFX_Flush_Action();
    }
#if defined(CC_MT5890)    
    REGT(u4_G_CONFIG) = 0xAEC0031F; 	
    GFX_CMDENQ(GREG_G_CONFIG);			
    REGT(u4_G_CONFIG) = 0xAAC0031F; 	
    GFX_CMDENQ(GREG_G_CONFIG);			
#endif    	
}

UINT64 GFX_Flush_Action(void)
{
    UINT64 u8_id = 0x0;
    
    #if !defined(GFX_RISC_MODE)
    u8_id = GFX_CmdQueAction();
    #endif

    return u8_id;
}

//-------------------------------------------------------------------------
/** _GFX_Flush
 *  force engine to do operations (for driver use).
 *  set in flush flag.
 *  call command queue action.
 */
//-------------------------------------------------------------------------
void _GFX_Flush(void)
{
    //HalFlushInvalidateDCache();
    GFX_Task_List_Dcache_Add(E_GFX_TASK_DCACHE_DFLUSH);

    GFX_Flush_Action();

    return;
}



//-------------------------------------------------------------------------
/** _GFX_Lock
 *  take the semaphore
 *
 */
//-------------------------------------------------------------------------
void _GFX_Lock(void)
{
    GFX_UNUSED_RET(x_sema_lock(_semGfxOp, X_SEMA_OPTION_WAIT));
    
    if(_fgGfxInited)
    {
        GFX_ClearRegister();
    }
}


//-------------------------------------------------------------------------
/** _GFX_TryLock
 *  try to take the semaphore
 *
 */
//-------------------------------------------------------------------------
void _GFX_TryLock(void)
{
    GFX_UNUSED_RET(x_sema_lock(_semGfxOp, X_SEMA_OPTION_NOWAIT))
}

//-------------------------------------------------------------------------
/** _GFX_Unlock
 *  release the semaphore
 *
 */
//-------------------------------------------------------------------------
void _GFX_Unlock(void)
{
    GFX_UNUSED_RET(x_sema_unlock(_semGfxOp))
}

//-------------------------------------------------------------------------
/** _GFX_LockCmdque
 *  use lock_timeout to do gfx engine crash detection
 *  crash cause 1 : engine eats invalid cmd
 *
 */
//-------------------------------------------------------------------------

void _GFX_LockCmdque(void)
{
    GFX_UNUSED_RET(x_sema_lock(_semGfxCmdque, X_SEMA_OPTION_WAIT))
}

//-------------------------------------------------------------------------
/** _GFX_UnlockCmdque
 *  release the semaphore
 *
 */
//-------------------------------------------------------------------------
void _GFX_UnlockCmdque(void)
{
    GFX_UNUSED_RET(x_sema_unlock(_semGfxCmdque))
}

//-------------------------------------------------------------------------
/** _GFX_QueryHwIdle
 *  query engine status
 *
 */
//-------------------------------------------------------------------------
INT32 _GFX_QueryHwIdle(void)
{
    return GFX_HwGetIdle();
}

//-------------------------------------------------------------------------
/** _GFX_SetNotify
 *  set notify function.
 *  set dif notification.
 */
//-------------------------------------------------------------------------
void _GFX_SetNotify(void(*pfnNotifyFunc)( UINT32))
{
    GFX_DifSetNotify(pfnNotifyFunc);
}

//-------------------------------------------------------------------------
/** _GFX_SetDst
 *  set output region.
 *  set output base.
 *  set output color mode.
 *  set output line size.
 *  set output address shift.
 *  enque color mode and line size registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetDst(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch)
{
#ifndef __KERNEL__
    UINT32 u4HighAddr = (UINT32) & Image$$RO$$Limit; // add memory proction, avoid accessing invaild memory address
    UNUSED(u4HighAddr);
    GFX_ASSERT((UINT32)pu1Base > u4HighAddr);
#endif

    GFX_ASSERT(pu1Base != NULL);
    GFX_CHECK_PITCH(u4Pitch);

    _pu1DstBase = pu1Base;
    _u4DstCM = u4ColorMode;
    _u4DstPitch = u4Pitch;
    _u4DstShift = _auColorModeShift[u4ColorMode];

    REG(fg_CM) = u4ColorMode;
    REG(fg_OSD_WIDTH) = (u4Pitch >> 4);

    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}

//-------------------------------------------------------------------------
/** _GFX_SetSrc
 *  set input region.
 *  set input base.
 *  set input color mode.
 *  set input line size.
 *  set input address shift.
 *  enque color mode and line size registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetSrc(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch)
{
#ifndef __KERNEL__
    UINT32 u4HighAddr = (UINT32) & Image$$RO$$Limit; // add memory proction, avoid accessing invaild memory address
    UNUSED(u4HighAddr);
    GFX_ASSERT((UINT32)pu1Base > u4HighAddr);
#endif

    // check 128 bits (16 bytes) alignment
    GFX_CHECK_PITCH(u4Pitch);

    GFX_ASSERT(pu1Base != NULL);

    _pu1SrcBase = pu1Base;
    _u4SrcCM = u4ColorMode;
    _u4SrcPitch = u4Pitch;
    _u4SrcShift = _auColorModeShift[u4ColorMode];

    REG(fg_SRC_CM) = u4ColorMode;
    REG(fg_SRC_PITCH) = (u4Pitch >> 4);

    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}

//-------------------------------------------------------------------------
/** _GFX_SetThird
 *  set input region.
 *  set input base.
 *  set input color mode.
 *  set input line size.
 *  set input address shift.
 *  enque color mode and line size registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetThird(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch)
{
    // check 128 bits (16 bytes) alignment
    GFX_CHECK_PITCH(u4Pitch);

    GFX_ASSERT(pu1Base != NULL);

    _pu1ThirdBase = pu1Base;
    _u4ThirdCM = u4ColorMode;
    //_u4ThirdPitch = u4Pitch;
    //_u4ThirdShift = _auColorModeShift[u4ColorMode];
    REG(fg_THIRD_CM) = u4ColorMode;
    REG(fg_THIRD_PITCH) = (u4Pitch >> 4);
    REG(fg_THIRD_BUF_EN) = 0x1;
    REG(fg_THIRD_BSAD) = (UINT32)(_pu1ThirdBase);
}

//-------------------------------------------------------------------------
/** _GFX_SetAlpha
 *  set alpha value.
 *  alpha value used in.
 *  alpha blending.
 *  alpha composition.
 */
//-------------------------------------------------------------------------
void _GFX_SetAlpha(UINT32 u4Alpha)
{
    REG(fg_ALPHA_VALUE) = u4Alpha;
    GFX_CMDENQ(GREG_BITBLT_MODE);
}

void _GFX_SetForceAlpha(UINT32 u4Enable)
{
    REG(fg_FORCE_USE_ALPHA) = u4Enable;
    GFX_CMDENQ(GREG_BITBLT_MODE);
}

void _GFX_SetARGB1555(UINT32 u4Enable)
{
    REG(fg_A1555_EN) = u4Enable;
}

void _GFX_SetMaxAlphaEn(UINT32 u4Enable)
{
    REG(fg_ALCOM_MAXALPHA) = u4Enable;
}

void _GFX_SetRGB0En(UINT32 u4Enable)
{
    REG(fg_ALPHA0_RGB0) = u4Enable;
    GFX_CMDENQ(GREG_ALCOM_LOOP);
}

void _GFX_SetXORColor(UINT32 u4Color)
{
    REG(fg_XOR_COLOR) = u4Color;
    GFX_CMDENQ(GREG_XOR_COLOR);
}
void _GFX_Set_PRE_COLORIZE(BOOL u4PreColorize)
{
    REG(fg_PRE_COLORIZE) = u4PreColorize;
    GFX_CMDENQ(GREG_ALCOM_LOOP);
}
void _GFX_Set_FastBitBlt(UINT32 u4FastBitBlt)
{
    if (u4FastBitBlt)
    {
        REG(fg_FAST_BITBLIT) = 0x3;
    }
    else
    {
        REG(fg_FAST_BITBLIT) = 0x0;
    }
    GFX_CMDENQ(GREG_BITBLT_MODE);
}
void _GFX_Set_SrcAlReplace(UINT32 u4SrcAlRep)
{
    REG(fg_SRC_ALPHA_REPLACE) = u4SrcAlRep;
    GFX_CMDENQ(GREG_ALCOM_LOOP);
}
void _GFX_Set_UseRectSrc(UINT32 u4UseRectSrc)
{
    REG(fg_USE_RECT_SRC) = u4UseRectSrc;
    //GFX_CMDENQ(GREG_G_MODE);
}

void _GFX_Set_WriteNoPitch(UINT32 u4WriteNoPitch)
{
    REG(fg_WRITE_NO_PITCH) = u4WriteNoPitch;
    #if DTV_VFY
    GFX_CMDENQ(GREG_G_MODE);
    #endif
}
void _GFX_Set_WriteCbCrOder(UINT32 u4WriteCbCrOder)
{
    REG(fg_WRITE_CBCR_ORDER) = u4WriteCbCrOder;
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
}
void _GFX_Set_RGB24BitOder(UINT32 u4RGB24BitOder)
{
    REG(fg_RGB_24BIT_ORDER) = u4RGB24BitOder;
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
}
void _GFX_Set_SrcDst(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4DstPitch,UINT32 u4DstColorMode,UINT32 u4DstX,UINT32 u4DstY,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight)
{
    UINT32 ui4_dst_addr = 0x0;
#ifndef __KERNEL__
    UINT32 u4HighAddr = (UINT32) & Image$$RO$$Limit; // add memory proction, avoid accessing invaild memory address
    UNUSED(u4HighAddr);
    GFX_ASSERT((UINT32)u4DstAdress > u4HighAddr);
#endif

    GFX_ASSERT(u4DstAdress != NULL);
    GFX_CHECK_PITCH(u4DstPitch);
    _pu1DstBase = u4DstAdress;
    _u4DstCM = u4DstColorMode;
    _u4DstPitch = u4DstPitch;
    _u4DstShift = _auColorModeShift[u4DstColorMode];
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));

    REG(fg_CM) = u4DstColorMode;
    REG(fg_OSD_WIDTH) = (u4DstPitch >> 4);
    REG(fg_SRC_BSAD) = (UINT32) pu1SrcY;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG((UINT32)pu1SrcY);
    REG(fg_SRC_PITCH) = (u4SrcYPitch * 2);
    REG(fg_SRCCBCR_BSAD) = (UINT32) pu1SrcCbCr;
    REG(fg_SRCCBCR_BSAD_MSB) = ((UINT32) pu1SrcCbCr >> 30);
    REG(fg_SRCCBCR_PITCH) = (u4SrcCbCrPitch);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_OP_MODE) = (UINT32) OP_YCBCR_TO_RGB;
    REG(fg_SRC_WIDTH) = u4SrcWidth;
    REG(fg_SRC_HEIGHT) = (u4SrcHeight - 1);
    GFX_SET_BURST_READ();
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    
}
void _GFX_Set_ColConvInfo(UINT32 u4YCFmt, UINT32 u4SwapMode, UINT32 u4VidStd,
        UINT32 u4VidSys,UINT32 u4FieldPic,UINT32 u4Alpha)
{
    REG(fg_YC_FMT) = u4YCFmt;
    REG(fg_VSTD) = u4VidStd;
    REG(fg_VSYS) = u4VidSys;
    REG(fg_SWAP_MODE) = u4SwapMode;
    REG(fg_VSCLIP) = GFX_DISABLE;
    REG(fg_ALPHA_VALUE) = 0xFF;
    REG(fg_FLD_PIC) = u4FieldPic;
    REG(fg_ALPHA_VALUE) = u4Alpha;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;
    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);
}

void _GFX_Set_FlashColCnvInfo(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight)
{
    REG(fg_SRC_BSAD) = (UINT32) pu1SrcY;
    REG(fg_SRC_BSAD_MSB) = ((UINT32) pu1SrcY >> 30);
    REG(fg_SRCCBCR_BSAD) = (UINT32) pu1SrcCbCr;
    REG(fg_SRCCBCR_BSAD_MSB) = ((UINT32) pu1SrcCbCr >> 30);
    REG(fg_SRC_PITCH) = (u4SrcYPitch * 2); //Y pitch (byte) * 2
    REG(fg_SRCCBCR_PITCH) = u4SrcCbCrPitch; //CbCr pitch (byte)
    REG(fg_DST_BSAD) = (UINT32) u4DstAdress;
    REG(fg_DST_BSAD_MSB) = ((UINT32) u4DstAdress >> 30);
    REG(fg_SRC_WIDTH) = u4SrcWidth;
    REG(fg_SRC_HEIGHT) = u4SrcHeight - 1;
    REG(fg_VFULL) = 0x1;
    REG(fg_SWAP_MODE) = 0x2;

    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
}

void _GFX_Set_FlashColCnvInfo_WithPitch(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4Dst_Pitch)
{
    REG(fg_SRC_BSAD) = (UINT32) pu1SrcY;
    REG(fg_SRC_BSAD_MSB) = ((UINT32) pu1SrcY >> 30);
    REG(fg_SRCCBCR_BSAD) = (UINT32) pu1SrcCbCr;
    REG(fg_SRCCBCR_BSAD_MSB) = ((UINT32) pu1SrcCbCr >> 30);
    REG(fg_SRC_PITCH) = (u4SrcYPitch * 2); //Y pitch (byte) * 2
    REG(fg_SRCCBCR_PITCH) = u4SrcCbCrPitch; //CbCr pitch (byte)
    REG(fg_DST_BSAD) = (UINT32) u4DstAdress;
    REG(fg_DST_BSAD_MSB) = ((UINT32) u4DstAdress >> 30);
    REG(fg_OSD_WIDTH) = (u4Dst_Pitch>>4);
    REG(fg_SRC_WIDTH) = u4SrcWidth;
    REG(fg_SRC_HEIGHT) = ((u4SrcHeight-1) & 0x007FF);
    REG(fg_SRC_HEIGHT_12) = (((u4SrcHeight -1)>>11) & 0x1);
    REG(fg_VFULL) = 0x1;
    REG(fg_SWAP_MODE) = 0x2;

    GFX_CMDENQ(GREG_0x4284);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
    REG(fg_SRC_HEIGHT_12) = 0;
}

void _GFX_Set_YCbCr420ToARGB32(UINT32 u4AlphaValue, UINT32 u4FieldPic)
{
    GFX_SET_BURST_READ();
    REG(fg_CM) = 0xE;
    REG(fg_OP_MODE) = 0xA;
    REG(fg_WRITE_NO_PITCH) = 0x1;
    REG(fg_ALPHA_VALUE) = u4AlphaValue;
    REG(fg_FLD_PIC) = u4FieldPic;
    REG(fg_YC_FMT) = 0x0;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;
    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);
}


void _GFX_Set_YCbCrBlockToRaster(UINT32 u4Option)
{
    GFX_SET_BURST_READ();
    REG(fg_OP_MODE) = u4Option;
    REG(fg_CM)      = 0xE;
    REG(fg_WRITE_NO_PITCH) = 0x1;
    REG(fg_FLD_PIC) = 0x0;
    REG(fg_YC_FMT) = 0x0;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;
    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
    GFX_CMDENQ(GREG_G_MODE);
}


void _GFX_Set_YCbCrBlockToRaster_WithPitch(UINT32 u4Option, UINT32 u4WriteNoPitch)
{
    GFX_SET_BURST_READ();
    REG(fg_CM) = 0xE;
    REG(fg_OP_MODE) = u4Option;
    REG(fg_WRITE_NO_PITCH) = u4WriteNoPitch;
    REG(fg_FLD_PIC) = 0x0;
    REG(fg_YC_FMT) = 0x0;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;
    REG(fg_WRITE_NO_PITCH) = 0;
    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
    GFX_CMDENQ(GREG_G_MODE);
}


void _GFX_Set_YuyvToRaster_4284(UINT32 u4Enable)
{
    REG(fg_YuyvConv) = u4Enable;
    GFX_CMDENQ(GREG_0x4284);
}

//-------------------------------------------------------------------------
/** _GFX_SetColor
 *  set color.
 *  color value used in.
 *  rectangle fill.
 *  hline.
 *  vline.
 */
//-------------------------------------------------------------------------
void _GFX_SetColor(UINT32 u4Color)
{
    REG(fg_RECT_COLOR) = u4Color;
    GFX_CMDENQ(GREG_RECT_COLOR);
}

//-------------------------------------------------------------------------
/** _GFX_SetBmpSrc
 *  set bitmap source
 *
 */
//-------------------------------------------------------------------------
void _GFX_SetBmpSrc(UINT8 *pu1Base, UINT32 u4BmpCM)
{
    // no support BMP_CM_8BIT now
    GFX_ASSERT((u4BmpCM == (UINT32)E_BMP_CM_1BIT) ||
            (u4BmpCM == (UINT32)E_BMP_CM_2BIT) ||
            (u4BmpCM == (UINT32)E_BMP_CM_4BIT));

    _pu1BmpBase = pu1Base;
    _u4CharCM = u4BmpCM;

    REG(fg_CHAR_CM) = u4BmpCM;
}

//-------------------------------------------------------------------------
/** _GFX_SetColorMap
 *  set color mapping
 *
 */
//-------------------------------------------------------------------------
void _GFX_SetColorMap(UINT32 u4BmpCM, UINT32 u4DstCM,
        const UINT32 *psu4ColorMap)
{
    GFX_ASSERT(psu4ColorMap != NULL);

    if ((UINT32) E_BMP_CM_2BIT == u4BmpCM)
    {
        switch (u4DstCM)
        {
        case CM_YCbCr_CLUT8:
        case CM_RGB_CLUT8:
            REG(fg_COLOR_TRANS0) = (psu4ColorMap[0] & 0xff)
                    | ((psu4ColorMap[1] & 0xff) << 8) | ((psu4ColorMap[2]
                    & 0xff) << 16) | ((psu4ColorMap[3] & 0xff) << 24);
            break;

        case CM_RGB565_DIRECT16:
        case CM_ARGB1555_DIRECT16:
        case CM_ARGB4444_DIRECT16:
            REG(fg_COLOR_TRANS0) = (psu4ColorMap[0] & 0xffff)
                    | ((psu4ColorMap[1] & 0xffff) << 16);
            REG(fg_COLOR_TRANS1) = (psu4ColorMap[2] & 0xffff)
                    | ((psu4ColorMap[3] & 0xffff) << 16);
            break;

        case CM_AYCbCr8888_DIRECT32:
        case CM_ARGB8888_DIRECT32:
            REG(fg_COLOR_TRANS0) = psu4ColorMap[0];
            REG(fg_COLOR_TRANS1) = psu4ColorMap[1];
            REG(fg_COLOR_TRANS2) = psu4ColorMap[2];
            REG(fg_COLOR_TRANS3) = psu4ColorMap[3];
            break;

        default:
            return;
        }
    }
    else if ((UINT32) E_BMP_CM_4BIT == u4BmpCM)
    {
        switch (u4DstCM)
        {
        case CM_YCbCr_CLUT8:
        case CM_RGB_CLUT8:
            REG(fg_COLOR_TRANS0) = (psu4ColorMap[0] & 0xff)
                    | ((psu4ColorMap[1] & 0xff) << 8) | ((psu4ColorMap[2]
                    & 0xff) << 16) | ((psu4ColorMap[3] & 0xff) << 24);
            REG(fg_COLOR_TRANS1) = (psu4ColorMap[4] & 0xff)
                    | ((psu4ColorMap[5] & 0xff) << 8) | ((psu4ColorMap[6]
                    & 0xff) << 16) | ((psu4ColorMap[7] & 0xff) << 24);
            REG(fg_COLOR_TRANS2) = (psu4ColorMap[8] & 0xff)
                    | ((psu4ColorMap[9] & 0xff) << 8) | ((psu4ColorMap[10]
                    & 0xff) << 16) | ((psu4ColorMap[11] & 0xff) << 24);
            REG(fg_COLOR_TRANS3) = (psu4ColorMap[12] & 0xff)
                    | ((psu4ColorMap[13] & 0xff) << 8) | ((psu4ColorMap[14]
                    & 0xff) << 16) | ((psu4ColorMap[15] & 0xff) << 24);
            break;

        case CM_RGB565_DIRECT16:
        case CM_ARGB1555_DIRECT16:
        case CM_ARGB4444_DIRECT16:
            REG(fg_COLOR_TRANS0) = (psu4ColorMap[0] & 0xffff)
                    | ((psu4ColorMap[1] & 0xffff) << 16);
            REG(fg_COLOR_TRANS1) = (psu4ColorMap[2] & 0xffff)
                    | ((psu4ColorMap[3] & 0xffff) << 16);
            REG(fg_COLOR_TRANS2) = (psu4ColorMap[4] & 0xffff)
                    | ((psu4ColorMap[5] & 0xffff) << 16);
            REG(fg_COLOR_TRANS3) = (psu4ColorMap[6] & 0xffff)
                    | ((psu4ColorMap[7] & 0xffff) << 16);
            REG(fg_COLOR_TRANS4) = (psu4ColorMap[8] & 0xffff)
                    | ((psu4ColorMap[9] & 0xffff) << 16);
            REG(fg_COLOR_TRANS5) = (psu4ColorMap[10] & 0xffff)
                    | ((psu4ColorMap[11] & 0xffff) << 16);
            REG(fg_COLOR_TRANS6) = (psu4ColorMap[12] & 0xffff)
                    | ((psu4ColorMap[13] & 0xffff) << 16);
            REG(fg_COLOR_TRANS7) = (psu4ColorMap[14] & 0xffff)
                    | ((psu4ColorMap[15] & 0xffff) << 16);
            break;

        default:
            return;
        }
    }
    else // for lint happy
    {
        ;
    }

    GFX_CMDENQ(GREG_COL_TRAN0);
    GFX_CMDENQ(GREG_COL_TRAN1);
    GFX_CMDENQ(GREG_COL_TRAN2);
    GFX_CMDENQ(GREG_COL_TRAN3);
    GFX_CMDENQ(GREG_COL_TRAN4);
    GFX_CMDENQ(GREG_COL_TRAN5);
    GFX_CMDENQ(GREG_COL_TRAN6);
    GFX_CMDENQ(GREG_COL_TRAN7);
}

//-------------------------------------------------------------------------
/** _GFX_SetBmpOpt
 *  set bitmap option.
 */
//-------------------------------------------------------------------------
void _GFX_SetBmpOpt(BOOL fgSubPicDec, BOOL fgRunlenDec)
{
    // sub-picture decode ?
    if (fgSubPicDec == TRUE)
    {
        REG(fg_SPU_RLDEC) = 1;
    }
    else
    {
        REG(fg_SPU_RLDEC) = 0;
    }

    // run length decode ?
    if (fgRunlenDec == TRUE)
    {
        REG(fg_RL_DEC) = 1;
    }
    else
    {
        REG(fg_RL_DEC) = 0;
    }

    GFX_CMDENQ(GREG_SRC_SIZE);
}

//-------------------------------------------------------------------------
/** _GFX_SetSrcColorKey
 *  set src color key.
 */
//-------------------------------------------------------------------------
void _GFX_SetSrcColorKey(BOOL fgSrcKeyEn, UINT32 u4SrcKey)
{
    REG(fg_ALCOM_SRC_KEY_EN) = (UINT32) fgSrcKeyEn;
    REG(fg_ALCOM_SRC_KEY) = u4SrcKey;
    GFX_CMDENQ(GREG_0x40DC);
}

//-------------------------------------------------------------------------
/** _GFX_SetThirdColorKey
 *  set src color key.
 */
//-------------------------------------------------------------------------
void _GFX_SetThirdColorKey(BOOL fgThirdKeyEn, UINT32 u4ThirdKey)
{
    REG(fg_ALCOM_THIRD_KEY_EN) = (UINT32) fgThirdKeyEn;
    REG(fg_ALCOM_THIRD_KEY) = u4ThirdKey;
    GFX_CMDENQ(GREG_0x40E0);
}

//-------------------------------------------------------------------------
/** _GFX_BmpBlt
 *  bitmap/text mapping.
 *  
 */
//-------------------------------------------------------------------------
void _GFX_BmpBlt(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    UINT32 ui4_src_addr = 0x0;
    UINT32 ui4_dst_addr = 0x0;

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    ui4_src_addr = (UINT32)(_pu1BmpBase);

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    REG(fg_OP_MODE) = (UINT32) OP_BITMAP_BLT;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}

//-------------------------------------------------------------------------
/** _GFX_Fill
 *  rectangle fill.
 *  calculate start memory address base + (y * width) + x.
 *  set fill width.
 *  set fill height.
 *  enque output address.
 *  enque input size.
 *  enque gfx mode (include fire control).
 */
//-------------------------------------------------------------------------
void _GFX_Fill(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    UINT32 ui4_dst_addr = 0x0;

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));

    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    REG(fg_OP_MODE) = (UINT32) OP_RECT_FILL;
    REG(fg_SRC_WIDTH) = u4Width;
    #if defined(CC_MT5890) ||defined(CC_MT5882)
    REG(fg_SRC_HEIGHT) = ((u4Height-1) & 0x007FF);
    REG(fg_SRC_HEIGHT_12) = (((u4Height -1)>>11) & 0x1);
    #else
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    #endif
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();
    
  #if defined(CC_MT5890) ||defined(CC_MT5882)
    //REG(fg_SRC_HEIGHT_12) = 0;
    GFX_CMDENQ(GREG_0x4284);
  #endif
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);

}

//-------------------------------------------------------------------------
/** _GFX_HLine
 *  draw horizontal line.
 *  calculate output address.
 *  set operation mode.
 *  set width.
 *  enque relative registers.
 */
//-------------------------------------------------------------------------
void _GFX_HLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width)
{
    UINT32 ui4_dst_addr = 0x0;

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    REG(fg_OP_MODE) = (UINT32) OP_DRAW_HLINE;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}

//-------------------------------------------------------------------------
/** _GFX_VLine
 *  draw vertical line.
 *  calculate output address.
 *  set operation mode.
 *  set height.
 *  enque relative registers.
 */
//-------------------------------------------------------------------------
void _GFX_VLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Height)
{
    UINT32 ui4_dst_addr = 0x0;

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    REG(fg_OP_MODE) = (UINT32) OP_DRAW_VLINE;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}

//-------------------------------------------------------------------------
/** _GFX_SetBltOpt
 *  set bitblt option.
 *  set transparent enable bit.
 *  set color change enable bit.
 *  set keynot enable bit.
 *  set clip enable bit.
 *  set 4bpp special mode bit.
 *  set color key min.
 *  set color key max.
 *  set color conversion enable to false.
 *  enque color key min and max.
 */
//-------------------------------------------------------------------------
void _GFX_SetBltOpt(UINT32 u4Switch, UINT32 u4ColorMin, UINT32 u4ColorMax)
{
    GFX_ASSERT(!((u4Switch & D_GFXFLAG_TRANSPARENT) &&
                    (u4Switch & D_GFXFLAG_COLORCHANGE)));

    REG(fg_TRANS_ENA) = (u4Switch & D_GFXFLAG_TRANSPARENT) ? 1 : 0;
    REG(fg_KEYNOT_ENA) = (u4Switch & D_GFXFLAG_KEYNOT) ? 1 : 0;
    REG(fg_COLCHG_ENA) = (u4Switch & D_GFXFLAG_COLORCHANGE) ? 1 : 0;
    REG(fg_KEYSDSEL) = (u4Switch & D_GFXFLAG_KEYSDSEL) ? 1 : 0;
    REG(fg_DSTPITCH_DEC) = (u4Switch & D_GFXFLAG_FLIP) ? 1 : 0;
    REG(fg_MIRROR) = (u4Switch & D_GFXFLAG_MIRROR) ? 1 : 0;

    REG(fg_DST_WR_ROTATE) = (u4Switch & D_GFXFLAG_DST_WR_RT) ? 1 : 0;
    REG(fg_DST_RD_ROTATE) = (u4Switch & D_GFXFLAG_DST_RD_RT) ? 1 : 0;

    if (u4Switch & D_GFXFLAG_OVERLAP)
    {
        REG(fg_DSTPITCH_DEC) = (UINT32) 1;
        REG(fg_MIRROR) = (UINT32) 1;
        REG(fg_SRCPITCH_DEC) = (UINT32) 1;
        REG(fg_SRCMIRROR) = (UINT32) 1;
    }

    REG(fg_COLOR_KEY_MIN) = u4ColorMin;
    REG(fg_COLOR_KEY_MAX) = u4ColorMax;
    REG(fg_CFMT_ENA) = 0;

    GFX_CMDENQ(GREG_KEY_DATA0);
    GFX_CMDENQ(GREG_KEY_DATA1);
    GFX_CMDENQ(GREG_BITBLT_MODE);

    _u4GfxBltSwitch = u4Switch;
}

//-------------------------------------------------------------------------
/** _GFX_BitBlt
 *  bitblt.
 *  calculate output address.
 *  calculate input address.
 *  set bitblt op mode.
 *  set color conversion flag.
 *  enque output address register.
 *  enque input address register.
 *  enque size and clip registers.
 *  enque bitblp option register.
 *  enque mode register.
 */
//-------------------------------------------------------------------------
void _GFX_BitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY,
        UINT32 u4Width, UINT32 u4Height)
{
    UINT32 ui4_src_addr = 0x0;
    UINT32 ui4_dst_addr = 0x0;

    //destination base address
    if ((_u4GfxBltSwitch & D_GFXFLAG_FLIP) && (_u4GfxBltSwitch
            & D_GFXFLAG_MIRROR))
    {
        ui4_dst_addr = (UINT32)(
                _pu1DstBase + ((u4DstY + u4Height - 1) * _u4DstPitch)
                        + ((u4DstX + u4Width - 1) << _u4DstShift));
    }
    else if (_u4GfxBltSwitch & D_GFXFLAG_FLIP)
    {
        ui4_dst_addr = (UINT32)(
                _pu1DstBase + ((u4DstY + u4Height - 1) * _u4DstPitch) + (u4DstX
                        << _u4DstShift));
    }
    else if (_u4GfxBltSwitch & D_GFXFLAG_MIRROR)
    {
        ui4_dst_addr = (UINT32)(
                _pu1DstBase + (u4DstY * _u4DstPitch) + ((u4DstX + u4Width - 1)
                        << _u4DstShift));
    }
    else if (_u4GfxBltSwitch & D_GFXFLAG_OVERLAP)
    {
        ui4_dst_addr = (UINT32)(
                _pu1DstBase + ((u4DstY + u4Height - 1) * _u4DstPitch)
                        + ((u4DstX + u4Width - 1) << _u4DstShift));
    }
    else
    {
        ui4_dst_addr = (UINT32)(
                _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    }

    if (_u4GfxBltSwitch & D_GFXFLAG_OVERLAP)
    {
        ui4_src_addr = (UINT32)(
                _pu1SrcBase + ((u4SrcY + u4Height - 1) * _u4SrcPitch)
                        + ((u4SrcX + u4Width - 1) << _u4SrcShift));
    }
    else
    {
        //source base address
        ui4_src_addr = (UINT32)(
                _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));
    }

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    if(u4Width>GFX_FAST_BITBLIT_MIN_W)
    {
        REG(fg_FAST_BITBLIT) = (UINT32) GFX_FAST_BITBLIT_ENABLE;
    }
    else
    {
        REG(fg_FAST_BITBLIT) = (UINT32) GFX_FAST_BITBLIT_DISABLE;
    }

    REG(fg_OP_MODE) = (UINT32) OP_BITBLT;
    REG(fg_CFMT_ENA) = (UINT32)(_u4DstCM != _u4SrcCM);
    REG(fg_SRC_WIDTH) = u4Width;

    REG(fg_SRC_HEIGHT) = ((u4Height-1) & 0x007FF);
    REG(fg_SRC_HEIGHT_12) = (((u4Height -1)>>11) & 0x1);
    //REG(fg_A1555_EN)       = GFX_ENABLE;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();
    GFX_CMDENQ(GREG_0x4284);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);
    REG(fg_SRC_HEIGHT_12) = 0;
    GFX_CMDENQ(GREG_0x4284);

    // reset bitblt option
    _GFX_SetBltOpt(0, 0x0, 0xffffffff);
}

//-------------------------------------------------------------------------
/** _GFX_Blend
 *  alpha blending.
 *  calculate input and output address.
 *  set op mode.
 *  set width and height.
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_Blend(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY,
        UINT32 u4Width, UINT32 u4Height)
{
    UINT32 ui4_src_addr = 0x0;
    UINT32 ui4_dst_addr = 0x0;

    GFX_ASSERT(_u4SrcCM == _u4DstCM);

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_OP_MODE) = (UINT32) OP_ALPHA_BLEND;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);
}

//-------------------------------------------------------------------------
/** _GFX_Compose
 *  alpha composition.
 *  set input gfx region.
 *  set output gfx region.
 *  do pass 0 ~ pass 3.
 */
//-------------------------------------------------------------------------
/*lint -save -e534 -e774 -e831  */
void _GFX_Compose(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY,
        UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, UINT32 u4Mode)
{
    UINT32 u4MaxLines;
    UINT32 u4Line, u4TotalLines;
    INT32 i4MhpPatch = GFX_ENABLE;
    UINT32 ui4_src_addr = 0x0;
    UINT32 ui4_dst_addr = 0x0;

    GFX_ASSERT(_u4SrcCM == _u4DstCM);

    // Add CLEAR, SRC, SRC_OVER patches for MHP project
    // 1. CLEAR    -> use Rect Fill (fill 0x0 in rect_color)
    // 2. SRC      -> use Pass0 and Pass5 of Alpha Composition
    // 3. SRC_OVER -> consider two corner cases
    //          case1: Ar=1.0 & As=1.0
    //          case2: Ar=1.0 & As=0.0
    //          note: Ar=0.0 is the same as case2
    if (((UINT32) CM_ARGB8888_DIRECT32 == _u4SrcCM)
            && ((UINT32) CM_ARGB8888_DIRECT32 == _u4DstCM) && (GFX_ENABLE
            == i4MhpPatch))
    {
        if ((UINT32) E_AC_CLEAR == u4Mode)
        {
            UINT32 u4RectColor = REG(fg_RECT_COLOR);

            GFX_SetColor(0x0);
            GFX_Fill(u4DstX, u4DstY, u4Width, u4Height);

            // restore original value
            GFX_SetColor(u4RectColor);

            return;
        }
        else if ((UINT32) E_AC_SRC == u4Mode)
        {
            if (0 == u4Ar)
            {
                UINT32 u4RectColor = REG(fg_RECT_COLOR);

                GFX_SetColor(0x0);
                GFX_Fill(u4DstX, u4DstY, u4Width, u4Height);

                // restore original value
                GFX_SetColor(u4RectColor);
            }
            else
            {
                // 1. Pass 0
                GFX_AlphaComposePass(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width,
                        u4Height, 0, u4Ar);
                GFX_Flush(); // flush queue
            }

            return;
        }
        else if (((UINT32) E_AC_SRC_OVER == u4Mode) && (0 == u4Ar))
        {
            UINT32 u4RectColor = REG(fg_RECT_COLOR);
            UINT32 u4MinColor = REG(fg_COLOR_KEY_MIN);
            UINT32 u4MaxColor = REG(fg_COLOR_KEY_MAX);
            UINT32 u4Switch = 0x0;

            if (REG(fg_TRANS_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_TRANSPARENT);
            }

            if (REG(fg_KEYNOT_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_KEYNOT);
            }

            if (REG(fg_COLCHG_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_COLORCHANGE);
            }

            // BitBlt with ColorChange (from Dst to Dst)
            GFX_SetColor(0x0);
            GFX_SetBltOpt(D_GFXFLAG_COLORCHANGE, 0x00000000, 0x00FFFFFF);

            // set new src & dst pitch
            REG(fg_OSD_WIDTH) = _u4DstPitch >> 4;
            REG(fg_SRC_PITCH) = _u4DstPitch >> 4;

            ui4_dst_addr = (UINT32)(
                    _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX
                            << _u4DstShift));
            ui4_src_addr = (UINT32)(
                    _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX
                            << _u4DstShift));

            REG(fg_SRC_BSAD) = ui4_src_addr;
            REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
            REG(fg_DST_BSAD) = ui4_dst_addr;
            REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
            REG(fg_OP_MODE) = (UINT32) OP_BITBLT;
            REG(fg_CFMT_ENA) = (UINT32)(_u4DstCM != _u4SrcCM);
            REG(fg_SRC_WIDTH) = u4Width;
            REG(fg_SRC_HEIGHT) = ((u4Height-1) & 0x007FF);
            REG(fg_SRC_HEIGHT_12) = (((u4Height -1)>>11) & 0x1);
            
            REG(fg_FIRE) = GFX_ENGINE_FIRE;
            GFX_SET_BURST_READ();
            

            GFX_CMDENQ(GREG_0x4284);
            GFX_CMDENQ(GREG_S_OSD_WIDTH);
            GFX_CMDENQ(GREG_DST_BSAD);
            GFX_CMDENQ(GREG_SRC_BSAD);
            GFX_CMDENQ(GREG_ADDR_MSB1);
            GFX_CMDENQ(GREG_SRC_SIZE);
            GFX_CMDENQ(GREG_BITBLT_MODE);
            GFX_CMDENQ(GREG_G_MODE);

            // restore original value
            REG(fg_OSD_WIDTH) = _u4DstPitch >> 4;
            REG(fg_SRC_PITCH) = _u4SrcPitch >> 4;
            ui4_src_addr = (UINT32)(
                    _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX
                            << _u4SrcShift));
            REG(fg_SRC_BSAD) = ui4_src_addr;
            REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
            GFX_SetColor(u4RectColor);
            GFX_SetBltOpt(u4Switch, u4MinColor, u4MaxColor);
            GFX_CMDENQ(GREG_S_OSD_WIDTH);
            GFX_CMDENQ(GREG_SRC_BSAD);
            GFX_CMDENQ(GREG_ADDR_MSB1);
            GFX_CMDENQ(GREG_BITBLT_MODE);
            REG(fg_SRC_HEIGHT_12) = 0; 
            return;
        }
        else
        {
            //do nothing, for lint happy
        }
    }

    // calculate how many lines can we handle using working buffer
    _u4WorkingPitch = ((u4Width << _u4DstShift) + 0xf) & ~0xf;
    u4MaxLines = (_u4WorkingBufSize / _u4WorkingPitch);

    if (u4Height > u4MaxLines)
    {
        // break down whole area into sets of lines
        u4TotalLines = (u4Height - u4MaxLines);
        for (u4Line = 0; u4Line < u4TotalLines; u4Line += u4MaxLines)
        {
            _GfxAlphaCompPass0(u4SrcX, (u4SrcY + u4Line), u4DstX,
                    (u4DstY + u4Line), u4Ar);
            _GfxAlphaCompPass(u4Width, u4MaxLines);
            _GfxAlphaCompPass1a();
            _GfxAlphaCompPass(u4Width, u4MaxLines);
            _GfxAlphaCompPass1b(u4SrcX, (u4SrcY + u4Line), u4DstX,
                    (u4DstY + u4Line));
            _GfxAlphaCompPass(u4Width, u4MaxLines);
            _GfxAlphaCompPass2(u4SrcX, (u4SrcY + u4Line), u4DstX,
                    (u4DstY + u4Line), u4Mode);
            _GfxAlphaCompPass(u4Width, u4MaxLines);
            _GfxAlphaCompPass3(u4SrcX, (u4SrcY + u4Line), u4DstX,
                    (u4DstY + u4Line));
            _GfxAlphaCompPass(u4Width, u4MaxLines);
        }
        u4MaxLines = (u4Height - u4Line);
    }
    else
    {
        u4Line = 0;
        u4MaxLines = u4Height;
    }

    // process last few lines
    if (u4MaxLines)
    {
        _GfxAlphaCompPass0(u4SrcX, (u4SrcY + u4Line), u4DstX,
                (u4DstY + u4Line), u4Ar);
        _GfxAlphaCompPass(u4Width, u4MaxLines);
        _GfxAlphaCompPass1a();
        _GfxAlphaCompPass(u4Width, u4MaxLines);
        _GfxAlphaCompPass1b(u4SrcX, (u4SrcY + u4Line), u4DstX,
                (u4DstY + u4Line));
        _GfxAlphaCompPass(u4Width, u4MaxLines);
        _GfxAlphaCompPass2(u4SrcX, (u4SrcY + u4Line), u4DstX,
                (u4DstY + u4Line), u4Mode);
        _GfxAlphaCompPass(u4Width, u4MaxLines);
        _GfxAlphaCompPass3(u4SrcX, (u4SrcY + u4Line), u4DstX, (u4DstY + u4Line));
        _GfxAlphaCompPass(u4Width, u4MaxLines);
    }

    if (((UINT32) CM_ARGB8888_DIRECT32 == _u4SrcCM)
            && ((UINT32) CM_ARGB8888_DIRECT32 == _u4DstCM) && (GFX_ENABLE
            == i4MhpPatch))
    {
        if ((u4Mode == (UINT32) E_AC_SRC_OVER) && (255 == u4Ar))
        {
            UINT32 u4MinColor = REG(fg_COLOR_KEY_MIN);
            UINT32 u4MaxColor = REG(fg_COLOR_KEY_MAX);
            UINT32 u4Switch = 0x0;

            if (REG(fg_TRANS_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_TRANSPARENT);
            }

            if (REG(fg_KEYNOT_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_KEYNOT);
            }

            if (REG(fg_COLCHG_ENA))
            {
                u4Switch = (u4Switch | D_GFXFLAG_COLORCHANGE);
            }

            // restore src & dst pitch
            REG(fg_OSD_WIDTH) = _u4DstPitch >> 4;
            REG(fg_SRC_PITCH) = _u4SrcPitch >> 4;
            GFX_CMDENQ(GREG_S_OSD_WIDTH);

            // do (Transparent & Keynot)-BitBlt
            GFX_SetBltOpt((D_GFXFLAG_TRANSPARENT | D_GFXFLAG_KEYNOT),
                    0xFF000000, 0xFFFFFFFF);
            GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);

            // restore bltopt
            GFX_SetBltOpt(u4Switch, u4MinColor, u4MaxColor);
            GFX_CMDENQ(GREG_BITBLT_MODE);
        }
    }

    // restore src & dst pitch
    REG(fg_OSD_WIDTH) = (_u4DstPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4SrcPitch >> 4);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}
/*lint -restore */

//-------------------------------------------------------------------------
/** _GFX_AlphaComposePass
 *  alpha compose pass.
 *  exported for pass 4 copy color and pass 5 copy alpha.
 */
//-------------------------------------------------------------------------
void _GFX_AlphaComposePass(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Pass,
        UINT32 u4Param)
{
    UINT32 ui4_src_addr = 0x0;
    UINT32 ui4_dst_addr = 0x0;

    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4DstShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_ALCOM_PASS) = u4Pass;
    REG(fg_ALPHA_VALUE) = u4Param;
    REG(fg_OSD_WIDTH) = (_u4DstPitch >> 4);
    REG(fg_SRC_PITCH) = (_u4SrcPitch >> 4);

    REG(fg_OP_MODE) = (UINT32) OP_ALPHA_COMPOSE;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = ((u4Height-1) & 0x007FF);
    REG(fg_SRC_HEIGHT_12) = (((u4Height -1)>>11) & 0x1);
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();
     
    GFX_CMDENQ(GREG_0x4284);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
    GFX_CMDENQ(GREG_G_MODE);
    REG(fg_SRC_HEIGHT_12) = 0;
}

//-------------------------------------------------------------------------
/** _GFX_SetColCnvFmt
 *  set ycbcr to rgb color conversion format.
 *  set yc format.
 *  set video standard.
 *  set video system.
 *  set mtk swap mode.
 *  enable saturate.
 */
//-------------------------------------------------------------------------
void _GFX_SetColCnvFmt(UINT32 u4YCFmt, UINT32 u4SwapMode, UINT32 u4VidStd,
        UINT32 u4VidSys)
{
    REG(fg_YC_FMT) = u4YCFmt;
    REG(fg_VSTD) = u4VidStd;
    REG(fg_VSYS) = u4VidSys;
    REG(fg_SWAP_MODE) = u4SwapMode;
    REG(fg_VSCLIP) = GFX_ENABLE;
    REG(fg_ALPHA_VALUE) = 0xFF;

    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
    GFX_CMDENQ(GREG_BITBLT_MODE);

}

//-------------------------------------------------------------------------
/** _GFX_SetVFull
 */
//-------------------------------------------------------------------------
void _GFX_SetVFull(UINT32 u4VFull)
{
    REG(fg_VFULL) = u4VFull;
}

//-------------------------------------------------------------------------
/** _GFX_SetColCnvSrc
 *  set color conversion source.
 *  luma input address, line size.
 *  chroma input address, linesize.
 *  field/frame mode.
 */
//-------------------------------------------------------------------------
void _GFX_SetColCnvSrc(UINT8 *pu1SrcLuma, UINT32 u4SrcLumaPitch,
        UINT8 *pu1SrcChroma, UINT32 u4SrcChromaPitch, UINT32 u4FieldPic)
{
    REG(fg_SRC_BSAD) = (UINT32) pu1SrcLuma;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG((UINT32)pu1SrcLuma);
    REG(fg_SRC_PITCH) = (u4SrcLumaPitch >> 4);
    REG(fg_SRCCBCR_BSAD) = (UINT32) pu1SrcChroma;
    REG(fg_SRCCBCR_BSAD_MSB) = GFX_REG_ADDR_MSG((UINT32)pu1SrcChroma);
    REG(fg_SRCCBCR_PITCH) = (u4SrcChromaPitch >> 4);
    REG(fg_FLD_PIC) = u4FieldPic;

    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
}

#ifdef GFX_CC_YC

void _GFX_SetColCnvSrc_(UINT8 *pu1SrcLuma, UINT32 u4SrcLumaPitch,
        UINT8 *pu1SrcChroma, UINT32 u4SrcChromaPitch, UINT32 u4FieldPic)
{
    REG(fg_SRC_BSAD) = (UINT32) pu1SrcLuma;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG((UINT32)pu1SrcLuma);
    REG(fg_SRC_PITCH) = (u4SrcLumaPitch << 1);
    REG(fg_SRCCBCR_BSAD) = (UINT32) pu1SrcChroma;
    REG(fg_SRCCBCR_BSAD_MSB) = GFX_REG_ADDR_MSG((UINT32)pu1SrcChroma);
    REG(fg_SRCCBCR_PITCH) = (u4SrcChromaPitch);
    REG(fg_FLD_PIC) = u4FieldPic;

    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
}

//-------------------------------------------------------------------------
/** _GFX_SetColCnvFmt_
 *  set ycbcr to rgb color conversion format.
 *  set yc format.
 *  set video standard.
 *  set video system.
 *  set mtk swap mode.
 *  enable saturate.
 */
//-------------------------------------------------------------------------
void _GFX_SetColCnvFmt_(UINT32 u4YCFmt, UINT32 u4SwapMode, UINT32 u4VidStd,
        UINT32 u4VidSys)
{
    REG(fg_YC_FMT) = u4YCFmt;
    REG(fg_VSTD) = u4VidStd;
    REG(fg_VSYS) = u4VidSys;
    REG(fg_SWAP_MODE) = u4SwapMode;
    REG(fg_VSCLIP) = GFX_DISABLE;
    REG(fg_ALPHA_VALUE) = 0xFF;

    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
    GFX_CMDENQ(GREG_BITBLT_MODE);

}
void _GFX_SetDst_(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch)
{
#ifndef __KERNEL__
    UINT32 u4HighAddr = (UINT32) & Image$$RO$$Limit;
    UNUSED(u4HighAddr);
    GFX_ASSERT((UINT32)pu1Base > u4HighAddr);
#endif
    // check 128 bits (16 bytes) alignment
    GFX_CHECK_PITCH(u4Pitch);

    //GFX_CHK_PITCH(u4Pitch);
    GFX_ASSERT(pu1Base != NULL);

    _pu1DstBase = pu1Base;
    _u4DstCM = u4ColorMode;
    _u4DstPitch = u4Pitch;
    _u4DstShift = _auColorModeShift[u4ColorMode];

    REG(fg_CM) = u4ColorMode;
    REG(fg_OSD_WIDTH) = (u4Pitch >> 4);

    GFX_CMDENQ(GREG_S_OSD_WIDTH);
}

BOOL _GFX_ColorConvert_YC(GFX_COLOR_CONVERT_T* prGfxColorConvert)
{
    UINT32 udx, udy;

    ASSERT(prGfxColorConvert->pu1SrcAddr != NULL);
    ASSERT(prGfxColorConvert->pu1DstAddr != NULL);
    ASSERT(prGfxColorConvert->pu1CbCr != NULL);

    ASSERT(prGfxColorConvert->u4Width > 0);
    ASSERT(prGfxColorConvert->u4Height > 0);

    udx = (UINT32) prGfxColorConvert->u4DstX;
    udy = (UINT32) prGfxColorConvert->u4DstY;

    GFX_Lock();

    if (prGfxColorConvert->t_mmu.u4_enable)
    {
        _GFX_MMU_Set_Cfg(&prGfxColorConvert->t_mmu);
    }
    else
    {
        GFX_MMU_Set_Enable(FALSE);
    }

    // one pass transform
    _GFX_SetColCnvFmt_(E_YCFMT_420MB, (UINT32) E_SWAP_BLOCK,
            (UINT32) E_VSTD_BT601, (UINT32) E_VSYS_VID);

    _GFX_SetColCnvSrc_(prGfxColorConvert->pu1SrcAddr,
            prGfxColorConvert->u4SrcPitch, prGfxColorConvert->pu1CbCr,
            prGfxColorConvert->u4SrcPitch, 0);

    _GFX_SetDst_(prGfxColorConvert->pu1DstAddr, prGfxColorConvert->u4DstCM,
            prGfxColorConvert->u4DstPitch);

    GFX_SetAlpha((UINT32) prGfxColorConvert->u1Alpha);

    GFX_ColConv(udx, udy, prGfxColorConvert->u4Width,
            prGfxColorConvert->u4Height);

    GFX_Flush();
    GFX_Task_Wait_Sync();

    GFX_Unlock();

    return TRUE;
}
#endif

//-------------------------------------------------------------------------
/** _GFX_ColConv
 *  convert ycbcr to rgb.
 *  calculate output address.
 *  set output width height.
 */
//-------------------------------------------------------------------------
void _GFX_ColConv(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    UINT32 ui4_dst_addr = 0x0;

    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    REG(fg_OP_MODE) = (UINT32) OP_YCBCR_TO_RGB;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = ((u4Height-1) & 0x007FF);
    REG(fg_SRC_HEIGHT_12) = (((u4Height -1)>>11) & 0x1);
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_0x4284);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);

    REG(fg_SRC_HEIGHT_12) = 0;
}

void _GFX_SetDstWrRgb24Bit(UINT32 u4Enable)
{
    REG(fg_DSTWR_RGB_24BIT) = u4Enable;
}

void _GFX_SetReadArgbOrder(UINT32 u4Enable, UINT32 u4Aorder, UINT32 u4Rorder,
        UINT32 u4Gorder, UINT32 u4Border)
{
    if (u4Enable)
    {
        REG(fg_SRC_RD_B_ORDER) = u4Border;
        REG(fg_SRC_RD_G_ORDER) = u4Gorder;
        REG(fg_SRC_RD_R_ORDER) = u4Rorder;
        REG(fg_SRC_RD_A_ORDER) = u4Aorder;
    }
    else
    {
        REG(fg_DST_RD_B_ORDER) = u4Border;
        REG(fg_DST_RD_G_ORDER) = u4Gorder;
        REG(fg_DST_RD_R_ORDER) = u4Rorder;
        REG(fg_DST_RD_A_ORDER) = u4Aorder;
    }
    GFX_CMDENQ(GREG_READ_ARGB_ORDER);

}

void _GFX_SetWriteArgbOrder(UINT32 u4Aorder, UINT32 u4Rorder, UINT32 u4Gorder,
        UINT32 u4Border)
{

    REG(fg_DST_WR_B_ORDER) = u4Border;
    REG(fg_DST_WR_G_ORDER) = u4Gorder;
    REG(fg_DST_WR_R_ORDER) = u4Rorder;
    REG(fg_DST_WR_A_ORDER) = u4Aorder;
    GFX_CMDENQ(GREG_READ_ARGB_ORDER);

}

//-------------------------------------------------------------------------
/** _GFX_Fltr_CbCr
 *  filter cbcr to cb,cr.
 */
//-------------------------------------------------------------------------
static void _GFX_Fltr_CbCr(GFX_FLTR_CBCR_T *pt_cbcr)
{
    GFX_ASSERT(pt_cbcr);
    GFX_ASSERT(0x0 == (pt_cbcr->ui4_src_pitch%0x10));
    GFX_ASSERT(pt_cbcr->ui4_src_pitch >= 0x20);

    /* pass 1: filter cb*/REG(fg_OP_MODE) = (UINT32) OP_STRETCH_BITBLT;
    REG(fg_CM) = CM_RGB_CLUT8;

    REG(fg_SRC_BSAD) = pt_cbcr->ui4_cbcr_addr;
    REG(fg_DST_BSAD) = pt_cbcr->ui4_cb_addr;

    REG(fg_SRC_WIDTH) = (pt_cbcr->ui4_src_width - 1);
    REG(fg_SRC_HEIGHT) = (pt_cbcr->ui4_src_height - 1);
    REG(fg_OSD_WIDTH) = (pt_cbcr->ui4_src_pitch >> 5);
    REG(fg_STR_DST_WIDTH) = (pt_cbcr->ui4_src_width >> 1);
    REG(fg_STR_DST_HEIGHT) = (pt_cbcr->ui4_src_height - 1);
    REG(fg_STR_BLT_H) = 0x20000;
    REG(fg_STR_BLT_V) = 0x10000;

    REG(fg_SRC_CM) = CM_RGB_CLUT8;
    REG(fg_SRC_PITCH) = (pt_cbcr->ui4_src_pitch >> 4);

    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_STR_BLT_H);
    GFX_CMDENQ(GREG_STR_BLT_V);
    GFX_CMDENQ(GREG_STR_DST_SIZE);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);

    GFX_CMDENQ(GREG_G_MODE);

    /* pass 2: filter cr*/REG(fg_SRC_BSAD) = (pt_cbcr->ui4_cbcr_addr + 1);
    REG(fg_DST_BSAD) = pt_cbcr->ui4_cr_addr;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_G_MODE);

    return;
}

//-------------------------------------------------------------------------
/** GFX_Fltr_CbCr
 *  filter cbcr, firing gfx hw,and waiting finished.
 */
//-------------------------------------------------------------------------
void GFX_Fltr_CbCr(GFX_FLTR_CBCR_T *pt_cbcr)
{
    _GFX_Lock();
    //_GFX_LockCmdque();

    _GFX_Fltr_CbCr(pt_cbcr);

    _GFX_Flush();
    _GFX_Wait();

    //_GFX_UnlockCmdque();
    _GFX_Unlock();

    return;
}

//-------------------------------------------------------------------------
/** _GFX_StretchBlt
 *  stretch bitblt.
 *  calculate output address.
 *  calculate input address.
 *  calculate ration.
 *  set input width height.
 *  set output width height.
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_StretchBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW,
        UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW,
        UINT32 u4DstH)
{
    UINT32 u4MnV, u4MnH;
    UINT32 ui4_src_addr = 0x0;
    UINT32 ui4_dst_addr = 0x0;

    GFX_ASSERT(_u4DstShift == _u4SrcShift);

    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    // calculate m n ratio for scale up and down
    if ((u4DstW == u4SrcW) || (u4DstW == 1))
    {
        u4MnH = 0x00010000;
        u4SrcW = u4DstW;
    }
    else if (u4SrcW > u4DstW) // scale down
    {
        u4MnH = (u4SrcW << 16) / (u4DstW - 1);
        if (((u4SrcW << 16) % (u4DstW - 1)) == 0)
        {
            u4MnH--;
        }
    }
    else // (u4DstW > u4SrcW)
    {
        UINT32 u4Step;

        u4Step = (u4SrcW << 16) / (u4DstW);
        if ((u4Step * (u4DstW - 1)) < ((u4SrcW - 1) << 16))
        {
            u4MnH = (u4SrcW << 16) / (u4DstW - 1);
            if (u4MnH == 0x10000)
            {
                u4MnH--;
            }
        }
        else
        {
            u4MnH = u4Step;
        }
    }

    if ((u4DstH == u4SrcH) || (u4DstH == 1))
    {
        u4MnV = 0x00010000;
        u4SrcH = u4DstH;
    }
    else
    {
        u4MnV = (u4SrcH << 16) / (u4DstH - 1);
        if (u4MnV == 0x00010000)
        {
            u4MnV = (u4SrcH << 16) / (u4DstH);
        }
    }

    REG(fg_OP_MODE) = (UINT32) OP_STRETCH_BITBLT;
    REG(fg_SRC_WIDTH) = u4SrcW;

    REG(fg_SRC_HEIGHT)     = ((u4SrcH-1) & 0x7FF);
    REG(fg_SRC_HEIGHT_12)  = (((u4SrcH -1)>>11) & 0x1);

    REG(fg_STR_DST_WIDTH)  = u4DstW;
    REG(fg_STR_DST_HEIGHT) = (u4DstH - 1);
    REG(fg_STR_BLT_H) = u4MnH;
    REG(fg_STR_BLT_V) = u4MnV;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_0x4284);
    GFX_CMDENQ(GREG_STR_BLT_H);
    GFX_CMDENQ(GREG_STR_BLT_V);
    GFX_CMDENQ(GREG_STR_DST_SIZE);
    GFX_CMDENQ(GREG_G_MODE);

    REG(fg_SRC_HEIGHT_12) =0x0;
}

//-------------------------------------------------------------------------
/** _GFX_StretchBlt_forYBlock
 *  stretch bitblt.
 *  calculate output address.
 *  calculate input address.
 *  calculate ration.
 *  set input width height.(set src widht -1) //stretch y out from yuyv
 *  set output width height.  
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_StretchBlt_forYBlock(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW,
        UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW,
        UINT32 u4DstH)
{
    UINT32 u4MnV, u4MnH;
    UINT32 ui4_src_addr = 0x0;
    UINT32 ui4_dst_addr = 0x0;

    GFX_ASSERT(_u4DstShift == _u4SrcShift);

    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    // calculate m n ratio for scale up and down
    if ((u4DstW == u4SrcW) || (u4DstW == 1))
    {
        u4MnH = 0x00010000;
        u4SrcW = u4DstW;
    }
    else if (u4SrcW > u4DstW) // scale down 
    {
        u4MnH = (u4SrcW << 16) / (u4DstW );
    }
    else // (u4DstW > u4SrcW)
    {
        UINT32 u4Step;

        u4Step = (u4SrcW << 16) / (u4DstW);
        if ((u4Step * (u4DstW - 1)) < ((u4SrcW - 1) << 16))
        {
            u4MnH = (u4SrcW << 16) / (u4DstW - 1);
            if (u4MnH == 0x10000)
            {
                u4MnH--;
            }
        }
        else
        {
            u4MnH = u4Step;
        }
    }

    if ((u4DstH == u4SrcH) || (u4DstH == 1))
    {
        u4MnV = 0x00010000;
        u4SrcH = u4DstH;
    }
    else
    {
        u4MnV = (u4SrcH << 16) / (u4DstH - 1);
        if (u4MnV == 0x00010000)
        {
            u4MnV = (u4SrcH << 16) / (u4DstH);
        }
    }

    REG(fg_OP_MODE) = (UINT32) OP_STRETCH_BITBLT;
    REG(fg_SRC_WIDTH) = (u4SrcW - 1);
    REG(fg_SRC_HEIGHT) = (u4SrcH - 1);
    REG(fg_STR_DST_WIDTH) = u4DstW ;
    REG(fg_STR_DST_HEIGHT) = (u4DstH - 1);
    REG(fg_STR_BLT_H) = u4MnH;
    REG(fg_STR_BLT_V) = u4MnV;
    
    GFX_SET_BURST_READ();
    REG(fg_FIRE) = GFX_ENGINE_FIRE;


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_STR_BLT_H);
    GFX_CMDENQ(GREG_STR_BLT_V);
    GFX_CMDENQ(GREG_STR_DST_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}


//-------------------------------------------------------------------------
/** _GFX_StretchBlt_forUVBlock
 *  stretch bitblt.
 *  calculate output address.
 *  calculate input address.
 *  calculate ration.
 *  set input width height.(set src widht -1) //stretch y out from yuyv
 *  set output width height.  
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_StretchBlt_forUVBlock(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW,
        UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW,
        UINT32 u4DstH)
{
    UINT32 u4MnV, u4MnH;
    UINT32 ui4_src_addr = 0x0;
    UINT32 ui4_dst_addr = 0x0;

    GFX_ASSERT(_u4DstShift == _u4SrcShift);

    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    // calculate m n ratio for scale up and down
    if ((u4DstW == u4SrcW) || (u4DstW == 1))
    {
        u4MnH = 0x00010000;
        u4SrcW = u4DstW;
    }
    else if (u4SrcW > u4DstW) // scale down 
    {
        u4MnH = (u4SrcW << 16) / (u4DstW );
    }
    else // (u4DstW > u4SrcW)
    {
        UINT32 u4Step;

        u4Step = (u4SrcW << 16) / (u4DstW);
        if ((u4Step * (u4DstW - 1)) < ((u4SrcW - 1) << 16))
        {
            u4MnH = (u4SrcW << 16) / (u4DstW - 1);
            if (u4MnH == 0x10000)
            {
                u4MnH--;
            }
        }
        else
        {
            u4MnH = u4Step;
        }
    }

    if ((u4DstH == u4SrcH) || (u4DstH == 1))
    {
        u4MnV = 0x00010000;
        u4SrcH = u4DstH;
    }
    else
    {
        u4MnV = (u4SrcH << 16) / (u4DstH - 1);
        if (u4MnV == 0x00010000)
        {
            u4MnV = (u4SrcH << 16) / (u4DstH);
        }
    }

    REG(fg_OP_MODE) = (UINT32) OP_STRETCH_BITBLT;
    REG(fg_SRC_WIDTH) = (u4SrcW - 3);
    REG(fg_SRC_HEIGHT) = (u4SrcH - 1);
    REG(fg_STR_DST_WIDTH) = u4DstW ;
    REG(fg_STR_DST_HEIGHT) = (u4DstH - 1);
    REG(fg_STR_BLT_H) = u4MnH;
    REG(fg_STR_BLT_V) = u4MnV;
    
    GFX_SET_BURST_READ();
    REG(fg_FIRE) = GFX_ENGINE_FIRE;


    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_STR_BLT_H);
    GFX_CMDENQ(GREG_STR_BLT_V);
    GFX_CMDENQ(GREG_STR_DST_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}



//-------------------------------------------------------------------------
/** _GFX_AlphaMapBitBlt
 *  alpha map bitblt.
 *  calculate output address.
 *  calculate input address.
 *  set alpha map bitblt op mode.
 *  set color conversion flag.
 *  enque output address register.
 *  enque input address register.
 *  enque size registers.
 *  enque mode register.
 */
//-------------------------------------------------------------------------
void _GFX_AlphaMapBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    UINT32 ui4_src_addr = 0x0;
    UINT32 ui4_dst_addr = 0x0;

    GFX_ASSERT(_u4SrcCM == (UINT32)CM_RGB_CLUT8);

    GFX_ASSERT((_u4DstCM == (UINT32)CM_ARGB8888_DIRECT32) ||
            (_u4DstCM == (UINT32)CM_ARGB4444_DIRECT16) ||
            (_u4DstCM == (UINT32)CM_ARGB1555_DIRECT16));

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_OP_MODE) = (UINT32) OP_ALPHA_MAP_BITBLT;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}

//-------------------------------------------------------------------------
/** _GFX_SetRopOpt
 *  set rop option.
 *  rop bitblt.
 */
//-------------------------------------------------------------------------
void _GFX_SetRopOpt(UINT32 u4RopCode)
{
    REG(fg_ROP_OPCODE) = u4RopCode;
    REG(fg_ROP_NO_WR) = GFX_DISABLE;
    GFX_CMDENQ(GREG_ROP);
}

//-------------------------------------------------------------------------
/** _GFX_RopBitBlt
 *  raster operation.
 *  calculate input and output address.
 *  set rop op code.
 *  set width and height.
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_RopBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, UINT32 u4DstY,
        UINT32 u4Width, UINT32 u4Height)
{
    UINT32 ui4_dst_addr = 0x0;
    UINT32 ui4_src_addr = 0x0;

    GFX_ASSERT(_u4SrcCM == _u4DstCM);

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_OP_MODE) = (UINT32) OP_ROP_BITBLT;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = ((u4Height-1) & 0x007FF);
    REG(fg_SRC_HEIGHT_12) = (((u4Height -1)>>11) & 0x1);
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_0x4284);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
    REG(fg_SRC_HEIGHT_12) = 0;
}


UINT32 GFX_Get_CMSIze(UINT32 u4_val)
{
    switch(u4_val)
    {
        case CM_AYCbCr8888_DIRECT32:
        case CM_ARGB8888_DIRECT32:
            return 32;
        case CM_RGB565_DIRECT16:
        case CM_ARGB1555_DIRECT16:
        case CM_CbYCrY422_DIRECT16:
        case CM_YCbYCr422_DIRECT16:
        case CM_ARGB4444_DIRECT16:
            return 16;
        default:
                return 8;
                            
    }
}

//#ifdef CC_GFX_COMPOSITION_IMPROVE
void _GFX_ComposeLoopImprove(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar,
        UINT32 u4OpCode, UINT32 u4RectSrc)
{
    UINT32 u4CMDiff;
    UINT32 ui4_dst_addr = 0x0;
    UINT32 ui4_src_addr = 0x0;

    if (_u4SrcCM != _u4DstCM)
    {
        u4CMDiff = 1;
    }
    else
    {
        u4CMDiff = 0;
    }

    if (u4Ar == 255)
    {
        REG(fg_ALCOM_NORMAL) = GFX_ENABLE;
    }
    else
    {
        REG(fg_ALCOM_NORMAL) = GFX_DISABLE;
    }

    REG(fg_ALCOM_DIFF_CM) = u4CMDiff;

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    if(u4CMDiff)
    {
        if(((((ui4_src_addr&0xf)*8)+(u4Width*GFX_Get_CMSIze(_u4SrcCM)))>128)&&
           ((((ui4_src_addr&0xf)*8)+(u4Width*GFX_Get_CMSIze(_u4DstCM)))<=128))
        {
            REG(fg_DIFCM) =0x1;
        }

        if((((ui4_src_addr&0xf)*8)+(u4Width*GFX_Get_CMSIze(_u4SrcCM))) <= 128)
        {
            REG(fg_SRCOWN) = 0x1;
        }
        else
        {
            REG(fg_SRCOWN) = 0x0;
        }
    }
    
    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_OP_MODE) = (UINT32) OP_ALPHA_COMPOSE_LOOP;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = ((u4Height-1) & 0x007FF);
    REG(fg_SRC_HEIGHT_12) = (((u4Height -1)>>11) & 0x1);
    REG(fg_ALCOM_AR) = u4Ar;
    REG(fg_ALCOM_OPCODE) = u4OpCode;
    REG(fg_ALCOM_RECT_SRC) = u4RectSrc;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_0x4284);
    GFX_CMDENQ(GREG_0x40D0);
    GFX_CMDENQ(GREG_0x40D4);
    GFX_CMDENQ(GREG_0x40D8);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_ALCOM_LOOP);
    GFX_CMDENQ(GREG_G_MODE);
    REG(fg_DIFCM) =0x0;
    REG(fg_SRCOWN) = 0x0;
    REG(fg_SRC_HEIGHT_12) = 0;
}
//#endif

//-------------------------------------------------------------------------
/** _GFX_SetIdx2DirSrc
 *  set input region.
 *  set input base.
 *  set input color mode.
 *  set input line size.
 *  set input address shift.
 *  enque line size registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetIdx2DirSrc(UINT8 *pu1Base, UINT32 u4SrcCM)
{
    // Src_pitch can be ignored, since gfx hw reads data continuously.
    GFX_ASSERT((u4SrcCM == (UINT32)E_BMP_CM_1BIT) ||
            (u4SrcCM == (UINT32)E_BMP_CM_2BIT) ||
            (u4SrcCM == (UINT32)E_BMP_CM_4BIT) ||
            (u4SrcCM == (UINT32)E_BMP_CM_8BIT));

    _pu1Idx2DirSrcBase = pu1Base;
    _u4CharCM = u4SrcCM;
}

//-------------------------------------------------------------------------
/** _GFX_SetIdx2DirOpt
 *  set index to direct color bitblt option.
 *  set palette start address in bytes in DRAM (must 16 bytes aligned)
 *  set flag of left pixel index location in one byte of source.
 *  set flag of source line start byte aligned.
 *  enque idx2dir registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetIdx2DirOpt(UINT8 *pu1PaletteBase, UINT32 u4MsbLeft,
        UINT32 u4StartByteAligned)
{
    GFX_CHECK_PITCH((UINT32)pu1PaletteBase);

    REG(fg_PAL_BSAD) = (UINT32)(pu1PaletteBase);
    REG(fg_PAL_BSAD_MSB) = GFX_REG_ADDR_MSG((UINT32)pu1PaletteBase);
    REG(fg_MSB_LEFT) = u4MsbLeft;
    REG(fg_LN_ST_BYTE_AL) = u4StartByteAligned;

    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_IDX2DIR);
}

//-------------------------------------------------------------------------
/** _GFX_Idx2DirBitBlt
 *  index to direct color bitblt.
 *  
 */
//-------------------------------------------------------------------------
void _GFX_Idx2DirBitBlt(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width,
        UINT32 u4Height)
{
    UINT32 ui4_dst_addr = 0x0;
    UINT32 ui4_src_addr = 0x0;

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    ui4_src_addr = (UINT32)(_pu1Idx2DirSrcBase);

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_OP_MODE) = (UINT32) OP_IDX_TO_DIR_BITBLT;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_CHAR_CM) = _u4CharCM; // source color mode
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}

void _GFX_Idx2DirSrcPitchEn(UINT32 u4Enable)
{

    REG(fg_IDX2DIR_SRC_PITCH_EN) = u4Enable;
    GFX_CMDENQ(GREG_BITBLT_MODE);
}

//-------------------------------------------------------------------------
/** _GFX_2DMemCompare
 *  2D memory compare operation.
 *  calculate input and output address.
 *  set rop op code = XOR.
 *  set NO_WR = 1.
 *  set width and height.
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_2DMemCompare(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height)
{
    UINT32 ui4_dst_addr = 0x0;
    UINT32 ui4_src_addr = 0x0;

    GFX_ASSERT(_u4SrcCM == _u4DstCM);

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_OP_MODE) = (UINT32) OP_ROP_BITBLT;
    REG(fg_ROP_OPCODE) = (UINT32) E_ROP_SRC_XOR_DST;
    REG(fg_ROP_NO_WR) = GFX_ENABLE;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_ROP);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);
}

//-------------------------------------------------------------------------
/** _GFX_GetMemCompareResult
 *  get memory compare result from register
 *  if no error, return 0
 */
//-------------------------------------------------------------------------
INT32 _GFX_GetMemCompareResult(void)
{
    return GFX_HwGetMemCompareResult();
}

//-------------------------------------------------------------------------
/** _GFX_SetHoriToVertLineOpt
 *  Set DSTPITCH_DEC 0 or 1 to determine whether the bitmap
 *  rotated by 90" clockwise or Counterclockwise.
 */
//-------------------------------------------------------------------------
void _GFX_SetHoriToVertLineOpt(UINT32 u4Is90dCcw)
{
    REG(fg_DSTPITCH_DEC) = u4Is90dCcw;
    GFX_CMDENQ(GREG_BITBLT_MODE);
}

//-------------------------------------------------------------------------
/** _GFX_HoriToVertLine
 *  gfx horizontal line to vertical line operation.
 *  calculate output address.
 *  calculate input address.
 *  enque output address register.
 *  enque input address register.
 *  enque width and height registers.
 *  enque str_width and str_height registers.
 *  enque mode register.
 */
//-------------------------------------------------------------------------
void _GFX_HoriToVertLine(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4HoriLineWidth)
{
    UINT32 ui4_dst_addr = 0x0;
    UINT32 ui4_src_addr = 0x0;

    GFX_ASSERT(u4HoriLineWidth != 0);

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));
    //source base address
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_OP_MODE) = (UINT32) OP_LINE_HORI_TO_VERT;
    REG(fg_SRC_WIDTH) = u4HoriLineWidth;
    REG(fg_SRC_HEIGHT) = 0;
    REG(fg_STR_DST_WIDTH) = 1;
    REG(fg_STR_DST_HEIGHT) = (u4HoriLineWidth - 1);
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_STR_DST_SIZE);
    GFX_CMDENQ(GREG_G_MODE);
}

void _GFX_SetCharCM(BOOL flag, UINT32 u4SrcCM)
{
    GFX_ASSERT((u4SrcCM == (UINT32)E_BMP_CM_1BIT) ||
            (u4SrcCM == (UINT32)E_BMP_CM_2BIT) ||
            (u4SrcCM == (UINT32)E_BMP_CM_4BIT) ||
            (u4SrcCM == (UINT32)E_BMP_CM_8BIT));

    if (flag)
    {
        _u4CharCM = u4SrcCM;
        REG(fg_CHAR_CM) = _u4CharCM;
    }
    else
    {
        _u4ThirdCharCM = u4SrcCM;
        REG(fg_THIRD_CHAR_CM) = _u4ThirdCharCM;
    }

}
//-------------------------------------------------------------------------
/** _GFX_SetThirdPalle
 *  set index to direct color bitblt option.
 *  set palette start address in bytes in DRAM (must 16 bytes aligned)
 *  set flag of left pixel index location in one byte of source.
 *  set flag of source line start byte aligned.
 *  enque idx2dir registers.
 */
//-------------------------------------------------------------------------
void _GFX_SetThirdPallete(UINT8 *pu1PaletteBase, UINT32 u4MsbLeft,
        UINT32 u4StartByteAligned)
{
    GFX_CHECK_PITCH((UINT32)pu1PaletteBase);

    REG(fg_THIRD_PAL_BSAD) = (UINT32)(pu1PaletteBase);
    REG(fg_THIRD_PAL_BSAD_MSB) = GFX_REG_ADDR_MSG((UINT32)pu1PaletteBase);
    REG(fg_THIRD_MSB_LEFT) = u4MsbLeft;
    REG(fg_THIRD_LN_ST_BYTE_AL) = u4StartByteAligned;

    GFX_CMDENQ(GREG_0x40D4);
}

void _GFX_SetCompressionOpt(UINT32 u4LineSeparate, UINT32 u4QualityMode,
        UINT32 u4RollBackEn)
{

    REG(fg_ROLL_BACK_EN) = u4RollBackEn;
    REG(fg_QUALITY_MODE) = u4QualityMode;
    REG(fg_LINE_SEPARATE) = u4LineSeparate;

    GFX_CMDENQ(GREG_BPCOMP_CFG);
}

//-------------------------------------------------------------------------
/** _GFX_GeneralAlphaCom
 *  New compression bitblt.
 *  
 */
//-------------------------------------------------------------------------


void _GFX_GeneralAlphaCom(UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar,
        UINT32 u4OpCode, UINT32 u4RectSrc, UINT32 u4ThirdEnable,
        BOOL fgAlcomNormal, BOOL fgOnePassCompress)
{
    UINT32 u4CMDiff;
    UINT32 ui4_dst_addr = 0x0;
    UINT32 ui4_src_addr = 0x0;

    if (u4ThirdEnable == FALSE)
    {
        if (_u4SrcCM != _u4DstCM)
            u4CMDiff = 1;
        else
            u4CMDiff = 0;
    }
    else
    {
        if (_u4SrcCM == _u4ThirdCM && _u4ThirdCM == _u4DstCM)
            u4CMDiff = 0;
        else
            u4CMDiff = 1;
    }

    if(!(GFX_GetGoldenMode()&0x1))
    {
        if (u4Ar == 255)
        {
            fgAlcomNormal = 1;
        }
        else
        {
            fgAlcomNormal = 0;
        }
    }

    REG(fg_APP_BPCOMP) = fgOnePassCompress;
    REG(fg_ALCOM_NORMAL) = fgAlcomNormal;
    REG(fg_THIRD_BUF_EN) = u4ThirdEnable;
    REG(fg_ALCOM_DIFF_CM) = u4CMDiff;

    //destination base address

    ui4_dst_addr = (UINT32)(_pu1DstBase);
    //source base address
    ui4_src_addr = (UINT32)(_pu1SrcBase);

    REG(fg_THIRD_BSAD) = (UINT32)(_pu1ThirdBase);

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_THIRD_BSAD_MSB) = ((UINT32)(_pu1ThirdBase)) >> 30;

    REG(fg_OP_MODE) = (UINT32) OP_ALPHA_COMPOSE_LOOP;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = ((u4Height-1) & 0x007FF);
    REG(fg_SRC_HEIGHT_12) = (((u4Height -1)>>11) & 0x1);
    REG(fg_ALCOM_AR) = u4Ar;
    REG(fg_ALCOM_OPCODE) = u4OpCode;
    REG(fg_ALCOM_RECT_SRC) = u4RectSrc;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_0x4284);
    GFX_CMDENQ(GREG_0x40D0);
    GFX_CMDENQ(GREG_0x40D4);
    GFX_CMDENQ(GREG_0x40D8);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_ALCOM_LOOP);
    GFX_CMDENQ(GREG_G_MODE);
    REG(fg_SRC_HEIGHT_12) = 0;
}

void _VGFX_GeneralAlphaComSet(UINT32 u4ImgCm, UINT32 u4WbCm, UINT32 u4Ar,
        UINT32 u4OpCode, BOOL fgAlcomNormal, UINT32 u4PremultSrcR,
        UINT32 u4PremultDstR, UINT32 u4PremultDstW, UINT32 u4PremultOvrflw)
{
    UINT32 u4CMDiff;

    if (u4WbCm == u4ImgCm)
        u4CMDiff = 0;
    else
        u4CMDiff = 1;

    REG(fg_ALCOM_NORMAL) = fgAlcomNormal;
    REG(fg_ALCOM_DIFF_CM) = u4CMDiff;
    REG(fg_ALCOM_AR) = u4Ar;
    REG(fg_ALCOM_OPCODE) = u4OpCode;
    REG(fg_PREMULT_SRC_R) = u4PremultSrcR;
    REG(fg_PREMULT_DST_R) = u4PremultDstR;
    REG(fg_PREMULT_DST_W) = u4PremultDstW;
    REG(fg_PREMULT_OVERFLOW_EN) = u4PremultOvrflw;

    GFX_CMDENQ(GREG_ALCOM_LOOP);
    // GFX_CMDENQ(GREG_G_MODE);   //only fire
}

void _VGFX_Enable(void)
{
    REG(fg_OP_MODE) = (UINT32) OP_SCALER;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;
    GFX_CMDENQ(GREG_G_MODE);

}

//-------------------------------------------------------------------------
/** _GFX_SetPremult
 *  Indicate Src/Dst Premult R/W 
 *  
 */
//-------------------------------------------------------------------------
void _GFX_SetPremult(UINT32 u4PremultSrcR, UINT32 u4PremultDstR,
        UINT32 u4PremultDstW, UINT32 u4PremultOvrflw)
{

    REG(fg_PREMULT_SRC_R) = u4PremultSrcR;
    REG(fg_PREMULT_DST_R) = u4PremultDstR;
    REG(fg_PREMULT_DST_W) = u4PremultDstW;
    REG(fg_PREMULT_OVERFLOW_EN) = u4PremultOvrflw;

}

//-------------------------------------------------------------------------
/** _GFX_StretchAlphaCom
 *  New compression bitblt.
 *  
 */
//-------------------------------------------------------------------------
void _GFX_StretchAlphaCom(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW,
        UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW,
        UINT32 u4DstH, UINT32 u4Ar, UINT32 u4OpCode)
{
    UINT32 u4CMDiff;
    UINT32 u4MnV, u4MnH;
    BOOL fgAlcomNormal  = 0x0;
    UINT32 ui4_dst_addr = 0x0;
    UINT32 ui4_src_addr = 0x0;

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    GFX_ASSERT(_u4DstShift == _u4SrcShift);

    if (_u4SrcCM != _u4DstCM)
    {
        u4CMDiff = 1;
    }
    else
    {
        u4CMDiff = 0;
    }

    // calculate m n ratio for scale up and down
    if ((u4DstW == u4SrcW) || (u4DstW == 1))
    {
        u4MnH = 0x00010000;
        u4SrcW = u4DstW;
    }
    else if (u4SrcW > u4DstW) // scale down
    {
        u4MnH = (u4SrcW << 16) / (u4DstW - 1);
        if (((u4SrcW << 16) % (u4DstW - 1)) == 0)
        {
            u4MnH--;
        }
    }
    else // (u4DstW > u4SrcW)
    {
        UINT32 u4Step;

        u4Step = (u4SrcW << 16) / (u4DstW);
        if ((u4Step * (u4DstW - 1)) < ((u4SrcW - 1) << 16))
        {
            u4MnH = (u4SrcW << 16) / (u4DstW - 1);
            if (u4MnH == 0x10000)
            {
                u4MnH--;
            }
        }
        else
        {
            u4MnH = u4Step;
        }
    }

    if ((u4DstH == u4SrcH) || (u4DstH == 1))
    {
        u4MnV = 0x00010000;
        u4SrcH = u4DstH;
    }
    else
    {
        u4MnV = (u4SrcH << 16) / (u4DstH - 1);
        if (u4MnV == 0x00010000)
        {
            u4MnV = (u4SrcH << 16) / (u4DstH);
        }
    }

    if(!(GFX_GetGoldenMode()&0x1))
    {
        if(u4Ar == 255)
        {
            fgAlcomNormal = 1;
        }
        else
        {
            fgAlcomNormal = 0;
        }
    }

    REG(fg_ALCOM_NORMAL) = fgAlcomNormal;
    REG(fg_ALCOM_DIFF_CM) = u4CMDiff;

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4DstY * _u4DstPitch) + (u4DstX << _u4DstShift));

    //source base address
    ui4_src_addr = (UINT32)(
            _pu1SrcBase + (u4SrcY * _u4SrcPitch) + (u4SrcX << _u4SrcShift));

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);
    REG(fg_OP_MODE) = (UINT32) OP_STRETCH_ALCOM;
    REG(fg_ALCOM_AR) = u4Ar;
    REG(fg_ALCOM_OPCODE) = u4OpCode;
    REG(fg_ALCOM_RECT_SRC) = 0x0;
    REG(fg_SRC_WIDTH) = u4SrcW;
    REG(fg_SRC_HEIGHT) = (u4SrcH - 1);
    REG(fg_STR_DST_WIDTH) = u4DstW;
    REG(fg_STR_DST_HEIGHT) = (u4DstH - 1);
    REG(fg_STR_BLT_H) = u4MnH;
    REG(fg_STR_BLT_V) = u4MnV;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_STR_BLT_H);
    GFX_CMDENQ(GREG_STR_BLT_V);
    GFX_CMDENQ(GREG_STR_DST_SIZE);
    GFX_CMDENQ(GREG_ALCOM_LOOP);
    GFX_CMDENQ(GREG_G_MODE);

}

//-------------------------------------------------------------------------
/** _GFX_NewCompressBlt
 *  New compression bitblt.
 *  
 */
//-------------------------------------------------------------------------
void _GFX_NewCompressBlt(UINT8 *pu1SrcBase, UINT32 u4SrcX, UINT32 u4SrcY,
        UINT32 u4SrcCM, UINT32 u4SrcPitch, UINT8 *pu1DstBase, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4BufHeight, UINT32 u4RollBackEn,
        UINT32 u4QualityMode, UINT32 u4BpCompAddrEnd, UINT32 u4_blt_fg,
        BOOL fgBurstRead, BOOL fgLineSeparate, UINT32 u4DstPitch)
{
    //UINT32 u4DstPitch;
    UINT32 ui4_src_addr = 0x0;
    UINT32 ui4_dst_addr = 0x0;

    //GFX_CHK_32B_ALIGN((UINT32)pu1SrcBase);
    //GFX_CHK_32B_ALIGN((UINT32)pu1DstBase);
    GFX_CHK_8B_ALIGN((UINT32)pu1SrcBase);
    GFX_CHK_8B_ALIGN((UINT32)pu1DstBase);

    GFX_ASSERT(u4SrcCM == (UINT32)CM_ARGB8888_DIRECT32 || u4SrcCM == (UINT32)CM_ARGB4444_DIRECT16 ||
            u4SrcCM == (UINT32)CM_AYCbCr8888_DIRECT32 || u4SrcCM == (UINT32)CM_YCbCr_CLUT8 ||
            u4SrcCM == (UINT32)CM_RGB_CLUT8 || u4SrcCM== (UINT32)CM_RGB565_DIRECT16 || u4SrcCM==CM_ARGB1555_DIRECT16);

    GFX_ASSERT((u4Width % 8) == 0);

    if( (u4_blt_fg &(D_GFXFLAG_MIRROR)) && (u4_blt_fg &(D_GFXFLAG_FLIP)) )
    {
        REG(fg_SRCPITCH_DEC) = (UINT32) 1;
        REG(fg_SRCMIRROR) = (UINT32) 1;
        ui4_src_addr = (UINT32)(pu1SrcBase +
                ((u4SrcY + u4Height - 1) * u4SrcPitch) +
                ((u4SrcX + u4Width - 1) << _auColorModeShift[u4SrcCM]));
        ui4_dst_addr = (UINT32)(pu1DstBase +
                ((u4BufHeight - u4SrcY - u4Height) * u4DstPitch) +
                (u4SrcX << _auColorModeShift[u4SrcCM]));
    }
    else if (u4_blt_fg &(D_GFXFLAG_MIRROR))
    {
        REG(fg_SRCMIRROR) = (UINT32) 1;
        ui4_src_addr = (UINT32)(pu1SrcBase +
                (u4SrcY * u4SrcPitch) +
                ((u4SrcX + u4Width - 1) << _auColorModeShift[u4SrcCM]));
        ui4_dst_addr = (UINT32)(pu1DstBase +
                (u4SrcY * u4DstPitch) + (u4SrcX << _auColorModeShift[u4SrcCM]));
    }
    else if (u4_blt_fg &(D_GFXFLAG_FLIP))
    {
        REG(fg_SRCPITCH_DEC) = (UINT32) 1;
        ui4_src_addr = (UINT32)(pu1SrcBase +
                ((u4SrcY + u4Height - 1) * u4SrcPitch) +
                (u4SrcX << _auColorModeShift[u4SrcCM]));
        ui4_dst_addr = (UINT32)(pu1DstBase +
                ((u4BufHeight - u4SrcY - u4Height) * u4DstPitch) +
                (u4SrcX << _auColorModeShift[u4SrcCM]));
    }
    else
    {
        REG(fg_SRCPITCH_DEC) = (UINT32) 0;
        REG(fg_SRCMIRROR) = (UINT32) 0;
        ui4_src_addr = (UINT32)(
                pu1SrcBase + (u4SrcY * u4SrcPitch) + (u4SrcX
                        << _auColorModeShift[u4SrcCM]));
        ui4_dst_addr = (UINT32)(
                pu1DstBase + (u4SrcY * u4DstPitch) + (u4SrcX
                        << _auColorModeShift[u4SrcCM]));
    }

    REG(fg_SRC_BSAD) = ui4_src_addr;
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_src_addr);
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    REG(fg_SRC_CM) = u4SrcCM;
    REG(fg_SRC_PITCH) = (u4SrcPitch >> 4);

    REG(fg_CM) = u4SrcCM;
    REG(fg_OSD_WIDTH) = (u4DstPitch >> 4);

    REG(fg_OP_MODE) = (UINT32) OP_NEW_COMPRESS_BITBLT;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = ((u4Height-1) & 0x007FF);
    REG(fg_SRC_HEIGHT_12) = (((u4Height -1)>>11) & 0x1);
    //REG(fg_A_METHOD)   = u4AlphaMethod;
    REG(fg_LINE_SEPARATE) = (UINT32) fgLineSeparate;
    REG(fg_ROLL_BACK_EN) = u4RollBackEn;
    REG(fg_QUALITY_MODE) = u4QualityMode;
    REG(fg_BPCOMP_AD_END) = u4BpCompAddrEnd;

    REG(fg_FIRE) = GFX_ENGINE_FIRE;
    
    GFX_SET_BURST_READ();
    
    GFX_CMDENQ(GREG_0x4284);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_S_OSD_WIDTH);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BPCOMP_CFG);
    GFX_CMDENQ(GREG_BPCOMP_AD_END);
    GFX_CMDENQ(GREG_G_MODE);

    REG(fg_APP_BPCOMP) = 0;
    REG(fg_DSTPITCH_DEC) = 0;
    REG(fg_SRCPITCH_DEC) = (UINT32) 0;
    REG(fg_SRCMIRROR) = (UINT32) 0;
    GFX_CMDENQ(GREG_BITBLT_MODE);
    REG(fg_SRC_HEIGHT_12) = 0;
}

// normal driver
//-------------------------------------------------------------------------
/** _GFX_SetGradOpt
 *  gfx set gradient options.
 *  x increment pixels.
 *  y increment pixels.
 *  x delta in color components (a, r, g, b), 7.1 format.
 *  y delta in color components.
 */
//-------------------------------------------------------------------------
void _GFX_SetGradOpt(UINT32 u4IncX, UINT32 u4IncY, const UINT32 asu4DeltaX[4],
        const UINT32 asu4DeltaY[4])
{
    GFX_ASSERT(asu4DeltaX != NULL);
    GFX_ASSERT(asu4DeltaY != NULL);

    REG(fg_GRAD_X_PIX_INC) = u4IncX;
    REG(fg_GRAD_Y_PIX_INC) = u4IncY;

    REG(fg_DELTA_X_C0) = asu4DeltaX[0] & 0xff;
    REG(fg_DELTA_X_C1) = asu4DeltaX[1] & 0xff;
    REG(fg_DELTA_X_C2) = asu4DeltaX[2] & 0xff;
    REG(fg_DELTA_X_C3) = asu4DeltaX[3] & 0xff;

    REG(fg_DELTA_Y_C0) = asu4DeltaY[0] & 0xff;
    REG(fg_DELTA_Y_C1) = asu4DeltaY[1] & 0xff;
    REG(fg_DELTA_Y_C2) = asu4DeltaY[2] & 0xff;
    REG(fg_DELTA_Y_C3) = asu4DeltaY[3] & 0xff;

    GFX_CMDENQ(GREG_GRAD_X_DELTA);
    GFX_CMDENQ(GREG_GRAD_Y_DELTA);
}

//-------------------------------------------------------------------------
/** _GFX_GradFill
 *  gfx gradient rectangle fill.
 *  calculate output address.
 *  set gradient fill op mode.
 *  set width and height.
 *  enque registers.
 */
//-------------------------------------------------------------------------
void _GFX_GradFill(UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4Mode)
{
    UINT32 ui4_dst_addr = 0x0;

    //destination base address
    ui4_dst_addr = (UINT32)(
            _pu1DstBase + (u4Y * _u4DstPitch) + (u4X << _u4DstShift));
    REG(fg_DST_BSAD) = ui4_dst_addr;
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG(ui4_dst_addr);

    REG(fg_OP_MODE) = (UINT32) OP_GRAD_FILL;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = (u4Height - 1);
    REG(fg_GRAD_MODE) = u4Mode;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_GRAD_XY_INC);
    GFX_CMDENQ(GREG_G_MODE);
}

//-------------------------------------------------------------------------
/** _GFX_DMA
 *  gfx dram to dram dma function.
 *  32bit align.
 *  set output address.
 *  set input address.
 *  set number of 32bit words.
 */
//-------------------------------------------------------------------------
void _GFX_DMA(UINT32 *pu4Dst, UINT32 *pu4Src, UINT32 u4NumWrd)
{
    UINT32 u4Width, u4Height;

    GFX_SetBltOpt(D_GFXFLAG_NONE, 0x00000000, 0xffffffff);
    u4Width = u4NumWrd & 0x7fff;
    u4Height = u4NumWrd >> 15;

    REG(fg_SRC_BSAD) = (UINT32)(pu4Src);
    REG(fg_SRC_BSAD_MSB) = GFX_REG_ADDR_MSG((UINT32)(pu4Src));
    REG(fg_DST_BSAD) = (UINT32)(pu4Dst);
    REG(fg_DST_BSAD_MSB) = GFX_REG_ADDR_MSG((UINT32)(pu4Dst));

    REG(fg_OP_MODE) = (UINT32) OP_DMA;
    REG(fg_CFMT_ENA) = GFX_DISABLE;
    REG(fg_SRC_WIDTH) = u4Width;
    REG(fg_SRC_HEIGHT) = u4Height;
    REG(fg_CM) = (UINT32) CM_ARGB8888_DIRECT32;
    REG(fg_FIRE) = GFX_ENGINE_FIRE;

    GFX_SET_BURST_READ();

    GFX_CMDENQ(GREG_DST_BSAD);
    GFX_CMDENQ(GREG_SRC_BSAD);
    GFX_CMDENQ(GREG_ADDR_MSB1);
    GFX_CMDENQ(GREG_SRC_SIZE);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);

    // restore dst color mode
    REG(fg_CM) = _u4DstCM;
}


void _GFX_MMU_Init(void)
{
    _GFX_MMU_Enable(GFX_MMU_ENABLE);
    _GFX_MMU_Set_Agent0(GFX_MMU_AGENT_EN, GFX_MMU_AGENT_MID0);
    _GFX_MMU_Set_Agent1(GFX_MMU_AGENT_EN, GFX_MMU_AGENT_MID4);
    _GFX_MMU_Set_Agent2(GFX_MMU_AGENT_EN, GFX_MMU_AGENT_MID1);
    _GFX_MMU_Irq_En(GFX_MMU_IRQ_ENABLE, GFX_MMU_IRQ_MASK);
    _GFX_MMU_Clear_INT();
    _GFX_MMU_Reset_INT();
    // _GFX_MMU_SelfFire();
}


void _GFX_MMU_Enable(UINT32 u4Enable)
{
    REG_MMU(fg_MMU_EN) = u4Enable;
    REG_MMU(fg_MMU_CFG0_DEFAULT) = 0x7F; //reset  value
    GFX_MMU_CMDENQ(GREG_MMU_CFG0);
}

void _GFX_MMU_SelfFire(void)
{
    REG_MMU(fg_MMU_SELF_FIRE) = GFX_MMU_SELF_FILE;
    REG_MMU(fg_FIRE_SEL)      = 0x0;
    GFX_MMU_CMDENQ(GREG_MMU_CFG4);
}

void _GFX_MMU_Set_TabAddr(UINT32 u4TabAddress)
{
    REG_MMU(fg_MMU_TTB) = u4TabAddress;
    GFX_MMU_CMDENQ(GREG_MMU_CFG1);
}

unsigned int gfx_va_to_pa(unsigned int u4_va) 
{
    UINT32  u4_pa               = 0x0;
    UINT32  u4_pgt              = 0x0;
    UINT32* pu4_1_tab_addr      = NULL;
    UINT32 u4_1_tab_val         = 0x0;
    UINT32* pu4_2_tab_addr      = NULL;
    UINT32  u4_2_tab_val        = 0x0;
    
    #ifdef LINUX_TURNKEY_SOLUTION
    u4_pgt          = HalCpuGetPhyPgd();
    #else
    u4_pgt          = HalGetMMUTableAddress();
    #endif
    
    pu4_1_tab_addr  = (UINT32*)VIRTUAL(GFX_FIR_TBL_ADDR(u4_pgt,u4_va));
    u4_1_tab_val    = *pu4_1_tab_addr;
    pu4_2_tab_addr  = (UINT32*)VIRTUAL(GFX_SEC_TBL_ADDR(u4_1_tab_val,u4_va));
    u4_2_tab_val    = *pu4_2_tab_addr;
    u4_pa           = GFX_VA_2_PA(u4_2_tab_val,u4_va);
    
    if((0x0 == (u4_1_tab_val & 0x1))||(0x0 == (u4_2_tab_val & 0x3)))
    {
        GFX_PRINT("gfx_va_to_pa,pgt[0x%x],1[0x%x,0x%x],2[0x%x,0x%x],pg[0x%x,0x%x],vapa[0x%x,0x%x]\n",
                u4_pgt,
                (UINT32)PHYSICAL((UINT32)pu4_1_tab_addr),
                u4_1_tab_val,
                (UINT32)PHYSICAL((UINT32)pu4_2_tab_addr),
                u4_2_tab_val,
                (u4_1_tab_val & 0x1),
                (u4_2_tab_val & 0x3),
                u4_va,
                u4_pa);
    }
        
    return u4_pa;
}



void gfx_print_va_pa(UINT32 u4_start,UINT32 u4_size)
{
    #ifdef LINUX_TURNKEY_SOLUTION
    int             u4_tmp = 0x0;
    unsigned int    u4_va = 0x0;
    unsigned int    u4_pa = 0x0;
    int             u4_pgs = 0x0;

    if((!u4_start)||(!u4_size))
    {
        return;
    }
    
    u4_pgs = ((((u4_start + u4_size) & PAGE_MASK) >> PAGE_SHIFT)-((u4_start & PAGE_MASK) >> PAGE_SHIFT)+1);
    GFX_PRINT("gfx_print_va_pa[0x%x,0x%x,%d]\n",u4_start,u4_size,u4_pgs);
    
    while(u4_tmp < u4_pgs)
    {
        u4_va = (u4_start + u4_tmp*PAGE_SIZE);
        u4_pa = gfx_va_to_pa(u4_va);
        u4_tmp++;
    }
    #endif
}

void _GFX_MMU_Set_Cfg(GFX_MMU_T* pt_gfx_mmu)
{
    do
    {        
        if(!pt_gfx_mmu->u4_pgt)
        {
            #ifdef LINUX_TURNKEY_SOLUTION
            pt_gfx_mmu->u4_pgt = HalCpuGetPhyPgd();
            #else
            pt_gfx_mmu->u4_pgt = HalGetMMUTableAddress();
            #endif
        }
        
        if(GFX_MMU_SAME_CFG(pt_gfx_mmu))
        {
            if(!pt_gfx_mmu->u4_enable)
            {
                return;
            }
        }
        
        GFX_MMU_SAVE_CFG(pt_gfx_mmu);
       
        if (pt_gfx_mmu->u4_init)
        {
            _GFX_MMU_Set_Enable(pt_gfx_mmu->u4_enable);
        }

        if(!pt_gfx_mmu->u4_enable)
        {
            break;
        }
        
        if (pt_gfx_mmu->u4_src_rw_mmu)
        {
            if (OP_NEW_COMPRESS_BITBLT == pt_gfx_mmu->u4_op_md)
            {
                _GFX_MMU_Set_Agent0(GFX_MMU_AGENT_EN, GFX_MMU_AGENT_MID3);
            }
            else if(OP_CBCRBLOCK_TO_2CBCRRASTER == pt_gfx_mmu->u4_op_md)
            {
                _GFX_MMU_Set_Agent0(GFX_MMU_AGENT_EN, GFX_MMU_AGENT_MID6);
            }
            else
            {
                _GFX_MMU_Set_Agent0(GFX_MMU_AGENT_EN, GFX_MMU_AGENT_MID0);
            }
        }
        else
        {
            _GFX_MMU_Set_Agent0(GFX_MMU_AGENT_DISABLE, GFX_MMU_AGENT_MID0);
        }

        if(GFX_MMU_DST_RW == pt_gfx_mmu->u4_dst_rw_mmu)
        {
            _GFX_MMU_Set_Agent1(GFX_MMU_AGENT_EN, GFX_MMU_AGENT_MID4);
            _GFX_MMU_Set_Agent2(GFX_MMU_AGENT_EN, GFX_MMU_AGENT_MID1);
        }
        else if(GFX_MMU_DST_R == pt_gfx_mmu->u4_dst_rw_mmu)
        {
            _GFX_MMU_Set_Agent1(GFX_MMU_AGENT_EN, GFX_MMU_AGENT_MID4);
            _GFX_MMU_Set_Agent2(GFX_MMU_AGENT_DISABLE, GFX_MMU_AGENT_MID1);
        }
        else if(GFX_MMU_DST_W == pt_gfx_mmu->u4_dst_rw_mmu)
        {
            _GFX_MMU_Set_Agent1(GFX_MMU_AGENT_DISABLE, GFX_MMU_AGENT_MID4);
            _GFX_MMU_Set_Agent2(GFX_MMU_AGENT_EN, GFX_MMU_AGENT_MID1);
        }
        else
        {
            _GFX_MMU_Set_Agent1(GFX_MMU_AGENT_DISABLE, GFX_MMU_AGENT_MID4);
            _GFX_MMU_Set_Agent2(GFX_MMU_AGENT_DISABLE, GFX_MMU_AGENT_MID1);
        }
        
        _GFX_MMU_Set_TabAddr(pt_gfx_mmu->u4_pgt);
    }while(0);
    
    _GFX_MMU_SelfFire();

    if(0) //(!DTV_VFY)
    {
        if(GFX_TASK_IS_INITED&GFX_TASK_SUPPORT_MMU)
        {
            GFX_CmdQ_Action_Async(E_GFX_TASK_CMDQ_MMU);
        }
        else
        {
            GFX_CmdQueAction();
            GFX_Wait();
        }
    }

    return;
}


UINT32 _GFX_MMU_Get_Enable(void)
{
    return GFX_MMU_ENABLE_FG;
}


static void _GFX_MMU_Set_Enable(UINT32 u4_enable)
{
    if (u4_enable)
    {
        _GFX_MMU_Init();
    }
    else
    {
        _GFX_MMU_Set_Agent0(GFX_MMU_AGENT_DISABLE, GFX_MMU_AGENT_MID0);
        _GFX_MMU_Set_Agent1(GFX_MMU_AGENT_DISABLE, GFX_MMU_AGENT_MID0);
        _GFX_MMU_Set_Agent2(GFX_MMU_AGENT_DISABLE, GFX_MMU_AGENT_MID0);
        _GFX_MMU_Enable(GFX_MMU_DISABLE);
        _GFX_MMU_SelfFire();
    }

    return;
}

/********************************
 Agent0: Src Read

 A0_MID = 0x0 (non-compression)
 A0_MID = 0x3 (compression)

 *********************************/

void _GFX_MMU_Set_Agent0(UINT32 u4Enable, UINT32 u4Mid)
{
    REG_MMU(fg_MMU_A0_EN) = u4Enable;
    REG_MMU(fg_MMU_A0_MID) = u4Mid;
    REG_MMU(fg_MMU_A0_PREFETCH) = GFX_MMU_PREFETCH_EN;
    REG_MMU(fg_MMU_A0_2D_PREFETCH) = GFX_MMU_2RD_PREFETCH_EN;
    GFX_MMU_CMDENQ(GREG_MMU_CFG2);
}

/********************************
 Agent0: Dst Read

 A1_MID = 0x4

 *********************************/

void _GFX_MMU_Set_Agent1(UINT32 u4Enable, UINT32 u4Mid)
{
    REG_MMU(fg_MMU_A1_EN) = u4Enable;
    REG_MMU(fg_MMU_A1_MID) = u4Mid;
    REG_MMU(fg_MMU_A1_PREFETCH) = GFX_MMU_PREFETCH_EN;
    REG_MMU(fg_MMU_A1_2D_PREFETCH) = GFX_MMU_2RD_PREFETCH_EN;
    GFX_MMU_CMDENQ(GREG_MMU_CFG2);
}

/********************************
 Agent0: Dst Write

 A2_MID = 0x1

 *********************************/

void _GFX_MMU_Set_Agent2(UINT32 u4Enable, UINT32 u4Mid)
{
    REG_MMU(fg_MMU_A2_EN) = u4Enable;
    REG_MMU(fg_MMU_A2_MID) = u4Mid;
    REG_MMU(fg_MMU_A2_PREFETCH) = GFX_MMU_PREFETCH_EN;
    REG_MMU(fg_MMU_A2_2D_PREFETCH) = GFX_MMU_2RD_PREFETCH_EN;
    GFX_MMU_CMDENQ(GREG_MMU_CFG3);
}

void _GFX_MMU_Irq_En(UINT32 u4Enable, UINT32 u4IrqMode)
{
    REG_MMU(fg_MMU_INT_EN0)         = u4Enable; // 1M table error
    REG_MMU(fg_MMU_INT_EN1)         = u4Enable; // 4K table error
    REG_MMU(fg_MMU_INT_MASK)        = u4IrqMode;
    
    GFX_MMU_CMDENQ(GREG_MMU_CFG4);
}

void _GFX_MMU_Clear_INT(void)
{
    REG_MMU(fg_MMU_INT_CLR0) = 1; // 1M table error
    REG_MMU(fg_MMU_INT_CLR1) = 1; // 4K table error
    GFX_MMU_CMDENQ(GREG_MMU_CFG4);
}

void _GFX_MMU_Reset_INT(void)
{
    REG_MMU(fg_MMU_INT_CLR0) = 0; // 1M table error
    REG_MMU(fg_MMU_INT_CLR1) = 0; // 4K table error
    GFX_MMU_CMDENQ(GREG_MMU_CFG4);
}

void _GFX_MMU_Set_OverRead(UINT32 u4Enable, UINT32 u4Id, UINT32 u4Addr)
{
    REG_MMU(fg_MMU_OVERREAD_ADD0_EN) = u4Enable;
    REG_MMU(fg_MMU_OVERREAD_EN0) = u4Enable;
    REG_MMU(fg_MMU_OVERREAD_ID0) = u4Id;
    REG_MMU(fg_MMU_OVERREAD_ADD0) = u4Addr;
    GFX_MMU_CMDENQ(GREG_MMU_CFGB);
    GFX_MMU_CMDENQ(GREG_MMU_CFG9);
}

static void GFX_MMU_Enable_CmdQ(UINT32 u4_enable)
{
    if (u4_enable)
    {
        GFX_WRITE32(GFX_REG_G_CONFIG, 0xaac0031f);
    }
    else
    {
        GFX_WRITE32(GFX_REG_G_CONFIG, 0xaa40031f);
    }
    
    return;
}

static void GFX_Enable_CmdQ(UINT32 u4_enable)
{
    UINT32 u4Value = 0x0;

    u4Value = GFX_READ32(GFX_REG_G_CONFIG);

    if (u4_enable)
    {
        u4Value = (u4Value) | (GREG_MSK_EN_DRAMQ);
    }
    else
    {
        u4Value &= (~GREG_MSK_EN_DRAMQ);
    }

    GFX_WRITE32(GFX_REG_G_CONFIG, u4Value);

    return;
}

UINT32 GFX_MMU_Get_Status(void)
{
    UINT32 u4Value = 0x0;

    GFX_MMU_Enable_CmdQ(FALSE);
    u4Value = GFX_MMU_READ32(GFX_REG_MMU_RST);
    GFX_MMU_Enable_CmdQ(TRUE);

    return u4Value;
}

void GFX_MMU_Check_Status(void)
{
    UINT32 u4Value = 0x0;
    UINT32 u4_cnt  = 0x0;

    do
    {
        u4Value = GFX_MMU_Get_Status();
        if(u4_cnt++ > 0x10)
        {
            GFX_PRINT("gfx mmu warning:\n");
            break;
        }
    }while((u4Value&GREG_MSK_MMU_CFG4_ERROR)||(u4Value==_r_gfx_mmu.u4_pgt));

    if(u4Value & GREG_MSK_MMU_PGT_ERROR)
    {
        if(u4Value & GREG_MSK_MMU_CFG4_ERROR)
        {
            GFX_PRINT("gfx mmu warning:[0x%x,0x%x],op[0x%x]\n",u4Value,
                    GFX_MMU_Get_Status(),GFX_HwGetRegOpMode());
            return;
        }

        GFX_PRINT("gfx mmu error:[0x%x,0x%x],op[0x%x]\n",u4Value,
                GFX_MMU_Get_Status(),GFX_HwGetRegOpMode());
        
        while(GFX_DBG_LVL(GFX_DBG_MMU_ERR))
        {
            GFX_PRINT("gfx mmu error[0x%x]\n",GFX_MMU_Get_Status());
        }
    }

    return;
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** GFX_MemCompare
 *  do memory compare.
 *  use GFX_2DMemCompare() to complete this function.
 *  (high-level API)
 */
//-------------------------------------------------------------------------
INT32 GFX_MemCompare(UINT8 *pu1DstBase, UINT8 *pu1SrcBase, UINT32 u4Size)
{
    UINT32 u4Pitch = 16384; // WIDTH[14:0]
    UINT32 i, u4Loops, u4LastSize;
    UINT8 *pu1SrcAddr, *pu1DstAddr;

    GFX_ASSERT(u4Size != 0);

    u4Loops = u4Size / u4Pitch;
    u4LastSize = u4Size % u4Pitch;

    // lock gfx-op
    GFX_Lock();

    for (i = 0; i < u4Loops; i++)
    {
        pu1SrcAddr = (UINT8 *) ((UINT32) pu1SrcBase + (i * u4Pitch));
        pu1DstAddr = (UINT8 *) ((UINT32) pu1DstBase + (i * u4Pitch));

        GFX_SetSrc(pu1SrcAddr, (UINT32) CM_RGB_CLUT8, u4Pitch);
        GFX_SetDst(pu1DstAddr, (UINT32) CM_RGB_CLUT8, u4Pitch);
        GFX_2DMemCompare(0, 0, 0, 0, u4Pitch, 1);
        GFX_Flush();
        GFX_Wait();

        //check compare result
        if (GFX_GetMemCompareResult() == (INT32) E_GFX_INV_ARG)
        {
            GFX_Unlock();
            return -((INT32) E_GFX_INV_ARG);
        }
    } // ~for

    if (u4LastSize > 0)
    {
        pu1SrcAddr = (UINT8 *) ((UINT32) pu1SrcBase + (i * u4Pitch));
        pu1DstAddr = (UINT8 *) ((UINT32) pu1DstBase + (i * u4Pitch));

        GFX_SetSrc(pu1SrcAddr, (UINT32) CM_RGB_CLUT8, u4Pitch);
        GFX_SetDst(pu1DstAddr, (UINT32) CM_RGB_CLUT8, u4Pitch);
        GFX_2DMemCompare(0, 0, 0, 0, u4LastSize, 1);
        GFX_Flush();
        GFX_Wait();

        // check compare result
        if (GFX_GetMemCompareResult() == (INT32) E_GFX_INV_ARG)
        {
            GFX_Unlock();
            return -((INT32) E_GFX_INV_ARG);
        }
    }

    // unlock gfx-op
    GFX_Unlock();
    return -((INT32) E_GFX_OK);
}

//-------------------------------------------------------------------------
/** GFX_RotateBmp
 *  use "horizontal line to vertical line operation" to 
 *  complete this function.
 *  <assumption : src-pitch = dst-pitch>
 *  (high-level API)
 */
//-------------------------------------------------------------------------
void GFX_RotateBmp(UINT8 *pu1DstBase, UINT32 u4DstPitch, UINT8 *pu1SrcBase,
        UINT32 u4SrcPitch, UINT32 u4Width, UINT32 u4Height, UINT32 u4ColorMode,
        UINT32 u4Is90Ccw)
{
    UINT32 i, u4Dx, u4Dy;
    UINT32 u4Sx = 0, u4Sy = 0;

    GFX_ASSERT(pu1DstBase != NULL);
    GFX_ASSERT(pu1SrcBase != NULL);
    GFX_ASSERT(u4Width != 0);
    GFX_ASSERT(u4Height != 0);

    // lock gfx-op
    //GFX_Lock();

    GFX_SetSrc(pu1SrcBase, u4ColorMode, u4SrcPitch);
    GFX_SetDst(pu1DstBase, u4ColorMode, u4DstPitch);

    GFX_SetHoriToVertLineOpt(u4Is90Ccw);

    if (u4Is90Ccw == (UINT32) TRUE)
    {
        // rotated by 90" counter-clock-wise
        u4Dx = 0;
        u4Dy = (u4Width - 1);

        for (i = 0; i < u4Height; i++)
        {
            GFX_HoriToVertLine(u4Sx, u4Sy++, u4Dx++, u4Dy, u4Width);
        } // ~for
    }
    else
    {
        // rotated by 90" clock-wise
        u4Dx = (u4Height - 1);
        u4Dy = 0;

        for (i = 0; i < u4Height; i++)
        {
            GFX_HoriToVertLine(u4Sx, u4Sy++, u4Dx--, u4Dy, u4Width);
        } // ~for
    }

    // flush and wait
    GFX_Flush();
    GFX_Wait();

    // unlock gfx-op
    //GFX_Unlock();
}

//-------------------------------------------------------------------------
/** GFX_Memset
 *  use GFX engine to do memset
 *  (high-level API)
 */
//-------------------------------------------------------------------------
void GFX_Memset(UINT8 *pu1Addr, UINT32 u4Width, UINT32 Height, UINT32 u4Color)
{
    GFX_Lock();
    GFX_MMU_Set_Enable(FALSE);
    GFX_SetDst(pu1Addr, (UINT32) CM_RGB_CLUT8, u4Width);
    GFX_SetColor(u4Color);
    GFX_Fill(0, 0, u4Width, Height);
    GFX_Flush();
    GFX_Task_Wait_Sync();
    GFX_Unlock();
}
//-------------------------------------------------------------------------
/** GFX_Task_Memset
 *  use GFX engine to do memset, async mode, return Task id.
 *  (high-level API)
 */
//-------------------------------------------------------------------------

UINT64 GFX_Task_Memset(UINT8 *pu1Addr, UINT32 u4Width, UINT32 Height, UINT32 u4Color)
{
    UINT64 u8TaskId = 0;
    GFX_Lock();
    GFX_MMU_Set_Enable(FALSE);
    GFX_SetDst(pu1Addr, (UINT32) CM_RGB_CLUT8, u4Width);
    GFX_SetColor(u4Color);
    GFX_Fill(0, 0, u4Width, Height);
    u8TaskId = GFX_Task_Flush();
    if(u8TaskId == 0) //Do not support async mode.
    {
        GFX_Wait();
    }
    GFX_Unlock();
    return u8TaskId;
}

BOOL _GFX_ColorConvert(GFX_COLOR_CONVERT_T* prGfxColorConvert)
{
    UINT32 udx, udy;
    //UINT32 u4YcFmt;

    ASSERT(prGfxColorConvert->pu1SrcAddr != NULL);
    ASSERT(prGfxColorConvert->pu1DstAddr != NULL);
    ASSERT(prGfxColorConvert->pu1CbCr != NULL);

    ASSERT(prGfxColorConvert->u4Width > 0);
    ASSERT(prGfxColorConvert->u4Height > 0);

    //ASSERT(prGfxColorConvert->u4DstX >=0);
    //ASSERT(prGfxColorConvert->u4DstY >=0);
    
    udx = (UINT32) prGfxColorConvert->u4DstX;
    udy = (UINT32) prGfxColorConvert->u4DstY;

    GFX_Lock();
    
    if (prGfxColorConvert->t_mmu.u4_init&&prGfxColorConvert->t_mmu.u4_enable)
    {
       GFX_MMU_Set_Cfg(&prGfxColorConvert->t_mmu);
    }
    else
    {
       GFX_MMU_Set_Enable(FALSE);
    }
    
    
    GFX_SetColCnvFmt(E_YCFMT_420LINEAR, (UINT32) E_SWAP_BLOCK,
            (UINT32) E_VSTD_BT709, (UINT32) E_VSYS_COMP);

    GFX_SetColCnvSrc(prGfxColorConvert->pu1SrcAddr, prGfxColorConvert->u4Width,
            prGfxColorConvert->pu1CbCr, prGfxColorConvert->u4Width, 0);

    GFX_SetDst(prGfxColorConvert->pu1DstAddr, prGfxColorConvert->u4DstCM,
            prGfxColorConvert->u4DstPitch);

    GFX_SetAlpha((UINT32) prGfxColorConvert->u1Alpha);

    GFX_ColConv(udx, udy, prGfxColorConvert->u4Width,
            prGfxColorConvert->u4Height);

    GFX_Flush();
    GFX_Task_Wait_Sync();

    GFX_Unlock();

    return TRUE;
}

void _GFX_SetRotate90(UINT32 u4Is90dCcw, UINT32 u4DstRd, UINT32 u4DstWr)
{
    REG(fg_DSTPITCH_DEC) = u4Is90dCcw;
    REG(fg_DST_RD_ROTATE) = u4DstRd;
    REG(fg_DST_WR_ROTATE) = u4DstWr;
    
    GFX_CMDENQ(GREG_BITBLT_MODE);
    
}

void GFX_SetDirectFBMem(UINT32 u4Addr, UINT32 u4Size)
{
#ifdef __KERNEL__
    _u4DirectFBAddr = u4Addr;
    _u4DirectFBSize = u4Size;
#else
    return;
#endif
}

UINT32 GFX_GetDirectFBMem_Addr(void)
{
    #ifdef __KERNEL__
    return _u4DirectFBAddr;
	#else
	return 0x0;
	#endif
}

UINT32 GFX_GetDirectFBMem_Size(void)
{
    #ifdef __KERNEL__
    return _u4DirectFBSize;
	#else
	return 0x0;
	#endif
}

UINT32 GFX_Check_DirectFBMem(UINT32 u4_addr)
{
    #ifdef __KERNEL__
    {
        if( (u4_addr>=_u4DirectFBAddr)&&(u4_addr<(_u4DirectFBAddr+_u4DirectFBSize)))
        {
            return 1;
        }
    
        return 0;
    }
    #endif
    
    return 0;
}


void GFX_FlushInvalidateDCacheMultipleLine(void)
{
    HalFlushInvalidateDCache();
}

void _GFX_SetLegalAddr(UINT32 u4Enable,UINT32 u4Start, UINT32 u4End)
{
    REG(fg_LEGAL_START_ADDR)        = u4Start;
    REG(fg_LEGAL_START_ADDR_MSB)    = GFX_REG_ADDR_MSG(u4Start);
    REG(fg_LEGAL_END_ADDR)          = u4End;
    REG(fg_LEGAL_END_ADDR_MSB)      = GFX_REG_ADDR_MSG(u4End);
    REG(fg_MEM_PROTECT_EN)          = u4Enable;
    
    GFX_Enable_CmdQ(FALSE);
    GFX_RiscPushBack((UINT32)GREG_LEGAL_START_ADDR, _au4RegFile[(UINT32)GREG_LEGAL_START_ADDR]);
    GFX_RiscPushBack((UINT32)GREG_LEGAL_END_ADDR, _au4RegFile[(UINT32)GREG_LEGAL_END_ADDR]);
    GFX_RiscPushBack((UINT32)GREG_ADDR_MSB2, _au4RegFile[(UINT32)GREG_ADDR_MSB2]);
    GFX_Enable_CmdQ(TRUE);
    return;
}

UINT32 GFX_GetTmpBuf(void)
{
    return (GFX_OBJ_GET(E_GFX_OBJ_TMP_ADDR));
}

void GFX_SetTmpBuf(UINT32 u4_val)
{
    GFX_OBJ_SET(E_GFX_OBJ_TMP_ADDR,u4_val);
}

UINT32 GFX_GetTmpBufSize(void)
{
    return (GFX_OBJ_GET(E_GFX_OBJ_TMP_SIZE));
}

void GFX_SetTmpBufSize(UINT32 u4_val)
{
    GFX_OBJ_SET(E_GFX_OBJ_TMP_SIZE,u4_val);
}

void _GFX_Set_YCbCr420Swap2ToARGB32(UINT32 u4AlphaValue, UINT32 u4FieldPic,
                                            UINT32 u4Sys,UINT32 u4Std,UINT32 u4Vfull)
{
    GFX_SET_BURST_READ();
    REG(fg_CM)              = 0xE;
    REG(fg_OP_MODE)         = 0xA;
    REG(fg_WRITE_NO_PITCH)  = 0x1;
    REG(fg_ALPHA_VALUE)     = u4AlphaValue;
    REG(fg_FLD_PIC)         = u4FieldPic;
    REG(fg_YC_FMT)          = 0x0;
    REG(fg_VSYS)            = u4Sys;
    REG(fg_VSTD)            = u4Std;
    REG(fg_VFULL)           = u4Vfull;
    REG(fg_FIRE)            = GFX_ENGINE_FIRE;
    GFX_CMDENQ(GREG_SRCCBCR_BSAD);
    GFX_CMDENQ(GREG_SRCCBCR_PITCH);
    GFX_CMDENQ(GREG_BITBLT_MODE);
    GFX_CMDENQ(GREG_G_MODE);
}

//-------------------------------------------------------------------------
/** GFX_Task_Init
 *  gfx action thread
 */
//-------------------------------------------------------------------------

void GFX_Task_Init(void)
{
    x_memset((void *) t_gfx_task_list, 0x0, sizeof(t_gfx_task_list));
    x_memset(pt_task_data, 0x0, sizeof(GFX_TASK_THREAD_DATA_T));
    
    if(!GFX_TASK_SUPPORT_FG)
    {
        return;
    }
    #ifdef LINUX_TURNKEY_SOLUTION
    GFX_Task_List_Init();
    GFX_Task_Cmdq_Set_Mutex(0x0);
    GFX_Task_Cmdq_Set_Cmit_Cnt(0x0);
    GFX_Task_Cmdq_Set_Isr_Cnt(0x0);
    VERIFY (x_sema_create(&pt_task_data->h_task_sema, 
                            X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
    VERIFY (x_thread_create(&pt_task_data->h_th_hdl, 
                            DIAG_GFX_CMDQ_NAME, DIAG_GFX_CMDQ_STACK_SZ, 
                            (DIAG_GFX_CMDQ_PRIORITY - 1),
                            GFX_Task_Thread, 0, NULL) == OSR_OK);
   

    GFX_Task_List_Get_Create(GFX_TASK_CUR_PID);

    return;
    #endif
}

void GFX_Task_Lock(void)
{
    GFX_UNUSED_RET(x_sema_lock(pt_task_data->h_task_sema, X_SEMA_OPTION_WAIT));
    pt_task_data->u4_lock++;
    
    return;
}

void GFX_Task_Unlock(void)
{
    GFX_UNUSED_RET(x_sema_unlock(pt_task_data->h_task_sema));
    pt_task_data->u4_unlock++;
    
    return;
}

void* GFX_Task_List_Get_Create(UINT32 u4_id)
{   
    UINT32 u4_lp                        = 0x0;
    UINT32 u4_idle                      = (UINT32)GFX_TASK_LIST_MAX;
    GFX_TASK_LIST_T *pt_gfx_task_list   = GFX_Task_List_Addr;
    
    for(u4_lp=0x0; u4_lp< GFX_TASK_LIST_MAX; u4_lp++)
    {
        // GFX_PRINT("Create Task[%d][0x%x,0x%x][%d]\n",u4_lp,u4_id,pt_gfx_task_list[u4_lp].u4_pid,pt_gfx_task_list[u4_lp].u4_state);
        
        if( (u4_id == pt_gfx_task_list[u4_lp].u4_pid)&&
            (E_GFX_LIST_STATE_USE == pt_gfx_task_list[u4_lp].u4_state))
        {
            /* Get task list*/
            return &pt_gfx_task_list[u4_lp];
        }

        if( (E_GFX_LIST_STATE_USE != pt_gfx_task_list[u4_lp].u4_state) && 
            (GFX_TASK_LIST_MAX == u4_idle) )
        {
             GFX_PRINT("Create Task u4_idle[%d][0x%x,0x%x][%d]\n",u4_lp,u4_id,pt_gfx_task_list[u4_lp].u4_pid,pt_gfx_task_list[u4_lp].u4_state);
            u4_idle = u4_lp;
        }
    }
    
    if(GFX_TASK_LIST_MAX != u4_idle)
    {
        /* Create task list*/
        pt_gfx_task_list[u4_idle].u4_buf_size = GFX_CMDQ_BUF_SIZE<<1;
        pt_gfx_task_list[u4_idle].u4_buf=(UINT32)(BSP_AllocAlignedDmaMemory(
                       pt_gfx_task_list[u4_idle].u4_buf_size,E_GFX_CQ_ALIGN));
        
        //Printf("GFX_Cmd_Task_List_Create_Buffer[0x%x,%d][0x%x,0x%x]\n",u4_id,u4_idle,
                                        //pt_gfx_task_list[u4_idle].u4_buf,
                                        //pt_gfx_task_list[u4_idle].u4_buf_size);
                                        
        for(u4_lp=0x0; u4_lp< GFX_TASK_CMDQ_DBG_LEN; u4_lp++)
        {
            pt_task_data->u4_dbg_cmdq[u4_lp]=(UINT32)(BSP_AllocAlignedDmaMemory(GFX_CMDQ_BUF_SIZE,E_GFX_CQ_ALIGN));
            VERIFY(pt_task_data->u4_dbg_cmdq[u4_lp] != 0x0); 
            pt_task_data->u4_dbg_cmdq[u4_lp]=(UINT32)VIRTUAL(pt_task_data->u4_dbg_cmdq[u4_lp]);
        }
        
        VERIFY(pt_gfx_task_list[u4_idle].u4_buf != 0x0); 
        
        pt_gfx_task_list[u4_idle].u4_buf=(UINT32)VIRTUAL(pt_gfx_task_list[u4_idle].u4_buf);
       
        pt_gfx_task_list[u4_idle].u4_cnt        = 0x0;
        pt_gfx_task_list[u4_idle].pt_first      = NULL;
        pt_gfx_task_list[u4_idle].pt_end        = NULL;
        pt_gfx_task_list[u4_idle].u4_pid        = u4_id;
        pt_gfx_task_list[u4_idle].u4_state      = E_GFX_LIST_STATE_USE; 
        
        pt_gfx_task_list[u4_idle].u4_priority   = E_GFX_TASK_PRIORITY_NORMAL;
        if( u4_idle < 3 )
        {
            pt_gfx_task_list[u4_idle].u4_priority = E_GFX_TASK_PRIORITY_HEIGHT;
        }
        else if(u4_idle%2)
        {
            pt_gfx_task_list[u4_idle].u4_priority = E_GFX_TASK_PRIORITY_LOW;
        }
        
        //GFX_PRINT("Create Task List OK [%d]\n",u4_idle);
        return &pt_gfx_task_list[u4_idle];
    }

    GFX_PRINT("Create Task List Fail [0x%x]\n",u4_id);
    
    return NULL;
}

void GFX_Task_List_Init(void)
{
    UINT32 u4_lp    = 0x0;
    GFX_TASK_LIST_T *pt_gfx_task_list = GFX_Task_List_Addr;
    
    for(u4_lp=0x0; u4_lp<GFX_TASK_LIST_MAX; u4_lp++)
    {
        VERIFY(x_sema_create(&pt_gfx_task_list[u4_lp].h_sema, 
                            X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        
        VERIFY(x_sema_create(&pt_gfx_task_list[u4_lp].h_sync, 
                            X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        
        pt_gfx_task_list[u4_lp].u4_state     = E_GFX_LIST_STATE_NONE;
        pt_gfx_task_list[u4_lp].u4_pid       = GFX_INV_VAL_U32;
        pt_gfx_task_list[u4_lp].u4_priority  = E_GFX_TASK_PRIORITY_NORMAL;
    }
}

int GFX_Task_Send_Action(void* pv_data, UINT32 u4_size, UINT32 u4_type,UINT64* pu8_id)
{
    GFX_TASK_LIST_T *pt_list = NULL;
    GFX_TASK_CMDQ_T *pt_end  = NULL;
    GFX_TASK_CMDQ_T *pt_crnt = NULL;
    UINT32 u4_cmdq_addr = 0x0;
    int i4_ret          = 0x0;
    UINT32 u4_end_addr  = 0x0;  
    UINT32 u4_idle      = 0x0;
    UINT32 u4_to_end_size = 0x0;
    UINT32 u4_crc_size  = 0x0;
    UINT32 u4_mod       = 0x0;

    UNUSED(u4_to_end_size);
    pt_list= (GFX_TASK_LIST_T *)GFX_Task_List_Get_Create(GFX_TASK_CUR_PID);
    if((NULL == pt_list)||(u4_size > GFX_CMDQ_BUF_SIZE))
    {
        i4_ret = -2;
        GFX_PRINT("GFX TSA Fail[0x%x,0x%x,0x%x]\n",(UINT32)pt_list,u4_size,GFX_CMDQ_BUF_SIZE);
        return i4_ret;
    }
    
    do
    {
        GFX_Task_List_Lock(pt_list->h_sema);
        pt_end = pt_list->pt_end;

        if(pt_end)
        {
            u4_end_addr     = (pt_end->u4_cmdq_addr + pt_end->u4_size);
            if((UINT32)pt_list->pt_first >= u4_end_addr)
            {
                u4_cmdq_addr = u4_end_addr;
                u4_idle= (UINT32)((UINT32)pt_list->pt_first- u4_end_addr);
                u4_to_end_size = u4_idle;
            }
            else
            {
                if(u4_end_addr >= (UINT32)GFX_TASK_CMDQ_MID(pt_list))
                {
                    u4_cmdq_addr = GFX_TASK_CMDQ_ADDR(pt_list);
                    u4_idle= ((UINT32)pt_list->pt_first-GFX_TASK_CMDQ_ADDR(pt_list));
                }
                else
                {
                    u4_cmdq_addr = u4_end_addr;
                    u4_idle= (UINT32)(GFX_TASK_CMDQ_END(pt_list) - u4_end_addr);
                }
                u4_to_end_size = (UINT32)(GFX_TASK_CMDQ_END(pt_list) - u4_end_addr);
            }
        }
        else
        {
            u4_cmdq_addr    = GFX_TASK_CMDQ_ADDR(pt_list);
            u4_idle         = GFX_TASK_CMDQ_SIZE(pt_list);
            u4_to_end_size  = u4_idle;
        }

        if( (u4_size+GFX_TASK_CMDQ_ST_SIZE) > u4_idle)
        {
            i4_ret = -2;
            // Printf("Task Sed Fail [%d][0x%x,0x%x,0x%x]\n",pt_list->u4_cnt,u4_size,GFX_TASK_CMDQ_ST_SIZE,u4_idle);
            break;
        }

        if(0x0) //(pt_end&&(u4_to_end_size >= u4_size)&&(u4_type&(E_GFX_TASK_CMDQ)))
        {
            if((E_GFX_TASK_CMDQ==pt_end->u4_type)&&((pt_end->u4_size+u4_size) < GFX_CMDQ_BUF_SIZE ))
            {
                if((UINT32)((UINT32)pv_data + u4_size) > GFX_TASK_SRC_CMDQ_END)
                {
                    u4_mod = 0x1;
                }
                GFX_TASK_LIST_SRC_CRC_COPY(pv_data,u4_size,u4_end_addr,u4_mod);
                pt_end->u4_size += u4_size;
                pt_crnt = pt_list->pt_end;
                pt_task_data->u4_merge_cmd_cnt++;
                pt_task_data->u4_total_cmd_size += u4_size;
                break;
            }
        }

        pt_crnt = (GFX_TASK_CMDQ_T *) u4_cmdq_addr;
        pt_crnt->u4_size = u4_size;
        pt_crnt->u4_type = u4_type;
        pt_crnt->u4_cmdq_addr = (u4_cmdq_addr + GFX_TASK_CMDQ_ST_SIZE);
        pt_crnt->pt_next = NULL;

        if(pt_end)
        {
            pt_end->pt_next = pt_crnt;
        }
        else
        {
            pt_list->pt_first = pt_crnt;
        }

        pt_list->pt_end = pt_crnt;

        if((u4_type &(E_GFX_TASK_CMDQ|E_GFX_TASK_CMDQ_MMU))&&
                ((UINT32)((UINT32)pv_data + u4_size) > GFX_TASK_SRC_CMDQ_END))
        {
            u4_mod = 0x1;
        }

        GFX_TASK_LIST_SRC_CRC_COPY(pv_data,u4_size,pt_crnt->u4_cmdq_addr,u4_mod);
        pt_list->u4_cnt++;
        
        if(0x1 == pt_list->u4_cnt)
        {
            GFX_Task_List_Lock(pt_list->h_sync); 
        }
        
        pt_task_data->u4_total_cmd_cnt++;
        if(0x0 == pt_task_data->u4_total_cmd_cnt)
        {
            pt_task_data->u4_total_cmd_cnt = 0x1;
        }
        pt_crnt->u8_id   = pt_task_data->u4_total_cmd_cnt;
        pt_task_data->u4_total_cmd_size += u4_size;
        i4_ret = 0x0;
    }while(0);
    
    if(pt_crnt)
    {
        *pu8_id = pt_crnt->u8_id;
    }
    
    GFX_Task_List_Unlock(pt_list->h_sema);
    
    if(GFX_DBG_LVL(GFX_DBG_LVL_11))
    {
        GFX_PRINT("GFX_Task_Send_Action OK[%d]\n",pt_task_data->u4_total_cmd_cnt);
    }

    return i4_ret;
}



UINT64 GFX_Task_Send(void* pv_data, UINT32 u4_size, UINT32 u4_type)
{
    UINT64 u8_id = 0x0;
    
    while (GFX_Task_Send_Action(pv_data, u4_size, u4_type,&u8_id))
    {
        // Printf("GFX_Task_Wait_Sync \n");
        GFX_Wait();
    }
    
    pt_task_data->u4_send_cmd_cnt++;
     
    GFX_Task_Unlock();
    
    return u8_id;
}


void GFX_Task_Get_CmdQ(GFX_TASK_LIST_T **ppt_list,GFX_TASK_CMDQ_T** ppt_cmdq)
{
    UINT32          u4_lp                   = 0x0;
    GFX_TASK_CMDQ_T *pt_this                = NULL;
    GFX_TASK_LIST_T *pt_gfx_task_list_addr  = GFX_Task_List_Addr;
    GFX_TASK_LIST_T *pt_task_list           = NULL;
    GFX_TASK_LIST_T *pt_priority_list[E_GFX_TASK_PRIORITY_MAX] = {NULL,NULL,NULL};
    
    for(u4_lp =0x0; u4_lp < GFX_TASK_LIST_MAX; u4_lp++)
    {
        pt_task_list = &pt_gfx_task_list_addr[u4_lp];
        
        if(pt_task_list->u4_cnt&&(!pt_priority_list[pt_task_list->u4_priority]))
        {
            pt_priority_list[pt_task_list->u4_priority] = pt_task_list;
            if(E_GFX_TASK_PRIORITY_HEIGHT == pt_task_list->u4_priority)
            {
                break;
            }
        }
    }

    pt_task_list = NULL;
    for(u4_lp =0x0; u4_lp < E_GFX_TASK_PRIORITY_MAX; u4_lp++)
    {
        if(pt_priority_list[u4_lp])
        {
            pt_task_list = pt_priority_list[u4_lp];
            break;
        }
    }
    
    if(pt_task_list)
    {
        pt_this             = pt_task_list->pt_first;
    }
    
    *ppt_list = pt_task_list;
    *ppt_cmdq = pt_this;
    
    return;
} 

void GFX_Task_List_CmdQPrint(GFX_TASK_LIST_T * pt_this)
{
    GFX_TASK_CMDQ_T  *pt_cmdq = NULL ;
    
    if(!pt_this)
    {
        return;
    }
    pt_cmdq = pt_this->pt_first;
    GFX_PRINT("CmdQ:\n");
    while(pt_cmdq)
    {
        GFX_PRINT("[Type:%d,Addr:0x%x,Size:0x%x]\n",pt_cmdq->u4_type,pt_cmdq->u4_cmdq_addr,pt_cmdq->u4_size);
        pt_cmdq = pt_cmdq->pt_next;
    }
}


void GFX_Task_List_Dcache_Add(UINT32 u4_type)
{
    pt_task_data->u4_dcache_cnt[u4_type]++;
}

void GFX_Set_Dlta(UINT32 u4_s,UINT32 u4_ms)
{
    pt_task_data->u4_dbg_dlta_s     += u4_s;
    pt_task_data->u4_dbg_dlta_ms    += u4_ms;
}

void GFX_Dlta_Init(void)
{
    pt_task_data->u4_dbg_dlta_s  = 0x0;
    pt_task_data->u4_dbg_dlta_ms = 0x0;
}

void GFX_Task_List_Print(void)
{
    UINT32 u4_list_idx = 0x0;
    UINT32 u4_lp  = 0x0;
    UINT32 u4_cnt = 0x0;
    GFX_TASK_LIST_T *pt_gfx_task_list_addr = GFX_Task_List_Addr;
    GFX_TASK_LIST_T *pt_gfx_task_list = NULL;
    
    GFX_PRINT("Task List Info:[send=%lld,total=%lld,merge=%lld,finish=%lld,size=%lld]\n mutex[%lld] isr[%lld,%lld]\n",               
        pt_task_data->u4_send_cmd_cnt,        pt_task_data->u4_total_cmd_cnt,           
        pt_task_data->u4_merge_cmd_cnt,       pt_task_data->u4_finish_cmd_cnt,
        pt_task_data->u4_total_cmd_size,      pt_task_data->u4_mutex,
        pt_task_data->u4_cmmit_cnt,           pt_task_data->u4_isr_cnt);
    
    GFX_PRINT("Dcache Info: \n");
    for(u4_list_idx =0x0; u4_list_idx < E_GFX_TASK_DCACHE_MAX; u4_list_idx++)
    {
        GFX_PRINT("[%lld]",pt_task_data->u4_dcache_cnt[u4_list_idx]);
    }
    GFX_PRINT("dt[%d]s [%d]ms\n",pt_task_data->u4_dbg_dlta_s,pt_task_data->u4_dbg_dlta_ms);
    
    for(u4_list_idx =0x0; u4_list_idx < GFX_TASK_LIST_MAX; u4_list_idx++)
    {
        pt_gfx_task_list = &pt_gfx_task_list_addr[u4_list_idx];
        
        if(E_GFX_LIST_STATE_USE == pt_gfx_task_list->u4_state)
        {
            GFX_PRINT("idx[%4d],pid[%8d],priority[%4d],cmdq[%4d]\n",
                u4_list_idx,                        pt_gfx_task_list->u4_pid,
                pt_gfx_task_list->u4_priority,      pt_gfx_task_list->u4_cnt);
            
            if(pt_gfx_task_list->u4_cnt)
            {
                GFX_Task_List_CmdQPrint(pt_gfx_task_list);
            }
        }
    }

    GFX_Task_CmdQ_Print((UINT32)pt_task_data->u4_pre_cmdq_addr,pt_task_data->u4_pre_cmdq_size);

    if(GFX_DBG_LVL(GFX_DBG_CMDQ))
    {
        u4_cnt = pt_task_data->u4_dbg_cnt;
        for(u4_lp=0x0; u4_lp< GFX_TASK_CMDQ_DBG_LEN; u4_lp++)
        {
            if(u4_cnt)
            {
                u4_cnt--;
            }
            else
            {
                u4_cnt = (GFX_TASK_CMDQ_DBG_LEN -1);
            }
            
            GFX_Task_CmdQ_Print((UINT32)pt_task_data->u4_dbg_cmdq[u4_cnt],pt_task_data->u4_dbg_cmdq_sz[u4_cnt]);
        }
    }
}

UINT32 GFX_Task_Cnt(UINT32 u4_id)
{   
    GFX_TASK_LIST_T *pt_this    = NULL;
    
    pt_this = (GFX_TASK_LIST_T *)GFX_Task_List_Get_Create(u4_id);
    if(pt_this)
    {
        return pt_this->u4_cnt;
    }

    return 0x0;
}

void GFX_Task_Cmdq_Set_Mutex(UINT32 u4_val) 
{
    pt_task_data->u4_mutex = u4_val;
}

void GFX_Task_Cmdq_Set_Cmit_Cnt(UINT32 u4_val) 
{
    pt_task_data->u4_cmmit_cnt = u4_val;
}

UINT32 GFX_Task_Cmdq_Get_Cmit_Cnt(void) 
{
    return pt_task_data->u4_cmmit_cnt;
}

void GFX_Task_Cmdq_Set_Isr_Cnt(UINT32 u4_val) 
{
    pt_task_data->u4_isr_cnt = u4_val;
}

UINT32 GFX_Task_Cmdq_Get_Isr_Cnt(void) 
{
    return pt_task_data->u4_isr_cnt;
}

void GFX_Task_Cmdq_Commit(GFX_TASK_CMDQ_T* pt_this)                                           
{
    GFX_TASK_CHECK_NULL(pt_this);
    GFX_TASK_CHECK_NULL(pt_this->u4_cmdq_addr);
    
    GFX_LockCmdque();

    GFX_WRITE32(GFX_REG_DRAMQ_STAD, PHYSICAL(pt_this->u4_cmdq_addr));
    HalFlushDCacheMultipleLine(VIRTUAL((UINT32)pt_this->u4_cmdq_addr),pt_this->u4_size);
    
    GFX_Task_List_Dcache_Add(E_GFX_TASK_DCACHE_INVALID);
    
    GFX_TASK_CMDQ_FLUSH(pt_this->u4_size);
    
    // GFX_Task_Cmdq_Set_Mutex(0x1);
    GFX_Task_Cmdq_Set_Cmit_Cnt(1+GFX_Task_Cmdq_Get_Cmit_Cnt());
    
    pt_task_data->u4_pre_cmdq_addr = pt_this->u4_cmdq_addr;
    pt_task_data->u4_pre_cmdq_size = pt_this->u4_size;

    if(GFX_DBG_LVL(GFX_DBG_CMDQ)&&GFX_TASK_CMDQ_DBG_LEN)
    {
        x_memcpy((void *)pt_task_data->u4_dbg_cmdq[pt_task_data->u4_dbg_cnt],(void *)pt_this->u4_cmdq_addr,pt_this->u4_size);
        pt_task_data->u4_dbg_cmdq_sz[pt_task_data->u4_dbg_cnt] = pt_this->u4_size;
        pt_task_data->u4_dbg_cnt = (pt_task_data->u4_dbg_cnt+1)%GFX_TASK_CMDQ_DBG_LEN;
    }
    
    GFX_Wait();
    
    return;
}


void GFX_Task_CmdQ_Print(UINT32 u4_addr, UINT32 u4_size)
{
    UINT32 u4Reg            = 0x0;
    UINT32 u4Val            = 0x0;
    UINT32 u4_cmdq_size     = 0x0;
    UINT32* pu4_data        = (UINT32*)(u4_addr);

    printf("GFX_Task_CmdQ_Print[size = %d]:\n",(u4_size/8)); 
    
    while(u4_cmdq_size<(u4_size/4))
    {
        u4Val = (UINT32)pu4_data[u4_cmdq_size];
        u4Reg = (UINT32)pu4_data[u4_cmdq_size+1];
        
        if(u4_cmdq_size && (0 == (u4_cmdq_size%4)))
        {
            //if(GFX_DBG_LVL(GFX_DBG_LVL_5))
            {
                printf("\n");
            }
        }
        
        //if(GFX_DBG_LVL(GFX_DBG_LVL_5))
        {
            printf("[reg= 0x%-8x, val= 0x%-8x]",u4Reg,u4Val);
        }
        
        if(0xf000 != (u4Reg>>16))
        {
            printf("CMDQ ERROR[reg= 0x%-8x, val= 0x%-8x] [0x%x,0x%x]\n",u4Reg,u4Val,u4_addr,u4_size);
        }

        u4_cmdq_size +=2;
    }
    
    //if(GFX_DBG_LVL(GFX_DBG_LVL_5))
    {
        printf("\n");
    }
}

void GFX_Task_Action_Stretch(GFX_TASK_CMDQ_T* pt_this)
{
    GFX_Stretchblit((void*)pt_this->u4_cmdq_addr);
}

void GFX_Task_Action_Stretch_AlphaCompose(GFX_TASK_CMDQ_T* pt_this)
{
    GFX_Stretch_Alphacom((void*)pt_this->u4_cmdq_addr);
}

void GFX_Task_Action(GFX_TASK_CMDQ_T* pt_this)
{
    switch (pt_this->u4_type)
    {
        case E_GFX_TASK_CMDQ:
            GFX_Task_Cmdq_Commit(pt_this);
            break;
        case E_GFX_TASK_CMDQ_MMU:
            GFX_Task_Cmdq_Commit(pt_this);
            break;
        case E_GFX_TASK_STRETCH:
            GFX_Task_Action_Stretch(pt_this);
            GFX_Unlock();
            break;
        case E_GFX_TASK_STRETCH_ALPHACOM:
            GFX_Task_Action_Stretch_AlphaCompose(pt_this);
            GFX_Unlock();
            break;
        default:
            break;
    }
    
    pt_task_data->u4_finish_cmd_cnt++;
    
    return;
}

void GFX_Task_Wait_Sync(void)
{
    GFX_TASK_LIST_T *pt_this    = (GFX_TASK_LIST_T *)GFX_Task_List_Get_Create(GFX_TASK_CUR_PID);
    
    if((GFX_TASK_IS_INITED&GFX_TASK_SUPPORT_MMU)&&(pt_this))
    {
        if(GFX_Task_Cnt(GFX_TASK_CUR_PID))
        {
            GFX_Task_Unlock();
            #ifdef LINUX_TURNKEY_SOLUTION
            schedule();
            #endif
            GFX_Task_List_Lock(pt_this->h_sync);
            GFX_Task_List_Unlock(pt_this->h_sync);
        }
    }

    GFX_Wait();

    return;
}

UINT64 GFX_Task_Flush(void)
{
    return GFX_Flush_Action();
}

UINT32 GFX_Task_Check_Done(UINT64 u8_id)
{
    GFX_TASK_LIST_T *pt_list    = NULL;
    GFX_TASK_CMDQ_T *pt_cmd     = NULL;
    
    pt_list = (GFX_TASK_LIST_T *)GFX_Task_List_Get_Create(GFX_TASK_CUR_PID);
    
    if(pt_list)
    {
        pt_cmd = pt_list->pt_first;
    }
    
    while(pt_cmd)
    {
        if(u8_id == pt_cmd->u8_id)
        {
            return 0x0;
        }

        pt_cmd = pt_cmd->pt_next;
    }

    return 0x1;
}



void GFX_Task_Thread(void* pvArg)
{
    GFX_TASK_CMDQ_T* pt_cmdq = NULL;
    GFX_TASK_LIST_T* pt_list = NULL;

    while (TRUE)
    {
        GFX_Task_Lock();
        do
        {
            GFX_Task_Get_CmdQ(&pt_list,&pt_cmdq);
            if(!pt_cmdq)
            {
                break;
            }
            
            // GFX_Task_List_NLock(pt_list->h_sync);
            
            GFX_Task_Action(pt_cmdq);
            
            GFX_Task_List_Cmdq_Pop(pt_list);
            
        }while (TRUE);
    }
}

void GFX_TmpBuf_Lock(void)
{
    x_sema_lock(GFX_OBJ_GET(E_GFX_OBJ_TMP_SEM), X_SEMA_OPTION_WAIT);
}

void GFX_TmpBuf_Unock(void)
{
    x_sema_unlock(GFX_OBJ_GET(E_GFX_OBJ_TMP_SEM));
}

void GFX_SetGoldenMode(UINT32 u4_val)
{
    GFX_OBJ_SET(E_GFX_OBJ_EMU_FG,u4_val);
}

UINT32 GFX_GetGoldenMode(void)
{
    return GFX_OBJ_GET(E_GFX_OBJ_EMU_FG);
}

