/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: img_lib_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file img_lib_if.c
 *  Image library command state machine.
 */


/******************************************************************************
* Include files
******************************************************************************/
#include "img_flw.h"
#include "jpg_debug.h"
#include "drv_common.h"
#include "x_assert.h"
#include "jpg_if.h"
#include "jpg_hw.h"
#include "drvcust_if.h"
#include "img_lib_if.h"
#ifndef CC_IMAGE_PNG_DISABLE
#include "../png/png_drv.h"
#include "../png/png_hal_dec.h"
#endif

#ifndef CC_IMAGE_WEBP_DISABLE
#include "../webp/webp_drv.h"
#endif
#include "c_model.h"

//#include "linux/sched.h"

/******************************************************************************
* Local	Macros,	defines, typedefs, enums
******************************************************************************/
//#define JPG_THREAD_NAME				"JPG"
#define JPG_STACK_SIZE					3072
#define JPG_THREAD_PRIORITY			80

#define JPG_CMD_SIZE					4
#define JPG_CMD_Q_SIZE					6
#define JPG_DATA_Q_SIZE					3

/******************************************************************************
* Function prototype
******************************************************************************/
static void _ImgSetCmd(UCHAR ucImgId,UINT32 u4Cmd);
static void _ImgReceiveCmd(UCHAR ucImgId,BOOL bIsBlock);
static void _ImgSendData(HANDLE_T hDataQ, UINT32 u4DataSize, UINT32 u4DataAddr);
static void _ImgReceiveData(HANDLE_T hDataQ, UINT32 u4DataSize, void *pvData);
static void _ImgMainLoop(void* pvArg);
void _ImgHWPowerTurnOff(void);

/******************************************************************************
* Local	variables
******************************************************************************/
#if 0
static IMGHANDLE _hCurImg;
static HANDLE_T _hCmdQueue, _hDecDataQueue, _hSetDataQueue;
static HANDLE_T _hInitSema, _hReleaseSema, _hFillBufSema;
#ifndef CC_IMAGE_PNG_DISABLE
static HANDLE_T	_hPngSema;
#endif

static UINT32 _u4CurState = IMG_RELEASE;
static UINT32 _u4NotifyType = JPG_HW_NFY_NONE;
static UINT32 _u4ImgID = 0;
#endif
static BOOL _bImgModEnd = TRUE;
static IMG_INFO_T _rImgInfo;
static HANDLE_T hDBGInsertSema=0;

#ifdef __KERNEL__
extern UINT32 VIRTUAL(UINT32);
#endif

UINT32 u4hSemaphoreDBGCnt = 0;
UINT32 u4hSemaphoreDBG[256][2]={{0}, {0}};
UINT32 u4hInitSema =1;
UINT32 u4hReleaseSema =2;
UINT32 u4hFillBufSema =3;
UINT32 u4hardwareSema =4;
extern BOOL fgIsMJPEGPlaying;
/******************************************************************************
* Global Function definition
******************************************************************************/

void u4hSemaphoreDBGInsert(UINT32 type, UINT32 value)
{

	if(hDBGInsertSema!=0)
	VERIFY (x_sema_lock(hDBGInsertSema, X_SEMA_OPTION_WAIT) == OSR_OK);
	
	u4hSemaphoreDBG[u4hSemaphoreDBGCnt][0] = type;
	u4hSemaphoreDBG[u4hSemaphoreDBGCnt][1] = value;

	u4hSemaphoreDBGCnt++;
	
	if(u4hSemaphoreDBGCnt>=256)
	{	
		u4hSemaphoreDBGCnt = 0;
	}  	
	
	if(hDBGInsertSema!=0)
	VERIFY (x_sema_unlock(hDBGInsertSema) == OSR_OK);
}


/**
 * Initialize image library with async mode.
 *
 * @param void.
 * @return ENUM_IMG_RET.
 */
ENUM_IMG_RET IMG_Lib_Init(void)
{
	HANDLE_T hThread;
	UCHAR ucImgId ;
	CHAR szBuf[16];
	IMG_INFO_T *prImgInfo = _IMG_GetInfo();
	if (_bImgModEnd == TRUE)
	{
		_bImgModEnd = FALSE;
		prImgInfo->bImgModEnd = FALSE;
		VERIFY (x_sema_create(&hDBGInsertSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
		for (ucImgId = 0; ucImgId < IMG_MAX; ucImgId++)
		{
			LOG(0,"IMG_Lib_Init %d.........\n",ucImgId);
			prImgInfo->arImgInfoKeep[ucImgId].u4CurState = IMG_RELEASE;
			prImgInfo->arImgInfoKeep[ucImgId].u4NotifyType = JPG_HW_NFY_NONE;
			x_snprintf(szBuf, sizeof(szBuf), "IMG_CMDQ %3d", ucImgId);
			VERIFY(x_msg_q_create(&prImgInfo->arImgInfoKeep[ucImgId].hCmdQueue, szBuf, JPG_CMD_SIZE, JPG_CMD_Q_SIZE) == OSR_OK);
			x_snprintf(szBuf, sizeof(szBuf), "IMG_DECQ %3d", ucImgId);
			VERIFY(x_msg_q_create(&prImgInfo->arImgInfoKeep[ucImgId].hDecDataQueue,szBuf, sizeof(IMG_PARAM_T), JPG_DATA_Q_SIZE) == OSR_OK);
			x_snprintf(szBuf, sizeof(szBuf), "IMG_SETQ %3d", ucImgId);
			VERIFY(x_msg_q_create(&prImgInfo->arImgInfoKeep[ucImgId].hSetDataQueue, szBuf, sizeof(IMG_BUF_PARAM_T), JPG_DATA_Q_SIZE) == OSR_OK);

			VERIFY (x_sema_create(&prImgInfo->arImgInfoKeep[ucImgId].hInitSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
			VERIFY (x_sema_create(&prImgInfo->arImgInfoKeep[ucImgId].hReleaseSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
			VERIFY (x_sema_create(&prImgInfo->arImgInfoKeep[ucImgId].hFillBufSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
			VERIFY(x_sema_create(&prImgInfo->arImgInfoKeep[ucImgId].hPngSema, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
			VERIFY(x_sema_create(&prImgInfo->arImgInfoKeep[ucImgId].hHWResourceSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);			
			x_snprintf(szBuf, sizeof(szBuf), "IMAGE %3d", ucImgId);
			//create main loop thread, create cmd msg queue, data msg queue
			VERIFY(x_thread_create(&hThread, szBuf, JPG_STACK_SIZE,
								   JPG_THREAD_PRIORITY,  _ImgMainLoop, sizeof(ucImgId), (void*)&ucImgId) == OSR_OK);
		}
#ifndef CC_IMAGE_PNG_DISABLE
		VERIFY(i4PngHalInitHwInst() == OSR_OK);
#endif
	_ImgHWPowerTurnOff();
	}
	return E_IMG_OK;
}
/**
 * Release image library with async mode.
 *
 * @param void.
 * @return void.
 */
void IMG_Lib_End(void)
{
	UCHAR ucImgId ;
	IMG_INFO_T *prImgInfo = _IMG_GetInfo();

	if (_bImgModEnd == FALSE)
	{
		VERIFY(x_sema_delete(hDBGInsertSema) == OSR_OK);
		for (ucImgId = 0; ucImgId < IMG_MAX; ucImgId++)
		{
			VERIFY(x_msg_q_delete(prImgInfo->arImgInfoKeep[ucImgId].hCmdQueue)== OSR_OK);
			VERIFY(x_msg_q_delete(prImgInfo->arImgInfoKeep[ucImgId].hDecDataQueue)== OSR_OK);
			VERIFY(x_msg_q_delete(prImgInfo->arImgInfoKeep[ucImgId].hSetDataQueue)== OSR_OK);

			VERIFY(x_sema_delete(prImgInfo->arImgInfoKeep[ucImgId].hInitSema) == OSR_OK);
			VERIFY(x_sema_delete(prImgInfo->arImgInfoKeep[ucImgId].hReleaseSema) == OSR_OK);
			VERIFY(x_sema_delete(prImgInfo->arImgInfoKeep[ucImgId].hFillBufSema) == OSR_OK);
			VERIFY(x_sema_delete(prImgInfo->arImgInfoKeep[ucImgId].hPngSema) == OSR_OK);
			VERIFY(x_sema_delete(prImgInfo->arImgInfoKeep[ucImgId].hHWResourceSema) == OSR_OK);
		}
#ifndef CC_IMAGE_PNG_DISABLE
		VERIFY(i4PngHalUnInitHwInst() == OSR_OK);
#endif
		_bImgModEnd = TRUE;
	}
}


void IMG_LockHwResource(UCHAR ucImgId)
{
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
    if(prImgKeepInfo->hHWResourceSema)
    {
		VERIFY (x_sema_lock(prImgKeepInfo->hHWResourceSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
}

void IMG_UnLockHwResource(UCHAR ucImgId)
{
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
    if(prImgKeepInfo->hHWResourceSema)
    {
		VERIFY (x_sema_unlock(prImgKeepInfo->hHWResourceSema) == OSR_OK);
    }
}

#ifdef CC_FUJI3D_SUPPORT
 void IMG_GetMPOInfo(UCHAR ucImgId,UINT32* pu4IsSupportedMPF, UINT32* pu4MPFOffset)
 {
	 IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
	 if (prImgKeepInfo->pfnImgGetInfo && prImgKeepInfo->hCurImg)
	 {
		 prImgKeepInfo->pfnImgGetInfo
			 (prImgKeepInfo->hCurImg,IMG_GETINFO_MPO_PARSING,(UINT32)pu4IsSupportedMPF,(UINT32)pu4MPFOffset);
	 }
	 return;
 }
#endif

 #ifdef IMG_INPUT_CHECKSUM
extern UINT32 _u4Entry1StartAddr ;
extern UINT32 _u4Entry2StartAddr ;
UINT32 _CalcCheckSum(void* pvData, UINT32 u4ByteLen)
{
    UINT32* pu4Data;
    UINT32 u4Sum = 0;

    //ASSERT(pvData!=NULL);
    if(pvData == NULL)
    {
       LOG(1,"JPG _CalcCheckSum pvData == NULL");
       return 0;
    }
    pu4Data = (UINT32*)pvData;
    while (u4ByteLen > 3)
    {
        u4Sum += *pu4Data;
        if (u4ByteLen >= 4)
        {
            u4ByteLen -= 4;
        }
        else
        {
            u4ByteLen = 0;
        }
        pu4Data++;
    }

    return u4Sum;
}
UINT32 _u4Checksum = 0;
UINT32 _u4Size1 = 0;
UINT32 _u4Size2 = 0;
UINT32 _u4Checksum1 = 0;
#endif

/**
 * Initialize image module.
 *
 * @param eMemClass in/out buffer type.
 * @param pvTag private data.
 * @param pfnReg callback function pointer.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_Init_Ex(ENUM_IMG_BUFFER_TYPE eMemClass, void *pvTag, UINT32 u4CallbackId, UINT32 u4ImgID,
                      PFN_JPG_EVENT_NOTIFY pfnReg)
{
	UCHAR ucImgId;
	IMG_INFO_KEEP_T* prImgKeepInfo ;//= _IMG_GetInfoKeep(ucImgId);

    ucImgId = (UCHAR)(u4ImgID&0xFF);
    if (ucImgId >= IMG_MAX)
	{
		ucImgId = 0;
	}
	
	if(fgIsMJPEGPlaying && (ucImgId == 0))
	{
        LOG(0, "[%s][%d]Mjpeg video is playing , return IMG_Init_Ex Fail!!\n", __FUNCTION__, __LINE__);
		return E_IMG_INIT_FAIL;		
	}	
	prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);

	if (prImgKeepInfo->pfnImgInit)
	{
		prImgKeepInfo->hCurImg = prImgKeepInfo->pfnImgInit(ucImgId,eMemClass, pvTag);
	}
	else
	{
		return E_IMG_INIT_FAIL;
	}
	if (prImgKeepInfo->hCurImg == NULL)
	{
		return E_IMG_INIT_FAIL;
	}
	//register callback function
    _IMG_CBreg(ucImgId,pfnReg);
    prImgKeepInfo->u4ImgID = ucImgId;
    prImgKeepInfo->u4CallbackId = u4CallbackId;
	prImgKeepInfo->bIMG_FillDataTimeout = FALSE;    
#ifdef IMG_INPUT_CHECKSUM
   _u4Checksum = 0;
	_u4Entry1StartAddr = 0;
	_u4Entry2StartAddr = 0;
	_u4Size1 = 0;
	_u4Size2 = 0;
	_u4Checksum1 = 0;
#endif
	//send cmd msg queue, INIT
	_ImgSetCmd(ucImgId,IMG_INIT);
    //printk("IMG_Init lock %d \n", current->pid);
    u4hSemaphoreDBGInsert(u4hInitSema, 0xff-ucImgId);   //start code for debug
    u4hSemaphoreDBGInsert(u4hInitSema, 1);
	VERIFY (x_sema_lock(prImgKeepInfo->hInitSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    u4hSemaphoreDBGInsert(u4hInitSema, 2);   

	return E_IMG_OK;
}
/**
 * Initialize image module.
 *
 * @param eMemClass in/out buffer type.
 * @param pvTag private data.
 * @param pfnReg callback function pointer.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_Init(ENUM_IMG_BUFFER_TYPE eMemClass, void *pvTag, UINT32 u4ImgID,
                      PFN_JPG_EVENT_NOTIFY pfnReg)
{
/*
	UCHAR ucImgId;
	IMG_INFO_KEEP_T* prImgKeepInfo ;//= _IMG_GetInfoKeep(ucImgId);

    ucImgId = (UCHAR)(u4ImgID&0xFF);
    if (ucImgId >= IMG_MAX)
	{
		ucImgId = 0;
	}

	prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
	
	prImgKeepInfo->bNeedToUnlockHwSemaWhenRelease = TRUE;
    u4hSemaphoreDBGInsert(u4hardwareSema, 0+(u4ImgID*0xf0));
    IMG_LockHwResource(u4ImgID);
    u4hSemaphoreDBGInsert(u4hardwareSema, 1+(u4ImgID*0xf0));
*/    
    return IMG_Init_Ex(eMemClass, pvTag, u4ImgID, u4ImgID, pfnReg);
}



/**
 * Decode image and scale output raw data.
 *
 * @param rInfo input compressed image and output destination information.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_Decode(UCHAR ucImgId,IMG_PARAM_T rInfo)
{
	//send data msg queue
	//send cmd msg queue, DECODE
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);

	_ImgSendData(prImgKeepInfo->hDecDataQueue, sizeof(IMG_PARAM_T), (UINT32)&rInfo);
	_ImgSetCmd(ucImgId,IMG_DECODE);
	return E_IMG_OK;
}



/**
 * Apply operations on decoded raw image.
 *
 * @param rCmd command for image operation(zoom and rotate).
 * @return void
 */
void IMG_Set(IMG_BUF_PARAM_T rCmd, IMG_REGION_T *rDstRegion)
{
    _IMG_Set(rCmd, rDstRegion);
}



/**
 * Query module capability.
 *
 * @param rCap max supported image size and buffer size.
 * @return void.
 */
void IMG_Get(IMG_CAP_T* prCap)
{
    //get information
    _IMG_Get(prCap);
}

/**
 * Set Image File Size.
 *
 * @Image File Size.
 * @return void
 */
void IMG_Set_ImgSize(UCHAR ucImgId,UINT32 u4ImgSize)
{
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
	_IMG_SetImgSize(ucImgId,prImgKeepInfo->hCurImg,u4ImgSize);
}


/**
 * Query decoding progress.
 *
 * @param decoding process.
 * @return void
 */
void IMG_GetProg(UCHAR ucImgId,UINT32* pu4Status)
{
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
	if (prImgKeepInfo->pfnImgGetInfo)
	{
		prImgKeepInfo->pfnImgGetInfo(prImgKeepInfo->hCurImg,IMG_GETINFO_PROGRESS,(UINT32)pu4Status,0);
	}
	return;
}


/**
 * Query decoding capabilities.
 *
 * @param ptr of decoding capabilities.
 * @return void
 */
void IMG_GetCap(VOID *pv_get_info)
{
    IMG_LIMIT_T *prLimit = (IMG_LIMIT_T *)pv_get_info;

    if (prLimit->u4Type == IMG_LIMIT_PROGRESSIVE)
    {
        prLimit->u.fgProgressiveSupport = TRUE;
    }
    else if (prLimit->u4Type == IMG_LIMIT_PGSV_RESOLUTION)
    {
        prLimit->u.t_pgsv_resolution.u4MaxArea = (UINT32)(4000 * 4000);
        prLimit->u.t_pgsv_resolution.u4MinWidth = 4;
        prLimit->u.t_pgsv_resolution.u4MinHeight = 4;
    }
    else if (prLimit->u4Type == IMG_LIMIT_RESOLUTION)
    {
        prLimit->u.t_resolution.u4MaxWidth = 17000;
        prLimit->u.t_resolution.u4MaxHeight = 10000;
        prLimit->u.t_resolution.u4MinWidth = 4;
        prLimit->u.t_resolution.u4MinHeight = 4;
    }
    else if (prLimit->u4Type == IMG_LIMIT_PNG_RESOLUTION)
    {
        prLimit->u.t_resolution.u4MaxArea = (UINT32)(5760 * 4096);
        prLimit->u.t_resolution.u4MaxWidth = 8192;
        prLimit->u.t_resolution.u4MinWidth = 4;
        prLimit->u.t_resolution.u4MinHeight = 4;
    }
    else
    {
#ifdef CC_JPEG_PIC_MODE
        prLimit->u.fgFullModeSpport = _IMG_IsFullImgDecoding();
#else
        prLimit->u.fgFullModeSpport = FALSE;
#endif
    }
}

extern VOID vJpegSetProgressiveStop(void);
/**
 * Stop decoding procedure, sync cmd.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
void IMG_Stop(UCHAR ucImgId)
{
	//send cmd	msg queue, CANCEL
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
	_ImgSetCmd(ucImgId,IMG_STOP);
    //progressive decoding only in 0 pipeline
	if(ucImgId == 0)
	{
		vJpegSetProgressiveStop();
	}
	if (prImgKeepInfo->pfnImgStop)
	{
		prImgKeepInfo->pfnImgStop(prImgKeepInfo->hCurImg);
	}
}

/**
 * Release handler, sync cmd.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
void IMG_Release(UCHAR ucImgId)
{
	//send cmd msg queue, RELEASE
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
    if(prImgKeepInfo->u4CurState==IMG_BSBUF_UNDERFLOW)
		{    	
			//printk("IMG_Release call IMG_SetFillBsBufEnd() [prImgKeepInfo->u4CurState=%d]\n", prImgKeepInfo->u4CurState);
		prImgKeepInfo->bIMG_FillDataTimeout = TRUE;			
    	IMG_SetFillBsBufEnd(ucImgId,0, 0, TRUE);
    } 
	_ImgSetCmd(ucImgId,IMG_RELEASE);
    //if(_bNeedToUnlockHwSemaWhenRelease)
    if(prImgKeepInfo->bNeedToUnlockHwSemaWhenRelease)
    {
        //_bNeedToUnlockHwSemaWhenRelease = FALSE;
        prImgKeepInfo->bNeedToUnlockHwSemaWhenRelease = FALSE;
        u4hSemaphoreDBGInsert(u4hardwareSema, 10+(ucImgId*0xf0));
        IMG_UnLockHwResource(ucImgId);
        u4hSemaphoreDBGInsert(u4hardwareSema, 11+(ucImgId*0xf0));
    }	
    //printk("IMG_Release lock %d [prImgKeepInfo->u4CurState=%d]\n", current->pid, prImgKeepInfo->u4CurState);
    u4hSemaphoreDBGInsert(u4hReleaseSema, 1+(ucImgId*0xf0));
	VERIFY (x_sema_lock(prImgKeepInfo->hReleaseSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    u4hSemaphoreDBGInsert(u4hReleaseSema, 2+(ucImgId*0xf0));
}


/**
 * Connect video path.
 *
 * @param video plane id.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_VDP_Connect(UINT32 u4VideoId)
{
    return _IMG_VDP_Connect(u4VideoId);
}


/**
 * Query video plane info, sync cmd.
 *
 * @param video plane information.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_GetVideoPlaneInfo(IMG_VP_INFO_T *prVpInfo)
{
    return _IMG_GetVideoPlaneInfo(prVpInfo);
}


/**
 * Flip double buffer for video plane, swap pu1CurFrameAddr and pu1BackFrameAddr, sync cmd.
 *
 * @param void.
 * @return ENUM_IMG_RET
 */
ENUM_IMG_RET IMG_Flip(void)
{
    return _IMG_Flip();
}


/**
 * Set status while buffer filling operation finish.
 *
 * @param u4Entry1Size buffer entry1 filled size.
 * @param u4Entry2Size buffer entry2 filled size.
 * @param u4IsEOF flag if end of image file.
 * @return void
 */

void IMG_SetFillBsBufEnd(UCHAR ucImgId,UINT32 u4Entry1Size, UINT32 u4Entry2Size, BOOL bIsEOF)
{
    //send cmd msg queue, FILLBUF
    IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);

    if (prImgKeepInfo->pfnImgSetFillBsBufEnd)
    {
        prImgKeepInfo->pfnImgSetFillBsBufEnd(prImgKeepInfo->hCurImg, u4Entry1Size, u4Entry2Size, bIsEOF);
    }
    else
    {
        LOG(0, "IMG_SetFillBsBufEnd pfnImgSetFillBsBufEnd NULL %d!!!!!!!!!\n" ,ucImgId);
    }

#ifdef IMG_INPUT_CHECKSUM
    _u4Size1 = u4Entry1Size;
    _u4Size2 = u4Entry2Size;

    if(u4Entry1Size != 0)
    {
       _u4Checksum += _CalcCheckSum((void*)_u4Entry1StartAddr,u4Entry1Size);       
       LOG(7, "IMG_SetFillBsBufEnd _u4Entry1StartAddr = 0x%08x,u4Entry1Size = 0x%08x,ChecksumA = 0x%08x\n",_u4Entry1StartAddr,u4Entry1Size,_u4Checksum);
    }
    if(u4Entry2Size != 0)
    {
       _u4Checksum += _CalcCheckSum((void*)_u4Entry2StartAddr,u4Entry2Size);
       LOG(7, "IMG_SetFillBsBufEnd _u4Entry2StartAddr = 0x%08x,u4Entry2Size = 0x%08x,ChecksumB = 0x%08x\n",_u4Entry2StartAddr,u4Entry2Size,_u4Checksum);
    }

    if(bIsEOF == 1)
    {
        LOG(0, "IMG_SetFillBsBufEnd Checksum = 0x%08x!!!!!!!!!\n",_u4Checksum );
        _u4Checksum = 0;
    }
 #endif
 if (prImgKeepInfo->u4CurState == IMG_BSBUF_UNDERFLOW)
 {
    		u4hSemaphoreDBGInsert(u4hFillBufSema, 1+(ucImgId*0xf0));
	 VERIFY (x_sema_unlock(prImgKeepInfo->hFillBufSema) == OSR_OK);
        u4hSemaphoreDBGInsert(u4hFillBufSema, 2+(ucImgId*0xf0));
 }

}

/**
 * Get internal buffer info.
 *
 * @param prImgBufInfo buffer description.
 * @return void.
 */
void IMG_GetBufInfo(IMG_BUF_INFO_T* prImgBufInfo)
{
    _IMG_GetBufInfo(prImgBufInfo);
}

/**
 * Get internal bitstream ring buffer info.
 *
 * @param prStreamBufInfo buffer description.
 * @return UINT32 available size.
 */
UINT32 IMG_GetBSBufInfo(UCHAR ucImgId,IMG_LIB_FILL_RING_BUF_T *prStreamBufInfo)
{
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);

	return _IMG_GetBSBufInfo(ucImgId,prImgKeepInfo->hCurImg, prStreamBufInfo);
}

/**
 * Get state of decoding process.
 *
 * @param void.
 * @return decoding state.
 */
UINT32 IMG_GetDecState(UCHAR ucImgId)
{
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);

	return prImgKeepInfo->u4CurState;
}

ENUM_IMG_RET IMG_SetUseMw(UCHAR ucImgId,BOOL fgUseMw)
{
    IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)prImgKeepInfo->hCurImg;
    if (!prImg)
    {
        return E_IMG_INIT_FAIL;
    }

    prImg->bUseMw=fgUseMw;
    return E_IMG_OK;
}


BOOL _IMGRZ_UsedByJpeg(UCHAR ucImgId)
{
    IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
#ifdef CC_JPEG_PIC_MODE
    if (prImgKeepInfo->u4CurState!=IMG_RELEASE)
    {
        return TRUE;
    }
#endif
    return FALSE;
}


//for verification only
INT32 IMG_GetOutputBuffer(UCHAR ucImgId,void **ppvmem)
{
    IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);

    return _IMG_GetOutputBuffer(prImgKeepInfo->hCurImg, ppvmem);
}


INT32 IMG_BsBufManager(UCHAR ucImgId)
{
    INT32 i4Ret;
    IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);

    prImgKeepInfo->u4CurState = IMG_BSBUF_UNDERFLOW;
    i4Ret = _IMG_BsBufManager((IMGHANDLE)prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID);
    if (i4Ret == (INT32)E_IMG_OK)
    {
    		//printk("IMG_BsBufManager lock %d \n", current->pid);
    		u4hSemaphoreDBGInsert(u4hFillBufSema, 3+(ucImgId*0xf0));
        VERIFY (x_sema_lock(prImgKeepInfo->hFillBufSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        u4hSemaphoreDBGInsert(u4hFillBufSema, 4+(ucImgId*0xf0));
        prImgKeepInfo->u4CurState = IMG_DECODE;
    }

    return i4Ret;
}


INT32 IMG_BsGetData(UCHAR ucImgId,UINT32 u4DstAddr, UINT32 u4Offset, UINT32 u4Size)
{
    INT32 i4Ret;
    IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);

    prImgKeepInfo->u4CurState = IMG_BSBUF_UNDERFLOW;
    i4Ret = _IMG_BsGetData((IMGHANDLE)prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID , u4DstAddr, u4Offset, u4Size);
    if (i4Ret == (INT32)E_IMG_OK)
    {
    		//printk("IMG_BsGetData lock %d \n", current->pid);
    		u4hSemaphoreDBGInsert(u4hFillBufSema, 5+(ucImgId*0xf0));
        VERIFY (x_sema_lock(prImgKeepInfo->hFillBufSema, X_SEMA_OPTION_WAIT) == OSR_OK);
        u4hSemaphoreDBGInsert(u4hFillBufSema, 6+(ucImgId*0xf0));
        prImgKeepInfo->u4CurState = IMG_DECODE;
    }

    return i4Ret;
}

INT32 IMG_BsBufStartTrsData(UCHAR ucImgId,UINT32 u4TrsOfst)
{
    INT32 i4Ret = E_IMG_OK;

    IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
#ifndef CC_IMAGE_PNG_DISABLE
    IMG_LIB_INFO_T* prImg = (IMG_LIB_INFO_T*)prImgKeepInfo->hCurImg;
    PNG_INST_T* prPngInst = (PNG_INST_T*)prImg->hCurDec;
    LOG(7, "IMG_BsBufStartTrsData u4TrsOfst = 0x%08x\n",u4TrsOfst);

    if(prPngInst->u4ImgSize <  (PNG_FIFO_SIZE - 64))
    {    
        prPngInst->tFifoInfo.u4RdPoint = u4TrsOfst + prPngInst->tFifoInfo.u4FifoStart;
        prPngInst->tFifoInfo.u4PreRdPoint = prPngInst->tFifoInfo.u4RdPoint;
        prPngInst->tFifoInfo.u4ValidData = prPngInst->u4ImgSize - u4TrsOfst;
    }
    else
#endif
    {
        prImgKeepInfo->u4CurState = IMG_BSBUF_UNDERFLOW;
        i4Ret = _IMG_BsStartTrsData((IMGHANDLE)prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID ,u4TrsOfst);
        if (i4Ret == (INT32)E_IMG_OK)
        {
        		//printk("IMG_BsBufStartTrsData lock %d \n", current->pid);
        		u4hSemaphoreDBGInsert(u4hFillBufSema, 7+(ucImgId*0xf0));
            VERIFY (x_sema_lock(prImgKeepInfo->hFillBufSema, X_SEMA_OPTION_WAIT) == OSR_OK);
            u4hSemaphoreDBGInsert(u4hFillBufSema, 8+(ucImgId*0xf0));
            prImgKeepInfo->u4CurState = IMG_DECODE;
        }
    }
    return i4Ret;
}

/******************************************************************************
* Local	Function definition
******************************************************************************/
static void _ImgSetCmd(UCHAR ucImgId,UINT32 u4Cmd)
{
	INT32 i4Ret;
	UINT16 u2MsgQIdx;
	SIZE_T zMsgSize;
	UINT32 u4CmdDump;
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);

	i4Ret = x_msg_q_send(prImgKeepInfo->hCmdQueue, (void *)&u4Cmd, JPG_CMD_SIZE, 255);

	if (i4Ret == OSR_TOO_MANY)
	{
		zMsgSize = JPG_CMD_SIZE;
		VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4CmdDump, &zMsgSize,
							   &(prImgKeepInfo->hCmdQueue), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK);
		i4Ret = x_msg_q_send(prImgKeepInfo->hCmdQueue, (void *)&u4Cmd, JPG_CMD_SIZE, 255);
	}
	else
	{
		VERIFY(i4Ret == OSR_OK);
	}

	LOG(5, "Img Set command id=%d cmd=%x\n", ucImgId, u4Cmd);

}


static void _ImgReceiveCmd(UCHAR ucImgId,BOOL bIsBlock)
{
	UINT16 u2MsgQIdx;
	SIZE_T zMsgSize;
	UINT32 u4Cmd;
	INT32 i4Ret;
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);

	zMsgSize = JPG_CMD_SIZE;    
	if (bIsBlock)
	{
		i4Ret = x_msg_q_receive(&u2MsgQIdx, &u4Cmd, &zMsgSize,
								&(prImgKeepInfo->hCmdQueue), 1, X_MSGQ_OPTION_WAIT);
	}
	else
	{
		i4Ret = x_msg_q_receive(&u2MsgQIdx, &u4Cmd, &zMsgSize,
								&(prImgKeepInfo->hCmdQueue), 1, X_MSGQ_OPTION_NOWAIT);
		if (i4Ret == OSR_NO_MSG)
		{
			return;
		}
	}

    if (i4Ret == OSR_OK)
    {
        ASSERT(zMsgSize == JPG_CMD_SIZE);
	}

	//filter illegal command
	switch (u4Cmd)
	{
	case IMG_STOP:
		if (prImgKeepInfo->u4CurState == IMG_DECODE)
		{
			prImgKeepInfo->u4NotifyType= JPG_HW_NFY_STOP_DONE;
		}
		else
		{
			prImgKeepInfo->u4NotifyType = JPG_HW_NFY_ALREADY_STOPPED;
		}
		prImgKeepInfo->u4CurState = IMG_STOP;
		return;
		//break;
	case IMG_INIT:
		if (prImgKeepInfo->u4CurState == IMG_RELEASE)
		{
			prImgKeepInfo->u4CurState = IMG_INIT;
			return;
		}
		break;
	case IMG_DECODE:
		if (prImgKeepInfo->u4CurState == IMG_INIT)
		{
			prImgKeepInfo->u4CurState = IMG_DEC_PREPARE;
			return;
		}
		break;
	case IMG_SET:
		if (prImgKeepInfo->u4CurState == IMG_IDLE)
		{
			prImgKeepInfo->u4CurState = IMG_SET;
			return;
		}
		break;
	case IMG_RELEASE:
		prImgKeepInfo->u4CurState = IMG_RELEASE;
		return;
		//break;
	default:
		break;
	}

	if (prImgKeepInfo->u4CurState == IMG_DECODE)
	{
		_ImgReceiveCmd(ucImgId,FALSE);
	}
	else
	{
		_ImgReceiveCmd(ucImgId,TRUE);
	}
}



static void _ImgSendData(HANDLE_T hDataQ, UINT32 u4DataSize, UINT32 u4DataAddr)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    void *pvDataDump;

    i4Ret = x_msg_q_send(hDataQ, (void *)u4DataAddr, u4DataSize, 255);

    if (i4Ret == OSR_TOO_MANY)
    {
        zMsgSize = u4DataSize;
        pvDataDump = x_mem_alloc(zMsgSize);
        VERIFY(x_msg_q_receive(&u2MsgQIdx, pvDataDump, &zMsgSize,
                               &hDataQ, 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK);
        x_mem_free(pvDataDump);
        i4Ret = x_msg_q_send(hDataQ, (void *)u4DataAddr, u4DataSize, 255);
    }
    else
    {
        VERIFY(i4Ret == OSR_OK);
    }
}


static void _ImgReceiveData(HANDLE_T hDataQ, UINT32 u4DataSize, void *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    zMsgSize = u4DataSize;

    i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                            &hDataQ, 1, X_MSGQ_OPTION_WAIT);

    if (i4Ret == OSR_OK)
    {
        ASSERT(zMsgSize == u4DataSize);
    }
}


static void _ImgMainLoop(void* pvArg)
{
    IMG_PARAM_T rInfo;
    IMG_BUF_PARAM_T rCmd;
    IMG_LIB_INFO_T* prImg=NULL;
    INT32 i4Ret;
	IMG_INFO_KEEP_T* prImgKeepInfo;
	UCHAR ucImgId;
    IMG_REGION_T rDstRegion;
#ifdef CC_FUJI3D_SUPPORT
    UINT32 u4IsSupportedMPF, u4MPFOffset;
#endif
    IMG_INFO_T *prImgInfo = _IMG_GetInfo();
    if (pvArg == NULL)
    {
        ASSERT(!pvArg);
        ucImgId = 0;
        LOG(1, "_ImgMainLoop(): (pvArg == NULL)\n");
    }
    else
    {
        ucImgId = *(UCHAR*)pvArg;
    }

    UNUSED(pvArg);
	prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
    x_memset((void *)&rInfo, 0, sizeof(IMG_PARAM_T));
    x_memset((void *)&rCmd, 0, sizeof(IMG_BUF_PARAM_T));

	while (!prImgInfo->bImgModEnd)
    {
        if (prImgKeepInfo->u4CurState == IMG_DECODE)
        {
            _ImgReceiveCmd(ucImgId,FALSE);
        }
        else
        {
            _ImgReceiveCmd(ucImgId,TRUE);
        }
        
        //implementation for each state
        switch (prImgKeepInfo->u4CurState)
        {
        case IMG_INIT:
        	  u4hSemaphoreDBGInsert(u4hInitSema, 3+(ucImgId*0xf0));
			VERIFY (x_sema_unlock(prImgKeepInfo->hInitSema) == OSR_OK);
            u4hSemaphoreDBGInsert(u4hInitSema, 4+(ucImgId*0xf0));
            break;
        case IMG_IDLE:
            break;
        case IMG_DEC_PREPARE:

            prImg=(IMG_LIB_INFO_T*)prImgKeepInfo->hCurImg;

            if (prImgKeepInfo->hCurImg==NULL)
            {
                prImgKeepInfo->u4CurState=IMG_RELEASE;
                break;
            }
			
			
	    if (prImgKeepInfo->eImgType==IMAGE_WEBP)
            {
              prImg->bUseMw=FALSE;
            }
			
            prImgKeepInfo->u4CurState = IMG_DECODE;
            //receive data
            _ImgReceiveData(prImgKeepInfo->hDecDataQueue, sizeof(IMG_PARAM_T), (void *)&rInfo);

            //check parameter
            if (prImgKeepInfo->pfnImgCheckDecParam)
            {
                if (prImgKeepInfo->pfnImgCheckDecParam(prImgKeepInfo->hCurImg, &rInfo)!= (INT32)E_IMG_OK)
                {
                    //notify error
                    _IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID, IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
                    _IMG_Release(ucImgId,&prImgKeepInfo->hCurImg);
                    prImgKeepInfo->u4CurState = IMG_RELEASE;
                    break;
                }
            }
            LOG(5, "fgDrvFeedData:%d\n", rInfo.fgDrvFeedData);
            if((prImg->bUseMw)&&(rInfo.fgDrvFeedData)) //call IMG_SetUseMw(TRUE) in nav_viewer.c before decode.
                                                         //libjpeg case, don't need feed data in prepare state again
            {
                prImgKeepInfo->u4CurState = IMG_BSBUF_UNDERFLOW;
                if (_IMG_BsBufManager(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID )!= (INT32)E_IMG_OK)
                {
                    //notify error
                    _IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID, IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
                    _IMG_Release(ucImgId,&prImgKeepInfo->hCurImg);
                    prImgKeepInfo->u4CurState = IMG_RELEASE;
                }
                else
                {
                    //if (prImg->u1MemClass != (UINT8)EXTERNAL_RING_IN_DOUBLE_OUT)
                    {
            		//printk("_ImgMainLoop 1 lock %d \n", current->pid);
            		u4hSemaphoreDBGInsert(u4hFillBufSema, 9+(ucImgId*0xf0));
                        VERIFY (x_sema_lock(prImgKeepInfo->hFillBufSema, X_SEMA_OPTION_WAIT) == OSR_OK);
                        u4hSemaphoreDBGInsert(u4hFillBufSema, 10+(ucImgId*0xf0));
                    }

    		    if(prImgKeepInfo->bIMG_FillDataTimeout==TRUE)
       		    {
       			break;
       		    }
	
                    prImgKeepInfo->u4CurState = IMG_DECODE;
                }
            }
            

            /*prevent state have switched to release*/
    		prImgKeepInfo->bNeedToUnlockHwSemaWhenRelease = TRUE;
            
            u4hSemaphoreDBGInsert(u4hardwareSema, 0+(ucImgId*0xf0));
            IMG_LockHwResource(ucImgId);
            u4hSemaphoreDBGInsert(u4hardwareSema, 1+(ucImgId*0xf0));

            //--- header parsing and decoding setting prepare
            if (prImgKeepInfo->pfnImgDecPrepare)
            {
                i4Ret = prImgKeepInfo->pfnImgDecPrepare(prImgKeepInfo->hCurImg, &rInfo);
            }
            else
            {
                i4Ret = E_IMG_NULL_HANDLER;
            }
            if (i4Ret != (INT32)E_IMG_OK)
            {
				if (i4Ret == (INT32)E_IMG_FMT_UNSUPPORT)
				{
					//notify error
					_IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID , IMG_DRV_NOT_SUPPORT, JPG_HW_NFY_ERROR);
				}
#ifdef __KERNEL__
				else if (i4Ret == (INT32)E_IMG_ERR_COMPONENT)
				{
					//notify error	//code sync
					_IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID , IMG_DRV_ERR_COMPONENT, JPG_HW_NFY_ERROR);
				}
#endif
				else if (i4Ret == (INT32)E_IMG_AVI1_UNSUPPORT)
				{
					_IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID , IMG_DRV_AVI1_NOT_SUPPORT, JPG_HW_NFY_ERROR);
				}
				else
				{
					_IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID , IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
				}
                _IMG_Release(ucImgId,&prImgKeepInfo->hCurImg);
                //u4hSemaphoreDBGInsert(u4hardwareSema, 2+(ucImgId*0xf0));
				//IMG_UnLockHwResource(ucImgId);
				//u4hSemaphoreDBGInsert(u4hardwareSema, 3+(ucImgId*0xf0));
                prImgKeepInfo->u4CurState = IMG_RELEASE;
            }
            else
            {
                /*prevent state have switched to release*/
                //_bNeedToUnlockHwSemaWhenRelease = TRUE;
            }

            break;
        case IMG_DECODE:

			//_bNeedToUnlockHwSemaWhenRelease = FALSE;
			prImgKeepInfo->bNeedToUnlockHwSemaWhenRelease = FALSE;

            prImg = (IMG_LIB_INFO_T*)prImgKeepInfo->hCurImg;
            if (prImgKeepInfo->hCurImg == NULL)   //for lint happy
            {
                prImgKeepInfo->u4CurState = IMG_RELEASE;
                u4hSemaphoreDBGInsert(u4hardwareSema, 4+(ucImgId*0xf0));
				IMG_UnLockHwResource(ucImgId); 
				u4hSemaphoreDBGInsert(u4hardwareSema, 5+(ucImgId*0xf0));
                break;
            }
            else
            {
                if (prImg->bUseMw)
                {
					prImgKeepInfo->u4CurState = IMG_BSBUF_UNDERFLOW;
                }
            }

            if (prImgKeepInfo->pfnImgDecOneSegment)
            {
                i4Ret = prImgKeepInfo->pfnImgDecOneSegment(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID , &rInfo);
            }
            else
            {
                i4Ret = E_IMG_NULL_HANDLER;
            }
            if (i4Ret == (INT32)E_IMG_DEC_FINISH)
            {
            	if (prImg->bUseMw)
                {
                    prImgKeepInfo->u4CurState = IMG_IDLE;
                }
				
                #ifdef CC_FUJI3D_SUPPORT
                //UINT32 u4IsSupportedMPF, u4MPFOffset;
                IMG_GetMPOInfo(ucImgId, &u4IsSupportedMPF, &u4MPFOffset);
                if(u4IsSupportedMPF)
                {
                    _IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID , u4MPFOffset, JPG_HW_NFY_FINISHED);
                }
                else
                #endif
                {
                //notify decoding done
                _IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID , 0, JPG_HW_NFY_FINISHED);
                }
                if (!prImg->bUseMw)
                {
                    _IMG_Release(ucImgId,&prImgKeepInfo->hCurImg);
                    prImgKeepInfo->u4CurState = IMG_RELEASE;
                }
                u4hSemaphoreDBGInsert(u4hardwareSema, 6+(ucImgId*0xf0));
				IMG_UnLockHwResource(ucImgId);
				u4hSemaphoreDBGInsert(u4hardwareSema, 7+(ucImgId*0xf0));
            }
            else if (i4Ret == (INT32)E_IMG_BSBUF_UNDERFLOW)
            {
                prImgKeepInfo->u4CurState = IMG_BSBUF_UNDERFLOW;
                //printk("_ImgMainLoop 2 lock %d \n", current->pid);
                u4hSemaphoreDBGInsert(u4hFillBufSema, 11+(ucImgId*0xf0));
                VERIFY (x_sema_lock(prImgKeepInfo->hFillBufSema, X_SEMA_OPTION_WAIT) == OSR_OK);
                u4hSemaphoreDBGInsert(u4hFillBufSema, 12+(ucImgId*0xf0));
                prImgKeepInfo->u4CurState = IMG_DECODE;
            }
            else if (i4Ret!=(INT32)E_IMG_OK)
            {
                //notify error
                LOG(3,"Decode error type = %d\n",i4Ret);
                if (i4Ret == E_IMG_CANCEL)
                {
                    _IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID ,  0, JPG_HW_NFY_STOP_DONE);
                }
                else if (i4Ret == E_IMG_HWBUSY)
                {
                    _IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID ,  IMG_DRV_BUSY, JPG_HW_NFY_ERROR);
                }
                else
                {
                    _IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID ,  IMG_DRV_HW_ERROR, JPG_HW_NFY_ERROR);
                }

                if (prImg->bUseMw)
                {
                    prImgKeepInfo->u4CurState = IMG_IDLE;
                }
                else
                {
					_IMG_Release(ucImgId,&prImgKeepInfo->hCurImg);
					prImgKeepInfo->u4CurState = IMG_RELEASE;
                }
                u4hSemaphoreDBGInsert(u4hardwareSema, 8+(ucImgId*0xf0));
				IMG_UnLockHwResource(ucImgId);
				u4hSemaphoreDBGInsert(u4hardwareSema, 9+(ucImgId*0xf0));
            }
            else
            {
				prImgKeepInfo->u4CurState = IMG_DECODE;
            }
            break;
        case IMG_SET:
            //receive data
            _ImgReceiveData(prImgKeepInfo->hSetDataQueue, sizeof(IMG_BUF_PARAM_T), (void *)&rCmd);
            _IMG_Set(rCmd, &rDstRegion);
            _IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID , 0, JPG_HW_NFY_FINISHED);
            prImgKeepInfo->u4CurState = IMG_IDLE;
            break;
        case IMG_STOP:
            _IMG_NotifyStatus(prImgKeepInfo->hCurImg, prImgKeepInfo->u4ImgID , 0, prImgKeepInfo->u4NotifyType);
            prImgKeepInfo->u4NotifyType = JPG_HW_NFY_NONE;
            break;
        case IMG_RELEASE:
            _IMG_Release(ucImgId,&prImgKeepInfo->hCurImg);
            u4hSemaphoreDBGInsert(u4hReleaseSema, 3+(ucImgId*0xf0));
            VERIFY (x_sema_unlock(prImgKeepInfo->hReleaseSema) == OSR_OK);
            u4hSemaphoreDBGInsert(u4hReleaseSema, 4+(ucImgId*0xf0));
            break;
        default:
            break;
        }
    }
}

IMG_INFO_T* _IMG_GetInfo(void)
{
    return (IMG_INFO_T*)&_rImgInfo;
}

IMG_INFO_KEEP_T* _IMG_GetInfoKeep(UCHAR ucImgId)
{
    if (ucImgId >= IMG_MAX)
    {
        ucImgId = 0;
    }
    return &(_rImgInfo.arImgInfoKeep[ucImgId]);
}

INT32 IMG_Config(UCHAR ucImgId, ENUM_IMG_FORMAT img_type)
{
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
	
	switch (img_type)
	{
#ifndef CC_IMAGE_PNG_DISABLE
	case IMAGE_PNG:
		prImgKeepInfo->pfnImgInit = _PNG_Init;
		prImgKeepInfo->pfnImgDecPrepare = _PNG_DecPrepare;
		prImgKeepInfo->pfnImgDecOneSegment = _PNG_DecOneSegment;
		prImgKeepInfo->pfnImgBsBufManager = _IMG_BsBufManager;
		prImgKeepInfo->pfnImgSetFillBsBufEnd = _PNG_SetFillBsBufEnd;
		prImgKeepInfo->pfnImgCheckDecParam = NULL;
		prImgKeepInfo->pfnImgStop = _PNG_Stop;

		prImgKeepInfo->pfnImgMwFillBuffer = PNG_MwFillBuffer;
		prImgKeepInfo->pfnImgSetFileSize = PNG_SetFileSize;
		prImgKeepInfo->pfnImgFreeInstance = PNG_FreeInstance;
		prImgKeepInfo->pfnImgGetInfo=NULL;        
#ifdef CC_JPEG_FBM_DYNAMIC_MAPPING
                prImgKeepInfo->pfnImgUnmap = _PNG_Unmap;
#else
                prImgKeepInfo->pfnImgUnmap = NULL;
#endif
		break;
#endif

#ifndef CC_IMAGE_WEBP_DISABLE
	case IMAGE_WEBP:
		prImgKeepInfo->pfnImgInit = _WEBP_Init;
		prImgKeepInfo->pfnImgDecPrepare = _WEBP_DecPrepare;
		prImgKeepInfo->pfnImgDecOneSegment = _WEBP_DecOneSegment;
		prImgKeepInfo->pfnImgBsBufManager = _WEBP_BsBufManager;
		prImgKeepInfo->pfnImgSetFillBsBufEnd = _WEBP_SetFillBsBufEnd;
		prImgKeepInfo->pfnImgCheckDecParam = _WEBP_CheckDecParam;
		prImgKeepInfo->pfnImgStop = _WEBP_Stop;
		prImgKeepInfo->pfnImgMwFillBuffer = _WEBP_MwFillBuffer;
		prImgKeepInfo->pfnImgSetFileSize = _WEBP_SetFileSize;
		prImgKeepInfo->pfnImgFreeInstance = _WEBP_FreeInstance;
		prImgKeepInfo->pfnImgGetInfo=_WEBP_GetDecInfo;
		#ifdef CC_JPEG_FBM_DYNAMIC_MAPPING
                prImgKeepInfo->pfnImgUnmap=_WEBP_UnmapFbm;
		#else
                prImgKeepInfo->pfnImgUnmap=NULL;
		#endif
		break;
#endif
	case IMAGE_JPEG:
	case IMAGE_PROGRESSIVE_JPEG:    	
    case IMAGE_MPO:	    
		prImgKeepInfo->pfnImgInit = _IMG_Init;
		prImgKeepInfo->pfnImgDecPrepare = _IMG_DecPrepare;
		prImgKeepInfo->pfnImgDecOneSegment = _IMG_DecOneSegment;
		prImgKeepInfo->pfnImgBsBufManager = _IMG_BsBufManager;
		prImgKeepInfo->pfnImgSetFillBsBufEnd = _IMG_SetFillBsBufEnd;
		prImgKeepInfo->pfnImgCheckDecParam = _IMG_CheckDecParam;
		prImgKeepInfo->pfnImgStop = _IMG_Stop;
		prImgKeepInfo->pfnImgGetInfo=_IMG_GetDecInfo;
		prImgKeepInfo->pfnImgMwFillBuffer = JDEC_MwFillBuffer;
		prImgKeepInfo->pfnImgSetFileSize = JDEC_SetFileSize;
		prImgKeepInfo->pfnImgFreeInstance = JDEC_FreeInstance;
#ifdef CC_JPEG_FBM_DYNAMIC_MAPPING
                prImgKeepInfo->pfnImgUnmap = _JPG_UnmapFbm;
#else
                prImgKeepInfo->pfnImgUnmap = NULL;
#endif
		break;
	default:
		return -1;
	}
	
	prImgKeepInfo->eImgType=img_type;
	return 0;
}


HANDLE_T IMG_GetPngSema(UCHAR ucImgId)
{
#ifndef CC_IMAGE_PNG_DISABLE
    IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
    return (HANDLE_T)prImgKeepInfo->hPngSema;
#else
    return (HANDLE_T)NULL;
#endif
}


ENUM_IMG_FORMAT IMG_GetImageType(UCHAR ucImgId)
{
	IMG_INFO_KEEP_T* prImgKeepInfo = _IMG_GetInfoKeep(ucImgId);
    return prImgKeepInfo->eImgType;
}

#ifdef CC_SUPPORT_STR
void Image_pm_str_suspend(void)
{
    //UINT32 u4Ret;
	if(VLDJPG_HwIsBusy())
	{
		LOG(0, "STR_WARNING: JPG is not stopped\n");	
	    VLDJPG_SetPowerOff();
	}
	#ifndef CC_IMAGE_PNG_DISABLE
	#if !defined(CC_MT5881)
	if(fgPngHalIsBusy(0))
	{
		LOG(0, "STR_WARNING: PNG is not stopped\n");
		i4PngHalDisable(0);
	}
        i4PngHalCkGen(FALSE, 0);
        #endif
  #endif
    JDEC_CkgenOnOff(FALSE);
}


void Image_pm_str_resume(void)
{
    #ifndef CC_IMAGE_PNG_DISABLE
	#if !defined(CC_MT5881)
    i4PngHalCkGen(TRUE, 0);
  #endif
  #endif
    JDEC_CkgenOnOff(TRUE);
}
#endif /*CC_SUPPORT_STR*/

void _ImgHWPowerTurnOff(void)
{
    VLDJPG_SetPowerOff();
}
#ifdef __KERNEL__
static UINT32 _u4DFBPhyAddr = 0;
static UINT32 _u4DFBVirAddr = 0;
static UINT32 _u4DFBSize = 0;
#endif
#if 0
void IMG_SeDirectFBMem(UINT32 u4PhyAddr, UINT32 u4VirAddr,UINT32 u4Size)
{
#ifdef __KERNEL__
      _u4DFBPhyAddr = u4PhyAddr;
      _u4DFBVirAddr = u4VirAddr;
      _u4DFBSize = u4Size;
#endif
      return;
}
#endif
UINT32 VIRTUAL_DFB(UINT32 addr)
{
#ifdef __KERNEL__
    UINT32 u4FbmAddr,u4FbmSize;
    UINT32 u4Ret;
#if defined(CC_DYNAMIC_FBMSRM_CONF)
    #if defined(CC_MT5890)//for oryx, there are 3 channels. jpg_vdp in chB, and jpg in chA.
    UINT32 u4FbmBAddr, u4FbmBSize,u4FbmCAddr, u4FbmCSize;

    u4FbmBAddr = DRAM_CHB_START_ADDR + BSP_GetChBMemSize() - SRMFBM_CHB_GetConf() - TRUSTZONE_MEM_SIZE;
    u4FbmBSize = SRMFBM_CHB_GetConf();
    u4FbmCAddr = DRAM_CHC_START_ADDR;
    u4FbmCSize = SRMFBM_CHC_GetConf();        
    #endif
    u4FbmAddr = BSP_GetMemSize()- SRMFBM_GetConf();
    u4FbmSize = SRMFBM_GetConf();
#else
     #if defined(CC_MT5890)     
     UINT32 u4FbmBAddr, u4FbmBSize,u4FbmCAddr, u4FbmCSize;
     
     u4FbmCAddr = 0;
     u4FbmCSize = 0;
     if(IS_IC_5861())
     {
         u4FbmBAddr = DRVCUST_OptGet(eFbmMemChBStart);
         u4FbmBSize = DRVCUST_OptGet(eFbmMemChBSize);     
     }
     else
     {
        u4FbmBAddr = DRVCUST_OptGet(eFbmMemChBStart);
        u4FbmBSize = DRVCUST_OptGet(eFbmMemChBSize);
        u4FbmCAddr = DRVCUST_OptGet(eFbmMemChCStart);
        u4FbmCSize = DRVCUST_OptGet(eFbmMemChCSize);  
     }          
    #endif 
    
    u4FbmAddr = DRVCUST_OptGet(eFbmMemAddr);
    u4FbmSize = DRVCUST_OptGet(eFbmMemSize);
#endif
    u4Ret = 0;
    if((addr >= u4FbmAddr) && (addr <u4FbmAddr+u4FbmSize))
    {
        u4Ret = VIRTUAL(addr);
    }
    #if defined(CC_MT5890)
    else if(IS_IC_5861() )
    {        
        if((addr >= u4FbmBAddr) && (addr < u4FbmBAddr +u4FbmBSize))
        {
            u4Ret = VIRTUAL(addr);
        }
    }
    else if(((addr >= u4FbmBAddr) && (addr < u4FbmBAddr +u4FbmBSize))||((addr >= u4FbmCAddr) &&(addr < u4FbmCAddr + u4FbmCSize)))
    {
        u4Ret = VIRTUAL(addr);
    }
    #endif
    else if((addr >= _u4DFBPhyAddr) && (addr < _u4DFBPhyAddr+_u4DFBSize))
    {
        u4Ret = (addr - _u4DFBPhyAddr + _u4DFBVirAddr);
    }
    else
    {
        LOG(0,"The dst addr not a DFB FBM addr!!!\n");
        u4Ret =  0;
    }
    return u4Ret;
#else
     return addr;
#endif
}

