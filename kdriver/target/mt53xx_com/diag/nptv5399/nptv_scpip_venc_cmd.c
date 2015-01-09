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
 * $RCSfile: $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/
#include "drv_scaler_drvif.h"

#ifdef CC_SUPPORT_VENC
#include "x_os.h"
#include "x_stl_lib.h"

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"
#include "x_drv_cli.h"
#include "drv_scaler.h"


//#include "drv_scaler_venc.h"

#ifndef CC_CLI
#define CC_CLI
#endif
static INT32 _ScpipVencInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencMux(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencFldMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencInput(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencSourceSel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencUpdateResolution(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencSetDramAddress(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencSetWriteReadyMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencWriteTrigger(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencWriteEnable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencSetFrameDrop(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencStatusQuery(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencDumpDram(INT32 i4Argc, const CHAR **szArgv);
#if 0
static INT32 _ScpipVencPairInv(INT32 i4Argc, const CHAR **szArgv);
static INT32 _ScpipVencAddrInv(INT32 i4Argc, const CHAR **szArgv);
#endif




/******************************************************************************
* CLI Command Table
******************************************************************************/
CLIMOD_DEBUG_FUNCTIONS(VENC)

// Table for scpos command entry
CLI_EXEC_T arScpipVencCmdTbl[] = {
  {"init", "i", _ScpipVencInit, NULL, "Initialization", CLI_SUPERVISOR},  
  {"enc mux", "em", _ScpipVencMux, NULL, "ScpipVenc Mux select", CLI_GUEST},
  {"enc fld mode ", "ef", _ScpipVencFldMode, NULL, "ScpipVenc Fld Mode", CLI_GUEST},
  {"input", "input", _ScpipVencInput, NULL, "Select input path (0:main, 1:sub)", CLI_SUPERVISOR},   
  {"source", "src", _ScpipVencSourceSel, NULL, "Select dump path (0:main, 1:sub, 2: mon_out)", CLI_SUPERVISOR},   
  {"resolution", "res", _ScpipVencUpdateResolution, NULL, "Update resolution(width, height, scan)", CLI_SUPERVISOR},   
  {"address", "add", _ScpipVencSetDramAddress, NULL, "Set dram address, frame size, frame number", CLI_SUPERVISOR},   
  {"writemode", "wm", _ScpipVencSetWriteReadyMode, NULL, "Set dram write mode(0: continue, 1:trigger)", CLI_GUEST},   
  {"trigger", "t", _ScpipVencWriteTrigger, NULL, "Trigger dram write 1 frame", CLI_GUEST},   
  {"writeenable", "wen", _ScpipVencWriteEnable, NULL, "Switch dram write OnOff", CLI_GUEST},   
  {"framedrop", "fd", _ScpipVencSetFrameDrop, NULL, "Input frame drop N/M", CLI_GUEST},   
  {"dumpdram", "dd", _ScpipVencDumpDram, NULL, "dump scaler dram one", CLI_GUEST},
  #if 0
  {"pair invert", "pi", _ScpipVencPairInv, NULL, "field pair invert", CLI_GUEST},
  {"addr invert", "ai", _ScpipVencAddrInv, NULL, "address invert", CLI_GUEST},
  #endif
  {"query", "q", _ScpipVencStatusQuery, NULL, "Query dump status", CLI_GUEST},   

  CLIMOD_DEBUG_CLIENTRY(VENC),
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _ScpipVencInit(INT32 i4Argc, const CHAR **szArgv)
{
    LOG(0, "VRM APPSET\n");
    LOG(0, "_ScpipVencInput\n");
    LOG(0, "_ScpipVencSourceSel\n");
    //vScpip_Venc_init();
    return 0;
}

static INT32 _ScpipVencMux(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Mux;
    
    if (i4Argc != 2)
    {
        Printf("Usage:\n");
        Printf("E_VDP_VENC_MIXER :            0\n");
        Printf("E_VDP_VENC_MAIN_FSC_OUT :     1\n");
        Printf("E_VDP_VENC_SUB_FSC_OUT :      2\n");
        Printf("E_VDP_VENC_MAIN_MIB_OUT :     3\n");
        Printf("E_VDP_VENC_MAIN_BEFORE_PRED : 4\n");
        Printf("E_VDP_VENC_SUB_BEFORE_PRED :  5\n");
        Printf("E_VDP_VENC_SUB_MIB_OUT :      6\n");
        Printf("E_VDP_VENC_OSTG :             7\n");
        Printf("E_VDP_VENC_OSTG_OSD :         8\n");

        return 0;
    }

    u1Mux = (UINT8)StrToInt(szArgv[1]);
    
    vDrvScpipH264SetMux(u1Mux);
    return 0;
}

static INT32 _ScpipVencFldMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1FldMode;
    
    if (i4Argc != 2)
    {
        Printf("Usage:  0/1/2/3 \n");
        Printf("MTVENC_ENC_BOTH_FLD :     0\n");
        Printf("MTVENC_ENC_TOP_FLD :      1\n");
        Printf("MTVENC_ENC_BOTTOM_FLD :   2\n");
        Printf("MTVENC_ENC_PROGRESSIVE :  3\n");

        return 0;
    }

    u1FldMode = (UINT8)StrToInt(szArgv[1]);
    
    vDrvScpipH264SetFldMode(u1FldMode);
    return 0;
}

static INT32 _ScpipVencInput(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <Main: 0 /Sub: 1>\n");
        return 0;
    }

    bPath = (UINT8)StrToInt(szArgv[1]);
    if(bPath > 2)
    {
        Printf("Usage: <Main: 0 /Sub: 1>\n\n");
        return 0;
    }

    u1DrvScpipH264Input(bPath);
    
    printf("[SCPIP_VENC]Input change to path[%d]\n", (INT32)bPath);
    
    return 0;
}

static INT32 _ScpipVencSourceSel(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <Main: 0 /Sub: 1 /Monitor out: 2>\n");
        return 0;
    }

    bPath = (UINT8)StrToInt(szArgv[1]);
    if(bPath > 3)
    {
        Printf("Usage: <Main: 0 /Sub: 1 /Monitor out: 2>\n\n");
        return 0;
    }

    u1DrvScpipH264SourceSel(bPath);
    
    printf("[SCPIP_VENC]Source change to path[%d]\n", (INT32)bPath);
    
    return 0;
}

static INT32 _ScpipVencUpdateResolution(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Width, u4Height, u4Scan;
    
    if (i4Argc < 4)
    {
        Printf("Usage: <width> <height> <scan:I/P>\n");
        return 0;
    }

    u4Width  = (UINT32)StrToInt(szArgv[1]);
    u4Height = (UINT32)StrToInt(szArgv[2]);
    u4Scan   = (UINT32)StrToInt(szArgv[3]);

    if (u4Width != u4DrvScpipH264GetWidth() || u4Height != u4DrvScpipH264GetHeight() || u4Scan != u4DrvScpipH264IsInterlace())
    {
        u1DrvScpipH264UpdateResolution(u4Width, u4Height, u4Scan);
        LOG(0, "Manual Set VENC resolution %d x %d %c\n", u4Width, u4Height, u4Scan?'I':'P');
    }
    
    //printf("[SCPIP_VENC]Resolution change to (%d x %d)\n", u2Width, u2Height);
    
    return 0;
}

static INT32 _ScpipVencSetDramAddress(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4BaseAddr;
    UINT32 u4FrameSize;
    
    if (i4Argc < 3)
    {
        Printf("Usage: <BaseAddress YFrameSize>\n");
        return 0;
    }

    u4BaseAddr = (UINT32)StrToInt(szArgv[1]);
    u4FrameSize= (UINT32)StrToInt(szArgv[2]);

    u1DrvScpipH264SetDramAddress(u4BaseAddr, u4FrameSize);
    printf("[SCPIP_VENC]Set venc dram address[0x%x], Size[0x%x]\n", u4BaseAddr, u4FrameSize);

    return 0;
}

static INT32 _ScpipVencSetWriteReadyMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Select;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <u1Select(0: continuous mode,  1: ready mode>\n");
        return 0;
    }

    u1Select = (UINT8)StrToInt(szArgv[1]);

    u1DrvScpipH264ReadyMode(u1Select);
    printf("[SCPIP_VENC]Set dram write mode %d(0:continuous mode ,  1: ready mode)\n", u1Select);

    return 0;
}

static INT32 _ScpipVencWriteTrigger(INT32 i4Argc, const CHAR **szArgv)
{
    //UINT8 u1Result ;
    u1DrvScpipH264TriggerNext();

    #if 0
    if(u1Result ==FALSE)
    {
    	printf("[SCPIP_VENC] Dram write  trigger FAIL, Current write mode is not trigger mode\n");
    }
    else
    {
    	printf("[SCPIP_VENC] Dram write trigger OK\n");
    }
    #endif
    
    return 0;
}

static INT32 _ScpipVencWriteEnable(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Enable;
    
    if (i4Argc < 2)
    {
        Printf("Usage: <u1Enable(0:off, 1:on)>\n");
        return 0;
    }

    u1Enable = (UINT8)StrToInt(szArgv[1]);

    u1DrvScpipH264Enable(u1Enable);
    printf("[SCPIP_VENC] Write enable(%d)\n", u1Enable);

    return 0;

}
	
static INT32 _ScpipVencSetFrameDrop(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1VdpId, u1Enable;
    UINT8 u1DropM, u1DropN;
    
    if (i4Argc < 5)
    {
        Printf("Usage: <u1VdpId u1Enable u1DropM u1DropN>\n");
        Printf("[SCPIP_VENC] u1DropM and u1DropN must between 1~0xff\n");
        Printf("[SCPIP_VENC] Example: M=3, N=1 that means 1/3 data enable. Every 3 frames drop 2.\n");
        return 0;
    }
    u1VdpId  = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);
    u1DropM  = (UINT8)StrToInt(szArgv[3]);
    u1DropN  = (UINT8)StrToInt(szArgv[4]);

    if(u1DropM > 0xff || u1DropN > 0xff || u1VdpId >= VENC_NS || u1DropM < u1DropN)
    {
    	Printf("[SCPIP_VENC] u1DropM and u1DropN must between 1~0xff Or other setting fail\n");
    }
    else
    {
    	vDrvScpipVencDropFrame(u1VdpId, u1Enable, u1DropM, u1DropN);
    	printf("[SCPIP_VENC] Frame drop(%d) enable(%d), M(%d), N(%d)\n", u1VdpId, u1Enable, u1DropM, u1DropN);
    }
    return 0;

}

static INT32 _ScpipVencDumpDram(INT32 i4Argc, const CHAR **szArgv)
{
    if (i4Argc > 1)
    {
        Printf("Scaler venc dramp dram no input\n");
        return 0;
    }

    if(u1DrvScpipDumpDataToUsb())
    {
       Printf("Scaler venc dump successful\n"); 
    }
    else
    {
       Printf("Scaler venc dump fail:not open venc or define\n");
    }
    return 0;

}

#if 0
static INT32 _ScpipVencPairInv(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1VdpId, u1Enable;
    
    if (i4Argc < 3)
    {
        Printf("field pair invert on/off \n");
        Printf("Usage: <path u1Enable>\n");
        return 0;
    }

    u1VdpId  = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    vScpipSetCtrlFldInv(u1VdpId,u1Enable);

    return 0;

}

static INT32 _ScpipVencAddrInv(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1VdpId, u1Enable;
    
    if (i4Argc < 3)
    {
        Printf("address invert on/off\n");
        Printf("Usage: <path u1Enable>\n");
        return 0;
    }

    u1VdpId  = (UINT8)StrToInt(szArgv[1]);
    u1Enable = (UINT8)StrToInt(szArgv[2]);

    vScpipSetAddrFldInv(u1VdpId,u1Enable);
    
    return 0;

}
#endif

static INT32 _ScpipVencStatusQuery(INT32 i4Argc, const CHAR **szArgv)
{
	vDrvScpipH264Query();
	return 0;
}

#endif //#ifdef CC_CLI
#endif // #ifdef CC_SUPPORT_VENC

