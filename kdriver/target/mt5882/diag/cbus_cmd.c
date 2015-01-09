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
 * $RCSfile: cbus_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/
/** @file nptv_hdmicec_cmd.c
 *  This C file implements the HDMI CBUS CLI test functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"
#ifdef CC_MT5882
#include "drv_cbus_cpi_if.h"
#else
#include "drv_cbus_if.h"
#endif
#include "x_stl_lib.h"
#include "x_os.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#include "x_timer.h"
#include "x_rand.h"
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
//random data
#ifndef __KERNEL__
extern int rand(void);
extern void srand(unsigned int seed);	//extern void srand(UINT32 seed);
#endif
#ifndef CC_MT5882
static UINT16 _gen_random(void) //to generate the random seed
{
    UINT32 ticks;
	HAL_TIME_T tTime;
	HAL_GetTime(&tTime);
	ticks = tTime.u4Micros + tTime.u4Seconds;
#ifdef 	__KERNEL__
  return 0;
#else  
	srand(ticks);
	return (rand()>>5);
#endif	
}
#endif
static INT32 CbusCli_LoopbackTestCmd(INT32 i4Argc, const CHAR **szArgv)
{
	#ifndef CC_MT5882
	MHL_MSG_T arMsg[24];
	UINT8 cmd_id;
	UINT8 i=0;
	UINT16 tmp;
	INT32 j=0;
	if (i4Argc < 2)
	{
		Printf("txtest	cmd_id.\n");
		return 0;		 
	}		
	cmd_id = (UINT8)StrToInt(szArgv[1]);
	for(j=0; j< 10000; j++)
	{
	for(i=0; i<24; i++)
	{
	   tmp = _gen_random();
	   arMsg[i].u1Data = (tmp & 0xff);
	   arMsg[i].ctrl = (tmp & 0x100)>>8;
	   arMsg[i].channel = (tmp & 0x600)>>9;
	}

	//Printf("cbus send msg size: %d\n", cmd_id);

	for(i=0; i< cmd_id; i++)
	{
		_Mhl_TxEnqueue(&(arMsg[i]));
	}		
		vCbusSendMsg();
	x_thread_delay(10);
	}
	#endif
	return 0;
}

static INT32 CbusCli_TestCmd(INT32 i4Argc, const CHAR **szArgv)    
{
	UINT8 cmd_id = 0;
	UINT8 sub_cmd_id = 0;
    if (i4Argc < 2)
    {
        Printf("test  cmd_id.\n");
        Printf("1 set hpd \n");
        Printf("2 clr hpd \n");
        Printf("3 set path en");
        Printf("4 set DEV_CAP ready \n");
        Printf("5 DCAP_CHG \n");
        Printf("6 set EDID change\n");
        Printf("7 read CAP_DEV register \n");
        Printf("9 send INT_REQ_WRT \n");
        Printf("10 write scracthpad register \n");
        Printf("11 RAP polling cmd \n");
        Printf("12 RAP content on cmd \n");
        Printf("13 RAP conten off cmd\n");
#ifdef CC_MHL_3D_SUPPORT        
        Printf("16 3d edid config \n");
        Printf("17 3d write burst test \n");
#endif        
        Printf("18 get vendor id \n");
        Printf("19 get msc error code\n");
        Printf("20 path disable\n");
        return 0;        
    }    	
    cmd_id = (UINT8)StrToInt(szArgv[1]);
    if (i4Argc > 2)
    {
        sub_cmd_id = (UINT8)StrToInt(szArgv[2]);
    }
    vCbusCmdTest(cmd_id,sub_cmd_id);
    return 0;
}
static INT32 CbusCli_SetState(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 cmd_id;
    if (i4Argc < 2)
    {
        Printf("test  cmd_id.\n");
        Printf("1 MHL_STATE_INIT \n");
        Printf("2 MHL_STATE_BOOTUP \n");
        Printf("3 MHL_STATE_WAIT_CABLE_CONNECT \n");
        Printf("4 MHL_STATE_DISCOVERY\n");
        Printf("5 MHL_STATE_CONNECTED \n");
        Printf("6 MHL_STATE_CABLE_DISCONNECT \n");
        return 0;        
    }    	
    cmd_id = (UINT8)StrToInt(szArgv[1]);
    
	vCbusSetState(cmd_id);
    return 0;

}
static INT32 CbusCli_SetHpd(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 cmd_id;
	if (i4Argc < 2)
	{
		Printf("shpd  enable.\n");
		return 0;		 
	}		
	cmd_id = (UINT8)StrToInt(szArgv[1]);
	
	vCbusSetHpdFlag(cmd_id);
	return 0;

}
static INT32 CbusCli_DebugCmd(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 cmd_id;	
    if (i4Argc < 2)
    {
        Printf("open debug log.\n");
        Printf("1 enable \n");
        Printf("0 disable \n");
        return 0;        
    }    	
			
	cmd_id = (UINT8)StrToInt(szArgv[1]);
	
	vCbusOpenDebuglog(cmd_id);
	return 0;

}

static INT32 CbusCli_RCPTest(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 key_code;	
    if (i4Argc < 2)
    {
        Printf("rcp test: key-code.\n");
        return 0;        
    }    	
			
	key_code = (UINT8)StrToInt(szArgv[1]);
	
	vCbusRcpTest(key_code);
	return 0;

}

static INT32 CbusCli_UCPTest(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 key_code;	
    if (i4Argc < 2)
    {
        Printf("ucp test: utf-8 character.\n");
        return 0;        
    }    	
			
	key_code = (UINT8)StrToInt(szArgv[1]);
	vCbusUcpTest(key_code);
		
	return 0;

}

static INT32 CbusCli_UCPKTest(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 key_code;	
    if (i4Argc < 2)
    {
        Printf("ucp test: utf-8 character.\n");
        return 0;        
    }    	
			
	key_code = (UINT8)StrToInt(szArgv[1]);
	vCbusUcpKTest(key_code);

	return 0;

}

static INT32 CbusCli_QueryCmd(INT32 i4Argc, const CHAR **szArgv)    
{
    vCbusStatusQuery();
    return 0;
}
static INT32 CbusCli_Init(INT32 i4Argc, const CHAR **szArgv)
{
    Printf("CbusCli_Init.\n");
    
    vCbusInit();

    return 0;
}
static INT32 CbusCli_Stop(INT32 i4Argc, const CHAR **szArgv)
{
    Printf("stop cbus fw\n");
	vCbusStop();
	return 0;
}
static INT32 CbusCli_Resume(INT32 i4Argc, const CHAR **szArgv)
{
    Printf("Resume cbus fw\n");
	vCbusResume();
	return 0;
}

static INT32 CbusCli_Version(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Version = 0;
	Printf("CBUS current version %x\n", u1CbusGetVersion());
    if (i4Argc < 2)
    {
		Printf("set new version\n");
		Printf("32 for version 2.0\n");
		Printf("18 for version 1.2\n");
		Printf("17 for version 1.1\n");
		Printf("16 for version 1.0\n");
        return 0;        
    }    	
	u1Version = (UINT8)StrToInt(szArgv[1]);
	vCbusSetVersion(u1Version);
	Printf("CBUS new version %x\n", u1CbusGetVersion());

	return 0;
}



//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
CLIMOD_DEBUG_FUNCTIONS(CBUS)

// Table for CBUS command entry

static CLI_EXEC_T _arCbusCmdTbl [] =
{
    CLIMOD_DEBUG_CLIENTRY(CBUS),

    { "CBUS Loopback Test", "lptest", CbusCli_LoopbackTestCmd, NULL, "CBUS loopback test command", CLI_GUEST},
    { "CBUS Test", "test", CbusCli_TestCmd, NULL, "CBUS test command", CLI_GUEST},
    { "CBUS query", "q", CbusCli_QueryCmd, NULL, "CBUS query command", CLI_GUEST},
    { "CBUS Initial", "i", CbusCli_Init, NULL, "HDMI CBUS register initial", CLI_GUEST },
	{ "CBUS State", "ss", CbusCli_SetState, NULL, "HDMI CBUS Set state", CLI_GUEST },
	{ "CBUS Sethpd", "shpd", CbusCli_SetHpd, NULL, "HDMI CBUS Set hpd when connect", CLI_GUEST },
	{ "CBUS Query", "q", CbusCli_QueryCmd, NULL, "HDMI CBUS query current status", CLI_GUEST },
	{ "CBUS dbglvl", "dl", CbusCli_DebugCmd, NULL, "HDMI CBUS open debug log", CLI_GUEST },
	{ "CBUS rcptest", "rcp", CbusCli_RCPTest, NULL, "Rcp test", CLI_GUEST },
	{ "CBUS ucptest", "ucp", CbusCli_UCPTest, NULL, "Ucp test", CLI_GUEST },
	{ "CBUS ucptest", "ucpk", CbusCli_UCPKTest, NULL, "Ucpk test", CLI_GUEST },
	{ "CBUS Stop", "stop", CbusCli_Stop, NULL, "Cbus stop", CLI_GUEST },
	{ "CBUS resume", "resume", CbusCli_Resume, NULL, "Cbus resume", CLI_GUEST },
	{ "CBUS version","ver", CbusCli_Version, NULL, "Cbus version", CLI_GUEST},
    { NULL, NULL, NULL, NULL, NULL, CLI_GUEST }
};

CLI_MAIN_COMMAND_ITEM(Cbus)
{
    "cbus", NULL, NULL, _arCbusCmdTbl, "HDMI CBUS test", CLI_GUEST
};


