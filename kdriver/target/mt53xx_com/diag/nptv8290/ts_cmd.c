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
 * $RCSfile: tcon_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation for Thermal Control function
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_stl_lib.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"

#include "drv_thermal_if.h"

#ifndef CC_CLI
#define CC_CLI
#endif

/* cli Functions */
static INT32 _i32CliTSInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i32CliTSGetCPUTemp(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i32CliTSSetTriggerTemp(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i32CliTSSetADCSource(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i32CliTSSetTestMode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i32CliTSGetEfuseData(INT32 i4Argc, const CHAR **szArgv);


/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _i32CliTSInit(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvThermalInit();

    return 0;
}

static INT32 _i32CliTSGetCPUTemp(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("CPU temp = %5d\n", CPU_Temp());

    return 0;  

}

static INT32 _i32CliTSGetTriggerTemp(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	Get_Thermal_Ctrl_Trigger_Temp();

    return 0;    

}

static INT32 _i32CliTSSetTriggerTemp(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 i4Temp;
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("Tempature (Uint: 0.001 degree)\n");
		Printf("[80 degree] ptp.stt 80000 \n");		
		return 0;
	}

	i4Temp = (INT32) StrToInt(szArgv[1]);
	Set_Thermal_Ctrl_Trigger(i4Temp);
    
    return 0;   

}

static INT32 _i32CliTSSetADCSource(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Temp;
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("PTP value source:\n");	
		Printf("[0] E_PTP_AVERAGE\n");	
		Printf("[1] E_PTP_IMM\n");			
		Printf("[2] E_SRVAD_IMM\n");					
		return 0;
	}   

	u4Temp = (INT32) StrToInt(szArgv[1]);
	vDrvSetPTPADCMode((UINT8)u4Temp);
    
    return 0;   

}

static INT32 _i32CliTSSetTestMode(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Temp;
    if ((i4Argc < 2) || (szArgv == NULL) || (szArgv[1] == NULL))
	{
		Printf("PTP value source:\n");	
		Printf("[0] E_PTP_AVERAGE\n");	
		Printf("[1] E_PTP_IMM\n");			
		Printf("[2] E_SRVAD_IMM\n");					
		return 0;
	}   

	u4Temp = (INT32) StrToInt(szArgv[1]);
	TS_THERMAL_Module_Test((UINT8)u4Temp);
    
    return 0;   

}

static INT32 _i32CliTSGetEfuseData(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	vDrvGetEFuse_ThermalSesnorData();

    return 0;    

}

/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for command entry

static CLI_EXEC_T arTSCmdTbl[] = {
  {"Init", "i", _i32CliTSInit, NULL, "TS init", CLI_SUPERVISOR},
  {"CPU Temp", "t", _i32CliTSGetCPUTemp, NULL, "Get CPU temperature", CLI_GUEST},  	
  {"Get trig Temp", "gtt", _i32CliTSGetTriggerTemp, NULL, "Get trigger temperature", CLI_GUEST},  
  {"Set trig Temp", "stt", _i32CliTSSetTriggerTemp, NULL, "Set trigger temperature", CLI_GUEST},
  {"Set ADC source", "source", _i32CliTSSetADCSource, NULL, "Set ADC source", CLI_GUEST},  
  {"Do Test", "test", _i32CliTSSetTestMode, NULL, "Do test", CLI_SUPERVISOR},    
  {"Get Efuse data", "efuse", _i32CliTSGetEfuseData, NULL, "Get Efuse data", CLI_SUPERVISOR},  
	  

  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

//static CLI_EXEC_T _rNetModCmdTbl =
CLI_MAIN_COMMAND_ITEM(TS)
{
	"ptp",
	NULL,
	NULL,
	arTSCmdTbl,
	"PTP control command",
	CLI_GUEST
};


