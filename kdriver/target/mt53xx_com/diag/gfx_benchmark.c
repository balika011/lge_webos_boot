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


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_os.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_rand.h"
#include "x_debug.h"
#include "x_mid.h"
#include "x_hal_926.h"
#include "drv_hdtv.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#include "gfx_if.h"
#include "gfx_drvif.h"
#include "osd_if.h"
#include "gfx_emu_image.h"
#include "panel.h"

#include <stdarg.h>
//#include <string.h>

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define ADR_DST         FALSE
#define ADR_SRC         TRUE

#define BUF_SCREEN      (_u4GB_Buf[0])
#define BUF_BACK        (_u4GB_Buf[1])
#define BUF_ADDR1       (_u4GB_Buf[2])
#define BUF_ADDR2       (_u4GB_Buf[3])
#define BUF_ADDR3       (_u4GB_Buf[4])
#define BUF_ADDR4       (_u4GB_Buf[5])
#define BUF_ADDR5       (_u4GB_Buf[6])
#define BUF_ADDR6       (_u4GB_Buf[7])
#define BUF_ADDR7       (_u4GB_Buf[8])
#define BUF_ADDR8       (_u4GB_Buf[9])
#define BUF_TEMP        (_u4GB_Buf[10])

#define STRETCH_DST_BUF_ADDR1       (BUF_ADDR7)
#define STRETCH_DST_BUF_ADDR2       (BUF_ADDR8)


#define GFX_BURST_READ_ALIGN_SIZE 63
#define GFX_BURST_READ_ALIGN(X)  (((X) + GFX_BURST_READ_ALIGN_SIZE) & (~GFX_BURST_READ_ALIGN_SIZE))
#define D_GFX_TEST_AVAILABLE_MODE 5

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

HAL_TIME_T _RTIME_TAG;


static UINT32   _u4GB_Buf[11];

static UINT32   _u4FbmStartAddr = NULL;
static UINT32   _u4CurrentBuffer = NULL;
static INT32   _u4SwapRegion = -1;
static INT32   _u4SwapRList = -1;

static UINT32   _u4FbmTotalSize = 0;


static CHAR *   _szCurColorMode = "argb8888";
static UINT32   _szDstModeIndex = 14;
static UINT32   _u4CurColorModeIndex = 0;
static UINT32   _u4CurColor = 0xffffffff;
static UINT32   _u4GfxPitch = 7680;

static UINT32   _u4CanvasWidth = 1921;
static UINT32   _u4ScreenWidth = 1920;
static UINT32   _u4ScreenHeight = 1080;

 static UINT32	 _u4PanelWidth = 1920;
 static UINT32	 _u4PanelHeight = 1080;

 static UINT32 _au4ScreenWidth[] =
 {
	 720,
	 720,
	 720,
	 720,
	 720,
 };
 
 
 static UINT32 _au4ScreenHeight[] =
 {
	 480,
	 480,
	 480,
	 480,
	 480,
 };
 
 
 static CHAR * _aszColorMode[] =
 {
	 "argb8888",
	 "argb4444",
	 "argb1555",
	 "rgb565",
	 "rgb8",
 };
 
 
 static UINT32 _au4DefPitch[] =
 {
	 2880,
	 1440,
	 1440,
	 1440,
	 720,
 };
 

static INT32 _CLI_CMD(const CHAR* szCmd, ...)
{
    CHAR szBuf[256];
    VA_LIST t_ap;

    VA_START(t_ap, szCmd);
    x_vsprintf(szBuf, szCmd, t_ap);
    VA_END(t_ap);

    return CLI_Parser(szBuf);
}

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
extern void GFX_DifSetMode(UINT32 u4GfxMode);

//static INT32  _GB_ShowScreen(INT32 i4Argc, const CHAR ** szArgv);
static INT32  _GB_Init(INT32 i4Argc, const CHAR ** szArgv);
static INT32  _GB_SetDefMode(UINT32 u4BuffAddr, BOOL fgSrc);
static INT32  _GB_SetColor(INT32 i4Argc, const CHAR ** szArgv);
static void   _GB_CreateBuffers(void);
static INT32  _GB_SetMode(INT32 i4Argc, const CHAR ** szArgv);
static INT32  _GB_BlitTest1(INT32 i4Argc, const CHAR ** szArgv);
static INT32  _GB_StretchBlitTest1(INT32 i4Argc, const CHAR ** szArgv);
static INT32  _GB_MoveBmpTest1(INT32 i4Argc, const CHAR ** szArgv);
static INT32  _GB_MoveBmpTest2(INT32 i4Argc, const CHAR ** szArgv);
static INT32  _GB_MoveBmpTest3(INT32 i4Argc, const CHAR ** szArgv);
static INT32  _GB_GetBackBuffer(UINT32 * u4BufferAddr);
static INT32  _GB_SwapFlipto(UINT32 u4OsdPlane,UINT32 u4BufferAddr);
static INT32  _GB_SetOSDDisplay(INT32 i4Argc, const CHAR ** szArgv);
static INT32  _GB_SetPitch(UINT32 u4Pitch);
static INT32 _GB_MoveBmpTest4(INT32 i4Argc, const CHAR ** szArgv);


static CLI_EXEC_T _arGfxBenchmarkTal[] =
{
	{"init",		  "i",	        _GB_Init, 		    	NULL,	"Init whole system (when boot)", 		CLI_SUPERVISOR},
	{"color",		  "clr",	    _GB_SetColor, 			NULL,	"set gfx color",                 		CLI_SUPERVISOR},
	{"mode",		  "md",	        _GB_SetMode, 			NULL,	"set gfx mode(hw/sw/cm)",        		CLI_SUPERVISOR},
	{"blit1",		  "blt1",	    _GB_BlitTest1, 			NULL,	"gfx bit blit performence test", 		CLI_SUPERVISOR},
	{"osddisplay",	  "osd",	    _GB_SetOSDDisplay, 		NULL,	"set osd display arguments",     		CLI_SUPERVISOR},
	{"stretchblt1",	  "stblt1",	    _GB_StretchBlitTest1, 	NULL,	"gfx stretch blit performence test",    CLI_SUPERVISOR},
	{"movebmp1",	  "mvb1",	    _GB_MoveBmpTest1, 		NULL,	"gfx moving bmp performence test",	    CLI_SUPERVISOR},
	{"movebmp2",	  "mvb2",	    _GB_MoveBmpTest2, 		NULL,	"gfx moving bmp performence test 2",	CLI_SUPERVISOR},
	{"movebmp3",	  "mvb3",		_GB_MoveBmpTest3,		NULL,	"gfx moving bmp performence test 3",	CLI_SUPERVISOR},
	{"movebmp4",	  "mvb4",		_GB_MoveBmpTest4,		NULL,	"gfx moving bmp performence test 3",	CLI_SUPERVISOR},
#ifndef CC_5391_LOADER
    //CLIMOD_DEBUG_CLIENTRY(Gfxbmk),
#endif	
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

CLI_MAIN_COMMAND_ITEM(Gfxbmk)
{
    "gfxbmk",
	NULL,
	NULL,
	_arGfxBenchmarkTal,
    "gfx benchmark test",
    CLI_GUEST
};

/*********************************************************
*********************************************************/
static INT32 _GB_SetPitch(UINT32 u4Pitch)
{
	_u4GfxPitch= u4Pitch;

	//printf("set pitch (%d)\n",u4Pitch);
	return 0;
}

/*********************************************************
*********************************************************/
static INT32 _GB_SetOSDDisplay(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4OsdPlane ,u4SrcWidth ,u4SrcHeight ,u4Region ,u4Scaler ,u4SrcEnable;
	if (i4Argc < 3)
    {
        printf("Error input\n");
		printf("arg: {plane} {srcW} {srcH} \n");
        return 0;
    }

	u4OsdPlane = (UINT32) StrToInt(szArgv[1]);
	u4SrcWidth = (UINT32) StrToInt(szArgv[2]);
	u4SrcHeight = (UINT32) StrToInt(szArgv[3]);
	u4SrcEnable = (UINT32) StrToInt(szArgv[4]);

	u4Region = OSD_PLA_GetFirstRegion(u4OsdPlane);
	OSD_RGN_Set(u4Region,OSD_RGN_BMP_W,u4SrcWidth);
	OSD_RGN_Set(u4Region,OSD_RGN_BMP_H,u4SrcHeight);
	OSD_RGN_Set(u4Region,OSD_RGN_DISP_W,u4SrcWidth);
	OSD_RGN_Set(u4Region,OSD_RGN_DISP_H,u4SrcHeight);
	OSD_RGN_Set(u4Region,OSD_RGN_BMP_PITCH,_u4GfxPitch);

	OSD_RGN_Set(_u4SwapRegion,OSD_RGN_BMP_W,u4SrcWidth);
	OSD_RGN_Set(_u4SwapRegion,OSD_RGN_BMP_H,u4SrcHeight);
	OSD_RGN_Set(_u4SwapRegion,OSD_RGN_DISP_W,u4SrcWidth);
	OSD_RGN_Set(_u4SwapRegion,OSD_RGN_DISP_H,u4SrcHeight);
	OSD_RGN_Set(_u4SwapRegion,OSD_RGN_BMP_PITCH,_u4GfxPitch);
	

	u4Scaler = OSD_PLANE_TO_SCALER(u4OsdPlane);
	OSD_SC_Scale(u4Scaler,u4SrcEnable,u4SrcWidth,u4SrcHeight,_u4PanelWidth,_u4PanelHeight);

	return 0;
}

/*********************************************************
*********************************************************/
static INT32 _GB_SwapFlipto(UINT32 u4OsdPlane,UINT32 u4BufferAddr)
{
	INT32 u4Region = -1;

	u4Region = (INT32)OSD_PLA_GetFirstRegion(u4OsdPlane);
	OSD_RGN_Set((UINT32)u4Region,OSD_RGN_BMP_ADDR,u4BufferAddr);	

	_u4CurrentBuffer = u4BufferAddr; 
	return 0;
}

/*********************************************************
*********************************************************/
static INT32 _GB_GetBackBuffer(UINT32 * u4BufferAddr)
{
	if(_u4CurrentBuffer == BUF_SCREEN)
	{
		*u4BufferAddr = BUF_BACK;
	}
	else
	{
		*u4BufferAddr = BUF_SCREEN;
	}
	return 0;
}
/*********************************************************
*********************************************************/
static INT32 _GB_MoveBmpTest4(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4VideoShift=0,u4PanelW=0,  u4VideoGap=0 ,u4Tem=0;
	UINT32 u4VedioW =0, u4Delay =0;
	u4PanelW = PANEL_GetPanelWidth();

	u4VideoGap = 10000/u4PanelW ;
	
	if (i4Argc < 2)
    {
        printf("Error input: {u4VedioW}  {u4Delay}\n");
        return 0;
    }

	u4VedioW = (UINT32) StrToInt(szArgv[1]);

	if (i4Argc == 3)
    {
        u4Delay = (UINT32) StrToInt(szArgv[2]);
    }

	_CLI_CMD("mpv.d_l 0");
	_CLI_CMD("nptv.scl.d_l 0");
	_CLI_CMD("nptv.d_l 0");
	_CLI_CMD("nptv.di.d_l 0");
	_CLI_CMD("nptv.vrm.d_l 0");
	_CLI_CMD("fbm.d_l 0");
	_CLI_CMD("vdp.s.outr 0 0 %d 10000",(10000/u4VedioW));

	for(u4Tem=0;u4Tem<(u4PanelW - u4PanelW/u4VedioW);u4Tem++)
	{
		_CLI_CMD("vdp.s.outr 0 %d 0 %d 10000",u4VideoShift,(10000/u4VedioW));
		u4VideoShift = u4VideoShift + u4VideoGap;

		x_thread_delay(u4Delay);
	}
	return 0;
}

static INT32 _GB_MoveBmpTest3(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4BmpWidth =0, u4BmpHeight =0, u4BkWidth =0, u4BkHeight =0,u4StretchWidth =0, u4StretchHeight =0;
	UINT32 u4FrameBuffer =NULL;
	UINT8 * u4BackgroundBuffer = NULL;
	UINT32 u4Shift =0 ;
	UINT32  u4LdstX[9], u4LdstY[9], u4RdstX[9], u4RdstY[9];
	UINT32 u4VideoShift = 0 , u4VideoGap = 0 , u4VideoTag = 0;

	INT32  i4TagMove =0 , i =0;
	UINT32 u4Ar = 0x80,u4Mode = 0x7;
	UINT32 u4ScalerEn = 1;
	UINT32 u4MoveOff = 5;

	UINT8 * u4BmpRBuffer[9];
	UINT8 * u4BmpLBuffer[9];

	UINT32 u4OsdPlane =0 ;
    HAL_TIME_T rStart;
    HAL_TIME_T rStop;
    HAL_TIME_T rDelta;    
	
	u4OsdPlane = 1;
	UNUSED(u4BkWidth);
	UNUSED(u4BkHeight);
	UNUSED(u4BackgroundBuffer);


	u4BmpRBuffer[0] = IMAGE_512X120_BMP_R0;
	u4BmpRBuffer[1] = IMAGE_512X120_BMP_R1;
	u4BmpRBuffer[2] = IMAGE_512X120_BMP_R2;
	u4BmpRBuffer[3] = IMAGE_512X120_BMP_R3;
	u4BmpRBuffer[4] = IMAGE_512X120_BMP_R4;
	u4BmpRBuffer[5] = IMAGE_512X120_BMP_R5;
	u4BmpRBuffer[6] = IMAGE_512X120_BMP_R6;
	u4BmpRBuffer[7] = IMAGE_512X120_BMP_R7;
	u4BmpRBuffer[8] = IMAGE_512X120_BMP_R8;

	u4BmpLBuffer[0] = IMAGE_512X120_BMP_L8;
	u4BmpLBuffer[1] = IMAGE_512X120_BMP_L7;
	u4BmpLBuffer[2] = IMAGE_512X120_BMP_L6;
	u4BmpLBuffer[3] = IMAGE_512X120_BMP_L5;
	u4BmpLBuffer[4] = IMAGE_512X120_BMP_L4;
	u4BmpLBuffer[5] = IMAGE_512X120_BMP_L3;
	u4BmpLBuffer[6] = IMAGE_512X120_BMP_L2;
	u4BmpLBuffer[7] = IMAGE_512X120_BMP_L1;
	u4BmpLBuffer[8] = IMAGE_512X120_BMP_L0;

	u4BackgroundBuffer = IMAGE_960X1080_TESTBMP_1;

	u4BmpWidth 	=512;
	u4BmpHeight =120;
	u4BkWidth 	=960;
	u4BkHeight 	=1080;
	u4StretchWidth 	=1920;
	u4StretchHeight 	=1080;
	u4VideoGap = 10000/1920 ;

	UNUSED(u4BmpWidth);
	UNUSED(u4BmpHeight);

	if (i4Argc == 2)
    {
        u4ScalerEn = (UINT32) StrToInt(szArgv[1]);
    }	
	if (i4Argc == 3)
    {
        u4MoveOff = (UINT32) StrToInt(szArgv[2]);
    }
	

	_GB_SetPitch(u4StretchWidth * 4);
	
	
	_CLI_CMD("nptv.scl.d_l 0");
	_CLI_CMD("nptv.d_l 0");
	_CLI_CMD("nptv.di.d_l 0");
	_CLI_CMD("nptv.vrm.d_l 0");
	_CLI_CMD("fbm.d_l 0");
	_CLI_CMD("vdp.s.srcr 0 0 0 10000 10000");
	_CLI_CMD("vdp.s.outr 1 0 0 5000 10000");

	
	_GB_SetPitch(u4StretchWidth * 4);

	_CLI_CMD("gfxbmk.osd %d %d %d %d",u4OsdPlane , u4StretchWidth ,u4StretchHeight ,u4ScalerEn);

	_GB_GetBackBuffer(&u4FrameBuffer);


	x_memset((UINT32*)u4FrameBuffer,0,u4StretchWidth * u4StretchHeight*4);

	_CLI_CMD("gfxbmk.mode cm argb8888");

	printf( "\n>>Start 2D GPU Moving bmp test\n");

	HAL_GetTime(&rStart);

	for(u4Shift = 0;u4Shift < (u4StretchWidth - u4BmpWidth + (u4MoveOff*8));u4Shift = u4Shift + 2)
	{
		i4TagMove = (INT32)u4Shift;

		if(u4Shift >= ((u4StretchWidth - u4BmpWidth)/2))
		{
			if(u4VideoShift < 5000)
			{
				_CLI_CMD("vdp.s.outr 1 %d 0 5000 10000",u4VideoShift);
				u4VideoShift = u4VideoShift + u4VideoGap*2;
			}
			else if( !(u4VideoTag) )
			{
				_CLI_CMD("vdp.s.outr 1 5000 0 5000 10000");
				u4VideoTag = 1;
			}
		}		
		_GB_GetBackBuffer(&u4FrameBuffer);
		_GB_SetPitch(u4StretchWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_SRC);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_CLI_CMD("gfxbmk.color 0x0");
		_CLI_CMD("gfx.op.rect 0 0 %d %d",u4StretchWidth ,u4StretchHeight);

/*
		if(u4Shift >= (u4StretchWidth/2))
		{
			
			_CLI_CMD("vdp.s.outr 0 %d 0 5000 10000",u4VideoShift);
			u4VideoShift = u4VideoShift + u4VideoGap*2;
		}
		
		_GB_GetBackBuffer(&u4FrameBuffer);
		//stretch blit rigbackground  bmp   (note src and dst pitch)
		_GB_SetPitch(u4StretchWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_GB_SetPitch(u4BkWidth * 4);
		_GB_SetDefMode((UINT32)u4BackgroundBuffer, ADR_SRC);
*/

/*		
		_GFX_StretchAlphaCom(
				0, 0,u4BkWidth, u4BkHeight,
				0, 0,u4StretchWidth,u4StretchHeight,0x80,0x7
			);
		_CLI_CMD("gfx.flush");

		GFX_StretchBlt(
				0, 0,u4BkWidth, u4BkHeight,
				0, 0,u4StretchWidth,u4StretchHeight
			);
		_CLI_CMD("gfx.flush");*/
		for( i =0; i < 9 ;i++ )
		{
			if( (i4TagMove > (INT32)(u4StretchWidth - u4BmpWidth)) )
			{
				u4RdstX[8-i] = 0;
				u4LdstX[i] = u4StretchWidth - u4BmpWidth;
				
			}
			else if( i4TagMove < (INT32)0)
			{
				u4RdstX[8-i] = u4StretchWidth - u4BmpWidth;
				u4LdstX[i] = 0;
			}
			else
			{
				u4RdstX[8-i] = u4StretchWidth - u4BmpWidth -i4TagMove;
				u4LdstX[i] = i4TagMove;
			}
			
			u4RdstY[8-i] = u4StretchHeight - u4BmpHeight*(i+1);
			u4LdstY[i] = u4BmpHeight*i;

			i4TagMove = i4TagMove - u4MoveOff;
		}	

		for(i =0; i < 9 ;i++ )
		{
			//move left bmp
			_GB_SetPitch(u4StretchWidth * 4);
			_GB_SetDefMode(u4FrameBuffer, ADR_DST);
			_GB_SetPitch(u4BmpWidth * 4);
			_GB_SetDefMode((UINT32)u4BmpLBuffer[i], ADR_SRC);

			_CLI_CMD("gfx.op.composeloop 0 0 %d %d %d %d %d %d %d",u4LdstX[i] ,u4LdstY[i] ,u4BmpWidth ,u4BmpHeight,u4Ar,u4Mode,0x0);

			_CLI_CMD("gfx.flush");
			//move right bmp
			_GB_SetPitch(u4StretchWidth * 4);
			_GB_SetDefMode(u4FrameBuffer, ADR_DST);
			_GB_SetPitch(u4BmpWidth * 4);
			_GB_SetDefMode((UINT32)u4BmpRBuffer[i], ADR_SRC);

			_CLI_CMD("gfx.op.composeloop 0 0 %d %d %d %d %d %d %d",u4RdstX[i] ,u4RdstY[i] ,u4BmpWidth ,u4BmpHeight,u4Ar,u4Mode,0x0);

			_CLI_CMD("gfx.flush");
		}	
		//display
		//OSD_PLA_SetFading(u4OsdPlane,0xa0);

		_GB_SwapFlipto(u4OsdPlane , u4FrameBuffer);
			
	}

    HAL_GetTime(&rStop);
    HAL_GetDeltaTime(&rDelta, &rStart, &rStop);
   	printf( ">>Start(%u.%6u) \n", rStart.u4Seconds, rStart.u4Micros);
	printf( ">>Stop(%u.%6u) \n", rStop.u4Seconds, rStop.u4Micros);
    printf( ">>GetRefTime %u.%6u\n", rDelta.u4Seconds, rDelta.u4Micros);
	printf( ">>End of 2D GPU Moving bmp test\n\n");
	
	return 0;
}


/*********************************************************
*********************************************************/
static INT32 _GB_MoveBmpTest2(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4BmpWidth =0, u4BmpHeight =0, u4BkWidth =0, u4BkHeight =0,u4StretchWidth =0, u4StretchHeight =0;
	UINT32 u4FrameBuffer =NULL;
	UINT8 * u4BackgroundBuffer = NULL;
	UINT32 u4Shift =0 ;
	UINT32  u4LdstX[9], u4LdstY[9], u4RdstX[9], u4RdstY[9];

	INT32  i4TagMove =0 , i =0;
	UINT32 u4Ar = 0x80,u4Mode = 0x7;
	UINT32 u4ScalerEn = 1;
	UINT32 u4MoveOff = 5;

	UINT8 * u4BmpRBuffer[9];
	UINT8 * u4BmpLBuffer[9];

	UINT32 u4OsdPlane =0;
    HAL_TIME_T rStart;
    HAL_TIME_T rStop;
    HAL_TIME_T rDelta;    
	
	u4OsdPlane = 1;
	UNUSED(u4BkWidth);
	UNUSED(u4BkHeight);
	UNUSED(u4BackgroundBuffer);

	u4BmpRBuffer[0] = IMAGE_512X120_BMP_R0;
	u4BmpRBuffer[1] = IMAGE_512X120_BMP_R1;
	u4BmpRBuffer[2] = IMAGE_512X120_BMP_R2;
	u4BmpRBuffer[3] = IMAGE_512X120_BMP_R3;
	u4BmpRBuffer[4] = IMAGE_512X120_BMP_R4;
	u4BmpRBuffer[5] = IMAGE_512X120_BMP_R5;
	u4BmpRBuffer[6] = IMAGE_512X120_BMP_R6;
	u4BmpRBuffer[7] = IMAGE_512X120_BMP_R7;
	u4BmpRBuffer[8] = IMAGE_512X120_BMP_R8;

	u4BmpLBuffer[0] = IMAGE_512X120_BMP_L8;
	u4BmpLBuffer[1] = IMAGE_512X120_BMP_L7;
	u4BmpLBuffer[2] = IMAGE_512X120_BMP_L6;
	u4BmpLBuffer[3] = IMAGE_512X120_BMP_L5;
	u4BmpLBuffer[4] = IMAGE_512X120_BMP_L4;
	u4BmpLBuffer[5] = IMAGE_512X120_BMP_L3;
	u4BmpLBuffer[6] = IMAGE_512X120_BMP_L2;
	u4BmpLBuffer[7] = IMAGE_512X120_BMP_L1;
	u4BmpLBuffer[8] = IMAGE_512X120_BMP_L0;

	u4BackgroundBuffer = IMAGE_960X1080_TESTBMP_1;

	u4BmpWidth 	=512;
	u4BmpHeight =120;
	u4BkWidth 	=960;
	u4BkHeight 	=1080;
	u4StretchWidth 	=1920;
	u4StretchHeight 	=1080;

	UNUSED(u4BmpWidth);
	UNUSED(u4BmpHeight);

	_GB_SetPitch(u4StretchWidth * 4);

	
	if (i4Argc == 2)
    {
        u4ScalerEn = (UINT32) StrToInt(szArgv[1]);
    }	
	if (i4Argc == 3)
    {
        u4MoveOff = (UINT32) StrToInt(szArgv[2]);
    }

	
	_GB_SetPitch(u4StretchWidth * 4);

	_CLI_CMD("gfxbmk.osd %d %d %d %d",u4OsdPlane , u4StretchWidth ,u4StretchHeight ,u4ScalerEn);

	_GB_GetBackBuffer(&u4FrameBuffer);


	x_memset((UINT32*)u4FrameBuffer,0,u4StretchWidth * u4StretchHeight*4);

	_CLI_CMD("gfxbmk.mode cm argb8888");

	printf( "\n>>Start 2D GPU Moving bmp test\n");

	HAL_GetTime(&rStart);

	for(u4Shift = 0;u4Shift < (u4StretchWidth - u4BmpWidth + (u4MoveOff*8));u4Shift = u4Shift + 2)
	{
		i4TagMove = (INT32)u4Shift;
		_GB_GetBackBuffer(&u4FrameBuffer);
		_GB_SetPitch(u4StretchWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_SRC);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_CLI_CMD("gfxbmk.color 0x0");
		_CLI_CMD("gfx.op.rect 0 0 %d %d",u4StretchWidth ,u4StretchHeight);
		/*
		_GB_GetBackBuffer(&u4FrameBuffer);
		//stretch blit rigbackground  bmp   (note src and dst pitch)
		_GB_SetPitch(u4StretchWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_GB_SetPitch(u4BkWidth * 4);
		_GB_SetDefMode((UINT32)u4BackgroundBuffer, ADR_SRC);

		GFX_StretchBlt(
				0, 0,u4BkWidth, u4BkHeight,
				0, 0,u4StretchWidth,u4StretchHeight
			);
		_CLI_CMD("gfx.flush");*/
		for( i =0; i < 9 ;i++ )
		{
			if( (i4TagMove > (INT32)(u4StretchWidth - u4BmpWidth)) )
			{
				u4RdstX[8-i] = 0;
				u4LdstX[i] = u4StretchWidth - u4BmpWidth;
				
			}
			else if( i4TagMove < (INT32)0)
			{
				u4RdstX[8-i] = u4StretchWidth - u4BmpWidth;
				u4LdstX[i] = 0;
			}
			else
			{
				u4RdstX[8-i] = u4StretchWidth - u4BmpWidth -i4TagMove;
				u4LdstX[i] = i4TagMove;
			}
			
			u4RdstY[8-i] = u4StretchHeight - u4BmpHeight*(i+1);
			u4LdstY[i] = u4BmpHeight*i;

			i4TagMove = i4TagMove - u4MoveOff;
		}	

		for(i =0; i < 9 ;i++ )
		{
			//move left bmp
			_GB_SetPitch(u4StretchWidth * 4);
			_GB_SetDefMode(u4FrameBuffer, ADR_DST);
			_GB_SetPitch(u4BmpWidth * 4);
			_GB_SetDefMode((UINT32)u4BmpLBuffer[i], ADR_SRC);

			_CLI_CMD("gfx.op.composeloop 0 0 %d %d %d %d %d %d %d",u4LdstX[i] ,u4LdstY[i] ,u4BmpWidth ,u4BmpHeight,u4Ar,u4Mode,0x0);
			_CLI_CMD("gfx.flush");
		
			//move right bmp
			_GB_SetPitch(u4StretchWidth * 4);
			_GB_SetDefMode(u4FrameBuffer, ADR_DST);
			_GB_SetPitch(u4BmpWidth * 4);
			_GB_SetDefMode((UINT32)u4BmpRBuffer[i], ADR_SRC);

			_CLI_CMD("gfx.op.composeloop 0 0 %d %d %d %d %d %d %d",u4RdstX[i] ,u4RdstY[i] ,u4BmpWidth ,u4BmpHeight,u4Ar,u4Mode,0x0);
			_CLI_CMD("gfx.flush");
			
		}	
		//display
		_GB_SwapFlipto(u4OsdPlane , u4FrameBuffer);
			
	}

    HAL_GetTime(&rStop);
    HAL_GetDeltaTime(&rDelta, &rStart, &rStop);
   	printf( ">>Start(%u.%6u) \n", rStart.u4Seconds, rStart.u4Micros);
	printf( ">>Stop(%u.%6u) \n", rStop.u4Seconds, rStop.u4Micros);
    printf( ">>GetRefTime %u.%6u\n", rDelta.u4Seconds, rDelta.u4Micros);
	printf( ">>End of 2D GPU Moving bmp test\n\n");
	
	return 0;
}



/*********************************************************
*********************************************************/
static INT32 _GB_MoveBmpTest1(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4BmpWidth =0, u4BmpHeight =0, u4BkWidth =0, u4BkHeight =0,u4StretchWidth =0, u4StretchHeight =0;
	UINT32 u4FrameBuffer =NULL;
	UINT8 *u4BackgroundBuffer = NULL;
	UINT32 u4Shift =0 ;
	UINT32  u4LdstX[9], u4LdstY[9], u4RdstX[9], u4RdstY[9];
	INT32  i4TagMove = 0, i = 0;
	UINT32 u4ScalerEn = 1;
	UINT32 u4MoveOff = 5 ;

	UINT8 * u4BmpRBuffer[9];
	UINT8 * u4BmpLBuffer[9];

	UINT32 u4Pitch =0;
	UINT32 u4OsdPlane =0;
    HAL_TIME_T rStart;
    HAL_TIME_T rStop;
    HAL_TIME_T rDelta;    

	UNUSED(u4LdstX);
	UNUSED(u4LdstY);
	UNUSED(u4RdstX);
	UNUSED(u4RdstY);
	UNUSED(u4BmpRBuffer);
	UNUSED(u4BmpLBuffer);
	UNUSED(u4Pitch);
	UNUSED(u4BmpWidth);
	UNUSED(u4BmpHeight);
	
	
	u4OsdPlane = 1;
	UNUSED(u4BkWidth);
	UNUSED(u4BkHeight);
	UNUSED(u4BackgroundBuffer);

	u4BmpRBuffer[0] = IMAGE_512X120_BMP_R0;
	u4BmpRBuffer[1] = IMAGE_512X120_BMP_R1;
	u4BmpRBuffer[2] = IMAGE_512X120_BMP_R2;
	u4BmpRBuffer[3] = IMAGE_512X120_BMP_R3;
	u4BmpRBuffer[4] = IMAGE_512X120_BMP_R4;
	u4BmpRBuffer[5] = IMAGE_512X120_BMP_R5;
	u4BmpRBuffer[6] = IMAGE_512X120_BMP_R6;
	u4BmpRBuffer[7] = IMAGE_512X120_BMP_R7;
	u4BmpRBuffer[8] = IMAGE_512X120_BMP_R8;

	u4BmpLBuffer[0] = IMAGE_512X120_BMP_L8;
	u4BmpLBuffer[1] = IMAGE_512X120_BMP_L7;
	u4BmpLBuffer[2] = IMAGE_512X120_BMP_L6;
	u4BmpLBuffer[3] = IMAGE_512X120_BMP_L5;
	u4BmpLBuffer[4] = IMAGE_512X120_BMP_L4;
	u4BmpLBuffer[5] = IMAGE_512X120_BMP_L3;
	u4BmpLBuffer[6] = IMAGE_512X120_BMP_L2;
	u4BmpLBuffer[7] = IMAGE_512X120_BMP_L1;
	u4BmpLBuffer[8] = IMAGE_512X120_BMP_L0;

	u4BackgroundBuffer = IMAGE_960X1080_TESTBMP_1;

	u4BmpWidth 	=512;
	u4BmpHeight =120;
	u4BkWidth 	=960;
	u4BkHeight 	=1080;
	u4StretchWidth 	=1920;
	u4StretchHeight 	=1080;

	_GB_SetPitch(u4StretchWidth * 4);

	if (i4Argc == 2)
    {
        u4ScalerEn = (UINT32) StrToInt(szArgv[1]);
    }
	if (i4Argc == 3)
    {
        u4MoveOff = (UINT32) StrToInt(szArgv[2]);
    }
	_GB_SetPitch(u4StretchWidth * 4);

	_CLI_CMD("gfxbmk.osd %d %d %d %d",u4OsdPlane , u4StretchWidth ,u4StretchHeight ,u4ScalerEn);

	_GB_GetBackBuffer(&u4FrameBuffer);
	
	x_memset((UINT32*)u4FrameBuffer,0,u4StretchWidth * u4StretchHeight*4);

	_CLI_CMD("gfxbmk.mode cm argb8888");

	printf( "\n>>Start 2D GPU Moving bmp test\n");

	HAL_GetTime(&rStart);

	for(u4Shift = 0;u4Shift < (u4StretchWidth - u4BmpWidth + (u4MoveOff*8));u4Shift = u4Shift + 2)
	{
		i4TagMove = (INT32)u4Shift;
		_GB_GetBackBuffer(&u4FrameBuffer);
		_GB_SetPitch(u4StretchWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_SRC);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_CLI_CMD("gfxbmk.color 0x0");
		_CLI_CMD("gfx.op.rect 0 0 %d %d",u4StretchWidth ,u4StretchHeight);
		
		/*
		
		_GB_GetBackBuffer(&u4FrameBuffer);

		//stretch blit rigbackground  bmp   (note src and dst pitch)
		_GB_SetPitch(u4StretchWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_GB_SetPitch(u4BkWidth * 4);
		_GB_SetDefMode((UINT32)u4BackgroundBuffer, ADR_SRC);

		GFX_StretchBlt(
				0, 0,u4BkWidth, u4BkHeight,
				0, 0,u4StretchWidth,u4StretchHeight
			);
		_CLI_CMD("gfx.flush");*/

		for( i =0; i < 9 ;i++ )
		{
			if( (i4TagMove > (INT32)(u4StretchWidth - u4BmpWidth)) )
			{
				u4RdstX[8-i] = 0;
				u4LdstX[i] = u4StretchWidth - u4BmpWidth;
				
			}
			else if( i4TagMove < (INT32)0)
			{
				u4RdstX[8-i] = u4StretchWidth - u4BmpWidth;
				u4LdstX[i] = 0;
			}
			else
			{
				u4RdstX[8-i] = u4StretchWidth - u4BmpWidth -i4TagMove;
				u4LdstX[i] = i4TagMove;
			}
			
			u4RdstY[8-i] = u4StretchHeight - u4BmpHeight*(i+1);
			u4LdstY[i] = u4BmpHeight*i;

			i4TagMove = i4TagMove - u4MoveOff;
		}	
		for( i =0; i < 9 ;i++ )
		{
			//move left bmp
			_GB_SetPitch(u4StretchWidth * 4);
			_GB_SetDefMode(u4FrameBuffer, ADR_DST);
			_GB_SetPitch(u4BmpWidth * 4);
			_GB_SetDefMode((UINT32)u4BmpLBuffer[i], ADR_SRC);
			_CLI_CMD("gfx.set.bltopt 0 0 0");
			_CLI_CMD("gfx.op.bitblt 0 0 %d %d %d %d ",u4LdstX[i],u4LdstY[i],u4BmpWidth,u4BmpHeight);
			_CLI_CMD("gfx.flush");
			
			//move right bmp
			_GB_SetPitch(u4StretchWidth * 4);
			_GB_SetDefMode(u4FrameBuffer, ADR_DST);
			_GB_SetPitch(u4BmpWidth * 4);
			_GB_SetDefMode((UINT32)u4BmpRBuffer[i], ADR_SRC);
			_CLI_CMD("gfx.set.bltopt 0 0 0");
			_CLI_CMD("gfx.op.bitblt 0 0 %d %d %d %d ",u4RdstX[i],u4RdstY[i],u4BmpWidth,u4BmpHeight);
			_CLI_CMD("gfx.flush");
		}
		//display
		_GB_SwapFlipto(u4OsdPlane , u4FrameBuffer);
			
	}

    HAL_GetTime(&rStop);
    HAL_GetDeltaTime(&rDelta, &rStart, &rStop);
   	printf( ">>Start(%u.%6u) \n", rStart.u4Seconds, rStart.u4Micros);
	printf( ">>Stop(%u.%6u) \n", rStop.u4Seconds, rStop.u4Micros);
    printf( ">>GetRefTime %u.%6u\n", rDelta.u4Seconds, rDelta.u4Micros);
	printf( ">>End of 2D GPU Moving bmp test\n\n");
	
	return 0;
}


/*********************************************************
*********************************************************/
static INT32 _GB_StretchBlitTest1(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Case =0 ,u4Shift =0 ,u4Lstart_x =0 ,u4Lstart_y =0 ,u4Rstart_x =0 ,u4Rstart_y =0;
	UINT32 u4BmpWidth =0, u4BmpHeight =0, u4RectWidth =0, u4RectHeight =0 ,u4StretchWidth =0, u4StretchHeight =0;
	UINT32 u4BmpRBuffer =NULL, u4BmpLBuffer =NULL , u4FrameBuffer =NULL;
	
	UINT32 u4OsdPlane =0 , u4ScalerEn =0;
    HAL_TIME_T rStart;
    HAL_TIME_T rStop;
    HAL_TIME_T rDelta;    
	
	u4OsdPlane = 1;
	
	if (i4Argc < 2)
    {
        printf("Error input\n");
        return 0;
    }
	if (i4Argc == 3)
    {
        u4ScalerEn = (UINT32) StrToInt(szArgv[2]);
    }

	u4Case = (UINT32) StrToInt(szArgv[1]);		
	switch(u4Case)
	{
		case 1:
			u4BmpWidth =480;
			u4BmpHeight =540;
			u4RectWidth =960;
			u4RectHeight =540;
			u4BmpRBuffer =(UINT32)IMAGE_480X530_TESTBMP_1;
			u4BmpLBuffer =(UINT32)IMAGE_480X530_TESTBMP_2;
			break;
		case 2:
			u4BmpWidth =640;
			u4BmpHeight =720;
			u4RectWidth =1280;
			u4RectHeight =720;
			u4BmpRBuffer =(UINT32)IMAGE_640X720_TESTBMP_1;
			u4BmpLBuffer =(UINT32)IMAGE_640X720_TESTBMP_2;
			break;
		case 3:
			u4BmpWidth =960;
			u4BmpHeight =1080;
			u4RectWidth =1920;
			u4RectHeight =1080;
			u4BmpRBuffer =(UINT32)IMAGE_960X1080_TESTBMP_1;
			u4BmpLBuffer =(UINT32)IMAGE_960X1080_TESTBMP_2;
			break;
		default:
			printf("error test case,not support\n");
			break;
	} 

	_GB_SetPitch(u4RectWidth * 4);

	_GB_SetPitch(u4RectWidth * 4);

	_CLI_CMD("gfxbmk.osd %d %d %d %d",u4OsdPlane , u4RectWidth ,u4RectHeight ,u4ScalerEn);

	_GB_GetBackBuffer(&u4FrameBuffer);

	u4StretchWidth =u4BmpWidth;
	u4StretchHeight =u4BmpHeight;

	printf("gfx banchmark stretch blit test (case %d)",u4Case);
	x_memset((UINT32*)u4FrameBuffer,0,u4RectWidth * u4RectHeight*4);
	x_memset((UINT32*)BUF_ADDR1,0,u4BmpWidth * u4BmpHeight*4);
	x_memset((UINT32*)BUF_ADDR2,0,u4BmpWidth * u4BmpHeight*4);

	_CLI_CMD("gfxbmk.mode cm argb8888");

	//copy right bmp 
	_GB_SetPitch(u4BmpWidth * 4);
	_GB_SetDefMode(STRETCH_DST_BUF_ADDR1, ADR_DST);
	_GB_SetDefMode(u4BmpLBuffer, ADR_SRC);    
	_CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d ",u4BmpWidth,u4BmpHeight);
	_CLI_CMD("gfx.flush");

	//copy left bmp 
	_GB_SetPitch(u4BmpWidth * 4);
	_GB_SetDefMode(STRETCH_DST_BUF_ADDR2, ADR_DST);
	_GB_SetDefMode(u4BmpRBuffer, ADR_SRC);    
	_CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d ",u4BmpWidth,u4BmpHeight);
	_CLI_CMD("gfx.flush");


	printf( "\n>>Start 2D GPU Bitblit test\n");

	HAL_GetTime(&rStart);

	for(u4Shift = 0;u4Shift <= u4RectWidth/2;u4Shift = u4Shift + 2)
	{
		u4Lstart_x =u4Shift ;
		u4Lstart_y =0 ;
		u4Rstart_x = u4RectWidth/2 - u4Shift;
		u4Rstart_y =0;
		
		//fill rect
		_GB_GetBackBuffer(&u4FrameBuffer);
		_GB_SetPitch(u4RectWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_SRC);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_CLI_CMD("gfxbmk.color 0x7f30ff80");
		_CLI_CMD("gfx.op.rect 0 0 %d %d",u4RectWidth ,u4RectHeight);

		//blit right bmp   (note src and dst pitch)
		_GB_SetPitch(u4RectWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_GB_SetPitch(u4BmpWidth * 4);
		_GB_SetDefMode(STRETCH_DST_BUF_ADDR1, ADR_SRC);

		_GFX_StretchAlphaCom(
				0, 0,u4StretchWidth, u4StretchHeight,
				u4Rstart_x,u4Rstart_y,u4BmpWidth,u4BmpHeight,0x80,0x7
			);
		_CLI_CMD("gfx.flush");

		//blit left bmp   (note src and dst pitch)
		_GB_SetPitch(u4RectWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_GB_SetPitch(u4BmpWidth * 4);
		_GB_SetDefMode(STRETCH_DST_BUF_ADDR2, ADR_SRC);

		_GFX_StretchAlphaCom(
				0, 0,u4StretchWidth, u4StretchHeight,
				u4Lstart_x,u4Lstart_y,u4BmpWidth,u4BmpHeight,0x80,0x7
			);
		_CLI_CMD("gfx.flush");
		
		//display
		_GB_SwapFlipto(u4OsdPlane , u4FrameBuffer);

		u4StretchWidth --;
		u4StretchHeight --;
			
	}

    HAL_GetTime(&rStop);
    HAL_GetDeltaTime(&rDelta, &rStart, &rStop);
   	printf( ">>Start(%u.%6u) \n", rStart.u4Seconds, rStart.u4Micros);
	printf( ">>Stop(%u.%6u) \n", rStop.u4Seconds, rStop.u4Micros);
    printf( ">>GetRefTime %u.%6u\n", rDelta.u4Seconds, rDelta.u4Micros);
	printf( ">>End of 2D GPU Stretch Bitblit test\n\n");
	
	return 0;
}


/*********************************************************
*********************************************************/
static INT32 _GB_BlitTest1(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Case =0 ,u4Shift =0 ,u4Lstart_x =0 ,u4Lstart_y =0 ,u4Rstart_x =0 ,u4Rstart_y =0;
	UINT32 u4BmpWidth =0, u4BmpHeight =0, u4RectWidth =0, u4RectHeight =0;
	UINT32 u4BmpRBuffer =NULL, u4BmpLBuffer =NULL , u4FrameBuffer =NULL;
	UINT32 u4ScalerEn = 0;

	UINT32 u4Ar = 0x80 , u4Mode = 0x7;
	UINT32 u4OsdPlane =0;
    HAL_TIME_T rStart;
    HAL_TIME_T rStop;
    HAL_TIME_T rDelta;    

	u4OsdPlane = 1;
	
	if (i4Argc < 2)
    {
        printf("Error input\n");
        return 0;
    }

	u4Case = (UINT32) StrToInt(szArgv[1]);

	if (i4Argc == 3)
    {
        u4ScalerEn = (UINT32) StrToInt(szArgv[2]);
    }
	switch(u4Case)
	{
		case 1:
			u4BmpWidth =480;
			u4BmpHeight =540;
			u4RectWidth =960;
			u4RectHeight =540;
			u4BmpRBuffer =(UINT32)IMAGE_480X530_TESTBMP_1;
			u4BmpLBuffer =(UINT32)IMAGE_480X530_TESTBMP_2;
			break;
		case 2:
			u4BmpWidth =640;
			u4BmpHeight =720;
			u4RectWidth =1280;
			u4RectHeight =720;
			u4BmpRBuffer =(UINT32)IMAGE_640X720_TESTBMP_1;
			u4BmpLBuffer =(UINT32)IMAGE_640X720_TESTBMP_2;
			break;
		case 3:
			u4BmpWidth =960;
			u4BmpHeight =1080;
			u4RectWidth =1920;
			u4RectHeight =1080;
			u4BmpRBuffer =(UINT32)IMAGE_960X1080_TESTBMP_1;
			u4BmpLBuffer =(UINT32)IMAGE_960X1080_TESTBMP_2;
			break;
		default:
			printf("error test case,not support\n");
			break;
	} 

	_GB_SetPitch(u4RectWidth * 4);


	_GB_SetPitch(u4RectWidth * 4);
	_CLI_CMD("gfxbmk.osd %d %d %d %d",u4OsdPlane , u4RectWidth ,u4RectHeight ,u4ScalerEn);
	_GB_GetBackBuffer(&u4FrameBuffer);

	printf("gfx banchmark bit blit test (case %d)",u4Case);
	x_memset((UINT32*)u4FrameBuffer,0,u4RectWidth * u4RectHeight*4);
	x_memset((UINT32*)BUF_ADDR1,0,u4BmpWidth * u4BmpHeight*4);
	x_memset((UINT32*)BUF_ADDR2,0,u4BmpWidth * u4BmpHeight*4);

	_CLI_CMD("gfxbmk.mode cm argb8888");

	//copy right bmp 
	_GB_SetPitch(u4BmpWidth * 4);
	_GB_SetDefMode(BUF_ADDR1, ADR_DST);
	_GB_SetDefMode(u4BmpLBuffer, ADR_SRC);    
	_CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d ",u4BmpWidth,u4BmpHeight);
	_CLI_CMD("gfx.flush");

	//copy left bmp 
	_GB_SetPitch(u4BmpWidth * 4);
	_GB_SetDefMode(BUF_ADDR2, ADR_DST);
	_GB_SetDefMode(u4BmpRBuffer, ADR_SRC);    
	_CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.op.bitblt 0 0 0 0 %d %d ",u4BmpWidth,u4BmpHeight);
	_CLI_CMD("gfx.flush");


	printf( "\n>>Start 2D GPU Bitblit test\n");

	HAL_GetTime(&rStart);

	for(u4Shift = 0;u4Shift <= u4RectWidth/2;u4Shift = u4Shift + 2)
	{
		u4Lstart_x =u4Shift ;
		u4Lstart_y =0 ;
		u4Rstart_x = u4RectWidth/2 - u4Shift;
		u4Rstart_y =0;
		
		//fill rect
		_GB_GetBackBuffer(&u4FrameBuffer);
		_GB_SetPitch(u4RectWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_SRC);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_CLI_CMD("gfxbmk.color 0x7f30ff80");
		_CLI_CMD("gfx.op.rect 0 0 %d %d",u4RectWidth ,u4RectHeight);
		_CLI_CMD("gfx.flush");

		//blit right bmp   (note src and dst pitch)
		_GB_SetPitch(u4RectWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_GB_SetPitch(u4BmpWidth * 4);
		_GB_SetDefMode(BUF_ADDR1, ADR_SRC);

		_CLI_CMD("gfx.op.composeloop  0 0 %d %d %d %d %d %d %d",u4Rstart_x ,u4Rstart_y ,u4BmpWidth ,u4BmpHeight ,u4Ar,u4Mode,0x0);
		_CLI_CMD("gfx.flush");

		//blit left bmp   (note src and dst pitch)
		_GB_SetPitch(u4RectWidth * 4);
		_GB_SetDefMode(u4FrameBuffer, ADR_DST);
		_GB_SetPitch(u4BmpWidth * 4);
		_GB_SetDefMode(BUF_ADDR2, ADR_SRC);
		
		_GFX_SetForceAlpha(TRUE);
		_GFX_SetAlpha(0x80);
		_CLI_CMD("gfx.op.composeloop 0 0 %d %d %d %d %d %d %d",u4Lstart_x ,u4Lstart_y ,u4BmpWidth ,u4BmpHeight,u4Ar,u4Mode,0x0);
		_CLI_CMD("gfx.flush");

		//display
		_GB_SwapFlipto(u4OsdPlane , u4FrameBuffer);
			
	}

    HAL_GetTime(&rStop);
    HAL_GetDeltaTime(&rDelta, &rStart, &rStop);
   	printf( ">>Start(%u.%6u) \n", rStart.u4Seconds, rStart.u4Micros);
	printf( ">>Stop(%u.%6u) \n", rStop.u4Seconds, rStop.u4Micros);
    printf( ">>GetRefTime %u.%6u\n", rDelta.u4Seconds, rDelta.u4Micros);
	printf( ">>End of 2D GPU Bitblit test\n\n");
	
	return 0;
}

/*********************************************************
*********************************************************/
static INT32 _GB_SetMode(INT32 i4Argc, const CHAR ** szArgv)
{

    if (i4Argc == 1)
    {
        printf("In Hardware Mode\n");
        GFX_DifSetMode(1);
    }
    else if (i4Argc == 2 && x_strncmp("sw", szArgv[1], 2) == 0)
    {
        printf("In Software Mode\n");
        GFX_DifSetMode(0);        
    }
    else if (i4Argc == 3 && x_strncmp("cm", szArgv[1], 2) == 0)
    {
        INT32 i4Index;

        for (i4Index = 0; _aszColorMode[i4Index] != (CHAR *)NULL; i4Index++)
        {
            if (x_strcmp(_aszColorMode[i4Index], szArgv[2]) == 0 ||
                ((szArgv[2][0] >= '0' && szArgv[2][0] <= '9') && (i4Index == StrToInt(szArgv[2]))))
            {
                if (i4Index > D_GFX_TEST_AVAILABLE_MODE)
                {
                    printf("Currently mode %s is not supported in test mode\n", _aszColorMode[i4Index]);
                    return -1;
                }
                _u4CurColorModeIndex = (UINT32)i4Index;
                _szCurColorMode = _aszColorMode[i4Index];
				switch(_u4CurColorModeIndex)
				{
					case 0:
						_szDstModeIndex = 14;
						break;
					case 1:
						_szDstModeIndex = 13;
						break;
					case 2:
						_szDstModeIndex = 12;
						break;
					case 3:
						_szDstModeIndex = 11;
						break;
					case 4:
						_szDstModeIndex = 10;
						break;
					default:
						break;					
				}
                #ifdef GFX_BURST_READ
                    _u4GfxPitch = GFX_BURST_READ_ALIGN(_au4DefPitch[i4Index]);
                #else
                    _u4GfxPitch     = _au4DefPitch[i4Index];
                #endif
                _u4ScreenWidth  = _au4ScreenWidth[i4Index];
                _u4ScreenHeight = _au4ScreenHeight[i4Index];

                printf("turn off osd plane 2\n");
                
                _CLI_CMD("osd.plane.enable 2 0");
                
                printf("flip to list and region %d (mode %s pitch %u)\n",
                    i4Index, _szCurColorMode, _u4GfxPitch);
                
                _CLI_CMD("osd.plane.flip 2 %d", i4Index);
                
                printf("turn on osd plane 2\n");
                
                _CLI_CMD("osd.plane.enable 2 1");

                // only ARGB8888 is big_endian
                _CLI_CMD("osd.region.set %d big_endian 0", i4Index);
                _CLI_CMD("osd.region.set %d mix_sel 1", i4Index);
                
                printf("set gfx source and destination to default\n");
                printf("loading data ...\n");
                printf("color mode selection done\n");
                return 0;
            }
        }
        return -1;
    }
    else
    {
    	printf(">>error command\n");
        return -1;
    }
    return 0;
}


/*********************************************************
*********************************************************/
static void _GB_CreateBuffers(void)
{
    UINT32 u4Size = (_u4CanvasWidth * _u4ScreenHeight * 4);
    UINT32 i = 0, u4BufNum = 11;
    
    _u4FbmStartAddr = DRVCUST_OptGet(eFbmMemAddr);
    _u4FbmTotalSize = DRVCUST_OptGet(eFbmMemSize);

    _u4FbmStartAddr = ((_u4FbmStartAddr >> 12) + 1) << 12;    

    Printf("----------------------------------------------------------\n");
    Printf("FBM start addr = 0x%08x\n", _u4FbmStartAddr);
    Printf("FBM total size = %u (MB)\n", (_u4FbmTotalSize / 1048576));

    for (i = 0; i < u4BufNum; i++)
    {
        #ifdef GFX_BURST_READ
            _u4GB_Buf[i] = GFX_BURST_READ_ALIGN(_u4FbmStartAddr + (i * u4Size));
        #else
            _u4GB_Buf[i] = _u4FbmStartAddr + (i * u4Size);
        #endif
    }

    printf("Screen buf addr = 0x%08x\n", BUF_SCREEN);

    printf("Compare buf addr = 0x%08x\n", BUF_BACK);

    printf("Gfx buffer addr 1 = 0x%08x\n", BUF_ADDR1);

    printf("Gfx buffer addr 2 = 0x%08x\n", BUF_ADDR2);

	printf("Gfx buffer addr 3 = 0x%08x\n", BUF_ADDR3);

	printf("Gfx buffer addr 4 = 0x%08x\n", BUF_ADDR4);

	printf("Gfx buffer addr 5 = 0x%08x\n", BUF_ADDR5);

	printf("Gfx buffer addr 6 = 0x%08x\n", BUF_ADDR6);    

	printf("Gfx buffer addr 7 = 0x%08x\n", BUF_ADDR7);

	printf("Gfx buffer addr 8 = 0x%08x\n", BUF_ADDR8);

    printf("Gfx buffer temp = 0x%08x\n", BUF_TEMP);

    printf("----------------------------------------------------------\n");

}


/*********************************************************
*********************************************************/
static INT32 _GB_SetColor(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Color;
    UINT8 u1A, u1R, u1G, u1B;
    UINT32 u4Color4444, u4Color1555, u4Color565;

    if (i4Argc < 2)
    {
        Printf("gfxbmk.color {color}\n");
        Printf("current color is 0x%08x\n", _u4CurColor);
        return 0;
    }
    u4Color = StrToInt(szArgv[1]);

    u1A = u4Color >> 24;
    u1R = (u4Color >> 16) & 0xff;
    u1G = (u4Color >> 8) & 0xff;
    u1B = u4Color & 0xff;
    u4Color4444 = ((u1A << 8) & 0xf000) | ((u1R << 4) & 0x0f00) | (u1G & 0x00f0) | (u1B >> 4);
    u4Color1555 = (u1A == 0 ? 0 : 0x8000) | ((u1R << 7) & 0x7c00) | ((u1G << 2) & 0x03e0) | (u1B >> 3);
    u4Color565 = ((u1R << 8) & 0xf800) | ((u1G << 3) & 0x07e0) | (u1B >> 3);

    u4Color4444 |= u4Color4444 << 16;
    u4Color1555 |= u4Color1555 << 16;
    u4Color565  |= u4Color565  << 16;

    switch (_u4CurColorModeIndex)
    {
    case 1:     // argb4444
        _CLI_CMD("gfx.set.color 0x%08x", u4Color4444);
        break;
    case 2:     // argb1555
        _CLI_CMD("gfx.set.color 0x%08x", u4Color1555);
        break;
    case 3:     // rgb565
        _CLI_CMD("gfx.set.color 0x%08x", u4Color565);
        break;
    case 4:     // rgb8
        _CLI_CMD("gfx.set.color 0x%02x%02x%02x%02x", u4Color & 0xff, u4Color & 0xff, u4Color & 0xff, u4Color & 0xff);
        break;
    default:
        _CLI_CMD("gfx.set.color 0x%08x", u4Color);
        break;
    }
    return 0;
}



/*********************************************************
*********************************************************/
static INT32 _GB_SetDefMode(UINT32 u4BuffAddr, BOOL fgSrc)
{
    UINT32 u4Pitch;
    #ifdef GFX_BURST_READ
        u4Pitch = GFX_BURST_READ_ALIGN(_u4GfxPitch);
        printf("_GB_SetDefMode pitch : %d %s\n",u4Pitch,_szCurColorMode);
    #else
        u4Pitch = _u4GfxPitch;
    #endif
    _CLI_CMD("gfx.set.%s 0x%08x %s %u", 
        fgSrc ? "src" : "dst", u4BuffAddr, _szCurColorMode, u4Pitch);
    return 0;
}



/*********************************************************
*********************************************************/
static INT32 _GB_Init(INT32 i4Argc, const CHAR ** szArgv)
{
    //==========================================
    //...
    //==========================================
	printf( "...\n");

    // create buffers for emulation use
    _GB_CreateBuffers();
    HAL_GetTime(&_RTIME_TAG);  
	_CLI_CMD("mpv.d_l 0");
	_CLI_CMD("nptv.scl.d_l 0");
	_CLI_CMD("nptv.d_l 0");
	_CLI_CMD("nptv.di.d_l 0");
	_CLI_CMD("nptv.vrm.d_l 0");
	_CLI_CMD("fbm.d_l 0");

    //==========================================
    //init display flow
    //==========================================
    _CLI_CMD("fbm.i");
    
	printf( "Plane mixer init ...\n");
    _CLI_CMD("pmx.i");
	printf("Plane mixer init ... done\n");
	
	printf( "OSD init ...\n");
    _CLI_CMD("osd.init");

	_u4PanelWidth = PANEL_GetPanelWidth();
	_u4PanelHeight = PANEL_GetPanelHeight();

    // default create 7 lists
    _CLI_CMD("osd.list.create");    // 0
    _CLI_CMD("osd.list.create");    // 1
    _CLI_CMD("osd.list.create");    // 2
    _u4SwapRList = 2;
	UNUSED(_u4SwapRList);


    _CLI_CMD("osd.region.new 720 480 %u 14 2880 0 0 720 480", BUF_SCREEN); // 0 - ARGB8888 720x480
    _CLI_CMD("osd.region.new 720 480 %u 13 1440 0 0 720 480", BUF_SCREEN); // 1 - ARGB4444 720x480
	_u4CurrentBuffer = BUF_SCREEN;

    _CLI_CMD("osd.region.insert 0 0");
    _CLI_CMD("osd.region.insert 1 1");
	_CLI_CMD("osd.region.insert 2 2");
	_u4SwapRegion = 2;

    _CLI_CMD("osd.plane.flip 2 0"); // select region list 0 (region 0 - argb8888) as default
	
    _CLI_CMD("osd.sc.s 0 720 480 720 480");
    _CLI_CMD("osd.plane.enable 2 1");
        
    _CLI_CMD("osd.region.set 0 big_endian 0");  // region 0: little_endian (0)
    _CLI_CMD("osd.region.set 1 big_endian 0");  // region 1: little_endian (0) 

    _CLI_CMD("osd.region.set 0 mix_sel 3"); // region 0: pixel mode (1)
    _CLI_CMD("osd.region.set 1 mix_sel 3"); // region 1: pixel mode (1)

	printf("OSD init ... done\n");

	
    //==========================================
    //init gfx
    //==========================================
	printf("GFX Emu init ...\n");

    _CLI_CMD("gfx.init");
    _GB_SetDefMode(BUF_SCREEN, ADR_DST);
    _GB_SetDefMode(BUF_SCREEN, ADR_SRC);
    _CLI_CMD("gfx.set.bltopt 0 0 0");
    _CLI_CMD("gfx.set.alpha 255");
    _CLI_CMD("gfxbmk.color 0xffffffff");
    _CLI_CMD("gfx.flush off");	

    //==========================================
    //init gfx
    //==========================================
    _GB_SetDefMode(BUF_SCREEN, ADR_DST);
    _GB_SetDefMode(BUF_SCREEN, ADR_SRC);
 
    srand(1827364);
	UNUSED(_szDstModeIndex);
	UNUSED(_u4ScreenWidth);

    return 0;
}

/*********************************************************
*********************************************************/
#if 0
static INT32 _GB_ShowScreen(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4Plane = (INT32) StrToInt(szArgv[1]);
	UINT32 u4PanelWidth =0, u4PanelHeight =0;
	UINT32 u4Region =0, u4RegionList =0;
	UINT32 u4ColorMode =0, u4Width =0, u4Height =0, u4BmpAddr =0, u4BmpPitch =0;
	UINT32 u4DataSize =0 , u4ShowBufAddr =0;

	OSD_PLA_Enable(u4Plane, FALSE);
	
	u4PanelWidth = PANEL_GetPanelWidth();
	u4PanelHeight = PANEL_GetPanelHeight();
	
	LOG(5, "Panel %ld %ld \n", u4PanelWidth, u4PanelHeight);

	u4ColorMode =14;
	u4Width =480;
	u4Height =540;
	u4BmpAddr =BUF_SCREEN;

	OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch);
	u4DataSize=u4BmpPitch*u4Height;

	u4ShowBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4DataSize, 16);
    if (u4ShowBufAddr == 0)
    {
        Printf("u4LogoBuf null\n");
        return FALSE;
    }
    x_memcpy((void*)VIRTUAL(u4ShowBufAddr), (void*)u4BmpAddr, u4DataSize);
	HalFlushInvalidateDCacheMultipleLine((UINT32)(VIRTUAL(u4ShowBufAddr)),u4DataSize);
    u4BmpAddr = u4ShowBufAddr;  

	OSD_RGN_Create(&u4Region, u4Width, u4Height, (void *)u4BmpAddr,
                            u4ColorMode, u4BmpPitch, 0, 0, u4Width, u4Height);

	OSD_RGN_Insert(u4Region, u4RegionList);
    OSD_PLA_FlipTo(u4Plane, u4RegionList);
    
	return 0;
}

#endif



