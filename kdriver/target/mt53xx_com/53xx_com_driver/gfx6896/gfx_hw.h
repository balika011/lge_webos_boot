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
 * $RCSfile: gfx_hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_hw.h
 *  gfx hardware interface
 *  init, get register base, isr, vpll calibration, reset,
 *  get status, get que length, get idle, get yline, get op count,
 *  get hardware version, set power down mode
 */

#ifndef GFX_HW_H
#define GFX_HW_H

#include "gfx_common_if.h"

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//define PLL clock for gfx engine
#if defined(CC_MT5398) || defined(CC_MT5881)|| defined(CC_MT5399)||defined(CC_MT5882)
#define GFX_CLK_SEL_VAL     (0x6)
#elif defined(CC_MT5890)  
#define GFX_CLK_SEL_VAL     (0x8)
#else
#define GFX_CLK_SEL_VAL     (0x2)
#endif

//#define GFX_ADDR                IO_ADDR(GRAPH_BASE)
#define GFX_ADDR                (GRAPH_BASE)
#define GFX_MMU_ADDR                (GRAPH_MMU_BASE)
#define GFX_MMU_CMDQUE_ADDR         (GRAPH_MMU_CMD_BASE)
#define VGFX_ADDR                (VGRAPH_BASE)   //gfx scaler
#define VGFX_CMDQUE_ADDR                (VGRAPH_CMDQUE_BASE)    //only use 12 bit when cmdque,  [12]=0, 2D gfx, [12]=1, gfx scaler
#define VGFX_REG_G_CONFIG        (0x00)

// must use IO_READ32 or IO_WRITE32 to control GFX HW registers
#define GFX_REG_G_CONFIG        (0x00)
#define GFX_REG_G_STATUS        (0x04)
#define GFX_REG_DRAMQ_STAD      (0x08)
#define GFX_REG_DRAMQ_LEN       (0x0C)
#define GFX_REG_G_MODE          (0x10)
#define GFX_REG_FILL_COLOR      (0x14)

#define GFX_REG_SRC_BSAD        (0x18)
#define GFX_REG_DST_BSAD        (0x1C)
#define GFX_REG_SRC_SIZE        (0x28)
#define GFX_REG_DST_PITCH       (0x2C)

#define GFX_REG_LEGAL_START     (0x90)
#define GFX_REG_LEGAL_END       (0x94)
#define GFX_REG_LEGAL_MASK      (0x3FFFFFFF)

#define GFX_REG_ALCOM_LOOP       (0x9c)

#define GFX_REG_ROP             (0xA0)

#define GFX_REG_0x40E0          (0xE0)
#define GFX_REG_0x40E4          (0xE4)

#define GFX_REG_0x40EC          (0xEC)
#define GFX_REG_0x40F0          (0xF0)

#define GFX_REG_LARB_CTL        (0x104)
#define GFX_REG_ADDR_MSB_1      (0x118)
#define GFX_REG_ADDR_MSB_2      (0x11c)
#define GFX_REG_WAIT_MMU        (0x12c)


#define GFX_ISR_STATUS          (0x160)
#define GFX_REG_CMDQ_RP         (0x168)

#define GFX_REG_ADDR_MSG_MASK  (0xC0000000)
#define GFX_REG_ADDR_MSG(a)     (((a)&GFX_REG_ADDR_MSG_MASK)>>30)
#define GFX_REG_ADDR_MASK     (0x3FFFFFFF)
#define GFX_REG_SRC_ADDR_MSB_MASK     (0x3000)
#define GFX_REG_DST_ADDR_MSB_MASK     (0x30000)
#define GFX_REG_OP_MODE_MASK          (0x1F0)
#define GFX_REG_CMDQ_RP_MASK          (0x3FFF0000)

//------------------------------------------------
// GFX HW registers' shift
//   [G_CONFIG] - 0x4000
#define GREG_SHT_G_RST          30
#define GREG_SHT_CQ_RST         28
#define GREG_SHT_G_RST_CMDQ     26
#define GREG_SHT_REG_RST        24
#define GREG_SHT_MMU_EN_DRAMQ        23
#define GREG_SHT_MMU_RST        21
#define GREG_SHT_MMU_PGT_ERROR       27

#define GREG_SHT_ENG_LP         17
#define GREG_SHT_SHT_CMDQ       16
#define GREG_SHT_SDFIFO_THRS     9
#define GREG_SHT_DRAMQ_MODE      8
#define GREG_SHT_REQ_INTVAL      6
#define GREG_SHT_CMDFIFO_THRS    4
#define GREG_SHT_POST_THRS       2
#define GREG_SHT_INT_MASK        1
#define GREG_SHT_EN_DRAMQ        0
//   [G_STATUS] - 0x4004
#define GREG_SHT_VERSION_ID      8
#define GREG_SHT_IDLE            0
//   [DRAMQ_STAD] - 0x4008
#define GREG_SHT_CYC_SIZE       30
#define GREG_SHT_DRAMQ_BSAD      0
//   [ROP] - 0x40A0
#define GREG_SHT_CMP_FLAG        9

//------------------------------------------------
// GFX HW registers' mask
//   [G_CONFIG] - 0x4000
#define GREG_MSK_G_RST          ((UINT32)0x3 << GREG_SHT_G_RST)
#define GREG_MSK_CQ_RST         ((UINT32)0x3 << GREG_SHT_CQ_RST)
#define GREG_MSK_G_RST_CMDQ     ((UINT32)0x3 << GREG_SHT_G_RST_CMDQ)
#define GREG_MSK_REG_RST         ((UINT32)0x3 << GREG_SHT_REG_RST)
#define GREG_MSK_MMU_EN_DRAMQ         ((UINT32)0x1 << GREG_SHT_MMU_EN_DRAMQ)
#define GREG_MSK_MMU_RST         ((UINT32)0x3 << GREG_SHT_MMU_RST)
#define GREG_MSK_MMU_PGT_ERROR  ((UINT32)0x3 << GREG_SHT_MMU_PGT_ERROR)
#define GREG_MSK_MMU_SFIRE_RST  ((UINT32)0x20000000)
#define GREG_MSK_MMU_CFG4_ERROR (0x7fffff0)

#define GREG_MSK_ENG_LP         ((UINT32)0x1 << GREG_SHT_ENG_LP)
#define GREG_MSK_SHT_CMDQ       ((UINT32)0x1 << GREG_SHT_SHT_CMDQ)
#define GREG_MSK_SDFIFO_THRS    ((UINT32)0x3 << GREG_SHT_SDFIFO_THRS)
#define GREG_MSK_DRAMQ_MODE     ((UINT32)0x1 << GREG_SHT_DRAMQ_MODE)
#define GREG_MSK_REQ_INTVAL     ((UINT32)0x3 << GREG_SHT_REQ_INTVAL)
#define GREG_MSK_CMDFIFO_THRS   ((UINT32)0x3 << GREG_SHT_CMDFIFO_THRS)
#define GREG_MSK_POST_THRS      ((UINT32)0x3 << GREG_SHT_POST_THRS)
#define GREG_MSK_INT_MASK       ((UINT32)0x1 << GREG_SHT_INT_MASK)
#define GREG_MSK_EN_DRAMQ       ((UINT32)0x1 << GREG_SHT_EN_DRAMQ)
//   [G_STATUS] - 0x4004
#define GREG_MSK_VERSION_ID     ((UINT32)0xFF << GREG_SHT_VERSION_ID)
#define GREG_MSK_IDLE           ((UINT32)0x1 << GREG_SHT_IDLE)
//   [DRAMQ_STAD] - 0x4008
#define GREG_MSK_CYC_SIZE       ((UINT32)0x3 << GREG_SHT_CYC_SIZE)
#define GREG_MSK_DRAMQ_BSAD     ((UINT32)0xFFFFFFF << GREG_SHT_DRAMQ_BSAD)
//   [ROP] - 0x40A0
#define GREG_MSK_CMP_FLAG       ((UINT32)0x1 << GREG_SHT_CMP_FLAG)

#define GFX_G_RST_READY         ((UINT32)0x80000000)
#define GFX_CQ_RST_READY        ((UINT32)0x20000000)
#define GFX_REG_MMU_RST         ((UINT32)0x20)
#define GFX_REG_OFFSET_CFG4     ((UINT32)0x10)
#define GFX_SELF_FIRE_CLR_MSK   ((UINT32)0x40000000)
#define GFX_MMU_SELF_FILE_MSK   ((UINT32)(~(1<<31)))


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Read 32 bits data from GFX HW registers.
#define GFX_READ32(offset)           IO_READ32(GFX_ADDR, (offset))

/// Write 32 bits data into GFX HW registers.
#define GFX_WRITE32(offset, value)   IO_WRITE32(GFX_ADDR, (offset), (value))

/// Read 32 bits data from GFX MMU HW registers.
#define GFX_MMU_READ32(offset)           IO_READ32(GFX_MMU_ADDR, (offset))

/// Write 32 bits data into GFX MMU HW registers.
#define GFX_MMU_WRITE32(offset, value)   IO_WRITE32(GFX_MMU_ADDR, (offset), (value))
/// Read 32 bits data from VGFX HW registers.
#define VGFX_READ32(offset)           IO_READ32(VGFX_ADDR, (offset))
/// Write 32 bits data into VGFX HW registers.
#define VGFX_WRITE32(offset, value)   IO_WRITE32(VGFX_ADDR, (offset), (value))

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

// 5 hw engine state
enum EGFX_CORE_STATUS_T
{
    E_GFX_IDLE = 0, E_GFX_BUSY, E_GFX_SEMIIDLE, E_GFX_CRASHED, E_GFX_UNKNOWN
};

// gfx engine clock source
enum EGFX_ENGINE_CLOCK_T
{
    E_GFX_CLK_XTAL_CK = 0, E_GFX_CLK_SYSPLL_D3, // 216MHz
    E_GFX_CLK_DTDPLL_D3, // 200/144 MHz
    E_GFX_CLK_SYSPLL_D4, // 162MHz
    E_GFX_CLK_SYSPLL_D6, // 108MHz
    E_GFX_CLK_MEM_CK, // UNUSED
    E_GFX_CLK_TVDPLL_D2, // 216MHz
    E_GFX_CLK_USBPLL_CK, //240MHz
    E_GFX_CLK_MAX
};

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern void GFX_HwInit(void);

extern void GFX_HwGetRegBase(UINT32 **ppu4RegBase);

extern void GFX_HwISR(void);

extern void GFX_HwReset(void);

extern INT32 GFX_HwGetIdle(void);
extern INT32 GFX_HwIstIdle(void);

extern void GFX_HwAction(void);

extern void GFX_HwSetRiscMode(void);
extern void GFX_HwSetCmdQMode(void);
extern void GFX_HwSetEngClkSrc(INT32 i4ClkSrc);

extern INT32 GFX_HwGetMemCompareResult(void);

extern UINT32 GFX_HwGetRegDstBase(void);
extern UINT32 GFX_HwGetRegSrcBase(void);
extern UINT32 GFX_HwGetRegOpMode(void);
extern UINT32 GFX_HwGetRegFillColor(void);
extern UINT32 GFX_HwGetRegDstPitch(void);
extern UINT32 GFX_HwGetRegSrcWidth(void);
extern UINT32 GFX_HwGetRegSrcHight(void);
extern UINT32 GFX_HwGetRegCM(void);
extern void GFX_HwResetEngine(void);
extern UINT32 GFX_HwGetRegSrcCm(void);
extern UINT32 GFX_HwGetRegDstCm(void);
extern UINT32 GFX_HwGetRegSrcPitch(void);
extern UINT32 GFX_HwGetRegALCOM_LOOP(void);

extern UINT32 GFX_HwGetCmdQReadPointer(void);

extern void GFX_HwResetEngine(void);
extern void GFX_HwResetCmdQue(void);
extern void GFX_HwGetHwVersion(UINT32 *pu4HwVersion);
extern void GFX_HwEnableLowPowerMode(void);
extern void GFX_HwDisableLowPowerMode(void);
extern void GFX_HwSetSDFifoThreshold(UINT32 u4Value);
extern void GFX_HwSetCMDFifoThreshold(UINT32 u4Value);
extern void GFX_HwSetPOSTFifoThreshold(UINT32 u4Value);
extern void GFX_HwSetReqInterval(UINT32 u4ReqInterval);
#endif // GFX_HW_H

