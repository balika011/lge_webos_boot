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
/******************************************************************************
* [File]			msdc_if.c
* [Version]			v1.0
* [Revision Date]	2010-08-18
* [Author]			Jethro Chang, jethro.chang@mediatek.inc, 82896, 2010-08-18
* [Description]
*	BootROM C code
* [Copyright]
*	Copyright (C) 2007 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/

#include "x_typedef.h"
#include "x_ldr_env.h"

#include "msdc_debug.h"
#include "msdc_host_hw.h"
#include "msdc_slave_hw.h"
#include "msdc_drv.h"
#include "msdc_if.h"
#include "drvcust_if.h"

#if defined(CC_MTD_ENCRYPT_SUPPORT)
#include "msdc_ext.h"
#endif

// For SD gpio detection pin
#include "x_gpio.h"

#if defined(CC_MSDC_ENABLE) || defined(CC_SDMMC_SUPPORT)

extern sdcard_t sdCard[];
extern sdhost_t sdHost[];
extern msdc_env_t msdcenv;

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Definition
//---------------------------------------------------------------------------

extern UINT32 msdcClk[][MSDC_CLK_IDX_MAX];
extern UINT32 ch;
BOOL _fgMsdcIdentyCardOK = FALSE;
UINT32 EmmcChannel=0;
UINT32 _pu4BlkBuf[SDHC_BLK_SIZE>>2];

VOID MSDC_EmmcHostSet(UINT32 u4Ch)
{
    EmmcChannel = u4Ch;	
}

VOID MSDC_HostSet(UINT32 u4Ch)
{
    ch = u4Ch;	
}

UINT32 MSDC_HostGet(VOID)
{
    return ch;	
}

#if !defined(CC_MTK_LOADER)
//-----------------------------------------------------------------------------
/** _MSDC_LockInit()
 *  Lock sempahore for thread safe
 */
//-----------------------------------------------------------------------------
static VOID _MSDC_LockInit(HANDLE_T *hhSema)
{
    if (*hhSema == NULL_HANDLE)
    {
        VERIFY(OSR_OK == x_sema_create(hhSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK));
    }
}
#endif

//-----------------------------------------------------------------------------
/** MSDC_ApiLock()
 *  Lock sempahore for thread safe
 */
//-----------------------------------------------------------------------------
VOID MSDC_ApiLock(HANDLE_T *hhSema)
{
#if !defined(CC_MTK_LOADER)
    _MSDC_LockInit(hhSema);
    VERIFY(OSR_OK == x_sema_lock(*hhSema, X_SEMA_OPTION_WAIT));
#endif
}

//-------------------------------------------------------------------------
/** MSDC_ApiUnLock
 *  Unlock semaphore for thread safe.
 */
//-------------------------------------------------------------------------
VOID MSDC_ApiUnLock(HANDLE_T *hhSema)
{
#if !defined(CC_MTK_LOADER)
    VERIFY(OSR_OK == x_sema_unlock(*hhSema));
#endif
}

//---------------------------------------------------------------------------
// MSDC PinMux Switch  function
//---------------------------------------------------------------------------
VOID MSDC_PinMux(UINT32 u4Ch)
{
    UINT32 u4Value;
    
    UNUSED(u4Value);
    ch = u4Ch;

//MSDC Controller 0, Basic Address: 0xF0012000
//MSDC Controller 1, Basic Address: 0xF006D000 
    
#if defined(CC_MT5396)      
    if (ch == 0)  
    { 
        //pinmux register d408[1:0], function 1 	    
        MSDC_CLRBIT(0xF000D408, 0x03<<0);
        MSDC_SETBIT(0xF000D408, 0x01<<0);

        //Local Arbitor open
        MSDC_CLRBIT(0xF0012200, (0x03<<16) | (0x1F<<0));	 
        MSDC_SETBIT(0xF0012200, (0x01<<16) | (0x1F<<0));
    }
    else  
    {
        //pinmux register d414[21:20]/d40c[31:30]/d40c[17], function 2
        MSDC_CLRBIT(0xF000D40C, (0x03<<30) | (0x01<<17));
        MSDC_SETBIT(0xF000D40C, (0x02<<30) | (0x01<<17));

        MSDC_CLRBIT(0xF000D414, 0x03<<20);
        MSDC_SETBIT(0xF000D414, 0x02<<20);

        //Local Arbitor open
        MSDC_CLRBIT(0xF0012200, (0x03<<16) | (0x1F<<5));	
        MSDC_SETBIT(0xF0012200, (0x01<<16) | (0x1F<<5));	
    }

#elif defined(CC_MT5368)  
    //sync from rom code, should be confirmed when used
    //TODO: Confirm!
    MSDC_CLRBIT(0xF000D1D8, (0x3F<<0));

#elif defined(CC_MT5389)  
    //pinmux register d404[27:25], function 3 - DAT1
    //pinmux register d404[30:28], function 3 - CMD/CLK/DAT2/DAT3
    //pinmux register d408[1:0] & d404[31], function 3 - DAT0
    MSDC_CLRBIT(0xF000D404, (0x07<<25) | (0x07<<28) | (0x01<<31));
    MSDC_SETBIT(0xF000D404, (0x03<<25) | (0x03<<28) | (0x01<<31));
	
    MSDC_CLRBIT(0xF000D408, (0x03<<0));
    MSDC_SETBIT(0xF000D408, (0x01<<0));
	  
#elif defined(CC_MT5398)
    if(ch == 0)
    {
        //pinmux register d604[27:26], function 1 - CMD/CLK/DAT0~DAT3
        MSDC_CLRBIT(0xF000D604, 0x03<<26);
        MSDC_SETBIT(0xF000D604, 0x01<<26);
        
        //Local Arbitor open
        MSDC_CLRBIT(0xF0012200, (0x03<<16) | (0x1F<<0));	 
        MSDC_SETBIT(0xF0012200, (0x01<<16) | (0x1F<<0));  
    }
    else
    {
        //pinmux register d600[5:4], function 2 - CMD/DAT0~DAT3
        //pinmux register d600[7:6], function 2 - DAT4~DAT7
        //pinmux register d610[7], function 1 - CLK
        MSDC_CLRBIT(0xF000D600, (0x03<<4) | (0x03<<6));
        MSDC_SETBIT(0xF000D600, (0x02<<4) | (0x02<<6));
        MSDC_CLRBIT(0xF000D610, 0x01<<7);
        MSDC_SETBIT(0xF000D610, 0x01<<7);
        
        //Local Arbitor open
        MSDC_CLRBIT(0xF0012200, (0x03<<16) | (0x1F<<5));	
        MSDC_SETBIT(0xF0012200, (0x01<<16) | (0x1F<<5));    	
    }

#elif defined(CC_MT5880) || defined(CC_MT5860)

    if (IS_IC_5860_A_ES2())    
    {
        if (ch == 0)
        {
            MSDC_CLRBIT(0xF000D620, (0x01<<7));
            MSDC_SETBIT(0xF000D620, (0x01<<7));
            MSDC_CLRBIT(0xF000D610, 0x3FFFF<<12);
            MSDC_SETBIT(0xF000D610, 0x12492<<12);
            //Local Arbitor open
            MSDC_CLRBIT(0xF0012200, (0x03<<16) | (0x1F<<0));	 
            MSDC_SETBIT(0xF0012200, (0x01<<16) | (0x1F<<0));
        }
        else
        {
            //pinmux register D604[5:2], function 2 - CMD/CLK/DAT0/DAT1/DAT2/DAT3
            //TODO: the pinmux setting after MT5860 ECO 
            MSDC_CLRBIT(0xF000D604, (0xf<<2));
            MSDC_SETBIT(0xF000D604, 0xa<<2);
            //Local Arbitor open
            MSDC_CLRBIT(0xF0012200, (0x03<<16) | (0x1F<<5));	
            MSDC_SETBIT(0xF0012200, (0x01<<16) | (0x1F<<5));
        }
    }
    else
    {
        ch = 0;
        MSDC_CLRBIT(0xF000D604, (0x03<<4));
        MSDC_SETBIT(0xF000D604, (0x02<<4));
    }
#elif defined(CC_MT5399) || defined(CC_MT5399)|| \
      defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)
    if(IS_IC_5861_ES1())
	{
		if(ch == 0)
		{
				//pinmux register d604[27:26], function 1 - CMD/CLK/DAT0~DAT3
				MSDC_CLRBIT(0xF000D604, 0x03<<26);
				MSDC_SETBIT(0xF000D604, 0x01<<26);
				
				//Local Arbitor open
				MSDC_CLRBIT(0xF0012200, (0x1));  
				MSDC_SETBIT(0xF0012200, (0x1)); 		
		}
		else
		{
				MSDC_CLRBIT(0xF000D600, (0xF<<4));
				MSDC_SETBIT(0xF000D600, (0xA<<4));
				MSDC_CLRBIT(0xF000D610, 0x1<<7);
				MSDC_SETBIT(0xF000D610, 0x1<<7);
				
				//Local Arbitor open
				MSDC_CLRBIT(0xF0012200, (0x1<<5));	
				MSDC_SETBIT(0xF0012200, (0x1<<5));		
		}

	}
	else
	{	
    if(ch == 0)
    {
        //pinmux register d604[27:26], function 1 - CMD/CLK/DAT0~DAT3
        MSDC_CLRBIT(0xF000D604, 0x03<<26);
        MSDC_SETBIT(0xF000D604, 0x01<<26);
        
        //Local Arbitor open
        MSDC_CLRBIT(0xF0012200, (0x03<<16) | (0x1F<<0));	 
        MSDC_SETBIT(0xF0012200, (0x01<<16) | (0x1F<<0));      	
    }
    else
    {
        //pinmux register d600[5:4], function 2 - CMD/DAT0~DAT3
        //pinmux register d600[7:6], function 2 - DAT4~DAT7
        //pinmux register d610[7], function 1 - CLK
        MSDC_CLRBIT(0xF000D600, (0x03<<4) | (0x03<<6));
        MSDC_SETBIT(0xF000D600, (0x02<<4) | (0x02<<6));
        MSDC_CLRBIT(0xF000D610, 0x01<<7);
        MSDC_SETBIT(0xF000D610, 0x01<<7);
        
        //Local Arbitor open
        MSDC_CLRBIT(0xF0012200, (0x03<<16) | (0x1F<<5));	
        MSDC_SETBIT(0xF0012200, (0x01<<16) | (0x1F<<5));    	
    }

	}

 #elif   defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
     if(ch == 0)
    {
        //pinmux register d604[27:26], function 1 - CMD/CLK/DAT0~DAT3
        MSDC_CLRBIT(0xF000D604, 0x7<<29);
        MSDC_SETBIT(0xF000D604, 0x1<<29);
        
        //Local Arbitor open
        MSDC_CLRBIT(0xF0012200, (0x1));	 
        MSDC_SETBIT(0xF0012200, (0x1));      	
    }
    else
    {
        MSDC_CLRBIT(0xF000D600, (0xF<<4) | (0x1<<2));
        MSDC_SETBIT(0xF000D600, (0xA<<4) | (0x1<<2));
        
        //Local Arbitor open
        MSDC_CLRBIT(0xF0012200, (0x1<<5));	
        MSDC_SETBIT(0xF0012200, (0x1<<5));    	
    }
#endif

}

VOID MsdcPrintBuff(UINT8 *pBuff, UINT32 u4Len)
{
    UINT32 i;
    
    for(i = 0;i< u4Len;i+=0x04)
    {
        if(i%0x10 == 0)
        {
            MSDC_LOG(MSG_LVL_TITLE, "%08X |", i);	
        }
        
        MSDC_LOG(MSG_LVL_TITLE, " %08X", *(UINT32 *)(pBuff + i));
        
        if((i+0x04)%0x10 == 0) 
        {
            MSDC_LOG(MSG_LVL_TITLE, "\n");	
        }       	
    }	
    
    MSDC_LOG(MSG_LVL_TITLE, "\n");
}

//---------------------------------------------------------------------------
// MSDC interface function
//---------------------------------------------------------------------------
INT32 MsdcCardIdentify(VOID)
{    
    INT32 i4Ret = MSDC_SUCCESS;
    UINT32 u4ClkIndx = 0;
	  
    i4Ret = MsdcSetClkFreq(msdcClk[0][MSDC_INIT_CLK_IDX]);
    if(MSDC_SUCCESS != i4Ret)
    {
        return MSDC_FAILED;
    }

    i4Ret = MsdcCardRescan();
    if(MSDC_SUCCESS != i4Ret)
    {
        return MSDC_FAILED;
    }

    i4Ret = MsdcHandleCSD(0);
    if(MSDC_SUCCESS != i4Ret)
    {
        return MSDC_FAILED;
    }

    // CMD7 Select the target card 
    i4Ret = cmd7_select_card(1);
    if(CMD_ERR_SUCCESS != i4Ret)
    {
        return MSDC_FAILED;
    }
	
    if(sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {
        if(MsdcGetEXTCSD())
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to Get EXT_CSD.\n");	
        }
    }

    // Set bus width
    i4Ret = MsdcSetBusWidth(sdHost[ch].busWidth);
    if(MSDC_SUCCESS != i4Ret)
    {
        return MSDC_FAILED;
    }

    if(sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {        
        sdHost[ch].speedMode = SPEED_MODE_DS;
        u4ClkIndx = MSDC_NORM_CLK_IDX;	
    }
    else if(sdCard[ch].cardType == CARDTYPE_MEM_SD)
    {
        i4Ret = MsdcGetSCR();
        if(MSDC_SUCCESS != i4Ret)
        {
            return MSDC_FAILED;
        }
        
        i4Ret = MsdcEnHSMode(1);
        if(MSDC_SUCCESS != i4Ret)
        {
            MSDC_LOG(MSG_LVL_WARN, "Not Support HS mode.\n");	
			sdHost[ch].speedMode = SPEED_MODE_DS;
            u4ClkIndx = MSDC_NORM_CLK_IDX;	
        }
        else
        {
            sdHost[ch].speedMode = SPEED_MODE_HS;
            u4ClkIndx = MSDC_HIGH_CLK_IDX;
        }       
    }
    
    i4Ret = MsdcSetClkFreq(msdcClk[0][u4ClkIndx]);
    if(MSDC_SUCCESS != i4Ret)
    {
        return MSDC_FAILED;
    }

    return MSDC_SUCCESS;
}

INT32 MsdcSysInit(VOID)
{
    MsdcInitHostConfig();
	
    // Init MSDC
    if(MSDC_SUCCESS != MsdcInit())
    {
        return MSDC_FAILED;
    }
   
    // Initial Msdc Freq setting flag
    MsdcContinueClock(0);    // Clock power down when no SD Command
   
    return MSDC_SUCCESS;
}

INT32 MsdcSetDataMode(UINT32 fgDataMode)
{
    sdHost[ch].dataMode = fgDataMode;
    if(fgDataMode > PIO_DATA_MODE)
    {
        // Set Host DMA Mode
        MSDC_CLRBIT(MSDC_CFG, MSDC_CFG_PIO_MODE);
    }
    else
    {
        // Set Host PIO Mode
        MSDC_SETBIT(MSDC_CFG, MSDC_CFG_PIO_MODE);
    }

    return MSDC_SUCCESS; 
}

/* There are four kinds of data end mode:
* 0. send cmd12 by hand
* 1. send cmd23 by hand
* 2. send cmd12 by auto
* 3. send cmd23 by auto
* cmd12 can be supported by any version of MMC and SD standard;
* but CMD23 can only be supported by MMC and SD3.0
*/
INT32 MsdcSetDataEndMode(UINT32 fgEndMode)
{
    fgEndMode &= (MSDC_DATA_PRE_DEF_MASK | MSDC_DATA_END_AUTO_MASK);
    sdHost[ch].hostAttr &= ~(MSDC_DATA_PRE_DEF_MASK | MSDC_DATA_END_AUTO_MASK);
    sdHost[ch].hostAttr |= fgEndMode;

    return MSDC_SUCCESS; 
}

INT32 MsdcSetDDRMode(UINT32 fgDDRMode)
{
    UINT32 i4Ret = MSDC_SUCCESS;

    i4Ret = MsdcEnDDRMode(fgDDRMode);
    if(MSDC_SUCCESS != i4Ret)
    {
        return MSDC_FAILED;
    }

    sdHost[ch].hostAttr &= ~(MSDC_DATA_DDR_MODE_MASK);
    if(fgDDRMode)
    {
        sdHost[ch].hostAttr |= (0x1<<2);
    }

    sdHost[ch].speedMode = SPEED_MODE_DDR50;    

    return MSDC_SUCCESS; 
}

INT32 MsdcSetDataBusWidth(UINT32 busWidth)
{
    sdHost[ch].busWidth = busWidth;

    if(MSDC_SUCCESS != MsdcSetBusWidth(sdHost[ch].busWidth))
    {
        MSDC_LOG(MSG_LVL_ERR, "Set Data Bus Width to %dbit Mode Failed\n", busWidth);
        return MSDC_FAILED;
    }

    return MSDC_SUCCESS; 
}

INT32 MsdcSetSpeedMode(UINT32 SpeedMode)
{
    sdHost[ch].speedMode = SpeedMode;

    if(MSDC_SUCCESS != MsdcEnHSMode(sdHost[ch].speedMode))
    {
        MSDC_LOG(MSG_LVL_ERR, "Switch speed mode to %s Mode Failed\n", SpeedMode?"High":"Normal");
        return MSDC_FAILED;
    }

    if((SpeedMode == SPEED_MODE_DDR50) &&
	   (sdCard[ch].cardType == CARDTYPE_MEM_SD))
    {
		sdHost[ch].hostAttr &= ~(MSDC_DATA_DDR_MODE_MASK);
		sdHost[ch].hostAttr |= (0x1<<2);
    }

    return MSDC_SUCCESS; 
}

INT32 MsdcGetRegisters(UINT32 fgWhich)
{
    UINT32 u4Tmp;	
	
    switch(fgWhich)
    {
    case 0:
        cmd7_select_card(0);
        MsdcHandleCSD(0);
        cmd7_select_card(1);
        MsdcPrintBuff((UINT8 *)(&sdCard[ch].csd), sizeof(sdCard[ch].csd));
        break;
    case 1:
    	  MsdcGetEXTCSD();
    	  MsdcPrintBuff((UINT8 *)sdHost[ch].EXT_CSD, sizeof(sdHost[ch].EXT_CSD));
        break;
    case 2:
    	  MsdcGetStatus(&u4Tmp);
    	  MsdcPrintBuff((UINT8 *)(&u4Tmp), sizeof(u4Tmp));
        break;
    case 3:
    	  MsdcPrintBuff((UINT8 *)(&sdCard[ch].cid), sizeof(sdCard[ch].cid));
        break;
    case 4:
    	  MsdcPrintBuff((UINT8 *)(&sdCard[ch].memOcr), sizeof(sdCard[ch].memOcr));
        break;
    default:   
        break; 		
    }   
    
    return MSDC_SUCCESS; 	
}

INT32 MsdcGetDevInfo(MSDC_DevInfo_T *MsdcDevInfo)
{	
    UINT8 EraseGrpDef = 0;	
    UINT32 *pcsd = ((UINT32 *)&sdCard[ch].csd);
    UINT8 *pext_csd = sdHost[ch].EXT_CSD;
    UINT32 blocknr, mult, block_len;
    x_memset(MsdcDevInfo, 0, sizeof(MSDC_DevInfo_T));
    
    if (sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {
        EraseGrpDef = ERASE_GROUP_DEF(pext_csd);
    }
   
    MsdcDevInfo->Write_BlkSz = (0x1<<WRITE_BL_LEN(pcsd));
      
    if (sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {  
        if(0 == EraseGrpDef)
        {
            MsdcDevInfo->Erase_GrpSz = MsdcDevInfo->Write_BlkSz; 
            MsdcDevInfo->Erase_GrpSz *= (ERASE_GRP_SIZE(pcsd) + 1)*(ERASE_GRP_MULT(pcsd) + 1);
	        
            MsdcDevInfo->WP_GrpSz = MsdcDevInfo->Erase_GrpSz;
            MsdcDevInfo->WP_GrpSz *= WP_GRP_SIZE(pcsd) + 1;  	
        }
        else
        {
            MsdcDevInfo->Erase_GrpSz = (512*1024); 
            MsdcDevInfo->Erase_GrpSz *= HC_ERASE_GRP_SIZE(pext_csd);
	        
            MsdcDevInfo->WP_GrpSz = MsdcDevInfo->Erase_GrpSz;
            MsdcDevInfo->WP_GrpSz *= HC_WP_GRP_SIZE(pext_csd);       	
        }
    }
    else if(sdCard[ch].cardType == CARDTYPE_MEM_SD)
    {
    	  // need not to use these, so assign 0x200 to them
        MsdcDevInfo->Erase_GrpSz = 0x200;
        MsdcDevInfo->WP_GrpSz = 0x200;	
    }
    
    
    if(HIGH_CAPACITY(*((UINT32 *)(&sdCard[ch].memOcr))))
    {
        MsdcDevInfo->Dev_Sz = 0x200;
        if (sdCard[ch].cardType == CARDTYPE_MEM_MMC)
        {
            MsdcDevInfo->Dev_Sz *= SEC_COUNT(pext_csd); 
        }
        else if((sdCard[ch].cardType == CARDTYPE_MEM_SD) &&
                (CSD_STRUCTURE(pcsd) == 0x1))
        {
            MsdcDevInfo->Dev_Sz *= 1024;
            MsdcDevInfo->Dev_Sz *= (C_SIZE_SD20(pcsd) + 1);	
        }
        
        //Printf("HDev:%08X, LDev:%08X, sec_count:%08X", (UINT32)(MsdcDevInfo->Dev_Sz>>32), 
                                                   //(UINT32)(MsdcDevInfo->Dev_Sz), C_SIZE_SD20(pcsd));	
    }
    else
    {
        mult = (0x1<<(C_SIZE_MULT(pcsd)+2));  
        block_len = (0x1<<READ_BL_LEN(pcsd));
        blocknr = (C_SIZE(pcsd))*mult;
        
        MsdcDevInfo->Dev_Sz = blocknr*block_len; 
        //Printf("devsz:0x%08X%08X %08X %08X %08X %08X\n", (UINT32)(MsdcDevInfo->Dev_Sz>>32), (UINT32)(MsdcDevInfo->Dev_Sz),  
                        //C_SIZE_MULT(pcsd), mult, READ_BL_LEN(pcsd), C_SIZE(pcsd));  
    }
    
    if (sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {
        MsdcDevInfo->Boot_Partition_Sz = (128*1024);
        MsdcDevInfo->Boot_Partition_Sz *= BOOT_SIZE_MULT(pext_csd);
    }

    return MSDC_SUCCESS; 	    	
}

INT32 MsdcSetBusClock(UINT32 busClock)
{
    return MsdcSetClkFreq(busClock);
}

INT32 MsdcSetContinueClock(UINT32 fgContinueClock)
{
    MsdcContinueClock(fgContinueClock);

    return MSDC_SUCCESS;
}

INT32 MsdcPrintConfigInfo(VOID)
{
    if(sdHost[ch].fgValid == 0x484F5354)
    {
        MSDC_LOG(MSG_LVL_TITLE, "\n**********MSDC HOST CONFIG INFORMATION**********\n");
        MSDC_LOG(MSG_LVL_TITLE, "Block Length: %08X(Fixed)\n", sdHost[ch].blkLen);
        MSDC_LOG(MSG_LVL_TITLE, "BUS Width: %d\n", sdHost[ch].busWidth);
        MSDC_LOG(MSG_LVL_TITLE, "High Speed Mode: %d\n", sdHost[ch].speedMode);
			
        if(sdHost[ch].dataMode == PIO_DATA_MODE)
        {
            MSDC_LOG(MSG_LVL_TITLE, "Data Mode: PIO\n");
        }
        else if(sdHost[ch].dataMode == BASIC_DMA_DATA_MODE)
        {
            MSDC_LOG(MSG_LVL_TITLE, "Data Mode: Basic DMA\n");
        }
        else if(sdHost[ch].dataMode == DESC_DMA_DATA_MODE)
        {
            MSDC_LOG(MSG_LVL_TITLE, "Data Mode: Descriptor DMA\n");
        }
        else if(sdHost[ch].dataMode == ENHANCED_DMA_DATA_MODE)
        {
            MSDC_LOG(MSG_LVL_TITLE, "Data Mode: Enhanced DMA\n");
        }
		
        if(sdHost[ch].dataMode > PIO_DATA_MODE)
        {
            MSDC_LOG(MSG_LVL_TITLE, "DMA Burst Size: %d(Bytes)\n", 0x1<<sdHost[ch].dmaBstSize);
        }
			
        MSDC_LOG(MSG_LVL_TITLE, "MAX DMA XFER SIZE: %04X\n", sdHost[ch].maxBasicDmaLen);
		
        MSDC_LOG(MSG_LVL_TITLE, "MAX Clock Frequency: %d(MHz)\n", sdHost[ch].maxClock);
        MSDC_LOG(MSG_LVL_TITLE, "Current Clock Frequency: %d(MHz)\n", sdHost[ch].curClock);
		
        if(sdHost[ch].polling == 1)
        {
            MSDC_LOG(MSG_LVL_TITLE, "Interrupt Handle: Polling\n");
        }
        else if(sdHost[ch].polling == 0)
        {
            MSDC_LOG(MSG_LVL_TITLE, "Interrupt Handle: ISR\n");
        }

        MSDC_LOG(MSG_LVL_TITLE, "Host Cap: %08X\n", sdHost[ch].hostCap);
        MSDC_LOG(MSG_LVL_TITLE, "Host Attr: %08X\n", sdHost[ch].hostAttr);
    }
    else
    {
        MSDC_LOG(MSG_LVL_ERR, "Not Valid configuration for MSDC%d\n", ch);	
    }

    return MSDC_SUCCESS; 
	
}

INT32 MsdcIdentyCard(BOOL fgForceIdentCard)
{
    if(sdHost[ch].fgCardIdent && !fgForceIdentCard)
    {
        return MSDC_SUCCESS;
    }

    // Clear sdCard structure, let card re-identify	
    x_memset(&sdCard[ch], 0, sizeof(sdcard_t));

    // Identify Card
    if(MSDC_SUCCESS != MsdcCardIdentify()) 
    {
        return MSDC_FAILED;
    }

    sdHost[ch].fgCardIdent = TRUE;
    return MSDC_SUCCESS;
}

INT32 MsdcReadCard(UINT64 u8Addr, UINT32 *pu4Buf, UINT32 u4Length)
{
    INT32 i4Ret;
    UINT64 u8BlkOffset;
    UINT32 u4OffsetInBlk, u4RestSize = u4Length;
    UINT32 u4MemPtr = (UINT32)pu4Buf;
    
    u4OffsetInBlk = (UINT32)(u8Addr & (SDHC_BLK_SIZE - 1));
    u8BlkOffset = (UINT64)(u8Addr - u4OffsetInBlk);

    MSDC_LOG(MSG_LVL_INFO, "Read Address %08X, Length: %08X, u4OffsetInBlk: %08X, u4BlkOffset: %08X.\n", u8Addr, u4Length, u4OffsetInBlk, u8BlkOffset);
    
    // Handle offset non-block-size-aligned case
    if(u4OffsetInBlk)
    {
        UINT32 u4OpSize;
        i4Ret = MsdcMemRead(u8BlkOffset, (VOID *)_pu4BlkBuf, SDHC_BLK_SIZE);
        if (i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "MsdcReadCard: Read In Process #1 Failed!\n");
            return i4Ret;
        }
        
        if (u4RestSize >= (SDHC_BLK_SIZE - u4OffsetInBlk))
        {
            u4OpSize = SDHC_BLK_SIZE - u4OffsetInBlk;                
        } 
        else
        {
            u4OpSize = u4RestSize;
        }

        x_memcpy((VOID *)u4MemPtr, (VOID *)((UINT8 *)_pu4BlkBuf + u4OffsetInBlk), u4OpSize); 
        
        u4RestSize  -= u4OpSize;
        u4MemPtr    += u4OpSize;
        u8BlkOffset += SDHC_BLK_SIZE;
    }     

    // Handle offset & size block-size-aligned case
    if(u4RestSize >= SDHC_BLK_SIZE)
    {
        UINT32 u4ByteCnt = u4RestSize & (~(SDHC_BLK_SIZE - 1));
        
        i4Ret = MsdcMemRead(u8BlkOffset, (VOID *)u4MemPtr, u4ByteCnt); 
        if (i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "MsdcReadCard: Read In Process #2 Failed!\n");
            return i4Ret;
        } 
        
        u4RestSize  -= u4ByteCnt;
        u4MemPtr    += u4ByteCnt;
        u8BlkOffset += u4ByteCnt;
    }  
    
    // Handle size none-block-size-aligned case
    if(u4RestSize)
    {
        i4Ret = MsdcMemRead(u8BlkOffset, (VOID *)_pu4BlkBuf, SDHC_BLK_SIZE); 
        if (i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "MsdcReadCard: Read In Process #3 Failed!\n");
            return i4Ret;      
        } 
        
        x_memcpy((VOID *)u4MemPtr, (VOID *)_pu4BlkBuf, u4RestSize);
        
    }
	
    return MSDC_SUCCESS;

}

INT32 MsdcWriteCard(UINT32 *u4SrcAddr, UINT64 u8ByteOffset, UINT32 u4Len)
{
    INT32 i4Ret;
    UINT64 u8BlkOffset;
    UINT32 u4OffsetInBlk, u4RestSize = u4Len;
    UINT32 u4MemPtr = (UINT32)u4SrcAddr;

    //Printf("Write Address %08X, Length: %08X.\n", u4ByteOffset, u4Len);
    
    u4OffsetInBlk = (UINT32)(u8ByteOffset & (SDHC_BLK_SIZE - 1));
    u8BlkOffset = (UINT64)(u8ByteOffset - u4OffsetInBlk);
    
    // Handle offset non-block-size-aligned case
    if(u4OffsetInBlk)
    {
        UINT32 u4OpSize;
        i4Ret = MsdcMemRead(u8BlkOffset, (VOID *)_pu4BlkBuf, SDHC_BLK_SIZE);
        if(i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "MsdcWriteCard: Read In Write Process #1 Failed\n");
            return i4Ret;
        }
        
        if(u4RestSize >= (SDHC_BLK_SIZE - u4OffsetInBlk))
        {
            u4OpSize = SDHC_BLK_SIZE - u4OffsetInBlk;                
        } 
        else
        {
            u4OpSize = u4RestSize;
        }

        x_memcpy((VOID *)((UINT8 *)_pu4BlkBuf + u4OffsetInBlk), (VOID *)u4SrcAddr, u4OpSize); 
        
        i4Ret = MsdcMemWrite(u8BlkOffset, (VOID *)_pu4BlkBuf, SDHC_BLK_SIZE); 
        if (i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "MsdcWriteCard: Write In Write Process #1 Failed\n");
            return i4Ret;
        } 
        
        u4RestSize  -= u4OpSize;
        u4MemPtr    += u4OpSize;
        u8BlkOffset += SDHC_BLK_SIZE;
    }     

    // Handle offset & size block-size-aligned case
    if(u4RestSize >= SDHC_BLK_SIZE)
    {
        UINT32 u4ByteCnt = u4RestSize & (~(SDHC_BLK_SIZE - 1));
        
        i4Ret = MsdcMemWrite(u8BlkOffset, (VOID *)u4SrcAddr, u4ByteCnt); 
        if (i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "MsdcWriteCard: Write In Write Process #2 Failed\n");
            return i4Ret;
        } 
        
        u4RestSize  -= u4ByteCnt;
        u4MemPtr    += u4ByteCnt;
        u8BlkOffset += u4ByteCnt;
    }  
    
    // Handle size none-block-size-aligned case
    if(u4RestSize)
    {
        i4Ret = MsdcMemRead(u8BlkOffset, (VOID *)_pu4BlkBuf, SDHC_BLK_SIZE); 
        if (i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "MsdcWriteCard: Read In Write Process #3 Failed\n");
            return i4Ret;      
        } 
        
        x_memcpy((VOID *)_pu4BlkBuf, (VOID *)u4MemPtr, u4RestSize);

        i4Ret = MsdcMemWrite(u8BlkOffset, (VOID *)_pu4BlkBuf, SDHC_BLK_SIZE);         
        if (i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "MsdcWriteCard: Write In Write Process #3 Failed\n");
            return i4Ret;       
        } 
        
    }
	
    return MSDC_SUCCESS;
}

INT32 MsdcEraseCard(UINT64 u8ByteOffset, UINT32 u4Len, UINT32 u4EraseType)
{
    UINT64 u8StartAddr, u8EndAddr;
    
    u8StartAddr = u8ByteOffset;
    u8EndAddr = (UINT64)(u8ByteOffset + u4Len - 1);
    
    if((u4EraseType > 0) || (u4EraseType < 5))
    {
        if((u4EraseType == 1) && 
          ((SEC_FEATURE_SUPPORT(sdHost[ch].EXT_CSD) & SEC_GB_CL_EN_MASK) == 0x0))
        {
            MSDC_LOG(MSG_LVL_WARN, "Trim operation is not supported\n");	
            return MSDC_SUCCESS;
        }
        
        if((u4EraseType == 2) &&
          ((SEC_FEATURE_SUPPORT(sdHost[ch].EXT_CSD) & SEC_ER_EN_MASK) == 0x0))
        {
            MSDC_LOG(MSG_LVL_WARN, "Secure Erase operation is not supported\n");	
            return MSDC_SUCCESS;
        }	
        
        if(((u4EraseType == 3) || (u4EraseType == 4)) &&
          ((SEC_FEATURE_SUPPORT(sdHost[ch].EXT_CSD) & SEC_GB_CL_EN_MASK) == 0x0) &&
          ((SEC_FEATURE_SUPPORT(sdHost[ch].EXT_CSD) & SEC_ER_EN_MASK) == 0x0))
        {
            MSDC_LOG(MSG_LVL_WARN, "Secure Trim Step 1 operation is not supported\n");	
            return MSDC_SUCCESS;
        }      
    }

    return MsdcMemErase(u8StartAddr, u8EndAddr, u4EraseType);
}

VOID ReverseBuffer(UINT8 *pBuff, UINT32 u4Len)
{
    UINT32 i, j;
    UINT8 u1Tmp;

    for(i = 0, j = u4Len - 1;i < j; i++, j--)
    {
        u1Tmp = pBuff[i];
        pBuff[i] = pBuff[j];
        pBuff[j] = u1Tmp;
    }
}

INT32 MsdcWPCard(UINT32 u4WPLevel, UINT32 u4WPType, UINT32 u4fgEn, UINT64 wpAddr)
{
    UINT32 *pcsd = ((UINT32 *)&sdCard[ch].csd);
    UINT8 *pext_csd = sdHost[ch].EXT_CSD;
    UINT32 u4Tmp = 0;
    
    MsdcGetEXTCSD();
    	
    /* u4WPLevel:
        *    0 - All Card
        *    1 - USER AREA
        *    2 - BOOT PARTITION
        * u4WPType:
        *    0 - Temporary
        *    1 - Power on
        *    2 - Permenent 
        * u4fgEn:
        *    0 - Disable WP
        *    1 - Enable WP
        */
    if(0 == u4WPLevel)
    {
        if(u4WPType == 1)
        {
            MSDC_LOG(MSG_LVL_WARN, "Not support Power-on write protect for All card!\n");
            return MSDC_SUCCESS; 	
        }
    	  
        if(((u4WPType == 0) && (TMP_WRITE_PROTECT(pcsd) == u4fgEn)) ||
        	((u4WPType == 2) && (PERM_WRITE_PROTECT(pcsd) == u4fgEn)))
        {
            MSDC_LOG(MSG_LVL_WARN, "write protect has been %s!\n", (u4fgEn == 0)?"disable":"enable");
            return MSDC_SUCCESS; 	
        }	
        
        if(u4WPType == 0)
        {
            if(0x1 == u4fgEn)
            {
                SET_TMP_WRITE_PROTECT(pcsd);	
            }
            else if(0x0 == u4fgEn)
            {
        	      CLR_TMP_WRITE_PROTECT(pcsd);
            }
        }

#if 0        
        if(u4WPType == 2)
        {
            if(0x1 == u4fgEn)
            {
                SET_PERM_WRITE_PROTECT(pcsd);	
            }
            else if(0x0 == u4fgEn)
            {
        	      CLR_PERM_WRITE_PROTECT(pcsd);
            }
        } 
#endif

        ReverseBuffer((UINT8 *)pcsd, 16);
        if(MSDC_SUCCESS != MsdcHandleCSD(1))
        {
            MSDC_LOG(MSG_LVL_ERR, "MsdcWPCard: Program CSD Failed\n"); 
            ReverseBuffer((UINT8 *)pcsd, 16);
            return MSDC_FAILED;
        }
        ReverseBuffer((UINT8 *)pcsd, 16);

        if (CMD_ERR_SUCCESS != MsdcGetStatus((VOID *)(&u4Tmp)))
        {
            MSDC_LOG(MSG_LVL_ERR, "MsdcWPCard: Send CMD13 Failed\n");
            return MSDC_FAILED;
        }
        if(u4Tmp & (0x1<<16))
        {
            MSDC_LOG(MSG_LVL_WARN, "Read only section not match the card content!\n");
            return MSDC_FAILED;
        }	        
               
    }
    else if(1 == u4WPLevel)
    {  
        if(u4WPType == 0)
        {
            if(1 == u4fgEn)
            {
                CLR_US_PWR_WP_EN(pext_csd);
                CLR_US_PERM_WP_EN(pext_csd);
                MsdcSetEXTCSD(171, USER_WP(pext_csd));
            }   
            MsdcWPSeg(wpAddr, u4fgEn);	
        }
        else if (u4WPType == 1)
        {
            if(0 == u4fgEn)
            {
                MSDC_LOG(MSG_LVL_WARN, "Not support disable Power-on/Permenant write protect\n");	
                return MSDC_SUCCESS;
            }
            SET_US_PWR_WP_EN(pext_csd);
            CLR_US_PERM_WP_EN(pext_csd); 
            MsdcSetEXTCSD(171, USER_WP(pext_csd));  
            MsdcWPSeg(wpAddr, u4fgEn);    
        }
#if 0
        else if(u4WPType == 2)
        {
            if(0 == u4fgEn)
            {
                MSDC_LOG(MSG_LVL_WARN, "Not support disable Power-on/Permenant write protect\n");	
                return MSDC_SUCCESS;
            }
            SET_US_PERM_WP_EN(pext_csd); 
            MsdcSetEXTCSD(171, USER_WP(pext_csd));  
            MsdcWPSeg(wpAddr, u4fgEn); 		
        }	
#endif
    }
    else
    {  
        if(u4WPType == 0)
        {
            MSDC_LOG(MSG_LVL_WARN, "Boot partition not support temporary protect!\n");    	
        }
        else if (u4WPType == 1)
        {
            CLR_B_PERM_WP_EN(pext_csd);
            CLR_B_PWR_WP_EN(pext_csd);  
            MsdcSetEXTCSD(173, BOOT_WP(pext_csd));
        }
#if 0
        else if(u4WPType == 2)
        {
            SET_B_PERM_WP_EN(pext_csd); 
            MsdcSetEXTCSD(173, BOOT_WP(pext_csd));		
        }	
#endif    	
    }
    
    return MSDC_SUCCESS;
	
}

VOID MsdcToggleContClock(VOID)
{
    static INT32 i4ContClock = 1;
    
    // Continue clock & non-continue clock toggle for retry  
    if (i4ContClock)
    {
        MsdcContinueClock(0);    // Clock power down when no SD Command
    }
    else
    {
        MsdcContinueClock(1);    // Clock continues even if no SD Command
    }

    i4ContClock = !i4ContClock;
}

INT32 MsdcIdentReadMMC(UINT32 *pu4Buf, UINT64 u8Addr, UINT32 u4Length)
{
    INT32 i4Ret;
    INT32 i4RetryCnt = 0;

    // Identify Card    
    i4RetryCnt = 0;
    do
    {
        if(i4RetryCnt == 0)
        {
            // Don't need force identify card in the first time
            i4Ret = MsdcIdentyCard(FALSE);
        }
        else
        {
            // Force identify if retry
            i4Ret = MsdcIdentyCard(TRUE);
        }
        
        if(i4Ret != MSDC_SUCCESS)
        {
            // Toggle Continue clock setting for retry
            MsdcToggleContClock();
        } 

        if(i4RetryCnt++ > MSDC_IDENTIFY_CARD_RETRY_LIMIT)
        {
            break;
        }
    } while (i4Ret != MSDC_SUCCESS);

    if(i4Ret != MSDC_SUCCESS)
    {
        MSDC_LOG(MSG_LVL_ERR, "# 2\n");
        return EMMC_IDENT_CARD_FAIL;
    }

    // Read data
    i4RetryCnt = 0;
    do
    {
        i4Ret = MsdcReadCard(u8Addr, pu4Buf, u4Length);
        if(i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "# 3\n");	
            // Toggle Continue clock setting for retry
            MsdcToggleContClock();
            
            // If retry too many times, re-init the eMMC card
            if (i4RetryCnt >= (MSDC_READ_BOOTLDR_DATA_RETRY_LIMIT/2))
            {
                MsdcIdentyCard(TRUE);
            }
        }         
        
        if(i4RetryCnt++ > MSDC_READ_BOOTLDR_DATA_RETRY_LIMIT)
        {
            break;
        }
    } while (i4Ret != MSDC_SUCCESS);

    if(i4Ret != MSDC_SUCCESS)
    {
        return EMMC_READ_CARD_FAIL;
    }

    return EMMC_SUCCESS;
}

INT32 eMMC2DRAM(UINT64 u8Addr, UINT32 *pSrcOrDes, UINT32 u4Length, UINT32 u4Type)
{
    INT32 i4Ret;
    UINT32 i4RetryCnt = 0;
    static UINT32 fgInited = 0;
#ifdef CC_EMMC_BOOT
    LDR_ENV_T *prLdrEnv = (LDR_ENV_T *)CC_LDR_ENV_OFFSET;
    sdcard_t sdTmpCard;
    #if (defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882))   
    UINT32 hs_max_dtr = 0;
    #endif
#endif

#ifdef CC_EMMC_BOOT
    UNUSED(prLdrEnv);
#endif
    UNUSED(i4Ret);
    UNUSED(i4RetryCnt);

    if(!fgInited)
    {
#if defined(CC_MTD_ENCRYPT_SUPPORT)  
        MSDCEXT_EncryptInit(128);
#endif
#ifdef CC_PARTITION_WP_SUPPORT
        MSDCPART_WpConfig(1);
#endif

#ifdef CC_EMMC_BOOT
        MSDC_LOG(MSG_LVL_INFO, "LOADER FAST READ EMMC!\n");
        ASSERT(sizeof(prLdrEnv->szMSDCenv) == sizeof(sdcard_t));  
        x_memcpy(&sdTmpCard, (VOID *)(prLdrEnv->szMSDCenv), sizeof(sdcard_t));
        
        MSDC_PinMux(sdTmpCard.flagHost);
        
        x_memset(&sdCard[ch], 0, sizeof(sdcard_t));
        x_memcpy(&sdCard[ch], &sdTmpCard, sizeof(sdcard_t));
        sdCard[ch].flagHost = 1;
        MsdcInitHostConfig();
        
	if(sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {
        MsdcFindDevName((UINT32 *)(&(sdCard[ch].cid)));
    }
        MsdcSetDataMode(sdHost[ch].dataMode);
#ifdef CC_EMMC_HWRST_ENABLE 
       // MsdcGetRegisters(1); 
        
        if (CMD_ERR_SUCCESS != MsdcSetEXTCSD(162, 0x01))
        {
            MSDC_LOG(MSG_LVL_ERR,"Failed to Enable Hardware RST register.");
            return MSDC_FAILED;
        }
        {
            UINT32 status;
            do
            {
                if(MSDC_SUCCESS != MsdcGetStatus(&status))
                {
                    MSDC_LOG(MSG_LVL_ERR, "%s Failed to get card status\n", __FUNCTION__);  
                }
                MSDC_LOG(MSG_LVL_INFO, "(E)Card Status: %08X\n", status);
            }while(((status>>9)&0x0F) != 0x4);
        }
        MSDC_LOG(MSG_LVL_ERR,"Success to Enable Hardware RST register!\n");
      //  MsdcGetRegisters(1);
#endif
        
        //msdcenv.ext_csd_185 = sdHost[ch].EXT_CSD[185];
        //msdcenv.ext_csd_192 = sdHost[ch].EXT_CSD[192];
        //msdcenv.ext_csd_196 = sdHost[ch].EXT_CSD[196];
        //msdcenv.ext_csd_212 = sdHost[ch].EXT_CSD[212];
        //msdcenv.ext_csd_213 = sdHost[ch].EXT_CSD[213];
        //msdcenv.ext_csd_214 = sdHost[ch].EXT_CSD[214];
        //msdcenv.ext_csd_215 = sdHost[ch].EXT_CSD[215]; 
        ASSERT(sizeof(prLdrEnv->szMSDCenv1) == sizeof(msdc_env_t));  
        x_memcpy((VOID *)(prLdrEnv->szMSDCenv1), (VOID *)(&msdcenv), sizeof(msdc_env_t));
        
        sdHost[ch].fgCardIdent = TRUE;  
        MsdcSetBusClock(sdHost[ch].curClock); 
        #if (defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)) 
        #define EXT_CSD_CARD_TYPE_26	(1<<0)	/* Card can run at 26MHz */
        #define EXT_CSD_CARD_TYPE_52	(1<<1)	/* Card can run at 52MHz */
        #define EXT_CSD_CARD_TYPE_MASK	0x3F	/* Mask out reserved bits */
        #define EXT_CSD_CARD_TYPE_DDR_1_8V  (1<<2)   /* Card can run at 52MHz */
        					     /* DDR mode @1.8V or 3V I/O */
        #define EXT_CSD_CARD_TYPE_DDR_1_2V  (1<<3)   /* Card can run at 52MHz */
        					     /* DDR mode @1.2V I/O */
        #define EXT_CSD_CARD_TYPE_DDR_52       (EXT_CSD_CARD_TYPE_DDR_1_8V  \
        					| EXT_CSD_CARD_TYPE_DDR_1_2V)
        #define EXT_CSD_CARD_TYPE_SDR_1_8V	(1<<4)	/* Card can run at 200MHz */
        #define EXT_CSD_CARD_TYPE_SDR_1_2V	(1<<5)	/* Card can run at 200MHz */
        						/* SDR mode @1.2V I/O */
        
        #define EXT_CSD_CARD_TYPE_SDR_200	(EXT_CSD_CARD_TYPE_SDR_1_8V | \
        					 EXT_CSD_CARD_TYPE_SDR_1_2V)
        
        #define EXT_CSD_CARD_TYPE_SDR_ALL	(EXT_CSD_CARD_TYPE_SDR_200 | \
        					 EXT_CSD_CARD_TYPE_52 | \
        					 EXT_CSD_CARD_TYPE_26)
        
        #define	EXT_CSD_CARD_TYPE_SDR_1_2V_ALL	(EXT_CSD_CARD_TYPE_SDR_1_2V | \
        					 EXT_CSD_CARD_TYPE_52 | \
        					 EXT_CSD_CARD_TYPE_26)
        
        #define	EXT_CSD_CARD_TYPE_SDR_1_8V_ALL	(EXT_CSD_CARD_TYPE_SDR_1_8V | \
        					 EXT_CSD_CARD_TYPE_52 | \
        					 EXT_CSD_CARD_TYPE_26)
        
        #define EXT_CSD_CARD_TYPE_SDR_1_2V_DDR_1_8V	(EXT_CSD_CARD_TYPE_SDR_1_2V | \
        						 EXT_CSD_CARD_TYPE_DDR_1_8V | \
        						 EXT_CSD_CARD_TYPE_52 | \
        						 EXT_CSD_CARD_TYPE_26)
        
        #define EXT_CSD_CARD_TYPE_SDR_1_8V_DDR_1_8V	(EXT_CSD_CARD_TYPE_SDR_1_8V | \
        						 EXT_CSD_CARD_TYPE_DDR_1_8V | \
        						 EXT_CSD_CARD_TYPE_52 | \
        						 EXT_CSD_CARD_TYPE_26)
        
        #define EXT_CSD_CARD_TYPE_SDR_1_2V_DDR_1_2V	(EXT_CSD_CARD_TYPE_SDR_1_2V | \
        						 EXT_CSD_CARD_TYPE_DDR_1_2V | \
        						 EXT_CSD_CARD_TYPE_52 | \
        						 EXT_CSD_CARD_TYPE_26)
        
        #define EXT_CSD_CARD_TYPE_SDR_1_8V_DDR_1_2V	(EXT_CSD_CARD_TYPE_SDR_1_8V | \
        						 EXT_CSD_CARD_TYPE_DDR_1_2V | \
        						 EXT_CSD_CARD_TYPE_52 | \
        						 EXT_CSD_CARD_TYPE_26)
        
        #define EXT_CSD_CARD_TYPE_SDR_1_2V_DDR_52	(EXT_CSD_CARD_TYPE_SDR_1_2V | \
        						 EXT_CSD_CARD_TYPE_DDR_52 | \
        						 EXT_CSD_CARD_TYPE_52 | \
        						 EXT_CSD_CARD_TYPE_26)
        
        #define EXT_CSD_CARD_TYPE_SDR_1_8V_DDR_52	(EXT_CSD_CARD_TYPE_SDR_1_8V | \
        						 EXT_CSD_CARD_TYPE_DDR_52 | \
        						 EXT_CSD_CARD_TYPE_52 | \
        						 EXT_CSD_CARD_TYPE_26)
        
        #define EXT_CSD_CARD_TYPE_SDR_ALL_DDR_1_8V	(EXT_CSD_CARD_TYPE_SDR_200 | \
        						 EXT_CSD_CARD_TYPE_DDR_1_8V | \
        						 EXT_CSD_CARD_TYPE_52 | \
        						 EXT_CSD_CARD_TYPE_26)
        
        #define EXT_CSD_CARD_TYPE_SDR_ALL_DDR_1_2V	(EXT_CSD_CARD_TYPE_SDR_200 | \
        						 EXT_CSD_CARD_TYPE_DDR_1_2V | \
        						 EXT_CSD_CARD_TYPE_52 | \
        						 EXT_CSD_CARD_TYPE_26)
        
        #define EXT_CSD_CARD_TYPE_SDR_ALL_DDR_52	(EXT_CSD_CARD_TYPE_SDR_200 | \
        						 EXT_CSD_CARD_TYPE_DDR_52 | \
        						 EXT_CSD_CARD_TYPE_52 | \
        						 EXT_CSD_CARD_TYPE_26)
        	MsdcSetSpeedMode(1);				 
        	MsdcGetEXTCSD();
  	    switch (sdHost[ch].EXT_CSD[196] & EXT_CSD_CARD_TYPE_MASK) 
  	    {
      	case EXT_CSD_CARD_TYPE_SDR_ALL:
      	case EXT_CSD_CARD_TYPE_SDR_ALL_DDR_1_8V:
      	case EXT_CSD_CARD_TYPE_SDR_ALL_DDR_1_2V:
      	case EXT_CSD_CARD_TYPE_SDR_ALL_DDR_52:
      		hs_max_dtr = 200000000;
      		break;
      	case EXT_CSD_CARD_TYPE_SDR_1_2V_ALL:
      	case EXT_CSD_CARD_TYPE_SDR_1_2V_DDR_1_8V:
      	case EXT_CSD_CARD_TYPE_SDR_1_2V_DDR_1_2V:
      	case EXT_CSD_CARD_TYPE_SDR_1_2V_DDR_52:
      		hs_max_dtr = 200000000;
      		break;
      	case EXT_CSD_CARD_TYPE_SDR_1_8V_ALL:
      	case EXT_CSD_CARD_TYPE_SDR_1_8V_DDR_1_8V:
      	case EXT_CSD_CARD_TYPE_SDR_1_8V_DDR_1_2V:
      	case EXT_CSD_CARD_TYPE_SDR_1_8V_DDR_52:
      		hs_max_dtr = 200000000;
      		break;
      	case EXT_CSD_CARD_TYPE_DDR_52 | EXT_CSD_CARD_TYPE_52 |
      	     EXT_CSD_CARD_TYPE_26:
              hs_max_dtr = 52000000;
      		break;
      	case EXT_CSD_CARD_TYPE_DDR_1_2V | EXT_CSD_CARD_TYPE_52 |
      	     EXT_CSD_CARD_TYPE_26:
      		hs_max_dtr = 52000000;
      		break;
      	case EXT_CSD_CARD_TYPE_DDR_1_8V | EXT_CSD_CARD_TYPE_52 |
      	     EXT_CSD_CARD_TYPE_26:
      		hs_max_dtr = 52000000;
      		break;
      	case EXT_CSD_CARD_TYPE_52 | EXT_CSD_CARD_TYPE_26:
      		hs_max_dtr = 52000000;
      		break;
      	case EXT_CSD_CARD_TYPE_26:
      		hs_max_dtr = 26000000;
      		break;
         }
         #if (defined(CC_EMMC_HS200))
         if(hs_max_dtr == 200000000)     
         {
              MsdcSetDataMode(BASIC_DMA_DATA_MODE);
              MsdcSetSpeedMode(2);
              MsdcSetBusClock(0);//hw issue:should set different clk src first
              MsdcSetBusClock(200);
              MsdcSetDataBusWidth(8);
         }
         #else
         if((hs_max_dtr == 52000000) || (hs_max_dtr == 200000000))     
         {
             MsdcSetDataMode(BASIC_DMA_DATA_MODE);
             //MsdcSetDataMode(PIO_DATA_MODE);
             MsdcSetSpeedMode(1);
             MsdcSetBusClock(0);//hw issue:should set different clk src first
             MsdcSetBusClock(52);
             MsdcSetDataBusWidth(8);
         } 
         #endif //#if (defined(CC_EMMC_HS200))
         #endif //#if (defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890))
#else
        MSDC_PinMux(1);
	
        do
        {
            i4Ret = MsdcSysInit();
            if (i4RetryCnt++ > MSDC_SYS_INIT_RETRY_LIMIT)
            {
                break;
            }		
        } while (i4Ret != MSDC_SUCCESS);

        if (i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "eMMC2DRAM SysInit Failed!\n");
            return i4Ret;
        }     
                
        i4Ret = MsdcIdentyCard (TRUE);
        if(i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "eMMC2DRAM Identy Card Failed!\n");
            return i4Ret;
        }
#endif
        fgInited = 1;
    }

    if(u4Type == 0)
    {
        return MsdcReadCard(u8Addr, pSrcOrDes, u4Length);
    }
    else
    {
        return MsdcWriteCard(pSrcOrDes, u8Addr, u4Length);    
    }

}

/* 
 * detect the gpio level of SD module,
 * which indicates SD-like card insert or not.
 * return value:
 * 0: low voltage - insert
 * 1: high voltage - un-insert
 */
INT32 MSDCDetectSDGPIO(VOID)
{
    return 0;
    //return GPIO_GetIn(MSDC_SD_DETECT_PIN);
}

/* 
 * control the power voltage of SD module,
 * which is decided by logical level output of GPIO63:
 * 0: high voltage - 3.3V
 * 1: low voltage - 1.8V
 */
INT32 MSDCSDSwitchVoltage(INT32 hV)
{
    return MsdcSDPowerSwitch(hV);
}

INT32 MSDCSDInit(BOOL fgForce)
{
    INT32 i4Ret = MSDC_SUCCESS;
    UINT32 i4RetryCnt = 0, tmpch = 0;
    static UINT32 fgInited = 0;

    tmpch = ch;
    if((fgForce) || (!fgInited))
    {
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
        MSDCSDSwitchVoltage(0);
#endif
		
        //MSDC_PinMux(0);
		MSDC_PinMux(EmmcChannel);
        x_memset(&sdCard[ch], 0, sizeof(sdcard_t));
		
        do
        {
            i4Ret = MsdcSysInit();
            if (i4RetryCnt++ > MSDC_SYS_INIT_RETRY_LIMIT)
            {
                break;
            }		
        } while (i4Ret != MSDC_SUCCESS);
	
        if (i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "SD SysInit Failed!\n");
            goto funcEnd;
        }
        
        i4Ret = MsdcIdentyCard (TRUE);
        if(i4Ret != MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_ERR, "SD Identy Card Failed!\n");
           goto funcEnd;
        }
        
        fgInited = 1;
    }

funcEnd:
    ch = tmpch;
    return i4Ret;
}

INT32 MSDCGetSDInsert(BOOL fgForce)
{
    INT32 i4Ret;

    // if no any card inserted, return directly
    i4Ret = MSDCDetectSDGPIO();
    if(i4Ret != 0)
    {
        return MSDC_SD_NONE;
    }

    i4Ret = MSDCSDInit(fgForce);
    if(i4Ret != MSDC_SUCCESS)
    {
        return MSDC_SD_UNKNOWN;    
    }

    return MSDC_SD_INSERT;
    
}

INT32 MSDCGetSDDevInfo(MSDC_DevInfo_T *MsdcDevInfo)
{
    INT32 i4Ret;
    UINT32 tmpch = 0;

    tmpch = ch;
    ch = EmmcChannel;
    i4Ret = MsdcGetDevInfo(MsdcDevInfo);
    ch = tmpch;

    return i4Ret;
}

INT32 SD2DRAM(UINT64 u8Addr, UINT32 *pSrcOrDes, UINT32 u4Length, UINT32 u4Type)
{
    INT32 i4Ret;
    UINT32 tmpch = 0;
    #if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)
	#ifdef CC_MTK_LOADER
	UINT32 ui4SDMMCUpgradeSupport;
	#endif
    #endif

    tmpch = ch;
    ch = EmmcChannel;
    #if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)
	#ifdef CC_MTK_LOADER
	ui4SDMMCUpgradeSupport = (UINT32)(DRVCUST_InitGet64(eSDCARDUpgradeSupport));
	if(ui4SDMMCUpgradeSupport)
	{
    if(1==ch)
    {
        u8Addr+=(UINT64)(DRVCUST_InitGet64(eSDCARDUpgradeOffset));
        //MSDC_LOG(MSG_LVL_ERR, "u8Addr %08X %08X .\n",(UINT32)(u8Addr>>32),(UINT32)(u8Addr));
    }
	}
	#endif
    #endif
    if(u4Type == 0)
    {
        i4Ret = MsdcReadCard(u8Addr, pSrcOrDes, u4Length);
    }
    else
    {
        i4Ret = MsdcWriteCard(pSrcOrDes, u8Addr, u4Length);	  
    }

    ch = tmpch;
    
    return i4Ret;
}

#ifdef CC_SUPPORT_STR
//-----------------------------------------------------------------------------
/** MSDC_pm_str_suspend
 */
//-----------------------------------------------------------------------------
void MSDC_pm_str_suspend(void)
{
    //gate hclk & sclk
    MSDC_SETBIT(MSDC_CLK_S_REG0, MSDC_CLK_GATE_BIT);	
}


//-----------------------------------------------------------------------------
/** NOR_pm_str_resume
 */
//-----------------------------------------------------------------------------
void MSDC_pm_str_resume(void)
{
    //flash_ck clock is normal clock
     //gate hclk & sclk
    MSDC_CLRBIT(MSDC_CLK_S_REG0, MSDC_CLK_GATE_BIT);	
}
#endif // CC_SUPPORT_STR

#endif
