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
 * $RCSfile: nptv_hdmi_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/


#include "x_os.h"
#include "x_stl_lib.h"
#include "video_def.h"
#include "vdp_drvif.h"
#include "util.h"
#include "hdmi_debug.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"

#include "source_table.h"
#include "sv_const.h"
#include "drv_hdmi2.h"
#include "drv_hdmi2com.h"
#include "fbm_drvif.h"
#include "drv_hdmi.h"

#include "drvcust_if.h"

#ifndef CC_CLI
#define CC_CLI
#endif

//#ifdef CC_CLI

#define DEBUG_CHANNEL_BYTE_COUNT (128*128*2*4)
#define DEBUG_CHANNEL_WORD_COUNT (128*128*2)
#define DEBUG_MAX_COUNT (1000000)
/* Macro */
#define ABSDIFF(a,b) ((a)>(b)?((a)-(b)):((b)-(a)))
/* Extern Variables*/
extern E_HDMI_SWITCH_NUM eActiveHdmiPort;
/* Type Define */
typedef struct
{
    UINT32* _pu4DebugBufWP_CH0Even;
    UINT32* _pu4DebugBufWP_CH0Odd;
    UINT32* _pu4DebugBufWP_CH1Even;
    UINT32* _pu4DebugBufWP_CH1Odd;
    UINT32* _pu4DebugBufWP_CH2Even;
    UINT32* _pu4DebugBufWP_CH2Odd;
    INT8 s1Par1;
    INT8 s1Par2;
    INT8 s1Par1Min;
    INT8 s1Par1Max;
    INT8 s1Par2Min;
    INT8 s1Par2Max;
}HDMI2ANA_DEBUG_PARA;
/* Static Variables */
static UINT32 _u4DebugBufSize = 0;
static UINT32* _pu4DebugBufVirtualAddr = NULL;
static HDMI2ANA_DEBUG_PARA tDebugPara;

static BOOL fgFirstSetAll = TRUE;
static BOOL fgConControlOn = FALSE;
static INT8 s1ConControl = -64;
static UINT8 u1LastConSel = 0;


/* Functions for HDMI */
static INT32 _u1vHDMI2Init(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2MainLoop(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2DebugMessage(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2StopFw(INT32 i4Argc,const CHAR **szArgv);
static INT32 _u1vHDMI2ResumeFw(INT32 i4Argc,const CHAR **szArgv);
static INT32 _u1vHDMI2AnaInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaReset(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaSetConMon(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaGetCDRStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaGetAlgMonStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaFixEQ(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaScanEQ(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaGetEQCtrl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaAllocDebugBuf(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaDoDebug(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaLogDebugStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaDelay(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaCrc(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaRstb(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaEQBand(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaPLL(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaDeepPLL(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaGetAllStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaSetAll(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaTestAll(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaOscCheck(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaFunctionOpt(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2AnaConTest(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2DigInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2HDCP2Init(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2DdcSel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2ScrambleSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2CRC(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2SplitSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2420Set(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2DdcSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2HpdSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2ClkSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2NewDigSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2DigitalPhyRst(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2DeepColorRst(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2FifoRst(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2Hdcp1Rst(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2Hdcp2Rst(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2APLLSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2DPLLSet(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2GetClockRate(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2GetVideoStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2GetAudioStatus(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2GetHdcp14Status(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2GetHdcp22Status(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2GetInfoframe(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMI2Get5vStatus(INT32 i4Argc, const CHAR **szArgv);

//#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for HDMI command entry
CLI_EXEC_T arHDMI2AnaComCmdTbl[] = {
  {"Init", "i", _u1vHDMI2AnaInit, NULL, "HDMI analog initialization", CLI_SUPERVISOR},
  {"Set", "s", _u1vHDMI2AnaSet, NULL, "HDMI analog setting", CLI_SUPERVISOR},
  {"Reset", "reset", _u1vHDMI2AnaReset, NULL, "HDMI analog reset (0:Data,1:EQALG,2:SAOSC,3:EQOSC,4:CDR,5:LEQ,6:EQ_Increase,7:Adjust) is40xMode", CLI_SUPERVISOR},
  {"Setcm", "scm", _u1vHDMI2AnaSetConMon, NULL, "HDMI analog set con monitor", CLI_SUPERVISOR},
  {"Getcs", "gcs", _u1vHDMI2AnaGetCDRStatus, NULL, "HDMI analog get CDR status", CLI_SUPERVISOR},
  {"Getam", "gam", _u1vHDMI2AnaGetAlgMonStatus, NULL, "HDMI analog get alg monitor status", CLI_SUPERVISOR},
  {"Fixeq", "feq", _u1vHDMI2AnaFixEQ, NULL, "HDMI analog fix LEQ", CLI_SUPERVISOR},
  {"Scaneq", "seq", _u1vHDMI2AnaScanEQ, NULL, "HDMI analog scan LEQ", CLI_SUPERVISOR},
  {"Geteq", "geq", _u1vHDMI2AnaGetEQCtrl, NULL, "HDMI analog get EQ control", CLI_SUPERVISOR},
  {"Debug", "dbg", _u1vHDMI2AnaDoDebug, NULL, "HDMI analog do debug", CLI_SUPERVISOR},
  {"Debugbuf", "dbgbuf", _u1vHDMI2AnaAllocDebugBuf, NULL, "HDMI analog set debug buffer", CLI_SUPERVISOR},
  {"Debuglog", "dbglog", _u1vHDMI2AnaLogDebugStatus, NULL, "HDMI analog log debug status", CLI_SUPERVISOR},
  {"Delay", "delay", _u1vHDMI2AnaDelay, NULL, "HDMI analog set delay", CLI_SUPERVISOR},
  {"Rstb", "rstb", _u1vHDMI2AnaRstb, NULL, "HDMI analog set reset bit", CLI_SUPERVISOR},
  {"Crc", "crc", _u1vHDMI2AnaCrc, NULL, "HDMI analog set delay", CLI_SUPERVISOR},
  {"Eqband", "eb", _u1vHDMI2AnaEQBand, NULL, "HDMI analog set EQ band", CLI_SUPERVISOR},
  {"PLL", "pll", _u1vHDMI2AnaPLL, NULL, "HDMI analog set PLL", CLI_SUPERVISOR},
  {"DeepPLL", "dpll", _u1vHDMI2AnaDeepPLL, NULL, "HDMI analog set deep PLL", CLI_SUPERVISOR},
  {"Getas", "gas", _u1vHDMI2AnaGetAllStatus, NULL, "HDMI analog get all status", CLI_SUPERVISOR},
  {"Seta", "sa", _u1vHDMI2AnaSetAll, NULL, "HDMI analog set all", CLI_SUPERVISOR},
  {"Testa", "ta", _u1vHDMI2AnaTestAll, NULL, "HDMI analog test all(set & get status)", CLI_SUPERVISOR},
  {"Oscc", "oc", _u1vHDMI2AnaOscCheck, NULL, "HDMI analog offset cal check", CLI_SUPERVISOR},
  {"FuncOpt", "fo", _u1vHDMI2AnaFunctionOpt, NULL, "HDMI analog optional function sel", CLI_SUPERVISOR},
  {"Cont", "ct", _u1vHDMI2AnaConTest, NULL, "HDMI analog con test)", CLI_SUPERVISOR},
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_EXEC_T arHDMI2DigComCmdTbl[] = {
  {"Init", "i", _u1vHDMI2DigInit, NULL, "HDMI digital initialization", CLI_SUPERVISOR},
  {"Hdcp2", "hdcp2", _u1vHDMI2HDCP2Init, NULL, "HDMI HDCP2 initialization", CLI_SUPERVISOR},
  {"ddcsel", "sel", _u1vHDMI2DdcSel, NULL, "HDMI DDC Select", CLI_SUPERVISOR},
  {"Scramble", "scrm", _u1vHDMI2ScrambleSet, NULL, "HDMI Scrambling setting", CLI_SUPERVISOR},
  {"Split", "slt", _u1vHDMI2SplitSet, NULL, "HDMI split setting", CLI_SUPERVISOR},
  {"420", "420", _u1vHDMI2420Set, NULL, "HDMI 420 set", CLI_SUPERVISOR},
  {"CRC", "c", _u1vHDMI2CRC, NULL, "HDMI CRC check", CLI_SUPERVISOR},
  {"DDC", "ddc", _u1vHDMI2DdcSet, NULL, "HDMI DDC set", CLI_SUPERVISOR},
  {"Hpd", "hpd", _u1vHDMI2HpdSet, NULL, "HDMI hpd set", CLI_SUPERVISOR},
  {"clk", "clk", _u1vHDMI2ClkSet, NULL, "HDMI 2.0 port clock set", CLI_SUPERVISOR},
  {"new digital", "dig", _u1vHDMI2NewDigSet, NULL, "HDMI 2.0 new digital select", CLI_SUPERVISOR},
  {"phyrst", "dyrst", _u1vHDMI2DigitalPhyRst, NULL, "HDMI digital phy reset", CLI_SUPERVISOR},
  {"deeprst", "deeprst", _u1vHDMI2DeepColorRst, NULL, "HDMI deep color reset", CLI_SUPERVISOR},
  {"fiforst", "fiforst", _u1vHDMI2FifoRst, NULL, "HDMI fifo reset", CLI_SUPERVISOR},
  {"Hdcprst", "hdcp1rst", _u1vHDMI2Hdcp1Rst, NULL, "HDMI hdcp1 reset", CLI_SUPERVISOR},
  {"Hdcprst", "hdcp2rst", _u1vHDMI2Hdcp2Rst, NULL, "HDMI hdcp2 reset", CLI_SUPERVISOR},
  {"Apll", "apll", _u1vHDMI2APLLSet, NULL, "HDMI apll reset", CLI_SUPERVISOR},
  {"Dpll", "dpll", _u1vHDMI2DPLLSet, NULL, "HDMI dpll reset", CLI_SUPERVISOR},
  {"Getcr", "gcr", _u1vHDMI2GetClockRate, NULL, "HDMI get tmds clock rate", CLI_SUPERVISOR},
  {"Getvs", "gvs", _u1vHDMI2GetVideoStatus, NULL, "HDMI video format", CLI_SUPERVISOR},
  {"Getas", "gas", _u1vHDMI2GetAudioStatus, NULL, "HDMI audio format", CLI_SUPERVISOR},
  {"Gethdcp1", "ghdcp14", _u1vHDMI2GetHdcp14Status, NULL, "HDMI get HDCP1.4 status", CLI_SUPERVISOR},
  {"Gethdcp2", "ghdcp22", _u1vHDMI2GetHdcp22Status, NULL, "HDMI get HDCP2.2 status", CLI_SUPERVISOR},
  {"Getinfo", "ginfo", _u1vHDMI2GetInfoframe, NULL, "HDMI get infoframe infomation", CLI_SUPERVISOR}, 
  {"Get5v", "g5v", _u1vHDMI2Get5vStatus, NULL, "HDMI get 5v status", CLI_SUPERVISOR},
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLIMOD_DEBUG_FUNCTIONS(HDMI)
CLI_EXEC_T arHDMI2CmdTbl[] = {
	CLIMOD_DEBUG_CLIENTRY(HDMI),
  {"Init", "i", _u1vHDMI2Init, NULL, "HDMI initialization", CLI_SUPERVISOR},
  {"Mainloop", "m", _u1vHDMI2MainLoop, NULL, "HDMI mainloop", CLI_SUPERVISOR},
  {"DebugMessage", "dmsg", _u1vHDMI2DebugMessage, NULL, "HDMI mainloop", CLI_SUPERVISOR},
  {"Stop fw", "stop", _u1vHDMI2StopFw, NULL, "HDMI mainloop", CLI_SUPERVISOR},
  {"Resume fw", "resume", _u1vHDMI2ResumeFw, NULL, "HDMI mainloop", CLI_SUPERVISOR},
  {"Anacom", "acom", NULL, arHDMI2AnaComCmdTbl, "HDMI analog common code test", CLI_SUPERVISOR},
  {"Digcom", "dcom", NULL, arHDMI2DigComCmdTbl, "HDMI digital common code test", CLI_SUPERVISOR},
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
CLI_MAIN_COMMAND_ITEM(Hdmi)
{
    "hdmi", NULL, NULL, arHDMI2CmdTbl, "HDMI test", CLI_GUEST
};

//#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _u1vHDMI2Init(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	vHDMI2Init();

	Printf("vHDMI2Init\n");

	return 0;
}

static INT32 _u1vHDMI2MainLoop(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	vHDMI2MainLoop();

	Printf("vHDMI2MainLoop\n");
	return 0;
}

static INT32 _u1vHDMI2DebugMessage(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	vHDMI2PrintHdmiStatus(4);

	return 0;
}
extern E_HDMI_SWITCH_NUM eActiveHdmiPort;
static INT32 _u1vHDMI2StopFw(INT32 i4Argc,const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}
	vHDMI2StopFw((HdmiRxPort)eActiveHdmiPort);
	return 0;
}

static INT32 _u1vHDMI2ResumeFw(INT32 i4Argc,const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}
	vHDMI2ResumeFw((HdmiRxPort)eActiveHdmiPort);
	return 0;
}
static INT32 _u1vHDMI2AnaInit(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

    vHDMI2ComAnaInit(u2HDMIAnaBaseAddr);
    Printf("HDMI analog init done\n");

    return 0;
}

static HdmiTmdsClkRate_t _u1vTransClockToBand(UINT16 u2Clock)
{
    HdmiTmdsClkRate_t eBand = HDMI_TMDS_CLOCK_UNKNOW;
    if(u2Clock <= 25)
    {
        eBand = HDMI_TMDS_CLOCK_25;
    }
    if((u2Clock > 25) && (u2Clock <= 35))
    {
        eBand = HDMI_TMDS_CLOCK_25_35;
    }
    if((u2Clock > 35) && (u2Clock <= 40))
    {
        eBand = HDMI_TMDS_CLOCK_35_40;
    }
    if((u2Clock > 40) && (u2Clock <= 50))
    {
        eBand = HDMI_TMDS_CLOCK_40_50;
    }
    if((u2Clock > 50) && (u2Clock <= 80))
    {
        eBand = HDMI_TMDS_CLOCK_50_80;
    }
    if((u2Clock > 80) && (u2Clock <= 100))
    {
        eBand = HDMI_TMDS_CLOCK_80_100;
    }
    if((u2Clock > 100) && (u2Clock <= 120))
    {
        eBand = HDMI_TMDS_CLOCK_100_120;
    }
    if((u2Clock > 120) && (u2Clock <= 140))
    {
        eBand = HDMI_TMDS_CLOCK_120_140;
    }
    if((u2Clock > 140) && (u2Clock <= 200))
    {
        eBand = HDMI_TMDS_CLOCK_140_200;
    }
    if((u2Clock > 200) && (u2Clock <= 300))
    {
        eBand = HDMI_TMDS_CLOCK_200_300;
    }

    return eBand;
}

static INT32 _u1vHDMI2AnaSet(INT32 i4Argc, const CHAR **szArgv)
{
    BOOL b40xMode = TRUE;
    HdmiTmdsClkRate_t eBand = HDMI_TMDS_CLOCK_UNKNOW;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if (i4Argc < 2)
	{
		Printf("Usage: %s is40xMode\n", szArgv[0]);
		return 0;
	}
    eBand = bHDMI2ComGetClkRate(0);
    Printf("eband is %d\n",eBand);
    if(StrToInt(szArgv[1]))
    {
        b40xMode = TRUE;
    }
    else
    {
        b40xMode = FALSE;
    }
    vHDMI2ComAnaSetting(u2HDMIAnaBaseAddr,eBand,b40xMode);
    Printf("HDMI analog set done\n");

    return 0;
}

#define HDMI2_ANA_RESET_ITEM_COUNT 8
static INT32 _u1vHDMI2AnaReset(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1ResetItem = HDMI2_ANA_RESET_ITEM_COUNT;
    HDMI2ANA_LEQ_ITEM tLeqItem;
    BOOL b40xMode = TRUE;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if (i4Argc < 3)
	{
		Printf("Usage: %s (0:Data,1:EQALG,2:SAOSC,3:EQOSC,4:CDR,5:LEQ,6:EQ_Increase,7:Adjust) is40xMode\n", szArgv[0]);
		return 0;
	}
    u1ResetItem = (UINT8)StrToInt(szArgv[1]);
    if(StrToInt(szArgv[2]))
    {
        b40xMode = TRUE;
    }
    else
    {
        b40xMode = FALSE;
    }
    if(u1ResetItem >= HDMI2_ANA_RESET_ITEM_COUNT)
    {
        Printf("Reset item is %d(>=max item count%d),do nothing!\n",u1ResetItem,HDMI2_ANA_RESET_ITEM_COUNT);
        return 0;
    }
    if(u1ResetItem == 0)
    {
        vHDMI2ComAnaDataReset(u2HDMIAnaBaseAddr);
    }
    if(u1ResetItem == 1)
    {
        vHDMI2ComAnaEQALGReset(u2HDMIAnaBaseAddr);
    }
    if(u1ResetItem == 2)
    {
        vHDMI2ComAnaSAOSCReset(u2HDMIAnaBaseAddr);
    }
    if(u1ResetItem == 3)
    {
        vHDMI2ComAnaEQOSCReset(u2HDMIAnaBaseAddr);
    }
    if(u1ResetItem == 4)
    {
        vHDMI2ComAnaCDRReset(u2HDMIAnaBaseAddr);
    }
    if(u1ResetItem == 5)
    {
        vHDMI2ComAnaLeqItemGet(&tLeqItem);
        vHDMI2ComAnaEQReset(u2HDMIAnaBaseAddr,b40xMode,&tLeqItem);
    }
    if(b40xMode)
    {
        if(u1ResetItem == 6)
        {
            vHDMI2ComAnaEQIncreaseReset(u2HDMIAnaBaseAddr);
        }
        if(u1ResetItem == 7)
        {
            if(bHDMI2ComAnaAdjustRatioSet(u2HDMIAnaBaseAddr))
            {
                vHDMI2ComAnaAdjustReset(u2HDMIAnaBaseAddr);
            }
        }
    }
    Printf("HDMI analog reset done\n");
    return 0;
}

static INT32 _u1vHDMI2AnaSetConMon(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1MonSel = 0;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if (i4Argc < 2)
	{
		Printf("Usage: %s monsel\n", szArgv[0]);
		return 0;
	}
    u1MonSel = (UINT8)StrToInt(szArgv[1]);
    vHDMI2ComAnaSetConMon(u2HDMIAnaBaseAddr,u1MonSel);

    Printf("HDMI analog set con monitor done\n");
    return 0;
}

static INT32 _u1vHDMI2AnaGetCDRStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1CDRLpo = 0;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}
    u1CDRLpo = bHDMI2ComAnaGetCDRLPO(u2HDMIAnaBaseAddr,HDMI_TMDS_DATA_CHANNEL_0);
    Printf("CH0~2 CDRLPO:%d--",u1CDRLpo);
    u1CDRLpo = bHDMI2ComAnaGetCDRLPO(u2HDMIAnaBaseAddr,HDMI_TMDS_DATA_CHANNEL_1);
    Printf("%d--",u1CDRLpo);
    u1CDRLpo = bHDMI2ComAnaGetCDRLPO(u2HDMIAnaBaseAddr,HDMI_TMDS_DATA_CHANNEL_2);
    Printf("%d\n",u1CDRLpo);

    return 0;
}

static INT8 _s1TransSignedValue(UINT8 u1Value, UINT8 u1TotalBit)
{
    INT8 s1SignedValue = 0;
    UINT8 u1MaxValue = 0;
    if(u1TotalBit >= 8)
    {
        return (INT8)u1Value;
    }
    u1MaxValue = (0xff)>>(8-u1TotalBit);
    if(u1Value > (u1MaxValue >> 1))
    {
        s1SignedValue = u1Value - u1MaxValue; 
    }
    else
    {
        s1SignedValue = u1Value;
    }

    return s1SignedValue;
}

static UINT8 _u1TransUnsignedValue(INT8 s1Value, UINT8 u1TotalBit)
{
	UINT8 u1UnsignedValue = 0;
    INT8 s1MaxValue = 0;
	if(u1TotalBit >= 8)
	{
		return (UINT8)s1Value;
	}
    s1MaxValue = (0xff)>>(8-u1TotalBit);
	if(s1Value < 0)
	{
		u1UnsignedValue = (UINT8)(s1MaxValue + s1Value + 1);
	}
	else
	{
		u1UnsignedValue = (UINT8)s1Value;
	}

    return u1UnsignedValue;
}

static INT32 _u1vHDMI2AnaOscCheck(INT32 i4Argc, const CHAR **szArgv)
{
    INT8 as1MaxValue[3][16],as1MinValue[3][16],s1MonStatus;
    UINT8 u1TotalBit,u1MonStatus;
    UINT8 au1MaxEQCtrl[3],au1MinEQCtrl[3],u1EQCtrl,u1CrcTimes,u1SAOSC_LFSEL,u1EQOSC_LFSEL;
    UINT16 u2CheckTimes,i,j,k;
    BOOL b40xMode = TRUE;
    UINT16 u2HDMIAnaBaseAddr;
	BOOL bStopCrcFail,bVerBose;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    for(i=0; i<3; i++)
    {
        for(j=0; j<16; j++)
        {
            as1MaxValue[i][j] = -64;
            as1MinValue[i][j] = 63;
        }
        au1MaxEQCtrl[i] = 0;
        au1MinEQCtrl[i] = 63;
    }
    if (i4Argc < 6)
	{
		Printf("Usage: %s is40xMode check_times crc_times bStopCrcFail bVerBose\n", szArgv[0]);
		return 0;
	}
    if((i4Argc > 6) && (i4Argc < 8))
    {
        Printf("Usage: %s is40xMode check_times crc_times bStopCrcFail bVerBose [u1SAOSC_LFSEL u1EQOSC_LFSEL]\n", szArgv[0]);
		return 0;
    }
    if(i4Argc >= 8)
    {
        u1SAOSC_LFSEL = (UINT8)StrToInt(szArgv[6]);
        u1EQOSC_LFSEL = (UINT8)StrToInt(szArgv[7]);
        vHDMI2ComAnaLFSelValue_Set(u1SAOSC_LFSEL,u1EQOSC_LFSEL);
    }
    u2CheckTimes = (UINT16)StrToInt(szArgv[2]);
    u1CrcTimes = (UINT8)StrToInt(szArgv[3]);
	bStopCrcFail = (BOOL)StrToInt(szArgv[4]);
	bVerBose = (BOOL)StrToInt(szArgv[5]);
    if(StrToInt(szArgv[1]))
    {
        b40xMode = TRUE;
    }
    else
    {
        b40xMode = FALSE;
    }
    for(i=0;i<u2CheckTimes;i++)
    {
        CLI_Parser_Arg("n.hdmi2.acom.sa %d",b40xMode);
        if(!bStopCrcFail)
        {
            CLI_Parser_Arg("n.hdmi2.acom.gas %d %d",u1CrcTimes,bVerBose);
        }
        else
        {
            CLI_Parser_Arg("n.hdmi2.acom.gas 0 %d",bVerBose);
            if(!(fgHDMI2ComCRC(0,u1CrcTimes)))
            {
                break;
            }
        }
        vHDMI2ComAnaDebug1Ctrl_EN(u2HDMIAnaBaseAddr,1);
        vHDMI2ComAnaDebug2Enable(u2HDMIAnaBaseAddr);
        vHDMI2ComAnaDebug1Ctrl_EN(u2HDMIAnaBaseAddr,0);
        for(j=0; j<16; j++)
        {
            if(9 == j)
            {
                u1TotalBit = 5;
            }
            else
            {
                u1TotalBit = 7;
            }
            for(k=0; k<3; k++)
            {
                u1MonStatus = bHDMI2ComAnaGetAlgMonStatus(u2HDMIAnaBaseAddr,j,k);
                s1MonStatus = _s1TransSignedValue(u1MonStatus,u1TotalBit);
                if(s1MonStatus > as1MaxValue[k][j])
                {
                    as1MaxValue[k][j] = s1MonStatus;
                }
                if(s1MonStatus < as1MinValue[k][j])
                {
                    as1MinValue[k][j] = s1MonStatus;
                }
            }
        }
        for(j=0; j<3; j++)
        {
            u1EQCtrl = bHDMI2ComAnaGetEQCtrl(u2HDMIAnaBaseAddr,j);
            if(u1EQCtrl > au1MaxEQCtrl[j])
            {
                au1MaxEQCtrl[j] = u1EQCtrl;
            }
            if(u1EQCtrl < au1MinEQCtrl[j])
            {
                au1MinEQCtrl[j] = u1EQCtrl;
            }
        }
    }
    for(i=0; i<3; i++)
    {
        Printf("CH%d_max: ",i);
        for(j=0; j<16; j++)
        {
            Printf("%-3d\t",as1MaxValue[i][j]);
        }
        Printf("%-3d\t",au1MaxEQCtrl[i]);
        Printf("\n");
        Printf("CH%d_min: ",i);
        for(j=0; j<16; j++)
        {
            Printf("%-3d\t",as1MinValue[i][j]);
        }
        Printf("%-3d\t",au1MinEQCtrl[i]);
        Printf("\n");
    }

    return 0;
}

static INT32 _u1vHDMI2AnaFunctionOpt(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Func;
    BOOL fgEnable;
    if (i4Argc < 3)
    {
        Printf("Usage: %s func(0:3G HDMI20 support, 1:Force 40x mode) enable\n", szArgv[0]);
        return 0;
    }
    u1Func = (UINT8)StrToInt(szArgv[1]);
    fgEnable = (BOOL)StrToInt(szArgv[2]);
    switch(u1Func)
    {
        case 0:
            vHDMI2ComAnaSupport3GHDMI20(fgEnable);
            break;
        case 1:
            vHDMI2ComAnaForce40xMode(fgEnable);
            break;
        default:
            break;
    }

    return 0;
}

static INT32 _u1vHDMI2AnaConTest(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2HDMIAnaBaseAddr;
	UINT8 u1ConSel,u1ConValue,i;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
	if(i4Argc < 2)
	{
		Printf("Usage: %s Consel\n", szArgv[0]);
		return 0;
	}
	if(!fgConControlOn)
	{
		vHDMI2ComAnaConTestSet(u2HDMIAnaBaseAddr);
		fgConControlOn = TRUE;
	}
	u1ConSel = (UINT8)StrToInt(szArgv[1]);
	if(u1LastConSel != u1ConSel)
	{
		s1ConControl = -64;
		u1LastConSel = u1ConSel;
		for(i=0; i<3; i++)
		{
			if(i != u1ConSel)
			{
				vHDMI2ComAnaConControl(u2HDMIAnaBaseAddr,0,i);
			}
		}
	}
	Printf("Current s1ConControl:%d\n",s1ConControl);
	if(s1ConControl > 63)
	{
		s1ConControl = 63;
	}
	u1ConValue = _u1TransUnsignedValue(s1ConControl,7);
	vHDMI2ComAnaConControl(u2HDMIAnaBaseAddr,u1ConValue,u1ConSel);
	s1ConControl += 8;
	

	return 0;
}


static INT32 _u1vHDMI2AnaGetAlgMonStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1MonSel_Min = 0;
    UINT8 u1MonSel_Max = 0xf;
    UINT8 i;
    UINT8 u1MonStatus = 0;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if (i4Argc >= 2)
	{
		u1MonSel_Min = (UINT8)StrToInt(szArgv[1]);
        u1MonSel_Max = u1MonSel_Min;
	}

    for(i=u1MonSel_Min; i<=u1MonSel_Max; i++)
    {
        u1MonStatus = bHDMI2ComAnaGetAlgMonStatus(u2HDMIAnaBaseAddr,i,HDMI_TMDS_DATA_CHANNEL_0);
        Printf("Mon%-2d_CH0~2 Status:(%-3d)",i,u1MonStatus);
        u1MonStatus = bHDMI2ComAnaGetAlgMonStatus(u2HDMIAnaBaseAddr,i,HDMI_TMDS_DATA_CHANNEL_1);
        Printf("(%-3d)",u1MonStatus);
        u1MonStatus = bHDMI2ComAnaGetAlgMonStatus(u2HDMIAnaBaseAddr,i,HDMI_TMDS_DATA_CHANNEL_2);
        Printf("(%-3d)\n",u1MonStatus);
    }
    
    return 0;
}
//Fix EQ
static INT32 _u1vHDMI2AnaFixEQ(INT32 i4Argc, const CHAR **szArgv)
{
    BOOL bFix;
    UINT8 u1Eq;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if (i4Argc < 3)
    {
        Printf("Usage: %s fixeq(0:Not fix,1:Fix) eqvalue\n", szArgv[0]);
		return 0;
    }
    if(StrToInt(szArgv[1]))
    {
        bFix = TRUE;
    }
    else
    {
        bFix = FALSE;
    }
    u1Eq = (UINT8)StrToInt(szArgv[2]);
    if(u1Eq > 15)
    {
        Printf("Fixeq must be <= 15!\n");
        return 0;
    }
    vHDMI2ComAnaFixEQ(u2HDMIAnaBaseAddr,bFix,u1Eq);

    return 0;
}

static INT32 _u1vHDMI2AnaScanEQ(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 i = 0;
    if (i4Argc != 1)
    {
        Printf("Usage: %s\n", szArgv[0]);
		return 0;
    }
    for(i=0; i<=15; i++)
    {
        Printf("EQCtrl:%-2d ",i);
        CLI_Parser_Arg("n.hdmi2.acom.feq 1 %d",i);
        CLI_Parser_Arg("n.hdmi2.acom.gas 100 0");
    }

    return 0;
}

static INT32 _u1vHDMI2AnaGetEQCtrl(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1EQCtrl = 0;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}
    u1EQCtrl = bHDMI2ComAnaGetEQCtrl(u2HDMIAnaBaseAddr,HDMI_TMDS_DATA_CHANNEL_0);
    Printf("CH0~2 EQCtrl:%d--",u1EQCtrl);
    u1EQCtrl = bHDMI2ComAnaGetEQCtrl(u2HDMIAnaBaseAddr,HDMI_TMDS_DATA_CHANNEL_1);
    Printf("%d--",u1EQCtrl);
    u1EQCtrl = bHDMI2ComAnaGetEQCtrl(u2HDMIAnaBaseAddr,HDMI_TMDS_DATA_CHANNEL_2);
    Printf("%d\n",u1EQCtrl);

    return 0;
}

static INT32 _u1vHDMI2AnaAllocDebugBuf(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Size;
    UINT8 u1BufChannel = 0;
    
    if (i4Argc < 2)
	{
		Printf("Usage: %s buf_channel(1~3)\n", szArgv[0]);
		return 0;
	}
    u1BufChannel = (UINT8)StrToInt(szArgv[1]);
    if(0 == u1BufChannel)
    {
        u1BufChannel = 1;
    }
    if(u1BufChannel > 3)
    {
        u1BufChannel = 3;
    }
    if(_pu4DebugBufVirtualAddr != NULL)
    {
        x_mem_free(_pu4DebugBufVirtualAddr);
        _pu4DebugBufVirtualAddr = NULL;
    }
    u4Size = u1BufChannel * DEBUG_CHANNEL_BYTE_COUNT;
    _pu4DebugBufVirtualAddr = (UINT32 *)x_mem_alloc(u4Size);
    if(NULL == _pu4DebugBufVirtualAddr)
    {
        Printf("Error:Allocate buffer fail!\n");
        return -1;
    }
    _u4DebugBufSize = u4Size;
    Printf("Buffer Virtual addr:0x%x, size: 0x%x\n", (UINT32)_pu4DebugBufVirtualAddr,_u4DebugBufSize);
        
    return 0;
}

static void _u1vHDMI2AnaResetDebugWP(void)
{
    tDebugPara._pu4DebugBufWP_CH0Even = _pu4DebugBufVirtualAddr;
    tDebugPara._pu4DebugBufWP_CH0Odd = tDebugPara._pu4DebugBufWP_CH0Even + DEBUG_CHANNEL_WORD_COUNT/2;
    if(_u4DebugBufSize >= 2*DEBUG_CHANNEL_BYTE_COUNT)
    {
        tDebugPara._pu4DebugBufWP_CH1Even = _pu4DebugBufVirtualAddr + DEBUG_CHANNEL_WORD_COUNT;
        tDebugPara._pu4DebugBufWP_CH1Odd = tDebugPara._pu4DebugBufWP_CH1Even + DEBUG_CHANNEL_WORD_COUNT/2;
    }
    if(_u4DebugBufSize >= 3*DEBUG_CHANNEL_BYTE_COUNT)
    {
        tDebugPara._pu4DebugBufWP_CH2Even = _pu4DebugBufVirtualAddr + 2*DEBUG_CHANNEL_WORD_COUNT;
        tDebugPara._pu4DebugBufWP_CH2Odd = tDebugPara._pu4DebugBufWP_CH2Even + DEBUG_CHANNEL_WORD_COUNT/2;
    }

    return;
}

static BOOL _u1vHDMI2AnaInitDebugBuf(void)
{
    if(NULL == _pu4DebugBufVirtualAddr)
    {
        Printf("Please alloc debug buffer!\n");
        return FALSE;
    }
    x_memset((void*)_pu4DebugBufVirtualAddr,0,_u4DebugBufSize);
    _u1vHDMI2AnaResetDebugWP();

    return TRUE;
}

static void _u1vHDMI2AnaRecordDebugStatus(void)
{
    UINT8 j;
    HdmiTmdsDataChannel_t channel = HDMI_TMDS_DATA_CHANNEL_0;
    UINT32 u4DebugStatusEven_Low, u4DebugStatusEven_High, u4DebugStatusEven;
    UINT32 u4DebugStatusOdd_Low, u4DebugStatusOdd_High, u4DebugStatusOdd;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);

    j = _u4DebugBufSize/(DEBUG_CHANNEL_BYTE_COUNT);
    while((j > 0) && (channel <= HDMI_TMDS_DATA_CHANNEL_2))
    {
        u4DebugStatusEven_Low = (UINT32)wHDMI2ComAnaGetDebug1Status(u2HDMIAnaBaseAddr,1,2,channel);
        u4DebugStatusEven_High = (UINT32)wHDMI2ComAnaGetDebug1Status(u2HDMIAnaBaseAddr,1,0,channel);
        u4DebugStatusEven = u4DebugStatusEven_Low + (u4DebugStatusEven_High << 10);
        
        u4DebugStatusOdd_Low = (UINT32)wHDMI2ComAnaGetDebug1Status(u2HDMIAnaBaseAddr,0,2,channel);
        u4DebugStatusOdd_High = (UINT32)wHDMI2ComAnaGetDebug1Status(u2HDMIAnaBaseAddr,0,0,channel);
        u4DebugStatusOdd = u4DebugStatusOdd_Low + (u4DebugStatusOdd_High << 10);

        if(HDMI_TMDS_DATA_CHANNEL_0 == channel)
        {
            *tDebugPara._pu4DebugBufWP_CH0Even += u4DebugStatusEven;
            *tDebugPara._pu4DebugBufWP_CH0Odd += u4DebugStatusOdd;
            tDebugPara._pu4DebugBufWP_CH0Even++;
            tDebugPara._pu4DebugBufWP_CH0Odd++;
            
        }
        if(HDMI_TMDS_DATA_CHANNEL_1 == channel)
        {
            *tDebugPara._pu4DebugBufWP_CH1Even += u4DebugStatusEven;
            *tDebugPara._pu4DebugBufWP_CH1Odd += u4DebugStatusOdd;
            tDebugPara._pu4DebugBufWP_CH1Even++;
            tDebugPara._pu4DebugBufWP_CH1Odd++;
        }
        if(HDMI_TMDS_DATA_CHANNEL_2 == channel)
        {
            *tDebugPara._pu4DebugBufWP_CH2Even += u4DebugStatusEven;
            *tDebugPara._pu4DebugBufWP_CH2Odd += u4DebugStatusOdd;
            tDebugPara._pu4DebugBufWP_CH2Even++;
            tDebugPara._pu4DebugBufWP_CH2Odd++;
        }
        channel++;
        j--;
    }

    return;
}

static BOOL _u1vHDMI2AnaDebugSetSkipValue(INT8 s1Par1, INT8 s1Par2, UINT32 u4SkipValue)
{
    if((s1Par1 > tDebugPara.s1Par1Max) || (s1Par1 < tDebugPara.s1Par1Min) || 
        (s1Par2 > tDebugPara.s1Par2Max) || (s1Par2 < tDebugPara.s1Par1Min))
    {
        *tDebugPara._pu4DebugBufWP_CH0Even += u4SkipValue;
        *tDebugPara._pu4DebugBufWP_CH0Odd += u4SkipValue;
        tDebugPara._pu4DebugBufWP_CH0Even++;
        tDebugPara._pu4DebugBufWP_CH0Odd++;
        *tDebugPara._pu4DebugBufWP_CH1Even += u4SkipValue;
        *tDebugPara._pu4DebugBufWP_CH1Odd += u4SkipValue;
        tDebugPara._pu4DebugBufWP_CH1Even++;
        tDebugPara._pu4DebugBufWP_CH1Odd++;
        *tDebugPara._pu4DebugBufWP_CH2Even += u4SkipValue;
        *tDebugPara._pu4DebugBufWP_CH2Odd += u4SkipValue;
        tDebugPara._pu4DebugBufWP_CH2Even++;
        tDebugPara._pu4DebugBufWP_CH2Odd++;

        return TRUE;
    }

    return FALSE;
}

static INT8 _u1vHDMI2AnaDebugStepPar(INT8 s1Par, INT8 s1ParMin, INT8 s1Step)
{
    INT8 s1Tmp;
    if(ABSDIFF(s1Par,s1ParMin) > s1Step)
    {
        if(s1Par > s1ParMin)
        {
            s1Tmp = s1Par - s1Step;
        }
        else
        {
            s1Tmp = s1Par + s1Step;;
        }
    }
    else
    {
        s1Tmp = s1ParMin;
    }

    return s1Tmp;
}

static void _u1vHDMI2AnaResetDebugPar(void)
{
    INT8 s1Step = 32;
    UINT16 u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    while((tDebugPara.s1Par1 != tDebugPara.s1Par1Min) || (tDebugPara.s1Par2 != tDebugPara.s1Par2Min))
    {
        tDebugPara.s1Par1 = _u1vHDMI2AnaDebugStepPar(tDebugPara.s1Par1, tDebugPara.s1Par1Min, s1Step);
        tDebugPara.s1Par2 = _u1vHDMI2AnaDebugStepPar(tDebugPara.s1Par2, tDebugPara.s1Par2Min, s1Step);
        vHDMI2ComAnaDebug1ParSet(u2HDMIAnaBaseAddr,tDebugPara.s1Par1,tDebugPara.s1Par2);
        vUtDelay2us(100);
    }
    
    return;
}

static INT32 _u1vHDMI2AnaLogDebugStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Flag;
    UINT8 u1Channel,u1EvenOdd;
    UINT32 *pu4ChannelWPEven = NULL;
    UINT32 *pu4ChannelWPOdd = NULL;
    UINT8 i,j;
    u4Flag = HalCriticalStart();
    if(i4Argc < 3)
    {
        Printf("Usage: %s channel_id(0~2) even_odd(1:even,2:odd,3:both) \n", szArgv[0]);
        HalCriticalEnd(u4Flag);
        return 0;
    }
    u1Channel = (UINT8)StrToInt(szArgv[1]);
    if(u1Channel > 2)
    {
        Printf("Please input channel_id(0~2),current channel_id is %d > 2!\n",u1Channel);
        HalCriticalEnd(u4Flag);
        return 0;
    }
    if(u1Channel >= (_u4DebugBufSize/DEBUG_CHANNEL_BYTE_COUNT))
    {
        Printf("Channel %d data is not ready!\n",u1Channel);
        HalCriticalEnd(u4Flag);
        return 0;
    }
    u1EvenOdd = (UINT8)StrToInt(szArgv[2]);
    if(u1EvenOdd > 2)
    {
        u1EvenOdd = 3;
    }
    _u1vHDMI2AnaResetDebugWP();
    switch(u1Channel)
    {
        case 0:
            {
                pu4ChannelWPEven = tDebugPara._pu4DebugBufWP_CH0Even;
                pu4ChannelWPOdd = tDebugPara._pu4DebugBufWP_CH0Odd;
            }
            break;
        case 1:
            {
                pu4ChannelWPEven = tDebugPara._pu4DebugBufWP_CH1Even;
                pu4ChannelWPOdd = tDebugPara._pu4DebugBufWP_CH1Odd;
            }
            break;
        case 2:
            {
                pu4ChannelWPEven = tDebugPara._pu4DebugBufWP_CH2Even;
                pu4ChannelWPOdd = tDebugPara._pu4DebugBufWP_CH2Odd;
            }
            break;
        default:
            break;
    }
    if(u1EvenOdd & 0x01)
    {
        Printf("Channel %d even data:\n",u1Channel);
        for(i=0;i<128;i++)
        {
            for(j=0;j<128;j++)
            {
                Printf("%07d ",((*pu4ChannelWPEven) > DEBUG_MAX_COUNT ? DEBUG_MAX_COUNT:(*pu4ChannelWPEven)));
                pu4ChannelWPEven++;
            }
            Printf("\n");
        }
    }
    if(u1EvenOdd & 0x02)
    {
        Printf("Channel %d odd data:\n",u1Channel);
        for(i=0;i<128;i++)
        {
            for(j=0;j<128;j++)
            {
				Printf("%07d ",((*pu4ChannelWPOdd) > DEBUG_MAX_COUNT ? DEBUG_MAX_COUNT:(*pu4ChannelWPOdd)));
                pu4ChannelWPOdd++;
            }
            Printf("\n");
        }
    }
    HalCriticalEnd(u4Flag);

    return 0;
}

static INT32 _u1vHDMI2AnaDoDebug(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4CntTarget;
    UINT16 u2PatMask, u2TargetPat;
    UINT8 i,j;
    INT8 s1Par1,s1Par2;
    UINT16 u2HDMIAnaBaseAddr,u2Loop;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    u2PatMask = 0;
    u2TargetPat = 0;
    tDebugPara.s1Par1 = 0; //Initial value is 0
    tDebugPara.s1Par2 = 0; //Initial value is 0
    tDebugPara.s1Par1Min = -64;
    tDebugPara.s1Par1Max = 63;
    tDebugPara.s1Par2Min = -64;
    tDebugPara.s1Par2Max = 63;
    
    if (i4Argc < 3)
	{
		Printf("Usage: %s cnt_target u2Loop [PAT_MASK] [TARGET_PAT] [s1Par1Min] [s1Par1Max] [s1Par2Min] [s1Par2Max]\n", szArgv[0]);
		return 0;
	}
    if((i4Argc > 3) && (i4Argc < 5))
    {
        Printf("Usage: %s cnt_target u2Loop PAT_MASK TARGET_PAT [s1Par1Min] [s1Par1Max] [s1Par2Min] [s1Par2Max]\n", szArgv[0]);
		return 0;
    }
    if((i4Argc > 5) && (i4Argc < 9))
    {
        Printf("Usage: %s cnt_target u2Loop PAT_MASK TARGET_PAT s1Par1Min s1Par1Max [s1Par2Min] [s1Par2Max]\n", szArgv[0]);
		return 0;
    }
    if(!_u1vHDMI2AnaInitDebugBuf())
    {
        return -1;
    }

    vHDMI2ComAnaDebug1Ctrl_EN(u2HDMIAnaBaseAddr,1);
    u4CntTarget = (UINT32)StrToInt(szArgv[1]);
	u2Loop = (UINT16)StrToInt(szArgv[2]);
    vHDMI2ComAnaDebug1CntTargetSet(u2HDMIAnaBaseAddr,u4CntTarget);
    if(i4Argc >= 5)
    {
        u2PatMask = (UINT16)StrToInt(szArgv[3]);
        u2TargetPat = (UINT16)StrToInt(szArgv[4]);
        vHDMI2ComAnaDebug1PatSet(u2HDMIAnaBaseAddr,u2PatMask,u2TargetPat);
    }
    if(i4Argc >= 9)
    {
        tDebugPara.s1Par1Min = (INT8)i4StrToInt(szArgv[5]);
        tDebugPara.s1Par1Max = (INT8)i4StrToInt(szArgv[6]);
        tDebugPara.s1Par2Min = (INT8)i4StrToInt(szArgv[7]);
        tDebugPara.s1Par2Max = (INT8)i4StrToInt(szArgv[8]);
    }
    vHDMI2ComAnaDebug2Enable(u2HDMIAnaBaseAddr);
	vHDMI2ComAnaDebug1Enable(u2HDMIAnaBaseAddr,FALSE);
	for(j=0; j<u2Loop; j++)
	{
	    Printf("Debug loop %d >>>>>>>>>>>>>>>>>>\n",j);
		_u1vHDMI2AnaResetDebugPar();
		_u1vHDMI2AnaResetDebugWP();
	    for(s1Par1=-64; s1Par1<64; s1Par1++)
	    {
	        for(s1Par2=-64; s1Par2<64; s1Par2++)
	        {
	            if(_u1vHDMI2AnaDebugSetSkipValue(s1Par1,s1Par2,u4CntTarget))
                {
                    continue;
                }
	            vHDMI2ComAnaDebug1ParSet(u2HDMIAnaBaseAddr,s1Par1,s1Par2);
	            vHDMI2ComAnaDebug1Enable(u2HDMIAnaBaseAddr,TRUE);
	            for(i=0; i<100; i++)
	            {
	                vUtDelay2us(100);
	                if(bHDMI2ComAnaGetDebug1Ready(u2HDMIAnaBaseAddr,HDMI_TMDS_DATA_CHANNEL_0) && bHDMI2ComAnaGetDebug1Ready(u2HDMIAnaBaseAddr,HDMI_TMDS_DATA_CHANNEL_1) 
	                    && bHDMI2ComAnaGetDebug1Ready(u2HDMIAnaBaseAddr,HDMI_TMDS_DATA_CHANNEL_2))
	                {
	                    break;
	                }
	            }
	            if(100 == i)
	            {
	                Printf("Error:Debug is not ready!\n");
	                return -1;
	            }
	            vHDMI2ComAnaDebug1Enable(u2HDMIAnaBaseAddr,FALSE);
                tDebugPara.s1Par1 = s1Par1;
                tDebugPara.s1Par2 = s1Par2;
	            _u1vHDMI2AnaRecordDebugStatus();
	        }
	    }
	}

    vHDMI2ComAnaDebug1Ctrl_EN(u2HDMIAnaBaseAddr,0);
    Printf("Debug data is ready,do acom.dbglog to get status!\n");
    return 0;
}

static INT32 _u1vHDMI2AnaDelay(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1DelayModule;
    UINT16 u2Delay;
    if(i4Argc < 2)
    {
        Printf("Current delay ratio(*10%):\n");
        u2Delay = wHDMI2ComAnaDelayGet(HDMI2ANA_DELAY_SAOSC);
        Printf("SAOSC:%d\n",u2Delay);
        u2Delay = wHDMI2ComAnaDelayGet(HDMI2ANA_DELAY_EQLOCK_HALF);
        Printf("EQLock[Halfrate]:%d\n",u2Delay);
        u2Delay = wHDMI2ComAnaDelayGet(HDMI2ANA_DELAY_EQLOCK_FULL);
        Printf("EQLock[Fullrate]:%d\n",u2Delay);
        u2Delay = wHDMI2ComAnaDelayGet(HDMI2ANA_DELAY_EQINCREASE);
        Printf("EQIncrease:%d\n",u2Delay);
        return 0;
    }
    else if(i4Argc < 3)
    {
        Printf("Usage: %s delay_module(0:SAOSC,1:EQLock[Halfrate],2:EQLock(Fullrate),3:EQIncrease) delay_ratio(*10%)\n", szArgv[0]);
		return 0;
    }
    u1DelayModule = (UINT8)StrToInt(szArgv[1]);
    u2Delay = (UINT16)StrToInt(szArgv[2]);
    if(u1DelayModule >= HDMI2ANA_DELAY_MAX)
    {
        Printf("delaymodule is %d >= delay module count(%d)!\n",u1DelayModule,HDMI2ANA_DELAY_MAX);
        return 0;
    }
    vHDMI2ComAnaDelaySet(u1DelayModule,u2Delay);

    return 0;
}

static INT32 _u1vHDMI2AnaRstb(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Module;
    BOOL bReset;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if(i4Argc < 3)
    {
        Printf("Usage: %s reset_module(0:Data,1:EQALG,2:SAOSC,3:EQOSC,4:CDR,5:LEQ,6:EQ_Increase,7:Adjust) reset(0:reset 1:reset release)\n", szArgv[0]);
		return 0;
    }
    u1Module = (UINT8)StrToInt(szArgv[1]);
    bReset = (BOOL)StrToInt(szArgv[2]);
    switch(u1Module)
    {
        case 0:
            {
                vHDMI2ComAnaData_RSTB(u2HDMIAnaBaseAddr,bReset);
            }
            break;
        case 1:
            {
                vHDMI2ComAnaEQALG_RSTB(u2HDMIAnaBaseAddr,bReset);
            }
            break;
        case 2:
            {
                vHDMI2ComAnaSAOSC_RSTB(u2HDMIAnaBaseAddr,bReset);
            }
            break;
        case 3:
            {
                vHDMI2ComAnaEQOSC_RSTB(u2HDMIAnaBaseAddr,bReset);
            }
            break;
        case 4:
            {
                vHDMI2ComAnaCDR_RST(u2HDMIAnaBaseAddr,!bReset);
            }
            break;
        case 5:
            {
                vHDMI2ComAnaEQ_RSTB(u2HDMIAnaBaseAddr,bReset);
            }
            break;
        case 6:
            {
                vHDMI2ComAnaEQIncrease_RSTB(u2HDMIAnaBaseAddr,bReset);
            }
            break;
        case 7:
            {
                vHDMI2ComAnaAdjust_RSTB(u2HDMIAnaBaseAddr,bReset);
            }
            break;
        default:
            Printf("Usage: %s reset_module(0:Data,1:EQALG,2:SAOSC,3:EQOSC,4:CDR,5:LEQ,6:EQ_Increase,7:Adjust) reset(0:reset 1:reset release)\n", szArgv[0]);
            break;
    }

    return 0;
}
static INT32 _u1vHDMI2AnaCrc(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc < 2)
	{
		Printf("Usage: %s times\n", szArgv[0]);
		return 0;
	}
	fgHDMI2ComAnaCRC(0,(UINT8)StrToInt(szArgv[1]));
	return 0;
}

static INT32 _u1vHDMI2AnaEQBand(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2Clock;
    BOOL b40xMode = TRUE;
    HdmiTmdsClkRate_t eBand = HDMI_TMDS_CLOCK_UNKNOW;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if (i4Argc < 3)
	{
		Printf("Usage: %s clock is40xMode\n", szArgv[0]);
		return 0;
	}
    u2Clock = (UINT16)StrToInt(szArgv[1]);
    eBand = _u1vTransClockToBand(u2Clock);
    if(StrToInt(szArgv[2]))
    {
        b40xMode = TRUE;
    }
    else
    {
        b40xMode = FALSE;
    }
    vHDMI2ComAnaEQBandSet(u2HDMIAnaBaseAddr,eBand,b40xMode);
    Printf("HDMI analog set EQ band done\n");

    return 0;
}

static INT32 _u1vHDMI2AnaPLL(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2Clock;
    BOOL b40xMode = TRUE;
    HdmiTmdsClkRate_t eBand = HDMI_TMDS_CLOCK_UNKNOW;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if (i4Argc < 3)
	{
		Printf("Usage: %s clock is40xMode\n", szArgv[0]);
		return 0;
	}
    u2Clock = (UINT16)StrToInt(szArgv[1]);
    eBand = _u1vTransClockToBand(u2Clock);
    if(StrToInt(szArgv[2]))
    {
        b40xMode = TRUE;
    }
    else
    {
        b40xMode = FALSE;
    }
    vHDMI2ComAnaPLLSet(u2HDMIAnaBaseAddr,eBand,b40xMode);
    Printf("HDMI analog set PLL done\n");

    return 0;
}

static INT32 _u1vHDMI2AnaDeepPLL(INT32 i4Argc, const CHAR **szArgv)
{
    UINT16 u2Clock;
    BOOL b40xMode = TRUE;
    HdmiTmdsClkRate_t eBand = HDMI_TMDS_CLOCK_UNKNOW;
    UINT16 u2HDMIAnaBaseAddr;
    u2HDMIAnaBaseAddr = u2HDMI2GetAnaBaseAddr(eActiveHdmiPort);
    if (i4Argc < 3)
	{
		Printf("Usage: %s clock is40xMode\n", szArgv[0]);
		return 0;
	}
    u2Clock = (UINT16)StrToInt(szArgv[1]);
    eBand = _u1vTransClockToBand(u2Clock);
    if(StrToInt(szArgv[2]))
    {
        b40xMode = TRUE;
    }
    else
    {
        b40xMode = FALSE;
    }
    vHDMI2ComAnaDeepPLLSet(u2HDMIAnaBaseAddr,eBand,b40xMode);
    Printf("HDMI analog set deep PLL done\n");

    return 0;
}

static INT32 _u1vHDMI2AnaGetAllStatus(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1CrcTimes = 100;
    BOOL fgVerbose = TRUE;
    if (i4Argc < 2)
	{
		Printf("Usage: %s crc_times [verbose]\n", szArgv[0]);
		return 0;
	}
    u1CrcTimes = (UINT8)StrToInt(szArgv[1]);
    if(i4Argc >= 3)
    {
        if(StrToInt(szArgv[2]))
        {
            fgVerbose = TRUE;
        }
        else
        {
            fgVerbose = FALSE;
        }
    }
    vHDMI2ComDeepRst1(0);
    vHDMI2ComDigPhyRst1(0);
    x_thread_delay(50); //50ms

    if(fgVerbose)
    {
        Printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<------------>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        Printf("b.gpc 0\n");
        CLI_Parser_Arg("b.gpc 0");
        Printf("b.gpc 0x2b\n");
        CLI_Parser_Arg("b.gpc 0x2b");
        Printf("b.gpc 0\n");
        CLI_Parser_Arg("b.gpc 0");
        Printf("b.gpc 0x2c\n");
        CLI_Parser_Arg("b.gpc 0x2c");
        Printf("b.gpc 0\n");
        CLI_Parser_Arg("b.gpc 0");
        Printf("b.gpc 0x2d\n");
        CLI_Parser_Arg("b.gpc 0x2d");
        Printf("b.gpc 0\n");
        CLI_Parser_Arg("b.gpc 0");
        Printf("b.gpc 0x38\n");
        CLI_Parser_Arg("b.gpc 0x38");
        Printf("b.gpc 0\n");
        CLI_Parser_Arg("b.gpc 0");
        Printf("b.gpc 0x39\n");
        CLI_Parser_Arg("b.gpc 0x39");
        Printf("b.gpc 0\n");
        CLI_Parser_Arg("b.gpc 0");
        Printf("b.gpc 0x3a\n");
        CLI_Parser_Arg("b.gpc 0x3a");
        Printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<------------>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        Printf("do 10 n.hdmi2.acom.gcs\n");
        CLI_Parser_Arg("do 10 n.hdmi2.acom.gcs");
        Printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<------------>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        Printf("EQ_Increase P0:\n");
        CLI_Parser_Arg("n.hdmi2.acom.gam 0");
        Printf("EQ_Increase P1:\n");
        CLI_Parser_Arg("n.hdmi2.acom.gam 1");
        Printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<------------>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        Printf("n.hdmi2.acom.crc %d\n",u1CrcTimes);
        CLI_Parser_Arg("n.hdmi2.acom.crc %d",u1CrcTimes);
    }
    Printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<------------>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    Printf("n.hdmi2.dcom.gvs\n");
    CLI_Parser_Arg("n.hdmi2.dcom.gvs");
    Printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<------------>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    Printf("n.hdmi2.acom.geq\n");
    CLI_Parser_Arg("n.hdmi2.acom.geq");
    Printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<------------>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    Printf("n.hdmi2.dcom.c %d\n",u1CrcTimes);
    CLI_Parser_Arg("n.hdmi2.dcom.c %d",u1CrcTimes);

    return 0;
}


static INT32 _u1vHDMI2AnaSetAll(INT32 i4Argc, const CHAR **szArgv)
{
    BOOL b40xMode = TRUE;
    if (i4Argc < 2)
	{
		Printf("Usage: %s is40xMode\n", szArgv[0]);
		return 0;
	}
    if(StrToInt(szArgv[1]))
    {
        b40xMode = TRUE;
    }
    else
    {
        b40xMode = FALSE;
    }
    CLI_Parser_Arg("n.hdmi2.acom.i");
    CLI_Parser_Arg("n.hdmi2.dcom.hpd 1");
    CLI_Parser_Arg("n.hdmi2.dcom.clk");
    if(fgFirstSetAll)
    {
        x_thread_delay(2000);
        fgFirstSetAll = FALSE;
    }
    else
    {
        x_thread_delay(100);
    }
    CLI_Parser_Arg("n.hdmi2.acom.s %d",b40xMode);

    return 0;  
}

static INT32 _u1vHDMI2AnaTestAll(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u140xMode,u1CrcTimes;
    BOOL fgVerbose = TRUE;
    if (i4Argc < 3)
	{
		Printf("Usage: %s is40xMode crc_times [verbose]\n", szArgv[0]);
		return 0;
	}
    u140xMode = (UINT8)StrToInt(szArgv[1]);
    u1CrcTimes = (UINT8)StrToInt(szArgv[2]);
    if(i4Argc >= 4)
    {
        if(StrToInt(szArgv[3]))
        {
            fgVerbose = TRUE;
        }
        else
        {
            fgVerbose = FALSE;
        }
    }
    CLI_Parser_Arg("n.hdmi2.acom.sa %d",u140xMode);
    CLI_Parser_Arg("n.hdmi2.acom.gas %d %d",u1CrcTimes,fgVerbose);
    
    return 0;
}

static INT32 _u1vHDMI2DigInit(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

    vHDMI2ComRxInit(0);
    Printf("HDMI RX digital init done\n");

    return 0;
}
static INT32 _u1vHDMI2HDCP2Init(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

    vHDMI2ComHDCP2Init(0);
    Printf("HDMI HDCP2.2 init done\n");

    return 0;
}

static INT32 _u1vHDMI2DdcSel(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Type=0;
	
	if (i4Argc < 2)
	{
		Printf("Usage: %s Type(1:HDMI2.0,0:HDMI1.4)\n", szArgv[0]);
		return 0;
	}
	u1Type = (UINT8)StrToInt(szArgv[1]);

	if(u1Type)
	{
		Printf("DDC select HDMI2.0\n");
	}
	else
	{
		Printf("DDC select HDMI1.4\n");
	}
	
    vHDMI2ComDdcSel(0,u1Type);

    return 0;
}
static INT32 _u1vHDMI2ScrambleSet(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1En;
	
	if (i4Argc < 2)
	{
		Printf("Usage: %s en\n", szArgv[0]);
		return 0;
	}
	
	u1En = (UINT8)StrToInt(szArgv[1]);

	if(u1En)
	{
		Printf("Set scrambling on\n");
	}
	else
	{
		Printf("Set scrambling off\n");
	}

    vHDMI2ComSetScrambling(0,u1En);

    return 0;
}
static INT32 _u1vHDMI2CRC(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc < 2)
	{
		Printf("Usage: %s times\n", szArgv[0]);
		return 0;
	}
	fgHDMI2ComCRC(0,(UINT8)StrToInt(szArgv[1]));
	return 0;
}
static INT32 _u1vHDMI2SplitSet(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1En;
	
	if (i4Argc < 2)
	{
		Printf("Usage: %s en\n", szArgv[0]);
		return 0;
	}
	
	u1En = (UINT8)StrToInt(szArgv[1]);

	if(u1En)
	{
		Printf("Set split on\n");
	}
	else
	{
		Printf("Set split off\n");
	}

    vHDMI2ComSetSplit(0,u1En);
	return 0;
}
static INT32 _u1vHDMI2420Set(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1En;
	
	if (i4Argc < 2)
	{
		Printf("Usage: %s en\n", szArgv[0]);
		return 0;
	}
	
	u1En = (UINT8)StrToInt(szArgv[1]);

	if(u1En)
	{
		Printf("Set 420 on\n");
	}
	else
	{
		Printf("Set 420 off\n");
	}

    vHDMI2ComSet420To444(0,u1En);
	return 0;
}
static INT32 _u1vHDMI2DdcSet(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1En;
	
	if (i4Argc < 2)
	{
		Printf("Usage: %s en\n", szArgv[0]);
		return 0;
	}
	
	u1En = (UINT8)StrToInt(szArgv[1]);

	if(u1En)
	{
		Printf("Set DDC on\n");
	}
	else
	{
		Printf("Set DDC off\n");
	}

    vHDMI2ComSetHdcpDDC(0,u1En);
	return 0;
}
static INT32 _u1vHDMI2HpdSet(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1En;
	
	if (i4Argc < 2)
	{
		Printf("Usage: %s High/Low\n", szArgv[0]);
		return 0;
	}
	
	u1En = (UINT8)StrToInt(szArgv[1]);

	if(u1En)
	{
		Printf("Set hpd high\n");
		vHDMIHPDHigh(4);
	}
	else
	{
		Printf("Set hpd low\n");
		vHDMIHPDLow(4);
	}
	
    //vHDMI2ComSetDDC(0,u1En);
    return 0;
}

static INT32 _u1vHDMI2ClkSet(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

    vHDMI2ComSetClk(0);
	return 0;
}
	
static INT32 _u1vHDMI2NewDigSet(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc < 2)
	{
		Printf("Usage: %s en\n", szArgv[0]);
		return 0;
	}

	vHDMI2ComNewDigSel(0,(UINT8)StrToInt(szArgv[1]));
	return 0;
}
static INT32 _u1vHDMI2DigitalPhyRst(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

   // vHDMI2ComDigPhyRst(0,(UINT8)StrToInt(szArgv[1]));
	vHDMI2ComDigPhyRst1(0);
	return 0;
}
static INT32 _u1vHDMI2DeepColorRst(INT32 i4Argc, const CHAR **szArgv)
{	
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

    vHDMI2ComDeepRst1(0);
	return 0;
}
static INT32 _u1vHDMI2FifoRst(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

    vHDMI2ComFifoRst(0);
	return 0;
}
static INT32 _u1vHDMI2Hdcp1Rst(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

    vHDMI2ComHdcp1Rst(0);
	return 0;
}

static INT32 _u1vHDMI2Hdcp2Rst(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

    vHDMI2ComHdcp2Rst(0);
	return 0;
}

static INT32 _u1vHDMI2APLLSet(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

    vHDMI2ComAudPllSel(0,1);
	return 0;
}
static INT32 _u1vHDMI2DPLLSet(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

    vHDMI2ComAudPllSel(0,0);
	return 0;
}
static INT32 _u1vHDMI2GetClockRate(INT32 i4Argc, const CHAR **szArgv)
{
	HdmiTmdsClkRate_t clkRate;
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

    clkRate = bHDMI2ComGetClkRate(0);

	switch(clkRate)
	{
		case HDMI_TMDS_CLOCK_25:
			Printf("TMDS clock < 25M\n");
			break;
		case HDMI_TMDS_CLOCK_25_35:
			Printf("25M < TMDS clock < 35M\n");
			break;
		case HDMI_TMDS_CLOCK_35_40:
			Printf("35M < TMDS clock < 40M\n");
			break;
		case HDMI_TMDS_CLOCK_40_50:
			Printf("40M < TMDS clock < 50M\n");
			break;
		case HDMI_TMDS_CLOCK_50_80:
			Printf("50M < TMDS clock < 80M\n");
			break;
		case HDMI_TMDS_CLOCK_80_100:
			Printf("80M < TMDS clock < 100M\n");
			break;
		case HDMI_TMDS_CLOCK_100_120:
			Printf("100M < TMDS clock < 120M\n");
			break;
		case HDMI_TMDS_CLOCK_140_200:
			Printf("140M < TMDS clock < 200M\n");
			break;
		case HDMI_TMDS_CLOCK_200_300:
			Printf("200M < TMDS clock < 300M\n");
			break;
		default:
			Printf("invalid TMDS clock\n");
			break;
	}

	return 0;

}
static INT32 _u1vHDMI2GetVideoStatus(INT32 i4Argc, const CHAR **szArgv)
{
	vHDMI2ComGetVideoStatus(0);
	return 0;
}
static INT32 _u1vHDMI2GetAudioStatus(INT32 i4Argc, const CHAR **szArgv)
{
	vHDMI2ComGetAudioStatus(0);
	return 0;
}
static INT32 _u1vHDMI2GetHdcp14Status(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1HdcpSta;
	u1HdcpSta = bHDMI2ComGetHdcp1Status(0);
	if(u1HdcpSta & HDMI_RX_HDCP1_STATUS_AUTH_DONE)
	{
		Printf("HDCP1.4 auth done\n");
	}
	else if(u1HdcpSta & HDMI_RX_HDCP1_STATUS_AUTH_FAIL)
	{
		Printf("HDCP1.4 auth fail\n");
	}
	else if(u1HdcpSta & HDMI_RX_HDCP1_STATUS_DECRYPT_SUCCESS)
	{
		Printf("HDCP1.4 decrypt success\n");
	}
	else if(u1HdcpSta & HDMI_RX_HDCP1_STATUS_DECRYPT_FAIL)
	{
		Printf("HDCP1.4 decrypt fail\n");
	}
	return 0;
}
static INT32 _u1vHDMI2GetHdcp22Status(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1HdcpSta;
	u1HdcpSta = bHDMI2ComGetHdcp2Status(0);

	if(u1HdcpSta & HDMI_RX_HDCP2_STATUS_AUTH_DONE)
	{
		Printf("Hdcp2.2 auth done\n");
	}
	else if(u1HdcpSta & HDMI_RX_HDCP2_STATUS_AUTH_FAIL)
	{
		Printf("Hdcp2.2 auth fail\n");
	}
	else if(u1HdcpSta & HDMI_RX_HDCP2_STATUS_CCHK_DONE)
	{
		Printf("Hdcp2.2 cchk done\n");
	}
	else if(u1HdcpSta & HDMI_RX_HDCP2_STATUS_CCHK_FAIL)
	{
		Printf("Hdcp2.2 cchk fail\n");
	}
	else if(u1HdcpSta & HDMI_RX_HDCP2_STATUS_AKE_SENT_RCVD)
	{
		Printf("Hdcp2.2 Ake sent rcvd\n");
	}
	else if(u1HdcpSta & HDMI_RX_HDCP2_STATUS_SKE_SENT_RCVD)
	{
		Printf("Hdcp2.2 Ske sent rcvd\n");
	}
	else if(u1HdcpSta & HDMI_RX_HDCP2_STATUS_CERT_SENT_RCVD)
	{
		Printf("Hdcp2.2 Cert sent rcvd\n");
	}
	return 0;
}
static INT32 _u1vHDMI2GetInfoframe(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1InfoType=0;
	
	if (i4Argc != 2)
    {
		Printf("Usage: %s INFO_Type\n", szArgv[0]);
		Printf("1. AVI info parser\n");
		Printf("2. VSI info parser\n");
		Printf("3. HF_VSI info parser\n");
        return 0;
    }
	u1InfoType = (UINT8)StrToInt(szArgv[1]);

	if(u1InfoType == 1)
	{
		vHDMI2ComGetVideoInfoByAVI(0);
	}
	else if(u1InfoType == 2)
	{
		//VSI info
	}
	else if(u1InfoType == 3)
	{
		//HF VSI info
	}
	return 0;
	
}

static INT32 _u1vHDMI2Get5vStatus(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}
	if(bHDMIPort5VStatus(4))
	{
		Printf("HDMI4 have 5v status\n");
	}
	else
	{
		Printf("HDMI4 no 5v status\n");
	}

	return 0;
}
