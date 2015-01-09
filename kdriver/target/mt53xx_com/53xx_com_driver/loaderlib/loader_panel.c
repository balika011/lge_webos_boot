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
 * $RCSfile: loader_panel.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file loader_if.c
 *  loader_if.c provides loader high-level interface functions.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "c_model.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#ifndef LOADER_LOGO_NUMBER
#define LOADER_LOGO_NUMBER          (0)
#endif

#ifndef LOADER_MULTIPLE_LOGO_PARTITION
#define LOADER_MULTIPLE_LOGO_PARTITION	(0)
#endif

#ifndef LOADER_LED_TWINKLE
#define LOADER_LED_TWINKLE          (0)
#endif

#ifndef CC_LOADER_LOGO_LONG_TIME
#define CC_LOADER_LOGO_LONG_TIME
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#if 1 //LOADER_LOGO_NUMBER || LOADER_MULTIPLE_LOGO_PARTITION
#include "ostg_if.h"
#include "pmx_drvif.h"
#include "pmx_if.h"
#include "osd_drvif.h"
#include "gfx_if.h"
#include "panel.h"
#include "drv_video.h"
#include "drv_vdoclk.h"
#include "drv_display.h"
#include "drv_lvds.h"
#include "sv_const.h"
#include "hw_scpos.h"
#include "hw_lvds.h"
#include "vdp_if.h"
#include "nptv_if.h"
#include "x_pdwnc.h"
#if defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5860) || defined(CC_MT5881)
#include "drv_tcon.h"
#endif
#endif /* LOADER_LOGO_NUMBER */
#include "loader_if.h"

#include "x_hal_926.h"
#ifdef CC_MT5395
#include "drv_vbyone.h"
#endif

#ifdef CC_ONE_PKG_MULTI_DRAM_SIZE_SUPPORT

#include "x_dram.h"
#include "x_bim.h"

#endif 

//---------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------

//#if  LOADER_LOGO_NUMBER || LOADER_MULTIPLE_LOGO_PARTITION
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef CC_UPGRADING_DYNAMIC_LOGO_SUPPORT
static DYNAMIC_LOGO_INFO_T rDynamicLogo;
#endif
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
BOOL LDR_IsLogoDisplay(LDR_DATA_T *prLdrData)
{
#if LOADER_LOGO_NUMBER > 0
    BOOL bFlagEnableLogo=FALSE;
	DTVCFG_T rDtvCfg;
	rDtvCfg = prLdrData->rDtvCfg;

	rDtvCfg = prLdrData->rDtvCfg;
	if (rDtvCfg.u1Flags2 & DTVCFG_FLAG2_DISABLE_LOGO)
	{
	  bFlagEnableLogo = TRUE;
	}
	if (rDtvCfg.u1Flags2 & DTVCFG_FLAG2_QUIET_BOOT)
	{
	  bFlagEnableLogo = FALSE;
	}	
    return bFlagEnableLogo;
#else /* LOADER_LOGO_NUMBER > 0 */
    return 0;
#endif /* LOADER_LOGO_NUMBER > 0 */
}

//-----------------------------------------------------------------------------
/** LDR_OsdDisplay(): Setup OSD driver.
 *  This function is going to setup osd driver with bmp info.
 *
 *  @param  u4BmpAddr       The bmp data pointer.
 *  @param  u4Width         The width info of the bmp data.
 *  @param  u4Height        The height info of the bmp data.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
#ifndef CC_LOGO_FIT_PANEL_RESIZE
UINT32 LDR_OsdDisplay(UINT32 u4ColorMode, UINT32 u4BmpAddr, UINT32 u4Width, UINT32 u4Height,UINT32 ScaleUpMutiple)
#else
UINT32 LDR_OsdDisplay(UINT32 u4ColorMode, UINT32 u4BmpAddr,
                        UINT32 u4Width, UINT32 u4Height,
                        UINT32 u4PerfactWidth,UINT32 u4PerfactHeight)
#endif
{
    UINT32 u4RegionList, u4Region, u4BmpPitch;
    UINT32 u4PanelWidth, u4PanelHeight;
    UINT32 u4OutWidth, u4OutHeight, u4OutX, u4OutY;
    UINT32 u4OsdPlane = OSD_PLANE_2;
    UINT32 u4Scaler = OSD_SCALER_2;
    UINT32 ui4OSDMonitorReg = 0;
    INT32 ret;

#ifdef CC_FLIP_MIRROR_SUPPORT
    //UINT32 u4OutX, u4OutY;
    UINT8 u1Config;
#endif
#ifdef CC_LOADER_LOGO_LONG_TIME
  UINT32 u4DisAddr = 0;
  UINT32 u4HaderAddr = 0;
  UINT32 u4FirstRegionAddr = 0;
  
#endif

    #define UPDATE_TIMING_BIT  (1 << 23)
    #define OSD_BASE_MONITOR   (OSD_BASE_REG+ 0xfc)
	UINT32 _au4PlaneOrderArray[PMX_MAX_INPORT_NS] =    //need keep the same with fb init setting
	{
#ifdef ANDROID
	    PMX_OSD3,
#else
        PMX_OSD2,
#endif
#ifdef ANDROID
	    PMX_OSD2,
#else
        PMX_OSD3,
#endif
        PMX_OSD1, 
        PMX_PIP, 
        PMX_MAIN 
    };

    Printf("Color:%d BmpAddr:0x%08x Width:%d Height:%d\n", u4ColorMode, u4BmpAddr, u4Width, u4Height);
    OSD_Init();
    ui4OSDMonitorReg = IO_READ32MSK(OSD_BASE_MONITOR, 0, UPDATE_TIMING_BIT);
    IO_WRITE32MSK(OSD_BASE_MONITOR, 0, UPDATE_TIMING_BIT, UPDATE_TIMING_BIT);
    OSD_UpdateReg();

#ifdef CC_LOGO_FIT_PANEL_RESIZE
    ASSERT(u4PerfactWidth>0);
    ASSERT(u4PerfactHeight>0);
#endif

    u4PanelWidth = PANEL_GetPanelWidth();
    u4PanelHeight = PANEL_GetPanelHeight();
#ifdef CC_LOADER_LOGO_LONG_TIME
   u4HaderAddr =TOTAL_MEM_SIZE - FBM_MEM_CFG_SIZE + 0x1780000;
//   prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG2);
//   u4HaderAddr = prFbmPool->u4Addr;

   u4DisAddr = u4HaderAddr + 0x1000;
   Printf("u4HaderAddr = 0x%x, u4DisAddr=0x%x\n", u4HaderAddr, u4DisAddr);
#endif

    Printf("Panel %d x %d \n", u4PanelWidth, u4PanelHeight);

    ret = OSD_RGN_LIST_Create(&u4RegionList);
    if (ret != OSD_RET_OK) return 1;
    u4BmpPitch = 0;

	if(u4Width == 1366)
	{
		OSD_GET_PITCH_SIZE(u4ColorMode, u4Width+2, u4BmpPitch); // to set u4BmpPitch by u4ColorMode and u4Width.
	}
	else
	{
		OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch);
	}
			
    Printf("\n*************u4BmpPitch=%d******************\n",u4BmpPitch);

    OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch); // to set u4BmpPitch by u4ColorMode and u4Width.
    u4BmpPitch = (u4BmpPitch +0xF) & (~0xF);
   #ifdef CC_LOADER_LOGO_LONG_TIME
   x_memcpy((void*)u4DisAddr, (void*)u4BmpAddr, u4BmpPitch*u4Height);
     ret = OSD_RGN_Create(&u4Region, u4Width, u4Height, (void *)PHYSICAL(u4DisAddr),
                            u4ColorMode, u4BmpPitch, 0, 0, u4Width, u4Height);
    if (ret != OSD_RET_OK) return 2;
  #else
    ret = OSD_RGN_Create(&u4Region, u4Width, u4Height, (void *)PHYSICAL(u4BmpAddr),
                            u4ColorMode, u4BmpPitch, 0, 0, u4Width, u4Height);
    if (ret != OSD_RET_OK) return 2;
#endif

#ifdef CC_FLIP_MIRROR_SUPPORT
  u1Config= u1GetFlipMirrorConfig();
  if (u1Config & SYS_MIRROR_CONFIG_ON) {
    ret = OSD_RGN_Set(u4Region,OSD_RGN_H_MIRROR,1);
  }
  if (u1Config & SYS_FLIP_CONFIG_ON) {
    ret = OSD_RGN_Set(u4Region,OSD_RGN_V_FLIP,1);
  }
ret = _OSD_BASE_UpdateHwReg();
#endif

    ret = OSD_RGN_Insert(u4Region, u4RegionList);
    if (ret != OSD_RET_OK) return 3;
    ret = OSD_PLA_FlipTo(OSD_PLANE_2, u4RegionList);
    if (ret != OSD_RET_OK) return 4;

    // Scaler bmp size. 
#ifndef CC_LOGO_FIT_PANEL_RESIZE
    u4OutWidth = u4Width * ScaleUpMutiple;
    u4OutHeight = u4Height * ScaleUpMutiple;
#else
    if(u4PanelWidth!=u4PerfactWidth)
    {
        u4OutWidth = (u4Width * u4PanelWidth)/u4PerfactWidth;
        u4OutHeight = (u4Height * u4PanelHeight)/u4PerfactHeight;
    }
    else
    {
        u4OutWidth=u4Width;
        u4OutHeight=u4Height;
    }
#endif    
    u4OutX = ((u4PanelWidth > u4OutWidth))? ((u4PanelWidth - u4OutWidth)>> 1):0;
    u4OutY = ((u4PanelHeight > u4OutHeight))? ((u4PanelHeight - u4OutHeight)>> 1):0;
    Printf("u4OutWidth=%d,u4OutHeight=%d,u4OutX=%d,u4OutY=%d.\n",
            u4OutWidth,u4OutHeight,u4OutX,u4OutY);
  #ifdef CC_LOADER_LOGO_LONG_TIME
  if((u4OutWidth < u4PanelWidth) && (u4OutHeight < u4PanelHeight))  //for using region position setting, but not use plane position setting(else, will set plane  position again when fb init)
    {
     ret = OSD_SC_Scale(u4Scaler, 1, u4PanelWidth/ScaleUpMutiple, u4PanelHeight/ScaleUpMutiple, u4PanelWidth,
            u4PanelHeight);
   }
   else
   {
     ret = OSD_SC_Scale(u4Scaler, 1, u4Width, u4Height, u4OutWidth,
            u4OutHeight);
   }
   
    if (ret != OSD_RET_OK) return 5;
    ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X,u4OutX);
    if (ret != OSD_RET_OK)
        return 6;
    ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y,u4OutY);
    if (ret != OSD_RET_OK)
        return 7;
   // OSD_BASE_SetOsdPosition(u4OsdPlane,u4OutX,u4OutY);
   _OSD_RGN_GetAddress((UINT32)u4Region, &u4FirstRegionAddr);
    memcpy((void*)u4HaderAddr, (void*)u4FirstRegionAddr, sizeof(OSD_RGN_UNION_T));
   _OSD_PLA_SetHeaderAddr(OSD_PLANE_2, u4HaderAddr);
   
 #else
    ret = OSD_SC_Scale(u4Scaler, 1, u4Width, u4Height, u4OutWidth,
            u4OutHeight);
    if (ret != OSD_RET_OK) return 5;
    ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X,0);
    if (ret != OSD_RET_OK)
        return 6;
    ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y,0);
    if (ret != OSD_RET_OK)
        return 7;
    OSD_BASE_SetOsdPosition(u4OsdPlane,u4OutX,u4OutY);
    ret = OSD_PLA_FlipTo(u4OsdPlane, u4RegionList);
    if (ret != OSD_RET_OK) return 4;
 #endif
    PMX_SetPlaneOrderArray(_au4PlaneOrderArray);
    HalFlushDCache();
    
    ret = OSD_PLA_Enable(u4OsdPlane, TRUE);
    if (ret != OSD_RET_OK) return 8;
    IO_WRITE32MSK(OSD_BASE_MONITOR, 0, ui4OSDMonitorReg, UPDATE_TIMING_BIT);
    OSD_UpdateReg();
    return 0;
}

#ifdef CC_UPGRADING_DYNAMIC_LOGO_SUPPORT
UINT32 LDR_PatternToggleInit(UINT32 u4ColorMode, UINT32 u4Width, UINT32 u4Height, UINT32 u4LogoNum)
{
    UINT32 u4BmpPitch;
	UINT32 u4BmpAddr;
    UINT32 u4PanelWidth, u4PanelHeight;
    UINT32 ui4OSDMonitorReg = 0;
	UINT32 puDstAddress = TOTAL_MEM_SIZE - FBM_MEM_CFG_SIZE;
    INT32 ret;
    #ifdef CC_FLIP_MIRROR_SUPPORT
    UINT8 u1Config;
    #endif

    Printf("Color:%d BmpAddr:0x%08x Width:%d Height:%d\n", u4ColorMode, u4BmpAddr, u4Width, u4Height);
    OSD_Init();
    ui4OSDMonitorReg = IO_READ32MSK(OSD_BASE_MONITOR, 0, UPDATE_TIMING_BIT);
    IO_WRITE32MSK(OSD_BASE_MONITOR, 0, UPDATE_TIMING_BIT, UPDATE_TIMING_BIT);
    OSD_UpdateReg();

    u4BmpPitch = 0;
	u4BmpAddr = rDynamicLogo.u4BmpAddr[u4LogoNum];
    puDstAddress = puDstAddress + u4LogoNum*0x800000;	
	if(u4Width == 1366)
	{
		OSD_GET_PITCH_SIZE(u4ColorMode, u4Width+2, u4BmpPitch); // to set u4BmpPitch by u4ColorMode and u4Width.
	}
	else
	{
		OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch);
	}
    //copy bmp to DstAddress
    x_memcpy((void*)puDstAddress, (void*)u4BmpAddr, u4BmpPitch*u4Height);
    rDynamicLogo.u4DstAddr[u4LogoNum] = puDstAddress; 
    ret = OSD_RGN_LIST_Create(&rDynamicLogo.u4RegionList[u4LogoNum]);
    if (ret != OSD_RET_OK) return 1;

			
    Printf("\n*************u4BmpPitch=%d******************\n",u4BmpPitch);

    OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch); // to set u4BmpPitch by u4ColorMode and u4Width.
    u4BmpPitch = (u4BmpPitch +0xF) & (~0xF);

    ret = OSD_RGN_Create(&rDynamicLogo.u4Region[u4LogoNum], u4Width, u4Height, (void *)PHYSICAL(puDstAddress),
                            u4ColorMode, u4BmpPitch, 0, 0, u4Width, u4Height);
    if (ret != OSD_RET_OK) return 2;
	
    #ifdef CC_FLIP_MIRROR_SUPPORT
    u1Config= u1GetFlipMirrorConfig();
    if (u1Config & SYS_MIRROR_CONFIG_ON) {
        ret = OSD_RGN_Set(rDynamicLogo.u4Region[u4LogoNum],OSD_RGN_H_MIRROR,1);
    }
    if (u1Config & SYS_FLIP_CONFIG_ON) {
        ret = OSD_RGN_Set(rDynamicLogo.u4Region[u4LogoNum],OSD_RGN_V_FLIP,1);
    }
    ret = _OSD_BASE_UpdateHwReg();
    #endif

    ret = OSD_RGN_Insert(rDynamicLogo.u4Region[u4LogoNum], rDynamicLogo.u4RegionList[u4LogoNum]);
    if (ret != OSD_RET_OK) return 3;

    return 0;
}

UINT32 LDR_PatternToggleDisplay(UINT32 u4ColorMode, UINT32 u4Width, UINT32 u4Height,
                                      UINT32 ScaleUpMutiple, UINT32 u4LogoNum)
{
    UINT32 u4PanelWidth, u4PanelHeight;
    UINT32 u4OutWidth, u4OutHeight, u4OutX, u4OutY;
    UINT32 u4OsdPlane = OSD_PLANE_2;
    UINT32 u4Scaler = OSD_SCALER_2;
    UINT32 ui4OSDMonitorReg = 0;
    INT32 ret;
	UINT32 _au4PlaneOrderArray[PMX_MAX_INPORT_NS] =    //need keep the same with fb init setting
	{
#ifdef ANDROID
	    PMX_OSD3,
#else
        PMX_OSD2,
#endif
#ifdef ANDROID
	    PMX_OSD2,
#else
        PMX_OSD3,
#endif
        PMX_OSD1, 
        PMX_PIP, 
        PMX_MAIN 
    };

    u4PanelWidth = PANEL_GetPanelWidth();
    u4PanelHeight = PANEL_GetPanelHeight();

    //Printf("Panel %d x %d \n", u4PanelWidth, u4PanelHeight);
    //Printf("LDR_PatternToggleDisplay:BmpAddr:0x%08x / DstAddr:0x%08x\n", 
	//	    rDynamicLogo.u4BmpAddr[u4LogoNum],rDynamicLogo.u4DstAddr[u4LogoNum]);

    ret = OSD_PLA_FlipTo(OSD_PLANE_2, rDynamicLogo.u4RegionList[u4LogoNum]);
    if (ret != OSD_RET_OK) return 4;

    u4OutWidth = u4Width * ScaleUpMutiple;
    u4OutHeight = u4Height * ScaleUpMutiple;
   
    u4OutX = ((u4PanelWidth > u4OutWidth))? ((u4PanelWidth - u4OutWidth)>> 1):0;
    u4OutY = ((u4PanelHeight > u4OutHeight))? ((u4PanelHeight - u4OutHeight)>> 1):0;
    //Printf("u4OutWidth=%d,u4OutHeight=%d,u4OutX=%d,u4OutY=%d.\n",
    //          u4OutWidth,u4OutHeight,u4OutX,u4OutY);

    ret = OSD_SC_Scale(u4Scaler, 1, u4Width, u4Height, u4OutWidth,
              u4OutHeight);
    if (ret != OSD_RET_OK) return 5;
    ret = OSD_RGN_Set(rDynamicLogo.u4Region[u4LogoNum], OSD_RGN_POS_X,0);
    if (ret != OSD_RET_OK)
          return 6;
    ret = OSD_RGN_Set(rDynamicLogo.u4Region[u4LogoNum], OSD_RGN_POS_Y,0);
    if (ret != OSD_RET_OK)
          return 7;
    OSD_BASE_SetOsdPosition(u4OsdPlane,u4OutX,u4OutY);
    ret = OSD_PLA_FlipTo(u4OsdPlane, rDynamicLogo.u4RegionList[u4LogoNum]);
    if (ret != OSD_RET_OK) return 4;

    PMX_SetPlaneOrderArray(_au4PlaneOrderArray);
    HalFlushDCache();
    ret = OSD_PLA_Enable(u4OsdPlane, TRUE);
    if (ret != OSD_RET_OK) return 8;
    IO_WRITE32MSK(OSD_BASE_MONITOR, 0, ui4OSDMonitorReg, UPDATE_TIMING_BIT);
    OSD_UpdateReg();
    return 0;
}
#endif

//#endif /* LOADER_LOGO_NUMBER */

//-----------------------------------------------------------------------------
/** LDR_LogoDisplay(): To display logo with panel index.
 *  This function is going to display logo at boot loader.
 *
 *  @param  u4BmpAddr       The bmp data pointer.
 *  @param  u4Width         The width info of the bmp data.
 *  @param  u4Height        The height info of the bmp data.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 LDR_LogoDisplay(LDR_DATA_T *prLdrData, UINT32 u4Idx)
{
#if LOADER_LOGO_NUMBER > 0
    UINT32 u4Ret;

    if (u4Idx >= LOADER_LOGO_NUMBER)
    {
        Printf("no such index %d\n", u4Idx);
        return 0;
    }

    if (DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + u4Idx))==0)
    {
        return 0;
    }
    #ifdef CC_QUIET_HOT_BOOT_SUPPORT
    if (prLdrData->rDtvCfg.u1Flags4 & DTVCFG_FLAG4_QUIET_HOT_BOOT)
    {
        Printf("Quiet Hot Boot\n");
        return 0;
    }
    #endif

    Printf("Display 0x%08x Idx:%d BG:0x%08x\n", prLdrData, u4Idx, DRVCUST_InitGet(eLoaderMsgBackground));
    LDR_PmxDisplay(DRVCUST_InitGet(eLoaderLogoBackground));

    u4Ret = LDR_OsdDisplay(DRVCUST_InitGet(eLoaderLogoColorMode),
                    DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + u4Idx)),
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoWidthArray))[u4Idx],
#ifndef CC_LOGO_FIT_PANEL_RESIZE 
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoHightArray))[u4Idx],1);
#else
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoHightArray))[u4Idx],960,540);
#endif
    Printf("LDR_OsdDisplay(%d, 0x%08x, %d, %d) return %d\n",
                    DRVCUST_InitGet(eLoaderLogoColorMode),
                    DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + u4Idx)),
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoWidthArray))[u4Idx],
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoHightArray))[u4Idx], u4Ret);
    return u4Ret;
#else /* LOADER_LOGO_NUMBER > 0 */
    return 0;
#endif /* LOADER_LOGO_NUMBER > 0 */
}

#ifdef CC_UPGRADING_DYNAMIC_LOGO_SUPPORT
UINT32 LDR_DynamicLogoInit(LDR_DATA_T *prLdrData)
{
#if LOADER_LOGO_NUMBER > 0
    UINT32 u4Ret;
    UINT32 u4LogoNum;
	
    Printf("Init 0x%08x BG:0x%08x\n", prLdrData, DRVCUST_InitGet(eLoaderMsgBackground));
    LDR_PmxDisplay(DRVCUST_InitGet(eLoaderLogoBackground));
	rDynamicLogo.u4BmpAddr[0] = DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + 1));
	rDynamicLogo.u4BmpAddr[1] = DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + 2));
	for(u4LogoNum=0;u4LogoNum<2;u4LogoNum++)
	{
    u4Ret = LDR_PatternToggleInit(DRVCUST_InitGet(eLoaderLogoColorMode),
		                          ((UINT32 *)DRVCUST_InitGet(eLoaderLogoWidthArray))[u4LogoNum+1],
		                          ((UINT32 *)DRVCUST_InitGet(eLoaderLogoHightArray))[u4LogoNum+1],u4LogoNum);
	
	}
    return u4Ret;
#else /* LOADER_LOGO_NUMBER > 0 */
    return 0;
#endif /* LOADER_LOGO_NUMBER > 0 */
}

UINT32 LDR_DynamicLogoDisplay(LDR_DATA_T *prLdrData, UINT32 u4Idx)
{
#if LOADER_LOGO_NUMBER > 0
    UINT32 u4Ret;
	
    if (u4Idx >= LOADER_LOGO_NUMBER)
    {
        Printf("no such index %d\n", u4Idx);
        return 0;
    }

    if (DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + u4Idx + 1))==0)
    {
        return 0;
    }

    //Printf("Display 0x%08x Idx:%d BG:0x%08x\n", prLdrData, u4Idx, DRVCUST_InitGet(eLoaderMsgBackground));
    u4Ret = LDR_PatternToggleDisplay(DRVCUST_InitGet(eLoaderLogoColorMode),
		                             ((UINT32 *)DRVCUST_InitGet(eLoaderLogoWidthArray))[u4Idx+1],
                                     ((UINT32 *)DRVCUST_InitGet(eLoaderLogoHightArray))[u4Idx+1],1,u4Idx);
    Printf("LDR_OsdDisplay(%d, 0x%08x, %d, %d) return %d\n",
                    DRVCUST_InitGet(eLoaderLogoColorMode),
                    DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + u4Idx + 1)),
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoWidthArray))[u4Idx+1],
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoHightArray))[u4Idx+1], u4Ret);
    return u4Ret;
#else /* LOADER_LOGO_NUMBER > 0 */
    return 0;
#endif /* LOADER_LOGO_NUMBER > 0 */
}
#endif

//-----------------------------------------------------------------------------
/** LDR_PanelLogo(): display logo on panel.
 *  This function is going to find out the logo location and call lvds/pmx/osd 
 *  low level driver to display logo on panel.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
#if LOADER_MULTIPLE_LOGO_PARTITION
EXTERN INT32 Loader_ReadNandFlash(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen);
VOID  jpeg_decode(UINT32 u4Buffer,UINT32 u4size,UINT32 u4WorkBuffer,UINT32 u4Worksize,UINT32 u4DstAddr,UINT32 u4Width,UINT32 u4Height);
#endif

UINT32 LDR_PanelLogo(LDR_DATA_T *prLdrData)
{
    UINT32 u4Ret;
#ifdef CC_MAPLE_BE_LOADER_BOOT_FLOW
    BOOL fgBeExist=FALSE;
#endif

    // customization show loader logo or not
#if defined(DEFINED_PFN_LDR_PanelLogoCheck_FUNC)
    PFN_LDR_PanelLogoCheck_FUNC pfnLdrCheckLogo = NULL;
    pfnLdrCheckLogo = (PFN_LDR_PanelLogoCheck_FUNC)DRVCUST_InitGet(eLoaderPanelLogoCheckFunc);
    if (pfnLdrCheckLogo != NULL)
    {
        if (pfnLdrCheckLogo()) //don't show logo
        {
            return 0;
        }
    }
#endif

#if LOADER_LED_TWINKLE
    Printf("Loader_BootLED: PDWNC_BASE=0x%08x\n", CKGEN_BASE);
    u4Ret = IO_READ32(PDWNC_BASE, 0x0b4);
    IO_WRITE32(PDWNC_BASE, 0x0b4, u4Ret | 0x40000);
    u4Ret = (0x6c000 / 1000) * LOADER_LED_TWINKLE;
    IO_WRITE32(PDWNC_BASE, 0x150, 0xf0000001 | (u4Ret << 4));
#endif

#ifdef CC_MAPLE_BE_LOADER_BOOT_FLOW
    fgBeExist = LDR_BE_Support();
    if(fgBeExist)
    {
      LDR_BEBootVerify(prLdrData);
      Printf("LDR_PanelLogo: Display by 4KBE!!\n");
	  return 0;
    }
#endif

#if LOADER_MULTIPLE_LOGO_PARTITION

#define JPEG_BUFFER_SIZE 0x00100000
#define LOADER_LOGO_OFFSET    0
    UINT8 *puJpgBuf = (UINT8 *)0x0D000000;
    UINT8 *puJpgWorkBuf = puJpgBuf + 0x01000000; // JPEG_BUFFER_SIZE;
    UINT8 *puJpgDestBuf = puJpgWorkBuf + 0x01000000; //JPEG_BUFFER_SIZE;
    UINT32 w, h;

    u4Ret = Loader_ReadNandFlash(LOADER_MULTIPLE_LOGO_PARTITION, LOADER_LOGO_OFFSET, puJpgBuf, JPEG_BUFFER_SIZE);
    Printf("Loader_ReadNandFlash(%d, %d, 0x%08x, %d)\n", LOADER_MULTIPLE_LOGO_PARTITION, LOADER_LOGO_OFFSET, puJpgBuf, JPEG_BUFFER_SIZE);
    LDR_PmxDisplay(DRVCUST_InitGet(eLoaderLogoBackground));
    w = PANEL_GetPanelWidth();
    h = PANEL_GetPanelHeight();

    jpeg_decode((UINT32)puJpgBuf, JPEG_BUFFER_SIZE, (UINT32)puJpgWorkBuf, JPEG_BUFFER_SIZE, (UINT32)puJpgDestBuf, w, h);
    Printf("jpeg_decode(0x%08x, %d, 0x%08x, %d, 0x%08x, %d, %d)\n",
                    puJpgBuf, JPEG_BUFFER_SIZE, puJpgWorkBuf, JPEG_BUFFER_SIZE, puJpgDestBuf, w, h);
#ifndef CC_LOGO_FIT_PANEL_RESIZE
    u4Ret = LDR_OsdDisplay(OSD_CM_ARGB8888_DIRECT32, (UINT32) puJpgDestBuf, w, h,1);
#else
    u4Ret = LDR_OsdDisplay(OSD_CM_ARGB8888_DIRECT32, (UINT32) puJpgDestBuf, w, h,960,540);
#endif
    Printf("LDR_OsdDisplay(%d, 0x%08x, %d, %d) return %d\n",
                    OSD_CM_ARGB8888_DIRECT32, (UINT32) puJpgDestBuf, w, h, u4Ret);
    return u4Ret;

#elif LOADER_LOGO_NUMBER
    UINT32 puDstAddress = TOTAL_MEM_SIZE - FBM_MEM_CFG_SIZE;
    UINT32 u4BmpAddress=0;
    UINT32 u4ColorMode=0;
    UINT32 u4BmpWidth=0;
    UINT32 u4BmpHeight=0;
    UINT32 u4BmpPitch=0;

	#ifdef DRV_SUPPORT_MT8283
	UINT8 u1RetErr;
	#endif
  
    if (DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + 0))==0)
    {
        return 0;
    }

    // Check quiet boot flag and RTC at the same time.
    #ifdef CC_SUPPORT_ANDROID_L_RTC_WAKEUP
    if ((prLdrData->rDtvCfg.u1Flags2 & DTVCFG_FLAG2_QUIET_BOOT) || (PDWNC_ReadWakeupReason() == PDWNC_WAKE_UP_REASON_RTC))
	#else
    if ((prLdrData->rDtvCfg.u1Flags2 & DTVCFG_FLAG2_QUIET_BOOT) && (PDWNC_ReadWakeupReason() == PDWNC_WAKE_UP_REASON_RTC))
	#endif
    {
        Printf("Quiet Boot\n");
        return 0;
    }
    #ifdef CC_QUIET_HOT_BOOT_SUPPORT
    if (prLdrData->rDtvCfg.u1Flags4 & DTVCFG_FLAG4_QUIET_HOT_BOOT)
    {
        Printf("Quiet Hot Boot\n");
        return 0;
    }
    #endif    
    if (prLdrData->rDtvCfg.u1Flags2 & DTVCFG_FLAG2_DISABLE_LOGO)
    {
	    Printf("AP force LOGO disabled\n");
	    return 0;
    }
    u4BmpAddress=DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + 0));
    u4ColorMode=DRVCUST_InitGet(eLoaderLogoColorMode);
    u4BmpWidth=((UINT32 *)DRVCUST_InitGet(eLoaderLogoWidthArray))[0];
    u4BmpHeight=((UINT32 *)DRVCUST_InitGet(eLoaderLogoHightArray))[0];
    OSD_GET_PITCH_SIZE(u4ColorMode, u4BmpWidth, u4BmpPitch);
    
    //copy bmp to DstAddress
    x_memcpy((void*)puDstAddress, (void*)u4BmpAddress, u4BmpPitch*u4BmpHeight);
    Printf("Display 0x%08x background:0x%08x\n", prLdrData, DRVCUST_InitGet(eLoaderLogoBackground));
    LDR_PmxDisplay(DRVCUST_InitGet(eLoaderLogoBackground));
   
    #ifdef DRV_SUPPORT_MT8283
    x_thread_delay(3000);
    drv_extmjc_set_video_fmt(0,60,2200,1125,0,1920,1080,&u1RetErr);
    drv_extmjc_set_Mode_Change(&u1RetErr);
    //x_thread_delay(1000);
    #endif
    
    Printf("LDR_OsdDisplay(%d, 0x%08x, %d, %d)\n",u4ColorMode,puDstAddress,u4BmpWidth,u4BmpHeight);
#ifndef CC_LOGO_FIT_PANEL_RESIZE
    u4Ret = LDR_OsdDisplay(u4ColorMode,puDstAddress,u4BmpWidth,u4BmpHeight,1);
#else
    u4Ret = LDR_OsdDisplay(u4ColorMode,puDstAddress,u4BmpWidth,u4BmpHeight,960,540);
#endif
    #ifdef DRV_SUPPORT_MT8283
    x_thread_delay(1000);
    #endif
    return u4Ret;
#else /* LOADER_LOGO_NUMBER */
    UNUSED(u4Ret);
    return 0;
#endif /* LOADER_LOGO_NUMBER */
}

UINT32 LDR_BEPatternDisplay(UINT32 u4FrameIdx, UINT32 u4Offset)
{
    UINT32 u4Ret=0;
	#ifdef CC_MAPLE_BE_LOADER_BOOT_FLOW
	LDR_PmxDisplay(DRVCUST_InitGet(eLoaderLogoBackground));
	#if defined(CC_MT5890) && defined(SUPPORT_PANEL_GRAPH_MODE)
	if (IS_SUPPORT_VB1_GRAPH_MODE)
	{
		//set OSD lane to 4K1K 60Hz
		Printf("LDR_BEPatternDisplay Set to 4K1K60Hz!!");
		GraphMode_SetPanelType(PANEL_TYPE_4K1K_60);
		vDrvSetGFXTiming(SV_DCLK_60HZ);
	}
    #endif
	//	
    LDR_BEFrameDisplay(u4FrameIdx,u4Offset);
	#else
    Printf("LDR_BEPatternDisplay: Not support !!\n");
	#endif
    return u4Ret;
}
UINT32 LDR_BENotifySetup(LDR_DATA_T *prLdrData)
{
    UINT32 u4Ret=0;
	#ifdef CC_MAPLE_BE_LOADER_BOOT_FLOW
    LDR_BE_Init(prLdrData);
	#else
    Printf("LDR_BENotifySetup: Not support !!\n");
    #endif
    return u4Ret;
}
UINT32 LDR_BEBootVerify(LDR_DATA_T *prLdrData)
{
    UINT32 u4Ret=0;
    #ifdef CC_MAPLE_BE_LOADER_BOOT_FLOW
	LDR_BEBootSeq(prLdrData);
	#else
    Printf("LDR_BEBootSeq: Not support !!\n");
	#endif
    return u4Ret;
}

BOOL LDR_BECheckDDRI(VOID)
{
    BOOL fgSupport=FALSE;
    #ifdef CC_MAPLE_BE_LOADER_BOOT_FLOW
	fgSupport = LDR_BE_Support();
	#else
    Printf("LDR_BECheckDDRI: Not support !!\n");
	#endif
    return fgSupport;
}

UINT32 LDR_BEUpgReboot(LDR_DATA_T *prLdrData)
{
    UINT32 ui4BootStatus=0;
    #ifdef CC_MAPLE_BE_LOADER_BOOT_FLOW
	LDR_BE_Reset();
	LDR_BEBootSeq(prLdrData);
	ui4BootStatus = prLdrData->u4LdrFlags;
	ui4BootStatus = ui4BootStatus & 0x00000003;
	if((ui4BootStatus & LDRFLAG_BEBOOT_FAIL) == 0)
	{
	    LDR_BE_Erase_Del_Partition();
		Printf("LDR_BEUpgReboot: BE Boot erase delet type partition!!\n");
	}
	else
	{
	    Printf("LDR_BEUpgReboot: BE Boot Fail after upgrade!!\n");
	}
	#else
    Printf("LDR_BEUpgReboot: Not support !!\n");
	#endif
    return ui4BootStatus;
}


