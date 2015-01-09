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
 * $RCSfile: api_ycproc.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file Drv_pq_cust.h
 *  Function to Access Quality Value Stored in Drv_cust Section due to these 
 *  value may not be access directly in 8032
 */

#ifndef _DRV_PQ_CUST_H
#define _DRV_PQ_CUST_H

#define GAMMA_INDEX 257
#define SCE_LUMA_HUE_SLICE 16
#define SCE_HUE_SLICE 28
#define SCE_INDEX 1216   // 22*28+8=624, plus 8 for FlashPQ size alignment (16x)

#include "pe_if.h"	// TODO: remove!

//DFT QTY Table
extern tagDFT_QTY aDefaultQtyTbl[QUALITY_MAX];  

#define READ_DEFUALT_QTY_TBL_MIN(index) (aDefaultQtyTbl[(index)].bDftQtyMin)
#define READ_DEFUALT_QTY_TBL_MAX(index) (aDefaultQtyTbl[(index)].bDftQtyMax)
#define READ_DEFUALT_QTY_TBL_DFT(index) (aDefaultQtyTbl[(index)].bDftQtyDft)
#define READ_DEFUALT_QTY_TBL_REF(index) (aDefaultQtyTbl[(index)].wDftQtyRefenence)

#define WRITE_DEFUALT_QTY_TBL_MIN(index,value) {aDefaultQtyTbl[(index)].bDftQtyMin=(UINT8)(value);}
#define WRITE_DEFUALT_QTY_TBL_MAX(index,value) {aDefaultQtyTbl[(index)].bDftQtyMax=(UINT8)(value);}
#define WRITE_DEFUALT_QTY_TBL_DFT(index,value) {aDefaultQtyTbl[(index)].bDftQtyDft=(UINT8)(value);}
#define WRITE_DEFUALT_QTY_TBL_REF(index,value) {aDefaultQtyTbl[(index)].wDftQtyRefenence=(UINT16)(value);}

//QTY_ITEM
extern UINT16 const CODE wCustomQtyItem[FLASHPQ_NUM][CUSTOM_QTY_TBL_DIM + TBL_HEAD_HASH];

#define READ_CUST_QTY_ITEM(j,index)  (wCustomQtyItem[j][index])

//CUST_SRC_TIMING_MAPPING TABLE
extern UINT8 bCustomSrcTimingTbl[];

//CUST_SOURCE_GROUP
extern UINT8 bCustomSourceGroup[][QTY_IDX_RANGE_DIM];

#define READ_CUSTOM_SRC_GROUP(index,item) (bCustomSourceGroup[(index)][(item)])
#define WRITE_CUSTOM_SRC_GROUP(index,item,value) {bCustomSourceGroup[(index)][(item)]=(value);}

//SCE_BY_SOURCE
extern UINT8 SceBySource[][QTY_IDX_RANGE_DIM];
#define READ_Sce_SRC_GROUP(index,item) (SceBySource[(index)][(item)])
#define WRITE_Sce_SRC_GROUP(index,item,value) {SceBySource[(index)][(item)]=(value);}

//CUST_QTY_TBL 
#ifdef CC_MAPLE_CUST_DRV
extern UINT8 bCustomQtyTbl[FLASHPQ_NUM][27][CUSTOM_QTY_TBL_DIM];
#else
extern UINT8 bCustomQtyTbl[FLASHPQ_NUM][26][CUSTOM_QTY_TBL_DIM];
#endif
#define READ_CUSTOM_QTY_TBL(timing,item) (bCustomQtyTbl[((item>>QTY_TBL_REF_INDEX_SHIFT)<FLASHPQ_NUM)?(item>>QTY_TBL_REF_INDEX_SHIFT):0][MIN(timing, 25)][item &0x3FFF])
#define WRITE_CUSTOM_QTY_TBL(timing,item,value) {bCustomQtyTbl[item>>QTY_TBL_REF_INDEX_SHIFT][MIN(timing, 25)][item &0x3FFF]=(value);}
    
//SCE    
extern UINT8 DEFAULT_SCE_TBL[SCE_INDEX];
extern UINT8 BASE_SCE_TBL[SCE_INDEX];
extern UINT8 MIN_SCE_TBL[SCE_INDEX];

#define READ_SCE_TBL(index1,index2) (DEFAULT_SCE_TBL[(index1)*SCE_HUE_SLICE + (index2)])
#define WRITE_SCE_TBL(index1,index2,value) {DEFAULT_SCE_TBL[(index1)*SCE_HUE_SLICE + (index2)]=(UINT8)(value);}

//DEMO Position    
extern UINT16 const CODE DEMO_MODE_BEFORE_SCALER_ARG[][DEMO_MODE_BEFORE_SCALER_END];
extern UINT16 const CODE DEMO_MODE_AFTER_SCALER_ARG[][DEMO_MODE_AFTER_SCALER_END];

#define READ_DEMO_BFR_SCALER_ARG(timing,EngineType) (DEMO_MODE_BEFORE_SCALER_ARG[(timing)][(EngineType)])
#define READ_DEMO_AFT_SCALER_ARG(timing,EngineType) (DEMO_MODE_AFTER_SCALER_ARG[(timing)][(EngineType)])

//GAMMA TBL
extern UINT8 GAMMA_256[3][GAMMA_INDEX];

enum
{
    E_PQ_SNR            = 0,
    E_PQ_MNR            = 1,
    E_PQ_BNR            = 2,
    E_PQ_TNR            = 3,
    E_PQ_CBE            = 4,
    E_PQ_DERING         = 5,
    E_PQ_HSHARP         = 6,
    E_PQ_CTIF           = 7,
    E_PQ_CTIR           = 8,
    E_PQ_LTI            = 9,
    E_PQ_TDSHARP        = 10,
    E_PQ_SCE            = 11,
    E_PQ_ADL            = 12,    
    E_PQ_POST_MATRIX    = 13,
    E_PQ_BLUE_STRETCH   = 14,
    E_PQ_XVYCC          = 15,
    E_PQ_RGB_GAIN       = 16,
    E_PQ_RGB_OFFSET     = 17,
    E_PQ_DBL            = 18,
    E_PQ_GAMMA          = 19,
    E_PQ_LCDIM          = 20,
    E_PQ_OD             = 21,
    E_PQ_MAX            = 22       
};

EXTERN UINT8 DRVCUST_Get4k2kPQRule(UINT8 u1Module);
      
#endif
