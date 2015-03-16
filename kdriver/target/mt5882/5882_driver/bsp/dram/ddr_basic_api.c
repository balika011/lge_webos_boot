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
 * $Date: 2015/03/16 $
 * $RCSfile: pi_basic_api.c,v $
 * $Revision: #8 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_basic_api.c
 *  Basic DRAMC API implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "drvcust_if.h"
#include "x_printf.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_bim.h"

#include "ddr.h"
#include "x_dram.h"
#include "x_os.h"

LINT_EXT_HEADER_END

#ifdef CC_MTK_PRELOADER
#define fcMEMCTLPLL_PWD_ISSUE
#define fcSET_AXI_CLK_USE_CLK_DRAM 1 //[important]!!! must set, otherwise BIM can't access dram

//-------------------------------------------------------------------------
/** DdrPhyInit
 *  DDR PHY Initialization.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DdrPhyInit(void)
{
    U16 u2frequency1, u2real_freq;
    U32 u4frequency_hex;
    U8 fgvcocal_cplt, ucvco_state, ucvco_state_ab;
    U32 u4value;
	U32 u4clock;
	//U8 ucloop_count = 0;
	U32 u4mempll_prediv_hex = 0;
	U32 u4mempll_predivider[3] = {1, 2, 4};	//caution: u4mempll_predivider = 2^u4mempll_prediv_hex;
	U32 u4ssc_prd, u4ssc_delta;
	U32 u4Permillage = 0;
    U32 u4Frequency = 0;
	U32 ucloop_count=0;	
	U32 u4feed_div_int=0,u4feed_div_fraction=0,u4real_freq_fraction=0;

	u4clock = (TCMGET_DDR_CLK()/BASE_DDR_CLK)/2;
	u2frequency1 = u4clock / 2;
    u4frequency_hex = (U32) (u2frequency1/(XTAL_MHZ/u4mempll_predivider[u4mempll_prediv_hex]));

    u4frequency_hex = u4frequency_hex - 1;
    
    //  bit8 to 0  we just need [7:0]
    u4frequency_hex =u4frequency_hex&0x000000ff;
    u2real_freq = (U16) ((u4frequency_hex+1)*(XTAL_MHZ/u4mempll_predivider[u4mempll_prediv_hex])*2);
	
	//MEMPLL fractional setting...
	u4feed_div_int=(u4clock/2)/(XTAL_MHZ);
	u4feed_div_fraction=(u4clock/2-u4feed_div_int*XTAL_MHZ)*16/24;
	mcSHOW_DBG_MSG("u4feed_div_int =%d, u4feed_div_fraction =%d\n", u4feed_div_int, u4feed_div_fraction);
	
	u4real_freq_fraction = (U32)u2real_freq + ((U32)u2frequency1-(u4frequency_hex+1)*XTAL_MHZ)/u4mempll_predivider[u4mempll_prediv_hex]*2;
		
    mcSHOW_DBG_MSG("Real frequency from PLL is %d MHz\n", u4real_freq_fraction);
	
	UNUSED(u4real_freq_fraction);

	//PHY initial settings (based on DR simulation settings, from Justin Chan)
	//-A1 part
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x000), 0x00000000);
	//--2012/10/03, for 2133MHz
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x008), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x00c), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x010), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x014), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x018), 0x00000000);
	//--2012/10/03, for 2133MHz	
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x024), 0x00000000);
	//--2012/10/03, for 2133MHz	
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x030), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x034), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x038), 0x00000008);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x03c), 0x00000008);
#ifdef DEFAULT_DDR_CHA_BUS_X8
	//RG_*_RX_BYTE_SWAP=1・b1
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x040), 0x0000006e);
	//RG_*_RX_DQSI_SEL=16・hf0f0
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x044), 0xF0F00100);
#else
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x040), 0x0000002e);
	//--modified by benson 1018, change DQS sel setting [31:16]
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x044), 0xFF000100);
#endif
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x048), 0x8000e008|(DEFAULT_TX_VREF<<4));
	//--2012/10/12, Alcuary, SCAN_IN_BUF off [31:16] for power saving
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x04c), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x050), 0x00600000|(SR_VALUE_CLK<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x054), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x058), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x05c), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x060), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x064), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x068), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x06c), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x070), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x074), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x078), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x07c), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x080), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x084), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x088), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x08c), 0x00200f00|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x090), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x094), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x098), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x09c), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0a0), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0a4), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0a8), 0x00200f00|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0ac), 0x00200f00|(SR_VALUE<<16));
	//--RG_TX_ARCS_CSBEN, [22]=1 for differential CS
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0b0), 0x00600000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0b4), 0x00200000|(SR_VALUE<<16));
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0b8), 0x00200000|(SR_VALUE<<16));
	//RG_TX_CMDA_EN=0 for power saving, SP, 2012/10/29
    //ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0bc), 0x00600000|(SR_VALUE<<16));
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0bc), 0x00600000|(SR_VALUE<<16)|(1<<15));
	//--PLL registers will be set later
	//...0x10c4~0x10cc
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0c0), 0x00000002);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d0), (DEFAULT_PI_A1_DQA<<24)|(DEFAULT_PI_A1_DQB<<8));
	//only for channel A, clk pi delay 14*T/64
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d4), 0x3d00001d);
	//--RG_MEMPHYPLL_A1_TEST_EN (10D8H[4]) may be set later?, use default values here
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d8), 0x00230000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0dc), 0x00000000);
	//--RG_PHYPLL_A1_TOP_REV (10E0H[31:16]) may be set later?, use default values here
	//--set OCD / ODT default values to 45ohm / 120ohm
	//--[31:28] active cap off for power consumtion, 2012/10/12, from SP
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0e0), 0xf0001717);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0e4), 0x0000b8b8);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0e8), 0x00000300);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0ec), 0x0000001e);
    if(TCMGET_DDR_CLK() > CLK_1700MHZ)
	{
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x004), 0x00400000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x01c), 0x01500000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x020), 0x00000150|(SR_VALUE));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x028), 0x00400000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x02c), 0x00000040|(SR_VALUE));
	}
	else
	{
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x004), 0x00500000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x01c), 0x01400000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x020), 0x00000140|(SR_VALUE));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x028), 0x00500000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x02c), 0x00000050|(SR_VALUE));
	}
	

	//AB part
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x200), 0x00000000);
	//2012/10/03, for 2133MHz
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x208), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x20c), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x210), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x214), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x218), 0x00000000);
	//2012/10/03, for 2133MHz	
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x224), 0x00000000);
	//2012/10/03, for 2133MHz	
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x230), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x234), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x238), 0x00000008);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x23c), 0x00000008);
#ifdef DEFAULT_DDR_CHA_BUS_X8
	//RG_*_RX_BYTE_SWAP=1・b1
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x240), 0x0000006e);
	//RG_*_RX_DQSI_SEL=16・hf0f0
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x244), 0xf0f00100);
#else
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x240), 0x0000002e);
	//modified by benson 1018, change DQS sel setting
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x244), 0xff000100);
#endif
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x248), 0x8000e008|(DEFAULT_TX_VREF<<4));
	//2012/10/12, Alcuary, SCAN_IN_BUF off [31:16]
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x24c), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x250), 0x00600000|(SR_VALUE_CLK<<16));
	//RG_MEMPLL_* (12A0H[15:0]), will be set later, use default values here
	//2012/10/2, MEMPLL to 2X mode
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a0), 0x0060a050|(SR_VALUE_CLK<<16));
	//PLL registers will be set later (12a4~12c4)
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2bc), 0x00008000);
	//RG_MEMPHYPLL_AB_*_DL (12C8H[20:16], 12C8H[28:24]), may be set later, use default values here
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), DEFAULT_PI_A2_DQA<<8);
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2cc), 0x00003d00|(DEFAULT_PI_A2_DQB<<24));
	//RG_MEMPHYPLL_AB_TEST_EN (12D0[12]), may be set later, use default values here
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2d0), 0x23000000);
	//RG_PHYPLL_A2_TOP_REV (12D4[31:16]), may be set later, use default values here
	//[31:30] active cap off for power consumtion, 2012/10/12, from SP
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2d4), 0xc0000000|(DEFAULT_PI_B1_DQA<<8));
	//PLL registers will be set later, RG_RSTB18V & RG_DMSUS18V?? (ignored from ACD/SP)
	//[31:30] active cap off for power consumtion, 2012/10/12, from SP
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2e0), 0xc0000600);

    if(TCMGET_DDR_CLK() > CLK_1700MHZ)
	{
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x204), 0x00400000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x21c), 0x01500000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x220), 0x00000150|(SR_VALUE));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x228), 0x00400000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x22c), 0x00000040|(SR_VALUE));
	}
	else
	{
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x204), 0x00500000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x21c), 0x01400000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x220), 0x00000140|(SR_VALUE));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x228), 0x00500000|(SR_VALUE<<16));
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x22c), 0x00000050|(SR_VALUE));
	}

	// ch-A, ???
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0f0), 0x80806500);
	
	//PHY Wrapper part
	//RG_DDRPHY_RESETB (1A4CH[31]), will be set later, use default values here
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xa4c), 0x03440055);
	//to solve 1st data DQS voltage too high issue (from Chaowei), 20121115
	//reg_dly_ODT(0x1a50[28:27])=00
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xa50), 0x08002803);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xa54), 0x01100051);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xa58), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xa5c), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xa60), 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xa64), 0x00000000);
	//R_DMSYNCRST (1B44H[6]), will be set later, use default values here
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xb44), 0x00080300);

	//For MT5399, the default value is 32・h0000_f220 to disable PHY input buffer.
	//Enable it here
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xb94), 0x00000110);

	//wait 1us, 1ms here
	mcDELAY_us(1);

	//TX serializer RSTB, RG_TX_CMDA_RSTB (10BCH[14]) toggle from 0 to 1 (default is 0)
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0bc));
	mcSET_BIT(u4value, 14);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0bc), u4value);
	//DDRPHY clock engine mute, RG_DDRPHY_RESETB (1A4CH[31]) can be released (0->1)
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0xa4c));
	mcSET_BIT(u4value, 31);
	//mcSET_FIELD(u4value, 0x0, 0x03000000, 24);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xa4c), u4value);

	//wait 1us, 1ms here
	mcDELAY_us(1);

	//u4value = ucDram_Register_Read(0xf000d210);
	//mcCLR_BIT(u4value, 6);
	//ucDram_Register_Write(0xf000d210, u4value);

	//MEMPLL initialization sequence (refer to A60807 verification plan)	

#ifdef fcMEMCTLPLL_PWD_ISSUE
    // Move MEMCTLPLL init here; due to MEMPLL bias current controlled by MEMCTLPLL PWD
    //RG_MEMCTLPLL_AUTOK_LOAD (12B4H[0]) set to 0
    //RG_MEMCTLPLL_AUTOK_VCO (12B4[1]) set to 0
	//MEMCTLPLL setting (BW)
	//RG_MEMCTLPLL_DIVEN (12B4H[30:28])
	//RG_MEMCTLPLL_BC     (12B4H[20:19]=11)
	//RG_MEMCTLPLL_BIC    (12B4H[18:16]=010)
	//RG_MEMCTLPLL_BIR    (12B4H[27:24]=0010)
	//RG_MEMCTLPLL_BP     (12B4H[15:12]=0001)
	//RG_MEMCTLPLL_BR     (12B4H[23:21]=100)
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2b4), 0x029a1880);

	//delay 1us, 1ms here
	mcDELAY_us(1);

    // move to below
	//RG_MEMCTLPLL_PWD (12B0H[15]) from 1 to 0
	//u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2b0));
	//mcCLR_BIT(u4value, 15);
	//ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2b0), u4value);

	//delay 100us, 1ms here
	//mcDELAY_us(100);
#endif

	//T1:
	//RG_MEMPLL_BIAS_PWD             (12A8H[25] = 1 -> 0)
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a8), 0x8400200c);
	//RG_MEMPLL_AUTOK_LOAD           (12A4H[0]  = 0 -> 1)
	//recommend by ACD/SP, 20120810
	//RG_MEMPLL_BC     (12A4H[20:19]=11)
	//RG_MEMPLL_BIC    (12A4H[18:16]=010)
	//RG_MEMPLL_BIR    (12A4H[27:24]=0010)
	//RG_MEMPLL_BP     (12A4H[15:12]=0001)
	//RG_MEMPLL_BR     (12A4H[23:21]=100)
	//PLL setting...
	//RG_MEMPLL_PREDIV (12A0H[1:0] = 00)
	//RG_MEMPLL_POSDIV (12A0H[5:4] = 00)
	//RG_MEMPLL_FBSEL  (12A0H[7:6] = 00)
	//RG_MEMPLL_FBDIV  (12A0H[14:8])
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a0));
	mcSET_FIELD(u4value, u4frequency_hex, 0x00007f00, 8);
	mcSET_FIELD(u4value, u4mempll_prediv_hex, 0x00000003, 0);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a0), u4value);
	//20121005, from SP
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a4), 0x0d997883);

	//RG_MEMPLL_REV (2A8H[23:20])
	//set this value as MEMPLL fraction
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a8));
	mcSET_FIELD(u4value, u4feed_div_fraction, 0x00f00000, 20);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a8), u4value);

	//DDS setting...
	//RG_MEMPLL_PCW_NCPO (12A8H[14:8])
	//set this value as MEMPLL FBDIV
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a8));
	mcSET_FIELD(u4value, u4frequency_hex, 0x00007f00, 8);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a8), u4value);

	//Adjust SSC amplitude and frequency
	//DELTA1 (0x12B0[23:16])
	//DELTA (0x12B0[31:24])
	//PRD (0x12AC[15:0])
	//SSC_REV (0x12AC[23:16])
	//from A60806
	//DDS SSC setting
	//RG_SSC_PRD= (Fout/Fmod/2)=(24M/30K/2)=400
	//RG_SSC_DELTA=(Fin/Fout)*2A/(Fout/Fmod/2)*2^18
	//                       =(408M/24M)*0.5%*2/400*2^18=112
	    /* Get Spectrum Permillage. */
    u4Permillage = DRVCUST_InitGet(eDmpllSpectrumPermillage);
	u4Frequency = DRVCUST_InitGet(eDmpllSpectrumFrequency);

	if ((DRVCUST_InitGet(eDmpllSpectrumPermillage) != 0) &&
         (DRVCUST_InitGet(eDmpllSpectrumFrequency) != 0))
	{
		u4ssc_prd = (((XTAL_MHZ/u4mempll_predivider[u4mempll_prediv_hex])*1000)/u4Frequency)/2;
		u4ssc_delta = ((u2frequency1*262144)/XTAL_MHZ)*u4Permillage*2/(10000*u4ssc_prd);
		mcSHOW_DBG_MSG("u4ssc_prd =%2d, u4ssc_delta =%2d\n", u4ssc_prd, u4ssc_delta);
	}
	else
	{	
		u4ssc_prd = 400;
		u4ssc_delta = 112;
	}
    // RG_MEMPLL_SSC_PHINI = 1 and RG_MEMPLL_SSC_DELTA1=RG_MEMPLL_SSC_DELTA for down-spread (default)
    // RG_MEMPLL_SSC_PHINI = 1 and RG_MEMPLL_SSC_DELTA1=RG_MEMPLL_SSC_DELTA/2 for center-spread
    // RG_MEMPLL_SSC_PHINI = 0 for up-spread
#ifdef fcMEMCTLPLL_PWD_ISSUE
    //u4value = 0x00000000|((u4ssc_delta/2)<<16)|(u4ssc_delta<<24);
    u4value = 0x00000000|(u4ssc_delta<<16)|(u4ssc_delta<<24);
#else
    //u4value = 0x00008000|((u4ssc_delta/2)<<16)|(u4ssc_delta<<24);
    u4value = 0x00008000|(u4ssc_delta<<16)|(u4ssc_delta<<24);
#endif
	
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2b0), u4value);
	u4value = 0x10ff0000|(u4ssc_prd);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2ac), u4value);

	//delay 10us, 1ms here
	mcDELAY_us(10);

	//T2
	//RG_MEMPLL_PWD (12A0H[15]) from 1 to 0
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a0));
	mcCLR_BIT(u4value, 15);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a0), u4value);

	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2e8));
	while(mcCHK_BIT1(u4value, 25) == 0)
	{
	    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2e8));
	    mcDELAY_us(CMP_CPT_POLLING_PERIOD);
	    ucloop_count++;
	    if (ucloop_count > 10000)
	    {
	        mcSHOW_ERROR_CHIP_DisplayString("MEMPLL VCOCAL cplt flag polling timeout.\n");
	        break;
	    }
	}

	//check RGS_MEMPLL_VCOCAL_CPLT (12E8H[25]) & RGS_MEMPLL_VCO_STATE (12E8H[31:26])
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2e8));
	fgvcocal_cplt = (U8) mcGET_FIELD(u4value, 0x02000000, 25);
	ucvco_state = (U8) mcGET_FIELD(u4value, 0xfc000000, 26);
	mcSHOW_DBG_MSG("MEMPLL VCOCAL CPLT FLAG = %d; VCO STATE = %d\n", fgvcocal_cplt, ucvco_state);

	//T3
	//RG_MEMPLL_BIAS_RST (12A8H[26]) from 1 to 0 (after VCO band K cplt)
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a8));
	mcCLR_BIT(u4value, 26);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a8), u4value);

	//MEMPLL ready -->

	//MEMPHYPLL initialization sequence (refer to A60807 verification plan)	
	//T0
	//RG_MEMPHYPLL_A1_BIAS_PWD (10C8H[9]) from 1 to 0
	//RG_MEMPHYPLL_B2_BIAS_PWD (14C8H[9]) from 1 to 0
	//RG_MEMPHYPLL_AB_BIAS_PWD (12C4H[25]) from 1 to 0
	//RG_PI_A1_EN (10D8H[17]) from 0 to 1, has set @ PHY initial settings
	//RG_PI_A2_EN (12D0H[25]) from 0 to 1, has set @ PHY initial settings
	//RG_PI_B1_EN (12DCH[25]) from 0 to 1, has set @ PHY initial settings
	//RG_PI_B2_EN (14D8H[17]) from 0 to 1, has set @ PHY initial settings

	//T1
	//RG_MEMPHYPLL_A1_AUTOK_LOAD (10C8[16]) from 0 to 1
	//RG_MEMPHYPLL_B2_AUTOK_LOAD (14C8[16]) from 0 to 1
	//RG_MEMPHYPLL_AB_AUTOK_LOAD (12C0[0]) from 0 to 1
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0c8), 0x18838400);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c0), 0x029a1883);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c4), 0x84000001);

	//PLL settings (BW...)
	//recommend by ACD/SP, 20120810
	//RG_MEMPHYPLL_A1_BC     (10C4H[5:4]=11)
	//RG_MEMPHYPLL_A1_BIC    (10C4H[3:1]=010)
	//RG_MEMPHYPLL_A1_BIR    (10C4H[12:9]=0010)
	//RG_MEMPHYPLL_A1_BP     (10C8H[31:28]=0001)
	//RG_MEMPHYPLL_A1_BR     (10C4H[8:6]=100)
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0c4), 0x00000534);
	//10c8H has set above


	//recommend by ACD/SP, 20120810
	//RG_MEMPHYPLL_AB_BC     (12C0H[20:19]=11)
	//RG_MEMPHYPLL_AB_BIC    (12C0H[18:16]=010)
	//RG_MEMPHYPLL_AB_BIR    (12C0H[27:24]=0010)
	//RG_MEMPHYPLL_AB_BP     (12C0H[15:12]=0001)
	//RG_MEMPHYPLL_AB_BR     (12C0H[23:21]=100)
	//has set above

	//delay 1us, 1ms here
	mcDELAY_us(1);

	//T2
	//(RG_MEMPHYPLL_A1_EN? not found) RG_MEMPHYPLL_A1_PWD (10C0H[1]) from 1 to 0
	//(RG_MEMPHYPLL_B2_EN? not found) RG_MEMPHYPLL_B2_PWD (14C0H[1]) from 1 to 0
	//(RG_MEMPHYPLL_AB_EN? not found) RG_MEMPHYPLL_AB_PWD (12BCH[15]) from 1 to 0
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0c0));
	mcCLR_BIT(u4value, 1);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0c0), u4value);
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2bc));
	mcCLR_BIT(u4value, 15);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2bc), u4value);

	//delay 100us, 1ms here
	mcDELAY_us(100);

	//check RGS_MEMPHYPLL_A1_VCOCAL_CPLT (10ecH[25]) & RGS_MEMPHYPLL_A1_VCO_STATE (10ecH[31:26])
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0f4));
	fgvcocal_cplt = (U8) mcGET_FIELD(u4value, 0x02000000, 25);
	ucvco_state = (U8) mcGET_FIELD(u4value, 0xfc000000, 26);
	mcSHOW_DBG_MSG("MEMPHYPLL A1 VCOCAL CPLT FLAG = %d; VCO STATE = %d\n", fgvcocal_cplt, ucvco_state);
	//check RGS_MEMPHYPLL_AB_VCOCAL_CPLT (12E8H[7]) & RGS_MEMPHYPLL_AB_VCO_STATE (12E8H[13:8])
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2e8));
	fgvcocal_cplt = (U8) mcGET_FIELD(u4value, 0x00000080, 7);
	ucvco_state_ab = (U8) mcGET_FIELD(u4value, 0x00003f00, 8);
	mcSHOW_DBG_MSG("MEMPHYPLL AB VCOCAL CPLT FLAG = %d; VCO STATE = %d\n", fgvcocal_cplt, ucvco_state_ab);

	//T4
	//RG_MEMPHYPLL_A1_BIAS_RST (10C8H[10]) from 1 to 0
	//RG_MEMPHYPLL_B2_BIAS_RST (14C8H[10]) from 1 to 0
	//RG_MEMPHYPLL_AB_BIAS_RST (12C4H[26]) from 1 to 0 (suggestion from ACD/SP, after VCO band K cplt)
	//RG_MEMPHYPLL_A1_AUTOK_VCO (10C8H[17]) from 1 to 0 (not to do here from SP)
	//RG_MEMPHYPLL_B2_AUTOK_VCO (14C8H[17]) from 1 to 0 (not to do here from SP)
	//RG_MEMPHYPLL_AB_AUTOK_VCO (12C0H[1]) from 1 to 0 (not to do here from SP)
	//BIAS_RST 1->0
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0c8));
	mcCLR_BIT(u4value, 10);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0c8), u4value);
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c4));
	mcCLR_BIT(u4value, 26);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c4), u4value);

	//PHY Clock ready -->	


#ifndef fcMEMCTLPLL_PWD_ISSUE
	//MEMCTLPLL initialization sequence (refer to A60807 verification plan)
	//delay 1us, no need to delay, believe it is enough for MEMPLL 
	//T1
	//RG_MEMCTLPLL_AUTOK_LOAD (12B4H[0]) from 0 to 1
	//MEMCTLPLL setting (BW)
	//RG_MEMCTLPLL_DIVEN (12B4H[30:28])
	//RG_MEMCTLPLL_BC     (12B4H[20:19]=11)
	//RG_MEMCTLPLL_BIC    (12B4H[18:16]=010)
	//RG_MEMCTLPLL_BIR    (12B4H[27:24]=0010)
	//RG_MEMCTLPLL_BP     (12B4H[15:12]=0001)
	//RG_MEMCTLPLL_BR     (12B4H[23:21]=100)
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2b4), 0x029a1883);

	//delay 1us, 1ms here
	mcDELAY_us(1);

	//T2
	//RG_MEMCTLPLL_PWD (12B0H[15]) from 1 to 0
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2b0));
	mcCLR_BIT(u4value, 15);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2b0), u4value);

	//delay 100us, 1ms here
	mcDELAY_us(100);

	//T3

	//T4
	//RG_MEMCTLPLL_AUTOK_VCO (12B4H[1]) from 1 to 0
#else
    //Assign MEMPHYPLL_AB VCO state to MEMCTLPLL
    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2b4));
	mcSET_FIELD(u4value, ucvco_state_ab, 0x000000fc, 2);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2b4), u4value);		

	mcDELAY_us(100);
#endif

    //check RGS_MEMCTLPLL_VCOCAL_CPLT (12E8H[17]) & RGS_MEMCTLPLL_VCO_STATE (12E8H[23:18])
	u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2e8));
	fgvcocal_cplt = (U8) mcGET_FIELD(u4value, 0x00020000, 17);
	ucvco_state = (U8) mcGET_FIELD(u4value, 0x00fc0000, 18);
	mcSHOW_DBG_MSG("MEMCTLPLL VCOCAL CPLT FLAG = %d; VCO STATE = %d\n", fgvcocal_cplt, ucvco_state);

	//MEMCTLPLL Ready -->	
	
	mcDELAY_us(100);
	

#if 1 // SSC off @ bring up
	//enable ssc
	if (IS_DDR_DMSSON())
	{
		//Enable SSC flow
		//Disable all first
		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a8));
		u4value = u4value & 0xffff7f0f;
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a8), u4value);

		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2ac));
		mcCLR_BIT(u4value, 29);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2ac), u4value);

		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a4));
		mcCLR_BIT(u4value, 8);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a4), u4value);

		//RG_MEMPLL_DDS_PWDB (12A8H[15]) from 0 to 1
		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a8));
		mcSET_BIT(u4value, 15);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a8), u4value);

		mcDELAY_us(100);

		//RG_MEMPLL_DDS_RSTB (12A8H[6]) from 0 to 1
		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a8));
		mcSET_BIT(u4value, 6);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a8), u4value);

		mcDELAY_us(100);

		//RG_MEMPLL_PCW_NCPO_CHG (12A8H[5]) from 0 to 1
		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a8));
		mcSET_BIT(u4value, 5);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a8), u4value);

		mcDELAY_us(100);

		//RG_MEMPLL_FIFO_START_MAN (12A8H[4]) from 0 to 1
		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a8));
		mcSET_BIT(u4value, 4);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a8), u4value);

		mcDELAY_us(100);

		//RG_MEMPLL_NCPO_EN (12A8H[7]) from 0 to 1
		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a8));
		mcSET_BIT(u4value, 7);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a8), u4value);

		mcDELAY_us(100);

		//RG_MEMPLL_SSC_EN (12ACH[29]) from 0 to 1 (DDS SSC enable). 20us after RG_MEMPLL_BIAS_RST from FNPLL AN (It is enough here)
		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2ac));
		mcSET_BIT(u4value, 29);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2ac), u4value);

		mcDELAY_us(100);

		//RG_MEMPLL_DDSEN (12A4H[8]) from 0 to 1 (DDS Feedback Enable)
		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a4));
		mcSET_BIT(u4value, 8);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2a4), u4value);
	}
#endif

	mcDELAY_us(100);

	//RX RESET for channel A & B, R_DMPHYRST (*0F0H[28]) to 1 for at least 100ns and then set it back to 0
	//Async FIFO RESET, R_DMSYNCRST (1B44H[6]) to 1 for at least 100ns and then set it back to 0
	ucDram_Register_Write(0xf00070f0, 0x10000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xb44), 0x00080340);
	//delay 100ns, 1ms here
	mcDELAY_us(1);
	ucDram_Register_Write(0xf00070f0, 0x00000000);
	ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xb44), 0x00080300);

	UNUSED(fgvcocal_cplt);
	UNUSED(ucvco_state);

    // log example
    /*
Real frequency from PLL is 918 MHz
MEMPLL VCOCAL CPLT FLAG = 1; VCO STATE = 13
MEMPHYPLL A1 VCOCAL CPLT FLAG = 1; VCO STATE = 11
MEMPHYPLL B2 VCOCAL CPLT FLAG = 1; VCO STATE = 11
MEMPHYPLL AB VCOCAL CPLT FLAG = 1; VCO STATE = 11
MEMCTLPLL VCOCAL CPLT FLAG = 1; VCO STATE = 10
   */
#if fcSET_AXI_CLK_USE_CLK_DRAM // JC ? need to check CKGEN register, de-glitch toggle? NO, auto in ckgen when switch
    /*[important]!!! change axi_clk to clk_dram after mempll enable, otherwise can't BIM access dram */
    mcDELAY_us(1);
    /*u4value = ucDram_Register_Read(0xf0000010);
    mcSET_BIT(u4value, 27);
    ucDram_Register_Write(0xf0000010, u4value);*/
    u4value = ucDram_Register_Read(0xf000d210);
    mcCLR_BIT(u4value, 6);
    ucDram_Register_Write(0xf000d210, u4value);
    mcDELAY_us(1);
#endif
}

//-------------------------------------------------------------------------
/** DramcInit
 *  DRAMC Initialization.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcInit(void)
{
	U32 u4value;
	// This function is implemented based on DE's bring up flow for DRAMC
	//for CS1=CS0 in capri
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 0x00001000);
	
	// DDR3 in channel A
	//========dramc_init============
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), 0x0000110d);

	//DDR3 SBS pinmux should be set to 00 (reg0xd8[31:30])        

#ifdef DRAM_CAPRI_MCM_CFG
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), 0x80100900);
#else
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), 0x00100900);
#endif

	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), 0x000000b2);
	// GDDR3RST must keep HIGH > 500us (1ms here)
	mcDELAY_us(500);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CLK1DELAY), 0x00000001);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_IOCTL), 0x00000000);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSIEN), 0x80000000);

	//A60806: 0x0dc=0x83000000
	//[23:12] DQS1 input range control, 1 hot encoding
	//[11:0]  DQS0 input range control, 1 hot encoding
	//gating window coarse tune default value
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL0), 0x83008008);

	//A60806: 0x0e0=0x12000000
	//[23:12] DQS3 input range control, 1 hot encoding
	//[11:0]  DQS2 input range control, 1 hot encoding
	//[26:24] DQS input range control by M_CK
	//gating window coarse tune default value
	//by KS
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCTL1), 0x10008008);

	//A60806: 0x0f0=0x80000000
	//[31] DQ 4-bit multiplex for DDR3
	//no 4-bit swap for A60807

#if defined(DEFAULT_DDR_CHA_BUS_X8)
		//R_DMDQ4BMUX=1・b1
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), 0x80000000);
		//R_DMDQMSWAP=1・b1(MT5399 only)
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), 0x81000000);
#else

#ifdef DRAM_CAPRI_MCM_CFG
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), 0x80000000);
#else
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), 0x00000000);
#endif
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), 0x01000000);
#endif

	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ARBCTL0), 0x00000080);

	//A60806: reg_duty_free_sel_cmd (not set)
	//A60807: DRAM CLOCK ENABLE CONTROL, register change
	//Useless for A60807, no connection from DRAMC to DDRPHY
	//#proc_write 0x2000${DRAMC_ADDR}130 0x30000000

	//A60806: dly_sel_mux2to1 (not set)
	//A60807: INPUT DQS GATING CONTROL, register change
	//Useless for A60807
	//#proc_write 0x2000${DRAMC_ADDR}124 0x80000033

	//A60806: 0x094=0x80000000
	//DQS INPUT RANGE FINE TUNER
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSIEN), 0x40404040);

	//A60806: chA, 0x1c0=0x0f000000 (default=0x00000000)
	//DQS CAL CONTROL 0
	//[31] DQS strobe calibration enable
	//[28:24] RA output delay chain setting for bit14
	//[15] DQS strobe calibration high-limit enable
	//[14:8] DQS strobe calibration high-limit value
	//[7] DQS strobe calibration low-limit enable
	//[6:0] DQS strobe calibration low-limit value
	//Useless for A60807?
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQSCAL0), 0x8000c8b8);

	//new register, Write Leveling
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), 0x00000000);

	//new register, DQ/DQM/DQS selph
	//2012/10/03, for 2133MHz
    if(TCMGET_DDR_CLK() > CLK_1700MHZ)
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH), 0x36dc07c0);
	else
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SELPH), 0x349b07c0);
	
	//new register, (blank)
	//DC balance by benson
	if (IS_DDR_DCBALANCEON())
	{
	    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), 0x00003c02);
	}
	else
	{
	    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), 0x00000c02);
	}
	//caustion, delay 100us to charge the caps after enable dc-balance
	mcDELAY_us(100);

	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DLLCONF), 0xf1200f01);

	//A60806: 0x1e0=0x88000000
	//[31] DRAM address decode
	//[30] Select IO O1 as output
	//[29] DDR mode for A[14] pin (LPDDR2 DDR command rate)
	//[26] Fast IO output enable
	//For A60807, bit 31 is 1, bit 30 is useless, bit 29 is useless, bit 26 is useless
	//Will set below
	//#proc_write 0x2000${DRAMC_ADDR}1e0 0x6c000000

	//A60806: 0x110=0b051111
	//[27] ?
	//[26:24] Rank address selection
	//[20] ?
	//[19:18] cross rank timing W2W
	//[11] ?
	//[7] Per-bank refresh enable for LPDDR2?
	//[2:0] Multi-rank mode support? Set to non-zero for multi-rank
	//Useless for A60807
	//#proc_write 0x2000${DRAMC_ADDR}110 0x00111990

	//A60806: 0x158=0x0ff00ff0
	//not in register map
	//may be 4-bit swap. TBD
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_158), 0x00000000);
	
	//========dram_init_start==========
	//[15] CKE function enabling
	//[10] BL4
	//[0] DM64BITEN
	//[15] -> When set to MRS, make sure that we don't toggle CKE.
	//2012/11/06, for suspend / resume
	if (IS_DDR_SUSPENDSTATE())
	{
	    //in suspend state, self refresh mode
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), 0xf47402c4);        
	}
	else
	{
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), 0xf07402c4);
	}

	//[2] CKE always ON
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), 0x000000b6);

	//set mode registers, delay is to wait 50 XTAL cycles, 1ms here
	//MR2
	//A60806: 4018
	//[5:3] CWL (CAS Write Latency)
	//depend on data rate
	//2012/10/03, for 2133MHz, CWL=101 (10)
    if(TCMGET_DDR_CLK() > CLK_1700MHZ)            
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), DEFAULT_MR2_VALUE);
	else
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), 0x4018);
           
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000001);
	mcDELAY_us(1);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000000);

	//MR3
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), 0x00006000);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000001);
	mcDELAY_us(1);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000000);

	//MR1
	//60ohm, 2012/10/02, by KS
	//for dynamic ODT: RTT_Nom = OFF, MR1[9,6,2]=000, 2013/1/3
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), DEFAULT_MR1_VALUE);
          
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000001);
	mcDELAY_us(1);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000000);

	//MR0
	//A60806: 0d71
	//[11:9] WR
	//[6:4] CAS Latency
	//[2] CL
	//depend on data rate
	//only for 2400MHz, CL = 0101 (14)
	
    if(TCMGET_DDR_CLK() > CLK_1700MHZ)
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), 0x00000f15);
	else
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), 0x00000d71);

	//ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), 0x00000f25);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000001);
	mcDELAY_us(1);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000000);

	//Enable ZQ calibration (A10=1 ZQCL, A10=0 ZQCS)
	//0x088[10] represents A10
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), 0x00000400);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000010);
	mcDELAY_us(1);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000000);

	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00001100);

	//[2] CKE control by HW
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), 0x000000b6);

	//A60806: 0x1e0=0x88000000
	//[31] DRAM address decode
	//[30] Select IO O1 as output
	//[29] DDR mode for A[14] pin (LPDDR2 DDR command rate)
	//[26] Fast IO output enable
	//For A60807, bit 31 is 0, bit 30 is useless, bit 29 is useless, bit 26 is useless
	//2012/09/27, for test chip, bit 31 is 0 for TE/TA/UART; for SoC it is 1
	//2012/10/03, the same as A60806, for TA&UART b'31=1; for TE b'31=0
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2), 0x88000000);

	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), 0x0000ffff);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000020);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), 0x00000000);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), 0x10622842);

	//For A60807, disable power down function, CKE will keep high (b15)
	//So we don't enable b15 here. Ignored. b10 & b0 are set in dramc_config()
	//#proc_write 0x2000${DRAMC_ADDR}004 0xf07486e3			

	//========dram_init_end============
	//A60806: 0xff000000
	//[31:28] CS1 signal output delay
	//[27:24] DRAM clock signal output delay
	//based on simulation
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL1), 0x00000000);

	//A60806: 0x55fc47eb
	//Timing settings
	//See register map for detail
	//depend on data rate. Excel table for AC timing calculation provided by Justin
	//2012/10/03, for 2133MHz
    if(TCMGET_DDR_CLK() > CLK_1700MHZ)
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM0), 0x55bc48ab);
	else
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM0), 0x55dc489b);

	//A60806: 0x28820400
	//[19:16] tRFC Timing setting
	//[3:0] Test loop number of test agent2
	//depend on data rate. Excel table for AC timing calculation provided by Justin
	//2012/10/03, for 2133MHz
    if(TCMGET_DDR_CLK() > CLK_1700MHZ)
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), 0x28880401);
	else
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), 0x28800401);

	//A60806: 0x00000c20
	//[15:8] tRFCPB Timing setting
	//[7:4] tRFC Timing setting for bit 7 ~ 4
	//depend on data rate. Excel table for AC timing calculation provided by Justin
	//2012/10/03, for 2133MHz
    if(TCMGET_DDR_CLK() > CLK_1700MHZ)
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), 0x00000660);
	else
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_ACTIM1), 0x00000660);
	//A60806: 0x03047960
	//[26:24] Refresh threshold value for promoting refresh request to high-priority
	//[7:0] Refresh period = (REFCNT * 16) DRAMC clock cycles
	//depend on capacity/size of DRAM. Refresh period spec.
	//2012/10/03, for 2133MHz
    if(TCMGET_DDR_CLK() > CLK_1700MHZ)
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), 0x00047967);
	else
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), 0x00047960); // DDR-1600

	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL2), 0x00000000);
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL7), 0xedcb000f);

	//A60806: 0x27010000
	//[30:28] tXP Timing setting
	//depend on data rate. Excel table for AC timing calculation provided by Justin
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MISCTL0), 0x37010000);

	//
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_RKCFG), 0x00051100);
	
	//enable scramble bit[28] and wdatkey=bit{[31:30], [27:26], [23:22],[19:18]}=0x56
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_OCDK), 0x54480000);

	//========DMARB_init_start===========
	//not in A60806?
	//OK. DRAM arbiter
	//CHSEL=0x5, channel A 1Gbytes
    // JC, check base address and arbiter settings (from ShengCheng Deng)
    ucDram_Register_Write(mcSET_ARBITER_ADDR(0x0), 0xe0501f00);
	u4value=ucDram_Register_Read(mcSET_ARBITER_ADDR(0x6c));
	mcSET_BIT(u4value, 7);
	ucDram_Register_Write(mcSET_ARBITER_ADDR(0x6c), u4value);

    //Wait until tZQinit (max(512nCK,640ns)) is satisfied
    mcDELAY_us(1);
}


//-------------------------------------------------------------------------
/** DramcConfig
 *  DRAM configuration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param data_width      (DRAM_DATA_WIDTH_T): data width
 *  @param dram_mode        (DRAM_DRAM_MODE_T): 1x or 2x mode
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcConfig(void)
{
// This function may be modified by different design. Need to review with DE
/*   
     mode                 R_DM64BITEN,R_DMFREQDIV2,R_DM16BITFULL  R_DMBL4
     LPDDR2-1066 32bit 2x      1,           1,          0            1
     DDR3-1600   16bit 2x      0,           1,          0            1
     DDR3-1600   32bit 2x      1,           1,          0            1
*/
    //DDR3 SBS pinmux should be set to 00 (reg0xd8[31:30])

#ifdef DRAM_CAPRI_MCM_CFG
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), 0x80900900);
#else
	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), 0x00900900);
#endif

    //0x4[9:8]  control the number of the column address, 
    //need set to differrnt value when use different DRAM,
    //current we set 2`b10, mean have 10 bit column address
    //A60806: 0x004=0xf07406c3
    //[15] CKE function enabling (0: disable power down function, CKE will keep high 1: enable power down function, CKE will go down when idle)
    //[5] ?
    //b15 set to 0 by A60807 (not concern power issue). May validate set to 1 case. b5 TBD
    //2012/11/06, for suspend / resume
     if (IS_DDR_SUSPENDSTATE())
    {
        //in suspend state, self refresh mode
            if(TCMGET_DDR_CLK() > CLK_1700MHZ)
            {
                ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), 0xf4748685);
            }
            else
            {
                ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), 0xf47486c5);
            }            
    }
    else
    {
            if(TCMGET_DDR_CLK() > CLK_1700MHZ)
            {
                ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), 0xf0748685);
            }
            else
            {
                ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), 0xf07486c5);
            }            
    }

    //change 0x7c[7:4] from 9 to a for A channel DDR3
    //A60806: chA 0x07c=0xd48733a1, chb/c 0x07c=0xd4873391
    //[30:28] Write latency = WLAT + 1/WLAT + 3 when FDIV2 = 0/1
    //[26:24] Read ODT timing control for DDR2 (000: For CL3 001: For CL4 and CL5 010: for CL6 and CL7)
    //[6:4] Internal read data timing control
    //[3] Write ODT enabling
    //[2] Read ODT enabling
    //[30:28] depend on data rate. Excel table for AC timing calculation provided by Justin
    //[26:24] depend on data rate. Excel table for AC timing calculation provided by Justin
    //[6:4] may depend on PCB. Calibration is required
    //[3] A60807 R/W ODT are enabled
    //2012/10/03, for 2133MHz, DATLAT e -> [6:4] = 110, tR2W [15:12] = 0100
    if(TCMGET_DDR_CLK() > CLK_1700MHZ)
    	ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), 0xe28743ed);	
	else
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), 0xe28733dd);
	
    //A60807 verification plan has no this setting? default is 0x00000000
    //[12] CS0 is also applied to CS1
    //A60807 no CS1 for fly by. No need to set for A60807
    //#proc_write 0x2000${DRAMC_ADDR}1ec 0x00001000

    //set default driving here
    //OCD: 60ohm, ODT: 120ohm, CLK OCD: 45ohm
    DramcDqDriving(DEFAULT_OCDP_DRIVING, DEFAULT_OCDN_DRIVING, DEFAULT_ODTP_DRIVING, DEFAULT_ODTN_DRIVING);
    DramcDqsDriving(DEFAULT_OCDP_DRIVING, DEFAULT_OCDN_DRIVING, DEFAULT_ODTP_DRIVING, DEFAULT_ODTN_DRIVING);
    DramcDqmDriving(DEFAULT_OCDP_DRIVING, DEFAULT_OCDN_DRIVING, DEFAULT_ODTP_DRIVING, DEFAULT_ODTN_DRIVING);
    DramcClkDriving(DEFAULT_OCDP_DRIVING_CLK, DEFAULT_OCDN_DRIVING_CLK);
    DramcCaDriving(DEFAULT_OCDP_DRIVING, DEFAULT_OCDN_DRIVING);
}

//-------------------------------------------------------------------------
/** DramcDqDriving
 *  DRAMC DQ driving settings.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param ocd_pvalue       (U8): OCD P value
 *  @param ocd_nvalue       (U8): OCD N value
 *  @param odt_pvalue       (U8): ODT P value
 *  @param odt_nvalue       (U8): ODT N value
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcDqDriving(U8 ocd_pvalue, U8 ocd_nvalue, U8 odt_pvalue, U8 odt_nvalue)
{
    U32 u4value;

        //A1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x000));
        mcSET_FIELD(u4value, odt_pvalue, 0x000000ff, 0);
        mcSET_FIELD(u4value, ocd_pvalue, 0x0000ff00, 8);
        mcSET_FIELD(u4value, odt_nvalue, 0x00ff0000, 16);
        mcSET_FIELD(u4value, ocd_nvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x000), u4value);

        //A2
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x200));
        mcSET_FIELD(u4value, odt_pvalue, 0x000000ff, 0);
        mcSET_FIELD(u4value, ocd_pvalue, 0x0000ff00, 8);
        mcSET_FIELD(u4value, odt_nvalue, 0x00ff0000, 16);
        mcSET_FIELD(u4value, ocd_nvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x200), u4value);
}

//-------------------------------------------------------------------------
/** DramcDqsDriving
 *  DRAMC DQS driving settings.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param ocd_pvalue       (U8): OCD P value
 *  @param ocd_nvalue       (U8): OCD N value
 *  @param odt_pvalue       (U8): ODT P value
 *  @param odt_nvalue       (U8): ODT N value
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcDqsDriving(U8 ocd_pvalue, U8 ocd_nvalue, U8 odt_pvalue, U8 odt_nvalue)
{
    U32 u4value;
	

        //A1, DQS0
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x018));
        mcSET_FIELD(u4value, odt_pvalue, 0x000000ff, 0);
        mcSET_FIELD(u4value, ocd_pvalue, 0x0000ff00, 8);
        mcSET_FIELD(u4value, odt_nvalue, 0x00ff0000, 16);
        mcSET_FIELD(u4value, ocd_nvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x018), u4value);

        //A1, DQS1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x01c));
        mcSET_FIELD(u4value, odt_nvalue, 0x000000ff, 0);
        mcSET_FIELD(u4value, ocd_nvalue, 0x0000ff00, 8);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x01c), u4value);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x020));
        mcSET_FIELD(u4value, odt_pvalue, 0x00ff0000, 16);
        mcSET_FIELD(u4value, ocd_pvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x020), u4value);

        //A2, DQS2
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x218));
        mcSET_FIELD(u4value, odt_pvalue, 0x000000ff, 0);
        mcSET_FIELD(u4value, ocd_pvalue, 0x0000ff00, 8);
        mcSET_FIELD(u4value, odt_nvalue, 0x00ff0000, 16);
        mcSET_FIELD(u4value, ocd_nvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x218), u4value);

        //A2, DQS3
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x21c));
        mcSET_FIELD(u4value, odt_nvalue, 0x000000ff, 0);
        mcSET_FIELD(u4value, ocd_nvalue, 0x0000ff00, 8);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x21c), u4value);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x220));
        mcSET_FIELD(u4value, odt_pvalue, 0x00ff0000, 16);
        mcSET_FIELD(u4value, ocd_pvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x220), u4value);

}

//-------------------------------------------------------------------------
/** DramcDqmDriving
 *  DRAMC DQM driving settings.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param ocd_pvalue       (U8): OCD P value
 *  @param ocd_nvalue       (U8): OCD N value
 *  @param odt_pvalue       (U8): ODT P value
 *  @param odt_nvalue       (U8): ODT N value
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcDqmDriving(U8 ocd_pvalue, U8 ocd_nvalue, U8 odt_pvalue, U8 odt_nvalue)
{
    U32 u4value;

        //A1, DQM0
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x024));
        mcSET_FIELD(u4value, odt_pvalue, 0x000000ff, 0);
        mcSET_FIELD(u4value, ocd_pvalue, 0x0000ff00, 8);
        mcSET_FIELD(u4value, odt_nvalue, 0x00ff0000, 16);
        mcSET_FIELD(u4value, ocd_nvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x024), u4value);

        //A1, DQM1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x028));
        mcSET_FIELD(u4value, odt_nvalue, 0x000000ff, 0);
        mcSET_FIELD(u4value, ocd_nvalue, 0x0000ff00, 8);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x028), u4value);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x02c));
        mcSET_FIELD(u4value, odt_pvalue, 0x00ff0000, 16);
        mcSET_FIELD(u4value, ocd_pvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x02c), u4value);

        //A2, DQM2
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x224));
        mcSET_FIELD(u4value, odt_pvalue, 0x000000ff, 0);
        mcSET_FIELD(u4value, ocd_pvalue, 0x0000ff00, 8);
        mcSET_FIELD(u4value, odt_nvalue, 0x00ff0000, 16);
        mcSET_FIELD(u4value, ocd_nvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x224), u4value);

        //A2, DQM3
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x228));
        mcSET_FIELD(u4value, odt_nvalue, 0x000000ff, 0);
        mcSET_FIELD(u4value, ocd_nvalue, 0x0000ff00, 8);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x228), u4value);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x22c));
        mcSET_FIELD(u4value, odt_pvalue, 0x00ff0000, 16);
        mcSET_FIELD(u4value, ocd_pvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x22c), u4value);

}

//-------------------------------------------------------------------------
/** DramcClkDriving
 *  DRAMC CLK driving settings.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param ocd_pvalue       (U8): OCD P value
 *  @param ocd_nvalue       (U8): OCD N value
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcClkDriving(U8 ocd_pvalue, U8 ocd_nvalue)
{
    U32 u4value;
    
        //A1, CLK
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x04c));
        mcSET_FIELD(u4value, ocd_nvalue, 0x000000ff, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x04c), u4value);
        
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x050));
        mcSET_FIELD(u4value, ocd_pvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x050), u4value);

        //CMDACLK
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0b8));
        mcSET_FIELD(u4value, ocd_nvalue, 0x000000ff, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0b8), u4value);
        
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0bc));
        mcSET_FIELD(u4value, ocd_pvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0bc), u4value);

        //A2, CLK
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x24c));
        mcSET_FIELD(u4value, ocd_nvalue, 0x000000ff, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x24c), u4value);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x250));
        mcSET_FIELD(u4value, ocd_pvalue, 0xff000000, 24);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x250), u4value);

}

//-------------------------------------------------------------------------
/** DramcCaDriving
 *  DRAMC CA driving settings.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param ocd_pvalue       (U8): OCD P value
 *  @param ocd_nvalue       (U8): OCD N value
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcCaDriving(U8 ocd_pvalue, U8 ocd_nvalue)
{
    U8 i;
    U32 u4value, u4addr1, u4addr2;
    
    u4addr1 = mcSET_PHY_REG_ADDR(0x050);
    u4addr2 = mcSET_PHY_REG_ADDR(0x054);


    // ?RA0~14, ?RCKE, ?RBA0~2, ?RA15, ?RCAS, ?RRAS, ?RODT, ?RCS, ?RRESET, ?RWE
    for (i=0; i<=25; i++)
    {
        u4value = ucDram_Register_Read(u4addr1);
        mcSET_FIELD(u4value, ocd_nvalue, 0x000000ff, 0);
        ucDram_Register_Write(u4addr1, u4value);

        u4value = ucDram_Register_Read(u4addr2);
        mcSET_FIELD(u4value, ocd_pvalue, 0xff000000, 24);
        ucDram_Register_Write(u4addr2, u4value);

        u4addr1 +=4;
        u4addr2 +=4;        
    }
}


//-------------------------------------------------------------------------
/** DramcEngine1
 *  start the self test engine inside dramc to test dram w/r.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  test2_0         (U32): 16bits,set pattern1 [31:24] and set pattern0 [23:16].
 *  @param  test2_1         (U32): 28bits,base address[27:0].
 *  @param  test2_2         (U32): 28bits,offset address[27:0].
 *  @param  loopforever     (S16):  0 read\write one time ,then exit
 *                                 >0 enable eingie1, after "loopforever" second ,write log and exit
 *                                 -1 loop forever to read\write, every "period" seconds ,check result ,only when we find error,write log and exit
 *                                 -2 loop forever to read\write, every "period" seconds ,write log ,only when we find error,write log and exit
 *                                 -3 just enable loop forever ,then exit
 *  @param period           (U8):  it is valid only when loopforever <0; period should greater than 0
 *  @retval status          (U32): return the value of DM_CMP_ERR  ,0  is ok ,others mean  error
 */
//-------------------------------------------------------------------------
/*
U32 DramcEngine1(U32 test2_0, U32 test2_1, U32 test2_2, S16 loopforever, U8 period)
{
    // This function may not need to be modified unless test engine-1 design has changed

    U8 ucengine_status;
    U8 ucnumber;
    U32 u4value, u4result = 0xffffffff;
    U8 ucloop_count = 0;
      
    // we get the status 
    // loopforever    period    status    mean 
    //     0             x         1       read\write one time ,then exit ,don't write log 
    //    >0             x         2       read\write in a loop,after "loopforever" seconds ,disable it ,return the R\W status  
    //    -1            >0         3       read\write in a loop,every "period" seconds ,check result ,only when we find error,write log and exit 
    //    -2            >0         4       read\write in a loop,every "period" seconds ,write log ,only when we find error,write log and exit
    //    -3             x         5       just enable loop forever , then exit (so we should disable engine1 outside the function)
    if (loopforever == 0)
    {
        ucengine_status = 1;
    }
    else if (loopforever > 0)
    {
        ucengine_status = 2;
    }
    else if (loopforever == -1)
    {
        if (period > 0)
        {
            ucengine_status = 3;
        }
        else
        {
            mcSHOW_ERROR_CHIP_DisplayString(("parameter 'status' should be equal or greater than 0\n"));
            return u4result;
        }
    }
    else if (loopforever == -2)
    {
        if (period > 0)
        {
            ucengine_status = 4;
        }
        else
        {
            mcSHOW_ERROR_CHIP_DisplayString(("parameter 'status' should be equal or greater than 0\n"));
            return u4result;
        }
    }
    else if (loopforever == -3)
    {
        ucengine_status = 5;
    }
    else
    {
        mcSHOW_ERROR_CHIP_DisplayString(("wrong parameter!\n"));
        mcSHOW_ERROR_CHIP_DisplayString(("loopforever    period    status    mean \n"));
        mcSHOW_ERROR_CHIP_DisplayString(("      0                x           1         read/write one time ,then exit ,don't write log\n"));
        mcSHOW_ERROR_CHIP_DisplayString(("    >0                x           2         read/write in a loop,after [loopforever] seconds ,disable it ,return the R/W status\n"));
        mcSHOW_ERROR_CHIP_DisplayString(("    -1              >0           3         read/write in a loop,every [period] seconds ,check result ,only when we find error,write log and exit\n"));
        mcSHOW_ERROR_CHIP_DisplayString(("    -2              >0           4         read/write in a loop,every [period] seconds ,write log ,only when we find error,write log and exit\n"));
        mcSHOW_ERROR_CHIP_DisplayString(("    -3                x           5         just enable loop forever , then exit (so we should disable engine1 outside the function)\n"));
        return u4result;
    }

    // set ADRDECEN=0,address decode not by DRAMC
    //2012/10/03, the same as A60806, for TA&UART b'31=1; for TE b'31=0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2));
    mcCLR_BIT(u4value, POS_LPDDR2_ADRDECEN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2), u4value);

    // step
    // 1.select loop forever or not
    // 2.set pattern, base address,offset address
    // 3.enable test1 
    // 4.run different code according status
    // 5.diable test1
    // 6.return DM_CMP_ERR

    if (ucengine_status == 4)
    {
        mcSHOW_DBG_MSG(("============================================\n"));
        mcSHOW_DBG_MSG(("enable test egine1 loop forever\n"));        
        mcSHOW_DBG_MSG(("============================================\n"));
        ucnumber = 1;
    }

    // 1.
    if (loopforever != 0)
    {
        // enable infinite loop
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1));
        mcSET_BIT(u4value, POS_CONF1_TESTLP);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), u4value);
    }
    else
    {
        // disable infinite loop
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1));
        mcCLR_BIT(u4value, POS_CONF1_TESTLP);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), u4value);
    }
    // 2.
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_0), test2_0);    
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), test2_1);    
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), test2_2);    
    // 3.
    // enable test engine 1 (first write and then read)
    // disable it before enable ,DM_CMP_ERR may not be 0,because may be loopforever and don't disable it before
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
    mcCLR_BIT(u4value, POS_CONF2_TEST1);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);

    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
    mcSET_BIT(u4value, POS_CONF2_TEST1);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);
    // 4.
    if (ucengine_status == 1)
    {
        // read data compare ready check
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
        // infinite loop??? check DE about the time???
        ucloop_count = 0;
        while(mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_CPT) == 0)
        {            
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
            mcDELAY_us(CMP_CPT_POLLING_PERIOD);
            ucloop_count++;
            if (ucloop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
                mcSHOW_ERR_MSG(("TESTRPT_DM_CMP_CPT polling timeout\n"));
                break;
            }
        }

        // delay 10ns after ready check from DE suggestion (1ms here)
        mcDELAY_us(1);

        // save  DM_CMP_ERR, 0 is ok ,others are fail,disable test engine 1
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
        u4result = mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_ERR);
        mcSHOW_DBG_MSG(("0x3fc = %d\n", u4value));
    }
    else if (ucengine_status == 2)
    {
        // wait "loopforever" seconds
        mcDELAY_us(loopforever*1000);
        // get result, no need to check read data compare ready???
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
        u4result = mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_ERR);
    }
    else if (ucengine_status == 3)
    {
        while(1)
        {
            // wait "period" seconds
            mcDELAY_us(period*1000);
            // get result
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
            u4result = mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_ERR);
            if (u4result == 0)
            {
                // pass, continue to check
                continue;
            }
            // some bit error
            // write log
            mcSHOW_DBG_MSG(("%d#    CMP_ERR = 0x%8x\n", ucnumber, u4result));
            break;
        }
    }
    else if (ucengine_status == 4)
    {
        while(1)
        {
            // wait "period" seconds
            mcDELAY_us(period*1000);
            // get result
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
            u4result = mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_ERR);

            // write log
            mcSHOW_DBG_MSG(("%d#    CMP_ERR = 0x%8x\n", ucnumber, u4result));
            
            if (u4result == 0)
            {
                // pass, continue to check
                continue;
            }
            // some bit error            
            break;
        }
    }
    else if (ucengine_status == 5)
    {
        // loopforever is  enable ahead ,we just exit this function
        return 0;
    }
    else
    {
    }

    // 5. disable engine1
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
    mcCLR_BIT(u4value, POS_CONF2_TEST1);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);
    
    // 6. 
    // set ADRDECEN to 1
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2));
    mcSET_BIT(u4value, POS_LPDDR2_ADRDECEN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_LPDDR2), u4value);

	UNUSED(ucnumber);

    return u4result;
}
*/
//-------------------------------------------------------------------------
/** DramcEngine2
 *  start the self test engine 2 inside dramc to test dram w/r.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  wr              (DRAM_TE_OP_T): TE operation
 *  @param  test2_0         (U32): 16bits,set pattern1 [31:24] and set pattern0 [23:16].
 *  @param  test2_1         (U32): 28bits,base address[27:0].
 *  @param  test2_2         (U32): 28bits,offset address[27:0]. (unit is 16-byte, i.e: 0x100 is 0x1000).
 *  @param  loopforever     (S16): 0    read\write one time ,then exit
 *                                >0 enable eingie2, after "loopforever" second ,write log and exit
 *                                -1 loop forever to read\write, every "period" seconds ,check result ,only when we find error,write log and exit
 *                                -2 loop forever to read\write, every "period" seconds ,write log ,only when we find error,write log and exit
 *                                -3 just enable loop forever ,then exit
 *  @param period           (U8):  it is valid only when loopforever <0; period should greater than 0
 *  @param log2loopcount    (U8): test loop number of test agent2 loop number =2^(log2loopcount) ,0 one time
 *  @retval status          (U32): return the value of DM_CMP_ERR  ,0  is ok ,others mean  error
 */
//-------------------------------------------------------------------------
U32 DramcEngine2(DRAM_TE_OP_T wr, U32 test2_0, U32 test2_1, U32 test2_2)
{

    U8 ucloop_count = 0;
    U32 u4value, u4result = 0x0;

    // disable self test engine1 and self test engine2 
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
    mcCLR_MASK(u4value, MASK_CONF2_TE12_ENABLE);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);

    // 1.set pattern ,base address ,offset address
    // 2.select  ISI pattern or audio pattern
    // 3.set loop number
    // 4.enable read or write
    // 5.loop to check DM_CMP_CPT
    // 6.return CMP_ERR
    // currently only implement ucengine_status = 1, others are left for future extension    

    // 1
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_0), test2_0);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_1), test2_1);    
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_2), test2_2);    

    // 2 & 3
    //use audio pattern

    // set AUDINIT=0x11 AUDINC=0x0d AUDBITINV=1 AUDMODE=1
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4));
    mcSET_FIELD(u4value, 0x00000011, MASK_TEST2_4_TESTAUDINIT, POS_TEST2_4_TESTAUDINIT);
#if defined(DRAM_LOAD_BOARD) || defined(__MODEL_slt__) || defined(DRAM_WRITE_READ_LOOP_AFTER_CALIBRATION)
	mcSET_BIT(u4value, POS_TEST2_4_TEST2DISSCRAM);
#else
	mcCLR_BIT(u4value, POS_TEST2_4_TEST2DISSCRAM);
#endif
    mcSET_FIELD(u4value, 0x0000000d, MASK_TEST2_4_TESTAUDINC, POS_TEST2_4_TESTAUDINC);
    mcSET_BIT(u4value, POS_TEST2_4_TESTAUDBITINV);
    mcSET_BIT(u4value, POS_TEST2_4_TESTAUDMODE);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u4value);

    // set addr 0x044 [7] to 1 ,select audio pattern
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3));
    mcSET_BIT(u4value, POS_TEST2_3_TESTAUDPAT);
    mcSET_FIELD(u4value, 0, MASK_TEST2_3_TESTCNT, POS_TEST2_3_TESTCNT);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4value);

    // 4
    if (wr == TE_OP_READ_CHECK)
    {	
		// enable read, 0x008[31:29]
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
        mcSET_FIELD(u4value, (U32) 2, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);

		//if audio pattern, enable read only (disable write after read), AUDMODE=0x48[15]=0
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4));
        mcCLR_BIT(u4value, POS_TEST2_4_TESTAUDMODE);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u4value);
    }
    else if (wr == TE_OP_WRITE_READ_CHECK)
    {
        // enable write
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
        mcSET_FIELD(u4value, (U32) 4, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);

        // read data compare ready check
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
        ucloop_count = 0;
        while(mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_CPT) == 0)
        {
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
            mcDELAY_us(CMP_CPT_POLLING_PERIOD);
            ucloop_count++;
            if (ucloop_count > MAX_CMP_CPT_WAIT_LOOP)
            {
                mcSHOW_ERROR_CHIP_DisplayString("TESTRPT_DM_CMP_CPT polling timeout\n");
                break;
            }
        }
        
        // disable write
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
        mcSET_FIELD(u4value, (U32) 0, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);

        // enable read
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
        mcSET_FIELD(u4value, (U32) 2, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);
    }

    // 5
    // read data compare ready check
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));        
    ucloop_count = 0;
    while(mcCHK_BIT1(u4value, POS_TESTRPT_DM_CMP_CPT) == 0)
    {
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
        mcDELAY_us(CMP_CPT_POLLING_PERIOD);
        ucloop_count++;
        if (ucloop_count > MAX_CMP_CPT_WAIT_LOOP)
        {
            mcSHOW_ERROR_CHIP_DisplayString("TESTRPT_DM_CMP_CPT polling timeout\n");
            break;
        }
    }

    // delay 10ns after ready check from DE suggestion (1ms here)
    mcDELAY_us(1);

    // 6
    // return CMP_ERR, 0 is ok ,others are fail,diable test2w or test2r
    // get result
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR));
    // or all result
    u4result |= u4value;
    // disable read
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
    mcCLR_MASK(u4value, MASK_CONF2_TE12_ENABLE);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4value);

    return u4result;    
}

#ifdef DRAM_LOAD_BOARD
void LoadBoardGpioInit()
{
	//pinmux setting
	HAL_WRITE32(0xF000D608,HAL_READ32(0xF000D608) & 0xC03FFFFF); //pinmux setting [29:22]=0
	//output enable setting
	HAL_WRITE32(0xF000D720,HAL_READ32(0xF000D720) | 0x000001FF); //output enable setting [8:0]=1
	//output data
	HAL_WRITE32(0xF000D700,HAL_READ32(0xF000D700) & 0xFFFFFE00);	
}
void GpioOutput(U8 gpio_no, U8 low_high)
{
	if(low_high == 0)
	{
		HAL_WRITE32(0xF000D700,HAL_READ32(0xF000D700) & (~(1<<gpio_no)));
	}
	else
	{
		HAL_WRITE32(0xF000D700,HAL_READ32(0xF000D700) | (1<<gpio_no));
	}
}
void LoadBoardShowResult(U8 step,U8 error_type,U8 complete)
{
	CHIP_DisplayString("result: ");
	switch(complete)
	{
		case FLAG_NOT_COMPLETE_OR_FAIL:
			GpioOutput(5, 0);
			CHIP_DisplayString("0");
			break;
		case FLAG_COMPLETE_AND_PASS:
			GpioOutput(5, 1);
			CHIP_DisplayString("1");
			break;
		default:
			break;
	}
	switch(error_type)
	{
		case FLAG_CALIBRATION_PASS:
			GpioOutput(4, 0);
			GpioOutput(3, 0);
			CHIP_DisplayString("00");
			break;
		case FLAG_WINDOW_TOO_SMALL:
			GpioOutput(4, 0);
			GpioOutput(3, 1);
			CHIP_DisplayString("01");
			break;
		case FLAG_WINDOW_TOO_BIG:
			GpioOutput(4, 1);
			GpioOutput(3, 0);
			CHIP_DisplayString("10");
			break;
		case FLAG_CALIBRATION_FAIL:
			GpioOutput(4, 1);
			GpioOutput(3, 1);
			CHIP_DisplayString("11");
			break;
		default:
			break;
	}

	switch(step)
	{
		case FLAG_INITIAL:
			GpioOutput(2, 0);
			GpioOutput(1, 0);
			GpioOutput(0, 0);
			CHIP_DisplayString("000");
			break;
		case FLAG_WRITE_LEVELING:
			GpioOutput(2, 0);
			GpioOutput(1, 0);
			GpioOutput(0, 1);
			CHIP_DisplayString("001");
			break;
		case FLAG_GATING_CLIBRATION:
			GpioOutput(2, 0);
			GpioOutput(1, 1);
			GpioOutput(0, 0);
			CHIP_DisplayString("010");
			break;
		case FLAG_RX_DQS_CALIBRATION:
			GpioOutput(2, 0);
			GpioOutput(1, 1);
			GpioOutput(0, 1);
			CHIP_DisplayString("011");
			break;
		case FLAG_TX_DQS_CALIBRATION:
			GpioOutput(2, 1);
			GpioOutput(1, 0);
			GpioOutput(0, 0);
			CHIP_DisplayString("100");
			break;
		default:
			break;
	}

	CHIP_DisplayString("\n");
}
#endif

#ifdef CC_SUPPORT_STR_CORE_OFF
//-------------------------------------------------------------------------
/** DramcEnterSR
 *  DRAMC issue self refresh command to DRAM
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (U8): 0: OK, others: Fail
 */
//-------------------------------------------------------------------------
/*
U8 DramcWriteDataToDramBeforeEnterSR(void)
{
    U32 u4err_value;
	U32 test2_0 = DEFAULT_TEST2_0_CAL;
    U32 test2_1 = DEFAULT_TEST2_1_CAL;
    U32 test2_2 = DEFAULT_TEST2_2_CAL;

    //Write pre-defined data into dram (selftest2 in write mode)
    //u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2);
	u4err_value = 0;
	
    //Issue self-refresh command to dram
    //entry self refresh    4h [26]	
    //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1));
    //mcSET_BIT(u4value, POS_CONF1_SELFREF);
    //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), u4value);

    //Wait at least 9*tREFI+tCKSRE+20*tMIOCK (or 1ms)
    mcDELAY_us(1);

    mcSHOW_DBG_MSG("%d (2: cha, 3: chb) DRAMC enter self refresh...err_value=%8x\n", IS_DRAM_CHANNELB_ACTIVE(), u4err_value);

	UNUSED(u4err_value);

    return DRAM_OK;
}
*/
//-------------------------------------------------------------------------
/** DramcExitSR
 *  DRAMC issue exit self refresh command to DRAM
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (U8): 0: OK, others: Fail
 */
//-------------------------------------------------------------------------
U8 DramcExitSR(void)
{
    U32 u4value;
    U32 test2_0 = DEFAULT_TEST2_0_CAL;
    U32 test2_1 = DEFAULT_TEST2_1_CAL;
    U32 test2_2 = DEFAULT_TEST2_2_CAL;

    //Exit self-refresh command to dram
    //exit self refresh    4h [26]	
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1));
    mcCLR_BIT(u4value, POS_CONF1_SELFREF);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF1), u4value);

    //Wait at least 9*tREFI+tCKSRE+20*tMIOCK (or 1ms)
    mcDELAY_us(1);

    //reset phy R_DMPHYRST: 0xf0[28] 
    // 0x0f0[28] = 1 -> 0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1));
    mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);
    //delay 10ns, 1ms here
    mcDELAY_us(1);
    mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

    //Async FIFO RESET, R_DMSYNCRST 0x1b44[6]
    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0xb44));
    mcSET_BIT(u4value, 6);
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xb44), u4value);
    //delay 100ns, 1ms here
    mcDELAY_us(1);
    mcCLR_BIT(u4value, 6);
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xb44), u4value);

    // read data counter reset
    // 0x0f4[25] = 1 -> 0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1));
    mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
    //delay 10ns, 1ms here
    mcDELAY_us(1);                
    mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
    mcSHOW_DBG_MSG("fcSET_AXI_CLK_USE_CLK_DRAM\n");
#if fcSET_AXI_CLK_USE_CLK_DRAM
    /*[important]!!! change axi_clk to clk_dram after mempll enable, otherwise can't BIM access dram */
    /*u4value = ucDram_Register_Read(0xf0000010);
    mcSET_BIT(u4value, 27);
    ucDram_Register_Write(0xf0000010, u4value);*/
    u4value = ucDram_Register_Read(0xf000d210);
    mcCLR_BIT(u4value, 6);
    ucDram_Register_Write(0xf000d210, u4value);
    mcDELAY_us(1);
#endif

    mcSHOW_DBG_MSG("DRAM_RX_DATLAT_CAL\n");
#ifdef DRAM_RX_DATLAT_CAL 
	// For MT5399, add DATLAT calibration
    // Issue from MT6589, async fifo, core power off and on, DATLAT window may have 1T shift, 2012/11/28
    DramcRxdatlatCal();
#endif

    return DRAM_OK;
}

/*
void DramcBackupReg(UINT32 *u4DRAMCTemp)
{
    U8 ii;
    U32 u4value, u4addr, u4PHY_BASE_ADDR, u4PHY_BASE_ADDR1;
	// DRAMC backup registers
	U16 gu2dramc_backup_regaddr[DRAMC_BACKUP_REG_NUM] = {DRAMC_REG_CONF1, DRAMC_REG_CONF2, DRAMC_REG_TEST2_3, DRAMC_REG_DDR2CTL, DRAMC_REG_PADCTL4, \
                                                              DRAMC_REG_DQIDLY1, DRAMC_REG_DQIDLY2, DRAMC_REG_DQIDLY3, DRAMC_REG_DQIDLY4, DRAMC_REG_DQIDLY5, \
                                                              DRAMC_REG_DQIDLY6, DRAMC_REG_DQIDLY7, DRAMC_REG_DQIDLY8};
        
    //Backup all the VCCK domain registers
    if (!IS_DRAM_CHANNELB_ACTIVE())
    {
        u4PHY_BASE_ADDR = mcSET_PHY_REG_ADDR(0x000);
        u4PHY_BASE_ADDR1 = mcSET_PHY_REG_ADDR(0x200);
    }
    else
    {
        u4PHY_BASE_ADDR = mcSET_PHY_REG_ADDR(0x400);
        u4PHY_BASE_ADDR1 = mcSET_PHY_REG_ADDR(0x250);
    }
    
    //DRAMC registers
    for (ii=0; ii<DRAMC_BACKUP_REG_NUM; ii++)
    {
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(gu2dramc_backup_regaddr[ii]));
        *(u4DRAMCTemp+ii) = u4value;
    }

    //DDRPHY registers
    //---A1, B2 registers
    u4addr = u4PHY_BASE_ADDR + 0x00;
    for (ii = 0; ii <= 3; ii++)
    {
        u4value = ucDram_Register_Read(u4addr);
        *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM) = u4value;
        u4addr = u4addr + 4;
    }

    u4addr = u4PHY_BASE_ADDR + 0x18;
    for (ii = 4; ii <= 14; ii++)
    {
        u4value = ucDram_Register_Read(u4addr);
        *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM) = u4value;
        u4addr = u4addr + 4;
    }

    u4addr = u4PHY_BASE_ADDR + 0x4c;
    for (ii = 15; ii <= 43; ii++)
    {
        u4value = ucDram_Register_Read(u4addr);
        *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM) = u4value;
        u4addr = u4addr + 4;
    }

    u4addr = u4PHY_BASE_ADDR + 0xcc;
    for (ii = 44; ii <= 46; ii++)
    {
        u4value = ucDram_Register_Read(u4addr);
        *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM) = u4value;
        u4addr = u4addr + 4;
    }

    u4addr = u4PHY_BASE_ADDR + 0xe0;
    for (ii = 47; ii <= 48; ii++)
    {
        u4value = ucDram_Register_Read(u4addr);
        *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM) = u4value;
        u4addr = u4addr + 4;
    }

    //ii=49
    u4addr = u4PHY_BASE_ADDR + 0xf0;
    u4value = ucDram_Register_Read(u4addr);
    *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM) = u4value;

    //---A2, B1 and PLL registers
    //A2, B1 (A2+0x50) registers
    //0x00~0x0c
    u4addr = u4PHY_BASE_ADDR1 + 0x00;
    for (ii = 50; ii <= 53; ii++)
    {
        u4value = ucDram_Register_Read(u4addr);
        *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM) = u4value;
        u4addr = u4addr + 4;
    }

    //0x18~0x40
    u4addr = u4PHY_BASE_ADDR1 + 0x18;
    for (ii = 54; ii <= 64; ii++)
    {
        u4value = ucDram_Register_Read(u4addr);
        *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM) = u4value;
        u4addr = u4addr + 4;
    }

    //0x4c~0x50
    u4addr = u4PHY_BASE_ADDR1 + 0x4c;
    for (ii = 65; ii <= 66; ii++)
    {
        u4value = ucDram_Register_Read(u4addr);
        *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM) = u4value;
        u4addr = u4addr + 4;
    }

    //0xc8~0xcc (0xd4~0xd8)
    if (!IS_DRAM_CHANNELB_ACTIVE())
    {
        u4addr = mcSET_PHY_REG_ADDR(0x2c8);
    }
    else
    {
        u4addr = mcSET_PHY_REG_ADDR(0x2d4);
    }
    for (ii = 67; ii <= 68; ii++)
    {
        u4value = ucDram_Register_Read(u4addr);
        *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM) = u4value;
        u4addr = u4addr + 4;
    }

    //0xd4 (0xe0)
    //ii=69
    if (!IS_DRAM_CHANNELB_ACTIVE())
    {
        u4addr = mcSET_PHY_REG_ADDR(0x2d4);
    }
    else
    {
        u4addr = mcSET_PHY_REG_ADDR(0x2e0);
    }
    u4value = ucDram_Register_Read(u4addr);
    *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM) = u4value;    
}    

void DramcBackupCommonReg(UINT32 *u4DRAMCTemp)
{
	U8 ii;
	U32 u4value, u4addr;
	
	//PLL registers, regardless of channel
	//May execute twice for chA & chB
	u4addr = mcSET_PHY_REG_ADDR(0x2b8);
	for (ii = 0; ii <= 1; ii++)
	{
		u4value = ucDram_Register_Read(u4addr);
		*(u4DRAMCTemp+ii) = u4value;
		u4addr = u4addr + 4;
	 }

	ii=2;
	u4addr = mcSET_PHY_REG_ADDR(0x2c8);
	u4value = ucDram_Register_Read(u4addr);
	*(u4DRAMCTemp+ii) = u4value;

	//---DDRPHY wrapper registers
	ii=3;
	*(u4DRAMCTemp+ii) = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0xa4c));

	//Arbiter chsel 
	ii=4;
	*(u4DRAMCTemp+ii) = TCM_DRAM_SIZE;
}
*/
void DramcWriteBackReg(UINT32 *u4DRAMCTemp)
{
    U8 ii;
    U32 u4value, u4addr, u4PHY_BASE_ADDR, u4PHY_BASE_ADDR1;
	// DRAMC backup registers
	U16 gu2dramc_backup_regaddr[DRAMC_BACKUP_REG_NUM] = {DRAMC_REG_CONF2, DRAMC_REG_TEST2_3, DRAMC_REG_DDR2CTL, DRAMC_REG_PADCTL4, \
                                                              DRAMC_REG_DQIDLY1, DRAMC_REG_DQIDLY2, DRAMC_REG_DQIDLY3, DRAMC_REG_DQIDLY4, DRAMC_REG_DQIDLY5, \
                                                              DRAMC_REG_DQIDLY6, DRAMC_REG_DQIDLY7, DRAMC_REG_DQIDLY8};
        
    //restore all the VCCK domain registers
    u4PHY_BASE_ADDR = mcSET_PHY_REG_ADDR(0x000);
    u4PHY_BASE_ADDR1 = mcSET_PHY_REG_ADDR(0x200);

    
    //DRAMC registers
    for (ii=0; ii<DRAMC_BACKUP_REG_NUM; ii++)
    {
        u4value = *(u4DRAMCTemp+ii);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(gu2dramc_backup_regaddr[ii]), u4value);        
    }

    //DDRPHY registers
    //---A1, B2 registers
    u4addr = u4PHY_BASE_ADDR + 0x00;
    for (ii = 0; ii <= 3; ii++)
    {
        u4value = *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM);
        ucDram_Register_Write(u4addr, u4value);
        u4addr = u4addr + 4;
    }

    u4addr = u4PHY_BASE_ADDR + 0x18;
    for (ii = 4; ii <= 14; ii++)
    {
        u4value = *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM);
        ucDram_Register_Write(u4addr, u4value);        
        u4addr = u4addr + 4;
    }

    u4addr = u4PHY_BASE_ADDR + 0x4c;
    for (ii = 15; ii <= 43; ii++)
    {
        u4value = *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM);
        ucDram_Register_Write(u4addr, u4value);        
        u4addr = u4addr + 4;
    }

    u4addr = u4PHY_BASE_ADDR + 0xcc;
    for (ii = 44; ii <= 46; ii++)
    {
        u4value = *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM);
        ucDram_Register_Write(u4addr, u4value);        
        u4addr = u4addr + 4;
    }   

    u4addr = u4PHY_BASE_ADDR + 0xe0;
    for (ii = 47; ii <= 48; ii++)
    {
        u4value = *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM);
        ucDram_Register_Write(u4addr, u4value);        
        u4addr = u4addr + 4;
    }

    //ii=49
    u4addr = u4PHY_BASE_ADDR + 0xf0;
    u4value = *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM);
    ucDram_Register_Write(u4addr, u4value);    

    //---A2, B1 and PLL registers
    //A2, B1 (A2+0x50) registers
    //0x00~0x0c
    u4addr = u4PHY_BASE_ADDR1 + 0x00;
    for (ii = 50; ii <= 53; ii++)
    {
        u4value = *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM);
        ucDram_Register_Write(u4addr, u4value);
        u4addr = u4addr + 4;
    }

    //0x18~0x40
    u4addr = u4PHY_BASE_ADDR1 + 0x18;
    for (ii = 54; ii <= 64; ii++)
    {
        u4value = *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM);
        ucDram_Register_Write(u4addr, u4value);
        u4addr = u4addr + 4;
    }

    //0x4c~0x50
    u4addr = u4PHY_BASE_ADDR1 + 0x4c;
    for (ii = 65; ii <= 66; ii++)
    {
        u4value = *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM);
        ucDram_Register_Write(u4addr, u4value);
        u4addr = u4addr + 4;
    }

    //0xc8~0xcc (0xd4~0xd8)
    if (!IS_DRAM_CHANNELB_ACTIVE())
    {
        u4addr = mcSET_PHY_REG_ADDR(0x2c8);
    }
    else
    {
        u4addr = mcSET_PHY_REG_ADDR(0x2d4);
    }
    for (ii = 67; ii <= 68; ii++)
    {
        u4value = *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM);
        ucDram_Register_Write(u4addr, u4value);
        u4addr = u4addr + 4;
    }

    //0xd4 (0xe0)
    //ii=69

    u4addr = mcSET_PHY_REG_ADDR(0x2d4);

    u4value = *(u4DRAMCTemp+ii+DRAMC_BACKUP_REG_NUM);
    ucDram_Register_Write(u4addr, u4value);
}

void DramcWriteBackCommonReg(UINT32 *u4DRAMCTemp)
{
	U8 ii;
	U32 u4value, u4addr;
	//PLL registers, regardless of channel
    //May execute twice for chA & chB
    u4addr = mcSET_PHY_REG_ADDR(0x2b8);
    for (ii = 0; ii <= 1; ii++)
    {
        u4value = *(u4DRAMCTemp+ii);
        ucDram_Register_Write(u4addr, u4value);
        u4addr = u4addr + 4;
     }

    ii=2;
    u4addr = mcSET_PHY_REG_ADDR(0x2c8);
    u4value = *(u4DRAMCTemp+ii);
    ucDram_Register_Write(u4addr, u4value);
    
    //---DDRPHY wrapper registers
    ii=3;
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xa4c), *(u4DRAMCTemp+ii));

	//Arbiter chsel
	ii=4;
	TCM_DRAM_SIZE = *(u4DRAMCTemp+ii);

	//768MB resume setting
	u4value=(HAL_READ32(0xF000D748)>>12) & 0x1;
	if(u4value==0x1)
		{
			DRAM_WRITE32(0x04, DRAM_READ32(0x04)|0x00000002);//Enable Asymmetric mode
			DRAM_WRITE32(0x04, DRAM_READ32(0x04)|0x00000010);//Enable 4G+2G function
			//For CS/CSX async in dramc
			ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 0x00000000);
			mcDELAY_us(1);
			//16bit for Low address, 32bit for high address 
			DRAM_ARB_WRITE32(0x0, DRAM_ARB_READ32(0x0)|(1<<25));
		}

#if 0//defined(CC_CHA_CHB_NO_GAP)
    // Set CHSEL channel B according to channel A Mbytes.
    u4value = ucDram_Register_Read(mcSET_ARBITER_ADDR(0x0));
    u4value &= ~0x700000;
    if(TCMGET_CHANNELA_SIZE()==0x40)     
        ucDram_Register_Write(mcSET_ARBITER_ADDR(0x0), u4value |0x100000);//channel A 64Mbytes
    else if(TCMGET_CHANNELA_SIZE()==0x80)    
        ucDram_Register_Write(mcSET_ARBITER_ADDR(0x0), u4value |0x200000);//channel A 128Mbytes
    else if(TCMGET_CHANNELA_SIZE()==0x100)    
        ucDram_Register_Write(mcSET_ARBITER_ADDR(0x0), u4value |0x300000);//channel A 256Mbytes
    else if(TCMGET_CHANNELA_SIZE()==0x200)
        ucDram_Register_Write(mcSET_ARBITER_ADDR(0x0), u4value |0x400000);//channel A 512Mbytes
    else
        ucDram_Register_Write(mcSET_ARBITER_ADDR(0x0), u4value |0x500000);//channel A 1Gbytes
    #endif 	
}

//-------------------------------------------------------------------------
/** DramcSuspend
 *  Suspend DRAMC and DDRPHY
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
/*
UINT32 DRAMC_str_save_registers(UINT32 *u4DRAMCTemp, UINT32 u4Size)
{
	// enter self refresh
	TCMSET_CHANNELA_ACTIVE();
	DramcWriteDataToDramBeforeEnterSR();
	TCMSET_CHANNELB_ACTIVE();
	DramcWriteDataToDramBeforeEnterSR();
		
	// backup necessary registers
	TCMSET_CHANNELA_ACTIVE();
	DramcBackupReg(u4DRAMCTemp);
	TCMSET_CHANNELB_ACTIVE();
	DramcBackupReg(u4DRAMCTemp+(PHY_BACKUP_REG_NUM+DRAMC_BACKUP_REG_NUM));
	TCMSET_CHANNELA_ACTIVE();
	DramcBackupCommonReg(u4DRAMCTemp+(PHY_BACKUP_REG_NUM+DRAMC_BACKUP_REG_NUM)*2);
	
	// no suspend S1 state in MT5399
	TCMSET_DRAM_SUSPEND();

	// Set DMSUS33=1, for MT5399, it is GPIO control
	
	// RSTB33=0, for MT5399, it is GPIO control
	
	// Shut down VCCK, for MT5399, it is GPIO control
	
	// Shut down AVDD33, for MT5399, it is GPIO control

	return ((PHY_BACKUP_REG_NUM+DRAMC_BACKUP_REG_NUM)*2+CHA_CHB_COMMON_REG_NUM)*sizeof(UINT32);
   
}
*/
//-------------------------------------------------------------------------
/** DramcResume
 *  Suspend DRAMC and DDRPHY
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DRAMC_str_restore_registers(UINT32 *u4DRAMCTemp)
{
    // Turn on AVDD33, for MT5399, it is GPIO control

    // Turn on VCCK, for MT5399, it is GPIO control
 	
    // RSTB33=1, for MT5399, it is GPIO control

	//Current dram state is suspend
	TCMSET_DRAM_SUSPEND();
	
    // re-Init...(dramc_init, dramc_config)
    TCMSET_CHANNELA_ACTIVE();
    DramcInit();
	DramcConfig();
	DramcWriteBackReg(u4DRAMCTemp);
	DramcWriteBackCommonReg(u4DRAMCTemp+PHY_BACKUP_REG_NUM+DRAMC_BACKUP_REG_NUM);
}

void DRAMC_Exit_Suspend(UINT32 *u4DRAMCTemp)
{
    //Set DMSUS33=0 (here due to less impact to DRAM), for MT5399, it is GPIO control

    // exit self refresh
    TCMSET_CHANNELA_ACTIVE();
    DramcExitSR();
    //TCMSET_CHANNELB_ACTIVE();
    //DramcExitSR();
	
    TCMSET_DRAM_NORMAL();
}
#endif//#ifdef CC_SUPPORT_STR_CORE_OFF

#else//#ifdef CC_MTK_PRELOADER
#ifdef CC_SUPPORT_STR_CORE_OFF

static UINT32* pm_save_tcm_msg= NULL;

void DMARC_TCM_suspend(void)
{
	pm_save_tcm_msg = x_mem_alloc(8 * sizeof(UINT32));//
	mcSHOW_DBG_MSG("DMARC_TCM_suspend addr : 0x%x .\n",pm_save_tcm_msg);
	if (pm_save_tcm_msg!=NULL)
	{
		pm_save_tcm_msg[0] = TCM_DRAM_SIZE;
		pm_save_tcm_msg[1] = TCM_DRAM_FLAGS;
		pm_save_tcm_msg[2] = TCM_DRAM_FLAGS1;
		pm_save_tcm_msg[3] = TCM_DRAM_FLAGS2;
		pm_save_tcm_msg[4] = TCM_DRAM_FLAGS3;
		pm_save_tcm_msg[5] = TCM_DRAM_FLAGS4;
		pm_save_tcm_msg[6] = TCM_DRAM_DLYCELL_PERT;
	}
}

void DMARC_TCM_resume(void)
{
	if (pm_save_tcm_msg!=NULL)
	{
		TCM_DRAM_SIZE 			= pm_save_tcm_msg[0];
		TCM_DRAM_FLAGS 			= pm_save_tcm_msg[1];
		TCM_DRAM_FLAGS1 		= pm_save_tcm_msg[2];
		TCM_DRAM_FLAGS2 		= pm_save_tcm_msg[3];
		TCM_DRAM_FLAGS3			= pm_save_tcm_msg[4];
		TCM_DRAM_FLAGS4			= pm_save_tcm_msg[5];
		TCM_DRAM_DLYCELL_PERT	= pm_save_tcm_msg[6];
		x_mem_free(pm_save_tcm_msg);
	}
}
#endif//#ifdef CC_SUPPORT_STR_CORE_OFF
#endif//#ifdef CC_MTK_PRELOADER

#if 0
//-------------------------------------------------------------------------
/** DramcRegDump
 *  Dump all registers (DDRPHY and DRAMC)
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcRegDump()
{
    U16 ii;
    U32 u4addr, u4value;
        
    mcSHOW_DBG_MSG("\n=================mt5399 PLL/PHY register value================================");
    for (ii=0; ii<=0x0f4; ii=ii+4)
    {
        // confirm SoC platform has "%" operation
        if (ii%16==0)
        {
            mcSHOW_DBG_MSG("\n0x%8x | ", mcSET_PHY_REG_ADDR(0x000)+ii);
        }
        u4addr = mcSET_PHY_REG_ADDR(0x000)+ii;
        u4value = ucDram_Register_Read(u4addr);
        mcSHOW_DBG_MSG(" %08x", u4value);
    }

    for (ii=0x2a0; ii<=0x2e8; ii=ii+4)
    {
        // confirm SoC platform has "%" operation
        if (ii%16==0)
        {
            mcSHOW_DBG_MSG("\n0x%8x | ", mcSET_PHY_REG_ADDR(0x000)+ii);
        }
        u4addr = mcSET_PHY_REG_ADDR(0x000)+ii;
        u4value = ucDram_Register_Read(u4addr);
        mcSHOW_DBG_MSG(" %08x", u4value);
    }

    mcSHOW_DBG_MSG("\n=================mt8563 PLL Wrapper register value================================");
    for (ii=0xa40; ii<=0xbd0; ii=ii+4)
    {
        // confirm SoC platform has "%" operation
        if ((ii-0xa40)%16==0)
        {
            mcSHOW_DBG_MSG("\n0x%8x | ", mcSET_PHY_REG_ADDR(0x000)+ii);
        }
        u4addr = mcSET_PHY_REG_ADDR(0x000)+ii;
        u4value = ucDram_Register_Read(u4addr);
        mcSHOW_DBG_MSG(" %08x", u4value);
    }
    mcSHOW_DBG_MSG("\n=========mt5399 cha dramc register value===========================");
    for (ii=0; ii<0x400; ii=ii+4)
    {
        // confirm SoC platform has "%" operation
        if (ii%16==0)
        {
            mcSHOW_DBG_MSG("\n0x%8x | ", mcSET_DRAMC_REG_ADDR(ii));
        }
        u4addr = mcSET_DRAMC_REG_ADDR(ii);
        u4value = ucDram_Register_Read(u4addr);
        mcSHOW_DBG_MSG(" %08x", u4value);
    }		
	

	UNUSED(u4value);
	
	return DRAM_OK;

    // log example
    /*
    =================a60807 PLL/PHY register value================================
mcSET_PHY_REG_ADDR(0x000) |  80178017   550000        0        0        0        0
0x20001018 |  80178017  1458017 80170145 80178017   558017 80170055
0x20001030 |         0        0        2   900002   8e002e ff000100
0x20001048 |  803df088       b8 b86f0080 80250080 80250080 80250080
0x20001060 |  80250080 80250080 80250080 80250080 80250080 80250080
0x20001078 |  80250080 80250080 80250080 80250080 80250080 80250080
0x20001090 |  80250080 80250080 80250080 80250080 80250080 80250080
0x200010a8 |  80250080 80250080 80650080 80250080 802500b8 b8654000
0x200010c0 |         0      534 18838000    10003  d0b0d0b        0
0x200010d8 |    230000        0 f0001717     b8b8      300 2e800000
0x200010f0 |         0        0        0        0        0        0
0x20001108 |         0        0        0        0        0        0
0x20001120 |         0        0        0        0        0        0
0x20001138 |         0        0        0        0        0        0
0x20001150 |         0        0        0        0        0        0
0x20001168 |         0        0        0        0        0        0
0x20001180 |         0        0        0        0        0        0
0x20001198 |         0        0        0        0        0        0
0x200011b0 |         0        0        0        0        0        0
0x200011c8 |         0        0        0        0        0        0
0x200011e0 |         0        0        0        0        0        0
0x200011f8 |         0        0 80178017   550000        0        0
0x20001210 |         0        0 80178017  1458017 80170145 80178017
0x20001228 |    558017 80170055        0        0        2   8a0002
0x20001240 |    87002e ff000100 8000e088       b8 b86f8017 80170055
0x20001258 |         0        0        0        0 80178017  1458017
0x20001270 |  80170145 80178017   558017 80170055        0        0
0x20001288 |         2   980002   98002e ff000100 8000e088       b8
0x200012a0 |  b86f1050  d997883 8000100c 10ff01c2 57570000  29a1883
0x200012b8 |  80000100  2000000  29a1883 80000001    30c02  d000000
0x200012d0 |  23000000 c0000a0d  a0e0000 23000000 c0000600        0
0x200012e8 |  362a0ba0        0        0        0        0        0
0x20001300 |         0        0        0        0        0        0
0x20001318 |         0        0        0        0        0        0
0x20001330 |         0        0        0        0        0        0
0x20001348 |         0        0        0        0        0        0
0x20001360 |         0        0        0        0        0        0
0x20001378 |         0        0        0        0        0        0
0x20001390 |         0        0        0        0        0        0
0x200013a8 |         0        0        0        0        0        0
0x200013c0 |         0        0        0        0        0        0
0x200013d8 |         0        0        0        0        0        0
0x200013f0 |         0        0        0        0 80178017   550000
0x20001408 |         0        0        0        0 80178017  1458017
0x20001420 |  80170145 80178017   558017 80170055        0        0
0x20001438 |         2   8a0002   8b002e ff000100 8000e088       b8
0x20001450 |  b86f0080 80250080 80250080 80020080 80250080 80250080
0x20001468 |  80250080 80250080 80250080 80250080 80250080 80250080
0x20001480 |  80250080 80250080 80250080 80250080 80250080 80250080
0x20001498 |  80250080 80250080 80250080 80250080 80250080 80250080
0x200014b0 |  80650080 80250080 802500b8 b8654000        0      534
0x200014c8 |  18838000    10003  a090b08        0   230000        0
0x200014e0 |  f0001717     b8b8      300 2e800000        0
=================a60807 PLL Wrapper register value================================
0x20001a4c |  83446655     2800  1100051        0        0        0
0x20001a64 |         0        0        0        0        0        0
0x20001a7c |         0        0        0        0        0 b04c033e
0x20001a94 |         0        0        0        0        0        0
0x20001aac |         0        0        0        0        0        0
0x20001ac4 |         0        0        0        0        0        0
0x20001adc |         0        0        0        0        0        0
0x20001af4 |         0        0        0 aa220000        0 aa220000
0x20001b0c |         0 aa220000        0 aa220000        0        0
0x20001b24 |         0        0        0        0        0        0
0x20001b3c |         0   110000    80300        0        0        0
0x20001b54 |         0        0     1010        0        0        0
0x20001b6c |         0        0        0        0        0        0
0x20001b84 |         0        0        0        0      110
0x20001f94 |       110
=========a60807 dramc register value===========================
0x20002000 |  66fe49ff f07486e3    4794c        0        0        0
0x20002018 |         0        0        0        0 f1200f01 55010000
0x20002030 |  33000fff 55020000 33000fff 55000000 aa000400 28880480
0x20002048 |      d10d        0        0        0        0        0
0x20002060 |         0        0        0        0        0        0
0x20002078 |         0 e28743dd        0        0     2004        1
0x20002090 |         0 40404040        0        0        0        0
0x200020a8 |         0        0        0        0 aa22aa22 aa22aa22
0x200020c0 |         0        0        0        0        0        0
0x200020d8 |    100900 83008008 10008008       b2        0        0
0x200020f0 |         0  1000000 edcb000f 37010000        0        0
0x20002108 |         0        0  b051100        0        0        0
0x20002120 |         0 aa080088        0        0 50000000        0
0x20002138 |         0        0        0        0        0        0
0x20002150 |         0        0        0        0        0        0
0x20002168 |        80        0        0        0        0        0
0x20002180 |         0        0        0        0        0        0
0x20002198 |         0        0        0        0        0        0
0x200021b0 |         0        0        0        0 8000c8b8        0
0x200021c8 |         0        0        0        0   c80000 10622842
0x200021e0 |  88000000        0      690        0        0        0
0x200021f8 |         0        0        0        0        0        0
0x20002210 |   1020102  1020102  1020102  2010102  3010102        0
0x20002228 |   2030303  1030101        0        0        0        0
0x20002240 |         0        0        0        0        0        0
0x20002258 |         0        0        0        0        0        0
0x20002270 |         0        0        0        0        0        0
0x20002288 |         0        0        0        0        0        0
0x200022a0 |         0        0        0        0        0        0
0x200022b8 |         0        0        0        0        0        0
0x200022d0 |         0        0        0        0        0        0
0x200022e8 |         0        0        0        0        0        0
0x20002300 |         0        0        0        0        0        0
0x20002318 |         0        0 ffffffff ffffffff        0   d92fc3
0x20002330 |    d92fc3        0        0        0        0 46e407c0
0x20002348 |       400        0        0        0        0        0
0x20002360 |    d92fc3        0        0        0        0 40404040
0x20002378 |         0        0        0        0        0        3
0x20002390 |         3        3        3        0        0        0
0x200023a8 |         0        0        0        0      300        0
0x200023c0 |         0        0        0        0        0        0
0x200023d8 |         0        0        0        0        0        0
0x200023f0 |         0        0        0        0
   */
}
#endif

