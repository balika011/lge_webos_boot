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
 * $RCSfile: dmx_es_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sha256.c
 *  SHA256 driver
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_drm_drvif.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define x_memset memset

// Constants defined in SHA-256
const UINT32 K[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/*
 *  Define the SHA256
 */
#define	ROTATER(x,n)	(((x) >> (n)) | ((x) << (32 - (n))))
#define	SHIFTR(x,n)		((x) >> (n))

#define	CH(x,y,z)		(((x) & (y)) ^ (~(x) & (z)))
#define	MAJ(x,y,z)		(((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define	SUM0(x)			(ROTATER((x), 2) ^ ROTATER((x), 13) ^ ROTATER((x), 22))
#define	SUM1(x)			(ROTATER((x), 6) ^ ROTATER((x), 11) ^ ROTATER((x), 25))
#define	RHO0(x)			(ROTATER((x), 7) ^ ROTATER((x), 18) ^ SHIFTR((x), 3))
#define	RHO1(x)			(ROTATER((x), 17) ^ ROTATER((x), 19) ^ SHIFTR((x), 10))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

void SHA256PadMessage(SHA256_CONTEXT_T *);
void SHA256ProcessMessageBlock(SHA256_CONTEXT_T *);

/*
 *  SHA256_Reset
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to reset.
 *
 *  Returns:
 *      sha Error Code.
 *
 */
INT32 SHA256_Reset(SHA256_CONTEXT_T *context)
{
    if (!context)
    {
        return shaNull;
    }

    context->Length_Low             = 0;
    context->Length_High            = 0;
    context->Message_Block_Index    = 0;

    context->Intermediate_Hash[0]   = 0x6a09e667;
    context->Intermediate_Hash[1]   = 0xbb67ae85;
    context->Intermediate_Hash[2]   = 0x3c6ef372;
    context->Intermediate_Hash[3]   = 0xa54ff53a;
    context->Intermediate_Hash[4]   = 0x510e527f;
    context->Intermediate_Hash[5]   = 0x9b05688c;
    context->Intermediate_Hash[6]   = 0x1f83d9ab;
    context->Intermediate_Hash[7]   = 0x5be0cd19;

    context->Computed   = 0;
    context->Corrupted  = 0;

    return shaSuccess;
}

/*
 *  SHA256_Result
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to use to calculate the SHA-256 hash.
 *      Message_Digest: [out]
 *          Where the digest is returned.
 *
 *  Returns:
 *      sha Error Code.
 *
 */
INT32 SHA256_Result( SHA256_CONTEXT_T *context,
                UINT8 Message_Digest[SHA256_HASH_SIZE])
{
    INT32 i;

    if (!context || !Message_Digest)
    {
        return shaNull;
    }

    if (context->Corrupted)
    {
        return context->Corrupted;
    }

    if (!context->Computed)
    {
        SHA256PadMessage(context);
        for(i=0; i<64; ++i)
        {
            /* message may be sensitive, clear it out */
            context->Message_Block[i] = 0;
        }
        context->Length_Low = 0;    /* and clear length */
        context->Length_High = 0;
        context->Computed = 1;

    }

    for(i = 0; i < SHA256_HASH_SIZE; ++i)
    {
        Message_Digest[i] = context->Intermediate_Hash[i>>2]
                            >> 8 * ( 3 - ( i & 0x03 ) );
    }

    return shaSuccess;
}

/*
 *  SHA256_Input
 *
 *  Description:
 *      This function accepts an array of octets as the next portion
 *      of the message.
 *
 *  Parameters:
 *      context: [in/out]
 *          The SHA context to update
 *      message_array: [in]
 *          An array of characters representing the next portion of
 *          the message.
 *      length: [in]
 *          The length of the message in message_array
 *
 *  Returns:
 *      sha Error Code.
 *
 */
INT32 SHA256_Input(SHA256_CONTEXT_T *context, const UINT8 *message_array, UINT32 length)
{
    if (!length)
    {
        return shaSuccess;
    }

    if (!context || !message_array)
    {
        return shaNull;
    }

    if (context->Computed)
    {
        context->Corrupted = shaStateError;

        return shaStateError;
    }

    if (context->Corrupted)
    {
         return context->Corrupted;
    }
    while(length-- && !context->Corrupted)
    {
        context->Message_Block[context->Message_Block_Index++] =
                        (*message_array & 0xFF);

        context->Length_Low += 8;
        if (context->Length_Low == 0)
        {
            context->Length_High++;
            if (context->Length_High == 0)
            {
                /* Message is too long */
                context->Corrupted = 1;
            }
        }

        if (context->Message_Block_Index == 64)
        {
            SHA256ProcessMessageBlock(context);
        }

        message_array++;
    }

    return shaSuccess;
}

/*
 *  SHA256ProcessMessageBlock
 *
 *  Description:
 *      This function will process the next 512 bits of the message
 *      stored in the Message_Block array.
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:

 *      Many of the variable names in this code, especially the
 *      single character names, were used because those were the
 *      names used in the publication.
 *
 *
 */
void SHA256ProcessMessageBlock(SHA256_CONTEXT_T *context)
{
    INT32       t;                      /* Loop counter                */
    UINT32      temp, temp2;            /* Temporary word value        */
    UINT32      W[64];                  /* Word sequence               */
    UINT32      A, B, C, D, E, F, G, H; /* Word buffers                */

    /*
     *  Initialize the first 16 words in the array W
     */
    for(t = 0; t < 16; t++)
    {
        W[t] = (context->Message_Block[t * 4 + 0] << 24) |
               (context->Message_Block[t * 4 + 1] << 16) |
               (context->Message_Block[t * 4 + 2] <<  8) |
               (context->Message_Block[t * 4 + 3]);
    }

	for(t = 16; t < 64; ++t)
	{
		W[t] = RHO1(W[t - 2]) + W[t - 7] + RHO0(W[t - 15]) + W[t - 16];
	}

    A = context->Intermediate_Hash[0];
    B = context->Intermediate_Hash[1];
    C = context->Intermediate_Hash[2];
    D = context->Intermediate_Hash[3];
    E = context->Intermediate_Hash[4];
    F = context->Intermediate_Hash[5];
    G = context->Intermediate_Hash[6];
    H = context->Intermediate_Hash[7];

	for(t = 0; t < 64; ++t)
	{
		temp = H + SUM1(E) + CH(E, F, G) + K[t] + W[t];
		temp2 = SUM0(A) + MAJ(A, B, C);
		H = G;
		G = F;
		F = E;
		E = D + temp;
		D = C;
		C = B;
		B = A;
		A = temp + temp2;
	}

    context->Intermediate_Hash[0] += A;
    context->Intermediate_Hash[1] += B;
    context->Intermediate_Hash[2] += C;
    context->Intermediate_Hash[3] += D;
    context->Intermediate_Hash[4] += E;
    context->Intermediate_Hash[5] += F;
    context->Intermediate_Hash[6] += G;
    context->Intermediate_Hash[7] += H;

    context->Message_Block_Index = 0;
}

/*
 *  SHA256PadMessage
 *

 *  Description:
 *      According to the standard, the message must be padded to an even
 *      512 bits.  The first padding bit must be a '1'.  The last 64
 *      bits represent the length of the original message.  All bits in
 *      between should be 0.  This function will pad the message
 *      according to those rules by filling the Message_Block array
 *      accordingly.  It will also call the ProcessMessageBlock function
 *      provided appropriately.  When it returns, it can be assumed that
 *      the message digest has been computed.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to pad
 *      ProcessMessageBlock: [in]
 *          The appropriate SHA*ProcessMessageBlock function
 *  Returns:
 *      Nothing.
 *
 */

void SHA256PadMessage(SHA256_CONTEXT_T *context)
{
    /*
     *  Check to see if the current message block is too small to hold
     *  the initial padding bits and length.  If so, we will pad the
     *  block, process it, and then continue padding into a second
     *  block.
     */
    if (context->Message_Block_Index > 55)
    {
        context->Message_Block[context->Message_Block_Index++] = 0x80;
        while(context->Message_Block_Index < 64)
        {
            context->Message_Block[context->Message_Block_Index++] = 0;
        }

        SHA256ProcessMessageBlock(context);

        while(context->Message_Block_Index < 56)
        {
            context->Message_Block[context->Message_Block_Index++] = 0;
        }
    }
    else
    {
        context->Message_Block[context->Message_Block_Index++] = 0x80;
        while(context->Message_Block_Index < 56)
        {

            context->Message_Block[context->Message_Block_Index++] = 0;
        }
    }

    /*
     *  Store the message length as the last 8 octets
     */
    context->Message_Block[56] = context->Length_High >> 24;
    context->Message_Block[57] = context->Length_High >> 16;
    context->Message_Block[58] = context->Length_High >> 8;
    context->Message_Block[59] = context->Length_High;
    context->Message_Block[60] = context->Length_Low >> 24;
    context->Message_Block[61] = context->Length_Low >> 16;
    context->Message_Block[62] = context->Length_Low >> 8;
    context->Message_Block[63] = context->Length_Low;

    SHA256ProcessMessageBlock(context);
}


INT32 SHA256(const UINT8 *message_array, UINT32 length, UINT8 Message_Digest[SHA256_HASH_SIZE])
{
    SHA256_CONTEXT_T rContext;
    if(SHA256_Reset(&rContext) != shaSuccess)
    {
        //LOG(5, "%s Fail to reset.", __FUNCTION__);
        return shaNull;
    }

    if(SHA256_Input(&rContext, message_array, length) != shaSuccess)
    {
        //LOG(5, "%s Fail to input.", __FUNCTION__);
        return shaStateError;
    }

    if(SHA256_Result(&rContext, Message_Digest) != shaSuccess)
    {
        //LOG(5, "%s Fail to get result.", __FUNCTION__);
        return shaNull;
    }

    return shaSuccess;
}


INT32 mtk_hmac_sha256(UINT8 * key, UINT32 key_size, UINT8 * msg, UINT32 msg_size,  UINT8 result[SHA256_HASH_SIZE])
{
	UINT8 ipad[SHA_BLOCK_SIZE], opad[SHA_BLOCK_SIZE], kpad[SHA_BLOCK_SIZE];
	UINT8 hash_result[SHA256_HASH_SIZE];
	UINT8 *buf = NULL, i;

	x_memset(ipad, 0x36, sizeof(ipad));
	x_memset(opad, 0x5c, sizeof(opad));
	x_memset(kpad, 0x00, sizeof(kpad));

	if(key_size > SHA_BLOCK_SIZE) {
		SHA256(key, key_size, hash_result);
		x_memcpy(kpad, hash_result, SHA256_HASH_SIZE);
	} else {
		x_memcpy(kpad, key, key_size);
	}

    for(i = 0; i < SHA_BLOCK_SIZE; i++)
    {
    	ipad[i] ^= kpad[i];
    	opad[i] ^= kpad[i];
    }

	buf = (UINT8 *)x_mem_alloc(SHA_BLOCK_SIZE + msg_size);
	if(buf) {
		/* 1st hash */
		x_memset(buf, 0, sizeof(SHA_BLOCK_SIZE + msg_size));
		x_memcpy(buf, ipad, SHA_BLOCK_SIZE);
		x_memcpy(buf + SHA_BLOCK_SIZE, msg, msg_size);
		SHA256(buf, msg_size + SHA_BLOCK_SIZE, hash_result);

		/* 2nd hash */
		x_memset(buf, 0, sizeof(msg_size + SHA_BLOCK_SIZE));
		x_memcpy(buf, opad, SHA_BLOCK_SIZE);
		x_memcpy(buf + SHA_BLOCK_SIZE, hash_result, SHA256_HASH_SIZE);
		SHA256(buf, SHA_BLOCK_SIZE + SHA256_HASH_SIZE, hash_result);

		/* result */
		x_memcpy(result, hash_result, SHA256_HASH_SIZE);

		/* free */
		x_mem_free(buf);
	} else {
		return -1;
	}

	return 0;
}


#ifdef TEST_MODE
void Sha256_unit_test()
{
    UINT8 result[SHA256_HASH_SIZE], i;
    UINT8 *key = "key";
    UINT8 *msg = "The quick brown fox jumps over the lazy dog";

    SHA256(msg, x_strlen(msg), result);

    Printf("SHA256\n");

    for(i = 0; i < SHA256_HASH_SIZE; i++)
    {
        //Printf("0x%x\n", result[i]);
    }

    Printf("HMAC SHA256 NULL\n");
    mtk_hmac_sha256(NULL, 0, NULL, 0, result);

    for(i = 0; i < SHA256_HASH_SIZE; i++)
    {
        //Printf("0x%x\n", result[i]);
    }

    Printf("HMAC SHA256 len\n");
    mtk_hmac_sha256(key, x_strlen(key), msg, x_strlen(msg), result);

    for(i = 0; i < SHA256_HASH_SIZE; i++)
    {
        //Printf("0x%x\n", result[i]);
    }

    return;
}
#endif
