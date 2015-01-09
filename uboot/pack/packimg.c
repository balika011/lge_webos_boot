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

#define NOT_SPECIFIC    -1
#define NOT_LIMIT       0x7FFFFFFF

#define MAX_BUF_COUNT	2048
UINT8 WriteBuf[MAX_BUF_COUNT];

typedef struct
{
    char FileName[1024];
    INT32 BaseAddr;
    INT32 Length;
} FILE_INFO_T;

INT32 GetBaseAddr(UINT8 *szBaseAddr) 
{
    INT32 i4BaseAddr;
    if( szBaseAddr[0] == '.' )                               //Not Specific base address
        i4BaseAddr = NOT_SPECIFIC;
    else
        i4BaseAddr = strtol (szBaseAddr,NULL,16);

    return i4BaseAddr;
}

INT32 GetLength(UINT8 *szLength) 
{
    INT32 i4Length;
    if( szLength[0] == '.' )                                //Not Specific length
        i4Length = NOT_LIMIT;
    else
        i4Length = strtol (szLength,NULL,16);
    
    return i4Length;
}

static void _PACK_Usage(UINT8 **argv)
{
    printf("Usage: %s -f\n", argv[0]);
    printf("\t -f [script] Scripter filename\n");
}

//-----------------------------------------------------------------------------
/** FillFF to File handler pOutputFile.
  *  Details of FillFF (optional).
  *  @retval 0	Success
  *  @retval 1	Fail 
  */
//-----------------------------------------------------------------------------
INT32 FillFF(FILE *pOutputFile, INT32 Length) 
{
    INT32 i4WCnt = 0;
    INT32 i4TotoalWCnt = 0, i4Count = 0;
    //fill out the space with 0xFF
    while(i4TotoalWCnt < Length)
	{
		if( (i4TotoalWCnt + MAX_BUF_COUNT) < Length)
			i4Count = MAX_BUF_COUNT;
		else
			i4Count = Length - i4TotoalWCnt;
			
		memset(WriteBuf, 0xff, i4Count);
		i4WCnt = fwrite(WriteBuf, 1, i4Count, pOutputFile);
		if(i4WCnt != i4Count)
		{
			printf("Fill 0xFF to file fail!!\n");
			goto ErrHandle;
		}
		i4TotoalWCnt += i4WCnt;
	}
    
ErrHandle:    
    if( Length == i4TotoalWCnt )
        return 0;
    else
        return 1;
}

//-----------------------------------------------------------------------------
/** FileMerge to merge input file to output file.
  *  Details of FileMerge (optional).
  *  @retval 0	            Success
  *  @retval negative value	Fail 
  */
//-----------------------------------------------------------------------------
INT32 FileMerge(FILE_INFO_T rOutputFile, FILE_INFO_T rInputFile, UINT32 *pu4FilePointer) 
{

    FILE *pOutputFile, *pInputFile;
    INT32 i4Ret = 0, i4Count = 0, i4WCnt = 0, i4RCnt = 0, i4Len = 0;
    
   	pOutputFile = fopen(rOutputFile.FileName, "ab+");
	if(pOutputFile == NULL)
	{
		printf("open file %s error!!\n", rOutputFile.FileName);
        i4Ret = -1;
		goto ErrHandle;
	}
	
   	pInputFile = fopen(rInputFile.FileName, "rb");
	if(pInputFile == NULL)
	{
		printf("open file %s error!!\n", rInputFile.FileName);
        i4Ret = -2;
		goto ErrHandle;
	}

    // file pointer is less than base address of output file
    // This is the special case when you set the base address of output file is not "." or "0x0"
    if( rOutputFile.BaseAddr > *pu4FilePointer )             
    {
        if( FillFF(pOutputFile, rOutputFile.BaseAddr - *pu4FilePointer) )
        {
			printf("Fill 0xFF to %s fail!!\n", rOutputFile.FileName);
			i4Ret = -3;
			goto ErrHandle;
        }
        else 
        {
    		*pu4FilePointer += rOutputFile.BaseAddr - *pu4FilePointer;
        }
    }

    // The current file pointer is over the output file limitation
    if( rOutputFile.BaseAddr + rOutputFile.Length < *pu4FilePointer )             
    {
        printf("Current File Pointer 0x%x is over the adddress length limitation 0x%x of Output File %s!!\n", *pu4FilePointer, rOutputFile.BaseAddr+rOutputFile.Length, rOutputFile.FileName);
	    i4Ret = -3;
		goto ErrHandle;
    }
    
    if( *pu4FilePointer <= rInputFile.BaseAddr ) 
    {
        //fill out the space with 0xFF
        if( FillFF(pOutputFile, rInputFile.BaseAddr - *pu4FilePointer) )
        {
			printf("Fill 0xFF to %s fail!!\n", rOutputFile.FileName);
    		i4Ret = -4;
			goto ErrHandle;
        }
        else 
        {
    		*pu4FilePointer += rInputFile.BaseAddr - *pu4FilePointer;
        }
    }
    else 
    {
        printf("Current File Pointer 0x%x is over the base adddress 0x%x of Input File %s!!\n", *pu4FilePointer, rInputFile.BaseAddr, rInputFile.FileName);
		i4Ret = -5;
		goto ErrHandle;        
    }

    // Merge input file into output file
	while((i4RCnt=fread(WriteBuf, 1, MAX_BUF_COUNT, pInputFile)) > 0)
	{
		i4WCnt = fwrite(WriteBuf, 1, i4RCnt, pOutputFile);
		if(i4WCnt != i4RCnt)
		{
			printf("Read %s or Write %s fail!!\n",rInputFile.FileName, rOutputFile.FileName);
			i4Ret = -6;
			goto ErrHandle;
		}
		i4Len += i4RCnt;
	}
	*pu4FilePointer += i4Len;
	
    // Checking status
    if( i4Len > rInputFile.Length ) 
    {
        printf("Size 0x%x of %s is over the length limiation 0x%x in script file !!\n", i4Len, rInputFile.FileName, rInputFile.Length);
		i4Ret = -7;
		goto ErrHandle;
    }

    if((rInputFile.Length != NOT_LIMIT) && (rInputFile.BaseAddr + rInputFile.Length > *pu4FilePointer) ) 
    {

        //fill out the space with 0xFF
        if( FillFF(pOutputFile, rInputFile.BaseAddr + rInputFile.Length - *pu4FilePointer) )
        {
			printf("Fill 0xFF to %s fail!!\n", rOutputFile.FileName);
    		i4Ret = -8;
			goto ErrHandle;
        }
        else 
        {
    		*pu4FilePointer += rInputFile.BaseAddr + rInputFile.Length - *pu4FilePointer;
        }
    }

    // This is a special case when output file has length limitation, but not be used
    // Then we need to fill out all the remaining space with 0xff after merge all the other files
/*
    if(rOutputFile.Length != NOT_LIMIT && (*pu4FilePointer <= rOutputFile.BaseAddr + rOutputFile.Length)) 
    {

        //fill out the space with 0xFF
        if( FillFF(pOutputFile, rOutputFile.BaseAddr + rOutputFile.Length - *pu4FilePointer) )
        {
			printf("Fill 0xFF to %s fail!!\n", rOutputFile.FileName);
    		i4Ret = -9;
			goto ErrHandle;
        }
        else 
        {
    		*pu4FilePointer += rOutputFile.BaseAddr + rOutputFile.Length - *pu4FilePointer;
        }
    }
    else 
    {
        printf("Size of %s is over the length limiation in script file !!\n",rOutputFile.FileName);
		i4Ret = -10;
		goto ErrHandle;
    }
*/
    if ( ftell (pOutputFile) != *pu4FilePointer ) 
    {
        printf("Current file pointer 0x%x is not equal to pu4FilePointer: 0x%x !!\n", ftell(pOutputFile), *pu4FilePointer);
		i4Ret = -11;
		goto ErrHandle;
    }

ErrHandle:
    if(i4Ret != 0) 
    {
        printf("FileMerge Error Code: %d !!\n", i4Ret);        
    }

	if(pInputFile!=NULL)	fclose(pInputFile);
	if(pOutputFile!=NULL)	fclose(pOutputFile);

	return i4Ret;
}       
        
//-----------------------------------------------------------------------------
/** ProcessScriptFile parse script file and process it.
  *  Details of ProcessScriptFile (optional).
  *  @retval 0	Success
  *  @retval 1	Fail 
  */
//-----------------------------------------------------------------------------
int ProcessScriptFile(UINT8 *szScript) 
{
    FILE *pScriptFile, *pOutputFile;
    FILE_INFO_T rOutputFile;
    FILE_INFO_T rInputFile;
    char szOutputBase[10], szOutputLength[10];
    char szInputBase[10], szInputLength[10];
    UINT32 i4FilePointer = 0x0; //maintain the output file pointer for operation.
    INT32 i4Ret = 0;
    
    pScriptFile = fopen(szScript, "r");

    // Get output file info
    fscanf(pScriptFile, "%s %s %s", rOutputFile.FileName, szOutputBase, szOutputLength );
    printf( "\nProcessing line : %s %s %s\n", rOutputFile.FileName, szOutputBase, szOutputLength );
    
    rOutputFile.BaseAddr = GetBaseAddr(szOutputBase);
    rOutputFile.Length = GetLength(szOutputLength);

    printf("Output file name: %s\n", rOutputFile.FileName );

    if(rOutputFile.BaseAddr == NOT_SPECIFIC)
        rOutputFile.BaseAddr = 0x0;    
    printf("Base address    : 0x%x\n", rOutputFile.BaseAddr );

    if(rOutputFile.Length != NOT_LIMIT)
        printf("Length limit    : 0x%x\n", rOutputFile.Length );

    // remove previous output file if it exists
    remove(rOutputFile.FileName);
    
    // Process each input file
    while( !feof(pScriptFile) ) 
    {
        
        if( fscanf(pScriptFile, "%s %s %s", rInputFile.FileName, szInputBase, szInputLength ) != 3 )
        {
            break;    
        }
        
        printf("\nProcessing line : %s %s %s\n",rInputFile.FileName, szInputBase, szInputLength );
        
        rInputFile.BaseAddr = GetBaseAddr(szInputBase);
        rInputFile.Length = GetLength(szInputLength);

        printf("Input file name : %s\n", rInputFile.FileName );

        if(rInputFile.BaseAddr == NOT_SPECIFIC)
            rInputFile.BaseAddr = i4FilePointer;
        printf("Base address    : 0x%x\n", rInputFile.BaseAddr );

        if(rInputFile.Length != NOT_LIMIT)
            printf("Length limit    : 0x%x\n", rInputFile.Length );

        //Input File merge into Output File
        if(FileMerge(rOutputFile, rInputFile, &i4FilePointer) != 0)
        {
            i4Ret = -1;
            break;    
        }
    }

    if(i4Ret != 0) 
    {
        printf("Processing script file failed: Error %d\n", i4Ret );
    }
    else 
    {
        printf("\nProcessing script file completed.\n" );
        printf("Output file name: %s\n", rOutputFile.FileName );
        printf("File Size       : 0x%x\n", i4FilePointer );
        
    }
    
    if(pScriptFile!=NULL)	fclose(pScriptFile);
    return i4Ret;
}

int main(int argc, char **argv)
{
    UINT8 *szScript;
    INT32 i4_ret = 0;
    UINT32 i = 0;
    
    if (argc < 2) 
    {
        _PACK_Usage(argv);
        return 1;
    }

    for (i=1; i<(argc-1); i++) 
    {
        if (argv[i][0]!='-') { continue; }
        switch(argv[i][1]) 
        {
        case 'f': case 'F':
            i++;
            szScript = argv[i];
            break;        	
          
        default:
            _PACK_Usage(argv);
            return 1;
        }
    }

    i4_ret = ProcessScriptFile(szScript);
    
    return i4_ret;
    
}

