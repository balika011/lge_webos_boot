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
 * $RCSfile: x_midxbuld.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2015/01/09 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/9 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 907020e011d0dbd7ca156869d249b742 $ Asa
 *
 * Description: 
 *         This file exports all media index builder API and constant for 
 *         other module using.
 *---------------------------------------------------------------------------*/

#ifndef _X_MIDXBULD_H_
#define _X_MIDXBULD_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "u_common.h"
#include "u_handle.h"
#include "u_midxbuld.h"
#include "x_mfmtrecg.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "u_midxbuld.h"
#include "x_mfmtrecg.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/* This API opens a media file to do media indexing. */                        
extern INT32 x_midxbuld_fopen (
                        CHAR*                    ps_path_name, /* (I) specify the path string of file which will be opened in minfo. */ 
                        x_midxbuld_nfy_fct       pf_nfy,       /* (I) specify the function point of notification function. */ 
                        VOID*                    pv_tag,       /* (I) specify the tag which will be send back to AP when minfo notification. */ 
                        MEDIA_FORMAT_T*          pt_format,    /* (I/O) if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file./If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP. */ 
                        MIDXBULD_SOURCE_INFO_T*  pt_src_info,  /* (I) specify the media source information for index building */
                        MIDXBULD_KEY_INFO_T*     pt_key_info,  /* (I/O) specify the key frame information for index building & retrieve handles for key frame. */
                        HANDLE_T*                ph_midxbuld); /* (O) The parameter will retrieve a media index builder handle. */ 


/* This API opens a media memory buffer to do media indexing. */                        
extern INT32 x_midxbuld_mopen (
                        VOID*                    pv_mem_data,  /* (I) specify the memory address of media data buffer. */ 
                        SIZE_T                   z_mem_leng,   /* (I) specify the memory length of media data buffer. */ 
                        x_midxbuld_nfy_fct       pf_nfy,       /* (I) specify the function point of notification function. */ 
                        VOID*                    pv_tag,       /* (I) specify the tag which will be send back to AP when minfo notification. */ 
                        MEDIA_FORMAT_T*          pt_format,    /* (I/O) if the application specify the format (except MEDIA_TYPE_UNKNOWN), media info module will use this information to select the right handler to deal with the media file./If the application specify the unknown format (ie. MEDIA_TYPE_UNKNOWN), media info module will use format recognizer to look it up; then return it to AP. */ 
                        MIDXBULD_SOURCE_INFO_T*  pt_src_info,  /* (I) specify the media source information for index building */
                        MIDXBULD_KEY_INFO_T*     pt_key_info,  /* (I/O) specify the key frame information for index building & retrieve handles for key frame. */
                        HANDLE_T*                ph_midxbuld); /* (O) The parameter will retrieve a media index builder handle. */ 

/* This API closes the media index builder handle. */
extern INT32 x_midxbuld_close (
                        HANDLE_T                 h_midxbuld);  /*(I) specify the media index builder handle which will be closed. */ 

/* This API starts indexing the media.This function call is asynchronous call. */
extern INT32 x_midxbuld_start_indexing (
                        HANDLE_T                 h_midxbuld);  /* (I) specify the media index builder handle which will start indexing*/


/* This API stops indexing the media.This function call is asynchronous call. */
extern INT32 x_midxbuld_stop_indexing (
                        HANDLE_T                 h_midxbuld);  /* (I) specify the media index builder handle which will start indexing*/

/* This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete. */
extern INT32 x_midxbuld_get_info (
                        HANDLE_T                 h_midxbuld,   /* (I) specify the media idx builder handle which will be retrieved information. */ 
                        MIDXBULD_GET_INFO_TYPE_T e_info_type,  /* (I) specify the information type of the media. */ 
                        VOID*                    pv_buf,       /* (I/O) specify the memory buffer which will contain the media information./specify the memory buffer which will contain the media information */ 
                        SIZE_T*                  pz_buf_leng); /* (I/O) specify the length of the buffer./If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length. */ 

/* This API retrieves the information of media idx builder handle. It will return MINFO_NOT_READY, before parse media complete. */
extern INT32 x_midxbuld_set_info (
                        HANDLE_T                 h_midxbuld,   /* (I) specify the media idx builder handle which will be retrieved information. */ 
                        MIDXBULD_SET_INFO_TYPE_T e_info_type,  /* (I) specify the information type of the media. */ 
                        VOID*                    pv_buf,       /* (I/O) specify the memory buffer which will contain the media information./specify the memory buffer which will contain the media information */ 
                        SIZE_T*                  pz_buf_leng); /* (I/O) specify the length of the buffer./If it is smaller than required, it will return MINFO_INSUFFICENT_MEM, and pz_buf_leng will contain required length. */
#ifdef __cplusplus
}
#endif

#endif /* _X_MIDXBULD_H_ */
