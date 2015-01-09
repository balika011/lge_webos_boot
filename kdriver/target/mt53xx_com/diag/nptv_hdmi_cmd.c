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
#include "drv_hdmi.h"
#include "drv_dvi.h"
#include "hw_hdmi.h"
#include "hw_vdoin.h"

#include "drvcust_if.h"

#ifndef CC_CLI
#define CC_CLI
#endif

//#ifdef CC_CLI


/* Functions for HDMI */
static INT32 _u1vHDMIInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIMainLoop(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIStopFw(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIResumeFw(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMICRC(INT32 i4Argc, const CHAR **szArgv);
#ifdef __MODEL_slt__
static INT32 _u1vHDMICRCMulti(INT32 i4Argc, const CHAR **szArgv);
#endif
#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _u1vHDMIPHYCRC(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _u1vCCIR2(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vDVICRC(INT32 i4Argc, const CHAR **szArgv);
#ifdef NPTV_CMD_HDMI_RW_DEV_SUPPORT
static INT32 _u1vHDMIWriteDevL(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIWriteDevH(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1bHDMIReadDevL(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1bHDMIReadDevH(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1bHDMIDumpDevL(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1bHDMIDumpDevH(INT32 i4Argc, const CHAR **szArgv);
#endif //NPTV_CMD_HDMI_RW_DEV_SUPPORT
static INT32 _u1GPIO_Output(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIDebugMsg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIHDCPKey(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIReloadHDCPKey(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIHDCPSelfBist(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIBypassVdo(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIvLoadColorMatrix(INT32 i4Argc, const CHAR **szArgv);
static INT32  _u1vHDMIMCMHdcpWrite(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMISetHDMIState(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMISetSwitch(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIEQSel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMILBWSet(INT32 i4Argc, const CHAR **szArgv);

static INT32 _u1vHDMIEQGain(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMITHR(INT32 i4Argc, const CHAR **szArgv);
#if defined(CC_MT5399) || defined(CC_MT5890)
static INT32 _u1HDMISPDINFOCHECK(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1HDMISPDINFOSET(INT32 i4Argc, const CHAR **szArgv);
#endif
static INT32 _u1vHDMIHPDHigh(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIHPDLow(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIGetInfoFrame(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIHPDIndepCtrl(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIModeSel(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIDebug(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIForce3D(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIForce3DTo2D(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIPHYTune(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1HDMIVSTABLECNT(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vHDMIEqQuery(INT32 i4Argc, const CHAR **szArgv);
#ifdef CC_MHL_SENSE_NOT_CONNECTED
static INT32 _u1ForceMHLMode(INT32 i4Argc, const CHAR **szArgv);
#endif

//#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for HDMI command entry
CLI_EXEC_T arHDMICmdTbl[] = {
  //#ifdef CC_CLI
  {"Init", "i", _u1vHDMIInit, NULL, "HDMI initialization", CLI_SUPERVISOR},
  {"Mainloop", "m", _u1vHDMIMainLoop, NULL, "HDMI mainloop", CLI_SUPERVISOR},
  {"stop fw", "stop", _u1vHDMIStopFw, NULL, "stop HDMI mainloop", CLI_SUPERVISOR},
  {"resume fw", "resume", _u1vHDMIResumeFw, NULL, "resume HDMI mainloop", CLI_SUPERVISOR},
  {"set fw state", "set", _u1vHDMISetHDMIState, NULL, "control fw state machine", CLI_SUPERVISOR},
  #if defined(CC_MT5365) || defined(CC_MT5395)
  {"PHYCRC", "pc", _u1vHDMIPHYCRC, NULL, "HDMI analog PHY CRC check", CLI_SUPERVISOR},
  #endif
  {"CRC", "c", _u1vHDMICRC, NULL, "HDMI CRC check", CLI_SUPERVISOR},
#ifdef __MODEL_slt__
  {"CRC1", "cc", _u1vHDMICRCMulti, NULL, "HDMI CRC1 check", CLI_SUPERVISOR},
#endif
  {"CCIR2", "ccir", _u1vCCIR2, NULL, "ccir2 for wifi push", CLI_SUPERVISOR},
  {"DVICRC", "dc", _u1vDVICRC, NULL, "DVI CRC check", CLI_SUPERVISOR},
  {"dmesg", "dmsg", _u1vHDMIDebugMsg, NULL, "Print HDMI status", CLI_GUEST},
  {"hdcp log key", "hlk", _u1vHDMIHDCPKey, NULL, "Print hdcp key", CLI_SUPERVISOR},
  {"hdcp reload key", "hrk", _u1vHDMIReloadHDCPKey, NULL, "Reload hdcp key", CLI_SUPERVISOR},
  {"hdcp bist", "hdb", _u1vHDMIHDCPSelfBist, NULL, "Hdcp seltbist", CLI_SUPERVISOR},
  {"hdmi bypass", "hbv", _u1vHDMIBypassVdo, NULL, "Bypass HDMI processing", CLI_SUPERVISOR},
  {"switch", "switch", _u1vHDMISetSwitch, NULL, "switch HDMI1/HDMI2", CLI_SUPERVISOR},
  {"GPIO", "io", _u1GPIO_Output, NULL, "I/O", CLI_SUPERVISOR},
  {"EQSEL", "eq", _u1vHDMIEQSel, NULL, "set EQ_SEL", CLI_SUPERVISOR},
  {"LBW", "lbw", _u1vHDMILBWSet, NULL, "set LBW", CLI_SUPERVISOR},
  {"EQ_GAIN", "eqg", _u1vHDMIEQGain, NULL, "set EQ_GAIN", CLI_SUPERVISOR},
  {"HDMI THR", "thr", _u1vHDMITHR, NULL, "set mode change THR and mode detect THR", CLI_SUPERVISOR},
  {"HPD high", "hh", _u1vHDMIHPDHigh, NULL, "set HPD HIGH", CLI_GUEST},
  {"HPD Low", "hl", _u1vHDMIHPDLow, NULL, "set HPD LOW", CLI_GUEST},
   {"InfoFrame read", "info", _u1vHDMIGetInfoFrame, NULL, "read infoframe", CLI_SUPERVISOR},
   {"CKDT detect", "ckdt", _u1vHDMIHPDIndepCtrl, NULL, "5v detect /CKDT detect", CLI_SUPERVISOR},
  {"3x3", "33", _u1vHDMIvLoadColorMatrix, NULL, "3x3 color transform", CLI_SUPERVISOR},
  {"MCM", "MCM", _u1vHDMIMCMHdcpWrite, NULL, "MCM EEPROM write", CLI_SUPERVISOR},
  {"Mode Sel", "mode", _u1vHDMIModeSel, NULL, "Auto/Graphic/Video mode select", CLI_SUPERVISOR},
  {"HDMIdebug", "debug", _u1vHDMIDebug, NULL, "HDMI debug command", CLI_GUEST},
  {"Force 3D", "f3d", _u1vHDMIForce3D, NULL, "HDMI force 3D", CLI_SUPERVISOR},
  {"Force 3D to 2D", "f2d", _u1vHDMIForce3DTo2D, NULL, "HDMI force 3D to 2D", CLI_SUPERVISOR},
  {"HDMI PHY tune", "phyt", _u1vHDMIPHYTune, NULL, "HDMI PHY tune", CLI_GUEST},
  {"HDMI EQ query", "eqq", _u1vHDMIEqQuery, NULL, "HDMI EQ Query", CLI_GUEST},
  {"HDMI STABLE Cnt","vs",_u1HDMIVSTABLECNT,NULL,"HDMI STABLE Cnt",CLI_SUPERVISOR},
  #if defined(CC_MT5399) || defined(CC_MT5890)
  {"SPD Info Check","spd",_u1HDMISPDINFOCHECK,NULL,"SPD Info Check",CLI_GUEST},
  {"SPD Info Set","spds",_u1HDMISPDINFOSET,NULL,"SPD Info Set",CLI_GUEST},
  #endif
  #ifdef CC_MHL_SENSE_NOT_CONNECTED
  {"forceMHL", "fcM", _u1ForceMHLMode, NULL, "Force MHL mode 1:MHL; 0:HDMI", CLI_GUEST},
  #endif
  //#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

//#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _u1vHDMIInit(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	vHDMIInit();

	Printf("vHDMIInit\n");

	return 0;
}

static INT32 _u1vHDMIMainLoop(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 1)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	vHDMIMainLoop();

	Printf("vHDMIMainLoop\n");

	return 0;
}

static INT32 _u1vHDMIStopFw(INT32 i4Argc, const CHAR **szArgv)
{
	vHDMIStopFw();
    return 0;
}

static INT32 _u1vHDMIResumeFw(INT32 i4Argc, const CHAR **szArgv)
{
	vHDMIResumeFw();
    return 0;
}

static INT32 _u1vHDMICRC(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	UNUSED(fgHDMICRC((UINT16)StrToInt(szArgv[1])));

	return 0;
}

#ifdef __MODEL_slt__
static INT32 _u1vHDMICRCMulti(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	UNUSED(fgHDMICRCMulti((UINT16)StrToInt(szArgv[1])));

	return 0;
}
#endif

#if defined(CC_MT5365) || defined(CC_MT5395)
static INT32 _u1vHDMIPHYCRC(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	UNUSED(fgHDMIPHYCRC((UINT16)StrToInt(szArgv[1])));

	return 0;
}
#endif
static INT32 _u1vCCIR2(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s(1:on/0:off)\n", szArgv[0]);
		return 0;
	}

	UNUSED(fgCCIR2((UINT16)StrToInt(szArgv[1])));

	return 0;

}

static INT32 _u1vDVICRC(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	UNUSED(fgDVICRC((UINT16)StrToInt(szArgv[1])));

	return 0;
}

#ifdef NPTV_CMD_HDMI_RW_DEV_SUPPORT
static INT32 _u1vHDMIWriteDevL(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3)
	{
		Printf("Usage: %s addr value\n", szArgv[0]);
		return 0;
	}
//	vHDMIWriteDevL((UINT8)StrToInt(szArgv[1]), (UINT8)StrToInt(szArgv[2]));

	return 0;
}

static INT32 _u1vHDMIWriteDevH(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3)
	{
		Printf("Usage: %s addr value\n", szArgv[0]);
		return 0;
	}
//	vHDMIWriteDevH((UINT8)StrToInt(szArgv[1]), (UINT8)StrToInt(szArgv[2]));

	return 0;
}
static INT32 _u1bHDMIReadDevL(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s addr\n", szArgv[0]);
		return 0;
	}

//	Printf("read low value 0x%x\n", bHDMIReadDevL((UINT8)StrToInt(szArgv[1])));

	return 0;
}
static INT32 _u1bHDMIReadDevH(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s addr\n", szArgv[0]);
		return 0;
	}

//	Printf("read high value 0x%x\n", bHDMIReadDevH((UINT8)StrToInt(szArgv[1])));

	return 0;
}


static INT32 _u1bHDMIDumpDevL(INT32 i4Argc, const CHAR **szArgv)
{
    INT32  i;
	// UINT32 u4Idx;

	for (i=0; i<0xff ; i++)
	{
//	    Printf("0x%2X read low value 0x%x\n", i, bHDMIReadDevL(i));
	}

	return 0;
}

static INT32 _u1bHDMIDumpDevH(INT32 i4Argc, const CHAR **szArgv)
{
    INT32  i;
	// UINT32 u4Idx;

	for (i=0; i<0xff ; i++)
	{
//	    Printf("0x%2X read high value 0x%x\n", i, bHDMIReadDevH(i));
	}

	return 0;
}

#endif //NPTV_CMD_HDMI_RW_DEV_SUPPORT

static INT32 _u1GPIO_Output(INT32 i4Argc, const CHAR **szArgv)
{
	INT32 i;

	if (i4Argc != 3)
	{
		Printf("Usage: %s addr high/low\n", szArgv[0]);
		return 0;
	}

	Printf("addr: %x\n", StrToInt(szArgv[1]));
	if ((UINT8)StrToInt(szArgv[2]) == 0)
	{
		i = 0;
		Printf("output low\n");
	}
	else
	{
		i = 1;
		Printf("output high\n");
	}
	GPIO_Output(StrToInt(szArgv[1]), &i);
    return 0;
}

static INT32 _u1vHDMIEQSel(INT32 i4Argc, const CHAR **szArgv)
{
	#if defined(CC_MT5387) ||defined(CC_MT5363)
	UINT32 _dwVal = 0;
	if (i4Argc != 2)
	{
        Printf("***************EQ = %x*********************\n", IO32ReadFldAlign(TMDS_CTRL1, EQSEL));
        Printf("Usage Example: EQ 0xd\n");
        return 0;
	}
	_dwVal = (UINT8)StrToInt(szArgv[1]);
	Printf("***************pre EQ = %x*********************\n", IO32ReadFldAlign(TMDS_CTRL1, EQSEL));
	vIO32WriteFldAlign(TMDS_CTRL1,_dwVal, EQSEL);
	Printf("***************now EQ = %x*********************\n", IO32ReadFldAlign(TMDS_CTRL1, EQSEL));
	#endif

	#if defined(CC_MT5365) || defined(CC_MT5395)
		#if !Support_HW_Adaptive_EQ  // Fix EQ
			UINT32 _dwVal = 0;
			if (i4Argc != 2)
			{
		        Printf("***************HDMI_EQ_FILT = 0x%x*********************\n", IO32ReadFldAlign(ANA_INTF_1, RG_HDMI_EQ_FILT));
		        Printf("Usage Example: EQ 0xd\n");
			  Printf("Max->Min: 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0, 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x8\n");
		        return 0;
			}
			_dwVal = (UINT8)StrToInt(szArgv[1]);
			Printf("***************pre HDMI_EQ_FILT = 0x%x*********************\n", IO32ReadFldAlign(ANA_INTF_1, RG_HDMI_EQ_FILT));
			vIO32WriteFldAlign(ANA_INTF_1,_dwVal, RG_HDMI_EQ_FILT);
			Printf("***************now HDMI_EQ_FILT = 0x%x*********************\n", IO32ReadFldAlign(ANA_INTF_1, RG_HDMI_EQ_FILT));
		#endif
	#endif
	return 0;
	
}

static INT32 _u1vHDMILBWSet(INT32 i4Argc, const CHAR **szArgv)
{
	#if defined(CC_MT5387) ||defined(CC_MT5363)
	UINT32 _dwVal = 0;
	if (i4Argc != 2)
	{
        Printf("***************LBW = %x*********************\n", IO32ReadFldAlign(TMDS_CTRL0, HDMI_LBW));
        Printf("Usage Example: LBW 0x2\n");
        return 0;
	}
	_dwVal = (UINT8)StrToInt(szArgv[1]);
	Printf("***************pre LBW = %x*********************\n", IO32ReadFldAlign(TMDS_CTRL0, HDMI_LBW));
    vIO32WriteFldAlign(TMDS_CTRL0,_dwVal, HDMI_LBW);
	Printf("***************now LBW = %x*********************\n", IO32ReadFldAlign(TMDS_CTRL0, HDMI_LBW));
	#endif

	#if defined(CC_MT5365) || defined(CC_MT5395)
		#if !Support_HW_Adaptive_EQ  // Fix EQ
			UINT32 _dwVal = 0;
			if (i4Argc != 3)
			{
		        Printf("***************HDMI_EQBIAS = 0x%x, HDMI_PLLBIAS = 0x%x*********************\n", IO32ReadFldAlign(ANA_INTF_1,RG_HDMI_EQBIAS), IO32ReadFldAlign(ANA_INTF_2,RG_HDMI_PLLBIAS));
		        Printf("Usage Example: LBW 0x4 0x2\n");
		        return 0;
			}
			_dwVal = (UINT8)StrToInt(szArgv[1]);
			Printf("***************pre HDMI_EQBIAS = 0x%x*********************\n", IO32ReadFldAlign(ANA_INTF_1,RG_HDMI_EQBIAS));
			vIO32WriteFldAlign(ANA_INTF_1,_dwVal, RG_HDMI_EQBIAS);
			Printf("***************now HDMI_EQBIAS = 0x%x*********************\n", IO32ReadFldAlign(ANA_INTF_1, RG_HDMI_EQBIAS));
			_dwVal = (UINT8)StrToInt(szArgv[2]);
			Printf("***************pre HDMI_PLLBIAS = 0x%x*********************\n", IO32ReadFldAlign(ANA_INTF_2, RG_HDMI_PLLBIAS));
			vIO32WriteFldAlign(ANA_INTF_2,_dwVal, RG_HDMI_PLLBIAS);
			Printf("***************now HDMI_PLLBIAS = 0x%x*********************\n", IO32ReadFldAlign(ANA_INTF_2, RG_HDMI_PLLBIAS));
		#endif
	#endif
	return 0;
}

static INT32 _u1vHDMIEQGain(INT32 i4Argc, const CHAR **szArgv)
{
	#if defined(CC_MT5387) ||defined(CC_MT5363)
	UINT32 _dwVal = 0;
	if (i4Argc != 2)
	{
		Printf("***************EQ_GAIN = %x*********************\n", IO32ReadFldAlign(TMDS_CTRL1, EQ_GAIN));
        Printf("Usage Example: EQG 0x2\n");
        return 0;
	}
	_dwVal = (UINT8)StrToInt(szArgv[1]);
	Printf("***************pre EQ_GAIN = %x*********************\n", IO32ReadFldAlign(TMDS_CTRL1, EQ_GAIN));
    vIO32WriteFldAlign(TMDS_CTRL1,_dwVal, EQ_GAIN);
	Printf("***************now EQ_GAIN = %x*********************\n", IO32ReadFldAlign(TMDS_CTRL1, EQ_GAIN));
	#endif

	#if defined(CC_MT5365) || defined(CC_MT5395)
		#if !Support_HW_Adaptive_EQ  // Fix EQ
			UINT32 _dwVal = 0;
			if (i4Argc != 2)
			{
		        Printf("***************HDMI_EQ_GAIN = 0x%x*********************\n", IO32ReadFldAlign(ANA_INTF_2,RG_HDMI_EQ_GAIN));
		        Printf("Usage Example: EQG 0x0\n");
		        return 0;
			}
			_dwVal = (UINT8)StrToInt(szArgv[1]);
			Printf("***************pre HDMI_EQ_GAIN = 0x%x*********************\n", IO32ReadFldAlign(ANA_INTF_2,RG_HDMI_EQ_GAIN));
			vIO32WriteFldAlign(ANA_INTF_2,_dwVal, RG_HDMI_EQ_GAIN);
			Printf("***************now HDMI_EQ_GAIN = 0x%x*********************\n", IO32ReadFldAlign(ANA_INTF_2, RG_HDMI_EQ_GAIN));
		#endif
	#endif
	return 0;
}

extern UINT8 _bDviMdChgCntTHR ;
extern UINT8 _bDviDeChgCntTHR ;
extern UINT8 _bDviPixClkChgCntTHR ;
#if CCIR_decode_444
extern UINT8 _bCCIRNeedModeChgCntTHR;  // jiewen, 20100519
#endif
extern UINT32 _wDviTmpTHR ;
extern UINT32  _wDVI_WAIT_STABLE_COUNTTHR ;
extern UINT8   _bHDMISTABLECNT;
static INT32 _u1HDMIVSTABLECNT(INT32 i4Argc, const CHAR **szArgv)
{
   if(i4Argc != 2)
   	{
     Printf(" Current  HDMISTABLECNT is %d \n",_bHDMISTABLECNT); 
	 return 0;
    }
   _bHDMISTABLECNT=(UINT8)StrToInt(szArgv[1]);
   Printf(" _bHDMISTABLECNT has been set %d \n",_bHDMISTABLECNT);
   return 0;
}
#if defined(CC_MT5399) || defined(CC_MT5890)
static INT32 _u1HDMISPDINFOCHECK(INT32 i4Argc, const CHAR **szArgv)
{
	vHDMISPDDebugMsg();
	return 0;
}
static INT32 _u1HDMISPDINFOSET(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc != 7)
	{
		printf("set method:spdflag/spdtest/vdrname0/vdrname1/prdspt0/prdspt1/SrcInfo\n");
		return 0;
	}
	u1IsSPDFlag = (UINT8)StrToInt(szArgv[1]);
	u1SpdTest = (UINT8)StrToInt(szArgv[2]);
	u4SpdData.VdrNam[0] = (UINT32)StrToInt(szArgv[3]);
	u4SpdData.VdrNam[1] =(UINT32)StrToInt(szArgv[4]);
	u4SpdData.PrdDspt[0] = (UINT32)StrToInt(szArgv[5]);
	u4SpdData.SrcInfo = (UINT8)StrToInt(szArgv[6]);
	return 0;
}
#endif
static INT32 _u1vHDMITHR(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 6)
        {
			Printf(" _bDviMdChgCntTHR = %d\n", _bDviMdChgCntTHR);
			Printf(" _bDviDeChgCntTHR = %d\n", _bDviDeChgCntTHR);
			Printf("_bDviPixClkChgCntTHR = %d\n", _bDviPixClkChgCntTHR);
			Printf(" _wDviTmpTHR = %d\n", (int)_wDviTmpTHR);
			Printf(" _wDVI_WAIT_STABLE_COUNTTHR = %d\n", (int)_wDVI_WAIT_STABLE_COUNTTHR);
		#if CCIR_decode_444
			Printf("_bCCIRNeedModeChgCntTHR = %d\n", _bCCIRNeedModeChgCntTHR);  // jiewen, 20100519
		#endif
            return 0;
        }
		Printf("pre _bDviMdChgCntTHR = %d\n", _bDviMdChgCntTHR);
		Printf("pre _bDviDeChgCntTHR = %d\n", _bDviDeChgCntTHR);
		Printf("pre _bDviPixClkChgCntTHR = %d\n", _bDviPixClkChgCntTHR);
		Printf("pre _wDviTmpTHR = %d\n", (int)_wDviTmpTHR);
		Printf("pre _wDVI_WAIT_STABLE_COUNTTHR = %d\n", (int)_wDVI_WAIT_STABLE_COUNTTHR);
	#if CCIR_decode_444
		Printf("pre _bCCIRNeedModeChgCntTHR = %d\n", _bCCIRNeedModeChgCntTHR);  // jiewen, 20100519
	#endif

		_bDviMdChgCntTHR = (UINT8)StrToInt(szArgv[1]);
		_bDviDeChgCntTHR = (UINT8)StrToInt(szArgv[2]);
		_bDviPixClkChgCntTHR = (UINT8)StrToInt(szArgv[3]) ;
		_wDviTmpTHR = (UINT8)StrToInt(szArgv[4]);
		_wDVI_WAIT_STABLE_COUNTTHR = (UINT8)StrToInt(szArgv[5]);
	#if CCIR_decode_444
		_bCCIRNeedModeChgCntTHR = (UINT8)StrToInt(szArgv[6]);  // jiewen, 20100519
	#endif
		
		Printf("now _bDviMdChgCntTHR = %d\n", _bDviMdChgCntTHR);
		Printf("now _bDviDeChgCntTHR = %d\n", _bDviDeChgCntTHR);
		Printf("now _bDviPixClkChgCntTHR = %d\n", _bDviPixClkChgCntTHR);
		Printf("now _wDviTmpTHR = %d\n", (int)_wDviTmpTHR);
		Printf("now _wDVI_WAIT_STABLE_COUNTTHR = %d\n", (int)_wDVI_WAIT_STABLE_COUNTTHR);
	#if CCIR_decode_444
		Printf("now _bCCIRNeedModeChgCntTHR = %d\n", _bCCIRNeedModeChgCntTHR);  // jiewen, 20100519
	#endif
        return 0;
}
static INT32 _u1vHDMIHPDHigh(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 2)
        {
                Printf("Usage: %s switch\n", szArgv[0]);
                return 0;
        }
        vHDMIHPDHigh((E_HDMI_SWITCH_NUM)StrToInt(szArgv[1]));
        return 0;
}
static INT32 _u1vHDMIHPDLow(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 2)
        {
                Printf("Usage: %s switch\n", szArgv[0]);
                return 0;
        }
        vHDMIHPDLow((E_HDMI_SWITCH_NUM)StrToInt(szArgv[1]));
        return 0;
}
static INT32 _u1vHDMIDebug(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 3)
        {
			Printf("Usage: %s cmd_number  value\n", szArgv[0]);
			Printf("1. toggle TDFIFO and RTCKAUTO\n");
			Printf("2. HDMI reset\n");
			Printf("3. HDMI Core	reset\n");
			Printf("4. DVI pattern\n");
			Printf("5. EQ adjust\n");
			Printf("6. Disable HW auto mute\n");
			Printf("7. Reset audio FIFO\n");
			Printf("8. Stop DVI detec\n");
			Printf("9. Stop HDMI and DVI detect\n");
			Printf("10. Get EQ setting\n");
			Printf("11. HDMI PHY Reset||HDMI_RST_ALL:1	HDMI_RST_EQ:2 HDMI_RST_DEEPCOLOR:3	HDMI_RST_FIXEQ:4  HDMI_RST_RTCK:5  HDMI_RST_DIGITALPHY:6\n");
			Printf("12. No Vendor Specific InfoFrame\n");
			Printf("13.force set mhl mode||mhl mode:1  hdmi mode:0\n");
			Printf("14.force set mhl packet pixel mode||ppmode:1  normal mode:0\n");
			Printf("15.Equalizer offset calibration 7--0--8--15\n");
			Printf("16.set HDCP key\n");
			Printf("17.query power 5v staus:1/2/3/4\n");
                return 0;
        }
        vHDMIDebug((UINT8)StrToInt(szArgv[1]),(UINT32)StrToInt(szArgv[2]));
        return 0;
}
static INT32 _u1vHDMIGetInfoFrame(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 2)
        {
                Printf("Usage: %s (ex:0,1,2,3,4)\n", szArgv[0]);
                return 0;
        }
        vHDMIGetInfoFrame((UINT8)StrToInt(szArgv[1]));
        return 0;
}
static INT32 _u1vHDMIHPDIndepCtrl(INT32 i4Argc, const CHAR **szArgv)
{
        if (i4Argc != 2)
        {
                Printf("Usage: %s 1 (5v)  or 0 (CKDT) \n", szArgv[0]);
                return 0;
        }
        vHDMIHPDIndepCtrl((UINT8)StrToInt(szArgv[1]));
        return 0;
}


static INT32 _u1vHDMIDebugMsg(INT32 i4Argc, const CHAR **szArgv)
{
	//if (i4Argc != 2)
	//{
	//	Printf("Usage: %s cmd\n", szArgv[0]);
	//	return 0;
	//}
	vHDMIDebugMsg();
	return 0;
}

static INT32 _u1vHDMIHDCPKey(INT32 i4Argc, const CHAR **szArgv)
{
	vHDMIHDCPKey();
	return 0;
}

static INT32 _u1vHDMIReloadHDCPKey(INT32 i4Argc, const CHAR **szArgv)
{
	vHDMIReloadHDCPKey();
	return 0;
}

static INT32 _u1vHDMIHDCPSelfBist(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s mode\n", szArgv[0]);
		return 0;
	}

	vHDMIHDCPSelfBist((UINT8)StrToInt(szArgv[1]));
	return 0;
}

static INT32 _u1vHDMIBypassVdo(INT32 i4Argc, const CHAR **szArgv)
{
#if 0
	UINT8 cmd;
	if (i4Argc != 2)
	{
		Printf("Usage: %s cmd\n", szArgv[0]);
		return 0;
	}
	cmd = (UINT8)StrToInt(szArgv[1]);
	if (cmd == 0x01)
	{
		vHDMIBypassVdo();
	}
	else if (cmd == 0x02)
	{
		// HDMI APLL
		CKGEN_WRITE32(0x380, 0x000000c0);
		CKGEN_WRITE32(0x404, 0x00000000);
		CKGEN_WRITE32(0x52c, 0x1c000000);
		CKGEN_WRITE32(0x304, 0x01088000);
		vHDMIWriteDevL(0x8b, 0x01);
		vHDMIWriteDevH(0x3f, 0xcf);
		vHDMIWriteDevL(0x8c, 0xc4);
		vHDMIWriteDevL(0x88, 0x00);
	}
	else if (cmd == 0x03)
	{
		// HDMI TMDSPLL
		CKGEN_WRITE32(0x380, 0x000000c0);
		CKGEN_WRITE32(0x400, 0x000333c0);
		IO_WRITE32(0x20000000, 0x8094, 0x00050003);
	}
#endif
	return 0;
}

static INT32 _u1vHDMIvLoadColorMatrix(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s idx\n", szArgv[0]);
		return 0;
	}
	return 0;
}

static INT32 _u1vHDMIMCMHdcpWrite(INT32 i4Argc, const CHAR **szArgv)
{
	if(bHDMIMCMHdcpWrite()!=TRUE)
	{
		Printf("MCM EEPROM Write HDCP fail\n");
	}
	else
	{
		Printf("MCM EEPROM Write HDCP success\n");
	}
	return 0;
}
static INT32 _u1vHDMISetHDMIState(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s state\n", szArgv[0]);
		return 0;
	}
	vHDMISetHDMIState((UINT8)StrToInt(szArgv[1]));
	return 0;
}

static INT32 _u1vHDMISetSwitch(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s switch\n", szArgv[0]);
		return 0;
	}
	vHDMISetSwitch((E_HDMI_SWITCH_NUM)StrToInt(szArgv[1]));
	return 0;
}

extern UINT8 u1HdmiColorMode;
static INT32 _u1vHDMIModeSel(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s <Auto(0)/Graphic(1)/Video(2)>\n", szArgv[0]);
		return 0;
	}
	u1HdmiColorMode = (UINT8)StrToInt(szArgv[1]);
	return 0;
}

static INT32 _u1vHDMIForce3D(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 4)
	{
		Printf("Usage: %s Force3D_En(0/1) 3DStructure 3DExtData>\n", szArgv[0]);
		Printf("3DStructure: (0)FP (1)FieldAlternative (2)LineAlternative (3)SBS_F (4)LDepth (5)LDepthGraph (6)TB (8)SBS_H\n");
		Printf("3DExtData: Default value=0; If 3DStructure=SBS_H, should fill it!\n");
		return 0;
	}

	_fgForce3D = (UINT8)StrToInt(szArgv[1]);
	_bForce3DStructure = (UINT8)StrToInt(szArgv[2]);
	_bForce3DExtData = (UINT8)StrToInt(szArgv[3]);
	
	_bDviChkState = 2;  // DVI_WAIT_STABLE
    vDviInitial();
	
	return 0;
}

static INT32 _u1vHDMIForce3DTo2D(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 temp;
	
	if (i4Argc != 3)
	{
		Printf("Usage: %s Force2D_En(0/1) Input_3D_Type\n", szArgv[0]);
		Printf("Input_3D_Type: (9)FP_P (8)FP_I (13)SBS_P (12)SBS_I (11)TB_P (10)TB_I\n");
		return 0;
	}

	_fgForce2D = (UINT8)StrToInt(szArgv[1]);
	temp = (UINT8)StrToInt(szArgv[2]);
	_bQueryMode = 0x20000 | (temp);
	
	_bDviChkState = 2;  // DVI_WAIT_STABLE
    vDviInitial();
	
	return 0;
}

static INT32 _u1vHDMIPHYTune(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 au1Buf[5] = {0};
    
	if (i4Argc != 6)
	{
		Printf("Usage: %s KPGain(0~7) RXmode(0~3) EQ_Gain(0,1,3) ZSel(0~3) VRef(1,2,4)\n", szArgv[0]);
              Printf("ManuallyEQ tuning %d  \n", _bEQTuning);
		Printf("Pre_KPGain=%d\n", _bKPGain);
		Printf("Pre_RXMode=%d\n", _bRXMode);
		Printf("Pre_EQGain=%d\n", _bEQGain);
		Printf("Pre_ZSel=%d\n", _bZSel);
		Printf("Pre_VRef=%d\n", _bVRef);
		return 0;
	}

	
	au1Buf[0] = (UINT8)StrToInt(szArgv[1]);
	au1Buf[1] = (UINT8)StrToInt(szArgv[2]);
	au1Buf[2]  = (UINT8)StrToInt(szArgv[3]);
	au1Buf[3]  = (UINT8)StrToInt(szArgv[4]);
	au1Buf[4]  = (UINT8)StrToInt(szArgv[5]);

      if (au1Buf[0]  == 0 && au1Buf[1]  == 0 && au1Buf[2]  == 0 && au1Buf[3]  == 0 && au1Buf[4]  == 0)
      {
          _bEQTuning = 0;
          Printf("EQ tuning off \n");
      }
      else
      {
	  _bKPGain= au1Buf[0];
	  _bRXMode= au1Buf[1];
  	  _bEQGain= au1Buf[2];
	  _bZSel= au1Buf[3];
 	  _bVRef= au1Buf[4];
	
          _bEQTuning = 1;
          Printf("EQ tuning on \n");          
	Printf("New_KPGain=%d\n", _bKPGain);
	Printf("New_RXMode=%d\n", _bRXMode);
	Printf("New_EQGain=%d\n", _bEQGain);
	Printf("New_ZSel=%d\n", _bZSel);
	Printf("New_VRef=%d\n", _bVRef);
      }
	
	return 0;
}

static INT32 _u1vHDMIEqQuery(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4KPGain = 0;
    UINT32 u4RXMode = 0;
    UINT32 u4EQGain = 0;
    UINT32 u4ZSel = 0;
    UINT32 u4VRef = 0;

    Printf("EQ setting: KPGain(0~7) RXmode(0~3) EQ_Gain(0,1,3) ZSel(0~3) VRef(1,2,4)\n");
    if (DRVCUST_OptQuery(eHDMIEQ_LOW1_KPGAIN, &u4KPGain) != 0)
    {
        u4KPGain = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_LOW1_RXMODE, &u4RXMode) != 0)
    {
        u4RXMode = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_LOW1_EQGAIN, &u4EQGain) != 0)
    {
        u4EQGain = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_LOW1_ZSEL, &u4ZSel) != 0)
    {
        u4ZSel = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_LOW1_VREF, &u4VRef) != 0)
    {
        u4VRef = 0xf;
     }
    Printf("EQ low1: %d %d %d %d %d\n",u4KPGain,u4RXMode,u4EQGain,u4ZSel,u4VRef);

    if (DRVCUST_OptQuery(eHDMIEQ_LOW2_KPGAIN, &u4KPGain) != 0)
    {
        u4KPGain = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_LOW2_RXMODE, &u4RXMode) != 0)
    {
        u4RXMode = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_LOW2_EQGAIN, &u4EQGain) != 0)
    {
        u4EQGain = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_LOW2_ZSEL, &u4ZSel) != 0)
    {
        u4ZSel = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_LOW2_VREF, &u4VRef) != 0)
    {
        u4VRef = 0xf;
     }
    Printf("EQ low2: %d %d %d %d %d\n",u4KPGain,u4RXMode,u4EQGain,u4ZSel,u4VRef);
    
    if (DRVCUST_OptQuery(eHDMIEQ_MIDDLE_KPGAIN, &u4KPGain) != 0)
    {
        u4KPGain = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_MIDDLE_RXMODE, &u4RXMode) != 0)
    {
        u4RXMode = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_MIDDLE_EQGAIN, &u4EQGain) != 0)
    {
        u4EQGain = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_MIDDLE_ZSEL, &u4ZSel) != 0)
    {
        u4ZSel = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_MIDDLE_VREF, &u4VRef) != 0)
    {
        u4VRef = 0xf;
     }
    Printf("EQ mid.: %d %d %d %d %d\n",u4KPGain,u4RXMode,u4EQGain,u4ZSel,u4VRef);
    
    if (DRVCUST_OptQuery(eHDMIEQ_HIGH_KPGAIN, &u4KPGain) != 0)
    {
        u4KPGain = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_HIGH_RXMODE, &u4RXMode) != 0)
    {
        u4RXMode = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_HIGH_EQGAIN, &u4EQGain) != 0)
    {
        u4EQGain = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_HIGH_ZSEL, &u4ZSel) != 0)
    {
        u4ZSel = 0xf;
    }
    if (DRVCUST_OptQuery(eHDMIEQ_HIGH_VREF, &u4VRef) != 0)
    {
        u4VRef = 0xf;
     }
    Printf("EQ high: %d %d %d %d %d\n",u4KPGain,u4RXMode,u4EQGain,u4ZSel,u4VRef);
	
     return 0;

	 }
//#endif 

#ifdef CC_MHL_SENSE_NOT_CONNECTED
extern UINT8 u1SetCdsenseStatus(UINT8 u1ForceM);
static INT32 _u1ForceMHLMode(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1ForceM = (UINT8)StrToInt(szArgv[1]);
    u1SetCdsenseStatus(u1ForceM);
    return 0;
}
#endif
