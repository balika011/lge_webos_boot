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
 * $RCSfile: extmjc_prc_flow.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *
 * History:
 *           
 *---------------------------------------------------------------------------*/

#ifndef __EXTMJC_PRC_FLOW_H_
#define __EXTMJC_PRC_FLOW_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
BOOL extmjc_prc_flow_is_connect (VOID);

VOID extmjc_prc_flow_dump_status(VOID);

VOID extmjc_prc_flow_init(
    UINT8 ui1_max_reliable_msg_cnt,
    UINT8 ui1_noresp_req_buf_cnt,
    UINT8 ui1_withresp_req_buf_cnt);

VOID extmjc_prc_flow_tx_empty_nfy(VOID);

VOID extmjc_prc_flow_timeout_handler(VOID* pv_arg);

INT32 extmjc_prc_flow_send_noresp_req (
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body);

VOID extmjc_prc_flow_rec_noresp_req(
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body);

INT32 extmjc_prc_flow_send_withresp_req (
    UINT8               ui1_send_retry_cnt,
    UINT16              ui2_send_timeout,
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body,
    UINT8*              pui1_inmsg_id);

VOID extmjc_prc_flow_rec_withresp_req(
    BOOL                b_short_inmsg,
    UINT8               ui1_inmsg_id,
    UINT8               ui1_free_inmsg_id,
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body);

INT32 extmjc_prc_flow_send_resp (
    UINT8               ui1_inmsg_id,
    UINT8               ui1_inmsg_type,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body);

VOID extmjc_prc_flow_rec_resp (
    BOOL                b_short_inmsg,
    UINT8               ui1_inmsg_id,
    UINT16              ui2_inmsg_body_len,
    UINT8*              pui1_inmsg_body);


#endif /* __EXTMJC_PRC_FLOW_H_ */

