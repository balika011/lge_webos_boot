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
 * $Author: p4admin $
 * $Date: 2015/01/20 $
 * $RCSfile: musb_if.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file musb_if.c
 *  MUSB module api function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifndef CC_USB_DISABLE
#include "x_lint.h"

#if defined(CONFIG_ARCH_MT85XX)
#include "x_debug.h"
#include "drv_config.h"
#include "x_gpio.h"
#else
#ifdef CC_SUPPORT_STR
#include "x_hal_5381.h"
#include "plat_arc.h"
#include "mu_hdrdf.h"
#include "mu_drcdf.h"
#include "x_hal_io.h"
#include "x_ckgen.h"
#endif
#include "x_pinmux.h"
#endif

LINT_EXT_HEADER_BEGIN

#include "mu_scsi.h"

#include "mu_cdi.h"
#include "mu_impl.h"
#include "mu_mapi.h"

#ifndef MUSB_LOADER
#ifdef MUSB_IPOD
#include "mu_hfi_ipod_if.h"
#include "mu_ipod.h"
#endif

#ifdef MUSB_SICD
#include "mu_hfi_sicd_if.h"
#include "mu_sicdstate.h"
#include "mu_sicdapi.h"
#include "mu_sicdutil.h"
#endif

#ifdef CONFIG_DRV_LINUX
#undef MUSB_WIFI
#endif

#ifdef MUSB_WIFI
#include "mu_wifi.h"
#endif

#ifdef MUSB_HUB
#include "mu_hapi.h"
#endif

#ifdef MUSB_SERIAL
#include "uart_if.h"
#include "mu_serial.h"
#endif
#ifdef MUSB_HID
#include "mu_hid_if.h"
#if defined(CONFIG_ARCH_MT85XX)
#include "mu_hfi_hid_if.h"
#endif
#endif

#ifdef MUSB_AUD
#include "mu_audio.h"
#endif

#ifdef MUSB_PL2303
#include "mu_pl2303api.h"
#endif

#ifdef CC_USB_CSR_BLUETOOTH
#include "mu_bt.h"
#endif
#endif /* #ifndef MUSB_LOADER */

#include "musb_if.h"
#include "mu_descs.h"
#include "mu_devfn.h"
#include "mu_cdc_if.h"

#include "x_timer.h"
#include "x_pdwnc.h"

#ifndef MUSB_NONEOS
#include "mu_sys.h"
#else
#include "mu_none.h"
#endif

#include "drvcust_if.h"
#include "x_hal_arm.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define MUSB_SLT_SIZE   ((UINT32)0x800)

#define MUSB_STATE_ACTIVE    (1)
#define MUSB_STATE_IDLE        (2)

#if !defined(CONFIG_ARCH_MT85XX)

#ifndef MUSB_LOADER
//#define MUSB_OC_SUPPORT
#else
#define MUSB_LOADER_VBUS_WAIT_TIME   (0) // ms.
#define MUSB_LOADER_MAX_WAIT_TIME   (2000) // ms.
#endif

#define MUSB_OC_MAX_COUNT (5)   // Debounce loop.

#define MUSB_MAX_DRV_NUM ((UINT8)5)

#else //#if !defined(CONFIG_ARCH_MT85XX)

#ifdef MUSB_LOADER
#define MUSB_LOADER_MAX_WAIT_TIME        (1000) // ms.
#endif

#ifdef MUSB_HUB
#define MUSB_HUB_DRIVER 1
#else
#define MUSB_HUB_DRIVER 0
#endif

#ifdef MUSB_SICD
#define MUSB_SICD_DRIVER 1
#else
#define MUSB_SICD_DRIVER 0
#endif

#ifdef MUSB_PL2303
#define MUSB_PL2303_DRIVER 1
#else
#define MUSB_PL2303_DRIVER 0
#endif

#ifdef MUSB_WIFI
#define MUSB_WIFI_DRIVER 3
#else
#define MUSB_WIFI_DRIVER 0
#endif

#ifdef MUSB_AUD
#define MUSB_AUD_DRIVER 1
#else
#define MUSB_AUD_DRIVER 0
#endif

#ifdef MUSB_HID
#define MUSB_HID_DRIVER 1
#else
#define MUSB_HID_DRIVER 0
#endif

#ifdef CC_USB_CSR_BLUETOOTH
#define MUSB_BT_DRIVER 1
#else
#define MUSB_BT_DRIVER 0
#endif

#ifndef MUSB_LOADER
#define MUSB_OC_SUPPORT
#else
#define MUSB_LOADER_MAX_WAIT_TIME        (1000) // ms.
#endif

#ifdef MUSB_IPOD
#define MUSB_IPOD_CLIENT_DRIVER 1
#else
#define MUSB_IPOD_CLIENT_DRIVER 0
#endif

#define MUSB_OC_MAX_COUNT (5)   // Debounce loop.

#define MUSB_MAX_DRV_NUM (1+MUSB_HUB_DRIVER+MUSB_SICD_DRIVER+MUSB_PL2303_DRIVER+MUSB_WIFI_DRIVER+MUSB_AUD_DRIVER+MUSB_HID_DRIVER+MUSB_BT_DRIVER+MUSB_IPOD_CLIENT_DRIVER)

#endif // #if !defined(CONFIG_ARCH_MT85XX)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------
#ifdef MUSB_FUNCTION

extern MUSB_FunctionClient MGC_MsdFunctionClient;

extern MUSB_FunctionClient MGC_CdcFunctionClient;
extern MUSB_FunctionClient MGC_GenericUsbFunctionClient;

#endif


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static BOOL _fgMUSBInit = FALSE;
#if !defined(CONFIG_ARCH_MT85XX)
static BOOL _fgMUSBHostMode = TRUE;
static BOOL _fgMUSBDrvcustInit = FALSE;
static BOOL _fgMUSBVBUSEnable = 0xFF; /* For first time to init */
#endif

static MUSB_Port *MGC_pUsbPort[MUSB_MAX_CONTROLLERS];
static MGC_Port *MGC_pImplPort[MUSB_MAX_CONTROLLERS];

static UINT8 MGC_aUsbPeripheralList[25*MUSB_MAX_DRV_NUM];

/*
Class device driver entry.
*/
static MUSB_DeviceDriver MGC_aDeviceDriver[MUSB_MAX_DRV_NUM];

static MUSB_HostClient MGC_UsbHostClient =
{
    MGC_aUsbPeripheralList, /* peripheral list */
    0,                      /* filled in main */
    MGC_aDeviceDriver,
    0,                      /* filled in main */
    0,                      /* dwHostError */
    NULL,                /* void* pPrivateData; may be NULL in callback function */
    NULL                 /* MUSB_pfHostError pfHostError */
};

static MUSB_IO_DATA_T _rMUSBData;
static MUSB_NFY_INFO_T _rMUSBAsynInfo;
static UINT32 _u4MUSBLun = 0;
static BOOL _fgMUSBLockInit = FALSE;

#if defined(CONFIG_ARCH_MT85XX)
static BOOL _afgMUSBLockInit[USB_DIAG_MAX_LUN_NUM] = {0};
static MUSB_NFY_INFO_T arMUSBAsynInfo[USB_DIAG_MAX_LUN_NUM] = {{0}, {0}};
static MUSB_IO_DATA_T arMUSBData[USB_DIAG_MAX_LUN_NUM] = {{0}, {0}, {0}, {0}, {0}};
#endif

#ifndef MUSB_NONEOS    
static HANDLE_T _hMUSBLock;
#if defined(CONFIG_ARCH_MT85XX)
static HANDLE_T _ahMUSBLock[USB_DIAG_MAX_LUN_NUM];
#endif
#endif

// mass storage host plug callback function.
static UINT32 _u4MUSBCallbackTag[MUSB_UCFS_UNIT_COUNT];
static MUSB_CALLBACK _pfnMUSBCallback[MUSB_UCFS_UNIT_COUNT];
static MUSB_HOTSWAP_NFY_INFO_T _rMusbMediumInfo;

static MBlockFunc_T _rMUSBBlkFuncTbl =
{
    NULL,
    MUSB_GetLun,
    MUSB_SetLun,
    MUSB_GetSize,
    MUSB_GetBlockSize,
    MUSB_GetStatus,
    MUSB_GetModel,
    NULL,
    MUSB_Read,
    MUSB_Write,
    MUSB_SetHotswapCallback,
#if !defined(CONFIG_ARCH_MT85XX)
    MUSB_GetInfo
#endif
};

#ifndef MUSB_8226
static HAL_TIME_T _rMsdTimeStart;
static HAL_TIME_T _rMsdTimeEnd;
static HAL_TIME_T _rMsdTimeDelta;
static HAL_TIME_T _rMsdTimeTotal;

#if defined(CONFIG_ARCH_MT85XX)
static HAL_TIME_T _arMsdTimeStart[MUSB_UCFS_UNIT_COUNT];
static HAL_TIME_T _arMsdTimeEnd[MUSB_UCFS_UNIT_COUNT];
static HAL_TIME_T _arMsdTimeDelta[MUSB_UCFS_UNIT_COUNT];
static HAL_TIME_T _arMsdTimeTotal[MUSB_UCFS_UNIT_COUNT];
#endif

#endif

#if !defined(CONFIG_ARCH_MT85XX)
#ifdef MUSB_OC_SUPPORT
/* MUSB over current setting */
static BOOL _fgMUSBOCEnable = FALSE;
static HANDLE_T _hMUSBOCTimerHdl;
static HANDLE_T phMUSBOCThreadHdl;
static HANDLE_T phMUSBOCSemaHdl;
static INT32 _i4MUSBOCGpio[MUSB_MAX_CONTROLLERS];
static INT32 _i4MUSBOCPolarity[MUSB_MAX_CONTROLLERS];
static BOOL _fgMUSBOCStatus[MUSB_MAX_CONTROLLERS];

static const QUERY_TYPE_T _i4MUSBOCGpioDef[MUSB_MAX_CONTROLLERS] =
{
    eUSB0OCGpio,
    eUSB1OCGpio
};
static const QUERY_TYPE_T _i4MUSBOCEnablePolarityDef[MUSB_MAX_CONTROLLERS] =
{
    eUSB0OCEnablePolarity,
    eUSB1OCEnablePolarity
};
#endif /* #ifdef MUSB_OC_SUPPORT */
#endif //#if !defined(CONFIG_ARCH_MT85XX)

#ifdef MUSB_LOADER

static HAL_TIME_T _MusbT0, _MusbT1, _MusbDt;
static HAL_TIME_T _MusbLockT0, _MusbLockT1, _MusbLockDt;
#endif /* #ifdef MUSB_LOADER */

#if !defined(CONFIG_ARCH_MT85XX)
static INT32 _i4MUSBVbusGpio[MUSB_MAX_CONTROLLERS];
static INT32 _i4MUSBVbusPolarity[MUSB_MAX_CONTROLLERS];
static BOOL _fgMUSBVbusStatus[MUSB_MAX_CONTROLLERS];
static QUERY_TYPE_T _i4MUSBVbusGpioDef[MUSB_MAX_CONTROLLERS];
static QUERY_TYPE_T _i4MUSBVbusEnablePolarityDef[MUSB_MAX_CONTROLLERS];

#ifdef MUSB_PORT_CONFIG
static QUERY_TYPE_T _i4MUSBPortConfig[MUSB_MAX_CONTROLLERS];
#endif
#endif //#if !defined(CONFIG_ARCH_MT85XX)

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** _Musb_InitLock.
*   read/write sync initialization.
*  @param  void.
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
static INT32 _Musb_InitLock(void)
{
    INT32 i4Ret = 0;

#ifdef MUSB_NONEOS
    _fgMUSBLockInit = MUSB_STATE_ACTIVE;

#else
    if (!_fgMUSBLockInit)
    {
        _fgMUSBLockInit = TRUE;

        i4Ret = x_sema_create((HANDLE_T *)&_hMUSBLock, X_SEMA_TYPE_BINARY,
                                   X_SEMA_STATE_LOCK);
        if (OSR_OK != i4Ret)
        {
            return i4Ret;
        }
    }

    // make sure semphore is in lock state.
    i4Ret = x_sema_lock(_hMUSBLock, X_SEMA_OPTION_NOWAIT);
    if ((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK))
    {
        i4Ret = 0;
    }
#endif

#ifndef MUSB_8226
    HAL_GetTime(&_rMsdTimeStart);
#endif

    return i4Ret;
}

#if defined(CONFIG_ARCH_MT85XX)
INT32 Musb_InitLockEx(UINT32 u4Lun)
{
    INT32 i4Ret = 0;

    if(u4Lun >= USB_DIAG_MAX_LUN_NUM) return -1;
    
    if (!_afgMUSBLockInit[u4Lun])
    {
        _afgMUSBLockInit[u4Lun] = TRUE;

        i4Ret = x_sema_create((HANDLE_T *)&_ahMUSBLock[u4Lun], X_SEMA_TYPE_BINARY,
                                   X_SEMA_STATE_LOCK);
        if (OSR_OK != i4Ret)
        {
            return i4Ret;
        }
    }

    // make sure semphore is in lock state.
    i4Ret = x_sema_lock(_ahMUSBLock[u4Lun], X_SEMA_OPTION_NOWAIT);
    if ((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK))
    {
        i4Ret = 0;
    }    

#ifndef MUSB_8226
    HAL_GetTime(&_arMsdTimeStart[u4Lun]);
#endif

    return i4Ret;
}
#endif //#if defined(CONFIG_ARCH_MT85XX)

//-------------------------------------------------------------------------
/** _Musb_Unlock.
*   read/write unlock sync.
*  @param  void.
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
static INT32 _Musb_Unlock(void)
{
    INT32 i4Ret = 0;

#ifndef MUSB_8226
    HAL_GetTime(&_rMsdTimeEnd);
    HAL_GetDeltaTime(&_rMsdTimeDelta, &_rMsdTimeStart, &_rMsdTimeEnd);
    _rMsdTimeTotal.u4Seconds += _rMsdTimeDelta.u4Seconds;
    _rMsdTimeTotal.u4Micros  += _rMsdTimeDelta.u4Micros;
#endif

#ifdef MUSB_NONEOS
    _fgMUSBLockInit = MUSB_STATE_IDLE;

#else

    i4Ret = x_sema_unlock(_hMUSBLock);

#endif

    return i4Ret;
}

#if defined(CONFIG_ARCH_MT85XX)
INT32 Musb_UnlockEx(UINT32 u4Lun)
{
    INT32 i4Ret = 0;

    if(u4Lun >= MUSB_UCFS_UNIT_COUNT) return -1;

#ifndef MUSB_8226
    HAL_GetTime(&_arMsdTimeEnd[u4Lun]);
    HAL_GetDeltaTime(&_arMsdTimeDelta[u4Lun], &_arMsdTimeStart[u4Lun], &_arMsdTimeEnd[u4Lun]);
    _arMsdTimeTotal[u4Lun].u4Seconds += _arMsdTimeDelta[u4Lun].u4Seconds;
    _arMsdTimeTotal[u4Lun].u4Micros  += _arMsdTimeDelta[u4Lun].u4Micros;               
#endif

    if(u4Lun<USB_DIAG_MAX_LUN_NUM)
        i4Ret = x_sema_unlock(_ahMUSBLock[u4Lun]);

    return i4Ret;
}
#endif //#if defined(CONFIG_ARCH_MT85XX)

//-------------------------------------------------------------------------
/** _Musb_Lock.
*   read/write wait lock sync.
*  @param  void.
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
static INT32 _Musb_Lock(void)
{
    INT32 i4Ret = 0;

#ifdef MUSB_NONEOS
#if defined(CONFIG_ARCH_MT85XX)
    while (_fgMUSBLockInit != MUSB_STATE_IDLE)
    {
        MUSB_NoneRunBackground();
    }

#else //#if defined(CONFIG_ARCH_MT85XX)
    UINT32 u4Diff = 0;
    HAL_GetTime(&_MusbLockT0);
    HAL_GetTime(&_MusbLockT1);
    HAL_GetDeltaTime(&_MusbLockDt, &_MusbLockT0, &_MusbLockT1);
    u4Diff = _MusbLockDt.u4Seconds * 1000 + _MusbLockDt.u4Micros/1000;
    while ((_fgMUSBLockInit != MUSB_STATE_IDLE) && (u4Diff < 4000))
    {
        MUSB_NoneRunBackground();
        HAL_GetTime(&_MusbLockT1);
        HAL_GetDeltaTime(&_MusbLockDt, &_MusbLockT0, &_MusbLockT1);
        u4Diff = _MusbLockDt.u4Seconds * 1000 + _MusbLockDt.u4Micros/1000;
    }
if(u4Diff >= 4000)
    Printf("[]leave _Musb_lock %d %d\n", _fgMUSBLockInit, u4Diff);
#endif //#if defined(CONFIG_ARCH_MT85XX)

#else

    i4Ret = x_sema_lock(_hMUSBLock, X_SEMA_OPTION_WAIT);

#endif

    return i4Ret;
}

#if defined(CONFIG_ARCH_MT85XX)
INT32 Musb_LockEx(UINT32 u4Lun)
{
    INT32 i4Ret = 0;
    
    if(u4Lun >= USB_DIAG_MAX_LUN_NUM) return -1;

    i4Ret = x_sema_lock(_ahMUSBLock[u4Lun], X_SEMA_OPTION_WAIT);

    return i4Ret;
}
#endif

//-------------------------------------------------------------------------
/** _Musb_WPComplete
 *  USB MSD checking write protect complete call back function.
 *  @param  pvHfiNfyTag: call back notify tag.
 *  @param  e_nfy_cond: read write operation condition.
 *  @param  pv_data: io data structure.
 *  @retval  void.
 */
//-------------------------------------------------------------------------
static void _Musb_WPComplete(void *pvHfiNfyTag, int32_t e_nfy_cond, void *pv_data)
{
    MUSB_HfiMediumInfo *pMediumInfo;
    MUSB_IO_DATA_T *prMUSBData;
    MGC_ScsiCmdSetData* pScsi;
    uint8_t *pModeSenseData;
    MUSB_HfiAccessType AccessType = MUSB_HFI_ACCESS_RANDOM;

    MUSB_ASSERT(pvHfiNfyTag);
    MUSB_ASSERT(pv_data);

    pMediumInfo = (MUSB_HfiMediumInfo *)pvHfiNfyTag;
    prMUSBData = (MUSB_IO_DATA_T *)pv_data;
    MUSB_ASSERT(prMUSBData->pvData);
    pScsi = (MGC_ScsiCmdSetData *)prMUSBData->pvData;
    pModeSenseData = pScsi->aModeSenseData;
    MUSB_ASSERT(pModeSenseData);

    if (pScsi->bModeSenseType == MGC_SCSI_MODE_SENSE10)
    {
        AccessType = (pModeSenseData[3] & 0x80)
                         ? MUSB_HFI_ACCESS_RANDOM_READ : MUSB_HFI_ACCESS_RANDOM;
    }
    else if (pScsi->bModeSenseType == MGC_SCSI_MODE_SENSE)
    {
        AccessType = (pModeSenseData[2] & 0x80)
                         ? MUSB_HFI_ACCESS_RANDOM_READ : MUSB_HFI_ACCESS_RANDOM;
    }
    else
    {
        AccessType = MUSB_HFI_ACCESS_RANDOM_READ;
    }

    pMediumInfo->AccessType = AccessType;

    VERIFY(0 == _Musb_Unlock());

    UNUSED(e_nfy_cond);
}
//-------------------------------------------------------------------------
/** _Musb_Complete
 *  USB MSD read/write complete call back function.
 *  @param  pvHfiNfyTag: call back notify tag.
 *  @param  e_nfy_cond: read write operation condition.
 *  @param  pv_data: io data structure.
 *  @retval  void.
 */
//-------------------------------------------------------------------------
static void _Musb_Complete(void *pvHfiNfyTag, int32_t e_nfy_cond, void *pv_data)
{
    MUSB_ASSERT(pvHfiNfyTag == (void *)0x12345678);

    VERIFY(0 == _Musb_Unlock());

    UNUSED(pvHfiNfyTag);
    UNUSED(e_nfy_cond);
    UNUSED(pv_data);
}
#if defined(CONFIG_ARCH_MT85XX)
void Musb_CompleteEx(void *pvHfiNfyTag, int32_t e_nfy_cond, void *pv_data)
{
    INT32 status = -1;
    UINT32 u4Lun = ((UINT32)pvHfiNfyTag & 0x0000000F);

    status = Musb_UnlockEx(u4Lun);

    VERIFY(0 == status);

    UNUSED(pvHfiNfyTag);
    UNUSED(e_nfy_cond);
    UNUSED(pv_data);    
}
#endif

//-------------------------------------------------------------------------
/** _Musb_DevNfy.
*   usb device insert and remove notify handler.
*  @param  pvHfiHotSwapNfyTag   user's tag.
*  @param  e_nfy_cond     notify condition 0: absent 1: fail 2: insert.
*  @param  pv_data      driver callback pointer, usually is not used.
*  @retval void.
*/
//-------------------------------------------------------------------------
static void _Musb_DevNfy(void *pvTag)
{
    MUSB_HOTSWAP_NFY_INFO_T *prNfyInfo = NULL;
    UINT32 u4Lun;

    MUSB_ASSERT(pvTag);
    prNfyInfo = (MUSB_HOTSWAP_NFY_INFO_T *)pvTag;

    u4Lun = prNfyInfo->u4CompId;
    MUSB_ASSERT(u4Lun < MUSB_UCFS_UNIT_COUNT);

    // notify block device manager.
    if (_pfnMUSBCallback[u4Lun])
    {
        _pfnMUSBCallback[u4Lun](_u4MUSBCallbackTag[u4Lun], (INT32)prNfyInfo->u4Status);
    }
}

#ifdef MUSB_PORT_CONFIG
static void _Musb_Port_Init(void)
{
    uint8_t i;
    uint32_t u4UsbPortConfig;
	for(i = 0; i < MUSB_MAX_CONTROLLERS; i ++)
	{
		#ifdef CC_MTK_LOADER
		if(i == 0)
		{
			_i4MUSBPortConfig[i] = eUSB0LoaderConfig;

		}
		else if(i == 1)
		{
			_i4MUSBPortConfig[i] = eUSB1LoaderConfig;

		}
		else if(i == 2)
		{
			_i4MUSBPortConfig[i] = eUSB2LoaderConfig;
		}
		else if(i == 3)
		{
			_i4MUSBPortConfig[i] = eUSB3LoaderConfig;

		}
		#else
		if(i == 0)
		{
			_i4MUSBPortConfig[i] = eUSB0Config;

		}
		else if(i == 1)
		{
			_i4MUSBPortConfig[i] = eUSB1Config;

		}
		else if(i == 2)
		{
			_i4MUSBPortConfig[i] = eUSB2Config;
		}
		else if(i == 3)
		{
			_i4MUSBPortConfig[i] = eUSB3Config;

		}
		#endif
	}
    MGC_bUsbCount = 0;

    for(i = 0; i < MUSB_MAX_CONTROLLERS; i++)
    {
	if(0 == DRVCUST_InitQuery(_i4MUSBPortConfig[i], (UINT32 *)&u4UsbPortConfig))
	{
	    MGC_bUsbPortDef[i] = (uint8_t)u4UsbPortConfig;
	    if(MGC_bUsbPortDef[i])
	        MGC_bUsbCount ++;
	}
    }
    MGC_bSystemCount = MGC_bUsbCount;
    LOG(0, "It has %d USB Port,is %d, %d, %d, %d(1=using, 0=not use).\n",MGC_bUsbCount,
	MGC_bUsbPortDef[0],MGC_bUsbPortDef[1], MGC_bUsbPortDef[2], MGC_bUsbPortDef[3]);
}
#endif


#if !defined(CONFIG_ARCH_MT85XX)
//-------------------------------------------------------------------------
/** _Musb_VbusHandler
 *  Perform USB vbus turn on/off handler.
 *  @param  fgMode: TRUE to turn on vbus, FALSE to turn off vbus.
 *  @retval  void.
 */
//-------------------------------------------------------------------------
static void _Musb_VbusHandler(BOOL fgMode)
{
    UINT32 u4Gpio;
    UINT32 u4GpioPolarity;
    UINT32 i;

    if (fgMode == _fgMUSBVBUSEnable)
    {
        return;
    }
	for(i = 0; i < MUSB_MAX_CONTROLLERS; i ++)
	{
	    if(i == 0)
	    {
            _i4MUSBVbusGpioDef[i] = eUSBVbus0Gpio;
            _i4MUSBVbusEnablePolarityDef[i] = eUSBVbus0EnablePolarity;
	    }
	    else if(i == 1)
	    {
			_i4MUSBVbusGpioDef[i] = eUSBVbus1Gpio;
		    _i4MUSBVbusEnablePolarityDef[i] = eUSBVbus1EnablePolarity;
		}
		else if(i == 2)
		{
		    _i4MUSBVbusGpioDef[i] = eUSBVbus2Gpio;
		    _i4MUSBVbusEnablePolarityDef[i] = eUSBVbus2EnablePolarity;
		}

		else if(i == 3)
		{
		    _i4MUSBVbusGpioDef[i] = eUSBVbus3Gpio;
		    _i4MUSBVbusEnablePolarityDef[i] = eUSBVbus3EnablePolarity;
		}
	}

    _fgMUSBVBUSEnable = fgMode;
    UNUSED(_fgMUSBVBUSEnable);

    // turn on/off usb vbus control.
    for (i = 0; i < MUSB_MAX_CONTROLLERS; i++)
    {
        // fix warning
        UNUSED(_i4MUSBVbusGpio[i]);
        UNUSED(_i4MUSBVbusPolarity[i]);
        UNUSED(_fgMUSBVbusStatus[i]);
    	#ifdef MUSB_PORT_CONFIG
	if(MGC_bUsbPortDef[i])
	#endif
	{
        // check and set over current gpio setting. -1 if not define.
        if ((0 == DRVCUST_InitQuery(_i4MUSBVbusGpioDef[i], &u4Gpio)) &&
            (0 == DRVCUST_InitQuery(_i4MUSBVbusEnablePolarityDef[i], &u4GpioPolarity)))
        {
            _i4MUSBVbusGpio[i] = (INT32)u4Gpio;
            _i4MUSBVbusPolarity[i] = (INT32)u4GpioPolarity;
            _fgMUSBVbusStatus[i] = _fgMUSBVBUSEnable;

            u4GpioPolarity = (_fgMUSBVBUSEnable) ? u4GpioPolarity : (!u4GpioPolarity);
            LOG(0, "USB%d: Set GPIO%d = %d.\n", i, u4Gpio, u4GpioPolarity);
            if (GPIO_Output((INT32)u4Gpio, (INT32 *)&u4GpioPolarity)
                != (INT32)u4GpioPolarity)
            {
                LOG(0, "USB%d: Set GPIO%d error.\n", i, u4Gpio);
            }
        }
        else
        {
             LOG(0, "USB%d: No define vbus gpio.\n", i);
            _i4MUSBVbusGpio[i] = -1;
        }
    }
}
}

#ifdef MUSB_OC_SUPPORT
//-------------------------------------------------------------------------
/** _Musb_OCTimerOut
 *  Perform USB over current timer handler.
 *  @param  pt_tm_handle.
 *  @param  pv_tag  user's tag.
 *  @retval  void.
 */
//-------------------------------------------------------------------------
static void _Musb_OCTimerOut(HANDLE_T pt_tm_handle, void *pv_tag)
{
    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);

    VERIFY(OSR_OK == x_sema_unlock(phMUSBOCSemaHdl));
}

//-------------------------------------------------------------------------
/** _MUSB_OCThread
 *  USB over current check thread.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval   void
 */
//-------------------------------------------------------------------------
static VOID _MUSB_OCThread(VOID *pvArgv)
{
    MUSB_Port *pUsbPort = NULL;
    UINT32 i,u4PortUsing;
    BOOL fgOCStatus;
    BOOL fgOCNewStatus;
    BOOL fgOCServoGpio=0;
    UINT32 u4OCCount;
    UINT32 u4OCGpio;
    UINT32 u4OCGpioPolarity;
    UINT32 u4VbusGpio;
    UINT32 u4VbusGpioPolarity;
    UINT32 u4CheckTime = 1000;
    UINT32 u4Val;

    UNUSED(pvArgv);

    while(1)
    {
        (void)x_sema_lock(phMUSBOCSemaHdl, X_SEMA_OPTION_WAIT);

        // turn off vbus for device stable.
        if (!_fgMUSBVBUSEnable)
        {
            _Musb_VbusHandler(TRUE);

            u4CheckTime = 1000;
            (void)x_timer_start(_hMUSBOCTimerHdl, u4CheckTime,
                X_TIMER_FLAG_ONCE, _Musb_OCTimerOut, NULL);

            // Wait next timer event.
            continue;
        }

        if (_fgMUSBOCEnable)
        {
            // Check OverCurrent.
            u4CheckTime = 1000;

            //LOG(7, "USB over current checking...\n", NULL);
            for (i = 0,u4PortUsing=0; i < MUSB_MAX_CONTROLLERS; i++)
            {
		#ifdef MUSB_PORT_CONFIG
		if(MGC_bUsbPortDef[i])
		#endif
		{
                u4PortUsing++;
                // check if over current GPIO is exist
                if (-1 == _i4MUSBOCGpio[i])
                {
                    continue;
                }

/*
                if ((_i4MUSBOCGpio[i] >= SERVO_GPIO0) && (_i4MUSBOCGpio[i] <= SERVO_GPIO4))
                {
                    fgOCServoGpio = TRUE;
                    u4OCGpio = (UINT32)(_i4MUSBOCGpio[i]-SERVO_GPIO0);
                }
                else
                {
                    fgOCServoGpio = FALSE;
                    u4OCGpio = (UINT32)_i4MUSBOCGpio[i];
                }
*/
                u4Val = 0;
                u4VbusGpio = (UINT32)_i4MUSBVbusGpio[i];
                u4VbusGpioPolarity = (UINT32)_i4MUSBVbusPolarity[i];
                u4OCGpioPolarity = (UINT32)_i4MUSBOCPolarity[i];
                u4OCCount = 0;
                fgOCNewStatus = _fgMUSBOCStatus[i];

                do
                {
                /*
                    // check over current status.
                    if (fgOCServoGpio)
                    {
                        //if (PDWNC_ServoADCEnable(u4OCGpio) == 0)
                        {
                            u4Val = PDWNC_ReadServoADCChannelValue(u4OCGpio);
                        }
                        // Normal = 3.12V. Over current = 2.27V.
                        // Threshold = 2.7V = 2700mV.
                        u4Val = (u4Val*3120)/255; // Unit = mV.
                        LOG(9, "USB%d: Voltage=%d.\n", i, u4Val);
                        fgOCStatus = (u4OCGpioPolarity == (UINT32)(u4Val > 2700));
                    }
                    else
                    */
                    {
                        INT32 i4Set = 0;
                        GPIO_Enable((INT32)u4OCGpio, &i4Set);
                        fgOCStatus = (u4OCGpioPolarity == (UINT32)GPIO_Input((INT32)u4OCGpio));
                    }

                    // debounce.
                    if (fgOCNewStatus != fgOCStatus)
                    {
                        fgOCNewStatus = fgOCStatus;
                        u4OCCount = 0;
                    }
                    else
                    {
                        u4OCCount ++;
                    }

                    (void)MUSB_Sleep(50);
                } while(u4OCCount < 5);

                if (fgOCStatus != _fgMUSBOCStatus[i])
                {
                    // Over current is just disappear. Turn on Vbus, and check if it still happen.
                    if ((!fgOCStatus) &&
                         (_fgMUSBVbusStatus[i] == (BOOL)FALSE))
                    {
                        LOG(5, "USB%d: Turn on Vbus GPIO%d.\n", i, u4VbusGpio);
                        _fgMUSBVbusStatus[i] = TRUE;
                        if (GPIO_Output((INT32)u4VbusGpio, (INT32 *)&u4VbusGpioPolarity)
                            != (INT32)u4VbusGpioPolarity)
                        {
                            LOG(0, "USB%d: Set GPIO%d error.\n", i, u4VbusGpio);
                        }

                        // 50 ms to check if over current is still happen.
                        u4CheckTime = 50;
                        break;
                    }

                    if ((fgOCStatus) &&
                         (_fgMUSBVbusStatus[i] == (BOOL)TRUE))
                    {
                        LOG(5, "USB%d: Turn off Vbus GPIO%d.\n", i, u4VbusGpio);
                        _fgMUSBVbusStatus[i] = FALSE;

                        u4VbusGpioPolarity = (!u4VbusGpioPolarity);
                        if (GPIO_Output((INT32)u4VbusGpio, (INT32 *)&u4VbusGpioPolarity)
                            != (INT32)u4VbusGpioPolarity)
                        {
                            LOG(0, "USB%d: Set Vbus GPIO%d error.\n", i, u4VbusGpio);
                        }
                    }

                    _fgMUSBOCStatus[i] = fgOCStatus; // change over current status.

                    pUsbPort = MUSB_GetPort((u4PortUsing-1));
                    if ((pUsbPort) && (pUsbPort->pfOverCurrentNfy))
                    {
                        // report USB port over current.
                        pUsbPort->pfOverCurrentNfy((u4PortUsing-1), (UINT32)fgOCStatus);
                    }

                    LOG(0, "USB%d GPIO%d Over current %s !\n", i,
                        u4OCGpio,
                        ((fgOCStatus) ? ("happen") : ("disappear")));
                }
                else
                {
                    /*
                        When turn on servo gpio to get adc value, it will turn on vbus.
                    */
                    if (fgOCStatus)
                    {
                        if ((fgOCServoGpio) ||(_fgMUSBVbusStatus[i] == (BOOL)TRUE))
                        {
                            LOG(5, "USB%d: Turn off Vbus GPIO%d.\n", i, u4VbusGpio);
                            _fgMUSBVbusStatus[i] = FALSE;

                            u4VbusGpioPolarity = (!u4VbusGpioPolarity);
                            if (GPIO_Output((INT32)u4VbusGpio, (INT32 *)&u4VbusGpioPolarity)
                                != (INT32)u4VbusGpioPolarity)
                            {
                                LOG(0, "USB%d Set Vbus GPIO%d error.\n", i, u4VbusGpio);
                            }
                        }
                    }
                }
            }
            }

            (void)x_timer_start(_hMUSBOCTimerHdl, u4CheckTime,
                X_TIMER_FLAG_ONCE, _Musb_OCTimerOut, NULL);
        }
    }
}
#endif

//-------------------------------------------------------------------------
/** _Musb_DrvCust
 *  Perform USB driver customization.
 *  @param  u4Mode		1: host, 0: device.
 *  @retval  void.
 */
//-------------------------------------------------------------------------
static void _Musb_DrvCust(UINT32 u4Mode)
{
    UINT32 u4HSTERMC;
    UINT32 u4FullSpeedEnable;
    UINT32 u4DisableSuspend;
#ifdef MUSB_OC_SUPPORT
    UINT32 u4Gpio;
    UINT32 u4GpioPolarity;
    UINT32 i;
    UINT32 u4VbusTurnOnDelay;
#endif

    if (_fgMUSBDrvcustInit)
    {
        return;
    }

    _fgMUSBDrvcustInit = TRUE;

#ifdef MUSB_OC_SUPPORT
    _fgMUSBOCEnable = FALSE;
    for (i = 0; i < MUSB_MAX_CONTROLLERS; i++)
    {
	#ifdef MUSB_PORT_CONFIG
	if(MGC_bUsbPortDef[i])
	#endif
	{
        // check and set over current gpio setting. -1 if not define.
        if ((0 == DRVCUST_InitQuery(_i4MUSBOCGpioDef[i], &u4Gpio)) &&
            (0 == DRVCUST_InitQuery(_i4MUSBOCEnablePolarityDef[i], &u4GpioPolarity)))
        {
            _i4MUSBOCGpio[i] = (INT32)u4Gpio;
            _i4MUSBOCPolarity[i] = (INT32)u4GpioPolarity;
            _fgMUSBOCStatus[i] = FALSE;
            _fgMUSBOCEnable = TRUE;
        }
        else
        {
            _i4MUSBOCGpio[i] = -1;
        }
    }
    }

    // turn on vbus a few second later.
    // create a timer to polling over current flag.
    if (OSR_OK == x_timer_create(&_hMUSBOCTimerHdl))
    {
        (void)x_sema_create(&phMUSBOCSemaHdl,
            X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);

        /* back ground reading. */
        (void)(x_thread_create(&phMUSBOCThreadHdl, "USBOC", 1536, 66,
            _MUSB_OCThread, 0, NULL));

        if (u4Mode)
        {
            // Get turn on vbus delay.
            if (0 != DRVCUST_InitQuery(eUSBVbusTurnOnDelay, &u4VbusTurnOnDelay))
            {
                // Default value = 5000 ms.
                u4VbusTurnOnDelay = 5000;
            }

            // Set timer to turn on vbus.
            (void)x_timer_start(_hMUSBOCTimerHdl, u4VbusTurnOnDelay,
                X_TIMER_FLAG_ONCE, _Musb_OCTimerOut, NULL);
        }
    }
#endif

    // check if only operate in full speed mode.
    if (0 == DRVCUST_InitQuery(eUSBFullSpeedOnlyEnable, &u4FullSpeedEnable))
    {
        if (u4FullSpeedEnable)
        {
            MGC_bWantHighSpeed = FALSE;
        }
    }

    // check if there is customized HSTERMC.
    if ((MGC_bWantHighSpeed) &&
        (0 == DRVCUST_InitQuery(eUSBHSTERMC, &u4HSTERMC)))
    {
        /** Set USB HS_TERMC [4:0] in PHY CONTROL REGISTER 1
             It is used in MUSB_BoardSoftReset().
             If MGC_bHSTERMC = 0xFF, MUSB_BoardSoftReset() will
             use h/w default value to set HSTERMC.
        */
        MGC_bHSTERMC = (uint8_t)(u4HSTERMC & 0xFF);
    }

    // check if usb do not enter suspend mode.
    if (0 == DRVCUST_InitQuery(eUSBDisableSuspendMode, &u4DisableSuspend))
    {
        if (u4DisableSuspend)
        {
            MGC_bWantSuspend = FALSE;
        }
    }
}
#endif //#if !defined(CONFIG_ARCH_MT85XX)


#ifdef USB_VBUS_CONT_P1_P2
//-------------------------------------------------------------------------
/** _Musb_DrvCust
 *  Perform USB driver customization.
 *  @param  u4Mode		1: host, 0: device.
 *  @retval  void.
 */
//-------------------------------------------------------------------------
static void _Musb_DrvCust(UINT32 u4Mode)
{
   if(u4Mode) //Host Mode
   {
        GPIO_Output(USB_VBUS_PCONT1, HIGH);
        GPIO_Output(USB_VBUS_PCONT2, HIGH);
   }
   else
   {
        GPIO_Output(USB_VBUS_PCONT1, LOW);
        GPIO_Output(USB_VBUS_PCONT2, LOW);
   }
}
#endif

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** MUSB_Read
*  MUSB read function.
*  @param  u8Offset     read offset from card, u8Offset should be multiple of block size.
*  @param  u4MemPtr      user's read buffer address.
*  @param  u4MemLen      read length, u4MemLen should be multiple of block size.
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MUSB_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;
    static uint8_t breadcount = 0;
	breadcount ++;
	if(breadcount == 0xff)
		Printf(".");
    
    if ((u4MemPtr == 0) ||(u4MemLen < 512))
    {
        return -1;
    }
    if(u4MemPtr % g_u4Dcachelinesize != 0)
        {
        Printf("The DMA buffer Address[0x%08x] not 64byte align !\n",(UINT32)u4MemPtr);
        return -1;
    }

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return -1;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return -1;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return -1;
    }

    // get medium information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return -1;
    }

    if (pMediumInfo->dwBlockSize < 512)
    {
        return -1;
    }

    // Set read call back function.
    _rMUSBAsynInfo.pvHfiNfyTag = (void *)0x12345678;
    _rMUSBAsynInfo.pfHfiNfy = _Musb_Complete;
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &_rMUSBAsynInfo))
    {
        return -1;
    }

    // prepare sync in different os environment.
    if (0 != _Musb_InitLock())
    {
        return -1;
    }

    /*
     * Read the data from the same sector which has written already
     */
#if defined(CONFIG_ARCH_MT85XX)
    _rMUSBData.u8BlkNum = (uint64_t)(u8Offset / pMediumInfo->dwBlockSize);
#else
    #ifdef __linux__
    _rMUSBData.u8BlkNum = 0;
    #else
    _rMUSBData.u8BlkNum = (uint64_t)(u8Offset / pMediumInfo->dwBlockSize);
    #endif
#endif

    _rMUSBData.u4Count = u4MemLen;
    _rMUSBData.u4TransCount = 0;
    _rMUSBData.pvData = (void *)u4MemPtr;

    if (0 > pHfiDriver->pfRWBurst(u4Lun, MUSB_HFI_OP_READ, (void *)&_rMUSBData))
    {
        return -1;
    }

    // wait complete from _Musb_Complete().
    if (0 != _Musb_Lock())
    {
        return -1;
    }

    // check result.
    return ((_rMUSBData.u4Count == _rMUSBData.u4TransCount) ? 0 : -1);
}

#if defined(CONFIG_ARCH_MT85XX)
INT32 MUSB_ReadEx(UINT32 u4Lun, UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    //UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;

    if ((u4MemPtr == 0) ||(u4MemLen < 512))
    {
        return -1;
    }

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return -1;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return -1;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return -1;
    }

    // get medium information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return -1;
    }

    if (pMediumInfo->dwBlockSize < 512)
    {
        return -1;
    }

    // prepare sync in different os environment.
    if (0 != Musb_InitLockEx(u4Lun))
    {
        return -1;
    }
   
    // Set read call back function.
    arMUSBAsynInfo[u4Lun].pvHfiNfyTag = (void *)(0x12345670 | u4Lun);
    arMUSBAsynInfo[u4Lun].pfHfiNfy = Musb_CompleteEx;
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &arMUSBAsynInfo[u4Lun]))
    {
        return -1;
    }
    
    /*
     * Read the data from the same sector which has written already
     */
    arMUSBData[u4Lun].u8BlkNum = u8Offset / pMediumInfo->dwBlockSize;
    arMUSBData[u4Lun].u4Count = u4MemLen;
    arMUSBData[u4Lun].u4TransCount = 0;
    arMUSBData[u4Lun].pvData = (void *)u4MemPtr;

    if (0 > pHfiDriver->pfRWBurst(u4Lun, MUSB_HFI_OP_READ, (void *)&arMUSBData[u4Lun]))
    {
        return -1;
    }

    // wait complete from _Musb_Complete().
    if (0 != Musb_LockEx(u4Lun))
    {
        return -1;
    }

    // check result.
    return ((arMUSBData[u4Lun].u4Count == arMUSBData[u4Lun].u4TransCount) ? 0 : -1);
}
#endif //#if defined(CONFIG_ARCH_MT85XX)


//-------------------------------------------------------------------------
/** MUSB_Write
*  MUSB write function.
*  @param  u8Offset     write offset from card, u8Offset should be multiple of block size.
*  @param  u4MemPtr      user's write buffer address.
*  @param  u4MemLen      write length, u4MemLen should be multiple of block size.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MUSB_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;

    if ((u4MemPtr == 0) ||(u4MemLen < 512))
    {
        return -1;
    }

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return -1;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return -1;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return -1;
    }

    // get pointer which point to medium information structure.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return -1;
    }

    if (pMediumInfo->dwBlockSize < 512)
    {
        return -1;
    }
#if 0
    // prepare sync in different os environment.
    if (0 != _Musb_InitLock())
    {
        return -1;
    }

    // Set write call back function.
    _rMUSBAsynInfo.pvHfiNfyTag = (void *)pMediumInfo;
    _rMUSBAsynInfo.pfHfiNfy = _Musb_WPComplete;
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &_rMUSBAsynInfo))
    {
        return -1;
    }

    /*
    * Check write protect of this device
    */
    MUSB_MemSet(&_rMUSBData, 0, sizeof(MUSB_IO_DATA_T));

    if (0 > pHfiDriver->pfWriteProtect(u4Lun, (void *)&_rMUSBData))
    {
        return -1;
    }

    // wait complete from _Musb_Complete().
    if (0 != _Musb_Lock())
    {
        return -1;
    }
#endif
    if (pMediumInfo->AccessType != MUSB_HFI_ACCESS_RANDOM)
    {
        LOG(1, "Device in Write Protected.\n", NULL);
        return -1;
    }

    // Set write call back function.
    _rMUSBAsynInfo.pvHfiNfyTag = (void *)0x12345678;
    _rMUSBAsynInfo.pfHfiNfy = _Musb_Complete;
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &_rMUSBAsynInfo))
    {
        return -1;
    }

    // prepare sync in different os environment.
    if (0 != _Musb_InitLock())
    {
        return -1;
    }

    /*
     * Write the data to device
     */
#if defined(CONFIG_ARCH_MT85XX)
    _rMUSBData.u8BlkNum = (uint64_t)(u8Offset / pMediumInfo->dwBlockSize);
#else
    #ifdef __linux__
    _rMUSBData.u8BlkNum = 0;
    #else
    _rMUSBData.u8BlkNum = (uint64_t)(u8Offset / pMediumInfo->dwBlockSize);
    #endif
#endif

    _rMUSBData.u4Count = u4MemLen;
    _rMUSBData.u4TransCount = 0;
    _rMUSBData.pvData = (void *)u4MemPtr;

    if (0 > pHfiDriver->pfRWBurst(u4Lun, MUSB_HFI_OP_WRITE, (void *)&_rMUSBData))
    {
        return -1;
    }

    // wait complete from _Musb_Complete().
    if (0 != _Musb_Lock())
    {
        return -1;
    }

    // check result.
    return ((_rMUSBData.u4Count == _rMUSBData.u4TransCount) ? 0 : -1);
}

#if defined(CONFIG_ARCH_MT85XX)
INT32 MUSB_WriteEx(UINT32 u4Lun, UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;

    if ((u4MemPtr == 0) ||(u4MemLen < 512))
    {
        return -1;
    }

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return -1;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return -1;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return -1;
    }

    // get pointer which point to medium information structure.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return -1;
    }

    if (pMediumInfo->dwBlockSize < 512)
    {
        return -1;
    }

    if (pMediumInfo->AccessType != MUSB_HFI_ACCESS_RANDOM)
    {
        LOG(1, "Device in Write Protected.\n", NULL);
        return -1;
    }

    // prepare sync in different os environment.
    if (0 != Musb_InitLockEx(u4Lun))
    {
        return -1;
    }
    
    // Set write call back function.
    arMUSBAsynInfo[u4Lun].pvHfiNfyTag = (void *)(0x12345670 | u4Lun);
    arMUSBAsynInfo[u4Lun].pfHfiNfy = Musb_CompleteEx;
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &arMUSBAsynInfo[u4Lun]))
    {
        return -1;
    }

    /*
     * Write the data to device
     */
    arMUSBData[u4Lun].u8BlkNum = u8Offset / pMediumInfo->dwBlockSize;
    arMUSBData[u4Lun].u4Count = u4MemLen;
    arMUSBData[u4Lun].u4TransCount = 0;
    arMUSBData[u4Lun].pvData = (void *)u4MemPtr;

    if (0 > pHfiDriver->pfRWBurst(u4Lun, MUSB_HFI_OP_WRITE, (void *)&arMUSBData[u4Lun]))
    {
        return -1;
    }

    // wait complete from _Musb_Complete().
    if (0 != Musb_LockEx(u4Lun))
    {
        return -1;
    }

    // check result.
    return ((arMUSBData[u4Lun].u4Count == arMUSBData[u4Lun].u4TransCount) ? 0 : -1);
}
#endif //#if defined(CONFIG_ARCH_MT85XX)

//-------------------------------------------------------------------------
/** MUSB_GetModel
*  MUSB get model name function.
*  @param  VOID.
*  @return   USB model name.
*/
//-------------------------------------------------------------------------
CHAR *MUSB_GetModel(VOID)
{
    UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiDeviceInfo *pDeviceInfo;
    INT32 i4Status;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return NULL;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return NULL;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return NULL;
    }

    // get device information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_INFO, 0, &pDeviceInfo))
    {
        return NULL;
    }

    return (CHAR *)pDeviceInfo->pDiskProduct;
}

#if defined(CONFIG_ARCH_MT85XX)
CHAR *MUSB_GetModelEx(UINT32 u4Lun)
{
    //UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiDeviceInfo *pDeviceInfo;
    INT32 i4Status;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return NULL;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return NULL;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return NULL;
    }

    // get device information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_INFO, 0, &pDeviceInfo))
    {
        return NULL;
    }

    return (CHAR *)pDeviceInfo->pDiskProduct;
}
#endif //#if defined(CONFIG_ARCH_MT85XX)
//-------------------------------------------------------------------------
/** MUSB_GetBlockSize
*  MUSB get block size function.
*  @param  VOID.
*  @return  the card block size in bytes.
*/
//-------------------------------------------------------------------------
INT32 MUSB_GetBlockSize(VOID)
{
    UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return -1;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return -1;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return -1;
    }

    // get medium information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return -1;
    }

    if (pMediumInfo->dwBlockSize < 512)
    {
        return -1;
    }

    return (INT32)pMediumInfo->dwBlockSize;
}

#if defined(CONFIG_ARCH_MT85XX)
INT32 MUSB_GetBlockSizeEx(UINT32 u4Lun)
{
    //UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return -1;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return -1;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return -1;
    }

    // get medium information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return -1;
    }

    if (pMediumInfo->dwBlockSize < 512)
    {
        return -1;
    }

    return (INT32)pMediumInfo->dwBlockSize;
}
#endif //#if defined(CONFIG_ARCH_MT85XX)
//-------------------------------------------------------------------------
/** MUSB_GetSize
*  MUSB get card size function.
*  @param  VOID.
*  @return  the card size. Unit in sector:
*                512 bytes or 2048 bytes depend on pMediumInfo->dwBlockSize.
*/
//-------------------------------------------------------------------------
INT64 MUSB_GetSize(VOID)
{
    UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return -1;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return -1;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return -1;
    }

    // get medium information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return -1;
    }

    if (pMediumInfo->dwBlockSize < 512)
    {
        return -1;
    }

    return (INT64)(MUSB_MAKE64(pMediumInfo->dwBlockCountLo, pMediumInfo->dwBlockCountHi));
}


#ifdef MUSB_LOADER
//-------------------------------------------------------------------------
/** MUSB_UpgradeWaitTime
*  MUSB wait usb delay time to find upgrade file.
*  @param  VOID.
*  @retval 5000   5s delay ( LOADER_UPGRADE_USB_PORT_SUPPORT == 0)
*  @retval 1000   1s delay. ( LOADER_UPGRADE_USB_PORT_SUPPORT == 1)
*/
//-------------------------------------------------------------------------
UINT32 MUSB_UpgradeWaitTime(VOID)
{
    UINT32 u4WaitTime = 0;
    if(LOADER_UPGRADE_USB_PORT_SUPPORT)
        u4WaitTime = 5000;
    else
        u4WaitTime = 5000;
    return u4WaitTime;
}

static UINT32 u4DevicesOnHub = FALSE;

void MUSB_SetDeviceOnHubFlag(UINT32 u4flag)
{
    u4DevicesOnHub = u4flag;
}
UINT32 MUSB_GetDeviceOnHubFlag(VOID)
{
    return u4DevicesOnHub;
}

static UINT32 u4MassStorageOnHub = FALSE;

void MUSB_SetMassOnHubFlag(UINT32 u4Massflag)
{
    u4MassStorageOnHub = u4Massflag;
}
UINT32 MUSB_GetMassOnHubFlag(VOID)
{
    return u4MassStorageOnHub;
}


//-------------------------------------------------------------------------
/** MUSB_GetInsert
*  MUSB get usb insert status.
*  @param  VOID.
*  @retval 1   MSD Device is inserted in USB socket.
*  @retval 0   Device absent or not a MSD device.
*/
//-------------------------------------------------------------------------
#if defined(CONFIG_ARCH_MT85XX)
INT32 MUSB_GetInsert(VOID)
{
    MUSB_Port *pUsbPort = NULL;
    MGC_Port *pPort = NULL;
    uint32_t i;
    HAL_TIME_T t0, t1, dt;
    UINT32 u4Diff = 0;
    UINT32 u4UsbLoaderWaitTime = 0; // ms

    // Get user's define max device waiting time.
    if (0 != DRVCUST_InitQuery(eUSBLoaderWaitTime, &u4UsbLoaderWaitTime))
    {
        // use default value.
        u4UsbLoaderWaitTime = MUSB_LOADER_MAX_WAIT_TIME;
    }    

    HAL_GetTime(&_MusbT1);
    HAL_GetDeltaTime(&_MusbDt, &_MusbT0, &_MusbT1);
    u4Diff = _MusbDt.u4Seconds * 1000 + _MusbDt.u4Micros/1000;                        
    Printf("USB: Vbus turn up time = %d ms.\n", u4Diff);        

    // Turn on GPIO already above u4UsbLoaderWaitTime(ms). No Need to wait more time.
    u4UsbLoaderWaitTime = ((u4Diff >= u4UsbLoaderWaitTime) ?
        (50) : (u4UsbLoaderWaitTime - u4Diff));
    Printf("USB: Wait device insert time = %d ms.\n", u4UsbLoaderWaitTime);        

    HAL_GetTime(&t0);
    while (1)
    {
        MUSB_NoneRunBackground();
    
        for (i=0; i<MUSB_CountPorts(); i++)
        {
            pUsbPort = MUSB_GetPort(i);
            MUSB_ASSERT(pUsbPort);        
            pPort = (MGC_Port *)pUsbPort->pPrivateData;
            MUSB_ASSERT(pPort);
            
            if (pPort->bInsert)
            {
                HAL_GetTime(&t1);
                HAL_GetDeltaTime(&dt, &t0, &t1);
                u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;                        
                Printf("USB-%d: Insert time = %d ms.\n", i, u4Diff);        
                return 1;
            }
        }

        HAL_GetTime(&t1);
        HAL_GetDeltaTime(&dt, &t0, &t1);
        u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;
        // wait Max time to check device on this port.
        if (u4Diff >=u4UsbLoaderWaitTime)
        {
            break;
        }
    }

    return 0;
}
#else //#if defined(CONFIG_ARCH_MT85XX)
INT32 MUSB_GetInsert(VOID)
{
    MUSB_Port *pUsbPort = NULL;
    MGC_Port *pPort = NULL;
    uint32_t i, u1port;
    HAL_TIME_T t0, t1, dt;
    UINT32 u4Diff = 0;
    UINT32 u4UsbLoaderWaitTime = MUSB_LOADER_MAX_WAIT_TIME; // ms
    /*
        Notice: This vbus turn on until polling device time gap.
        Normally must above 500ms before checking device is inserted.
    */
    UINT32 u4UsbVbusWaitTime = 0;
    uint16_t wIndex = 0;
    uint16_t wCount = 0;
    MUSB_Device *pDevice;
    UINT32 u4PortStatus = 0;

    // Get user's define max device waiting time.
    if (0 != DRVCUST_InitQuery(eUSBLoaderWaitTime, &u4UsbVbusWaitTime))
    {
        // use default value.
        u4UsbVbusWaitTime = MUSB_LOADER_VBUS_WAIT_TIME;
    }

    HAL_GetTime(&_MusbT1);
    HAL_GetDeltaTime(&_MusbDt, &_MusbT0, &_MusbT1);
    u4Diff = _MusbDt.u4Seconds * 1000 + _MusbDt.u4Micros/1000;
    Printf("USB: Vbus turn up time = %d ms, Max =%d ms.\n", u4Diff, u4UsbVbusWaitTime);

    u1port = LOADER_UPGRADE_USB_PORT_SUPPORT;

    // Check device insert and set connect to u4PortStatus;
    u4PortStatus = 0;
    HAL_GetTime(&t0);
    do{

        for (i=u1port; i<MUSB_CountPorts(); i++)
        {
            MUSB_NoneRunBackground();

            if ((u4PortStatus & (1 << i)) == 0)
            {
                pUsbPort = MUSB_GetPort(i);
                if(pUsbPort)
                {
                    pPort = (MGC_Port *)pUsbPort->pPrivateData;
                    MUSB_ASSERT(pPort);

                    if (pPort->bInsert)
                    {
                        Printf("USB-%d: insert.\n", i);
                        u4PortStatus |= 1 << i;
                    }
                }
                else
                {
                    MUSB_ASSERT(pUsbPort);
                }
            }
        }

        HAL_GetTime(&t1);
        HAL_GetDeltaTime(&dt, &t0, &t1);
        u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;
    }while (u4UsbVbusWaitTime > u4Diff);

    if (u4PortStatus == 0)
    {
        return FIND_MUSB_NONE;
    }

    // Check Device is mass storage class.
    HAL_GetTime(&t0);
    while (1)
    {
        for (i=u1port; i<MUSB_CountPorts(); i++)
        {
            MUSB_NoneRunBackground();

            if (u4PortStatus & (1 << i))
            {
                pUsbPort = MUSB_GetPort(i);
                if(pUsbPort)
                {
                pPort = (MGC_Port *)pUsbPort->pPrivateData;
                MUSB_ASSERT(pPort);
                // Check if device is mass storage class.
                wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
                for (wIndex=0; wIndex < wCount; wIndex++)
                {
                    pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);
                    if (pDevice)
                    {
                        HAL_GetTime(&t1);
                        HAL_GetDeltaTime(&dt, &t0, &t1);
                        u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;

                        Printf("USB-%d: ClassCode= 0x%X, u4Diff=%d ms.\n", i, pDevice->bDeviceClass, u4Diff);

                        if (pDevice->bDeviceClass == MUSB_CLASS_MASS_STORAGE)
                        {
                            return FIND_CLASS_MASS_STORAGE;
                        }
                        else if(pDevice->bDeviceClass == MUSB_CLASS_HUB)
                        {
                            return FIND_CLASS_HUB;
                        }
                        else
                        {
                            // Clear u4PortStatus if this port is not mass storage class.
                            u4PortStatus &= ~(1 << i);
                            if (u4PortStatus == 0)
                            {
                                return FIND_MUSB_NONE;
                            }
                        }
                    }
                }
            }
                else
                {
                    MUSB_ASSERT(pUsbPort);
                }
            }
            HAL_GetTime(&t1);
            HAL_GetDeltaTime(&dt, &t0, &t1);
            u4Diff = dt.u4Seconds * 1000 + dt.u4Micros/1000;
            // wait Max time to check device on this port.
            if (u4Diff >=u4UsbLoaderWaitTime)
            {
                return FIND_MUSB_NONE;
            }
        }
    }
}
#endif //#if defined(CONFIG_ARCH_MT85XX)
#endif //#ifdef MUSB_LOADER
//-------------------------------------------------------------------------
/** MUSB_GetStatus
*  MUSB get usb status.
*  @param  VOID.
*  @return  the card status in STORG_DEV_STATUS_T.
*/
//-------------------------------------------------------------------------
INT32 MUSB_GetStatus(VOID)
{
    UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    INT32 i4Status;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return -1;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return -1;
    }

    return i4Status;
}

//-------------------------------------------------------------------------
/** MUSB_SetHotswapCallback
* MUSB register device call back function when usb is removed, inserted.
*  @param  u4Tag             user's tag in call back function.
*  @param  pfnCallback     user's call back function, it will be called at card inserted or removed.
*  @retval VOID.
*/
//-------------------------------------------------------------------------
void MUSB_SetHotswapCallback(UINT32 u4Tag, MUSB_CALLBACK pfnCallback)
{
    UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    UINT32 i;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();
    MUSB_ASSERT(pHfiDriver);

    // init medium call back function.
    for (i=0; i<MUSB_UCFS_UNIT_COUNT; i++)
    {
        _rMusbMediumInfo.pvTag = &_rMusbMediumInfo;
        _rMusbMediumInfo.pfNfy = _Musb_DevNfy;

        if (0 > pHfiDriver->pfIoCtl(i,
            (UINT32)MUSB_IOCTL_SET_SOCKET_DEV_NFY, 0, (void *)&_rMusbMediumInfo))
        {
            return;
        }
    }

    MUSB_ASSERT(u4Lun < MUSB_UCFS_UNIT_COUNT);
    _u4MUSBCallbackTag[u4Lun] = u4Tag;
    _pfnMUSBCallback[u4Lun] = pfnCallback;

    return;
}

//-------------------------------------------------------------------------
/** MUSB_GetInfo
* Get target protected information.
*  @retval 0         Success.
*  @retval 2  Device read-only.
*  @retval 3  Device abstent.
*/
//-------------------------------------------------------------------------
#if !defined(CONFIG_ARCH_MT85XX)
INT32 MUSB_GetInfo(void)
{
    UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return -1;
    }

    // get medium information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return 3; //USBM_MEDIA_STS_ABSENT
    }

    if (pMediumInfo->AccessType == MUSB_HFI_ACCESS_RANDOM_READ)
    {
        return 2; //USBM_MEDIA_STS_WR_PROTECT
    }

    return 0;
}
#endif //#if !defined(CONFIG_ARCH_MT85XX)


//-------------------------------------------------------------------------
/** MUSB_GetBlkFuncTbl
* Get MUSB block function table pointer.
*  @param  pFuncTable      define the query function that the block manager
*                                       can get a table of pointers to all IO functions.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MUSB_GetBlkFuncTbl(MBlockFunc_T *pFuncTable)
{
    x_memcpy(pFuncTable, &_rMUSBBlkFuncTbl, sizeof(MBlockFunc_T));
    return 0;
}

//-------------------------------------------------------------------------
/** MUSB_GetAccessType
*  MUSB get access type.
*  @retval device accesss type.
*/
//-------------------------------------------------------------------------
MUSB_HfiAccessType MUSB_GetAccessType(void)
{
    UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    // get medium information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    // prepare sync in different os environment.
    if (0 != _Musb_InitLock())
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    // Set write call back function.
    _rMUSBAsynInfo.pvHfiNfyTag = (void *)pMediumInfo;
    _rMUSBAsynInfo.pfHfiNfy = _Musb_WPComplete;
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &_rMUSBAsynInfo))
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    /*
    * Check write protect of this device
    */
    MUSB_MemSet(&_rMUSBData, 0, sizeof(MUSB_IO_DATA_T));

    if (0 > pHfiDriver->pfWriteProtect(u4Lun, (void *)&_rMUSBData))
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    // wait complete from _Musb_Complete().
    if (0 != _Musb_Lock())
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    return pMediumInfo->AccessType;
}

#if defined(CONFIG_ARCH_MT85XX)
MUSB_HfiAccessType MUSB_GetAccessTypeEx(UINT32 u4Lun)
{
    //UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiMediumInfo *pMediumInfo;
    INT32 i4Status;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    if (HFI_STATUS_READY != i4Status)
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    // get medium information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    // prepare sync in different os environment.
    if (0 != _Musb_InitLock())
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    // Set write call back function.
    _rMUSBAsynInfo.pvHfiNfyTag = (void *)pMediumInfo;
    _rMUSBAsynInfo.pfHfiNfy = _Musb_WPComplete;
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_SET_TYPENFY, 0, &_rMUSBAsynInfo))
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    /*
    * Check write protect of this device
    */
    MUSB_MemSet(&_rMUSBData, 0, sizeof(MUSB_IO_DATA_T));
    
    if (0 > pHfiDriver->pfWriteProtect(u4Lun, (void *)&_rMUSBData))
    {
        return MUSB_HFI_ACCESS_FAIL;
    }

    // wait complete from _Musb_Complete().
    if (0 != _Musb_Lock())
    {
        return MUSB_HFI_ACCESS_FAIL;
    }
   
    return pMediumInfo->AccessType;
}
#endif //#if defined(CONFIG_ARCH_MT85XX)

//-------------------------------------------------------------------------
/** MUSB_GetTotalLun
* Get total number of logical unit number in MSD device.
*  @param  void.
*  @retval   total number of logical unit number in MSD device.
*/
//-------------------------------------------------------------------------
UINT32 MUSB_GetTotalLun(void)
{
    UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiDeviceInfo *pDeviceInfo;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0;
    }

    // get device information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_INFO, 0, &pDeviceInfo))
    {
        return 0;
    }

    return (UINT32)pDeviceInfo->bLunCount;
}

#if defined(CONFIG_ARCH_MT85XX)
UINT32 MUSB_GetTotalLunEx(UINT32 u4Lun)
{
    //UINT32 u4Lun = _u4MUSBLun;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_HfiDeviceInfo *pDeviceInfo;

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0;
    }

    // get device information.
    if (0 > pHfiDriver->pfIoCtl(u4Lun, (uint32_t)MUSB_IOCTL_GET_DEVICE_INFO, 0, &pDeviceInfo))
    {
        return 0;
    }

    return (UINT32)pDeviceInfo->bLunCount;
}
#endif //#if defined(CONFIG_ARCH_MT85XX)

//-------------------------------------------------------------------------
/** MUSB_GetLun
* Get active target logical unit number of MSD device.
*  @param  u4Lun   target logical unit number of MSD device.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MUSB_GetLun(void)
{
    return (INT32)_u4MUSBLun;
}

//-------------------------------------------------------------------------
/** MUSB_SetLun
* Set target logical unit number of MSD device.
*  @param  u4Lun   target logical unit number of MSD device.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MUSB_SetLun(UINT32 u4Lun)
{
    _u4MUSBLun = u4Lun;
    return 0;
}

//-------------------------------------------------------------------------
/** MUSB_Suspend
* Set suspend usb bus.
*  @param  void.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MUSB_Suspend(void)
{
#if (!defined CC_SUPPORT_STR) || (defined(CONFIG_ARCH_MT85XX))
    MUSB_Port *pUsbPort = NULL;
    MGC_Port *pPort = NULL;
    uint32_t i;

    for (i=0; i<MUSB_CountPorts(); i++)
    {
        LOG(5, "HS_HOST_SUSPEND Port%d.\n", i);

        pUsbPort = MUSB_GetPort(i);
        MUSB_ASSERT(pUsbPort);
        if (pUsbPort)
        {
            pPort = (MGC_Port *)pUsbPort->pPrivateData;
            MUSB_ASSERT(pPort);
            if (pPort)
            {
                MUSB_SuspendBus((MUSB_BusHandle)pPort);
            }
        }
    }

#endif
    return 0;
}

#if (defined CC_SUPPORT_STR) && (!defined(CONFIG_ARCH_MT85XX))

static uint32_t MUSB_RESET(void* pBase)
{
    uint32_t u4Reg = 0;
    uint8_t nDevCtl = 0;
    MUSB_ASSERT(pBase);

    //USB DMA enable
	#if defined (CC_MT5880)
    u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0061300, 0, u4Reg);
	#elif defined (CC_MT5881)
    u4Reg= IO_READ32(0xf0061200, 0);
    u4Reg |= 0x00cfff00;
    IO_WRITE32(0xf0061200, 0, u4Reg);
	#elif defined(CC_MT5398)
    u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0060200, 0, u4Reg);    
	#elif defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5861)
    u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0061a00, 0, u4Reg);
	#elif defined(CC_MT5882)
	u4Reg = 0x00cfff00;
    IO_WRITE32(0xf0061a00, 0, u4Reg);
	#endif


    // VRT insert R enable
    u4Reg = MGC_PHY_Read32(pBase,M_REG_PHYC0);
    u4Reg |= 0x4000;
    MGC_PHY_Write32(pBase, M_REG_PHYC0, u4Reg);

    //Soft Reset, RG_RESET for Soft RESET
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
    u4Reg |=   0x00004000;
    MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);
    u4Reg &=  ~0x00004000;
    MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

	// MAC reset
	u4Reg = MGC_Read16(pBase,0x74);

	u4Reg |=0x2;
	MGC_Write16(pBase, 0x74, u4Reg);
	u4Reg &=~0x2;
	MGC_Write16(pBase, 0x74, u4Reg);

    //otg bit setting
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x6C);
    u4Reg &= ~0x3f3f;
    u4Reg |=  0x3f2c;
    MGC_PHY_Write32(pBase, (uint32_t)0x6C, u4Reg);

    //suspendom control
    u4Reg = MGC_PHY_Read32(pBase,(uint32_t)0x68);
    u4Reg &=  ~0x00040000;
    MGC_PHY_Write32(pBase, (uint32_t)0x68, u4Reg);

    u4Reg = MGC_Read8(pBase,M_REG_PERFORMANCE3);
    u4Reg |=  0x80;
    u4Reg &= ~0x40;
    MGC_Write8(pBase, M_REG_PERFORMANCE3, (uint8_t)u4Reg);

    // MT5368/MT5396/MT5389
    //0xf0050070[10]= 1 for Fs Hub + Ls Device
    u4Reg = MGC_Read8(pBase, (uint32_t)0x71);
    u4Reg |= 0x04;
    MGC_Write8(pBase, 0x71, (uint8_t)u4Reg)

    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase,0x604);
    u4Reg |= 0x01;
    MGC_Write32(pBase,0x604, u4Reg);

    // FS/LS Slew Rate change
    u4Reg = MGC_PHY_Read32(pBase, 0x10);
    u4Reg &= (uint32_t)(~0x000000ff);
    u4Reg |= (uint32_t)0x00000011;
    MGC_PHY_Write32(pBase, 0x10, u4Reg);


    // HS Slew Rate
    u4Reg = MGC_PHY_Read32(pBase, 0x10);
    u4Reg &= (uint32_t)(~0x00070000);
    u4Reg |= (uint32_t)0x00040000;
    MGC_PHY_Write32(pBase, 0x10, u4Reg);

    //TX_Current_EN setting 01 , 25:24 = 01 //enable TX current when
    //EN_HS_TX_I=1 or EN_HS_TERM=1
    u4Reg = MGC_PHY_Read32(pBase, 0x1C);
    u4Reg |= (uint32_t)0x01000000;
    MGC_PHY_Write32(pBase, 0x1C, u4Reg);


    // This is important: TM1_EN
    // speed up OTG setting for checking device connect event after MUC_Initial().
    u4Reg = MGC_Read32(pBase,0x604);
    u4Reg &= ~0x01;
    MGC_Write32(pBase, 0x604, u4Reg);

    // open session.
    nDevCtl = MGC_Read8(pBase, MGC_O_HDRC_DEVCTL);
    nDevCtl |= MGC_M_DEVCTL_SESSION;
    MGC_Write8(pBase, MGC_O_HDRC_DEVCTL, nDevCtl);

    LOG(0, "[USB]None pBase = 0x%08X init ok.\n", (uint32_t)pBase);

    return TRUE;
}
#endif //#if (defined CC_SUPPORT_STR) && (!defined(CONFIG_ARCH_MT85XX))

//-------------------------------------------------------------------------
/** MUSB_Resume
* Set resume usb bus.
*  @param  void.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MUSB_Resume(void)
{
#if (!defined CC_SUPPORT_STR) || (defined(CONFIG_ARCH_MT85XX))
    MUSB_Port *pUsbPort = NULL;
    MGC_Port *pPort = NULL;
    uint32_t i;

    for (i=0; i<MUSB_CountPorts(); i++)
    {
        LOG(5, "HS_HOST_RESUME port:%d.\n", i);

        pUsbPort = MUSB_GetPort(i);
        if (pUsbPort)
        {
            pPort = (MGC_Port *)pUsbPort->pPrivateData;
            MUSB_ASSERT(pPort);
            if (pPort)
            {
                MUSB_ResumeBus((MUSB_BusHandle)pPort);
            }
        }
    }

#else //#if (!defined CC_SUPPORT_STR) || (defined(CONFIG_ARCH_MT85XX))

 uint32_t *pBase;
 uint32_t dwFifoAddr = 0;
 int32_t bEnd=0;

 _fgMUSBInit = FALSE;
 _fgMUSBHostMode = FALSE;
 _fgMUSBDrvcustInit = FALSE;
_fgMUSBVBUSEnable = FALSE;

	_Musb_Port_Init();
	pBase=(uint32_t *)0xf0050000;
	MUSB_RESET(pBase);

	MGC_SelectEnd(pBase, 0);
	MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 3);
	MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 3);
	MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, 0);
	MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, 0);
	dwFifoAddr = MGC_END0_FIFOSIZE;

	 MGC_SelectEnd(pBase, 1);
	 MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 6);
	 MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 6);
	 MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
	 dwFifoAddr += 512;
	MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, dwFifoAddr/8);
	 dwFifoAddr += 512;

	MGC_SelectEnd(pBase, 2);
	MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 3);
	MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
	 dwFifoAddr += 64;

	 //unmark DMA interrupt  for channel 0 &1
	 MGC_Write8(pBase,0x203,0x3);

	 pBase=(uint32_t *)0xf0051000;
	MUSB_RESET(pBase);
	MGC_SelectEnd(pBase, 0);
	MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 3);
	MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 3);
	MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, 0);
	MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, 0);
	dwFifoAddr = MGC_END0_FIFOSIZE;

	for (bEnd = 1; bEnd < 6; bEnd++)
	  {
		  MGC_SelectEnd(pBase, bEnd);
		  MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 6);
		  MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 6);
		  MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, dwFifoAddr/8);
		  dwFifoAddr += 512;
		  MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, dwFifoAddr/8);
		  dwFifoAddr += 512;

	}

	//unmark DMA interrupt for channel 0 &1&2&3
	MGC_Write8(pBase,0x203,0xf);

	 _fgMUSBInit = TRUE;
	MUSB_SetPortMode(0, USB_PORT_TYPE_HOST);
	MUSB_SetPortMode(1, USB_PORT_TYPE_HOST);
	_Musb_VbusHandler(TRUE);
#endif //#if (!defined CC_SUPPORT_STR) || (defined(CONFIG_ARCH_MT85XX))

    return 0;
}
#ifdef CC_MT8563
//-------------------------------------------------------------------------
/** MUSB_Pdwnc
* USB power control
*  @port  port number, base 0.
*  @powerState  1 is power on, 0 is power down
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MUSB_Pdwnc(uint32_t port, uint32_t powerState)
{
    MUSB_Port *pUsbPort = NULL;
    MGC_Port *pPort = NULL;

    pUsbPort = MUSB_GetPort(port);
    if (pUsbPort)
    {
        pPort = (MGC_Port *)pUsbPort->pPrivateData;
        MUSB_ASSERT(pPort);
        if (pPort)
        {
            MUSB_PdwncBus((MUSB_BusHandle)pPort, powerState);
			if(powerState){
				MUSB_ResetPhy(port);
			}
        }
    }        
		
    return 0;
}
#endif
//-------------------------------------------------------------------------
/** MUSB_InitRateMonitor
* Initial variable of data rate monitor time.
*  @param void.
*  @retval void.
*/
//-------------------------------------------------------------------------
void MUSB_InitTimeMonitor(void)
{
#ifndef MUSB_8226
    // clear total time stamp.
    _rMsdTimeTotal.u4Micros = 0;
    _rMsdTimeTotal.u4Seconds = 0;
#endif
}
#if defined(CONFIG_ARCH_MT85XX)
void MUSB_InitTimeMonitorEx(UINT32 u4Lun)
{
    if(u4Lun >= MUSB_UCFS_UNIT_COUNT) return;

#ifndef MUSB_8226
    // clear total time stamp.
    _arMsdTimeTotal[u4Lun].u4Micros = 0;
    _arMsdTimeTotal[u4Lun].u4Seconds = 0;
#endif
}
#endif

//-------------------------------------------------------------------------
/** MUSB_InitRateMonitor
* Get total data rate monitor operation time.
*  @param void.
*  @retval  total operation time in micro second.
*/
//-------------------------------------------------------------------------
UINT64 MUSB_GetTimeMonitor(void)
{
    UINT64 u8Micros = 0;

#ifndef MUSB_8226

    u8Micros = ((UINT64)_rMsdTimeTotal.u4Seconds)*1000000;
    u8Micros += _rMsdTimeTotal.u4Micros;
#endif

    return u8Micros;
}

#if defined(CONFIG_ARCH_MT85XX)
UINT64 MUSB_GetTimeMonitorEx(UINT32 u4Lun)
{
    UINT64 u8Micros = 0;

    if(u4Lun >= MUSB_UCFS_UNIT_COUNT) return 0;

#ifndef MUSB_8226
    u8Micros = ((UINT64)_arMsdTimeTotal[u4Lun].u4Seconds)*1000000;
    u8Micros += _arMsdTimeTotal[u4Lun].u4Micros;
#endif

    return u8Micros;
}
#endif

//-------------------------------------------------------------------------
/** MUSB_SetPortMode
 *  Set USB port to host or device mode function.
 *  @param  u4PortIndex   port index 0 ~ (MUSB_CountPorts() - 1).
 *  @param  u4Type
            1: USB_PORT_TYPE_FUNCTION
            2: USB_PORT_TYPE_HOST
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL.
 */
//-------------------------------------------------------------------------
INT32 MUSB_SetPortMode(UINT32 u4PortIndex, UINT32 u4Type)
{
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;

    if ((!_fgMUSBInit) || (u4PortIndex >= MUSB_CountPorts()))
    {
        return -1;
    }

    pUsbPort = MUSB_GetPort(u4PortIndex);
    MUSB_ASSERT(pUsbPort);
    if (!pUsbPort)
    {
        return -1;
    }
    pUsbPort->Type = (MUSB_PortType)u4Type;
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    if (!pPort)
    {
        return -1;
    }

    // disable suspend before reset port.
    pPort->bWantSuspend = FALSE;
    pPort->pfProgramBusState(pPort);

    pPort->pfResetPort(pPort);

    if (u4Type == USB_PORT_TYPE_HOST)
    {
        // Turn on suspend mode in host mode.
        pPort->bWantSuspend = TRUE;

        pPort->bOtgState = (uint8_t)MUSB_A_WAIT_BCON;
    }
    else
    {
        pPort->bOtgState = (uint8_t)MUSB_AB_IDLE;
    }

#if defined(CONFIG_ARCH_MT85XX)
	if(u4Type != USB_PORT_TYPE_OTG)
#endif
	{
		pPort->bWantSession = TRUE;
	    pPort->pfProgramBusState(pPort);
	}

    LOG(0, "Set port-%d to %s mode.\n",
        u4PortIndex, ((u4Type == USB_PORT_TYPE_HOST) ? "Host" : "Device"));

    return 0;
}

#if defined(CONFIG_ARCH_MT85XX)
BOOL MUSB_fgIsInit(void)
{
    return _fgMUSBInit;
}
#endif

extern uint16_t MGC_FillHubClassPeripheralList(uint8_t *pList, uint16_t wListLength);
extern MUSB_DeviceDriver *MGC_GetHubClassDriver(void);
//-------------------------------------------------------------------------
/** MUSB_Init
 *  Set USB host or device mode function.
 *  @param  u4Mode		1: host, 0: device.
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL.
 */
//-------------------------------------------------------------------------
INT32 MUSB_Init(UINT32 u4Mode)
{
    UINT8 *pList;

    uint16_t wCount, wSize, wRemain;
    UINT8 bDriver;
    MUSB_DeviceDriver *pDriver = NULL;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_Port *pUsbPort = NULL;
    uint16_t i = 0;
    MUSB_BusHandle hUsbSession;

#ifdef USB_VBUS_CONT_P1_P2
     // perform driver customization.
    _Musb_DrvCust(u4Mode);
#endif
	
#ifdef CC_MT8563  //M1V1 EVB
	//USB PORT0 default give power, low electrical level is enable
	if(u4Mode == MUSB_HOST_MODE){
		Printf("[usb]mt8563 m1v1 power on\n");
		GPIO_Config(1, 1, 1); //USB PORT1
		GPIO_Config(20, 1, 1); //USB PORT1
	}
#endif

#if defined(CONFIG_ARCH_MT85XX)
    if (!_fgMUSBInit)    
    {
        _fgMUSBInit = TRUE;        

#else //#if defined(CONFIG_ARCH_MT85XX)
    if (_fgMUSBInit)
    {
        if ((u4Mode == MUSB_HOST_MODE) && (!_fgMUSBHostMode))
        {
            // host mode.
            _fgMUSBHostMode = TRUE;

            for (i=0; i<MUSB_CountPorts(); i++)
            {
                VERIFY(0 == MUSB_SetPortMode(i, USB_PORT_TYPE_HOST));
            }
#if (defined(MUSB_LOADER)||!(defined(__KERNEL__) && defined(__MODEL_slt__)))
            // turn on vbus for device stable.
            _Musb_VbusHandler(TRUE);
#endif

#ifdef MUSB_OC_SUPPORT
            // Set 1 sec timer to start monitor vbus over current.
            (void)x_timer_start(_hMUSBOCTimerHdl, 1000,
                X_TIMER_FLAG_ONCE, _Musb_OCTimerOut, NULL);
#endif
        }
        else if ((u4Mode == MUSB_DEVICE_MODE) && (_fgMUSBHostMode))
        {
            // device mode.
            _fgMUSBHostMode = FALSE;

#ifdef MUSB_OC_SUPPORT
            // Stop monitor vbus over current.
            (void)x_timer_stop(_hMUSBOCTimerHdl);
#endif

            // turn off vbus for device stable.
            _Musb_VbusHandler(FALSE);
        }
    }
    else
    {
        _fgMUSBInit = TRUE;

        _fgMUSBHostMode = (u4Mode) ? TRUE: FALSE;

#ifdef MUSB_PORT_CONFIG
	_Musb_Port_Init();
#endif

#ifdef MUSB_OC_SUPPORT
        // turn off vbus for device stable.
        _Musb_VbusHandler(FALSE);
#else

#ifdef MUSB_LOADER
        // In loader turn on vbus directly.
        HAL_GetTime(&_MusbT0);
#endif
		
#if (defined(MUSB_LOADER)||!(defined(__KERNEL__) && defined(__MODEL_slt__)))
        _Musb_VbusHandler(TRUE);
#endif
#endif

        // perform driver customization.
        _Musb_DrvCust(u4Mode);
#endif //#if defined(CONFIG_ARCH_MT85XX)

        if (!MUSB_InitSystem(5))
        {
            return -1;
        }

        /* fill driver table */
        bDriver = 0;
        wSize = 0;
        wCount = 0;
        wRemain = (uint16_t)sizeof(MGC_aUsbPeripheralList);
        pList = MGC_aUsbPeripheralList;

        #ifdef MUSB_IPOD
        wSize = MUSB_FillIpodPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MUSB_GetIpodClientDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }                            
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }
        #endif

        wSize = MGC_FillStorageClassPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MGC_GetStorageClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }

#ifndef MUSB_LOADER
	#ifdef MUSB_WIFI
        wSize = MUSB_FillWifiClassPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MUSB_GetWifiClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }                            
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }

        wSize = MUSB_FillWifiClassPeripheralList2(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MUSB_GetWifiClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }                            
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }

        wSize = MUSB_FillWifiClassPeripheralList3(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MUSB_GetWifiClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }                            
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }
	#endif // #ifdef MUSB_WIFI

#ifdef MUSB_HID
        wSize = MUSB_FillHidClassPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MUSB_GetHidClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }
#endif
#ifdef MUSB_SERIAL
			for (i=0; i<MUSB_SERIAL_LIST_NUM; i++)
			{
				wSize = MUSB_FillSerialClassPeripheralList(pList, wRemain, i);
				MUSB_ASSERT(wSize < wRemain);
				pDriver = MUSB_GetSerialClassDriver();
				MUSB_ASSERT(pDriver);
				if (bDriver >= MUSB_MAX_DRV_NUM)
				{
					LOG(0, "Out of max driver size.\n", NULL);
					return -1;
				}
				MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]),
					pDriver, sizeof(MUSB_DeviceDriver));
				pList += wSize;
				wCount += wSize + 1;
				if (wRemain < (wSize + 1))
				{
					LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
					return -1;
				}
				wRemain -= (wSize + 1);
				(*pList) = bDriver;
				pList ++;
			}
			bDriver ++;
#endif
#ifdef MUSB_AUD
        wSize = MUSB_FillAudClassPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MUSB_GetAudClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }
#endif

#ifdef MUSB_PL2303       
        {
            // Insert PL2303 to the driver table. 
            wSize = MUSB_FillPl2303PeripheralList(pList, wRemain);
            MUSB_ASSERT(wSize < wRemain);
            pDriver = MUSB_GetPl2303Driver();
            MUSB_ASSERT(pDriver);
            if (bDriver >= MUSB_MAX_DRV_NUM)
            {
                LOG(0, "Out of max driver size.\n", NULL);
                return -1;
            }                            
            MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), 
                pDriver, sizeof(MUSB_DeviceDriver));
            pList += wSize;
            wCount += wSize + 1;
            if (wRemain < (wSize + 1))
            {
                LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                return -1;
            }
            wRemain -= (wSize + 1);
            (*pList) = bDriver;
            pList ++;
        }
        bDriver ++;
#endif

#ifdef MUSB_SICD
        // Note this should put in the last driver to hook on system.
        // Because it will get Microsoft OS descriptor in SICD driver for
        // device of class code = 0xFF or 0x00.
        for (i=0; i<MUSB_SICD_LIST_NUM; i++)
        {
            wSize = MUSB_FillSicdPeripheralList(pList, wRemain, i);
            MUSB_ASSERT(wSize < wRemain);
            pDriver = MUSB_GetSicdDriver();
            MUSB_ASSERT(pDriver);
            if (bDriver >= MUSB_MAX_DRV_NUM)
            {
                LOG(0, "Out of max driver size.\n", NULL);
                return -1;
            }
            MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]),
                pDriver, sizeof(MUSB_DeviceDriver));
            pList += wSize;
            wCount += wSize + 1;
            if (wRemain < (wSize + 1))
            {
                LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                return -1;
            }
            wRemain -= (wSize + 1);
            (*pList) = bDriver;
            pList ++;
        }
        bDriver ++;
#endif

#ifdef CC_USB_CSR_BLUETOOTH
        wSize = MUSB_FillBtClassPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MUSB_GetBtClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }
#endif

#if defined(CONFIG_ARCH_MT85XX)
#ifdef MUSB_HUB
        wSize = MGC_FillHubClassPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MGC_GetHubClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }
#endif
#endif //#if defined(CONFIG_ARCH_MT85XX)

#endif  /* #ifndef MUSB_LOADER */

#if !defined(CONFIG_ARCH_MT85XX)
#ifdef MUSB_HUB
        wSize = MGC_FillHubClassPeripheralList(pList, wRemain);
        if (wSize < wRemain)
        {
            pDriver = MGC_GetHubClassDriver();
            if (pDriver)
            {
                if (bDriver >= MUSB_MAX_DRV_NUM)
                {
                    LOG(0, "Out of max driver size.\n", NULL);
                    return -1;
                }
                MUSB_MemCopy(&(MGC_UsbHostClient.aDeviceDriverList[bDriver]), pDriver,
                             sizeof(MUSB_DeviceDriver));

                pList += wSize;
                wCount += wSize + 1;
                if (wRemain < (wSize + 1))
                {
                    LOG(0, "Out of MGC_aUsbPeripheralList[] size.\n", NULL);
                    return -1;
                }
                wRemain -= (wSize + 1);
                (*pList) = bDriver++;
                pList ++;
            }
        }
#endif
#endif //#if !defined(CONFIG_ARCH_MT85XX)

        MGC_UsbHostClient.wPeripheralListLength = wCount;
        MGC_UsbHostClient.bDeviceDriverListLength = bDriver;

        for (i=0; i<MUSB_CountPorts(); i++)
        {
            pUsbPort = MUSB_GetPort(i);
            if (!pUsbPort)
            {
                return -1;
            }
            if (i >= MUSB_MAX_CONTROLLERS)
            {
                return -1;
            }
            MGC_pUsbPort[i] = pUsbPort;

            /* start session */
            hUsbSession = MUSB_RegisterHostClient(pUsbPort, &MGC_UsbHostClient);
            if (!hUsbSession)
            {
                return -1;
            }
            MGC_pImplPort[i] = (MGC_Port *)hUsbSession;

            // fix warning.
            UNUSED(MGC_pUsbPort[i]);
            UNUSED(MGC_pImplPort[i]);

#if !defined(CONFIG_ARCH_MT85XX)
#ifdef MUSB_FUNCTION 
            hUsbSession = MUSB_RegisterFunctionClient(pUsbPort, &MGC_GenericUsbFunctionClient);
            if (!hUsbSession)
            {
                return -1;
            }

            if (u4Mode == 0)
            {
                MUSB_GenericDevInit();
            }
#endif /* #ifdef MUSB_FUNCTION */
#endif //#if !defined(CONFIG_ARCH_MT85XX)

            // Set port mode.
            VERIFY(0 == MUSB_SetPortMode(i,
                ((u4Mode) ? USB_PORT_TYPE_HOST : USB_PORT_TYPE_FUNCTION)));
        }

        // get Hfi driver structure.
        pHfiDriver = MUSB_HfiDriverGet();
        if (!pHfiDriver)
        {
            return -1;
        }

        // init device.
        if (0 > pHfiDriver->pfInitDevice(0))
        {
            return -1;
        }

#ifdef MUSB_SICD
        pHfiDriver = MUSB_HfiSicdDriverGet();
        if (!pHfiDriver)
        {
            return -1;
        }

        // init device.
        if (0 > pHfiDriver->pfInitDevice(0))
        {
            return -1;
        }
#endif /* #ifdef MUSB_SICD */
#if defined(CONFIG_ARCH_MT85XX)
#ifdef MUSB_HID
        pHfiDriver = MUSB_HfiHidDriverGet();
        if (!pHfiDriver)
        {
            return -1;
        }

        // init device.
        if (0 > pHfiDriver->pfInitDevice(0))
        {
            return -1;
        }
#endif /* #ifdef MUSB_SICD */
#endif
#ifdef MUSB_IPOD
        pHfiDriver = MUSB_HfiIpodDriverGet();
        if (!pHfiDriver)
        {
            return -1;
        }

        // init device.
        if (0 > pHfiDriver->pfInitDevice(0))
        {
            return -1;
        }
#endif /* #ifdef MUSB_IPOD */

        return 0;
    }

    if (u4Mode)
    {
        for (i=0; i<MUSB_CountPorts(); i++)
        {
            VERIFY(0 == MUSB_SetPortMode(i, USB_PORT_TYPE_HOST));
        }
    }

    return 0;
}

//-------------------------------------------------------------------------
/** MUSB_InitDeviceMode
 *  Set USB device mode function.
 *  @param  u4Mode
 *      MUSB_DEVICE_MODE_CAP         (0)
 *      MUSB_DEVICE_MODE_CDC        (1)
 *      MUSB_DEVICE_MODE_MSD        (2)
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL.
 */
//-------------------------------------------------------------------------
#ifdef MUSB_FUNCTION
INT32 MUSB_InitDeviceMode(UINT32 u4Mode)
{
    MUSB_Port *pUsbPort = NULL;
    uint16_t i = 0;
    MUSB_BusHandle hUsbSession;

    // set to host/device mode.
    for (i=0; i<MUSB_CountPorts(); i++)
    {
        if (i >= MUSB_MAX_CONTROLLERS)
        {
            return -1;
        }

        VERIFY(0 == MUSB_SetPortMode(i, USB_PORT_TYPE_FUNCTION));
        pUsbPort = MUSB_GetPort(i);
        if (!pUsbPort)
        {
            return -1;
        }

        switch (u4Mode)
        {
            case MUSB_DEVICE_MODE_CAP:
#if !defined(CONFIG_ARCH_MT85XX)
                hUsbSession = MUSB_RegisterFunctionClient(
                    pUsbPort, &MGC_GenericUsbFunctionClient);
                if (!hUsbSession)
                {
                    return -1;
                }
                MGC_pImplPort[i] = (MGC_Port *)hUsbSession;
                MUSB_GenericDevInit();
                break;
#endif
            case MUSB_DEVICE_MODE_CDC:
                hUsbSession = MUSB_RegisterFunctionClient(
                    pUsbPort, &MGC_CdcFunctionClient);
                if (!hUsbSession)
                {
                    return -1;
                }
                MGC_pImplPort[i] = (MGC_Port *)hUsbSession;
                MUSB_CdcInit();
                break;
            case MUSB_DEVICE_MODE_MSD:
                hUsbSession = MUSB_RegisterFunctionClient(
                    pUsbPort, &MGC_MsdFunctionClient);
                if (!hUsbSession)
                {
                    return -1;
                }
                MGC_pImplPort[i] = (MGC_Port *)hUsbSession;
                break;

            default:
                break;
        }
    }

    return 0;
}
#endif

#if defined(CONFIG_ARCH_MT85XX)
INT32 i4UsbHostInit(void)
{
     return MUSB_Init(MUSB_HOST_MODE);
}
#endif

#if ((defined(SUPPORT_USB30)) && !defined(__KERNEL__)) //defined(CC_MT5399)
extern INT32 MUSB3_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
#endif

//-------------------------------------------------------------------------
/** MUSB_CheckSLT
 *  USB SLT check function.
 *  @param  void.
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL.
 */
//-------------------------------------------------------------------------
INT32 MUSB_CheckSLT(void)
{
    static UINT8 _u1SltBuf[MUSB_SLT_SIZE];
    INT32 i4Pass = 0;
    INT32 i = 0;
    INT32 i4Lun;

    /*
        SLT checks an always connected MSD device the following item:
        1. device standard enumeration ok.
        2. MSD enumeration ok.
        3. compare MSD information to well known constant data (Not implement now).
    */
    i4Lun = MUSB_GetLun();

    // 5361B only one port.
#ifndef CC_LINUX_SLT_SUPPORT
    for (i=0; i<(INT32)MUSB_CountPorts(); i++)
#else
	for (i=0; i<4; i++)
#endif
    {
        (void)MUSB_SetLun((UINT32)i);
#ifdef SUPPORT_USB30 //defined(CC_MT5399)
#ifndef CC_MTK_LOADER
#ifdef __KERNEL__ 
        if(i==3) //SSUSB
        {
            if (MGC_Read32(0xf0000000, 0x707b4) == 0x09060005)
            {
                LOG(0, "USB2 is USB3.0 slt pass. Detect USB3.0 device!\n");
            }
            else if (MGC_Read32(0xf0000000, 0x707b4) == 0x09060003)
            {
                LOG(0, "USB2 is USB3.0 slt pass. Detect USB2.0 device!\n");
            }
            else
            {
                LOG(0, "USB2 is USB3.0 slt fail.\n");
            }
        }
        else
#else
        if(i==3) //SSUSB
        {
				if (MUSB3_Read((UINT64)0, (UINT32)_u1SltBuf, MUSB_SLT_SIZE) != 0)
            {
                LOG(0, "USB%d is USB3.0 slt fail.\n", i);
                i4Pass = -1;
            }
            else
            {
                LOG(0, "USB%d is USB3.0 slt pass.\n", i);
            }
        }
        else
#endif
#endif
        {
            if (MUSB_Read((UINT64)0, (UINT32)_u1SltBuf, MUSB_SLT_SIZE) != 0)
            {
                LOG(0, "USB%d slt fail.\n", i);
                i4Pass = -1;
            }
            else
            {
                LOG(0, "USB%d slt pass.\n", i);
            }
        }
#else
        if (MUSB_Read((UINT64)0, (UINT32)_u1SltBuf, MUSB_SLT_SIZE) != 0)
        {
            LOG(0, "USB%d slt fail.\n", i);
            i4Pass = -1;
        }
        else
        {
            LOG(0, "USB%d slt pass.\n", i);
        }
#endif
    }

    (void)MUSB_SetLun((UINT32)i4Lun);

    return i4Pass;
}

#if defined(CONFIG_ARCH_MT85XX)
//-------------------------------------------------------------------------
/** fgUsbSetMediaInUse
 *  8226 porting layer: check lun in device is in used function.
 *  @param  bMediaNum lun number.
 *  @retval  0 	USBM_MEDIA_STS_ABSENT.
 *  @retval  1 	USBM_MEDIA_STS_PRESENT.
 *  @retval  2	USBM_MEDIA_STS_WR_PROTECT.
 *  @retval  3   USBM_MEDIA_STS_ERROR. 
 */
//-------------------------------------------------------------------------
INT32 eUsbMediaStatus(BYTE bMediaNum)
{
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    UINT32 u4Unit = bMediaNum;
    INT32 i4Status;
    MUSB_HfiMediumInfo *pMediumInfo;

    if (!_fgMUSBInit)
    {
        return 0; //USBM_MEDIA_STS_ABSENT
    }

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

    if (!pHfiDriver)
    {
        return 0; //USBM_MEDIA_STS_ABSENT
    }

    // check device status.
    if (0 > pHfiDriver->pfIoCtl(u4Unit, (uint32_t)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &i4Status))
    {
        return 0; //USBM_MEDIA_STS_ABSENT
    }

    if (HFI_STATUS_READY != i4Status)
    {    
        return 0; //USBM_MEDIA_STS_ABSENT
    }

    // get medium information.
    if (0 > pHfiDriver->pfIoCtl(u4Unit, (uint32_t)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
    {    
        return 3; //USBM_MEDIA_STS_ABSENT
    }

    if (pMediumInfo->AccessType == MUSB_HFI_ACCESS_RANDOM_READ)
    {
        return 2; //USBM_MEDIA_STS_WR_PROTECT
    }

    return 1; //USBM_MEDIA_STS_PRESENT;
}

BOOL fgUsbVbusEnable(uint8_t bPortNum, BOOL fgEnable)
{
#ifdef USB_VBUS_CONT_P1_P2
    uint32_t i4GpioNum = 0;
    uint32_t i4High = 0;

    if(bPortNum == 0)
    {
        i4GpioNum = USB_VBUS_PCONT1;
    }
    else if(bPortNum == 1)
    {
        i4GpioNum = USB_VBUS_PCONT2;
    }
    else
    {
        return FALSE;
    }

    if(fgEnable == TRUE)
    {
        i4High = HIGH;
    }
    else if(fgEnable == FALSE)
    {
        i4High = LOW;
    }
    else
    {
        return FALSE;
    }

    GPIO_Output(i4GpioNum, i4High);
#endif

    return TRUE;
}
#endif //#if defined(CONFIG_ARCH_MT85XX)

#if !defined(CONFIG_ARCH_MT85XX)
//-------------------------------------------------------------------------
/** MUSB_TurnOffVBus
 *  Turn off USB 5V.
 *  @param  void.
 *  @retval void.
 */
//-------------------------------------------------------------------------
void MUSB_TurnOffVBus(void)
{
    _Musb_VbusHandler(FALSE);
}
//-------------------------------------------------------------------------
/** MUSB_TurnOnVBus
 *  Turn on USB 5V.
 *  @param  void.
 *  @retval void.
 */
//-------------------------------------------------------------------------
void MUSB_TurnOnVBus(void)
{
    _Musb_VbusHandler(TRUE);
}

//-------------------------------------------------------------------------
/** MUSB_GetTotalPort
* Get total number of port number on the platform.
*  @param  void.
*  @retval   total number of port number on the platform.
*/
//-------------------------------------------------------------------------
UINT32 MUSB_GetTotalPort(void)
{
    return (UINT32)MUSB_CountPorts();
}

#ifdef MUSB_SERIAL
BOOL MUSB_Uart_Cli_Support(void)
{
	return MUSB_Serial_Cli_Support();
}

uint32_t MUSB_Uart_Read(uint8_t *pBuffer, uint32_t u4NumToRead)
{
    uint32_t u4Lun = 0;
	uint32_t u4RecRealDataSZ = 0;
    MU_SERIAL_DEVICE_TYPE *pSerialDriver = NULL;

	// get Hfi driver structure.
	pSerialDriver = MUSB_SerialDriverGet();

	u4RecRealDataSZ = pSerialDriver->pfSerialBufGets(u4Lun, pBuffer, u4NumToRead);

	return u4RecRealDataSZ;
};

#if 1
void MUSB_Uart_PutChar(uint8_t u1Char)
{
    MU_SERIAL_DEVICE_TYPE *pSerialDriver = NULL;

	#if 1
	if(!MUSB_PL2303GetInsert(0))
		return;
	#endif

	// get Serial driver structure.
	pSerialDriver = MUSB_SerialDriverGet();

	if(pSerialDriver->pfSerialBufPut(u1Char))
	{
	    return;
	}

	return;
}

#else
/*It can not be used, because UART_ISR will show log when enter "TAB",
it will be lock when use pfSerialPuts()*/

void MUSB_Uart_PutChar(uint8_t u1Char)
{
    MU_SERIAL_DEVICE_TYPE *pSerialDriver = NULL;
	uint8_t *SrcBuf;
	uint8_t ReadBuffer[1];

    //INT32 i4Status;
	//LOG(1,"Serial Bulk Out...\n", NULL);

	if(!MUSB_PL2303GetInsert(0))
		return;

	// get Serial driver structure.
	pSerialDriver = MUSB_SerialDriverGet();
	ReadBuffer[0] = u1Char;
	SrcBuf = (uint8_t *)ReadBuffer;
	pSerialDriver->pfSerialPuts(0, SrcBuf, 1);
	return;
}
#endif
#endif //#ifdef MUSB_SERIAL
#endif //#if !defined(CONFIG_ARCH_MT85XX)
#endif

