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
 * $Date: 2015/03/05 $
 * $RCSfile: assert.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gcpu_lite.c
 *  gcpulite.c this is a lite version of GCPU driver that can used in preloader
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_MTK_PRELOADER

#include "x_assert.h"
#include "x_printf.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"

#include "gcpu.h"

#if defined(CC_MT5890)||defined(CC_MT5882)
#include "gcpu_iram0_90.h"
#elif defined(CC_MT5399)
#include "gcpu_iram1_99.h"
#elif defined(CC_MT5880)
#include "gcpu_iram1_98.h"
#else
#error "Pleases sepecific GCPU RAM code version"
#endif

#define AES_KEY_LEN_WRAPPER(len) (len == KEY_BIT_LEN_128)?0:(len == KEY_BIT_LEN_192)?1:2
#define IsValidSecureSlot(u4Ptr)    ((u4Ptr >= GCPU_SECURESLOT_PTR) && \
                                     (u4Ptr < GCPU_SECURESLOT_PTR + GCPU_SECURESLOT_NUM))

static UINT32 _u4Mode = 0;
static AES_PARAM_T rAesPrivate;
static GCPU_STRUCT_T rGcpuMtd;

static UINT32 _Gcpu_Align(UINT32 u4Addr, UINT32 u4Alignment)
{
	UINT32 u4Unaligned;

	if (u4Alignment <= 1)
	{
		return u4Addr;
	}

	u4Unaligned = u4Addr % u4Alignment;
	if (u4Unaligned != 0)
	{
		u4Addr += u4Alignment - u4Unaligned;
	}

	return u4Addr;
}

static BOOL _Gcpu_IsAligned(UINT32 u4Addr, UINT32 u4Alignment)
{
	return u4Addr == _Gcpu_Align(u4Addr, u4Alignment);
}

UINT32 _GCPU_CopyRingBuffer(UINT32 u4Dst, UINT32 u4DstStart, UINT32 u4DstEnd,
    UINT32 u4Src, UINT32 u4SrcStart, UINT32 u4SrcEnd, UINT32 u4Size)
{
    UINT32 i;
    UINT8 *pSrc, *pDst;

    ASSERT((u4Dst >= u4DstStart) && (u4Dst < u4DstEnd));
    ASSERT((u4Src >= u4SrcStart) && (u4Src < u4SrcEnd));

    pSrc = (UINT8*)u4Src;
    pDst = (UINT8*)u4Dst;

    for (i = 0; i < u4Size; i++)
    {
        *pDst++ = *pSrc++;

        if ((UINT32)pDst >= u4DstEnd)
        {
            pDst = (UINT8*)u4DstStart;
        }

        if ((UINT32)pSrc >= u4SrcEnd)
        {
            pSrc = (UINT8*)u4SrcStart;
        }
    }

    return u4Dst;
}

static BOOL _Gcpu_Hw_GetRamCode(UINT32 *pu4RamCode, UINT32 *pu4RamCodeSize, UINT32 *pu4Trapping, UINT32 *pu4TrapSize)
{
    if ((pu4RamCode == NULL) || (pu4Trapping == NULL) || (pu4RamCodeSize == NULL) || (pu4TrapSize == NULL))
    {
        return FALSE;
    }

    #if defined(CC_MT5890)||defined(CC_MT5882)
    *pu4RamCode = (UINT32) GCPU_IRAM0_CODE;
    *pu4Trapping = (UINT32) GCPU_IRAM0_TRAPPING;

    *pu4RamCodeSize = sizeof(GCPU_IRAM0_CODE);
    *pu4TrapSize = sizeof(GCPU_IRAM0_TRAPPING);
    #elif defined(CC_MT5399) || defined(CC_MT5880)
    *pu4RamCode = (UINT32) GCPU_IRAM1_CODE;
    *pu4Trapping = (UINT32) GCPU_IRAM1_TRAPPING;

    *pu4RamCodeSize = sizeof(GCPU_IRAM1_CODE);
    *pu4TrapSize = sizeof(GCPU_IRAM1_TRAPPING);
    #else
    #error "Pleases sepecific GCPU RAM code version"
    #endif
    return TRUE;
}

static BOOL _Gcpu_Hw_SetResetParam(GCPU_TZ_PARAM_T *prTzParam)
{
    UINT32 u4PhyRamCodeAddr = 0;
    UINT32 u4PhyTrapAddr = 0;
    UINT32 u4RamCodeSize = 0, u4TrapSize = 0;
    UINT32 u4RamSrcCode = NULL, u4TrappingSrc = NULL;
    BOOL fgInitRam = FALSE, fgInitDram = FALSE;
    BOOL fgRet = TRUE;

    if (prTzParam == NULL)
    {
        return FALSE;
    }

    memset((void *)prTzParam, 0, sizeof(GCPU_TZ_PARAM_T));
    if(_u4Mode == 0)
    {
        fgInitRam = TRUE;
    }
    else
    {
        fgInitDram = TRUE;
    }

    prTzParam->u4InitRam = (UINT32)fgInitRam;
    prTzParam->u4InitDram = (UINT32)fgInitDram;

    if (fgInitRam)
    {
        fgRet = _Gcpu_Hw_GetRamCode((UINT32 *)&u4RamSrcCode, (UINT32 *)&u4RamCodeSize,
                                    (UINT32 *)&u4TrappingSrc, (UINT32 *)&u4TrapSize);

        if (!fgRet)
        {
            LOG(1, "Can't get ram and grap code\n");
            goto lbExit1;
        }

        u4PhyRamCodeAddr = (UINT32)u4RamSrcCode;
        u4PhyTrapAddr = (UINT32)u4TrappingSrc;

        prTzParam->pu4RamCode = (UINT32 *)u4PhyRamCodeAddr;
        prTzParam->pu4Trapping = (UINT32 *)u4PhyTrapAddr ;
        prTzParam->u4RamCodeSize = u4RamCodeSize / 4;
        prTzParam->u4TrapSize = u4TrapSize / 4;
    }

    UNUSED(u4PhyRamCodeAddr);
    UNUSED(u4PhyTrapAddr);

    return TRUE;

lbExit1:

    return fgRet;
}

static INT32 _Gcpu_Hw_Reset(GCPU_TZ_PARAM_T *prTzParam)
{
    UINT32 u4Value = 0;
    UINT32 *pu4RamCode = NULL, *pu4Trapping = NULL;
    UINT32 u4RamCodeSize = 0, u4TrapSize = 0;
    UINT32 u4PhyDramCodeAddr = 0;
    BOOL fgInitRam, fgInitDram;

    if (prTzParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    pu4RamCode = (UINT32 *)VIRTUAL((UINT32)prTzParam->pu4RamCode);
    pu4Trapping = (UINT32 *)VIRTUAL((UINT32)prTzParam->pu4Trapping);
    u4RamCodeSize = prTzParam->u4RamCodeSize;
    u4TrapSize = prTzParam->u4TrapSize;
    u4PhyDramCodeAddr = prTzParam->u4PhyDramCodeAddr;
    fgInitRam = (BOOL)prTzParam->u4InitRam;
    fgInitDram = (BOOL)prTzParam->u4InitDram;

    u4Value = GCPUCMD_READ32(GCPU_REG_CTL);
    u4Value &= ~(0x7);
    GCPUCMD_WRITE32(GCPU_REG_CTL, u4Value);
    u4Value &= ~(0x8);
    GCPUCMD_WRITE32(GCPU_REG_CTL, u4Value);

    u4Value |= 0xF;
    GCPUCMD_WRITE32(GCPU_REG_CTL, u4Value);

    if ((fgInitRam) &&
        (pu4RamCode != NULL) && (u4RamCodeSize != 0) &&
        (pu4Trapping != NULL) && (u4TrapSize != 0))
    {
        //Init Ram Code
        UINT32 i;

        // clear
        GCPUCMD_WRITE32(GCPU_REG_MEM_ADDR, 0x80002000);
        for (i = 0; i < 1024; i++)
        {
            GCPUCMD_WRITE32(GCPU_REG_MEM_DATA, 0);
        }

        GCPUCMD_WRITE32(GCPU_REG_MEM_ADDR, 0x80002000);
        for (i = 0; i < u4RamCodeSize; i++)
        {
            GCPUCMD_WRITE32(GCPU_REG_MEM_DATA, *(pu4RamCode + i));
        }

        // trapping
        for (i = 0; i < u4TrapSize; i++)
        {
            GCPUCMD_WRITE32((GCPU_REG_TRAP_START + i), *(pu4Trapping + i));
        }
    }

    UNUSED(fgInitDram);
    UNUSED(u4PhyDramCodeAddr );

    return S_GCPU_OK;
}

INT32 _GCPU_Hw_Init(UINT32 u4Mode, BOOL fgIsLocked)
{
    GCPU_TZ_PARAM_T rTzParam;

    CKGEN_WRITE32(0x1C8, CKGEN_READ32(0x1C8) | (1 << 20));
    #ifdef CC_DMX_A1
    CKGEN_WRITE32(0x39C, 0x1);  // GCPU Max clock
    #else
    CKGEN_WRITE32(0x39C, 0x2);
    #endif

    // Enable GCPU
    _u4Mode = u4Mode;

    if (_Gcpu_Hw_SetResetParam(&rTzParam))
    {
        _Gcpu_Hw_Reset(&rTzParam);
    }
    else
    {
        LOG(1, "GCPU invalid reset parameters\n");
    }

    return S_GCPU_OK;
}

INT32 _GCPU_DisableIOMMU(void)
{
    IOMMU_GCPU_WRITE32(REG_IOMMU_CFG0, 0);

    return S_GCPU_OK;
}

static void GCPU_IRQ_LOCK(void)
{
    while((GCPUCMD_READ32(GCPU_REG_INT_SET) & 0x1) == 0x0);
    GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPUCMD_READ32(GCPU_REG_INT_CLR));
    GCPUCMD_WRITE32(GCPU_REG_INT_EN, 0);
}


INT32 _GCPU_Hw_SetSlot(UINT32 u4Idx, UINT32 u4Data)
{
    GCPUCMD_WRITE32(GCPU_REG_MEM_CMD + u4Idx, u4Data);

    return S_GCPU_OK;
}

INT32 _GCPU_SetSecureSlot(UINT32 u4SlotPtr, UINT8* pu1Data, UINT32 u4Size)
{
    UINT8 i, *p, u1Loop;

    if(u4Size > 32)
    {
        return E_GCPU_PARAM_WRONG;
    }

    if(u4SlotPtr > GCPU_SECURESLOT_PTR + GCPU_SECURESLOT_NUM)
    {
        return E_GCPU_PARAM_WRONG;
    }

    u1Loop = (u4Size + 3) / 4;
    for(i = 0; i < u1Loop; i++)
    {
        p = pu1Data + i * 4;
        _GCPU_Hw_SetSlot((u4SlotPtr + i), KEY_WRAPPER(p));
    }

    return S_GCPU_OK;
}

BOOL GCPU_SetAesKey(UINT8 *pu1Key, KEY_BIT_LEN eKeyBitLen, UINT8 *pu1Iv)
{
#ifdef CC_SECURE_BOOT_SCRAMBLE
    // Only support AES 128 bits key
    if(eKeyBitLen > 128)
    {
        return FALSE;
    }

    rAesPrivate.eMode = BLOCK_CIPHER_MODE_CBC;
    rAesPrivate.eKeyBitLen = KEY_BIT_LEN_128;
    rGcpuMtd.u4AesKeyPtr = LOADER_AES_KEY_PTR;
    rGcpuMtd.u4AesIvPtr = LOADER_AES_IV_PTR;
    if((pu1Key != NULL) && (pu1Key != NULL))
    {
        _GCPU_SetSecureSlot(rGcpuMtd.u4AesKeyPtr, pu1Key, 16);
        _GCPU_SetSecureSlot(rGcpuMtd.u4AesIvPtr, pu1Iv, 16);
    }
#endif // CC_SECURE_BOOT_SCRAMBLE
    return TRUE;
}

static INT32 _GCPU_Hw_RunCmd(GCPU_HW_CMD_T *prHwCmd)
{
    UINT32 i;
    INT32 i4Result = S_GCPU_OK;

    // Setup Parameter
    for(i = 1; i < GCPU_PARAM_NUM; i++)
    {
        GCPUCMD_WRITE32(GCPU_REG_MEM_CMD + i, prHwCmd->u4Param[i]);
    }

    // Clear/Enable GCPU Interrupt
    GCPUCMD_WRITE32(GCPU_REG_INT_CLR, GCPUCMD_READ32(GCPU_REG_INT_CLR));
    GCPUCMD_WRITE32(GCPU_REG_INT_EN, GCPU_INT_MASK);

    // GCPU Decryption Mode
    GCPUCMD_WRITE32(GCPU_REG_MEM_CMD, prHwCmd->u4Param[0]);

    // GCPU PC
    GCPUCMD_WRITE32(GCPU_REG_PC_CTL, 0x0);

    GCPU_IRQ_LOCK();

    // read back param 0 - 31
    for(i = 0; i < 32; i++)
    {
        prHwCmd->u4Param[i] = GCPUCMD_READ32(GCPU_REG_MEM_CMD + i);
    }

    // read back param 32 - 47
    for(i = GCPU_PARAM_RET_PTR; i < GCPU_PARAM_RET_PTR + GCPU_PARAM_RET_NUM; i++)
    {
        prHwCmd->u4Param[i] = GCPUCMD_READ32(GCPU_REG_MEM_CMD + i);
    }

    UNUSED(i);

    return i4Result;
}

INT32 _GCPU_Hw_CmdEx(GCPU_HW_CMD_T *prHwCmd, const GCPU_IO_BUF_T *prBuf, UINT32 u4Flags)
{
    INT32 i4Result = S_GCPU_OK;

    if ((prHwCmd == NULL) || (prBuf == NULL))
    {
        return E_GCPU_PARAM_WRONG;
    }

    _GCPU_DisableIOMMU();

    i4Result = _GCPU_Hw_RunCmd(prHwCmd);

    return i4Result;
}

// This simplified API does not support u4Handle and fgIOMMU
static INT32 _GCPU_AES_CmdEx(UINT32 u4Handle, UINT32 u4Id, void* prParam, BOOL fgIOMMU)
{
    INT32 i4Ret;
    UINT32 u4Tmp, i, u4Flags = 0;
    GCPU_HW_CMD_T rHwCmd;
    GCPU_IO_BUF_T rIoBuf;
    UINT8 *p;
    GCPU_STRUCT_T *prGcpu;

    UNUSED(i4Ret);

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_AES:
        {
            AES_PARAM_T *prLParam = (AES_PARAM_T *)prParam;
            AES_PARAM_T rLPParam = *prLParam;

            if(!fgIOMMU)
            {
                //traslate to physical memory
                rLPParam.u4SrcStartAddr = PHYSICAL(rLPParam.u4SrcStartAddr);
                rLPParam.u4SrcBufStart = PHYSICAL(rLPParam.u4SrcBufStart);
                rLPParam.u4SrcBufEnd = PHYSICAL(rLPParam.u4SrcBufEnd);
                rLPParam.u4DstStartAddr = PHYSICAL(rLPParam.u4DstStartAddr);
                rLPParam.u4DstBufStart = PHYSICAL(rLPParam.u4DstBufStart);
                rLPParam.u4DstBufEnd = PHYSICAL(rLPParam.u4DstBufEnd);
            }

            if(u4Handle != 0)
            {
                prGcpu = (GCPU_STRUCT_T *)u4Handle;
                LOG(7, "Use GCPU handler\n");
            }
            else
            {
                prGcpu = NULL;
            }


            if (!_Gcpu_IsAligned(rLPParam.u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(rLPParam.u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(rLPParam.u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(rLPParam.u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if ((!_GCPU_IsInRegion(rLPParam.u4SrcStartAddr, rLPParam.u4SrcBufStart,
                    rLPParam.u4SrcBufEnd)) ||
                (!_GCPU_IsInRegion(rLPParam.u4DstStartAddr, rLPParam.u4DstBufStart,
                    rLPParam.u4DstBufEnd)))
            {
                return E_GCPU_PARAM_WRONG;
            }

            // setup command
            switch(rLPParam.eMode)
            {
            case BLOCK_CIPHER_MODE_ECB:
                if(rLPParam.fgEncrypt)
                {
                    rHwCmd.u4Param[0] = AES_EPAK;
                }
                else
                {
                    rHwCmd.u4Param[0] = AES_DPAK;
                }
                break;
            case BLOCK_CIPHER_MODE_CTR:
                // the same operation for encryption and decryption
                rHwCmd.u4Param[0] = AES_CTR;
                break;
            case BLOCK_CIPHER_MODE_OFB:
                // the same operation for encryption and decryption
                rHwCmd.u4Param[0] = AES_OFB;
                break;
            case BLOCK_CIPHER_MODE_CBC:
                if(rLPParam.fgEncrypt)
                {
                    rHwCmd.u4Param[0] = AES_ECBC;
                }
                else
                {
                    rHwCmd.u4Param[0] = AES_DCBC;
                }
                break;

            default:
                return E_GCPU_UNEXPECT;
            }

            // setup parameter
            rHwCmd.u4Param[1] = rLPParam.u4SrcStartAddr;
            rHwCmd.u4Param[2] = rLPParam.u4DstStartAddr;
            if(rLPParam.eMode == BLOCK_CIPHER_MODE_CBC ||
                rLPParam.eMode == BLOCK_CIPHER_MODE_ECB)
            {
                rHwCmd.u4Param[3] = rLPParam.u4DatLen / 16;
            }
            else
            {
                rHwCmd.u4Param[3] = rLPParam.u4DatLen;
            }

            rHwCmd.u4Param[4] = AES_KEY_LEN_WRAPPER(rLPParam.eKeyBitLen);

            if(prGcpu != NULL)
            {
                if(prGcpu->u4AesKeyPtr != 0)
                {
                    if(IsValidSecureSlot(prGcpu->u4AesKeyPtr))
                    {
                        LOG(7, "Use secure aes key ptr\n");
                        rHwCmd.u4Param[5] = prGcpu->u4AesKeyPtr;
                    }
                    else
                    {
                        LOG(3, "Wrong seucre aes key slot ptr\n");
                        rHwCmd.u4Param[5] = GCPU_PARAM_AES_KEY_PTR;
                    }
                }
                else
                {
                    LOG(5, "Use normal aes key slot ptr\n");
                    rHwCmd.u4Param[5] = GCPU_PARAM_AES_KEY_PTR;
                }

                if(prGcpu->u4AesIvPtr != 0)
                {
                    if(IsValidSecureSlot(prGcpu->u4AesIvPtr))
                    {
                        LOG(7, "Use secure aes iv ptr\n");
                        rHwCmd.u4Param[6] = prGcpu->u4AesIvPtr;
                    }
                    else
                    {
                        LOG(3, "Wrong seucre aes iv slot ptr\n");
                        rHwCmd.u4Param[6] = GCPU_PARAM_AES_IV_PTR;
                    }
                }
                else
                {
                    LOG(5, "Use normal aes iv slot ptr\n");
                    rHwCmd.u4Param[6] = GCPU_PARAM_AES_IV_PTR;
                }

                if(prGcpu->u4AesRetIvPtr != 0)
                {
                    if(IsValidSecureSlot(prGcpu->u4AesRetIvPtr))
                    {
                        LOG(7, "Use secure aes ret iv ptr\n");
                        rHwCmd.u4Param[7] = prGcpu->u4AesRetIvPtr;
                    }
                    else
                    {
                        LOG(3, "Wrong seucre aes ret iv slot ptr\n");
                        rHwCmd.u4Param[7] = GCPU_PARAM_AES_RET_IV_PTR;
                    }
                }
                else
                {
                    LOG(5, "Use normal aes ret iv slot ptr\n");
                    rHwCmd.u4Param[7] = GCPU_PARAM_AES_RET_IV_PTR;
                }
            }
            else
            {
                rHwCmd.u4Param[5] = GCPU_PARAM_AES_KEY_PTR;
                rHwCmd.u4Param[6] = GCPU_PARAM_AES_IV_PTR;
                rHwCmd.u4Param[7] = GCPU_PARAM_AES_RET_IV_PTR;
            }

            rHwCmd.u4Param[8] = rLPParam.u4SrcBufStart;
            rHwCmd.u4Param[9] = rLPParam.u4SrcBufEnd;
            rHwCmd.u4Param[10] = rLPParam.u4DstBufStart;
            rHwCmd.u4Param[11] = rLPParam.u4DstBufEnd;

            //key
            for(i = 0; i < GCPU_PARAM_AES_KEY_NUM; i++)
            {
                p = rLPParam.au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            //iv
            for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
            {
                p = rLPParam.au1Iv + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_IV_PTR + i] = KEY_WRAPPER(p);
            }

            rIoBuf.u4InBufStart = prLParam->u4SrcBufStart;
            rIoBuf.u4InBufSize = prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart;
            rIoBuf.u4OutBufStart = prLParam->u4DstBufStart;
            rIoBuf.u4OutBufSize = prLParam->u4DstBufEnd - prLParam->u4DstBufStart;
            u4Flags |= GCPU_HWCMD_FLAG_DMA_BUF;

            i4Ret = _GCPU_Hw_CmdEx(&rHwCmd, &rIoBuf, u4Flags);

            //return result
            if(prGcpu == NULL || (prGcpu != NULL && prGcpu->u4AesRetIvPtr == 0))
            {
                for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
                {
                    u4Tmp = rHwCmd.u4Param[GCPU_PARAM_AES_RET_IV_PTR + i];
                    prLParam->au1Iv[i * 4] = (u4Tmp & 0xff);
                    prLParam->au1Iv[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                    prLParam->au1Iv[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                    prLParam->au1Iv[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
                }
            }
        }
        break;
    default:
        return E_GCPU_UNEXPECT;
    }

    return S_GCPU_OK;
}

INT32 _GCPU_MISC_CmdEx(UINT32 u4Handle, UINT32 u4Id, void* prParam, BOOL fgIOMMU)
{
    INT32 i4Ret = E_GCPU_UNEXPECT;
#if 1
    UINT32 u4Tmp, i, u4Flags = 0;
    GCPU_HW_CMD_T rHwCmd;
    GCPU_IO_BUF_T rIoBuf;
    UINT8 *p;

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_SHA_1:
    case GCPU_SHA_256:
    case GCPU_SHA_224:
    case GCPU_MD5:
        {
            MD_PARAM_T *prLParam = (MD_PARAM_T *)prParam;
            MD_PARAM_T rLPParam = *prLParam;
            UINT64 u8DataLenBitCnt = 0;
            UINT8 u1PadFlag = 0;
            static UINT8 *au1PadBuffer = NULL;

            if(!fgIOMMU)
            {
                //traslate to physical memory
                rLPParam.u4SrcStartAddr = PHYSICAL(rLPParam.u4SrcStartAddr);
                rLPParam.u4SrcBufStart = PHYSICAL(rLPParam.u4SrcBufStart);
                rLPParam.u4SrcBufEnd = PHYSICAL(rLPParam.u4SrcBufEnd);
            }

            if (!_Gcpu_IsAligned(rLPParam.u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(rLPParam.u4SrcBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(rLPParam.u4SrcStartAddr, rLPParam.u4SrcBufStart,
                    rLPParam.u4SrcBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if(u4Id == GCPU_SHA_1)
            {
                rHwCmd.u4Param[0] = SHA_1;
            }
            else if(u4Id == GCPU_SHA_224)
            {
                rHwCmd.u4Param[0] = SHA_224;
            }
            else if(u4Id == GCPU_SHA_256)
            {
                rHwCmd.u4Param[0] = SHA_256;
            }
            else if(u4Id == GCPU_MD5)
            {
                rHwCmd.u4Param[0] = MD5;
            }

            if (prLParam->fgFirstPacket)
            {
                rHwCmd.u4Param[3] = HASH_FIRST_PACKET_MODE;
                // seed
                for(i = 0; i < 8; i++)
                {
                    p = rLPParam.au1Hash + i * 4;
                    rHwCmd.u4Param[4 + i] = 0;
                }
            }
            else
            {
                rHwCmd.u4Param[3] = HASH_SUCCESSIVE_PACKET_MODE;
                // seed
                for(i = 0; i < 8; i++)
                {
                    p = rLPParam.au1Hash + i * 4;
                    rHwCmd.u4Param[4 + i] = KEY_WRAPPER(p);
                }
            }

            //Padding Check
            if ((rLPParam.fgFirstPacket == TRUE) && (rLPParam.fgLastPacket == TRUE))
            {
                //One Packet Case
                u8DataLenBitCnt = (UINT64)rLPParam.u4DatLen * 8;    //Byte to Bit
                u1PadFlag = 1;
            }
            else if (rLPParam.fgLastPacket == TRUE)
            {
                //Last Packet Case
                u8DataLenBitCnt = ((UINT64)rLPParam.u4DatLen * 8) + rLPParam.u8BitCnt;    //Byte to Bit
                u1PadFlag = 1;
            }

            if (u1PadFlag == 0)
            {
                //No Padding
                rHwCmd.u4Param[1] = rLPParam.u4SrcStartAddr;
                rHwCmd.u4Param[2] = rLPParam.u4DatLen / GCPU_SHA_1_PACKET_SIZE;
                rHwCmd.u4Param[12] = rLPParam.u4SrcBufStart;
                rHwCmd.u4Param[13] = rLPParam.u4SrcBufEnd;

                rIoBuf.u4InBufStart = prLParam->u4SrcBufStart;
                rIoBuf.u4InBufSize = prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart;
                rIoBuf.u4OutBufStart = 0;
                rIoBuf.u4OutBufSize = 0;
                u4Flags |= GCPU_HWCMD_FLAG_DMA_BUF;

                i4Ret = _GCPU_Hw_CmdEx(&rHwCmd, &rIoBuf, u4Flags);

            }
            else
            {
                UINT32 u4OptCnt, u4RemSize, u4RemBufOffset, u4PadOffset, u4PadBufSize, u4BitCnt;

                //Padding
                u4OptCnt = rLPParam.u4DatLen/GCPU_SHA_1_PACKET_SIZE;
                u4RemSize = rLPParam.u4DatLen - (u4OptCnt*GCPU_SHA_1_PACKET_SIZE);
                u4RemBufOffset = ADDR_INCR_IN_RING(prLParam->u4SrcStartAddr,
                    u4OptCnt * GCPU_SHA_1_PACKET_SIZE, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd);

                if (u4OptCnt)
                {
                    rHwCmd.u4Param[1] = rLPParam.u4SrcStartAddr;
                    rHwCmd.u4Param[2] = u4OptCnt;
                    rHwCmd.u4Param[12] = rLPParam.u4SrcBufStart;
                    rHwCmd.u4Param[13] = rLPParam.u4SrcBufEnd;

                    rIoBuf.u4InBufStart = prLParam->u4SrcBufStart;
                    rIoBuf.u4InBufSize = prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart;
                    rIoBuf.u4OutBufStart = 0;
                    rIoBuf.u4OutBufSize = 0;
                    u4Flags |= GCPU_HWCMD_FLAG_DMA_BUF;

                    i4Ret = _GCPU_Hw_CmdEx(&rHwCmd, &rIoBuf, u4Flags);

                    // res
                    for(i = 0; i < 8; i++)
                    {
                        u4Tmp = rHwCmd.u4Param[4 + i];
                        rLPParam.au1Hash[i * 4] = (u4Tmp & 0xff);
                        rLPParam.au1Hash[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                        rLPParam.au1Hash[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                        rLPParam.au1Hash[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
                    }
                }

                //Padding SHA1
                if (u4RemSize + 9 > GCPU_SHA_1_PACKET_SIZE)
                {
                    u4PadOffset = 32;
                    u4PadBufSize = GCPU_SHA_1_PACKET_SIZE * 2;
                }
                else
                {
                    u4PadOffset = 16;
                    u4PadBufSize = GCPU_SHA_1_PACKET_SIZE;
                }

                //if(au1PadBuffer == NULL)
                {
                    au1PadBuffer = 0xf00040;
                }

                memset(au1PadBuffer, 0x0, u4PadBufSize);
                _GCPU_CopyRingBuffer((UINT32)au1PadBuffer, (UINT32)au1PadBuffer,
                    (UINT32)(au1PadBuffer + GCPU_SHA_1_PACKET_SIZE * 2), u4RemBufOffset,
                    prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, u4RemSize);

                //Add one bit
                au1PadBuffer[u4RemSize] = 0x80;

                if(u4Id == GCPU_MD5)
                {
                    u4BitCnt = (UINT32) (((UINT32) (u8DataLenBitCnt & 0xFFFFFFFF)));
                    x_memcpy(au1PadBuffer + (u4PadOffset * 4 - 8), (BYTE*)&u4BitCnt, 4);
                    u4BitCnt = (UINT32) (UINT32) (((UINT32) ((u8DataLenBitCnt >> 32) & 0xFFFFFFFF)));
                    x_memcpy(au1PadBuffer + (u4PadOffset * 4 - 4), (BYTE*)&u4BitCnt, 4);
                }
                else
                {
                    u4BitCnt = (UINT32) (SwitchValue((UINT32) (u8DataLenBitCnt >> 32) & 0xFFFFFFFF));
                    x_memcpy(au1PadBuffer + (u4PadOffset * 4 - 8), (BYTE*)&u4BitCnt, 4);
                    u4BitCnt = (UINT32) (SwitchValue((UINT32) (u8DataLenBitCnt & 0xFFFFFFFF)));
                    x_memcpy(au1PadBuffer + (u4PadOffset * 4 - 4), (BYTE*)&u4BitCnt, 4);
                }


                if (u4OptCnt)
                {
                    rHwCmd.u4Param[3] = HASH_SUCCESSIVE_PACKET_MODE;
                    for(i = 0; i < 8; i++)
                    {
                        p = rLPParam.au1Hash + i * 4;
                        rHwCmd.u4Param[4 + i] = KEY_WRAPPER(p);
                    }
                }

                rHwCmd.u4Param[1] = (UINT32)au1PadBuffer;
                rHwCmd.u4Param[2] = (UINT32)u4PadBufSize / GCPU_SHA_1_PACKET_SIZE;
                rHwCmd.u4Param[12] = (UINT32)au1PadBuffer;
                rHwCmd.u4Param[13] = (UINT32)au1PadBuffer + GCPU_SHA_1_PACKET_SIZE * 2;

                rIoBuf.u4InBufStart = (UINT32)au1PadBuffer;
                rIoBuf.u4InBufSize = GCPU_SHA_1_PACKET_SIZE * 2;
                rIoBuf.u4OutBufStart = 0;
                rIoBuf.u4OutBufSize = 0;
                u4Flags |= GCPU_HWCMD_FLAG_DMA_BUF;

                i4Ret = _GCPU_Hw_CmdEx(&rHwCmd, &rIoBuf, u4Flags);
            }

            // res
            for(i = 0; i < 8; i++)
            {
                u4Tmp = rHwCmd.u4Param[4 + i];
                prLParam->au1Hash[i * 4] = (u4Tmp & 0xff);
                prLParam->au1Hash[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->au1Hash[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->au1Hash[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    default:
        return E_GCPU_UNEXPECT;
    }
#endif // 0
    return i4Ret;
}

BOOL GCPU_LoaderInit(UINT32 u4Mode)
{
    BOOL fgRet;

    fgRet = (_GCPU_Hw_Init(u4Mode, FALSE) == S_GCPU_OK)?TRUE:FALSE;
    // We impl init preloader AES struct if AES key/IV are already set in ROM code stage.
    // If you like to set another AES key again, you can call GCPU_SetAesKey() by pu1Key parameter
    GCPU_SetAesKey(NULL, KEY_BIT_LEN_128, NULL);

    return fgRet;
}

BOOL GCPU_Init(UINT32 u4Mode)
{
	return GCPU_LoaderInit(u4Mode);
}

BOOL GCPU_AesDecrypt(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
    rAesPrivate.u4SrcStartAddr = u4InBufStart;
    rAesPrivate.u4SrcBufStart = GCPU_LINER_BUFFER_START(u4InBufStart);
    rAesPrivate.u4SrcBufEnd = GCPU_LINER_BUFFER_END(u4InBufStart + u4BufSize);
    rAesPrivate.u4DstStartAddr = u4OutBufStart;
    rAesPrivate.u4DstBufStart = GCPU_LINER_BUFFER_START(u4OutBufStart);
    rAesPrivate.u4DstBufEnd = GCPU_LINER_BUFFER_END(u4OutBufStart + u4BufSize);
    rAesPrivate.u4DatLen = u4BufSize;
    rAesPrivate.fgEncrypt = FALSE;

    return (_GCPU_AES_CmdEx((UINT32)&rGcpuMtd, GCPU_AES, &rAesPrivate, FALSE) == S_GCPU_OK)?TRUE:FALSE;
}


/*
    Preloader can call this function for doing AES cipher at a time
    without calling other GCPU functions. Caller needs to set all mode/key/buffer/size.
*/
BOOL GCPU_DO_AES(BLOCK_CIPHER_MODE eMode, BOOL fgEncrypt, UINT8 *pu1Key, KEY_BIT_LEN eKeyBitLen,
    UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
    AES_PARAM_T rAesStruct;

    rAesStruct.eMode = eMode;
    rAesStruct.eKeyBitLen = eKeyBitLen;
    x_memcpy(rAesStruct.au1Key, pu1Key, eKeyBitLen/8);
    rAesStruct.fgEncrypt = fgEncrypt;

    rAesStruct.u4SrcStartAddr = u4InBufStart;
    rAesStruct.u4SrcBufStart = GCPU_LINER_BUFFER_START(u4InBufStart);
    rAesStruct.u4SrcBufEnd = GCPU_LINER_BUFFER_END(u4InBufStart + u4BufSize);
    rAesStruct.u4DstStartAddr = u4OutBufStart;
    rAesStruct.u4DstBufStart = GCPU_LINER_BUFFER_START(u4OutBufStart);
    rAesStruct.u4DstBufEnd = GCPU_LINER_BUFFER_END(u4OutBufStart + u4BufSize);
    rAesStruct.u4DatLen = u4BufSize;

    return (_GCPU_AES_CmdEx(0, GCPU_AES, &rAesStruct, FALSE) == S_GCPU_OK)?TRUE:FALSE;
}

BOOL GCPU_SHA256(UINT8 *pu1Buf, UINT32 u4Size, UINT8 Message_Digest[32])
{
    MD_PARAM_T rParam;
    BOOL fgRet;
    UINT32 u4Addr = (UINT32)pu1Buf;

    // make klocwork happy
    memset(rParam.au1Hash, 0, sizeof(rParam.au1Hash));

    rParam.u4SrcStartAddr = u4Addr;
    rParam.u4SrcBufStart = GCPU_LINER_BUFFER_START(u4Addr);
    rParam.u4SrcBufEnd = GCPU_LINER_BUFFER_END(u4Addr + u4Size);
    rParam.u4DatLen = u4Size;
    rParam.fgFirstPacket = TRUE;
    rParam.fgLastPacket = TRUE;
    rParam.u8BitCnt = 0;

    fgRet = (_GCPU_MISC_CmdEx(0, GCPU_SHA_256, &rParam, FALSE) == S_GCPU_OK)?TRUE:FALSE;

    x_memcpy(Message_Digest, rParam.au1Hash, sizeof(rParam.au1Hash));

    return fgRet;
}
#endif // CC_MTK_PRELOADER

