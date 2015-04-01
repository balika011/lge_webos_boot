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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//----------------------------------------------------------------
#define CHAR_HUFFTBL_NUM	288
#define POS_HUFFTBL_NUM	32

typedef struct
{
	int code;
	int no;
} huffcode;

static huffcode _arhuffcode_char[] =
{
	{0x2,   5}, {0x3,   5}, {0xa,  6}, {0x28,  7}, {0x29,  7}, {0x2a,  7}, 
	{0x2b,  7}, {0x2c,  7}, {0x2d, 7}, {0x2e,  7}, {0x2f,  7}, {0x8a,  8},
	{0x8b,  8}, {0x8c,  8}, {0x8d, 8}, {0x8e,  8}, {0x30,  7}, {0x8f,  8},
	{0x90,  8}, {0x180, 9}, {0x91, 8}, {0x181, 9}, {0x182, 9}, {0x183, 9},
	{0x31    , 7 },
	{0x92    , 8 },
	{0x93    , 8 },
	{0x94    , 8 },
	{0xb 	   , 6 },
	{0x184   , 9 },
	{0x185   , 9 },
	{0x186   , 9 },
	{0xc 	   , 6 },
	{0x32    , 7 },
	{0x33    , 7 },
	{0x95    , 8 },
	{0x96    , 8 },
	{0x97    , 8 },
	{0x187   , 9 },
	{0x188   , 9 },
	{0x34    , 7 },
	{0x98    , 8 },
	{0x99    , 8 },
	{0x189   , 9 },
	{0x9a    , 8 },
	{0x9b    , 8 },
	{0x18a   , 9 },
	{0x18b   , 9 },
	{0x35    , 7 },
	{0x9c    , 8 },
	{0x9d    , 8 },
	{0x18c   , 9 },
	{0x18d   , 9 },
	{0x18e   , 9 },
	{0x18f   , 9 },
	{0x190   , 9 },
	{0x9e    , 8 },
	{0x191   , 9 },
	{0x192   , 9 },
	{0x193   , 9 },
	{0x194   , 9 },
	{0x195   , 9 },
	{0x196   , 9 },
	{0x197   , 9 },
	{0x36    , 7 },
	{0x9f    , 8 },
	{0x37    , 7 },
	{0x38    , 7 },
	{0x198   , 9 },
	{0x199   , 9 },
	{0xa0    , 8 },
	{0x19a   , 9 },
	{0x39    , 7 },
	{0x3a    , 7 },
	{0x3b    , 7 },
	{0xa1    , 8 },
	{0xa2    , 8 },
	{0xa3    , 8 },
	{0x19b   , 9 },
	{0x19c   , 9 },
	{0x19d   , 9 },
	{0x19e   , 9 },
	{0x19f   , 9 },
	{0x1a0   , 9 },
	{0x1a1   , 9 },
	{0x3b0   , 10},
	{0x3b1   , 10},
	{0x3b2   , 10},
	{0x1a2   , 9 },
	{0x1a3   , 9 },
	{0x1a4   , 9 },
	{0x3b3   , 10},
	{0x1a5   , 9 },
	{0x3b4   , 10},
	{0x3b5   , 10},
	{0x3b6   , 10},
	{0x3c    , 7 },
	{0xa4    , 8 },
	{0xa5    , 8 },
	{0xa6    , 8 },
	{0xa7    , 8 },
	{0xa8    , 8 },
	{0x1a6   , 9 },
	{0x3b7   , 10},
	{0xd 	   , 6 },
	{0xa9    , 8 },
	{0x1a7   , 9 },
	{0x1a8   , 9 },
	{0x1a9   , 9 },
	{0x1aa   , 9 },
	{0x1ab   , 9 },
	{0x1ac   , 9 },
	{0x3d    , 7 },
	{0x1ad   , 9 },
	{0xaa    , 8 },
	{0x1ae   , 9 },
	{0x1af   , 9 },
	{0x1b0   , 9 },
	{0x3b8   , 10},
	{0x3b9   , 10},
	{0x3e    , 7 },
	{0x1b1   , 9 },
	{0x1b2   , 9 },
	{0x1b3   , 9 },
	{0x3ba   , 10},
	{0x3bb   , 10},
	{0x3bc   , 10},
	{0x3bd   , 10},
	{0x3f    , 7 },
	{0xab    , 8 },
	{0x1b4   , 9 },
	{0x1b5   , 9 },
	{0x1b6   , 9 },
	{0x3be   , 10},
	{0x3bf   , 10},
	{0x3c0   , 10},
	{0xac    , 8 },
	{0xad    , 8 },
	{0x1b7   , 9 },
	{0x1b8   , 9 },
	{0x1b9   , 9 },
	{0x3c1   , 10},
	{0x3c2   , 10},
	{0x3c3   , 10},
	{0xae    , 8 },
	{0xaf    , 8 },
	{0xb0    , 8 },
	{0x3c4   , 10},
	{0x3c5   , 10},
	{0x3c6   , 10},
	{0x3c7   , 10},
	{0x3c8   , 10},
	{0x40    , 7 },
	{0xb1    , 8 },
	{0x1ba   , 9 },
	{0x1bb   , 9 },
	{0x3c9   , 10},
	{0x3ca   , 10},
	{0x3cb   , 10},
	{0x3cc   , 10},
	{0xb2    , 8 },
	{0xb3    , 8 },
	{0x3cd   , 10},
	{0x3ce   , 10},
	{0x3cf   , 10},
	{0x3d0   , 10},
	{0x3d1   , 10},
	{0x7de   , 11},
	{0xb4    , 8 },
	{0x1bc   , 9 },
	{0x1bd   , 9 },
	{0x1be   , 9 },
	{0x3d2   , 10},
	{0x3d3   , 10},
	{0x7df   , 11},
	{0x7e0   , 11},
	{0xb5    , 8 },
	{0x3d4   , 10},
	{0x3d5   , 10},
	{0x7e1   , 11},
	{0x3d6   , 10},
	{0xb6    , 8 },
	{0x3d7   , 10},
	{0x7e2   , 11},
	{0x3d8   , 10},
	{0x7e3   , 11},
	{0x7e4   , 11},
	{0x7e5   , 11},
	{0x3d9   , 10},
	{0xb7    , 8 },
	{0x7e6   , 11},
	{0x7e7   , 11},
	{0xb8    , 8 },
	{0x1bf   , 9 },
	{0x3da   , 10},
	{0x3db   , 10},
	{0x3dc   , 10},
	{0x7e8   , 11},
	{0x7e9   , 11},
	{0x7ea   , 11},
	{0x1c0   , 9 },
	{0x1c1   , 9 },
	{0x3dd   , 10},
	{0x7eb   , 11},
	{0x3de   , 10},
	{0x7ec   , 11},
	{0x7ed   , 11},
	{0x7ee   , 11},
	{0xe 	   , 6 },
	{0xf 	   , 6 },
	{0x1c2   , 9 },
	{0x1c3   , 9 },
	{0x3df   , 10},
	{0x1c4   , 9 },
	{0x7ef   , 11},
	{0x7f0   , 11},
	{0x1c5   , 9 },
	{0x1c6   , 9 },
	{0x3e0   , 10},
	{0x3e1   , 10},
	{0x1c7   , 9 },
	{0x3e2   , 10},
	{0x7f1   , 11},
	{0x7f2   , 11},
	{0x10    , 6 },
	{0x1c8   , 9 },
	{0x1c9   , 9 },
	{0x3e3   , 10},
	{0x3e4   , 10},
	{0x1ca   , 9 },
	{0x1cb   , 9 },
	{0x41    , 7 },
	{0x1cc   , 9 },
	{0x1cd   , 9 },
	{0x1ce   , 9 },
	{0x3e5   , 10},
	{0x3e6   , 10},
	{0x3e7   , 10},
	{0x3e8   , 10},
	{0x3e9   , 10},
	{0x42    , 7 },
	{0xb9    , 8 },
	{0x7f3   , 11},
	{0x7f4   , 11},
	{0x3ea   , 10},
	{0x7f5   , 11},
	{0x1cf   , 9 },
	{0x1d0   , 9 },
	{0xba    , 8 },
	{0x1d1   , 9 },
	{0x1d2   , 9 },
	{0x1d3   , 9 },
	{0x1d4   , 9 },
	{0x1d5   , 9 },
	{0xbb    , 8 },
	{0x43    , 7 },
	{0x0 	   , 4 },
	{0x4 	   , 5 },
	{0x11    , 6 },
	{0x12    , 6 },
	{0x13    , 6 },
	{0x44    , 7 },
	{0xbc    , 8 },
	{0xbd    , 8 },
	{0xbe    , 8 },
	{0x1d6   , 9 },
	{0x1d7   , 9 },
	{0x3eb   , 10},
	{0x3ec   , 10},
	{0x3ed   , 10},
	{0x3ee   , 10},
	{0x7f6   , 11},
	{0x7f7   , 11},
	{0x7f8   , 11},
	{0x7f9   , 11},
	{0x7fa   , 11},
	{0x7fb   , 11},
	{0xff8   , 12},
	{0xff9   , 12},
	{0xffa   , 12},
	{0xffb   , 12},
	{0x1ffc  , 13},
	{0xffc   , 12},
	{0x1ffd  , 13},
	{0xffd   , 12},
	{0x1ffe  , 13},
	{0x1fff  , 13},
	{0xbf    , 8 },
};

static huffcode _arhuffcode_pos[] =
{	
	{ 0x0   , 2 }, { 0x2   , 3 }, { 0x6   , 4 }, { 0xe   , 5 },	{ 0xf   , 5 },
	{ 0x10  , 5 }, { 0x11  , 5 }, { 0x12  , 5 }, { 0x13  , 5 }, { 0x14  , 5 },
	{ 0x2a  , 6 }, { 0x2b  , 6 }, { 0x2c  , 6 }, { 0x2d  , 6 }, { 0x2e  , 6 },
	{ 0x2f  , 6 }, { 0x30  , 6 }, { 0x31  , 6 }, { 0x32  , 6 }, { 0x33  , 6 },
	{ 0x34  , 6 }, { 0x35  , 6 }, { 0x36  , 6 }, { 0x37  , 6 }, { 0x38  , 6 },
	{ 0x39  , 6 }, { 0x3a  , 6 }, { 0x3b  , 6 }, { 0x3c  , 6 }, { 0x3d  , 6 },
	{ 0x3e  , 6 }, { 0x3f  , 6 },
	
};

//----------------------------------------------------------------
#define THUMB_CONVERT 1         // filter thumb code first 
#define REMOVE_EXTRA_FILE  1
#define ADD_EXTRA_8_BYTE   1
//----------------------------------------------------------------

#define CONVFILE        "conv"

//----------------------------------------------------------------
#define RING_BIT     12   // no. of bits for the ring buffer, (max. 16)
#define RING_SIZE    (1<<RING_BIT) // (2^RING_BIT) size of ring buffer
#define POSDIFF      1    // record position as the diff. between search buf location and lookahead buf location
#define POSHUFF      1    // record part of position bit as huffman code
#define POSHUFF_BIT  5    // no. of bits as huffman code for position, used when POSHUFF = 1
#define POSHUFF_SIZE (1<<POSHUFF_BIT)   // (2^POSHUFF_BIT)size of the huffman code for position, used when POSHUFF = 1
#define MAXLEN       34     // upper limit for match_length (max. 32+2)
#define THRESHOLD    2          // encode string into position and length if match_length is greater than THRESHOLD
#define NIL      RING_SIZE      // index for root of binary search trees

#define RECORD_LENGTH 1 //record code size in compressed file's header
#define RECORD_CHECKSUM 1

#define LZHS_SOURCE_ALIGNMENT   8

int input_filesize = 0;
unsigned long int textsize = 0, // text size counter
                  codesize = 0,   // code size counter
                  lzssize = 0; // lzss size counter
                  
unsigned char checksum = 0;
                  
struct huffcode
{
    long f;     //frequency
    int lson;   //left son
    int rson;   //right son
    int code;   //huffman code
    int no;     //no. of bits of this huffman code
};

struct huffcode charcode[2 * (256 + MAXLEN - THRESHOLD) - 1];
struct huffcode poscode[2 * POSHUFF_SIZE - 1];
int charno = 0; // to count the no. of char that is actually used
int posno = 0;  // to count the no. of pos that is actually used

// ring buffer size, with extra MAXLEN - 1 bytes to facilitate string comparison
unsigned char text_buf[RING_SIZE + MAXLEN - 1];
// longest match.  These are set by the InsertNode() procedure.
int match_position, match_length;
//These constitute binary search trees.
int lson[RING_SIZE + 1], rson[RING_SIZE + 257], dad[RING_SIZE + 1];
// input & output files
FILE* infile, * convfile, * lzsfile, * outfile, *outfile_raw;

static void InitTree(void);
static void lazy_match(int r);
static void InsertNode(int r);
static void DeleteNode(int p);  
static void LZSS(char* tmplzhsfilename);
static void getmin(struct huffcode* hcode, int node_no, int* min, int* secmin);
static void traverse(int n, int no, struct huffcode* hcode, int symbol_no);
static void huffman_code(struct huffcode* hcode, int symbol_no, int node_no);
static void SHUFF(char* tmplzhsfilename);
static void writehuff(int code, int no);
static void ARMThumb_Convert();

static int PreprocessInputFile(char* infilename, char* tmpfilename);


//-------------------------------------------------------------------------------
static void Usage(char **argv)
{
    printf("Usage: %s [input_file] [output_file]\n", argv[0]);
    printf("\t [input]  Source file name\n");
    printf("\t [output] Encoded file name\n");
    printf("\n");
}


//-------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    char* infilename;
    char* outfilename;    
    char tmpfilename[256];
	char tmplzhsfilename[256];
    char ch = 0;
    int i;
    int maxcharno = 0;
    int maxposno = 0;    
  
    printf("\nLZHS compress(encode)\n");  
    if (argc != 3)
    {
        Usage(argv);
        return 1;
    }

    infilename = outfilename = NULL;
    infilename = argv[1];
    outfilename = argv[2];

    if((infilename==NULL) || (outfilename==NULL))
    {
        Usage(argv);
        return 1;
    }

    sprintf(tmpfilename, "%s.tmp", infilename);
	sprintf(tmplzhsfilename, "%s.lzs", infilename);
    if(PreprocessInputFile(infilename, tmpfilename) != 0)
    {
        printf("open file fail \n");
        return EXIT_FAILURE;
    }

    if (((infile = fopen(tmpfilename, "rb")) == NULL) ||
        ((outfile = fopen(outfilename, "wb")) == NULL))
    {
        printf("open file fail \n");  
        return EXIT_FAILURE;
    }
    
#if RECORD_LENGTH
    // pre-fill 8 DWRD for textsize and codesize
    for (i = 0; i < 8; i++)
    {
        putc(ch, outfile);
    }
#endif

#if RECORD_CHECKSUM
    for(i=0; i<8; i++)
    {
        putc(0, outfile);
    }
#endif
    ARMThumb_Convert();
    LZSS(tmplzhsfilename);
    SHUFF(tmplzhsfilename);
    //========== fill length ==========================================
#if RECORD_LENGTH
    fseek( outfile, 0, SEEK_END);
    codesize = ftell(outfile) - 0x10;
    printf("LZHS Out: %ld(0x%08x) bytes\n", codesize, codesize);
#if ADD_EXTRA_8_BYTE
    textsize =textsize-8;
#endif
    fseek(outfile, 0, 0);
    for (i = 0; i < 4; i++)
    {
        ch = (char) (textsize >> (i * 8));
        putc(ch, outfile);
    }

    for (i = 0; i < 4; i++)
    {
        ch = (char) (codesize >> (i * 8));
        putc(ch, outfile);
    }
#endif

#if RECORD_CHECKSUM
    putc((char)checksum, outfile);
#endif

    fclose(infile);
    fclose(outfile);

#if REMOVE_EXTRA_FILE
    remove(tmpfilename);
    remove(tmplzhsfilename);
    remove(CONVFILE);
#endif

    printf("\n");

    return EXIT_SUCCESS;
}

//=========== LZSS =============================================================
void LZSS(char* tmplzhsfilename)
{
    int i, c, len, r, s, last_match_length, code_buf_ptr;
    int max = 0;   
    unsigned char  code_buf[25], mask;  //max.(len,pos)= (2bytes, 1bytes)
    for (i = 0; i < (256 + MAXLEN - THRESHOLD); i++)
    {
        charcode[i].f = 0;
        charcode[i].no = 0;
        charcode[i].code = 0;
    }
    for (i = 0; i < POSHUFF_SIZE; i++)
    {
        poscode[i].f = 0;
        poscode[i].no = 0;
        charcode[i].code = 0;
    }
    if ((lzsfile = fopen(tmplzhsfilename, "wb")) == NULL)
    {
        printf("open file fail tmp.lzs\n");
    }

    InitTree();  /* initialize trees */
    code_buf[0] = 0;  /* code_buf[1..16] saves eight units of code, and
                      code_buf[0] works as eight flags, "1" representing that the unit
                      is an unencoded letter (1 byte), "0" a position-and-length pair
                      (2 bytes).  Thus, eight units require at most 16 bytes of code. */
    code_buf_ptr = mask = 1;
    s = 0;
    r = RING_SIZE - MAXLEN;
    /* Clear the buffer with any character that will appear often. */
    //for (i = s; i < r; i++)
    // text_buf[i] = ' ';  // mark this such that hardwar doesn't have to initialize sram

#if THUMB_CONVERT   
    if ((infile = fopen("conv", "rb")) == NULL)
    {
        printf("open file fail conv\n");
    }
#endif

    fseek(infile, 0, SEEK_SET);
    for (len = 0; len < MAXLEN && (c = getc(infile)) != EOF; len++)
    {
        //fprintf(tmp,"%d\n",c);     
        text_buf[r + len] = c;  //Read MAXLEN bytes into the last MAXLEN bytes of the buffer
    }
    if ((textsize = len) == 0)
    {
        return;
    }  /* text of size zero */

    /* Insert the MAXLEN strings,
      each of which begins with one or more 'space' characters.  Note
      the order in which these strings are inserted.  This way,
      degenerate trees will be less likely to occur. */
    //for (i = 1; i <= MAXLEN; i++)
    //InsertNode(r - i);  // mark this such that hardware doesn't have to initialize sram

    InsertNode(r);  /* Finally, insert the whole string just read.  The
                    global variables match_length and match_position are set. */
    do
    {
        if (match_length > len)
            match_length = len;  /* match_length    may be spuriously long near the end of text. */

        if (match_length <= THRESHOLD)
        {
            match_length = 1;  /* Not long enough match.  Send one byte. */
            code_buf[0] |= mask;  /* 'send one byte' flag */
            code_buf[code_buf_ptr++] = text_buf[r];  /* Send uncoded. */
            charcode[text_buf[r]].f++;
        }
        else
        {
            code_buf[code_buf_ptr++] = (unsigned char)
                (match_length - (THRESHOLD + 1));
            code_buf[code_buf_ptr++] = (unsigned char) (match_position >> 8);
            code_buf[code_buf_ptr++] = (unsigned char) match_position;

            charcode[255 - THRESHOLD + match_length].f++;

            if (POSHUFF)
            {
                poscode[(match_position >> (RING_BIT - POSHUFF_BIT))].f++;
            }
        }

        if ((mask <<= 1) == 0) /* Shift mask left one bit. */
        {
            for (i = 0; i < code_buf_ptr; i++)  /* Send at most 8 units of */
                putc(code_buf[i], lzsfile);     /* code together */

            lzssize += code_buf_ptr;
            code_buf[0] = 0;
            code_buf_ptr = mask = 1;
        }

        last_match_length = match_length;
        for (i = 0; i < last_match_length && (c = getc(infile)) != EOF; i++)
        {
            //fprintf(tmp,"%d\n",c);
            DeleteNode(s);    // Delete old strings and
            text_buf[s] = c;  // read new bytes
            if (s < MAXLEN - 1)
                text_buf[s + RING_SIZE] = c;  /* If the position is
                                              near the end of buffer, extend the buffer to make
                                              string comparison easier. */

            //increment the position modulo RING_SIZE.
            s = (s + 1) % RING_SIZE;
            r = (r + 1) % RING_SIZE;

            InsertNode(r); // Register the string in text_buf[r..r+MAXLEN-1]
        }
        textsize += i;
        while (i++ < last_match_length) // After the end of text,
        {
            DeleteNode(s);
            s = (s + 1) % RING_SIZE;
            r = (r + 1) % RING_SIZE;
            if (--len)
               InsertNode(r);      // buffer may not be empty.
        }
    }
    while (len > 0);  /* until length of string to be processed is zero */
    if (code_buf_ptr > 1) /* Send remaining code. */
    {
        for (i = 0; i < code_buf_ptr; i++)
            putc(code_buf[i], lzsfile);

        lzssize += code_buf_ptr;
    }
    printf("In : %ld bytes\n", textsize);   /* Encoding is done. */
    printf("LZSS Out: %ld bytes\n", lzssize);
    printf("LZSS Out/In: %.4f\n", (double) lzssize / textsize);

    //==== don't let the symbol which never shows up be in the huffman tree =====
    for (i = 0; i < (256 + MAXLEN - THRESHOLD); i++)
    {
        if (charcode[i].f == 0)
        {
            charcode[i].f = -1;
        }
        else
        {
            charno++;
        }
    }
    for (i = 0; i < POSHUFF_SIZE; i++)
    {
        if (poscode[i].f == 0)
        {
            poscode[i].f = -1;
        }
        else
        {
            posno++;
        }
    }

    fclose(lzsfile);
}

//=========== initialize trees =========================
static void InitTree(void)
{
    int i;

    /* For i = 0 to RING_SIZE - 1, rson[i] and lson[i] will be the right and
       left children of node i.  These nodes need not be initialized.
       Also, dad[i] is the parent of node i.  These are initialized to
       NIL (= RING_SIZE), which stands for 'not used.'
       For i = 0 to 255, rson[RING_SIZE + i + 1] is the root of the tree
       for strings that begin with character i.  These are initialized
       to NIL.  Note there are 256 trees. */

    for (i = RING_SIZE + 1; i <= RING_SIZE + 256; i++)
    {
        rson[i] = NIL;
    }
    for (i = 0; i < RING_SIZE; i++)
    {
        dad[i] = NIL;
    }
}
//========== check the next symobl's match length ================
static void lazy_match(int r)
{
    int lazy_match_length;
    int i, p, cmp;
    unsigned char * key;

    lazy_match_length = 0;

    if (match_length < (MAXLEN - 1))
    {
        cmp = 1;
        r += 1;
        key = &text_buf[r];
        p = RING_SIZE + 1 + key[0];
        //rson[r+1] = lson[r+1] = NIL;
        lazy_match_length = 0;
        for (; ;)
        {
            if (cmp >= 0)
            {
                if (rson[p] != NIL)
                    p = rson[p];
                else
                {
                    //rson[p] = r;
                    //dad[r] = p;
                    break;
                }
            }
            else
            {
                if (lson[p] != NIL)
                    p = lson[p];
                else
                {
                    //lson[p] = r;
                    //dad[r] = p;
                    break;
                }
            }
            for (i = 1; i < MAXLEN; i++)
            {
                cmp = key[i] - text_buf[p + i];
                if (cmp != 0)
                    break;
            }
            if (i > lazy_match_length)
            {
                if ((lazy_match_length = i) >= MAXLEN)
                    break;
            }
        }
    }

    if (lazy_match_length > match_length)
    {
        match_length = 0;
    }
}

//======================================================
static void InsertNode(int r)
    /* Inserts string of length MAXLEN, text_buf[r..r+MAXLEN-1], into one of
         the trees (text_buf[r]'th tree) and returns the longest-match position
     and length via the global variables match_position and match_length.
     If match_length = MAXLEN, then removes the old node in favor of the
         new one, because the old one will be deleted sooner.
     Note r plays double role, as tree node and position in buffer. */
{
    int i, p, cmp;
    unsigned char * key;
    int match_position_tmp;
    cmp = 1;
    key = &text_buf[r];
    p = RING_SIZE + 1 + key[0];
    rson[r] = lson[r] = NIL;
    match_length = 0;
    for (; ;)
    {
        if (cmp >= 0)
        {
            if (rson[p] != NIL)
                p = rson[p];
            else
            {
                rson[p] = r;
                dad[r] = p;
                lazy_match(r);
                return;
            }
        }
        else
        {
            if (lson[p] != NIL)
                p = lson[p];
            else
            {
                lson[p] = r;
                dad[r] = p;
                lazy_match(r);
                return;
            }
        }
        for (i = 1; i < MAXLEN; i++)
        {
            cmp = key[i] - text_buf[p + i];
            if (cmp != 0)
                break;
        }
        if (i >= match_length)
        {
            if (POSDIFF)
            {
                if (r >= p)
                    match_position_tmp = r - p;
                else
                    match_position_tmp = RING_SIZE - p + r;
            }
            else
                match_position_tmp = p;
			
            if (i == match_length) // when match_lenth equal
            {
                // use the one with smaller match_position
                if (match_position_tmp < match_position)
                    match_position = match_position_tmp;
            }
            else
                match_position = match_position_tmp;

            if ((match_length = i) >= MAXLEN)
                break;
        }
    }
    //when match_length = MAXLEN
    dad[r] = dad[p];
    lson[r] = lson[p];
    rson[r] = rson[p];
    dad[lson[p]] = r;
    dad[rson[p]] = r;
    if (rson[dad[p]] == p)
    {
        rson[dad[p]] = r;
    }
    else
    {
        lson[dad[p]] = r;
    }
    dad[p] = NIL;  /* remove p */
}
//===============================================================
static void DeleteNode(int p)  /* deletes node p from tree */
{
    int q;

    if (dad[p] == NIL)
    {
        return;
    }  /* not in tree */
    if (rson[p] == NIL)
    {
        q = lson[p];
    }
    else if (lson[p] == NIL)
    {
        q = rson[p];
    }
    else
    {
        q = lson[p];
        if (rson[q] != NIL)
        {
            do
            {
                q = rson[q];
            }
            while (rson[q] != NIL);
            rson[dad[q]] = lson[q];  dad[lson[q]] = dad[q];
            lson[q] = lson[p];  dad[lson[p]] = q;
        }
        rson[q] = rson[p];  dad[rson[p]] = q;
    }
    dad[q] = dad[p];
    if (rson[dad[p]] == p)
    {
        rson[dad[p]] = q;
    }
    else
    {
        lson[dad[p]] = q;
    }
    dad[p] = NIL;
}


//========= SHUFF ==============================
int precode = 0x0000;
int tmpcode;
int code;
int no;
int preno = 0;
int tmpno;

//========= static huffman encode after LZSS done =========
static void SHUFF(char* tmplzhsfilename)
{
    unsigned char c;
    int posh, posl, len;
    int poshuff;
    int i, k, r;
    unsigned int  flags;
    int end = 0;
    unsigned long lzsindex = 0;

    if ((lzsfile = fopen(tmplzhsfilename, "rb")) == NULL)
    {
        printf("open file fail tmp.lzs\n");
    }

    // Get char huffman parameter
	for(i=0;i<(256+MAXLEN-THRESHOLD);i++)	
	{
	    charcode[i].code = _arhuffcode_char[i].code;
	    charcode[i].no = _arhuffcode_char[i].no;
	}

    // Get pos huffman parameter
	for(i=0;i<POSHUFF_SIZE;i++)	
	{
	    poscode[i].code = _arhuffcode_pos[i].code;
	    poscode[i].no = _arhuffcode_pos[i].no;
	}

    for (i = 0; i < RING_SIZE - MAXLEN; i++)
    {
        text_buf[i] = ' ';
    }
    r = RING_SIZE - MAXLEN;  
    flags = 0;
    while (end == 0)
    {
        if (((flags >>= 1) & 256) == 0)
        {
            c = getc(lzsfile);
            lzsindex++;
            if (lzsindex >= lzssize)
                break;
            flags = c | 0xff00;     /* uses higher byte cleverly */
        }                           /* to count eight */
        if (flags & 1)    // char
        {
            c = getc(lzsfile);
            lzsindex++;
            if (lzsindex > lzssize)
                break;
            writehuff(charcode[c].code, charcode[c].no);
            text_buf[r++] = c;
            r = r % RING_SIZE;
        }
        else     //(len,posh,posl)
        {
            len = getc(lzsfile);
            lzsindex++;
            if (lzsindex > lzssize)
                break;
            posh = getc(lzsfile);
            lzsindex++;
            if (lzsindex > lzssize)
            {
                printf("(len,pos) sync error");
                break;
            }
            posl = fgetc(lzsfile);
            lzsindex++;
            if (lzsindex > lzssize)
            {
                printf("(len,pos) sync error");
                break;
            }
            i = (posh << 8) | posl;
            writehuff(charcode[(256 + len)].code, charcode[256 + len].no);
            if (POSHUFF)
            {
                poshuff = i >> (RING_BIT - POSHUFF_BIT);
                if (poshuff >= (2 * POSHUFF_SIZE - 1))
                    printf("error\n");
                writehuff(poscode[poshuff].code, poscode[poshuff].no);
                writehuff((i - (poshuff << (RING_BIT - POSHUFF_BIT))),
                    (RING_BIT - POSHUFF_BIT));      
            }
            else
            {
                writehuff(i, RING_BIT);
            }
            for (k = 0; k <= len; k++)
            {
                c = text_buf[(i + k) % RING_SIZE];
                text_buf[r++] = c;
                r = r % RING_SIZE ;
            }
        }
    } // while(end==0)

    //put residual bit in the buffer
    if (preno > 0)
        ;
    {
        putc((precode << (8 - preno)), outfile);
        codesize += preno;
    }

    codesize = codesize / 8;
    //printf("LZHS Out: %ld bytes\n", codesize);
    printf("LZHS Out/In: %.4f\n", (double) codesize / textsize);
}

//========= function to get the min. two frequencies =========
static void getmin(struct huffcode* hcode, int node_no, int* min, int* secmin)
{
    int i, c, t;
    c = 0;
    for (i = 0; i < node_no; i++)
    {
        if ((hcode[i].f != -1) && (c == 0))
        {
            *min = i;
            c = 1;
        }
        else if ((hcode[i].f != -1) && (c == 1))
        {
            t = i + 1;
            if (hcode[*min].f < hcode[i].f)
                *secmin = i;
            else
            {
                *secmin = *min;
                *min = i;
            }
            break;
        }
    }
    for (i = t; i < node_no; i++)
    {
        if (hcode[i].f != -1)
        {
            if (hcode[i].f < hcode[*min].f)
            {
                *secmin = *min;
                *min = i;
            }
            else if (hcode[i].f < hcode[*secmin].f)
            {
                *secmin = i;
            }
        }
    }
}
//============ get the huffmancode for each symbol =============================
static void huffman_code(struct huffcode* hcode, int symbol_no, int node_no)
{
    int i;
    int min, secmin;
    unsigned char max_bits = 0;
    unsigned long code = 0;
    int bl_counts[256];
    unsigned long next_code[256];

    for (i = symbol_no; i < (symbol_no + node_no - 1); i++)
    {
        hcode[i].f = -1;
    }

    for (i = symbol_no; i < (symbol_no + node_no - 1); i++)
    {
        getmin(hcode, (symbol_no + node_no - 1), &min, &secmin);
        hcode[i].f = hcode[min].f + hcode[secmin].f;
        hcode[i].rson = min;
        hcode[i].lson = secmin;
        hcode[min].f = -1;
        hcode[secmin].f = -1;
    }

    traverse((symbol_no + node_no - 2), 0, hcode, symbol_no);

    //==== assign the huffman code lexicographically consecutive ======
    for (i = 0; i < 256; i++)
    {
        bl_counts[i] = 0;
    }
    for (i = 0; i < symbol_no; i++)
    {
        bl_counts[hcode[i].no]++;
        if (hcode[i].no > max_bits)
            max_bits = hcode[i].no;
    }
    bl_counts[0] = 0;
    for (i = 1; i <= max_bits; i++)
    {
        code = (code + bl_counts[i - 1]) << 1;
        next_code[i] = code;
    }
    for (i = 0; i < symbol_no; i++)
    {
        if (hcode[i].no != 0)
        {
            hcode[i].code = next_code[hcode[i].no];
            next_code[hcode[i].no]++;
        }
    }
}

//== after huffman tree are constructed, traverse the tree to get the huffman code no of each symbol ==
static void traverse(int n, int no, struct huffcode* hcode, int symbol_no)
{
    if ((n >= 0) && (n <= (symbol_no - 1)))
    {
        hcode[n].no = no;
    }
    else
    {
        traverse(hcode[n].lson, no + 1, hcode, symbol_no);//left son get bit 0
        traverse(hcode[n].rson, no + 1, hcode, symbol_no);//right son get bit 1
        no++;
    }
}

//==============================================
static void writehuff(int code, int no)
{
    codesize += no;
    if ((preno + no) < 8)
    {
        preno = preno + no;
        precode = (precode << no) | code;
    }
    else
    {
        do
        {
            tmpno = 8 - preno;
            no = no - tmpno;
            tmpcode = code >> no;
            putc((precode << tmpno) | tmpcode, outfile);
            code = code - (tmpcode << no);
            preno = 0;
            precode = 0;
        }
        while (no >= 8);

        preno = no;
        precode = code;
    }
}

//============== prefilter for thumb code ==========================
static void ARMThumb_Convert()
{    
    unsigned int i;
    unsigned int dest;
    unsigned int insize;
    unsigned char * data;

    fseek(infile, 0, SEEK_END);
    insize = ftell(infile);
    fseek(infile, 0, SEEK_SET);
    data = malloc(insize);
    fread(data, 1, insize, infile);

    checksum = 0;
    for (i = 0; i < insize; i++)
    {
        checksum += data[i];
        //if (i == 299999)
        //    printf("checksum for the first 100k bytes = %x\n", checksum);
    }
    printf("checksum = %x\n", checksum);

#if THUMB_CONVERT
    if ((convfile = fopen(CONVFILE, "wb")) == NULL)
    {
        printf("open file fail conv\n");
    }

    for (i = 0; i + 4 + 6 <= insize; i += 2)
    {
        if ((data[i + 1] & 0xF8) == 0xF0 && (data[i + 3] & 0xF8) == 0xF8)
        {
            unsigned int src = ((data[i + 1] & 0x7) << 19) |
                (data[i + 0] << 11) |
                ((data[i + 3] & 0x7) << 8) |
                (data[i + 2]);

            dest = i / 2 + 2 + src;

            data[i + 1] = 0xF0 | ((dest >> 19) & 0x7);
            data[i + 0] = (dest >> 11);
            data[i + 3] = 0xF8 | ((dest >> 8) & 0x7);
            data[i + 2] = (dest);
            i += 2;
        }
    }
    fwrite(data, 1, insize, convfile);
    fclose(infile);
    fclose(convfile);
#endif
}


//---------------------------------------------------------------------------
static int PreprocessInputFile(char* infilename, char* tmpfilename)
{
    FILE *pinfile, *ptmpfile;
    char buf[512];
    int len, align;

    if( (pinfile = fopen(infilename, "rb")) == NULL)
    {
        printf("open file %s fail \n", infilename);
        return 1;
    }
    
    if( (ptmpfile = fopen(tmpfilename, "wb")) == NULL)
    {
        printf("open file %s fail\n", tmpfilename);
        return 1;
    }

    input_filesize = 0;
    while( (len = fread(buf, 1, 512, pinfile)) > 0)
    {
        fwrite(buf, 1, len, ptmpfile);
        input_filesize += len;
    }

    align = input_filesize % LZHS_SOURCE_ALIGNMENT;
    if(align != 0)
    {
        len = LZHS_SOURCE_ALIGNMENT - align;
        memset(buf, 0xFF, len);
        fwrite(buf, 1, len, ptmpfile);
    }
	#if ADD_EXTRA_8_BYTE
    memset(buf, 0x00, 8);
    fwrite(buf, 1, 8, ptmpfile);
	#endif
    fclose(pinfile);
    fclose(ptmpfile);
    
    return 0;
}

