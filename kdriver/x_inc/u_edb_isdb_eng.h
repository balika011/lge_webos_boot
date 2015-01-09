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
/*! @file u_edb_isdb_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         EDB ISDB engine.
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_EDB_EDB_ISDB_ENG EDB ISDB Engine
 *  @ingroup groupMW_EDB
 *  @brief Provides events operate function for ISDB
 *  %This module is EDB ISDB instance.
 *  @see groupMW_EDB
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


#ifndef _U_EDB_ISDB_ENG_H_
#define _U_EDB_ISDB_ENG_H_

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/
#include "u_common.h"


/*-----------------------------------------------------------------------------
  macros, defines, typedefs, enums
  ----------------------------------------------------------------------------*/
typedef UINT32      EDB_ISDB_CONFIG_T;    /**< EDB ISDB Engine config type       */

#define EDB_ISDB_CONFIG_PARSE_EXTEND_ITEMS                         (MAKE_BIT_MASK_32(0))    /**< Config of parse extended event items*/
#define EDB_ISDB_CONFIG_USE_MAX_EVENT_NAME                         (MAKE_BIT_MASK_32(1))    /**< Config of use max length retrive content*/


#endif /* _U_EDB_ISDB_ENG_H_ */


/*----------------------------------------------------------------------------*/
/*! @} */                                 /* end of groupMW_EDB */
/*----------------------------------------------------------------------------*/


