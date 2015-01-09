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
 * $Date  $
 * $RCSfile: hw_ospe.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

// *********************************************************************
// Down Scaler related Register and Macro
// *********************************************************************
#ifndef _HW_LDM_PQ_V2_H_
#define _HW_LDM_PQ_V2_H_

#include "typedef.h"
#include "x_hal_5381.h"
#include "hw_vdoin.h"

#define LDM_OFFSET (VDOIN_COLOR_BASE+0xD000)

//Page LOCDIM2_1
#define LD2_MODE_00 (LDM_OFFSET + 0x800)
    #define C_LD_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_LD_BYPASS Fld(1,30,AC_MSKB3)//[30:30]
    #define C_LD_3D_MODE Fld(2,28,AC_MSKB3)//[29:28]
    #define C_LD_BLK_VERT_NUM Fld(4,22,AC_MSKW32)//[25:22]
    #define C_LD_BLK_HOR_NUM Fld(5,17,AC_MSKB2)//[21:17]
    #define C_LD_SCENE_CHG Fld(1,16,AC_MSKB2)//[16:16]
    #define C_LD_DIS_DS Fld(1,13,AC_MSKB1)//[13:13]
    #define C_LD_DBUF_ON Fld(1,12,AC_MSKB1)//[12:12]
    #define C_LD_3D_PHASE Fld(2,10,AC_MSKB1)//[11:10]
    #define C_LD_MAX_PXL_TH Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_MODE_01 (LDM_OFFSET + 0x804)
    #define C_LD_BLK_NUM Fld(8,24,AC_FULLB3)//[31:24]
    #define C_LD_BLK_HEIGHT Fld(11,13,AC_MSKW21)//[23:13]
    #define C_LD_BLK_WIDTH Fld(13,0,AC_MSKW10)//[12:0]
#define LD2_MODE_02 (LDM_OFFSET + 0x808)
    #define C_LD_BLK_CTR_MODE Fld(3,29,AC_MSKB3)//[31:29]
    #define C_LD_BLK_CTR_V1 Fld(11,16,AC_MSKW32)//[26:16]
    #define C_LD_BLK_CTR_H1 Fld(13,0,AC_MSKW10)//[12:0]
#define LD2_MODE_03 (LDM_OFFSET + 0x80C)
    #define C_LD_INK Fld(4,27,AC_MSKB3)//[30:27]
    #define C_LD_BLK_CTR_V2 Fld(11,16,AC_MSKW32)//[26:16]
    #define C_LD_BLK_CTR_H2 Fld(13,0,AC_MSKW10)//[12:0]
#define LD2_MODE_04 (LDM_OFFSET + 0x810)
    #define C_LD_DIM_WET_3 Fld(6,24,AC_MSKB3)//[29:24]
    #define C_LD_DIM_WET_2 Fld(6,16,AC_MSKB2)//[21:16]
    #define C_LD_DIM_WET_1 Fld(6,8,AC_MSKB1)//[13:8]
    #define C_LD_DIM_WET_0 Fld(6,0,AC_MSKB0)//[5:0]
#define LD2_MODE_05 (LDM_OFFSET + 0x814)
    #define C_LD_DIM_WET_4 Fld(6,22,AC_MSKW32)//[27:22]
    #define C_LD_MAX_DIM Fld(10,12,AC_MSKW21)//[21:12]
    #define C_LD_MIN_DIM Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_MODE_06 (LDM_OFFSET + 0x818)
    #define C_LD_BLKCONF_MODE Fld(1,30,AC_MSKB3)//[30:30]
    #define C_LD_BLKCONF_ZERO_STEP Fld(5,25,AC_MSKB3)//[29:25]
    #define C_LD_DIM_STEP_SHIFT Fld(2,23,AC_MSKW32)//[24:23]
    #define C_LD_DY_DIM_SLOPE Fld(6,17,AC_MSKB2)//[22:17]
    #define C_LD_DY_DIM_OFST Fld(5,12,AC_MSKW21)//[16:12]
#define LD2_MODE_07 (LDM_OFFSET + 0x81C)
    #define C_LD_LP_REF_COEF2 Fld(5,23,AC_MSKW32)//[27:23]
    #define C_LD_LP_REF_COEF Fld(5,18,AC_MSKB2)//[22:18]
    #define C_LD_LP_VERT_RATIO Fld(3,15,AC_MSKW21)//[17:15]
    #define C_LD_LP_HOR_RATIO Fld(3,12,AC_MSKB1)//[14:12]
    #define C_LD_LP_VERT Fld(6,6,AC_MSKW10)//[11:6]
    #define C_LD_LP_HOR Fld(6,0,AC_MSKB0)//[5:0]
#define LD2_MODE_08 (LDM_OFFSET + 0x820)
    #define LD_RGN_ST Fld(3,29,AC_MSKB3)//[31:29]
    #define C_LD_SPI_HW_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define C_LD_SPI_ADDR_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define C_LD_SPI_RISING_SEL Fld(2,22,AC_MSKB2)//[23:22]
    #define C_LD_SPI_LIMIT_LNUM Fld(7,15,AC_MSKW21)//[21:15]
    #define C_LD_RGB_WEIGHT Fld(4,10,AC_MSKB1)//[13:10]
    #define C_LD_LPF_WEIGHT Fld(5,5,AC_MSKW10)//[9:5]
    #define C_LD_LP_OFST Fld(5,0,AC_MSKB0)//[4:0]
#define LD2_MODE_09 (LDM_OFFSET + 0x824)
    #define C_LD_FORCE_DIM_TYPE Fld(1,30,AC_MSKB3)//[30:30]
    #define C_LD_FORCE_DIM_VALUE Fld(10,20,AC_MSKW32)//[29:20]
    #define C_LD_FORCE_DIM_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define C_LD_PHASE_VERT_RATIO Fld(3,16,AC_MSKB2)//[18:16]
    #define C_LD_PHASE_HOR_RATIO Fld(3,12,AC_MSKB1)//[14:12]
    #define C_LD_PHASE_VERT Fld(5,5,AC_MSKW10)//[9:5]
    #define C_LD_PHASE_HOR Fld(5,0,AC_MSKB0)//[4:0]
#define LD2_MODE_10 (LDM_OFFSET + 0x828)
    #define C_LD_PHASE_BLK_HOR_7 Fld(4,28,AC_MSKB3)//[31:28]
    #define C_LD_PHASE_BLK_HOR_6 Fld(4,24,AC_MSKB3)//[27:24]
    #define C_LD_PHASE_BLK_HOR_5 Fld(4,20,AC_MSKB2)//[23:20]
    #define C_LD_PHASE_BLK_HOR_4 Fld(4,16,AC_MSKB2)//[19:16]
    #define C_LD_PHASE_BLK_HOR_3 Fld(4,12,AC_MSKB1)//[15:12]
    #define C_LD_PHASE_BLK_HOR_2 Fld(4,8,AC_MSKB1)//[11:8]
    #define C_LD_PHASE_BLK_HOR_1 Fld(4,4,AC_MSKB0)//[7:4]
    #define C_LD_PHASE_BLK_HOR_0 Fld(4,0,AC_MSKB0)//[3:0]
#define LD2_MODE_11 (LDM_OFFSET + 0x82C)
    #define C_LD_PHASE_BLK_HOR_15 Fld(4,28,AC_MSKB3)//[31:28]
    #define C_LD_PHASE_BLK_HOR_14 Fld(4,24,AC_MSKB3)//[27:24]
    #define C_LD_PHASE_BLK_HOR_13 Fld(4,20,AC_MSKB2)//[23:20]
    #define C_LD_PHASE_BLK_HOR_12 Fld(4,16,AC_MSKB2)//[19:16]
    #define C_LD_PHASE_BLK_HOR_11 Fld(4,12,AC_MSKB1)//[15:12]
    #define C_LD_PHASE_BLK_HOR_10 Fld(4,8,AC_MSKB1)//[11:8]
    #define C_LD_PHASE_BLK_HOR_9 Fld(4,4,AC_MSKB0)//[7:4]
    #define C_LD_PHASE_BLK_HOR_8 Fld(4,0,AC_MSKB0)//[3:0]
#define LD2_MODE_12 (LDM_OFFSET + 0x830)
    #define C_LD_PHASE_BLK_HOR_20 Fld(4,16,AC_MSKB2)//[19:16]
    #define C_LD_PHASE_BLK_HOR_19 Fld(4,12,AC_MSKB1)//[15:12]
    #define C_LD_PHASE_BLK_HOR_18 Fld(4,8,AC_MSKB1)//[11:8]
    #define C_LD_PHASE_BLK_HOR_17 Fld(4,4,AC_MSKB0)//[7:4]
    #define C_LD_PHASE_BLK_HOR_16 Fld(4,0,AC_MSKB0)//[3:0]
#define LD2_MODE_13 (LDM_OFFSET + 0x834)
    #define C_LD_PHASE_BLK_VERT_7 Fld(4,28,AC_MSKB3)//[31:28]
    #define C_LD_PHASE_BLK_VERT_6 Fld(4,24,AC_MSKB3)//[27:24]
    #define C_LD_PHASE_BLK_VERT_5 Fld(4,20,AC_MSKB2)//[23:20]
    #define C_LD_PHASE_BLK_VERT_4 Fld(4,16,AC_MSKB2)//[19:16]
    #define C_LD_PHASE_BLK_VERT_3 Fld(4,12,AC_MSKB1)//[15:12]
    #define C_LD_PHASE_BLK_VERT_2 Fld(4,8,AC_MSKB1)//[11:8]
    #define C_LD_PHASE_BLK_VERT_1 Fld(4,4,AC_MSKB0)//[7:4]
    #define C_LD_PHASE_BLK_VERT_0 Fld(4,0,AC_MSKB0)//[3:0]
#define LD2_MODE_14 (LDM_OFFSET + 0x838)
    #define C_LD_PHASE_BLK_VERT_15 Fld(4,28,AC_MSKB3)//[31:28]
    #define C_LD_PHASE_BLK_VERT_14 Fld(4,24,AC_MSKB3)//[27:24]
    #define C_LD_PHASE_BLK_VERT_13 Fld(4,20,AC_MSKB2)//[23:20]
    #define C_LD_PHASE_BLK_VERT_12 Fld(4,16,AC_MSKB2)//[19:16]
    #define C_LD_PHASE_BLK_VERT_11 Fld(4,12,AC_MSKB1)//[15:12]
    #define C_LD_PHASE_BLK_VERT_10 Fld(4,8,AC_MSKB1)//[11:8]
    #define C_LD_PHASE_BLK_VERT_9 Fld(4,4,AC_MSKB0)//[7:4]
    #define C_LD_PHASE_BLK_VERT_8 Fld(4,0,AC_MSKB0)//[3:0]
#define LD2_MODE_15 (LDM_OFFSET + 0x83C)
    #define C_LD_PHASE_BLK_VERT_20 Fld(4,16,AC_MSKB2)//[19:16]
    #define C_LD_PHASE_BLK_VERT_19 Fld(4,12,AC_MSKB1)//[15:12]
    #define C_LD_PHASE_BLK_VERT_18 Fld(4,8,AC_MSKB1)//[11:8]
    #define C_LD_PHASE_BLK_VERT_17 Fld(4,4,AC_MSKB0)//[7:4]
    #define C_LD_PHASE_BLK_VERT_16 Fld(4,0,AC_MSKB0)//[3:0]
#define LD2_GAIN_LIMIT_00 (LDM_OFFSET + 0x840)
    #define C_LD_GAIN_LIMIT_0 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_1 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_01 (LDM_OFFSET + 0x844)
    #define C_LD_GAIN_LIMIT_2 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_3 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_02 (LDM_OFFSET + 0x848)
    #define C_LD_GAIN_LIMIT_4 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_5 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_03 (LDM_OFFSET + 0x84C)
    #define C_LD_GAIN_LIMIT_6 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_7 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_04 (LDM_OFFSET + 0x850)
    #define C_LD_GAIN_LIMIT_8 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_9 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_05 (LDM_OFFSET + 0x854)
    #define C_LD_GAIN_LIMIT_10 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_11 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_06 (LDM_OFFSET + 0x858)
    #define C_LD_GAIN_LIMIT_12 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_13 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_07 (LDM_OFFSET + 0x85C)
    #define C_LD_GAIN_LIMIT_14 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_15 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_08 (LDM_OFFSET + 0x860)
    #define C_LD_GAIN_LIMIT_16 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_17 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_09 (LDM_OFFSET + 0x864)
    #define C_LD_GAIN_LIMIT_18 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_19 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_10 (LDM_OFFSET + 0x868)
    #define C_LD_GAIN_LIMIT_20 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_21 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_11 (LDM_OFFSET + 0x86C)
    #define C_LD_GAIN_LIMIT_22 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_23 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_12 (LDM_OFFSET + 0x870)
    #define C_LD_GAIN_LIMIT_24 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_25 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_13 (LDM_OFFSET + 0x874)
    #define C_LD_GAIN_LIMIT_26 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_27 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_14 (LDM_OFFSET + 0x878)
    #define C_LD_GAIN_LIMIT_28 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_29 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_15 (LDM_OFFSET + 0x87C)
    #define C_LD_GAIN_LIMIT_30 Fld(14,16,AC_MSKW32)//[29:16]
    #define C_LD_GAIN_LIMIT_31 Fld(14,0,AC_MSKW10)//[13:0]
#define LD2_GAIN_LIMIT_16 (LDM_OFFSET + 0x880)
    #define C_LD_GAIN_LIMIT_32 Fld(14,16,AC_MSKW32)//[29:16]
#define LD2_SRAM_IF_00 (LDM_OFFSET + 0x884)
    #define C_LD_FORCE_SRAM_CS_ON Fld(1,17,AC_MSKB2)//[17:17]
    #define C_LD_VA_READ_LATEST_WDATA Fld(1,16,AC_MSKB2)//[16:16]
    #define C_LD_RD_AUTO_INC_ADDR Fld(1,11,AC_MSKB1)//[11:11]
    #define C_LD_WR_AUTO_INC_ADDR Fld(1,10,AC_MSKB1)//[10:10]
    #define C_LD_PIO_MODE Fld(1,9,AC_MSKB1)//[9:9]
    #define C_LD_FORCE_WR_NO_WAIT Fld(1,8,AC_MSKB1)//[8:8]
    #define C_LD_MAX_WAIT_CYCLE Fld(8,0,AC_FULLB0)//[7:0]
#define LD2_SRAM_IF_01 (LDM_OFFSET + 0x888)
    #define LD_CPU_SRAM_ADDR Fld(8,16,AC_FULLB2)//[23:16]
    #define C_LD_ADDR Fld(8,0,AC_FULLB0)//[7:0]
#define LD2_SRAM_IF_02 (LDM_OFFSET + 0x88C)
    #define C_LD_WDATA_0 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_SRAM_IF_03 (LDM_OFFSET + 0x890)
    #define C_LD_WDATA_1 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_SRAM_IF_04 (LDM_OFFSET + 0x894)
    #define C_LD_WDATA_2 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_SRAM_IF_05 (LDM_OFFSET + 0x898)
    #define C_LD_WDATA_3 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_SRAM_IF_06 (LDM_OFFSET + 0x89C)
    #define C_LD_WDATA_4 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_SRAM_IF_07 (LDM_OFFSET + 0x8A0)
    #define C_LD_WDATA_5 Fld(8,0,AC_FULLB0)//[7:0]
#define LD2_SRAM_IF_08 (LDM_OFFSET + 0x8A4)
    #define LD_RDATA_0 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_SRAM_IF_09 (LDM_OFFSET + 0x8A8)
    #define LD_RDATA_1 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_SRAM_IF_10 (LDM_OFFSET + 0x8AC)
    #define LD_RDATA_2 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_SRAM_IF_11 (LDM_OFFSET + 0x8B0)
    #define LD_RDATA_3 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_SRAM_IF_12 (LDM_OFFSET + 0x8B4)
    #define LD_RDATA_4 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_SRAM_IF_13 (LDM_OFFSET + 0x8B8)
    #define LD_RDATA_5 Fld(8,0,AC_FULLB0)//[7:0]
#define LD2_SRAM_IF_14 (LDM_OFFSET + 0x8BC)
    #define C_LD_CPURW_SEL_0 Fld(1,9,AC_MSKB1)//[9:9]
    #define C_LD_CPURW_ACTIVE Fld(1,8,AC_MSKB1)//[8:8]
    #define LD_CPU_SEL_SRAM_1 Fld(1,7,AC_MSKB0)//[7:7]
    #define LD_CPU_SEL_SRAM_0 Fld(1,6,AC_MSKB0)//[6:6]
    #define LD_TABLE_WAIT_VSYNC Fld(1,5,AC_MSKB0)//[5:5]
    #define C_LD_FILL_TABLE_READY Fld(1,4,AC_MSKB0)//[4:4]
    #define LD_READ_FINISH Fld(1,3,AC_MSKB0)//[3:3]
    #define C_LD_READ Fld(1,2,AC_MSKB0)//[2:2]
    #define LD_WRITE_FINISH Fld(1,1,AC_MSKB0)//[1:1]
    #define C_LD_WRITE Fld(1,0,AC_MSKB0)//[0:0]
#define LD2_DBUG (LDM_OFFSET + 0x8C0)
    #define LD_ERR_R_INIT_LP Fld(1,31,AC_MSKB3)//[31:31]
    #define C_LD_CLEAR_ERR Fld(1,30,AC_MSKB3)//[30:30]
    #define C_LD_DBG_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define LD2_STATUS_00 (LDM_OFFSET + 0x8C8)
    #define LD_3D_PERIOD Fld(2,30,AC_MSKB3)//[31:30]
    #define LD_MAX_INFO Fld(10,8,AC_MSKW21)//[17:8]
    #define C_LD_READ_BLK_NUM Fld(8,0,AC_FULLB0)//[7:0]
#define LD2_STATUS_01 (LDM_OFFSET + 0x8CC)
    #define LD_OUT_DIM Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_TAR_DIM Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_CONFIDENCE Fld(5,0,AC_MSKB0)//[4:0]
#define LD2_INK_DATA (LDM_OFFSET + 0x8E4)
    #define C_LD_INK3_SHIFT Fld(2,30,AC_MSKB3)//[31:30]
    #define C_LD_INK_B Fld(10,20,AC_MSKW32)//[29:20]
    #define C_LD_INK_G Fld(10,10,AC_MSKW21)//[19:10]
    #define C_LD_INK_R Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DEMO_00 (LDM_OFFSET + 0x8E8)
    #define C_LD_DEMO_YSTART Fld(16,16,AC_FULLW32)//[31:16]
    #define C_LD_DEMO_XSTART Fld(16,0,AC_FULLW10)//[15:0]
#define LD2_DEMO_01 (LDM_OFFSET + 0x8EC)
    #define C_LD_DEMO_YEND Fld(16,16,AC_FULLW32)//[31:16]
    #define C_LD_DEMO_XEND Fld(16,0,AC_FULLW10)//[15:0]
#define LD2_DEMO_02 (LDM_OFFSET + 0x8F0)
    #define C_LD_DEMO_REGION_SWAP Fld(1,2,AC_MSKB0)//[2:2]
    #define C_LD_DEMO_NOHIT_MODE Fld(2,0,AC_MSKB0)//[1:0]
#define LD2_DIM_RESULT_00 (LDM_OFFSET + 0x8F4)
    #define LD_DIM_READY Fld(1,30,AC_MSKB3)//[30:30]
    #define LD_DIM_002 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_001 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_000 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_01 (LDM_OFFSET + 0x8F8)
    #define LD_DIM_005 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_004 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_003 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_02 (LDM_OFFSET + 0x8FC)
    #define LD_DIM_008 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_007 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_006 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_03 (LDM_OFFSET + 0x900)
    #define LD_DIM_011 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_010 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_009 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_04 (LDM_OFFSET + 0x904)
    #define LD_DIM_014 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_013 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_012 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_05 (LDM_OFFSET + 0x908)
    #define LD_DIM_017 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_016 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_015 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_06 (LDM_OFFSET + 0x90C)
    #define LD_DIM_020 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_019 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_018 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_07 (LDM_OFFSET + 0x910)
    #define LD_DIM_023 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_022 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_021 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_08 (LDM_OFFSET + 0x914)
    #define LD_DIM_026 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_025 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_024 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_09 (LDM_OFFSET + 0x918)
    #define LD_DIM_029 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_028 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_027 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_10 (LDM_OFFSET + 0x91C)
    #define LD_DIM_032 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_031 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_030 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_11 (LDM_OFFSET + 0x920)
    #define LD_DIM_035 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_034 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_033 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_12 (LDM_OFFSET + 0x924)
    #define LD_DIM_038 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_037 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_036 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_13 (LDM_OFFSET + 0x928)
    #define LD_DIM_041 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_040 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_039 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_14 (LDM_OFFSET + 0x92C)
    #define LD_DIM_044 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_043 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_042 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_15 (LDM_OFFSET + 0x930)
    #define LD_DIM_047 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_046 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_045 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_16 (LDM_OFFSET + 0x934)
    #define LD_DIM_050 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_049 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_048 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_17 (LDM_OFFSET + 0x938)
    #define LD_DIM_053 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_052 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_051 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_18 (LDM_OFFSET + 0x93C)
    #define LD_DIM_056 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_055 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_054 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_19 (LDM_OFFSET + 0x940)
    #define LD_DIM_059 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_058 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_057 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_20 (LDM_OFFSET + 0x944)
    #define LD_DIM_062 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_061 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_060 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_21 (LDM_OFFSET + 0x948)
    #define LD_DIM_065 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_064 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_063 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_22 (LDM_OFFSET + 0x94C)
    #define LD_DIM_068 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_067 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_066 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_23 (LDM_OFFSET + 0x950)
    #define LD_DIM_071 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_070 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_069 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_24 (LDM_OFFSET + 0x954)
    #define LD_DIM_074 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_073 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_072 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_25 (LDM_OFFSET + 0x958)
    #define LD_DIM_077 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_076 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_075 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_26 (LDM_OFFSET + 0x95C)
    #define LD_DIM_080 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_079 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_078 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_27 (LDM_OFFSET + 0x960)
    #define LD_DIM_083 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_082 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_081 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_28 (LDM_OFFSET + 0x964)
    #define LD_DIM_086 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_085 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_084 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_29 (LDM_OFFSET + 0x968)
    #define LD_DIM_089 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_088 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_087 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_30 (LDM_OFFSET + 0x96C)
    #define LD_DIM_092 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_091 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_090 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_31 (LDM_OFFSET + 0x970)
    #define LD_DIM_095 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_094 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_093 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_32 (LDM_OFFSET + 0x974)
    #define LD_DIM_098 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_097 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_096 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_33 (LDM_OFFSET + 0x978)
    #define LD_DIM_101 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_100 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_099 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_34 (LDM_OFFSET + 0x97C)
    #define LD_DIM_104 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_103 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_102 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_35 (LDM_OFFSET + 0x980)
    #define LD_DIM_107 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_106 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_105 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_36 (LDM_OFFSET + 0x984)
    #define LD_DIM_110 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_109 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_108 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_37 (LDM_OFFSET + 0x988)
    #define LD_DIM_113 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_112 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_111 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_38 (LDM_OFFSET + 0x98C)
    #define LD_DIM_116 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_115 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_114 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_39 (LDM_OFFSET + 0x990)
    #define LD_DIM_119 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_118 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_117 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_40 (LDM_OFFSET + 0x994)
    #define LD_DIM_122 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_121 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_120 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_41 (LDM_OFFSET + 0x998)
    #define LD_DIM_125 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_124 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_123 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_42 (LDM_OFFSET + 0x99C)
    #define LD_DIM_128 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_127 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_126 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_43 (LDM_OFFSET + 0x9A0)
    #define LD_DIM_131 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_130 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_129 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_44 (LDM_OFFSET + 0x9A4)
    #define LD_DIM_134 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_133 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_132 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_45 (LDM_OFFSET + 0x9A8)
    #define LD_DIM_137 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_136 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_135 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_46 (LDM_OFFSET + 0x9AC)
    #define LD_DIM_140 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_139 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_138 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_47 (LDM_OFFSET + 0x9B0)
    #define LD_DIM_143 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_142 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_141 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_48 (LDM_OFFSET + 0x9B4)
    #define LD_DIM_146 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_145 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_144 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_49 (LDM_OFFSET + 0x9B8)
    #define LD_DIM_149 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_148 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_147 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_50 (LDM_OFFSET + 0x9BC)
    #define LD_DIM_152 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_151 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_150 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_51 (LDM_OFFSET + 0x9C0)
    #define LD_DIM_155 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_154 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_153 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_52 (LDM_OFFSET + 0x9C4)
    #define LD_DIM_158 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_157 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_156 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_53 (LDM_OFFSET + 0x9C8)
    #define LD_DIM_161 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_160 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_159 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_54 (LDM_OFFSET + 0x9CC)
    #define LD_DIM_164 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_163 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_162 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_55 (LDM_OFFSET + 0x9D0)
    #define LD_DIM_167 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_166 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_165 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_56 (LDM_OFFSET + 0x9D4)
    #define LD_DIM_170 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_169 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_168 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_57 (LDM_OFFSET + 0x9D8)
    #define LD_DIM_173 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_172 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_171 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_58 (LDM_OFFSET + 0x9DC)
    #define LD_DIM_176 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_175 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_174 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_59 (LDM_OFFSET + 0x9E0)
    #define LD_DIM_179 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_178 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_177 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_60 (LDM_OFFSET + 0x9E4)
    #define LD_DIM_182 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_181 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_180 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_61 (LDM_OFFSET + 0x9E8)
    #define LD_DIM_185 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_184 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_183 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_62 (LDM_OFFSET + 0x9EC)
    #define LD_DIM_188 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_187 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_186 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_63 (LDM_OFFSET + 0x9F0)
    #define LD_DIM_191 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_190 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_189 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_64 (LDM_OFFSET + 0x9F4)
    #define LD_DIM_194 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_193 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_192 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_65 (LDM_OFFSET + 0x9F8)
    #define LD_DIM_197 Fld(10,20,AC_MSKW32)//[29:20]
    #define LD_DIM_196 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_195 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_DIM_RESULT_66 (LDM_OFFSET + 0x9FC)
    #define LD_DIM_199 Fld(10,10,AC_MSKW21)//[19:10]
    #define LD_DIM_198 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_MODE_16 (LDM_OFFSET + 0xA00)
    #define C_LD_MAXINFO_LPF_MODE Fld(2,30,AC_MSKB3)//[31:30]
    #define C_LD_BLKINFO_WEIGHT Fld(6,24,AC_MSKB3)//[29:24]
    #define C_LD_BLKAVG_LSUM_SHIFT Fld(4,20,AC_MSKB2)//[23:20]
    #define C_LD_BLKAVE_SHIFT_C Fld(4,16,AC_MSKB2)//[19:16]
    #define C_LD_BLKAVE_MULT_B Fld(10,5,AC_MSKW10)//[14:5]
    #define C_LD_BLKAVE_SHIFT_A Fld(4,0,AC_MSKB0)//[3:0]
#define LD2_MODE_17 (LDM_OFFSET + 0xA04)
    #define C_LD_MAX_UP_DIM_STEP Fld(8,24,AC_FULLB3)//[31:24]
    #define C_LD_MAX_DN_DIM_STEP Fld(8,16,AC_FULLB2)//[23:16]
    #define C_LD_BLK_SCENE_CHG_0_15 Fld(16,0,AC_FULLW10)//[15:0]
#define LD2_MODE_18 (LDM_OFFSET + 0xA08)
    #define C_LD_BLK_SCENE_CHG_16_47 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_MODE_19 (LDM_OFFSET + 0xA0C)
    #define C_LD_BLK_SCENE_CHG_48_79 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_MODE_20 (LDM_OFFSET + 0xA10)
    #define C_LD_BLK_SCENE_CHG_80_111 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_MODE_21 (LDM_OFFSET + 0xA14)
    #define C_LD_BLK_SCENE_CHG_112_143 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_MODE_22 (LDM_OFFSET + 0xA18)
    #define C_LD_BLK_SCENE_CHG_144_175 Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_MODE_23 (LDM_OFFSET + 0xA1C)
    #define C_LD_BLK_SCENE_CHG_MODE Fld(2,30,AC_MSKB3)//[31:30]
    #define C_LD_BLK_SCENE_CHG_DN_TH Fld(3,27,AC_MSKB3)//[29:27]
    #define C_LD_BLK_SCENE_CHG_UP_TH Fld(3,24,AC_MSKB3)//[26:24]
    #define C_LD_BLK_SCENE_CHG_176_199 Fld(24,0,AC_MSKDW)//[23:0]
#define LD2_MODE_24 (LDM_OFFSET + 0xA20)
    #define C_LD_RGB_WEIGHT2 Fld(4,24,AC_MSKB3)//[27:24]
    #define C_LD_BLK_SCENE_CHG_HW_THD2 Fld(8,16,AC_FULLB2)//[23:16]
    #define C_LD_BLK_SCENE_CHG_HW_THD1 Fld(8,8,AC_FULLB1)//[15:8]
    #define C_LD_BLK_SCENE_CHG_HW_THD0 Fld(8,0,AC_FULLB0)//[7:0]
#define LD2_MODE_25 (LDM_OFFSET + 0xA24)
    #define LD_BLKINFO_004 Fld(6,24,AC_MSKB3)//[29:24]
    #define LD_BLKINFO_003 Fld(6,18,AC_MSKB2)//[23:18]
    #define LD_BLKINFO_002 Fld(6,12,AC_MSKW21)//[17:12]
    #define LD_BLKINFO_001 Fld(6,6,AC_MSKW10)//[11:6]
    #define LD_BLKINFO_000 Fld(6,0,AC_MSKB0)//[5:0]
#define LD2_MODE_26 (LDM_OFFSET + 0xA28)
    #define LD_BLKINFO_009 Fld(6,24,AC_MSKB3)//[29:24]
    #define LD_BLKINFO_008 Fld(6,18,AC_MSKB2)//[23:18]
    #define LD_BLKINFO_007 Fld(6,12,AC_MSKW21)//[17:12]
    #define LD_BLKINFO_006 Fld(6,6,AC_MSKW10)//[11:6]
    #define LD_BLKINFO_005 Fld(6,0,AC_MSKB0)//[5:0]
#define LD2_MODE_27 (LDM_OFFSET + 0xA2C)
    #define LD_BLKINFO_014 Fld(6,24,AC_MSKB3)//[29:24]
    #define LD_BLKINFO_013 Fld(6,18,AC_MSKB2)//[23:18]
    #define LD_BLKINFO_012 Fld(6,12,AC_MSKW21)//[17:12]
    #define LD_BLKINFO_011 Fld(6,6,AC_MSKW10)//[11:6]
    #define LD_BLKINFO_010 Fld(6,0,AC_MSKB0)//[5:0]
#define LD2_MODE_28 (LDM_OFFSET + 0xA30)
    #define LD_BLKINFO_019 Fld(6,24,AC_MSKB3)//[29:24]
    #define LD_BLKINFO_018 Fld(6,18,AC_MSKB2)//[23:18]
    #define LD_BLKINFO_017 Fld(6,12,AC_MSKW21)//[17:12]
    #define LD_BLKINFO_016 Fld(6,6,AC_MSKW10)//[11:6]
    #define LD_BLKINFO_015 Fld(6,0,AC_MSKB0)//[5:0]
#define LD2_MODE_29 (LDM_OFFSET + 0xA34)
    #define LD_BLKINFO_024 Fld(6,24,AC_MSKB3)//[29:24]
    #define LD_BLKINFO_023 Fld(6,18,AC_MSKB2)//[23:18]
    #define LD_BLKINFO_022 Fld(6,12,AC_MSKW21)//[17:12]
    #define LD_BLKINFO_021 Fld(6,6,AC_MSKW10)//[11:6]
    #define LD_BLKINFO_020 Fld(6,0,AC_MSKB0)//[5:0]
#define LD2_MODE_30 (LDM_OFFSET + 0xA38)
    #define LD_BLKINFO_029 Fld(6,24,AC_MSKB3)//[29:24]
    #define LD_BLKINFO_028 Fld(6,18,AC_MSKB2)//[23:18]
    #define LD_BLKINFO_027 Fld(6,12,AC_MSKW21)//[17:12]
    #define LD_BLKINFO_026 Fld(6,6,AC_MSKW10)//[11:6]
    #define LD_BLKINFO_025 Fld(6,0,AC_MSKB0)//[5:0]
#define LD2_MODE_31 (LDM_OFFSET + 0xA3C)
    #define C_LD_BLKINFO_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define C_LD_BLKINFO_START_NUM Fld(8,16,AC_FULLB2)//[23:16]
    #define LD_BLKINFO_031 Fld(6,6,AC_MSKW10)//[11:6]
    #define LD_BLKINFO_030 Fld(6,0,AC_MSKB0)//[5:0]
#define LD2_MODE_32 (LDM_OFFSET + 0xA40)
    #define LD_BLKINFO_READY Fld(32,0,AC_FULLDW)//[31:0]
#define LD2_MODE_33 (LDM_OFFSET + 0xA44)
    #define C_LD_FIX_DIM_STEP_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define C_LD_FW_DIM_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define C_LD_FW_DIM_000 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_LD_FW_DIM_001 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_LD_FW_DIM_002 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_MODE_34 (LDM_OFFSET + 0xA48)
    #define C_LD_FW_DIM_003 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_LD_FW_DIM_004 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_LD_FW_DIM_005 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_MODE_35 (LDM_OFFSET + 0xA4C)
    #define C_LD_FW_DIM_006 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_LD_FW_DIM_007 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_LD_FW_DIM_008 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_MODE_36 (LDM_OFFSET + 0xA50)
    #define C_LD_FW_DIM_009 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_LD_FW_DIM_010 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_LD_FW_DIM_011 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_MODE_37 (LDM_OFFSET + 0xA54)
    #define C_LD_FW_DIM_012 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_LD_FW_DIM_013 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_LD_FW_DIM_014 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_MODE_38 (LDM_OFFSET + 0xA58)
    #define C_LD_FW_DIM_015 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_LD_FW_DIM_016 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_LD_FW_DIM_017 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_MODE_39 (LDM_OFFSET + 0xA5C)
    #define C_LD_FW_DIM_018 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_LD_FW_DIM_019 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_LD_FW_DIM_020 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_MODE_40 (LDM_OFFSET + 0xA60)
    #define C_LD_FW_DIM_021 Fld(10,20,AC_MSKW32)//[29:20]
    #define C_LD_FW_DIM_022 Fld(10,10,AC_MSKW21)//[19:10]
    #define C_LD_FW_DIM_023 Fld(10,0,AC_MSKW10)//[9:0]
#define LD2_MODE_41 (LDM_OFFSET + 0xA64)
    #define C_LD_FW_CURDIM_SUM Fld(18,0,AC_MSKDW)//[17:0]
#define LD2_MODE_42 (LDM_OFFSET + 0xA68)
    #define C_LD_CURDIM_SUM_STA Fld(18,0,AC_MSKDW)//[17:0]

#endif
