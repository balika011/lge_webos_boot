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
#ifndef X_DRAM_H
#define X_DRAM_H

#include "x_typedef.h"

#define DRAMC_BASE_ADDRESS 0xf0000000

// PLL registers

// DRAMC registers
#define DRAMC_REG_ACTIM0 0x000
    #define POS_ACTIM0_BL2 15
#define DRAMC_REG_CONF1 0x004
    #define POS_CONF1_BL4 10
    #define POS_CONF1_SELFREF 26
    #define POS_CONF1_TESTLP 27    
#define DRAMC_REG_CONF2 0x008
    #define POS_CONF2_TEST1 29
    #define MASK_CONF2_TE12_ENABLE 0xe0000000
#define DRAMC_REG_PADCTL1 0x00c
#define DRAMC_REG_PADCTL2 0x010
#define DRAMC_REG_PADCTL3 0x014
#define DRAMC_REG_DELDLY1 0x018
#define DRAMC_REG_01C 0x01c
#define DRAMC_REG_DLLCONF 0x028
#define DRAMC_REG_TEST2_0 0x038   // new on MT5399
#define DRAMC_REG_TEST2_1 0x03C
#define DRAMC_REG_TEST2_2 0x040
#define DRAMC_REG_TEST2_3 0x044
    #define POS_TEST2_3_ADVREFEN 30
    #define POS_TEST2_3_TESTAUDPAT 7
    // bit0~3
    #define MASK_TEST2_3_TESTCNT 0x0000000f
    #define POS_TEST2_3_TESTCNT 0
#define DRAMC_REG_TEST2_4 0x048
    // bit0~4
    #define MASK_TEST2_4_TESTAUDINC 0x0000001f
    #define POS_TEST2_4_TESTAUDINC 0
	//bit5
	#define POS_TEST2_4_TEST2DISSCRAM 5
    // bit8~12
    #define MASK_TEST2_4_TESTAUDINIT 0x00001f00
    #define POS_TEST2_4_TESTAUDINIT 8
    // bit 14, 15
    #define POS_TEST2_4_TESTAUDBITINV 14
    #define POS_TEST2_4_TESTAUDMODE 15
    //bit 16
    #define POS_TEST2_4_TESTXTALKPAT 16
#define DRAMC_REG_DDR2CTL 0x07c
    #define MASK_DDR2CTL_DATLAT 0x00000070
    #define POS_DDR2CTL_DTALAT 4
    #define POS_DDR2CTL_WOEN 3
#define DRAMC_REG_084 0x084
#define DRAMC_REG_MRS 0x088
#define DRAMC_REG_CLK1DELAY 0x08c
#define DRAMC_REG_IOCTL 0x090
#define DRAMC_REG_DQSIEN 0x094
#define DRAMC_REG_DRVCTL0 0x0b8
    #define MASK_DRVCTL0_DQDRVN 0x00000f00
    #define MASK_DRVCTL0_DQDRVP 0x0000f000
    #define MASK_DRVCTL0_DQSDRVN 0x0f000000
    #define MASK_DRVCTL0_DQSDRVP 0xf0000000
    #define POS_DRVCTL0_DQDRVN 8
    #define POS_DRVCTL0_DQDRVP 12
    #define POS_DRVCTL0_DQSDRVN 24
    #define POS_DRVCTL0_DQSDRVP 28
#define DRAMC_REG_DRVCTL1 0x0bc
    #define MASK_DRVCTL1_CLKDRVN 0x0f000000
    #define MASK_DRVCTL1_CLKDRVP 0xf0000000
    #define POS_DRVCTL1_CLKDRVN 24
    #define POS_DRVCTL1_CLKDRVP 28
#define DRAMC_REG_MCKDLY 0x0d8
    //bit 23
    #define POS_MCKDLY_FIXODT 23
    //bit 12~15
    #define POS_MCKDLY_FIXDQIEN 12
    #define MASK_MCKDLY_FIXDQIEN 0x0000f000
#define DRAMC_REG_DQSCTL0 0x0dc
#define DRAMC_REG_DQSCTL1 0x0e0
    #define POS_DQSCTL1_DQSIENMODE 28
    #define MASK_DQSCTL1_DQSINCTL 0x07000000
    #define POS_DQSCTL1_DQSINCTL 24
#define DRAMC_REG_PADCTL4 0x0e4
    #define POS_PADCTL4_DATLAT3 4
    #define POS_PADCTL4_CKEFIXON 2
#define DRAMC_REG_PHYCTL1 0x0f0
    #define POS_PHYCTL1_PHYRST 28
#define DRAMC_REG_GDDR3CTL1 0x0f4
    #define POS_GDDR3CTL1_DQMSWAP 31
    #define POS_GDDR3CTL1_RDATRST 25
    #define POS_GDDR3CTL1_BKSWAP 20
#define DRAMC_REG_PADCTL7 0x0f8
#define DRAMC_REG_MISCTL0 0x0fc
#define DRAMC_REG_OCDK 0x100
#define DRAMC_REG_RKCFG 0x110
    #define POS_RKCFG_WDATKEY64 29   //32-bit channel it is 0, for 16-bit or asym DRAM it is 1
#define DRAMC_REG_DQ_DQS01_SEL 0x120
#define DRAMC_REG_158 0x158
#define DRAMC_REG_ARBCTL0 0x168
#define DRAMC_REG_CMDDLY0 0x1a8
#define DRAMC_REG_DQSCAL0 0x1c0
#define DRAMC_REG_DRAMC_PD_CTRL 0x1dc
    #define MASK_DRAMC_PD_CTRL_REFCNT_FR_CLK 0x00ff0000
#define DRAMC_REG_LPDDR2 0x1e0
    #define POS_LPDDR2_ADRDECEN 31
#define DRAMC_REG_SPCMD 0x1e4
    #define POS_SPCMD_MRWEN 0
    #define POS_SPCMD_DQSGCNTEN 8
    #define POS_SPCMD_DQSGCNTRST 9
#define DRAMC_REG_ACTIM1 0x1e8
#define DRAMC_REG_PERFCTL0 0x1ec
#define DRAMC_REG_DQODLY1 0x200
#define DRAMC_REG_DQODLY2 0x204
#define DRAMC_REG_DQODLY3 0x208
#define DRAMC_REG_DQODLY4 0x20c
#define DRAMC_REG_DQIDLY1 0x210
#define DRAMC_REG_DQIDLY2 0x214
#define DRAMC_REG_DQIDLY3 0x218
#define DRAMC_REG_DQIDLY4 0x21c
#define DRAMC_REG_DQIDLY5 0x220
#define DRAMC_REG_DQIDLY6 0x224
#define DRAMC_REG_DQIDLY7 0x228
#define DRAMC_REG_DQIDLY8 0x22c
#define DRAMC_REG_STBENERR_R 0x320
#define DRAMC_REG_STBENERR_F 0x324
#define DRAMC_REG_TOGGLE_CNT 0x32c
#define DRAMC_REG_DQS_ERR_CNT 0x330
#define DRAMC_REG_DQ_ERR_CNT 0x334
#define DRAMC_REG_WRLEV 0x340
    //bit 8
    #define POS_WRLEV_DQS_WLEV 8
    //bit 1~4
    #define POS_WRLEV_DQS_Bx_G 1
    #define MASK_WRLEV_DQS_Bx_G 0x0000001e
    //bit 0
    #define POS_WRLEV_WRITE_LEVEL_EN 0
#define DRAMC_REG_SELPH 0x344
#define DRAMC_REG_DCBLN 0x348
    #define POS_DCBLN_RSTBCNT_LATCH_EN 11
    #define POS_DCBLN_RX_MIOCK_JIT_EN 2
    #define POS_DCBLN_RX_EYE_SCAN_EN 1
    #define POS_DCBLN_REG_SW_RST 0
#define DRAMC_REG_TOGGLE_CNT_2 0x360
#define DRAMC_REG_DQS_ERR_CNT_2 0x364
#define DRAMC_REG_DQ_ERR_CNT_2 0x368
#define DRAMC_REG_CMP_ERR 0x370
#define DRAMC_REG_DQSGNWCNT0 0x3c0
#define DRAMC_REG_DQSGNWCNT1 0x3c4
#define DRAMC_REG_TESTRPT 0x3fc
    #define POS_TESTRPT_DM_CMP_CPT 10
    #define POS_TESTRPT_DM_CMP_ERR 14
#define DRAMC_REG_DLY_CMD 0x400
#define DRAMC_REG_TX_DLY_CMD 0x404
    #define MASK_TX_DLY_CMD_TX_DLY_DQSGATED_MCK 0x00000007
    #define POS_TX_DLY_CMD_TX_DLY_DQSGATED_MCK 0
#define DRAMC_REG_TX_DLY_DQ 0x408
#define DRAMC_REG_DLY_DQ 0x40C
    #define MASK_DLY_DQ_DLY_DQSGATED 0x70000000
    #define POS_DLY_DQ_DLY_DQSGATED 28
#define DRAMC_REG_DLY_DQ_CB1 0x410
    #define MASK_DLY_DQ_CB1_DLY_DQSGATED 0x70000000
    #define POS_DLY_DQ_CB1_DLY_DQSGATED 28
#define DRAMC_REG_DLY_SEL_RAS 0x414
#define DRAMC_REG_COM0_DLY_SEL_DGTED 0x478
#define DRAMC_REG_COM0_DLY_SEL_DQS0 0x47C
#define DRAMC_REG_COM0_DLY_SEL_DQS1 0x480
#define DRAMC_REG_COM0_DLY_SEL_DQM0 0x484
#define DRAMC_REG_COM0_DLY_SEL_DQ2 0x494
#define DRAMC_REG_COM0_DLY_SEL_DQ3 0x498
#define DRAMC_REG_COM0_DLY_SEL_DQ4 0x49c
#define DRAMC_REG_COM0_DLY_SEL_DQ5 0x4a0
#define DRAMC_REG_COM0_DLY_SEL_DQ6 0x4a4
#define DRAMC_REG_COM0_DLY_SEL_DQ7 0x4a8
#define DRAMC_REG_COM1_DLY_SEL_DGTED 0x4CC
#define DRAMC_REG_COM1_DLY_SEL_DQS0 0x4D0
#define DRAMC_REG_COM1_DLY_SEL_DQS1 0x4D4
#define DRAMC_REG_COM1_DLY_SEL_DQM0 0x4D8
#define DRAMC_REG_COM1_DLY_SEL_DQ0 0x4e0
#define DRAMC_REG_COM1_DLY_SEL_DQ2 0x4e8
#define DRAMC_REG_COM1_DLY_SEL_DQ3 0x4ec
#define DRAMC_REG_COM1_DLY_SEL_DQ4 0x4f0
#define DRAMC_REG_COM1_DLY_SEL_DQ5 0x4f4
#define DRAMC_REG_COM1_DLY_SEL_DQ6 0x4f8
#define DRAMC_REG_COM1_DLY_SEL_DQ7 0x4fc
#define DRAMC_REG_PH_EN 0x520
#define DRAMC_REG_DLY_SEL_MCK_ANA 0x524
#define DRAMC_REG_DLY_SEL_CLKGEN 0x528
#define DRAMC_REG_DLY_SEL_CLKGEN2 0x52C


//============================================================================
#define TCM_SRAM_ADDR       	(IO_VIRT + 0x8000)
#define TCM_DRAM_FLAGS    		(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + REG_RW_GPRB6)))
#define TCM_DRAM_SIZE      		(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + REG_RW_GPRB7)))
#define TCM_DRAM_FLAGS1      	(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + REG_RW_GPRDW0)))  //for boot time DQS byte 0~1
#define TCM_DRAM_FLAGS2     	(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + REG_RW_GPRDW1)))  //for boot time DQS byte 2~3
#define TCM_DRAM_FLAGS3     	(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + REG_RW_GPRDW2)))
#define TCM_DRAM_FLAGS4     	(*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + REG_RW_GPRDW3)))
#define TCM_DRAM_DLYCELL_PERT   (*((volatile UINT32 *)BIMSWAP(TCM_SRAM_ADDR + REG_RW_GPRDW3)))

#define DRAM_CLOCK_MASK     (0x00000FFFU)
#define DRAM_TYPE_MASK      (0x0000F000U)
#define TYPE_SHIFT       	(12)
#define DRAM_DDR_CL_MASK    (0x000F0000U)
#define CL_SHIFT       		(16)

#define CHB_SUPPORT_SHIFT       (20)
#define DRAM_CHANNELB_SUPPORT   (1U << CHB_SUPPORT_SHIFT)
#define CHB_ACTIVE_SHIFT        (21)
#define DRAM_CHANNELB_ACTIVE    (1U << CHB_ACTIVE_SHIFT)
#define CHA_FORCE32_SHIFT       (22)
#define DRAM_CHA_FORCE32        (1U << CHA_FORCE32_SHIFT)
#define CHB_FORCE32_SHIFT       (23)
#define DRAM_CHB_FORCE32        (1U << CHB_FORCE32_SHIFT)
#define CHC_ACTIVE_SHIFT        (24)
#define DRAM_CHANNELC_ACTIVE    (1U << CHC_ACTIVE_SHIFT)
#define CHC_SUPPORT_SHIFT       (25)
#define DRAM_CHANNELC_SUPPORT   (1U << CHC_SUPPORT_SHIFT)
//#define QFP_SHIFT                         (24)
//#define DRAM_DDR_QFP                 (1U << QFP_SHIFT)
//#define SWAP16BIT_SHIFT              (25)
//#define DRAM_SWAP16BIT              (1U << SWAP16BIT_SHIFT)
#define DMSSON_SHIFT            (26)
#define DRAM_DMSSON				(1U << DMSSON_SHIFT)
#define DC_BALANCE_SHIFT		(27)
#define DRAM_DC_BALANCE        	(1U << DC_BALANCE_SHIFT)
#define DYNAMIC_ODT_SHIFT       (28)
#define DRAM_DYNAMIC_ODT        (1U << DYNAMIC_ODT_SHIFT)
#define BUSX8_SHIFT				(29)
#define DRAM_BUSX8				(1U << BUSX8_SHIFT)
#define SUSPEND_STATE_SHIFT		(30)
#define DRAM_SUSPEND_STATE			(1U << SUSPEND_STATE_SHIFT)

#define BASE_DDR_CLK			1000000

#define TCMGET_DDR_CLK()				((TCM_DRAM_FLAGS & DRAM_CLOCK_MASK) * BASE_DDR_CLK)
#define TCMGET_DRAMTYPE()				((TCM_DRAM_FLAGS & DRAM_TYPE_MASK) >> TYPE_SHIFT)
#define TCMGET_DDR_CL()					((TCM_DRAM_FLAGS & DRAM_DDR_CL_MASK) >> CL_SHIFT)
#define TCMSET_CHANNELA_ACTIVE()    	(TCM_DRAM_FLAGS &= ~DRAM_CHANNELB_ACTIVE)
#define TCMSET_CHANNELB_ACTIVE()    	(TCM_DRAM_FLAGS |= DRAM_CHANNELB_ACTIVE)
#define IS_DRAM_CHANNELB_SUPPORT()		((TCM_DRAM_FLAGS & DRAM_CHANNELB_SUPPORT) ? 1 : 0)
#define IS_DRAM_CHANNELA_ACTIVE()		((TCM_DRAM_FLAGS & DRAM_CHANNELC_ACTIVE) ? 0 : ((TCM_DRAM_FLAGS & DRAM_CHANNELB_ACTIVE)? 0 : 1))
#define IS_DRAM_CHANNELB_ACTIVE()		((TCM_DRAM_FLAGS & DRAM_CHANNELC_ACTIVE) ? 0 : ((TCM_DRAM_FLAGS & DRAM_CHANNELB_ACTIVE)? 1 : 0))
#define IS_DRAM_CHANNELC_ACTIVE()		0//((TCM_DRAM_FLAGS & DRAM_CHANNELC_ACTIVE) ? 1 : 0)
#define TCMSET_CHANNELA_SIZE(size)		(TCM_DRAM_SIZE = (TCM_DRAM_SIZE & 0xffff0000) + (size & 0xffff))   // size in Mbytes.
#define TCMGET_CHANNELA_SIZE()			(TCM_DRAM_SIZE & 0xFFFF) // size in Mbytes. 
#define TCMSET_CHANNELB_SIZE(size)		(TCM_DRAM_SIZE = (TCM_DRAM_SIZE & 0xffff) + (size << 16))
#define TCMGET_CHANNELB_SIZE()			((TCM_DRAM_SIZE >> 16) & 0xFFFF) // size in Mbytes.
#define TCMSET_CHANNELC_SIZE(size)		0//(TCM_DRAM_FLAGS1 = (TCM_DRAM_FLAGS1 & 0xffff0000) + (size & 0xffff))   // size in Mbytes.
#define TCMGET_CHANNELC_SIZE()			0//(TCM_DRAM_FLAGS1 & 0xFFFF) // size in Mbytes. 
#define TCMSET_DRAM_SUSPEND()			(TCM_DRAM_FLAGS |= DRAM_SUSPEND_STATE)
#define TCMSET_DLYCELL_PERT(delaycell_pert)	TCM_DRAM_DLYCELL_PERT = (IS_DRAM_CHANNELB_ACTIVE() ? \
												((TCM_DRAM_DLYCELL_PERT&0x00ff)|((delaycell_pert)<<8)) \
												: ((TCM_DRAM_DLYCELL_PERT&0xff00)|(delaycell_pert)))
#define TCMGET_DLYCELL_PERT				(IS_DRAM_CHANNELB_ACTIVE() ? \
												(TCM_DRAM_DLYCELL_PERT&0xff00)>>8 \
												: (TCM_DRAM_DLYCELL_PERT&0x00ff))												
#define TCMSET_DRAM_NORMAL()			(TCM_DRAM_FLAGS &= ~DRAM_SUSPEND_STATE)
#define IS_FORCE32()					(IS_DRAM_CHANNELB_ACTIVE() ? \
                                                ((TCM_DRAM_FLAGS & DRAM_CHB_FORCE32) ? 1 : 0) : \
                                                ((TCM_DRAM_FLAGS & DRAM_CHA_FORCE32) ? 1 : 0))
#define IS_DDR_QFP()					0 //((TCM_DRAM_FLAGS & DRAM_DDR_QFP) ? 1 : 0)
#define IS_DDR_SWAP16BIT()				0 //((TCM_DRAM_FLAGS & DRAM_SWAP16BIT) ? 1 : 0)
#define IS_DDR_DMSSON()					((TCM_DRAM_FLAGS & DRAM_DMSSON) ? 1 : 0)
#define IS_DDR_DCBALANCEON()				((TCM_DRAM_FLAGS & DRAM_DC_BALANCE) ? 1 : 0)
#define IS_DDR_DYNAMICODTON()				((TCM_DRAM_FLAGS & DRAM_DYNAMIC_ODT) ? 1 : 0)
#define IS_DDR_BUSX8()					((TCM_DRAM_FLAGS & DRAM_BUSX8) ? 1 : 0)
#define IS_DDR_SUSPENDSTATE()			((TCM_DRAM_FLAGS & DRAM_SUSPEND_STATE) ? 1 : 0)	

//lint -emacro({717}, ADD_DDR_CLK)  to disable "do {...} while (0)" lint warning
#define ADD_DDR_CLK()                                                       \
            do {                                                            \
                UINT32 u4AddClk;                                            \
                u4AddClk = (TCM_DRAM_FLAGS & DRAM_CLOCK_MASK);              \
                TCM_DRAM_FLAGS &= ~(DRAM_CLOCK_MASK);                       \
                TCM_DRAM_FLAGS |= ((u4AddClk + (IS_XTAL_24MHZ()? 48 : 54)) & DRAM_CLOCK_MASK);     \
            } while (0)

//lint -emacro({717}, SUB_DDR_CLK)  to disable "do {...} while (0)" lint warning
#define SUB_DDR_CLK()                                                       \
            do {                                                            \
                UINT32 u4SubClk;                                            \
                u4SubClk = (TCM_DRAM_FLAGS & DRAM_CLOCK_MASK);              \
                    TCM_DRAM_FLAGS &= ~(DRAM_CLOCK_MASK);                   \
                TCM_DRAM_FLAGS |= ((u4SubClk - (IS_XTAL_24MHZ()? 48 : 54)) & DRAM_CLOCK_MASK);   \
            } while (0)

#define mcDELAY_us(x) CHIP_Delay_us(x)

// DRAM data read/write macros
#define INIT_DRAM_B_BASE            0x80000000
#define INIT_DRAM_B_CHB_BASE    	0x40000000

// timeout for TE2: (CMP_CPT_POLLING_PERIOD X MAX_CMP_CPT_WAIT_LOOP) 
// complete flag, need to double check with DE
#define CMP_CPT_POLLING_PERIOD 1
#define MAX_CMP_CPT_WAIT_LOOP 100   // max loop

#define DEFAULT_TEST2_0_CAL 0xaa550000   // patter 1 and 0
#define DEFAULT_TEST2_1_CAL 0x30000000   // base address for test engine when we do calibration, [30:28] is row mask, A15 masked
#define DEFAULT_TEST2_2_CAL 0x00000400   // offset address for test engine when we  do calibraion

// suspend/resume
#define DRAMC_BACKUP_REG_NUM 12   // number of backup registers in DRAMC when suspend
#define PHY_BACKUP_REG_NUM 70   // number of backup registers in DDRPHY when suspend
#define CHA_CHB_COMMON_REG_NUM 5	//cha&chb common register when suspend

//xtal frequency
#define XTAL_MHZ	24

/***********************************************************************/
/*          Public Types                                               */
/***********************************************************************/
typedef unsigned char   U8;
typedef unsigned short  U16;
typedef unsigned long   U32;
typedef unsigned int    UINT;
typedef char            S8;
typedef short           S16;
typedef long            S32;
typedef int             SINT;

typedef U32             DWORD;  //32 bits

typedef U8              Data8;
typedef U16             Data16;
typedef U32             Data32;

typedef U8              *PU8;
typedef U16             *PU16;
typedef U32             *PU32;
typedef S8              *PS8;
typedef S16             *PS16;

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
#define ENABLE  1
#define DISABLE 0

typedef enum
{
    DRAM_OK = 0, // OK
    DRAM_FAIL    // FAIL
} DRAM_STATUS_T; // DRAM status type

typedef enum
{
	DRAM_CALIBRATION_PASS = 0,
	DRAM_CALIBRATION_FAIL,
	DRAM_CALIBRATION_WINDOW_SIZE_TOO_SMALL,
	DRAM_CALIBRATION_WINDOW_SIZE_TOO_BIG
} DRAM_CALIBRATION_STATUS;

typedef enum
{
    TE_OP_WRITE_READ_CHECK = 0,
    TE_OP_READ_CHECK
} DRAM_TE_OP_T;

typedef enum
{
    DLINE_0 = 0,
    DLINE_1,
    DLINE_TOGGLE    
} PLL_PHASE_CAL_STATUS_T;


////////////////////////////

typedef struct _RXDQS_PERBIT_DLY_T
{
    S8 first_dqdly_pass;
    S8 last_dqdly_pass;
    S8 first_dqsdly_pass;
    S8 last_dqsdly_pass;
    U8 best_dqdly;
    U8 best_dqsdly;
} RXDQS_PERBIT_DLY_T;

typedef struct _TXDQS_PERBIT_DLY_T
{
    S8 first_dqdly_l_pass;
    S8 last_dqdly_l_pass;
    S8 first_dqdly_r_pass;
    S8 last_dqdly_r_pass;
    S8 pi_dqdly_ok_center;
} TXDQS_PERBIT_DLY_T;

/************************ Bit Process *************************/
#define mcBITL(b)               (1L << (b))
#define mcBIT(b)                (1L << (b))
#define mcMASK(w)               (mcBIT(w) - 1)
#define mcMASKS(w, b)           (mcMASK(w) << (b))
#define mcFIELD(val, msk, pos)  (((val) << (pos)) & (msk))

#define mcSET_MASK(a, b)        ((a) |= (b))
#define mcCLR_MASK(a, b)        ((a) &= (~(b)))
#define mcCHK_MASK(a, b)        ((a) & (b))
//#define mcSET_BIT(a, b)         mcSET_MASK(a, mcBIT(b))
#define mcSET_BIT(a, b)         ((a) |= ((U32)1L<<(b)))
//#define mcCLR_BIT(a, b)         mcCLR_MASK(a, mcBIT(b))
#define mcCLR_BIT(a, b)         ((a) &= (~((U32)1L<<(b))))
#define mcCHK_BIT1(a, b)        ((a) & mcBIT(b))
#define mcCHK_BITM(a, b, m)     (((a) >> (b)) & (m))
#define mcCHK_BITS(a, b, w)     mcCHK_BITM(a, b, mcMASK(w))
//#define mcTEST_BIT(a, b)        mcCHK_BITM(a, b, 1)
#define mcTEST_BIT(a, b)        mcCHK_BIT1(a, b)
#define mcCHG_BIT(a, b)         ((a) ^= mcBIT(b))

#define mcSET_FIELD0(var, val, msk, pos)    mcSET_MASK(var, mcFIELD(val, msk, pos))

#define mcSET_FIELD(var, value, mask, pos)  \
{                                           \
    mcCLR_MASK(var, mask);                  \
    mcSET_FIELD0(var, value, mask, pos);    \
}

#define mcGET_FIELD(var, mask, pos)     (((var) & (mask)) >> (pos))

#define mcSET_DRAMC_REG_ADDR(offset)    ((IS_DRAM_CHANNELB_ACTIVE() ? DRAM_CHB_BASE : DRAM_BASE) | (offset))
#define mcSET_PHY_REG_ADDR(offset)    	(DRAM_DDRPHY_BASE | (offset))
#define mcSET_ARBITER_ADDR(offset)		((IS_DRAM_CHANNELB_ACTIVE() ? DRAM_DMARB_CHB_BASE : DRAM_DMARB_BASE) | (offset))

// DRAM controller register read/write macros
#define ucDram_Register_Read(reg)			HAL_READ32(reg)
#if 1
#define ucDram_Register_Write(reg, value)	HAL_WRITE32(reg, value)
#else
#define ucDram_Register_Write(reg, value)	do{\
												HAL_WRITE32(reg, value); \
												Printf("DRAM W: 0x%08X = 0x%08X.\n", reg, value);\
											}while(0)
#endif

// DRAM controller register read/write macros
#define DRAM_READ32(offset)           (IS_DRAM_CHANNELB_ACTIVE() ? \
                                                                   IO_READ32(DRAM_CHB_BASE, (offset)) : \
                                                                   IO_READ32(DRAM_BASE, (offset)))
#define DRAM_WRITE32(offset, value)   (IS_DRAM_CHANNELB_ACTIVE() ? \
                                                                   IO_WRITE32(DRAM_CHB_BASE, (offset), (value)) : \
                                                                   IO_WRITE32(DRAM_BASE, (offset), (value)))

#define DRAM_ARB_READ32(offset)			(IS_DRAM_CHANNELB_ACTIVE() ? \
                                                                   IO_READ32(DRAM_DMARB_CHB_BASE, (offset)) : \
                                                                   IO_READ32(DRAM_DMARB_BASE, (offset)))
#define DRAM_ARB_WRITE32(offset, value)   (IS_DRAM_CHANNELB_ACTIVE() ? \
																	   IO_WRITE32(DRAM_DMARB_CHB_BASE, (offset), (value)) : \
																	   IO_WRITE32(DRAM_DMARB_BASE, (offset), (value)))


// DRAM parameters - Params size in DWORD
#define DRAM_PARAMS_TAG_SIZE    (4)
#define DRAM_PARAMS_BANK_SIZE   (48)
#define DRAM_PARAMS_BANK_COUNT  (4)
#define DRAM_PARAMS_SIZE        DRAM_PARAMS_TAG_SIZE + (DRAM_PARAMS_BANK_SIZE * DRAM_PARAMS_BANK_COUNT)

#define DRAMTAG_0 0x2F4B544D
#define DRAMTAG_1 0x2F565444
#define DRAMTAG_2 0x4D415244
#define DRAMTAG_3 0x3031562F

#define DRAM_PARAMS_ADDR    (0x3D00000)    //61MB
#define LOADER_DRAM_ADDR    (0x3E00000)    //62MB
#define DRAM_PARAMS_OFFSET  (0x9800)
#define DRAM_PARAMS_ADDR_AT_SRAM  (0xFB009800)

#if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER)
typedef struct _DRAM_CFG_T
{
    UINT8       ui1_ssc_range;
    UINT8       ui1_ssc_modulation;
    UINT8       ui1_clk_driving;
    UINT8       ui1_clk_delay;
    UINT8       ui1_cmd_driving;
    UINT8       ui1_cmd_delay;
    UINT8       ui1_wdqs_driving;
    UINT8       ui1_wdqs0_delay;
    UINT8       ui1_wdqs1_delay;
    UINT8       ui1_wdqs2_delay;    
    UINT8       ui1_wdqs3_delay;        
    UINT8       ui1_wdq_driving;
    UINT8       ui1_wdq0_delay;    
    UINT8       ui1_wdq1_delay;    
    UINT8       ui1_wdq2_delay; 
    UINT8       ui1_wdq3_delay;        
} DRAM_CFG_T;
#endif /* !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */

// DDR related functions.
extern CHAR * DDR_DramTypeString(UINT32 u4Type);
extern UINT32 DDR_IsDqsFail(UINT32 u4DQSth, UINT32 u4DQSVal, UINT32 u4DQSType, UINT32 u4ChipNum);
extern UINT32 DDR_CalibrateDqs(void);
extern UINT32 DDR_CalibrateOutDqs(void);
extern void DDR_SetDramController(void);
extern void DDR_SetAgentPriority(const UINT8 *pu1Prio);
extern void DDR_SetBurstLen(UINT32 u4TimeSlot1, UINT32 u4TimeSlot2);
extern void DDR_SetArbiterTime(UINT8 u1Group, UINT8 u1Time);
extern UINT32 DRAMC_str_save_registers(UINT32 *u4DRAMCTemp, UINT32 u4Size);
extern void DRAMC_str_restore_registers(UINT32 *u4DRAMCTemp);
extern void DRAMC_Exit_Suspend(UINT32 *u4DRAMCTemp);
extern void DMARC_TCM_suspend(void);
extern void DMARC_TCM_resume(void);
extern void DMARC_TCM_partial_resume(void);
extern U32 DramcEngine2(DRAM_TE_OP_T wr, U32 test2_0, U32 test2_1, U32 test2_2);

/*
    DRAM configuration API for factory mode.
        res_mngr\drv\u_drv_cust.h
        mw_if\drv_cust_api.c
*/ 
#if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER)
extern BOOL DDR_SetCustCfg(DRAM_CFG_T *prDdrCfg);
extern BOOL DDR_GetCustCfg(DRAM_CFG_T *prDdrCfg);
#endif /* !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */

// DRAM parameters
UINT32 DRAMGetParameter(UINT8 u1Bank, UINT8 u1Idx);
BOOL DRAMSetParameter(UINT8 u1Bank, UINT8 u1Idx, UINT32 u4Value);
BOOL DRAM_StoreParams(UINT32 u4Addr);

#endif /* X_DRAM_H */
