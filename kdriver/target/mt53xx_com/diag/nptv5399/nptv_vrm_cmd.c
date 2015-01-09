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
 * $RCSfile: nptv_vrm_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/
#include "x_drv_cli.h"
#include "x_debug.h"
#include "vdo_rm.h"
#include "x_os.h"
#include "x_stl_lib.h"
#include "x_assert.h"

#include "drv_scaler.h"
#include "hw_scpos.h"
#include "hw_di.h"
#include "hw_regen.h"
#include "hw_predown.h"

#ifndef CC_CLI
#define CC_CLI
#endif

#ifdef CC_CLI
//VRM command
static INT32 _VRMInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VRMUpdate(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VRMGet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VRMDump(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VRMSetImportDelay(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VRMSetImportModule(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VRMGetImportStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VRMTriggerImport(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VRMSetForceOvsnModule(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VRMSetGetOvsnModule(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VRMSetAppMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _VRMGetAppMode(INT32 i4Argc, const CHAR **szArgv);

#endif

CLIMOD_DEBUG_FUNCTIONS(SCPOS)

CLI_EXEC_T arVrmCmdTbl[] = 
{
    #ifdef CC_CLI
    {"VRM init",            "i",     _VRMInit,              NULL, "VRM init",                    CLI_SUPERVISOR},
    {"VRM update",          "u",     _VRMUpdate,            NULL, "VRM update",                  CLI_SUPERVISOR},    
    {"VRM get",             "g",     _VRMGet,               NULL, "VRM get",                     CLI_SUPERVISOR},        
    {"VRM dump",            "d",     _VRMDump,              NULL, "VRM dump",                    CLI_GUEST},            
    {"Set Import Delay",    "dly",   _VRMSetImportDelay,    NULL, "Set Import Delay",            CLI_SUPERVISOR},            
    {"Set Import Module",   "sim",   _VRMSetImportModule,   NULL, "Set Import Module",           CLI_SUPERVISOR},            
    {"Get Import Status",   "gis",   _VRMGetImportStatus,   NULL, "Get Import Status",           CLI_SUPERVISOR},            
    {"Trigger",             "t",     _VRMTriggerImport,     NULL, "Trigger manually",            CLI_GUEST},            
    {"Force ovsn module",   "fom",   _VRMSetForceOvsnModule,NULL, "Set Forced overscan module",  CLI_GUEST},
    {"Get ovsn module",     "gom",   _VRMSetGetOvsnModule,  NULL, "Get overscan module",         CLI_GUEST},
    {"Set App mode",        "appset",_VRMSetAppMode,        NULL, "Set application mode",        CLI_SUPERVISOR},
    {"Get App mode",        "appget",_VRMGetAppMode,        NULL, "Get application mode",        CLI_SUPERVISOR},    
    #endif
    CLIMOD_DEBUG_CLIENTRY(SCPOS),    
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#ifdef CC_CLI
//VRM Command
static INT32 _VRMInit(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vVRMSwInit();
    Printf("Init VRM.....\n");

    return 0;    
}

static INT32 _VRMUpdate(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    if(i4Argc != 2)
    {
        Printf("Usage: u path\n");
        Printf("path: 0 Main.\n");
        Printf("path: 1 Sub.\n");
        return 0;
    }
    
    bPath = ((UINT8)StrToInt(szArgv[1])==0)? SV_VP_MAIN:SV_VP_PIP;

    vVRMTriggerUpdateByCli(bPath);
    return 0;
}

static INT32 _VRMGet(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    UINT16 u2ModuleId;
    VRM_INFO_T in;
    VRM_INFO_T out;
 
    if(i4Argc != 3)
    {
        Printf("Usage: g path moduleId\n");
        Printf("path: 0 Main.\n");
        Printf("path: 1 Sub.\n");
        Printf("moduleId: 0 VRM_MODULE_OMUX.\n");
        Printf("moduleId: 1 VRM_MODULE_CB.\n");  
        Printf("moduleId: 2 VRM_MODULE_DQ.\n");
        Printf("moduleId: 3 VRM_MODULE_TTT.\n");
        Printf("moduleId: 4 VRM_MODULE_PREDOWN.\n");
        Printf("moduleId: 5 VRM_MODULE_REGEN.\n");
        Printf("moduleId: 6 VRM_MODULE_MIB.\n");
        Printf("moduleId: 7 VRM_MODULE_FSC.\n");
        Printf("moduleId: 8 VRM_MODULE_MJC.\n");
        Printf("moduleId: 9 VRM_MODULE_PSC.\n");        
        return 0;
    }
    
    bPath = ((UINT8)StrToInt(szArgv[1])==0)? SV_VP_MAIN:SV_VP_PIP;
    u2ModuleId = (UINT16)StrToInt(szArgv[2]);

    VRMGetModuleVRMInfo(bPath,(VRM_MODULE_E)u2ModuleId, &in, &out);
    vVRMDumpVRMInfo(bPath, (VRM_MODULE_E)in.u2VRMID);
    vVRMDumpVRMInfo(bPath, (VRM_MODULE_E)out.u2VRMID);
    return 0;
}

static INT32 _VRMDump(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);
    Printf("VRM Dump.....\n");
    vVRMDumpAllVRMInfo();
    return 0;
}

static INT32 _VRMSetImportDelay(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath, u1Delay;
    
    if(i4Argc != 3)
    {
        Printf("Usage: dly path delay\n");
        Printf("Main = %d Sub = %d\n", vVRMGetImportDelay(SV_VP_MAIN), vVRMGetImportDelay(SV_VP_PIP));
        return 0;
    }

    bPath   = (UINT8)StrToInt(szArgv[1]);
    u1Delay = (UINT8)StrToInt(szArgv[2]);
    vVRMSetImportDelay(bPath, u1Delay);
    return 0;
}

static INT32 _VRMSetImportModule(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    UINT8 bModuleId;
    
    if(i4Argc != 3)
    {
        Printf("Usage: sim path moduleId\n");
        Printf("path: 0 Main.\n");
        Printf("path: 1 Sub.\n");
        Printf("moduleId: 0 IMPORT_MODULE_NONE.\n");
        Printf("moduleId: 1 IMPORT_MODULE_B2R.\n");  
        Printf("moduleId: 2 IMPORT_MODULE_PDS.\n");
        Printf("moduleId: 3 IMPORT_MODULE_REGEN.\n");
        Printf("moduleId: 4 IMPORT_MODULE_MIB.\n");
        Printf("moduleId: 5 IMPORT_MODULE_FSC.\n");
        Printf("moduleId: 6 IMPORT_MODULE_UNKNOWN.\n");

        Printf("Main: %d Sub: %d\n", vDrvGetImportTriggerModule(SV_VP_MAIN), vDrvGetImportTriggerModule(SV_VP_PIP));
        return 0;
    }
    
    bPath = ((UINT8)StrToInt(szArgv[1])==0)? SV_VP_MAIN:SV_VP_PIP;
    bModuleId = (UINT8)StrToInt(szArgv[2]);
    vDrvSetForcedImportTriggerModule(bPath,(IMPORT_MODULE_E)bModuleId);
    return 0;
}    

static INT32 _VRMGetImportStatus(INT32 i4Argc, const CHAR **szArgv)
{
//    UINT8 bPath = 0;
    UINT32 pipOffset = 0;
    
    if(i4Argc != 1)
    {
        Printf("Usage: gis\n");
        //SCCTRL1_0A_SC_CLR_STA_SET_RES_O_1;
        return 0;
    }
    
    Printf("\n<Import Setting>\n\n");
    NPTV_REG_PRINT(PRE_DS_MAIN_05+pipOffset,     PRE_DS_MAIN_05_MAIN_SETRES_ACT_LINE);   
    NPTV_REG_PRINT(PRE_DS_MAIN_05+pipOffset,     PRE_DS_MAIN_05_PREDS_MAIN_SETRES_EN);   
    NPTV_REG_PRINT(PRE_RE_GEN_MAIN_00+pipOffset, RG_M_SET_RES_MODE);   
    NPTV_REG_PRINT(PRE_RE_GEN_MAIN_00+pipOffset, RG_M_SET_RES_IN_TRIG_SEL);   
    NPTV_REG_PRINT(PRE_RE_GEN_MAIN_00+pipOffset, RG_M_BYPASS_SET_RES_DB);   
    NPTV_REG_PRINT(MCVP_KC_18+pipOffset,         MCVP_SET_RESOLUTION);   
    NPTV_REG_PRINT(SCPIP_SCCTRL1_0A+pipOffset,   SCCTRL1_0A_SC_SET_RES_TOGGLE_1);
    NPTV_REG_PRINT(SCPIP_SCCTRL1_0A+pipOffset,   SCCTRL1_0A_SC_SET_RES_IMPORT_ENABLE_1);
    
    Printf("\n<Import Status>\n\n");
    NPTV_REG_PRINT(SCPIP_SCCTRL1_0A+pipOffset,   SCCTRL1_0A_SC_STA_SET_RES_I_1);   
    NPTV_REG_PRINT(SCPIP_SCCTRL1_0A+pipOffset,   SCCTRL1_0A_SC_STA_SET_RES_O_1);   
    NPTV_REG_PRINT(MCVP_KC_18+pipOffset,         MCVP_SET_RES_FINISH_I);   
    NPTV_REG_PRINT(MCVP_KC_18+pipOffset,         MCVP_SET_RES_FINISH_O);   
    NPTV_REG_PRINT(PRE_RE_GEN_MAIN_00+pipOffset, RG_M_SET_RES_FINISHED_I);   
    NPTV_REG_PRINT(PRE_RE_GEN_MAIN_00+pipOffset, RG_M_SET_RES_FINISHED_O);   
    NPTV_REG_PRINT(PRE_DS_MAIN_05+pipOffset,     PRE_DS_MAIN_05_MAIN_SET_RES_FINISH_I);
    NPTV_REG_PRINT(PRE_DS_MAIN_05+pipOffset,     PRE_DS_MAIN_05_MAIN_SET_RES_FINISH_O);

    return 0;
}

static INT32 _VRMTriggerImport(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    
    if(i4Argc != 2)
    {
        Printf("Usage: t path\n");
        
        Printf("Main: %d Sub: %d\n", vDrvGetImportTriggerModule(SV_VP_MAIN), vDrvGetImportTriggerModule(SV_VP_PIP));
        return 0;
    }
    
    bPath = ((UINT8)StrToInt(szArgv[1])==0)? SV_VP_MAIN:SV_VP_PIP;
    vDrvFireImportTrigger(bPath);
    return 0;
}

static INT32 _VRMSetForceOvsnModule(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bPath;
    UINT8 bOvsnModule;
    
    if(i4Argc != 3)
    {
        Printf("Usage: fom path module\n");        
        Printf("[path] 0:main 1:sub\n");
        Printf("[module] 0:auto 1:regen 2:fsc regen\n");        
        return 0;
    }
    
    bPath = ((UINT8)StrToInt(szArgv[1])==0)? SV_VP_MAIN:SV_VP_PIP;
    bOvsnModule = (UINT8)StrToInt(szArgv[2]);

    if(bOvsnModule > 2)
    {
        Printf("Invalid parameter\n");    
        Printf("[module] 0:auto 1:regen 2:fsc regen\n");
        return 0;    
    }

    vVrmSetForcedOvsnModule(bPath, (FORCED_OVSN_MODULE_E)bOvsnModule);

    return 0;    
}

static INT32 _VRMSetGetOvsnModule(INT32 i4Argc, const CHAR **szArgv)
{
    OVSN_MODULE_E bOvsnModule;
    UINT8 bPath;
    
     if(i4Argc != 2)
    {
        Printf("[path] 0:main 1:sub\n");
        return 0;
    }
     
    bPath = ((UINT8)StrToInt(szArgv[1])==0)? SV_VP_MAIN:SV_VP_PIP;
    
    bOvsnModule = eVrmGetOvsnModule(bPath);
    
    Printf("Current Overscan Module :%s\n",bOvsnModule == 0?"OVSN_MODULE_REGEN":"OVSN_MODULE_FSC_REGEN"); 

    return 0;    
}

static INT32 _VRMSetAppMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bAppMode;
    
    if(i4Argc != 2)
    {
        Printf("Usage: set application mode\n");
        Printf("bit0: Sub display\n");
        Printf("bit1: Video encoder\n");
        Printf("bit2: Graphic dump\n");
        Printf("bit3: SCART out\n");
        return 0;
    }
    
    bAppMode = (UINT8)StrToInt(szArgv[1]);
    if (bAppMode > 0xF)
    {
        Printf("Invalid parameter\n");    
        return 0;    
    }

    vDrvVrmSetAppFlag(bAppMode);

    return 0;    
}

static INT32 _VRMGetAppMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 bAppMode;
    
    UNUSED(i4Argc);
    UNUSED(szArgv); 
    
    bAppMode = u4DrvVrmGetAppFlag();
    Printf("Current App Mode :%d\n",bAppMode); 

    return 0;    
}


#endif 
 
