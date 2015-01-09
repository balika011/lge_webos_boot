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
#ifndef _HW_EPI_H_
#define _HW_EPI_H_


#define EPI_B0 (IO_VIRT + 0x35400)
    #define RG_PN_SEL Fld(6, 24, AC_MSKB3) //29:24
    #define RG_INV_SEL Fld(6,16,AC_MSKB2)//[21:16]
    #define RG_EPI_MON_SEL Fld(8,8,AC_FULLB1)//[15:8]
    #define RG_BIT_SEL Fld(2,0,AC_MSKB0)//[1:0]
#define EPI_B1 (IO_VIRT + 0x35404)
    #define RG_SWAP_OUT5_SEL Fld(3,20,AC_MSKB2)//[22:20]
    #define RG_SWAP_OUT4_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define RG_SWAP_OUT3_SEL Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_SWAP_OUT2_SEL Fld(3,8,AC_MSKB1)//[10:8]
    #define RG_SWAP_OUT1_SEL Fld(3,4,AC_MSKB0)//[6:4]
    #define RG_SWAP_OUT0_SEL Fld(3,0,AC_MSKB0)//[2:0]
#define EPI_B7 (IO_VIRT + 0x3541C)
    #define RG_CTRL2_SELF_5 Fld(1,13,AC_MSKB1)//[13:13]
    #define RG_CTRL2_SELF_4 Fld(1,12,AC_MSKB1)//[12:12]
    #define RG_CTRL2_SELF_3 Fld(1,11,AC_MSKB1)//[11:11]
    #define RG_CTRL2_SELF_2 Fld(1,10,AC_MSKB1)//[10:10]
    #define RG_CTRL2_SELF_1 Fld(1,9,AC_MSKB1)//[9:9]
    #define RG_CTRL2_SELF_0 Fld(1,8,AC_MSKB1)//[8:8]
    #define RG_CTRL1_SELF_5 Fld(1,5,AC_MSKB0)//[5:5]
    #define RG_CTRL1_SELF_4 Fld(1,4,AC_MSKB0)//[4:4]
    #define RG_CTRL1_SELF_3 Fld(1,3,AC_MSKB0)//[3:3]
    #define RG_CTRL1_SELF_2 Fld(1,2,AC_MSKB0)//[2:2]
    #define RG_CTRL1_SELF_1 Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_CTRL1_SELF_0 Fld(1,0,AC_MSKB0)//[0:0]
#define EPI_T0 (IO_VIRT + 0x35500)
    #define RG_CTR_2_RGC_EN Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_ABNOR_UNLOCK_GATE Fld(1,14,AC_MSKB1)//[14:14]
    #define RG_TCTG_UNLOCK_GATE Fld(1,13,AC_MSKB1)//[13:13]
    #define RG_DE_UNLOCK_GATE Fld(1,12,AC_MSKB1)//[12:12]
    #define RG_HS_UNLOCK_GATE Fld(1,11,AC_MSKB1)//[11:11]
    #define RG_VS_UNLOCK_GATE Fld(1,10,AC_MSKB1)//[10:10]
    #define RG_PRE_AMBLE_NUM Fld(3,4,AC_MSKB0)//[6:4]
    #define RG_DE_IN_POL Fld(1,2,AC_MSKB0)//[2:2]
    #define RG_HS_IN_POL Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_VS_HS_POL Fld(1,0,AC_MSKB0)//[0:0]
#define EPI_T1 (IO_VIRT + 0x35504)
    #define RG_CTR_START_DUMMY Fld(24,6,AC_MSKDW)//[29:6]
    #define RG_CTR_START_IND Fld(6,0,AC_MSKB0)//[5:0]
#define EPI_T2 (IO_VIRT + 0x35508)
    #define RG_CTR_1_DUMMY Fld(12,18,AC_MSKW32)//[29:18]
    #define RG_CTR_1_SOE_WIDTH Fld(10,8,AC_MSKW21)//[17:8]
    #define RG_CTR_1_SOE_START Fld(8,0,AC_FULLB0)//[7:0]
#define EPI_T3 (IO_VIRT + 0x3550C)
    #define RG_CTR_2_REV2 Fld(16,14,AC_MSKDW)//[29:14]
    #define RG_CTR_2_POLC Fld(1,13,AC_MSKB1)//[13:13]
    #define RG_CTR_2_GMAEN2 Fld(1,12,AC_MSKB1)//[12:12]
    #define RG_CTR_2_GMAEN1 Fld(1,11,AC_MSKB1)//[11:11]
    #define RG_CTR_2_CSC Fld(1,10,AC_MSKB1)//[10:10]
    #define RG_CTR_2_GSP Fld(1,9,AC_MSKB1)//[9:9]
    #define RG_CTR_2_REV1 Fld(2,7,AC_MSKW10)//[8:7]
    #define RG_CTR_2_PWRC2 Fld(1,6,AC_MSKB0)//[6:6]
    #define RG_CTR_2_PWRC1 Fld(1,5,AC_MSKB0)//[5:5]
    #define RG_CTR_2_LTD2 Fld(1,4,AC_MSKB0)//[4:4]
    #define RG_CTR_2_LTD1 Fld(1,3,AC_MSKB0)//[3:3]
    #define RG_CTR_2_H2DOT Fld(1,2,AC_MSKB0)//[2:2]
    #define RG_CTR_2_MODE Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_CTR_2_POL Fld(1,0,AC_MSKB0)//[0:0]
#define EPI_T4 (IO_VIRT + 0x35510)
    #define RG_DATA_START_DUMMY Fld(24,6,AC_MSKDW)//[29:6]
    #define RG_DATA_START_IND Fld(6,0,AC_MSKB0)//[5:0]
#define EPI_T5 (IO_VIRT + 0x35514)
    #define RG_ENC_CS_CTRL Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_FIFO_EN Fld(1,0,AC_MSKB0)//[0:0]
#define EPI_T11 (IO_VIRT + 0x3552C)
    #define EPI_RG_ABNOR_V_CNT Fld(11,16,AC_MSKW32)//[26:16]
    #define EPI_CLOCK_DATA_T11 Fld(2,6,AC_MSKB0)//[7:6]
    #define EPI_CLOCK_EN Fld(6,0,AC_MSKB0)//[5:0]
#define EPI_T12 (IO_VIRT + 0x35530)
    #define RG_TCO_7_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_6_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_5_SEL Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_4_SEL Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_3_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_2_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_1_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_0_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T13 (IO_VIRT + 0x35534)
    #define RG_TCO_15_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_14_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_13_SEL Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_12_SEL Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_11_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_10_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_9_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_8_SEL Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T14 (IO_VIRT + 0x35538)
    #define RG_CTR_2_RGC_POL_0 Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_CTR_2_RGC_EN_0 Fld(16,0,AC_FULLW10)//[15:0]
#define EPI_T15 (IO_VIRT + 0x3553C)
    #define RG_CTR_2_RGC_POL_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_CTR_2_RGC_EN_1 Fld(16,0,AC_FULLW10)//[15:0]
#define EPI_T16 (IO_VIRT + 0x35540)
    #define RG_CTR_2_RGC_POL_2 Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_CTR_2_RGC_EN_2 Fld(16,0,AC_FULLW10)//[15:0]
#define EPI_T17 (IO_VIRT + 0x35544)
    #define RG_CTR_2_RGC_POL_3150 Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_CTR_2_RGC_EN_3 Fld(16,0,AC_FULLW10)//[15:0]
#define EPI_T18 (IO_VIRT + 0x35548)
    #define RG_CTR_2_RGC_POL_4 Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_CTR_2_RGC_EN_4 Fld(16,0,AC_FULLW10)//[15:0]
#define EPI_T19 (IO_VIRT + 0x3554C)
    #define RG_CTR_2_RGC_POL_5 Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_CTR_2_RGC_EN_5150 Fld(16,0,AC_FULLW10)//[15:0]
#define EPI_T20 (IO_VIRT + 0x35550)
    #define RG_CTRL1_0 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T21 (IO_VIRT + 0x35554)
    #define RG_CTRL1_1 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T22 (IO_VIRT + 0x35558)
    #define RG_CTRL1_2 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T23 (IO_VIRT + 0x3555C)
    #define RG_CTRL1_3 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T24 (IO_VIRT + 0x35560)
    #define RG_CTRL1_4 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T25 (IO_VIRT + 0x35564)
    #define RG_CTRL1_5 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T26 (IO_VIRT + 0x35568)
    #define RG_CTRL2_0 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T27 (IO_VIRT + 0x3556C)
    #define RG_CTRL2_1 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T28 (IO_VIRT + 0x35570)
    #define RG_CTRL2_2 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T29 (IO_VIRT + 0x35574)
    #define RG_CTRL2_3 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T30 (IO_VIRT + 0x35578)
    #define RG_CTRL2_4 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T31 (IO_VIRT + 0x3557C)
    #define RG_CTRL2_5 Fld(30,0,AC_MSKDW)//[29:0]
#define EPI_T32 (IO_VIRT + 0x35580)
    #define RG_TCO_7_SEL_0 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_6_SEL_0 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_5_SEL_0 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_4_SEL_0 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_3_SEL_0 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_2_SEL_0 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_1_SEL_0 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_0_SEL_0 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T33 (IO_VIRT + 0x35584)
    #define RG_TCO_15_SEL_0 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_14_SEL_0 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_13_SEL_0 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_12_SEL_0 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_11_SEL_0 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_10_SEL_0 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_9_SEL_0 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_8_SEL_0 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T34 (IO_VIRT + 0x35588)
    #define RG_TCO_7_SEL_1 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_6_SEL_1 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_5_SEL_1 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_4_SEL_1 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_3_SEL_1 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_2_SEL_1 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_1_SEL_1 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_0_SEL_1 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T35 (IO_VIRT + 0x3558C)
    #define RG_TCO_15_SEL_1 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_14_SEL_1 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_13_SEL_1 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_12_SEL_1 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_11_SEL_1 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_10_SEL_1 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_9_SEL_1 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_8_SEL_1 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T36 (IO_VIRT + 0x35590)
    #define RG_TCO_7_SEL_2 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_6_SEL_2 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_5_SEL_2 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_4_SEL_2 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_3_SEL_2 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_2_SEL_2 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_1_SEL_2 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_0_SEL_2 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T37 (IO_VIRT + 0x35594)
    #define RG_TCO_15_SEL_2 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_14_SEL_2 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_13_SEL_2 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_12_SEL_2 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_11_SEL_2 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_10_SEL_2 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_9_SEL_2 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_8_SEL_2 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T38 (IO_VIRT + 0x35598)
    #define RG_TCO_7_SEL_3 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_6_SEL_3 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_5_SEL_3 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_4_SEL_3 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_3_SEL_3 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_2_SEL_3 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_1_SEL_3 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_0_SEL_3 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T39 (IO_VIRT + 0x3559C)
    #define RG_TCO_15_SEL_3 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_14_SEL_3 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_13_SEL_3 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_12_SEL_3 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_11_SEL_3 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_10_SEL_3 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_9_SEL_3 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_8_SEL_3 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T40 (IO_VIRT + 0x355A0)
    #define RG_TCO_7_SEL_4 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_6_SEL_4 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_5_SEL_4 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_4_SEL_4 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_3_SEL_4 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_2_SEL_4 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_1_SEL_4 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_0_SEL_4 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T41 (IO_VIRT + 0x355A4)
    #define RG_TCO_15_SEL_4 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_14_SEL_4 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_13_SEL_4 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_12_SEL_4 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_11_SEL_4 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_10_SEL_4 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_9_SEL_4 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_8_SEL_4 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T42 (IO_VIRT + 0x355A8)
    #define RG_TCO_7_SEL_5 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_6_SEL_5 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_5_SEL_5 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_4_SEL_5 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_3_SEL_5 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_2_SEL_5 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_1_SEL_5 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_0_SEL_5 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T43 (IO_VIRT + 0x355AC)
    #define RG_TCO_15_SEL_5 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_TCO_14_SEL_5 Fld(4,24,AC_MSKB3)//[27:24]
    #define RG_TCO_13_SEL_5 Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_TCO_12_SEL_5 Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_TCO_11_SEL_5 Fld(4,12,AC_MSKB1)//[15:12]
    #define RG_TCO_10_SEL_5 Fld(4,8,AC_MSKB1)//[11:8]
    #define RG_TCO_9_SEL_5 Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_TCO_8_SEL_5 Fld(4,0,AC_MSKB0)//[3:0]
#define EPI_T44 (IO_VIRT + 0x355B0)
    //#define EPI_CLOCK_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define EPI_TRO0 (IO_VIRT + 0x355C0)
    #define ST_REORDER_FIFO_UF Fld(13,16,AC_MSKW32)//[28:16]
    #define ST_REORDER_FIFO_OF Fld(13,0,AC_MSKW10)//[12:0]
#define EPI_TRO1 (IO_VIRT + 0x355C4)
    #define CRC6_FAIL Fld(1,29,AC_MSKB3)//[29:29]
    #define CRC5_FAIL Fld(1,28,AC_MSKB3)//[28:28]
    #define CRC4_FAIL Fld(1,27,AC_MSKB3)//[27:27]
    #define CRC3_FAIL Fld(1,26,AC_MSKB3)//[26:26]
    #define CRC2_FAIL Fld(1,25,AC_MSKB3)//[25:25]
    #define CRC1_FAIL Fld(1,24,AC_MSKB3)//[24:24]
    #define MUTE_STATE Fld(2,16,AC_MSKB2)//[17:16]
    #define EN_CS_STATE Fld(2,8,AC_MSKB1)//[9:8]
    #define LOCK Fld(1,0,AC_MSKB0)//[0:0]
#define EPI_TRO2 (IO_VIRT + 0x355C8)
    #define AUTO_V_ACTIVE Fld(11,16,AC_MSKW32)//[26:16]
    #define AUTO_H_TOTAL Fld(12,0,AC_MSKW10)//[11:0]
#define EPI_TRO3 (IO_VIRT + 0x355CC)
    #define CRC0_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define EPI_TRO4 (IO_VIRT + 0x355D0)
    #define CRC1_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define EPI_TRO5 (IO_VIRT + 0x355D4)
    #define CRC2_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define EPI_TRO6 (IO_VIRT + 0x355D8)
    #define CRC3_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define EPI_TRO7 (IO_VIRT + 0x355DC)
    #define CRC4_VALUE Fld(32,0,AC_FULLDW)//[31:0]
#define EPI_TRO8 (IO_VIRT + 0x355E0)
    #define CRC5_VALUE Fld(32,0,AC_FULLDW)//[31:0]
    
#endif

