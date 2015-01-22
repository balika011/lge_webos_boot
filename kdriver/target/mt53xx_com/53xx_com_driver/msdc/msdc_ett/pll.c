/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 * 
 * MediaTek Inc. (C) 2010. All rights reserved.
 * 
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include "typedefs.h"
//#include "common.h"
//#include "platform.h"


#include "pll.h"
#include "msdc.h"
//#include "timer.h"
//#include "spm.h"

//#include "dramc_pi_api.h"
//#include "dramc_common.h"
//#include "dramc_register.h"

#define DRV_WriteReg32  MSDC_WRITE32
#define DRV_Reg32  MSDC_READ32

int A_Reg3e0=0, A_Reg3e4=0;
int B_Reg3e0=0, B_Reg3e4=0;

//extern DRAMC_CTX_T DramCtx_LPDDR3;
//extern DRAMC_CTX_T DramCtx_PCDDR3;

#if 0
void mt_mempll_init(DRAMC_CTX_T *p)
{
    /*p->channel = CHANNEL_A;
    MemPllInit((DRAMC_CTX_T *) p);

    p->channel = CHANNEL_B;*/
    MemPllInit((DRAMC_CTX_T *) p);	
    return;
}

void mt_mempll_cali(DRAMC_CTX_T *p)
{
    // called after chA and chB init done
    // MEMPLL05 registers, some are located @ chA and others are @ chB
    p->channel = CHANNEL_A;
    DramcPllPhaseCal(p);
    p->channel = CHANNEL_B;
    DramcPllPhaseCal(p);
	
    //Should only be called after channel A/B MEMPLL phase calibration had been done.
    DramCPllGroupsCal(p);
    return;
}

void mt_mempll_pre(void)
{
    DRAMC_CTX_T *psDramCtx;

#ifdef DDRTYPE_LPDDR3
    psDramCtx = &DramCtx_LPDDR3;
#endif

#ifdef DDRTYPE_DDR3
    psDramCtx = &DramCtx_PCDDR3;
#endif

    mt_mempll_init(psDramCtx);
    return;
}

void mt_mempll_post(void)
{
    DRAMC_CTX_T *psDramCtx;

#ifdef DDRTYPE_LPDDR3
    psDramCtx = &DramCtx_LPDDR3;
#endif

#ifdef DDRTYPE_DDR3
    psDramCtx = &DramCtx_PCDDR3;
#endif

    mt_mempll_cali(psDramCtx);
    return;
}
#endif


unsigned int mt_get_smallcpu_freq(void)
{
    int output = 0;
    unsigned int temp, clk26cali_0, clk_cfg_8, clk_misc_cfg_1, clk26cali_1;
    unsigned int top_ckmuxsel, top_ckdiv1, ir_rosc_ctl;

    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0 | 0x80); // enable fmeter_en

    clk_misc_cfg_1 = DRV_Reg32(CLK_MISC_CFG_1);
    DRV_WriteReg32(CLK_MISC_CFG_1, 0xFFFFFF00); // select divider

    clk_cfg_8 = DRV_Reg32(CLK_CFG_8);
    DRV_WriteReg32(CLK_CFG_8, (46 << 8)); // select armpll_occ_mon

    top_ckmuxsel = DRV_Reg32(TOP_CKMUXSEL);
    DRV_WriteReg32(TOP_CKMUXSEL, (top_ckmuxsel & 0xFFFFFFFC) | 0x1);

    top_ckdiv1 = DRV_Reg32(TOP_CKDIV1);
    DRV_WriteReg32(TOP_CKDIV1, (top_ckdiv1 & 0xFFFFFFE0) | 0xb);
    
    ir_rosc_ctl = DRV_Reg32(IR_ROSC_CTL);
    DRV_WriteReg32(IR_ROSC_CTL, ir_rosc_ctl | 0x08100000);

    temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, temp | 0x1); // start fmeter

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x1)
    {
        sys_print("wait for frequency meter finish, CLK26CALI = 0x%x\n", DRV_Reg32(CLK26CALI_0));
        //mdelay(10);
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) / 1024) * 4; // Khz

    DRV_WriteReg32(CLK_CFG_8, clk_cfg_8);
    DRV_WriteReg32(CLK_MISC_CFG_1, clk_misc_cfg_1);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(TOP_CKMUXSEL, top_ckmuxsel);
    DRV_WriteReg32(TOP_CKDIV1, top_ckdiv1);
    DRV_WriteReg32(IR_ROSC_CTL, ir_rosc_ctl);

    //print("CLK26CALI = 0x%x, cpu frequency = %d Khz\n", temp, output);

    return output;
}
unsigned int mt_get_bigcpu_freq(void)
{
    int output = 0;
    unsigned int temp, clk26cali_0, clk_cfg_8, clk_misc_cfg_1, clk26cali_1;
    unsigned int top_ckmuxsel, top_ckdiv1, ir_rosc_ctl, ca15l_mon_sel;

    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0 | 0x80); // enable fmeter_en

    clk_misc_cfg_1 = DRV_Reg32(CLK_MISC_CFG_1);
    DRV_WriteReg32(CLK_MISC_CFG_1, 0xFFFFFF00); // select divider

    clk_cfg_8 = DRV_Reg32(CLK_CFG_8);
    DRV_WriteReg32(CLK_CFG_8, (46 << 8)); // select abist_cksw

    top_ckmuxsel = DRV_Reg32(TOP_CKMUXSEL);
    DRV_WriteReg32(TOP_CKMUXSEL, (top_ckmuxsel & 0xFFFFFFF3) | (0x1<<2));

    top_ckdiv1 = DRV_Reg32(TOP_CKDIV1);
    DRV_WriteReg32(TOP_CKDIV1, (top_ckdiv1 & 0xFFFFFC1F) | (0xb<<5));
    
    ca15l_mon_sel = DRV_Reg32(CA15L_MON_SEL);
    DRV_WriteReg32(CA15L_MON_SEL, ca15l_mon_sel | 0x00000500);
    
    ir_rosc_ctl = DRV_Reg32(IR_ROSC_CTL);
    DRV_WriteReg32(IR_ROSC_CTL, ir_rosc_ctl | 0x10000000);

    temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, temp | 0x1); // start fmeter

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x1)
    {
        sys_print("wait for frequency meter finish, CLK26CALI = 0x%x\n", DRV_Reg32(CLK26CALI_0));
        //mdelay(10);
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = ((temp * 26000) / 1024) * 4; // Khz

    DRV_WriteReg32(CLK_CFG_8, clk_cfg_8);
    DRV_WriteReg32(CLK_MISC_CFG_1, clk_misc_cfg_1);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);
    DRV_WriteReg32(TOP_CKMUXSEL, top_ckmuxsel);
    DRV_WriteReg32(TOP_CKDIV1, top_ckdiv1);
    DRV_WriteReg32(CA15L_MON_SEL, ca15l_mon_sel);
    DRV_WriteReg32(IR_ROSC_CTL, ir_rosc_ctl);

    //sys_print("CLK26CALI = 0x%x, cpu frequency = %d Khz\n", temp, output);

    return output;
}


unsigned int mt_get_mem_freq(void)
{
    int output = 0;
    unsigned int temp, clk26cali_0, clk_cfg_8, clk_misc_cfg_1, clk26cali_1;

    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0 | 0x80); // enable fmeter_en

    clk_misc_cfg_1 = DRV_Reg32(CLK_MISC_CFG_1);
    DRV_WriteReg32(CLK_MISC_CFG_1, 0xFFFFFF00); // select divider

    clk_cfg_8 = DRV_Reg32(CLK_CFG_8);
    DRV_WriteReg32(CLK_CFG_8, (24 << 8)); // select abist_cksw

    temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, temp | 0x1); // start fmeter

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x1)
    {
        sys_print("wait for frequency meter finish, CLK26CALI = 0x%x\n", DRV_Reg32(CLK26CALI_0));
        //mdelay(10);
    }

    temp = DRV_Reg32(CLK26CALI_1) & 0xFFFF;

    output = (temp * 26000) / 1024; // Khz

    DRV_WriteReg32(CLK_CFG_8, clk_cfg_8);
    DRV_WriteReg32(CLK_MISC_CFG_1, clk_misc_cfg_1);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);

    //print("CLK26CALI = 0x%x, mem frequency = %d Khz\n", temp, output);

    return output;
}

unsigned int mt_get_bus_freq(void)
{
    int output = 0;
    unsigned int temp, clk26cali_0, clk_cfg_9, clk_misc_cfg_1, clk26cali_2;

    clk26cali_0 = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0 | 0x80); // enable fmeter_en

    clk_misc_cfg_1 = DRV_Reg32(CLK_MISC_CFG_1);
    DRV_WriteReg32(CLK_MISC_CFG_1, 0x00FFFFFF); // select divider

    clk_cfg_9 = DRV_Reg32(CLK_CFG_9);
    DRV_WriteReg32(CLK_CFG_9, (1 << 16)); // select ckgen_cksw

    temp = DRV_Reg32(CLK26CALI_0);
    DRV_WriteReg32(CLK26CALI_0, temp | 0x10); // start fmeter

    /* wait frequency meter finish */
    while (DRV_Reg32(CLK26CALI_0) & 0x10)
    {
        sys_print("wait for frequency meter finish, CLK26CALI = 0x%x\n", DRV_Reg32(CLK26CALI_0));
        //mdelay(10);
    }

    temp = DRV_Reg32(CLK26CALI_2) & 0xFFFF;

    output = (temp * 26000) / 1024; // Khz

    DRV_WriteReg32(CLK_CFG_9, clk_cfg_9);
    DRV_WriteReg32(CLK_MISC_CFG_1, clk_misc_cfg_1);
    DRV_WriteReg32(CLK26CALI_0, clk26cali_0);

    //sys_print("CLK26CALI = 0x%x, bus frequency = %d Khz\n", temp, output);

    return output;
}

void mt_pll_post_init(void)
{
    unsigned int temp;
	//mt_mempll_post();

    //step 48
    temp = DRV_Reg32(AP_PLL_CON3);
    DRV_WriteReg32(AP_PLL_CON3, temp & 0xFFF44440); // Only UNIVPLL SW Control

    //step 49
    temp = DRV_Reg32(AP_PLL_CON4);
    DRV_WriteReg32(AP_PLL_CON4, temp & 0xFFFFFFF4); // Only UNIVPLL SW Control

//    sys_print("mt_pll_post_init: mt_get_smallcpu_freq = %dKhz\n", mt_get_smallcpu_freq());
//    sys_print("mt_pll_post_init: mt_get_bigcpu_freq = %dKhz\n", mt_get_bigcpu_freq());
//    sys_print("mt_pll_post_init: mt_get_bus_freq = %dKhz\n", mt_get_bus_freq());
    sys_print("mt_pll_post_init: mt_get_mem_freq = %dKhz\n", mt_get_mem_freq());

#ifndef MTK_FORCE_CLUSTER1
    disable_pll(ARMCA15PLL, "pll");
#endif
    
    #if 1
    sys_print("mt_pll_post_init: AP_PLL_CON3        = 0x%x, GS = 0x00000000\n", DRV_Reg32(AP_PLL_CON3));
    sys_print("mt_pll_post_init: AP_PLL_CON4        = 0x%x, GS = 0x00000000\n", DRV_Reg32(AP_PLL_CON4));
    sys_print("mt_pll_post_init: AP_PLL_CON6        = 0x%x, GS = 0x00000000\n", DRV_Reg32(AP_PLL_CON6));
    sys_print("mt_pll_post_init: CLKSQ_STB_CON0     = 0x%x, GS = 0x05010501\n", DRV_Reg32(CLKSQ_STB_CON0));
    sys_print("mt_pll_post_init: PLL_ISO_CON0       = 0x%x, GS = 0x00080008\n", DRV_Reg32(PLL_ISO_CON0));
    sys_print("mt_pll_post_init: ARMCA15PLL_CON0    = 0x%x, GS = 0x00000101\n", DRV_Reg32(ARMCA15PLL_CON0));
    sys_print("mt_pll_post_init: ARMCA15PLL_CON1    = 0x%x, GS = 0x80108000\n", DRV_Reg32(ARMCA15PLL_CON1));
    sys_print("mt_pll_post_init: ARMCA15PLL_PWR_CON0= 0x%x, GS = 0x00000001\n", DRV_Reg32(ARMCA15PLL_PWR_CON0));
    sys_print("mt_pll_post_init: ARMCA7PLL_CON0     = 0x%x, GS = 0xF1000101\n", DRV_Reg32(ARMCA7PLL_CON0));
    sys_print("mt_pll_post_init: ARMCA7PLL_CON1     = 0x%x, GS = 0x800E8000\n", DRV_Reg32(ARMCA7PLL_CON1));
    sys_print("mt_pll_post_init: ARMCA7PLL_PWR_CON0 = 0x%x, GS = 0x00000001\n", DRV_Reg32(ARMCA7PLL_PWR_CON0));
    sys_print("mt_pll_post_init: MAINPLL_CON0       = 0x%x, GS = 0xF1000101\n", DRV_Reg32(MAINPLL_CON0));
    sys_print("mt_pll_post_init: MAINPLL_CON1       = 0x%x, GS = 0x800A8000\n", DRV_Reg32(MAINPLL_CON1));
    sys_print("mt_pll_post_init: MAINPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(MAINPLL_PWR_CON0));
    sys_print("mt_pll_post_init: UNIVPLL_CON0       = 0x%x, GS = 0xFF000011\n", DRV_Reg32(UNIVPLL_CON0));
    sys_print("mt_pll_post_init: UNIVPLL_CON1       = 0x%x, GS = 0x80180000\n", DRV_Reg32(UNIVPLL_CON1));
    sys_print("mt_pll_post_init: UNIVPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(UNIVPLL_PWR_CON0));
    sys_print("mt_pll_post_init: MMPLL_CON0         = 0x%x, GS = 0x00000101\n", DRV_Reg32(MMPLL_CON0));
    sys_print("mt_pll_post_init: MMPLL_CON1         = 0x%x, GS = 0x820D8000\n", DRV_Reg32(MMPLL_CON1));
    sys_print("mt_pll_post_init: MMPLL_PWR_CON0     = 0x%x, GS = 0x00000001\n", DRV_Reg32(MMPLL_PWR_CON0));
    sys_print("mt_pll_post_init: MSDCPLL_CON0       = 0x%x, GS = 0x00000111\n", DRV_Reg32(MSDCPLL_CON0));
    sys_print("mt_pll_post_init: MSDCPLL_CON1       = 0x%x, GS = 0x800F6276\n", DRV_Reg32(MSDCPLL_CON1));
    sys_print("mt_pll_post_init: MSDCPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(MSDCPLL_PWR_CON0));
    sys_print("mt_pll_post_init: TVDPLL_CON0        = 0x%x, GS = 0x00000101\n", DRV_Reg32(TVDPLL_CON0));
    sys_print("mt_pll_post_init: TVDPLL_CON1        = 0x%x, GS = 0x80112276\n", DRV_Reg32(TVDPLL_CON1));
    sys_print("mt_pll_post_init: TVDPLL_PWR_CON0    = 0x%x, GS = 0x00000001\n", DRV_Reg32(TVDPLL_PWR_CON0));
    sys_print("mt_pll_post_init: VENCPLL_CON0       = 0x%x, GS = 0x00000111\n", DRV_Reg32(VENCPLL_CON0));
    sys_print("mt_pll_post_init: VENCPLL_CON1       = 0x%x, GS = 0x800E989E\n", DRV_Reg32(VENCPLL_CON1));
    sys_print("mt_pll_post_init: VENCPLL_PWR_CON0   = 0x%x, GS = 0x00000001\n", DRV_Reg32(VENCPLL_PWR_CON0));
    sys_print("mt_pll_post_init: MPLL_CON0          = 0x%x, GS = 0x00010111\n", DRV_Reg32(MPLL_CON0));
    sys_print("mt_pll_post_init: MPLL_CON1          = 0x%x, GS = 0x801C0000\n", DRV_Reg32(MPLL_CON1));
    sys_print("mt_pll_post_init: MPLL_PWR_CON0      = 0x%x, GS = 0x00000001\n", DRV_Reg32(MPLL_PWR_CON0));
    sys_print("mt_pll_post_init: VCODECPLL_CON0     = 0x%x, GS = 0x00000121\n", DRV_Reg32(VCODECPLL_CON0));
    sys_print("mt_pll_post_init: VCODECPLL_CON1     = 0x%x, GS = 0x80130000\n", DRV_Reg32(VCODECPLL_CON1));
    sys_print("mt_pll_post_init: VCODECPLL_PWR_CON0 = 0x%x, GS = 0x00000001\n", DRV_Reg32(VCODECPLL_PWR_CON0));
    sys_print("mt_pll_post_init: APLL1_CON0         = 0x%x, GS = 0xF0000131\n", DRV_Reg32(APLL1_CON0));
    sys_print("mt_pll_post_init: APLL1_CON1         = 0x%x, GS = 0xB7945EA6\n", DRV_Reg32(APLL1_CON1));
    sys_print("mt_pll_post_init: APLL1_PWR_CON0     = 0x%x, GS = 0x00000001\n", DRV_Reg32(APLL1_PWR_CON0));
    sys_print("mt_pll_post_init: APLL2_CON0         = 0x%x, GS = 0x00000131\n", DRV_Reg32(APLL2_CON0));
    sys_print("mt_pll_post_init: APLL2_CON1         = 0x%x, GS = 0xBC7EA932\n", DRV_Reg32(APLL2_CON1));
    sys_print("mt_pll_post_init: APLL2_PWR_CON0     = 0x%x, GS = 0x00000001\n", DRV_Reg32(APLL2_PWR_CON0));
    #endif

    
}



void mt_pll_init(void)
{
    int ret = 0;
    unsigned int temp;

    //step 1
    DRV_WriteReg32(CLKSQ_STB_CON0, 0x05010501); // reduce CLKSQ disable time
    
    //step 2
    DRV_WriteReg32(PLL_ISO_CON0, 0x00080008); // extend PWR/ISO control timing to 1us
    
    //step 3
    DRV_WriteReg32(AP_PLL_CON6, 0x00000000); //

    /*************
    * xPLL PWR ON 
    **************/
    //step 4
    temp = DRV_Reg32(ARMCA15PLL_PWR_CON0);
    DRV_WriteReg32(ARMCA15PLL_PWR_CON0, temp | 0x1);

    temp = DRV_Reg32(ARMCA7PLL_PWR_CON0);
    DRV_WriteReg32(ARMCA7PLL_PWR_CON0, temp | 0x1);

    //step 5
    temp = DRV_Reg32(MAINPLL_PWR_CON0);
    DRV_WriteReg32(MAINPLL_PWR_CON0, temp | 0x1);
    
    //step 6
    temp = DRV_Reg32(UNIVPLL_PWR_CON0);
    DRV_WriteReg32(UNIVPLL_PWR_CON0, temp | 0x1);
    
    //step 7
    temp = DRV_Reg32(MMPLL_PWR_CON0);
    DRV_WriteReg32(MMPLL_PWR_CON0, temp | 0x1);
    
    //step 8
    temp = DRV_Reg32(MSDCPLL_PWR_CON0);
    DRV_WriteReg32(MSDCPLL_PWR_CON0, temp | 0x1);
    
    //step 9
    temp = DRV_Reg32(VENCPLL_PWR_CON0);
    DRV_WriteReg32(VENCPLL_PWR_CON0, temp | 0x1);
    
    //step 10
    temp = DRV_Reg32(TVDPLL_PWR_CON0);
    DRV_WriteReg32(TVDPLL_PWR_CON0, temp | 0x1);

    //step 11
    temp = DRV_Reg32(MPLL_PWR_CON0);
    DRV_WriteReg32(MPLL_PWR_CON0, temp | 0x1);
    
    //step 12
    temp = DRV_Reg32(VCODECPLL_PWR_CON0);
    DRV_WriteReg32(VCODECPLL_PWR_CON0, temp | 0x1);

    //step 13
    temp = DRV_Reg32(APLL1_PWR_CON0);
    DRV_WriteReg32(APLL1_PWR_CON0, temp | 0x1);
    
    //step 14
    temp = DRV_Reg32(APLL2_PWR_CON0);
    DRV_WriteReg32(APLL2_PWR_CON0, temp | 0x1);

    //gpt_busy_wait_us(5); // wait for xPLL_PWR_ON ready (min delay is 1us)
    CTP_Wait_msec(100); // wait for xPLL_PWR_ON ready (min delay is 1us)

    /******************
    * xPLL ISO Disable
    *******************/
    //step 15
    temp = DRV_Reg32(ARMCA15PLL_PWR_CON0);
    DRV_WriteReg32(ARMCA15PLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    temp = DRV_Reg32(ARMCA7PLL_PWR_CON0);
    DRV_WriteReg32(ARMCA7PLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 16
    temp = DRV_Reg32(MAINPLL_PWR_CON0);
    DRV_WriteReg32(MAINPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 17
    temp = DRV_Reg32(UNIVPLL_PWR_CON0);
    DRV_WriteReg32(UNIVPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 18
    temp = DRV_Reg32(MMPLL_PWR_CON0);
    DRV_WriteReg32(MMPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 19
    temp = DRV_Reg32(MSDCPLL_PWR_CON0);
    DRV_WriteReg32(MSDCPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 20
    temp = DRV_Reg32(VENCPLL_PWR_CON0);
    DRV_WriteReg32(VENCPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 21
    temp = DRV_Reg32(TVDPLL_PWR_CON0);
    DRV_WriteReg32(TVDPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 22
    temp = DRV_Reg32(MPLL_PWR_CON0);
    DRV_WriteReg32(MPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 23
    temp = DRV_Reg32(VCODECPLL_PWR_CON0);
    DRV_WriteReg32(VCODECPLL_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 24
    temp = DRV_Reg32(APLL1_PWR_CON0);
    DRV_WriteReg32(APLL1_PWR_CON0, temp & 0xFFFFFFFD);
    
    //step 25
    temp = DRV_Reg32(APLL2_PWR_CON0);
    DRV_WriteReg32(APLL2_PWR_CON0, temp & 0xFFFFFFFD);
    
    /********************
    * xPLL Frequency Set
    *********************/
    //step 26
    //DRV_WriteReg32(ARMCA15PLL_CON1, 0x80108000); // 1716MHz
    DRV_WriteReg32(ARMCA15PLL_CON1, 0x810F8000); // 806MHz

    //DRV_WriteReg32(ARMCA7PLL_CON1, 0x800E8000); // 1508MHz
    //DRV_WriteReg32(ARMCA7PLL_CON1, 0x800B0000); // 1144MHz
    //DRV_WriteReg32(ARMCA7PLL_CON1, 0x810F8000); // 806MHz
    DRV_WriteReg32(ARMCA7PLL_CON1, 0x800B0000); // 1144MHz
    //DRV_WriteReg32(ARMCA7PLL_CON1, 0x800E8000); // 1508MHz
    
    //step 27
    DRV_WriteReg32(MAINPLL_CON1, 0x800A8000); //1092MHz
    
    //step 28
    DRV_WriteReg32(MMPLL_CON1, 0x82118000); //455MHz
    
    //step 29
    DRV_WriteReg32(MSDCPLL_CON1, 0x800F6276); //800MHz
    
    //step 30
    //temp = ((~(DRV_Reg32(VENCPLL_CON1) & 0x80000000)) & 0x80000000);
    //DRV_WriteReg32(VENCPLL_CON1, temp | 0x000B6000);
    DRV_WriteReg32(VENCPLL_CON1, 0x800F6276); //800MHz
    //DRV_WriteReg32(VENCPLL_CON1, 0x8009A000);
    
    //step 31
    DRV_WriteReg32(TVDPLL_CON1, 0x80112276); // 445.5MHz

    //step 32
    DRV_WriteReg32(MPLL_CON1, 0x801C0000);
    DRV_WriteReg32(MPLL_CON0, 0x00010110); //52MHz

    //step 33
    DRV_WriteReg32(VCODECPLL_CON1, 0x80130000); // 494MHz

    //APLL1 and APLL2 use the default setting 
    /***********************
    * xPLL Frequency Enable
    ************************/
    //step 34
    temp = DRV_Reg32(ARMCA15PLL_CON0);
    DRV_WriteReg32(ARMCA15PLL_CON0, temp | 0x1);
    
    temp = DRV_Reg32(ARMCA7PLL_CON0);
    DRV_WriteReg32(ARMCA7PLL_CON0, temp | 0x1);
    
    //step 35
    temp = DRV_Reg32(MAINPLL_CON0) & (~ 0x70);
/* CC: modify for testing */
#if 1
    /* CCI400 @ 500MHz */
    /* not divide by 2 */
#else
    /* CCI400 @ 250MHz */
    /* divide by 2 */
    temp |= 0x10;
#endif
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x1);
    
    //step 36
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x1);
    
    //step 37
    temp = DRV_Reg32(MMPLL_CON0);
    DRV_WriteReg32(MMPLL_CON0, temp | 0x1);
    
    //step 38
    temp = DRV_Reg32(MSDCPLL_CON0);
    DRV_WriteReg32(MSDCPLL_CON0, temp | 0x1);
    
    //step 39
    temp = DRV_Reg32(VENCPLL_CON0);
    DRV_WriteReg32(VENCPLL_CON0, temp | 0x1);
    
    //step 40
    temp = DRV_Reg32(TVDPLL_CON0);
    DRV_WriteReg32(TVDPLL_CON0, temp | 0x1); 

    //step 41
    temp = DRV_Reg32(MPLL_CON0);
    DRV_WriteReg32(MPLL_CON0, temp | 0x1); 
    
    //step 42
    temp = DRV_Reg32(VCODECPLL_CON0);
    DRV_WriteReg32(VCODECPLL_CON0, temp | 0x1); 
    
    //step 43
    temp = DRV_Reg32(APLL1_CON0);
    DRV_WriteReg32(APLL1_CON0, temp | 0x1); 
    
    //step 44
    temp = DRV_Reg32(APLL2_CON0);
    DRV_WriteReg32(APLL2_CON0, temp | 0x1); 
    
    //gpt_busy_wait_us(20); // wait for PLL stable (min delay is 20us)
    CTP_Wait_msec(1000); // wait for PLL stable (min delay is 20us)

    /***************
    * xPLL DIV RSTB
    ****************/
    //step 45
    temp = DRV_Reg32(ARMCA7PLL_CON0);
    DRV_WriteReg32(ARMCA7PLL_CON0, temp | 0x01000000);
    
    //step 46
    temp = DRV_Reg32(MAINPLL_CON0);
    DRV_WriteReg32(MAINPLL_CON0, temp | 0x01000000);
    
    //step 47
    temp = DRV_Reg32(UNIVPLL_CON0);
    DRV_WriteReg32(UNIVPLL_CON0, temp | 0x01000000);

    /*****************
    * xPLL HW Control
    ******************/
#if 0
    //default is SW mode, set HW mode after MEMPLL caribration
    //step 48
    temp = DRV_Reg32(AP_PLL_CON3);
    DRV_WriteReg32(AP_PLL_CON3, temp & 0xFFF4CCC0); // UNIVPLL SW Control

    //step 49
    temp = DRV_Reg32(AP_PLL_CON4);
    DRV_WriteReg32(AP_PLL_CON4, temp & 0xFFFFFFFC); // UNIVPLL,  SW Control
#endif
    /*************
    * MEMPLL Init
    **************/

    //mt_mempll_pre();

    /**************
    * INFRA CLKMUX
    ***************/

    temp = DRV_Reg32(TOP_DCMCTL);
    DRV_WriteReg32(TOP_DCMCTL, temp | 0x1); // enable infrasys DCM

    //CA7: INFRA_TOPCKGEN_CKDIV1[4:0](0x10001008)
    //CA9: INFRA_TOPCKGEN_CKDIV1[9:5](0x10001008)
    temp = DRV_Reg32(TOP_CKDIV1);
    DRV_WriteReg32(TOP_CKDIV1, temp & 0xFFFFFC00); // CPU clock divide by 1

    //CA7: INFRA_TOPCKGEN_CKMUXSEL[1:0] (0x10001000) =1
    //CA9: INFRA_TOPCKGEN_CKMUXSEL[3:2] (0x10001000)= 1
    temp = DRV_Reg32(TOP_CKMUXSEL);
    DRV_WriteReg32(TOP_CKMUXSEL, temp | 0x5); // switch CA7_ck to ARMCA7PLL, and CA15_ck to ARMCA15PLL

    /************
    * TOP CLKMUX
    *************/

    DRV_WriteReg32(CLK_CFG_0, 0x01000101); //ddrphycfg_ck = 26MHz

    DRV_WriteReg32(CLK_CFG_1, 0x01010100); // pwm = 26Mhz

    DRV_WriteReg32(CLK_CFG_2, 0x01010000); // uart = camtg = 26Mhz

    //MSDC_1 use MSDCPLL_D4, MSDC_50_0 use MSDCPLL_D4,MSDC50_hclk use SYSPLL4_D2
    DRV_WriteReg32(CLK_CFG_3, 0x02060301); 

    DRV_WriteReg32(CLK_CFG_4, 0x01000101); // audio = 26M

    DRV_WriteReg32(CLK_CFG_5, 0x01000100); // bit16~23 is reserved, pmicspi use 26MHz

/* CC: modify for testing */
#if 0
    DRV_WriteReg32(CLK_CFG_6, 0x01050101);//cci400 use mainpll
#else
    /* CCI400 use universal PLL */
    //DRV_WriteReg32(CLK_CFG_6, 0x01040101);

    DRV_WriteReg32(CLK_CFG_6, 0x01010101);//cci400 use vencpll, 800MHz
#endif

    DRV_WriteReg32(CLK_CFG_7, 0x01010101);

    DRV_WriteReg32(CLK_SCP_CFG_0, 0x7FF); // enable scpsys clock off control
    DRV_WriteReg32(CLK_SCP_CFG_1, 0x15); // enable scpsys clock off control

    /*for MTCMOS*/
    //spm_mtcmos_ctrl_disp(STA_POWER_ON);

    /*turn on DISP*/
    DRV_WriteReg32(DISP_CG_CLR0, 0xFFFFFFFF);
    DRV_WriteReg32(DISP_CG_CLR1, 0x3FF);

    /*Turn on LARB0 OSTD*/
    temp = DRV_Reg32(SMI_LARB0_STAT);
    if(0 == temp)
    {
        DRV_WriteReg32(SMI_LARB0_OSTD_CTRL_EN , 0xffffffff);//Turn on the OSTD on LARB0
    }
    else
    {
//        sys_print("LARB0 is busy , cannot set OSTD 0x%x\n" , temp);
    }

    /*Cautions !!! 
      If more MM engines will be enabled in preloader other than LARB0, 
      please clear LARB clock gate
      and set corresponded LARB# OSTD as following
    DRV_WriteReg32(SMI_LARB1_OSTD_CTRL_EN , 0xffffffff);
    DRV_WriteReg32(SMI_LARB2_OSTD_CTRL_EN , 0xffffffff);
    */
    
}
#if 0
int spm_mtcmos_ctrl_disp(int state)
{
    int err = 0;
    volatile unsigned int val;
    unsigned long flags;

    spm_write(SPM_POWERON_CONFIG_SET, (SPM_PROJECT_CODE << 16) | (1U << 0));

    if (state == STA_POWER_DOWN) {
        
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) | SRAM_PDN);
#if 1
        while ((spm_read(SPM_DIS_PWR_CON) & DIS_SRAM_ACK) != DIS_SRAM_ACK) {
        }
#endif
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) | PWR_ISO);

        val = spm_read(SPM_DIS_PWR_CON);
        val = (val & ~PWR_RST_B) | PWR_CLK_DIS;
        spm_write(SPM_DIS_PWR_CON, val);

        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) & ~(PWR_ON | PWR_ON_S));

        while ((spm_read(SPM_PWR_STATUS) & DIS_PWR_STA_MASK)
                || (spm_read(SPM_PWR_STATUS_2ND) & DIS_PWR_STA_MASK)) {
        }
    } else {    /* STA_POWER_ON */
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) | PWR_ON);
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) | PWR_ON_S);

        while (!(spm_read(SPM_PWR_STATUS) & DIS_PWR_STA_MASK) 
                || !(spm_read(SPM_PWR_STATUS_2ND) & DIS_PWR_STA_MASK)) {
        }

        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) & ~PWR_CLK_DIS);
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) & ~PWR_ISO);
        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) | PWR_RST_B);

        spm_write(SPM_DIS_PWR_CON, spm_read(SPM_DIS_PWR_CON) & ~SRAM_PDN);

#if 1
        while ((spm_read(SPM_DIS_PWR_CON) & DIS_SRAM_ACK)) {
        }
#endif
    }

    return err;
}
#endif


#define PLL_PWR_ON  (0x1 << 0)
#define PLL_ISO_EN  (0x1 << 1)

#define clk_setl(addr, val) \
    DRV_WriteReg32(addr, DRV_Reg32(addr) | (val))

#define clk_clrl(addr, val) \
    DRV_WriteReg32(addr, DRV_Reg32(addr) & ~(val))

int disable_pll(int id, char *name)
{
	if(id != 0)
		return 1;
	
    clk_clrl(ARMCA15PLL_CON0, 0x1);

    clk_setl(ARMCA15PLL_PWR_CON0, PLL_ISO_EN);
    clk_clrl(ARMCA15PLL_PWR_CON0, PLL_PWR_ON);	
    
    sys_print("disable_ca15armpll done\n");
    
    return 0;
}


#if 0
void PLL_DVT()
{
    int ARMCA15_EFUSE, ARMCA7_EFUSE, MM_EFUSE;
    int PCW, POSDIV;

//    ARMCA15_EFUSE = (DRV_Reg32(0x10206040) & 0x7);
//    ARMCA7_EFUSE = (DRV_Reg32(0x10206040) & 0x7);
//    MM_EFUSE  = (DRV_Reg32(0x10206040) & 0xC0000000)>>30;
    print("ARMCA15_EFUSE=%d, ARMCA7_EFUSE=%d, MM_EFUSE=%d \n",ARMCA15_EFUSE, ARMCA7_EFUSE, MM_EFUSE);

/*ARMCA15_PLL*/	
    if(ARMCA15_EFUSE==0)
    {
        //no limit
        //set output=2470MHz
        DRV_WriteReg32(ARMCA15PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA15PLL_CON1, 0x8017C000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA15PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA15PLL_CON1)&0x07000000)>>24;
        
        sys_print("ARMCA15_EFUSE=0, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x0017C000) && (POSDIV==0) )
            print("ARMCA15_EFUSE=0, PASS\n");
        else
        	sys_print("ARMCA15_EFUSE=0, FAIL\n");
    }
    else if (ARMCA15_EFUSE==1)
    {
        //POSTDIV=1, FOUT:1001~2405
        //set output=2418MHz
        DRV_WriteReg32(ARMCA15PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA15PLL_CON1, 0x80174000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA15PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA15PLL_CON1)&0x07000000)>>24;
        
        print("ARMCA15_EFUSE=1, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x00174000) && (POSDIV==0) )
            print("ARMCA15_EFUSE=1, FAIL\n");
        else
        	print("ARMCA15_EFUSE=1, PASS\n");
    }	
    else if (ARMCA15_EFUSE==2)
    {
        //POSTDIV=1, FOUT:1001~2314
        //set output=2327MHz
        DRV_WriteReg32(ARMCA15PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA15PLL_CON1, 0x80166000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA15PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA15PLL_CON1)&0x07000000)>>24;
        
        print("ARMCA15_EFUSE=2, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x00166000) && (POSDIV==0) )
            print("ARMCA15_EFUSE=2, FAIL\n");
        else
        	print("ARMCA15_EFUSE=2, PASS\n");        
    }
    else if (ARMCA15_EFUSE==3)
    {
        //POSTDIV=1, FOUT:1001~2210
        //set output=2223MHz
        DRV_WriteReg32(ARMCA15PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA15PLL_CON1, 0x80156000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA15PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA15PLL_CON1)&0x07000000)>>24;
        
        print("ARMCA15_EFUSE=3, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x00156000) && (POSDIV==0) )
            print("ARMCA15_EFUSE=3, FAIL\n");
        else
        	print("ARMCA15_EFUSE=3, PASS\n");
    }
    else if (ARMCA15_EFUSE==4)
    {
        //POSTDIV=1, FOUT:1001~2106
        //set output=2119MHz
        DRV_WriteReg32(ARMCA15PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA15PLL_CON1, 0x80146000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA15PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA15PLL_CON1)&0x07000000)>>24;
        
        print("ARMCA15_EFUSE=4, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x00146000) && (POSDIV==0) )
            print("ARMCA15_EFUSE=4, FAIL\n");
        else
        	print("ARMCA15_EFUSE=4, PASS\n");        
    }
    else if (ARMCA15_EFUSE==5)
    {
        //POSTDIV=1, FOUT:1001~2015
        //set output=2028MHz
        DRV_WriteReg32(ARMCA15PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA15PLL_CON1, 0x80138000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA15PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA15PLL_CON1)&0x07000000)>>24;
        
        print("ARMCA15_EFUSE=5, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x00138000) && (POSDIV==0) )
            print("ARMCA15_EFUSE=5, FAIL\n");
        else
            print("ARMCA15_EFUSE=5, PASS\n");    
    }
    else if (ARMCA15_EFUSE==6)
    {
        //POSTDIV=1, FOUT:1001~1807
        //set output=1820MHz
        DRV_WriteReg32(ARMCA15PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA15PLL_CON1, 0x80118000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA15PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA15PLL_CON1)&0x07000000)>>24;
        
        print("ARMCA15_EFUSE=6, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x00118000) && (POSDIV==0) )
            print("ARMCA15_EFUSE=6, FAIL\n");
        else
        	print("ARMCA15_EFUSE=6, PASS\n");        
    }
    else if (ARMCA15_EFUSE==7)
    {
        //POSTDIV=1, FOUT:1001~1716
        //set output=1729MHz
        DRV_WriteReg32(ARMCA15PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA15PLL_CON1, 0x8010A000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA15PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA15PLL_CON1)&0x07000000)>>24;
        
        print("ARMCA15_EFUSE=7, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x0010A000) && (POSDIV==0) )
            print("ARMCA15_EFUSE=7, FAIL\n");
        else
        	print("ARMCA15_EFUSE=7, PASS\n");        
    }
    else
        print("ARMCA15_EFUSE=%d, ARM ARMCA15_EFUSE illegal\n", ARMCA15_EFUSE);        
    
/*ARMCA7_PLL*/    
    if(ARMCA7_EFUSE==0)
    {
        //no limit
        //set output=2080MHz
        DRV_WriteReg32(ARMCA7PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA7PLL_CON1, 0x80140000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA7PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA7PLL_CON1)&0x07000000)>>24;
        
        print("ARMCA7_EFUSE=0, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x00140000) && (POSDIV==0) )
            print("ARMCA7_EFUSE=0, PASS\n");
        else
        	print("ARMCA7_EFUSE=0, FAIL\n");
    }
    else if (ARMCA7_EFUSE==1)
    {
        //POSTDIV=1, FOUT:1001~1911
        //set output=1924MHz
        DRV_WriteReg32(ARMCA7PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA7PLL_CON1, 0x80128000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA7PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA7PLL_CON1)&0x07000000)>>24;
        
        print("ARMCA7_EFUSE=1, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x00128000) && (POSDIV==0) )
            print("ARMCA7_EFUSE=1, FAIL\n");
        else
        	print("ARMCA7_EFUSE=1, PASS\n");
    }	
    else if (ARMCA7_EFUSE==2)
    {
        //POSTDIV=1, FOUT:1001~1716
        //set output=1729MHz
        DRV_WriteReg32(ARMCA7PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA7PLL_CON1, 0x8010A000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA7PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA7PLL_CON1)&0x07000000)>>24;
        
        print("ARMCA7_EFUSE=2, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x0010A000) && (POSDIV==0) )
            print("ARMCA7_EFUSE=2, FAIL\n");
        else
        	print("ARMCA7_EFUSE=2, PASS\n");        
    }
    else if (ARMCA7_EFUSE==3)
    {
        //POSTDIV=1, FOUT:1001~1508
        //set output=1521MHz
        DRV_WriteReg32(ARMCA7PLL_CON0, 0x00000101);
        DRV_WriteReg32(ARMCA7PLL_CON1, 0x800EA000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(ARMCA7PLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(ARMCA7PLL_CON1)&0x07000000)>>24;
        
        print("ARMCA7_EFUSE=3, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x000EA000) && (POSDIV==0) )
            print("ARMCA7_EFUSE=3, FAIL\n");
        else
        	print("ARMCA7_EFUSE=3, PASS\n");
    }    
    
    
    
/*MM_PLL*/    
    if (MM_EFUSE==0)
    {
        //no limit
        //set output=747.5MHz
        DRV_WriteReg32(MMPLL_CON0, 0x00000101);
        DRV_WriteReg32(MMPLL_CON1, 0x821CC000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(MMPLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(MMPLL_CON1)&0x07000000)>>24;
        
        print("MM_EFUSE=0, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x001CC000) && (POSDIV==2) )
            print("MM_EFUSE=0, PASS\n");
        else
            print("MM_EFUSE=0, FAIL\n");
    }
    else if (MM_EFUSE==1)
    {
        //POSTDIV=4, FOUT:484.25~702
        //set output=705.25MHz
        DRV_WriteReg32(MMPLL_CON0, 0x00000101);
        DRV_WriteReg32(MMPLL_CON1, 0x821B2000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(MMPLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(MMPLL_CON1)&0x07000000)>>24;
        
        print("MM_EFUSE=1, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x001B2000) && (POSDIV==2) )
            print("MM_EFUSE=1, FAIL\n");
        else
            print("MM_EFUSE=1, PASS\n");
    }
    else if (MM_EFUSE==2)
    {
        //POSTDIV=4, FOUT:484.25~650
        //set output=653.25MHz
        DRV_WriteReg32(MMPLL_CON0, 0x00000101);
        DRV_WriteReg32(MMPLL_CON1, 0x82192000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(MMPLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(MMPLL_CON1)&0x07000000)>>24;
        
        print("MM_EFUSE=2, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x00192000) && (POSDIV==2) )
            print("MM_EFUSE=2, FAIL\n");
        else
        	print("MM_EFUSE=2, PASS\n");        
    }
    else if (MM_EFUSE==3)
    {
        //POSTDIV=4, FOUT:484.25~702
        //set output=705.25MHz
        DRV_WriteReg32(MMPLL_CON0, 0x00000101);
        DRV_WriteReg32(MMPLL_CON1, 0x801B2000);
        
        gpt_busy_wait_us(200);
        
        PCW = (DRV_Reg32(MMPLL_CON2)&0x001FFFFF)>>0;
        POSDIV = (DRV_Reg32(MMPLL_CON1)&0x07000000)>>24;
        
        print("MM_EFUSE=3, PCW=%d, POSDIV=%d \n", PCW, POSDIV);
        if( (PCW==0x001B2000) && (POSDIV==2) )
            print("MM_EFUSE=0, FAIL\n");
        else
        	print("MM_EFUSE=0, PASS\n");
    }
    else
        print("MM_EFUSE=%d, MM EFUSE illegal\n", MM_EFUSE); 
	
    return;
}
#endif
