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
#include "drv_w2dr2r.h"
#ifndef CC_CLI
#define CC_CLI
#endif

static INT32 _u1vW2DInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vR2RInit(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vW2DConfi(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vR2RConfi(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vR2RVb1(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vR2RCapture(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vW2dEnable(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u1vW2dDisable(INT32 i4Argc, const CHAR **szArgv);
static INT32 vCreateW2DFGPAThread(INT32 i4Argc, const CHAR **szArgv);
static INT32 vSetintCounter(INT32 i4Argc, const CHAR **szArgv);
static INT32 vManualSwap(INT32 i4Argc, const CHAR **szArgv);


// Table for W2D command entry
CLI_EXEC_T arW2dCmdTbl[] = {
  //#ifdef CC_CLI
  {"w2dInit", "w2di", _u1vW2DInit, NULL, "w2d initialization", CLI_SUPERVISOR},
  {"r2rInit", "r2ri", _u1vR2RInit, NULL, "r2rInit", CLI_SUPERVISOR},
  {"w2d confi", "w2dconfi", _u1vW2DConfi, NULL, "w2d confi", CLI_SUPERVISOR},
  {"r2r confi", "r2rconfi", _u1vR2RConfi, NULL, "r2r confi", CLI_SUPERVISOR},
  {"r2r vb1", "r2rvb1", _u1vR2RVb1, NULL, "r2r VB1", CLI_SUPERVISOR},
  {"r2r r2rcapture", "r2rcapture", _u1vR2RCapture, NULL, "r2r capture", CLI_SUPERVISOR},
  {"w2d enable", "enable", _u1vW2dEnable, NULL, "enable", CLI_SUPERVISOR},
  {"w2d disable", "disable", _u1vW2dDisable, NULL, "disable", CLI_SUPERVISOR},
  {"w2d Thread", "CreateThread", vCreateW2DFGPAThread, NULL, "CreateThread", CLI_SUPERVISOR},
  {"w2d Set Counter", "SetCounter", vSetintCounter, NULL, "SetintCounter", CLI_SUPERVISOR},  
  {"w2d vManualSwap", "swap", vManualSwap, NULL, "vManualSwap", CLI_SUPERVISOR},
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};




//#ifdef CC_CLI
/******************************************************************************
* Local Function
******************************************************************************/
static INT32 _u1vW2DInit(INT32 i4Argc, const CHAR **szArgv)
{
	vW2DInit();

	Printf("vW2DInit\n");

	return 0;
}
static INT32 _u1vR2RInit(INT32 i4Argc, const CHAR **szArgv)
{
	vR2RInit();

	Printf("vR2RInit\n");

	return 0;
}
extern 	R2R_CONFI r2r_configures[2];
extern W2D_CONFI w2d_configures[2];

static INT32 _u1vW2DConfi(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc<3)
		return 0;

	Printf("timing index, vsync start\n");
	vConfiW2D(w2d_configures[StrToInt(szArgv[1])],StrToInt(szArgv[2]));
	Printf("W2DConfi\n");
	return 0;
}

static INT32 _u1vR2RConfi(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc<3)
		return 0;
	Printf("timing index , bit\n");
	vConfiR2R(r2r_configures[StrToInt(szArgv[1])], StrToInt(szArgv[2]));
	Printf("R2RConfi\n");
	return 0;
}

extern BOOL fgR2RVB1Enable;
extern BOOL fgR2RCaptureMode;

static INT32 _u1vR2RVb1(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc!=2)
		return 0;
	fgR2RVB1Enable = StrToInt(szArgv[1])?TRUE:FALSE;
	Printf("R2Rvb1 R2RConfiAsVb1 : %s\n",fgR2RVB1Enable?"enable":"disable");
	return 0;
}
static INT32 _u1vR2RCapture(INT32 i4Argc, const CHAR **szArgv)
{
	if(i4Argc!=2)
		return 0;
	fgR2RCaptureMode = StrToInt(szArgv[1])?TRUE:FALSE;
	Printf("R2Rvb1 R2RConfiAsVb1 : %s\n",fgR2RCaptureMode?"enable":"disable");
	return 0;
}
static INT32 _u1vW2dEnable(INT32 i4Argc, const CHAR **szArgv)
{
	BOOL fgLeft;
	if(i4Argc!=2)
		return 0;
	fgLeft = StrToInt(szArgv[1])?1:0;
	vW2DEnable(fgLeft);
	Printf("w2d enable: %s\n",fgLeft?"Left":"Right");
	return 0;
}
static INT32 _u1vW2dDisable(INT32 i4Argc, const CHAR **szArgv)
{
	BOOL fgLeft;
	if(i4Argc!=2)
		return 0;
	fgLeft = StrToInt(szArgv[1])?1:0;
	vW2DDisable(fgLeft);
	Printf("w2d Disable: %s\n",fgLeft?"Left":"Right");
	return 0;
}
static HANDLE_T pW2DFpgaThreadHdl;
extern void vW2DFGPAThread( void * pvArgs);

static INT32 vCreateW2DFGPAThread(INT32 i4Argc, const CHAR **szArgv)
{
	VERIFY(x_thread_create(&pW2DFpgaThreadHdl, 
			"W2DFpgaThread", 2048,  60, vW2DFGPAThread, 0, NULL) == OSR_OK);
	printf("W2DFpgaThread creat!\n");
	return 0;
}
extern UINT32 u4CounterMax;
static INT32 vSetintCounter(INT32 i4Argc, const CHAR **szArgv)
{
	
	if(i4Argc!=2)
		return 0;
	u4CounterMax = StrToInt(szArgv[1]);
	Printf("u4CounterMax : %d\n",u4CounterMax);
	return 0;
}
static INT32 vManualSwap(INT32 i4Argc, const CHAR **szArgv)
{
	
	vW2DFPGAemulation();
	Printf("manual do swap w2d and r2r address!\n");
	return 0;
}

