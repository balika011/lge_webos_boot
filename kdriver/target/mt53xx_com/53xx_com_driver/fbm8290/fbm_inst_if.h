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
 * $RCSfile: fbm_inst_if.h,v $
 * $Revision: #1 $

 *
 *---------------------------------------------------------------------------*/

/** @file fbm_inst_if.h
 *  API of FBM (Frame Buffer Manager) instance
 *  Details of file fbm_inst_drvif.h (optional).
 */

#ifndef FBM_INST_IF_H
#define FBM_INST_IF_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

//#include "x_typedef.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define FBM_INST_ID_UNKNOWN  0xFF

#define FBM_STATUS_OK 0
#define FBM_UNEXPECTED_ERROR 1
#define FBM_INVALID_ARGUMENT 2

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

#if !defined (_TYPEDEF_FBM_UCHAR_)
typedef unsigned char  UCHAR_T;
#define _TYPEDEF_FBM_UCHAR_
#endif

#if !defined (_TYPEDEF_FBM_UINT32_)
typedef unsigned long    UINT32_T;
#define _TYPEDEF_FBM_UINT32_
#endif

#if !defined (_TYPEDEF_FBM_BOOL_)
typedef unsigned char  BOOL_T;
#define _TYPEDEF_FBM_BOOL_

#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif
#define TRUE ((BOOL_T) 1)
#define FALSE ((BOOL_T) 0)
#endif


typedef enum
{
    FBM_CFMT_420,
    FBM_CFMT_422,
    FBM_CFMT_444,
    FBM_CFMT_ARGB
} FBM_ColorFMT_T;

typedef enum
{
    FBM_H264,
    FBM_MPEG2,
    FBM_MPEG4,
    FBM_VC1,
    FBM_JPEG,
    FBM_RV,
    FBM_AVS,
    FBM_RAW,
    FBM_VP6,
    FBM_VP8,
    FBM_H265
} FBM_VDEC_FORMAT;

typedef enum
{
    FBM_FRAME_SIZE,
    FBM_FRAME_COUNT
} FBM_INST_INFO_TYPE;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

UCHAR_T FBM_GetInstance(FBM_VDEC_FORMAT eVDecFormat, UINT32_T u4Width, UINT32_T u4Height, FBM_ColorFMT_T eColorMode, UINT32_T u4FrameNum, BOOL_T fgAlloc, UINT32_T u4ExtOption);
UINT32_T FBM_FreeInstance(UCHAR_T ucInstId);
UINT32_T FBM_FreeBuf(UCHAR_T ucInstId, UINT32_T u4Addr);
UINT32_T FBM_AllocBuf(UCHAR_T ucInstId, UINT32_T *pu4Addr);
UINT32_T FBM_Alloc_YCBuf(UCHAR_T ucInstId, UINT32_T *pu4AddrY,UINT32_T *pu4AddrC);
UINT32_T FBM_QueryBufNum(FBM_VDEC_FORMAT eVDecFormat, UINT32_T u4Width,
    UINT32_T u4Height, FBM_ColorFMT_T eColorMode, UINT32_T *pu4BufNum,
    UINT32_T *pu4Stride, UINT32_T *pu4SliceHeight,
    UINT32_T *pu4UVAddrOffset, UINT32_T u4ExtOption);
UINT32_T FBM_GetInstInfo(UCHAR_T ucInstId, FBM_INST_INFO_TYPE eInfoType, UINT32_T *pInfoResult);
#endif    // FBM_INST_IF_H


