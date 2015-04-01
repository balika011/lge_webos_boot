/*
* Copyright (c) MediaTek Inc.
*
* This program is distributed under a dual license of GPL v2.0 and
* MediaTek proprietary license. You may at your option receive a license
* to this program under either the terms of the GNU General Public
* License (GPL) or MediaTek proprietary license, explained in the note
* below.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*
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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>

typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef int INT32;
typedef short INT16;
typedef char UINT8;

#include "x_dlm.h"
#include "dlm.h"
#include "loader_imghdr.h"

// #define BUFFER_SIZE                     0x00100000  /* 1MB */
#define BUFFER_SIZE                     0x00010000  /* 64KB */
#define LOADER_BUFFER_SIZE              0x00100000  /* 1MB */
#define IFRAME_OFFSET                   0x002C0000
#define SZ_IFRAME_FILE                  "iframe.dat"
#define PHOTO_OFFSET                    0x00560000
#define SZ_PHOTO_BIN                    "photo.bin"
#define FLASHDATA_OFFSET                0x00760000
#define SZ_FLASHDATA_BIN                "data/flash_data.bin"

#if 0
typedef struct
{
     UINT32 u4Signature;
     UINT32 u4Size;
     UINT32 fgCompress;
     UINT32 u4Reserved;
} IMAGE_HEADER_T;
#endif

void fill_hdr(UINT8 *pHdr, UINT8 *szCustomer, UINT8 *szModel, UINT8 *szVersion)
{
    // fill as 0.
    memset(pHdr, 0, 128);

    // ID
    if (strlen(szCustomer)>4)
    {
        memcpy((void *)&pHdr[0], szCustomer, 4);
    }
    else
    {
        strcpy((UINT8 *)&pHdr[0], szCustomer);
    }

    // magic
    memcpy((void *)&pHdr[16], DLM_HDR_MAGIC, 8);

    // model
    strcpy((UINT8 *)&pHdr[32], szModel);
    
    // Version
    strcpy((UINT8 *)&pHdr[64], szVersion);
}

static char _achBuf[LOADER_BUFFER_SIZE];

UINT32 read_image_chksum(int fddiag, UINT32 u4Size)
{
    int i4_ret;
    UINT32 *pu4Val;
    UINT32 u4Chksum, u4Len;

    printf("fddiag size:%d\n", u4Size);
    u4Chksum = 0;
    u4Len = 0;
    u4Size = ((u4Size + 3) & 0xfffffffcU);
    do {
        memset(_achBuf, 0xff, BUFFER_SIZE);
        i4_ret = read(fddiag, _achBuf, BUFFER_SIZE);
        for (pu4Val = (UINT32 *)_achBuf; (((UINT32)pu4Val - (UINT32)_achBuf) < i4_ret); pu4Val++)
        {
            u4Chksum += *pu4Val;
            u4Len += 4;
        }
    } while (i4_ret > 0);
    if (u4Len != u4Size)
    {
        exit(1);
    }
    printf("fddiag size:%d u4Len:%d chksum:0x%08x\n", u4Size, u4Len, u4Chksum);
    return u4Chksum;
}

typedef struct
{
    UINT8 *szLoader;
    UINT8 *szApp;
    UINT8 *szTarget;
    INT32 i4CompressType;
    UINT8 *szCustomer;
    UINT8 *szModel;
    UINT8 *szVersion;
    UINT32 u4LoaderMaxSize;
    INT32 fgUpgVer;
    INT32 fgCompactLdrSize;
} COMMAND_ARG_T;

#ifdef BIN_8280_ADDR
#define BIN_8280_PATH "8280BinPath"

void get_8280bin_path(char *sz8280Bin) 
{
    // TODO: Only for test. Need to get the real bin file path from Makefile or script file.
    //try to find the bin name from script file
    int fdscript;
    int i4_ret;
    UINT8 szBinName[128];
    
    fdscript = open(BIN_8280_PATH, O_RDONLY);
    if ((fdscript < 0))
    {
        printf("Use default path of 8280.bin\n");
        sprintf(sz8280Bin, "8280.bin");             //The default  bin name
    } 
    else
    {
        i4_ret = read(fdscript, sz8280Bin, 128);
        sz8280Bin[i4_ret-1] = '\0';
        close(fdscript);
        printf("get 8280 bin path: %s\n", sz8280Bin);
    }
}

UINT32 get_8280bin_address() 
{
    // Need to get this address from Makefile.
    return BIN_8280_ADDR;
}
#endif

//-----------------------------------------------------------------------------
/** Usage: Show the command usage
  *  Details of ParseCommandArg (optional).
  *  @retval void
  */
//-----------------------------------------------------------------------------
void Usage(int argc, char **argv) 
{
        printf("%s [loader] [app-program] [target] [compress] [[customer] [model] [version] [loader size] [[upg_ver]]]\n", argv[0]);
        printf("\t %s will find file:%s and put it at offset: 0x%08x.\n",
            argv[0], SZ_IFRAME_FILE, IFRAME_OFFSET);
}

//-----------------------------------------------------------------------------
/** ParseCommandArg parse command arguments and fills out COMMAND_ARG_T struct.
  *  Details of ParseCommandArg (optional).
  *  @retval 1	Success
  *  @retval 0	Fail 
  */
//-----------------------------------------------------------------------------
int ParseCommandArgs(int argc, char **argv, COMMAND_ARG_T *rpCMDARG) 
{
    int i4Tmp;

    memset(rpCMDARG, 0, sizeof(COMMAND_ARG_T));

    // 1. Check if the number of arguments
    if (argc < 5) {
        Usage(argc, argv);
        return 0;
    }

    //  2. Get loader, app, and target name
    rpCMDARG->szLoader = argv[1];
    rpCMDARG->szApp = argv[2];
    rpCMDARG->szTarget = argv[3];
    
    //  3. Check the Compress type
    rpCMDARG->i4CompressType = atoi(argv[4]);
    if((rpCMDARG->i4CompressType != 0) && (rpCMDARG->i4CompressType != 1) && (rpCMDARG->i4CompressType != 2))
    {
        printf("%d is wrong compress type!\n", rpCMDARG->i4CompressType);
        return 0;
    }

    //  4. Get Customer, Model, Version
    if (argc >= 8) 
    {
        rpCMDARG->szCustomer = argv[5];
        rpCMDARG->szModel = argv[6];
        rpCMDARG->szVersion = argv[7];
    }

    //  5. Check the loader size
    rpCMDARG->u4LoaderMaxSize = LOADER_MAX_SIZE;
    if (argc >= 9)
    {
        i4Tmp = atoi(argv[8]);
        if ((i4Tmp != 0) && ((i4Tmp & 0xffff) == 0))
        {
            rpCMDARG->u4LoaderMaxSize = i4Tmp;
        }
    }

    //  6. Is Upgrade Version
	rpCMDARG->fgUpgVer = 1;
#if 0
    if ((argc >= 10) && (strncasecmp(argv[9], "upg_ver", 8)==0))
    {
        rpCMDARG->fgUpgVer = 1;
    }
    else 
    {
        rpCMDARG->fgUpgVer = 0;
    }
#else
    if (argc >= 10)
    {
        if (atoi(argv[9]) == 2)
        {
            rpCMDARG->fgCompactLdrSize = 1;
        }
    }
#endif
    return 1;
}


int main(int argc, char **argv)
{
    int fdloader, fdldrhdr, fddiag, fdapphdr, fdphoto, fd8280bin, fdflash;
    int i4_len, i4_ret, i4_cnt;
    int i4Tmp, i4Ret;
    UINT32 *pu4Ptr;
    UINT32 u4Val, u4LoaderSize;
    IMAGE_UPGRADE_HEADER_T rImgUpHdr;
    IMAGE_HEADER_T rImgHdr;
    UINT8 szAppHdr[128], szLdrHdr[128];
    struct stat st;
    COMMAND_ARG_T *rpCMDARG;
#ifdef BIN_8280_ADDR
    //8280 bin
    UINT8 sz8280Bin[128];
    UINT32 u48280BinAddr;

    // TODO: temp
    printf("the bin address: 0x%08x\n", get_8280bin_address());
#endif    
    //malloc COMMAND_ARG_T
    rpCMDARG = (COMMAND_ARG_T *) malloc (sizeof (COMMAND_ARG_T));

    //Parse command arguments
    //
    if(!ParseCommandArgs(argc, argv, rpCMDARG))
    {
        printf("Parse command failed!\n");
        i4_ret = 1;
        goto ErrRet;
    }

    sprintf(szAppHdr, "%s.hdr", rpCMDARG->szApp);
    sprintf(szLdrHdr, "%s.hdr.bin", rpCMDARG->szLoader);

    /* open loader, diag, and target. */
    fdloader = open(rpCMDARG->szLoader, O_RDONLY);
    fdldrhdr = open(szLdrHdr, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    fddiag = open(rpCMDARG->szApp, O_RDONLY);
    fdapphdr = open(szAppHdr, O_CREAT | O_TRUNC | O_WRONLY, 0644);


    if ((fdloader < 0) || (fddiag < 0) || (fdapphdr < 0) || (fdldrhdr < 0))
    {
        printf("%s, %s, %s, %s or %s cannot be opened.\n", rpCMDARG->szLoader, rpCMDARG->szApp, rpCMDARG->szTarget, szAppHdr, szLdrHdr);
        i4_ret = 1;
        goto ErrRet;
    }

    i4_len = 0;
    memset(_achBuf, 0xff, LOADER_BUFFER_SIZE);
    i4_ret = read(fdloader, _achBuf, LOADER_BUFFER_SIZE);
    close(fdloader);
    fdloader = -1;

    if (rpCMDARG->fgCompactLdrSize)
    {
        u4LoaderSize = (i4_ret + 0x7f) & ~0x7f;
    }
    else
    {
        u4LoaderSize = rpCMDARG->u4LoaderMaxSize - 0x80;
    }
    printf("u4LoaderSize=%d(0x%x)\n", i4_ret, i4_ret);

    // fill upgrade header at tail of loader.
    if (rpCMDARG->szVersion != '\0')        //We have Customer, szModel, szVersion field
    {
        fill_hdr(&_achBuf[u4LoaderSize], rpCMDARG->szCustomer, rpCMDARG->szModel, rpCMDARG->szVersion);
    }
    else                                                    //Just fill out with zero
    {
        memset(&_achBuf[u4LoaderSize], 0, 0x80);
    }

    u4LoaderSize += 0x80;

    // fill the loader checksum
    pu4Ptr = (UINT32 *)_achBuf;
    u4Val = 0;
    while ((UINT32)pu4Ptr < (UINT32)&_achBuf[u4LoaderSize])
    {
        u4Val += *pu4Ptr;
        pu4Ptr++;
    }
    pu4Ptr--;
    *pu4Ptr = ~(u4Val);

    // write the loader size.
    if ((u4LoaderSize >= 0) && (u4LoaderSize <= (rpCMDARG->u4LoaderMaxSize))) {
        i4_cnt = write(fdldrhdr, _achBuf, u4LoaderSize);
        if (i4_cnt == u4LoaderSize) {
            i4_len = i4_cnt;
        } else {
            printf("%s(%d) failed\n", __FILE__, __LINE__);
            i4_ret = 1;
            goto ErrRet;
        }
    } else {
        printf("%s(%d) loader is too big\n", __FILE__, __LINE__);
        i4_ret = 1;
        goto ErrRet;
    }
    close(fdldrhdr);

    i4_ret = fstat(fddiag, &st);
    if (i4_ret != 0) {
        printf("fstat diag failed\n");
        i4_ret = 1;
        goto ErrRet;
    }

    if (rpCMDARG->fgUpgVer)
    {
        // write ImgHdr
        memset(&rImgUpHdr, 0, sizeof(rImgUpHdr));
        rImgUpHdr.u4Signature = IMAGE_SIGNATURE_UPGRADE;
        rImgUpHdr.u4Size = (UINT32)st.st_size;
        rImgUpHdr.fgCompress = rpCMDARG->i4CompressType;
        rImgUpHdr.u4FirmwareChksum = read_image_chksum(fddiag, rImgUpHdr.u4Size);
        rImgUpHdr.u4FirmwareExecAddr = 0x10000;
#ifndef USER_PQDATA_ADDRESS
        rImgUpHdr.u4PQDataAddr = 0x380000;
#else
        rImgUpHdr.u4PQDataAddr = USER_PQDATA_ADDRESS;
#endif /* ISER_PQDATA_ADDRESS */
        strncpy((void *)rImgUpHdr.au1CustomerId, rpCMDARG->szCustomer, 16);
        strncpy((void *)rImgUpHdr.au1ModelName, rpCMDARG->szModel, 32);
        strncpy((void *)rImgUpHdr.au1Version, rpCMDARG->szVersion, 64);

        if (sizeof(rImgUpHdr) != 144) {
            printf("rImgUpHdr is %d, wrong\n", sizeof(rImgUpHdr));
            i4_ret = 1;
            goto ErrRet;
        }

        i4Tmp = write(fdapphdr, &rImgUpHdr, sizeof(rImgUpHdr));
        if (i4Tmp != sizeof(rImgUpHdr)) {
            printf("write rImgUpHdr failed on AppHdr\n");
            i4_ret = 1;
            goto ErrRet;
        }
    }
    else
    {
        // write ImgHdr
        memset(&rImgHdr, 0, sizeof(rImgHdr));
        rImgHdr.u4Signature = IMAGE_SIGNATURE;
        rImgHdr.u4Size = (UINT32)st.st_size;
        rImgHdr.fgCompress = rpCMDARG->i4CompressType;
        rImgHdr.u4FirmwareChksum = read_image_chksum(fddiag, rImgHdr.u4Size);
        rImgHdr.u4FirmwareExecAddr = 0x10000;
#ifndef USER_PQDATA_ADDRESS
        rImgHdr.u4PQDataAddr = 0x380000;
#else
        rImgHdr.u4PQDataAddr = USER_PQDATA_ADDRESS;
#endif /* ISER_PQDATA_ADDRESS */

        if (sizeof(rImgHdr) != 64) {
            printf("rImgHdr is %d, wrong\n", sizeof(rImgHdr));
            i4_ret = 1;
            goto ErrRet;
        }

        i4Tmp = write(fdapphdr, &rImgHdr, sizeof(rImgHdr));
        if (i4Tmp != sizeof(rImgHdr)) {
            printf("write rImgHdr failed on AppHdr\n");
            i4_ret = 1;
            goto ErrRet;
        }
    }
    close(fddiag);

    fddiag = open(rpCMDARG->szApp, O_RDONLY);
    do {
        memset(_achBuf, 0xff, BUFFER_SIZE);
        i4_ret = read(fddiag, _achBuf, BUFFER_SIZE);
        i4_ret = ((i4_ret + 3) & 0xfffffffcU);
        if (i4_ret > 0) {
            i4Tmp = write(fdapphdr, _achBuf, i4_ret);
            if (i4Tmp != i4_ret) {
                printf("%s(%d) failed\n", __FILE__, __LINE__);
                i4Ret = 1;
                goto ErrRet;
            }
        }
    } while (i4_ret > 0);
    close(fddiag);
    fddiag = -1;

    close(fdapphdr);
    fdapphdr = -1;

    printf("target size is %d(0x%08x)\n", i4_len, i4_len);

ErrRet:
    if (fdloader >= 0) { close(fdloader); }
    if (fddiag >= 0) { close(fddiag); }
    if (fdapphdr >= 0) { close(fdapphdr); }

    //mfree COMMAND_ARG_T
    free ((void *)rpCMDARG);
    
    return i4_ret;
}

