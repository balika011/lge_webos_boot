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
 * $RCSfile: loader_upgrade.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file loader_upgrade.c
 *  This file provides upgrade check and execution mechanism functions.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "loader_if.h"
#include "drvcust_if.h"
#include "musb_if.h"
#ifdef SSUSB_LOADER
#include "musb3_if.h"
#endif
#if defined(CC_MSDC_ENABLE) || defined(CC_SDMMC_SUPPORT)
#include "msdc_if.h"
#endif
#include "x_pdwnc.h"
#include "x_pwr_ctrl.h"
#include "x_serial.h"

#ifdef CC_NAND_BOOT
#include "nand_if.h"
#endif
#include "nor_if.h"
#include "pdwnc_if.h"
#include "loader_imghdr.h"
#include "c_model.h"
#include "x_hal_5381.h"
#include "eeprom_if.h"

#ifdef LOADER_USB_UPGRADE
#include "x_fm.h"
#include "x_fm_fat.h"
#include "fm_ufs.h"
#include "x_dlm.h"
#include "x_timer.h"
#include "x_ldr_env.h"
#include "x_bim.h"
#endif

#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif

#ifdef ENABLE_USB2UART_FUNC//wangle
#include "x_pdwnc.h"
#include "x_pinmux.h"
#include "x_gpio.h"
#endif

#ifdef CC_MTK_AUTO_TEST
UINT8 EepromSuffixFlag;
#endif
//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

EXTERN void MUSB_NoneRunBackground(void);
EXTERN INT32 handle_init (UINT16   ui2_num_handles,
                          VOID**   ppv_mem_addr,
                          SIZE_T*  pz_mem_size);
#ifdef LOADER_USB_UPGRADE
#ifdef CC_DEF_PART_VER_CHK_FUNC
static void LOADER_DefUpgradeDlmGetTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer);
static void LOADER_DefUpgradeDlmSetTagVerCbFunc(UCHAR* ucTagName, CHAR *szUpgVer);
static INT32 LOADER_DefUpgradeDlmCmpTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer, CHAR *szUpgVer);
static void write_new_pkg_part_version(CHAR* szFileNameWithPath);
#endif
#endif
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef enum
{
    NAND = 0,
    NOR = 1,
} LDR_UPG_FLASH_TYPE_T;

typedef struct _LDR_UPG_CFG_
{
    CHAR    szTagName[5];
    LDR_UPG_FLASH_TYPE_T    eFlashType;
#ifdef CC_ONE_IMG_FOR_MULTI_NAND
	BOOL 	fgNandMatch;
#endif
    UINT8     u1PartitionID;
#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
    BOOL      fgSupportRecipeUpgarde;
    UINT8     u1DataAmounts;
    UINT32    u4DataSize;
#endif
} LDR_UPG_CFG_T;

#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
    #ifdef CC_MAPLE_LOADER_UPGRADE_NEW_RECIPE_SPEC
typedef struct _LDR_RCIP_UPG_CFG_
{
    CHAR       szTagName[5];
    UINT8      u1DataCaseIndex;
} LDR_RCIP_UPG_CFG_T;
    #else
typedef struct _LDR_RCIP_UPG_CFG_
{
    CHAR     szModelName[33];
    UINT32   u4ModelIDMask;
    UINT8    u1PartitionID[32];
    UINT8    u1DataOffsetIdx[32];
    UINT32   u4DataSkipCounter[32];
    UINT32   u4DataOfstToWrite[32];
} LDR_RCIP_UPG_CFG_T;
    #endif
#endif

#define DUAL_BANKS_SHARE_ONE_IMG
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef CC_MAPLE_LOADER_UPGRADE_VERSION_CTRL_FLOW
#define LOADER_UPGRADE_CFG_PKG_VERSION_INFO_ADDR     0xB420 //from L to H
#define LOADER_UPGRADE_CFG_PKG_VERSION_INFO_SIZE     0x40
#define LOADER_UPGRADE_CFG_PKG_UID_ADDR         LOADER_UPGRADE_CFG_PKG_VERSION_INFO_ADDR //from H to L
#define LOADER_UPGRADE_CFG_UID_SIZE     12
#define LOADER_UPGRADE_CFG_PKG_EMID_ADDR        LOADER_UPGRADE_CFG_PKG_UID_ADDR+LOADER_UPGRADE_CFG_UID_SIZE+1 //from H to L
#define LOADER_UPGRADE_CFG_EMID_SIZE     8
#define LOADER_UPGRADE_CFG_PKG_VERSION_ADDR     LOADER_UPGRADE_CFG_PKG_EMID_ADDR+LOADER_UPGRADE_CFG_EMID_SIZE+1 //from L to H
#define LOADER_UPGRADE_CFG_PKG_VERSION_SIZE     4
#define LOADER_UPGRADE_CFG_PKG_BROADCAST_SIZE   4
#define LOADER_UPGRADE_CFG_PKG_HMAC_ADDR         LOADER_UPGRADE_CFG_PKG_VERSION_INFO_ADDR+LOADER_UPGRADE_CFG_PKG_VERSION_INFO_SIZE //from H to L
#define LOADER_UPGRADE_CFG_PKG_HMAC_SIZE         0x20

#define LOADER_UPGRADE_PNLSTR_RECIP_ADDR         0xB400 //from H to L
#define LOADER_UPGRADE_PNLSTR_CACHE_ADDR         0xB410 //from H to L
#define LOADER_UPGRADE_PNLSTR_SIZE     16
#endif
#ifdef LOADER_USB_UPGRADE
#ifndef CC_USB_DISABLE
static UINT32 _u4Upgraded = 0;
static UINT64 _u8FileSize = 0;
static CHAR   _szFileName[64];
static UINT32 _u4BufSize;
#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
static UINT32 _u4ImageStartAddr;
#else
static UINT64 _u8ImageStartAddr;
#endif
#ifdef DUAL_BANKS_SHARE_ONE_IMG
static UINT32 _u4ImageOffset;
static UCHAR _s_last_tag[5];
#endif
#ifdef CC_DEF_PART_VER_CHK_FUNC
static INT32 _i1SavePartVersion = 0;
#endif
static UINT8 _u1CurrentPartitionId = 0xFF;
static UINT32 *_pu4NorBuffer = NULL;
#if 1//def CC_MAPLE_LOADER_UPGRADE_FLOW
#define MAX_UPG_CFG_NUM 64
#else
#define MAX_UPG_CFG_NUM 32
#endif
static LDR_UPG_CFG_T _arUpgCfg[MAX_UPG_CFG_NUM];
static UINT32 _u4NoOfTags;
#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
#define MAX_UPG_MODEL_NUM 128
static BOOL _fgPartitionFirstBlock = TRUE;
static BOOL _fgRecipeWriteFromFirstByte = TRUE;
static BOOL _fgRecipUpgradeDone = FALSE;
static BOOL _fgRecipUpgradeFullWrite = FALSE;
static BOOL _fgRecipUpgradeMode = FALSE;
#ifdef CC_MAPLE_LOADER_UPGRADE_NEW_RECIPE_SPEC
#define RECIPE_COND_DATA_ADDR  0x10010
static LDR_RCIP_UPG_CFG_T _arRcipUpgCfg[MAX_UPG_CFG_NUM];
static UINT8 _u1CurrentRcipCondData[32] = {0};
static UINT32 _u4NoOfRecipeTags=0;
#else
static LDR_RCIP_UPG_CFG_T _arRcipUpgCfg[MAX_UPG_MODEL_NUM];
#endif
#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW_DEBUG
static UINT32 _u4NoOfRecipeModels=0;
static UINT8 _u1PartitionArrayId=32;
static UINT32 u4PartIdx = 0;
#endif
static UINT8 _u1CurrentModelIdx = 0;
static UINT32 ui4DataWritenSize=0;
static BOOL _u1RecipeIDMatch=TRUE;
#endif
#ifdef CC_MAPLE_LOADER_UPGRADE_VERSION_CTRL_FLOW
static BOOL _fgForceUpgrade=TRUE;
static BOOL _fgVersionInit=FALSE;
static BOOL _fgVerionCtrlInfoExist=TRUE;
static CHAR szUpdateID[LOADER_UPGRADE_CFG_UID_SIZE+1];
static CHAR szEncryptModelID[LOADER_UPGRADE_CFG_EMID_SIZE+1];
//for pkg version info
static CHAR szVersionID[LOADER_UPGRADE_CFG_PKG_VERSION_SIZE+1];
static CHAR szBroadcastID[LOADER_UPGRADE_CFG_PKG_BROADCAST_SIZE+1];
static UINT32 u4PackageVersion;
#endif

#ifdef CC_LDR_ATOMIC_MODE_SUPPORT
static UINT8 _u1UpgradeMode = 0; //0:default / 1: migration mode 
#endif

#if defined(CC_SECURE_BOOT) && defined(CC_LOADER_VERIFY_PKG)
static UINT32 _u4UsigCmd = USIG_VERIFY_FAIL;
#endif

//show logo when upgrade
#ifdef CC_SUPPORT_BL_DLYTIME_CUT
extern void vDrvTurnOnBacklt(void);
#endif

#endif // CC_USB_DISABLE
#endif // LOADER_USB_UPGRADE

#define USB_ROOT_PATH "/mnt/usb_0"
#define MSDC_ROOT_PATH "mnt/msdc_0"
#define BYTES_PER_READ 4096
#ifndef MAX_UPGRADE_IMAGE_FILE_SIZE
#define MAX_UPGRADE_IMAGE_FILE_SIZE 64 * 1024 * 1024
#endif
#define FIRMWARE_FLASH_OFFSET           LOADER_MAX_SIZE

#ifdef DEBUG
#undef DEBUG
//#define USB_UPGRADE_DEBUG
#endif

#ifdef USB_UPGRADE_DEBUG
static UCHAR* _pucFileBuf = NULL;
#endif // USB_UPGRADE_DEBUG

#ifdef USB_UPGRADE_DEBUG
#define DEBUG(fmt,args...)  Printf (fmt ,##args)
#else
#define DEBUG(fmt,args...)
#endif

//For Identify USB or SD/MMC Upgrade
static UINT32 _fgDeviceType = 0;

#ifdef CC_USB_UPGRADE_CHECK_FILE_VERSION
extern UINT32 StrToInt(const CHAR*);
#define LOADER_USB_UPGRADE_MAJOR_VERSION     0x2BA1
#define LOADER_USB_UPGRADE_MINOR_VERSION     0x2BA2
static UINT8 _u1FilenameVersionMajor = 0;
static UINT8 _u1FilenameVersionMinor = 0;
static void backup_new_filename_version(UINT8 u1Major, UINT8 u1Minor)
{
    _u1FilenameVersionMajor = u1Major;
    _u1FilenameVersionMinor = u1Minor;
}
static void write_new_filename_version(void)
{
    UNUSED(EEPROM_Write(LOADER_USB_UPGRADE_MAJOR_VERSION, (UINT32)(void *)&_u1FilenameVersionMajor,
                        sizeof(UINT8)));
    UNUSED(EEPROM_Write(LOADER_USB_UPGRADE_MINOR_VERSION, (UINT32)(void *)&_u1FilenameVersionMinor,
                        sizeof(UINT8)));
}
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef LOADER_USB_UPGRADE
#ifdef CC_MAPLE_LOADER_UPGRADE_VERSION_CTRL_FLOW
extern UINT32 StrToInt(const CHAR*);
extern UINT32 StrToHex(const CHAR* pszStr, UINT32 u4Len);

static void write_new_pkg_cfg_version(void)
{
    UINT32 i;
    CHAR puTmp='_';
	UINT8 arOnBoardVerInfo[LOADER_UPGRADE_CFG_PKG_VERSION_INFO_SIZE];
	UINT8 arVerInfoHmac[LOADER_UPGRADE_CFG_PKG_HMAC_SIZE];

	UNUSED(EEPROM_Write(LOADER_UPGRADE_CFG_PKG_UID_ADDR, 
					   (UINT32)(void *)&szUpdateID[0],LOADER_UPGRADE_CFG_UID_SIZE));

	UNUSED(EEPROM_Write((LOADER_UPGRADE_CFG_PKG_UID_ADDR+LOADER_UPGRADE_CFG_UID_SIZE), 
					   (UINT32)(void *)&puTmp,1));
	
	UNUSED(EEPROM_Write(LOADER_UPGRADE_CFG_PKG_EMID_ADDR, 
					   (UINT32)(void *)&szEncryptModelID[0],LOADER_UPGRADE_CFG_EMID_SIZE));

	UNUSED(EEPROM_Write((LOADER_UPGRADE_CFG_PKG_EMID_ADDR+LOADER_UPGRADE_CFG_EMID_SIZE), 
					   (UINT32)(void *)&puTmp,1));

	UNUSED(EEPROM_Write(LOADER_UPGRADE_CFG_PKG_VERSION_ADDR, 
					   (UINT32)(void *)&szVersionID[0],LOADER_UPGRADE_CFG_PKG_VERSION_SIZE));

	UNUSED(EEPROM_Write((LOADER_UPGRADE_CFG_PKG_VERSION_ADDR+LOADER_UPGRADE_CFG_PKG_BROADCAST_SIZE), 
					   (UINT32)(void *)&szBroadcastID[0],LOADER_UPGRADE_CFG_PKG_BROADCAST_SIZE));   

    //store HMAC 
    #if defined(CC_SECURESTORAGE_SUPPORT)
    UNUSED(EEPROM_Read(LOADER_UPGRADE_CFG_PKG_VERSION_INFO_ADDR,
                       (UINT32)(VOID *)&arOnBoardVerInfo[0], LOADER_UPGRADE_CFG_PKG_VERSION_INFO_SIZE));
    UNUSED(_LdrVersionHmacGen(arOnBoardVerInfo,arVerInfoHmac));

	UNUSED(EEPROM_Write(LOADER_UPGRADE_CFG_PKG_HMAC_ADDR, 
					   (UINT32)(void *)&arVerInfoHmac[0],LOADER_UPGRADE_CFG_PKG_HMAC_SIZE));
	#endif
	
	Printf("write_new_pkg_cfg_version updated!\n");
}
// @ret
//      1   continue to upgrade
//      0   don't upgrade
static INT32 _LdrCheckVersion(void)
{
    UINT8 puRcipData[LOADER_UPGRADE_PNLSTR_SIZE+1];
    UINT8 puCacheData[LOADER_UPGRADE_PNLSTR_SIZE+1];
	CHAR szUID[LOADER_UPGRADE_CFG_UID_SIZE+1];
	CHAR szEMID[LOADER_UPGRADE_CFG_EMID_SIZE+1];
	CHAR szVID[LOADER_UPGRADE_CFG_PKG_VERSION_SIZE+1];	
	UINT8 puVerCtrlInfo[LOADER_UPGRADE_CFG_PKG_VERSION_INFO_SIZE];
	UINT8 puVerCtrlInfoHmac[LOADER_UPGRADE_CFG_PKG_HMAC_SIZE];
	UINT8 arResetVesionInfo[LOADER_UPGRADE_CFG_PKG_VERSION_INFO_SIZE+LOADER_UPGRADE_CFG_PKG_HMAC_SIZE]={0};
	UINT32 u4VersionInfo;

    if(_fgForceUpgrade) 
    {
		Printf("_LdrCheckVersion force upgrade!\n");
        return 1;
    }
    else 
    {
      #if defined(CC_SECURESTORAGE_SUPPORT)
	  UNUSED(EEPROM_Read(LOADER_UPGRADE_CFG_PKG_VERSION_INFO_ADDR,
					   (UINT32)(VOID *)&puVerCtrlInfo[0], LOADER_UPGRADE_CFG_PKG_VERSION_INFO_SIZE));
      
	  UNUSED(EEPROM_Read(LOADER_UPGRADE_CFG_PKG_HMAC_ADDR,
						 (UINT32)(VOID *)&puVerCtrlInfoHmac[0], LOADER_UPGRADE_CFG_PKG_HMAC_SIZE));

      if(0 != (_LdrVersionHmacCompare(puVerCtrlInfo,puVerCtrlInfoHmac)))
      {
          #if 0
          UNUSED(_LdrVersionHmacGen(puVerCtrlInfo,puVerCtrlInfoHmac));
		  Printf("_LdrCheckVersion HMAC not match :\n");
		  Printf("HMAC(0~32) = \n");
		  for(i=0;i<4;i++)
		  {
		      Printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
			  	     puVerCtrlInfoHmac[i*8],puVerCtrlInfoHmac[i*8+1],puVerCtrlInfoHmac[i*8+2],
			  	     puVerCtrlInfoHmac[i*8+3],puVerCtrlInfoHmac[i*8+4],puVerCtrlInfoHmac[i*8+5],
			  	     puVerCtrlInfoHmac[i*8+6],puVerCtrlInfoHmac[i*8+7]);
		  }
	      #endif
		  
          if(_fgVersionInit)
          {
		      Printf("_LdrCheckVersion HMAC not match upgrade via VersionInit!\n");
			  //to do init version info 
	          szVersionID[0] = 0x30;
	          szVersionID[1] = 0x30;
	          szVersionID[2] = 0x30;
	          szVersionID[3] = 0x30;
              return 1;          
      }
		  else
      {
		      Printf("_LdrCheckVersion HMAC not match upgrade abort!\n");
			  //to do inform AP delete pkg
		      return 0;
		  }
      }
	  #endif
	  
      UNUSED(EEPROM_Read(LOADER_UPGRADE_CFG_PKG_VERSION_ADDR,
                         (UINT32)(VOID *)&szVID[0], LOADER_UPGRADE_CFG_PKG_VERSION_SIZE));
	  szVID[LOADER_UPGRADE_CFG_PKG_VERSION_SIZE]='\0';
	  u4VersionInfo = StrToHex(szVID,4);

	  UNUSED(EEPROM_Read(LOADER_UPGRADE_CFG_PKG_UID_ADDR,
						 (UINT32)(VOID *)&szUID[0], LOADER_UPGRADE_CFG_UID_SIZE));
	  szUID[LOADER_UPGRADE_CFG_UID_SIZE]='\0';

	  UNUSED(EEPROM_Read(LOADER_UPGRADE_CFG_PKG_EMID_ADDR,
						 (UINT32)(VOID *)&szEMID[0], LOADER_UPGRADE_CFG_EMID_SIZE)); 
	  szEMID[LOADER_UPGRADE_CFG_EMID_SIZE]='\0';
      //read cache and rcipdata for panel string cmp
      
	  UNUSED(EEPROM_Read(LOADER_UPGRADE_PNLSTR_RECIP_ADDR,
						 (UINT32)(VOID *)&puRcipData[0], LOADER_UPGRADE_PNLSTR_SIZE)); 
	  puRcipData[LOADER_UPGRADE_PNLSTR_SIZE]='\0';
	  
	  UNUSED(EEPROM_Read(LOADER_UPGRADE_PNLSTR_CACHE_ADDR,
						 (UINT32)(VOID *)&puCacheData[0], LOADER_UPGRADE_PNLSTR_SIZE)); 
      puCacheData[LOADER_UPGRADE_PNLSTR_SIZE]='\0';
	  
	  if((0 != (x_strncmp(&szUpdateID[0], &szUID[0], LOADER_UPGRADE_CFG_UID_SIZE))) || 
		  (0 != (x_strncmp(&szEncryptModelID[0], &szEMID[0], LOADER_UPGRADE_CFG_EMID_SIZE))))
	  {
        Printf("Do not upgrade due to filename version check\n");
        return 0;	
        }
        else 
        {
        if(0 != (x_strncmp(&puRcipData[0], &puCacheData[0], LOADER_UPGRADE_PNLSTR_SIZE)))
        {
		  Printf("_LdrCheckVersion update panel string in rcip area!\n");
          UNUSED(EEPROM_Write(LOADER_UPGRADE_PNLSTR_RECIP_ADDR, 
			  	             (UINT32)(void *)&puCacheData[0],LOADER_UPGRADE_PNLSTR_SIZE));
	    if(u4PackageVersion >= u4VersionInfo)
            {
		  Printf("_LdrCheckVersion : pkg ver(%d) >= pcba ver(%d) => upgrade!\n",u4PackageVersion,u4VersionInfo);
                return 1;
            }
            else 
            {
          Printf("_LdrCheckVersion : pkg ver(%d) < pcba ver(%d) => skip upgrade!\n",u4PackageVersion,u4VersionInfo);
                return 0;
            }
        }
		else
		{
	      if(u4PackageVersion > u4VersionInfo)
	      {
		    Printf("_LdrCheckVersion : pkg ver(%d) > pcba ver(%d) => upgrade!\n",u4PackageVersion,u4VersionInfo);
            return 1;	    
	      }
		  else
		  {
            Printf("_LdrCheckVersion : pkg ver(%d) <= pcba ver(%d) => skip upgrade!\n",u4PackageVersion,u4VersionInfo);
            return 0;		
		  }			
		}
	  }
    }
}
#endif

#ifndef CC_USB_DISABLE
#ifndef LOADER_UPGRADE_IN_ONE_STEP
static void _LdrLoaderUpgradeReboot(void)
{
    #define REG_RW_WATCHDOG_TMR 0x0110          // Watchdog Timer Register
    #define REG_RW_WATCHDOG_EN  0x0100          // Watchdog Timer Control Register
    #define PDWNC_WRITE32(offset, value)    IO_WRITE32(PDWNC_BASE, (offset), value)
    #define PDWNC_READ32(offset)            IO_READ32(PDWNC_BASE, (offset))

    if(PDWNC_READ32(REG_RW_WATCHDOG_EN)==0)
    {
#ifdef SSUSB_LOADER
        MUSB3_SetSe0();  //reset USB3 port device
#endif
        PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, 0xffff0000);
        PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0xff0000);
    }
}
#endif
#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) // nand or emmc boot
static INT32 _LdrFlashDynamicDataWrite(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    UINT32 u4Offset, u4Size, u4PartitionSize = 0;
    VOID *pvMemPtr;

    if (e_flag != DLM_WRITE)
    {
        DEBUG("e_flag type error (DLM_WRITE only)\n");
        return DLM_OTHERS;
    }

    u4Size = *ui4_len;
    pvMemPtr = (VOID *)pauc_buffer;
#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
    if ((_u8ImageStartAddr & (_u4BufSize-1)) && (!_fgRecipUpgradeMode))
#else
    if ((_u8ImageStartAddr & (_u4BufSize-1)))
#endif
    {
        DEBUG("image write address is not %d Byte alignment\n", _u4BufSize);
        return -1;
    }

#if defined(CC_USE_2TB_EMMC)
    u4Offset = _u8ImageStartAddr - DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
    u4PartitionSize = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartSize0+_u1CurrentPartitionId));
#else
    u4Offset = _u8ImageStartAddr - DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
	u4PartitionSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0+_u1CurrentPartitionId));
#endif

if((u4Offset >= u4PartitionSize)||((u4Offset+u4Size) > u4PartitionSize))
{
	Printf("Flash write fail: write size overflow patition size Offset:0x%08x%08x, Size:0x%08x\n",
				(UINT32)(_u8ImageStartAddr>>32), (UINT32)_u8ImageStartAddr, u4Size);
			while(1);
}

#if defined(CC_NAND_BOOT)
    if (Loader_WriteNandFlash(_u1CurrentPartitionId, u4Offset, pvMemPtr, u4Size) != u4Size)
#else
    if (Loader_WriteMsdc(_u1CurrentPartitionId, u4Offset, pvMemPtr, u4Size) != u4Size)
#endif
    {
        Printf("Flash write fail: Offset:0x%08x%08x, Size:0x%08x\n",
			(UINT32)(_u8ImageStartAddr>>32), (UINT32)_u8ImageStartAddr, u4Size);
        while(1);
    }

    Printf("Flash write complete: Offset:0x%08x%08x, Size:0x%08x\n",
		(UINT32)(_u8ImageStartAddr>>32), (UINT32)_u8ImageStartAddr, u4Size);
    _u8ImageStartAddr += u4Size;

    return DLM_OK;
}

static INT32 _LdrFlashWrite(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    UNUSED(_pu4NorBuffer);
    return _LdrFlashDynamicDataWrite(e_flag, pauc_buffer, ui4_len);
}

#else // nor boot

static INT32 _LdrFlashWrite(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    INT32 i4Size = 0;
    UINT32 u4BlkSize, u4MemPtr, i;
    UINT32 *pu4Cmp;
    UINT32 cnt = 0;
    UINT32 u4FlashAddr = PBI_A_BASE + DRVCUST_InitGet(eNorFlashLoaderSize);

    if (e_flag != DLM_WRITE)
    {
        DEBUG("e_flag type error (DLM_WRITE only)\n");
        return DLM_OTHERS;
    }

    i4Size = *ui4_len;
    u4BlkSize = _u4BufSize;
    u4MemPtr = (UINT32)pauc_buffer;

    if(_pu4NorBuffer == NULL)
    {
        _pu4NorBuffer = x_mem_alloc(u4BlkSize);
    }

    if ((*ui4_len) != _u4BufSize)
    {
        x_memcpy(_pu4NorBuffer, (const void *)(u4FlashAddr+cnt), 0x10000);
    }

    if ( _u4ImageStartAddr & (u4BlkSize-1))
    {
        DEBUG("image write address is not %d Byte alignment\n", u4BlkSize);
        return -1;
    }

    do
    {
        if (NOR_EraseBlk(_u4ImageStartAddr))
        {
            Printf("Nor flash erase fail: Offset:0x%08x\n", _u4ImageStartAddr);
        }

        if (NOR_Write((UINT64)_u4ImageStartAddr, u4MemPtr, u4BlkSize) )
        {
            Printf("Nor flash write fail: Offset:0x%08x, BlkSize:0x%x\n", _u4ImageStartAddr, u4BlkSize);
        }

        if (NOR_Read((UINT64)_u4ImageStartAddr, (UINT32)_pu4NorBuffer, u4BlkSize) )
        {
            Printf("Nor flash read fail: Offset:0x%08x, BlkSize:0x%x\n", _u4ImageStartAddr, u4BlkSize);
        }

        pu4Cmp = (UINT32 *)u4MemPtr;
        for (i = 0; i < u4BlkSize/4; i++)
        {
            if ((_pu4NorBuffer != NULL)&&(pu4Cmp[i] != _pu4NorBuffer[i]))
            {
                _u4ImageStartAddr += u4BlkSize;
                DEBUG("compare fail\n");
                return DLM_WRITE_DATA_FAILED;
            }
        }

        Printf("Nor flash write complete: Offset:0x%08x, BlkSize:0x%x\n", _u4ImageStartAddr, u4BlkSize);
        u4MemPtr += u4BlkSize;
        i4Size -= u4BlkSize;
        _u4ImageStartAddr += u4BlkSize;
    }while(i4Size>0);

    return DLM_OK;
}
#endif

static INT32 _LdrAllUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    static UINT32 u4Flag = 0;

    if (!u4Flag)
    {
#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
        _u4ImageStartAddr = 0;
#else
        _u8ImageStartAddr = 0;
#endif
        u4Flag = 1;
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);
}

static INT32 _LdrFirmUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    static UINT32 u4Flag = 0;

    if (!u4Flag)
    {
#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
        _u4ImageStartAddr = LOADER_MAX_SIZE;
#else
        _u1CurrentPartitionId = DRVCUST_InitGet(eNANDFlashPartIdSysImgA);
#if defined(CC_USE_2TB_EMMC)
        _u8ImageStartAddr = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
#else
        _u8ImageStartAddr = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
#endif
#endif

        u4Flag = 1;
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);
}

static INT32 _LdrFir2Upgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    static UINT32 u4Flag = 0;

    if (!u4Flag)
    {
#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
        _u4ImageStartAddr = LOADER_MAX_SIZE;
#else
        _u1CurrentPartitionId = DRVCUST_InitGet(eNANDFlashPartIdSysImgB);
#if defined(CC_USE_2TB_EMMC)
        _u8ImageStartAddr = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
#else
        _u8ImageStartAddr = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
#endif
#endif

        u4Flag = 1;
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);
}

static INT32 _LdrMicpUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    static UINT32 u4Flag = 0;

    if (!u4Flag)
    {
        _u1CurrentPartitionId = DRVCUST_InitGet(eNANDFlashPartIdUpA);
#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
        _u4ImageStartAddr = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));;
#else
#if defined(CC_USE_2TB_EMMC)
        _u8ImageStartAddr = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
#else
        _u8ImageStartAddr = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
#endif
#endif
        u4Flag = 1;
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);
}

static INT32 _Ldr8281Upgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    UNUSED(e_flag);
    UNUSED(pauc_buffer);
    UNUSED(ui4_len);

#ifdef DRV_SUPPORT_EXTMJC
    static UINT32 u4SAddress = 0;
    UINT32 u4Len;
    UINT8 ui1_err;

    UNUSED(ui1_err);

    if (FALSE == drv_extmjc_is_enable())
    {
        return DLM_OK;
    }

    Printf("\n_Ldr8281Upgrade ............\n");
    if (e_flag != DLM_WRITE)
    {
        DEBUG("e_flag type error (DLM_WRITE only)\n");
        return DLM_OTHERS;
    }

    u4Len = (UINT32)(*ui4_len);

#ifdef DRV_SUPPORT_MT8283
    drv_extmjc_i2c_access_enable_password(&ui1_err);
    u1ExtmjcWriteRegByI2C(0x81C8, 0);
#endif

    if(u4SAddress == 0x0)
    {
        d_custom_extmjc_upgrade_open();
    }

#ifdef DRV_SUPPORT_MT8283
    d_custom_extmjc_register_test();
#endif

    Printf("\n_Ldr8282 Upgrade Address:%x, Size:%x\n", u4SAddress, u4Len);

#ifndef DRV_SUPPORT_MT8283
    d_custom_extmjc_upgrade_erase();
#else
    d_custom_extmjc_upgrade_erase(u4SAddress, u4Len);
#endif

    d_custom_extmjc_upgrade_write(u4SAddress, pauc_buffer, u4Len);
    u4SAddress = u4SAddress + u4Len;

    if(u4Len % 0x10000 != 0)
    {
        d_custom_extmjc_upgrade_close();
        Printf("\n_Ldr8282Upgrade Close");
        Printf("\n_Ldr8282 Reset");
        drv_extmjc_reset();
    }
#endif

    return DLM_OK;
}

static INT32 _LdrLoaderUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) // nand or emmc boot
    static UINT32 u4Flag = 0;

    if (!u4Flag)
    {
        _u1CurrentPartitionId = 0;
        _u8ImageStartAddr = 0;
        u4Flag = 1;
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);

#else // nor boot

    UINT32 cnt = 0;
    UINT32 u4FlashAddr = PBI_A_BASE + DRVCUST_InitGet(eNorFlashLoaderSize);
    INT32 i4Size = 0;
    UINT32 u4BlkSize, u4MemPtr, i;
    UINT32 *pu4Cmp;
    static UINT32 _u4LoaderImageStartAddr = 0;

    if (e_flag != DLM_WRITE)
    {
        DEBUG("e_flag type error (DLM_WRITE only)\n");
        return DLM_OTHERS;
    }

    i4Size = *ui4_len;
    u4BlkSize = _u4BufSize;
    u4MemPtr = (UINT32)pauc_buffer;

    if(_pu4NorBuffer == NULL)
    {
       _pu4NorBuffer = x_mem_alloc(u4BlkSize);
    }

    if ((*ui4_len) != _u4BufSize)
    {
        x_memcpy(_pu4NorBuffer, (const void *)(u4FlashAddr+cnt), 0x10000);
    }

    if ( _u4LoaderImageStartAddr & (u4BlkSize-1))
    {
        DEBUG("image write address is not %d Byte alignment\n", u4BlkSize);
        return -1;
    }

    do
    {
        if (NOR_EraseBlk(_u4LoaderImageStartAddr))
        {
            Printf("Nor flash erase fail: Offset:0x%08x\n", _u4LoaderImageStartAddr);
        }

        if (NOR_Write((UINT64)_u4LoaderImageStartAddr, u4MemPtr, u4BlkSize) )
        {
            Printf("Nor flash write fail: Offset:0x%08x, BlkSize:0x%x\n", _u4LoaderImageStartAddr, u4BlkSize);
        }

        if (NOR_Read((UINT64)_u4LoaderImageStartAddr, (UINT32)_pu4NorBuffer, u4BlkSize) )
        {
            Printf("Nor flash read fail: Offset:0x%08x, BlkSize:0x%x\n", _u4LoaderImageStartAddr, u4BlkSize);
        }

        pu4Cmp = (UINT32 *)u4MemPtr;
        for (i = 0; i < u4BlkSize/4; i++)
        {
            if ((_pu4NorBuffer != NULL)&&(pu4Cmp[i] != _pu4NorBuffer[i]))
            {
                _u4LoaderImageStartAddr += u4BlkSize;
                DEBUG("compare fail\n");
                return DLM_WRITE_DATA_FAILED;
            }
        }

        Printf("Nor flash write complete: Offset:0x%08x, BlkSize:0x%x\n", _u4LoaderImageStartAddr, u4BlkSize);
        u4MemPtr += u4BlkSize;
        i4Size -= u4BlkSize;
        _u4LoaderImageStartAddr += u4BlkSize;
    }while(i4Size>0);

    return DLM_OK;
#endif
}

static INT32 _LdrDynamicUpgrade(UCHAR* s_tag_name, DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32 *pi4_len)
{
    UINT32 u4Idx = 0;
	#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
	UINT8  u1RcipIdx = 0;
	INT32 i4DataLen=0;
    #endif
	INT32 i4_ret = DLM_OTHERS;
	
#ifdef DUAL_BANKS_SHARE_ONE_IMG
	UINT32 u4_wr_len = 0;

	if (x_strncmp((const char *)_s_last_tag, (const char *)s_tag_name, 4) != 0)
	{
		x_strncpy((char*)_s_last_tag, (const char *)s_tag_name, 4);
		_u4ImageOffset = 0;
	}
#endif
    for (u4Idx = 0; u4Idx < _u4NoOfTags; u4Idx++)
    {
        if (x_strncmp(_arUpgCfg[u4Idx].szTagName, (const char *)s_tag_name, 4) == 0)
        {
            /* Tag matched, use _arUpgCfg[u4Idx].szTagName.eFlashType and
                         _arUpgCfg[u4Idx].u1PartitionID to do something here. */

#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) // nand or emmc boot
            if (_arUpgCfg[u4Idx].eFlashType == NAND)
            {
                if (_u1CurrentPartitionId != _arUpgCfg[u4Idx].u1PartitionID)
                {
                    _u1CurrentPartitionId = _arUpgCfg[u4Idx].u1PartitionID;
					#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
				    _fgPartitionFirstBlock = TRUE;
					_fgRecipUpgradeDone = FALSE;
					_fgRecipUpgradeFullWrite = FALSE;
					_fgRecipeWriteFromFirstByte = TRUE;
					_fgRecipUpgradeMode = FALSE;
					ui4DataWritenSize = 0;
					#endif
#if defined(CC_USE_2TB_EMMC)
                    _u8ImageStartAddr = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
#else
                    _u8ImageStartAddr = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+_u1CurrentPartitionId));
#endif
#ifdef DUAL_BANKS_SHARE_ONE_IMG
					_u8ImageStartAddr += _u4ImageOffset;
#endif
                }
                #ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
                if (_arUpgCfg[u4Idx].fgSupportRecipeUpgarde)
                {
                  if(_u1RecipeIDMatch)
                  {
                    _fgRecipUpgradeMode = TRUE;
                    if(!_fgRecipUpgradeDone)
                    {
                    
                        #ifdef CC_MAPLE_LOADER_UPGRADE_NEW_RECIPE_SPEC
                        for(u1RcipIdx = 0; u1RcipIdx < _u4NoOfRecipeTags; u1RcipIdx++)
                        {
                            if((x_strncmp(_arUpgCfg[u4Idx].szTagName, _arRcipUpgCfg[u1RcipIdx].szTagName, 4) == 0) &&
					  	       (_arRcipUpgCfg[u1RcipIdx].u1DataCaseIndex <= _arUpgCfg[u4Idx].u1DataAmounts))
                            {
                                
				     Printf("szTagName: %s, u1DataCaseIndex:%d, u1DataAmounts:%d \n", _arRcipUpgCfg[u1RcipIdx].szTagName, _arRcipUpgCfg[u1RcipIdx].u1DataCaseIndex, _arUpgCfg[u4Idx].u1DataAmounts);
                                break;
                            }
                        }
                        #else
                        for(u1RcipIdx = 0; u1RcipIdx < 32; u1RcipIdx++)
                        {
                            if((_arUpgCfg[u4Idx].u1PartitionID == _arRcipUpgCfg[_u1CurrentModelIdx].u1PartitionID[u1RcipIdx]) &&
					  	       (_arRcipUpgCfg[_u1CurrentModelIdx].u1DataOffsetIdx[u1RcipIdx] < _arUpgCfg[u4Idx].u1DataAmounts))
                            {
                                break;
                            }
                        }
                        #endif
		    
                        //#ifdef CC_MAPLE_LOADER_UPGRADE_NEW_RECIPE_SPEC
                        #if 1
				  UINT8 u1DataOffset = 0;
				  if (_arRcipUpgCfg[u1RcipIdx].u1DataCaseIndex > 0)
				  {
				      u1DataOffset = _arRcipUpgCfg[u1RcipIdx].u1DataCaseIndex - 1;
				  }
				  
				  Printf("u1DataOffset: %d, i4DataLen:%d, pauc_buffer:%c, %c\n", u1DataOffset, _arUpgCfg[u4Idx].u4DataSize, *pauc_buffer, *(pauc_buffer+2));				  
				  i4DataLen = (INT32)_arUpgCfg[u4Idx].u4DataSize;
				  pauc_buffer = pauc_buffer + (u1DataOffset * i4DataLen);	  
				  Printf("u1DataOffset: %d, i4DataLen:%d, pauc_buffer:%c, %c\n", u1DataOffset, i4DataLen, *pauc_buffer, *(pauc_buffer+2));				  
				  _LdrFlashDynamicDataWrite(e_flag, pauc_buffer, &i4DataLen);
				  #ifdef DUAL_BANKS_SHARE_ONE_IMG
								if (u4_wr_len == 0)
								{
									u4_wr_len = i4DataLen;
								}
								else if (u4_wr_len != i4DataLen)
								{
									Printf("Dual banks' written size are different: size1:%d, size2:%d\n", u4_wr_len, i4DataLen);
									return DLM_OTHERS;
								}
                            #endif
				 //ui4DataWritenSize = ui4DataWritenSize+(UINT32)i4DataLen;
				//if(ui4DataWritenSize >= _arUpgCfg[u4Idx].u4DataSize) 
				//{
					 _fgRecipUpgradeDone = TRUE;
				//}
			    #else
                        if(_fgPartitionFirstBlock)
                        {
                            if(!_arRcipUpgCfg[_u1CurrentModelIdx].u4DataSkipCounter[u1RcipIdx])
                            {
								if((_arUpgCfg[u4Idx].u4DataSize - ui4DataWritenSize)>= 0x100000) 
								{
								    _fgRecipUpgradeFullWrite = TRUE;
								} 
								else
								{
								    _fgRecipUpgradeFullWrite = FALSE;
								}
								
								if((!_arRcipUpgCfg[_u1CurrentModelIdx].u4DataOfstToWrite[u1RcipIdx]) || 
									(!_arRcipUpgCfg[_u1CurrentModelIdx].u1DataOffsetIdx[u1RcipIdx]))
								{
								    _fgRecipeWriteFromFirstByte = TRUE;
								}
								else
								{
								    _fgRecipeWriteFromFirstByte = FALSE;
								}
								
                                if(!_fgRecipeWriteFromFirstByte)
                                {
								    pauc_buffer = pauc_buffer + _arRcipUpgCfg[_u1CurrentModelIdx].u4DataOfstToWrite[u1RcipIdx];
								    i4DataLen = *pi4_len;
								    //Printf("1_LdrDynamicUpgrade(recipe upgrade) Write1 BufferDataLen(Size:0x%08x)!\n",i4DataLen);
								    i4DataLen = i4DataLen-((INT32)_arRcipUpgCfg[_u1CurrentModelIdx].u4DataOfstToWrite[u1RcipIdx]);
									if(i4DataLen > _arUpgCfg[u4Idx].u4DataSize)
									{
									    i4DataLen = (INT32)_arUpgCfg[u4Idx].u4DataSize;
									}
									_arRcipUpgCfg[_u1CurrentModelIdx].u4DataOfstToWrite[u1RcipIdx] = 0;
								    //Printf("1_LdrDynamicUpgrade(recipe upgrade) Write1 DataLen(Size:0x%08x)!\n",i4DataLen);								
                                }
								else if(!_fgRecipUpgradeFullWrite)
								{
								    //Printf("1_LdrDynamicUpgrade(recipe upgrade) Write2 BufferDataLen(Size:0x%08x)!\n",i4DataLen);
								    i4DataLen = (INT32)(_arUpgCfg[u4Idx].u4DataSize-ui4DataWritenSize);
								    //Printf("1_LdrDynamicUpgrade(recipe upgrade) Write2 DataLen(Size:0x%08x)!\n",i4DataLen);									    
								}
								else
								{
								    i4DataLen = *pi4_len;
								    //Printf("1_LdrDynamicUpgrade(recipe upgrade) FullWrite DataLen(Size:0x%08x)!\n",i4DataLen);
								}
								_LdrFlashDynamicDataWrite(e_flag, pauc_buffer, &i4DataLen);
#ifdef DUAL_BANKS_SHARE_ONE_IMG
								if (u4_wr_len == 0)
								{
									u4_wr_len = i4DataLen;
								}
								else if (u4_wr_len != i4DataLen)
								{
									Printf("Dual banks' written size are different: size1:%d, size2:%d\n", u4_wr_len, i4DataLen);
									return DLM_OTHERS;
								}
#endif
							    ui4DataWritenSize = ui4DataWritenSize+(UINT32)i4DataLen;
								if(ui4DataWritenSize >= _arUpgCfg[u4Idx].u4DataSize) 
								{
								    _fgRecipUpgradeDone = TRUE;
						        }								
						    }
							else
							{
                                _arRcipUpgCfg[_u1CurrentModelIdx].u4DataSkipCounter[u1RcipIdx]--;
								//Printf("_LdrDynamicUpgrade(recipe upgrade) skip write to FLASH at 1st Block(%d)!\n",_arRcipUpgCfg[_u1CurrentModelIdx].u4DataSkipCounter[u1RcipIdx]);
							}
							_fgPartitionFirstBlock = FALSE;
                        }
						else
						{
						    if(_arRcipUpgCfg[_u1CurrentModelIdx].u4DataSkipCounter[u1RcipIdx])
						    {
						        _arRcipUpgCfg[_u1CurrentModelIdx].u4DataSkipCounter[u1RcipIdx]--;
								//Printf("_LdrDynamicUpgrade(recipe upgrade) skip write to FLASH(%d)!\n",_arRcipUpgCfg[_u1CurrentModelIdx].u4DataSkipCounter[u1RcipIdx]);
						    }
							else
							{
								if((_arUpgCfg[u4Idx].u4DataSize - ui4DataWritenSize)>= 0x100000) 
								{
								    _fgRecipUpgradeFullWrite = TRUE;
								} 
								else
								{
								    _fgRecipUpgradeFullWrite = FALSE;
								}
								
								if((!_arRcipUpgCfg[_u1CurrentModelIdx].u4DataOfstToWrite[u1RcipIdx]) || 
									(!_arRcipUpgCfg[_u1CurrentModelIdx].u1DataOffsetIdx[u1RcipIdx]))

								{
								    _fgRecipeWriteFromFirstByte = TRUE;
								}
								else
								{
								    _fgRecipeWriteFromFirstByte = FALSE;
								}
								
                                if(!_fgRecipeWriteFromFirstByte)
                                {
								    pauc_buffer = pauc_buffer + _arRcipUpgCfg[_u1CurrentModelIdx].u4DataOfstToWrite[u1RcipIdx];
								    i4DataLen = *pi4_len;
								    //Printf("2_LdrDynamicUpgrade(recipe upgrade) Write1 BufferDataLen(Size:0x%08x)!\n",i4DataLen);
								    i4DataLen = i4DataLen-((INT32)_arRcipUpgCfg[_u1CurrentModelIdx].u4DataOfstToWrite[u1RcipIdx]);
									if(i4DataLen > _arUpgCfg[u4Idx].u4DataSize)
									{
									    i4DataLen = (INT32)_arUpgCfg[u4Idx].u4DataSize;
									}									
									_arRcipUpgCfg[_u1CurrentModelIdx].u4DataOfstToWrite[u1RcipIdx] = 0;
								    //Printf("2_LdrDynamicUpgrade(recipe upgrade) Write1 DataLen(Size:0x%08x)!\n",i4DataLen);								
                                }
								else if(!_fgRecipUpgradeFullWrite)
								{
								    //Printf("2_LdrDynamicUpgrade(recipe upgrade) Write2 BufferDataLen(Size:0x%08x)!\n",i4DataLen);
								    i4DataLen = (INT32)(_arUpgCfg[u4Idx].u4DataSize-ui4DataWritenSize);
								    //Printf("2_LdrDynamicUpgrade(recipe upgrade) Write2 DataLen(Size:0x%08x)!\n",i4DataLen);									    
								}
								else
								{
								    i4DataLen = *pi4_len; 
								    //Printf("2_LdrDynamicUpgrade(recipe upgrade) FullWrite DataLen(Size:0x%08x)!\n",i4DataLen);
								}
								_LdrFlashDynamicDataWrite(e_flag, pauc_buffer, &i4DataLen);
#ifdef DUAL_BANKS_SHARE_ONE_IMG
								if (u4_wr_len == 0)
								{
									u4_wr_len = i4DataLen;
								}
								else if (u4_wr_len != i4DataLen)
								{
									Printf("Dual banks' written size are different: size1:%d, size2:%d\n", u4_wr_len, i4DataLen);
									return DLM_OTHERS;
								}
#endif
							    ui4DataWritenSize = ui4DataWritenSize+(UINT32)i4DataLen;
								if(ui4DataWritenSize >= _arUpgCfg[u4Idx].u4DataSize) 
								{
								    _fgRecipUpgradeDone = TRUE;
						        }
							}
						}
				#endif //CC_MAPLE_LOADER_UPGRADE_NEW_RECIPE_SPEC
                    }
				    else
				    {
				        //Printf("_LdrDynamicUpgrade Done!\n");
				    }
                }
				else
				{
				      Printf("_LdrDynamicUpgrade Recipe Fail(No correct MID)!\n");
				  }
                }
				else
				{
				    _LdrFlashDynamicDataWrite(e_flag, pauc_buffer, pi4_len);
#ifdef DUAL_BANKS_SHARE_ONE_IMG
					if (u4_wr_len == 0)
					{
						u4_wr_len = *pi4_len;
					}
					else if (u4_wr_len != *pi4_len)
					{
						Printf("Dual banks' written size are different: size1:%d, size2:%d\n", u4_wr_len, *pi4_len);
						return DLM_OTHERS;
					}
#endif
				}
				#else
				_LdrFlashDynamicDataWrite(e_flag, pauc_buffer, pi4_len);
#ifdef DUAL_BANKS_SHARE_ONE_IMG
				if (u4_wr_len == 0)
				{
					u4_wr_len = *pi4_len;
				}
				else if (u4_wr_len != *pi4_len)
				{
					Printf("Dual banks' written size are different: size1:%d, size2:%d\n", u4_wr_len, *pi4_len);
					return DLM_OTHERS;
				}
#endif
				#endif				
            }
#else // nor boot
            if (_arUpgCfg[u4Idx].eFlashType == NOR)
            {
                if (_u1CurrentPartitionId != _arUpgCfg[u4Idx].u1PartitionID)
                {
                    _u1CurrentPartitionId = _arUpgCfg[u4Idx].u1PartitionID;
                    _u4ImageStartAddr = DRVCUST_InitGet((QUERY_TYPE_T)(eNorFlashPartOffset0+_u1CurrentPartitionId));
#ifdef DUAL_BANKS_SHARE_ONE_IMG
					_u4ImageStartAddr += _u4ImageOffset;
#endif
                }

                _LdrFlashWrite(e_flag, pauc_buffer, pi4_len);
#ifdef DUAL_BANKS_SHARE_ONE_IMG
				if (u4_wr_len == 0)
				{
					u4_wr_len = *pi4_len;
				}
				else if (u4_wr_len != *pi4_len)
				{
					Printf("Dual banks' written size are different: size1:%d, size2:%d\n", u4_wr_len, *pi4_len);
					return DLM_OTHERS;
				}
#endif
            }
#endif

			i4_ret = DLM_OK;
        }
    }

    if (i4_ret == DLM_OK)
    {
#ifdef DUAL_BANKS_SHARE_ONE_IMG
		_u4ImageOffset += u4_wr_len;
#endif
        return DLM_OK;
    }
    else
    {
        DEBUG("Tag not found!\n");
        return DLM_OTHERS;
    }
}

#if defined(CC_SECURE_BOOT) && defined(CC_LOADER_VERIFY_PKG)

#define ENCRYPTED_DIGEST_OFFSET   0
#define DIGEST_OFFSET    256
#define COMMAND_OFFSET   256+128

extern INT32 RSADecryption(UINT32 *pu1Signature, UINT32 *pu4PublicKey, UINT32 *pu4CheckSum);

static INT32 _LdrUsigUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    //UINT32 u4BufIdx=0;
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
    UINT32 au4Key[64]; // 2048bit key
    UINT32 au4Result[64]; // 2048bit key
    UINT32 u4DIAG;
    UINT32 u4Addr;
    
    if (!pauc_buffer)
    {
        DEBUG("Memory buffer is NULL!\n");
        return DLM_OTHERS;
    }

    // get vendor public key
    x_memcpy((void*)au4Key, (void*)prLdrEnv->au4CustKey, 256);

    // decrypt diagest
    RSADecryption((UINT32 *)(pauc_buffer+ENCRYPTED_DIGEST_OFFSET), au4Key, au4Result);

    if (x_memcmp((void*)(pauc_buffer+DIGEST_OFFSET), au4Result, 255) == 0)
    {
        if (pauc_buffer[COMMAND_OFFSET] == USIG_DO_UPGRADE)
        {
            _u4UsigCmd = USIG_DO_UPGRADE;
        }
        else if (pauc_buffer[COMMAND_OFFSET] == USIG_RUN_ON_USB)
        {
            _u4UsigCmd = USIG_RUN_ON_USB;
            x_strncpy((void*)prLdrEnv->szUenv, (void*)&pauc_buffer[COMMAND_OFFSET+2], 125);
            prLdrEnv->u4CfgFlag = USIG_RUN_ON_USB;
        }
        else if (pauc_buffer[COMMAND_OFFSET] == USIG_DO_DIAG)
        {
            _u4UsigCmd = USIG_DO_DIAG;
            x_strncpy((void*)prLdrEnv->szUenv, (void*)&pauc_buffer[COMMAND_OFFSET+2], 125);
            prLdrEnv->u4CfgFlag = USIG_DO_DIAG;
        }

        if (DRVCUST_InitQuery(eEepromDIAGFlagAddr, &u4Addr) == 0)
        {
            if (pauc_buffer[COMMAND_OFFSET] == USIG_DO_DIAG)
    		{
    		    u4DIAG=1; 
    			EEPROM_Write((UINT64)u4Addr, (UINT32)&u4DIAG, 1);
    		}
    		else
    		{
    		    u4DIAG=0;
    			EEPROM_Write((UINT64)u4Addr, (UINT32)&u4DIAG, 1);
    		}
		}
		
        Printf("_LdrUsigUpgrade, verify key pass, command=\'%c\'(0x%x)\n",
        pauc_buffer[COMMAND_OFFSET], pauc_buffer[COMMAND_OFFSET]);

#ifndef USIG_VER_CHECK
      Printf("No version defined!!\n");
#endif

#ifndef CC_DISABLE_UART_UPGRADE
      Printf("No CC_DISABLE_UART_UPGRADE defined!!\n");
#endif

#if (defined(CC_DISABLE_UART_UPGRADE)&& defined(USIG_VER_CHECK))
#include <string.h>
       UINT32 current = 0;
       Printf("_u4UsigCmd : %x!!\n", _u4UsigCmd);
       if ((_u4UsigCmd == '3')&&(strstr(prLdrEnv->szUenv, USIG_VER_CHECK)!=0)){
            Printf("parsing succeed %c:%s\n",_u4UsigCmd,USIG_VER_CHECK);
            //BSP_PinSet(OPCTRL(12), 0);
            current = IO_READ32(0xf0028000,0x8);
            IO_WRITE32(0xf0028000,0x8, current & 0xffffdfff);	// Enable UART RX with 0xF0028008[13]=0
            Printf("turn on uart:w 0x%x 0x%x \n",0xf0028008, current & 0xffffdfff);
       }else {
            Printf("parsing failed %c:%s\n",_u4UsigCmd,USIG_VER_CHECK);
       }
#endif
    }
    else
    {
        _u4UsigCmd = USIG_VERIFY_FAIL;
        Printf("_LdrUsigUpgrade, verify key fail\n");
    }

    return DLM_OK;
}
#endif

static INT32 _LdrCfigUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
#ifdef CC_ONE_IMG_FOR_MULTI_NAND
    CHAR *szBuf, szTmp[32],szTmp2[10];
#else
#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
    CHAR *szBuf, szTmp[32];
#else
    CHAR *szBuf, szTmp[8];
#endif
#endif
#ifdef CC_MAPLE_LOADER_UPGRADE_VERSION_CTRL_FLOW
    CHAR szVerCtrl[2];
    UINT8 u1VerCtrlMode;
    UINT32 k=0;
#endif
    INT32 i4Size = * ui4_len;
    UINT32 u4BufIdx = 0, u4TmpIdx;
	#ifndef CC_MAPLE_LOADER_UPGRADE_VERSION_CTRL_FLOW
	extern UINT32 StrToInt(const CHAR*);
    #endif

    if (!pauc_buffer)
    {
        DEBUG("Memory buffer is NULL!\n");
        return DLM_OTHERS;
    }
    szBuf = (CHAR *)pauc_buffer;

    #ifdef CC_MAPLE_LOADER_UPGRADE_VERSION_CTRL_FLOW
    if(0 != (x_strncmp(&szBuf[u4BufIdx], "CTRL", 4)))
    {
        //DEBUG("CTRL not found at cfig\n");
		Printf("CTRL not found at cfig\n");
		_fgVerionCtrlInfoExist = FALSE;
    }
	if(_fgVerionCtrlInfoExist)
	{
	    u4BufIdx+= 4;
	    szVerCtrl[0] = szBuf[u4BufIdx];
	    szVerCtrl[1] = '\0';
		u1VerCtrlMode = (UINT8)StrToInt(szVerCtrl);
		Printf("_LdrCfigUpgrade u1VerCtrlMode = 0x%x\n",u1VerCtrlMode);
		switch(u1VerCtrlMode)
		{
		  case 0:
		  	_fgForceUpgrade = TRUE;	
			_fgVersionInit = FALSE;
		  	break;
		  case 1:
		  	_fgForceUpgrade = FALSE;	
			_fgVersionInit = FALSE;
		  	break;
		  case 2:
		  	_fgForceUpgrade = TRUE;	
			_fgVersionInit = TRUE;	  	
		  	break;
		  case 3:
		    _fgForceUpgrade = FALSE;	
			_fgVersionInit = TRUE;
		  	break;
		  default:
		  	_fgForceUpgrade = TRUE;	
			_fgVersionInit = FALSE;		  	
		  	break;
		}
	    u4BufIdx+= 2;
	    for(k=0; k<LOADER_UPGRADE_CFG_UID_SIZE; k++, u4BufIdx++)
	    {
		    szUpdateID[k] = szBuf[u4BufIdx];
	    }
	    u4BufIdx++;
		szUpdateID[LOADER_UPGRADE_CFG_UID_SIZE] = '\0';
	    Printf("_LdrCfigUpgrade Version Check updateid = %s!\n", szUpdateID);
	    for(k=0; k<LOADER_UPGRADE_CFG_EMID_SIZE; k++, u4BufIdx++)
	    {
		    szEncryptModelID[k] = szBuf[u4BufIdx];
	    }
	    u4BufIdx++;
		szEncryptModelID[LOADER_UPGRADE_CFG_EMID_SIZE] = '\0';
	    Printf("_LdrCfigUpgrade Version Check encryptmodelid = %s!\n", szEncryptModelID);
	    for(k=0; k<LOADER_UPGRADE_CFG_PKG_VERSION_SIZE; k++, u4BufIdx++)
	    {
		    szVersionID[k] = szBuf[u4BufIdx];
	    }
		szVersionID[LOADER_UPGRADE_CFG_PKG_VERSION_SIZE] = '\0';
	    Printf("_LdrCfigUpgrade Version Check packageversion = %s!\n", szVersionID);
		u4PackageVersion = StrToHex(szVersionID,4);
	    for(k=0; k<LOADER_UPGRADE_CFG_PKG_BROADCAST_SIZE; k++, u4BufIdx++)
	    {
		    szBroadcastID[k] = szBuf[u4BufIdx];
	    }
		szBroadcastID[LOADER_UPGRADE_CFG_PKG_BROADCAST_SIZE] = '\0';
	    Printf("_LdrCfigUpgrade Version Check broadcastversion = %s!\n", szBroadcastID);		
	}
    #endif

    while(0 != (x_strncmp(&szBuf[u4BufIdx], "START", 5)))
    {
        if (u4BufIdx >= i4Size-5)
        {
            DEBUG("START not found at cfig\n");
            return DLM_OTHERS;
        }

        u4BufIdx++;
    }
	
    u4BufIdx+= 6;

    for ( ; u4BufIdx < i4Size; _u4NoOfTags++)
    {
        if (_u4NoOfTags >= MAX_UPG_CFG_NUM)
        {
            Printf("warning! cfg partition number is over limitation(=%d)!\n", MAX_UPG_CFG_NUM);
            break;
        }

        for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
        {
            szTmp[u4TmpIdx] = szBuf[u4BufIdx];
        }

        u4BufIdx++;
        szTmp[u4TmpIdx] = '\0';

        x_strncpy(_arUpgCfg[_u4NoOfTags].szTagName, szTmp, 4);
        _arUpgCfg[_u4NoOfTags].szTagName[4] = '\0';

        for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
        {
            szTmp[u4TmpIdx] = szBuf[u4BufIdx];
        }

        u4BufIdx++;
        szTmp[u4TmpIdx] = '\0';
        if (x_strcmp(szTmp, "nand") == 0)
        {
            _arUpgCfg[_u4NoOfTags].eFlashType = NAND;
        }
        else if (x_strcmp(szTmp, "nor") == 0)
        {
            _arUpgCfg[_u4NoOfTags].eFlashType = NOR;
        }
        else
        {
            DEBUG("FlashType not support\n");
            //return DLM_OTHERS;
        }

        for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
        {
            szTmp[u4TmpIdx] = szBuf[u4BufIdx];
        }
        #ifdef CC_ONE_IMG_FOR_MULTI_NAND
		{
		#ifdef CC_NAND_BOOT
		char* NandName = NAND_GetName();
		#endif 
	
		NANDDEV_T nand_dev_t;
		NAND_GetInfo(&nand_dev_t);
		SPrintf(szTmp2,"%dk_%dKiB",nand_dev_t.u4PgSz/1024,nand_dev_t.u4BlkSz/1024);
        u4BufIdx++;
        szTmp[u4TmpIdx] = '\0';
        if (x_strcmp(szTmp, "NAND_ALL")==0
			#ifdef CC_NAND_BOOT
			|| ((NandName!=NULL) && (x_strcmp(szTmp, NandName)==0) || (x_strcmp(szTmp2,szTmp)==0))
			#endif
			)
        {
            _arUpgCfg[_u4NoOfTags].fgNandMatch = TRUE;
			Printf("[LoaderUpgrade]Tag %s is allowed to upgrade(%s)\n",_arUpgCfg[_u4NoOfTags].szTagName,szTmp);
        }
        else
        {
        	#ifdef CC_NAND_BOOT
			Printf("[LoaderUpgrade]Partition nand(%s) format doesn't match actual nand type(%s)!\n", szTmp, NAND_GetName());
            _arUpgCfg[_u4NoOfTags].fgNandMatch = FALSE;
			#endif
        }

        for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
        {
            szTmp[u4TmpIdx] = szBuf[u4BufIdx];
        }
		}
        #endif
        u4BufIdx++;
        szTmp[u4TmpIdx] = '\0';
        _arUpgCfg[_u4NoOfTags].u1PartitionID = (UINT8)StrToInt(szTmp);

        #ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
		for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
		{
			szTmp[u4TmpIdx] = szBuf[u4BufIdx];
		}
		u4BufIdx++;
		szTmp[u4TmpIdx] = '\0';
        if (x_strcmp(szTmp, "TRUE") == 0)
        {
            _arUpgCfg[_u4NoOfTags].fgSupportRecipeUpgarde = TRUE;
        }
        else
        {
            _arUpgCfg[_u4NoOfTags].fgSupportRecipeUpgarde = FALSE;        
            //Printf("Recipe upgrade not support on PartID(%s)!\n",_arUpgCfg[_u4NoOfTags].szTagName);
        }

		for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
		{
			szTmp[u4TmpIdx] = szBuf[u4BufIdx];
		}
		u4BufIdx++;
		szTmp[u4TmpIdx] = '\0';
		_arUpgCfg[_u4NoOfTags].u1DataAmounts = (UINT8)StrToInt(szTmp);
		
		for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
		{
			szTmp[u4TmpIdx] = szBuf[u4BufIdx];
		}
		u4BufIdx++;
		szTmp[u4TmpIdx] = '\0';
		_arUpgCfg[_u4NoOfTags].u4DataSize = StrToInt(szTmp);
        #endif
        if (x_strncmp(&szBuf[u4BufIdx], "END", 3) == 0)
        {
            _u4NoOfTags++;
            break;
        }
    }
    
	
    return DLM_OK;
}

#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
static INT32 _LdrRcipUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    CHAR *szBuf, szTmp[64], szMIDL2BStr[4];
    INT32 i4Size = * ui4_len;
    UINT32 u4BufIdx = 0, u4TmpIdx;
	UINT8  u1ArrayIdx=0, u4Idx=0;
	UINT32 u4DataOffsetSize=0, u4DataUnitSize=0;	
	UINT32 u4RcipMIDL2B=0, u4RcipMIDMsk=0, u4TVMIDL2B=0, u4TVMIDMsk=0, u4RcipCmp=0, u4TVCmp=0;
	BOOL fgSkipParsing=FALSE;
    #ifndef CC_MAPLE_LOADER_UPGRADE_FLOW_DEBUG
	UINT32 _u4NoOfRecipeModels=0;
	UINT8 _u1PartitionArrayId=32;
	#endif

    #ifdef CC_MAPLE_LOADER_UPGRADE_NEW_RECIPE_SPEC
	UINT32 u4PartIdx = 0;
       UINT8 _u1TmpRcipCondData[32]={0};
       BOOL fgIsIndexGet=FALSE;
    #endif

	extern UINT32 StrToInt(const CHAR*);
	extern UINT32 StrToHex(const CHAR* pszStr, UINT32 u4Len);

    if (!pauc_buffer)
    {
        DEBUG("Memory buffer is NULL!\n");
        return DLM_OTHERS;
    }

    szBuf = (CHAR *)pauc_buffer;

#ifdef CC_MAPLE_LOADER_UPGRADE_NEW_RECIPE_SPEC

    UNUSED(EEPROM_Read(RECIPE_COND_DATA_ADDR, (UINT32)(void *)&_u1CurrentRcipCondData, sizeof(UINT8)*32));
    Printf("Current Recipe Condition Data:");
    for (u4TmpIdx = 0; u4TmpIdx < 32; u4TmpIdx++)
    {
        Printf("%x", _u1CurrentRcipCondData[u4TmpIdx]);
    }
    Printf("\n");

    while ( u4BufIdx < i4Size)
    {
        /*Find "START"*/
        while(0 != (x_strncmp(&szBuf[u4BufIdx], "START", 5)))
        {
            if (u4BufIdx >= i4Size-5)
            {
                DEBUG("START not found at recipe\n");
                return DLM_OTHERS;
            }

            u4BufIdx++;
        }

        u4BufIdx+= 6;

        /*Get tag name*/
        for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
        {
            szTmp[u4TmpIdx] = szBuf[u4BufIdx];
        }
        u4BufIdx++;
        szTmp[u4TmpIdx] = '\0';

        x_strncpy(_arRcipUpgCfg[_u4NoOfRecipeTags].szTagName, szTmp, 4);
        _arRcipUpgCfg[_u4NoOfRecipeTags].szTagName[4] = '\0';

        /*Search correct Recipe Condition Data mask*/
        for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
        {
             szTmp[u4TmpIdx] = szBuf[u4BufIdx];
        }

        u4BufIdx++;
        szTmp[u4TmpIdx] = '\0';

        while (x_strncmp(&szTmp[0], "default", 7) != 0)
        {
            for (u4TmpIdx = 0; u4TmpIdx < 32; u4TmpIdx++)
            {
                _u1TmpRcipCondData[u4TmpIdx] = StrToHex(&szTmp[2+u4TmpIdx],1);
            }

            //Printf("Tmp Recipe Condition Data string: %s\n", szTmp);
            Printf("Tmp Recipe Condition Data:");
	      for (u4TmpIdx = 0; u4TmpIdx < 32; u4TmpIdx++)
	      {
		    Printf("%x", _u1TmpRcipCondData[u4TmpIdx]);
		    if (_u1TmpRcipCondData[u4TmpIdx] & _u1CurrentRcipCondData[u4TmpIdx])
		    {
		        if (u4RcipCmp == 0)
		            u4RcipCmp = 1;
		    }
	      }
		Printf("\n");

             /*Get this index*/
		for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
             {
                 szTmp[u4TmpIdx] = szBuf[u4BufIdx];
             }

             u4BufIdx++;
             szTmp[u4TmpIdx] = '\0';
		Printf("Tmp buf: %s, %d\n", szTmp, StrToInt(szTmp));
		
            if (u4RcipCmp)
            {
                /*Recipe Condition Data mask match; save this index*/
                _arRcipUpgCfg[_u4NoOfRecipeTags].u1DataCaseIndex = StrToInt(szTmp);
                fgIsIndexGet = TRUE;
                break;
            }
            else
            {              
              
                /*Search another mask*/
                for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
                {
                    szTmp[u4TmpIdx] = szBuf[u4BufIdx];
                }

                u4BufIdx++;
                szTmp[u4TmpIdx] = '\0';
            }
        }

        if (!fgIsIndexGet)
        {
            _arRcipUpgCfg[_u4NoOfRecipeTags].u1DataCaseIndex = StrToInt(szBuf[u4BufIdx]);
            fgIsIndexGet = TRUE;
        }

         /*Find "END"*/
        while(0 != (x_strncmp(&szBuf[u4BufIdx], "END", 3)))
        {
            if (u4BufIdx >= i4Size-3)
            {
                DEBUG("END not found at recipe\n");
                return DLM_OTHERS;
            }

            u4BufIdx++;
        }
        u4BufIdx+=4;
        _u4NoOfRecipeTags++;
    }

    Printf("\t_u4NoOfRecipeTags: %d\n", _u4NoOfRecipeTags);

    for(u4PartIdx = 0; u4PartIdx < _u4NoOfRecipeTags; u4PartIdx++)
    {
        Printf("\tszTagName: %s\n", _arRcipUpgCfg[u4PartIdx].szTagName);
        Printf("\tu1DataCaseIndex: %d\n", _arRcipUpgCfg[u4PartIdx].u1DataCaseIndex);
    }

#else

    while(0 != (x_strncmp(&szBuf[u4BufIdx], "START", 5)))
    {
        if (u4BufIdx >= i4Size-5)
        {
            DEBUG("START not found at recipe\n");
            return DLM_OTHERS;
        }

        u4BufIdx++;
    }
    u4BufIdx+= 6;

    _u1CurrentModelIdx = 128;
    for ( ; u4BufIdx < i4Size; _u4NoOfRecipeModels++)
    {
        if (_u4NoOfRecipeModels >= MAX_UPG_MODEL_NUM)
        {
            Printf("warning! recipe model number is over limitation(=%d)!\n", MAX_UPG_MODEL_NUM);
            break;
        }

        for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
        {
            szTmp[u4TmpIdx] = szBuf[u4BufIdx];
        }

        u4BufIdx++;
        szTmp[u4TmpIdx] = '\0';

        x_strncpy(_arRcipUpgCfg[_u4NoOfRecipeModels].szModelName, szTmp, 32);
        _arRcipUpgCfg[_u4NoOfRecipeModels].szModelName[32] = '\0';

        for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
        {
            szTmp[u4TmpIdx] = szBuf[u4BufIdx];
        }

        u4BufIdx++;
        szTmp[u4TmpIdx] = '\0';
		_arRcipUpgCfg[_u4NoOfRecipeModels].u4ModelIDMask = StrToHex(szTmp,4);

		szMIDL2BStr[0]= _arRcipUpgCfg[_u4NoOfRecipeModels].szModelName[28];
		szMIDL2BStr[1]= _arRcipUpgCfg[_u4NoOfRecipeModels].szModelName[29];
		szMIDL2BStr[2]= _arRcipUpgCfg[_u4NoOfRecipeModels].szModelName[30];
		szMIDL2BStr[3]= _arRcipUpgCfg[_u4NoOfRecipeModels].szModelName[31];

        u4RcipMIDL2B = StrToHex(szMIDL2BStr,4);
		u4RcipMIDMsk = _arRcipUpgCfg[_u4NoOfRecipeModels].u4ModelIDMask;
		u4RcipCmp = u4RcipMIDL2B & u4RcipMIDMsk;

        u4TVMIDL2B = 0x2566;
		u4TVMIDMsk = 0x0F0F;
		u4TVCmp = u4TVMIDL2B & u4TVMIDMsk;
		
        if(u4RcipCmp != u4TVCmp)
        {
          fgSkipParsing = TRUE;
          for (u4TmpIdx = 0; szBuf[u4BufIdx] != 'E'; u4TmpIdx++, u4BufIdx++)
          {
          }
		  if (x_strncmp(&szBuf[u4BufIdx], "END", 3) == 0)
		  {
			u4BufIdx+= 4;
		  }			  
        }
		else
		{
		  _u1CurrentModelIdx = (UINT8)_u4NoOfRecipeModels;
		  fgSkipParsing = FALSE;
		}
        if(!fgSkipParsing)
        {		
        for(u1ArrayIdx =0; u1ArrayIdx<32; u1ArrayIdx++)
        {
            for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
            {
                szTmp[u4TmpIdx] = szBuf[u4BufIdx];
            }
            u4BufIdx++;
            szTmp[u4TmpIdx] = '\0';
            _arRcipUpgCfg[_u4NoOfRecipeModels].u1PartitionID[u1ArrayIdx] = (UINT8)StrToInt(szTmp);
            for (u4TmpIdx = 0; szBuf[u4BufIdx] != '\n'; u4TmpIdx++, u4BufIdx++)
            {
                szTmp[u4TmpIdx] = szBuf[u4BufIdx];
            }
            u4BufIdx++;
            szTmp[u4TmpIdx] = '\0';
            _arRcipUpgCfg[_u4NoOfRecipeModels].u1DataOffsetIdx[u1ArrayIdx] = (UINT8)StrToInt(szTmp);
			u4DataUnitSize = 0;
            for (u4Idx = 0; u4Idx < _u4NoOfTags; u4Idx++)
            {
                if(_arRcipUpgCfg[_u4NoOfRecipeModels].u1PartitionID[u1ArrayIdx] == _arUpgCfg[u4Idx].u1PartitionID)
                {
                    u4DataUnitSize = _arUpgCfg[u4Idx].u4DataSize;
					break;
                }
            }
			u4DataOffsetSize = (UINT32)((_arRcipUpgCfg[_u4NoOfRecipeModels].u1DataOffsetIdx[u1ArrayIdx]) * u4DataUnitSize);
			_arRcipUpgCfg[_u4NoOfRecipeModels].u4DataSkipCounter[u1ArrayIdx] = (UINT32)((u4DataOffsetSize&0xFFF00000)>>20);
			_arRcipUpgCfg[_u4NoOfRecipeModels].u4DataOfstToWrite[u1ArrayIdx] = (UINT32)(u4DataOffsetSize&0x000FFFFF);;
			
            if (x_strncmp(&szBuf[u4BufIdx], "END", 3) == 0)
            {
                _u1PartitionArrayId = u1ArrayIdx;
                u4BufIdx+= 4;
                break;
            }			
        }
        }
        if (x_strncmp(&szBuf[u4BufIdx], "RECIPEEND", 9) == 0)
        {
            _u4NoOfRecipeModels++;
			_u1PartitionArrayId++;
			if(_u1CurrentModelIdx == 128)
			{
			  _u1RecipeIDMatch = FALSE;
			}
            break;
        }
    }

	Printf("Current Recipe Model No: %d\n", _u1CurrentModelIdx);
	Printf("\tModelName: %s\n", _arRcipUpgCfg[_u1CurrentModelIdx].szModelName);
	Printf("\tModelIDMask: 0x%04x\n", _arRcipUpgCfg[_u1CurrentModelIdx].u4ModelIDMask);
    #ifdef CC_MAPLE_LOADER_UPGRADE_FLOW_DEBUG
	for(u4PartIdx = 0; u4PartIdx < _u1PartitionArrayId; u4PartIdx++)
	{
	  Printf("\tPartitionID: %d\n", _arRcipUpgCfg[_u1CurrentModelIdx].u1PartitionID[u4PartIdx]);
	  Printf("\tDataOffsetIdx: %d\n", _arRcipUpgCfg[_u1CurrentModelIdx].u1DataOffsetIdx[u4PartIdx]);
	  Printf("\tu4DataSkipCounter: %d\n", _arRcipUpgCfg[_u1CurrentModelIdx].u4DataSkipCounter[u4PartIdx]);
	  Printf("\tu4DataOfstToWrite: %d\n", _arRcipUpgCfg[_u1CurrentModelIdx].u4DataOfstToWrite[u4PartIdx]);	  
	}
    #endif
#endif
    return DLM_OK;
}
#endif

static void _LdrDlmTagVerChkFn(CHAR* szFileNameWithPath)
{
#ifdef LOADER_USB_UPGRADE

    PFN_DLM_GET_TAG_VER_CB_FUNC pfnGetVerCb = NULL;
    PFN_DLM_CMP_TAG_VER_CB_FUNC pfnCmpVerCb = NULL;
    Append_T* prAppendTag = NULL;
    CHAR szTagData[64];
    CHAR szUpgVer[128];
    CHAR szCurVer[32];
    UCHAR* szTag;
    UCHAR a_szTag[5];
    UINT32 u4VerOff, u4VerLen;

#if 0
    UTIN32 u4TagId = 0;
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
#endif

    pfnGetVerCb = (PFN_DLM_GET_TAG_VER_CB_FUNC)DRVCUST_InitGet(eLoaderUpgradeDlmGetTagVerCbFunc);
    pfnCmpVerCb = (PFN_DLM_CMP_TAG_VER_CB_FUNC)DRVCUST_InitGet(eLoaderUpgradeDlmCmpTagVerCbFunc);
    u4VerOff = DRVCUST_InitGet(eLoaderUpgradeDlmVerDataOff);
    u4VerLen = DRVCUST_InitGet(eLoaderUpgradeDlmVerDataLen);

    // 0.check if there are callback functions, return this function if not define
#ifdef CC_DEF_PART_VER_CHK_FUNC
	_i1SavePartVersion = 0;
	if (pfnGetVerCb == NULL)
	{
		pfnGetVerCb = LOADER_DefUpgradeDlmGetTagVerCbFunc;
	}
	if (pfnCmpVerCb == NULL)
	{
		pfnCmpVerCb = LOADER_DefUpgradeDlmCmpTagVerCbFunc;
	}
#else
    if ((pfnGetVerCb == NULL) || (pfnCmpVerCb == NULL))
    {
        return;
    }
#endif
    // 0.check if size of local variable is enough
    if (u4VerOff < 4) //must be after 4 bytes tag, e.g. "iMtk"
    {
        ASSERT(0);
        return;
    }

    if ((u4VerOff + u4VerLen) > sizeof(szTagData))
    {
        ASSERT(0);
        return;
    }

    if (u4VerLen > sizeof(szUpgVer))
    {
        ASSERT(0);
        return;
    }

#if 0
    // 0.reset loader env.
    x_memset((void*)prLdrEnv->arTagVer, 0, sizeof(prLdrEnv->arTagVer));
#endif

    // 1.get all tags from x_dlm_get_codefile_tag_list()
    prAppendTag = x_dlm_get_codefile_tag_list(szFileNameWithPath);

    // 2.for each tag
    while (prAppendTag != NULL)
    {
        szTag = prAppendTag->uc_type;

        x_memcpy((void*)a_szTag, (void*)szTag, 4);
        a_szTag[4] = 0;

        // 2.1 get PKG version by x_dlm_tag_test()
        x_dlm_tag_test(szFileNameWithPath, szTag, NULL, szTagData, sizeof(szTagData));
        Printf("a_szTag=%s, szTagData=%s\n", a_szTag, szTagData);

        // 2.2 check if it starts with a DLM_APPEND_HDR_MAGIC, i.e."iMtk"
        if (x_strncmp(szTagData, "iMtK", 4) == 0)
        {
            x_memset(szUpgVer, 0, sizeof(szUpgVer));
            x_memset(szCurVer, 0, sizeof(szCurVer));

            u4VerLen = *(UINT32*)&szTagData[u4VerOff - 4];
            x_strncpy(szUpgVer, &szTagData[u4VerOff], u4VerLen);
            Printf("szUpgVer=%s", szUpgVer);

#ifdef CC_DEF_PART_VER_CHK_FUNC
			if (!x_dlm_is_version_validate())
			{
				Printf("%s,%d->upgrade ALLOW\n", __FUNCTION__, __LINE__);
				_i1SavePartVersion = 1;
				prAppendTag = prAppendTag->pt_next;
				continue;
			}
#endif
            // 2.2.1 get current version by (PFN_GET_TAG_VER_FUNC)DRVCUST_InitGet(eLoaderUpgradeDlmGetTagVersionCbFunc)
            pfnGetVerCb(a_szTag, szCurVer);
            Printf(", szCurVer=%s\n", szCurVer);

            // 2.2.2 save the current version to loader env.
#if 0
            if (u4TagId < CC_LDR_ENV_TAG_NUM)
            {
                x_strncpy(prLdrEnv->arTagVer[u4TagId].szTag, (CHAR*)szTag, 4);
                x_strncpy(prLdrEnv->arTagVer[u4TagId].szVersion, szCurVer, u4VerLen);
                u4TagId++;
            }
            else
            {
                ASSERT(0);
                return;
            }
#endif

            // 2.2.3 compare version by (PFN_CMP_TAG_VER_FUNC)DRVCUST_InitGet(eLoaderUpgradeDlmCmpTagVersionCbFunc)
            if (pfnCmpVerCb(a_szTag, szCurVer, szUpgVer) == 0)
            {
                Printf("->upgrade NOT ALLOW\n", szUpgVer);
                // 2.2.3.1 if not allow, call x_dlm_unreg_append_callback()
                x_dlm_unreg_append_callback(szTag);
            }
            else
            {
                Printf("->upgrade ALLOW\n", szUpgVer);
#ifdef CC_DEF_PART_VER_CHK_FUNC
				_i1SavePartVersion = 1;
#endif
            }
        }

        prAppendTag = prAppendTag->pt_next;
    }
#endif
}

static INT32 _LdrDlmVerChkFn(char *ps_version)
{
    IMAGE_UPGRADE_HEADER_T *prImgUpHdr = NULL;
    UINT32 u4Checksum;
    UINT32 *pu4Val;
    PFN_VER_CMP_FUNC pfnVerCmp = NULL;

    Printf("Download firmware version - %s\n", ps_version);

#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) // nand or emmc boot
    UNUSED(prImgUpHdr);
    UNUSED(u4Checksum);
    UNUSED(pu4Val);
#else

    prImgUpHdr = (IMAGE_UPGRADE_HEADER_T *)(PBI_A_BASE + FIRMWARE_FLASH_OFFSET);
    // How to check current version?
    // 1. If there is no 539P header in norflash, current version is NULL, always return DLM_OK.
    if (prImgUpHdr->u4Signature != IMAGE_SIGNATURE_UPGRADE)
    {
        return DLM_OK;  // zero to allow upgrade
    }

    // 2. check image checksum, if checksum is not equal to 5382P header.u4FirmwareChksum, always return DLM_OK.
    u4Checksum = 0;
    pu4Val = (UINT32 *)(PBI_A_BASE + FIRMWARE_FLASH_OFFSET + sizeof(IMAGE_UPGRADE_HEADER_T));
    while ((((UINT32)pu4Val) - (PBI_A_BASE + FIRMWARE_FLASH_OFFSET + sizeof(IMAGE_UPGRADE_HEADER_T))) < prImgUpHdr->u4Size)
    {
        u4Checksum += *pu4Val;
        pu4Val++;
    }

    if (u4Checksum != prImgUpHdr->u4FirmwareChksum)
    {
        Printf("Image Checksum is incorrect chk:0x%08x firm:0x%08x\n", u4Checksum, prImgUpHdr->u4FirmwareChksum);
        return DLM_OK;  // zero to allow upgrade
    }
#endif

    pfnVerCmp = (PFN_VER_CMP_FUNC)DRVCUST_InitGet(eLoaderUpgradeVerCmpFunc);
    if (pfnVerCmp == NULL)
    {
        return DLM_OK;  // zero to allow upgrade
    }

    // 3. There is a version string in 539P header.au1Version,
    //    pass this version string and ps_version to customization version compare function.
    //    return the result of the version compare function.
    //    zero to allow upgrade, otherwise to reject upgrade.
    return pfnVerCmp((char *)prImgUpHdr->au1Version, ps_version);
}

VOID _LdrDlmInstallCallBack(UINT32 u4Upgrade)
{
    PFN_DLM_CB_FUNC pfnDlmCb = NULL;

    UCHAR        all[] = "all";
    UCHAR        firm[] = "firm";
	UCHAR        fir2[] = "fir2";
	UCHAR        micp[] = "micp";
	UCHAR        nor1[] = "nor1";
	UCHAR        f8281[] = "8281";
	UCHAR        load[] = "load";

    // Set version compare callback function.
    x_dlm_reg_version_compare_callback((dlm_ver_chk_fn)_LdrDlmVerChkFn);

    // Check upgrade bit.
    if (u4Upgrade & PC_UPGRADE_CTRL_NOR_1)
    {
#if defined(CC_NAND_BOOT) // nand boot
        INT32 i4NandBlkSize;

        i4NandBlkSize = Loader_GetNandFlashBlockSize();
        if(i4NandBlkSize <= 0)
        {
            _u4BufSize = 16*1024;
        }
        else
        {
            _u4BufSize = (UINT32)i4NandBlkSize;
        }

        Printf("NAND Erase Unit:0x%x\n", _u4BufSize);

#elif defined(CC_EMMC_BOOT) // emmc boot

        INT32 i4MsdcEraseSize;

        i4MsdcEraseSize = Loader_GetMsdcEraseSize();
        if(i4MsdcEraseSize <= 0)
        {
            _u4BufSize = 128*1024;
        }
        else
        {
            _u4BufSize = (UINT32)i4MsdcEraseSize;
        }

        Printf("Msdc Erase Unit:0x%x\n", _u4BufSize);

#else // nor boot

        _u4BufSize = 64*1024;
        Printf("Nor Erase Unit:0x%x\n", _u4BufSize);
#endif

        x_dlm_reg_append_callback(all,  _u4BufSize, _LdrAllUpgrade);
        x_dlm_reg_append_callback(firm, _u4BufSize, _LdrFirmUpgrade);
        x_dlm_reg_append_callback(fir2, _u4BufSize, _LdrFir2Upgrade);
        x_dlm_reg_append_callback(micp, _u4BufSize, _LdrMicpUpgrade);
        x_dlm_reg_append_callback(nor1, _u4BufSize, _LdrFirmUpgrade);
        x_dlm_reg_append_callback(f8281, _u4BufSize, _Ldr8281Upgrade);
        x_dlm_reg_append_callback(load, _u4BufSize, _LdrLoaderUpgrade);

        pfnDlmCb = (PFN_DLM_CB_FUNC)DRVCUST_InitGet(eLoaderUpgradeDlmCbFunc);
        if (pfnDlmCb != NULL)
        {
            pfnDlmCb(_u4BufSize);
        }
    }
}

static INT32 _LdrSetUpgradeState(UINT8 u1Ch)
{
#ifdef CC_CUST_USB_BREAK_HANDLE_SUPPORT
    UINT32 u4UpgradeGoAddr;
    UINT8  u1UpgradeState = 0;

    if (DRVCUST_InitQuery(eLoaderUpgradeBreakStateOffset, &u4UpgradeGoAddr) == 0)
    {
        UNUSED(EEPROM_Read(u4UpgradeGoAddr, (UINT32)(void *)&u1UpgradeState, sizeof(UINT8)));

        if (u1Ch == 0)
        {
            u1UpgradeState &= (UINT8)(~(DTVCFG_FLAG_USB_UPGRADE_ON));
        }
        else if (u1Ch == 1)
        {
            u1UpgradeState |= (DTVCFG_FLAG_USB_UPGRADE_ON);
        }

        UNUSED(EEPROM_Write(u4UpgradeGoAddr, (UINT32)(void *)&u1UpgradeState, sizeof(UINT8)));
    }
#else
    UNUSED(u1Ch);
#endif /* CC_CUST_USB_BREAK_HANDLE_SUPPORT */

    return 0;
}

static INT32 _LdrSetT8032Cmd(UINT8 u1Cmd, UINT8 u1SubCmd)
{
#ifndef CC_MT8223
    #if defined(CC_MTK_LOADER)
        PDWNC_T8032_CMD_T rCmd;

        rCmd.u1Cmd = u1Cmd;
        rCmd.u1SubCmd = u1SubCmd;

        return PDWNC_T8032Cmd(&rCmd, NULL);
    #else
        return 0;
    #endif

#else

    UNUSED(u1Cmd);
    UNUSED(u1SubCmd);
    return 0;
#endif
}
UINT32 _fgupgrading =0;
static INT32 _LdrDlmHandler(UINT32 u4Upgrade)
{
    PFN_GPIO_LEDCTRL_FUNC pfnLedCtrlFunc = NULL;
    DEFAULT_CODE_FILE_HDR_T t_code_hdr;
    char szFileNameWithPath[96];
    UINT32 u4Idx = 0;
    INT32 i4Ret = 0;
    #ifdef CC_LDR_ATOMIC_MODE_SUPPORT
	UINT32 u4Offset, u4Len, u4ereaseindex;
	CHAR atomic_boot[]= "boot";
    #endif
    #ifdef CC_MAPLE_BE_LOADER_BOOT_FLOW
	UINT32 u4DdriPartOfst, u4DdriPartSize;
	CHAR ddri[]= "ddri";
    #endif
    #if defined(CC_SECURESTORAGE_SUPPORT)
	UINT8 auSecureDataZone[256];
    #endif
    #ifdef CC_DLM_V2
	UINT32 au4Key[64];
    #endif

    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
    x_memcpy(t_code_hdr.uac_id, (void *)(prLdrEnv->u.rHdr.szCustomer), 4);
    x_memcpy(t_code_hdr.s_model, (void *)(prLdrEnv->u.rHdr.szModel), 32);
	#ifdef DLM_USE_AES_MODE
    x_strncpy((char *)t_code_hdr.s_version, (void *)(prLdrEnv->u.rHdr.szVersion), 56);
	#else
	x_strncpy((char *)t_code_hdr.s_version, (void *)(prLdrEnv->u.rHdr.szVersion), 64);
	#endif
    if((_fgDeviceType == 0x02) || (_fgDeviceType == 0x00))
    {
        x_strcpy(szFileNameWithPath, USB_ROOT_PATH);
    }
    else
    {
        x_strcpy(szFileNameWithPath, MSDC_ROOT_PATH);
    }
    x_strcat(szFileNameWithPath, "/");
    x_strcat(szFileNameWithPath, _szFileName);

    /* register dlm callback API */
    _LdrDlmInstallCallBack(u4Upgrade);

    x_dlm_set_buffer(0x800000);
    x_dlm_boot_init((CHAR*)&t_code_hdr);

    #ifdef CC_DLM_V2
	#if defined(CC_SECURESTORAGE_SUPPORT)
	_LdrReadFromSecureStorage(0,auSecureDataZone,256);
	if((auSecureDataZone[0] != 1) && BIM_IS_SECURE_BOOT)
	{
	  //please set CVPK2 RSA/AES Key2
      x_memcpy((void*)au4Key, (void*)prLdrEnv->au4CustKey, 256);
	  x_dlm_set_customer_key(au4Key);
	  x_dlm_set_customer_aes_key((0xfb005000 + sizeof(au4Key)),16,((0xfb005000 + sizeof(au4Key))+16),16);	  
	}
	else
	{
	  //please set default RSA/AES Key2
	  x_dlm_set_customer_key(au1RSAPublicKey2);
	  x_dlm_set_customer_aes_key(au1AesKey2,16,au1AesIv,16);
	}
	#else
	if (BIM_IS_SECURE_BOOT)
		{   
			// get vendor public key
    		x_memcpy((void*)au4Key, (void*)prLdrEnv->au4CustKey, 256);
			x_dlm_set_customer_key(au4Key);
			x_dlm_set_customer_aes_key((0xfb005000 + sizeof(au4Key)),16,((0xfb005000 + sizeof(au4Key))+16),16);
		}
	else
		{
			x_dlm_set_customer_key(au1RSAPublicKey2);
			x_dlm_set_customer_aes_key(au1AesKey2,16,au1AesIv,16);
		}

    #endif
    #endif
	
    _LdrSetUpgradeState(1);
    _LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 1);
    UNUSED(DRVCUST_InitQuery(eGpioLedCtrlFunc, (UINT32 *)(void *)&pfnLedCtrlFunc));
    if (pfnLedCtrlFunc != NULL)
    {
        pfnLedCtrlFunc(eLedDetectBlink);
    }

    if ((x_dlm_integrity_check((CHAR*)szFileNameWithPath, NULL)) == DLM_OK)
    {
        if (pfnLedCtrlFunc != NULL)
        {
            pfnLedCtrlFunc(eLedUpgradeBlink);
        }
        Printf("Do loader usb upgrade....");

#if defined(CC_SECURE_BOOT) && defined(CC_LOADER_VERIFY_PKG)

        if (BIM_IS_SECURE_BOOT)
        {
        	UCHAR usig[]= "usig";
        	prLdrEnv->u4CfgFlag = USIG_VERIFY_FAIL;
        	x_dlm_reg_append_callback(usig, 512, _LdrUsigUpgrade);
        	x_dlm_download_by_tag(szFileNameWithPath, usig, NULL);
        	x_dlm_unreg_append_callback(usig);

        	if (_u4UsigCmd != USIG_DO_UPGRADE)
        	{
            Printf("secure upgrade stop, usigcmd=\'%c\'(0x%x)\n", _u4UsigCmd, _u4UsigCmd);
            return 1;
        	}
        }
#endif  
			  UCHAR cfig[]= "cfig";

        x_dlm_reg_append_callback(cfig, 1024, _LdrCfigUpgrade);
        
        
		x_dlm_download_by_tag(szFileNameWithPath, cfig, NULL);
        x_dlm_unreg_append_callback(cfig);
        #ifdef CC_MAPLE_LOADER_UPGRADE_VERSION_CTRL_FLOW
        i4Ret = _LdrCheckVersion();
		if(!i4Ret)
		{
          if (pfnLedCtrlFunc != NULL)
          {
            pfnLedCtrlFunc(eLedUpgradeFinish);
          }
          _fgupgrading =0;
          Printf("SKIP upgrade!\n");
          _u4Upgraded++;
		  return 0;
		}
        #endif

        for (u4Idx = 0; u4Idx < _u4NoOfTags; u4Idx++)
        {
            Printf("Dynamic Tag No: %d\n", u4Idx);
            Printf("\tTagName: %s\n", _arUpgCfg[u4Idx].szTagName);
            Printf("\tFlashType: %d\n", _arUpgCfg[u4Idx].eFlashType);
            Printf("\tPartitionID: %d\n", _arUpgCfg[u4Idx].u1PartitionID);
			#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
            Printf("\tRecipesupport: %d\n", _arUpgCfg[u4Idx].fgSupportRecipeUpgarde);
            Printf("\tDataAmounts: %d\n", _arUpgCfg[u4Idx].u1DataAmounts);
			Printf("\tDataSize: %d\n", _arUpgCfg[u4Idx].u4DataSize);
			#endif
          #ifdef CC_ONE_IMG_FOR_MULTI_NAND
			if(_arUpgCfg[u4Idx].fgNandMatch)
		  #endif
			{
                #ifdef CC_LDR_ATOMIC_MODE_SUPPORT
				if(_u1UpgradeMode != 0)
				{
				    if ((x_strncmp(_arUpgCfg[u4Idx].szTagName, atomic_boot, x_strlen(atomic_boot)) == 0))
		            {
			            i4Ret = x_dlm_reg_append_callback2((UCHAR *)_arUpgCfg[u4Idx].szTagName,  _u4BufSize, _LdrDynamicUpgrade);
		            }
		            else
	                {
			            Printf("_LdrDlmHandler skip upgrade in atomic mode!!\n");
		            }
				}
				else
				{
            	i4Ret = x_dlm_reg_append_callback2((UCHAR *)_arUpgCfg[u4Idx].szTagName,  _u4BufSize, _LdrDynamicUpgrade);
			}
                #else
				i4Ret = x_dlm_reg_append_callback2((UCHAR *)_arUpgCfg[u4Idx].szTagName,  _u4BufSize, _LdrDynamicUpgrade);
				#endif			
			}
            if (i4Ret != DLM_OK)
            {
                DEBUG("Warning: x_dlm_reg_append_callback2 return %d\n", i4Ret);
            }
        }
		
		#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW
        UCHAR rcip[]= "rcip";
        x_dlm_reg_append_callback(rcip, 1024, _LdrRcipUpgrade);
        x_dlm_download_by_tag(szFileNameWithPath, rcip, NULL);
        x_dlm_unreg_append_callback(rcip);
		//#ifdef CC_MAPLE_LOADER_UPGRADE_FLOW_DEBUG
		#if 0
        for (u4Idx = 0; u4Idx < _u4NoOfRecipeModels; u4Idx++)
        {
            Printf("Recipe Model No: %d\n", u4Idx);
            Printf("\tModelName: %s\n", _arRcipUpgCfg[u4Idx].szModelName);
			for(u4PartIdx = 0; u4PartIdx < _u1PartitionArrayId; u4PartIdx++)
			{
              Printf("\tPartitionID: %d\n", _arRcipUpgCfg[u4Idx].u1PartitionID[u4PartIdx]);
			  Printf("\tDataOffsetIdx: %d\n", _arRcipUpgCfg[u4Idx].u1DataOffsetIdx[u4PartIdx]);
			}
        }
		#endif
        #endif
		
        _fgupgrading =1;
        // version control of each tag
        _LdrDlmTagVerChkFn((CHAR*)szFileNameWithPath);
#ifndef LOADER_UPGRADE_IN_ONE_STEP
#ifdef CC_EEP2EMMC_DUAL_BANK_SUPPORT
        // trigger nvih eeprom header first
        UCHAR NVIH[]= "nvih";
        if (x_dlm_download_by_tag(szFileNameWithPath, NVIH, NULL) == DLM_OK)
        {
            x_dlm_unreg_append_callback(NVIH);
        }
#endif
        // trigger loader partition first
        UCHAR load[]= "load";
        if (x_dlm_download_by_tag(szFileNameWithPath, load, NULL) == DLM_OK)
        {
            x_dlm_unreg_append_callback(load);
            _LdrLoaderUpgradeReboot();
        }
#endif
#ifdef CC_MAPLE_BE_LOADER_BOOT_FLOW
        if ((LDR_BECheckDDRI()) != TRUE)
        {
            x_dlm_unreg_append_callback(ddri);
            #if defined(CC_USE_2TB_EMMC)
		    u4DdriPartOfst = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0+26));
            u4DdriPartSize = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartSize0+26));
            #else
			u4DdriPartOfst = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+26));
            u4DdriPartSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0+26));
            #endif
			UNUSED(MsdcEraseCard(u4DdriPartOfst, u4DdriPartSize, 0));
			Printf("loader upgrade remove ddri in 2K model\n");
        }
#endif
        if ((x_dlm_download((CHAR*)szFileNameWithPath, NULL, 0)) != DLM_OK)
        {
            if (pfnLedCtrlFunc != NULL)
            {
                pfnLedCtrlFunc(eLedUpgradeFail);
            }

            //Printf("Fail!\n");
			Printf("loader upgrade failed \n");
            _LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
            _fgupgrading =0;
			while(1);
           // return 1;
        }

#ifdef CC_USB_UPGRADE_CHECK_FILE_VERSION 
        write_new_filename_version();
#endif
#ifdef CC_MAPLE_LOADER_UPGRADE_VERSION_CTRL_FLOW
        write_new_pkg_cfg_version();
#endif
#ifdef CC_DEF_PART_VER_CHK_FUNC
		write_new_pkg_part_version((CHAR*)szFileNameWithPath);
#endif
        if (pfnLedCtrlFunc != NULL)
        {
            pfnLedCtrlFunc(eLedUpgradeFinish);
        }
        _fgupgrading =0;
        Printf("Success!\n");
        _u4Upgraded++;
        #ifdef CC_LDR_ATOMIC_MODE_SUPPORT
	    if(_u1UpgradeMode != 0)
		{
            #if defined(CC_USE_2TB_EMMC)
		    u4Offset = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0+13));
            u4Len = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartSize0+13));			
            #else
			u4Offset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0+13));
            u4Len = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0+13));		
            #endif
			UNUSED(MsdcEraseCard(u4Offset, u4Len, 0));
			Printf("_LdrDlmHandler erease basic emmc (%d)\n",u4Len);
            #if defined(CC_USE_2TB_EMMC)
		    u4Offset = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0+24));
            u4Len = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartSize0+24));			
            #else
		    u4Offset = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0+24));
            u4Len = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartSize0+24));			
            #endif
			UNUSED(MsdcEraseCard(u4Offset, u4Len, 0));
			Printf("_LdrDlmHandler erease upgrade emmc (%d)\n",u4Len);			
		}
		#endif
    }
    else
    {
        Printf("No proper firmware exist, ");
        if (pfnLedCtrlFunc != NULL)
        {
            pfnLedCtrlFunc(eLedUpgradeFail);
        }

        _LdrSetUpgradeState(0);
        _LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
        return 1;
    }

    _LdrSetUpgradeState(0);
    _LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);

    return 0;
}
#ifdef CC_USB_UPGRADE_CHECK_FILE_VERSION
// @ret
//      1   continue to upgrade
//      0   don't upgrade
static INT32 _LdrUsbCheckFileVersion(char *szFileName)
{
    CHAR* szPrefix = "";
    CHAR* szSuffix = "";
    CHAR szMajorVer[3];
    CHAR szMinorVer[3];
    UINT8 u1MajorVer;
    UINT8 u1MinorVer;
    UINT32 u4PrefixLen;
    UINT8 i;

    if (DRVCUST_InitGet(eUsbUpgradeFileNamePrefix) != 0 && DRVCUST_InitGet(eUsbUpgradeFileNameSuffix) != 0)
    {
        szPrefix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeFileNamePrefix);
        szSuffix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeFileNameSuffix);
    }
	#if 0
    if ((x_strlen(szPrefix) == 0) ||
        (x_strlen(szPrefix) > 0 && x_strncmp(szFileName, szPrefix, x_strlen(szPrefix)) != 0) )
    {
        if (DRVCUST_InitGet(eUsbUpgradeFileNamePrefixSecondary) != 0 && DRVCUST_InitGet(eUsbUpgradeFileNameSuffixSecondary) != 0)
        {
            szPrefix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeFileNamePrefixSecondary);
            szSuffix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeFileNameSuffixSecondary);
        }
    }
    #endif
    u4PrefixLen = x_strlen(szPrefix);

    if (x_strlen(szFileName) != u4PrefixLen + x_strlen(szSuffix) + 4)
    {
        Printf("Can't recognize file verison, filename length incorrect\n");
        return 0;
    }

    //The file format is 4 char dec, check if we can get the . first
    for (i=0; i<4; ++i)
    {
        if(szFileName[u4PrefixLen + i] < '0' || szFileName[u4PrefixLen + i] > '9' )
        {
            Printf("Can't recognize file verison, not digital, stop upgrade\n");
            return 0;
        }
    }
    
    szMajorVer[0] = szFileName[u4PrefixLen];
    szMajorVer[1] = szFileName[u4PrefixLen + 1];
    szMajorVer[2] = '\0';
    szMinorVer[0] = szFileName[u4PrefixLen + 2];
    szMinorVer[1] = szFileName[u4PrefixLen + 3];
    szMinorVer[2] = '\0';

    u1MajorVer = (UINT8)StrToInt(szMajorVer);
    u1MinorVer = (UINT8)StrToInt(szMinorVer);

    if(u1MajorVer == 0 && u1MinorVer == 0) 
    {
        Printf("Version is 0000, force to upgrade\n");
        backup_new_filename_version(u1MajorVer, u1MinorVer);

        return 1;
    }
    else 
    {
        UINT8 u1PreviousMajorVer = 0;
        UINT8 u1PreviousMinorVer = 0;
        UNUSED(EEPROM_Read(LOADER_USB_UPGRADE_MAJOR_VERSION,
                           (UINT32)(VOID *)&u1PreviousMajorVer, sizeof(UINT8)));
        UNUSED(EEPROM_Read(LOADER_USB_UPGRADE_MINOR_VERSION,
                           (UINT32)(VOID *)&u1PreviousMinorVer, sizeof(UINT8)));

        // if 0xff means there is no previous version in system 
        if(u1PreviousMinorVer == 0xff || u1PreviousMinorVer == 0xff)
        {
            Printf("Can't get previous version, force to upgrade\n");
            backup_new_filename_version(u1MajorVer, u1MinorVer);
            return 1;            
        }
        else 
        {
            Printf("System verion: [%d][%d], PKG version: [%d][%d] ", u1PreviousMajorVer, 
                u1PreviousMinorVer, u1MajorVer, u1MinorVer);
            //version comparsion
            if((u1MajorVer > u1PreviousMajorVer) || 
                ((u1MajorVer == u1PreviousMajorVer) && (u1MinorVer > u1PreviousMinorVer))) 
            {
                Printf("continue to upgrade\n");
                backup_new_filename_version(u1MajorVer, u1MinorVer);

                return 1;
            }
            else 
            {
                Printf("Do not upgrade due to filename version check\n");
                return 0;
            }
        }
    }

    Printf("Do not upgrade\n");
    return 0;
}
#endif
static INT32 _LdrUsbFileRead()
{
    char szFileNameWithPath[96];
    HANDLE_T rImgFile;
    FM_FILE_INFO_T  rFileInfo;
    HANDLE_T    h_dir;

    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    BOOL fgExactTarget = FALSE;
    UINT32 u4FuzzyTargetCount = 0;
    CHAR szFuzzyFileName[64];
    CHAR* szPrefix = "";
    CHAR* szSuffix = "";

    if (x_fm_open_dir(FM_ROOT_HANDLE, USB_ROOT_PATH, &h_dir) != FMR_OK)
    {
        Printf("Open USB root directory fail, ");
        return 1;
    }

    x_memset(szFileNameWithPath, 0, sizeof(szFileNameWithPath));
    x_memset(szFuzzyFileName, 0, sizeof(szFuzzyFileName));

    if (DRVCUST_InitGet(eUsbUpgradeFileNamePrefix) != 0)
    {
        szPrefix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeFileNamePrefix);
    }
	#ifndef CC_MTK_AUTO_TEST
    if (DRVCUST_InitGet(eUsbUpgradeFileNameSuffix) != 0)
    {
        szSuffix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeFileNameSuffix);
    }
	#else
	{
		if (EepromSuffixFlag  == 1)
			{
			
        		if (DRVCUST_InitGet(eUsbUpgradeGolednFileNameSuffix) != 0)
        		{
        			szSuffix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeGolednFileNameSuffix);
        		}
			}
		else
			{

    if (DRVCUST_InitGet(eUsbUpgradeFileNameSuffix) != 0)
    {
        szSuffix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeFileNameSuffix);
    }
			}
	}
#endif
#ifdef CC_LDR_ATOMIC_MODE_SUPPORT
    if(_u1UpgradeMode != 0)
    {
        if (DRVCUST_InitGet(eLdrAtomicUpgFileNamePrefix) != 0)
        {
	        szPrefix = (CHAR *)DRVCUST_InitGet(eLdrAtomicUpgFileNamePrefix);
        }
        if (DRVCUST_InitGet(eLdrAtomicUpgFileNameSuffix) != 0)
        {
	        szSuffix = (CHAR *)DRVCUST_InitGet(eLdrAtomicUpgFileNameSuffix);
        }		
    }
#endif
    // list all files
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {
        UINT32 len = x_strlen(at_dir_entry[0].s_name);
        //Printf("[_LdrUsbFileRead]%s\n", at_dir_entry[0].s_name);
        if (x_strcmp(at_dir_entry[0].s_name, _szFileName) == 0)
        {
            fgExactTarget = TRUE;
            break;
        }

        if (len < (x_strlen(szPrefix) + x_strlen(szSuffix)))
        {
            continue;
        }

        len -= x_strlen(szSuffix);

        // if filename starts with "upgrade" and ends with ".pkg"
        if ((x_strncmp(at_dir_entry[0].s_name, szPrefix, x_strlen(szPrefix)) == 0) &&
            (x_strcmp(&at_dir_entry[0].s_name[len], szSuffix) == 0))
        {
            if (u4FuzzyTargetCount == 0)
            {
                x_strncpy(szFuzzyFileName, at_dir_entry[0].s_name, x_strlen(at_dir_entry[0].s_name));
            }

            u4FuzzyTargetCount++;
        }
    }

    if (!fgExactTarget)
    {
        if (u4FuzzyTargetCount == 0)
        {
            x_fm_close(h_dir);
            Printf("Error: No %sxxx%s was found\n", szPrefix, szSuffix);
            return 1;
        }
        else if (u4FuzzyTargetCount > 1)
        {
            x_fm_close(h_dir);
            Printf("Error: Multiple %sxxx%s were found\n", szPrefix, szSuffix);
            return 1;
        }
        else
        {
            x_strncpy(_szFileName, szFuzzyFileName, sizeof(_szFileName)-1);
            _szFileName[sizeof(_szFileName)-1] = '\0';
            Printf("upgrade file %s was found\n", _szFileName);
        }
    }
    // end

    x_strcpy(szFileNameWithPath, USB_ROOT_PATH);
    x_strcat(szFileNameWithPath, "/");
    x_strcat(szFileNameWithPath, _szFileName);

#ifdef CC_USB_UPGRADE_CHECK_FILE_VERSION
		if(_LdrUsbCheckFileVersion(_szFileName) == 0) 
		{
			Printf("Don't upgrade pkg after file version checking\n");
			return 1;
		}
#endif

    // Open upgrade image file
    if (x_fm_open(FM_ROOT_HANDLE, szFileNameWithPath, FM_READ_ONLY, 0777, FALSE, &rImgFile) != FMR_OK)
    {
        Printf("\nError: Open file %s fail, ", _szFileName);
        return 1;
    }

    // Get file info
    if(x_fm_get_info_by_handle(rImgFile, &rFileInfo) != FMR_OK)
    {
        Printf("\nError: Get file %s info fail, ", _szFileName);
        return 1;
    }
    _u8FileSize = (UINT64)rFileInfo.ui8_size;

    // Print file info
    Printf("Upgrade File Name: %s\n", _szFileName);
    Printf("Upgrade File Size: 0x%08x%08x Bytes\n", (UINT32)(_u8FileSize>>32), (UINT32)(_u8FileSize));

    if (_u8FileSize > MAX_UPGRADE_IMAGE_FILE_SIZE)
    {
    	static UINT64 _u8MaxUpgradeFileSize = (UINT64)MAX_UPGRADE_IMAGE_FILE_SIZE;
        Printf("\nError: File %s is over the maximum allowed size: 0x%08x%08x, ", _szFileName,
			(UINT32)(_u8MaxUpgradeFileSize>>32), (UINT32)(_u8MaxUpgradeFileSize));
        return 1;
    }

#ifdef USB_UPGRADE_DEBUG

	if( (_pucFileBuf = x_mem_alloc(_u8FileSize)) == NULL)
		{
			Printf("\nError: Buffer allocated fail, ");
			return 1;
		}

    // Read file into buffer
    UINT64 u8ReadFileSize = 0;
    UCHAR* pu1Buf = _pucFileBuf;
    UINT32 u4Read = 0;
    INT32 i4_ret;


    while(u8ReadFileSize != _u8FileSize)
    {
        if ((i4_ret = x_fm_read(rImgFile, pu1Buf, BYTES_PER_READ, &u4Read)) != FMR_OK)
        {
            Printf("\nError: Read file %s fail, ", _szFileName);
            Printf("\nx_fm_read return %d\n", i4_ret);
            Printf("Read File Size: 0x%08x%08x Bytes\n", (UINT32)(u8ReadFileSize>>32), (UINT32)(u8ReadFileSize));
            x_fm_close(rImgFile);
            x_fm_close(h_dir);
            x_mem_free(_pucFileBuf);
            pu1Buf = NULL;
            return 1;
        }

        u8ReadFileSize += u4Read;
        pu1Buf += u4Read;
    }

    Printf("Read File Size: 0x%08x%08x Bytes\n", (UINT32)(u8ReadFileSize>>32), (UINT32)(u8ReadFileSize));
#endif

    x_fm_close(rImgFile);
    x_fm_close(h_dir);
    return 0;
}

#if defined(CC_SDMMC_SUPPORT)
static INT32 _LdrSDFileRead()
{
    char szFileNameWithPath[96];
    HANDLE_T rImgFile;
    FM_FILE_INFO_T  rFileInfo;
    HANDLE_T    h_dir;

    FM_DIR_ENTRY_T at_dir_entry[1];
    UINT32  ui4_num_entries;
    BOOL fgExactTarget = FALSE;
    UINT32 u4FuzzyTargetCount = 0;
    CHAR szFuzzyFileName[64];
    CHAR* szPrefix = "";
    CHAR* szSuffix = "";
#ifdef UPGRADE_VERSION_CHECK
	  UINT32 i = 0;
#endif
    if (x_fm_open_dir(FM_ROOT_HANDLE, MSDC_ROOT_PATH, &h_dir) != FMR_OK)
    {
        Printf("Open MSDC root directory fail, ");
        return 1;
    }

    x_memset(szFileNameWithPath, 0, sizeof(szFileNameWithPath));
    x_memset(szFuzzyFileName, 0, sizeof(szFuzzyFileName));

#ifdef UPGRADE_VERSION_CHECK
	  for(;i<UPGRADEFILE_MAX_NUM;i++)
	  {
	    if((szMultipleUpgradeFile[i] = (CHAR*)x_mem_alloc(UPGRADEFILE_NAME_MAX)) == NULL)
	    {
	        Printf("\n----szMultipleUpgradeFile[%d]--x_mem_alloc---fail\n");
			    return 1;
	    }
		  x_memset(szMultipleUpgradeFile[i],0,UPGRADEFILE_NAME_MAX);
	  }
	  i = 0;
#endif
    if (DRVCUST_InitGet(eUsbUpgradeFileNamePrefix) != 0)
    {
        szPrefix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeFileNamePrefix);
    }

    if (DRVCUST_InitGet(eUsbUpgradeFileNameSuffix) != 0)
    {
        szSuffix = (CHAR *)DRVCUST_InitGet(eUsbUpgradeFileNameSuffix);
    }

#ifdef CC_LDR_ATOMIC_MODE_SUPPORT
	if(_u1UpgradeMode != 0)
	{
		if (DRVCUST_InitGet(eLdrAtomicUpgFileNamePrefix) != 0)
		{
			szPrefix = (CHAR *)DRVCUST_InitGet(eLdrAtomicUpgFileNamePrefix);
		}
		if (DRVCUST_InitGet(eLdrAtomicUpgFileNameSuffix) != 0)
		{
			szSuffix = (CHAR *)DRVCUST_InitGet(eLdrAtomicUpgFileNameSuffix);
		}		
	}
#endif

    // list all files
    while (x_fm_read_dir_entries(h_dir, at_dir_entry, 1, &ui4_num_entries) == FMR_OK)
    {
        UINT32 len = x_strlen(at_dir_entry[0].s_name);
        //Printf("[_LdrUsbFileRead]%s\n", at_dir_entry[0].s_name);
        if (x_strcmp(at_dir_entry[0].s_name, _szFileName) == 0)
        {
            fgExactTarget = TRUE;
            break;
        }

        if (len < (x_strlen(szPrefix) + x_strlen(szSuffix)))
        {
            continue;
        }

        len -= x_strlen(szSuffix);

        // if filename starts with "upgrade" and ends with ".pkg"
        if ((x_strncmp(at_dir_entry[0].s_name, szPrefix, x_strlen(szPrefix)) == 0) &&
            (x_strcmp(&at_dir_entry[0].s_name[len], szSuffix) == 0))
        {
            if (u4FuzzyTargetCount == 0)
            {
                x_strncpy(szFuzzyFileName, at_dir_entry[0].s_name, x_strlen(at_dir_entry[0].s_name));
            }

            u4FuzzyTargetCount++;
#ifdef UPGRADE_VERSION_CHECK
			      x_strncpy(szMultipleUpgradeFile[i], at_dir_entry[0].s_name,x_strlen(at_dir_entry[0].s_name));
            i++;
#endif
        }
    }

    if (!fgExactTarget)
    {
        if (u4FuzzyTargetCount == 0)
        {
            x_fm_close(h_dir);
            Printf("Error: No %sxxx%s was found\n", szPrefix, szSuffix);
#ifdef LOADER_FORCE_UPGRADE
            return 2;
#else
            return 1;
#endif

        }
        else if (u4FuzzyTargetCount > 1)
        {
        #ifdef UPGRADE_VERSION_CHECK
		        _LdrUpgradeVersionCompare(u4FuzzyTargetCount);
			      x_strncpy(_szFileName, szMultipleUpgradeFile[0], x_strlen(szMultipleUpgradeFile[0]));
            Printf("upgrade file %s was found\n", _szFileName);
		    #else
            x_fm_close(h_dir);
            Printf("Error: Multiple %sxxx%s were found\n", szPrefix, szSuffix);
            return 1;
		#endif
        }
        else
        {
            x_strncpy(_szFileName, szFuzzyFileName, x_strlen(szFuzzyFileName));
            Printf("upgrade file %s was found\n", _szFileName);
        }
    }
    // end

    x_strcpy(szFileNameWithPath, MSDC_ROOT_PATH);
    x_strcat(szFileNameWithPath, "/");
    x_strcat(szFileNameWithPath, _szFileName);

#ifdef CC_USB_UPGRADE_CHECK_FILE_VERSION
    if(_LdrUsbCheckFileVersion(_szFileName) == 0) 
    {
        Printf("Don't upgrade pkg after file version checking\n");
        return 1;
    }
#endif

    // Open upgrade image file
    if (x_fm_open(FM_ROOT_HANDLE, szFileNameWithPath, FM_READ_ONLY, 0777, FALSE, &rImgFile) != FMR_OK)
    {
        Printf("\nError: Open file %s fail, ", _szFileName);
        return 1;
    }

    // Get file info
    if(x_fm_get_info_by_handle(rImgFile, &rFileInfo) != FMR_OK)
    {
        Printf("\nError: Get file %s info fail, ", _szFileName);
        return 1;
    }
    _u8FileSize = (UINT64)rFileInfo.ui8_size;

    // Print file info
    Printf("Upgrade File Name: %s\n", _szFileName);
    Printf("Upgrade File Size: 0x%08x%08x Bytes\n", (UINT32)(_u8FileSize>>32), (UINT32)(_u8FileSize));

    if (_u8FileSize > MAX_UPGRADE_IMAGE_FILE_SIZE)
    {
        Printf("\nError: File %s is over the maximum allowed size: 0x%08x%08x, ", _szFileName,
			(UINT32)(MAX_UPGRADE_IMAGE_FILE_SIZE>>32), (UINT32)(MAX_UPGRADE_IMAGE_FILE_SIZE));
        return 1;
    }

#ifdef USB_UPGRADE_DEBUG

	if( (_pucFileBuf = x_mem_alloc(_u8FileSize)) == NULL)
		{
			Printf("\nError: Buffer allocated fail, ");
			return 1;
		}

    // Read file into buffer
    UINT64 u8ReadFileSize = 0;
    UCHAR* pu1Buf = _pucFileBuf;
    UINT32 u4Read = 0;
    INT32 i4_ret;


    while(u8ReadFileSize != _u8FileSize)
    {
        if ((i4_ret = x_fm_read(rImgFile, pu1Buf, 128*BYTES_PER_READ, &u4Read)) != FMR_OK)
        {
            Printf("\nError: Read file %s fail, ", _szFileName);
            Printf("\nx_fm_read return %d\n", i4_ret);
            Printf("Read File Size: 0x%08x%08x Bytes\n", (UINT32)(u8ReadFileSize>>32), (UINT32)(u8ReadFileSize));
            x_fm_close(rImgFile);
            x_fm_close(h_dir);
            x_mem_free(_pucFileBuf);
            pu1Buf = NULL;
            return 1;
        }

        u8ReadFileSize += u4Read;
        pu1Buf += u4Read;
    }

    Printf("Read File Size: 0x%08x%08x Bytes\n", (UINT32)(u8ReadFileSize>>32), (UINT32)(u8ReadFileSize));
#endif

    x_fm_close(rImgFile);
    x_fm_close(h_dir);
#ifdef UPGRADE_VERSION_CHECK
	  i = 0;
	  for(;i<UPGRADEFILE_MAX_NUM;i++)
	  {
	    if(szMultipleUpgradeFile[i] != NULL)
	    {
	        x_mem_free(szMultipleUpgradeFile[i]);
			szMultipleUpgradeFile[i] = NULL;
	    }
	  }
#endif
    return 0;
}
#endif

static INT32 _LdrUsbMount(void)
{
    INT32 i4_ret;
    UINT32 ui4_part_ns, ui4_cnt;
    VOID*          pv_mem_addr;
    SIZE_T         z_mem_size;

    /*  1. dev point creation */
    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/dev", 0666);

    if (i4_ret == FMR_OK)
    {
        DEBUG("/dev is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            DEBUG("/dev is already existed.  That is OK.\n");
        }
        else
        {
            DEBUG("make /dev fail\n");
            DEBUG("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  2. mount point creation */
    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/mnt", 0666);

    if (i4_ret == FMR_OK)
    {
        DEBUG("/mnt is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            DEBUG("/mnt is already existed.  That is OK.\n");
        }
        else
        {
            DEBUG("make /mnt fail\n");
            DEBUG("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  3. make entry for USB */
    i4_ret = x_fm_make_entry(FM_ROOT_HANDLE,
                             "/dev/usb",
                             FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0660,
                             FM_DRVT_USB_MASS_STORAGE,
                             0);

    if (i4_ret == FMR_OK)
    {
        DEBUG("/dev/usb is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            DEBUG("/dev/usb is already existed.  That is OK.\n");
        }
        else
        {
            DEBUG("make /dev/usb fail\n");
            DEBUG("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  4. init handle library  */
#define MAX_NUM_HANDLES     1024
#define HANDLE_MEM_SIZE     64*1024

    pv_mem_addr = x_mem_calloc(1, HANDLE_MEM_SIZE);
    z_mem_size = HANDLE_MEM_SIZE;

    i4_ret = handle_init(MAX_NUM_HANDLES,
                              &pv_mem_addr,
                              &z_mem_size);

    if (i4_ret != FMR_OK)
    {
        DEBUG("init handle library error.");
        DEBUG("return value:%d\n", i4_ret);
        goto func_exit;
    }


    /*  5. parse drive  */
    i4_ret = x_fm_parse_drive(FM_ROOT_HANDLE,
                              "/dev/usb",
                              (VOID *) NULL);

    if (i4_ret != FMR_OK)
    {
        DEBUG("parse drive error.");
        DEBUG("return value:%d\n", i4_ret);
        goto func_exit;
    }

    /*  6. get partition ns */
    i4_ret = x_fm_get_part_ns(FM_ROOT_HANDLE,
                              (const char *) "/dev/usb",
                              &ui4_part_ns);

    if (i4_ret != FMR_OK)
    {
        DEBUG("get partition ns fail.\n");
        DEBUG("return value:%d\n", i4_ret);
        goto func_exit;
    }

    for (ui4_cnt = 0; ui4_cnt < ui4_part_ns; ui4_cnt++)
    {
        FM_PART_INFO_T t_part_info;

        char ps_dev_name[40];
        char ps_mnt_name[40];

        i4_ret = x_fm_get_part_info(FM_ROOT_HANDLE,
                                    (const char *) "/dev/usb",
                                    ui4_cnt,
                                    &t_part_info);

        if (i4_ret != FMR_OK)
        {
            DEBUG("x_fm_get_part_info fails.\n");
            DEBUG("return value:%d\n", i4_ret);
            goto func_exit;
        }

        x_strcpy((char *) ps_dev_name, "/dev/");
        x_strcpy((char *) ps_mnt_name, "/mnt/");

        x_strcat((char *) ps_dev_name, (const char *) t_part_info.ps_part_name);
        x_strcat((char *) ps_mnt_name, (const char *) t_part_info.ps_part_name);

        i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, ps_mnt_name, 0666);

        if (i4_ret == FMR_OK)
        {
            DEBUG("%s is created.\n", ps_mnt_name);
        }
        else
        {
            if (i4_ret == FMR_EXIST)
            {
                DEBUG("%s is already existed.  That is OK.\n", ps_mnt_name);
            }
            else
            {
                DEBUG("make %s fail.\n", ps_mnt_name);
                DEBUG("return value:%d\n", i4_ret);
                goto func_exit;
            }
        }

        i4_ret = x_fm_mount(FM_ROOT_HANDLE, ps_dev_name, FM_ROOT_HANDLE, ps_mnt_name);

        if (i4_ret == FMR_OK)
        {
            DEBUG("%s is mounted as %s.\n", ps_dev_name, ps_mnt_name);
        }
        else
        {
            DEBUG("ERR: mount %s as %s fail.\n", ps_dev_name, ps_mnt_name);
            DEBUG("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }
    return 0;

func_exit:

    return 1;
}

#if defined(CC_SDMMC_SUPPORT)
static INT32 _LdrSDMount(void)
{
    INT32 i4_ret;
    UINT32 ui4_part_ns, ui4_cnt;
    VOID*          pv_mem_addr;
    SIZE_T         z_mem_size;

    /*  1. dev point creation */
    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/dev", 0666);

    if (i4_ret == FMR_OK)
    {
        DEBUG("/dev is created.\n");Printf("/dev is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            DEBUG("/dev is already existed.  That is OK.\n");Printf("/dev is already existed.  That is OK.\n");
        }
        else
        {
            DEBUG("make /dev fail\n");Printf("make /dev fail\n");
            DEBUG("return value:%d\n", i4_ret);Printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  2. mount point creation */
    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/mnt", 0666);

    if (i4_ret == FMR_OK)
    {
        DEBUG("/mnt is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            DEBUG("/mnt is already existed.  That is OK.\n");Printf("/mnt is already existed.  That is OK.\n");
        }
        else
        {
            DEBUG("make /mnt fail\n");Printf("make /mnt fail\n");
            DEBUG("return value:%d\n", i4_ret);Printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  3. make entry for USB */
    i4_ret = x_fm_make_entry(FM_ROOT_HANDLE,
                             "/dev/msdc",
                             FM_MODE_TYPE_BLK | FM_MODE_DEV_TRUE | 0660,
                             FM_DRVT_MEM_CARD,
                             0);

    if (i4_ret == FMR_OK)
    {
        DEBUG("/dev/msdc is created.\n");Printf("/dev/msdc is created.\n");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            DEBUG("/dev/msdc is already existed.  That is OK.\n");Printf("/dev/msdc is already existed.  That is OK.\n");
        }
        else
        {
            DEBUG("make /dev/msdc fail\n");Printf("make /dev/msdc fail\n");
            DEBUG("return value:%d\n", i4_ret);Printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    /*  4. init handle library  */
#define MAX_NUM_HANDLES     1024
#define HANDLE_MEM_SIZE     64*1024

    pv_mem_addr = x_mem_calloc(1, HANDLE_MEM_SIZE);
    z_mem_size = HANDLE_MEM_SIZE;

    i4_ret = handle_init(MAX_NUM_HANDLES,
                              &pv_mem_addr,
                              &z_mem_size);

    if (i4_ret != FMR_OK)
    {
        DEBUG("init handle library error.");Printf("init handle library error.");
        DEBUG("return value:%d\n", i4_ret);Printf("return value:%d\n", i4_ret);
        goto func_exit;
    }


    /*  5. parse drive  */
    i4_ret = x_fm_parse_drive(FM_ROOT_HANDLE,
                              "/dev/msdc",
                              (VOID *) NULL);

    if (i4_ret != FMR_OK)
    {
        DEBUG("parse drive error.");Printf("parse drive error.");
        DEBUG("return value:%d\n", i4_ret);Printf("return value:%d\n", i4_ret);
        goto func_exit;
    }

    /*  6. get partition ns */
    i4_ret = x_fm_get_part_ns(FM_ROOT_HANDLE,
                              (const char *) "/dev/msdc",
                              &ui4_part_ns);

    if (i4_ret != FMR_OK)
    {
        DEBUG("get partition ns fail.\n");Printf("get partition ns fail.\n");
        DEBUG("return value:%d\n", i4_ret);Printf("return value:%d\n", i4_ret);
        goto func_exit;
    }

    for (ui4_cnt = 0; ui4_cnt < ui4_part_ns; ui4_cnt++)
    {
        FM_PART_INFO_T t_part_info;

        char ps_dev_name[40];
        char ps_mnt_name[40];

        i4_ret = x_fm_get_part_info(FM_ROOT_HANDLE,
                                    (const char *) "/dev/msdc",
                                    ui4_cnt,
                                    &t_part_info);

        if (i4_ret != FMR_OK)
        {
            DEBUG("x_fm_get_part_info fails.\n");Printf("x_fm_get_part_info fails.\n");
            DEBUG("return value:%d\n", i4_ret);Printf("return value:%d\n", i4_ret);
            goto func_exit;
        }

        x_strcpy((char *) ps_dev_name, "/dev/");
        x_strcpy((char *) ps_mnt_name, "/mnt/");

        x_strcat((char *) ps_dev_name, (const char *) t_part_info.ps_part_name);
        x_strcat((char *) ps_mnt_name, (const char *) t_part_info.ps_part_name);

        i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, ps_mnt_name, 0666);

        if (i4_ret == FMR_OK)
        {
            DEBUG("%s is created.\n", ps_mnt_name);Printf("%s is created.\n", ps_mnt_name);
        }
        else
        {
            if (i4_ret == FMR_EXIST)
            {
                DEBUG("%s is already existed.  That is OK.\n", ps_mnt_name);Printf("%s is already existed.  That is OK.\n", ps_mnt_name);
            }
            else
            {
                DEBUG("make %s fail.\n", ps_mnt_name);Printf("make %s fail.\n", ps_mnt_name);
                DEBUG("return value:%d\n", i4_ret);Printf("return value:%d\n", i4_ret);
                goto func_exit;
            }
        }

        i4_ret = x_fm_mount(FM_ROOT_HANDLE, ps_dev_name, FM_ROOT_HANDLE, ps_mnt_name);

        if (i4_ret == FMR_OK)
        {
            DEBUG("%s is mounted as %s.\n", ps_dev_name, ps_mnt_name);Printf("%s is mounted as %s.\n", ps_dev_name, ps_mnt_name);
        }
        else
        {
            DEBUG("ERR: mount %s as %s fail.\n", ps_dev_name, ps_mnt_name);Printf("ERR: mount %s as %s fail.\n", ps_dev_name, ps_mnt_name);
            DEBUG("return value:%d\n", i4_ret);Printf("return value:%d\n", i4_ret);
            goto func_exit;
        }
    }

    return 0;

func_exit:

    return 1;
}
#endif

//#define UPGRADE_HUB_WAIT_TIME        2500  //if hub insert ,but no devices on hub,  use this time to check any device on hub
#define UPGRADE_HUB_WAIT_TIME        5000  //Caihua:[DTV00596107] usb upgrade fail on model like PAA
#define UPGRADE_HUB_MASS_WAIT_TIME   3500  //if hub insert and detect any devices on hub,  use this time to check mass  storage exist or not.
#define UPGRADE_NORMAL_WAIT_TIME     5000  // use this time to check mass storage can be use or not

static INT32 _LdrUsbUpgrade(LDR_DATA_T *prLdrData, char *szFileName, UINT32 u4Upgrade)
{
    static UINT8 u1USBBlock[4096];
    HAL_TIME_T t0, t1, dt;
    INT32 i4Result;
#ifdef SSUSB_LOADER
    INT32 i4Result_U3;
#endif    
    UINT32 u4Diff;
#ifdef CC_SUPPORT_USB_DIRECT_UART 	
	PFN_GPIO_LEDCTRL_FUNC pfnLedCtrlFunc = NULL;
#endif	
    #ifdef CC_QUIET_HOT_BOOT_SUPPORT
	DTVCFG_T rDtvCfg;
    #endif
    #if defined(CC_SDMMC_SUPPORT_USB_RECOVERY)
	UINT32 u4MMCRecoveryEepromOffset, u4MMCRecoveryThreshold=3;
	UINT32 u4Val = 0;
	#endif
    #ifdef CC_LDR_ATOMIC_MODE_SUPPORT
	UINT32 u4Mode = 0;
    #if defined(CC_SECURESTORAGE_SUPPORT)
	UINT8 auSecureDataZone[256];
    #endif	
    #endif

#if defined(CC_ENABLE_REBOOT_REASON) && defined(ANDROID) && defined(CC_EMMC_BOOT)
#if 0 /* for flags saved in 8032 xdata */
    #define PDWNC_REBOOT_FLAG           0x5a
    #define PDWNC_REBOOT_TAG            "recovery"
    extern INT32 PDWNC_Write_T8032_XData(UINT32 u4XAddr, UINT32 u4Size, const void *u4RiscAddr);
    extern INT32 PDWNC_Read_T8032_XData(UINT32 u4XAddr, UINT32 u4Size, const void *u4RiscAddr);
    UINT32 u4RebootOffset, u4RebootSize, u4RebootFlagOffset;

    if (DRVCUST_InitQuery(eT8032RebootOffset, &u4RebootOffset) || DRVCUST_InitQuery(eT8032RebootSize, &u4RebootSize))
    {
        Printf("Cannot get linux reboot reason information\n");
    }
    else
    {
        flag[0] = 0;
        u4RebootFlagOffset = (u4RebootOffset+u4RebootSize) - 1;
        if (PDWNC_Read_T8032_XData(u4RebootFlagOffset, 1, flag))
        {
            if (flag[0] == PDWNC_REBOOT_FLAG)
            {
                flag[0] = 0;
                PDWNC_Write_T8032_XData(u4RebootFlagOffset, 1, flag);
                Printf("Android upgrade flag was set. Skip USB upgrade\n");
                return 0;
            }
        }
    }
#else /* for flags saved in eeprom */
    #define EEPROM_REBOOT_FLAG           0x5a
    #define EEPROM_REBOOT_TAG            "recovery"
    UINT32 u4RebootEepromOffset, u4RebootEepromSize, u4RebootFlagEepromOffset;
    char   flag[1];
    UINT32 EepromFlag[1];

    if (DRVCUST_InitQuery(eRebootEepromAddress, &u4RebootEepromOffset) || DRVCUST_InitQuery(eRebootEepromSize, &u4RebootEepromSize))
    {
        Printf("Cannot get linux reboot reason information from eeprom \n");
    }
    else
    {
        EepromFlag[0] = 0;
        u4RebootFlagEepromOffset = (u4RebootEepromOffset+u4RebootEepromSize) - 1;

        if (EEPROM_Read(u4RebootFlagEepromOffset,  (UINT32)((void *)EepromFlag), 1) == 0)
        {
            if ((EepromFlag[0] & 0xff ) == EEPROM_REBOOT_FLAG)
            {
                EepromFlag[0] = 0;
                UNUSED(EEPROM_Write(u4RebootFlagEepromOffset, (UINT32)((void *)EepromFlag), sizeof(UINT32)));
                Printf("Android upgrade flag was set by eeprom. Skip USB upgrade\n");
                #ifdef CC_QUIET_HOT_BOOT_SUPPORT
	            UNUSED(EEPDTV_GetCfg(&rDtvCfg));
	            rDtvCfg.u1Flags4 &= (UINT8)~DTVCFG_FLAG4_QUIET_HOT_BOOT;
	            prLdrData->rDtvCfg.u1Flags4 &= (UINT8)~DTVCFG_FLAG4_QUIET_HOT_BOOT;
	            UNUSED(EEPDTV_SetCfg(&rDtvCfg));
				LDR_PanelLogo(prLdrData);
                #endif				
                return 0;
            }
        }
    }
#endif
#endif

    if (_u4Upgraded)
    {
        Printf("\nUSB upgraded\n");
        return 0;
    }
#ifdef CC_SUPPORT_USB_DIRECT_UART 	   
	UNUSED(DRVCUST_InitQuery(eGpioLedCtrlFunc, (UINT32 *)(void *)&pfnLedCtrlFunc));
    if (pfnLedCtrlFunc != NULL)
    {
        pfnLedCtrlFunc(eLedUpgrageStart);
    }
#endif	
    x_strcpy(_szFileName, szFileName);

    // Check if usb device is inserted.
    i4Result = MUSB_GetInsert();
    if (i4Result != FIND_CLASS_MASS_STORAGE)
    {
        if(i4Result != FIND_CLASS_HUB)
            _fgDeviceType |= 0x01;

#ifdef SSUSB_LOADER
#define SSUSB_TIMEOUT 0
int ssusb_time_cnt=0;
//	MUSB3_Init(1);	
//    x_thread_delay(1000);
while(1)
{
 	    if (BIM_IsIrqPending(79))
	    {
            Printf("\nBIM_IsIrqPending time_cnt=%d\n",ssusb_time_cnt);
		    _xHCIIrqHandler(VECTOR_USB3_1);
		    BIM_ClearIrq(79);		 
            usb_init(0);
        break;
	    }
        x_thread_delay(1);
        ssusb_time_cnt++;
        if(ssusb_time_cnt >= SSUSB_TIMEOUT)
        {
            Printf("\nSSUSB_TIMEOUT time_cnt=%d\n",ssusb_time_cnt);
            break;
        }
}
#endif
#ifdef SSUSB_LOADER
        i4Result_U3 = MUSB3_GetInsert();
        if (i4Result_U3 != FIND_CLASS_MASS_STORAGE)
        {
            if(i4Result != FIND_CLASS_HUB)
                _fgDeviceType |= 0x01;
            Printf("\n No U3 USB device.\n");
        }
        else
        {
            _fgDeviceType &= ~0x01;
            i4Result = i4Result_U3;
        }
#endif
        {
            Printf("\n No USB device.\n");
        }
    }

#if defined(CC_SDMMC_SUPPORT_USB_RECOVERY)
	if((prLdrData->rDtvCfg.u1Upgrade_Ext) == 0x59)
	{
      if(DRVCUST_InitQuery(eMMCRecoveryEepromAddress, &u4MMCRecoveryEepromOffset))
      {
        Printf("Cannot get MMC USB recovery information from eeprom \n");
      }
      else
      {
		  UNUSED(EEPROM_Read((UINT64)u4MMCRecoveryEepromOffset, (UINT32)&u4Val, 1));
		  if(u4Val < u4MMCRecoveryThreshold)
		  {
		    Printf("\n Force to SD MMC upgrade(%d/%d).\n",u4Val,u4MMCRecoveryThreshold);
		    _fgDeviceType |= 0x01;
			i4Result = FIND_MUSB_NONE;
		  }
      }	    
	}
#endif

#if defined(CC_SDMMC_SUPPORT)
    // Check if SD device is inserted by GPIO detection
    if(MSDCDetectSDGPIO())
    {
        Printf("\nNo or Unknown SD/MMC device.\n");
        _fgDeviceType |= 0x02;
    }
#endif

#if defined(CC_SDMMC_SUPPORT)
    if((_fgDeviceType & 0x03) == 0x03)
#else
    if((_fgDeviceType & 0x01) == 0x01)
#endif
    {
        return 1;
    }

    if (i4Result == FIND_CLASS_MASS_STORAGE)
    {
        Printf("\n FIND_CLASS_MASS_STORAGE.\n");
    }

    if (i4Result == FIND_CLASS_HUB)
    {
        Printf("\n FIND_CLASS_HUB.\n");
        HAL_GetTime(&t0);
        UINT32 u4DeviceOnHubFlag = FALSE;
        UINT32 MassOnHubFlag = FALSE;
        while (MUSB_GetStatus() != HFI_STATUS_READY)
        {
            MUSB_NoneRunBackground();

            HAL_GetTime(&t1);
            HAL_GetDeltaTime(&dt, &t0, &t1);
            u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;

            if(FALSE==u4DeviceOnHubFlag) {
                if((u4Diff<UPGRADE_HUB_WAIT_TIME)) {
                    u4DeviceOnHubFlag = MUSB_GetDeviceOnHubFlag();
                } else {
                        _fgDeviceType |= 0x01;                  // no device on HUB
                        Printf("\n HUB No USB Medium on Hub.\n");
                        break;
                }
            } else { /* HUB inserted */
                if(FALSE==MassOnHubFlag) {
                    if((u4Diff<UPGRADE_HUB_MASS_WAIT_TIME)) {
                        MassOnHubFlag = MUSB_GetMassOnHubFlag();
                    } else {
                        Printf("\n HUB No Mass Storage on Hub.\n");
                        break;
                    }
                } else {  /* Mass storage detected */
                    break;
                }
            }
        }
    }

#if defined(CC_SDMMC_SUPPORT)
    if((_fgDeviceType & 0x03) == 0x03)
#else
    if((_fgDeviceType & 0x01) == 0x01)
#endif
    {
        Printf("\n No USB or SD/MMC on HUB & USB.\n");
        return 1;
    }

    HAL_GetTime(&t0);
    if((_fgDeviceType & 0x01) == 0x00)
    {
        Printf("Wait USB medium to be mounted...\n");
    }
#if defined(CC_SDMMC_SUPPORT)
    else if((_fgDeviceType & 0x02) == 0x00)
    {
        if((prLdrData->rDtvCfg.u1Upgrade_Ext)==0x59)
        {
          Printf("Wait eMMC medium to be mounted...\n");
          MSDC_EmmcHostSet(1);
        }
		else
		{
          Printf("Wait SD medium to be mounted...\n");  
          MSDC_EmmcHostSet(0);
		}

    }
#endif

    // wait for usb device to be processed by USB driver.
    if(((_fgDeviceType & 0x01) == 0x00)&&((i4Result == FIND_CLASS_MASS_STORAGE)||(MUSB_GetMassOnHubFlag())))
    {
		    while (MUSB_GetStatus() != HFI_STATUS_READY)
		    {
		        MUSB_NoneRunBackground();

		        HAL_GetTime(&t1);
		        HAL_GetDeltaTime(&dt, &t0, &t1);
		        u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;

		        // time out = 5000 ms.
		        if ((u4Diff >= UPGRADE_NORMAL_WAIT_TIME) && (MUSB_GetStatus() != HFI_STATUS_READY))
		        {
		            Printf("\nNo USB Medium on Device.\n");
		            break;
		        }
#ifdef SSUSB_LOADER
		        if (MUSB3_GetInsert() == FIND_CLASS_MASS_STORAGE)
		        {
		            /* USB3.0 port Mass storage detected */
                    break;
		        }
#endif
		    }

		    HAL_GetTime(&t1);
		    HAL_GetDeltaTime(&dt, &t0, &t1);
		    u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;
		    Printf("USB Medium on Device, Time = %d ms.\n", u4Diff);
		    Printf("USB block size = %d bytes.\n", MUSB_GetBlockSize());

		    // try to read 1st block of data from USB.
#ifdef SSUSB_LOADER
            if(MUSB3_GetInsert() == FIND_CLASS_MASS_STORAGE)
            {
                if (MUSB3_Read(0, (UINT32)u1USBBlock, MUSB3_GetBlockSize()) != 0)
                {
                    Printf("\nError: USB Read fail, ");
                    return 1;
                }
            }
            else
#endif
            {
		    if (MUSB_Read(0, (UINT32)u1USBBlock, MUSB_GetBlockSize()) != 0)
		    {
		        Printf("\nError: USB Read fail, ");
		        return 1;
		    }
            }
    }
#if defined(CC_SDMMC_SUPPORT)
    else if((_fgDeviceType & 0x02) == 0x00)
    {
        //TO-DO........
        while (MSDCDetectSDGPIO())
        {
            HAL_GetTime(&t1);
            HAL_GetDeltaTime(&dt, &t0, &t1);
            u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;

            // time out = 5000 ms.
            if (u4Diff >= 5000)
            {
                Printf("\nNo SD Medium on Device.\n");
                return 1;
            }
        }

        if(MSDCGetSDInsert(TRUE) != MSDC_SD_INSERT)
        {
            Printf("\nUnknown SD Medium on Device.\n");
            return 1;
        }

        HAL_GetTime(&t1);
        HAL_GetDeltaTime(&dt, &t0, &t1);
        u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;
        Printf("SD/MMC Medium on Device, Time = %d ms.\n", u4Diff);
        Printf("SD/MMC block size = %d bytes.\n", 0x200);

        // try to read 1st block of data from USB.
        if (SD2DRAM(0, (UINT32 *)u1USBBlock, 0x200, 0) != 0)
        {
            Printf("\nError: SD/MMC Read fail, ");
            return 1;
        }
    }
#endif

    if (fm_init() != FMR_OK)
    {
        Printf("\nError: Init FM fail, ");
        return 1;
    }

    if (x_fm_fat_init(NULL) != FMR_OK)
    {
        Printf("\nError: Init FAT fail, ");
        return 1;
    }

    if((_fgDeviceType & 0x01) == 0x00)
    {
        if (_LdrUsbMount())
        {
            Printf("\nError: USB mount fail, ");
            return 1;
        }

        if(_LdrUsbFileRead() == 1)
        {
            Printf("\nError: No valid upgrade file found, ");
            return 1;
        }
    }
#if defined(CC_SDMMC_SUPPORT)
    else if((_fgDeviceType & 0x02) == 0x00)
    {
        if (_LdrSDMount())
        {
            Printf("\nError: SD mount fail, ");
            #if defined(CC_SDMMC_SUPPORT_USB_RECOVERY)
			UNUSED(EEPROM_Read((UINT64)u4MMCRecoveryEepromOffset, (UINT32)&u4Val, 1));
			u4Val++;
            UNUSED(EEPROM_Write((UINT64)u4MMCRecoveryEepromOffset, (UINT32)&u4Val, 1));
            #endif			
            return 1;
        }

        if(_LdrSDFileRead() == 1)
        {
            Printf("\nError: No valid upgrade file found, ");
            #if defined(CC_SDMMC_SUPPORT_USB_RECOVERY)
			UNUSED(EEPROM_Read((UINT64)u4MMCRecoveryEepromOffset, (UINT32)&u4Val, 1));
			u4Val++;
            UNUSED(EEPROM_Write((UINT64)u4MMCRecoveryEepromOffset, (UINT32)&u4Val, 1));
            #endif				
            return 1;
        }
    }
#endif

#ifdef CC_FAST_INIT
    prLdrData->rDtvCfg.u1Flags3 |= (UINT8)DTVCFG_FLAG3_NORMAL_BOOT_WITH_SUSPEND;
    prLdrData->rDtvCfg.u1Flags3 &= (UINT8)~DTVCFG_FLAG3_FAST_BOOT;
    prLdrData->rDtvCfg.u1Flags3 &= (UINT8)~DTVCFG_FLAG3_NORMAL_BOOT_WITHOUT_SUSPEND;

    if (EEPDTV_SetCfg(&prLdrData->rDtvCfg))
    {
        Printf("\nError: eeprom write fail, ");
        return 1;
    }
#endif

    vIO32Write4B(PDWNC_WDTCTL, 0);// PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0);

//show logo when upgrade
#ifdef CC_SUPPORT_BL_DLYTIME_CUT
	vDrvTurnOnBacklt();
#endif

#ifdef CC_QUIET_HOT_BOOT_SUPPORT
	UNUSED(EEPDTV_GetCfg(&rDtvCfg));
	rDtvCfg.u1Flags4 &= (UINT8)~DTVCFG_FLAG4_QUIET_HOT_BOOT;
	prLdrData->rDtvCfg.u1Flags4 &= (UINT8)~DTVCFG_FLAG4_QUIET_HOT_BOOT;
	UNUSED(EEPDTV_SetCfg(&rDtvCfg));

	LDR_PanelLogo(prLdrData);
	#ifdef CC_UPGRADING_DYNAMIC_LOGO_SUPPORT
	x_thread_delay(1000);
	LDR_DynamicLogoInit(prLdrData);
	LDR_DynamicLogoDisplay(prLdrData, 0);
	#endif
#else
    #ifndef CC_UPGRADING_DYNAMIC_LOGO_SUPPORT
    // To display logo index 1 for start upgrading.
    LDR_LogoDisplay(prLdrData, 1);
	#else
	if ((LDR_BECheckDDRI()) != TRUE)
	{
	    LDR_DynamicLogoInit(prLdrData);    
        LDR_DynamicLogoDisplay(prLdrData, 0);
	}
	#endif
#endif

    if (_LdrDlmHandler(u4Upgrade))
    {
        // To display logo index 3 for upgrade failed.
        LDR_LogoDisplay(prLdrData, 3);
        return 1;
    }

#ifdef USB_UPGRADE_DEBUG
    if (_pucFileBuf)
    {
        x_mem_free(_pucFileBuf);
        _pucFileBuf = NULL;
    }
#endif

#if defined(CC_ENABLE_REBOOT_REASON) && defined(ANDROID) && defined(CC_EMMC_BOOT)
    /* set android upgrade flag and recovery command.
       Next usb upgrade will be skip if android upgrade flag is set. */

#if 0 /* for flags saved in 8032 xdata */
    flag[0] = PDWNC_REBOOT_FLAG;
    PDWNC_Write_T8032_XData(u4RebootOffset,     sizeof(PDWNC_REBOOT_TAG), PDWNC_REBOOT_TAG);
    PDWNC_Write_T8032_XData(u4RebootFlagOffset, 1,                        flag);
#else /* for flags saved in eeprom */
    EepromFlag[0] = EEPROM_REBOOT_FLAG;
    UNUSED(EEPROM_Write(u4RebootEepromOffset,     EEPROM_REBOOT_TAG,            sizeof(EEPROM_REBOOT_TAG)));
    UNUSED(EEPROM_Write(u4RebootFlagEepromOffset, (UINT32)((void *)EepromFlag), 1));
#endif

#endif
#ifdef CC_MTK_AUTO_TEST
if(EepromSuffixFlag != 1)
{
	EepromSuffixFlag = 1;
    UNUSED(EEPROM_Write((DRVCUST_InitGet(eUsbEepromAutoTestOffset)), (UINT32)((void *)&EepromSuffixFlag), 1));
}
#endif

#if defined(CC_SDMMC_SUPPORT)
    if((prLdrData->rDtvCfg.u1Upgrade_Ext)==0x59)
	{
	  prLdrData->rDtvCfg.u1Upgrade_Ext = 0x79;
#if defined(CC_SDMMC_SUPPORT_USB_RECOVERY)
	  u4Val = 0;
      UNUSED(EEPROM_Write((UINT64)u4MMCRecoveryEepromOffset, (UINT32)&u4Val, 1));
#endif
      if (EEPDTV_SetCfg(&prLdrData->rDtvCfg))
      {
        Printf("\nError: eeprom write fail, ");
        return 1;
      }	
	  MSDC_EmmcHostSet(0);
	}
#endif

#ifdef ENABLE_USB2UART_FUNC//wangle
	/*check if keep to be UART*/
	//UINT32 u4Size;
	DTVCFG_T rDtvCfg;
	INT32 i4_ret;

	//u4Size = EEPDTV_GetSize();
	//ASSERT(u4Size >= sizeof(DTVCFG_T));
	//UNUSED(u4Size);

	i4_ret = EEPDTV_GetCfg(&rDtvCfg);
	ASSERT(i4_ret==0);
	UNUSED(i4_ret);

	BSP_PinSet(PIN_USB_DM_P1, PINMUX_FUNCTION5);
	BSP_PinSet(PIN_USB_DP_P1, PINMUX_FUNCTION5);

	rDtvCfg.u1Flags4 |= DTVCFG_FLAG4_USB_UART_STATUS;

	EEPDTV_SetCfg(&rDtvCfg);
	Printf("@@@@@[Loader]@@@@ set USB pinmux to UART function\n");
#endif
#ifdef CC_LDR_ATOMIC_MODE_SUPPORT
	u4Mode = 0x50;
    if(_u1UpgradeMode != 0)
    {	
        prLdrData->u4LdrAtomicMode = 0;
        UNUSED(EEPROM_Write(0xA1F0, (UINT32)&u4Mode, 1));        
	    //start : this should be replace with secrue mechnism for read
        #if defined(CC_SECURESTORAGE_SUPPORT)
		_LdrReadFromSecureStorage(0,auSecureDataZone,256);
		auSecureDataZone[0] = 0; //enable console lock
		auSecureDataZone[1] = u4Mode; //set to P-mode
		auSecureDataZone[2] = 0; //disable log
		_LdrWriteToSecureStorage(auSecureDataZone);
        #endif
	    //end 
	    _u1UpgradeMode = 0;
	    Printf("use atomic pkg to upgrade the system done and set to P-mode\n");
    }
	else
	{
	    Printf("use normal pkg to upgrade the system done\n");
	}	
#endif

#ifdef CC_MAPLE_BE_LOADER_BOOT_FLOW
	if (LDR_BECheckDDRI())
	{
        UNUSED(LDR_BEUpgReboot(prLdrData));
	    Printf("loader upgrade Reboot BE in 4K model\n");
	}
#endif
    // To display logo index 4 for upgrade success.
    LDR_LogoDisplay(prLdrData, 4);
#ifdef SSUSB_LOADER
    MUSB3_SetSe0(); //reset USB3 port device
#endif
    return 0;
}

static INT32 _LdrSetUpgradeBit(LDR_DATA_T *prLdrData, UINT8 u1Ch)
{
    UINT32 i4Ret;

    if (u1Ch == 0)
    {
        prLdrData->rDtvCfg.u1Flags &= (UINT8)(~(DTVCFG_FLAG_USB_UPGRADE_ON));
    }
    else if (u1Ch == 1)
    {
        prLdrData->rDtvCfg.u1Flags |= (DTVCFG_FLAG_USB_UPGRADE_ON);
    }

    i4Ret = EEPDTV_SetCfg(&prLdrData->rDtvCfg);
    if (i4Ret)
    {
        printf("eeprom write failed, return %d\n", i4Ret);
    }

    return 0;
}
#endif // CC_USB_DISABLE
#endif // LOADER_USB_UPGRADE


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

INT32 LDR_FlashWrite(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len, UINT32 u4ResetStartAddr, UINT32 u4WriteStartAddr)
{
#ifdef LOADER_USB_UPGRADE

#ifndef CC_USB_DISABLE
    if (u4ResetStartAddr)
    {
#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
        _u4ImageStartAddr = u4WriteStartAddr;
#else
        _u8ImageStartAddr = u4WriteStartAddr;
#endif
    }
    else
    {
        UNUSED(u4WriteStartAddr);
    }

    return _LdrFlashWrite(e_flag, pauc_buffer, ui4_len);
#else
    return DLM_OTHERS;
#endif /* CC_USB_DISABLE */

#else
    return DLM_OTHERS;
#endif /* LOADER_USB_UPGRADE */
}

static INT32 _LdrSetUpgradeBit_1(LDR_DATA_T *prLdrData, UINT8 u1Ch)
{
    UINT32 i4Ret;

    if (u1Ch == 0)
    {
    	 printf("\n _LdrSetUpgradeBit_1 set to 0\n");
        prLdrData->rDtvCfg.u1Flags2 &= (UINT8)(~(DTVCFG_FLAG2_UPGRADE_GO));
    }
    else if (u1Ch == 1)
    {
    	printf("\n _LdrSetUpgradeBit_1 set to 1\n");
        prLdrData->rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_UPGRADE_GO);
    }
    
    i4Ret = EEPDTV_SetCfg(&prLdrData->rDtvCfg);
    if (i4Ret)
    {
        printf("_LdrSetUpgradeBit_1 EEP write failed, return %d\n", i4Ret);
    }

    return 0;
}


UINT32 LDR_CheckUpgrade(LDR_DATA_T *prLdrData)
{
    #ifdef CC_SUPPORT_USB_DIRECT_UART 	
	PFN_GPIO_LEDCTRL_FUNC pfnLedCtrlFunc = NULL;
    #endif	
#ifdef CC_LDR_ATOMIC_MODE_SUPPORT
	UINT32 u4Val = 0;
	UINT32 u4Mode = 0;
	QUERY_TYPE_T UsbUpgradeFileName = eUsbUpgradeFileName;
    #if defined(CC_SECURESTORAGE_SUPPORT)
	UINT8 auSecureDataZone[256];
    #endif
    prLdrData->u4LdrAtomicMode = 0; //config to P-mode first
    //start : this should be replace with secrue mechnism for read
    #if defined(CC_SECURESTORAGE_SUPPORT)
	_LdrReadFromSecureStorage(0,auSecureDataZone,256);
	u4Mode = auSecureDataZone[1];
	#endif
	//end 
    switch(u4Mode)
    {
	    case 0x46:
		prLdrData->u4LdrAtomicMode = 2;
	    UNUSED(EEPROM_Read(0xA1F0, (UINT32)&u4Val, 1));
	    if(u4Val == 0x4D)
	    {
	      //start : this should be replace with secrue mechnism for read
	      #if defined(CC_SECURESTORAGE_SUPPORT)
	      auSecureDataZone[1] = u4Val;
		  _LdrWriteToSecureStorage(auSecureDataZone);
          #endif
		  //end
		  prLdrData->u4LdrAtomicMode = 1;
		  _u1UpgradeMode = 1;
	    }
	    else
	    {
	      _u1UpgradeMode = 0;
	    }		
		break;
	    case 0x4D:
		prLdrData->u4LdrAtomicMode = 1;
		_u1UpgradeMode = 1;
		break;
	    case 0x50:
		prLdrData->u4LdrAtomicMode = 0;
		_u1UpgradeMode = 0;
		break;
	    default:
	    //start : this should be replace with secrue mechnism for read
	    #if defined(CC_SECURESTORAGE_SUPPORT)
	    auSecureDataZone[1] = 0x50;
		_LdrWriteToSecureStorage(auSecureDataZone);
        #endif
		//end			
		prLdrData->u4LdrAtomicMode = 0;
		_u1UpgradeMode = 0;
		break;
    }
#endif

#ifdef LOADER_USB_UPGRADE
#ifndef CC_USB_DISABLE
    {
        UINT32 u4Addr = 0;
        // Whether will reset DIAG mode flag or not  
        if (DRVCUST_InitQuery(eEepromDIAGFlagAddr, &u4Addr) == 0)
        {
            UINT32 u4DIAG=0;
            EEPROM_Write((UINT64)u4Addr, (UINT32)&u4DIAG, 1);
        }
    }
    
#if defined(DEFINED_PFN_LDR_USB_UpgradeCheck_FUNC)
    {
        PFN_LDR_USB_UpgradeCheck_FUNC pfnLdrUSBUpgradeCheck = NULL;
        // call driver customization USB update check function. //pfnLdrUSBUpgradeCheck retrun 1-->don't do USB upgrade
        pfnLdrUSBUpgradeCheck = (PFN_LDR_USB_UpgradeCheck_FUNC)DRVCUST_InitGet(eLoaderUpgradeCheckFunc);
        if (pfnLdrUSBUpgradeCheck != NULL)
        {
            if (pfnLdrUSBUpgradeCheck())
            {
                return 0;
            }
        }
    }
#endif

#ifdef CC_LOADER_UPGRADE_FORCE_DISABLE_SUPPORT
    {
        UINT32 u4UpgradeForceDisableAddr;
        if (DRVCUST_InitQuery(eUsbUpgradeForceDisable, &u4UpgradeForceDisableAddr) == 0)
        {
            UINT8 u1Disable = 0;
            UNUSED(EEPROM_Read(u4UpgradeForceDisableAddr, (UINT32)((void *)&u1Disable), sizeof(UINT8)));
            if (u1Disable == 1)
            {
                // Force disable loader upgrade
                return 0;
            }
        }
    }
#endif /* CC_LOADER_UPGRADE_FORCE_DISABLE_SUPPORT */

    if (DRVCUST_InitGet(eloaderUpgradeOnlyACon))
    {
    	#if defined(CC_MT5890)
		if (_PDWNC_ReadWakeupStatus(0)!=0 && _PDWNC_ReadWakeupStatus(1)!=0)
		#else
        if (_PDWNC_ReadWakeupStatus()!=0)
		#endif
        {
            return 0;
        }
    }

    // Check DTVCFG_FLAG_USB_UPGRADE_ON for usb upgrade
    if ((DRVCUST_InitGet(eAutoUsbUpgradeEnable)) ||
         ((DRVCUST_InitGet(eEepromUsbUpgradeEnable)) &&
            (prLdrData->rDtvCfg.u1Flags & (DTVCFG_FLAG_USB_UPGRADE_ON))))
    {
#ifdef CC_MTK_AUTO_TEST
		QUERY_TYPE_T UsbUpgradeFileName = eUsbUpgradeFileName;
		if (EEPROM_Read((DRVCUST_InitGet(eUsbEepromAutoTestOffset)),  (UINT32)((void *)&EepromSuffixFlag), 1) == 0)
		{
            Printf("DRVCUST_EEPROM_AUTOTEST_UPGRADED_PKG_OFFSET = 0x%8x \n",(DRVCUST_InitGet(eUsbEepromAutoTestOffset))); 
            Printf("EepromSuffixFlag = %d \n",EepromSuffixFlag); 
    		
			if (EepromSuffixFlag == 1)
			{
				UsbUpgradeFileName =  eUsbUpgradeGoldenFileName;
				Printf("\n use upgrade_loader.golden to upgrade the system \n");
			}
			else
			{
				UsbUpgradeFileName =eUsbUpgradeFileName;
				Printf("\n use upgrade_loader.pkg to upgrade the system \n");
			}
		}
#endif

#ifdef CC_LDR_ATOMIC_MODE_SUPPORT
		if (_u1UpgradeMode != 0)
		{
		    UsbUpgradeFileName =  eLdrAtomicUpgFileName;
			Printf("\n use atomic pkg to upgrade the system \n");
		}
		else
	    {
			UsbUpgradeFileName =eUsbUpgradeFileName;
			Printf("\n use upgrade_loader.pkg to upgrade the system \n");
		}
		Printf("LDR_CheckUpgrade UsbUpgFileName = %s \n",(char *)DRVCUST_InitGet(UsbUpgradeFileName));
#endif
        Printf("\nDo USB upgrade\n");
        //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 1);
        HalEnableIRQ();
		if(prLdrData->rDtvCfg.u1Flags2 & (DTVCFG_FLAG_USB_UPGRADE_ON))
		{
			printf("\n @@@_LdrSetUpgradeBit_1 had set to 1: ignore this time upgrade,then clear flag to 0\n");
			_LdrSetUpgradeBit_1(prLdrData, 0);
			return 0;
		}

	    //for USB2/3 after mt5396
    	BIM_EnableIrq(VECTOR_MISC);
#ifdef SSUSB_LOADER
        if (BIM_IsIrqPending(79))
        {
            _xHCIIrqHandler(VECTOR_USB3_1);
            BIM_ClearIrq(79);        
        }
#endif
#if defined(CC_MTK_AUTO_TEST) || defined(CC_LDR_ATOMIC_MODE_SUPPORT)
		if (_LdrUsbUpgrade(prLdrData, (char *)DRVCUST_InitGet(UsbUpgradeFileName), PC_UPGRADE_CTRL_NOR_1))
#else
        if (_LdrUsbUpgrade(prLdrData, (char *)DRVCUST_InitGet(eUsbUpgradeFileName), PC_UPGRADE_CTRL_NOR_1))
#endif
        {
            Printf("USB upgrade stop\n");
#ifdef CC_SUPPORT_USB_DIRECT_UART 	   
		UNUSED(DRVCUST_InitQuery(eGpioLedCtrlFunc, (UINT32 *)(void *)&pfnLedCtrlFunc));
		if (pfnLedCtrlFunc != NULL)
		{
			pfnLedCtrlFunc(eLedUpgrageStop);
		}
#endif				
            //HalDisableIRQ();
            if (DRVCUST_InitGet(eAutoUsbUpgradeEnable))
            {
                _LdrSetUpgradeBit(prLdrData, 0);
            }
            else
            {
                _LdrSetUpgradeBit(prLdrData, 1);
            }

#ifndef CC_MT8223
            //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
#endif
            return 1;
        }

        // Set to image bank 1 after loader usb upgrade complete
        prLdrData->rDtvCfg.u1Flags2 &= (UINT8)(~(DTVCFG_FLAG2_IMAGE_BANK));
        //HalDisableIRQ();

#ifndef CC_MT8223
        //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
#endif
        _LdrSetUpgradeBit(prLdrData, 0);
		x_thread_delay(1000);
		_LdrSetUpgradeBit_1(prLdrData, 1);
		printf("\n@@@upgrade finish _LdrSetUpgradeBit_1 set to 1:ignore next upgrade\n");
		printf("\n@@@\===reboot===\n");
#ifndef CC_MT5881
		CHIP_Reboot();
#endif
    }
#endif
#endif

    return 0;
}

UINT32 LDR_ForceUpgrade(LDR_DATA_T *prLdrData)
{
#ifdef LOADER_USB_UPGRADE
#ifndef CC_USB_DISABLE
#if 0
    if ((DRVCUST_InitGet(eAutoUsbUpgradeEnable)) ||
         ((DRVCUST_InitGet(eEepromUsbUpgradeEnable)) &&
            (prLdrData->rDtvCfg.u1Flags & (DTVCFG_FLAG_USB_UPGRADE_ON))))
    {
        return 1;
    }
#endif
#ifdef CC_SUPPORT_USB_DIRECT_UART 	
	PFN_GPIO_LEDCTRL_FUNC pfnLedCtrlFunc = NULL;
#endif	
    Printf("\nForce USB upgrade\n\n");

    // Enable the upgrades setting.
    _LdrSetUpgradeBit(prLdrData, 1);
    //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 1);
    HalEnableIRQ();

    //for USB2/3 after mt5396
    BIM_EnableIrq(VECTOR_MISC);
    if (_LdrUsbUpgrade(prLdrData, (char *)DRVCUST_InitGet(eUsbUpgradeFileName), PC_UPGRADE_CTRL_NOR_1))
    {
        Printf("USB upgrade stop\n");
#ifdef CC_SUPPORT_USB_DIRECT_UART 	   
		UNUSED(DRVCUST_InitQuery(eGpioLedCtrlFunc, (UINT32 *)(void *)&pfnLedCtrlFunc));
		if (pfnLedCtrlFunc != NULL)
		{
			pfnLedCtrlFunc(eLedUpgrageStop);
		}
#endif		
        //HalDisableIRQ();
        //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
        return 1;
    }

    // Set to image bank 1 after loader usb upgrade complete
    prLdrData->rDtvCfg.u1Flags2 &= (UINT8)(~(DTVCFG_FLAG2_IMAGE_BANK));
    //HalDisableIRQ();
    //_LdrSetT8032Cmd(PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE, 2);
    _LdrSetUpgradeBit(prLdrData, 0);
#endif
#endif

    return 0;
}

#ifdef LOADER_USB_UPGRADE
#ifdef CC_DEF_PART_VER_CHK_FUNC
#include "sha256_slim.h"
static SHA256_CTX    ctx_256;

#ifndef PART_VER_TBL_TOT_LEN_ELEM_SZ
#define PART_VER_TBL_TOT_LEN_ELEM_SZ			2 //Byte
#endif
#ifndef PART_VER_TBL_PID_ELEM_SZ
#define PART_VER_TBL_PID_ELEM_SZ				1 //Byte
#endif
#ifndef PART_VER_TBL_TAG_NAME_ELEM_SZ
#define PART_VER_TBL_TAG_NAME_ELEM_SZ			4 //Byte
#endif
#ifndef PART_VER_TBL_VER_LEN_ELEM_SZ
#define PART_VER_TBL_VER_LEN_ELEM_SZ			0 //Byte
#endif
#ifndef PART_VER_TBL_VER_ELEM_SZ
#define PART_VER_TBL_VER_ELEM_SZ				4 //Byte
#endif
#ifndef PART_VER_TBL_PART_VALIDATION_TYPE
#define PART_VER_TBL_PART_VALIDATION_TYPE		0 //0:partition id, 1:tag name
#endif
#ifndef PART_VER_TBL_VER_TYPE
#define PART_VER_TBL_VER_TYPE					0 //0: Digital, 1: String, 2: Digit+Separator+Digit+..., 3: Customized
#endif
#ifndef PART_VER_TBL_VER_SEPA
#define PART_VER_TBL_VER_SEPA					".-_|:" // used when PART_VER_TBL_VER_TYPE=2
#endif
#ifndef PART_VER_TBL_STORE_MEDIUM
#define PART_VER_TBL_STORE_MEDIUM				0 //0: eeprom, >0: nand flash partition id
#endif
#ifndef PART_VER_TBL_STORE_ENC_DIGEST
#define PART_VER_TBL_STORE_ENC_DIGEST			1 //0: no encrypt, 1: AES encrypt
#endif
#ifndef PART_VER_TBL_STORE_POS_NUM
#define PART_VER_TBL_STORE_POS_NUM				1 //default allocated eep:0x240~0x360(50*5+32+2=284B<288) to store max 50 partition versions, can be customized in drv_cust/mtk/config/.h.
#endif
#ifndef PART_VER_TBL_STORE_POS_LEN_ELEM_SZ
#define PART_VER_TBL_STORE_POS_LEN_ELEM_SZ		2 //Byte
#endif

#define PART_VER_CHK_DBG_INFO		0

typedef struct
{
    CHAR    szTagName[5];
	CHAR    szVersion[PART_VER_TBL_VER_ELEM_SZ];
    UINT32  ui4VerLen;
} LDR_UPG_VER_T;
static LDR_UPG_VER_T* s_ptVerLst = NULL;

#ifndef VER_POS_INFO_T_DEF
#define VER_POS_INFO_T_DEF  \
typedef struct{ \
	UINT32 ui4_offset; \
	UINT32 ui4_len; \
} VER_POS_INFO_T;
#endif

//below is for example, it should be defined in drv_cust
#ifndef PART_VER_STORE_POS_INF_ARRAY
#define PART_VER_STORE_POS_INF_ARRAY  \
VER_POS_INFO_T PART_VER_STORE_POS_INF[PART_VER_TBL_STORE_POS_NUM] = { \
		{0x240, 0x120} \
};
#endif

#ifdef VER_POS_INFO_T_DEF
VER_POS_INFO_T_DEF
#endif

#ifdef PART_VER_STORE_POS_INF_ARRAY
PART_VER_STORE_POS_INF_ARRAY
#endif

#ifndef GET_VER_STORE_OFF
#define GET_VER_STORE_OFF(x) PART_VER_STORE_POS_INF[x].ui4_offset
#endif

#ifndef GET_VER_STORE_LEN
#define GET_VER_STORE_LEN(x) PART_VER_STORE_POS_INF[x].ui4_len
#endif

extern INT32 Loader_ReadNandFlash(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen);
extern INT32 Loader_WriteNandFlash(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen);
extern BOOL DMX_NAND_AES_Decryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize);
extern BOOL DMX_NAND_AES_Encryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize);


static INT32 ReadStorageVersionData(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
#ifdef PART_VER_CHK_DBG_INFO
	printf("\n%s,%d, off:%u, len:%u!\n", __FUNCTION__, __LINE__, u4Offset, u4MemLen);
#endif
	if (PART_VER_TBL_STORE_MEDIUM > 0)
	{
		return Loader_ReadNandFlash(PART_VER_TBL_STORE_MEDIUM, u4Offset, (void *)u4MemPtr, u4MemLen);
	}
	else
	{
		return (0 - EEPROM_Read(u4Offset, u4MemPtr, u4MemLen));
	}
}

static INT32 WriteStorageVersionData(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
#ifdef PART_VER_CHK_DBG_INFO
	printf("\n%s,%d, off:%u, len:%u!\n", __FUNCTION__, __LINE__, u4Offset, u4MemLen);
#endif
	if (PART_VER_TBL_STORE_MEDIUM > 0)
	{
		return Loader_WriteNandFlash(PART_VER_TBL_STORE_MEDIUM, u4Offset, (void *)u4MemPtr, u4MemLen);
	}
	else
	{
		return (0 - EEPROM_Write(u4Offset, u4MemPtr, u4MemLen));
	}
}

static UINT64 Buf2Int(UCHAR* ucBuf, UINT8 ui1IntSize)
{
	UINT64 ui8Return = 0;
	x_memcpy(&ui8Return, ucBuf, ui1IntSize);
	return ui8Return;
}

static void Int2Buf(UCHAR* ucBuf, UINT64 ui8Data, UINT8 ui1IntSize)
{
	x_memcpy(ucBuf, &ui8Data, ui1IntSize);
}

static UINT64 Str2Ull(UCHAR* ucStr, UINT32* stopPos)
{
	UINT32 ui4StrLen = x_strlen(ucStr);
	UINT32 i;
	UINT64 ui8Return = 0;
	
	for (i = 0; i < ui4StrLen; i++)
	{
		if (ucStr[i] < '0' || ucStr[i] > '9')
		{
			if (stopPos)
			{
				*stopPos = i;
			}
			break;
		}
		else
		{
			ui8Return = ui8Return * 10 + (ucStr[i] - '0');
		}
	}
	return ui8Return;
}

static void Ull2Str(UINT32 ui4Int, UCHAR* ucStr)
{
	UINT32 i = 0;
	UINT32 ui4Tmp = 10;
	while (ui4Int / ui4Tmp > 0)
	{
		ui4Tmp *= 10;
	}
	while (ui4Tmp > 1)
	{
		ui4Tmp /= 10;
		ucStr[i] = ui4Int / ui4Tmp + '0';
		ui4Int %= ui4Tmp;
		i++;
	}
	ucStr[i] = '\0';
}

static INT32 IsSeparator(UCHAR ucChar)
{
	UINT32 ui4StrLen = x_strlen(PART_VER_TBL_VER_SEPA);
	UINT32 i;
	for (i = 0; i < ui4StrLen; i++)
	{
		if (PART_VER_TBL_VER_SEPA[i] == ucChar)
		{
			return 1;
		}
		
	}
	return 0;
}

static void VersionDigest(UCHAR *uc_src, UCHAR *uc_digest, UINT32 ui4_len)
{
	SHA256_Init(&ctx_256);
	SHA256_Update(&ctx_256, uc_src, ui4_len);
	x_memset(uc_digest, 0, 32);
    SHA256_Final(uc_digest,&ctx_256);
}

static INT32 i4ReadVersionInfo(LDR_UPG_VER_T* ptVerLst)
{
	INT32 i4_ret = 0;
	UINT32 ui4_get_tot_len = 0;
	UINT32 ui4_len = 0;
	UINT32 ui4_blk = 0; 
	UINT32 ui4_idx = 0; 
	UCHAR* uc_enc_buf = NULL;
	UCHAR* uc_dec_buf = NULL;
	UINT32 aui4_blk_len[PART_VER_TBL_STORE_POS_NUM];
	UCHAR auc_digest[32];
	
	//read length
	x_memset(aui4_blk_len, 0, sizeof(aui4_blk_len));
	
	for (ui4_blk = 0; ui4_blk < PART_VER_TBL_STORE_POS_NUM; ui4_blk++)
	{
#ifdef PART_VER_CHK_DBG_INFO
		printf("\n%s,%d, off:%u, len:%u!\n", __FUNCTION__, __LINE__, GET_VER_STORE_OFF(ui4_blk), PART_VER_TBL_STORE_POS_LEN_ELEM_SZ);
#endif
		if (GET_VER_STORE_LEN(ui4_blk) < PART_VER_TBL_STORE_POS_LEN_ELEM_SZ)
		{
			printf("\n%s fail! Line:%d, Storage space is too small:%d!\n", __FUNCTION__, __LINE__, GET_VER_STORE_LEN(ui4_blk));
			i4_ret = -1;
			goto END_i4ReadVersionInfo;
		}
		if (ReadStorageVersionData(GET_VER_STORE_OFF(ui4_blk), 
								   (UINT32)&aui4_blk_len[ui4_blk], 
								   PART_VER_TBL_STORE_POS_LEN_ELEM_SZ) < 0)
		{
			printf("\n%s fail! Line:%d, cannot read version table!\n", __FUNCTION__, __LINE__);
			i4_ret = -1;
			goto END_i4ReadVersionInfo;
		}
		if (aui4_blk_len[ui4_blk] + PART_VER_TBL_STORE_POS_LEN_ELEM_SZ > GET_VER_STORE_LEN(ui4_blk))
		{
			printf("\n%s fail! Line:%d, version table damaged in storage!\n", __FUNCTION__, __LINE__);
			i4_ret = -1;
			goto END_i4ReadVersionInfo;
		}
		ui4_get_tot_len += aui4_blk_len[ui4_blk];
	}
	if (ui4_get_tot_len == 0)
	{
		printf("\n%s fail! Line:%d, version table is empty!\n", __FUNCTION__, __LINE__);
		i4_ret = -2;
		goto END_i4ReadVersionInfo;
	}
	//allocate buffer
	uc_enc_buf = (UCHAR*)x_mem_alloc(ui4_get_tot_len);
	if (uc_enc_buf)
	{
		x_memset(uc_enc_buf, 0, ui4_get_tot_len);
	}
	else
	{
		printf("\n%s fail! Line:%d, cannot allocate memory:%d\n", __FUNCTION__, __LINE__, ui4_get_tot_len);
		i4_ret = -3;
		goto END_i4ReadVersionInfo;
	}
	//read partition version
	ui4_len = 0;
	for (ui4_blk = 0; ui4_blk < PART_VER_TBL_STORE_POS_NUM; ui4_blk++)
	{
		if (aui4_blk_len[ui4_blk] > 0)
		{
#ifdef PART_VER_CHK_DBG_INFO
			printf("\n%s,%d, off:%u, len:%u!\n", __FUNCTION__, __LINE__, GET_VER_STORE_OFF(ui4_blk)+PART_VER_TBL_STORE_POS_LEN_ELEM_SZ, aui4_blk_len[ui4_blk]);
#endif
			if (ReadStorageVersionData(GET_VER_STORE_OFF(ui4_blk)+PART_VER_TBL_STORE_POS_LEN_ELEM_SZ, 
									   (UINT32)(&uc_enc_buf[ui4_len]), 
									   aui4_blk_len[ui4_blk]) < 0)
			{
				printf("\n%s fail! Line:%d, cannot read version table!\n", __FUNCTION__, __LINE__);
				i4_ret = -1;
				goto END_i4ReadVersionInfo;
			}
			ui4_len += aui4_blk_len[ui4_blk];
		}
	}
	//encrypt & digest
	if (PART_VER_TBL_STORE_ENC_DIGEST)
	{
		// decrypt 
		uc_dec_buf = (UCHAR*)x_mem_alloc(ui4_get_tot_len);
		if (uc_dec_buf)
		{
			x_memset(uc_dec_buf, 0, ui4_get_tot_len);
		}
		else
		{
			printf("\n%s fail! Line:%d, cannot allocate memory:%d\n", __FUNCTION__, __LINE__, ui4_get_tot_len);
			i4_ret = -3;
			goto END_i4ReadVersionInfo;
		}
		if (!DMX_NAND_AES_Decryption((UINT32)uc_enc_buf, (UINT32)uc_dec_buf, ui4_get_tot_len))
		{
			printf("\n%s fail! Line:%d, Decrypt Error Found!\n", __FUNCTION__, __LINE__);
			i4_ret = -4;
			goto END_i4ReadVersionInfo;
		}
		//digest check
		if (ui4_get_tot_len < 32)
		{
			printf("\n%s fail! Line:%d, cannot digest for length:%d is smaller than 32!\n", __FUNCTION__, __LINE__, ui4_get_tot_len);
			i4_ret = -5;
			goto END_i4ReadVersionInfo;
		}
		ui4_get_tot_len = (UINT32)Buf2Int(uc_dec_buf, PART_VER_TBL_TOT_LEN_ELEM_SZ)+PART_VER_TBL_TOT_LEN_ELEM_SZ;
		VersionDigest((UCHAR *)uc_dec_buf, (UCHAR *)auc_digest, ui4_get_tot_len);
		if (x_strncmp(auc_digest, &uc_dec_buf[ui4_get_tot_len], 32) != 0)
		{
			printf("\n%s fail! Line:%d, version table is corrupted!\n", __FUNCTION__, __LINE__);
			i4_ret = -6;
			goto END_i4ReadVersionInfo;
		}
	}
	else//no encrypt & digest
	{
		uc_dec_buf = uc_enc_buf;
		uc_enc_buf = NULL;
	}
	//save to structure
	ui4_idx = 0;
	for (ui4_len = PART_VER_TBL_TOT_LEN_ELEM_SZ; ui4_len < ui4_get_tot_len; ui4_idx++)
	{
		UINT32 ui4_ver_len = 0;
		UINT32 ui4_id = 0;
		
		if (PART_VER_TBL_PART_VALIDATION_TYPE == 1)//tag name
		{
			x_memcpy(ptVerLst[ui4_idx].szTagName, &uc_dec_buf[ui4_len], PART_VER_TBL_TAG_NAME_ELEM_SZ);
			ui4_len += PART_VER_TBL_TAG_NAME_ELEM_SZ;
		}
		else//partition id
		{
			ui4_id = (UINT32)Buf2Int(&uc_dec_buf[ui4_len], PART_VER_TBL_PID_ELEM_SZ);
			ui4_len += PART_VER_TBL_PID_ELEM_SZ;
		}
		//version length
		if (PART_VER_TBL_VER_LEN_ELEM_SZ > 0)
		{
			ui4_ver_len = (UINT32)Buf2Int(&uc_dec_buf[ui4_len], PART_VER_TBL_VER_LEN_ELEM_SZ);
		}
		else
		{
			ui4_ver_len = PART_VER_TBL_VER_ELEM_SZ;
		}
		
		ui4_len += PART_VER_TBL_VER_LEN_ELEM_SZ;
		//version
		if (ui4_ver_len <= PART_VER_TBL_VER_ELEM_SZ)
		{
			if (PART_VER_TBL_PART_VALIDATION_TYPE == 1)//tag name
			{
				x_memcpy(ptVerLst[ui4_idx].szVersion, &uc_dec_buf[ui4_len], ui4_ver_len);
				ptVerLst[ui4_idx].ui4VerLen = ui4_ver_len;
			}
			else//partition id
			{
				x_memcpy(ptVerLst[ui4_id].szVersion, &uc_dec_buf[ui4_len], ui4_ver_len);
				ptVerLst[ui4_id].ui4VerLen = ui4_ver_len;
			}
		}
		else
		{
			printf("\n%s fail! Line:%d, Version data length:%d is larger than physical memory space!\n", __FUNCTION__, __LINE__, ui4_ver_len);
			i4_ret = -5;
			goto END_i4ReadVersionInfo;
		}
		ui4_len += ui4_ver_len;
	}
END_i4ReadVersionInfo:	
	if (uc_enc_buf)
	{
		x_mem_free(uc_enc_buf);
		uc_enc_buf = NULL;
	}
	if (uc_dec_buf)
	{
		x_mem_free(uc_dec_buf);
		uc_dec_buf = NULL;
	}
	return i4_ret;
}

static INT32 i4WriteVersionInfo(LDR_UPG_VER_T* ptVerLst)
{
	INT32 i4_ret = 0;
	UINT32 ui4_idx = 0;
	UINT16 ui4_blk = 0; 
	UINT32 ui4_store_len = 0;
	UINT32 ui4_set_tot_len = 0;
	UCHAR* uc_enc_buf = NULL;
	UCHAR* uc_set_buf = NULL;

	//calculate total length of version table
	for (ui4_blk = 0; ui4_blk < PART_VER_TBL_STORE_POS_NUM; ui4_blk++)
	{
		ui4_store_len += GET_VER_STORE_LEN(ui4_blk);
		if (GET_VER_STORE_LEN(ui4_blk) >= PART_VER_TBL_STORE_POS_LEN_ELEM_SZ)
		{
			ui4_store_len -= PART_VER_TBL_STORE_POS_LEN_ELEM_SZ;
		}
		else
		{
			printf("\n%s fail! Line:%d, Storage space is too small:%d!\n", __FUNCTION__, __LINE__, GET_VER_STORE_LEN(ui4_blk));
			i4_ret = -1;
			goto END_i4WriteVersionInfo;
		}
	}
	for (ui4_idx = 0; ui4_idx < MAX_UPG_CFG_NUM; ui4_idx++)
	{
		if (ptVerLst[ui4_idx].ui4VerLen > 0)
		{
			ui4_set_tot_len += ptVerLst[ui4_idx].ui4VerLen;
			if (PART_VER_TBL_PART_VALIDATION_TYPE == 1)//tag name
			{
				ui4_set_tot_len += PART_VER_TBL_TAG_NAME_ELEM_SZ;
			}
			else//partition id
			{
				ui4_set_tot_len += PART_VER_TBL_PID_ELEM_SZ;
			}
		}
	}
	if (ui4_set_tot_len == 0)
	{
		printf("\n%s fail! Line:%d, version table is null!\n", __FUNCTION__, __LINE__);
		i4_ret = -1;
		goto END_i4WriteVersionInfo;
	}
	ui4_set_tot_len += PART_VER_TBL_TOT_LEN_ELEM_SZ; 		//reserve for tot data len
	//allocate buffer
	uc_set_buf = (UCHAR*)x_mem_alloc(ui4_set_tot_len+32); //32 is reserve for digest
	if (uc_set_buf)
	{
		x_memset(uc_set_buf, 0, ui4_set_tot_len+32);
	}
	else
	{
		printf("\n%s fail! Line:%d, cannot allocate memory:%d\n", __FUNCTION__, __LINE__, ui4_set_tot_len+32);
		i4_ret = -2;
		goto END_i4WriteVersionInfo;
	}
	//set total length
	Int2Buf(uc_set_buf, ui4_set_tot_len-PART_VER_TBL_TOT_LEN_ELEM_SZ, PART_VER_TBL_TOT_LEN_ELEM_SZ);

	//copy partition versions to buffer
	ui4_set_tot_len = PART_VER_TBL_TOT_LEN_ELEM_SZ;
	for (ui4_idx = 0; ui4_idx < MAX_UPG_CFG_NUM; ui4_idx++)
	{
		if (ptVerLst[ui4_idx].ui4VerLen > 0)
		{
			//partition id
			if (PART_VER_TBL_PART_VALIDATION_TYPE == 1)//tag name
			{
				x_memcpy(&uc_set_buf[ui4_set_tot_len], ptVerLst[ui4_idx].szTagName, PART_VER_TBL_TAG_NAME_ELEM_SZ);
				ui4_set_tot_len += PART_VER_TBL_TAG_NAME_ELEM_SZ;
			}
			else
			{
				Int2Buf(&uc_set_buf[ui4_set_tot_len], ui4_idx, PART_VER_TBL_PID_ELEM_SZ);
				ui4_set_tot_len += PART_VER_TBL_PID_ELEM_SZ;
			}
			//version length
			if (PART_VER_TBL_VER_LEN_ELEM_SZ > 0)
			{
				x_memcpy(&uc_set_buf[ui4_set_tot_len], &ptVerLst[ui4_idx].ui4VerLen, PART_VER_TBL_VER_LEN_ELEM_SZ);
				ui4_set_tot_len += PART_VER_TBL_VER_LEN_ELEM_SZ;
			}
			//version
			x_memcpy(&uc_set_buf[ui4_set_tot_len], ptVerLst[ui4_idx].szVersion, ptVerLst[ui4_idx].ui4VerLen);
			ui4_set_tot_len += ptVerLst[ui4_idx].ui4VerLen;
		}
	}

	//encrypt & digest
	if (PART_VER_TBL_STORE_ENC_DIGEST)
	{
		//digest 
		VersionDigest((UCHAR *)uc_set_buf, (UCHAR *)&uc_set_buf[ui4_set_tot_len], ui4_set_tot_len);
		ui4_set_tot_len += 32;
		// encrypt 
		if(ui4_set_tot_len & 0x0F)
		{
			ui4_set_tot_len = (ui4_set_tot_len & 0xFFFFFFF0) + 0x10;
		}
		uc_enc_buf = (UCHAR*)x_mem_alloc(ui4_set_tot_len);
		if (uc_enc_buf)
		{
			x_memset(uc_enc_buf, 0, ui4_set_tot_len);
		}
		else
		{
			printf("\n%s fail! Line:%d, cannot allocate memory:%d\n", __FUNCTION__, __LINE__, ui4_set_tot_len);
			i4_ret = -2;
			goto END_i4WriteVersionInfo;
		}
		if (!DMX_NAND_AES_Encryption((UINT32)uc_set_buf, (UINT32)uc_enc_buf, ui4_set_tot_len))
		{
			printf("\n%s fail! Line:%d, Encrypt Error Found!\n", __FUNCTION__, __LINE__);
			i4_ret = -2;
			goto END_i4WriteVersionInfo;
		}
	}
	else
	{
		uc_enc_buf = uc_set_buf;
		uc_set_buf = NULL;
	}

	if (ui4_set_tot_len > ui4_store_len)
	{
		printf("\n%s fail! Line:%d, version table is too large to store!\n", __FUNCTION__, __LINE__);
		i4_ret = -1;
		goto END_i4WriteVersionInfo;
	}
	//save to storage
	ui4_store_len = 0;
	for (ui4_blk = 0; ui4_blk < PART_VER_TBL_STORE_POS_NUM; ui4_blk++)
	{
		UINT32 ui4_len = 0;
		UINT32 ui4Offset = GET_VER_STORE_OFF(ui4_blk);
		
		if (ui4_store_len < ui4_set_tot_len)
		{
			//calculate length
			if (GET_VER_STORE_LEN(ui4_blk) - PART_VER_TBL_STORE_POS_LEN_ELEM_SZ > ui4_set_tot_len - ui4_store_len)
			{
				ui4_len = ui4_set_tot_len - ui4_store_len;
			}
			else
			{
				ui4_len = GET_VER_STORE_LEN(ui4_blk) - PART_VER_TBL_STORE_POS_LEN_ELEM_SZ;
			}
			//write length
#ifdef PART_VER_CHK_DBG_INFO
			printf("\n%s,%d, off:%u, len:%u!\n", __FUNCTION__, __LINE__, ui4Offset, PART_VER_TBL_STORE_POS_LEN_ELEM_SZ);
#endif
			if (WriteStorageVersionData(ui4Offset, (UINT32)&ui4_len, PART_VER_TBL_STORE_POS_LEN_ELEM_SZ) < 0)
			{
				printf("\n%s fail! Line:%d, cannot store version table!\n", __FUNCTION__, __LINE__);
				i4_ret = -3;
				goto END_i4WriteVersionInfo;
			}

			ui4Offset += PART_VER_TBL_STORE_POS_LEN_ELEM_SZ;
			//write data
#ifdef PART_VER_CHK_DBG_INFO
			printf("\n%s,%d, off:%u, len:%u!\n", __FUNCTION__, __LINE__, ui4Offset, ui4_len);
#endif
			if (WriteStorageVersionData(ui4Offset, (UINT32)(uc_enc_buf + ui4_store_len), ui4_len))
			{
				printf("\n%s fail! Line:%d, cannot store version table!\n", __FUNCTION__, __LINE__);
				i4_ret = -3;
				goto END_i4WriteVersionInfo;
			}
			ui4_store_len += ui4_len;
		}
		else//other block length should set to 0
		{
			//ui4_len=0
#ifdef PART_VER_CHK_DBG_INFO
			printf("\n%s,%d, off:%u, len:%u!\n", __FUNCTION__, __LINE__, ui4Offset, PART_VER_TBL_STORE_POS_LEN_ELEM_SZ);
#endif
			if (WriteStorageVersionData(ui4Offset, (UINT32)&ui4_len, PART_VER_TBL_STORE_POS_LEN_ELEM_SZ))
			{
				printf("\n%s fail! Line:%d, cannot store version table!\n", __FUNCTION__, __LINE__);
				i4_ret = -3;
				goto END_i4WriteVersionInfo;
			}
		}
	}
END_i4WriteVersionInfo:	
	if (uc_set_buf)
	{
		x_mem_free(uc_set_buf);
		uc_set_buf = NULL;
	}
	if (uc_enc_buf)
	{
		x_mem_free(uc_enc_buf);
		uc_enc_buf = NULL;
	}
	return i4_ret;
}


static void LOADER_DefUpgradeDlmGetTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer)
{
	UINT32 u4Idx = 0;

	if (ucTagName == NULL || PART_VER_TBL_STORE_POS_NUM == 0)
	{
		printf("\n%s fail! Line:%d, ucTagName:%s invalid!\n", __FUNCTION__, __LINE__, ucTagName);
		return;
	}
	if (PART_VER_TBL_STORE_POS_NUM == 0)
	{
		printf("\n%s fail! Line:%d, No space to store version!\n", __FUNCTION__, __LINE__);
		return;
	}
	
	if (!s_ptVerLst)
	{
		s_ptVerLst = (LDR_UPG_VER_T*)x_mem_alloc(MAX_UPG_CFG_NUM * sizeof(LDR_UPG_VER_T));
		if (s_ptVerLst)
		{
			x_memset(s_ptVerLst, 0, MAX_UPG_CFG_NUM * sizeof(LDR_UPG_VER_T));
		}
		else
		{
			printf("\n%s fail! Line:%d, cannot allocate memory:%d\n", __FUNCTION__, __LINE__, MAX_UPG_CFG_NUM * sizeof(LDR_UPG_VER_T));
			return;
		}
		
		if (i4ReadVersionInfo(s_ptVerLst) != 0)
		{
			printf("\n%s fail! Line:%d, cannot read version info from storage.\n", __FUNCTION__, __LINE__);
			x_mem_free(s_ptVerLst);
			s_ptVerLst = NULL;
			return;
		}
	}
	
	if (PART_VER_TBL_PART_VALIDATION_TYPE == 1)//tag name
	{
		for (u4Idx = 0; u4Idx < MAX_UPG_CFG_NUM; u4Idx++)
		{
			if (x_strncmp(ucTagName, s_ptVerLst[u4Idx].szTagName, PART_VER_TBL_TAG_NAME_ELEM_SZ) == 0)
			{
				break;
			}
		}
		if (u4Idx == MAX_UPG_CFG_NUM)
		{
			Printf("\n%s fail! Line:%d, ucTagName:%s has not version!\n", __FUNCTION__, __LINE__, ucTagName);
			return;
		}
	}
	else//partition id
	{
		for (u4Idx = 0; u4Idx < _u4NoOfTags; u4Idx++)
		{
			if (x_strncmp(ucTagName, _arUpgCfg[u4Idx].szTagName, 4) == 0)
			{
				break;
			}
		}
		if (u4Idx == _u4NoOfTags)
		{
			Printf("\n%s fail! Line:%d, ucTagName:%s has not id!\n", __FUNCTION__, __LINE__, ucTagName);
			return;
		}
		if (u4Idx < _u4NoOfTags)
		{
			u4Idx = _arUpgCfg[u4Idx].u1PartitionID;
		}
	}
#ifdef PART_VER_CHK_DBG_INFO
	printf("\nEnter %s,%d,Tag:%s,ID:%d\n", __FUNCTION__, __LINE__, ucTagName, u4Idx);
#endif
	//get version
	if (s_ptVerLst[u4Idx].ui4VerLen > 0)
	{
		if (PART_VER_TBL_VER_TYPE == 0)
		{
			UINT32 ui4Ver = (UINT32)Buf2Int(s_ptVerLst[u4Idx].szVersion, s_ptVerLst[u4Idx].ui4VerLen); 
			Ull2Str(ui4Ver, szCurVer);
		}
		else
		{
			x_memcpy(szCurVer, &s_ptVerLst[u4Idx].szVersion, s_ptVerLst[u4Idx].ui4VerLen);
		}
	}

#ifdef PART_VER_CHK_DBG_INFO
	printf("\nExit %s,%d,szCurVer:%s\n", __FUNCTION__, __LINE__, szCurVer);
#endif
}

static void LOADER_DefUpgradeDlmSetTagVerCbFunc(UCHAR* ucTagName, CHAR *szUpgVer)
{
	UINT32 u4Idx = 0;

	if (ucTagName == NULL || sizeof(ucTagName) > 5)
	{
		printf("\n%s fail! Line:%d, Invalid ucTagName:%s!\n", __FUNCTION__, __LINE__, ucTagName);
		return;
	}
	if (sizeof(szUpgVer) > PART_VER_TBL_VER_ELEM_SZ || (szUpgVer == NULL && x_strncmp(ucTagName, "end", 3) != 0))
	{
		printf("\n%s fail! Line:%d, Invalid szUpgVer:%s!\n", __FUNCTION__, __LINE__, szUpgVer);
		return;
	}
	if (PART_VER_TBL_STORE_POS_NUM == 0)
	{
		printf("\n%s fail! Line:%d, No space to store version!\n", __FUNCTION__, __LINE__);
		return;
	}
	
	if (!s_ptVerLst)
	{
		s_ptVerLst = (UCHAR*)x_mem_alloc(MAX_UPG_CFG_NUM * sizeof(LDR_UPG_VER_T));
		if (s_ptVerLst)
		{
			x_memset(s_ptVerLst, 0, MAX_UPG_CFG_NUM * sizeof(LDR_UPG_VER_T));
		}
		else
		{
			printf("\n%s fail! Line:%d, cannot allocate memory:%d\n", __FUNCTION__, __LINE__, MAX_UPG_CFG_NUM * sizeof(LDR_UPG_VER_T));
			return;
		}
	}
	
	if (PART_VER_TBL_PART_VALIDATION_TYPE == 1)//tag name
	{
		for (u4Idx = 0; u4Idx < MAX_UPG_CFG_NUM; u4Idx++)
		{
			if (x_strncmp(ucTagName, s_ptVerLst[u4Idx].szTagName, PART_VER_TBL_TAG_NAME_ELEM_SZ) == 0 ||
				s_ptVerLst[u4Idx].ui4VerLen == 0)
			{
				break;
			}
		}
		if (u4Idx == MAX_UPG_CFG_NUM)
		{
			Printf("\n%s fail! Line:%d, ucTagName:%s has not version!\n", __FUNCTION__, __LINE__, ucTagName);
			return;
		}
	}
	else//partition id
	{
		for (u4Idx = 0; u4Idx < _u4NoOfTags; u4Idx++)
		{
			if (x_strncmp(ucTagName, _arUpgCfg[u4Idx].szTagName, 4) == 0)
			{
				break;
			}
		}
		if (u4Idx == _u4NoOfTags && x_strncmp(ucTagName, "end", 3) != 0)
		{
			Printf("\n%s fail! Line:%d, ucTagName:%s has not id\n", __FUNCTION__, __LINE__, ucTagName);
			return;
		}
		if (u4Idx < _u4NoOfTags)
		{
			u4Idx = _arUpgCfg[u4Idx].u1PartitionID;
		}
	}
#ifdef PART_VER_CHK_DBG_INFO
		printf("\nEnter %s,%d,Tag:%s,PID:%d,szUpgVer:%s\n", __FUNCTION__, __LINE__, ucTagName, u4Idx, szUpgVer);
#endif
	if (x_strncmp(ucTagName, "end", 3) == 0)
	{
		i4WriteVersionInfo(s_ptVerLst);
		//delete s_ptVerLst
		if (s_ptVerLst)
		{
			x_mem_free(s_ptVerLst);
			s_ptVerLst = NULL;
		}
#ifdef PART_VER_CHK_DBG_INFO
		printf("\nEnd %s,%d\n", __FUNCTION__, __LINE__);
#endif
	}
	else
	{
		x_memcpy(s_ptVerLst[u4Idx].szTagName, ucTagName, sizeof(ucTagName));
		if (PART_VER_TBL_VER_LEN_ELEM_SZ > 0)
		{
			s_ptVerLst[u4Idx].ui4VerLen = sizeof(szUpgVer);
			x_memcpy(s_ptVerLst[u4Idx].szVersion, szUpgVer, sizeof(szUpgVer));
		}
		else
		{
			UINT64 ui8Ver = Str2Ull(szUpgVer, NULL);
			s_ptVerLst[u4Idx].ui4VerLen = PART_VER_TBL_VER_ELEM_SZ;
			Int2Buf(s_ptVerLst[u4Idx].szVersion, ui8Ver, PART_VER_TBL_VER_ELEM_SZ);
		}
	}
#ifdef PART_VER_CHK_DBG_INFO
	printf("\nExit %s,%d\n", __FUNCTION__, __LINE__);
#endif
}

// compare if corresponding tag could be download or not.
static INT32 LOADER_DefUpgradeDlmCmpTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer, CHAR *szUpgVer)
{
	INT32 i4_ret = 0;
	
#ifdef PART_VER_CHK_DBG_INFO
	printf("\nEnter %s,%d,Tag:%s,szCurVer:%s,szUpgVer:%s\n", __FUNCTION__, __LINE__, ucTagName, szCurVer, szUpgVer);
#endif
	if(PART_VER_TBL_STORE_POS_NUM == 0)
	{
		return 1;
	}
	
	if (PART_VER_TBL_VER_TYPE == 1)
	{
		if (x_strcmp(szUpgVer, szCurVer) > 0)
		{
		    i4_ret = 1;
		}
	}
	else if (PART_VER_TBL_VER_TYPE == 0)
	{
		UINT64 ui8_cur_ver = Str2Ull(szCurVer, NULL);
		UINT64 ui8_upg_ver = Str2Ull(szUpgVer, NULL);

	    // compare here, return 1 as allow, return 0 as NOT allow.
	    if (ui8_upg_ver > ui8_cur_ver)
		{
		    i4_ret = 1;
		}
	}
	else if (PART_VER_TBL_VER_TYPE == 2)
	{
		UINT64 ui8_cur_ver = 0;
		UINT64 ui8_upg_ver = 0;
		UINT32 ui4_cur_iter = 0;
		UINT32 ui4_upg_iter = 0;
		while (szCurVer[ui4_cur_iter] && szUpgVer[ui4_upg_iter])
		{
			ui8_upg_ver = Str2Ull(&szUpgVer[ui4_upg_iter], &ui4_upg_iter);
			ui8_cur_ver = Str2Ull(&szCurVer[ui4_cur_iter], &ui4_cur_iter);
			if (ui8_upg_ver > ui8_cur_ver)
			{
				i4_ret = 1;
				break;
			}
			else if (ui8_upg_ver < ui8_cur_ver)
			{
				break;
			}
			else
			{
				if (szUpgVer[ui4_upg_iter] == '\0')
				{
					break;
				}
				else if (szCurVer[ui4_cur_iter] == '\0')
				{
					i4_ret = 1;
					break;
				}
				else if (IsSeparator(szCurVer[ui4_cur_iter]) && IsSeparator(szUpgVer[ui4_upg_iter]))
				{
					ui4_upg_iter ++;
					ui4_cur_iter ++;
				}
				else 
				{
					break;
				}
			}
		}
	}
#ifdef PART_VER_CHK_DBG_INFO
	printf("\nExit %s,%d,ret:%d\n", __FUNCTION__, __LINE__, i4_ret);
#endif
	return i4_ret;
}

static void write_new_pkg_part_version(CHAR* szFileNameWithPath)
{
    CHAR szTagData[64];
    CHAR szUpgVer[128];
    UCHAR* szTag;
    UCHAR a_szTag[5];
    UINT32 u4VerOff, u4VerLen;
	
    Append_T* prAppendTag = NULL;
    PFN_DLM_SET_TAG_VER_CB_FUNC pfnSetVerCb = NULL;

	if (_i1SavePartVersion)
	{
		u4VerOff = DRVCUST_InitGet(eLoaderUpgradeDlmVerDataOff);
		u4VerLen = DRVCUST_InitGet(eLoaderUpgradeDlmVerDataLen);
		if (u4VerOff < 4) //must be after 4 bytes tag, e.g. "iMtk"
		{
			ASSERT(0);
			return;
		}
		
		if ((u4VerOff + u4VerLen) > sizeof(szTagData))
		{
			ASSERT(0);
			return;
		}
		
		if (u4VerLen > sizeof(szUpgVer))
		{
			ASSERT(0);
			return;
		}
		
		pfnSetVerCb = (PFN_DLM_GET_TAG_VER_CB_FUNC)DRVCUST_InitGet(eLoaderUpgradeDlmSetTagVerCbFunc);
		if (pfnSetVerCb == NULL)
		{
			pfnSetVerCb = LOADER_DefUpgradeDlmSetTagVerCbFunc;
		}
		
		prAppendTag = x_dlm_get_codefile_tag_list(szFileNameWithPath);
		while (prAppendTag != NULL)
		{
			szTag = prAppendTag->uc_type;
		
			x_memcpy((void*)a_szTag, (void*)szTag, 4);
			a_szTag[4] = 0;
		
			// 2.1 get PKG version by x_dlm_tag_test()
			x_dlm_tag_test(szFileNameWithPath, szTag, NULL, szTagData, sizeof(szTagData));
		
			// 2.2 check if it starts with a DLM_APPEND_HDR_MAGIC, i.e."iMtk"
			if (x_strncmp(szTagData, "iMtK", 4) == 0)
			{
				x_memset(szUpgVer, 0, sizeof(szUpgVer));
		
				u4VerLen = *(UINT32*)&szTagData[u4VerOff - 4];
				x_strncpy(szUpgVer, &szTagData[u4VerOff], u4VerLen);
		
				//save the upg version
				pfnSetVerCb(a_szTag, szUpgVer);
			}
			prAppendTag = prAppendTag->pt_next;
		}
		pfnSetVerCb("end", NULL);
	}
}
#endif
#endif


