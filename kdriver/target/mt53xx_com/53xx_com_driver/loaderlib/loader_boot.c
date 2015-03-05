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
 * Description:
 *         The main routine of boot loader
 *
 *---------------------------------------------------------------------------*/

#include "x_printf.h"
#include "x_drv_cli.h"
#include "x_assert.h"
#include "x_serial.h"
#include "x_stl_lib.h"
#include "x_timer.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_bim.h"
#include "x_pdwnc.h"
#include "drvcust_if.h"
#include "x_typedef.h"
#include "x_hal_5381.h"
#include "loader_imghdr.h"
#include "loader_if.h"
#include "eeprom_if.h"
#include "c_model.h"
#include "x_ldr_env.h"
#ifdef CC_L_DUAL_BOOT
#include "Lpartinfo.h"
#endif

#ifdef CC_NAND_BOOT
#include "nand_if.h"
#endif

#ifdef CC_EMMC_BOOT
#include "msdc_if.h"
#endif

#include "zip.h"
#include "x_ckgen.h"
#include "x_bim.h"
#ifdef CC_LZMA_DECODE
#include "../lzma/LzmaEnc.h"
#include "../lzma/LzmaLib.h"
#endif

#if defined(CC_SECURESTORAGE_SUPPORT)
#include "gcpu_if.h"
#include "tomcrypt.h"
#include "sha256.h"

#endif

#ifdef CC_ONE_PKG_MULTI_DRAM_SIZE_SUPPORT

#include "x_dram.h"
#include "x_bim.h"

#endif 

#define FIRMWARE_FLASH_OFFSET           LOADER_MAX_SIZE
#if 0
#define HalInstructionMemoryBarrier()
#define HalDisableCaches()
#define HalDisableMMU()
#define Printf(...)
#define SerWaitTxBufClear()
#define SerEnd()
#endif

#if defined(CC_NAND_BOOT)
#define CHIP_FlashCopyToDRAMLZHS(u4PartId, u4Dest, u4Offset, u4Len)  Loader_ReadNandFlash(u4PartId, u4Offset, (void *)(u4Dest), u4Len)
#elif defined(CC_EMMC_BOOT)
#define CHIP_FlashCopyToDRAMLZHS(u4PartId, u4Dest, u4Offset, u4Len)  Loader_ReadMsdc(u4PartId, u4Offset, (void *)(u4Dest), u4Len)
#else
#define CHIP_FlashCopyToDRAMLZHS(u4PartId, u4Dest, u4Offset, u4Len)
#endif

static UINT32 _u4DualOffset = 0;
#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT)
static UINT32 _u415MBImageAddr = 0;
#define MAX_COMPRESS_IMAGE_SIZE 0x1000000
#endif

#ifdef CC_LOAD_UBOOT
#define MAX_PARTNUM         32

#define uswap_32(x) \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24))

INT32 Loader_GetPartIDByName(char *szPartName, UINT32  *pu4PartId)
{
    UINT32 u4PartId;
    char *szName;

    for (u4PartId = 0; u4PartId < MAX_PARTNUM; u4PartId++)
    {
        szName = (char *)DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartName0 + u4PartId));
        if (strcmp(szPartName, szName) == 0)
        {
            *pu4PartId = u4PartId;
            return 0;
        }
    }
    return -1;
}
#endif
#if defined(CC_SECURESTORAGE_SUPPORT)
#define REG_EFUSE_KEY       (0x688)
#define REG_EFUSE_SDATA_0   (0x694)
#define REG_EFUSE_SDATA_1   (0x698)
#define REG_EFUSE_SDATA_2   (0x69c)
#define REG_EFUSE_SDATA_3   (0x6a0)

#define LOADER_SECURESTORAGE_OFFSET 				(0x100000) /* 1024 KB */
//UINT32 DataInFlash[2048] = { 0x11111111 ,0x22222222,0x33333333,0x44444444,0x55555555};
typedef struct {
UINT8 Header[4];
UINT8 Aes_key[16];
UINT8 Aes_Iv[16];
UINT8 Hmac_sha256_key[32];
} Key_cfg ;

static Key_cfg _Ldr_Kemcfg;

BOOL LoaderAesDecrypt(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
	GCPU_LoaderInit(0);
	_GCPU_SetAesKey(_Ldr_Kemcfg.Aes_key, 128, _Ldr_Kemcfg.Aes_Iv);
	GCPU_AesDecrypt(u4InBufStart,u4OutBufStart,u4BufSize);

}
BOOL LoaderAesEncrypt(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize)
{
	GCPU_LoaderInit(0);
	_GCPU_SetAesKey(_Ldr_Kemcfg.Aes_key, 128, _Ldr_Kemcfg.Aes_Iv);
	GCPU_AesEncrypt(u4InBufStart,u4OutBufStart,u4BufSize);

}


void _LdrGenarageKey(void)
{
	hash_state t_hash_state;
	UINT32 i;

	UINT32 InputSource[256 + 4];
	UINT8 OutputSource[64];

    BIM_WRITE32(REG_EFUSE_KEY, 0x883);
    BIM_WRITE32(REG_EFUSE_KEY, 0x1404);
	x_memset(InputSource,0xFF,sizeof(InputSource));
	x_memset(OutputSource,0xFF,sizeof(OutputSource));
    InputSource[1] = BIM_READ32(REG_EFUSE_SDATA_1);
    InputSource[3] = BIM_READ32(REG_EFUSE_SDATA_3);
    InputSource[0] = BIM_READ32(REG_EFUSE_SDATA_0);
    InputSource[2] = BIM_READ32(REG_EFUSE_SDATA_2);


	x_memcpy((InputSource +4),0xf4000000,1024);
	//for(i=0;i<sizeof(InputSource);i++)
	//printf("InputSource= 0x%x,i=%d\n",InputSource[i],i);


	sha512_init(&t_hash_state);
	sha512_process(&t_hash_state,InputSource,sizeof(InputSource));
	sha512_done(&t_hash_state,OutputSource);

	//for(i=0;i<sizeof(OutputSource);i++)
	//printf("OutputSource= 0x%x,i=%d\n",OutputSource[i],i);

	_Ldr_Kemcfg.Header[0]=0x99;
	_Ldr_Kemcfg.Header[1]=0x53;
	_Ldr_Kemcfg.Header[2]=0x99;
	_Ldr_Kemcfg.Header[3]=0x53;

	x_memcpy(_Ldr_Kemcfg.Aes_key,(UINT8 *)OutputSource,16);
	x_memcpy(_Ldr_Kemcfg.Aes_Iv,((UINT8 *)OutputSource + 16),16);
	x_memcpy(_Ldr_Kemcfg.Hmac_sha256_key,((UINT8 *)OutputSource + 32),32);

	#if 0
		for(i=0;i<sizeof(_Ldr_Kemcfg.Header);i++)
		printf("_Ldr_Kemcfg.Header= 0x%x,i=%d\n",_Ldr_Kemcfg.Header[i],i);

		for(i=0;i<sizeof(_Ldr_Kemcfg.Aes_key);i++)
		printf("_Ldr_Kemcfg.Aes_key= 0x%x,i=%d\n",_Ldr_Kemcfg.Aes_key[i],i);

		for(i=0;i<sizeof(_Ldr_Kemcfg.Aes_Iv);i++)
		printf("_Ldr_Kemcfg.Aes_Iv= 0x%x,i=%d\n",_Ldr_Kemcfg.Aes_Iv[i],i);


		for(i=0;i<sizeof(_Ldr_Kemcfg.Hmac_sha256_key);i++)
		printf("_Ldr_Kemcfg.Hmac_sha256_key= 0x%x,i=%d\n",_Ldr_Kemcfg.Hmac_sha256_key[i],i);
	#endif
}
#define PAGE_SIZE (2048)
#define DIGEST_SIZE (256)

//INT32 _LdrReadFromSecureStorage(void)
INT32 _LdrReadFromSecureStorage(UINT8 Offset,UINT8 *ReadBuffer,UINT32 len)
{

	UINT32 i;

	UINT8 DataAfterDigest[32],DataBeforeDigest[DIGEST_SIZE];
	UINT8 *DataReadFromFlash = (UINT8 *)x_mem_alloc(0x20000) ;
	_LdrGenarageKey();

    CHIP_FlashCopyToDRAMLZHS(1, DataReadFromFlash, LOADER_SECURESTORAGE_OFFSET, PAGE_SIZE);
	x_memcpy(DataBeforeDigest,(DataReadFromFlash + 36 ),DIGEST_SIZE);

	mtk_hmac_sha256(_Ldr_Kemcfg.Hmac_sha256_key, sizeof(_Ldr_Kemcfg.Hmac_sha256_key), DataBeforeDigest, DIGEST_SIZE,  DataAfterDigest);

	if((*(DataReadFromFlash + 0 ))==0x99 &&
		(*(DataReadFromFlash + 1 ))==0x53 &&
		(*(DataReadFromFlash + 2 ))==0x99 &&
		(*(DataReadFromFlash + 3 ))==0x53 )
		{

		printf("header check ok\n");
		if(x_memcmp((DataReadFromFlash+4),DataAfterDigest,32) != 0)
			{


				UINT32 i;
				UINT8 DefaultValue[DIGEST_SIZE];
				for(i=0;i<DIGEST_SIZE;i++)
						DefaultValue[i] = i;
				_LdrWriteToSecureStorage(DefaultValue);

				printf("digest check failed,Reset to default value\n");

				x_mem_free(DataReadFromFlash);
				return -1;
			}

		printf("digest check ok\n");
		LoaderAesDecrypt((UINT32 *)DataBeforeDigest,(UINT32 *)DataBeforeDigest,DIGEST_SIZE);
		x_memcpy(ReadBuffer,(DataBeforeDigest + Offset),len);
		}
	else
		{

			printf("header check failed\n");
		}
	x_mem_free(DataReadFromFlash);
	printf("read done\n");
	return 0;
}
void _LdrWriteToSecureStorage(UINT8 *DataBeforeDigest)
{
	UINT32 i;
	UINT8 offset = 0,DataWriterToFlash[3000],DataAfterDigest[32];

	x_memset(DataWriterToFlash,0xFF,sizeof(DataWriterToFlash));
	_LdrGenarageKey();

	LoaderAesEncrypt((UINT32 *)DataBeforeDigest,(UINT32 *)DataBeforeDigest,DIGEST_SIZE);

	//aes encrypt

	mtk_hmac_sha256(_Ldr_Kemcfg.Hmac_sha256_key, sizeof(_Ldr_Kemcfg.Hmac_sha256_key), DataBeforeDigest, DIGEST_SIZE,  DataAfterDigest);

	x_memcpy(DataWriterToFlash,_Ldr_Kemcfg.Header,sizeof(_Ldr_Kemcfg.Header));
	offset += sizeof(_Ldr_Kemcfg.Header);
	x_memcpy((DataWriterToFlash + offset),DataAfterDigest,32);
	offset += 32;
	x_memcpy((DataWriterToFlash + offset),DataBeforeDigest,DIGEST_SIZE);
	offset += DIGEST_SIZE;


#if defined(CC_NAND_BOOT)
if (Loader_WriteNandFlash(1, LOADER_SECURESTORAGE_OFFSET, DataWriterToFlash, PAGE_SIZE) != PAGE_SIZE)
#elif defined(CC_EMMC_BOOT)
if (Loader_WriteMsdc(1, LOADER_SECURESTORAGE_OFFSET, DataWriterToFlash, PAGE_SIZE) != PAGE_SIZE)
#endif
	{
		Printf("Flash write fail");
		while(1);
	}
	printf("writer done\n");
}

//add for version control
INT32 _LdrVersionHmacCompare(UINT8 *Data_in,UINT8 *Hmac_in)
{
	UINT8 arHmacOut[32];

	_LdrGenarageKey();
	mtk_hmac_sha256(_Ldr_Kemcfg.Hmac_sha256_key, sizeof(_Ldr_Kemcfg.Hmac_sha256_key), Data_in, 64,  arHmacOut);

	if(x_memcmp(Hmac_in,arHmacOut,32) != 0)
	{
		printf("_LdrVersionHmacCompare not match!!\n");
	    return -1;
	}
	printf("compare done\n");
	return 0;
}

INT32 _LdrVersionHmacGen(UINT8 *Data_in,UINT8 *Hmac_out)
{
	_LdrGenarageKey();
	mtk_hmac_sha256(_Ldr_Kemcfg.Hmac_sha256_key, sizeof(_Ldr_Kemcfg.Hmac_sha256_key), Data_in, 64,  Hmac_out);
	printf("Hmac generation done\n");
	return 0;
}
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static INT32 _LdrSetImageBank(LDR_DATA_T *prLdrData, UINT8 u1Image)
{
    UINT32 i4Ret;

    if (u1Image==0)
    {
        prLdrData->rDtvCfg.u1Flags2 &= (UINT8)(~(DTVCFG_FLAG2_IMAGE_BANK));
    }
    else if (u1Image==1)
    {
        prLdrData->rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_IMAGE_BANK);
    }
    i4Ret = EEPDTV_SetCfg(&prLdrData->rDtvCfg);
    if (i4Ret)
    {
        printf("eeprom write failed, return %d\n", i4Ret);
    }
    return 0;
}


//-----------------------------------------------------------------------------
/** _LdrImageDecompressOther(): decompress image to dram on the offset of entry.
 *  This function is going to search the target device and find out the compressed
 *  image. Then copy to dram (if necessary) and decompress the data from dram
 *  to dram by LZHS hardware.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
static UINT32 _LdrImageDecompressOther(LDR_DATA_T *prLdrData)
{
    IMAGE_HEADER_T* prHeader;
    UINT32 u4FirmwareExecAddr = 0;
    UINT32 u4IfSecondImage = 0;

    u4IfSecondImage = prLdrData->rDtvCfg.u1Flags2 & DTVCFG_FLAG2_IMAGE_BANK;    // 0 for the first image, 1 for the second one

    Printf("\nTry the other image, DTVCFG_FLAG2_IMAGE_BANK=%d\n", u4IfSecondImage);

#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
    UINT32 u4ImageAddr;
    static BOOL _fgNorClkChange = FALSE;

    if (_fgNorClkChange == FALSE)
    {
        _fgNorClkChange = TRUE;
    }

    if (_u4DualOffset && u4IfSecondImage)
    {
        u4ImageAddr = PBI_A_BASE + _u4DualOffset;
    }
    else
    {
        u4ImageAddr = PBI_A_BASE + FIRMWARE_FLASH_OFFSET;
    }

    Printf("NOR load image from: 0x%x\n", (u4ImageAddr - PBI_A_BASE));

#else /* nand or emmc boot */

    UINT32 u4ImageAddr;

    if (_u415MBImageAddr == 0)
    {
        _u415MBImageAddr = (UINT32)x_mem_alloc(MAX_COMPRESS_IMAGE_SIZE);
    }
    u4ImageAddr = _u415MBImageAddr;
    u4ImageAddr = (u4ImageAddr & 0xfffffff0) + 0x00000010; //16-bytes alignment

    if (_u4DualOffset && u4IfSecondImage)
    {
        Printf("\nFlash load lzhs header from SysImgB(part:%d) to dram(0x%x), size=2048\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr);
        CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr, 0, (2*1024));
    }
    else
    {
        Printf("\nFlash load lzhs header from SysImgA(part:%d) to dram(0x%x), size=2048\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr);
        CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr, 0, (2*1024));
    }
#endif

    // Check if valid signature
    prHeader = (IMAGE_HEADER_T *)u4ImageAddr;
    if ((prHeader->u4Signature != IMAGE_SIGNATURE) && (prHeader->u4Signature != IMAGE_SIGNATURE_UPGRADE))
    {
        // Not a valid image
        Printf("No valid image found! 0x%08x: 0x%08x 0x%08x\n", u4ImageAddr, prHeader->u4Signature, IMAGE_SIGNATURE);
        return 1;
    }

    UINT32 u4ImgaeHeaderSize = (prHeader->u4Signature == IMAGE_SIGNATURE_UPGRADE) ? sizeof(IMAGE_UPGRADE_HEADER_T) : sizeof (IMAGE_HEADER_T);
    u4ImageAddr += u4ImgaeHeaderSize;

    if(prHeader->fgCompress == COMPRESS_TYPE_LZHS)
    {
        UINT32 u4DstSize;
        UINT8 u1Checksum;

        prLdrData->rLdrHdr.u4FirmwareExecAddr = u4FirmwareExecAddr = prHeader->u4FirmwareExecAddr;

        u4DstSize = (*((UINT32 *)u4ImageAddr));
        u1Checksum = (*((UINT8 *)(u4ImageAddr + 8)));

#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) /* nand or emmc boot */
        UINT32 u4ImageLzhsSize = (*((UINT32 *)(u4ImageAddr + 4))) + 0x10 + u4ImgaeHeaderSize;

    #if defined(CC_SECURE_BOOT_ALL)
        u4ImageLzhsSize += 768;
    #endif
        if(u4ImageLzhsSize > MAX_COMPRESS_IMAGE_SIZE)
		{
		    Printf("alloc Compress Image buffer 0x%x is too small than Image lzhs 0x%x \n", MAX_COMPRESS_IMAGE_SIZE, u4ImageLzhsSize);
		    ASSERT(0);
		}

        if(_u4DualOffset && u4IfSecondImage)
        {
            Printf("\nFlash load image from SysImgB(part:%d) to dram(0x%x), size=0x%x\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgB),
                u4ImageAddr - u4ImgaeHeaderSize, u4ImageLzhsSize);
            CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr - u4ImgaeHeaderSize, 0, u4ImageLzhsSize);
        }
        else
        {
            Printf("\nFlash load image from SysImgA(part:%d) to dram(0x%x), size=0x%x\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgA),
                u4ImageAddr - u4ImgaeHeaderSize, u4ImageLzhsSize);
            CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr - u4ImgaeHeaderSize, 0, u4ImageLzhsSize);
        }
#endif

      #ifdef CC_LZMA_HW_DECODE
		UINT32 u4SrcSize;
		u4SrcSize =(*((UINT32 *)(u4ImageAddr +4)));
		if(LZMA_Dec(u4FirmwareExecAddr, u4DstSize, u4ImageAddr + 0x10, u4SrcSize ,&u1Checksum) != 0)
	  #else
        if(LZHS_Dec(u4ImageAddr + 0x10, u4FirmwareExecAddr, u4DstSize, &u1Checksum) != 0)
	  #endif
        {
            Printf("Decompress  fail!\n");
            return 1;
        }
    }
    else
    {
        #if 0
        // Copy the image to destination address
        Printf("Copying image...\n");
        x_memcpy((void *)u4FirmwareExecAddr, (void *)u4ImageAddr, prHeader->u4Size);
        #endif
    }

    return 0;
}
#ifdef SIGGEN_KEY_LOADER
#include "public_key.h"
#else
#define test
#ifdef test
#define RSA_SIZE 280
#ifdef PRODUCTION
static UINT8 au1RSAPublicKey[RSA_SIZE]=
{
	0xa7, 0xf9, 0xe1, 0xf7, 0x14, 0x01, 0xca, 0x9b, 0x62, 0xdb, 0xa3, 0x68, 0xee, 0xa9, 0x67, 0x62,
	0xa3, 0x87, 0xf8, 0x36, 0x4d, 0xca, 0x55, 0x4c, 0xdf, 0x79, 0x32, 0x24, 0x79, 0x33, 0x73, 0xfe,
	0x7d, 0x51, 0x6b, 0x6d, 0xac, 0xd2, 0xa4, 0x25, 0xee, 0x15, 0xca, 0x38, 0xf2, 0x46, 0x56, 0xe4,
	0x55, 0x27, 0xf1, 0xee, 0x36, 0xac, 0x90, 0x1d, 0xd4, 0xf0, 0x58, 0xb1, 0x2e, 0x97, 0x5d, 0xaf,
	0x7b, 0xdc, 0x29, 0xc4, 0x2f, 0x64, 0xa6, 0xc3, 0x0b, 0xc9, 0x5c, 0xc3, 0xaf, 0xfe, 0xe3, 0x00,
	0x86, 0xb3, 0x26, 0xf6, 0x45, 0x1c, 0xcf, 0x9e, 0x51, 0xe7, 0x82, 0xe5, 0x6a, 0x52, 0xcb, 0xe9,
	0x60, 0x57, 0x0d, 0x21, 0x0f, 0x35, 0x9c, 0xf9, 0x23, 0x92, 0xf9, 0x77, 0x77, 0x4c, 0x0f, 0xd4,
	0xf6, 0xb7, 0xb0, 0x03, 0xf3, 0xf1, 0x2b, 0xc5, 0x8b, 0x29, 0xfa, 0x22, 0x6c, 0xf9, 0x20, 0x09,
	0x96, 0x5b, 0xc4, 0x7b, 0xdc, 0xc6, 0xd7, 0x4b, 0x61, 0x11, 0x39, 0x3b, 0x01, 0x60, 0x7c, 0xf1,
	0x73, 0xee, 0xe5, 0x12, 0x34, 0xef, 0x48, 0xb9, 0x28, 0xe5, 0x8e, 0xee, 0xbb, 0x97, 0x40, 0x93,
	0x3a, 0x03, 0x5a, 0x44, 0xcc, 0x89, 0xcc, 0x03, 0x46, 0x69, 0xcb, 0xd4, 0x20, 0x25, 0xcd, 0x61,
	0xa5, 0x74, 0x52, 0xe0, 0x8c, 0x53, 0x72, 0x22, 0x95, 0x10, 0x59, 0xe2, 0x92, 0x1f, 0xc9, 0x64,
	0xfc, 0x8a, 0xf7, 0x56, 0x56, 0x80, 0x5e, 0x3b, 0xc3, 0xf6, 0x43, 0x33, 0x9e, 0x48, 0x3d, 0x66,
	0xf4, 0xf3, 0x77, 0xc1, 0x80, 0x94, 0xf7, 0xbd, 0x5b, 0x33, 0x57, 0x0d, 0x11, 0x31, 0xac, 0x18,
	0x1e, 0x5c, 0x04, 0x88, 0x7b, 0x28, 0x4a, 0x72, 0x40, 0x52, 0x5f, 0x2c, 0x3b, 0xde, 0xbb, 0x8d,
	0x6e, 0xfd, 0x7f, 0x7a, 0x4e, 0x63, 0x7b, 0xab, 0xdc, 0xdf, 0x3d, 0x5d, 0x7e, 0x9c, 0x82, 0xac


};

#else
static UINT8 au1RSAPublicKey[RSA_SIZE]=
{
    0x99, 0xc4, 0xcd, 0x15, 0xaf, 0x54, 0xe0, 0xf6, 0x0f, 0x70, 0x25, 0x12, 
    0xfd, 0x30, 0x83, 0xb6, 0x0e, 0x00, 0x09, 0x91, 0xa2, 0x78, 0xd9, 0x54, 
    0x01, 0xae, 0xb1, 0xbd, 0xb1, 0x4d, 0x8e, 0x06, 0x57, 0x64, 0x64, 0x38, 
    0x85, 0x04, 0xe9, 0x65, 0x8e, 0x1a, 0x64, 0xca, 0xf5, 0xa1, 0x14, 0x85, 
    0x51, 0x6c, 0x5f, 0xbc, 0xe2, 0x23, 0xbb, 0x99, 0xe1, 0xd9, 0x30, 0x3d, 
    0x05, 0x71, 0x06, 0x7a, 0xe4, 0xbc, 0x51, 0x25, 0x23, 0xf8, 0x52, 0x9f, 
    0x31, 0xae, 0x24, 0x0e, 0x68, 0x4c, 0x4c, 0x5e, 0x22, 0x7f, 0x31, 0x28, 
    0x73, 0x2b, 0x93, 0x8b, 0x0a, 0x40, 0x15, 0xe5, 0x9f, 0xa5, 0xbd, 0xe7, 
    0xfb, 0xb0, 0x53, 0xf5, 0xcd, 0x8b, 0x8c, 0x9e, 0x21, 0x6a, 0xdb, 0xd3, 
    0x4d, 0xca, 0xb2, 0xb6, 0x60, 0xab, 0xaf, 0xb9, 0x3f, 0xec, 0x5a, 0x85, 
    0xd2, 0x69, 0x40, 0xda, 0x04, 0xa2, 0xd2, 0x56, 0x91, 0x62, 0x72, 0x1d, 
    0xd6, 0x4c, 0xe2, 0x1d, 0x8d, 0x26, 0xc2, 0xaf, 0x77, 0x78, 0xad, 0x6c, 
    0x14, 0xe1, 0xde, 0xa0, 0x6d, 0xdf, 0xc4, 0x18, 0x8d, 0x19, 0xb5, 0xe3, 
    0x1a, 0x5e, 0xda, 0x37, 0xfd, 0x85, 0x10, 0x7a, 0xca, 0xc2, 0x07, 0x99, 
    0x3b, 0x13, 0x89, 0x02, 0xd5, 0x3a, 0xfd, 0x18, 0xf9, 0x9e, 0x0b, 0xfd, 
    0xf9, 0x2d, 0xa3, 0x83, 0xdc, 0x63, 0x71, 0x27, 0x19, 0x66, 0xd7, 0xf1, 
    0xf3, 0x60, 0x4f, 0x5d, 0xa9, 0xe9, 0x63, 0x79, 0x72, 0x3d, 0x03, 0x5e, 
    0x6a, 0x2e, 0x09, 0xe6, 0x22, 0xd8, 0x42, 0x09, 0xa8, 0x66, 0x04, 0x5c, 
    0xea, 0x0e, 0xfa, 0x56, 0x5a, 0xd7, 0xfc, 0x28, 0x91, 0x6c, 0x55, 0xc1, 
    0x73, 0x3a, 0x10, 0x31, 0x57, 0x0b, 0x6e, 0x71, 0xd8, 0xbb, 0xc4, 0x04, 
    0x7e, 0xc7, 0x68, 0xb2, 0xa4, 0xc6, 0x54, 0x71, 0x83, 0xd7, 0x73, 0xfc, 
    0x6a, 0x91, 0xbc, 0xc2

};
#endif
#endif
#endif


//-----------------------------------------------------------------------------
/** LDR_ImageDecompress(): decompress image to dram on the offset of entry.
 *  This function is going to search the target device and find out the compressed
 *  image. Then copy to dram (if necessary) and decompress the data from dram
 *  to dram by LZHS hardware.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
#include "x_dram.h"
UINT32 LDR_ImageDecompress(LDR_DATA_T *prLdrData)
{
    IMAGE_HEADER_T* prHeader;
    UINT32 u4FirmwareExecAddr = 0;
    UINT32 u4IfSecondImage = 0;
#ifdef CC_L_DUAL_BOOT
		struct partition_info *pPart = 0;
#endif

    _u4DualOffset = DRVCUST_InitGet(eLoaderDualBootOffset);
    u4IfSecondImage = prLdrData->rDtvCfg.u1Flags2 & DTVCFG_FLAG2_IMAGE_BANK;    // 0 for the first image, 1 for the second one
#ifdef CC_SECURE_BOOT_SCRAMBLE
if (BIM_IS_SECURE_BOOT)
	{	
		LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
				// get vendor public key
#ifdef CC_SECURE_BOOT_SCRAMBLE
				_GCPU_SetAesKey(prLdrEnv->aes_key2, KEY_BIT_LEN_128, prLdrEnv->aes_iv2);
#endif // CC_SECURE_BOOT_SCRAMBLE
			
	
	}
#endif
// 1. read lzhs header
#if !defined(CC_NAND_BOOT) && !defined(CC_EMMC_BOOT) // nor boot
    UINT32 u4ImageAddr;

    if (_u4DualOffset && u4IfSecondImage)
    {
        u4ImageAddr = PBI_A_BASE + _u4DualOffset;
    }
    else
    {
        u4ImageAddr = PBI_A_BASE + FIRMWARE_FLASH_OFFSET;
    }

    Printf("\nNOR load lzhs header from:0x%x\n", (u4ImageAddr - PBI_A_BASE));

#else /* nand or emmc boot */

    UINT32 u4ImageAddr;

    if (_u415MBImageAddr == 0)
    {
        _u415MBImageAddr = (UINT32)x_mem_alloc(MAX_COMPRESS_IMAGE_SIZE);
    }
    u4ImageAddr = _u415MBImageAddr;
    u4ImageAddr = (u4ImageAddr & 0xfffffff0) + 0x00000010; //16-bytes alignment

#ifdef CC_LOAD_UBOOT
#ifdef CC_L_DUAL_BOOT
		if (0 != init_partinfo())
		{
			Printf ("Error: init partinfo fail\n");
		}
	
		pPart = get_used_partition("boot");
		Printf("\nDualBoot Flash load lzhs header from 0x%x to dram(0x%x), size=2048\n", (UINT32)(pPart->offset & 0xffffffff), u4ImageAddr);
		CHIP_FlashCopyToDRAMLZHS(0, u4ImageAddr, (UINT32)(pPart->offset & 0xffffffff), (2*1024));
#else
		Printf("\nFlash load lzhs header from 0x%x to dram(0x%x), size=2048\n", LOADER_MAX_SIZE, u4ImageAddr);
		CHIP_FlashCopyToDRAMLZHS(0, u4ImageAddr, LOADER_MAX_SIZE, (2*1024));
#endif

#else /* CC_LOAD_UBOOT */

    if (_u4DualOffset && u4IfSecondImage)
    {
        Printf("\nFlash load lzhs header from SysImgB(part:%d) to dram(0x%x), size=2048\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr);
        CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr, 0, (2*1024));
    }
    else
    {
        Printf("\nFlash load lzhs header from SysImgA(part:%d) to dram(0x%x), size=2048\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr);
        CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr, 0, (2*1024));
    }
#endif /* CC_LOAD_UBOOT */
    #ifdef CC_SECURE_BOOT_SCRAMBLE
        GCPU_LoaderInit(0);
        GCPU_AesDecrypt(u4ImageAddr, u4ImageAddr, (2*1024));
    #endif // CC_SECURE_BOOT_SCRAMBLE
#endif //nor boot

// 2. read uboot/sys compressed image for lzds decoding
#ifdef CC_LOAD_UBOOT
    UINT32 u4DstSize, u4DstAddr;
    UINT8 u1Checksum;
    BOOL fgIsLzhs = TRUE;
    PFN_IMAGE pfnImage;

    UNUSED(prHeader);
    UNUSED(_LdrSetImageBank);
    UNUSED(_LdrImageDecompressOther);
    UNUSED(u4IfSecondImage);

	#ifndef CC_BUILTIN		//zanyun.wang add, uboot image compressed address need to rearward movement with the builtin kernel size increase
	//prLdrData->rLdrHdr.u4FirmwareExecAddr = u4FirmwareExecAddr = 0x1000000;
	prLdrData->rLdrHdr.u4FirmwareExecAddr = u4FirmwareExecAddr = 0x2DA00000;
	#else
    prLdrData->rLdrHdr.u4FirmwareExecAddr = u4FirmwareExecAddr = 0x3000000;
    #endif
    Printf("Decompression uboot to 0x%08x...\n", u4FirmwareExecAddr);

    u4DstSize = (*((UINT32 *)u4ImageAddr));
    u1Checksum = (*((UINT8 *)(u4ImageAddr + 8)));

    if(u4DstSize == *((UINT32 *)(u4ImageAddr + 4)))
    {
        fgIsLzhs = FALSE;
        u4DstAddr = u4FirmwareExecAddr - 0x10;
    }
    else
    {
        fgIsLzhs = TRUE;
        u4DstAddr = u4ImageAddr;
    }
#if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) /* nand or emmc boot */
    UINT32 u4ImageLzhsSize;
    u4ImageLzhsSize = (*((UINT32 *)(u4ImageAddr + 4))) + 0x10;

    #if defined(CC_SECURE_BOOT_ALL)
    u4ImageLzhsSize += 0x300;
    #endif
#ifdef CC_L_DUAL_BOOT
		Printf("\nDualBoot Flash load image from 0x%x to dram(0x%x), size=0x%x\n", (UINT32)(pPart->offset & 0xffffffff), u4DstAddr, u4ImageLzhsSize);
		CHIP_FlashCopyToDRAMLZHS(0, u4DstAddr, (UINT32)(pPart->offset & 0xffffffff), u4ImageLzhsSize);
#else
		Printf("\nFlash load image from 0x%x to dram(0x%x), size=0x%x\n", LOADER_MAX_SIZE, u4ImageAddr, u4ImageLzhsSize);
		CHIP_FlashCopyToDRAMLZHS(0, u4DstAddr, LOADER_MAX_SIZE, u4ImageLzhsSize);
#endif

    #ifdef CC_SECURE_BOOT_SCRAMBLE
        GCPU_LoaderInit(0);
        GCPU_AesDecrypt(u4DstAddr, u4DstAddr, u4ImageLzhsSize & ~(0xf)); // must be 16 bytes alignment
    #endif // CC_SECURE_BOOT_SCRAMBLE
#if defined(CC_TRUSTZONE_SUPPORT) && !defined(CC_NOT_LOAD_TRUSTZONE_AT_MTKLOADER)
    do
    {
        DTVCFG_T rDtvCfg;
        UINT8 u1TzChksum;
        INT32 i4TzPartID;
        char *szKernelName;
        UINT32 u4KernelID, u4TzOffset, u4TzLzhsDstDram, u4TzLzhsSrcDram, u4TzPlainSize, u4TzLzhsSize;

#if defined(CC_TRUSTZONE_IN_CHB)
		u4TzLzhsSrcDram = TOTAL_MEM_SIZE - TRUSTZONE_MEM_SIZE - 0x200000;
		u4TzLzhsDstDram = TCMGET_CHANNELA_SIZE()*0x100000+TCMGET_CHANNELB_SIZE()*0x100000-TRUSTZONE_MEM_SIZE+TRUSTZONE_CODE_BASE;
#else
		u4TzLzhsSrcDram = TOTAL_MEM_SIZE - TRUSTZONE_MEM_SIZE - 0x200000;
        u4TzLzhsDstDram = TOTAL_MEM_SIZE - TRUSTZONE_MEM_SIZE + TRUSTZONE_CODE_BASE;
#endif
		
        if((i4TzPartID = DRVCUST_InitGet(eTzFlashPartition)) != -1)
        {
            //int i;

            u4KernelID = i4TzPartID;
            u4TzOffset = 0;
            //copy first 4k first to check tz lzhs header
            CHIP_FlashCopyToDRAMLZHS(u4KernelID, u4TzLzhsSrcDram, u4TzOffset, 0x1000);

            u4TzPlainSize = *((UINT32 *)u4TzLzhsSrcDram);
            u4TzLzhsSize = *((UINT32 *)(u4TzLzhsSrcDram+4));
            u1TzChksum = *((UINT8 *)(u4TzLzhsSrcDram+8));
        }
        else
        {
            EEPDTV_GetCfg(&rDtvCfg);
            if ((rDtvCfg.u1Flags3 & DTVCFG_FLAG3_KERNEL_BANK) == DTVCFG_FLAG3_KERNEL_BANK)
            {
                #ifdef CC_TWO_WORLD_RFS
				szKernelName = "boot";
				#else
                szKernelName = "kernelB";
				#endif
            }
            else
            {
                #ifdef CC_TWO_WORLD_RFS
				szKernelName = "boot";
				#else
                szKernelName = "kernelA";
				#endif
            }

            if (Loader_GetPartIDByName(szKernelName, &u4KernelID))
            {
                Printf("Cannot Find Kernel partition for trustzone binary!\n");
                while (1);
            }
			
            //copy first 2k to read kernel header
            CHIP_FlashCopyToDRAMLZHS(u4KernelID, u4TzLzhsSrcDram, 0, 0x800);

            // tz_offset=kernel_header(64)+kernel_size+kernel_signature(512)
#if defined(CC_SECURE_BOOT_ALL)
            #if defined(CC_SECURE_BOOT_V2)
            u4TzOffset = ((uswap_32(*((UINT32 *)(u4TzLzhsSrcDram+12))) + 0x40 + 0x100 + 0x10) & ~0xf);
            #else
            u4TzOffset = uswap_32(*((UINT32 *)(u4TzLzhsSrcDram+12))) + 0x40 + 0x200;
            #endif
#else
            u4TzOffset = uswap_32(*((UINT32 *)(u4TzLzhsSrcDram+12))) + 0x40;
#endif // CC_SECURE_BOOT_ALL

            //copy first 4k first to check tz lzhs header
            CHIP_FlashCopyToDRAMLZHS(u4KernelID, u4TzLzhsSrcDram, u4TzOffset, 0x1000);
            #ifdef CC_SECURE_BOOT_SCRAMBLE
                GCPU_LoaderInit(0);
                GCPU_AesDecrypt(u4TzLzhsSrcDram, u4TzLzhsSrcDram, 0x1000);
            #endif // CC_SECURE_BOOT_SCRAMBLE
            u4TzPlainSize = *((UINT32 *)u4TzLzhsSrcDram);
            u4TzLzhsSize = *((UINT32 *)(u4TzLzhsSrcDram+4));
            u1TzChksum = *((UINT8 *)(u4TzLzhsSrcDram+8));
        }

        Printf("\nFlash load tz from 0x%x(part_%d) to dram(0x%x), size=0x%x\n", u4TzOffset, u4KernelID, u4TzLzhsSrcDram, u4TzLzhsSize);

        //copy rest tz data
        CHIP_FlashCopyToDRAMLZHS(u4KernelID, u4TzLzhsSrcDram, u4TzOffset, u4TzLzhsSize+0x1000);
        #ifdef CC_SECURE_BOOT_SCRAMBLE
            GCPU_LoaderInit(0);
            GCPU_AesDecrypt(u4TzLzhsSrcDram, u4TzLzhsSrcDram, u4TzLzhsSize+0x1000);
        #endif // CC_SECURE_BOOT_SCRAMBLE
        #if defined(CC_SECURE_BOOT_V2) && defined(CC_SECURE_BOOT_ALL)
            if (BIM_IS_SECURE_BOOT)
            {
                BIM_VerifyImgSig(u4TzLzhsSrcDram);
            }
        #endif  // defined(CC_SECURE_BOOT_V2) && defined(CC_SECURE_BOOT_ALL)

        if(fgIsLzhs)
        {
            if(LZHS_Dec(u4TzLzhsSrcDram + 0x10, u4TzLzhsDstDram, u4TzPlainSize, &u1TzChksum) != 0)
            {
                Printf("TZ LZHS decode fail! src=0x%x,dst=0x%x,psize=0x%x,lsize=0x%x,chk=0x%x\n",
                    u4TzLzhsSrcDram + 0x10, u4TzLzhsDstDram, u4TzPlainSize, u4TzLzhsSize, u1TzChksum);
                return 1;
            }
        }
        else
        {
            x_memcpy(u4TzLzhsDstDram, u4TzLzhsSrcDram + 0x10, u4TzPlainSize);
        }
    } while (0);
#endif // CC_TRUSTZONE_SUPPORT
#endif // defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT)
#ifdef test
	do
		{
			
			CHAR LoadVesion[8] ={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
			LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
			x_memcpy((void*)prLdrEnv->au4CustKey, (void*)au1RSAPublicKey, 256);
			x_memcpy((void*)prLdrEnv->LoadVesion, (void*)LoadVesion, 8);

		}while(0);
#else
do
	{
		
		CHAR LoadVesion[8] ={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
		LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
		x_memcpy((void*)prLdrEnv->au4CustKey, (void*)customer_pub_mtka5lr_key, 256);
		x_memcpy((void*)prLdrEnv->LoadVesion, (void*)LoadVesion, 8);
	}while(0);

#endif

    #if defined(CC_SECURE_BOOT_V2) && defined(CC_SECURE_BOOT_ALL)
        if (BIM_IS_SECURE_BOOT)
        {
            BIM_VerifyImgSig(u4DstAddr);
        }
    #endif  // defined(CC_SECURE_BOOT_V2) && defined(CC_SECURE_BOOT_ALL)
    if(!fgIsLzhs)
    {
        Printf("plaintext uboot\n");
        pfnImage = (PFN_IMAGE)(u4DstAddr + 0x10);
        pfnImage();
    }
    else
    {
        #ifdef CC_LZMA_HW_DECODE
    	 UINT32 u4SrcSize;
    	 u4SrcSize =(*((UINT32 *)u4ImageAddr+4));
    	if(LZMA_Dec(u4FirmwareExecAddr, u4DstSize, u4ImageAddr + 0x10, u4SrcSize ,&u1Checksum) != 0)
    	#else /* CC_LZMA_HW_DECODE  */
        if(LZHS_Dec(u4ImageAddr + 0x10, u4FirmwareExecAddr, u4DstSize, &u1Checksum) != 0)
    	#endif /* CC_LZMA_HW_DECODE  */
        {
            Printf("decompress decode fail!\n");
            return 1;
        }
    }


#else // CC_LOAD_UBOOT (nucleus)

    // Check if valid signature
    prHeader = (IMAGE_HEADER_T *)u4ImageAddr;
    if ((prHeader->u4Signature != IMAGE_SIGNATURE) && (prHeader->u4Signature != IMAGE_SIGNATURE_UPGRADE))
    {
        // Not a valid image
        Printf("No valid image found! 0x%08x: 0x%08x 0x%08x\n", u4ImageAddr, prHeader->u4Signature, IMAGE_SIGNATURE);

        if (_u4DualOffset)
        {
            _LdrSetImageBank(prLdrData, !u4IfSecondImage);
            return _LdrImageDecompressOther(prLdrData);
        }
        else
        {
            return 1;
        }
    }

    UINT32 u4ImgaeHeaderSize = (prHeader->u4Signature == IMAGE_SIGNATURE_UPGRADE) ? sizeof(IMAGE_UPGRADE_HEADER_T) : sizeof (IMAGE_HEADER_T);
    u4ImageAddr += u4ImgaeHeaderSize;
    #ifdef CC_LZMA_DECODE
    UINT32 u4DstSize, u4DstAddr, u4SrcSize, u4SrcAddr;
    LZMA_FILE_HEADER* prLzmaHdr = (LZMA_FILE_HEADER *)u4ImageAddr;

    u4DstSize = prLzmaHdr->org_size;
    u4DstAddr = prHeader->u4FirmwareExecAddr;
    u4SrcAddr = u4ImageAddr + sizeof(LZMA_FILE_HEADER);
    u4SrcSize = prLzmaHdr->compressed_size;
    prLdrData->rLdrHdr.u4FirmwareExecAddr = prHeader->u4FirmwareExecAddr;
    UNUSED(u4FirmwareExecAddr);

    Printf("LZMA decode, src{addr=0x%08x, len=%d(0x%08x)}, dst{addr=0x%08x, len=%d(0x%08x)}\n",
        u4SrcAddr, u4SrcSize, u4SrcSize, u4DstAddr, u4DstSize, u4DstSize);

    UINT32 u4Ret = (UINT32)LzmaUncompress((unsigned char *)u4DstAddr, (size_t *)&u4DstSize,
        (unsigned char *)u4SrcAddr, (size_t *)&u4SrcSize,
        (unsigned char *)&prLzmaHdr->props, sizeof(CLzmaEncProps));

    if (u4Ret != SZ_OK)
    {
        Printf("LZMA fail, code=%d\n", u4Ret);
        return 1;
    }
    #else
    if(prHeader->fgCompress == COMPRESS_TYPE_LZHS)
    {
        UINT32 u4DstSize;
        UINT8 u1Checksum;

        prLdrData->rLdrHdr.u4FirmwareExecAddr = u4FirmwareExecAddr = prHeader->u4FirmwareExecAddr;
        Printf("Decompression image to 0x%08x...\n", u4FirmwareExecAddr);

        u4DstSize = (*((UINT32 *)u4ImageAddr));
        u1Checksum = (*((UINT8 *)(u4ImageAddr + 8)));

    #if defined(CC_NAND_BOOT) || defined(CC_EMMC_BOOT) /* nand or emmc boot */
        UINT32 u4ImageLzhsSize = (*((UINT32 *)(u4ImageAddr + 4))) + 0x10 + u4ImgaeHeaderSize;

        #if defined(CC_SECURE_BOOT_ALL)
        u4ImageLzhsSize += 768;
        #endif
		if(u4ImageLzhsSize > MAX_COMPRESS_IMAGE_SIZE)
		{
    		Printf("alloc Compress Image buffer 0x%x is too small than Image lzhs 0x%x \n", MAX_COMPRESS_IMAGE_SIZE, u4ImageLzhsSize);
    		ASSERT(0);
		}

        if(_u4DualOffset && u4IfSecondImage)
        {
            Printf("\nFlash load image from SysImgB(part:%d) to dram(0x%x), size=0x%x\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgB),
                u4ImageAddr - u4ImgaeHeaderSize, u4ImageLzhsSize);
            CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgB), u4ImageAddr - u4ImgaeHeaderSize, 0, u4ImageLzhsSize);
        }
        else
        {
            Printf("\nFlash load image from SysImgA(part:%d) to dram(0x%x), size=0x%x\n", DRVCUST_InitGet(eNANDFlashPartIdSysImgA),
                u4ImageAddr - u4ImgaeHeaderSize, u4ImageLzhsSize);
            CHIP_FlashCopyToDRAMLZHS(DRVCUST_InitGet(eNANDFlashPartIdSysImgA), u4ImageAddr - u4ImgaeHeaderSize, 0, u4ImageLzhsSize);
        }
    #endif /* nand or emmc boot */
      #ifdef CC_LZMA_HW_DECODE
	     UINT32 u4SrcSize;
	     u4SrcSize =(*((UINT32 *)(u4ImageAddr +4)));
	    if(LZMA_Dec(u4FirmwareExecAddr, u4DstSize, u4ImageAddr + 0x10, u4SrcSize ,&u1Checksum) != 0)
	  #else
        if(LZHS_Dec(u4ImageAddr + 0x10, u4FirmwareExecAddr, u4DstSize, &u1Checksum) != 0)
	  #endif
        {
            Printf("Decompress decode fail!\n");
            if (_u4DualOffset)
            {
                _LdrSetImageBank(prLdrData, !u4IfSecondImage);
                return _LdrImageDecompressOther(prLdrData);
            }
            else
            {
                return 1;
            }
        }
    }
    else
    {
        #if 0
        // Copy the image to destination address
        Printf("Copying image...\n");
        x_memcpy((void *)u4FirmwareExecAddr, (void *)u4ImageAddr, prHeader->u4Size);
        #endif
    }
#endif
#endif // nucleus

    return 0;
}

