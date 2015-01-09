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
 * $RCSfile: imgrz_hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_hw.h
 *  image resizer hardware interface
 *  init, get register base, isr, vpll calibration, reset,
 *  get status, get que length, get idle, get yline, get op count,
 *  get hardware version, set power down mode
 */

#ifndef IMGRZ_HW_H
#define IMGRZ_HW_H

//#define IMGRZ_HW_2ND_RZ
//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define IMGRZ_ADDR3                IO_ADDR(0x6B000)
#ifdef IMGRZ_HW_2ND_RZ
#define IMGRZ_ADDR2                 (IMGRZ_BASE2)
#endif
#define IMGRZ_ADDR                  (IMGRZ_BASE)

//hw0 : 0x680c0, hw1 0x68140
#define IMGRZ_MMU_BASE_3              IO_ADDR(0x681c0)

#define IMGRZ_ENG_RST_READY         ((UINT32)0x00000000)
#define IMGRZ_CMDQ_RST_READY        ((UINT32)0x00000000)

// must use IO_READ32 or IO_WRITE32 to control HW registers
//#define RZREG_RZSTART               (0x00)
//#define RZREG_RZDONE                (0xFC)

//#define RZREG_DRAMQ_CFG             (0x1C4)
//#define RZREG_DRAMQ_STAD            (0x1C8)

//#define RZREG_DRAMQ_LEN             (0x1CC)

#define IMGRZ_REG_MASK              (0xFFF)


//------------------------------------------------
// HW registers' shift
//   [RZSTART] - 0x20000
#define RZREG_SHT_REGRST             8
#define RZREG_SHT_INT_MASK           7
#define RZREG_SHT_SW_RSTB            2
#define RZREG_SHT_CKEN               1
#define RZREG_SHT_START              0
//   [RZDONE] - 0x200FC
#define RZREG_SHT_IDLE               4
//   [DRAMQ_CFG] - 0x201C4
#define RZREG_SHT_SW_CMDQ_RST_B     30
#define RZREG_SHT_STOP              16
#define RZREG_SHT_DRAMQ_SIZE         8
#define RZREG_SHT_CMDQ_THRS          4
#define RZREG_SHT_DRAMQ_MODE         1
#define RZREG_SHT_EN_DRAMQ           0
//   [DRAMQ_STAD] - 0x201C8
#define RZREG_SHT_DRAMQ_BSAD         0


//------------------------------------------------
// HW registers' mask
//   [RZSTART] - 0x20000
#define RZREG_MSK_INT_MASK      ((UINT32)0x1 << RZREG_SHT_INT_MASK)
#define RZREG_MSK_SW_RSTB       ((UINT32)0x3 << RZREG_SHT_SW_RSTB)
#define RZREG_MSK_CKEN          ((UINT32)0x1 << RZREG_SHT_CKEN)
#define RZREG_MSK_START         ((UINT32)0x1 << RZREG_SHT_START)
#define RZREG_MSK_REGRST        ((UINT32)0x3 << RZREG_SHT_REGRST)
//   [RZDONE] - 0x200FC
#define RZREG_MSK_IDLE          ((UINT32)0x1 << RZREG_SHT_IDLE)
//   [DRAMQ_CFG] - 0x201C4
#define RZREG_MSK_SW_CMDQ_RST_B ((UINT32)0x3 << RZREG_SHT_SW_CMDQ_RST_B)
#define RZREG_MSK_STOP          ((UINT32)0x1 << RZREG_SHT_STOP)
#define RZREG_MSK_DRAMQ_SIZE    ((UINT32)0x7 << RZREG_SHT_DRAMQ_SIZE)
#define RZREG_MSK_CMDQ_THRS     ((UINT32)0x3 << RZREG_SHT_CMDQ_THRS)
#define RZREG_MSK_DRAMQ_MODE    ((UINT32)0x1 << RZREG_SHT_DRAMQ_MODE)
#define RZREG_MSK_EN_DRAMQ      ((UINT32)0x1 << RZREG_SHT_EN_DRAMQ)
//   [DRAMQ_STAD] - 0x201C8
#define RZREG_MSK_DRAMQ_BSAD    ((UINT32)0xFFFFFFF << RZREG_SHT_DRAMQ_BSAD)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

extern const UINT32 ImgrzBase[IMGRZ_NUM];

#define IMGRZ_READ32(u4Which,offset)         IO_READ32(ImgrzBase[u4Which], (offset))
#define IMGRZ_WRITE32(u4Which,offset,value)   IO_WRITE32(ImgrzBase[u4Which], (offset), (value))



#if defined(CC_MT5368)||defined(CC_MT5396) ||defined(CC_MT5389)||defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890) || defined(CC_MT5882)

#ifdef IMGRZ_HW_IO_MMU

extern const UINT32 ImgrzMMuBase[IMGRZ_NUM];

#define IMGRZ_MMU_READ32(u4Which, offset)           IO_READ32(ImgrzMMuBase[u4Which], (offset))
#define IMGRZ_MMU_WRITE32(u4Which, offset, value)   IO_WRITE32(ImgrzMMuBase[u4Which], (offset), (value))

//#define IMGRZ_MMU_ADDR	    IMGRZ_MMU_BASE

/// Read 32 bits data from GFX MMU HW registers.


#define RZ_REG_RW_IOMMU_CFG0		0x0
#define RZ_REG_RW_IOMMU_CFG1		0x4
#define RZ_REG_RW_IOMMU_CFG2		0x8
#define RZ_REG_RW_IOMMU_CFG3		0xc
#define RZ_REG_RW_IOMMU_CFG4		0x10
#define RZ_REG_RW_IOMMU_CFG8		0x20
#define RZ_REG_RW_IOMMU_CFG9		0x24
#define RZ_REG_RW_IOMMU_CFGA		0x28
#define RZ_REG_RW_IOMMU_CFGB		0x2c
#define RZ_REG_RW_IOMMU_CFGC		0x30
#define RZ_REG_RW_IOMMU_CFGD		0x34

#define RZ_MSK_MMU_SFIRE_RST		((UINT32)0x20000000)

#endif

#endif

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

// 5 hw engine state
enum EIMGRZ_CORE_STATUS_T
{
    E_IMGRZ_IDLE = 0,
    E_IMGRZ_BUSY,
    E_IMGRZ_SEMIIDLE,
    E_IMGRZ_CRASHED,
    E_IMGRZ_UNKNOWN
};

// engine clock source
enum EIMGRZ_ENGINE_CLOCK_T
{
#if 1//def CC_MT5360
    E_IMGRZ_CLK_XTAL_CK = 0,
    E_IMGRZ_CLK_SYSPLL_D2,    // system pll / 2
    E_IMGRZ_CLK_TVDPLL_D3,    // tv-dec pll / 3
    E_IMGRZ_CLK_TVDPLL_D5,    // tv-dec pll / 5
    E_IMGRZ_CLK_DTDPLL_D3,    // dtd    pll / 3   (200 MHz)
    E_IMGRZ_CLK_DTDPLL_D4,    // dtd    pll / 4
    E_IMGRZ_CLK_SYSPLL_D1P5,  //sys      pll / 1 (216 MHz)
    E_IMGRZ_CLK_MEM,          // mem    pll
#else
    E_IMGRZ_CLK_XTAL_CK = 0,
    E_IMGRZ_CLK_SYSPLL_D3,     // 216MHz
    E_IMGRZ_CLK_DTDPLL_D3,     // 200/144 MHz
    E_IMGRZ_CLK_SYSPLL_D4,     // 162MHz
    E_IMGRZ_CLK_SYSPLL_D6,     // 108MHz
    E_IMGRZ_CLK_MEM_CK,        // UNUSED
    E_IMGRZ_CLK_TVDPLL_D2,     // 270MHz
    E_IMGRZ_CLK_USBPLL_CK,     //240MHz
#endif
    E_IMGRZ_CLK_MAX
};

//MT5368, MT5396
enum EIMGRZ_CKCFG_T
{
    IMGRZ_CLK_XTAL = 0,		    // 0: 27
    IMGRZ_CLK_USBPLL_D2,        // 1: 240
    IMGRZ_CLK_SYSPLL_D3,        // 2: 216
    IMGRZ_CLK_USBPLL_D2P5,      // 3: 192
    IMGRZ_CLK_SYSPLL_D4,        // 4: 162
    IMGRZ_CLK_SYSPLL_D6,        // 5:108
    IMGRZ_CLK_USBPLL_D6,        // 6: 80
    IMGRZ_CLK_SAWLESSPLL_D1P5,  // 7: 288
#ifndef IMGRZ_HW_V5
    IMGRZ_CLK_MEM,              // 8: 200~400
#else
	IMGRZ_CLK_MEM,				// 8: 324
    IMGRZ_CLK_337M,				// 9: 337.5
    IMGRZ_CLK_TAPLL_DIV_2,		//10: Tapll/2
    IMGRZ_CLK_G3DPLL_DIV_2,		//11: G3DPLL/2
#endif
    IMGRZ_CLK_MAX
};


#if IMGRZ_UFO_EN
//0x200
#define IMG_RESZ_UFO_UFO_EN                 ((unsigned)0x1 << 12)
#define IMG_RESZ_UFO_UFO_CFG_8BIT           ((unsigned)0x0 << 13)
#define IMG_RESZ_UFO_UFO_CFG_10BIT_REORDER  ((unsigned)0x1 << 13)
#define IMG_RESZ_UFO_UFO_CFG_10BIT_COMPACT  ((unsigned)0x3 << 13)
#define IMG_RESZ_UFO_UFO_10BIT_BYPASS_EN    ((unsigned)0x1 << 15)
#define IMG_RESZ_UFO_UFO_10BIT_BYPASS_DIS   ((unsigned)0x0 << 15)
#define IMG_RESZ_UFO_UFO_16BIT_BYPASS_EN    ((unsigned)0x1 << 16)
#define IMG_RESZ_UFO_UFO_16BIT_BYPASS_DIS   ((unsigned)0x0 << 16)
#endif

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern void IMGRZ_HwInit(UINT32 u4Which);

extern void IMGRZ_HwGetRegBase(UINT32 u4Which, UINT32 **ppu4RegBase);

extern void IMGRZ_HwISR(UINT32 u4Which);

extern void IMGRZ_HwReset(UINT32 u4Which);

extern INT32 IMGRZ_HwGetIdle(UINT32 u4Which);

extern void IMGRZ_HwAction(UINT32 u4Which);

//extern void IMGRZ_HwSetRiscMode(UINT32 u4Which);

//extern void IMGRZ_HwSetCmdQMode(UINT32 u4Which);

extern void IMGRZ_HwSetEngClkSrc(UINT32 u4Which, INT32 i4ClkSrc);

#ifdef IMGRZ_POLLING
extern INT32 IMGRZ_HwPolling(UINT32 u4Which);
#endif

extern void IMGRZ_HwBreak(UINT32 u4Which);

extern void IMGRZ_HwResume(UINT32 u4Which);
extern void IMGRZ_HW_Cfg_MMU(UINT32 u4Which, UINT32 MMU_cfg, UINT32 MMU_table_addr);

extern void IMGRZ_HwPowerDown(UINT32 u4Which);

extern void IMGRZ_SetMMU(UINT32 u4Which);

extern void IMGRZ_HW_MMU_Enable(UINT32 u4Which, BOOL bEnable);

void  vIOMMU_OverRead_Protection(UINT32 u4Which, UINT32 ui4_Agent,
        UINT32 ui4_StartAddr0,
        UINT32 ui4_Buff_sz0,
        UINT32 ui4_StartAddr1,
        UINT32 ui4_Buff_sz1);

#if IMGRZ_UFO_EN
UINT32 IMGRZ_UFO_Cfg(UINT32 u4Which,
		     BOOL fgEnable,
		     RZ_UFO_TYPE eUfoType);
UINT32 IMGRZ_UFO_SZBuf(UINT32 u4Which,
		    UINT32 u4YBufSZSA, UINT32 u4CBufSZSA);
UINT32 IMGRZ_UFO_SetNewPitch(UINT32 u4Which, UINT32 u4UfoPitch);
#endif

#endif // IMGRZ_HW_H
