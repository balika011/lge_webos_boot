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
 
#define VB1SYS0 (IO_VIRT + 0x35814)
	#define VB1SYS0_3DLR_EN Fld(1,19,AC_MSKB2)//[19:19]
    #define VB1SYS0_BIT_INV Fld(1,18,AC_MSKB2)//[18:18]
    #define VB1SYS0_VSYN_INV Fld(1,17,AC_MSKB2)//[17:17]
    #define VB1SYS0_HSYN_INV Fld(1,16,AC_MSKB2)//[16:16]
    #define VB1SYS0_LANE_3_EN Fld(1,11,AC_MSKB1)//[11:11]
    #define VB1SYS0_LANE_2_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define VB1SYS0_LANE_1_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define VB1SYS0_LANE_0_EN Fld(1,8,AC_MSKB1)//[8:8]
	    #define VB1SYS0_LANE_EN_MUX Fld(4, 8, AC_MSKB1) //[11:8]
    #define VB1SYS0_FIELDBET Fld(1,6,AC_MSKB0)//[6:6]
    #define VB1SYS0_PATGEN_START Fld(1,5,AC_MSKB0)//[5:5]
    #define VB1SYS0_DE_RST_CTRL Fld(1,4,AC_MSKB0)//[4:4]
    #define VB1SYS0_LANE_NUM Fld(2,2,AC_MSKB0)//[3:2]
    	#define VB1SYS0_LANE_NUM_1 0
    	#define VB1SYS0_LANE_NUM_2 1
    	#define VB1SYS0_LANE_NUM_4 2		
    #define VB1SYS0_COL Fld(2,0,AC_MSKB0)//[1:0]
    	#define VB1SYS0_COL_3BYTES 1
    	#define VB1SYS0_COL_4BYTES 2
    	#define VB1SYS0_COL_5BYTES 3		

#define VB1SYS1 (IO_VIRT + 0x35818)
    #define VB1SYS1_LANE3_MUX Fld(3,16,AC_MSKB2)//[18:16]
    #define VB1SYS1_LANE2_MUX Fld(3,13,AC_MSKB1)//[15:13]
    #define VB1SYS1_LANE1_MUX Fld(3,10,AC_MSKB1)//[12:10]
    #define VB1SYS1_LANE0_MUX Fld(3,7,AC_MSKW10)//[9:7]
    #define VB1SYS1_MON_SEL Fld(5,2,AC_MSKB0)//[6:2]
    #define VB1SYS1_TTL_REORDER Fld(2,0,AC_MSKB0)//[1:0]

#define VB1SYS2 (IO_VIRT + 0x3581C)
    #define VB1SYS2_LANE2_ANA_VALUE Fld(10,20,AC_MSKW32)//[29:20]
    #define VB1SYS2_LANE1_ANA_VALUE Fld(10,10,AC_MSKW21)//[19:10]
    #define VB1SYS2_LANE0_ANA_VALUE Fld(10,0,AC_MSKW10)//[9:0]

#define VB1SYS3 (IO_VIRT + 0x35820)
    #define VB1SYS3_CRC_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define VB1SYS3_LANE_PRBS_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define VB1SYS3_LANE3_ANA Fld(1,27,AC_MSKB3)//[27:27]
    #define VB1SYS3_LANE2_ANA Fld(1,26,AC_MSKB3)//[26:26]
    #define VB1SYS3_LANE1_ANA Fld(1,25,AC_MSKB3)//[25:25]
    #define VB1SYS3_LANE0_ANA Fld(1,24,AC_MSKB3)//[24:24]
    #define VB1SYS3_LANE3_INV Fld(1,19,AC_MSKB2)//[19:19]
    #define VB1SYS3_LANE2_INV Fld(1,18,AC_MSKB2)//[18:18]
    #define VB1SYS3_LANE1_INV Fld(1,17,AC_MSKB2)//[17:17]
    #define VB1SYS3_LANE0_INV Fld(1,16,AC_MSKB2)//[16:16]
    #define VB1SYS3_ANA_RX_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define VB1SYS3_LANE3_ANA_VALUE Fld(10,0,AC_MSKW10)//[9:0]

#define VB1SYS6 (IO_VIRT + 0x3582C)
    #define VB1SYS6_START Fld(1,0,AC_MSKB0)//[0:0]

#define VB1SYS8 (IO_VIRT + 0x35834)
    #define VB1SYS8_CRC4_FAIL Fld(1,19,AC_MSKB2)//[19:19]
    #define VB1SYS8_CRC3_FAIL Fld(1,18,AC_MSKB2)//[18:18]
    #define VB1SYS8_CRC2_FAIL Fld(1,17,AC_MSKB2)//[17:17]
    #define VB1SYS8_CRC1_FAIL Fld(1,16,AC_MSKB2)//[16:16]
    #define VB1SYS8_IMPCAL_VALUE Fld(4,8,AC_MSKB1)//[11:8]
    #define VB1SYS8_IMP_PD Fld(1,7,AC_MSKB0)//[7:7]
    #define VB1SYS8_HOLD Fld(1,6,AC_MSKB0)//[6:6]
    #define VB1SYS8_IMP_CALIB_DONE Fld(1,5,AC_MSKB0)//[5:5]
    #define VB1SYS8_GET_STATE Fld(3,2,AC_MSKB0)//[4:2]
    #define VB1SYS8_LOCKN Fld(1,1,AC_MSKB0)//[1:1]
    #define VB1SYS8_HTDPN Fld(1,0,AC_MSKB0)//[0:0]

#define VB1ANA0 (IO_VIRT + 0x35838)
    #define VB1ANA0_RG_LOOPPAT_2TO1 Fld(1,1,AC_MSKB0)//[1:1]
    #define VB1ANA0_RG_LOOPPAT_EN Fld(1,0,AC_MSKB0)//[0:0]
	
#define VB1RES (IO_VIRT + 0x35840)
    #define VB1RES_DISP_R_INV Fld(1,25,AC_MSKB3)//[25:25]
    #define VB1RES_REG_CONTRL Fld(1,24,AC_MSKB3)//[24:24]
    #define VB1RES_CTRL Fld(24,0,AC_MSKDW)//[23:0]

#define VB1CRC0 (IO_VIRT + 0x35880)
    #define VB1CRC0_CRC0_VALUE Fld(32,0,AC_FULLDW)//[31:0]
    
#define VB1CRC1 (IO_VIRT + 0x35884)
    #define VB1CRC1_CRC1_VALUE Fld(32,0,AC_FULLDW)//[31:0]
    
#define VB1CRC2 (IO_VIRT + 0x35888)
    #define VB1CRC2_CRC2_VALUE Fld(32,0,AC_FULLDW)//[31:0]
    
#define VB1CRC3 (IO_VIRT + 0x3588C)
    #define VB1CRC3_CRC3_VALUE Fld(32,0,AC_FULLDW)//[31:0]


