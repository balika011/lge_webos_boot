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
/*! @addtogroup groupMW_STRM_MNGR_MM_SBTL_HDLR
*  @{ 
*/
/*----------------------------------------------------------------------------*/
#ifndef _X_SBTL_IPCC_UTIL_H_
#define _X_SBTL_IPCC_UTIL_H_
#ifndef __KERNEL__

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_os.h"


#include "u_sbtl_ipcc.h"


/*-----------------------------------------------------------------------------
                    function declaration
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to divide ps_str by ps_rgx
 *
 * @param [in]      ps_rgx        - String separator
 * @param [in]      ps_str        - String which will be splited
 * @param [in]      ps_rslt       - Save the result after split
 * @param [in]      i4_len        - The length of ps_rslt, if ps_rslt is a pointer of array, then i4_len will be the length of array
 * @param [in]      ps_rest_str   - Save the rest string after splited
 * @param [out]     ps_rslt       - Save the result after split  
 * @return return the error code
 * @retval MMSBTLR_OK              - Routine successful.
 * @retval MMSBTLR_INV_ARG         - Invalid para.
 * @retval MMSBTLR_NOT_FOUND       - ps_rgx was not found in ps_str
 */
/* ----------------------------------------------------------------------------*/
INT32 x_sbtl_ipcc_str_split(CHAR* ps_rgx, CHAR* ps_str, CHAR** ps_rslt, INT32 i4_len, CHAR* ps_rest_str);



/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to convert string to float
 *
 * @param [in]      ps_src_str        - String which will be convert
 * @param [out]     pf_rslt           - The result after convert  
 * @return return the error code
 * @retval MMSBTLR_OK              - Routine successful.
 * @retval MMSBTLR_INV_ARG         - Invalid para.
 */
/* ----------------------------------------------------------------------------*/
INT32 x_sbtl_ipcc_str_to_float(CHAR* ps_src_str, FLOAT* pf_rslt);




/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to detect lenght unit
 *
 * @param [in]      ps_str          - String which will be detect
 * @param [out]     pe_origin_type  - The result after detect  
 * @return return the error code
 * @retval MMSBTLR_OK              - Routine successful.
 * @retval MMSBTLR_INV_ARG         - Invalid para.
 */
/* ----------------------------------------------------------------------------*/
INT32 x_sbtl_ipcc_util_detect_length_unit(CHAR* ps_str, TTML_LENGTH_UNIT_TYPE* pe_origin_type);




/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to abstract value from string
 *
 * @param [in]      ps_string          - String which will be abstract
 * @param [in]      pe_length_unit     - Length unit
 * @param [out]     ps_row             -   
 * @param [out]     ps_col             - 
 * @return return the error code
 * @retval MMSBTLR_OK              - Routine successful.
 * @retval MMSBTLR_INV_ARG         - Invalid para.
 */
/* ----------------------------------------------------------------------------*/
INT32 x_sbtl_ipcc_util_abstract_value(CHAR* ps_string, TTML_LENGTH_UNIT_TYPE* pe_length_unit, 
                                        CHAR* ps_row, CHAR* ps_col);





/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to detect the current character is whitespace or not
 *
 * @param [in]      c_char          - Current character
 * @return return the error code
 * @retval TRUE
 * @retval FALSE 
 */
/* ----------------------------------------------------------------------------*/
BOOL x_sbtl_ipcc_util_is_space (CHAR  c_char);





/*-----------------------------------------------------------------------------*/
/*! @brief  This API is used to calculate the row count of the specify text
 *
 * @param [in]      ps_text          - Current text
 * @param [out]     pi4_row_count    - Row count
 * @return return the error code
 * @retval MMSBTLR_OK              - Routine successful.
 * @retval MMSBTLR_INV_ARG         - Invalid para.
 */
/* ----------------------------------------------------------------------------*/
INT32 x_sbtl_ipcc_util_calculate_row_count(const CHAR* ps_text, INT32* pi4_row_count);






#endif
#endif/* _X_SBTL_IPCC_UTIL_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */   /* end of  groupMW_STRM_MNGR_MM_SBTL_HDLR*/
/*----------------------------------------------------------------------------*/



