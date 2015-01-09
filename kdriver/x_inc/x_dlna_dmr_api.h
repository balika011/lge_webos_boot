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
/*! @file x_dlna_dmr_api.h 
*  $RCSfile: $
*  $Revision: #1 $
*  $Date: 2015/01/09 $
*  $Author: dtvbm11 $
*  
*  @par Description:
*         This header file contains DLNA DMR exported API.
*/
/*----------------------------------------------------------------------------*/

/*!
* @addtogroup  groupMW_INET_DLNA
* @brief       Provide an external API about DLNA.
* %DLNA API would be used by user who wants to play media stream in local network  
* 
* @see 
*
* @{
* */


#ifndef X_DLNA_DMR_API_H
#define X_DLNA_DMR_API_H

#include "u_dlna_dmr.h"

/**
 * Utility
 */

/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_start(x_dlna_device_nfy pf_nfy, VOID * pv_tag)
*  @brief  Starts the DMR
*          - Called while who wants to start DMR
*  @param  [in]pf_nfy          - The callback while device found or left
*  @param  [in]pv_tag          - The callback tag
*  
*  @note   
*  @see    x_dlna_dmr_stop  
*  @return Return the error code                
*  @retval DLNAR_OK     	- Start dmr core ok. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmr_start
*
* Description: The API to start dmr kernel
*
* Inputs:  
*
* Outputs: -
*
* Returns: DLNAR_OK - start dmr core ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_start(x_dlna_device_nfy pf_nfy, VOID * pv_tag);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_add_player_listener(x_dlna_dmr_player_nfy pf_nfy, VOID * pv_tag)
*  @brief  Adds the listener to the DMR
*          - Called while who wants to add a listener to DMR
*  @param  [in]pf_nfy          - The callback while player state changed
*  @param  [in]pv_tag          - The callback tag
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_OK     	   - Add ok. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmr_add_player_listener
*
* Description: The API to add dmr player listener
*
* Inputs:  
*
* Outputs: -
*
* Returns: DLNAR_OK - start dmr core ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_add_player_listener(x_dlna_dmr_player_nfy pf_nfy, VOID * pv_tag);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_remove_player_listener(VOID)
*  @brief  Removes the listener to the DMR
*          - Called while who wants to remove a listener to DMR
*  @param  
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_OK     	   - Remove ok. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmr_remove_player_listener
*
* Description: The API to add dmr player listener
*
* Inputs:  
*
* Outputs: -
*
* Returns: DLNAR_OK - start dmr core ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_remove_player_listener(VOID);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_stop(VOID)
*  @brief  Stops the DMR
*          - Called while who wants to stop DMR
*  @param  
*  
*  @note   
*  @see    x_dlna_dmr_start  
*  @return Return the error code                
*  @retval DLNAR_OK     	- stop dmr core ok. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmr_stop
*
* Description: The API to close dmr kernel
*
* Inputs:  -
*
* Outputs: -
*
* Returns: DLNAR_OK - close dmr ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_stop(VOID);


/*----------------------------------------------------------------------------*/
/*! @fn    INT32 x_dlna_dmr_set_file_handler(x_dlna_dmr_file_hdlr pf_handler)
*  @brief  Sets the file handle to the DMR
*          - Called while who wants to add a listener to DMR
*  @param  [in]pf_handler        - The file handler 
*  
*  @note   
*  @see      
*  @return Return the error code                
*  @retval DLNAR_OK     	   - Set ok. 
*/
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
* Name: x_dlna_dmr_set_file_handler
*
* Description: The API to mount uri and protocol info to a local file path
*
* Inputs:  -
*
* Outputs: -
*
* Returns: DLNAR_OK -  ok
----------------------------------------------------------------------------*/
extern INT32 x_dlna_dmr_set_file_handler(x_dlna_dmr_file_hdlr pf_handler);

/*-----------------------------------------------------------------------------
* Name: x_dlna_dmr_getname
*
* Description: The API to get dmr friendly name from dmr.xml
*
* Inputs:  -
*
* Outputs: -
*
* Returns: DLNAR_OK -  ok
*/
extern INT32 x_dlna_dmr_getname(CHAR ** ps_dmr_name);

/*-----------------------------------------------------------------------------
* Name: x_dlna_dmr_setname
*
* Description: The API to set dmr friendly name to dmr.xml
*
* Inputs:  -
*
* Outputs: -
*
* Returns: DLNAR_OK -  ok
*/
extern INT32 x_dlna_dmr_setname(CHAR * ps_dmr_name);

/*-----------------------------------------------------------------------------
* Name: x_dlna_dmr_get_media_info
*
* Description: The API to get dmr media detail info
*
* Inputs:  -
*
* Outputs: -
*
* Returns: DLNAR_OK -  ok
*/
extern INT32 x_dlna_dmr_get_media_info(DLNA_DMR_MEDIA_INFO_T **pt_dmr_media_info);
#endif

/*! @} */

