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
/*----------------------------------------------------------------------------*/
/*! @file 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains DRM specific definitions,
 *         which are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_DRM DRM
 *
 *  @ingroup groupMW
 *  @brief Provides a unified interface for DRM
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_DRM_H_
#define _U_DRM_H_
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*! @struct DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T
 *  @brief  This structure defines the drm output signal protection info.
 *  @code
 *  typedef struct _DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T
 *  {
 *  #define CGMSA_COPYFREELY ((UINT8)0)
 *  #define CGMSA_COPYNOMORE ((UINT8)1)
 *  #define CGMSA_COPYONCE ((UINT8)2)
 *  #define CGMSA_COPYNEVER ((UINT8)3)
 *      UINT8 ui1_cgmsa_flag; 
 *  #define ACPTB_OFF ((UINT8)0)
 *  #define ACPTB_AGC ((UINT8)1)
 *  #define ACPTB_2LCB ((UINT8)2)
 *  #define ACPTB_4LCB ((UINT8)3)                                             
 *      UINT8 ui1_acptb_flag;   
 *  #define DIGITAL_PROTECTION_OFF ((UINT8)0)
 *  #define DIGITAL_PROTECTION_ON ((UINT8)1)                                            
 *      UINT8 ui1_digital_protection_flag;  
 *  #define ICT_OFF ((UINT8)0)
 *  #define ICT_ON ((UINT8)1)
 *      UINT8 ui1_ict_flag;
 *  } DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T;
 *  @endcode
 *  @li@c  ui1_cgmsa_flag               - indicate CGMSA parameter.
 *  @li@c  ui1_acptb_flag               - indicate ACPTB parameter.
 *  @li@c  ui1_digital_protection_flag  - indicate Digital Protection parameter.
 *  @li@c  ui1_ict_flag                 - indicate ICT parameter. 
 */
/*---------------------------------------------------------------------------*/
typedef struct _DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T
{
/* 
   CGMSA
   00 - CopyFreely
   01 - CopyNoMore
   10 - CopyOnce
   11 - CopyNever
*/    
#define CGMSA_COPYFREELY ((UINT8)0)
#define CGMSA_COPYNOMORE ((UINT8)1)
#define CGMSA_COPYONCE ((UINT8)2)
#define CGMSA_COPYNEVER ((UINT8)3)
    UINT8 ui1_cgmsa_flag; 
/*
   ACPTB
   00 - off
   01 - automatic gain control
   10 - 2 line color burst
   11 - 4 line color burst
*/
#define ACPTB_OFF ((UINT8)0)
#define ACPTB_AGC ((UINT8)1)
#define ACPTB_2LCB ((UINT8)2)
#define ACPTB_4LCB ((UINT8)3)                                             
    UINT8 ui1_acptb_flag;   
/*
   Digital Protection
   0 - off
   1 - on
*/
#define DIGITAL_PROTECTION_OFF ((UINT8)0)
#define DIGITAL_PROTECTION_ON ((UINT8)1)                                            
    UINT8 ui1_digital_protection_flag;  
/*
   ICT
   0 - analog HD allowed
   1 - analog image must be constrained
*/
#define ICT_OFF ((UINT8)0)
#define ICT_ON ((UINT8)1)
    UINT8 ui1_ict_flag;
} DRM_OUTPUT_SIGNAL_PROTECTION_INFO_T;

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* _U_DRM_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                               /* end of groupMW_DRM */
/*----------------------------------------------------------------------------*/
