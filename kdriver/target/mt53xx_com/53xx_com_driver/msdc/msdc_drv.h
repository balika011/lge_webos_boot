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
* [File]			msdc_drv.h
* [Version]			v1.0
* [Revision Date]	2011-05-04
* [Author]			Shunli Wang, shunli.wang@mediatek.inc, 82896, 2011-05-04
* [Description]
*	MSDC Driver Header File
* [Copyright]
*	Copyright (C) 2011 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/

#ifndef _MSDC_DRV_H_
#define _MSDC_DRV_H_

#include "msdc_host_hw.h"
#include "msdc_slave_hw.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#if defined(CC_MT5396) || defined(CONFIG_ARCH_MT5396) || \
    defined(CC_MT5368) || defined(CONFIG_ARCH_MT5368) || \
    defined(CC_MT5389) || defined(CONFIG_ARCH_MT5389)
#define MSDC_CLK_TARGET   54, 48, 43, 40, 36, 30, 27, 24, 18, 13, 0
#define MSDC_CLK_SRC_VAL  2, 3, 4, 8, 5, 7, 0, 9, 6, 1, 0
#define MSDC_CLK_MODE_VAL 1, 1, 1, 0, 1, 0, 2, 0, 0, 0
#define MSDC_CLK_DIV_VAL  0, 0, 0, 0, 0, 1, 0, 2, 0, 16
#define MSDC_CLK_DRV_VAL  60, 60, 57, 25, 23, 16, 16, 7, 7, 0
#define MSDC_CLK_SAMP_VAL 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x0, 0x0, 0x0, 0x0
#define MSDC_CLK_IDX_MAX  11
#define MSDC_CLK_S_REG1   0xF000D2F8
#define MSDC_CLK_S_REG0   0xF000D264
#define MSDC_CLK_H_REG1   0xF000D2F8
#define MSDC_CLK_H_REG0   0xF000D264
#define MSDC_CLK_GATE_BIT (0x1<<7)
#define MSDC_CLK_SEL_MASK (0x0F<<0)
#define MSDC_HIGH_CLK_IDX 1
#define MSDC_NORM_CLK_IDX 7
#define MSDC_INIT_CLK_IDX 10
   
#elif defined(CC_MT5880) || defined(CONFIG_ARCH_MT5880) || \
      defined(CC_MT5860) || defined(CONFIG_ARCH_MT5860) || \
      defined(CC_MT5881) || defined(CONFIG_ARCH_MT5881) || \
      defined(CC_MT5398) || defined(CONFIG_ARCH_MT5398)
#define MSDC_CLK_TARGET   54, 48, 43, 40, 36, 30, 27, 24, 18, 13, 0
#define MSDC_CLK_SRC_VAL  1, 2, 8, 6, 9, 5, 3, 7, 10, 4, 0
#define MSDC_CLK_MODE_VAL 1, 1, 1, 0, 1, 0, 2, 0, 0, 0
#define MSDC_CLK_DIV_VAL  0, 0, 0, 0, 0, 1, 0, 2, 0, 16
#define MSDC_CLK_DRV_VAL  60, 60, 57, 25, 23, 16, 16, 7, 7, 0
#define MSDC_CLK_SAMP_VAL 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x0, 0x0, 0x0, 0x0
#define MSDC_CLK_IDX_MAX  11
#define MSDC_CLK_S_REG1   0xF000D380
#define MSDC_CLK_S_REG0   0xF000D32C
#define MSDC_CLK_H_REG1   0xF000D384
#define MSDC_CLK_H_REG0   0xF000D3A0
#define MSDC_CLK_GATE_BIT (0x1<<7)
#define MSDC_CLK_SEL_MASK (0x0F<<0)
#define MSDC_HIGH_CLK_IDX 1
#define MSDC_NORM_CLK_IDX 7
#define MSDC_INIT_CLK_IDX 10
#elif  defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
       defined(CC_MT5861) || defined(CONFIG_ARCH_MT5861)
#define MSDC_CLK_TARGET   200, 144, 120, 100, 80, 50, 50, 25, 12, 0
#define MSDC_CLK_SRC_VAL   11,   5,   6,  11,  9,  8, 11, 11,  0, 0
#define MSDC_CLK_MODE_VAL   1,   1,   1,   0,  1,  1,  2,  0,  0, 0
#define MSDC_CLK_DIV_VAL    0,   0,   0,   0,  0,  0,  0,  2,  0, 16
#define MSDC_CLK_DRV_VAL    1,   1,   1,   1 , 1,  1,  1,  1,  1, 0
#define MSDC_CLK_SAMP_VAL 0x6, 0x6, 0x6, 0x6,0x6,0x6,0x6,0x0,0x0,0x0, 0x0
#define MSDC_CLK_IDX_MAX  10
#define MSDC_CLK_S_REG1   0xF000D380
#define MSDC_CLK_S_REG0   0xF000D32C
#define MSDC_CLK_H_REG1   0xF000D384
#define MSDC_CLK_H_REG0   0xF000D3A0
#define MSDC_CLK_GATE_BIT (0x1<<7)
#define MSDC_CLK_SEL_MASK (0x0F<<0)
#define MSDC_HIGH_CLK_IDX 6
#define MSDC_NORM_CLK_IDX 7
#define MSDC_INIT_CLK_IDX 9
#elif defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
       defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
#define MSDC_CLK_TARGET   200, 144, 120, 100, 80, 50, 50, 25, 12, 0
#define MSDC_CLK_SRC_VAL  11, 5, 6, 11, 9, 8, 11, 11, 0, 0
#define MSDC_CLK_MODE_VAL 1, 1, 1, 0, 1, 1, 2, 0, 0, 0
#define MSDC_CLK_DIV_VAL  0, 0, 0, 0, 0, 0, 0, 2, 0, 16
#define MSDC_CLK_DRV_VAL  60, 60, 60, 25, 23, 23, 16, 7, 7, 0
#define MSDC_CLK_SAMP_VAL 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x0, 0x0, 0x0, 0x0
#define MSDC_CLK_IDX_MAX  10
#define MSDC_CLK_S_REG1   0xF000D380
#define MSDC_CLK_S_REG0   0xF000D32C
#define MSDC_CLK_H_REG1   0xF000D384
#define MSDC_CLK_H_REG0   0xF000D3A0
#define MSDC_CLK_GATE_BIT (0x1<<7)
#define MSDC_CLK_SEL_MASK (0x0F<<0)
#define MSDC_HIGH_CLK_IDX 6
#define MSDC_NORM_CLK_IDX 7
#define MSDC_INIT_CLK_IDX 9
#endif


#define CMD0_RESET_LIMIT            (3)
#define MMC_CMD1_RETRY_LIMIT        (1500)
#define SDHC_BLK_SIZE               (512)
#define SDHC_BLK_SIZE_ORDER         (9)
#define MMC_DEF_RCA                 0x0001
#define SD_DEF_RCA                  0x0000   

// GPIO63
#if defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
#define MSDC_SD_VOLTAGE_PIN     43
#else
#define MSDC_SD_VOLTAGE_PIN     63
#endif
#define MSDC_SD_DETECT_PIN      76

//---------------------------------------------------------------------------
// Basic definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define MSDC_RST_TIMEOUT_LIMIT_COUNT                        5         
#define MSDC_CLK_TIMEOUT_LIMIT_COUNT                        5 
#define MSDC_FIFOCLR_TIMEOUT_LIMIT_COUNT                    5   

#define MSDC_WAIT_SDC_BUS_TIMEOUT_LIMIT_COUNT               400                 
#define MSDC_WAIT_CMD_TIMEOUT_LIMIT_COUNT                   200                     
#define MSDC_WAIT_DATA_TIMEOUT_LIMIT_COUNT                  5000              
#define MSDC_WAIT_DATA_COMPLETE_TIMEOUT_LIMIT_COUNT         1000000//1000000             

#define MSDC_WAIT_BTSTS_1_TIMEOUT_LIMIT_COUNT               20                     
#define MSDC_WAIT_BTSTS_0_TIMEOUT_LIMIT_COUNT               1000                   
#define MSDC_WAIT_EMMC_ACK_TIMEOUT_LIMIT_COUNT              60                 
#define MSDC_WAIT_EMMC_DATA_TIMEOUT_LIMIT_COUNT             1100 

#define EMMC_READ_CARD_FAIL                                 1
#define EMMC_MSDC_INIT_FAIL                                 2
#define EMMC_IDENT_CARD_FAIL                                3
#define EMMC_SUCCESS                                        0

//---------------------------------------------------------------------------
// MSDC error retry limitation
//---------------------------------------------------------------------------

#define MSDC_LOAD_MMC_IMAGE_RETRY_LIMIT               10
#define MSDC_SYS_INIT_RETRY_LIMIT                      3
#define MSDC_IDENTIFY_CARD_RETRY_LIMIT                10
#define MSDC_READ_CARD_RETRY_LIMIT                     6
#define MSDC_READ_BOOTLDR_DATA_RETRY_LIMIT             8

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

#define MSDC_WRITE32(addr, value)      (*(volatile unsigned int *)(addr)) = (value)
#define MSDC_READ32(addr)              (*(volatile unsigned int *)(addr))

#define MSDC_SETBIT(addr, dBit)        MSDC_WRITE32(addr, MSDC_READ32(addr) | (dBit))
#define MSDC_CLRBIT(addr, dBit)        MSDC_WRITE32(addr, MSDC_READ32(addr) & (~(dBit)))

#define MSDC_BITVAL(addr, dBit)        ((MSDC_READ32(addr)>>dBit)&0x1)

#define ArraySize(a)              (sizeof(a)/sizeof(a[0]))
#define MAX_NUM(a, b)             ((a)>(b)?(a):(b))
#define MIN_NUM(a, b)             ((a)>(b)?(b):(a))
	
#define RESPONSE_NO			          (0)
#define RESPONSE_R1			          (1)
#define RESPONSE_R2			          (2)
#define RESPONSE_R3			          (3)
#define RESPONSE_R4			          (4)
#define RESPONSE_R5			          (5)
#define RESPONSE_R6			          (6)
#define RESPONSE_R7			          (7)
#define RESPONSE_R1B		          (8)

#define MSDC_SUCCESS                  (int)(0)
#define MSDC_FAILED                   (int)(-1)

#define CMD_ERR_SUCCESS               (MSDC_SUCCESS)
#define CMD_ERR_FAILED                (MSDC_FAILED)

#define CMD_ERR_NO_RESP               (0x01 << 0)
#define CMD_ERR_RESP_CRCERR           (0x01 << 1)
#define CMD_ERR_WAIT_CMD_TO           (0x01 << 2)

#define CMD_ERR_DATTO                 (0x01 << 3)
#define CMD_ERR_DATA_CRCERR           (0x01 << 4)
#define CMD_ERR_WAIT_DATATO           (0x01 << 5)
#define CMD_ERR_DATA_FAILED           (0x01 << 6)

#define ERR_CMD_FAILED                (0x01 << 1)
#define ERR_DAT_FAILED                (0x01 << 4)

//---------------------------------------------------------------------------
// Log system declaration
//---------------------------------------------------------------------------

#define MSG_LVL_OFF						(0)
#define MSG_LVL_FATAL					(1)
#define MSG_LVL_ERR						(2)
#define MSG_LVL_WARN					(3)
#define MSG_LVL_TITLE					(4)
#define MSG_LVL_INFO					(5)
#define MSG_LVL_CMD						(6)
#define MSG_LVL_DBG						(7)
#define MSG_LVL_TRACE					(8)

#ifndef CC_UBOOT
#ifndef CC_HS200_CALIBRATION_SUPPORT   // tyler.zhang mark it for calibration
#define MSG_LVL_DISPLAY       (4)
#else
#define MSG_LVL_DISPLAY       (0)
#endif
#define MSDC_LOG(level, formatStr...) \
if (level <= MSG_LVL_DISPLAY) {\
    Printf(formatStr);\
}
#else
#ifndef CC_HS200_CALIBRATION_SUPPORT   // tyler.zhang mark it for calibration
#define MSG_LVL_DISPLAY       (4)
#else
#define MSG_LVL_DISPLAY       (0)
#endif
#define MSDC_LOG(level, formatStr...) \
if (level <= MSG_LVL_DISPLAY) {\
    printf(formatStr);\
}
#endif

//---------------------------------------------------------------------------
// Struct declaration
//---------------------------------------------------------------------------

typedef struct __sd_cmd_t__ 
{
    UINT8 idx;
    UINT32 arg;
    UINT32 resp[4];		// Maximum Length Reserved
    UINT32 autoStopResp;	
    UINT32 dataMode;
    UINT32 buffLen;
    UINT32 flags;
    VOID *pBuff;				// Point to data buffer in PIO and basic DMA mode
} sd_cmd_t;

typedef struct __msdc_env_s__ 
{
    UINT8 fgHost;
    UINT8 cid[16];
    UINT8 rca[2];
    UINT8 csd[16];
    UINT8 ocr[4];
    //UINT8 ext_csd_185;
    //UINT8 ext_csd_192;
    //UINT8 ext_csd_196;
    //UINT8 ext_csd_212;
    //UINT8 ext_csd_213;
    //UINT8 ext_csd_214;
    //UINT8 ext_csd_215;
    UINT8 ext_csd_rsv[25];
} msdc_env_t;

typedef struct __sdhost_s__ 
{
    UINT32 fgValid;	
 
#define HOST_CAP_MMC_SUPPORT             (0x1<<0)
#define HOST_CAP_SD_SUPPORT              (0x1<<1)
#define HOST_CAP_SDIO_SUPPORT            (0x1<<2)
#define HOST_CAP_LOW_VOLTAGE_18V_SUPPORT (0x1<<3)
    UINT32 hostCap;

#define MSDC_DATA_PRE_DEF_MASK      (0x1<<0)
#define MSDC_DATA_END_AUTO_MASK     (0x1<<1) 
#define MSDC_DATA_DDR_MODE_MASK     (0x1<<2) 
    UINT32 hostAttr;
	
    UINT32 busWidth;
    UINT32 blkLen;

#define SPEED_MODE_DS               (0x0)
#define SPEED_MODE_HS               (0x1)
#define SPEED_MODE_HS200            (0x2)
#define SPEED_MODE_SDR50            (0x2)
#define SPEED_MODE_SDR104           (0x3)
#define SPEED_MODE_DDR50            (0x4) 
    UINT32 speedMode;

    // Select polling mode or interrupt mode 
    UINT32 polling;   

    // Host Data Mode 
    UINT32 dataMode;  

    // Host Bus Clock
    UINT32 maxClock;
    UINT32 curClock;

    // DMA Burst Size 
    UINT32 dmaBstSize;
    // Configurable Maximum Basic DMA Len     
    UINT32 maxBasicDmaLen;    

    // Accumulated interrupt vector
    volatile UINT32 accuVect;

    // EXT_CSD
    UINT8 EXT_CSD[512];
    BOOL fgUpdateExtCsd; 

	// Card identification flag
	BOOL fgCardIdent;
} sdhost_t;

#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)|| \
    defined(CC_MT5861) || defined(CONFIG_ARCH_MT5861)
typedef struct
{
    UINT8 acName[32];
    UINT32 u4ID1;
    UINT32 u4ID2;
    UINT32 DS26Sample;
	UINT32 DS26Delay;
    UINT32 HS52Sample;
    UINT32 HS52Delay;
    UINT32 DDR52Sample;
    UINT32 DDR52Delay;
    UINT32 HS200Sample;
    UINT32 HS200Delay;
} EMMC_FLASH_DEV_T;
#else
typedef struct
{
    char acName[32];
    UINT32 u4ID1;
    UINT32 u4ID2;
    UINT32 u4Sample;
} EMMC_FLASH_DEV_T;
#endif

//---------------------------------------------------------------------------
// export function declaration
//---------------------------------------------------------------------------

#ifndef CC_UBOOT
EXTERN INT32 MsdcInit (VOID);
EXTERN INT32 MsdcSDPowerSwitch(INT32 i4V);
EXTERN VOID MsdcInitHostConfig(VOID);
EXTERN VOID MsdcContinueClock (UINT32 i4ContinueClock);
EXTERN INT32 MsdcSetClkFreq (UINT32 clkFreq);
EXTERN INT32 MsdcFindHynixName (void);
EXTERN INT32 MsdcSetBusWidth (UINT32 busWidth);
EXTERN INT32 MsdcEnHSMode(UINT32 fgHSMode);
EXTERN INT32 MsdcEnDDRMode(UINT32 fgDDRMode);
EXTERN INT32 MsdcCardRescan (VOID);
EXTERN INT32 MsdcMemRead (UINT64 addr, VOID *pBuff, UINT32 buffLen);
EXTERN INT32 MsdcMemWrite(UINT64 addr, VOID *pBuff, UINT32 buffLen);
EXTERN INT32 MsdcMemErase(UINT64 startAddr, UINT64 endAddr, UINT32 eraseType);
EXTERN INT32 MsdcWPSeg(UINT64 wpAddr, UINT32 u4fgEn);
EXTERN INT32 MsdcHandleCSD(UINT32 fgHandle);
EXTERN INT32 MsdcGetEXTCSD(VOID);
EXTERN UINT8 MsdcGetEXTCSD_BootPartition(UINT32 index);
EXTERN INT32 MsdcSetEXTCSD(UINT32 index, UINT8 value);
EXTERN INT32 MsdcGetStatus(VOID *pBuff);
EXTERN INT32 MsdcGetWPStatus(UINT64 wpAddr, UINT32 *pWP, UINT64 *pWPType);
EXTERN INT32 MsdcGetSCR(VOID);
EXTERN INT32 MsdcGetSSR(VOID *pBuff);
EXTERN void MsdcFindDevName(UINT32 *pCID);
EXTERN INT32 MsdcSendCmd (sd_cmd_t *pCmd);
EXTERN INT32 cmd0_go_idle_state(UINT32 arg);
EXTERN INT32 cmd3_send_relative_addr(VOID);
EXTERN INT32 cmd7_select_card (UINT32 sel);
EXTERN INT32 cmd11_sd_voltage_switch(VOID);
EXTERN INT32 cmd19_sd_send_tuning_pattern(VOID);
EXTERN INT32 cmd21_mmc_send_tuning_block(VOID *pBuff, UINT32 buffLen);

#endif

#endif // #ifndef _MSDC_DRV_H_
