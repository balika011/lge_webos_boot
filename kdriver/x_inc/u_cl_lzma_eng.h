/*******************************************************************************
 * LEGAL DISCLAIMER
 *    
 * (Header of MediaTek Software/Firmware Release or Documentation)
 *    
 * BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
 * AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK 
 * SOFTWARE") RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO
 * BUYER ON AN "AS-IS" BASISONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL 
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH 
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, 
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO
 * LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES 
 * MADE TO BUYER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN
 * FORUM.
 * 
 * BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE 
 * LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, 
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO 
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
 * 
 * THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH
 * THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS 
 * PRINCIPLES.
 ******************************************************************************/
/*------------------------------------------------------------------------------
 * Copyright (c) 2009, Mediatek Inc.
 * All rights reserved.
 * 
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @file u_cl_lzma_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file exports all compression library specific definitions and structures.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_CL
  *  @{  
  */
/*----------------------------------------------------------------------------*/
#ifndef _U_CL_LZMA_ENG_H_
#define _U_CL_LZMA_ENG_H_
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/****************************************************************************/
/*                       zlib compression parameter                         */
/****************************************************************************/
#define CL_LZMA_NO_COMPRESSION         0    /**<0 gives no compression at all (the input data is simply copied a block at a time).       */
#define CL_LZMA_BEST_SPEED             1    /**<1 gives best speed.        */
#define CL_LZMA_BEST_COMPRESSION       9    /**<9 gives best compression.        */
#define CL_LZMA_DEFAULT_COMPRESSION    CL_LZMA_BEST_COMPRESSION    /**<Default compression level.        */
/* compression levels */
/*
The compression level must be between 0 and 9: 
1 gives best speed, 
9 gives best compression, 
0 gives no compression at all (the input data is simply copied a block at a time).
*/
#define CL_LZMA_MAX_WBITS               15    /**<The windowBits parameter is the base two logarithm of the window size (the size of the history buffer). It should be in the range 8..15 for this version of the library. Larger values of this parameter result in better compression at the expense of memory usage.         */
#define CL_LZMA_DEF_WBITS               CL_LZMA_MAX_WBITS    /**<The default value is 15 if deflateInit is used instead.CAUTION: The decompression window bits should be larger than compression.        */
/*
The windowBits parameter is the base two logarithm of the window size (the size of the history buffer). 
It should be in the range 8..15 for this version of the library. 
Larger values of this parameter result in better compression at the expense of memory usage. 
The default value is 15 if deflateInit is used instead.
CAUTION: The decompression window bits should be larger than compression.
*/
#define CL_LZMA_DEF_MEM_LEVEL 8    /**<Specifies how much memory should be allocated for the internal compression state.  The default value is 8. Deflate memory usage (bytes) = (1 << (windowBits+2)) + (1 << (memLevel+9)) .      */
#define CL_LZMA_MAX_MEM_LEVEL 9    /**<Maximum memory for optimal speed.         */
#define CL_LZMA_MIN_MEM_LEVEL 1    /**<Minimum memory but is slow and reduces compression ratio.        */
/*
The memLevel parameter specifies how much memory should be allocated for the internal compression state. 
memLevel=1 uses minimum memory but is slow and reduces compression ratio ; 
memLevel=9 uses maximum memory for optimal speed. 
The default value is 8. 
*/

/*
The memory requirements for compression depend on two parameters, windowBits and memLevel:

    deflate memory usage (bytes) = (1 << (windowBits+2)) + (1 << (memLevel+9)) 
    inflate memory usage (bytes) = (1 << windowBits) + 1440*2*sizeof(int)
*/

/*------------------------------------------------------------------*/
/*! @enum CL_LZMA_COMPRESSION_STRATEGY_T
 *  @brief   The strategy parameter is used to tune the compression algorithm. Use the
 *              value Z_DEFAULT_STRATEGY for normal data, Z_FILTERED for data produced by a
 *              filter (or predictor), Z_HUFFMAN_ONLY to force Huffman encoding only (no
 *              string match), or Z_RLE to limit match distances to one (run-length
 *              encoding). Filtered data consists mostly of small values with a somewhat
 *              random distribution. In this case, the compression algorithm is tuned to
 *              compress them better. The effect of Z_FILTERED is to force more Huffman
 *              coding and less string matching; it is somewhat intermediate between
 *              Z_DEFAULT and Z_HUFFMAN_ONLY. Z_RLE is designed to be almost as fast as
 *              Z_HUFFMAN_ONLY, but give better compression for PNG image data. The strategy
 *              parameter only affects the compression ratio but not the correctness of the
 *              compressed output even if it is not set appropriately.
 *  @code
 *  typedef enum 
 *  {
 *      CL_LZMA_DEFAULT_STRATEGY = 0,
 *      CL_LZMA_FILTERED,
 *      CL_LZMA_HUFFMAN_ONLY,
 *      CL_LZMA_RLE,
 *      CL_LZMA_LAST_ONE 
 *  } CL_LZMA_COMPRESSION_STRATEGY_T;
 *  @endcode
 *  @li@c  CL_LZMA_DEFAULT_STRATEGY                           -Normal data.
 *  @li@c  CL_LZMA_FILTERED                                   -Data produced by a filter (or predictor).
 *  @li@c  CL_LZMA_HUFFMAN_ONLY                               -Force Huffman encoding only (no string match).
 *  @li@c  CL_LZMA_RLE                                        - Limit match distances to one (run-length encoding).
 *  @li@c  CL_LZMA_LAST_ONE                                   - No use, just a latest one indicator.
 */
/*------------------------------------------------------------------*/
typedef enum 
{
    CL_LZMA_DEFAULT_STRATEGY = 0,
    CL_LZMA_FILTERED,
    CL_LZMA_HUFFMAN_ONLY,
    CL_LZMA_RLE,
    CL_LZMA_LAST_ONE /* no use, just a latest one indicator*/
} CL_LZMA_COMPRESSION_STRATEGY_T;
/* The strategy parameter is used to tune the compression algorithm. Use the
   value Z_DEFAULT_STRATEGY for normal data, Z_FILTERED for data produced by a
   filter (or predictor), Z_HUFFMAN_ONLY to force Huffman encoding only (no
   string match), or Z_RLE to limit match distances to one (run-length
   encoding). Filtered data consists mostly of small values with a somewhat
   random distribution. In this case, the compression algorithm is tuned to
   compress them better. The effect of Z_FILTERED is to force more Huffman
   coding and less string matching; it is somewhat intermediate between
   Z_DEFAULT and Z_HUFFMAN_ONLY. Z_RLE is designed to be almost as fast as
   Z_HUFFMAN_ONLY, but give better compression for PNG image data. The strategy
   parameter only affects the compression ratio but not the correctness of the
   compressed output even if it is not set appropriately.
 */
/*------------------------------------------------------------------*/
/*! @struct CL_LZMA_COMPRESSION_T
 *  @brief   Compression parameter.
 *  @code
 *  typedef struct _CL_LZMA_COMPRESSION_T
 *  {
 *      UINT8                           ui1_level;
 *      UINT8                           ui1_windowbits;
 *      UINT8                           ui1_mem_level;
 *      CL_LZMA_COMPRESSION_STRATEGY_T   e_strategy;
 *  } CL_LZMA_COMPRESSION_T;
 *  @endcode
 *  @li@c  ui1_level                                         - Compression levels,The compression level must be between 0 and 9: 1 gives best speed, 9 gives best compression, 0 gives no compression at all (the input data is simply copied a block at a time).
 *  @li@c  ui1_windowbits                                    - The windowBits parameter is the base two logarithm of the window size (the size of the history buffer). It should be in the range 8..15 for this version of the library. 
 *  @li@c  ui1_mem_level                                     - The memLevel parameter specifies how much memory should be allocated for the internal compression state.
 *  @li@c  e_strategy                                        - The strategy parameter is used to tune the compression algorithm.
 */
/*------------------------------------------------------------------*/
typedef struct _CL_LZMA_COMPRESSION_T
{
    UINT8                           ui1_level;/*0~9*/
    UINT8                           ui1_windowbits;/*8~15*/
    UINT8                           ui1_mem_level;/*1~9*/
    CL_LZMA_COMPRESSION_STRATEGY_T   e_strategy;
} CL_LZMA_COMPRESSION_T;

/*------------------------------------------------------------------*/
/*! @struct CL_LZMA_DECOMPRESSION_T
 *  @brief The memory requirements for compression depend on two parameters, windowBits and memLevel: deflate memory usage (bytes) = (1 << (windowBits+2)) + (1 << (memLevel+9))  inflate memory usage (bytes) = (1 << windowBits) + 1440*2*sizeof(int)
 *  @code
 *  typedef struct _CL_LZMA_DECOMPRESSION_T
 *  {
 *      UINT8                           ui1_windowbits;
 *  } CL_LZMA_DECOMPRESSION_T;
 *  @endcode
 *  @li@c  ui1_windowbits                                    - The windowBits parameter is the base two logarithm of the window size (the size of the history buffer). It should be in the range 8..15 for this version of the library. 
 */
/*------------------------------------------------------------------*/
typedef struct _CL_LZMA_DECOMPRESSION_T
{
    UINT8                           ui1_windowbits;/*8~15*/
} CL_LZMA_DECOMPRESSION_T;

#define CL_LZMA_ENGINE_COMPRESS_PARAM_TYPE    0x2    /**<Compress parameter type.        */
#define CL_LZMA_ENGINE_DECOMPRESS_PARAM_TYPE  0x3    /**<Decompress parameter type.        */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#endif /* _U_CL_LZMA_ENG_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                                 /* end of groupMW_CL*/
/*----------------------------------------------------------------------------*/
