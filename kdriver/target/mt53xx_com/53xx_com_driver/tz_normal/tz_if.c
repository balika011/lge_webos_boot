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
 * $Author: p4admin $
 * $Date: 2015/02/24 $
 * $RCSfile: dmx_if.c,v $
 * $Revision: #6 $
 *
 *---------------------------------------------------------------------------*/

/** @file tz_if.c
 *  trustzone driver - public interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "tz.h"
#include "tz_if.h"
#include "x_lint.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_hal_arm.h"
#include "drvcust_if.h"
#include "tz_wv_if.h"

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
typedef enum
{
    TZ_SEMA_IDX_TVP = 0,

    TZ_SEMA_IDX_MAX
} TZ_SEMA_IDX_T;

//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
static void TZ_LockApi(TZ_SEMA_IDX_T eIdx);
static void TZ_UnlockApi(TZ_SEMA_IDX_T eIdx);

extern BOOL _TZ_CTL(UINT32 u4Cmd, UINT32 u4PAddr, UINT32 u4Size);

// Any TZ_XXX APIs that trigger GCPU HW inside secure world
// must lock GCPU HW resouce by GCPU_HW_LOCK() before getting into secure world
// and unlock GCPU HW resource by GCPU_HW_UNLOCK() after getting back to normal world
extern void GCPU_HW_LOCK(void);
extern void GCPU_HW_UNLOCK(void);

void TZ_NWD_BufferStart(UINT32 u4VAddr, UINT32 u4Size);
void TZ_NWD_BufferEnd(UINT32 u4VAddr, UINT32 u4Size);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HANDLE_T _ahTzApiSem[TZ_SEMA_IDX_MAX] = {NULL_HANDLE};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void TZ_LockApi(TZ_SEMA_IDX_T eIdx)
{
    ASSERT(eIdx < TZ_SEMA_IDX_MAX);

    if (NULL_HANDLE != _ahTzApiSem[eIdx])
    {
        VERIFY(x_sema_lock(_ahTzApiSem[eIdx], X_SEMA_OPTION_WAIT) == OSR_OK);
    }
    else
    {
        static BOOL _fgInit[TZ_SEMA_IDX_MAX];
        CRIT_STATE_T rCritState;
        BOOL fgTempInit;

        rCritState = x_crit_start();
        fgTempInit = _fgInit[eIdx];
        _fgInit[eIdx] = TRUE;
        x_crit_end(rCritState);

        if (!fgTempInit)
        {
            VERIFY(x_sema_create(&_ahTzApiSem[eIdx], X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
        }

        VERIFY(x_sema_lock(_ahTzApiSem[eIdx], X_SEMA_OPTION_WAIT) == OSR_OK);
    }
}

static void TZ_UnlockApi(TZ_SEMA_IDX_T eIdx)
{
    ASSERT(eIdx < TZ_SEMA_IDX_MAX);

    VERIFY(x_sema_unlock(_ahTzApiSem[eIdx]) == OSR_OK);
}

static BOOL TZ_CTL(UINT32 u4Cmd, UINT32 u4PAddr, UINT32 u4Size)
{
    BOOL fgRet;

#if 0 //def CONFIG_SMP
    HANDLE_T hdl;
    UINT32 oid, nid;

    oid = smp_processor_id();
    x_thread_self(&hdl);
    if (hdl)
    {
        x_thread_set_affinity(hdl, 1);
    }
    nid = smp_processor_id();

    if (oid != nid)
    {
        Printf("cpu switch:[%d]->[%d]\n", oid, nid);
    }
#endif

    switch (u4Cmd) {
    case TZCTL_TEE_PLAYREADY_CONTENT_AESCTR_DECRYPT:
    case TZCTL_TEE_PLAYREADY_MULTI_CONTENT_AESCTR_DECRYPT:
    case TZCTL_TEE_PLAYREADY_DASH_SAMPLE_AESCTR_DECRYPT:
    case TZCTL_TEE_PLAYREADY_DASH_SAMPLE_AESCTR_DECRYPT_AUDIO:
    case TZCTL_OEMCrypto_DecryptCTR:
    case TZCTL_OEMCrypto_DecryptVideo:
    case TZCTL_TEE_PLAYREADY_MULTI_CONTENT_SECUREBUFFER_DECRYPT:
        GCPU_HW_LOCK();
        break;
    }

    if (u4PAddr == 0 || u4Size == 0)
    {
		fgRet = _TZ_CTL(u4Cmd, u4PAddr, u4Size);
    }
    else
    {
    	TZ_NWD_BufferStart(VIRTUAL(u4PAddr), u4Size);
    	fgRet = _TZ_CTL(u4Cmd, u4PAddr, u4Size);
    	TZ_NWD_BufferEnd(VIRTUAL(u4PAddr), u4Size);
    }

    switch (u4Cmd) {
    case TZCTL_TEE_PLAYREADY_CONTENT_AESCTR_DECRYPT:
    case TZCTL_TEE_PLAYREADY_MULTI_CONTENT_AESCTR_DECRYPT:
    case TZCTL_TEE_PLAYREADY_DASH_SAMPLE_AESCTR_DECRYPT:
    case TZCTL_TEE_PLAYREADY_DASH_SAMPLE_AESCTR_DECRYPT_AUDIO:
    case TZCTL_OEMCrypto_DecryptCTR:
    case TZCTL_OEMCrypto_DecryptVideo:
    case TZCTL_TEE_PLAYREADY_MULTI_CONTENT_SECUREBUFFER_DECRYPT:
        GCPU_HW_UNLOCK();
        break;
    }

    return fgRet;
}

static BOOL TZ_CTL_Lock(TZ_SEMA_IDX_T eIdx, UINT32 u4Cmd, UINT32 u4PAddr, UINT32 u4Size)
{
    BOOL fgRet;

    TZ_LockApi(eIdx);
    fgRet = TZ_CTL(u4Cmd, u4PAddr, u4Size);
    TZ_UnlockApi(eIdx);

    return fgRet;
}

////-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
BOOL TZ_IsTrustzoneEnable(void)
{
    UINT32 u4TzEnable;

    if (0 == DRVCUST_InitQuery(eTrustzoneEnable, &u4TzEnable))
    {
        return (u4TzEnable == 1)?TRUE:FALSE;
    }

    return FALSE;
}

void TZ_NWD_BufferStart(UINT32 u4VAddr, UINT32 u4Size)
{
    if (((u4VAddr & TZ_CACHE_LINE_SIZE_MSK) != 0) || (((u4Size & TZ_CACHE_LINE_SIZE_MSK) != 0)))
    {
        Printf("tz0 buffer is not cache line alignment: addr=0x%x, size=0x%x!\n", u4VAddr, u4Size);
        while(1);
    }

    HalFlushInvalidateDCacheMultipleLine(u4VAddr, u4Size);
}

void TZ_NWD_BufferEnd(UINT32 u4VAddr, UINT32 u4Size)
{
    if (((u4VAddr & TZ_CACHE_LINE_SIZE_MSK) != 0) || (((u4Size & TZ_CACHE_LINE_SIZE_MSK) != 0)))
    {
        Printf("tz1 buffer is not cache line alignment: addr=0x%x, size=0x%x!\n", u4VAddr, u4Size);
        while(1);
    }

    HalInvalidateDCacheMultipleLine(u4VAddr, u4Size);
}

void TZ_EnableDebug(UINT32 u4Enable,UINT32 sec_start,UINT32 sec_end)
{
    UINT32 u4BufVAddr, u4BufPAddr;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(UINT32), TZ_CACHE_LINE_SIZE);
    u4BufVAddr = VIRTUAL(u4BufPAddr);
    Printf("u4BufPAddr=0x%x, u4BufVAddr=0x%x\n", u4BufPAddr, u4BufVAddr);

    *(UINT32 *)u4BufVAddr = u4Enable;
	*(UINT32 *)(u4BufVAddr+4) = sec_start;
	*(UINT32 *)(u4BufVAddr+8) = sec_end;
    TZ_CTL(TZCTL_SYSTEM_ENABLE_DEBUG, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(UINT32)));

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    Printf("tz_debug_on=%d\n", u4Enable);
}

// tz unit test
void TZ_RunUT(UINT32 u4Count)
{
    UINT32 u4BufVAddr, u4BufPAddr;
    UINT32 i, u4Ret, u4Size = 256;
    UINT32 t1, t2, td;



    UNUSED(u4Ret);
    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4Size, TZ_CACHE_LINE_SIZE);
    u4BufVAddr = VIRTUAL(u4BufPAddr);
    Printf("u4BufPAddr=0x%x, u4BufVAddr=0x%x\n", u4BufPAddr, u4BufVAddr);

    for (i = 0; i < u4Size; i++)
    {
        ((UINT8*)(u4BufVAddr))[i] = i;
    }

    u4Ret = TZ_CTL(TZCTL_SYSTEM_REVERSE_ARRAY, u4BufPAddr, TZ_CACHE_ALIGN(u4Size));

    for (i = 0; i < u4Size; i++)
    {
        if (i%16 == 0)
        {
            Printf("\n");
        }

        Printf("%02x ", ((UINT8*)(u4BufVAddr))[i]);
        if (((UINT8*)(u4BufVAddr))[i] != (u4Size-1-i))
        {
            while (1);
        }
    }
    Printf("\n");

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    t1 = ~BIM_READ32(REG_RW_TIMER2_LOW);
    for (i = 0; i < u4Count; i++)
    {
        TZ_CTL(TZCTL_SYSTEM_ECHO, 0, 0);
    }
    t2 = ~BIM_READ32(REG_RW_TIMER2_LOW);

    td = (t1>t2)?(~(t1)+t2):(t2-t1);
    Printf("%d times echos: ms=%d, tick=%d\n", u4Count, td/27000, td);



}

// tz stress test
void TZ_RunST(void)
{
    UINT32 u4BufVAddr, u4BufPAddr;
    UINT32 i, u4Ret, u4Size = 256;

    UNUSED(u4Ret);
    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4Size, TZ_CACHE_LINE_SIZE);
    u4BufVAddr = VIRTUAL(u4BufPAddr);

    for (i = 0; i < u4Size; i++)
    {
        ((UINT8*)(u4BufVAddr))[i] = i;
    }

    u4Ret = TZ_CTL(TZCTL_SYSTEM_REVERSE_ARRAY, u4BufPAddr, TZ_CACHE_ALIGN(u4Size));

    for (i = 0; i < u4Size; i++)
    {
        if (((UINT8*)(u4BufVAddr))[i] != (u4Size-1-i))
        {
            Printf("tz stress test fail!!!\n");
            while (1);
        }
    }

    BSP_FreeAlignedDmaMemory(u4BufPAddr);
}

#ifndef CC_NDEBUG
BOOL TZ_IO_WRITE32(UINT32 u4Base, UINT32 u4Offset, UINT32 u4Val)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
    TZ_IO_T *pIo;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(TZ_IO_T), TZ_CACHE_LINE_SIZE);
    pIo = (TZ_IO_T *)(VIRTUAL(u4BufPAddr));
    pIo->u4Base = u4Base;
    pIo->u4Offset = u4Offset;
    pIo->u4Val = u4Val;

    fgRet = TZ_CTL(TZCTL_SYSTEM_IO_WRITE32, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(TZ_IO_T)));

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_IO_WRITE16(UINT32 u4Base, UINT32 u4Offset, UINT16 u2Val)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
    TZ_IO_T *pIo;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(TZ_IO_T), TZ_CACHE_LINE_SIZE);
    pIo = (TZ_IO_T *)(VIRTUAL(u4BufPAddr));
    pIo->u4Base = u4Base;
    pIo->u4Offset = u4Offset;
    pIo->u4Val = (UINT32)u2Val;

    fgRet = TZ_CTL(TZCTL_SYSTEM_IO_WRITE16, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(TZ_IO_T)));

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_IO_WRITE8(UINT32 u4Base, UINT32 u4Offset, UINT8 u1Val)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
    TZ_IO_T *pIo;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(TZ_IO_T), TZ_CACHE_LINE_SIZE);
    pIo = (TZ_IO_T *)(VIRTUAL(u4BufPAddr));
    pIo->u4Base = u4Base;
    pIo->u4Offset = u4Offset;
    pIo->u4Val = (UINT32)u1Val;

    fgRet = TZ_CTL(TZCTL_SYSTEM_IO_WRITE8, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(TZ_IO_T)));

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_IO_READ32(UINT32 u4Base, UINT32 u4Offset, UINT32 *pu4Val)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
    TZ_IO_T *pIo;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(TZ_IO_T), TZ_CACHE_LINE_SIZE);
    pIo = (TZ_IO_T *)(VIRTUAL(u4BufPAddr));
    pIo->u4Base = u4Base;
    pIo->u4Offset = u4Offset;

    fgRet = TZ_CTL(TZCTL_SYSTEM_IO_READ32, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(TZ_IO_T)));

    *pu4Val = pIo->u4Val;
    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_IO_READ16(UINT32 u4Base, UINT32 u4Offset, UINT16 *pu2Val)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
    TZ_IO_T *pIo;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(TZ_IO_T), TZ_CACHE_LINE_SIZE);
    pIo = (TZ_IO_T *)(VIRTUAL(u4BufPAddr));
    pIo->u4Base = u4Base;
    pIo->u4Offset = u4Offset;

    fgRet = TZ_CTL(TZCTL_SYSTEM_IO_READ16, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(TZ_IO_T)));

    *pu2Val = (UINT16)pIo->u4Val;
    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_IO_READ8(UINT32 u4Base, UINT32 u4Offset, UINT8 *pu1Val)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
    TZ_IO_T *pIo;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(TZ_IO_T), TZ_CACHE_LINE_SIZE);
    pIo = (TZ_IO_T *)(VIRTUAL(u4BufPAddr));
    pIo->u4Base = u4Base;
    pIo->u4Offset = u4Offset;

    fgRet = TZ_CTL(TZCTL_SYSTEM_IO_READ8, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(TZ_IO_T)));

    *pu1Val = (UINT8)pIo->u4Val;
    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}
#endif // CC_NDEBUG

BOOL TZ_DRM_WV_SetAssetKey(UINT8 *pu1Key, UINT32 u4KeyLen)
{
    BOOL fgRet;
    UINT32 u4BufPAddr, u4BufVAddr;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4KeyLen, TZ_CACHE_LINE_SIZE);
    u4BufVAddr = (UINT32)(VIRTUAL(u4BufPAddr));
    x_memcpy((void *)u4BufVAddr, pu1Key, u4KeyLen);

    fgRet = TZ_CTL(TZCTL_WV_SET_ASSET_KEY, u4BufPAddr, TZ_CACHE_ALIGN(u4KeyLen));

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_DRM_WV_DeriveCW(UINT8 *pu1Ecm, UINT32 *pu4Flags)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
    TZ_WV_DERIVECW_T *pWv;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(TZ_WV_DERIVECW_T), TZ_CACHE_LINE_SIZE);
    pWv = (TZ_WV_DERIVECW_T *)(VIRTUAL(u4BufPAddr));
    x_memcpy(pWv->au1Ecm, pu1Ecm, 32);

    fgRet = TZ_CTL(TZCTL_WV_SET_DERIVE_CW, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(TZ_WV_DERIVECW_T)));

    *pu4Flags = pWv->u4Flags;
    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_DRM_WV_PassKeybox(UINT8 *pu1Keybox, UINT32 u4Size)
{
    BOOL fgRet;
    UINT32 u4BufPAddr, u4BufVAddr;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4Size, TZ_CACHE_LINE_SIZE);
    u4BufVAddr = (UINT32)(VIRTUAL(u4BufPAddr));
    x_memcpy((void *)u4BufVAddr, pu1Keybox, u4Size);

    fgRet = TZ_CTL(TZCTL_WV_PASS_KEYBOX, u4BufPAddr, TZ_CACHE_ALIGN(u4Size));

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

///////////////////////////////////for hulu+dash+wv drm only begin//////////////
BOOL TZ_DRM_WV_ValidateKeybox(UINT32 *result)
{
	BOOL fgRet;
    UINT32 u4BufPAddr, u4BufVAddr;

	//printf("TZ_DRM_WV_ValidateKeybox enter \n");

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(UINT32), TZ_CACHE_LINE_SIZE);
    u4BufVAddr = (UINT32)(VIRTUAL(u4BufPAddr));

	if (!u4BufVAddr)
	{
		printf("TZ_DRM_WV_ValidateKeybox Allocate  failed\n");
		return FALSE;
	} 

    fgRet = TZ_CTL(TZCTL_WV_VALIDATE_KEYBOX, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(UINT32)));

	*result = *(UINT32 *)u4BufVAddr;

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_DRM_WV_CreatSession(UINT32 *sesscnt)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
	WV_IOCTL_SESS_T *u4BufVAddr;
	
	//printf("TZ_DRM_WV_CreatSession enter \n");

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(WV_IOCTL_SESS_T), TZ_CACHE_LINE_SIZE);
    u4BufVAddr = (WV_IOCTL_SESS_T *)(VIRTUAL(u4BufPAddr));
    
	if (!u4BufVAddr)
	{
		printf("TZ_DRM_WV_CreatSession Allocate SessionContext failed\n");
		return FALSE;
	} 

	u4BufVAddr->sessaddr = 0;

    fgRet = TZ_CTL(TZCTL_WV_CREATESESSION, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(WV_IOCTL_SESS_T)));

	*sesscnt = u4BufVAddr->sessaddr;
	
	//printf("TZ_DRM_WV_CreatSession u4BufVAddr->sessaddr=0x%x, sessaddr=0x%x \n",u4BufVAddr->sessaddr, *sesscnt);

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}
BOOL TZ_DRM_WV_DestroySession(SessionContext *sesscnt)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
	WV_IOCTL_SESS_T *u4BufVAddr;

	//printf("TZ_DRM_WV_DestroySession enter \n");

	u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(WV_IOCTL_SESS_T), TZ_CACHE_LINE_SIZE);
	u4BufVAddr = (WV_IOCTL_SESS_T *)(VIRTUAL(u4BufPAddr));
	
	if (!u4BufVAddr)
	{
		printf("TZ_DRM_WV_DestroySession Allocate SessionContext failed\n");
		return FALSE;
	} 
	
	u4BufVAddr->sessaddr = (UINT32)sesscnt;
	//printf("TZ_DRM_WV_DestroySession sessaddr=0x%x \n",u4BufVAddr->sessaddr);

    fgRet = TZ_CTL(TZCTL_WV_DESTROYSESSION, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(WV_IOCTL_SESS_T)));

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_DRM_WV_GenerateNonceSess(SessionContext * sesscnt, uint32_t * nonce)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
	WV_IOCTL_NONCE_T *pNonce;

	//printf("TZ_DRM_WV_GenerateNonceSess enter \n");

	u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(WV_IOCTL_NONCE_T), TZ_CACHE_LINE_SIZE);
	pNonce = (WV_IOCTL_NONCE_T *)(VIRTUAL(u4BufPAddr));
	
	if (!pNonce)
	{
		printf("TZ_DRM_WV_GenerateNonceSess Allocate WV_IOCTL_NONCE_T failed\n");
		return FALSE;
	} 
	
	pNonce->SessionContext = (UINT32)sesscnt;
	pNonce->u4Nonce = 0;
	
    fgRet = TZ_CTL(TZCTL_WV_GENERATE_NONCE, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(WV_IOCTL_NONCE_T)));
	
	*nonce = pNonce->u4Nonce;
	//printf("TZ_DRM_WV_GenerateNonceSess nonce=0x%x \n",*nonce);

    BSP_FreeAlignedDmaMemory(u4BufPAddr);
	
	return fgRet;
}

BOOL TZ_DRM_WV_GenerateDeriveKeys(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_GENERATE_DERIVEKEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_GenerateSignature(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_GENERATE_SIGNATURE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_ValidateMessage(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_VALIDATEM_MESSAGE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}


BOOL TZ_DRM_WV_InstallKey(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_INSTALL_KEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_FlushNoncesSess(SessionContext * sesscnt)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
	WV_IOCTL_SESS_T *u4BufVAddr;

	//printf("TZ_DRM_WV_FlushNoncesSess enter \n");

	u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(WV_IOCTL_SESS_T), TZ_CACHE_LINE_SIZE);
	u4BufVAddr = (WV_IOCTL_SESS_T *)(VIRTUAL(u4BufPAddr));
	
	if (!u4BufVAddr)
	{
		printf("TZ_DRM_WV_FlushNoncesSess Allocate WV_IOCTL_SESS_T failed\n");
		return FALSE;
	} 
	
	u4BufVAddr->sessaddr = (UINT32)sesscnt;

    fgRet = TZ_CTL(TZCTL_WV_FLUSH_NONCE, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(WV_IOCTL_SESS_T)));

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;

}

BOOL TZ_DRM_WV_UpdateMacKeys(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_UPDATE_MACKEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_RefreshKey(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_REFRESH_KEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_SelectContentKey(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_SELECT_CONTENT_KEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_CheckDecCTRContentKey(SessionContext *sesscnt,uint32_t curtime,BufferType buffer_type,unsigned short * tmp_slot)
{
	BOOL fgRet;
	UINT32 u4BufPAddr;
	WV_IOCTL_CHK_KEY_T *u4BufVAddr;

	u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(WV_IOCTL_CHK_KEY_T), TZ_CACHE_LINE_SIZE);
	u4BufVAddr = (WV_IOCTL_CHK_KEY_T *)(VIRTUAL(u4BufPAddr));
	if (!u4BufVAddr)
	{
		printf("TZ_DRM_WV_CheckDecCTRContentKey Allocate WV_IOCTL_CHK_KEY_T failed\n");
		return FALSE;
	} 
		
	u4BufVAddr->SessionContext = (UINT32)sesscnt;
	u4BufVAddr->u4Curtime = curtime;
	u4BufVAddr->buffer_type = buffer_type;
	u4BufVAddr->tem_slot = 0 ;

	fgRet = TZ_CTL(TZCTL_WV_CHK_CTR_KEY, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(WV_IOCTL_CHK_KEY_T)));

	*tmp_slot = u4BufVAddr->tem_slot ;
	//printf("TZ_DRM_WV_CheckDecCTRContentKey tmp_slot=%d \n",*tmp_slot);

	BSP_FreeAlignedDmaMemory(u4BufPAddr);

	return fgRet;
}

BOOL TZ_DRM_WV_CheckEncDecCBCContentKey(SessionContext *sesscnt,uint32_t curtime,int enc_dec,unsigned short * tmp_slot)
{
	BOOL fgRet;
	UINT32 u4BufPAddr;
	WV_IOCTL_CHK_KEY_T *u4BufVAddr;

	u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(WV_IOCTL_CHK_KEY_T), TZ_CACHE_LINE_SIZE);
	u4BufVAddr = (WV_IOCTL_CHK_KEY_T *)(VIRTUAL(u4BufPAddr));
	if (!u4BufVAddr)
	{
		printf("TZ_DRM_WV_CheckEncDecCBCContentKey Allocate WV_IOCTL_CHK_KEY_T failed\n");
		return FALSE;
	} 
		
	u4BufVAddr->SessionContext = (UINT32)sesscnt;
	u4BufVAddr->u4Curtime = curtime;
	u4BufVAddr->enc_dec = enc_dec;
	u4BufVAddr->tem_slot = 0 ;

	fgRet = TZ_CTL(TZCTL_WV_CHK_CBC_KEY, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(WV_IOCTL_CHK_KEY_T)));

	*tmp_slot = u4BufVAddr->tem_slot ;
	//printf("TZ_DRM_WV_CheckEncDecCBCContentKey tmp_slot=%d \n",*tmp_slot);

	BSP_FreeAlignedDmaMemory(u4BufPAddr);

	return fgRet;

}

#if 0
BOOL TZ_DRM_WV_DecryptCTR(SessionContext* session,
                const uint8_t* iv,
                size_t block_offset,
                const uint8_t* cipher_data,
                size_t cipher_data_length,
                bool is_encrypted,
                void* clear_data,
                BufferType buffer_type,uint32_t curtime)
{
	BOOL fgRet;
	UINT32 u4BufPAddr;
	WV_IOCTL_DEC_CTR_T *u4BufVAddr;
	uint8_t* puil_cont_iv = NULL;
	uint8_t* puil_cont_cipher_data = NULL;
	void* puil_cont_clear_data = NULL;

	printf("TZ_DRM_WV_DecryptCTR enter \n");
	printf("TZ_DRM_WV_DecryptCTR cipher_data_length=%d, block_offset=%d, is_encrypted=%d, buffer_type=%d, time=%d\n",cipher_data_length,block_offset,is_encrypted,buffer_type,curtime);

	u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(WV_IOCTL_DEC_CTR_T), TZ_CACHE_LINE_SIZE);
	u4BufVAddr = (WV_IOCTL_DEC_CTR_T *)(VIRTUAL(u4BufPAddr));
	if (!u4BufVAddr)
	{
		printf("TZ_DRM_WV_DecryptCTR Allocate WV_IOCTL_CHK_KEY_T failed\n");
		return false;
	} 
		
	u4BufVAddr->SessionContext = (UINT32)session;

	puil_cont_iv = (uint8_t *)VIRTUAL(BSP_AllocAlignedDmaMemory(AES_FUN_BLK_LEN, TZ_CACHE_LINE_SIZE));
	if(NULL == puil_cont_iv)
	{
		printf("TZ_DRM_WV_DecryptCTR alloc puil_cont_iv fail\n");
		fgRet = false;	
		goto s;		
	}

	puil_cont_cipher_data = (uint8_t *)VIRTUAL(BSP_AllocAlignedDmaMemory(cipher_data_length, TZ_CACHE_LINE_SIZE));
	if(NULL == puil_cont_cipher_data)
	{
		printf("TZ_DRM_WV_DecryptCTR alloc puil_cont_cipher_data fail\n");
		fgRet = false;	
		goto s;		
	}

	puil_cont_clear_data = (void *)VIRTUAL(BSP_AllocAlignedDmaMemory(cipher_data_length, TZ_CACHE_LINE_SIZE));
	if(NULL == puil_cont_clear_data)
	{
		printf("TZ_DRM_WV_DecryptCTR alloc puil_cont_clear_data fail\n");
		fgRet = false;	
		goto s;		
	}

	//Copy input into continuous memory
	x_memcpy(puil_cont_iv, iv, AES_FUN_BLK_LEN);	
	x_memcpy(puil_cont_cipher_data, cipher_data, cipher_data_length);	
	x_memcpy(puil_cont_clear_data, clear_data, cipher_data_length);	

	u4BufVAddr->iv = (uint8_t *)PHYSICAL((UINT32)puil_cont_iv);
	u4BufVAddr->block_offset = block_offset;
	u4BufVAddr->cipher_data = (uint8_t *)PHYSICAL((UINT32)puil_cont_cipher_data);
	u4BufVAddr->cipher_data_length = cipher_data_length;
	if (0 != is_encrypted)
		u4BufVAddr->is_encrypted = 1;
	else
		u4BufVAddr->is_encrypted = 0;
	u4BufVAddr->clear_data = (void *)PHYSICAL((UINT32)puil_cont_clear_data);
	u4BufVAddr->buffer_type = buffer_type;
	u4BufVAddr->u4Curtime = curtime;

	TZ_NWD_BufferStart((UINT32)puil_cont_iv, TZ_CACHE_ALIGN(AES_FUN_BLK_LEN));
	TZ_NWD_BufferStart((UINT32)puil_cont_cipher_data, TZ_CACHE_ALIGN(cipher_data_length));
	TZ_NWD_BufferStart((UINT32)puil_cont_clear_data, TZ_CACHE_ALIGN(cipher_data_length));

	fgRet = TZ_CTL(TZCTL_WV_CHK_DEC_CTR, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(WV_IOCTL_DEC_CTR_T)));

	TZ_NWD_BufferEnd((UINT32)puil_cont_iv, TZ_CACHE_ALIGN(AES_FUN_BLK_LEN));
	TZ_NWD_BufferEnd((UINT32)puil_cont_cipher_data, TZ_CACHE_ALIGN(cipher_data_length));
	TZ_NWD_BufferEnd((UINT32)puil_cont_clear_data, TZ_CACHE_ALIGN(cipher_data_length));
	
	//Copy output from continuous memory
	x_memcpy(clear_data, puil_cont_clear_data, cipher_data_length);

s:
	BSP_FreeAlignedDmaMemory(u4BufPAddr);
	BSP_FreeAlignedDmaMemory(PHYSICAL((UINT32)puil_cont_iv));
	BSP_FreeAlignedDmaMemory(PHYSICAL((UINT32)puil_cont_cipher_data));
	BSP_FreeAlignedDmaMemory(PHYSICAL((UINT32)puil_cont_clear_data));

	return fgRet;

}


BOOL TZ_DRM_WV_Generic_Encrypt(SessionContext *sesscnt, const uint8_t* in_buffer,
                     size_t buffer_length,
                     const uint8_t* iv,
                     OEMCrypto_Algorithm algorithm,
                     uint8_t* out_buffer, uint32_t curtime)
{
	BOOL fgRet;
	UINT32 u4BufPAddr;
	WV_IOCTL_GEN_T *u4BufVAddr;
	uint8_t* puil_cont_iv = NULL;
	uint8_t* puil_cont_in_buffer = NULL;
	uint8_t* puil_cont_out_buffer = NULL;

	printf("TZ_DRM_WV_Generic_Encrypt enter \n");
	
	u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(WV_IOCTL_GEN_T), TZ_CACHE_LINE_SIZE);
	u4BufVAddr = (WV_IOCTL_GEN_T *)(VIRTUAL(u4BufPAddr));
	if (!u4BufVAddr)
	{
		printf("TZ_DRM_WV_Generic_Encrypt Allocate WV_IOCTL_GEN_T failed\n");
		return false;
	} 
			
	u4BufVAddr->SessionContext = (UINT32)sesscnt;

	puil_cont_iv = (uint8_t *)VIRTUAL(BSP_AllocAlignedDmaMemory(AES_FUN_BLK_LEN, TZ_CACHE_LINE_SIZE));
	if(NULL == puil_cont_iv)
	{
		printf("TZ_DRM_WV_Generic_Encrypt alloc puil_cont_iv fail\n");
		fgRet = false;	
		goto s;		
	}

	puil_cont_in_buffer = (uint8_t *)VIRTUAL(BSP_AllocAlignedDmaMemory(buffer_length, TZ_CACHE_LINE_SIZE));
	if(NULL == puil_cont_in_buffer)
	{
		printf("TZ_DRM_WV_Generic_Encrypt alloc puil_cont_in_buffer fail\n");
		fgRet = false;	
		goto s;		
	}

	puil_cont_out_buffer = (uint8_t *)VIRTUAL(BSP_AllocAlignedDmaMemory(buffer_length, TZ_CACHE_LINE_SIZE));
	if(NULL == puil_cont_out_buffer)
	{
		printf("TZ_DRM_WV_Generic_Encrypt alloc puil_cont_out_buffer fail\n");
		fgRet = false;	
		goto s;		
	}

	//Copy input into continuous memory
	x_memcpy(puil_cont_iv, iv, AES_FUN_BLK_LEN);	
	x_memcpy(puil_cont_in_buffer, in_buffer, buffer_length);	
	x_memcpy(puil_cont_out_buffer, out_buffer, buffer_length);	

	u4BufVAddr->in_buffer = (uint8_t *)PHYSICAL((UINT32)puil_cont_in_buffer);
	u4BufVAddr->buffer_length = buffer_length;
	u4BufVAddr->iv = (uint8_t *)PHYSICAL((UINT32)puil_cont_iv);
	u4BufVAddr->algorithm = algorithm;
	u4BufVAddr->out_buffer = (uint8_t *)PHYSICAL((UINT32)puil_cont_out_buffer);
	u4BufVAddr->u4Curtime = curtime;

	TZ_NWD_BufferStart((UINT32)puil_cont_iv, TZ_CACHE_ALIGN(AES_FUN_BLK_LEN));
	TZ_NWD_BufferStart((UINT32)puil_cont_in_buffer, TZ_CACHE_ALIGN(buffer_length));
	TZ_NWD_BufferStart((UINT32)puil_cont_out_buffer, TZ_CACHE_ALIGN(buffer_length));

	fgRet = TZ_CTL(TZCTL_WV_GENERIC_ENC, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(WV_IOCTL_GEN_T)));

	TZ_NWD_BufferEnd((UINT32)puil_cont_iv, TZ_CACHE_ALIGN(AES_FUN_BLK_LEN));
	TZ_NWD_BufferEnd((UINT32)puil_cont_in_buffer, TZ_CACHE_ALIGN(buffer_length));
	TZ_NWD_BufferEnd((UINT32)puil_cont_out_buffer, TZ_CACHE_ALIGN(buffer_length));

	x_memcpy(out_buffer, puil_cont_out_buffer, buffer_length);

s:
	BSP_FreeAlignedDmaMemory(u4BufPAddr);
	BSP_FreeAlignedDmaMemory(PHYSICAL((UINT32)puil_cont_iv));
	BSP_FreeAlignedDmaMemory(PHYSICAL((UINT32)puil_cont_in_buffer));
	BSP_FreeAlignedDmaMemory(PHYSICAL((UINT32)puil_cont_out_buffer));

	return fgRet;

}

BOOL TZ_DRM_WV_Generic_Decrypt(SessionContext *sesscnt, const uint8_t* in_buffer,
                     size_t buffer_length,
                     const uint8_t* iv,
                     OEMCrypto_Algorithm algorithm,
                     uint8_t* out_buffer, uint32_t curtime)
{
	BOOL fgRet;
	UINT32 u4BufPAddr;
	WV_IOCTL_GEN_T *u4BufVAddr;
	uint8_t* puil_cont_iv = NULL;
	uint8_t* puil_cont_in_buffer = NULL;
	uint8_t* puil_cont_out_buffer = NULL;

	printf("TZ_DRM_WV_Generic_Decrypt enter \n");
	
	u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(WV_IOCTL_GEN_T), TZ_CACHE_LINE_SIZE);
	u4BufVAddr = (WV_IOCTL_GEN_T *)(VIRTUAL(u4BufPAddr));
	if (!u4BufVAddr)
	{
		printf("TZ_DRM_WV_Generic_Decrypt Allocate WV_IOCTL_GEN_T failed\n");
		return false;
	} 
			
	u4BufVAddr->SessionContext = (UINT32)sesscnt;

	puil_cont_iv = (uint8_t *)VIRTUAL(BSP_AllocAlignedDmaMemory(AES_FUN_BLK_LEN, TZ_CACHE_LINE_SIZE));
	if(NULL == puil_cont_iv)
	{
		printf("TZ_DRM_WV_Generic_Decrypt alloc puil_cont_iv fail\n");
		fgRet = false;	
		goto s;		
	}

	puil_cont_in_buffer = (uint8_t *)VIRTUAL(BSP_AllocAlignedDmaMemory(buffer_length, TZ_CACHE_LINE_SIZE));
	if(NULL == puil_cont_in_buffer)
	{
		printf("TZ_DRM_WV_Generic_Decrypt alloc puil_cont_in_buffer fail\n");
		fgRet = false;	
		goto s;		
	}

	puil_cont_out_buffer = (uint8_t *)VIRTUAL(BSP_AllocAlignedDmaMemory(buffer_length, TZ_CACHE_LINE_SIZE));
	if(NULL == puil_cont_out_buffer)
	{
		printf("TZ_DRM_WV_Generic_Decrypt alloc puil_cont_out_buffer fail\n");
		fgRet = false;	
		goto s;		
	}

	//Copy input into continuous memory
	x_memcpy(puil_cont_iv, iv, AES_FUN_BLK_LEN);	
	x_memcpy(puil_cont_in_buffer, in_buffer, buffer_length);	
	x_memcpy(puil_cont_out_buffer, out_buffer, buffer_length);	

	u4BufVAddr->in_buffer = (uint8_t *)PHYSICAL((UINT32)puil_cont_in_buffer);
	u4BufVAddr->buffer_length = buffer_length;
	u4BufVAddr->iv = (uint8_t *)PHYSICAL((UINT32)puil_cont_iv);
	u4BufVAddr->algorithm = algorithm;
	u4BufVAddr->out_buffer = (uint8_t *)PHYSICAL((UINT32)puil_cont_out_buffer);
	u4BufVAddr->u4Curtime = curtime;

	TZ_NWD_BufferStart((UINT32)puil_cont_iv, TZ_CACHE_ALIGN(AES_FUN_BLK_LEN));
	TZ_NWD_BufferStart((UINT32)puil_cont_in_buffer, TZ_CACHE_ALIGN(buffer_length));
	TZ_NWD_BufferStart((UINT32)puil_cont_out_buffer, TZ_CACHE_ALIGN(buffer_length));

	fgRet = TZ_CTL(TZCTL_WV_GENERIC_DEC, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(WV_IOCTL_GEN_T)));

	TZ_NWD_BufferEnd((UINT32)puil_cont_iv, TZ_CACHE_ALIGN(AES_FUN_BLK_LEN));
	TZ_NWD_BufferEnd((UINT32)puil_cont_in_buffer, TZ_CACHE_ALIGN(buffer_length));
	TZ_NWD_BufferEnd((UINT32)puil_cont_out_buffer, TZ_CACHE_ALIGN(buffer_length));

	x_memcpy(out_buffer, puil_cont_out_buffer, buffer_length);

s:
	BSP_FreeAlignedDmaMemory(u4BufPAddr);
	BSP_FreeAlignedDmaMemory(PHYSICAL((UINT32)puil_cont_iv));
	BSP_FreeAlignedDmaMemory(PHYSICAL((UINT32)puil_cont_in_buffer));
	BSP_FreeAlignedDmaMemory(PHYSICAL((UINT32)puil_cont_out_buffer));

	return fgRet;

}
#endif

BOOL TZ_DRM_WV_Generic_Sign(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_GENERIC_SIGN, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_Generic_Verify(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_GENERIC_VERIFY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_RewrapDeviceRSAKeySess(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_REWRAP_RSAKEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_LoadDeviceRSAKeySessCtx(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_LOAD_RSAKET, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_GenerateRSASignature(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_GENERATE_RSASIGN, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_RSADeriveKeys(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_RSA_DERIVEKEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_WV_SaveKeybox(UINT8 *pu1Keybox, UINT32 u4Size)
{
    BOOL fgRet;
    UINT32 u4BufPAddr, u4BufVAddr;

	//printf("TZ_DRM_WV_SaveKeybox enter \n");

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4Size, TZ_CACHE_LINE_SIZE);
    u4BufVAddr = (UINT32)(VIRTUAL(u4BufPAddr));
	if (!u4BufVAddr)
	{
		printf("TZ_DRM_WV_SaveKeybox Allocate failed\n");
		return FALSE;
	} 
	
    x_memcpy((void *)u4BufVAddr, pu1Keybox, u4Size);

    fgRet = TZ_CTL(TZCTL_WV_SAVE_KEYBOX, u4BufPAddr, TZ_CACHE_ALIGN(u4Size));

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_DRM_WV_GetRandomNum(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_WV_GET_RANDOM_NUM, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

///////////////////////////////////for hulu+dash+wv drm only end//////////////

BOOL TZ_HCI_Init(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_INIT, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_Fin(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_FIN, PHYSICAL((UINT32)prKernParam), 0);
}

BOOL TZ_HCI_Lock(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_LOCK, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_Unlock(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_UNLOCK, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_KeyRefCreate(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_KEYREF_CREATE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_KeyRefCheck(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_KEYREF_CHECK, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_KeyRefDestroy(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_KEYREF_DESTROY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_DevIdsGetNum(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_DEVICE_GET_NUM, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_DevIdsGetSize(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_DEVICE_GET_SIZE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_DevIdsGetName(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_DEVICE_GET_NAME, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_ImportKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_IMPORT_KEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_ExportKeySize(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_EXPORT_KEY_SIZE, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_ExportKey(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_EXPORT_KEY, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_StarfishIdGetSize(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_GET_START_FISH_ID_SIZE, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_StarfishIdGet(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_GET_START_FISH_ID, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_KeyDataGetIfStarfishSize(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_GET_START_FISH_KEYDATA_SIZE, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_KeyDataGetIfStarfish(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_GET_START_FISH_KEYDATA, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_ImportStarfishKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_IMPORT_START_FISH_KEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_DataSignSize(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_DATA_SIGN_SIZE, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_DataSign(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_DATA_SIGN, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_DataDecryptSize(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_DATA_DECRYPT_SIZE, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_DataDecrypt(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_DATA_DECRYPT, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_StreamDecryptSize(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_STREAM_DECRYPT_SIZE, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_StreamDecrypt(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_STREAM_DECRYP, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_KeyDataEncryptSize(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_KEYDATA_ENCRYPT_SIZE, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_KeyDataEncrypt(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_KEYDATA_ENCRYPT, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_HmacCheck(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_HMAC_CHECK, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_HashCheck(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_HASH_CHECK, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_MoveKeySize(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_MOVE_KEY_SIZE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_MoveKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_MOVE_KEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_FreeMem(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_FREEMEM, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_PersonalityKeyLoad(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HCI_PERSONALITYKEY_LOAD, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HCI_PersonalityKeyCheck(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_PERSONALITYKEY_CHECK, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_ImportSasSize(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_IMPORT_SAS_SIZE, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_ImportSas(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_IMPORT_SAS, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_HCI_ImportSasIds(void *prKernParam, UINT32 u4Size)
{
    return 1;//TZ_CTL(TZCTL_HCI_IMPORT_SAS_IDS, (VOID*)PHYSICAL((UINT32)prKernParam), (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL, (VOID*)NULL);
}

BOOL TZ_SKB_GetInstance(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_GET_INSTANCE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_Release(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_RELEASE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_GetInfo(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_GET_INFO, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_GetProertiesSize(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_GET_PROPERTIES_SIZE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_GetProertiesData(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_GET_PROPERTIES_DATA, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_DataFromWrapped(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_CREATE_DATA_FROM_WRAPPED, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_DataFromExported(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_CREATE_DATA_FROM_EXPORTED, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_CreateTransform(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_CREATE_TRANSFORM, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_CreateCipher(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_CREATE_CIPHER, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_SecureData_Release(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_SECUREDATA_RELEASE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_SecureDataGetInfo(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_SECUREDATA_GETINFO, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_SecureData_Export(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_SECUREDATA_EXPORT, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_SecureData_Derive(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_SECUREDATA_DERIVE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_Transform_Release(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_TRANSFORM_RELEASE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_Transform_AddBytes(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_TRANSFORM_ADDBYTES, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_Transform_AddSecureData(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_TRANSFORM_ADDSECUREDATA, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_Transform_GetOutput(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_TRANSFORM_GETOUTPUT, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_Cipher_ProcessBuffer(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_CIPHER_PROCESSBUFFER, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SKB_Cipher_Release(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_SKB_CIPHER_RELEASE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_Divx_GetHwSecretKey0(UINT8 *pu1Buf)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
    TZ_GET_HWSECRETKEY_T *pGetHwSecretKey;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(TZ_GET_HWSECRETKEY_T), TZ_CACHE_LINE_SIZE);
    pGetHwSecretKey = (TZ_GET_HWSECRETKEY_T *)(VIRTUAL(u4BufPAddr));

    fgRet = TZ_CTL(TZCTL_DRM_DIVX_GETHWSECRETKEY0, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(TZ_GET_HWSECRETKEY_T)));

    x_memcpy(pu1Buf, pGetHwSecretKey->au1Buf, 32);
    BSP_FreeAlignedDmaMemory(u4BufPAddr);
    return fgRet;
}

BOOL TZ_DRM_Divx_GetHwSecretKey1(UINT8 *pu1Buf)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;
    TZ_GET_HWSECRETKEY_T *pGetHwSecretKey;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(TZ_GET_HWSECRETKEY_T), TZ_CACHE_LINE_SIZE);
    pGetHwSecretKey = (TZ_GET_HWSECRETKEY_T *)(VIRTUAL(u4BufPAddr));

    fgRet = TZ_CTL(TZCTL_DRM_DIVX_GETHWSECRETKEY1, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(TZ_GET_HWSECRETKEY_T)));

    x_memcpy(pu1Buf, pGetHwSecretKey->au1Buf, 32);
    BSP_FreeAlignedDmaMemory(u4BufPAddr);
    return fgRet;
}

BOOL TZ_BIM_GetRomBytes(UINT32 u4Offset, UINT8 *pu1Buf, UINT32 u4Size)
{
    return FALSE;
}

BOOL TZ_BIM_GetSecureID(UINT8 *pu1Buf, UINT32 u4Size)
{
    return FALSE;
}

BOOL TZ_DRM_TZ_LoadSecretDataType(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_DRM_TZ_LOADSECRETDATATYPE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_TZ_SetSecretDataType(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_DRM_TZ_SETSECRETDATATYPE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_TZ_Open(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_DRM_TZ_OPEN, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_TZ_Close(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_DRM_TZ_CLOSE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_DRM_TZ_DecodePacketsVOD(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_DRM_TZ_DECODEPACKETSVOD, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

#ifdef CC_ENABLE_HDCP2
// hdcp 2.0
BOOL TZ_HDCP2_SetEncLc128(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_SET_ENC_LC128, PHYSICAL((INT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_SetEncKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_SET_ENC_KEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_GetCertInfo(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_GETCERTINFO, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_DecryptRSAESOAEP(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_DECRYPT_RSAESOAEP, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_KdKeyDerivation(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_KDKEYDERIVATION, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_ComputeHprime(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_COMPUTE_HPRIME, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_ComputeLprime(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_COMPUTE_LPRIME, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_ComputeKh(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_COMPUTE_KH, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_EncryptKmUsingKh(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_ENCRYPT_KM, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_DecryptKmUsingKh(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_DECRYPT_KM, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_DecryptEKs(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_DECRYPT_EKS, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_KsXorLC128(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_KSLC128XOR, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_SetRiv_Pid(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_SETRIV_PID, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_PresetKsLC128(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_PRESETKSLC128, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_Generate_Km(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_GENERATE_KM, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_Generate_Ks(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_GENERATE_KS, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_SetEncKm(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_SET_ENC_KM, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_GetEncKm(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_GET_ENC_KM, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}




/* for HDCP 2.2 */


BOOL TZ_HDCP2_2_KdKeyDerivation(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_2_KDKEYDERIVATION, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_2_ComputeHprime(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_2_COMPUTE_HPRIME, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_2_ComputeLprime(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_2_COMPUTE_LPRIME, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}


/* for HDCP2.X ALIGN*/
BOOL TZ_HDCP2_SetEncKeySet(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_SET_ENC_KEYSET, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}



/* for HDCP2.X  TX */
BOOL TZ_HDCP2_RetrvEkh_km(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_RETRVE_KH_KM, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_SavePairingInfo(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_SAVE_PAIRINGINFO, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}


BOOL TZ_HDCP2_CheckRxID(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_CHECK_RXID, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}


BOOL TZ_HDCP2_ComputeMprime(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_COMPUTE_MPRIME, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_ComputeVprime(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_COMPUTE_VPRIME, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_2_ComputeVprime(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_2_COMPUTE_VHPRIME, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_VerifySignature(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_VERIFY_SIGNATURE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}


BOOL TZ_HDCP2_Generate_Km_for_tx(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_GENERATE_KM_FOR_TX, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}


BOOL TZ_HDCP2_UseTestKey_InKernel(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_SET_KEY_TYPE, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}


BOOL TZ_HDCP2_enable_tz_key(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_ENABLE_TZ_KEY, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}




BOOL TZ_HDCP2_ResetPairingInfo(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_ResetPairingInfo, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_HDCP2_HMAC_SHA256(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_HDCP2_HMAC_SHA256, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_SW_Decrypt(void *prKernParam, UINT32 u4Size)
{
	return TZ_CTL(TZCTL_HDCP2_SW_DecryptData, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_HDCP2_GetPDKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_GetPDKey, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}


BOOL TZ_HDCP2_SetPDKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_HDCP2_SetPDKey, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

#endif





/* sony iptves */


/* sac */
BOOL TZ_IPTVES_SAC_CONSTRUCT(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SAC_CONSTRUCT, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}




BOOL TZ_IPTVES_SAC_DESTRUCT(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SAC_DESTRUCT, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}



BOOL TZ_IPTVES_SAC_MkMsg(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SAC_MAKEMSG, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}



BOOL TZ_IPTVES_SAC_ProcessMsg(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SAC_PROCESSMSG, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}







/* sdi */
BOOL TZ_IPTVES_SDI_INIT(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SDI_INIT, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
          			     
BOOL TZ_IPTVES_SDI_FIN(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SDI_FIN, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
          			     
BOOL TZ_IPTVES_SDI_CleanupNVS(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SDI_CleanupNVS, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
       		   
BOOL TZ_IPTVES_SDI_GetDeviceID(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SDI_GetDeviceID, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
    			   
BOOL TZ_IPTVES_SDI_VerifyServiceMetaData(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SDI_VerifyServiceMetaData, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
   
BOOL TZ_IPTVES_SDI_JudgeCRLUpdate(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SDI_JudgeCRLUpdate, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
      		 
BOOL TZ_IPTVES_SDI_GetCRLDstPoint(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SDI_GetCRLDstPoint, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
          
BOOL TZ_IPTVES_SDI_SetCRL(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SDI_SetCRL, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
                  
BOOL TZ_IPTVES_SDI_CheckTrustedTime(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_SDI_CheckTrustedTime, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
        
                                                 
                                                 
/* ecm */                                        
BOOL TZ_IPTVES_ECM_ExtractScrambleKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_ECM_ExtractScrambleKey, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
      
BOOL TZ_IPTVES_ECM_GetScrambleKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_ECM_GetScrambleKey, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
    		   
                                                 
                                                 
/* drm */                                        
BOOL TZ_IPTVES_DRM_ExtractContentKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_DRM_ExtractContentKey, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
       
BOOL TZ_IPTVES_DRM_DelContentKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_DRM_DelContentKey, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
          	
BOOL TZ_IPTVES_DRM_DelUncommittedKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_DRM_DelUncommittedKey, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
       
BOOL TZ_IPTVES_DRM_GetKeyInfo(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_DRM_GetKeyInfo, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
   			     
BOOL TZ_IPTVES_DRM_GetKeyNum(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_DRM_GetKeyNum, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
     			     
BOOL TZ_IPTVES_DRM_GetKeyRefList(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_DRM_GetKeyRefList, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
      	   
BOOL TZ_IPTVES_DRM_GetContentKey(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_DRM_GetContentKey, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
           



/* nvs */
BOOL TZ_IPTVES_NVS_MEM_Init(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_NVS_MEM_Init, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
           

BOOL TZ_IPTVES_NVS_MEM_Fin(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_NVS_MEM_Fin, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
           


BOOL TZ_IPTVES_NVS_MEM_GetRWLen(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_NVS_MEM_GetRWLen, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}
           




/* time */



BOOL TZ_IPTVES_TIME_SetTrustedTime(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_TIME_SetTrustedTime, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}


BOOL TZ_IPTVES_TIME_GetTrustedTime(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_TIME_GetTrustedTime, PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_IPTVES_TIME_Cleanup(void *prKernParam, UINT32 u4Size)
{
    return TZ_CTL(TZCTL_IPTVES_TIME_Cleanup,       PHYSICAL((UINT32)prKernParam), TZ_CACHE_ALIGN(u4Size));
}






// TEE
BOOL TZ_TEE_Common_Operate(UINT32 ui4Command, void *ptCommonArg, UINT32 u4Size)
{
    return TZ_CTL(ui4Command, PHYSICAL((UINT32)ptCommonArg), TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SWDMX_MpgParsingPts(void* ptDmxPesMsg)
{
    BOOL fgRet;
    UINT32 u4BufPAddr,u4BufLen;
    TZ_DMX_PES_MSG_T *prPes;

    u4BufLen = TZ_CACHE_ALIGN(sizeof(TZ_DMX_PES_MSG_T));
    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    prPes =  (TZ_DMX_PES_MSG_T*)(VIRTUAL(u4BufPAddr));
    prPes->u4BufStart = ((TZ_DMX_PES_MSG_T*)ptDmxPesMsg)->u4BufStart;
    prPes->u4BufEnd = ((TZ_DMX_PES_MSG_T*)ptDmxPesMsg)->u4BufEnd;
    prPes->u4FrameAddr = ((TZ_DMX_PES_MSG_T*)ptDmxPesMsg)->u4FrameAddr;
    prPes->u4PrevFrameAddr = ((TZ_DMX_PES_MSG_T*)ptDmxPesMsg)->u4PrevFrameAddr;
    prPes->u4PrevFrameType = ((TZ_DMX_PES_MSG_T*)ptDmxPesMsg)->u4PrevFrameType;
    prPes->u4Pts = ((TZ_DMX_PES_MSG_T*)ptDmxPesMsg)->u4Pts;
    prPes->u4Dts = ((TZ_DMX_PES_MSG_T*)ptDmxPesMsg)->u4Dts;

/*
    Printf("u4BufStart=0x%x, u4BufEnd=0x%x u4FrameAddr=0x%x u4PrevFrameAddr=0x%x u4PrevFrameType=0x%x u4Pts=0x%x u4Dts=0x%x\n",
            prPes->u4BufStart,
            prPes->u4BufEnd,
            prPes->u4FrameAddr,
            prPes->u4PrevFrameAddr,
            prPes->u4PrevFrameType,
            prPes->u4Pts,
            prPes->u4Dts);
*/

    TZ_NWD_BufferStart(VIRTUAL(u4BufPAddr), u4BufLen);
    fgRet = TZ_CTL(TZCTL_SWDMX_MPGPARSPTS, u4BufPAddr, u4BufLen);
    ((TZ_DMX_PES_MSG_T*)ptDmxPesMsg)->u4Pts = prPes->u4Pts;
    ((TZ_DMX_PES_MSG_T*)ptDmxPesMsg)->u4Dts = prPes->u4Dts;

    // Printf("u4Pts=0x%x u4Dts=0x%x\n",prPes->u4Pts,prPes->u4Dts);
    TZ_NWD_BufferEnd(VIRTUAL(u4BufPAddr), u4BufLen);

    BSP_FreeAlignedDmaMemory(u4BufPAddr);
    return fgRet;
}

BOOL TZ_SWDMX_DecryptContent(void* pvDmxMmData)
{
    BOOL fgRet;
    UINT32 u4BufPAddr,u4BufLen;
    UINT32 u4PhyBufStart;
    UINT32 u4PhyBufEnd;
    UINT32 u4PhyStartAddr;  
    
    TZ_DESCRYPT_DATA_T *ptDescpyptData = NULL;
    
    u4BufLen = TZ_CACHE_ALIGN(sizeof(TZ_DESCRYPT_DATA_T));
    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    
    ptDescpyptData = (TZ_DESCRYPT_DATA_T*)(VIRTUAL(u4BufPAddr));
    
    ptDescpyptData->fgFrameHead  = ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->fgFrameHead;
    ptDescpyptData->u4BufStart  = ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4BufStart;
    ptDescpyptData->u4BufEnd    = ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4BufEnd;
    ptDescpyptData->u4StartAddr = ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4StartAddr;
    ptDescpyptData->u4FrameSize = ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4FrameSize;
    ptDescpyptData->eEncryptMode = ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->eEncryptMode;
    /* 
    Printf("NW before:u4BufStart 0x%x u4BufEnd 0x%x u4StartAddr 0x%x u4FrameSize 0x%x\n",
        ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4BufStart,
        ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4BufEnd,
        ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4StartAddr,
        ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4FrameSize);
        */

    
    u4PhyBufStart = PHYSICAL(ptDescpyptData->u4BufStart);
    u4PhyBufEnd = PHYSICAL(ptDescpyptData->u4BufEnd);
    u4PhyStartAddr = PHYSICAL(ptDescpyptData->u4StartAddr);
    
    ptDescpyptData->u4BufStart = u4PhyBufStart;
    ptDescpyptData->u4BufEnd   = u4PhyBufEnd;
    ptDescpyptData->u4StartAddr = u4PhyStartAddr;
    

    GCPU_HW_LOCK();
    
    TZ_NWD_BufferStart(VIRTUAL(u4BufPAddr), u4BufLen);
    fgRet = TZ_CTL(TZCTL_SWDMX_DECRYPT_DATA, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(TZ_DESCRYPT_DATA_T)));
    ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4BufStart = ptDescpyptData->u4BufStart;
    ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4BufEnd = ptDescpyptData->u4BufEnd;
    ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4StartAddr = ptDescpyptData->u4StartAddr;
    ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4FrameSize = ptDescpyptData->u4FrameSize;

    /* 
    Printf("NW after:u4BufStart 0x%x u4BufEnd 0x%x u4StartAddr 0x%x u4FrameSize 0x%x\n",
        ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4BufStart,
        ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4BufEnd,
        ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4StartAddr,
        ((TZ_DESCRYPT_DATA_T*)pvDmxMmData)->u4FrameSize);
        */
        
    TZ_NWD_BufferEnd(VIRTUAL(u4BufPAddr), u4BufLen); 
    GCPU_HW_UNLOCK();
    
    BSP_FreeAlignedDmaMemory(u4BufPAddr);
    
    return fgRet;
}
BOOL TZ_SWDMX_FreeDecryptBuf(void)
{
    BOOL fgRet;
    fgRet = TZ_CTL(TZCTL_SWDMX_FREE_DECRYPT_BUF, 0, 0);
    return fgRet;
}

#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_TVP_SUPPORT)
BOOL TZ_VDEC_FB_Operation(VOID* prTzFbOp, UCHAR ucOperation)
{
    BOOL fgRet;
    UINT32 u4BufPAddr,u4BufLen;
    TZ_FB_OPERATION_T *prFbOp;

    u4BufLen = TZ_CACHE_ALIGN(sizeof(TZ_FB_OPERATION_T));
    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    prFbOp = (TZ_FB_OPERATION_T*)(VIRTUAL(u4BufPAddr));
    prFbOp->eOperation = (ENUM_TZ_VDEC_FB_OPERATION_T)ucOperation;
    prFbOp->u4SrcAddr = ((TZ_FB_OPERATION_T*)prTzFbOp)->u4SrcAddr;
    prFbOp->u4DesAddr = ((TZ_FB_OPERATION_T*)prTzFbOp)->u4DesAddr;
    prFbOp->u4Size = ((TZ_FB_OPERATION_T*)prTzFbOp)->u4Size;
    prFbOp->u4Value = ((TZ_FB_OPERATION_T*)prTzFbOp)->u4Value;

    TZ_NWD_BufferStart(VIRTUAL(u4BufPAddr), u4BufLen);
    fgRet = TZ_CTL(TZCTL_VDEC_FB_OPERATION, u4BufPAddr, u4BufLen);
    TZ_NWD_BufferEnd(VIRTUAL(u4BufPAddr), u4BufLen);

    ((TZ_FB_OPERATION_T*)prTzFbOp)->u4Value = prFbOp->u4Value;
    
    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_VDEC_Check_RealD(VOID *prDisp)
{
	BOOL fgRet;
    UINT32 u4BufPAddr,u4BufLen;
	TZ_DISP_PIC_INFO_T *prDispInfo;

	u4BufLen = TZ_CACHE_ALIGN(sizeof(TZ_DISP_PIC_INFO_T));
    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
	prDispInfo = (TZ_DISP_PIC_INFO_T*)(VIRTUAL(u4BufPAddr));
	prDispInfo->pvYAddr = ((TZ_DISP_PIC_INFO_T*)prDisp)->pvYAddr;
	prDispInfo->pvCAddr = ((TZ_DISP_PIC_INFO_T*)prDisp)->pvCAddr;
	prDispInfo->u4H = ((TZ_DISP_PIC_INFO_T*)prDisp)->u4H;
	prDispInfo->u4H_Cmp = ((TZ_DISP_PIC_INFO_T*)prDisp)->u4H_Cmp;
	prDispInfo->u4W = ((TZ_DISP_PIC_INFO_T*)prDisp)->u4W;
	prDispInfo->u4W_Cmp = ((TZ_DISP_PIC_INFO_T*)prDisp)->u4W_Cmp;
	prDispInfo->u4UnqualifiedCnt = ((TZ_DISP_PIC_INFO_T*)prDisp)->u4UnqualifiedCnt;
	prDispInfo->u1QuincunxCnt = ((TZ_DISP_PIC_INFO_T*)prDisp)->u1QuincunxCnt;

	TZ_NWD_BufferStart(VIRTUAL(u4BufPAddr), u4BufLen);
    fgRet = TZ_CTL(TZCTL_VDEC_CHECK_REALD, u4BufPAddr, u4BufLen);
	((TZ_DISP_PIC_INFO_T*)prDisp)->u4UnqualifiedCnt = prDispInfo->u4UnqualifiedCnt;
	((TZ_DISP_PIC_INFO_T*)prDisp)->u1QuincunxCnt = prDispInfo->u1QuincunxCnt;

	TZ_NWD_BufferEnd(VIRTUAL(u4BufPAddr), u4BufLen);
    BSP_FreeAlignedDmaMemory(u4BufPAddr);

	return fgRet;
}
#endif

#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_SVP_SUPPORT)
BOOL TZ_AU_DELIM_APPEND(UINT32 u4Base, UINT32 u4Offset, UINT32 u4DelimSize, UINT32* u4DelimBuf)
{
    BOOL fgRet;
    UINT32 u4BufPAddr;    
    UINT32 u4AUBuf;
    TZ_AU_DELIM_APPEND_T *pDelimAppend;

    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(TZ_AU_DELIM_APPEND_T), TZ_CACHE_LINE_SIZE);
    pDelimAppend = (TZ_AU_DELIM_APPEND_T *)(VIRTUAL(u4BufPAddr));
    pDelimAppend->u4Base = u4Base;
    pDelimAppend->u4Offset = u4Offset;
    pDelimAppend->u4Size = u4DelimSize;    
    u4AUBuf = (UINT32)BSP_AllocAlignedDmaMemory(sizeof(u4DelimSize), TZ_CACHE_LINE_SIZE);    
    x_memcpy((void *)VIRTUAL((UINT32)u4AUBuf), u4DelimBuf, u4DelimSize);
    pDelimAppend->pu1Buf = (UINT8 *)u4AUBuf;
    
    TZ_NWD_BufferStart(VIRTUAL((UINT32)u4AUBuf), TZ_CACHE_ALIGN(u4DelimSize));
    fgRet = TZ_CTL(TZCTL_AU_DELIM_APPEND, u4BufPAddr, TZ_CACHE_ALIGN(sizeof(TZ_AU_DELIM_APPEND_T)));
	TZ_NWD_BufferEnd(VIRTUAL((UINT32)u4AUBuf), TZ_CACHE_ALIGN(u4DelimSize));    

    BSP_FreeAlignedDmaMemory(u4BufPAddr);
    BSP_FreeAlignedDmaMemory(u4AUBuf);

    return fgRet;
}
#endif

BOOL TZ_DMX_Init()
{
    return TZ_CTL(TZCTL_DMX_INIT, 0, 0);
}

BOOL TZ_DMX_SetMMKey(UINT32 u4Addr, UINT32 u4Val)
{
    BOOL fgRet;
    UINT32 u4BufPAddr, u4BufLen;
    TZ_IO_T *pIo;

    u4BufLen = TZ_CACHE_ALIGN(sizeof(TZ_IO_T));
    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    pIo = (TZ_IO_T *)(VIRTUAL(u4BufPAddr));
    pIo->u4Base = u4Addr;
    pIo->u4Val = u4Val;

    fgRet = TZ_CTL(TZCTL_DMX_SET_MMKEY, u4BufPAddr, u4BufLen);

    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_NDDRM_SetAESKey (UINT8 * u4KeyBuf, UINT32 u4KeyLen, UINT8 * u4IvBuf, UINT32 u4IvLen)
{
    BOOL fgRet;
    UINT8 * u4KeyBufPAddr, *u4KeyBufVAddr;
    UINT8 *u4IvBufPAddr, *u4IvBufVAddr;
    UINT8 * u4BufPAddr, u4BufLen;
    TZ_NDDRM_T * ptNddrm;
    u4BufLen = TZ_CACHE_ALIGN(sizeof(TZ_NDDRM_T));
    u4BufPAddr = (UINT8 * )BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    ptNddrm = (TZ_NDDRM_T *)(VIRTUAL((UINT32)u4BufPAddr));

    /* config content key */
    u4KeyBufPAddr = (UINT8 * )BSP_AllocAlignedDmaMemory(u4KeyLen, TZ_CACHE_LINE_SIZE);
    u4KeyBufVAddr = (UINT8 * )(VIRTUAL((UINT32)u4KeyBufPAddr));
    x_memcpy((void *)u4KeyBufVAddr, u4KeyBuf, u4KeyLen);
    ptNddrm->pu1KeyBuf = u4KeyBufPAddr;
    ptNddrm->u4KeyLen = u4KeyLen;

    /* config iv  */
    u4IvBufPAddr = (UINT8 * )BSP_AllocAlignedDmaMemory(u4IvLen, TZ_CACHE_LINE_SIZE);
    u4IvBufVAddr = (UINT8 * )(VIRTUAL((UINT32)u4IvBufPAddr));
    x_memcpy((void *)u4IvBufVAddr, u4IvBuf, u4IvLen);
    ptNddrm->pu1IvBuf = u4IvBufPAddr;
    ptNddrm->u4IvLen = u4IvLen;

	if (ptNddrm->u4KeyLen != 16) {
        Printf("[DRM_SetAESKey]u4KeyLen %d != %d.\n",ptNddrm->u4KeyLen, 16);
    }

    if (ptNddrm->pu1KeyBuf == NULL)
    {
        Printf("[DRM_SetAESKey]pu1KeyBuf is NULL. \n");
    }

    if (ptNddrm->u4IvLen != 16/2) {
        Printf("[DRM_SetAESKey]u4IvLen %d < %d.\n", ptNddrm->u4IvLen ,16/2);
    }

    if (ptNddrm->pu1IvBuf  == NULL)
    {
        Printf("[DRM_SetAESKey]pu1KeyBuf is NULL. \n");
    }

    TZ_NWD_BufferStart((UINT32)u4KeyBufVAddr, TZ_CACHE_ALIGN(u4KeyLen));
    TZ_NWD_BufferStart((UINT32)u4IvBufVAddr, TZ_CACHE_ALIGN(u4IvLen));
    fgRet = TZ_CTL(TZCTL_TEE_PLAYREADY_CONTENTKEY_AESCTR_DMX_SET, (UINT32)u4BufPAddr, u4BufLen );
    TZ_NWD_BufferEnd((UINT32)u4KeyBufVAddr, TZ_CACHE_ALIGN(u4KeyLen));
    TZ_NWD_BufferEnd((UINT32)u4IvBufVAddr, TZ_CACHE_ALIGN(u4IvLen));

    BSP_FreeAlignedDmaMemory((UINT32)u4KeyBufPAddr);
    BSP_FreeAlignedDmaMemory((UINT32)u4IvBufPAddr);
    BSP_FreeAlignedDmaMemory((UINT32)u4BufPAddr);
    return fgRet;

}


BOOL TZ_DMX_GetPicHeader(UINT32 u4Addr, UINT32 u4BufStart, UINT32 u4BufEnd, UINT8 *pu1Buf, UINT32 u4Size)
{
    BOOL fgRet;
    UINT32 u4BufPAddr, u4BufLen;
    UINT32 u4PicHeaderPAddr, u4PicHeaderBufLen;
    TZ_DMX_PICHEADER_T *pPicHeader;

    u4BufLen = TZ_CACHE_ALIGN(sizeof(TZ_DMX_PICHEADER_T));
    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    pPicHeader = (TZ_DMX_PICHEADER_T *)(VIRTUAL(u4BufPAddr));
    u4PicHeaderBufLen = TZ_CACHE_ALIGN(u4Size);
    u4PicHeaderPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4PicHeaderBufLen, TZ_CACHE_LINE_SIZE);
    pPicHeader->u4Addr = u4Addr;
    pPicHeader->u4BufStart = u4BufStart;
    pPicHeader->u4BufEnd = u4BufEnd;
    pPicHeader->pu1Buf = (UINT8 *)u4PicHeaderPAddr;
    pPicHeader->u4Size = u4Size;

  	TZ_NWD_BufferStart(VIRTUAL(u4PicHeaderPAddr), u4PicHeaderBufLen);
    fgRet = TZ_CTL(TZCTL_DMX_GETPICHDR, u4BufPAddr, u4BufLen);
    TZ_NWD_BufferEnd(VIRTUAL(u4PicHeaderPAddr), u4PicHeaderBufLen);

    x_memcpy(pu1Buf, (void *)VIRTUAL(u4PicHeaderPAddr), u4Size);

    BSP_FreeAlignedDmaMemory(u4PicHeaderPAddr);
    BSP_FreeAlignedDmaMemory(u4BufPAddr);
    return fgRet;
}

BOOL TZ_DMX_GetSecureVfifo(UINT32 *pu4Addr, UINT32 *pu4Size)
{
    BOOL fgRet;
    UINT32 u4BufPAddr, u4BufLen;
    TZ_DMX_SECURE_VFIFO_T *pVfifo;

    u4BufLen = TZ_CACHE_ALIGN(sizeof(TZ_DMX_SECURE_VFIFO_T));
    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    pVfifo = (TZ_DMX_SECURE_VFIFO_T *)(VIRTUAL(u4BufPAddr));

    fgRet = TZ_CTL(TZCTL_DMX_GESECUREVFIFO, u4BufPAddr, u4BufLen);

    *pu4Addr = pVfifo->u4Addr;
    *pu4Size = pVfifo->u4Size;
    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_DMX_GetSecureVfifo2(UINT32 *pu4Addr, UINT32 *pu4Size)
{
    BOOL fgRet;
    UINT32 u4BufPAddr, u4BufLen;
    TZ_DMX_SECURE_VFIFO_T *pVfifo;

    u4BufLen = TZ_CACHE_ALIGN(sizeof(TZ_DMX_SECURE_VFIFO_T));
    u4BufPAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    pVfifo = (TZ_DMX_SECURE_VFIFO_T *)(VIRTUAL(u4BufPAddr));

    fgRet = TZ_CTL(TZCTL_DMX_GESECUREVFIFO2, u4BufPAddr, u4BufLen);

    *pu4Addr = pVfifo->u4Addr;
    *pu4Size = pVfifo->u4Size;
    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_DMX_SetCryptoKey(void *prKeyInfo, UINT32 u4Size)
{
    BOOL fgRet;
    UINT32 u4BufPAddr, u4BufLen;
    UINT8 *pu1Buf;

    u4BufLen = TZ_CACHE_ALIGN(u4Size);
    u4BufPAddr = BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    pu1Buf = (UINT8 *)VIRTUAL(u4BufPAddr);

    x_memcpy((void *)pu1Buf, prKeyInfo, u4Size);

    fgRet = TZ_CTL(TZCTL_DMX_SETCRYPTOKEY, u4BufPAddr, u4BufLen);
    
    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_FVR_SetCryptoKey(void *prKeyInfo, UINT32 u4Size)
{
    BOOL fgRet;
    UINT32 u4BufPAddr, u4BufLen;
    UINT8 *pu1Buf;

    u4BufLen = TZ_CACHE_ALIGN(u4Size);
    u4BufPAddr = BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    pu1Buf = (UINT8 *)VIRTUAL(u4BufPAddr);

    x_memcpy((void *)pu1Buf, prKeyInfo, u4Size);

    fgRet = TZ_CTL(TZCTL_FVR_SETCRYPTOKEY, u4BufPAddr, u4BufLen);
    
    BSP_FreeAlignedDmaMemory(u4BufPAddr);

    return fgRet;
}

BOOL TZ_DMX_ReloadSecureKey(void *prKeyInfo, UINT32 u4Size)
{
    BOOL fgRet;
    UINT32 u4BufPAddr, u4BufLen;
    UINT8 *pu1Buf;

    u4BufLen = TZ_CACHE_ALIGN(u4Size);
    u4BufPAddr = BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    pu1Buf = (UINT8 *)VIRTUAL(u4BufPAddr);

    x_memcpy((void *)pu1Buf, prKeyInfo, u4Size);

    fgRet = TZ_CTL(TZCTL_DMX_RELOADSECUREKEY, u4BufPAddr, u4BufLen);
    
    BSP_FreeAlignedDmaMemory(u4BufPAddr);
    return fgRet;
}

BOOL TZ_DMX_FreeSecureKeyIv(UINT8 u1Pidx)
{
    return TZ_CTL_Lock(TZ_SEMA_IDX_TVP, TZCTL_DMX_FREESECUREKEY, (UINT32)u1Pidx, 0);
}

BOOL TZ_DMX_SetMMSecurity(void *prKeyInfo, UINT32 u4Size)
{
    BOOL fgRet;
    UINT32 u4BufPAddr, u4BufLen;
    UINT8 *pu1Buf;

    u4BufLen = TZ_CACHE_ALIGN(u4Size);
    u4BufPAddr = BSP_AllocAlignedDmaMemory(u4BufLen, TZ_CACHE_LINE_SIZE);
    pu1Buf = (UINT8 *)VIRTUAL(u4BufPAddr);

    x_memcpy((void *)pu1Buf, prKeyInfo, u4Size);

    fgRet = TZ_CTL(TZCTL_DMX_SETMMSECURITY, u4BufPAddr, u4BufLen);
    
    BSP_FreeAlignedDmaMemory(u4BufPAddr);
    return fgRet;
}

BOOL TZ_GCPU_Hw_CmdRun(void *prKernParam, UINT32 u4ParamSize)
{
    return TZ_CTL(TZCTL_GCPU_HW_CMDRUN, PHYSICAL((UINT32)prKernParam), u4ParamSize);
}

BOOL TZ_GCPU_Hw_CmdReturn(void *prKernParam, UINT32 u4ParamSize)
{
    return TZ_CTL(TZCTL_GCPU_HW_CMDRETURN, PHYSICAL((UINT32)prKernParam), u4ParamSize);
}

BOOL TZ_GCPU_EnableIOMMU(void *prKernParam, UINT32 u4ParamSize)
{
    return TZ_CTL(TZCTL_GCPU_ENABLE_IOMMU, PHYSICAL((UINT32)prKernParam), u4ParamSize);
}

BOOL TZ_GCPU_DisableIOMMU(void)
{
    return TZ_CTL(TZCTL_GCPU_DISABLE_IOMMU, 0, 0);
}

BOOL TZ_GCPU_ISR_ClearIRQ(void)
{
    /* called in ISR */
    return TZ_CTL(TZCTL_GCPU_CLEAR_IRQ, 0, 0);
}

BOOL TZ_GCPU_ClearIRQ(void)
{
    return TZ_CTL(TZCTL_GCPU_CLEAR_IRQ, 0, 0);
}

BOOL TZ_GCPU_ISR_ClearIommuIRQ(void)
{
    /* called in ISR */
    return TZ_CTL(TZCTL_GCPU_CLEAR_IOMMU_IRQ, 0, 0);
}

BOOL TZ_GCPU_ClearIommuIRQ(void)
{
    return TZ_CTL(TZCTL_GCPU_CLEAR_IOMMU_IRQ, 0, 0);
}

BOOL TZ_GCPU_Hw_SetSlot(void *prKernParam, UINT32 u4ParamSize)
{
    return TZ_CTL(TZCTL_GCPU_SET_SLOT, PHYSICAL((UINT32)prKernParam), u4ParamSize);
}

BOOL TZ_GCPU_Hw_Reset(void *prKernParam, UINT32 u4ParamSize)
{
    return TZ_CTL(TZCTL_GCPU_HW_RESET, PHYSICAL((UINT32)prKernParam), u4ParamSize);
}

BOOL TZ_GCPU_IrqHandle(void *prKernParam, UINT32 u4ParamSize)
{
    /* called in ISR */
    return TZ_CTL(TZCTL_GCPU_IRQ_HANDLER, PHYSICAL((UINT32)prKernParam), u4ParamSize);
}

BOOL TZ_GCPU_IOMMU_IrqHandle(void *prKernParam, UINT32 u4ParamSize)
{
    /* called in ISR */
    return TZ_CTL(TZCTL_GCPU_IOMMU_IRQ_HANDLER, PHYSICAL((UINT32)prKernParam), u4ParamSize);
}

BOOL TZ_GCPU_GetRomCodeChecksum(void *prKernParam, UINT32 u4ParamSize)
{
    return TZ_CTL(TZCTL_GCPU_GET_CHECKSUM, PHYSICAL((UINT32)prKernParam), u4ParamSize);
}

BOOL TZ_GCPU_Hw_RestoreSecureKeys(void)
{
    return TZ_CTL(TZCTL_GCPU_RESTORE_SECURE_KEYS, 0, 0);
}

BOOL TZ_OEMCrypto_Initialize(void *prKernParam               , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_Initialize                , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_Terminate(void *prKernParam                , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_Terminate                 , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_OpenSession(void *prKernParam              , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_OpenSession               , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_CloseSession(void *prKernParam             , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_CloseSession              , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_GenerateDerivedKeys(void *prKernParam      , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_GenerateDerivedKeys       , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_GenerateNonce(void *prKernParam            , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_GenerateNonce             , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_GenerateSignature(void *prKernParam        , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_GenerateSignature         , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_LoadKeys(void *prKernParam                 , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_LoadKeys                  , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_RefreshKeys(void *prKernParam              , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_RefreshKeys               , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_SelectKey(void *prKernParam                , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_SelectKey                 , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_DecryptCTR(void *prKernParam               , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_DecryptCTR                , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_InstallKeybox(void *prKernParam            , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_InstallKeybox             , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_IsKeyboxValid(void *prKernParam            , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_IsKeyboxValid             , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_GetDeviceID(void *prKernParam              , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_GetDeviceID               , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_GetKeyData(void *prKernParam               , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_GetKeyData                , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_GetRandom(void *prKernParam                , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_GetRandom                 , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_WrapKeybox(void *prKernParam               , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_WrapKeybox                , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_RewrapDeviceRSAKey(void *prKernParam       , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_RewrapDeviceRSAKey        , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_LoadDeviceRSAKey(void *prKernParam         , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_LoadDeviceRSAKey          , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_GenerateRSASignature(void *prKernParam     , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_GenerateRSASignature      , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_DeriveKeysFromSessionKey(void *prKernParam , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_DeriveKeysFromSessionKey  , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_Generic_Encrypt(void *prKernParam          , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_Generic_Encrypt           , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_Generic_Decrypt(void *prKernParam          , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_Generic_Decrypt           , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_Generic_Sign(void *prKernParam             , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_Generic_Sign              , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_Generic_Verify(void *prKernParam           , UINT32 u4Size){
    return TZ_CTL( TZCTL_OEMCrypto_Generic_Verify            , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_OEMCrypto_SetEntitlementKey(void *prKernParam        , UINT32 u4Size) {
    return TZ_CTL( TZCTL_OEMCrypto_SetEntitlementKey      , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_DeriveControlWord(void *prKernParam        , UINT32 u4Size) {
    return TZ_CTL( TZCTL_OEMCrypto_DeriveControlWord      , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_DecryptVideo(void *prKernParam             , UINT32 u4Size) {
    return TZ_CTL( TZCTL_OEMCrypto_DecryptVideo           , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_DecryptAudio(void *prKernParam             , UINT32 u4Size) {
    return TZ_CTL( TZCTL_OEMCrypto_DecryptAudio           , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_UpdateUsageTable(void *prKernParam, UINT32 u4Size){ 
    return TZ_CTL( TZCTL_OEMCrypto_UpdateUsageTable              , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_DeactivateUsageEntry(void *prKernParam, UINT32 u4Size){ 
    return TZ_CTL( TZCTL_OEMCrypto_DeactivateUsageEntry              , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_ReportUsage(void *prKernParam, UINT32 u4Size){ 
    return TZ_CTL( TZCTL_OEMCrypto_ReportUsage              , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_DeleteUsageEntry(void *prKernParam, UINT32 u4Size){ 
    return TZ_CTL( TZCTL_OEMCrypto_DeleteUsageEntry              , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_DeleteUsageTable(void *prKernParam, UINT32 u4Size){ 
    return TZ_CTL( TZCTL_OEMCrypto_DeleteUsageTable              , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_GetUsageTableWriteRequest(void *prKernParam, UINT32 u4Size){ 
    return TZ_CTL( TZCTL_OEMCrypto_GetUsageTableWriteRequest              , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_VerifyWriteResponse(void *prKernParam, UINT32 u4Size){ 
    return TZ_CTL( TZCTL_OEMCrypto_VerifyWriteResponse              , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_GetUsageTableReadRequest(void *prKernParam, UINT32 u4Size){ 
    return TZ_CTL(TZCTL_OEMCrypto_GetUsageTableReadRequest              , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_OEMCrypto_VerifyReadResponse(void *prKernParam, UINT32 u4Size){ 
    return TZ_CTL(TZCTL_OEMCrypto_VerifyReadResponse              , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}

BOOL TZ_SEC_BUF_OPEN_SESSION(void *prKernParam  , UINT32 u4Size) {
    return TZ_CTL(TZCTL_SEC_BUF_OPEN_SESSION   , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_SEC_BUF_CLOSE_SESSION(void *prKernParam , UINT32 u4Size) {
    return TZ_CTL(TZCTL_SEC_BUF_CLOSE_SESSION  , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_SEC_BUF_ALLOC(void *prKernParam         , UINT32 u4Size) {
    return TZ_CTL(TZCTL_SEC_BUF_ALLOC          , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_SEC_BUF_FREE(void *prKernParam          , UINT32 u4Size) {
    return TZ_CTL(TZCTL_SEC_BUF_FREE           , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_SEC_BUF_FLUSH(void *prKernParam         , UINT32 u4Size) {
    return TZ_CTL(TZCTL_SEC_BUF_FLUSH          , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_SEC_BUF_FRAGMENT_ALLOC(void *prKernParam , UINT32 u4Size) {
    return TZ_CTL(TZCTL_SEC_BUF_FRAGMENT_ALLOC   , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_SEC_BUF_FRAGMENT_FREE(void *prKernParam  , UINT32 u4Size) {
	return TZ_CTL(TZCTL_SEC_BUF_FRAGMENT_FREE    , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_SEC_BUF_CPB_MOVEDATA(void *prKernParam  , UINT32 u4Size) {
	return TZ_CTL(TZCTL_SEC_BUF_CPB_MOVEDATA, PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_SEC_BUF_CPB_GETDATA(void *prKernParam  , UINT32 u4Size) {
	return TZ_CTL(TZCTL_SEC_BUF_CPB_GETDATA, PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}

int TZ_QUERY_TVP_ENABLED(void)
{
	BOOL ret = FALSE;
	UINT8 *p_tmp =(UINT8*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(TZ_CACHE_LINE_SIZE, TZ_CACHE_LINE_SIZE));
	ret=TZ_CTL(TZCTL_QUERY_TVP_ENABLED,PHYSICAL((UINT32)p_tmp) , TZ_CACHE_ALIGN(TZ_CACHE_LINE_SIZE));
	ret = *(UINT32*)p_tmp;
	BSP_FreeAlignedDmaMemory((UINT32)p_tmp);
	if( ret== 1)
		return 1;
	return 0;
}

#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_TVP_SUPPORT)
extern int (*query_tvp_enabled)(void);
BOOL TZ_FBM_Initialization(void *prKernParam  , UINT32 u4Size)
{
	BOOL ret = FALSE;
	UINT8 *p_tmp =(UINT8*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(u4Size, TZ_CACHE_LINE_SIZE));
	x_memcpy(p_tmp,prKernParam,u4Size);
	ret=TZ_CTL(TZCTL_FBM_INITIALIZATION,PHYSICAL((UINT32)p_tmp) , TZ_CACHE_ALIGN(u4Size));
	BSP_FreeAlignedDmaMemory((UINT32)p_tmp);
	query_tvp_enabled = TZ_QUERY_TVP_ENABLED;
	return ret;
}
#endif
BOOL TZ_RPMB_LOAD_MAC_KEY(void *prKernParam         , UINT32 u4Size) {
    return TZ_CTL(TZCTL_RPMB_LOAD_MAC_KEY          , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_RPMB_GET_WCNT_REQ(void *prKernParam , UINT32 u4Size) {
    return TZ_CTL(TZCTL_RPMB_GET_WCNT_REQ   , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}
BOOL TZ_RPMB_SET_WCNT_RESP(void *prKernParam  , UINT32 u4Size) {
	return TZ_CTL(TZCTL_RPMB_SET_WCNT_RESP    , PHYSICAL((UINT32)prKernParam) , TZ_CACHE_ALIGN(u4Size));
}   
