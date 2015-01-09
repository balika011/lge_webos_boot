/*
   LZ4 - Fast LZ compression algorithm
   Copyright (C) 2011, Yann Collet.
   BSD License

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:

       * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above
   copyright notice, this list of conditions and the following disclaimer
   in the documentation and/or other materials provided with the
   distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//**************************************
// Compilation Directives
//**************************************
#if __STDC_VERSION__ >= 199901L
  /* "restrict" is a known keyword */
#else
#define restrict  // Disable restrict
#endif


//**************************************
// Includes
//**************************************
#include <common.h>
#include "lz4.h"


//**************************************
// Performance parameter
//**************************************
// Increasing this value improves compression ratio
// Lowering this value reduces memory usage
// Lowering may also improve speed, typically on reaching cache size limits (L1 32KB for Intel, 64KB for AMD)
// Memory usage formula for 32 bits systems : N->2^(N+2) Bytes (examples : 17 -> 512KB ; 12 -> 16KB)
#define HASH_LOG 12


//**************************************
// Basic Types
//**************************************
#if defined(_MSC_VER)    // Visual Studio does not support 'stdint' natively
#define BYTE	unsigned __int8
#define U16		unsigned __int16
#define U32		unsigned __int32
#define S32		__int32
#else
//#include <stdint.h>
#define BYTE	uint8_t
#define U16		uint16_t
#define U32		uint32_t
#define S32		int32_t
#endif


//**************************************
// Constants
//**************************************
#define MINMATCH 4
#define SKIPSTRENGTH 6
#define STACKLIMIT 13
#define HEAPMODE (HASH_LOG>STACKLIMIT)  // Defines if memory is allocated into the stack (local variable), or into the heap (malloc()).
#define COPYTOKEN 4
#define COPYLENGTH 8
#define LASTLITERALS 5
#define MFLIMIT (COPYLENGTH+MINMATCH)
#define MINLENGTH (MFLIMIT+1)

#define MAXD_LOG 16
#define MAX_DISTANCE ((1 << MAXD_LOG) - 1)

#define HASHTABLESIZE (1 << HASH_LOG)
#define HASH_MASK (HASHTABLESIZE - 1)

#define ML_BITS 4
#define ML_MASK ((1U<<ML_BITS)-1)
#define RUN_BITS (8-ML_BITS)
#define RUN_MASK ((1U<<RUN_BITS)-1)

#undef DEBUG
//#define DEBUG
/* note: prints function name for you */
#ifdef DEBUG
#define dprintf(fmt, args...)	printf("%s: " fmt, __FUNCTION__, ## args)
#else
#define dprintf(fmt, args...)
#endif	/* DEBUG */

//**************************************
// Local structures
//**************************************
struct refTables
{
	const BYTE* hashTable[HASHTABLESIZE];
};

#ifdef __GNUC__
#  define _PACKED __attribute__ ((packed))
#else
#  define _PACKED
#endif

typedef struct _U32_S
{
	U32 v;
} _PACKED U32_S;

typedef struct _U16_S
{
	U16 v;
} _PACKED U16_S;

#define A32(x) (((U32_S *)(x))->v)
#define A16(x) (((U16_S *)(x))->v)


//**************************************
// Macros
//**************************************
#define LZ4_HASH_FUNCTION(i)	(((i) * 2654435761U) >> ((MINMATCH*8)-HASH_LOG))
#define LZ4_HASH_VALUE(p)		LZ4_HASH_FUNCTION(A32(p))
#define LZ4_COPYPACKET(s,d)		A32(d) = A32(s); d+=4; s+=4; A32(d) = A32(s); d+=4; s+=4;
#define LZ4_WILDCOPY(s,d,e)		do { LZ4_COPYPACKET(s,d) } while (d<e);
#define LZ4_BLINDCOPY(s,d,l)	{ BYTE* e=d+l; LZ4_WILDCOPY(s,d,e); d=e; }


#if 0
//****************************
// Compression CODE
//****************************

int LZ4_compressCtx(void** ctx,
				 char* source,
				 char* dest,
				 int isize)
{
#if HEAPMODE
	struct refTables *srt = (struct refTables *) (*ctx);
	const BYTE** HashTable;
#else
	const BYTE* HashTable[HASHTABLESIZE] = {0};
#endif

	const BYTE* ip = (BYTE*) source;
	const BYTE* anchor = ip;
	const BYTE* const iend = ip + isize;
	const BYTE* const mflimit = iend - MFLIMIT;
#define matchlimit (iend - LASTLITERALS)

	BYTE* op = (BYTE*) dest;

	const size_t DeBruijnBytePos[32] = { 0, 0, 3, 0, 3, 1, 3, 0, 3, 2, 2, 1, 3, 2, 0, 1, 3, 3, 1, 2, 2, 2, 2, 0, 3, 1, 2, 0, 1, 0, 1, 1 };
	int len, length;
	const int skipStrength = SKIPSTRENGTH;
	U32 forwardH;


	// Init
	if (isize<MINLENGTH) goto _last_literals;
#if HEAPMODE
	if (*ctx == NULL)
	{
		srt = (struct refTables *) malloc ( sizeof(struct refTables) );
		*ctx = (void*) srt;
	}
	HashTable = srt->hashTable;
	memset((void*)HashTable, 0, sizeof(srt->hashTable));
#else
	(void) ctx;
#endif


	// First Byte
	HashTable[LZ4_HASH_VALUE(ip)] = ip;
	ip++; forwardH = LZ4_HASH_VALUE(ip);

	// Main Loop
    for ( ; ; )
	{
		int findMatchAttempts = (1U << skipStrength) + 3;
		const BYTE* forwardIp = ip;
		const BYTE* ref;
		BYTE* token;

		// Find a match
		do {
			U32 h = forwardH;
			int step = findMatchAttempts++ >> skipStrength;
			ip = forwardIp;
			forwardIp = ip + step;

			if (forwardIp > mflimit) { goto _last_literals; }

			forwardH = LZ4_HASH_VALUE(forwardIp);
			ref = HashTable[h];
			HashTable[h] = ip;

		} while ((ref < ip - MAX_DISTANCE) || (A32(ref) != A32(ip)));

		// Catch up
		while ((ip>anchor) && (ref>(BYTE*)source) && (ip[-1]==ref[-1])) { ip--; ref--; }

		// Encode Literal length
		length = ip - anchor;
		token = op++;
		if (length>=(int)RUN_MASK) { *token=(RUN_MASK<<ML_BITS); len = length-RUN_MASK; for(; len > 254 ; len-=255) *op++ = 255; *op++ = (BYTE)len; }
		else *token = (length<<ML_BITS);

		// Copy Literals
		LZ4_BLINDCOPY(anchor, op, length);


_next_match:
		// Encode Offset
		A16(op) = (ip-ref); op+=2;

		// Start Counting
		ip+=MINMATCH; ref+=MINMATCH;   // MinMatch verified
		anchor = ip;
		while (ip<matchlimit-3)
		{
			int diff = A32(ref) ^ A32(ip);
			if (!diff) { ip+=4; ref+=4; continue; }
			ip += DeBruijnBytePos[((U32)((diff & -diff) * 0x077CB531U)) >> 27];
			goto _endCount;
		}
		if ((ip<(matchlimit-1)) && (A16(ref) == A16(ip))) { ip+=2; ref+=2; }
		if ((ip<matchlimit) && (*ref == *ip)) ip++;
_endCount:
		len = (ip - anchor);

		// Encode MatchLength
		if (len>=(int)ML_MASK) { *token+=ML_MASK; len-=ML_MASK; for(; len > 509 ; len-=510) { *op++ = 255; *op++ = 255; } if (len > 254) { len-=255; *op++ = 255; } *op++ = (BYTE)len; }
		else *token += len;

		// Test end of chunk
		if (ip > mflimit) { anchor = ip;  break; }

		// Fill table
		HashTable[LZ4_HASH_VALUE(ip-2)] = ip-2;

		// Test next position
		ref = HashTable[LZ4_HASH_VALUE(ip)];
		HashTable[LZ4_HASH_VALUE(ip)] = ip;
		if ((ref > ip - (MAX_DISTANCE + 1)) && (A32(ref) == A32(ip))) { token = op++; *token=0; goto _next_match; }

		// Prepare next loop
		anchor = ip++;
		forwardH = LZ4_HASH_VALUE(ip);
	}

_last_literals:
	// Encode Last Literals
	{
		int lastRun = iend - anchor;
		if (lastRun>=(int)RUN_MASK) { *op++=(RUN_MASK<<ML_BITS); lastRun-=RUN_MASK; for(; lastRun > 254 ; lastRun-=255) *op++ = 255; *op++ = (BYTE) lastRun; }
		else *op++ = (lastRun<<ML_BITS);
		memcpy(op, anchor, iend - anchor);
		op += iend-anchor;
	}

	// End
	return (int) (((char*)op)-dest);
}



// Note : this function is valid only if isize < LZ4_64KLIMIT
#define LZ4_64KLIMIT ((1U<<16) + (MFLIMIT-1))
#define HASHLOG64K (HASH_LOG+1)
#define LZ4_HASH64K_FUNCTION(i)	(((i) * 2654435761U) >> ((MINMATCH*8)-HASHLOG64K))
#define LZ4_HASH64K_VALUE(p)	LZ4_HASH64K_FUNCTION(A32(p))
int LZ4_compress64kCtx(void** ctx,
				 char* source,
				 char* dest,
				 int isize)
{
#if HEAPMODE
	struct refTables *srt = (struct refTables *) (*ctx);
	U16* HashTable;
#else
	U16 HashTable[HASHTABLESIZE<<1] = {0};
#endif

	const BYTE* ip = (BYTE*) source;
	const BYTE* anchor = ip;
	const BYTE* const base = ip;
	const BYTE* const iend = ip + isize;
	const BYTE* const mflimit = iend - MFLIMIT;
#define matchlimit (iend - LASTLITERALS)

	BYTE* op = (BYTE*) dest;

	const size_t DeBruijnBytePos[32] = { 0, 0, 3, 0, 3, 1, 3, 0, 3, 2, 2, 1, 3, 2, 0, 1, 3, 3, 1, 2, 2, 2, 2, 0, 3, 1, 2, 0, 1, 0, 1, 1 };
	int len, length;
	const int skipStrength = SKIPSTRENGTH;
	U32 forwardH;


	// Init
	if (isize<MINLENGTH) goto _last_literals;
#if HEAPMODE
	if (*ctx == NULL)
	{
		srt = (struct refTables *) malloc ( sizeof(struct refTables) );
		*ctx = (void*) srt;
	}
	HashTable = (U16*)(srt->hashTable);
	memset((void*)HashTable, 0, sizeof(srt->hashTable));
#else
	(void) ctx;
#endif


	// First Byte
	ip++; forwardH = LZ4_HASH64K_VALUE(ip);

	// Main Loop
    for ( ; ; )
	{
		int findMatchAttempts = (1U << skipStrength) + 3;
		const BYTE* forwardIp = ip;
		const BYTE* ref;
		BYTE* token;

		// Find a match
		do {
			U32 h = forwardH;
			int step = findMatchAttempts++ >> skipStrength;
			ip = forwardIp;
			forwardIp = ip + step;

			if (forwardIp > mflimit) { goto _last_literals; }

			forwardH = LZ4_HASH64K_VALUE(forwardIp);
			ref = base + HashTable[h];
			HashTable[h] = ip - base;

		} while (A32(ref) != A32(ip));

		// Catch up
		while ((ip>anchor) && (ref>(BYTE*)source) && (ip[-1]==ref[-1])) { ip--; ref--; }

		// Encode Literal length
		length = ip - anchor;
		token = op++;
		if (length>=(int)RUN_MASK) { *token=(RUN_MASK<<ML_BITS); len = length-RUN_MASK; for(; len > 254 ; len-=255) *op++ = 255; *op++ = (BYTE)len; }
		else *token = (length<<ML_BITS);

		// Copy Literals
		LZ4_BLINDCOPY(anchor, op, length);


_next_match:
		// Encode Offset
		A16(op) = (ip-ref); op+=2;

		// Start Counting
		ip+=MINMATCH; ref+=MINMATCH;   // MinMatch verified
		anchor = ip;
		while (ip<matchlimit-3)
		{
			int diff = A32(ref) ^ A32(ip);
			if (!diff) { ip+=4; ref+=4; continue; }
			ip += DeBruijnBytePos[((U32)((diff & -diff) * 0x077CB531U)) >> 27];
			goto _endCount;
		}
		if ((ip<(matchlimit-1)) && (A16(ref) == A16(ip))) { ip+=2; ref+=2; }
		if ((ip<matchlimit) && (*ref == *ip)) ip++;
_endCount:
		len = (ip - anchor);

		// Encode MatchLength
		if (len>=(int)ML_MASK) { *token+=ML_MASK; len-=ML_MASK; for(; len > 509 ; len-=510) { *op++ = 255; *op++ = 255; } if (len > 254) { len-=255; *op++ = 255; } *op++ = (BYTE)len; }
		else *token += len;

		// Test end of chunk
		if (ip > mflimit) { anchor = ip;  break; }

		// Test next position
		ref = base + HashTable[LZ4_HASH64K_VALUE(ip)];
		HashTable[LZ4_HASH64K_VALUE(ip)] = ip - base;
		if (A32(ref) == A32(ip)) { token = op++; *token=0; goto _next_match; }

		// Prepare next loop
		anchor = ip++;
		forwardH = LZ4_HASH64K_VALUE(ip);
	}

_last_literals:
	// Encode Last Literals
	{
		int lastRun = iend - anchor;
		if (lastRun>=(int)RUN_MASK) { *op++=(RUN_MASK<<ML_BITS); lastRun-=RUN_MASK; for(; lastRun > 254 ; lastRun-=255) *op++ = 255; *op++ = (BYTE) lastRun; }
		else *op++ = (lastRun<<ML_BITS);
		memcpy(op, anchor, iend - anchor);
		op += iend-anchor;
	}

	// End
	return (int) (((char*)op)-dest);
}



int LZ4_compress(char* source,
				 char* dest,
				 int isize)
{
#if HEAPMODE
	void* ctx = malloc(sizeof(struct refTables));
	int result;
	if (isize < LZ4_64KLIMIT)
		result = LZ4_compress64kCtx(&ctx, source, dest, isize);
	else result = LZ4_compressCtx(&ctx, source, dest, isize);
	free(ctx);
	return result;
#else
	if (isize < (int)LZ4_64KLIMIT) return LZ4_compress64kCtx(NULL, source, dest, isize);
	return LZ4_compressCtx(NULL, source, dest, isize);
#endif
}

#endif


//****************************
// Decompression CODE
//****************************

// Note : The decoding functions LZ4_uncompress() and LZ4_uncompress_unknownOutputSize()
//		are safe against "buffer overflow" attack type
//		since they will *never* write outside of the provided output buffer :
//		they both check this condition *before* writing anything.
//		A corrupted packet however can make them *read* within the first 64K before the output buffer.

int LZ4_uncompress(char* source,
				 char* dest,
				 int osize)
{
	// Local Variables
	const BYTE* restrict ip = (const BYTE*) source;
	const BYTE* restrict ref;

	BYTE* restrict op = (BYTE*) dest;
	BYTE* const oend = op + osize;
	BYTE* cpy;

	BYTE token;

	U32	dec[4]={0, 3, 2, 3};
	int	len, length;

	// Main Loop
	while (1)
	{
		// get runlength
		token = *ip++;
		if ((length=(token>>ML_BITS)) == RUN_MASK)  { for (;(len=*ip++)==255;length+=255){} length += len; }

		// copy literals
		cpy = op+length;
		if (cpy>oend-COPYLENGTH)
		{
			if (cpy > oend) goto _output_error;
			memcpy(op, ip, length);
			ip += length;
			break;    // Necessarily EOF
		}
		LZ4_WILDCOPY(ip, op, cpy); ip -= (op-cpy); op = cpy;


		// get offset
		ref = cpy - A16(ip); ip+=2;

		// get matchlength
		if ((length=(token&ML_MASK)) == ML_MASK) { for (;*ip==255;length+=255) {ip++;} length += *ip++; }

		// copy repeated sequence
		if (op-ref<COPYTOKEN)
		{
			*op++ = *ref++;
			*op++ = *ref++;
			*op++ = *ref++;
			*op++ = *ref++;
			ref -= dec[op-ref];
			A32(op)=A32(ref);
		} else { A32(op)=A32(ref); op+=4; ref+=4; }
		cpy = op + length;
		if (cpy > oend-COPYLENGTH)
		{
			if (cpy > oend) goto _output_error;
			LZ4_WILDCOPY(ref, op, (oend-COPYLENGTH));
			while(op<cpy) *op++=*ref++;
			op=cpy;
			if (op == oend) break;    // Check EOF (should never happen, since last 5 bytes are supposed to be literals)
			continue;
		}
		LZ4_WILDCOPY(ref, op, cpy);
		op=cpy;		// correction
	}

	// end of decoding
	return (int) (((char*)ip)-source);

	// write overflow error detected
_output_error:
	return (int) (-(((char*)ip)-source));
}


int LZ4_uncompress_unknownOutputSize(
				char* source,
				char* dest,
				int isize,
				int maxOutputSize)
{
	// Local Variables
	const BYTE* restrict ip = (const BYTE*) source;
	const BYTE* const iend = ip + isize;
	const BYTE* restrict ref;

	BYTE* restrict op = (BYTE*) dest;
	BYTE* const oend = op + maxOutputSize;
	BYTE* cpy;

	BYTE token;

	U32	dec[4]={0, 3, 2, 3};
	int	len, length;


	// Main Loop
	while (ip<iend)
	{
		// get runlength
		token = *ip++;
		if ((length=(token>>ML_BITS)) == RUN_MASK)  { for (;(len=*ip++)==255;length+=255){} length += len; }

		// copy literals
		cpy = op+length;
		if (cpy>oend-COPYLENGTH)
		{
			if (cpy > oend) goto _output_error;
			memcpy(op, ip, length);
			op += length;
			break;    // Necessarily EOF
		}
		LZ4_WILDCOPY(ip, op, cpy); ip -= (op-cpy); op = cpy;
		if (ip>=iend) break;    // check EOF


		// get offset
		ref = cpy - A16(ip); ip+=2;

		// get matchlength
		if ((length=(token&ML_MASK)) == ML_MASK) { for (;(len=*ip++)==255;length+=255){} length += len; }

		// copy repeated sequence
		if (op-ref<COPYTOKEN)
		{
			*op++ = *ref++;
			*op++ = *ref++;
			*op++ = *ref++;
			*op++ = *ref++;
			ref -= dec[op-ref];
			A32(op)=A32(ref);
		} else { A32(op)=A32(ref); op+=4; ref+=4; }
		cpy = op + length;
		if (cpy>oend-COPYLENGTH)
		{
			if (cpy > oend) goto _output_error;
			LZ4_WILDCOPY(ref, op, (oend-COPYLENGTH));
			while(op<cpy) *op++=*ref++;
			op=cpy;
			if (op == oend) break;    // Check EOF (should never happen, since last 5 bytes are supposed to be literals)
			continue;
		}
		LZ4_WILDCOPY(ref, op, cpy);
		op=cpy;		// correction
	}

	// end of decoding
	return (int) (((char*)op)-dest);

	// write overflow error detected
_output_error:
	return (int) (-(((char*)ip)-source));
}


//**************************************
// Basic Types
//**************************************
typedef struct
{
	uint32_t	magic;		// 'L','Z','4','P'
	uint32_t	osize;		// original size
	uint32_t	csize;		// compressed size
	uint32_t	bsize;		// block size
	uint32_t	nblock;		// block count
	uint32_t	reserved[3];
} lz4p_header_t;


//****************************
// Constants
//****************************
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define MAGIC(a,b,c,d)		((a) | (b) << 8 | (c) << 16 | (d) << 24)
#else
#define MAGIC(a,b,c,d)		((a) << 24 | (b) << 16 | (c) << 8 | (d))
#endif

#define COMPRESSOR_NAME "Compression CLI using LZ4HC algorithm"
#define COMPRESSOR_VERSION ""
#define COMPILED __DATE__
#define AUTHOR "Yann Collet"
#define BINARY_NAME "lz4pack"
#define EXTENSION ".lz4"
#define WELCOME_MESSAGE "*** %s %s, by %s (%s) ***\n", COMPRESSOR_NAME, COMPRESSOR_VERSION, AUTHOR, COMPILED

#define LZ4P_MAGIC	MAGIC('L','Z','4','P')

//#define CHUNKSIZE (8<<20)    // 8 MB
#define CHUNKSIZE (256<<10)    // 256 KB
#define CACHELINE 64
#define OUT_CHUNKSIZE (CHUNKSIZE + (CHUNKSIZE/255) + CACHELINE)



//**************************************
// MACRO
//**************************************
#define DISPLAY(...)	printf(__VA_ARGS__)

#define LZ4_E_INPUT_OVERRUN	    (-4)

#define unlz4_check_input_size(size)   		\
        while (in_len < (size))           		\
        {										\
			ret = fill(fill_data);  			\
			if (ret < 0)						\
                    return LZ4_E_INPUT_OVERRUN;\
            in_len += ret;						\
        }

#ifdef DEBUG
static void hexdump (unsigned char *buf, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if ((i % 16) == 0)
			printf ("%s%08x: ", i ? "\n" : "", (unsigned int) &buf[i]);
		printf ("%02x ", buf[i]);
	}
	printf ("\n");
}
#endif

int lz4_do_decomp(unsigned char * pDecomp, unsigned char * pComp, unsigned long * pDecompSize)
{
	unsigned char* src = pComp;
	unsigned char* dst = pDecomp;

	unsigned long long filesize = 0;
	char* in_buff;
	char* out_buff;
	int sinkint;
	int nextSize;
	lz4p_header_t * header;
	unsigned int block_size;

	uint32_t	n, block_count;
	uint32_t*	list_block_size;

	// Check Archive Header
	header = (lz4p_header_t *)src;
	if(header->magic != LZ4P_MAGIC){ DISPLAY("Unrecognized header : file cannot be decoded\n"); return 6; }

	src += sizeof(lz4p_header_t);
	
	block_size = header->bsize;
	block_count = header->nblock;

	list_block_size = (uint32_t*)malloc(sizeof(uint32_t)*block_count);
	memcpy(list_block_size, src, sizeof(uint32_t) * block_count);
	src += (sizeof(uint32_t) * block_count);

	// Main Loop
	for(n=0; n<block_count; n++)
	{
		nextSize = list_block_size[n];

		in_buff = (char *)src;
		out_buff = (char *)dst;

		if(n < (header->nblock-1))
		{
			// Decode Block
			sinkint = LZ4_uncompress(in_buff, out_buff, block_size);
			if(sinkint < 0 || sinkint != nextSize)
			{
				DISPLAY("Uncompress error. n:%d, res:%d, nextSize:%d\n",
						n, sinkint, nextSize);
				return 8;
			}
			filesize += block_size;
		}
		else
		{
			// Last Block
			sinkint = LZ4_uncompress_unknownOutputSize(in_buff, out_buff, nextSize, block_size);
			if(sinkint < 0){ DISPLAY("Uncompress error : res=%d\n", sinkint); return 9; }
			filesize += sinkint;
			break;
		}

		src += nextSize;
		dst += block_size;
		
	}

	free(list_block_size);
	*pDecompSize = (unsigned long)filesize;
	
	return 0;
}

unsigned int unlz4_get_decompsize(unsigned char * buf)
{
	lz4p_header_t * lz4p_header = (lz4p_header_t * )buf ;
	return (unsigned int)(lz4p_header->osize);
}

int unlz4_get_hdroffset(unsigned char *input, int in_len)
{
#if 0
	int offset = 0;
	
	while (offset < in_len)
	{
		if (memcmp(input+offset, LZ4P_MAGIC, sizeof(uint32_t)) == 0) {
			dprintf("offset = %x\n", offset);
			return offset;
		}
		/* assume lzop hdr is aligned by sizeof(uint32_t) */
		offset += sizeof(uint32_t);
	}
	return -1;
#else
	return 0;
#endif
}

int unlz4_read(u8 *input, int in_len,
				int (*fill) (void *fill_data),
				void *fill_data,
				u8 *output, int *posp)
{
	u8 *in_buf, *out_buf;
	int sink_int, next_size;
	int ret = -1;
	
	lz4p_header_t *header;
	unsigned int block_size;

	uint32_t	n, block_count, blocklist_size;
	uint32_t*	list_block_size;
	
	dprintf("unlz4_read in %x in_len %d output %x\n", input, in_len, output);
	
	out_buf = output;
	in_buf  = input;

	if (*posp)
		*posp = 0;

	// check header...
	unlz4_check_input_size(sizeof(lz4p_header_t));
	
	header = (lz4p_header_t*)in_buf;
	if(header->magic != LZ4P_MAGIC) {
		DISPLAY("Unrecognized header : file cannot be decoded\n");
		return 6;
	}
//	hexdump((u8*)header, sizeof(lz4p_header_t));
	in_buf += sizeof(lz4p_header_t);
	in_len -= sizeof(lz4p_header_t);
	

	block_size = header->bsize;
	block_count = header->nblock;
	
	blocklist_size = sizeof(uint32_t) * block_count;
	list_block_size = (uint32_t*)malloc(blocklist_size);

	unlz4_check_input_size(blocklist_size);
	memcpy(list_block_size, in_buf, blocklist_size);
//	hexdump(in_buf, blocklist_size);
	in_buf += blocklist_size;	
	in_len -= blocklist_size; 

	for(n=0; n<block_count; n++)
	{
		next_size = list_block_size[n];
		dprintf("[%d] next size = %x\n", n, next_size);
		unlz4_check_input_size(next_size);
//		hexdump((u8*)in_buf, 0x20);

		if(n < (block_count-1))
		{
			// Decode Block
			sink_int = LZ4_uncompress(in_buf, out_buf, block_size);
			if(sink_int < 0 || sink_int != next_size)
			{
				DISPLAY("Uncompress error. n:%d, res:%d, nextSize:%d\n",
						n, sink_int, next_size);
				return 8;
			}
			*posp += block_size;
		}
		else
		{
			// Last Block
			sink_int = LZ4_uncompress_unknownOutputSize(in_buf, out_buf, next_size, block_size);
			if(sink_int < 0) {
				DISPLAY("Uncompress error : res=%d\n", sink_int);
				return 9;
			}
			*posp += sink_int;
			break;
		}

		in_buf += next_size;
		out_buf += block_size;
		in_len -= next_size;
		
	}

	free(list_block_size);

	return 0;
}

