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
/** @file ddr.h
 *  Dram configurations and options.
 */

#ifndef DDR_H
#define DDR_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "c_model.h"
#include "../../drv_cust/drv_default.h"
#include "drvcust_if.h"
#include "x_typedef.h"
#include "x_dram.h"

LINT_EXT_HEADER_END

//lint --e{717} do ... while (0);

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#ifdef NDEBUG
// DRAM debug mode.
//#undef NDEBUG
#endif

#ifndef NDEBUG
//#define DRAM_DEBUG
//#define DRAM_DEBUG_MSG
//#define DRAM_DEBUG_MSG2
#endif /* NDEBUG */

#define mcSHOW_ERROR_CHIP_DisplayString 		CHIP_DisplayString
#define mcSHOW_ERROR_CHIP_DisplayInteger(x)		CHIP_DisplayInteger(x)
#define mcSHOW_ERROR_CHIP_DisplayHex(x)			CHIP_DisplayHex(x)

#ifdef DRAM_DEBUG_MSG
#define mcSHOW_DBG_MSG 	Printf
#else
#define mcSHOW_DBG_MSG(fmt...)
#endif

#ifdef DRAM_DEBUG_MSG2
#define mcSHOW_DBG_MSG2	Printf
#else
#define mcSHOW_DBG_MSG2(fmt...)	
#endif

//#define DRAM_SLT_DEBUG   /* Use for SLT and Nand boot debug DRAM loader */
#define DRAM_Pll_PHASE_CAL
#define DRAM_IMPEDANCE_CAL 
#define DRAM_WRITE_LEVELING_CAL
#define DRAM_MiockJmeter
#define DRAM_GATING_SCAN
#define DRAM_RX_DATLAT_CAL
#define DRAM_RX_WINDOW_PERBIT_CAL
#define DRAM_TX_WINDOW_PERBIT_CAL
#ifndef DRAM_TX_PERBIT_DQ_DESKEW
#define DRAM_TX_PERBIT_DQ_DESKEW
#endif
#ifdef DRAM_TX_PERBIT_DQ_DESKEW
#define DRAM_TX_PERBIT_DQM_DESKEW
#endif
//#define DRAM_CAPRI_MCM_CFG
//#define DRAM_RX_ODT_SCAN
//#define DRAM_TX_VREF_CAL
//#define DRAM_TX_OCD_DRV_SCAN
//#define DRAM_RX_EYE_SCAN
//#define DRAM_TX_EYE_SCAN
//#define DRAM_LOAD_BOARD
//#define DRAM_WRITE_READ_LOOP_AFTER_CALIBRATION
//#define DDR_ASYMMETRIC_DRAM_CONFIG
//init
#define SR_VALUE 5   // slew rate
#define SR_VALUE_CLK 0xf // slew rate of CLK

#ifndef DEFAULT_OCDP_DRIVING
#define DEFAULT_OCDP_DRIVING 0x17
#endif 
#ifndef DEFAULT_OCDN_DRIVING
#define DEFAULT_OCDN_DRIVING 0x17
#endif
#ifndef DEFAULT_ODTP_DRIVING
#define DEFAULT_ODTP_DRIVING 0x17
#endif
#ifndef DEFAULT_ODTN_DRIVING
#define DEFAULT_ODTN_DRIVING 0x17
#endif
#ifndef DEFAULT_OCDP_DRIVING_CLK
#define DEFAULT_OCDP_DRIVING_CLK 0xb8
#endif
#ifndef DEFAULT_OCDN_DRIVING_CLK
#define DEFAULT_OCDN_DRIVING_CLK 0xb8
#endif

//tx vref define
#ifndef DEFAULT_TX_VREF
#define DEFAULT_TX_VREF 8
#endif

#ifndef DEFAULT_MR1_VALUE
#define DEFAULT_MR1_VALUE 0x00002004
#endif
#ifndef DEFAULT_MR2_VALUE
#define DEFAULT_MR2_VALUE 0x00004020
#endif
//default perbyte DQ delay
#ifndef DEFAULT_PI_A1_DQA
#define DEFAULT_PI_A1_DQA 0x10
#endif
#ifndef DEFAULT_PI_A1_DQB
#define DEFAULT_PI_A1_DQB DEFAULT_PI_A1_DQA
#endif
#ifndef DEFAULT_PI_A2_DQA
#define DEFAULT_PI_A2_DQA DEFAULT_PI_A1_DQA
#endif
#ifndef DEFAULT_PI_A2_DQB
#define DEFAULT_PI_A2_DQB DEFAULT_PI_A1_DQA
#endif
#ifndef DEFAULT_PI_B1_DQA
#define DEFAULT_PI_B1_DQA 0x10
#endif
#ifndef DEFAULT_PI_B1_DQB
#define DEFAULT_PI_B1_DQB DEFAULT_PI_B1_DQA
#endif
#ifndef DEFAULT_PI_B2_DQA
#define DEFAULT_PI_B2_DQA DEFAULT_PI_B1_DQA
#endif
#ifndef DEFAULT_PI_B2_DQB
#define DEFAULT_PI_B2_DQB DEFAULT_PI_B1_DQA
#endif

#define DEFAULT_TEST2_1_DQSIEN 0x55000000   // pattern0 and base address for test engine when we do dqs gating window
#define DEFAULT_TEST2_2_DQSIEN 0xaa000010   // pattern1 and offset address for test engine when we  do dqs gating window
#define DEFAULT_GOLD_DQSIEN 0x20202020   // gold pattern for dqsien compare
#define DEFAULT_DRAM_MODE MODE_2X

// PLL phase calibration
#define PLL_PHASE_CAL_CONF_COUNT 16

// gating window
#define DLY_DQSIENSTB_LOOP 7   // coarse tune 2T loop
#define DLY_DQSIENSTB_MAX 7   // max is 7, may reduce for time saving
#define RX_DQS_CTL_LOOP 4    // 4*0.5T = 2T
#define RX_DQS_CTL_MAX 12    // may no need to use this
#define RX_DLY_DQSIENSTB_MAX 63   // max fine tune value

#define DQS_NUMBER 4
#define DQ_DATA_WIDTH 32
#define DQS_BIT_NUMBER (DQ_DATA_WIDTH/DQS_NUMBER)

// RX DQ/DQS
#define MAX_RX_DQSDLY_TAPS 32   // 0x018, 0~63 delay tap
#define MAX_RX_DQDLY_TAPS 16      // 0x210~0x22c, 0~15 delay tap

// DATLAT
#define DATLAT_TAP_NUMBER 16   // DATLAT[3:0] = {0x0e4[4] 0x07c[6:4]}
#define RECORD_DQS_REG             (IO_VIRT + 0x8100)

#define CLK_1866MHZ    1866000000
#define CLK_1700MHZ    1700000000
#define CLK_1600MHZ    1600000000
#define CLK_1333MHZ    1333000000
#define CLK_1242MHZ    1242000000
#define CLK_1188MHZ    1188000000
#define CLK_1080MHZ    1080000000
#define CLK_1066MHZ    1066000000
#define CLK_972MHZ      972000000
#define CLK_810MHZ      810000000
#define CLK_800MHZ      800000000

#define DRAM_CHANGE_CLK_SETTING						1
#define DRAM_GEN_TEST_PATTERN_AFTER_CALIBRATION		2
#define DRAM_CHANGE_CLK_PI_SETTING					3
#define DRAM_CHANGE_CLK_SCRAMBLE_SETTING			4

#define CC_CHA_CHB_NO_GAP

#ifdef DRAM_LOAD_BOARD
//Step
#define FLAG_INITIAL 				0
#define FLAG_WRITE_LEVELING			1
#define FLAG_GATING_CLIBRATION 		2
#define FLAG_RX_DQS_CALIBRATION 	3
#define FLAG_TX_DQS_CALIBRATION		4
//Error Type
#define	FLAG_CALIBRATION_PASS					0
#define FLAG_WINDOW_TOO_SMALL		1
#define FLAG_WINDOW_TOO_BIG			2
#define FLAG_CALIBRATION_FAIL		3
//Complete flag
#define FLAG_NOT_COMPLETE_OR_FAIL	0
#define FLAG_COMPLETE_AND_PASS		1
#endif


/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
#if !defined(__MODEL_slt__) || defined(DRAM_SLT_DEBUG)
EXTERN void CLK_QueryDramSetting(UINT8 *szString, UINT32 u4Stage);
#else
#define CLK_QueryDramSetting(x, y)
#endif

/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
// basic function
void DdrPhyInit(void);
void DramcInit(void);
void DramcConfig(void);
void DramcDqDriving(U8 ocd_pvalue, U8 ocd_nvalue, U8 odt_pvalue, U8 odt_nvalue);
void DramcDqsDriving(U8 ocd_pvalue, U8 ocd_nvalue, U8 odt_pvalue, U8 odt_nvalue);
void DramcDqmDriving(U8 ocd_pvalue, U8 ocd_nvalue, U8 odt_pvalue, U8 odt_nvalue);
void DramcClkDriving(U8 ocd_pvalue, U8 ocd_nvalue);
void DramcCaDriving(U8 ocd_pvalue, U8 ocd_nvalue);
//U32 DramcEngine1(U32 test2_0, U32 test2_1, U32 test2_2, S16 loopforever, U8 period);
DRAM_STATUS_T DramcRegDump(void);
void LoadBoardGpioInit(void);
void LoadBoardShowResult(U8 step, U8 error_type, U8 complete);

// mandatory calibration function
void DramcPllPhaseCal(void);
void DramcImpedanceCalTxOcd(void);
void DramcImpedanceCalRxOdt(void);
void DramcImpedanceCalApply(void);
void DramcWriteLeveling(void);
void DramcMiockJmeter(void);
DRAM_STATUS_T DramcRxdqsGatingCal(void);
DRAM_STATUS_T DramcRxWindowPerbitCal(U8 only_get_bitwindowsum);
DRAM_STATUS_T DramcRxdatlatCal(void);
DRAM_STATUS_T DramcTxWindowPerbitCal(U8 only_get_bitwindowsum);
void DramcTxVrefCal(void);
void DramcTxOcdDrvScan(void);
void DramcRxOdtScan(void);

// reference function
DRAM_STATUS_T DramcRxEyeScan(U8 dq_no);
DRAM_STATUS_T DramcTxEyeScan(void);
DRAM_STATUS_T DramcDqsJmeter(U8 dqs_no);

extern void CHIP_Delay_us(UINT32 u4Micros);
extern void CHIP_DisplayString(const CHAR *szString);
extern void CHIP_DisplayInteger(INT32 i);
extern void CHIP_DisplayHex(UINT32 u4Val);
#endif // _DDR_H

