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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: nptv_scpip_gfx_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/
#include "drv_scaler_drvif.h"

#ifdef SCPIP_SUPPORT_GFX_DUMP
#include "x_os.h"
#include "x_stl_lib.h"

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"
#include "x_drv_cli.h"
#include "vdo_rm.h"
#include "drv_scaler_gfx.h"

#ifndef CC_CLI
#define CC_CLI
#endif
static INT32 _ScpipGfxInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxSourceSel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxUpdateResolution(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxSetFrameNum(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxSetDramAddress(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxSetWriteMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxWriteTrigger(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxWriteEnable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfx3DTo2DEnable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxSetFrameDrop(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxSetDumpFormat(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxSetAlpha(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxDumpSetting(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipGfxStatusQuery(INT32 i4Argc, const CHAR **szArgv);



/******************************************************************************
* CLI Command Table
******************************************************************************/

CLIMOD_DEBUG_FUNCTIONS(GFX)

// Table for scpos command entry
CLI_EXEC_T arScpipGfxDumpCmdTbl[] = {
  {"init", "i", _ScpipGfxInit, NULL, "Initialization", CLI_SUPERVISOR},   
  {"source", "src", _ScpipGfxSourceSel, NULL, "Select dump path(0:main, 1:sub)", CLI_SUPERVISOR},   
  {"resolution", "res", _ScpipGfxUpdateResolution, NULL, "Update resolution(width, height)", CLI_SUPERVISOR},   
  {"framenum", "fn", _ScpipGfxSetFrameNum, NULL, "Set frame number(1~4)", CLI_SUPERVISOR},   
  {"address", "add", _ScpipGfxSetDramAddress, NULL, "Set dram address, frame size, frame number", CLI_SUPERVISOR},   
  {"writemode", "wm", _ScpipGfxSetWriteMode, NULL, "Set dram write mode(0: continue, 1:trigger)", CLI_SUPERVISOR},   
  {"trigger", "t", _ScpipGfxWriteTrigger, NULL, "Trigger dram write 1 frame", CLI_GUEST},   
  {"writeenable", "wen", _ScpipGfxWriteEnable, NULL, "Switch dram write OnOff", CLI_GUEST},   
  {"3DTo2D", "32", _ScpipGfx3DTo2DEnable, NULL, "Enable 3D to 2D", CLI_SUPERVISOR},   
  {"framedrop", "fd", _ScpipGfxSetFrameDrop, NULL, "Input frame drop N/M", CLI_GUEST},   
  {"format", "f", _ScpipGfxSetDumpFormat, NULL, "Set dump format(0:RGBA888, 1:RGB565)", CLI_GUEST},   
  {"alpah", "a", _ScpipGfxSetAlpha, NULL, "Set alpha value(0~127)", CLI_SUPERVISOR},   
  {"dumpseting", "d", _ScpipGfxDumpSetting, NULL, "gfx dump setting", CLI_SUPERVISOR},  
  {"query", "q", _ScpipGfxStatusQuery, NULL, "Query dump status", CLI_GUEST},   

  CLIMOD_DEBUG_CLIENTRY(GFX),
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _ScpipGfxInit(INT32 i4Argc, const CHAR **szArgv)
{
	vScpip_GFX_init();
	return 0;
}

static INT32 _ScpipGfxSourceSel(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    
    if (i4Argc < 2)
    {
		if ( IS_IC_5861() )       
	        Printf("Usage: <Main_FSC:0/Sub_FSC:1/Main_MIB:2/Sub_MIB:3/Main_PreProc:4/Sub_PreProc:5/TVE:6/Hybrid(4K input):7>\n");
		else
        	Printf("Usage: <Main_FSC:0/Sub_FSC:1/Main_MIB:2/Sub_MIB:3/Main_PreProc:4/Sub_PreProc:5/TVE:6>\n");
        return 0;
    }

    bPath = (UINT8)StrToInt(szArgv[1]);
    if(IS_IC_5861()&& bPath > 7)
    {
    	Printf("Usage: <Main_FSC:0/Sub_FSC:1/Main_MIB:2/Sub_MIB:3/Main_PreProc:4/Sub_PreProc:5/TVE:6/Hybrid(4K input):7>\n");
    	return 0;
    }
	else if(bPath >6)
    {
        Printf("Usage: <Main_FSC:0/Sub_FSC:1/Main_MIB:2/Sub_MIB:3/Main_PreProc:4/Sub_PreProc:5/TVE:6>\n");
        return 0;
    }
    u1Scpip_GFX_Source_Sel(bPath);    
    printf("[SCPIP_GFX]Source change to path[%d]\n", (INT32)bPath);
    
    return 0;
}

static INT32 _ScpipGfxUpdateResolution(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Width, u4Height;
    
    if (i4Argc < 3)
    {
        Printf("Usage: <width> <height>\n");
        return 0;
    }

    u4Width  = (UINT32)StrToInt(szArgv[1]);
    u4Height = (UINT32)StrToInt(szArgv[2]);

    u1Scpip_GFX_Update_Resolution(u4Width, u4Height);
    
    printf("[SCPIP_GFX]Resolution change to (%d x %d)\n", u4Width, u4Height);
    
    return 0;
}

static INT32 _ScpipGfxSetFrameNum(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Number;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <FramNum(1~4)>\n");
        return 0;
    }

    u1Number = (UINT8)StrToInt(szArgv[1]);

     // 1~4 frames
    if(u1Number >0 && u1Number<5)
    {
    	u1Scpip_GFX_Set_Frame_Num(u1Number);
    	printf("[SCPIP_GFX]Set Frame Number to [%d]\n", (INT32)u1Number);
    }
    else
    {
    	Printf("Usage: <FramNum(1~4)>\n");
    }
    
    return 0;
}

static INT32 _ScpipGfxSetDramAddress(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4BaseAddr;
    UINT32 u4FrameSize;
    UINT8 u1FrameNum;
    
    if (i4Argc < 4)
    {
        Printf("Usage: <BaseAddress  FrameSize u1FrameNum>\n");
        return 0;
    }

    u4BaseAddr = (UINT32)StrToInt(szArgv[1]);
    u4FrameSize= (UINT32)StrToInt(szArgv[2]);
    u1FrameNum = (UINT8)StrToInt(szArgv[3]);

    u1Scpip_GFX_Set_Dram_Address(u4BaseAddr, u4FrameSize, u1FrameNum);
    printf("[SCPIP_GFX]Set dump dram address[0x%x], Size[0x%x]\n", u4BaseAddr, u4FrameSize, u1FrameNum);

    return 0;
}

static INT32 _ScpipGfxSetWriteMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Select;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <u1Select(0:continuous mode ,  1: trigger mode>\n");
        return 0;
    }

    u1Select = (UINT8)StrToInt(szArgv[1]);

    u1Scpip_GFX_Set_Write_Mode(u1Select);
    printf("[SCPIP_GFX]Set dram write mode %d(0:continuous mode ,  1: trigger mode)\n", u1Select);

    return 0;
}

static INT32 _ScpipGfxWriteTrigger(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Result;
    u1Result = u1Scpip_GFX_Write_Trigger();

    if(u1Result ==FALSE)
    {
    	printf("[SCPIP_GFX] Dram write  trigger FAIL, Current write mode is not trigger mode\n");
    }
    else
    {
    	printf("[SCPIP_GFX] Dram write trigger OK\n");
    }
    return 0;
}

static INT32 _ScpipGfxWriteEnable(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Enable;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <u1Enable(0:off, 1:on)>\n");
        return 0;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    u1Scpip_GFX_Write_Enable(u1Enable);
    printf("[SCPIP_GFX] Write enable(%d)\n", u1Enable);

    return 0;

}

static INT32 _ScpipGfx3DTo2DEnable(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1VdpId, u1Enable;
    
    if (i4Argc < 3)
    {
        Printf("Usage: <u1VdpId u1Enable(0:off, 1:on)>\n");
        return 0;
    }
    u1VdpId  = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    u1Scpip_GFX_Set_3DTo2D_En(u1VdpId, u1Enable);
    printf("[SCPIP_GFX] Write(%d) enable(%d)\n", u1VdpId, u1Enable);

    return 0;
}
	
static INT32 _ScpipGfxSetFrameDrop(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1VdpId, u1Enable;
    UINT8 u1DropM, u1DropN;
    
    if (i4Argc < 5)
    {
        Printf("Usage: <u1VdpId u1Enable u1DropM u1DropN>\n");
        Printf("[SCPIP_GFX] u1DropM and u1DropN must between 1~0xff\n");
        Printf("[SCPIP_GFX] Example: M=3, N=1 that means 1/3 data enable. Every 3 frames drop 2.\n");
        return 0;
    }
    u1VdpId  = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);
    u1DropM  = (UINT8)StrToInt(szArgv[3]);
    u1DropN  = (UINT8)StrToInt(szArgv[4]);

    if(u1DropM > 0xff || u1DropN > 0xff)
    {
    	Printf("[SCPIP_GFX] u1DropM and u1DropN must between 1~0xff\n");
    }
    else
    {
    	u1Scpip_GFX_Set_Frame_Drop(u1VdpId, u1Enable, u1DropM, u1DropN);
    	printf("[SCPIP_GFX] Frame drop(%d) enable(%d), M(%d), N(%d)\n", u1VdpId, u1Enable, u1DropM, u1DropN);
    }
    return 0;

}
	
static INT32 _ScpipGfxSetDumpFormat(INT32 i4Argc, const CHAR **szArgv)
{
    SCPIP_GFX_FORMAT Format;

    if (i4Argc < 2)
    {
        Printf("Usage: <Format>(0:RGBA8888, 1:RGB565, 2:RGBA2101010, 3:YUVA2101010)\n");
        return 0;
    }

    Format = (SCPIP_GFX_FORMAT)StrToInt(szArgv[1]);

    u1Scpip_GFX_Set_Dump_Format(Format);
    printf("[SCPIP_GFX] Dump Format(%d),(0:RGBA8888, 1:RGB565,2:RGBA2101010, 3:YUVA2101010)\n", Format);
    
    return 0;
}

static INT32 _ScpipGfxSetAlpha(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Alpha;
    
    if (i4Argc < 2)
    {
        Printf("Usage: u1Alpha(0~255)\n");
        return 0;
    }

    u1Alpha = (UINT8)StrToInt(szArgv[1]);

    u1Scpip_GFX_Set_Alpha(u1Alpha);
    printf("[SCPIP_GFX] Alpha value(%d)\n", u1Alpha);

    return 0;

}

static INT32 _ScpipGfxDumpSetting(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 bPath;
	SCPIP_GFX_FORMAT Format;
	UINT32 u4BaseAddr;
    UINT32 u4FrameSize;
    UINT8 u1FrameNum;
	
	if (i4Argc != 6)
    {
        Printf("Usage: path/ format/ baseaddress/ framesize/ framenumber\n");
        return 0;
    }

	bPath  =     (UINT8)StrToInt(szArgv[1]);
    Format =     (SCPIP_GFX_FORMAT)StrToInt(szArgv[2]);
    u4BaseAddr = (UINT32)StrToInt(szArgv[3]);
    u4FrameSize= (UINT32)StrToInt(szArgv[4]);
    u1FrameNum = (UINT8)StrToInt(szArgv[5]);

	vDrvVrmSetAppFlag(VRM_APP_GRAPHIC_DUMP);
	vScpip_GFX_init();
	u1Scpip_GFX_Set_Alpha(0xff);
	u1Scpip_GFX_Set_Dump_Format(Format);
	u1Scpip_GFX_Set_Dram_Address(u4BaseAddr, u4FrameSize, u1FrameNum);
	u1Scpip_GFX_Source_Sel(bPath);
	u1Scpip_GFX_Set_Write_Mode(1);
	u1Scpip_GFX_Write_Enable(1);

	return 1;
	
}

	
static INT32 _ScpipGfxStatusQuery(INT32 i4Argc, const CHAR **szArgv)
{
	vScpip_GFX_Query();
	return 0;
}


#endif //#ifdef CC_CLI
#endif // #ifdef SCPIP_SUPPORT_GFX_DUMP

