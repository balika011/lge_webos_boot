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
 * $Author: 
 * $Date: 
 * $RCSfile: 
 * $Revision: 
 *
 * Description:
 *         This file contains CLI implementation of DGO.
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
#include "util.h"

#include "drv_display.h"
#include "drv_dgo.h"
#include "hw_dgo.h"
#include "sif_if.h"

#ifndef CC_CLI
#define CC_CLI
#endif

#define DGI_I2C_CH   0x04
#define DGI_DEV_ADDR 0x3a

/* Functions for DGO*/
#ifdef CC_CLI
static INT32 _u4Dgo_Init(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u4Dgo_3DSetting(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u4DgoOff(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u4Dgo_Pattern(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u4Dgo_Set8193Mode(INT32 i4Argc, const CHAR **szArgv);
static INT32 _u4Dgo_Read8193(INT32 i4Argc, const CHAR **szArgv);
#endif

/******************************************************************************
* CLI Command Table
******************************************************************************/
// Table for DGO command entry
CLI_EXEC_T arDgoCmdTbl[] = {
#ifdef CC_CLI
  {"DGO init", "i", _u4Dgo_Init, NULL, "DGO init", CLI_SUPERVISOR},
  {"DGO 3D Setting", "3d", _u4Dgo_3DSetting, NULL, "DGO 3D", CLI_SUPERVISOR},
  {"DGO OFF", "off", _u4DgoOff, NULL, "DGO OFF", CLI_SUPERVISOR},
  {"DGO test pattern", "pt", _u4Dgo_Pattern, NULL, "DGO test pattern", CLI_SUPERVISOR},
  {"DGI(8193) mode setting", "dgim", _u4Dgo_Set8193Mode, NULL, "DGI(8193) mode setting", CLI_SUPERVISOR},
  {"DGI(8193) REG read", "dgir", _u4Dgo_Read8193, NULL, "DGI(8193) REG read", CLI_SUPERVISOR},
#endif
  {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

/******************************************************************************
* Local Function
******************************************************************************/
#ifdef CC_CLI
static INT32 _u4Dgo_Init(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
        Printf("Usage: %s  DispMode\n", szArgv[0]);
        Printf("<DispMode>\n");
        Printf("1: 480p\n");
        Printf("2: 576p\n");
        Printf("3: 720p\n");
        Printf("4: 1080p\n");
        Printf("5: 1080p_24_FP\n");
        Printf("6: 1080p_25_FP\n");
        Printf("7: 1080p_30_FP\n");
        return 1;
	}
	
    vDrvDGOInit(StrToInt(szArgv[1]));
    return 0;
}

static INT32 _u4Dgo_3DSetting(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 2)
	{
        Printf("Usage: %s  DispMode\n", szArgv[0]);
        Printf("<DispMode>\n");
        Printf("5: 1080p_24_FP\n");
        Printf("6: 1080p_25_FP\n");
        Printf("7: 1080p_30_FP\n");
        return 1;
	}
	
    vDrvDGO3DSetting(StrToInt(szArgv[1]));
    return 0;
}

static INT32 _u4DgoOff(INT32 i4Argc, const CHAR **szArgv)
{
    vDrvDGOOff();
    return 0;
}

static INT32 _u4Dgo_Pattern(INT32 i4Argc, const CHAR **szArgv)
{
	if (i4Argc != 3)
	{
        Printf("Usage: %s  <OnOff> <Mode>\n", szArgv[0]);
        Printf("<OnOff>\n");
        Printf("0: OFF\n");
        Printf("1: ON\n");
        Printf("<Mode>\n");
        Printf("1: 480P\n");
        Printf("2: 576P\n");
        printf("3: 720P\n");
        printf("4: 1080P\n");
        return 1;
	}
	
    vDrvDGOPatternGen(StrToInt(szArgv[1]));
    return 0;
}

static void vDGISet_480P(void)
{
	UINT8 u1Array[4];
	UINT8 i;

	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x42;
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x02;  // 0x00000002
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x19c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x82;  // 0x00000082
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1a0, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x0d;  // 0x0000000d
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1a4, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x75;  // 0x00000075
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x52;  // 0x00000052
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0xa8;  // 0x000000a8
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x00;  // 0x00000000
    for (i=0;i<12;i++)
    {
		SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
		vUtDelay1ms(200);
		Printf("%d", i);
	}
	Printf("\n");
	u1Array[2] = 0x03;
	u1Array[3] = 0x23;  // 0x00000323
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1254, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x00;
	u1Array[3] = 0x04;  // 0x00000004
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x101c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x99;
	u1Array[3] = 0x99;  // 0x00009999
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1328, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x20;
	u1Array[2] = 0x00;
	u1Array[3] = 0x94;  // 0x00200094
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1334, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0xf4;
	u1Array[1] = 0xf0;
	u1Array[2] = 0x14;
	u1Array[3] = 0x00;  // 0xf4f01400
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1338, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x12;  // 0x00000012
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1344, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x11;
	u1Array[1] = 0xff;
	u1Array[2] = 0x00;
	u1Array[3] = 0x00;  // 0x11ff0000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1348, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x30;
	u1Array[2] = 0x00;
	u1Array[3] = 0x94;  // 0x00300094
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1334, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x02;  // 0x00000002
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x02d4, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x00;  // 0x00000000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x02d8, u1Array, 4);
	vUtDelay1ms(200);
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0448, u1Array, 4);  // 0x00000000
	vUtDelay1ms(200);
	u1Array[2] = 0x06;
	u1Array[3] = 0x51;  // 0x00000651
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0450, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x80;
	u1Array[1] = 0x03;
	u1Array[2] = 0x00;
	u1Array[3] = 0x05;  // 0x80030005
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0608, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x0f;  // 0x0000000f
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x065c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x06;
	u1Array[2] = 0x00;
	u1Array[3] = 0x40;  // 0x00060040
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0604, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x10;
	u1Array[2] = 0x40;
	u1Array[3] = 0x00;  // 0x00104000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x061c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x02;
	u1Array[1] = 0x0d;
	u1Array[2] = 0x03;
	u1Array[3] = 0x5a;  // 0x020d035a
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0624, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x06;
	u1Array[2] = 0x00;
	u1Array[3] = 0x3e;  // 0x0006003e
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0628, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x7b;
	u1Array[2] = 0x03;
	u1Array[3] = 0x4a;  // 0x007b034a
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0630, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x25;
	u1Array[2] = 0x02;
	u1Array[3] = 0x04;  // 0x00250204
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0634, u1Array, 4);
	vUtDelay1ms(200);  
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0638, u1Array, 4);  // 0x00250204
	vUtDelay1ms(200);
	u1Array[0] = 0x04;
	u1Array[1] = 0x1a;
	u1Array[2] = 0x03;
	u1Array[3] = 0x54;  // 0x041a0354
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0620, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x01;  // 0x00000001
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0600, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x01;
	u1Array[3] = 0x02;  // 0x00000102
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x060c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x00;
	u1Array[3] = 0x60;  // 0x00000060
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x062c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x02;
	u1Array[1] = 0x0d;
	u1Array[2] = 0x03;
	u1Array[3] = 0x5a;  // 0x020d035a
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0700, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x06;
	u1Array[2] = 0x00;
	u1Array[3] = 0x3e;  // 0x0006003e
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0704, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x24;
	u1Array[2] = 0x00;
	u1Array[3] = 0x7a;  // 0x0024007a
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0708, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x01;
	u1Array[1] = 0xe0;
	u1Array[2] = 0x02;
	u1Array[3] = 0xd0;  // 0x01e002d0
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x070c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x02;
	u1Array[3] = 0x00;  // 0x00000200
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0710, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0xff;
	u1Array[2] = 0x88;
	u1Array[3] = 0x44;  // 0x00ff8844
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0714, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0xaa;
	u1Array[2] = 0xff;
	u1Array[3] = 0x55;  // 0x00aaff55
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0718, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x0e;
	u1Array[2] = 0x00;
	u1Array[3] = 0x40;  // 0x000e0040
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0604, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x06;  // 0x00060040
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0604, u1Array, 4);
	vUtDelay1ms(200);
}

void vDGISet_576P(void)
{
	UINT8 u1Array[4];
	UINT8 i;
	
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x42;
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x02;  // 0x00000002
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x19c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x82;  // 0x00000082
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1a0, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x0d;  // 0x0000000d
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1a4, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x75;  // 0x00000075
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x52;  // 0x00000052
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0xa8;  // 0x000000a8
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x00;  // 0x00000000
    for (i=0;i<12;i++)
    {
		SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
		vUtDelay1ms(200);
		Printf("%d", i);
	}
	Printf("\n");
	u1Array[2] = 0x03;
	u1Array[3] = 0x23;  // 0x00000323
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1254, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x00;
	u1Array[3] = 0x04;  // 0x00000004
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x101c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x99;
	u1Array[3] = 0x99;  // 0x00009999
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1328, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x20;
	u1Array[2] = 0x00;
	u1Array[3] = 0x94;  // 0x00200094 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1334, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0xf4;
	u1Array[1] = 0xf0;
	u1Array[2] = 0x14;
	u1Array[3] = 0x00;  // 0xf4f01400 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1338, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x12;  // 0x00000012
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1344, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x11;
	u1Array[1] = 0xff;
	u1Array[2] = 0x00;
	u1Array[3] = 0x00;  // 0x11ff0000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1348, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x30;
	u1Array[2] = 0x00;
	u1Array[3] = 0x94;  // 0x00300094 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1334, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x02;  // 0x00000002
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x02d4, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x00;  // 0x00000000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x02d8, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x06;
	u1Array[3] = 0x51;  // 0x00000651 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0450, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x00;
	u1Array[3] = 0x00;  // 0x00000000 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0448, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x80;
	u1Array[1] = 0x03;
	u1Array[2] = 0x00;
	u1Array[3] = 0x05;  // 0x80030005 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0608, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x0f;  // 0x0000000f
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x065c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x10;
	u1Array[2] = 0x40;
	u1Array[3] = 0x00;  // 0x00104000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x061c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x02;
	u1Array[1] = 0x71;
	u1Array[2] = 0x03;
	u1Array[3] = 0x60;  // 0x02710360 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0624, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x05;
	u1Array[2] = 0x00;
	u1Array[3] = 0x40;  // 0x00050040 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0628, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x85;
	u1Array[2] = 0x03;
	u1Array[3] = 0x54;  // 0x00850354 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0630, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x2d;
	u1Array[2] = 0x02;
	u1Array[3] = 0x6c;  // 0x002d026c (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0634, u1Array, 4);
	vUtDelay1ms(200);  
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0638, u1Array, 4);  // 0x002a0461 (*)
	vUtDelay1ms(200);
	u1Array[0] = 0x04;
	u1Array[1] = 0xe0;
	u1Array[2] = 0x03;
	u1Array[3] = 0x5c;  // 0x04e0035c (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0620, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x01;  // 0x00000000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0600, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x01;
	u1Array[3] = 0x02;  // 0x00000102 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x060c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x60;  // 0x00000060 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x062c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x02;
	u1Array[1] = 0x71;
	u1Array[2] = 0x03;
	u1Array[3] = 0x60;  // 0x02710360 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0700, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x03;
	u1Array[2] = 0x00;
	u1Array[3] = 0x05;  // 0x00030005 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0704, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x10;
	u1Array[2] = 0x00;
	u1Array[3] = 0x8a;  // 0x0010008a (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0708, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x02;
	u1Array[1] = 0x40;
	u1Array[2] = 0x02;
	u1Array[3] = 0xd0;  // 0x024002d0 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x070c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x02;
	u1Array[3] = 0x00;  // 0x00000200
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0710, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0xff;
	u1Array[2] = 0x88;
	u1Array[3] = 0x44;  // 0x00ff8844
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0714, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0xaa;
	u1Array[2] = 0xff;
	u1Array[3] = 0x55;  // 0x00aaff55
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0718, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x0e;
	u1Array[2] = 0x00;
	u1Array[3] = 0x40;  // 0x000e0040 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0604, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x06;  // 0x00060040 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0604, u1Array, 4);
	vUtDelay1ms(200);
}

void vDGISet_720P(void)
{
	UINT8 u1Array[4];
	UINT8 i;

	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x42;
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x02;  // 0x00000002
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x19c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x82;  // 0x00000082
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1a0, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x0d;  // 0x0000000d
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1a4, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x75;  // 0x00000075
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x52;  // 0x00000052
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0xa8;  // 0x000000a8
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x00;  // 0x00000000
    for (i=0;i<11;i++)
    {
		SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
		vUtDelay1ms(200);
		Printf("%d", i);
	}
	Printf("\n");
	u1Array[2] = 0x03;
	u1Array[3] = 0x23;  // 0x00000323
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1254, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x00;
	u1Array[3] = 0x04;  // 0x00000004
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x101c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x99;
	u1Array[3] = 0x99;  // 0x00009999
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1328, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x20;
	u1Array[2] = 0x00;
	u1Array[3] = 0x8f;  // 0x0020008f (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1334, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0xd4;
	u1Array[1] = 0xa8;
	u1Array[2] = 0x8f;
	u1Array[3] = 0x00;  // 0xd4a88f00 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1338, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x12;  // 0x00000012
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1344, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x11;
	u1Array[1] = 0xff;
	u1Array[2] = 0x00;
	u1Array[3] = 0x00;  // 0x11ff0000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1348, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x30;
	u1Array[2] = 0x00;
	u1Array[3] = 0x8f;  // 0x0030008f (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1334, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x02;  // 0x00000002
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x02d4, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x00;  // 0x00000000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x02d8, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x06;
	u1Array[3] = 0x51;  // 0x00000651 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0450, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x00;
	u1Array[3] = 0x00;  // 0x00000000 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0448, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x80;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x05;  // 0x80000005 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0608, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x0f;  // 0x0000000f
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x065c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x02;
	u1Array[2] = 0x00;
	u1Array[3] = 0x40;  // 0x00020040 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0604, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x10;
	u1Array[2] = 0x40;
	u1Array[3] = 0x00;  // 0x00104000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x061c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x02;
	u1Array[1] = 0xee;
	u1Array[2] = 0x06;
	u1Array[3] = 0x72;  // 0x02ee0672 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0624, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x05;
	u1Array[2] = 0x00;
	u1Array[3] = 0x28;  // 0x00050028 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0628, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x01;
	u1Array[1] = 0x05;
	u1Array[2] = 0x06;
	u1Array[3] = 0x04;  // 0x01050604 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0630, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x1a;
	u1Array[2] = 0x02;
	u1Array[3] = 0xe9;  // 0x001a02e9 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0634, u1Array, 4);
	vUtDelay1ms(200);  
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0638, u1Array, 4);  // 0x001a02e9 (*)
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x02;
	u1Array[2] = 0x06;
	u1Array[3] = 0x6f;  // 0x0002066f (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0620, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x01;  // 0x00000000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0600, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x00;
	u1Array[3] = 0x02;  // 0x00000002 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x060c, u1Array, 4);
	vUtDelay1ms(200);
	
	u1Array[0] = 0x02;
	u1Array[1] = 0xee;
	u1Array[2] = 0x06;
	u1Array[3] = 0x72;  // 0x02ee0672 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0700, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x03;
	u1Array[2] = 0x00;
	u1Array[3] = 0x05;  // 0x00030005 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0704, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x01;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x80;  // 0x01000080 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0708, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x02;
	u1Array[1] = 0xd0;
	u1Array[2] = 0x05;
	u1Array[3] = 0x00;  // 0x02d00500 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x070c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x02;
	u1Array[3] = 0x00;  // 0x00000200
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0710, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0xff;
	u1Array[2] = 0x88;
	u1Array[3] = 0x44;  // 0x00ff8844
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0714, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0xaa;
	u1Array[2] = 0xff;
	u1Array[3] = 0x55;  // 0x00aaff55
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0718, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x0a;
	u1Array[2] = 0x00;
	u1Array[3] = 0x40;  // 0x000a0040 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0604, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x02;  // 0x00020040 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0604, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x80;
	u1Array[1] = 0x00;
	u1Array[2] = 0x02;
	u1Array[3] = 0x00;  // 0x00000200 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0728, u1Array, 4);
	vUtDelay1ms(200);
}

void vDGISet_1080P(UINT8 u13DMode)
{
	UINT8 u1Array[4];
	UINT8 i;

	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x42;
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x02;  // 0x00000002
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x19c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x82;  // 0x00000082
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1a0, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x0d;  // 0x0000000d
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1a4, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x75;  // 0x00000075
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x52;  // 0x00000052
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0xa8;  // 0x000000a8
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x00;  // 0x00000000
    for (i=0;i<12;i++)
    {
		SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x188, u1Array, 4);
		vUtDelay1ms(200);
		Printf("%d", i);
	}
	Printf("\n");
	u1Array[2] = 0x03;
	u1Array[3] = 0x23;  // 0x00000323
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1254, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x00;
	u1Array[3] = 0x04;  // 0x00000004
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x101c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x99;
	u1Array[3] = 0x99;  // 0x00009999
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1328, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x20;
	u1Array[2] = 0x00;
	u1Array[3] = 0x8f;  // 0x0020008f (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1334, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0xb4;
	u1Array[1] = 0x88;
	u1Array[2] = 0x8f;
	u1Array[3] = 0x00;  // 0xb4888f00 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1338, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x12;  // 0x00000012
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1344, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x11;
	u1Array[1] = 0xff;
	u1Array[2] = 0x00;
	u1Array[3] = 0x00;  // 0x11ff0000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1348, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x30;
	u1Array[2] = 0x00;
	u1Array[3] = 0x8f;  // 0x0030008f (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x1334, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x02;  // 0x00000002
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x02d4, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[3] = 0x00;  // 0x00000000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x02d8, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x01;  // 0x00010000 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0448, u1Array, 4);  // 0x00000000
	vUtDelay1ms(200);
	u1Array[0] = 0x80;
	u1Array[1] = 0x00;
	u1Array[2] = 0x06;
	u1Array[3] = 0x51;  // 0x80000651 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0450, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x80;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x05;  // 0x80000005 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0608, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x0f;  // 0x0000000f
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x065c, u1Array, 4);
	vUtDelay1ms(200);
	if (!u13DMode)  // 2d_1080p
	{
		u1Array[1] = 0x02;
		u1Array[2] = 0x00;
		u1Array[3] = 0x40;  // 0x00020040 (*)
	}
	else  // 3d_1080p_24/25/30_FP
	{
		u1Array[3] = 0x40;  // 0x00000040 (*)
	}
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0604, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x10;
	u1Array[2] = 0x40;
	u1Array[3] = 0x00;  // 0x00104000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x061c, u1Array, 4);
	vUtDelay1ms(200);
	if (!u13DMode)  // 2d_1080p
	{
		u1Array[0] = 0x04;
		u1Array[1] = 0x65;
		u1Array[2] = 0x08;
		u1Array[3] = 0x98;  // 0x04650898 (*)
	}
	else if (u13DMode==E_DGO_3D_1080P_24_FP)
	{
		u1Array[0] = 0x08;
		u1Array[1] = 0xca;
		u1Array[2] = 0x0a;
		u1Array[3] = 0xbe;  // 0x08ca0abe (*)
	}
	else if (u13DMode==E_DGO_3D_1080P_25_FP)
	{
		u1Array[0] = 0x08;
		u1Array[1] = 0xca;
		u1Array[2] = 0x0a;
		u1Array[3] = 0x50;  // 0x08ca0a50 (*)
	}
	else if (u13DMode==E_DGO_3D_1080P_30_FP)
	{
		u1Array[0] = 0x08;
		u1Array[1] = 0xca;
		u1Array[2] = 0x08;
		u1Array[3] = 0x98;  // 0x08ca0898 (*)
	}
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0624, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x05;
	u1Array[2] = 0x00;
	u1Array[3] = 0x2c;  // 0x0005002c (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0628, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0xc1;
	u1Array[2] = 0x08;
	u1Array[3] = 0x40;  // 0x00c10840 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0630, u1Array, 4);
	vUtDelay1ms(200);
	if (!u13DMode)  // 2d_1080p
	{
		u1Array[1] = 0x2a;
		u1Array[2] = 0x04;
		u1Array[3] = 0x61;  // 0x002a0461 (*)
	}
	else  // 3d_1080p_24/25/30_FP
	{
		u1Array[1] = 0x2a;
		u1Array[2] = 0x08;
		u1Array[3] = 0xc6;  // 0x002a08c6 (*)
	}
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0634, u1Array, 4);
	vUtDelay1ms(200);  
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0638, u1Array, 4);  // 0x002a0461 (*); 0x002a08c6 (*)
	vUtDelay1ms(200);
	if (!u13DMode)  // 2d_1080p
	{
		u1Array[0] = 0x00;
		u1Array[1] = 0x0f;
		u1Array[2] = 0x00;
		u1Array[3] = 0x1a;  // 0x000f001a (*)
	}
	else if (u13DMode==E_DGO_3D_1080P_24_FP)
	{
		u1Array[0] = 0x00;
		u1Array[1] = 0x11;
		u1Array[2] = 0x00;
		u1Array[3] = 0x31;  // 0x00110031 (*)
	}
	else if ((u13DMode==E_DGO_3D_1080P_25_FP) || (u13DMode==E_DGO_3D_1080P_30_FP))
	{	
		u1Array[0] = 0x00;
		u1Array[1] = 0x11;
		u1Array[2] = 0x02;
		u1Array[3] = 0x01;  // 0x00110201 (*)
	}
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0620, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x00;
	u1Array[3] = 0x01;  // 0x00000000
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0600, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[2] = 0x00;
	u1Array[3] = 0x02;  // 0x00000002 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x060c, u1Array, 4);
	vUtDelay1ms(200);
	if (!u13DMode)  // 2d_1080p
	{
		u1Array[0] = 0x04;
		u1Array[1] = 0x65;
		u1Array[2] = 0x0a;
		u1Array[3] = 0x50;  // 0x04650a50 (*)
	}
	else // 3d_1080p_24/25/30_FP
	{
		u1Array[0] = 0x08;
		u1Array[1] = 0xca;
		u1Array[2] = 0x0a;
		u1Array[3] = 0x50;  // 0x08ca0a50 (*)
	}
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0700, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x03;
	u1Array[2] = 0x00;
	u1Array[3] = 0x05;  // 0x00030005 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0704, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0x10;
	u1Array[2] = 0x01;
	u1Array[3] = 0x00;  // 0x00100100 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0708, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x04;
	u1Array[1] = 0x38;
	u1Array[2] = 0x07;
	u1Array[3] = 0x80;  // 0x04380780 (*)
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x070c, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[0] = 0x00;
	u1Array[1] = 0x00;
	u1Array[2] = 0x02;
	u1Array[3] = 0x00;  // 0x00000200
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0710, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0xff;
	u1Array[2] = 0x88;
	u1Array[3] = 0x44;  // 0x00ff8844
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0714, u1Array, 4);
	vUtDelay1ms(200);
	u1Array[1] = 0xaa;
	u1Array[2] = 0xff;
	u1Array[3] = 0x55;  // 0x00aaff55
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0718, u1Array, 4);
	vUtDelay1ms(200);
	if (!u13DMode)  // 2d_1080p
	{
		u1Array[1] = 0x0a;
		u1Array[2] = 0x00;
		u1Array[3] = 0x40;  // 0x000a0040 (*)
	}
	else // 3d_1080p_24/25/30_FP
	{
		u1Array[1] = 0x08;
		u1Array[2] = 0x00;
		u1Array[3] = 0x40;  // 0x00080040 (*)
	}
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0604, u1Array, 4);
	vUtDelay1ms(200);
	if (!u13DMode)  // 2d_1080p
	{
		u1Array[1] = 0x02;  // 0x00020040 (*)
	}
	else // 3d_1080p_24/25/30_FP
	{
		u1Array[1] = 0x00;  // 0x00000040 (*)
	}
	SIF_X_Write(DGI_I2C_CH,0x100, DGI_DEV_ADDR, 2, 0x0604, u1Array, 4);
	vUtDelay1ms(200);
}

static INT32 _u4Dgo_Set8193Mode(INT32 i4Argc, const CHAR **szArgv)
{
	UINT8 u1Mode=0;
	
	if (i4Argc != 2)
	{
        Printf("Usage: %s  Mode\n", szArgv[0]);
        Printf("<Mode>\n");
        Printf("1: 480P\n");
        Printf("2: 576P\n");
        printf("3: 720P\n");
        printf("4: 1080P\n");
        Printf("5: 1080P_24_FP\n");
        printf("6: 1080P_25_FP\n");
        printf("7: 1080P_30_FP\n");
        return 1;
	}
	
	u1Mode = (UINT8)StrToInt(szArgv[1]);

    switch(u1Mode)
    {
    	case E_DGO_3D_1080P_24_FP:
    		vDGISet_1080P(E_DGO_3D_1080P_24_FP);
    		break;
    	case E_DGO_3D_1080P_25_FP:
    		vDGISet_1080P(E_DGO_3D_1080P_25_FP);
    		break;
    	case E_DGO_3D_1080P_30_FP:
    		vDGISet_1080P(E_DGO_3D_1080P_30_FP);
    		break;
		case E_DGO_576P:  // 576P
			vDGISet_576P();
			break;
		case E_DGO_720P:  // 720P
			vDGISet_720P();
			break;
		case E_DGO_1080P:  // 1080P
			vDGISet_1080P(0);
			break;
		case E_DGO_480P:  // 480P
			vDGISet_480P();
		default:
			break;
    }
    
    return 0;
}

static INT32 _u4Dgo_Read8193(INT32 i4Argc, const CHAR **szArgv)
{
	UINT32 u4WordAddr;
	UINT32 u4Cnt, i;
	UINT8  u1Array[4];
	
	if (i4Argc != 3)
	{
        Printf("Usage: %s  <reg_addr> <num>\n", szArgv[0]);
        return 1;
	}

	u4WordAddr = StrToInt(szArgv[1]);
	u4Cnt = StrToInt(szArgv[2]);

	for (i=0;i<u4Cnt;i++)
	{	
		SIF_X_Read(DGI_I2C_CH,0x100,DGI_DEV_ADDR,2,u4WordAddr,u1Array,4);
		Printf("0x%x: 0x%x, 0x%x, 0x%x, 0x%x\n", u4WordAddr, u1Array[3], u1Array[2], u1Array[1], u1Array[0]);
		u4WordAddr+=4;
	}
	
	return 0;
}

#endif   // ifdef CC_CLI
