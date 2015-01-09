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
 * $RCSfile: gfx_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_drvif.h
 *  gfx driver internal public interfaces
 *  
 */

#ifndef GFX_DRVIF_H
#define GFX_DRVIF_H
#include "gfx_common_if.h"

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


#define DIAG_GFX_CMDQ_NAME              "GFX_Thread"
#define DIAG_GFX_CMDQ_STACK_SZ          2048
#define DIAG_GFX_CMDQ_PRIORITY          50


#define GFX_CMDQ_BUF_SIZE               (256 * 1024)
#define GFX_TASK_CMDQ_DBG_LEN           0x1



typedef struct _GFX_TASK_THREAD_DATA_T
{
    HANDLE_T        h_sema;
    HANDLE_T        h_task_sema;
    HANDLE_T        h_th_hdl;
    UINT64          u4_op;
    UINT64          u4_lock;
    UINT64          u4_unlock;
    UINT64          u4_send_cmd_cnt;
    UINT64          u4_merge_cmd_cnt;
    UINT64          u4_mg_ng;
    UINT64          u4_total_cmd_cnt;
    UINT64          u4_total_cmd_size;
    UINT64          u4_finish_cmd_cnt;
    UINT64          u4_pre_cmdq_addr;
    UINT64          u4_pre_cmdq_size;
    UINT64          u4_cmmit_cnt;
    UINT64          u4_isr_cnt;
    UINT64          u4_dcache_cnt[E_GFX_TASK_DCACHE_MAX];
    UINT64          u4_mutex;
    UINT32          u4_dbg_cnt;
    UINT32          u4_dbg_dlta_s;
    UINT32          u4_dbg_dlta_ms;
    UINT32          u4_dbg_cmdq[GFX_TASK_CMDQ_DBG_LEN];
    UINT32          u4_dbg_cmdq_sz[GFX_TASK_CMDQ_DBG_LEN];
    void*           pv_data;
} GFX_TASK_THREAD_DATA_T;

/* gfx reg cmdq */

#define GFX_TASK_CMDQ_ALIGN             0x10
#define GFX_TASK_CMDQ_INV               ((GFX_ADDR) | ((GREG_DUMMY << 2) & 0xfff))
#define GFX_TASK_CMDQ_INV_VAL           0x0

#define GFX_TASK_CMDQ_ST_SIZE           (sizeof(GFX_TASK_CMDQ_T))
#define GFX_TASK_LIST_ST_SIZE           (sizeof(GFX_TASK_LIST_T))


typedef struct _GFX_TASK_BUF_T
{
} GFX_TASK_BUF_T;

typedef enum _GFX_TASK_CMDQ_STATE_E
{
    E_GTCS_NONE,
    E_GTCS_IDLE,
    E_GTCS_DOING,
    E_GTCS_DONE,
    E_GTCS_MAX
} GFX_TASK_CMDQ_STATE_E;

typedef enum _GFX_TASK_EXCEPTION_E
{
    E_GT_EXCEPTION_NONE,
    E_GT_EXCEPTION_BUSY,
    E_GT_EXCEPTION_CMDQ,
    E_GT_EXCEPTION_MAX
} GFX_TASK_EXCEPTION_E;

typedef enum _GFX_TASK_CMDQ_MRG_E
{
    E_GTCM_NONE,
    E_GTCM_START,
    E_GTCM_DONE,
    E_GTCM_FAIL,
    E_GTCM_MAX
} GFX_TASK_CMDQ_MRG_E;

typedef struct _GFX_TASK_CMDQ_T
{
    UINT32 u4_type;
    UINT32 u4_size;
    UINT32 u4_cmdq_addr;
    struct _GFX_TASK_CMDQ_T *pt_next;
    UINT64  u8_id;
    UINT32  u4_state;
    UINT32  u4_mrg_state;
} GFX_TASK_CMDQ_T;

#define GFX_TASK_CMDQ_TEST_LEN         100

typedef struct _GFX_TASK_LIST_T
{
    UINT32                  u4_init;
    UINT32                  u4_state;
    UINT32                  u4_buf;
    UINT32                  u4_buf_size;
    UINT32                  u4_cnt;
    UINT32                  u4_priority;
    UINT32                  u4_pid;
    GFX_TASK_CMDQ_T         *pt_first;
    GFX_TASK_CMDQ_T         *pt_end;
    HANDLE_T                h_sema;
    HANDLE_T                h_sync;
} GFX_TASK_LIST_T;

#define GFX_TASK_CMDQ_SIZE_ALIGN(pt_this)                                       \
{                                                                               \
    memset((void *)((pt_this->u4_cmdq_addr + pt_this->u4_size)),GFX_TASK_CMDQ_INV,0x4);\
    memset((void *)((pt_this->u4_cmdq_addr + pt_this->u4_size)+0x4),GFX_TASK_CMDQ_INV_VAL,0x4);\
    pt_this->u4_size += 0x8;                                                   \
}


#define GFX_TASK_CMDQ_FLUSH(u4_size)        (GFX_WRITE32(GFX_REG_DRAMQ_LEN, u4_size))


typedef enum _GFX_OBJ_E
{
    E_GFX_OBJ_NONE,
    E_GFX_OBJ_TMP_ADDR,
    E_GFX_OBJ_TMP_SIZE,
    E_GFX_OBJ_TMP_SEM,
    E_GFX_OBJ_EMU_FG,
    E_GFX_OBJ_MAX
} GFX_OBJ_E;

typedef struct _GFX_OBJ_T
{
    UINT32 u4_info[E_GFX_OBJ_MAX];
} GFX_OBJ_T;

#define GFX_OBJ_GET(e)          t_gfx_obj.u4_info[e]
#define GFX_OBJ_SET(e,val)      t_gfx_obj.u4_info[e]=val

#define GFX_OBJ_INIT()                                                          \
{                                                                               \
    x_memset(&t_gfx_obj,0x0,sizeof(GFX_OBJ_T));                                 \
    VERIFY(x_sema_create(&t_gfx_obj.u4_info[E_GFX_OBJ_TMP_SEM],                 \
            X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);                \
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

extern INT32 _GFX_IsFlushing(void);

extern void _GFX_SetFlushStatus(INT32 i4Status);

extern void _GFX_ReInit(void);

extern void _GFX_Init(void);

extern void _GFX_Reset(void);

extern void _GFX_Wait(void);

extern void _GFX_Wait_Vdp(void);

extern void _GFX_Wait_Timeout(INT32 i4Timeout);

extern void _GFX_MwFlush(void);

extern void _GFX_Flush(void);

extern UINT64 GFX_Flush_Action(void);

extern void _GFX_Lock(void);

extern void _GFX_TryLock(void);

extern void _GFX_Unlock(void);

extern void _GFX_LockCmdque(void);

extern void _GFX_UnlockCmdque(void);

extern INT32 _GFX_QueryHwIdle(void);

extern void _GFX_SetNotify(void(*pfnNotifyFunc)(UINT32));

extern void _GFX_SetDst(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch);

extern void _GFX_SetSrc(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch);

extern void _GFX_SetThird(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch);

extern void _GFX_SetCharCM(BOOL flag, UINT32 u4SrcCM);

extern void _GFX_SetThirdPallete(UINT8 *pu1PaletteBase, UINT32 u4MsbLeft,
        UINT32 u4StartByteAligned);

extern void _GFX_SetCompressionOpt(UINT32 u4LineSeparate, UINT32 u4QualityMode,
        UINT32 u4RollBackEn);

extern void _GFX_GeneralAlphaCom(UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar,
        UINT32 u4OpCode, UINT32 u4RectSrc, UINT32 u4ThirdEnable,
        BOOL fgAlcomNormal, BOOL fgOnePassCompress);

extern void _VGFX_GeneralAlphaComSet(UINT32 u4ImgCm, UINT32 u4WbCm,
        UINT32 u4Ar, UINT32 u4OpCode, BOOL fgAlcomNormal, UINT32 u4PremultSrcR,
        UINT32 u4PremultDstR, UINT32 u4PremultDstW, UINT32 u4PremultOvrflw);
extern void _VGFX_Enable(void);

extern void _GFX_SetAlpha(UINT32 u4Alpha);

extern void _GFX_SetForceAlpha(UINT32 u4Enable);

extern void _GFX_SetARGB1555(UINT32 u4Enable);

extern void _GFX_SetMaxAlphaEn(UINT32 u4Enable);

extern void _GFX_SetRGB0En(UINT32 u4Enable);

extern void _GFX_SetColor(UINT32 u4Color);

extern void _GFX_SetBmpSrc(UINT8 *pu1Base, UINT32 u4BmpCM);

extern void _GFX_SetColorMap(UINT32 u4BmpCM, UINT32 u4DstCM,
        const UINT32 *psu4ColorMap);

extern void _GFX_SetBmpOpt(BOOL fgSubPicDec, BOOL fgRunlenDec);

extern void _GFX_SetSrcColorKey(BOOL fgSrcKeyEn, UINT32 u4SrcKey);

extern void _GFX_SetThirdColorKey(BOOL fgThirdKeyEn, UINT32 u4ThirdKey);

extern void _GFX_BmpBlt(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width,
        UINT32 u4Height);

extern void _GFX_Fill(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width,
        UINT32 u4Height);

extern void _GFX_HLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width);

extern void _GFX_VLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Height);

extern void _GFX_SetBltOpt(UINT32 u4Switch, UINT32 u4ColorMin,
        UINT32 u4ColorMax);

extern void _GFX_BitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);

extern void _GFX_Blend(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);

extern void _GFX_Compose(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar,
        UINT32 u4Mode);

extern void _GFX_AlphaComposePass(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Pass,
        UINT32 u4Param);

extern void _GFX_SetColCnvFmt(UINT32 u4YCFmt, UINT32 u4SwapMode,
        UINT32 u4VidStd, UINT32 u4VidSys);

extern void _GFX_SetVFull(UINT32 u4VFull);

extern void _GFX_SetColCnvSrc(UINT8 *pu1SrcLuma, UINT32 u4SrcLumaPitch,
        UINT8 *pu1SrcChroma, UINT32 u4SrcChromaPitch, UINT32 u4FieldPic);

extern void _GFX_ColConv(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width,
        UINT32 u4Height);

extern void _GFX_SetDstWrRgb24Bit(UINT32 u4Enable);
extern void _GFX_Idx2DirSrcPitchEn(UINT32 u4Enable);
extern void _GFX_SetReadArgbOrder(UINT32 u4Enable, UINT32 u4Aorder,
        UINT32 u4Rorder, UINT32 u4Gorder, UINT32 u4Border);
extern void _GFX_SetWriteArgbOrder(UINT32 u4Aorder, UINT32 u4Rorder,
        UINT32 u4Gorder, UINT32 u4Border);

extern void _GFX_StretchBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW,
        UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW,
        UINT32 u4DstH);

extern void _GFX_AlphaMapBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);

extern void _GFX_SetRopOpt(UINT32 u4RopCode);

extern void _GFX_RopBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);

//#ifdef CC_GFX_COMPOSITION_IMPROVE
extern void _GFX_ComposeLoopImprove(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar,
        UINT32 u4OpCode, UINT32 u4RectSrc);
//#endif

extern void _GFX_SetIdx2DirSrc(UINT8 *pu1Base, UINT32 u4SrcCM);

extern void _GFX_SetIdx2DirOpt(UINT8 *pu1PaletteBase, UINT32 u4MsbLeft,
        UINT32 u4StartByteAligned);

extern void _GFX_Idx2DirBitBlt(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width,
        UINT32 u4Height);

extern void _GFX_2DMemCompare(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);

extern INT32 _GFX_GetMemCompareResult(void);

extern void _GFX_SetHoriToVertLineOpt(UINT32 u4Is90dCcw);

extern void _GFX_HoriToVertLine(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX,
        UINT32 u4DstY, UINT32 u4HoriLineWidth);

extern void _GFX_NewCompressBlt(UINT8 *pu1SrcBase, UINT32 u4SrcX, UINT32 u4SrcY,
        UINT32 u4SrcCM, UINT32 u4SrcPitch, UINT8 *pu1DstBase, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4BufHeight, UINT32 u4RollBackEn,
        UINT32 u4QualityMode, UINT32 u4BpCompAddrEnd, UINT32 u4_blt_fg,
        BOOL fgBurstRead, BOOL fgLineSeparate, UINT32 u4DstPitch);

extern void _GFX_SetPremult(UINT32 u4PremultSrcR, UINT32 u4PremultDstR,
        UINT32 u4PremultDstW, UINT32 u4PremultOvrflw);

extern void _GFX_StretchAlphaCom(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW,
        UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW,
        UINT32 u4DstH, UINT32 u4Ar, UINT32 u4OpCode);

// normal driver
extern void _GFX_SetGradOpt(UINT32 u4IncX, UINT32 u4IncY,
        const UINT32 asu4DeltaX[4], const UINT32 asu4DeltaY[4]);
extern void _GFX_GradFill(UINT32 u4X, UINT32 u4Y, UINT32 u4Width,
        UINT32 u4Height, UINT32 u4Mode);
extern void _GFX_DMA(UINT32 *pu4Dst, UINT32 *pu4Src, UINT32 u4NumWrd);

extern void _GFX_MMU_Init(void);
extern void _GFX_MMU_Enable(UINT32 u4Enable);
extern UINT32 _GFX_MMU_Get_Enable(void);
extern void _GFX_MMU_SelfFire(void);
extern void _GFX_MMU_Set_TabAddr(UINT32 u4TabAddress);
extern void _GFX_MMU_Set_Agent0(UINT32 u4Enable, UINT32 u4Mid);
extern void _GFX_MMU_Set_Agent1(UINT32 u4Enable, UINT32 u4Mid);
extern void _GFX_MMU_Set_Agent2(UINT32 u4Enable, UINT32 u4Mid);
extern void _GFX_MMU_Irq_En(UINT32 u4Enable, UINT32 u4IrqMode);
extern void _GFX_MMU_Set_OverRead(UINT32 u4Enable, UINT32 u4Id, UINT32 u4Addr);
extern void _GFX_MMU_Set_Cfg(GFX_MMU_T* pt_gfx_mmu);
extern void _GFX_MMU_Clear_INT(void);
extern void _GFX_MMU_Reset_INT(void);
extern void GFX_MMU_Check_Status(void);
extern UINT32 GFX_MMU_Get_Status(void);
extern void GFX_MMU_Risc_Self_Fire_Clr(void);


//---------------------------------------------------------------------------
// Public functions (high-level APIs)
//---------------------------------------------------------------------------

extern INT32
        GFX_MemCompare(UINT8 *pu1DstBase, UINT8 *pu1SrcBase, UINT32 u4Size);

extern void GFX_RotateBmp(UINT8 *pu1DstBase, UINT32 u4DstPitch,
        UINT8 *pu1SrcBase, UINT32 u4SrcPitch, UINT32 u4Width, UINT32 u4Height,
        UINT32 u4ColorMode, UINT32 u4Is90Ccw);

extern void GFX_Memset(UINT8 *pu1Addr, UINT32 u4Width, UINT32 Height,
        UINT32 u4Color);
extern UINT64 GFX_Task_Memset(UINT8 *pu1Addr, UINT32 u4Width, UINT32 Height, UINT32 u4Color);
extern UINT32 GFX_Task_Check_Done(UINT64 u8_id);

/*
 BOOL _GFX_ColorConvert(UINT8* pu1SrcAddr,UINT8* pu1DstAddr,UINT8* pu1CbCr,UINT32 u4Width,
 UINT32 u4Height,UINT32 u4DstX,UINT32 u4DstY,UINT32 u4DstPitch,UINT8 u1Alpha,UINT32 u4DstCM);
 */

BOOL _GFX_ColorConvert(GFX_COLOR_CONVERT_T* prGfxColorConvert);

extern void GFX_FlushInvalidateDCacheMultipleLine(void);

#ifdef GFX_CC_YC
extern BOOL _GFX_ColorConvert_YC(GFX_COLOR_CONVERT_T* prGfxColorConvert);
#endif

extern void GFX_Fltr_CbCr(GFX_FLTR_CBCR_T *pt_cbcr);

extern void _GFX_SetXORColor(UINT32 u4Color);
extern void _GFX_Set_PRE_COLORIZE(BOOL u4PreColorize);
extern void _GFX_Set_FastBitBlt(UINT32 u4FastBitBlt);
extern void _GFX_Set_SrcAlReplace(UINT32 u4SrcAlRep);
extern void _GFX_Set_UseRectSrc(UINT32 u4UseRectSrc);
extern void _GFX_Set_WriteNoPitch(UINT32 u4WriteNoPitch);
extern void _GFX_Set_WriteCbCrOder(UINT32 u4WriteCbCrOder);
extern void _GFX_Set_RGB24BitOder(UINT32 u4RGB24BitOder);
extern void _GFX_Set_SrcDst(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4DstPitch,UINT32 u4DstColorMode,UINT32 u4DstX,UINT32 u4DstY,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight);
extern void _GFX_Set_ColConvInfo(UINT32 u4YCFmt, UINT32 u4SwapMode, UINT32 u4VidStd,
        UINT32 u4VidSys,UINT32 u4FieldPic,UINT32 u4Alpha);
extern void _GFX_Set_FlashColCnvInfo(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight);
extern void _GFX_Set_FlashColCnvInfo_WithPitch(UINT8 *pu1SrcY, UINT32 u4SrcYPitch,
        UINT8 *pu1SrcCbCr, UINT32 u4SrcCbCrPitch, UINT8 * u4DstAdress,
        UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4Dst_Pitch);
extern void _GFX_Set_YCbCr420ToARGB32(UINT32 u4AlphaValue, UINT32 u4FieldPic);
extern void _GFX_Set_YCbCrBlockToRaster(UINT32 u4Option);
extern void _GFX_Set_YCbCrBlockToRaster_WithPitch(UINT32 u4Option, UINT32 u4WriteNoPitch);

// gfx_sw.h
extern INT32 GFX_SwBlock2Linear(UINT8 *pu1SrcBase, UINT8 *pu1DstBase,
        INT32 i4Width, INT32 i4Height, INT32 i4MBWidth, INT32 i4MBHeight);
extern INT32 GFX_SwBlock2Swap(UINT8 *pu1SrcBase, UINT8 *pu1DstBase,
        INT32 i4Width, INT32 i4Height, INT32 i4MBWidth, INT32 i4MBHeight);
extern INT32 GFX_SwBlock2Mergetop(UINT8 *pu1SrcBase, UINT8 *pu1DstBase,
        INT32 i4Width, INT32 i4Height, INT32 i4MBWidth, INT32 i4MBHeight);
extern INT32 GFX_SwGetMemCompareResult(void);

extern void _GFX_SetRotate90(UINT32 u4Is90dCcw, UINT32 u4DstRd, UINT32 u4DstWr);
extern void _GFX_Fire(void);
extern void _GFX_Set_YuyvToRaster_4284(UINT32 u4Enable);

extern void _GFX_StretchBlt_forUVBlock(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW,
            UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW,
            UINT32 u4DstH);
extern void _GFX_StretchBlt_forYBlock(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW,
            UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW,
            UINT32 u4DstH);


extern void _GFX_SetLegalAddr(UINT32 u4Enable,UINT32 u4Start, UINT32 u4End);
extern void GFX_Set_DbgLvl(UINT32 u4Value);
extern void GFX_Dlta_Init(void);

extern UINT32 GFX_Get_DbgLvl(void);
extern void GFX_Stretchblit(void* pt_this);
extern UINT32 GFX_Check_DirectFBMem(UINT32 u4_addr);

extern void GFX_Task_Init(void);
extern void GFX_Task_Lock(void);
extern void GFX_Task_Unlock(void);
extern void GFX_Action_Lock(void);
extern void GFX_Action_Unlock(void);
extern void GFX_Task_Wait_Sync(void);
extern UINT64 GFX_Task_Send(void* pv_data, UINT32 u4_size, UINT32 u4_type);
extern void GFX_Task_Thread(void* pvArg);
extern UINT64 GFX_CmdQ_Action_Async(UINT32 u4_type);
extern void* GFX_Task_List_Get_Create(UINT32 u4_id);
extern void GFX_Task_List_Init(void);
extern void GFX_Task_List_Print(void);

extern void GFX_Task_Cmdq_Set_Mutex(UINT32 u4_val);
extern void GFX_Task_Action(GFX_TASK_CMDQ_T* pt_this);
extern void GFX_Task_CmdQ_Print(UINT32 u4_addr, UINT32 u4_size);
extern UINT64 GFX_Task_Flush(void);

extern unsigned int gfx_va_to_pa(unsigned int u4_va);
extern void gfx_print_va_pa(UINT32 u4_start,UINT32 u4_size);

extern void GFX_CmdPreQueDump(void);
extern UINT32 Gfx_Cmd_Get_Task_SrcAddr(void);

extern void GFX_Task_Cmdq_Set_Cmit_Cnt(UINT32 u4_val);
extern UINT32 GFX_Task_Cmdq_Get_Cmit_Cnt(void);
extern void GFX_Task_Cmdq_Set_Isr_Cnt(UINT32 u4_val);
extern UINT32 GFX_Task_Cmdq_Get_Isr_Cnt(void);

extern unsigned int gfx_va_to_pa(unsigned int u4_va);
extern void gfx_print_va_pa(UINT32 u4_start,UINT32 u4_size);

extern UINT32 GFX_GetTmpBuf(void);
extern void GFX_SetTmpBuf(UINT32 u4_val);
extern UINT32 GFX_GetTmpBufSize(void);
extern void GFX_SetTmpBufSize(UINT32 u4_val);

extern void _GFX_Set_YCbCr420Swap2ToARGB32(UINT32 u4AlphaValue, UINT32 u4FieldPic,
                                            UINT32 u4Sys,UINT32 u4Std,UINT32 u4Vfull);

extern void GFX_ClearRegister(void);

extern void GFX_TmpBuf_Lock(void);
extern void GFX_TmpBuf_Unock(void);

extern UINT32 GFX_GetGoldenMode(void);
extern void GFX_SetGoldenMode(UINT32 u4_val);
extern UINT32 GFX_Get_CMSIze(UINT32 u4_val);

extern void GFX_Task_List_Dcache_Add(UINT32 u4_type);
extern void GFX_Set_Dlta(UINT32 u4_s,UINT32 u4_ms);

#endif // GFX_DRVIF_H

