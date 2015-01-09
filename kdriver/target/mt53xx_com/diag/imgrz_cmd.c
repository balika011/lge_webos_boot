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
 * $RCSfile: imgrz_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_cmd.c
 *  This file contains implementation of CLI CMD for IMGRZ
 *  
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/*lint -save -e960 -e961 */
#include "x_printf.h"
#include "x_stl_lib.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#define DEFINE_IS_LOG   IMGRZ_IsLog       // for LOG use
#include "x_debug.h"

#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "drvcust_if.h"
#include "gfx_if.h"
/*lint -restore */

/*lint -save -e534 -e715 -e786 -e818 -e826 -e830 -e950 -e957 */
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//#define IMGRZ_CLI_DEBUG
//---------------------------------------------------------------------------

typedef struct _IMGRZCMDTEXT2ENUM_T
{
    CHAR *szText;
    INT32 i4Value;
} IMGRZCMDTEXT2ENUM_T;


typedef enum _ENUM_CLIIMGRZ_ERR_CODE_T
{
    E_CLIIMGRZ_OK          = 0,
    E_CLIIMGRZ_ERR_GENERAL = 1000,
    E_CLIIMGRZ_ERR_PARAM   = 1001,
    // add here
    E_CLIIMGRZ_UNKNOWN     = 1999
} ENUM_CLIIMGRZ_ERR_CODE_T;

typedef enum _ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T
{
	//YCBCR444
	E_YCBCR4442AYUV8888, //ycbcr444 =>ayuv8888,  0
	E_YCBCR4442YC444, //ycbcr444 =>YC444
	E_YCBCR4442YC422, //YCBCR444 TO YC422
	E_YCBCR4442YC420, //YCBCR444 TO YC420
	
	//YCBCR422
	E_YCBCR4222AYUV8888, //ycbcr422 =>ayuv8888,  4
	E_YCBCR4222YC444, //ycbcr422 =>YC444
	E_YCBCR4222YC422, //YCBCR422 TO YC422
	E_YCBCR4222YC420, //YCBCR422 TO YC420
	
	//YCBCR420
	E_YCBCR4202AYUV8888, //ycbcr420=>ayuv8888, 8
	E_YCBCR4202YC444, //ycbcr420=>YC444
	E_YCBCR4202YC422, //YCBCR420 TO YC422
	E_YCBCR4202YC420, //YCBCR420 TO YC420

	//YC444
	E_YC4442AYUV8888, //yc444 =>ayuv8888,  12
	E_YC4442YC444, //yc444 =>YC444
	E_YC4442YC422, //YC444 TO YC422
	E_YC4442YC420, //Yc444 TO YC420
	
	//YC422
	E_YC4222AYUV8888, //ycbcr422 =>ayuv8888,  16
	E_YC4222YC444, //yc422 =>YC444
	E_YC4222YC422, //YC422 TO YC422
	E_YC4222YC420, //YC422 TO YC420
	
	//YC420
	E_YC4202AYUV8888, //yc420=>ayuv8888,  20
	E_YC4202YC444, //yc420=>YC444
	E_YC4202YC422, //YC420 TO YC422
	E_YC4202YC420, //YC420 TO YC420

	//AYUV8888
	E_AYUV88882AYUV8888, //AYUV8888 TO AYUV8888, 24
	E_AYUV88882ARGB8888,  //AYUV8888 TO ARGB8888
	E_AYUV88882ARGB1555, //AYUV8888 TO ARGB1555
	E_AYUV88882ARGB4444, //AYUV8888 TO ARGB4444
	E_AYUV88882RGB565,   //AYUV8888 TO RGB565
	
	//ARGB
	E_ARGB88882AYUV8888, //ARGB8888 TO AYUV8888, 29
	E_ARGB15552AYUV8888, //ARGB1555 to ayuv8888
	E_ARGB44442AYUV8888, //argb4444 to ayuv 8888
	E_RGB5652AYUV8888, //rgb565 to ayuv8888, 32
	
	E_ARGB88882ARGB8888, 
	E_ARGB15552ARGB8888, 
	E_ARGB44442ARGB8888, 
	E_RGB5652ARGB8888, //36	
	
	E_ARGB88882ARGB1555, 
	E_ARGB15552ARGB1555, 
	E_ARGB44442ARGB1555, 
	E_RGB5652ARGB1555, //40
	
	E_ARGB88882ARGB4444, 
	E_ARGB15552ARGB4444, 
	E_ARGB44442ARGB4444, 
	E_RGB5652ARGB4444, //44	

	E_ARGB88882RGB565, 
	E_ARGB15552RGB565, 
	E_ARGB44442RGB565, 
	E_RGB5652RGB565, //48
	E_CVT_TYPE_MAX	
}ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define IMGRZCMD_CHK_NUM_MIN_PARAM(N)           \
    if (i4Argc < ((N)+1))                       \
    {                                           \
        return -(INT32)E_CLIIMGRZ_ERR_PARAM;    \
    }


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

extern INT32 CLI_Parser(const CHAR* szCmdBuf);

// imgrz_debug.h
extern void IMGRZ_DbgDumpInfo(UINT32 u4Which);

// imgrz_hw.h
extern void IMGRZ_HwSetEngClkSrc(UINT32 u4Which, INT32 i4ClkSrc);

// imgrz_cmdque.h
extern void IMGRZ_CmdQueDbgInfo(UINT32 u4Which);
extern void IMGRZ_CmdQueSetNewConfig(UINT32 u4Which, INT32 i4NewConfig);

// imgrz_dif.h
extern void IMGRZ_DifSetMode(UINT32 u4Which, UINT32 u4Mode);;


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

// set

// JPG Op

// VDO Op

// OSD Op
static INT32 _ImgrzOsdOpCmdSetSrcBuf    (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzOsdOpCmdSetTgBuf     (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzOsdOpCmdScale        (INT32 i4Argc, const CHAR ** szArgv);


#if defined(IMGRZ_ENABLE_SW_MODE)
static INT32 _ImgrzCmdEnCm              (INT32 i4Argc, const CHAR ** szArgv);
#endif

static INT32 _ImgrzCmdInit              (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdReset             (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdFlush             (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdQuery             (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdSetEngClkSrc      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdSetQueConfig      (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdSetCrashDetect    (INT32 i4Argc, const CHAR ** szArgv);
#ifdef __MODEL_slt__
static INT32 _ImgrzCmdSlt               (INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _ImgrzCmdOnOffFlushInvalidate(INT32 i4Argc,const CHAR ** szArgv);

//static INT32 _RzCmdGetEnum(const CHAR *szText, const IMGRZCMDTEXT2ENUM_T* prText2Enum);
#ifdef __MODEL_slt__
static INT32 _ImgrzSlt(UINT32 u4Loop, UINT32 u4SwEn);
#endif

static INT32 _ImgrzCmdYCbCr2AYUV      (ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T eType, INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdYCbCr2YC           (ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T eType, INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdYC2AYUV      (ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T eType, INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdYC2YC           (ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T eType, INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdArgbAyuvIntcov      (ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T eType, INT32 i4Argc, const CHAR ** szArgv);
static INT32 _ImgrzCmdClrCvtTimeConsume(INT32 i4Argc, const CHAR ** szArgv);
#ifdef IMGRZ_CLI_DEBUG
static INT32 _ImgrzDumpData(const INT8 *fileName, const INT8 *buf, UINT32 count);
#endif

static INT32 _ImgrzCmdModeLevel(INT32 i4Argc, const CHAR ** szArgv);

CLIMOD_DEBUG_FUNCTIONS(IMGRZ)     // for LOG use


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

// set table
static CLI_EXEC_T _arImgrzSetCmdTbl[] =
{

//    {
//        "idx2dirsrc", NULL, _ImgrzSetCmdIdx2DirSrc, NULL, 
//        "Set idx2dir source buffer", CLI_SUPERVISOR
//    },

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

// osd operation table
static CLI_EXEC_T _arImgrzOsdOpCmdTbl[] =
{
    {
        "src", NULL, _ImgrzOsdOpCmdSetSrcBuf, NULL, 
        "set source buffer", CLI_SUPERVISOR
    },
    {
        "tg", NULL, _ImgrzOsdOpCmdSetTgBuf, NULL, 
        "set target buffer", CLI_SUPERVISOR
    },
    {
        "scale", NULL, _ImgrzOsdOpCmdScale, NULL, 
        "scale up/down", CLI_SUPERVISOR
    },

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};


// cmd table
static CLI_EXEC_T _arImgrzCmdTbl[] =
{
#if defined(IMGRZ_ENABLE_SW_MODE)
    {
        "encm", NULL, _ImgrzCmdEnCm, NULL, 
        "Enable IMGRZ C-model", CLI_SUPERVISOR
    },
#endif
#ifdef __MODEL_slt__
    {
        "slt", NULL, _ImgrzCmdSlt, NULL, 
        "Imgrz SLT", CLI_SUPERVISOR
    },
#endif
    {
        "init", "i", _ImgrzCmdInit, NULL, 
        "Imgrz init", CLI_SUPERVISOR
    },
    {
        "reset", "res", _ImgrzCmdReset, NULL, 
        "Imgrz reset", CLI_SUPERVISOR
    },
    {
        "flush", "f", _ImgrzCmdFlush, NULL, 
        "Imgrz command queue flush (auto on off)", CLI_SUPERVISOR
    },
    {
        "query", "q", _ImgrzCmdQuery, NULL, 
        "Imgrz Query HW status", CLI_SUPERVISOR
    },
    {
        "clksrc", "clk", _ImgrzCmdSetEngClkSrc, NULL, 
        "Imgrz engine clock source", CLI_SUPERVISOR
    },
    {
        "cmdqcfg", "qcfg", _ImgrzCmdSetQueConfig, NULL, 
        "Imgrz engine cmdque configuration", CLI_SUPERVISOR
    },
    {
        "crashd", NULL, _ImgrzCmdSetCrashDetect, NULL, 
        "Imgrz engine crash detection", CLI_SUPERVISOR
    },
    {
        "set", "s", NULL, _arImgrzSetCmdTbl, 
        "Imgrz parameter set", CLI_SUPERVISOR
    },
    {
        "osdop", NULL, NULL, _arImgrzOsdOpCmdTbl, 
        "Imgrz-OSD operations", CLI_SUPERVISOR
    },    
#ifdef __MODEL_slt__
    {
        "slt", NULL, _ImgrzCmdSlt, NULL,
        "Imgrz-SLT operations", CLI_SUPERVISOR
    },
#endif
    {
        "pft", NULL, _ImgrzCmdClrCvtTimeConsume, NULL,
        "Imgrz-PerformanceTest operations", CLI_SUPERVISOR
    },      
    {
        "enfi", NULL, _ImgrzCmdOnOffFlushInvalidate, NULL,
        "Imgrz-FlushInvalidateDCache on off", CLI_SUPERVISOR
    },    
    {
        "ml", NULL,  _ImgrzCmdModeLevel, NULL,
        "Modelevel", CLI_SUPERVISOR
    },
//    {
//        "vdo", NULL, NULL, _arImgrzVdoOpCmdTbl, 
//        "Imgrz-Video operations", CLI_SUPERVISOR
//    },
//    {
//        "jpg", NULL, NULL, _arImgrzJpgOpCmdTbl, 
//        "Imgrz-JPG operations", CLI_SUPERVISOR
//    },


    CLIMOD_DEBUG_CLIENTRY(IMGRZ),     // for LOG use

    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

#if 0
static IMGRZCMDTEXT2ENUM_T _arImgrzOsdCmEnumTbl[] =
{
    {"cbycry",      (INT32)E_RZ_OSD_DIR_CM_CbYCrY},
    {"ycbycr",      (INT32)E_RZ_OSD_DIR_CM_YCbYCr},
    {"argb8888",    (INT32)E_RZ_OSD_DIR_CM_AYCbCr8888},
    {"aycbcr8888",  (INT32)E_RZ_OSD_DIR_CM_ARGB8888},
    {"argb4444",    (INT32)E_RZ_OSD_DIR_CM_ARGB4444},
    {"argb1555",    (INT32)E_RZ_OSD_DIR_CM_ARGB1555},
    {"rgb565",      (INT32)E_RZ_OSD_DIR_CM_RGB565},
    {"8bpp",        (INT32)E_RZ_OSD_IDX_CM_8BPP},
    {"4bpp",        (INT32)E_RZ_OSD_IDX_CM_4BPP},
    {"2bpp",        (INT32)E_RZ_OSD_IDX_CM_2BPP},

    {NULL,          (INT32)E_RZ_OSD_DIR_CM_ARGB8888}
};
#endif

static INT32 _i4ImgrzFlushMode = 0;

static char _arColCvType[E_CVT_TYPE_MAX][20] = 
{
	  //YCBCR444
	"YCBCR4442AYUV8888", //ycbcr444 =>ayuv8888,  0
	"YCBCR4442YC444   ", //ycbcr444 =>YC444
	"YCBCR4442YC422   ", //YCBCR444 TO YC422
	"YCBCR4442YC420   ", //YCBCR444 TO YC420
	
	//YCBCR422"
	"YCBCR4222AYUV8888", //ycbcr422 =>ayuv8888,  4
	"YCBCR4222YC444   ", //ycbcr422 =>YC444
	"YCBCR4222YC422   ", //YCBCR422 TO YC422
	"YCBCR4222YC420   ", //YCBCR422 TO YC420
	
	//YCBCR420
	"YCBCR4202AYUV8888", //ycbcr420=>ayuv8888, 8
	"YCBCR4202YC444   ", //ycbcr420=>YC444
	"YCBCR4202YC422   ", //YCBCR420 TO YC422
	"YCBCR4202YC420   ", //YCBCR420 TO YC420

	//YC444
	"YC4442AYUV8888   ", //yc444 =>ayuv8888,  12
	"YC4442YC444      ", //yc444 =>YC444
	"YC4442YC422      ", //YC444 TO YC422
	"YC4442YC420      ", //Yc444 TO YC420
	
	//YC422
	"YC4222AYUV8888", //ycbcr422 =>ayuv8888,  16
	"YC4222YC444   ", //yc422 =>YC444
	"YC4222YC422   ", //YC422 TO YC422
	"YC4222YC420   ", //YC422 TO YC420
	
	//YC420
	"YC4202AYUV8888", //yc420=>ayuv8888,  20
	"YC4202YC444   ", //yc420=>YC444
	"YC4202YC422   ", //YC420 TO YC422
	"YC4202YC420   ", //YC420 TO YC420

	//AYUV8888
	"AYUV88882AYUV8888", //AYUV8888 TO AYUV8888, 24
	"AYUV88882ARGB8888",  //AYUV8888 TO ARGB8888
	"AYUV88882ARGB1555", //AYUV8888 TO ARGB1555
	"AYUV88882ARGB4444", //AYUV8888 TO ARGB4444
	"AYUV88882RGB565  ",   //AYUV8888 TO RGB565
	
	//ARGB"
	"ARGB88882AYUV8888", //ARGB8888 TO AYUV8888, 29
	"ARGB15552AYUV8888", //ARGB1555 to ayuv8888
	"ARGB44442AYUV8888", //argb4444 to ayuv 8888
	"RGB5652AYUV8888  ", //rgb565 to ayuv8888, 32
	
	"ARGB88882ARGB8888", 
	"ARGB15552ARGB8888", 
	"ARGB44442ARGB8888", 
	"RGB5652ARGB8888  ", //36	
	
	"ARGB88882ARGB1555", 
	"ARGB15552ARGB1555", 
	"ARGB44442ARGB1555", 
	"RGB5652ARGB1555  ", //40
	
	"ARGB88882ARGB4444", 
	"ARGB15552ARGB4444", 
	"ARGB44442ARGB4444", 
	"RGB5652ARGB4444  ", //44	

	"ARGB88882RGB565  ", 
	"ARGB15552RGB565  ", 
	"ARGB44442RGB565  ", 
	"RGB5652RGB565    " //48
};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#ifdef IMGRZ_CLI_DEBUG
//-------------------------------------------------------------------------
/** _ImgrzDumpData, file, buffer, size
 */
//-------------------------------------------------------------------------
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/param.h>
#include "x_os.h"

static INT32 _ImgrzDumpData(const INT8 *fileName, const INT8 *buf, UINT32 count)
{
	mm_segment_t oldfs;
	struct file *filep;
	size_t ret;

	printk("[DUMP]filename=%s, buf=0x%x,size=0x%x\n", fileName, (UINT32)buf, (UINT32)count);

	oldfs = get_fs();
	set_fs(KERNEL_DS);
	filep = filp_open(fileName, (O_CREAT | O_WRONLY | O_TRUNC), 0);
	if (IS_ERR(filep))
	{
		set_fs(oldfs);
		printk("[DUMP] filp_open error %x!\n", filep);
		return -1;
	}

	ret = filep->f_op->write(filep, (VOID*)buf, count, &filep->f_pos);
	//if (ret != count)
	//{
//		printk("[DUMP] f_op->write error ret=%x!\n", ret);
		//return -1;
	//}
	printk("[DUMP]buf=0x%x,size=0x%x, ret=0x%x\n", (UINT32)buf, (UINT32)count, (UINT32)ret);
	filp_close(filep, 0);
	set_fs(oldfs);
	return 0;
}
#endif

//-------------------------------------------------------------------------
/** _ImgrzOsdOpCmdSetSrcBuf
 *  imgrz.osdop.src 
 *      {base_addr} 
 *      {direct_color_mode_flag}
 *      {color_mode} 
 *      {pitch_in_bytes}
 *      {x_coordinate}
 *      {y_coordinate}
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzOsdOpCmdSetSrcBuf(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 u4Base, u4IsDirCM, u4CM, u4Pitch, u4Sx, u4Sy;
    IMGRZCMD_CHK_NUM_MIN_PARAM(6);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) ||
        (szArgv[3] == NULL) || (szArgv[4] == NULL) || 
        (szArgv[5] == NULL) || (szArgv[6] == NULL))
    {
        return -1;
    }
    
    u4Base    = (UINT32)StrToInt(szArgv[1]);
    u4IsDirCM = (UINT32)StrToInt(szArgv[2]);
    u4CM      = (UINT32)_RzCmdGetEnum(szArgv[3], _arImgrzOsdCmEnumTbl);
    u4Pitch   = (UINT32)StrToInt(szArgv[4]);
    u4Sx      = (UINT32)StrToInt(szArgv[5]);
    u4Sy      = (UINT32)StrToInt(szArgv[6]);
#endif
    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzOsdOpCmdSetTgBuf
 *  imgrz.osdop.tg 
 *     {base_addr} 
 *     {direct_color_mode} 
 *     {pitch_in_bytes}
 *     {x_coordinate}
 *     {y_coordinate}
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzOsdOpCmdSetTgBuf(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 u4Base, u4DirCM, u4Pitch, u4Dx, u4Dy;
    IMGRZCMD_CHK_NUM_MIN_PARAM(5);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || (szArgv[2] == NULL) ||
        (szArgv[3] == NULL) || (szArgv[4] == NULL) || (szArgv[5] == NULL))
    {
        return -1;
    }
    
    u4Base  = (UINT32)StrToInt(szArgv[1]);
    u4DirCM = (UINT32)_RzCmdGetEnum(szArgv[2], _arImgrzOsdCmEnumTbl);
    u4Pitch = (UINT32)StrToInt(szArgv[3]);
    u4Dx    = (UINT32)StrToInt(szArgv[4]);
    u4Dy    = (UINT32)StrToInt(szArgv[5]);
#endif
    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzOsdOpCmdScale
 *  imgrz.osdop.scale 
 *      {src_width} 
 *      {src_height}
 *      {tg_width} 
 *      {tg_height}
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzOsdOpCmdScale(INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    UINT32 u4SrcW, u4SrcH, u4TgW, u4TgH;
    IMGRZCMD_CHK_NUM_MIN_PARAM(4);

    if ((szArgv == NULL) || (szArgv[1] == NULL) || 
        (szArgv[2] == NULL) || (szArgv[3] == NULL) ||
        (szArgv[4] == NULL))
    {
        return -1;
    }

    u4SrcW = (UINT32) StrToInt(szArgv[1]);
    u4SrcH = (UINT32) StrToInt(szArgv[2]);
    u4TgW  = (UINT32) StrToInt(szArgv[3]);
    u4TgH  = (UINT32) StrToInt(szArgv[4]);
#endif
//    IMGRZ_OsdScale(u4SrcW, u4SrcH, u4TgW, u4TgH);
    if (_i4ImgrzFlushMode)
    {
        IMGRZ_Flush();
        IMGRZ_Wait();
    }
    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdInit
 *  imgrz.init
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

	/*
    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    IMGRZ_Init();
	*/
	if (i4Argc == 1)
	{
		IMGRZ_Init();
	}

	if (i4Argc >= 2)
	{
		IMGRZ_Init_Ex((UINT32) StrToInt(szArgv[1]));
	}
	
    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdEnCm
 *  imgrz.encm
 */
//-------------------------------------------------------------------------
#if defined(IMGRZ_ENABLE_SW_MODE)
static INT32 _ImgrzCmdEnCm(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4EnCm = 0;

    if (szArgv == NULL)
    {
        return -1;
    }
    
    if (i4Argc < 2)
    {
        Printf("arg: EnCm (1:on, 0:off)\n");
        return -1;
    }

    u4EnCm = (UINT32) StrToInt(szArgv[1]);
    
    if (u4EnCm)
    {
        _IMGRZ_Reset(E_FIRST_RESIZER);
        IMGRZ_DifSetMode(E_FIRST_RESIZER, (UINT32)E_RZ_SW_MOD);
        Printf("Enable IMGRZ C-model\n");
    }
    else
    {
        _IMGRZ_Reset(E_FIRST_RESIZER);
        IMGRZ_DifSetMode(E_FIRST_RESIZER, (UINT32)E_RZ_HW_MOD);
        Printf("Disable IMGRZ C-model\n");
    }

    return 0;
}
#endif


//-------------------------------------------------------------------------
/** _ImgrzCmdReset
 *  imgrz.reset
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdReset(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

	/*
    UNUSED(i4Argc);
    UNUSED(szArgv);
    */
    
    // Prevent emulation program from 
    // resetting engine before returning interrupt

	if (i4Argc == 1)
	{
	    IMGRZ_Wait();
	    IMGRZ_Reset();
	}
	if (i4Argc >= 2)
	{
		IMGRZ_Wait_Ex((UINT32) StrToInt(szArgv[1]));
		IMGRZ_Reset_Ex((UINT32) StrToInt(szArgv[1]));
	}
	
    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdFlush
 *  imgrz.flush [{auto, delayed|off}]
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdFlush(INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }

    if (i4Argc == 1)
    {
        IMGRZ_Flush();
        IMGRZ_Wait();
		_i4ImgrzFlushMode = 0;
    }

    if (i4Argc >= 2)
    {
		if ((x_strncmp("auto", szArgv[1], 4) == 0) || (x_strncmp("AUTO", szArgv[1], 4) == 0))
	    {
	        _i4ImgrzFlushMode = 1;
	    }
		else if (i4Argc == 2)
		{
			_i4ImgrzFlushMode = 0;
		}
    	else if (i4Argc == 3)
    	{
			IMGRZ_Flush_Ex((UINT32)StrToInt(szArgv[2]));
			IMGRZ_Wait_Ex((UINT32)StrToInt(szArgv[2]));
			_i4ImgrzFlushMode = 0;
    	}
    }
	
    return (INT32)E_CLIIMGRZ_OK;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdQuery
 *  imgrz.query
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdQuery           (INT32 i4Argc, const CHAR ** szArgv)
{
    if (szArgv == NULL)
    {
        return -1;
    }
    
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("current hw status = %s\n", _IMGRZ_QueryHwIdle(E_FIRST_RESIZER) ? "idle" : "busy");

#if defined(IMGRZ_DEBUG_MODE)
    Printf("    << In IMGRZ Debug Mode >>\n");
    Printf("(if imgrz engine status is idle, "    \
        "flush count must be euqal to interrupt count)\n");

    IMGRZ_DbgDumpInfo(E_FIRST_RESIZER);
    IMGRZ_CmdQueDbgInfo(E_FIRST_RESIZER);
#endif // #if defined(IMGRZ_DEBUG_MODE)

    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdSetEngClkSrc
 *  imgrz.clksrc
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdSetEngClkSrc    (INT32 i4Argc, const CHAR ** szArgv)
{
#if defined(IMGRZ_DEBUG_MODE)

    INT32 i4ClkSrc = 0;
    INT32 i4ClkSrcMax = 8;

    if (i4Argc < 2)
    {
        Printf("\narg: clk_src_num\n");
        Printf("   0 : xtal clk\n");
        Printf("   1 : syspll / 2  (162 / 216 MHz)\n");
        Printf("   2 : tvdpll / 3  (180   MHz)\n");
        Printf("   3 : tvdpll / 5  (108   MHz)\n");
        Printf("   4 : dtdpll / 3  (200   MHz)\n");
        Printf("   5 : dtdpll / 4  (150   MHz)\n");
        Printf("   6 : pspll  / 1  (148.5 MHz)\n");
        Printf("   7 : mem\n");
        return -1;
    }
    
    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }
    
    i4ClkSrc  = (INT32) StrToInt(szArgv[1]);

    if (i4ClkSrc >= i4ClkSrcMax)
    {
        Printf("Error: clk src = %d\n", i4ClkSrc);
        return -1;
    }
    
    IMGRZ_HwSetEngClkSrc(E_FIRST_RESIZER, i4ClkSrc);
    Printf("GRA_CLK_ADDR : 0x2000D224\n");
#endif // #if defined(IMGRZ_DEBUG_MODE)

    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdSetQueConfig
 *  imgrz.cmdqcfg
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdSetQueConfig    (INT32 i4Argc, const CHAR ** szArgv)
{
#if defined(IMGRZ_DEBUG_MODE)

    INT32 i4QueCfg = 0;
    INT32 i4QueCfgMax = 8;

    if (i4Argc < 2)
    {
        Printf("\narg: cmdq_cfg\n");
        Printf("   0 : cmdq_002_KB\n");
        Printf("   1 : cmdq_004_KB\n");
        Printf("   2 : cmdq_008_KB\n");
        Printf("   3 : cmdq_016_KB\n");
        Printf("   4 : cmdq_032_KB\n");
        Printf("   5 : cmdq_064_KB\n");
        Printf("   6 : cmdq_128_KB\n");
        Printf("   7 : cmdq_256_KB\n");
        return -1;
    }

    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }
    
    i4QueCfg  = (INT32) StrToInt(szArgv[1]);

    if (i4QueCfg >= i4QueCfgMax)
    {
        Printf("Error: cmdq_cfg = %d\n", i4QueCfg);
        return -1;
    }
    
    IMGRZ_CmdQueSetNewConfig(E_FIRST_RESIZER, i4QueCfg);
#endif // #if defined(IMGRZ_DEBUG_MODE)

    return 0;
}


//-------------------------------------------------------------------------
/** _ImgrzCmdSetCrashDetect
 *  imgrz.crashd
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdSetCrashDetect    (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Value = 0;

    if (i4Argc < 2)
    {
        Printf("\narg:\n");
        Printf("   0 : disable crash detection\n");
        Printf("   1 : enable  crash detection\n");
        return -1;
    }

    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }
    
    u4Value = (UINT32) StrToInt(szArgv[1]);
    
    _IMGRZ_SetCrashDetection(E_FIRST_RESIZER, u4Value);

    Printf("IMGRZ engine crash detection = %s \n", 
        _IMGRZ_GetCrashDetection(E_FIRST_RESIZER) ? "Enable" : "Disable");

    return 0;
}


extern INT32 u4ShowReg;
static INT32 _ImgrzCmdModeLevel(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4ModeLevel = 0;
    BOOL fgOpen = 0;
    u4ModeLevel  = (INT32) StrToInt(szArgv[1]);
    fgOpen  = (BOOL) StrToInt(szArgv[2]);

    Printf("<ImgRz>ModeLevel %d %d %d \n", i4Argc, u4ModeLevel, fgOpen);

    if (i4Argc >= 3 && u4ModeLevel == 1)
	{
        u4ShowReg = (fgOpen == 1) ? TRUE : FALSE;
        Printf("<ImgRz>u4ShowReg %d \n", u4ShowReg);
	}
    return 0;
}
#ifdef __MODEL_slt__

//-------------------------------------------------------------------------
/** _ImgrzCmdSlt
 *  imgrz.slt
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdSlt(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;
    UINT32 u4SwEn = 0;   // c-model enabled
    UINT32 u4Loop = 10;

    if (szArgv == NULL)
    {
        return -1;
    }
    
    if (i4Argc == 2)
    {
        u4Loop = (UINT32) StrToInt(szArgv[1]);
    }

    if (i4Argc == 3)
    {
        u4Loop = (UINT32) StrToInt(szArgv[1]);
        u4SwEn = (UINT32) StrToInt(szArgv[2]);
    }

    i4Ret = _ImgrzSlt(u4Loop, u4SwEn);

    if ((INT32)E_CLIIMGRZ_OK == i4Ret)
    {
        Printf("IMGRZ SLT (%u loops)......... SUCCESS\n", u4Loop);
        return 0;
    }
    else
    {
        Printf("IMGRZ SLT (%u loops)......... FAILED\n", u4Loop);
        return -1;
    }
}
#endif
//-------------------------------------------------------------------------
/** _ImgrzCmdOnOffFlushInvalidate
 *  imgrz.enfi
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdOnOffFlushInvalidate(INT32 i4Argc,const CHAR ** szArgv)
{
    UINT32 u4Value = 0;

    if (i4Argc < 2)
    {
        Printf("\narg:\n");
        Printf("   0 : disable FlushInvalidateDCache\n");
        Printf("   1 : enable  FlushInvalidateDCache\n");
        return -1;
    }

    if ((szArgv == NULL) || (szArgv[1] == NULL))
    {
        return -1;
    }
    
    u4Value = (UINT32) StrToInt(szArgv[1]);

	if(u4Value == 1)
	{
		Printf("IMGRZ On HalFlushInvalidateDCache\n");
		_IMGRZ_SetHalFlushInvalidateDCache(TRUE);			
	}
	else
	{
		Printf("IMGRZ Off HalFlushInvalidateDCache\n");	
		_IMGRZ_SetHalFlushInvalidateDCache(FALSE);
	}
	
    return 0;
}

//-------------------------------------------------------------------------
/** _ImgrzCmdClrCvtTimeConsume
 *  
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdClrCvtTimeConsume(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4Ret = 0, i4Count = 0;
	ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T eType;
#ifdef IMGRZ_CLI_DEBUG	
	INT32 i = 0;
#endif	
    if (i4Argc < 2) 
    {
    	Printf("\n");
    	for(i4Count = 0; i4Count < E_CVT_TYPE_MAX; i4Count++)
    	{
			Printf("%2d : %s \n", i4Count, _arColCvType[i4Count]);				
		}
		return -1;
	}
	
#ifdef IMGRZ_CLI_DEBUG	
	Printf("\ni4Argc %d\n", i4Argc);
	
	for(i = 0; i < i4Argc; i++)
	{
		Printf("szArgv[%d] = 0x%x\n", i, StrToInt(szArgv[i]));
	}
#endif	


	eType = (ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T) StrToInt(szArgv[1]);
	
	switch(eType)
	{		
	//YCBCR2AYUV
	case E_YCBCR4442AYUV8888:
	case E_YCBCR4222AYUV8888:	
	case E_YCBCR4202AYUV8888:		
		i4Ret = _ImgrzCmdYCbCr2AYUV(eType, i4Argc, szArgv);		
		break;
	//ycbcr2yc
	//ycbcr444
	case E_YCBCR4442YC444:
	case E_YCBCR4442YC422:
	case E_YCBCR4442YC420:
	//YCBCR422
	case E_YCBCR4222YC444:
	case E_YCBCR4222YC422:
	case E_YCBCR4222YC420:	
	//YCBCR420
	case E_YCBCR4202YC444:
	case E_YCBCR4202YC422:
	case E_YCBCR4202YC420:
		i4Ret = _ImgrzCmdYCbCr2YC(eType, i4Argc, szArgv);
		break;

	//YC2AYUV
	case E_YC4442AYUV8888:
	case E_YC4222AYUV8888:
	case E_YC4202AYUV8888:
		i4Ret = _ImgrzCmdYC2AYUV(eType, i4Argc, szArgv);		
		break;

	//yc2yc
	//YC444
	case E_YC4442YC444:
	case E_YC4442YC422:
	case E_YC4442YC420:	
	//YC422		
	case E_YC4222YC444:
	case E_YC4222YC422:
	case E_YC4222YC420:	
	//YC420
	case E_YC4202YC444:
	case E_YC4202YC422:
	case E_YC4202YC420:
		i4Ret = _ImgrzCmdYC2YC(eType, i4Argc, szArgv);
		break;
		
	//AYUV8888
	case E_AYUV88882AYUV8888:
	case E_AYUV88882ARGB8888:
	case E_AYUV88882ARGB1555:
	case E_AYUV88882ARGB4444:
	case E_AYUV88882RGB565  :
	//ARGB
	case E_ARGB88882AYUV8888:
	case E_ARGB15552AYUV8888:
	case E_ARGB44442AYUV8888:
	case E_RGB5652AYUV8888  :
	case E_ARGB88882ARGB8888: 
	case E_ARGB15552ARGB8888: 
	case E_ARGB44442ARGB8888: 
	case E_RGB5652ARGB8888  : //36	
	case E_ARGB88882ARGB1555: 
	case E_ARGB15552ARGB1555: 
	case E_ARGB44442ARGB1555: 
	case E_RGB5652ARGB1555  : //40
	case E_ARGB88882ARGB4444: 
	case E_ARGB15552ARGB4444: 
	case E_ARGB44442ARGB4444: 
	case E_RGB5652ARGB4444  : //44	
	case E_ARGB88882RGB565  : 
	case E_ARGB15552RGB565  : 
	case E_ARGB44442RGB565  : 
	case E_RGB5652RGB565    : //48			
	 	i4Ret = _ImgrzCmdArgbAyuvIntcov(eType, i4Argc, szArgv);
	 	break;
	default:
		break;
	}
	return i4Ret;

}
//-------------------------------------------------------------------------
/** _ImgrzCmdYCbCr2AYUV
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdYCbCr2AYUV(ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T eType, INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4SrcYAddr, u4SrcCbAddr, u4SrcCrAddr, u4TgYAddr, u4Which, i;
	UINT32 u4SrcWidth, u4SrcHeight, u4TgWidth, u4TgHeight;
	RZ_JPG_SCL_PARAM_SET_T rJpgParam;
    HAL_TIME_T  t_current,t_start,t_delta;
#ifdef IMGRZ_CLI_DEBUG	
	char fileName[256] = "/mnt/usb/sda1/y.bin";	
#endif
	//init
	u4SrcYAddr = u4SrcCbAddr = u4SrcCrAddr = u4TgYAddr = u4Which = i = 0;
	u4SrcWidth = u4SrcHeight = u4TgWidth = u4TgHeight = 0;

	Printf("\nImgrz color fmt convert: %s\n", _arColCvType[eType]);
	
	if(i4Argc < 11)
	{
		Printf("The argument number is less than 10!!\n");
		Printf("FMT- imgrz.pft e_type imgrz_id y_addr cb_addr cr_addr src_w src_h tg_addr tg_w tg_h\n");
		Printf("Eg . imgrz.pft %d %d 0x384f1000 0x385f1000 0x386f1000 1024 1024 0x3457a000 1024 1024\n", eType, u4Which);		
		return -1;
	}
	else
	{
		//to get src and tg params
		for(i = 0; i < i4Argc; i++)
		{
			if ((szArgv[i] == NULL))
			{
				return -1;
			}
		}		
		u4Which = StrToInt(szArgv[2]);
		u4SrcYAddr = StrToInt(szArgv[3]);
		u4SrcCbAddr = StrToInt(szArgv[4]);
		u4SrcCrAddr = StrToInt(szArgv[5]);
		u4SrcWidth = StrToInt(szArgv[6]);	
		u4SrcHeight = StrToInt(szArgv[7]);
		u4TgYAddr = StrToInt(szArgv[8]);
		u4TgWidth = StrToInt(szArgv[9]);
		u4TgHeight = StrToInt(szArgv[10]);
		
		if(u4Which > (IMGRZ_NUM-1))
		{
			Printf("The imgrz hw number is %d, the max imgrz id is %d!!\n", IMGRZ_NUM, (IMGRZ_NUM - 1));
			return -1;
		}	
		
		if((u4SrcYAddr&0xf) || (u4SrcCbAddr&0xf) || (u4SrcCrAddr&0xf) || (u4TgYAddr&0xf))
		{
			Printf("Make sure that the test address is 16bit align!!\n");
			return -1;
		}

		if((u4TgWidth < 4) || (u4TgHeight < 4) || (u4SrcWidth < 4) || (u4SrcHeight < 4))
		{
			Printf("Make sure that the src/dest width/height is larger than 4!!\n");
			return -1;
		}

		Printf("Params: \nimgrz_id = %d, \ny_addr=0x%x,cb_addr=0x%x,cr_addr=0x%x,src_w=%d,src_h=%d,\ntg_addr=0x%x,tg_w=%d,tg_h=%d\n",
			u4Which, u4SrcYAddr, u4SrcCbAddr, u4SrcCrAddr, u4SrcWidth, u4SrcHeight, u4TgYAddr, u4TgWidth, u4TgHeight);
	}
	
#ifdef IMGRZ_CLI_DEBUG
	//y
	GFX_Memset((UINT8*)(u4SrcYAddr), 1024, 256, 0x40);			
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024 * 256), 1024, 256, 0x80);			
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024 * 512), 1024, 256, 0xc0);			
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024 * 768), 1024, 256, 0xe0);
	//cb
	GFX_Memset((UINT8*)(u4SrcCbAddr), 1024, 256, 0x00);			
	GFX_Memset((UINT8*)(u4SrcCbAddr + 1024 * 256), 1024, 256, 0x22);			
	GFX_Memset((UINT8*)(u4SrcCbAddr + 1024 * 512), 1024, 256, 0x44);			
	GFX_Memset((UINT8*)(u4SrcCbAddr + 1024 * 768), 1024, 256, 0x66);	
	//cr
	GFX_Memset((UINT8*)(u4SrcCrAddr), 1024, 256, 0x00);			
	GFX_Memset((UINT8*)(u4SrcCrAddr + 1024 * 256), 1024, 256, 0x22);			
	GFX_Memset((UINT8*)(u4SrcCrAddr + 1024 * 512), 1024, 256, 0x44);			
	GFX_Memset((UINT8*)(u4SrcCrAddr + 1024 * 768), 1024, 256, 0x66);			
			   
	// clean target buffer
	GFX_Memset((UINT8*)(u4TgYAddr), 1024, 1024, 0xc1);			
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*1024), 1024, 1024, 0xd1);
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*2*1024), 1024, 1024, 0xe1);
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*3*1024), 1024, 1024, 0xf1);
	
	x_thread_delay(400);				
	_ImgrzDumpData("/mnt/usb/sda1/y.bin", (UINT8 *)(u4SrcYAddr), 1024*1024);
	x_thread_delay(400);	
	_ImgrzDumpData("/mnt/usb/sda1/cb.bin", (UINT8 *)(u4SrcCbAddr), 1024*1024);
	x_thread_delay(400);	
	_ImgrzDumpData("/mnt/usb/sda1/cr.bin", (UINT8 *)(u4SrcCrAddr), 1024*1024);
#endif
	

	//x_memset((void *)&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));
	x_memset((void *)&rJpgParam, 0, sizeof(RZ_JPG_SCL_PARAM_SET_T));
	
	//input 
	rJpgParam.u4IsRsIn		= 1;
	rJpgParam.u4YSrcBase1	= (u4SrcYAddr);
	rJpgParam.u4YSrcBase2	= (u4SrcYAddr);
	
	rJpgParam.u4YSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));
	rJpgParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf)); 
	
	rJpgParam.u4YSrcHOffset = 0;
	rJpgParam.u4YSrcVOffset = 0;
	rJpgParam.u4YSrcW		= u4SrcWidth;
	rJpgParam.u4YSrcH		= u4SrcHeight;
	
	rJpgParam.u4CbSrcBase1	= (u4SrcCbAddr);
	rJpgParam.u4CbSrcBase2	= (u4SrcCbAddr);
	
	rJpgParam.u4CbSrcHOffset	= 0;
	rJpgParam.u4CbSrcVOffset	= 0;
	rJpgParam.u4CbSrcW		= u4SrcWidth;
	
	rJpgParam.u4CrSrcBase1	= (u4SrcCrAddr);
	rJpgParam.u4CrSrcBase2	= (u4SrcCrAddr);
	
	rJpgParam.u4CrSrcHOffset	= 0;
	rJpgParam.u4CrSrcVOffset	= 0;
	rJpgParam.u4CrSrcW		= u4SrcWidth;

	rJpgParam.u4IsRsOut 		= 1;  
	rJpgParam.u4OutSwap 		= 0;	
	rJpgParam.u4ColorComp = 7;
	
	switch(eType)
	{
	case E_YCBCR4442AYUV8888:
		rJpgParam.u4CrSrcH		= u4SrcHeight;
		rJpgParam.u4CbSrcH		= u4SrcHeight;	
		break;
		
	case E_YCBCR4222AYUV8888:
		rJpgParam.u4CrSrcH		= u4SrcHeight >> 1;
		rJpgParam.u4CbSrcH		= u4SrcHeight >> 1;				
		break;	
		
	case E_YCBCR4202AYUV8888:
		rJpgParam.u4CrSrcH		= u4SrcHeight >> 2;
		rJpgParam.u4CbSrcH		= u4SrcHeight >> 2;			
		break;	
		
	default:
		return -1;
	}	
	
	rJpgParam.u4YTgBase 	= (u4TgYAddr);
	rJpgParam.u4CTgBase 		= (u4TgYAddr);//for ayuv8888, CTgBase = YTgBase
	
	rJpgParam.u4IsJpgSameaddr	= 0;
	rJpgParam.u4YTgHOffset		= 0;
	rJpgParam.u4YTgVOffset		= 0;
	rJpgParam.u4YTgBufLen		= (u4TgWidth * 4 + 0xf) & (~0xf);
	rJpgParam.u4CTgBufLen		= (u4TgWidth * 4 + 0xf) & (~0xf);
	
	rJpgParam.u4IsVdo2Osd = 1;	   // output in OSD format ?
	rJpgParam.u4YTgCM = E_RZ_OSD_DIR_CM_AYCbCr8888;
	rJpgParam.u4YTgW = u4TgWidth;
	rJpgParam.u4YTgH = u4TgHeight;		
	rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_444;
	rJpgParam.u4CTgW = u4TgWidth;
	rJpgParam.u4CTgH = u4TgHeight;
	rJpgParam.u4IsFstBl = 1;
	rJpgParam.u4IsLstBl = 1;
	rJpgParam.u4JpgVfacY = 1;
	rJpgParam.u4JpgVfacCb = 1;
	rJpgParam.u4JpgVfacCr = 1;		
	rJpgParam.fgBlassign = TRUE;

	//time measure
	HAL_GetTime(&t_start);	
	
	IMGRZ_Lock_Ex(u4Which);
	IMGRZ_ReInit_Ex(u4Which);
	IMGRZ_SetScaleOpt_Ex(u4Which, E_RZ_INOUT_TYPE_JPGMD);
	IMGRZ_Scale_Ex(u4Which, &rJpgParam);
	IMGRZ_Flush_Ex(u4Which);
	IMGRZ_Wait_Ex(u4Which);
	IMGRZ_Unlock_Ex(u4Which);
	
    HAL_GetTime(&t_current);
    HAL_GetDeltaTime(&t_delta,&t_start,&t_current);     
    printf("\nResult[time = %u MS]\n", (t_delta.u4Seconds*1000 + t_delta.u4Micros/1000));
		
#ifdef IMGRZ_CLI_DEBUG	
	x_thread_delay(400);	
	sprintf(fileName, "/mnt/usb/sda1/d%d.bin", eType);		
	_ImgrzDumpData(fileName, (UINT8 *)(u4TgYAddr), 1024*1024*4);
#endif
	return 0;
}

//-------------------------------------------------------------------------
/** _ImgrzCmdYCbCr2YC
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdYCbCr2YC(ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T eType, INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4SrcYAddr, u4SrcCbAddr, u4SrcCrAddr, u4TgYAddr, u4TgCAddr, u4Which, i;
	UINT32 u4SrcWidth, u4SrcHeight, u4TgWidth, u4TgHeight, u4RsOut;
	RZ_JPG_SCL_PARAM_SET_T rJpgParam;
    HAL_TIME_T  t_current,t_start,t_delta;
#ifdef IMGRZ_CLI_DEBUG	
	char fileName[256] = "/mnt/usb/sda1/y.bin";	
#endif	
	//init
	u4SrcYAddr = u4SrcCbAddr = u4SrcCrAddr = u4TgYAddr = u4TgCAddr = u4Which = i = 0;
	u4SrcWidth = u4SrcHeight = u4TgWidth = u4TgHeight = u4RsOut = 0;
	//eType = eType;
	Printf("\nImgrz color fmt convert: %s\n", _arColCvType[eType]);

	if(i4Argc < 13)
	{
		Printf("The argument number is less than 13!!\n");
		Printf("FMT- imgrz.pft e_type imgrz_id y_addr cb_addr cr_addr src_w src_h tg_yaddr tg_caddr tg_w tg_h raster_out\n");
		Printf("Eg . imgrz.pft %d %d 0x384f1000 0x385f1000 0x386f1000 1024 1024 0x3457a000 0x3467a000 1024 1024 1\n", eType, u4Which);		
		return -1;
	}
	else
	{
		for(i = 0; i < i4Argc; i++)
		{
			if ((szArgv[i] == NULL))
			{
				return -1;
			}
		}	
		//to get src and tg params
		u4Which = StrToInt(szArgv[2]);
		u4SrcYAddr = StrToInt(szArgv[3]);
		u4SrcCbAddr = StrToInt(szArgv[4]);
		u4SrcCrAddr = StrToInt(szArgv[5]);
		u4SrcWidth = StrToInt(szArgv[6]);	
		u4SrcHeight = StrToInt(szArgv[7]);
		u4TgYAddr = StrToInt(szArgv[8]);
		u4TgCAddr = StrToInt(szArgv[9]);		
		u4TgWidth = StrToInt(szArgv[10]);
		u4TgHeight = StrToInt(szArgv[11]);
		u4RsOut = StrToInt(szArgv[12]);

		if(u4Which > (IMGRZ_NUM-1))
		{
			Printf("The imgrz hw number is %d, the max imgrz id is %d!!\n", IMGRZ_NUM, (IMGRZ_NUM - 1));
			return -1;
		}
		
		if((u4SrcYAddr&0xf) || (u4SrcCbAddr&0xf) || (u4SrcCrAddr&0xf) || (u4TgYAddr&0xf) || (u4TgCAddr&0xf))
		{
			Printf("Make sure that the test address is 16bit align!!\n");
			return -1;
		}

		if((u4TgWidth < 4) || (u4TgHeight < 4) || (u4SrcWidth < 4) || (u4SrcHeight < 4))
		{
			Printf("Make sure that the src/dest width/height is larger than 4!!\n");
			return -1;
		}

		if(u4TgWidth&0x1)
		{
			Printf("For video format output, make sure that the dest width is even!!\n");
			return -1;
		}
		Printf("\nParams: \nimgrz_id = %d,\ny_addr=0x%x,cb_addr=0x%x,cr_addr=0x%x,src_w=%d,src_h=%d,\ntg_yaddr=0x%x, tg_caddr=0x%x, tg_w=%d,tg_h=%d\nu4RsOut=%d\n",
			u4Which, u4SrcYAddr, u4SrcCbAddr, u4SrcCrAddr, u4SrcWidth, u4SrcHeight, u4TgYAddr, u4TgCAddr, u4TgWidth, u4TgHeight, u4RsOut);
	}
#ifdef IMGRZ_CLI_DEBUG
	//y
	GFX_Memset((UINT8*)(u4SrcYAddr), 1024, 1024, 0x43);
	//cb
	GFX_Memset((UINT8*)(u4SrcCbAddr), 1024, 256, 0x62);
	//cr
	GFX_Memset((UINT8*)(u4SrcCrAddr), 1024, 256, 0xd8);		
			   
	// clean target buffer
	GFX_Memset((UINT8*)(u4TgYAddr), 1024, 1024, 0x00);
	GFX_Memset((UINT8*)(u4TgCAddr), 1024, 1024, 0x00);

	x_thread_delay(400);
	sprintf(fileName, "/storage/sdcard/y.bin");		
	_ImgrzDumpData(fileName, (UINT8 *)(u4SrcYAddr), 1024*1024);
	x_thread_delay(400);
	sprintf(fileName, "/storage/sdcard/cb.bin");			
	_ImgrzDumpData(fileName, (UINT8 *)(u4SrcCbAddr), 1024*1024);
	x_thread_delay(400);
	sprintf(fileName, "/storage/sdcard/cr.bin");		
	_ImgrzDumpData(fileName, (UINT8 *)(u4SrcCrAddr), 1024*1024);
										
#endif
	x_memset((void *)&rJpgParam, 0, sizeof(RZ_JPG_SCL_PARAM_SET_T));

	switch(eType)
	{
	case E_YCBCR4442YC444:
		rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_444; //420, 422, 444	
		rJpgParam.u4CbSrcH		= u4SrcHeight;
		rJpgParam.u4CrSrcH		= u4SrcHeight;		
		rJpgParam.u4CTgW = u4TgWidth;
		rJpgParam.u4CTgH = u4TgHeight;		
		break;
	case E_YCBCR4442YC422:
		rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422; //420, 422, 444		
		rJpgParam.u4CbSrcH		= u4SrcHeight;	
		rJpgParam.u4CrSrcH		= u4SrcHeight;				
		rJpgParam.u4CTgW = u4TgWidth >> 1;
		rJpgParam.u4CTgH = u4TgHeight;				
		break;
	case E_YCBCR4442YC420:
		rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_420; //420, 422, 444	
		rJpgParam.u4CbSrcH		= u4SrcHeight;	
		rJpgParam.u4CrSrcH		= u4SrcHeight;					
		rJpgParam.u4CTgW = u4TgWidth >> 1;
		rJpgParam.u4CTgH = u4TgHeight >> 1;				
		break;
	case E_YCBCR4222YC444: //ycbcr422 =>YC444
		rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_444; //420, 422, 444		
		rJpgParam.u4CbSrcH		= u4SrcHeight >> 1;	
		rJpgParam.u4CrSrcH		= u4SrcHeight >> 1;					
		rJpgParam.u4CTgW = u4TgWidth;
		rJpgParam.u4CTgH = u4TgHeight;		
		break;
	case E_YCBCR4222YC422: //YCBCR422 TO YC422
		rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422; //420, 422, 444	
		rJpgParam.u4CbSrcH		= u4SrcHeight >> 1;	
		rJpgParam.u4CrSrcH		= u4SrcHeight >> 1;				
		rJpgParam.u4CTgW = u4TgWidth >> 1;
		rJpgParam.u4CTgH = u4TgHeight;		
		break;
	case E_YCBCR4222YC420: //YCBCR422 TO YC420
		rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_420; //420, 422, 444		
		rJpgParam.u4CbSrcH		= u4SrcHeight >> 1;	
		rJpgParam.u4CrSrcH		= u4SrcHeight >> 1;				
		rJpgParam.u4CTgW = u4TgWidth >> 1;
		rJpgParam.u4CTgH = u4TgHeight >> 1;		
	case E_YCBCR4202YC444: //ycbcr422 =>YC444
		rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_444; //420, 422, 444	
		rJpgParam.u4CbSrcW		= u4SrcWidth;
		rJpgParam.u4CbSrcH		= u4SrcHeight >> 2;	
		rJpgParam.u4CrSrcW		= u4SrcWidth;
		rJpgParam.u4CrSrcH		= u4SrcHeight >> 2;				
		rJpgParam.u4CTgW = u4TgWidth;
		rJpgParam.u4CTgH = u4TgHeight;		
		break;
	case E_YCBCR4202YC422: //YCBCR422 TO YC422
		rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_422; //420, 422, 444
		rJpgParam.u4CbSrcH		= u4SrcHeight >> 2;	
		rJpgParam.u4CrSrcH		= u4SrcHeight >> 2;				
		rJpgParam.u4CTgW = u4TgWidth >> 1;
		rJpgParam.u4CTgH = u4TgHeight;		
		break;
	case E_YCBCR4202YC420: //YCBCR422 TO YC420
		rJpgParam.u4OutMode = E_RZ_VDO_OUTMD_420; //420, 422, 444	
		rJpgParam.u4CbSrcH		= u4SrcHeight >> 2;	
		rJpgParam.u4CrSrcH		= u4SrcHeight >> 2;					
		rJpgParam.u4CTgW = u4TgWidth >> 1;
		rJpgParam.u4CTgH = u4TgHeight >> 1;			
		break;
	default:
		Printf("Error occur, no support!\n");
		return -1;

	}
		
	//input ycbcr, output yc
	rJpgParam.u4IsRsIn		= 1; //block/raster in
	rJpgParam.u4YSrcBase1	= (u4SrcYAddr);
	rJpgParam.u4YSrcBase2	= (u4SrcYAddr);
	
	rJpgParam.u4YSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));
	rJpgParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf)); 
	
	rJpgParam.u4YSrcHOffset = 0;
	rJpgParam.u4YSrcVOffset = 0;
	rJpgParam.u4YSrcW		= u4SrcWidth;
	rJpgParam.u4YSrcH		= u4SrcHeight;
	
	rJpgParam.u4CbSrcBase1	= (u4SrcCbAddr);
	rJpgParam.u4CbSrcBase2	= (u4SrcCbAddr);	
	rJpgParam.u4CbSrcHOffset	= 0;
	rJpgParam.u4CbSrcVOffset	= 0;
	rJpgParam.u4CbSrcW		= u4SrcWidth;		
	
	rJpgParam.u4CrSrcBase1	= (u4SrcCrAddr);
	rJpgParam.u4CrSrcBase2	= (u4SrcCrAddr);	
	rJpgParam.u4CrSrcHOffset	= 0;
	rJpgParam.u4CrSrcVOffset	= 0;
	rJpgParam.u4CrSrcW		= u4SrcWidth;

	rJpgParam.u4IsRsOut 		= u4RsOut;  //block/raster out
	rJpgParam.u4OutSwap 		= 0;	
	rJpgParam.u4ColorComp = 7; //y,cb,cr exists
	
	rJpgParam.u4YTgBase 	= (u4TgYAddr);
	rJpgParam.u4CTgBase 	= (u4TgCAddr);
	
	rJpgParam.u4IsJpgSameaddr	= 0;
	rJpgParam.u4YTgHOffset		= 0;
	rJpgParam.u4YTgVOffset		= 0;
	rJpgParam.u4YTgBufLen		= (u4TgWidth + 0xf) & (~0xf);
	if(eType == E_YCBCR4442YC444 || eType == E_YCBCR4222YC444 || eType == E_YCBCR4202YC444)
	{
		rJpgParam.u4CTgBufLen = ((u4TgWidth + 0xf) & (~0xf)) << 1;
	}
	else
	{
		rJpgParam.u4CTgBufLen = (u4TgWidth + 0xf) & (~0xf);
	}
	
	rJpgParam.u4IsVdo2Osd = 0;	   // output in OSD format ?
	rJpgParam.u4YTgCM = 0; //E_RZ_OSD_DIR_CM_AYCbCr8888;
	rJpgParam.u4YTgW = u4TgWidth;
	rJpgParam.u4YTgH = u4TgHeight;		

	rJpgParam.u4IsFstBl = 1;
	rJpgParam.u4IsLstBl = 1;
	rJpgParam.u4JpgVfacY = 1; 
	rJpgParam.u4JpgVfacCb = 1;
	rJpgParam.u4JpgVfacCr = 1;		
	rJpgParam.fgBlassign = TRUE; //indicates to directly assign input/output address
	//time measure
	HAL_GetTime(&t_start);	

	IMGRZ_Lock_Ex(u4Which);
	IMGRZ_ReInit_Ex(u4Which);
	IMGRZ_SetScaleOpt_Ex(u4Which, E_RZ_INOUT_TYPE_JPGMD);
	IMGRZ_Scale_Ex(u4Which, &rJpgParam);
	IMGRZ_Flush_Ex(u4Which);
	IMGRZ_Wait_Ex(u4Which);
	IMGRZ_Unlock_Ex(u4Which);
	
    HAL_GetTime(&t_current);
    HAL_GetDeltaTime(&t_delta,&t_start,&t_current);     
    printf("\nResult[time = %u MS]\n", (t_delta.u4Seconds*1000 + t_delta.u4Micros/1000));
#ifdef IMGRZ_CLI_DEBUG	
	x_thread_delay(400);
	sprintf(fileName, "/mnt/usb/sda1/ty%d.bin", eType);	
	_ImgrzDumpData(fileName, (UINT8 *)(u4TgYAddr), 1024*1024);
	x_thread_delay(400);	
	sprintf(fileName, "/mnt/usb/sda1/tc%d.bin", eType);		
	_ImgrzDumpData(fileName, (UINT8 *)(u4TgCAddr), 1024*1024*2);										
#endif
	return 0;
}

//-------------------------------------------------------------------------
/** _ImgrzCmdYC2AYUV
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdYC2AYUV(ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T eType, INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4SrcYAddr, u4SrcCAddr, u4TgYAddr, u4Which, u4RsIn, i;
	UINT32 u4SrcWidth, u4SrcHeight, u4TgWidth, u4TgHeight;
	HAL_TIME_T	t_current,t_start,t_delta;
	RZ_VDO_SCL_PARAM_SET_T rSclParam;
#ifdef IMGRZ_CLI_DEBUG	
	char fileName[256] = "/mnt/usb/sda1/s.bin";	
#endif
	//init
	u4SrcYAddr = u4SrcCAddr  = u4TgYAddr = u4Which = u4RsIn = i = 0;
	u4SrcWidth = u4SrcHeight = u4TgWidth = u4TgHeight = 0;
	Printf("\nImgrz color fmt convert: %s\n", _arColCvType[eType]);

	if(i4Argc < 11)
	{
		Printf("The argument number is less than 11!!\n");
		Printf("FMT- imgrz.pft e_type imgrz_id y_addr c_addr src_w src_h tg_addr tg_w tg_h u4RsIn\n");
		Printf("Eg . imgrz.pft %d %d 0x384f1000 0x385f1000 1024 1024 0x3457a000 1024 1024 1\n", eType, u4Which);		
		return -1;
	}
	else
	{
		for(i = 0; i < i4Argc; i++)
		{
			if ((szArgv[i] == NULL))
			{
				return -1;
			}
		}	
		//to get src and tg params
		u4Which = StrToInt(szArgv[2]);
		u4SrcYAddr = StrToInt(szArgv[3]);
		u4SrcCAddr = StrToInt(szArgv[4]);
		u4SrcWidth = StrToInt(szArgv[5]);	
		u4SrcHeight = StrToInt(szArgv[6]);
		u4TgYAddr = StrToInt(szArgv[7]);
		u4TgWidth = StrToInt(szArgv[8]);
		u4TgHeight = StrToInt(szArgv[9]);
		u4RsIn = StrToInt(szArgv[10]);
		
		if(u4Which > (IMGRZ_NUM-1))
		{
			Printf("The imgrz hw number is %d, the max imgrz id is %d!!\n", IMGRZ_NUM, (IMGRZ_NUM - 1));
			return -1;
		}
				
		if((u4SrcYAddr&0xf) || (u4SrcCAddr&0xf) || (u4TgYAddr&0xf))
		{
			Printf("Make sure that the test address is 16bit align!!\n");
			return -1;
		}

		if((u4TgWidth < 4) || (u4TgHeight < 4) || (u4SrcWidth < 4) || (u4SrcHeight < 4))
		{
			Printf("Make sure that the src/dest width/height is larger than 4!!\n");
			return -1;
		}

		if(u4SrcWidth&0x1)
		{
			Printf("For video format input, make sure that the width is even!!\n");
			return -1;
		}
		Printf("Params: \nimgrz_id = %d,\ny_addr=0x%x,c_addr=0x%x,src_w=%d,src_h=%d,\ntg_addr=0x%x,tg_w=%d,tg_h=%d\nu4RsIn=%d\n",
			u4Which, u4SrcYAddr, u4SrcCAddr, u4SrcWidth, u4SrcHeight, u4TgYAddr, u4TgWidth, u4TgHeight, u4RsIn);
	}
#ifdef IMGRZ_CLI_DEBUG
	//y
	GFX_Memset((UINT8*)(u4SrcYAddr), 1024, 256, 0x40);			
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024 * 256), 1024, 256, 0x80);			
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024 * 512), 1024, 256, 0xc0);			
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024 * 768), 1024, 256, 0xe0);
	//c
	GFX_Memset((UINT8*)(u4SrcCAddr), 1024, 256, 0x00);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 256), 1024, 256, 0x22);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 512), 1024, 256, 0x44);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 768), 1024, 256, 0x66);	
	//c
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 1024), 1024, 256, 0x00);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 1024 + 1024 * 256), 1024, 256, 0x22);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 1024 + 1024 * 512), 1024, 256, 0x44);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 1024 + 1024 * 768), 1024, 256, 0x66);			
			   	
	// clean target buffer
	GFX_Memset((UINT8*)(u4TgYAddr), 1024, 1024, 0xc1);			
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*1024), 1024, 1024, 0xd1);
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*2*1024), 1024, 1024, 0xe1);
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*3*1024), 1024, 1024, 0xf1);
	x_thread_delay(400);
	sprintf(fileName, "/mnt/usb/sda1/sy.bin");	
	_ImgrzDumpData(fileName, (UINT8 *)(u4SrcYAddr), 1024*1024);
	x_thread_delay(400);
	sprintf(fileName, "/mnt/usb/sda1/sc.bin");		
	_ImgrzDumpData(fileName, (UINT8 *)(u4SrcCAddr), 1024*1024*2);
#endif
	
	x_memset((void *)&rSclParam, 0, sizeof(RZ_VDO_SCL_PARAM_SET_T));
	//input yc444
	switch(eType)
	{
	case E_YC4442AYUV8888:
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_444;				
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf)) << 1;
		rSclParam.u4CSrcW		= u4SrcWidth;
		rSclParam.u4CSrcH		= u4SrcHeight;		
		break;
		
	case E_YC4222AYUV8888:
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_422;				
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));	
		rSclParam.u4CSrcW		= u4SrcWidth >> 1;
		rSclParam.u4CSrcH		= u4SrcHeight;		
		break;	
		
	case E_YC4202AYUV8888:
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_420;				
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));	
		rSclParam.u4CSrcW		= u4SrcWidth >> 1;
		rSclParam.u4CSrcH		= u4SrcHeight >> 1;		
		break;	
		
	default:
		return -1;
	}	
	
	rSclParam.u4IsRsIn		= u4RsIn;
	rSclParam.u4YSrcBase	= (u4SrcYAddr);
	rSclParam.u4YSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));
	rSclParam.u4YSrcHOffset = 0;
	rSclParam.u4YSrcVOffset = 0;
	rSclParam.u4YSrcW		= u4SrcWidth;
	rSclParam.u4YSrcH		= u4SrcHeight;
	rSclParam.u4CSrcBase	= (u4SrcCAddr);
	rSclParam.u4CSrcHOffset = 0;
	rSclParam.u4CSrcVOffset = 0;
	rSclParam.u4IsRsOut 	= 1;
	
	//output aycbcr8888
	rSclParam.u4OutMode 	= (UINT32) E_RZ_VDO_OUTMD_444;
	rSclParam.u4IsVdo2Osd	= 1; //OSD format ?
	rSclParam.u4YTgCM		= (UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
	
	rSclParam.u4YTgBase 	= (u4TgYAddr);		
	rSclParam.u4YTgBufLen	= (((u4TgWidth + 0xf) & (~0xf)) * 4);
	rSclParam.u4YTgW		= u4TgWidth;
	rSclParam.u4YTgH		= u4TgHeight; 	
	rSclParam.u4YTgHOffset	= 0;
	rSclParam.u4YTgVOffset	= 0;
	
	rSclParam.u4CTgBase 	= (u4TgYAddr);		
	rSclParam.u4CTgBufLen	= (((u4TgWidth + 0xf) & (~0xf)) * 4);
	rSclParam.u4CTgW		= u4TgWidth;
	rSclParam.u4CTgH		= u4TgHeight;
	
	rSclParam.u4SrcSwap 	= 0;//(UINT32)IMGRZ_SWAP;
	rSclParam.u4OutSwap 	= 0;
	rSclParam.u4ABlend		= 0x80;
	
	//time measure
	HAL_GetTime(&t_start);	
	
	IMGRZ_Lock_Ex(u4Which);
	IMGRZ_ReInit_Ex(u4Which);
	IMGRZ_SetScaleOpt_Ex(u4Which, E_RZ_INOUT_TYPE_VDOMD);
	IMGRZ_Scale_Ex(u4Which, &rSclParam);
	IMGRZ_Flush_Ex(u4Which);
	IMGRZ_Wait_Ex(u4Which);
	IMGRZ_Unlock_Ex(u4Which);
	
	HAL_GetTime(&t_current);
	HAL_GetDeltaTime(&t_delta,&t_start,&t_current); 	
    printf("\nResult[time = %u MS]\n", (t_delta.u4Seconds*1000 + t_delta.u4Micros/1000));
		
#ifdef IMGRZ_CLI_DEBUG	
	x_thread_delay(400);
	sprintf(fileName, "/mnt/usb/sda1/d%d.bin", eType);	
	_ImgrzDumpData(fileName, (UINT8 *)(u4TgYAddr), 1024*1024*4);
#endif

	return 0;
}

//-------------------------------------------------------------------------
/** _ImgrzCmdYC2YC
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdYC2YC(ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T eType, INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4SrcYAddr, u4SrcCAddr, u4TgYAddr, u4TgCAddr, u4Which, u4RsInOut, i;
	UINT32 u4SrcWidth, u4SrcHeight, u4TgWidth, u4TgHeight;
	HAL_TIME_T	t_current,t_start,t_delta;
	RZ_VDO_SCL_PARAM_SET_T rSclParam;
#ifdef IMGRZ_CLI_DEBUG	
	char fileName[256] = "/mnt/usb/sda1/sy.bin";	
#endif	
	//init
	u4SrcYAddr = u4SrcCAddr  = u4TgYAddr = u4TgCAddr = u4Which = u4RsInOut = i = 0;
	u4SrcWidth = u4SrcHeight = u4TgWidth = u4TgHeight = 0;
	Printf("\nImgrz color fmt convert: %s\n", _arColCvType[eType]);
	
	if(i4Argc < 12)
	{
		Printf("The argument number is less than 12!!\n");
		Printf("FMT- imgrz.pft e_type imgrz_id y_addr c_addr src_w src_h tg_yaddr tg_caddr tg_w tg_h rs_in_out\n");
		Printf("Rs_in_out, 0:block in,blk out;1:blk in, raster out;2:rs in, blk out;3:rs in, rs out\n");
		Printf("Eg . imgrz.pft %d %d 0x384f1000 0x385f1000 1024 1024 0x3457a000 0x3467a000 1024 1024 3\n", eType, u4Which);		
		return -1;
	}
	else
	{
		for(i = 0; i < i4Argc; i++)
		{
			if ((szArgv[i] == NULL))
			{
				return -1;
			}
		}	
		//to get src and tg params
		u4Which = StrToInt(szArgv[2]);
		u4SrcYAddr = StrToInt(szArgv[3]);
		u4SrcCAddr = StrToInt(szArgv[4]);
		u4SrcWidth = StrToInt(szArgv[5]);	
		u4SrcHeight = StrToInt(szArgv[6]);
		u4TgYAddr = StrToInt(szArgv[7]);
		u4TgCAddr = StrToInt(szArgv[8]);		
		u4TgWidth = StrToInt(szArgv[9]);
		u4TgHeight = StrToInt(szArgv[10]);
		u4RsInOut = StrToInt(szArgv[11]);
		
		if(u4Which > (IMGRZ_NUM-1))
		{
			Printf("The imgrz hw number is %d, the max imgrz id is %d!!\n", IMGRZ_NUM, (IMGRZ_NUM - 1));
			return -1;
		}
		
		if((u4SrcYAddr&0xf) || (u4SrcCAddr&0xf) || (u4TgYAddr&0xf) || (u4TgCAddr&0xf))
		{
			Printf("Make sure that the test address is 16bit align!!");
			return -1;
		}

		if((u4TgWidth < 4) || (u4TgHeight < 4) || (u4SrcWidth < 4) || (u4SrcHeight < 4))
		{
			Printf("Make sure that the src/dest width/height is larger than 4!!");
			return -1;
		}

		if((u4TgWidth&0x1) || (u4SrcWidth&0x1))
		{
			Printf("For video format input/output, make sure that src/dest width is even!!");
			return -1;
		}
		Printf("Params: \nimgrz_id = %d, \ny_addr=0x%x,c_addr=0x%x,src_w=%d,src_h=%d,\ntg_yaddr=0x%x,tg_caddr=0x%x,tg_w=%d,tg_h=%d\nu4RsInOut=%d\n",
			u4Which, u4SrcYAddr, u4SrcCAddr, u4SrcWidth, u4SrcHeight, u4TgYAddr, u4TgCAddr, u4TgWidth, u4TgHeight, u4RsInOut);
	}
#ifdef IMGRZ_CLI_DEBUG
	//y
	GFX_Memset((UINT8*)(u4SrcYAddr), 1024, 256, 0x40);			
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024 * 256), 1024, 256, 0x80);			
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024 * 512), 1024, 256, 0xc0);			
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024 * 768), 1024, 256, 0xe0);
	//c
	GFX_Memset((UINT8*)(u4SrcCAddr), 1024, 256, 0x00);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 256), 1024, 256, 0x22);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 512), 1024, 256, 0x44);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 768), 1024, 256, 0x66);	
	//c
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 1024), 1024, 256, 0x00);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 1024 + 1024 * 256), 1024, 256, 0x22);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 1024 + 1024 * 512), 1024, 256, 0x44);			
	GFX_Memset((UINT8*)(u4SrcCAddr + 1024 * 1024 + 1024 * 768), 1024, 256, 0x66);			
			   	
	// clean target buffer
	GFX_Memset((UINT8*)(u4TgYAddr), 1024, 1024, 0xc1);			
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*1024), 1024, 1024, 0xd1);
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*2*1024), 1024, 1024, 0xe1);
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*3*1024), 1024, 1024, 0xf1);
	x_thread_delay(400);
	sprintf(fileName, "/mnt/usb/sda1/sy.bin");	
	_ImgrzDumpData(fileName, (UINT8 *)(u4SrcYAddr), 1024*1024);
	x_thread_delay(400);
	sprintf(fileName, "/mnt/usb/sda1/sc.bin");		
	_ImgrzDumpData(fileName, (UINT8 *)(u4SrcCAddr), 1024*1024*2);
#endif
	
	x_memset((void *)&rSclParam, 0, sizeof(RZ_VDO_SCL_PARAM_SET_T));
	//input yc444
	switch(eType)
	{
	case E_YC4442YC444:
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf)) << 1;			
		rSclParam.u4CTgBufLen	= (((u4TgWidth + 0xf) & (~0xf))) << 1;				
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_444;			
		rSclParam.u4OutMode 	= E_RZ_VDO_OUTMD_444;		
		rSclParam.u4CSrcW		= u4SrcWidth;
		rSclParam.u4CSrcH		= u4SrcHeight;	
		rSclParam.u4CTgW		= u4TgWidth;
		rSclParam.u4CTgH		= u4TgHeight;		
		break;
		
	case E_YC4442YC422:
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf)) << 1;			
		rSclParam.u4CTgBufLen	= (((u4TgWidth + 0xf) & (~0xf)));				
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_444;			
		rSclParam.u4OutMode 	= E_RZ_VDO_OUTMD_422;				
		rSclParam.u4CSrcW		= u4SrcWidth ;
		rSclParam.u4CSrcH		= u4SrcHeight;	
		rSclParam.u4CTgW		= u4TgWidth >> 1;
		rSclParam.u4CTgH		= u4TgHeight;		
		break;	
		
	case E_YC4442YC420:
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf)) << 1;			
		rSclParam.u4CTgBufLen	= (((u4TgWidth + 0xf) & (~0xf)));				
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_444;			
		rSclParam.u4OutMode 	= E_RZ_VDO_OUTMD_420;				
		rSclParam.u4CSrcW		= u4SrcWidth;
		rSclParam.u4CSrcH		= u4SrcHeight; 	
		rSclParam.u4CTgW		= u4TgWidth >> 1;
		rSclParam.u4CTgH		= u4TgHeight >> 1;		
		break;	
	case E_YC4222YC444:
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));			
		rSclParam.u4CTgBufLen	= (((u4TgWidth + 0xf) & (~0xf))) << 1;				
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_422;			
		rSclParam.u4OutMode 	= E_RZ_VDO_OUTMD_444;				
		rSclParam.u4CSrcW		= u4SrcWidth >> 1;
		rSclParam.u4CSrcH		= u4SrcHeight;	
		rSclParam.u4CTgW		= u4TgWidth;
		rSclParam.u4CTgH		= u4TgHeight;		
		break;
		
	case E_YC4222YC422:
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));			
		rSclParam.u4CTgBufLen	= (((u4TgWidth + 0xf) & (~0xf)));				
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_422;			
		rSclParam.u4OutMode 	= E_RZ_VDO_OUTMD_422;		
		rSclParam.u4CSrcW		= u4SrcWidth >> 1;
		rSclParam.u4CSrcH		= u4SrcHeight;	
		rSclParam.u4CTgW		= u4TgWidth >> 1;
		rSclParam.u4CTgH		= u4TgHeight;		
		break;	
		
	case E_YC4222YC420:
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));			
		rSclParam.u4CTgBufLen	= (((u4TgWidth + 0xf) & (~0xf)));				
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_422;			
		rSclParam.u4OutMode 	= E_RZ_VDO_OUTMD_420;			
		rSclParam.u4CSrcW		= u4SrcWidth >> 1;
		rSclParam.u4CSrcH		= u4SrcHeight; 	
		rSclParam.u4CTgW		= u4TgWidth >> 1;
		rSclParam.u4CTgH		= u4TgHeight >> 1;		
		break;	
		
	case E_YC4202YC444:
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));			
		rSclParam.u4CTgBufLen	= (((u4TgWidth + 0xf) & (~0xf))) << 1;				
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_420;			
		rSclParam.u4OutMode 	= E_RZ_VDO_OUTMD_444;			
		rSclParam.u4CSrcW		= u4SrcWidth >> 1;
		rSclParam.u4CSrcH		= u4SrcHeight >> 1;	
		rSclParam.u4CTgW		= u4TgWidth;
		rSclParam.u4CTgH		= u4TgHeight;		
		break;
		
	case E_YC4202YC422:
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));			
		rSclParam.u4CTgBufLen	= (((u4TgWidth + 0xf) & (~0xf)));		
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_420;			
		rSclParam.u4OutMode 	= E_RZ_VDO_OUTMD_422;			
		rSclParam.u4CSrcW		= u4SrcWidth >> 1;
		rSclParam.u4CSrcH		= u4SrcHeight >> 1;	
		rSclParam.u4CTgW		= u4TgWidth >> 1;
		rSclParam.u4CTgH		= u4TgHeight;		
		break;	
		
	case E_YC4202YC420:
		rSclParam.u4CSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));			
		rSclParam.u4CTgBufLen	= (((u4TgWidth + 0xf) & (~0xf)));				
		rSclParam.u4InMode		= E_RZ_VDO_OUTMD_420;			
		rSclParam.u4OutMode 	= E_RZ_VDO_OUTMD_420;			
		rSclParam.u4CSrcW		= u4SrcWidth >> 1;
		rSclParam.u4CSrcH		= u4SrcHeight >> 1; 	
		rSclParam.u4CTgW		= u4TgWidth >> 1;
		rSclParam.u4CTgH		= u4TgHeight >> 1;		
		break;			
		
	default:
		Printf("Error!!Can not find the color convert type!");
		return -1;
	}	
	
	rSclParam.u4IsRsIn		= ((u4RsInOut >> 1) & 0x1);
	rSclParam.u4YSrcBase	= (u4SrcYAddr);
	rSclParam.u4YSrcBufLen	= ((u4SrcWidth + 0xf) & (~0xf));
	rSclParam.u4YSrcHOffset = 0;
	rSclParam.u4YSrcVOffset = 0;
	rSclParam.u4YSrcW		= u4SrcWidth;
	rSclParam.u4YSrcH		= u4SrcHeight;
	rSclParam.u4CSrcBase	= (u4SrcCAddr);
	rSclParam.u4CSrcHOffset = 0;
	rSclParam.u4CSrcVOffset = 0;
	rSclParam.u4IsRsOut 	= (u4RsInOut & 0x1);
	
	//output aycbcr8888
	rSclParam.u4IsVdo2Osd	= 0; //OSD format ?
	rSclParam.u4YTgCM		= (UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
	
	rSclParam.u4YTgBase 	= (u4TgYAddr);		
	rSclParam.u4YTgBufLen	= (((u4TgWidth + 0xf) & (~0xf)));
	rSclParam.u4YTgW		= u4TgWidth;
	rSclParam.u4YTgH		= u4TgHeight;	
	rSclParam.u4YTgHOffset	= 0;
	rSclParam.u4YTgVOffset	= 0;	
	rSclParam.u4CTgBase 	= (u4TgCAddr);		
	rSclParam.u4SrcSwap 	= 0;//(UINT32)IMGRZ_SWAP;
	rSclParam.u4OutSwap 	= 0;
	rSclParam.u4ABlend		= 0x80;
	
	//time measure
	HAL_GetTime(&t_start);	
	
	IMGRZ_Lock_Ex(u4Which);
	IMGRZ_ReInit_Ex(u4Which);
	IMGRZ_SetScaleOpt_Ex(u4Which, E_RZ_INOUT_TYPE_VDOMD);
	IMGRZ_Scale_Ex(u4Which, &rSclParam);
	IMGRZ_Flush_Ex(u4Which);
	IMGRZ_Wait_Ex(u4Which);
	IMGRZ_Unlock_Ex(u4Which);
	
	HAL_GetTime(&t_current);
	HAL_GetDeltaTime(&t_delta,&t_start,&t_current); 	
    printf("\nResult[time = %u MS]\n", (t_delta.u4Seconds*1000 + t_delta.u4Micros/1000));
		
#ifdef IMGRZ_CLI_DEBUG	
	x_thread_delay(400);
	sprintf(fileName, "/mnt/usb/sda1/dy%d.bin", eType);		
	_ImgrzDumpData(fileName, (UINT8 *)(u4TgYAddr), 1024*1024);
	x_thread_delay(400);	
	sprintf(fileName, "/mnt/usb/sda1/dc%d.bin", eType);			
	_ImgrzDumpData(fileName, (UINT8 *)(u4TgCAddr), 1024*1024*2);	
#endif

	return 0;
}

//-------------------------------------------------------------------------
/** _ImgrzCmdArgbAyuvIntcov
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzCmdArgbAyuvIntcov(ENUM_CLIIMGRZ_COLOR_CONVERT_CODE_T eType, INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4SrcYAddr, u4TgYAddr, u4Which, i;
	UINT32 u4SrcWidth, u4SrcHeight, u4TgWidth, u4TgHeight;
	HAL_TIME_T	t_current,t_start,t_delta;
	RZ_OSD_SCL_PARAM_SET_T rSclParam;
#ifdef IMGRZ_CLI_DEBUG	
	char fileName[256] = "/mnt/usb/sda1/s.bin";	
#endif	
	//init
	u4SrcYAddr  = u4TgYAddr = u4Which = i = 0;
	u4SrcWidth = u4SrcHeight = u4TgWidth = u4TgHeight = 0;
	Printf("\nImgrz color fmt convert: %s\n", _arColCvType[eType]);
		
	if(i4Argc < 9)
	{
		Printf("The argument number is less than 8!!\n");
		Printf("FMT- imgrz.pft e_type imgrz_id y_addr src_w src_h tg_addr tg_w tg_h\n");
		Printf("Eg . imgrz.pft %d %d 0x384f1000 1024 1024 0x3457a000 1024 1024\n", eType, u4Which);		
		return -1;
	}
	else
	{
		for(i = 0; i < i4Argc; i++)
		{
			if ((szArgv[i] == NULL))
			{
				return -1;
			}
		}	
		//to get src and tg params
		u4Which = StrToInt(szArgv[2]);
		u4SrcYAddr = StrToInt(szArgv[3]);
		u4SrcWidth = StrToInt(szArgv[4]);	
		u4SrcHeight = StrToInt(szArgv[5]);
		u4TgYAddr = StrToInt(szArgv[6]);
		u4TgWidth = StrToInt(szArgv[7]);
		u4TgHeight = StrToInt(szArgv[8]);
		
		if(u4Which > (IMGRZ_NUM-1))
		{
			Printf("The imgrz hw number is %d, the max imgrz id is %d!!\n", IMGRZ_NUM, (IMGRZ_NUM - 1));
			return -1;
		}
				
		if((u4SrcYAddr&0xf) || (u4TgYAddr&0xf))
		{
			Printf("Make sure that the test address is 16bit align!!");
			return -1;
		}

		if((u4TgWidth < 4) || (u4TgHeight < 4) || (u4SrcWidth < 4) || (u4SrcHeight < 4))
		{
			Printf("Make sure that the src/dest width/height is larger than 4!!");
			return -1;
		}
		Printf("Params: \nimgrz_id = %d,\ny_addr=0x%x,src_w=%d,src_h=%d,\ntg_yaddr=0x%x,tg_w=%d,tg_h=%d\n",
			u4Which, u4SrcYAddr, u4SrcWidth, u4SrcHeight, u4TgYAddr, u4TgWidth, u4TgHeight);
	}
#ifdef IMGRZ_CLI_DEBUG
	//to set buffer color
	//y
	GFX_Memset((UINT8*)(u4SrcYAddr), 1024, 1024, 0x22);
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024*1024), 1024, 1024, 0x44);
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024*2*1024), 1024, 1024, 0x66);
	GFX_Memset((UINT8*)(u4SrcYAddr + 1024*3*1024), 1024, 1024, 0x88);
	
	// clean target buffer
	GFX_Memset((UINT8*)(u4TgYAddr), 1024, 1024, 0xc1);			
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*1024), 1024, 1024, 0xd1);
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*2*1024), 1024, 1024, 0xe1);
	GFX_Memset((UINT8*)(u4TgYAddr + 1024*3*1024), 1024, 1024, 0xf1);
	x_thread_delay(400);
	_ImgrzDumpData("/mnt/usb/sda1/s.bin", (UINT8 *)(u4SrcYAddr), 1024*1024*4);
#endif
	x_memset((void *)&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));
	//input argb8888
	switch(eType)
	{
	case E_AYUV88882ARGB8888:
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_AYCbCr8888;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB8888;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_AYUV8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 2) + 0xf) & (~0xf);
		rSclParam.u4TgBufLen	= ((u4TgWidth << 2) + 0xf) & (~0xf);		
		break;
		
	case E_AYUV88882AYUV8888:
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_AYCbCr8888;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_AYCbCr8888;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_AYUV8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_AYUV8888;	
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 2) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 2) + 0xf) & (~0xf);	
		break;	
		
	case E_AYUV88882ARGB1555:
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_AYCbCr8888;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB1555;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_AYUV8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 2) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);		
		break;
		
	case E_AYUV88882ARGB4444:
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_AYCbCr8888;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB4444;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_AYUV8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 2) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);		
		break;
		
	case E_AYUV88882RGB565:	
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_AYCbCr8888;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_RGB565;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_AYUV8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;	
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 2) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);			
		break;
		
  	case E_ARGB88882AYUV8888:
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB8888;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_AYCbCr8888;
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_AYUV8888;	
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 2) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 2) + 0xf) & (~0xf);			
		break;
		
  	case E_ARGB15552AYUV8888:
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB1555;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_AYCbCr8888;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_AYUV8888;
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 2) + 0xf) & (~0xf);					
		break;
		
  	case E_ARGB44442AYUV8888:
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB4444;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_AYCbCr8888;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_AYUV8888;
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 2) + 0xf) & (~0xf);							
		break;
		
  	case E_RGB5652AYUV8888:
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_RGB565;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_AYCbCr8888;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_AYUV8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 2) + 0xf) & (~0xf);		
		break;
	case E_ARGB88882ARGB8888:
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB8888;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB8888;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 2) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 2) + 0xf) & (~0xf);	
		break;
		
	case E_ARGB15552ARGB8888: 
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB1555;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB8888;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 2) + 0xf) & (~0xf);	
		break;
	case E_ARGB44442ARGB8888: 
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB4444;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB8888;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 2) + 0xf) & (~0xf);	
		break;
		
	case E_RGB5652ARGB8888  : //36
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_RGB565;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB8888;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 2) + 0xf) & (~0xf);	
		break;	
	
	case E_ARGB88882ARGB1555: 
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB8888;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB1555;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 2) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;
		
	case E_ARGB15552ARGB1555: 
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB1555;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB1555;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;			
	case E_ARGB44442ARGB1555: 
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB4444;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB1555;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;
		
	case E_RGB5652ARGB1555  :  //40
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_RGB565;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB1555;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;			
	case E_ARGB88882ARGB4444: 
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB8888;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB4444;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 2) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;	
		
	case E_ARGB15552ARGB4444:  
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB1555;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB4444;		
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;			
	case E_ARGB44442ARGB4444: 
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB4444;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB4444;	
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;
		
	case E_RGB5652ARGB4444  :  //44	
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_RGB565;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_ARGB4444;	
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;	

	case E_ARGB88882RGB565:
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB8888;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_RGB565;	
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 2) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;
		
	case E_ARGB15552RGB565:
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB1555;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_RGB565;	
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;
		
	case E_ARGB44442RGB565: 
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_ARGB4444;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_RGB565;	
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;
		
	case E_RGB5652RGB565  : //48	
		rSclParam.u4SrcCM		= E_RZ_OSD_DIR_CM_RGB565;
		rSclParam.u4TgCM		= E_RZ_OSD_DIR_CM_RGB565;	
		rSclParam.csc_src_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;
		rSclParam.csc_des_format	  = (E_RZ_CSC_FORMAT_T) E_RZ_OSD_CSC_FORMAT_ARGB8888;		
		rSclParam.u4SrcBufLen	= ((u4SrcWidth << 1) + 0xf) & (~0xf);	
		rSclParam.u4TgBufLen	= ((u4TgWidth << 1) + 0xf) & (~0xf);	
		break;		

	default:
		Printf("Error!!!!Can not support this case!");
		return -1;
	}	
	rSclParam.u4SrcBase 	= (u4SrcYAddr);
	rSclParam.u4IsSrcDirCM	= IMGRZ_TRUE;
	rSclParam.u4SrcHOffset	= 0;
	rSclParam.u4SrcVOffset	= 0;
	rSclParam.u4SrcW		= u4SrcWidth;
	rSclParam.u4SrcH		= u4SrcHeight;
	
	rSclParam.u4TgBase		= (u4TgYAddr);
	rSclParam.u4TgHOffset	= 0;
	rSclParam.u4TgVOffset	= 0;
	rSclParam.u4TgW 		= u4TgWidth;
	rSclParam.u4TgH 		= u4TgHeight;
	
	rSclParam.u4Csc_enable	= IMGRZ_TRUE;
	rSclParam.fgRBExChange	= FALSE;

	//time measure
	HAL_GetTime(&t_start);	
	
	IMGRZ_Lock_Ex(u4Which);
	IMGRZ_ReInit_Ex(u4Which);
	IMGRZ_SetScaleOpt_Ex(u4Which, E_RZ_INOUT_TYPE_OSDMD);
	IMGRZ_Scale_Ex(u4Which, &rSclParam);
	IMGRZ_Flush_Ex(u4Which);
	IMGRZ_Wait_Ex(u4Which);
	IMGRZ_Unlock_Ex(u4Which);
	
	HAL_GetTime(&t_current);
	HAL_GetDeltaTime(&t_delta,&t_start,&t_current); 	
    printf("\nResult[time = %u MS]\n", (t_delta.u4Seconds*1000 + t_delta.u4Micros/1000));
		
#ifdef IMGRZ_CLI_DEBUG	
	x_thread_delay(400);
	sprintf(fileName, "/mnt/usb/sda1/d%d.bin", eType);			
	_ImgrzDumpData(fileName, (UINT8 *)(u4TgYAddr), 1024*1024*4);
#endif

	return 0;						
}
#ifdef __MODEL_slt__
//-------------------------------------------------------------------------
/** _SLT_Imgrz
 *
 */
//-------------------------------------------------------------------------
static INT32 _ImgrzSlt(UINT32 u4Loop, UINT32 u4SwEn)
{
    RZ_OSD_SCL_PARAM_SET_T rScaleParam;
    INT32 i4Flag = 0, i4FailCount;
    UINT32 i, j = 0;
    UINT32 u4FbmStartAddr, u4FbmTotalSize;
    UINT32 u4Pitch32bpp;
    UINT32 u4CanvasWidth, u4CanvasHeight;
    UINT32 u4Width, u4Height;
    UINT32 u4BytePerPix, u4Sum, u4SltAns;
    UINT32 u4CanvasBufSize, u4CanvasPitch;
    UINT8 *pu1HwSrc, *pu1HwDst, *pu1HwDst2, *pu1HwDst3;
    UINT8 *pu1Ptr;

	x_memset(&rScaleParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));
	
    u4SltAns = 0x10073045;//mustang modify imgrz_coeff.c cl:1590473/1640607
                                                //ever chksum:0x100573fc;
    pu1HwDst3 = pu1HwDst2 = pu1HwDst = pu1HwSrc = NULL;
    u4BytePerPix = 4;   // 32-bpp (argb8888)
    
    u4CanvasWidth = 768;
    u4CanvasHeight = 512;
    u4CanvasPitch = (u4CanvasWidth * u4BytePerPix);
    
    u4Width = 720;
    u4Height = 496;

    u4CanvasBufSize = (u4CanvasWidth * u4CanvasHeight * u4BytePerPix);
    
    u4Pitch32bpp = (u4Width * u4BytePerPix);

    u4FbmStartAddr = DRVCUST_OptGet(eFbmMemAddr);
    u4FbmTotalSize = DRVCUST_OptGet(eFbmMemSize);

    HalFlushDCacheMultipleLine(VIRTUAL((UINT32)u4FbmStartAddr), 4*u4CanvasBufSize );
    
    // assign 4 buffers (src, dst, dst2, dst3)
    pu1HwSrc = (UINT8 *)(u4FbmStartAddr);
    VERIFY(pu1HwSrc != NULL);
    
    pu1HwDst = (UINT8 *)(u4FbmStartAddr + u4CanvasBufSize);
    VERIFY(pu1HwDst != NULL);

    pu1HwDst2 = (UINT8 *)(u4FbmStartAddr + (2 * u4CanvasBufSize));
    VERIFY(pu1HwDst2 != NULL);

    pu1HwDst3 = (UINT8 *)(u4FbmStartAddr + (3 * u4CanvasBufSize));
    VERIFY(pu1HwDst3 != NULL);

    Printf("------------------- [GFX-SCalar SLT] --------------------\n");
    Printf("FBM start addr = 0x%08x\n", u4FbmStartAddr);
    Printf("FBM total size = %u (MB)\n", (u4FbmTotalSize / 1048576));
    Printf("SRC  start addr = 0x%08x\n", (UINT32)pu1HwSrc);
    Printf("DST  start addr = 0x%08x\n", (UINT32)pu1HwDst);
    Printf("DST2 start addr = 0x%08x\n", (UINT32)pu1HwDst2);
    Printf("DST3 start addr = 0x%08x\n", (UINT32)pu1HwDst3);

    // initialization
    GFX_Init();
    //GFXSC_Init();
    
    // clear 4 buffers
    GFX_SetDst(pu1HwSrc, (UINT32)CM_ARGB8888_DIRECT32, u4CanvasPitch);
    GFX_SetColor(0x0);
    GFX_Fill(0, 0, u4CanvasWidth, u4CanvasHeight);
    GFX_SetDst(pu1HwDst, (UINT32)CM_ARGB8888_DIRECT32, u4CanvasPitch);
    GFX_SetColor(0x0);
    GFX_Fill(0, 0, u4CanvasWidth, u4CanvasHeight);
    GFX_SetDst(pu1HwDst2, (UINT32)CM_ARGB8888_DIRECT32, u4CanvasPitch);
    GFX_SetColor(0x0);
    GFX_Fill(0, 0, u4CanvasWidth, u4CanvasHeight);
    GFX_SetDst(pu1HwDst3, (UINT32)CM_ARGB8888_DIRECT32, u4CanvasPitch);
    GFX_SetColor(0x0);
    GFX_Fill(0, 0, u4CanvasWidth, u4CanvasHeight);

    GFX_Flush();
    GFX_Wait();

    i4Flag = 0;
    i4FailCount = 0;
    
    for (j = 0; j < u4Loop; j++)
    {
        // HW Action
        // 1. rect fill (src)
        GFX_SetDst(pu1HwSrc, (UINT32)CM_ARGB8888_DIRECT32, u4Pitch32bpp);
        GFX_SetColor(0xfa6359be);
        GFX_Fill(0, 0, u4Width, u4Height);
        GFX_SetColor(0xFFFFEBCD);    //blanchedalmond
        GFX_Fill(20,20,140,120);
        GFX_SetColor(0xFFFAEBD7);    //antiquewhite
        GFX_Fill(200, 20, 140, 120);
        GFX_SetColor(0xFF8A2BE2);    //blueviolet
        GFX_Fill(380, 20, 140, 120);
        GFX_SetColor(0xFF7FFFD4);    //aquamarine
        GFX_Fill(560, 20, 140, 120);
        GFX_SetColor(0xFFF0FFFF);    //azure
        GFX_Fill(20, 180, 140, 120);
        GFX_SetColor(0xFFF5F5DC);    //beige
        GFX_Fill(200, 180, 140, 120);
        GFX_SetColor(0xFFFFE4C4);    //bisque
        GFX_Fill(380, 180, 140, 120);
        GFX_SetColor(0xFFF0F8FF);    //aliceblue
        GFX_Fill(560, 180, 140, 120);
        GFX_SetColor(0xFFDEB887);    //burlywood
        GFX_Fill(20, 340, 140, 120);
        GFX_SetColor(0xFF00FFFF);    //aqua
        GFX_Fill(200, 340, 140, 120);
        GFX_SetColor(0xFFA52A2A);    //brown
        GFX_Fill(380, 340, 140, 120);
        GFX_SetColor(0xFF0000FF);    //blue
        GFX_Fill(560, 340, 140, 120);
        GFX_SetColor(0xFFFFFFFF);
        GFX_Fill(0, 0, 720, 2);
        GFX_Fill(0, 159, 720, 3);
        GFX_Fill(0, 319, 720, 3);
        GFX_Fill(0, 478, 720, 2);
        GFX_Fill(0, 0, 2, 480);
        GFX_Fill(179, 0, 3, 480);
        GFX_Fill(359, 0, 3, 480);
        GFX_Fill(539, 0, 3, 480);
        GFX_Fill(718, 0, 2, 480);
        GFX_Flush();
        GFX_Wait();
        
        HalInvalidateDCacheMultipleLine(VIRTUAL((UINT32)pu1HwSrc), u4CanvasBufSize);
        
        //scale down testing
        IMGRZ_ReInit();
        rScaleParam.u4SrcBase = (UINT32)pu1HwSrc;
        rScaleParam.u4IsSrcDirCM = 1;
        rScaleParam.u4SrcCM = (UINT32)E_RZ_OSD_DIR_CM_AYCbCr8888;
        rScaleParam.u4SrcHOffset = 0;
        rScaleParam.u4SrcVOffset = 0;
        rScaleParam.u4SrcW = u4Width;
        rScaleParam.u4SrcH = u4Height;
        rScaleParam.u4TgBase = (UINT32)pu1HwDst;
        rScaleParam.u4TgCM = rScaleParam.u4SrcCM;
      
        rScaleParam.u4SrcBufLen = u4Width << 2;
        rScaleParam.u4TgBufLen = u4Width << 1;
        rScaleParam.u4TgHOffset = 0;
        rScaleParam.u4TgVOffset = 0;
        rScaleParam.u4TgW = u4Width / 2;
        rScaleParam.u4TgH = u4Height / 2;                
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
        IMGRZ_Scale((void *)(&rScaleParam));   
        IMGRZ_Flush();
        IMGRZ_Wait();

        HalInvalidateDCacheMultipleLine(VIRTUAL((UINT32)pu1HwDst2), u4CanvasBufSize );
 

        //scale up testing
        IMGRZ_ReInit();
        rScaleParam.u4SrcBase = (UINT32)pu1HwDst;
        rScaleParam.u4IsSrcDirCM = 1;
        rScaleParam.u4SrcCM = (UINT32)E_RZ_OSD_DIR_CM_AYCbCr8888;
        rScaleParam.u4SrcHOffset = 0;
        rScaleParam.u4SrcVOffset = 0;
        rScaleParam.u4SrcW = u4Width / 2;
        rScaleParam.u4SrcH = u4Height / 2;
        rScaleParam.u4TgBase = (UINT32)pu1HwDst2;
        rScaleParam.u4TgCM = rScaleParam.u4SrcCM;
      
        rScaleParam.u4SrcBufLen = u4Width << 1;
        rScaleParam.u4TgBufLen = u4Width << 2;
        rScaleParam.u4TgHOffset = 0;
        rScaleParam.u4TgVOffset = 0;
        rScaleParam.u4TgW = u4Width;
        rScaleParam.u4TgH = u4Height;
        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
        IMGRZ_Scale((void *)(&rScaleParam));   
        IMGRZ_Flush();
        IMGRZ_Wait();

        HalInvalidateDCacheMultipleLine(VIRTUAL(pu1HwDst2), u4CanvasBufSize );

        // physical -> virtual address
        pu1Ptr = (UINT8 *)VIRTUAL((UINT32)pu1HwDst2);
        
        // check sum
        u4Sum = 0;
        for (i = 0; i < u4CanvasBufSize; i++)
        {
            u4Sum += ((UINT32)(*(pu1Ptr + i)));
        }

        Printf("[Run %d]\n", j);
        Printf("u4Sum    = 0x%08x\n", u4Sum);
        Printf("u4SltAns = 0x%08x\n", u4SltAns);

        if (u4Sum != u4SltAns)
        {
            i4Flag = 1;
            i4FailCount++;
        }
    } // ~for (j = 0; j < i4Loop; j++)

    if (i4Flag == 1)
    {
        Printf("[IMGRZ] Fail Count = %d\n", i4FailCount);
        return -1;
    }
    else
    {
        return (INT32)E_CLIIMGRZ_OK;
    }
}
#endif
#if 0
//-------------------------------------------------------------------------
/** _GetEnum
 *  
 */
//-------------------------------------------------------------------------
static INT32 _RzCmdGetEnum(const CHAR *szText, const IMGRZCMDTEXT2ENUM_T* prText2Enum)
{
    if ((NULL == szText) ||
        (NULL == prText2Enum))
    {
        return -(INT32)E_CLIIMGRZ_ERR_PARAM;
    }

    while (prText2Enum->szText)
    {
        if (x_strncmp(prText2Enum->szText, szText, x_strlen(prText2Enum->szText)) == 0)
        {
            break;
        }
        else
        {
            prText2Enum++;
        }
    }
    return prText2Enum->i4Value;
}
#endif

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

CLI_MAIN_COMMAND_ITEM(IMGRZ)
{
    "imgrz",
    NULL,
    NULL,
    _arImgrzCmdTbl,
    "Imgrz command",
    CLI_SUPERVISOR
};
/*lint -restore */


