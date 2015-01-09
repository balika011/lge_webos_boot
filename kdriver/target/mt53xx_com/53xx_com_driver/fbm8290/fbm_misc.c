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
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: fbm_misc.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_misc.c
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "fbm_drvif.h"
#include "fbm_fb.h"
#include "fbm_cs.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_debug.h"
#include "x_stl_lib.h"
#include "b2r_drvif.h"
#include "x_drv_cli.h"
#include "x_hal_arm.h"

#ifdef MAIN_SUB_OPEN
#include "x_hal_io.h"
#include "x_hal_5381.h"
#endif

#ifdef FBM_VIRTUAL_MPEG
#include "drv_t32.h"
#endif

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#ifdef FBM_VIRTUAL_MPEG
#ifndef FBM_VIRTUAL_MACHINE
#define FBM_VIRTUAL_MACHINE
#endif
#endif

#ifdef FBM_VIRTUAL_DISPLAY
#ifndef FBM_VIRTUAL_MACHINE
#define FBM_VIRTUAL_MACHINE
#endif
#endif


#ifdef FBM_VIRTUAL_MACHINE

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define FBM_VD_STACK_SIZE                           2048

#define FBM_VD_THREAD_PRIORITY                      98

#define FBM_VM_STACK_SIZE                           2048

#define FBM_VM_THREAD_PRIORITY                      99

#define FBM_VM_START_ADDR                           (81 * 1024 * 1024)  // from 65 ~ 256 MB
//#define FBM_VM_START_ADDR                           (0x1b5eb800)  // from 65 ~ 256 MB

#define R_MAP_MAX_VALUE								8
#define R_MAP_STEP									2

#ifdef FBM_VIRTUAL_MPEG
#define WIDTH 			"width"
#define HEIGHT 			"height"
#define PITCH 			"pitch"
#define ADDR_MODE		"addr_mode"
#define PICTURE_TYPE	"picture_type"
#define PICTURE_NUMBER  "picture_number"
#define DIMENSION		"dimension"
#endif

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
#ifdef FBM_VIRTUAL_MPEG
typedef struct tagDirList
{
	CHAR* dir;
	struct tagDirList* next;
}*FBM_B2R_AUTO_TEST_DIR_LIST;

enum
{
	PICTURE_2D,
	PICTURE_SBS,
	PICTURE_TAB,
	PICTURE_TWO_ADDR,
	PICTURE_PIC
};

enum
{
	NOT_START,
	START,
	DOING,
	FINISH
};

typedef struct
{
	int width;
	int height;
	int pitch;
	int addr_mode;
	int picture_type;
	int picture_number;
	int dimension;
}FBM_B2R_AUTO_TEST_PIC_PARAM_T;

typedef struct 
{
	UINT16 active_width;
	UINT16 active_height;
	UINT16 pitch;
	UINT32 width;
	UINT32 height;
	UINT32 frame_size;
}FBM_B2R_AUTO_TEST_FBG_CFG_T;


#endif
//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


extern FBM_FBG_T _arFbg[FBG_MAX_NS];

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define VERIFY_FBG(id)                                  \
    ((id >= FBG_MAX_NS) ||                              \
    (_arFbg[id].ucFbgType == FBM_FBG_TYPE_UNKNOWN) ||   \
    (_arFbg[id].ucFbgId != id))

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
#ifdef FBM_VIRTUAL_DISPLAY

static void _FbmVirutualDispRoutine(void* pvArg);

#endif

#ifdef FBM_VIRTUAL_MPEG

static void _FbmVirutualMpegRoutine(void* pvArg);

static void _FbmVirtualGetPictureParam(CHAR* prSource, FBM_B2R_AUTO_TEST_PIC_PARAM_T* prParam);

static void _FbmVirtualParseCRC(CHAR* pcSource, UCHAR ucType);

static void _FbmVirtualBuildDirList(CHAR* pcSource);

static void _FbmVirtualDistroyDirList(void);

static BOOL _FbmVirtualCompareCRC_MirrorFlip(UCHAR ucIndex, BOOL fg2D, BOOL fgTwoAddr, BOOL fgProgressive);

static BOOL _FbmVirtualCompareCRC_SingleMirrorFlip(UCHAR ucIndex, BOOL fg2D, BOOL fgTwoAddr, UCHAR ucMirror, UCHAR ucFlip, BOOL fgProgressive);

static BOOL _FbmVirtualDoCompareCRC(UCHAR ucIndex, BOOL fg2D, BOOL fgTwoAddr, UCHAR ucType);

static BOOL _FbmVirtualCompareCRC_Interlace(UCHAR ucIndex, BOOL fg2D, BOOL fgTwoAddr, CHAR* pcDir);

static BOOL _FbmVirtualCompareCRC_Progressive(UCHAR ucIndex, BOOL fg2D, BOOL fgTwoAddr, CHAR* pcDir);

static BOOL _FbmVirtualCompareCRC_2D(CHAR* pcDir, UINT32 u4Num);

static BOOL _FbmVirtualCompareCRC_SBS(CHAR* pcDir, UINT32 u4Num);

static BOOL _FbmVirtualCompareCRC_TAB(CHAR* pcDir, UINT32 u4Num);

static BOOL _FbmVirtualCompareCRC_TwoAddr(CHAR* pcDir, UINT32 u4Num);

static BOOL _FbmVirtualCompareCRC_Picture(CHAR* pcDir, UINT32 u4Num);

static BOOL _FbmVirtualSelectFbgConfig(UINT32 u2Height, UINT32 u2Width, UINT32 u2Pitch);

static void _FbmVirtualCreateFbg(BOOL fgProgressive, UCHAR ucPicType, UCHAR ucFrameRateIndex);

static void _FbmVirtualSendFrame2Display(void);

static void _FbmVirtualLoadBitstream(UCHAR ucType, UINT32 u4Index, CHAR* pcDir);

static void _FbmVirtualLoadCRC(UCHAR ucType, UINT32 u4Index, CHAR* pcDir);


static UINT32 _FbmVirtualGetCRC(UCHAR ucType);
#ifdef MAIN_SUB_OPEN
static UINT32 _FbmVirtualGetCRC_Sub(UCHAR ucType);
static UINT32 _FbmVirtualGetCRC_Main(UCHAR ucType);
#endif

static void _FbmVirtualRunCli(void);


#endif

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
#ifdef FBM_VIRTUAL_DISPLAY

static UCHAR _ucFbmVirtualDispStart = 0;

static UCHAR _ucFbmVirtualDispDestroy = 0;

static UINT32 _ucFbmVirtualDispDelay = 0;

#endif

#ifdef FBM_VIRTUAL_MPEG

UINT32 _u4YAddr = 0;

UINT32 _u4CAddr = 0;
#ifdef MAIN_SUB_OPEN
UINT32 _u4YAddr_2 = 0;
UINT32 _u4CAddr_2 = 0;
#endif

UINT32 _u4YAddr_TwoAddr = 0;

UINT32 _u4CAddr_TwoAddr = 0;

static UCHAR _ucFbmVirtualMpegStart = 0;

static UCHAR _ucFbmVirtualMpegDestroy = 0;

static UINT32 _ucFbmVirtualMpegDelay = 0;

static FBM_B2R_AUTO_TEST_DIR_LIST _prB2rAutoTestDirList = NULL;
static FBM_B2R_AUTO_TEST_DIR_LIST _prCurB2rAutoTestDir = NULL;

static UINT32 _ar2DCRC[16] = {0};
static UINT32 _ar3DCRC[32] = {0};
static UINT32 _ar2DCRC_TwoAddr[16] = {0};

static UCHAR _ucFbgId;

static FBM_B2R_AUTO_TEST_FBG_CFG_T _rB2rAutoTestFbgCfg;

static INT32 _i4B2rVerifyStatus = NOT_START;

static UINT32 _u4CRC_INTERLACE_TOP = 0xFFFFFFFF;

static UINT32 _u4CRC_INTERLACE_BOTTOM = 0xFFFFFFFF;
#ifdef MAIN_SUB_OPEN
static UINT32 _u4CRC_INTERLACE_TOP_Sub = 0xFFFFFFFF;
static UINT32 _u4CRC_INTERLACE_BOTTOM_Sub = 0xFFFFFFFF;
static UINT32 _u4CRC_INTERLACE_TOP_Main = 0xFFFFFFFF;
static UINT32 _u4CRC_INTERLACE_BOTTOM_Main = 0xFFFFFFFF;
#endif

static HANDLE_T _hMutex_B2rAutoTest;

static UINT32 _u4DelayTime;

static UINT32 _u4AddrMode;

static UINT32 _u4PicDimension;

#endif



//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _FbmVirutualDispRoutine
 *  VD Thread
 */
//-------------------------------------------------------------------------
#ifdef FBM_VIRTUAL_MPEG

static void _FbmVirtualRunCli(void)
{
	/*CLI_Parser("av.l 0");
	CLI_Parser("mid.r");
	CLI_Parser("fbm.i");
	CLI_Parser("vdp.i");
	CLI_Parser("vdp.e 0 1");
	CLI_Parser("vdp.s.i 0 0 0 ");
	CLI_Parser("aud.i");
	CLI_Parser("gfx.i");
	CLI_Parser("n.i");
	CLI_Parser("n.ms dtv1");*/
}


static void _FbmVirtualGetPictureParam(CHAR * pcSource, FBM_B2R_AUTO_TEST_PIC_PARAM_T * prParam)
{
	CHAR* p = pcSource;
	CHAR token = '=';
	CHAR* pcBuf = x_mem_alloc(256 * sizeof(CHAR));
	CHAR* q = p;
	CHAR* s = q;;
	CHAR* t = q;;
	while (*p != -1) 
	{
		if (*p == token)
		{
			s = p;
			p++;
		}
		else if (*p == '\r')
		{
			t = p - 1;
			if (s > q && t > s)
			{
				*s = '\0';
				*p = '\0';
				s++;
				x_strcpy(pcBuf, q);
				if (!x_strcmp(pcBuf, WIDTH))
				{
					x_strcpy(pcBuf, s);
					prParam->width = StrToInt(pcBuf);
				}
				if (!x_strcmp(pcBuf, HEIGHT))
				{
					x_strcpy(pcBuf, s);
					prParam->height = StrToInt(pcBuf);
				}
				if (!x_strcmp(pcBuf, PITCH))
				{
					x_strcpy(pcBuf, s);
					prParam->pitch = StrToInt(pcBuf);
				}
				if (!x_strcmp(pcBuf, ADDR_MODE))
				{
					x_strcpy(pcBuf, s);
					prParam->addr_mode = StrToInt(pcBuf);
				}
				if (!x_strcmp(pcBuf, PICTURE_TYPE))
				{
					x_strcpy(pcBuf, s);
					prParam->picture_type = StrToInt(pcBuf);
				}
				if (!x_strcmp(pcBuf, PICTURE_NUMBER))
				{
					x_strcpy(pcBuf, s);
					prParam->picture_number= StrToInt(pcBuf);
				}
				if (!x_strcmp(pcBuf, DIMENSION))
				{
					x_strcpy(pcBuf, s);
					prParam->dimension= StrToInt(pcBuf);
				}
			}
			p++;
		}
		else if (*p == '\n')
		{
			p++;
			q = p;
			s = q;
			t = q;
		}
		else 
		{
			p++;
		}
	}
	x_mem_free(pcBuf);
}

static void _FbmVirtualParseCRC(CHAR * pcSource, UCHAR ucType)
{
	CHAR* p = pcSource;
	while (*p != -1)
	{
		BOOL fgTAB = FALSE;
		BOOL fgSBS = FALSE;
		int index = 0;
		while (*p != '\r')
		{
			int d;
			switch (*p)
			{
			case 'T':
				fgTAB = TRUE;
				p += 2;
				continue;
			case 'L':
				fgSBS = TRUE;
				p += 2;
				continue;
			case 'B':
				fgTAB = TRUE;
				index += (1 << 4);
				p += 2;
				continue;
			case 'R':
				fgSBS = TRUE;
				index += (1 << 4);
				p += 2;
				continue;
			case 'P':
				index += (1 << 3);
				p += 2;
				continue;
			case 'I':
				p++;
				index += ((*p - 48) << 2);
				p += 2;
				continue;
			case 'F':
				p++;
				index += ((*p - 48) << 1);
				p += 2;
				continue;
			case 'M':
				p++;
				index += (*p - 48);
				p++;
				continue;
			case ' ':
				p++;
				x_sscanf(p, "%x", &d);
				if (fgSBS)
				{
					if (ucType == PICTURE_SBS)
					{
						_ar3DCRC[index] = d;
					}
				}
				else if (fgTAB)
				{
					if (ucType == PICTURE_TAB)
					{
						_ar3DCRC[index] = d;
					}
				}
				else 
				{
					if (ucType == PICTURE_TWO_ADDR)
					{
						_ar2DCRC_TwoAddr[index]= d;
					}
					else 
					{
						_ar2DCRC[index] = d;
					}
				}
				continue;
			default:
				p++;
				continue;
			}
		}
		p += 2;
	}
}

static void _FbmVirtualBuildDirList(CHAR * pcSource)
{
	_prB2rAutoTestDirList = (struct tagDirList*)x_mem_alloc(sizeof(struct tagDirList));
	_prB2rAutoTestDirList->next = NULL;
	_prCurB2rAutoTestDir = _prB2rAutoTestDirList;
	CHAR* p = pcSource;
	UINT32 count = 0;
	CHAR* q = p;
	while (*p != -1)
	{
		if (*p == '\r')
		{
			FBM_B2R_AUTO_TEST_DIR_LIST prTmpDir =  (struct tagDirList*)(x_mem_alloc(sizeof(struct tagDirList)));
			prTmpDir->dir = (CHAR*)x_mem_alloc(sizeof(CHAR) * (count + 1));
			x_strncpy(prTmpDir->dir, q, count);
			*(prTmpDir->dir + count) = '\0';
			_prCurB2rAutoTestDir->next = prTmpDir;
			prTmpDir->next = NULL;
			_prCurB2rAutoTestDir = prTmpDir;
			count = 0;
			p += 2;
			q = p;
		}
		else
		{
			p++, count++;
		}
	}
	_prCurB2rAutoTestDir = _prB2rAutoTestDirList->next;
	_i4B2rVerifyStatus = START;
}

static void _FbmVirtualDistroyDirList()
{
	_prCurB2rAutoTestDir = _prB2rAutoTestDirList->next;
	FBM_B2R_AUTO_TEST_DIR_LIST prTmpDir = _prB2rAutoTestDirList->next;
	while (_prCurB2rAutoTestDir)
	{
		prTmpDir = _prCurB2rAutoTestDir->next;
		if (_prCurB2rAutoTestDir->dir)
		{
			x_mem_free(_prCurB2rAutoTestDir->dir);
		}
		x_mem_free(_prCurB2rAutoTestDir);
		_prCurB2rAutoTestDir = prTmpDir;
	}
}

static void _FbmVirtualSendFrame2Display()
{
	UCHAR ucFbId;
	x_thread_delay(_ucFbmVirtualMpegDelay);
#ifdef MAIN_SUB_OPEN
	IO_WRITE32(0xF0026000, 0,  _u4YAddr);
	IO_WRITE32(0xF0026004, 0, _u4CAddr);
	IO_WRITE32(0xF0026100, 0, _u4YAddr_2);
	IO_WRITE32(0xF0026104, 0, _u4CAddr_2);
	ucFbId =  FBM_GetEmptyRefFrameBuffer(_ucFbgId, 100);
	FBM_SetFrameBufferStatus(_ucFbgId, ucFbId, FBM_FB_STATUS_READY);
	FBM_SetFrameBufferStatus(_ucFbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);
	
#else
	ucFbId =  FBM_GetEmptyRefFrameBuffer(_ucFbgId, 100);	
	if (ucFbId != FBM_FB_ID_UNKNOWN)
	{
		_arFbg[_ucFbgId].au4AddrY[ucFbId] = _u4YAddr;
		_arFbg[_ucFbgId].au4AddrC[ucFbId] = _u4CAddr;
		FBM_SetFrameBufferStatus(_ucFbgId, ucFbId, FBM_FB_STATUS_READY);
		FBM_SetFrameBufferStatus(_ucFbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);
	}
#endif
}

static BOOL _FbmVirtualDoCompareCRC(UCHAR ucIndex, BOOL fg2D, BOOL fgTwoAddr, UCHAR ucType)
{
	UINT32 u4RealCrc;
	UINT32 u4TheoryCrc;
#ifdef MAIN_SUB_OPEN
	UINT32 u4RealCrcSub;
	UINT32 u4RealCrcMain;
#endif
	if (fgTwoAddr)
	{
		VDP_HalDoB2RAutoTestCRCReadSetting(PICTURE_TWO_ADDR_VIEW, 0);
		Printf("Left view\n");
	}
#ifdef MAIN_SUB_OPEN
	_FbmVirtualSendFrame2Display();
	x_thread_delay(_u4DelayTime);
	_u4DelayTime = 200;
	u4RealCrcSub = _FbmVirtualGetCRC_Sub(ucType);
	u4RealCrcMain = _FbmVirtualGetCRC_Main(ucType);
	u4TheoryCrc = (fg2D ? _ar2DCRC[ucIndex] : _ar3DCRC[ucIndex]);
	Printf("Threory CRC : %x VS Sub Real CRC : %x VS Main Real CRC: %x\n", u4TheoryCrc, u4RealCrcSub, u4RealCrcMain);
	if ((u4TheoryCrc != u4RealCrc) || (u4RealCrcSub != u4RealCrcMain))
	{
		return FALSE;
	}
#else
	_FbmVirtualSendFrame2Display();
	x_thread_delay(_u4DelayTime);
	_u4DelayTime = 200;
	u4RealCrc = _FbmVirtualGetCRC(ucType);
	u4TheoryCrc = (fg2D ? _ar2DCRC[ucIndex] : _ar3DCRC[ucIndex]);
	Printf("Threory CRC : %x VS Real CRC : %x\n", u4TheoryCrc, u4RealCrc);
	if (u4TheoryCrc != u4RealCrc)
	{
		return FALSE;
	}
#endif
	if (fgTwoAddr)
	{
		Printf("Right view\n");
		VDP_HalDoB2RAutoTestCRCReadSetting(PICTURE_TWO_ADDR_VIEW, 1);
		x_thread_delay(_u4DelayTime);
		u4RealCrc = _FbmVirtualGetCRC(ucType);
		u4TheoryCrc = _ar2DCRC_TwoAddr[ucIndex];
		Printf("Threory CRC : %x VS Real CRC : %x\n", u4TheoryCrc, u4RealCrc);
		return (u4TheoryCrc == u4RealCrc);
	}
	return TRUE; 
}


static BOOL _FbmVirtualCompareCRC_SingleMirrorFlip(UCHAR ucIndex,BOOL fg2D,BOOL fgTwoAddr,UCHAR ucMirror,UCHAR ucFlip, BOOL fgProgressive)
{
	UCHAR ucType;
	Printf("Mirror = %d, Flip = %d\n", ucMirror, ucFlip);
	VDP_HalDoB2RAutoTestCRCReadSetting(PICTURE_MIRROR, ucMirror);
	VDP_HalDoB2RAutoTestCRCReadSetting(PICTURE_FLIP, ucFlip);
	UCHAR ucIndexT = ucIndex;
	UCHAR ucIndexB = ucIndex;
	if (fgProgressive)
	{
		ucIndex += (1 << 3);
		ucIndex += (ucFlip<< 1) + (ucMirror<< 0);
		ucType = CRC_INTERLACE_TOP;
		if (!_FbmVirtualDoCompareCRC(ucIndex, fg2D, fgTwoAddr, ucType))
		{
			return FALSE;
		}
	}
	else
	{	
		ucIndexB += (1 << 2);
		ucIndexB += (ucFlip<< 1) + (ucMirror<< 0);
		ucType = (ucFlip ? CRC_INTERLACE_BOTTOM: CRC_INTERLACE_TOP);
		Printf("Top field\n");
		if (!_FbmVirtualDoCompareCRC(ucIndexB, fg2D, fgTwoAddr, ucType))
		{
			return FALSE;
		}
		ucIndexT += (ucFlip<< 1) + (ucMirror<< 0);
		ucType = (ucFlip ? CRC_INTERLACE_TOP: CRC_INTERLACE_BOTTOM);
		Printf("Bottom field\n");
		if (!_FbmVirtualDoCompareCRC(ucIndexT, fg2D, fgTwoAddr, ucType))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

static BOOL _FbmVirtualCompareCRC_MirrorFlip(UCHAR ucIndex, BOOL fg2D, BOOL fgTwoAddr, BOOL fgProgressive)
{
	if (!_FbmVirtualCompareCRC_SingleMirrorFlip(ucIndex, fg2D, fgTwoAddr, 0, 0, fgProgressive))
	{
		return FALSE;
	}
	if (!_FbmVirtualCompareCRC_SingleMirrorFlip(ucIndex, fg2D, fgTwoAddr, 0, 1, fgProgressive))
	{
		return FALSE;
	}
	if (!_FbmVirtualCompareCRC_SingleMirrorFlip(ucIndex, fg2D, fgTwoAddr, 1, 0, fgProgressive))
	{
		return FALSE;
	}
	if (!_FbmVirtualCompareCRC_SingleMirrorFlip(ucIndex, fg2D, fgTwoAddr, 1, 1, fgProgressive))
	{
		return FALSE;
	}
	return TRUE;
}

static BOOL _FbmVirtualCompareCRC_Progressive(UCHAR ucIndex, BOOL fg2D, BOOL fgTwoAddr, CHAR* pcDir)
{
	Printf("Progressive\n");
	if (!_FbmVirtualCompareCRC_MirrorFlip(ucIndex, fg2D, fgTwoAddr, TRUE))
	{
		return FALSE;
	}
	return TRUE;
}

static BOOL _FbmVirtualCompareCRC_Interlace(UCHAR ucIndex, BOOL fg2D, BOOL fgTwoAddr, CHAR* pcDir)
{
	Printf("Interlace\n");
	if (!_FbmVirtualCompareCRC_MirrorFlip(ucIndex, fg2D, fgTwoAddr, FALSE))
	{
		return FALSE;
	}
	return TRUE;
}

static void _FbmVirtualLoadCRC(UCHAR ucType,UINT32 u4Index,CHAR * pcDir)
{
	CHAR* pcBuf = x_mem_alloc(2048 * sizeof(CHAR));
	x_memset(pcBuf, 0xFF, 2048 * sizeof(CHAR));
	Printf("d.load.binary \"%s/out_%d.crc\" 0x%x /ny\n", pcDir, u4Index, pcBuf);
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/out_%d.crc\" 0x%x /ny", pcDir, u4Index, pcBuf));
	if (ucType == PICTURE_TWO_ADDR)
	{
		_FbmVirtualParseCRC(pcBuf, PICTURE_2D);
	}
	else
	{
		_FbmVirtualParseCRC(pcBuf, ucType);
	}

	if (ucType == PICTURE_TWO_ADDR)
	{
		++u4Index;
		x_memset(pcBuf, 0xFF, 2048 * sizeof(CHAR));
		Printf("d.load.binary \"%s/out_%d.crc\" 0x%x /ny\n", pcDir, u4Index, pcBuf);
		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/out_%d.crc\" 0x%x /ny", pcDir, u4Index, pcBuf));
		_FbmVirtualParseCRC(pcBuf, ucType);
	}

	x_mem_free(pcBuf);
}

static void _FbmVirtualLoadBitstream(UCHAR ucType, UINT32 u4Index, CHAR* pcDir)
{
	Printf("d.load.binary \"%s/out_%d.ramY\" 0x%x /ny\n", pcDir, u4Index, FBM_VM_START_ADDR);
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/out_%d.ramY\" 0x%x /ny\n", pcDir, u4Index, FBM_VM_START_ADDR));
	_u4YAddr = FBM_VM_START_ADDR;
	Printf("d.load.binary \"%s/out_%d.ramCbCr\" 0x%x /ny\n", pcDir, u4Index, FBM_VM_START_ADDR + _rB2rAutoTestFbgCfg.frame_size);
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/out_%d.ramCbCr\" 0x%x /ny\n", pcDir, u4Index, FBM_VM_START_ADDR + _rB2rAutoTestFbgCfg.frame_size));	
	_u4CAddr = FBM_VM_START_ADDR + _rB2rAutoTestFbgCfg.frame_size;

	//add 2 path crc test
#ifdef MAIN_SUB_OPEN
	Printf("-------load again--------!\n");
	Printf("d.load.binary \"%s/out_%d.ramY\" 0x%x /ny\n", pcDir, u4Index, FBM_VM_START_ADDR + (_rB2rAutoTestFbgCfg.frame_size * 4));
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/out_%d.ramY\" 0x%x /ny\n", pcDir, u4Index, FBM_VM_START_ADDR + (_rB2rAutoTestFbgCfg.frame_size * 4)));
	_u4YAddr_2 = FBM_VM_START_ADDR + (_rB2rAutoTestFbgCfg.frame_size * 4);
	Printf("d.load.binary \"%s/out_%d.ramCbCr\" 0x%x /ny\n", pcDir, u4Index, FBM_VM_START_ADDR + (_rB2rAutoTestFbgCfg.frame_size * 5));
	UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/out_%d.ramCbCr\" 0x%x /ny\n", pcDir, u4Index, FBM_VM_START_ADDR + (_rB2rAutoTestFbgCfg.frame_size * 5)));	
	_u4CAddr_2 = FBM_VM_START_ADDR + (_rB2rAutoTestFbgCfg.frame_size * 5);
#endif
	if (ucType == PICTURE_TWO_ADDR)
	{
		++u4Index;
		Printf("d.load.binary \"%s/out_%d.ramY\" 0x%x /ny\n", pcDir, u4Index, FBM_VM_START_ADDR + _rB2rAutoTestFbgCfg.frame_size * 2);
		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/out_%d.ramY\" 0x%x /ny", pcDir, u4Index, FBM_VM_START_ADDR + _rB2rAutoTestFbgCfg.frame_size * 2));
		_u4YAddr_TwoAddr = FBM_VM_START_ADDR + _rB2rAutoTestFbgCfg.frame_size * 2;
		Printf("d.load.binary \"%s/out_%d.ramCbCr\" 0x%x /ny\n", pcDir, u4Index, FBM_VM_START_ADDR + _rB2rAutoTestFbgCfg.frame_size * 3);
		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s/out_%d.ramCbCr\" 0x%x /ny", pcDir, u4Index, FBM_VM_START_ADDR + _rB2rAutoTestFbgCfg.frame_size * 3));	
		_u4CAddr_TwoAddr = FBM_VM_START_ADDR + _rB2rAutoTestFbgCfg.frame_size * 3;
		VDP_HalDoB2RAutoTestCRCReadSetting(TWO_ADDR_Y_ADDR, _u4YAddr_TwoAddr);
		VDP_HalDoB2RAutoTestCRCReadSetting(TWO_ADDR_C_ADDR, _u4CAddr_TwoAddr);
	}

	HalFlushInvalidateDCache();
	
	Printf("%s/out_%d.ramY\n%s/out_%d.ramCbCr\n", pcDir, u4Index, pcDir, u4Index);
}

static BOOL _FbmVirtualCompareCRC_Picture(CHAR * pcDir,UINT32 u4Num)
{
	UCHAR ucCrcIndex = 0;
	UINT32 u4Index = 0; 
	
	_FbmVirtualCreateFbg(TRUE, PICTURE_2D, MPEG_FRAME_RATE_30);
	while (u4Index < u4Num)
	{	
		_FbmVirtualLoadBitstream(PICTURE_PIC, u4Index, pcDir);
		_FbmVirtualLoadCRC(PICTURE_PIC, u4Index, pcDir);
		if (!_FbmVirtualCompareCRC_Progressive(ucCrcIndex, TRUE, FALSE, pcDir))
		{
			return FALSE;
		}
		u4Index++;
	}
	return TRUE;
}

static BOOL _FbmVirtualCompareCRC_2D(CHAR* pcDir, UINT32 u4Num)
{
	UCHAR ucCrcIndex = 0;
	UINT32 u4Index = 0; 
	
	_FbmVirtualCreateFbg(TRUE, PICTURE_2D, MPEG_FRAME_RATE_30);
	while (u4Index < u4Num)
	{	
		_FbmVirtualLoadBitstream(PICTURE_2D, u4Index, pcDir);
		_FbmVirtualLoadCRC(PICTURE_2D, u4Index, pcDir);
		if (!_FbmVirtualCompareCRC_Progressive(ucCrcIndex, TRUE, FALSE, pcDir))
		{
			return FALSE;
		}	
		u4Index++;
	}
	u4Index = 0;
	_FbmVirtualCreateFbg(FALSE, PICTURE_2D, MPEG_FRAME_RATE_30);
	_u4DelayTime = 500;
	while (u4Index < u4Num)
	{
		_FbmVirtualLoadBitstream(PICTURE_2D, u4Index, pcDir);
		_FbmVirtualLoadCRC(PICTURE_2D, u4Index, pcDir);
		if (!_FbmVirtualCompareCRC_Interlace(ucCrcIndex, TRUE, FALSE, pcDir))
		{
			return FALSE;
		}
		u4Index++;
	}
	return TRUE;
}

static BOOL _FbmVirtualCompareCRC_SBS(CHAR* pcDir, UINT32 u4Num)
{
	UCHAR ucCrcIndex = 0;
	UINT32 u4Index = 0; 
	
	_FbmVirtualCreateFbg(TRUE, PICTURE_SBS, MPEG_FRAME_RATE_30);
	while (u4Index < u4Num)
	{
		_FbmVirtualLoadBitstream(PICTURE_SBS, u4Index, pcDir);
		_FbmVirtualLoadCRC(PICTURE_SBS, u4Index, pcDir);
		VDP_HalDoB2RAutoTestCRCReadSetting(H_OFFSET, 0);
		UCHAR ucCrcIndexL = ucCrcIndex;
		Printf("Left view\n");
		if (!_FbmVirtualCompareCRC_Progressive(ucCrcIndexL, FALSE, FALSE, pcDir))
		{
			return FALSE;
		}
		VDP_HalDoB2RAutoTestCRCReadSetting(H_OFFSET, _rB2rAutoTestFbgCfg.active_width / 2);
		UCHAR ucCrcIndexR = ucCrcIndex;
		ucCrcIndexR += (1 << 4);
		Printf("Right view\n");
		if (!_FbmVirtualCompareCRC_Progressive(ucCrcIndexR, FALSE, FALSE, pcDir))
		{
			return FALSE;
		}
		u4Index++;
	}
	u4Index = 0;
	_FbmVirtualCreateFbg(FALSE, PICTURE_SBS, MPEG_FRAME_RATE_30);
	_u4DelayTime = 500;
	while (u4Index < u4Num)
	{
		_FbmVirtualLoadBitstream(PICTURE_SBS, u4Index, pcDir);
		_FbmVirtualLoadCRC(PICTURE_SBS, u4Index, pcDir);
		VDP_HalDoB2RAutoTestCRCReadSetting(H_OFFSET, 0);
		UCHAR ucCrcIndexL = ucCrcIndex;
		Printf("Left view\n");
		if (!_FbmVirtualCompareCRC_Interlace(ucCrcIndexL, FALSE, FALSE, pcDir))
		{
			return FALSE;
		}
		VDP_HalDoB2RAutoTestCRCReadSetting(H_OFFSET, _rB2rAutoTestFbgCfg.active_width / 2);
		UCHAR ucCrcIndexR = ucCrcIndex;
		ucCrcIndexR += (1 << 4);
		Printf("Right view\n");
		if (!_FbmVirtualCompareCRC_Interlace(ucCrcIndexR, FALSE, FALSE, pcDir))
		{
			return FALSE;
		}
		u4Index++;
	}
	VDP_HalDoB2RAutoTestCRCReadSetting(H_OFFSET, 0);
	return TRUE;
}

static BOOL _FbmVirtualCompareCRC_TAB(CHAR* pcDir, UINT32 u4Num)
{
	UCHAR ucCrcIndex = 0;
	UINT32 u4Index = 0; 
	
	_FbmVirtualCreateFbg(TRUE, PICTURE_TAB, MPEG_FRAME_RATE_30);
	while (u4Index < u4Num)
	{
		_FbmVirtualLoadBitstream(PICTURE_TAB, u4Index, pcDir);
		_FbmVirtualLoadCRC(PICTURE_TAB, u4Index, pcDir);
		VDP_HalDoB2RAutoTestCRCReadSetting(V_OFFSET, 0);
		UCHAR ucCrcIndexT = ucCrcIndex;
		Printf("Top view\n");
		if (!_FbmVirtualCompareCRC_Progressive(ucCrcIndexT, FALSE, FALSE, pcDir))
		{
			return FALSE;
		}
		VDP_HalDoB2RAutoTestCRCReadSetting(V_OFFSET, _rB2rAutoTestFbgCfg.active_height / 2);
		UCHAR ucCrcIndexB = ucCrcIndex;
		ucCrcIndexB += (1 << 4);
		Printf("Bottom view\n");
		if (!_FbmVirtualCompareCRC_Progressive(ucCrcIndexB, FALSE, FALSE, pcDir))
		{
			return FALSE;
		}
		u4Index++;
	}
	u4Index = 0;
	_FbmVirtualCreateFbg(FALSE, PICTURE_TAB, MPEG_FRAME_RATE_30);
	_u4DelayTime = 500;
	while (u4Index < u4Num)
	{
		_FbmVirtualLoadBitstream(PICTURE_TAB, u4Index, pcDir);
		_FbmVirtualLoadCRC(PICTURE_TAB, u4Index, pcDir);
		VDP_HalDoB2RAutoTestCRCReadSetting(V_OFFSET, 0);
		UCHAR ucCrcIndexT = ucCrcIndex;
		Printf("Top view\n");
		if (!_FbmVirtualCompareCRC_Interlace(ucCrcIndexT, FALSE, FALSE, pcDir))
		{
			return FALSE;
		}
		VDP_HalDoB2RAutoTestCRCReadSetting(V_OFFSET, _rB2rAutoTestFbgCfg.active_height / 2);
		UCHAR ucCrcIndexB = ucCrcIndex;
		ucCrcIndexB += (1 << 4);
		Printf("Bottom view\n");
		if (!_FbmVirtualCompareCRC_Interlace(ucCrcIndexB, FALSE, FALSE, pcDir))
		{
			return FALSE;
		}
		u4Index++;
	}
	VDP_HalDoB2RAutoTestCRCReadSetting(V_OFFSET, 0);
	return TRUE;
}

static BOOL _FbmVirtualCompareCRC_TwoAddr(CHAR * pcDir, UINT32 u4Num)
{
	UINT32 u4CrcIndex = 0;
	UINT32 u4Index = 0; 
	
	_FbmVirtualCreateFbg(TRUE, PICTURE_TWO_ADDR, MPEG_FRAME_RATE_30);
	VDP_HalDoB2RAutoTestCRCReadSetting(TWO_ADDR, 1);
	while (u4Index < u4Num - 1)
	{
		_FbmVirtualLoadBitstream(PICTURE_TWO_ADDR, u4Index, pcDir);
		_FbmVirtualLoadCRC(PICTURE_TWO_ADDR, u4Index, pcDir);
		if (!_FbmVirtualCompareCRC_Progressive(u4CrcIndex, TRUE, TRUE, pcDir))
		{
			return FALSE;
		}
		u4Index++;
	}
	u4Index = 0;
	_FbmVirtualCreateFbg(FALSE, PICTURE_TWO_ADDR, MPEG_FRAME_RATE_30);
	_u4DelayTime = 500;
	while (u4Index < u4Num - 1)
	{
		_FbmVirtualLoadBitstream(PICTURE_TWO_ADDR, u4Index, pcDir);
		_FbmVirtualLoadCRC(PICTURE_TWO_ADDR, u4Index, pcDir);
		if (!_FbmVirtualCompareCRC_Interlace(u4CrcIndex, TRUE, TRUE, pcDir))
		{
			return FALSE;
		}
		u4Index++;
	}
	return TRUE;
}

static UINT32 _FbmVirtualGetCRC(UCHAR ucType)
{
	UINT32 u4Ret = 0xFFFFFFFF;
	VERIFY(x_sema_lock(_hMutex_B2rAutoTest, X_SEMA_OPTION_WAIT) == OSR_OK);
	switch (ucType)
	{
	case CRC_INTERLACE_TOP:
		u4Ret = _u4CRC_INTERLACE_TOP;
		break;
	case CRC_INTERLACE_BOTTOM:
		u4Ret = _u4CRC_INTERLACE_BOTTOM;
		break;
	default:
		break;
	}
	VERIFY(x_sema_unlock(_hMutex_B2rAutoTest) == OSR_OK);
	return u4Ret;
}

#ifdef MAIN_SUB_OPEN
static UINT32 _FbmVirtualGetCRC_Sub(UCHAR ucType)
{
	UINT32 u4Ret = 0xFFFFFFFF;
	VERIFY(x_sema_lock(_hMutex_B2rAutoTest, X_SEMA_OPTION_WAIT) == OSR_OK);
	switch (ucType)
	{
	case CRC_INTERLACE_TOP:
		u4Ret = _u4CRC_INTERLACE_TOP_Sub;
		break;
	case CRC_INTERLACE_BOTTOM:
		u4Ret = _u4CRC_INTERLACE_BOTTOM_Sub;
		break;
	default:
		break;
	}
	VERIFY(x_sema_unlock(_hMutex_B2rAutoTest) == OSR_OK);
	return u4Ret;
}

static UINT32 _FbmVirtualGetCRC_Main(UCHAR ucType)
{
	UINT32 u4Ret = 0xFFFFFFFF;
	VERIFY(x_sema_lock(_hMutex_B2rAutoTest, X_SEMA_OPTION_WAIT) == OSR_OK);
	switch (ucType)
	{
	case CRC_INTERLACE_TOP:
		u4Ret = _u4CRC_INTERLACE_TOP_Main;
		break;
	case CRC_INTERLACE_BOTTOM:
		u4Ret = _u4CRC_INTERLACE_BOTTOM_Main;
		break;
	default:
		break;
	}
	VERIFY(x_sema_unlock(_hMutex_B2rAutoTest) == OSR_OK);
	return u4Ret;
}

#endif
static BOOL _FbmVirtualSelectFbgConfig(UINT32 u4Pitch, UINT32 u4Width, UINT32 u4Height)
{
	if (u4Pitch <= 768 && u4Width <= 768 && u4Height <= 576)
	{
		_rB2rAutoTestFbgCfg.width = 720;
		_rB2rAutoTestFbgCfg.height = 480;
		_rB2rAutoTestFbgCfg.active_width = u4Width;
		_rB2rAutoTestFbgCfg.active_height = u4Height;
		_rB2rAutoTestFbgCfg.pitch = u4Pitch;
		_rB2rAutoTestFbgCfg.frame_size = FBM_FBG_TYPE_PAL_Y_SIZE;	
	}
	else if (u4Pitch <= 1920 && u4Width <= 1920 && u4Height <= 1088)
	{

		_rB2rAutoTestFbgCfg.width = 1920;
		_rB2rAutoTestFbgCfg.height = 1080;
		_rB2rAutoTestFbgCfg.active_width = u4Width;
		_rB2rAutoTestFbgCfg.active_height = u4Height;
		_rB2rAutoTestFbgCfg.pitch = u4Pitch;
		_rB2rAutoTestFbgCfg.frame_size = FBM_FBG_TYPE_1080HD_Y_SIZE;

	}
	else if (u4Pitch <= 3840 && u4Width <= 3840 && u4Height <= 2160)
	{
		_rB2rAutoTestFbgCfg.width = 1920;
		_rB2rAutoTestFbgCfg.height = 1080;
		_rB2rAutoTestFbgCfg.active_width = u4Width;
		_rB2rAutoTestFbgCfg.active_height = u4Height;
		_rB2rAutoTestFbgCfg.pitch = u4Pitch;
		_rB2rAutoTestFbgCfg.frame_size = FBM_FBG_TYPE_SBSHD_Y_SIZE;
	}
	else 
	{
		return FALSE;
	}
	return TRUE;
}

static void _FbmVirtualCreateFbg(BOOL fgProgressive, UCHAR ucPicType, UCHAR ucFrameRateIndex)
{
	FBM_ReleaseGroup(_ucFbgId);
	VDP_HalDoB2RAutoTestCRCReadSetting(CLEAR_PICTURE_PITCH, 0);

	UCHAR ucFbgIdx;
	UCHAR ucFbId;
	
	_ucFbgId = FBM_CreateGroup(FBM_FBG_TYPE_PAL, FBM_VDEC_MPEG2, _rB2rAutoTestFbgCfg.width, _rB2rAutoTestFbgCfg.height);
	ASSERT(_ucFbgId != FBM_FBG_ID_UNKNOWN);
	
	FBM_FbgChgNotify(_ucFbgId, 0);
	
	FBM_SetSyncStc(_ucFbgId,VID_SYNC_MODE_NONE,STC_SRC_A1);
	
	_arFbg[_ucFbgId].u4FbgFlag = (FBM_FLAG_SEQ_CHG | FBM_FLAG_DISP_READY);
	_arFbg[_ucFbgId].rSeqHdr.u2LineSize = _rB2rAutoTestFbgCfg.pitch;
	_arFbg[_ucFbgId].rSeqHdr.u2HSize = _rB2rAutoTestFbgCfg.active_width;
	if (ucPicType == PICTURE_SBS)
	{
		_arFbg[_ucFbgId].rSeqHdr.u2HSize = _rB2rAutoTestFbgCfg.active_width / 2;
	}
	_arFbg[_ucFbgId].rSeqHdr.u2VSize = _rB2rAutoTestFbgCfg.active_height;
	if (ucPicType == PICTURE_TAB)
	{
		_arFbg[_ucFbgId].rSeqHdr.u2VSize = _rB2rAutoTestFbgCfg.active_height / 2;
	}
	_arFbg[_ucFbgId].rSeqHdr.ucAspRatInf = 3;		 
	_arFbg[_ucFbgId].rSeqHdr.ucFrmRatCod = ucFrameRateIndex;		
	_arFbg[_ucFbgId].rSeqHdr.fgProgressiveSeq = fgProgressive;   
	_arFbg[_ucFbgId].rSeqHdr.fg422Mode = _u4PicDimension;
	if (_u4AddrMode == ADDR_MODE_64_32)
	{
		_arFbg[_ucFbgId].rSeqHdr.ucBlockType = 0;
	}
	else if (_u4AddrMode == ADDR_MODE_16_32)
	{
		_arFbg[_ucFbgId].rSeqHdr.ucBlockType = 1;
	}
	else 
	{
		_arFbg[_ucFbgId].rSeqHdr.fgRasterOrder = TRUE;
	}
	
	for (ucFbId = 0; ucFbId < _arFbg[_ucFbgId].ucFbNs; ucFbId++)
	{
		x_memset((void *) &(_arFbg[_ucFbgId].prPicHdr[ucFbId]), 0, sizeof(FBM_PIC_HDR_T));

		_arFbg[_ucFbgId].prPicHdr[ucFbId].fgTopFldFirst = 1;
		_arFbg[_ucFbgId].prPicHdr[ucFbId].fgRepFirstFld = 0;
		_arFbg[_ucFbgId].prPicHdr[ucFbId].fgProgressiveFrm = fgProgressive;

		if (ucFbId == 0)
		{
			FBM_SetFrameBufferZebraColor(ucFbgIdx, ucFbId);
		}
		else
		{
			FBM_SetFrameBufferColor(ucFbgIdx, ucFbId, 0x00EB00);
		}
	}
}

	//-------------------------------------------------------------------------
/** _FbmVirutualMpegRoutine
 *  VM Thread
 */
//-------------------------------------------------------------------------
static void _FbmVirutualMpegRoutine(void* pvArg)
{ 
	BOOL fgSuccess = TRUE;
    while (_ucFbmVirtualMpegDestroy == 0)
    {
    	if (_i4B2rVerifyStatus == NOT_START)
    	{
    		continue;
    	}
    	if (!_prCurB2rAutoTestDir)
    	{
    		_i4B2rVerifyStatus = FINISH;
    	}
		if (_i4B2rVerifyStatus == START || _i4B2rVerifyStatus == DOING)
		{
			if (_i4B2rVerifyStatus == START)
			{
				Printf("Start...\n");
				_i4B2rVerifyStatus = DOING;
			}
			BOOL fgRet = TRUE;
			CHAR* pcDir = (CHAR*)x_mem_alloc(128 * sizeof(CHAR));
			x_memcpy(pcDir, _prCurB2rAutoTestDir->dir, x_strlen(_prCurB2rAutoTestDir->dir) + 1);
			CHAR* pcPicParamStr = (CHAR*)x_mem_alloc(128 * sizeof(CHAR));	
			x_memset(pcPicParamStr, 0xFF, 128 * sizeof(CHAR));
			Printf("d.load.binary \"%s\\bitstream.ini\" 0x%x /ny\n", pcDir, pcPicParamStr);
			UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\bitstream.ini\" 0x%x /ny", pcDir, pcPicParamStr));
			FBM_B2R_AUTO_TEST_PIC_PARAM_T* prPicParam = (FBM_B2R_AUTO_TEST_PIC_PARAM_T*)x_mem_alloc(sizeof(FBM_B2R_AUTO_TEST_PIC_PARAM_T));
			_FbmVirtualGetPictureParam(pcPicParamStr, prPicParam);
			x_mem_free(pcPicParamStr);
			_FbmVirtualSelectFbgConfig(prPicParam->pitch, prPicParam->width, prPicParam->height);
			UINT32 u4Num = prPicParam->picture_number;
			_u4AddrMode = prPicParam->addr_mode;
			_u4PicDimension = prPicParam->dimension;
			VDP_HalDoB2RAutoTestCRCReadSetting(CRC_READING_ENABLE, 1);
			UINT32 u4Quotient = prPicParam->picture_type;
			UINT32 u4Remainder;
			UINT32 u4ShiftBits = 0;
			_u4DelayTime = 500;
			while (u4Quotient)
			{
				u4Remainder = u4Quotient % 2;
				u4Quotient /= 2;
				if (u4Remainder)
				{
					switch (u4ShiftBits)
					{
					case PICTURE_2D:
						Printf("%s -- 2D\n", pcDir);
						fgRet = _FbmVirtualCompareCRC_2D(pcDir, u4Num);
						break;
					case PICTURE_SBS:
						Printf("%s -- SBS\n", pcDir);
						fgRet = _FbmVirtualCompareCRC_SBS(pcDir, u4Num);
						break;
					case PICTURE_TAB:
						Printf("%s -- TAB\n", pcDir);
						fgRet = _FbmVirtualCompareCRC_TAB(pcDir, u4Num);
						break;
					case PICTURE_TWO_ADDR:
						Printf("%s -- Two_addr\n", pcDir);
						fgRet = _FbmVirtualCompareCRC_TwoAddr(pcDir, u4Num);
						break;
					case PICTURE_PIC:
						Printf("%s -- Picture\n", pcDir);
						fgRet = _FbmVirtualCompareCRC_Picture(pcDir, u4Num);
						break;
					default:
						break;
					}
				}
				if (!fgRet)
				{
					break;
				}
				u4ShiftBits++;
			}		
			x_mem_free(pcDir);
			x_mem_free(prPicParam);
			if (!fgRet)
			{	
				_i4B2rVerifyStatus = FINISH;
				fgSuccess = FALSE;
			}
			_prCurB2rAutoTestDir = _prCurB2rAutoTestDir->next;
			if (!_prCurB2rAutoTestDir)
			{
				_i4B2rVerifyStatus = FINISH;
			}
		}
		if (_i4B2rVerifyStatus == FINISH)
		{
			if (fgSuccess)
			{
				Printf("Success");
			}
			else 
			{
				Printf("Failed");
			}
			break;
		}
 	 }

    FBM_ReleaseGroup(_ucFbgId);
	_FbmVirtualDistroyDirList();
    _ucFbmVirtualMpegStart = 0;
    x_thread_exit();
}

void B2R_AutoTestSetCRC(UCHAR ucType, UINT32 u4Value)
{
	if (!_ucFbmVirtualMpegStart)
	{
		return;
	}
	VERIFY(x_sema_lock(_hMutex_B2rAutoTest, X_SEMA_OPTION_WAIT) == OSR_OK);
	//Printf("Set CRC, u4Value = %x\n", u4Value);
	switch (ucType)
	{
	case CRC_INTERLACE_TOP:
		_u4CRC_INTERLACE_TOP = u4Value;
		break;
	case CRC_INTERLACE_BOTTOM:
		_u4CRC_INTERLACE_BOTTOM = u4Value;
		break;
	default:
		break;
	}
	VERIFY(x_sema_unlock(_hMutex_B2rAutoTest) == OSR_OK);
}

#ifdef MAIN_SUB_OPEN
void B2R_AutoTestSetCRCSub(UCHAR ucType, UINT32 u4Value)
{

	if (!_ucFbmVirtualMpegStart)
	{
		return;
	}

	VERIFY(x_sema_lock(_hMutex_B2rAutoTest, X_SEMA_OPTION_WAIT) == OSR_OK);
	Printf("Set Sub CRC, u4Value = %x\n", u4Value);
	switch (ucType)
	{
	case CRC_INTERLACE_TOP:
		_u4CRC_INTERLACE_TOP_Sub= u4Value;
		break;
	case CRC_INTERLACE_BOTTOM:
		_u4CRC_INTERLACE_BOTTOM_Sub = u4Value;
		break;
	default:
		break;
	}
	VERIFY(x_sema_unlock(_hMutex_B2rAutoTest) == OSR_OK);
}

void B2R_AutoTestSetCRCMain(UCHAR ucType, UINT32 u4Value)
{

	if (!_ucFbmVirtualMpegStart)
	{
		return;
	}

	
	VERIFY(x_sema_lock(_hMutex_B2rAutoTest, X_SEMA_OPTION_WAIT) == OSR_OK);
	Printf("Set Main CRC, u4Value = %x\n", u4Value);
	switch (ucType)
	{
	case CRC_INTERLACE_TOP:
		_u4CRC_INTERLACE_TOP_Main = u4Value;
		break;
	case CRC_INTERLACE_BOTTOM:
		_u4CRC_INTERLACE_BOTTOM_Main = u4Value;
		break;
	default:
		break;
	}
	VERIFY(x_sema_unlock(_hMutex_B2rAutoTest) == OSR_OK);
}

#endif

#endif



#ifdef FBM_VIRTUAL_DISPLAY
static void _FbmVirutualDispRoutine(void* pvArg)
{
    UCHAR ucFbgIdx;
    UCHAR aucFbLock[FBG_MAX_NS];
    UCHAR aucDecodeStart[FBG_MAX_NS];

    UINT32 u4DisplayCount = 0;
    UINT32 u4EmptyCount = 0;
    UINT32 u4VirtualDispPath = 0;

    if(pvArg == NULL)
    {
        ASSERT(!pvArg);
        LOG(1, "_FbmVirutualDispRoutine(): (pvArg == NULL)\n");
    }
    else
    {
        u4VirtualDispPath = *(UINT32*)pvArg;
    }

    for (ucFbgIdx = 0; ucFbgIdx < FBG_MAX_NS; ucFbgIdx++)
    {
        aucFbLock[ucFbgIdx] = FBM_FB_ID_UNKNOWN;
        aucDecodeStart[ucFbgIdx] = 0;
    }

    while (_ucFbmVirtualDispDestroy == 0)
    {
        for (ucFbgIdx = 0; ucFbgIdx < FBG_MAX_NS; ucFbgIdx++)
        {
            if(u4VirtualDispPath)
            {
                if(!((_arFbg[ucFbgIdx].u1DecoderSrcId + 1) == u4VirtualDispPath))
                {
                    continue;
                }
            }
            if (!VERIFY_FBG(ucFbgIdx))
            {
                UCHAR ucFbId;

                ucFbId = FBM_GetFrameBufferFromDispQ(ucFbgIdx);

                if (ucFbId != FBM_FB_ID_UNKNOWN)
                {
                    if (aucFbLock[ucFbgIdx] != FBM_FB_ID_UNKNOWN)
                    {
                        VERIFY(FBM_CheckFrameBufferStatus(ucFbgIdx, aucFbLock[ucFbgIdx], FBM_FB_STATUS_LOCK));

                        FBM_SetFrameBufferStatus(ucFbgIdx, aucFbLock[ucFbgIdx], FBM_FB_STATUS_EMPTY);

                        aucFbLock[ucFbgIdx] = FBM_FB_ID_UNKNOWN;
                    }

                    FBM_SetFrameBufferStatus(ucFbgIdx, ucFbId, FBM_FB_STATUS_LOCK);

                    aucFbLock[ucFbgIdx] = ucFbId;

                    aucDecodeStart[ucFbgIdx] = 1;

                    u4DisplayCount++;
                }
                else
                {
                    if (aucDecodeStart[ucFbgIdx])
                    {
                        LOG(7, "Fbg(%d) DispQ Empty\n", ucFbgIdx);

                        u4EmptyCount++;
                    }
                }
            }
        }

        // delay
        x_thread_delay(_ucFbmVirtualDispDelay);
    }

    _ucFbmVirtualDispStart = 0;

    // destroy thread
    x_thread_exit();
}

#endif 

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** FBM_VirtualDispStart
 *  Start VP
 */
//-------------------------------------------------------------------------
#ifdef FBM_VIRTUAL_MPEG
//-------------------------------------------------------------------------
/** FBM_VirtualMpegStart
 *  Start VM
 */
//-------------------------------------------------------------------------
void FBM_VirtualMpegStart(UINT32 u4Delay, const CHAR* pcFile)
{
    if (_ucFbmVirtualMpegStart == 0)
    { 
		CHAR*  pcDirList = (CHAR*)x_mem_alloc(256 * sizeof(CHAR));
		x_memset(pcDirList, 0xFF, 256 * sizeof(CHAR));
		Printf("d.load.binary \"%s\" 0x%x /ny\n", pcFile, (UINT32)pcDirList);
		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\" 0x%x /ny", pcFile, (UINT32)pcDirList));
		HalFlushInvalidateDCache();
		_FbmVirtualBuildDirList(pcDirList);
		x_mem_free(pcDirList);

		_FbmVirtualRunCli();
		
		HANDLE_T hThread1;

        _ucFbmVirtualMpegDestroy = 0;
        _ucFbmVirtualMpegStart = 1;

        _ucFbmVirtualMpegDelay = u4Delay;

		VERIFY(x_sema_create(&_hMutex_B2rAutoTest, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

        // create thread

		VERIFY(x_thread_create(&hThread1, "VM Thread", FBM_VM_STACK_SIZE, FBM_VM_THREAD_PRIORITY,
                               _FbmVirutualMpegRoutine, 0, NULL) == OSR_OK);
    }
}

//-------------------------------------------------------------------------
/** FBM_VirtualMpegStop
 *  Stop VM
 */
//-------------------------------------------------------------------------
void FBM_VirtualMpegStop(void)
{
    if (_ucFbmVirtualMpegStart == 1)
    {
        _ucFbmVirtualMpegDestroy = 1;
    }
}

#endif

#ifdef FBM_VIRTUAL_DISPLAY
void FBM_VirtualDispStart(UINT32 u4Delay, UINT32 u4VirtualDispPath)
{
    if (_ucFbmVirtualDispStart == 0)
    {
        HANDLE_T hThread1;

        _ucFbmVirtualDispDestroy = 0;
        _ucFbmVirtualDispStart = 1;

        _ucFbmVirtualDispDelay = u4Delay;

        // create thread
        VERIFY(x_thread_create(&hThread1, "VD Thread", FBM_VD_STACK_SIZE, FBM_VD_THREAD_PRIORITY,
                               _FbmVirutualDispRoutine, sizeof(UINT32), (VOID*)&u4VirtualDispPath) == OSR_OK);
    }
}

//-------------------------------------------------------------------------
/** FBM_VirtualDispStop
 *  Stop VP
 */
//-------------------------------------------------------------------------
void FBM_VirtualDispStop(void)
{
    if (_ucFbmVirtualDispStart == 1)
    {
        _ucFbmVirtualDispDestroy = 1;
    }
}



#endif
#endif


