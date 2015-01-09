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
/******************************************************************************
* [File]			msdc_test.c
* [Version]			v1.0
* [Revision Date]	2013-02-28
* [Author]			Shunli Wang, shunli.wang@mediatek.inc, 82896, 2013-02-28
* [Description]
*	extended code about MSDC
* [Copyright]
*	Copyright (C) 2007 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/
#include "msdc_debug.h"
#include "msdc_if.h"
#include "msdc_drv.h"
#include "x_lint.h"
#include "drvcust_if.h"
#include "x_debug.h"
#include "msdc_ext.h"

LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "x_bim.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_timer.h"
#include "x_printf.h"
#include "x_pinmux.h"
LINT_EXT_HEADER_END

// AES Encrypt
#include "dmx_drm_if.h"

#if defined(CC_MTD_ENCRYPT_SUPPORT)

INT32 MSDCEXT_EncryptInit(UINT16 u2KeyLen)
{
	#if 0
    UINT8 u1Key[16] = {0};
    
    if(DMX_NAND_AES_INIT(u1Key, u2KeyLen))
    {
        MSDC_LOG(MSG_LVL_ERR, "AES init success!\n"); 	
        return MSDC_SUCCESS;  	
    }
    else
    {
        MSDC_LOG(MSG_LVL_ERR, "AES init failed!\n"); 	
        return MSDC_FAILED;	
    }
    #endif
    return MSDC_SUCCESS; 
}

INT32 MSDCEXT_Block_Encrypted(UINT32 u4BlkIdx, UINT32 *pu4Data)
{
	  UINT32 u4PartId, u4Encryped, i;
    UINT64 u8Offset, u8PartSize;
    
    for (u4PartId = 0; u4PartId < MSDCPART_PARTNUM; u4PartId++)
    {
        u8PartSize = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
        u8Offset   = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));	
        u4Encryped = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartEncypt0 + u4PartId));
        
        if((u4Encryped != 0) &&
        	 ((UINT32)(u8Offset>>9) <= u4BlkIdx) &&
        	 ((UINT32)((u8Offset + u8PartSize)>>9) > u4BlkIdx))
        {
            for(i = 0; i < 0x80; i++)
            {
                if(pu4Data[i] != 0xFFFFFFFF)
                {
                    return 1;	
                }	
            }    	
        }
    }
    
    return 0;
}

INT32 MSDCEXT_Partition_Encrypted(UINT32 u4BlkIdx)
{
	  UINT32 u4PartId, u4Encryped;
    UINT64 u8Offset, u8PartSize;
    
    for (u4PartId = 0; u4PartId < MSDCPART_PARTNUM; u4PartId++)
    {
        u8PartSize = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
        u8Offset   = DRVCUST_InitGet64((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));	
        u4Encryped = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartEncypt0 + u4PartId));
        
        if((u4Encryped != 0) &&
        	 ((UINT32)(u8Offset>>9) <= u4BlkIdx) &&
        	 ((UINT32)((u8Offset + u8PartSize)>>9) > u4BlkIdx))
        {
        	  MSDC_LOG(MSG_LVL_INFO, "This partition is encrypted(blkidx:0x%08X)!\n", u4BlkIdx); 
            return 1;   	
        }
    }
    
    MSDC_LOG(MSG_LVL_INFO, "This partition is not encrypted(blkidx:0x%08X)!\n", u4BlkIdx); 
    return 0;
}

INT32 MSDCEXT_AES_Encryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
    BOOL fgRet;
    
    fgRet = DMX_NAND_AES_Encryption(u4InBufStart, u4OutBufStart, u4BufSize);	
    
    return fgRet?MSDC_SUCCESS:MSDC_FAILED;
}

INT32 MSDCEXT_AES_Decryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
    BOOL fgRet;
    
    fgRet = DMX_NAND_AES_Decryption(PHYSICAL(u4InBufStart), PHYSICAL(u4OutBufStart), u4BufSize);	
    
    return fgRet?MSDC_SUCCESS:MSDC_FAILED;
    	
}

INT32 MSDCEXT_AES_Encryption_S(UINT32 u4BufStart, UINT32 u4BufSize)
{
    BOOL fgRet;
    UINT32 u4TmpBufStart;
    
    u4TmpBufStart = (UINT32)x_mem_alloc(u4BufSize);
    if(NULL == (void *)u4TmpBufStart)
    {
        MSDC_LOG(MSG_LVL_ERR, "alloc memory(size:0x%08X) failed in Encryption routine!\n", u4TmpBufStart); 	
        return MSDC_FAILED;
    }
    
    fgRet = DMX_NAND_AES_Encryption(PHYSICAL(u4BufStart), PHYSICAL(u4TmpBufStart), u4BufSize);	
    
    x_memcpy((void *)u4BufStart, (void *)u4TmpBufStart, u4BufSize);
    
    x_mem_free((void *)u4TmpBufStart);
    
    return fgRet?MSDC_SUCCESS:MSDC_FAILED;
}

INT32 MSDCEXT_AES_Decryption_S(UINT32 u4BufStart, UINT32 u4BufSize)
{
    BOOL fgRet;
    UINT32 u4TmpBufStart;
    
    u4TmpBufStart = (UINT32)x_mem_alloc(u4BufSize);
    if(NULL == (void *)u4TmpBufStart)
    {
        MSDC_LOG(MSG_LVL_ERR, "alloc memory(size:0x%08X) failed in Decryption routine!\n", u4TmpBufStart); 	
        return MSDC_FAILED;
    }
    
    fgRet = DMX_NAND_AES_Decryption(PHYSICAL(u4BufStart), PHYSICAL(u4TmpBufStart), u4BufSize);	
    
    x_memcpy((void *)u4BufStart, (void *)u4TmpBufStart, u4BufSize);
    
    x_mem_free((void *)u4TmpBufStart);
    
    return fgRet?MSDC_SUCCESS:MSDC_FAILED;
    	
}

#endif // defined(CC_MTD_ENCRYPT_SUPPORT)









