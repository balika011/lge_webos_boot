/*-----------------------------------------------------------------------------
 * uboot/board/mt5398/mt5398_secure_rom.c
 *
 * MT53xx Secure Boot driver
 *
 * Copyright (c) 2008-2012 MediaTek Inc.
 * $Author: p4admin $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 *---------------------------------------------------------------------------*/


#ifdef CC_SECURE_ROM_BOOT

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "mt5882_secure_rom.h"
#include "../include/configs/mt5882.h"
#include "bignum.h"
#include "x_ckgen.h"
#include "x_ldr_env.h"
#include <common.h>

#if defined CC_EMMC_BOOT
#include <mmc.h>
#else
#include <nand.h>
#endif

#ifdef CC_A1_SECURE_BOOT
//#define CC_USE_SHA1
//#define CC_USE_HW_SHA1
#define CC_USE_SHA256
#define CC_USE_HW_SHA256


#include <partinfo.h>
#include "lg_modeldef.h"
#include "gcpu_if.h"

extern void writeFullVerifyOTP(void);
#else
#define CC_USE_MD5
#endif // CC_A1_SECURE_BOOT

unsigned int u4FragSize = 0;
unsigned int u4FragNum = 0;
typedef struct{
	unsigned int frag_num;
	unsigned int frag_size;

	
}signature_header;
// the customer public key is copied from CC_LDR_ENV_OFFSET in mt53xx_sif.c
extern LDR_ENV_T *_prLdrEnv;

int verify_done = 0;

int IsRunOnUsb(char* uenv, int uenv_size)
{
    static int run_on_usb = 0;
    static int fgInit = 0;

    if (!fgInit)
    {
	    LDR_ENV_T *prLdrEnv = (LDR_ENV_T *)CC_LDR_ENV_OFFSET;

	    if (prLdrEnv->u4CfgFlag == USIG_RUN_ON_USB)
	    {
	        if (uenv)
	        {
	            strncpy(uenv, prLdrEnv->szUenv, uenv_size);
	        }

	        run_on_usb = 1;
	    }

	    fgInit = 1;
    }

    return run_on_usb;
}

#if defined(CC_USE_MD5)

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/* F, G, H and I are basic MD5 functions.
 */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
 */
#define FF(a, b, c, d, x, s, ac) { \
    (a) += F ((b), (c), (d)) + (x) + (UINT32)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
    }
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G ((b), (c), (d)) + (x) + (UINT32)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
    }
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H ((b), (c), (d)) + (x) + (UINT32)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
    }
#define II(a, b, c, d, x, s, ac) { \
    (a) += I ((b), (c), (d)) + (x) + (UINT32)(ac); \
    (a) = ROTATE_LEFT ((a), (s)); \
    (a) += (b); \
    }
                
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT8 PADDING[64] = 
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static void MD5Transform(UINT32 [4], UINT8 [64]);
static void Encode(UINT8 *, UINT32 *, UINT32);
static void Decode(UINT32 *, UINT8 *, UINT32);

/* MD5 initialization. Begins an MD5 operation, writing a new context.
 */
void MD5_Reset(MD5_CONTEXT_T *context)
{
    context->au4Count[0] = context->au4Count[1] = 0;
    
    context->au4State[0] = 0x67452301;
    context->au4State[1] = 0xefcdab89;
    context->au4State[2] = 0x98badcfe;
    context->au4State[3] = 0x10325476;
}

/* MD5 block update operation. Continues an MD5 message-digest
  operation, processing another message block, and updating the
  context.
 */
void MD5_Input(MD5_CONTEXT_T *context, const UINT8 *input, UINT32 inputLen)
{
    UINT32 i, index, partLen;

    /* Compute number of bytes mod 64 */
    index = (UINT32)((context->au4Count[0] >> 3) & 0x3F);

    /* Update number of bits */
    if ((context->au4Count[0] += ((UINT32)inputLen << 3)) < ((UINT32)inputLen << 3)) 
    {
        context->au4Count[1]++;
    }

    context->au4Count[1] += ((UINT32)inputLen >> 29);

    partLen = 64 - index;

    /* Transform as many times as possible.*/

    if (inputLen >= partLen) 
    {
        memcpy((UINT8 *)&context->au1Buffer[index], (UINT8 *)input, partLen);
        MD5Transform (context->au4State, context->au1Buffer);

        for (i = partLen; i + 63 < inputLen; i += 64)
        {
            MD5Transform (context->au4State, &input[i]);
        }

        index = 0;
    }
    else 
    {
        i = 0;
    }

    /* Buffer remaining input */
    memcpy((UINT8 *)&context->au1Buffer[index], (UINT8 *)&input[i], inputLen-i);
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
  the message digest and zeroizing the context.
 */
void MD5_Result(MD5_CONTEXT_T *context, UINT8 digest[MD5_HASH_SIZE]) 
{
    UINT8 bits[8];
    UINT32 index, padLen;

    /* Save number of bits */
    Encode (bits, context->au4Count, 8);

    /* Pad out to 56 mod 64.*/
    index = (UINT32)((context->au4Count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    MD5_Input (context, PADDING, padLen);

    /* Append length (before padding) */
    MD5_Input (context, bits, 8);

    /* Store state in digest */
    Encode (digest, context->au4State, 16);

    /* Zeroize sensitive information.*/
    memset((UINT8 *)context, 0, sizeof(*context));
}

/* MD5 basic transformation. Transforms state based on block.*/
static void MD5Transform (UINT32 state[4], UINT8 block[64])
{
    UINT32 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    Decode (x, block, 64);

    /* Round 1 */
    FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
    FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
    FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
    FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
    FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
    FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
    FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
    FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
    FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
    FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
    FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
    FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
    FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
    FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
    FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
    FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

    /* Round 2 */
    GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
    GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
    GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
    GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
    GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
    GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
    GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
    GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
    GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
    GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
    GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */

    GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
    GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
    GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
    GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
    GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

    /* Round 3 */
    HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
    HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
    HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
    HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
    HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
    HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
    HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
    HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
    HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
    HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
    HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
    HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
    HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
    HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
    HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
    HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

    /* Round 4 */
    II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
    II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
    II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
    II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
    II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
    II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
    II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
    II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
    II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
    II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
    II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
    II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
    II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
    II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
    II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
    II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    /* Zeroize sensitive information.*/
    memset ((UINT8 *)x, 0, sizeof(x));    
}

//-----------------------------------------------------------------------------
// Type definitions
static void Encode (UINT8 *output, UINT32 *input, UINT32 len)
{
    UINT32 i, j;

    for (i = 0, j = 0; j < len; i++, j += 4) {
        output[j] = (UINT8)(input[i] & 0xff);
        output[j+1] = (UINT8)((input[i] >> 8) & 0xff);
        output[j+2] = (UINT8)((input[i] >> 16) & 0xff);
        output[j+3] = (UINT8)((input[i] >> 24) & 0xff);
    }
}

/* Decodes input (UINT8) into output (UINT32). Assumes len is
  a multiple of 4.
 */
static void Decode (UINT32 *output, UINT8 *input, UINT32 len)
{
    UINT32 i, j;

    for (i = 0, j = 0; j < len; i++, j += 4) 
    {
        output[i] = ((UINT32)input[j]) | (((UINT32)input[j+1]) << 8) |
            (((UINT32)input[j+2]) << 16) | (((UINT32)input[j+3]) << 24);
    }
}

UINT32 MD_PadMessage(UINT8 *u1Data, UINT32 u4Size)
{
    // If byte size is 64 bytes alignment, do nothing.
    UINT32 u4Offset;
    u4Offset = u4Size;
    UINT32 u4SizeInBits = u4Size * 8;

    UINT32 index, padLen;
    
    /* Pad out to 56 mod 64.*/
    index = (UINT32)(u4Size & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    memcpy((UINT8 *)(u1Data+u4Size), PADDING, padLen);

    u4Offset = u4Size + padLen;

    *(UINT8 *)(u1Data + u4Offset++) = (u4SizeInBits);
    *(UINT8 *)(u1Data + u4Offset++) = (u4SizeInBits) >> 8;
    *(UINT8 *)(u1Data + u4Offset++) = (u4SizeInBits) >> 16;
    *(UINT8 *)(u1Data + u4Offset++) = (u4SizeInBits) >> 24;
    *(UINT8 *)(u1Data + u4Offset++) = 0;
    *(UINT8 *)(u1Data + u4Offset++) = 0;
    *(UINT8 *)(u1Data + u4Offset++) = 0;
    *(UINT8 *)(u1Data + u4Offset++) = 0;
                
    return u4Offset; 
}

void X_CalculateMD5(UINT8 *pu1MessageDigest, UINT32 StartAddr, UINT32 u4Size)
{
    MD5_CONTEXT_T context;

    MD5_Reset(&context);

    MD5_Input(&context, (const unsigned char *)StartAddr, u4Size);

    MD5_Result(&context, pu1MessageDigest);

    return 0;
}
#endif  // CC_USE_MD5

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
int X_CalculateSHA256(UINT8 *pu1MessageDigest, UINT32 StartAddr, UINT32 u4Size)
{
    int err;
    
#ifdef CC_USE_HW_SHA256
    MD_PARAM_T rParam;
    UINT32 u4BufStart;

    GCPU_Init(0);

    if((StartAddr == 0) || (u4Size == 0))
    {
        return -1;
    }

    if(((StartAddr & 0xf0) >> 4) % 4 != 0)
{
        return -1;
    }

    rParam.u4SrcStartAddr = (UINT32)StartAddr;
    rParam.u4SrcBufStart = GCPU_LINER_BUFFER_START(StartAddr);
    rParam.u4SrcBufEnd = GCPU_LINER_BUFFER_END(StartAddr + u4Size);
    rParam.u4DatLen = u4Size;
    rParam.fgFirstPacket = TRUE;
    rParam.fgLastPacket = TRUE;
    rParam.u8BitCnt = 0;

    err = GCPU_Cmd(0, GCPU_SHA_256, &rParam);
    
    memcpy(pu1MessageDigest, rParam.au1Hash, 32);
#else
    SHA1Context sha;

    err = SHA1Reset(&sha);
    if (err)
    {
        //CHIP_DisplayString("SHA1Reset Error.\n");
    }

    err = SHA1Input(&sha, (const unsigned char *)StartAddr, u4Size);
    if (err)
    {
        //CHIP_DisplayString("SHA1Input Error.\n");
    }

    err = SHA1Result(&sha, pu1MessageDigest);
    if (err)
    {
        //CHIP_DisplayString("SHA1Result Error.\n");
    }
#endif

    return err;
}

#if defined(CC_USE_SHA1)

/*
 *  Define the SHA1 circular left shift macro
 */
#define SHA1CircularShift(bits,word) (((word) << (bits)) | ((word) >> (32-(bits))))

/* Local Function Prototyptes */
static void SHA1PadMessage(SHA1Context *);
static void SHA1ProcessMessageBlock(SHA1Context *);

/*
 *  SHA1Reset
 *
 *  Description:
 *      This function will initialize the SHA1Context in preparation
 *      for computing a new SHA1 message digest.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to reset.
 *
 *  Returns:
 *      sha Error Code.
 *
 */
int SHA1Reset(SHA1Context *context)
{
    if (!context)
    {
        return shaNull;
    }

    context->Length_Low             = 0;
    context->Length_High            = 0;
    context->Message_Block_Index    = 0;

    context->Intermediate_Hash[0]   = 0x67452301;
    context->Intermediate_Hash[1]   = 0xEFCDAB89;
    context->Intermediate_Hash[2]   = 0x98BADCFE;
    context->Intermediate_Hash[3]   = 0x10325476;
    context->Intermediate_Hash[4]   = 0xC3D2E1F0;

    context->Computed   = 0;
    context->Corrupted  = 0;

    return shaSuccess;
}

/*
 *  SHA1Result
 *
 *  Description:
 *      This function will return the 160-bit message digest into the
 *      Message_Digest array  provided by the caller.
 *      NOTE: The first octet of hash is stored in the 0th element,
 *            the last octet of hash in the 19th element.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to use to calculate the SHA-1 hash.
 *      Message_Digest: [out]
 *          Where the digest is returned.
 *
 *  Returns:
 *      sha Error Code.
 *
 */
int SHA1Result( SHA1Context *context, UINT8 Message_Digest[SHA1HashSize])
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
        SHA1PadMessage(context);
        for(i=0; i<64; ++i)
        {
            /* message may be sensitive, clear it out */
            context->Message_Block[i] = 0;
        }
        
        context->Length_Low = 0;    /* and clear length */
        context->Length_High = 0;
        context->Computed = 1;

    }

    for (i = 0; i < SHA1HashSize; ++i)
    {
        Message_Digest[i] = context->Intermediate_Hash[i>>2]
                            >> 8 * ( 3 - ( i & 0x03 ) );
    }

    return shaSuccess;
}

/*
 *  SHA1Input
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
int SHA1Input(SHA1Context *context, const UINT8 *message_array, UINT32 length)
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
        if (context->Message_Block_Index<64)
        {
            context->Message_Block[context->Message_Block_Index++] = (*message_array & 0xFF);
        }

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
            SHA1ProcessMessageBlock(context);
        }

        message_array++;
    }

    return shaSuccess;
}

/*
 *  SHA1ProcessMessageBlock
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
static void SHA1ProcessMessageBlock(SHA1Context *context)
{
    const UINT32 K[] =    {       /* Constants defined in SHA-1   */
                            0x5A827999,
                            0x6ED9EBA1,
                            0x8F1BBCDC,
                            0xCA62C1D6
                          };
    INT32       t;                      /* Loop counter                */
    UINT32      temp;              /* Temporary word value        */
    UINT32      W[80];             /* Word sequence               */
    UINT32      A, B, C, D, E;     /* Word buffers                */

    /*
     *  Initialize the first 16 words in the array W
     */
    for(t = 0; t < 16; t++)
    {
        W[t]  = context->Message_Block[t * 4] << 24;
        W[t] |= context->Message_Block[t * 4 + 1] << 16;
        W[t] |= context->Message_Block[t * 4 + 2] << 8;
        W[t] |= context->Message_Block[t * 4 + 3];
    }

    for (t = 16; t < 80; t++)
	{
       W[t] = SHA1CircularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
	}

    A = context->Intermediate_Hash[0];
    B = context->Intermediate_Hash[1];
    C = context->Intermediate_Hash[2];
    D = context->Intermediate_Hash[3];
    E = context->Intermediate_Hash[4];

    for (t = 0; t < 20; t++)
	{
        temp =  SHA1CircularShift(5,A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        E = D;
		D = C;
        C = SHA1CircularShift(30,B);

		B = A;
        A = temp;
    }

    for (t = 20; t < 40; t++)
    {
        temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    for (t = 40; t < 60; t++)
    {
        temp = SHA1CircularShift(5,A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    for (t = 60; t < 80; t++)
    {
        temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
	}

    context->Intermediate_Hash[0] += A;
    context->Intermediate_Hash[1] += B;
    context->Intermediate_Hash[2] += C;
    context->Intermediate_Hash[3] += D;
    context->Intermediate_Hash[4] += E;

    context->Message_Block_Index = 0;
}

/*
 *  SHA1PadMessage
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
static void SHA1PadMessage(SHA1Context *context)
{
    /*
     *  Check to see if the current message block is too small to hold
     *  the initial padding bits and length.  If so, we will pad the
     *  block, process it, and then continue padding into a second
     *  block.
     */
    if (context->Message_Block_Index > 55)
    {
        if (context->Message_Block_Index < 64)
        {
        context->Message_Block[context->Message_Block_Index++] = 0x80;
        }
        
        while(context->Message_Block_Index < 64)
        {
            context->Message_Block[context->Message_Block_Index++] = 0;
        }

        SHA1ProcessMessageBlock(context);

        while(context->Message_Block_Index < 56)
        {
            context->Message_Block[context->Message_Block_Index++] = 0;
        }
    }
    else
    {
        if (context->Message_Block_Index < 64)
        {
        context->Message_Block[context->Message_Block_Index++] = 0x80;
        }
        
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

    SHA1ProcessMessageBlock(context);
}

//-----------------------------------------------------------------------------
/** X_CalculateSHA1() calculate SHA-1 from flash.
 *  @param pu1MessageDigest For putting the calculated SHA-1 result.
 *  @param StartAddr flash offset
 *  @param u4Size flash size
 *  @retval 0 suceess, others are error code
 */
//-----------------------------------------------------------------------------
int X_CalculateSHA1(UINT8 *pu1MessageDigest, UINT32 StartAddr, UINT32 u4Size)
{
    int err;
    
#ifdef CC_USE_HW_SHA1
    MD_PARAM_T rParam;
    UINT32 u4BufStart;

    GCPU_Init(0);

    if((StartAddr == 0) || (u4Size == 0))
    {
        return -1;
    }

    if(((StartAddr & 0xf0) >> 4) % 4 != 0)
{
        return -1;
    }

    rParam.u4SrcStartAddr = (UINT32)StartAddr;
    rParam.u4SrcBufStart = GCPU_LINER_BUFFER_START(StartAddr);
    rParam.u4SrcBufEnd = GCPU_LINER_BUFFER_END(StartAddr + u4Size);
    rParam.u4DatLen = u4Size;
    rParam.fgFirstPacket = TRUE;
    rParam.fgLastPacket = TRUE;
    rParam.u8BitCnt = 0;

    err = GCPU_Cmd(0, GCPU_SHA_1, &rParam);
    
    memcpy(pu1MessageDigest, rParam.au1Hash, 160/8);
#else
    SHA1Context sha;

    err = SHA1Reset(&sha);
    if (err)
    {
        //CHIP_DisplayString("SHA1Reset Error.\n");
    }

    err = SHA1Input(&sha, (const unsigned char *)StartAddr, u4Size);
    if (err)
    {
        //CHIP_DisplayString("SHA1Input Error.\n");
    }

    err = SHA1Result(&sha, pu1MessageDigest);
    if (err)
    {
        //CHIP_DisplayString("SHA1Result Error.\n");
    }
#endif

    return err;
}
#endif  // CC_USE_SHA1


#if defined(CC_USE_CHKSUM)
int x_chksum(UINT8 *pu1MessageDigest, UINT32 u4StartAddr, UINT32 u4Size)
{
    UINT32 u4CheckSum = 0;
    UINT32 i;

    for (i = 0; i < u4Size; i+=4)
    {
        u4CheckSum += *(UINT32 *)(u4StartAddr + i);
    }

    memcpy(pu1MessageDigest, &u4CheckSum, 4);

    return 0;
}
#endif


#define DWORDSWAP(u4Tmp) (((u4Tmp & 0xff) << 24) | ((u4Tmp & 0xff00) << 8) | ((u4Tmp & 0xff0000) >> 8) | ((u4Tmp & 0xff000000) >> 24))

BOOL DataSwap(UINT32 *pu4Dest, UINT32 *pu4Src, UINT32 u4Size, UINT32 u4Mode)
{
    INT32 i = 0;
    UINT32 u4Tmp;

    if (u4Mode == 0)
    {
        for (i = 0; i < u4Size; i++) //memcpy
        {
            *(pu4Dest + i) = *(pu4Src + i);
        }
    }
    else if (u4Mode == 1) //Endien Change
    {
        for (i = 0; i < u4Size; i++)
        {
            u4Tmp = *(pu4Src + i);
            u4Tmp = DWORDSWAP(u4Tmp);
            *(pu4Dest + i) = u4Tmp;
        }
    }
    else if (u4Mode == 2) //Head Swap
    {
        for (i = 0; i < u4Size; i++)
        {
            *(pu4Dest + u4Size - 1 - i) = *(pu4Src + i);
        }
    }
    else if (u4Mode == 3) //Head Swap + Endien Change
    {
        for (i = 0; i < u4Size; i++)
        {
            u4Tmp = *(pu4Src + i);
            u4Tmp = DWORDSWAP(u4Tmp);
            *(pu4Dest + u4Size - 1 - i) = u4Tmp;
        }
    }

    return TRUE;
}

void ExtractPKCSblock(UINT32 au4CheckSum[], UINT8 au1ExtractMsg[])
{
	UINT8 au1Block[256];
    INT32 i;

    memset(au1Block, 0, 256);
    DataSwap((UINT32 *)au1Block, au4CheckSum, 64, 3);
    if (au1Block[0] == 0x02 && au1Block[1] == 0x00)
    {
		memcpy(au1ExtractMsg, &au1Block[256-32], 32);
	//	dumpBinary(au1Block,64,"au1Block ");
    }
    else
    {
        printf("ERROR: Bad data entryption.\n");
    }
}

//---------------------------------------------------------------------
// Function    : RSA
// Description : Verify signature by RSA
// Parameter   :
//		pu1Signature	[in]: The signature must be 2048bit
//      pu4CheckSum     [in/out]: Checksum value must be 2048bit
// Return      : verify OK or not
//---------------------------------------------------------------------
INT32 RSADecryption(UINT32 *pu1Signature, UINT32 *pu4PublicKey, UINT32 *pu4CheckSum)
{
    mpi s;
    mpi n;
    mpi t1, t2;
    UINT32 t1_4096[128];
    UINT32 t2_4096[128];

    s.s = 1;
    s.n = 64;
    s.p = pu1Signature;
    n.s = 1;
    n.n = 64;
    n.p = pu4PublicKey;
    t1.s = 1;
    t1.n = 128;
    t1.p = t1_4096;
    t2.s = 1;
    t2.n = 128;
    t2.p = t2_4096;

    // m = s ^ 3 mod n
    //y = s * s mod n
    mpi_mul_mpi(&t1, &s, &s);
    mpi_mod_mpi(&t2, &t1, &n);

    //y = y * s mode n
    mpi_mul_mpi(&t1, &t2, &s);
    mpi_mod_mpi(&t2, &t1, &n);

    memcpy(pu4CheckSum, t2_4096, 256);

    return VERIFY_OK;
}

void sig_authetication(UINT32 u4StartAddr, UINT32 u4Size, UINT32 *pu4Signature, UINT32 *pu4EncryptedKey)
{
    BYTE au1ExtractMsg[SHA1HashSize];
    BYTE au1MessageDigest[SHA1HashSize];
    LDR_ENV_T* prLdrEnv = _prLdrEnv;

    if (SECURE_BOOT_EFUSE == SECURE_BOOT)
    {
        //UINT32 au4MiscBlock[64]; // 2048bit key
        UINT32 au4CheckSum[SIGNATURE_SIZE];
        INT32 i4Ret = 0;

        memcpy((void *)au4CheckSum, (void *)prLdrEnv->au4CustKey, sizeof(prLdrEnv->au4CustKey));

        // RSA verification via au4Signature and au4CheckSum(public key)
        // Then save the decrypted PKCS block into au4CheckSum
        i4Ret = RSADecryption(pu4Signature, au4CheckSum, au4CheckSum);
        if (i4Ret != VERIFY_OK)
        {
            goto ErrorHandler;
        }

        ExtractPKCSblock(au4CheckSum, au1ExtractMsg);

        memset(au1MessageDigest, 0, SHA1HashSize);
        
#if defined(CC_USE_SHA1)
        X_CalculateSHA1(au1MessageDigest, u4StartAddr, u4Size);
#elif defined(CC_USE_MD5)
        X_CalculateMD5(au1MessageDigest, u4StartAddr, u4Size);
#elif defined(CC_USE_SHA256)
	X_CalculateSHA256(au1MessageDigest, u4StartAddr, u4Size);
#else
        x_chksum(au1MessageDigest, u4StartAddr, u4Size);

#endif

        if (memcmp(au1ExtractMsg, au1MessageDigest, SHA1HashSize) != 0)
        {
            printf("Checking message digest failed.\n");
            goto ErrorHandler;
            }
            else
            {
            printf("Loader succeeded in signature verification\n");
                return;
            }
        }
        else
    {
        return;
    }

ErrorHandler:
    while(1);
    return;   //Should never come here
}


// A1 secure flow----------------------------------------------------------------------------------------------------------------------
#ifdef CC_A1_SECURE_BOOT

#define BUFFER_ADDRESS  0x4000000
#define SIG_SIZE        256
#define PAD_ALIGN       4095 //4k

//#define SECURE_DEBUG

int gSecureFlag = 0;
extern int do_cp2ram(cmd_tbl_t *, int, int, char *[]);

static void dumpBinary(unsigned char* bin, int size, char* name)
{
#ifdef SECURE_DEBUG
    int i = 0;
    printf("%s=", name);

    for (i = 0; i < size; i++)
    {
        if (i%16 == 0)
        {
            printf("\n");
        }
        printf("%02x ", bin[i]);
    }

    printf("\n");
#endif
}

int getFullVerifyOTP(void)
{
#ifdef FULL_VERIFY_OFF
    printf("getOTP: return 0 forcely by FULL_VERIFY_OFF\n");
    return 0;
#else
   // printf("getOTP: value = 0x%x\n", BIM_READ32(0x664));
   // return (BIM_READ32(0x664) & 0x1);
   
return 1;
#endif
}

int setFullVerifyOTP(void)
{
    UINT32 u4Val;

    // switch bus clock to xtal clock.
    u4Val = CKGEN_READ32(0x208);
    CKGEN_WRITE32(0x208, u4Val & ~0x700);

    // set key, enable in-system e-fuse blowing function.
    BIM_WRITE32(0x688, 0x716);

    // set counter for e-fuse programming timing requirement.
    BIM_WRITE32(0x6B4, 0x80CD9020);

    // turn on burn mode.
    BIM_WRITE32(0x6B0, 0x1);

    // delay 5us for power
    udelay(5);

    // trigger
    BIM_WRITE32(0x6B0, 0x11);

    // polling bit 4, indicate blowing e-fuse is completed.
    while (BIM_READ32(0x6B0) & 0x10);

    // set key, disable in-system e-fuse blowing function.
    BIM_WRITE32(0x688, 0x0);

    // refresh e-fuse.
    BIM_WRITE32(0x6B4, 0x0);
    BIM_WRITE32(0x6B4, 0x80000000);

    // Restore bus clock.
    CKGEN_WRITE32(0x208, u4Val);

    printf("Set partial verification OTP done, 0x%08X!\n", BIM_READ32(0x664));
}

// return 0 -> OK, return -1 -> fail
int copyFlash(const char* szPartName, unsigned char* pu1DestDram, unsigned int u4FlashOffset, unsigned int u4Size)
{
    char* pargs[5];
    char* local_args[4][32];

    sprintf(local_args[0], "%s",   szPartName);
    sprintf(local_args[1], "0x%x", pu1DestDram);
    sprintf(local_args[2], "0x%x", u4FlashOffset);
    sprintf(local_args[3], "0x%x", u4Size);

    pargs[0] = 0;
    pargs[1] = local_args[0];
    pargs[2] = local_args[1];
    pargs[3] = local_args[2];
    pargs[4] = local_args[3];

    return do_cp2ram(NULL, 0, 5, pargs);
}

// return 0 -> OK, return -1 -> fail
static int getFlashPartFileSize(const char* szPartName, unsigned int* pu4Size)
{
    struct partition_info* mpi = get_partition_by_name(szPartName);

    if (mpi == NULL)
    {
        printf("^R^failed to get partition free\n");
        return -1;
    }

    if (mpi->valid == NO)
    {
        printf("^R^Partition is Not valid! => Skip!");
        return -1;
    }

    if (!mpi->filesize)
    {
        printf("^g^File image size is Zero, Using partition size!!");
        printf("sizeof mpi->size : %d", sizeof(mpi->size));
        *pu4Size = mpi->size;
    }
    else
    {
        *pu4Size = mpi->filesize;
    }

    return 0;
}

//unsigned int random(void)
//{
//    return BIM_READ32(REG_RW_TIMER2_LOW);
//}
UINT32 au4CheckSumTemp[SIGNATURE_SIZE];
char LoadVesion[8];

int verifySignature(unsigned int u4StartAddr, unsigned int u4Size, unsigned char *pu1EncryptedSignature)
{
    BYTE au1ExtractMsg[32];
    BYTE au1MessageDigest[32];
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
#ifdef 	SECURE_DEBUG

    UINT8 au4CustKey[256] = {
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


    printf("SECURE_BOOT_EFUSE : 0x%x\n", SECURE_BOOT_EFUSE);
    printf("SECURE_BOOT : 0x%x\n", SECURE_BOOT);

    if (SECURE_BOOT_EFUSE == SECURE_BOOT)
    {
        INT32 i4Ret = -1;
		static INT32 IsAlradyHaveKey = 0;
		
		int i = 0;
		UINT32 au4CheckSum[SIGNATURE_SIZE];
#ifdef SECURE_DEBUG
        printf("verifySignature u4StartAddr=%x, u4Size=%x\n", u4StartAddr, u4Size);
#endif

#ifndef SECURE_DEBUG
	if (!IsAlradyHaveKey)
		{
        	memcpy((void*)au4CheckSumTemp, (void*)prLdrEnv->au4CustKey, sizeof(prLdrEnv->au4CustKey));
        	memcpy((void*)LoadVesion, (void*)prLdrEnv->LoadVesion, sizeof(prLdrEnv->LoadVesion));
			IsAlradyHaveKey = 1;
		}
			
#if 1//def SECURE_DEBUG
			
				for (i = 0; i < 8; i++)
				{

					printf("%02x ", LoadVesion[i]);
				}
			
				printf("\n");
#endif
        memcpy((void*)au4CheckSum, (void*)au4CheckSumTemp, sizeof(au4CheckSum));
#else  //use vendor public key
        memcpy((void*)au4CheckSum, (void*)au4CustKey, 256);

#endif
        dumpBinary((unsigned char*)pu1EncryptedSignature, SIG_SIZE, "encrypted signature");
        dumpBinary((unsigned char*)au4CheckSum, SIG_SIZE, "public key");
        dumpBinary((unsigned char*)u4StartAddr, 4096, "signed image");

        // RSA verification via au4Signature and au4CheckSum(public key)
        // Then save the decrypted PKCS block into au4CheckSum
        i4Ret = RSADecryption((UINT32*)pu1EncryptedSignature, au4CheckSum, au4CheckSum);
        if (i4Ret != VERIFY_OK)
        {
            printf("RSADecryption error\n");
            return -2;
        }

        ExtractPKCSblock(au4CheckSum, au1ExtractMsg);
        dumpBinary((unsigned char*)au1ExtractMsg, SHA1HashSize, "au1ExtractMsg");

        memset(au1MessageDigest, 0, SHA1HashSize);

#if defined(CC_USE_SHA1)
        X_CalculateSHA1(au1MessageDigest, u4StartAddr, u4Size);
#elif defined(CC_USE_SHA256)
        X_CalculateSHA256(au1MessageDigest, u4StartAddr, u4Size);
#elif defined(CC_USE_MD5)
        X_CalculateMD5(au1MessageDigest, u4StartAddr, u4Size);

#else
        x_chksum(au1MessageDigest, u4StartAddr, u4Size);
#endif

        dumpBinary((unsigned char*)au1MessageDigest, SHA1HashSize, "au1MessageDigest");

        if (memcmp(au1ExtractMsg, au1MessageDigest, SHA1HashSize) != 0)
        {
            printf("signature check fail!\n");
           // writeFullVerifyOTP();

            while(1);
            return -1;
        }
        else
        {
            printf("SOK\n");
            return 0;
        }
    }
    else
    {
	    printf("Secure boot\n");
        return 0;
    }
}

int verifyPartition(const char *szPartName, ulong addr, unsigned int preloaded)
{
    unsigned int offset = 0, image_size = 0;
    unsigned char *pu1Image;
    int ret = -1;
    unsigned char au1EncryptedSignature[SIG_SIZE];
	signature_header header;
    unsigned char au1Frag[8];

	printf("full verify ~~ \n");
	
    // 0. check if partition exist
    if (getFlashPartFileSize(szPartName, &image_size) != 0)
    {
        printf("partition name doesn't exist\n");
        return -1;
    }
	printf("pname = %s \n", szPartName);
	printf("image_size = 0x%x \n", image_size);

    if (preloaded)
    {
        pu1Image = (unsigned char *)addr;
    }
    else
    {
        // 1. copy image
        pu1Image = (unsigned char *)addr;
        if (copyFlash(szPartName, pu1Image, 0, image_size) != 0)
        {
            printf("copy image fail\n");
			while(1);
            return -1;
        }
    }
	
	image_size -=8;
		// 2. get fragment 
		 memcpy((void*)&header, (void*)(pu1Image+image_size), 8);
		//u4FragNum = au1Frag[0]|(au1Frag[1]<<8)|(au1Frag[2]<<16)|(au1Frag[3]<<24);
			//u4FragSize = au1Frag[4]|(au1Frag[5]<<8)|(au1Frag[6]<<16)|(au1Frag[7]<<24);
		dumpBinary((unsigned char*)au1Frag, 8, "framement parameter");
		
		printf("header.frag_num  = %d\n", header.frag_num);
		printf("header.frag_size  = %d\n", header.frag_size);

	u4FragNum = header.frag_num;
	u4FragSize = header.frag_size ;

    // 2. get encrypted signature
     memcpy((void*)au1EncryptedSignature, (void*)(pu1Image+image_size-SIG_SIZE), SIG_SIZE);

    // 3. verify signature
#ifdef SIGN_USE_PARTIAL
    ret = verifySignature((unsigned int)pu1Image, image_size - (u4FragNum+1)*(SIG_SIZE), au1EncryptedSignature);
#else
	ret = verifySignature((unsigned int)pu1Image, image_size - SIG_SIZE, au1EncryptedSignature);
#endif

    return ret;
}
int sbverifyPartition(unsigned long long offset,unsigned int image_size)
{
    unsigned char *pu1Image;
    int ret = -1;
    unsigned char au1EncryptedSignature[SIG_SIZE];
	ulong 		addr = 0x8000000;
    unsigned char au1Frag[8];

	printf("full verify ~~ \n");

        // 1. copy image
        pu1Image = (unsigned char *)addr;
        if (emmc_read(offset, image_size,pu1Image) != 0)
        {
            printf("copy image fail\n");
            return -1;
        }
		
		image_size -=8;
		// 2. get fragment 
		 memcpy((void*)au1Frag, (void*)(pu1Image+image_size), 8);
		u4FragNum = au1Frag[0]|(au1Frag[1]<<8)|(au1Frag[2]<<16)|(au1Frag[3]<<24);
		u4FragSize = au1Frag[4]|(au1Frag[5]<<8)|(au1Frag[6]<<16)|(au1Frag[7]<<24);
        dumpBinary((unsigned char*)au1Frag, 8, "framement parameter");
		

    // 2. get encrypted signature
     memcpy((void*)au1EncryptedSignature, (void*)(pu1Image+image_size-SIG_SIZE), SIG_SIZE);

    // 3. verify signature
#ifdef SIGN_USE_PARTIAL
    ret = verifySignature((unsigned int)pu1Image, image_size - (u4FragNum+1)*(SIG_SIZE), au1EncryptedSignature);
#else
	ret = verifySignature((unsigned int)pu1Image, image_size - SIG_SIZE, au1EncryptedSignature);
#endif

    return ret;
}
int sbverifyPartialPartition(unsigned long long offset,unsigned int image_size)
{
    unsigned int i = 0, seed = 0, group_num;
    int ret = -1;
    unsigned char *pu1AllFrag;
    unsigned char au1EncryptedSignature[SIG_SIZE];
	ulong 		addr = 0x8000000;
    unsigned char au1test[512];
    unsigned char au1Frag[8];


	printf("partial verify ~~ \n");
	#if 1
	ret = emmc_read((unsigned long)offset + image_size - 512, 512, au1test);
	if (ret)
	{
		printf("block read failed..\n");
		return 1;
	}
	// 2. get fragment 
	 memcpy((void*)au1Frag, (void*)(au1test+512 -8), 8);
	u4FragNum = au1Frag[0]|(au1Frag[1]<<8)|(au1Frag[2]<<16)|(au1Frag[3]<<24);
		u4FragSize = au1Frag[4]|(au1Frag[5]<<8)|(au1Frag[6]<<16)|(au1Frag[7]<<24);
	dumpBinary((unsigned char*)au1Frag, 8, "framement parameter");
#else
u4FragNum = 20;
u4FragSize = 4096;

#endif
	image_size -=8;

	// 2. copy u4FragSize(4K) every u4FragSize*u4FragNum bytes
	printf("u4FragNum  = %d\n", u4FragNum);
	printf("u4FragSize  = %d\n", u4FragSize);
    // if image size is smaller less than group size, do full verification
    group_num = (image_size - ((u4FragNum+1)*256)) / (u4FragNum*u4FragSize);
	
	printf("group_num = %d\n",group_num);
    if (group_num == 0)
    {
        return sbverifyPartition(offset,image_size+8);
    }

    // 1. generate a random number and allocate buffer
    seed = random() % u4FragNum;
    printf("part=%d\n", seed);

	pu1AllFrag = (unsigned char *)addr;

    if (pu1AllFrag == 0)
    {
        printf("malloc fail\n");
        return -1;
    }

	printf("[%d] read fragments start\n", readMsTicks());
	
	{
	    for (i = 0; i < group_num; i++)
	    {
	        // this is flash offset
			ret = emmc_read((unsigned long)( offset + ((i*u4FragNum + seed) * u4FragSize)), u4FragSize, pu1AllFrag+(u4FragSize*i));
			if (ret)
			{
				printf("block read failed..\n");
				return 1;
			}
	    }
	}
	printf("[%d]:%s read fragments end\n", readMsTicks());

	// 3. get encrypted signature
	{
	    offset = image_size - SIG_SIZE*(u4FragNum + 1 - seed);

		ret = emmc_read((unsigned long)(offset + ((i*u4FragNum + seed) * u4FragSize)), SIG_SIZE, au1EncryptedSignature);
		if (ret)
		{
			printf("block read failed..\n");
			return 1;
		}
	}

	dumpBinary((unsigned char*)pu1AllFrag, u4FragSize, "before verfiysiganture start");

    // 4. verify signature
    ret = verifySignature((unsigned int)pu1AllFrag, u4FragSize*group_num, au1EncryptedSignature);

    return ret;
}

int verifyPartialPartition(const char *szPartName, ulong addr, unsigned int preloaded)
{
    unsigned int i = 0, offset = 0, seed = 0, image_size = 0, group_num;
    int ret = -1;
    unsigned char *pu1AllFrag;
    unsigned char au1EncryptedSignature[SIG_SIZE];
	signature_header header;
    unsigned char au1test[512];
	struct partition_info *pi = NULL;
    unsigned char au1Frag[8];

	pi = get_used_partition(szPartName);

	printf("partial verify ~~ \n");
    // 0. check if partition exist
    if (getFlashPartFileSize(szPartName, &image_size) != 0)
    {
        printf("partition name doesn't exist\n");
        return -1;
    }
	#if 1
	ret = emmc_read((unsigned long)pi->offset + image_size - 512, 512, au1test);
	if (ret)
	{
		printf("block read failed..\n");
		while(1);
		return 1;
	}
	// 2. get fragment 
	 memcpy((void*)&header, (void*)(au1test+512 -8), 8);
	//u4FragNum = au1Frag[0]|(au1Frag[1]<<8)|(au1Frag[2]<<16)|(au1Frag[3]<<24);
		//u4FragSize = au1Frag[4]|(au1Frag[5]<<8)|(au1Frag[6]<<16)|(au1Frag[7]<<24);
	dumpBinary((unsigned char*)au1Frag, 8, "framement parameter");
	u4FragNum = header.frag_num;
	u4FragSize = header.frag_size;
	printf("header.frag_num  = %d\n", header.frag_num);
	printf("header.frag_size  = %d\n", header.frag_size);
#else
u4FragNum = 20;
u4FragSize = 4096;

#endif
	image_size -=8;

	// 2. copy u4FragSize(4K) every u4FragSize*u4FragNum bytes
	printf("preloaded = %d\n", preloaded);
	printf("u4FragNum  = %d\n", u4FragNum);
	printf("u4FragSize  = %d\n", u4FragSize);
	printf("image_size  = %d\n", image_size);
    // if image size is smaller less than group size, do full verification
    group_num = (image_size - ((u4FragNum+1)*256)) / (u4FragNum*u4FragSize);
	
	printf("group_num = %d\n",group_num);
    if (group_num == 0)
    {
        return verifyPartition(szPartName, addr, preloaded);
    }

    // 1. generate a random number and allocate buffer
    seed = random() % u4FragNum;
    printf("part=%d\n", seed);

	pu1AllFrag = (unsigned char *)addr;

    if (pu1AllFrag == 0)
    {
        printf("malloc fail\n");
        return -1;
    }

	printf("[%d]:%s read fragments start\n", readMsTicks(), pi->name);
	if (preloaded)
	{
		for (i = 0; i < group_num; i++)
	    {
			offset = addr + (i*u4FragNum + seed) * u4FragSize; //get sigend partial fragment
			memcpy((void *)(pu1AllFrag+(u4FragSize*i)),(void *)offset,u4FragSize); //copy fragment to buffer
		}
	}
	else
	{
	    for (i = 0; i < group_num; i++)
	    {
	        // this is flash offset
	        offset = (i*u4FragNum + seed) * u4FragSize;
			ret = emmc_read((unsigned long)pi->offset + offset, u4FragSize, pu1AllFrag+(u4FragSize*i));
			if (ret)
			{
				printf("block read failed..\n");
				
				while(1);
				return 1;
			}
	    }
	}
	printf("[%d]:%s read fragments end\n", readMsTicks(), pi->name);

	// 3. get encrypted signature
	if (preloaded)
	{
		offset = addr + image_size - SIG_SIZE*(u4FragNum + 1 - seed);
		memcpy((void*)au1EncryptedSignature, (void*)offset, SIG_SIZE);
	}
	else
	{
	    offset = image_size - SIG_SIZE*(u4FragNum + 1 - seed);

		ret = emmc_read((unsigned long)pi->offset + offset, SIG_SIZE, au1EncryptedSignature);
		if (ret)
		{
			printf("block read failed..\n");
			
			while(1);
			return 1;
		}
	}

	dumpBinary((unsigned char*)pu1AllFrag, u4FragSize, "before verfiysiganture start");

    // 4. verify signature
    ret = verifySignature((unsigned int)pu1AllFrag, u4FragSize*group_num, au1EncryptedSignature);

    return ret;
}

int do_verification(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]) // should be used integer type function
{
	char pname[20] = {0,};
	ulong addr;
	unsigned char partial_verify = 0;

	if (argc != 4)
	{
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	partial_verify = simple_strtoul(argv[1], NULL, 16);
	strcpy(pname, argv[2]);
	printf("pname = %s \n", pname);
	addr = simple_strtoul(argv[3], NULL, 16);
	printf("addr = 0x%x \n", addr);
	printf("[%d]:%s verification start \n", readMsTicks(), pname);

	if (!strcmp("kernel", pname))
    {
		gSecureFlag |= SECURE_FLG_KERNEL;
    }
	else
    {
		gSecureFlag |= SECURE_FLG_OTHERS;
    }

#ifdef SIGN_USE_PARTIAL
		if (getFullVerifyOTP())
		{
			verifyPartition(pname, addr, 1);
		}
		else
		{
			if (!strcmp("kernel", pname))
		    {
				verifyPartition(pname, addr, 1);
		    }
			else
		    {
				verifyPartialPartition(pname, addr, 1);
		    }
		}
#else
		if (partial_verify)
	    {
			verifyPartialPartition(pname, addr, 1);
	    }
		else
	    {
			verifyPartition(pname, addr, 1);
	    }
#endif

	printf("[%d]:%s verification end\n", readMsTicks(), pname);
	return 0;
}

U_BOOT_CMD(
	verification,	4,	0,	do_verification,
	"verification\t- cp2ram 1 partition to ram and verification signature\n",
        "empty help text for verification\n"
);


extern int DDI_OTP_WR_Enable(uchar ctrl);
int do_controlFullVerifyOtp(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]) // should be used integer type function
{
	int otpval;
	int set;

	if (argc != 2)
	{
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	set = simple_strtoul(argv[1], NULL, 16);
	if ( set != 0 && set != 1)
	{
		printf (" 0 or 1 is available\n");
	}

	if(set)
	{
		printf("start set full verify OTP\n");
		DDI_OTP_WR_Enable(0);
		udelay(50000);
		setFullVerifyOTP();
		udelay(10000);
		DDI_OTP_WR_Enable(1);
		printf("end set full verify OTP\n");
	}
	else
	{
		otpval = getFullVerifyOTP();
		printf("full verify OTP value = %d\n", otpval);
	}

	return 0;
}

U_BOOT_CMD(
     ctrlotp,   2,  0,  do_controlFullVerifyOtp,
     "ctrlotp\t- [0: get|1: set]set or get full verify otp flag\n",
     "empty help text for ctrlotp\n"
);

typedef struct
{
	const char* part_name;
	uint32_t	mode;
} verify_list_t;

/* Only For Test in Citrix */
static verify_list_t verify_list[] =
{
	{"rootfs", BOOT_COLD},
	{"swue",		BOOT_COLD | BOOT_SNAPSHOT},
	{"tzfw",		BOOT_COLD | BOOT_SNAPSHOT},
	{"tvservice",	BOOT_COLD },
	{"otncabi",		BOOT_COLD},
	{"otycabi",		BOOT_COLD},
	{"smartkey",	BOOT_COLD},
};

static int is_verify_part(int idx, int boot_mode)
{
	if( boot_mode & verify_list[idx].mode)
	{
		return 1;
	}

	return 0;
}
int sb_verify_image(const char *szPartName, ulong addr, unsigned int flag)
{
	if(flag == 1)
		return verifyPartition(szPartName,addr, 0);
	else
		return verifyPartialPartition(szPartName,addr, 0);
}

extern int do_cp2ramz (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
int verify_apps(int boot_mode)
{
    int         idx;
    uint64_t    offset;
    unsigned int    filesize, imgsize;
    uint8_t*    sign;
    uint32_t    sign_size;
    uint32_t    flags = 0;
    uint32_t    loop_cnt;
    int         IsFullverify = 0;
    int         ret = 0;
    int         retry_cnt = 0;
	ulong 		addr = 0x8000000;

    struct partition_info * pi = NULL;
    

	loop_cnt = sizeof(verify_list)/sizeof(verify_list_t);

    IsFullverify = DDI_NVM_GetFullVerifyFlag();

    if (IsFullverify)
        flags = 1;
    else
        flags = 0;

    printf("\033[0;32m flags for verifying application is 0x%x \033[0m\n",flags);


    for(idx=0; idx<loop_cnt; idx++)
    {
        if( IsFullverify || is_verify_part(idx, boot_mode) )
        {
            pi = get_used_partition(verify_list[idx].part_name);

//          if( !get_swumode() && !strcmp(verify_list[idx].part_name,"swue") ) //not swum on case
//          {
//              printf("\033[0;32m skip swue verification ... \033[0m\n");
//              continue;
//          }
            if(get_swumode())
            	{
            		if(strcmp(verify_list[idx].part_name,"swue") )
            			{
            				 printf("\033[0;32m skip %s verification ... \033[0m\n",verify_list[idx].part_name);
				             continue;
            			}
            	}
			else
				{
					if(!strcmp(verify_list[idx].part_name,"swue") )
						{
							printf("\033[0;32m skip swue verification ... \033[0m\n");
              				continue;
						}
				}
            printf("\033[0;32m[%d]Verifying image in the '%s' partition idx[%d] \033[0m\n", readMsTicks(), pi->name, idx);
            printf("\033[0;32m[%d] %s start \n", readMsTicks(), __FUNCTION__);

            offset = pi->offset;
            filesize = pi->filesize;
            imgsize = filesize;                 
            printf("\033[0;32m[%d]Verifying image offset 0x%x  partition size [%d] \033[0m\n", offset,filesize);

            do {            
                ret = sb_verify_image(pi->name, addr, flags);
                retry_cnt++;
                printf("\033[0;32msb_verify_application check %d time \033[0m\n",retry_cnt);
                if (ret < 0)
                    udelay(200000);
            } while (ret < 0 && retry_cnt < VERIFY_RETRY_MAX);

            if (ret < 0)
            {
                printf("\033[0;32m %s verification failed in %d check ... fullverify flag will be set and System will stop \033[0m\n",verify_list[idx].part_name, retry_cnt);
                DDI_NVM_SetFullVerifyFlag(1);
                goto verify_error;
            }

            retry_cnt = 0;      
            printf("\033[0;32m[%d]Completed... \033[0m\n",readMsTicks());

        }
    }

    printf("\033[0;32m Application integrity verified \033[0m\n");
    verify_done |= VERIFY_APPS_DONE;

	//Clear full verify flag on this point in resume mode
	if (IsFullverify)
	{
		if ( verify_done == VERIFY_APPS_DONE )
		{
			printf("\033[0;31m Full Verify is all ok, Full Verify will be clear to partial \033[0m\n");
			IsFullverify = 0;
			DDI_NVM_SetFullVerifyFlag(0);
		}	
	}
	
    return 0;

verify_error:
    printf("\033[0;32m Verify error !!!\033[0m\n");
    while(1);
    return -1;
}

#endif // CC_A1_SECURE_BOOT

#endif // CC_SECURE_ROM_BOOT
