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

#include "x_typedef.h"

#ifndef CC_UBOOT
#ifndef CC_SECURE_WORLD
#include "x_os.h"
#endif
#endif

#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_ckgen.h"
#include "x_bim.h"
#include "x_pdwnc.h"
#include "x_printf.h"
#include "x_serial.h"
#include "x_dram.h"
#include "x_assert.h"
#include "x_lint.h"
#include "x_timer.h"
#include "drvcust_if.h"
#include "c_model.h"
#include "chip_txt.h"
#include <string.h>
#include "ddr.h"
#include "x_hal_5381.h"

extern void CHIP_Delay_us(UINT32 u4Micros);
extern void CHIP_DisplayString(const CHAR *szString);
extern void CHIP_DisplayInteger(INT32 i);
extern void CHIP_DisplayHex(UINT32 u4Val);

//lint --e{961} Violates MISRA Advisory Rule 28, 'register' class
//lint --e{950} No ANSI reserved word (__asm)
//lint --e{529} Symbol not subsequently referenced
//lint --e{506} Constant value Boolean
//lint --e{774} Boolean within 'if' always evaluates to constant
//lint --e{775} non-negative quantity cannot be less than zero
LINT_SUPPRESS_SYMBOL(18, Printf)


//-----------------------------------------------------------------------------
// Config definition
//-----------------------------------------------------------------------------

#ifndef CC_INIT_BOOT_VERBOSE
#define CC_INIT_BOOT_VERBOSE        1
#endif /* CC_INIT_BOOT_VERBOSE */

#if defined(CC_MTK_PRELOADER) && defined(NDEBUG)
#define Printf(fmt...)
#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */

//-----------------------------------------------------------------------------
// Constant definition
//-----------------------------------------------------------------------------

#define ICACHE_ENABLE               (1 << 12)
#define SDR_32_TURBO                0x00000570



#define KEY_ESCAPE                      0x1b

//-----------------------------------------------------------------------------
// Public functions declaration
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
EXTERN UINT32 g_u4DramSize;
EXTERN UINT32 g_u4DramChBSize;
#endif /* CC_MTK_LOADER */

EXTERN void RemapJumpToDram(void);

//-----------------------------------------------------------------------------
// This file
//-----------------------------------------------------------------------------

EXTERN UINT32 IsInitBootVerbose(void);
EXTERN UINT32 IsBootVerbose(void);
EXTERN void BootInit(void);
EXTERN void BootBimInitial(void);
EXTERN void HalSetInstTCMReg(UINT32 u4BaseAddr, UINT32 u4Size, BOOL fgEnable);

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
/** IsInitBootVerbose() return assembly verbose flag, only call from assembly.
 *  @retval 1 to verbose message, otherwise quite.
 */
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER

UINT32 IsInitBootVerbose()
{
    return CC_INIT_BOOT_VERBOSE;
}

UINT32 IsBootVerbose()
{
    return 1; // DRVCUST_InitGet(eFlagBootVerbose);
}

void BootInit()
{
#if 0
    register UINT32 r, zero;

    r = ((UINT32)g_pu4TIME_Array_Head) | 5; // size 8KB & enable.
    __asm       // ITCM
    {
        MCR p15, 0, r, c9, c1, 1
    }

    g_pu4TIME_Array_Head[0] = 15;

    if (BIM_READ32(REG_RW_REMAP) == 0)
    {
        g_pu4TIME_Array_Head[0] = 1;
        zero = 0;
        for (r=1; r<32; r++)        // 128 bytes data needed.
        {
            g_pu4TIME_Array_Head[r] = zero;
        }
    }
#endif
    BIM_WRITE32(REG_MISC_IRQEN, 0);
    BIM_WRITE32(REG_RW_IRQEN, 0);
  //  PDWNC_WRITE32(REG_RW_PDWNC_INTEN, 0);

    if (((BIM_READ32(REG_RW_TIMER_CTRL) & (TMR2_CNTDWN_EN)) == 0) ||
        (BIM_READ32(REG_RW_REMAP) == 0))
    {
        BIM_WRITE32(REG_RW_TIMER2_LLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_LOW , 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HIGH, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER_CTRL, (TMR2_CNTDWN_EN | TMR2_AUTOLD_EN));
    }
}

void BootBimInitial()
{
    // unlock UART0 output pad to display.
//    PDWNC_WRITE32(REG_RW_IOPADEN, PDWNC_READ32(REG_RW_IOPADEN) | PAD_UR0_TX_EN);//vIO32Write4B(PDWNC_GPIOEN, u4IO32Read4B(PDWNC_GPIOEN) | PAD_UR0_TX_EN);
    //HAL_WRITE32(PDWNC_GPIOEN1, HAL_READ32(PDWNC_GPIOEN1) | PAD_UR0_TX_EN);

    // set bus access timeout to avoid register access halting CPU.
    BIM_WRITE32(REG_RW_TIMEOUT_CTRL, TO_FLASH_CNT | TO_DRAM_CNT | TO_IOBUS_CNT | TO_FLASH_EN | TO_DRAM_EN | TO_IOBUS_EN);

    // Disable and clear interrupt.
    BIM_WRITE32(REG_IRQEN, 0);
    BIM_WRITE32(REG_FIQEN, 0);
    BIM_WRITE32(REG_IRQCL, 0xffffffff);
    BIM_WRITE32(REG_FIQCL, 0xffffffff);
   
 #ifndef CC_SECURE_WORLD
    #ifdef CC_ARM_GIC
    GIC_Init();
    #endif
#endif

}
#endif /* CC_MTK_LOADER */




//-----------------------------------------------------------------------------
// Public functions.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/* CLK_QueryDramSetting() for user to modify TCM_DRAM_SETUP value of the DRAM
 *      controller parameters.
 */
//-----------------------------------------------------------------------------

#ifdef CC_MTK_PRELOADER

#if defined(CC_FPGA)
//-----------------------------------------------------------------------------
/* CLK_SdrCalibrate() for SDR calibration
 *
 */
//-----------------------------------------------------------------------------
UINT32 CLK_SdrCalibrate(void)
{
    //The following code is suggested by Maoauo Lin, 2009/09/09.
    *((volatile UINT32*)(IO_VIRT + 0x07000)) = 0x338B1165;
    *((volatile UINT32*)(IO_VIRT + 0x07004)) = 0x03015007;
    *((volatile UINT32*)(IO_VIRT + 0x07010)) = 0x00000022;

    *((volatile UINT32*)(IO_VIRT + 0x0700C)) = 0x00000200; // Trigger auto initialization

#ifdef __GCC__
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
#else
    __asm
    {
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
    }
#endif
    *((volatile UINT32*)(IO_VIRT + 0x0700C)) = 0x00000000;

    *((volatile UINT32*)(IO_VIRT + 0x0701C)) = 0x03300080;

    *((volatile UINT32*)(IO_VIRT + 0x07008)) = 0x0000020A;

    #ifndef CC_FPGA_SDR64B
    // 128 bits DRAM setting => 2 DRAM modules on fpga.
    *((volatile UINT32*)(IO_VIRT + 0x07018)) = 0xC0021410;
    #else
    // 64 bits DRAM setting => 1 DRAM module on fpga.
    *((volatile UINT32*)(IO_VIRT + 0x07018)) = 0xC0021010;
    #endif

#ifdef CC_FPGA
    TCMSET_CHANNELA_SIZE(FPGA_DRAM_SIZE_MB);
#else
    TCMSET_CHANNELA_SIZE(_DDR_CheckSize(FALSE)/0x100000);
#endif
    CHIP_DisplayString("DRAM Size = ");
    CHIP_DisplayInteger(TCMGET_CHANNELA_SIZE());
    CHIP_DisplayString(" Mbytes.\n");

    return 0;
}

void CLK_DramCSelftest(void)
{
  UINT32 u4Val;
  *((volatile UINT32*)(IO_VIRT + 0x06100)) = 0x0;
  *((volatile UINT32*)(IO_VIRT + 0x06104)) = 0x00800000;
  *((volatile UINT32*)(IO_VIRT + 0x0611c)) = 0x000033aa;
  *((volatile UINT32*)(IO_VIRT + 0x06118)) = 0x1400110d;

 // *((volatile UINT32*)(IO_VIRT + 0x0707c)) = 0x8750;
 // *((volatile UINT32*)(IO_VIRT + 0x071d8)) = 0xc80030;
 // IO_READ32(ANA_PLL_GROUP_BASE, 0x28);
 CHIP_Delay_us(2000);
  u4Val = IO_READ32(IO_VIRT+0x6000,0x140);
  if(u4Val==0x0101)
  	{
	CHIP_DisplayChar('P');
	CHIP_DisplayChar('-');
  	}
  else
  	{
    CHIP_DisplayChar('F');
	CHIP_DisplayChar('-');
  	}
}

#endif /* #if defined(CC_FPGA) */

#if defined(CC_MTK_PRELOADER) && (defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG))
static void _CLK_DisplayInfo(UINT32 u4Stage)
{
    if (u4Stage == DRAM_CHANGE_CLK_SETTING)
    {
        /*Stage 1.*/
        CHIP_DisplayString("CLK: ");
        CHIP_DisplayInteger((TCMGET_DDR_CLK()/BASE_DDR_CLK));
        CHIP_DisplayString("MHz ('[' or ']' to change)\n");

        if (IS_DDR_DMSSON())
        {
            CHIP_DisplayString("DMSS: On ('\\' to change)\n");
        }
        else
        {
            CHIP_DisplayString("DMSS: Off ('\\' to change)\n");
        }

        CHIP_DisplayString("CAS Latency: ");
        CHIP_DisplayInteger(TCMGET_DDR_CL());
        CHIP_DisplayString("('T' to change)\n");
		CHIP_DisplayString("DDR Clock:");
		CHIP_DisplayInteger(TCMGET_DDR_CLK()/BASE_DDR_CLK);
		CHIP_DisplayString(" MHz ('-' to manual setting)\n");
    }
	#if 0
	else if(u4Stage == DRAM_GEN_TEST_PATTERN_AFTER_CALIBRATION)
	{
		/*Stage 2.*/
		CHIP_DisplayString("Current Channel: ");
        CHIP_DisplayInteger(IS_DRAM_CHANNELB_ACTIVE());
        CHIP_DisplayString(" [ 'c' to change channel ]\n");
		CHIP_DisplayString("'a' to andio pattern, 'x' to xtalk pattern \n");
	}
	#endif
	#if 0
	else if(u4Stage == DRAM_CHANGE_CLK_PI_SETTING)
	{
		/*Stage 3.*/
		CHIP_DisplayString("Current Channel: ");
        CHIP_DisplayInteger(IS_DRAM_CHANNELB_ACTIVE());
		CHIP_DisplayString("\n");
		if(!IS_DRAM_CHANNELB_ACTIVE())
		{
			CHIP_DisplayString("Current Clock PI value: ");
			CHIP_DisplayHex(ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d4)) & 0x7f);
			CHIP_DisplayString("\n");
			CHIP_DisplayString("Current Clock Cmd/Addr PI value: ");
			CHIP_DisplayHex((ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d4)) & 0x007f0000)>>16);
			CHIP_DisplayString("\n");
		}
		else
		{
			CHIP_DisplayString("Current Clock PI value: ");
			CHIP_DisplayHex(ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2d8)) & 0x7f);
			CHIP_DisplayString("\n");
			CHIP_DisplayString("Current Clock Cmd/Addr PI value: ");
			CHIP_DisplayHex((ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x4d4)) & 0x007f0000)>>16);
			CHIP_DisplayString("\n");
		}
		CHIP_DisplayString("'[' or ']' to change\n");
	}
	#endif
	
	#if 1
	if (u4Stage == DRAM_CHANGE_CLK_SCRAMBLE_SETTING)
    {	
        /*Stage 4.*/
		UINT32 u4temp,u4temp1=0;
		u4temp=ucDram_Register_Read(DRAM_BASE+DRAMC_REG_OCDK);
		u4temp1|=(u4temp>>18&0x3)|(u4temp>>22&0x3)<<2|(u4temp>>26&0x3)<<4|(u4temp>>30&0x3)<<6;
		Printf("SCRAMBLE: CHANNELA=%d , CHANNELB=%d \n",u4temp1,u4temp1);

		Printf("click 'd' to enter one step change mode , click 's' to enter self setting mode .");
		
    }
	#endif
	
}

static void _CLK_DisplayMenu(UINT32 u4Stage)
{
    CHIP_DisplayString("\n--\n");
    _CLK_DisplayInfo(u4Stage);
    CHIP_DisplayString("Press 'b' to download\n");
    CHIP_DisplayString("Press '0' to exit:\n\n");
}
#endif /* #if defined(CC_MTK_PRELOADER) && (defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG)) */


#ifdef CC_MTK_PRELOADER

#ifdef DRAM_LOAD_BOARD
void DDR_InitLoadBoard(void)
{
    LoadBoardGpioInit();
	LoadBoardShowResult(FLAG_INITIAL, FLAG_CALIBRATION_PASS, FLAG_NOT_COMPLETE_OR_FAIL);
	//while(1);
}
#endif

static void _CLKResetTCM(UINT32 u4DefClk)
{
    UINT32 u4Ret;
    UINT32 u4DramType = 0;
	UINT32 u4Val;

    TCM_DRAM_SIZE = 0;
    TCM_DRAM_FLAGS = 0;
    TCM_DRAM_FLAGS1 = 0;
    TCM_DRAM_FLAGS2 = 0;
    TCM_DRAM_FLAGS3 = 0;
    TCM_DRAM_FLAGS4 = 0;

	TCM_DRAM_DLYCELL_PERT = 0x3131;

    // Set DRAM type.
    u4Ret = DRVCUST_InitGet(eDramType);
    TCM_DRAM_FLAGS |= ((u4Ret & 0xF) << TYPE_SHIFT);
    u4DramType = u4Ret;

    // Set DRAM clock.
    TCM_DRAM_FLAGS |= (((u4DefClk) / BASE_DDR_CLK) & DRAM_CLOCK_MASK);

    // Set DRAM DMSS on/off.
    if ((DRVCUST_InitGet(eDmpllSpectrumPermillage) != 0) &&
         (DRVCUST_InitGet(eDmpllSpectrumFrequency) != 0))
    {
        TCM_DRAM_FLAGS |= DRAM_DMSSON;
    }

	//dc-balance on/off.
#if (DEFAULT_DRAM_PCB_CHK == 1)
	u4Val = u4IO32Read4B(0xf0028304);
	vIO32Write4B(0xf0028304, (u4Val | 0x01));
	mcDELAY_us(500);
	u4Val = u4IO32Read4B(0xf0028374);
	u4Val = u4Val & 0x3FF;
	if ((u4Val > 0x35A)||(u4Val < 0x159))
	{
		CHIP_DisplayString("PCB with RC \n");
		TCM_DRAM_FLAGS |=  DRAM_DC_BALANCE;
	}
	else
	{
		CHIP_DisplayString("PCB without RC \n");
		TCM_DRAM_FLAGS &=  ~DRAM_DC_BALANCE;
	}
#else
	TCM_DRAM_FLAGS |=  (DRVCUST_InitGet(eFlagDDRDCBalance)) ? DRAM_DC_BALANCE : 0;
#endif

	// Default enter normal state
	TCM_DRAM_FLAGS &= ~DRAM_SUSPEND_STATE;

    // Set if channel B is support.
    if ((u4DramType == DDR_II_x3) || (u4DramType == DDR_III_x3))
    {
        TCM_DRAM_FLAGS |= DRAM_CHANNELB_SUPPORT;
        TCM_DRAM_FLAGS |= DRAM_CHB_FORCE32;
    }
    else if((u4DramType == DDR_II_x4) || (u4DramType == DDR_III_x4))
    {
    	TCM_DRAM_FLAGS |= DRAM_CHANNELB_SUPPORT;
    }

    // Set 16 bit mode or 32 bit mode data bus.
    if ((u4DramType == DDR_II_x1) ||(u4DramType == DDR_III_x1))
    {
        TCM_DRAM_FLAGS |= DRAM_CHA_FORCE32;
    }

    // Set bus is x8 or x16.
    TCM_DRAM_FLAGS |=  DRVCUST_InitGet(eDdrBusX8) ? DRAM_BUSX8 : 0;

    // Set CAS latency by DRAM clock.
	if(u4DefClk <= CLK_1600MHZ)
    {
        TCM_DRAM_FLAGS |= ((11) << CL_SHIFT);
    }
	else if(u4DefClk <= CLK_1866MHZ)
    {
        TCM_DRAM_FLAGS |= ((13) << CL_SHIFT);
    }
	else
	{
		TCM_DRAM_FLAGS |= ((14) << CL_SHIFT);
	}
}
#endif /* #ifdef CC_MTK_PRELOADER */

//-----------------------------------------------------------------------------
/* CLK_QueryDramSetting() for user to modify TCM_DRAM_SETUP value of the DRAM
 *      controller parameters.
 */
//-----------------------------------------------------------------------------
#if (!defined(__MODEL_slt__)) && defined(CC_MTK_PRELOADER) && (defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG))
void CLK_QueryDramSetting(UINT8 *szString, UINT32 u4Stage)
{
		UINT32 u4Val;
		U8 u1ClkPIDelay, u1CmdPIDelay;
		UINT32 i, u4CpuClk,u4temp,u4temp1;
		UCHAR uc,uc1;
	
		u4CpuClk = CLK_200MHZ;
		// Wait a moment for ESC to enter setup menu
		CHIP_DisplayString((const char *)szString);
		SerTransparent();
		uc = 0;
		CHIP_DisplayString("\n");
		for (i=0; i<(u4CpuClk >> 15); i++)
		{
			CHIP_DisplayString("\r");
			switch(i & 0x3) {
			case 0:
				CHIP_DisplayString("|"); break;
			case 1:
				CHIP_DisplayString("/"); break;
			case 2:
				CHIP_DisplayString("-"); break;
			case 3:
				CHIP_DisplayString("\\"); break;
			default:
				continue;
			}
	
			if (SerInByte(&uc))
			{
				if (uc == KEY_ESCAPE)
				{
					break;
				}
			}
		}
	
		// Without ESC, just return.
		if (uc != KEY_ESCAPE)
		{
			SerNormalMode();
			CHIP_DisplayString("\n");
			return;
		}
	
		// Otherwise, enter menu loop.
		while (1)
		{
			_CLK_DisplayMenu(u4Stage);
	
			// wait a key.
			while (!SerInByte(&uc)) { }
	
			if (u4Stage == DRAM_CHANGE_CLK_SETTING)
			{
				switch(uc)
				{
				case ']':
					ADD_DDR_CLK();
					break;
				case '[':
					SUB_DDR_CLK();
					break;
				#if 0	
				case '\\':
					// Toggle DMSS on/off status.
					if (IS_DDR_DMSSON())
					{
						TCM_DRAM_FLAGS &= ~DRAM_DMSSON;
					}
					else
					{
						TCM_DRAM_FLAGS |= DRAM_DMSSON;
					}
					break;
	
				case 'T': case 't':
					u4Val = TCMGET_DDR_CL();
					u4Val++;
					if (u4Val > 11)
					{
						u4Val = 6;
					}
					TCM_DRAM_FLAGS &= ~(DRAM_DDR_CL_MASK);
					TCM_DRAM_FLAGS |= ((u4Val & 0xF) << CL_SHIFT);
					break;

				case '_': case '-':
					CHIP_DisplayString("Enter DRAM clock:\n");
					while (!SerInByte(&uc)) { }
					// Ascii to integer.
					u4Val = (uc-0x30)*1000;
					while (!SerInByte(&uc)) { }
					u4Val += (uc-0x30)*100;
					while (!SerInByte(&uc)) { }
					u4Val += (uc-0x30)*10;
					while (!SerInByte(&uc)) { }
					u4Val += (uc-0x30);
	
					CHIP_DisplayString("Set DRAM clock = ");
					CHIP_DisplayInteger(u4Val);
					CHIP_DisplayString(" MHz.\n");
	
					TCM_DRAM_FLAGS &= ~(DRAM_CLOCK_MASK);
					TCM_DRAM_FLAGS |= (u4Val & DRAM_CLOCK_MASK);
					// Clear uc to aviod exit.
					uc = 0;
					break;
				#endif
				
				default:
					break;
				}
			}
			#if 0
			else if(u4Stage == DRAM_GEN_TEST_PATTERN_AFTER_CALIBRATION)
			{
				switch(uc)
				{
				case 'c': case 'C':
					if(!IS_DRAM_CHANNELB_ACTIVE())
					{
						TCMSET_CHANNELB_ACTIVE();
					}
					else
					{
						TCMSET_CHANNELA_ACTIVE();
					}
					break;
				case 'a': case 'A':
					u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3));
					mcSET_FIELD0(u4Val, 0xf, 0x0000000f, 0);
					mcSET_BIT(u4Val,7);
					ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4Val);

					u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3));
					mcSET_BIT(u4Val,7);
					ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4Val);
				
					while(1)
					{
						u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
            			mcSET_FIELD(u4Val, (U32) 4, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
            			ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4Val);

						u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
						while(mcCHK_BIT1(u4Val, POS_TESTRPT_DM_CMP_CPT) == 0)
        				{
            				u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
            				CHIP_Delay_us(1);
						}
						u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
            			mcSET_FIELD(u4Val, (U32) 0, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
            			ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4Val);
					}
				case 'x': case 'X':
					u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3));
					mcSET_FIELD0(u4Val, 0xf, 0x0000000f, 0);
					mcCLR_BIT(u4Val,7);
					ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4Val);

					u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4));
					mcSET_BIT(u4Val,16);
					ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u4Val);

					while(1)
					{
						u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
            			mcSET_FIELD(u4Val, (U32) 4, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
            			ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4Val);

						u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
						while(mcCHK_BIT1(u4Val, POS_TESTRPT_DM_CMP_CPT) == 0)
        				{
            				u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
            				CHIP_Delay_us(1);
						}
						u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
            			mcSET_FIELD(u4Val, (U32) 0, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
            			ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4Val);
					}
				default:
					break;
				}
			}
			#endif
			#if 0
			else if(u4Stage == DRAM_CHANGE_CLK_PI_SETTING)
			{
				switch(uc)
				{
				case '[':
					if(!IS_DRAM_CHANNELB_ACTIVE())
					{
						u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d4));
						u1ClkPIDelay = u4Val & 0x7f;
						if(u1ClkPIDelay == 0)
						{
							u1CmdPIDelay = (u4Val & 0x007f0000)>>16;
							u1CmdPIDelay = u1CmdPIDelay+1;
							mcSET_FIELD(u4Val, u1CmdPIDelay, 0x007f0000, 16);
							ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d4),u4Val) ;
						}
						else
						{
							u1ClkPIDelay = u1ClkPIDelay-1;
							mcSET_FIELD(u4Val, u1ClkPIDelay, 0x0000007f, 0);
							ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d4),u4Val) ;
						}
					}
					else
					{
						u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2d8));
						u1ClkPIDelay = u4Val & 0x7f;
						if(u1ClkPIDelay == 0)
						{ 
							u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x4d4));
							u1CmdPIDelay = (u4Val & 0x007f0000)>>16;
							u1CmdPIDelay = u1CmdPIDelay+1;
							mcSET_FIELD(u4Val, u1CmdPIDelay, 0x007f0000, 16);
							ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x4d4),u4Val) ;
						}
						else
						{
							u1ClkPIDelay = u1ClkPIDelay-1;
							mcSET_FIELD(u4Val, u1ClkPIDelay, 0x0000007f, 0);
							ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2d8),u4Val) ;
						}
					}
					break;
				case ']':
					if(!IS_DRAM_CHANNELB_ACTIVE())
					{
						u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d4));
						u1ClkPIDelay = u4Val & 0x7f;
						u1ClkPIDelay = u1ClkPIDelay+1;
						mcSET_FIELD(u4Val, u1ClkPIDelay, 0x0000007f, 0);
						ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d4),u4Val) ;
					}
					else
					{
						u4Val = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2d8));
						u1ClkPIDelay = u4Val & 0x7f;
						u1ClkPIDelay = u1ClkPIDelay+1;
						mcSET_FIELD(u4Val, u1ClkPIDelay, 0x0000007f, 0);
						ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2d8),u4Val) ;
					}
					break;
				default:
					break;
				}
			}
			#endif

			#if 1
				if(u4Stage==DRAM_CHANGE_CLK_SCRAMBLE_SETTING)
					{
						switch(uc)
							{
								case 'd':
									Printf("please press ']' to increase scramble value \npress '[' to decrase scramble value \npress 'e' to exit ");
									while (1)
										{
											while (!SerInByte(&uc1)) { }
											switch (uc1)
												{
													case '[':
														u4temp1=0;
														u4temp=ucDram_Register_Read(DRAM_BASE+DRAMC_REG_OCDK);
														u4temp1|=(u4temp>>18&0x3)|(u4temp>>22&0x3)<<2|(u4temp>>26&0x3)<<4|(u4temp>>30&0x3)<<6;
														u4temp1--;
														u4temp&=(~(0x3<<18))&(~(0x3<<22))&(~(0x3<<26))&(~(0x3<<30));
														u4temp|=(u4temp1&0x3)<<18|((u4temp1>>2)&0x3)<<22|((u4temp1>>4)&0x3)<<26|((u4temp1>>6)&0x3)<<30;
														ucDram_Register_Write(DRAM_CHB_BASE+DRAMC_REG_OCDK,u4temp);
														ucDram_Register_Write(DRAM_BASE+DRAMC_REG_OCDK,u4temp);
														Printf("current value is %d\n",u4temp1);
														break;
													case ']':
														u4temp1=0;
														u4temp=ucDram_Register_Read(DRAM_BASE+DRAMC_REG_OCDK);
														u4temp1|=(u4temp>>18&0x3)|(u4temp>>22&0x3)<<2|(u4temp>>26&0x3)<<4|(u4temp>>30&0x3)<<6;
														u4temp1++;
														u4temp&=(~(0x3<<18))&(~(0x3<<22))&(~(0x3<<26))&(~(0x3<<30));
														u4temp|=(u4temp1&0x3)<<18|((u4temp1>>2)&0x3)<<22|((u4temp1>>4)&0x3)<<26|((u4temp1>>6)&0x3)<<30;
														ucDram_Register_Write(DRAM_CHB_BASE+DRAMC_REG_OCDK,u4temp);
														ucDram_Register_Write(DRAM_BASE+DRAMC_REG_OCDK,u4temp);
														Printf("current value is %d\n",u4temp1);
														break;
													default :
														break;
												}
											if(uc1=='e')
												break;
										}
									break;
								case 's':
									//Printf("scramble value scope is 0~255 , please enter the your value\n");
									u4temp1=0;
									Printf("please enter the value:");
									while(1)
										{
											while (!SerInByte(&uc1)) { }
											if(uc1=='\n'||uc1=='\r')
												{
													Printf("\n");
													break;
												}
											u4temp1=u4temp1*10+(int)(uc1-'0');
											Printf("%c",uc1);
										}
									u4temp=ucDram_Register_Read(DRAM_BASE+DRAMC_REG_OCDK);
									u4temp&=~(0x3<<18)&~(0x3<<22)&~(0x3<<26)&~(0x3<<30);
									u4temp|=(u4temp1&0x3)<<18|((u4temp1>>2)&0x3)<<22|((u4temp1>>4)&0x3)<<26|((u4temp1>>6)&0x3)<<30;
									ucDram_Register_Write(DRAM_CHB_BASE+DRAMC_REG_OCDK,u4temp);
									ucDram_Register_Write(DRAM_BASE+DRAMC_REG_OCDK,u4temp);
									Printf("current value is %d\n\n",u4temp1);
									break;
								default :
									break;
							}
					}
			#endif
			//Printf("Press 'b' to download and Press '0' to exit:\n");
			switch(uc)
			{
			case 'B': case 'b':
				SerNormalMode();
				CHIP_DisplayString("RS232 leave transparent mode! Console stop!\n");
				while (1) { }
			case '0': // to Exit.
				break;
	
			default:
				continue;
			}
	
			if (uc == '0')
			{
				break;
			}
		}
		SerNormalMode();
		return;

}
#endif /* #if defined(CC_MTK_PRELOADER) && (defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG)) */   

UINT32 CLK_SetDdrPll(void)
{
    //UINT32 u4Clock;
	//UINT32 u4Val;

    //HAL_WRITE32(PDWNC_CLKPDN, HAL_READ32(PDWNC_CLKPDN)|PWRDWN_CLOCK_LOCK);
    CHIP_DisplayString("\n");

#ifdef DRAM_LOAD_BOARD
	DDR_InitLoadBoard();
#endif
	
    _CLKResetTCM(DRVCUST_InitGet(eDdrClock));

#if defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG)
    // User's chance to modify the default settings at TCM_DRAM_SETUP.
    CLK_QueryDramSetting((UINT8 *)"First setup dram menu:", DRAM_CHANGE_CLK_SETTING);
#endif /* #if defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG) */

    //u4Clock = TCMGET_DDR_CLK();

	DdrPhyInit();
	
    return 0;
}
#endif /* #ifdef CC_MTK_PRELOADER */

UINT32 CLK_SetDdr()
{
#ifdef CC_MTK_PRELOADER

#if defined(CC_FPGA)
    CLK_DramCSelftest();
    return CLK_SdrCalibrate();
#else
    return CLK_SetDdrPll();
#endif // #if defined(CC_FPGA)

#else
    return 0;
#endif /* #ifdef CC_MTK_PRELOADER */
}

#ifndef CC_MTK_PRELOADER
UINT32 BSP_IsSystemAcOn()
{
    //return IS_SYS_ACON();
    return 1;
}
#endif /* CC_MTK_PRELOADER */

//-----------------------------------------------------------------------------
// Below is cut from board.c
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** BSP_IsFPGA() Check if current platform if FPGA or not
 *  @reval TRUE: FPGA
 *  @reval FALSE: IC
 */
//-----------------------------------------------------------------------------
BOOL BSP_IsFPGA(void)
{
    UINT32 u4Val;

    /* If there is FPGA ID, it must be FPGA, too. */
    u4Val = BIM_READ32(REG_RO_FPGA_ID);
    if (u4Val != 0) { return 1; }

    /* otherwise, it's not FPGA. */
    return 0;
}

//-----------------------------------------------------------------------------
/** BSP_GetIcVersion() Get IC version
 *  @return The IC version, or IC_VER_FPGA if running in a FPGA platform, or
 *      IC_VER_UNKOWN for unknown IC version
 */
//-----------------------------------------------------------------------------
IC_VERSION_T BSP_GetIcVersion(void)
{
    IC_VERSION_T eVer = IC_VER_FPGA;

    if (!BSP_IsFPGA())
    {
        UINT32 u4Version;

        u4Version = CKGEN_READ32(REG_RO_SW_ID);

        return (IC_VERSION_T)u4Version;
    }

    return eVer;
}


#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** BSP_GetChBMemSize()
 *  @return the channel B dram size.
 */
//-----------------------------------------------------------------------------
UINT32 BSP_GetChBMemSize()
{
#ifndef CC_SECURE_WORLD
    return g_u4DramChBSize;
#else
    return TCMGET_CHANNELB_SIZE() * 0x100000;
#endif
}

//-----------------------------------------------------------------------------
/** BSP_GetMemSize()
 *  @return the channel A dram size.
 */
//-----------------------------------------------------------------------------
UINT32 BSP_GetMemSize()
{
#ifndef CC_SECURE_WORLD
#ifdef FAKE_MEM_SIZE
    return (g_u4DramSize > FAKE_MEM_SIZE) ? FAKE_MEM_SIZE : g_u4DramSize;
#else
    return g_u4DramSize;
#endif
#else
    return TCMGET_CHANNELA_SIZE() * 0x100000;
#endif
}

//-----------------------------------------------------------------------------
/** BSP_SetMemSize()
 *  @return the total dram size.
 */
//-----------------------------------------------------------------------------
void BSP_SetMemSize(void)
{
#ifndef CC_SECURE_WORLD
    g_u4DramSize = TCMGET_CHANNELA_SIZE() * 0x100000;
#endif
}

//-----------------------------------------------------------------------------
/** BSP_SetChBMemSize()
 *  @return the total dram size.
 */
//-----------------------------------------------------------------------------
void BSP_SetChBMemSize(void)
{
#ifndef CC_SECURE_WORLD
    g_u4DramChBSize = TCMGET_CHANNELB_SIZE() * 0x100000;
#endif
}

#endif /* !CC_MTK_LOADER */

//lint -save
//lint --e{550} Symbol not accessed
//lint --e{529} Symbol not subsequently referenced

//-----------------------------------------------------------------------------
// Below is cut from hal_926.c
//-----------------------------------------------------------------------------

#ifndef __GCC__
#pragma arm
#endif

//-----------------------------------------------------------------------------
/** HalInvalidateICache() Invalidate entire I-cache
 */
//-----------------------------------------------------------------------------
void HalInvalidateICache(void)
{
    register UINT32 r = 0;
#ifdef __GCC__
    __asm__ ("MCR     p15, 0, %0, c7, c5, 0" : : "r" (r));
#else
    __asm
    {
        MCR     p15, 0, r, c7, c5, 0
    }
#endif
}


//-----------------------------------------------------------------------------
/** HalEnableICache() Enable I-cache
 */
//-----------------------------------------------------------------------------
void HalEnableICache(void)
{
    register UINT32 r = 0;

    HalInvalidateICache();

#ifdef __GCC__
    __asm__ ("MRC     p15, 0, %0, c1, c0, 0" : "=r" (r));
    __asm__ ("ORR     %0, %1, %2" : "=r" (r) : "r" (r), "r" (ICACHE_ENABLE));
    __asm__ ("MCR     p15, 0, %0, c1, c0, 0" : : "r" (r));
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
    __asm__ ("NOP");
#else
    __asm
    {
        MRC     p15, 0, r, c1, c0, 0
        ORR     r, r, #ICACHE_ENABLE
        MCR     p15, 0, r, c1, c0, 0
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
    }
#endif
}
