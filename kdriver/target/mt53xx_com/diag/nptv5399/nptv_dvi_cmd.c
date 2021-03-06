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
 * $RCSfile: nptv_dvi_cmd.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         This file contains CLI implementation of NPTV Video.
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "x_stl_lib.h"
#include "video_def.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_assert.h"
#include "x_pinmux.h"
#include "x_debug.h"

#include "sv_const.h"
#include "drv_dvi.h"

UINT8 _bDviFlag;
#ifndef CC_CLI
#define CC_CLI
#endif

//#ifdef CC_CLI
/* Functions for DVI */
static INT32 _u1vDviConnect(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vDviInitial(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vDviModeDetect(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vDviChkModeChange(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vDVIDebugMsg(INT32 i4Argc, const CHAR **szArgv);
static INT32 _i4DviFw(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vDVITimingSearch(INT32 i4Argc, const CHAR **szArgv);
extern void vDviConnect(UINT8 bchannel, UINT8 fgIsOn);
extern void vDviInitial(void);
extern void vDVIDebugMsg(void);
extern UINT8 bDviStdTimingSearch(UINT8);

//#endif /*#ifdef CC_CLI*/
/******************************************************************************
* CLI Command Table
******************************************************************************/

// Table for DVI command entry
CLI_EXEC_T arDVICmdTbl[] = {
  // #ifdef CC_CLI
  {"connect", "c", _u1vDviConnect, NULL, "DVI connect", CLI_SUPERVISOR},
  {"initial", "i", _u1vDviInitial, NULL, "DVI initial", CLI_SUPERVISOR},
  {"mode detect", "md", _u1vDviModeDetect, NULL, "DVI mode detect", CLI_SUPERVISOR},
  {"mode change", "mc", _u1vDviChkModeChange, NULL, "DVI mode change", CLI_SUPERVISOR},
  {"dmesg", "dmsg", _u1vDVIDebugMsg, NULL, "Print DVI status", CLI_GUEST},
  {"fw", "fw", _i4DviFw, NULL, "Fw on/off", CLI_SUPERVISOR},
  {"timing search", "ts", _u1vDVITimingSearch, NULL, "DVI timing search", CLI_SUPERVISOR},
  //#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

//#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/

static INT32 _u1vDviConnect(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: connect on(1)/off(0)\n");
		return 0;
	}

	if ((UINT8)StrToInt(szArgv[1]) == 1)
	{
		vDviConnect(SV_VP_MAIN, SV_ON);
	}
	else
	{
		vDviConnect(SV_VP_MAIN, SV_OFF);
	}

	return 0;
}

static INT32 _u1vDviInitial(INT32 i4Argc, const CHAR **szArgv)
{
	vDviInitial();

	return 0;
}

static INT32 _u1vDviModeDetect(INT32 i4Argc, const CHAR **szArgv)
{
	vDviModeDetect();

	return 0;
}

static INT32 _u1vDviChkModeChange(INT32 i4Argc, const CHAR **szArgv)
{
	vDviChkModeChange();

	return 0;
}

static INT32 _u1vDVIDebugMsg(INT32 i4Argc, const CHAR **szArgv)
{
	vDVIDebugMsg();

	return 0;
}


static INT32 _i4DviFw(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc==2)
	{
		if(StrToInt(szArgv[1]) == 1)
	{
			vClrDviFlg(0x80);
		}
		else if(StrToInt(szArgv[1]) == 0)
		{
			vSetDviFlg(0x80);
		}
		else
		{
		Printf("fw	0/1 \n");
		}
	}
	else
	{
		Printf("fw	0/1 \n");
	}


	return 0;
}

static INT32 _u1vDVITimingSearch(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
		Printf("Usage: %s\n", szArgv[0]);
		return 0;
	}

	bDviStdTimingSearch((UINT8)StrToInt(szArgv[1]));

	return 0;
}

//#endif // #ifdef CC_CLI

