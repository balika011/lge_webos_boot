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
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

//============================================================================
// NOTE:
//      In this file, all function cannot call sub-function!!!
//============================================================================
#include "rs232.h"

#include "x_ckgen.h"
#include "x_assert.h"
#include "x_timer.h"
#include "x_bim.h"
#include "x_pdwnc.h"
#include "drvcust_if.h"
#include "c_model.h"
#include "hw_pll_group.h"
#include "hw_hpcpll.h"
#include "hw_ifpll.h"
#include "hw_ifpga.h"
#include "hw_ospe.h"
//#include "hw_mempll.h"


extern void CHIP_DisplayChar(UINT32 u4Char);
extern void CHIP_DisplayString(const CHAR *szString);

void CHIP_ClockSetting(void);
void CHIP_BimInit(void);
void CHIP_1stBootRegInit(void);


void CHIP_Delay_us(UINT32 u4Micros)
{
	#ifdef CC_FPGA
    volatile UINT32 i;

    for (i = 0; i < (24*u4Micros)*100; i++)
    {
        ;
    }
    #else
    UINT32 u4Xtal;
    UINT64 u8Current;
    UINT64 u8TimeStop;

#if 1
	u4Xtal = 24*u4Micros;
#else
	switch(XTAL_STRAP_MODE) {
	
    case SET_XTAL_27MHZ:
        u4Xtal = 27*u4Micros;
        break;
    case SET_XTAL_24MHZ:
        u4Xtal = 24*u4Micros;
        break;
    default:
        while (1) { }
    }
#endif

    do {
        u8Current = BIM_READ32(REG_RW_TIMER2_HIGH);
        u8Current = u8Current << 32;
        u8Current |= BIM_READ32(REG_RW_TIMER2_LOW);
        u8TimeStop = u8Current - u4Xtal;
    } while (u8TimeStop > u8Current);
    do {
        u8Current = BIM_READ32(REG_RW_TIMER2_HIGH);
        u8Current = u8Current << 32;
        u8Current |= BIM_READ32(REG_RW_TIMER2_LOW);
    } while (u8TimeStop < u8Current);
    #endif
}

void CHIP_ClockSetting()
{
#ifndef CC_FPGA
    BIM_WRITE32(REG_RW_BTIME, 0x11); // delay register read path 1T

	BIM_WRITE32(REG_CA9_CTL, 0x8); //ca9 axi/2->axi/1
    #if 0
    #ifdef CC_MTK_LOADER
    if ((!(BIM_READ32(REG_RW_REMAP) & REMAP_ENABLE)) && (!(BIM_READ32(REG_RW_TIMER_CTRL) & TMR2_CNTDWN_EN)))
    {
        // Start timer 2 for bootup time counter enable.
        BIM_WRITE32(REG_RW_TIMER2_LLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_LOW , 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HIGH, 0xffffffff);
        BIM_SetTimeLog(0);
        BIM_SetTimeLog(1);
        BIM_SetTimeLog(2);
        BIM_SetTimeLog(3);
        BIM_SetTimeLog(4);
        BIM_SetTimeLog(5);
        BIM_SetTimeLog(6);
        BIM_SetTimeLog(7);
        BIM_WRITE32(REG_RW_TIMER_CTRL, (TMR2_CNTDWN_EN | TMR2_AUTOLD_EN));
    }
    #endif /* CC_MTK_LOADER */
    #endif /* CC_MTK_LOADER */

    CHIP_Delay_us(20);

//    vIO32WriteFldAlign(REG_MEMPLL0, 0x1, FLD_RG_MEMPLL_DIVEN);

    // Set for Mustang DVDD12=1.2V
//    vIO32WriteFldAlign(REG_MEMPLL1, 0x0, FLD_RG_MEMPLL_V11EN); // ?
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG1, 0x0, FLD_RG_VGAPLL_V11EN);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 0x0, FLD_RG_TAPLL_V11EN);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x0, FLD_RG_SYSPLL_V11EN);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0x0, FLD_RG_HADDS2_V11EN);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0x0, FLD_RG_G3DPLL_V11EN);
    vIO32WriteFldAlign(REG_HPCPLL_CFG1, 0x0, FLD_RG_HPCPLL_V11EN);

    // PLL MW->1.2
    vIO32WriteFldMulti(REG_IFPLL_CFG0, P_Fld(0xE,FLD_RG_IFPLL_BC)|P_Fld(3,FLD_RG_IFPLL_BIC)|
                                       P_Fld(2,FLD_RG_IFPLL_BR));
    vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x08, FLD_RG_IFPLL_BP);

    // Enable PLL watch dog
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x01, FLD_RG_SYSPLL_DET_EN);

    // PLL 4-stage Phase VCO
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x0, FLD_RG_SYSPLL_LF);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG3, 0x0, FLD_RG_HADDS2_LF);

    // Set PI cap
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x07, FLD_RG_XDDS_PI_C);
    vIO32WriteFldMulti(REG_PLL_GROUP_CFG8, P_Fld(7,FLD_RG_DDDS1_PI_C)|P_Fld(7,FLD_RG_DDDS2_PI_C)|P_Fld(7,FLD_RG_DMSS_PI_C));
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG9, 0x07, FLD_RG_HADDS1_PI_C);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG4, IO32ReadFldAlign(REG_PLL_GROUP_CFG4, FLD_RG_HADDS2_DDS_DMY) | 0x07, FLD_RG_HADDS2_DDS_DMY);

    // 1. Init SYSPLL, CPUPLL, G3DPLL, TAPLL
    // Xtal Gm control
    #if 1
    vIO32WriteFldMulti(PDWNC_XTALCFG0, P_Fld(0xF,FLD_RG_XTAL_GM)|P_Fld(1,FLD_RG_XTAL_TMEN));
	#endif

    // set CPU/BUS/SRAM/BIST to xtal before PLL setting
    vIO32WriteFldMulti(CKGEN_CPU_CKCFG, P_Fld(0,FLD_BUS_CK_SEL)|P_Fld(0,FLD_CPU_CK_SEL));
	BIM_WRITE32(0x400, (u4IO32Read4B(0xf0008400)&(~0xc00)));// bit[11:10]: 0 xtal, 1 armpll
	
    vIO32WriteFldAlign(CKGEN_ABIST_SRAM_CKCFG, 0x0, FLD_ABIST_SRAM_SEL);
    vIO32WriteFldAlign(CKGEN_REG_BIST_CKCFG, 0x0, FLD_BIST_SEL);
	

    // Power down PLL
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 0x1, FLD_RG_TAPLL_PWD);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x1, FLD_RG_SYSPLL_PWD);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0x1, FLD_RG_G3DPLL_PWD);
    vIO32WriteFldAlign(REG_HPCPLL_CFG0, 0x1, FLD_RG_HPCPLL_PWD);
	
	vIO32WriteFldAlign(REG_ARMPLL_CFG0, 0x0, FLD_RG_ARMPLL_BIAS_EN);
	vIO32WriteFldAlign(REG_ARMPLL_CFG0, 0x0, FLD_RG_ARMPLL_EN);

    // Set PLLGP_BIAS_RST = 1
    CHIP_Delay_us(40);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_PLLGP_BIAS_RST);

    // Set PLLGP_BIAS_PWD = 1
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x1, FLD_RG_PLLGP_BIAS_PWD);

    // Select the source of SYSPLL, G3DPLL and TAPLL from Xtal
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_XTAL_SEL);

    // Set SYSPLL for 648 MHz
    // SYSPLL = Xtal * SPLL_FBDIV2 * (SPLL_FBDIV + 1) / SPLL_PREDIV / SPLL_POSDIV
    // Default SYSPLL = Xtal * (1) * (26 + 1) / 1 / 1
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x1A, FLD_RG_SYSPLL_FBDIV);

    // Set CPUPLL
    // MJCPLL = Xtal * CPUPLL_FBDIV2 * (CPUPLL_FBDIV + 1) / CPUPLL_PREDIV / CPUPLL_POSDIV
    // Default CPUPLL = Xtal * (2) * (34 + 1) / 2 / 1
    // Set CPUPLL for 840 MHz
#ifdef CC_CPU_SPEED_TEST_ENABLE
    // Get CPUPLL setting
    UINT32 u4CPUPLL_FBDIV = 0x53;
    if ((u4IO32Read4B(0xf0028128) == 0) && u4IO32Read4B(0xf0028100) == 0)
    {
        vIO32Write4B(0xf0008150, 0xFFFFFFFF);
        vIO32Write4B(0xf0008154, u4CPUPLL_FBDIV);
    }
    else
    {
        if (u4IO32Read4B(0xf0008150) == 0xFFFFFFFF)
        {
            u4CPUPLL_FBDIV = u4IO32Read4B(0xf0008154);
        }
    }
    vIO32WriteFldMulti(REG_HPCPLL_CFG0,P_Fld(u4CPUPLL_FBDIV, FLD_RG_HPCPLL_FBDIV)|P_Fld(1,FLD_RG_HPCPLL_FBSEL)|P_Fld(2,FLD_RG_HPCPLL_PREDIV)|P_Fld(4,FLD_RG_HPCPLL_DIVEN)|P_Fld(1, FLD_RG_HPCPLL_BP));
#else
    vIO32WriteFldAlign(REG_HPCPLL_CFG0, 0x22, FLD_RG_HPCPLL_FBDIV);
//    vIO32WriteFldMulti(REG_HPCPLL_CFG0,P_Fld(0x4F, FLD_RG_HPCPLL_FBDIV)|P_Fld(1,FLD_RG_HPCPLL_FBSEL)|P_Fld(2,FLD_RG_HPCPLL_PREDIV)|P_Fld(4,FLD_RG_HPCPLL_DIVEN)|P_Fld(1, FLD_RG_HPCPLL_BP));
#endif // CC_CPU_SPEED_TEST_ENABLE

    // Set G3DPLL
    // G3DPLL = Xtal * G3DPLL_FBDIV2 * (G3DPLL_FBDIV + 1) / G3DPLL_PREDIV / G3DPLL_POSDIV
    // Default G3DPLL = Xtal * 2 * (32 + 1) / 2 / 2 (396Mhz)
    if (IS_IC_5399_ES1()) // CC_MT5882_TODO
    {
        // Set G3DPLL for 252MHz (ES1 issue)
        vIO32WriteFldMulti(REG_PLL_GROUP_CFG5, P_Fld(0x14,FLD_RG_G3DPLL_FBDIV)|P_Fld(0x01,FLD_RG_G3DPLL_DIVEN));
    }
    else
    {
    	#ifdef CC_MT5882
        // Set G3DPLL as 408MHz
        vIO32WriteFldMulti(REG_PLL_GROUP_CFG5, P_Fld(0x21,FLD_RG_G3DPLL_FBDIV)|P_Fld(0x01,FLD_RG_G3DPLL_DIVEN));
		#else
        // Set G3DPLL as 372MHz (ES2 and the following version)
        vIO32WriteFldMulti(REG_PLL_GROUP_CFG5, P_Fld(0x1E,FLD_RG_G3DPLL_FBDIV)|P_Fld(0x01,FLD_RG_G3DPLL_DIVEN));
		#endif
    }

    // Set TAPLL
    // TAPLL = Xtal * TAPLL_FBDIV2 * (TAPLL_FBDIV + 1) / TAPLL_PREDIV / TAPLL_POSDIV
    // Default TAPLL = Xtal * 2 * (35 + 1) / 2 / 2 (432Mhz)
    // Set TAPLL for 324MHz
#ifdef __MODEL_slt__
    vIO32WriteFldMulti(REG_PLL_GROUP_CFG0, P_Fld(0x1A,FLD_RG_TAPLL_FBDIV)|P_Fld(0x01,FLD_RG_TAPLL_DIVEN));
#elif (defined (CC_SUPPORT_4K2K) && defined (CC_MT5399))
	vIO32WriteFldMulti(REG_PLL_GROUP_CFG0, P_Fld(0x1F,FLD_RG_TAPLL_FBDIV)|P_Fld(0x01,FLD_RG_TAPLL_DIVEN));
#else
	#ifdef CC_MT5882
	// Set TAPLL for 336MHz
	vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 0x1B,FLD_RG_TAPLL_FBDIV);
	#endif
#endif

	// Set ARMPLL // for audio ARM11 use
	// ARMPLL = Xtal / ARMPLL_PREDIV * (ARMPLL_FBDIV + 1) * ARMPLL_FBSEL   / ARMPLL_POSDIV
	// Default ARMPLL = Xtal /(1) * (0x27+1) * 2  / 4 = 12*	(0x27+1)		//default  480M
	// Set ARMPLL for 480MHz
    vIO32WriteFldAlign(REG_ARMPLL_CFG0, 0x27, FLD_RG_ARMPLL_FBDIV);

    // Power on PLL
    // Set PLLGP_BIAS_PWD = 0
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_BIAS_PWD);
    CHIP_Delay_us(10);

    // Power on G3DPLL, SYSPLL, TAPLL, CPUPLL, ARMPLL
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG0, 0x0, FLD_RG_TAPLL_PWD);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG2, 0x0, FLD_RG_SYSPLL_PWD);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG5, 0x0, FLD_RG_G3DPLL_PWD);
    vIO32WriteFldAlign(REG_HPCPLL_CFG0, 0x0, FLD_RG_HPCPLL_PWD);
	
	vIO32WriteFldAlign(REG_ARMPLL_CFG0, 0x1, FLD_RG_ARMPLL_BIAS_EN);
	vIO32WriteFldAlign(REG_ARMPLL_CFG0, 0x1, FLD_RG_ARMPLL_EN);
	vIO32WriteFldAlign(REG_ARMPLL_CFG0, 0x1, FLD_RG_ARMPLL_OUT_EN);	

    // Enable DDS setting
    vIO32WriteFldMulti(REG_PLL_GROUP_CFG8,P_Fld(1,FLD_RG_DDDS1_PWDB)|P_Fld(1,FLD_RG_DDDS2_PWDB)|P_Fld(1,FLD_RG_DMSS_PWDB)|P_Fld(1,FLD_RG_HADDS1_PWDB));
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG6, 0x1, FLD_RG_XDDS_PWDB);

    // Set PLLGP_BIAS_RST = 0
    CHIP_Delay_us(20);
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG10, 0x0, FLD_RG_PLLGP_BIAS_RST);

    // 2. Set IFPLL
    // Load BAIS current
    vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x1, FLD_RG_IFPLL_BIAS_LPF_ENB); // IFPLL_BIAS_LPF_ENB = 0
    vIO32WriteFldAlign(REG_IFPLL_CFG1, 0x0, FLD_RG_IFPLL_BIAS_PWD); // IFPLL_BIAS_PWD = 0

    // Load PLL 1728Mhz default
    CHIP_Delay_us(10);
    vIO32WriteFldAlign(REG_IFPGA_CFG0, 0x1, FLD_RG_IFPLL_PWD); // IFPLL_PWD = 1
    vIO32WriteFldMulti(REG_IFPLL_CFG0, P_Fld(0xD,FLD_RG_IFPLL_BIR)|P_Fld(3,FLD_RG_IFPLL_BIC));
    vIO32WriteFldMulti(REG_IFPGA_CFG1, P_Fld(0x1,FLD_RG_IFPLL_AUTOK_EN)|P_Fld(0x0,FLD_RG_IFPLL_AUTOK_LOAD));

    // enable IFPLL clock to ckgen, IFADC, CVBS, PWMDAC
    vIO32WriteFldMulti(REG_IFPLL_CFG1, P_Fld(1,FLD_RG_IFPLL_DIGCK_EN)|P_Fld(1,FLD_RG_IFPLL_ANACK_EN)|
                                       P_Fld(1,FLD_RG_IFPLL_CVBSCK_EN)|P_Fld(1,FLD_RG_IFPLL_PWMCK_EN)|
                                       P_Fld(0,FLD_RG_IFPLL_XTAL1_EN));

    // start calibration
    CHIP_Delay_us(1);

    // power on IFPLL
    vIO32WriteFldAlign(REG_IFPLL_CFG0, 0, FLD_RG_IFPLL_PWD);

    // stop calibration
    CHIP_Delay_us(200);
    vIO32WriteFldMulti(REG_IFPLL_CFG1, P_Fld(0,FLD_RG_IFPLL_AUTOK_EN)|P_Fld(0,FLD_RG_IFPLL_BIAS_LPF_ENB)|
                                      P_Fld(1,FLD_RG_IFPLL_XTAL1_EN));

    // init Ethernet PLL
    // XPLL = Xtal(24Mhz) *  XPLL_FBDIV(25)
    // Default XPLL = 24Mhz * 25
    // pls_fbdiv_pre[15:9] Feedback divide ratio = 25
    // ethtop_dmy_pre[7:0]
    //    0: the input xtal clk shoule be 27MHZ
    //    1: the input xtal clk shoule be 24MHZ
    // Set XPLL for 600 Mhz
    vIO32Write4B(0xf003231C,((u4IO32Read4B(0xf003231C) & 0xffff0000) | 0x00003001));

    // 3. init USB PLL
    // already do this in step0
    CHIP_Delay_us(500);
    vIO32WriteFldAlign(CKGEN_REG_USB_CKCFG, 1, FLD_USB_SEL);
    vIO32WriteFldAlign(CKGEN_USB_CLK_CKCFG, 1, FLD_USBPHY_CK_DIV9);
    vIO32Write4B(0xf0059800,(u4IO32Read4B(0xf0059800)|0x00004000)); //RG_USB20_INTR_EN=1b
    vIO32Write4B(0xf0059a00,(u4IO32Read4B(0xf0059a00)|0x00004000)); //RG_USB20_INTR_EN=1b
    vIO32Write4B(0xf0059868,(u4IO32Read4B(0xf0059868)&(~0x00040000)));

    // change USB2.0 DRAM priority from group2 to group1
	vIO32Write4B(0xf000d5ac, (u4IO32Read4B(0xf000d5ac) | 0x0f000000));

    // 4. Select CPU clock to MJCPLL and Bus clock
    // cpu clock = cpupll (840Mhz), bus clock = 216Mhz
    /*
    Clock source selection[3:0]
        0: xtal_ck
        1: cpupll_ck
        2: enetpll_ck (600MHZ)
        3: syspll_ck (648MHz)
        4: usbpll_ck (480MHz)
        5: sawlesspll_ck (432MHz)
        6: enetpll_d2_ck (300MHZ)
        7: usbpll_d2_ck (240MHz )
        8: sawlesspll_d2_ck (216MHz )
        9: sawlesspll_d3_ck (144MHz )
        10: usbpll_d4_ck (120MHz )
        11: sawlesspll_d4_ck (108MHZ)
        12: enetpll_d4_ck (150MHZ)
        13: MEM_CK
        14: slow_ck
        15: bist_ck
    BUS_CK selection[10:8]
        0: bus_ck = xtal_ck
        1: bus_ck = sawlesspll_d2_ck
        2: bus_ck = usbpll_d2p5_ck
        3: bus_ck = syspll_d4_ck
        4: bus_ck = sawlesspll_d3_ck
        5: bus_ck = usbpll_d4_ck
        6: bus_ck = syspll_d3_ck
        7: bus_ck = slow_ck
    */
    vIO32WriteFldMulti(CKGEN_CPU_CKCFG, P_Fld(6,FLD_BUS_CK_SEL)|P_Fld(1,FLD_CPU_CK_SEL));
	// fix cpu clock is xtal issue
	vIO32Write4B(0xf0078060,0x12); //clock div
	vIO32Write4B(0xf0078064,0x12); //clock div
	BIM_WRITE32(0x400, (u4IO32Read4B(0xf0008400)&(~0xc00))|0x400); // bit[11:10]: 0 xtal, 1 armpll

#if 0
    // 5. set MCK to normal clcok (mempll)
    vIO32WriteFldAlign(CKGEN_MEM_CKCFG, 0x3, FLD_M_CK_TST);
#endif

    // 6. set common clk
    // C27M = sawless / 16
    vIO32WriteFldAlign(CKGEN_REG_C27M_CKCFG, 0x1, FLD_C27M_SEL);

    // 7. setup vopll
    // vopll source xtal
    vIO32WriteFldAlign(REG_PLL_GROUP_CFG8, 2, FLD_RG_DDDS1_CKSEL);

    // vopll setting
    vIO32Write4B(REG_VOPLL_CFG0, 0x22879404);
    vIO32Write4B(REG_VOPLL_CFG1, 0x84020020);
    vIO32Write4B(REG_VOPLL_CFG2, 0x00332E84);
    vIO32Write4B(REG_VOPLL_CFG3, 0x00008754);
    vIO32Write4B(REG_VOPLL_CFG4, 0x00800000);

    // Power on
    CHIP_Delay_us(10);
    vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_PWD);
    vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_LVDS_PWD);
    vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BG_VB1_TX_PWD);
    vIO32WriteFldAlign(REG_VOPLL_CFG0, 0, RG_VPLL_BIAS_BYP);

    // OCLK and TCLK Common
    vIO32WriteFldAlign(CKGEN_MBIST_CK_CTRL, 0, FLD_BIST_VPCLK_SEL);

    // OCLK Path
    vIO32WriteFldAlign(CKGEN_TCLK_TEST, FLD_OCLK_SEL_LVDS, FLD_OCLK_SEL);
    vIO32WriteFldAlign(CKGEN_TCLK_TEST, 0, FLD_OCLK_TSTSEL);

    // TCLK Path
    vIO32WriteFldAlign(CKGEN_TCLK_TEST, 1, FLD_TCLK_SEL);
    vIO32WriteFldAlign(CKGEN_TCLK_TEST, 0, FLD_TCLK_TSTSEL);

    // 8. setup serial flash pll
    //   0: 3.375 MHz   1: 5.4 MHz      2: 6.75 MHz     3: 9 MHz
    //   4: 13.5 MHz    5: 27 MHz       6: 30 MHz       7: 48 MHz
    //   8: 54 MHz      9: 72 MHz       10: 108 MHz     11: 0.84375 MHz
    IO_WRITE32(0, 0xF00087B0, 5);

    // 9. setup sram/BIST clock
    vIO32WriteFldAlign(CKGEN_ABIST_SRAM_CKCFG, 0x1, FLD_ABIST_SRAM_SEL);

#if 0
    // Speed up the ROM code clock
    // BIST clock selection
    // 0 : xtal (24Mhz)
    // 1 : sawlesspll (432Mhz)
    // 2 : SYSPLL / 2 (324Mhz)
    // 3 : sawlesspll / 3 (144Mhz)
    vIO32WriteFldAlign(CKGEN_REG_BIST_CKCFG, 0x2, FLD_BIST_SEL);
#endif

    // 10. setup nand pll (0: XTAL 24Mhz, 6: 192Mhz), and improve nand PIN driving
    vIO32WriteFldAlign(CKGEN_REG_NFI_CKCFG, 0x6, FLD_NFI_SEL);


    // 11. init USB3
#if 0 //for USB3. Capri not support USB3.0
    vIO32WriteFldAlign(CKGEN_BLOCK_CKEN_CFG2, 0x1, FLD_SSUSB_TOP_CKEN);
    vIO32WriteFldAlign(CKGEN_REG_TOP_BACKUP2, 0x1, FLD_SSUSB_XHCI_SLEEPB);
    vIO32WriteFldAlign(CKGEN_REG_TOP_BACKUP2, 0x0, FLD_SSUSB_XHCI_PD);
//    vIO32WriteFldAlign(CKGEN_REG_TOP_BACKUP3, 0x0, FLD_USB3CHA_R_DMAGSEL);
//    vIO32WriteFldAlign(CKGEN_REG_TOP_BACKUP3, 0x0, FLD_USB3CHB_R_DMAGSEL);
    vIO32WriteFldAlign(CKGEN_REG_TOP_BACKUP3, 0x1, FLD_RG_USB20_MUX); //mux to usb20
    vIO32WriteFldAlign(CKGEN_REG_TOP_BACKUP3, 0x1, FLD_USBREF_CK_SEL);
    vIO32WriteFldAlign(0xf0070700, 0x0, Fld(1,0,AC_MSKB0));
    vIO32WriteFldAlign(0xf0070704, 0x0, Fld(1,0,AC_MSKB0));

#endif

    // 12. Set Postscaler output panel timing to output stage
#ifdef CC_MT5882
    vIO32WriteFldAlign(OSTG_01, 0, REG_MJC_IN_EN);
#else
    vIO32WriteFldAlign(OSTG_01, 1, REG_MJC_IN_EN);
#endif     
       
#endif
}

void CHIP_BimInit()
{
    // set bus access timeout to avoid register access halting CPU.
    BIM_WRITE32(REG_RW_TIMEOUT_CTRL, TO_FLASH_CNT | TO_DRAM_CNT | TO_IOBUS_CNT | TO_FLASH_EN | TO_DRAM_EN | TO_IOBUS_EN);
    //BIM_WRITE32(REG_RW_BTIME, TO_IORW | TO_IOWW);

    // Disable irq & fiq
    BIM_WRITE32(REG_RW_IRQEN, 0);
    BIM_WRITE32(REG_RW_FIQEN, 0);
    BIM_WRITE32(REG_RW_IRQCL, 0xffffffff);
    BIM_WRITE32(REG_RW_FIQCL, 0xffffffff);
    #ifndef CC_LOAD_UBOOT
	BIM_CpuDie(1);
	BIM_CpuDie(2);
	BIM_CpuDie(3);
    #endif
    #ifndef CC_MTK_PRELOADER
    #ifdef CC_ARM_GIC
    GIC_Init();
    #endif
    #endif
}

void CHIP_1stBootRegInit()
{
#ifndef CC_MTK_PRELOADER
    // Initial timer, then can use HAL_Delay_us()
    VERIFY(HAL_InitTimer());
#endif /* CC_MTK_PRELOADER */
}

