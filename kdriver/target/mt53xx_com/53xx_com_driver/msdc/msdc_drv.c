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
* [File]			msdc_drv.c
* [Version]			v1.0
* [Revision Date]	2011-05-04
* [Author]			Shunli Wang, shunli.wang@mediatek.inc, 82896, 2011-05-04
* [Description]
*	MSDC Driver Source File
* [Copyright]
*	Copyright (C) 2011 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/

#include "x_typedef.h"
#include "x_printf.h"
#include "x_hal_arm.h"
#include "x_assert.h"
#include "x_timer.h"
#include "x_os.h"
#include "x_bim.h"

#include "msdc_debug.h"
#include "msdc_host_hw.h"
#include "msdc_slave_hw.h"
#include "msdc_drv.h"
#if defined(CC_MTD_ENCRYPT_SUPPORT)
#include "msdc_ext.h"
#endif

// For SD gpio detection pin
#include "x_gpio.h"

#if defined(CC_MSDC_ENABLE) || defined(CC_SDMMC_SUPPORT)
//---------------------------------------------------------------------------
// Static/Global variables
//---------------------------------------------------------------------------
volatile static UINT32 _u4MsdcAccuVect = 0;
msdc_gpd_t DMA_MSDC_Header, DMA_MSDC_End;
msdc_bd_t DMA_BD[MAX_BD_PER_GPD];
UINT32 BDNum = 0;

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------
UINT32 ch = 0;
sdcard_t sdCard[MSDC_CH_NUM];
sdhost_t sdHost[MSDC_CH_NUM];
msdc_env_t msdcenv;



// new definition
/* target clock
*  clock source value
*  clock mode value
*  clock divide value
*/
UINT32 msdcClk[][MSDC_CLK_IDX_MAX] = {{MSDC_CLK_TARGET},
                                      {MSDC_CLK_SRC_VAL},
                                      {MSDC_CLK_MODE_VAL},
                                      {MSDC_CLK_DIV_VAL},
                                      {MSDC_CLK_DRV_VAL}};

/* the global variable related to sample edge */
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
UINT32 devIndex = 0; 
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
const EMMC_FLASH_DEV_T _arEMMC_DevInfo[] =
{
  // Device name                     ID1         ID2     DS26Sample  DS26Delay   HS52Sample  HS52Delay   DDR52Sample DDR52Delay  HS200Sample HS200Delay  //HS200Delay ,0x:WRDAT_CRCS_TA_CNTR,CKGEN_MSDC_DLY_SEL,PAD_DAT_RD_RXDLY,PAD_DAT_WR_RXDLY
  { "UNKNOWN",                   0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x0000000A, 0x00000002, 0x00060F0F},
  { "H26M21001ECR",              0x4A48594E, 0x49582056, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "H26M31003FPR",              0x4A205849, 0x4E594812, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "H26M31001HPR",              0x4A483447, 0x32611101, 0x00000000, 0x00000F0F, 0x00000006, 0x00000F0F, 0x00000002, 0x03000d0d, 0x00000002, 0x03060d0f},
  { "H26M52103FMR",              0x4A484147, 0x32650502, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000002, 0x00040F0F, 0x00000100, 0x04150917},
  { "H26M41103HPR",              0x4A483847, 0x31650502, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000002, 0x00000F0F, 0x00000000, 0x030a080f},
  { "SDIN5D2-4G",                0x0053454D, 0x30344790, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000102, 0x0000030A},
  { "KLM2G1HE3F-B001",           0x004D3247, 0x31484602, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "KLMAG2GEAC-B031000",        0x004D4147, 0x3247430B, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000002, 0x00000F0F, 0x00000002, 0x0308080C},
  { "KLM8G1GEAC-B031xxx",        0x004D3847, 0x3147430B, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000002, 0x00000F0F, 0x00000002, 0x0309090C},
  { "THGBM3G4D1FBAIG",           0x00303032, 0x47303000, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "THGBM4G4D1FBAIG",           0x00303032, 0x47343900, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "THGBM4G5D1HBAIR",           0x00303034, 0x47343900, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "THGBM4G6D2HBAIR",           0x00303038, 0x47344200, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "THGBMBG5D1KBAIL",           0x00303034, 0x47453000, 0x00000000, 0x00000000, 0x00000006, 0x00000F0F, 0x00000102, 0x0000130F, 0x00000002, 0x0306090A},
  { "THGBMBG5D1KBAIT",           0x00303034, 0x47453000, 0x00000000, 0x00000000, 0x00000006, 0x00000F0F, 0x00000102, 0x0000130F, 0x00000002, 0x0306090A},
  { "THGBMBG6D1KBAIL-XXX",       0x00303038, 0x47453000, 0x00000000, 0x00000000, 0x00000006, 0x00000F0F, 0x00000102, 0x0000130F, 0x00000100, 0x0309090C},
  { "MTFC4GMVEA-1M WT(JW857)",   0x4E4D4D43, 0x3034473A, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x0000000A, 0x00000006, 0x00000000},
  { "MTFC4GACAEAM-1M WT",        0x4E52314A, 0x35344110, 0x00000000, 0x00000000, 0x00000006, 0x00000F0F, 0x00000102, 0x0000130F, 0x00000002, 0x02060808},//JWA80
  { "MTFC4GACAAAM-1M WT",	       0x4E503158, 0x58585812, 0x00000000, 0x00000F0F, 0x00000006, 0x00000F0F, 0x00000002, 0x03000d0d, 0x00000002, 0x03080808},
  { "MTFC8GACAAAM-1M WT",        0x4E503158, 0x58585812, 0x00000000, 0x00000F0F, 0x00000006, 0x00000F0F, 0x00000002, 0x03000d0d, 0x00000002, 0x03080808},// JWA61
  { "THGBMAG6A2JBAIR",           0x00303038, 0x47393251, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x0000130F, 0x00000002, 0x0309080C},
  { "MTFC8GACAEAM-1M WT",        0x4E52314A, 0x35354110, 0x00000000, 0x00000000, 0x00000006, 0x00000F0F, 0x00000102, 0x0000130F, 0x00000000, 0x030B090C},
};
#else
const EMMC_FLASH_DEV_T _arEMMC_DevInfo[] =
{
  // Device name                     ID1         ID2     DS26Sample  DS26Delay   HS52Sample  HS52Delay   DDR52Sample DDR52Delay  HS200Sample HS200Delay  //HS200Delay ,0x:WRDAT_CRCS_TA_CNTR,CKGEN_MSDC_DLY_SEL,PAD_DAT_RD_RXDLY,PAD_DAT_WR_RXDLY
  { "UNKNOWN",                   0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x0000000A, 0x00000102, 0x0000030A},
  { "H26M21001ECR",              0x4A48594E, 0x49582056, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "H26M31003FPR",              0x4A205849, 0x4E594812, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "H26M31001HPR",              0x4A483447, 0x32611101, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000002, 0x00000F0F, 0x00000002, 0x02090D08},
  { "H26M52103FMR",              0x4A484147, 0x32650502, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000002, 0x00040F0F, 0x00000100, 0x04150917},
  { "H26M41103HPR",              0x4A483847, 0x31650502, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000002, 0x00000F0F, 0x00000000, 0x030a080f},
  { "SDIN5D2-4G",                0x0053454D, 0x30344790, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000102, 0x0000030A},
  { "KLM2G1HE3F-B001",           0x004D3247, 0x31484602, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "KLM8G1GEND-B031",           0x0038474E, 0x44335201, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000002, 0x03000d0d, 0x00000002, 0x0309080c},
  { "KLMAG2GEND-B031",           0x0041474E, 0x44335201, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000002, 0x03000d0d, 0x00000002, 0x0309080c},
  { "KLMAG2GEAC-B031000",        0x004D4147, 0x3247430B, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000002, 0x00000F0F, 0x00000002, 0x0308080C},
  { "KLM8G1GEAC-B031xxx",        0x004D3847, 0x3147430B, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000002, 0x00000F0F, 0x00000002, 0x0309090C},
  { "THGBM3G4D1FBAIG",           0x00303032, 0x47303000, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "THGBM4G4D1FBAIG",           0x00303032, 0x47343900, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "THGBM4G5D1HBAIR",           0x00303034, 0x47343900, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "THGBM4G6D2HBAIR",           0x00303038, 0x47344200, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000006, 0x00000000},
  { "THGBMBG5D1KBAIL",           0x00303034, 0x47453000, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x00000000, 0x00000002, 0x04060315},
  { "THGBMBG6D1KBAIL-XXX",       0x00303038, 0x47453000, 0x00000000, 0x00000000, 0x00000006, 0x00000F0F, 0x00000102, 0x0000130F, 0x00000100, 0x0309090C},
  { "MTFC4GMVEA-1M WT(JW857)",   0x4E4D4D43, 0x3034473A, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x0000000A, 0x00000006, 0x00000000},
  { "MTFC8GACAAAM-1M WT",        0x4E503158, 0x58585812, 0x00000100, 0x00000F0F, 0x00000106, 0x0105150F, 0x00000002, 0x03000d0d, 0x00000002, 0x03020815},// JWA61
  { "THGBMAG6A2JBAIR",           0x00303038, 0x47393251, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000102, 0x0000130F, 0x00000002, 0x0309080C},
  { "MTFC8GACAEAM-1M WT",        0x4E52314A, 0x35354110, 0x00000000, 0x00000000, 0x00000006, 0x00000F0F, 0x00000102, 0x0000130F, 0x00000000, 0x030B090C},
};
#endif

#else
const EMMC_FLASH_DEV_T _arEMMC_DevInfo1[] =
{
    // Device name              ID1         ID2      Sample
    { "UNKNOWN",            0x00000000, 0x00000000, 0x00110000},
    { "H26M21001ECR",       0x4A48594E, 0x49582056, 0x00101100},
    { "H26M31003FPR",       0x4A205849, 0x4E594812, 0x00110000},
    { "SDIN5D2-4G",         0x0053454D, 0x30344790, 0x00101100},
    { "KLM2G1HE3F-B001",    0x004D3247, 0x31484602, 0x00000000},
    { "KLM4G1FEAC-B031",    0x00344645, 0x41434200, 0x00110000},
    { "THGBM3G4D1FBAIG",    0x00303032, 0x47303000, 0x00000100},
    { "THGBM4G4D1FBAIG",    0x00303032, 0x47343900, 0x00110100},
    { "THGBM4G5D1HBAIR",    0x00303034, 0x47343900, 0x00110100},
    { "THGBM4G6D2HBAIR",    0x00303038, 0x47344200, 0x00110100},
    { "THGBMAG5A1JBAIR",    0x00303034, 0x47393051, 0x00110000},
    { "MTFC4GMCDM-1M WT",   0x4E503158, 0x58585810, 0x00110000},
    { "2FA18-JW812",        0x4E4D4D43, 0x30344734, 0x00110000},
};
#endif

//---------------------------------------------------------------------------
// MSDC related function
//---------------------------------------------------------------------------

INT32 MsdcSDPowerSwitch(INT32 i4V)
{
    if(i4V != GPIO_SetOut(MSDC_SD_VOLTAGE_PIN, i4V))
    {
        MSDC_LOG(MSG_LVL_INFO, "switch SD power(GPIO %d) failed!\n", MSDC_SD_VOLTAGE_PIN);
		return MSDC_FAILED;
	}

	return MSDC_SUCCESS;
}

#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882) 
INT32 MsdcFindHynixName(void)
{
	MSDC_LOG(MSG_LVL_ERR, " emmc name %s!\n", _arEMMC_DevInfo[devIndex].acName);
	
	if((_arEMMC_DevInfo[devIndex].u4ID1 ==  0x4A484147)&&( _arEMMC_DevInfo[devIndex].u4ID2==0x32650502))
	{
		return MSDC_SUCCESS;
	}
	else
	{
		return MSDC_FAILED;
	}
}
#endif
void MsdcFindDevName(UINT32 *pCID)
{
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882) 
    UINT32 i = 0, devNum = 0;
    UINT32 buff;

    /*
      * why we need to define the id mask of emmc
      * Some vendors' emmc has the same brand & type but different product revision.
      * That means the firmware in eMMC has different revision
      * We should treat these emmcs as same type and timing
      * So id mask can ignore this case
      */
    UINT32 idMask = 0xFFFFFF00;
	buff  = pCID[1];
	pCID[0] = pCID[3];
	pCID[1] = pCID[2];
	pCID[2] = buff;
    devNum = sizeof(_arEMMC_DevInfo)/sizeof(EMMC_FLASH_DEV_T);
    MSDC_LOG(MSG_LVL_ERR, "%08X:%08X:%08X:%08X\n", pCID[0], pCID[1], pCID[2], pCID[3]);
    MSDC_LOG(MSG_LVL_ERR, "id1:%08X id2:%08X\n", ID1(pCID), ID2(pCID));

    for(i = 0; i<devNum; i++)
    {
        if((_arEMMC_DevInfo[i].u4ID1 == ID1(pCID)) &&
           ((_arEMMC_DevInfo[i].u4ID2 & idMask) == (ID2(pCID) & idMask)))
        {
            break;
        }
    }

    devIndex = (i == devNum)?0:i;

    MSDC_LOG(MSG_LVL_ERR, "eMMC Name: %s\n", _arEMMC_DevInfo[devIndex].acName);	
#endif
}


#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882) 
static INT32 MsdcHostVoltageSwitch(VOID)
{	
    // wait SDC_STA.SDCBUSY = 0
    while(MSDC_READ32(SDC_STS)&SDC_STS_SDCBUSY);

	// Disable pull-up resistors
	MSDC_CLRBIT(SD30_PAD_CTL1, (0x1<<17));
	MSDC_CLRBIT(SD30_PAD_CTL2, (0x1<<17));

    // make sure CMD/DATA lines are pulled to 0
	while((MSDC_READ32(MSDC_PS)&(0x1FF<<16)));

	// Program the PMU of host from 3.3V to 1.8V
	if(MSDC_SUCCESS != MsdcSDPowerSwitch(1))
    {
		return MSDC_FAILED;
	}

	// Delay at least 5ms
	HAL_Delay_us(6000);

	// Disable pull-up resistors
	MSDC_SETBIT(SD30_PAD_CTL1, (0x1<<17));
	MSDC_SETBIT(SD30_PAD_CTL2, (0x1<<17));

	// start 1.8V voltage detecting
	MSDC_SETBIT(MSDC_CFG, MSDC_CFG_BUS_VOL_START);

	// Delay at most 1ms
	HAL_Delay_us(1000);

	// check MSDC.VOL_18V_START_DET is 0
	while(MSDC_READ32(MSDC_CFG)&MSDC_CFG_BUS_VOL_START);

	// check MSDC_CFG.VOL_18V_PASS
	if(MSDC_READ32(MSDC_CFG)&MSDC_CFG_BUS_VOL_PASS)
    {
        MSDC_LOG(MSG_LVL_INFO, "switch 1.8v voltage success!\n");  
		return MSDC_SUCCESS;
	}
	else
	{
        MSDC_LOG(MSG_LVL_INFO, "switch 1.8v voltage faled!\n"); 
		return MSDC_FAILED;
	}
}
#endif

static VOID MsdcShowChipInfo(UINT32 *pCID)
{	
    MSDC_LOG(MSG_LVL_INFO, "\nCID:%08X:%08X:%08X:%08X", pCID[0], pCID[1], pCID[2], pCID[3]);
    
    if(sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {
		    MSDC_LOG(MSG_LVL_INFO, "\nMID:%02X", pCID[3]>>24);
		    MSDC_LOG(MSG_LVL_INFO, "\nDevice Type: ");
		    if(((pCID[3]>>16)&0x3) == 0x00)
		    {
		        MSDC_LOG(MSG_LVL_INFO, "Card(removable)");
		    }
		    else if(((pCID[3]>>16)&0x3) == 0x01)
		    {
		        MSDC_LOG(MSG_LVL_INFO, "BGA(Discret embedded)");
		    }
		    else if(((pCID[3]>>16)&0x3) == 0x02)
		    {
		        MSDC_LOG(MSG_LVL_INFO, "POP");
		    }
		    else
		    {
		        MSDC_LOG(MSG_LVL_INFO, "Unknown");
		    }
		    MSDC_LOG(MSG_LVL_INFO, "\nOID:%02X", pCID[3]>>8);
		    MSDC_LOG(MSG_LVL_INFO, "\nPNM: %c%c%c%c%c%c", pCID[3]&0xFF, (pCID[2]>>24)&0xFF, (pCID[2]>>16)&0xFF,  
				                             (pCID[2]>>8)&0xFF, pCID[2]&0xFF, pCID[1]>>24);
		    MSDC_LOG(MSG_LVL_INFO, "\nPRV:%02X", (pCID[1]>>16)&0xFF);
		    MSDC_LOG(MSG_LVL_INFO, "\nPSN:%08X", (pCID[0]>>16)|(pCID[1]<<16));
		    MSDC_LOG(MSG_LVL_INFO, "\nMDT:%02X\n", pCID[0]>>8);
    }
    else if(sdCard[ch].cardType == CARDTYPE_MEM_SD)
    {
        MSDC_LOG(MSG_LVL_INFO, "\nMID:%02X", pCID[3]>>24);
        MSDC_LOG(MSG_LVL_INFO, "\nOID:%c%c", (pCID[3]>>16)&0xFF, (pCID[3]>>8)&0xFF);
        MSDC_LOG(MSG_LVL_INFO, "\nPNM: %c%c%c%c%c", pCID[3]&0xFF, (pCID[2]>>24)&0xFF, (pCID[2]>>16)&0xFF,  
    			                             (pCID[2]>>8)&0xFF, pCID[2]&0xFF);
        MSDC_LOG(MSG_LVL_INFO, "\nPRV:%01X.%01X", ((pCID[1]>>24)&0xFF)>>4, (pCID[1]>>24)&0x0F);
        MSDC_LOG(MSG_LVL_INFO, "\nPSN:%08X", (pCID[1]<<8)|(pCID[0]>>24));
        MSDC_LOG(MSG_LVL_INFO, "\nMDT:%02X\n", (pCID[0]>>8)&&0xFFF);    	
    }
    else
    {
        MSDC_LOG(MSG_LVL_ERR, "\nNot Support Card Type!\n");	
    }
    
}

VOID MsdcDumpRegister(VOID)
{
    UINT32 i = 0;	
   
    for(; i < 0x104; i+= 4)
    {
    	 if(i%0x10 == 0)
    	     Printf("\n0X%08X | ", i);
    	     
       Printf("%08X ", MSDC_READ32(MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + i));     	
    }
 
     if (ch == 1)
     {
        Printf("\nClock Src Setting - 0x%08X: 0x%08X\n", MSDC_CLK_S_REG1, MSDC_READ32(MSDC_CLK_S_REG1));
     }
     else
     {
        Printf("\nClock Src Setting - 0x%08X: 0x%08X\n", MSDC_CLK_S_REG0, MSDC_READ32(MSDC_CLK_S_REG0));
     }
    Printf("Pinmux Setting - 0xF000D604: 0x%08X\n", MSDC_READ32(0xF000D604));
    
    for(i = 0;i <= 25; i++)
    {
        MSDC_WRITE32(DBG_SEL, i);
        Printf("DBG_SEL:%d - %08X\n", i, MSDC_READ32(DBG_OUT));	
    }
    
    while(1);
}

UINT8 MsdcCheckSumCal(UINT8 *buf, UINT32 len)
{
    UINT32 i = 0, sum = 0;

    for(; i<len;i++)
    {
        sum += *(buf + i);
    }

    return (0xFF-(UINT8)sum);
}

VOID MsdcDescriptorConfig(VOID *pBuff, UINT32 u4BufLen)
{
    UINT32 i, tmpBDNum, tmpBuflen;
    UINT8 *tmppBuff = (UINT8 *)pBuff;

	UNUSED(tmpBDNum);

    if(u4BufLen == 0)
        return;

    BDNum = u4BufLen / BD_MAX_LEN + ((u4BufLen % BD_MAX_LEN == 0)?0:1);
    tmpBDNum = BDNum;
    tmpBuflen = u4BufLen;

    MSDC_LOG(MSG_LVL_INFO, "\n----BD NUM: %d----\n", BDNum);

    if(BDNum > MAX_BD_NUM)
    {
        MSDC_LOG(MSG_LVL_INFO, "BD Number exceeds MAX value(%d)\n", MAX_BD_NUM);
        return;
    }

    // Initial the structures
    x_memset(&DMA_MSDC_Header, 0x0, sizeof(msdc_gpd_t));
    x_memset(&DMA_MSDC_End, 0x0, sizeof(msdc_gpd_t));
    x_memset(DMA_BD, 0x0, sizeof(msdc_bd_t)*MAX_BD_NUM);

    // Configure the BD structure array
    for(i = 0;i<BDNum;i++)
    {
        if(i != BDNum - 1)
        {
            DMA_BD[i].pNext = (VOID *)(&DMA_BD[i+1]);
        }
        else
            DMA_BD[i].eol = 1;
	
        DMA_BD[i].pBuff = (VOID *)(tmppBuff);
        DMA_BD[i].buffLen = (tmpBuflen>BD_MAX_LEN)?(BD_MAX_LEN):(tmpBuflen);
        tmpBuflen -= DMA_BD[i].buffLen;
        tmppBuff += DMA_BD[i].buffLen;

        DMA_BD[i].chksum = 0;
        DMA_BD[i].chksum = MsdcCheckSumCal((UINT8 *)(DMA_BD+i), 16);
    }

    // Configure the GPD Header structure
    DMA_MSDC_Header.hwo = 1;  /* hw will clear it */
    DMA_MSDC_Header.bdp = 1;	 
    DMA_MSDC_Header.pNext = (VOID *)(&DMA_MSDC_End);
    DMA_MSDC_Header.pBuff = (VOID *)(DMA_BD);
    DMA_MSDC_Header.chksum = 0;  /* need to clear first. */  
    DMA_MSDC_Header.chksum = MsdcCheckSumCal((UINT8 *)(&DMA_MSDC_Header), 16);

    HalFlushInvalidateDCacheMultipleLine((UINT32)(&DMA_MSDC_Header), sizeof(msdc_gpd_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)(&DMA_MSDC_End), sizeof(msdc_gpd_t));
    HalFlushInvalidateDCacheMultipleLine((UINT32)(DMA_BD), sizeof(msdc_bd_t)*BDNum);
    //HalDmaBufferStart((VOID *)(&DMA_MSDC_Header), sizeof(msdc_gpd_t), HAL_DMA_TO_DEVICE);
    //HalDmaBufferStart((VOID *)(DMA_BD), sizeof(msdc_bd_t)*BDNum, HAL_DMA_TO_DEVICE);    

    // Configure the DMA HW registers
    MSDC_SETBIT(DMA_CFG, DMA_CFG_CHKSUM);
    MSDC_WRITE32(DMA_SA, 0x0);
    MSDC_SETBIT(DMA_CTRL, DMA_CTRL_BST_64);
    MSDC_SETBIT(DMA_CTRL, DMA_CTRL_DESC_MODE);

    MSDC_WRITE32(DMA_SA, (UINT32)(&DMA_MSDC_Header));

}

/* delay value setting for hs data sample
*/
void MsdcSmapleDelay(UINT32 flag)
{
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    // Sample Edge init
    MSDC_CLRBIT(PAD_TUNE, 0xFFFFFFFF);

    // Sample Edge init
    if(sdHost[ch].speedMode == SPEED_MODE_DS)
    {
        MSDC_SETBIT(PAD_TUNE, _arEMMC_DevInfo[devIndex].DS26Delay);
    }
    else if(sdHost[ch].speedMode == SPEED_MODE_HS)
    {
        MSDC_SETBIT(PAD_TUNE, _arEMMC_DevInfo[devIndex].HS52Delay);
    }
    else if(sdHost[ch].speedMode == SPEED_MODE_DDR50)
    {
        MSDC_SETBIT(PAD_TUNE, _arEMMC_DevInfo[devIndex].DDR52Delay);
    }
    else if(sdHost[ch].speedMode == SPEED_MODE_HS200)
    {
        MSDC_SETBIT(PAD_TUNE, _arEMMC_DevInfo[devIndex].HS200Delay);

        MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
        MSDC_SETBIT(PATCH_BIT0, 0x3 << CKGEN_MSDC_DLY_SEL_SHIFT);       
#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    	MSDC_CLRBIT(PAD_TUNE, 0xFFFFFFFF);
        MSDC_SETBIT(PAD_TUNE, _arEMMC_DevInfo[devIndex].HS200Delay&0x0000ffff);
        
        MSDC_CLRBIT(PAD_TUNE, PAD_CLK_TXDLY);
        MSDC_SETBIT(PAD_TUNE, 0x8<<PAD_CLK_TXDLY_SHIFT);
        
        MSDC_CLRBIT(PATCH_BIT1, 0x7);
        MSDC_SETBIT(PATCH_BIT1, (_arEMMC_DevInfo[devIndex].HS200Delay&0xff000000)>>24);
        
        MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
        MSDC_SETBIT(PATCH_BIT0, ((_arEMMC_DevInfo[devIndex].HS200Delay&0x00ff0000)>>16)<< CKGEN_MSDC_DLY_SEL_SHIFT);
#endif
    }
	else if(sdHost[ch].speedMode == SPEED_MODE_SDR104)
    {
        MSDC_SETBIT(PAD_TUNE, _arEMMC_DevInfo[devIndex].HS200Delay);

        MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
        MSDC_SETBIT(PATCH_BIT0, 0x3 << CKGEN_MSDC_DLY_SEL_SHIFT);       
#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    	MSDC_CLRBIT(PAD_TUNE, 0xFFFFFFFF);
        MSDC_SETBIT(PAD_TUNE, _arEMMC_DevInfo[devIndex].HS200Delay&0x0000ffff);
        
        MSDC_CLRBIT(PAD_TUNE, PAD_CLK_TXDLY);
        MSDC_SETBIT(PAD_TUNE, 0x8<<PAD_CLK_TXDLY_SHIFT);
        
        MSDC_CLRBIT(PATCH_BIT1, 0x7);
        MSDC_SETBIT(PATCH_BIT1, (_arEMMC_DevInfo[devIndex].HS200Delay&0xff000000)>>24);
        
        MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
        MSDC_SETBIT(PATCH_BIT0, ((_arEMMC_DevInfo[devIndex].HS200Delay&0x00ff0000)>>16)<< CKGEN_MSDC_DLY_SEL_SHIFT);
#endif
	}
    
#endif
}

/* when I test TF card about DDR mode,  I find the truth:
* The write operation in DDR mode needs write status falling edge sample + 0xA delay for CMD/DAT line, 
* but rising edge sample should be set for the read operation in DDR mode
*/
void MsdcSampleEdge(UINT32 flag)
{
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    MSDC_CLRBIT(MSDC_IOCON, 0xFFFFFF);
    
    // Sample Edge init
    if(sdHost[ch].speedMode == SPEED_MODE_DS)
    {
        MSDC_SETBIT(MSDC_IOCON, _arEMMC_DevInfo[devIndex].DS26Sample & 0xFFFFFF);
    }
    else if(sdHost[ch].speedMode == SPEED_MODE_HS)
    {
        MSDC_SETBIT(MSDC_IOCON, _arEMMC_DevInfo[devIndex].HS52Sample & 0xFFFFFF);
    }
    else if(sdHost[ch].speedMode == SPEED_MODE_DDR50)
    {
        MSDC_SETBIT(MSDC_IOCON, _arEMMC_DevInfo[devIndex].DDR52Sample & 0xFFFFFF);
        MSDC_WRITE32(MSDC_CLK_H_REG1, 0x2);
    }
    else if(sdHost[ch].speedMode == SPEED_MODE_HS200)
    {
        MSDC_SETBIT(MSDC_IOCON, _arEMMC_DevInfo[devIndex].HS200Sample & 0xFFFFFF);
        MSDC_WRITE32(MSDC_CLK_H_REG1, 0x2);
    }
    else if(sdHost[ch].speedMode == SPEED_MODE_SDR104)
    {
        MSDC_SETBIT(MSDC_IOCON, _arEMMC_DevInfo[devIndex].HS200Sample & 0xFFFFFF);
        MSDC_WRITE32(MSDC_CLK_H_REG0, 0x2);
    }
#else
    // TBD    
#endif
}

/* different clock needs different max driving strength
*/
void MsdcDrivingStrength(UINT32 driving)
{
    // PAD CTRL
    /* The value is determined by the experience
      * So, a best proper value should be investigate further.
      *   - when I test TF card, which is connected to main board by a SDIO daughter board,
      *      it will happen crc error in 48MHz, so I enhance pad drving strenght again.
      */
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882) 
    MSDC_CLRBIT(SD30_PAD_CTL0, (0x7<<15) |(0x7<<4) | (0x7<<0));
    MSDC_CLRBIT(SD30_PAD_CTL1, (0x7<<15) |(0x7<<4) | (0x7<<0));
    MSDC_CLRBIT(SD30_PAD_CTL2, (0x7<<15) |(0x7<<4) | (0x7<<0));	

    MSDC_SETBIT(SD30_PAD_CTL0, (((0x4)<<15) |((driving>>3)&0x7)<<4) | ((driving&0x7)<<0));
    MSDC_SETBIT(SD30_PAD_CTL1, (((0x4)<<15) |((driving>>3)&0x7)<<4) | ((driving&0x7)<<0));
    MSDC_SETBIT(SD30_PAD_CTL2, (((0x4)<<15) |((driving>>3)&0x7)<<4) | ((driving&0x7)<<0));	
#endif

#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)
    MSDC_CLRBIT(SD30_PAD_CTL0, (0x7<<15) | (0x7<<0));
    MSDC_CLRBIT(SD30_PAD_CTL1, (0x7<<15) | (0x7<<0));
    MSDC_CLRBIT(SD30_PAD_CTL2, (0x7<<15) | (0x7<<0));	

    if(IS_IC_5861_ES1())    
    {
    	MSDC_SETBIT(SD30_PAD_CTL0, ((0x4)<<15) | ((2&0x7)<<0));//CLK pull up 10k
    	MSDC_SETBIT(SD30_PAD_CTL1, ((0x4)<<15) | ((2&0x7)<<0));//CMD pull up 10k
    	MSDC_SETBIT(SD30_PAD_CTL2, ((0x4)<<15) | ((2&0x7)<<0));//DATA pull up 10k
    	
    	MSDC_SETBIT(MSDC_IOCON, (0x1<<8));
    }
    else
    {
   		MSDC_SETBIT(SD30_PAD_CTL0, ((0x4)<<15) | ((driving&0x7)<<0));//CLK pull up 10k
    	MSDC_SETBIT(SD30_PAD_CTL1, ((0x4)<<15) | ((driving&0x7)<<0));//CMD pull up 10k
    	MSDC_SETBIT(SD30_PAD_CTL2, ((0x4)<<15) | ((driving&0x7)<<0));//DATA pull up 10k
    	
    	MSDC_CLRBIT(MSDC_IOCON, (0x1<<8));
    	MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
        MSDC_SETBIT(PATCH_BIT0, 0x1<< CKGEN_MSDC_DLY_SEL_SHIFT);
    }
#endif    
}

INT32 MsdcReset(VOID)
{
    UINT32 i;
    
    // Reset MSDC
    MSDC_SETBIT(MSDC_CFG, MSDC_CFG_RST);

    for (i = 0; i < MSDC_RST_TIMEOUT_LIMIT_COUNT; i++)
    {
        if (0 == (MSDC_READ32(MSDC_CFG) & MSDC_CFG_RST))
        {
            break;
        }
        
        HAL_Delay_us(1000);
    }
    
    if (i == MSDC_RST_TIMEOUT_LIMIT_COUNT)
    {
        return MSDC_FAILED;        
    }
	
    return MSDC_SUCCESS;
}

INT32 MsdcClrFifo(VOID)
{
    UINT32 i;
    
    // Reset FIFO
    MSDC_SETBIT(MSDC_FIFOCS, MSDC_FIFOCS_FIFOCLR);

    for (i = 0; i < MSDC_RST_TIMEOUT_LIMIT_COUNT * 1000; i++)
    {
        if (0 == (MSDC_READ32(MSDC_FIFOCS) & (MSDC_FIFOCS_FIFOCLR | MSDC_FIFOCS_TXFIFOCNT_MASK | MSDC_FIFOCS_RXFIFOCNT_MASK))) 
        {
            break;
        }
        
        HAL_Delay_us(1);
    }
    
    if (i == MSDC_FIFOCLR_TIMEOUT_LIMIT_COUNT)
    {
        return MSDC_FAILED;       
    }

    return MSDC_SUCCESS;
}

VOID MsdcChkFifo(sd_cmd_t *pCmd)
{
    // Check if rx/tx fifo is zero
    if ((MSDC_READ32(MSDC_FIFOCS) & (MSDC_FIFOCS_TXFIFOCNT_MASK | MSDC_FIFOCS_RXFIFOCNT_MASK)) != 0)
    {
        MSDC_LOG(MSG_LVL_WARN, "CMD%d : FiFo not 0, FIFOCS(0x%08X):0x%08X!!\n", pCmd->idx, MSDC_FIFOCS, MSDC_READ32(MSDC_FIFOCS));
        MsdcClrFifo();
    }
}

VOID MsdcClrIntr(VOID)
{
    // Check MSDC Interrupt vector register
    if (0 != MSDC_READ32(MSDC_INT))
    {
        MSDC_LOG(MSG_LVL_WARN, "MSDC INT(0x%08X) not 0:0x%08X!!\n", MSDC_INT, MSDC_READ32(MSDC_INT));

        // Clear MSDC Interrupt vector register
        MSDC_WRITE32(MSDC_INT, MSDC_READ32(MSDC_INT));
    }
}

INT32 MsdcWaitClkStable (VOID)
{
    UINT32 i;

    for (i = 0; i < MSDC_CLK_TIMEOUT_LIMIT_COUNT * 1000; i++)
    {
        if (0 != (MSDC_READ32(MSDC_CFG) & MSDC_CFG_CARD_CK_STABLE))
        {
            break;
        }
        
        HAL_Delay_us(1);
    }
    
    if (i == MSDC_CLK_TIMEOUT_LIMIT_COUNT)
    {
        MSDC_LOG(MSG_LVL_ERR, "WaitClkStable Failed!\n");
        return MSDC_FAILED;
    }
	
    return MSDC_SUCCESS;
}

INT32 MsdcInit (VOID) 
{
    // Reset MSDC
    MsdcReset();

    // Set PIO Mode & SD/MMC Mode 
    MSDC_SETBIT(MSDC_CFG, MSDC_CFG_SD);
    if(sdHost[ch].dataMode > PIO_DATA_MODE)
    {
        MSDC_CLRBIT(MSDC_CFG, MSDC_CFG_PIO_MODE);
    }
    else
    {
    	  MSDC_SETBIT(MSDC_CFG, MSDC_CFG_PIO_MODE);
    }

    // Disable sdio & Set bus to 1 bit mode
    MSDC_CLRBIT(SDC_CFG, SDC_CFG_SDIO | SDC_CFG_BW_MASK);

    // set clock mode (DIV mode)
    MSDC_CLRBIT(MSDC_CFG, (((UINT32)0x03) << 16));

    // Wait until clock is stable
    if (MSDC_FAILED == MsdcWaitClkStable())
    {
        return MSDC_FAILED;
    }
    
    // Set default RISC_SIZE for DWRD pio mode
    MSDC_WRITE32(MSDC_IOCON, (MSDC_READ32(MSDC_IOCON) & ~MSDC_IOCON_RISC_SIZE_MASK) | MSDC_IOCON_RISC_SIZE_DWRD);

    // Set Data timeout setting => Maximum setting
    MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~(((UINT32)0xFF) << SDC_CFG_DTOC_SHIFT)) | (((UINT32)0xFF) << SDC_CFG_DTOC_SHIFT));
    
    return MSDC_SUCCESS;
}

VOID MsdcInitHostConfig(VOID)
{
    x_memset(&sdHost[ch], 0, sizeof(sdhost_t));

    /* HOST Capacity Configuration
     * Traditionally, MSDC1 is designed for eMMC device
     * MSDC0 is designed for SD-like card.
     * But for some old Chip, for example, MT5396/MT5398/MT5860,
     * MSDC0 can also be specified for eMMC device.
     * In that case, there is no posibility to support SD-like card.
     */
    if(ch == 1)
    {
        sdHost[ch].hostCap |= HOST_CAP_MMC_SUPPORT;	
    }
    else
    {
    	if(1 == sdCard[ch].flagHost)
    	{
    	    sdHost[ch].hostCap |= HOST_CAP_MMC_SUPPORT;		
    	}
    	else
    	{
            sdHost[ch].hostCap |= HOST_CAP_SD_SUPPORT;
        }	  	
    }
    sdHost[ch].hostCap &= ~(HOST_CAP_SDIO_SUPPORT);
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    sdHost[ch].hostCap |= HOST_CAP_LOW_VOLTAGE_18V_SUPPORT;
#endif
    sdHost[ch].hostAttr &= ~(MSDC_DATA_PRE_DEF_MASK | MSDC_DATA_END_AUTO_MASK |
                             MSDC_DATA_DDR_MODE_MASK);
   
    sdHost[ch].accuVect = 0;
    sdHost[ch].blkLen = SDHC_BLK_SIZE;
    sdHost[ch].busWidth = 4;
    //sdHost[ch].busWidth = 1;
    sdHost[ch].speedMode = SPEED_MODE_DS;
    sdHost[ch].dataMode = PIO_DATA_MODE;
    //sdHost[ch].dataMode = BASIC_DMA_DATA_MODE;
    //sdHost[ch].dataMode = DESC_DMA_DATA_MODE;
    sdHost[ch].maxClock = 48;
    sdHost[ch].curClock = msdcClk[0][MSDC_NORM_CLK_IDX];
    sdHost[ch].dmaBstSize = DMA_BST_64;
    sdHost[ch].maxBasicDmaLen = BASIC_DMA_MAX_LEN;
    sdHost[ch].polling = 1;
    sdHost[ch].fgUpdateExtCsd = 0;
    sdHost[ch].fgValid = 0x484F5354;
    
    // only boot device setting will be stored
    if(1 == sdCard[ch].flagHost)
    {
        x_memset(&msdcenv, 0, sizeof(msdc_env_t));
    	  
        msdcenv.fgHost = (UINT8)ch;
        x_memcpy((VOID *)msdcenv.cid, (VOID *)(&sdCard[ch].cid), sizeof(sd_cid_t));
        x_memcpy((VOID *)msdcenv.csd, (VOID *)(&sdCard[ch].csd), sizeof(csd_20_t));
        x_memcpy((VOID *)msdcenv.rca, (VOID *)(&sdCard[ch].memRca), sizeof(UINT16));
        x_memcpy((VOID *)msdcenv.ocr, (VOID *)(&sdCard[ch].memOcr), sizeof(mem_ocr_t));
    }
}

VOID MsdcContinueClock (UINT32 i4ContinueClock)
{
    if (i4ContinueClock)
    {
       // Set clock continuous even if no command
       MSDC_SETBIT(MSDC_CFG, (((UINT32)0x01) << 1));
    }
    else
    {
       // Set clock power down if no command
       MSDC_CLRBIT(MSDC_CFG, (((UINT32)0x01) << 1));
    }
}

INT32 MsdcSetClkFreq(UINT32 clkFreq) 
{
    UINT32 idx = 0, ddr = 0;

    ddr = (sdHost[ch].hostAttr & MSDC_DATA_DDR_MODE_MASK)?1:0;
	do
    {
        if((clkFreq < msdcClk[0][idx]) ||
		   (ddr && (msdcClk[2][idx] != 2)))
		    continue;
		else
			break;
		
	}while(++idx < MSDC_CLK_IDX_MAX);

	// Enable msdc_src_clk gate
    if (ch == 1)
        MSDC_SETBIT(MSDC_CLK_S_REG1, MSDC_CLK_GATE_BIT);
    else
        MSDC_SETBIT(MSDC_CLK_S_REG0, MSDC_CLK_GATE_BIT);

    // Set clock source value
	if (ch == 1)
	{
        // Clr msdc_src_clk selection
        MSDC_CLRBIT(MSDC_CLK_S_REG1, MSDC_CLK_SEL_MASK);  
        MSDC_SETBIT(MSDC_CLK_S_REG1, msdcClk[1][idx]<<0);  
    }
    else
    {
        // Clr msdc_src_clk selection
        MSDC_CLRBIT(MSDC_CLK_S_REG0, MSDC_CLK_SEL_MASK);  
        MSDC_SETBIT(MSDC_CLK_S_REG0, msdcClk[1][idx]<<0);    
    }

	// Set clock mode value
	MSDC_CLRBIT(MSDC_CFG, (((UINT32)0x03) << 16));
    MSDC_SETBIT(MSDC_CFG, ((msdcClk[2][idx]) << 16));

    // Set clock divide value
    MSDC_CLRBIT(MSDC_CFG, (((UINT32)0xFF) << 8));
    MSDC_SETBIT(MSDC_CFG, ((msdcClk[3][idx]) << 8));	


    // Disable msdc_src_clk gate
    if (ch == 1)
        MSDC_CLRBIT(MSDC_CLK_S_REG1, MSDC_CLK_GATE_BIT);
    else
        MSDC_CLRBIT(MSDC_CLK_S_REG0, MSDC_CLK_GATE_BIT);

    
    // Wait until clock is stable
    if (MSDC_FAILED == MsdcWaitClkStable())
    {
        MSDC_LOG(MSG_LVL_ERR, "Set Bus Clock as %d(MHz) Failed!\n", msdcClk[0][idx]);
        return MSDC_FAILED;
    }
	
    MSDC_LOG(MSG_LVL_INFO, "Set Bus Clock as %d(MHz) Success!\n", msdcClk[0][idx]); 
    sdHost[ch].curClock= msdcClk[0][idx];
	
    MsdcSampleEdge(0);
	MsdcSmapleDelay(0);
    MsdcDrivingStrength(msdcClk[4][idx]);
  
    return MSDC_SUCCESS;
}

/* timeout count is determined by bus condition
* higher bus width and bus clock will need less timeout count
* but lower bus width and bus clock will need more timeout count
* for example, clock=350Khz and 1bit mode for 1MB data transfer will
* need 25s wait. So, if the timeout count is assigned to small will induce some timeout issue.
*/
INT32 MsdcWaitIntr (UINT32 vector, UINT32 timeoutCnt, UINT32 waitMode) 
{
    UINT32 i, retVector;
    
    // Clear Vector variable
    _u4MsdcAccuVect = 0;  
	retVector = INT_SD_DATTO | INT_SD_DATA_CRCERR;

    if(sdHost[ch].polling == 1)
    {
// Which one is better?
#if 0
        if(waitMode == 0)
        {
            for (i = 0;i < timeoutCnt; i++)
            {
                if (0 != (MSDC_READ32(MSDC_INT) & vector)) 
                {
                    _u4MsdcAccuVect |= MSDC_READ32(MSDC_INT);
                    MSDC_WRITE32(MSDC_INT, _u4MsdcAccuVect & vector);
                    return MSDC_SUCCESS;
                }
                
                HAL_Delay_us(1000);
            }
        }   
        else
        {
            for (i = 0;i < timeoutCnt; i++)
            {
                if (vector == (MSDC_READ32(MSDC_INT) & vector)) 
                {
                    _u4MsdcAccuVect |= MSDC_READ32(MSDC_INT);
                    MSDC_WRITE32(MSDC_INT, _u4MsdcAccuVect & vector);
                    return MSDC_SUCCESS;
                }
                
                HAL_Delay_us(1000);
            }
        }
#else
        for (i = 0; i < timeoutCnt; i++)
        {
            if(waitMode == 0)
            {
                if (0 != (MSDC_READ32(MSDC_INT) & vector)) 
                {
                    _u4MsdcAccuVect |= MSDC_READ32(MSDC_INT);
                    MSDC_WRITE32(MSDC_INT, _u4MsdcAccuVect & vector);
                    return MSDC_SUCCESS;
                }
            }
            else
            {
            	  if (vector == (MSDC_READ32(MSDC_INT) & vector)) 
                {
                    _u4MsdcAccuVect |= MSDC_READ32(MSDC_INT);
                    MSDC_WRITE32(MSDC_INT, _u4MsdcAccuVect & vector);
                    return MSDC_SUCCESS;
                }    	
            }

			if((MSDC_READ32(MSDC_INT) & retVector) != 0)
            {
                // return directly
                return MSDC_FAILED; 
            }
            
            HAL_Delay_us(1);
        }
#endif
    }
    else
    {
        // Wait ISR
    }

    // Timeout case
    return MSDC_FAILED;
}

VOID MsdcSetupCmd(sd_cmd_t *pCmd, UINT32 *pu4respType, UINT32 *pu4sdcCmd)
{
    /* Figure out the response type */
    switch (pCmd->idx) 
    {
    case CMD0_GO_IDLE_STATE:
        *pu4respType = RESPONSE_NO;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_NO;
        break;
        
    case CMD1_MMC_SEND_OP_COND:
        *pu4respType = RESPONSE_R3;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R3;
        break;
        
    case CMD2_ALL_SEND_CID:
        *pu4respType = RESPONSE_R2;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R2;
        break;
        
    case CMD3_SEND_RELATIVE_ADDR:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1;
        break;

	case CMD5_IO_SEND_OP_COND:
		*pu4respType = RESPONSE_R4;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R4;
        break;
        
    case CMD6_MMC_SWITCH:
        if(sdCard[ch].cardType == CARDTYPE_MEM_MMC)
        {
            *pu4respType = RESPONSE_R1B;
            (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1B;
        }
        else
        {
            *pu4respType = RESPONSE_R1;
            (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1;
            
            if(pCmd->pBuff != NULL)
            {
                (*pu4sdcCmd) |= (DTYPE_SINGLE_BLK | SDC_CMD_READ);
                MSDC_WRITE32(SDC_BLK_NUM, 1);    	
            }
        }
        break;
        
    case CMD7_SELECT_CARD:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1;	
        break;
        
    case CMD8_SEND_IF_COND:
        if(sdCard[ch].cardType == CARDTYPE_MEM_MMC)
        {
            *pu4respType = RESPONSE_R1; 
            (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_READ);
            MSDC_WRITE32(SDC_BLK_NUM, 1);
        }
        else
        {
            *pu4respType = RESPONSE_R7;
            (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R7;            	
        }
        break;
        
    case CMD9_SEND_CSD:
        *pu4respType = RESPONSE_R2; 
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R2;
        break;   

	case CMD11_VOLTAGE_SWITCH:
        *pu4respType = RESPONSE_R1; 
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | SDC_CMD_VOL_SWITCH);
        break; 
        
    case CMD12_STOP_TRANSMISSION:
        *pu4respType = RESPONSE_R1B;	  
        (*pu4sdcCmd) |= (SDC_CMD_STOP | SDC_CMD_RSPTYPE_R1B);
        break;
        
    case CMD13_SEND_STATUS:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1;
        if(pCmd->pBuff != NULL)
        {
            (*pu4sdcCmd) |= (DTYPE_SINGLE_BLK | SDC_CMD_READ);
            MSDC_WRITE32(SDC_BLK_NUM, 1);      
        } 
        break;
        
    case CMD17_READ_SINGLE_BLOCK:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_READ);
        MSDC_WRITE32(SDC_BLK_NUM, 1);
        break;
        
    case CMD18_READ_MULTIPLE_BLOCK:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_MULTI_BLK | SDC_CMD_READ);
        MSDC_WRITE32(SDC_BLK_NUM, pCmd->buffLen / SDHC_BLK_SIZE);
        break;

	case CMD19_SEND_TUNING_PATTERN:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_AUTO_CMD19);
        break;

	case CMD21_SEND_TUNING_BLOCK:
		*pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_READ);
        MSDC_WRITE32(SDC_BLK_NUM, 1);

    case CMD23_SET_BLOCK_COUNT:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1;
        break;
        
    case CMD24_WRITE_BLOCK:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_WRITE);
        MSDC_WRITE32(SDC_BLK_NUM, 1);
        break;
        
    case CMD25_WRITE_MULTIPLE_BLOCK:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_MULTI_BLK | SDC_CMD_WRITE);
        MSDC_WRITE32(SDC_BLK_NUM, pCmd->buffLen / SDHC_BLK_SIZE);
        break;
        
    case CMD27_PROGRAM_CSD:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_WRITE);
        MSDC_WRITE32(SDC_BLK_NUM, 1);
        break;
        
    case CMD28_SET_WRITE_PROT:
    case CMD29_CLR_WRITE_PROT:
        *pu4respType = RESPONSE_R1B;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1B;
        break;
       
    case CMD30_SEND_WRITE_PROT:
    case CMD31_SEND_WRITE_PROT_TYPE:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1;
        break;
        
    case CMD35_ERASE_WR_BLK_START:
    case CMD36_ERASE_WR_BLK_END:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1;
        break;
        
    case CMD38_ERASE:
        *pu4respType = RESPONSE_R1B;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1B;
        break;
        
    case ACMD41_SD_SEND_OP_COND:
        *pu4respType = RESPONSE_R3;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R3;
        break;

    case ACMD51_SEND_SCR:
        *pu4respType = (RESPONSE_R1);
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R1 | DTYPE_SINGLE_BLK | SDC_CMD_READ);
        MSDC_WRITE32(SDC_BLK_NUM, 1);
        break;

	case CMD52_IO_RW_DIRECT:
        *pu4respType = (RESPONSE_R5);
        (*pu4sdcCmd) |= (SDC_CMD_RSPTYPE_R5);
        break;

	case CMD53_IO_RW_EXTENDED:
        *pu4respType = (RESPONSE_R5);
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R5;
		if(pCmd->arg&0x80000000)
		{
            (*pu4sdcCmd) |= SDC_CMD_WRITE;
		}
		else
		{
		    (*pu4sdcCmd) |= SDC_CMD_READ;
		}
		
		if(pCmd->buffLen/sdHost[ch].blkLen > 1)
		{
            (*pu4sdcCmd) |= DTYPE_MULTI_BLK;    
		}
		else
		{
            (*pu4sdcCmd) |= DTYPE_SINGLE_BLK;
		}
		MSDC_WRITE32(SDC_BLK_NUM, pCmd->buffLen/sdHost[ch].blkLen);
        break;
        
    case CMD55_APP_CMD:
        *pu4respType = RESPONSE_R1;
        (*pu4sdcCmd) |= SDC_CMD_RSPTYPE_R1;
        break;
        	
    }  

	// auto-cmd19 handle
	if(pCmd->idx == CMD19_SEND_TUNING_PATTERN)
	{
	    MSDC_LOG(MSG_LVL_ERR, "cmd19 tuning setup successful!\n");
	}

    // Set Blk Length & auto-cmd12/23
    if(pCmd->pBuff)
    {
        // special for sdio device
        if(pCmd->idx == CMD53_IO_RW_EXTENDED)
        {
            (*pu4sdcCmd) |= (sdHost[ch].blkLen << SDC_CMD_LEN_SHIFT);                
        }
		else
        {
	        (*pu4sdcCmd) |= (MIN_NUM(pCmd->buffLen, SDHC_BLK_SIZE) << SDC_CMD_LEN_SHIFT);
	        if(pCmd->buffLen > SDHC_BLK_SIZE)
	        {
	            if(sdHost[ch].hostAttr & MSDC_DATA_END_AUTO_MASK)
	            {
	                if(sdHost[ch].hostAttr & MSDC_DATA_PRE_DEF_MASK)
	                {    
	                    (*pu4sdcCmd) |= (SDC_CMD_AUTO_CMD23);
	                }
	                else
	                {
	                    (*pu4sdcCmd) |= (SDC_CMD_AUTO_CMD12);    
	                }
	            }
	        }
        }
    }
    
    // Set SDC_CMD.CMD
    (*pu4sdcCmd) |= (pCmd->idx & 0x3F);

    // Set SDC Argument
    MSDC_WRITE32(SDC_ARG, pCmd->arg);

    // Send the commands to the device
    MSDC_WRITE32(SDC_CMD, (*pu4sdcCmd));   
    
    MSDC_LOG(MSG_LVL_INFO, "MsdcSendCmd : CMD%d ARGU%08X : CMD%08X ARGU%08X BLK_NUM%08X(ch=%d, cardCap:%08X)!!\n", pCmd->idx, pCmd->arg, 
                                                                          MSDC_READ32(SDC_CMD), 
                                                                          MSDC_READ32(SDC_ARG),
                                                                          MSDC_READ32(SDC_BLK_NUM), ch, sdCard[ch].cardCap);    
    
}

INT32 MsdcWaitHostIdle(UINT32 timeout)
{
    INT32 i4Ret = CMD_ERR_SUCCESS;
	UINT32 i;
	
    for (i = 0; i < timeout * 1000; i++)
    {
        if ((0 == (MSDC_READ32(SDC_STS) & (SDC_STS_SDCBUSY | SDC_STS_CMDBUSY))) && 
			(0x00  == MSDC_READ32(MSDC_INT)))
        {
		    break;
        }
        
        HAL_Delay_us(1);
    }
    
    if (i == MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT)
    {
        i4Ret = CMD_ERR_FAILED; 
        goto ErrorEnd;        
    }

ErrorEnd:
    return i4Ret;	
}

VOID MsdcHandleResp(sd_cmd_t *pCmd, UINT32 *pu4respType, UINT32 *pu4sdcCmd)
{
    // Handle the response
    switch (*pu4respType) 
    {
    case RESPONSE_NO:
        MSDC_LOG(MSG_LVL_INFO, "CMD%d CMD 0x%08X ARG 0x%08X WITHOUT RESPONSE\n", pCmd->idx, *pu4sdcCmd, pCmd->arg);
        
    case RESPONSE_R1:
	case RESPONSE_R5:
    case RESPONSE_R6:
    case RESPONSE_R7:
    case RESPONSE_R1B:
        pCmd->resp[0] = MSDC_READ32(SDC_RESP0);
        MSDC_LOG(MSG_LVL_INFO, "CMD%d CMD 0x%08X ARG 0x%08X RESPONSE_R1/R5/R6/R7/R1B:%08X\n", pCmd->idx, *pu4sdcCmd, pCmd->arg, 
			                                                                                  pCmd->resp[0]);
        break;
        
    case RESPONSE_R2:
        pCmd->resp[0] = MSDC_READ32(SDC_RESP0);
        pCmd->resp[1] = MSDC_READ32(SDC_RESP1);
        pCmd->resp[2] = MSDC_READ32(SDC_RESP2);
        pCmd->resp[3] = MSDC_READ32(SDC_RESP3);
        MSDC_LOG(MSG_LVL_INFO, "CMD%d CMD 0x%08X ARG 0x%08X RESPONSE_R2:%08X %08X %08X %08X\n", pCmd->idx, *pu4sdcCmd, pCmd->arg,
			                                                                                    pCmd->resp[0], pCmd->resp[1], pCmd->resp[2], pCmd->resp[3]);
        break;
        
    case RESPONSE_R3:
        pCmd->resp[0] = MSDC_READ32(SDC_RESP0);
        MSDC_LOG(MSG_LVL_INFO, "CMD%d CMD 0x%08X ARG 0x%08X RESPONSE_R3:%08X\n", pCmd->idx, *pu4sdcCmd, pCmd->arg, 
			                                                                     pCmd->resp[0]);
        break;
	case RESPONSE_R4:
        pCmd->resp[0] = MSDC_READ32(SDC_RESP0);
        MSDC_LOG(MSG_LVL_INFO, "CMD%d CMD 0x%08X ARG 0x%08X RESPONSE_R4:%08X\n", pCmd->idx, *pu4sdcCmd, pCmd->arg,
			                                                                     pCmd->resp[0]);
        break;
    }
    
    if(pCmd->pBuff)
    {
        MSDC_LOG(MSG_LVL_TRACE, "Data Mode:%d Buffer Addr:%08X Len:%08X!!\n", pCmd->dataMode, 
                                                                              (UINT32)(pCmd->pBuff), 
                                                                              (UINT32)(pCmd->buffLen));
    }

}

INT32 MsdcReqCmdStart(sd_cmd_t *pCmd)
{
    UINT32 u4CmdDoneVect,
		   respType = RESPONSE_NO, sdcCmd = 0;
    INT32 i4Ret = CMD_ERR_SUCCESS;

    // Check if rx/tx fifo is zero
    MsdcChkFifo(pCmd);

    // Clear interrupt Vector
    MsdcClrIntr();  

    if(pCmd->idx != CMD12_STOP_TRANSMISSION)
	{
        if(MsdcWaitHostIdle(MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT))
        {
            i4Ret = ERR_CMD_FAILED;
            MSDC_LOG(MSG_LVL_ERR, "Wait HOST idle failed: SDC_STS(%08X), MSDC_INT(%08X)!\n", MSDC_READ32(SDC_STS), MSDC_READ32(MSDC_INT));
            goto ErrorEnd;
        }
    }

    MsdcSetupCmd(pCmd, &respType, &sdcCmd);

    // Wait for command and response if existed
    if(pCmd->idx == CMD19_SEND_TUNING_PATTERN)
    {
        u4CmdDoneVect = INT_AUTOCMD19_DONE;
    }
	else
    {
        u4CmdDoneVect = INT_SD_CMDRDY | INT_SD_CMDTO | INT_SD_RESP_CRCERR;
    }

    if (MSDC_SUCCESS != MsdcWaitIntr(u4CmdDoneVect, 10*MSDC_WAIT_CMD_TIMEOUT_LIMIT_COUNT, 0)) 
    {
        MSDC_LOG(MSG_LVL_ERR, "Failed to send CMD/RESP, DoneVect = 0x%08x INTR = 0x%08x.\n", u4CmdDoneVect, MSDC_READ32(MSDC_INT));
        i4Ret = CMD_ERR_WAIT_CMD_TO;
        goto ErrorEnd;
    }

    if (_u4MsdcAccuVect & INT_SD_CMDTO)
    {
        MSDC_LOG(MSG_LVL_ERR, "CMD%d ARG 0x%08X - CMD Timeout (AccuVect 0x%08X INTR 0x%08X).\n", pCmd->idx, pCmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
        i4Ret = CMD_ERR_NO_RESP;
        goto ErrorEnd;
    }
    
    if (_u4MsdcAccuVect & INT_SD_RESP_CRCERR)
    {
        MSDC_LOG(MSG_LVL_ERR, "CMD%d ARG 0x%08X - CMD CRC Error (AccuVect 0x%08X INTR 0x%08X).\n", pCmd->idx, pCmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
        i4Ret = CMD_ERR_RESP_CRCERR;
        goto ErrorEnd;
    }

    /* I think it is not necessary to add this interrupt check
    *  We found command interrupt and data transfer interrupt come out together
    */
    /*if ((_u4MsdcAccuVect & (~(INT_SD_CMDRDY))) || (0 != MSDC_READ32(MSDC_INT)))
    {
        MSDC_LOG(MSG_LVL_ERR, "CMD%d ARG 0x%08X - UnExpect status (AccuVect 0x%08X INTR 0x%08X).\n", pCmd->idx, pCmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
        //i4Ret = CMD_ERR_FAILED;      
        //goto ErrorHandling;
    }*/

	if(pCmd->idx == CMD19_SEND_TUNING_PATTERN)
	{
        MSDC_LOG(MSG_LVL_ERR, "tuning result: %08x\n", MSDC_READ32(ACMD19_STS));
		return MSDC_SUCCESS;
	}

	// Handle the response
    MsdcHandleResp(pCmd, &respType, &sdcCmd);

ErrorEnd:
    return i4Ret;	
}

INT32 MsdcReqDataStop(VOID) 
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD12_STOP_TRANSMISSION;
    cmd.dataMode = NULL_DATA_MODE;

    i4Ret = MsdcReqCmdStart(&cmd);
    return i4Ret;	
}

VOID MsdcStopDMA(VOID)
{
    MSDC_LOG(MSG_LVL_INFO, "DMA status: 0x%.8x\n",MSDC_READ32(DMA_CFG));
	
    MSDC_SETBIT(DMA_CTRL, DMA_CTRL_STOP);
    while(MSDC_READ32(DMA_CFG) & DMA_CFG_DMA_STATUS);
	
    MSDC_LOG(MSG_LVL_INFO, "DMA Stopped!\n");
}

INT32 MsdcErrorHandling(sd_cmd_t *pCmd)
{
    int i4Ret = MSDC_SUCCESS;
    MSDC_LOG(MSG_LVL_INFO, "Start Error Handling...!\n");

    // Reset MSDC
    MsdcReset();

    // Stop DMA
    if((sdHost[ch].dataMode > PIO_DATA_MODE) > 1)
    {
        MsdcStopDMA();
    }

    // Clear FIFO and wait it becomes 0
    i4Ret = MsdcClrFifo();
    if(MSDC_SUCCESS != i4Ret)
    {
        goto ErrorEnd;	 
    }

    // Clear MSDC interrupts and make sure all are cleared
    MsdcClrIntr();
    if  (0x00  != MSDC_READ32(MSDC_INT))
    {
        i4Ret = MSDC_FAILED;
        goto ErrorEnd;	
    }

    // Send Stop Command for Multi-Write/Read
    if((pCmd->pBuff) && (pCmd->buffLen > SDHC_BLK_SIZE))
    {
        if (MsdcReqDataStop()) 
        {
            MSDC_LOG(MSG_LVL_WARN, "MSDC fail to send stop cmd\n");
        }
    }

    MSDC_LOG(MSG_LVL_INFO, "End Error Handling...!\n");

ErrorEnd:
    return i4Ret;

}

INT32 MsdcReqCmdTune(sd_cmd_t *pCmd)
{
    INT32 result = MSDC_SUCCESS;
    UINT32 rsmpl, cur_rsmpl, orig_rsmpl;
    UINT32 rrdly, cur_rrdly, orig_rrdly;
    UINT32 skip = 1;

    MSDC_LOG(MSG_LVL_WARN, "----->Go into Command Tune!\n");
	
    MsdcReset();
    MsdcClrFifo();
    MsdcClrIntr();

    orig_rsmpl = ((MSDC_READ32(MSDC_IOCON) & MSDC_IOCON_R_SMPL) >> MSDC_IOCON_R_SMPL_SHIFT);
    orig_rrdly = ((MSDC_READ32(PAD_TUNE) & PAD_CMD_RESP_RXDLY) >> PAD_CMD_RESP_RXDLY_SHIFT);

    rrdly = 0; 
    do 
    {
        for (rsmpl = 0; rsmpl < 2; rsmpl++) 
        {
            /* Lv1: R_SMPL[1] */		
            cur_rsmpl = (orig_rsmpl + rsmpl) % 2;		  
            if (skip == 1) 
            {
                skip = 0;	
                continue;	
            }
        }

        MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_R_SMPL);
        MSDC_SETBIT(MSDC_IOCON, (cur_rsmpl << MSDC_IOCON_R_SMPL_SHIFT));

        result = MsdcReqCmdStart(pCmd);
        if(result == MSDC_SUCCESS)
        {
            MSDC_LOG(MSG_LVL_WARN, "Command Tune Success\n");
            return MSDC_SUCCESS;
        }

        /* Lv2: PAD_CMD_RESP_RXDLY[26:22] */              	
        cur_rrdly = (orig_rrdly + rrdly + 1) % 32;
        MSDC_CLRBIT(PAD_TUNE, PAD_CMD_RESP_RXDLY);
        MSDC_SETBIT(PAD_TUNE, (cur_rrdly << PAD_CMD_RESP_RXDLY_SHIFT));
    }while (++rrdly < 32);

    return result;

}

INT32 MsdcHandleDataTransfer(sd_cmd_t *pCmd)
{
    UINT32 dataEndIntr = 0, u4CmdDoneVect = 0;
	INT32 i4Ret = CMD_ERR_SUCCESS;
	
    /* auto-cmd13 interrupt will raise after response come in
      * but auto-cmd12 interrupt will raise after all data is transferred.
      * we don't handle auto-cmd13 at the response stage, so we can 
      * handle auto-cmd12/13 together here. 
      */
    if((pCmd->buffLen > SDHC_BLK_SIZE) &&
       (sdHost[ch].hostAttr & MSDC_DATA_END_AUTO_MASK))
    {
        dataEndIntr = INT_SD_AUTOCMD_RDY;
    }

    if (PIO_DATA_MODE == pCmd->dataMode) 
    {
        UINT32 u4RxFifoCnt, u4TxFifoCnt;
        UINT32 *pBufData = (UINT32 *)(pCmd->pBuff);
        UINT32 u4BufLen = (UINT32)(pCmd->buffLen);
        UINT32 u4BufEnd = (UINT32)pBufData + u4BufLen;
        UINT32 u4RxCnt = 0;
        
        ASSERT(pBufData != NULL);

        u4CmdDoneVect = INT_SD_DATTO | INT_SD_DATA_CRCERR;

        // Read
        if (!(pCmd->flags & (0x1<<0)))
        {
            while (u4BufLen) 
            {   
                // wait until fifo has enough data
                u4RxFifoCnt = (MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_RXFIFOCNT_MASK);

                while ((u4BufEnd > (UINT32)pBufData) && (sizeof(int) <= u4RxFifoCnt))
                {
                    // Read Data
                    *pBufData = MSDC_READ32(MSDC_RXDATA);
                     pBufData++;

                     u4RxFifoCnt -= sizeof(int);
                     u4BufLen -= sizeof(int);

                     u4RxCnt += sizeof(int);
                     
                     if (u4RxCnt == 64)   // Check CRC error happens on every 128 Byte
                     {
                         // Check if done vector occurs
                         if (u4CmdDoneVect & MSDC_READ32(MSDC_INT))
                         {
                             break;
                         }
                         u4RxCnt = 0;
                     }
                }              
            }         
        }
        else
        {
            while(u4BufEnd > (UINT32)pBufData) 
            {
                // Check if error done vector occurs
                if (u4CmdDoneVect & MSDC_READ32(MSDC_INT))
                {
                    MSDC_LOG(MSG_LVL_ERR, "DoneVect:0x%08X, INTR:0x%08X\n", u4CmdDoneVect, MSDC_READ32(MSDC_INT));
                    break;            
                }

                // wait until fifo has enough space               
                while(1)
                {
                    if((MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_TXFIFOCNT_MASK) == 0)
                    {
                        break;        
                    }    
                }
                
                u4TxFifoCnt = MSDC_FIFO_LEN;

                if(sizeof(int) <= u4TxFifoCnt)
                {
                    while((u4BufEnd > (UINT32)pBufData) && (sizeof(int) <= u4TxFifoCnt))
                    {
                        // Write Data
                        MSDC_WRITE32(MSDC_TXDATA, *pBufData);
                        pBufData++;
                        u4TxFifoCnt -= sizeof(int);
                    }
                }
            }
        }

        // Wait for data complete		
        if(MSDC_SUCCESS != MsdcWaitIntr((dataEndIntr | INT_SD_XFER_COMPLETE), 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT, 1)) 
        {
            MSDC_LOG(MSG_LVL_ERR, "Wait Intr timeout (AccuVect 0x%08X INTR 0x%08X).\n", _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
            i4Ret = CMD_ERR_WAIT_DATATO;
            goto ErrorEnd;
        }

        if(_u4MsdcAccuVect & INT_SD_DATTO)
        {
            MSDC_LOG(MSG_LVL_ERR, "CMD%d ARG 0x%08X - Data Timeout (AccuVect 0x%08X INTR 0x%08X).\n", pCmd->idx, pCmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
            i4Ret = CMD_ERR_WAIT_DATATO;
            goto ErrorEnd;
        }
        else if(_u4MsdcAccuVect & INT_SD_DATA_CRCERR)
        {
            MSDC_LOG(MSG_LVL_ERR, "CMD%d ARG 0x%08X - Data CRC Error (AccuVect 0x%08X INTR 0x%08X).\n", pCmd->idx, pCmd->arg, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
            i4Ret = CMD_ERR_DATA_CRCERR;
            goto ErrorEnd;
        }
        else if((_u4MsdcAccuVect & ~(dataEndIntr | INT_SD_XFER_COMPLETE)) || (0 != MSDC_READ32(MSDC_INT)))
        {
            MSDC_LOG(MSG_LVL_ERR, "UnExpect status (AccuVect 0x%08X INTR 0x%08X).\n", _u4MsdcAccuVect, MSDC_READ32(MSDC_INT));
            i4Ret = CMD_ERR_DATA_FAILED;
            goto ErrorEnd;
        }
    }
    else if(BASIC_DMA_DATA_MODE == pCmd->dataMode)
    {
        UINT32 *pBufData = (UINT32 *)(pCmd->pBuff);
        UINT32 u4BufLen = (UINT32)(pCmd->buffLen);
#if (defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
     defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
     defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882))
        MSDC_WRITE32(DMA_SA, (UINT32)pBufData);
        {

            MSDC_WRITE32(DMA_LENGTH, u4BufLen);
            MSDC_WRITE32(DMA_CTRL, (DMA_BST_64 << DMA_CTRL_BST_SHIFT) | DMA_CTRL_LAST_BUF | DMA_CTRL_START);

            if(MSDC_SUCCESS != MsdcWaitIntr(dataEndIntr | INT_SD_XFER_COMPLETE | INT_DMA_XFER_DONE, 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT, 1)) 
            {
                MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send/receive data (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
                return CMD_ERR_WAIT_DATATO;
            }
            if((_u4MsdcAccuVect & ~(dataEndIntr | INT_SD_XFER_COMPLETE | INT_DMA_XFER_DONE)) || (0 != MSDC_READ32(MSDC_INT))) 
            {
                MSDC_LOG(MSG_LVL_ERR, "%s: Unexpected status (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
                return CMD_ERR_DATA_FAILED;
            }
        }
#else
        UINT32 u4AccLen = 0;

        while(u4AccLen < u4BufLen) 
        {
            MSDC_WRITE32(DMA_SA, (UINT32)pBufData + u4AccLen);
			
            if(u4BufLen - u4AccLen <= BASIC_DMA_MAX_LEN) 
            {
                //MSDC_LOG(MSG_LVL_INFO, "->Last: AccLen = %08X\r\n", u4AccLen);
		#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
            defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    		defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
                MSDC_WRITE32(DMA_LENGTH, (u4BufLen - u4AccLen));
                MSDC_WRITE32(DMA_CTRL, (DMA_BST_64 << DMA_CTRL_BST_SHIFT) | DMA_CTRL_LAST_BUF | DMA_CTRL_START);
		#else
                MSDC_WRITE32(DMA_CTRL, ((u4BufLen - u4AccLen) << DMA_CTRL_XFER_SIZE_SHIFT) | (DMA_BST_64 << DMA_CTRL_BST_SHIFT) | DMA_CTRL_LAST_BUF | DMA_CTRL_START);
		#endif
                // Wait for sd xfer complete
                if(MSDC_SUCCESS != MsdcWaitIntr(dataEndIntr | INT_SD_XFER_COMPLETE | INT_DMA_XFER_DONE, 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT, 1)) 
                {
                    MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send/receive data (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
                    return CMD_ERR_WAIT_DATATO;
                }
                if((_u4MsdcAccuVect & ~(dataEndIntr | INT_SD_XFER_COMPLETE | INT_DMA_XFER_DONE)) || (0 != MSDC_READ32(MSDC_INT))) 
                {
                    MSDC_LOG(MSG_LVL_ERR, "%s: Unexpected status (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
                    return CMD_ERR_DATA_FAILED;
                }

                u4AccLen += u4BufLen - u4AccLen;
            }
            else 
            {
                //MSDC_LOG(MSG_LVL_INFO, "->AccLen = %08X\r\n", u4AccLen);
		#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
            defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
            defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
                MSDC_WRITE32(DMA_LENGTH, BASIC_DMA_MAX_LEN);
                MSDC_WRITE32(DMA_CTRL, (DMA_BST_64 << DMA_CTRL_BST_SHIFT) | DMA_CTRL_START);
		#else
                MSDC_WRITE32(DMA_CTRL, (BASIC_DMA_MAX_LEN << DMA_CTRL_XFER_SIZE_SHIFT) | (DMA_BST_64 << DMA_CTRL_BST_SHIFT) | DMA_CTRL_START);
		#endif

                if(MSDC_SUCCESS != MsdcWaitIntr(INT_DMA_XFER_DONE, 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT, 1)) 
                {
                    MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send/receive data (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
                    return CMD_ERR_WAIT_DATATO;
                }

                // Check DMA status
                if(0 != (MSDC_READ32(DMA_CFG) & (DMA_CFG_DMA_STATUS | DMA_CFG_BD_CS_ERR | DMA_CFG_GPD_CS_ERR))) 
                {
                    MSDC_LOG(MSG_LVL_ERR, "%s: Incorrect DMA status. DMA_CFG: 0x%08X\r\n", __FUNCTION__, MSDC_READ32(DMA_CFG));
                    return CMD_ERR_DATA_FAILED;
                }

                if(_u4MsdcAccuVect & ~(dataEndIntr | INT_DMA_XFER_DONE)) 
                {
                    MSDC_LOG(MSG_LVL_ERR, "%s: Unexpected status (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
                    return CMD_ERR_DATA_FAILED;
                }
                u4AccLen += BASIC_DMA_MAX_LEN;
            }
        }
#endif
    }
    else if(DESC_DMA_DATA_MODE == pCmd->dataMode)
    {
        MSDC_SETBIT(DMA_CTRL, DMA_CTRL_START);
    	
        // Wait for sd xfer complete
        if(MSDC_SUCCESS != MsdcWaitIntr((dataEndIntr | INT_SD_XFER_COMPLETE | INT_DMA_XFER_DONE | INT_DMA_Q_EMPTY), 10*MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT, 1)) 
        {
            MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send/receive data (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
        }

        if((_u4MsdcAccuVect & ~(dataEndIntr | INT_SD_XFER_COMPLETE | INT_DMA_XFER_DONE | INT_DMA_Q_EMPTY)) || (0 != MSDC_READ32(MSDC_INT))) 
        {
            MSDC_LOG(MSG_LVL_ERR, "%s: Unexpected status (AccuVect 0x%08X INTR 0x%08X). %s line %d\r\n", __FUNCTION__, _u4MsdcAccuVect, MSDC_READ32(MSDC_INT), __FILE__, __LINE__);
            i4Ret = CMD_ERR_WAIT_DATATO;
            goto ErrorEnd;
        }

        if(MSDC_READ32(DMA_CFG) & (DMA_CFG_GPD_CS_ERR))
        {
            MSDC_LOG(MSG_LVL_ERR, "Descriptor DMA GPD checksum error");
            i4Ret = CMD_ERR_DATA_FAILED;
            goto ErrorEnd;
        }

        if(MSDC_READ32(DMA_CFG) & (DMA_CFG_BD_CS_ERR))
        {
            MSDC_LOG(MSG_LVL_ERR, "Descriptor DMA BD checksum error");
            i4Ret = CMD_ERR_DATA_FAILED;
            goto ErrorEnd;
        }

        // Check DMA status
        if(0 != (MSDC_READ32(DMA_CFG) & (DMA_CFG_DMA_STATUS))) 
        {
            MSDC_LOG(MSG_LVL_ERR, "%s: Incorrect DMA status. DMA_CFG: 0x%08X\r\n", __FUNCTION__, MSDC_READ32(DMA_CFG));
            i4Ret = CMD_ERR_DATA_FAILED;
            goto ErrorEnd;
        }		
    }
	
ErrorEnd:
    return i4Ret;

}


INT32 MsdcRequest(sd_cmd_t *pCmd)
{
    INT32 i4Ret = MSDC_SUCCESS;
	
    i4Ret = MsdcReqCmdStart(pCmd);
    if(MSDC_SUCCESS != i4Ret)
    {
        if(pCmd->pBuff)
        {
            if(MSDC_SUCCESS != MsdcErrorHandling(pCmd))
            {
			    goto ErrorEnd;
            }
        }
		
        i4Ret = MsdcReqCmdTune(pCmd);
        if(MSDC_SUCCESS != i4Ret)
        {
            goto ErrorEnd;
        }
    }
	
    if(pCmd->pBuff)
    {
        i4Ret = MsdcHandleDataTransfer(pCmd);
        if(MSDC_SUCCESS != i4Ret)
        {
            #ifndef CC_HS200_CALIBRATION_SUPPORT   // tyler.zhang mark it for calibration 
            if(MSDC_SUCCESS != MsdcErrorHandling(pCmd))
            #endif
            {
                goto ErrorEnd;
            }
        }
    }
	
ErrorEnd:
    return i4Ret;
}

INT32 MsdcReqDataReadTune(sd_cmd_t *pCmd)
{
    UINT32 ddr=(sdHost[ch].hostAttr & MSDC_DATA_DDR_MODE_MASK)?1:0;	
    UINT32 dcrc = 0;
    UINT32 rxdly, cur_rxdly0, cur_rxdly1;
    //uint rxdly, cur_rxdly;
    UINT32 dsmpl, cur_dsmpl,  orig_dsmpl;
    UINT32 cur_dat0,  cur_dat1,  cur_dat2,  cur_dat3;
    UINT32 cur_dat4,  cur_dat5,  cur_dat6,  cur_dat7;
    UINT32 orig_dat0, orig_dat1, orig_dat2, orig_dat3;
    UINT32 orig_dat4, orig_dat5, orig_dat6, orig_dat7;
    //uint cur_dat, orig_dat;
#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)||defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    UINT32 dsel, cur_dsel = 0, orig_dsel;
    //uint dl_cksel, cur_dl_cksel = 0, orig_dl_cksel;
#endif
    INT32 result = -1;
    UINT32 skip = 1;

    MSDC_LOG(MSG_LVL_WARN, "----->Go into Data Read Tune!\n");

    orig_dsmpl = ((MSDC_READ32(MSDC_IOCON) & MSDC_IOCON_D_SMPL) >> MSDC_IOCON_D_SMPL_SHIFT);

    /* Tune Method 2. */
    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_DLYLINE_SEL);
    MSDC_SETBIT(MSDC_IOCON, (1 << MSDC_IOCON_D_DLYLINE_SEL_SHIFT));

    MSDC_LOG(MSG_LVL_WARN, "CRC(R) Error Register: %08X!\n", MSDC_READ32(SDC_DATCRC_STS));
    
#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)||defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    orig_dsel = ((MSDC_READ32(PATCH_BIT0) & CKGEN_MSDC_DLY_SEL) >> CKGEN_MSDC_DLY_SEL_SHIFT);
    cur_dsel = orig_dsel;

    dsel = 0;
    do
    {
#endif

    rxdly = 0; 
    do 
    {
        for (dsmpl = 0; dsmpl < 2; dsmpl++) 
        {
            cur_dsmpl = (orig_dsmpl + dsmpl) % 2;
            if (skip == 1) 
            {
                skip = 0; 	
                continue;	
            } 

            MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_SMPL);
            MSDC_SETBIT(MSDC_IOCON, (cur_dsmpl << MSDC_IOCON_D_SMPL_SHIFT));

            result = MsdcRequest(pCmd);

            dcrc = MSDC_READ32(SDC_DATCRC_STS);
            if(!ddr)
                dcrc &= (~SDC_DATCRC_STS_NEG);
			
#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)||defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
                MSDC_LOG(MSG_LVL_WARN, "TUNE_READ<%s> dcrc<0x%x> DATRDDLY0/1<0x%x><0x%x> dsmpl<0x%x> CKGEN_MSDC_DLY_SEL<0x%x>",
                            (result == MSDC_SUCCESS && dcrc == 0) ? "PASS" : "FAIL", dcrc,
                            MSDC_READ32(DAT_RD_DLY0), MSDC_READ32(DAT_RD_DLY1), cur_dsmpl, cur_dsel);
#else
                MSDC_LOG(MSG_LVL_WARN, "TUNE_READ<%s> dcrc<0x%x> DATRDDLY0/1<0x%x><0x%x> dsmpl<0x%x>",
                            (result == MSDC_SUCCESS && dcrc == 0) ? "PASS" : "FAIL", dcrc,
                            MSDC_READ32(DAT_RD_DLY0), MSDC_READ32(DAT_RD_DLY1), cur_dsmpl);
#endif
			
            if((result == MSDC_SUCCESS) && dcrc == 0)
            {
                goto done;
            }
            else
            {
                // Tuning Data error but Command error happens, directly return
                if((result != MSDC_SUCCESS) && (result != ERR_DAT_FAILED))
                {
                    MSDC_LOG(MSG_LVL_WARN, "TUNE_READ(1): result<0x%x> ", result);	
                    goto done;  
                }
                else if((result != MSDC_SUCCESS) && (result == ERR_DAT_FAILED))
                {
                	  // Going On
                    MSDC_LOG(MSG_LVL_WARN, "TUNE_READ(2): result<0x%x>", result);	
                }
            }
        }

        cur_rxdly0 = MSDC_READ32(DAT_RD_DLY0);
        cur_rxdly1 = MSDC_READ32(DAT_RD_DLY1);

        /* E1 ECO. YD: Reverse */
        if (MSDC_READ32(ECO_VER) >= 4) 
        {
            orig_dat0 = (cur_rxdly0 >> 24) & 0x1F;
            orig_dat1 = (cur_rxdly0 >> 16) & 0x1F;
            orig_dat2 = (cur_rxdly0 >>	8) & 0x1F;
            orig_dat3 = (cur_rxdly0 >>	0) & 0x1F;
            orig_dat4 = (cur_rxdly1 >> 24) & 0x1F;
            orig_dat5 = (cur_rxdly1 >> 16) & 0x1F;
            orig_dat6 = (cur_rxdly1 >>	8) & 0x1F;
            orig_dat7 = (cur_rxdly1 >>	0) & 0x1F;
        } 
        else 
        {   
            orig_dat0 = (cur_rxdly0 >>	0) & 0x1F;
            orig_dat1 = (cur_rxdly0 >>	8) & 0x1F;
            orig_dat2 = (cur_rxdly0 >> 16) & 0x1F;
            orig_dat3 = (cur_rxdly0 >> 24) & 0x1F;
            orig_dat4 = (cur_rxdly1 >>	0) & 0x1F;
            orig_dat5 = (cur_rxdly1 >>	8) & 0x1F;
            orig_dat6 = (cur_rxdly1 >> 16) & 0x1F;
            orig_dat7 = (cur_rxdly1 >> 24) & 0x1F;
        }

        if(ddr) 
        {
            cur_dat0 = (dcrc & (1 << 0) || dcrc & (1 << 8))  ? ((orig_dat0 + 1) % 32) : orig_dat0;
            cur_dat1 = (dcrc & (1 << 1) || dcrc & (1 << 9))  ? ((orig_dat1 + 1) % 32) : orig_dat1;
            cur_dat2 = (dcrc & (1 << 2) || dcrc & (1 << 10)) ? ((orig_dat2 + 1) % 32) : orig_dat2;
            cur_dat3 = (dcrc & (1 << 3) || dcrc & (1 << 11)) ? ((orig_dat3 + 1) % 32) : orig_dat3;
        } 
        else 
        {
            cur_dat0 = (dcrc & (1 << 0)) ? ((orig_dat0 + 1) % 32) : orig_dat0;
            cur_dat1 = (dcrc & (1 << 1)) ? ((orig_dat1 + 1) % 32) : orig_dat1;
            cur_dat2 = (dcrc & (1 << 2)) ? ((orig_dat2 + 1) % 32) : orig_dat2;
            cur_dat3 = (dcrc & (1 << 3)) ? ((orig_dat3 + 1) % 32) : orig_dat3;
        }
        cur_dat4 = (dcrc & (1 << 4)) ? ((orig_dat4 + 1) % 32) : orig_dat4;
        cur_dat5 = (dcrc & (1 << 5)) ? ((orig_dat5 + 1) % 32) : orig_dat5;
        cur_dat6 = (dcrc & (1 << 6)) ? ((orig_dat6 + 1) % 32) : orig_dat6;
        cur_dat7 = (dcrc & (1 << 7)) ? ((orig_dat7 + 1) % 32) : orig_dat7;

        /* E1 ECO. YD: Reverse */
        if (MSDC_READ32(ECO_VER) >= 4) 
        {
            cur_rxdly0 = (cur_dat0 << 24) | (cur_dat1 << 16) | (cur_dat2 << 8) | (cur_dat3 << 0);
            cur_rxdly1 = (cur_dat4 << 24) | (cur_dat5 << 16) | (cur_dat6 << 8) | (cur_dat7 << 0);
        }
        else
        {
            cur_rxdly0 = (cur_dat3 << 24) | (cur_dat2 << 16) | (cur_dat1 << 8) | (cur_dat0 << 0);
            cur_rxdly1 = (cur_dat7 << 24) | (cur_dat6 << 16) | (cur_dat5 << 8) | (cur_dat4 << 0);   
        }
	
        MSDC_WRITE32(DAT_RD_DLY0, cur_rxdly0);
        MSDC_WRITE32(DAT_RD_DLY1, cur_rxdly1);   
    }while(++rxdly < 32);
#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)||defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
        cur_dsel = (orig_dsel + dsel + 1) % 32;
        MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
        MSDC_SETBIT(PATCH_BIT0, (cur_dsel << CKGEN_MSDC_DLY_SEL_SHIFT));
    } while (++dsel < 32);
#endif

done:		
    return result;
}

INT32 MsdcReqDataWriteTune(sd_cmd_t *pCmd)
{
    UINT32 wrrdly, cur_wrrdly = 0, orig_wrrdly;
    UINT32 dsmpl,  cur_dsmpl,  orig_dsmpl;
    UINT32 rxdly,  cur_rxdly0;
    UINT32 orig_dat0, orig_dat1, orig_dat2, orig_dat3;
    UINT32 cur_dat0,  cur_dat1,  cur_dat2,  cur_dat3;
#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)||defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    unsigned int d_cntr, cur_d_cntr = 0, orig_d_cntr;
#endif
    INT32 result = -1;
    UINT32 skip = 1;

    MSDC_LOG(MSG_LVL_WARN, "----->Go into Data Write Tune!\n");

    orig_wrrdly = ((MSDC_READ32(PAD_TUNE) & PAD_DAT_WR_RXDLY) >> PAD_DAT_WR_RXDLY_SHIFT);
    orig_dsmpl = ((MSDC_READ32(MSDC_IOCON) & MSDC_IOCON_D_SMPL) >> MSDC_IOCON_D_SMPL_SHIFT);
#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)||defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    orig_dsmpl = ((MSDC_READ32(MSDC_IOCON) & MSDC_IOCON_W_D_SMPL_SEL) >> MSDC_IOCON_W_D_SMPL_SHIFT);
#else
    orig_dsmpl = ((MSDC_READ32(MSDC_IOCON) & MSDC_IOCON_D_SMPL) >> MSDC_IOCON_D_SMPL_SHIFT);
#endif
    MSDC_LOG(MSG_LVL_WARN, "CRC(W) Error Register: %08X!\n", MSDC_READ32(SDC_DATCRC_STS));

    /* Tune Method 2. just DAT0 */  
    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_DLYLINE_SEL);
    MSDC_SETBIT(MSDC_IOCON, (1 << MSDC_IOCON_D_DLYLINE_SEL_SHIFT));
    cur_rxdly0 = MSDC_READ32(DAT_RD_DLY0);

    /* E1 ECO. YD: Reverse */
    if (MSDC_READ32(ECO_VER) >= 4) 
    {
        orig_dat0 = (cur_rxdly0 >> 24) & 0x1F;
        orig_dat1 = (cur_rxdly0 >> 16) & 0x1F;
        orig_dat2 = (cur_rxdly0 >>	8) & 0x1F;
        orig_dat3 = (cur_rxdly0 >>	0) & 0x1F;
    } 
    else 
    {
        orig_dat0 = (cur_rxdly0 >>	0) & 0x1F;
        orig_dat1 = (cur_rxdly0 >>	8) & 0x1F;
        orig_dat2 = (cur_rxdly0 >> 16) & 0x1F;
        orig_dat3 = (cur_rxdly0 >> 24) & 0x1F;
    }
#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)||defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    orig_d_cntr = ((MSDC_READ32(PATCH_BIT1) & WRDAT_CRCS_TA_CNTR) >> WRDAT_CRCS_TA_CNTR_SHIFT);
    cur_d_cntr = orig_d_cntr;

    d_cntr = 0;
    do
    {
#endif
    rxdly = 0;
    do 
    {
        wrrdly = 0;
        do 
        {    
            for (dsmpl = 0; dsmpl < 2; dsmpl++) 
            {
                cur_dsmpl = (orig_dsmpl + dsmpl) % 2;
                if (skip == 1) 
                {
                    skip = 0;
                    continue; 	
                }

#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)||defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
                    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL);
                    MSDC_SETBIT(MSDC_IOCON, (cur_dsmpl << MSDC_IOCON_W_D_SMPL_SHIFT));
#else
                    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_SMPL);
                    MSDC_SETBIT(MSDC_IOCON, (cur_dsmpl << MSDC_IOCON_D_SMPL_SHIFT));
#endif

                result = MsdcRequest(pCmd);

#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)||defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
                    MSDC_LOG(MSG_LVL_WARN,  "TUNE_WRITE<%s> DSPL<%d> DATWRDLY<0x%x> MSDC_DAT_RDDLY0<0x%x> WRDAT_CRCS_TA_CNTR<0x%x>", 
                                      (result == MSDC_SUCCESS ? "PASS" : "FAIL"), cur_dsmpl, cur_wrrdly, cur_rxdly0, cur_d_cntr);
#else
                    MSDC_LOG(MSG_LVL_WARN,  "TUNE_WRITE<%s> DSPL<%d> DATWRDLY<0x%x> MSDC_DAT_RDDLY0<0x%x>", 
                                      (result == MSDC_SUCCESS ? "PASS" : "FAIL"), cur_dsmpl, cur_wrrdly, cur_rxdly0);
#endif

                if(result == MSDC_SUCCESS)
                {
                    goto done;
                }
                else
                {
                	  // Tuning Data error but Command error happens, directly return
                    if((result != MSDC_SUCCESS) && (result != ERR_DAT_FAILED))
                    {
                        MSDC_LOG(MSG_LVL_WARN, "TUNE_WRITE(1): result<0x%x>", result);
						
                        goto done; 
                    }
                    else if((result != MSDC_SUCCESS) && (result == ERR_DAT_FAILED))
                    {
                    	  // Going On
                        MSDC_LOG(MSG_LVL_WARN, "TUNE_WRITE(2): result<0x%x>", result);
                    }
                }
            }

            cur_wrrdly = (orig_wrrdly + wrrdly + 1) % 32;
            MSDC_CLRBIT(PAD_TUNE, PAD_DAT_WR_RXDLY);
            MSDC_SETBIT(PAD_TUNE, (cur_wrrdly << PAD_DAT_WR_RXDLY_SHIFT));
        }while(++wrrdly < 32);
		
        cur_dat0 = (orig_dat0 + rxdly) % 32; /* only adjust bit-1 for crc */
        cur_dat1 = orig_dat1;
        cur_dat2 = orig_dat2;
        cur_dat3 = orig_dat3;                    

        /* E1 ECO. YD: Reverse */
        if (MSDC_READ32(ECO_VER) >= 4) 
        {
            cur_rxdly0 = (cur_dat0 << 24) | (cur_dat1 << 16) | (cur_dat2 << 8) | (cur_dat3 << 0);  
        }
        else
        {
            cur_rxdly0 = (cur_dat3 << 24) | (cur_dat2 << 16) | (cur_dat1 << 8) | (cur_dat0 << 0); 
        }
        MSDC_WRITE32(DAT_RD_DLY0, cur_rxdly0); 
    }while(++rxdly < 32);
#if defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)||defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
        cur_d_cntr = (orig_d_cntr + d_cntr + 1) % 8;
        MSDC_CLRBIT(PATCH_BIT1, WRDAT_CRCS_TA_CNTR);
        MSDC_SETBIT(PATCH_BIT1, (cur_d_cntr << WRDAT_CRCS_TA_CNTR_SHIFT));
    } while (++d_cntr < 8);
#endif

done:
    return result;
}

INT32 MsdcReqDataTune(sd_cmd_t *pCmd)
{
    INT32 i4Ret = MSDC_SUCCESS;

    // cmd21 doesn't need tuning process.
	if(pCmd->idx == CMD21_SEND_TUNING_BLOCK)
	{
       return MSDC_FAILED;
	}
	
    if (pCmd->flags & (0x1<<0))
    {
        i4Ret = MsdcReqDataWriteTune(pCmd);
    }
    else
    {
        i4Ret = MsdcReqDataReadTune(pCmd);
    }

    return i4Ret;
}


INT32 MsdcSendCmd (sd_cmd_t *pCmd) 
{
    INT32 i4Ret = MSDC_SUCCESS;
	
    i4Ret = MsdcRequest(pCmd);
    if((i4Ret >= ERR_DAT_FAILED) && 
	   (pCmd->pBuff) )
    {
        #ifndef CC_HS200_CALIBRATION_SUPPORT   // tyler.zhang mark it for calibration 
        i4Ret = MsdcReqDataTune(pCmd);	
        if(MSDC_SUCCESS != i4Ret)
     	#endif
        {
            goto ErrorEnd;
        }
    } 
	
ErrorEnd:
    return i4Ret;
}

/* Command Set */
INT32 cmd0_go_idle_state(UINT32 arg)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD0_GO_IDLE_STATE;
    cmd.arg = arg;
    cmd.dataMode = NULL_DATA_MODE;

    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;
}

INT32 cmd1_mmc_send_op_cond(VOID)
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD1_MMC_SEND_OP_COND;
    cmd.dataMode = NULL_DATA_MODE;
    cmd.arg = HOST_OCR;

    i4Ret = MsdcSendCmd(&cmd);
    if(CMD_ERR_SUCCESS == i4Ret)
    {
        x_memcpy(&(sdCard[ch].memOcr), &cmd.resp[0], sizeof(mem_ocr_t));    	
    }
    return i4Ret;
}

INT32 cmd2_all_send_cid(VOID)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD2_ALL_SEND_CID;
    cmd.dataMode = NULL_DATA_MODE;

    i4Ret = MsdcSendCmd(&cmd);
    if(CMD_ERR_SUCCESS == i4Ret)
    {
        x_memcpy(&(sdCard[ch].cid), cmd.resp, sizeof(sd_cid_t));
        MsdcShowChipInfo(cmd.resp);    	
    }
    return i4Ret;
}

INT32 cmd3_send_relative_addr(VOID) 
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD3_SEND_RELATIVE_ADDR;
    cmd.dataMode = NULL_DATA_MODE;

    if (sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {
        sdCard[ch].memRca = MMC_DEF_RCA;
    }
    else if(sdCard[ch].cardType == CARDTYPE_MEM_SD)
    {
        sdCard[ch].memRca = SD_DEF_RCA;
    }
	else  // maybe sdio devcie
	{
        sdCard[ch].memRca = SD_DEF_RCA;
	}

    cmd.arg = (sdCard[ch].memRca) << 16;
            
    i4Ret = MsdcSendCmd(&cmd);
    if((CMD_ERR_SUCCESS == i4Ret) && (sdCard[ch].cardType != CARDTYPE_MEM_MMC))
    {
        sdCard[ch].memRca = (cmd.resp[0]>>16);
    }
 
    return i4Ret;
}

INT32 cmd6_mmc_set_bus_width(INT32 busWidth) 
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD6_MMC_SWITCH;
    cmd.dataMode = NULL_DATA_MODE;
	
    // determine Arguments according to bus selection
    if (1 == busWidth) 
    {
        cmd.arg = MMC_CMD6_ARG_1BIT_BUS;
    }
    else if (4 == busWidth) 
    {
        cmd.arg = MMC_CMD6_ARG_4BIT_BUS;
    }
    else if (8 == busWidth)
    {
        cmd.arg = MMC_CMD6_ARG_8BIT_BUS;
    }
	         
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;
}

INT32 cmd6_mmc_set_speed_mode(INT32 fgHSMode) 
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD6_MMC_SWITCH;
    cmd.dataMode = NULL_DATA_MODE;
	
    // determine Arguments according to speed mode selection
    if(SPEED_MODE_DS == fgHSMode) 
    {
        cmd.arg = MMC_CMD6_ARG_NORM_SPEED;
    }
    else if(SPEED_MODE_HS == fgHSMode)
    {
        cmd.arg = MMC_CMD6_ARG_HIGH_SPEED;
    }
	else if(SPEED_MODE_HS200 == fgHSMode)
	{
        cmd.arg = MMC_CMD6_ARG_HIGH_SPEED200;
    }
    else if(SPEED_MODE_DDR50 == fgHSMode)
	{
        cmd.arg = MMC_CMD6_ARG_8BIT_BUS_DUAL;
    }
	else
	{
        MSDC_LOG(MSG_LVL_ERR, "not support speed mode(%d)!\n", fgHSMode);
		return MSDC_FAILED;
	}
       
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;
}

INT32 cmd6_mmc_set_ext_csd(UINT32 index, UINT8 value) 
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD6_MMC_SWITCH;
    cmd.dataMode = NULL_DATA_MODE;
    cmd.arg = CMD6_ARGU_EXTCSD(index, value);
       
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;
}

INT32 cmd6_sd_switch_func(UINT8 *status, UINT32 arg)
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD6_SD_SWITCH;
    cmd.dataMode = sdHost[ch].dataMode;
    cmd.arg = arg;
    cmd.pBuff = status;
    cmd.buffLen = 0x40;
       
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

INT32 cmd7_select_card(UINT32 sel) 
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD7_SELECT_CARD;
    cmd.dataMode = NULL_DATA_MODE;

    if(sel)
    {
	    cmd.arg = (sdCard[ch].memRca) << 16;
    }
    else 
    {
        cmd.arg = 0xFFF0;
    }

    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;
}

INT32 cmd8_sd_send_if_cond(VOID)
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD8_SEND_IF_COND;
    cmd.dataMode = NULL_DATA_MODE;
    cmd.arg = (0x1 << 8) | 0xCC;

    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;
}

INT32 cmd8_mmc_send_ext_csd(UINT8 *pBuff)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD8_MMC_SEND_EXT_CSD;
    cmd.dataMode = sdHost[ch].dataMode;
    cmd.pBuff = pBuff;
    cmd.buffLen = ArraySize(sdHost[ch].EXT_CSD);
    
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;
}

/* Get CSD: Card Specific Data;
*  Information about the card operation conditions
*  CMD9 
*/
INT32 cmd9_send_csd(UINT8 *pBuff)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD9_SEND_CSD;
    cmd.dataMode = NULL_DATA_MODE;;
    cmd.arg = (sdCard[ch].memRca) << 16;
    
    i4Ret = MsdcSendCmd(&cmd);
    if(CMD_ERR_SUCCESS == i4Ret)
    {
        x_memcpy(pBuff, cmd.resp, sizeof(csd_20_t));
    }
    return i4Ret;	
}

INT32 cmd11_sd_voltage_switch(VOID)
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD11_VOLTAGE_SWITCH;
    cmd.dataMode = NULL_DATA_MODE;;
    cmd.arg = 0x0;
    
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}


/* Get CSR: Card Status;
*  Information about the card status
*  CMD13 
*/
INT32 cmd13_send_status(VOID *pBuff)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD13_SEND_STATUS;
    cmd.dataMode = NULL_DATA_MODE;;
    cmd.arg = (sdCard[ch].memRca) << 16;
    
    i4Ret = MsdcSendCmd(&cmd);
    if(CMD_ERR_SUCCESS == i4Ret)
    {
        x_memcpy(pBuff, &cmd.resp[0], sizeof(int));    	
    }
    
    return i4Ret;		
}

/* Get SSR: SD Status;
*  Information about the card proprietary features 
*  ACMD13 
*/
INT32 cmd13_send_sd_status(VOID *pBuff)
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD13_SEND_STATUS;
    cmd.dataMode = sdHost[ch].dataMode;
    cmd.pBuff = pBuff;
    cmd.buffLen = 0x40;
    
    i4Ret = MsdcSendCmd(&cmd);   
    return i4Ret;		
}

INT32 cmd17_read_single_block(UINT64 addr, VOID *pBuff, UINT32 buffLen)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD17_READ_SINGLE_BLOCK;
    cmd.dataMode = sdHost[ch].dataMode;
    cmd.pBuff = pBuff;
	
    if (sdCard[ch].cardCap & CARD_CAP_BLOCK_MODE)
    {
        cmd.arg = (UINT32)(addr >> 9);      // Block addressing mode
    }
    else
    {
        cmd.arg = (UINT32)addr;           // Byte addressing mode
    }
	
    cmd.buffLen = buffLen;
	
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

INT32 cmd18_read_multiple_block(UINT64 addr, VOID *pBuff, UINT32 buffLen) 
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD18_READ_MULTIPLE_BLOCK;
    cmd.dataMode = sdHost[ch].dataMode;
    cmd.pBuff = pBuff;
	
    if (sdCard[ch].cardCap & CARD_CAP_BLOCK_MODE)
    {
        cmd.arg = (UINT32)(addr >> 9);    // Block addressing mode
    }
    else
    {
        cmd.arg = (UINT32)addr;           // Byte addressing mode
    }
	
    cmd.buffLen = buffLen;
	
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

INT32 cmd19_sd_send_tuning_pattern(VOID) 
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD19_SEND_TUNING_PATTERN;
	
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

INT32 cmd21_mmc_send_tuning_block(VOID *pBuff, UINT32 buffLen)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD21_SEND_TUNING_BLOCK;
    cmd.dataMode = sdHost[ch].dataMode;
    cmd.pBuff = pBuff;
	
    cmd.buffLen = buffLen;

    if(sdHost[ch].dataMode > 1)
    {
        HalDmaBufferStart((VOID *)pBuff, buffLen, HAL_DMA_FROM_DEVICE);
    }
    
    if(sdHost[ch].dataMode == 3)
        MsdcDescriptorConfig(pBuff, buffLen);
	
    i4Ret = MsdcSendCmd(&cmd);

    if(sdHost[ch].dataMode > 1)
    {
        HalDmaBufferDone((VOID *)pBuff, buffLen, HAL_DMA_FROM_DEVICE);
    }
	
    return i4Ret;	
}


INT32 cmd23_set_block_count(UINT32 blk_cnt)
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD23_SET_BLOCK_COUNT;
    cmd.dataMode = NULL_DATA_MODE;
    cmd.arg = (blk_cnt&0xFFFF);
    
    i4Ret = MsdcSendCmd(&cmd);   
    return i4Ret;

}

INT32 cmd24_write_single_block(UINT64 addr, VOID *pBuff, UINT32 buffLen)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD24_WRITE_BLOCK;
    cmd.dataMode = sdHost[ch].dataMode;
    cmd.pBuff = pBuff;
    cmd.buffLen = buffLen;
	cmd.flags |= (0x1<<0);
	
    if (sdCard[ch].cardCap & CARD_CAP_BLOCK_MODE)
    {
        cmd.arg = (UINT32)(addr >> 9);  // Block addressing mode
    }
    else
    {
        cmd.arg = (UINT32)addr;       // Byte addressing mode
    }
	
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	

}

INT32 cmd25_write_multiple_block(UINT64 addr, VOID *pBuff, UINT32 buffLen)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD25_WRITE_MULTIPLE_BLOCK;
    cmd.dataMode = sdHost[ch].dataMode;
    cmd.pBuff = pBuff;
    cmd.buffLen = buffLen;
	cmd.flags |= (0x1<<0);

    if (sdCard[ch].cardCap & CARD_CAP_BLOCK_MODE)
    {
        cmd.arg = (UINT32)(addr >> 9);     // Block addressing mode
    }
    else
    {
        cmd.arg = (UINT32)addr;           // Byte addressing mode
    }
	
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

int cmd27_program_csd(VOID *pBuff)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD27_PROGRAM_CSD;
    cmd.dataMode = sdHost[ch].dataMode;
    cmd.pBuff = pBuff;
    cmd.buffLen = sizeof(csd_20_t);	
    cmd.arg = 0;
    
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

int cmd28_set_write_prot(UINT64 wpAddr)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD28_SET_WRITE_PROT;
    cmd.dataMode = NULL_DATA_MODE;
    if (sdCard[ch].cardCap & CARD_CAP_BLOCK_MODE)
    {
        cmd.arg = (UINT32)(wpAddr >> 9);      // Block addressing mode
    }
    else
    {
        cmd.arg = (UINT32)wpAddr;           // Byte addressing mode
    }
    
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

int cmd29_clr_write_prot(UINT64 wpAddr)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD29_CLR_WRITE_PROT;
    cmd.dataMode = NULL_DATA_MODE;
    if (sdCard[ch].cardCap & CARD_CAP_BLOCK_MODE)
    {
        cmd.arg = (UINT32)(wpAddr >> 9);      // Block addressing mode
    }
    else
    {
        cmd.arg = (UINT32)wpAddr;           // Byte addressing mode
    }
    
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

int cmd30_send_write_prot(UINT64 wpAddr, VOID *pBuff)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD30_SEND_WRITE_PROT;
    cmd.dataMode = sdHost[ch].dataMode;
    cmd.pBuff = pBuff;
    cmd.buffLen = 4;
    if (sdCard[ch].cardCap & CARD_CAP_BLOCK_MODE)
    {
        cmd.arg = (UINT32)(wpAddr >> 9);      // Block addressing mode
    }
    else
    {
        cmd.arg = (UINT32)wpAddr;           // Byte addressing mode
    }
    
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

int cmd31_send_write_prot_type(UINT64 wpAddr, VOID *pBuff)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD31_SEND_WRITE_PROT_TYPE;
    cmd.dataMode = NULL_DATA_MODE;
    cmd.pBuff = pBuff;
    cmd.buffLen = 8;
    if (sdCard[ch].cardCap & CARD_CAP_BLOCK_MODE)
    {
        cmd.arg = (UINT32)(wpAddr >> 9);      // Block addressing mode
    }
    else
    {
        cmd.arg = (UINT32)wpAddr;           // Byte addressing mode
    }
    
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

INT32 cmd35_erase_wr_blk_start(UINT64 addr)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD35_ERASE_WR_BLK_START;
    cmd.dataMode = NULL_DATA_MODE;
    if (sdCard[ch].cardCap & CARD_CAP_BLOCK_MODE)
    {
        cmd.arg = (UINT32)(addr >> 9);    // Block addressing mode
    }
    else
    {
        cmd.arg = (UINT32)addr;           // Byte addressing mode
    }
	
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	

}

INT32 cmd36_erase_wr_blk_end(UINT64 addr)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD36_ERASE_WR_BLK_END;
    cmd.dataMode = NULL_DATA_MODE;
    if (sdCard[ch].cardCap & CARD_CAP_BLOCK_MODE)
    {
        cmd.arg = (UINT32)(addr >> 9);    // Block addressing mode
    }
    else
    {
        cmd.arg = (UINT32)addr;           // Byte addressing mode
    }
	
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

INT32 cmd38_erase(UINT32 arg)
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD38_ERASE;
    cmd.dataMode = NULL_DATA_MODE;
    cmd.arg = arg;
	
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

INT32 cmd41_sd_send_op_cond(INT32 inquiry)
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = ACMD41_SD_SEND_OP_COND;
    cmd.dataMode = NULL_DATA_MODE;

    if(inquiry)
    {
        cmd.arg = 0;
    }
    else
    {
         cmd.arg = (HOST_OCR_ATTRIBUTE |
		 	       (HOST_OCR_VOLTAGE& *(UINT32 *)(&sdCard[ch].memOcr)));
    }

    i4Ret = MsdcSendCmd(&cmd);
    if(CMD_ERR_SUCCESS == i4Ret)
    {
        x_memcpy(&(sdCard[ch].memOcr), &cmd.resp[0], sizeof(mem_ocr_t));		
    }

    return i4Ret;
}

INT32 cmd51_sd_send_scr(VOID)
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = ACMD51_SEND_SCR;
    cmd.dataMode = sdHost[ch].dataMode;
    cmd.pBuff = (VOID *)(&(sdCard[ch].memScr));
    cmd.buffLen = sizeof(sdCard[ch].memScr);

    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;
}

INT32 cmd55_app_cmd(VOID)
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD55_APP_CMD;
    cmd.dataMode = NULL_DATA_MODE;
    cmd.arg = (sdCard[ch].memRca << 16);

    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;
}

INT32 acmd13_send_sd_status(VOID *pBuff)
{
    INT32 i4Ret;

    i4Ret = cmd55_app_cmd();
    if(CMD_ERR_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send CMD55. %s line %d.\n", __FUNCTION__, __FILE__, __LINE__);
        return i4Ret;
    }

    i4Ret = cmd13_send_sd_status(pBuff);
    if(CMD_ERR_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send CMD13. %s line %d.\n", __FUNCTION__, __FILE__, __LINE__);
        return i4Ret;    
    } 

    return CMD_ERR_SUCCESS;		
}

INT32 acmd41_sd_send_op_cond(INT32 inquiry)
{
    INT32 i4Ret;

    i4Ret = cmd55_app_cmd();
    if(CMD_ERR_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send CMD55. %s line %d.\n", __FUNCTION__, __FILE__, __LINE__);
        return i4Ret;
    }

    i4Ret = cmd41_sd_send_op_cond(inquiry);
    if(CMD_ERR_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send CMD41. %s line %d.\n", __FUNCTION__, __FILE__, __LINE__);
        return i4Ret;    
    } 

    return CMD_ERR_SUCCESS;
}

INT32 acmd51_sd_send_scr(VOID)
{
    INT32 i4Ret;

    i4Ret = cmd55_app_cmd();
    if(CMD_ERR_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send CMD55. %s line %d.\n", __FUNCTION__, __FILE__, __LINE__);
        return i4Ret;
    }

    i4Ret = cmd51_sd_send_scr();
    if(CMD_ERR_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send CMD51. %s line %d.\n", __FUNCTION__, __FILE__, __LINE__);
        return i4Ret;    
    } 

    return CMD_ERR_SUCCESS;
}

INT32 cmd6_sd_set_bus_width(INT32 busWidth)
{
    INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = ACMD6_SET_BUS_WIDTH;
    cmd.dataMode = NULL_DATA_MODE;
	
    // determine Arguments according to bus selection
    if (1 == busWidth) 
    {
        cmd.arg = ACMD6_BUS_WIDTH_1;
    }
    else if (4 == busWidth) 
    {
        cmd.arg = ACMD6_BUS_WIDTH_4;
    }
	         
    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;

}

INT32 acmd6_sd_set_bus_width(INT32 busWidth)
{
    if(CMD_ERR_SUCCESS != cmd55_app_cmd())
    {
        return CMD_ERR_FAILED;
    }

    if(CMD_ERR_SUCCESS != cmd6_sd_set_bus_width(busWidth))
    {
        return CMD_ERR_FAILED;
    }

    return CMD_ERR_SUCCESS;
}

INT32 cmd12_stop_transmission(VOID) 
{
	  INT32 i4Ret;
    sd_cmd_t cmd;

    x_memset(&cmd, 0, sizeof(sd_cmd_t));
    cmd.idx = CMD12_STOP_TRANSMISSION;
    cmd.dataMode = NULL_DATA_MODE;

    i4Ret = MsdcSendCmd(&cmd);
    return i4Ret;	
}

/* Final stage of card identification flow.
 */
INT32 MsdcCardTypeChk(VOID)
{
    // CMD2
    if (CMD_ERR_SUCCESS != cmd2_all_send_cid())
    {
        MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send CMD2. %s line %d.\n", __FUNCTION__, __FILE__, __LINE__);
        return MSDC_FAILED;
    }

	if(sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {
        MsdcFindDevName((UINT32 *)(&(sdCard[ch].cid)));
    }

    // CMD3
    if (CMD_ERR_SUCCESS != cmd3_send_relative_addr()) 
    {
        MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send CMD3. %s line %d.\n", __FUNCTION__, __FILE__, __LINE__);
        return MSDC_FAILED;
    }
    
    return MSDC_SUCCESS;
}

INT32 MsdcCardQuery(VOID)
{
    INT32 ret = 0;
    UINT32 retry = 0;

    if (sdCard[ch].memOcr.ocr & HOST_OCR) 
    {
        // Set new Voltage		
        while (1) 
        {
            if(sdCard[ch].cardType == CARDTYPE_MEM_MMC)
            {
                ret = cmd1_mmc_send_op_cond();
            }
            else if(sdCard[ch].cardType == CARDTYPE_MEM_SD)
            {
                ret = acmd41_sd_send_op_cond(0);
            }

            if (CMD_ERR_SUCCESS == ret) 
            {
                if (sdCard[ch].memOcr.memRdy) 
                {
                    sdCard[ch].memInitDone = 1;
                    
                    if (sdCard[ch].memOcr.ccs)
                    {
                        sdCard[ch].cardCap |= CARD_CAP_BLOCK_MODE;
                    }
                    else
                    {
						sdCard[ch].cardCap &= ~CARD_CAP_BLOCK_MODE;
                    }

                    if(sdCard[ch].cardType == CARDTYPE_MEM_SD)
                    {
						if (sdCard[ch].memOcr.s18a)
					    {
                            sdCard[ch].cardCap |= CARD_CAP_LOW_VOLTAGE_18V;
					    }
						else
						{
						    sdCard[ch].cardCap &= ~CARD_CAP_LOW_VOLTAGE_18V;
						}
                    }
     
                    return MSDC_SUCCESS;
                }
                else 
                {
                    retry++;
                    if (retry < MMC_CMD1_RETRY_LIMIT) 
                    {
                        HAL_Delay_us(2000);
                        continue;
                    }
                    else 
                    {
                        MSDC_LOG(MSG_LVL_ERR, "Failed to init the card, Retry = %d.\n", retry);
                        return MSDC_FAILED;
                    }
                }
            }
            else 
            {
                MSDC_LOG(MSG_LVL_ERR, "Failed to send operation condition cmd.\n");
                return MSDC_FAILED;
            }
        }
    }

    return MSDC_FAILED;
}

INT32 MsdcCardRescanMMC(VOID)
{
    INT32 i;

    for (i = 0; i < CMD0_RESET_LIMIT; i++)
    {
    	// CMD0 Pin1 = High
        if (MSDC_SUCCESS != cmd0_go_idle_state(0x00)) 
    	{
          MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send GO_IDLE_STATE cmd in %s line %d\n", __FUNCTION__, __FILE__, __LINE__);
          return MSDC_FAILED;
    	}
    }
  	
    if (MSDC_SUCCESS != cmd1_mmc_send_op_cond())
    {
        MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send CMD1 %s line %d\n", __FUNCTION__, __FILE__, __LINE__);
        return MSDC_FAILED;
    }

    sdCard[ch].cardType = CARDTYPE_MEM_MMC;
    
    return MSDC_SUCCESS;
}

INT32 MsdcCardRescanSD(VOID)
{
    INT32 ret, i;
	
    for (i = 0; i < CMD0_RESET_LIMIT; i++)
    {
        // CMD0 Pin1 = High
        if (MSDC_SUCCESS != cmd0_go_idle_state(0x00)) 
        {
            MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send GO_IDLE_STATE cmd in %s line %d\n", __FUNCTION__, __FILE__, __LINE__);
                return MSDC_FAILED;
            }
        }

    /* send cmd8 to check the operating voltage
        */
    ret = cmd8_sd_send_if_cond();
    if(CMD_ERR_NO_RESP == ret)
    {
        MSDC_LOG(MSG_LVL_WARN, "%s: Without Response to CMD8 %s line %d\n", __FUNCTION__, __FILE__, __LINE__); 
    }
    else if((CMD_ERR_FAILED == ret) || (CMD_ERR_RESP_CRCERR == ret))
    {
        MSDC_LOG(MSG_LVL_ERR, "%s: Failed to send CMD8 %s line %d\n", __FUNCTION__, __FILE__, __LINE__); 
        return MSDC_FAILED;
    }

    /* send acmd41 to check the operating condition
    */
    ret = acmd41_sd_send_op_cond(1);
    if(CMD_ERR_SUCCESS != ret)
    {
        sdCard[ch].cardType = CARDTYPE_NOT_SD;
        return MSDC_FAILED;
    }

    sdCard[ch].cardType = CARDTYPE_MEM_SD;
    
    return MSDC_SUCCESS;
}

INT32 MsdcCardRescan() 
{
    INT32 ret = MSDC_FAILED;

    if(sdHost[ch].hostCap & HOST_CAP_SD_SUPPORT)
    {
        ret = MsdcCardRescanSD();
        if(MSDC_SUCCESS != ret)
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to Try to Rescan SD Card!\n");
        }
    }
    
    if((MSDC_FAILED == ret) &&
       (sdHost[ch].hostCap & HOST_CAP_MMC_SUPPORT))
    {
        ret = MsdcCardRescanMMC();
        if(MSDC_SUCCESS != ret)
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to Try to Rescan MMC Card!\n");
        } 	
    }

    if(MSDC_SUCCESS == ret)
    {
        ret = MsdcCardQuery();
        if(MSDC_SUCCESS != ret)
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to Try to Query Memory Card!\n");
        }
    }

#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
	if(sdCard[ch].cardType == CARDTYPE_MEM_SD)
    {
	    if((MSDC_SUCCESS == ret) &&
		   (HOST_OCR_ATTRIBUTE & S18R_MASK) &&
		   (sdCard[ch].cardCap & CARD_CAP_LOW_VOLTAGE_18V))
	    {
	        ret = cmd11_sd_voltage_switch();
	        if(MSDC_SUCCESS != ret)
	        {
	            MSDC_LOG(MSG_LVL_ERR, "Failed to Try to switch voltage(cmd11)!\n");
	        }

			if(MSDC_SUCCESS == ret)
			{
				ret = MsdcHostVoltageSwitch();
			    if(MSDC_SUCCESS != ret)
		        {
		            MSDC_LOG(MSG_LVL_ERR, "Failed to Try to switch voltage(host switch)!\n");
		        }
			}
	    }       
    }
#endif

    if(MSDC_SUCCESS == ret)
    {
		if (MSDC_SUCCESS != MsdcCardTypeChk()) 
	    {
	        MSDC_LOG(MSG_LVL_ERR, "%s: Failed to check the card type. %s line %d\n", __FUNCTION__, __FILE__, __LINE__);
	    }
    }
    
    return ret;
}

INT32 MsdcHandleCSD(UINT32 fgHandle)
{
    if(0 == fgHandle)
    {
        if(CMD_ERR_SUCCESS != cmd9_send_csd((VOID *)(&sdCard[ch].csd)))
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to Get CSD registers.\n");	
            return MSDC_FAILED;
        }	
    }	
    else
    {
        if(CMD_ERR_SUCCESS != cmd27_program_csd((VOID *)(&sdCard[ch].csd)))
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to Program CSD registers.\n");	
            return MSDC_FAILED;
        }	    	
    }
    
    return MSDC_SUCCESS;
}

UINT8 MsdcGetEXTCSD_BootPartition(UINT32 index)
{
    MsdcGetEXTCSD();
    
    if(179 == index)
    {
        return (UINT8)sdHost[ch].EXT_CSD[index];
    }
    else
    {
        MSDC_LOG(MSG_LVL_ERR, "The registers bit is invalid.\n");	
        return MSDC_SUCCESS;
    }
}

INT32 MsdcGetEXTCSD(VOID)
{
    UINT8 *pBuff;
    UINT32 buffLen;
    
    pBuff = (UINT8 *)(sdHost[ch].EXT_CSD);
    buffLen = ArraySize(sdHost[ch].EXT_CSD);
    
    if(sdHost[ch].dataMode > 1)
    {
        HalDmaBufferStart((VOID *)pBuff, buffLen, HAL_DMA_FROM_DEVICE);
    }
    
    if(sdHost[ch].dataMode == 3)
        MsdcDescriptorConfig(pBuff, buffLen);
  
    if(sdHost[ch].fgUpdateExtCsd == 0)
    {	
        if(CMD_ERR_SUCCESS != cmd8_mmc_send_ext_csd(pBuff))
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to Get EXT_CSD registers.\n");	
            return MSDC_FAILED;
        }
        
        sdHost[ch].fgUpdateExtCsd = 1;	
    }
    
    if(sdHost[ch].dataMode > 1)
    {
        HalDmaBufferDone((VOID *)pBuff, buffLen, HAL_DMA_FROM_DEVICE);
    }
    
    return MSDC_SUCCESS;	
}

INT32 MsdcSetEXTCSD(UINT32 index, UINT8 value)
{
    if(CMD_ERR_SUCCESS != cmd6_mmc_set_ext_csd(index, value))
    {
        MSDC_LOG(MSG_LVL_ERR, "Failed to Set EXT_CSD registers(%d, %02x).\n", index, value);	
        return MSDC_FAILED;
    }
        
    sdHost[ch].fgUpdateExtCsd = 0;	
    
    return MSDC_SUCCESS;	
}

INT32 MsdcGetSCR(VOID)
{
#if 0
    INT32 i = 0;
#endif
    
    if(CMD_ERR_SUCCESS != acmd51_sd_send_scr())
    {
        MSDC_LOG(MSG_LVL_ERR, "Failed to Get SD Configuration Register(SCR).\n");	
        return MSDC_FAILED;
    }	

#if 0
    Printf("SD SCR:\n");
    for(i = 0;i < 0x8;i++)
        Printf("%02X ", *((UINT8 *)(&sdCard[ch].memScr) + i));
	Printf("\n");
#endif
    return MSDC_SUCCESS;	
}

INT32 MsdcGetStatus(VOID *pBuff)
{
    if(CMD_ERR_SUCCESS != cmd13_send_status(pBuff))
    {
        MSDC_LOG(MSG_LVL_ERR, "Failed to Get card status(CSR).\n");	
        return MSDC_FAILED;
    }	
    
    return MSDC_SUCCESS;	
}

INT32 MsdcGetSSR(VOID *pBuff)
{
    if(CMD_ERR_SUCCESS != acmd13_send_sd_status(pBuff))
    {
        MSDC_LOG(MSG_LVL_ERR, "Failed to Get SD status(SSR).\n");	
        return MSDC_FAILED;
    }	
    
    return MSDC_SUCCESS;	
}

INT32 MsdcWPSeg(UINT64 wpAddr, UINT32 u4fgEn)
{
    if(1 == u4fgEn)
    {     	
        if(CMD_ERR_SUCCESS != cmd28_set_write_prot(wpAddr))
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to write protect special segment.\n");	
            return MSDC_FAILED;
        }	
    }
    else if(0 == u4fgEn)
    {
        if(CMD_ERR_SUCCESS != cmd29_clr_write_prot(wpAddr))
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to disable write protect special segment.\n");	
            return MSDC_FAILED;
        }	    	
    }
    		
    return MSDC_SUCCESS;				
}

INT32 MsdcGetWPStatus(UINT64 wpAddr, UINT32 *pWP, UINT64 *pWPType)
{
    if(CMD_ERR_SUCCESS != cmd30_send_write_prot(wpAddr, pWP))
    {
        MSDC_LOG(MSG_LVL_ERR, "Failed to Get card write protect status.\n");	
        return MSDC_FAILED;
    }	
    
    if(CMD_ERR_SUCCESS != cmd31_send_write_prot_type(wpAddr, pWPType))
    {
        MSDC_LOG(MSG_LVL_ERR, "Failed to Get card write protect type.\n");	
        return MSDC_FAILED;
    }	
    
    return MSDC_SUCCESS;	
}

INT32 MsdcSetBusWidth (UINT32 busWidth) 
{
    if(sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {
        if (CMD_ERR_SUCCESS != cmd6_mmc_set_bus_width(busWidth)) 
        {
                MSDC_LOG(MSG_LVL_ERR, "Set mmc card bus width to %d Failed!\n", busWidth);
            return MSDC_FAILED;
        }
    }
    else if(sdCard[ch].cardType == CARDTYPE_MEM_SD)
    {
        if (CMD_ERR_SUCCESS != acmd6_sd_set_bus_width(busWidth)) 
        {
            MSDC_LOG(MSG_LVL_ERR, "Set sd card bus width to %d Failed!\n", busWidth);
            return MSDC_FAILED;
        }
    }

    /* Modify MSDC Register Settings */
    if (1 == busWidth) 
    {
        MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~SDC_CFG_BW_MASK) | (0x00 <<  SDC_CFG_BW_SHIFT));
    }
    else if (4 == busWidth) 
    {
        MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~SDC_CFG_BW_MASK) | (0x01 <<  SDC_CFG_BW_SHIFT));
    }
    else if(8 == busWidth)
    {
        MSDC_WRITE32(SDC_CFG, (MSDC_READ32(SDC_CFG) & ~SDC_CFG_BW_MASK) | (0x02 <<  SDC_CFG_BW_SHIFT));
    }

    return MSDC_SUCCESS;
}

INT32 MsdcSwitchFuncSD(INT32 mode, INT32 group, UINT8 value, UINT8 *status)
{
    INT32 i4Ret = CMD_ERR_SUCCESS;
    UINT32 arg = 0;

    x_memset(status, 0x0, 0x40);
    
    /* System will be reset if initialized local buffer 
      *  is operated by HalDmaBufferStart.
      *  The alloced buffer is OK for this function
      */
    if(sdHost[ch].dataMode > 1)
    {
        HalDmaBufferStart((VOID *)status, 0x40, HAL_DMA_FROM_DEVICE);
    }

    if(sdHost[ch].dataMode == 3)
        MsdcDescriptorConfig(status, 0x40);

    /* check the switch funtion
      */
    arg = ((mode << 31) | 0x00FFFFFF);
    arg &= ~(0xF << (group * 4));
    arg |= (value << (group * 4));
    	
    i4Ret = cmd6_sd_switch_func(status, arg);
    if (CMD_ERR_SUCCESS != i4Ret) 
    {
        MSDC_LOG(MSG_LVL_ERR, "%s card's switch function Failed!\n", (mode)?"check":"do");
    }
      
    /* this operation must be done before any access to the buffer. 
      */
    if(sdHost[ch].dataMode > 1)
    {
        HalDmaBufferDone((VOID *)status, 0x40, HAL_DMA_FROM_DEVICE);
    }
    
    return i4Ret;
}

INT32 MsdcEnHSMode(UINT32 fgHSMode)
{
    UINT8 *status, *pTmp;
	  
    if(sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {
        if (CMD_ERR_SUCCESS != cmd6_mmc_set_speed_mode(fgHSMode)) 
        {
            MSDC_LOG(MSG_LVL_ERR, "Switch card's speed mode to %s Failed!\n", fgHSMode?"High":"Normal");
            return MSDC_FAILED;
        }
        sdHost[ch].fgUpdateExtCsd = 0;
    }
    else if(sdCard[ch].cardType == CARDTYPE_MEM_SD)
    {
        status = (UINT8 *)x_mem_alloc(0x40);

        pTmp = (UINT8 *)(&sdCard[ch].memScr);
        if((pTmp[0]&0x0F) < SCR_SPEC_VER_1)
        {
            MSDC_LOG(MSG_LVL_ERR, "card spec is under version 1.10!\n");
            return MSDC_FAILED;    
        }
        pTmp = (UINT8 *)(&sdCard[ch].csd);
        if(!(((pTmp[11]<<4)|(pTmp[10]>>4)) & (0x1<<10)))
        {
            MSDC_LOG(MSG_LVL_ERR, "command class 10 not supported!\n");
            return MSDC_FAILED;    
        }
    	  	  
        if (CMD_ERR_SUCCESS != MsdcSwitchFuncSD(0, 0, fgHSMode, status)) 
        {
            MSDC_LOG(MSG_LVL_ERR, "check card's switch function Failed!\n");
            return MSDC_FAILED;	
        }

        /* check high-speed mode is supported or not - bit[400~415]
             * and whether it is in busy state - bit[273]
             */
        if(!(status[13] & (0x1<<fgHSMode)))
        {
            MSDC_LOG(MSG_LVL_ERR, "card not support the speed mode %d!\n", fgHSMode);
            return MSDC_FAILED;	 	
        }
        if(status[29] & (0x1<<fgHSMode))
        {
            MSDC_LOG(MSG_LVL_ERR, "card's speed function %d is busy!!\n", fgHSMode);
            return MSDC_FAILED;	 	
        }
        
        /* Check the switch function
             ************************************************
             *  Do the switch function
             */
        
        if (CMD_ERR_SUCCESS != MsdcSwitchFuncSD(1, 0, fgHSMode, status)) 
        {
            MSDC_LOG(MSG_LVL_ERR, "set card's switch function %d Failed!\n", fgHSMode);
            return MSDC_FAILED;	
        }
    
        /* check high-speed or default mode is applied or not - bit[379~376]
            */
        if((status[16]&0xF) != fgHSMode)
        {
            MSDC_LOG(MSG_LVL_ERR, "card switch high speed mode failed(%02X)!\n", (status[16]&0xF));
            return MSDC_FAILED;	   	
        }
    }
    
    return MSDC_SUCCESS;	

}

INT32 MsdcEnDDRMode(UINT32 fgDDRMode)
{
    UINT8 tmpByte = 0;
    
    if(sdCard[ch].cardType == CARDTYPE_MEM_MMC)
    {
       if((sdHost[ch].busWidth == 1)&&
          (fgDDRMode))
       {
           MSDC_LOG(MSG_LVL_ERR, "ddr mode doesn't support 1-bit mode for emmc!\n");
           return MSDC_FAILED;
       }
       
        /* 0 = 1/4
             *   1 = 4/4
             *   2 = 8/4
             */
        tmpByte = sdHost[ch].busWidth/4;
        if(fgDDRMode)
        {
            tmpByte += 4;
        }

        return MsdcSetEXTCSD(183, tmpByte);        
    }
	else if(sdCard[ch].cardType == CARDTYPE_MEM_SD)
	{
        return MsdcEnHSMode(SPEED_MODE_DDR50);
	}

	return MSDC_SUCCESS;
}

INT32 MsdcMemRead (UINT64 addr, VOID *pBuff, UINT32 buffLen) 
{
    INT32 i4Ret, i4RetStop;
    UINT32 u4Tmp, status = 0;
    UINT32 *pu4TmpBuf = 0;
    UINT32 *pu4TmpBufAlign = 0;
    UINT32 u4Buff = (UINT32)(pBuff);

    if(sdHost[ch].dataMode > PIO_DATA_MODE)
    {
        HalDmaBufferStart((VOID *)pBuff, buffLen, HAL_DMA_FROM_DEVICE);
    }
    
    if(addr & (SDHC_BLK_SIZE - 1))
    	{
    		 MSDC_LOG(MSG_LVL_ERR, "yf disk address is not block align\n");
          return MSDC_FAILED;
    	}
    
    if(buffLen & (SDHC_BLK_SIZE - 1))
    	{
    		 MSDC_LOG(MSG_LVL_ERR, "yf read length is not block align\n");
          return MSDC_FAILED;
    	}
    	
    	if(((UINT32)pBuff) & (32 - 1))
    	{
    		//MSDC_LOG(MSG_LVL_ERR, "yf mem addr is not cache line align\n");
    		pu4TmpBuf = (UINT32 *)x_mem_alloc(buffLen + 32);
    		if(!pu4TmpBuf)
    		{
    	         MSDC_LOG(MSG_LVL_ERR, "yf alloc mem len 0x%x failed\n",buffLen);
             return MSDC_FAILED;
    		}
    		pu4TmpBufAlign = (UINT32 *)(((UINT32)pu4TmpBuf & (~((UINT32)(32 - 1)))) + 32);
    		pBuff = pu4TmpBufAlign;
    	}
    if(sdHost[ch].dataMode == 3)
        MsdcDescriptorConfig(pBuff, buffLen);
    
    u4Tmp = (UINT32)(addr - (addr & (~(SDHC_BLK_SIZE - 1)))) + buffLen;

    MSDC_LOG(MSG_LVL_INFO, "MsdcMemRead : addr: %08X%08X, bufflen: %08X, actual len: %08X\n", 
                                         (UINT32)(addr>>32), (UINT32)(addr), buffLen, u4Tmp);
    //MsdcSampleEdgeInit(1);

    if (u4Tmp <= SDHC_BLK_SIZE) 
    {
        // CMD17 Single block read
        i4Ret = cmd17_read_single_block(addr, pBuff, buffLen);
        if (CMD_ERR_SUCCESS != i4Ret) 
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to read single block with address 0x%08X%08X buffLen 0x%08X\n", 
                                                            (UINT32)(addr>>32), (UINT32)(addr), buffLen);
         	#ifndef CC_HS200_CALIBRATION_SUPPORT   // tyler.zhang mark it for calibration                                                  
            //MsdcDumpRegister();
            #endif
            if(pu4TmpBufAlign)
            {
                x_mem_free(pu4TmpBuf);    	
                pu4TmpBuf = 0;
                pu4TmpBufAlign = 0;
            }
            return i4Ret;
        }
    }
    else 
    {
        if((sdHost[ch].hostAttr & MSDC_DATA_PRE_DEF_MASK) &&
           (!(sdHost[ch].hostAttr & MSDC_DATA_END_AUTO_MASK)))
        {
            i4RetStop = cmd23_set_block_count(buffLen>>SDHC_BLK_SIZE_ORDER);
            if (CMD_ERR_SUCCESS != i4RetStop) 
            {
                MSDC_LOG(MSG_LVL_ERR, "%s Failed to send hand pre-defined block count\n", __FUNCTION__);
                if(pu4TmpBufAlign)
                {
                    x_mem_free(pu4TmpBuf);    	
                    pu4TmpBuf = 0;
                    pu4TmpBufAlign = 0;
                }
                return i4RetStop;
            }
        }
        
        // CMD18 Multiple block read
        i4Ret = cmd18_read_multiple_block(addr, pBuff, buffLen);
        if (CMD_ERR_SUCCESS != i4Ret) 
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to read multiple blocks with address 0x%08X%08X buffLen 0x%08X\n", 
                                                               (UINT32)(addr>>32), (UINT32)(addr), buffLen);
            #ifndef CC_HS200_CALIBRATION_SUPPORT   // tyler.zhang mark it for calibration  
            //MsdcDumpRegister();
			#endif
            i4RetStop = cmd12_stop_transmission();
            // Error Handling process : sw should manually add stop command when multi-block read Failed
            if (CMD_ERR_SUCCESS != i4RetStop) 
            {
            	   if(pu4TmpBufAlign)
                {
                    x_mem_free(pu4TmpBuf);    	
                    pu4TmpBuf = 0;
                    pu4TmpBufAlign = 0;
                }
                MSDC_LOG(MSG_LVL_ERR, "%s Failed to send hand stop\n", __FUNCTION__);
                return i4RetStop;
            }
            if(pu4TmpBufAlign)
            {
                x_mem_free(pu4TmpBuf);    	
                pu4TmpBuf = 0;
                pu4TmpBufAlign = 0;
            }
            return i4Ret;
        }

        if((!(sdHost[ch].hostAttr & MSDC_DATA_PRE_DEF_MASK)) &&
           (!(sdHost[ch].hostAttr & MSDC_DATA_END_AUTO_MASK)))
        {
            i4RetStop = cmd12_stop_transmission();
            if (CMD_ERR_SUCCESS != i4RetStop) 
            {
                MSDC_LOG(MSG_LVL_ERR, "%s Failed to send hand stop\n", __FUNCTION__);
                if(pu4TmpBufAlign)
                {
                    x_mem_free(pu4TmpBuf);    	
                    pu4TmpBuf = 0;
                    pu4TmpBufAlign = 0;
                }
                return i4RetStop;
            }
        }
    }

    if(sdHost[ch].dataMode > PIO_DATA_MODE)
    {
        HalDmaBufferDone((VOID *)pBuff, buffLen, HAL_DMA_FROM_DEVICE);
    }    
    
    do
    {
        if(MSDC_SUCCESS != MsdcGetStatus(&status))
        {
            MSDC_LOG(MSG_LVL_ERR, "%s Failed to get card status\n", __FUNCTION__);	
        }
        MSDC_LOG(MSG_LVL_INFO, "(R)Card Status: %08X\n", status);
    }while(((status>>9)&0x0F) != 0x4);
    
#if defined(CC_MTD_ENCRYPT_SUPPORT) 
    if(MSDCEXT_Partition_Encrypted((UINT32)(addr>>9)))
    { 
        i4Ret = MSDCEXT_AES_Decryption((UINT32)pBuff, (UINT32)pBuff, buffLen);
        if(MSDC_SUCCESS != i4Ret)
        {
            MSDC_LOG(MSG_LVL_ERR, "AES decryption failed\n");  	
        }
    }
#endif
    if(pu4TmpBufAlign)
    {
        x_memcpy((VOID *)u4Buff, (VOID *)(pu4TmpBufAlign), buffLen);    
        x_mem_free(pu4TmpBuf);    	
        pu4TmpBuf = 0;
        pu4TmpBufAlign = 0;
    }

    return i4Ret;
}

INT32 MsdcMemWrite(UINT64 addr, VOID *pBuff, UINT32 buffLen)
{
    INT32 i4Ret, i4RetStop;
    UINT32 status = 0;
    VOID * destBuff = 0;
    
#if defined(CC_MTD_ENCRYPT_SUPPORT)
    if(MSDCEXT_Partition_Encrypted((UINT32)(addr>>9)))
    { 
    	destBuff = (VOID *)x_mem_alloc(buffLen);
        i4Ret = MSDCEXT_AES_Encryption((UINT32)pBuff, (UINT32)destBuff, buffLen);
        if(MSDC_SUCCESS != i4Ret)
        {
            MSDC_LOG(MSG_LVL_ERR, "AES Encryption failed\n"); 
            x_mem_free(destBuff);
            return i4Ret; 	
        }
    }
    else
    {
    	destBuff = pBuff;
    }
#else
destBuff = pBuff;
#endif

    if(sdHost[ch].dataMode > 1)
    {
        HalDmaBufferStart((VOID *)destBuff, buffLen, HAL_DMA_TO_DEVICE);
    }
    
    if(sdHost[ch].dataMode == 3)
        MsdcDescriptorConfig((VOID *)destBuff, buffLen);

    MSDC_LOG(MSG_LVL_INFO, "MsdcMemWrite : addr: %08X%08X, bufflen: %08X, actual len: %08X\n", 
                                         (UINT32)(addr>>32), (UINT32)(addr), buffLen, buffLen);

	//MsdcSampleEdgeInit(2);

    if (buffLen <= SDHC_BLK_SIZE) 
    {
        // CMD24 Single block write
        i4Ret = cmd24_write_single_block(addr, (VOID *)destBuff, buffLen);
        if (CMD_ERR_SUCCESS != i4Ret) 
        {
            MSDC_LOG(MSG_LVL_ERR, "%s Failed to write single block with address 0x%08X%08X buffLen 0x%08X\n", 
                                                                (UINT32)(addr>>32), (UINT32)(addr), buffLen);
            #ifndef CC_HS200_CALIBRATION_SUPPORT   // tyler.zhang mark it for calibration                                                        
            //MsdcDumpRegister();
            #endif
            #if defined(CC_MTD_ENCRYPT_SUPPORT)
            if(MSDCEXT_Partition_Encrypted((UINT32)(addr>>9)))
            x_mem_free(destBuff);
            #endif
            return i4Ret;
        }
    }
    else 
    {
        if((sdHost[ch].hostAttr & MSDC_DATA_PRE_DEF_MASK) &&
           (!(sdHost[ch].hostAttr & MSDC_DATA_END_AUTO_MASK)))
        {
            i4RetStop = cmd23_set_block_count(buffLen>>SDHC_BLK_SIZE_ORDER);
            if (CMD_ERR_SUCCESS != i4RetStop) 
            {
                MSDC_LOG(MSG_LVL_ERR, "%s Failed to send hand pre-defined block count\n", __FUNCTION__);
            	#if defined(CC_MTD_ENCRYPT_SUPPORT)
            	if(MSDCEXT_Partition_Encrypted((UINT32)(addr>>9)))
            	x_mem_free(destBuff);
            	#endif
                return i4RetStop;
            }
        }
    
        // CMD25 Multiple block write
        i4Ret = cmd25_write_multiple_block(addr, (VOID *)destBuff, buffLen);
        if (CMD_ERR_SUCCESS != i4Ret) 
        {
            MSDC_LOG(MSG_LVL_ERR, "Failed to write multiple blocks with address 0x%08X%08X buffLen 0x%08X\n", 
                                                                (UINT32)(addr>>32), (UINT32)(addr), buffLen);
            #ifndef CC_HS200_CALIBRATION_SUPPORT   // tyler.zhang mark it for calibration                                                                                                                           
            //MsdcDumpRegister();
			#endif
            i4RetStop = cmd12_stop_transmission();
            // Error Handling process : sw should manually add stop command when multi-block read Failed
            if (CMD_ERR_SUCCESS != i4RetStop) 
            {
                MSDC_LOG(MSG_LVL_ERR, "%s Failed to send auto stop\n", __FUNCTION__);
             	#if defined(CC_MTD_ENCRYPT_SUPPORT)
             	if(MSDCEXT_Partition_Encrypted((UINT32)(addr>>9)))
            	x_mem_free(destBuff);
           		#endif
                return i4RetStop;
            }
            #if defined(CC_MTD_ENCRYPT_SUPPORT)
            if(MSDCEXT_Partition_Encrypted((UINT32)(addr>>9)))
            x_mem_free(destBuff);
            #endif
            return i4Ret;
        }

        if((!(sdHost[ch].hostAttr & MSDC_DATA_PRE_DEF_MASK)) &&
           (!(sdHost[ch].hostAttr & MSDC_DATA_END_AUTO_MASK)))
        {
            i4RetStop = cmd12_stop_transmission();
            if (CMD_ERR_SUCCESS != i4RetStop) 
            {
                MSDC_LOG(MSG_LVL_ERR, "%s Failed to send auto stop\n", __FUNCTION__);
                #if defined(CC_MTD_ENCRYPT_SUPPORT)
                if(MSDCEXT_Partition_Encrypted((UINT32)(addr>>9)))
                x_mem_free(destBuff);
            	#endif
                return i4RetStop;
            }
        }
    }

    if(sdHost[ch].dataMode > 1)
    {
        HalDmaBufferDone((VOID *)destBuff, buffLen, HAL_DMA_TO_DEVICE);
    }
    
    do
    {
        if(MSDC_SUCCESS != MsdcGetStatus(&status))
        {
            MSDC_LOG(MSG_LVL_ERR, "%s Failed to get card status\n", __FUNCTION__);	
        }
        MSDC_LOG(MSG_LVL_INFO, "(W)Card Status: %08X\n", status);
    }while(((status>>9)&0x0F) != 0x4);
    #if defined(CC_MTD_ENCRYPT_SUPPORT)
    if(MSDCEXT_Partition_Encrypted((UINT32)(addr>>9)))
    x_mem_free(destBuff);
    #endif
    return i4Ret;
}

INT32 MsdcMemErase(UINT64 startAddr, UINT64 endAddr, UINT32 eraseType)
{
    INT32 i4Ret;
    UINT32 u4Arg;
    UINT32 status = 0;
    
    u4Arg = 0x00000000;
    switch(eraseType)
    {
    case 0:
    	  u4Arg = 0x00000000;
        break;      
    case 1:
    	  u4Arg = 0x00000001;
    	  break;
    case 2:
        u4Arg = 0x80000000;
        break;
    case 3:
        u4Arg = 0x80000001;
    	  break;
    case 4:
        u4Arg = 0x80008000;
    	  break;
    default:
    	  u4Arg = 0x00000000;
        break;    		
    }

    i4Ret = cmd35_erase_wr_blk_start(startAddr);
    if (CMD_ERR_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "%s Failed to set erase start address\n", __FUNCTION__);
        return i4Ret;
    }

    i4Ret = cmd36_erase_wr_blk_end(endAddr);
    if (CMD_ERR_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "%s Failed to set erase end address\n", __FUNCTION__);
        return i4Ret;
    }

    i4Ret = cmd38_erase(u4Arg);
    if (CMD_ERR_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "%s Failed to erase card\n", __FUNCTION__);
        return i4Ret;
    }
    
    do
    {
        if(MSDC_SUCCESS != MsdcGetStatus(&status))
        {
            MSDC_LOG(MSG_LVL_ERR, "%s Failed to get card status\n", __FUNCTION__);	
        }
        MSDC_LOG(MSG_LVL_INFO, "(E)Card Status: %08X\n", status);
    }while(((status>>9)&0x0F) != 0x4);
	
    return i4Ret;
}

#endif
