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
 * $RCSfile: drv_pq_cust.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


//-----------------------------------------------------------------------------
/** Brief

 * @file Implement the PQ Customization
 * For compatibility of 8032, 
 * All Items Put in DRAM May not be able to access,
 * Directly , thus CAN NOT Access array in drv_cust directly,
 * drv_pq_cust.o will be the interface between DRV_CUST_LIB and other PQ Driver
 */

#include "drv_pq_cust.h"
#include "drv_dram.h"
#include "pe_if.h"
#include "drv_ycproc.h"

#ifdef  CC_UP8032_ATV 

//These Array Will Put directly in FLASH
extern const UINT16 CODE wCustomQtyItem[CUSTOM_QTY_TBL_DIM];

//These Function Will Put In DRAM
extern const tagDFT_QTY  CODE aDefaultQtyTbl[QUALITY_MAX];
extern const UINT8 CODE bCustomSourceGroup[][QTY_IDX_RANGE_DIM];
extern const UINT8 CODE bCustomQtyTbl[][CUSTOM_QTY_TBL_DIM];
extern const UINT8 CODE DEFAULT_SCE_TBL[SCE_INDEX];
extern const UINT8 CODE GAMMA_256[3][257];

//Copy All Customization Item into DRAM     
void vInitDrvPQCustomization(void)
{
    x_memcpy(aDefaultQtyTbl     ,ADDR_DFT_PQ_TBL    , SIZE_OF_DEF_QTY_TBL);
    x_memcpy(bCustomSourceGroup ,ADDR_CUST_SRC_TBL  , SIZE_OF_CUST_SRC_TBL);    
    x_memcpy(bCustomQtyTbl      ,ADDR_CUST_PQ_TBL   , SIZE_OF_CUST_QTY_TBL);
    x_memcpy(DEFAULT_SCE_TBL    ,ADDR_DEF_SCE_TBL   , SIZE_OF_SCE_TBL);
    x_memcpy(GAMMA_256   ,ADDR_GAMMAE_BRIGHT , SIZE_OF_PER_GAMMA);
}


UINT16 wReadCustomQualityItem(UINT16 wIndex)
{
    //Array Put in Diferent Bank, Must Access By Function
    return wCustomQtyItem[wIndex];
}

#else


#endif


UINT8 u14k2kPQRule[E_PQ_MAX] = 
{
    SV_OFF,     // E_PQ_SNR             0
    SV_OFF,     // E_PQ_MNR             1
    SV_OFF,     // E_PQ_BNR             2
    SV_OFF,     // E_PQ_TNR             3
    SV_OFF,     // E_PQ_CBE             4
    SV_OFF,     // E_PQ_DERING          5
    SV_OFF,     // E_PQ_HSHARP          6
    SV_OFF,     // E_PQ_CTIF            7
    SV_OFF,     // E_PQ_CTIR            8
    SV_OFF,     // E_PQ_LTI             9
    SV_OFF,     // E_PQ_TDSHARP         10
    SV_ON,      // E_PQ_SCE             11
    SV_ON,      // E_PQ_ADL             12    
    SV_ON,      // E_PQ_POST_MATRIX     13
    SV_ON,      // E_PQ_BLUE_STRETCH    14
    SV_ON,      // E_PQ_XVYCC           15
    SV_ON,      // E_PQ_RGB_GAIN        16
    SV_ON,      // E_PQ_RGB_OFFSET      17
    SV_ON,      // E_PQ_DBL             18
    SV_ON,      // E_PQ_GAMMA           19
    SV_OFF,     // E_PQ_LCDIM           20
    SV_OFF      // E_PQ_OD              21
};

UINT8 DRVCUST_Get4k2kPQRule(UINT8 u1Module)
{
    return u14k2kPQRule[u1Module];
}


