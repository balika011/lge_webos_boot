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

 * $RCSfile:  $

 * $Revision: #1 $

 *
 *---------------------------------------------------------------------------*/
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------



#include "x_typedef.h"
#include "x_hal_arm.h"

#include "x_hal_5381.h"

#include "x_bim.h"
#include "x_assert.h"
#include "drvcust_if.h"
#include "c_model.h"

extern 	void CHIP_Delay_us(UINT32 u4Micros);
void BIM_CpuDie(UINT32 cpu)
{
	unsigned int regval32;

	if(cpu == 0)
	{
	    //Printf("we should not turn off CPU%u\n", cpu);
		return;
	}

	if(cpu >= 4)
	{
	    //Printf("we can't handle CPU%u\n", cpu);
		return;
	}

#if defined(CC_MT5890)
	regval32 = BIM_READ32(REG_CPU0_PWR_CTL + 4 * cpu);
    if (BSP_GetIcVersion() < IC_VER_5890_AC)
    {
        regval32 |= CKISO;
    }
    else
    {
        regval32 |= ES3_CKISO;
    }
	BIM_WRITE32(REG_CPU0_PWR_CTL + 4 * cpu, regval32);

    CHIP_Delay_us(20);

	#ifndef CONFIG_MT53_FPGA
	regval32 = BIM_READ32(REG_L1_PD_CTL);
	regval32 |= (1 << cpu);
	BIM_WRITE32(REG_L1_PD_CTL, regval32);

    if (BSP_GetIcVersion() < IC_VER_5890_AC)
    {
    	do {
    		regval32 = BIM_READ32(REG_L1_PD_CTL);
    	}while(!(((regval32 & L1_SRAM_ACK) >> 4) & (1 << cpu)));
    }
    else
    {
    	do {
    		regval32 = BIM_READ32(REG_L1_PD_CTL);
    	}while(!(((regval32 & ES3_L1_SRAM_ACK) >> 2) & (1 << cpu)));
    }
	#endif

    CHIP_Delay_us(20);

    // Set reset enable bit to "1"
   	regval32 = BIM_READ32(REG_CPU0_PWR_CTL + 4 * cpu);
	regval32 |= PWR_RST_EN;
	BIM_WRITE32(REG_CPU0_PWR_CTL + 4 * cpu, regval32);

    CHIP_Delay_us(20);

	regval32 = BIM_READ32(REG_CA15ISOLATECPU);
	regval32 &= ~(1 << cpu);
	BIM_WRITE32(REG_CA15ISOLATECPU, regval32);

    CHIP_Delay_us(20);

	regval32 = BIM_READ32(REG_CPU0_PWR_CTL + 4 * cpu);
	regval32 &= ~PWR_ON;
	BIM_WRITE32(REG_CPU0_PWR_CTL + 4 * cpu, regval32);

    if (BSP_GetIcVersion() < IC_VER_5890_AC)
    {
    	do {
    		regval32 = BIM_READ32(REG_CPU0_PWR_CTL + 4 * cpu);
    	} while( regval32 & PWR_ON_ACK);
    }
    else
    {
    	do {
    		regval32 = BIM_READ32(REG_CPU0_PWR_CTL + 4 * cpu);
    	} while( regval32 & ES3_PWR_ON_ACK);
    }
    CHIP_Delay_us(20);

	regval32 = BIM_READ32(REG_CPU0_PWR_CTL + 4 * cpu);
    if (BSP_GetIcVersion() < IC_VER_5890_AC)
    {
	    regval32 &= ~PWR_ON_2ND;
    }
    else
    {
	    regval32 &= ~ES3_PWR_ON_2ND;
    }
	BIM_WRITE32(REG_CPU0_PWR_CTL + 4 * cpu, regval32);

    if (BSP_GetIcVersion() < IC_VER_5890_AC)
    {
    	do {
    		regval32 = BIM_READ32(REG_CPU0_PWR_CTL + 4 * cpu);
    	}while( regval32 & PWR_ON_2ND_ACK);
    }
    else
    {
    	do {
    		regval32 = BIM_READ32(REG_CPU0_PWR_CTL + 4 * cpu);
    	}while( regval32 & ES3_PWR_ON_2ND_ACK);
    }
    CHIP_Delay_us(20);

    // disable CPU1/2/3 wait for other CPUs power down ack
    *(volatile unsigned int *)(0xf0078168) = 0x7bd0;
    #elif defined(CC_MT5882)
	cpu=cpu-1;
	//set pd to "1" //ok
	regval32 = BIM_READ32(REG_CPU1_PWR_CTL + 4 * cpu);
	regval32 |= MEM_PD;
	BIM_WRITE32(REG_CPU1_PWR_CTL + 4 * cpu,regval32);
	CHIP_Delay_us(20);
	do {
		regval32 = BIM_READ32(REG_CPU1_PWR_CTL + 4 * cpu);
	}while(!(regval32 & MEM_PD_ACK));

	 // Set clamp bit to "1" //ng
	regval32 = BIM_READ32(REG_CPU1_PWR_CTL + 4 * cpu);
	regval32 |= CLAMP;
	BIM_WRITE32(REG_CPU1_PWR_CTL + 4 * cpu,regval32);
	CHIP_Delay_us(20);

    // Set pwr clk_dis bit to "1"
	regval32 = BIM_READ32(REG_CPU1_PWR_CTL + 4 * cpu);
	regval32 |= PWR_CLK_DIS;
	BIM_WRITE32(REG_CPU1_PWR_CTL + 4 * cpu,regval32);
	CHIP_Delay_us(20);

   // Set reset enable bit to "1"
	regval32 = BIM_READ32(REG_CPU1_PWR_CTL + 4 * cpu);
	regval32 |= PWR_RST_EN;
	BIM_WRITE32(REG_CPU1_PWR_CTL + 4 * cpu,regval32);
	CHIP_Delay_us(20);

	//set pwr on to "0" //ok
	regval32 = BIM_READ32(REG_CPU1_PWR_CTL + 4 * cpu);
	regval32 &= ~PWR_ON;
	BIM_WRITE32(REG_CPU1_PWR_CTL + 4 * cpu,regval32);

	do {
		regval32 = BIM_READ32(REG_CPU1_PWR_CTL + 4 * cpu);
	}while( regval32 & PWR_ON_ACK);
	CHIP_Delay_us(20);

	//set pwr on 2nd to "0" //ng
	regval32 = BIM_READ32(REG_CPU1_PWR_CTL + 4 * cpu);
	regval32 &= ~PWR_ON_2ND;
	BIM_WRITE32(REG_CPU1_PWR_CTL + 4 * cpu,regval32);

	do {
		regval32 = BIM_READ32(REG_CPU1_PWR_CTL + 4 * cpu);
	}while( regval32 & PWR_ON_2ND_ACK);
	CHIP_Delay_us(20);

    #endif // defined(CC_MT5890)
}

void BIM_ClusterUp(UINT32 u4Cluster)
{
    #if defined(CC_MT5890)
	unsigned int regval32;

	if(u4Cluster != 1)
	{
	    //Printf("we only turn on cluster1, but not %u\n", u4Cluster);
		return;
	}

    //
    // CPUSYS power on
    //
	regval32 = BIM_READ32(REG_CA7_CPUSYS_TOP_PWR);
	regval32 |= CA7_CPUSYS_PWR_ON;
	BIM_WRITE32(REG_CA7_CPUSYS_TOP_PWR, regval32);

    #ifndef CC_FPGA
	do {
		regval32 = BIM_READ32(REG_CA7_CPUSYS_TOP_PWR);
	} while(!(regval32 & CA7_CPUSYS_PWR_ACK));
    #endif

	regval32 = BIM_READ32(REG_CA7_CPUSYS_TOP_PWR);
	regval32 |= CA7_CPUSYS_PWR_2ND_ON;
	BIM_WRITE32(REG_CA7_CPUSYS_TOP_PWR, regval32);

	do {
		regval32 = BIM_READ32(REG_CA7_CPUSYS_TOP_PWR);
	} while(!(regval32 & CA7_CPUSYS_PWR_ACK_2ND));

    regval32 = BIM_READ32(REG_CA7_CPUSYS_PD);
    regval32 &= ~CA7_CPUSYS_PD;
    BIM_WRITE32(REG_CA7_CPUSYS_PD, regval32);

    do {
        regval32 = BIM_READ32(REG_CA7_CPUSYS_PD);
    } while(regval32 & CA7_CPUSYS_PD_ACK);

    regval32 = BIM_READ32(REG_CA7_CPUSYS_PD);
    regval32 |= CA7_CPUSYS_ISOINTB;
    BIM_WRITE32(REG_CA7_CPUSYS_PD, regval32);

    regval32 = BIM_READ32(REG_CA7_CPUSYS_PD);
    regval32 &= ~CA7_CPUSYS_CKISO;
    BIM_WRITE32(REG_CA7_CPUSYS_PD, regval32);

    regval32 = BIM_READ32(REG_CA7_CPUSYS_TOP_PWR);
    regval32 &= ~CA7_CPUSYS_TOP_CLAMP;
    BIM_WRITE32(REG_CA7_CPUSYS_TOP_PWR, regval32);

    regval32 = BIM_READ32(REG_CA7_CPUSYS_TOP_PWR);
    regval32 &= ~CA7_CPUSYS_CLK_DIS;
    BIM_WRITE32(REG_CA7_CPUSYS_TOP_PWR, regval32);

    regval32 = BIM_READ32(REG_CA7_CPUSYS_TOP_PWR);
    regval32 &= ~CA7_CPUSYS_RST_EN;
    BIM_WRITE32(REG_CA7_CPUSYS_TOP_PWR, regval32);

    // check CPU WFI/WFE status

    regval32 = BIM_READ32(REG_CA7_CACTIVE);
    regval32 |= CA7_PWRDNREQN;
    BIM_WRITE32(REG_CA7_CACTIVE, regval32);

	do {
		regval32 = BIM_READ32(REG_CA7_CACTIVE);
	} while(!(regval32 & CA7_PWRDNACKN));


    //
    // dbg power on
    //
	regval32 = BIM_READ32(REG_CA7_DBG_PWR);
	regval32 |= CA7_DBG_PWR_ON;
	BIM_WRITE32(REG_CA7_DBG_PWR, regval32);

	do {
		regval32 = BIM_READ32(REG_CA7_DBG_PWR);
	} while(!(regval32 & CA7_DBG_PWR_ACK));

	regval32 = BIM_READ32(REG_CA7_DBG_PWR);
	regval32 |= CA7_DBG_PWR_2ND_ON;
	BIM_WRITE32(REG_CA7_DBG_PWR, regval32);

	do {
		regval32 = BIM_READ32(REG_CA7_DBG_PWR);
	} while(!(regval32 & CA7_DBG_PWR_ACK_2ND));

    regval32 = BIM_READ32(REG_CA7_DBG_PWR);
    regval32 &= ~CA7_DBG_CLAMP;
    BIM_WRITE32(REG_CA7_DBG_PWR, regval32);

    regval32 = BIM_READ32(REG_CA7_DBG_PWR);
    regval32 &= ~CA7_CPUSYS_CLK_DIS;
    BIM_WRITE32(REG_CA7_DBG_PWR, regval32);

    regval32 = BIM_READ32(REG_CA7_DBG_PWR);
    regval32 &= ~CA7_DBG_RST_EN;
    BIM_WRITE32(REG_CA7_DBG_PWR, regval32);

    // enable EVENTI pin for wfe/sev communication cross cluster
    regval32 = *(UINT32 *)0xf007801c;
    regval32 = regval32 | (1 << 14) | (1 << 15);
    *(UINT32 *)0xf007801c = regval32;

    // enable ca7 cci snoop
    regval32 = *(UINT32 *)0xf007811c;
    regval32 &= ~(1 << 4);
    *(UINT32 *)0xf007811c = regval32;

    // enable ca17 cci snoop
    regval32 = *(UINT32 *)0xf007820c;
    regval32 &= ~(1 << 0);
    *(UINT32 *)0xf007820c = regval32;

    // enable ca17 cci snoop
    regval32 = *(UINT32 *)0xf2094000;
    regval32 |= 0x3;
    *(UINT32 *)0xf2094000 = regval32;

    // enable ca7 cci snoop
    regval32 = *(UINT32 *)0xf2095000;
    regval32 |= 0x3;
    *(UINT32 *)0xf2095000 = regval32;

    #endif // defined(CC_MT5890)
}

void BIM_ClusterUpCA17Only(UINT32 u4Cluster)
{

    #if defined(CC_MT5890)
	unsigned int regval32;

    if(BSP_GetIcVersion() == IC_VER_5890_AC)
    {
        // enable ca17 cci snoop
        regval32 = *(UINT32 *)0xf007820c;
        regval32 &= ~(1 << 0);
        *(UINT32 *)0xf007820c = regval32;
    }
    #endif // defined(CC_MT5890)
}

void BIM_CA7_CpuUp(UINT32 cpu)
{
    #if defined(CC_MT5890)
	unsigned int regval32;

    regval32 = BIM_READ32(REG_CA7_CORE0_PWR + 4 * cpu);
    regval32 |= CA7_CPU_PWR_ON;
    BIM_WRITE32(REG_CA7_CORE0_PWR + 4 * cpu, regval32);

    do {
        regval32 = BIM_READ32(REG_CA7_CORE0_PWR + 4 * cpu);
    } while(!(regval32 & CA7_CPU_PWR_ACK));

    regval32 = BIM_READ32(REG_CA7_CORE0_PWR + 4 * cpu);
    regval32 |= CA7_CPU_PWR_2ND_ON;
    BIM_WRITE32(REG_CA7_CORE0_PWR + 4 * cpu, regval32);

    do {
        regval32 = BIM_READ32(REG_CA7_CORE0_PWR + 4 * cpu);
    } while(!(regval32 & CA7_CPU_PWR_ACK_2ND));


    regval32 = BIM_READ32(REG_CA7_CORE0_PD + 4 * cpu);
    regval32 &= ~CA7_CPU_PD;
    BIM_WRITE32(REG_CA7_CORE0_PD + 4 * cpu, regval32);

    do {
        regval32 = BIM_READ32(REG_CA7_CORE0_PD + 4 * cpu);
    } while(regval32 & CA7_CPU_PD_ACK);

    regval32 = BIM_READ32(REG_CA7_CORE0_PD + 4 * cpu);
    regval32 |= CA7_CPU_ISOINTB;
    BIM_WRITE32(REG_CA7_CORE0_PD + 4 * cpu, regval32);


    regval32 = BIM_READ32(REG_CA7_CORE0_PD + 4 * cpu);
    regval32 &= ~CA7_CPU_CKISO;
    BIM_WRITE32(REG_CA7_CORE0_PD + 4 * cpu, regval32);

    regval32 = BIM_READ32(REG_CA7_CORE0_PWR + 4 * cpu);
    regval32 &= ~CA7_CPU_CLAMP;
    BIM_WRITE32(REG_CA7_CORE0_PWR + 4 * cpu, regval32);

    regval32 = BIM_READ32(REG_CA7_CORE0_PWR + 4 * cpu);
    regval32 &= ~CA7_CPU_CLK_DIS;
    BIM_WRITE32(REG_CA7_CORE0_PWR + 4 * cpu, regval32);

    regval32 = BIM_READ32(REG_CA7_CORE0_PWR + 4 * cpu);
    regval32 &= ~CA7_CPU_RST_EN;
    BIM_WRITE32(REG_CA7_CORE0_PWR + 4 * cpu, regval32);
    #endif // defined(CC_MT5890)
}

