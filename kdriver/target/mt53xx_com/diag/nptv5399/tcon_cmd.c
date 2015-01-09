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
 *         This file contains CLI implementation for Tcon function
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

#include "drv_tcon.h"
#include "drv_lvds.h"
#include "panel.h"
#include "drv_vdoclk.h"

#ifndef CC_CLI
#define CC_CLI
#endif

/* cli Functions */
static INT32 _i32CliTconInit(INT32 i4Argc, const CHAR **szArgv);

static INT32 _i32CliTconOffOn(INT32 i4Argc, const CHAR **szArgv);


static INT32 _i32CliTconIsReady(INT32 i4Argc, const CHAR **szArgv);

static INT32 _i32CliTconTp(INT32 i4Argc, const CHAR **szArgv);
//static INT32 _i32CliTconRegDump(INT32 i4Argc, const CHAR **szArgv);
//static INT32 _i32CliTconRegDiff(INT32 i4Argc, const CHAR **szArgv);

/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _i32CliTconInit(INT32 i4Argc, const CHAR **szArgv)
{
	INT32 i4Ret;
	
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	i4Ret = u1DrvTconInit();

	if (i4Ret == 1)
	{
		Printf("Tcon Init ok");
	}
	else
	{
		Printf("Tcon Init fail !!!");
	}

	return 0;
}

static INT32 _i32CliTconOffOn(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 i4Ret;
    VIDEO_TCON_POWER_SEQ_T eTconPowSeq;
    BOOL val;
	
	if (i4Argc != 3)
	{
		Printf("Usage: %s [TCON signal] [on/off]\n" \
            "[TCON signal] \n" \
            "  0: VIDEO_TCON_GST \n" \
            "  1: VIDEO_TCON_MCLK \n" \
            "  2: VIDEO_TCON_GCLK \n" \
            "  3: VIDEO_TCON_EO \n" \
            "  4: VIDEO_TCON_VCOM_DYN \n" \
            "  5: VIDEO_TCON_EPISIGNAL \n" \
            "  6: VIDEO_TCON_ALL \n" \
            "  others: TCON Vsync ISR \n" \
            "[on/off] \n" \
            "  0: off \n" \
            "  1: on \n", szArgv[0]);
		return 0;
	}

    eTconPowSeq = (VIDEO_TCON_POWER_SEQ_T)StrToInt(szArgv[1]);
    val = (BOOL)StrToInt(szArgv[2]);
	i4Ret = (INT32)u1DrvEPITgOnOff(eTconPowSeq,  val);

	if (i4Ret == 1)
	{
		Printf("Tcon %d off/on successful", i4Argc);
	}
	else
	{
		Printf("Tcon %d off/on fail!!!", i4Argc);
	}

	return 0;
}

static INT32 _i32CliTconIsReady(INT32 i4Argc, const CHAR **szArgv)
{
	INT32 i4Ret;
	
	if (i4Argc != 1)
	{
		Printf("Usage: %s \n", szArgv[0]);
		return 0;
	}

	i4Ret = (INT32)u1DrvTconIsReady();

	if (i4Ret == 1)
	{
		Printf("Tcon is ready");
	}
	else
	{
		Printf("Tcon is not ready!!!");
	}

	return 0;
}

static const CHAR * _aszTconTp[] =
{
  "TCON_TP_OFF",
  "TCON_TP_R",
  "TCON_TP_G",
  "TCON_TP_B",
  "TCON_TP_BLACK",
  "TCON_TP_WHITE",
  "TCON_TP_COLORRAMP",
  "TCON_TP_MOVING",
  "TCON_TP_AUTO",
  "TCON_TP_MAX", 
};

static INT32 _i32CliTconTp(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 i;
	UINT8 u1arg1;
	
	if (i4Argc != 2)
	{
		Printf("Usage: %s \n", szArgv[0]);

		for (i = 0 ; i < TCON_TP_MAX ; i++)
		{
		    Printf("%d: %s \n", i, _aszTconTp[i]);
		}

		return 0;
	}

	u1arg1 = (UINT8)StrToInt(szArgv[1]);

	u1DrvTconTP(u1arg1);

	return 0;
}
/*
static INT32 _i32CliTconRegDump(INT32 i4Argc, const CHAR **szArgv)
{

	if (i4Argc != 1)
	{
		Printf("Usage: %s \n", szArgv[0]);
		return 0;
	}

	u1DrvTconRegDump();

	return 0;
}

static INT32 _i32CliTconRegDiff(INT32 i4Argc, const CHAR **szArgv)
{

	if (i4Argc != 1)
	{
		Printf("Usage: %s \n", szArgv[0]);
		return 0;
	}

	u1DrvTconRegDiff();

	return 0;
}
*/

static INT32 _i32CliTconPanelSet(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1;
	
	if (i4Argc != 2)
	{
		Printf("Usage: [case] \n" \
			   "[Case]: [Freq] [PORT] [Ration] \n" \
			   "Case  1: 297000000 FOUR_PORT   TCLK_PCLK_RATION_1\n" \
			   "Case  2: 148500000 DUAL_PORT   TCLK_PCLK_RATION_1\n" \
			   "Case  3:  74500000 SINGLE_PORT TCLK_PCLK_RATION_1\n" \
			   "Case  4: 340000000 FOUR_PORT   TCLK_PCLK_RATION_1\n" \
			   "Case  5: 170000000 DUAL_PORT   TCLK_PCLK_RATION_1\n" \
			   "Case  6:  85000000 SINGLE_PORT TCLK_PCLK_RATION_1\n" \
			   "Case  7: 170000000 DUAL_PORT   TCLK_PCLK_RATION_2\n" \
			   "Case  8:  85000000 SINGLE_PORT TCLK_PCLK_RATION_2\n" \
			   "Case  9:  85000000 SINGLE_PORT TCLK_PCLK_RATION_4\n" \
			   "Case 10:  85000000 SINGLE_PORT TCLK_PCLK_RATION_1P5\n" \
			   "Case 11:  85000000 SINGLE_PORT TCLK_PCLK_RATION_3\n" \
			   "Case 12:  85000000 SINGLE_PORT TCLK_PCLK_RATION_0P75\n" \
			   "Case 13: 170000000 DUAL_PORT   TCLK_PCLK_RATION_0P75\n",szArgv[0]);
			   
		return 0;
	}
	u1arg1 = (UINT8)StrToInt(szArgv[1]);

	if (u1arg1==1) u1DrvTconMLDSVOPLLFreqSet(297000000,  FOUR_PORT,TCLK_PCLK_RATION_1);
    else if (u1arg1==2) u1DrvTconMLDSVOPLLFreqSet(148500000,  DUAL_PORT,TCLK_PCLK_RATION_1);
    else if (u1arg1==3) u1DrvTconMLDSVOPLLFreqSet( 74250000,SINGLE_PORT,TCLK_PCLK_RATION_1);
	else if (u1arg1==4) u1DrvTconMLDSVOPLLFreqSet(340000000,  FOUR_PORT,TCLK_PCLK_RATION_1);
    else if (u1arg1==5) u1DrvTconMLDSVOPLLFreqSet(170000000,  DUAL_PORT,TCLK_PCLK_RATION_1);
    else if (u1arg1==6) u1DrvTconMLDSVOPLLFreqSet( 85000000,SINGLE_PORT,TCLK_PCLK_RATION_1);
    else if (u1arg1==7) u1DrvTconMLDSVOPLLFreqSet(170000000,  DUAL_PORT,TCLK_PCLK_RATION_2);
    else if (u1arg1==8) u1DrvTconMLDSVOPLLFreqSet( 85000000,SINGLE_PORT,TCLK_PCLK_RATION_2);
    else if (u1arg1==9) u1DrvTconMLDSVOPLLFreqSet( 85000000,SINGLE_PORT,TCLK_PCLK_RATION_4);
    else if (u1arg1==10) u1DrvTconMLDSVOPLLFreqSet( 85000000,SINGLE_PORT,TCLK_PCLK_RATION_1P5);
    else if (u1arg1==11) u1DrvTconMLDSVOPLLFreqSet( 85000000,SINGLE_PORT,TCLK_PCLK_RATION_3);
    else if (u1arg1==12) u1DrvTconMLDSVOPLLFreqSet( 85000000,SINGLE_PORT,TCLK_PCLK_RATION_0P75);
    else if (u1arg1==13) u1DrvTconMLDSVOPLLFreqSet(170000000,  DUAL_PORT,TCLK_PCLK_RATION_0P75);
    
    return 0;
}



static INT32 _i32CliTconFreqSet(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 u1arg1;
	UINT8 u1arg2;
	UINT8 u1arg3;
	
	if (i4Argc != 4)
	{
		Printf("Usage: [Freq] [PORT] [Ration] \n" \
			   "Ex 01: tcon.freq 297000000 16 0 \n" \
			   "Ex 02: tcon.freq 148500000 8 0 \n" \
			   "Ex 03: tcon.freq  74250000 0 0 \n" \
			   "Ex 04: tcon.freq 340000000 16 0 \n" \
			   "Ex 05: tcon.freq 170000000 8 0 \n" \
			   "Ex 06: tcon.freq  85000000 0 0 \n" \
			   "Ex 07: tcon.freq 170000000 8 2 \n" \
			   "Ex 08: tcon.freq  85000000 0 2 \n" \
			   "Ex 09: tcon.freq  85000000 0 4 \n" \
			   "Ex 10: tcon.freq  85000000 0 1 \n" \
			   "Ex 11: tcon.freq  85000000 0 3 \n" \
			   "Ex 12: tcon.freq  85000000 0 5 \n" \
			   "Ex 13: tcon.freq 170000000 8 5 \n" \
			   "[Freq]: PCLK (UINT: HZ) \n" \
			   "[PORT]:  0: Singel \n" \
			   "[PORT]:  8: DUAL \n" \
			   "[PORT]: 16: FOUR \n" \
			   "[Ration]: 0:TCLK/PCLK=1 \n" \
			   "[Ration]: 1:TCLK/PCLK=1P5 \n" \
			   "[Ration]: 2:TCLK/PCLK=2 \n" \
			   "[Ration]: 3:TCLK/PCLK=3 \n" \
			   "[Ration]: 4:TCLK/PCLK=4 \n" \
			   "[Ration]: 5:TCLK/PCLK=0P75 \n",szArgv[0]);
		return 0;
	}

	u1arg1 = (UINT32)StrToInt(szArgv[1]);
	if (u1arg1>340000000)
	{
     return 0;
	}
	u1arg2 = (UINT8)StrToInt(szArgv[2]);
	u1arg3 = (UINT8)StrToInt(szArgv[3]);

	u1DrvTconMLDSVOPLLFreqSet(u1arg1,u1arg2,u1arg3);

	return 0;
}

static INT32 _TconEHInitCmd(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vErrorHandleInit();

    return 0;
}

static INT32 _TconEHStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vErrorHandleStatus();

    return 0;
}

static INT32 _TconEHFixEnable(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1;
	if (i4Argc != 2)
	{
		Printf("On: 1 / Off: 0\n",szArgv[0]);		   
		return 0;
	}
	u1arg1 = (UINT8)StrToInt(szArgv[1]);

    u1ErrorHandleFixEnable(u1arg1);

    return 0;
}

static INT32 _TconEHMuteEnable(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1;
	if (i4Argc != 2)
	{
		Printf("On: 1 / Off: 0\n",szArgv[0]);		   
		return 0;
	}
	u1arg1 = (UINT8)StrToInt(szArgv[1]);

    u1ErrorHandleMuteEnable(u1arg1);

    return 0;

}



static INT32 _TconEHSourceSet(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1;
	
	if (i4Argc != 2)
	{
		Printf("Usage: [mode] \n" \
			   "0: From OD (Test)\n" \
			   "1: From OSTG (default)n",szArgv[0]);
			   
        u1arg1 = u1ErrorHandlingSourceGet();
        Printf("== Current Source ==\n");    
        if (u1arg1 == 1)
        {
         Printf("[%d] FFrom OD (test)\n",u1arg1);
        }
        else if (u1arg1 == 0)
        {
         Printf("[%d] From OSTG (default)\n",u1arg1);
        }
			   
		return 0;
	}
	u1arg1 = (UINT8)StrToInt(szArgv[1]);

    u1ErrorHandlingSourceSet(u1arg1);    
    
    return 0;
}

static INT32 _TconIFSourceSet(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1;
	
	if (i4Argc != 2)
	{
   
		Printf("Usage: [mode] \n" \
			   "0: From TCON-Error handling\n" \
			   "1: From OD\n" \
			   "2: From SCPOS\n",szArgv[0]);

    u1arg1 = u1TCONIFSourceGet();
    Printf("== Current Source ==\n");    
    if (u1arg1 == 0)
    {
     Printf("[0] From TCON-Error handling\n");
    }
    else if (u1arg1 == 1)
    {
     Printf("[1] From OD\n");
    }
    else if (u1arg1 == 2)
    {
     Printf("[2] From SCPOS\n");
    }  
    
		return 0;
	}
	u1arg1 = (UINT8)StrToInt(szArgv[1]);

    u1TCONIFSourceSet(u1arg1);    
    
    return 0;
}

static INT32 _TconEPISLT(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    vDrvEPISLT();

    return 0;
}

static INT32 _TconMLVDSCRC(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1arg1,j,k;
	UINT32 u4freq,u4range;
    UNUSED(i4Argc);
    UNUSED(szArgv);

    u4freq=200000;
    u4range=120;
    vDrvSetSSWithCLICmd(1);
    DDDS_SetSpreadSpectrum(0, 0);
    x_thread_delay(1000);
    u1arg1 = vDrvMLVDSCRC();
    for (j = 1 ; j < 15 ; j++)//frequency
    {
        for (k = 1 ; k < 17 ; k++)//range
        {
            if(j<=9)
                u4freq = (j+1)*10000;
            else if(j==10)
                u4freq = 120000;
            else if(j==11)
                u4freq = 150000;
            else if(j==12)
                u4freq = 180000;
            else if(j==13)
                u4freq = 200000;
            else if(j==14)
                u4freq = 250000;
            if(k<=12)
                u4range = k*10;
            else if(k==13)
                u4range = 160;
            else if(k==14)
                u4range = 180;
            else if(k==15)
                u4range = 200;
            else if(k==16)
                u4range = 250;
   
            vDrvSetSSWithCLICmd(1);
            DDDS_SetSpreadSpectrum(u4freq, u4range);
            Printf("Pmx.s.s %d %d\n",u4freq,u4range);
			x_thread_delay(10);
            u1arg1 = vDrvMLVDSCRC();
            if (u1arg1 == 0)
                Printf("CRC Check NG\n");
            else
                Printf("CRC Check OK\n");
			
			Printf("\n");
        }
    }
    return 0;
}

static INT32 _TconEFUSECheck(INT32 i4Argc, const CHAR **szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

            if (u1DrvTconEfuseCheck())
                Printf("This IC support TCON function!!\n");
            else
                Printf("This IC do not support TCON function!!\n");
    return 0;
}
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for command entry

static CLI_EXEC_T _arTconEHCmdTbl[] =
{
    {"init", "i", _TconEHInitCmd, NULL, "Error Handling initial", CLI_SUPERVISOR},
    {"status", "s", _TconEHStatus, NULL, "Error Handling mute status", CLI_GUEST},
    {"fix function", "fix", _TconEHFixEnable, NULL, "Error Handling fix function enable", CLI_SUPERVISOR},
    {"mute function", "mute", _TconEHMuteEnable, NULL, "Error Handling mute function enable", CLI_SUPERVISOR},   
    {"EH source", "source", _TconEHSourceSet, NULL, "Error Handling source select", CLI_GUEST},       
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arTconIFCmdTbl[] =
{
    {"IF source", "source", _TconIFSourceSet, NULL, "IF source select", CLI_GUEST},       
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T _arTconEPICmdTbl[] =
{
    {"SLT", "slt", _TconEPISLT, NULL, "EPI loop back test", CLI_GUEST},       
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

static CLI_EXEC_T arTconCmdTbl[] = {
  {"Init", "i", _i32CliTconInit, NULL, "Tcon init", CLI_GUEST},
  {"offon", "pw", _i32CliTconOffOn, NULL, "Tcon off/on", CLI_GUEST},
  {"IsReady", "r", _i32CliTconIsReady, NULL, "Is Tcon ready", CLI_GUEST},
  {"Tp", "tp", _i32CliTconTp, NULL, "Tcon test pattern", CLI_GUEST},
  //{"RegDump", "dump", _i32CliTconRegDump, NULL, "Tcon reg dump", CLI_GUEST},
  //{"RegDiff", "diff", _i32CliTconRegDiff, NULL, "Tcon reg diff", CLI_GUEST},
  {"PanelCmd", "p", _i32CliTconPanelSet, NULL, "Tcon panel command", CLI_SUPERVISOR},  
  {"FrqCmd", "freq", _i32CliTconFreqSet, NULL, "Tcon vopll command", CLI_SUPERVISOR},     
  {"Error_handling", "eh",NULL, _arTconEHCmdTbl, "Error Handling parameters", CLI_GUEST},
  {"Interface", "if",NULL, _arTconIFCmdTbl, "Interface", CLI_SUPERVISOR},  
  {"EPI", "epi",NULL, _arTconEPICmdTbl, "EPI panel", CLI_SUPERVISOR},  
  {"MLVDS_CRC", "MLVDS_CRC", _TconMLVDSCRC, NULL, "MLVDS_CRC", CLI_GUEST},	 
  {"Efuse_Check", "efuse", _TconEFUSECheck, NULL, "EFUSE check", CLI_GUEST},  

  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

//static CLI_EXEC_T _rNetModCmdTbl =
CLI_MAIN_COMMAND_ITEM(Tcon)
{
	"tcon",
	NULL,
	NULL,
	arTconCmdTbl,
	"Tcon command",
	CLI_GUEST
};


