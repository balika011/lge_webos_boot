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
/*! @file u_dsmcc_db.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains DSMCC Data Broadcast specific
 *         definitions, which are exported.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_TBL_MNGR_DSMCC_DB DSM-CC Data Broadcast
 *  @ingroup groupMW_TBL_MNGR
 *  @brief Provides DSM-CC Data Broadcast related functions
 *  %This module allows an application to retrieve data from DSM-CC Data Carousel
 *  @see groupMW_TBL_MNGR_DSMCC_ENG, groupMW_TBL_MNGR_DSMCC_OC
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_DSMCC_DB_H_
#define _U_DSMCC_DB_H_

/*---------------------------------------------------------------------------
                    include files
---------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_tm.h"


/*---------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 --------------------------------------------------------------------------*/


/* DSMCC-DB handle types. */
#define TMT_DSMCC_DB_MODULE  (TMT_DSMCC + ((HANDLE_TYPE_T)  6))    /**< DSM-CC module handle type */

/* DSMCC-DB get types. */
#define DSMCC_DB_GET_TYPE_MODULE_RD  (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 16))    /**< DSM-CC get module get type */

/* DSMCC-DB load types. */
#define DSMCC_DB_LOAD_TYPE_MODULE  (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 16))    /**< DSM-CC load module load type */

#define DSMCC_DB_LOAD_TYPE_MODULE_EX  (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 17))    /**< DSM-CC load module load type */


/* DSMCC-DB specification */
/*------------------------------------------------------------------*/
/*! @enum  DSMCC_DB_SPEC_T
 *  @brief Indicates if DSM-CC should parse GroupInfoIndication object according to which specification.
 *  @code
 *  typedef enum
 *  {
 *      DSMCC_DB_SPEC_EN_301_192 = 0,
 *      DSMCC_DB_SPEC_TS_102_006
 *  }   DSMCC_DB_SPEC_T;
 *  @endcode
 *  @li@c  DSMCC_DB_SPEC_EN_301_192 - Follow EN 301 192 specification.
 *  @li@c  DSMCC_DB_SPEC_TS_102_006 - Follow TS 102 006 specification.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DSMCC_DB_SPEC_EN_301_192 = 0,
    DSMCC_DB_SPEC_TS_102_006
}   DSMCC_DB_SPEC_T;

/* Returns the group info from a DSI. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_DB_GROUP_INFO_T
 *  @brief  Indicates the information of a group.
 *  @code
 *  typedef struct _DSMCC_DB_GROUP_INFO_T
 *  {
 *      UINT32  ui4_group_id;
 *      UINT32  ui4_group_size;
 *  }   DSMCC_DB_GROUP_INFO_T;
 *  @endcode
 *  @li@c  ui4_group_id   - Group ID.
 *  @li@c  ui4_group_size - Group size.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_DB_GROUP_INFO_T
{
    UINT32  ui4_group_id;
    UINT32  ui4_group_size;
}   DSMCC_DB_GROUP_INFO_T;

/* Structure to read a whole or partial module. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_DB_MODULE_RD_INFO_T
 *  @brief  Read information of a module.
 *  @code
 *  typedef struct _DSMCC_DB_MODULE_RD_INFO_T
 *  {
 *      HANDLE_T  h_dii;                                       
 *      UINT32  ui4_block_start;                               
 *      UINT32  ui4_block_offs;                                
 *      UINT32  ui4_block_size;                                
 *      UINT32  ui4_timeout;                                   
 *      UINT16  ui2_module_idx;                                
 *  }   DSMCC_DB_MODULE_RD_INFO_T;
 *  @endcode
 *  @li@c  h_dii           - The DII handle used for the request.
 *  @li@c  ui4_block_start - First DDB block number.
 *  @li@c  ui4_block_offs  - Offset of the first DDB block.
 *  @li@c  ui4_block_size  - The size of a single DDB block.
 *  @li@c  ui4_timeout     - Timeout value for the parked client.
 *  @li@c  ui2_module_idx  - Module index.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_DB_MODULE_RD_INFO_T
{
    HANDLE_T  h_dii;                                       /* The DII handle used for the request. */

    UINT32  ui4_block_start;                               /* First DDB block number. */
    UINT32  ui4_block_offs;                                /* Offset of the first DDB block. */
    UINT32  ui4_block_size;                                /* The size of a single DDB block. */

    UINT32  ui4_timeout;                                   /* Timeout value for the parked client. */

    UINT16  ui2_module_idx;                                /* Module index. */
}   DSMCC_DB_MODULE_RD_INFO_T;

/* Structure notified with TM_COND_LOAD_PROGRESS. */
/* Notify a client with the load progress.        */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_DB_LOAD_PROGRESS_T
 *  @brief  Indicates the load progress of a module.
 *  @code
 *  typedef struct _DSMCC_DB_LOAD_PROGRESS_T
 *  {
 *      UINT32  ui4_curr;
 *      UINT32  ui4_total;
 *  }   DSMCC_DB_LOAD_PROGRESS_T;
 *  @endcode
 *  @li@c  ui4_curr  - Number of bytes loaded.
 *  @li@c  ui4_total - Total number of bytes to load.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_DB_LOAD_PROGRESS_T
{
    UINT32  ui4_curr;
    UINT32  ui4_total;
}   DSMCC_DB_LOAD_PROGRESS_T;


/* Structure associated with DSMCC_DB_LOAD_TYPE_MODULE. */
/*------------------------------------------------------------------*/
/*! @struct DSMCC_DB_MODULE_T
 *  @brief  Information of a module.
 *  @code
 *  typedef struct _DSMCC_DB_MODULE_T
 *  {
 *      UINT8*  pui1_buff;                                     
 *      SIZE_T  z_buff_len;                                    
 *      UINT32  ui4_block_start;                               
 *      UINT32  ui4_block_offs;                                
 *      UINT32  ui4_block_size;  
 *      UINT8*  pui1_buff_2;                                     
 *      SIZE_T  z_buff_len_2;                                    
 *      UINT32  ui4_block_start_2;                               
 *      UINT32  ui4_block_offs_2;                                
 *      UINT32  ui4_block_size; 
 *      HANDLE_T  h_dii;                                       
 *      UINT16  ui2_module_idx;                                
 *  }   DSMCC_DB_MODULE_T;
 *  @endcode
 *  @li@c  pui1_buff       - Client buffer.
 *  @li@c  z_buff_len      - Client buffer length.
 *  @li@c  ui4_block_start - First DDB block number.
 *  @li@c  ui4_block_offs  - Offset of the first DDB block.
 *  @li@c  ui4_block_size  - The size of a single DDB block.
 *  @li@c  pui1_buff       - Client buffer 2.
 *  @li@c  z_buff_len      - Client buffer 2 length.
 *  @li@c  ui4_block_start - First DDB block number for buffer 2.
 *  @li@c  ui4_block_offs  - Offset of the first DDB block for buffer 2.
 *  @li@c  ui4_block_size  - The size of a single DDB block for buffer 2.
 *  @li@c  h_dii           - The DII handle used for the request.
 *  @li@c  ui2_module_idx  - Module Index.
 */
/*------------------------------------------------------------------*/
typedef struct _DSMCC_DB_MODULE_T
{
    UINT8*  pui1_buff;                                     /* Client buffer. */
    
    SIZE_T  z_buff_len;                                    /* Client buffer length. */

    UINT32  ui4_block_start;                               /* First DDB block number. */
    UINT32  ui4_block_offs;                                /* Offset of the first DDB block. */

    
    UINT8*  pui1_buff_2;                                   /* Client buffer 2. */
    
    SIZE_T  z_buff_len_2;                                  /* Client buffer 2 length. */

    UINT32  ui4_block_start_2;                             /* First DDB block number for buffer 2. */
    UINT32  ui4_block_offs_2;                              /* Offset of the first DDB block for buffer 2. */
    
    UINT32  ui4_block_size;                                /* The size of a single DDB block. */

    HANDLE_T  h_dii;                                       /* The DII handle used for the request. */

    UINT16  ui2_module_idx;                                /* Module Index. */
}   DSMCC_DB_MODULE_T;

/* DATA_OBJ_T used to control the acquisition of data from the TS. */
/*------------------------------------------------------------------*/
/*! @struct DATA_OBJ_T
 *  @brief  Information of a module.
 *  @code
 *  typedef struct _DATA_OBJ_T 
 *   {
 *       UINT8*  pui1_buff;                                     
 *       UINT8*  pui1_buff_2;                                     
 *       
 *       UINT8*  pui1_ddb_status;                               
 *       UINT8*  pui1_ddb_status_2;                               
 *
 *       HANDLE_T  h_ddb;                                       
 *
 *       UINT32  ui4_first_block_num;                           
 *       UINT32  ui4_last_block_num;                            
 *       UINT32  ui4_first_block_offs;                          
 *       UINT32  ui4_last_block_len;                            
 *       
 *       UINT32  ui4_first_block_num_2;                           
 *       UINT32  ui4_last_block_num_2;                           
 *       UINT32  ui4_first_block_offs_2;                         
 *       UINT32  ui4_last_block_len_2;                           
 *
 *       UINT32  ui4_block_size;                                
 *
 *       UINT32  ui4_load_progress;                             
 *       UINT32  ui4_buff_len;                                 
 *       UINT32  ui4_buff_len_2;                                  
 *
 *       BOOL    b_avail_buff;
 *       BOOL    b_avail_buff_2;
 *       BOOL    b_full_buff;
 *       BOOL    b_full_buff_2;
 *
 *       BOOL    b_complete_all;
 *       BOOL    b_complete_one_buff;
 *   }   DATA_OBJ_T;
 *  @endcode
 *  @li@c  pui1_buff                  - The clients buffer.
 *  @li@c  pui1_buff                  - The clients buffer 2.
 *  @li@c  pui1_ddb_status            - Bit array which keeps track of the acquired DDB blocks.
 *  @li@c  pui1_ddb_status            - Bit array which keeps track of the acquired DDB blocks for buffer 2.
 *  @li@c  h_ddb                      - Handle to DDB object.
 *  @li@c  ui4_first_block_num        - First DDB block number.
 *  @li@c  ui4_last_block_num         - Last DDB block number.
 *  @li@c  ui4_first_block_offs       - Offset in first DDB block.
 *  @li@c  ui4_last_block_len         - Length in last DDB block.
 *  @li@c  ui4_first_block_num_2      - First DDB block number for buffer 2.
 *  @li@c  ui4_last_block_num_2       - Last DDB block number for buffer 2.
 *  @li@c  ui4_first_block_offs_2     - Offset in first DDB block for buffer 2.
 *  @li@c  ui4_last_block_len_2       - Length in last DDB block for buffer 2.
 *  @li@c  ui4_block_size             - The size of a single DDB block.
 *  @li@c  ui4_load_progress          - Load progression.
 *  @li@c  ui4_buff_len               - Length of client buffer.
 *  @li@c  ui4_buff_len_2             - Length of client buffer 2.
 *  @li@c  b_avail_buff               - Availability of buffer.
 *  @li@c  b_avail_buff_2             - Availability of buffer 2.
 *  @li@c  b_full_buff                - Buffer full or not.
 *  @li@c  b_full_buff_2              - Buffer 2 full or not.
 *  @li@c  b_complete_all             - ALL DDB blockes have been required.
 *  @li@c  b_complete_one_buff        - One of buffers is full.
 */
/*------------------------------------------------------------------*/

typedef struct _DATA_OBJ_T
{
    UINT8*  pui1_buff;                  /* The clients buffer. */
    UINT8*  pui1_buff_2;                /* The clients buffer 2. */
    
    UINT8*  pui1_ddb_status;            /* Bit array which keeps track of the acquired DDB blocks. */
    UINT8*  pui1_ddb_status_2;          /* Bit array which keeps track of the acquired DDB blocks for buffer 2. */

    HANDLE_T  h_ddb;                    /* Handle to DDB object. */

    UINT32  ui4_first_block_num;        /* First DDB block number. */
    UINT32  ui4_last_block_num;         /* Last DDB block number. */
    UINT32  ui4_first_block_offs;       /* Offset in first DDB block. */
    UINT32  ui4_last_block_len;         /* Length in last DDB block. */
    
    UINT32  ui4_first_block_num_2;      /* First DDB block number for buffer 2. */
    UINT32  ui4_last_block_num_2;       /* Last DDB block number for buffer 2. */
    UINT32  ui4_first_block_offs_2;     /* Offset in first DDB block for buffer 2. */
    UINT32  ui4_last_block_len_2;       /* Length in last DDB block for buffer 2. */

    UINT32  ui4_block_size;             /* The size of a single DDB block. */

    UINT32  ui4_load_progress;          /* Load progression. */
    UINT32  ui4_buff_len;               /* Length of client buffer. */
    UINT32  ui4_buff_len_2;             /* Length of client buffer 2. */

    BOOL    b_avail_buff;               /* Availability of buffer. */
    BOOL    b_avail_buff_2;             /* Availability of buffer 2. */
    BOOL    b_full_buff;                /* Buffer full or not. */
    BOOL    b_full_buff_2;              /* Buffer 2 full or not. */

    BOOL    b_complete_all;             /* ALL DDB blockes have been required. */
    BOOL    b_complete_one_buff;        /* One of buffers is full. */
}   DATA_OBJ_T;

#endif /* _U_DSMCC_DB_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                 /* end of groupMW_TBL_MNGR_DSMCC_DB */
/*----------------------------------------------------------------------------*/


