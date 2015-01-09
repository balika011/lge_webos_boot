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

/*
 * uC/OS target-specific code for any target supported by the ARM Firmware Suite
 * $Revision: #1 $
 */
#if defined(CONFIG_ARCH_MT85XX)
//#if defined(CC_MT8530)
#include "drv_config.h"
#include "x_hal_ic.h"
//#endif
#include "x_bim.h"
#endif // #if defined(CONFIG_ARCH_MT85XX)

#include "mu_cdi.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "mu_sys.h"
#include "brd_cnf.h"
#include "brd_main.h"
#include "mu_hdrdf.h"

#if defined(CONFIG_ARCH_MT85XX)
#define USB_AUTOK_SLEWRATE  0
#define POWN_DOWN_VERIFY
#endif

#ifdef MUSB_BC12
extern UINT32 IS_SupportBC12(VOID);
#endif
/***************************** TYPES ******************************/
/**
 * MGC_AfsUds.
 * Board-specific UDS instance data.
 * @field pfIsr ISR
 * @field pIsrParam parameter to pass controller ISR
 * @field hTimerHdl: MTK OSAI handler. 
 * @field wIrq interrupt number
 * @field wTimerCount how many wrappers
 * @field bIndex our index into the global array
 */
typedef struct
{
    MUSB_pfUcosIsr pfIsr;
    void *pIsrParam;
    HANDLE_T hTimerHdl;    
    uint16_t wIrq;
    uint8_t bIndex;
} MGC_AfsUds;



/*************************** FORWARDS *****************************/


/**************************** GLOBALS *****************************/

/** since AFS doesn't allow for instance data on ISRs or timer callbacks */
static MGC_AfsUds _arMGC_apAfsUds[MUSB_MAX_CONTROLLERS];
#if defined(CONFIG_ARCH_MT85XX)
static uint32_t arIrqStatus[3] = {0,0,0};
#endif

//  add for debug memory allocation and free.
#ifdef MUSB_MEMDEBUG
MGC_AfsMem MGC_Mem[MGC_MEM_ALLOC_ITEM];
uint32_t MGC_Mem_Index;
uint32_t MGC_Mem_Size;
#endif

/*************************** FUNCTIONS ****************************/

#if defined(CONFIG_ARCH_MT85XX)
uint32_t MUSB_GetIrqStatus(uint16_t wIrq)
{
    uint8_t bPortNum = 0;
    switch(wIrq)
    {
        case VECTOR_USB:
            bPortNum = 0;
            break;
        case VECTOR_USB2:
            bPortNum = 1;
            break;
#if defined(UNIFIED_USB)
        case VECTOR_USB3:
            bPortNum = 2;
            break;
#endif
        default:
            Printf("USB irq number is wrong\n");
            ASSERT(0);
            break;
    }
    return arIrqStatus[bPortNum];
}

void MUSB_SetIrqStatus(uint16_t wIrq, uint32_t dwEnable)
{
    uint8_t bPortNum = 0;
    switch(wIrq)
    {
        case VECTOR_USB:
            bPortNum = 0;
            break;
        case VECTOR_USB2:
            bPortNum = 1;
            break;
#if defined(UNIFIED_USB)
        case VECTOR_USB3:
            bPortNum = 2;
            break;
#endif
        default:
            Printf("USB irq number is wrong\n");
            ASSERT(0);
            break;
    }
    arIrqStatus[bPortNum] = dwEnable;
}
#endif //#if defined(CONFIG_ARCH_MT85XX)

static void MGC_AfsUdsIsr(UINT16 wInterruptNumber)
{
    uint8_t bIndex;
    MGC_AfsUds *pUds;
    int32_t result = -1;

    for (bIndex = 0; bIndex < MGC_bSystemCount; bIndex++)
    {
        MUSB_ASSERT(bIndex < (uint8_t)MUSB_MAX_CONTROLLERS);
        if (bIndex < (uint8_t)MUSB_MAX_CONTROLLERS)
        {
            pUds = &_arMGC_apAfsUds[bIndex];
            if ((uint16_t)wInterruptNumber == pUds->wIrq)
            {
#if defined(CONFIG_ARCH_MT85XX)
                //Printf("Irq disable\n");
                MUSB_SetIrqStatus(pUds->wIrq,0);
#endif
                result = pUds->pfIsr(pUds->pIsrParam);
                UNUSED(result);
#if defined(CONFIG_ARCH_MT85XX)
                //Printf("Irq enable\n");
                MUSB_SetIrqStatus(pUds->wIrq,1);
#endif
                return;
            }
        }
    }
}


#if !defined(CONFIG_ARCH_MT85XX)

#if defined(CC_MT5890) || defined(CC_MT5882)
int MUSB_U2_SlewRate_Calibration(void * pBase)
{
	int i=0;
	int fgRet = 0;	
	uint32_t u4FmOut = 0;
	uint32_t u4Tmp = 0;
    uint32_t u4Reg = 0;
    uint8_t uPortNum = 0;

	// => RG_USB20_HSTX_SRCAL_EN = 1
	// enable HS TX SR calibration
#if defined(CC_MT5890)
    u4Reg = MGC_PHY_Read32(pBase, (uint32_t)0x14);
    u4Reg |= 0x8000;                //  RG_USB20_HSTX_SRCAL_EN  [15:15]
    MGC_PHY_Write32(pBase, (uint32_t)0x14, u4Reg);
#else
    u4Reg = MGC_PHY_Read32(pBase, (uint32_t)0x10);
    u4Reg |= 0x800000;              //  RG_USB20_HSTX_SRCAL_EN  [23:23]
    MGC_PHY_Write32(pBase, (uint32_t)0x10, u4Reg);
#endif

	// => RG_FRCK_EN = 1    
	// Enable free run clock
	u4Reg = *(volatile uint32_t*)0xf0059f10;
    u4Reg |= 0x100;            //  RG_FRCK_EN  [8:8]
    *(volatile uint32_t*)0xf0059f10 = u4Reg;

	// => RG_CYCLECNT = 0x400
	// Setting cyclecnt = 0x400
	u4Reg = *(volatile uint32_t*)0xf0059f00;
    u4Reg |= 0x400;            //  CYCLECNT  [23:0]
    *(volatile uint32_t*)0xf0059f00 = u4Reg;

    // => RG_MONCLK_SEL = port number
    // Monitor clock selection
	u4Reg = *(volatile uint32_t*)0xf0059f00;

    if(((uint32_t)pBase) == MUSB_BASE)
        {
        u4Reg |= 0x00000000;            //  RG_MONCLK_SEL  [27:26]
        uPortNum = 0;
    }
    else if(((uint32_t)pBase) == MUSB1_BASE)
        {
        u4Reg |= 0x04000000;            //  RG_MONCLK_SEL  [27:26]
        uPortNum = 1;
    }
    else if(((uint32_t)pBase) == MUSB2_BASE)
        {
        u4Reg |= 0x08000000;            //  RG_MONCLK_SEL  [27:26]
        uPortNum = 2;
    }
    else if(((uint32_t)pBase) == MUSB3_BASE)
        {
        uPortNum = 3;
#if defined(CC_MT5890)                  // Oryx/Gazelle PHY MAC base address not match.  port3 MAC 0xf0053000 PHY 0xf0059a00, so need to set RG_MONCLK_SEL 10
        u4Reg |= 0x08000000;            //  RG_MONCLK_SEL  [27:26]
#else
        u4Reg |= 0x0C000000;            //  RG_MONCLK_SEL  [27:26]
#endif
    }
    else
        {
        Printf("pBase Address Error !\n");
        return -1;
    }
    *(volatile uint32_t*)0xf0059f00 = u4Reg;

	// => RG_FREQDET_EN = 1
	// Enable frequency meter
	u4Reg = *(volatile uint32_t*)0xf0059f00;
    u4Reg |= 0x01000000;            //  RG_FREQDET_EN  [24:24]
    *(volatile uint32_t*)0xf0059f00 = u4Reg;

	// wait for FM detection done, set 10ms timeout
	for(i=0; i<10; i++){
		// => u4FmOut = USB_FM_OUT
		// read FM_OUT
		//u4FmOut = U3PhyReadReg32(((PHY_UINT32)&info->sifslv_fm_regs_e->fmmonr0));
        u4FmOut = *(volatile uint32_t*)0xf0059f0C;
        
		Printf("FM_OUT value: u4FmOut = %d(0x%08X)\n", u4FmOut, u4FmOut);
		// check if FM detection done 
		if (u4FmOut != 0)
		{
			fgRet = 0;
			Printf("FM detection done! loop = %d\n", i);
			break;
		}
		fgRet = 1;
        //mdelay(1);
        x_thread_delay(1);
	}
    
	// => RG_FREQDET_EN = 0
	// disable frequency meter
	u4Reg = *(volatile uint32_t*)0xf0059f00;
    u4Reg &= ~0x01000000;            //  RG_FREQDET_EN  [24:24]
    *(volatile uint32_t*)0xf0059f00 = u4Reg;

    // => RG_MONCLK_SEL = port number
    // Monitor clock selection
	u4Reg = *(volatile uint32_t*)0xf0059f00;
    u4Reg &= ~0x0C000000;
    *(volatile uint32_t*)0xf0059f00 = u4Reg;

	// => RG_FRCK_EN = 0
	// disable free run clock
	u4Reg = *(volatile uint32_t*)0xf0059f10;
    u4Reg &= ~0x100;            //  RG_FRCK_EN  [8:8]
    *(volatile uint32_t*)0xf0059f10 = u4Reg;


	// => RG_USB20_HSTX_SRCAL_EN = 0
	// disable HS TX SR calibration
    u4Reg = MGC_PHY_Read32(pBase, (uint32_t)0x10);
    u4Reg &= ~0x800000;            //  RG_USB20_HSTX_SRCAL_EN  [23:23]
    MGC_PHY_Write32(pBase, (uint32_t)0x10, u4Reg);

    //mdelay(1);
    x_thread_delay(1);

	if(u4FmOut == 0){
		fgRet = 1;
	}
	else{
		// set reg = (1024/FM_OUT) * REF_CK * U2_SR_COEF_E60802 / 1000 (round to the nearest digits)
		u4Tmp = (((1024 * 24 * 22) / u4FmOut) + 500) / 1000;
		Printf("U2 Port%d SR auto calibration value = 0x%x\n", uPortNum,u4Tmp);

#if defined(CC_MT5890)
        u4Reg = MGC_PHY_Read32(pBase, 0x14);
        u4Reg &= (uint32_t)(~0x00007000);
        u4Reg |= (uint32_t)u4Tmp<<12;
        MGC_PHY_Write32(pBase, 0x14, u4Reg);
#else
        u4Reg = MGC_PHY_Read32(pBase, 0x10);
        u4Reg &= (uint32_t)(~0x00070000);
        u4Reg |= (uint32_t)u4Tmp<<16;
        MGC_PHY_Write32(pBase, 0x10, u4Reg);
#endif
        
    }

	return fgRet;
}
#endif

static uint8_t MUSB_BoardSoftReset(void* pBase)
{
    uint32_t u4Reg = 0;
    uint8_t nDevCtl = 0;
    MUSB_ASSERT(pBase);
    
    //USB DMA enable
    #if defined (CC_MT5880)
    u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0061300, 0, u4Reg);
    #elif defined (CC_MT5881)
    u4Reg= IO_READ32(0xf0061200, 0);
    u4Reg |= 0x00cfff00;
    IO_WRITE32(0xf0061200, 0, u4Reg);
    #elif defined(CC_MT5398)
    u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0060200, 0, u4Reg);    
	#elif defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5861)
    u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0061a00, 0, u4Reg);
	#elif defined(CC_MT5882)
	u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0061b00, 0, u4Reg);
    #endif

	#if !defined(CC_MT5890) ////5890 no need set to 1 
    // VRT insert R enable 
    u4Reg = MGC_PHY_Read32(pBase,M_REG_PHYC0);
    u4Reg |= 0x4000; //RG_USB20_INTR_EN, 5890 is 0x00[5], others is 0x00[14]
    MGC_PHY_Write32(pBase, M_REG_PHYC0, u4Reg);
	#endif

    //TX_Current_EN setting 01 , 25:24 = 01 //enable TX current when
    //EN_HS_TX_I=1 or EN_HS_TERM=1
    u4Reg = MGC_PHY_Read32(pBase, 0x1C);	
    u4Reg |= (uint32_t)0x01000000;	  
    MGC_PHY_Write32(pBase, 0x1C, u4Reg);

    //Soft Reset, RG_RESET for Soft RESET, reset MAC
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
    u4Reg |=   0x00004000; 
    MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);
    u4Reg &=  ~0x00004000; 
    MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

	#if defined(CC_MT5890)
	//add for Oryx USB20 PLL fail
	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x0); //RG_USB20_USBPLL_FORCE_ON=1b
	u4Reg |=   0x00008000; 
	//u4Reg =   0x0000940a; //for register overwrite by others issue
	MGC_PHY_Write32(pBase, (uint32_t)0x0, u4Reg);

	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x14); //RG_USB20_HS_100U_U3_EN=1b
	u4Reg |=   0x00000800; 
	MGC_PHY_Write32(pBase, (uint32_t)0x14, u4Reg);

    if(IS_IC_5890_ES1())
	{
		//driving adjust
		u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x04);
		u4Reg &=  ~0x00007700;
		MGC_PHY_Write32(pBase, (uint32_t)0x04, u4Reg);
	}
	#endif

    //otg bit setting
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x6C);
    u4Reg &= ~0x3f3f;
    u4Reg |=  0x3f2c;
    MGC_PHY_Write32(pBase, (uint32_t)0x6C, u4Reg);
 		
    //suspendom control
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
    u4Reg &=  ~0x00040000; 
    MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

    //disconnect threshold,set 620mV
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x18); //RG_USB20_DISCTH
	#if defined(CC_MT5890)
    if(IS_IC_5890_ES1())
	{
	    u4Reg &= ~0x0000000f; //0x18[3:0]
	    u4Reg |=  0x0000000b;
	}
	else
	{
	    u4Reg &= ~0x000000f0; //0x18[7:4]
	    u4Reg |=  0x000000b0;
	}
	#else
    u4Reg &= ~0x000f0000; //others IC is 0x18[19:16]
    u4Reg |=  0x000b0000;
	#endif
    MGC_PHY_Write32(pBase, (uint32_t)0x18, u4Reg);

    u4Reg = MGC_Read8(pBase,M_REG_PERFORMANCE3);
    u4Reg |=  0x80;
    u4Reg &= ~0x40;
    MGC_Write8(pBase, M_REG_PERFORMANCE3, (uint8_t)u4Reg);

    #if defined(CC_MT5890)
    u4Reg = MGC_Read8(pBase, (uint32_t)0x7C);
    u4Reg |= 0x04;
    MGC_Write8(pBase, 0x7C, (uint8_t)u4Reg);
    #else
    // MT5368/MT5396/MT5389 
    //0xf0050070[10]= 1 for Fs Hub + Ls Device 
    u4Reg = MGC_Read8(pBase, (uint32_t)0x71);
    u4Reg |= 0x04; //RGO_USB20_PRBS7_LOCK
    MGC_Write8(pBase, 0x71, (uint8_t)u4Reg);
	#endif

    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase,0x604);
    u4Reg |= 0x01;
    MGC_Write32(pBase, 0x604, u4Reg);
	
    // open session.
    nDevCtl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);   
    nDevCtl |= MGC_M_DEVCTL_SESSION;        
    MGC_Write8(pBase, MGC_O_HDRC_DEVCTL, nDevCtl);

    // FS/LS Slew Rate change
    u4Reg = MGC_PHY_Read32(pBase, 0x10);
    u4Reg &= (uint32_t)(~0x000000ff);
    u4Reg |= (uint32_t)0x00000011;
    MGC_PHY_Write32(pBase, 0x10, u4Reg);

    // HS Slew Rate, RG_USB20_HSTX_SRCTRL
	#if defined(CC_MT5890)
    u4Reg = MGC_PHY_Read32(pBase, 0x14); //bit[14:12]
    if(IS_IC_5890_ES1())
    {
	    u4Reg &= (uint32_t)(~0x00007000);
	    u4Reg |= (uint32_t)0x00001000;
	}
	else
	{
		u4Reg &= (uint32_t)(~0x00007000);
		u4Reg |= (uint32_t)0x00004000;
	}
    MGC_PHY_Write32(pBase, 0x14, u4Reg);
	#else
    u4Reg = MGC_PHY_Read32(pBase, 0x10);
    u4Reg &= (uint32_t)(~0x00070000);
    u4Reg |= (uint32_t)0x00040000;
    MGC_PHY_Write32(pBase, 0x10, u4Reg);
	#endif
	#if defined(CC_MT5890) //usb2.0 port3 driving adjust
    u4Reg = MGC_PHY_Read32(pBase, 0x14);
    u4Reg &= ~(uint32_t)0x300000;
    u4Reg |= (uint32_t)0x10200000; //bit[21:20]: RG_USB20_DISCD[1:0]=2'b10. bit[28]: RG_USB20_DISC_FIT_EN=1'b1
    MGC_PHY_Write32(pBase, 0x14, u4Reg);
	#endif

#if defined(CC_MT5890) || defined(CC_MT5882)
    // USB2.0 slewrate auto calibration
    MUSB_U2_SlewRate_Calibration(pBase);
#endif

    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase, 0x604);
    u4Reg &= ~0x01;
    MGC_Write32(pBase, 0x604, u4Reg);

    LOG(5, "[USB]Nuclues pBase = 0x%08X init ok.\n", (uint32_t)pBase);
	
#ifdef MUSB_BC12
#if defined(CC_MT5881) || defined(CC_MT5890) || defined(CC_MT5882)
if(IS_SupportBC12())
{
	#if defined(CC_MT5881)
	if(MUSB1_BASE == pBase)
	#elif defined(CC_MT5890) || defined(CC_MT5882)
	if(MUSB_BASE == pBase)
	#endif
	{
		uint8_t power = MGC_Read8(pBase,MGC_O_HDRC_POWER);
		uint8_t nPower= power ;
		
		LOG(0, "[USB]Pull up RG_USB20_BGR_EN RG_USB20_CHP_EN on port1.\n");
		u4Reg = MGC_PHY_Read32(pBase, M_REG_PHYC0);
		u4Reg |= 0x3;
		MGC_PHY_Write32(pBase, M_REG_PHYC0, u4Reg);
		LOG(0, "[USB]Enable BC12 function on port1 when init.\n");
		LOG(0, "[USB]Reset port\n");

		nPower |= MGC_POWER_RESET ;
		if(nPower != power)
		{
			MGC_Write8(pBase,MGC_O_HDRC_POWER,nPower);
		}

		//pull up RG_CHGDT_EN
		#if !defined(CC_MT5890)
		u4Reg = MGC_PHY_Read32(pBase,M_REG_U2PHYACR5);
		#else
		u4Reg = MGC_PHY_Read32(pBase,0x40); //5890 IC issue, read 0x40 instead of M_REG_U2PHYACR5(0x24)
		#endif
		u4Reg |= 0x1;
		MGC_PHY_Write32(pBase,M_REG_U2PHYACR5,u4Reg);
	}
}
#endif
#endif
    return TRUE;
}
#else //#if !defined(CONFIG_ARCH_MT85XX)
#if defined(CC_MT8580)
//may be need modify fm r/w api
uint32_t vMeasure_CLK(void* pBase)
{
  uint8_t bPort = 0;
  uint32_t u4Reg,u4Fx;

  uint16_t u2Count = 0x400;
  u4Fx = 0;

  switch((uint32_t)pBase)
  {
		  case MUSB_BASE:
		   bPort = 0;
		   break;
		  case MUSB1_BASE:
		   bPort = 1;
		   break;  
		  case MUSB2_BASE:
		   bPort = 0;
		   break;  
		  default:
		   LOG(0, "USB invalid pBase: 0x%x\n", (uint32_t)pBase);
		   break;  
  } 

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x10);	
   u4Reg |= 0x100;
   MGC_PHY_FM_Write32(pBase,0x10,u4Reg);

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x00);	
   u4Reg &= ~0xffff;
   u4Reg |= u2Count;
   MGC_PHY_FM_Write32(pBase,0x00,u4Reg);   

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x00);	
   u4Reg &= ~0xc000000;
   u4Reg |= (bPort<<26);
   MGC_PHY_FM_Write32(pBase,0x00,u4Reg);     

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x00);	
   u4Reg |= 0x1000000;
   MGC_PHY_FM_Write32(pBase,0x00,u4Reg); 

   Printf("[usb]testing freq ....\n");

   while(!(MGC_PHY_FM_Read32(pBase,0x10)&0x1))

   //MUSB_Sleep(10000);
   x_thread_delay(1);

   u4Fx = MGC_PHY_FM_Read32(pBase,0x0C);	

   Printf("[usb]Fx=%d\n",u4Fx);

   u4Fx = (12 * u2Count)/u4Fx;

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x00);	
   u4Reg &= ~0x1000000;
   MGC_PHY_FM_Write32(pBase,0x00,u4Reg);   

   x_thread_delay(1);

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x10);	
   u4Reg &= ~0x100;
   MGC_PHY_FM_Write32(pBase,0x10,u4Reg);  
   

   return u4Fx;
  
}

static uint8_t MUSB_BoardSoftReset(void* pBase)
{
	uint32_t u4Reg = 0;
  #if USB_AUTOK_SLEWRATE
    uint32_t u4Fx = 0;
  #endif
	MUSB_ASSERT(pBase);

#ifdef POWN_DOWN_VERIFY
	u4Reg = *(volatile uint32_t*)0xfd024188;
    if(u4Reg & 0x2000000)
    {
      Printf("Risc control Register 0x%x\n",u4Reg);
      u4Reg &= ~0x2000000;
	  *(volatile uint32_t*)0xfd024188 = u4Reg;
    }
#endif

    MU_MB();

    //2010.11.13, for MT8560, set USB module clk.
    u4Reg = MGC_VIRT_Read32(0x7C);
	u4Reg |= 0x00010000; 
    MGC_VIRT_Write32(0x7C, u4Reg);
    //~2010.11.13

    MU_MB();

    //Soft Reset, RG_RESET for Soft RESET
	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
	u4Reg |=   0x00004000; 
	MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

	MU_MB();

	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
	u4Reg &=  ~0x00004000; 
	MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

	MU_MB();	

    //otg bit setting
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x6C);
	u4Reg &= ~0x3f3f;
    u4Reg |=  0x3e2c;
    MGC_PHY_Write32(pBase, (uint32_t)0x6C, u4Reg);
 		
    //suspendom control
	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
	u4Reg &=  ~0x00040000; 
	MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);
	//mt8580 fine tune maybe different
	//update it after IC comeback
#if 1
    //hs eye finetune
	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x10);
	u4Reg &=  ~0x00070000;
	u4Reg |= 0x00050000;
	MGC_PHY_Write32(pBase, (uint32_t)0x10, u4Reg);

    //PLL setting to reduce clock jitter
    u4Reg = MGC_PHY_Read32(pBase, 0x0);
    u4Reg &= ~0x70000000;
    u4Reg |= 0x20000000;
    MGC_PHY_Write32(pBase, 0x0, u4Reg);

    u4Reg = MGC_PHY_Read32(pBase, 0x4);
    u4Reg |= 0x3;
    MGC_PHY_Write32(pBase, 0x4, u4Reg);
    //End of PLL setting

	//For FS/LS eye pattern fine-tune
    u4Reg = MGC_PHY_Read32(pBase, 0x10);
    u4Reg &= ~0x00007707;
    u4Reg |=  0x00005503;
    MGC_PHY_Write32(pBase, 0x10, u4Reg);
	//End of FS/LS eye pattern fine-tune
#endif
#if USB_AUTOK_SLEWRATE
	if(MUSB_BASE == (uint32_t)pBase
	   || MUSB1_BASE == (uint32_t)pBase
	   || MUSB2_BASE == (uint32_t)pBase)
	{ 
		u4Reg = MGC_PHY_Read32(pBase, 0x10);
		u4Reg |=  0x00800000L; 
		MGC_PHY_Write32(pBase, 0x10, u4Reg);

		x_thread_delay(1);
	
	    u4Fx = vMeasure_CLK(pBase);
		
		LOG(0, "Frx: %d for pBase 0x%x\n", (uint32_t)u4Fx,(uint32_t)pBase);
		
		if(MUSB_BASE == (uint32_t)pBase || MUSB1_BASE == (uint32_t)pBase)
		u4Fx = (5*u4Fx+78)/156;
		else if(MUSB2_BASE == (uint32_t)pBase)
		u4Fx = (5*u4Fx+57)/115;
	
		u4Fx &= 7;
		u4Fx <<= 16;
	
		//Set Slew rate (RG_HSTX_SRCTRL)
		u4Reg = MGC_PHY_Read32(pBase, 0x10);
		u4Reg &=  ~0x00070000;
		u4Reg |=  u4Fx; 
		MGC_PHY_Write32(pBase, 0x10, u4Reg);
	
    	u4Reg = MGC_PHY_Read32(pBase, 0x10);
    	u4Reg &=  ~0x00800000L; 
    	MGC_PHY_Write32(pBase, 0x10, u4Reg);

		LOG(0, "Set SR=0x%08X for pBase 0x%x\n", (uint32_t)u4Reg,(uint32_t)pBase);
	}
#endif	
    u4Reg = MGC_Read8(pBase,M_REG_PERFORMANCE3);
    u4Reg |=  0x80;
    u4Reg &= ~0x40;
    MGC_Write8(pBase, M_REG_PERFORMANCE3, (uint8_t)u4Reg);	
	
	(void)MUSB_Sleep(10);

	LOG(0,"[MUSB]init complete. value 0x%x at addr 0x%x\n",
			MGC_Read8(pBase, MGC_O_HDRC_DEVCTL),MGC_O_HDRC_DEVCTL);

	return TRUE;
}

#elif defined(CC_MT8563)
//may be need modify fm r/w api
uint32_t vMeasure_CLK(void* pBase)
{
  uint8_t bPort = 0;
  uint32_t u4Reg,u4Fx;

  uint16_t u2Count = 0x400;
  u4Fx = 0;

  switch((uint32_t)pBase)
  {
		  case MUSB_BASE:
		   bPort = 0;
		   break;
		  case MUSB_BASE2:
		   bPort = 1;
		   break;
		  case MUSB_BASE3:
		   bPort = 2;
		   break;
		  default:
		   LOG(0, "USB invalid pBase: 0x%x\n", (uint32_t)pBase);
		   break;  
  } 

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x10);	
   u4Reg |= 0x100;
   MGC_PHY_FM_Write32(pBase,0x10,u4Reg);

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x00);	
   u4Reg &= ~0xffff;
   u4Reg |= u2Count;
   MGC_PHY_FM_Write32(pBase,0x00,u4Reg);   

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x00);	
   u4Reg &= ~0xc000000;
   u4Reg |= (bPort<<26);
   MGC_PHY_FM_Write32(pBase,0x00,u4Reg);     

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x00);	
   u4Reg |= 0x1000000;
   MGC_PHY_FM_Write32(pBase,0x00,u4Reg); 

   Printf("[usb]testing freq ....\n");

   while(!(MGC_PHY_FM_Read32(pBase,0x10)&0x1))

   //MUSB_Sleep(10000);
   x_thread_delay(1);

   u4Fx = MGC_PHY_FM_Read32(pBase,0x0C);	

   Printf("[usb]Fx=%d\n",u4Fx);

   u4Fx = (12 * u2Count)/u4Fx;

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x00);	
   u4Reg &= ~0x1000000;
   MGC_PHY_FM_Write32(pBase,0x00,u4Reg);   

   x_thread_delay(1);

   u4Reg = 	MGC_PHY_FM_Read32(pBase,0x10);	
   u4Reg &= ~0x100;
   MGC_PHY_FM_Write32(pBase,0x10,u4Reg);  
   

   return u4Fx;
  
}

static uint8_t MUSB_BoardSoftReset(void* pBase)
{
	uint32_t u4Reg = 0;
  #if USB_AUTOK_SLEWRATE
    uint32_t u4Fx = 0;
  #endif
	MUSB_ASSERT(pBase);

#ifdef POWN_DOWN_VERIFY
	u4Reg = *(volatile uint32_t*)0xfd024188;
    if(u4Reg & 0x2000000)
    {
      Printf("Risc control Register 0x%x\n",u4Reg);
      u4Reg &= ~0x2000000;
	  *(volatile uint32_t*)0xfd024188 = u4Reg;
    }
#endif

    //2010.11.13, for MT8560, set USB module clk.
    u4Reg = MGC_VIRT_Read32(0x7C);
	u4Reg |= 0x00010000; 
    MGC_VIRT_Write32(0x7C, u4Reg);
    //~2010.11.13


    //Soft Reset, RG_RESET for Soft RESET
	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
	u4Reg |=   0x00004000; 
	MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
	u4Reg &=  ~0x00004000; 
	MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);
	
    //otg bit setting
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x6C);
	u4Reg &= ~0x3f3f;
    u4Reg |=  0x403e2c;
    MGC_PHY_Write32(pBase, (uint32_t)0x6C, u4Reg);
 		
    //suspendom control
	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
	u4Reg &=  ~0x00040000; 
	MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);
	//mt8580 fine tune maybe different
	//update it after IC comeback
#if 1
    //hs eye finetune
	u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x10);
	u4Reg &=  ~0x00070000;
	u4Reg |= 0x00050000;
	MGC_PHY_Write32(pBase, (uint32_t)0x10, u4Reg);

    //PLL setting to reduce clock jitter
    u4Reg = MGC_PHY_Read32(pBase, 0x0);
    u4Reg &= ~0x70000000;
    u4Reg |= 0x20000000;
    MGC_PHY_Write32(pBase, 0x0, u4Reg);

    u4Reg = MGC_PHY_Read32(pBase, 0x4);
    u4Reg |= 0x3;
    MGC_PHY_Write32(pBase, 0x4, u4Reg);
    //End of PLL setting

	//For FS/LS eye pattern fine-tune
    u4Reg = MGC_PHY_Read32(pBase, 0x10);
    u4Reg &= ~0x00007707;
    u4Reg |=  0x00005503;
    MGC_PHY_Write32(pBase, 0x10, u4Reg);
	//End of FS/LS eye pattern fine-tune

    //For MT8561/MT8552 need enable TX current When HS to reduce clock jitter
    u4Reg = MGC_PHY_Read32(pBase, 0x1c);
    u4Reg &= ~0x03000000;
    u4Reg |=  0x01000000;
    MGC_PHY_Write32(pBase, 0x1c, u4Reg);
	//End of enable TX current
	
#endif
#if USB_AUTOK_SLEWRATE
	if(MUSB_BASE == (uint32_t)pBase)
	{ 
		u4Reg = MGC_PHY_Read32(pBase, 0x10);
		u4Reg |=  0x00800000L; 
		MGC_PHY_Write32(pBase, 0x10, u4Reg);

		x_thread_delay(1);
	
	    u4Fx = vMeasure_CLK(pBase);
		
		LOG(0, "Frx: %d for pBase 0x%x\n", (uint32_t)u4Fx,(uint32_t)pBase);
		
		if(MUSB_BASE == (uint32_t)pBase)
		u4Fx = (5*u4Fx+78)/156;
		else if(MUSB1_BASE == (uint32_t)pBase)
		u4Fx = (5*u4Fx+57)/115;
	
		u4Fx &= 7;
		u4Fx <<= 16;
	
		//Set Slew rate (RG_HSTX_SRCTRL)
		u4Reg = MGC_PHY_Read32(pBase, 0x10);
		u4Reg &=  ~0x00070000;
		u4Reg |=  u4Fx; 
		MGC_PHY_Write32(pBase, 0x10, u4Reg);
	
    	u4Reg = MGC_PHY_Read32(pBase, 0x10);
    	u4Reg &=  ~0x00800000L; 
    	MGC_PHY_Write32(pBase, 0x10, u4Reg);

		LOG(0, "Set SR=0x%08X for pBase 0x%x\n", (uint32_t)u4Reg,(uint32_t)pBase);
	}
#endif	

    u4Reg = MGC_Read8(pBase,M_REG_PERFORMANCE3);
    u4Reg |=  0x80;
    u4Reg &= ~0x40;
    MGC_Write8(pBase, M_REG_PERFORMANCE3, (uint8_t)u4Reg);	
	
	(void)MUSB_Sleep(10);

	LOG(0,"[MUSB]init complete. value 0x%x at addr 0x%x\n",
			MGC_Read8(pBase, MGC_O_HDRC_DEVCTL),MGC_O_HDRC_DEVCTL);

	return TRUE;
}

#endif
#endif //#if !defined(CONFIG_ARCH_MT85XX)

void *MUSB_BoardInitController(void *pPrivateData, MUSB_pfUcosIsr pfIsr,
                               const MUSB_UcosController *pControllerInfo, uint8_t **ppBaseIsr,
                               uint8_t **ppBaseBsr)
{
    MGC_AfsUds *pUds;
    VOID (*pfnOldIsr)(UINT16);

    MUSB_ASSERT(pControllerInfo);
    UNUSED(ppBaseIsr);       
    MUSB_ASSERT(ppBaseBsr);
    
    if (!MUSB_BoardSoftReset(*ppBaseBsr))
    {
        return NULL;
    }
    //MUSB_Int_Disable(*ppBaseBsr);
    
//  change to static allocation.
    MUSB_ASSERT(MGC_bSystemCount < (uint8_t)MUSB_MAX_CONTROLLERS);
    if (MGC_bSystemCount >= (uint8_t)MUSB_MAX_CONTROLLERS)
    {
        return NULL;
    }
    pUds = &_arMGC_apAfsUds[MGC_bSystemCount];

    MUSB_MemSet(pUds, 0, sizeof(MGC_AfsUds));

    pUds->wIrq = pControllerInfo->dwInfo;
    pUds->pfIsr = pfIsr;
    pUds->pIsrParam = pPrivateData;
   
    /* assign the interrupt */
    LOG(3, "REG ISR: 0x%04X\n", (UINT16)pUds->wIrq);
    VERIFY(OSR_OK == x_reg_isr((UINT16)pUds->wIrq, MGC_AfsUdsIsr, &pfnOldIsr));
    UNUSED(pfnOldIsr);

    VERIFY(BIM_DisableIrq(pUds->wIrq));
#if defined(CONFIG_ARCH_MT85XX)
    //Printf("Irq disable\n");
    MUSB_SetIrqStatus(pUds->wIrq,0);
#endif
    pUds->bIndex = MGC_bSystemCount;
    return pUds;
}

uint8_t MUSB_BoardInitTimers(void *pPrivateData)
{
    MGC_AfsUds *pUds;
    
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;

    VERIFY(OSR_OK == x_timer_create(&pUds->hTimerHdl));
   
    return TRUE;
}

#ifndef MUSB_CODE_SHRINK
void MUSB_BoardDestroyController(void *pPrivateData)
{
    UNUSED(pPrivateData);
}
#endif
#if defined(CONFIG_ARCH_MT85XX)
void MUSB_SlewRateCalibration(void)
{
    uint32_t u4Reg = 0;
    uint32_t u4Fx = 0;
    uint32_t pBase = MUSB_BASE; //0x7000E000;  
	uint8_t bPort = 0;

    for ( bPort = 0;bPort < MUSB_MAX_CONTROLLERS;bPort++ )
    { 
       switch(bPort)
       {
           case 0 : 
		   	   pBase = MUSB_BASE; 
			   break;
           case 1 : 
		   	   pBase = MUSB1_BASE; 
			   break;
           case 2 : 
		   	   pBase = MUSB2_BASE;
			   break;
		   default :
		   	   Printf("port number is wrong!");
			   break;
	    }
		
#if defined(CC_MT8580)

		u4Reg = MGC_PHY_Read32(pBase, 0x10);
		u4Reg |=  0x00800000L; 
		MGC_PHY_Write32(pBase, 0x10, u4Reg);

		x_thread_delay(1);
	
	    u4Fx = vMeasure_CLK((void*)pBase);
		
		LOG(0, "Frx: %d for pBase 0x%x\n", (uint32_t)u4Fx,(uint32_t)pBase);
		
		if(MUSB_BASE == (uint32_t)pBase || MUSB1_BASE == (uint32_t)pBase)
		u4Fx = (5*u4Fx+78)/156;
		else if(MUSB2_BASE == (uint32_t)pBase)
		u4Fx = (5*u4Fx+57)/115;
	
		u4Fx &= 7;
		u4Fx <<= 16;
	
		//Set Slew rate (RG_HSTX_SRCTRL)
		u4Reg = MGC_PHY_Read32(pBase, 0x10);
		u4Reg &=  ~0x00070000;
		u4Reg |=  u4Fx; 
		MGC_PHY_Write32(pBase, 0x10, u4Reg);
	
    	u4Reg = MGC_PHY_Read32(pBase, 0x10);
    	u4Reg &=  ~0x00800000L; 
    	MGC_PHY_Write32(pBase, 0x10, u4Reg);

		LOG(0, "Set SR=0x%08X for pBase 0x%x\n", (uint32_t)u4Reg,(uint32_t)pBase);
		
#elif defined(CC_MT8563)

	u4Reg = MGC_PHY_Read32(pBase, 0x10);
	u4Reg |=  0x00800000L; 
	MGC_PHY_Write32(pBase, 0x10, u4Reg);

	x_thread_delay(1);

	u4Fx = vMeasure_CLK((void*)pBase);

	LOG(0, "Frx: %d for pBase 0x%x\n", (uint32_t)u4Fx,(uint32_t)pBase);

	if(MUSB_BASE == (uint32_t)pBase || MUSB1_BASE == (uint32_t)pBase)
	u4Fx = (5*u4Fx+78)/156;
	else if(MUSB2_BASE == (uint32_t)pBase)
	u4Fx = (5*u4Fx+57)/115;

	u4Fx &= 7;
	u4Fx <<= 16;

	//Set Slew rate (RG_HSTX_SRCTRL)
	u4Reg = MGC_PHY_Read32(pBase, 0x10);
	u4Reg &=  ~0x00070000;
	u4Reg |=  u4Fx; 
	MGC_PHY_Write32(pBase, 0x10, u4Reg);

	u4Reg = MGC_PHY_Read32(pBase, 0x10);
	u4Reg &=  ~0x00800000L; 
	MGC_PHY_Write32(pBase, 0x10, u4Reg);

	LOG(0, "Set SR=0x%08X for pBase 0x%x\n", (uint32_t)u4Reg,(uint32_t)pBase);
#endif

    }
    return;
}
#endif //#if defined(CONFIG_ARCH_MT85XX)

uint8_t MUSB_BoardArmTimer(void *pPrivateData, uint16_t wIndex, uint32_t dwTime, uint8_t bPeriodic,
                           x_os_timer_cb_fct pfExpireCallback, void *pParam)
{
    MGC_AfsUds *pUds;
    TIMER_FLAG_T e_flags;

    UNUSED(wIndex);
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;

    LOG(9, "Timer:%d Start delay = %d ms.\n", wIndex, dwTime);
    e_flags = (bPeriodic) ? X_TIMER_FLAG_REPEAT : X_TIMER_FLAG_ONCE;
    VERIFY(OSR_OK == x_timer_start(pUds->hTimerHdl, dwTime, e_flags, pfExpireCallback, pParam));
    return TRUE;
}

void MUSB_BoardCancelTimer(void *pPrivateData, uint16_t wIndex)
{
    MGC_AfsUds *pUds;

    UNUSED(wIndex);
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;

    LOG(9, "Timer:%d Stop.\n", wIndex);
    
    VERIFY(OSR_OK == x_timer_stop(pUds->hTimerHdl));
}

/*
* Controller calls this to get a bus address (for DMA) from a system address
*/
void *MUSB_BoardSystemToBusAddress(const void *pSysAddr)
{
#if !defined(CONFIG_ARCH_MT85XX)
    return (void *)PHYSICAL((UINT32) pSysAddr);
#else
    if((PAGE_OFFSET<=(unsigned int)pSysAddr) && 
        ((unsigned int)pSysAddr<(PAGE_OFFSET + g_u4Ch1DramSize + g_u4Ch2DramSize)))
    {
        LOG(9, "System address@[%08X]\n", (int)pSysAddr);   
    }
    else
    {
        LOG(1, "System address@[%08X] out of range\n", (int)pSysAddr);
    }

    return (void *)0;//PHYSICAL((uint32_t)pSysAddr);
#endif
}

/*
* Controller calls this to enter a lock irq
*/
uint32_t MUSB_BoardIrqLock(void *pPrivateData)
{
    uint32_t u4Ret;
    MGC_AfsUds* pUds;
    
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;

    // disable USB interrupt.
#if defined(CONFIG_ARCH_MT85XX)
    u4Ret = MUSB_GetIrqStatus(pUds->wIrq);
#else
    u4Ret = (UINT32)BIM_IsIrqEnabled(pUds->wIrq);
#endif
    if (u4Ret)
    {
        VERIFY(BIM_DisableIrq(pUds->wIrq));
#if defined(CONFIG_ARCH_MT85XX)
        MUSB_SetIrqStatus(pUds->wIrq,0);
#endif
    }

    return u4Ret;
}

/*
* Controller calls this to enter a unlock irq
*/
void MUSB_BoardIrqUnlock(void *pPrivateData, uint32_t dwPrevIrq)
{
    MGC_AfsUds* pUds;
    
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;
   
    // interrupt is previous enable.
    if (dwPrevIrq)
    {
        // enable USB interrupt.
        VERIFY(BIM_EnableIrq(pUds->wIrq));
#if defined(CONFIG_ARCH_MT85XX)
        MUSB_SetIrqStatus(pUds->wIrq,1);
#endif
    }
}

/*
* Controller calls this to get irq status.
*/
uint32_t MUSB_BoardIsIrqPending(void *pPrivateData)
{
    uint32_t u4Ret;
    MGC_AfsUds* pUds;
    
    MUSB_ASSERT(pPrivateData);
    pUds = (MGC_AfsUds *)pPrivateData;

    // disable USB interrupt.
    u4Ret = (UINT32)BIM_IsIrqPending(pUds->wIrq);

    return u4Ret;
}

#if defined(CONFIG_ARCH_MT85XX)
void MUSB_BoardResetPhy(void* pBase)
{
	MUSB_BoardSoftReset(pBase);
}
#endif

